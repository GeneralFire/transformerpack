/** @file
  Implementation of the WHEA Error Log Listener.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/RasDebugLib.h>
#include <Library/ErrorReportLib.h>
#include <Library/PlatPolicyLib.h>
#include <IndustryStandard/Acpi.h>
#include <Guid/ErrorReportLibListener.h>
#include <Library/WheaSciLib.h>
#include <Library/IioRasLib.h>
#include <Library/WheaSupportLib.h>
#include <RasSwSmi.h>
#include <Library/RasGlobalDataLib.h>
#include "WheaErrorLogListener.h"
#include <Protocol/GlobalNvsArea.h>
#include <RasAcpi.h>

///
/// PCI Express Root Port AER Structure Definition
///
EFI_ACPI_6_2_PCI_EXPRESS_ROOT_PORT_AER_STRUCTURE  PcieRootPortAerSource = {
  EFI_ACPI_6_2_PCI_EXPRESS_ROOT_PORT_AER, //UINT16  Type;
  0, //UINT16  SourceId;
  {0,0}, //UINT8   Reserved0[2];
  3, //UINT8   Flags;
  0, //UINT8   Enabled;
  1,//UINT32  NumberOfRecordsToPreAllocate;
  1,//UINT32  MaxSectionsPerRecord;
  0,//UINT32  Bus;
  0,//UINT16  Device;
  0,//UINT16  Function;
  0,//UINT16  DeviceControl;
  {0,0},//UINT8   Reserved1[2];
  0,//UINT32  UncorrectableErrorMask;
  0,//UINT32  UncorrectableErrorSeverity;
  0,//UINT32  CorrectableErrorMask;
  0,//UINT32  AdvancedErrorCapabilitiesAndControl;
  0,//UINT32  RootErrorCommand;
};

///
/// PCI Express Device AER Structure Definition
///
EFI_ACPI_6_2_PCI_EXPRESS_DEVICE_AER_STRUCTURE  PcieDeviceAerSource = {
  EFI_ACPI_6_2_PCI_EXPRESS_DEVICE_AER, //UINT16  Type;
  0,//UINT16  SourceId;
  {0,0},//UINT8   Reserved0[2];
  3,//UINT8   Flags;
  0,//UINT8   Enabled;
  1,//UINT32  NumberOfRecordsToPreAllocate;
  1,//UINT32  MaxSectionsPerRecord;
  0,//UINT32  Bus;
  0,//UINT16  Device;
  0,//UINT16  Function;
  0,//UINT16  DeviceControl;
  {0,0},//UINT8   Reserved1[2];
  0,//UINT32  UncorrectableErrorMask;
  0,//UINT32  UncorrectableErrorSeverity;
  0,//UINT32  CorrectableErrorMask;
  0,//UINT32  AdvancedErrorCapabilitiesAndControl;
};

///
/// PCI Express Bridge AER Structure Definition
///
EFI_ACPI_6_2_PCI_EXPRESS_BRIDGE_AER_STRUCTURE  PcieBridgeAerSource = {
  EFI_ACPI_6_2_PCI_EXPRESS_BRIDGE_AER,//UINT16  Type;
  0,//UINT16  SourceId;
  {0,0},//UINT8   Reserved0[2];
  3,//UINT8   Flags;
  0,//UINT8   Enabled;
  1,//UINT32  NumberOfRecordsToPreAllocate;
  1,//UINT32  MaxSectionsPerRecord;
  0,//UINT32  Bus;
  0,//UINT16  Device;
  0,//UINT16  Function;
  0,//UINT16  DeviceControl;
  {0,0},//UINT8   Reserved1[2];
  0,//UINT32  UncorrectableErrorMask;
  0,//UINT32  UncorrectableErrorSeverity;
  0,//UINT32  CorrectableErrorMask;
  0,//UINT32  AdvancedErrorCapabilitiesAndControl;
  0,//UINT32  SecondaryUncorrectableErrorMask;
  0,//UINT32  SecondaryUncorrectableErrorSeverity;
  0,//UINT32  SecondaryAdvancedErrorCapabilitiesAndControl;
};

EFI_ACPI_6_2_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE  CorrErrSource = {
  EFI_ACPI_6_2_GENERIC_HARDWARE_ERROR,     // Type
  0,                                       // SourceId
  0xffff,                                  // RelatedSourceId
  0,                                       // Flags
  TRUE,                                    // Enabled
  NUM_RECORD_PRE_ALLOCATED,                // NumberOfRecordsToPreAllocate
  MAX_SECTION_PER_RECORD,                  // MaxSectionsPerRecord
  MAX_RAW_DATA_LENGTH,                     // MaxRawDataLength
  {0, 0x40, 0, 4, 0},                       // ErrorStatusAddress
  {EFI_ACPI_6_2_HARDWARE_ERROR_NOTIFICATION_SCI, \
    sizeof(EFI_ACPI_6_2_HARDWARE_ERROR_NOTIFICATION_STRUCTURE),\
    {0},0,0,0,0,0,0},                        // NotificationStructure
  ERROR_STATUS_BLOCK_LENGTH                // ErrorStatusBlockLength
};

EFI_ACPI_6_2_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE  UnCorrErrSource = {
  EFI_ACPI_6_2_GENERIC_HARDWARE_ERROR,     // Type
  1,                                       // SourceId
  0xffff,                                  // RelatedSourceId
  0,                                       // Flags
  TRUE,                                    // Enabled
  NUM_RECORD_PRE_ALLOCATED,                // NumberOfRecordsToPreAllocate
  MAX_SECTION_PER_RECORD,                  // MaxSectionsPerRecord
  MAX_RAW_DATA_LENGTH,                     // MaxRawDataLength
  {0, 0x40, 0, 4, 0},                       // ErrorStatusAddress
  {EFI_ACPI_6_2_HARDWARE_ERROR_NOTIFICATION_NMI, \
    sizeof(EFI_ACPI_6_2_HARDWARE_ERROR_NOTIFICATION_STRUCTURE),\
    {0},0,0,0,0,0,0},                        // NotificationStructure
  ERROR_STATUS_BLOCK_LENGTH                // ErrorStatusBlockLength
};

// Module global data
RAS_POLICY               *mPolicyData =NULL;
UINT64                    mCorrectedAddress;
UINT64                    mUnCorrectedAddress;
BIOS_ACPI_PARAM          *mAcpiParameter;
extern RAS_ACPI_PARAM    *mRasAcpiParam;

/**
  Restart a WHEA Elog by address.

  @param[in]  ErrorAddress      Address of the error log.

  @retval EFI_SUCCESS           The error logs were successfully cleared.
  @retval EFI_INVALID_PARAMETER The error address is null.
**/
EFI_STATUS
RestartWheaElogByAddress (
  IN  UINT64  ErrorAddress
  )
{
  EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE *ErrStsBlk;

  ErrStsBlk = (EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE *) (UINTN) (*(UINT64 *)(UINTN)ErrorAddress);

  if (ErrStsBlk == NULL) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "[WheaLog] ERROR: ErrStsBlk is NULL\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  //
  // WHEA aware OS will clear following flag after it read WEHA log.
  //
  if (((ErrStsBlk->BlockStatus.CorrectableErrorValid == 0) &&
       (ErrStsBlk->BlockStatus.UncorrectableErrorValid == 0))) {

    RAS_DEBUG ((LEVEL_FUNC_FLOW, "[WheaLog] Erasing all status blocks\n"));

    ZeroMem ((VOID *) ErrStsBlk, ERROR_LOG_BUFFER_SIZE - sizeof (UINT64));

    //
    // Fill the layout info into the Generic Error Status Block, such as the
    // Raw Data Offset, Raw Data Length.
    //
    ErrStsBlk->RawDataOffset = ERROR_STATUS_BLOCK_LENGTH;

    //
    // Once restarted, there is no error, so the severity level should be set
    // to EFI_ACPI_6_2_ERROR_SEVERITY_NONE.
    //
    ErrStsBlk->ErrorSeverity = EFI_ACPI_6_2_ERROR_SEVERITY_NONE;
  }

  return EFI_SUCCESS;
}

/**
  Restart all WHEA Elog.

  This function clears all error sources so a fresh log will be sent to the OS.

  @retval EFI_SUCCESS           The error logs were successfully cleared.
  @retval EFI_NOT_READY         WHEA is not enabled.
  @retval EFI_INVALID_PARAMETER An error address is null.
**/
EFI_STATUS
RestartAllWheaElog (
  VOID
  )
{
  EFI_STATUS Status;

  if (!(mRasAcpiParam->WheaEnabled)) {
    return EFI_NOT_READY;
  }

  Status = RestartWheaElogByAddress (mCorrectedAddress);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = RestartWheaElogByAddress (mUnCorrectedAddress);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Entry point for the Core Handler initialization.

  This function initializes the WHEA Error Log Listener


  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval Status.

**/
EFI_STATUS
EFIAPI
InitializWheaErrorLogListener (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
  EFI_STATUS                      Status;
  UINT64                          *HestGhesCorrStsBlkAddr;
  UINT64                          *HestGhesUnCorrStsBlkAddr;
  EFI_GLOBAL_NVS_AREA_PROTOCOL    *AcpiNvsProtocol = NULL;
  BOOLEAN                         NewAllocation;

  RAS_DEBUG((LEVEL_FUNC_FLOW, "[WheaLog]InitializWheaErrorLogListener++\n"));
  mPolicyData = GetRasPolicy();

  RAS_DEBUG((LEVEL_FUNC_FLOW, "[WheaLog]WheaSupportEn: %d, WheaLogMemoryEn: %d, WheaLogProcEn: %d, WheaLogPciEn: %d\n",\
                               mPolicyData->WheaPolicy.WheaSupportEn, \
                               mPolicyData->WheaPolicy.WheaLogMemoryEn, \
                               mPolicyData->WheaPolicy.WheaLogProcEn, \
                               mPolicyData->WheaPolicy.WheaLogPciEn \
                               ));

  if ((mPolicyData->WheaPolicy.WheaSupportEn == 0) || \
      ((mPolicyData->WheaPolicy.WheaLogPciEn == 0) && \
       (mPolicyData->WheaPolicy.WheaLogMemoryEn == 0) && \
       (mPolicyData->WheaPolicy.WheaLogProcEn == 0))
     )  {
    return EFI_SUCCESS;
  }

  //
  // Add generic error source
  //
  AddHESTErrorSource (
    &CorrErrSource,
    sizeof (EFI_ACPI_6_2_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE),
    FALSE,
    &mCorrectedAddress
    );

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "[WheaLog] corrected buffer: %lx\n", mCorrectedAddress));

  AddHESTErrorSource (
    &UnCorrErrSource,
    sizeof (EFI_ACPI_6_2_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE),
    FALSE,
    &mUnCorrectedAddress
    );

  //
  // Linux check PCIE Error Source to determine AER FFM mode, so need to
  // install them when OS Native AER is disabled.
  //
  if (mPolicyData->IioPolicy.OsNativeAerSupport == 0) {
    AddHESTErrorSource (
      &PcieRootPortAerSource,
      sizeof (EFI_ACPI_6_2_PCI_EXPRESS_ROOT_PORT_AER_STRUCTURE),
      FALSE,
      NULL
      );

    AddHESTErrorSource (
      &PcieDeviceAerSource,
      sizeof (EFI_ACPI_6_2_PCI_EXPRESS_DEVICE_AER_STRUCTURE),
      FALSE,
      NULL
      );

    AddHESTErrorSource (
      &PcieBridgeAerSource,
      sizeof (EFI_ACPI_6_2_PCI_EXPRESS_BRIDGE_AER_STRUCTURE),
      FALSE,
      NULL
      );
  }

  //
  // Publish HEST Table
  //
  AddHESTErrorSource (
    NULL,
    0,
    TRUE,
    NULL
    );

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "[WheaLog] uncorrected buffer: %lx\n", mUnCorrectedAddress));

  //
  // Save status block address
  //
  HestGhesCorrStsBlkAddr = GetRasGlobalData (L"HestGhesCorrStsBlkAddr", sizeof (UINT64), NULL);
  HestGhesUnCorrStsBlkAddr = GetRasGlobalData (L"HestGhesUnCorrStsBlkAddr", sizeof (UINT64), NULL);

  if (HestGhesCorrStsBlkAddr != NULL){
    *HestGhesCorrStsBlkAddr = mCorrectedAddress;
  }
  if (HestGhesUnCorrStsBlkAddr != NULL){
    *HestGhesUnCorrStsBlkAddr = mUnCorrectedAddress;
  }

  Status = gBS->LocateProtocol(
                &gEfiGlobalNvsAreaProtocolGuid,
                NULL,
                &AcpiNvsProtocol
                );
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR(Status)) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "AcpiNvsProtocol is not installed\n", Status));
    return Status;
  }
  mAcpiParameter = AcpiNvsProtocol->Area;
  if (mAcpiParameter == NULL) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "ACPIParameter is NULL\n", Status));
    return EFI_UNSUPPORTED;
  }

  mRasAcpiParam = GetRasGlobalDataEfiReservedMemory (
                    L"RasAcpiParam",
                    sizeof (RAS_ACPI_PARAM),
                    &NewAllocation);
  if (mRasAcpiParam == NULL) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "RasAcpiParam buffer is not allocated\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  mRasAcpiParam->WheaSupportEn= mPolicyData->WheaPolicy.WheaSupportEn;

  Status = RegisterErrorLogListener (LogErrorToWhea);
  ASSERT_EFI_ERROR (Status);

  RAS_DEBUG((LEVEL_FUNC_FLOW, "[WheaLog]InitializWheaErrorLogListener--, %r\n", Status));

  return Status;
}

/**
  The WHEA Error Logger.

  This function is responsible for logging errors via the Elog interface.

  @param[in]  CodeType      Indicates the type of status code being reported.
  @param[in[  Value            Describes the current status of a hardware or
                           software entity. This includes information about the class and
                           subclass that is used to classify the entity as well as an operation.
                           For progress codes, the operation is the current activity.
                           For error codes, it is the exception.For debug codes,it is not defined at this time.
  @param[in]  Instance         The enumeration of a hardware or software entity within
                           the system. A system may contain multiple entities that match a class/subclass
                           pairing. The instance differentiates between them. An instance of 0 indicates
                           that instance information is unavailable, not meaningful, or not relevant.
                           Valid instance numbers start with 1.
  @param[in]  CallerId         This optional parameter may be used to identify the caller.
                           This parameter allows the status code driver to apply different rules to
                           different callers.
  @param[in]  Data             This optional parameter may be used to pass additional data.

  @retval EFI_SUCCESS      Status code reported to serial I/O successfully.

**/
EFI_STATUS
EFIAPI
LogErrorToWhea (
  IN EFI_STATUS_CODE_TYPE     CodeType,
  IN EFI_STATUS_CODE_VALUE    Value,
  IN UINT32                   Instance,
  IN EFI_GUID                 *CallerId,
  IN EFI_STATUS_CODE_DATA     *Data
  )
{
  EFI_COMMON_ERROR_RECORD_HEADER  *ErrorRecordHeader;
  EFI_ERROR_SECTION_DESCRIPTOR                           *ErrorRecordDescription;
  EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE            *GenErrSts;
  EFI_ACPI_6_2_GENERIC_ERROR_DATA_ENTRY_STRUCTURE        *GenErrData;
  VOID                                                   *ErrorRecord;
  VOID                                                   *ErrorSection;
  EFI_STATUS                                             Status;
  UINT64                                                 ErrorBufferAddress;
  BOOLEAN                                                ValidEntry;
  UINT32                                                 SectionSize;
  BOOLEAN                                                IsCeLogged = FALSE;    // No CE logged yet
  BOOLEAN                                                IsUceLogged = FALSE;
  BOOLEAN                                                MemErrorFound = FALSE;
  BOOLEAN                                                PcieErrorFound = FALSE;

  ValidEntry = FALSE;
  SectionSize =  0;

  if (!(mRasAcpiParam->WheaEnabled)) {
      return EFI_NOT_READY;
  }

  //
  // Check whether status code is what we are interested in.
  //
  if (((CodeType & EFI_STATUS_CODE_TYPE_MASK) != EFI_ERROR_CODE) || \
      (Value != SMM_RAS_ENHANCED_ERROR_LOG) || \
      (Data == NULL)) {
    return EFI_UNSUPPORTED;
  }

  RAS_DEBUG((LEVEL_FUNC_FLOW, "[WheaLog]LogErrorToWhea++, CodeType: %X\n", CodeType));

  Status = RestartAllWheaElog();
  RAS_DEBUG((LEVEL_FUNC_FLOW, "[WheaLog]RestartAllWheaElog: %r\n", Status));

  if (EFI_ERROR(Status)) {
    return Status;
  }

  ErrorRecordHeader = (EFI_COMMON_ERROR_RECORD_HEADER *)((UINT8 *)Data + sizeof (EFI_STATUS_CODE_DATA));

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

  ErrorRecordDescription = (EFI_ERROR_SECTION_DESCRIPTOR *)((UINT8 *)ErrorRecordHeader + sizeof(EFI_COMMON_ERROR_RECORD_HEADER));
  ErrorRecord = (UINT8 *)ErrorRecordDescription + sizeof(EFI_ERROR_SECTION_DESCRIPTOR);

  // Dump each fields:
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordDescription->SectionOffset: 0x%X\n", ErrorRecordDescription->SectionOffset));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordDescription->SectionLength: 0x%X\n", ErrorRecordDescription->SectionLength));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordDescription->Revision: 0x%X\n", ErrorRecordDescription->Revision));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordDescription->SecValidMask: 0x%X\n", ErrorRecordDescription->SecValidMask));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordDescription->SectionFlags: 0x%X\n", ErrorRecordDescription->SectionFlags));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordDescription->SectionType: 0x%g\n", ErrorRecordDescription->SectionType));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordDescription->FruId: 0x%g\n", ErrorRecordDescription->FruId));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordDescription->Severity: 0x%X\n", ErrorRecordDescription->Severity));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordDescription->FruString: 0x%a\n", ErrorRecordDescription->FruString));

  RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordDescription->SectionType: %g\n", ErrorRecordDescription->SectionType));

  if (CompareGuid (&ErrorRecordDescription->SectionType, &gEfiProcessorGenericErrorSectionGuid)) {
    if (mPolicyData->WheaPolicy.WheaLogProcEn == 1) {
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "WHEA: Detected Processor Error\n"));

      ValidEntry = TRUE;
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "WHEA: Ignore Processor WHEA error now.\n"));
      SectionSize = sizeof (EFI_PROCESSOR_GENERIC_ERROR_DATA);

    }
  } else if (CompareGuid (&ErrorRecordDescription->SectionType, &gEfiProcessorSpecificErrorSectionGuid)) {
    // Currently not used
  } else if (CompareGuid (&ErrorRecordDescription->SectionType, &gEfiPlatformMemoryErrorSectionGuid)) {
    if (mPolicyData->WheaPolicy.WheaLogMemoryEn == 1) {
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "WHEA: Detected Memory Error\n"));
      MemErrorFound = TRUE;
      ValidEntry = TRUE;
      SectionSize = sizeof (EFI_PLATFORM_MEMORY_ERROR_DATA);
    }
  } else if (CompareGuid (&ErrorRecordDescription->SectionType, &gEfiDirectedIoDMArErrorSectionGuid)) {
    // Currently not used
  } else if (CompareGuid (&ErrorRecordDescription->SectionType, &gEfiPcieErrorSectionGuid)) {
    if (mPolicyData->WheaPolicy.WheaLogPciEn == 1) {
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "WHEA: Detected PCIe Error\n"));
      PcieErrorFound = TRUE;
      ValidEntry = TRUE;
      SectionSize = sizeof (EFI_PCIE_ERROR_DATA);
    }
  }

  if ((ErrorRecordDescription->Severity == EFI_ACPI_6_2_ERROR_SEVERITY_FATAL) || 
      ((PcieErrorFound == TRUE) && (ErrorRecordDescription->Severity == EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTABLE))) {  // EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTABLE mapped to Pcie NONFATAL_ERROR_TYPE
    ErrorBufferAddress = mUnCorrectedAddress;
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "[WheaLog] Uncorrected address: %lx\n", ErrorBufferAddress));
  } else {
    ErrorBufferAddress = mCorrectedAddress;
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "[WheaLog] Corrected address: %lx\n", ErrorBufferAddress));
  }

  GenErrSts = (EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE *)(UINTN)(*(UINT64 *)(UINTN)ErrorBufferAddress);

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "[WheaLog]GenErrSts: %p\n", GenErrSts));
  if (GenErrSts == NULL) {
    return EFI_NOT_FOUND;
  }

  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrSts->BlockStatus.CorrectableErrorValid: 0x%X\n", GenErrSts->BlockStatus.CorrectableErrorValid));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrSts->BlockStatus.UncorrectableErrorValid: 0x%X\n", GenErrSts->BlockStatus.UncorrectableErrorValid));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrSts->BlockStatus.MultipleCorrectableErrors: 0x%X\n", GenErrSts->BlockStatus.MultipleCorrectableErrors));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrSts->BlockStatus.MultipleUncorrectableErrors: 0x%X\n", GenErrSts->BlockStatus.MultipleUncorrectableErrors));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrSts->BlockStatus.ErrorDataEntryCount: 0x%X\n", GenErrSts->BlockStatus.ErrorDataEntryCount));

  // Calculate offset for next error data log
  GenErrData = (EFI_ACPI_6_2_GENERIC_ERROR_DATA_ENTRY_STRUCTURE *) ((UINT8 *)GenErrSts + sizeof(EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE) + GenErrSts->DataLength);
  ErrorSection = (VOID *) ((UINT8 *)GenErrData + sizeof(EFI_ACPI_6_2_GENERIC_ERROR_DATA_ENTRY_STRUCTURE));

  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrData->SectionType: 0x%g\n", GenErrData->SectionType));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrData->ErrorSeverity: 0x%X\n", GenErrData->ErrorSeverity));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrData->Revision: 0x%X\n", GenErrData->Revision));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrData->ValidationBits: 0x%X\n", GenErrData->ValidationBits));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrData->Flags: 0x%X\n", GenErrData->Flags));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrData->ErrorDataLength: 0x%X\n", GenErrData->ErrorDataLength));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrData->FruId: 0x%g\n", GenErrData->FruId));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "GenErrData->FruText: 0x%a\n", GenErrData->FruText));

  RAS_DEBUG((LEVEL_FUNC_FLOW, "ValidEntry: %X\n", ValidEntry));

  if (!ValidEntry) {
    return EFI_NOT_FOUND;
  }

  //size check, no need to check count
  if (GenErrSts->DataLength + sizeof (EFI_ACPI_6_2_GENERIC_ERROR_DATA_ENTRY_STRUCTURE) + SectionSize > ERROR_STATUS_BLOCK_LENGTH){
    RAS_DEBUG ((
      LEVEL_FUNC_FLOW,
      "Requested size %x exceeds region max size %x\n",
      GenErrSts->DataLength + sizeof (EFI_ACPI_6_2_GENERIC_ERROR_DATA_ENTRY_STRUCTURE) + SectionSize,
      ERROR_STATUS_BLOCK_LENGTH
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

  if ((ErrorRecordDescription->Severity == EFI_ACPI_6_2_ERROR_SEVERITY_FATAL)||(ErrorRecordDescription->Severity == EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTABLE)) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, " --Logging Uncorrected Error to WHEA\n"));
    IsUceLogged = TRUE;
    if (GenErrSts->BlockStatus.UncorrectableErrorValid == 0) {
      GenErrSts->BlockStatus.UncorrectableErrorValid = 1;
    } else {
      GenErrSts->BlockStatus.MultipleUncorrectableErrors = 1;
    }
  } else {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, " --Logging Corrected Error to WHEA\n"));
    IsCeLogged = TRUE;
    if (GenErrSts->BlockStatus.CorrectableErrorValid == 0) {
      GenErrSts->BlockStatus.CorrectableErrorValid = 1;
    } else {
      GenErrSts->BlockStatus.MultipleCorrectableErrors = 1;
    }
  }

  if (MemErrorFound == TRUE) {
    if (ErrorRecordDescription->Severity == EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTABLE) {
      GenErrSts->ErrorSeverity = EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTABLE;

      IsCeLogged = TRUE;
      IsUceLogged = FALSE;
    }
  }

  GenErrSts->DataLength += sizeof(EFI_ACPI_6_2_GENERIC_ERROR_DATA_ENTRY_STRUCTURE) + SectionSize;
  GenErrSts->BlockStatus.ErrorDataEntryCount++;

  GenErrData->ErrorDataLength = SectionSize;
  GenErrData->ErrorSeverity = ErrorRecordDescription->Severity;
  GenErrData->Revision = ErrorRecordDescription->Revision;
  GenErrData->Flags |= BIT0; // Primary
  CopyGuid ((EFI_GUID *)(VOID *)&GenErrData->SectionType, &ErrorRecordDescription->SectionType);
  if (ErrorRecordDescription->SecValidMask & BIT0) {
    GenErrData->ValidationBits |= BIT0; // FruIdValid
    CopyGuid ((EFI_GUID *)(VOID *)&GenErrData->FruId, &ErrorRecordDescription->FruId);
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


  CopyMem (ErrorSection, ErrorRecord, SectionSize);

  // For WHEA Mechanism, we need to generate SCI, but we don't want to preclude other
  // notify mechanisms, so don't set OsNotified

  if (IsCeLogged) {
    mAcpiParameter->WheaSci = 1;
    SignalWheaSci ();
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "WHEA: Sending OS notification via SCI. %r\n", Status));
  }

  RAS_DEBUG((LEVEL_FUNC_FLOW, "[WheaLog]LogErrorToWhea--:%r\n", Status));

  return EFI_SUCCESS;
}
