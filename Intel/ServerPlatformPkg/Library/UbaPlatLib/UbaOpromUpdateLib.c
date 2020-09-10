/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 Intel Corporation. <BR>

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

#include <Library/UbaOpromUpdateLib.h>

#include <Protocol/UbaCfgDb.h>


BOOLEAN
PlatformCheckPcieRootPort (
  IN  UINTN                 Bus,
  IN  UINT32                PcieSlotOpromBitMap
)
{
  EFI_STATUS                              Status;

  UBA_CONFIG_DATABASE_PROTOCOL      *UbaConfigProtocol = NULL;
  UINTN                                   DataLength = 0;
  PLATFORM_OPTION_ROM_UPDATE_DATA         OptionRomUpdateTable;

  Status = gBS->LocateProtocol (
                  &gUbaConfigDatabaseProtocolGuid,
                  NULL,
                  &UbaConfigProtocol
                  );
  if (EFI_ERROR(Status)) {
    return TRUE;
  }

  DataLength  = sizeof (OptionRomUpdateTable);
  Status = UbaConfigProtocol->GetData (
                                    UbaConfigProtocol,
                                    &gPlatformOptionRomUpdateConfigDataGuid,
                                    &OptionRomUpdateTable,
                                    &DataLength
                                    );
  if (EFI_ERROR(Status)) {
    return TRUE;
  }

  ASSERT (OptionRomUpdateTable.Signature == PLATFORM_OPTION_ROM_UPDATE_SIGNATURE);
  ASSERT (OptionRomUpdateTable.Version == PLATFORM_OPTION_ROM_UPDATE_VERSION);

  return OptionRomUpdateTable.CallCheckRootPort (Bus, PcieSlotOpromBitMap);
}

EFI_STATUS
PlatformGetOptionRomTable (
  IN  PC_PCI_OPTION_ROM_TABLE         **OptionRomTable
)
{
  EFI_STATUS                              Status;

  UBA_CONFIG_DATABASE_PROTOCOL      *UbaConfigProtocol = NULL;
  UINTN                                   DataLength = 0;
  PLATFORM_OPTION_ROM_UPDATE_DATA         OptionRomUpdateTable;

  Status = gBS->LocateProtocol (
                  &gUbaConfigDatabaseProtocolGuid,
                  NULL,
                  &UbaConfigProtocol
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  DataLength  = sizeof (OptionRomUpdateTable);
  Status = UbaConfigProtocol->GetData (
                                    UbaConfigProtocol,
                                    &gPlatformOptionRomUpdateConfigDataGuid,
                                    &OptionRomUpdateTable,
                                    &DataLength
                                    );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (OptionRomUpdateTable.Signature == PLATFORM_OPTION_ROM_UPDATE_SIGNATURE);
  ASSERT (OptionRomUpdateTable.Version == PLATFORM_OPTION_ROM_UPDATE_VERSION);

  if (OptionRomUpdateTable.GetOptionRomTable == NULL) {
    return EFI_NOT_FOUND;
  }

  return OptionRomUpdateTable.GetOptionRomTable (OptionRomTable);
}

EFI_STATUS
PlatformGetNicSetupConfigTable (
  IN  NIC_SETUP_CONFIGURATION_STUCT     **NicSetupConfigTable,
  IN  UINTN                             *NumOfConfig
)
{
  EFI_STATUS                              Status;

  UBA_CONFIG_DATABASE_PROTOCOL      *UbaConfigProtocol = NULL;
  UINTN                                   DataLength = 0;
  PLATFORM_OPTION_ROM_UPDATE_DATA         OptionRomUpdateTable;

  Status = gBS->LocateProtocol (
                  &gUbaConfigDatabaseProtocolGuid,
                  NULL,
                  &UbaConfigProtocol
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  DataLength  = sizeof (OptionRomUpdateTable);
  Status = UbaConfigProtocol->GetData (
                                    UbaConfigProtocol,
                                    &gPlatformOptionRomUpdateConfigDataGuid,
                                    &OptionRomUpdateTable,
                                    &DataLength
                                    );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (OptionRomUpdateTable.Signature == PLATFORM_OPTION_ROM_UPDATE_SIGNATURE);
  ASSERT (OptionRomUpdateTable.Version == PLATFORM_OPTION_ROM_UPDATE_VERSION);

  if (OptionRomUpdateTable.GetNicSetupConfigTable == NULL) {
    return EFI_NOT_FOUND;
  }

  return OptionRomUpdateTable.GetNicSetupConfigTable (NicSetupConfigTable, NumOfConfig);
}

EFI_STATUS
PlatformGetNicCapabilityTable (
  IN  NIC_OPTIONROM_CAPBILITY_STRUCT    **NicCapabilityTable,
  IN  UINTN                             *NumOfNicCapTable
  )
{
  EFI_STATUS                              Status;

  UBA_CONFIG_DATABASE_PROTOCOL      *UbaConfigProtocol = NULL;
  UINTN                                   DataLength = 0;
  PLATFORM_OPTION_ROM_UPDATE_DATA         OptionRomUpdateTable;

  Status = gBS->LocateProtocol (
                  &gUbaConfigDatabaseProtocolGuid,
                  NULL,
                  &UbaConfigProtocol
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  DataLength  = sizeof (OptionRomUpdateTable);
  Status = UbaConfigProtocol->GetData (
                                    UbaConfigProtocol,
                                    &gPlatformOptionRomUpdateConfigDataGuid,
                                    &OptionRomUpdateTable,
                                    &DataLength
                                    );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (OptionRomUpdateTable.Signature == PLATFORM_OPTION_ROM_UPDATE_SIGNATURE);
  ASSERT (OptionRomUpdateTable.Version == PLATFORM_OPTION_ROM_UPDATE_VERSION);

  if (OptionRomUpdateTable.GetNicCapabilityTable == NULL) {
    return EFI_NOT_FOUND;
  }

  return OptionRomUpdateTable.GetNicCapabilityTable (NicCapabilityTable, NumOfNicCapTable);
}

EFI_STATUS
PlatformSetupPcieSlotNumber (
  OUT  UINT8                   *PcieSlotItemCtrl
)
{
  EFI_STATUS                              Status;

  UBA_CONFIG_DATABASE_PROTOCOL      *UbaConfigProtocol = NULL;
  UINTN                                   DataLength = 0;
  PLATFORM_OPTION_ROM_UPDATE_DATA         OptionRomUpdateTable;

  Status = gBS->LocateProtocol (
                  &gUbaConfigDatabaseProtocolGuid,
                  NULL,
                  &UbaConfigProtocol
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  DataLength  = sizeof (OptionRomUpdateTable);
  Status = UbaConfigProtocol->GetData (
                                    UbaConfigProtocol,
                                    &gPlatformOptionRomUpdateConfigDataGuid,
                                    &OptionRomUpdateTable,
                                    &DataLength
                                    );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (OptionRomUpdateTable.Signature == PLATFORM_OPTION_ROM_UPDATE_SIGNATURE);
  ASSERT (OptionRomUpdateTable.Version == PLATFORM_OPTION_ROM_UPDATE_VERSION);

  return OptionRomUpdateTable.SetupSlotNumber (PcieSlotItemCtrl);
}

