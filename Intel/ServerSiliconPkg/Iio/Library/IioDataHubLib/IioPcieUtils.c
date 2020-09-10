/** @file

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

#include <Base.h>
#include <Uefi.h>

#include <Library/CpuAndRevisionLib.h>
#include <IioRegs.h>
#include <Library/IioDataHubLib.h>
#include <Library/IioSysInfoLib.h>

typedef struct {
  UINT8     LanesInPort[8];
} PORT_LANES_TABLE;

PORT_LANES_TABLE  PI5CommonLanesTable[] = {
//Port A,B,C,D,E,F,G,H
      {4,0,4,0,4,0,4,0},  // IIO_BIFURCATE_x4x4x4x4,
      {8,0,0,0,4,0,4,0},  // IIO_BIFURCATE_x4x4xxx8,
      {4,0,4,0,8,0,0,0},  // IIO_BIFURCATE_xxx8x4x4,
      {8,0,0,0,8,0,0,0},  // IIO_BIFURCATE_xxx8xxx8,
      {16,0,0,0,0,0,0,0}, // IIO_BIFURCATE_xxxxxx16,
      {8,0,0,0,4,0,2,2},  // IIO_BIFURCATE_x2x2x4x8,
      {8,0,0,0,2,2,4,0},  // IIO_BIFURCATE_x4x2x2x8,
      {4,0,2,2,8,0,0,0},  // IIO_BIFURCATE_x8x2x2x4,
      {2,2,4,0,8,0,0,0},  // IIO_BIFURCATE_x8x4x2x2,
      {4,0,4,0,4,0,2,2},  // IIO_BIFURCATE_x2x2x4x4x4,
      {4,0,4,0,2,2,4,0},  // IIO_BIFURCATE_x4x2x2x4x4,
      {4,0,2,2,4,0,4,0},  // IIO_BIFURCATE_x4x4x2x2x4,
      {2,2,4,0,4,0,4,0},  // IIO_BIFURCATE_x4x4x4x2x2,
      {8,0,0,0,2,2,2,2},  // IIO_BIFURCATE_x2x2x2x2x8,
      {2,2,2,2,8,0,0,0},  // IIO_BIFURCATE_x8x2x2x2x2,
      {4,0,4,0,2,2,2,2},  // IIO_BIFURCATE_x2x2x2x2x4x4,
      {4,0,2,2,4,0,2,2},  // IIO_BIFURCATE_x2x2x4x2x2x4,
      {2,2,4,0,4,0,2,2},  // IIO_BIFURCATE_x2x2x4x4x2x2,
      {4,0,2,2,2,2,4,0},  // IIO_BIFURCATE_x4x2x2x2x2x4,
      {2,2,4,0,2,2,4,0},  // IIO_BIFURCATE_x4x2x2x4x2x2,
      {2,2,2,2,4,0,4,0},  // IIO_BIFURCATE_x4x4x2x2x2x2,
      {4,0,2,2,2,2,2,2},  // IIO_BIFURCATE_x2x2x2x2x2x2x4,
      {2,2,4,0,2,2,2,2},  // IIO_BIFURCATE_x2x2x2x2x4x2x2,
      {2,2,2,2,4,0,2,2},  // IIO_BIFURCATE_x2x2x4x2x2x2x2,
      {2,2,2,2,2,2,4,0},  // IIO_BIFURCATE_x4x2x2x2x2x2x2,
      {2,2,2,2,2,2,2,2}   // IIO_BIFURCATE_x2x2x2x2x2x2x2x2,
};

PORT_LANES_TABLE  PI5Gen5NtbLanesTable[] = {
//Port A,B,C,D,E,F,G,H    // NTB port always in port H and x2 not supported
      {4,0,4,0,4,0,0,4},  // IIO_BIFURCATE_x4x4x4x4,
      {8,0,0,0,4,0,0,4},  // IIO_BIFURCATE_x4x4xxx8,
      {4,0,4,0,0,0,0,8},  // IIO_BIFURCATE_xxx8x4x4,
      {8,0,0,0,0,0,0,8},  // IIO_BIFURCATE_xxx8xxx8,
      {0,0,0,0,0,0,0,16}, // IIO_BIFURCATE_xxxxxx16,
      {8,0,0,0,4,0,2,0},  // IIO_BIFURCATE_x2x2x4x8,
      {8,0,0,0,2,2,0,4},  // IIO_BIFURCATE_x4x2x2x8,
      {4,0,2,2,0,0,0,8},  // IIO_BIFURCATE_x8x2x2x4,
      {2,2,4,0,0,0,0,8},  // IIO_BIFURCATE_x8x4x2x2,
      {4,0,4,0,4,0,2,0},  // IIO_BIFURCATE_x2x2x4x4x4,
      {4,0,4,0,2,2,0,4},  // IIO_BIFURCATE_x4x2x2x4x4,
      {4,0,2,2,4,0,0,4},  // IIO_BIFURCATE_x4x4x2x2x4,
      {2,2,4,0,4,0,0,4},  // IIO_BIFURCATE_x4x4x4x2x2,
      {8,0,0,0,2,2,2,0},  // IIO_BIFURCATE_x2x2x2x2x8,
      {2,2,2,2,0,0,0,8},  // IIO_BIFURCATE_x8x2x2x2x2,
      {4,0,4,0,2,2,2,0},  // IIO_BIFURCATE_x2x2x2x2x4x4,
      {4,0,2,2,4,0,2,0},  // IIO_BIFURCATE_x2x2x4x2x2x4,
      {2,2,4,0,4,0,2,0},  // IIO_BIFURCATE_x2x2x4x4x2x2,
      {4,0,2,2,2,2,0,4},  // IIO_BIFURCATE_x4x2x2x2x2x4,
      {2,2,4,0,2,2,0,4},  // IIO_BIFURCATE_x4x2x2x4x2x2,
      {2,2,2,2,4,0,0,4},  // IIO_BIFURCATE_x4x4x2x2x2x2,
      {4,0,2,2,2,2,2,0},  // IIO_BIFURCATE_x2x2x2x2x2x2x4,
      {2,2,4,0,2,2,2,0},  // IIO_BIFURCATE_x2x2x2x2x4x2x2,
      {2,2,2,2,4,0,2,0},  // IIO_BIFURCATE_x2x2x4x2x2x2x2,
      {2,2,2,2,2,2,0,4},  // IIO_BIFURCATE_x4x2x2x2x2x2x2,
      {2,2,2,2,2,2,2,0}   // IIO_BIFURCATE_x2x2x2x2x2x2x2x2,
};


PORT_LANES_TABLE  PI5Gen4OnlyLanesTable[] = {
//Port A,B,C,D,E,F,G,H
      {0,4,0,4,0,4,0,4}, // IIO_BIFURCATE_x4x4x4x4,
      {0,0,0,8,0,4,0,4}, // IIO_BIFURCATE_x4x4xxx8,
      {0,4,0,4,0,0,0,8}, // IIO_BIFURCATE_xxx8x4x4,
      {0,0,0,8,0,0,0,8}, // IIO_BIFURCATE_xxx8xxx8,
      {0,0,0,0,0,0,0,16} // IIO_BIFURCATE_xxxxxx16,
};

PORT_LANES_TABLE  FourPortsLanesTable[] = {
//Port A,B,C,D,-,-,-,-
      {4,4,4,4,0,0,0,0}, // IIO_BIFURCATE_x4x4x4x4,
      {8,0,4,4,0,0,0,0}, // IIO_BIFURCATE_x4x4xxx8,
      {4,4,8,0,0,0,0,0}, // IIO_BIFURCATE_xxx8x4x4,
      {8,0,8,0,0,0,0,0}, // IIO_BIFURCATE_xxx8xxx8,
      {16,0,0,0,0,0,0,0} // IIO_BIFURCATE_xxxxxx16,
};

/**
  Returns number of lanes assigned to given port based on this port bifurcation

  @param[in]  Bifurcation         Bifurcation of stack
  @param[in]  PortInStackIndex    Index of the port in this stack
  @param[in]  PortGen             Type of stack (PCIE controller version)
  @param[in]  PI5Mode             Mode of the PI5 controller (value ignored for non-PI5)

  @return     number of lanes number assigned to given port
 */
UINT8
IioGetLanesNumOfPortInStack (
  IN  UINT8         Bifurcation,
  IN  UINT8         PortInStackIndex,
  IN  IIO_PORT_GEN  PortGen,
  IN  PI5_MODES     PI5Mode
  )
{
  //
  // Validate port index in stack
  //
  if (PortInStackIndex >= MAX_IIO_PORTS_PER_STACK) {
    ASSERT (FALSE);
    return 0;
  }

  switch (PortGen) {
  case IIO_GEN3:
  case IIO_GEN4:
    if (Bifurcation >= NELEMENTS(FourPortsLanesTable)) {
      ASSERT (FALSE);
      return 0;
    }
    return FourPortsLanesTable[Bifurcation].LanesInPort[PortInStackIndex];
  case IIO_GEN5:
    switch (PI5Mode) {
    case PI5_MODE_GEN5_AND_GEN4:
    case PI5_MODE_GEN5_ONLY:
      if (Bifurcation >= NELEMENTS(PI5CommonLanesTable)) {
        ASSERT (FALSE);
        return 0;
      }
      return PI5CommonLanesTable[Bifurcation].LanesInPort[PortInStackIndex];
      break;
    case PI5_MODE_NTB:
      if (Bifurcation >= NELEMENTS(PI5Gen5NtbLanesTable)) {
        ASSERT (FALSE);
        return 0;
      }
      return PI5Gen5NtbLanesTable[Bifurcation].LanesInPort[PortInStackIndex];
      break;
    case PI5_MODE_GEN4_ONLY:
    case PI5_MODE_NTB_GEN4:
      if (Bifurcation >= NELEMENTS(PI5Gen4OnlyLanesTable)) {
        ASSERT (FALSE);
        return 0;
      }
      return PI5Gen4OnlyLanesTable[Bifurcation].LanesInPort[PortInStackIndex];
      break;

    case PI5_MODE_DMI:
      //
      // DMI has always 8 lanes for SPR
      //
      return 8;
      break;
    case PI5_MODE_IAL:
    default:
      //
      // TODO: Implementation is not ready here
      //
      return 0;
    }
    break;
  default:
    //
    // Implementation is not ready here
    //
    ASSERT (FALSE);
    return 0;
  } // switch IIO_PORT_GEN
}

/**
  Returns number of lanes assigned to given port based on this port bifurcation

  @param[in]  IioGlobalData  Pointer to IIO globals
  @param[in]  IioIndex       Socket number of the port
  @param[in]  PortIndex      Port number

  @return     number of lanes assigned to given port
 */
UINT8
IioGetLanesNumOfPort (
  IN  IIO_GLOBALS  *IioGlobalData,
  IN  UINT8        IioIndex,
  IN  UINT8        PortIndex
  )
{
  UINT8         PortInStackIndex;
  UINT8         Bifurcation;
  UINT8         Stack;
  UINT8         Iou;
  IIO_PORT_GEN  PortGen;
  PI5_MODES     Pi5Mode;

  PortInStackIndex = IioPortIndexToStackPort (PortIndex, &Stack);
  if (Stack >= MAX_STACKS_PER_SOCKET || PortInStackIndex >= MAX_IIO_PORTS_PER_STACK) {
    ASSERT (FALSE);
    return 0;
  }

  if (!IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    if (IsDmiStack (Stack)) {
      if (IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
        return 16;
      } else {
        return 4;
      }
    }
    Iou = Stack -1;

    //
    // If statement to resolve KW issue
    //
    if (Iou >= NELEMENTS (IioGlobalData->SetupData.ConfigIOU[IioIndex])) {
      ASSERT (FALSE);
      return 0;
    }

    Bifurcation = IioGlobalData->SetupData.ConfigIOU[IioIndex][Iou];
    PortGen = IioPortControllerGen (IioGlobalData, IioIndex, PortIndex);
    if (Bifurcation == IIO_BIFURCATE_xxxxxxxx || Bifurcation == IIO_BIFURCATE_AUTO) {
      DEBUG ((DEBUG_ERROR, "[%d p%d] %a: Wrong bifurcation = %X\n", IioIndex, PortIndex, __FUNCTION__, Bifurcation));
      ASSERT (FALSE);
      return FALSE;
    }

    return IioGetLanesNumOfPortInStack (Bifurcation, PortInStackIndex, PortGen, PI5_MODE_MAX);
  } else { // SPR version

    if (IsDmiStack (Stack)) {
      //
      // Dmi stack has always 8 lanes
      //
      return 8;
    }
    Iou = Stack -1;

    //
    // If statement to resolve KW issue
    //
    if (Iou >= NELEMENTS (IioGlobalData->SetupData.ConfigIOU[IioIndex])) {
      ASSERT (FALSE);
      return 0;
    }

    Bifurcation = IioGlobalData->SetupData.ConfigIOU[IioIndex][Iou];
    Pi5Mode = GetStackMode (IioGlobalData, IioIndex, Stack);
    return IioGetLanesNumOfPortInStack (Bifurcation, PortInStackIndex, IIO_GEN5, Pi5Mode);
  }
}

/**
  Returns TRUE if any lane is assigned to given port.

  @param[in]  IioGlobalData  Pointer to IIO globals
  @param[in]  IioIndex       Socket number of the port
  @param[in]  PortIndex      Port number

  @return     TRUE if any lane is assigned to given port
 */
BOOLEAN
IioAreLanesAssignedToPort (
  IN  IIO_GLOBALS  *IioGlobalData,
  IN  UINT8        IioIndex,
  IN  UINT8        PortIndex
  )
{
  return IioGetLanesNumOfPort (IioGlobalData, IioIndex, PortIndex) > 0;
}

/**
  Returns TRUE if HotPlug is enabled for given port

  @param[in]  IioIndex       Socket number of the port
  @param[in]  PortIndex      Port number
 */
BOOLEAN
IioIsHpEnabledOnPort (
  IN  UINT8        IioIndex,
  IN  UINT8        PortIndex
  )
{
  UINT32      MaxPortNumberPerSocket;
  UINT32      TotalPortIndex;
  IIO_CONFIG  *IioSetupData;

  IioSetupData = GetIioConfig ();
  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);
  TotalPortIndex = IioIndex * MaxPortNumberPerSocket + PortIndex;

  if (IioSetupData->IioPcieConfig.PcieHotPlugEnable == IIO_OPTION_DISABLE) {
    return FALSE;
  }
  switch (IioSetupData->PcieHotPlugOnPort[IioIndex][PortIndex]) {
  case IIO_OPTION_AUTO:
    return IioSetupData->SLOTHPCAP[TotalPortIndex] == 1;
  case IIO_OPTION_ENABLE:
    return TRUE;
  case IIO_OPTION_DISABLE:
    return FALSE;
  default:
    ASSERT(FALSE);
    return FALSE;
  }
}

