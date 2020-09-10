/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1996 - 2020 Intel Corporation. <BR>

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
#include <Acpi/Cedt.h>
#include <Library/KtiApi.h>

extern BIOS_ACPI_PARAM             *mAcpiParameter;
extern EFI_IIO_UDS_PROTOCOL        *mIioUds;

/**
  Dump CEDT Header

  @param [in]      HdrPtr       Pointer to CEDT Header

  @retval None

**/
VOID
DumpCedtHeader (
  EFI_ACPI_DESCRIPTION_HEADER *HdrPtr
  )
{
  DEBUG((EFI_D_INFO, "=========== CEDT header ==========\n"));
  DEBUG((EFI_D_INFO, " Signature:          %.4a\n",     (CHAR8 *)&HdrPtr->Signature));
  DEBUG((EFI_D_INFO, " Length:             0x%8X\n",    HdrPtr->Length));
  DEBUG((EFI_D_INFO, " Revision:           0x%2X\n",    HdrPtr->Revision));
  DEBUG((EFI_D_INFO, " Checksum:           0x%2X\n",    HdrPtr->Checksum));
  DEBUG((EFI_D_INFO, " OemId:              %.6a\n",     (CHAR8 *)&HdrPtr->OemId));
  DEBUG((EFI_D_INFO, " OemTableId:         %.8a\n",     (CHAR8 *)&HdrPtr->OemTableId));
  DEBUG((EFI_D_INFO, " OemRevision:        0x%8X\n",    HdrPtr->OemRevision));
  DEBUG((EFI_D_INFO, " CreatorId:          %.4a\n",     (CHAR8 *)&HdrPtr->CreatorId));
  DEBUG((EFI_D_INFO, " CreatorRevision:    0x%8X\n",    HdrPtr->CreatorRevision));
  DEBUG((EFI_D_INFO, "==================================\n"));
  DEBUG((EFI_D_INFO, "\n"));
}

/**
  Dump CHBS Structure

  @param [in]      ChbsPtr        Pointer to CHBS Structure

  @retval None

**/
VOID
DumpChbs (
  EFI_ACPI_CXL_HOST_BRIDGE_STRUCTURE  *Chbs
  )
{
  DEBUG((EFI_D_INFO, "=========== CHBS Table =============================\n"));
  DEBUG((EFI_D_INFO, " Type:               0x%2X\n",    Chbs->Type));
  DEBUG((EFI_D_INFO, " Length:             0x%4X\n",    Chbs->Length));
  DEBUG((EFI_D_INFO, " UID:                0x%8X\n",    Chbs->Uid));
  DEBUG((EFI_D_INFO, " Version:            0x%8X\n",    Chbs->Version));
  DEBUG((EFI_D_INFO, " RcrbSize:           0x%8X\n",    Chbs->RcrbSize));
  DEBUG((EFI_D_INFO, " RcrbBase:           0x%16X\n",   Chbs->RcrbBase));
  DEBUG((EFI_D_INFO, "=====================================================\n"));
  DEBUG((EFI_D_INFO, "\n"));
}


/**
  Dump CEDT table

  @param [in]      CedtAcpiTable       Pointer to CEDT table.

  @retval None

**/
VOID
DumpCedt (
  EFI_ACPI_CEDT_DESCRIPTION_TABLE *CedtAcpiTable
  )
{
  UINT8   Index;
  UINT8   ChbsEntries;

  DumpCedtHeader(&CedtAcpiTable->Header);

  if (CedtAcpiTable->Header.Length != sizeof(EFI_ACPI_DESCRIPTION_HEADER)) {
    ChbsEntries = (UINT8)((CedtAcpiTable->Header.Length - sizeof(EFI_ACPI_DESCRIPTION_HEADER)) / sizeof(EFI_ACPI_CXL_HOST_BRIDGE_STRUCTURE));
    for (Index = 0; Index < ChbsEntries; Index++) {
      DumpChbs (&CedtAcpiTable->CxlHostBridgeStructure[Index]);
    }
  }
}

/**

    Update the CEDT ACPI table

    @param *TableHeader   - The table to be set

    @retval EFI_SUCCESS -  Returns Success

**/
EFI_STATUS
PatchCedtAcpiTable (
  IN OUT   EFI_ACPI_COMMON_HEADER   *Table
  )
{
  UINT8             Socket = 0;
  UINT8             Stack = 0;
  UINT8             Entry = 0;
  UINTN             Idx = 0;
  UINT8             Checksum = 0;
  EFI_ACPI_CEDT_DESCRIPTION_TABLE *CedtAcpiTable;

  CedtAcpiTable = (EFI_ACPI_CEDT_DESCRIPTION_TABLE *)Table;

  for(Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for(Stack = 0; Stack < MAX_IOU_PER_SOCKET; Stack++) {
      if (GetCxlStatus (Socket, Stack) == AlreadyInCxlMode) {
        //Init CHBS entry.
        CedtAcpiTable->CxlHostBridgeStructure[Entry].Type = EFI_ACPI_CXL_HOST_BRIDGE_STRUCTURE_TYPE;
        CedtAcpiTable->CxlHostBridgeStructure[Entry].Length = EFI_ACPI_CXL_HOST_BRIDGE_STRUCTURE_LENGTH;
        CedtAcpiTable->CxlHostBridgeStructure[Entry].Uid = Socket * MAX_LOGIC_IIO_STACK + Stack;
        CedtAcpiTable->CxlHostBridgeStructure[Entry].Version = EFI_ACPI_CXL_SPECIFICATION_REVISION_1P1;
        CedtAcpiTable->CxlHostBridgeStructure[Entry].RcrbSize = EFI_ACPI_CXL_1P1_RCRB_SIZE;
        CedtAcpiTable->CxlHostBridgeStructure[Entry].RcrbBase = mIioUds->IioUdsPtr->PlatformData.IIO_resource[Socket].StackRes[Stack].Mmio32Base;
        Entry++;
      }
    }
  }

  CedtAcpiTable->Header.Length = Entry * sizeof(EFI_ACPI_CXL_HOST_BRIDGE_STRUCTURE) + sizeof(EFI_ACPI_DESCRIPTION_HEADER);

  //update Checksum
  for(Idx = 0; Idx < sizeof(EFI_ACPI_CEDT_DESCRIPTION_TABLE); Idx++) {
    Checksum = Checksum + (UINT8) (((UINT8 *)(CedtAcpiTable))[Idx]);
  }
  CedtAcpiTable->Header.Checksum = (UINT8) (0 - Checksum);

  DumpCedt (CedtAcpiTable);
  return EFI_SUCCESS;

}

