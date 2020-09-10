/** @file
  Implementation of Basic IP Block Services Library for SMM drivers.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiPei.h> // Need PPI Descriptor definition only
#include <Uefi.h>
#include <PiSmm.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>

/**
  This service registers an interface into the phase specific database using
  the phase specific service from the UEFI PI Arch spec or UEFI Spec.
  The input follows PPI style descriptors to take a list of GUID/Pointer pairs.

  @param  InterfaceList         A pointer to the list of interfaces that the caller shall install.

  @retval EFI_SUCCESS           The interface was successfully installed.
  @retval EFI_INVALID_PARAMETER The InterfaceList pointer is NULL.
  @retval EFI_INVALID_PARAMETER Any of the PEI PPI descriptors in the list do not have the
                                EFI_PEI_PPI_DESCRIPTOR_PPI bit set in the Flags field.
  @retval EFI_OUT_OF_RESOURCES  There is no additional space in the database.

**/
EFI_STATUS
EFIAPI
InstallInterface (
  IN CONST EFI_PEI_PPI_DESCRIPTOR     *InterfaceList
  )
{
  EFI_HANDLE                Handle = NULL;
  EFI_STATUS                Status  = EFI_SUCCESS;

  if (InterfaceList == NULL) {
    ASSERT (InterfaceList != NULL);
    return EFI_INVALID_PARAMETER;
  }

  //
  // This is loop installs all EFI_PEI_PPI_DESCRIPTOR in the InterfaceList. It is terminated
  // by the EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST being set in the last
  // EFI_PEI_PPI_DESCRIPTOR in the list.
  //

  for (;;) {
    //
    // Check if it is a valid EFI_PEI_PPI_DESCRIPTOR.
    //
    if ((InterfaceList->Flags & EFI_PEI_PPI_DESCRIPTOR_PPI) == 0) {
      return  EFI_INVALID_PARAMETER;
    }

    Handle = NULL;
    Status = gSmst->SmmInstallProtocolInterface (
      &Handle,
      InterfaceList->Guid,
      EFI_NATIVE_INTERFACE,
      InterfaceList->Ppi
      );

    if (EFI_ERROR (Status)) {
      return Status;
    }

    if ((InterfaceList->Flags & EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST) ==
               EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST) {
      //
      // Continue until the end of the Interface List.
      //
      break;
    }
    InterfaceList++;
  }

  return Status;
}

/**
  This service locates an interface in the phase specific database using the
  phase specific service from the UEFI PI Arch spec or UEFI Spec.

  @param  Guid                  A pointer to the GUID whose corresponding interface needs to be
                                found.
  @param  Instance              The N-th instance of the interface that is required.
  @param  Interface             A pointer to the instance of the interface.

  @retval EFI_SUCCESS           The interface was successfully returned.
  @retval EFI_NOT_FOUND         The instance of the interface was not found in the database.

**/
EFI_STATUS
EFIAPI
LocateInterface (
  IN EFI_GUID                     *Guid,
  IN UINTN                        Instance,
  IN OUT VOID                     **Interface
  )
{
  EFI_STATUS                        Status;
  UINTN                             NumOfHandles;
  UINTN                             HandleBufferSize;
  EFI_HANDLE                        *HandleBuffer;

  //
  // Get the list of all handles with target protocol in the handle database.
  //
  HandleBufferSize = 0;
  HandleBuffer = NULL;
  Status = gSmst->SmmLocateHandle (
    ByProtocol,
    Guid,
    NULL,
    &HandleBufferSize,
    HandleBuffer
    );

  if (Status != EFI_BUFFER_TOO_SMALL) {
    return EFI_NOT_FOUND;
  }

  HandleBuffer = AllocateZeroPool (HandleBufferSize);
  if (HandleBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = gSmst->SmmLocateHandle(
    ByProtocol,
    Guid,
    NULL,
    &HandleBufferSize,
    HandleBuffer
    );
  if (EFI_ERROR (Status)) {
      //
      // Free memory if needed
      //
      if (HandleBuffer != NULL) {
        FreePool (HandleBuffer);
      }
      return EFI_NOT_FOUND;
  } else {
    //
    // We have the handle buffer with instances of the interface
    // Check that instance requested exists
    //
    NumOfHandles = HandleBufferSize / sizeof (EFI_HANDLE);
    if (Instance > NumOfHandles) {
      Status = EFI_NOT_FOUND;
    } else {
      //
      // Get the protocol from the instance
      //
      Status = gSmst->SmmHandleProtocol (
        HandleBuffer[Instance],
        Guid,
        (VOID**) Interface
        );
      if (EFI_ERROR (Status)) {
        //
        // We should never find handles and then fail to find on the handle
        //
        ASSERT_EFI_ERROR (Status);
        Status = EFI_NOT_FOUND;
      }
    }
  }

  //
  // Free the buffer containing the list of handles from the handle database
  //
  if (HandleBuffer != NULL) {
    FreePool (HandleBuffer);
  }

  return Status;
}
