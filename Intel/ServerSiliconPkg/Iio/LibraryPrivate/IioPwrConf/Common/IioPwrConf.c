/** @file
  This file provides routines for PCIe/DMI ports clock gating configuration.
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

#include <IioPlatformData.h>
#include <Library/IioDataHubLib.h>
#include <IioSetupDefinitions.h>
#include <RcRegs.h>
#include <Library/IioAccessLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/KtiApi.h>
#include <LibraryPrivate/IioDebug.h>
#include <LibraryPrivate/IioPchLinkPwrConf.h>
#include <LibraryPrivate/IioPciePwrConf.h>
#include <LibraryPrivate/IioPwrConf.h>
#include <Library/CpuAndRevisionLib.h>

/**
  Enabling clock gating configuration settings for PCIe/DMI ports

  @param IioGlobalData - Pointer to IIO global data.
  @param IioIndex      - Socket number of the port being configured.
**/
VOID
IioPciePortPowerConfig (
  IN  IIO_GLOBALS                                   *IioGlobalData,
  IN  UINT8                                          IioIndex
  )
{
  UINT8     MaxPortNumberPerSocket;
  UINT8     PortIndex;

  //
  // Enable clock gating for PCIe/DMI ports
  //
  MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];
  for (PortIndex = 0; PortIndex < MaxPortNumberPerSocket; PortIndex++) {
    if ((PortIndex == 0) && IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {  // for port 0 on every socket
      if (!IfStackPresent (IioIndex, 0)){
        continue;
      }
    } else {
      if (!IioVData_ConfigurePciePort (IioGlobalData, IioIndex, PortIndex)) {
        continue;
      }
    }
    //
    // Clock Gating Feature
    //
    if (IioGlobalData->SetupData.IioPcieConfig.PciePortClkGateEnable[PortIndex]) {

      if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPort)) {
        //
        // This is required for port 0 in zero and non-zero sockets
        //
        IioNorthBridgePortClkGatingEn (IioGlobalData, IioIndex, PortIndex);
      } else {
        IioPcieClkGatingEn (IioGlobalData, IioIndex, PortIndex);
      }
    }

    //
    // ASMP L1 feature programming necessary only for DMI ports
    // PCIe ASPM L1 feature already handled in PcieLinkInit ()
    //
    if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPortActive) &&
         GetAspmSettingForPort (IioGlobalData, IioIndex, PortIndex) == PCIE_ASPM_L1_ONLY) {
        //
        // IP specific, not handled in existing IioDmiInit()
        //
        IioNorthBridgePortL1En (IioGlobalData, IioIndex, PortIndex);
    }
  }
}


/**
  Enable different access enabling configuration settings

  @param IioGlobalData - Pointer to IIO Globals
  @param IioIndex      - Socket number of the port being configured.
**/
VOID
IioEnableAccessPerSocket (
IN  IIO_GLOBALS         *IioGlobalData,
IN  UINT8               IioIndex
)
{
  UINT8     PortIndex;
  UINT8     MaxPortNumberPerSocket;

  MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];
  for (PortIndex = 0; PortIndex < MaxPortNumberPerSocket; PortIndex++) {
    if (!IioVData_ConfigurePciePort (IioGlobalData, IioIndex, PortIndex)) {
      continue;
    }

    if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPort) && !IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPortAsPci)) {

      IioEnablePchLinkPortDfxAccess (IioGlobalData, IioIndex, PortIndex);

    } else {

      IioPcieEnableAccess (IioGlobalData, IioIndex, PortIndex);
    }
  }
}


/**
 Disable different access enabling configuration settings.

 @param[in] IioGlobalData - Pointer to IIO Globals
 @param[in] IioIndex      - Socket number of the port being configured.
**/
VOID
IioRestrictAccessPerSocket (
  IN  IIO_GLOBALS *IioGlobalData,
  IN  UINT8        IioIndex
  )
{
  UINT8            PortIndex;
  UINT8            MaxPortNumberPerSocket;

  MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];
  for (PortIndex = 0; PortIndex < MaxPortNumberPerSocket; PortIndex++) {
    if (!IioVData_ConfigurePciePort (IioGlobalData, IioIndex, PortIndex)) {
      continue;
    }

    if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPort) && !IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPortAsPci)) {

      IioRestrictPchLinkPortDfxAccess (IioGlobalData, IioIndex, PortIndex);

    } else {

      IioPcieDisableAccess (IioGlobalData, IioIndex, PortIndex);
    }
  }
}
