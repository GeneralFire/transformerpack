//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file PlatformVideoInit.c
    LIB driver for initializing the Video in PEI 

**/
#include <PlatformVideoInit.h>

CONST EFI_PEI_PPI_DESCRIPTOR gVideoInitDoneSentSignalPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gVideoInitDoneSentGuid,
  NULL
};

/**
    Initializes video controller with VGA standard init. 

    @param[in] PeiServices   Describes the list of possible PEI Services.

    @retval  EFI_STATUS
**/
EFI_STATUS 
AmiVideoInit (
    IN  EFI_PEI_SERVICES  **PeiServices
)
{
    VIDEO_PARAMETERS    *DevInfo;
    EFI_STATUS          Status;  
    
    DevInfo = AllocateZeroPool(sizeof(VIDEO_PARAMETERS));
    
    if (DevInfo == NULL) {
        return EFI_OUT_OF_RESOURCES;    
    }
    
    // Fill the platform related parameters for Video Init
    DevInfo->RootBus            = mVideoVidPath[0].Bus;
    DevInfo->DevPath            = mVideoVidPath;                                            //Pointer to a buffer of dev path structure that define the location of the video controller in the PCI topology  
    DevInfo->DevPathEntries     = sizeof(mVideoVidPath)/sizeof(DEV_PATH);                   //Count of entries in the preceding buffer.        
    DevInfo->PciExpressCfgBase  = (UINTN) PcdGet64(PcdPciExpressBaseAddress);               //The base address of PCI Express Memory Mapped Configuration Space.
    DevInfo->MemBase            = (UINTN) PcdGet64(PcdPciExpressBaseAddress) + 0x10000000;  //The base address of the region where MMIO BARs are assigned when configuring the VGA controller
    DevInfo->MemSizeMax         = 0x08000000;                                               //The maximum amount of memory to allow during BAR assignment.
    DevInfo->IoBase             = 0x2000;                                                   //The base address of the region where IO BARs are assigned when configuring the VGA controller    
    DevInfo->IoSizeMax          = 0x1000;                                                   //The maximum amount of IO to allow during BAR assignment. 
    DevInfo->FontMap            = &mVideoFontMapInfo;                                       //Pointer to a buffer of font map information structures that define the font map(s) to load.
    DevInfo->FontMapCount       = sizeof(mVideoFontMapInfo)/sizeof(FONT_MAP_INFO);;         //Count of entries in the preceding buffer.
    DevInfo->PeiServices        = PeiServices;
    
    //Simics
    if (IsSimicsEnvironment() == TRUE) {
        DevInfo->DevPath            = mSimulatedVideoVidPath;                       //Pointer to a buffer of dev path structure that define the location of the video controller in the PCI topology 
        DevInfo->DevPathEntries     = sizeof(mSimulatedVideoVidPath)/sizeof(DEV_PATH);       //Count of entries in the preceding buffer.  
        DevInfo->RootBus            = mSimulatedVideoVidPath[0].Bus;
    }

    // Initialize VGA Hardware
    Status = VideoInit (*DevInfo); 
    
    if(!EFI_ERROR(Status)) {
        Status = (*PeiServices)->InstallPpi(PeiServices, &gVideoInitDoneSentSignalPpi);
    }
    
    return Status;
}

