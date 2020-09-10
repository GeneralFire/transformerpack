/** @file
  HardwareSempahoreTargetLib.h

  Target library API function header for HardwareSemaphoreLib.  This API provides definitions
  for functions related to the Hardware Semaphore Library. These functions provide specific
  functionality in a simulated/hardware environment.

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

#ifndef _HARDWARE_SEMAPHORE_TARGET_LIB_H_
#define _HARDWARE_SEMAPHORE_TARGET_LIB_H_


/**
  Acquire the semaphore for general use based on the build target.

  @retval EFI_SUCCESS       - successfully acquired semaphore
  @retval EFI_DEVICE_ERROR  - error acquiring semaphore
**/
EFI_STATUS
EFIAPI
AcquireGenSemaphoreControl (
  VOID
  );

/**
  Acquire the semaphore for the SMBus based on the build target.

  @param[in] Socket            - Socket to acquire semaphore

  @retval EFI_SUCCESS       - successfully acquired semaphore
  @retval EFI_DEVICE_ERROR  - error acquiring semaphore
**/
EFI_STATUS
EFIAPI
AcquireSmbusSemaphoreControl (
  IN UINT8                    Socket
  );

/**
  Release the semaphore for general use based on the build target.

  @retval EFI_SUCCESS  - successfully released semaphore
**/
EFI_STATUS
EFIAPI
ReleaseGenSemaphoreControl (
  VOID
  );

/**
  Release the semaphore for the SMBus based on the build target.

  @param[in] Socket            - Socket to acquire semaphore

  @retval EFI_SUCCESS  - successfully released semaphore
**/
EFI_STATUS
EFIAPI
ReleaseSmbusSemaphoreControl (
  IN UINT8                    Socket
  );

#endif // _HARDWARE_SEMAPHORE_TARGET_LIB_H_

