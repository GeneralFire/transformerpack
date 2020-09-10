/** @file
  UbaSlotUpdateLib implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2016 Intel Corporation. <BR>

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
#include <Uefi/UefiSpec.h>
#include <Ppi/UbaCfgDb.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/DebugLib.h>
#include <Library/UbaSlotUpdateLib.h>

EFI_STATUS
PlatformGetSlotTableData (
  IN OUT IIO_BROADWAY_ADDRESS_DATA_ENTRY  **BroadwayTable,
  IN OUT UINT8                            *IOU2Setting,
  IN OUT UINT8                            *FlagValue
)
{
  EFI_STATUS                        Status;
  UBA_CONFIG_DATABASE_PPI           *UbaConfigPpi = NULL;
  PLATFORM_SLOT_UPDATE_TABLE        IioSlotTable;
  UINTN                             TableSize;

  Status = PeiServicesLocatePpi (
              &gUbaConfigDatabasePpiGuid,
              0,
              NULL,
              &UbaConfigPpi
              );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  TableSize = sizeof(IioSlotTable);
  Status = UbaConfigPpi->GetData (
                                 UbaConfigPpi,
                                 &gPlatformSlotDataGuid,
                                 &IioSlotTable,
                                 &TableSize
                                 );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (IioSlotTable.Signature == PLATFORM_SLOT_UPDATE_SIGNATURE);
  ASSERT (IioSlotTable.Version == PLATFORM_SLOT_UPDATE_VERSION);

  *BroadwayTable = IioSlotTable.BroadwayTablePtr;
  *IOU2Setting   = IioSlotTable.GetIOU2Setting (*IOU2Setting);
  *FlagValue      = IioSlotTable.FlagValue;

  return Status;
}

EFI_STATUS
PlatformGetSlotTableData2 (
  IN OUT IIO_BROADWAY_ADDRESS_DATA_ENTRY  **BroadwayTable,
  IN OUT UINT8                            *IOU0Setting,
  IN OUT UINT8                            *FlagValue,
  IN OUT UINT8                            *IOU2Setting,
  IN     UINT8                            SkuPersonalityType
)
{
  EFI_STATUS                        Status;
  UBA_CONFIG_DATABASE_PPI           *UbaConfigPpi = NULL;
  PLATFORM_SLOT_UPDATE_TABLE2       IioSlotTable;
  UINTN                             TableSize;

  Status = PeiServicesLocatePpi (
              &gUbaConfigDatabasePpiGuid,
              0,
              NULL,
              &UbaConfigPpi
              );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  TableSize = sizeof(IioSlotTable);
  if ((SkuPersonalityType == 1) || (SkuPersonalityType == 3)) {
    Status = UbaConfigPpi->GetData (
                                 UbaConfigPpi,
                                 &gPlatformSlotDataGuid2_1,
                                 &IioSlotTable,
                                 &TableSize
                                 );
  } else {
    Status = UbaConfigPpi->GetData (
                                 UbaConfigPpi,
                                 &gPlatformSlotDataGuid2,
                                 &IioSlotTable,
                                 &TableSize
                                 );
  }
  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (IioSlotTable.Signature == PLATFORM_SLOT_UPDATE_SIGNATURE);
  ASSERT (IioSlotTable.Version == PLATFORM_SLOT_UPDATE_VERSION);

  *BroadwayTable = IioSlotTable.BroadwayTablePtr;
  *IOU0Setting   = IioSlotTable.GetIOU0Setting (*IOU0Setting);
  *FlagValue     = IioSlotTable.FlagValue;
  *IOU2Setting   = IioSlotTable.GetIOU2Setting (SkuPersonalityType, *IOU2Setting);

  return Status;
}

EFI_STATUS
PlatformPchGetPciSlotImplementedTableData (
  IN OUT UINT8                     **SlotImplementedTable
)
{
  EFI_STATUS                                      Status;
  UBA_CONFIG_DATABASE_PPI                         *UbaConfigPpi = NULL;
  PLATFORM_PCH_PCI_SLOT_IMPLEMENTED_UPDATE_TABLE  SITable;
  UINTN                                           TableSize;

  Status = PeiServicesLocatePpi (
              &gUbaConfigDatabasePpiGuid,
              0,
              NULL,
              (VOID **)&UbaConfigPpi
              );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  TableSize = sizeof(SITable);
  Status = UbaConfigPpi->GetData (
                                 UbaConfigPpi,
                                 &gPlatformPciSlotImplementedGuid,
                                 &SITable,
                                 &TableSize
                                 );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (SITable.Signature == PLATFORM_SLOT_UPDATE_SIGNATURE);
  ASSERT (SITable.Version == PLATFORM_SLOT_UPDATE_VERSION);

  *SlotImplementedTable = SITable.SlotImplementedTableDataPtr;

  return Status;
}
