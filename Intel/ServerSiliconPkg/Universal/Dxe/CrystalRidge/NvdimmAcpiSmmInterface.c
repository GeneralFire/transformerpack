/** @file NvdimmAcpiSmmInterface.c
  Implementation of NVDIMM ACPI SMM Interface

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/NvdimmCommonLib.h>
#include "CrystalRidge.h"

EFI_GUID mEfiNvdimmAcpiSmmInterfaceGuid = CR_ACPI_SMM_INTERFACE_UEFI_VARIABLE_GUID;


/**
  Initializes interface between ACPI and SMM

**/
EFI_STATUS
InitializeNvdimmAcpiSmmInterface (
  VOID
  )
{
  EFI_PHYSICAL_ADDRESS  NvdimmAcpiSmmInterface = 0xFFFFFFFF;
  UINT32                Attributes;
  UINTN                 DataSize;
  EFI_STATUS            Status;

  // To share mNvdimmAcpiSmmInterface address value
  // between DXE and SMM driver we use UEFI VARIABLE mechanism.
  DataSize = sizeof(mNvdimmAcpiSmmInterface);
  Status = gRT->GetVariable (
                  CR_ACPI_SMM_INTERFACE_UEFI_VARIABLE_NAME,
                  &mEfiNvdimmAcpiSmmInterfaceGuid,
                  NULL,
                  &DataSize,
                  &mNvdimmAcpiSmmInterface
                  );

  if(Status == EFI_SUCCESS){
    return Status;
  }

  Status = gBS->AllocatePages(
    AllocateMaxAddress,
    EfiACPIMemoryNVS,
    EFI_SIZE_TO_PAGES(sizeof(NVDIMM_ACPI_SMM_INTERFACE)),
    &NvdimmAcpiSmmInterface
    );

  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "[NVDIMM] ERROR: ACPI<->SMM memory allocation failed with status (%r)\n", Status));
    return Status;
  }

  mNvdimmAcpiSmmInterface = (NVDIMM_ACPI_SMM_INTERFACE *)NvdimmAcpiSmmInterface;
  ZeroMem (mNvdimmAcpiSmmInterface, sizeof(NVDIMM_ACPI_SMM_INTERFACE));

  Attributes = EFI_VARIABLE_BOOTSERVICE_ACCESS |
               EFI_VARIABLE_RUNTIME_ACCESS;
  DataSize = sizeof(mNvdimmAcpiSmmInterface);
  Status = gRT->SetVariable (
                  CR_ACPI_SMM_INTERFACE_UEFI_VARIABLE_NAME,
                  &mEfiNvdimmAcpiSmmInterfaceGuid,
                  Attributes,
                  DataSize,
                  &mNvdimmAcpiSmmInterface
                  );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
