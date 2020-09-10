/** @file

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

#include <Base.h>
#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>

#include <Library/UbaSystemBoardInfoLib.h>
#include <Protocol/UbaCfgDb.h>


EFI_STATUS
GetSystemBoardInfo (
  IN OUT   DXE_SYSTEM_BOARD_INFO       **SystemboardInfoTableBuffer
  )
{
  EFI_STATUS                        Status;
  UBA_CONFIG_DATABASE_PROTOCOL      *UbaConfigProtocol = NULL;
  UINTN                             DataLength = 0;
  SYSTEM_BOARD_INFO_DATA            SystemBoardInfoData;

  Status = gBS->LocateProtocol (
                  &gUbaConfigDatabaseProtocolGuid,
                  NULL,
                  &UbaConfigProtocol
                  );

  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR," [GetSystemBoardInfo] Locate UbaConfigProtocol fail!\n"));
    return Status;
  }

  DataLength = sizeof(SystemBoardInfoData);
  Status = UbaConfigProtocol->GetData (
                                   UbaConfigProtocol,
                                   &gSystemBoardInfoConfigDataGuid,
                                   &SystemBoardInfoData,
                                   &DataLength
                                   );

  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR," [GetSystemBoardInfo] Get Data fail!\n"));
    return Status;
  }

  ASSERT (SystemBoardInfoData.Signature == SYSTEM_SYSTEM_BOARD_INFO_SIGNATURE);
  ASSERT (SystemBoardInfoData.Version   == SYSTEM_SYSTEM_BOARD_INFO_VERSION);

  *SystemboardInfoTableBuffer = SystemBoardInfoData.CallUpdate ();

  return Status;
}
