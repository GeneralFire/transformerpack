//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
// @file PlatformPeiSystemInformation.c
//  This file retrieves the system information and prints it to the screen
//

#include "PlatformPeiSystemInformation.h"

static EFI_PEI_NOTIFY_DESCRIPTOR  mPeiCpuDimmNotifyList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiSiliconRcHobsReadyPpi,
    OutputCpuDimmInformation
  }
};

/**
    Outputs system information to Pei Early Video

    @param[in] PeiServices   Describes the list of possible PEI Services.

    @retval  EFI_STATUS
**/
EFI_STATUS
OutputBiosVersion (
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{   
    EFI_STATUS                  Status;
    AMI_PEI_TEXT_OUT            *EarlyVideoPpi;
    CHAR8                       TextString[160];
    AMI_TEXT_OUT_HOB            *VideoConsoleHob;   
	CHAR8                       PlatformString[20];
   
    DEBUG((EFI_D_INFO,"%a Entry... \n",__FUNCTION__));
    
    Status = (*PeiServices)->LocatePpi(
                                    PeiServices,
#if ConsoleOutConsplitter_SUPPORT 
                                    &gAmiPeiConsplitterTextOutPpiGuid,
#else                         
                                    &gAmiPeiTextOutPpiGuid,
#endif                        
                                    0,
                                    NULL,
                                    &EarlyVideoPpi);
    if (EFI_ERROR(Status)) 
        return Status;
 
    AsciiSPrint (PlatformString, 20, CONVERT_TO_STRING(PLATFORM_NAME));
    
    VideoConsoleHob = GetHobList();
    if (VideoConsoleHob == NULL) {
        return EFI_NOT_FOUND;
    }
    
    VideoConsoleHob = GetNextGuidHob (&gAmiVideoTextOutHobGuid, VideoConsoleHob);
    if (VideoConsoleHob == NULL) {
        return EFI_NOT_FOUND;
    }
    VideoConsoleHob->UpperCursorCol = 0;
    VideoConsoleHob->UpperCursorRow = 0;
    
    Status = EarlyVideoPpi->SetAttribute(EarlyVideoPpi, 0xF, 0, 0);
    AsciiSPrint (TextString, 160, "Copyright(c) %d American Megatrends, Inc. \n\r", THIS_YEAR);
    Status = EarlyVideoPpi->WriteString(EarlyVideoPpi, 0, TextString); 
	Status = EarlyVideoPpi->WriteString(EarlyVideoPpi, 0, "Sample System Information \n\r"); 
    AsciiSPrint (TextString, 160, "%a System BIOS Version: %a Date: %a \n\r", PlatformString, CONVERT_TO_STRING(BIOS_TAG), CONVERT_TO_STRING(TODAY));
    Status = EarlyVideoPpi->WriteString(EarlyVideoPpi, 0, TextString);
    Status = EarlyVideoPpi->SetAttribute(EarlyVideoPpi, 0x7, 0, 0); 

    return Status;
}

/**
    Outputs system information to Pei Early Video

    @param[in] PeiServices        Describes the list of possible PEI Services.
    @param[in] NotifyDescriptor   
    @param[in] Ppi   

    @retval  EFI_STATUS
**/
EFI_STATUS
OutputCpuDimmInformation (
  IN CONST EFI_PEI_SERVICES             **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR          *NotifyDescriptor,
  IN VOID                               *Ppi
  )
{
    EFI_STATUS              Status;
    AMI_PEI_TEXT_OUT	    *EarlyVideoPpi;

    DEBUG((EFI_D_INFO,"%a Entry... \n",__FUNCTION__));

    Status = (*PeiServices)->LocatePpi(
                                    PeiServices,
#if ConsoleOutConsplitter_SUPPORT 
                                    &gAmiPeiConsplitterTextOutPpiGuid,
#else                
                                    &gAmiPeiTextOutPpiGuid,
#endif          
                                    0,
                                    NULL,
                                    &EarlyVideoPpi);
    
    if (EFI_ERROR(Status))
        return Status;

    InitializeCpuData (PeiServices, EarlyVideoPpi);
    
    InitializeDimmData (PeiServices, EarlyVideoPpi);
    return Status;
}

/**
    Registers notification for gEfiSiliconRcHobsReadyPpi. 

    @param[in] FileHandle    Pointer to image file handle.
    @param[in] PeiServices   Describes the list of possible PEI Services.

    @retval  EFI_STATUS
**/
EFI_STATUS
PeiSystemInformationEntryPoint (
    IN        EFI_PEI_FILE_HANDLE     FileHandle,
    IN  CONST EFI_PEI_SERVICES        **PeiServices 
    )
{
        EFI_STATUS                      Status;
        
        DEBUG((EFI_D_INFO,"%a Entry... \n",__FUNCTION__));
        
        Status = OutputBiosVersion(PeiServices);

        if (EFI_ERROR(Status)) {
            return Status;
        }
        
        Status = (*PeiServices)->NotifyPpi(PeiServices, mPeiCpuDimmNotifyList);
        
        return Status;
}
