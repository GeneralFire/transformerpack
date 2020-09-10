/** @file
  ACPI Platform Driver Hooks

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1996 - 2018 Intel Corporation. <BR>

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

//
// Statements that include other files
//
#include "AcpiPlatformLibLocal.h"

extern BIOS_ACPI_PARAM      *mAcpiParameter;



/**

    Update the MIGT ACPI table

    @param *TableHeader   - The table to be set

    @retval EFI_SUCCESS -  Returns Success

**/
EFI_STATUS
PatchMigtAcpiTable (
  IN OUT   EFI_ACPI_COMMON_HEADER   *Table
  )
{
  EFI_STATUS        Status;
  UINT64            Address;
  UINTN             idx;
  UINT8             checksum;
  EFI_MIGT_ACPI_DESCRIPTION_TABLE *MigtAcpiTable;

  MigtAcpiTable = (EFI_MIGT_ACPI_DESCRIPTION_TABLE *)Table;
  Address = 0xffffffff;

  Status  = gBS->AllocatePages (
                   AllocateMaxAddress,
                   EfiACPIMemoryNVS,
                   1,     //page
                   &Address
                   );

  if (EFI_ERROR (Status)) {
    return Status;
  }
  //update MIGT ACPI table
  MigtAcpiTable->ActionRegion.Address = Address;

  //update checksum
  MigtAcpiTable->Header.Checksum = 0;
  checksum = 0;
  for(idx = 0; idx < sizeof(EFI_MIGT_ACPI_DESCRIPTION_TABLE); idx++) {
    checksum = checksum + (UINT8) (((UINT8 *)(MigtAcpiTable))[idx]);
  }
  MigtAcpiTable->Header.Checksum = (UINT8) (0 - checksum);

  //
  // Update Migration Action Region GAS address
  //
  mAcpiParameter->MigrationActionRegionAddress = Address;

  return Status;

}

