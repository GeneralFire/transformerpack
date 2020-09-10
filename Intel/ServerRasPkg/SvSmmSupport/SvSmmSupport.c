/** @file
  SV SMM Driver implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2008 - 2018 Intel Corporation. <BR>

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

#include "SvSmmSupport.h"
#include <Protocol/SvSmmProtocol.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/RasDebugLib.h>
#include <Library/UefiLib.h>


EFI_SV_SERVICE_PROTOCOL mSvSmmService = {
  {
    0,
    NULL,
  },
  {
    GetSvosGasAddress,
    SetSvosGasAddress,
    GetSvosSmiDoorBell,
    SetSvosSmiDoorBell,
  }
};

SV_COMMUNICATION_DATA mSvCommData;

//
//Intel_IP_Start
//
/**

    Smm support driver entry point

    @param ImageHandle   EFI_HANDLE: A handle for the image that is initializing this driver
    @param SystemTable   EFI_SYSTEM_TABLE: A pointer to the EFI system table

    @retval EFI_SUCCESS:              Driver initialized successfully
    @retval EFI_UNSUPPORTED:           Failed to Install interface

**/
EFI_STATUS
EFIAPI
InitSvSmmSupport (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_HANDLE mHandle = NULL;

  Status = gBS->InstallMultipleProtocolInterfaces (&mHandle, &gEfiSvSmmProtocolGuid, &mSvSmmService, NULL);
  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "Error, failed to install SvSmmProtocol: %x\n", Status));
    return EFI_UNSUPPORTED;
  }
  return Status;
}

/**

    Give access to the acpi gas address

    @param Address - Pointer to address that will be populated

    @retval None

**/
VOID
EFIAPI
GetSvosGasAddress (
  OUT UINTN                *Address
  )
{
  *Address = mSvCommData.AcpiData.AcpiGasAddress;
}

/**

    Set ACPI gas address

    @param Address - Pointer to address that will be populated

    @retval None

**/
VOID
EFIAPI
SetSvosGasAddress (
  IN UINTN                  Address
  )
{
  mSvCommData.AcpiData.AcpiGasAddress = Address;
}

/**

    Set acpi smidoorbell

    @param SmiDoorBell - Pointer to address that is used to set acpi smidoorbell

    @retval None

**/
VOID
EFIAPI
GetSvosSmiDoorBell (
  OUT UINT8                 *SmiDoorBell
  )
{
  *SmiDoorBell = mSvCommData.AcpiData.SmiDoorBell;
}

/**

    Set acpi smidoorbell

    @param SmiDoorBell - Pointer to address that is used to set acpi smidoorbell

    @retval None

**/
VOID
EFIAPI
SetSvosSmiDoorBell (
  IN UINT8                   SmiDoorBell
  )
{
  mSvCommData.AcpiData.SmiDoorBell = SmiDoorBell;
}

//
//Intel_IP_End
//
