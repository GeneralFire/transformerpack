/** @file
  LegacyBridge.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2017 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary    and
  confidential information of Intel Corporation and its suppliers and licensors,
  and is protected by worldwide copyright and trade secret laws and treaty
  provisions. No part of the Material may be used, copied, reproduced, modified,
  published, uploaded, posted, transmitted, distributed, or disclosed in any way
  without Intel's prior express written permission.

  No license under any patent, copyright, trade secret or other intellectual
  property right is granted to or conferred upon you by disclosure or delivery
  of the Materials, either expressly, by implication, inducement, estoppel or
  otherwise. Any license under such intellectual property rights must be
  express and approved by Intel in writing.

  Unless otherwise agreed by Intel in writing, you may not remove or alter
  this notice or any other notice embedded in Materials by Intel or
  Intel's suppliers or licensors in any way.
**/

#ifndef _LEGACY_BRIDGE_H_
#define _LEGACY_BRIDGE_H_


//
// Forward reference for pure ANSI compatability
//
typedef struct _LEGACY_BRIDGE_PROTOCOL LEGACY_BRIDGE_PROTOCOL;


#define LEGACY_BRIDGE_SIGNATURE     SIGNATURE_32('L', 'G', 'B', 'G')
#define LEGACY_BRIDGE_VERSION       1

#define USB_DEV_NUMBER_IGNORE       0xFF

typedef
struct _USB_DEVICES_INFO {
  UINT32      NumberOfUsbMass;
  UINT32      NumberOfUsbKb;
  UINT32      NumberOfUsbMs;
  UINT32      NumberOfUsbHub;

} USB_DEVICES_INFO;


/**
  Get number of USB mass storage device.

  @param[out]  Num          Number of USB mass storage.

  @retval  EFI_SUCCESS       Operation executed successfully.
  @retval  EFI_DEVICE_ERROR  Devices is not work.

**/
typedef
EFI_STATUS
(EFIAPI *BRG_GET_USB_MASS_NUM) (
  IN  OUT UINT32              *Num
  );

/**
  Get string description for connected USB devices.

  @param[in out]  String          String description.
  @param[in]      Length          String length.

  @retval  EFI_SUCCESS       Operation executed successfully.
  @retval  EFI_DEVICE_ERROR  Devices is not work.

**/
typedef
EFI_STATUS
(EFIAPI *BRG_GET_USB_CONNECTED_DEVICES) (
  IN  OUT CHAR16              *String,
  IN  UINT32                  Length
  );

/**
  Get USB mass storage device name.

  @param[in out]  Data            ASCII string of device name.
  @param[in]      Size            String length.
  @param[in]      DevAddr         Device address.

  @retval  UINT8            Next avaiable device address.

**/
typedef
UINT8
(EFIAPI *BRG_GET_MASS_DEV_NAME) (
  IN  OUT UINT8               *Data,
  IN    UINT8                 Size,
  IN    UINT8                 DevAddr
  );

/**
  Get USB devices information.

  @param[out]  DevInfo         Devices information.

  @retval  EFI_SUCCESS       Operation executed successfully.
  @retval  EFI_DEVICE_ERROR  Devices is not work.

**/
typedef
EFI_STATUS
(EFIAPI *BRG_GET_USB_DEV_INFO) (
  IN  OUT USB_DEVICES_INFO    *DevInfo
  );

/**
  Enable serial redirection.

  @retval  EFI_SUCCESS       Operation executed successfully.
  @retval  EFI_DEVICE_ERROR  Devices is not work.

**/
typedef
EFI_STATUS
(EFIAPI *BRG_ENABLE_SERIAL_REDIRECTION) (
  VOID
  );

/**
  Disable serial redirection.

  @retval  EFI_SUCCESS       Operation executed successfully.
  @retval  EFI_DEVICE_ERROR  Devices is not work.

**/
typedef
EFI_STATUS
(EFIAPI *BRG_DISABLE_SERIAL_REDIRECTION) (
  VOID
  );

/**
  Uninstall legacy USB mass storage devices, safe hot remove device.

  if HcNumber or PortNumber is USB_DEV_NUMBER_IGNORE, their value will ignored,
  if not USB_DEV_NUMBER_IGNORE, function will match these value before uninstall.

  @param[in]  VendorId          USB device vendor id.
  @param[in]  DeviceId          USB device device id.
  @param[in]  HcNumber          USB device under host controller number.
  @param[in]  PortNumber        USB device hub port number.

  @retval  EFI_SUCCESS       Operation executed successfully.
  @retval  EFI_DEVICE_ERROR  Devices is not work.

**/
typedef
EFI_STATUS
(EFIAPI  *BRG_UNINSTALL_USB_MASS_DEV) (
  IN  UINT16                  VendorId,
  IN  UINT16                  DeviceId,
  IN  UINT8                   HcNumber, OPTIONAL
  IN  UINT8                   PortNumber OPTIONAL
  );

/**
  Get USB device descrption by its device handle.

  This function is for support get devices name in UEFI mode.

  @param[in]      Handle          BlockIo or UsbIo handle.
  @param[in out]  Description     String description.

  @retval  EFI_SUCCESS       Operation executed successfully.
  @retval  EFI_DEVICE_ERROR  Devices is not work.

**/
typedef
EFI_STATUS
(EFIAPI *BRG_GET_USB_DEV_NAME) (
  IN EFI_HANDLE                   Handle,
  IN  OUT CHAR16                  *Description
  );

//
// Legacy Bridge Protocol
//
struct _LEGACY_BRIDGE_PROTOCOL {

  UINT32                          Signature;
  UINT32                          Version;

  BRG_GET_USB_MASS_NUM             GetUsbMassNumber;
  BRG_GET_USB_CONNECTED_DEVICES    GetUsbConnectedDevices;
  BRG_GET_MASS_DEV_NAME            GetUsbMassDevName;
  BRG_GET_USB_DEV_INFO             GetUsbDevInfo;

  BRG_ENABLE_SERIAL_REDIRECTION    EnableSerialRedirection;
  BRG_DISABLE_SERIAL_REDIRECTION   DisableSerialRedirection;

  BRG_UNINSTALL_USB_MASS_DEV       UninstallUsbMassDev;
  BRG_GET_USB_DEV_NAME             GetUsbDeviceName;

};

//
// Extern the GUID for protocol users.
//
extern EFI_GUID gLegacyBridgeProtocolGuid;

#endif
