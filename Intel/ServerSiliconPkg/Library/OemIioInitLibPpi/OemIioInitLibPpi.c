/** @file
  This library class provides Iio Platform Hooks Ppi wrapper.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2018 Intel Corporation. <BR>

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

#include <Uefi.h>
#include <PiPei.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Ppi/OemIioHooksPpi.h>

/**


This function checks Whether OEM IIO Init Hooks PPI initialized or not

@param mOemIioHooksPpi - Pointer to Oem Iio Hooks PPI

@ret TRUE  If Oem Iio Hooks PPI is located successfully.
@ret FALSE  If Oem Iio Hooks PPI is not located successfully.

**/
BOOLEAN
IsOemIiomHooksPpiReadyIio(
  OEM_IIO_HOOKS_PPI   **mOemIioHooksPpi
)
{
  BOOLEAN         Result = FALSE;
  EFI_STATUS      Status;

  Status = PeiServicesLocatePpi(
              &gPeiOemIioHooksPpiGuid,
              0,
              NULL,
              mOemIioHooksPpi
            );
  if (Status == EFI_SUCCESS) {
    Result = TRUE;
  }

  return Result;
}


VOID
OemGetIioPlatformInfo (
  IIO_GLOBALS *IioGlobalData
  )
{
  OEM_IIO_HOOKS_PPI     *mOemIioHooksPpiInstance;
  if (IsOemIiomHooksPpiReadyIio (&mOemIioHooksPpiInstance)) {
    mOemIioHooksPpiInstance->OemGetIioPlatformInfo(IioGlobalData);
  }
}

VOID
OemIioUplinkPortDetails (
  IIO_GLOBALS   *IioGlobalData,
  UINT8         IioIndex,
  UINT8         *PortIndex,
  UINT8         *Bus,
  UINT8         *Dev,
  UINT8         *Func
  )
{
  OEM_IIO_HOOKS_PPI     *mOemIioHooksPpiInstance;
  if (IsOemIiomHooksPpiReadyIio (&mOemIioHooksPpiInstance)) {
    mOemIioHooksPpiInstance->OemIioUplinkPortDetails(IioGlobalData, IioIndex, PortIndex, Bus, Dev, Func);
  }
}

VOID
OemIioEarlyWorkAround (
    IIO_GLOBALS                     *IioGlobalData,
    UINT8                           IioIndex,
    IIO_INIT_ENUMERATION            Phase
  )
{
  OEM_IIO_HOOKS_PPI     *mOemIioHooksPpiInstance;
  if (IsOemIiomHooksPpiReadyIio (&mOemIioHooksPpiInstance)) {
    mOemIioHooksPpiInstance->OemIioEarlyWorkAround(IioGlobalData, IioIndex, Phase);
  }
}

VOID
OemOverrideIioRxRecipeSettings (
    IIO_GLOBALS                     *IioGlobalData,
    UINT8                           IioIndex
  )
{
  OEM_IIO_HOOKS_PPI     *mOemIioHooksPpiInstance;
  if (IsOemIiomHooksPpiReadyIio (&mOemIioHooksPpiInstance)) {
    mOemIioHooksPpiInstance->OemOverrideIioRxRecipeSettings(IioGlobalData, IioIndex);
  }
}

VOID
OemDefaultIioPortBifurationInfo (
  IN  IIO_GLOBALS  *IioGlobalData
  )
{
  OEM_IIO_HOOKS_PPI     *mOemIioHooksPpiInstance;
  if (IsOemIiomHooksPpiReadyIio (&mOemIioHooksPpiInstance)) {
    mOemIioHooksPpiInstance->OemDefaultIioPortBifurationInfo(IioGlobalData);
  }
}

VOID
OemInitIioPortBifurcationInfo (
  IN  IIO_GLOBALS  *IioGlobalData
  )
{
  OEM_IIO_HOOKS_PPI     *mOemIioHooksPpiInstance;
  if (IsOemIiomHooksPpiReadyIio (&mOemIioHooksPpiInstance)) {
    mOemIioHooksPpiInstance->OemInitIioPortBifurcationInfo(IioGlobalData);
  }
}

VOID
OemDmiPreInit (
  IN IIO_GLOBALS    *IioGlobalData,
  IN UINT8          IioIndex,
  IN UINT8          PortIndex
  )
{
  OEM_IIO_HOOKS_PPI     *mOemIioHooksPpiInstance;
  if (IsOemIiomHooksPpiReadyIio (&mOemIioHooksPpiInstance)) {
    mOemIioHooksPpiInstance->OemDmiPreInit(IioGlobalData, IioIndex, PortIndex);
  }
}

VOID
OemDmiDeviceInit (
  IN IIO_GLOBALS    *IioGlobalData,
  IN UINT8          IioIndex,
  IN UINT8          PortIndex
  )
{
  OEM_IIO_HOOKS_PPI     *mOemIioHooksPpiInstance;
  if (IsOemIiomHooksPpiReadyIio (&mOemIioHooksPpiInstance)) {
    mOemIioHooksPpiInstance->OemDmiDeviceInit(IioGlobalData, IioIndex, PortIndex);
  }
}

VOID
OemDmiResourceAssignedInit (
  IN IIO_GLOBALS    *IioGlobalData,
  IN UINT8          IioIndex,
  IN UINT8          PortIndex
  )
{
  OEM_IIO_HOOKS_PPI     *mOemIioHooksPpiInstance;
  if (IsOemIiomHooksPpiReadyIio (&mOemIioHooksPpiInstance)) {
    mOemIioHooksPpiInstance->OemDmiResourceAssignedInit(IioGlobalData, IioIndex, PortIndex);
  }
}

VOID
OemSetIioDefaultValues (
  IN  IIO_GLOBALS  *IioGlobalData
  )
{
  OEM_IIO_HOOKS_PPI     *mOemIioHooksPpiInstance;
  if (IsOemIiomHooksPpiReadyIio (&mOemIioHooksPpiInstance)) {
    mOemIioHooksPpiInstance->OemSetIioDefaultValues(IioGlobalData);
  }
}

VOID
OemSetPlatformDataValues (
  IN  IIO_GLOBALS  *IioGlobalData
  )
{
  OEM_IIO_HOOKS_PPI     *mOemIioHooksPpiInstance;
  if (IsOemIiomHooksPpiReadyIio (&mOemIioHooksPpiInstance)) {
    mOemIioHooksPpiInstance->OemSetPlatformDataValues(IioGlobalData);
  }
}

VOID
OemPublishOpaSocketMapHob (
  IN  IIO_GLOBALS  *IioGlobalData
 )
{
  OEM_IIO_HOOKS_PPI     *mOemIioHooksPpiInstance;
  if (IsOemIiomHooksPpiReadyIio (&mOemIioHooksPpiInstance)) {
    mOemIioHooksPpiInstance->OemPublishOpaSocketMapHob(IioGlobalData);
  }
}
