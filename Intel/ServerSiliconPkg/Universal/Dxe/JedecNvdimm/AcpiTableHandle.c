/** @file AcpiTableHandle.c
  ACPI Table Handling

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>

#include <Protocol/AcpiTable.h>

#include "AcpiTableHandle.h"
#include "JedecNvdimm.h"

#define AML_OPCODE_DEVICE           0x0E
#define AML_OPCODE_DWORD            0x0C
#define AML_OPCODE_NAME             0x08

typedef struct AML_ENTRY{
  UINT16    ExpectedOccurrences;
  UINT32    Signature;
  UINT8     OpCode;
} AML_ENTRY;


AML_ENTRY   AmlTable[] = {
              {.Signature = SIGNATURE_32 ('F', 'I', 'X', 'A'), .OpCode = AML_OPCODE_DWORD,  .ExpectedOccurrences = 1},
            };


EFI_STATUS
FindNextOccurrence (
  OUT UINT8   **TablePointer,
  UINT8       *TableEndPointer
)
{
  UINT8       *CurrentPointer;
  UINT32      Signature;
  UINT8       Index;

  for (CurrentPointer = *TablePointer; CurrentPointer <= TableEndPointer; ++CurrentPointer) {
    Signature = * (UINT32 *) CurrentPointer;
    for (Index = 0; Index < ARRAY_SIZE(AmlTable); Index++) {
      if (Signature == AmlTable[Index].Signature && CurrentPointer[-1] == AmlTable[Index].OpCode) {
        *TablePointer = CurrentPointer;
        return EFI_SUCCESS;
      }
    }
  }
  return EFI_NOT_FOUND;
}

EFI_STATUS
VerifyAmlStructure (
  UINT8       *TablePointer,
  UINT8       *TableEndPointer
)
{
  UINT8       FoundOccurrences[ARRAY_SIZE(AmlTable)] = {0};
  UINT8       *CurrentPointer;
  UINT32      Signature;
  UINT8       Index;


  // Scan whole AML to check if AML has expected structure.
  // If not, leave function with EFI_UNSUPPORTED, not patching anything.

  for (CurrentPointer = TablePointer; CurrentPointer <= TableEndPointer; ++CurrentPointer) {
    Signature = * (UINT32 *) CurrentPointer;
    for (Index = 0; Index < ARRAY_SIZE(AmlTable); Index++) {
      if (Signature == AmlTable[Index].Signature && CurrentPointer[-1] == AmlTable[Index].OpCode) {
        FoundOccurrences[Index]++;
      }
    }
  }

  for (Index = 0; Index < ARRAY_SIZE (FoundOccurrences); Index++) {
    if (FoundOccurrences[Index] != AmlTable[Index].ExpectedOccurrences) {
      DEBUG((DEBUG_ERROR, "Failed, Signature: '%c%c%c%c'\n",
            ((CHAR8*)&AmlTable[Index].Signature)[0], ((CHAR8*)&AmlTable[Index].Signature)[1],
            ((CHAR8*)&AmlTable[Index].Signature)[2], ((CHAR8*)&AmlTable[Index].Signature)[3]));
      DEBUG((DEBUG_ERROR, "Found: %d Expected: %d\n", FoundOccurrences[Index], AmlTable[Index].ExpectedOccurrences));
      return EFI_UNSUPPORTED;
    }
  }
  return EFI_SUCCESS;
}

/**
  Patch SSDTNVDR table.

  Function is an implementation of a naive byte-scanner, which firstly verifies if table AML
  has correct structure, comparing actual structure to predefined expected values that are stored
  in AmlTable array. If AML structure is correct, then actual patching begins.

**/
EFI_STATUS
PatchAcpiSsdtNvdrTable (
  IN OUT   EFI_ACPI_COMMON_HEADER        *TableHeader
  )
{
  UINT8                 *TableBeginPointer;
  UINT8                 *TableEndPointer;
  UINT8                 *CurrentPointer;
  UINT32                Signature;
  EFI_STATUS            Status;

  TableBeginPointer     = (UINT8 *) (TableHeader);
  TableEndPointer       = (TableBeginPointer + ((EFI_ACPI_COMMON_HEADER *) TableBeginPointer)->Length);

  Status = VerifyAmlStructure (TableBeginPointer, TableEndPointer);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "Correct structure of AML.\n"));

  CurrentPointer = TableBeginPointer;
  while(EFI_SUCCESS == FindNextOccurrence (&CurrentPointer, TableEndPointer)) {
    Signature = *(UINT32 *) CurrentPointer;

    // Notice: CurrentPointer is set in each case to point directly to value that is required by proper patching function.
    switch (Signature)
    {
      case (SIGNATURE_32 ('F', 'I', 'X', 'A')):
        *(UINT32 *) CurrentPointer = (UINT32) (UINTN) (UINT64)mNvdimmAcpiSmmInterface;
        break;
      default:
        break;
    }
    CurrentPointer = CurrentPointer + sizeof(Signature);
  }

  return EFI_SUCCESS;
}


/**
  Function finds SSDTNVDR table, then runs patching. If both were successful,
  then the table is installed.
**/
EFI_STATUS
LoadAcpiSsdtNvdrTable(
  VOID
)
{
  EFI_ACPI_TABLE_PROTOCOL        *AcpiTableProtocol;
  UINTN                          SsdtNvdrTableKey;
  UINTN                          SectionInstance;
  EFI_ACPI_DESCRIPTION_HEADER    *TableHeader;
  UINTN                          TableSize;
  EFI_STATUS                     Status;
  EFI_ACPI_COMMON_HEADER         *Table;


  Status          = EFI_SUCCESS;
  SectionInstance = 0;

  //
  // Locate the EFI_ACPI_TABLE_PROTOCOL.
  //
  Status = gBS->LocateProtocol (
                  &gEfiAcpiTableProtocolGuid,
                  NULL,
                  (VOID **)&AcpiTableProtocol
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Scan all the EFI raw section instances in FV to find the NVDIMM root device SSDT.
  //
  DEBUG((DEBUG_INFO, "Start searching for SSDTNVDR...\n"));

  while (TRUE) {
    Status = GetSectionFromFv (
               &gEfiCallerIdGuid,
               EFI_SECTION_RAW,
               SectionInstance,
               (VOID **) &Table,
               &TableSize
               );

    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "No more tables found.\n"));
      break;
    }
    TableHeader = (EFI_ACPI_DESCRIPTION_HEADER *) (Table);

    DEBUG((DEBUG_INFO, "Found table: TableHeader->OemTableId = %lx\n", TableHeader->OemTableId));

    if (TableHeader->OemTableId == SIGNATURE_64 ('S', 'S', 'D', 'T', 'N', 'V', 'D', 'R')) {
      DEBUG((DEBUG_INFO, "Found SSDTNVDR table\n"));

      Status = PatchAcpiSsdtNvdrTable (Table);

      if (!EFI_ERROR (Status)) {

        Status = AcpiTableProtocol->InstallAcpiTable (
                                      AcpiTableProtocol,
                                      Table,
                                      TableSize,
                                      &SsdtNvdrTableKey
                                      );
          if (!EFI_ERROR (Status)) {
            DEBUG ((DEBUG_INFO, "Successful installation of SSDTNVDR.\n"));
          }
      }
      else{
        DEBUG ((DEBUG_ERROR, "Error during patching SSDTNVDR table.\n"));
      }
      FreePool (Table);
      return Status;
    }
    FreePool (Table);
    SectionInstance++;
  }

  return Status;
}
