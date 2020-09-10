/** @file
  PeiBoardInit.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#ifndef _PEI_BOARD_INIT_PEIM_H_
#define _PEI_BOARD_INIT_PEIM_H_

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Ppi/UbaCfgDb.h>
#include <Guid/PlatformInfo.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>

#include <Library/GpioLib.h>
#include <Pins/GpioPinsVer3.h>

// TypeArcherCityEVB
EFI_STATUS
TypeArcherCityEVBPlatformUpdateUsbOcMappings (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
);

EFI_STATUS
TypeArcherCityEVBPlatformUpdateAcpiTablePcds (
  VOID
);

EFI_STATUS
TypeArcherCityEVBInstallGpioData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
);

EFI_STATUS
TypeArcherCityEVBInstallClockgenData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
);

EFI_STATUS
TypeArcherCityEVBInstallPcdData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
);

EFI_STATUS
TypeArcherCityEVBPchEarlyUpdate (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
);

EFI_STATUS
TypeArcherCityEVBIioPortBifurcationInit (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
);

EFI_STATUS
TypeArcherCityEVBPchPcieBifurcation (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
);

EFI_STATUS
TypeArcherCityEVBInstallSlotTableData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
);

EFI_STATUS
TypeArcherCityEVBInstallKtiEparamData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
);

EFI_STATUS
TypeArcherCityEVBInstallGpioPlatformData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
);

#endif // _PEI_BOARD_INIT_PEIM_H_
