/** @file
  This file is for Intel Required UEFI Variables PPI initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 Intel Corporation. <BR>

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

#include <Library/PeiServicesLib.h>
#include <Guid/SetupVariable.h>
#include <Ppi/RequiredVariables.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

//
// This table contains Boot Critical UEFI Variables descriptions (GUID + Variable Name)
//
REQUIRED_VARIABLES_DESCRIPTOR mRequiredUefiVariables [] = {
  {
    SYSTEM_CONFIGURATION_GUID,
    PLATFORM_SETUP_VARIABLE_NAME
  },
  {
    PCH_SETUP_GUID,
    PCH_SETUP_VARIABLE_NAME
  }
};

//
// RequiredUefiVarPeim PEIM entry point
//
/**

  RequiredUefiVarPeim PEIM installs gUefiVariableCreatorInitPpiGuid PPI which describes Boot Critical UEFI Variables.
  PlatformVariableInitPei PEIM locate this PPI and verifies the existence of those UEFI variables.
  If one of those variables is not exist - it should be created and default values must be applied.

  @param ImageHandle  -  Handle for the image of this driver.
  @param SystemTable  -  Pointer to the EFI System Table.

  @retval EFI_SUCCESS       Operation completed successfully.
  @retval Otherwise         PPI Installation failed.
**/
EFI_STATUS
EFIAPI
RequiredUefiVarPeimEntry (
  IN EFI_PEI_FILE_HANDLE          FileHandle,
  IN CONST EFI_PEI_SERVICES       **PeiServices
  )
{
  EFI_STATUS                    Status;
  EFI_PEI_PPI_DESCRIPTOR        *PpiDescriptor;
  REQUIRED_VARIABLES_PPI        *RequiredVariables;
  VOID                          *AllocatedMemory;
  UINT8                         Index;
  UINT8                         Count;
  UINTN                         MemorySize;

  Count = sizeof (mRequiredUefiVariables) / sizeof (REQUIRED_VARIABLES_DESCRIPTOR);

  //
  // Allocate memory to fit EFI_PEI_PPI_DESCRIPTOR + REQUIRED_VARIABLES_PPI { Count + (Count * UefiVariableDescriptor)
  //
  MemorySize = sizeof (*PpiDescriptor) + sizeof (UINT8) + (Count * sizeof (REQUIRED_VARIABLES_DESCRIPTOR));

  AllocatedMemory = AllocateZeroPool (MemorySize);
  if (AllocatedMemory == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Memory for PPI Descriptor and REQUIRED_VARIABLES_PPI is allocated.
  //
  PpiDescriptor = (EFI_PEI_PPI_DESCRIPTOR *) AllocatedMemory;

  //
  // Leave memory window for PPI Descriptor
  //
  RequiredVariables = (REQUIRED_VARIABLES_PPI *) (PpiDescriptor + 1);

  RequiredVariables->Count = Count;
  for (Index = 0; Index < Count; Index++) {
    CopyGuid (&(RequiredVariables->UefiVariables[Index].Guid), &mRequiredUefiVariables[Index].Guid);
    StrCpyS (RequiredVariables->UefiVariables[Index].VariableName, VARIABLE_NAME_LENGTH, mRequiredUefiVariables[Index].VariableName);
  }

  //
  // Specify Ppi Descriptor
  //
  PpiDescriptor->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  PpiDescriptor->Guid  = &gRequiredVariablesPpiGuid;
  PpiDescriptor->Ppi   = RequiredVariables;

  //
  // Initialize platform PPIs
  //
  Status = PeiServicesInstallPpi (PpiDescriptor);
  ASSERT_EFI_ERROR (Status);

  return Status;
}
