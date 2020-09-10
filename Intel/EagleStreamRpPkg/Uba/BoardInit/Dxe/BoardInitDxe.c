/** @file
  BOARD INIT DXE Driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <Protocol/UbaCfgDb.h> // for UBA_CONFIG_DATABASE_PROTOCOL
#include <Library/DebugLib.h>  // for EFI_D_ERROR
#include <PlatformInfoTypes.h>  // for TypeArcherCityModular
#include <Library/UefiBootServicesTableLib.h>  // for gBS
#include <Library/CpuAndRevisionLib.h>  // for IsCpuAndRevision

#include <Protocol/PlatformIncompatiblePciDeviceProtocol.h>

INCOMPATIBLE_PCI_DEVICE mPlatformIncompatiblePciDevice[] = {
  {{ 0x8086, 0x2260, 0xFF, 0xFFFF, 0xFFFF }, { 0, 0, ACPI_ADDRESS_SPACE_TYPE_MEM, 0, 0, 64, 0, EVEN_ALIGN, MAX_UINT64, 0 }} // Intel KNL Cougar Rock (Pcie Co-Processor)
};

INCOMPATIBLE_PCI_DEVICE_PROTOCOL mPlatformIncompatiblePciDeviceProtocol = {
  sizeof(mPlatformIncompatiblePciDevice) / sizeof(INCOMPATIBLE_PCI_DEVICE),
  mPlatformIncompatiblePciDevice
};

INCOMPATIBLE_PCI_DEVICE mPlatformIncompatiblePciDeviceAx[] = {
  {{ 0x8086, 0x2260, 0xFF, 0xFFFF, 0xFFFF }, { 0, 0, ACPI_ADDRESS_SPACE_TYPE_MEM, 0, 0, 64, 0, EVEN_ALIGN, MAX_UINT64, 0         }}, // Intel KNL Cougar Rock (Pcie Co-Processor)
  {{ 0x8086, 0x09A6, 0xFF, 0xFFFF, 0xFFFF }, { 0, 0, ACPI_ADDRESS_SPACE_TYPE_MEM, 0, 0,  0, 0,     0x1FFF, MAX_UINT64, 0x2000    }}, // Device OOB-MSM BAR0
  {{ 0x8086, 0x09A7, 0xFF, 0xFFFF, 0xFFFF }, { 0, 0, ACPI_ADDRESS_SPACE_TYPE_MEM, 0, 0,  0, 0,     0x3FFF,          0, 0x4000    }}, // Device PMON OOB-MSM BAR0
  {{ 0x8086, 0x09A7, 0xFF, 0xFFFF, 0xFFFF }, { 0, 0, ACPI_ADDRESS_SPACE_TYPE_MEM, 0, 0,  0, 0,    0xFFFFF,          1, 0x100000  }}, // Device PMON OOB-MSM BAR1
  {{ 0x8086, 0x09A8, 0xFF, 0xFFFF, 0xFFFF }, { 0, 0, ACPI_ADDRESS_SPACE_TYPE_MEM, 0, 0,  0, 0,    0xFFFFF, MAX_UINT64, 0x100000  }}  // Device DFX OOB-MSM BAR0
};

INCOMPATIBLE_PCI_DEVICE_PROTOCOL mPlatformIncompatiblePciDeviceProtocolAx = {
  sizeof(mPlatformIncompatiblePciDeviceAx) / sizeof(INCOMPATIBLE_PCI_DEVICE),
  mPlatformIncompatiblePciDeviceAx
};

/**
  The Driver Entry Point.

  The function is the driver Entry point.

  @param ImageHandle   A handle for the image that is initializing this driver
  @param SystemTable   A pointer to the EFI system table

  @retval EFI_SUCCESS:              Driver initialized successfully
  @retval EFI_LOAD_ERROR:           Failed to Initialize or has been loaded
  @retval EFI_OUT_OF_RESOURCES      Could not allocate needed resources

**/
EFI_STATUS
EFIAPI
BoardInitDxeDriverEntry (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
)
{
  EFI_STATUS                              Status = EFI_SUCCESS;
  UBA_CONFIG_DATABASE_PROTOCOL           *UbaConfigProtocol = NULL;
  UINT32                                  PlatformType = 0;
  EFI_HANDLE                              Handle = NULL;

  DEBUG((DEBUG_INFO,"UBA DXE: %a\n",__FUNCTION__));

  if (IsCpuAndRevision (CPU_SPRSP, REV_AX)) {
    Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gPlatformIncompatiblePciDeviceProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mPlatformIncompatiblePciDeviceProtocolAx
                    );
  } else {
    Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gPlatformIncompatiblePciDeviceProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mPlatformIncompatiblePciDeviceProtocol
                    );
  }
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to install gPlatformIncompatiblePciDeviceProtocolGuid protocol !!\n"));
  }

  Handle = NULL;
  Status = gBS->LocateProtocol (
                  &gUbaConfigDatabaseProtocolGuid,
                  NULL,
                  &UbaConfigProtocol
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = UbaConfigProtocol->GetSku(
                         UbaConfigProtocol,
                         &PlatformType,
                         NULL,
                         NULL
                         );
  ASSERT_EFI_ERROR (Status);

  DEBUG ((EFI_D_ERROR, "Uba init Dxe driver:PlatformType=%d\n", PlatformType));

  //according to the platform type to install different dummy maker.
  //later, the PEIM will be loaded by the dependency.
  switch(PlatformType)
  {
    case TypeArcherCityRP:
      Status = gBS->InstallProtocolInterface (
            &Handle,
            &gEfiPlatformTypeArcherCityRPProtocolGuid,
            EFI_NATIVE_INTERFACE,
            NULL
            );
      ASSERT_EFI_ERROR (Status);
      break;

    case TypeArcherCityModular:
      Status = gBS->InstallProtocolInterface(
            &Handle,
            &gEfiPlatformTypeArcherCityModularProtocolGuid,
            EFI_NATIVE_INTERFACE,
            NULL
      );
      ASSERT_EFI_ERROR(Status);
      break;

    case TypeArcherCityEVB:
      Status = gBS->InstallProtocolInterface (
            &Handle,
            &gEfiPlatformTypeArcherCityEVBProtocolGuid,
            EFI_NATIVE_INTERFACE,
            NULL
            );
      ASSERT_EFI_ERROR (Status);
      break;

    case TypeArcherCityXPV:
      Status = gBS->InstallProtocolInterface (
            &Handle,
            &gEfiPlatformTypeArcherCityXPVProtocolGuid,
            EFI_NATIVE_INTERFACE,
            NULL
            );
      ASSERT_EFI_ERROR (Status);
      break;

    case TypeBigPineKey:
      Status = gBS->InstallProtocolInterface (
            &Handle,
            &gEfiPlatformTypeBigPineKeyProtocolGuid,
            EFI_NATIVE_INTERFACE,
            NULL
            );
      ASSERT_EFI_ERROR (Status);
      break;

    default:
      DEBUG ((EFI_D_ERROR, "There was not board type found for this board.\n"));
      // CAN'T GO TO HERE.
      ASSERT_EFI_ERROR(FALSE);
  }

  return Status;
}
