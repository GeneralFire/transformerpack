/** @file
  IIO Config Update.

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

#include "IioCfgUpdateDxe.h"

static IIO_BIFURCATION_DATA_ENTRY   IioBifurcationTable[] =
{
  { Iio_Socket0, Iio_Iou0, IIO_BIFURCATE_xxx8x4x4 },
  { Iio_Socket0, Iio_Iou1, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket0, Iio_Iou2, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket0, Iio_Mcp0, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket0, Iio_Mcp1, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket1, Iio_Iou0, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket1, Iio_Iou1, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket1, Iio_Iou2, IIO_BIFURCATE_xxx8xxx8 },
  { Iio_Socket1, Iio_Mcp0, IIO_BIFURCATE_xxxxxx16 },
  { Iio_Socket1, Iio_Mcp1, IIO_BIFURCATE_xxxxxx16 },
};

static IIO_SLOT_CONFIG_DATA_ENTRY   IioSlotTable[] = {
  // Port        |  Slot      | Inter      | Power Limit | Power Limit | Hot     | Vpp          | Vpp          | PcieSSD | PcieSSD     | PcieSSD       | Hidden
  // Index       |            | lock       | Scale       |  Value      | Plug    | Port         | Addr         | Cap     | VppPort     | VppAddr       |
  { PORT_1A_INDEX, NO_SLT_IMP , DISABLE    , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , ENABLE  , VPP_PORT_0  , 0x4C          , HIDE    },//Oculink
  { PORT_1B_INDEX, NO_SLT_IMP , DISABLE    , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , ENABLE  , VPP_PORT_1  , 0x4C          , HIDE    },//Oculink
  { PORT_1C_INDEX,  1         , DISABLE    , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE },
  { PORT_2A_INDEX, NO_SLT_IMP , DISABLE    , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE },
  // Slot 2 supports HP:  PCA9555 (CPU0) Addres 0x40, SCH (Rev 0.604) P 118 (MRL in J65)
  { PORT_3A_INDEX,  2         , ENABLE     , PWR_SCL_MAX , PWR_VAL_MAX , ENABLE  , VPP_PORT_0   , 0x40         , ENABLE  , VPP_PORT_0    , 0x40         , NOT_HIDE },
  { PORT_3B_INDEX, NO_SLT_IMP , DISABLE    , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , ENABLE  , VPP_PORT_1    , 0x40         , HIDE    },
  { PORT_3C_INDEX, NO_SLT_IMP , DISABLE    , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , ENABLE  , VPP_PORT_0    , 0x42         , HIDE    },
  { PORT_3D_INDEX, NO_SLT_IMP , DISABLE    , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , ENABLE  , VPP_PORT_1    , 0x42         , HIDE    },
  { SOCKET_1_INDEX +
    PORT_0_INDEX ,   6        , DISABLE    , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE },
  // Slot 4 supports HP: PCA9554 (CPU1) Address 0x40, SCH (Rev 0.604) P 121 (MRL in J287)
  { SOCKET_1_INDEX +
    PORT_1A_INDEX,   4        , ENABLE     , PWR_SCL_MAX , PWR_VAL_MAX , ENABLE  , VPP_PORT_1   , 0x40         , ENABLE  , VPP_PORT_0    , 0x40         , NOT_HIDE },
  { SOCKET_1_INDEX +
    PORT_1B_INDEX, NO_SLT_IMP , ENABLE     , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , ENABLE  , VPP_PORT_1    , 0x40         , HIDE    },
  { SOCKET_1_INDEX +
    PORT_1C_INDEX, NO_SLT_IMP , ENABLE     , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , ENABLE  , VPP_PORT_0    , 0x42         , HIDE    },
  { SOCKET_1_INDEX +
    PORT_1D_INDEX, NO_SLT_IMP , ENABLE     , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , ENABLE  , VPP_PORT_1    , 0x42         , HIDE    },
  { SOCKET_1_INDEX +
    PORT_2A_INDEX,  8         , DISABLE    , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_1   , VPP_ADDR_MAX , ENABLE  , VPP_PORT_0    , 0x44         , NOT_HIDE },
  { SOCKET_1_INDEX +
    PORT_2B_INDEX, NO_SLT_IMP , DISABLE    , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , ENABLE  , VPP_PORT_1    , 0x44         , HIDE    },
  { SOCKET_1_INDEX +
    PORT_2C_INDEX, NO_SLT_IMP , DISABLE    , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , ENABLE  , VPP_PORT_0    , 0x46         , HIDE    },
  { SOCKET_1_INDEX +
    PORT_2D_INDEX, NO_SLT_IMP , DISABLE    , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , ENABLE  , VPP_PORT_1    , 0x46         , HIDE    },
  { SOCKET_1_INDEX +
    PORT_3A_INDEX,  5         , DISABLE    , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE },
  { SOCKET_1_INDEX +
    PORT_3C_INDEX,  7         , DISABLE    , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE },
};


EFI_STATUS
UpdateArcherCityModularIioConfig (
  IN  IIO_GLOBALS             *IioGlobalData
  )
{
  return EFI_SUCCESS;
}

PLATFORM_IIO_CONFIG_UPDATE_TABLE  TypeArcherCityModularIioConfigTable =
{
  PLATFORM_IIO_CONFIG_UPDATE_SIGNATURE,
  PLATFORM_IIO_CONFIG_UPDATE_VERSION,

  IioBifurcationTable,
  sizeof(IioBifurcationTable),
  UpdateArcherCityModularIioConfig,
  IioSlotTable,
  sizeof(IioSlotTable)

};

/**
  The Driver Entry Point.

  @param ImageHandle   A handle for the image that is initializing this driver
  @param SystemTable   A pointer to the EFI system table

  @retval EFI_SUCCESS:              Driver initialized successfully
  @retval EFI_LOAD_ERROR:           Failed to Initialize or has been loaded
  @retval EFI_OUT_OF_RESOURCES      Could not allocate needed resources
**/
EFI_STATUS
EFIAPI
IioCfgUpdateEntry (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
)
{
  EFI_STATUS                               Status;
  UBA_CONFIG_DATABASE_PROTOCOL             *UbaConfigProtocol = NULL;

  DEBUG((EFI_D_ERROR, "UBA:IioCfgUpdate-TypeArcherCityModular\n"));
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
                                     &gPlatformIioConfigDataDxeGuid,
                                     &TypeArcherCityModularIioConfigTable,
                                     sizeof(TypeArcherCityModularIioConfigTable)
                                     );

  Status = UbaConfigProtocol->AddData (
                                     UbaConfigProtocol,
                                     &gPlatformIioConfigDataDxeGuid_1,
                                     &TypeArcherCityModularIioConfigTable,
                                     sizeof(TypeArcherCityModularIioConfigTable)
                                     );

  Status = UbaConfigProtocol->AddData (
                                     UbaConfigProtocol,
                                     &gPlatformIioConfigDataDxeGuid_2,
                                     &TypeArcherCityModularIioConfigTable,
                                     sizeof(TypeArcherCityModularIioConfigTable)
                                     );

  Status = UbaConfigProtocol->AddData (
                                     UbaConfigProtocol,
                                     &gPlatformIioConfigDataDxeGuid_3,
                                     &TypeArcherCityModularIioConfigTable,
                                     sizeof(TypeArcherCityModularIioConfigTable)
                                     );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  return Status;
}
