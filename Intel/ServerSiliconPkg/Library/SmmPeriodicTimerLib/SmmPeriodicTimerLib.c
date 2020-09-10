/** @file
  Security Revision Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2017 Intel Corporation. <BR>

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
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SmmServicesTableLib.h>


EFI_SMM_PERIODIC_TIMER_DISPATCH2_PROTOCOL  *gSmmPeriodicTimerDispatchPtr = NULL;

EFI_SMM_PERIODIC_TIMER_REGISTER_CONTEXT     gPeriodicTimer;

EFI_SMM_HANDLER_ENTRY_POINT2                gDispatchFunction = NULL;

EFI_HANDLE                                  gPeriodicTimerHandle = NULL;

/**
  Sets periodic SMM timer.

  This function initialize periodic SMM timer so that it can be used with StartSmmgPeriodicTimer and
  EndSmmgPeriodicTimer.

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

  RASDEBUG ((DEBUG_INFO, "SetSmmgPeriodicTimer ()\n"));

  gPeriodicTimer.Period          = Period;
  gPeriodicTimer.SmiTickInterval = Interval;
  gDispatchFunction              = DispatchFunction;

  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmPeriodicTimerDispatch2ProtocolGuid,
                    NULL,
                    &gSmmPeriodicTimerDispatchPtr
                    );
  ASSERT_EFI_ERROR (Status);

  RASDEBUG ((DEBUG_INFO, "Register SMM Periodic Timer Period %lx, Interval %lx, Status=%r\n", Period, Interval, Status));

  return Status;
}

/**
  Starts periodic SMM timer.

  This function starts periodic SMM timer initialized with SetSmmgPeriodicTimer ()

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

  RASDEBUG ((DEBUG_INFO, "StartSmmgPeriodicTimer ()\n"));

  //
  // Ensure we have set SMM periodic timer before
  //
  ASSERT (gSmmPeriodicTimerDispatchPtr != NULL);

  if (gPeriodicTimerHandle != NULL) {
    RASDEBUG ((DEBUG_INFO, "SMM periodic timer has already started!\n"));
    return EFI_SUCCESS;
  }
  Status = gSmmPeriodicTimerDispatchPtr->Register (
                                         gSmmPeriodicTimerDispatchPtr,
                                         gDispatchFunction,
                                         &gPeriodicTimer,
                                         &gPeriodicTimerHandle
                                         );
  RASDEBUG ((DEBUG_INFO, "Start SMM periodic Timer, status=%r\n", Status));
  return Status;
}

/**
  Ends periodic SMM timer.

  This function ends periodic SMM timer initialized with SetSmmgPeriodicTimer ()

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

  RASDEBUG ((DEBUG_INFO, "EndSmmgPeriodicTimer ()\n"));

  if (gPeriodicTimerHandle == NULL) {
    RASDEBUG ((DEBUG_INFO, "SMM periodic timer has not yet started!\n"));
    return EFI_SUCCESS;
  }

  Status = gSmmPeriodicTimerDispatchPtr->UnRegister (gSmmPeriodicTimerDispatchPtr, gPeriodicTimerHandle);
  RASDEBUG ((DEBUG_INFO, "End SMM periodic timer, status=%r\n", Status));
  gPeriodicTimerHandle  = NULL;

  return Status;
}
