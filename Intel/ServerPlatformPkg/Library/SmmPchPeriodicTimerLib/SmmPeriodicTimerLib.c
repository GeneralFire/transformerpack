/** @file
  SMM Periodic timer library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2015 Intel Corporation. <BR>

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

#include <Protocol/SmmPeriodicTimerDispatch2.h>
#include <Protocol/PchSmmPeriodicTimerControl.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/SmmServicesTableLib.h>


EFI_SMM_PERIODIC_TIMER_DISPATCH2_PROTOCOL   *mSmmPeriodicTimerDispatch = NULL;

PCH_SMM_PERIODIC_TIMER_CONTROL_PROTOCOL     *mPchSmmPeriodicTimerControl = NULL;

EFI_SMM_PERIODIC_TIMER_REGISTER_CONTEXT     mPeriodicTimer;

EFI_SMM_HANDLER_ENTRY_POINT2                mDispatchFunction = NULL;

EFI_HANDLE                                  mPeriodicTimerHandle = NULL;

BOOLEAN                                     mStarted = FALSE;


/**
  Main entry point for an SMM handler dispatch or communicate-based callback.

  @param[in]     DispatchHandle  The unique handle assigned to this handler by SmiHandlerRegister().
  @param[in]     Context         Points to an optional handler context which was specified when the
                                 handler was registered.
  @param[in,out] CommBuffer      A pointer to a collection of data in memory that will
                                 be conveyed from a non-SMM environment into an SMM environment.
  @param[in,out] CommBufferSize  The size of the CommBuffer.

  @retval EFI_SUCCESS                         The interrupt was handled and quiesced. No other handlers
                                              should still be called.
  @retval EFI_WARN_INTERRUPT_SOURCE_QUIESCED  The interrupt has been quiesced but other handlers should
                                              still be called.
  @retval EFI_WARN_INTERRUPT_SOURCE_PENDING   The interrupt is still pending and other handlers should still
                                              be called.
  @retval EFI_INTERRUPT_PENDING               The interrupt could not be quiesced.
**/
EFI_STATUS
EFIAPI
InternalDispatchFunction (
  IN EFI_HANDLE  DispatchHandle,
  IN CONST VOID  *Context         OPTIONAL,
  IN OUT VOID    *CommBuffer      OPTIONAL,
  IN OUT UINTN   *CommBufferSize  OPTIONAL
  )
{
  if (!mStarted) {
    //
    // In case SMI periodic timer has been launched by other handler
    //
    return EFI_NOT_READY;
  }

  return mDispatchFunction (DispatchHandle, Context, CommBuffer, CommBufferSize);
}


/**
  Sets periodic SMM timer.

  This function initialize periodic SMM timer so that it can be used with StartSmmPeriodicTimer and
  EndSmmPeriodicTimer.

  @param[in] Period             The minimum period of time in 100 nanosecond units that the child gets called. The
                                child will be called back after a time greater than the time Period.
  @param[in] Interval           The period of time interval between SMIs. Children of this interface should use this
                                field when registering for periodic timer intervals when a finer granularity periodic
                                SMI is desired.
  @param[in] DispatchFunction   Function to register for handler when at least the specified amount of time has elapsed.

  @retval EFI_SUCCESS           SMM periodic timer is successfully set.
  @return others                Some error occurs when setting periodic SMM timer.

**/
EFI_STATUS
EFIAPI
SetSmmPeriodicTimer (
  IN UINT64                        Period,
  IN UINT64                        Interval,
  IN EFI_SMM_HANDLER_ENTRY_POINT2  DispatchFunction
  )
{
  EFI_STATUS      Status;

  mPeriodicTimer.Period = Period;
  mPeriodicTimer.SmiTickInterval = Interval;
  mDispatchFunction = DispatchFunction;

  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmPeriodicTimerDispatch2ProtocolGuid,
                    NULL,
                    &mSmmPeriodicTimerDispatch
                    );
  ASSERT_EFI_ERROR (Status);

  Status = gSmst->SmmLocateProtocol (
                    &gPchSmmPeriodicTimerControlGuid,
                    NULL,
                    &mPchSmmPeriodicTimerControl
                    );
  ASSERT_EFI_ERROR (Status);

  DEBUG ((EFI_D_INFO, "Register SMM Periodic Timer Period %lx, Interval %lx, Status=%r\n", Period, Interval, Status));


  Status = mSmmPeriodicTimerDispatch->Register(
                                        mSmmPeriodicTimerDispatch,
                                        InternalDispatchFunction,
                                        &mPeriodicTimer,
                                        &mPeriodicTimerHandle
                                        );
  DEBUG ((EFI_D_INFO, "Start SMM periodic Timer, status=%r\n", Status));
  ASSERT_EFI_ERROR (Status);

  Status = mPchSmmPeriodicTimerControl->Pause (mPchSmmPeriodicTimerControl, mPeriodicTimerHandle);
  DEBUG ((EFI_D_INFO, "Pause SMM Periodic timer returns %r\n", Status));
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Starts periodic SMM timer.

  This function starts periodic SMM timer initialized with SetSmmPeriodicTimer()

  @retval EFI_SUCCESS           SMM periodic timer is successfully started.
  @return others                Some error occurs when starting periodic SMM timer.

**/
EFI_STATUS
EFIAPI
StartSmmPeriodicTimer (
  VOID
  )
{
  EFI_STATUS              Status;

  //
  // Ensure we have set SMM periodic timer before
  //
  ASSERT (mPeriodicTimerHandle != NULL);

  Status = mPchSmmPeriodicTimerControl->Resume (mPchSmmPeriodicTimerControl, mPeriodicTimerHandle);
  DEBUG ((EFI_D_INFO, "Starting SMM Periodic timer returns %r\n", Status));
  mStarted = TRUE;

  return Status;
}


/**
  Ends periodic SMM timer.

  This function ends periodic SMM timer initialized with SetSmmPeriodicTimer()

  @retval EFI_SUCCESS         SMM periodic timer is successfully ended.
  @return others              Some error occurs when ending periodic SMM timer.

**/
EFI_STATUS
EFIAPI
EndSmmPeriodicTimer (
  VOID
  )
{
  EFI_STATUS                  Status;

  mStarted = FALSE;
  Status = mPchSmmPeriodicTimerControl->Pause (mPchSmmPeriodicTimerControl, mPeriodicTimerHandle);
  DEBUG ((EFI_D_INFO, "Ending SMM Periodic timer returns %r\n", Status));

  return Status;
}
