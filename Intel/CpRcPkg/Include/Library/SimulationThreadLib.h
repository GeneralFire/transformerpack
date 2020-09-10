/** @file
  SimulationThreadLib.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#ifndef _SIMULATION_THREAD_LIB_H_
#define _SIMULATION_THREAD_LIB_H_

#include <SysHost.h>

/**
  Sleep for Duration Milliseconds.

  @param[in]  Duration  Number of milliseconds to sleep.

  @retval None

**/

VOID
EFIAPI
ThreadSleep (
  IN UINT32 Duration
  );

/**
  Release the Generic Mutex.

  @param  None

  @retval None

**/

VOID
EFIAPI
ThreadReleaseGenMutex (
  VOID
  );

/**
  Acquire the Generic Mutex.

  @param  None

  @retval None

**/

VOID
EFIAPI
ThreadAcquireGenMutex (
  VOID
  );

/**
  Release the Smbus Mutex.

  @param  None

  @retval None

**/

VOID
EFIAPI
ThreadReleaseSmbusMutex (
  VOID
  );

/**
  Acquire the Smbus Mutex.

  @param  None

  @retval None

**/

UINT32
EFIAPI
ThreadAcquireSmbusMutex (
  VOID
  );

/**
  Prepare for reset.

  @param[in]  Host          Ptr to the Host structure

  @retval None

**/

VOID
EFIAPI
PrepareForReset (
  IN VOID *Host
  );

/**
  Release the PBSPs out of reset.

  @param  None

  @retval None

**/

VOID
EFIAPI
ReleasePbsps (
  VOID
  );

/**
  Exit simulation threads.

  @param  None

  @retval None

**/

VOID
EFIAPI
ExitThreads (
  VOID
  );

/**
  Create simulation threads.

  @param  None

  @retval None

**/

VOID
EFIAPI
CreateThreads (
  VOID
  );

#endif // #ifndef _SIMULATION_THREAD_LIB_H_
