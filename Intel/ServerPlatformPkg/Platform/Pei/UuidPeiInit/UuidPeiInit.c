/** @file
  Get Uuid value and build a hob for Uuid.
  If in MFG mode and get variable MfgSystemUuid successfully, then write UUID into PDR.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2018 Intel Corporation. <BR>

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

#include <PiPei.h>
#include <Library/PeiPdrAccessLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Guid/HobList.h>

extern EFI_GUID gEfiMfgUUIDHobGuid;

/**
  Build UUID Hob, if get a new UUID variable in MFG Mode, write it to PDR.

  @param[in] FileHandle     Pointer to the PEIM FFS file header.
  @param[in] PeiServices    General purpose services available to every PEIM.

  @retval EFI_SUCCESS   Operation completed successfully.
  @retval Otherwise     Get UUID or Build hob failed.
**/
EFI_STATUS
EFIAPI
UuidInitEntry (
  IN  EFI_PEI_FILE_HANDLE     FileHandle,
  IN  CONST EFI_PEI_SERVICES  **PeiServices
  )
{
  EFI_STATUS     Status;
  EFI_BOOT_MODE  BootMode;
  EFI_GUID       SystemUuid;

  BootMode = BOOT_WITH_FULL_CONFIGURATION;
  Status   = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  if (BootMode == BOOT_WITH_MFG_MODE_SETTINGS) {
    Status = MfgUuidGet ((EFI_PEI_SERVICES**)PeiServices, &SystemUuid);
    DEBUG((EFI_D_INFO, "MfgUuidGet return Status:%r\n", Status));
  } else {
    Status = UuidGetByPdr ((EFI_PEI_SERVICES**)PeiServices, &SystemUuid);
    DEBUG((EFI_D_INFO, "UuidGetByPdr return Status:%r\n", Status));
  }

  //
  //  If there is no storage (eg PreSi), then we must use some initial value (eg 0)
  //  and not call the UuidRefresh function. Otherwise the generated UUID from the
  //  Refesh function will fail to write to storage and we end up in a situation where
  //  a different value for the UUID is generated (and stored in HOB) on every boot.
  //
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "Get UUID Error:  Setting UUID HOB to default value.\n"));
    ZeroMem (&SystemUuid, sizeof (SystemUuid));
  } else {
    //
    // If NOT in an error state, then check value of the UUID
    // For RP PXD PDK test, the value of UUID cannot be all 0's or F's
    //
    // An ifdef is not a good way to handle this case, since it silently changes
    // the value of a user defined input. Eg, if someone needed UUID of 0 for Mfg
    // or other testing then this would override it and cause potential issues.
    // (better approach would be to trigger this via a Platform Setup Variable)
    Status = RpUuidRefresh (PeiServices, &SystemUuid);
  }

  //
  // Create the HOB with the UUID
  //
  DEBUG((EFI_D_INFO, "UUID HOB Data: %g\n", &SystemUuid));
  BuildGuidDataHob (
    &gEfiMfgUUIDHobGuid,
    &SystemUuid,
    sizeof (SystemUuid)
    );

  return Status;
}
