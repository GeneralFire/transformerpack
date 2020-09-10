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

#include <IioSetupDefinitions.h>
#include <Library/IioAccessLib.h>
#include <RcRegs.h>
#include <Library/IoLib.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/IioSysInfoLib.h>
#include <PcieRegs.h>
#include <Library/IioDataHubLib.h>
#include <LibraryPrivate/IioDebug.h>
#include <Library/SiliconWorkaroundLib.h>
#include <LibraryPrivate/IioPcieLib.h>

static UINTN BifurcationMap[5][8] = {{ 4, 0, 4, 0, 4, 0, 4, 0},
                                     { 8, 0, 0, 0, 4, 0, 4, 0},
                                     { 4, 0, 4, 0, 8, 0, 0, 0},
                                     { 8, 0, 0, 0, 8, 0, 0, 0},
                                     {16, 0, 0, 0, 0, 0, 0, 0} };

/**
  Routine name: PciePhyTestMode
  Description: This routine sets the PCIe PHY test mode

    @param IioGlobalData - IIO global data structure
    @param IioIndex      - IIO index

    @retval None

**/
VOID
PciePhyTestMode (
  IN  IIO_GLOBALS              *IioGlobalData
  )
{
  return;
}

/**

  This routine is to any IIO RX recipe workarounds which would be done post IIO init

    @param IioGlobalData  - Pointer to IIO_GLOBALS
    @param IioIndex       - Index to CPU/IIO

*/
VOID
IioRxRecipeWorkAround (
    IIO_GLOBALS                     *IioGlobalData,
    UINT8                           IioIndex
  )
{
  UINT8                        PortIndex;
  UINT8                        ClusterPort;
  UINT8                        Bifurcation;
  UINTN                        CtrEnd;
  UINT8                        Ctr;
  PCI_REG_PCIE_LINK_CONTROL    LnkCon;
  UINT8                        PCIeCapOffset;
  IIO_CONFIG                   *SetupData;
  PCI_ROOT_BRIDGE_PCI_ADDRESS  PciAddress;
  UINT16                       LnkCtlOffset;
  UINT8                        Bus;
  UINT8                        Device;
  UINT8                        Function;
  UINT16                       Vid;
  UINT8                        OffsetFromClusterPort;

  SetupData = GetIioConfig ();
  for (PortIndex = 0; PortIndex < NUMBER_PORTS_PER_SOCKET; PortIndex++) {
    if (!IioVData_ConfigurePciePort (IioGlobalData, IioIndex, PortIndex)) {
      //
      // skip if the PCIe port cannot be configured
      //
      continue;
    }

    if (SetupData->SRIS[IioIndex * NUMBER_PORTS_PER_SOCKET + PortIndex] == IIO_OPTION_ENABLE) {
      continue;
    }

    if (SetupData->PcieCommonClock[IioIndex * NUMBER_PORTS_PER_SOCKET + PortIndex] == IIO_OPTION_ENABLE) {
      continue;
    }

    if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPortActive)) {
      LnkCon.Uint16 = IioReadCpuCsr16 (IioIndex, PortIndex, LINKCTL_IIO_PCIEDMI_REG);
      LnkCon.Bits.CommonClockConfiguration = 0;
      IioWriteCpuCsr16 (IioIndex, PortIndex, LINKCTL_IIO_PCIEDMI_REG, LnkCon.Uint16);
    } else {
      GetPciePortBusDeviceFunction (IioIndex, PortIndex, &PciAddress);
      Bus = PciAddress.Bus;
      Device = PciAddress.Device;
      Function = PciAddress.Function;
      IIO_D_PCIDBG ("%a:%d Port:%d Bus:%x Dev:%d Func:%d \n",
        __FUNCTION__, IioIndex, PortIndex, Bus, Device, Function);
      PCIeCapOffset = IioPCIeGetCapOffset (IioIndex, Bus, Device, Function, EFI_PCI_CAPABILITY_ID_PCIEXP);
      if (PCIeCapOffset == 0) {
        Vid = IioPciExpressRead16 (IioIndex, Bus, Device, Function, 0);
        IIO_D_PCIERR ("%a Can't find PCIeCapOffset for %2x:%2x.%d!\n",
          __FUNCTION__, Bus, Device, Function, Vid);
        ASSERT (PCIeCapOffset != 0);
        continue;
      }
      Bifurcation = CheckBifurcationMapPort(IioGlobalData, IioIndex, PortIndex);
      ClusterPort = GetPciePortSuperClusterPort (PortIndex);
      OffsetFromClusterPort = PortIndex - ClusterPort;

      ASSERT (Bifurcation < ARRAY_SIZE (BifurcationMap));
      ASSERT (ClusterPort <= PortIndex);
      ASSERT (OffsetFromClusterPort < ARRAY_SIZE (BifurcationMap[0]));
      if (!(Bifurcation < ARRAY_SIZE (BifurcationMap) &&
          ClusterPort <= PortIndex &&
          OffsetFromClusterPort < ARRAY_SIZE (BifurcationMap[0]))) {
        IIO_D_PCIERR ("\n%a() - BifurcationMap[Bifurcation] out of bound.\n", __FUNCTION__);
        continue;
      }
      CtrEnd = (BifurcationMap[Bifurcation][OffsetFromClusterPort] / 4);

      for (Ctr = 0; Ctr < CtrEnd; Ctr++) {
        GetPciePortBusDeviceFunction (IioIndex, PortIndex + Ctr, &PciAddress);
        Bus = PciAddress.Bus;
        Device = PciAddress.Device;
        Function = PciAddress.Function;
        IIO_D_PCIDBG ("%a:%d Port:%d Bus:%x Dev:%d Func:%d \n",
          __FUNCTION__, IioIndex, PortIndex + Ctr, Bus, Device, Function);
        PCIeCapOffset = IioPCIeGetCapOffset (IioIndex, Bus, Device, Function, EFI_PCI_CAPABILITY_ID_PCIEXP);
        if (PCIeCapOffset == 0) {
          Vid = IioPciExpressRead16 (IioIndex, Bus, Device, Function, 0);
          IIO_D_PCIERR ("%a Can't find PCIeCapOffset for %2x.%2x.%d Vid %x!\n",
            __FUNCTION__, Bus, Device, Function, Vid);
          ASSERT (PCIeCapOffset != 0);
          continue;
        }
        LnkCtlOffset = PCIeCapOffset + R_PCIE_LCTL_OFFSET;
        LnkCon.Uint16 = IioPciExpressRead16 (IioIndex, Bus, Device, Function, LnkCtlOffset);
        LnkCon.Bits.CommonClockConfiguration = 0;
        IioPciExpressWrite16 (IioIndex, Bus, Device, Function, LnkCtlOffset, LnkCon.Uint16);
      }
    }
  }
}

VOID
IioPcieWorkaroundsPreLinkPerSocket (
  IN  IIO_GLOBALS          *IioGlobalData,
  IN  UINT8                IioIndex
)
{
  ClearXPUncErrStsBit6 (IioGlobalData, IioIndex);
  IioPcieWorkarounds (IioGlobalData, IioIndex);
}

/**

  This routine is for silicon workaround after link training

  @param[in]  IioGlobalData - Pointer to IioGlobalData
  @param[in]  IioIndex      - Index to Iio

**/
VOID
IioRetimerVidWorkAround (
  IN  IIO_GLOBALS                     *IioGlobalData,
  IN   UINT8                          IioIndex
)
{
  RTLLCTL_IIO_PCIE_G5_STRUCT              RtllCtl;
  LTSSMSMSTS_IIO_PCIE_G5_STRUCT           LtssmSts;
  RTLLCTL3_IIO_PCIE_G5_STRUCT             RtllCtl3;
  LINKSTS2_IIO_PCIE_G5_STRUCT             LinkSts2;
  UINT8                                   PortIndex;
  UINT8                                   MaxPortNumberPerSocket;

  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);
  for (PortIndex = 0; PortIndex < MaxPortNumberPerSocket; PortIndex++) {
    if (!IioVData_ConfigurePciePort(IioGlobalData, IioIndex, PortIndex)) {
      continue;
    }
    LinkSts2.Data = IioReadCpuCsr16 (IioIndex, PortIndex, LINKSTS2_IIO_PCIE_G5_REG);
    //
    // Check whether retimer be present or not
    //
    if (LinkSts2.Bits.rpd == 0) {
      continue;
    }
    //
    // 14010882657:VID is reversed if lane reversed which cause retimer failures in entering low latency mode
    //
    if (IsSiliconWorkaroundEnabled ("S14010882657")) {
      LtssmSts.Data = IioReadCpuCsr32 (IioIndex, PortIndex, LTSSMSMSTS_IIO_PCIE_G5_REG);
      if (LtssmSts.Bits.lnkreversed) {
        RtllCtl3.Data = IioReadCpuCsr32 (IioIndex, PortIndex, RTLLCTL3_IIO_PCIE_G5_REG);
        RtllCtl3.Bits.vid = 0x6808; // reverse VID from 0x8086 to 0x6808 if lane reversed
        IioWriteCpuCsr32 (IioIndex, PortIndex, RTLLCTL3_IIO_PCIE_G5_REG, RtllCtl3.Data);
      }
      RtllCtl.Data = IioReadCpuCsr32 (IioIndex, PortIndex, RTLLCTL_IIO_PCIE_G5_REG);
      RtllCtl.Bits.enable = ((IIO_GEN4 == IioPortControllerGen (IioGlobalData, IioIndex, PortIndex))?1:3);
      IioWriteCpuCsr32 (IioIndex, PortIndex, RTLLCTL_IIO_PCIE_G5_REG, RtllCtl.Data);
    }
  }
}

VOID
IioPcieWorkaroundsPostLinkPerSocket (
  IN  IIO_GLOBALS          *IioGlobalData,
  IN  UINT8                IioIndex
)
{
  IioRetimerVidWorkAround (IioGlobalData, IioIndex);
  IioRxRecipeWorkAround (IioGlobalData, IioIndex);
}
