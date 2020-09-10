//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

/** @file IpmiBootBoard.c
    This file validates Remotely connected device details
    with boot option to select proper remote device. Remote devices details
    needs to be ported based on device type.

*/

//---------------------------------------------------------------------------

#include "IpmiBoot.h"

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

extern AMI_IPMI_BOOT_DEVICE_SELECTOR    gIpmiForceBootDevice;

//---------------------------------------------------------------------------

/*
#define IS_PCI_DEVICE_PATH (Dp)     ((Dp)->Header.Type==HARDWARE_DEVICE_PATH && \
                                    (Dp)->Header.SubType==HW_PCI_DP)
#define IS_USB_DEVICE_PATH (Dp)     ((Dp)->Header.Type==MESSAGING_DEVICE_PATH && \
                                    (Dp)->Header.SubType==MSG_USB_DP)
*/
/** @internal
    Get the device path protocol interface installed on a specified handle.

    @param[in] Handle   Handle to get device path.

    @return EFI_DEVICE_PATH_PROTOCOL*   Returns device path installed on
                                        given handle.

**/
/*EFI_DEVICE_PATH_PROTOCOL *
EfiDevicePathFromHandle (
  IN EFI_HANDLE  Handle )
{
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath = NULL;

  gBS->HandleProtocol (
        Handle,
        &gEfiDevicePathProtocolGuid,
        (VOID *) &DevicePath );
  return DevicePath;
}*/

/** @internal
    Compares remote device details with Boot option details.

    @param[in] BootOption   The option that should be validated with
                            remote device details.

    @return BOOLEAN - compares the two strings and returns the difference

    Note: This is a porting function. Based on the Remote device details,
          this function definition can be changed to verify the boot option
          details with remote device.
**/
/*BOOLEAN
CompareRemoteDeviceDetails (
  IN BOOT_OPTION    *BootOption )
{
    ACPI_HID_DEVICE_PATH        *AcpiDpNode;
    PCI_DEVICE_PATH             *PciDpNode;
    USB_DEVICE_PATH             *UsbDpNode;
    EFI_DEVICE_PATH_PROTOCOL    *OptionDp;

    OptionDp = EfiDevicePathFromHandle (BootOption->DeviceHandle);*/

    /* Device path: ACPI(PNP0A03,0)/PCI(1A|0)/USB(1,0)/USB(3,0)
       Check for ACPI device path: ACPI(PNP0A03,0).*/
    /*AcpiDpNode = (ACPI_HID_DEVICE_PATH  *) OptionDp;
    if (!((IS_ACPI_DEVICE_PATH(AcpiDpNode)) && \
            (AcpiDpNode->HID == EISA_PNP_ID(0x0A03)) &&\
            (AcpiDpNode->UID == 0x00) \
           )) {
        return FALSE;
    }

    // Check for PCI device path: PCI(1A|0).
    OptionDp = NEXT_NODE(OptionDp);
    PciDpNode = (PCI_DEVICE_PATH *) OptionDp;
    if (!((IS_PCI_DEVICE_PATH(PciDpNode)) && \
            (PciDpNode->Function == 0x00) &&\
            (PciDpNode->Device == 0x1A) \
           )) {
        return FALSE;
    }

    // Check for USB device path: USB(1,0).
    OptionDp = NEXT_NODE(OptionDp);
    UsbDpNode = (USB_DEVICE_PATH *) OptionDp;
    if (!((IS_USB_DEVICE_PATH(UsbDpNode)) && \
            (UsbDpNode->ParentPortNumber == 0x01) &&\
            (UsbDpNode->InterfaceNumber == 0x00) \
           )) {
        return FALSE;
    }

    // Check for USB device path: USB(3,0).
    OptionDp = NEXT_NODE(OptionDp);
    UsbDpNode = (USB_DEVICE_PATH *) OptionDp;
    if (!((IS_USB_DEVICE_PATH(UsbDpNode)) && \
            (UsbDpNode->ParentPortNumber == 0x03) &&\
            (UsbDpNode->InterfaceNumber == 0x00) \
           )) {
        return FALSE;
    }

    return TRUE;
}*/

/** @internal
    Boot to remote device if proper remote device is available.

    @param[in] IpmiForceBootDevice  Boot device.

    @return EFI_STATUS  Return status of remote boot.

**/
EFI_STATUS
RemoteDeviceBoot (
  IN AMI_IPMI_BOOT_DEVICE_SELECTOR  IpmiForceBootDevice )
{
    EFI_STATUS  Status = EFI_SUCCESS;

    switch (IpmiForceBootDevice) {
        case AmiIpmiForceBootRemoteFloppy:
         /* Sample Code 
          * Status = SetBootOptionToHighestPriority (
                                BBS_FLOPPY,
                                ACPI_DEVICE_PATH,
                                ACPI_DP ); */
            break;

        case AmiIpmiForceBootRemoteHardDrive:
             Status = SetBootOptionToHighestPriority (
                                 BBS_BEV_DEVICE,
                                 MESSAGING_DEVICE_PATH,
                                 MSG_ISCSI_DP );
        /* Sample code 
         * Status = SetBootOptionToHighestPriority (
                                BBS_HARDDISK,
                                MEDIA_DEVICE_PATH,
                                MEDIA_HARDDRIVE_DP ); */
            break;

        case AmiIpmiForceBootRemoteCdDvd:
        /* Sample Code 
         * Status = SetBootOptionToHighestPriority (
                                BBS_CDROM,
                                MEDIA_DEVICE_PATH,
                                MEDIA_CDROM_DP ); */
            break;

        default:
            break;

    }
    return Status;
}

/** @internal
    Validates whether to boot to normal HDD for ForceBootHardDriveSafeMode
    and ForceBootDiagnosticPartition also.

    @param[in] UefiBoot     TRUE for UEFI boot type, FALSE for Legacy boot.
    @param[in] BbsDevType   BBS device Type of the Remote device.
    @param[in] DpType       Device path type of the Remote device.
    @param[in] DpSubType    Device path sub type of the Remote device.
    @param[in] Option       The option that should be validated with 
                            remote device details.

    @retval TRUE    Return TRUE if not to boot to normal HDD.
    @retval FALSE   Returns FALSE if boot to normal HDD.

**/
BOOLEAN
BootToDiagnosticPartitionOrHddRequestSafeMode (
  IN BOOLEAN        UefiBoot,
  IN UINT16         BbsDevType,
  IN UINT16         DpType,
  IN UINT16         DpSubType,
  IN BOOT_OPTION    *BootOption )
{
    // gIpmiForceBootDevice tells the type of user selected boot device.
    switch (gIpmiForceBootDevice) {
        case AmiIpmiForceBootHardDriveSafeMode:
            /* Write the code to validate boot option for
               ForceBootHardDriveSafeMode and return appropriate value.*/
            break;
        case AmiIpmiForceBootDiagnosticPartition:
            /* Write the code to validate boot option for
               ForceBootDiagnosticPartition and return appropriate value.*/
            break;
        default:
            break;
    }
    /* Always return False as BIOS doesn't know the difference between ForceBootHardDrive,
       ForceBootHardDriveSafeMode,ForceBootDiagnosticPartition.*/
    return FALSE;
}

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
