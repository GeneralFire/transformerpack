/** @file Fis.c
  Implementation of Firmware Interface Specification for Apache Pass Firmware.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

#include <PiDxe.h>
#include <Library/PrintLib.h>
#include <FnvAccessCommon.h>
#include "CrystalRidge.h"
#include "Fis.h"
#include "NvdimmSmbusAccess.h"
#include "XlateFunctions/XlateFunctions.h"
#include "Dsm.h"
#include "Ars/ArsFlows.h"
#include "AcpiNotify.h"


/******************************************************************************
 * Definitions
 ******************************************************************************/
#define FIS_DUMP_SIZE   0  // Lenght of request/response payload dumps

/******************************************************************************
 * Variables.
 ******************************************************************************/
//
// This table keeps long operations started by BIOS. An entry gets cleared
// when operation result is consumed by BIOS. This table is used to make sure
// long operation result is consumed by BIOS before starting any new long operation
// that would overwrite it.
//
UINT16 FisLongOpRunning[MAX_SOCKET][MAX_CH] = {NULL_COMMAND};
UINT32 FisLongOpSeqNum[MAX_SOCKET][MAX_CH] = {0};

//
// Performace counter properties used to measure time
//
struct {
  UINT64 CntStartValue;
  UINT64 CntEndValue;
  UINT64 CntFreq;
  UINT64 CntValueAtInit;
  UINT32 RtcTimeAtInit;
} PerfCntProp;

/******************************************************************************
 * Functions
 ******************************************************************************/
VOID
FisInit (VOID)
{
  PerfCntProp.CntFreq = GetPerformanceCounterProperties (&PerfCntProp.CntStartValue, &PerfCntProp.CntEndValue);
  PerfCntProp.CntValueAtInit = GetPerformanceCounter ();
  OemGetTimeStamp (&PerfCntProp.RtcTimeAtInit);
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("FIS") "TSC Start %llu, End %llu, Frequecey %llu, CntValueAtInit %llu, RtcTimeAtInit %u\n",
           PerfCntProp.CntStartValue, PerfCntProp.CntEndValue, PerfCntProp.CntFreq, PerfCntProp.CntValueAtInit, PerfCntProp.RtcTimeAtInit));
}

/**
  @brief Get the Boot Status Register value for given DIMM.

  @param[in]  FisIface  - FIS interface to use for the request.
  @param[in]  Skt       - Skt Number
  @param[in]  Ch        - DDR Channel ID
  @param[in]  Dimm      - DIMM Number
  @param[out] BsrPtr    - Pointer to buffer for BSR value

  @return EFI_STATUS is returned.
**/
EFI_STATUS
FisGetBootStatusRegister (
  IN  FIS_INTERFACE             FisIface,
  IN  UINT8                     Skt,
  IN  UINT8                     Ch,
  IN  UINT8                     Dimm,
  OUT FIS_BOOT_STATUS_REGISTER  *BsrPtr
  )
{
  EFI_STATUS  Status;
  NVDIMM      *DimmPtr = NULL;

  if ((FisIface >= FIS_IFC_MAX) || (NULL == BsrPtr)) {
    return EFI_INVALID_PARAMETER;
  }

  DimmPtr = GetDimm (&mCrInfo.NvdimmInfo, Skt, Ch, Dimm, ((FisIface == FIS_DDRT) ? NVDIMM_FLAG_ENABLED : NVDIMM_FLAG_ALL));
  if (DimmPtr == NULL) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("FIS") "S%d.C%d.D%d: No NVDIMM\n", Skt, Ch, Dimm));
    return EFI_NOT_FOUND;
  }

  if (FisIface == FIS_DDRT) {
    BsrPtr->QWord = *(VOLATILE UINT64*)DimmPtr->BsrSpa;
  } else {
    Status = NvdimmSmbusAccessReadBootStatusRegisters (Skt, Ch, Dimm, &BsrPtr->QWord);
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("FIS") "S%d.C%d.D%d: SMBUs Boot Status Register read error (status %r)\n", Skt, Ch, Dimm, Status));
      return EFI_DEVICE_ERROR;
    }
  }

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR("FIS") "S%d.C%d.D%d: Boot Status Register 0x%016llx\n", Skt, Ch, Dimm, BsrPtr->QWord));
  return EFI_SUCCESS;
}


/**

Routine Description: Writes data from the buffer to small input payload.

  @param[in] *NvmDimm       - NVDIMM structure pointer
  @param[in] *Buffer        - buffer to write
  @param[in] BufferSize     - buffer size in bytes
  @param[in] UseOsMailbox   - if TRUE uses OS mailbox and BIOS mailbox otherwise.

  @retval EFI_SUCCESS           Operation finished successfully
  @retval EFI_INVALID_PARAMETER NvmDimm or Buffer are NULL, BufferSize is 0 or
                                greater than Payload size.

**/
EFI_STATUS
WriteToSmallPayload (
  NVDIMM      *NvmDimm,
  CONST VOID  *Buffer,
  UINTN       BufferSize,
  BOOLEAN     UseOsMailbox
  )
{
  CR_MAILBOX      *Mailbox;
  UINT8           PayloadIndex;
  UINT8           PayloadLen;
  UINT32          Trail = 0;
  UINT8           TrailLen;

  if ((NvmDimm == NULL) || (Buffer == NULL) || (BufferSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  if (BufferSize > NUM_PAYLOAD_REG * sizeof (UINT32)) {
    return EFI_INVALID_PARAMETER;
  }

  Mailbox = GetMailbox (NvmDimm, UseOsMailbox);

  //
  // We need to fit the Buffer into 32 bit payload registers, so
  // PayloadLen should be the ceiling of BufferSize divided by size of
  // single payload register
  //
  PayloadLen = (UINT8) (BufferSize / sizeof (UINT32));

  for (PayloadIndex = 0; PayloadIndex < PayloadLen; PayloadIndex++) {
    if (mUseWpqFlush) {
      *(VOLATILE UINT32 *)Mailbox->pInPayload[PayloadIndex] = ((UINT32 *)Buffer)[PayloadIndex];
      WaWpqFlush (NvmDimm);
    } else {
      DurableCacheLineWrite (
        (UINTN)Mailbox->pInPayload[PayloadIndex],
        ((UINT32 *) Buffer)[PayloadIndex],
        sizeof (UINT32)
        );
    }
  }
  TrailLen = BufferSize % sizeof (UINT32);
  if (TrailLen && PayloadIndex < NUM_PAYLOAD_REG) {
    CopyMem (&Trail, (UINT32 *)Buffer + PayloadIndex, TrailLen);
    if (mUseWpqFlush) {
      *(VOLATILE UINT32 *)Mailbox->pInPayload[PayloadIndex] = Trail;
      WaWpqFlush (NvmDimm);
    } else {
      DurableCacheLineWrite (
        (UINTN)Mailbox->pInPayload[PayloadIndex],
        Trail,
        sizeof (UINT32)
        );
    }
  }
  return EFI_SUCCESS;
}

/**
  Checks if specified command is a non-long-op command that will clash
  with a running ARS.

  @param[in]  Opcode  FW command to check

  @retval  TRUE   The command is a non-long-op command that will clash
                  with a running ARS
  @retval  FALSE  The command is not a non-long-op command that will clash
                  with a running ARS
**/
BOOLEAN
IsArsClashCommand (
  IN UINT16 Opcode
  )
{
  switch (Opcode) {
    case SECURE_ERASE_UNIT:
    case INJECT_POISON_ERROR:
      return TRUE;
  }
  return FALSE;
}

/**
  Checks if specified command is a long-op command that will clash
  with a running ARS.

  @param[in]  Opcode  FW command to check

  @retval  TRUE   The command is a long-op command that will clash
                  with a running ARS
  @retval  FALSE  The command is not a long-op command that will clash
                  with a running ARS
**/
BOOLEAN
IsArsClashLongCommand (
  IN UINT16 Opcode
  )
{
  switch (Opcode) {
    case OVERWRITE_DIMM:
    case UPDATE_FNV_FW:
      return TRUE;
  }
  return FALSE;
}

/**

Routine Description: Reads data from small output payload to the buffer

  @param[in]  *NvmDimm       - NVDIMM structure pointer
  @param[out] *Buffer        - buffer to read to
  @param[in]  BufferSize     - buffer size in bytes and data to read
  @param[in]  UseOsMailbox   - if TRUE uses OS mailbox and BIOS mailbox otherwise.

  @retval EFI_SUCCESS           Operation finished successfully
  @retval EFI_INVALID_PARAMETER NvmDimm or Buffer are NULL, BufferSize is 0 or
                                greater than Payload size.

**/
EFI_STATUS
ReadFromSmallPayload (
  NVDIMM    *NvmDimm,
  VOID      *Buffer,
  UINTN     BufferSize,
  BOOLEAN   UseOsMailbox
  )
{
  CR_MAILBOX      *Mailbox;
  UINT8           PayloadIndex;
  UINT32          Payload[NUM_PAYLOAD_REG];
  UINT8           PayloadLen;

  if ((NvmDimm == NULL) || (Buffer == NULL) || (BufferSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  if (BufferSize > sizeof (Payload)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // We need to fit the 32 bit payload registers into Buffer, so
  // PayloadLen should be the ceiling of BufferSize divided by size of
  // single payload register
  //
  PayloadLen = (UINT8)((BufferSize / sizeof (Payload[0])) +
                      ((BufferSize % sizeof (Payload[0])) != 0));

  Mailbox = GetMailbox (NvmDimm, UseOsMailbox);

  for (PayloadIndex = 0; PayloadIndex < PayloadLen; PayloadIndex++) {
    Payload[PayloadIndex] = *(UINT32 *)(UINTN)Mailbox->pOutPayload[PayloadIndex];
  }

  CopyMem (Buffer, Payload, BufferSize);

  return EFI_SUCCESS;
}

/**
 @brief Sends a command to NVDIMM via Small Payload registers as defined in FIS.

 Note that this function actively waits for NVDIMM FW response.

 @param[in]  Socket        - Socket Number
 @param[in]  Ch            - DDR Channel ID
 @param[in]  Dimm          - DIMM Number
 @param[in]  InputPayload  - Pointer to buffer containing Input Payload data (NULL if not needed)
 @param[in]  InputLength   - Length of input payload data (may be 0 if no input)
 @param[out] OutputPayload - Pointer to buffer for Output Payload data (NULL if not needed)
 @param[in]  Opcode        - Opcode of the command to be sent
 @param[in]  Timeout       - Optional timeout in us, use zero to assign default timeout
 @param[out] FisStsPtr     - Optional pointer to buffer for FIS status value (NULL if unused)

 @return NVDIMM status translated to EFI_STATUS is returned.

**/
EFI_STATUS
FisRequest (
  IN       UINT8     Socket,
  IN       UINT8     Ch,
  IN       UINT8     Dimm,
  IN CONST UINT32    *InputPayload,
  IN       UINT32    InputLength,
     OUT   UINT32    OutputPayload[NUM_PAYLOAD_REG],
  IN       UINT16    Opcode,
  IN       UINT64    Timeout,
     OUT FIS_STATUS *FisStsPtr
  )
{
  EFI_STATUS                                      Status = EFI_SUCCESS;
  NVDIMM                                          *NvmDimm = NULL;
  CR_MAILBOX                                      *Mailbox;
  CR_MAILBOX                                      *OSMailbox;
  UINT64                                          TimeoutCounter = 0;
  INT64                                           WaitTime;
  UINT64                                          TotalTimeInNanoSeconds = 0;
  UINT64                                          StartTime;
  UINT64                                          EndTime;
  INT64                                           CounterCycle;
  MB_SMM_CMD_FNV_SPA_MAPPED_0_STRUCT              CmdReg;
  MB_SMM_CMD_FNV_SPA_MAPPED_0_STRUCT              OSCmdReg;
  MB_SMM_CMD_FNV_SPA_MAPPED_0_STRUCT              SmbusCmdReg;
  VOLATILE MB_SMM_CMD_FNV_SPA_MAPPED_0_STRUCT     *SmbusCmdRegPtr;
  MB_SMM_STATUS_FNV_SPA_MAPPED_0_STRUCT           StsReg;
  CHAR8                                           *MboxErrStrPtr = "Mailbox Failure: N/A";
  VOLATILE MB_SMM_CMD_FNV_SPA_MAPPED_0_STRUCT     *CmdRegPtr;
  VOLATILE MB_SMM_CMD_FNV_SPA_MAPPED_0_STRUCT     *OSCmdRegPtr;
  VOLATILE MB_SMM_STATUS_FNV_SPA_MAPPED_0_STRUCT  *StsRegPtr;
  UINT32                                          TimeStamp;
  BOOLEAN                                         DoNotify = FALSE;
  BOOLEAN                                         UseOsMailboxBkp;

  if (mInSmm) {
    //
    // When SMM Mailbox is used, check if all APs have checked-in into SMM. If not, return EFI_NOT_READY
    //
    if (!mUseOSMailbox) {
      if (!CheckAllApCheckedIn (NULL)) {
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("FIS") "S%d.C%d.D%d: Threads not in sync in SMM, command 0x%X failed\n",
                Socket, Ch, Dimm, Opcode));
        return EFI_NOT_READY;
      }
    }
  }
  if (FisStsPtr != 0) {
    *FisStsPtr = FIS_STS_UNDEFINED;
  }

  NvmDimm = GetDimm (&mCrInfo.NvdimmInfo, Socket, Ch, Dimm, NVDIMM_FLAG_ENABLED);
  if (NvmDimm == NULL) {

    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("FIS") "S%d.C%d.D%d: No NVDIMM\n", Socket, Ch, Dimm));
    return EFI_INVALID_PARAMETER;
  }

  if (ArsInBackground () && ArsIsInProgress () && FisLongOpRunning[Socket][Ch] == SET_ADDRESS_RANGE_SCRUB) {
    if (IsArsClashLongCommand (Opcode) || IsArsClashCommand (Opcode)) {
      UseOsMailboxBkp = mUseOSMailbox;
      mUseOSMailbox = FALSE;
      ArsAbort ();
      ArsProcessForAllDimms ();
      mUseOSMailbox = UseOsMailboxBkp;
    }
    if (IsArsClashCommand (Opcode)) {
      DoNotify = TRUE;
    }
  }

  Mailbox = GetMailbox (NvmDimm, mUseOSMailbox);
  CmdRegPtr = (MB_SMM_CMD_FNV_SPA_MAPPED_0_STRUCT *)Mailbox->pCommand;
  StsRegPtr = (MB_SMM_STATUS_FNV_SPA_MAPPED_0_STRUCT *)Mailbox->pStatus;

  if (Timeout == 0) {

    switch (Opcode) {
      case RESET_ALL_THRESHOLDS:
        Timeout = CR_DOORBELL_TIMEOUT_RESET_ALL_THRESHOLDS;
        break;
      case GET_PLATFORM_CONFIG:
        Timeout = CR_DOORBELL_TIMEOUT_GET_PLATFORM_CONFIG_DATA;
        break;
      case SET_PLATFORM_CONFIG:
        Timeout = CR_DOORBELL_TIMEOUT_SET_PLATFORM_CONFIG_DATA;
        break;
      case DDRT_IO_INIT:
        Timeout = CR_DOORBELL_TIMEOUT_SET_DDDRT_IO_INIT_INFO(NvmDimm->FisVersion);
        break;
      case GET_FIRMWARE_DEBUG:
        Timeout = CR_DOORBELL_TIMEOUT_GET_FIRMWARE_DEBUG;
        break;
      case GET_COMMAND_EFFECT_LOG:
        Timeout = CR_DOORBELL_TIMEOUT_GET_COMMAND_EFFECT_LOG;
        break;
      case UPDATE_FNV_FW:
        Timeout = CR_DOORBELL_TIMEOUT_UPDATE_FNV_FW(NvmDimm->FisVersion);
        break;
      case INJECT_SW_TRIGGER:
        Timeout = CR_DOORBELL_TIMEOUT_INJECT_SW_TRIGGER(NvmDimm->FisVersion);
        break;
      default:
        Timeout = CR_DOORBELL_TIMEOUT;
    }
    if (FIS_OPCODE_GET_MAJOR(Opcode) == MC_SET_SECURITY_INFO) {

      Timeout = CR_BIOS_POLLING_TIME + CR_DOORBELL_TIMEOUT_SECURITY_CMD_TIMEOUT;
    }
    if (StsRegPtr->Bits.bos) {
      //
      // Extend timeout by 50ms if background operation is running.
      //
      Timeout += 50000;
    }
  }
  if (PerfCntProp.CntStartValue > PerfCntProp.CntEndValue) {
    TimeStamp = PerfCntProp.RtcTimeAtInit + (UINT32)((PerfCntProp.CntValueAtInit - GetPerformanceCounter ()) / PerfCntProp.CntFreq);
  } else {
    TimeStamp = PerfCntProp.RtcTimeAtInit + (UINT32)((GetPerformanceCounter () - PerfCntProp.CntValueAtInit) / PerfCntProp.CntFreq);
  }
  CmdReg.Data = CmdRegPtr->Data;
  StsReg.Data = StsRegPtr->Data;

  if (mInSmm) {
    //
    // Make sure doorbell is clear and mailbox delay not active, since mechanism in FW
    // (in case of DDRT alert transmission while BIOS mailbox is being attacked)
    // doesn't work instantly. Attacker's command first needs to bounce with INVALID NONCE
    // As this may take some time, max 3 attempts with 10us interval are used instead of simple checking.
    //
    TimeoutCounter = CR_BIOS_MB_READY_POLLING_TIME / COMMON_DELAY_UNIT;
    while (CmdReg.Bits.doorbell || StsReg.Bits.MbDelayed) {
      if (TimeoutCounter == 0) {
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("FIS") "S%d.C%d.D%d: [%u] Previous command did not complete (cmd: 0x%llX sts: 0x%llX)\n",
                Socket, Ch, Dimm, TimeStamp, CmdReg.Data, StsReg.Data));
        Status = EFI_NOT_READY;
        goto GetOut;
      }
      MicroSecondDelay (COMMON_DELAY_UNIT);
      TimeoutCounter--;
      CmdReg.Data = CmdRegPtr->Data;
      StsReg.Data = StsRegPtr->Data;
    }
  } else {
    //
    // Make sure doorbell is clear.
    //
    if (CmdReg.Bits.doorbell) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("FIS") "S%d.C%d.D%d: [%u] Previous command did not complete (cmd: 0x%llX sts: 0x%llX)\n",
              Socket, Ch, Dimm, TimeStamp, CmdReg.Data, StsReg.Data));
      Status = EFI_NOT_READY;
      goto GetOut;
    }

    if (StsReg.Bits.MbDelayed) {
      //
      // if MB Rate Limited, bump timeout by 2ms.
      //
      Timeout += CR_MB_DELAYED_TIMEOUT;
    }
  }
  TimeoutCounter = Timeout / COMMON_DELAY_UNIT;

  // In the SMM check if OS doorbell is set in case that FW is executing OS or SMBUS mailbox command
  // If so wait for this command to complete and also decrease SMM timeout budget
  if (mInSmm) {
    OSMailbox = GetMailbox (NvmDimm, TRUE);
    OSCmdRegPtr = (MB_SMM_CMD_FNV_SPA_MAPPED_0_STRUCT *)OSMailbox->pCommand;
    OSCmdReg.Data = OSCmdRegPtr->Data;
    SmbusCmdRegPtr = (MB_SMM_CMD_FNV_SPA_MAPPED_0_STRUCT *)NvmDimm->SmbusCmdRegSpa;
    if (NvmDimm->FisVersion >= FIS_2_04) {
      SmbusCmdReg.Data = SmbusCmdRegPtr->Data;
    }
    while (OSCmdReg.Bits.doorbell || (NvmDimm->FisVersion >= FIS_2_04 && SmbusCmdReg.Bits.doorbell)) {
      if (mEadrProtocol != NULL) {
        Status = mEadrProtocol->EadrCheckStatusAndExecute ();
        if (EFI_ERROR (Status)) {
          CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("FIS") "Failed to obtain eADR status: %r\n", Status));
        }
      }
      if (TimeoutCounter == 0) {
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("FIS") "S%d.C%d.D%d: [%u] Previous OS or SMBUS command did not complete (OS cmd: 0x%llX SMBUS: 0x%llX)\n",
                Socket, Ch, Dimm, TimeStamp, OSCmdReg.Data, SmbusCmdReg.Data));
        Status = EFI_NOT_READY;
        goto GetOut;
      }
      MicroSecondDelay (COMMON_DELAY_UNIT);
      TimeoutCounter--;
      OSCmdReg.Data = OSCmdRegPtr->Data;
      if (NvmDimm->FisVersion >= FIS_2_04) {
        SmbusCmdReg.Data = SmbusCmdRegPtr->Data;
      }
    }
  }

  //
  // Copy Input Payload
  //
  if (InputPayload != NULL) {

    WriteToSmallPayload (NvmDimm, InputPayload, InputLength, mUseOSMailbox);
#if FIS_DUMP_SIZE
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("FIS") "S%d.C%d.D%d: Request 0x%04X input:\n", Socket, Ch, Dimm, Opcode));
    DisplayBuffer (DEBUG_INFO, (UINT8*)InputPayload, MIN (FIS_DUMP_SIZE, InputLength));
#endif
  }

  if (mUseWpqFlush) {
    //
    // Per NVMCTLR Spec, toggle the Sequence Bit everytime we send a command so F/W knows if the command is a retry
    //
    (CmdRegPtr->Bits.sequence_bit) ? (CmdRegPtr->Bits.sequence_bit = 0) : (CmdRegPtr->Bits.sequence_bit = 1);

    //
    // Set the Opcode
    //
    CmdRegPtr->Bits.opcode = Opcode;

    //
    // If the Security Nonce has been set, copy the Nonce to the Nonce0 register (and only if we use BIOS Mailbox)
    //
    if ((NvmDimm->NonceSet) && (!mUseOSMailbox)) {
      *(UINT64 *)(UINTN)Mailbox->pNonce0 = NvmDimm->Nonce;
    }

    //
    // Per EKV architect, a WPQ flush is required prior to setting the doorbell bit
    //
    IssueWpqFlush (NvmDimm, TRUE);

    //
    // Set the doorbell to tell the controller that the command is ready to be processed.
    //
    CmdRegPtr->Bits.doorbell = 1;
    IssueWpqFlush (NvmDimm, TRUE);

  } else {

    CmdReg.Data = CmdRegPtr->Data;
    //
    // Per NVMCTLR Spec, toggle the Sequence Bit everytime we send a command so FW knows if the command is a retry.
    //
    (CmdRegPtr->Bits.sequence_bit) ? (CmdReg.Bits.sequence_bit = 0) : (CmdReg.Bits.sequence_bit = 1);

    //
    // Set the Opcode.
    //
    CmdReg.Bits.opcode = Opcode;
    DurableCacheLineWrite ((UINTN) CmdRegPtr, CmdReg.Data, sizeof (UINT64));

    //
    // If the Security Nonce has been set, copy the Nonce to the Nonce0 register (and only if we use BIOS Mailbox)
    //
    if ((NvmDimm->NonceSet) && (!mUseOSMailbox)) {
      DurableCacheLineWrite ((UINTN) Mailbox->pNonce0, NvmDimm->Nonce, sizeof (UINT64));
    }

    //
    // Set the doorbell to tell the controller that the command is ready to be processed.
    //
    CmdReg.Bits.doorbell = 1;
    DurableCacheLineWrite ((UINTN) CmdRegPtr, CmdReg.Data, sizeof (UINT64));
  }

  TimeoutCounter = Timeout / COMMON_DELAY_UNIT;
  CmdReg.Data = CmdRegPtr->Data;
  StartTime = GetPerformanceCounter ();
  while (CmdReg.Bits.doorbell && TimeoutCounter) {
    if (mEadrProtocol != NULL) {
      Status = mEadrProtocol->EadrCheckStatusAndExecute ();
      if (EFI_ERROR (Status)) {
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("FIS") "Failed to obtain eADR status: %r\n", Status));
      }
    }

    MicroSecondDelay (COMMON_DELAY_UNIT);
    TimeoutCounter--;
    CmdReg.Data = CmdRegPtr->Data;
  }
  StsReg.Data = StsRegPtr->Data;
  EndTime = GetPerformanceCounter ();

  CounterCycle = PerfCntProp.CntEndValue - PerfCntProp.CntStartValue;
  if (CounterCycle < 0) {
    CounterCycle = -CounterCycle;
  }
  CounterCycle++;

  WaitTime = EndTime - StartTime;
  if (PerfCntProp.CntStartValue > PerfCntProp.CntEndValue) {
    WaitTime = -WaitTime;
  }

  if (WaitTime < 0) {
    WaitTime += CounterCycle;
  }

  TotalTimeInNanoSeconds = GetTimeInNanoSecond (WaitTime);

  //
  // Clear Nonce, in case it wasn't cleared by the controller, but only
  // if the Security Nonce has been set (and only if we use BIOS Mailbox)
  // don't clear when DB is still set, since it's R/O)
  //
  if ((NvmDimm->NonceSet) && (!mUseOSMailbox) && (!CmdReg.Bits.doorbell)) {
    if (mUseWpqFlush) {
      IssueWpqFlush (NvmDimm, TRUE);
      *(UINT64 *) (UINTN) Mailbox->pNonce0 = 0;
      IssueWpqFlush (NvmDimm, TRUE);
    } else {
      DurableCacheLineWrite ((UINTN) Mailbox->pNonce0, 0, sizeof (UINT64));
    }
  }

  //
  // If doorbell is still set, return command failure status.
  //
  if (CmdReg.Bits.doorbell) {

    Status = EFI_NO_RESPONSE;
    goto GetOut;
  }

  //
  // If the completion bit is not set and doorbell is cleared it means that
  // FW rejected the cmd as duplicated. Return command failure status.
  //
  if (!StsReg.Bits.comp) {

    Status = EFI_DEVICE_ERROR;
    goto GetOut;
  }

  //
  // Copy Output Payload
  //
  if (OutputPayload != NULL) {

    ReadFromSmallPayload (NvmDimm, OutputPayload, sizeof(UINT32) * NUM_PAYLOAD_REG, mUseOSMailbox);
#if FIS_DUMP_SIZE
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("FIS") "S%d.C%d.D%d: Request 0x%04X output:\n", Socket, Ch, Dimm, Opcode));
    DisplayBuffer (DEBUG_INFO, (UINT8*)OutputPayload, MIN (FIS_DUMP_SIZE, sizeof(UINT32) * NUM_PAYLOAD_REG));
#endif
  }

  //
  // Check Mailbox status
  //
  if (FisStsPtr != NULL) {

    *FisStsPtr = StsReg.Bits.stat;
  }

  Status = MailboxStatusDecode ((UINT8)StsReg.Bits.stat, Opcode, InputPayload, &MboxErrStrPtr);

 GetOut:
  if (EFI_ERROR(Status) && (Opcode != GET_LONG_OPERATION || Status != EFI_NOT_STARTED)) {

    if (CmdReg.Bits.doorbell || !StsReg.Bits.comp || StsReg.Bits.nonce_fail || StsReg.Bits.MbDelayed) {

      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("FIS") "S%d.C%d.D%d: [%u]", Socket, Ch, Dimm, TimeStamp));
      if (StsReg.Bits.MbDelayed) {

        CRDEBUG ((DEBUG_ERROR, "Rate Limiting enabled "));
      }
      if (CmdReg.Bits.doorbell) {

        CRDEBUG ((DEBUG_ERROR, "Doorbell still set "));
      }
      if (!StsReg.Bits.comp) {

        CRDEBUG ((DEBUG_ERROR, "Completion bit not set "));
      }
      if (StsReg.Bits.nonce_fail) {

        CRDEBUG ((DEBUG_ERROR, "Nonce mismatch bit set "));
      }
      CRDEBUG ((DEBUG_ERROR, "(cmd: 0x%llX, status: 0x%llX)\n", CmdReg.Data, StsReg.Data));
    }
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("FIS") "S%d.C%d.D%d: [%u] Request 0x%04X on %a mailbox failed after %dus",
            Socket, Ch, Dimm, TimeStamp, Opcode, (mUseOSMailbox) ? "OS" : "SMM", TotalTimeInNanoSeconds / 1000));
    CRDEBUG ((DEBUG_ERROR, " (timeout %dus) return '%r' (%a)\n", Timeout, Status, MboxErrStrPtr));

  } else {

    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("FIS") "S%d.C%d.D%d: [%u] Request 0x%04X on %a mailbox completed in %dus (timeout %dus)\n",
            Socket, Ch, Dimm, TimeStamp, Opcode, (mUseOSMailbox) ? "OS" : "SMM", TotalTimeInNanoSeconds / 1000, Timeout));
    switch (Opcode) {
      case OVERWRITE_DIMM:
      case SET_ADDRESS_RANGE_SCRUB:
      case UPDATE_FNV_FW:
        FisLongOpRunning[Socket][Ch] = Opcode;
        mNvdimmAcpiSmmInterface->LongOpStatus[Socket][Ch] = Opcode;
        FisLongOpSeqNum[Socket][Ch]++;
        break;
    }
  }
  if (DoNotify) {
    AcpiRootNotifyEx (TRUE);
  }
  return Status;
}

/**
  @brief Converts mailbox status to EFI status and message describing the status.

  @param[in]  MboxStatus   - FIS status value
  @param[in]  CmdOpcode    - The command that is being executed
  @param[in]  InputPayload  - Pointer to buffer containing Input Payload data (NULL if not needed)
  @param[out] ErrStrPtr    - Messsage describing the status

  @return EFI status value corresponding to mailbox status code is returned.
**/
EFI_STATUS
MailboxStatusDecode (
  IN       UINT8   MboxStatus,
  IN       UINT16  CmdOpcode,
  IN CONST UINT32  *InputPayload,
  OUT      CHAR8   **ErrStrPtr
  )
{
  EFI_STATUS            Status;
  FIS_FW_DEBUG_LOG_REQ  *DebugLogReq;

  static CHAR8 UnknownStsStr[64];

  switch (MboxStatus) {

  case FIS_STS_SUCCESS:
    Status = EFI_SUCCESS;
    *ErrStrPtr = "Mailbox SUCCESS";
    break;
  case FIS_STS_INVALID_PARAM:
    Status = EFI_INVALID_PARAMETER;
    *ErrStrPtr = "Mailbox Failure: Invalid Command Parameter";
    break;
  case FIS_STS_DATA_XFER_ERR:
    Status = EFI_DEVICE_ERROR;
    *ErrStrPtr = "Mailbox Failure: Data Transfer Error";
    break;
  case FIS_STS_INTERNAL_ERR:
    if (CmdOpcode == GET_LONG_OPERATION) {
      //
      // Up to FIS 1.4 InternalError is returned if no long op was stared.
      // But we should not report it as error, just long op status is not available yet.
      //
      Status = EFI_NOT_STARTED;
      *ErrStrPtr = "Mailbox: No Long Operation Started Yet";

    } else {

      Status = EFI_DEVICE_ERROR;
      *ErrStrPtr = "Mailbox Failure: Internal Device Error";
    }
    break;
  case FIS_STS_UNSUPPORTED_CMD:
    Status = EFI_UNSUPPORTED;
    *ErrStrPtr = "Mailbox Failure: Unsupported Command";
    break;
  case FIS_STS_DEVICE_BUSY:
    Status = EFI_NOT_READY;
    *ErrStrPtr = "Mailbox Failure: Device Busy";
    break;
  case FIS_STS_INCORRECT_NONCE:
    Status = EFI_ACCESS_DENIED;
    *ErrStrPtr = "Mailbox Failure: Incorrect Passphrase/Security Nonce";
    break;
  case FIS_STS_SECURITY_CHK_FAIL:
    Status = EFI_ACCESS_DENIED;
    *ErrStrPtr = "Mailbox Failure: FW Authentification Failed";
    break;
  case FIS_STS_INVALID_SEC_STATE:
    Status = EFI_SECURITY_VIOLATION;
    *ErrStrPtr = "Mailbox Failure: Invalid Security State";
    break;
  case FIS_STS_SYS_TIME_NOT_SET:
    Status = EFI_NOT_STARTED;
    *ErrStrPtr = "Mailbox Failure: System Time Not Set";
    break;
  case FIS_STS_DATA_NOT_SET:
    if (CmdOpcode == GET_LONG_OPERATION) {
      //
      // Since FIS 1.5 DataNotSet is returned if no long op was stared.
      // But we should not report it as error, just long op status is not available yet.
      //
      Status = EFI_NOT_STARTED;
      *ErrStrPtr = "Mailbox: No Long Operation Started Yet";
    } else {
      if ((CmdOpcode == GET_FIRMWARE_DEBUG) && (NULL != InputPayload)) {
        DebugLogReq = (FIS_FW_DEBUG_LOG_REQ*)(InputPayload);
        if (DebugLogReq->Bits.LogAction == FIS_FW_DEBUG_LOG_GET_SPI_LOG_PAGE) {
          //
          // Log page is backed up in SPI log region if a fatal error occurred.
          // Get SPI Log Page returns DATA NOT SET status code when no log is saved in SPI
          //
          Status = EFI_NOT_FOUND;
          *ErrStrPtr = "Mailbox: Data Not Found";
        } else {
          Status = EFI_DEVICE_ERROR;
          *ErrStrPtr = "Mailbox Failure: Data Not Set";
        }
      } else {
        Status = EFI_DEVICE_ERROR;
        *ErrStrPtr = "Mailbox Failure: Data Not Set";
      }
    }
    break;
  case FIS_STS_ABORTED:
    Status = EFI_ABORTED;
    *ErrStrPtr = "Mailbox Failure: Aborted";
    break;
  case FIS_STS_NO_NEW_FW:
    Status = EFI_DEVICE_ERROR;
    *ErrStrPtr = "Mailbox Failure: No New FW to Execute";
    break;
  case FIS_STS_REVISION_FAIL:
    Status = EFI_DEVICE_ERROR;
    *ErrStrPtr = "Mailbox Failure: Revision Failure";
    break;
  case FIS_STS_INJECT_NOT_ENABLED:
    Status = EFI_UNSUPPORTED;
    *ErrStrPtr = "Mailbox Failure: Injection Not Enabled";
    break;
  case FIS_STS_CONFIG_LOCKED:
    Status = EFI_WRITE_PROTECTED;
    *ErrStrPtr = "Mailbox Failure: Config Lockdown";
    break;
  case FIS_STS_INVALID_ALIGNMENT:
    Status = EFI_INVALID_PARAMETER;
    *ErrStrPtr = "Mailbox Failure: Invalid Alignment";
    break;
  case FIS_STS_INCOMPATIBLE_DIMM:
    Status = EFI_INCOMPATIBLE_VERSION;
    *ErrStrPtr = "Mailbox Failure: Incompatible DIMM Type";
    break;
  case FIS_STS_TIMEOUT:
    Status = EFI_TIMEOUT;
    *ErrStrPtr = "Mailbox Failure: Timeout Occured";
    break;
  case FIS_STS_MEDIA_DISABLED:
    Status = EFI_NO_MEDIA;
    *ErrStrPtr = "Mailbox Failure: Media Disabled";
    break;
  case FIS_STS_FW_UPDATE_ALREADY_OCCURRED:
    Status = EFI_DEVICE_ERROR;
    *ErrStrPtr = "Mailbox Failure: FW Update Already Occurred";
    break;
  case FIS_STS_NO_RESOURCES_AVAILABLE:
    Status = EFI_OUT_OF_RESOURCES;
    *ErrStrPtr = "Mailbox Failure: No Resources Available";
    break;
  default:
    Status = EFI_NO_RESPONSE;
    AsciiSPrint (&UnknownStsStr[0], sizeof (UnknownStsStr), "Mailbox Failure: Unknown status %d", MboxStatus);
    *ErrStrPtr = &UnknownStsStr[0];
  }
  return  Status;
}


/**
 @brief Sends a command to NVDIMM via Small Payload registers using SMBUS mailbox as defined in FIS.

 Note that this function actively waits for NVDIMM FW response.

 @param[in]  Socket        - Socket index
 @param[in]  Ch            - DDR Channel ID
 @param[in]  Dimm          - DIMM Number
 @param[in]  InputPayload  - Pointer to buffer containing Input Payload data (NULL if not needed)
 @param[in]  InputLength   - Length of input payload data (may be 0 if no input)
 @param[out] OutputPayload - Pointer to buffer for Output Payload data (NULL if not needed)
 @param[in]  Opcode        - Opcode of the command to be sent
 @param[in]  Timeout       - Optional timeout in us, use zero to assign default timeout
 @param[out] FisStsPtr     - Optional pointer to buffer for FIS status value (NULL if unused)

 @return NVDIMM status translated to EFI_STATUS is returned.

**/
EFI_STATUS
FisSmbusRequest (
  IN  UINT8       Skt,
  IN  UINT8       Ch,
  IN  UINT8       Dimm,
  IN  UINT32      *InputPayload,
  IN  UINT32      InputLength,
  OUT UINT32      OutputPayload[NUM_PAYLOAD_REG],
  IN  UINT16      Opcode,
  IN  UINT64      Timeout,
  OUT FIS_STATUS *FisStsPtr
  )
{
  EFI_STATUS      Status;
  UINT32          InputPayloadFull[NUM_PAYLOAD_REG];

  if (InputLength > NUM_PAYLOAD_REG * sizeof (UINT32)) {
    InputLength = NUM_PAYLOAD_REG * sizeof (UINT32);
  }

  ZeroMem (InputPayloadFull, sizeof (InputPayloadFull));
  if (InputPayload) {
    CopyMem (InputPayloadFull, InputPayload, InputLength);
  }

  *FisStsPtr = FIS_STS_UNDEFINED;
  Status = NvdimmSmbusAccessMailboxInterface (Skt, Ch, Dimm, InputPayloadFull, OutputPayload, Opcode, Timeout, (UINT8*)FisStsPtr);
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("FIS") "S%d.C%d.D%d: SMBus mailbox interface failed (status %r)\n", Skt, Ch, Dimm, Status));
    return Status;
  }

  return EFI_SUCCESS;
}

