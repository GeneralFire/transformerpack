//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file IpmiBoot.h
    Header file for IpmiBoot Module.

*/

#ifndef _IPMI_BOOT_H_
#define _IPMI_BOOT_H_

//---------------------------------------------------------------------------

#include <Token.h>
#include <Library/UefiLib.h>
#include <IndustryStandard/Pci22.h>
#include <BootOptions.h>
#include <AmiIpmiNetFnChassisDefinitions.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/AmiIpmiBootLib.h>
#include <Protocol/DevicePath.h>
#include <Protocol/LegacyBios.h>
#include <Protocol/IpmiTransportProtocol.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#ifndef RfBootOptions_SUPPORT
#define REDFISH_BOOT_SUPPORT  0
#else
#define REDFISH_BOOT_SUPPORT  RfBootOptions_SUPPORT
#endif

#define MAXIMUM_BOOT_OPTION_NUMBER          0xFFFF

#define VAR_FLAG                            EFI_VARIABLE_BOOTSERVICE_ACCESS | \
                                            EFI_VARIABLE_NON_VOLATILE

// BAID Device Types macros.
#define BAID_TYPE_HDD                       1
#define BAID_TYPE_RMD_HDD                   2
#define BAID_TYPE_CDROM                     3
#define BAID_TYPE_RMD_FDD                   4
#define BAID_TYPE_FDD                       5

#define IPMI_BOOT_GUID  \
    { 0x842680f2, 0x1a9c, 0x48e6, { 0xa4, 0x33, 0xbe, 0x9a, 0xcb, 0xd, 0xd4, 0x38 } }

#define HIGHEST_BOOT_OPTION_PRIORITY            0x00
#define HIGHEST_BOOT_OPTION_TAG                 0x00
#define INTERNAL_EXTERNAL_DEVICE_INSTANCE_MASK  0x10
#define DEVICE_INSTANCE_SELECTION_MASK          0x0F
#define BOOT_OPTION_NUMBER_SIZE                 sizeof(UINT16)

#define INVALID_DEVICE                          0xFF
#define NON_USB_COMPATIBLE_DEVICE               0x00
#define USB_COMPATIBLE_DEVICE                   0x01

#define IS_USB_DEVICE_CLASS_CODE(Option) ((Option )? \
                                            ((Option)->BbsEntry->Class == PCI_CLASS_SERIAL) && \
                                            ((Option)->BbsEntry->SubClass == PCI_CLASS_SERIAL_USB): 0)
#define IS_USB_COMPATIBLE_DEVICE(Option, DevType) \
    ((Option) ? ((((Option)->BbsEntry->InitPerReserved) >> 24)& 0XFF) == DevType: 0)

#define IS_ACPI_DEVICE_PATH(Dp)  ( (Dp) ? (Dp)->Header.Type==ACPI_DEVICE_PATH && \
                                        (Dp)->Header.SubType==ACPI_DP && \
                                        (Dp)->HID==EISA_PNP_ID(0x0604): 0)
#define IS_BBS_DEVICE_PATH(Dp, BbsDevType)  \
                                        ((Dp) ? (Dp)->Header.Type==BBS_DEVICE_PATH && \
                                        (Dp)->Header.SubType==BBS_BBS_DP && \
                                        (Dp)->DeviceType==BbsDevType: 0)

extern EFI_GUID gEfiIpmiBootGuid;

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

/** @internal
    Set the highest priority to the requested SATA compatible device boot
    option.

    @param[in] BbsDevType   BBS Device Type of the boot option.
    @param[in] DpType       Device path type of the boot option.
    @param[in] DpSubType    Device path sub type of boot option.

    @retval EFI_SUCCESS     Requested boot option is found and set as high
                            priority.
    @retval EFI_NOT_FOUND   Proper boot option is not found to set priority.

**/
EFI_STATUS 
SetBootOptionToHighestPriority (
  IN UINT16     BbsDevType,
  IN UINT16     DpType,
  IN UINT16     DpSubType
);

/** @internal
    Boot to remote device if proper remote device is available.

    @param[in] IpmiForceBootDevice  Boot device.

    @return EFI_STATUS  Return status of remote boot.

**/
EFI_STATUS
RemoteDeviceBoot (
  IN AMI_IPMI_BOOT_DEVICE_SELECTOR  IpmiForceBootDevice
);

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
  IN BOOT_OPTION    *BootOption
);

//---------------------------------------------------------------------------

#endif


