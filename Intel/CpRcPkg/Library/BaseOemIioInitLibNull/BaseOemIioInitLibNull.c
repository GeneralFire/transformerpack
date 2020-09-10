/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 Intel Corporation. <BR>

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

#include <IioPlatformData.h>
#include <Library/OemIioInit.h>

/**

  Initialize the Global Data Structure with the IOUx Bifurcation
  with default values

  @param[i]n] IioGlobalData - Pointer to IioGlobalData

  @retval VOID           All other error conditions encountered result in an ASSERT

**/

VOID
OemDefaultIioPortBifurationInfo (
  IN  IIO_GLOBALS  *IioGlobalData
  )
{

  return;

} // OemDefaultIioPortBifurationInfo

/**

  Initialize the Global Data Structure with the IOUx Bifurcation
  based on Board layout or user selection

  @param[in] IioGlobalData - Pointer to IioGlobalData

  @retval VOID All other error conditions encountered result in an ASSERT

**/

VOID
OemInitIioPortBifurcationInfo (
  IN  IIO_GLOBALS  *IioGlobalData
  )
{

  return;

} // OemInitIioPortBifurcationInfo

/**

  Initialize the Global Data Structure with the Default Values

  @param IioGlobalData[in, out] - Pointer to IioGlobalData

  @retval NONE

**/

VOID
OemSetIioDefaultValues (
  IN OUT IIO_GLOBALS *IioGlobalData
  )
{

  return;

} // OemSetIioDefaultValues

/**

  Initialize the Global Data Structure with the Setup Values
  read from NVRAM

  @param IioGlobalData[in, out] - Pointer to IioGlobalData

  @retval VOID              All other error conditions encountered result in an ASSERT

**/

VOID
OemSetPlatformDataValues (
  IN OUT IIO_GLOBALS *IioGlobalData
  )
{

  return;

} // OemSetPlatformDataValues

/**

  OEM Hook before bus number is assigned to DMI (earliest PCI enumeration point) but after chipset DMI
  Initialization occurs (chipset DMI initialization before resources are assigned).

  @param IioGlobalData[in]   Pointer to the IIO Global for this driver
  @param IioIndex[in]        Iio index
  @param PortIndex[in]       Port Index

  @retval VOID           All other error conditions encountered result in an ASSERT

**/

VOID
OemDmiPreInit (
  IN IIO_GLOBALS  *IioGlobalData,
  IN UINT8         IioIndex,
  IN UINT8         PortIndex
  )
{

  return;

} // OemDmiPreInit

/**

  OEM Hook before resources (Bus number is assigned, other resources aren't) are assigned to DMI but after
  chipset DMI Initialization occurs.

  @param IioGlobalData   Pointer to the IIO Global for this driver
  @param IioIndex[in]        Iio index
  @param PortIndex[in]       Port Index

  @retval VOID              All other error conditions encountered result in an ASSERT

**/

VOID
OemDmiDeviceInit (
  IN IIO_GLOBALS  *IioGlobalData,
  IN UINT8         IioIndex,
  IN UINT8         PortIndex
  )
{

  return;

} // OemDmiDeviceInit

/**

  OEM Hook after all Dmi resources are assigned and all DMI Chipset init is complete.

  @param IioGlobalData[in]   Pointer to the IIO Global for this driver
  @param IioIndex[in]        Iio index
  @param PortIndex[in]       Port Index

  @retval VOID              All other error conditions encountered result in an ASSERT

**/

VOID
OemDmiResourceAssignedInit (
  IN IIO_GLOBALS   *IioGlobalData,
  IN UINT8         IioIndex,
  IN UINT8         PortIndex
  )
{

  return;

} // OemDmiResourceAssignedInit

/**

  This function returns the Uplink port details from the IIO
               side, as per the platform board schematics, if no
               uplink port than return 0xFF for both IIO and
               port index.

  @param IioGlobalData[in]    Pointer to the IIO Global for this driver
  @param  IioIndex[in]        Index to Iio
  @param  PortIndex[in]       Index to Iio Port
  @param  Bus[in]             Bus where uplink is located
  @param  Device[in]          Device where uplink is located
  @param  Function[in]        Function where uplink is located

  @retval  CPU index & its port index

 **/

VOID
OemIioUplinkPortDetails (
  IN  IIO_GLOBALS  *IioGlobalData,
  IN  UINT8   IioIndex,
  IN  UINT8   *PortIndex,
  IN  UINT8   *Bus,
  IN  UINT8   *Dev,
  IN  UINT8   *Funcs
  )
{

  return;

} // OemIioUplinkPortDetails

/**

  OEM Hook to apply any Si WA

  @param IioGlobalData[in]         - IIO Global data structure
  @param IioIndex[in]              - Socket Index
  @param Phase[in]                 - Before/After Bifurcation

  @retval  NONE

**/

VOID
OemIioEarlyWorkAround (
  IN IIO_GLOBALS                     *IioGlobalData,
  IN UINT8                           IioIndex,
  IN IIO_INIT_ENUMERATION            Phase
  )
{

  return;

} // OemIioEarlyWorkAround

/**

  OEM Hook to override the IIO DXF settings

  @param IioGlobalData[in]         - IIO Global data structure
  @param IioIndex[in]              - Socket Index

  @retval  NONE

**/

VOID
OemOverrideIioRxRecipeSettings (
  IN IIO_GLOBALS *IioGlobalData,
  IN UINT8       IioIndex
  )
{

  return;

} // OemOverrideIioRxRecipeSettings

/**

  Publish OPAHob for HIFIGen3 driver usage

  @param IioGlobalData[in] - Pointer to IioGlobalData

  @retval VOID

**/

VOID
OemPublishOpaSocketMapHob (
  IN IIO_GLOBALS *IioGlobalData
 )
{

  return;

} // OemPublishOpaSocketMapHob

VOID
OemGetIioPlatformInfo (
  IN  IIO_GLOBALS *IioGlobalData
  )
{

  return;

} // OemGetIioPlatformInfo

/**

  This function checks if we are running under Simics environment

  @param None

  @retval IsSimicsEnvironment  - TRUE/FALSE if Simics


**/

UINT8
CheckSoftSimulationPlatform (
  VOID
  )
{

  return 0;

} // CheckSoftSimulationPlatform

/**

  This function update SRIS configuration settings
  based on the IOUx bifurcation.

  @param IioGlobalData[in]  - Pointer to IioGlobalData
  @param Iou[in]            - Iou
  @param IioIndex[in]       - IioIndex


  @retval None

**/

VOID
OemUpdateSrisConfigSkx (
  IN IIO_GLOBALS   *IioGlobalData,
  IN UINT8          Iou,
  IN UINT8          IioIndex
  )
{

  return;

} // OemUpdateSrisConfigSkx

/**

  This function update SRIS configuration settings
  based on the IOUx bifurcation.

  @param IioGlobalData[in]  - Pointer to IioGlobalData
  @param Iou[in]            - Iou
  @param IioIndex[in]       - IioIndex

  @retval None

**/

VOID
OemUpdateSrisConfig10nm (
  IN IIO_GLOBALS   *IioGlobalData,
  IN UINT8          Iou,
  IN UINT8          IioIndex
  )
{

  return;

} // OemUpdateSrisConfig10nm

/**

  This function update SRIS configuration settings
  based on the IOUx bifurcation.

  @param IioGlobalData[in]  - Pointer to IioGlobalData
  @param Iou[in]            - Iou
  @param IioIndex[in]       - IioIndex

  @retval None

**/

VOID
OemUpdateSrisConfig10nmD (
  IN IIO_GLOBALS   *IioGlobalData,
  IN UINT8            Iou,
  IN UINT8            IioIndex
)
{

  return;

} // OemUpdateSrisConfig10nmD

/**

  This function update the HidePEXPMenu based on PciePort present values and
  do extra setup based on the IIO Features enabled.

  @param IioGlobalData[in]     - Pointer to IioGlobalData

  @retval None

**/

VOID
OemUpdateHidePCIePortMenu (
  IN  IIO_GLOBALS  *IioGlobalData
  )
{

  return;

} // OemUpdateHidePCIePortMenu

/**

  OEM Hook to check for particular support on IIO initialized devices and take action,
  such as install the proper protocol to indicate it is detected

  @param  IioGlobalData[in]  Pointer to the IIO Global for this driver.

  @retval None

**/

VOID
OemCheckForOtherSupport (
  IN IIO_GLOBALS  *IioGlobalData
  )
{

  return;

} // OemCheckForOtherSupport

/**
  This function update the VtdSupport setup item based on x2Apic status from CPU side.
  If CPU threads > 256, x2Apic should be enabled thus Vtd need to be enabled, either.

  @param IioGlobalData[in]     - Pointer to IioGlobalData

  @retval None
**/

VOID
OemUpdateX2ApicSetupItems (
  IN  IIO_GLOBALS  *IioGlobalData
  )
{

  return;

} // OemUpdateX2ApicSetupItems
