/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2018 Intel Corporation. <BR>

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

#include <Base.h>
#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>

#include <Library/UbaSystemConfigUpdateLib.h>
#include <Protocol/UbaCfgDb.h>

EFI_STATUS
UpdateIioDefaultConfig (
  IN  SYSTEM_CONFIGURATION       *Default
  )
{
  EFI_STATUS                        Status;
  UBA_CONFIG_DATABASE_PROTOCOL      *UbaConfigProtocol = NULL;
  UINTN                             DataLength = 0;
  SYSTEM_CONFIG_UPDATE_DATA         SystemConfigUpdateTable;

  Status = gBS->LocateProtocol (
                  &gUbaConfigDatabaseProtocolGuid,
                  NULL,
                  &UbaConfigProtocol
                  );
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR," [UpdateIioDefaultConfig] Locate UbaConfigProtocol fail!\n"));
    return Status;
  }

  DataLength = sizeof(SystemConfigUpdateTable);
  Status = UbaConfigProtocol->GetData (
                                   UbaConfigProtocol,
                                   &gSystemConfigUpdateDataGuid,
                                   &SystemConfigUpdateTable,
                                   &DataLength
                                   );

  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR," [UpdateIioDefaultConfig] Get Data fail!\n"));
    return Status;
  }

  ASSERT (SystemConfigUpdateTable.Signature == SYSTEM_CONFIG_UPDATE_SIGNATURE);
  ASSERT (SystemConfigUpdateTable.Version   == SYSTEM_CONFIG_UPDATE_VERSION);

  SystemConfigUpdateTable.CallUpdateIioConfig (Default);

  return Status;
}
