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

#include <UncoreCommonIncludes.h>
#include <Library/IioAccessLib.h>
#include <RcRegs.h>
#include <Library/IoLib.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/IioSysInfoLib.h>
#include <PcieRegs.h>
#include <Library/IioDataHubLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <LibraryPrivate/IioPcieLib.h>

/**
  Clear error status bit6 in XPUNCERRSTS register
  Context: HSD 3613071

  @param[in] IioGlobalData - IIO global data structure
  @param[in] IioIndex      - IIO index
**/
VOID
ClearXPUncErrStsBit6 (
  IN  IIO_GLOBALS          *IioGlobalData,
  IN  UINT8                IioIndex
  )
{
  UINT8   PortIndex;
  UINT8   MaxPortNumberPerSocket;
  ERRUNCSTS_IIO_PCIEDMI_STRUCT     DmiXpUncerrSts;
  ERRUNCSTS_IIO_PCIE_G5_STRUCT     PcieXpUncerrSts;

  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);
  for (PortIndex = 0; PortIndex < MaxPortNumberPerSocket; PortIndex++) {
    //
    // Check whether the PCIe port can be configured
    //
    if (!IioVData_ConfigurePciePort(IioGlobalData, IioIndex, PortIndex)) {
      continue;
    }

    if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPort)) {
      DmiXpUncerrSts.Data = IioReadCpuCsr32 (IioIndex, PortIndex, ERRUNCSTS_IIO_PCIEDMI_REG);
      //
      // This bit is R/W1CS = write 1 to clear
      //
      DmiXpUncerrSts.Bits.ure = 1;
      IioWriteCpuCsr32 (IioIndex, PortIndex, ERRUNCSTS_IIO_PCIEDMI_REG, DmiXpUncerrSts.Data);
    } else {
      PcieXpUncerrSts.Data = IioReadCpuCsr32 (IioIndex, PortIndex, ERRUNCSTS_IIO_PCIE_G5_REG);
      //
      // This bit is R/W1CS = write 1 to clear
      //
      PcieXpUncerrSts.Bits.ure = 1;
      IioWriteCpuCsr32 (IioIndex, PortIndex, ERRUNCSTS_IIO_PCIE_G5_REG, PcieXpUncerrSts.Data);
    }
  } // foreach PortIndex
}

/**
  Routine name: IioPcieWorkarounds()
  Description: Routine contains early workarounds for each PCIe port before its link is enabled

  @param[in] IioGlobalData - Pointer to IioGlobalData structure.
  @param[in] IioIndex      - IIO index
**/
VOID
IioPcieWorkarounds (
  IN  IIO_GLOBALS             *IioGlobalData,
  IN  UINT8                   IioIndex
  )
{
  UINT8                           PortIndex,MaxPortNumberPerSocket;
  IIO_CONFIG                      *SetupData;
  DLFCAP_IIO_PCIE_G5_STRUCT       DataLinkFeatureCap;
  IOBASE_IIO_PCIE_G5_STRUCT       IoBas;
  ACSCAP_IIO_PCIE_G5_STRUCT       AcsCapReg;

  SetupData = GetIioConfig ();
  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);
  for(PortIndex = 0; PortIndex < MaxPortNumberPerSocket; PortIndex++) {

    if (!IioVData_ConfigurePciePort(IioGlobalData, IioIndex, PortIndex)){
      continue;
    }

    //
    // HSD 4030421: IVT PCIE Link Layer not returning posted data credits for MSI transactions
    //  Resolution: Remove 2nd part of W/A for s4030322
    //  Removed setting of XPDFXSPAREREG.disable_msi_gt_1dw to 1

    //
    // ivt_eco: b3247660: CLONE from jaketown: Bad Replay Packets during
    //                        PCIe NAK Test in Gen3 X16 config with 256B MPS
    //  [cloned to s4030491] - common for IVT EP,EX,
    //
    //  4167964: PCIe HSX UniPhy Recipe Updated to v5.00 (PCIe changes only)
    //  4985815: UniPhy Recipe Not Being Set On Port 0 Of Non-Legacy Sockets
    //
    //  Skip DMI but do it for Port 0 in Non-Legacy Sockets.
    // SKX discard PXPRETRYCTRL3_IIO_DFX_REG

    //
    // 4032294 W/A for HSX B0,
    // iio_cr_xpdfxsparereg_0_(6_0|6_3|7_0)_cfg::disable_msi_gt_1dw = 1
    // HSX B0 stepping code is removed from SKX

    //
    // HSD 4166558 (HSX): HSX Clone: PCIE: Pcode is setting the ltssmdbg0.forcereverse bit in IOU0 (Dev6 / Fun3)
    // HSX Ax stepping code removed from SKX

    //
    // as per the s4031981, the WA for PM L2/3 failures as per the IVT s4677468
    // has to be removed..
    //

    //
    // ACS P2P Request Redirect (RACSP2PRR) & P2P Completion Redirect (CACSP2PRR)
    // used to be enabled in Gen4/Gen5.
    //
    AcsCapReg.Data = IioReadCpuCsr16 (IioIndex, PortIndex, ACSCAP_IIO_PCIE_G5_REG);
    AcsCapReg.Bits.racsp2prr = 1;
    AcsCapReg.Bits.cacsp2prr = 1;
    IioWriteCpuCsr32 (IioIndex, PortIndex, ACSCAP_IIO_PCIE_G5_REG, AcsCapReg.Data);

    IoBas.Data = IioReadCpuCsr8 (IioIndex, PortIndex, IOBASE_IIO_PCIE_G5_REG);
    IoBas.Bits.iobc &= 3;
    IioWriteCpuCsr8 (IioIndex, PortIndex, IOBASE_IIO_PCIE_G5_REG, (UINT8)IoBas.Data);

    if (IsSiliconWorkaroundEnabled ("S1507340892") &&
      (IioGlobalData->SetupData.PcieDataLinkFeatureExchangeEnable[IioIndex*MaxPortNumberPerSocket + PortIndex] == IIO_OPTION_DISABLE)) {
      //
      // HSD1507340892 Disable the Data Link Feature Exchange in PCIe system
      // [Base spec 4.0 chapter 7.7.4.2 Data Link Feature Capabilities Register (Offset 04h)]
      // to be compatible with legacy hardware
      //
      DataLinkFeatureCap.Data = IioReadCpuCsr32 (IioIndex, PortIndex, DLFCAP_IIO_PCIE_G5_REG);
      DataLinkFeatureCap.Bits.dlfee = 0;
      IioWriteCpuCsr32 (IioIndex, PortIndex, DLFCAP_IIO_PCIE_G5_REG, DataLinkFeatureCap.Data);
    }
  } // foreach PortIndex
}
