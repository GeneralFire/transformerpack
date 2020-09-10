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

#include <PiPei.h>
#include <Uefi/UefiSpec.h>
#include <Ppi/UbaCfgDb.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/UbaPcieBifurcationUpdateLib.h>

EFI_STATUS
PlatformGetPchPcieBifurcationConfig (
  IN OUT   PCIE_BIFURCATION_CONFIG         **PchPcieBifurcationConfig,
  IN OUT   PCH_SLOT_CONFIG_DATA_ENTRY_EX   **PchSlotConfig
)
{
  EFI_STATUS                                    Status;
  UBA_CONFIG_DATABASE_PPI                       *UbaConfigPpi = NULL;
  PLATFORM_PCH_PCIE_BIFURCATION_UPDATE_TABLE    BifurcationUpdateTable;
  UINTN                                         TableSize;

  Status = PeiServicesLocatePpi (
              &gUbaConfigDatabasePpiGuid,
              0,
              NULL,
              &UbaConfigPpi
              );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  TableSize = sizeof(BifurcationUpdateTable);
  Status = UbaConfigPpi->GetData (
                           UbaConfigPpi,
                           &gPlatformUbaPcieBifurcationGuid,
                           &BifurcationUpdateTable,
                           &TableSize
                           );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  ASSERT (BifurcationUpdateTable.Signature == PLATFORM_PCH_PCIE_BIFURCATION_UPDATE_SIGNATURE);
  ASSERT (BifurcationUpdateTable.Version == PLATFORM_PCH_PCIE_BIFURCATION_UPDATE_VERSION);

  BifurcationUpdateTable.CallPcieBifurcationUpdate(PchPcieBifurcationConfig, PchSlotConfig);
  return Status;
}
