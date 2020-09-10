/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2017 Intel Corporation. <BR>

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

#ifndef _PCH_PERIODIC_TIMER_LIB_H_
#define _PCH_PERIODIC_TIMER_LIB_H_


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
  );

/**
  Starts periodic SMM timer.

  This function starts periodic SMM timer initialized with SetSmmPeriodicTimer ()

  @retval EFI_SUCCESS           SMM periodic timer is successfully started.
  @return others                Some error occurs when starting periodic SMM timer.

**/
EFI_STATUS
EFIAPI
StartSmmPeriodicTimer (
  VOID
  );


/**
  Ends periodic SMM timer.

  This function ends periodic SMM timer initialized with SetSmmPeriodicTimer ()

  @retval EFI_SUCCESS         SMM periodic timer is successfully ended.
  @return others              Some error occurs when ending periodic SMM timer.

**/
EFI_STATUS
EFIAPI
EndSmmPeriodicTimer (
  VOID
  );

#endif // _PCH_PERIODIC_TIMER_LIB_H_
