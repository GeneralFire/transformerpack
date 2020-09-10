/** @file
  Smbios Rp Table Driver file.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2020 Intel Corporation. <BR>

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

#include <IndustryStandard/SmBios.h>
#include <Protocol/Smbios.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Guid/MdeModuleHii.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HiiLib.h>
#include <Library/UefiLib.h>
#include <Library/PcdLib.h>
#include <OemSmbios.h>


EFI_STATUS
AddOemSmbiosType133 (
  IN  EFI_SMBIOS_PROTOCOL  *Smbios
  )
{
  UINT32                     ReservedMemorySize;
  EFI_PHYSICAL_ADDRESS       AllocatedAddress;
  OEM_SMBIOS_TABLE_TYPE_133  *Type133;
  EFI_STATUS                 Status;
  EFI_SMBIOS_HANDLE          Handle;
  
  ReservedMemorySize = 0x4000;
  AllocatedAddress = SIZE_4GB - 1;
  Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  EfiACPIMemoryNVS,
                  EFI_SIZE_TO_PAGES (ReservedMemorySize),
                  &AllocatedAddress
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to allocate ACPI NVS memory as BIOS utility communication buffer, Status %r\n", Status));
    return Status;
  }
  DEBUG ((EFI_D_ERROR, "Allocated Communication Buffer address = %x\n", AllocatedAddress));

  Type133 = AllocateZeroPool (sizeof (OEM_SMBIOS_TABLE_TYPE_133) + 1 + 1);
  if (Type133 == NULL) {
    DEBUG ((EFI_D_ERROR, "Failed to allocate smbios record\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  Handle = SMBIOS_HANDLE_PI_RESERVED;
  Type133->Hdr.Type = OEM_SMBIOS_TYPE_UTILITY_COMMUNICATION;
  Type133->Hdr.Length = (UINT8) sizeof (*Type133);
  Type133->Hdr.Handle = 0;

  Type133->Address = (UINT32) AllocatedAddress;
  Type133->Size = ReservedMemorySize;

  Status = Smbios->Add (Smbios, NULL, &Handle, (EFI_SMBIOS_TABLE_HEADER *) Type133);
  DEBUG ((EFI_D_ERROR, "Smbios protocol addition (Type 133) returns %r\n", Status));

  return Status;
}


EFI_STATUS
EFIAPI
SmbiosRpTableEntryPoint(
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS           Status;
  EFI_SMBIOS_PROTOCOL  *Smbios;

  Status = gBS->LocateProtocol(&gEfiSmbiosProtocolGuid, NULL, (VOID**)&Smbios);

  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "SmbiosRpTable Could not locate SMBIOS protocol.%r\n", Status));
    return Status;
  }

  AddOemSmbiosType133 (Smbios);

  return Status;
}
