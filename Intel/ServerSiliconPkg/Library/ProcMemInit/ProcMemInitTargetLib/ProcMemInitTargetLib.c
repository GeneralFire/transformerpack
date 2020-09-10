/** @file
  This file implements the ProcMemInitTargetLib library class.

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

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/MemRcLib.h>


/**

  Disables MRC steps not needed for HVM mode


  @retval           EFI_SUCCESS     Completed successfully.
  @retval !EFI_SUCCESS    Failure.

**/
EFI_STATUS
EFIAPI
DisableTrainingStepsTarget (
  VOID
  )
{
  PSYSHOST              Host = NULL;
  UINT32                Socket = 0;
#ifdef CWV_A0_REMOVE_POST_PO
  UINT8                 Index;
  const UINT8 CwvA0PoSkipTrainingSteps[] = {
    CaSlewRateTraining,                   // CaSlewRate -                   CaSlewRateTraining                   - bit71
    DcaTcoTraining,                       // TrainDcaTco -                  DcaTcoTraining                       - bit72
    DcaDckDutyCycleTraining,              // RcdDcaDckDutyCycle -           DcaDckDutyCycleTraining              - bit66
    RcdDcaDfeDdr5,                        // DcaDfeDdr5 -                   RcdDcaDfeDdr5                        - bit67
    ReadDqDqsPreDfe2DCenteringTraining,   // ReadDqDqsPreDfe2DCentering -   ReadDqDqsPreDfe2DCenteringTraining   - bit68
    DramDutyCycleAdjusterTraining,        // DramDutyCycleAdjuster -        DramDutyCycleAdjusterTraining        - bit79
    RxDfeDdr5Training,                    // RxDfeDdr5 -                    RxDfeDdr5Training                    - bit75
    ReadDqDqsPostDfe2DCenteringTraining,  // ReadDqDqsPostDfe2DCentering -  ReadDqDqsPostDfe2DCenteringTraining  - bit69
    WriteDqDqsPreDfe2DCenteringTraining,  // WriteDqDqsPreDfe2DCentering -  WriteDqDqsPreDfe2DCenteringTraining  - bit64
    DqSlewRateTraining,                   // TxDqSlewRate -                 DqSlewRateTraining                   - bit77
    TcoDqDqsTraining,                     // TrainTcoDqDqs -                TcoDqDqsTraining                     - bit80
    TxDfeDdr5Training,                    // TxDfeDdr5 -                    TxDfeDdr5Training                    - bit76
    DbDfeDdr5Training,                    // TxDbDfeDdr5 -                  DbDfeDdr5Training                    - bit82
    TxOdtLatencyTraining,                 // WriteOdtLatencyTraining -      TxOdtLatencyTraining                 - bit83
    WriteDqDqsPostDfe2DCenteringTraining, // WriteDqDqsPostDfe2DCentering - WriteDqDqsPostDfe2DCenteringTraining - bit65
    CmdNormalization,                     // NormalizeCmd -                 CmdNormalization                     - bit34
    PeriodicTxReTraining,                 // PeriodicTxRetrain -            PeriodicTxReTraining                 - bit70
    RoundTripOptimizeTraining,            // RoundTripOptimize -            RoundTripOptimizeTraining            - bit13
    TurnaroundTraining,                   // TurnaroundTrain -              TurnaroundTraining                   - bit74
  };
  UINT8 CwvA0PoSkipTrainingStepsSize = sizeof (CwvA0PoSkipTrainingSteps) / sizeof (UINT8);
#endif

  if (IsBrsPresent (Host, Socket)) {
    //
    // DDRIO design recommendation
    //
    DisableMemFlow (PhaseCrosstalkTraining);
    DisableMemFlow (TxDdjcTraining);
    DisableMemFlow (RxDfeTraining);
    DisableMemFlow (McOdtTraining);
    //
    // KIT recommendation
    //
    DisableMemFlow (DramROnTraining);
    //
    // Training
    //
    DisableMemFlow (LateCmdClkTraining);
    DisableMemFlow (TxEqTraining);
    DisableMemFlow (ImodeTraining);
    DisableMemFlow (TxRiseFallSlewRateTraining);
    DisableMemFlow (TcoCompTraining);
    DisableMemFlow (RoundTripOptimizeTraining);
    DisableMemFlow (CmdVrefCenteringTraining);
    DisableMemFlow (CmdNormalization);
    DisableMemFlow (RxCtleTraining);
    DisableMemFlow (McROnTraining);

    EnableMemFlow (LegacyRankMarginTool);
  }

#ifdef CWV_A0_REMOVE_POST_PO
  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    for (Index = 0; Index < CwvA0PoSkipTrainingStepsSize; Index++) {
      DisableMemFlow (CwvA0PoSkipTrainingSteps[Index]);
    }
  }
#endif

  return EFI_SUCCESS;
} // DisableTrainingStepsTarget
