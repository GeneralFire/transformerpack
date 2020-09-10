/** @file
  This file implements DCPMM NVDIMM Firmware Interface protocol.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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
**/

#include <Base.h>
#include <Uefi.h>
#include <PiDxe.h>
#include <Chip/Include/FnvAccessCommon.h>
#include <Protocol/Dcpmm.h>
#include <Library/CrystalRidgeFlushNearMemoryLib.h>

#include "NvdimmSmbusAccess.h"
#include "CrystalRidge.h"
#include "Fis.h"
#include "Dsm.h"


/*****************************************************************************
 * Definitions.
 *****************************************************************************/
//
// The values below define combination of bits in
// LLssExtDetails << 8 | LLssDetails in FIS_GET_SMART_HEALTH_RSP
// that identify type of clean and dirty shutdown reason.
// For dirty shutdown not all bits matter, so mask must be applied first.
// See Intel Optain DC Persistent Memory Module Software RAS and Error Recovery
// Specifiction for details:
// Table 6-5 Power Down Signals and LSS Clean Shutdown Determination
// Table 6-6 Power Down Signals and LSS Dirty Shutdown Determination
//
#define NVM_LSS_REASON_CLEAN_LSS_ADR_POWER_FLOW                0x0499
#define NVM_LSS_REASON_CLEAN_LSS_S3_POWER_FLOW                 0x1492
#define NVM_LSS_REASON_CLEAN_LSS_S4_POWER_FLOW                 0x1C90
#define NVM_LSS_REASON_CLEAN_LSS_S5_POWER_FLOW                 0x1494
#define NVM_LSS_REASON_CLEAN_LSS_IDLE_POWER_FLOW               0x1490
#define NVM_LSS_REASON_CLEAN_LSS_WARM_RESET_FLOW               0x14B0

#define NVM_LSS_REASON_DIRTY_LSS_ADR_STARTED_NO_DDRT_PWR_FAIL_CMD_RECV       0x0091
#define NVM_LSS_REASON_DIRTY_LSS_ADR_STARTED_NO_DDRT_PWR_FAIL_CMD_RECV_MASK  0x19FF
#define NVM_LSS_REASON_DIRTY_LSS_DDRT_PWR_FAIL_CMD_RECV_ADR_NOT_STARTED      0x0098
#define NVM_LSS_REASON_DIRTY_LSS_DDRT_PWR_FAIL_CMD_RECV_ADR_NOT_STARTED_MASK 0x19FF
#define NVM_LSS_REASON_DIRTY_LSS_SURPRISE_PWR_LOSS_AT_NVDIMM                 0x0090
#define NVM_LSS_REASON_DIRTY_LSS_SURPRISE_PWR_LOSS_AT_NVDIMM_MASK            0x19FF
#define NVM_LSS_REASON_DIRTY_LSS_NVDIMM_THERMAL_TRIP                         0x00C0
#define NVM_LSS_REASON_DIRTY_LSS_NVDIMM_THERMAL_TRIP_MASK                    0x3BFF
#define NVM_LSS_REASON_DIRTY_LSS_NVDIMM_VIRAL_TRIGGERED_BEFORE_PWR_DOWN      0x0100
#define NVM_LSS_REASON_DIRTY_LSS_NVDIMM_VIRAL_TRIGGERED_BEFORE_PWR_DOWN_MASK 0x0100
#define NVM_LSS_REASON_DIRTY_LSS_DDRT_SURPRISE_CLOCK_STOP                    0x0290
#define NVM_LSS_REASON_DIRTY_LSS_DDRT_SURPRISE_CLOCK_STOP_MASK               0x1BFF
#define NVM_LSS_REASON_DIRTY_LSS_WRITE_DATA_FLUSH_NOT_COMPLETED              0x0090
#define NVM_LSS_REASON_DIRTY_LSS_WRITE_DATA_FLUSH_NOT_COMPLETED_MASK         0x1DF6
#define NVM_LSS_REASON_DIRTY_LSS_INJECTED_DIRTY_SHUTDOWN                     0x0080
#define NVM_LSS_REASON_DIRTY_LSS_INJECTED_DIRTY_SHUTDOWN_MASK                0x3FFF
#define NVM_LSS_REASON_DIRTY_LSS_NVDIMM_INTERNAL_CONTEXT_FLUSH_FAILED        0x0000
#define NVM_LSS_REASON_DIRTY_LSS_NVDIMM_INTERNAL_CONTEXT_FLUSH_FAILED_MASK   0x0080


/*****************************************************************************
 * Global variables
 *****************************************************************************/
STATIC EFI_DCPMM_PROTOCOL mDcpmmProtocol;

/******************************************************************************
 * Functions
 ******************************************************************************/

/**
  @brief Get NFIT handle and state of next DCPMM NVDIMM.

  This function facilities iterating over NVDIMMs present in the system.

  @param[in,out] NfitHandlePtr  On input handle from former call, or NFIT_HANDLE_NONE for first call.
                                On output NFIT handle of next NVDIMM found.
  @param[out]    NvdimmStatePtr Optional pointer to a buffer for the state of NVDIMM.

  @retval EFI_SUCCESS           Command completed successfully.
  @retval EFI_INVALID_PARAMETER NfitHandlePtr doesn't correspond to a DCPMM NVDIMM.
  @retval EFI_NOT_FOUND         No more DCPMM NVDIMMs found.
**/
EFI_STATUS
EFIAPI
DcpmmNextNvdimm (
  IN OUT UINT32             *NfitHandlePtr,
     OUT DCPMM_NVDIMM_STATE *NvdimmStatePtr
  )
{
  UINT8                      Skt;
  UINT8                      Ch;
  UINT8                      Dimm;
  UINT8                      Imc;
  UINT8                      ImcCh;
  FIS_BOOT_STATUS_REGISTER   Bsr;
  NVDIMM                     *DimmPtr;

  if (NfitHandlePtr == NULL) {

    return EFI_INVALID_PARAMETER;
  }
  if (*NfitHandlePtr == NFIT_HANDLE_NONE) {

    Skt = Ch = Dimm = Imc = ImcCh = 0;

  } else {

    Skt = NFIT_HANDLE_TO_SKT (*NfitHandlePtr);
    Ch = ConvertImcChToSktCh (NFIT_HANDLE_TO_MC (*NfitHandlePtr), NFIT_HANDLE_TO_CH (*NfitHandlePtr));
    Dimm = NFIT_HANDLE_TO_DIMM (*NfitHandlePtr);
    Imc = ConvertSktChToImc (Ch);
    ImcCh = ConvertSktChToImcCh (Ch);
    if (*NfitHandlePtr != NFIT_HANDLE_BUILD (Skt, Imc, ImcCh, Dimm)) {
      //
      // Some of reserved fields in HFIT handle has unexpected value.
      //
      return EFI_INVALID_PARAMETER;
    }
    Dimm++;
  }
  for (; Skt < NELEMENTS (mSystemMemoryMap->Socket); Skt++, Ch = 0) {

    if (!mSystemMemoryMap->Socket[Skt].SocketEnabled) {
      continue;
    }
    for (; Ch < NELEMENTS (mSystemMemoryMap->Socket[0].ChannelInfo); Ch++, Dimm = 0) {

      if (!mSystemMemoryMap->Socket[Skt].ChannelInfo[Ch].Enabled) {
        continue;
      }
      Imc = ConvertSktChToImc (Ch);
      ImcCh = ConvertSktChToImcCh (Ch);
      for (; Dimm < NELEMENTS (mSystemMemoryMap->Socket[0].ChannelInfo[0].DimmInfo); Dimm++) {

        if (!IsNvmDimm (Skt, Ch, Dimm)) {
          continue;
        }
        if (NvdimmStatePtr != NULL) {

          DimmPtr = GetDimm (&mCrInfo.NvdimmInfo, Skt, Ch, Dimm, NVDIMM_FLAG_ALL);
          if (DimmPtr != NULL && DimmPtr->SADSpaBase != SPA_NOT_MAPPED) {

            Bsr.QWord = *(VOLATILE UINT64*)DimmPtr->BsrSpa;
            *NvdimmStatePtr = FNV_MEDIA_IS_USABLE (Bsr) ? NvdimmStateFunctional : NvdimmStateMediaDisabled;

          } else {

            *NvdimmStatePtr = NvdimmStateNotMapped;
            ASSERT (DimmPtr != NULL);
          }
        }
        *NfitHandlePtr = NFIT_HANDLE_BUILD (Skt, Imc, ImcCh, Dimm);
        return EFI_SUCCESS;
      } // for (Dimm...)
    } // for (Ch...)
  } // for (Skt...)
  return EFI_NOT_FOUND;
} // DcpmmNextNvdimm()



/**
  @brief Performs commands defined in DCPMM Firmware Interface Specification.

  This method handles also BIOS emulated commands, with command major code FIS_CMD_EMULATED_MAJOR.
  If FIS command is an emulated BIOS command, it is handled by BIOS, not snt to NVDIMM FW.
  Otherwise the command is handled by the NVDIMM FW (FW command).

  @param[in]     FisIface     FIS interface to use for the request.
  @param[in]     NfitHandle   NFIT handle of the target DCPMM NVDIMM.
  @param[in]     InputPtr     Pointer to a structuree with FIS command opcode and input data.
                              [FW command]   InputPtr->Payload is copied to Small Payload Input.
                              [BIOS command] Consumption is command-specific.
  @param[in,out] OutputPtr    Pointer to a structure prepared for output data.
                              May be NULL if not expecting output payload.
                              [FW command]   Small Payload Output is stored in output data
                                             if mailbox command has executed successfully.
                              [BIOS command] Use of Output is command-specific.
                              In OutputPtr->Head.DataSize the size of output buffer is provided on entry
                              and on exit the size of returned data is provided.
  @param[in]     Timeout      [FW command]   Optional timeout in us to use for FIS command
                              [BIOS command] Always ignored.
                              If set to zero then command specific default timeout is calculated.
  @param[out]    MboxStsPtr   [FW command]   Optional pointer to store FW Mailbox Status Code under.
                              [BIOS command] Always ignored.
                              Ignored if NULL.

  @retval  EFI_SUCCESS            Command completed successfully.
                                  [FW command]   FW MB Status Code is zero.
  @retval  EFI_INVALID_PARAMETER  NfitHandle doesn't correspond to a DCPMM NVDIMM.
                                  [FW command]   InputSize is larger than controller's Small Payload Input size.
                                  [FW command]   OutputSize is larger than controller's Small Payload Output size.
                                  [BIOS command] InputSize not valid for specified FisCmd.
                                  [BIOS command] OutputSize is not valid for specified CommandOpCode.
  @retval  EFI_NOT_SUPPORTED      [BIOS command] Unknown Mailbox Sub-Opcode
  @retval  EFI_NOT_READY          [FW command]   Mailbox not ready for command.
                                                 Mailbox status not updated.
  @retval  EFI_DEVICE_ERROR       [FW command]   Mailbox command did not complete. Device error.
                                                 MailboxStatus not updated.
  @retval  EFI_TIMEOUT            [FW command]   Mailbox command did not complete in time allotted.
                                                 Mailbox status not updated.
  @retval  EFI_PROTOCOL_ERROR     [FW command]   Mailbox command completed with non-zero FW MB Status Code
**/
EFI_STATUS
EFIAPI
DcpmmFisRequest (
  IN     DCPMM_FIS_INTERFACE FisIface,
  IN     UINT32              NfitHandle,
  IN     DCPMM_FIS_INPUT    *InputPtr,
  IN OUT DCPMM_FIS_OUTPUT   *OutputPtr,
  IN     UINT32              Timeout,
     OUT UINT8              *MboxStsPtr
  )
{
  UINT32 OutputPayload[NUM_PAYLOAD_REG];

  EFI_STATUS Status;
  FIS_STATUS FisSts;
  UINT8      Skt;
  UINT8      Ch;
  UINT8      Dimm;
  UINT8      Imc;
  UINT8      ImcCh;
  NVDIMM     *DimmPtr;

  //
  // Validate input parameters
  //
  if ((FisIface >= FisIfaceMax) || (InputPtr == NULL) || ((OutputPtr != NULL) && (OutputPtr->Head.DataSize == 0))) {
    return EFI_INVALID_PARAMETER;
  }

  if (MboxStsPtr != NULL) {
    *MboxStsPtr = FisSts = FIS_STS_UNDEFINED;
  }

  Skt = NFIT_HANDLE_TO_SKT (NfitHandle);
  Ch = ConvertImcChToSktCh (NFIT_HANDLE_TO_MC (NfitHandle), NFIT_HANDLE_TO_CH (NfitHandle));
  Dimm = NFIT_HANDLE_TO_DIMM (NfitHandle);
  Imc = ConvertSktChToImc (Ch);
  ImcCh = ConvertSktChToImcCh (Ch);
  if (NfitHandle != NFIT_HANDLE_BUILD (Skt, Imc, ImcCh, Dimm)) {
    //
    // Some of reserved fields in HFIT handle has unexpected value.
    //
    return EFI_INVALID_PARAMETER;
  }

  //
  // DDRT interface selected -> look only for enabled DIMMs
  // SMBus interface selected -> look for all available DIMMs (enabled and disabled)
  //
  DimmPtr = GetDimm (&mCrInfo.NvdimmInfo, Skt, Ch, Dimm, ((FisIface == FisOverDdrt) ? NVDIMM_FLAG_ENABLED : NVDIMM_FLAG_ALL));
  if (DimmPtr == NULL) {
    return EFI_NOT_FOUND;
  }

  if (FisIface >= FisIfaceMax) {
    //
    // Invalid FIS interface selector
    //
    return EFI_INVALID_PARAMETER;
  }

   if (FIS_OPCODE_GET_MAJOR (InputPtr->Head.FisCmd) == FIS_CMD_EMULATED_MAJOR) {
     if ((FisIface == FisOverSmbus) && (InputPtr->Head.FisCmd != FIS_CMD_GET_BOOT_STATUS)) {
       //
       // Only Get Boot Status Register Emulated command is allowed over SMBUS
       //
       return EFI_INVALID_PARAMETER;
     }
  }

  switch (InputPtr->Head.FisCmd) {

    case FIS_CMD_GET_LP_MB_INFO:
      CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DFP") "S%d.C%d.D%d: Get large payload info\n", Skt, Ch, Dimm));
      if (OutputPtr == NULL || OutputPtr->Head.DataSize < sizeof (OutputPtr->Data.LpInfo)) {

        return EFI_INVALID_PARAMETER;
      }
      OutputPtr->Data.LpInfo.InpPayloadSize = CR_LARGE_PAYLOAD_SIZE; // Mailbox large input payload in bytes
      OutputPtr->Data.LpInfo.OutPayloadSize = CR_LARGE_PAYLOAD_SIZE; // Mailbox large output payload in bytes
      OutputPtr->Data.LpInfo.DataChunkSize = CR_LARGE_PAYLOAD_SIZE;
      OutputPtr->Head.DataSize = sizeof (OutputPtr->Data.LpInfo);
      return EFI_SUCCESS;

    case FIS_CMD_WRITE_LP_INPUT_MB:
      CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DFP") "S%d.C%d.D%d: Write large payload %d B at offset 0x%X\n",
              Skt, Ch, Dimm, InputPtr->Data.LpWrite.Size, InputPtr->Data.LpWrite.Offset));
      if (InputPtr == NULL || InputPtr->Head.DataSize != sizeof (InputPtr->Data.LpWrite) + InputPtr->Data.LpWrite.Size ||
          (UINT64)InputPtr->Data.LpWrite.Offset + InputPtr->Data.LpWrite.Size > CR_LARGE_PAYLOAD_SIZE) {

        return EFI_INVALID_PARAMETER;
      }
      //
      // Copy the provided buffer to the large input payload of the mailbox at the specified offset.
      //
      Status = CopyFromBufferToLargePayload ((UINT8*)&InputPtr->Data.LpWrite.Payload[0], 0, Skt, Ch, Dimm,
                                             InputPtr->Data.LpWrite.Offset, InputPtr->Data.LpWrite.Size, LARGE_PAYLOAD_INPUT);
      ASSERT_EFI_ERROR (Status);
      if (!mUseWpqFlush) {
        Status = CopyFromLargePayloadToBuffer (Skt, Ch, Dimm, InputPtr->Data.LpWrite.Offset, mLargePayloadBuffer, 0,
                                               InputPtr->Data.LpWrite.Size, LARGE_PAYLOAD_INPUT);
        ASSERT_EFI_ERROR (Status);
      }
      return Status;

    case FIS_CMD_READ_LP_OUTPUT_MB:
      CRDEBUG ((DEBUG_INFO, CR_INFO_STR("DFP") "S%d.C%d.D%d: Read large payload %d B at offset 0x%X\n",
              Skt, Ch, Dimm, InputPtr->Data.LpRead.Size, InputPtr->Data.LpRead.Offset));
      if (OutputPtr == NULL || InputPtr->Head.DataSize < sizeof (InputPtr->Data.LpRead) ||
          (UINT64)InputPtr->Data.LpRead.Offset + InputPtr->Data.LpRead.Size > CR_LARGE_PAYLOAD_SIZE ||
          OutputPtr->Head.DataSize < InputPtr->Data.LpRead.Size) {

        return EFI_INVALID_PARAMETER;
      }
      Status = CopyFromLargePayloadToBuffer (Skt, Ch, Dimm,
                                             InputPtr->Data.LpRead.Offset, (UINT8*)&OutputPtr->Data.LpData.Payload[0], 0,
                                             InputPtr->Data.LpRead.Size, LARGE_PAYLOAD_OUTPUT);
      if (!EFI_ERROR (Status)) {

        OutputPtr->Head.DataSize = InputPtr->Data.LpRead.Size;
      }
      return Status;

    case FIS_CMD_GET_BOOT_STATUS:
      if (OutputPtr == NULL || OutputPtr->Head.DataSize < sizeof (OutputPtr->Data.Bsr)) {
        return EFI_INVALID_PARAMETER;
      }
      Status = FisGetBootStatusRegister (((FisIface == FisOverDdrt) ? FIS_DDRT : FIS_SMBUS), Skt, Ch, Dimm, (FIS_BOOT_STATUS_REGISTER*)&OutputPtr->Data.Bsr);
      if (!EFI_ERROR (Status)) {
        OutputPtr->Head.DataSize = sizeof (OutputPtr->Data.Bsr);
      }
      return Status;

    default:
      break;
  } // switch (emulated commands)
  //
  // Here we know it is not one of emulated commands, let's send it to the NVDIMM FW.
  //
  if (InputPtr->Head.DataSize > sizeof (UINT32) * NUM_PAYLOAD_REG) {

    CRDEBUG ((DEBUG_INFO, CR_INFO_STR("DFP") "S%d.C%d.D%d: Input payload %d B exceeds mailbox input payload (%d B, what is %d dwords)\n",
            Skt, Ch, Dimm, InputPtr->Head.DataSize, sizeof (UINT32) * NUM_PAYLOAD_REG, NUM_PAYLOAD_REG));
    return EFI_INVALID_PARAMETER;
  }
  ZeroMem (&OutputPayload[0], sizeof (OutputPayload));
  if (FisIface == FisOverDdrt) {

    Status = FisRequest (Skt, Ch, Dimm, &InputPtr->Data.Fis.Payload[0], InputPtr->Head.DataSize, &OutputPayload[0], InputPtr->Head.FisCmd, Timeout, &FisSts);

  } else {

    Status = FisSmbusRequest (Skt, Ch, Dimm, &InputPtr->Data.Fis.Payload[0], InputPtr->Head.DataSize, &OutputPayload[0], InputPtr->Head.FisCmd, Timeout, &FisSts);
  }
  if (!EFI_ERROR (Status) && OutputPtr != NULL) {

    if (OutputPtr->Head.DataSize > sizeof (OutputPayload)) {

      OutputPtr->Head.DataSize = sizeof (OutputPayload);
    }
    CopyMem (&OutputPtr->Data.Fis.Payload[0], &OutputPayload[0], OutputPtr->Head.DataSize);
  }
  if (MboxStsPtr != NULL) {

    *MboxStsPtr = FisSts;
  }
  return Status;
} // DcpmmFisRequest()

/**
  @brief Get status of Address Range Scrub done on DCPMM NVDIMM.

  This function allows reading list of errors in the medium that could cause boot failure.
  When called with NULL in ArsErrTablePtr it will simply return length of ARS errors table.

  @param[in,out] ArsErrTableLenPtr On input number of entries in the ArsErrTablePtr buffer for ARS errors.
                                   On output number of all known error records is returned, can be larger than the input value.
  @param[out]    ArsErrTablePtr    Optional pointer to a buffer for ARS errors, can be
                                   NULL if user just needs to retrieve the number of error records.

  @retval EFI_SUCCESS           Command completed successfully, ArsErrTableLenPtr and ArsErrTablePtr updated.
  @retval EFI_INVALID_PARAMETER ArsErrTableLenPtr is NULL, or contains value greater than zero while ArsErrTablePtr is NULL.
  @retval EFI_NOT_STARTED       There is no ARS data in the system, ARS was not started.
  @retval EFI_NOT_READY         ARS is running, data returned may be incomplete.
  @retval EFI_BUFFER_TOO_SMALL  Buffer at ArsErrTablePtr is too small for all error entries, at ArsErrTableLenPtr retuning number of all entries.
**/
EFI_STATUS
EFIAPI
DcpmmArsStatus (
  IN OUT UINT32                 *ArsErrTableLenPtr,
     OUT DCPMM_ARS_ERROR_RECORD *ArsErrTablePtr
  )
{
  EFI_STATUS  Status;
  UINT32      ArsErrTableLen;
  UINT8       Idx;

  ARS_ERROR_RECORD *ArsErrPtr;

  if (ArsErrTableLenPtr == NULL || (ArsErrTablePtr == NULL && *ArsErrTableLenPtr != 0)) {

    return EFI_INVALID_PARAMETER;
  }
  ArsErrTableLen = *ArsErrTableLenPtr;
  *ArsErrTableLenPtr = 0;
  //
  // Invoke Query ARS via SW SMI to execute the retrieve the ARS results.
  //
  ArsErrPtr = (ARS_ERROR_RECORD*)&mNvdimmAcpiSmmInterface->OutputBuffer[0];
  mNvdimmAcpiSmmInterface->InputLength = mNvdimmAcpiSmmInterface->OutputLength = 0;
  mNvdimmAcpiSmmInterface->DeviceHandle = DSM_ROOT_DEVICE_ACPI_HANDLE;
  mNvdimmAcpiSmmInterface->FunctionIndex = DSM_ROOT_GET_ARS_STATE;
  IoWrite8 (0xB2, DSM_SW_SMI);

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR("DFP") "Got ARS state %d, records number %d\n",
          mNvdimmAcpiSmmInterface->Status, mNvdimmAcpiSmmInterface->OutputLength));
  //
  // SMI was executed and updated the mNvdimmAcpiSmmInterface structure. Let's see what is there.
  //
  switch (mNvdimmAcpiSmmInterface->Status) {
    case DSM_STS_EXT_ROOT_QUERY_ARS_COMPLETED:
    case DSM_STS_EXT_ROOT_ARS_STOPPED_PREMATURELY:
      Status = EFI_SUCCESS;
      break;
    case DSM_STS_EXT_ROOT_ARS_IN_PROGRESS:
      Status = EFI_NOT_READY;
      break;
    default:
      return EFI_NOT_STARTED;
  }
  *ArsErrTableLenPtr =mNvdimmAcpiSmmInterface->OutputLength;
  if (ArsErrTablePtr != NULL) {

    if (ArsErrTableLen >= mNvdimmAcpiSmmInterface->OutputLength) {

      ArsErrTableLen = mNvdimmAcpiSmmInterface->OutputLength;

    } else {

      Status = EFI_BUFFER_TOO_SMALL; // returned data will be truncated
    }
    for (Idx = 0; Idx < ArsErrTableLen; Idx++) {

      ArsErrTablePtr[Idx].NfitHandle = ArsErrPtr[Idx].NfitHandle;
      ArsErrTablePtr[Idx].Reserved = ArsErrPtr[Idx].Reserved;
      ArsErrTablePtr[Idx].SpaOfErrLoc = ArsErrPtr[Idx].SpaOfErrLoc;
      ArsErrTablePtr[Idx].Length = ArsErrPtr[Idx].Length;
      CRDEBUG ((DEBUG_INFO, CR_INFO_STR("DFP") "%04X: Got error at %016llX, length %d\n",
              ArsErrPtr[Idx].NfitHandle, ArsErrPtr[Idx].SpaOfErrLoc, ArsErrPtr[Idx].Length));
    }
  }
  return Status;
} // DcpmmArsStatus()

/**
  @brief Return maximum amount of time allowed to use the SMBus

  This function returns maximum amount of time allowed for UEFI MGMT SW to use the SMBus
  and can be overwritten by OEM hook. Note, that zero is special value and means no limit.

  @param[out] TimeMs           Maximum amount of time in ms allowed to use the SMBus.

  @retval EFI_SUCCESS           Command completed successfully.
  @retval EFI_INVALID_PARAMETER TimeMs is NULL.
**/
EFI_STATUS
EFIAPI
DcpmmSmbusGetMaxAccessTime (
  OUT UINT32 *TimeMs
  )
{
  return NvdimmSmbusGetMaxAccessTime (TimeMs);
}

/**
  @brief Provides interface to acquire SMBus access

  @param[in]  Socket            Socket index at system level
  @param[in]  Imc               iMC index at socket level

  @retval EFI_SUCCESS           Command completed successfully.
  @retval EFI_NOT_READY         TSOD control acquisition or TSOD state change failed.
  @retval EFI_INVALID_PARAMETER Invalid Socket or Imc value.
**/
EFI_STATUS
EFIAPI
DcpmmSmbusAccessAcquire (
  IN UINT8 Socket,
  IN UINT8 Imc
  )
{
  return NvdimmSmbusAccessAcquire (Socket, Imc);
}

/**
  @brief Provides interface to release SMBus access

  @param[in]  Socket            Socket index at system level
  @param[in]  Imc               iMC index at socket level

  @retval EFI_SUCCESS           Command completed successfully.
  @retval EFI_NOT_READY         TSOD control acquisition or TSOD state change failed.
  @retval EFI_INVALID_PARAMETER Invalid Socket or Imc value.
**/
EFI_STATUS
EFIAPI
DcpmmSmbusAccessRelease (
  IN UINT8 Socket,
  IN UINT8 Imc
  )
{
  return NvdimmSmbusAccessRelease (Socket, Imc);
}

/**
  @brief Convert SMART LSS details and ext. details into NVM_INTERPRETED_LSS_REASON.

  @param[in] LssDetails     - last shutdown state details from SMART
  @param[in] LssExtDetails  - last shutdown state extended details from SMART

  @return NVM_INTERPRETED_LSS_REASON is returned.
**/
STATIC NVM_INTERPRETED_LSS_REASON
GetInterpretedLssReason (
  IN  UINT8  LssDetails,
  IN  UINT8  LssExtDetails
  )
{
  UINT16     CombinedLssDetails;

  CombinedLssDetails = (UINT16)LssExtDetails << 8 | LssDetails;

  switch (CombinedLssDetails) {
    case NVM_LSS_REASON_CLEAN_LSS_ADR_POWER_FLOW:
      return NvmLssReasonCleanAdrPwrFlow;

    case NVM_LSS_REASON_CLEAN_LSS_S3_POWER_FLOW:
      return NvmLssReasonCleanS3PwrFlow;

    case NVM_LSS_REASON_CLEAN_LSS_S4_POWER_FLOW:
      return NvmLssReasonCleanS4PwrFlow;

    case NVM_LSS_REASON_CLEAN_LSS_S5_POWER_FLOW:
      return NvmLssReasonCleanS5PwrFlow;

    case NVM_LSS_REASON_CLEAN_LSS_IDLE_POWER_FLOW:
      return NvmLssReasonCleanIdlePwrFlow;

    case NVM_LSS_REASON_CLEAN_LSS_WARM_RESET_FLOW:
      return NvmLssReasonCleanWarmResetPwrFlow;

    default:
      //
      // None of clean shutdown statuses recognized, interpret as dirty shutdown.
      //
      if (NVM_LSS_REASON_DIRTY_LSS_ADR_STARTED_NO_DDRT_PWR_FAIL_CMD_RECV ==
          (CombinedLssDetails & NVM_LSS_REASON_DIRTY_LSS_ADR_STARTED_NO_DDRT_PWR_FAIL_CMD_RECV_MASK)) {

        return NvmLssReasonDirtyAdrStartedNoDdrtPwrFailCmdRecv;

      } else if (NVM_LSS_REASON_DIRTY_LSS_DDRT_PWR_FAIL_CMD_RECV_ADR_NOT_STARTED ==
                 (CombinedLssDetails & NVM_LSS_REASON_DIRTY_LSS_DDRT_PWR_FAIL_CMD_RECV_ADR_NOT_STARTED_MASK)) {

        return NvmLssReasonDirtyDdrtPwrFailCmdRecvAdrNotStarted;

      } else if (NVM_LSS_REASON_DIRTY_LSS_SURPRISE_PWR_LOSS_AT_NVDIMM ==
                 (CombinedLssDetails & NVM_LSS_REASON_DIRTY_LSS_SURPRISE_PWR_LOSS_AT_NVDIMM_MASK)) {

        return NvmLssReasonDirtySuprisePwrLossAtNvdimm;

      } else if (NVM_LSS_REASON_DIRTY_LSS_NVDIMM_THERMAL_TRIP ==
                 (CombinedLssDetails & NVM_LSS_REASON_DIRTY_LSS_NVDIMM_THERMAL_TRIP_MASK)) {

        return NvmLssReasonDirtyNvdimmThermalTrip;

      } else if (NVM_LSS_REASON_DIRTY_LSS_NVDIMM_VIRAL_TRIGGERED_BEFORE_PWR_DOWN ==
                 (CombinedLssDetails & NVM_LSS_REASON_DIRTY_LSS_NVDIMM_VIRAL_TRIGGERED_BEFORE_PWR_DOWN_MASK)) {

        return NvmLssReasonDirtyViralTriggeredBeforPwrDown;

      } else if (NVM_LSS_REASON_DIRTY_LSS_DDRT_SURPRISE_CLOCK_STOP ==
                 (CombinedLssDetails & NVM_LSS_REASON_DIRTY_LSS_DDRT_SURPRISE_CLOCK_STOP_MASK)) {

        return NvmLssReasonDirtyDdrtSupriseClkStop;

      } else if (NVM_LSS_REASON_DIRTY_LSS_WRITE_DATA_FLUSH_NOT_COMPLETED ==
                 (CombinedLssDetails & NVM_LSS_REASON_DIRTY_LSS_WRITE_DATA_FLUSH_NOT_COMPLETED_MASK)) {

        return NvmLssReasonDirtyWriteDataFlushNotCompleted;

      } else if (NVM_LSS_REASON_DIRTY_LSS_INJECTED_DIRTY_SHUTDOWN ==
                 (CombinedLssDetails & NVM_LSS_REASON_DIRTY_LSS_INJECTED_DIRTY_SHUTDOWN_MASK)) {

        return NvmLssReasonDirtyInjectedDirtyShutdown;

      } else if (NVM_LSS_REASON_DIRTY_LSS_NVDIMM_INTERNAL_CONTEXT_FLUSH_FAILED ==
                 (CombinedLssDetails & NVM_LSS_REASON_DIRTY_LSS_NVDIMM_INTERNAL_CONTEXT_FLUSH_FAILED_MASK)) {

        return NvmLssReasonDirtyNvdimmInternalCtxFlushFailed;
      }
      break;
  }
  CRDEBUG ((DEBUG_WARN, CR_WARN_STR ("DFP") "LSS details 0x%04X not recognized as expected shutdown reason\n", CombinedLssDetails));
  return NvmLssReasonDirtyOther;
} // GetInterpretedLssReason()


/**
  @brief Provides interpreted last shutdown state and reason using latched data.

  @param[in]  NfitHandle              NFIT handle of the target DCPMM NVDIMM.
  @param[out] InterpretedLssPtr       Buffer for interpreted last shutdown state.
  @param[out] InterpretedLssReasonPtr Buffer for interpreted last shutdown state reason.

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
EFIAPI
DcpmmGetPlatformInterpretedLss (
  IN  UINT32                        NfitHandle,
  OUT DCPMM_INTERPRETED_LSS        *InterpretedLssPtr,
  OUT DCPMM_INTERPRETED_LSS_REASON *InterpretedLssReasonPtr
  )
{
  EFI_STATUS Status;
  UINT8      NmFlushState;
  UINT8      Skt;
  UINT8      Ch;
  UINT8      Dimm;
  UINT8      Imc;
  UINT8      ImcCh;

  FIS_BOOT_STATUS_REGISTER                Bsr;
  FIS_GET_LATCH_SYSTEM_SHUTDOWN_STATE_RSP LssRsp;
  FIS_GET_SMART_HEALTH_RSP                SmartRsp;
  HOST_DDRT_DIMM_DEVICE_INFO_STRUCT      *DdrtDimmInfo;

  NVDIMM *DimmPtr;

  if (InterpretedLssPtr == NULL || InterpretedLssReasonPtr == NULL) {

    return EFI_INVALID_PARAMETER;
  }
  Skt = NFIT_HANDLE_TO_SKT (NfitHandle);
  Ch = ConvertImcChToSktCh (NFIT_HANDLE_TO_MC (NfitHandle), NFIT_HANDLE_TO_CH (NfitHandle));
  Dimm = NFIT_HANDLE_TO_DIMM (NfitHandle);
  Imc = ConvertSktChToImc (Ch);
  ImcCh = ConvertSktChToImcCh (Ch);
  if (NfitHandle != NFIT_HANDLE_BUILD (Skt, Imc, ImcCh, Dimm)) {
    //
    // One of reserved fields in HFIT handle has unexpected value.
    //
    return EFI_INVALID_PARAMETER;
  }
  DimmPtr = GetDimm (&mCrInfo.NvdimmInfo, Skt, Ch, Dimm, NVDIMM_FLAG_ALL);
  if (DimmPtr == NULL) {

    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("DFP") "S%d.C%d.D%d: No such NVDIMM\n", Skt, Ch, Dimm));
    return EFI_NOT_FOUND;
  }
  //
  // Retrieve NVDIMM LSS Latch and SMART data. If DIMM is mapped use DDRT mailbox, otherwise use SMBUS.
  //
  ZeroMem (&LssRsp, sizeof (LssRsp));
  ZeroMem (&SmartRsp, sizeof (SmartRsp));
  if (DimmPtr->SADSpaBase != SPA_NOT_MAPPED) {

    Status = FisRequest (Skt, Ch, Dimm, NULL, 0, &LssRsp.DWord[0], GET_LATCH_SYSTEM_SHUTDOWN_STATUS, 0, NULL);
    if (!EFI_ERROR (Status)) {
      Status = FisRequest (Skt, Ch, Dimm, NULL, 0, &SmartRsp.DWord[0], GET_SMART_HEALTH, 0, NULL);
    }
  } else {

    Status = FisSmbusRequest (Skt, Ch, Dimm, NULL, 0, &LssRsp.DWord[0], GET_LATCH_SYSTEM_SHUTDOWN_STATUS, 0, NULL);
    if (!EFI_ERROR (Status)) {
      Status = FisSmbusRequest (Skt, Ch, Dimm, NULL, 0, &SmartRsp.DWord[0], GET_SMART_HEALTH, 0, NULL);
    }
  }
  if (EFI_ERROR (Status) || !SmartRsp.Bits.VendorDataSizeVal || SmartRsp.Bits.VendorDataSize < sizeof (SmartRsp.Bits.VendorData) -
                                                                                               sizeof (SmartRsp.Bits.VendorData.Reserved7) -
                                                                                               sizeof (SmartRsp.Bits.VendorData.ULssDetails) -
                                                                                               sizeof (SmartRsp.Bits.VendorData.ULssExtDetails) -
                                                                                               sizeof (SmartRsp.Bits.VendorData.MaxMediaTemp) -
                                                                                               sizeof (SmartRsp.Bits.VendorData.MaxCoreTemp)) {
    //
    // Sanity check if we are possibly called with some old firmware.
    //
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("DFP") "S%d.C%d.D%d: NVDIMM firmware does not ", Skt, Ch, Dimm));
    CRDEBUG ((DEBUG_ERROR, "provide data necessary to interpret latched LSS (Status '%r', valid %d, size %d)\n",
              Status, SmartRsp.Bits.VendorDataSizeVal, SmartRsp.Bits.VendorDataSize));
    return EFI_UNSUPPORTED;
  }
  DdrtDimmInfo = &mSystemMemoryMap->HostDdrtDimmInfo[Skt][Ch];
  Bsr.QWord = *(UINT64 *)&DdrtDimmInfo->NgnBsr;
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DFP") "S%d.C%d.D%d: BSR 0x%012llX, LinkFAilOnPrevBoot %d, MediaErrLogged %d, TempErrLogged %d\n",
            Skt, Ch, Dimm, Bsr.QWord, DdrtDimmInfo->NvdimmLinkFailOnPrevBoot,
            DdrtDimmInfo->NvdimmMediaErrLogged,
            DdrtDimmInfo->NvdimmTempErrLogged));
  if (DdrtDimmInfo->NvdimmLinkFailOnPrevBoot) {
    //
    // Using LinkFailureOnPrevBoot from the DIMM structure, because in BSR it could get cleared during boot.
    // The DIMM strcucture keeps the snapshot taken from BSR when it was checked early in PEI phase.
    //
    *InterpretedLssPtr = (LssRsp.Bits.PrevLatchSystemShutdownStateEn) ? DcpmmLssDirty : DcpmmLssDirtyNotLatched;
    if (Bsr.Bits.MediaDisabled) {

      *InterpretedLssReasonPtr = (Bsr.Bits.WatchdogTimerStatus) ? DcpmmLssReasonDirtyDimmFatalFwHang : DcpmmLssReasonDirtyDimmFatal;
      return EFI_SUCCESS;
    }
    if (DdrtDimmInfo->NvdimmTempErrLogged) {

      *InterpretedLssReasonPtr = DcpmmLssReasonDirtyTempFatal;
      return EFI_SUCCESS;
    }
    if (DdrtDimmInfo->NvdimmMediaErrLogged) {

      *InterpretedLssReasonPtr = DcpmmLssReasonDirtyTransactionFatal;
      return EFI_SUCCESS;
    }
    //
    // Let it decide according to SMART info below.
    //
  }
  NmFlushState = GetNmFlushStatusFromCmosReg ();
  //
  // Default to clean shutdown, check if was latched.
  //
  *InterpretedLssPtr = (LssRsp.Bits.PrevLatchSystemShutdownStateEn) ? NvmLssClean : NvmLssCleanNotLatched;

  if (SmartRsp.Bits.LLastShutdownStatus == FIS_LSS_PLATFORM_CLEAN_SHUTDOWN && (SIG_NM_SUCCESS == NmFlushState)) {
    //
    // Last shutdown was clean, near memory flush if required was complete.
    // Interpret the clean shutdown reason.
    //
    *InterpretedLssReasonPtr = GetInterpretedLssReason (SmartRsp.Bits.VendorData.LLssDetails.Byte, SmartRsp.Bits.VendorData.LLssExtDetails.Byte[0]);
    if (*InterpretedLssReasonPtr > DcpmmLssReasonCleanOther) {

      CRDEBUG ((DEBUG_WARN, CR_WARN_STR ("DFP") "S%d.C%d.D%d: Got dirty shutdown reason %d for clean shutdown, override to %d\n",
              Skt, Ch, Dimm, *InterpretedLssReasonPtr, NvmLssReasonCleanOther));
      *InterpretedLssReasonPtr = NvmLssReasonCleanOther;
    }
  } else {

    if (mSystemMemoryMap->Socket[Skt].ChannelInfo[Ch].DimmInfo[Dimm].VolCap != 0 &&
        mSystemMemoryMap->Socket[Skt].ChannelInfo[Ch].DimmInfo[Dimm].nonVolCap == 0) {
      //
      // This NVDIMM is 100% volatile 2LM. Ignore SMART shutdown state, keep clean default and set reason to cfg.
      //
      *InterpretedLssReasonPtr = NvmLssReasonCleanIgnoredDueToCfg;
      goto GetOut;
    }
    //
    // Last shutdown was dirty. Interpret the dirty shutdown reason.
    //
    *InterpretedLssPtr = (LssRsp.Bits.PrevLatchSystemShutdownStateEn) ? NvmLssDirty : NvmLssDirtyNotLatched;

    if (SIG_NM_ERROR == NmFlushState) {

      *InterpretedLssReasonPtr = NvmLssReasonDirtyPersistentCachingFailure;

    } else {

      *InterpretedLssReasonPtr = GetInterpretedLssReason (SmartRsp.Bits.VendorData.LLssDetails.Byte, SmartRsp.Bits.VendorData.LLssExtDetails.Byte[0]);
      if (*InterpretedLssReasonPtr <= DcpmmLssReasonCleanOther) {

        CRDEBUG ((DEBUG_WARN, CR_WARN_STR ("DFP") "S%d.C%d.D%d: Got clean shutdown reason %d for dirty shutdown, override to %d\n",
                  Skt, Ch, Dimm, *InterpretedLssReasonPtr, NvmLssReasonDirtyOther));
        *InterpretedLssReasonPtr = NvmLssReasonDirtyOther;
      }
    }
  }
GetOut:
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("DFP") "S%d.C%d.D%d: LSSS 0x%04X, NM Flush State 0x%X, ",
            Skt, Ch, Dimm, LssRsp.DWord[0], NmFlushState));
  CRDEBUG ((DEBUG_INFO, "LLSS details 0x%02X%02X -> interpreted LSS %d reason %d\n",
            SmartRsp.Bits.VendorData.LLssExtDetails.Byte[0], SmartRsp.Bits.VendorData.LLssDetails.Byte,
            *InterpretedLssPtr, *InterpretedLssReasonPtr));
  return EFI_SUCCESS;
} // DcpmmGetPlatformInterpretedLss()


/**
  @brief This function installs DCPMM protocol.

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
DcpmmProtocolInstall (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS Status;
  EFI_HANDLE Handle = NULL;

  ASSERT (!mInSmm);
  ASSERT (mNvdimmAcpiSmmInterface != NULL);

  ZeroMem (&mDcpmmProtocol, sizeof (mDcpmmProtocol));

  //
  // Set current CRP revision
  //
  mDcpmmProtocol.ProtocolVersion = DCPMM_PROTOCOL_VER_CUR;

  mDcpmmProtocol.DcpmmNextNvdimm = DcpmmNextNvdimm;
  mDcpmmProtocol.DcpmmFisRequest = DcpmmFisRequest;
  mDcpmmProtocol.DcpmmArsStatus = DcpmmArsStatus;
  mDcpmmProtocol.DcpmmSmbusGetMaxAccessTime = DcpmmSmbusGetMaxAccessTime;
  mDcpmmProtocol.DcpmmSmbusAccessAcquire = DcpmmSmbusAccessAcquire;
  mDcpmmProtocol.DcpmmSmbusAccessRelease = DcpmmSmbusAccessRelease;
  mDcpmmProtocol.DcpmmGetPlatformInterpretedLss = DcpmmGetPlatformInterpretedLss;

  Status = gBS->InstallProtocolInterface (&Handle, &gEfiDcpmmProtocolGuid, EFI_NATIVE_INTERFACE, &mDcpmmProtocol);
  if (EFI_ERROR (Status)) {

    CRDEBUG ((EFI_D_ERROR, CR_ERROR_STR ("DFP") "Cannot install PMem protocol (%r)\n", Status));
  }
  return Status;
} // DcpmmProtocolInstall()

