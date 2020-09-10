/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018-2020 Intel Corporation. <BR>

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

#include <PchLimits.h>
#include <Library/UbaPcieBifurcationUpdateLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PchInfoLib.h>

PCIE_BIFURCATION_CONFIG TypeArcherCityModularPchPcieBifurcationConfig[PCH_MAX_PCIE_CONTROLLERS] =
{
  //
  // SPA Controller - All lanes utilized by SATA - default
  //
  PcieBifurcationDefault,

  //
  // SPB Controller - Foxville x1, BMC x1
  //
  PcieBifurcation4x2,

  //
  // SPC Controller - PCIe x2 Slot, M.2 PCIe x2
  //
  PcieBifurcation4x2,

  //
  // SPD Controller - All lanes utilized by DMI - default
  //
  PcieBifurcationDefault
};

static PCH_SLOT_CONFIG_DATA_ENTRY_EX   PchSlotTable[] = {
  // Port Index  | Slot       |Interlock |power       |Power        |Hotplug  |Vpp Port      |Vpp Addr      |PCIeSSD  |PCIeSSD       |PCIeSSD       |Hidden    |Common   |Uplink   |Retimer  |Retimer        |ExtnCard|ExtnCard      |ExtnCard      |ExtnCard |ExtnCard Retimer|ExtnCard |ExtnCard Hotplug|ExtnCard Hotplug|Max Retimer|
  //             |            |          |Limit Scale |Limit Value  |Cap      |              |              |Cap      |Port          |Address       |          |Clock    |Port     |         |Address        |Support |SMBus Port    |SMBus Addr    |Retimer  |SMBus Address   |Hotplug  |Vpp Port        |Vpp Address     |           |
  {SOCKET_0_INDEX +
    PORT_0_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , SMB_ADDR_MAX , ENABLE  , VPP_PORT_0   , 0x4C         , DISABLE , SMB_ADDR_MAX   , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_0_INDEX +
    PORT_1_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , SMB_ADDR_MAX , ENABLE  , VPP_PORT_0   , 0x4C         , DISABLE , SMB_ADDR_MAX   , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_0_INDEX +
    PORT_2_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , SMB_ADDR_MAX , ENABLE  , VPP_PORT_0   , 0x4C         , DISABLE , SMB_ADDR_MAX   , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_0_INDEX +
    PORT_3_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , SMB_ADDR_MAX , ENABLE  , VPP_PORT_0   , 0x4C         , DISABLE , SMB_ADDR_MAX   , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_0_INDEX +
    PORT_4_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , SMB_ADDR_MAX , ENABLE  , VPP_PORT_0   , 0x4C         , DISABLE , SMB_ADDR_MAX   , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_0_INDEX +
    PORT_5_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , SMB_ADDR_MAX , ENABLE  , VPP_PORT_0   , 0x4C         , DISABLE , SMB_ADDR_MAX   , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_0_INDEX +
    PORT_6_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , SMB_ADDR_MAX , ENABLE  , VPP_PORT_0   , 0x4C         , DISABLE , SMB_ADDR_MAX   , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_0_INDEX +
    PORT_7_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , SMB_ADDR_MAX , ENABLE  , VPP_PORT_0   , 0x4C         , DISABLE , SMB_ADDR_MAX   , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_0_INDEX +
    PORT_8_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , SMB_ADDR_MAX , ENABLE  , VPP_PORT_0   , 0x4C         , DISABLE , SMB_ADDR_MAX   , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_0_INDEX +
    PORT_9_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , SMB_ADDR_MAX , ENABLE  , VPP_PORT_0   , 0x4C         , DISABLE , SMB_ADDR_MAX   , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_0_INDEX +
    PORT_10_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , SMB_ADDR_MAX , ENABLE  , VPP_PORT_0   , 0x4C         , DISABLE , SMB_ADDR_MAX   , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_0_INDEX +
    PORT_11_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , SMB_ADDR_MAX , ENABLE  , VPP_PORT_0   , 0x4C         , DISABLE , SMB_ADDR_MAX   , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_0_INDEX +
    PORT_12_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , SMB_ADDR_MAX , ENABLE  , VPP_PORT_0   , 0x4C         , DISABLE , SMB_ADDR_MAX   , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_0_INDEX +
    PORT_13_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , SMB_ADDR_MAX , ENABLE  , VPP_PORT_0   , 0x4C         , DISABLE , SMB_ADDR_MAX   , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_0_INDEX +
    PORT_14_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , SMB_ADDR_MAX , ENABLE  , VPP_PORT_0   , 0x4C         , DISABLE , SMB_ADDR_MAX   , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      },
  {SOCKET_0_INDEX +
    PORT_15_INDEX, NO_SLT_IMP , DISABLE , PWR_SCL_MAX , PWR_VAL_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , DISABLE , VPP_PORT_MAX , VPP_ADDR_MAX , NOT_HIDE , ENABLE  , DISABLE , DISABLE , SMB_ADDR_MAX , ENABLE  , VPP_PORT_0   , 0x4C         , DISABLE , SMB_ADDR_MAX   , DISABLE , VPP_PORT_MAX   , SMB_ADDR_MAX   , 0x0      }

};
/**
  Callback called when UBA is to update PCH PCIe bifurcation

  @param[in]  PchPcieControllerConfig        Delivers bifurcation configuration for PCH PCIe root ports

  @retval     EFI_SUCCESS                    The function completes successfully
  @retval     EFI_INVALID_PARAMETER          Given parameter was invalid
**/
EFI_STATUS
TypeArcherCityModularPchPcieBifurcationUpdateCallback (
  IN OUT   PCIE_BIFURCATION_CONFIG         **PchPcieBifurcationConfig,
  IN OUT   PCH_SLOT_CONFIG_DATA_ENTRY_EX   **PchSlotConfig
  )
{
  if (PchPcieBifurcationConfig == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  *PchPcieBifurcationConfig   = TypeArcherCityModularPchPcieBifurcationConfig;
  *PchSlotConfig = PchSlotTable;
  return EFI_SUCCESS;
}

PLATFORM_PCH_PCIE_BIFURCATION_UPDATE_TABLE  TypeArcherCityModularPchPcieBifurcationUpdate =
{
   PLATFORM_PCH_PCIE_BIFURCATION_UPDATE_SIGNATURE,
   PLATFORM_PCH_PCIE_BIFURCATION_UPDATE_VERSION,
   TypeArcherCityModularPchPcieBifurcationUpdateCallback
};

/**
  Adds PCH PCIe bifurcation settings to UBA database

  @param[in]  UbaConfigPpi           The general config database of settings for UBA

  @retval     EFI_SUCCESS            The function completes successfully
  @retval     EFI_OUT_OF_RESOURCES   Insufficient resources to create database
  @retval     EFI_INVALID_PARAMETER  Given parameter was invalid
**/
EFI_STATUS
TypeArcherCityModularPchPcieBifurcation (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
  )
{
  //
  // bifurcation configurations TBD once boards are described
  //

  EFI_STATUS                   Status;

  DEBUG((DEBUG_INFO,"UBA PEI: %a\n",__FUNCTION__));

  ASSERT ( ARRAY_SIZE(PchSlotTable) == GetPchMaxPciePortNum ());

  if (UbaConfigPpi == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = UbaConfigPpi->AddData (
                                 UbaConfigPpi,
                                 &gPlatformUbaPcieBifurcationGuid,
                                 &TypeArcherCityModularPchPcieBifurcationUpdate,
                                 sizeof(TypeArcherCityModularPchPcieBifurcationUpdate)
                                 );

  return Status;
}
