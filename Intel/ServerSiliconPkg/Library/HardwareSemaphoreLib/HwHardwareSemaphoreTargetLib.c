/** @file
  HwHardwareSempahoreLib.c

  Hardware Target Lib implementation for the Hardware Semaphore Library.  Contains
  functions to acquire and release a socket's semaphore - for use on hardware.

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

#include <Library/SemaphoreLib.h>
#include <Library/SystemInfoLib.h>

/**
  Attempt to acquire the specified socket's semaphore.

  If not acquired immediately, wait until semaphore is acquired before
  proceeding.

  @param[in] Socket            - Socket to acquire semaphore
  @param[in] SysSemaphoreNum   - SYSTEMSEMAPHORE register number (0 or 1)

  @retval EFI_SUCCESS  - successfully acquired semaphore
**/
EFI_STATUS
AcquireSemaphoreControl (
  IN UINT8                    Socket,
  IN SYSTEM_SEMAPHORE_NUMBER  SysSemaphoreNum
  )
{
  BOOLEAN   Acquired;
  UINT32    QueueNumber;

  if (FeaturePcdGet (PcdHardwareLocalSemaphores)) {
    Acquired = AcquireLocalSemaphore (LocalSemaphore0, ADD_TO_QUEUE, &QueueNumber);
    while (!Acquired) {
      Acquired = IsLocalSemaphoreOwned (LocalSemaphore0, QueueNumber);
    }
  }

  Acquired = AcquireSocketSemaphore (Socket, SysSemaphoreNum, ADD_TO_QUEUE, &QueueNumber);
  while (!Acquired) {
    Acquired = IsSocketSemaphoreOwned (Socket, SysSemaphoreNum, QueueNumber);
  }

  return EFI_SUCCESS;
}

/**
  Release the specified socket's semaphore.

  @param[in] Socket            - Socket to acquire semaphore
  @param[in] SysSemaphoreNum   - SYSTEMSEMAPHORE register number (0 or 1)

  @retval EFI_SUCCESS  - successfully released semaphore
**/
EFI_STATUS
ReleaseSemaphoreControl (
  IN UINT8                    Socket,
  IN SYSTEM_SEMAPHORE_NUMBER  SysSemaphoreNum
  )
{
  EFI_STATUS  Status;
  EFI_STATUS  ReturnStatus;

  ReturnStatus = EFI_SUCCESS;

  Status = ReleaseSocketSemaphore (Socket, SysSemaphoreNum);
  if (EFI_ERROR (Status)) {
    ReturnStatus = Status;
  }

  if (FeaturePcdGet (PcdHardwareLocalSemaphores)) {
    Status = ReleaseLocalSemaphore (LocalSemaphore0);
    if (EFI_ERROR (Status)) {
      ReturnStatus = Status;
    }
  }

  return ReturnStatus;
}

/**
  Acquire the semaphore for general use based on the build target.

  @retval EFI_SUCCESS       - successfully acquired semaphore
  @retval EFI_DEVICE_ERROR  - error acquiring semaphore
**/
EFI_STATUS
EFIAPI
AcquireGenSemaphoreControl (
  VOID
  )
{
  EFI_STATUS  Status;
  UINT8       BspSocket;

  BspSocket = GetSysSbspSocketIdNv ();

  Status = AcquireSemaphoreControl (BspSocket, SystemSemaphore1);

  return Status;
}

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
  )
{
  EFI_STATUS  Status;

  Status = AcquireSemaphoreControl (Socket, SystemSemaphore0);

  return Status;
}

/**
  Release the semaphore for general use based on the build target.

  @retval EFI_SUCCESS  - successfully released semaphore
**/
EFI_STATUS
EFIAPI
ReleaseGenSemaphoreControl (
  VOID
  )
{
  ReleaseSemaphoreControl (0, SystemSemaphore1);

  return EFI_SUCCESS;
}

/**
  Release the semaphore for the SMBus based on the build target.

  @param[in] Socket            - Socket to acquire semaphore

  @retval EFI_SUCCESS  - successfully released semaphore
**/
EFI_STATUS
EFIAPI
ReleaseSmbusSemaphoreControl (
  IN UINT8                    Socket
  )
{
  ReleaseSemaphoreControl (Socket, SystemSemaphore0);

  return EFI_SUCCESS;
}

