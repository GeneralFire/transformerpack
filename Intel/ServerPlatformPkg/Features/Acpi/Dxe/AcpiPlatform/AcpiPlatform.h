/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#ifndef _ACPI_PLATFORM_H_
#define _ACPI_PLATFORM_H_

//
// Statements that include other header files
//
#include <PiDxe.h>
#include <PchAccess.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/SerialPortConsoleRedirectionTable.h>
#include <Acpi/DMARemappingReportingTable.h>
#include <IndustryStandard/HighPrecisionEventTimerTable.h>
#include <IndustryStandard/MemoryMappedConfigurationSpaceAccessTable.h>
#include <IndustryStandard/Pci.h>
#include <Library/AcpiPlatformLib.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Protocol/GlobalNvsArea.h>
#include <Protocol/IioUds.h>
#include <Protocol/DmaRemap.h>
#include <Protocol/PciIo.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/SerialIo.h>
#include <Protocol/LpcPolicy.h>
#include <Protocol/MpService.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/GlobalNvsArea.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Library/MemMapDataLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Guid/PlatformInfo.h>
#include <Guid/SetupVariable.h>
#include <PchSetupVariable.h>
#include <Guid/SocketVariable.h>
#include <Guid/HobList.h>
#include <Guid/MemoryMapData.h>
#include <Protocol/PlatformType.h>
#include <Protocol/CpuCsrAccess.h>
#include <Cpu/CpuCoreRegs.h>
#include <PpmPolicyPeiDxeCommon.h>
#include <Acpi/Mcfg.h>
#include <Acpi/Hpet.h>
#include <Acpi/Srat.h>
#include <Acpi/Slit.h>
#include <Acpi/Migt.h>
#include <Acpi/Msct.h>
#include <Acpi/Bdat.h>
#include <Acpi/Nfit.h>
#include <Acpi/Pcat.h>
#include "Platform.h"
#include <Acpi/AcpiVTD.h>
#include <Protocol/Smbios.h>
#include <IndustryStandard/SmBios.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PchInfoLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/PlatformStatusCodes.h>
#include <Library/FpgaConfigurationLib.h>
#include <Fpga.h>

#include <SystemBoard.h>
#include <Protocol/CrystalRidge.h>
#include <Library/SystemInfoLib.h>
#include <Register/Cpuid.h>
#include <PchAccess.h>
#include <Library/PmcLib.h>
#include <Library/MmPciBaseLib.h>
#include <Library/PchPcrLib.h>
#include <UncoreCommonIncludes.h>


#define RTC_ADDRESS_REGISTER_D                      13


/**

  Entry point for Acpi platform driver.

  @param ImageHandle  -  A handle for the image that is initializing this driver.
  @param SystemTable  -  A pointer to the EFI system table.

  @retval EFI_SUCCESS           -  Driver initialized successfully.
  @retval EFI_LOAD_ERROR        -  Failed to Initialize or has been loaded.
  @retval EFI_OUT_OF_RESOURCES  -  Could not allocate needed resources.

**/
EFI_STATUS
EFIAPI
AcpiPlatformEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  );

EFI_STATUS
LocateSupportProtocol (
  IN   EFI_GUID       *Protocol,
  IN   EFI_GUID       gEfiAcpiMultiTableStorageGuid,
  OUT  VOID           **Instance,
  IN   UINT32         Type
  );

VOID
AcpiVtdIntRemappingEnable (
  VOID
  );

EFI_STATUS
AcpiVtdTablesInstall (
  VOID
  );

#endif // _ACPI_PLATFORM_H_
