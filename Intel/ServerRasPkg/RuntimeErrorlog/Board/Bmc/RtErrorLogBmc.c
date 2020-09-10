//**********************************************************************//
//**********************************************************************//
//**                                                                  **//
//**        (C)Copyright 1985-2020, American Megatrends, Inc.         **//
//**                                                                  **//
//**                       All Rights Reserved.                       **//
//**                                                                  **//
//**       5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093       **//
//**                                                                  **//
//**                       Phone: (770)-246-8600                      **//
//**                                                                  **//
//**********************************************************************//
//**********************************************************************//
//
// $Header: $
//
// $Revision: $
//
// $Date: $
//
//
//*****************************************************************************
// Revision History
// ----------------
// $Log: $
// 
//<AMI_FHDR_START>
//----------------------------------------------------------------------------
//
// Name:		RtErrorLogBmc.c
//
// Description:	Runtime error log BMC library function definitions
//
//----------------------------------------------------------------------------
//<AMI_FHDR_END>

#include "RtErrorLogBmc.h"

EFI_SEL_RECORD_DATA           mSelRecord;


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   UpdateGenericSelInfo
//
// Description: 
// This function will update generic IPMI info fields 
//
// Input:
//
//    SelRecord           - Pointer to SEL Record
// Output:
//
// Returns:     
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
UpdateGenericSelInfo (
  IN OUT  EFI_SEL_RECORD_DATA     *SelRecord
  )
{

  SelRecord->TimeStamp.Year = 0;
  SelRecord->RecordType = SEL_SYSTEM_RECORD;
  SelRecord->GeneratorId = (UINT16)EFI_GENERATOR_ID(SMI_HANDLER_SOFTWARE_ID);
  SelRecord->EvMRevision = EFI_EVM_REVISION;
  SelRecord->EventDirType = EFI_SENSOR_TYPE_EVENT_CODE_DISCRETE;
  return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   UpdateSelInfoForMe
//
// Description: 
// Update the SEL Record information for Memory Errors
//
// Input:
//
//    MemorySensorOffset  - Memory offset to enum EFI_SEL_SENSOR_MEMORY_OFFSET
//    Channel             - Channel, base 0
//    DimmNumber          - Dimm Number, base 0
//    SelRecord           - Pointer to SEL Record
// Output:
//
// Returns:     
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
UpdateSelInfoForMe (
  IN      EFI_SEL_SENSOR_MEMORY_OFFSET      MemorySensorOffset,
  IN      MEM_ERROR_LOCATION                *MemErrorLoc,
  IN OUT  EFI_SEL_RECORD_DATA               *SelRecord
  )
{
  // Update Generic SEL related information

  UpdateGenericSelInfo (SelRecord);

  SelRecord->SensorType = EFI_MEMORY_SENSOR_TYPE;
  SelRecord->SensorNumber = (UINT8)0x8; // for UCE/CE

  if (MemorySensorOffset == EFI_MEMORY_SENSOR_OFFSET_ECC_UNCORRECTABLE) {
    // UCE error
    SelRecord->OEMEvData1 = 0xA0 | MEMORY_SENSOR_UCE_OFFSET; // Add error Id to identify the error
  } else if (MemorySensorOffset == EFI_MEMORY_SENSOR_OFFSET_ECC_CORRECTABLE){
    // Single bit error
    SelRecord->OEMEvData1 = 0xA0 | MEMORY_SENSOR_CE_OFFSET; // Add error Id to identify the error
  } else if (MemorySensorOffset == EFI_MEMORY_SENSOR_OFFSET_ECC_CORRECTABLE_MAX){
	// Single bit error threshold reached
    SelRecord->OEMEvData1 = 0xA0 | MEMORY_SENSOR_CEMAX_OFFSET; // Add error Id to identify the error
  }

  if (MemErrorLoc->Dimm == 0xff) {
      // No info about the error is available
      SelRecord->OEMEvData3 = (UINT8)((MemErrorLoc->Socket << SOCKET_NUM_BITS) |(MemErrorLoc->DdrChannel << CHANNEL_NUM_BITS) | DIMM_NUM_BITS);
  } else {
      SelRecord->OEMEvData3 = (UINT8)((MemErrorLoc->Socket << SOCKET_NUM_BITS) |(MemErrorLoc->DdrChannel << CHANNEL_NUM_BITS) | MemErrorLoc->Dimm);
  }

  return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   AllocSelRecord
//
// Description: 
//     Allocate memory for SelRecord
//
// Input:
//
// Output:
//
// Returns:     
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID
AllocSelRecord(
  IN OUT EFI_SEL_RECORD_DATA **SelRecord
  )
{
  *SelRecord = &mSelRecord;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   LogInfoToSel
//
// Description: 
//
//	This function will log the data into SEL using Generic Log API

// Input:
//
// Output:
//
// Returns:     
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
LogInfoToSel (
  IN  VOID              *SelRecord
  )
{
  EFI_STATUS      				Status;
  EFI_IPMI_TRANSPORT			*IpmiTransport;
  UINT8      		  		    ResponseData[10];
  UINT8        	  				ResponseDataSize;
#if IPMI_COMPLETION_CODE_SUPPORT
  UINT8                         CompletionCode;
#endif
 ResponseDataSize = 10;

 Status = gSmst->SmmLocateProtocol (&gEfiSmmIpmiTransportProtocolGuid, NULL, &IpmiTransport);
  if (Status == EFI_SUCCESS) {
    Status = IpmiTransport-> SendIpmiCommand (IpmiTransport,
						EFI_SM_STORAGE,
						0,
						ADD_SEL_ENTRY,
						(UINT8 *) SelRecord,
						sizeof (EFI_SEL_RECORD_DATA),
						(UINT8 *) &ResponseData,
#if IPMI_COMPLETION_CODE_SUPPORT
						(UINT8 *) &ResponseDataSize,
						(UINT8 *) &CompletionCode
#else
						(UINT8 *) &ResponseDataSize
#endif
						); 
  }

  Status = EFI_SUCCESS;
  return Status;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   LogMemErrorToSel
//
// Description: 
//	Log the memory errors to BMC SEL
//
// Input:
//
// Output:
//
// Returns:     
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
LogMemErrorToSel (
  EFI_COMMON_ERROR_RECORD_HEADER    *ErrorRecordHeader,
  BOOLEAN                           CurrentBoot
  )
{
    EFI_STATUS                      Status = EFI_SUCCESS;
    EFI_SEL_RECORD_DATA             *SelRecord = NULL;
    EFI_SEL_SENSOR_MEMORY_OFFSET    MemorySensorOffset = EFI_MEMORY_SENSOR_OFFSET_ECC_CORRECTABLE_MAX;
    MEM_ERROR_LOCATION              MemErrorLoc;
    EFI_ERROR_SECTION_DESCRIPTOR    *ErrorRecordDescription;
    EFI_PLATFORM_MEMORY_ERROR_DATA  *MemoryErrorRecord;
    
    ErrorRecordDescription = (EFI_ERROR_SECTION_DESCRIPTOR *)((UINT8 *)ErrorRecordHeader + sizeof(EFI_COMMON_ERROR_RECORD_HEADER));
    MemoryErrorRecord = (EFI_PLATFORM_MEMORY_ERROR_DATA *)((UINT8 *)ErrorRecordDescription + sizeof(EFI_ERROR_SECTION_DESCRIPTOR));
    
    if(((MemoryErrorRecord->ValidFields & EFI_PLATFORM_MEMORY2_NODE_VALID) &&
       (MemoryErrorRecord->ValidFields & EFI_PLATFORM_MEMORY2_CARD_VALID) &&
       (MemoryErrorRecord->ValidFields & EFI_PLATFORM_MEMORY2_MODULE_VALID)) == 0)
        return  Status;

    // Allocate memory for SelRecord
    AllocSelRecord(&SelRecord);

    // Set location
    MemErrorLoc.Socket = (UINT8)(MemoryErrorRecord->Node);
    MemErrorLoc.DdrChannel = (UINT8)(MemoryErrorRecord->Card);
    MemErrorLoc.Dimm = (UINT8)(MemoryErrorRecord->ModuleRank);

    // Set type
    MemorySensorOffset = (ErrorRecordDescription->Severity == EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTED ? 
        EFI_MEMORY_SENSOR_OFFSET_ECC_CORRECTABLE_MAX :
        EFI_MEMORY_SENSOR_OFFSET_ECC_UNCORRECTABLE);

    // Log sel entry to report
    Status = UpdateSelInfoForMe (MemorySensorOffset, &MemErrorLoc, SelRecord);
    
    // Changes done for the LASTBOOT ERROR Support
    if (!CurrentBoot) {
        SelRecord->OEMEvData2 |= BIT0; //ED2[0]: 0 - Current Boot. 1 - Last boot
    }
    
    Status = LogInfoToSel((VOID*)SelRecord);
    return  Status;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   LogProcErrorToSel
//
// Description: 
//  Log the Processor errors to BMC SEL
//
// Input:
//
// Output:
//
// Returns:     
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
LogProcErrorToSel (
  EFI_COMMON_ERROR_RECORD_HEADER    *ErrorRecordHeader,
  BOOLEAN                           CurrentBoot
  )
{
    EFI_STATUS                              Status = EFI_SUCCESS;
    EFI_SEL_RECORD_DATA                     SelRecord;
    //EFI_CPU_PHYSICAL_LOCATION       Loc;
    UINT8                                   Socket = 0;
    EFI_SEL_PROCESSOR_BANK_TYPE             BankType = 0;
    EFI_SEL_PROCESSOR_GENERIC_ERROR_TYPE    ProcessorErrorType = 0;
    EFI_ERROR_SECTION_DESCRIPTOR            *ErrorRecordDescription;
    EFI_PROCESSOR_GENERIC_ERROR_DATA        *ProcessorErrorRecord;
    MCA_BANK_INFO                           *ProcessorMcBankInfo;
    
    ErrorRecordDescription = (EFI_ERROR_SECTION_DESCRIPTOR *)((UINT8 *)ErrorRecordHeader + sizeof(EFI_COMMON_ERROR_RECORD_HEADER));
    ProcessorErrorRecord = (EFI_PROCESSOR_GENERIC_ERROR_DATA *)((UINT8 *)ErrorRecordDescription + sizeof(EFI_ERROR_SECTION_DESCRIPTOR));
    ProcessorMcBankInfo = (MCA_BANK_INFO *)((UINT8 *)ProcessorErrorRecord + sizeof(EFI_PROCESSOR_GENERIC_ERROR_DATA));
    
    // Initialize generic parts of SEL record.
    Status = UpdateGenericSelInfo(&SelRecord);

    // Initialize specific parts of SEL record.
    SelRecord.SensorType = EFI_PROCESSOR_SENSOR_TYPE; 
    SelRecord.SensorNumber = (UINT8)0x0; 

    SelRecord.OEMEvData1 = 0xA0 | EFI_PROCESSOR_SENSOR_OFFSET_CONFIGURATION_ERROR; // Add error Id to identify the error

    //ExtractProcessorLocation((UINT32)ProcessorErrorRecord->ApicId, &Loc);
    //Socket = Loc.Package;

    switch (ProcessorMcBankInfo->BankType) {
        case MCA_UNIT_TYPE_IFU:
            BankType =  MCA_UNIT_IFU;
            break;
        case MCA_UNIT_TYPE_DCU:
            BankType =  MCA_UNIT_DCU;
            break;
        case MCA_UNIT_TYPE_DTLB:
            BankType =  MCA_UNIT_DTLB;
            break;
        case MCA_UNIT_TYPE_MLC:
            BankType =  MCA_UNIT_MLC;
            break;
        case MCA_UNIT_TYPE_PCU:
            BankType =  MCA_UNIT_PCU;
            break;
        case MCA_UNIT_TYPE_IIO:
            BankType =  MCA_UNIT_IIO;
            break;
        case MCA_UNIT_TYPE_CHA:
            BankType =  MCA_UNIT_CHA;
            break;
        case MCA_UNIT_TYPE_KTI:
            BankType =  MCA_UNIT_KTI;
            break;
        default:
            BankType = MCA_UNIT_NONE;
    }
    
    switch (ProcessorErrorRecord->ErrorType) {
        case EFI_GENERIC_ERROR_PROC_ERROR_TYPE_UNKNOWN:
            ProcessorErrorType =  SEL_PROC_ERROR_TYPE_UNKNOWN;
            break;
        case EFI_GENERIC_ERROR_PROC_ERROR_TYPE_CACHE:
            ProcessorErrorType =  SEL_PROC_ERROR_TYPE_CACHE;
            break;
        case EFI_GENERIC_ERROR_PROC_ERROR_TYPE_TLB:
            ProcessorErrorType =  SEL_PROC_ERROR_TYPE_TLB;
            break;
        case EFI_GENERIC_ERROR_PROC_ERROR_TYPE_BUS:
            ProcessorErrorType =  SEL_PROC_ERROR_TYPE_BUS;
            break;
        case EFI_GENERIC_ERROR_PROC_ERROR_TYPE_MICRO_ARCH:
            ProcessorErrorType =  SEL_PROC_ERROR_TYPE_MICRO_ARCH;
            break;
        default:
            ProcessorErrorType = SEL_PROC_ERROR_TYPE_UNKNOWN;
    }
    
    SelRecord.OEMEvData2 = (Socket << 4) | BankType;
    
    if (CurrentBoot) {
        SelRecord.OEMEvData3 = (ProcessorErrorType << 4) | (ErrorRecordDescription->Severity << 1);
    }
    else {
        SelRecord.OEMEvData3 = (ProcessorErrorType << 4) | (ErrorRecordDescription->Severity << 1) | BIT0; // //ED3[0]: 0 - Current Boot. 1 - Last Boot
    }
    
    // Report the error.
    Status = LogInfoToSel( (VOID*)&SelRecord ); 
    
    return  Status;
}

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
// Procedure:   LogPciErrorToSel
//
// Description:
//  Takes in an error type and the address of a PCI device. Correctly fills
//  in a SEL record and logs to the SEL.
//
// Input:
//  IN  UINT8     ErrorType - Type of PCI error (SERR, PERR, etc.)
//  IN  UINT8     Bus
//  IN  UINT8     Dev
//  IN  UINT8     Func
//
// Output: 
//  EFI_STATUS
//      EFI_SUCCESS - Error successfully logged.
//      Others - A problem occurred while trying to log error.
//
// Modified:
//  Nothing
//
// Referrals:
//  UpdateGenericSelInfo()
//  LogInfoToSel()
//
// Notes:
//  None
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
LogPciErrorToSel (
  EFI_COMMON_ERROR_RECORD_HEADER    *ErrorRecordHeader,
  BOOLEAN                           CurrentBoot
  )
{
    EFI_SEL_RECORD_DATA             SelRecord;
    EFI_STATUS                      Status;
    EFI_ERROR_SECTION_DESCRIPTOR    *ErrorRecordDescription;
    PCIE_ERROR_SECTION              *PcieErrorRecord;
    
    ErrorRecordDescription = (EFI_ERROR_SECTION_DESCRIPTOR *)((UINT8 *)ErrorRecordHeader + sizeof(EFI_COMMON_ERROR_RECORD_HEADER));
    PcieErrorRecord = (PCIE_ERROR_SECTION *)((UINT8 *)ErrorRecordDescription + sizeof(EFI_ERROR_SECTION_DESCRIPTOR));
    
    // Initialize generic parts of SEL record.
    Status = UpdateGenericSelInfo(&SelRecord);

    // Initialize specific parts of SEL record.
    SelRecord.SensorType = SEL_SENS_TYPE_CRIT_ERR;
    SelRecord.SensorNumber = 0x00;
    
    if (!CurrentBoot) {
        SelRecord.OEMEvData1 = 0xA0 | LASTBOOT_PCIE_ERROR;    // OEM Data for byte 2, 3 + Last Boot PCIe Error
    }
    else {
      if (ErrorRecordDescription->Severity == EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTED) {
          SelRecord.OEMEvData1 = 0xA0 | CRITICAL_INTERRUPT_BUS_CORERR;
      }
      else if (ErrorRecordDescription->Severity == EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTABLE) {
          SelRecord.OEMEvData1 = 0xA0 | CRITICAL_INTERRUPT_BUS_UNCERR;
      }
      else if (ErrorRecordDescription->Severity == EFI_ACPI_5_0_ERROR_SEVERITY_FATAL) {
          if (PcieErrorRecord->Section.PortType == PCIE_DEVICE_PORT_TYPE_PCIE_TO_PCI_BRIDGE) {
              if (PcieErrorRecord->Section.BridgeControlStatus & (PCI_STS_MSTR_DATA_PERR | PCI_STS_SIG_SERR | PCI_STS_DET_PERR | PCI_STS_REC_TRGT_ABRT)) {
                  if (PcieErrorRecord->Section.BridgeControlStatus & PCI_STS_SIG_SERR) {
                      SelRecord.OEMEvData1 = 0xA0 | CRITICAL_INTERRUPT_PCI_SERR;
                  }
                  else {
                      SelRecord.OEMEvData1 = 0xA0 | CRITICAL_INTERRUPT_PCI_PERR;
                  }
              }
              else {
                  SelRecord.OEMEvData1 = 0xA0 | CRITICAL_INTERRUPT_BUS_FATERR;
              }
          }
          else {
              SelRecord.OEMEvData1 = 0xA0 | CRITICAL_INTERRUPT_BUS_FATERR;
          }
      }
    }
    SelRecord.OEMEvData2 = PcieErrorRecord->Section.DevBridge.PrimaryOrDeviceBus;                 // Fill with Bus.
    SelRecord.OEMEvData3 = (PcieErrorRecord->Section.DevBridge.Device << 3) | PcieErrorRecord->Section.DevBridge.Function;   // Fill with Dev and Func.

    // Report the error.
    Status = LogInfoToSel((VOID*)&SelRecord); 

    return Status;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   LogUpiErrorToSel
//
// Description: 
//  Log the UPI errors to BMC SEL
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
LogUpiErrorToSel (
  EFI_COMMON_ERROR_RECORD_HEADER    *ErrorRecordHeader,
  BOOLEAN                           CurrentBoot
  )
{
    EFI_STATUS                      Status = EFI_SUCCESS;
    EFI_SEL_RECORD_DATA             SelRecord;
    EFI_ERROR_SECTION_DESCRIPTOR    *ErrorRecordDescription;
    RAS_UPI_FAILOVER_ERROR_SECTION  *RasUpiFailoverErrorSection;
    
    ErrorRecordDescription = (EFI_ERROR_SECTION_DESCRIPTOR *)((UINT8 *)ErrorRecordHeader + sizeof(EFI_COMMON_ERROR_RECORD_HEADER));
    RasUpiFailoverErrorSection = (RAS_UPI_FAILOVER_ERROR_SECTION *)((UINT8 *)ErrorRecordDescription + sizeof(EFI_ERROR_SECTION_DESCRIPTOR));
    
    // Initialize generic parts of SEL record.
    Status = UpdateGenericSelInfo(&SelRecord);

    // Initialize specific parts of SEL record.
    SelRecord.SensorType = EFI_PROCESSOR_SENSOR_TYPE;
    SelRecord.SensorNumber = (UINT8)0x0;

    SelRecord.OEMEvData1 = 0xA0 | EFI_PROCESSOR_SENSOR_OFFSET_CONFIGURATION_ERROR;
    
    SelRecord.OEMEvData2 = (RasUpiFailoverErrorSection->Socket << 4) | MCA_UNIT_KTI;
    
    if (CurrentBoot) {
        SelRecord.OEMEvData3 = (RasUpiFailoverErrorSection->LaneIndex << 4) | (ErrorRecordDescription->Severity << 1);
    }
    else {
        SelRecord.OEMEvData3 = (RasUpiFailoverErrorSection->LaneIndex << 4) | (ErrorRecordDescription->Severity << 1) | BIT0; // //ED3[0]: 0 - Current Boot. 1 - Last Boot
    }
    
    // Report the error.
    Status = LogInfoToSel( (VOID*)&SelRecord );
    
    return  Status;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   LogIehErrorToSel
//
// Description: 
//  Log the IEH errors to BMC SEL
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
LogIehErrorToSel (
  EFI_COMMON_ERROR_RECORD_HEADER    *ErrorRecordHeader,
  BOOLEAN                           CurrentBoot
  )
{
    EFI_STATUS                      Status = EFI_SUCCESS;
    EFI_SEL_RECORD_DATA             SelRecord;
    EFI_ERROR_SECTION_DESCRIPTOR    *ErrorRecordDescription;
    RAS_IEH_ERROR_SECTION           *RasIehErrorSection;
    UINT8                           Bus;
    
    ErrorRecordDescription = (EFI_ERROR_SECTION_DESCRIPTOR *)((UINT8 *)ErrorRecordHeader + sizeof(EFI_COMMON_ERROR_RECORD_HEADER));
    RasIehErrorSection = (RAS_IEH_ERROR_SECTION *)((UINT8 *)ErrorRecordDescription + sizeof(EFI_ERROR_SECTION_DESCRIPTOR));
    
    // Initialize generic parts of SEL record.
    Status = UpdateGenericSelInfo(&SelRecord);

    // Initialize specific parts of SEL record.
    SelRecord.SensorType = SEL_SENS_TYPE_CRIT_ERR;
    SelRecord.SensorNumber = 0x00;
    
    if (!CurrentBoot) {
        SelRecord.OEMEvData1 = 0xA0 | LASTBOOT_PCIE_ERROR;    // OEM Data for byte 2, 3 + Last Boot PCIe Error
    }
    else {
        if (ErrorRecordDescription->Severity == FATAL_ERROR_TYPE) {
            SelRecord.OEMEvData1 = 0xA0 | CRITICAL_INTERRUPT_BUS_FATERR;
        }
        else if (ErrorRecordDescription->Severity == NONFATAL_ERROR_TYPE) {
            SelRecord.OEMEvData1 = 0xA0 | CRITICAL_INTERRUPT_BUS_UNCERR;
        }
        else if (ErrorRecordDescription->Severity == COR_ERROR_TYPE) {
            SelRecord.OEMEvData1 = 0xA0 | CRITICAL_INTERRUPT_BUS_CORERR;
        }
    }
    
    GetStackStartBus (RasIehErrorSection->Socket, RasIehErrorSection->IehIndex, &Bus);
    SelRecord.OEMEvData2 = Bus;                 // Fill with Bus.
    
    SelRecord.OEMEvData3 = (RT_LOCAL_IEH_DEV << 3) | RT_LOCAL_IEH_FUN;   // Fill with Dev and Func.
    
    // Report the error.
    Status = LogInfoToSel( (VOID*)&SelRecord );
    
    return  Status;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   LogIioXpErrorToSel
//
// Description: 
//  Log the IioXp errors to BMC SEL
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
LogIioXpErrorToSel (
  EFI_COMMON_ERROR_RECORD_HEADER    *ErrorRecordHeader,
  BOOLEAN                           CurrentBoot
  )
{
    EFI_STATUS                      Status = EFI_SUCCESS;
    EFI_SEL_RECORD_DATA             SelRecord;
    EFI_ERROR_SECTION_DESCRIPTOR    *ErrorRecordDescription;
    RAS_IIO_XP_ERROR_SECTION        *RasIioXpErrorSection;
     
    ErrorRecordDescription = (EFI_ERROR_SECTION_DESCRIPTOR *)((UINT8 *)ErrorRecordHeader + sizeof(EFI_COMMON_ERROR_RECORD_HEADER));
    RasIioXpErrorSection = (RAS_IIO_XP_ERROR_SECTION *)((UINT8 *)ErrorRecordDescription + sizeof(EFI_ERROR_SECTION_DESCRIPTOR));
     
    // Initialize generic parts of SEL record.
    Status = UpdateGenericSelInfo(&SelRecord);

    // Initialize specific parts of SEL record.
    SelRecord.SensorType = SEL_SENS_TYPE_CRIT_ERR;
    SelRecord.SensorNumber = (UINT8)0x0;

    if (!CurrentBoot) {
        SelRecord.OEMEvData1 = 0xA0 | LASTBOOT_PCIE_ERROR;    // OEM Data for byte 2, 3 + Last Boot PCIe Error
    }
    else {
        if (ErrorRecordDescription->Severity == FATAL_ERROR_TYPE) {
             SelRecord.OEMEvData1 = 0xA0 | CRITICAL_INTERRUPT_BUS_FATERR;
        }
        else if (ErrorRecordDescription->Severity == NONFATAL_ERROR_TYPE) {
            SelRecord.OEMEvData1 = 0xA0 | CRITICAL_INTERRUPT_BUS_UNCERR;
        }
        else if (ErrorRecordDescription->Severity == COR_ERROR_TYPE) {
            SelRecord.OEMEvData1 = 0xA0 | CRITICAL_INTERRUPT_BUS_CORERR;
        }
    }

    SelRecord.OEMEvData2 = RasIioXpErrorSection->Bus;
    
    SelRecord.OEMEvData3 = (RasIioXpErrorSection->Device << 3) | RasIioXpErrorSection->Function;
     
    // Report the error.
    Status = LogInfoToSel( (VOID*)&SelRecord );
     
    return  Status;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   LogLeakyBucketErrorToSel
//
// Description: 
//  Log the Leaky Bucket errors to BMC SEL
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
LogLeakyBucketErrorToSel (
  EFI_COMMON_ERROR_RECORD_HEADER    *ErrorRecordHeader,
  BOOLEAN                           CurrentBoot
  )
{
    EFI_STATUS                      Status = EFI_SUCCESS;
    EFI_SEL_RECORD_DATA             SelRecord;
    EFI_ERROR_SECTION_DESCRIPTOR    *ErrorRecordDescription;
    RAS_LEAKY_BUCKET_ERROR_SECTION        *RasLeakyBucketErrorSection;
     
    ErrorRecordDescription = (EFI_ERROR_SECTION_DESCRIPTOR *)((UINT8 *)ErrorRecordHeader + sizeof(EFI_COMMON_ERROR_RECORD_HEADER));
    RasLeakyBucketErrorSection = (RAS_LEAKY_BUCKET_ERROR_SECTION *)((UINT8 *)ErrorRecordDescription + sizeof(EFI_ERROR_SECTION_DESCRIPTOR));
     
    // Initialize generic parts of SEL record.
    Status = UpdateGenericSelInfo(&SelRecord);

    // Initialize specific parts of SEL record.
    SelRecord.SensorType = SEL_SENS_TYPE_CRIT_ERR;
    SelRecord.SensorNumber = (UINT8)0x0;

    if (!CurrentBoot) {
        SelRecord.OEMEvData1 = 0xA0 | LASTBOOT_PCIE_ERROR;    // OEM Data for byte 2, 3 + Last Boot PCIe Error
    }
    else {
        if (ErrorRecordDescription->Severity == FATAL_ERROR_TYPE) {
                    SelRecord.OEMEvData1 = 0xA0 | CRITICAL_INTERRUPT_BUS_FATERR;
               }
               else if (ErrorRecordDescription->Severity == NONFATAL_ERROR_TYPE) {
                   SelRecord.OEMEvData1 = 0xA0 | CRITICAL_INTERRUPT_BUS_UNCERR;
               }
               else if (ErrorRecordDescription->Severity == COR_ERROR_TYPE) {
                   SelRecord.OEMEvData1 = 0xA0 | CRITICAL_INTERRUPT_BUS_CORERR;
               }
            
    }
    
    SelRecord.OEMEvData2 = RasLeakyBucketErrorSection->Bus;
    
    SelRecord.OEMEvData3 = (RasLeakyBucketErrorSection->Device << 3) | RasLeakyBucketErrorSection->Function;
     
    // Report the error.
    Status = LogInfoToSel( (VOID*)&SelRecord );
     
    return  Status;
}



//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   LogSpareCoreErrorToSel
//
// Description: 
//  Log the Leaky Bucket errors to BMC SEL
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
LogSpareCoreErrorToSel (
  EFI_COMMON_ERROR_RECORD_HEADER    *ErrorRecordHeader,
  BOOLEAN                           CurrentBoot
  )
{
    EFI_STATUS                      Status = EFI_SUCCESS;
    EFI_SEL_RECORD_DATA             SelRecord;
    EFI_ERROR_SECTION_DESCRIPTOR    *ErrorRecordDescription;
    RAS_SPARE_CORE_ERROR_SECTION        *RasSpareCoreErrorSection;
     
    ErrorRecordDescription = (EFI_ERROR_SECTION_DESCRIPTOR *)((UINT8 *)ErrorRecordHeader + sizeof(EFI_COMMON_ERROR_RECORD_HEADER));
    RasSpareCoreErrorSection = (RAS_SPARE_CORE_ERROR_SECTION *)((UINT8 *)ErrorRecordDescription + sizeof(EFI_ERROR_SECTION_DESCRIPTOR));
     
    // Initialize generic parts of SEL record.
    Status = UpdateGenericSelInfo(&SelRecord);

    // Initialize specific parts of SEL record.
    SelRecord.SensorType = EFI_PROCESSOR_SENSOR_TYPE;;
    SelRecord.SensorNumber = (UINT8)0x0;
    SelRecord.OEMEvData1 = 0xA0 | EFI_PROCESSOR_SENSOR_OFFSET_CONFIGURATION_ERROR;
    SelRecord.OEMEvData2 = (RasSpareCoreErrorSection->Socket << 4) | (ErrorRecordDescription->Severity);
    SelRecord.OEMEvData3 = RasSpareCoreErrorSection->FailedCore;

     
    // Report the error.
    Status = LogInfoToSel( (VOID*)&SelRecord );
     
    return  Status;
}


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   LogVtdErrorType
//
// Description: 
//  Logs the Vtd Unc Error Type from Error Status
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
LogVtdUncErrorType (
  EFI_SEL_RECORD_DATA       *SelRecord,
  UINT32                    ErrorStatus
  )
{
    EFI_STATUS              Status = EFI_SUCCESS;
    UINT8                   Bit = 0;
            
    for (Bit = 0; Bit <= 31; Bit++) {
        switch (ErrorStatus & (1 << Bit)) {
            case BIT0:
                SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_UNC_BIT00;
                // Report the error.
                Status = LogInfoToSel((VOID*)&SelRecord);
                break;
            case BIT1:
                SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_UNC_BIT01;
                // Report the error.
                Status = LogInfoToSel((VOID*)&SelRecord);
                break;
            case BIT2:
                SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_UNC_BIT02;
                // Report the error.
                Status = LogInfoToSel((VOID*)&SelRecord);
                break;
            case BIT3:
                SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_UNC_BIT03;
                // Report the error.
                Status = LogInfoToSel((VOID*)&SelRecord);
                break;
/*#ifdef CPX_HOST 
            case BIT4:
                SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_UNC_BIT04;
                // Report the error.
                Status = LogInfoToSel((VOID*)&SelRecord);
                break;
            case BIT5:
                SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_UNC_BIT05;
                // Report the error.
                Status = LogInfoToSel((VOID*)&SelRecord);
                break;
            case BIT6:
                SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_UNC_BIT06;
                // Report the error.
                Status = LogInfoToSel((VOID*)&SelRecord);
                break;
            case BIT7:
                SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_UNC_BIT07;
                // Report the error.
                Status = LogInfoToSel((VOID*)&SelRecord);
                break;
            case BIT8:
                SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_UNC_BIT08;
                // Report the error.
                Status = LogInfoToSel((VOID*)&SelRecord);
                break;
#endif
                
#ifdef ICX_HOST
            case BIT16:
                SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_UNC_BIT16;
                // Report the error.
                Status = LogInfoToSel((VOID*)&SelRecord);
                break;
            case BIT17:
                SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_UNC_BIT17;
                // Report the error.
                Status = LogInfoToSel((VOID*)&SelRecord);
                break;
            case BIT18:
                SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_UNC_BIT18;
                // Report the error.
                Status = LogInfoToSel((VOID*)&SelRecord);
                break;
            case BIT19:
                SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_UNC_BIT19;
                // Report the error.
                Status = LogInfoToSel((VOID*)&SelRecord);
                break;
            case BIT20:
                SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_UNC_BIT20;
                // Report the error.
                Status = LogInfoToSel((VOID*)&SelRecord);
                break;
            case BIT21:
                SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_UNC_BIT21;
                // Report the error.
                Status = LogInfoToSel((VOID*)&SelRecord);
                break;
#endif
        
#ifdef CPX_HOST
            case BIT31:
                SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_UNC_BIT31;
                // Report the error.
                Status = LogInfoToSel((VOID*)&SelRecord);
                break;
#endif */
            default:
                SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_UNC_RESERVED;
                // Report the error.
                Status = LogInfoToSel((VOID*)&SelRecord);
                break;
        }
    }
    return Status;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   LogVtdCorErrorType
//
// Description: 
//  Logs the Vtd Corr Error Type from Error Status
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
LogVtdCorErrorType (
  EFI_SEL_RECORD_DATA       *SelRecord,
  UINT32                    ErrorStatus
  )
{
    EFI_STATUS              Status = EFI_SUCCESS;
    
    switch (ErrorStatus) {
        case BIT0:
            SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_COR_BIT00;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT1:
            SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_COR_BIT01;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT2:
            SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_COR_BIT02;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT3:
            SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_COR_BIT03;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT4:
            SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_COR_BIT04;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT5:
            SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_COR_BIT05;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT6:
            SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_COR_BIT06;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT7:
            SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_COR_BIT07;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT8:
            SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_COR_BIT08;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT9:
            SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_COR_BIT09;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT10:
            SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_COR_BIT10;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT11:
            SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_COR_BIT11;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT12:
            SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_COR_BIT12;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT13:
            SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_COR_BIT13;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT14:
            SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_COR_BIT14;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT15:
            SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_COR_BIT15;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT16:
            SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_COR_BIT16;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT17:
            SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_COR_BIT17;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT18:
            SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_COR_BIT18;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT19:
            SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_COR_BIT19;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT20:
            SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_COR_BIT20;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT21:
            SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_COR_BIT21;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT22:
            SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_COR_BIT22;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT23:
            SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_COR_BIT23;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        default:
            SelRecord->OEMEvData3 = IIO_INTERNAL_VTD_COR_RESERVED;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
    }
    return Status;
}


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   LogCBDmaUncErrorType
//
// Description: 
//  Logs the CBDma Uncorr Error Type from Error Status
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
/*
#ifdef CPX_HOST
EFI_STATUS
LogCBDmaUncErrorType (
  EFI_SEL_RECORD_DATA       *SelRecord,
  UINT32                    ErrorStatus,
  UINT32                    ErrorRegister
  )
{
    EFI_STATUS              Status = EFI_SUCCESS;
    
    if (ErrorRegister == DMAUNCERRSTS_IIOCB_FUNC0_REG)
           {    
   
    switch (ErrorStatus) {
    
        case BIT2:
            SelRecord->OEMEvData3 = IIO_INTERNAL_DMA_UNC_BIT02;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT3:
            SelRecord->OEMEvData3 = IIO_INTERNAL_DMA_UNC_BIT03;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT4:
            SelRecord->OEMEvData3 = IIO_INTERNAL_DMA_UNC_BIT04;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT7:
            SelRecord->OEMEvData3 = IIO_INTERNAL_DMA_UNC_BIT07;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT10:
            SelRecord->OEMEvData3 = IIO_INTERNAL_DMA_UNC_BIT10;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT12:
            SelRecord->OEMEvData3 = IIO_INTERNAL_DMA_UNC_BIT12;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        default:
            SelRecord->OEMEvData3 = IIO_INTERNAL_DMA_UNC_RESERVED;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
       }
}
       
if (ErrorRegister == CHANERR_INT_IIOCB_FUNC0_REG)
    {
    switch (ErrorStatus) {
        case BIT0:
             SelRecord->OEMEvData3 = IIO_INTERNAL_CHN_ERR_BIT00;
             // Report the error.
             Status = LogInfoToSel((VOID*)&SelRecord);
             break;
         case BIT1:
             SelRecord->OEMEvData3 = IIO_INTERNAL_CHN_ERR_BIT01;
             // Report the error.
             Status = LogInfoToSel((VOID*)&SelRecord);
             break;
         case BIT2:
             SelRecord->OEMEvData3 = IIO_INTERNAL_CHN_ERR_BIT02;
             // Report the error.
             Status = LogInfoToSel((VOID*)&SelRecord);
             break;
         case BIT3:
             SelRecord->OEMEvData3 = IIO_INTERNAL_CHN_ERR_BIT03;
             // Report the error.
             Status = LogInfoToSel((VOID*)&SelRecord);
             break;
         case BIT4:
             SelRecord->OEMEvData3 = IIO_INTERNAL_CHN_ERR_BIT04;
             // Report the error.
             Status = LogInfoToSel((VOID*)&SelRecord);
             break;
         case BIT5:
             SelRecord->OEMEvData3 = IIO_INTERNAL_CHN_ERR_BIT05;
             // Report the error.
             Status = LogInfoToSel((VOID*)&SelRecord);
             break;
         case BIT6:
             SelRecord->OEMEvData3 = IIO_INTERNAL_CHN_ERR_BIT06;
             // Report the error.
             Status = LogInfoToSel((VOID*)&SelRecord);
             break;
         case BIT7:
             SelRecord->OEMEvData3 = IIO_INTERNAL_CHN_ERR_BIT07;
             // Report the error.
             Status = LogInfoToSel((VOID*)&SelRecord);
             break;
         case BIT8:
             SelRecord->OEMEvData3 = IIO_INTERNAL_CHN_ERR_BIT08;
             // Report the error.
             Status = LogInfoToSel((VOID*)&SelRecord);
             break;
         case BIT9:
             SelRecord->OEMEvData3 = IIO_INTERNAL_CHN_ERR_BIT09;
             // Report the error.
             Status = LogInfoToSel((VOID*)&SelRecord);
             break;
         case BIT10:
             SelRecord->OEMEvData3 = IIO_INTERNAL_CHN_ERR_BIT10;
             // Report the error.
             Status = LogInfoToSel((VOID*)&SelRecord);
             break;
         case BIT11:
             SelRecord->OEMEvData3 = IIO_INTERNAL_CHN_ERR_BIT11;
             // Report the error.
             Status = LogInfoToSel((VOID*)&SelRecord);
             break;
         case BIT12:
             SelRecord->OEMEvData3 = IIO_INTERNAL_CHN_ERR_BIT12;
             // Report the error.
             Status = LogInfoToSel((VOID*)&SelRecord);
             break;
         case BIT13:
             SelRecord->OEMEvData3 = IIO_INTERNAL_CHN_ERR_BIT13;
             // Report the error.
             Status = LogInfoToSel((VOID*)&SelRecord);
             break;
         case BIT16:
             SelRecord->OEMEvData3 = IIO_INTERNAL_CHN_ERR_BIT16;
             // Report the error.
             Status = LogInfoToSel((VOID*)&SelRecord);
             break;
         case BIT17:
             SelRecord->OEMEvData3 = IIO_INTERNAL_CHN_ERR_BIT17;
             // Report the error.
             Status = LogInfoToSel((VOID*)&SelRecord);
             break;
         case BIT18:
             SelRecord->OEMEvData3 = IIO_INTERNAL_CHN_ERR_BIT18;
             // Report the error.
             Status = LogInfoToSel((VOID*)&SelRecord);
             break;
         case BIT19:
             SelRecord->OEMEvData3 = IIO_INTERNAL_CHN_ERR_BIT19;
             // Report the error.
             Status = LogInfoToSel((VOID*)&SelRecord);
             break;
         case BIT20:
             SelRecord->OEMEvData3 = IIO_INTERNAL_CHN_ERR_BIT20;
             // Report the error.
             Status = LogInfoToSel((VOID*)&SelRecord);
             break;
         case BIT21:
             SelRecord->OEMEvData3 = IIO_INTERNAL_CHN_ERR_BIT21;
             // Report the error.
             Status = LogInfoToSel((VOID*)&SelRecord);
             break;
         case BIT22:
             SelRecord->OEMEvData3 = IIO_INTERNAL_CHN_ERR_BIT22;
             // Report the error.
             Status = LogInfoToSel((VOID*)&SelRecord);
             break;
         case BIT23:
             SelRecord->OEMEvData3 = IIO_INTERNAL_CHN_ERR_BIT23;
             // Report the error.
             Status = LogInfoToSel((VOID*)&SelRecord);
             break;
         default:
             SelRecord->OEMEvData3 = IIO_INTERNAL_CHN_ERR_RESERVED;
             // Report the error.
             Status = LogInfoToSel((VOID*)&SelRecord);
             break;
    }

    }
    
    return Status;
}
#endif 
*/
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   LogIrpErrorType
//
// Description: 
//  Logs the IRP Error Type from Error Status
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
LogIrpErrorType (
  EFI_SEL_RECORD_DATA       *SelRecord,
  UINT32                    ErrorStatus
  )
{
    EFI_STATUS              Status = EFI_SUCCESS;
    
    switch (ErrorStatus) {
        case BIT1:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPERR_BIT01;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT2:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPERR_BIT02;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT3:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPERR_BIT03;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT4:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPERR_BIT04;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT5:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPERR_BIT05;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT6:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPERR_BIT06;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT10:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPERR_BIT10;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT13:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPERR_BIT13;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT14:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPERR_BIT14;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
/*#ifdef ICX_HOST
        case BIT16:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPERR_BIT16;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT17:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPERR_BIT17;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT18:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPERR_BIT18;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT19:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPERR_BIT19;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT20:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPERR_BIT20;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT21:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPERR_BIT21;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
#endif*/
        default:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPERR_RESERVED;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
    }
    return Status;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   LogRingErrorType
//
// Description: 
//  Logs the Ring Error Type from Error Status
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
LogRingErrorType (
  EFI_SEL_RECORD_DATA       *SelRecord,
  UINT32                    ErrorStatus
  )
{
    EFI_STATUS              Status = EFI_SUCCESS;
    
    switch (ErrorStatus) {
        case BIT0:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPRING_BIT00;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT1:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPRING_BIT01;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT2:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPRING_BIT02;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT3:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPRING_BIT03;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT4:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPRING_BIT04;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
/*#ifdef ICX_HOST
        case BIT5:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPRING_BIT05;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT6:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPRING_BIT06;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT7:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPRING_BIT07;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT8:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPRING_BIT08;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT9:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPRING_BIT09;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT10:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPRING_BIT10;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT11:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPRING_BIT11;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT12:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPRING_BIT12;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT13:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPRING_BIT13;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
#endif*/
        default:
            SelRecord->OEMEvData3 = IIO_INTERNAL_IRPRING_RESERVED;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
    }
    return Status;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   LogItcErrorType
//
// Description: 
//  Logs the ITC Error Type from Error Status
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
LogItcErrorType (
  EFI_SEL_RECORD_DATA       *SelRecord,
  UINT32                    ErrorStatus
  )
{
    EFI_STATUS              Status = EFI_SUCCESS;

    switch (ErrorStatus) {
/*#ifdef ICX_HOST  
        case BIT12:
            SelRecord->OEMEvData3 = IIO_INTERNAL_ITC_BIT12;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT13:
            SelRecord->OEMEvData3 = IIO_INTERNAL_ITC_BIT13;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT14:
            SelRecord->OEMEvData3 = IIO_INTERNAL_ITC_BIT14;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT15:
            SelRecord->OEMEvData3 = IIO_INTERNAL_ITC_BIT15;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
#endif*/
        case BIT16:
            SelRecord->OEMEvData3 = IIO_INTERNAL_ITC_BIT16;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT17:
            SelRecord->OEMEvData3 = IIO_INTERNAL_ITC_BIT17;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT18:
            SelRecord->OEMEvData3 = IIO_INTERNAL_ITC_BIT18;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT19:
            SelRecord->OEMEvData3 = IIO_INTERNAL_ITC_BIT19;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT20:
            SelRecord->OEMEvData3 = IIO_INTERNAL_ITC_BIT20;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT21:
            SelRecord->OEMEvData3 = IIO_INTERNAL_ITC_BIT21;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT22:
            SelRecord->OEMEvData3 = IIO_INTERNAL_ITC_BIT22;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT23:
            SelRecord->OEMEvData3 = IIO_INTERNAL_ITC_BIT23;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT24:
            SelRecord->OEMEvData3 = IIO_INTERNAL_ITC_BIT24;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT25:
            SelRecord->OEMEvData3 = IIO_INTERNAL_ITC_BIT25;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT26:
            SelRecord->OEMEvData3 = IIO_INTERNAL_ITC_BIT26;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT27:
            SelRecord->OEMEvData3 = IIO_INTERNAL_ITC_BIT27;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT28:
            SelRecord->OEMEvData3 = IIO_INTERNAL_ITC_BIT28;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT29:
            SelRecord->OEMEvData3 = IIO_INTERNAL_ITC_BIT29;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
/*#ifdef ICX_HOST
        case BIT30:
            SelRecord->OEMEvData3 = IIO_INTERNAL_ITC_BIT30;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT31:
            SelRecord->OEMEvData3 = IIO_INTERNAL_ITC_BIT31;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
#endif*/
        default:
            SelRecord->OEMEvData3 = IIO_INTERNAL_ITC_RESERVED;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
    }
    return Status;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   LogOtcErrorType
//
// Description: 
//  Logs the OTC Error Type from Error Status
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
LogOtcErrorType (
  EFI_SEL_RECORD_DATA       *SelRecord,
  UINT32                    ErrorStatus
  )
{
    EFI_STATUS              Status = EFI_SUCCESS;
    
    switch (ErrorStatus) {
        case BIT0:
            SelRecord->OEMEvData3 = IIO_INTERNAL_OTC_BIT00;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT1:
            SelRecord->OEMEvData3 = IIO_INTERNAL_OTC_BIT01;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT2:
            SelRecord->OEMEvData3 = IIO_INTERNAL_OTC_BIT02;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT3:
            SelRecord->OEMEvData3 = IIO_INTERNAL_OTC_BIT03;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT4:
            SelRecord->OEMEvData3 = IIO_INTERNAL_OTC_BIT04;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT5:
            SelRecord->OEMEvData3 = IIO_INTERNAL_OTC_BIT05;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT6:
            SelRecord->OEMEvData3 = IIO_INTERNAL_OTC_BIT06;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT7:
            SelRecord->OEMEvData3 = IIO_INTERNAL_OTC_BIT07;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT8:
            SelRecord->OEMEvData3 = IIO_INTERNAL_OTC_BIT08;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT9:
            SelRecord->OEMEvData3 = IIO_INTERNAL_OTC_BIT09;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT10:
            SelRecord->OEMEvData3 = IIO_INTERNAL_OTC_BIT10;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT11:
            SelRecord->OEMEvData3 = IIO_INTERNAL_OTC_BIT11;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT12:
            SelRecord->OEMEvData3 = IIO_INTERNAL_OTC_BIT12;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT13:
            SelRecord->OEMEvData3 = IIO_INTERNAL_OTC_BIT13;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
/*#ifdef ICX_HOST
        case BIT14:
            SelRecord->OEMEvData3 = IIO_INTERNAL_OTC_BIT14;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT15:
            SelRecord->OEMEvData3 = IIO_INTERNAL_OTC_BIT15;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT16:
            SelRecord->OEMEvData3 = IIO_INTERNAL_OTC_BIT16;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT17:
            SelRecord->OEMEvData3 = IIO_INTERNAL_OTC_BIT17;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT18:
            SelRecord->OEMEvData3 = IIO_INTERNAL_OTC_BIT18;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT19:
            SelRecord->OEMEvData3 = IIO_INTERNAL_OTC_BIT19;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
        case BIT20:
            SelRecord->OEMEvData3 = IIO_INTERNAL_OTC_BIT20;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
#endif*/
        default:
            SelRecord->OEMEvData3 = IIO_INTERNAL_OTC_RESERVED;
            // Report the error.
            Status = LogInfoToSel((VOID*)&SelRecord);
            break;
    }
    return Status;
}
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   LogIioInternalErrorToSel
//
// Description: 
//  Log the IIO Internal errors to BMC SEL
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
LogIioInternalErrorToSel (
  EFI_COMMON_ERROR_RECORD_HEADER    *ErrorRecordHeader,
  BOOLEAN                           CurrentBoot,
  UINT32                            ErrorStatus,
  UINT32                            ErrorRegister
  )
{
    EFI_STATUS                      Status = EFI_SUCCESS;
    EFI_SEL_RECORD_DATA             SelRecord;
    EFI_ERROR_SECTION_DESCRIPTOR    *ErrorRecordDescription;
    RAS_IIO_INTERNAL_ERROR_SECTION  *RasIioInternalErrorSection;
    
    ErrorRecordDescription = (EFI_ERROR_SECTION_DESCRIPTOR *)((UINT8 *)ErrorRecordHeader + sizeof(EFI_COMMON_ERROR_RECORD_HEADER));
    RasIioInternalErrorSection = (RAS_IIO_INTERNAL_ERROR_SECTION *)((UINT8 *)ErrorRecordDescription + sizeof(EFI_ERROR_SECTION_DESCRIPTOR));
    
    // Initialize generic parts of SEL record.
    Status = UpdateGenericSelInfo(&SelRecord);

    // Initialize specific parts of SEL record.
    SelRecord.SensorType = EFI_PROCESSOR_SENSOR_TYPE;
    SelRecord.SensorNumber = (UINT8)0x0;

    SelRecord.OEMEvData1 = 0xA0 | EFI_PROCESSOR_SENSOR_OFFSET_CONFIGURATION_ERROR; // Add error Id to identify the error
    
    if (CurrentBoot) {
        SelRecord.OEMEvData2 = (RasIioInternalErrorSection->Socket << 4) | (RasIioInternalErrorSection->IioStack << 1);
    }
    else {
        SelRecord.OEMEvData2 = (RasIioInternalErrorSection->Socket << 4) | (RasIioInternalErrorSection->IioStack << 1) | BIT0; // //ED3[0]: 0 - Current Boot. 1 - Last Boot
    }
    
    switch (RasIioInternalErrorSection->IioErrorType) {
        case RasIioInternalErrorVtd:
            if ((ErrorRecordDescription->Severity == NONFATAL_ERROR_TYPE) || (ErrorRecordDescription->Severity == FATAL_ERROR_TYPE)) {
                Status = LogVtdUncErrorType (&SelRecord, ErrorStatus);
            }
            if (ErrorRecordDescription->Severity == COR_ERROR_TYPE) {
                Status = LogVtdCorErrorType (&SelRecord, ErrorStatus);
            }
            break;
        case RasIioInternalErrorCBDma:
/*#ifdef CPX_HOST
            Status = LogCBDmaUncErrorType (&SelRecord, ErrorStatus, ErrorRegister);
#endif*/
            break;
        case RasIioInternalErrorM2PCIE:
            // Not Implemented
            break;
        case RasIioInternalErrorIrp:
            Status = LogIrpErrorType (&SelRecord, ErrorStatus);
            break;
        case RasIioInternalErrorRing:
            Status = LogRingErrorType (&SelRecord, ErrorStatus);
            break;
        case RasIioInternalErrorItc:
            Status = LogItcErrorType (&SelRecord, ErrorStatus);
            break;
        case RasIioInternalErrorOtc:
            Status = LogOtcErrorType (&SelRecord, ErrorStatus);
            break;
        default:
            break;
    }

    return  Status;
}
