/** @file
  This library provides a set of platform only ACPI tables and functions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2020 Intel Corporation. <BR>

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

#ifndef _PLATFORM_SPECIFIC_ACPI_TABLE_LIB_H_
#define _PLATFORM_SPECIFIC_ACPI_TABLE_LIB_H_

#include <IndustryStandard/Acpi.h>
#include <Library/AcpiPlatformLib.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Protocol/DevicePath.h>
#include <Protocol/SerialIo.h>
#include <Protocol/SuperIo.h>
#include <Guid/GlobalVariable.h>
#include <IndustryStandard/SerialPortConsoleRedirectionTable.h>
/**
  Update the SPMI table.

  @param [in, out]      SpmiAcpiTable       The table to be set.

  @retval EFI SUCCESS   Procedure returned successfully.
**/
EFI_STATUS
PatchSpmiAcpiTable (
  IN OUT  EFI_ACPI_COMMON_HEADER     *Table
  );

/**

  This function will check ACPI Table is active or not active.

  @param Table  -  The table to update

  @retval EFI_SUCCESS  -  The Table is active.

**/
EFI_STATUS
PlatformAcpiReportHooksTableIsActive (
  IN OUT EFI_ACPI_COMMON_HEADER     *Table
  );

/**

  This function will patch to update platform Acpi Table information.
  This currently includes:
    SPMI ACPI Table is WhitelyPC only. NFIT/PCAT is not supported by SNR.
    SPCR function is implemented by platform own.

  @param Table  -  The table to update

  @retval EFI_SUCCESS  -  The function completed successfully.
'
**/
EFI_STATUS
PatchPlatformSpecificAcpiTableHooks (
  IN OUT EFI_ACPI_COMMON_HEADER     *Table
  );

/**

  This function will patch to update SPCR Table information.

  @param Table  -  The table to update

  @retval EFI_SUCCESS  -  The function completed successfully.

**/
EFI_STATUS
PatchSpcrAcpiTable (
  IN OUT  EFI_ACPI_COMMON_HEADER  *Table
  );

/**
  Update the HMAT table.

  @param [in, out]      HmatAcpiTable       The table to be set.

  @retval EFI SUCCESS   Procedure returned successfully.
**/
EFI_STATUS
PatchHmatAcpiTable (
  IN OUT  EFI_ACPI_COMMON_HEADER  *Table
  );
EFI_STATUS
PatchPlatformMemoryTopologyTable (
  IN OUT   EFI_ACPI_COMMON_HEADER  *Table
  );

/**

  Update the MSCT ACPI table

  @param *MsctAcpiTable - The table to be set

  @retval EFI_SUCCESS -  Returns Success

**/
EFI_STATUS
PatchMsctAcpiTable (
  IN OUT   EFI_ACPI_COMMON_HEADER   *Table
  );

/**

  Update the MIGT ACPI table

  @param *TableHeader   - The table to be set

  @retval EFI_SUCCESS -  Returns Success

**/
EFI_STATUS
PatchMigtAcpiTable (
  IN OUT   EFI_ACPI_COMMON_HEADER   *Table
  );

/**

  Update the BDAT ACPI table: Multiple instances of the BDAT DATA HOB are placed into one contiguos memory range

  @param *TableHeader   - The table to be set

  @retval EFI_SUCCESS -  Returns Success

**/
EFI_STATUS
PatchBdatAcpiTable (
  IN OUT  EFI_ACPI_COMMON_HEADER  *Table
  );

/**

  Update the CEDT ACPI table

  @param *TableHeader   - The table to be set

  @retval EFI_SUCCESS -  Returns Success

**/
EFI_STATUS
PatchCedtAcpiTable (
  IN OUT  EFI_ACPI_COMMON_HEADER  *Table
  );
#endif
