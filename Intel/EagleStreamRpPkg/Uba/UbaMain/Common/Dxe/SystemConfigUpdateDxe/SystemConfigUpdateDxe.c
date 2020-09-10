/** @file
  System Congfig Update.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018-2020 Intel Corporation. <BR>

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

#include <Guid/PlatformInfo.h> // for EFI_PLATFORM_INFO
#include <Guid/SetupVariable.h> // for SYSTEM_CONFIGURATION
#include <Library/UbaSystemConfigUpdateLib.h> // for SYSTEM_CONFIG_UPDATE_SIGNATURE
#include <Uefi/UefiBaseType.h> // for EFI_STATUS
#include <Protocol/UbaCfgDb.h> // for UBA_CONFIG_DATABASE_PROTOCOL
#include <Library/UefiBootServicesTableLib.h> // for gBS
#include <Library/HobLib.h> // for GetFirstGuidHob()
#include <Library/DebugLib.h> // for ASSERT()


EFI_PLATFORM_INFO        *mPlatformInfo = NULL;

/**

    Update the IioDefaults

    @param *SYSTEM_CONFIGURATION   Pointer to the SystemConfiguration structure

    @retval None

**/
VOID
IioDefaultConfigUpdateCallback (
  IN  SYSTEM_CONFIGURATION       *Default
  )
{
  Default->PlatformOCSupport = 0;
}

SYSTEM_CONFIG_UPDATE_DATA SystemConfigUpdateTable = {
    SYSTEM_CONFIG_UPDATE_SIGNATURE,
    SYSTEM_CONFIG_UPDATE_VERSION,
    IioDefaultConfigUpdateCallback
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
SystemConfigUpdateEntry (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
)
{
  EFI_STATUS                                Status;
  UBA_CONFIG_DATABASE_PROTOCOL              *UbaConfigProtocol = NULL;
  EFI_HOB_GUID_TYPE                         *GuidHob;

  GuidHob = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  ASSERT (GuidHob != NULL);
  mPlatformInfo  = GET_GUID_HOB_DATA(GuidHob);

  DEBUG((EFI_D_ERROR, "UBA:System Config Update Table.\n"));
  Status = gBS->LocateProtocol (
                  &gUbaConfigDatabaseProtocolGuid,
                  NULL,
                  &UbaConfigProtocol
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = UbaConfigProtocol->AddData (
                                     UbaConfigProtocol,
                                     &gSystemConfigUpdateDataGuid,
                                     &SystemConfigUpdateTable,
                                     sizeof(SystemConfigUpdateTable)
                                     );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  return Status;
}
