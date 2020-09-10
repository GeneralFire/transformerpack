/** @file
  OemIioHooksPpi.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

#ifndef _OEM_IIO_HOOKS_PPI_H_
#define _OEM_IIO_HOOKS_PPI_H_


#include <IioPlatformData.h>

extern EFI_GUID gPeiOemIioHooksPpiGuid;
/**

Platform function to get platform info

@param IioGlobalData  - Pointer to the IIO global Data structure

@retval               - N/A

**/
typedef
VOID
(EFIAPI *OEM_GET_IIO_PLATFORM_INFO) (
  IIO_GLOBALS    *IioGlobalData
  );

/**

Platform function to get IIO uplink port info

@param IioGlobalData  - Pointer to the IIO global Data structure
@param IioIndex       - Index of IIO
@param PortIndex      - pointer to port index
@param Bus            - pointer to bus number
@param Device         - pointer to device index
@param Function       - pointer to function index

@retval               - N/A

**/
typedef
VOID
(EFIAPI *OEM_IIO_UPLINK_PORT_DETAILS) (
  IIO_GLOBALS   *IioGlobalData,
  UINT8         IioIndex,
  UINT8         *PortIndex,
  UINT8         *Bus,
  UINT8         *Device,
  UINT8         *Function
  );

/**

Platform function to do IIO WA at early stage

@param IioGlobalData  - Pointer to the IIO global Data structure
@param IioIndex       - Index of IIO
@param Phase          - phase to hook

@retval               - N/A

**/
typedef
VOID
(EFIAPI *OEM_IIO_EARLY_WORK_AROUND) (
  IIO_GLOBALS                 *IioGlobalData,
  UINT8                       IioIndex,
  IIO_INIT_ENUMERATION        Phase
  );

/**

Platform function to override Iio Rx recipe settings

@param IioGlobalData  - Pointer to the IIO global Data structure
@param IioIndex       - Index of IIO

@retval               - N/A

**/
typedef
VOID
(EFIAPI *OEM_OVERRIDE_IIO_RX_RECIPE_SETTINGS) (
  IIO_GLOBALS                *IioGlobalData,
  UINT8                      IioIndex
  );


typedef
VOID
(EFIAPI *OEM_DEFAULT_IIO_PORT_BIFURATION_INFO) (
  IIO_GLOBALS  *IioGlobalData
  );

/**

Platform function to initialize IIO Port Bifurcation

@param IioGlobalData  - Pointer to the IIO global Data structure

@retval  N/A

**/
typedef
VOID
(EFIAPI *OEM_INIT_IIO_PORT_BIFURCATION_INFO) (
  IIO_GLOBALS  *IioGlobalData
  );

/**

Platform function to pre-initialize Dmi device

@param PciAddress     - Pci address
@param IioGlobalData  - Pointer to the IIO global Data structure
@param IioIndex       - Index of IIO to pre-initialize
@param PortIndex      - Index of IIO port

@retval  N/A

**/
typedef
VOID
(EFIAPI *OEM_DMI_PRE_INIT) (
  IN IIO_GLOBALS    *IioGlobalData,
  IN UINT8          IioIndex,
  IN UINT8          PortIndex
  );

/**

Platform function to initialize Dmi device

@param PciAddress     - Pci address
@param IioGlobalData  - Pointer to the IIO global Data structure
@param IioIndex       - Index of IIO to initialize
@param PortIndex      - Index of IIO port

@retval  N/A

**/
typedef
VOID
(EFIAPI *OEM_DMI_DEVICE_INIT) (
  IN IIO_GLOBALS    *IioGlobalData,
  IN UINT8          IioIndex,
  IN UINT8          PortIndex
  );

/**

Platform function to initialize Dmi Resource

@param PciAddress     - Pci address
@param IioGlobalData  - Pointer to the IIO global Data structure
@param IioIndex       - Index of IIO to initialize
@param PortIndex      - Index of IIO port

@retval  N/A

**/

typedef
VOID
(EFIAPI *OEM_DMI_RESOURCE_ASSIGNED_INIT) (
  IN IIO_GLOBALS    *IioGlobalData,
  IN UINT8          IioIndex,
  IN UINT8          PortIndex
  );

/**

Initialize the Global Data Structure with the Default Values

@param IioGlobalData - Pointer to IioGlobalData

@retval NONE

**/
typedef
VOID
(EFIAPI *OEM_SET_IIO_DEFAULT_VALUES) (
  IIO_GLOBALS  *IioGlobalData
  );

/**

Initialize the Global Data Structure with the Setup Values
read from NVRAM

@param IioGlobalData - Pointer to IioGlobalData

@retval VOID              All other error conditions encountered result in an ASSERT

**/
typedef
VOID
(EFIAPI *OEM_SET_PLATFORM_DATA_VALUES) (
  IIO_GLOBALS  *IioGlobalData
  );

/**

Publish the HOB for HFI UEFI driver usage

@param IioGlobalData - Pointer to IioGlobalData

@retval VOID        All other error conditions encountered result in an ASSERT

**/
typedef
VOID
(EFIAPI *OEM_PUBLISH_OPA_SOCKET_MAP_HOB) (
  IIO_GLOBALS  *IioGlobalData
  );

typedef struct _OEM_IIO_HOOKS_PPI         OEM_IIO_HOOKS_PPI;

struct _OEM_IIO_HOOKS_PPI {
  OEM_GET_IIO_PLATFORM_INFO               OemGetIioPlatformInfo;
  OEM_IIO_UPLINK_PORT_DETAILS             OemIioUplinkPortDetails;
  OEM_IIO_EARLY_WORK_AROUND               OemIioEarlyWorkAround;
  OEM_OVERRIDE_IIO_RX_RECIPE_SETTINGS     OemOverrideIioRxRecipeSettings;
  OEM_DEFAULT_IIO_PORT_BIFURATION_INFO    OemDefaultIioPortBifurationInfo;
  OEM_INIT_IIO_PORT_BIFURCATION_INFO      OemInitIioPortBifurcationInfo;
  OEM_DMI_PRE_INIT                        OemDmiPreInit;
  OEM_DMI_DEVICE_INIT                     OemDmiDeviceInit;
  OEM_DMI_RESOURCE_ASSIGNED_INIT          OemDmiResourceAssignedInit;
  OEM_SET_IIO_DEFAULT_VALUES              OemSetIioDefaultValues;
  OEM_SET_PLATFORM_DATA_VALUES            OemSetPlatformDataValues;
  OEM_PUBLISH_OPA_SOCKET_MAP_HOB          OemPublishOpaSocketMapHob;
};

#endif  // _OEM_IIO_HOOKS_PPI
