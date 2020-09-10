/** @file
  PeiUbaIioConfigLib implementation.

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

#include <PiPei.h>
#include <Uefi/UefiSpec.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/DebugLib.h>
#include <Ppi/UbaCfgDb.h>
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
  UBA_CONFIG_DATABASE_PPI           *UbaConfigPpi = NULL;
  PLATFORM_IIO_CONFIG_UPDATE_TABLE  IioConfigTable;
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

  TableSize = sizeof(IioConfigTable);
  Status = UbaConfigPpi->GetData (
                                 UbaConfigPpi,
                                 &gPlatformIioConfigDataGuid,
                                 &IioConfigTable,
                                 &TableSize
                                 );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (IioConfigTable.Signature == PLATFORM_IIO_CONFIG_UPDATE_SIGNATURE);
  ASSERT (IioConfigTable.Version == PLATFORM_IIO_CONFIG_UPDATE_VERSION);

  *BifurcationTable = IioConfigTable.IioBifurcationTablePtr;
  *BifurcationEntries = (UINT8) (IioConfigTable.IioBifurcationTableSize/sizeof(IIO_BIFURCATION_DATA_ENTRY));

  *SlotTable = IioConfigTable.IioSlotTablePtr;
  *SlotEntries = (UINT8)(IioConfigTable.IioSlotTableSize/sizeof(IIO_SLOT_CONFIG_DATA_ENTRY));

  return Status;
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
  EFI_STATUS                        Status = EFI_SUCCESS;
  UBA_CONFIG_DATABASE_PPI           *UbaConfigPpi = NULL;
  PLATFORM_IIO_CONFIG_UPDATE_TABLE  IioConfigTable;
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

  TableSize = sizeof(IioConfigTable);
  if (SkuPersonalityType == 1) {
    Status = UbaConfigPpi->GetData (
                                   UbaConfigPpi,
                                   &gPlatformIioConfigDataGuid_1,
                                   &IioConfigTable,
                                   &TableSize
                                   );
  } else if (SkuPersonalityType == 2) {
    Status = UbaConfigPpi->GetData (
                                   UbaConfigPpi,
                                   &gPlatformIioConfigDataGuid_2,
                                   &IioConfigTable,
                                   &TableSize
                                   );
  } else if (SkuPersonalityType == 3) {
    Status = UbaConfigPpi->GetData (
                                   UbaConfigPpi,
                                   &gPlatformIioConfigDataGuid_3,
                                   &IioConfigTable,
                                   &TableSize
                                   );
  } else {
    Status = UbaConfigPpi->GetData (
                                   UbaConfigPpi,
                                   &gPlatformIioConfigDataGuid,
                                   &IioConfigTable,
                                   &TableSize
                                   );
  }
  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (IioConfigTable.Signature == PLATFORM_IIO_CONFIG_UPDATE_SIGNATURE);
  ASSERT (IioConfigTable.Version == PLATFORM_IIO_CONFIG_UPDATE_VERSION);

  *BifurcationTable = IioConfigTable.IioBifurcationTablePtr;
  *BifurcationEntries = (UINT8) (IioConfigTable.IioBifurcationTableSize/sizeof(IIO_BIFURCATION_DATA_ENTRY));

  *SlotTable = IioConfigTable.IioSlotTablePtr;
  *SlotEntries = (UINT8)(IioConfigTable.IioSlotTableSize/sizeof(IIO_SLOT_CONFIG_DATA_ENTRY));

  return Status;
}

EFI_STATUS
PlatformUpdateIioConfig (
  IN  IIO_GLOBALS             *IioGlobalData
)
{
  EFI_STATUS                        Status;
  UBA_CONFIG_DATABASE_PPI           *UbaConfigPpi = NULL;
  PLATFORM_IIO_CONFIG_UPDATE_TABLE  IioConfigTable;
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

  TableSize = sizeof(IioConfigTable);
  Status = UbaConfigPpi->GetData (
                                 UbaConfigPpi,
                                 &gPlatformIioConfigDataGuid,
                                 &IioConfigTable,
                                 &TableSize
                                 );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (IioConfigTable.Signature == PLATFORM_IIO_CONFIG_UPDATE_SIGNATURE);
  ASSERT (IioConfigTable.Version == PLATFORM_IIO_CONFIG_UPDATE_VERSION);

  Status = IioConfigTable.CallUpdate (IioGlobalData);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  return Status;
}

EFI_STATUS
PlatformUpdateIioConfig_EX (
  IN  IIO_GLOBALS             *IioGlobalData
)
{
  EFI_STATUS                           Status;
  UBA_CONFIG_DATABASE_PPI              *UbaConfigPpi = NULL;
  PLATFORM_IIO_CONFIG_UPDATE_TABLE_EX  IioConfigTable;
  UINTN                                TableSize;

  Status = PeiServicesLocatePpi (
              &gUbaConfigDatabasePpiGuid,
              0,
              NULL,
              &UbaConfigPpi
              );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  TableSize = sizeof(IioConfigTable);
  Status = UbaConfigPpi->GetData (
                                 UbaConfigPpi,
                                 &gPlatformIioConfigDataGuid,
                                 &IioConfigTable,
                                 &TableSize
                                 );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (IioConfigTable.Signature == PLATFORM_IIO_CONFIG_UPDATE_SIGNATURE);

  Status = IioConfigTable.CallUpdate (IioGlobalData);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  return Status;
}