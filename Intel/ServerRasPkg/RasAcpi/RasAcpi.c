/** @file
  SMM driver for RAS ACPI between OS and firmware.
  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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

#include <PiSmm.h>
#include <IndustryStandard/Acpi.h>
#include <Protocol/AcpiTable.h>
#include <Library/PlatPolicyLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <IioRegs.h>
#include <Library/IioTopologyLib.h>
#include <Library/MmPciExtLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/RasDebugLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/RasGlobalDataLib.h>
#include <RasAcpi.h>
#include "RasAcpiInternal.h"

RAS_ACPI_PARAM      *mRasAcpiParam;
/**
  Patch the SSDT to set the communication buffer address and length.

  @param[in, out] Table         The SSDT data to patch.

  @retval EFI_SUCCESS           The SSDT was successfully patched.
  @retval EFI_NOT_FOUND         The OpRegion was not found.
**/
EFI_STATUS
PatchSsdt (
  IN OUT  EFI_ACPI_DESCRIPTION_HEADER  *Table
  )
{
  EFI_STATUS                    Status;
  UINT8                         *DataStart;
  UINT8                         *DataEnd;
  UINT8                         *BytePointer;
  ADDRESS_TRANSLATION_OP_REGION *OpRegion;

  RAS_DEBUG ((
    LEVEL_FUNC_FLOW,
    "[RAS ACPI] Enter PatchSsdt\n"
    ));

  DataStart = (UINT8 *) (Table + 1);
  DataEnd = ((UINT8 *) Table) + Table->Length;

  Status = EFI_NOT_FOUND;

  //
  // Iterate through the data one byte at a time, but stop when the end of the
  // OpRegion starting at the current byte would be past the end of the data.
  //
  for (BytePointer = DataStart;
       BytePointer < DataEnd + 1 - sizeof (ADDRESS_TRANSLATION_OP_REGION);
       BytePointer++) {
    //
    // Search for the expected OpRegion.
    //
    OpRegion = (ADDRESS_TRANSLATION_OP_REGION *) BytePointer;
    if (OpRegion->ExtOpPrefix != AML_EXT_OP) {
      continue;
    }
    if (OpRegion->ExtOpCode != AML_EXT_REGION_OP) {
      continue;
    }
    if (OpRegion->NameString != SIGNATURE_32 ('R', 'A', 'S', 'P')) {
      continue;
    }
    if (OpRegion->RegionSpace != 0x00) {  // 0x00 = SystemMemory
      continue;
    }
    if (OpRegion->RegionOffset.DWordPrefix != AML_DWORD_PREFIX) {
      continue;
    }
    if (OpRegion->RegionLen.DWordPrefix != AML_DWORD_PREFIX) {
      continue;
    }

    //
    // Found it. Update the offset and length, and exit the loop.
    //
    OpRegion->RegionOffset.DWordData = (UINT32) (UINTN) mRasAcpiParam;
    OpRegion->RegionLen.DWordData = sizeof (RAS_ACPI_PARAM);
    Status = EFI_SUCCESS;
    break;
  }

  RAS_DEBUG ((
    LEVEL_FUNC_FLOW,
    "[RAS ACPI] Exit PatchSsdt: %r\n",
    Status
    ));

  return Status;
} // PatchSsdt

/**
  Install the SSDT that implements the Address Translation DSM.

  @retval EFI_SUCCESS           The SSDT was successfully installed.
  @retval EFI_OUT_OF_RESOURCES  There is not enough memory.
**/
EFI_STATUS
InstallSsdt (
  VOID
  )
{
  EFI_STATUS                  Status;
  EFI_ACPI_TABLE_PROTOCOL     *AcpiTable;
  EFI_ACPI_DESCRIPTION_HEADER *Table;
  UINTN                       TableSize;
  UINTN                       TableKey;

  RAS_DEBUG ((
    LEVEL_FUNC_FLOW,
    "[RAS ACPI] Enter InstallSsdt\n"
    ));

  Status = gBS->LocateProtocol (
    &gEfiAcpiTableProtocolGuid,
    NULL,
    (VOID **) &AcpiTable
    );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = GetSectionFromFv (
    &gEfiCallerIdGuid,
    EFI_SECTION_RAW,
    0,
    (VOID **) &Table,
    &TableSize
    );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = PatchSsdt (Table);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  TableKey = 0;
  Status = AcpiTable->InstallAcpiTable (
                        AcpiTable,
                        Table,
                        TableSize,
                        &TableKey
                        );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  RAS_DEBUG ((
    LEVEL_FUNC_FLOW,
    "[RAS ACPI] Exit InstallSsdt: %r\n",
    Status
    ));

  return Status;
} // InstallSsdt


/**
  The driver entry point.

  @param[in]  ImageHandle       The firmware allocated handle for the EFI image.
  @param[in]  SystemTable       A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The entry point executed successfully.
  @retval EFI_UNSUPPORTED       One or more SW SMIs are invalid or in use.
  @retval EFI_DEVICE_ERROR      One or more SW SMIs could not be enabled.
  @retval EFI_OUT_OF_RESOURCES  There is not enough memory to register SW SMIs.
--*/
EFI_STATUS
EFIAPI
RasAcpiEntry (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS                      Status;
  BOOLEAN                         NewAllocation;

  RAS_DEBUG ((
    LEVEL_FUNC_FLOW,
    "[RAS ACPI] Enter RasAcpiEntry\n"
    ));

  mRasAcpiParam = GetRasGlobalDataEfiReservedMemory (
                    L"RasAcpiParam",
                    sizeof (RAS_ACPI_PARAM),
                    &NewAllocation);

  if (mRasAcpiParam == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  RAS_DEBUG ((
    LEVEL_FUNC_FLOW,
    "[RAS ACPI] Handshake buffer address: %x\n", mRasAcpiParam
    ));

  Status = InstallSsdt ();

  RAS_DEBUG ((
    LEVEL_FUNC_FLOW,
    "[RAS ACPI] Exit RasAcpiEntry: %r\n",
    Status
  ));

  return Status;
}
