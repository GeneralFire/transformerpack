//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2016, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

//**********************************************************************
// $Header:  $
//
// $Revision:  $
//
// $Date:  $
//**********************************************************************
// Revision History
// ----------------
// $Log:  $
// 
// 
// 
//**********************************************************************
//<AMI_FHDR_START>
//
// Name:  RtErrorLogGpnv.c
//
// Description: Runtime error log GPNV library function definitions
//
//<AMI_FHDR_END>
//**********************************************************************

#include "RtErrorLogGpnv.h"


EFI_SM_ELOG_PROTOCOL      *gGenericElogProtocol     = NULL;


//<AMI_PHDR_START>
//-------------------------------------------------------------------------
// Name:   EfiSmSetEventLogData
//
// Description:
//  This function sends event log data to the destination such as LAN, ICMB,
// BMC etc.  
//
// Input:
//  IN  UINT8                             *ElogData,
//  IN  EFI_SM_ELOG_TYPE                  DataType,
//  IN  BOOLEAN                           AlertEvent,
//  IN  UINTN                             DataSize,
//  OUT UINT64                            *RecordId
//
// Output:
//     EFI_STATUS
//
//--------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS
EfiSmSetEventLogData (
  IN  UINT8                             *ElogData,
  IN  EFI_SM_ELOG_TYPE                  DataType,
  IN  BOOLEAN                           AlertEvent,
  IN  UINTN                             DataSize,
  OUT UINT64                            *RecordId  )
{
    EFI_STATUS              Status;

    if (gGenericElogProtocol == NULL) {

        Status = gSmst->SmmLocateProtocol (
                &gEfiGenericElogProtocolGuid,
                NULL,
                &gGenericElogProtocol);
        if ((!EFI_ERROR(Status)) && (gGenericElogProtocol != NULL)) {
            return gGenericElogProtocol->SetEventLogData (
                    gGenericElogProtocol,
                    ElogData,
                    DataType,
                    AlertEvent,
                    DataSize,
                    RecordId );
        }
    }else {  
        return gGenericElogProtocol->SetEventLogData (
                gGenericElogProtocol,
                ElogData,
                DataType,
                AlertEvent,
                DataSize,
                RecordId );
    }

    return Status;

}

//<AMI_PHDR_START>
//-------------------------------------------------------------------------
// Name:   LogMemErrorToGpnv
//
// Description:
//  This function log memory errors to GPNV.  
//
// Input:
//  IN AMI_MEMORY_ERROR_SECTION     *MemInfo
//
// Output:
//     EFI_STATUS
//
//--------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS
LogMemErrorToGpnv(
  EFI_COMMON_ERROR_RECORD_HEADER    *ErrorRecordHeader,
  BOOLEAN                           CurrentBoot
  )
{
    EFI_STATUS                      Status = EFI_SUCCESS;
    UINT64                          RecordId;
    EFI_GPNV_RUNTIME_ERROR          ToWrite;
    EFI_ERROR_SECTION_DESCRIPTOR    *ErrorRecordDescription;
    
    ErrorRecordDescription = (EFI_ERROR_SECTION_DESCRIPTOR *)((UINT8 *)ErrorRecordHeader + sizeof(EFI_COMMON_ERROR_RECORD_HEADER));

    ToWrite.RecordType = EFI_EVENT_LOG_TYPE_OEM_GPNV_RUNTIME;
    ToWrite.EventLogType = EFI_EVENT_LOG_TYPE_SINGLE_BIT_ECC_MEMORY_ERROR;
    ToWrite.EventLogType = (ErrorRecordDescription->Severity == EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTED ?
                        EFI_EVENT_LOG_TYPE_SINGLE_BIT_ECC_MEMORY_ERROR :
                        EFI_EVENT_LOG_TYPE_MULTI_BIT_ECC_MEMORY_ERROR);

    Status = EfiSmSetEventLogData (
                (UINT8*)&ToWrite,
                EfiElogSmSMBIOS,
                FALSE,
                sizeof (EFI_GPNV_RUNTIME_ERROR),
                &RecordId );

    return Status;
}

//<AMI_PHDR_START>
//-------------------------------------------------------------------------
// Name:   LogPciErrorToGpnv
//
// Description:
//  This function log PCI errors to GPNV.  
//
// Input:
//  IN AMI_PCIE_ERROR_SECTION    *PcieInfo
//
// Output:
//     EFI_STATUS
//
//--------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS
LogPciErrorToGpnv (
  EFI_COMMON_ERROR_RECORD_HEADER    *ErrorRecordHeader,
  BOOLEAN                           CurrentBoot
  )
{
    EFI_STATUS                      Status = EFI_SUCCESS;
    UINT64                          RecordId;
    EFI_GPNV_RUNTIME_ERROR          ToWrite;
    EFI_ERROR_SECTION_DESCRIPTOR    *ErrorRecordDescription;
    PCIE_ERROR_SECTION              *PcieErrorRecord;
    UINT8                           ErrorType;
    
    ErrorRecordDescription = (EFI_ERROR_SECTION_DESCRIPTOR *)((UINT8 *)ErrorRecordHeader + sizeof(EFI_COMMON_ERROR_RECORD_HEADER));
    PcieErrorRecord = (PCIE_ERROR_SECTION *)((UINT8 *)ErrorRecordDescription + sizeof(EFI_ERROR_SECTION_DESCRIPTOR));
    
    if (ErrorRecordDescription->Severity == EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTED) {
        ErrorType = CRITICAL_INTERRUPT_BUS_CORERR;
    }
    else if (ErrorRecordDescription->Severity == EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTABLE) {
        ErrorType = CRITICAL_INTERRUPT_BUS_UNCERR;
    }
    else if (ErrorRecordDescription->Severity == EFI_ACPI_5_0_ERROR_SEVERITY_FATAL) {
        if (PcieErrorRecord->Section.PortType == PCIE_DEVICE_PORT_TYPE_PCIE_TO_PCI_BRIDGE) {
            if (PcieErrorRecord->Section.BridgeControlStatus & (PCI_STS_MSTR_DATA_PERR | PCI_STS_SIG_SERR | PCI_STS_DET_PERR | PCI_STS_REC_TRGT_ABRT)) {
                if (PcieErrorRecord->Section.BridgeControlStatus & PCI_STS_SIG_SERR) {
                    ErrorType = CRITICAL_INTERRUPT_PCI_SERR;
                }
                else {
                    ErrorType = CRITICAL_INTERRUPT_PCI_PERR;
                }
            }
            else {
                ErrorType = CRITICAL_INTERRUPT_BUS_FATERR;
            }
        }
        else {
            ErrorType = CRITICAL_INTERRUPT_BUS_FATERR;
        }
    }
    
    ToWrite.RecordType = EFI_EVENT_LOG_TYPE_OEM_GPNV_RUNTIME;
    ToWrite.EventLogType = ErrorType;

    Status = EfiSmSetEventLogData (
                (UINT8*)&ToWrite,
                EfiElogSmSMBIOS,
                FALSE,
                sizeof (EFI_GPNV_RUNTIME_ERROR),
                &RecordId );

    return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//-------------------------------------------------------------------------
// Name:   LogProcessorErrorToGpnv
//
// Description:
//  This function log memory errors to GPNV.  
//
// Input:
//  IN AMI_MEMORY_ERROR_SECTION     *MemInfo
//
// Output:
//     EFI_STATUS
//
//--------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS
LogProcessorErrorToGpnv(
  EFI_COMMON_ERROR_RECORD_HEADER    *ErrorRecordHeader,
  BOOLEAN                           CurrentBoot
  )
{
    EFI_STATUS                          Status = EFI_SUCCESS;
    UINT64                              RecordId;
    EFI_GPNV_RUNTIME_ERROR              ToWrite;
    EFI_ERROR_SECTION_DESCRIPTOR        *ErrorRecordDescription;
    EFI_PROCESSOR_GENERIC_ERROR_DATA    *ProcessorErrorRecord;
    
    ErrorRecordDescription = (EFI_ERROR_SECTION_DESCRIPTOR *)((UINT8 *)ErrorRecordHeader + sizeof(EFI_COMMON_ERROR_RECORD_HEADER));
    ProcessorErrorRecord = (EFI_PROCESSOR_GENERIC_ERROR_DATA *)((UINT8 *)ErrorRecordDescription + sizeof(EFI_ERROR_SECTION_DESCRIPTOR));

    ToWrite.RecordType = EFI_EVENT_LOG_TYPE_OEM_GPNV_RUNTIME;
    ToWrite.EventLogType = ProcessorErrorRecord->ErrorType;

    Status = EfiSmSetEventLogData (
                (UINT8*)&ToWrite,
                EfiElogSmSMBIOS,
                FALSE,
                sizeof (EFI_GPNV_RUNTIME_ERROR),
                &RecordId );

    return Status;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2016, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
