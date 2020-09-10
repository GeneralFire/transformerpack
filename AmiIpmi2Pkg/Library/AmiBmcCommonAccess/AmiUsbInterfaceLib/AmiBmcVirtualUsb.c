//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file AmiBmcVirtualUsb.c
    This file provides Power Consumption Support feature in BMC
    over USB interface (enable or disable BMC USB interface through KCS
    functions provided in this file).

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <AmiIpmiInitHooks.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/AmiUsbInterfaceLib.h>
#include <Protocol/IpmiTransportProtocol.h>
#include <Protocol/IpmiUsbNotifyProtocol.h>
#include <Library/MemoryAllocationLib.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

// NetFn.
#define EFI_APP_ENABLE_MESSAGE_CHANNEL_RECEIVE          0x32

// Virtual USB devices State.
#define VIRTUAL_USB_DEVICES_ENABLED                     0x00
#define VIRTUAL_USB_DEVICES_DISABLED                    0x01
#define COMMAND_SUCCESS                                 0x01

// Command to set power savings state.
#define BMC_OEM_COM_FOR_POWER_SAVING_SEND_COMMAND       0xAA

// Command to get power savings state.
#define BMC_OEM_COM_FOR_POWER_SAVING_RECEIVE_COMMAND    0xAB

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

// gResponseData[0]-Completion code, gResponseData[1]-Virtual device state.
UINT8               gResponseData[2];
EFI_IPMI_TRANSPORT  *gIpmiTransport = NULL;
LIST_ENTRY          gAmiIpmiUsbNotifyFunctionsList;
BOOLEAN             gInitHead = FALSE;

// Protocol Initialization
AMI_IPMI_USB_EXIT_BOOT_NOTIFY_PROTOCOL AmiIpmiUsbExitBootNotifyProtocol = {
    Register,
    UnRegister
};

extern AMI_BMC_USB_ENABLE_AND_DISABLE BMC_USB_HOOK_LIST \
                                        EndOfAmiBmcUsbInterfaceHookListFunctions;
AMI_BMC_USB_ENABLE_AND_DISABLE  *gBmcUsbInterfaceHookList[]  = { \
                                    BMC_USB_HOOK_LIST \
                                    NULL \
                                    };

//---------------------------------------------------------------------------

/**
    Registers the callback function

    @param[in]  Handle            Handle to which notification function need to be registered
    @param[in]  NotifyFunction    Notification function to be notified for the input handle
  
    @return EFI_STATUS
    @retval EFI_INVALID_PARAMETER If NotifyFunction or Handle parameter is NULL
    @retval EFI_UNSUPPORTED       If the input parameter is already registered
    @retval EFI_NOT_FOUND         Resource not avilable for the New entry 
    @retval EFI_SUCCESS           The function is registered successfully.
**/

EFI_STATUS
Register (
    IN EFI_HANDLE                                       Handle,
    IN AMI_IPMI_USB_EXIT_BOOT_NOTIFICATION_FUNCTION     NotifyFunction
    )
{
    AMI_IPMI_USB_EXIT_BOOT_FUNCTIONS_LIST *NewEntry = NULL;
    AMI_IPMI_USB_EXIT_BOOT_FUNCTIONS_LIST *NextLink = NULL;

    if (NotifyFunction == NULL || Handle == NULL ) {
        return EFI_INVALID_PARAMETER; 
    }

    // Verify if the Entry already Present
    for (NextLink = (AMI_IPMI_USB_EXIT_BOOT_FUNCTIONS_LIST*)GetFirstNode(&gAmiIpmiUsbNotifyFunctionsList);
            !IsNull(&gAmiIpmiUsbNotifyFunctionsList, &NextLink->Link); 
            NextLink = (AMI_IPMI_USB_EXIT_BOOT_FUNCTIONS_LIST*)GetNextNode(&gAmiIpmiUsbNotifyFunctionsList, &NextLink->Link)
        ){
        if ((NextLink->Handle == Handle) && (NextLink->NotifyFunction == NotifyFunction)) {
            // Entry Already present.
            return EFI_UNSUPPORTED;
        }
    }

    // Allocate the Buffer and Add the New Entry
    NewEntry = AllocateZeroPool (sizeof (AMI_IPMI_USB_EXIT_BOOT_FUNCTIONS_LIST));

    if (NewEntry == NULL) {
        return EFI_OUT_OF_RESOURCES;
    }

    NewEntry->Handle = Handle;
    NewEntry->NotifyFunction = NotifyFunction;
    InsertTailList((LIST_ENTRY*)&gAmiIpmiUsbNotifyFunctionsList, (LIST_ENTRY*)&(NewEntry->Link));

    return EFI_SUCCESS;
}

/**
    Unregistered the callback function .

    @param[in]  Handle            Handle to which notification function is registered
    @param[in]  NotifyFunction    Notification function which is registered already
  
    @return EFI_STATUS
    @retval EFI_INVALID_PARAMETER If NotifyFunction or Handle parameter is NULL
    @retval EFI_NOT_FOUND         The input parameter is not registered already
    @retval EFI_SUCCESS           The function is unregistered successfully.
**/

EFI_STATUS
UnRegister (
    IN EFI_HANDLE                                        Handle,
    IN AMI_IPMI_USB_EXIT_BOOT_NOTIFICATION_FUNCTION      NotifyFunction
    )
{
    AMI_IPMI_USB_EXIT_BOOT_FUNCTIONS_LIST *NextLink = NULL;

    if (NotifyFunction == NULL || Handle == NULL ) {
        return EFI_INVALID_PARAMETER; 
    } 

    for (NextLink = (AMI_IPMI_USB_EXIT_BOOT_FUNCTIONS_LIST*)GetFirstNode(&gAmiIpmiUsbNotifyFunctionsList);
            !IsNull(&gAmiIpmiUsbNotifyFunctionsList, &NextLink->Link); 
            NextLink = (AMI_IPMI_USB_EXIT_BOOT_FUNCTIONS_LIST*)GetNextNode(&gAmiIpmiUsbNotifyFunctionsList, &NextLink->Link)
        ){
        if ((NextLink->Handle == Handle) && (NextLink->NotifyFunction == NotifyFunction)) {
            // Entry Found and Remove from the Linked List
            RemoveEntryList((LIST_ENTRY*)&(NextLink->Link));
            gBS->FreePool(NextLink);
            return EFI_SUCCESS;
        }
    }
    // Unable to find the entry in the Linked List for the given input data
    return EFI_NOT_FOUND;
}

/**
    Invokes the functions registered.

    @param[in]   handle

**/

VOID
NotifyRegisteredFunctions ()
{
    AMI_IPMI_USB_EXIT_BOOT_FUNCTIONS_LIST   *NextLink = NULL;
    AMI_IPMI_USB_EXIT_BOOT_FUNCTIONS_LIST   *TmpLink = NULL;

    for (NextLink = (AMI_IPMI_USB_EXIT_BOOT_FUNCTIONS_LIST*)GetFirstNode(&gAmiIpmiUsbNotifyFunctionsList);
        !IsNull(&gAmiIpmiUsbNotifyFunctionsList, &NextLink->Link); 
        NextLink = TmpLink ) {

        TmpLink = (AMI_IPMI_USB_EXIT_BOOT_FUNCTIONS_LIST*)GetNextNode(&gAmiIpmiUsbNotifyFunctionsList, &NextLink->Link);
        NextLink->NotifyFunction (NextLink->Handle);

    }
}

/**
    Initialize Linked List and Install Notify Protocol 

    @param[in]   Handle 

**/

EFI_STATUS
InstallIpmiUsbExitBootNotifyProtocol ()
{

    EFI_STATUS      Status;
    EFI_HANDLE      Handle = NULL;

    // Initialize the Linked List only one time. 
    if (FALSE == gInitHead) {
        InitializeListHead(&gAmiIpmiUsbNotifyFunctionsList);
        gInitHead = TRUE;
    }

    Status = gBS->InstallProtocolInterface (
                            &Handle,
                            &gAmiIpmiUsbExitBootNotifyProtocolGuid,
                            EFI_NATIVE_INTERFACE,
                            &AmiIpmiUsbExitBootNotifyProtocol );
    DEBUG((DEBUG_INFO, " AmiIpmiUsbExitBootNotifyProtocol Installed- %r \n", Status ));
    return Status;
}


/**
    Notify function to call the Bmc Usb interface hooks.

    @param[in] Event    Callback event.
    @param[in] Context  Pointer to calling context.

    @return VOID    Nothing.

**/
VOID
EFIAPI
BmcUsbInterfaceHook (
  IN EFI_EVENT  Event,
  IN VOID       *Context )
{
    UINTN   Index;

    for (Index = 0; gBmcUsbInterfaceHookList[Index]; Index++) {
        gBmcUsbInterfaceHookList[Index] ();
    }
    gBS->CloseEvent (Event);
}

/** @internal
    This function checks the status of BMC Virtual USB devices and updates
    the below device state.
        00h if Virtual USB devices are enabled.
        01h if Virtual USB devices are disabled.

    @param[out] VirtualUsbDevicesState  Address of Virtual Usb Device State
                                        to get the Status.

    @retval EFI_SUCCESS         Status updated successfully.
    @retval EFI_DEVICE_ERROR    Returns when command is not success or
                                erroneous response data.

**/
EFI_STATUS
BmcUsbStatus (
  OUT UINT8 *VirtualUsbDevicesState )
{
    EFI_STATUS  Status;
    UINT8       ResponseDataSize = sizeof(gResponseData);

    Status = gIpmiTransport->SendIpmiCommand (
                               gIpmiTransport,
                               EFI_APP_ENABLE_MESSAGE_CHANNEL_RECEIVE,
                               AMI_BMC_LUN,
                               BMC_OEM_COM_FOR_POWER_SAVING_RECEIVE_COMMAND,
                               NULL,
                               0,
                               gResponseData,
                               &ResponseDataSize );

    DEBUG((DEBUG_INFO, "Send Ipmi Command in USB Status Function :: Status - %r and Response data :: %x.\n", Status, gResponseData[1]));

    if (!EFI_ERROR(Status)) {  // Returning response data.
        if ((gResponseData[1] == VIRTUAL_USB_DEVICES_ENABLED) || \
            (gResponseData[1] == VIRTUAL_USB_DEVICES_DISABLED)) {
            *VirtualUsbDevicesState = gResponseData[1];
            return EFI_SUCCESS;
        }
    }

    // Command is not success or erroneous response data.
    return EFI_DEVICE_ERROR;
}

/**
    Notify function to disable Bmc Virtual Usb devices if not disabled,
    else exits.

    @param[in] Event    Callback event.
    @param[in] Context  Pointer to calling context.

    @return VOID    Nothing.

**/
VOID
EFIAPI
DisableUsbInterface (
  IN EFI_EVENT  Event,
  IN VOID      *Context )
{
    EFI_STATUS  Status;
    UINT8       UsbDisableCommandData = 1; // 1 means disable BMC USB Interface.
    UINT8       ResponseDataSize;

    // Call the registered function 
    NotifyRegisteredFunctions();

    ResponseDataSize = sizeof(gResponseData);
    Status = gIpmiTransport->SendIpmiCommand (
                               gIpmiTransport,
                               EFI_APP_ENABLE_MESSAGE_CHANNEL_RECEIVE,
                               AMI_BMC_LUN,
                               BMC_OEM_COM_FOR_POWER_SAVING_SEND_COMMAND,
                               &UsbDisableCommandData,
                               sizeof(UsbDisableCommandData),
                               gResponseData,
                               &ResponseDataSize );

    DEBUG((DEBUG_INFO, "SendIpmiCommand in Disable Function :: Status- %r and ResponseData[0] :: %x ResponseData[1] :: %x\n", Status, gResponseData[0], gResponseData[1]));
}

/** @internal
    This function enables Bmc Virtual Usb devices if not enabled.
    If enabled, registers notify function for exit boot services
    and legacy boot in order to disable usb interface.

    @param  VOID    Nothing.

    @retval EFI_SUCCESS Enabled usb interface and registered notify events.
    @retval Others      Error status returned from functions used.

**/
EFI_STATUS
AmiBmcUsbInterface (
  VOID )
{
    EFI_STATUS  Status;
    UINT8       VirtualUsbDevicesState;
    UINT8       UsbEnableCommandData = 0;  // 0 means enable BMC USB Interface.
    UINT8       ResponseDataSize;
    EFI_EVENT   ExitBootServicesEvent;
    EFI_EVENT   LegacyBootEvent;

    Status = gBS->LocateProtocol (
                             &gEfiDxeIpmiTransportProtocolGuid,
                             NULL,
                             (VOID **)&gIpmiTransport );

    DEBUG((DEBUG_INFO, "LocateProtocol In Enable Function :: Status - %r\n", Status));

    if (Status == EFI_SUCCESS) {
        // Checking BMC USB Status and getting the Response Data 
        Status = BmcUsbStatus (&VirtualUsbDevicesState);

        DEBUG((DEBUG_INFO, "BmcUsbStatus - %r VirtualUsbDevicesState:%x\n", Status ,VirtualUsbDevicesState ));

        if (Status == EFI_SUCCESS) {
            // If Virtual USB devices are Disabled, then enable them.
            if (VirtualUsbDevicesState == VIRTUAL_USB_DEVICES_DISABLED) {
                ResponseDataSize = sizeof(gResponseData);
                Status = gIpmiTransport->SendIpmiCommand (
                                            gIpmiTransport,
                                            EFI_APP_ENABLE_MESSAGE_CHANNEL_RECEIVE,
                                            AMI_BMC_LUN,
                                            BMC_OEM_COM_FOR_POWER_SAVING_SEND_COMMAND,
                                            &UsbEnableCommandData,
                                            sizeof (UsbEnableCommandData),
                                            gResponseData,
                                            &ResponseDataSize );

                DEBUG((DEBUG_INFO, "SendIpmiCommand in Enable Function :: Status- %r  and ResponseData :: %x.\n", Status, gResponseData[1]));
            }
            // Install protocol to give control to registered function at exit boot service event [at DisableUsbInterface()]
            InstallIpmiUsbExitBootNotifyProtocol();
               
           // To disable BMC Virtual USB devices at exit boot.
           Status = gBS->CreateEvent (
                               EVT_SIGNAL_EXIT_BOOT_SERVICES,
                               TPL_CALLBACK,
                               DisableUsbInterface,
                               NULL,
                               &ExitBootServicesEvent );
           ASSERT_EFI_ERROR (Status);

           Status = EfiCreateEventLegacyBootEx (
                               TPL_CALLBACK,
                               DisableUsbInterface,
                               NULL,
                               &LegacyBootEvent );
           ASSERT_EFI_ERROR (Status);
        }
    }
    return Status;
}
