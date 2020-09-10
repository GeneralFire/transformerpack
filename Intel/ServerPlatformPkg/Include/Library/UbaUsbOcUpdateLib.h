/** @file
  UBA USB OC Update Library Header File.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2019 Intel Corporation. <BR>

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

#ifndef _UBA_USBOC_UPDATE_LIB_H
#define _UBA_USBOC_UPDATE_LIB_H

#include <Base.h>
#include <Uefi.h>
#include <ConfigBlock.h>
#include <ConfigBlock/UsbConfig.h>
#include <ConfigBlock/Usb2PhyConfig.h>

#define PLATFORM_USBOC_UPDATE_SIGNATURE  SIGNATURE_32 ('P', 'U', 'S', 'B')
#define PLATFORM_USBOC_UPDATE_VERSION    02

// {88238275-5922-46b6-9e35-656df55df44c}
#define   PEI_PLATFORM_USBOC_CONFIG_DATA_GUID \
{ 0x88238275, 0x5922, 0x46f6, { 0x9e, 0x35, 0x65, 0x6d, 0xf5, 0x5d, 0xf4, 0x4c } }
// {2638009e-3850-4e4b-b05d-042a32dbb9d1}
#define   DXE_PLATFORM_USBOC_CONFIG_DATA_GUID \
{ 0x2638009e, 0x3850, 0x4e4b, { 0xb0, 0x5d, 0x04, 0x2a, 0x32, 0xdb, 0xb9, 0xd1 } }

typedef
EFI_STATUS
(*USBOC_UPDATE_CALLBACK) (
  IN OUT   USB_OVERCURRENT_PIN   **Usb20OverCurrentMappings,
  IN OUT   USB_OVERCURRENT_PIN   **Usb30OverCurrentMappings,
  IN OUT   USB2_PHY_PARAMETERS   **Usb20AfeParams
);

typedef struct _PLATFORM_USBOC_UPDATE_TABLE{
  UINT32                    Signature;
  UINT32                    Version;
  USBOC_UPDATE_CALLBACK     CallUsbOcUpdate;
} PLATFORM_USBOC_UPDATE_TABLE;

EFI_STATUS
PlatformGetUsbOcMappings (
  IN OUT   USB_OVERCURRENT_PIN   **Usb20OverCurrentMappings,
  IN OUT   USB_OVERCURRENT_PIN   **Usb30OverCurrentMappings,
  IN OUT   USB2_PHY_PARAMETERS   **Usb20AfeParams
);
STATIC  EFI_GUID gPeiPlatformUbaOcConfigDataGuid = PEI_PLATFORM_USBOC_CONFIG_DATA_GUID;
STATIC  EFI_GUID gDxePlatformUbaOcConfigDataGuid = DXE_PLATFORM_USBOC_CONFIG_DATA_GUID;

#endif //_UBA_USBOC_UPDATE_LIB_H
