/** @file

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

#include <PiPei.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <IioPlatformData.h>
#include <IioRegs.h>
#include <IioSetupDefinitions.h>
#include <Guid/IioPolicyHob.h>
#include <Chip/Include/CpuPciAccess.h>
#include "OemIioInitCommon.h"
#include <Library/BaseMemoryLib.h>

VOID
OemGetIioPlatformInfo (
  IN IIO_GLOBALS *IioGlobalData
  )
{
  return;
}

/**

  This function returns the Uplink port details from the IIO
  side, as per the platform board schematics, if no
  uplink port then return 0xFF for both IIO and
  port index.

  @param[in]   IioGlobalData - Pointer to IioGlobalData
  @param[in]   IioIndex - Index to Iio

  @param[out]  PortIndex - Index to Iio Port
  @param[out]  Bus       - Bus where uplink is located
  @param[out]  Device    - Device where uplink is located
  @param[out]  Function  - Function where uplink is located

**/
VOID
OemIioUplinkPortDetails (
  IN   IIO_GLOBALS  *IioGlobalData,
  IN   UINT8   IioIndex,
  OUT  UINT8   *PortIndex,
  OUT  UINT8   *Bus,
  OUT  UINT8   *Dev,
  OUT  UINT8   *Func
  )
{
  UINT8       MaxPortNumberPerSocket;
  *PortIndex = 0xFF;
  *Bus = 0xFF;
  *Dev = 0xFF;
  *Func = 0xFF;

  if (IioIndex == 0) {
    *PortIndex = PcdGet8 (PcdOemSkuUplinkPortIndex);
    if (*PortIndex != 0xFF) {
      MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];
      if (*PortIndex < ARRAY_SIZE (IioGlobalData->IioVar.IioVData.SocketPortBusNumber[IioIndex]) && (*PortIndex < MaxPortNumberPerSocket)) {
        *Bus = IioGlobalData->IioVar.IioVData.SocketPortBusNumber[IioIndex][*PortIndex];
        *Dev = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[*PortIndex].Device;
        *Func = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[*PortIndex].Function;
      }
    }
  }
  return;
}

/**

    This function checks if we are running under Simics environment

    @param None

    @retval IsSimicsEnvironment  - TRUE/FALSE if Simics


**/
UINT8
CheckSoftSimulationPlatform(
  VOID
  )
{
  return (UINT8)IsSimicsEnvironment();
}



/**

    This function update SRIS configuration settings
    based on the IOUx bifurcation.

    @param IioGlobalData - Pointer to IioGlobalData

    @retval None

**/
VOID
OemUpdateSrisConfigSkx(
  IN IIO_GLOBALS   *IioGlobalData,
  UINT8            Iou,
  UINT8            IioIndex
  )
{
  UINT8 MaxPortNumberPerSocket;
  UINT8 PXPOffset;
  UINT8 CurrentIOUConfigValue = 0;

  MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];

  PXPOffset = (IioIndex * MaxPortNumberPerSocket);
  switch (Iou) {
    case IioIou0:
      CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU[IioIndex][0];
      PXPOffset+= PORT_1A_INDEX;
      break;
    case IioIou1:
      CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU[IioIndex][1];
      PXPOffset+= PORT_2A_INDEX;
      break;
    case IioIou2:
      CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU[IioIndex][2];
      PXPOffset+= PORT_3A_INDEX;
      break;
    case IioMcp0:
      CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU[IioIndex][3];
      PXPOffset+= PORT_4A_INDEX;
      break;
    case IioMcp1:
      CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU[IioIndex][4];
      PXPOffset += PORT_5A_INDEX;
      break;
    default:
      DEBUG ((DEBUG_WARN, "default case.\n"));  //Auto added. Please review.
      break;
  }// switch Iou

  DEBUG ((DEBUG_INFO, "SRIS: Iou = %x, CurrentIOUConfigValue = %x \n", Iou, CurrentIOUConfigValue));

  switch(CurrentIOUConfigValue){
    case IIO_BIFURCATE_x4x4xxx8:
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortA] == IIO_OPTION_ENABLE){
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_DISABLE;
      }
      break;
    case IIO_BIFURCATE_xxx8x4x4:
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortC] == IIO_OPTION_ENABLE){
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_DISABLE;
      }
      break;
    case IIO_BIFURCATE_xxx8xxx8:
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortA] == IIO_OPTION_ENABLE){
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_DISABLE;
      }
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortC] == IIO_OPTION_ENABLE){
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_DISABLE;
      }
      break;
    case IIO_BIFURCATE_xxxxxx16:
      if( IioGlobalData->SetupData.SRIS[PXPOffset + IioPortA] == IIO_OPTION_ENABLE){
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_ENABLE;
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortC] = IIO_OPTION_ENABLE;
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_DISABLE;
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortC] = IIO_OPTION_DISABLE;
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_DISABLE;
      }
      break;
    default:
      // Do nothing we will conserve user configuration
      break;
  }// CurrentIOUConfigValue
}

VOID
OemUpdateSrisConfig10nm (
  IN IIO_GLOBALS   *IioGlobalData,
  UINT8            Iou,
  UINT8            IioIndex
  )
{
  UINT8 MaxPortNumberPerSocket;
  UINT8 PXPOffset;
  UINT8 CurrentIOUConfigValue = 0;

  MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];;

  PXPOffset = IioIndex * MaxPortNumberPerSocket;
  switch (Iou) {
    case IioIou010nm:
      CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU[IioIndex][0];
      PXPOffset += PORT_1A_INDEX_1;
      break;
    case IioIou110nm:
      CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU[IioIndex][1];
      PXPOffset += PORT_2A_INDEX_2;
      break;
    case IioIou210nm:
      CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU[IioIndex][2];
      PXPOffset += PORT_3A_INDEX_3;
      break;
    case IioIou310nm:
      CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU[IioIndex][3];
      PXPOffset += PORT_4A_INDEX_4;
      break;
    case IioIou410nm:
      CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU[IioIndex][4];
      PXPOffset += PORT_5A_INDEX_5;
      break;
    default:
      DEBUG ((EFI_D_WARN, "default case.\n"));  //Auto added. Please review.
      break;
  }// switch Iou

  DEBUG ((DEBUG_INFO, "SRIS: Iou = %x, CurrentIOUConfigValue = %x\n", Iou, CurrentIOUConfigValue));

  switch (CurrentIOUConfigValue) {
    case IIO_BIFURCATE_x4x4xxx8:
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortA] == IIO_OPTION_ENABLE) {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_DISABLE;
      }
      break;
    case IIO_BIFURCATE_xxx8x4x4:
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortC] == IIO_OPTION_ENABLE) {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_DISABLE;
      }
      break;
    case IIO_BIFURCATE_xxx8xxx8:
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortA] == IIO_OPTION_ENABLE) {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_DISABLE;
      }
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortC] == IIO_OPTION_ENABLE) {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_DISABLE;
      }
      break;
    case IIO_BIFURCATE_xxxxxx16:
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortA] == IIO_OPTION_ENABLE) {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_ENABLE;
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortC] = IIO_OPTION_ENABLE;
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_DISABLE;
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortC] = IIO_OPTION_DISABLE;
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_DISABLE;
      }
      break;
    default:
      // Do nothing we will conserve user configuration
      break;
  }// CurrentIOUConfigValue
}

VOID
OemUpdateSrisConfig10nmD (
  IN IIO_GLOBALS   *IioGlobalData,
  UINT8            Iou,
  UINT8            IioIndex
  )
{
  UINT8 MaxPortNumberPerSocket;
  UINT8 PXPOffset;
  UINT8 CurrentIOUConfigValue = 0;

  MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];;

  PXPOffset = IioIndex * MaxPortNumberPerSocket;
  switch (Iou) {
    case IioIou010nm:
      CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU[IioIndex][0];
      PXPOffset+= PORT_1A_INDEX_S1;
      break;
    case IioIou110nm:
      CurrentIOUConfigValue = IioGlobalData->SetupData.ConfigIOU[IioIndex][1];
      PXPOffset+= PORT_2A_INDEX_S1;
      break;
    default:
      break;
  }// switch Iou

  DEBUG ((DEBUG_INFO, "SRIS: Iou = %x, CurrentIOUConfigValue = %x\n", Iou, CurrentIOUConfigValue));

  switch (CurrentIOUConfigValue) {
    case IIO_BIFURCATE_x4x4xxx8:
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortA] == IIO_OPTION_ENABLE) {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_DISABLE;
      }
      break;
    case IIO_BIFURCATE_xxx8x4x4:
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortC] == IIO_OPTION_ENABLE) {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_DISABLE;
      }
      break;
    case IIO_BIFURCATE_xxx8xxx8:
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortA] == IIO_OPTION_ENABLE) {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_DISABLE;
      }
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortC] == IIO_OPTION_ENABLE) {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_DISABLE;
      }
      break;
    case IIO_BIFURCATE_xxxxxx16:
      if (IioGlobalData->SetupData.SRIS[PXPOffset + IioPortA] == IIO_OPTION_ENABLE) {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_ENABLE;
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortC] = IIO_OPTION_ENABLE;
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_ENABLE;
      } else {
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortB] = IIO_OPTION_DISABLE;
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortC] = IIO_OPTION_DISABLE;
        IioGlobalData->SetupData.SRIS[PXPOffset + IioPortD] = IIO_OPTION_DISABLE;
      }
      break;
    default:
      // Do nothing we will conserve user configuration
      break;
  }// CurrentIOUConfigValue
}

/**

  Initialize the Global Data Structure with the IOUx Bifurcation
  based on Board layout or user selection

  @param IioGlobalData - Pointer to IioGlobalData

  @retval VOID              All other error conditions encountered result in an ASSERT

**/
VOID
OemInitIioPortBifurcationInfo (
  IN  IIO_GLOBALS  *IioGlobalData
  )
{

return;
}

/**

  Initialize the Global Data Structure with the Setup Values
  read from NVRAM

  @param IioGlobalData - Pointer to IioGlobalData

  @retval VOID              All other error conditions encountered result in an ASSERT

**/
VOID
OemSetPlatformDataValues (
  IN OUT IIO_GLOBALS *IioGlobalData
  )
{
return;
}

/**

  Publish OPAHob for HIFIGen3 driver usage

  @param IioGlobalData - Pointer to IioGlobalData

  @retval VOID

**/
VOID
OemPublishOpaSocketMapHob (
  IN IIO_GLOBALS *IioGlobalData
 )
{
  OPA_SOCKET_MAP_HOB    OpaSocketMapHob;
  VOID               *HobPtr;
  UINT8               Socket;

  ZeroMem(&OpaSocketMapHob, sizeof(OPA_SOCKET_MAP_HOB));

  for (Socket = 0; Socket < MaxIIO; Socket++) {
    if (IioGlobalData->IioVar.IioVData.SocketPresent[Socket] == 0){
      continue;
    }
    OpaSocketMapHob.Info[Socket].Valid = 1;
    OpaSocketMapHob.Info[Socket].SocketId = Socket;
    OpaSocketMapHob.Info[Socket].Segment = IioGlobalData->IioVar.IioVData.SegmentSocket[Socket];
    //
    // Only send the MCP (PSTACK3) Bus range
    //
    OpaSocketMapHob.Info[Socket].BusBase = IioGlobalData->IioVar.IioVData.SocketStackBaseBusNumber[Socket][IIO_PSTACK3];
    OpaSocketMapHob.Info[Socket].BusLimit = IioGlobalData->IioVar.IioVData.SocketStackLimitBusNumber[Socket][IIO_PSTACK3];
  }

  HobPtr = BuildGuidDataHob (&gEfiOpaSocketMapHobGuid, &OpaSocketMapHob, sizeof (OPA_SOCKET_MAP_HOB));
  if(HobPtr == NULL){
    DEBUG ((DEBUG_ERROR, "ERROR: gEfiOpaSocketMapHobGuid could not be Published\n"));
  } else {
    DEBUG ((DEBUG_INFO, "gEfiOpaSocketMapHobGuid Published!!!\n"));
  }

}
