/** @file
  DXE/SMM implementation of the SemaphoreLib for 10nm.

  This implementation for 10nm allows a calling agent to acquire a global (the
  SBSP) semaphore or a socket's semaphore.  It also provides functionality to
  release the semaphore and check if ownership has been obtained.  If a semaphore
  is desired, an agent should first attempt to acquire it, then check if it has
  ownership.  If ownership has not been obtained, the agent must wait until
  ownership has been obtained before proceeding.  Once the desired task is complete
  the semaphore must be released.  Semaphores should be used for when ensuring
  exclusive access to resoruces among CPU sockets is necessary.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include <PiDxe.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include "HardwareSemaphoreLib10nm.h"

PRIVATE_SEMAPHORE_DATA mPrivateSemaphoreDataDxe;
BOOLEAN mPrivateSemaphoreDataInitialized = FALSE;

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
  )
{
  //
  // If the data hasn't been initialized, do it first.
  //
  if (!mPrivateSemaphoreDataInitialized) {
    //
    // Zeros indicate no current owner.
    //
    ZeroMem (&mPrivateSemaphoreDataDxe, sizeof (mPrivateSemaphoreDataDxe));
    mPrivateSemaphoreDataInitialized = TRUE;
  }

  //
  // Return a pointer to our local data.
  //
  *Data = &mPrivateSemaphoreDataDxe;
  return EFI_SUCCESS;
}

/**
  Report a semaphore owner error.

  Errors are reported differently for PEI versus DXE/SMM. Common code calls this
  function to report when a semaphore isn't owned that should be.
**/
VOID
SemaphoreOwnerError (
  VOID
  )
{
  ASSERT (FALSE);
}

/**
  Report a semaphore depth error.

  Errors are reported differently for PEI versus DXE/SMM. Common code calls this
  function to report when a semaphore's call depth rolls over to zero.
**/
VOID
SemaphoreDepthError (
  VOID
  )
{
  ASSERT (FALSE);
}
