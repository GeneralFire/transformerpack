/** @file
  fADR SMM driver source file.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include "Fadr.h"

EFI_SMM_SYSTEM_TABLE2               *mSmst = NULL;
EFI_SMM_CPU_SERVICE_PROTOCOL        *mSmmCpuServiceProtocol = NULL;
EFI_SMM_SX_DISPATCH2_PROTOCOL       *mSxDispatch = 0;
EFI_SMM_IO_TRAP_DISPATCH2_PROTOCOL  *mIoTrapDispatch = NULL;
PCH_SMM_IO_TRAP_CONTROL_PROTOCOL    *mPchIoTrapCtrl = NULL;
EFI_CRYSTAL_RIDGE_PROTOCOL          *mCrystalRidgeProtocol = NULL;

SPIN_LOCK                           mFadrApTaskLock;
SPIN_LOCK                           mFadrDbgMsgLock;
FADR_DATA                           mFadrData;
FADR_TASK_DATA                      mFadrTaskData;
UINT32                              mFadrDriverInfoLevel;
UINT32                              mFadrFlowInfoLevel;
UINT32                              mFadrTimeInfoLevel;

CHAR16*                             mFadrFlowName[FADR_FLOWS_MAX] = { L"PCU assisted IIO and CPU Cache Flush",
                                                                      L"cold and warm reset CPU Cache Flush",
                                                                      L"S3 entry CPU Cache Flush",
                                                                      L"S4 entry CPU Cache Flush",
                                                                      L"S5 entry CPU Cache Flush" };

/**

  Configures fADR state on PMEM Module

  @param[in] State    - fADR state (enabled/disabled)

  @retval EFI_SUCCESS - Configuration completed successfully,
                        error code otherwise

**/
EFI_STATUS
FadrSetPmemModuleExtendedAdrState (
  IN EXT_ADR_STATE State
  )
{
  if (NULL == mCrystalRidgeProtocol) {
    return EFI_NOT_READY;
  }

  if ((EXT_ADR_ENABLED != State) && (EXT_ADR_DISABLED != State)) {
    return EFI_INVALID_PARAMETER;
  }

  FADR_DEBUG ((mFadrDriverInfoLevel, "[fADR] %s Extended ADR (fADR) on PMEM Modules\n", (State) ? L"Enable" : L"Disable"));

  return mCrystalRidgeProtocol->ExtAdrStateUpdate (EXT_ADR_FAST_ADR, State);
}

/**

  Updates Extended ADR (fADR) enabled flows CR UEFI variable

  @param[in] Flows    - fADR enabled flows map

  @retval EFI_SUCCESS - Configuration completed successfully,
                        error code otherwise

**/
EFI_STATUS
FadrSetExtendedAdrEnabledFlows (
  IN UINT8 Flows
  )
{
  if (NULL == mCrystalRidgeProtocol) {
    return EFI_NOT_READY;
  }

  FADR_DEBUG ((mFadrDriverInfoLevel, "[fADR] Setting Extended ADR (fADR) enabled flows map to 0x%x\n", Flows));
  return mCrystalRidgeProtocol->ExtAdrFlowsUpdate (Flows);
}

/**

  Set fADR stage in PMEM Module CSR to update the Extended ADR Last-Shutdown-Status

  @param[in] Stage - Extended ADR Last-Shutdown-Status stage

  @retval EFI_SUCCESS - Operation completed successfully,
                        error code otherwise
**/
EFI_STATUS
FadrSetPmemModuleExtendedAdrLssState (
  IN FADR_STAGE Stage
  )
{
  if (!FadrIsStageSupported (Stage)) {
    return EFI_UNSUPPORTED;
  }

  if (NULL == mCrystalRidgeProtocol) {
    return EFI_NOT_READY;
  }

  FADR_DEBUG ((mFadrFlowInfoLevel, "[fADR] Set the Extended ADR (fADR) stage (ADR%d)\n", Stage));
  return mCrystalRidgeProtocol->ExtAdrLssStateUpdate ((UINT32)Stage);
}

/**

  This function is used to check if all APs started tasks

  @param[out] NumOfRemaining - Number of APs that hasn't started task yet.

  @retval TRUE if all APs started tasks, FALSE otherwise

**/
BOOLEAN
FadrAllTasksStarted (
  UINTN *NumOfRemaining OPTIONAL
  )
{
  BOOLEAN  CpuCacheFlushAllTasksStarted = FALSE;

  while (!AcquireSpinLockOrFail (&mFadrApTaskLock)) {
    CpuPause ();
  }

  if ((mFadrTaskData.NumOfScheduled - mFadrTaskData.NumOfCompleted) == 0) {
    CpuCacheFlushAllTasksStarted = TRUE;
  }

  if (NULL != NumOfRemaining) {
    *NumOfRemaining = (mFadrTaskData.NumOfScheduled - mFadrTaskData.NumOfCompleted);
  }

  ReleaseSpinLock (&mFadrApTaskLock);

  return CpuCacheFlushAllTasksStarted;
}

/**

  Changes the performance state

  @param[out] PerformanceRatio - Pointer to the set new value of performance Ratio

  @retval None

**/
VOID
FadrChangeCpuPerformanceState (
  OUT UINT32 *PerformanceRatio
  )
{
  MSR_IA32_PERF_CTL_REGISTER          PerformanceControlMsr;
  SPR_MSR_PLATFORM_INFO_REGISTER      PlatformInfoMsr;
  MSR_IA32_HWP_CAPABILITIES_REGISTER  HwpCapabilitiesMsr;
  MSR_IA32_HWP_REQUEST_REGISTER       HwpHwReqMsr;
  MSR_IA32_PM_ENABLE_REGISTER         HwpEnableMsr;

  HwpEnableMsr.Uint64 = AsmReadMsr64 (MSR_IA32_PM_ENABLE);
  if (HwpEnableMsr.Bits.HWP_ENABLE) {
    HwpCapabilitiesMsr.Uint64 = AsmReadMsr64 (MSR_IA32_HWP_CAPABILITIES);
    HwpHwReqMsr.Uint64 = AsmReadMsr64 (MSR_IA32_HWP_REQUEST);

    HwpHwReqMsr.Bits.Minimum_Performance = HwpCapabilitiesMsr.Bits.Lowest_Performance;
    HwpHwReqMsr.Bits.Maximum_Performance = (HwpCapabilitiesMsr.Bits.Lowest_Performance + 1);

    AsmWriteMsr64 (MSR_IA32_HWP_REQUEST, HwpHwReqMsr.Uint64);

    if (NULL != PerformanceRatio) {
    *PerformanceRatio = HwpCapabilitiesMsr.Bits.Lowest_Performance;
    }
  } else {
    PerformanceControlMsr.Uint64 = AsmReadMsr64 (MSR_IA32_PERF_CTL);
    PlatformInfoMsr.Uint64 = AsmReadMsr64 (SPR_MSR_PLATFORM_INFO);

    PerformanceControlMsr.Bits.TargetState &= ~((UINT32)B_IA32_PERF_CTRLP_STATE_TARGET);
    PerformanceControlMsr.Bits.TargetState |= ((PlatformInfoMsr.Bits.MaxEfficiencyRatio) & ((UINT32)B_IA32_PERF_CTRLP_STATE_TARGET));

    AsmWriteMsr64 (MSR_IA32_PERF_CTL, PerformanceControlMsr.Uint64);

    if (NULL != PerformanceRatio) {
      *PerformanceRatio = PlatformInfoMsr.Bits.MaxEfficiencyRatio;
    }
  }

  while (!AcquireSpinLockOrFail (&mFadrApTaskLock)) {
    CpuPause ();
  }
  mFadrTaskData.NumOfCompleted++;
  ReleaseSpinLock (&mFadrApTaskLock);
}

/**

  Changes the performance state on AP

  @param[out] PerformanceRatio - Pointer to the set new value of performance Ratio

  @retval None

**/
VOID
FadrChangeCpuPerformanceStateAp (
  OUT UINT32 *PerformanceRatio
  )
{
  FadrChangeCpuPerformanceState (PerformanceRatio);
}

/**

  This routine decrease CPUs performance to max efficiency ratio

  @param None

  @retval EFI_SUCCESS - Operation completed successfully,
                        error code otherwise

**/
EFI_STATUS
FadrChangeCpusPerformanceState (
  VOID
  )
{
  EFI_STATUS  Status;
  UINTN       Cpu;
  UINT32      PerformanceRatio;
  UINTN       NumOfRemaining;

  //
  // Initialize eADR tasks spin lock
  //
  InitializeSpinLock(&mFadrApTaskLock);

  //
  // Initialize task counter
  //
  ZeroMem (&mFadrTaskData, sizeof (mFadrTaskData));

  //
  // Run through all CPUs
  //
  mFadrTaskData.NumOfScheduled = mSmst->NumberOfCpus;

  //
  // Set new performance state for all CPUs
  //
  for (Cpu = 0; Cpu < mSmst->NumberOfCpus; Cpu++) {
    if (Cpu == mSmst->CurrentlyExecutingCpu) {
      FadrChangeCpuPerformanceState (&PerformanceRatio);
    } else {
      Status = mSmst->SmmStartupThisAp (FadrChangeCpuPerformanceStateAp, Cpu, &PerformanceRatio);
      if (EFI_ERROR (Status)) {
        FADR_DEBUG ((DEBUG_ERROR, "[fADR] CPU (%d) failed to dispatch task (status %r)\n", Cpu, Status));
        return Status;
      }
    }
    FADR_DEBUG ((mFadrFlowInfoLevel, "[fADR] CPU (%d) change performance state (ratio: 0x%x)\n", Cpu, PerformanceRatio));
  }

  //
  // Wait for all CPUs to complete performance state change
  //
  while (!FadrAllTasksStarted (&NumOfRemaining)) {
    FADR_DEBUG ((mFadrFlowInfoLevel, "[fADR] Performance state change - waiting for CPUs to complete performance change state (remaining tasks %d)\n",
      NumOfRemaining));
    CpuPause ();
  }

  FADR_DEBUG ((mFadrFlowInfoLevel, "[fADR] Performance state change - all CPUs completed performance change state\n"));
  return EFI_SUCCESS;
}


/**

  Hang the executing CPU core/thread.
  (This is equivalent to a cli-hlt loop in assembly)

  @param None

  @retval None

**/
VOID
FadrCpuHang (
  VOID
  )
{
  volatile BOOLEAN LoopFlag;

  LoopFlag = TRUE;

  while (LoopFlag) {
    //
    // execute the CLI instruction
    //
    DisableInterrupts ();

    //
    // execute the HLT instruction
    //
    CpuSleep ();
  }
}

/**

  Hang the AP
  (This is equivalent to a cli-hlt loop in assembly)

  @param[in/out] Buffer - The pointer to private data buffer.

  @retval None

**/
VOID
FadrCpuHangAp (
  IN OUT VOID  *Buffer
  )
{
  FadrCpuHang ();
}

/**

  This function is used to flushes the processor caches serially by executing WBINVD instruction

  @param[in/out] Buffer - The pointer to private data buffer.

  @retval None

**/
VOID
FadrCpuCacheFlush (
  IN OUT VOID  *Buffer
  )
{
  AsmWbinvd ();

  while (!AcquireSpinLockOrFail (&mFadrApTaskLock)) {
    CpuPause ();
  }
  mFadrTaskData.NumOfCompleted++;
  ReleaseSpinLock (&mFadrApTaskLock);
}

/**

  This function is used to flushes the processor caches on AP

  @param[in/out] Buffer - The pointer to private data buffer.

  @retval None

**/
VOID
FadrCpuCacheFlushAp (
  IN OUT VOID  *Buffer
  )
{
  //
  // Run CPU Cache flush
  //
  FadrCpuCacheFlush (NULL);
  //
  // Hang CPU
  //
  FadrCpuHang();
}

/**
  Executes CPUs Cache Flush (serial mode)

  @param None

  @retval EFI_SUCCESS  - Operation completed successfully,
                         error code otherwise

**/
EFI_STATUS
FadrCpusCacheFlush (
  VOID
  )
{
  EFI_STATUS                Status;
  UINT64                    StartTime;
  UINT64                    EndTime;
  UINTN                     Cpu;
  EFI_PROCESSOR_INFORMATION CpuInfo;
  UINT8                     *CpuRunCacheFlushTask;
  UINTN                     NumOfRemaining;
  UINT16                    ProcessedSocketsMap = 0;

  if (NULL == mSmst) {
    return EFI_NOT_READY;
  }

  //
  // Start CPU Cache Flush Flow
  //
  FADR_DEBUG ((mFadrFlowInfoLevel, "[fADR] CPU cache flush\n"));

  //
  // Get CPU Cache Flow execution start time
  //
  StartTime = GetPerformanceCounter ();

  //
  // Allocate memory to build info about CPUs that will run cache flush task (depend of selected flush mode)
  //
  CpuRunCacheFlushTask = AllocateZeroPool ((sizeof (UINT8) * mSmst->NumberOfCpus));
  if (NULL == CpuRunCacheFlushTask) {
    return EFI_OUT_OF_RESOURCES;
  }
  //
  // Initialize task counter
  //
  ZeroMem (&mFadrTaskData, sizeof (mFadrTaskData));
  //
  // Select CPUs to run the flush tasks
  //
  for (Cpu = 0; Cpu < mSmst->NumberOfCpus; Cpu++) {
    if (Cpu == mSmst->CurrentlyExecutingCpu) {
      Status = mSmmCpuServiceProtocol->GetProcessorInfo (mSmmCpuServiceProtocol, Cpu, &CpuInfo);
      if (EFI_ERROR (Status)) {
        FreePool(CpuRunCacheFlushTask);
        return Status;
      }

      CpuRunCacheFlushTask[Cpu] = FADR_CPU_CACHE_FLUSH_RUN_BSP;
      ProcessedSocketsMap |= (BIT0 << CpuInfo.Location.Package);
      mFadrTaskData.NumOfScheduled++;
      break;
    }
  }
  //
  // Process all other CPUs
  //
  for (Cpu = 0; Cpu < mSmst->NumberOfCpus; Cpu++) {
    if (Cpu == mSmst->CurrentlyExecutingCpu) {
      continue;
    }

    Status = mSmmCpuServiceProtocol->GetProcessorInfo (mSmmCpuServiceProtocol, Cpu, &CpuInfo);
    if (EFI_ERROR (Status)) {
      FreePool(CpuRunCacheFlushTask);
      return Status;
    }

    if (!(ProcessedSocketsMap & (BIT0 << CpuInfo.Location.Package))) {
      CpuRunCacheFlushTask[Cpu] = FADR_CPU_CACHE_FLUSH_RUN_NON_BSP;
      ProcessedSocketsMap |= (BIT0 << CpuInfo.Location.Package);
      mFadrTaskData.NumOfScheduled++;
    } else {
      CpuRunCacheFlushTask[Cpu] = FADR_CPU_CACHE_FLUSH_HALT;
    }
  }

  FADR_DEBUG ((mFadrFlowInfoLevel, "[fADR] CPU cache flush - scheduled tasks: %d \n", mFadrTaskData.NumOfScheduled));
  //
  // Initialize eADR tasks spin lock
  //
  InitializeSpinLock(&mFadrApTaskLock);
  //
  // Start CPU cache flush
  //
  for (Cpu = 0; Cpu < mSmst->NumberOfCpus; Cpu++) {
    //
    // Halt CPU
    //
    if (CpuRunCacheFlushTask[Cpu] == FADR_CPU_CACHE_FLUSH_HALT) {
      Status = mSmst->SmmStartupThisAp (FadrCpuHangAp, Cpu, NULL);
      if (EFI_ERROR (Status)) {
        FADR_DEBUG ((DEBUG_ERROR, "[fADR] CPU (%d) failed to dispatch task (status %r)\n", Cpu, Status));
        FreePool(CpuRunCacheFlushTask);
        return Status;
      }
      continue;
    }

    Status = mSmmCpuServiceProtocol->GetProcessorInfo (mSmmCpuServiceProtocol, Cpu, &CpuInfo);
    if (EFI_ERROR (Status)) {
      FreePool(CpuRunCacheFlushTask);
      return Status;
    }

    FADR_DEBUG ((mFadrFlowInfoLevel, "[fADR] eADR S%d.C%d.T%d cache flush - start\n",
      CpuInfo.Location.Package, CpuInfo.Location.Core, CpuInfo.Location.Thread));

    //
    // Run CPU cache flush on BSP CPU
    //
    if (CpuRunCacheFlushTask[Cpu] == FADR_CPU_CACHE_FLUSH_RUN_BSP) {
      FadrCpuCacheFlush (NULL);
      continue;
    }
    //
    // Run CPU cache flush
    //
    if (CpuRunCacheFlushTask[Cpu] == FADR_CPU_CACHE_FLUSH_RUN_NON_BSP) {
      Status = mSmst->SmmStartupThisAp (FadrCpuCacheFlushAp, Cpu, NULL);
      if (EFI_ERROR (Status)) {
        FADR_DEBUG ((DEBUG_ERROR, "[fADR] CPU (%d) failed to dispatch task (status %r)\n", Cpu, Status));
        FreePool(CpuRunCacheFlushTask);
        return Status;
      }
      continue;
    }
  }

  //
  // Wait for all CPUs to start flush task
  //
  while (!FadrAllTasksStarted (&NumOfRemaining)) {
    FADR_DEBUG ((mFadrFlowInfoLevel, "[fADR] CPU cache flush - waiting for CPUs to start cache flush (remaining tasks %d)\n", NumOfRemaining));
    CpuPause ();
  }

  FADR_DEBUG ((mFadrFlowInfoLevel, "[fADR] CPU cache flush - all CPUs started cache flush\n"));

  //
  // Get CPU Cache Flow execution end time
  //
  EndTime = GetPerformanceCounter ();
  FADR_DEBUG ((mFadrTimeInfoLevel, "[fADR] CPU cache flush started successfully - time: %ld [us]\n",
    FADR_TIME_NS_TO_US (GetTimeInNanoSecond (EndTime - StartTime))));

  FreePool(CpuRunCacheFlushTask);
  return EFI_SUCCESS;
}

/**
  Executes IMC WPQ Flush

  @param None

  @retval EFI_SUCCESS  - Operation completed successfully,
                         error code otherwise

**/
EFI_STATUS
FadrImcWpqFlush (
  VOID
  )
{
  if (NULL == mCrystalRidgeProtocol) {
    return EFI_NOT_READY;
  }

  FADR_DEBUG ((mFadrFlowInfoLevel, "[fADR] IMC WPQ flush\n"));

  return mCrystalRidgeProtocol->ImcWpqFlush ();
}
/**

  fADR actions handler

  @param None

  @retval EFI_SUCCESS - Execution completed successfully,
                        error code otherwise

**/
EFI_STATUS
FadrActionsHandler (
  VOID
  )
{
  EFI_STATUS  Status;
  UINT64      StartTime;
  UINT64      EndTime;
  BOOLEAN     FlushErrorOccured = FALSE;

  //
  // Get fADR flow start time
  //
  StartTime = GetPerformanceCounter ();

  //
  // Start the fADR flow
  //
  FADR_DEBUG ((mFadrFlowInfoLevel, "[fADR] fADR Start\n"));

  //
  // Update Extended ADR (fADR) stage - phase one start
  //
  Status = FadrSetPmemModuleExtendedAdrLssState (FADR_STAGE_1_PHASE_1_START);
  if (EFI_ERROR (Status)) {
    FADR_DEBUG ((DEBUG_ERROR, "[fADR] Setting Extended ADR (fADR) stage (ADR%d) failed (status %r)\n", FADR_STAGE_1_PHASE_1_START, Status));
    return Status;
  }

  //
  // Change CPU performance state
  //
  Status = FadrChangeCpusPerformanceState ();
  if (EFI_ERROR (Status)) {
    FADR_DEBUG ((DEBUG_ERROR, "[fADR] Changing CPU performance state failed (status %r)\n", FADR_STAGE_1_PHASE_1_START, Status));
    return Status;
  }

  //
  // Update Extended ADR (fADR) stage - CPU Cache flush start
  //
  Status = FadrSetPmemModuleExtendedAdrLssState (FADR_STAGE_7_CPU_CACHE_FLUSH_START);
  if (EFI_ERROR (Status)) {
    FADR_DEBUG ((DEBUG_ERROR, "[fADR] Setting Extended ADR (fADR) stage (ADR%d) failed (status %r)\n", FADR_STAGE_7_CPU_CACHE_FLUSH_START, Status));
    return Status;
  }

  //
  // Execute Core Cache flush
  //
  Status = FadrCpusCacheFlush ();
  if (EFI_ERROR (Status)) {
    FADR_DEBUG ((DEBUG_ERROR, "[fADR] CPU Cache flush execution failed (status %r)\n", Status));
    FlushErrorOccured = TRUE;
  }

  //
  // Update Extended ADR (fADR) stage - Extended Flush Completed
  //
  if (!FlushErrorOccured) {
    Status = FadrSetPmemModuleExtendedAdrLssState (FADR_STAGE_15_EXTENDED_FLUSH_COMPLETION);
    if (EFI_ERROR (Status)) {
      FADR_DEBUG ((DEBUG_ERROR, "[fADR] Setting Extended ADR (fADR) stage (ADR%d) failed (status %r)\n", FADR_STAGE_15_EXTENDED_FLUSH_COMPLETION, Status));
      return Status;
    }
  }

  //
  // Execute IMC WPQ flush
  //
  Status = FadrImcWpqFlush ();
  if (EFI_ERROR (Status)) {
    FADR_DEBUG ((DEBUG_ERROR, "[fADR] IMC WPQ flush execution failed (status %r)\n", Status));
  }

  //
  // Get fADR flow end time
  //
  EndTime = GetPerformanceCounter ();
  FADR_DEBUG ((mFadrTimeInfoLevel, "[fADR] fADR flow completed %s - time: %ld [us]\n", (!FlushErrorOccured) ? L"successfully" : L"with errors", FADR_TIME_NS_TO_US (GetTimeInNanoSecond (EndTime - StartTime))));

  return EFI_SUCCESS;
}

/**

  fADR S3/S4/S5 entry handler

  @param[in] DispatchHandle         - The unique dispatch handle assigned to this handler by SmiHandlerRegister().
  @param[in] DispatchContext        - Pointer to the optional handler context which was specified when the handler was registered.
  @param[in, out] CommBuffer        - Buffer used for communication between caller/callback function.
  @param[in, out] CommBufferSize    - The size of the communication buffer.

  @retval EFI_SUCCESS               - Execution completed successfully,
                                      error code otherwise

**/
EFI_STATUS
FadrSxEntryHandler (
  IN  EFI_HANDLE                        DispatchHandle,
  IN CONST  EFI_SMM_SX_REGISTER_CONTEXT *DispatchContext,
  IN OUT    VOID                        *CommBuffer,     OPTIONAL
  IN OUT    UINTN                       *CommBufferSize  OPTIONAL
  )
{
  EFI_STATUS Status;

  if (NULL == DispatchContext) {
    return EFI_DEVICE_ERROR;
  }

  if (!FadrIsSxTypeEntrySupported (DispatchContext->Type)) {
    return EFI_UNSUPPORTED;
  }

  FADR_DEBUG ((mFadrFlowInfoLevel, "[fADR] S%d entry handler\n", DispatchContext->Type));

  //
  // execute fADR flow
  //
  Status = FadrActionsHandler ();
  if (EFI_ERROR (Status)) {
    FADR_DEBUG ((DEBUG_ERROR,"[fADR] fADR flow execution failed (status %r)\n", Status));
    return Status;
  }

  return EFI_SUCCESS;
}

/**

  Registers fADR S3/S4/S5 entry handler

  @param[in] SxType     - Type of SX entry to register handler for

  @retval EFI_SUCCESS   - Operation completed successfully,
                           error code otherwise

**/
EFI_STATUS
FadrRegisterSxEntryHandler (
  IN EFI_SLEEP_TYPE SxType
  )
{
  EFI_STATUS                  Status;
  EFI_SMM_SX_REGISTER_CONTEXT SxContext;

  if (NULL == mSxDispatch) {
    return EFI_NOT_READY;
  }

  switch (SxType) {
    case SxS3:
      FADR_DEBUG ((mFadrDriverInfoLevel, "[fADR] Register fADR S3 entry handler\n"));

      ZeroMem (&SxContext, sizeof(SxContext));
      SxContext.Phase = SxEntry;
      SxContext.Type = SxS3;

      Status = mSxDispatch->Register (mSxDispatch, FadrSxEntryHandler, &SxContext, &mFadrData.Smi.S3EntryHandle);
      if (EFI_ERROR (Status)) {
        FADR_DEBUG ((DEBUG_ERROR, "[fADR] fADR S3 entry handler registration failed (status %r)\n", Status));
        return Status;
      }
      return EFI_SUCCESS;

    case SxS4:
      FADR_DEBUG ((mFadrDriverInfoLevel, "[fADR] Register fADR S4 entry handler\n"));

      ZeroMem (&SxContext, sizeof(SxContext));
      SxContext.Phase = SxEntry;
      SxContext.Type = SxS4;

      Status = mSxDispatch->Register (mSxDispatch, FadrSxEntryHandler, &SxContext, &mFadrData.Smi.S4EntryHandle);
      if (EFI_ERROR (Status)) {
        FADR_DEBUG ((DEBUG_ERROR, "[fADR] fADR S4 entry handler registration failed (status %r)\n", Status));
        return Status;
      }
      return EFI_SUCCESS;

    case SxS5:
      FADR_DEBUG ((mFadrDriverInfoLevel, "[fADR] Register fADR S5 entry handler\n"));

      ZeroMem (&SxContext, sizeof(SxContext));
      SxContext.Phase = SxEntry;
      SxContext.Type = SxS5;

      Status = mSxDispatch->Register (mSxDispatch, FadrSxEntryHandler, &SxContext, &mFadrData.Smi.S5EntryHandle);
      if (EFI_ERROR (Status)) {
        FADR_DEBUG ((DEBUG_ERROR, "[fADR] fADR S5 entry handler registration failed (status %r)\n", Status));
        return Status;
      }
      return EFI_SUCCESS;

    default:
      return EFI_UNSUPPORTED;
  }
}

/**

  fADR CF9 reset handler

  @param[in] DispatchHandle         - The unique dispatch handle assigned to this handler by SmiHandlerRegister().
  @param[in] DispatchContext        - Pointer to the optional handler context which was specified when the handler was registered.
  @param[in, out] CommBuffer        - Buffer used for communication between caller/callback function.
  @param[in, out] CommBufferSize    - The size of the communication buffer.

  @retval EFI_SUCCESS               - Execution completed successfully,
                                      error code otherwise

**/
EFI_STATUS
FadrResetHandler (
  IN     EFI_HANDLE                     DispatchHandle,
  IN     CONST EFI_SMM_IO_TRAP_CONTEXT  *DispatchContext, OPTIONAL
  IN OUT VOID                           *CommBuffer,      OPTIONAL
  IN OUT UINTN                          *CommBufferSize   OPTIONAL
  )
{
  EFI_STATUS Status;
  UINT8      ResetType;

  //
  // We need to have the context to get the write data
  //
  if (NULL == DispatchContext) {
    return EFI_DEVICE_ERROR;
  }

  //
  // Retrieve the reset type
  //
  ResetType = (UINT8)(DispatchContext->WriteData >> FADR_PCH_IO_CF9_RESET_TRAP_SHIFT);

  //
  // Check the reset type
  //
  if (!FadrIsResetTypeSupported (ResetType)) {
    return EFI_UNSUPPORTED;
  }

  FADR_DEBUG ((mFadrFlowInfoLevel, "[fADR] Reset handler entry (reset type: %s)\n", (ResetType == FADR_PCH_IO_CF9_TRAP_COLD_RESET) ? L"Cold Reset" : L"Warm Reset"));

  //
  // execute fADR flow
  //
  Status = FadrActionsHandler ();
  if (EFI_ERROR (Status)) {
    FADR_DEBUG ((DEBUG_ERROR,"[fADR] fADR flow execution failed (status %r)\n", Status));
    return Status;
  }

  //
  // Pause 0xCF9 trap dispatch
  //
  Status = mPchIoTrapCtrl->Pause (mPchIoTrapCtrl, DispatchHandle);
  if (EFI_ERROR (Status)) {
    FADR_DEBUG ((DEBUG_ERROR,"[fADR] Pausing of reset trap failed (status %r)\n", Status));
    return Status;
  }

  //
  // Continue with reset
  //
  FADR_DEBUG ((mFadrFlowInfoLevel,"[fADR] Resetting the platform (reset type: %s)\n", (ResetType == FADR_PCH_IO_CF9_TRAP_COLD_RESET) ? L"Cold Reset" : L"Warm Reset"));

  Status = mSmst->SmmIo.Io.Write (&mSmst->SmmIo, SMM_IO_UINT8, FADR_PCH_IO_CF9_TRAP_ADDRESS, FADR_PCH_IO_CF9_TRAP_IO_OPS_CNT, &ResetType);
  if (EFI_ERROR (Status)) {
    FADR_DEBUG ((DEBUG_ERROR,"[fADR] Resetting the platform (reset type: %s) failed (status %r)\n", (ResetType == FADR_PCH_IO_CF9_TRAP_COLD_RESET) ? L"Cold Reset" : L"Warm Reset", Status));
    return Status;
  }

  return EFI_SUCCESS;
}

/**

  Registers fADR CF9 reset handler

  @param None

  @retval EFI_SUCCESS - Operation completed successfully,
                        error code otherwise

**/
EFI_STATUS
FadrRegisterResetHandler (
  VOID
  )
{
  EFI_STATUS                        Status;
  EFI_SMM_IO_TRAP_REGISTER_CONTEXT  IoTrapContext;

  if (NULL == mIoTrapDispatch) {
    return EFI_NOT_READY;
  }

  FADR_DEBUG ((mFadrDriverInfoLevel, "[fADR] Register fADR CF9 reset handler\n"));

  ZeroMem (&IoTrapContext, sizeof(IoTrapContext));
  IoTrapContext.Type     = WriteTrap;
  IoTrapContext.Length   = FADR_PCH_IO_CF9_TRAP_LENGTH;
  IoTrapContext.Address  = FADR_PCH_IO_CF9_TRAP_ADDRESS;

  Status = mIoTrapDispatch->Register (mIoTrapDispatch, (EFI_SMM_HANDLER_ENTRY_POINT2)FadrResetHandler, &IoTrapContext, &mFadrData.Smi.ResetHandle);
  if (EFI_ERROR (Status)) {
    FADR_DEBUG ((DEBUG_ERROR, "[fADR] fADR CF9 reset handler registration failed (status %r)\n", Status));
    return Status;
  }

  return EFI_SUCCESS;
}

/**

  Configures PCU for fADR

  @param None

  @retval EFI_SUCCESS - Configuration completed successfully,
                        error code otherwise

**/
EFI_STATUS
FadrConfigurePcu (
  VOID
  )
{
  SYSTEM_MEMORY_MAP_HOB *SystemMemoryMap;
  UINT8                 Socket;

  SystemMemoryMap = GetSystemMemoryMapData ();
  if (SystemMemoryMap == NULL) {
    return EFI_NOT_FOUND;
  }

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (!SystemMemoryMap->Socket[Socket].SocketEnabled) {
        continue;
    }
    FADR_DEBUG ((mFadrDriverInfoLevel, "[fADR] fADR - socket (%d) PCU configuration\n", Socket));
    PcuSetFadrConfig (Socket);
  }
  return EFI_SUCCESS;
}

/**

  Configures fADR driver

  @param None

  @retval EFI_SUCCESS - Configuration completed successfully,
                        error code otherwise

**/
EFI_STATUS
FadrConfigure (
  VOID
  )
{
  EFI_STATUS      Status;
  EFI_SLEEP_TYPE  SxType;
  FADR_FLOWS      FadrFlow;
  UINT8           FadrFlowsEnabled = 0;

  FADR_DEBUG ((mFadrDriverInfoLevel, "[fADR] fADR configuration\n"));

  //
  // Enable extended ADR (fADR) support on PMEM Modules
  //
  Status = FadrSetPmemModuleExtendedAdrState (EXT_ADR_ENABLED);
  if (EFI_ERROR (Status)) {
    FADR_DEBUG ((DEBUG_ERROR, "[fADR] fADR - PMEM Modules configuration (status %r)\n", Status));
    return Status;
  }

  //
  // Configure PCU
  //
  Status = FadrConfigurePcu ();
  if (!EFI_ERROR (Status)) {
    FadrFlowsEnabled |= (BIT0 << FADR_PCU_ASSISTED_IIO_CPU_CACHE_FLUSH);
  } else {
    FADR_DEBUG ((DEBUG_ERROR, "[fADR] fADR - PCU configuration failed (status %r)\n", Status));
  }

  //
  // Register fADR reset SMI handler
  //
  Status = FadrRegisterResetHandler ();
  if (!EFI_ERROR (Status)) {
    FadrFlowsEnabled |= (BIT0 << FADR_RESET_CPU_CACHE_FLUSH);
  } else {
    FADR_DEBUG ((DEBUG_ERROR, "[fADR] fADR - CF9 Reset SMI handler configuration failed (status %r)\n", Status));
  }

  //
  // Register fADR S3/S4/S5 transition SMI handlers
  //
  for (SxType = SxS3; SxType <= SxS5; SxType++) {
    Status = FadrRegisterSxEntryHandler (SxType);
    if (!EFI_ERROR (Status)) {
      switch (SxType) {
        case SxS3:
          FadrFlowsEnabled |= (BIT0 << FADR_S3_ENTRY_CPU_CACHE_FLUSH);
          break;

        case SxS4:
          FadrFlowsEnabled |= (BIT0 << FADR_S4_ENTRY_CPU_CACHE_FLUSH);
          break;

        case SxS5:
          FadrFlowsEnabled |= (BIT0 << FADR_S5_ENTRY_CPU_CACHE_FLUSH);
          break;
      }
    } else {
      FADR_DEBUG ((DEBUG_ERROR, "[fADR] fADR - S%d Entry SMI handler configuration failed (status %r)\n", SxType, Status));
    }
  }

  if (FadrFlowsEnabled == FADR_ALL_FLOWS_DISABLED) {
    Status = FadrSetPmemModuleExtendedAdrState (EXT_ADR_DISABLED);
    if (EFI_ERROR (Status)) {
      FADR_DEBUG ((DEBUG_ERROR, "[fADR] PMEM Modules fADR state update failed (status %r)\n", Status));
    }
    Status = FadrSetExtendedAdrEnabledFlows (FADR_ALL_FLOWS_DISABLED);
    if (EFI_ERROR (Status)) {
      FADR_DEBUG ((DEBUG_ERROR, "[fADR] fADR enabled flows update failed (status %r)\n", Status));
    }
    return EFI_UNSUPPORTED;
  }

  //
  // Print info about enabled flows
  //
  for (FadrFlow = FADR_PCU_ASSISTED_IIO_CPU_CACHE_FLUSH; FadrFlow < FADR_FLOWS_MAX; FadrFlow++) {
    FADR_DEBUG (((FadrFlowsEnabled != FADR_ALL_FLOWS_ENABLED) ? DEBUG_ERROR : mFadrDriverInfoLevel,
      "[fADR] fADR %s - %s\n", mFadrFlowName[FadrFlow], (FadrFlowsEnabled & (BIT0 << FadrFlow)) ? L"enabled" : L"disabled"));
  }

  //
  // Update info about enabled flows
  //
  Status = FadrSetExtendedAdrEnabledFlows (FadrFlowsEnabled);
  if (EFI_ERROR (Status)) {
    FADR_DEBUG ((DEBUG_ERROR, "[fADR] fADR enabled flows update failed (status %r)\n", Status));
  }
  return EFI_SUCCESS;
}

/**

  Verifies if memory mode is supported

  @param None

  @retval TRUE  - memory mode is supported
          FALSE - memory mode is not supported

**/
BOOLEAN
FadrIsMemoryModeSupported (
  VOID
  )
{
  SYSTEM_MEMORY_MAP_HOB *SystemMemoryMap;
  BOOLEAN               MemTypeSupported = FALSE;
  UINT8                 Socket;
  UINT8                 Sad;

  SystemMemoryMap = GetSystemMemoryMapData ();
  if (SystemMemoryMap == NULL) {
    return FALSE;
  }

  //
  // Check SAD rules for all sockets (no need to enable fADR for memory mode only mode)
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (!SystemMemoryMap->Socket[Socket].SocketEnabled) {
      continue;
    }
    for (Sad = 0; Sad < MAX_SAD_RULES; Sad++) {
      if ((SystemMemoryMap->Socket[Socket].SAD[Sad].Enable == 0) ||
          (SystemMemoryMap->Socket[Socket].SAD[Sad].local == 0)) {
        continue;
      }
      if (IsMemTypeAppDirect (SystemMemoryMap->Socket[Socket].SAD[Sad].type)) {
        //
        // PMEM region found - enable fADR
        //
        MemTypeSupported = TRUE;
        break;
      }
    }
  }

  if (!MemTypeSupported) {
    FADR_DEBUG ((DEBUG_ERROR, "[fADR] System configuration not supported (PMEM regions not found)\n"));
    return FALSE;
  }

  return TRUE;
}

/**

  Verifies if system configuration supports fADR

  @param None

  @retval TRUE  - system configuration supports fADR
          FALSE - system configuration doesn't support

**/
BOOLEAN
FadrIsConfigSupported (
  VOID
  )
{
  SYSTEM_MEMORY_MAP_HOB *SystemMemoryMap;
  BOOLEAN               SupportedNvdimmFound = FALSE;
  UINT8                 Socket;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT8                 MaxCh = GetMaxChDdr ();

  SystemMemoryMap = GetSystemMemoryMapData ();
  if (SystemMemoryMap == NULL) {
    return FALSE;
  }

  //
  // Check system configuration (look for PMEM Modules with fADR support)
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (!SystemMemoryMap->Socket[Socket].SocketEnabled) {
      continue;
    }
    for (Channel = 0; Channel < MaxCh; Channel++) {
      if (!SystemMemoryMap->Socket[Socket].ChannelInfo[Channel].Enabled) {
        continue;
      }
      for (Dimm = 0; Dimm < SystemMemoryMap->Socket[Socket].ChannelInfo[Channel].MaxDimm; Dimm++) {
        if (!SystemMemoryMap->Socket[Socket].ChannelInfo[Channel].DimmInfo[Dimm].DcpmmPresent) {
          continue;
        }
        //
        // Supported PMEM Module found
        //
        if(SystemMemoryMap->Socket[Socket].ChannelInfo[Channel].DimmInfo[Dimm].SubsystemDeviceID == FADR_SUBSYSTEM_DEV_ID_CWV) {
          SupportedNvdimmFound = TRUE;
          break;
        }
      }
    }
  }

  if (!SupportedNvdimmFound) {
    FADR_DEBUG ((DEBUG_ERROR, "[fADR] System configuration not supported (PMEM Modules doesn't support fADR)\n"));
    return FALSE;
  }

  return TRUE;
}

/**

  Initializes the fADR driver

  @param None

  @retval EFI_SUCCESS     - Initialization completed successfully,
                            error code otherwise

**/
EFI_STATUS
FadrInitialize (
  VOID
  )
{
  EFI_STATUS            Status;
  SYSTEM_MEMORY_MAP_HOB *SystemMemoryMap;

  FADR_DEBUG ((mFadrDriverInfoLevel, "[fADR] fADR initialization\n"));

  //
  // Check if system configuration supports fADR
  //
  if (!FadrIsConfigSupported ()) {
    return EFI_UNSUPPORTED;
  }

  //
  // Check if current memory mode supports fADR
  //
  if (!FadrIsMemoryModeSupported ()) {
    Status = FadrSetPmemModuleExtendedAdrState (EXT_ADR_DISABLED);
    if (EFI_ERROR (Status)) {
      FADR_DEBUG ((DEBUG_ERROR, "[fADR] PMEM Modules fADR state update failed (status %r)\n", Status));
    }
    Status = FadrSetExtendedAdrEnabledFlows (FADR_ALL_FLOWS_DISABLED);
    if (EFI_ERROR (Status)) {
      FADR_DEBUG ((DEBUG_ERROR, "[fADR] fADR enabled flows update failed (status %r)\n", Status));
    }
    return EFI_UNSUPPORTED;
  }

  //
  // Get Memory Map data to retrieve fADR parameters
  //
  SystemMemoryMap = GetSystemMemoryMapData ();
  if (SystemMemoryMap == NULL) {
    Status = FadrSetPmemModuleExtendedAdrState (EXT_ADR_DISABLED);
    if (EFI_ERROR (Status)) {
      FADR_DEBUG ((DEBUG_ERROR, "[fADR] PMEM Modules fADR state update failed (status %r)\n", Status));
    }
    Status = FadrSetExtendedAdrEnabledFlows (FADR_ALL_FLOWS_DISABLED);
    if (EFI_ERROR (Status)) {
      FADR_DEBUG ((DEBUG_ERROR, "[fADR] fADR enabled flows update failed (status %r)\n", Status));
    }
    return EFI_LOAD_ERROR;
  }

  //
  // Check if fADR enabled
  //
  if (SystemMemoryMap->MemSetup.FadrSupport == FADR_DISABLED) {
    Status = FadrSetPmemModuleExtendedAdrState (EXT_ADR_DISABLED);
    if (EFI_ERROR (Status)) {
      FADR_DEBUG ((DEBUG_ERROR, "[fADR] PMEM Modules fADR state update failed (status %r)\n", Status));
    }
    Status = FadrSetExtendedAdrEnabledFlows (FADR_ALL_FLOWS_DISABLED);
    if (EFI_ERROR (Status)) {
      FADR_DEBUG ((DEBUG_ERROR, "[fADR] fADR enabled flows update failed (status %r)\n", Status));
    }
    return EFI_UNSUPPORTED;
  }

  //
  // Initialize fADR data storage
  //
  ZeroMem (&mFadrData, sizeof(mFadrData));

  return EFI_SUCCESS;
}

/**

  Locates protocols used by the fADR driver

  @param None

  @retval EFI_SUCCESS - Operation completed successfully,
                        error code otherwise

**/
EFI_STATUS
FadrLocateProtocols (
  VOID
  )
{
  EFI_STATUS Status;

  if (NULL == mSmst) {
    return EFI_NOT_READY;
  }

  Status = mSmst->SmmLocateProtocol (&gEfiSmmCpuServiceProtocolGuid, NULL, &mSmmCpuServiceProtocol);
  if (EFI_ERROR (Status)) {
    FADR_DEBUG ((DEBUG_ERROR, "[fADR] SMM CPU Services protocol location error (status %r)\n", Status));
    return Status;
  }

  Status = mSmst->SmmLocateProtocol (&gEfiSmmIoTrapDispatch2ProtocolGuid, NULL, &mIoTrapDispatch);
  if (EFI_ERROR (Status)) {
    FADR_DEBUG ((DEBUG_ERROR, "[fADR] SMM IO Trap protocol location error (status %r)\n", Status));
    return Status;
  }

  Status = mSmst->SmmLocateProtocol (&gPchSmmIoTrapControlGuid, NULL, &mPchIoTrapCtrl);
  if (EFI_ERROR (Status)) {
    FADR_DEBUG ((DEBUG_ERROR,"[fADR] Can't locate PCH SMM IO Trap protocol (status %r)\n", Status));
    return Status;
  }

  Status = mSmst->SmmLocateProtocol (&gEfiSmmSxDispatch2ProtocolGuid, NULL, &mSxDispatch);
  if (EFI_ERROR (Status)) {
    FADR_DEBUG ((DEBUG_ERROR, "[fADR] SMM SX Dispatch protocol location error (status %r)\n", Status));
    return Status;
  }

  Status = mSmst->SmmLocateProtocol (&gEfiCrystalRidgeSmmGuid, NULL, &mCrystalRidgeProtocol);
  if (EFI_ERROR (Status)) {
    FADR_DEBUG ((DEBUG_ERROR, "[fADR] Crystal Ridge SMM protocol location error (status %r)\n", Status));
    return Status;
  }

  return EFI_SUCCESS;
}

/**

  Sets fADR driver/fADR flow/fADR time info messages verbosity

  @param None

  @retval None

**/
VOID
FadrSetDriverInfoLevel (
  VOID
  )
{
  SYSTEM_MEMORY_MAP_HOB *SystemMemoryMap;

  SystemMemoryMap = GetSystemMemoryMapData ();
  if (SystemMemoryMap != NULL) {
    //
    // Set fADR Driver and fADR flow info messages level
    //
    switch (SystemMemoryMap->MemSetup.dfxMemSetup.DfxFadrDebugLogsEnable) {
      //
      // Increase verbosity of messages printed by fADR driver and during fADR flow execution
      //
      case FADR_DEBUG_LOGS_ALL:
        mFadrDriverInfoLevel = DEBUG_ERROR;
        mFadrFlowInfoLevel = DEBUG_ERROR;
        break;

      //
      // Increase verbosity of messages printed by fADR driver
      //
      case FADR_DEBUG_LOGS_DRIVER:
        mFadrDriverInfoLevel = DEBUG_ERROR;
        break;

      //
      // Increase verbosity of messages printed during fADR flow execution
      //
      case FADR_DEBUG_LOGS_FLOW:
        mFadrFlowInfoLevel = DEBUG_ERROR;
        break;

      //
      // Auto - follow system level of debug traces
      //
      case FADR_DEBUG_LOGS_AUTO:
        mFadrDriverInfoLevel = DEBUG_INFO;
        mFadrFlowInfoLevel = DEBUG_INFO;
        break;

      default:
        mFadrDriverInfoLevel = DEBUG_INFO;
        mFadrFlowInfoLevel = DEBUG_INFO;
        break;
    }

    //
    // Set fADR flow time info messages level
    //
    if (SystemMemoryMap->MemSetup.dfxMemSetup.DfxFadrFlowTimeLogsEnable) {
      mFadrTimeInfoLevel = DEBUG_ERROR;
    } else {
      mFadrTimeInfoLevel = DEBUG_INFO;
    }
  } else {
    //
    // Memory Map data not found - set default level
    //
    mFadrDriverInfoLevel = DEBUG_INFO;
    mFadrFlowInfoLevel = DEBUG_INFO;
    mFadrTimeInfoLevel = DEBUG_INFO;
  }
}

/**

  Initializes fADR task and fADR debug spin locks

  @param None

  @retval None

**/
VOID
FadrInitSpinLocks (
  VOID
  )
{
  InitializeSpinLock (&mFadrApTaskLock);
  InitializeSpinLock (&mFadrDbgMsgLock);
}

/**

  fADR SMM driver entry point

  Initializes the Fast ADR SMM Driver

  @param[in] ImageHandle  - The image handle of SMI Handler Driver.
  @param[in] SystemTable  - The standard EFI system table.

  @retval EFI_SUCCESS     - Initialization completed successfully,
                            error code otherwise

**/
EFI_STATUS
EFIAPI
FadrDriver (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS              Status;
  BOOLEAN                 InSmm;
  EFI_SMM_BASE2_PROTOCOL  *SmmBase = NULL;

  Status = gBS->LocateProtocol (&gEfiSmmBase2ProtocolGuid, NULL, (VOID **) &SmmBase);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (SmmBase == NULL) {
    InSmm = FALSE;
  } else {
    Status = SmmBase->InSmm (SmmBase, &InSmm);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  if (!InSmm) {
    return EFI_UNSUPPORTED;
  }

  FadrInitSpinLocks ();

  FadrSetDriverInfoLevel ();

  Status = SmmBase->GetSmstLocation (SmmBase, &mSmst);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Locate necessary protocols
  //
  Status = FadrLocateProtocols ();
  if (EFI_ERROR (Status)) {
    FADR_DEBUG ((DEBUG_ERROR, "[fADR] fADR driver used protocols location failed (status %r)\n", Status));
    return Status;
  }

  //
  // Initialize fADR driver:
  // - verify if PMEM Modules supports fADR
  // - verify if current memory mode supports fADR
  //
  Status = FadrInitialize ();
  if (EFI_ERROR (Status)) {
    FADR_DEBUG ((DEBUG_ERROR, "[fADR] fADR driver initialization failed (status %r)\n", Status));
    return Status;
  }

  //
  // Configure fADR driver:
  // - enable extended ADR (fADR) on PMEM Module
  // - PCU configuration
  // - cold and warm rests handlers for CPU cache flush configuration
  // - S3/S4/S5 entry handlers for CPU cache flush configuration
  //
  Status = FadrConfigure ();
  if (EFI_ERROR (Status)) {
    FADR_DEBUG ((DEBUG_ERROR, "[fADR] fADR driver configuration failed (status %r)\n", Status));
    return Status;
  }

  FADR_DEBUG ((mFadrDriverInfoLevel, "[fADR] fADR driver started\n"));
  return EFI_SUCCESS;
}
