/** @file
  Implementation of whea support library on top of whea support protocol.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#include <Library/SmmServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/RasGlobalDataLib.h>
#include <Library/RasDebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/WheaSupportLib.h>
#include <Protocol/AcpiTable.h>
// APTIOV_SERVER_OVERRIDE_RC_START: Added support to Log LastBoot Errors to BMC SEL/Gpnv using RtErrorLog Listener
#include <RuntimeErrorLog/RtErrorLog.h>
// APTIOV_SERVER_OVERRIDE_RC_END: Added support to Log LastBoot Errors to BMC SEL/Gpnv using RtErrorLog Listener


#define EFI_ACPI_CREATOR_ID                 0x4C544E49  // "INTL"
#define EFI_ACPI_CREATOR_REVISION           0x00000001
#define EFI_WHEA_OEM_REVISION               0x0001;

//
// Maximum buffer size for WHEA ACPI tables
//
#define MAX_HEST_SIZE               0x1000
#define MAX_BOOT_ERROR_LOG_SIZE     0x8000
#define MAX_ERST_SIZE               0x1000
#define MAX_EINJ_SIZE               0x2000

BOOLEAN  EINJTableInstalled = FALSE;
BOOLEAN  HESTInstalled      = FALSE;

//
// Global variable to cache pointer to SMM Whea support protocol.
//
EFI_ACPI_TABLE_PROTOCOL         *mAcpiTbl = NULL;

VOID
EFIAPI
DumpACPITable (
    EFI_ACPI_DESCRIPTION_HEADER *TableHeader
  )
{
  UINT32 i;
  UINT32 Length = TableHeader->Length;
  UINT8 *Buffer = (UINT8 *)TableHeader;

  RAS_DEBUG((LEVEL_BASIC_FLOW, " dump ACPI table  \n"));

  for (i = 0; i < Length; i++) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "%x  ", Buffer[i]));
    if ((i % 16 == 0) && (i != 0)) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "\n"));
    }
  }
  RAS_DEBUG((LEVEL_BASIC_FLOW, "\n"));
}


VOID
EFIAPI
UpdateAcpiTableIds (
  EFI_ACPI_DESCRIPTION_HEADER *TableHeader
  )
{
  UINT64  TempOemTableId;

  //
  // Update the OEMID, Creator ID, and Creator revision.
  //
  *(UINT32 *) (TableHeader->OemId) = 'I' + ('N' << 8) + ('T' << 16) + ('E' << 24);
  *(UINT16 *) (TableHeader->OemId + 4) = 'L' + (' ' << 8);

  //
  // Update the OEMID and OEM Table ID.
  //
  TempOemTableId = PcdGet64 (PcdAcpiDefaultOemTableId);

  CopyMem (TableHeader->OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (TableHeader->OemId));
  CopyMem (&TableHeader->OemTableId, &TempOemTableId, sizeof (TableHeader->OemTableId));

  TableHeader->CreatorId = EFI_ACPI_CREATOR_ID;
  TableHeader->CreatorRevision = EFI_ACPI_CREATOR_REVISION;

  //
  // Update OEM revision and OEM TABLE ID based on the platform/SKU
  //
  TableHeader->OemRevision = EFI_WHEA_OEM_REVISION;
}


VOID
EFIAPI
InstallAcpiWheaTables (
  IN EFI_ACPI_DESCRIPTION_HEADER  *ACPITableHeader
  )
{
  EFI_STATUS                      Status;
  UINTN                           TableKey;

  RAS_DEBUG((LEVEL_BASIC_FLOW, "[WHEAINIT] start to install WHEA ACPI tables \n"));

  TableKey = 0;
  Status = mAcpiTbl->InstallAcpiTable (mAcpiTbl, ACPITableHeader, ACPITableHeader->Length, &TableKey);

  RAS_DEBUG((LEVEL_BASIC_FLOW, "[WHEAINIT] install WHEA ACPI tables status:%r \n", Status));
}


/**
  The constructor function caches the pointer to WHEA support protocol.

  The constructor function locates SMM WEHA support  protocol from protocol database.
  It will ASSERT() if that operation fails and it will always return EFI_SUCCESS.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
InitializeWheaSupportLib (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
{
  EFI_STATUS                      Status;
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, &mAcpiTbl);

  if (EFI_ERROR (Status)) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "[WHEAINIT] Fail to locate gEfiAcpiTableProtocolGuid: %r \n", Status));
  }

  return EFI_SUCCESS;
}


/**
  This routine is to add error source in HEST table. And return reference buffer address.


  @param  [in]  ErrorSource -- point to error source
  @param  [in]  InstallACPIFlag -- TRUE: install HEST ACPI table; FALSE: don't install.
  @param  [out] BufferAddress  -- buffer address in reserved area for the specific error source.

  @return none.
**/
VOID
EFIAPI
AddHESTErrorSource (
  IN  VOID                                                 *ErrorSource,
  IN  UINT32                                                Size,
  IN  BOOLEAN                                               InstallACPIFlag,
  OUT UINT64                                               *BufferAddress
  )
{
  EFI_ACPI_6_2_HARDWARE_ERROR_SOURCE_TABLE_HEADER       *Hest;
  EFI_ACPI_6_2_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE  *ErrorDest;
  UINT8                                                 *Buffer = 0;
  EFI_STATUS                                             Status;
  BOOLEAN                                               NewAllocation;

  Hest = GetRasGlobalData (L"WHEAHESTData", MAX_HEST_SIZE, &NewAllocation);

  if (Hest == NULL) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "[HEST] ERROR: GetRasGlobalData returned NULL pointer\n"));
    RAS_ASSERT (FALSE);
    return;
  }

  if (NewAllocation) {
    Hest->Header.Signature = EFI_ACPI_6_2_HARDWARE_ERROR_SOURCE_TABLE_SIGNATURE;
    Hest->Header.Length = sizeof (EFI_ACPI_6_2_HARDWARE_ERROR_SOURCE_TABLE_HEADER);
    Hest->Header.Revision = EFI_ACPI_6_2_HARDWARE_ERROR_SOURCE_TABLE_REVISION;
    Hest->ErrorSourceCount = 0;
    UpdateAcpiTableIds (&Hest->Header);
  }

  if (HESTInstalled) {
    return;
  }

  if (ErrorSource != NULL && Size != 0) {
    if ((Hest->Header.Length + Size) > MAX_HEST_SIZE) {
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "[HEST] HEST table size is too small to add new source!\n"));
      RAS_ASSERT (FALSE);
      return;
    }

    //
    //point to end of current table.
    //
    ErrorDest = (EFI_ACPI_6_2_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE *) (VOID *) ((UINT8 *) Hest + Hest->Header.Length);
    CopyMem (ErrorDest, ErrorSource, Size);

    if (EFI_ACPI_6_2_GENERIC_HARDWARE_ERROR == ErrorDest->Type) {
      Status = gBS->AllocatePool (
                EfiReservedMemoryType,
                ERROR_LOG_BUFFER_SIZE,
                &Buffer
                );
      ZeroMem (Buffer, ERROR_LOG_BUFFER_SIZE);

      *(UINT64 *) Buffer = (UINT64) (UINTN) ((UINT8 *) Buffer + sizeof (UINT64));
      ErrorDest->ErrorStatusAddress.Address = (UINT64) (UINTN) Buffer;
      *BufferAddress = ErrorDest->ErrorStatusAddress.Address;
    }
    Hest->Header.Length += Size;
    ErrorDest->SourceId = (UINT16) Hest->ErrorSourceCount;
    Hest->ErrorSourceCount ++;
    RAS_DEBUG ((
      LEVEL_BASIC_FLOW,
      "[HEST] Current source counter:%d  table length:%x  buffer address:%lx\n",
      Hest->ErrorSourceCount,
      Hest->Header.Length,
      Buffer
      ));
  }

  if (InstallACPIFlag) {
    HESTInstalled = TRUE;
    DumpACPITable (&Hest->Header);
    InstallAcpiWheaTables (&Hest->Header);
  }
}


/**
  This routine returns BERT error address. This address is at reserved attribution address.


  @param [out] ErrorAddress    - error address of specific error type.

  @retval  VOID
**/
VOID
EFIAPI
GetBERTErrorAddress (
  OUT  UINT64                           *ErrorAddress
  )
{
  UINT8                                                 *Buffer;
  EFI_STATUS                                            Status;
  EFI_ACPI_6_2_BOOT_ERROR_RECORD_TABLE_HEADER           *BERTHeader;
  BOOLEAN                                               NewAllocation;
  EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE           *StatusBlock;

  BERTHeader = GetRasGlobalData (L"WHEABERTData", sizeof(EFI_ACPI_6_2_BOOT_ERROR_RECORD_TABLE_HEADER), &NewAllocation);

  if (BERTHeader == NULL) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "ERROR: GetRasGlobalData returned NULL pointer\n"));
    RAS_ASSERT (FALSE);
    return;
  }

  if (NewAllocation) {
    BERTHeader->Header.Signature = EFI_ACPI_6_2_BOOT_ERROR_RECORD_TABLE_SIGNATURE;
    BERTHeader->Header.Length = sizeof (EFI_ACPI_6_2_BOOT_ERROR_RECORD_TABLE_HEADER);
    BERTHeader->Header.Revision = EFI_ACPI_6_2_BOOT_ERROR_RECORD_TABLE_REVISION;
    UpdateAcpiTableIds (&BERTHeader->Header);

    Status = gBS->AllocatePool (
             EfiReservedMemoryType,
             MAX_BOOT_ERROR_LOG_SIZE,
             &Buffer
             );
    ZeroMem (Buffer, MAX_BOOT_ERROR_LOG_SIZE);

    BERTHeader->BootErrorRegionLength = MAX_BOOT_ERROR_LOG_SIZE;
    BERTHeader->BootErrorRegion = (UINT64) (UINTN) Buffer;
    StatusBlock = (EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE*) Buffer;
    StatusBlock->RawDataLength = 0;
    StatusBlock->RawDataOffset = MAX_BOOT_ERROR_LOG_SIZE;

    //
    // initially, there's no errors, so the severity should be None.
    //
    StatusBlock->ErrorSeverity = EFI_ACPI_6_2_ERROR_SEVERITY_NONE;

    DumpACPITable (&BERTHeader->Header);
    InstallAcpiWheaTables (&BERTHeader->Header);
  }

  *ErrorAddress = BERTHeader->BootErrorRegion;
}

/**
  This routine add EINJ error injection instruction entry. And it can install EINJ table.
  EINJ table only can install once.


  @param [in]  InjectionEntry    - pointer to EINJ error injection entry.
  @param [in]  InstallTableFlag  - TRUE --> Install EINJ table; FALSE --> don't install.

  @retval  VOID
**/
VOID
EFIAPI
AddEinjActionItem (
  IN   EFI_ACPI_6_2_EINJ_INJECTION_INSTRUCTION_ENTRY       *InjectionEntry,
  IN   BOOLEAN                                             InstallTableFlag
  )
{
  EFI_ACPI_6_2_ERROR_INJECTION_TABLE_HEADER             *EINJHeader;
  BOOLEAN                                               NewAllocation;

  EINJHeader = GetRasGlobalData (L"WHEAEinjData", MAX_EINJ_SIZE, &NewAllocation);

  if (EINJHeader == NULL) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "[WHEAEINJ] ERROR: GetRasGlobalData returned NULL pointer\n"));
    RAS_ASSERT (FALSE);
    return;
  }

  if (NewAllocation) {
    EINJHeader->Header.Signature = EFI_ACPI_6_2_ERROR_INJECTION_TABLE_SIGNATURE;
    EINJHeader->Header.Length = sizeof (EFI_ACPI_6_2_ERROR_INJECTION_TABLE_HEADER);
    EINJHeader->Header.Revision = EFI_ACPI_6_2_ERROR_INJECTION_TABLE_REVISION;
    UpdateAcpiTableIds (&EINJHeader->Header);

    EINJHeader->InjectionHeaderSize = sizeof (EFI_ACPI_6_2_ERROR_INJECTION_TABLE_HEADER);
    EINJHeader->InjectionFlags = 0;
    EINJHeader->InjectionEntryCount = 0;
  }

  if (EINJTableInstalled) {
    return;
  }

  CopyMem ((VOID *) (UINTN) ((UINT8 *) EINJHeader + EINJHeader->Header.Length), (VOID *) InjectionEntry, sizeof (EFI_ACPI_6_2_EINJ_INJECTION_INSTRUCTION_ENTRY));
  EINJHeader->Header.Length += sizeof (EFI_ACPI_6_2_EINJ_INJECTION_INSTRUCTION_ENTRY);
  EINJHeader->InjectionEntryCount ++;
  RAS_DEBUG((LEVEL_REG, "[WHEAEINJ] table length:%x  entry count:%d \n", EINJHeader->Header.Length, EINJHeader->InjectionEntryCount));

  if (InstallTableFlag) {
    EINJTableInstalled = TRUE;
    DumpACPITable (&EINJHeader->Header);
    InstallAcpiWheaTables (&EINJHeader->Header);
  }

}


/**
  This routine is to install ERST table.


  @param [in]  ERSTInstruction    - pointer to ERST instruction list.
  @param [in]  Size               - size of whole ERST instruction.

  @retval  VOID
**/
VOID
EFIAPI
AddERSTInstruction (
  IN  EFI_ACPI_6_2_ERST_SERIALIZATION_INSTRUCTION_ENTRY  *ERSTInstruction,
  IN  UINT32                                              Size
  )
{
  EFI_ACPI_6_2_ERROR_RECORD_SERIALIZATION_TABLE_HEADER  *ERSTHeader;
  BOOLEAN                                               NewAllocation;

  ERSTHeader = GetRasGlobalData (L"WHEAERSTData", MAX_ERST_SIZE, &NewAllocation);

  if (NewAllocation) {
    ERSTHeader->Header.Signature = EFI_ACPI_6_2_ERROR_RECORD_SERIALIZATION_TABLE_SIGNATURE;
    ERSTHeader->Header.Length = sizeof (EFI_ACPI_6_2_ERROR_RECORD_SERIALIZATION_TABLE_HEADER);
    ERSTHeader->Header.Revision = EFI_ACPI_6_2_ERROR_RECORD_SERIALIZATION_TABLE_REVISION;
    ERSTHeader->SerializationHeaderSize = sizeof (EFI_ACPI_6_2_ERROR_RECORD_SERIALIZATION_TABLE_HEADER);
    ERSTHeader->InstructionEntryCount = 0;
    UpdateAcpiTableIds (&ERSTHeader->Header);

    CopyMem ((VOID *) (UINTN) ((UINT8 *) ERSTHeader + ERSTHeader->Header.Length), ERSTInstruction, Size);
    ERSTHeader->Header.Length += Size;
    ERSTHeader->InstructionEntryCount += Size/sizeof (EFI_ACPI_6_2_ERST_SERIALIZATION_INSTRUCTION_ENTRY);

    DumpACPITable (&ERSTHeader->Header);
    InstallAcpiWheaTables (&ERSTHeader->Header);
  }

}


/**
  This routine is to log error to BERT table.

  @param [in]  CperHeader                    pointer to a CPER header, the CPER wraps the boot error details.
  @param [in]  BootErrorRegionAddress        address of the Boot Error Region memory block.

  @retval  EFI_STATUS
**/
EFI_STATUS
EFIAPI
LogErrorToBert (
  IN  EFI_COMMON_ERROR_RECORD_HEADER                     *CperHeader,
  IN  UINT64                                              BootErrorRegionAddress
  )
{
  EFI_COMMON_ERROR_RECORD_HEADER                         *ErrorRecordHeader;
  EFI_ERROR_SECTION_DESCRIPTOR                           *ErrorRecordDescription;
  VOID                                                   *ErrorRecord;
  EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE            *GenErrSts;
  EFI_ACPI_6_2_GENERIC_ERROR_DATA_ENTRY_STRUCTURE        *GenErrData;
  VOID                                                   *ErrorSection;
  BOOLEAN                                                ValidEntry;
  UINT32                                                 SectionSize;
  BOOLEAN                                                IsCeLogged = FALSE;    // No CE logged yet
  BOOLEAN                                                IsUceLogged = FALSE;

  ValidEntry = FALSE;
  SectionSize = 0;

  ErrorRecordHeader = CperHeader;

// APTIOV_SERVER_OVERRIDE_RC_START: Added support to Log LastBoot Errors to BMC SEL/Gpnv using RtErrorLog Listener
  CommonErrorHandling(ErrorRecordHeader, FALSE);
// APTIOV_SERVER_OVERRIDE_RC_END: Added support to Log LastBoot Errors to BMC SEL/Gpnv using RtErrorLog Listener

  //
  // Dump header
  //
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordHeader SignatureStart: 0x%X\n", ErrorRecordHeader->SignatureStart));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordHeader Revision: 0x%X\n", ErrorRecordHeader->Revision));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordHeader SignatureEnd: 0x%X\n", ErrorRecordHeader->SignatureEnd));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordHeader SectionCount: 0x%X\n", ErrorRecordHeader->SectionCount));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordHeader ErrorSeverity: 0x%X\n", ErrorRecordHeader->ErrorSeverity));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordHeader ValidationBits: 0x%X\n", ErrorRecordHeader->ValidationBits));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordHeader RecordLength: 0x%X\n", ErrorRecordHeader->RecordLength));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordHeader TimeStamp: 0x%lX\n", ErrorRecordHeader->TimeStamp));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordHeader PlatformID: 0x%g\n", ErrorRecordHeader->PlatformID));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordHeader CreatorID: 0x%g\n", ErrorRecordHeader->CreatorID));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordHeader NotificationType: 0x%g\n", ErrorRecordHeader->NotificationType));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordHeader RecordID: 0x%lX\n", ErrorRecordHeader->RecordID));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordHeader PersistenceInfo: 0x%lX\n", ErrorRecordHeader->PersistenceInfo));

  ErrorRecordDescription = (EFI_ERROR_SECTION_DESCRIPTOR *) ((UINT8 *) ErrorRecordHeader + sizeof (EFI_COMMON_ERROR_RECORD_HEADER));
  ErrorRecord = (UINT8 *) ErrorRecordDescription + sizeof (EFI_ERROR_SECTION_DESCRIPTOR);

  //
  // Dump each fields:
  //
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordDescription->SectionOffset: 0x%X\n", ErrorRecordDescription->SectionOffset));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordDescription->SectionLength: 0x%X\n", ErrorRecordDescription->SectionLength));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordDescription->Revision: 0x%X\n", ErrorRecordDescription->Revision));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordDescription->SecValidMask: 0x%X\n", ErrorRecordDescription->SecValidMask));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordDescription->SectionFlags: 0x%X\n", ErrorRecordDescription->SectionFlags));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordDescription->SectionType: 0x%g\n", ErrorRecordDescription->SectionType));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordDescription->FruId: 0x%g\n", ErrorRecordDescription->FruId));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordDescription->Severity: 0x%X\n", ErrorRecordDescription->Severity));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordDescription->FruString: 0x%a\n", ErrorRecordDescription->FruString));

  GenErrSts = (EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE*) (UINTN) BootErrorRegionAddress;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "[WheaLog]GenErrSts: %p\n", GenErrSts));
  if (GenErrSts == NULL) {
    return EFI_NOT_FOUND;
  }

  //
  // Pre-dump of the generic error status block's Block Status
  //
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrSts->BlockStatus.CorrectableErrorValid: 0x%X\n", GenErrSts->BlockStatus.CorrectableErrorValid));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrSts->BlockStatus.UncorrectableErrorValid: 0x%X\n", GenErrSts->BlockStatus.UncorrectableErrorValid));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrSts->BlockStatus.MultipleCorrectableErrors: 0x%X\n", GenErrSts->BlockStatus.MultipleCorrectableErrors));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrSts->BlockStatus.MultipleUncorrectableErrors: 0x%X\n", GenErrSts->BlockStatus.MultipleUncorrectableErrors));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrSts->BlockStatus.ErrorDataEntryCount: 0x%X\n", GenErrSts->BlockStatus.ErrorDataEntryCount));

  //
  // Calculate offset for next error data log
  //
  GenErrData = (EFI_ACPI_6_2_GENERIC_ERROR_DATA_ENTRY_STRUCTURE *) ((UINT8 *) GenErrSts + sizeof (EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE) + GenErrSts->DataLength);
  ErrorSection = (VOID *) ((UINT8 *) GenErrData + sizeof (EFI_ACPI_6_2_GENERIC_ERROR_DATA_ENTRY_STRUCTURE));

  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrData->SectionType: 0x%g\n", GenErrData->SectionType));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrData->ErrorSeverity: 0x%X\n", GenErrData->ErrorSeverity));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrData->Revision: 0x%X\n", GenErrData->Revision));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrData->ValidationBits: 0x%X\n", GenErrData->ValidationBits));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrData->Flags: 0x%X\n", GenErrData->Flags));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrData->ErrorDataLength: 0x%X\n", GenErrData->ErrorDataLength));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrData->FruId: 0x%g\n", GenErrData->FruId));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrData->FruText: 0x%a\n", GenErrData->FruText));

  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordDescription->SectionType: %g\n", ErrorRecordDescription->SectionType));

  //
  //decide the error section size based on the error type
  //
  if (CompareGuid (&ErrorRecordDescription->SectionType, &gEfiProcessorGenericErrorSectionGuid)) { //Processor Error Record
    SectionSize = sizeof (EFI_PROCESSOR_GENERIC_ERROR_DATA);
    ValidEntry = TRUE;
  } else if (CompareGuid (&ErrorRecordDescription->SectionType, &gEfiProcessorSpecificErrorSectionGuid)) {
    //
    // Currently not used
    //
  } else if (CompareGuid (&ErrorRecordDescription->SectionType, &gEfiPlatformMemoryErrorSectionGuid)) {//Platform Memory Error. Both corrected memory error and platform memory error have gEfiPlatformMemoryErrorSectionGuid, but there's only report function for platform memory error. so here it must be platform memory error.
    SectionSize = sizeof (EFI_PLATFORM_MEMORY_ERROR_DATA);
    ValidEntry = TRUE;
  } else if (CompareGuid (&ErrorRecordDescription->SectionType, &gEfiDirectedIoDMArErrorSectionGuid)) {
    //
    // Currently not used
    //
  } else if (CompareGuid (&ErrorRecordDescription->SectionType, &gEfiPcieErrorSectionGuid)) {// Pcie Error
    SectionSize = sizeof (EFI_PCIE_ERROR_DATA);
    ValidEntry = TRUE;
  } else if (CompareGuid (&ErrorRecordDescription->SectionType, &gEfiFirmwareErrorSectionGuid)) { // Firmware Error
    SectionSize = ErrorRecordDescription->SectionLength;
    ValidEntry = TRUE;
  }

  RAS_DEBUG((LEVEL_FUNC_FLOW, "ValidEntry: %X\n", ValidEntry));

  if (!ValidEntry) {
    return EFI_NOT_FOUND;
  }

  //
  //size check, no need to check count
  //
  if (GenErrSts->DataLength + sizeof (EFI_ACPI_6_2_GENERIC_ERROR_DATA_ENTRY_STRUCTURE) + SectionSize > MAX_BOOT_ERROR_LOG_SIZE) {
    RAS_DEBUG ((
      LEVEL_FUNC_FLOW,
      "ERROR: Requested size %x exceeds region max size %x\n",
      GenErrSts->DataLength + sizeof (EFI_ACPI_6_2_GENERIC_ERROR_DATA_ENTRY_STRUCTURE) + SectionSize,
      MAX_BOOT_ERROR_LOG_SIZE
      ));
    return EFI_UNSUPPORTED;
  }

  //
  // check error severity
  // APEI spec defines error severities as below:
  //    - Corrected
  //    - Uncorrected
  //         -- Fatal (i.e. Uncorrectable)
  //         -- Correctable
  // So ultimately, there are 3 different error severities: corrected, fatal, correctable.
  // But the BlockStatus field only contains 2 fields to record the error severity:
  //    - BlockStatus.UncorrectableErrorValid
  //    - BlockStatus.CorrectableErrorValid
  // Spec may actually mean Corrected/Uncorrected but have chosen the wrong words Correctable/Uncorrectable.
  //
  if (IsHigherSeverity (GenErrSts->ErrorSeverity, ErrorRecordDescription->Severity)) {
    GenErrSts->ErrorSeverity = ErrorRecordDescription->Severity;
  }
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "ErrorRecordDescription->Severity: 0x%X\n", ErrorRecordDescription->Severity));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "GenErrSts->ErrorSeverity: 0x%X\n", GenErrSts->ErrorSeverity));

  if ((ErrorRecordDescription->Severity == EFI_ACPI_6_2_ERROR_SEVERITY_FATAL) || (ErrorRecordDescription->Severity == EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTABLE)) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, " --Logging Uncorrected Error to BERT\n"));//if we log fatal and correctable errors here, the info should be Uncorrected, not Uncorrectable.
    IsUceLogged = TRUE;
    if (GenErrSts->BlockStatus.UncorrectableErrorValid == 0) {
      GenErrSts->BlockStatus.UncorrectableErrorValid = 1;
    } else {
      GenErrSts->BlockStatus.MultipleUncorrectableErrors = 1;
    }
  } else {
    RAS_DEBUG((LEVEL_BASIC_FLOW, " --Logging Corrected Error to BERT\n"));// Correctable -> Corrected
    IsCeLogged = TRUE;
    if (GenErrSts->BlockStatus.CorrectableErrorValid == 0) {
      GenErrSts->BlockStatus.CorrectableErrorValid = 1;
    } else {
      GenErrSts->BlockStatus.MultipleCorrectableErrors = 1;
    }
  }

  //
  //fill the generic error status block
  //
  GenErrSts->DataLength += sizeof (EFI_ACPI_6_2_GENERIC_ERROR_DATA_ENTRY_STRUCTURE) + SectionSize;// only the length of the generic error data
  GenErrSts->BlockStatus.ErrorDataEntryCount++; // multiple errors CAN be logged into HEST.

  //
  //fill the error data entry header fields
  //
  GenErrData->ErrorDataLength = SectionSize; // error data entry
  GenErrData->ErrorSeverity = ErrorRecordDescription->Severity;
  GenErrData->Revision = ErrorRecordDescription->Revision;
  GenErrData->Flags |= BIT0; // Primary // BIT0==1, FRUID is valid
  CopyGuid ((EFI_GUID *) (VOID *) &GenErrData->SectionType, &ErrorRecordDescription->SectionType);
  if (ErrorRecordDescription->SecValidMask & BIT0) {
    GenErrData->ValidationBits |= BIT0; // FruIdValid
    CopyGuid ((EFI_GUID *) (VOID *) &GenErrData->FruId, &ErrorRecordDescription->FruId);
  }
  if (ErrorRecordDescription->SecValidMask & BIT1) {
    GenErrData->ValidationBits |= BIT1; // FruStringValid
    CopyMem (&GenErrData->FruText[0], &ErrorRecordDescription->FruString[0], 20);
  }

  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrSts->BlockStatus.CorrectableErrorValid: 0x%X\n", GenErrSts->BlockStatus.CorrectableErrorValid));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrSts->BlockStatus.UncorrectableErrorValid: 0x%X\n", GenErrSts->BlockStatus.UncorrectableErrorValid));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrSts->BlockStatus.MultipleCorrectableErrors: 0x%X\n", GenErrSts->BlockStatus.MultipleCorrectableErrors));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrSts->BlockStatus.MultipleUncorrectableErrors: 0x%X\n", GenErrSts->BlockStatus.MultipleUncorrectableErrors));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrSts->BlockStatus.ErrorDataEntryCount: 0x%X\n", GenErrSts->BlockStatus.ErrorDataEntryCount));

  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrData->SectionType: 0x%g\n", GenErrData->SectionType));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrData->ErrorSeverity: 0x%X\n", GenErrData->ErrorSeverity));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrData->Revision: 0x%X\n", GenErrData->Revision));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrData->ValidationBits: 0x%X\n", GenErrData->ValidationBits));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrData->Flags: 0x%X\n", GenErrData->Flags));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrData->ErrorDataLength: 0x%X\n", GenErrData->ErrorDataLength));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrData->FruId: 0x%g\n", GenErrData->FruId));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrData->FruText: 0x%a\n", GenErrData->FruText));

  RAS_DEBUG((LEVEL_FUNC_FLOW, "SectionSize: 0x%x\n", SectionSize));

  //
  //copy the cper error data section
  //
  CopyMem (ErrorSection, ErrorRecord, SectionSize);

  return EFI_SUCCESS;
}

/**
  This routine is to compare to severity values and determine if higher severity value is encountered.
  Higher severity value indicates more severe error.

  @param [in]  CurrentSeverity               Current severity recorded in the status block.
  @param [in]  NewSeverity                   Severity of the new error incoming error.

  @retval  BOOLEAN
**/
BOOLEAN
EFIAPI
IsHigherSeverity (
  IN  UINT32                     CurrentSeverity,
  IN  UINT32                     NewSeverity
  )
{
    //
    // ACPI spec defines below severities:
    //   - EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTABLE   = 0
    //   - EFI_ACPI_6_2_ERROR_SEVERITY_FATAL         = 1
    //   - EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTED     = 2
    //   - EFI_ACPI_6_2_ERROR_SEVERITY_NONE          = 3
    // Unfortunately, the severity value doesn't match the severity level.
    // So we use an array to rearrange their values based on sorted severity level. i.e. NONE < CORRECTED < CORRECTABLE < FATAL
    // The ACPI defined severity value is used as index into the array.
    // The array value represents sorted value for the severity levels, respectively.
    // The rearranged severity level is:
    //   - EFI_ACPI_6_2_ERROR_SEVERITY_FATAL         = 4
    //   - EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTABLE   = 3
    //   - EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTED     = 2
    //   - EFI_ACPI_6_2_ERROR_SEVERITY_NONE          = 1
    //
    UINT32 SortedSeverities[] = {3, 4, 2, 1 };

    return SortedSeverities[NewSeverity] > SortedSeverities[CurrentSeverity];
}
