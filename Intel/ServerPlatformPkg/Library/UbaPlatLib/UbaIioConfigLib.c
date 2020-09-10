/** @file
  DxeUbaIioConfigLib implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2018 Intel Corporation. <BR>

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
#include <Protocol/UbaCfgDb.h>
#include <Library/UbaIioConfigLib.h>

EFI_STATUS
PlatformIioConfigInit (
  IN OUT IIO_BIFURCATION_DATA_ENTRY       **BifurcationTable,
  IN OUT UINT8                            *BifurcationEntries,
  IN OUT IIO_SLOT_CONFIG_DATA_ENTRY       **SlotTable,
  IN OUT UINT8                            *SlotEntries
)
{
  EFI_STATUS                        Status;
  UBA_CONFIG_DATABASE_PROTOCOL      *UbaConfigProtocol = NULL;
  UINTN                             DataLength = 0;
  PLATFORM_IIO_CONFIG_UPDATE_TABLE  IioUpdateTable;

  Status = gBS->LocateProtocol (
                  &gUbaConfigDatabaseProtocolGuid,
                  NULL,
                  &UbaConfigProtocol
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  DataLength  = sizeof (IioUpdateTable);
  Status = UbaConfigProtocol->GetData (
                                    UbaConfigProtocol,
                                    &gPlatformIioConfigDataDxeGuid,
                                    &IioUpdateTable,
                                    &DataLength
                                    );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (IioUpdateTable.Signature == PLATFORM_IIO_CONFIG_UPDATE_SIGNATURE);
  ASSERT (IioUpdateTable.Version == PLATFORM_IIO_CONFIG_UPDATE_VERSION);

  *BifurcationTable = IioUpdateTable.IioBifurcationTablePtr;
  *BifurcationEntries = (UINT8) (IioUpdateTable.IioBifurcationTableSize/sizeof(IIO_BIFURCATION_DATA_ENTRY));

  *SlotTable = IioUpdateTable.IioSlotTablePtr;
  *SlotEntries = (UINT8)(IioUpdateTable.IioSlotTableSize/sizeof(IIO_SLOT_CONFIG_DATA_ENTRY));

  return EFI_SUCCESS;
}

EFI_STATUS
PlatformIioConfigInit2 (
  IN     UINT8                            SkuPersonalityType,
  IN OUT IIO_BIFURCATION_DATA_ENTRY       **BifurcationTable,
  IN OUT UINT8                            *BifurcationEntries,
  IN OUT IIO_SLOT_CONFIG_DATA_ENTRY       **SlotTable,
  IN OUT UINT8                            *SlotEntries
)
{
  EFI_STATUS                        Status;
  UBA_CONFIG_DATABASE_PROTOCOL      *UbaConfigProtocol = NULL;
  UINTN                             DataLength = 0;
  PLATFORM_IIO_CONFIG_UPDATE_TABLE  IioUpdateTable;

  Status = gBS->LocateProtocol (
                  &gUbaConfigDatabaseProtocolGuid,
                  NULL,
                  &UbaConfigProtocol
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  DataLength  = sizeof (IioUpdateTable);
  if (SkuPersonalityType == 1) {
    Status = UbaConfigProtocol->GetData (
                                      UbaConfigProtocol,
                                      &gPlatformIioConfigDataDxeGuid_1,
                                      &IioUpdateTable,
                                      &DataLength
                                      );
  } else if (SkuPersonalityType == 2) {
    Status = UbaConfigProtocol->GetData (
                                      UbaConfigProtocol,
                                      &gPlatformIioConfigDataDxeGuid_2,
                                      &IioUpdateTable,
                                      &DataLength
                                      );
  } else if (SkuPersonalityType == 3) {
    Status = UbaConfigProtocol->GetData (
                                      UbaConfigProtocol,
                                      &gPlatformIioConfigDataDxeGuid_3,
                                      &IioUpdateTable,
                                      &DataLength
                                      );
  } else {
    Status = UbaConfigProtocol->GetData (
                                      UbaConfigProtocol,
                                      &gPlatformIioConfigDataDxeGuid,
                                      &IioUpdateTable,
                                      &DataLength
                                      );
  }
  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (IioUpdateTable.Signature == PLATFORM_IIO_CONFIG_UPDATE_SIGNATURE);
  ASSERT (IioUpdateTable.Version == PLATFORM_IIO_CONFIG_UPDATE_VERSION);

  *BifurcationTable = IioUpdateTable.IioBifurcationTablePtr;
  *BifurcationEntries = (UINT8) (IioUpdateTable.IioBifurcationTableSize/sizeof(IIO_BIFURCATION_DATA_ENTRY));

  *SlotTable = IioUpdateTable.IioSlotTablePtr;
  *SlotEntries = (UINT8)(IioUpdateTable.IioSlotTableSize/sizeof(IIO_SLOT_CONFIG_DATA_ENTRY));

  return EFI_SUCCESS;
}

