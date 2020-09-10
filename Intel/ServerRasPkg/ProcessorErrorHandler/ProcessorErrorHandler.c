/** @file
  Implementation of the CPU Core Error Handler.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2020 Intel Corporation. <BR>

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
#include <Library/PerformanceLib.h>
// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#include <Library/AmiOemRasLib.h>
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support

// Module data
PROCESSOR_RAS_SETUP                   mProcessorRasSetup;
PROCESSOR_RAS_CAPABILITY              mProcessorRasCap;
EFI_SMM_CPU_SERVICE_PROTOCOL          *mSmmCpuService;
UINT32                                mSeverity;
BOOLEAN                               mBroadcast = FALSE;
EFI_HANDLE                            mHandle = NULL;
UINT8                                 mMode;
RAS_POLICY                            *mPolicyData;
extern MEM_TOPOLOGY                   *mMemTopology;

// Global data
SMM_ERROR_CPU_DATA_BLOCK              *gSmmErrorCpuData = NULL;

#define  MCACOD_IOMCA         0x0E0B
#define  MSCOD_IOMCA          0
#define  MLC_YELLOW_MARK_MASK (BIT53|BIT54)
#define  MLC_YELLOW_MARK_IND  BIT54
#define  MSCOD_UPI_FAILOVER   0x22
#define  LOW_LANE_ACTIVE      0x05

UINT8
GetMcaBankErrorType (
  IN UINT32 McBankIdx
  )
{
  UINT32       UnitType;
  UINT8        ErrorType;

  UnitType = GetMcBankUnitType (McBankIdx);
  switch (UnitType) {
  case MCA_UNIT_TYPE_IFU:
  case MCA_UNIT_TYPE_DCU:
  case MCA_UNIT_TYPE_DTLB:
  case MCA_UNIT_TYPE_MLC:
    ErrorType = EFI_GENERIC_ERROR_PROC_ERROR_TYPE_MICRO_ARCH;
    break;
  case MCA_UNIT_TYPE_CHA:
    ErrorType = EFI_GENERIC_ERROR_PROC_ERROR_TYPE_CACHE;
    break;
  default:
    ErrorType = EFI_GENERIC_ERROR_PROC_ERROR_TYPE_UNKNOWN;
  }

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "[Mca]GetMcaBankErrorType bank 0x%x, UnitType 0x%x, Error Type 0x%x\n", McBankIdx, UnitType, ErrorType));
  return ErrorType;
}

// Function definitions

////////////////////////////////////// Common Routines //////////////////////////////////////

/**
  Check the lmce status for all the threads and decide if MCE needs to be
  broadcast or local.

**/
BOOLEAN
EFIAPI
CheckLmceStatus(
  VOID
  )
{
  BOOLEAN Broadcast;
  BOOLEAN Lmce;

  if (mProcessorRasCap.LmceCap && mProcessorRasSetup.LmceEn) {
    Broadcast = IsBmce ();
    Lmce = IsLmce ();
    //if broadcast == true && lmce == true then clear lmce
    //if broadcast == false && lmce == false then default to broadcast.
    if(Broadcast) {
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "Broadcast == TRUE\n"));
      if(Lmce) {
        RAS_DEBUG ((LEVEL_FUNC_FLOW, "Lmce == TRUE\n"));
        RAS_DEBUG ((LEVEL_FUNC_FLOW, "Clear Lmce Status\n"));
        ClearLmceStatus ();
      }
    } else {
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "Broadcast == FALSE\n"));
      if(!Lmce) {
        RAS_DEBUG ((LEVEL_FUNC_FLOW, "Lmce == FALSE : setting broadcast TRUE\n"));
        Broadcast = TRUE;
      }
    }
  } else {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "LMCE disabled broadcast = TRUE\n"));
    Broadcast = TRUE;
  }

  return Broadcast;
}

/**
  Notify the OS via Processor mechanisms

  This function takes the appropriate action for a particular error based on severity.

  @retval Status.

**/
EFI_STATUS
McaNotifyOs (
  VOID
  )
{
  UINTN                         CpuNumber;
  BOOLEAN                       UncorrectedErrorFound = FALSE;
  BOOLEAN                       CorrectedErrorFound = FALSE;
  volatile SMM_ERROR_EVENT_LOG  *EventLogPtr;
  EFI_PROCESSOR_INFORMATION     ProcessorInfoBuffer;
  UINT32                        McBankIdx;

  if(!mProcessorRasCap.EmcaGen2Cap) {
    return EFI_UNSUPPORTED;
  }

  for (CpuNumber = 0; CpuNumber < gSmst->NumberOfCpus; CpuNumber++) {
    EventLogPtr = &gSmmErrorCpuData[CpuNumber].EventLog;
    if (EventLogPtr->ErrorsFound) {

      if (((EventLogPtr->SeverityMap & LShiftU64 (1, SEVERITY_MAP_RECOVERABLE)) != 0) ||
         ((EventLogPtr->SeverityMap & LShiftU64 (1, SEVERITY_MAP_FATAL)) != 0)) {
        if( mProcessorRasSetup.MsmiEn == MODE_EMCA) {
          RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca]Find uncorrected error at CPU %ld\n", CpuNumber));
          UncorrectedErrorFound = TRUE;
          if ((EventLogPtr->SeverityMap & LShiftU64 (1, SEVERITY_MAP_RECOVERABLE)) != 0) {
            McBankIdx = EventLogPtr->Log[SEVERITY_MAP_RECOVERABLE].BankIdx;
          } else {
            McBankIdx = EventLogPtr->Log[SEVERITY_MAP_FATAL].BankIdx;
          }
        }
      }

      if(((EventLogPtr->SeverityMap & LShiftU64 (1, SEVERITY_MAP_CORRECTED)) != 0)) {
        if(mProcessorRasSetup.CsmiEn == MODE_EMCA) {
          mSmmCpuService->GetProcessorInfo (mSmmCpuService, CpuNumber, &ProcessorInfoBuffer);
          //
          // Socket is die level per MCP, and package level per NON-MCP. Its value is read from CPUID 0x0B.
          //
          RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca]Sending CMCI for CPU 0x%lx Bank 0x%x, Socket 0x%x Core 0x%x\n",
                      CpuNumber,
                      EventLogPtr->Log[SEVERITY_MAP_CORRECTED].BankIdx,
                      ProcessorInfoBuffer.Location.Package,
                      ProcessorInfoBuffer.Location.Core
                      ));
          SignalCmciToOs (EventLogPtr->Log[SEVERITY_MAP_CORRECTED].BankIdx,
                          ProcessorInfoBuffer.Location.Package,
                          ProcessorInfoBuffer.Location.Core,
                          &CpuNumber
                          );
          CorrectedErrorFound = TRUE;
        }
      }
    }
    ZeroMem ((VOID *) EventLogPtr, sizeof (*EventLogPtr));
  }

  if(UncorrectedErrorFound) {
    if (!SkipMceSignalWa1507442372 (McBankIdx)) {
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca]Sending OS notification via MCE\n"));
      SignalMceToOs (gSmst->CpuSaveState, gSmst->CpuSaveStateSize, mBroadcast);
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca]Successfully notify OS via MCE\n"));
    }
  }

  mBroadcast = FALSE;
  return  EFI_SUCCESS;
}

/**
 Put CPU into infinite loop based on setup option.

**/
VOID
SpinLoopOnFatalErr(
  VOID
  )
{
  if(mProcessorRasSetup.FatalErrSpinLoopEn) {
    RAS_DEBUG((MP_PRINT | LEVEL_FUNC_FLOW, "[Mca]SpinLoopOnFatalErr: dead loop function\n"));
    CpuDeadLoop();
  } else {
    RAS_DEBUG((MP_PRINT | LEVEL_FUNC_FLOW, "[Mca]SpinLoopOnFatalErr: skipped dead loop\n"));
  }
  return;
}



/**
  Get error severity from error signature.

  @param[in] MceClass   MC Error class

  @retval Sev   Severity

**/
UINT32
MapErrorSeverity (
  IN      MCE_CLASS  MceClass
  )
{
  UINT32                                Severity;

  RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "[Mca]MapErrorSeverity: MCE class is 0x%x\n", MceClass));
  switch (MceClass) {
  case SRAR:
  case SRAO:
    Severity = EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTABLE;
    break;
  case CORRECTED:
  case UCNA:
    Severity = EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTED;
    break;
  case FATAL:
    Severity = EFI_ACPI_6_2_ERROR_SEVERITY_FATAL;
    SpinLoopOnFatalErr();
    break;
  default:
    Severity = EFI_ACPI_6_2_ERROR_SEVERITY_NONE;
    break;
  }

  return Severity;
}

/**
  Returns the more severe of the two inputs.

  @param[in] NewSev   The newer severity
  @param[in] OldSev   The old severity

  @retval The more severe of the two severities.

**/
UINT32
McaGetHighestSeverity (
  IN      UINT32    NewSev,
  IN      UINT32    OldSev
  )
{
  switch (NewSev) {
  case EFI_ACPI_6_2_ERROR_SEVERITY_FATAL:
    return NewSev;
    break;
  case EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTABLE:
    if (OldSev == EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTED || OldSev == EFI_ACPI_6_2_ERROR_SEVERITY_NONE) {
      return NewSev;
    }
    break;
  case EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTED:
    if (OldSev == EFI_ACPI_6_2_ERROR_SEVERITY_NONE) {
      return NewSev;
    }
    break;
  case EFI_ACPI_6_2_ERROR_SEVERITY_NONE:
    return OldSev;
  default:
    RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "[Mca]McaGetHighestSeverity: Invalid error severity: 0x%x\n", NewSev));
    RAS_ASSERT (FALSE);
    break;
  }
  return OldSev;
}

/**
  Handle an uncorrected machine check bank error.

  @param[in] McBank   The machine check bank to handle
  @param[in] skt      The socket number
  @param[in] CpuData  Cpu data block
  @param[out] ErrSts  Error Status

**/
VOID
McBankErrorHandler (
  IN      UINT8                     McBank,
  IN      UINT8                     Socket,
  IN      SMM_ERROR_CPU_DATA_BLOCK  *CpuData
  )
{
  EFI_STATUS                        Status;
  MCE_CLASS                         MceClass;
  EMCA_MC_SIGNATURE_ERR_RECORD      McErrSig;
  EFI_PLATFORM_MEMORY_ERROR_DATA    MemoryErrorData;
  UINT32                            Sev;
  UINT64                            PrevState;
  UINT64                            State;
  UINT16                            SecType;
  UINT8                             ErrorType;
  UINT32                            UnitType;
  UINT32                            UpiInstance;
  UINT32                            UpiFailedLane;
  MSR_P6_MC_STATUS_REGISTER         McSts;


  SecType = GetMcBankSecType (McBank);
  UnitType = GetMcBankUnitType (McBank);


  if (SecType == EMCA_SECTYPE_INVALID) {
    RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "[Mca]SecType == EMCA_SECTYPE_INVALID \n"));
    RAS_ASSERT (FALSE);
    return;
  }

  RAS_DEBUG((MP_PRINT | LEVEL_FUNC_FLOW, "[Mca]McBankErrorHandler: Inside the function \n"));

  //Init structures
  PrevState = SmmMceClearedState;
  State = SmmMceClearedState;
  McErrSig.BankIndex = McBank;
  McErrSig.Signature.McSts = 0;
  McErrSig.Signature.McAddr = 0;
  McErrSig.Signature.McMisc = 0;
  McErrSig.ApicId = 0;
  Sev = EFI_ACPI_6_2_ERROR_SEVERITY_NONE;

  PrevState = InterlockedCompareExchange64 ((UINT64*) &CpuData->SmmMceSyncState, SmmMceClearedState, SmmMceReadingState);
  State = (PrevState == SmmMceClearedState) ? SmmMceReadingState : PrevState; //Reading State
  do {
    RAS_DEBUG ((MP_PRINT | LEVEL_BASIC_FLOW, "[Mca]McBankErrorHandler: Skt = 0x%x, McBank = 0x%x, State = 0x%x\n", Socket, McBank, State));
    switch(State) {
      case SmmMceReadingState:
        ReadMcBankSigHook (McBank, Socket, &McErrSig);
        PrevState = InterlockedCompareExchange64 ((UINT64*) &CpuData->SmmMceSyncState, SmmMceReadingState, SmmMceProcessingState);
        State = (PrevState == SmmMceReadingState) ? SmmMceProcessingState : PrevState;
        break;
      case SmmMceProcessingState:
        RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "[Mca]McBankErrorHandler: IoMcaErrorHandler Skt = 0x%x, McBank = 0x%x, State = 0x%x\n", Socket, McBank, State));
        if( McBank == ((UINT8) HighBitSet64 (MCA_UNIT_TYPE_IIO))) { //IioMca bank
          if(mProcessorRasSetup.IoMcaEn) {
            McSts.Uint64 = McErrSig.Signature.McSts;
            //
            //As IOSF and UBOX share the same MCA bank, so use MsCOD and McaCOD to identify IOSF(IOMCA) error source.
            // If an IEH error overlaps perfectly with a masked ubox error, the mscod for the ubox error is getting logged with the mcacod for the IEH error. 
            // So remove mscode check for IOMCA to avoid missing IEH error.
            //
            if ((McSts.Bits.MC_STATUS_MCACOD == MCACOD_IOMCA) && (!SkikpMc6IehErrorLogWa1506843150 ())) {

              IOMCAErrorhandler();
            }
          }
        }

        if( McBank == ((UINT8) HighBitSet64 (MCA_UNIT_TYPE_MLC))) { //MlcMca bank
          if(mProcessorRasSetup.OscEn) {
            McSts.Uint64 = McErrSig.Signature.McSts;
            //
            //MLC MCA uses MciSts bit53 and bit54 to identify MLC persistent error.
            //
            if ((McSts.Uint64 & MLC_YELLOW_MARK_MASK) == MLC_YELLOW_MARK_IND) {
              RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "[Mca]McBankErrorHandler: MlcOscErrorHandler Skt = 0x%x, McBank = 0x%x, State = 0x%x\n", Socket, McBank, State));
              OscErrorhandler();
            }
          }
        }

        MceClass = GetMceClass (&McErrSig);
        if (MceClass == INVALID) {
          //Removed MciStsMsr.Bits.uc != 1  || MciStsMsr.Bits.en != 1
          // This is to process correctable errors also. Further review might be needed.
          PrevState = InterlockedCompareExchange64 ((UINT64*) &CpuData->SmmMceSyncState, SmmMceProcessingState, SmmMceClearedState);
          State = (PrevState == SmmMceProcessingState) ? SmmMceClearedState : PrevState;
          break;
        } //else
        Sev = MapErrorSeverity (MceClass);

        if(Sev == EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTED) {
          CpuData->CmciMcBankErrCount[McBank]++;

          // Check if the error on bank is occuring more than 20 times, then it is hard error
          // disable CMCI morphing on the particular bank and come out the loop
          if ((mProcessorRasSetup.CsmiDynamicDis) && (CpuData->CmciMcBankErrCount[McBank] >= 20)) {
            DisableCsmiAtMcBank (McBank);
          }
        }
        //Recovery not posible, log the error.
        PrevState = InterlockedCompareExchange64 ((UINT64*) &CpuData->SmmMceSyncState, SmmMceProcessingState, SmmMceLoggingState);
        State = (PrevState == SmmMceProcessingState) ? SmmMceLoggingState : PrevState;
        break;
      case SmmMceWritingState:
        WriteMcBank (McBank, &McErrSig);
        PrevState = InterlockedCompareExchange64 ((UINT64*) &CpuData->SmmMceSyncState, SmmMceWritingState, SmmMceLoggingState);
        State = (PrevState == SmmMceWritingState) ? SmmMceLoggingState : PrevState;
        break;
      case SmmMceLoggingState:
        switch (SecType) {
          case EMCA_SECTYPE_PLATMEM:
            RAS_DEBUG((MP_PRINT | LEVEL_FUNC_FLOW, "[Mca]McBankErrorHandler: Section Type = EMCA_SECTYPE_PLATMEM\n"));
            Status = FillPlatformMemoryErrorData (Socket, &McErrSig, FALSE, &MemoryErrorData);
            if (!EFI_ERROR (Status)) {
              RAS_DEBUG((MP_PRINT | LEVEL_FUNC_FLOW, "[Mca]McBankErrorHandler: Report platform memory error log\n"));
              Status = ReportPlatMemErrorLog (McBank, Socket, Sev, &MemoryErrorData, &McErrSig);
            }
            break;
          case EMCA_SECTYPE_NONE:
            ErrorType = GetMcaBankErrorType (McBank);
            RAS_DEBUG((MP_PRINT | LEVEL_FUNC_FLOW, "[Mca]McBankErrorHandler: Section Type = EMCA_SECTYPE_NONE, Error Type = 0x%x\n", ErrorType));
            if (mProcessorRasSetup.PoisonEn) { // For Poison enable case. Check for possible memory source error first.
              Status = FillPlatformMemoryErrorData (Socket, &McErrSig, FALSE, &MemoryErrorData);
              if (!EFI_ERROR (Status)) {
                RAS_DEBUG((MP_PRINT | LEVEL_FUNC_FLOW, "[Mca]McBankErrorHandler: Report platform memory error log\n"));
                Status = ReportPlatMemErrorLog (McBank, Socket, Sev, &MemoryErrorData, &McErrSig);
              } else {
                RAS_DEBUG((MP_PRINT | LEVEL_FUNC_FLOW, "[Mca]McBankErrorHandler: Report processor memory error log\n"));
                Status = ReportProcessorErrorLog (McBank, Socket, ErrorType, Sev, &McErrSig);
              }
            } else {
              RAS_DEBUG((MP_PRINT | LEVEL_FUNC_FLOW, "[Mca]McBankErrorHandler: Report processor memory error log\n"));
              Status = ReportProcessorErrorLog (McBank, Socket, ErrorType, Sev, &McErrSig);
            }

            if (UnitType == MCA_UNIT_TYPE_KTI) {
              if (McSts.Bits.MC_STATUS_MCACOD == MSCOD_UPI_FAILOVER) {
                UpiInstance = GetMcBankInstance (McBank);
                if (BitFieldRead64 (McErrSig.Signature.McMisc, 37, 39) == LOW_LANE_ACTIVE) {
                  UpiFailedLane = 1;
                } else {
                  UpiFailedLane = 0;
                }
                ReportUpiFailoverErrorLog (Socket, UpiInstance, UpiFailedLane);
              } else {
                UpiInstance = GetMcBankInstance (McBank);
                ReportUpiErrorLog (Socket, UpiInstance, McSts.Bits.MC_STATUS_MCACOD);
              }
            }
            break;
          default:
            return;
        }

        CpuData->EventLog.ErrorsFound = TRUE;
        //Update severity
        switch(Sev) {
        default:
          break;
        case EFI_ACPI_6_2_ERROR_SEVERITY_NONE:
          break;
        case EFI_ACPI_6_2_ERROR_SEVERITY_FATAL:
          CpuData->EventLog.SeverityMap |= LShiftU64 (1, SEVERITY_MAP_FATAL);
          if(SEVERITY_MAP_FATAL < MAX_ERROR_SEVERITY) {
            CpuData->EventLog.Log[SEVERITY_MAP_FATAL].ApicId = McErrSig.ApicId;
            CpuData->EventLog.Log[SEVERITY_MAP_FATAL].BankIdx = McBank;
          }
          break;
        case EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTABLE:
          CpuData->EventLog.SeverityMap |=  LShiftU64 (1, SEVERITY_MAP_RECOVERABLE);
          if(SEVERITY_MAP_RECOVERABLE < MAX_ERROR_SEVERITY) {
            CpuData->EventLog.Log[SEVERITY_MAP_RECOVERABLE].ApicId = McErrSig.ApicId;
            CpuData->EventLog.Log[SEVERITY_MAP_RECOVERABLE].BankIdx = McBank;
          }
          break;
        case EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTED:
            CpuData->EventLog.SeverityMap |= LShiftU64 (1, SEVERITY_MAP_CORRECTED);
            if(SEVERITY_MAP_CORRECTED < MAX_ERROR_SEVERITY) {
            CpuData->EventLog.Log[SEVERITY_MAP_CORRECTED].ApicId = McErrSig.ApicId;
            CpuData->EventLog.Log[SEVERITY_MAP_CORRECTED].BankIdx = McBank;
            }
            break;
        }

        if (mSeverity != McaGetHighestSeverity (Sev, mSeverity)) {
          mSeverity = Sev;
        }

        PrevState = InterlockedCompareExchange64 ((UINT64*) &CpuData->SmmMceSyncState, SmmMceLoggingState, SmmMceClearedState);
        State = (PrevState == SmmMceLoggingState) ? SmmMceClearedState : PrevState;
        break;
      case SmmMceReadingInterrupted:
      case SmmMceProcessingInterrupted:
        //Recovery is not possible
        PrevState = InterlockedCompareExchange64 ((UINT64*) &CpuData->SmmMceSyncState, State, SmmMceClearedState);
        State = (PrevState == State) ? SmmMceClearedState : PrevState;
        break;
      case SmmMceClearedState:
        break;
      case SmmMceWritingInterrupted:
        RAS_DEBUG((MP_PRINT | LEVEL_FUNC_FLOW, "[Mca]McBankErrorHandler: SmmMce Writing Interrupted\n"));
        RAS_ASSERT (FALSE); //MCE handler never returns when Writing is interrupted.
        break;
      default:
        RAS_ASSERT (FALSE);
        break;
    }
  } while (State != SmmMceClearedState);

  return;
}


/**
  Get the machine check banks bit maks for core/uncore scope.

  Returns a bitmap indicating core/uncore machine check banks.

  @retval McBankBitField

**/
UINT64
EmcaReadMcBankBitField (
  IN BOOLEAN   CoreScope
  )
{
  UINT64 McBankBitField;
  UINT32 MaxBank;

  MaxBank = GetNumMcBanksPerLp();
  McBankBitField = CoreScope ? ReadCoreSmiSrc () : ReadUncoreSmiSrc ();
  McBankBitField &= LShiftU64 (1, MaxBank) - 1;

  RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "[Mca]EmcaReadMcBankBitField: Scope %a, BitField 0x%lx\n",
    CoreScope ? "Core" : "Uncore", McBankBitField));

  return McBankBitField;
}

/**
  Clear the machine check banks bit maks for core/uncore scope.

  Returns a bitmap indicating core/uncore machine check banks.

  @retval McBankBitField

**/
VOID
EmcaClearMcBankBitField (
  IN BOOLEAN  CoreScope,
  IN UINT64   McBankBitField
  )
{
  RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "[Mca]EmcaClearMcBankBitField: Scope %a, BitField 0x%lx\n",
    CoreScope ? "Core" : "Uncore", McBankBitField));

  CoreScope? ClearCoreSmiSrc (McBankBitField) :  ClearUncoreSmiSrc (McBankBitField);
}

/**
  Get the machine check banks bit maks for core/uncore scope.

  Returns a bitmap indicating core/uncore machine check banks.

  @retval McBankBitField

**/
UINT64
ReadMcBankBitField (
  IN UINT16   Scope
  )
{
  UINT64 McBankBitField;
  UINT32 MaxBank;
  UINT32 McBank;
  UINT16 McScope;

  McBankBitField = 0;
  MaxBank = GetNumMcBanksPerLp();
  for (McBank = 0; McBank < MaxBank; McBank++) {
    McScope = GetMcBankScope (McBank);
    if (McScope == MC_SCOPE_ERROR) {
      RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "[Mca]McScope == MC_SCOPE_ERROR\n"));
      RAS_ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
    }
    if (McScope == Scope) {
      McBankBitField |= LShiftU64 (1, McBank);
    }
  }
  RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "[Mca]ReadMcBankBitField: Scope 0x%x, BitField 0x%lx\n", Scope, McBankBitField));

  return McBankBitField;
}

/**
  Process Core/Uncore MC Bank errors.

  @param[in] buffer     - Boolean value indicate whether it is core scope or uncore scope

**/
VOID
ProcessMcBankError (
  IN      BOOLEAN     *Buffer
  )
{
  UINT8                       McBank;
  UINT64                      McBankBitField;
  UINTN                       CpuNumber;
  MP_CPU_INFO                 CpuInfo;
  BOOLEAN                     CoreScope;
  volatile UINT64             *ProcessingBank;

  CoreScope = *Buffer;

  CpuNumber = GetProcessorInfo (&CpuInfo);
  RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "[Mca]ProcessMcBankError: Scope %a\n", CoreScope? "Core" : "Uncore"));

  gSmmErrorCpuData[CpuNumber].SmmMceSyncState = SmmMceClearedState;
  ProcessingBank = CoreScope? &gSmmErrorCpuData[CpuNumber].CoreProcessingBank : &gSmmErrorCpuData[CpuNumber].UncoreProcessingBank;

  if (mMode == MODE_EMCA) {
    McBankBitField = EmcaReadMcBankBitField (CoreScope);
  } else {
    McBankBitField = ReadMcBankBitField (CoreScope ? MC_SCOPE_CORE : MC_SCOPE_PKG);
  }

  while (McBankBitField != 0) {
    RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "[Mca]McBankBitField: 0x%lx \n", McBankBitField));
    McBank = (UINT8) HighBitSet64 (McBankBitField);
    *ProcessingBank = LShiftU64 (1, McBank);
    McBankBitField &= ~(*ProcessingBank);
    if (mMode == MODE_EMCA) {
      EmcaClearMcBankBitField (CoreScope, *ProcessingBank);
    }
    McBankErrorHandler (McBank, (UINT8) CpuInfo.Package, &gSmmErrorCpuData[CpuNumber]);
    gSmmErrorCpuData[CpuNumber].CoreProcessingBank = 0;

    if(mMode == MODE_EMCA) {
      McBankBitField = EmcaReadMcBankBitField (CoreScope);
    }
  }
}


/**
  Process Socket MC Bank errors.

**/
VOID
ProcessSocketMcBankError (
  VOID
  )
{
  BOOLEAN                CoreScope;

  RAS_DEBUG((LEVEL_BASIC_FLOW, "[Mca]ProcessSocketMcBankError: Inside the function\n"));

  CoreScope = FALSE;
  MultiProcessorExecute (OneApPerPackage, ALL_PACKAGE, ALL_CORE, TRUE, ProcessMcBankError, &CoreScope);

  CoreScope = TRUE;
  MultiProcessorExecute (AllAps, ALL_PACKAGE, ALL_CORE, TRUE, ProcessMcBankError, &CoreScope);
}



/**
  This function enables the SMI generation of uncorrected errors.

  @retval EFI_SUCCESS if the call is succeed.

**/
VOID
EnableUncorrectableErrRep (
  VOID
  )
{
  //
  // Enable SMI for uncorrected errors, if logging needed
  //
  if (mProcessorRasSetup.EmcaEn) {
    if (mProcessorRasCap.EmcaGen2Cap && (mProcessorRasSetup.MsmiEn == MODE_EMCA)) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "[Mca]EnableEmca2UncorrectableError\n"));
      EnableEmca2UncorrectableError (mProcessorRasSetup.MsmiBankBitFieldEn);
    }
  }
}

/**
  This function enables LMCE


**/
VOID
InitLmce (
  VOID
)
{
  if(mProcessorRasCap.LmceCap && mProcessorRasSetup.LmceEn) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca]Enable LMCE\n"));
    EnableLmce ();
  }
}

/**
  Enables CSMI.

**/
VOID
EnableCsmi (
  VOID
  )
{
  if (mProcessorRasCap.EmcaGen2Cap && (mProcessorRasSetup.CsmiEn == MODE_EMCA)) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca]Enable CSMI Gen2\n"));
    EnableCsmiGen2 (mProcessorRasSetup.CsmiBankBitFieldEn, mProcessorRasSetup.CsmiThreshold, mProcessorRasSetup.CloakingEn);
  }
}

//
// Interfaces for reporter protocol
//

BOOLEAN
EFIAPI
McaIsSupported (
  VOID
  )
{
  BOOLEAN Supported;

  //
  // Assume not supported, and then override by capabilities and enables.
  //
  Supported = FALSE;

  //
  // If EMCA is capable and enabled, MCA is supported.
  //
  if (mProcessorRasCap.EmcaGen1Cap || mProcessorRasCap.EmcaGen2Cap) {
    if (mProcessorRasSetup.SystemErrorEn && mProcessorRasSetup.EmcaEn) {
      Supported = TRUE;
    }
  }

  return Supported;
}

VOID
EFIAPI
McaEnableReporting (
  VOID
  )
{
  //SiWAForInit();
  if(InitSmmRuntimeCtlHooks() == EFI_SUCCESS) {
    SetEmcaHandlerState(SmmMceNotStartedState);
  }


 // Init Uncorrected processor errors / Enable SMI routing
  EnableUncorrectableErrRep();

   // Program corrected error signals
  EnableCsmi();

  //
  // When poison enabled and BWV present; set DCU_MODE MSR(0x31) for SKX/CLX or CPX only
  //
  if (IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    RAS_DEBUG((LEVEL_FUNC_FLOW, "McaEnableReporting: PoisonEn:%d; BwvPresent:%d \n",
                 mProcessorRasSetup.PoisonEn, mMemTopology->BwvPresent));
    if ((mProcessorRasSetup.PoisonEn) && (mMemTopology->BwvPresent)) {
      SetDCUPoisonDetectionMode();
    }
  }

}

/**
  Checks for pending processor errors.

  @retval TRUE  - Pending error exists
          FALSE - No pending error exists

**/
BOOLEAN
EFIAPI
McaCheckStatus (
  VOID
  )
{
  if((mMode == MODE_EMCA) && CheckEmcaSmiError ()) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca]CheckEmcaSmiError returns TRUE\n"));
    return TRUE;
  }

  return FALSE;
}


/**
  Error reporting for  MCERRs and IERRs.

  @retval NONE

**/
VOID
EFIAPI
ReportUboxOrderError (
  VOID
  )
{
  UINT8   Socket;

  UINT32  FirstMcerrValid;
  UINT32  FirstMcerrPort;
  UINT32  SecondMcerrValid;
  UINT32  SecondMcerrPort;

  UINT32  FirstIerrValid;
  UINT32  FirstIerrPort;
  UINT32  SecondIerrValid;
  UINT32  SecondIerrPort;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (IsSocketPresent (Socket) == FALSE) {
      continue;
    }

    GetMcerrLoggingData (Socket, &FirstMcerrValid, &FirstMcerrPort, &SecondMcerrValid, &SecondMcerrPort);

    GetIerrLoggingData (Socket, &FirstIerrValid, &FirstIerrPort, &SecondIerrValid, &SecondIerrPort);

    if (FirstMcerrValid == 0) {
      FirstMcerrPort   = INVALID_U32;
    }

    if (SecondMcerrValid == 0) {
      SecondMcerrPort   = INVALID_U32;
    }

    if (FirstIerrValid == 0) {
      FirstIerrPort   = INVALID_U32;
    }

    if (SecondIerrValid == 0) {
      SecondIerrPort   = INVALID_U32;
    }

    if (FirstMcerrValid == 1 || FirstIerrValid == 1) {
      ReportUboxOrderErrorLog (Socket, FirstMcerrPort, SecondMcerrPort, FirstIerrPort, SecondIerrPort);
    }
  }

}


VOID
EFIAPI
McaDetectAndHandle (
  VOID
  )
{
  UINT64                            PrevState;
  UINT64                            State;

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca]McaDetectAndHandle start\n"));

  State = SmmMceNotStartedState;
  mSeverity = EFI_ACPI_6_2_ERROR_SEVERITY_NONE;

  do {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca]McaDetectAndHandle, state is 0x%x\n", State));
    switch(State) {
      case SmmMceNotStartedState:
        PrevState = TransitionEmcaHandlerState(SmmMceNotStartedState,SmmMceInProgressState);
        State = (PrevState == SmmMceNotStartedState) ? SmmMceInProgressState : PrevState;
        break;
      case SmmMceInProgressState:
        ProcessSocketMcBankError();
        PrevState = TransitionEmcaHandlerState(SmmMceInProgressState,SmmMceFinishedState);
        State = (PrevState == SmmMceInProgressState) ? SmmMceFinishedState : PrevState;
        break;
      case SmmMceInterruptedState:
        PrevState = TransitionEmcaHandlerState(SmmMceInterruptedState,SmmMceInProgressState);
        State = (PrevState == SmmMceInterruptedState) ? SmmMceInProgressState : PrevState;
        break;
      default:
        RAS_ASSERT (FALSE);
        break;
    }
  } while (State != SmmMceFinishedState);

  mBroadcast = CheckLmceStatus();

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca]McaDetectAndHandle, severity = 0x%x, Broadcast is 0x%x\n", mSeverity, mBroadcast));
}

VOID
EFIAPI
McaClearStatus (
  VOID
  )
{

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca] Clear Status\n"));
  if (mMode == MODE_EMCA) {
    if (mSeverity != EFI_ACPI_6_2_ERROR_SEVERITY_FATAL) {
      if (!GetPendSmiFlag ()) {
        RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca]ClearErrLogReg\n"));
        if (mProcessorRasSetup.MsmiEn != MODE_UMC) {
          ClearErrLogReg ();
        }
      }
    }
  } else if (mMode == MODE_UMC) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca]ClearUmcStatus\n"));
    ClearUmcStatus ();
  }
}

/**
  The callback function to handle subsequent SMIs.  This callback will be called by SmmCoreDispatcher.

  @param[in] DispatchHandle             Not used
  @param[in] DispatchContext            Not used
  @param[in, out] CommBuffer            Not used
  @param[in, out] CommBufferSize        Not used

  @retval EFI_SUCCESS                   Function successfully completed
**/
EFI_STATUS
EFIAPI
McaErrorHandler (
  IN EFI_HANDLE            DispatchHandle,
  IN CONST VOID            *DispatchContext,
  IN OUT VOID              *CommBuffer,
  IN OUT UINTN             *CommBufferSize
  )
{
  PERF_START (NULL, "McaErrorHandler", "RAS", 0);
  if (McaCheckStatus ()) {
    if (mProcessorRasSetup.McaSpinLoop) {
      RAS_DEBUG ((MP_PRINT | LEVEL_BASIC_FLOW, "MCA Spin Loop\n"));
      CpuDeadLoop ();
    }
    Wa_1706854359 ();
    McaDetectAndHandle ();
    ReportApPlatMemErrorLog ();
    ReportUboxOrderError ();
    McaClearStatus ();
    McaNotifyOs ();
    PERF_END (NULL, "McaErrorHandler", "RAS", 0);
  }

  return EFI_SUCCESS;
}


VOID
GetRasSetting (
  VOID
  )
{
  RAS_POLICY            *RasPolicy;

  RasPolicy = GetRasPolicy ();

  if (RasPolicy == NULL){
    RAS_DEBUG ((MP_PRINT | LEVEL_FUNC_FLOW, "[Mca] RasPolicy == NULL \n"));
    RAS_ASSERT (FALSE);
    return;
  }

  mProcessorRasSetup.EmcaEn             = RasPolicy->McaPolicy.EmcaEn;
  mProcessorRasSetup.CsmiEn             = RasPolicy->McaPolicy.EmcaCsmiEn;
  mProcessorRasSetup.CsmiThreshold      = RasPolicy->McaPolicy.EmcaCsmiThreshold;
  mProcessorRasSetup.CsmiDynamicDis     = RasPolicy->McaPolicy.CsmiDynamicDisable;
  mProcessorRasSetup.MsmiEn             = RasPolicy->McaPolicy.EmcaMsmiEn;
  mProcessorRasSetup.LmceEn             = RasPolicy->McaPolicy.LmceEn;
  mProcessorRasSetup.IoMcaEn            = (RasPolicy->IioPolicy.IoMcaEn) & (RasPolicy->IioPolicy.IioErrorEn);
  mProcessorRasSetup.CloakingEn         = RasPolicy->McaPolicy.CloakingEn;
  mProcessorRasSetup.PoisonEn           = RasPolicy->PoisonEn;
  mProcessorRasSetup.MsmiBankBitFieldEn = RasPolicy->McaPolicy.MSMIBankBitFieldEn;
  mProcessorRasSetup.CsmiBankBitFieldEn = RasPolicy->McaPolicy.CSMIBankBitFieldEn;
  mProcessorRasSetup.SystemErrorEn      = RasPolicy->SystemErrorEn;
  mProcessorRasSetup.FatalErrSpinLoopEn = RasPolicy->FatalErrSpinLoopEn;
  mProcessorRasSetup.OscEn              = RasPolicy->McaPolicy.OscEn;
  mProcessorRasSetup.McaSpinLoop        = (BOOLEAN) (RasPolicy->McaPolicy.McaSpinLoop == 1);
  mProcessorRasSetup.UboxErrorMask      = RasPolicy->McaPolicy.UboxErrorMask;

  if (mProcessorRasSetup.OscEn) {
    mProcessorRasSetup.CsmiBankBitFieldEn |= MCA_UNIT_TYPE_MLC;
  }
}

VOID
DumpRasSettings (
  VOID
  )
{
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca][Cap] EmcaGen1Cap=0x%x\n", mProcessorRasCap.EmcaGen1Cap));
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca][Cap] EmcaGen2Cap=0x%x\n", mProcessorRasCap.EmcaGen2Cap));
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca][Cap] LmceCap=0x%x\n", mProcessorRasCap.LmceCap));

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca][Setup] SystemErrorEn=0x%x\n", mProcessorRasSetup.SystemErrorEn));
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca][Setup] PoisonEn=0x%x\n", mProcessorRasSetup.PoisonEn));
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca][Setup] CloakingEn=0x%x\n", mProcessorRasSetup.CloakingEn));
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca][Setup] FatalErrSpinLoopEn=0x%x\n", mProcessorRasSetup.FatalErrSpinLoopEn));
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca][Setup] EmcaEn=0x%x\n", mProcessorRasSetup.EmcaEn));
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca][Setup] CsmiEn=0x%x\n", mProcessorRasSetup.CsmiEn));
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca][Setup] MsmiEn=0x%x\n", mProcessorRasSetup.MsmiEn));
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca][Setup] LmceEn=0x%x\n", mProcessorRasSetup.LmceEn));
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca][Setup] MsmiBankBitFieldEn=0x%x\n", mProcessorRasSetup.MsmiBankBitFieldEn));
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca][Setup] CsmiBankBitFieldEn=0x%x\n", mProcessorRasSetup.CsmiBankBitFieldEn));
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca][Setup] OscEn=0x%x\n", mProcessorRasSetup.OscEn));
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca][Setup] mMode=0x%x\n", mMode));
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca][Setup] mProcessorRasSetup.UboxErrorMask=%x\n", mProcessorRasSetup.UboxErrorMask));
}


/**
  Entry point for the Processor Handler initialization.

  This function initializes the error handling and enables error
  sources for the CPU Core.


  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval Status.

**/
EFI_STATUS
EFIAPI
InitializeProcessorErrHandler (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
  EFI_STATUS                Status;

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca]Entering InitializeProcessorErrorHandler (0x%x) \n", InitializeProcessorErrHandler));

  gSmmErrorCpuData = AllocateZeroPool (sizeof (*gSmmErrorCpuData) * PcdGet32 (PcdCpuMaxLogicalProcessorNumber));
  if (gSmmErrorCpuData == NULL) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca]gSmmErrorCpuData == NULL \n"));
    RAS_ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca]Allocate Cpu Error Data structure at %p\n", gSmmErrorCpuData));

  mProcessorRasCap.EmcaGen1Cap = IsEmcaLogCap ();
  mProcessorRasCap.EmcaGen2Cap = IsEmcaGen2Cap ();
  mProcessorRasCap.LmceCap = IsLmceProcessorSupported ();

  GetRasSetting ();

  if (mProcessorRasCap.EmcaGen2Cap && (mProcessorRasSetup.MsmiEn == MODE_EMCA || mProcessorRasSetup.CsmiEn == MODE_EMCA)) {
    mMode = MODE_EMCA;
  } else {
    mMode = MODE_UMC;
  }

  DumpRasSettings ();

  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmCpuServiceProtocolGuid,
                    NULL,
                    &mSmmCpuService
                    );
  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "%a: Locate gEfiSmmCpuServiceProtocolGuid failed, Status=%r\n", __FUNCTION__, Status));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // Perform initialization here
  //
  if(McaIsSupported()) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca]Enable MCA reporting \n"));
    //
    // Clear sticky status register and shadow MSR status register
    // before enabling RAS features to avoid last boot error impact current boot flow.
    //
    ClearStickyErrLogReg ();
    ClearShadowMsrReg ();
    ConfigUboxErrorMask (mProcessorRasSetup.UboxErrorMask);
    McaEnableReporting ();
// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#if AMI_RAS_ENHANCEMENTS_AND_HOOKS_SUPPORT
    OemMcaErrEnableDisableReporting ();
#endif
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support

    mHandle = NULL;
    Status = gSmst->SmiHandlerRegister (McaErrorHandler, NULL, &mHandle);
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca]Register MCA error handler %r\n", Status));
    if (EFI_ERROR (Status)) {
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "%a: [Mca] McaErrorHandler registration failed, Status=%r\n", __FUNCTION__, Status));
      ASSERT_EFI_ERROR (Status);
      return Status;
    }

    if (mProcessorRasCap.EmcaGen2Cap && mProcessorRasSetup.EmcaEn) {
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca]Installing MCE Handler...\n"));
      Status = mSmmCpuService->RegisterExceptionHandler (mSmmCpuService, EXCEPT_IA32_MACHINE_CHECK, SmiMcaHandler);
      if (EFI_ERROR (Status)) {
        RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca]RegisterExceptionHandler Failed!, Status=%r\n", Status));
        ASSERT_EFI_ERROR (Status);
      } else {
        RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca]SmiMcaHandler Address = %p\n", SmiMcaHandler));
      }
    }
  } else {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca]MCA Is not supported!\n"));
  }

  //
  //Enable LMCE
  //
  InitLmce();

  if(mProcessorRasSetup.CloakingEn) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca]System Cloaking is enabled.\n"));
    EnableCloaking ();
  } else {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Mca]System Cloaking is disabled.\n"));
  }

  EnableClearPendingSmiOnSipi ();

  return Status;
}
