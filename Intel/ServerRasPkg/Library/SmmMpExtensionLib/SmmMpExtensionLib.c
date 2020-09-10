/** @file
  Implementation of generate RAS global data access for silicon library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2017 Intel Corporation. <BR>

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

#include <PiSmm.h>
#include <Protocol/SmmCpuService.h>

#include <Library/BaseLib.h>
#include <Library/SynchronizationLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/MpExtensionLib.h>
#include <Library/RasDebugLib.h>

typedef struct _CALLBACK_WRAPPER {
  EFI_AP_PROCEDURE       SmmApFunction;
  VOID                   *ProcArguments;
  volatile UINT32        CpuCount;
} CALLBACK_WRAPPER;


typedef struct _CPU_INFO {
  UINTN                     CpuIndex;
  BOOLEAN                   Started;
  UINT32                    Package;
  UINT32                    Core;
} CPU_INFO;


//
// Global variable to cache pointer to SMM CPU Service protocol.
//
EFI_SMM_CPU_SERVICE_PROTOCOL      *mSmmCpuServiceProtocolInterface = NULL;

//
// Pre-allocated global CPU info buffer to avoid poll allocation in SMM runtime.
//
CPU_INFO                          *mCpuInfoList = NULL;
UINT32                            mBspPackage = 0;
UINT32                            mBspCore = 0;
BOOLEAN                           mBspScheduled = FALSE;

/**
  The constructor function caches the pointer to SMM CPU Service protocol.

  The constructor function locates SMM CPU Service protocol from protocol database.
  It will ASSERT() if that operation fails and it will always return EFI_SUCCESS.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
InitializeSmmMpExtensionLib (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
{
  EFI_STATUS  Status;

  Status = gSmst->SmmLocateProtocol (&gEfiSmmCpuServiceProtocolGuid, NULL, (VOID**) &mSmmCpuServiceProtocolInterface);
  ASSERT_EFI_ERROR (Status);
  RAS_ASSERT (mSmmCpuServiceProtocolInterface != NULL);

  mCpuInfoList = AllocatePool (sizeof (CPU_INFO) * PcdGet32 (PcdCpuMaxLogicalProcessorNumber));
  RAS_ASSERT (mCpuInfoList != NULL);

  return EFI_SUCCESS;
}

/**

  Worker function to prepare the AP list ready to run.

  @param  Package                Specific Package number.  ALL_PACKAGE means the whole system.

  @return the number of AP list ready to run.

**/
UINT32
GetCpuList (
  IN  UINT32                      Package,
  IN  UINT32                      Core
  )
{
  EFI_STATUS                      Status;
  UINTN                           Index;
  CPU_INFO                        *CpuPtr;
  EFI_PROCESSOR_INFORMATION       ProcessorInfo;

  CpuPtr = mCpuInfoList;
  mBspScheduled = FALSE;

  for (Index = 0; Index < gSmst->NumberOfCpus; Index++) {
    Status = mSmmCpuServiceProtocolInterface->GetProcessorInfo (mSmmCpuServiceProtocolInterface, Index, &ProcessorInfo);
    if (Status == EFI_NOT_FOUND) {
      continue;
    }
    ASSERT_EFI_ERROR (Status);

    if ((Package == ALL_PACKAGE) || (ProcessorInfo.Location.Package == Package)) {
      if ((Core == ALL_CORE) || (ProcessorInfo.Location.Core == Core)) {
        if (Index != gSmst->CurrentlyExecutingCpu) {
          CpuPtr->CpuIndex = Index;
          CpuPtr->Started = FALSE;
          CpuPtr->Package = ProcessorInfo.Location.Package;
          CpuPtr->Core = ProcessorInfo.Location.Core;
          CpuPtr++;
        } else {
          mBspPackage = ProcessorInfo.Location.Package;
          mBspCore = ProcessorInfo.Location.Core;
          mBspScheduled = TRUE;
        }
      }
    }
  }

  return (UINT32) (CpuPtr - mCpuInfoList);
}


/**

  Worker function to check whether current CPU needs to run.
  If one AP in the same package/core has been scheduled before, it will return TRUE; otherwise, it return FALSE.

  It always returns FALSE if ApRunType is AllAps.

  @param  CurrentIndex  The index for the current CPU in the mCpuInfoList.
  @param  ApRunType     Specify AP run type: All APs, one AP per core or one AP per package.

  @retval TRUE         If one AP in the same package/core has been scheduled before.
  @retval FALSE        If none of AP in the same package/core has been scheduled before.

**/
BOOLEAN
CheckApScheduled (
  IN UINTN              CurrentIndex,
  IN AP_RUN_TYPE        ApRunType
  )
{
  UINT32                CurrentPackage;
  UINT32                CurrentCore;
  UINTN                 Index;

  if (ApRunType == AllAps) {
    return FALSE;
  }

  CurrentPackage = mCpuInfoList[CurrentIndex].Package;
  CurrentCore = mCpuInfoList[CurrentIndex].Core;
  if (mBspScheduled && (CurrentPackage == mBspPackage)) {
    if ((ApRunType == OneApPerPackage) || (CurrentCore == mBspCore)) {
      //
      // BSP in the same package/core has been scheduled, skip this AP.
      //
      return TRUE;
    }
  }

  for (Index = 0; Index < CurrentIndex; Index++) {
    if (mCpuInfoList[Index].Started && (mCpuInfoList[Index].Package == CurrentPackage)) {
      if ((ApRunType == OneApPerPackage) || (mCpuInfoList[Index].Core == CurrentCore)) {
        //
        // Previous AP in the same package/core has started successfully, skip this AP.
        //
        return TRUE;
      }
    }
  }

  return FALSE;
}

/**

  Worker function to wait until  whether current CPU needs to run. This function will wait until
  the completed AP counts has reached the input ExpectCpuCount.

  @param  CallbackWrapper             The CALLBACK_WRAPPER structure containing completed CPU count.
  @param  ExpectCpuCount              The number of expected CPU count.

**/
VOID
WaitForCpuCount (
  IN CALLBACK_WRAPPER       *CallbackWrapper,
  IN UINTN                  ExpectCpuCount
  )
{
  while (CallbackWrapper->CpuCount < ExpectCpuCount) {
    CpuPause ();
  }
}


/**

  Worker function to increase completed AP number. After complete user AP function,
  it will increase the completed count. BPS can track the total completed AP number.

  @param  CallbackWrapper             The CALLBACK_WRAPPER structure containing completed CPU count.

**/
VOID
SmmApCallbackWrapper (
  IN OUT VOID         *CallbackWrapper
  )
{
  CALLBACK_WRAPPER    *Wrapper;

  Wrapper = (CALLBACK_WRAPPER *) CallbackWrapper;

  Wrapper->SmmApFunction (Wrapper->ProcArguments);

  InterlockedIncrement ((UINT32 *) &Wrapper->CpuCount);
}

/**
  Run simultaneously or one by one under SMM mode on specific package/core or whole system.

  This service lets the caller get all enabled APs to execute a caller-provided function. This function will
  ensure all ApProcedure will exit.

  Note that BSP itself might also execute SmmProcedure if BSP package/core meet the requirement.
  If ApRunType is larger or equal to MaxApRunType, then ASSERT().
  If ApProcedure is NULL, then ASSERT()

  @param  ApRunType             Specify AP run type: All APs, one AP per core or one AP per package.
  @param  Package               Specific package number.  ALL_PACKAGE means the whole system.
  @param  Core                  Specific core number.  ALL_CORE means all cores in the package.
  @param  SingleThread          If TRUE, then all the enabled APs execute the function specified by Procedure one by one.
                                If FALSE, then all the enabled APs execute the function specified by Procedure simultaneously
  @param  ApProcedure           A pointer to the function to be run on enabled APs of the system.
  @param  ProcArguments         Pointer to the optional parameter of the assigned function.

  @retval RETRURN_SUCCESS       In blocking mode, all APs have finished before the timeout expired.
  @retval RETRURN_SUCCESS       In non-blocking mode, function has been dispatched to all enabled APs.

**/
RETURN_STATUS
EFIAPI
MultiProcessorExecute (
  IN AP_RUN_TYPE                  ApRunType,
  IN UINT32                       Package,
  IN UINT32                       Core,
  IN BOOLEAN                      SingleThread,
  IN AP_PROCEDURE                 ApProcedure,
  IN OUT VOID                     *ProcArguments
  )
{
  EFI_STATUS                      Status;
  UINT32                          CpuNum;
  UINT32                          Index;
  UINT32                          ExpectedCpuCount;
  CALLBACK_WRAPPER                CallbackWrapper;

  if (ApRunType >= MaxApRunType) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: ApRunType >= MaxApRunType\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (ApProcedure == NULL) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: ApProcedure is NULL\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  CallbackWrapper.SmmApFunction = ApProcedure;
  CallbackWrapper.ProcArguments = ProcArguments;
  CallbackWrapper.CpuCount = 0;

  CpuNum = GetCpuList (Package, Core);

  ExpectedCpuCount = 0;
  for (Index = 0; Index < CpuNum; Index++) {
    if (CheckApScheduled (Index, ApRunType)) {
      continue;
    }
    Status = gSmst->SmmStartupThisAp (SmmApCallbackWrapper, mCpuInfoList[Index].CpuIndex, &CallbackWrapper);
    if (EFI_ERROR (Status)) {
      continue;
    }
    ExpectedCpuCount++;
    mCpuInfoList[Index].Started = TRUE;
    if (SingleThread) {
      WaitForCpuCount (&CallbackWrapper, ExpectedCpuCount);
    }
  }
  //
  // BSP execute SmmApFunction if scheduled
  //
  if (mBspScheduled) {
    ApProcedure (ProcArguments);
  }

  if (!SingleThread) {
    WaitForCpuCount (&CallbackWrapper, ExpectedCpuCount);
  }

  return EFI_SUCCESS;
}

/**
  Schedule a procedure to run on the specified CPU in blocking mode.

  Note that BSP itself might also execute SmmProcedure if BSP index meet the requirement.
  If CpuIndex > CpuNumber, then ASSERT().
  If ApProcedure is NULL, then ASSERT()

  @param  CpuIndex              Target CPU Index.
  @param  ApProcedure           A pointer to the function to be run on enabled APs of the system.
  @param  ProcArguments         Pointer to the optional parameter of the assigned function.

  @retval EFI_INVALID_PARAMETER    CpuNumber not valid.
  @retval EFI_INVALID_PARAMETER    ApProcedure is NULL.
  @retval EFI_INVALID_PARAMETER    The AP specified by CpuNumber did not enter SMM
  @retval EFI_INVALID_PARAMETER    The AP specified by CpuNumber is busy
  @retval EFI_SUCCESS              The procedure has been successfully scheduled.

**/
RETURN_STATUS
EFIAPI
MultiProcessorStartupThisThread (
  IN UINTN                        CpuIndex,
  IN AP_PROCEDURE                 ApProcedure,
  IN OUT VOID                     *ProcArguments
  )
{
  EFI_STATUS                      Status;
  UINT32                          ExpectedCpuCount;
  CALLBACK_WRAPPER                CallbackWrapper;

  if (CpuIndex >= gSmst->NumberOfCpus) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: CpuIndex >= Cpu number\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (ApProcedure == NULL) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: ApProcedure is NULL\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  CallbackWrapper.SmmApFunction = ApProcedure;
  CallbackWrapper.ProcArguments = ProcArguments;
  CallbackWrapper.CpuCount = 0;
  ExpectedCpuCount = 1;

  if (CpuIndex != gSmst->CurrentlyExecutingCpu) {
    Status = gSmst->SmmStartupThisAp (SmmApCallbackWrapper, CpuIndex, &CallbackWrapper);
    if (EFI_ERROR (Status)) {
      return EFI_INVALID_PARAMETER;
    }
    WaitForCpuCount (&CallbackWrapper, ExpectedCpuCount);
  } else {
    ApProcedure (ProcArguments);
  }

  return EFI_SUCCESS;
}

/**
  Return current CPU logic index and optionally get it location. This function can be invoked by either BSP or AP

  @param CpuInfo   A pointer to the buffer where information for the requested processor is deposited

  @return current ProcessorNumber

**/
UINTN
EFIAPI
GetProcessorInfo (
  OUT MP_CPU_INFO  *CpuInfo OPTIONAL
  )
{
  EFI_STATUS                  Status;
  UINTN                       ProcessorNumber;
  EFI_PROCESSOR_INFORMATION   ProcessorInfoBuffer;

  Status = mSmmCpuServiceProtocolInterface->WhoAmI (mSmmCpuServiceProtocolInterface, &ProcessorNumber);
  ASSERT_EFI_ERROR (Status);

  if (CpuInfo != NULL) {
    Status = mSmmCpuServiceProtocolInterface->GetProcessorInfo (mSmmCpuServiceProtocolInterface, ProcessorNumber, &ProcessorInfoBuffer);
    ASSERT_EFI_ERROR (Status);

    CpuInfo->ApicId = (UINT32) ProcessorInfoBuffer.ProcessorId;
    CpuInfo->Package = ProcessorInfoBuffer.Location.Package;
    CpuInfo->Core = ProcessorInfoBuffer.Location.Core;
    CpuInfo->Thread = ProcessorInfoBuffer.Location.Thread;
  }

  return ProcessorNumber;
}
