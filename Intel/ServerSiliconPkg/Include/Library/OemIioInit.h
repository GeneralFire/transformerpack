/** @file
  Oem Hooks file for UncoreInit Module

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2017 Intel Corporation. <BR>

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

#ifndef _IIO_OEM_IIO_INIT_H_
#define _IIO_OEM_IIO_INIT_H_

#include <IioPlatformData.h>

VOID
OemDefaultIioPortBifurationInfo(
    IN  IIO_GLOBALS  *IioGlobalData
  );

VOID
OemInitIioPortBifurcationInfo (
  IN  IIO_GLOBALS  *IioGlobalData
  );

VOID
OemSetIioDefaultValues(
  IN OUT IIO_GLOBALS *IioGlobalData
  );

VOID
OemSetPlatformDataValues(
  IN OUT IIO_GLOBALS *IioGlobalData
  );

VOID
OemDmiPreInit (
  IIO_GLOBALS  *IioGlobalData,
  UINT8         IioIndex,
  UINT8         PortIndex
  );

VOID
OemDmiDeviceInit (
  IIO_GLOBALS  *IioGlobalData,
  UINT8         IioIndex,
  UINT8         PortIndex
  );

VOID
OemDmiResourceAssignedInit (
     IIO_GLOBALS   *IioGlobalData,
     UINT8         IioIndex,
     UINT8         PortIndex
    );

VOID
OemIioUplinkPortDetails(
  IN  IIO_GLOBALS  *IioGlobalData,
  IN  UINT8   IioIndex,
  IN  UINT8   *PortIndex,
  IN  UINT8   *Bus,
  IN  UINT8   *Dev,
  IN  UINT8   *Funcs
  );

VOID
OemIioEarlyWorkAround (
    IIO_GLOBALS                     *IioGlobalData,
    UINT8                           IioIndex,
    IIO_INIT_ENUMERATION            Phase
  );

VOID
OemOverrideIioRxRecipeSettings (
    IIO_GLOBALS                     *IioGlobalData,
    UINT8                           IioIndex
  );

VOID
OemPublishOpaSocketMapHob (
  IN IIO_GLOBALS *IioGlobalData
 );

VOID
OemGetIioPlatformInfo(
  IN  IIO_GLOBALS *IioGlobalData
  );

//
// 4926908: SKX not support Vcp
//

UINT8
CheckSoftSimulationPlatform(
  VOID
  );

VOID
OemUpdateSrisConfigSkx(
  IIO_GLOBALS   *IioGlobalData,
  UINT8          Iou,
  UINT8          IioIndex
);

VOID
OemUpdateSrisConfig10nm(
  IIO_GLOBALS   *IioGlobalData,
  UINT8          Iou,
  UINT8          IioIndex
);

VOID
OemUpdateSrisConfig10nmD (
  IN IIO_GLOBALS   *IioGlobalData,
  UINT8            Iou,
  UINT8            IioIndex
);

#ifndef IA32
VOID
OemUpdateHidePCIePortMenu (
  IN  IIO_GLOBALS  *IioGlobalData
  );

VOID
OemCheckForOtherSupport (
  IIO_GLOBALS  *IioGlobalData
  );

VOID
OemUpdateX2ApicSetupItems (
  IN  IIO_GLOBALS  *IioGlobalData
  );
#endif // IA32
#endif //_IIO_OEM_IIO_INIT_H_
