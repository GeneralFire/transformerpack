/** @file
  UbaSsidSvidUpdateLib implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2016 Intel Corporation. <BR>

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

#include <Library/UbaSsidSvidUpdateLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>

#include <Protocol/UbaCfgDb.h>

/**
  Register a SSIDSVID data update callback.

  @param RegData                  Register data.

  @retval EFI_SUCCESS             Register successfully.
  @retval EFI_NOT_FOUND           Depend protocol not found.

**/
EFI_STATUS
PlatformRegisterSsidSvidUpdate (
  IN      PLATFORM_SSIDSVID_UPDATE_DATA     *RegData
)
{
  EFI_STATUS                                    Status;
  STATIC  UBA_CONFIG_DATABASE_PROTOCOL    *UbaConfigProtocol = NULL;

  if (UbaConfigProtocol == NULL) {

    Status = gBS->LocateProtocol (
                    &gUbaConfigDatabaseProtocolGuid,
                    NULL,
                    &UbaConfigProtocol
                    );
    if (EFI_ERROR(Status)) {
      return Status;
    }
  }

  RegData->Signature  = PLATFORM_SSIDSVID_UPDATE_SIGNATURE;
  RegData->Version    = PLATFORM_SSIDSVID_UPDATE_VERSION;

  Status = UbaConfigProtocol->AddData (
                                     UbaConfigProtocol,
                                     &gPlatformSsidSvidConfigDataGuid,
                                     RegData,
                                     sizeof(PLATFORM_SSIDSVID_UPDATE_DATA)
                                     );

  return Status;
}

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
)
{
  EFI_STATUS                              Status;

  UBA_CONFIG_DATABASE_PROTOCOL      *UbaConfigProtocol = NULL;
  UINTN                                   DataLength = 0;
  PLATFORM_SSIDSVID_UPDATE_DATA           *NewRegData;

  if (RegData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  NewRegData = AllocateZeroPool (sizeof (PLATFORM_SSIDSVID_UPDATE_DATA));
  if (NewRegData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = gBS->LocateProtocol (
                  &gUbaConfigDatabaseProtocolGuid,
                  NULL,
                  &UbaConfigProtocol
                  );

  if (EFI_ERROR(Status)) {
    gBS->FreePool(NewRegData);
    return Status;
  }

  DataLength  = sizeof (PLATFORM_SSIDSVID_UPDATE_DATA);
  Status = UbaConfigProtocol->GetData (
                                    UbaConfigProtocol,
                                    &gPlatformSsidSvidConfigDataGuid,
                                    NewRegData,
                                    &DataLength
                                    );
  if (!EFI_ERROR(Status)) {

    ASSERT (NewRegData->Signature == PLATFORM_SSIDSVID_UPDATE_SIGNATURE);
    ASSERT (NewRegData->Version == PLATFORM_SSIDSVID_UPDATE_VERSION);

    *RegData =  NewRegData;
    return EFI_SUCCESS;
  }

  gBS->FreePool(NewRegData);
  return EFI_NOT_FOUND;
}

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
)
{
  EFI_STATUS                                Status;
  EFI_PCI_IO_PROTOCOL                       *PciIo = NULL;

  UINTN                                     HandleCount = 0;
  EFI_HANDLE                                *HandleBuffer = NULL;
  UINTN                                     Index;

  PCI_TYPE00                                PciDevice;
  UINT32                                    ThisSidVid = 0;

  if (SsidSvid == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );

  if (EFI_ERROR(Status)) {
    return EFI_DEVICE_ERROR;
  }

  for (Index = 0; Index < HandleCount; Index++) {

    Status = gBS->HandleProtocol (HandleBuffer[Index], &gEfiPciIoProtocolGuid, &PciIo);
    if (!EFI_ERROR (Status)) {
      Status = PciIo->Pci.Read (
                          PciIo,
                          EfiPciIoWidthUint32,
                          0,
                          sizeof (PciDevice)/sizeof (UINT32),
                          &PciDevice
                          );
      ThisSidVid = (PciDevice.Hdr.DeviceId << 16) | PciDevice.Hdr.VendorId;

      if (ThisSidVid == SidVid) {
        *SsidSvid = (PciDevice.Device.SubsystemID << 16) | PciDevice.Device.SubsystemVendorID;
        return EFI_SUCCESS;
      }
    }
  }

  return EFI_NOT_FOUND;
}

