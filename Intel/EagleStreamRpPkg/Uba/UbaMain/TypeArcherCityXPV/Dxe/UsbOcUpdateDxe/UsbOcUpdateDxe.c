/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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


#include <Library/DebugLib.h>  // for DEBUG()
#include <Protocol/UbaCfgDb.h> // for UBA_CONFIG_DATABASE_PROTOCOL
#include <Library/UefiBootServicesTableLib.h> // for gBS

#include <PchLimits.h>
#include <ConfigBlock.h>
#include <UsbConfig.h>
#include <Usb2PhyConfig.h>
#include <Library/PchInfoLib.h>
#include <Library/UbaUsbOcUpdateLib.h>

USB_OVERCURRENT_PIN TypeArcherCityXPVUsb20OverCurrentMappings[MAX_USB2_PORTS] = {
  0x4, //UsbOverCurrentPin4,      //Port00: Internal
  0x4, //UsbOverCurrentPin4,      //Port01: Internal
  0x2, //UsbOverCurrentPin2,      //Port02: Rear Panel
  0x2, //UsbOverCurrentPin2,      //Port03:
  0x3, //UsbOverCurrentPin3,      //Port04: Internal
  0x6, //UsbOverCurrentPin6,      //Port05:
  0x3, //UsbOverCurrentPin3,      //Port06: Internal
  0x6, //UsbOverCurrentPin6,      //Port07:
  0x1, //UsbOverCurrentPin1,      //Port08: Rear Panel
  0x5, //UsbOverCurrentPin5,      //Port09:
  USB_OC_SKIP, //UsbOverCurrentPinSkip,   //Port10: NC
  0x7, //UsbOverCurrentPin7,      //Port11:
  USB_OC_SKIP, //UsbOverCurrentPinSkip,   //Port12: NC
  0x1, //      //Port13: 
  USB_OC_SKIP, //UsbOverCurrentPinSkip,
  USB_OC_SKIP, //UsbOverCurrentPinSkip
};

USB_OVERCURRENT_PIN TypeArcherCityXPVUsb30OverCurrentMappings[MAX_USB3_PORTS] = {
  USB_OC_SKIP,
  USB_OC_SKIP,
  USB_OC_SKIP,
  USB_OC_SKIP,
  USB_OC_SKIP,
  USB_OC_SKIP,
  USB_OC_SKIP,
  USB_OC_SKIP,
  0x1,      //Port08: Real Panel
  0x6       //Port09: Front Panel
};

USB2_PHY_PARAMETERS TypeArcherCityXPVUsb20AfeParams[4];


EFI_STATUS
TypeArcherCityXPVPlatformUsbOcUpdateCallback (
  IN OUT   USB_OVERCURRENT_PIN   **Usb20OverCurrentMappings,
  IN OUT   USB_OVERCURRENT_PIN   **Usb30OverCurrentMappings,
  IN OUT   USB2_PHY_PARAMETERS   **Usb20AfeParams
  )
{
  DEBUG ((DEBUG_INFO,"UBA DXE: %a\n",__FUNCTION__));

  *Usb20OverCurrentMappings = &TypeArcherCityXPVUsb20OverCurrentMappings[0];
  *Usb30OverCurrentMappings = &TypeArcherCityXPVUsb30OverCurrentMappings[0];
  *Usb20AfeParams           = TypeArcherCityXPVUsb20AfeParams;

  return EFI_SUCCESS;
}

PLATFORM_USBOC_UPDATE_TABLE  TypeArcherCityXPVUsbOcUpdate = {
   PLATFORM_USBOC_UPDATE_SIGNATURE,
   PLATFORM_USBOC_UPDATE_VERSION,
   TypeArcherCityXPVPlatformUsbOcUpdateCallback
};

/**
  The Driver Entry Point.

  The function is the driver Entry point.

  @param ImageHandle   A handle for the image that is initializing this driver
  @param SystemTable   A pointer to the EFI system table

  @retval EFI_SUCCESS:              Driver initialized successfully
  @retval EFI_LOAD_ERROR:           Failed to Initialize or has been loaded
  @retval EFI_OUT_OF_RESOURCES      Could not allocate needed resources

**/
EFI_STATUS
EFIAPI
UsbOcUpdateEntry (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  EFI_STATUS                          Status;
  UBA_CONFIG_DATABASE_PROTOCOL        *UbaConfigProtocol = NULL;

  DEBUG ((DEBUG_INFO,"UBA DXE: %a\n",__FUNCTION__));
  Status = gBS->LocateProtocol (
                  &gUbaConfigDatabaseProtocolGuid,
                  NULL,
                  &UbaConfigProtocol
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = UbaConfigProtocol->AddData (
                                UbaConfigProtocol,
                                &gDxePlatformUbaOcConfigDataGuid,
                                &TypeArcherCityXPVUsbOcUpdate,
                                sizeof(TypeArcherCityXPVUsbOcUpdate)
                                );

  return Status;
}
