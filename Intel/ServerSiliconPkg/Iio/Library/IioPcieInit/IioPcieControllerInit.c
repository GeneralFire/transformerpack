/** @file
  PCIE controller initialization routines.

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

#include <Library/IioAccessLib.h>
#include <RcRegs.h>
#include <Chip/Include/CpuPciAccess.h>
#include <Library/IioSysInfoLib.h>
#include <IioSetupDefinitions.h>
#include <Library/PciMmio32AssignLib.h>
#include <Library/KtiApi.h>
#include <Library/IioPcieInit.h>
#include <Library/IioDataHubLib.h>
#include <Library/IioNtbInit.h>
#include <LibraryPrivate/IioDmiConf.h>


#include <LibraryPrivate/IioDebug.h>
#include <LibraryPrivate/IioPwrConf.h>
#include <LibraryPrivate/IioPcieLib.h>

#include <Library/CpuAndRevisionLib.h>
#include <LibraryPrivate/IioPwrConf.h>

#include "PcieControllerInit.h"


/**
    This function initialize the PCIE Port link width limits independent of
    the bifurcation configuration.

    @param IioGlobalData              - Pointer to IioGlobalData
    @param IioIndex                   - Iio Index
**/
VOID
InitPcieLinkWidthLimit (
  IN  IIO_GLOBALS                           *IioGlobalData,
  IN  UINT8                                 IioIndex
  )
{

  UINT8        PortIndex;
  UINT8        DefaultMaxLinkWidth;
  UINT8        OverrideMaxLinkWidthOptVal;
  UINT8        OverrideMaxLinkWidth[NUMBER_PORTS_PER_SOCKET];
  UINT8        *ArrayMaxLinkWidth;
  UINT8        MaxPortNumberPerSocket;
  IIO_CONFIG   *SetupData;
  IIO_OUT_DATA *IioOutData;

  SetupData = &IioGlobalData->SetupData;
  IioOutData = &IioGlobalData->IioVar.IioOutData;

  ArrayMaxLinkWidth = &(IioOutData->MaxPXPMap[0]);

  //
  // Collect data about overridden max link widths except Port 13 and 14 (MCP Ports)
  //
  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);
  for (PortIndex = 0; PortIndex < MaxPortNumberPerSocket; PortIndex++) {
    if (IioVData_ConfigurePciePort (IioGlobalData, IioIndex, PortIndex) == FALSE) {
      //check whether the PCIe port can be configured
      continue;
    }

    // Store default max link width
    DefaultMaxLinkWidth = IioOutData->CurrentPXPMap[(IioIndex * MaxPortNumberPerSocket) + PortIndex];

    // Check setup option and choose desired width
    OverrideMaxLinkWidthOptVal = SetupData->PciePortLinkMaxWidth[((IioIndex) * MaxPortNumberPerSocket) + PortIndex];

    //
    // Find option setting
    //
    switch (OverrideMaxLinkWidthOptVal) {
    case 1:
      // x1
      OverrideMaxLinkWidth[PortIndex] = PORT_LINK_WIDTH_x1;
      break;
    case 2:
      // x2
      OverrideMaxLinkWidth[PortIndex] = PORT_LINK_WIDTH_x2;
      break;
    case 3:
      // x4
      OverrideMaxLinkWidth[PortIndex] = PORT_LINK_WIDTH_x4;
      break;
    case 4:
      // x8
      OverrideMaxLinkWidth[PortIndex] = PORT_LINK_WIDTH_x8;
      break;
    case 5:
      // x16
      OverrideMaxLinkWidth[PortIndex] = PORT_LINK_WIDTH_x16;
      break;
    case 0:
    default:
      // Shouldn't happen, set to default bifurcated link width
      OverrideMaxLinkWidth[PortIndex] = DefaultMaxLinkWidth;
      break;
     }

    // Check to make sure Override Max doesn't exceed the max possible by bifurcation
    if (OverrideMaxLinkWidth[PortIndex] > DefaultMaxLinkWidth) {
      OverrideMaxLinkWidth[PortIndex] = DefaultMaxLinkWidth;
    }

    if (DefaultMaxLinkWidth != OverrideMaxLinkWidth[PortIndex]) {
      // Document change of link width, if there is any change
      IIO_D_PCIDBG ("[%d p%d] Link Width forced from default x%d to override x%d\n", IioIndex, PortIndex, DefaultMaxLinkWidth, OverrideMaxLinkWidth[PortIndex]);
    }

    // Don't override if the setup option is auto or already default value
    if ((OverrideMaxLinkWidthOptVal == 0) || (OverrideMaxLinkWidth[PortIndex] == DefaultMaxLinkWidth)) {
      continue;
    }

    ArrayMaxLinkWidth[(IioIndex * MaxPortNumberPerSocket) + PortIndex] = OverrideMaxLinkWidth[PortIndex];
  } //End PortIndex For
}


/**
    This function limits PCIE Port link width independent of
    Bifurcation configuration

    @param IioGlobalData              - Pointer to IioGlobalData
    @param IioIndex                   - Iio Index
**/
VOID
LimitPcieLinkWidth (
  IN  IIO_GLOBALS                           *IioGlobalData,
  IN  UINT8                                 IioIndex
  )
{
  UINT8        PortIndex;
  UINT8        DefaultMaxLinkWidth;
  UINT8        OverrideMaxLinkWidthOptVal;
  UINT8        OverrideMaxLinkWidth;
  UINT8        *ArrayMaxLinkWidth;
  UINT8        *ArrayReverseLink;
  UINT8        MaxPortNumberPerSocket;
  IIO_CONFIG   *SetupData;
  IIO_OUT_DATA *IioOutData;

  SetupData = GetIioConfig ();
  IioOutData = GetIioOutData ();

  //
  // Fetch setup data
  //
  ArrayMaxLinkWidth = &(IioOutData->MaxPXPMap[0]);
  ArrayReverseLink = &(IioOutData->LaneReversedPXPMap[0]);

  //
  // Calculate port masks for slots where max link width has been overridden
  //
  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);
  for (PortIndex = 0; PortIndex < MaxPortNumberPerSocket; PortIndex++) {

    if (!IioVData_ConfigurePciePort (IioGlobalData, IioIndex, PortIndex)) {  //check whether the PCIe port can be configured
      continue;
    }

    DefaultMaxLinkWidth = IioOutData->CurrentPXPMap[(IioIndex * MaxPortNumberPerSocket) + PortIndex];
    OverrideMaxLinkWidthOptVal = SetupData->PciePortLinkMaxWidth[((IioIndex) * MaxPortNumberPerSocket) + PortIndex];
    OverrideMaxLinkWidth = ArrayMaxLinkWidth[(IioIndex * MaxPortNumberPerSocket) + PortIndex];

    if ((OverrideMaxLinkWidthOptVal == 0) || (OverrideMaxLinkWidth == DefaultMaxLinkWidth)) {
      continue;
    }
    if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPort)) {
      //
      // Overriding max link width is not supported on DMI port.
      //
    } else if ((IioPortControllerGen (IioGlobalData, IioIndex, PortIndex) != IIO_GEN3) &&
                CheckNtbPortConfSetting (IioIndex, PortIndex)) {
      //
      // On Pcie Gen3 controller limitation of link width for Ntb is handled by IioPcieMaxLinkWidth()
      //
      IioNtbMaxLinkWidth (IioGlobalData, IioIndex, PortIndex, DefaultMaxLinkWidth, OverrideMaxLinkWidth);
    } else {
      IioPcieMaxLinkWidth (IioGlobalData, IioIndex, PortIndex, DefaultMaxLinkWidth, OverrideMaxLinkWidth);
    }
  } // end PortIndex For
}

