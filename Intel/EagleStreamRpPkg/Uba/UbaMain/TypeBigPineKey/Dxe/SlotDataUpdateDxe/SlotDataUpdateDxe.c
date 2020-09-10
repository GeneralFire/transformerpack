/** @file
  Slot Data Update.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include "SlotDataUpdateDxe.h"

UINT8
GetTypeBigPineKeyIOU0Setting (
  UINT8  IOU0Data
)
{
  //
  // Change bifurcation of Port1A-1B as xxx8 when QATGpio enabled.
  //
  IOU0Data = IIO_BIFURCATE_xxx8xxx8;
  return IOU0Data;
}

UINT8
GetTypeBigPineKeyIOU2Setting (
  UINT8  SkuPersonalityType,
  UINT8  IOU2Data
)
{
  return IOU2Data;
}

static IIO_BROADWAY_ADDRESS_DATA_ENTRY   SlotTypeBigPineKeyBroadwayTable[] = {
    {Iio_Socket0, Iio_Iou2, Bw5_Addr_0 },
    {Iio_Socket1, Iio_Iou1, Bw5_Addr_2},
    {Iio_Socket1, Iio_Iou0, Bw5_Addr_1 },
};


PLATFORM_SLOT_UPDATE_TABLE  TypeBigPineKeySlotTable =
{
  PLATFORM_SLOT_UPDATE_SIGNATURE,
  PLATFORM_SLOT_UPDATE_VERSION,

  SlotTypeBigPineKeyBroadwayTable,
  GetTypeBigPineKeyIOU0Setting,
  0
};

PLATFORM_SLOT_UPDATE_TABLE2  TypeBigPineKeySlotTable2 =
{
  PLATFORM_SLOT_UPDATE_SIGNATURE,
  PLATFORM_SLOT_UPDATE_VERSION,

  SlotTypeBigPineKeyBroadwayTable,
  GetTypeBigPineKeyIOU0Setting,
  0,
  GetTypeBigPineKeyIOU2Setting
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
SlotDataUpdateEntry (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
)
{
  EFI_STATUS                               Status;
  UBA_CONFIG_DATABASE_PROTOCOL             *UbaConfigProtocol = NULL;

  DEBUG((EFI_D_ERROR, "UBA:SlotDataUpdate-TypeBigPineKey\n"));
  Status = gBS->LocateProtocol (
                  &gUbaConfigDatabaseProtocolGuid,
                  NULL,
                  &UbaConfigProtocol
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = UbaConfigProtocol->AddData (
                                     UbaConfigProtocol,
                                     &gPlatformSlotDataDxeGuid,
                                     &TypeBigPineKeySlotTable,
                                     sizeof(TypeBigPineKeySlotTable)
                                     );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = UbaConfigProtocol->AddData (
                                     UbaConfigProtocol,
                                     &gPlatformSlotDataDxeGuid,
                                     &TypeBigPineKeySlotTable2,
                                     sizeof(TypeBigPineKeySlotTable2)
                                     );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  return Status;
}
