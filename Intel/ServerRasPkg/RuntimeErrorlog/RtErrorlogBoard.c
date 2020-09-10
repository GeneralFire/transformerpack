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

//<AMI_FHDR_START>
//----------------------------------------------------------------------------
//
// Name:		RtErrorLogBoard.c
//
// Description:	
//
//----------------------------------------------------------------------------
//<AMI_FHDR_END>

#include "RtErrorlogBoard.h"


//---------------------------------------------------------------------------
// External (eLink) handler functions
//---------------------------------------------------------------------------

// Memory Error Handler
typedef EFI_STATUS (MEM_ERROR_HANDLER_FUNCTION) (
    EFI_COMMON_ERROR_RECORD_HEADER  *MemInfo,
    BOOLEAN                         CurrentBoot
);

extern MEM_ERROR_HANDLER_FUNCTION MEM_ERROR_HANDLER_FUNCTION_LIST EndOfHandlerList;

MEM_ERROR_HANDLER_FUNCTION* MemErrorHandlerList[] = {
    MEM_ERROR_HANDLER_FUNCTION_LIST
    NULL
};

// PCIe Error Handler
typedef EFI_STATUS (PCI_ERROR_HANDLER_FUNCTION) (
    EFI_COMMON_ERROR_RECORD_HEADER  *PcieInfo,
    BOOLEAN                         CurrentBoot
);

extern PCI_ERROR_HANDLER_FUNCTION PCI_ERROR_HANDLER_FUNCTION_LIST PciEndOfHandlerList;

PCI_ERROR_HANDLER_FUNCTION* PciErrorHandlerList[] = {
    PCI_ERROR_HANDLER_FUNCTION_LIST
    NULL
};

// Processor Error Handler
typedef EFI_STATUS (PROCESSOR_ERROR_HANDLER_FUNCTION) (
    EFI_COMMON_ERROR_RECORD_HEADER  *ProcInfo,
    BOOLEAN                         CurrentBoot
);

extern PROCESSOR_ERROR_HANDLER_FUNCTION PROCESSOR_ERROR_HANDLER_FUNCTION_LIST EndOfProcessorHandlerList;

PROCESSOR_ERROR_HANDLER_FUNCTION* ProcessorErrorHandlerList[] = {
    PROCESSOR_ERROR_HANDLER_FUNCTION_LIST
    NULL
};

// UPI Error Handler
typedef EFI_STATUS (UPI_ERROR_HANDLER_FUNCTION) (
    EFI_COMMON_ERROR_RECORD_HEADER  *UpiInfo,
    BOOLEAN                         CurrentBoot
);

extern UPI_ERROR_HANDLER_FUNCTION UPI_ERROR_HANDLER_FUNCTION_LIST EndOfUpiHandlerList;

UPI_ERROR_HANDLER_FUNCTION* UpiErrorHandlerList[] = {
    UPI_ERROR_HANDLER_FUNCTION_LIST
    NULL
};

// IEH Error Handler
typedef EFI_STATUS (IEH_ERROR_HANDLER_FUNCTION) (
    EFI_COMMON_ERROR_RECORD_HEADER  *IehInfo,
    BOOLEAN                         CurrentBoot
);

extern IEH_ERROR_HANDLER_FUNCTION IEH_ERROR_HANDLER_FUNCTION_LIST EndOfIehHandlerList;

IEH_ERROR_HANDLER_FUNCTION* IehErrorHandlerList[] = {
    IEH_ERROR_HANDLER_FUNCTION_LIST
    NULL
};

// IioXp Error Handler
typedef EFI_STATUS (IIOXP_ERROR_HANDLER_FUNCTION) (
    EFI_COMMON_ERROR_RECORD_HEADER  *IioXpInfo,
    BOOLEAN                         CurrentBoot
);

extern IIOXP_ERROR_HANDLER_FUNCTION IIOXP_ERROR_HANDLER_FUNCTION_LIST EndOfIioXpHandlerList;

IIOXP_ERROR_HANDLER_FUNCTION* IioXpErrorHandlerList[] = {
    IIOXP_ERROR_HANDLER_FUNCTION_LIST
    NULL
};


// LeakyBucket Error Handler
typedef EFI_STATUS (LEAKY_BUCKET_ERROR_HANDLER_FUNCTION) (
    EFI_COMMON_ERROR_RECORD_HEADER  *LeakyBucketInfo,
    BOOLEAN                         CurrentBoot
);

extern LEAKY_BUCKET_ERROR_HANDLER_FUNCTION LEAKY_BUCKET_ERROR_HANDLER_FUNCTION_LIST EndOfLeakyBucketHandlerList;

LEAKY_BUCKET_ERROR_HANDLER_FUNCTION* LeakyBucketErrorHandlerList[] = {
    LEAKY_BUCKET_ERROR_HANDLER_FUNCTION_LIST
    NULL
};


// SpareCore Error Handler
typedef EFI_STATUS (SPARE_CORE_ERROR_HANDLER_FUNCTION) (
    EFI_COMMON_ERROR_RECORD_HEADER  *SpareCoreInfo,
    BOOLEAN                         CurrentBoot
);

extern SPARE_CORE_ERROR_HANDLER_FUNCTION SPARE_CORE_ERROR_HANDLER_FUNCTION_LIST EndOfSpareCoreHandlerList;

SPARE_CORE_ERROR_HANDLER_FUNCTION* SpareCoreErrorHandlerList[] = {
    SPARE_CORE_ERROR_HANDLER_FUNCTION_LIST
    NULL
};


// IioInternal Error Handler
typedef EFI_STATUS (IIO_INTERNAL_ERROR_HANDLER_FUNCTION) (
    EFI_COMMON_ERROR_RECORD_HEADER  *IioInternalInfo,
    BOOLEAN                         CurrentBoot,
    UINT32                          ErrorStatus, 
    UINT32                          ErrorRegister
);

extern IIO_INTERNAL_ERROR_HANDLER_FUNCTION IIO_INTERNAL_ERROR_HANDLER_FUNCTION_LIST EndOfIioInternalHandlerList;

IIO_INTERNAL_ERROR_HANDLER_FUNCTION* IioInternalErrorHandlerList[] = {
    IIO_INTERNAL_ERROR_HANDLER_FUNCTION_LIST
    NULL
};

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   LogMemErrorRecoed
//
// Description: 
//
// Parameters:  
//
// Returns:     
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
LogMemErrorRecord (
  EFI_COMMON_ERROR_RECORD_HEADER    *ErrorRecordHeader,
  BOOLEAN                           CurrentBoot
  )
{

    // Call each handler function.
    {
        UINTN i;
        
        for(i = 0; MemErrorHandlerList[i]; i++) {
            MemErrorHandlerList[i] (ErrorRecordHeader, CurrentBoot);
        }
    }

  return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   LogPciErrorRecord
//
// Description: 
//
// Parameters:  
//
// Returns:     
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
LogPciErrorRecord (
  EFI_COMMON_ERROR_RECORD_HEADER    *ErrorRecordHeader,
  BOOLEAN                           CurrentBoot
  )
{

    // Call each handler function.
    {
        UINTN i;
        
        for(i = 0; PciErrorHandlerList[i]; i++) {
            PciErrorHandlerList[i] (ErrorRecordHeader, CurrentBoot);
        }
    }

    return EFI_SUCCESS;

}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   LogProcessorErrorRecord
//
// Description: 
//
// Parameters:  
//
// Returns:     
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
LogProcessorErrorRecord (
  EFI_COMMON_ERROR_RECORD_HEADER    *ErrorRecordHeader,
  BOOLEAN                           CurrentBoot
  )
{

    // Call each handler function.
    {
        UINTN i;
        
        for(i = 0; ProcessorErrorHandlerList[i]; i++) {
            ProcessorErrorHandlerList[i] (ErrorRecordHeader, CurrentBoot);
        }
    }

  return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   LogUpiErrorRecord
//
// Description: 
//
// Parameters:  
//
// Returns:     
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
LogUpiErrorRecord (
  EFI_COMMON_ERROR_RECORD_HEADER    *ErrorRecordHeader,
  BOOLEAN                           CurrentBoot
  )
{

    // Call each handler function.
    {
        UINTN i;
        
        for(i = 0; UpiErrorHandlerList[i]; i++) {
            UpiErrorHandlerList[i] (ErrorRecordHeader, CurrentBoot);
        }
    }
    
    return  EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   LogIehErrorRecord
//
// Description: 
//
// Parameters:  
//
// Returns:     
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
LogIehErrorRecord (
  EFI_COMMON_ERROR_RECORD_HEADER    *ErrorRecordHeader,
  BOOLEAN                           CurrentBoot
  )
{

    // Call each handler function.
    {
        UINTN i;
        
        for(i = 0; IehErrorHandlerList[i]; i++) {
            IehErrorHandlerList[i] (ErrorRecordHeader, CurrentBoot);
        }
    }
    
    return  EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   LogIioXpErrorRecord
//
// Description: 
//
// Parameters:  
//
// Returns:     
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
LogIioXpErrorRecord (
  EFI_COMMON_ERROR_RECORD_HEADER    *ErrorRecordHeader,
  BOOLEAN                           CurrentBoot
  )
{

    // Call each handler function.
    {
        UINTN i;
        
        for(i = 0; IioXpErrorHandlerList[i]; i++) {
            IioXpErrorHandlerList[i] (ErrorRecordHeader, CurrentBoot);
        }
    }
    
    return  EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   LogLeakyBucketErrorRecord
//
// Description: 
//
// Parameters:  
//
// Returns:     
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
LogLeakyBucketErrorRecord (
  EFI_COMMON_ERROR_RECORD_HEADER    *ErrorRecordHeader,
  BOOLEAN                           CurrentBoot
  )
{

    // Call each handler function.
    {
        UINTN i;
        
        for(i = 0; LeakyBucketErrorHandlerList[i]; i++) {
            LeakyBucketErrorHandlerList[i] (ErrorRecordHeader, CurrentBoot);
        }
    }
    
    return  EFI_SUCCESS;
}


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   LogSpareCoreErrorRecord
//
// Description: 
//
// Parameters:  
//
// Returns:     
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
LogSpareCoreErrorRecord (
  EFI_COMMON_ERROR_RECORD_HEADER    *ErrorRecordHeader,
  BOOLEAN                           CurrentBoot
  )
{

    // Call each handler function.
    {
        UINTN i;
        
        for(i = 0; SpareCoreErrorHandlerList[i]; i++) {
            SpareCoreErrorHandlerList[i] (ErrorRecordHeader, CurrentBoot);
        }
    }
    
    return  EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   LogIioInternalErrorRecord
//
// Description: 
//
// Parameters:  
//
// Returns:     
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
LogIioInternalErrorRecord (
  EFI_COMMON_ERROR_RECORD_HEADER    *ErrorRecordHeader,
  BOOLEAN                           CurrentBoot
  )
{
    EFI_ERROR_SECTION_DESCRIPTOR    *ErrorRecordDescription;
    RAS_IIO_INTERNAL_ERROR_SECTION  *RasIioInternalErrorSection;
    UINT32                          ErrorStatus = 0;
    UINT32                          ErrorRegister = 0;
    UINT8                           Bus;
    UINT8                           Function = 0;
    
    ErrorRecordDescription = (EFI_ERROR_SECTION_DESCRIPTOR *)((UINT8 *)ErrorRecordHeader + sizeof(EFI_COMMON_ERROR_RECORD_HEADER));
    RasIioInternalErrorSection = (RAS_IIO_INTERNAL_ERROR_SECTION *)((UINT8 *)ErrorRecordDescription + sizeof(EFI_ERROR_SECTION_DESCRIPTOR));

    GetStackStartBus (RasIioInternalErrorSection->Socket, RasIioInternalErrorSection->IioStack, &Bus);
    
    switch (RasIioInternalErrorSection->IioErrorType) {
        case RasIioInternalErrorVtd:
            if ((ErrorRecordDescription->Severity == NONFATAL_ERROR_TYPE) || (ErrorRecordDescription->Severity == FATAL_ERROR_TYPE)) {
                //ErrorStatus = MmioRead32 (MmPciAddressExt (RasIioInternalErrorSection->Socket, Bus, RT_VTD_DEV, RT_VTD_FUN, ONLY_REGISTER_OFFSET (VTUNCERRSTS_IIO_VTD_REG)));
            }
            if (ErrorRecordDescription->Severity == COR_ERROR_TYPE) {
#ifdef ICX_HOST
                ErrorStatus =  MmioRead32 (MmPciAddressExt (RasIioInternalErrorSection->Socket, Bus, RT_VTD_DEV, RT_VTD_FUN, ONLY_REGISTER_OFFSET (VTCORERRSTS_IIO_VTD_REG)));
#endif
            }
            break;
        case RasIioInternalErrorCBDma:
#ifdef CPX_HOST
            for (Function =0; Function <= RT_CBDMA_FUN_MAX;Function++){
                if (Function ==0){
                    ErrorStatus = MmioRead32 (MmPciAddressExt (RasIioInternalErrorSection->Socket, Bus, RT_CBDMA_DEV, Function, ONLY_REGISTER_OFFSET (DMAUNCERRSTS_IIOCB_FUNC0_REG)));
                    if (ErrorStatus != 0) {
                        ErrorRegister =  DMAUNCERRSTS_IIOCB_FUNC0_REG;
                        break;
                        }
                }
                    ErrorStatus = MmioRead32 (MmPciAddressExt (RasIioInternalErrorSection->Socket, Bus, RT_CBDMA_DEV, Function, ONLY_REGISTER_OFFSET (CHANERR_INT_IIOCB_FUNC0_REG)));
                    if (ErrorStatus != 0) {
                        ErrorRegister =  CHANERR_INT_IIOCB_FUNC0_REG;
                        break;
                        }
            }

#endif
            break;
        case RasIioInternalErrorM2PCIE:
            // Not Implemented
            break;
        case RasIioInternalErrorIrp:
#ifdef ICX_HOST
            ErrorStatus = MmioRead32 (MmPciAddressExt (RasIioInternalErrorSection->Socket, Bus, RT_RAS_DEV, RT_RAS_FUN, ONLY_REGISTER_OFFSET (IRPERRST_IIO_RAS_REG)));
#endif
#ifdef CPX_HOST
            ErrorStatus = MmioRead32 (MmPciAddressExt (RasIioInternalErrorSection->Socket, Bus, RT_RAS_DEV, RT_RAS_FUN, ONLY_REGISTER_OFFSET (IRPP0ERRST_IIO_RAS_REG)));
#endif
            
            break;
        case RasIioInternalErrorRing:

//            ErrorStatus = MmioRead32 (MmPciAddressExt (RasIioInternalErrorSection->Socket, Bus, RT_RAS_DEV, RT_RAS_FUN, ONLY_REGISTER_OFFSET (IRPRINGERRST_IIO_RAS_REG)));

            break;
        case RasIioInternalErrorItc:
#ifdef ICX_HOST
            ErrorStatus = MmioRead32 (MmPciAddressExt (RasIioInternalErrorSection->Socket, Bus, RT_RAS_DEV, RT_RAS_FUN, ONLY_REGISTER_OFFSET (ITCERRST_IIO_RAS_REG)));
#endif
            
#ifdef CPX_HOST
            ErrorStatus = MmioRead32 (MmPciAddressExt (RasIioInternalErrorSection->Socket, Bus, RT_RAS_DEV, RT_RAS_FUN, ONLY_REGISTER_OFFSET (TCERRST_IIO_RAS_REG)));
#endif            
            
            break;
        case RasIioInternalErrorOtc:
#ifdef ICX_HOST
            ErrorStatus = MmioRead32 (MmPciAddressExt (RasIioInternalErrorSection->Socket, Bus, RT_RAS_DEV, RT_RAS_FUN, ONLY_REGISTER_OFFSET (OTCERRST_IIO_RAS_REG)));
#endif

#ifdef CPX_HOST
            ErrorStatus = MmioRead32 (MmPciAddressExt (RasIioInternalErrorSection->Socket, Bus, RT_RAS_DEV, RT_RAS_FUN, ONLY_REGISTER_OFFSET (TCERRST_IIO_RAS_REG)));
#endif     
            break;
        default:
            break;
    }

    // Call each handler function.
    {
        UINTN i;
        
        for(i = 0; IioInternalErrorHandlerList[i]; i++) {
            IioInternalErrorHandlerList[i] (ErrorRecordHeader, CurrentBoot, ErrorStatus, ErrorRegister);
        }
    }
    
    return  EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   LogRasNonStandardErrorRecord
//
// Description: 
//
// Parameters:  
//
// Returns:     
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
LogRasNonStandardErrorRecord (
  EFI_COMMON_ERROR_RECORD_HEADER    *ErrorRecordHeader,
  BOOLEAN                           CurrentBoot
  )
{
    EFI_ERROR_SECTION_DESCRIPTOR    *ErrorRecordDescription;
    RAS_NON_STANDARD_ERROR_SECTION  *RasNonStandardErrorRecord;
    
    ErrorRecordDescription = (EFI_ERROR_SECTION_DESCRIPTOR *)((UINT8 *)ErrorRecordHeader + sizeof(EFI_COMMON_ERROR_RECORD_HEADER));
    RasNonStandardErrorRecord = (RAS_NON_STANDARD_ERROR_SECTION *)((UINT8 *)ErrorRecordDescription + sizeof(EFI_ERROR_SECTION_DESCRIPTOR));

    switch (RasNonStandardErrorRecord->Common.ErrorType) {
        case RasErrorTypeSparing:
            // Not Implemented
            break;
        case RasErrorTypeUpiFailover:
		case RasErrorTypeUpiError:
            LogUpiErrorRecord (ErrorRecordHeader, CurrentBoot);
            break;
        case RasErrorTypeIeh:
            LogIehErrorRecord (ErrorRecordHeader, CurrentBoot);
            break;
        case RasErrorTypeIioInternal:
            LogIioInternalErrorRecord (ErrorRecordHeader, CurrentBoot);
            break;
        case RasErrorTypeIioXpError:
            LogIioXpErrorRecord (ErrorRecordHeader, CurrentBoot);
            break;
        case RasErrorTypeSpdSmbus:
            // Not Implemented
            break;
        case RasErrorTypeDdrtAlert:
            // Not Implemented
            break;
        case RasErrorTypeSpareCore:
            LogSpareCoreErrorRecord (ErrorRecordHeader, CurrentBoot);
            break;
        case RasErrorTypeUboxOrder:
            // Not Implemented
            break;
        case RasErrorTypeLeakyBucket:
            LogLeakyBucketErrorRecord (ErrorRecordHeader, CurrentBoot);
            break;
        default:
            DEBUG ((DEBUG_INFO, "ERROR: ErrorType is unknown (%d)\n", RasNonStandardErrorRecord->Common.ErrorType));
			break;
    }

    return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   CommonErrorHandling
//
// Description: 
//
// Parameters:  
//
// Returns:     
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
CommonErrorHandling (
  VOID      *ErrorDataStructure,
  BOOLEAN   CurrentBoot
  )
{
  EFI_STATUS                            Status = EFI_SUCCESS;
  EFI_COMMON_ERROR_RECORD_HEADER        *ErrorRecordHeader;
  EFI_ERROR_SECTION_DESCRIPTOR          *ErrorRecordDescription;

  ErrorRecordHeader = (EFI_COMMON_ERROR_RECORD_HEADER *)ErrorDataStructure;
  ErrorRecordDescription = (EFI_ERROR_SECTION_DESCRIPTOR *)((UINT8 *)ErrorRecordHeader + sizeof(EFI_COMMON_ERROR_RECORD_HEADER));
  
  if (CompareGuid (&ErrorRecordDescription->SectionType, &gEfiPlatformMemoryErrorSectionGuid)) {
      LogMemErrorRecord (ErrorRecordHeader, CurrentBoot);
  }
  else if (CompareGuid (&ErrorRecordDescription->SectionType, &gEfiPcieErrorSectionGuid)) {
      LogPciErrorRecord (ErrorRecordHeader, CurrentBoot);
  }
  else if (CompareGuid (&ErrorRecordDescription->SectionType, &gEfiProcessorGenericErrorSectionGuid)) {
      LogProcessorErrorRecord (ErrorRecordHeader, CurrentBoot);
  }
  else if (CompareGuid (&ErrorRecordDescription->SectionType, &gRasNonStandardErrorSectionGuid)) {
      LogRasNonStandardErrorRecord (ErrorRecordHeader, CurrentBoot);
  }
  
  return Status;
}

