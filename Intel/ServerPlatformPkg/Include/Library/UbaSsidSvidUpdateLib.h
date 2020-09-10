/** @file
  UBA ssid/svid Update Library Header File.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2015 Intel Corporation. <BR>

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

#ifndef _UBA_SSID_SVID_UPDATE_LIB_
#define _UBA_SSID_SVID_UPDATE_LIB_

#include <Base.h>
#include <Uefi.h>
#include <IndustryStandard/Pci.h>
#include <Protocol/PciIo.h>

#define PLATFORM_SSIDSVID_UPDATE_SIGNATURE  SIGNATURE_32 ('S', 'S', 'I', 'D')
#define PLATFORM_SSIDSVID_UPDATE_VERSION    01

// {EF363C9F-CAA1-4303-833F-E2BEB7632E50}
STATIC EFI_GUID  gPlatformSsidSvidConfigDataGuid =
{ 0xef363c9f, 0xcaa1, 0x4303, { 0x83, 0x3f, 0xe2, 0xbe, 0xb7, 0x63, 0x2e, 0x50 } };

typedef
EFI_STATUS
(*PLATFORM_SSIDSVID_UPDATE_CALLBACK) (
  IN      EFI_PCI_IO_PROTOCOL               *PciIo,
  IN      PCI_DEVICE_INDEPENDENT_REGION     *PciHeader,
  IN  OUT UINT32                            *SsidSvid
);

typedef struct {
  UINT32                              Signature;
  UINT32                              Version;

  UINT32                              PlatformType;

  PLATFORM_SSIDSVID_UPDATE_CALLBACK   CallUpdate;       // Optional
} PLATFORM_SSIDSVID_UPDATE_DATA;

/**
  Register a SSIDSVID data update callback.

  @param RegData                  Register data.

  @retval EFI_SUCCESS             Register successfully.
  @retval EFI_NOT_FOUND           Depend protocol not found.

**/
EFI_STATUS
PlatformRegisterSsidSvidUpdate (
  IN      PLATFORM_SSIDSVID_UPDATE_DATA     *RegData
);

/**
  Get registered SSIDSVID data update callback.

  @param RegData                  Register data.

  @retval EFI_SUCCESS             Get registered data successfully.
  @retval EFI_NOT_FOUND           Data not found.
  @retval EFI_INVALID_PARAMETER   Parameter not correct.

**/
EFI_STATUS
PlatformGetSsidSvid (
  IN  OUT PLATFORM_SSIDSVID_UPDATE_DATA     **RegData
);

/**
  Get the SSIDSVID value for specific device by given SID&VID.

  @param SidVid                   The system id and vendor id for device.
  @param SsidSvid                 The subsystem id and subvendor id will return

  @retval EFI_SUCCESS             Get data successfully.
  @retval EFI_NOT_FOUND           Data not found.
  @retval EFI_INVALID_PARAMETER   Parameter not correct.
  @retval EFI_DEVICE_ERROR        Depend protocol not found

**/
EFI_STATUS
PlatformSearchSsidForDevice (
  IN      UINT32                            SidVid,
  OUT     UINT32                            *SsidSvid
);

#endif //_UBA_SSID_SVID_UPDATE_LIB_
