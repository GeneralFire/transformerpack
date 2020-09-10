/** @file
  Implementation of Basic IP Block Services Library for PEI.

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


#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
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
  CONST EFI_PEI_SERVICES  **PeiServices;

  PeiServices = GetPeiServicesTablePointer ();
  return (*PeiServices)->InstallPpi (PeiServices, InterfaceList);
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
  CONST EFI_PEI_SERVICES **PeiServices;

  PeiServices = GetPeiServicesTablePointer ();
  return (*PeiServices)->LocatePpi (PeiServices, Guid, Instance, NULL, Interface);
}
