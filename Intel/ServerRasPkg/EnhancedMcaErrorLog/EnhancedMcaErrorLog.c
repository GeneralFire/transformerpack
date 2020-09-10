/** @file
  Implementation of the eMca Error Logger.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <Library/RasDebugLib.h>
#include <Library/ErrorReportLib.h>
#include <Library/PlatPolicyLib.h>
#include <IndustryStandard/Acpi50.h>
#include <Emca.h>
#include <Guid/ErrorReportLibListener.h>
#include "EnhancedMcaLogPrivateLib.h"
#include "EnhancedMcaErrorLog.h"
#include <Library/RasGlobalDataLib.h>
#include <RasAcpi.h>

//
// Module global data
//
RAS_POLICY               *mPolicyData = NULL;


/**
  Entry point for the Core Handler initialization.

  This function initializes the error handling and enables error
  sources for the CPU Core.


  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval Status.

**/
EFI_STATUS
EFIAPI
InitializEnhancedMcaErrorLogger (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
  EFI_STATUS    Status;
  VOID                            *EmcaL1HdrPtr = NULL;
  VOID                            *EmcaElogDirPtr = NULL;
  BOOLEAN                         NewAllocation;
  RAS_ACPI_PARAM                  *RasAcpiParam = NULL;

  RAS_DEBUG((LEVEL_BASIC_FLOW, "InitializEnhancedMcaErrorLogger++\n"));
  mPolicyData = GetRasPolicy ();

  RAS_DEBUG((LEVEL_BASIC_FLOW, "EmcaEn: %d, EmcaIgnOptin: %d, ElogCorrErrEn: %d, ElogMemErrEn: %d, ElogProcErrEn: %d\n", \
                               mPolicyData->McaPolicy.EmcaEn, \
                               mPolicyData->McaPolicy.EmcaIgnOptin, \
                               mPolicyData->McaPolicy.ElogCorrErrEn, \
                               mPolicyData->McaPolicy.ElogMemErrEn, \
                               mPolicyData->McaPolicy.ElogProcErrEn \
                               ));

  if (mPolicyData->McaPolicy.EmcaEn == 0) {
    return EFI_UNSUPPORTED;
  }

  RasAcpiParam = GetRasGlobalDataEfiReservedMemory (
                    L"RasAcpiParam",
                    sizeof (RAS_ACPI_PARAM),
                    &NewAllocation);

  if (RasAcpiParam == NULL) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "RasAcpiParam buffer is not allocated\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  RasAcpiParam -> EmcaEn = mPolicyData->McaPolicy.EmcaEn;
  Status = InitEMCA (&EmcaL1HdrPtr, &EmcaElogDirPtr);
  if (EFI_ERROR (Status)) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "EmcaL1DirAddr InitEMCA return Error (0x%08x)\n", Status));
    return Status;
  } else {
    RasAcpiParam -> EmcaL1DirAddr = (UINT64) EmcaL1HdrPtr;
    RAS_DEBUG((LEVEL_BASIC_FLOW, "EmcaL1DirAddr = 0x%08x\n", EmcaL1HdrPtr));
  }

  Status = RegisterErrorLogListener (LogErrorToElog);
  RAS_DEBUG((LEVEL_BASIC_FLOW, "InitializEnhancedMcaErrorLogger--, %r\n", Status));

  return Status;
}

/**
  The Elog Error Logger.

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
LogErrorToElog (
  IN EFI_STATUS_CODE_TYPE     CodeType,
  IN EFI_STATUS_CODE_VALUE    Value,
  IN UINT32                   Instance,
  IN EFI_GUID                 *CallerId,
  IN EFI_STATUS_CODE_DATA     *Data
  )
{
  EFI_COMMON_ERROR_RECORD_HEADER  *ErrorRecordHeader;
  EFI_ERROR_SECTION_DESCRIPTOR    *ErrorRecordDescription;
  VOID                            *ErrorRecord;
  UINT8                           GenErrRecord[SIZE_4KB];
  EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE            *GenErrSts;
  EFI_ACPI_6_2_GENERIC_ERROR_DATA_ENTRY_STRUCTURE        *GenErrData;
  VOID                            *ErrorSection;
  UINT32                          McBank = 0;
  EMCA_MC_SIGNATURE_ERR_RECORD    McErrSig;

  //
  // Check whether status code is what we are interested in.
  //
  if (((CodeType & EFI_STATUS_CODE_TYPE_MASK) != EFI_ERROR_CODE) || \
      (Value != SMM_RAS_ENHANCED_ERROR_LOG) || \
      (Data == NULL)) {
    return EFI_UNSUPPORTED;
  }

  RAS_DEBUG((LEVEL_FUNC_FLOW, "LogErrorToElog++, CodeType: %X, Value: %X\n", CodeType, Value));

  ErrorRecordHeader = (EFI_COMMON_ERROR_RECORD_HEADER *) ((UINT8 *)Data + sizeof (EFI_STATUS_CODE_DATA));

  RAS_DEBUG((LEVEL_FUNC_FLOW, "isEmcaOptInFlagSet (): %X\n", IsEmcaOptInFlagSet ()));

  if (IsEmcaOptInFlagSet () || (mPolicyData->McaPolicy.EmcaIgnOptin == 1)) {
    GenErrSts = (EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE *) &GenErrRecord;
    GenErrData = (EFI_ACPI_6_2_GENERIC_ERROR_DATA_ENTRY_STRUCTURE *) ((UINT8 *) GenErrSts + sizeof (EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE));
    ErrorSection = (VOID *) ((UINT8 *) GenErrData + sizeof (EFI_ACPI_6_2_GENERIC_ERROR_DATA_ENTRY_STRUCTURE));

    ErrorRecordDescription = (EFI_ERROR_SECTION_DESCRIPTOR *) ((UINT8 *) ErrorRecordHeader + sizeof (EFI_COMMON_ERROR_RECORD_HEADER));
    ErrorRecord = (UINT8 *) ErrorRecordDescription + sizeof (EFI_ERROR_SECTION_DESCRIPTOR);

    if ((ErrorRecordDescription->Severity == EFI_GENERIC_ERROR_CORRECTED)
        && (mPolicyData->McaPolicy.ElogCorrErrEn == 0)) {
      RAS_DEBUG((LEVEL_FUNC_FLOW, "ErrorRecordDescription->Severity: %X, return.\n", ErrorRecordDescription->Severity));

      return  EFI_UNSUPPORTED;
    }

    ZeroMem (GenErrSts, sizeof (EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE));
    ZeroMem (GenErrData, sizeof (EFI_ACPI_6_2_GENERIC_ERROR_DATA_ENTRY_STRUCTURE));
    ZeroMem (&McErrSig, sizeof (EMCA_MC_SIGNATURE_ERR_RECORD));

    RAS_DEBUG((LEVEL_FUNC_FLOW, "LogErrorToElog: SectionType: %g\n", ErrorRecordDescription->SectionType));

    if (CompareGuid (&ErrorRecordDescription->SectionType, &gEfiPlatformMemoryErrorSectionGuid)
       && (ErrorRecordHeader->RecordLength == sizeof (EMCA_MEM_ERROR_RECORD))) {
      if (mPolicyData->McaPolicy.ElogMemErrEn == 1) {
        RAS_DEBUG((LEVEL_FUNC_FLOW, "LogErrorToElog: Detected Memory Error\n"));

        GenErrData->ErrorDataLength = sizeof (EMCA_MEMORY_ERROR_SECTION) - sizeof (MCA_BANK_INFO);
        CopyMem (ErrorSection, ErrorRecord, sizeof (EMCA_MEMORY_ERROR_SECTION) - sizeof (MCA_BANK_INFO));

        McBank = ((EMCA_MEMORY_ERROR_SECTION *) ErrorRecord)->McaBankInfo.BankNum;
        McErrSig.ApicId = ((EMCA_MEMORY_ERROR_SECTION *) ErrorRecord)->McaBankInfo.ApicId;
        McErrSig.Signature.McSts = ((EMCA_MEMORY_ERROR_SECTION *) ErrorRecord)->McaBankInfo.McaStatus;
        McErrSig.Signature.McAddr = ((EMCA_MEMORY_ERROR_SECTION *) ErrorRecord)->McaBankInfo.McaAddress;
        McErrSig.Signature.McMisc = ((EMCA_MEMORY_ERROR_SECTION *) ErrorRecord)->McaBankInfo.McaMisc;
      }
    } else if (CompareGuid (&ErrorRecordDescription->SectionType, &gEfiProcessorGenericErrorSectionGuid)) {
      if (mPolicyData->McaPolicy.ElogProcErrEn == 1) {
        RAS_DEBUG((LEVEL_FUNC_FLOW, "LogErrorToElog: Detected Processor Error\n"));

        GenErrData->ErrorDataLength = sizeof (EMCA_PROCESSOR_ERROR_SECTION) - sizeof (MCA_BANK_INFO);
        CopyMem (ErrorSection, ErrorRecord, sizeof (EMCA_PROCESSOR_ERROR_SECTION) - sizeof (MCA_BANK_INFO));

        McBank = ((EMCA_PROCESSOR_ERROR_SECTION *) ErrorRecord)->McaBankInfo.BankNum;
        McErrSig.ApicId = ((EMCA_PROCESSOR_ERROR_SECTION *) ErrorRecord)->McaBankInfo.ApicId;
        McErrSig.Signature.McSts = ((EMCA_PROCESSOR_ERROR_SECTION *) ErrorRecord)->McaBankInfo.McaStatus;
        McErrSig.Signature.McAddr = ((EMCA_PROCESSOR_ERROR_SECTION *) ErrorRecord)->McaBankInfo.McaAddress;
        McErrSig.Signature.McMisc = ((EMCA_PROCESSOR_ERROR_SECTION *) ErrorRecord)->McaBankInfo.McaMisc;
      }
    } else {
      return EFI_UNSUPPORTED;
    }

    RAS_DEBUG((LEVEL_FUNC_FLOW, "LogErrorToElog: McBank: 0x%x, ApicId: 0x%x, McSts: 0x%lx, McAddr: 0x%lx, McMisc: 0x%lx\n", \
      McBank, McErrSig.ApicId, McErrSig.Signature.McSts, McErrSig.Signature.McAddr, McErrSig.Signature.McMisc));


    RAS_DEBUG((LEVEL_FUNC_FLOW, "LogErrorToElog: --Logging Error to EMCA\n"));
    GenErrSts->ErrorSeverity = ErrorRecordDescription->Severity;
    GenErrSts->BlockStatus.ErrorDataEntryCount = 1;
    if ((ErrorRecordDescription->Severity == EFI_GENERIC_ERROR_FATAL) \
        || (ErrorRecordDescription->Severity == EFI_GENERIC_ERROR_RECOVERABLE)) {
      GenErrSts->BlockStatus.UncorrectableErrorValid = 1;
    } else {
      GenErrSts->BlockStatus.CorrectableErrorValid = 1;
    }
    GenErrSts->RawDataLength = 1;
    CopyMem (&GenErrData->SectionType, &ErrorRecordDescription->SectionType, sizeof (GUID));
    GenErrData->ErrorSeverity = ErrorRecordDescription->Severity;
    GenErrData->Revision = ErrorRecordDescription->Revision;
    GenErrData->Flags |= BIT0;  //Primary = 1;
    if (ErrorRecordDescription->SecValidMask & BIT0) {
      GenErrData->ValidationBits |= BIT0;// FruIdValid
      CopyMem (&GenErrData->FruId, &ErrorRecordDescription->FruId, sizeof (GUID));
    }
    if (ErrorRecordDescription->SecValidMask & BIT1) {
      GenErrData->ValidationBits |= BIT1; // FruStringValid
      CopyMem (&GenErrData->FruText[0], &ErrorRecordDescription->FruString[0], 20);
    }

    GenErrSts->DataLength = GenErrData->ErrorDataLength + sizeof (EFI_ACPI_6_2_GENERIC_ERROR_DATA_ENTRY_STRUCTURE);
    GenErrSts->RawDataOffset = GenErrSts->RawDataLength + sizeof (EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE);

    LogEmcaRecord (McBank, &McErrSig, GenErrSts);

  }

  return EFI_SUCCESS;
}
