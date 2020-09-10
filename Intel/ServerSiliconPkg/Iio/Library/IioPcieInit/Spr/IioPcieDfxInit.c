/** @file

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

#include <Base.h>
#include <Uefi.h>
#include <Library/DebugLib.h>

#include <Library/OemIioInit.h>
#include <Library/IioAccessLib.h>
#include <Chip/Include/EvAutoRecipe.h>
#include <RcRegs.h>
#include <Chip/Include/SysHostChip.h>
#include "IioRecipeInit.h"
#include <Chip/Include/CpuPciAccess.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/IioDataHubLib.h>
#include <IioSetupDefinitions.h>
#include <LibraryPrivate/IioPcieLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/PreSiliconEnvDetectLib.h>


/**

    IioPrePhyRecipeInit: IIO Pre PHY recipe programming, before PHY Rx recipe settings (if any)

    @param IioIndex           - Index of CPU/IIO

    @retval None

**/
VOID IioPrePhyRecipeInit (
    UINT8                              IioIndex
    )
{
  return;
}

/**

    IioPostPhyRecipeInit: IIO Post PHY recipe programming, after PHY Rx recipe settings (if any)

    @param IioIndex           - Index of CPU/IIO

    @retval None

**/
VOID
IioPostPhyRecipeInit (
    UINT8                               IioIndex
    )
{
  return;
}

/**
  DfxPcieInit: Contains functions for programming Rx, Tx, Isoc parameters and some WA.

  @param[in] IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
  @param[in] IioIndex           - Index of IIO/CPU
**/
VOID
DfxEarlyInit (
  IN  IIO_GLOBALS                         *IioGlobalData,
  IN  UINT8                               IioIndex
  )
{
  UINT8                                 Stack;
  UINT8                                 PortIndex;
  UINT8                                 MaxPortNumberPerSocket;
  IIO_CONFIG                            *SetupData;
  IIO_V_DATA                            *IioVData;
  IIO_OUT_DATA                          *IioOutData;
  SPARE_SWWEST2_IIO_DFX_VTD_STRUCT      SpareSwwest2;

  SetupData = GetIioConfig ();
  IioVData = GetIioVData ();
  IioOutData = GetIioOutData ();

  for(Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
    if(!(IioVData->StackPresentBitmap[IioIndex] & (1<<Stack))){
      continue;
    }
    //
    // Relax Ordering override programming
    //
    SpareSwwest2.Data = IioReadCpuCsr32 (IioIndex, Stack, SPARE_SWWEST2_IIO_DFX_VTD_REG);
    if (SetupData->PcieRelaxedOrdering) {
      SpareSwwest2.Bits.disable_ro_override = 0;
    } else {
      SpareSwwest2.Bits.disable_ro_override = 1;
    }
    IioWriteCpuCsr32 (IioIndex, Stack, SPARE_SWWEST2_IIO_DFX_VTD_REG, SpareSwwest2.Data);
  }

  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);
  for(PortIndex = 0; PortIndex < MaxPortNumberPerSocket; PortIndex++) {
    if (!IioOutData->PciePortConfig[(IioIndex * MaxPortNumberPerSocket) + PortIndex]){
      continue;
    }
    // no per port DFX init settings yet
  } //end of per port DFX init settings
}

/**
  DfxPcieInit: Contains functions for programming Rx, Tx, Isoc parameters and some WA.

  @param[in] IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
  @param[in] IioIndex           - Index of IIO/CPU
**/
VOID
DfxPcieInit (
  IN  IIO_GLOBALS                         *IioGlobalData,
  IN  UINT8                               IioIndex
  )
{
  //
  // IIO Rx Recalibration programming, before UNIPHY Rx recipe settings (if any)
  //
  IioPrePhyRecipeInit (IioIndex);
  //
  // Apply IIO RX Recipe settings here
  //
  if (!UbiosGenerationEnabled () && !IsSimicsEnvironment()) {
    IioPhyRecipeInit (IioGlobalData, IioIndex);
  }
  //
  //IIO Rx Recalibration programming, after UNIPHY Rx recipe settings (if any)
  //
  IioPostPhyRecipeInit (IioIndex);
  //
  // Allow OEMs to do an override of any of the default IIO RX Recipe settings
  //
  OemOverrideIioRxRecipeSettings (IioGlobalData, IioIndex);

  DfxEarlyInit (IioGlobalData, IioIndex);
}

/**
  The purpose of this function is to verify if an override is required
  before issue a Gen3 Link intialization.

  @param[in] IioGlobalData - Pointer to IIO_GLOBALS structure
  @param[in] IioIndex      - Socket Index
**/
VOID
PciePhyPreLinkOverride (
  IN  IIO_GLOBALS        *IioGlobalData,
  IN  UINT8              IioIndex
  )
{
#if 0 // SPR TODO
  UINT8               PortIndex;
  UINT8               MaxPortNumberPerSocket;
  IIO_OUT_DATA        *IioOutData;

  IioOutData = GetIioOutData ();
  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);
  for (PortIndex = 0; PortIndex < MaxPortNumberPerSocket; PortIndex++) {

    if ((IioOutData->PciePortPresent[(IioIndex*MaxPortNumberPerSocket)+PortIndex] == 0 ) ||
        (IioOutData->PciePortConfig[(IioIndex*MaxPortNumberPerSocket)+PortIndex] == 0 )) {
        continue;
    }
    //
    // If PCI port is set to disable (Disable Link = TRUE) than skip, but do not skip DMI.
    //
    if (CheckVarPortEnable(IioGlobalData, IioIndex, PortIndex) == PCIE_PORT_DISABLE &&
        !IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPortActive)) {
      continue;
    }

    Gen3PrelinkOverride (IioGlobalData, IioIndex, PortIndex);
    IioStall (UNIPHY_RECIPE_DELAY);   //Intended fix for s4030382: give a delay of 180us after Pre-link Gen3 override
  }
  return;
#endif
}
