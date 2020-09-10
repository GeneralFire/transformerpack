//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

//**********************************************************************
/** @file AmiUsbInterfaceDriver.c
    UsbInterfaceDriver implementation

**/
//**********************************************************************
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/AmiUsbInterfaceLib.h>
#include <Protocol/UsbIo.h>
#include <Protocol/IpmiTransportProtocol.h>
#include <Protocol/DriverBinding.h>

/**
    Test to see if this driver supports ControllerHandle.

    @param This                 Protocol instance pointer
    @param Controller           Handle of device to test
    @param RemainingDevicePath  Optional parameter use to pick a specific child
                                device to start.

    @retval EFI_SUCCESS         This driver supports this device.
    @retval EFI_UNSUPPORTED     This driver does not support this device.

**/
EFI_STATUS
EFIAPI
UsbInterfaceDriverBindingSupported (
  EFI_DRIVER_BINDING_PROTOCOL    *This,
  EFI_HANDLE                     ControllerHandle,
  EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
  )
{	
	
    EFI_STATUS                      Status;
	EFI_USB_IO_PROTOCOL             *UsbIo = NULL; 
 
    Status = IsBmcUsbUsingHandle (ControllerHandle);
    
    if (EFI_ERROR (Status)) {
        return Status;
    }

    // Open the USB protocol 
    Status = gBS->OpenProtocol (
                        ControllerHandle,
                        &gEfiUsbIoProtocolGuid,
                        (VOID **)&UsbIo,
                        This->DriverBindingHandle,
                        ControllerHandle,
                        EFI_OPEN_PROTOCOL_BY_DRIVER );
    
    if (EFI_ERROR (Status)) {
        return Status;
    }
    
    // Close the USB protocol 
    Status = gBS->CloseProtocol (
                        ControllerHandle,
                        &gEfiUsbIoProtocolGuid,
                        This->DriverBindingHandle,
                        ControllerHandle );
    return Status;
}

/**
    Binding protocol function to start the USB driver interface

    @param This                  Protocol instance pointer.
    @param Controller            Handle of device to test
    @param RemainingDevicePath   Optional parameter use to pick a specific child
                                 device to start.

    @retval EFI_SUCCESS          The device was started.
    @retval EFI_UNSUPPORTED      The driver failed to start the device.
    @retval EFI_OUT_OF_RESOURCES Failed to allocate resources.

**/
EFI_STATUS
UsbInterfaceDriverBindingStart(
  IN EFI_DRIVER_BINDING_PROTOCOL        *This,
  IN EFI_HANDLE                         ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL           *RemainingDevicePath OPTIONAL
)
{
    EFI_STATUS                  Status;
    EFI_USB_IO_PROTOCOL         *UsbIo = NULL;
    EFI_IPMI_TRANSPORT          *IpmiTransport = NULL;
    AMI_IPMI_BMC_INSTANCE_DATA  *IpmiInstance;
    
    DEBUG((DEBUG_INFO,"START: UsbInterfaceDriverBindingStart \n"));
    Status = gBS->OpenProtocol (
                          ControllerHandle,
                          &gEfiUsbIoProtocolGuid,
                          (VOID **)&UsbIo,
                          This->DriverBindingHandle,
                          ControllerHandle,
                          EFI_OPEN_PROTOCOL_BY_DRIVER
                          );
    
    if (EFI_ERROR(Status)) {
        return Status;
    }
    
    Status = gBS->LocateProtocol (
                        &gEfiDxeIpmiTransportProtocolGuid,
                        NULL,
                        (VOID **)&IpmiTransport );
    
    if (EFI_ERROR(Status)) {
        return Status;
    }
    
    IpmiInstance = AMI_IPMI_INSTANCE_FROM_EFI_SM_IPMI_BMC_THIS (IpmiTransport);
    if (IpmiInstance->Signature  != AMI_SM_IPMI_BMC_SIGNATURE)
        return EFI_UNSUPPORTED; 
    
    if (IpmiInstance->Interface.Usb == NULL) {
        IpmiInstance->Interface.Usb = (AMI_USB_SYSTEM_INTERFACE*)AllocateRuntimeZeroPool (sizeof (AMI_USB_SYSTEM_INTERFACE)); 
        if (IpmiInstance->Interface.Usb == NULL) {
            DEBUG ((DEBUG_ERROR, "%a - Failed to allocate memory for Interface->Usb! \n", __FUNCTION__));
            return EFI_OUT_OF_RESOURCES;
        }
    }

    if ((IpmiInstance->Interface.Usb->UsbIoHandle == NULL) || 
        (IpmiInstance->Interface.Usb->InterfaceState != AmiIpmiInterfaceInitialized)) {
        DEBUG((DEBUG_INFO, "Reconnect \n" ));
        IpmiInstance->Interface.Usb->BOTCommandTag = 0; //Initial value is zero.
        IpmiInstance->Interface.Usb->UsbInterfaceApiPtr = (UINTN)UsbIo;
        // Get Bulk In and Out end point addresses and fill in private data.
        Status = GetBulkEndPoints (&IpmiInstance->Interface);
        
        if (EFI_ERROR(Status)) {
            DEBUG((DEBUG_ERROR, "Failed to get Bulk In and Out end point addresses! \n"));
            return Status;
        }
        
        if (IpmiInstance->Interface.Usb->UsbCommBufferPtr == 0) {
            IpmiInstance->Interface.Usb->UsbCommBufferPtr = (UINTN)(UINTN *)AllocateRuntimeZeroPool (AMI_IPMI_MAX_USB_CMD_DATA_SIZE);
            if (!IpmiInstance->Interface.Usb->UsbCommBufferPtr) {
                DEBUG ((DEBUG_ERROR, "%a - Failed to allocate memory for Interface->Usb->UsbCommBufferPtr! \n", __FUNCTION__));
                return EFI_OUT_OF_RESOURCES;
            }
        }
        
        // Assign Timeout value to USB structure member.
        IpmiInstance->Interface.Usb->BulkTransferTimeOut = PcdGet16 (AmiPcdBulkDataTransferTimeout);
        IpmiInstance->Interface.Usb->InterfaceState = AmiIpmiInterfaceInitialized;     
    } 
    
    DEBUG((DEBUG_INFO,"END: UsbInterfaceDriverBindingStart \n"));
    return Status;   
}

/**
    Binding protocol function to stop the USB driver interface

    @param This                   Protocol instance pointer.
    @param DeviceHandle           Handle of device to stop driver on
    @param NumberOfChildren       Number of Children in the ChildHandleBuffer
    @param ChildHandleBuffer      List of handles for the children we need to stop.

    @retval EFI_SUCCESS           The device was stopped.
    @retval EFI_UNSUPPORTED       This driver was not removed from this device.

**/
EFI_STATUS
UsbInterfaceDriverBindingStop(
  IN EFI_DRIVER_BINDING_PROTOCOL *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN UINTN                        NumberOfChildren,
  IN EFI_HANDLE                   *ChildHandleBuffer
)
{   
    EFI_STATUS                  Status;
    EFI_IPMI_TRANSPORT          *IpmiTransport = NULL;
    AMI_IPMI_BMC_INSTANCE_DATA  *IpmiInstance;

    DEBUG((DEBUG_INFO,"START: UsbInterfaceDriverBindingStop\n"));
    
    Status = gBS->LocateProtocol (
                        &gEfiDxeIpmiTransportProtocolGuid,
                        NULL,
                        (VOID **)&IpmiTransport );
    
    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_ERROR,"ERROR : USB locate protocol failed \n"));
        return EFI_UNSUPPORTED;
    }
    
    IpmiInstance = AMI_IPMI_INSTANCE_FROM_EFI_SM_IPMI_BMC_THIS (IpmiTransport);
    IpmiInstance->Interface.Usb->UsbIoHandle = NULL;
    IpmiInstance->Interface.Usb->InterfaceState = AmiIpmiInterfaceNotReady;

    if ((UINTN *)IpmiInstance->Interface.Usb->UsbCommBufferPtr != NULL) {
        FreePool ((UINTN *)IpmiInstance->Interface.Usb->UsbCommBufferPtr);
        IpmiInstance->Interface.Usb->UsbCommBufferPtr = 0;
    }
    
    // Close the USB protocol 
    Status = gBS->CloseProtocol (
                         ControllerHandle,
                         &gEfiUsbIoProtocolGuid,
                         This->DriverBindingHandle,
                         ControllerHandle);
                        
    
    DEBUG((DEBUG_INFO,"END: UsbInterfaceDriverBindingStop %r\n", Status));
    return Status;
}

/// Driver Binding Protocol instance
EFI_DRIVER_BINDING_PROTOCOL gUsbInterfaceDriverBinding = { 
        UsbInterfaceDriverBindingSupported,
        UsbInterfaceDriverBindingStart,
        UsbInterfaceDriverBindingStop,
        USB_INTERFACE_DRIVER_VER, 
        NULL, 
        NULL 
};

/**
    This is the declaration of an EFI image entry point. This entry point is
    the same for UEFI Applications, UEFI OS Loaders, and UEFI Drivers including
    both device drivers and bus drivers.

    @param  ImageHandle           The firmware allocated handle for the UEFI image.
    @param  SystemTable           A pointer to the EFI System Table.

    @retval EFI_SUCCESS           The operation completed successfully.
    @retval Others                An unexpected error occurred.

 **/
EFI_STATUS UsbInterfaceDriverEntryPoint(
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
)
{
    EFI_STATUS Status;
    
    DEBUG((DEBUG_INFO,"START: UsbInterfaceDriver EntryPoint \n"));
    //
    // Install UEFI Driver Model protocol(s).
    //
    Status = gBS->InstallMultipleProtocolInterfaces(
                                              &gUsbInterfaceDriverBinding.DriverBindingHandle,
                                              &gEfiDriverBindingProtocolGuid, 
                                              &gUsbInterfaceDriverBinding,
                                              NULL
                                              );
    
    DEBUG((DEBUG_INFO,"END: UsbInterfaceDriver EntryPoint : %r \n", Status));
    return Status;
}
