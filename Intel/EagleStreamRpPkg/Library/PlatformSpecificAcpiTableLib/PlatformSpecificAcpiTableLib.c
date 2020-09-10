/** @file
   Hooks for Platform populate different function and Platform only ACPI Table.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2020 Intel Corporation. <BR>

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

#include <PiDxe.h>
#include <Library/PlatformSpecificAcpiTableLib.h>
#include <Acpi/Cedt.h>

/**

  This function will check ACPI Table is active or not active.

  @param Table  -  The table to update

  @retval EFI_SUCCESS  -  The Table is active.

**/
EFI_STATUS
PlatformAcpiReportHooksTableIsActive (
  IN OUT EFI_ACPI_COMMON_HEADER     *Table
  )
{
  EFI_ACPI_DESCRIPTION_HEADER     *TableHeader;

  TableHeader   = (EFI_ACPI_DESCRIPTION_HEADER *) Table;
  if (TableHeader->Signature == EFI_ACPI_5_1_SERVER_PLATFORM_MANAGEMENT_INTERFACE_TABLE_SIGNATURE) {
    return EFI_NOT_FOUND;
  }
  return EFI_SUCCESS;
}

/**

  This function will patch to update platform level Acpi Table information.
  This currently includes:
    Enabling the proper processor entries in the APIC tables.

  @param Table  -  The table to update

  @retval EFI_SUCCESS  -  The function completed successfully.

**/
EFI_STATUS
PatchPlatformSpecificAcpiTableHooks (
  IN OUT EFI_ACPI_COMMON_HEADER     *Table
  )
{
  EFI_ACPI_DESCRIPTION_HEADER     *TableHeader;

  TableHeader   = (EFI_ACPI_DESCRIPTION_HEADER *) Table;
  if (TableHeader->Signature == EFI_ACPI_CEDT_TABLE_SIGNATURE) {
    return PatchCedtAcpiTable (Table);
  }
  return EFI_SUCCESS;
}