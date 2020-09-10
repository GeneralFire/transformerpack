/** @file
  Implementation of _DSM functions for NVDIMM in ACPI.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary    and
  confidential information of Intel Corporation and its suppliers and licensors,
  and is protected by worldwide copyright and trade secret laws and treaty
  provisions. No part of the Material may be used, copied, reproduced, modified,
  published, uploaded, posted, transmitted, distributed, or disclosed in any way
  without Intel's prior express written permission.

  No license under any patent, copyright, trade secret or other intellectual
  property right is granted to or conferred upon you by disclosure or delivery
  of the Materials, either expressly, by implication, inducement, estoppel or
  otherwise. Any license under such intellectual property rights must be
  express and approved by Intel in writing.

  Unless otherwise agreed by Intel in writing, you may not remove or alter
  this notice or any other notice embedded in Materials by Intel or
  Intel's suppliers or licensors in any way.

  @par Revision Reference
  NVDIMM DSM Interface version 1.6
**/

#include <Chip/Include/SysHostChip.h>
#include <Protocol/PcatProtocol.h>
#include <Protocol/Dcpmm.h>
#include <Chip/Include/FnvAccessCommon.h>
#include <Library/CpuAndRevisionLib.h>

#include "CrystalRidge.h"
#include "Fis.h"
#include "Dsm.h"
#include "XlateFunctions/XlateFunctions.h"
#include "Ars/ArsFlows.h"
#include "Ars/ArsErrorInject.h"
#include "Ars/ArsFlowsPrivate.h"
#include "Ars/ArsPatrolScrubber.h"
#include "FwActivate.h"


/*****************************************************************************
 * Local definitions
 *****************************************************************************/
#define DSM_DUMP_SIZE                 0 // Max lenght of buffer dumps in DSM SMI handler

#define DSM_MAX_LABEL_DATA_CHUNK_SIZE (MIN (NVDIMM_MAX_INPUT_BUF_SIZE - sizeof (DSM_FNINP_SET_NAMESPACE_LABEL_DATA),\
                                            NVDIMM_MAX_OUTPUT_BUF_SIZE - sizeof (UINT32)) & ~0x1F)
#define DSM_MAX_LP_DATA_CHUNK_SIZE    (MIN (NVDIMM_MAX_INPUT_BUF_SIZE - sizeof (DSM_FNINP_VENDOR_PASSTHROUGH),\
                                            NVDIMM_MAX_OUTPUT_BUF_SIZE - sizeof (DSM_FNOUT_VENDOR_PASSTHROUGH)) & ~0xFF)

#define DSM_DIMMBITMAP_DEFINE(Name)             UINT16 Name[MAX_SOCKET]
#define DSM_DIMMBITMAP_SET(Name, Skt, Ch, Dimm) (Name[(Skt)] |= 1 << ((Ch) * 2 + (Dimm)))
#define DSM_DIMMBITMAP_CLR(Name, Skt, Ch, Dimm) (Name[(Skt)] &= ~1 << ((Ch) * 2 + (Dimm)))
#define DSM_DIMMBITMAP_GET(Name, Skt, Ch, Dimm) (Name[(Skt)] & (1 << ((Ch) * 2 + (Dimm))))

#define ACPI_SMM_BUF_SIZE              0x8000

typedef enum {
  DsmUpdateIdle = 0, // Entry unused.
  DsmUpdateRecv = 1, // Update session started, receiving image data.
  DsmUpdateSent = 2, // Image was sent to NVDIMM, waiting for verification status.
  DsmUpdateDone = 3  // Image sent to NVDIMM and accepted.
} DSM_UPDATE_STAGE;


/*****************************************************************************
 * Function prototypes
 *****************************************************************************/


/*****************************************************************************
 * Variables
 *****************************************************************************/
/**
 * This table handles ongoing NVDIMM FW update sesstions. It is kind of FSM,
 * where DsmFwImageTable[].UpdateStage keeps current state for each NVDIMM.
 */
struct {
  UINT32           DsmHandle;
  UINT32           MaxFwImageSize;
  UINT32           CurFwImageSize;
  DSM_UPDATE_STAGE UpdateStage;
  UINT32           QueryStatus;    // Status to return in DsmQueryFwUpdate()
  UINT8           *FwImagePtr;
} DsmFwImageTable[MAX_DCPMMS];


/**
 * This table keeps in SMM copy of NVDIMM Command Access Policy.
 * The FIS commands originated by DSM SMI handler invoked by ACPI _DSM method
 * should be sent to OS mailbox, but are sent to SMM mailbox because OS mailbox
 * is not usable for SMI handler. The coarse throttling on OS mailbox implemented
 * in NVDIMM FW as 2ms delay after each command makes the OS mailbox not usable
 * for SMI. Thus, to not bypass Command Access Policy on OS mailbox, we have to
 * copy its configuration to SMI handler and apply before sending OS command
 * via SMM mailbox.
 * NOTE: Currently this table keeps all FIS commands and is used by the SMI version of
 *       Vendor Pass-Throught DSM (DsmVendorSpecificCommand()). When the capability of
 *       switching pass-through DSM to SMI is removed from the code this table should be
 *       reduced to just the 9 configurable commands.
 */
struct {
  UINT16 Command;
  UINT8  IsCfgable : 1,    // Is configurable with Command Access Policy
         IsSmmOnly : 1;    // Is statically restricted to SMM mailbox (not configurable)
  UINT8  IsCfgSmm[MAX_SOCKET]; // One bit per DIMM with current configuration of cmd restriction
} DsmFisCmdAccessPolicy[] = {
                             {OVERWRITE_DIMM,                   1, 0},
                             {SET_PASSPHRASE,                   1, 0},
                             {SET_MASTERPASSPHRASE,             1, 0},
                             {SECURE_ERASE_UNIT,                1, 0},
                             {FREEZE_LOCK,                      1, 0},
                             {SET_ALARM_THRESHOLDS,             1, 0},
                             {SET_CONFIG_DATA_POLICY,           1, 0},
                             {SET_ADDRESS_RANGE_SCRUB,          1, 0},
                             {SET_PLATFORM_CONFIG,              1, 0},
                             {SET_LATCH_SYSTEM_SHUTDOWN_STATUS, 1, 0},
                             {UPDATE_FNV_FW,                    1, 0}
                            };


/*****************************************************************************
 * Functions
 *****************************************************************************/
/**
 @brief Read Command Access Policy filter from NVDIMMs.

 @return Void.
**/
VOID
DsmReadFisCmdAccessPolicy (
  VOID
  )
{
  EFI_STATUS Status;
  UINT8      Index;
  UINT8      DimmIdx;
  UINT8      Skt;
  UINT8      Ch;
  UINT8      Dimm;
  FIS_GET_CMD_ACCESS_POLICY_REQ FisReq;
  FIS_GET_CMD_ACCESS_POLICY_RSP FisRsp;

  for (Index = 0; Index < NELEMENTS (DsmFisCmdAccessPolicy); Index++) {

    if (!DsmFisCmdAccessPolicy[Index].IsCfgable) {
      continue;
    }
    ZeroMem (&DsmFisCmdAccessPolicy[Index].IsCfgSmm[0], sizeof (DsmFisCmdAccessPolicy[Index].IsCfgSmm));
    for (DimmIdx = 0; DimmIdx < mCrInfo.NvdimmInfo.NumNvdimms; DimmIdx++) {

      if (mCrInfo.NvdimmInfo.Nvdimms[DimmIdx].SADSpaBase == SPA_NOT_MAPPED ||
          mCrInfo.NvdimmInfo.Nvdimms[DimmIdx].FisVersion < 0x0106) {
        continue;
      }
      Skt = (UINT8)mCrInfo.NvdimmInfo.Nvdimms[DimmIdx].SocketId;
      Ch = mCrInfo.NvdimmInfo.Nvdimms[DimmIdx].Ch;
      Dimm = mCrInfo.NvdimmInfo.Nvdimms[DimmIdx].Dimm;
      FisReq.DWord[0] = 0;
      FisReq.Bits.Command = DsmFisCmdAccessPolicy[Index].Command;
      Status = FisRequest (Skt, Ch, Dimm, &FisReq.DWord[0], sizeof (FisReq), &FisRsp.DWord[0], GET_COMMAND_ACCESS_POLICY, 0, NULL);
      if (!EFI_ERROR (Status)) {

        DsmFisCmdAccessPolicy[Index].IsCfgSmm[Skt] |= (UINT8)(FisRsp.Bits.SmmOnly & 1) << Ch;

      } else {

        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("DSM") "S%d.C%d.D%d: Reading FIS command 0x%04X access policy failed (%r)\n",
                Skt, Ch, Dimm, DsmFisCmdAccessPolicy[Index].Command, Status));
      }
    }
  }
  return;
} // DsmReadFisCmdAccessPolicy()


/**
 @brief Update Command Access Policy filter for NVDIMM.

 @param[in]  Skt       - Socket number
 @param[in]  Ch        - DDR channel number (per socket)
 @param[in]  Command   - Command affected
 @param[in]  IsSmmOnly - The new policy to set

 @return EFI status is returned.
**/
VOID
DsmUpdateFisCmdAccessPolicy (
  IN UINT8   Skt,
  IN UINT8   Ch,
  IN UINT16  Command,
  IN BOOLEAN IsSmmOnly
  )
{
  UINT8      Index;

  ASSERT (Skt < NELEMENTS (DsmFisCmdAccessPolicy[Index].IsCfgSmm));
  ASSERT (Ch < sizeof (DsmFisCmdAccessPolicy[Index].IsCfgSmm[0]) * 8);
  ASSERT ((IsSmmOnly & ~1) == 0);

  for (Index = 0; Index < NELEMENTS (DsmFisCmdAccessPolicy); Index++) {

    if (DsmFisCmdAccessPolicy[Index].Command == Command) {

      if (DsmFisCmdAccessPolicy[Index].IsCfgable) {

        DsmFisCmdAccessPolicy[Index].IsCfgSmm[Skt] &= ~(1 << Ch);
        DsmFisCmdAccessPolicy[Index].IsCfgSmm[Skt] |= (UINT8)IsSmmOnly << Ch;

      } else {

        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("DSM") "Command 0x%04X access policy is not configurable, update refused\n", Command));
      }
      break;
    }
  }
  return;
} // DsmUpdateFisCmdAccessPolicy()


/**
 @brief Check in Command Access Policy whether a FIS command is allowed for OS mailbox.

 This function applies Command Access Policy filter to commands to NVDIMM from
 DSM SWSMI handler. The commands are originated by ACPI _DSM method, but are
 sent to SMM mailbox, because OS mailbox is not usable for SMI handler. The
 coarse throttling on OS mailbox implemented as 2ms delay after each command
 makes this mailbox not usable for SMI. Thus, to preserve Command Access Policy
 on OS mailbox we have to copy its configuration to SMI handler and apply before
 sending OS command via SMM mailbox.

 @param[in]  Skt     - Socket number, or -1 to check for all DIMMs
 @param[in]  Ch      - DDR channel number (per socket), ignored for Skt=-1
 @param[in]  Command - Command to be sent

 @return True if command is allowed for OS mailbox, otherwise false.
**/
BOOLEAN
DsmIsFisCmdAllowedForOsMbox (
  IN UINT8  Skt,
  IN UINT8  Ch,
  IN UINT16 Command
  )
{
  UINT8     Index;

  for (Index = 0; Index < NELEMENTS (DsmFisCmdAccessPolicy); Index++) {

    if (DsmFisCmdAccessPolicy[Index].Command == Command) {

      if (DsmFisCmdAccessPolicy[Index].IsSmmOnly) {
        return FALSE;
      }
      if (DsmFisCmdAccessPolicy[Index].IsCfgable) {

        if (Skt < NELEMENTS (DsmFisCmdAccessPolicy[Index].IsCfgSmm)) {

          return !(DsmFisCmdAccessPolicy[Index].IsCfgSmm[Skt] & (1 << Ch));
        }
        for (Skt = 0; Skt < NELEMENTS (DsmFisCmdAccessPolicy[Index].IsCfgSmm); Skt++) {

          if (DsmFisCmdAccessPolicy[Index].IsCfgSmm[Skt] != 0) {

            return FALSE; /* restricted at least for some DIMMs */
          }
        }
      }
      //
      // No restriction found for the command, so it is allowed.
      //
      return TRUE;
    }
  }
  return TRUE;
} // DsmIsFisCmdAllowedForOsMbox()


/**
 @brief Send a FIS command to NVDIMM but with Access Policy filter applied first.

 @param[in]  Skt           - Socket number
 @param[in]  Ch            - DDR channel number (per socket)
 @param[in]  Dimm          - DIMM number
 @param[in]  InputPayload  - Pointer to buffer containing Input Payload data (NULL if not needed)
 @param[in]  InputLength   - Length of input payload data (may be 0 if no input)
 @param[out] OutputPayload - Pointer to buffer for Output Payload data (NULL if not needed)
 @param[in]  Opcode        - Opcode of the command to be sent
 @param[in]  Timeout       - Command execution timeout in us
 @param[out] FisStsPtr     - Pointer to buffer for FIS status value (NULL if unused)

 @return DCPMM status translated to EFI_STATUS is returned.
**/
EFI_STATUS
DsmFisRequest (
  IN       UINT8  Skt,
  IN       UINT8  Ch,
  IN       UINT8  Dimm,
  IN CONST UINT32 *InputPayload,
  IN       UINT32 InputLength,
  OUT      UINT32 OutputPayload[NUM_PAYLOAD_REG],
  IN       UINT16 Opcode,
  IN       UINT64 Timeout,
  OUT FIS_STATUS *FisStsPtr
  )
{
  BOOLEAN IsCmdAllowed;

  IsCmdAllowed = DsmIsFisCmdAllowedForOsMbox (Skt, Ch, Opcode);
  //
  // SET_PLATFORM_CONFIG needs special care. Writing to PCD0 (BIOS partition) is always blocked and
  // writing to PCD1 (OS partition) is blocked if blocking is enabled in Access Policy.
  //
  if (Opcode == SET_PLATFORM_CONFIG) {

    if (InputLength < sizeof (FIS_SET_PLATFORM_CONFIG_REQ)) {
      return EFI_INVALID_PARAMETER;
    }
    switch (((FIS_SET_PLATFORM_CONFIG_REQ*)InputPayload)->Bits.PartitionId) {

    case BIOS_PARTITION:     // Writing BIOS partition is never allowed for OS mailbox
      IsCmdAllowed = FALSE;
      break;
    case OS_PARTITION:       // Follow the DsmIsFisCmdAllowedForOsMbox() result
      break;
    case OS_PARTITION_OTHER: // Always allowed
      IsCmdAllowed = TRUE;
      break;
    default:
      IsCmdAllowed = TRUE;   // Let NVDIMM return proper error for invalid partition
      break;
    }
  }
  if (!IsCmdAllowed) {

    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "S%d.C%d.D%d: Command 0x%04X rejected due to Access Policy\n",
              Skt, Ch, Dimm, Opcode));
    if (FisStsPtr != 0) {
      *FisStsPtr = FIS_STS_UNSUPPORTED_CMD;
    }
    return EFI_UNSUPPORTED;
  }
  return FisRequest (Skt, Ch, Dimm, InputPayload, InputLength, OutputPayload, Opcode, Timeout, FisStsPtr);
} // DsmFisRequest()


/**

Routine Description: GetDimmInfoFromDevHandle - This function
deconstructs Socket, Channel and Dimm from the Device Handle
that is passed in and returns them in their respective pointers
that are passed in.

  @param *Socket      - pointer to Socket that this function
         will return
  @param *Ch          - pointer to channel that this function
         will return
  @param *Dimm        - Pointer to Dimm that this function will
         return
  @param DevHandle    - 32-bit device handle.
  @retval None - void

**/
VOID
GetDimmInfoFromDevHandle (
  OUT UINT8       *Socket,
  OUT UINT8       *Ch,
  OUT UINT8       *Dimm,
  IN  UINT32      DevHandle
  )
{
  UINT8 Imc;

  // Bit 15:12 of DevHandle contains the socket
  *Socket = (UINT8)((DevHandle >> 12) & 0x0000000F);
  // Bit 11:8 contains the memory controller ID in Nfit DeviceHandle
  Imc = (UINT8)((DevHandle >> 8) & 0x0000000F);
  // Bit 7:4 contains the channel number
  *Ch = (UINT8)((DevHandle >> 4) & 0x0000000F);

  *Ch = ConvertImcChToSktCh (Imc, *Ch);

  *Dimm = (UINT8)(DevHandle & 0x0000000F);
}

UINT32
GetDevHandleFromDimmInfo (
  IN UINT8       Socket,
  IN UINT8       Ch,
  IN UINT8       Dimm
  )
{
  UINT8 Imc;
  UINT8 ImcCh;

  Imc = ConvertSktChToImc (Ch);
  ImcCh = ConvertSktChToImcCh (Ch);
  //
  // Bits 15:12 of DevHandle contain the socket
  // Bits 11:8 contain the memory controller
  // Bits 7:4 contain the channel number
  // Bits 3:0 contain DIMM slot
  //
  return (Socket << 12) | (Imc << 8) | (ImcCh << 4) | Dimm;
}


/**
  Returns current DIMM security state

  @param[in]  Socket   Socket index at system level
  @param[in]  Ch       Channel index at socket level
  @param[in]  Dimm     DIMM slot index within a channel
  @param[out] StatePtr Pointer to security state

  @retval EFI_SUCCESS Success. Security state stored in *State.
  @retval !EFI_SUCESS Failure sending FIS command.

**/
EFI_STATUS
GetDimmSecurityState (
  UINT8   Socket,
  UINT8   Ch,
  UINT8   Dimm,
  UINT32 *StatePtr
  )
{
  EFI_STATUS                 Status;
  FIS_GET_SECURITY_STATE_RSP FisRsp;

  Status = FisRequest (Socket, Ch, Dimm, NULL, 0, &FisRsp.DWord[0], GET_SECURITY_STATE, 0, NULL);
  if (!EFI_ERROR (Status)) {
    *StatePtr = FisRsp.DWord[0];
  }
  return Status;
}

/**
  This function converts NVDIMM FIS or EFI status code to DSM status code.

  @param[in] IsRootDev - TRUE if it is root device DSM, FALSE for leaf device DSM
  @param[in] FisStatus - FIS status returned by FisRequest()
  @param[in] EfiStatus - EFI status code returned from functions called by DSM handler

  @return DSM status code is returned.
**/
UINT16
GetDsmStatusCode (
  IN BOOLEAN     IsRootDev,
  IN FIS_STATUS  FisStatus,
  IN EFI_STATUS  EfiStatus
  )
{
  UINT16 DsmStatus;

  switch (FisStatus) {
  case FIS_STS_SUCCESS:
    DsmStatus = DSM_STS_SUCCESS;
    break;
  case FIS_STS_INVALID_PARAM:
    DsmStatus = (IsRootDev) ? DSM_STS_ROOT_INVALID_PARAMS : DSM_STS_INVALID_PARAMS;
    break;
  case FIS_STS_DATA_XFER_ERR:
    DsmStatus = (IsRootDev) ? DSM_STS_ROOT_RETRY_SUGGESTED : DSM_STS_RETRY_SUGGESTED;
    break;
  case FIS_STS_INTERNAL_ERR:
    DsmStatus = (IsRootDev) ? DSM_STS_ROOT_HW_ERROR : DSM_STS_HW_ERROR;
    break;
  case FIS_STS_UNSUPPORTED_CMD:
    DsmStatus = (IsRootDev) ? DSM_STS_ROOT_NOT_SUPPORTED : DSM_STS_NOT_SUPPORTED;
    break;
  case FIS_STS_DEVICE_BUSY:
    DsmStatus = (IsRootDev) ? DSM_STS_ROOT_RETRY_SUGGESTED : DSM_STS_RETRY_SUGGESTED;
    break;
  case FIS_STS_INCORRECT_NONCE:
    DsmStatus = (IsRootDev) ? DSM_STS_ROOT_UNKNOWN_FAIL : DSM_STS_INVPASSPHRASE;
    break;
  case FIS_STS_INVALID_SEC_STATE:
    DsmStatus = (IsRootDev) ? DSM_STS_ROOT_UNKNOWN_FAIL : DSM_STS_INVSECSTATE;
    break;
  case FIS_STS_NO_NEW_FW:
    DsmStatus = (IsRootDev) ? DSM_STS_ROOT_HW_ERROR : DSM_STS_HW_ERROR;
    break;
  case FIS_STS_REVISION_FAIL:
    DsmStatus = (IsRootDev) ? DSM_STS_ROOT_HW_ERROR : DSM_STS_HW_ERROR;
    break;
  case FIS_STS_INJECT_NOT_ENABLED:
    DsmStatus = (IsRootDev) ? DSM_STS_ROOT_NOT_SUPPORTED : DSM_STS_NOT_SUPPORTED;
    break;
  case FIS_STS_INVALID_ALIGNMENT:
    DsmStatus = (IsRootDev) ? DSM_STS_ROOT_INVALID_PARAMS : DSM_STS_INVALID_PARAMS;
    break;
  case FIS_STS_INCOMPATIBLE_DIMM:
    DsmStatus = (IsRootDev) ? DSM_STS_ROOT_HW_ERROR : DSM_STS_HW_ERROR;
    break;
  case FIS_STS_TIMEOUT:
    DsmStatus = (IsRootDev) ? DSM_STS_ROOT_RETRY_SUGGESTED : DSM_STS_RETRY_SUGGESTED;
    break;
  case FIS_STS_MEDIA_DISABLED:
    DsmStatus = (IsRootDev) ? DSM_STS_ROOT_HW_ERROR : DSM_STS_HW_ERROR;
    break;
  case FIS_STS_FW_UPDATE_ALREADY_OCCURRED:
    DsmStatus = (IsRootDev) ? DSM_STS_ROOT_HW_ERROR : DSM_STS_HW_ERROR;
    break;
  case FIS_STS_NO_RESOURCES_AVAILABLE:
    DsmStatus = (IsRootDev) ? DSM_STS_ROOT_UNKNOWN_FAIL : DSM_STS_OUTOFRESOURCES;
    break;
  case FIS_STS_UNDEFINED:
    switch (EfiStatus) {
    case EFI_SUCCESS:
      DsmStatus = DSM_STS_SUCCESS;
      break;
    case EFI_UNSUPPORTED:
      DsmStatus = (IsRootDev) ? DSM_STS_ROOT_NOT_SUPPORTED : DSM_STS_NOT_SUPPORTED;
      break;
    case EFI_INVALID_PARAMETER:
      DsmStatus = (IsRootDev) ? DSM_STS_ROOT_INVALID_PARAMS : DSM_STS_INVALID_PARAMS;
      break;
    case EFI_DEVICE_ERROR:
    case EFI_NO_MEDIA:
      DsmStatus = (IsRootDev) ? DSM_STS_ROOT_HW_ERROR : DSM_STS_HW_ERROR;
      break;
    case EFI_TIMEOUT:
    case EFI_NOT_READY:
    case EFI_NO_RESPONSE:
      DsmStatus = (IsRootDev) ? DSM_STS_ROOT_RETRY_SUGGESTED : DSM_STS_RETRY_SUGGESTED;
      break;
    case EFI_OUT_OF_RESOURCES:
      DsmStatus = (IsRootDev) ? DSM_STS_ROOT_UNKNOWN_FAIL : DSM_STS_OUTOFRESOURCES;
      break;
    case EFI_NOT_FOUND:
      DsmStatus = (IsRootDev) ? DSM_STS_ROOT_INVALID_PARAMS : DSM_STS_NO_MEM_DEVICE;
      break;
    default:
      DsmStatus = (IsRootDev) ? DSM_STS_ROOT_UNKNOWN_FAIL : DSM_STS_UNKNOWN_FAIL;
      break;
    }
    break;
  default:
    DsmStatus = (IsRootDev) ? DSM_STS_ROOT_UNKNOWN_FAIL : DSM_STS_UNKNOWN_FAIL;
    break;
  }
  return DsmStatus;
}


/**
  This routine copies Command Effects Log Data to the SMM Output buffer.
  A part of the data comes from small payload region and some from
  the large payload region.
  Functions changes mNvdimmAcpiSmmInterface as a side effect.

  @param Socket - Socket index at system level
  @param Ch   - Channel index at socket level
  @param Dimm - DIMM slot index within DDR channel

  @return Status
**/
EFI_STATUS
CopyEffectLogDataToSmmBuffer (
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm
  )
{
  EFI_STATUS      Status;
  NVDIMM          *DimmPtr = NULL;
  CR_MAILBOX      *MBoxPtr;
  UINT32          *DataPtr, LogCount;
  UINT32          *OutBufPtr;

  DimmPtr = GetDimm (&mCrInfo.NvdimmInfo, Socket, Ch, Dimm, NVDIMM_FLAG_ENABLED);

  if (DimmPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  MBoxPtr = GetMailbox (DimmPtr, mUseOSMailbox);

  //
  // First DWord of the small payload region has the number of log entries.
  DataPtr = (UINT32*)(UINTN)MBoxPtr->pOutPayload[0];
  LogCount = *DataPtr;
  OutBufPtr = (UINT32*)&mNvdimmAcpiSmmInterface->OutputBuffer[0];
  *OutBufPtr = LogCount;
  OutBufPtr++;
  if (LogCount * sizeof (UINT64) > NVDIMM_MAX_OUTPUT_BUF_SIZE - (UINT32)((UINT8*)OutBufPtr - mNvdimmAcpiSmmInterface->OutputBuffer)) {
    return EFI_DEVICE_ERROR;
  }

  Status = CopyFromLargePayloadToBuffer (Socket, Ch, Dimm, 0, (UINT8 *)OutBufPtr, 0, (UINT32)(LogCount * sizeof(UINT64)), LARGE_PAYLOAD_OUTPUT);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  //
  // Include LogCount into output length.
  mNvdimmAcpiSmmInterface->OutputLength = (sizeof(UINT64) * LogCount) + sizeof(UINT32);

  return EFI_SUCCESS;
}


/**
  Routine description: DSM method. Function returns adress range scrub
  capabilities (is there an option to run ARS) for given memory region.
  Functions changes mNvdimmAcpiSmmInterface as a side effect.

  @retval EFI_SUCCESS        - command successful
  @retval EFI_INVALID_PARAM  - ARS range out of proper boundaries
 */
EFI_STATUS
DsmQueryArsCapabilities (
  VOID
  )
{
  ROOT_DSM_FNINP_QUERY_ARS_CAPABILITIES *FnInpPtr;
  ROOT_DSM_FNOUT_QUERY_ARS_CAPABILITIES *FnOutPtr;
  UINT64                                ArsStartAddress;
  UINT64                                ArsEndAddress;

  mNvdimmAcpiSmmInterface->OutputLength = sizeof (*FnOutPtr);
  FnOutPtr = (ROOT_DSM_FNOUT_QUERY_ARS_CAPABILITIES *)mNvdimmAcpiSmmInterface->OutputBuffer;
  ZeroMem (FnOutPtr, sizeof (*FnOutPtr));

  if (mNvdimmAcpiSmmInterface->InputLength != sizeof (*FnInpPtr)) {
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "DsmQueryArsCapabilities: invalid input length %d, expected %d\n",
            mNvdimmAcpiSmmInterface->InputLength, sizeof (*FnInpPtr)));
    mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }

  FnInpPtr = (ROOT_DSM_FNINP_QUERY_ARS_CAPABILITIES *)&mNvdimmAcpiSmmInterfaceInputBuffer[0];
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "DsmQueryArsCapabilities: range <0x%llx:0x%llx)\n",
          FnInpPtr->ArsStartAddress,
          FnInpPtr->ArsStartAddress + FnInpPtr->ArsLength));

  if (FnInpPtr->ArsLength == 0) {
    mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }

  ArsStartAddress = FnInpPtr->ArsStartAddress;
  ArsEndAddress = ArsStartAddress + FnInpPtr->ArsLength - 1;

  if (!ArsIsScrubbable (ArsStartAddress, ArsEndAddress)) {
    mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }

  FnOutPtr->MaxQueryArsStatusOutputBufferSize =
    sizeof (mNvdimmAcpiSmmInterface->Status) +
    sizeof (ROOT_DSM_FNOUT_QUERY_ARS_STATUS) +
    MAX_ERROR_LOGS * sizeof (ARS_ERROR_RECORD);

  FnOutPtr->ClearUncorrectableUnitSize = UNCORRECTABLE_UNIT_SIZE;
  FnOutPtr->Flags |= DSM_QUERY_ARS_CAPABILITIES_FLAG_UNCONSUMED_UNCORRECTABLE_NOTIFICATION;
  FnOutPtr->Flags &= ~(DSM_QUERY_ARS_CAPABILITIES_FLAG_ARS_STOPPED_NOTIFICATION);

  if (mSystemMemoryMap->MemSetup.optionsNgn & NGN_ARS_PUBLISH) {
    //
    // Currently we only support scrubbing the PMEM region
    //
    mNvdimmAcpiSmmInterface->Status = CREATE_DSM_EXT_STATUS (DSM_STS_SUCCESS, PMEM_SCRUB_SUPPORTED);

  } else {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_SUCCESS;
  }

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "DsmQueryArsCapabilities: returning %r (interface status 0x%X)\n",
          EFI_SUCCESS, mNvdimmAcpiSmmInterface->Status));
  return EFI_SUCCESS;
}

/**
  Routine description: DSM method. This function handles Start Address Range
  Scrub command. Functions changes mNvdimmAcpiSmmInterface as a side effect.

  @retval EFI_SUCCESS - command successful
  @retval EFI_INVALID_PARAM - ARS range out of proper boundaries or unsupported
                              memory type given
  @retval EFI_ALREADY_STARTED - ARS is in progress
**/
EFI_STATUS
DsmStartArs (
  VOID
  )
{
  ROOT_DSM_FNINP_START_ARS *FnInpPtr;
  ROOT_DSM_FNOUT_START_ARS *FnOutPtr;
  EFI_STATUS               Status;
  UINT64                   ArsStartAddress;
  UINT64                   ArsEndAddress;

  mNvdimmAcpiSmmInterface->OutputLength = sizeof (*FnOutPtr);
  FnOutPtr = (ROOT_DSM_FNOUT_START_ARS *)mNvdimmAcpiSmmInterface->OutputBuffer;
  ZeroMem (FnOutPtr, sizeof (*FnOutPtr));

  if (mNvdimmAcpiSmmInterface->InputLength != sizeof (*FnInpPtr)) {
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "DsmStartArs: invalid input length %d, expected %d\n",
            mNvdimmAcpiSmmInterface->InputLength, sizeof (*FnInpPtr)));

    mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }

  FnInpPtr = (ROOT_DSM_FNINP_START_ARS *)&mNvdimmAcpiSmmInterfaceInputBuffer[0];
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "DsmStartArs: range <0x%llX:0x%llX), type: 0x%04X, flags: 0x%02X\n",
          FnInpPtr->ArsStartAddress,
          FnInpPtr->ArsStartAddress + FnInpPtr->ArsLength,
          FnInpPtr->ArsType,
          FnInpPtr->ArsFlags));

  if ( (FnInpPtr->ArsType & VOLATILE_SCRUB_SUPPORTED) ||
      !(FnInpPtr->ArsType & PMEM_SCRUB_SUPPORTED) ||
        FnInpPtr->ArsLength == 0 ||
      !(mSystemMemoryMap->MemSetup.optionsNgn & NGN_ARS_PUBLISH)) {

    CRDEBUG ((DEBUG_WARN, CR_WARN_STR ("DSM") "DsmStartArs: ARS of type 0x%04X is %a\n", FnInpPtr->ArsType,
            (mSystemMemoryMap->MemSetup.optionsNgn & NGN_ARS_PUBLISH) ? "not supported" : "disabled"));
    mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }

  ArsStartAddress = FnInpPtr->ArsStartAddress;
  ArsEndAddress = ArsStartAddress + FnInpPtr->ArsLength - 1;

  if (!ArsIsScrubbable (ArsStartAddress, ArsEndAddress)) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }

  if (!DsmIsFisCmdAllowedForOsMbox ((UINT8)-1, (UINT8)-1, SET_ADDRESS_RANGE_SCRUB)) {
     //
     // ARS was restricted for some DIMMs to SMM only, refuse DSM request from OS.
     //
    mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_NOT_SUPPORTED;
    return EFI_UNSUPPORTED;
  }

  if (ArsIsInProgress ()) {
    if (DsmStopArsWithOverflow ()) {
      if ((FnInpPtr->ArsFlags & DSM_START_ARS_FLAG_PREVIOUS_SCRUB) == DSM_START_ARS_FLAG_PREVIOUS_SCRUB) {
        if (ArsBackgroundCapable ()) {
          ArsBackgroundEnable ();
        } else {
          ArsAbort ();
          ArsProcessForAllDimms ();
          if (ArsIsInProgress ()) {
            ArsAbort ();
            ArsProcessForAllDimms ();
            if (ArsIsInProgress ()) {
              CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("DSM") "DsmStartArs: ARS still running. Abort failed?\n"));
              mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_FUNCTION_SPECIFIC;
              return EFI_ALREADY_STARTED;
            }
          }
        }
      }
    } else {
      if (!ArsInBackground ()) {
        mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_FUNCTION_SPECIFIC;
        return EFI_ALREADY_STARTED;
      } else {
        if ((FnInpPtr->ArsFlags & DSM_START_ARS_FLAG_PREVIOUS_SCRUB) == 0) {
          ArsAbort ();
          ArsProcessForAllDimms ();
          if (ArsIsInProgress ()) {
            CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("DSM") "DsmStartArs: ARS still running. Abort failed?\n"));
            mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_FUNCTION_SPECIFIC;
            return EFI_ALREADY_STARTED;
          }
          ArsBackgroundDisable ();
        }
      }
    }
  } else {
    if ((ArsInBackground () || (DsmStopArsWithOverflow () && ArsBackgroundCapable () && ArsIsOverflow ())) &&
        ((FnInpPtr->ArsFlags & DSM_START_ARS_FLAG_PREVIOUS_SCRUB) == DSM_START_ARS_FLAG_PREVIOUS_SCRUB)) {
      Status = ArsRestart ();
      if (!EFI_ERROR (Status)) {
        ArsBackgroundEnable ();
      }
    }
  }

  Status = ArsStart (
                     ArsStartAddress,
                     ArsEndAddress,
                     MULTIPLE_INT_SETS,
                     &FnOutPtr->EstimatedTime,
                     (FnInpPtr->ArsFlags & DSM_START_ARS_FLAG_PREVIOUS_SCRUB)
                     );
  // ArsStart modifies internal ARS State so we must return success here
  mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_SUCCESS;
  mNvdimmAcpiSmmInterface->StopArsWithOverflow = 0;
  return EFI_SUCCESS;
}

EFI_STATUS
DsmIntStopArsAndUpdateStatus (
  VOID
  )
{
  if (!ArsIsInProgress ()) {
    return EFI_SUCCESS;
  }
  ArsAbort ();
  if (mNvdimmAcpiSmmInterface->ImplementationControl.NoDdrtSmi == 1) {
    ArsProcessForAllDimms ();

    if (ArsIsInProgress ()) {
      ArsAbort ();
      ArsProcessForAllDimms ();
    }
  }
  return EFI_SUCCESS;
}

/**
  Populates current ARS status in supplied DSM output buffer.

  @param[out]     DsmStatus    Pointer to DSM status
  @param[out]     FnOutPtr     Pointer to DSM output buffer.
  @param[in,out]  FnOutLength  Pointer to length of the DSM output buffer.
                               Caller must populate it with maximal buffer size.
                               DsmGetArsStatus updates the length with consumed buffer size.
**/
STATIC
VOID
DsmGetArsStatus (
  OUT     UINT32                           *DsmStatus,
  OUT     ROOT_DSM_FNOUT_QUERY_ARS_STATUS  *FnOutPtr,
  IN OUT  UINT32                           *FnOutLength,
  IN      BOOLEAN                          UpdateState
  )
{
  EFI_STATUS                      Status;
  ARS_STATE                       *State;
  ARS_ERROR_RECORD                *ArsErrorEntry;
  UINT16                          Index;
  UINT32                          FnOutBufferSize;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("ARS") "DsmGetArsStatus ()\n"));

  FnOutBufferSize = *FnOutLength;
  ASSERT (FnOutBufferSize >= sizeof (*FnOutPtr));
  *FnOutLength = sizeof (*FnOutPtr);
  ZeroMem (FnOutPtr, sizeof (*FnOutPtr));
  FnOutPtr->OutputLength = *FnOutLength + sizeof (UINT32);

  Status = ArsState (&State, UpdateState);
  if (EFI_ERROR (Status)) {

    if (Status != EFI_NOT_READY) {

      ASSERT_EFI_ERROR (Status);
      *DsmStatus = DSM_STS_ROOT_INVALID_PARAMS;
      return;
    }
    //
    // For not found report success.
    //
    *DsmStatus = DSM_STS_EXT_ROOT_ARS_NOT_PERFORMED;
    return;
  }
  if (State->IsInProgress) {

    *DsmStatus = DSM_STS_EXT_ROOT_ARS_IN_PROGRESS;
    return;
  }

  FnOutPtr->Type = PMEM_SCRUB_SUPPORTED;
  FnOutPtr->Flags = State->OverflowCondition ? DSM_ROOT_FLAG_OVERFLOW : 0;

  FnOutPtr->StartSpa = State->StartSpa;
  FnOutPtr->Length = State->EndSpa - State->StartSpa + 1;

  FnOutPtr->RestartSpa = State->RestartSpa;
  FnOutPtr->RestartLength = State->EndSpa - State->RestartSpa + 1;

  *DsmStatus = (State->StoppedPrematurely) ? DSM_STS_EXT_ROOT_ARS_STOPPED_PREMATURELY : DSM_STS_EXT_ROOT_QUERY_ARS_COMPLETED;

  FnOutPtr->NumErrorRecords = State->NumberOfErrorRecords;
  ASSERT (FnOutBufferSize >= *FnOutLength + FnOutPtr->NumErrorRecords * sizeof (ARS_ERROR_RECORD));
  *FnOutLength += FnOutPtr->NumErrorRecords * sizeof (ARS_ERROR_RECORD);
  FnOutPtr->OutputLength = *FnOutLength + sizeof (UINT32);

  ArsErrorEntry = FnOutPtr->ErrorRecords;
  for (Index = 0; Index < FnOutPtr->NumErrorRecords; Index++) {
    ArsErrorEntry->NfitHandle = State->ErrorRecords[Index].NfitHandle;
    ArsErrorEntry->SpaOfErrLoc = State->ErrorRecords[Index].SpaOfErrLoc;
    ArsErrorEntry->Length = State->ErrorRecords[Index].Length;
    ArsErrorEntry->Reserved = 0;
    ArsErrorEntry++;
  }
  return;
}

/**
  Routine description: DSM method. This functions gathers status from the
  previous Start ARS commands for dimms that were participating in the Start
  Address Range Scrub command from the OS and returns Status as
  specified in the DSM Spec.
  Function changes mNvdimmAcpiSmmInterface as a side effect.

  @retval EFI_SUCCESS       - command successful
  @retval EFI_INVALID_PARAM - should not be returned
**/
EFI_STATUS
DsmQueryArsStatus (
  VOID
  )
{
  ROOT_DSM_FNOUT_QUERY_ARS_STATUS *FnOutPtr;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "DsmQueryArsStatus ()\n"));

  if (mNvdimmAcpiSmmInterface->InputLength != 0) {

    mNvdimmAcpiSmmInterface->OutputLength = sizeof (*FnOutPtr);
    FnOutPtr = (ROOT_DSM_FNOUT_QUERY_ARS_STATUS *)mNvdimmAcpiSmmInterface->OutputBuffer;
    ZeroMem (FnOutPtr, sizeof (*FnOutPtr));
    FnOutPtr->OutputLength = mNvdimmAcpiSmmInterface->OutputLength + sizeof (mNvdimmAcpiSmmInterface->Status);
    mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }
  mNvdimmAcpiSmmInterface->OutputLength = sizeof (mNvdimmAcpiSmmInterface->OutputBuffer);
  DsmGetArsStatus (&mNvdimmAcpiSmmInterface->Status, (ROOT_DSM_FNOUT_QUERY_ARS_STATUS *) mNvdimmAcpiSmmInterface->OutputBuffer, &mNvdimmAcpiSmmInterface->OutputLength, TRUE);
  return EFI_SUCCESS;
}

/**
  Populates appropriate ACPI buffer with updated Query ARS Status data.
**/
VOID
DsmUpdateArsStatusData (
  IN BOOLEAN UpdateState
  )
{
  ARS_STATUS_DATA *ArsStatusDataPtr;

  if (mNvdimmAcpiSmmInterface->ArsStatusBufferLock == mNvdimmAcpiSmmInterface->ArsStatusProducedBuffer) {
    mNvdimmAcpiSmmInterface->ArsStatusProducedBuffer = !mNvdimmAcpiSmmInterface->ArsStatusProducedBuffer;
  }
  ArsStatusDataPtr = &mNvdimmAcpiSmmInterface->ArsStatusData[mNvdimmAcpiSmmInterface->ArsStatusProducedBuffer];
  ArsStatusDataPtr->BufferLength = sizeof (ArsStatusDataPtr->Buffer);
  DsmGetArsStatus (&ArsStatusDataPtr->Status,
                   (ROOT_DSM_FNOUT_QUERY_ARS_STATUS *) ArsStatusDataPtr->Buffer,
                   &ArsStatusDataPtr->BufferLength, UpdateState);
}


/**
  Routine description: DSM method. Function initializes Clear Uncorrectable
  Error procedure.
  Functions changes mNvdimmAcpiSmmInterface as a side effect.

  @retval EFI_INVALID_PARAM - address not scrubbable
  @retval EFI_SUCCESS       - command successful
 */
EFI_STATUS
DsmClearUncorrectableError (
  VOID
  )
{
  ROOT_DSM_FNINP_CLEAR_UNCORRECTABLE_ERROR *FnInpPtr;
  ROOT_DSM_FNOUT_CLEAR_UNCORRECTABLE_ERROR *FnOutPtr;
  EFI_STATUS                  Status;
  UINT64                      ClearErrorStartAddress;
  UINT64                      ClearErrorEndAddress;
  UINT64                      ClearErrorStartAddressTemp;
  UINT64                      ClearErrorEndAddressTemp;
  UINT64                      ClearedBlocks = 0;

  mNvdimmAcpiSmmInterface->OutputLength = sizeof (*FnOutPtr);
  FnOutPtr = (ROOT_DSM_FNOUT_CLEAR_UNCORRECTABLE_ERROR *)mNvdimmAcpiSmmInterface->OutputBuffer;
  ZeroMem (FnOutPtr, sizeof (*FnOutPtr));

  if (mNvdimmAcpiSmmInterface->InputLength != sizeof (*FnInpPtr)) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }

  FnInpPtr = (ROOT_DSM_FNINP_CLEAR_UNCORRECTABLE_ERROR *)&mNvdimmAcpiSmmInterfaceInputBuffer[0];
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "DsmClearUncorrectableError: range <0x%llx:0x%llx)\n",
          FnInpPtr->SpaRangeBase,
          FnInpPtr->SpaRangeBase + FnInpPtr->SpaRangeLength));

  ClearErrorStartAddressTemp = ClearErrorStartAddress = FnInpPtr->SpaRangeBase;
  ClearErrorEndAddressTemp = ClearErrorEndAddress = ClearErrorStartAddress + FnInpPtr->SpaRangeLength;

  if (ClearErrorStartAddress % UNCORRECTABLE_UNIT_SIZE != 0 ||
      ClearErrorEndAddress % UNCORRECTABLE_UNIT_SIZE != 0) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }

  if (GetScrubbableInterleaveSets (&ClearErrorStartAddress, &ClearErrorEndAddress, NULL, mCrInfo.NvdimmInfo.NumPMEMRgns) != 1) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }

  if (ClearErrorStartAddress != ClearErrorStartAddressTemp ||
      ClearErrorEndAddress != ClearErrorEndAddressTemp) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }

  if (!DsmIsFisCmdAllowedForOsMbox ((UINT8)-1, (UINT8)-1, INJECT_POISON_ERROR)) {

     Status = EFI_UNSUPPORTED;

  } else {
    if (!ArsInBackground ()) {
      mNvdimmAcpiSmmInterface->StopArsWithOverflow = 1;
      DsmIntStopArsAndUpdateStatus ();
      mNvdimmAcpiSmmInterface->StopArsWithOverflow = 0;
    }
    Status = InjectErrorRange (ClearErrorStartAddress,
                               FnInpPtr->SpaRangeLength / UNCORRECTABLE_UNIT_SIZE,
                               POISON_ENABLE_CLEAR,
                               POISON_MEMORY_TYPE_CLEAR,
                               &ClearedBlocks);
  }
  FnOutPtr->ErrorRangeLength = ClearedBlocks * UNCORRECTABLE_UNIT_SIZE;
  ArsErrorInjectClear (ClearErrorStartAddress, ClearedBlocks, TRUE);

  mNvdimmAcpiSmmInterface->Status = GetDsmStatusCode (TRUE, FIS_STS_UNDEFINED, Status);

  return Status;
}


/**
  Performs ARS Error Inject DSM.

  @retval     EFI_SUCCESS           Injection succeeded.
  @retval     EFI_INVALID_PARAMETER Input validation failed.
                                    Matching SAD rule not found.
                                    Not a PMEM region.
  @retval     <EFI error status>    Failure. No errors injected.
**/
EFI_STATUS
DsmArsErrorInject (
  VOID
  )
{
  EFI_STATUS                      Status;
  ROOT_DSM_FNINP_ARS_ERROR_INJECT *ArsErrorInjectIn;
  UINT64                          ErrorInjectStartAddress;
  UINT64                          ErrorInjectEndAddress;
  UINT64                          ErrorInjectStartAddressTemp;
  UINT64                          ErrorInjectEndAddressTemp;

  UINT8                           Index;
  UINT8                           MemType = POISON_MEMORY_TYPE_APP_DIRECT;
  SPA_INT_SET                     InterleaveSets[MAX_PMEM_INTERLEAVE_SET];
  UINT8                           InterleaveSetsCount = 0;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "DsmArsErrorInject ()\n"));

  mNvdimmAcpiSmmInterface->OutputLength = 0;

  if (mNvdimmAcpiSmmInterface->InputLength != sizeof (ROOT_DSM_FNINP_ARS_ERROR_INJECT)) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }
  ArsErrorInjectIn = (ROOT_DSM_FNINP_ARS_ERROR_INJECT *) &mNvdimmAcpiSmmInterfaceInputBuffer[0];
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "DsmArsErrorInject: range <0x%llx:0x%llx), options: 0x%02x\n",
    ArsErrorInjectIn->SpaRangeBase,
    ArsErrorInjectIn->SpaRangeBase + ArsErrorInjectIn->SpaRangeLength,
    ArsErrorInjectIn->Options));

  ErrorInjectStartAddress = ArsErrorInjectIn->SpaRangeBase;
  ErrorInjectEndAddress = ErrorInjectStartAddress + ArsErrorInjectIn->SpaRangeLength;

  AlignAddressForArs(&ErrorInjectStartAddress, FALSE);
  AlignAddressForArs(&ErrorInjectEndAddress, TRUE);

  ErrorInjectStartAddressTemp = ErrorInjectStartAddress;
  ErrorInjectEndAddressTemp = ErrorInjectEndAddress;

  ZeroMem (InterleaveSets, sizeof (InterleaveSets));

  InterleaveSetsCount = GetScrubbableInterleaveSets (&ErrorInjectStartAddress, &ErrorInjectEndAddress, InterleaveSets, mCrInfo.NvdimmInfo.NumPMEMRgns);

  for (Index = 0; Index < InterleaveSetsCount - 1; Index++) {

    //check if next interleave set is following current interleave set
    if (InterleaveSets[Index].EndSpa != InterleaveSets[Index + 1].StartSpa) {
      mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_INVALID_PARAMS;
      return EFI_INVALID_PARAMETER;
    }
  }

  if (ErrorInjectStartAddress != ErrorInjectStartAddressTemp ||
      ErrorInjectEndAddress != ErrorInjectEndAddressTemp) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }

  if (ArsErrorInjectIn->Options & DSM_ARS_ERROR_INJECT_OPTION_UNCONSUMED_UNCORRECTABLE_NOTIFICATION) {
    MemType = POISON_MEMORY_TYPE_PATROL_SCRUB;
  }
  if (!DsmIsFisCmdAllowedForOsMbox ((UINT8)-1, (UINT8)-1, INJECT_POISON_ERROR)) {

     Status = EFI_UNSUPPORTED;

  } else {

    Status = ArsErrorInject (ErrorInjectStartAddress,
                             (ErrorInjectEndAddress - ErrorInjectStartAddress) / UNCORRECTABLE_UNIT_SIZE,
                             MemType);
  }
  mNvdimmAcpiSmmInterface->Status = GetDsmStatusCode (TRUE, FIS_STS_UNDEFINED, Status);

  return Status;
}

/**
  Performs ARS Error Inject Clear DSM.

  @retval     EFI_SUCCESS           Clearing succeeded.
  @retval     EFI_INVALID_PARAMETER Input validation failed.
                                    Address or length not aligned properly.
  @retval     <EFI error status>    Failure.

**/
EFI_STATUS
DsmArsErrorInjectClear (
  VOID
  )
{
  EFI_STATUS                            Status;
  ROOT_DSM_FNINP_ARS_ERROR_INJECT_CLEAR *ArsErrorInjectClearIn;
  UINT64                                ErrorInjectClearStartAddress;
  UINT64                                ErrorInjectClearEndAddress;
  UINT64                                ErrorInjectClearStartAddressTemp;
  UINT64                                ErrorInjectClearEndAddressTemp;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "DsmArsErrorInjectClear ()\n"));

  mNvdimmAcpiSmmInterface->OutputLength = 0;
  if (mNvdimmAcpiSmmInterface->InputLength != sizeof (ROOT_DSM_FNINP_ARS_ERROR_INJECT_CLEAR)) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }
  ArsErrorInjectClearIn = (ROOT_DSM_FNINP_ARS_ERROR_INJECT_CLEAR *) &mNvdimmAcpiSmmInterfaceInputBuffer[0];
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "DsmArsErrorInjectClear: range <0x%llx:0x%llx)\n",
          ArsErrorInjectClearIn->SpaRangeBase,
          ArsErrorInjectClearIn->SpaRangeBase + ArsErrorInjectClearIn->SpaRangeLength));

  if (ArsErrorInjectClearIn->SpaRangeBase % UNCORRECTABLE_UNIT_SIZE != 0 ||
      ArsErrorInjectClearIn->SpaRangeLength % UNCORRECTABLE_UNIT_SIZE != 0) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }

  ErrorInjectClearStartAddressTemp = ErrorInjectClearStartAddress = ArsErrorInjectClearIn->SpaRangeBase;
  ErrorInjectClearEndAddressTemp = ErrorInjectClearEndAddress = ErrorInjectClearStartAddress + ArsErrorInjectClearIn->SpaRangeLength;

  if (GetScrubbableInterleaveSets (&ErrorInjectClearStartAddress, &ErrorInjectClearEndAddress, NULL, mCrInfo.NvdimmInfo.NumPMEMRgns) == 0) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }

  if (ErrorInjectClearStartAddress != ErrorInjectClearStartAddressTemp ||
      ErrorInjectClearEndAddress != ErrorInjectClearEndAddressTemp) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }

  if (!DsmIsFisCmdAllowedForOsMbox ((UINT8)-1, (UINT8)-1, INJECT_POISON_ERROR)) {

     Status = EFI_UNSUPPORTED;

  } else {

    ErrorInjectClearStartAddress = ArsErrorInjectClearIn->SpaRangeBase;

    Status = ArsErrorInjectClear (ErrorInjectClearStartAddress,
                                  ArsErrorInjectClearIn->SpaRangeLength / UNCORRECTABLE_UNIT_SIZE,
                                  FALSE);
  }
  mNvdimmAcpiSmmInterface->Status = GetDsmStatusCode (TRUE, FIS_STS_UNDEFINED, Status);
  return Status;
}


/**
  Performs ARS Error Inject Status Query DSM.

  @retval     EFI_SUCCESS           Records retreived OK.
  @retval     EFI_INVALID_PARAMETER Input validation failed.

**/
EFI_STATUS
DsmArsErrorInjectStatusQuery (
  VOID
  )
{
  UINT32                                       BufferSize;
  ROOT_DSM_FNOUT_ARS_ERROR_INJECT_STATUS_QUERY *ArsErrorInjectStatusQueryOut;
  UINT32                                       Index;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "DsmArsErrorInjectStatusQuery ()\n"));

  if (mNvdimmAcpiSmmInterface->InputLength != 0) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }

  BufferSize = NVDIMM_MAX_OUTPUT_BUF_SIZE - 4;
  BufferSize -= sizeof (*ArsErrorInjectStatusQueryOut);

  ArsErrorInjectStatusQueryOut = (ROOT_DSM_FNOUT_ARS_ERROR_INJECT_STATUS_QUERY *) mNvdimmAcpiSmmInterface->OutputBuffer;

  ArsErrorInjectStatusQueryOut->ErrorRecordCount =
      ArsErrorInjectStatusQuery (ArsErrorInjectStatusQueryOut->ErrorRecords,
                                BufferSize / sizeof (ArsErrorInjectStatusQueryOut->ErrorRecords[0]));

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "DsmArsErrorInjectStatusQuery: %d records\n",
          ArsErrorInjectStatusQueryOut->ErrorRecordCount));

  for (Index = 0; Index < ArsErrorInjectStatusQueryOut->ErrorRecordCount; Index++) {
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "DsmArsErrorInjectStatusQuery: [%d] <0x%016llx:0x%016llx)[0x%04llx]\n",
      Index,
      ArsErrorInjectStatusQueryOut->ErrorRecords[Index].SpaRangeBase,
      ArsErrorInjectStatusQueryOut->ErrorRecords[Index].SpaRangeBase +
      ArsErrorInjectStatusQueryOut->ErrorRecords[Index].SpaRangeLength,
      ArsErrorInjectStatusQueryOut->ErrorRecords[Index].SpaRangeLength));
  }
  mNvdimmAcpiSmmInterface->OutputLength = ArsErrorInjectStatusQueryOut->ErrorRecordCount *
                                       sizeof (ArsErrorInjectStatusQueryOut->ErrorRecords[0]) +
                                       sizeof (*ArsErrorInjectStatusQueryOut);
  mNvdimmAcpiSmmInterface->Status = DSM_STS_SUCCESS;
  return EFI_SUCCESS;
}


/**
  DSM Method. This function converts SPA to DPA for Ctrl and Pmem regions

  @retval EFI_SUCCESS           - command successful
  @retval EFI_NOT_FOUND         - when dimm is not found
  @retval EFI_INVALID_PARAMETER - if SPA is wrong
**/
EFI_STATUS
DsmTranslateSpa (
  VOID
  )
{
  UINT64                       SpaStart;
  EFI_STATUS                   Status = EFI_SUCCESS;
  UINT64                       Dpa;
  UINT8                        Skt;
  UINT8                        Ch;
  UINT8                        Dimm;
  UINT8                        Imc;
  UINT8                        ImcCh;
  UINT16                       SadType;
  ROOT_DSM_FNOUT_TRANSLATE_SPA *RootDsmFnOutTranslateSpa;
  ROOT_DSM_FNINP_TRANSLATE_SPA *RootDsmFnInpTranslateSpa;
  UINT32                       LineSize;
  UINT32                       NumberOfNvdimms = 1; //default 1 when no mirror
  UINT8                        Flags = 0;           //default 0 when no mirror

  RootDsmFnInpTranslateSpa = (ROOT_DSM_FNINP_TRANSLATE_SPA *) &mNvdimmAcpiSmmInterfaceInputBuffer[0];

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "DsmTranslateSpa start. SPA address: 0x%llx\n", RootDsmFnInpTranslateSpa->Spa));

  RootDsmFnOutTranslateSpa = (ROOT_DSM_FNOUT_TRANSLATE_SPA *) &mNvdimmAcpiSmmInterface->OutputBuffer;
  mNvdimmAcpiSmmInterface->OutputLength = sizeof (ROOT_DSM_FNOUT_TRANSLATE_SPA);
  ZeroMem (RootDsmFnOutTranslateSpa, sizeof (ROOT_DSM_FNOUT_TRANSLATE_SPA));

  if (mNvdimmAcpiSmmInterface->InputLength != sizeof (ROOT_DSM_FNINP_TRANSLATE_SPA)) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }

  Status = SpaToNvmDpa (RootDsmFnInpTranslateSpa->Spa, &Skt, &Ch, &Dimm, &Dpa);
  mNvdimmAcpiSmmInterface->Status = GetDsmStatusCode (TRUE, FIS_STS_UNDEFINED, Status);
  if (EFI_ERROR (Status)) {

    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("DSM") "Translating SPA address 0x%llX to DPA failed (%r)\n",
            RootDsmFnInpTranslateSpa->Spa, Status));
    return Status;
  }

  GetDimmInfoFromSpa (RootDsmFnInpTranslateSpa->Spa, &SpaStart, &Skt, &Ch, &Dimm, &SadType);
  LineSize = GetChGranularityFromSadBaseAddr (SpaStart);
  if (LineSize == 0) {

    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("DSM") "Translating SPA address 0x%llX to DPA failed (zero line size)\n",
            RootDsmFnInpTranslateSpa->Spa));
    mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }

  if (NumberOfNvdimms > 1) {
    Flags |= BIT0;
  }

  Imc = ConvertSktChToImc (Ch);
  ImcCh = ConvertSktChToImcCh (Ch);

  RootDsmFnOutTranslateSpa->Flags = Flags;
  RootDsmFnOutTranslateSpa->NumberOfNvdimms = NumberOfNvdimms;

  RootDsmFnOutTranslateSpa->TranslatedLength = LineSize - ((RootDsmFnInpTranslateSpa->Spa - SpaStart) % LineSize);
  *(UINT32*)&RootDsmFnOutTranslateSpa->TranslatedNvdimmDeviceList[0].NfitHandle =
                                      NFIT_HANDLE_BUILD (Skt, Imc, ImcCh, Dimm);
  RootDsmFnOutTranslateSpa->TranslatedNvdimmDeviceList[0].Reserved = 0;
  RootDsmFnOutTranslateSpa->TranslatedNvdimmDeviceList[0].Dpa = Dpa;

  mNvdimmAcpiSmmInterface->OutputLength = mNvdimmAcpiSmmInterface->OutputLength +
                                       NumberOfNvdimms * sizeof (TRANSLATED_NVDIMM_DEVICE);
  return Status;
}


/**
  @brief DSM method, reads namespace label size from NVDIMM.

  Functions changes mNvdimmAcpiSmmInterface as a side effect.

  @return EFI status is returned and mNvdimmAcpiSmmInterface->Status is set acordingly.
**/
EFI_STATUS
DsmGetNamespaceLabelSize (
  VOID
  )
{
  EFI_STATUS                          Status;
  UINT32                              Size = 0;
  UINT8                               Socket;
  UINT8                               Ch;
  UINT8                               Dimm;
  UINT32                              SecState;
  DSM_FNOUT_GET_NAMESPACE_LABEL_SIZE *FnOutPtr;

  mNvdimmAcpiSmmInterface->OutputLength = 0;
  //
  // Decode DIMM location from _ADR value provided by ACPI in mNvdimmAcpiSmmInterface->DeviceHandle.
  //
  GetDimmInfoFromDevHandle (&Socket, &Ch, &Dimm, mNvdimmAcpiSmmInterface->DeviceHandle);

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "S%d.C%d.D%d: DsmGetNamespaceLabelSize\n", Socket, Ch, Dimm));

  if (GetDimm (&mCrInfo.NvdimmInfo, Socket, Ch, Dimm, NVDIMM_FLAG_ENABLED) == NULL) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_NO_MEM_DEVICE;
    return EFI_INVALID_PARAMETER;
  }
  if (mNvdimmAcpiSmmInterface->InputLength != 0) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }
  //
  // Command Access Policy does not affect access to OS partition so we don't need to check DsmIsFisCmdAllowedForOsMbox()
  //
  Status = GetPlatformConfigDataAreaSize (Socket, Ch, Dimm, OS_PARTITION_OTHER, &Size);
  mNvdimmAcpiSmmInterface->Status = GetDsmStatusCode (FALSE, FIS_STS_UNDEFINED, Status);

  if (EFI_ERROR (Status)) {

    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("DSM"), "GetNamespaceLabelSize: GetPlatformConfigDataAreaSize failed: %r\n", Status));
    return Status;
  }
  FnOutPtr = (DSM_FNOUT_GET_NAMESPACE_LABEL_SIZE*)&mNvdimmAcpiSmmInterface->OutputBuffer[0];
  FnOutPtr->LabelAreaSize = Size;
  FnOutPtr->MaxLabelDataChunkSize = DSM_MAX_LABEL_DATA_CHUNK_SIZE;
  mNvdimmAcpiSmmInterface->OutputLength = sizeof (*FnOutPtr);

  Status = GetDimmSecurityState (Socket, Ch, Dimm, &SecState);

  if (EFI_ERROR (Status) || (SecState & SECURITY_LOCKED)) {

    mNvdimmAcpiSmmInterface->Status = CREATE_DSM_EXT_STATUS (DSM_STS_SUCCESS, DSM_STS_EXT_PMEM_LOCKED);
  }
  //
  // Contact CLV before changing the trace below, test depends on it.
  //
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "GetNamespaceLabelSize: Getting Security State: %r, Security State: %d\n", Status, SecState));
  return EFI_SUCCESS;
}


/**
  @brief DSM method, reads namespace label data from NVDIMM.

  Functions changes mNvdimmAcpiSmmInterface as a side effect.

  @return EFI status is returned and mNvdimmAcpiSmmInterface->Status is set acordingly.
**/
EFI_STATUS
DsmGetNamespaceLabelData (
  VOID
  )
{
  EFI_STATUS                          Status;
  UINT8                               Socket;
  UINT8                               Ch;
  UINT8                               Dimm;
  UINT32                              ChunkSize;
  UINT32                              SecState;
  DSM_FNINP_GET_NAMESPACE_LABEL_DATA *FnInpPtr;
  DSM_FNOUT_GET_NAMESPACE_LABEL_DATA *FnOutPtr;

  mNvdimmAcpiSmmInterface->OutputLength = 0;
  //
  // Decode DIMM location from _ADR value provided by ACPI in mNvdimmAcpiSmmInterface->DeviceHandle.
  //
  GetDimmInfoFromDevHandle (&Socket, &Ch, &Dimm, mNvdimmAcpiSmmInterface->DeviceHandle);

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "S%d.C%d.D%d: DsmGetNamespaceLabelData\n", Socket, Ch, Dimm));

  if (GetDimm (&mCrInfo.NvdimmInfo, Socket, Ch, Dimm, NVDIMM_FLAG_ENABLED) == NULL) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_NO_MEM_DEVICE;
    return EFI_INVALID_PARAMETER;
  }
  FnInpPtr = (DSM_FNINP_GET_NAMESPACE_LABEL_DATA*)&mNvdimmAcpiSmmInterfaceInputBuffer[0];
  FnOutPtr = (DSM_FNOUT_GET_NAMESPACE_LABEL_DATA*)&mNvdimmAcpiSmmInterface->OutputBuffer[0];

  if (mNvdimmAcpiSmmInterface->InputLength != sizeof (*FnInpPtr) ||
      FnInpPtr->ChunkSize > DSM_MAX_LABEL_DATA_CHUNK_SIZE) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }
  Status = GetDimmSecurityState (Socket, Ch, Dimm, &SecState);
  if (EFI_ERROR (Status) || (SecState & SECURITY_LOCKED)) {

    mNvdimmAcpiSmmInterface->Status = CREATE_DSM_EXT_STATUS (DSM_STS_VEN_SPECIFIC_ERROR, DSM_STS_EXT_PMEM_LOCKED);
    return EFI_WRITE_PROTECTED;
  }
  //
  // Command Access Policy does not affect access to OS partition so we don't need to check DsmIsFisCmdAllowedForOsMbox()
  //
  ChunkSize = FnInpPtr->ChunkSize;
  Status = GetPlatformConfigDataAreaData (
               Socket,
               Ch,
               Dimm,
               &FnOutPtr->ChunkData[0],
               &ChunkSize,
               FnInpPtr->ChunkOffset,
               OS_PARTITION_OTHER,
               USE_LARGE_PAYLOAD
               );
  mNvdimmAcpiSmmInterface->Status = GetDsmStatusCode (FALSE, FIS_STS_UNDEFINED, Status);

  if (!EFI_ERROR(Status)) {

    mNvdimmAcpiSmmInterface->OutputLength = ChunkSize;
  }
  return Status;
}


/**

  @brief DSM method, sends namespace label data to NVDIMM

  Functions changes mNvdimmAcpiSmmInterface as a side effect.

  @return EFI status is returned and mNvdimmAcpiSmmInterface->Status is set acordingly.

**/
EFI_STATUS
DsmSetNamespaceLabelData (
  VOID
  )
{
  EFI_STATUS                          Status;
  UINT8                               Socket;
  UINT8                               Ch;
  UINT8                               Dimm;
  UINT32                              SecState;
  DSM_FNINP_SET_NAMESPACE_LABEL_DATA *FnInpPtr;

  mNvdimmAcpiSmmInterface->OutputLength = 0;
  GetDimmInfoFromDevHandle (&Socket, &Ch, &Dimm, mNvdimmAcpiSmmInterface->DeviceHandle);
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "S%d.C%d.D%d: DsmSetNamespaceLabelData\n", Socket, Ch, Dimm));

  if (GetDimm (&mCrInfo.NvdimmInfo, Socket, Ch, Dimm, NVDIMM_FLAG_ENABLED) == NULL) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_NO_MEM_DEVICE;
    return EFI_INVALID_PARAMETER;
  }
  FnInpPtr = (DSM_FNINP_SET_NAMESPACE_LABEL_DATA*)&mNvdimmAcpiSmmInterfaceInputBuffer[0];
  if (mNvdimmAcpiSmmInterface->InputLength < sizeof (*FnInpPtr) ||
      mNvdimmAcpiSmmInterface->InputLength < sizeof (*FnInpPtr) + FnInpPtr->ChunkSize ||
      FnInpPtr->ChunkSize > DSM_MAX_LABEL_DATA_CHUNK_SIZE) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }
  Status = GetDimmSecurityState (Socket, Ch, Dimm, &SecState);

  if (EFI_ERROR (Status) || (SecState & SECURITY_LOCKED)) {

    mNvdimmAcpiSmmInterface->Status = CREATE_DSM_EXT_STATUS (DSM_STS_VEN_SPECIFIC_ERROR, DSM_STS_EXT_PMEM_LOCKED);
    return EFI_WRITE_PROTECTED;
  }
  //
  // Command Access Policy does not affect access to OS partition so we don't need to check DsmIsFisCmdAllowedForOsMbox()
  //
  Status = SetPlatformConfigDataAreaData (
               Socket,
               Ch,
               Dimm,
               &FnInpPtr->ChunkData[0],
               FnInpPtr->ChunkSize,
               FnInpPtr->ChunkOffset,
               OS_PARTITION_OTHER,
               USE_LARGE_PAYLOAD
               );
  mNvdimmAcpiSmmInterface->Status = GetDsmStatusCode (FALSE, FIS_STS_UNDEFINED, Status);

  return Status;
}


/**
  DSM method. Function returns the Command Effect Log for all of
  the Vendor-Specific Commands and sets ACPI structures accordingly.
  Functions changes mNvdimmAcpiSmmInterface as a side effect.
  Now implemented also in CrystalRidge.asi

  @return EFI status returned from GetVendorSpecificCommandEffectLog or
          EFI_INVALID_PARAMETER if Dimm is not found.
**/
EFI_STATUS
DsmGetVendorSpecificCommandEffectLog (
  VOID
  )
{
  EFI_STATUS      Status;
  FIS_STATUS      FisStatus;
  UINT8           Socket;
  UINT8           Ch;
  UINT8           Dimm;

  GetDimmInfoFromDevHandle (&Socket, &Ch, &Dimm, mNvdimmAcpiSmmInterface->DeviceHandle);
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "S%d.C%d.D%d: DsmGetVendorSpecificCommandEffectLog\n", Socket, Ch, Dimm));

  mNvdimmAcpiSmmInterface->OutputLength = 0;

  if (GetDimm (&mCrInfo.NvdimmInfo, Socket, Ch, Dimm, NVDIMM_FLAG_ENABLED) == NULL) {
    mNvdimmAcpiSmmInterface->Status = DSM_STS_NO_MEM_DEVICE;
    return EFI_INVALID_PARAMETER;
  }

  Status = DsmFisRequest (Socket, Ch, Dimm, NULL, 0, NULL, GET_COMMAND_EFFECT_LOG, 0, &FisStatus);
  mNvdimmAcpiSmmInterface->Status = GetDsmStatusCode (FALSE, FisStatus, Status);

  if (EFI_ERROR (Status)) {

    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("DSM") "S%d.C%d.D%d: Reading vendor specific command effect log failed (%r)\n",
      Socket, Ch, Dimm, Status));
    return Status;
  }

  Status = CopyEffectLogDataToSmmBuffer (Socket, Ch, Dimm);
  mNvdimmAcpiSmmInterface->Status = GetDsmStatusCode (FALSE, FIS_STS_UNDEFINED, Status);
  return Status;
}


/**
  DSM method. Function provides access to the vendor specific commands
  sent directly to the DIMM. Part of the commands are not actually for the DIMM
  but are handled in BIOS (BIOS emulated).
  Functions changes mNvdimmAcpiSmmInterface as a side effect.

  @return EFI status returned from VendorSpecificCommand or
          EFI_INVALID_PARAMETER if Dimm is not found.
**/
EFI_STATUS
DsmVendorSpecificCommand (
  VOID
  )
{
  EFI_STATUS                    Status;
  FIS_STATUS                    FisStatus;
  UINT8                         Socket;
  UINT8                         Ch;
  UINT8                         Dimm;
  UINT64                        Timeout = 0;
  UINT32                        *PayloadIn;
  UINT32                        DataSizeHead = 0;
  UINT16                        TransferType = 0;
  UINT16                        OpCode;
  BOOLEAN                       UseOsMailboxBkp = mUseOSMailbox;
  BOOLEAN                       CheckLongOpClash = FALSE;
  NVDIMM                        *DimmPtr;
  DSM_FNINP_VENDOR_PASSTHROUGH  *FnInpPtr;
  DSM_FNOUT_VENDOR_PASSTHROUGH  *FnOutPtr;

  mNvdimmAcpiSmmInterface->OutputLength = 0;
  //
  // Decode DIMM location from _ADR value provided by ACPI in mNvdimmAcpiSmmInterface->DeviceHandle.
  //
  GetDimmInfoFromDevHandle(&Socket, &Ch, &Dimm, mNvdimmAcpiSmmInterface->DeviceHandle);

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "S%d.C%d.D%d: DsmVendorPassThrough\n", Socket, Ch, Dimm));

  FnInpPtr = (DSM_FNINP_VENDOR_PASSTHROUGH*)&mNvdimmAcpiSmmInterfaceInputBuffer[0];
  if (mNvdimmAcpiSmmInterface->InputLength < sizeof(FnInpPtr->Head) || FnInpPtr->Head.Reserved != 0 ||
      mNvdimmAcpiSmmInterface->InputLength != sizeof(FnInpPtr->Head) + FnInpPtr->Head.DataSize) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }

  if (FnInpPtr->Head.OpCode == 0x05FD && FnInpPtr->Data.ExtVenSpec.TransportInterface == FisOverSmbus) {
    DimmPtr = GetDimm (&mCrInfo.NvdimmInfo, Socket, Ch, Dimm, NVDIMM_FLAG_INCLUDE_DISABLED);
  } else {
    DimmPtr = GetDimm (&mCrInfo.NvdimmInfo, Socket, Ch, Dimm, NVDIMM_FLAG_ENABLED);
  }

  if (DimmPtr == NULL) {
    mNvdimmAcpiSmmInterface->Status = DSM_STS_NO_MEM_DEVICE;
    return EFI_INVALID_PARAMETER;
  }

  DataSizeHead = FnInpPtr->Head.DataSize;
  PayloadIn = &FnInpPtr->Data.Fis.Payload[0];
  OpCode = FnInpPtr->Head.OpCode;

  //
  // If BIOS Emulated Command, then handle it in another routine
  //
  if (FIS_OPCODE_GET_MAJOR(OpCode) == DSM_EMULATED_COMMAND) {
    //
    // ASL code use OS mailbox small payload. In SMM we use SMM mailbox. If we access large payload
    // while small is in ASL we should access OS mailbox large payload, because the FIS request
    // will be sent on OS mailbox in ASL.
    //
    mUseOSMailbox = TRUE;

    mNvdimmAcpiSmmInterface->OutputLength = 0;
    //
    // Input date was already validated. Just use it.
    //
    ZeroMem (&mNvdimmAcpiSmmInterface->OutputBuffer[0], sizeof (UINT8) * NVDIMM_MAX_OUTPUT_BUF_SIZE);
    FnOutPtr = (DSM_FNOUT_VENDOR_PASSTHROUGH*)&mNvdimmAcpiSmmInterface->OutputBuffer[0];
    FnOutPtr->Head.DataSize = 0;

    switch (FIS_OPCODE_GET_MINOR (FnInpPtr->Head.OpCode)) {

      case DSM_GET_LP_MB_INFO:
        CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "S%d.C%d.D%d: Get large payload info\n", Socket, Ch, Dimm));
        FnOutPtr->Data.LpInfo.InpPayloadSize = CR_LARGE_PAYLOAD_SIZE; // Mailbox large input payload in bytes
        FnOutPtr->Data.LpInfo.OutPayloadSize = CR_LARGE_PAYLOAD_SIZE; // Mailbox large output payload in bytes
        FnOutPtr->Data.LpInfo.DataChunkSize = ACPI_SMM_BUF_SIZE;
        FnOutPtr->Head.DataSize = sizeof (FnOutPtr->Data.LpInfo);
        mNvdimmAcpiSmmInterface->Status = DSM_STS_SUCCESS;
        Status = EFI_SUCCESS;
        break;

      case DSM_WRITE_LP_INPUT_MB:
        CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "S%d.C%d.D%d: Write large payload %d B at offset 0x%X\n",
               Socket, Ch, Dimm, FnInpPtr->Data.LpWrite.Size, FnInpPtr->Data.LpWrite.Offset));
        if (FnInpPtr->Data.LpWrite.Offset > CR_LARGE_PAYLOAD_SIZE) {

          mNvdimmAcpiSmmInterface->Status = CREATE_DSM_EXT_STATUS (DSM_STS_VEN_SPECIFIC_ERROR, DSM_STS_EXT_INVALID_MB_OFFSET);
          Status = EFI_INVALID_PARAMETER;
          break;
        }
        if (FnInpPtr->Data.LpWrite.Size > DSM_MAX_LP_DATA_CHUNK_SIZE ||
            FnInpPtr->Data.LpWrite.Offset + FnInpPtr->Data.LpWrite.Size > CR_LARGE_PAYLOAD_SIZE) {

          mNvdimmAcpiSmmInterface->Status = CREATE_DSM_EXT_STATUS (DSM_STS_VEN_SPECIFIC_ERROR, DSM_STS_EXT_INVALID_BYTES_TO_XFER);
          Status = EFI_INVALID_PARAMETER;
          break;
        }
        //
        // Copy the provided buffer to the large input payload of the mailbox at the specified offset.
        //
        Status = CopyFromBufferToLargePayload ((UINT8*)&FnInpPtr->Data.LpWrite.Payload[0], 0, Socket, Ch, Dimm,
                                               FnInpPtr->Data.LpWrite.Offset, FnInpPtr->Data.LpWrite.Size, LARGE_PAYLOAD_INPUT);
        ASSERT_EFI_ERROR (Status);

        if (!mUseWpqFlush) {
          Status = CopyFromLargePayloadToBuffer (Socket, Ch, Dimm, FnInpPtr->Data.LpWrite.Offset, mLargePayloadBuffer, 0,
                                                 FnInpPtr->Data.LpWrite.Size, LARGE_PAYLOAD_INPUT);
          ASSERT_EFI_ERROR (Status);
        }
        if (!EFI_ERROR (Status)) {
          mNvdimmAcpiSmmInterface->Status = DSM_STS_SUCCESS;
        } else {
          mNvdimmAcpiSmmInterface->Status = DSM_STS_UNKNOWN_FAIL;
        }
        break;

      case DSM_READ_LP_OUTPUT_MB:
        CRDEBUG ((DEBUG_INFO, CR_INFO_STR("DSM") "S%d.C%d.D%d: Read large payload %d B at offset 0x%X\n",
               Socket, Ch, Dimm, FnInpPtr->Data.LpRead.Size, FnInpPtr->Data.LpRead.Offset));
        if (FnInpPtr->Data.LpRead.Size > DSM_MAX_LP_DATA_CHUNK_SIZE) {

          mNvdimmAcpiSmmInterface->Status = CREATE_DSM_EXT_STATUS (DSM_STS_VEN_SPECIFIC_ERROR, DSM_STS_EXT_ERROR_BUFFER_OVERRUN);
          Status = EFI_INVALID_PARAMETER;
          break;
        }
        if (FnInpPtr->Data.LpRead.Offset > CR_LARGE_PAYLOAD_SIZE) {

          mNvdimmAcpiSmmInterface->Status = CREATE_DSM_EXT_STATUS (DSM_STS_VEN_SPECIFIC_ERROR, DSM_STS_EXT_INVALID_MB_OFFSET);
          Status = EFI_INVALID_PARAMETER;
          break;
        }
        if (FnInpPtr->Data.LpRead.Offset + FnInpPtr->Data.LpRead.Size > CR_LARGE_PAYLOAD_SIZE) {

          mNvdimmAcpiSmmInterface->Status = CREATE_DSM_EXT_STATUS (DSM_STS_VEN_SPECIFIC_ERROR, DSM_STS_EXT_INVALID_BYTES_TO_XFER);
          Status = EFI_INVALID_PARAMETER;
          break;
        }
        Status = CopyFromLargePayloadToBuffer (Socket, Ch, Dimm,
                                               FnInpPtr->Data.LpRead.Offset, (UINT8*)&FnOutPtr->Data.LpData.Payload[0], 0,
                                               FnInpPtr->Data.LpRead.Size, LARGE_PAYLOAD_OUTPUT);
        if (!EFI_ERROR (Status)) {
          FnOutPtr->Head.DataSize = FnInpPtr->Data.LpRead.Size;
          mNvdimmAcpiSmmInterface->Status = DSM_STS_SUCCESS;
        } else {
          mNvdimmAcpiSmmInterface->Status = DSM_STS_UNKNOWN_FAIL;
        }
        break;

      case DSM_EXTENDED_VEN_SPEC:
        CRDEBUG ((DEBUG_INFO, CR_INFO_STR("DFP") "S%d.C%d.D%d: Send Extended Vendor Specific Command via %a with timeout %ums\n",
                  Socket, Ch, Dimm, (FnInpPtr->Data.ExtVenSpec.TransportInterface) ? "SMBUS" : "DDRT", FnInpPtr->Data.ExtVenSpec.Timeout));

        if (FnOutPtr == NULL || FnInpPtr->Head.DataSize < sizeof (FnInpPtr->Data.ExtVenSpec) ||
            FnInpPtr->Data.ExtVenSpec.TransportInterface >= FisIfaceMax ) {
          mNvdimmAcpiSmmInterface->Status = CREATE_DSM_EXT_STATUS (DSM_STS_VEN_SPECIFIC_ERROR, DSM_STS_EXT_ERROR_BUFFER_OVERRUN);
          Status = EFI_INVALID_PARAMETER;
          break;
        }

        OpCode = FnInpPtr->Data.ExtVenSpec.FisCmd;
        DataSizeHead -= 12;
        PayloadIn = &FnInpPtr->Data.ExtVenSpec.Payload[0];
        TransferType = FnInpPtr->Data.ExtVenSpec.TransportInterface;
        Timeout = (UINT64)FnInpPtr->Data.ExtVenSpec.Timeout * 1000;

        if (FIS_OPCODE_GET_MAJOR (OpCode) == DSM_EMULATED_COMMAND) {
          if (FIS_OPCODE_GET_MINOR (OpCode) != DSM_GET_BOOT_STATUS) {
            mNvdimmAcpiSmmInterface->Status = DSM_STS_NOT_SUPPORTED;
            Status = EFI_UNSUPPORTED;
            break;
          }

          Status = FisGetBootStatusRegister (((TransferType == FisOverDdrt) ? FIS_DDRT : FIS_SMBUS), Socket, Ch, Dimm, (FIS_BOOT_STATUS_REGISTER*)&FnOutPtr->Data.Bsr);
          if (!EFI_ERROR (Status)) {
            FnOutPtr->Head.DataSize = sizeof (FnOutPtr->Data.Bsr);
            mNvdimmAcpiSmmInterface->Status = DSM_STS_SUCCESS;
          } else {
            mNvdimmAcpiSmmInterface->Status = DSM_STS_UNKNOWN_FAIL;
          }
        }
        break;

      default:
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("DSM") "S%d.C%d.D%d: Vendor specific opcode 0x%04X not supported\n",
               Socket, Ch, Dimm, FnInpPtr->Head.OpCode));
        mNvdimmAcpiSmmInterface->Status = DSM_STS_NOT_SUPPORTED;
        Status = EFI_UNSUPPORTED;
        break;
    }
    mUseOSMailbox = UseOsMailboxBkp;

    mNvdimmAcpiSmmInterface->OutputLength = sizeof (FnOutPtr->Head) + FnOutPtr->Head.DataSize;

    if (FIS_OPCODE_GET_MAJOR(OpCode) == DSM_EMULATED_COMMAND) {
      return Status;
    }
  }

  if (DataSizeHead > NUM_PAYLOAD_REG * sizeof (UINT32)) {
    mNvdimmAcpiSmmInterface->Status = DSM_STS_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }

  switch (OpCode) {
    case UPDATE_FNV_FW:
      // check if it is a long operation
      if (!(((FIS_UPDATE_FW_REQ*)&FnInpPtr->Data.Fis.Payload[0])->Bits.Header.InSmallPayload &&
          ((FIS_UPDATE_FW_REQ*)&FnInpPtr->Data.Fis.Payload[0])->Bits.Header.Phase != FIS_UPDATEFW_PHASE_END)) {
        CheckLongOpClash = TRUE;
      }
      break;
    case SET_ADDRESS_RANGE_SCRUB:
      // check if it is a long operation
      if (!(FnInpPtr->Head.DataSize >= 1 && (FnInpPtr->Data.Fis.Payload[0] & 0xFF) == 0)) {
        CheckLongOpClash = TRUE;
      }
      break;
    case OVERWRITE_DIMM:
      CheckLongOpClash = TRUE;
      break;
  }

  if (CheckLongOpClash && (FisLongOpRunning[Socket][Ch] != NULL_COMMAND)) {

    NvmCtlrLongOpComplete (Socket, Ch, Dimm);
    if (FisLongOpRunning[Socket][Ch] != NULL_COMMAND && !(ArsInBackground () && FisLongOpRunning[Socket][Ch] == SET_ADDRESS_RANGE_SCRUB)) {

      mNvdimmAcpiSmmInterface->Status = DSM_STS_RETRY_SUGGESTED;
      return EFI_SUCCESS;
    }
  }

  //
  // ASL code use OS mailbox small payload. In SMM we use SMM mailbox because on OS mailbox
  // delay is applied by FW after each request. Here we have to use the OS mailbox because
  // the operation may be be related to data in large payload. It will be only single
  // request here, so delay will not affect this call.
  //
  UseOsMailboxBkp = mUseOSMailbox;
  mUseOSMailbox = TRUE;
  FnOutPtr = (DSM_FNOUT_VENDOR_PASSTHROUGH*)&mNvdimmAcpiSmmInterface->OutputBuffer[0];

  //
  // Input and output buffers from ACPI are alinged to 4 bytes so just pass them to FisRequest() function.
  //
  if (TransferType == FisOverDdrt) {

    Status = DsmFisRequest (Socket, Ch, Dimm, PayloadIn, DataSizeHead, &FnOutPtr->Data.Fis.Payload[0], OpCode, Timeout, &FisStatus);
  } else {

    Status = FisSmbusRequest (Socket, Ch, Dimm, PayloadIn, DataSizeHead, &FnOutPtr->Data.Fis.Payload[0], OpCode, Timeout, &FisStatus);
  }

  mNvdimmAcpiSmmInterface->Status = GetDsmStatusCode (FALSE, FisStatus, Status);
  if (!EFI_ERROR (Status)) {
    //
    // For success always full output payload size is reported. We do not try to check what request
    // it was and how many bytes payload it produce.
    //
    FnOutPtr->Head.DataSize = NUM_PAYLOAD_REG * sizeof (UINT32);
    mNvdimmAcpiSmmInterface->OutputLength = sizeof (FnOutPtr->Head) + FnOutPtr->Head.DataSize;

  } else if (FisStatus != FIS_STS_UNDEFINED) {
    //
    // Provide the status code from mailbox as extended DSM status.
    //
    mNvdimmAcpiSmmInterface->Status = CREATE_DSM_EXT_STATUS (DSM_STS_VEN_SPECIFIC_ERROR, FisStatus);
  }
  mUseOSMailbox = UseOsMailboxBkp;
  return Status;
} // DsmVendorPassThrough()


/**

  DSM method. Function sets latch system shutdown status to FW
  and sets ACPI structures accordingly.
  Functions changes mNvdimmAcpiSmmInterface as a side effect.

  @retval EFI status returned from SetLatchSystemShutdownStatus or
  EFI_INVALID_PARAMETER if Dimm is not found.

**/
EFI_STATUS
DsmEnableLatchSystemShutdownStatus (
  VOID
  )
{
  EFI_STATUS  Status;
  FIS_STATUS  FisStatus;
  UINT8       Socket;
  UINT8       Ch;
  UINT8       Dimm;
  FIS_SET_LATCH_SYSTEM_SHUTDOWN_STATE_REQ        FisReq;
  DSM_FNINP_ENABLE_LATCH_SYSTEM_SHUTDOWN_STATUS *FnInpPtr;

  mNvdimmAcpiSmmInterface->OutputLength = 0;
  //
  // Decode DIMM location from _ADR value provided by ACPI in mNvdimmAcpiSmmInterface->DeviceHandle.
  //
  GetDimmInfoFromDevHandle (&Socket, &Ch, &Dimm, mNvdimmAcpiSmmInterface->DeviceHandle);

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "S%d.C%d.D%d: DsmEnableLatchSystemShutdownStatus\n", Socket, Ch, Dimm));

  if (GetDimm (&mCrInfo.NvdimmInfo, Socket, Ch, Dimm, NVDIMM_FLAG_ENABLED) == NULL) {
    mNvdimmAcpiSmmInterface->Status = DSM_STS_NO_MEM_DEVICE;
    return EFI_INVALID_PARAMETER;
  }
  FnInpPtr = (DSM_FNINP_ENABLE_LATCH_SYSTEM_SHUTDOWN_STATUS*)&mNvdimmAcpiSmmInterfaceInputBuffer[0];
  if (mNvdimmAcpiSmmInterface->InputLength != sizeof(*FnInpPtr) || FnInpPtr->Reserved != 0) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&FisReq, sizeof (FisReq));
  FisReq.Bits.LatchSystemShutdownStateEn = FnInpPtr->LatchSystemShutdownStateEn;

  Status = DsmFisRequest (Socket, Ch, Dimm, &FisReq.DWord[0], sizeof (FisReq), NULL, SET_LATCH_SYSTEM_SHUTDOWN_STATUS, 0, &FisStatus);

  mNvdimmAcpiSmmInterface->Status = GetDsmStatusCode (FALSE, FisStatus, Status);

  return Status;
}


/**
  @brief DSM method, returns NVDIMM firmware info.

  Function changes mNvdimmAcpiSmmInterface as a side effect.

  @return EFI status is returned and mNvdimmAcpiSmmInterface->Status is set acordingly.
**/
EFI_STATUS
DsmGetFwUpdateInfo (VOID)
{
  EFI_STATUS        Status;
  FIS_STATUS        FisStatus;
  UINT8             Socket;
  UINT8             Ch;
  UINT8             Dimm;
  NVDIMM            *DimmPtr;
  FIS_DIMM_IDENTIFY_RSP        DimmInfo;
  FIS_GET_FIRMWARE_INFO_RSP    FwInfo;
  DSM_FNOUT_GET_FWUPDATE_INFO *FnOutPtr;

  mNvdimmAcpiSmmInterface->OutputLength = 0;
  //
  // Decode DIMM location from _ADR value provided by ACPI in mNvdimmAcpiSmmInterface->DeviceHandle.
  //
  GetDimmInfoFromDevHandle (&Socket, &Ch, &Dimm, mNvdimmAcpiSmmInterface->DeviceHandle);

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "S%d.C%d.D%d: DsmGetFwUpdateInfo\n", Socket, Ch, Dimm));

  DimmPtr = GetDimm (&mCrInfo.NvdimmInfo, Socket, Ch, Dimm, NVDIMM_FLAG_ENABLED);
  if (DimmPtr == NULL) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_NO_MEM_DEVICE;
    return EFI_INVALID_PARAMETER;
  }
  if (mNvdimmAcpiSmmInterface->InputLength != 0) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&DimmInfo, sizeof (DimmInfo));
  ZeroMem (&FwInfo, sizeof (FwInfo));

  Status = DsmFisRequest (Socket, Ch, Dimm, NULL, 0, &DimmInfo.DWord[0], IDENTIFY_DIMM, 0, &FisStatus);
  if (!EFI_ERROR (Status)) {

    Status = DsmFisRequest (Socket, Ch, Dimm, NULL, 0, &FwInfo.DWord[0], GET_FIRMWARE_IMAGE, 0, &FisStatus);
  }
  mNvdimmAcpiSmmInterface->Status = GetDsmStatusCode (FALSE, FisStatus, Status);
  if (EFI_ERROR (Status)) {

    return Status;
  }
  if (DimmInfo.Bits.FwRev.ProductNo != FwInfo.Bits.FwRev.ProductNo   ||
      DimmInfo.Bits.FwRev.RevisionNo != FwInfo.Bits.FwRev.RevisionNo ||
      DimmInfo.Bits.FwRev.SecurityNo != FwInfo.Bits.FwRev.SecurityNo ||
      DimmInfo.Bits.FwRev.BuildNo != FwInfo.Bits.FwRev.BuildNo) {

     CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("DSM") "S%d.C%d.D%d: Different version reported in "
              "DIMM Identify and FW Image Info (%02X.%02X.%02X.%04X vs %02X.%02X.%02X.%04X)\n", Socket, Ch, Dimm,
              DimmInfo.Bits.FwRev.ProductNo, DimmInfo.Bits.FwRev.RevisionNo, DimmInfo.Bits.FwRev.SecurityNo, DimmInfo.Bits.FwRev.BuildNo,
              FwInfo.Bits.FwRev.ProductNo, FwInfo.Bits.FwRev.RevisionNo, FwInfo.Bits.FwRev.SecurityNo, FwInfo.Bits.FwRev.BuildNo));
  }
  FnOutPtr = (DSM_FNOUT_GET_FWUPDATE_INFO*)&mNvdimmAcpiSmmInterface->OutputBuffer[0];
  FnOutPtr->FwStorageSize = (DimmInfo.Bits.FisVer < 0x0110) ? DSM_FIS10X_IMGSIZE : 4096 * FwInfo.Bits.MaxFwImageSize;
  FnOutPtr->FwChunkSize = sizeof (mNvdimmAcpiSmmInterface->InputBuffer) - sizeof(DSM_FNINP_FWUPDATE_DATA) - sizeof(UINT64);
  FnOutPtr->QueryInterval = DSM_FISVER2QUERYINTERVAL (DimmInfo.Bits.FisVer);
  FnOutPtr->QueryTimeout = DSM_FISVER2QUERYTIMEOUT (DimmInfo.Bits.FisVer);
  FnOutPtr->ResetColdAfterUpdate = 1;
  FnOutPtr->RuntimeFwActivationWithIoAndProcessorQuiesce = DimmPtr->ActivationData.ActivationEnabled ? 1 : 0;
  FnOutPtr->Reserved0 = 0;
  FnOutPtr->RunningFisVer = DimmInfo.Bits.FisVer;
  FnOutPtr->RunningFwVer = (UINT64)FwInfo.Bits.FwRev.ProductNo << 32 | FwInfo.Bits.FwRev.RevisionNo << 24 |
                                   FwInfo.Bits.FwRev.SecurityNo << 16 | FwInfo.Bits.FwRev.BuildNo;
  FnOutPtr->UpdatedFwVer = (UINT64)FwInfo.Bits.StgFwRev.ProductNo << 32 | FwInfo.Bits.StgFwRev.RevisionNo << 24 |
                                   FwInfo.Bits.StgFwRev.SecurityNo << 16 | FwInfo.Bits.StgFwRev.BuildNo;
  mNvdimmAcpiSmmInterface->OutputLength = sizeof (*FnOutPtr);

  return EFI_SUCCESS;
}


/**
  @brief DSM method, initiates NVDIMM firmware update.

  Function changes mNvdimmAcpiSmmInterface as a side effect.

  @return EFI status is returned and mNvdimmAcpiSmmInterface->Status is set acordingly.
**/
EFI_STATUS
DsmStartFwUpdate (
  VOID
  )
{
  EFI_STATUS        Status;
  FIS_STATUS        FisStatus;
  UINT32            Index;
  UINT32            EmptyIndex;
  UINT32            MaxImageSize;
  UINT8             Socket;
  UINT8             Ch;
  UINT8             Dimm;
  FIS_DIMM_IDENTIFY_RSP     DimmInfo;
  FIS_GET_FIRMWARE_INFO_RSP FwInfo;
  DSM_FNOUT_START_FWUPDATE *FnOutPtr;
  NVDIMM            *DimmPtr;

  mNvdimmAcpiSmmInterface->OutputLength = 0;
  //
  // Decode DIMM location from _ADR value provided by ACPI in mNvdimmAcpiSmmInterface->DeviceHandle.
  //
  GetDimmInfoFromDevHandle (&Socket, &Ch, &Dimm, mNvdimmAcpiSmmInterface->DeviceHandle);

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "S%d.C%d.D%d: DsmStartFwUpdate\n", Socket, Ch, Dimm));

  if ((DimmPtr = GetDimm (&mCrInfo.NvdimmInfo, Socket, Ch, Dimm, NVDIMM_FLAG_ENABLED)) == NULL) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_NO_MEM_DEVICE;
    return EFI_INVALID_PARAMETER;
  }
  if (mNvdimmAcpiSmmInterface->InputLength != 0) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }
  FnOutPtr = (DSM_FNOUT_START_FWUPDATE*)&mNvdimmAcpiSmmInterface->OutputBuffer[0];

  ZeroMem (&DimmInfo, sizeof (DimmInfo));
  ZeroMem (&FwInfo, sizeof (FwInfo));

  Status = DsmFisRequest (Socket, Ch, Dimm, NULL, 0, &DimmInfo.DWord[0], IDENTIFY_DIMM, 0, &FisStatus);
  if (!EFI_ERROR (Status)) {

    Status = DsmFisRequest (Socket, Ch, Dimm, NULL, 0, &FwInfo.DWord[0], GET_FIRMWARE_IMAGE, 0, &FisStatus);
  }
  mNvdimmAcpiSmmInterface->Status = GetDsmStatusCode (FALSE, FisStatus, Status);
  if (EFI_ERROR (Status)) {

    return Status;
  }
  if (DimmPtr->FisVersion < 0x0203 && FwInfo.Bits.LastFwUpdateStatus == FIS_LFUSTATUS_NOTRUN) {
    //
    // New FW already in staging area of NVDIMM. FIS <2.3 will not accept another image until reboot.
    // This case may also happen when new firmware was staged outside of this API.
    //
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("DSM") "S%d.C%d.D%d: New FW already staged (LFUS: %02X)\n",
            Socket, Ch, Dimm, FwInfo.Bits.LastFwUpdateStatus));
    mNvdimmAcpiSmmInterface->Status = DSM_STS_EXT_UPDATE_INPROGRESS;
    return RETURN_ALREADY_STARTED;
  }
  //
  // Browse the updates table. If there is update for this NVDIMM already started return
  // error and the context of the running update. If no update started yet, find first
  // empty slot and allocate memory for the image. FIS version is used to determine if
  // entry is unused. FIS version is never zero.
  //
  EmptyIndex = NELEMENTS (DsmFwImageTable) + 1;
  for (Index = 0; Index < NELEMENTS (DsmFwImageTable); Index++) {

    if (DsmFwImageTable[Index].UpdateStage != DsmUpdateIdle) {

       if (DsmFwImageTable[Index].DsmHandle == mNvdimmAcpiSmmInterface->DeviceHandle) {
         //
         // Update already started for this NVDIMM. Return its context.
         //
         mNvdimmAcpiSmmInterface->Status = DSM_STS_EXT_UPDATE_CONTEXTINV;
         FnOutPtr->Context = Index + 1;
         mNvdimmAcpiSmmInterface->OutputLength = sizeof (*FnOutPtr);
         return EFI_ALREADY_STARTED;
       }
    } else if (EmptyIndex >= NELEMENTS (DsmFwImageTable)) {

      EmptyIndex = Index;
    }
  }
  if (EmptyIndex >= NELEMENTS (DsmFwImageTable)) {
    //
    // Update cannot be started because all slots are occupied by running updates.
    // It should never happen for DsmFwImageTable of MAX_DCPMMS size.
    //
    ASSERT(FALSE);
    mNvdimmAcpiSmmInterface->Status = DSM_STS_OUTOFRESOURCES;
    return EFI_OUT_OF_RESOURCES;
  }
  //
  // Allocate memory for the new image and return slot index + 1 as a context handle.
  // Context is defined as a non zero 32-bit value.
  //
  MaxImageSize = (DimmInfo.Bits.FisVer < 0x0110) ? DSM_FIS10X_IMGSIZE : 4096 * FwInfo.Bits.MaxFwImageSize;
  Status = mSmst->SmmAllocatePool (EfiRuntimeServicesData, MaxImageSize, (VOID**)&DsmFwImageTable[EmptyIndex].FwImagePtr);
  if (EFI_ERROR (Status)) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_OUTOFRESOURCES;
    return EFI_OUT_OF_RESOURCES;
  }
  DsmFwImageTable[EmptyIndex].DsmHandle = mNvdimmAcpiSmmInterface->DeviceHandle;
  DsmFwImageTable[EmptyIndex].MaxFwImageSize = MaxImageSize;
  DsmFwImageTable[EmptyIndex].CurFwImageSize = 0;
  DsmFwImageTable[EmptyIndex].UpdateStage = DsmUpdateRecv;
  DsmFwImageTable[EmptyIndex].QueryStatus = DSM_STS_EXT_UPDATE_OUTOFSEQUENCE;
  FnOutPtr->Context = EmptyIndex + 1;
  mNvdimmAcpiSmmInterface->OutputLength = sizeof (*FnOutPtr);

  return EFI_SUCCESS;
}


/**
  @brief DSM method, collects NVDIMM firmware image chunks.

  @return EFI status is returned and mNvdimmAcpiSmmInterface->Status is set acordingly.
**/
EFI_STATUS
DsmSendFwUpdateData (
  VOID
  )
{
  UINT8   Socket;
  UINT8   Ch;
  UINT8   Dimm;
  UINT32  Index;
  DSM_FNINP_FWUPDATE_DATA *FnInpPtr;

  mNvdimmAcpiSmmInterface->OutputLength = 0;
  //
  // Decode DIMM location from _ADR value provided by ACPI in mNvdimmAcpiSmmInterface->DeviceHandle.
  //
  GetDimmInfoFromDevHandle (&Socket, &Ch, &Dimm, mNvdimmAcpiSmmInterface->DeviceHandle);

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "S%d.C%d.D%d: DsmSendFwUpdateData\n", Socket, Ch, Dimm));

  if (GetDimm (&mCrInfo.NvdimmInfo, Socket, Ch, Dimm, NVDIMM_FLAG_ENABLED) == NULL) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_NO_MEM_DEVICE;
    return EFI_INVALID_PARAMETER;
  }
  FnInpPtr = (DSM_FNINP_FWUPDATE_DATA*)&mNvdimmAcpiSmmInterfaceInputBuffer[0];
  if (mNvdimmAcpiSmmInterface->InputLength < sizeof(*FnInpPtr) ||
      mNvdimmAcpiSmmInterface->InputLength < sizeof(*FnInpPtr) + FnInpPtr->Length) {

    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("DSM") "Wrong input data length %d, expect %d\n",
           mNvdimmAcpiSmmInterface->InputLength, sizeof(*FnInpPtr) + FnInpPtr->Length));
    mNvdimmAcpiSmmInterface->Status = DSM_STS_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }
  Index = FnInpPtr->Context - 1;
  if (Index >= NELEMENTS (DsmFwImageTable) ||
      DsmFwImageTable[Index].DsmHandle != mNvdimmAcpiSmmInterface->DeviceHandle) {

    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("DSM") "S%d.C%d.D%d: Invalid context %d for update data\n",
            Socket, Ch, Dimm, FnInpPtr->Context));
    mNvdimmAcpiSmmInterface->Status = DSM_STS_EXT_UPDATE_CONTEXTINV;
    return EFI_INVALID_PARAMETER;
  }
  if ((UINT64)FnInpPtr->Offset + (UINT64)FnInpPtr->Length > (UINT64)DsmFwImageTable[Index].MaxFwImageSize) {

    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("DSM") "Fragment off: 0x%X len: 0x%X exceeds max image length 0x%X\n",
            FnInpPtr->Offset,  FnInpPtr->Length, DsmFwImageTable[Index].MaxFwImageSize));
    mNvdimmAcpiSmmInterface->Status = DSM_STS_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }
  if (DsmFwImageTable[Index].UpdateStage != DsmUpdateRecv || DsmFwImageTable[Index].FwImagePtr == NULL) {

    ASSERT (DsmFwImageTable[Index].UpdateStage != DsmUpdateRecv); // Make sure we are here because of stage not null pointer
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("DSM") "S%d.C%d.D%d: Cannot receive data, new FW already staged\n", Socket, Ch, Dimm));
    mNvdimmAcpiSmmInterface->Status = DSM_STS_EXT_UPDATE_INPROGRESS;
    return RETURN_ALREADY_STARTED;
  }
  CopyMem (&DsmFwImageTable[Index].FwImagePtr[FnInpPtr->Offset], &FnInpPtr->Data[0], FnInpPtr->Length);

  if (DsmFwImageTable[Index].CurFwImageSize < FnInpPtr->Offset + FnInpPtr->Length) {

    DsmFwImageTable[Index].CurFwImageSize = FnInpPtr->Offset + FnInpPtr->Length;
  }
  mNvdimmAcpiSmmInterface->Status = DSM_STS_SUCCESS;

  return EFI_SUCCESS;
} // DsmSendFwUpdateData()


/**
  @brief DSM method, executes NVDIMM firmware udpate.

  This function only initates update as a FIS long operation. Later calls to
  DsmQueryFwUpdate() will provide actual result of the update.

  @return EFI_STATUS is returned and mNvdimmAcpiSmmInterface->Status is set accordingly.
**/
EFI_STATUS
DsmFinishFwUpdate (
  VOID
  )
{
  EFI_STATUS        Status;
  FIS_STATUS        FisStatus;
  UINT32            Index;
  UINT8             Socket;
  UINT8             Ch;
  UINT8             Dimm;
  NVDIMM            *DimmPtr;
  FIS_UPDATE_FW_REQ          FisReq;
  DSM_FNINP_FINISH_FWUPDATE *FnInpPtr;

  mNvdimmAcpiSmmInterface->OutputLength = 0;
  //
  // Decode DIMM location from _ADR value provided by ACPI in mNvdimmAcpiSmmInterface->DeviceHandle.
  //
  GetDimmInfoFromDevHandle (&Socket, &Ch, &Dimm, mNvdimmAcpiSmmInterface->DeviceHandle);
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "S%d.C%d.D%d: DsmFinishFwUpdate\n", Socket, Ch, Dimm));

  DimmPtr = GetDimm (&mCrInfo.NvdimmInfo, Socket, Ch, Dimm, NVDIMM_FLAG_ENABLED);
  if (DimmPtr == NULL) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_NO_MEM_DEVICE;
    return EFI_INVALID_PARAMETER;
  }
  FnInpPtr = (DSM_FNINP_FINISH_FWUPDATE*)&mNvdimmAcpiSmmInterfaceInputBuffer[0];
  if (mNvdimmAcpiSmmInterface->InputLength != sizeof(*FnInpPtr) ||
      FnInpPtr->Flag.Reserved0 != 0 || FnInpPtr->Reserved1[0] != 0 ||
      FnInpPtr->Reserved1[1] != 0 || FnInpPtr->Reserved1[2] != 0) {

    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("DSM") "S%d.C%d.D%d: Invalid parameters for finish update\n", Socket, Ch, Dimm));
    mNvdimmAcpiSmmInterface->Status = DSM_STS_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }
  Index = FnInpPtr->Context - 1;
  if (Index >= NELEMENTS (DsmFwImageTable) ||
      DsmFwImageTable[Index].DsmHandle != mNvdimmAcpiSmmInterface->DeviceHandle) {

    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("DSM") "S%d.C%d.D%d: Invalid context %d for finish update\n",
            Socket, Ch, Dimm, FnInpPtr->Context));
    mNvdimmAcpiSmmInterface->Status = DSM_STS_EXT_UPDATE_CONTEXTINV;
    return EFI_INVALID_PARAMETER;
  }
  if (DsmFwImageTable[Index].UpdateStage == DsmUpdateSent &&
      mNvdimmAcpiSmmInterface->ImplementationControl.NoDdrtSmi) {
    //
    // If update is in progress for this NVDIMM and we cannot relay on SMI interrupt
    // check long op status.
    //
    NvmCtlrLongOpComplete (Socket, Ch, Dimm);
  }
  if (DsmFwImageTable[Index].UpdateStage != DsmUpdateRecv || DsmFwImageTable[Index].FwImagePtr == NULL) {

    ASSERT (DsmFwImageTable[Index].UpdateStage != DsmUpdateRecv); // Make sure we are here because of stage not null pointer
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("DSM") "S%d.C%d.D%d: New FW already sent to NVDIMM\n", Socket, Ch, Dimm));
    mNvdimmAcpiSmmInterface->Status = DSM_STS_EXT_UPDATE_INPROGRESS;
    return RETURN_DEVICE_ERROR;
  }
  if (FnInpPtr->Flag.Abort) {

    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "S%d.C%d.D%d: Aborting update\n", Socket, Ch, Dimm));
    mSmst->SmmFreePool (DsmFwImageTable[Index].FwImagePtr);
    ZeroMem (&DsmFwImageTable[Index], sizeof (DsmFwImageTable[Index]));
    mNvdimmAcpiSmmInterface->Status = DSM_STS_EXT_UPDATE_ABORTDONE;
    return EFI_SUCCESS;
  }
  if (DsmFwImageTable[Index].CurFwImageSize == 0) {

    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("DSM") "S%d.C%d.D%d: Cannot finish update, no data received\n", Socket, Ch, Dimm));
    mNvdimmAcpiSmmInterface->Status = DSM_STS_EXT_UPDATE_IMAGEAUTHFAIL;
    return EFI_INVALID_PARAMETER;
  }
  if (FisLongOpRunning[Socket][Ch] != NULL_COMMAND) {
    //
    // If some long operation is in progress check if still running.
    // If so return retry-suggested.
    //
    NvmCtlrLongOpComplete (Socket, Ch, Dimm);
    if (FisLongOpRunning[Socket][Ch] != NULL_COMMAND && !(ArsInBackground () && FisLongOpRunning[Socket][Ch] == SET_ADDRESS_RANGE_SCRUB)) {

      mNvdimmAcpiSmmInterface->Status = DSM_STS_RETRY_SUGGESTED;
      return EFI_SUCCESS;
    }
  }
  Status = CopyFromBufferToLargePayload (DsmFwImageTable[Index].FwImagePtr, 0, Socket, Ch, Dimm, 0, DsmFwImageTable[Index].CurFwImageSize, LARGE_PAYLOAD_INPUT);
  if (!mUseWpqFlush) {
    Status = CopyFromBufferToLargePayload (DsmFwImageTable[Index].FwImagePtr, 0, Socket, Ch, Dimm, 0, DsmFwImageTable[Index].CurFwImageSize, LARGE_PAYLOAD_INPUT);
  }
  mNvdimmAcpiSmmInterface->Status = GetDsmStatusCode (FALSE, FIS_STS_UNDEFINED, Status);
  if (!EFI_ERROR (Status)) {
    //
    // Initiate update
    //
    ZeroMem (&FisReq, sizeof (FisReq));  // All zeros in UpdateData.Header initiate transfer via large payload
    Status = DsmFisRequest (Socket, Ch, Dimm, &FisReq.DWord[0], sizeof (FisReq), NULL, UPDATE_FNV_FW, 0, &FisStatus);
    mNvdimmAcpiSmmInterface->Status = GetDsmStatusCode (FALSE, FisStatus, Status);
    if (!EFI_ERROR (Status)) {
      //
      // Update initiated. If it is FIS 1.5 or older it means update is done and we can free buffer.
      // If it is FIS 1.6 or newer, we should wait for interrupt and then retireve long operation status.
      //
      CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "S%d.C%d.D%d: Finishing FW update initiated\n", Socket, Ch, Dimm));
      if (DimmPtr->FisVersion < 0x0106) {

        CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "S%d.C%d.D%d: FW update successful\n", Socket, Ch, Dimm));
        DsmFwImageTable[Index].UpdateStage = DsmUpdateDone;
        DsmFwImageTable[Index].QueryStatus = DSM_STS_SUCCESS;
        mSmst->SmmFreePool (DsmFwImageTable[Index].FwImagePtr);
        DsmFwImageTable[Index].FwImagePtr = NULL;

      } else {

        DsmFwImageTable[Index].UpdateStage = DsmUpdateSent;
        DsmFwImageTable[Index].QueryStatus = DSM_STS_EXT_UPDATE_INPROGRESS;
      }
    } else {

      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("DSM") "S%d.C%d.D%d: Finishing FW update failed (%r)\n", Socket, Ch, Dimm, Status));
      if (FisStatus == FIS_STS_SECURITY_CHK_FAIL) {

        mNvdimmAcpiSmmInterface->Status = DSM_STS_EXT_UPDATE_IMAGEAUTHFAIL;
        DsmFwImageTable[Index].QueryStatus = DSM_STS_EXT_UPDATE_IMAGEAUTHFAIL;
      }
    }
  }
  return Status;
} // DsmFinishFwUpdate()


/**
  @brief DSM method, queries for NVDIMM firmware udpate status.

  @return EFI_STATUS is returned and mNvdimmAcpiSmmInterface->Status is set accordingly.
**/
EFI_STATUS
DsmQueryFwUpdate (
  VOID
  )
{
  EFI_STATUS        Status;
  FIS_STATUS        FisStatus;
  UINT32            Index;
  UINT8             Socket;
  UINT8             Ch;
  UINT8             Dimm;
  FIS_GET_FIRMWARE_INFO_RSP FwInfo;
  DSM_FNINP_QUERY_FWUPDATE *FnInpPtr;
  DSM_FNOUT_QUERY_FWUPDATE *FnOutPtr;

  mNvdimmAcpiSmmInterface->OutputLength = 0;
  //
  // Decode DIMM location from _ADR value provided by ACPI in mNvdimmAcpiSmmInterface->DeviceHandle.
  //
  GetDimmInfoFromDevHandle (&Socket, &Ch, &Dimm, mNvdimmAcpiSmmInterface->DeviceHandle);
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "S%d.C%d.D%d: DsmQueryFwUpdate\n", Socket, Ch, Dimm));

  if (GetDimm(&mCrInfo.NvdimmInfo, Socket, Ch, Dimm, NVDIMM_FLAG_ENABLED) == NULL) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_NO_MEM_DEVICE;
    return EFI_INVALID_PARAMETER;
  }
  FnInpPtr = (DSM_FNINP_QUERY_FWUPDATE*)&mNvdimmAcpiSmmInterfaceInputBuffer[0];
  if (mNvdimmAcpiSmmInterface->InputLength != sizeof(*FnInpPtr)) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }
  //
  // If context is for active session make sure it points to correct DIMM.
  // If context is for inactive session make sure it existed and ended after image sent.
  //
  Index = FnInpPtr->Context - 1;
  if (Index >= NELEMENTS (DsmFwImageTable) ||
      DsmFwImageTable[Index].DsmHandle != mNvdimmAcpiSmmInterface->DeviceHandle) {

    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("DSM") "S%d.C%d.D%d: Invalid context %d for query update\n",
            Socket, Ch, Dimm, FnInpPtr->Context));
    mNvdimmAcpiSmmInterface->Status = DSM_STS_EXT_UPDATE_CONTEXTINV;
    return EFI_INVALID_PARAMETER;
  }
  if (DsmFwImageTable[Index].QueryStatus == DSM_STS_EXT_UPDATE_INPROGRESS &&
      mNvdimmAcpiSmmInterface->ImplementationControl.NoDdrtSmi) {
    //
    // If update is in progress for this NVDIMM and we cannot relay on SMI interrupt
    // check long op status.
    //
    NvmCtlrLongOpComplete (Socket, Ch, Dimm);
  }
  if (DsmFwImageTable[Index].QueryStatus != DSM_STS_SUCCESS) {

    mNvdimmAcpiSmmInterface->Status = DsmFwImageTable[Index].QueryStatus;
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "S%d.C%d.D%d: Query update status returns status 0x%X\n",
            Socket, Ch, Dimm, mNvdimmAcpiSmmInterface->Status));
    return EFI_SUCCESS;
  }
  ZeroMem (&FwInfo, sizeof (FwInfo));
  Status = DsmFisRequest (Socket, Ch, Dimm, NULL, 0, &FwInfo.DWord[0], GET_FIRMWARE_IMAGE, 0, &FisStatus);
  mNvdimmAcpiSmmInterface->Status = GetDsmStatusCode (FALSE, FisStatus, Status);

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "S%d.C%d.D%d: Query update status (%r) got LFUS %d\n",
          Socket, Ch, Dimm, Status, FwInfo.Bits.LastFwUpdateStatus));
  if (EFI_ERROR (Status)) {
    return Status;
  }
  switch (FwInfo.Bits.LastFwUpdateStatus) {

  case FIS_LFUSTATUS_NOTRUN:
    FnOutPtr = (DSM_FNOUT_QUERY_FWUPDATE*)&mNvdimmAcpiSmmInterface->OutputBuffer[0];
    FnOutPtr->UpdatedFwVer = (UINT64) FwInfo.Bits.StgFwRev.ProductNo << 32 | FwInfo.Bits.StgFwRev.RevisionNo << 24 |
      FwInfo.Bits.StgFwRev.SecurityNo << 16 | FwInfo.Bits.StgFwRev.BuildNo;
    mNvdimmAcpiSmmInterface->OutputLength = sizeof (*FnOutPtr);
    return EFI_SUCCESS;

  case FIS_LFUSTATUS_FAILED:
    mNvdimmAcpiSmmInterface->Status = DSM_STS_EXT_UPDATE_IMAGEAUTHFAIL;
    return EFI_LOAD_ERROR;

  case FIS_LFUSTATUS_LOADED:
    if (DsmFwImageTable[Index].UpdateStage == DsmUpdateRecv) {

      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("DSM") "S%d.C%d.D%d: Query update status called for context %d "
              "while Finish was not done yet\n", Socket, Ch, Dimm, FnInpPtr->Context));
      mNvdimmAcpiSmmInterface->Status = DSM_STS_EXT_UPDATE_OUTOFSEQUENCE;
      return EFI_INVALID_PARAMETER;
    }
  default:
    //
    // Finish Update is in progress for this NVDIMM, no status of new image yet.
    //
    mNvdimmAcpiSmmInterface->Status = DSM_STS_EXT_UPDATE_INPROGRESS;
    return EFI_LOAD_ERROR;
  }
} // DsmQueryFwUpdate()


/**
  @brief Callback called when long operation UPDATE_FNV_FW ends and NVDIMM signals it with interrupt.

  This function updates DsmFwImageTable[] entry for DIMM according to update result.

  @return EFI status is returned.
**/
EFI_STATUS
DsmProcessUpdateResults (
  IN UINT8 Skt,
  IN UINT8 Ch,
  IN UINT8 Dimm,
  IN FIS_GET_LONG_OPERATION_STATUS_RSP *FisOpStatusPtr
  )
{
  NVDIMM            *DimmPtr;
  UINT32            Index;
  UINT32            DsmHandle;

  DimmPtr = GetDimm (&mCrInfo.NvdimmInfo, Skt, Ch, Dimm, NVDIMM_FLAG_ENABLED);
  if (DimmPtr == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (FisOpStatusPtr->Bits.Command != UPDATE_FNV_FW) {
    ASSERT (FALSE);
    return EFI_NOT_FOUND;
  }
  DsmHandle = GetDevHandleFromDimmInfo (Skt, Ch, Dimm);

  for (Index = 0; Index < NELEMENTS (DsmFwImageTable); Index++) {

    if (DsmFwImageTable[Index].UpdateStage != DsmUpdateIdle &&
        DsmFwImageTable[Index].DsmHandle == DsmHandle) {

       DsmFwImageTable[Index].QueryStatus = GetDsmStatusCode (FALSE, FisOpStatusPtr->Bits.StatusCode, EFI_SUCCESS);
      if (FisOpStatusPtr->Bits.StatusCode == FIS_STS_SUCCESS) {
        //
        // Image accepted, release memory and we are done, waiting for reboot.
        //
        CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "S%d.C%d.D%d: FW update successful\n", Skt, Ch, Dimm));
        if (DimmPtr->FisVersion >= 0x0203) {
          // FIS 2.3+ allows multiple updates w/o activation/power-cycle
          DsmFwImageTable[Index].UpdateStage = DsmUpdateIdle;
        } else {
          DsmFwImageTable[Index].UpdateStage = DsmUpdateDone;
        }
        mSmst->SmmFreePool (DsmFwImageTable[Index].FwImagePtr);
        DsmFwImageTable[Index].FwImagePtr = NULL;

      } else {
        //
        // Image authentication failed or some generic error, go back to receive stage, user can send new image.
        //
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("DSM") "S%d.C%d.D%d: FW update failed (FIS status: %d)\n",
                Skt, Ch, Dimm, FisOpStatusPtr->Bits.StatusCode));
        DsmFwImageTable[Index].UpdateStage = DsmUpdateRecv;
        if (FisOpStatusPtr->Bits.StatusCode == FIS_STS_SECURITY_CHK_FAIL) {

          DsmFwImageTable[Index].QueryStatus = DSM_STS_EXT_UPDATE_IMAGEAUTHFAIL;
        }
      }
      return EFI_SUCCESS;
    }
  }
  CRDEBUG ((DEBUG_WARN, CR_INFO_STR ("DSM") "S%d.C%d.D%d: No FW update session for this DIMM, updated outside of DSM (FIS status: %d)\n",
          Skt, Ch, Dimm, FisOpStatusPtr->Bits.StatusCode));
  return EFI_NOT_FOUND;
} // DsmProcessUpdateResults()


/**
  @brief DSM method, performs Overwrite NVDIMM.

  Function changes mNvdimmAcpiSmmInterface as a side effect.

  @return EFI_STATUS is returned and mNvdimmAcpiSmmInterface->Status is set accordingly.
**/
EFI_STATUS
DsmOverwriteNvdimm (VOID)
{
  EFI_STATUS        Status;
  FIS_STATUS        FisStatus = FIS_STS_UNDEFINED;
  UINT8             Socket;
  UINT8             Ch;
  UINT8             Dimm;
  FIS_OVERWRITE_DIMM_REQ      FisReq;
  DSM_FNINP_OVERWRITE_NVDIMM *FnInpPtr;
  NVDIMM                     *DimmPtr;

  mNvdimmAcpiSmmInterface->OutputLength = 0;
  //
  // Decode DIMM location from _ADR value provided by ACPI in mNvdimmAcpiSmmInterface->DeviceHandle.
  //
  GetDimmInfoFromDevHandle (&Socket, &Ch, &Dimm, mNvdimmAcpiSmmInterface->DeviceHandle);

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "S%d.C%d.D%d: DsmOverwriteNvdimm\n", Socket, Ch, Dimm));

  DimmPtr = GetDimm (&mCrInfo.NvdimmInfo, Socket, Ch, Dimm, NVDIMM_FLAG_ENABLED);
  if (DimmPtr == NULL) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_NO_MEM_DEVICE;
    return EFI_INVALID_PARAMETER;
  }
  if (mNvdimmAcpiSmmInterface->InputLength != sizeof (*FnInpPtr)) {

    mNvdimmAcpiSmmInterface->Status = DSM_STS_INVALID_PARAMS;
    return EFI_INVALID_PARAMETER;
  }
  switch (mNvdimmAcpiSmmInterface->DimmOverwriteStatus[Socket][Ch]) {

    case FIS_STS_INCOMPATIBLE_DIMM:
      CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "S%d.C%d.D%d: Overwrite DIMM not alowed for 2LM\n", Socket, Ch, Dimm));
      mNvdimmAcpiSmmInterface->Status = DSM_STS_EXT_OVERWRITE_NOTALLOWED;
      return EFI_SUCCESS;

    case FIS_STS_DEVICE_BUSY:
      CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "S%d.C%d.D%d: Overwrite DIMM already started\n", Socket, Ch, Dimm));
      mNvdimmAcpiSmmInterface->Status = DSM_STS_RETRY_SUGGESTED;
      return EFI_SUCCESS;
  }
  if (FisLongOpRunning[Socket][Ch] != NULL_COMMAND) {
    //
    // If some long operation is in progress check if still running.
    // If so return retry-suggested.
    //
    NvmCtlrLongOpComplete (Socket, Ch, Dimm);
    if (FisLongOpRunning[Socket][Ch] != NULL_COMMAND && !(ArsInBackground () && FisLongOpRunning[Socket][Ch] == SET_ADDRESS_RANGE_SCRUB)) {

      mNvdimmAcpiSmmInterface->Status = DSM_STS_RETRY_SUGGESTED;
      return EFI_SUCCESS;
    }
  }
  FnInpPtr = (DSM_FNINP_OVERWRITE_NVDIMM*)&mNvdimmAcpiSmmInterfaceInputBuffer[0];
  ZeroMem (&FisReq, sizeof (FisReq));
  CopyMem (&FisReq.Bits.Passphrase[0], &FnInpPtr->Passphrase[0], sizeof (FisReq.Bits.Passphrase));
  Status = DsmFisRequest (Socket, Ch, Dimm, &FisReq.DWord[0], sizeof (FisReq), NULL, OVERWRITE_DIMM, 0, &FisStatus);
  ZeroMem (&FisReq.Bits.Passphrase[0], sizeof (FisReq.Bits.Passphrase));
  ZeroMem (&FnInpPtr->Passphrase[0], sizeof (FnInpPtr->Passphrase));
  mNvdimmAcpiSmmInterface->Status = GetDsmStatusCode (FALSE, FisStatus, Status);
  if (!EFI_ERROR (Status)) {

    mNvdimmAcpiSmmInterface->DimmOverwriteStatus[Socket][Ch] = FIS_STS_DEVICE_BUSY;
  }
  return Status;
} // DsmOverwriteNvdimm()


/**
  @brief Callback called when long operation OVERWRITE_DIMM ends and NVDIMM signals it with interrupt.

  @return EFI_STATUS is returned.
**/
EFI_STATUS
DsmProcessOverwriteDimmResults (
  IN UINT8 Skt,
  IN UINT8 Ch,
  IN UINT8 Dimm,
  IN FIS_GET_LONG_OPERATION_STATUS_RSP *FisOpStatusPtr
  )
{
  if (FisOpStatusPtr->Bits.Command != OVERWRITE_DIMM) {
    ASSERT (FALSE);
    return EFI_NOT_FOUND;
  }
  if (FisOpStatusPtr->Bits.StatusCode == FIS_STS_DEVICE_BUSY) {
    //
    // Device busy has special meaning for Query Overwrite Status.
    // Replace it with FIS_STS_DATA_XFER_ERR which is also mapped to DSM status RetrySuggested.
    //
    mNvdimmAcpiSmmInterface->DimmOverwriteStatus[Skt][Ch] = FIS_STS_DATA_XFER_ERR;

  } else {

    mNvdimmAcpiSmmInterface->DimmOverwriteStatus[Skt][Ch] = FisOpStatusPtr->Bits.StatusCode;
  }
  CRDEBUG ((DEBUG_WARN, CR_INFO_STR ("DSM") "S%d.C%d.D%d: Overwrite DIMM finished with FIS status: %d\n",
          Skt, Ch, Dimm, FisOpStatusPtr->Bits.StatusCode));
  return EFI_SUCCESS;
} // DsmProcessOverwriteDimmResults()


/**
  @brief Internal DSM method, returns ARS state.

  @return EFI_STATUS is returned and mNvdimmAcpiSmmInterface->Status is set accordingly.
**/
EFI_STATUS
DsmIntGetArsState (VOID)
{
  ARS_ERROR_RECORD *ArsErrTable;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "DsmIntGetArsState()\n"));
  mNvdimmAcpiSmmInterface->Status = DSM_STS_EXT_ROOT_QUERY_ARS_COMPLETED;
  //
  // First read patrol scrubber records to the output buffer.
  //
  mNvdimmAcpiSmmInterface->OutputLength = sizeof(mNvdimmAcpiSmmInterface->OutputBuffer) / sizeof(ARS_ERROR_RECORD);
  ArsErrTable = (ARS_ERROR_RECORD*)&mNvdimmAcpiSmmInterface->OutputBuffer[0];
  ArsPatrolScrubberGet (0, 0xFFFFFFFFFFFFFFFF, ArsErrTable, (UINT16*)&mNvdimmAcpiSmmInterface->OutputLength);
  return EFI_SUCCESS;
} // DsmCheckLongOpState()


/**
  @brief Internal DSM method, internal function to check long op status.

  @return EFI_STATUS is returned and mNvdimmAcpiSmmInterface->Status is set accordingly.
**/
EFI_STATUS
DsmIntCheckLongOpState (VOID)
{
  UINT8             Skt;
  UINT8             Ch;
  UINT8             Dimm;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "DsmIntCheckLongOpState(0x%X)\n",
          mNvdimmAcpiSmmInterface->DeviceHandle));
  mNvdimmAcpiSmmInterface->Status = 0;
  if (mNvdimmAcpiSmmInterface->DeviceHandle == DSM_ROOT_DEVICE_ACPI_HANDLE) {

    ArsProcessForAllDimms ();

  } else {
    //
    // Decode DIMM location from _ADR value provided by ACPI in mNvdimmAcpiSmmInterface->DeviceHandle
    // and check long operation status for this DIMM.
    //
    GetDimmInfoFromDevHandle (&Skt, &Ch, &Dimm, mNvdimmAcpiSmmInterface->DeviceHandle);
    NvmCtlrLongOpComplete (Skt, Ch, Dimm);
  }
  return EFI_SUCCESS;
} // DsmCheckLongOpState()


/**
  Routine description: Address Range Scrub related DSM commands dispatcher.

  @return Status of the DSM function called or EFI_UNSUPPORTED
          if function not supported.
**/
EFI_STATUS
DsmHandleArsCommandsFromOS (
  VOID
  )
{
  EFI_STATUS Status;

  switch (mNvdimmAcpiSmmInterface->FunctionIndex) {
  case DSM_ROOT_QUERY_ARS_CAPABILITES:
    Status = DsmQueryArsCapabilities ();
    break;
  case DSM_ROOT_START_ARS:
    Status = DsmStartArs ();
    break;
  case DSM_ROOT_QUERY_ARS_STATUS:
    Status = DsmQueryArsStatus ();
    break;
  case DSM_ROOT_CLEAR_UNCORRECTABLE_ERROR:
    Status = DsmClearUncorrectableError ();
    break;
  case DSM_ROOT_ARS_ERROR_INJECT:
    Status = DsmArsErrorInject ();
    break;
  case DSM_ROOT_ARS_ERROR_INJECT_CLEAR:
    Status = DsmArsErrorInjectClear ();
    break;
  case DSM_ROOT_ARS_ERROR_INJECT_STATUS_QUERY:
    Status = DsmArsErrorInjectStatusQuery ();
    break;
  case DSM_ROOT_TRANSLATE_SPA:
    Status = DsmTranslateSpa ();
    break;
  case DSM_ROOT_STOP_ARS_AND_UPDATE_STATUS:
    Status = DsmIntStopArsAndUpdateStatus ();
    break;
  case DSM_ROOT_GET_ARS_STATE:
    Status = DsmIntGetArsState ();
    break;
  case DSM_FN_CHECK_LONGOP_STATE:
    Status = DsmIntCheckLongOpState ();
    break;
  default:
    Status = EFI_UNSUPPORTED;
    mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_NOT_SUPPORTED;
    break;
  }
  return Status;
}

EFI_STATUS
DsmHandleIntelFunction (
  VOID
  )
{
  EFI_STATUS Status;

  Status = EFI_UNSUPPORTED;
  mNvdimmAcpiSmmInterface->Status = DSM_STS_ROOT_NOT_SUPPORTED;
  CopyMem (mNvdimmAcpiSmmInterface->OutputBuffer, &mNvdimmAcpiSmmInterface->Status, sizeof (mNvdimmAcpiSmmInterface->Status));
  mNvdimmAcpiSmmInterface->OutputLength = sizeof (mNvdimmAcpiSmmInterface->Status);
  switch (mNvdimmAcpiSmmInterface->FunctionIndex) {
    case DSM_ROOT_INTEL_GET_DEVICES_FW_RUNTIME_ACTIVATION_INFO:
      Status = DsmRootIntelGetDevicesFwRuntimeActivationInfo ();
      break;
    case DSM_ROOT_INTEL_ACTIVATE_FIRMWARE:
      Status = DsmRootIntelActivateFirmware ();
    default:
      break;
  }
  return Status;
}


/**
  SMM callback for all DSM methods.

  @param[in] DispatchHandle - not used
  @param[in] DispatchContext - not used
  @param[in] CommBuffer - not used
  @param[in] CommBufferSize - not used

  @return EFI status returned by DSM methods or
          EFI_UNSUPPORTED if DSM command function index is out of bounds.
**/
EFI_STATUS
EFIAPI
DsmServeNgnCommandsFromOS (
  IN       EFI_HANDLE                   DispatchHandle,
  IN CONST EFI_SMM_SW_REGISTER_CONTEXT  *DispatchContext,
  IN OUT   VOID                         *CommBuffer,     OPTIONAL
  IN OUT   UINTN                        *CommBufferSize  OPTIONAL
  )
{
  EFI_STATUS  Status;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "DsmServePMemCommandsFromOS BEGIN: device: 0x%x, function %d, input len %d\n",
           mNvdimmAcpiSmmInterface->DeviceHandle, mNvdimmAcpiSmmInterface->FunctionIndex, mNvdimmAcpiSmmInterface->InputLength));
  CopyDsmInputBuffer(mNvdimmAcpiSmmInterfaceInputBuffer, &mNvdimmAcpiSmmInterface->InputBuffer, mNvdimmAcpiSmmInterface->InputLength);
#if DSM_DUMP_SIZE
  DisplayBuffer (DEBUG_INFO, &mNvdimmAcpiSmmInterfaceInputBuffer[0],
                 (mNvdimmAcpiSmmInterface->InputLength > DSM_DUMP_SIZE) ? DSM_DUMP_SIZE : mNvdimmAcpiSmmInterface->InputLength);
#endif
  //
  // The encoding is that the device handle for the root device _DSM functions is 0xFFFFFFFF.
  //
  mNvdimmAcpiSmmInterface->OutputLength = 0;
  if (mNvdimmAcpiSmmInterface->DeviceHandle == DSM_ROOT_DEVICE_ACPI_HANDLE) {

    Status = DsmHandleArsCommandsFromOS ();

  } else if (mNvdimmAcpiSmmInterface->DeviceHandle == DSM_ROOT_INTEL_DEVICE_ACPI_HANDLE) {
    Status = DsmHandleIntelFunction ();
  } else {
    switch (mNvdimmAcpiSmmInterface->FunctionIndex) {
    case DSM_FN_GET_LABEL_SIZE:
      Status = DsmGetNamespaceLabelSize ();
      break;
    case DSM_FN_GET_LABEL_DATA:
      Status = DsmGetNamespaceLabelData ();
      break;
    case DSM_FN_SET_LABEL_DATA:
      Status = DsmSetNamespaceLabelData ();
      break;
    case DSM_FN_GET_VENDOR_LOG:
      Status = DsmGetVendorSpecificCommandEffectLog ();
      break;
    case DSM_FN_VENDOR_COMMAND:
      Status = DsmVendorSpecificCommand ();
      break;
    case DSM_FN_ENABLE_LATCH_SYSTEM_SHUTDOWN_STATUS:
      Status = DsmEnableLatchSystemShutdownStatus ();
      break;
    case DSM_FN_GET_FWUPDATE_INFO:
      Status = DsmGetFwUpdateInfo ();
      break;
    case DSM_FN_START_FWUPDATE:
      Status = DsmStartFwUpdate ();
      break;
    case DSM_FN_SEND_FWUPDATE_DATA:
      Status = DsmSendFwUpdateData ();
      break;
    case DSM_FN_FINISH_FWUPDATE:
      Status = DsmFinishFwUpdate ();
      break;
    case DSM_FN_QUERY_FWUPDATE:
      Status = DsmQueryFwUpdate ();
      break;
    case DSM_FN_OVERWRITE_NVDIMM:
      Status = DsmOverwriteNvdimm ();
      break;
    case DSM_FN_GET_DEVICE_RUNTIME_FW_ACTIVATION_INFO:
      Status = DsmGetDeviceRuntimeFwActivationInfo ();
      break;
    case DSM_FN_SET_DEVICE_RUNTIME_FW_ACTIVATION_ARM_STATE:
      Status = DsmSetDeviceRuntimeFwActivationArmState ();
      break;
    case DSM_FN_CHECK_LONGOP_STATE:
      Status = DsmIntCheckLongOpState ();
      break;
    default:
      Status = EFI_UNSUPPORTED;
      mNvdimmAcpiSmmInterface->Status = DSM_STS_NOT_SUPPORTED;
      break;
    }
  }
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DSM") "DsmServePMemCommandsFromOS END: status 0x%X, output len %d\n",
           mNvdimmAcpiSmmInterface->Status, mNvdimmAcpiSmmInterface->OutputLength));
#if DSM_DUMP_SIZE
  DisplayBuffer (DEBUG_INFO, &mNvdimmAcpiSmmInterface->OutputBuffer[0],
                 (mNvdimmAcpiSmmInterface->OutputLength > DSM_DUMP_SIZE) ? DSM_DUMP_SIZE : mNvdimmAcpiSmmInterface->OutputLength);
#endif

  return Status;
}

/**
  Determines if ARS stop should produce overflow condition.

  @return TRUE  ARS stop should always produce overflow condition.
  @return FALSE ARS stop should produce overflow condition only if indicated in FIS response
                by FIS_LONGOP_ARSSTATE_MAX_DPA_ERRORS_ADDRESS_REACHED

**/
BOOLEAN
DsmStopArsWithOverflow (
  VOID
  )
{
  return mNvdimmAcpiSmmInterface->StopArsWithOverflow != 0;
}
