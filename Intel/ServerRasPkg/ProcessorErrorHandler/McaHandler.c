/** @file
  Implementation of the MCE Error Handler for eMCA Gen 2.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2017 Intel Corporation. <BR>

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

#include "ProcessorErrorHandler.h"
#include <Library/CrystalRidgeFlushNearMemoryLib.h>
#include <Library/ProcessorRasLib.h>

extern PROCESSOR_RAS_SETUP  mProcessorRasSetup;

EFI_CRYSTALRIDGE_FLUSH_NEAR_MEMORY_PROTOCOL *mCrystalRidgeFlushNearMemoryProtocol = NULL;

/**
  Shutdown.
**/
VOID
MceShutdown(
  IN UINT8 Policy
  )
{
  if(Policy == 0) {
    CpuDeadLoop();
  } else {
    ResetWarm();
  }
}

BOOLEAN
DetectSmmErrorContention (
  IN   SMM_MCA_CPU_INFO             *CpuInfo,
  IN   EMCA_MC_SIGNATURE_ERR_RECORD *McSig
  )
{
  EFI_PHYSICAL_ADDRESS PhyAddr;
  /*
   * Need to add logic to check for MMIO address and detect a safe range to return to.
  */

  PhyAddr = GetMcAddress (McSig);
  if(PhyAddr != MAX_ADDRESS) {
    //
    // Check if the error is in SMRAM to avoid returning to the SMM handler if the cause of error is the handler.
    //
    if (!SmmIsBufferOutsideSmmValid (PhyAddr, 64)) {
      RAS_DEBUG ((MCE_PRINT | DEBUG_ERROR, "MCE %ld: MCAddress is in SMM\n", CpuInfo->CpuIndex));
      return TRUE;
    }
  } else {
    return TRUE;
  }

  return FALSE;
}

VOID
DetectHandleMcBankContention (
  IN        SMM_MCA_CPU_INFO         *CpuInfo,
  IN        UINT64                   *SmiSrc,
  IN        UINT64                   *ProcessingBank
  )
{
  UINTN                             CpuNumber;
  UINT8                             McBank;

  CpuNumber = CpuInfo->CpuIndex;

  RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "DetectHandleMcBankContention[%ld]: SmiSrc=%lx ProcessingBank=%lx\n", CpuNumber, *SmiSrc, *ProcessingBank));

  if ((*SmiSrc & *ProcessingBank) != 0) {
    RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "DetectHandleMcBankContention[%ld]: SmmMceSyncState=%lx\n", CpuNumber, gSmmErrorCpuData[CpuNumber].SmmMceSyncState));
    switch (gSmmErrorCpuData[CpuNumber].SmmMceSyncState) {
      case SmmMceReadingState:
        gSmmErrorCpuData[CpuNumber].SmmMceSyncState = SmmMceReadingInterrupted;
        break;
      case SmmMceProcessingState:
        gSmmErrorCpuData[CpuNumber].SmmMceSyncState = SmmMceProcessingInterrupted;
        break;
      case SmmMceWritingState:
        McBank = (UINT8) HighBitSet64 (*ProcessingBank);
        SetMcBankFirmwareUpdatedBit (McBank);
        gSmmErrorCpuData[CpuNumber].SmmMceSyncState = SmmMceWritingInterrupted;
        break;
      case SmmMceLoggingState:
      case SmmMceClearedState:
        //Do nothing
        break;
      default:
        ASSERT (FALSE);
        break;
    }
  }
}

/**
  Process core errors
  @param  CpuInfo     Cpu Information structure

**/
VOID
McaProcessErrors (
  IN        SMM_MCA_CPU_INFO         *CpuInfo,
  IN        UINT64                   *SmiSrc,
  IN        UINT64                   *ProcessingBank
)
{
  UINTN                             CpuNumber;
  UINT64                            NewErrorsBitmap;
  UINT8                             McBank;
  EMCA_MC_SIGNATURE_ERR_RECORD      McErrSig;
  MCE_CLASS                         MceClass;

  CpuNumber = CpuInfo->CpuIndex;

  switch(GetEmcaHandlerState()) {
    case SmmMceNotStartedState:
      //nothing to do but return
      return;
      break;
    case SmmMceInProgressState:
      DetectHandleMcBankContention(CpuInfo, SmiSrc, ProcessingBank);
      break;
    case SmmMceFinishedState:
      RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "McaProcessErrors[%ld]: Set pending flag to TRUE\n", CpuNumber));
      SetPendSmiFlag(TRUE);
      break;
    default:
      DEBUG ((EFI_D_INFO, "default case.\n"));  //Auto added. Please review.
      break;
  }
  NewErrorsBitmap = *SmiSrc;
  while (NewErrorsBitmap != 0) {
    RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "McaProcessErrors[%ld]: SMISrc %lx \n", CpuNumber, NewErrorsBitmap));
    McBank = (UINT8) HighBitSet64 (NewErrorsBitmap);
    RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "McaProcessErrors[%ld]: McBank = %d\n", CpuNumber, McBank));

    NewErrorsBitmap &= ~LShiftU64 (1, McBank);

    ReadMcBankSigHook (McBank, (UINT8) CpuInfo->Package, &McErrSig);

    RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "McaProcessErrors[%ld]: MCi_STS: %lx, MCi_ADDR: %lx, MCi_MISC: %lx, APICID: %lx\n", CpuNumber,
                                                                                   McErrSig.Signature.McSts,
                                                                                   McErrSig.Signature.McAddr,
                                                                                   McErrSig.Signature.McMisc,
                                                                                   McErrSig.ApicId));

    MceClass = GetMceClass(&McErrSig);
    switch(MceClass) {
      case INVALID:
        RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "McaProcessErrors[%ld] MCE Class INVALID: %ld\n", CpuNumber, MceClass));
        // Proceed
        break;
      case CORRECTED:
        RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "McaProcessErrors[%ld]: MCE Class CORRECTED: %ld\n", CpuNumber, MceClass));
        // Proceed
        break;
      case UCNA:
        RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "McaProcessErrors[%ld]: MCE Class UCNA: %ld\n", CpuNumber, MceClass));
        // Proceed
        break;
      case SRAO:
        RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "McaProcessErrors[%ld]: MCE Class SRAO: %ld\n", CpuNumber, MceClass));
        // Proceed
        break;
      case SRAR:
        RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "McaProcessErrors[%ld]: MCE Class SRAR: %ld\n", CpuNumber, MceClass));
        if(DetectSmmErrorContention(CpuInfo,&McErrSig)) {
          RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "McaProcessErrors[%ld]: Smm Error Contention\n", CpuNumber));
          RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "McaProcessErrors[%ld]: Perform Reset\n", CpuNumber));
          MceShutdown(0);
        }
        break;
      case DUE:
        RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "McaProcessErrors[%ld]: MCE Class DUE: %ld\n", CpuNumber, MceClass));
        RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "McaProcessErrors[%ld]: Perform Reset\n", CpuNumber));
        MceShutdown(0);
        break;
      case FATAL:
        RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "McaProcessErrors[%ld]: MCE Class FATAL: %ld\n", CpuInfo->CpuIndex, MceClass));
        RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "McaProcessErrors[%ld]: Perform Reset\n", CpuInfo->CpuIndex));
        MceShutdown(0);
        break;
      default:
        DEBUG ((EFI_D_INFO, "default case.\n"));  //Auto added. Please review.
        break;
    }
  }
}


/**
  Process core errors
  @param  CpuInfo     Cpu Information structure

**/
VOID
McaProcessCoreErrors (
  IN        SMM_MCA_CPU_INFO        *CpuInfo
)
{
  UINT64                            CoreSmiSrc;
  UINTN                             CpuNumber;

  CpuNumber = CpuInfo->CpuIndex;
  CoreSmiSrc = ReadCoreSmiSrc ();

  RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "[Mca] McaProcessCoreErrors %ld: Core SMI source %lx\n", CpuNumber, CoreSmiSrc));
  McaProcessErrors (CpuInfo, &CoreSmiSrc, (UINT64 *) &gSmmErrorCpuData[CpuNumber].CoreProcessingBank);
}

/**
  Process uncore errors
  @param  CpuInfo     Cpu Information structure

**/
VOID
McaProcessUncoreErrors (
  IN        SMM_MCA_CPU_INFO        *CpuInfo
)
{
  UINT64                            UncoreSmiSrc;
  UINTN                             CpuNumber;

  CpuNumber = CpuInfo->CpuIndex;
  UncoreSmiSrc = ReadUncoreSmiSrc ();

  RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "[Mca] McaProcessUncoreErrors %ld: Uncore SMI source %lx\n", CpuNumber, UncoreSmiSrc));
  McaProcessErrors (CpuInfo, &UncoreSmiSrc, (UINT64 *) &gSmmErrorCpuData[CpuNumber].UncoreProcessingBank);
}

/**
  Process FlushLoop core errors
  @param [in] CpuInfo - Cpu information structure
  @param [in] SmiSrc -  Smi source
  @param [in] ProcessingBank - Current processing bank

**/
VOID
McaProcessFlushLoopErrors (
  IN        SMM_MCA_CPU_INFO        *CpuInfo,
  IN        UINT64                   *SmiSrc,
  IN        UINT64                   *ProcessingBank
  )
{
  UINTN                             CpuNumber;
  UINT64                            NewErrorsBitmap;
  UINT8                             McBank;
  EMCA_MC_SIGNATURE_ERR_RECORD      McErrSig;
  MCE_CLASS                         MceClass;
  UINT32                            *Errcnt = mCrystalRidgeFlushNearMemoryProtocol->ErrCnt;
  CpuNumber = CpuInfo->CpuIndex;

  switch(GetEmcaHandlerState()) {
    case SmmMceNotStartedState:
      //nothing to do but return
      return;
      break;
    case SmmMceInProgressState:
      DetectHandleMcBankContention(CpuInfo,SmiSrc,ProcessingBank);
      break;
    case SmmMceFinishedState:
      SetPendSmiFlag(TRUE);
      break;
    default:
      break;
  }

  NewErrorsBitmap = *SmiSrc;
  while (NewErrorsBitmap != 0) {
    RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "McaProcessErrors[%ld]: SMISrc %lx \n", CpuNumber, NewErrorsBitmap));
    McBank = (UINT8) HighBitSet64 (NewErrorsBitmap);
    RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "McaProcessErrors[%ld]: McBank = %d\n", CpuNumber, McBank));

    NewErrorsBitmap &= ~LShiftU64 (1, McBank);


    ReadMcBankSigHook (McBank, (UINT8) CpuInfo->Package, &McErrSig);

    RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "McaProcessErrors[%ld]: MCi_STS: %lx, MCi_ADDR: %lx, MCi_MISC: %lx, APICID: %lx\n", CpuNumber,
                                              McErrSig.Signature.McSts,
                                              McErrSig.Signature.McAddr,
                                              McErrSig.Signature.McMisc,
                                              McErrSig.ApicId));

    MceClass = GetMceClass(&McErrSig);

    switch(MceClass) {
      case INVALID:
        RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "McaProcessErrors[%ld] MCE Class INVALID: %ld\n", CpuNumber, MceClass));
        // Numberof NM flush Errors
        *Errcnt = *Errcnt + 1;
        // Proceed
        break;
      case CORRECTED:
        RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "McaProcessErrors[%ld]: MCE Class CORRECTED: %ld\n", CpuNumber, MceClass));
        // Proceed
        break;
      case UCNA:
        RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "McaProcessErrors[%ld]: MCE Class UCNA: %ld\n", CpuNumber, MceClass));
        // Numberof NM flush Errors
        *Errcnt = *Errcnt + 1;
        // Proceed
        break;
      case SRAO:
        RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "McaProcessErrors[%ld]: MCE Class SRAO: %ld\n", CpuNumber, MceClass));
        // Numberof NM flush Errors
        *Errcnt = *Errcnt + 1;
        // Proceed
        break;
      case SRAR:
        RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "McaProcessErrors[%ld]: MCE Class SRAR: %ld\n", CpuNumber, MceClass));
        //if(DetectSmmErrorContention(CpuInfo,&McErrSig)) {
          //McePrint("MCE %ld: Smm Error Contention\n",CpuInfo->CpuIndex);
          //McePrint("MCE %ld: Perform Reset\n",CpuInfo->CpuIndex);
          //MceShutdown(0);
        //}
        // Numberof NM flush Errors
        *Errcnt = *Errcnt + 1;
        break;
      case DUE:
        RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "McaProcessErrors[%ld]: MCE Class DUE: %ld\n", CpuNumber, MceClass));
        RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "McaProcessErrors[%ld]: Perform Reset\n", CpuNumber));
        //MceShutdown(0);
        // Numberof NM flush Errors
        *Errcnt = *Errcnt + 1;
        break;
      case FATAL:
        RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "McaProcessErrors[%ld]: MCE Class FATAL: %ld\n", CpuInfo->CpuIndex, MceClass));
        RAS_DEBUG ((MCE_PRINT | LEVEL_FUNC_FLOW, "McaProcessErrors[%ld]: Perform Reset\n", CpuInfo->CpuIndex));
        //MceShutdown(0);
        // Numberof NM flush Errors
        *Errcnt = *Errcnt + 1;
        break;
      default:
        DEBUG ((EFI_D_INFO, "default case.\n"));  //Auto added. Please review.
        break;
    }
    // Clear MCE(no re-boot/stop execution) in Pmem flushing scenario
    ClearMcBank(McBank);
  }
}

/**
  Process FlushLoop core errors
  @param  [in] CpuInfo     Cpu Information structure

**/
VOID
McaProcessFlushLoopCoreErrors (
  IN        SMM_MCA_CPU_INFO        *CpuInfo
)
{
  UINT64  CoreSmiSrc;

  //McePrint("t %ld: Core Flsuh loop handler\n", CpuNumber);
  CoreSmiSrc = ReadCoreSmiSrc ();
  McaProcessFlushLoopErrors(CpuInfo, &CoreSmiSrc, (UINT64 *)&gSmmErrorCpuData[CpuInfo->CpuIndex].CoreProcessingBank);
}

/**
  Process FlushLoop uncore errors
  @param [in] CpuInfo     Cpu Information structure

**/
VOID
McaProcessFlushLoopUncoreErrors (
  IN        SMM_MCA_CPU_INFO        *CpuInfo
)
{
  UINT64  UncoreSmiSrc;

  //McePrint("t %ld: Uncore Flush Loop handler\n",CpuNumber);
  UncoreSmiSrc = ReadUncoreSmiSrc ();
  McaProcessFlushLoopErrors(CpuInfo, &UncoreSmiSrc, (UINT64 *)&gSmmErrorCpuData[CpuInfo->CpuIndex].UncoreProcessingBank);
}

/**
  MCA handler for NM flush Errors.
  Regardless 0f MCE we will continue to flush remaiing tasks.

  @param [in] InterruptType - Type of exception
  @param [in] SystemContext - System context

**/

VOID
EFIAPI
SmiMcaFlushLoopHandler (
  IN        EFI_EXCEPTION_TYPE    InterruptType,
  IN        EFI_SYSTEM_CONTEXT    SystemContext
)
{
  SMM_MCA_CPU_INFO CpuInfo;
  MP_CPU_INFO      MpCpuInfo;

  CpuInfo.CpuIndex = GetProcessorInfo (&MpCpuInfo);
  CpuInfo.Package = MpCpuInfo.Package;

  if (IsMcip () == FALSE) {
    RAS_DEBUG ((MCE_PRINT | LEVEL_BASIC_FLOW, "MCE %ld: MCIP == 0\n", CpuInfo.CpuIndex));
    ASSERT(FALSE);
    //MceShutdown(0);
    return;
  }

  // TBD: OEM has to pharse MC bank's to decide which information thay want to save
  McaProcessFlushLoopUncoreErrors (&CpuInfo);
  McaProcessFlushLoopCoreErrors (&CpuInfo);

  // Call back to save NM flush status info
  // For OEM's to save MCE record to their NVM storage, args to be passed which describes the Error, Address
  if (mCrystalRidgeFlushNearMemoryProtocol->CrNmFlushErrInfo != NULL) {
    mCrystalRidgeFlushNearMemoryProtocol->CrNmFlushErrInfo();
  }

  //McePrint ("MCE %ld: MceRestart\n",CpuInfo.CpuIndex);
  ClearMcip ();
}

/**
  MCA handler for SMM use.

  @param [in] InterruptType - Type of exception
  @param [in] SystemContext - System context
**/
VOID
EFIAPI
SmiMcaHandler (
  IN        EFI_EXCEPTION_TYPE    InterruptType,
  IN        EFI_SYSTEM_CONTEXT    SystemContext
)
{
  SMM_MCA_CPU_INFO          CpuInfo;
  MP_CPU_INFO               MpCpuInfo;
  EFI_STATUS                Status;
  UINT8                     *InCrFlushProtocol = NULL;

  if (mProcessorRasSetup.McaSpinLoop) {
    RAS_DEBUG ((MCE_PRINT | LEVEL_BASIC_FLOW, "MCA Spin Loop\n"));
    MceShutdown (0);
  }

  // Locate NM flush smm protocol
  Status = gSmst->SmmLocateProtocol (&gEfiCrystalRidgeFlushNearMemoryProtocolGuid, NULL, &mCrystalRidgeFlushNearMemoryProtocol);
  if (Status != EFI_SUCCESS) {
    //McePrint("Unable to locate gEfiCrystalRidgeFlushNearMemoryProtocolGuid\n")
    ASSERT(Status);
  }

  // Check whether this MCS is from Pmem cache flush
  // If yes, increment Err counter and issue callback to record the Err
  if (mCrystalRidgeFlushNearMemoryProtocol != NULL) {
    InCrFlushProtocol = mCrystalRidgeFlushNearMemoryProtocol->InCrystalridgeFlushNMProtocol;
    if ((*InCrFlushProtocol) == CR_FLUSH_NM_ETR) {
      SmiMcaFlushLoopHandler(InterruptType, SystemContext);
      return;
    }
  }

  CpuInfo.CpuIndex = GetProcessorInfo (&MpCpuInfo);
  CpuInfo.Package = MpCpuInfo.Package;

  RAS_DEBUG ((MCE_PRINT | LEVEL_BASIC_FLOW, "MCE %ld: SmiMcaHandler Entry: \n", CpuInfo.CpuIndex));

  if (!IsMcip ()) {
    RAS_DEBUG ((MCE_PRINT | LEVEL_BASIC_FLOW, "MCE %ld: MCIP == 0\n", CpuInfo.CpuIndex));
    ASSERT(FALSE);
    MceShutdown(0);
    return;
  }

  if(!IsRipv()) {
    RAS_DEBUG ((MCE_PRINT | LEVEL_BASIC_FLOW, "MCE %ld: RIPV == 0\n", CpuInfo.CpuIndex));
    RAS_DEBUG ((MCE_PRINT | LEVEL_BASIC_FLOW, "MCE %ld: Perform Reset\n", CpuInfo.CpuIndex));
    MceShutdown(0);
  }

  McaProcessUncoreErrors (&CpuInfo);
  McaProcessCoreErrors (&CpuInfo);


  RAS_DEBUG ((MCE_PRINT | LEVEL_BASIC_FLOW, "MCE %ld: MceRestart\n", CpuInfo.CpuIndex));
  ClearMcip ();
}

