//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
// @file PlatformVideoCtrlReset.c
//  LIB driver for Resetting the Video Controller Programming done in the PEI 
//*********************************************************************

#include <PiDxe.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/MmPciBaseLib.h>
#include <Library/IoLib.h>
#include <Protocol/AmiDxeTextOut.h>
#include <IndustryStandard/Pci.h>


typedef struct {
    UINT8   Bus;
    UINT8   Dev;
    UINT8   Func;
} DEV_PATH;


DEV_PATH mVideoVidPath [] = {{0x0,0xF,0x0},     //PCIe-PCI bridge
                             {0x2,0x00,0x0},     //Video Bridge
                             {0x3,0x00,0x0}      //Video Controller
                            };

extern BOOLEAN             gVideoDeviceInUse;

/**
    DisableVgaDecode- Disables legacy VGA decode down to the video controller

    @param VOID

    @return VOID
**/
VOID DisableVgaDecode( ) 
{
    UINT8               Bus;
    UINT8               Dev;
    UINT8               Func;
    UINT8               DevPathIdx;
    UINT16              Data16;
    DEV_PATH            *DevPath;
    UINT8               DevPathEntries;
    UINT32              PciExpressCfgBase;

    DevPath = mVideoVidPath;  
    DevPathEntries = sizeof(mVideoVidPath)/sizeof(DEV_PATH);             
    PciExpressCfgBase  = (UINT32) PcdGet64(PcdPciExpressBaseAddress);;               
        
    //Root Bus is 0
    Bus = 0;
    
    // Iterate through all the bridges in the dev path and disable VGA decode.
    for (DevPathIdx = 0; DevPathIdx < DevPathEntries-1; DevPathIdx++) {
        Dev = DevPath[DevPathIdx].Dev;
        Func = DevPath[DevPathIdx].Func;
        
        //Disable VGA decode
        Data16 = MmioRead16 (MmPciBase(Bus, Dev, Func) + PCI_BRIDGE_CONTROL_REGISTER_OFFSET);
        Data16 &= ~EFI_PCI_BRIDGE_CONTROL_VGA;
        MmioWrite16 ((MmPciBase(Bus, Dev, Func) + PCI_BRIDGE_CONTROL_REGISTER_OFFSET), Data16);
        Bus++;
    } 

    return;
}

/**
    Notification function for Pci Protocol 
    This function stops the video protocol after PCI enumeration 

    @param Event - Event which caused this handler
    @param Context - Context passed during Event Handler registration

    @return VOID
**/
VOID
PciProtocolCallBack (
  IN EFI_EVENT        Event,
  IN VOID             *TextOutContext 
)
{

    DEBUG((EFI_D_INFO,"%a Entry... \n",__FUNCTION__));

    //Disable VGA Decode in pci bridge devices
    DisableVgaDecode();   
    
    gVideoDeviceInUse = TRUE;   
    
    // Kill the Event
    gBS->CloseEvent(Event);
    return;
}

/**
    Install the Callback to Reset the Video Controller Programming done in the DXE Phase. 

    @param   None 

    @retval  EFI_NOT_FOUND
**/
EFI_STATUS 
AmiVideoControllerReset(
  VOID
  )
{
    EFI_STATUS              Status;
    EFI_EVENT               PciProtocolEvent;
    VOID                    *PciProtocolNotifyReg;

    // Install the Callback function to Reset the Video Controller programming. 
    // Porting Required.
    DEBUG((EFI_D_INFO,"%a Platform Controller Reset...\n", __FUNCTION__));
    
    //Create notification for PciEnumerationComplete
    Status = gBS->CreateEvent (
                        EVT_NOTIFY_SIGNAL, 
                        TPL_CALLBACK,
                        PciProtocolCallBack, 
                        NULL, 
                        &PciProtocolEvent);
    ASSERT_EFI_ERROR(Status);
    
    if (!EFI_ERROR(Status)) {
        Status = gBS->RegisterProtocolNotify (
                                    &gAmiBdsConnectDriversProtocolGuid,
                                    PciProtocolEvent, 
                                    &PciProtocolNotifyReg);
    
    }
    return Status;
}
