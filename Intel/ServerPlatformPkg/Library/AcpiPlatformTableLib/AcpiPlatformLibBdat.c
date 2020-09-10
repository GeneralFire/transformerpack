/** @file
  ACPI Platform Driver Hooks

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1996 - 2019 Intel Corporation. <BR>

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
#include <Library/CrcLib.h>
#include <BdatSchema.h>

extern struct SystemMemoryMapHob   *mSystemMemoryMap;
extern EFI_IIO_UDS_PROTOCOL        *mIioUds;
extern EFI_CPU_CSR_ACCESS_PROTOCOL *mCpuCsrAccess;

#include <Acpi/Bdat.h>

#ifndef MAX_HOB_ENTRY_SIZE
#define MAX_HOB_ENTRY_SIZE  60*1024
#endif

/**
  Get Number of Schemas From BSSA HOB

  @retval UINT16 - Number of BSSA Schemas
**/
UINT16
GetNumberOfSchemasFromBssaHob (
  VOID
  )
{
  UINT32 GuidIdx = 0;
  UINT16 NumberOfBssaSchemas = 0;

  ASSERT_EFI_ERROR (mSystemMemoryMap != NULL);
  for (GuidIdx = 0; GuidIdx < mSystemMemoryMap->bssaNumberDistinctGuids; GuidIdx++) {
//No. of HOBS per GUID added up for all GUIDs created from calls to saveToBdat()
    NumberOfBssaSchemas += mSystemMemoryMap->bssaNumberHobs[GuidIdx];
    DEBUG ((EFI_D_INFO, "GuidIdx = %d, total num hobs: %d\n", GuidIdx,  mSystemMemoryMap->bssaNumberHobs[GuidIdx]));
  }
  return NumberOfBssaSchemas;
}

/**
  Create BDAT Header with necessary information.
  Allocate memory with BdatSize and if failure return status.
  If Success return the pointer address for copying the schema information

  @param[out] BdatHeaderStructPtr - Pointer to BDAT Structure
  @param[in]  BdatSize            - Size of BDAT Structure

  @retval EFI_SUCCESS  - BDAT Structure created successfully
  @retval !EFI_SUCCESS - BDAT structure creation failed
**/
EFI_STATUS
CreateBdatHeader (
  OUT BDAT_STRUCTURE **BdatHeaderStructPtr,
  IN UINT32 BdatSize
  )
{
  EFI_TIME                      EfiTime;
  UINT64                        Address = 0xffffffff;
  EFI_STATUS                    Status = EFI_SUCCESS;

  //Allocating RealTime Memory for BDAT.

  Status = gBS->AllocatePages (
    AllocateMaxAddress,
    EfiACPIMemoryNVS,
    EFI_SIZE_TO_PAGES(BdatSize),
    &Address
  );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  // The memory location where the HOB's are to be copied to
  ZeroMem((VOID *)Address, BdatSize);

  *BdatHeaderStructPtr = (BDAT_STRUCTURE *)Address;

  DEBUG((EFI_D_ERROR, "\nBDAT Allocated Address = %x\n", Address));

  //
  // Create BIOS Data Signature
  //
  (*BdatHeaderStructPtr)->BdatHeader.BiosDataSignature[0] = 'B';
  (*BdatHeaderStructPtr)->BdatHeader.BiosDataSignature[1] = 'D';
  (*BdatHeaderStructPtr)->BdatHeader.BiosDataSignature[2] = 'A';
  (*BdatHeaderStructPtr)->BdatHeader.BiosDataSignature[3] = 'T';
  (*BdatHeaderStructPtr)->BdatHeader.BiosDataSignature[4] = 'H';
  (*BdatHeaderStructPtr)->BdatHeader.BiosDataSignature[5] = 'E';
  (*BdatHeaderStructPtr)->BdatHeader.BiosDataSignature[6] = 'A';
  (*BdatHeaderStructPtr)->BdatHeader.BiosDataSignature[7] = 'D';
  //
  // Structure size
  //
  (*BdatHeaderStructPtr)->BdatHeader.BiosDataStructSize = BdatSize;
  //
  // Primary Version
  //
  (*BdatHeaderStructPtr)->BdatHeader.PrimaryVersion = BDAT_PRIMARY_VER;
  //
  // Secondary Version
  //
  (*BdatHeaderStructPtr)->BdatHeader.SecondaryVersion = BDAT_SECONDARY_VER;
  //
  // CRC16 value of the BDAT_STRUCTURE
  //
  (*BdatHeaderStructPtr)->BdatHeader.Crc16 = 0;
  Status = CalculateCrc16(
    (VOID *)(*BdatHeaderStructPtr),
    BdatSize,
    &(*BdatHeaderStructPtr)->BdatHeader.Crc16
  );
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    (*BdatHeaderStructPtr)->BdatHeader.Crc16 = 0xFFFF;
  }
  (*BdatHeaderStructPtr)->BdatSchemas.SchemaListLength = GetNumberOfSchemasFromBssaHob();;
  (*BdatHeaderStructPtr)->BdatSchemas.Reserved = 0;
  (*BdatHeaderStructPtr)->BdatSchemas.Reserved1 = 0;
  //Initialize the Time parameters in the SCHEMA_LIST_STRUCTURE
  Status = gRT->GetTime(&EfiTime, NULL);
  if (!EFI_ERROR(Status)) {
    (*BdatHeaderStructPtr)->BdatSchemas.Year = EfiTime.Year;
    (*BdatHeaderStructPtr)->BdatSchemas.Month = EfiTime.Month;
    (*BdatHeaderStructPtr)->BdatSchemas.Day = EfiTime.Day;
    (*BdatHeaderStructPtr)->BdatSchemas.Hour = EfiTime.Hour;
    (*BdatHeaderStructPtr)->BdatSchemas.Minute = EfiTime.Minute;
    (*BdatHeaderStructPtr)->BdatSchemas.Second = EfiTime.Second;
  }
  return Status;
}

/**
  Dump BDAT Table to serial log

  @param[out] BdatHeaderStructPtr - Pointer to BDAT Structure
  @param[in]  BdatSize            - Size of BDAT Structure

  @retval None
**/
VOID
DumpBdatTable(
  IN BDAT_STRUCTURE **BdatHeaderStructPtr,
  IN UINT32 BdatSize
)
{
  UINT32 i = 1;
  UINT8 *Table = NULL;

  Table = (UINT8 *)(*BdatHeaderStructPtr);
  DEBUG((EFI_D_INFO, "\nPrint BDAT Table"));
  while (i <= BdatSize) {
    DEBUG((EFI_D_INFO, "0x%02x", Table[i-1]));
    if ((i % 16) == 0) {
      DEBUG ((EFI_D_INFO, "\n"));
    } else {
      if (i != BdatSize) {
        DEBUG ((EFI_D_INFO, " "));
      } else {
        DEBUG ((EFI_D_INFO, "\n"));
      }
    }
    i++;
  }
  DEBUG ((EFI_D_INFO, "\n"));
}

/**
  Copy BDAT Table pointer to scratchpad 5 register

  @param[in] BdatAddress - Bdat Table Address to be copied to Scratchpad 5 register

  @retval None
**/
VOID
CopyBdatPointerToScratchPad5 (
  IN UINT64 BdatAddress
  )
{
  UINT8 Socket = 0;

  //Copy BDAT base address to ScratchPad5
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].Valid) {
      mCpuCsrAccess->WriteCpuCsr (Socket, 0, BIOSNONSTICKYSCRATCHPAD5_UBOX_MISC_REG, (UINT32)BdatAddress);
      DEBUG ((EFI_D_INFO, "Scratchpad_Debug PatchBdaAcpiTable: Verify Non Sticky Scratchpad5 0x%08x\n", BdatAddress));
    }
  }
}

/**

    Update the BDAT ACPI table: Multiple instances of the BDAT DATA HOB are placed into one contiguos memory range

    @param *TableHeader   - The table to be set

    @retval EFI_SUCCESS -  Returns Success

**/
EFI_STATUS
PatchBdatAcpiTable (
  IN OUT  EFI_ACPI_COMMON_HEADER  *Table
  )
/*++

  Routine Description:

    Update the BDAT ACPI table: Multiple instances of the BDAT DATA HOB are placed into one contiguos memory range

  Arguments:

    *TableHeader   - The table to be set

  Returns:

    EFI_SUCCESS -  Returns Success

--*/
{
  EFI_STATUS                        Status                      = EFI_SUCCESS;
  EFI_PHYSICAL_ADDRESS              Address                     = 0;
  UINT32                            idx                         = 0;
  UINT8                             checksum                    = 0;
  BDAT_STRUCTURE                    *BdatHeaderStructPtr        = NULL;
  UINT32                            *SchemaAddrLocationArray    = NULL;
  UINT32                            TotalBDATstructureSize      = 0;
  UINT32                            BdatHeaderSize              = 0;
  UINT32                            CurrentHobSize              = 0;
  EFI_HOB_GUID_TYPE                 *GuidHob                    = NULL;
  VOID                              *HobData                    = NULL;
  EFI_GUID                          gEfiMemoryMapDataHobBdatBssaGuid  = {0};
  UINT16                            NumberBssaSchemas           = 0;
  UINT32                            GuidIdx                     = 0;
  UINT32                            HobIdx                      = 0;
  UINT32                            CurrentSchema               = 0;
  UINT32                            PreviousSchemaSize          = 0;
  UINT32                            RemainingHobSizeBssaSchema  = 0;
  BDAT_SCHEMA_HEADER_STRUCTURE      *BssaSchemaHeaderPtr        = NULL;
  EFI_BDAT_ACPI_DESCRIPTION_TABLE   *BdatAcpiTable              = NULL;


  BdatAcpiTable = (EFI_BDAT_ACPI_DESCRIPTION_TABLE *)Table;
  DEBUG ((EFI_D_INFO, "\nPatchBdatAcpiTable Started\n"));

  NumberBssaSchemas = GetNumberOfSchemasFromBssaHob ();

  RemainingHobSizeBssaSchema = mSystemMemoryMap->bssaBdatSize + (NumberBssaSchemas * sizeof(BDAT_SCHEMA_HEADER_STRUCTURE)); //Total size of all HOBs created by SaveToBdat() + NumberBssaSchemas*headerPerSchema
  DEBUG ((EFI_D_INFO, "NumberBssaSchemas = %d, total schema size: %d\n", NumberBssaSchemas,  RemainingHobSizeBssaSchema));

  BdatHeaderSize = sizeof(BDAT_STRUCTURE) + (NumberBssaSchemas * (sizeof(UINT32)));

  TotalBDATstructureSize = BdatHeaderSize + RemainingHobSizeBssaSchema;
  DEBUG ((EFI_D_INFO, "Total BDAT size: %d\n", TotalBDATstructureSize));

  Status = CreateBdatHeader (&BdatHeaderStructPtr, TotalBDATstructureSize);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((EFI_D_INFO, "BdatRegionAddress = %x\n", (UINT64)BdatHeaderStructPtr));
  CopyBdatPointerToScratchPad5 ((UINT64)BdatHeaderStructPtr);

  Address = (EFI_PHYSICAL_ADDRESS) BdatHeaderStructPtr;

  //Saving the "Schema Offsets" from the BDAT base in RT Memory into the Schemas Array in BDAT_STRUCT"; BdatHeaderStructPtr -> Base Address of RT Memory Allocated
  SchemaAddrLocationArray = (UINT32 *)(Address + sizeof (BDAT_STRUCTURE));
  SchemaAddrLocationArray[0] = BdatHeaderSize;  //*(x+1)=x[1]. Offset of BDAT_STRUCTURE start from BDAT base
  PreviousSchemaSize = BdatHeaderSize;

  //
  // Update BDAT ACPI table
  //
  BdatAcpiTable->BdatGas.Address = (UINT64)BdatHeaderStructPtr;

  //
  //Saving to RT Memory BDAT Data received from HOBs generated due to call/calls to SaveToBdat()
  //
  CurrentSchema = 1;
  for (GuidIdx = 0; GuidIdx < mSystemMemoryMap->bssaNumberDistinctGuids; GuidIdx++) {
    gEfiMemoryMapDataHobBdatBssaGuid  = mSystemMemoryMap->bssaBdatGuid[GuidIdx]; //get first GUID instance
    GuidHob = GetFirstGuidHob (&gEfiMemoryMapDataHobBdatBssaGuid);
    DEBUG ((EFI_D_INFO, "Getting BIOS SSA result with GUID  %g\n", &gEfiMemoryMapDataHobBdatBssaGuid));
    for (HobIdx = 0; HobIdx < mSystemMemoryMap->bssaNumberHobs[GuidIdx]; HobIdx++) { //looping through all HOBs linked to that GUID
      ASSERT(GuidHob != NULL);
      if (GuidHob == NULL) {
        return EFI_NOT_FOUND;
      }
      HobData = GET_GUID_HOB_DATA (GuidHob);
      CurrentHobSize = GET_GUID_HOB_DATA_SIZE (GuidHob);
      DEBUG ((EFI_D_INFO, "Initial HOB size  %d; remaining HOB size %d\n", CurrentHobSize, RemainingHobSizeBssaSchema));
      //Setting the header first
      if (RemainingHobSizeBssaSchema < sizeof(BDAT_SCHEMA_HEADER_STRUCTURE)) {
        //Nothing we can do, break execution
        DEBUG ((EFI_D_INFO, "Not enough space to add schema header to BIOS SSA result\n"));
        RemainingHobSizeBssaSchema = 0;
        break;
      }
      //Each HOB has a header added to it (BDAT_SCHEMA_HEADER_STRUCTURE)
      Address = Address + (EFI_PHYSICAL_ADDRESS)PreviousSchemaSize;
      DEBUG ((EFI_D_INFO, "\nStarting to copy new Schema at Address = %lx\n", Address));

      BssaSchemaHeaderPtr = (BDAT_SCHEMA_HEADER_STRUCTURE *)Address;
      BssaSchemaHeaderPtr->SchemaId = gEfiMemoryMapDataHobBdatBssaGuid;
      RemainingHobSizeBssaSchema -= sizeof(BDAT_SCHEMA_HEADER_STRUCTURE);
      Address = Address + sizeof(BDAT_SCHEMA_HEADER_STRUCTURE);
      //CRC16 value of the BDAT_SCHEMA_HEADER_STRUCTURE
      BssaSchemaHeaderPtr->Crc16 = 0;
      Status = CalculateCrc16 (
        (VOID *) BssaSchemaHeaderPtr,
        sizeof (BDAT_SCHEMA_HEADER_STRUCTURE),
        &BssaSchemaHeaderPtr->Crc16
        );
      ASSERT_EFI_ERROR (Status);
      if (EFI_ERROR (Status)) {
        BssaSchemaHeaderPtr->Crc16 = 0xFFFF;
      }

      if (RemainingHobSizeBssaSchema < CurrentHobSize) {
        DEBUG ((EFI_D_WARN, "Not enough space to add complete BIOS SSA result\n"));
        CurrentHobSize = RemainingHobSizeBssaSchema;
      }
      //HOB size won't overflow a UINT32.
      BssaSchemaHeaderPtr->DataSize = (UINT32)CurrentHobSize + sizeof(BDAT_SCHEMA_HEADER_STRUCTURE);
      DEBUG ((EFI_D_INFO, "Setting schema %g size to %d\n", &(BssaSchemaHeaderPtr->SchemaId), BssaSchemaHeaderPtr->DataSize));
      //HOB size won't overflow a UINT32.
      PreviousSchemaSize = (UINT32)CurrentHobSize + sizeof(BDAT_SCHEMA_HEADER_STRUCTURE);
      DEBUG ((EFI_D_INFO, "Copying %d bytes to 0x%x\n", CurrentHobSize, Address));

     //Copy HOB to RT Memory
      CopyMem ((VOID *)Address, (VOID *)HobData, (UINT32)CurrentHobSize);
      //HOB size won't overflow a UINT32.
      DEBUG ((EFI_D_INFO, "HOB size  %d; remaining HOB size %d\n", CurrentHobSize, RemainingHobSizeBssaSchema));
      RemainingHobSizeBssaSchema -= (UINT32)CurrentHobSize;
      if (RemainingHobSizeBssaSchema == 0) {
        break;
      }
      GuidHob = GET_NEXT_HOB (GuidHob); // Increment to next HOB
      GuidHob = GetNextGuidHob (&gEfiMemoryMapDataHobBdatBssaGuid, GuidHob);  // Now search for next instance of the BDAT HOB
      if (GuidHob == NULL) {
        break;
      }
      SchemaAddrLocationArray[CurrentSchema] = SchemaAddrLocationArray[CurrentSchema  -1] + PreviousSchemaSize;
      CurrentSchema++;
    }
    if (RemainingHobSizeBssaSchema == 0) {
      break;
    }
  }

  //
  // Update checksum
  //
  BdatAcpiTable->Header.Checksum = 0;
  checksum = 0;
  for(idx = 0; idx < sizeof(EFI_BDAT_ACPI_DESCRIPTION_TABLE); idx++) {
    checksum = checksum + (UINT8) (((UINT8 *)(BdatAcpiTable))[idx]);
  }
  BdatAcpiTable->Header.Checksum = (UINT8) (0 - checksum);

  DumpBdatTable (&BdatHeaderStructPtr, TotalBDATstructureSize);

  return Status;
}
