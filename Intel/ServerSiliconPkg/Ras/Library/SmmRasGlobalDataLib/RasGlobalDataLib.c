/** @file
  Implementation of generate RAS global data access for silicon library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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
#include <Protocol/SmmVariable.h>
#include <Protocol/VariableLock.h>
#include <Guid/RasGlobalDataVariable.h>
#include <Library/BaseMemoryLib.h>
#include <Library/RasGlobalDataLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>

//
// Global variable to cache pointer to SMM Variable protocol.
//
static EFI_SMM_VARIABLE_PROTOCOL      *mSmmVariableProtocol = NULL;

//
// Global variable to cache variable lock protocol.
//
EDKII_VARIABLE_LOCK_PROTOCOL   *mVariableLock = NULL;


/**
  The constructor function caches the pointer to SMM Variable protocol.

  The constructor function locates SMM variable protocol from protocol database.
  It will ASSERT() if that operation fails.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_STATUS    - EFI_SUCCESS - Constructor function was successful.
  @retval               - Other       - Constructor function was not successful.

**/
EFI_STATUS
EFIAPI
InitializeSmmRasGlobalData (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
{
  EFI_STATUS  Status;

  Status = gSmst->SmmLocateProtocol (&gEfiSmmVariableProtocolGuid, NULL, (VOID**) &mSmmVariableProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Locate gEfiSmmVariableProtocolGuid failed, Status=%r\n", __FUNCTION__, Status));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }
  if (mSmmVariableProtocol == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: failed to locate mSmmVariableProtocol\n", __FUNCTION__));
    ASSERT (mSmmVariableProtocol != NULL);
    return EFI_NOT_FOUND;
  }

  Status = gBS->LocateProtocol (&gEdkiiVariableLockProtocolGuid, NULL, (VOID **) &mVariableLock);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Locate gEdkiiVariableLockProtocolGuid failed, Status=%r\n", __FUNCTION__, Status));
    ASSERT_EFI_ERROR (Status);
    return  Status;
  }
  if (mVariableLock == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: failed to locate mVariableLock", __FUNCTION__));
    ASSERT (mVariableLock != NULL);
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}


/**

  Get Ras global data pointer.

  This function implements single design pattern to return the system global data for RAS usage.
  It is required for the caller of this library API to cache the return value to module global
  variable to in the POST phase.

  @param Name            NULL-terminated Unicode string as the unique key for the global variable.
  @param Size            The size of global data.
  @param NewAllocation   Indicate whether the data is newly allocated or not.

  @return Pointer to the global data or NULL if there is out of resource.

**/
VOID*
EFIAPI
GetRasGlobalData (
  IN  CHAR16         *Name,
  IN  UINTN          Size,
  OUT BOOLEAN        *NewAllocation OPTIONAL
  )
{
  EFI_STATUS         Status;
  VOID               *Data = NULL;
  UINTN              VariableSize;
  BOOLEAN            IsNewAllocation;

  IsNewAllocation = FALSE;

  VariableSize = sizeof (Data);
  Status = mSmmVariableProtocol->SmmGetVariable (
                                   Name,
                                   &gRasGlobalDataVariableGuid,
                                   NULL,
                                   &VariableSize,
                                   &Data
                                   );
  if (!EFI_ERROR (Status)) {
    //
    // The global data has been allocated before, we can reuse the data.
    //
    ASSERT (Data != NULL);
  } else if (Status == EFI_NOT_FOUND) {
    Data = AllocateZeroPool (Size);
    if (Data == NULL) {
      DEBUG ((EFI_D_ERROR, "Failed to allocate SMM pool!\n"));
      goto Done;
    }

    IsNewAllocation = TRUE;
    //
    // Save the global data to ensure single copy.
    //
    Status = mSmmVariableProtocol->SmmSetVariable (
                                     Name,
                                     &gRasGlobalDataVariableGuid,
                                     EFI_VARIABLE_BOOTSERVICE_ACCESS,
                                     sizeof (Data),
                                     &Data
                                     );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: set variable gRasGlobalDataVariableGuid failed, Status=%r\n", __FUNCTION__, Status));
      ASSERT_EFI_ERROR (Status);
    }

    //
    // Lock this variable to prevent attempting to changed by malicious SW.
    //
    Status = mVariableLock->RequestToLock (mVariableLock, Name, &gRasGlobalDataVariableGuid);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: RequestToLock mVariableLock failed, Status=%r\n", __FUNCTION__, Status));
      ASSERT_EFI_ERROR (Status);
    }
  } else {
    Data = NULL;
    ASSERT (FALSE);
  }

Done:
  if (NewAllocation != NULL) {
    *NewAllocation = IsNewAllocation;
  }
  return Data;
}

/**

  Get Ras global data pointer, This function differ from GetRasGlobalData.
  This function allocate memory from EfiReservedMemoryType

  This function implements single design pattern to return the system global data for RAS usage.
  It is required for the caller of this library API to cache the return value to module global
  variable to in the POST phase.

  @param Name            NULL-terminated Unicode string as the unique key for the global variable.
  @param Size            The size of global data.
  @param NewAllocation   Indicate whether the data is newly allocated or not.

  @return Pointer to the global data or NULL if there is out of resource.

**/
VOID*
EFIAPI
GetRasGlobalDataEfiReservedMemory (
  IN  CHAR16         *Name,
  IN  UINTN          Size,
  OUT BOOLEAN        *NewAllocation OPTIONAL
  )
{
  EFI_STATUS         Status;
  VOID               *Data;
  UINTN              VariableSize;
  BOOLEAN            IsNewAllocation;

  IsNewAllocation = FALSE;

  VariableSize = sizeof (Data);
  Status = mSmmVariableProtocol->SmmGetVariable (
                                   Name,
                                   &gRasGlobalDataVariableGuid,
                                   NULL,
                                   &VariableSize,
                                   &Data
                                   );
  if (!EFI_ERROR (Status)) {
    //
    // The global data has been allocated before, we can reuse the data.
    //
    ASSERT (Data != NULL);
  } else if (Status == EFI_NOT_FOUND) {
    Status = gBS->AllocatePool (
      EfiReservedMemoryType,
      Size,
      &Data
      );
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to allocate Reserved Memory pool!\n"));
      goto Done;
    }
    ZeroMem (Data, Size);

    IsNewAllocation = TRUE;
    //
    // Save the global data to ensure single copy.
    //
    Status = mSmmVariableProtocol->SmmSetVariable (
                                     Name,
                                     &gRasGlobalDataVariableGuid,
                                     EFI_VARIABLE_BOOTSERVICE_ACCESS,
                                     sizeof (Data),
                                     &Data
                                     );
    ASSERT_EFI_ERROR (Status);

    //
    // Lock this variable to prevent attempting to changed by malicious SW.
    //
    Status = mVariableLock->RequestToLock (mVariableLock, Name, &gRasGlobalDataVariableGuid);
    ASSERT_EFI_ERROR (Status);
  } else {
    Data = NULL;
    ASSERT (FALSE);
  }

Done:
  if (NewAllocation != NULL) {
    *NewAllocation = IsNewAllocation;
  }
  return Data;
}