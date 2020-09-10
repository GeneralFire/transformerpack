/** @file
  Implementation of Error Report library.

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

#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/ProcessorRasLib.h>
#include <Library/MpExtensionLib.h>
#include <Library/RasDebugLib.h>
#include <Library/PciLib.h>
#include <Library/PcieStdLib.h>
#include <Library/PcieRasLib.h>
#include <Register/Cpuid.h>
#include <IndustryStandard/Pci.h>
#include <Protocol/SmmReportStatusCodeHandler.h>
#include <Protocol/SmmCpuService.h>
#include <Library/ErrorReportLib.h>
#include <Guid/ErrorReportLibListener.h>
#include "ErrorReportLibPrivate.h"
#include <Library/MmPciExtLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/WheaSupportLib.h>
#include <Library/SynchronizationLib.h>
#include <Library/RasGlobalDataLib.h>
#include <IehTypes.h>
#include <IndustryStandard/Acpi62.h>
#include <Cpu/CpuCoreRegs.h>

#define ERROR_RECORD_REVISION 0x0300

EFI_SMM_RSC_HANDLER_PROTOCOL         *mRscHandlerProtocol = NULL;
extern EFI_SMM_CPU_SERVICE_PROTOCOL  *mSmmCpuServiceProtocolInterface;
EMCA_MEM_ERROR_RECORD_LIST           *mMemErrorRecordList;

//
// Spin lock used to serialize memory error record buffer update in BSP and all APs
//
SPIN_LOCK                     mMemErrorRecordUpdateLock;

/**
  This implementations is to intialize InitializeEnhancedError data.

  @param[in]  ImageHandle   A handle for the image that is initializing this driver
  @param[in]  SystemTable   A pointer to the EFI system table

  @retval   EFI_STATUS.
**/
EFI_STATUS
EFIAPI
InitializeErrorReportLib (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
{
  EFI_STATUS  Status;
  BOOLEAN     NewAllocation;

  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmRscHandlerProtocolGuid,
                    NULL,
                    (VOID **) &mRscHandlerProtocol
                    );
  RAS_DEBUG((LEVEL_FUNC_FLOW, "%a, %r\n", __FUNCTION__, Status));
  ASSERT_EFI_ERROR (Status);

  Status = gSmst->SmmLocateProtocol (&gEfiSmmCpuServiceProtocolGuid, NULL, (VOID**) &mSmmCpuServiceProtocolInterface);
  ASSERT_EFI_ERROR (Status);
  RAS_DEBUG((LEVEL_FUNC_FLOW, "%a, %r\n", __FUNCTION__, Status));

  InitializeSpinLock(&mMemErrorRecordUpdateLock);
  mMemErrorRecordList = GetRasGlobalData (L"MemErrorRecordList",
                 sizeof (EMCA_MEM_ERROR_RECORD_LIST),
                 &NewAllocation
                 );

  if (NewAllocation) {
    ZeroMem (mMemErrorRecordList, sizeof (EMCA_MEM_ERROR_RECORD_LIST));
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
RegisterErrorLogListener (
  IN EFI_RSC_HANDLER_CALLBACK  Callback
  )
{
  EFI_STATUS  Status;

  Status = mRscHandlerProtocol->Register (Callback);
  RAS_DEBUG((LEVEL_FUNC_FLOW, "%a, %r\n", __FUNCTION__, Status));

  ASSERT_EFI_ERROR (Status);
  return Status;
}

EFI_STATUS
EFIAPI
UnregisterErrorLogListener (
  IN EFI_RSC_HANDLER_CALLBACK  Callback
  )
{
  EFI_STATUS  Status;

  Status = mRscHandlerProtocol->Unregister (Callback);
  RAS_DEBUG((LEVEL_FUNC_FLOW, "%a, %r\n", __FUNCTION__, Status));

  ASSERT_EFI_ERROR (Status);
  return Status;
}

EFI_STATUS
FillMcaBankInfo (
  IN        UINT8                         McBank,
  IN        EMCA_MC_SIGNATURE_ERR_RECORD *McErrSig,
  OUT       MCA_BANK_INFO                *McaBankInfo
  )
{
  if (McErrSig == NULL) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "McErrSig is NULL\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  if (McaBankInfo == NULL) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "McaBankInfo is NULL\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  McaBankInfo->ApicId = McErrSig->ApicId;
  McaBankInfo->BankNum = McBank;
  McaBankInfo->BankType = GetMcBankUnitType (McBank);
  McaBankInfo->BankScope = GetMcBankScope (McBank);
  McaBankInfo->McaStatus = McErrSig->Signature.McSts;
  McaBankInfo->McaAddress = McErrSig->Signature.McAddr;
  McaBankInfo->McaMisc = McErrSig->Signature.McMisc;

  if (McaBankInfo->BankType == MCA_UNIT_TYPE_ERROR) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "BankType == MCA_UNIT_TYPE_ERROR\n"));
    RAS_ASSERT (FALSE);
    return EFI_NOT_FOUND;
  }

  if (McaBankInfo->BankScope == MC_SCOPE_ERROR) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "BankScope == MC_SCOPE_ERROR\n"));
    RAS_ASSERT (FALSE);
    return EFI_NOT_FOUND;
  }

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "McaBankInfo BankNum:    %d\n", McaBankInfo->BankNum));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "McaBankInfo ApicId:     0x%08x\n", McaBankInfo->ApicId));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "McaBankInfo BankType:   0x%08x\n", McaBankInfo->BankType));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "McaBankInfo BankScope:  0x%08x\n", McaBankInfo->BankScope));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "McaBankInfo McaStatus:  0x%016lx\n", McaBankInfo->McaStatus));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "McaBankInfo McaAddress: 0x%016lx\n", McaBankInfo->McaAddress));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "McaBankInfo McaMisc:    0x%016lx\n", McaBankInfo->McaMisc));

  return EFI_SUCCESS;
}

/**
  This function generates the Memory Fru information from memory error data: Node, Card and Module information.

  @param[in]  MemRecord      - memory error record
  @param[out] FruString   -  Memory Fru String for memory error record

  @retval EFI_STATUS
**/
EFI_STATUS
GetMemoryFruInfo (
  IN   EMCA_MEM_ERROR_RECORD  *MemRecord,
  OUT  CHAR8                  *FruString
  )
{
  EFI_STATUS            Status;
  CHAR8                 SlotStr[] = {'C', 'a', 'r', 'd', '?', '?', ',', ' ', 'C', 'h', 'n', '?', ',', ' ', 'D', 'I', 'M', 'M', '?', 0};
  UINT8                 RiserCard;
  EFI_PLATFORM_MEMORY_ERROR_DATA  MemoryErrorData;

  Status = EFI_SUCCESS;

  //MemoryErrorData = MemRecord->EmcaMemErrorSection.MemoryErrorData;
  CopyMem (&MemoryErrorData, &MemRecord->EmcaMemErrorSection.MemoryErrorData, sizeof (EFI_PLATFORM_MEMORY_ERROR_DATA));

  if ((MemoryErrorData.ValidFields & EFI_PLATFORM_MEMORY_NODE_VALID) && \
      (MemoryErrorData.ValidFields & EFI_PLATFORM_MEMORY_CARD_VALID) && \
      (MemoryErrorData.ValidFields & EFI_PLATFORM_MEMORY_MODULE_VALID)) {
    //
    // Data for Thunder Ridge memory.
    // FruString buffer is 20 bytes.
    //

    //
    // MemInfo->Node + 1 indicates the memory riser card number.
    //
    if ((MemoryErrorData.ValidFields & EFI_PLATFORM_MEMORY_NODE_VALID) && (MemoryErrorData.Node <= 98)) {
      RiserCard = (UINT8) (MemoryErrorData.Node + 1);
      RiserCard = DecimalToBcd8 (RiserCard);
      SlotStr[4] = (CHAR8) (((RiserCard & 0x00F0) >> 4) + '0');
      SlotStr[5] = (CHAR8) ((RiserCard & 0x000F) + '0');
    }

    //
    // Channel A, B, C, D on memory riser card.
    //
    if ((MemoryErrorData.ValidFields & EFI_PLATFORM_MEMORY_CARD_VALID) && (MemoryErrorData.Card <= 3)) {
      SlotStr[11] = (CHAR8) (MemoryErrorData.Card + 'A');
    }

    //
    // DIMM slot number on memory riser card.
    //
    if ((MemoryErrorData.ValidFields & EFI_PLATFORM_MEMORY_MODULE_VALID) && (MemoryErrorData.ModuleRank <= 9)) {
      SlotStr[18] = (CHAR8) (MemoryErrorData.ModuleRank + '0');
    }

    Status = AsciiStrCpyS (FruString, sizeof (MemRecord->Descriptor.FruString), SlotStr);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  } else {
    Status = EFI_UNSUPPORTED;
  }

  return Status;
}

/**
  Fills error record information for detected error.

  @param[in]  McBank      - MC Bank the error was detected on
  @param[in]  Socket         - The socket the MCE occured on
  @param[in]  Severity         - The error severity
  @param[in]  PlatformMemoryErrorData       - Pointer to data structure EFI_PLATFORM_MEMORY_ERROR_DATA
  @param[in]  McErrSig    - MCE error information
  @param[out] MemRecord   - The error record to fill

  @retval EFI_STATUS
    MemRecord - Error record for the detected error
**/
EFI_STATUS
CreatePlatformMemoryErrorRecord (
  IN        UINT8                         McBank,
  IN        UINT8                         Socket,
  IN        UINT32                        Severity,
  IN        EFI_PLATFORM_MEMORY_ERROR_DATA  *PlatformMemoryErrorData,
  IN        EMCA_MC_SIGNATURE_ERR_RECORD *McErrSig,
  OUT       EMCA_MEM_ERROR_RECORD        *MemRecord
  )
{
  EFI_STATUS    Status;
  UINT32        LogSeverity;
  IA32_MCI_STATUS_MSR_STRUCT  MsrIa32MCiStatus;

  RAS_DEBUG((LEVEL_FUNC_FLOW, "%a Entry,McBank: %d,Socket: %d,Severity: %d\n", \
             __FUNCTION__, McBank, Socket, Severity));
  Status = EFI_SUCCESS;

  if (MemRecord == NULL) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: MemRecord is NULL\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (McErrSig == NULL) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: McErrSig is NULL\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (PlatformMemoryErrorData == NULL) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: PlatformMemoryErrorData is NULL\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (MemRecord, sizeof (EMCA_MEM_ERROR_RECORD));
  MsrIa32MCiStatus.Data = McErrSig->Signature.McSts;
  LogSeverity = Severity;

  //
  //Log the corrected error as UC in case of Uncorrected PatrolScrub error or any other uncorrected errors
  //
  if (LogSeverity == EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTED) {
    if ((((MsrIa32MCiStatus.Bits.ms_code & IMC_PATROL_SCRUB_ERR_MSCOD_MSK) == IMC_UNC_PATROL_SCRUB_ERR_MSCOD) &&
         ((MsrIa32MCiStatus.Bits.mca_code & IMC_GENERIC_ERR_MCACOD_MSK) == IMC_GENERIC_ERR_MCACOD)) ||
        (MsrIa32MCiStatus.Bits.uc)) {
        LogSeverity = EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTABLE;
    }
  }
  //
  // Build Error Record Header
  //
  MemRecord->Header.SignatureStart = EFI_ERROR_RECORD_SIGNATURE_START;
  MemRecord->Header.SignatureEnd = EFI_ERROR_RECORD_SIGNATURE_END;
  MemRecord->Header.Revision = ERROR_RECORD_REVISION;  // UEFI Spec version 2.4
  MemRecord->Header.SectionCount = 1;  // Only supporting one section per error record
  MemRecord->Header.ErrorSeverity = LogSeverity;
  MemRecord->Header.ValidationBits = 0;
  MemRecord->Header.RecordLength = sizeof (EMCA_MEM_ERROR_RECORD);
  if (Severity == EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTED) {
    MemRecord->Header.Flags = EFI_HW_ERROR_FLAGS_RECOVERED;
    CopyGuid (&MemRecord->Header.NotificationType, &gEfiEventNotificationTypeCmcGuid);
  } else {
    CopyGuid (&MemRecord->Header.NotificationType, &gEfiEventNotificationTypeMceGuid);
  }

  //
  // Fill error descriptor
  //
  MemRecord->Descriptor.SectionOffset = sizeof (EFI_COMMON_ERROR_RECORD_HEADER) + sizeof (EFI_ERROR_SECTION_DESCRIPTOR);
  MemRecord->Descriptor.SectionLength = sizeof (EMCA_MEMORY_ERROR_SECTION);
  MemRecord->Descriptor.Revision = ERROR_RECORD_REVISION; // UEFI Spec version 2.4
  MemRecord->Descriptor.SecValidMask = 0;
  MemRecord->Descriptor.SectionFlags = 1; // Primary
  CopyGuid (&MemRecord->Descriptor.SectionType, &gEfiPlatformMemoryErrorSectionGuid);
  MemRecord->Descriptor.Severity = LogSeverity;

  CopyMem (&MemRecord->EmcaMemErrorSection.MemoryErrorData, PlatformMemoryErrorData, sizeof (EFI_PLATFORM_MEMORY_ERROR_DATA));

  Status = GetMemoryFruInfo (MemRecord, MemRecord->Descriptor.FruString);
  if (!EFI_ERROR (Status)) {
    MemRecord->Descriptor.SecValidMask |= EFI_ERROR_SECTION_FRU_STRING_VALID;
  }

  Status = FillMcaBankInfo (McBank, McErrSig, &(MemRecord->EmcaMemErrorSection.McaBankInfo));

  return Status;
}

/**
  Fills error record information for detected error.
  @param[in]  Severity                       - Error Severity
  @param[in]  CorrectedMemoryErrorData       - Pointer to data structure EFI_PLATFORM_MEMORY_ERROR_DATA
  @param[out] MemRecord   - The error record to fill

  @retval EFI_STATUS
    MemRecord - Error record for the detected error
**/
EFI_STATUS
CreateMemoryErrorRecord (
  IN        UINT8                            Severity,
  IN        EFI_PLATFORM_MEMORY_ERROR_DATA  *MemoryErrorData,
  OUT       MEM_ERROR_RECORD                *MemRecord
  )
{
  EFI_STATUS                  Status;

  Status = EFI_SUCCESS;

  RAS_DEBUG((LEVEL_FUNC_FLOW, "%a Entry\n", __FUNCTION__));

  if (MemRecord == NULL) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: MemRecord is NULL\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (MemoryErrorData == NULL) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: MemoryErrorData is NULL\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (MemRecord, sizeof (MEM_ERROR_RECORD));

  //
  // Build Error Record Header
  //
  MemRecord->Header.SignatureStart = EFI_ERROR_RECORD_SIGNATURE_START;
  MemRecord->Header.SignatureEnd = EFI_ERROR_RECORD_SIGNATURE_END;
  MemRecord->Header.Revision = ERROR_RECORD_REVISION;  // UEFI Spec version 2.4
  MemRecord->Header.SectionCount = 1;  // Only supporting one section per error record
  MemRecord->Header.ErrorSeverity = Severity;
  MemRecord->Header.ValidationBits = 0;
  MemRecord->Header.RecordLength = sizeof (MEM_ERROR_RECORD);
  if (Severity == EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTED) {
    MemRecord->Header.Flags = EFI_HW_ERROR_FLAGS_RECOVERED;
  }
  CopyGuid (&MemRecord->Header.NotificationType, &gEfiEventNotificationTypeCmcGuid);

  //
  // Fill error descriptor
  //
  MemRecord->Descriptor.SectionOffset = sizeof (EFI_COMMON_ERROR_RECORD_HEADER) + sizeof (EFI_ERROR_SECTION_DESCRIPTOR);
  MemRecord->Descriptor.SectionLength = sizeof (EFI_PLATFORM_MEMORY_ERROR_DATA);
  MemRecord->Descriptor.Revision = ERROR_RECORD_REVISION; // UEFI Spec version 2.4
  MemRecord->Descriptor.SecValidMask = 0;
  MemRecord->Descriptor.SectionFlags = 1; // Primary
  CopyGuid (&MemRecord->Descriptor.SectionType, &gEfiPlatformMemoryErrorSectionGuid);
  MemRecord->Descriptor.Severity = Severity;

  CopyMem (&MemRecord->MemErrorSection, MemoryErrorData, sizeof (EFI_PLATFORM_MEMORY_ERROR_DATA));

  return EFI_SUCCESS;
}



/**
  Create the processor error record

  @param[in] McBank     - The machine check bank
  @param[in] Socket           - The socket number
  @param[in] ErrorType  - The Error Type of the error
  @param[in] Severity          - The severity of the error
  @param[in] McErrSig    - MC Error signature
  @param[in] ErrSts       - A pointer to the Processor Error Record.

  @retval EFI_STATUS

**/
EFI_STATUS
CreateProcessorErrorRecord (
  IN        UINT8                         McBank,
  IN        UINT8                         Socket,
  IN        UINT8                         ErrorType,
  IN        UINT32                        Severity,
  IN        EMCA_MC_SIGNATURE_ERR_RECORD *McErrSig,
  OUT       PROCESSOR_ERROR_RECORD       *ErrSts
  )
{
  EFI_CPUID_REGISTER                      Register;
  EFI_STATUS                              Status;

  RAS_DEBUG((LEVEL_FUNC_FLOW, "%a Entry,McBank: %d,Socket: %d,ErrorType: %d,Severity: %d\n", \
             __FUNCTION__, McBank, Socket, ErrorType, Severity));

  if (McErrSig == NULL) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: McErrSig is NULL\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (ErrSts == NULL) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "ERROR: ErrSts is NULL\n"));
    RAS_ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (ErrSts, sizeof (PROCESSOR_ERROR_RECORD));
  AsmCpuid (CPUID_VERSION_INFO, &Register.RegEax, &Register.RegEbx, &Register.RegEcx, &Register.RegEdx);

  //
  // Build Error Record Header
  //
  ErrSts->Header.SignatureStart = EFI_ERROR_RECORD_SIGNATURE_START;
  ErrSts->Header.Revision = ERROR_RECORD_REVISION; // UEFI Spec version 2.4
  ErrSts->Header.SignatureEnd = EFI_ERROR_RECORD_SIGNATURE_END;
  ErrSts->Header.SectionCount = 1;  // Only supporting one section per error record
  ErrSts->Header.ErrorSeverity = Severity;
  ErrSts->Header.ValidationBits = 0;
  ErrSts->Header.RecordLength = sizeof (PROCESSOR_ERROR_RECORD);
  CopyGuid (&ErrSts->Header.NotificationType, &gEfiEventNotificationTypeMceGuid);
  if (Severity == EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTED) {
    ErrSts->Header.Flags = EFI_HW_ERROR_FLAGS_RECOVERED;
  }

  //
  // Fill error descriptor
  //
  ErrSts->Descriptor.SectionOffset = sizeof (EFI_COMMON_ERROR_RECORD_HEADER) + sizeof (EFI_ERROR_SECTION_DESCRIPTOR);
  ErrSts->Descriptor.SectionLength = sizeof (EMCA_PROCESSOR_ERROR_SECTION);
  ErrSts->Descriptor.Revision = ERROR_RECORD_REVISION; // UEFI Spec version 2.4
  ErrSts->Descriptor.SecValidMask = 0;
  ErrSts->Descriptor.SectionFlags = 1;   // Primary
  CopyGuid (&ErrSts->Descriptor.SectionType, &gEfiProcessorGenericErrorSectionGuid);
  ErrSts->Descriptor.Severity = Severity;

  //
  // Fill error section body
  //
  ErrSts->EmcaProcErrorSection.ProcessorGenericErrorData.Type = EFI_GENERIC_ERROR_PROC_TYPE_IA32_X64;
  ErrSts->EmcaProcErrorSection.ProcessorGenericErrorData.Isa = EFI_GENERIC_ERROR_PROC_ISA_X64;
  ErrSts->EmcaProcErrorSection.ProcessorGenericErrorData.ErrorType = ErrorType;
  ErrSts->EmcaProcErrorSection.ProcessorGenericErrorData.Operation = 0;  // Generic error
  ErrSts->EmcaProcErrorSection.ProcessorGenericErrorData.VersionInfo = Register.RegEax;
  ErrSts->EmcaProcErrorSection.ProcessorGenericErrorData.ApicId = McErrSig->ApicId;
  ErrSts->EmcaProcErrorSection.ProcessorGenericErrorData.ValidFields = EFI_GENERIC_ERROR_PROC_TYPE_VALID | EFI_GENERIC_ERROR_PROC_ISA_VALID | EFI_GENERIC_ERROR_PROC_ERROR_TYPE_VALID | \
    EFI_GENERIC_ERROR_PROC_OPERATION_VALID | EFI_GENERIC_ERROR_PROC_VERSION_VALID | EFI_GENERIC_ERROR_PROC_ID_VALID;

  Status = FillMcaBankInfo (McBank, McErrSig, &(ErrSts->EmcaProcErrorSection.McaBankInfo));

  return Status;
}

/**
  This function checks AER UNCERRMSK against UNCERRSTS and CORERRMSK against CORERRSTS.
  If the error bit set on UNCERRSTS or CORERRSTS is masked on UNCERRMSK or UNCERRSTS accordingly, return false.
  Otherwise, the error is valid and returns true.

  @param[in]  ErrorRec  CPER PCIe error section.
  @param[in]  ErrorType  Error Severity.

  @retval     AER error status is valid or not.
--*/
BOOLEAN
ValidateErrorRecord (
  PCIE_ERROR_SECTION CONST * CONST ErrorRec,
  IN      UINT8                    ErrorType
  )
{
  BOOLEAN     IsValid = FALSE;

  if (ErrorRec == NULL) {
    return IsValid;
  }

 switch (ErrorType) {
    case SERROR_TYPE:
    case PERROR_TYPE:
      IsValid = TRUE;
      break;
    case COR_ERROR_TYPE:
      IsValid = ((~ErrorRec->Corerrmsk) & ErrorRec->Corerrsts) ? TRUE : FALSE;
      break;
    case NONFATAL_ERROR_TYPE:
    case FATAL_ERROR_TYPE:
      IsValid = ((~ErrorRec->Uncerrmsk) & ErrorRec->Uncerrsts) ? TRUE : FALSE;
      break;
    default:
      IsValid = FALSE;
  }
  return IsValid;
}


EFI_STATUS
CreatePcieErrorRecord (
  IN      UINT8                    Socket,
  IN      UINT8                    Bus,
  IN      UINT8                    Device,
  IN      UINT8                    Function,
  IN      VOID                     *MmInfo,
  IN      UINT8                    ErrorType,
  OUT     PCIE_ERROR_RECORD        *PciErrRecord
  )
{

  UINT16                    ErrType;
  EFI_STATUS                Status;
  UINT64                    ValidBits;

  RAS_DEBUG((LEVEL_FUNC_FLOW, "%a Entry,Bus: %d,Device: %d,Function: %d,ErrorType: %d\n", \
             __FUNCTION__, Bus, Device, Function, ErrorType));

  ZeroMem (PciErrRecord, sizeof (PCIE_ERROR_RECORD));

  //
  // Build Error Record Header
  //
  PciErrRecord->Header.SignatureStart = EFI_ERROR_RECORD_SIGNATURE_START;
  PciErrRecord->Header.Revision = ERROR_RECORD_REVISION;
  PciErrRecord->Header.SignatureEnd = EFI_ERROR_RECORD_SIGNATURE_END;
  PciErrRecord->Header.SectionCount = 1;  // Only supporting one section per error record
  PciErrRecord->Header.ValidationBits = 0;
  PciErrRecord->Header.RecordLength = sizeof (PCIE_ERROR_RECORD);
  CopyGuid (&PciErrRecord->Header.NotificationType, &gEfiEventNotificationTypePcieGuid);

  //
  // Fill error section
  //
  PciErrRecord->PcieErrorSection.Section.Version = 0x0300;
  PciErrRecord->PcieErrorSection.Section.CommandStatus = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_COMMAND_OFFSET, MmInfo));
  PciErrRecord->PcieErrorSection.Section.DevBridge.VendorId = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_VENDOR_ID_OFFSET, MmInfo));
  PciErrRecord->PcieErrorSection.Section.DevBridge.DeviceId = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_DEVICE_ID_OFFSET, MmInfo));
  PciErrRecord->PcieErrorSection.Section.DevBridge.ClassCode[0] = MmioRead8  (MmPciAddressExt (Socket, Bus, Device, Function, PCI_CLASSCODE_OFFSET, MmInfo));
  PciErrRecord->PcieErrorSection.Section.DevBridge.ClassCode[1] = MmioRead8  (MmPciAddressExt (Socket, Bus, Device, Function, PCI_CLASSCODE_OFFSET + 1, MmInfo));
  PciErrRecord->PcieErrorSection.Section.DevBridge.ClassCode[2] = MmioRead8  (MmPciAddressExt (Socket, Bus, Device, Function, PCI_CLASSCODE_OFFSET + 2, MmInfo));
  PciErrRecord->PcieErrorSection.Section.DevBridge.Function = Function;
  PciErrRecord->PcieErrorSection.Section.DevBridge.Device = Device;
  PciErrRecord->PcieErrorSection.Section.DevBridge.Segment = 0;
  PciErrRecord->PcieErrorSection.Section.DevBridge.PrimaryOrDeviceBus = Bus;

  ValidBits = 0;

  if ((PciErrRecord->PcieErrorSection.Section.DevBridge.ClassCode[2] == PCI_CLASS_BRIDGE) && (PciErrRecord->PcieErrorSection.Section.DevBridge.ClassCode[1] == PCI_CLASS_BRIDGE_P2P) &&
      ((PciErrRecord->PcieErrorSection.Section.DevBridge.ClassCode[0] == PCI_IF_BRIDGE_P2P) || (PciErrRecord->PcieErrorSection.Section.DevBridge.ClassCode[0] == PCI_IF_BRIDGE_P2P))) {
    //
    //Yes, it is PCI bridge
    //Fill bridge information
    //
    PciErrRecord->PcieErrorSection.Section.DevBridge.SecondaryBus = MmioRead8 (MmPciAddressExt(Socket, Bus, Device, Function, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, MmInfo));   // Secondary Bus Number
    //
    // Bridge Control
    //
    PciErrRecord->PcieErrorSection.Section.BridgeControlStatus = (UINT32) (MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_BRIDGE_CONTROL_REGISTER_OFFSET, MmInfo)) << 16 |\
                                                         MmioRead16 (MmPciAddressExt(Socket, Bus, Device, Function, PCI_BRIDGE_STATUS_REGISTER_OFFSET, MmInfo)));
    //
    // Set Bridge information valid
    //
    ValidBits |= EFI_PCIE_ERROR_BRIDGE_CRL_STS_VALID;
    PciErrRecord->PcieErrorSection.Section.DevBridge.Slot.Number = PcieGetSlotNum (Socket,Bus, Device, Function, MmInfo);
  }
  Status = PcieGetCapabilities (
             &(PciErrRecord->PcieErrorSection.Section.Capability.PcieCap[0]),
             Socket,
             Bus,
             Device,
             Function,
             MmInfo
             );
  if (Status == EFI_SUCCESS) {
    PciErrRecord->PcieErrorSection.Section.PortType = ((PciErrRecord->PcieErrorSection.Section.Capability.PcieCap[2]) >> 4);
    ValidBits |= EFI_PCIE_ERROR_CAPABILITY_INFO_VALID | EFI_PCIE_ERROR_PORT_TYPE_VALID;
  }

  Status = PcieAerGetErrCapData (Socket, Bus, Device, Function, MmInfo, (EFI_PCIE_ERROR_DATA_CAPABILITY *)&(PciErrRecord->PcieErrorSection.Section.AerInfo.PcieAer[0]));
  if (Status == EFI_SUCCESS) {
    ValidBits |= EFI_PCIE_ERROR_AER_INFO_VALID;
    PciErrRecord->PcieErrorSection.Uncerrsts = PcieAerGetUncErrSts (Socket, Bus, Device, Function, MmInfo);
    PciErrRecord->PcieErrorSection.Corerrsts = PcieAerGetCorrErrSts (Socket, Bus, Device, Function, MmInfo);

    PcieAerGetRpErrSts (Socket, Bus, Device, Function, MmInfo, &PciErrRecord->PcieErrorSection.Rperrsts);
    PciErrRecord->PcieErrorSection.Uncerrmsk = PcieAerGetUncErrMsk (Socket, Bus, Device, Function, MmInfo);
    PciErrRecord->PcieErrorSection.Corerrmsk = PcieGetCorrErrMsk (Socket, Bus, Device, Function, MmInfo);
  }


  //
  // Fill error descriptor
  //
  PciErrRecord->Descriptor.SectionOffset = sizeof (EFI_COMMON_ERROR_RECORD_HEADER) + sizeof (EFI_ERROR_SECTION_DESCRIPTOR);
  PciErrRecord->Descriptor.SectionLength = sizeof (PCIE_ERROR_SECTION);
  PciErrRecord->Descriptor.Revision = ERROR_RECORD_REVISION; // UEFI Spec version 2.4
  PciErrRecord->Descriptor.SecValidMask = 0;
  PciErrRecord->Descriptor.SectionFlags = 1;   // Primary
  CopyGuid (&PciErrRecord->Descriptor.SectionType, &gEfiPcieErrorSectionGuid);

  PciErrRecord->PcieErrorSection.Section.ValidFields = ValidBits;
  if (!ValidateErrorRecord(&(PciErrRecord->PcieErrorSection), ErrorType)) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW,"CreatePcieErrorRecord Error in device: Error(s) found on device is masked bus-%x,dev-%x,Func-%x \n", Bus , Device, Function));
    //
    // Error severity updated from the input argument based on the previous boot IOMCA bank status where the uncorrected AER and other CSR error bits cleared,
    //
    if (ErrorType == FATAL_ERROR_TYPE) {
      PciErrRecord->Header.ErrorSeverity  = EFI_ACPI_6_2_ERROR_SEVERITY_FATAL;
      PciErrRecord->Descriptor.Severity   = EFI_ACPI_6_2_ERROR_SEVERITY_FATAL;
    }
    if (ErrorType == NONFATAL_ERROR_TYPE) {
      PciErrRecord->Header.ErrorSeverity  = EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTABLE;
      PciErrRecord->Descriptor.Severity   = EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTABLE;
    }
    return INVALID_ERROR_TYPE;
  }
  switch (ErrorType) {
    case SERROR_TYPE:
      ErrType = EFI_ACPI_6_2_ERROR_SEVERITY_FATAL;
      break;
    case PERROR_TYPE:
      ErrType = EFI_ACPI_6_2_ERROR_SEVERITY_FATAL;
      break;
    case COR_ERROR_TYPE:
      ErrType = EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTED;
      break;
    case NONFATAL_ERROR_TYPE:
      ErrType = EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTABLE;
      break;
    case FATAL_ERROR_TYPE:
      ErrType = EFI_ACPI_6_2_ERROR_SEVERITY_FATAL;
      break;
    default:
      return EFI_INVALID_PARAMETER;
  }
  RAS_DEBUG((LEVEL_FUNC_FLOW, "CreatePcieErrorRecord, ErrType = %x\n", ErrType));
  if (ErrType == EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTED) {
    PciErrRecord->Header.Flags = EFI_HW_ERROR_FLAGS_RECOVERED;
  }

  if (ErrorType == PERROR_TYPE || ErrorType == SERROR_TYPE) {//missed legacy PCI serr/perr
    PciErrRecord->Header.ErrorSeverity = ErrorType;
  } else {
    PciErrRecord->Header.ErrorSeverity = ErrType;
  }

  PciErrRecord->Descriptor.Severity = ErrType;
  PciErrRecord->PcieErrorSection.Section.ValidFields |= EFI_PCIE_ERROR_VERSION_VALID | EFI_PCIE_ERROR_COMMAND_STATUS_VALID | EFI_PCIE_ERROR_DEVICE_ID_VALID;
  return EFI_SUCCESS;
}

/**
  Create a firmware error record.

  This function fills in the common fields of the header, descriptor, and
  error data. Any payload data must be copied to the end of the record by the
  caller.

  The record ID GUID is optional; if null on entry, the GUID will be zeroed
  out in the output record.

  @param[in]  ErrorType                 The firmware-specific error type.
  @param[in]  Severity                  The severity of the error.
  @param[in]  RecordId                  The record ID number.
  @param[in]  RecordIdGuid              The record ID GUID.
  @param[in]  PayloadLength             The payload length the caller intends
                                        to append to the record.
  @param[out] Record                    The record to be filled in.

  @retval     EFI_SUCCESS               The error record was filled in.
  @retval     EFI_INVALID_PARAMETER     Record was null on entry.
**/
EFI_STATUS
CreateFirmwareErrorRecord (
  IN  UINT8                         ErrorType,
  IN  UINT32                        Severity,
  IN  UINT64                        RecordId,
  IN  EFI_GUID                      *RecordIdGuid,  OPTIONAL
  IN  UINT32                        PayloadLength,
  OUT FIRMWARE_ERROR_RECORD         *Record
  )
{
  if (Record == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Entry, ErrorType: %d, ", __FUNCTION__, ErrorType));
  if (RecordIdGuid == NULL) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "RecordId: 0x%x\n", RecordId));
  } else {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "RecordIdGuid: %g\n", RecordIdGuid));
  }

  ZeroMem (Record, sizeof (FIRMWARE_ERROR_RECORD));

  //
  // Fill error record header
  //
  Record->Header.SignatureStart = EFI_ERROR_RECORD_SIGNATURE_START;
  Record->Header.Revision = ERROR_RECORD_REVISION;
  Record->Header.SignatureEnd = EFI_ERROR_RECORD_SIGNATURE_END;
  Record->Header.SectionCount = 1;  // Only supporting one section per error record
  Record->Header.ErrorSeverity = Severity;
  Record->Header.ValidationBits = 0;
  Record->Header.RecordLength = sizeof (FIRMWARE_ERROR_RECORD) + PayloadLength;
  CopyGuid (&Record->Header.NotificationType, &gEfiEventNotificationTypeBootGuid);

  //
  // Fill error section descriptor
  //
  Record->Descriptor.SectionOffset = OFFSET_OF (FIRMWARE_ERROR_RECORD, Section);
  Record->Descriptor.SectionLength = sizeof (EFI_FIRMWARE_ERROR_DATA) + PayloadLength;
  Record->Descriptor.Revision = ERROR_RECORD_REVISION;
  Record->Descriptor.SecValidMask = 0;
  Record->Descriptor.SectionFlags = EFI_ERROR_SECTION_FLAGS_PRIMARY;
  CopyGuid (&Record->Descriptor.SectionType, &gEfiFirmwareErrorSectionGuid);
  Record->Descriptor.Severity = Severity;

  //
  // Fill error section data
  //
  Record->Section.ErrorType = ErrorType;
  Record->Section.Revision = FIRMWARE_ERROR_RECORD_REVISION;
  Record->Section.RecordId = RecordId;
  if (RecordIdGuid == NULL) {
    ZeroMem (&Record->Section.RecordIdGuid, sizeof (EFI_GUID));
  } else {
    CopyGuid (&Record->Section.RecordIdGuid, RecordIdGuid);
  }

  return EFI_SUCCESS;
}

/**
  Report platform memory error log for MCA.

  @param[in]  McBank      - MC Bank the error was detected on
  @param[in]  Socket         - The socket the MCE occured on
  @param[in]  Severity         - The error severity
  @param[in]  PlatformMemoryErrorData       - Pointer to data structure EFI_PLATFORM_MEMORY_ERROR_DATA
  @param[in]  McErrSig    - MC Error signature

  @retval EFI_STATUS
**/
EFI_STATUS
EFIAPI
ReportPlatMemErrorLog (
  IN  UINT8                            McBank,
  IN  UINT8                            Socket,
  IN  UINT32                           Severity,
  IN  EFI_PLATFORM_MEMORY_ERROR_DATA  *PlatformMemoryErrorData,
  IN  EMCA_MC_SIGNATURE_ERR_RECORD    *McErrSig
  )
{
  EFI_STATUS                  Status = EFI_SUCCESS;
  EMCA_MEM_ERROR_RECORD       MemRecord;
  UINTN                       ProcessorNumber;

  RAS_DEBUG((LEVEL_FUNC_FLOW, "%a Entry\n", __FUNCTION__));

  Status = CreatePlatformMemoryErrorRecord (McBank, Socket, Severity, PlatformMemoryErrorData, McErrSig, &MemRecord);

  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header SignatureStart: 0x%X\n", MemRecord.Header.SignatureStart));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header Revision: 0x%X\n", MemRecord.Header.Revision));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header SignatureEnd: 0x%X\n", MemRecord.Header.SignatureEnd));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header SectionCount: 0x%X\n", MemRecord.Header.SectionCount));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header ErrorSeverity: 0x%X\n", MemRecord.Header.ErrorSeverity));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header ValidationBits: 0x%X\n", MemRecord.Header.ValidationBits));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header RecordLength: 0x%X\n", MemRecord.Header.RecordLength));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header TimeStamp: 0x%lX\n", MemRecord.Header.TimeStamp));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header PlatformID: 0x%g\n", MemRecord.Header.PlatformID));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header CreatorID: 0x%g\n", MemRecord.Header.CreatorID));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header NotificationType: 0x%g\n", MemRecord.Header.NotificationType));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header RecordID: 0x%lX\n", MemRecord.Header.RecordID));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header Flags: 0x%X\n", MemRecord.Header.Flags));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header PersistenceInfo: 0x%lX\n", MemRecord.Header.PersistenceInfo));

  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Descriptor.Severity: 0x%lX\n", MemRecord.Descriptor.Severity));

  Status = mSmmCpuServiceProtocolInterface->WhoAmI (mSmmCpuServiceProtocolInterface, &ProcessorNumber);
  ASSERT_EFI_ERROR (Status);

  if (ProcessorNumber == gSmst->CurrentlyExecutingCpu) {
    REPORT_STATUS_CODE_WITH_EXTENDED_DATA (
      EFI_ERROR_CODE,
      SMM_RAS_ENHANCED_ERROR_LOG,
      &MemRecord.Header,
      sizeof (EMCA_MEM_ERROR_RECORD)
    );
  } else {
    AcquireSpinLock (&mMemErrorRecordUpdateLock);
    //
    // BIOS Guard does not allow non-SMM BSP write EFI variable, so copy non-SMM BSP error record in error record list in AP handler,
    // then report error in BSP handler.
    //
    if (mMemErrorRecordList->ErrorNum < MEM_ERROR_RECORD_BUFFER_NUM) {
      CopyMem (&mMemErrorRecordList->MemErrorRecordBuffer[mMemErrorRecordList->ErrorNum], &MemRecord, sizeof (EMCA_MEM_ERROR_RECORD));
      mMemErrorRecordList->ErrorNum++;
    } else {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "AP memory record buffer is full!\n"));
    }
    ReleaseSpinLock (&mMemErrorRecordUpdateLock);
  }

  RAS_DEBUG((LEVEL_FUNC_FLOW, "%a Exit,%r\n", __FUNCTION__, Status));

  return Status;
}


/**
  Report AP platform memory error log for MCA.

  BIOS Guard does not allow non-SMM BSP write EFI variable, so report AP memory error in BSP handler.

**/
VOID
EFIAPI
ReportApPlatMemErrorLog (
  VOID
  )
{
  EFI_STATUS             Status = EFI_SUCCESS;
  UINTN                  Index;

  RAS_DEBUG((LEVEL_FUNC_FLOW, "%a Entry\n", __FUNCTION__));
  for (Index =0; Index < mMemErrorRecordList->ErrorNum; Index++ ) {
    REPORT_STATUS_CODE_WITH_EXTENDED_DATA (
      EFI_ERROR_CODE,
      SMM_RAS_ENHANCED_ERROR_LOG,
      &mMemErrorRecordList->MemErrorRecordBuffer[Index].Header,
      sizeof (EMCA_MEM_ERROR_RECORD)
    );
  }

  ZeroMem (mMemErrorRecordList, sizeof(EMCA_MEM_ERROR_RECORD_LIST));

  RAS_DEBUG((LEVEL_FUNC_FLOW, "%a Exit,%r\n", __FUNCTION__, Status));
}

/**
  Report corrected memory error log for iMC.

  @param[in]  CorrectedMemoryErrorData       - Pointer to data structure EFI_PLATFORM_MEMORY_ERROR_DATA
  @param[in]  McErrSig    - MC Error signature

  @retval EFI_STATUS
**/
EFI_STATUS
EFIAPI
ReportCorrectedMemErrorLog (
  IN  EFI_PLATFORM_MEMORY_ERROR_DATA  *CorrectedMemoryErrorData
  )
{
  return ReportMemErrorLog (EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTED, CorrectedMemoryErrorData);
}

/**
  Report memory error log.

  @param[in]  Severity                       - Severity
  @param[in]  CorrectedMemoryErrorData       - Pointer to data structure EFI_PLATFORM_MEMORY_ERROR_DATA

  @retval EFI_STATUS
**/
EFI_STATUS
EFIAPI
ReportMemErrorLog (
  IN  UINT8                            Severity,
  IN  EFI_PLATFORM_MEMORY_ERROR_DATA  *MemoryErrorData
  )
{
  EFI_STATUS                   Status = EFI_SUCCESS;
  MEM_ERROR_RECORD                MemRecord;

  RAS_DEBUG((LEVEL_FUNC_FLOW, "%a Entry\n", __FUNCTION__));

  Status = CreateMemoryErrorRecord (Severity, MemoryErrorData, &MemRecord);

  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header SignatureStart: 0x%X\n", MemRecord.Header.SignatureStart));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header Revision: 0x%X\n", MemRecord.Header.Revision));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header SignatureEnd: 0x%X\n", MemRecord.Header.SignatureEnd));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header SectionCount: 0x%X\n", MemRecord.Header.SectionCount));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header ErrorSeverity: 0x%X\n", MemRecord.Header.ErrorSeverity));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header ValidationBits: 0x%X\n", MemRecord.Header.ValidationBits));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header RecordLength: 0x%X\n", MemRecord.Header.RecordLength));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header TimeStamp: 0x%lX\n", MemRecord.Header.TimeStamp));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header PlatformID: 0x%g\n", MemRecord.Header.PlatformID));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header CreatorID: 0x%g\n", MemRecord.Header.CreatorID));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header NotificationType: 0x%g\n", MemRecord.Header.NotificationType));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header RecordID: 0x%lX\n", MemRecord.Header.RecordID));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header PersistenceInfo: 0x%lX\n", MemRecord.Header.PersistenceInfo));


  REPORT_STATUS_CODE_WITH_EXTENDED_DATA (
    EFI_ERROR_CODE,
    SMM_RAS_ENHANCED_ERROR_LOG,
    &MemRecord.Header,
    sizeof (MEM_ERROR_RECORD)
    );

  RAS_DEBUG((LEVEL_FUNC_FLOW, "%a Exit,%r\n", __FUNCTION__, Status));

  return Status;
}

/**
  This function reports the processor error record fields logs the appropriate type of error.

  @param[in] McBank     - The machine check bank
  @param[in] Socket           - The socket number
  @param[in] ErrorType  - The Error Type of the error
  @param[in] Severity          - The severity of the error
  @param[in] McErrSig    - MC Error signature

  @retval    EFI_STATUS
--*/
EFI_STATUS
EFIAPI
ReportProcessorErrorLog (
  IN  UINT8                          McBank,
  IN  UINT8                          Socket,
  IN  UINT8                          ErrorType,
  IN  UINT32                         Severity,
  IN  EMCA_MC_SIGNATURE_ERR_RECORD  *McErrSig
  )
{
  EFI_STATUS               Status = EFI_SUCCESS;
  PROCESSOR_ERROR_RECORD      ProcRecord;

  RAS_DEBUG((LEVEL_FUNC_FLOW, "%a Entry\n", __FUNCTION__));

  Status = CreateProcessorErrorRecord (McBank, Socket, ErrorType, Severity, McErrSig, &ProcRecord);

  RAS_DEBUG((LEVEL_FUNC_FLOW, "ProcRecord.Header SignatureStart: 0x%X\n", ProcRecord.Header.SignatureStart));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ProcRecord.Header Revision: 0x%X\n", ProcRecord.Header.Revision));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ProcRecord.Header SignatureEnd: 0x%X\n", ProcRecord.Header.SignatureEnd));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ProcRecord.Header SectionCount: 0x%X\n", ProcRecord.Header.SectionCount));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ProcRecord.Header ErrorSeverity: 0x%X\n", ProcRecord.Header.ErrorSeverity));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ProcRecord.Header ValidationBits: 0x%X\n", ProcRecord.Header.ValidationBits));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ProcRecord.Header RecordLength: 0x%X\n", ProcRecord.Header.RecordLength));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ProcRecord.Header TimeStamp: 0x%lX\n", ProcRecord.Header.TimeStamp));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ProcRecord.Header PlatformID: 0x%g\n", ProcRecord.Header.PlatformID));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ProcRecord.Header CreatorID: 0x%g\n", ProcRecord.Header.CreatorID));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ProcRecord.Header NotificationType: 0x%g\n", ProcRecord.Header.NotificationType));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ProcRecord.Header RecordID: 0x%lX\n", ProcRecord.Header.RecordID));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ProcRecord.Header PersistenceInfo: 0x%lX\n", ProcRecord.Header.PersistenceInfo));

  REPORT_STATUS_CODE_WITH_EXTENDED_DATA (
    EFI_ERROR_CODE,
    SMM_RAS_ENHANCED_ERROR_LOG,
    &ProcRecord.Header,
    sizeof (PROCESSOR_ERROR_RECORD)
    );

  RAS_DEBUG((LEVEL_FUNC_FLOW, "%a Exit,%r\n", __FUNCTION__, Status));

  return Status;
}


/**
  This function reports the PCIE error record fields logs the appropriate type of error.

  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.
  @param[in] ErrorType      The type of error record

  @retval    EFI_STATUS
--*/
EFI_STATUS
EFIAPI
ReportPcieErrorLog (
  IN  UINT8             Socket,
  IN  UINT8             Bus,
  IN  UINT8             Device,
  IN  UINT8             Function,
  IN  VOID              *MmInfo,
  IN  UINT8             ErrorType
  )
{

  EFI_STATUS          Status = EFI_SUCCESS;
  PCIE_ERROR_RECORD      PciErrRecord;

  RAS_DEBUG((LEVEL_FUNC_FLOW, "%a Entry\n", __FUNCTION__));

  Status = CreatePcieErrorRecord (Socket, Bus, Device, Function, MmInfo, ErrorType, &PciErrRecord);

  RAS_DEBUG((LEVEL_FUNC_FLOW, "PciErrRecord.Header SignatureStart: 0x%X\n", PciErrRecord.Header.SignatureStart));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "PciErrRecord.Header Revision: 0x%X\n", PciErrRecord.Header.Revision));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "PciErrRecord.Header SignatureEnd: 0x%X\n", PciErrRecord.Header.SignatureEnd));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "PciErrRecord.Header SectionCount: 0x%X\n", PciErrRecord.Header.SectionCount));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "PciErrRecord.Header ErrorSeverity: 0x%X\n", PciErrRecord.Header.ErrorSeverity));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "PciErrRecord.Header ValidationBits: 0x%X\n", PciErrRecord.Header.ValidationBits));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "PciErrRecord.Header RecordLength: 0x%X\n", PciErrRecord.Header.RecordLength));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "PciErrRecord.Header TimeStamp: 0x%lX\n", PciErrRecord.Header.TimeStamp));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "PciErrRecord.Header PlatformID: 0x%g\n", PciErrRecord.Header.PlatformID));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "PciErrRecord.Header CreatorID: 0x%g\n", PciErrRecord.Header.CreatorID));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "PciErrRecord.Header NotificationType: 0x%g\n", PciErrRecord.Header.NotificationType));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "PciErrRecord.Header RecordID: 0x%lX\n", PciErrRecord.Header.RecordID));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "PciErrRecord.Header PersistenceInfo: 0x%lX\n", PciErrRecord.Header.PersistenceInfo));

  REPORT_STATUS_CODE_WITH_EXTENDED_DATA (
    EFI_ERROR_CODE,
    SMM_RAS_ENHANCED_ERROR_LOG,
    &PciErrRecord.Header,
    sizeof (PCIE_ERROR_RECORD)
    );

  RAS_DEBUG((LEVEL_FUNC_FLOW, "%a Exit,%r\n", __FUNCTION__, Status));

  return Status;
}


/**
  This function reports the PCIE boot error record fields logs the appropriate type of error.

  @param[in] Socket                      The socket number.
  @param[in] Bus                         Device's bus number.
  @param[in] Device                      Device's device number.
  @param[in] Function                    Device's function number.
  @param[in] MmInfo                      Memory Map information of target device.
  @param[in] ErrorType                   The type of error record
  @param[in] BootErrorRegionAddress      Boot error region address for the BERT

  @retval    EFI_STATUS
--*/
EFI_STATUS
EFIAPI
ReportPcieBootErrorLog (
  IN  UINT8                   Socket,
  IN  UINT8                   Bus,
  IN  UINT8                   Device,
  IN  UINT8                   Function,
  IN  VOID                    *MmInfo,
  IN  UINT8                   ErrorType,
  IN  UINT64                  BootErrorRegionAddress
  )
{
  EFI_STATUS          Status = EFI_SUCCESS;
  PCIE_ERROR_RECORD      PciErrRecord;
  RAS_DEBUG((LEVEL_FUNC_FLOW, "%a Entry\n", __FUNCTION__));
  Status = CreatePcieErrorRecord (Socket, Bus, Device, Function, MmInfo, ErrorType, &PciErrRecord);

  RAS_DEBUG((LEVEL_FUNC_FLOW, "PciErrRecord.Header SignatureStart: 0x%X\n", PciErrRecord.Header.SignatureStart));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "PciErrRecord.Header Revision: 0x%X\n", PciErrRecord.Header.Revision));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "PciErrRecord.Header SignatureEnd: 0x%X\n", PciErrRecord.Header.SignatureEnd));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "PciErrRecord.Header SectionCount: 0x%X\n", PciErrRecord.Header.SectionCount));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "PciErrRecord.Header ErrorSeverity: 0x%X\n", PciErrRecord.Header.ErrorSeverity));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "PciErrRecord.Header ValidationBits: 0x%X\n", PciErrRecord.Header.ValidationBits));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "PciErrRecord.Header RecordLength: 0x%X\n", PciErrRecord.Header.RecordLength));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "PciErrRecord.Header TimeStamp: 0x%lX\n", PciErrRecord.Header.TimeStamp));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "PciErrRecord.Header PlatformID: 0x%g\n", PciErrRecord.Header.PlatformID));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "PciErrRecord.Header CreatorID: 0x%g\n", PciErrRecord.Header.CreatorID));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "PciErrRecord.Header NotificationType: 0x%g\n", PciErrRecord.Header.NotificationType));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "PciErrRecord.Header RecordID: 0x%lX\n", PciErrRecord.Header.RecordID));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "PciErrRecord.Header PersistenceInfo: 0x%lX\n", PciErrRecord.Header.PersistenceInfo));

  Status = LogErrorToBert (&PciErrRecord.Header, BootErrorRegionAddress);
  RAS_DEBUG((LEVEL_FUNC_FLOW, "%a Exit,%r\n", __FUNCTION__, Status));
  return Status;
}


/**
  Report platform memory boot error log for MCA.

  @param[in] McBank                       MC Bank the error was detected on
  @param[in] Socket                       The socket the MCE occured on
  @param[in] Severity                     The error severity
  @param[in] PlatformMemoryErrorData      Pointer to data structure EFI_PLATFORM_MEMORY_ERROR_DATA
  @param[in] McErrSig                     MC Error signature
  @param[in] BootErrorRegionAddress       Boot error region address for the BERT

  @retval EFI_STATUS
**/
EFI_STATUS
EFIAPI
ReportPlatMemBootErrorLog (
  IN  UINT8                            McBank,
  IN  UINT8                            Socket,
  IN  UINT32                           Severity,
  IN  EFI_PLATFORM_MEMORY_ERROR_DATA  *PlatformMemoryErrorData,
  IN  EMCA_MC_SIGNATURE_ERR_RECORD    *McErrSig,
  IN  UINT64                           BootErrorRegionAddress
  )
{
  EFI_STATUS          Status = EFI_SUCCESS;
  EMCA_MEM_ERROR_RECORD  MemRecord;

  RAS_DEBUG((LEVEL_FUNC_FLOW, "%a Entry\n", __FUNCTION__));

  Status = CreatePlatformMemoryErrorRecord (McBank, Socket, Severity, PlatformMemoryErrorData, McErrSig, &MemRecord);

  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header SignatureStart: 0x%X\n", MemRecord.Header.SignatureStart));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header Revision: 0x%X\n", MemRecord.Header.Revision));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header SignatureEnd: 0x%X\n", MemRecord.Header.SignatureEnd));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header SectionCount: 0x%X\n", MemRecord.Header.SectionCount));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header ErrorSeverity: 0x%X\n", MemRecord.Header.ErrorSeverity));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header ValidationBits: 0x%X\n", MemRecord.Header.ValidationBits));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header RecordLength: 0x%X\n", MemRecord.Header.RecordLength));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header TimeStamp: 0x%lX\n", MemRecord.Header.TimeStamp));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header PlatformID: 0x%g\n", MemRecord.Header.PlatformID));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header CreatorID: 0x%g\n", MemRecord.Header.CreatorID));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header NotificationType: 0x%g\n", MemRecord.Header.NotificationType));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header RecordID: 0x%lX\n", MemRecord.Header.RecordID));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header Flags: 0x%X\n", MemRecord.Header.Flags));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Header PersistenceInfo: 0x%lX\n", MemRecord.Header.PersistenceInfo));

  RAS_DEBUG((LEVEL_FUNC_FLOW, "MemRecord.Descriptor.Severity: 0x%lX\n", MemRecord.Descriptor.Severity));

  Status = LogErrorToBert (&MemRecord.Header, BootErrorRegionAddress);
  RAS_DEBUG((LEVEL_FUNC_FLOW, "%a Exit,%r\n", __FUNCTION__, Status));

  return Status;
}



/**
  This function reports the processor error record fields logs the appropriate type of error.

  @param[in] McBank                      The machine check bank
  @param[in] Socket                      The socket number
  @param[in] ErrorType                   The Error Type of the error
  @param[in] Severity                    The severity of the error
  @param[in] McErrSig                    MC Error signature
  @param[in] BootErrorRegionAddress      Boot error region address for the BERT

  @retval    EFI_STATUS
--*/
EFI_STATUS
EFIAPI
ReportProcessorBootErrorLog (
  IN  UINT8                          McBank,
  IN  UINT8                          Socket,
  IN  UINT8                          ErrorType,
  IN  UINT32                         Severity,
  IN  EMCA_MC_SIGNATURE_ERR_RECORD  *McErrSig,
  IN  UINT64                         BootErrorRegionAddress
  )
{
  EFI_STATUS               Status = EFI_SUCCESS;
  PROCESSOR_ERROR_RECORD      ProcRecord;

  RAS_DEBUG((LEVEL_FUNC_FLOW, "%a Entry\n", __FUNCTION__));

  Status = CreateProcessorErrorRecord (McBank, Socket, ErrorType, Severity, McErrSig, &ProcRecord);

  RAS_DEBUG((LEVEL_FUNC_FLOW, "ProcRecord.Header SignatureStart: 0x%X\n", ProcRecord.Header.SignatureStart));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ProcRecord.Header Revision: 0x%X\n", ProcRecord.Header.Revision));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ProcRecord.Header SignatureEnd: 0x%X\n", ProcRecord.Header.SignatureEnd));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ProcRecord.Header SectionCount: 0x%X\n", ProcRecord.Header.SectionCount));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ProcRecord.Header ErrorSeverity: 0x%X\n", ProcRecord.Header.ErrorSeverity));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ProcRecord.Header ValidationBits: 0x%X\n", ProcRecord.Header.ValidationBits));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ProcRecord.Header RecordLength: 0x%X\n", ProcRecord.Header.RecordLength));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ProcRecord.Header TimeStamp: 0x%lX\n", ProcRecord.Header.TimeStamp));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ProcRecord.Header PlatformID: 0x%g\n", ProcRecord.Header.PlatformID));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ProcRecord.Header CreatorID: 0x%g\n", ProcRecord.Header.CreatorID));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ProcRecord.Header NotificationType: 0x%g\n", ProcRecord.Header.NotificationType));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ProcRecord.Header RecordID: 0x%lX\n", ProcRecord.Header.RecordID));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "ProcRecord.Header PersistenceInfo: 0x%lX\n", ProcRecord.Header.PersistenceInfo));

  Status = LogErrorToBert (&ProcRecord.Header, BootErrorRegionAddress);
  RAS_DEBUG((LEVEL_FUNC_FLOW, "%a Exit,%r\n", __FUNCTION__, Status));

  return Status;
}

/**
  Report a firmware boot error to BERT.

  RecordIdGuid is optional. If null, the zero GUID will be used. Payload is
  optional and may be null only if PayloadLength is zero.

  @param[in]  ErrorType                 The firmware-specific error type.
  @param[in]  Severity                  The severity of the error.
  @param[in]  RecordId                  The record ID number.
  @param[in]  RecordIdGuid              The record ID GUID.
  @param[in]  Payload                   The payload to append to the record.
  @param[in]  PayloadLength             The payload length.
  @param[in]  BootErrorRegionAddress    The boot error region address of BERT.

  @retval     EFI_SUCCESS               The error was successfully logged.
  @retval     EFI_INVALID_PARAMETER     Payload is null when PayloadLength is
                                        not zero.
  @retval     EFI_OUT_OF_RESOURCES      The record could not be allocated.
  @retval     EFI_UNSUPPORTED           The boot error region is full.
**/
EFI_STATUS
EFIAPI
ReportFirmwareBootErrorLog (
  IN  UINT8                         ErrorType,
  IN  UINT32                        Severity,
  IN  UINT64                        RecordId,
  IN  EFI_GUID                      *RecordIdGuid,  OPTIONAL
  IN  UINT8                         *Payload,       OPTIONAL
  IN  UINT32                        PayloadLength,
  IN  UINT64                        BootErrorRegionAddress
  )
{
  EFI_STATUS               Status;
  FIRMWARE_ERROR_RECORD    *Record;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Entry\n", __FUNCTION__));

  if (PayloadLength != 0 && Payload == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Record = AllocatePool (sizeof (FIRMWARE_ERROR_RECORD) + PayloadLength);
  if (Record == NULL) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a could not allocate record!\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  Status = CreateFirmwareErrorRecord (ErrorType, Severity, RecordId, RecordIdGuid, PayloadLength, Record);
  if (!EFI_ERROR (Status)) {
    if (PayloadLength != 0) {
      CopyMem ((VOID *) (Record + 1), Payload, PayloadLength);
    }
    Status = LogErrorToBert (&Record->Header, BootErrorRegionAddress);
  }

  FreePool (Record);

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit: %r\n", __FUNCTION__, Status));
  return Status;
}

/**
  Create a RAS non-standard error record.

  This function fills the common data for a RAS non-standard error into a
  given error record buffer. The buffer is passed as a pointer to a data
  structure of type RAS_NON_STANDARD_ERROR_RECORD, which is guaranteed to
  be large enough for any ErrorType. The actual length of data to be reported
  is contained in Record->Header.RecordLength on a successful return from this
  function.

  @param[in]  Severity            The error severity.
  @param[in]  ErrorType           The RAS error type.
  @param[out] Record              The error record to fill.

  @retval EFI_STATUS              The data was filled successfully.
  @retval EFI_INVALID_PARAMETER   ErrorType was an unknown value.
  @retval EFI_INVALID_PARAMETER   Record was null.
**/
EFI_STATUS
CreateRasNonStandardErrorRecord (
  IN  UINT32                        Severity,
  IN  RAS_ERROR_TYPE                ErrorType,
  OUT RAS_NON_STANDARD_ERROR_RECORD *Record
  )
{
  UINT32  SectionLength;
  UINT32  Revision;

  RAS_DEBUG ((
    LEVEL_FUNC_FLOW,
    "%a Entry, Severity: %d, ErrorType: %d\n",
    __FUNCTION__,
    Severity,
    ErrorType
    ));

  RAS_ASSERT (Record != NULL);
  if (Record == NULL) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "ERROR: Record is null\n"));
    return EFI_INVALID_PARAMETER;
  }

  RAS_ASSERT (ErrorType < RasErrorTypeMax);
  switch (ErrorType) {
  case RasErrorTypeSparing:
    SectionLength = sizeof (RAS_SPARING_ERROR_SECTION);
    Revision = RAS_SPARING_ERROR_SECTION_REVISION;
    break;
  case RasErrorTypeUpiFailover:
    SectionLength = sizeof (RAS_UPI_FAILOVER_ERROR_SECTION);
    Revision = RAS_UPI_FAILOVER_ERROR_SECTION_REVISION;
    break;
  case RasErrorTypeIeh:
    SectionLength = sizeof (RAS_IEH_ERROR_SECTION);
    Revision = RAS_IEH_ERROR_SECTION_REVISION;
    break;
  case RasErrorTypeIioInternal:
    SectionLength = sizeof (RAS_IIO_INTERNAL_ERROR_SECTION);
    Revision = RAS_IIO_INTERNAL_ERROR_SECTION_REVISION;
    break;
  case RasErrorTypeIioXpError:
    SectionLength = sizeof (RAS_IIO_XP_ERROR_SECTION);
    Revision = RAS_IIO_XP_ERROR_SECTION_REVISION;
    break;
  case RasErrorTypeSpdSmbus:
    SectionLength = sizeof (RAS_SPD_SMBUS_ERROR_SECTION);
    Revision = RAS_SPD_SMBUS_ERROR_SECTION_REVISION;
    break;
  case RasErrorTypeDdrtAlert:
    SectionLength = sizeof (RAS_DDRT_ALERT_ERROR_SECTION);
    Revision = RAS_DDRT_ALERT_ERROR_SECTION_REVISION;
    break;
  case RasErrorTypeSpareCore:
    SectionLength = sizeof (RAS_SPARE_CORE_ERROR_SECTION);
    Revision = RAS_SPARE_CORE_ERROR_SECTION_REVISION;
    break;
  case RasErrorTypeUboxOrder:
    SectionLength = sizeof (RAS_UBOX_ORDER_ERROR_SECTION);
    Revision = RAS_UBOX_ORDER_ERROR_SECTION_REVISION;
    break;
  case RasErrorTypeLeakyBucket:
    SectionLength = sizeof (RAS_LEAKY_BUCKET_ERROR_SECTION);
    Revision = RAS_LEAKY_BUCKET_ERROR_SECTION_REVISION;
    break;
  case RasErrorTypeUpiError:
    SectionLength = sizeof (RAS_UPI_ERROR_SECTION);
    Revision = RAS_UPI_ERROR_SECTION_REVISION;
    break;
  default:
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "ERROR: ErrorType is unknown (%d)\n", ErrorType));
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (Record, sizeof (RAS_NON_STANDARD_ERROR_RECORD));

  //
  // Fill common error record header.
  //
  Record->Header.SignatureStart = EFI_ERROR_RECORD_SIGNATURE_START;
  Record->Header.Revision = ERROR_RECORD_REVISION;  // UEFI Spec version 2.4
  Record->Header.SignatureEnd = EFI_ERROR_RECORD_SIGNATURE_END;
  Record->Header.SectionCount = 1;                  // one section per record
  Record->Header.ErrorSeverity = Severity;
  Record->Header.ValidationBits = 0;
  Record->Header.RecordLength = sizeof (RAS_NON_STANDARD_ERROR_RECORD) -
                                sizeof (RAS_NON_STANDARD_ERROR_SECTION) +
                                SectionLength;
  CopyGuid (&Record->Header.NotificationType, &gEfiEventNotificationTypeCmcGuid);

  //
  // Fill error descriptor.
  //
  Record->Descriptor.SectionOffset = sizeof (EFI_COMMON_ERROR_RECORD_HEADER) +
                                     sizeof (EFI_ERROR_SECTION_DESCRIPTOR);
  Record->Descriptor.SectionLength = SectionLength;
  Record->Descriptor.Revision = ERROR_RECORD_REVISION; // UEFI Spec version 2.4
  Record->Descriptor.SecValidMask = 0;
  Record->Descriptor.SectionFlags = EFI_ERROR_SECTION_FLAGS_PRIMARY;
  CopyGuid (&Record->Descriptor.SectionType, &gRasNonStandardErrorSectionGuid);
  Record->Descriptor.Severity = Severity;

  //
  // Fill error section common fields.
  //
  Record->Section.Common.ErrorType = ErrorType;
  Record->Section.Common.Revision = Revision;
  Record->Section.Common.ValidFields = 0;           // to be filled by caller

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit, Status: %r\n", __FUNCTION__, EFI_SUCCESS));
  return EFI_SUCCESS;
}

/**
  Report a memory sparing event.

  Report the details of a memory sparing operation event. Direction applies to
  sparing event types which may need to be reversed after being applied, in
  order to apply a different operation (e.g. ADDDC, PCLS).

  If an optional input parameter is INVALID_U32 on entry, the corresponding
  field is marked invalid in the log data.

  @param[in]  EventType           The type of sparing event to log.
                                  Refer to RAS_MEMORY_SPARE_EVENT_TYPE.
  @param[in]  Direction           The direction of the operation.
                                  Refer to RAS_MEMORY_SPARE_DIRECTION.
  @param[in]  CopyStatus          The status of the sparing copy.
                                  Refer to RAS_MEMORY_SPARE_COPY_STATUS.
  @param[in]  Socket              The socket number of the error(s).
  @param[in]  Channel             The channel number on the socket.
  @param[in]  EventIndex          The event index, from 0 to NumEvents-1.
  @param[in]  NumEvents           The number of expected events for this type.
  @param[in]  SourceRank          The source rank index.
  @param[in]  SourceBank          The source bank index.
  @param[in]  DestinationRank     The destination rank index.
  @param[in]  DestinationBank     The destination bank index.

  @retval EFI_SUCCESS             The data was logged successfully.
  @retval EFI_INVALID_PARAMETER   EventType, Direction, or CopyStatus was an
                                  unknown value.
  @retval EFI_INVALID_PARAMETER   EventIndex was not less than NumEvents.
  @retval EFI_OUT_OF_RESOURCES    There were not enough resources to log.
  @retval EFI_UNSUPPORTED         Report status code is not supported.
**/
EFI_STATUS
EFIAPI
ReportSpareErrorLog (
  IN  UINT32    EventType,
  IN  UINT32    Direction,        OPTIONAL
  IN  UINT32    CopyStatus,
  IN  UINT32    Socket,
  IN  UINT32    Channel,
  IN  UINT32    EventIndex,
  IN  UINT32    NumEvents,
  IN  UINT32    SourceRank,       OPTIONAL
  IN  UINT32    SourceBank,       OPTIONAL
  IN  UINT32    DestinationRank,  OPTIONAL
  IN  UINT32    DestinationBank   OPTIONAL
  )
{
  EFI_STATUS                                  Status;
  RAS_NON_STANDARD_ERROR_RECORD               Record;
  RAS_SPARING_ERROR_SECTION_VALID_FIELDS      ValidFields;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Entry\n", __FUNCTION__));

  if (EventType >= RasSpareEventMax) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "ERROR: EventType is unknown (%d)\n", EventType));
    return EFI_INVALID_PARAMETER;
  }

  if (Direction != INVALID_U32 && Direction >= RasSpareDirectionMax) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "ERROR: Direction is unknown (%d)\n", Direction));
    return EFI_INVALID_PARAMETER;
  }

  if (CopyStatus >= RasSpareCopyStatusMax) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "ERROR: CopyStatus is unknown (%d)\n", CopyStatus));
    return EFI_INVALID_PARAMETER;
  }

  if (EventIndex >= NumEvents) {
    RAS_DEBUG ((
      LEVEL_BASIC_FLOW,
      "ERROR: EventIndex is not less than NumEvents (%d >= %d)\n",
      EventIndex,
      NumEvents
      ));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Fill common fields.
  //
  Status = CreateRasNonStandardErrorRecord (
    EFI_GENERIC_ERROR_CORRECTED,
    RasErrorTypeSparing,
    &Record
    );
  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit, Status: %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Fill type specific fields. Start with 0 in ValidFields, and set the
  // correpsonding bit to 1 for each field we fill.
  //
  ValidFields.Data = 0;

  Record.Section.Sparing.EventType = EventType;
  ValidFields.Bits.EventType = 1;

  if (Direction != INVALID_U32) {
    Record.Section.Sparing.Direction = Direction;
    ValidFields.Bits.Direction = 1;
  }

  Record.Section.Sparing.CopyStatus = CopyStatus;
  ValidFields.Bits.CopyStatus = 1;

  Record.Section.Sparing.Socket = Socket;
  ValidFields.Bits.Socket = 1;

  Record.Section.Sparing.Channel = Channel;
  ValidFields.Bits.Channel = 1;

  Record.Section.Sparing.EventIndex = EventIndex;
  ValidFields.Bits.EventIndex = 1;

  Record.Section.Sparing.NumEvents = NumEvents;
  ValidFields.Bits.NumEvents = 1;

  if (SourceRank != INVALID_U32) {
    Record.Section.Sparing.SourceRank = SourceRank;
    ValidFields.Bits.SourceRank = 1;
  }

  if (SourceBank != INVALID_U32) {
    Record.Section.Sparing.SourceBank = SourceBank;
    ValidFields.Bits.SourceBank = 1;
  }

  if (DestinationRank != INVALID_U32) {
    Record.Section.Sparing.DestinationRank = DestinationRank;
    ValidFields.Bits.DestinationRank = 1;
  }

  if (DestinationBank != INVALID_U32) {
    Record.Section.Sparing.DestinationBank = DestinationBank;
    ValidFields.Bits.DestinationBank = 1;
  }

  //
  // Commit ValidFields to the section common structure.
  //
  Record.Section.Common.ValidFields = ValidFields.Data;

  Status = REPORT_STATUS_CODE_WITH_EXTENDED_DATA (
    EFI_ERROR_CODE,
    SMM_RAS_ENHANCED_ERROR_LOG,
    (VOID *) &Record,
    Record.Header.RecordLength
    );

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit, Status: %r\n", __FUNCTION__, Status));
  return Status;
}

/**
  Report a UPI failover.

  @param[in]  Socket              The socket number of the error(s).
  @param[in]  LaneIndex           The UPI lane index on the socket.
  @param[in]  FailedLanes         The encoded active lanes for the failure.
                                  Refer to RAS_UPI_FAILED_LANES.

  @retval EFI_SUCCESS             The data was logged successfully.
  @retval EFI_INVALID_PARAMETER   FailedLanes was an unknown value.
  @retval EFI_OUT_OF_RESOURCES    There were not enough resources to log.
  @retval EFI_UNSUPPORTED         Report status code is not supported.
**/
EFI_STATUS
EFIAPI
ReportUpiFailoverErrorLog (
  IN  UINT32    Socket,
  IN  UINT32    LaneIndex,
  IN  UINT32    FailedLanes
  )
{
  EFI_STATUS                                  Status;
  RAS_NON_STANDARD_ERROR_RECORD               Record;
  RAS_UPI_FAILOVER_ERROR_SECTION_VALID_FIELDS ValidFields;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Entry\n", __FUNCTION__));

  if (FailedLanes >= RasUpiFailLanesMax) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "ERROR: FailedLanes is unknown (%d)\n", FailedLanes));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Fill common fields.
  //
  Status = CreateRasNonStandardErrorRecord (
    EFI_GENERIC_ERROR_CORRECTED,
    RasErrorTypeUpiFailover,
    &Record
    );
  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit, Status: %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Fill type specific fields. Start with 0 in ValidFields, and set the
  // correpsonding bit to 1 for each field we fill.
  //
  ValidFields.Data = 0;

  Record.Section.UpiFailover.Socket = Socket;
  ValidFields.Bits.Socket = 1;

  Record.Section.UpiFailover.LaneIndex = LaneIndex;
  ValidFields.Bits.LaneIndex = 1;

  Record.Section.UpiFailover.FailedLanes = FailedLanes;
  ValidFields.Bits.FailedLanes = 1;

  //
  // Commit ValidFields to the section common structure.
  //
  Record.Section.Common.ValidFields = ValidFields.Data;

  Status = REPORT_STATUS_CODE_WITH_EXTENDED_DATA (
    EFI_ERROR_CODE,
    SMM_RAS_ENHANCED_ERROR_LOG,
    (VOID *) &Record,
    Record.Header.RecordLength
    );

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit, Status: %r\n", __FUNCTION__, Status));
  return Status;
}


/**
  Report a UPI error.

  @param[in]  Socket              The socket number of the error(s).
  @param[in]  LaneIndex           The UPI lane index on the socket.
  @param[in]  Mscod               The encoded mscod of UPI bank.
                                  Refer to RAS_UPI_FAILED_LANES.

  @retval EFI_SUCCESS             The data was logged successfully.
  @retval EFI_INVALID_PARAMETER   FailedLanes was an unknown value.
  @retval EFI_OUT_OF_RESOURCES    There were not enough resources to log.
  @retval EFI_UNSUPPORTED         Report status code is not supported.
**/
EFI_STATUS
EFIAPI
ReportUpiErrorLog (
  IN  UINT32    Socket,
  IN  UINT32    LaneIndex,
  IN  UINT32    Mscod
  )
{
  EFI_STATUS                                  Status;
  RAS_NON_STANDARD_ERROR_RECORD               Record;
  RAS_UPI_ERROR_SECTION_VALID_FIELDS          ValidFields;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Entry\n", __FUNCTION__));

  //
  // Fill common fields.
  //
  Status = CreateRasNonStandardErrorRecord (
    EFI_GENERIC_ERROR_CORRECTED,
    RasErrorTypeUpiError,
    &Record
    );
  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit, Status: %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Fill type specific fields. Start with 0 in ValidFields, and set the
  // correpsonding bit to 1 for each field we fill.
  //
  ValidFields.Data = 0;

  Record.Section.UpiError.Socket = Socket;
  ValidFields.Bits.Socket = 1;

  Record.Section.UpiError.LaneIndex = LaneIndex;
  ValidFields.Bits.LaneIndex = 1;

  Record.Section.UpiError.Mscod = Mscod;
  ValidFields.Bits.Mscod = 1;

  //
  // Commit ValidFields to the section common structure.
  //
  Record.Section.Common.ValidFields = ValidFields.Data;

  Status = REPORT_STATUS_CODE_WITH_EXTENDED_DATA (
    EFI_ERROR_CODE,
    SMM_RAS_ENHANCED_ERROR_LOG,
    (VOID *) &Record,
    Record.Header.RecordLength
    );

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit, Status: %r\n", __FUNCTION__, Status));
  return Status;
}


/**
  Report an IEH error.

  Report a "local" IEH error; i.e. one that cannot be reported via other
  standard formats.

  If an optional input parameter is INVALID_U32 on entry, the corresponding
  field is marked invalid in the log data.

  @param[in]  Socket              The socket number of the error(s).
  @param[in]  IehType             The type of IEH that had the error.
                                  Refer to RAS_IEH_TYPE.
  @param[in]  IehIndex            The index of the IEH that had the error, for
                                  types with multiple instances.
  @param[in]  IehErrorStatus      The Status of error that occurred.
  @param[in]  SourceIndex         The index of the error source, for error
                                  types with multiple sources.
  @param[in]  ErrSeverity         The error severity for the standard header.

  @retval EFI_SUCCESS             The data was logged successfully.
  @retval EFI_INVALID_PARAMETER   IehType was an unknown value.
  @retval EFI_OUT_OF_RESOURCES    There were not enough resources to log.
  @retval EFI_UNSUPPORTED         Report status code is not supported.
**/
EFI_STATUS
EFIAPI
ReportIehErrorLog (
  IN  UINT32    Socket,
  IN  UINT32    IehType,
  IN  UINT32    IehIndex,         OPTIONAL
  IN  UINT32    IehErrorStatus,
  IN  UINT32    SourceIndex,      OPTIONAL
  IN  UINT32    ErrSeverity
  )
{
  EFI_STATUS                                  Status;
  UINT32                                      Severity;
  RAS_NON_STANDARD_ERROR_RECORD               Record;
  RAS_IEH_ERROR_SECTION_VALID_FIELDS          ValidFields;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Entry\n", __FUNCTION__));

  if (IehType >= IehDevTypeEnd) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "ERROR: IehType is unknown (%d)\n", IehType));
    return EFI_INVALID_PARAMETER;
  }
  //
  // Convert the Severity to ACPI defined value
  //
  switch (ErrSeverity) {
    case COR_ERROR_TYPE:
      Severity = EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTED;
      break;
    case NONFATAL_ERROR_TYPE:
      Severity = EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTABLE;
      break;
    case FATAL_ERROR_TYPE:
      Severity = EFI_ACPI_6_2_ERROR_SEVERITY_FATAL;
      break;
    default:
      Severity = EFI_ACPI_6_2_ERROR_SEVERITY_NONE;
  }

  //
  // Fill common fields.
  //
  Status = CreateRasNonStandardErrorRecord (
    Severity,
    RasErrorTypeIeh,
    &Record
    );
  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit, Status: %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Fill type specific fields. Start with 0 in ValidFields, and set the
  // correpsonding bit to 1 for each field we fill.
  //
  ValidFields.Data = 0;

  Record.Section.Ieh.Socket = Socket;
  ValidFields.Bits.Socket = 1;

  Record.Section.Ieh.IehType = IehType;
  ValidFields.Bits.IehType = 1;

  if (IehIndex != INVALID_U32) {
    Record.Section.Ieh.IehIndex = IehIndex;
    ValidFields.Bits.IehIndex = 1;
  }

  Record.Section.Ieh.IehErrorStatus = IehErrorStatus;
  ValidFields.Bits.IehErrorStatus = 1;

  if (SourceIndex != INVALID_U32) {
    Record.Section.Ieh.SourceIndex = SourceIndex;
    ValidFields.Bits.SourceIndex = 1;
  }

  //
  // Commit ValidFields to the section common structure.
  //
  Record.Section.Common.ValidFields = ValidFields.Data;

  Status = REPORT_STATUS_CODE_WITH_EXTENDED_DATA (
    EFI_ERROR_CODE,
    SMM_RAS_ENHANCED_ERROR_LOG,
    (VOID *) &Record,
    Record.Header.RecordLength
    );

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit, Status: %r\n", __FUNCTION__, Status));
  return Status;
}

/**
  Report an IIO internal error.

  Report a "internal" IIO error; i.e. one that cannot be reported via other
  standard formats.

  @param[in]  Socket              The socket number of the error(s).
  @param[in]  IioStack            The index of the IIO stack that had the error.
  @param[in]  IioErrorType        The type of error that occurred.
                                  Refer to RAS_IIO_INTERNAL_ERROR_TYPE.
  @param[in]  ErrSeverity         The error severity for the standard header.

  @retval EFI_SUCCESS             The data was logged successfully.
  @retval EFI_INVALID_PARAMETER   IioErrorType was an unknown value.
  @retval EFI_OUT_OF_RESOURCES    There were not enough resources to log.
  @retval EFI_UNSUPPORTED         Report status code is not supported.
**/
EFI_STATUS
EFIAPI
ReportIioInternalErrorLog (
  IN  UINT32    Socket,
  IN  UINT32    IioStack,
  IN  UINT32    IioErrorType,
  IN  UINT32    ErrSeverity
  )
{
  EFI_STATUS                                  Status;
  UINT32                                      Severity;
  RAS_NON_STANDARD_ERROR_RECORD               Record;
  RAS_IIO_INTERNAL_ERROR_SECTION_VALID_FIELDS ValidFields;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Entry\n", __FUNCTION__));

  if (IioErrorType >= RasIioInternalErrorMax) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "ERROR: IioErrorType is unknown (%d)\n", IioErrorType));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Convert the Severity to ACPI defined value
  //
  switch (ErrSeverity) {
    case COR_ERROR_TYPE:
      Severity = EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTED;
      break;
    case NONFATAL_ERROR_TYPE:
      Severity = EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTABLE;
      break;
    case FATAL_ERROR_TYPE:
      Severity = EFI_ACPI_6_2_ERROR_SEVERITY_FATAL;
      break;
    default:
      Severity = EFI_ACPI_6_2_ERROR_SEVERITY_NONE;
  }

  //
  // Fill common fields.
  //
  Status = CreateRasNonStandardErrorRecord (
    Severity,
    RasErrorTypeIioInternal,
    &Record
    );
  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit, Status: %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Fill type specific fields. Start with 0 in ValidFields, and set the
  // correpsonding bit to 1 for each field we fill.
  //
  ValidFields.Data = 0;

  Record.Section.IioInternal.Socket = Socket;
  ValidFields.Bits.Socket = 1;

  Record.Section.IioInternal.IioStack = IioStack;
  ValidFields.Bits.IioStack = 1;

  Record.Section.IioInternal.IioErrorType = IioErrorType;
  ValidFields.Bits.IioErrorType = 1;

  //
  // Commit ValidFields to the section common structure.
  //
  Record.Section.Common.ValidFields = ValidFields.Data;

  Status = REPORT_STATUS_CODE_WITH_EXTENDED_DATA (
    EFI_ERROR_CODE,
    SMM_RAS_ENHANCED_ERROR_LOG,
    (VOID *) &Record,
    Record.Header.RecordLength
    );

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit, Status: %r\n", __FUNCTION__, Status));
  return Status;
}

/**
  Report an IIO Root Port XP error.

  Report a IIO Root Port XP error; i.e. one that cannot be reported via other
  standard formats.

  @param[in]  Socket              The socket number of the error(s).
  @param[in]  Bus                 The Bus number of the IIO Root port that had the error.
  @param[in]  Device              The Device number of the IIO Root port that had the error.
  @param[in]  Function            The Function number of the IIO Root port that had the error.
  @param[in]  XpCorErrSts         The XP Correctable error status of the IIO Root port that had the error.
  @param[in]  XpUncErrSts         The XP Uncorrectable error status of the IIO Root port that had the error.
  @param[in]  ErrSeverity         The error severity for the standard header.

  @retval EFI_SUCCESS             The data was logged successfully.
  @retval EFI_INVALID_PARAMETER   IioErrorType was an unknown value.
  @retval EFI_OUT_OF_RESOURCES    There were not enough resources to log.
  @retval EFI_UNSUPPORTED         Report status code is not supported.
**/
EFI_STATUS
EFIAPI
ReportIioXpErrorLog (
  IN  UINT32    Socket,
  IN  UINT8     Bus,
  IN  UINT8     Device,
  IN  UINT8     Function,
  IN  UINT32    XpCorErrSts,
  IN  UINT32    XpUncErrSts,
  IN  UINT32    ErrSeverity
  )
{
  EFI_STATUS                                  Status;
  UINT32                                      Severity;
  RAS_NON_STANDARD_ERROR_RECORD               Record;
  RAS_IIO_XP_ERROR_SECTION_VALID_FIELDS       ValidFields;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Entry\n", __FUNCTION__));

  //
  // Convert the Severity to ACPI defined value
  //
  switch (ErrSeverity) {
    case COR_ERROR_TYPE:
      Severity = EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTED;
      break;
    case NONFATAL_ERROR_TYPE:
      Severity = EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTABLE;
      break;
    case FATAL_ERROR_TYPE:
      Severity = EFI_ACPI_6_2_ERROR_SEVERITY_FATAL;
      break;
    default:
      Severity = EFI_ACPI_6_2_ERROR_SEVERITY_NONE;
  }

  //
  // Fill common fields.
  //
  Status = CreateRasNonStandardErrorRecord (
    Severity,
    RasErrorTypeIioXpError,
    &Record
    );
  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit, Status: %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Fill type specific fields. Start with 0 in ValidFields, and set the
  // correpsonding bit to 1 for each field we fill.
  //
  ValidFields.Data = 0;

  Record.Section.IioXpError.Socket = Socket;
  ValidFields.Bits.Socket = 1;

  Record.Section.IioXpError.Bus = Bus;
  ValidFields.Bits.Bus = 1;

  Record.Section.IioXpError.Device = Device;
  ValidFields.Bits.Device = 1;

  Record.Section.IioXpError.Function = Function;
  ValidFields.Bits.Function = 1;

  Record.Section.IioXpError.XpCorErrSts = XpCorErrSts;
  Record.Section.IioXpError.XpUncErrSts = XpUncErrSts;
  ValidFields.Bits.XpErrSts = 1;

  //
  // Commit ValidFields to the section common structure.
  //
  Record.Section.Common.ValidFields = ValidFields.Data;

  Status = REPORT_STATUS_CODE_WITH_EXTENDED_DATA (
    EFI_ERROR_CODE,
    SMM_RAS_ENHANCED_ERROR_LOG,
    (VOID *) &Record,
    Record.Header.RecordLength
    );

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit, Status: %r\n", __FUNCTION__, Status));
  return Status;
}

/**
  Report an SPD SMBUS error.

  @param[in]  Socket              The socket number of the error(s).
  @param[in]  SmbusSegment        The SMBUS segment index on the socket.
  @param[in]  Recovered           TRUE: recovery succeeded; FALSE: failed, or
                                  not attempted.

  @retval EFI_SUCCESS             The data was logged successfully.
  @retval EFI_OUT_OF_RESOURCES    There were not enough resources to log.
  @retval EFI_UNSUPPORTED         Report status code is not supported.
**/
EFI_STATUS
EFIAPI
ReportSpdSmbusErrorLog (
  IN  UINT32    Socket,
  IN  UINT32    SmbusSegment,
  IN  BOOLEAN   Recovered
  )
{
  EFI_STATUS                                  Status;
  RAS_NON_STANDARD_ERROR_RECORD               Record;
  RAS_SPD_SMBUS_ERROR_SECTION_VALID_FIELDS    ValidFields;
  RAS_SPD_SMBUS_FLAGS                         Flags;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Entry\n", __FUNCTION__));

  //
  // Fill common fields.
  //
  Status = CreateRasNonStandardErrorRecord (
    EFI_GENERIC_ERROR_CORRECTED,
    RasErrorTypeSpdSmbus,
    &Record
    );
  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit, Status: %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Create the flags dword from individual flag bits.
  //
  Flags.Data = 0;
  if (Recovered) {
    Flags.Bits.Recovered = 1;
  }

  //
  // Fill type specific fields. Start with 0 in ValidFields, and set the
  // correpsonding bit to 1 for each field we fill.
  //
  ValidFields.Data = 0;

  Record.Section.SpdSmbus.Socket = Socket;
  ValidFields.Bits.Socket = 1;

  Record.Section.SpdSmbus.SmbusSegment = SmbusSegment;
  ValidFields.Bits.SmbusSegment = 1;

  Record.Section.SpdSmbus.Flags = Flags.Data;
  ValidFields.Bits.Flags = 1;

  //
  // Commit ValidFields to the section common structure.
  //
  Record.Section.Common.ValidFields = ValidFields.Data;

  Status = REPORT_STATUS_CODE_WITH_EXTENDED_DATA (
    EFI_ERROR_CODE,
    SMM_RAS_ENHANCED_ERROR_LOG,
    (VOID *) &Record,
    Record.Header.RecordLength
    );

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit, Status: %r\n", __FUNCTION__, Status));
  return Status;
}


/**
  Report DDRT ECC mode switch error

  @param[in]  Socket              The socket number of the error.
  @param[in]  ChOnSkt             The channal number on the socket.
  @param[in]  Rank                The Rank number on the channel.
  @param[in]  Nibble              The failed nibble.

  @retval EFI_SUCCESS             The data was logged successfully.
  @retval EFI_OUT_OF_RESOURCES    There were not enough resources to log.
  @retval EFI_UNSUPPORTED         Report status code is not supported.
**/
EFI_STATUS
EFIAPI
ReportDdrtEccModeSwitchErrorLog (
  IN   UINT32   Socket,
  IN   UINT32   ChOnSkt,
  IN   UINT32   Rank,
  IN   UINT32   Nibble
)
{
  EFI_STATUS                                  Status;
  RAS_NON_STANDARD_ERROR_RECORD               Record;
  RAS_DDRT_ECC_ERROR_SECTION_VALID_FIELDS     ValidFields;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Entry\n", __FUNCTION__));

  //
  // Fill common fields.
  //
  Status = CreateRasNonStandardErrorRecord (
    EFI_GENERIC_ERROR_CORRECTED,
    RasErrorTypeDdrtEcc,
    &Record
  );
  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit, Status: %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Fill type specific fields. Start with 0 in ValidFields, and set the
  // correpsonding bit to 1 for each field we fill.
  //
  ValidFields.Data = 0;

  Record.Section.DdrtEcc.Socket = Socket;
  ValidFields.Bits.Socket = 1;

  Record.Section.DdrtEcc.ChOnSkt = ChOnSkt;
  ValidFields.Bits.ChOnSkt = 1;

  Record.Section.DdrtEcc.Rank = Rank;
  ValidFields.Bits.Rank = 1;

  Record.Section.DdrtEcc.Nibble = Nibble;
  ValidFields.Bits.Nibble = 1;

  //
  // Commit ValidFields to the section common structure.
  //
  Record.Section.Common.ValidFields = ValidFields.Data;

  Status = REPORT_STATUS_CODE_WITH_EXTENDED_DATA (
    EFI_ERROR_CODE,
    SMM_RAS_ENHANCED_ERROR_LOG,
    (VOID *)&Record,
    Record.Header.RecordLength
  );

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit, Status: %r\n", __FUNCTION__, Status));
  return Status;
}


/**
  Report a DDRT alert error.

  @param[in]  Socket              The socket number of the error(s).
  @param[in]  Channel             The channel number on the socket.
  @param[in]  Dimm                The DIMM number on the channel.
  @param[in]  DdrtErrorType       The type of error that occurred.
                                  Refer to RAS_DDRT_ALERT_ERROR_TYPE.
  @param[in]  DdrtAlertErrorLog   DDRT Alert Error Log.

  @retval EFI_SUCCESS             The data was logged successfully.
  @retval EFI_INVALID_PARAMETER   DdrtErrorType was an unknown value.
  @retval EFI_OUT_OF_RESOURCES    There were not enough resources to log.
  @retval EFI_UNSUPPORTED         Report status code is not supported.
**/
EFI_STATUS
EFIAPI
ReportDdrtAlertErrorLog (
  IN  UINT32                       Socket,
  IN  UINT32                       Channel,
  IN  UINT32                       Dimm,
  IN  UINT32                       DdrtAlertErrorType,
  IN  RAS_DDRT_ALERT_ERROR_LOG    *DdrtAlertErrorLog      OPTIONAL
  )
{
  EFI_STATUS                                  Status;
  RAS_NON_STANDARD_ERROR_RECORD               Record;
  RAS_DDRT_ALERT_ERROR_SECTION_VALID_FIELDS   ValidFields;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Entry\n", __FUNCTION__));

  if (DdrtAlertErrorType >= DdrtAlertErrorTypeMax) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "ERROR: DdrtAlertErrorType is unknown (%d)\n", DdrtAlertErrorType));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Fill common fields.
  //
  Status = CreateRasNonStandardErrorRecord (
    EFI_GENERIC_ERROR_RECOVERABLE,
    RasErrorTypeDdrtAlert,
    &Record
    );
  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit, Status: %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Fill type specific fields. Start with 0 in ValidFields, and set the
  // correpsonding bit to 1 for each field we fill.
  //
  ValidFields.Data = 0;

  Record.Section.DdrtAlert.Socket = Socket;
  ValidFields.Bits.Socket = 1;

  Record.Section.DdrtAlert.Channel = Channel;
  ValidFields.Bits.Channel = 1;

  Record.Section.DdrtAlert.Dimm = Dimm;
  ValidFields.Bits.Dimm = 1;

  Record.Section.DdrtAlert.DdrtAlertErrorType = DdrtAlertErrorType;
  ValidFields.Bits.DdrtAlertErrorType = 1;

  if (DdrtAlertErrorLog != NULL) {
    CopyMem (&Record.Section.DdrtAlert.DdrtAlertErrorLog, DdrtAlertErrorLog, sizeof (RAS_DDRT_ALERT_ERROR_LOG));
    ValidFields.Bits.DdrtAlertErrorLog = 1;
  }

  //
  // Commit ValidFields to the section common structure.
  //
  Record.Section.Common.ValidFields = ValidFields.Data;

  Status = REPORT_STATUS_CODE_WITH_EXTENDED_DATA (
    EFI_ERROR_CODE,
    SMM_RAS_ENHANCED_ERROR_LOG,
    (VOID *) &Record,
    Record.Header.RecordLength
    );

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit, Status: %r\n", __FUNCTION__, Status));
  return Status;
}

/**
  Report a spare core event.

  Report an event for the opportunistic spare core feature. Only one core is
  reported per log entry, so if multiple cores failed, this function should
  be invoked multiple times.

  @param[in]  Socket              The socket number of the error(s).
  @param[in]  FailedCore          The failed core index on the socket.

  @retval EFI_SUCCESS             The data was logged successfully.
  @retval EFI_OUT_OF_RESOURCES    There were not enough resources to log.
  @retval EFI_UNSUPPORTED         Report status code is not supported.
**/
EFI_STATUS
EFIAPI
ReportSpareCoreErrorLog (
  IN  UINT32    Socket,
  IN  UINT32    FailedCore
  )
{
  EFI_STATUS                                  Status;
  RAS_NON_STANDARD_ERROR_RECORD               Record;
  RAS_SPARE_CORE_ERROR_SECTION_VALID_FIELDS   ValidFields;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Entry\n", __FUNCTION__));

  //
  // Fill common fields.
  //
  Status = CreateRasNonStandardErrorRecord (
    EFI_GENERIC_ERROR_RECOVERABLE,
    RasErrorTypeSpareCore,
    &Record
    );
  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit, Status: %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Fill type specific fields. Start with 0 in ValidFields, and set the
  // correpsonding bit to 1 for each field we fill.
  //
  ValidFields.Data = 0;

  Record.Section.SpareCore.Socket = Socket;
  ValidFields.Bits.Socket = 1;

  Record.Section.SpareCore.FailedCore = FailedCore;
  ValidFields.Bits.FailedCore = 1;

  //
  // Commit ValidFields to the section common structure.
  //
  Record.Section.Common.ValidFields = ValidFields.Data;

  Status = REPORT_STATUS_CODE_WITH_EXTENDED_DATA (
    EFI_ERROR_CODE,
    SMM_RAS_ENHANCED_ERROR_LOG,
    (VOID *) &Record,
    Record.Header.RecordLength
    );

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit, Status: %r\n", __FUNCTION__, Status));
  return Status;
}

/**
  Report the UBOX order of errors.

  Report the first and second ports where an MCERR or IERR occurred on a given
  socket.

  If an optional input parameter is INVALID_U32 on entry, the corresponding
  field is marked invalid in the log data.

  @param[in]  Socket              The socket number of the error(s).
  @param[in]  FirstMcerrPort      The source port of the first MCERR.
  @param[in]  SecondMcerrPort     The source port of the second MCERR.
  @param[in]  FirstIerrPort       The source port of the first IERR.
  @param[in]  SecondIerrPort      The source port of the second IERR.

  @retval EFI_SUCCESS             The data was logged successfully.
  @retval EFI_INVALID_PARAMETER   SecondMcerrPort or SecondIerrPort was
                                  provided but FirstMcerrPort or FirstIerrPort
                                  (respectively) was not.
  @retval EFI_INVALID_PARAMETER   Neither FirstMcerrPort nor FirstIerrPort
                                  was provided.
  @retval EFI_OUT_OF_RESOURCES    There were not enough resources to log.
  @retval EFI_UNSUPPORTED         Report status code is not supported.
**/
EFI_STATUS
EFIAPI
ReportUboxOrderErrorLog (
  IN  UINT32    Socket,
  IN  UINT32    FirstMcerrPort,   OPTIONAL
  IN  UINT32    SecondMcerrPort,  OPTIONAL
  IN  UINT32    FirstIerrPort,    OPTIONAL
  IN  UINT32    SecondIerrPort    OPTIONAL
  )
{
  EFI_STATUS                                  Status;
  RAS_NON_STANDARD_ERROR_RECORD               Record;
  RAS_UBOX_ORDER_ERROR_SECTION_VALID_FIELDS   ValidFields;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Entry\n", __FUNCTION__));

  //
  // Make sure the second MCERR port is not provided if the first is not.
  //
  if (FirstMcerrPort == INVALID_U32) {
    if (SecondMcerrPort != INVALID_U32) {
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "ERROR: SecondMcerrPort is provided but FirstMcerrPort is not\n"));
      return EFI_INVALID_PARAMETER;
    }
  }

  //
  // Make sure the second IERR port is not provided if the first is not.
  //
  if (FirstIerrPort == INVALID_U32) {
    if (SecondIerrPort != INVALID_U32) {
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "ERROR: SecondIerrPort is provided but FirstIerrPort is not\n"));
      return EFI_INVALID_PARAMETER;
    }
  }

  //
  // Make sure at least the first MCERR or first IERR is provided.
  //
  if (FirstMcerrPort == INVALID_U32 && FirstIerrPort == INVALID_U32) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "ERROR: Neither FirstMcerrPort nor FirstIerrPort is provided\n"));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Fill common fields.
  //
  Status = CreateRasNonStandardErrorRecord (
    EFI_GENERIC_ERROR_RECOVERABLE,
    RasErrorTypeUboxOrder,
    &Record
    );
  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit, Status: %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Fill type specific fields. Start with 0 in ValidFields, and set the
  // correpsonding bit to 1 for each field we fill.
  //
  ValidFields.Data = 0;

  Record.Section.UboxOrder.Socket = Socket;
  ValidFields.Bits.Socket = 1;

  if (FirstMcerrPort != INVALID_U32) {
    Record.Section.UboxOrder.FirstMcerrPort = FirstMcerrPort;
    ValidFields.Bits.FirstMcerrPort = 1;
  }

  if (SecondMcerrPort != INVALID_U32) {
    Record.Section.UboxOrder.SecondMcerrPort = SecondMcerrPort;
    ValidFields.Bits.SecondMcerrPort = 1;
  }

  if (FirstIerrPort != INVALID_U32) {
    Record.Section.UboxOrder.FirstIerrPort = FirstIerrPort;
    ValidFields.Bits.FirstIerrPort = 1;
  }

  if (SecondIerrPort != INVALID_U32) {
    Record.Section.UboxOrder.SecondIerrPort = SecondIerrPort;
    ValidFields.Bits.SecondIerrPort = 1;
  }

  //
  // Commit ValidFields to the section common structure.
  //
  Record.Section.Common.ValidFields = ValidFields.Data;

  Status = REPORT_STATUS_CODE_WITH_EXTENDED_DATA (
    EFI_ERROR_CODE,
    SMM_RAS_ENHANCED_ERROR_LOG,
    (VOID *) &Record,
    Record.Header.RecordLength
    );

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit, Status: %r\n", __FUNCTION__, Status));
  return Status;
}

/**
  Report a leaky bucket event.

  Report re-equalization and/or link degradation events triggered by the PCIe
  leaky bucket feature.

  @param[in]  Segment             The segment number of the PCIe root port.
  @param[in]  Bus                 The bus number of the PCIe root port.
  @param[in]  Device              The device number of the PCIe root port.
  @param[in]  Function            The function number of the PCIe root port.
  @param[in]  LaneErrorMask       The bitmask of lanes that were triggered.
  @param[in]  LaneErrorCount      An array of error counts for each lane.
  @param[in]  Gen34ReEqualize     TRUE if re-equalization was requested.
  @param[in]  Gen2LinkDegrade     TRUE if Gen2 link degradation occurred.
  @param[in]  Gen3LinkDegrade     TRUE if Gen3 link degradation occurred.
  @param[in]  Gen4LinkDegrade     TRUE if Gen4 link degradation occurred.

  @retval EFI_SUCCESS             The data was logged successfully.
  @retval EFI_OUT_OF_RESOURCES    There were not enough resources to log.
  @retval EFI_UNSUPPORTED         Report status code is not supported.
**/
EFI_STATUS
EFIAPI
ReportLeakyBucketErrorLog (
  IN  UINT8     Segment,
  IN  UINT8     Bus,
  IN  UINT8     Device,
  IN  UINT8     Function,
  IN  UINT16    LaneErrorMask,
  IN  UINT8     LaneErrorCount[16],
  IN  BOOLEAN   Gen34ReEqualize,
  IN  BOOLEAN   Gen2LinkDegrade,
  IN  BOOLEAN   Gen3LinkDegrade,
  IN  BOOLEAN   Gen4LinkDegrade
  )
{
  EFI_STATUS                                  Status;
  RAS_NON_STANDARD_ERROR_RECORD               Record;
  RAS_LEAKY_BUCKET_ERROR_SECTION_VALID_FIELDS ValidFields;
  RAS_LEAKY_BUCKET_ACTION_STATUS              ActionStatus;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Entry\n", __FUNCTION__));

  //
  // Fill common fields.
  //
  Status = CreateRasNonStandardErrorRecord (
    EFI_GENERIC_ERROR_CORRECTED,
    RasErrorTypeLeakyBucket,
    &Record
    );
  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit, Status: %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Create the action status dword from individual status bits.
  //
  ActionStatus.Data = 0;
  if (Gen34ReEqualize) {
    ActionStatus.Bits.Gen34ReEqualize = 1;
  }
  if (Gen2LinkDegrade) {
    ActionStatus.Bits.Gen2LinkDegrade = 1;
  }
  if (Gen3LinkDegrade) {
    ActionStatus.Bits.Gen3LinkDegrade = 1;
  }
  if (Gen4LinkDegrade) {
    ActionStatus.Bits.Gen4LinkDegrade = 1;
  }

  //
  // Fill type specific fields. Start with 0 in ValidFields, and set the
  // correpsonding bit to 1 for each field we fill.
  //
  ValidFields.Data = 0;

  Record.Section.LeakyBucket.Segment = Segment;
  ValidFields.Bits.Segment = 1;

  Record.Section.LeakyBucket.Bus = Bus;
  ValidFields.Bits.Bus = 1;

  Record.Section.LeakyBucket.Device = Device;
  ValidFields.Bits.Device = 1;

  Record.Section.LeakyBucket.Function = Function;
  ValidFields.Bits.Function = 1;

  Record.Section.LeakyBucket.LaneErrorMask = LaneErrorMask;
  ValidFields.Bits.LaneErrorMask = 1;

  CopyMem (
    Record.Section.LeakyBucket.LaneErrorCount,
    LaneErrorCount,
    sizeof (UINT8) * 16
    );
  ValidFields.Bits.LaneErrorCount = 1;

  Record.Section.LeakyBucket.ActionStatus = ActionStatus.Data;
  ValidFields.Bits.ActionStatus = 1;

  //
  // Commit ValidFields to the section common structure.
  //
  Record.Section.Common.ValidFields = ValidFields.Data;

  Status = REPORT_STATUS_CODE_WITH_EXTENDED_DATA (
    EFI_ERROR_CODE,
    SMM_RAS_ENHANCED_ERROR_LOG,
    (VOID *) &Record,
    Record.Header.RecordLength
    );

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit, Status: %r\n", __FUNCTION__, Status));
  return Status;
}

/**
  Report a CXL error.

  @param[in]  Socket              The socket number of the error(s).
  @param[in]  Address             Physical address.


  @retval EFI_SUCCESS             The data was logged successfully.
  @retval EFI_INVALID_PARAMETER   FailedLanes was an unknown value.
  @retval EFI_OUT_OF_RESOURCES    There were not enough resources to log.
  @retval EFI_UNSUPPORTED         Report status code is not supported.
**/
EFI_STATUS
EFIAPI
ReportCxlErrorLog (
  IN  UINT32    Socket,
  IN  UINTN     Address
  )
{
  EFI_STATUS                                  Status;
  RAS_NON_STANDARD_ERROR_RECORD               Record;
  RAS_CXL_ERROR_SECTION_VALID_FIELDS          ValidFields;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Entry\n", __FUNCTION__));

  //
  // Fill common fields.
  //
  Status = CreateRasNonStandardErrorRecord (
    EFI_GENERIC_ERROR_CORRECTED,
    RasErrorTypeCxlError,
    &Record
    );
  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit, Status: %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Fill type specific fields. Start with 0 in ValidFields, and set the
  // correpsonding bit to 1 for each field we fill.
  //
  ValidFields.Data = 0;

  Record.Section.CxlError.Socket = Socket;
  ValidFields.Bits.Socket = 1;

  Record.Section.CxlError.Address = Address;
  ValidFields.Bits.Address = 1;

  //
  // Commit ValidFields to the section common structure.
  //
  Record.Section.Common.ValidFields = ValidFields.Data;

  Status = REPORT_STATUS_CODE_WITH_EXTENDED_DATA (
    EFI_ERROR_CODE,
    SMM_RAS_ENHANCED_ERROR_LOG,
    (VOID *) &Record,
    Record.Header.RecordLength
    );

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "%a Exit, Status: %r\n", __FUNCTION__, Status));
  return Status;
}