/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2016 Intel Corporation. <BR>

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

#include "BootMode.h"

EFI_STATUS
EFIAPI
UpdateBootMode (
  IN CONST EFI_PEI_SERVICES **PeiServices
)
/*++

Routine Description:

  Get the Boot Mode from the boot loader passed structure.

Arguments:

  PeiServices:      PEI Services Table.

Returns:

  !EFI_SUCCESS      - if any failure
  EFI_SUCCESS       - Successful

--*/
{
  EFI_STATUS    Status;
  EFI_BOOT_MODE BootMode;
  CHAR16        *strBootMode;
  FSPM_UPD 		*FspmUpd;

  //
  // Get BootMode from boot loader
  //
  FspmUpd = (FSPM_UPD *)GetFspMemoryInitUpdDataPointer ();
  BootMode = FspmUpd->FspmArchUpd.BootMode;

  //
  //Detect MANUFACTURING MODE
  //
  switch (BootMode) {
    case BOOT_WITH_FULL_CONFIGURATION:
      strBootMode = L"BOOT_WITH_FULL_CONFIGURATION";
      break;
    case BOOT_WITH_MINIMAL_CONFIGURATION:
      strBootMode = L"BOOT_WITH_MINIMAL_CONFIGURATION";
      break;
    case BOOT_ASSUMING_NO_CONFIGURATION_CHANGES:
      strBootMode = L"BOOT_ASSUMING_NO_CONFIGURATION_CHANGES";
      break;
    case BOOT_WITH_FULL_CONFIGURATION_PLUS_DIAGNOSTICS:
      strBootMode = L"BOOT_WITH_FULL_CONFIGURATION_PLUS_DIAGNOSTICS";
      break;
    case BOOT_WITH_DEFAULT_SETTINGS:
      strBootMode = L"BOOT_WITH_DEFAULT_SETTINGS";
      break;
    case BOOT_ON_S4_RESUME:
      strBootMode = L"BOOT_ON_S4_RESUME";
      break;
    case BOOT_ON_S5_RESUME:
      strBootMode = L"BOOT_ON_S5_RESUME";
      break;
    case BOOT_ON_S2_RESUME:
      strBootMode = L"BOOT_ON_S2_RESUME";
      break;
    case BOOT_ON_S3_RESUME:
      strBootMode = L"BOOT_ON_S3_RESUME";
      break;
    case BOOT_ON_FLASH_UPDATE:
      strBootMode = L"BOOT_ON_FLASH_UPDATE";
      break;
    case BOOT_IN_RECOVERY_MODE:
      strBootMode = L"BOOT_IN_RECOVERY_MODE";
      break;
    default:
      strBootMode = L"Unknown boot mode";
  } // switch (BootMode)
  DEBUG ((EFI_D_INFO, "Setting BootMode to %s\n", strBootMode));


  Status = PeiServicesSetBootMode (BootMode);
  ASSERT_EFI_ERROR (Status);

  return Status;
}
