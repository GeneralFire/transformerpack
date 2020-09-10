/** @file
  10nm hardware semaphore library private header file.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

#ifndef _HARDWARE_SEMAPHORE_LIB_10NM_H_
#define _HARDWARE_SEMAPHORE_LIB_10NM_H_

#include <Uefi/UefiBaseType.h>
#include <Library/SemaphoreLib.h>       // *this* library

typedef struct {
  UINT8   Owner;
  UINT8   Depth;
} PRIVATE_SEMAPHORE_TRACKER;

///
/// Private data structure used by this library class implementation.
///
typedef struct {
  PRIVATE_SEMAPHORE_TRACKER System[MAX_SOCKET][SystemSemaphoreMax];
  PRIVATE_SEMAPHORE_TRACKER Local[MAX_SOCKET][LocalSemaphoreMax];
} PRIVATE_SEMAPHORE_DATA;

/**
  Get semaphore data.

  Data is stored differently for PEI versus DXE/SMM. Common code calls this
  function to get it. If the data has not been initialized, this function
  attempts to do so before returning it.

  @param[out] Data              - private data

  @retval EFI_SUCCESS           - successfully returned the data pointer
  @retval Other                 - an error occurred
**/
EFI_STATUS
GetSemaphoreData (
  OUT PRIVATE_SEMAPHORE_DATA  **Data
  );

/**
  Report a semaphore owner error.

  Errors are reported differently for PEI versus DXE/SMM. Common code calls this
  function to report when a semaphore isn't owned that should be.
**/
VOID
SemaphoreOwnerError (
  VOID
  );

/**
  Report a semaphore depth error.

  Errors are reported differently for PEI versus DXE/SMM. Common code calls this
  function to report when a semaphore's call depth rolls over to zero.
**/
VOID
SemaphoreDepthError (
  VOID
  );

#endif // _HARDWARE_SEMAPHORE_LIB_10NM_H_
