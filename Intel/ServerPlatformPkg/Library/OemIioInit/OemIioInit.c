/** @file
  Oem Hooks file for IioInit Module

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#include <Register/PchRegsPmc.h>
#include <Library/MemoryCoreLib.h>
#include <IioSetupDefinitions.h>
#include <RcRegs.h>
#include <Chip/Include/CpuPciAccess.h>
#include <Library/BaseMemoryLib.h>
#include <IioPlatformData.h>
#include <Library/OemIioInitHookLib.h>

/**

  Initialize the Global Data Structure with the IOUx Bifurcation
  with default values

  @param IioGlobalData - Pointer to IioGlobalData

  @retval VOID           All other error conditions encountered result in an ASSERT

**/
VOID
OemDefaultIioPortBifurationInfo (
    IN  IIO_GLOBALS  *IioGlobalData
  )
{
  return;
}


/**

  Initialize the Global Data Structure with the Default Values

  @param IioGlobalData - Pointer to IioGlobalData

  @retval NONE

**/
VOID
OemSetIioDefaultValues (
  IN OUT IIO_GLOBALS *IioGlobalData
  )
{
  return;
}

/**

  OEM Hook before bus number is assigned to DMI (earliest PCI enumeration point) but after chipset DMI
  Initialization occurs (chipset DMI initialization before resources are assigned).

   @param IioGlobalData   Pointer to the IIO Global for this driver

   @retval VOID              All other error conditions
           encountered result in an ASSERT

**/
VOID
OemDmiPreInit (
  IN  IIO_GLOBALS   *IioGlobalData,
  IN UINT8          IioIndex,
  IN UINT8          PortIndex
  )
{
}

/**

  OEM Hook before resources (Bus number is assigned, other resources aren't) are assigned to DMI but after
  chipset DMI Initialization occurs.

   @param IioGlobalData   Pointer to the IIO Global for this driver

   @retval VOID              All other error conditions
           encountered result in an ASSERT

**/
VOID
OemDmiDeviceInit (
  IN  IIO_GLOBALS   *IioGlobalData,
  IN UINT8          IioIndex,
  IN UINT8          PortIndex
  )
{
}

/**

  OEM Hook after all Dmi resources are assigned and all DMI Chipset init is complete.

   @param IioGlobalData   Pointer to the IIO Global for this driver

   @retval VOID              All other error conditions encountered result in an ASSERT

**/
VOID
OemDmiResourceAssignedInit (
  IN  IIO_GLOBALS   *IioGlobalData,
  IN UINT8          IioIndex,
  IN UINT8          PortIndex
  )
{
}


/**

  OEM Hook to apply any Si WA

  @param IioGlobalData         - IIO Global data structure
  @param IioIndex              - Socket Index
  @param Phase                 - Before/After Bifurcation

  @retval  NONE

**/
VOID
OemIioEarlyWorkAround (
    IIO_GLOBALS                     *IioGlobalData,
    UINT8                           IioIndex,
    IIO_INIT_ENUMERATION            Phase
  )
{

  switch(Phase) {
    case IioBeforeBifurcation:
      OemIioPlatformWorkAround  (IioGlobalData, IioIndex);
      break;
    case IioAfterBifurcation:
      break;
    default:
      break;
  }

  return;
}

/**

  OEM Hook to override the IIO DXF settings

  @param IioGlobalData         - IIO Global data structure
  @param IioIndex              - Socket Index
  @param Phase                 - Before/After Bifurcation

  @retval  NONE

**/
VOID
OemOverrideIioRxRecipeSettings (
    IIO_GLOBALS                     *IioGlobalData,
    UINT8                           IioIndex
  )
{

}
