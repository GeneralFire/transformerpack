/** @file
  Slot Table Update.

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

#include "PeiBoardInit.h"
#include <Library/UbaSlotUpdateLib.h>
#include <IioPlatformData.h>

typedef enum {
  Iio_Socket0 = 0,
  Iio_Socket1,
  Iio_Socket2,
  Iio_Socket3,
  Iio_Socket4,
  Iio_Socket5,
  Iio_Socket6,
  Iio_Socket7
} IIO_SOCKETS;

typedef enum {
  Iio_Iou0 =0,
  Iio_Iou1,
  Iio_Iou2,
  Iio_Iou3,
  Iio_Iou4,
  Iio_IouMax
} IIO_IOUS;

typedef enum {
  Bw5_Addr_0 = 0,
  Bw5_Addr_1,
  Bw5_Addr_2,
  Bw5_Addr_3,
  Bw5_Addr_Max
} BW5_ADDRESS;


UINT8
GetTypeBigPineKeyIOU0Setting (
  UINT8  IOU0Data
)
{
  //
  // Change bifurcation of Port1A-1B as xxx8 when QATGpio enabled.
  //
  IOU0Data = IIO_BIFURCATE_xxxxxx16;
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
  Entry point function for the PEIM

  @param FileHandle      Handle of the file being invoked.
  @param PeiServices     Describes the list of possible PEI Services.

  @return EFI_SUCCESS    If we installed our PPI

**/
EFI_STATUS
TypeBigPineKeyInstallSlotTableData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
)
{
  EFI_STATUS                         Status;

  Status = UbaConfigPpi->AddData (
                                 UbaConfigPpi,
                                 &gPlatformSlotDataGuid,
                                 &TypeBigPineKeySlotTable,
                                 sizeof(TypeBigPineKeySlotTable)
                                 );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = UbaConfigPpi->AddData (
                                 UbaConfigPpi,
                                 &gPlatformSlotDataGuid2,
                                 &TypeBigPineKeySlotTable2,
                                 sizeof(TypeBigPineKeySlotTable2)
                                 );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  return Status;
}
