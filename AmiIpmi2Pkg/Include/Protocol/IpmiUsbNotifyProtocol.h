//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file IpmiUsbNotifyProtocol.h
    IPMI USB Notify Protocol definitions

**/

#ifndef _EFI_IPMI_USB_NOTIFUY_PROTOCOL_H
#define _EFI_IPMI_USB_NOTIFY_PROTOCOL_H


// {0588edc8-5205-48b8-b2a9-0570323a4cc6} 
#define AMI_IPMI_USB_EXIT_BOOT_NOTIFY_PROTOCOL_GUID \
  { \
    0x0588edc8, 0x5205, 0x48b8, {0xb2, 0xa9, 0x5, 0x70, 0x32, 0x3a, 0x4c, 0xc6} \
  }

typedef struct _AMI_IPMI_USB_EXIT_BOOT_NOTIFY_PROTOCOL   AMI_IPMI_USB_EXIT_BOOT_NOTIFY_PROTOCOL;

typedef
VOID
(EFIAPI *AMI_IPMI_USB_EXIT_BOOT_NOTIFICATION_FUNCTION) (
    IN EFI_HANDLE                           Handle
);

typedef
EFI_STATUS
(EFIAPI *AMI_IPMI_USB_EXIT_BOOT_REGISTER_FUNCTION) (
    IN EFI_HANDLE                                       Handle,     
    IN AMI_IPMI_USB_EXIT_BOOT_NOTIFICATION_FUNCTION     NotifyFunction
);

struct _AMI_IPMI_USB_EXIT_BOOT_NOTIFY_PROTOCOL {
    AMI_IPMI_USB_EXIT_BOOT_REGISTER_FUNCTION      Register;
    AMI_IPMI_USB_EXIT_BOOT_REGISTER_FUNCTION      UnRegister;
 };

extern EFI_GUID gAmiIpmiUsbExitBootNotifyProtocolGuid;

typedef struct {
    LIST_ENTRY                                      Link;
    EFI_HANDLE                                      Handle;
    AMI_IPMI_USB_EXIT_BOOT_NOTIFICATION_FUNCTION    NotifyFunction;
} AMI_IPMI_USB_EXIT_BOOT_FUNCTIONS_LIST;

EFI_STATUS
Register (
    IN EFI_HANDLE                                       Handle,
    IN AMI_IPMI_USB_EXIT_BOOT_NOTIFICATION_FUNCTION     NotifyFunction
);

EFI_STATUS
UnRegister (
   IN EFI_HANDLE                                        Handle,
   IN AMI_IPMI_USB_EXIT_BOOT_NOTIFICATION_FUNCTION      NotifyFunction
);

#endif

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
