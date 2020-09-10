/** @file
  This file initialize the CXL device on IIO

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#include <Library/IioDataHubLib.h>
#include <Library/IioAccessLib.h>
#include <RcRegs.h>
#include <Library/KtiApi.h>
#include <Library/CxlIpLib.h>
#include <Library/IioSysInfoLib.h>
#include <LibraryPrivate/IioDebug.h>
#include <Library/SiliconWorkaroundLib.h>

/**
  Enable clock gating configuration settings for stack configured as CXL

  For SPR CXL controllers

  @param IioIndex      - Socket number of the port being configured.
  @param CxlIndex      - Index of CPU/IIO

**/
VOID
IioCxlClkGatingEn (
  IN  UINT8                              IioIndex,
  IN  UINT8                              CxlIndex
)
{
  IOSFDEVCLKGCTL_IIO_CXL_STRUCT          IosfDevClkGCtl;
  SBDEVCLKGCTL_IIO_CXL_STRUCT            SbDevClkGCtl;
  PT0TXNCLKGCTL_IIO_CXL_STRUCT           PtNTxnClkGCtl;
  PT0LNKCLKGCTL_IIO_CXL_STRUCT           PtNLnkClkGCtl;
  PT0LDPHYCLKGCTL_IIO_CXL_STRUCT         Pt0LDPhyClkGCtl;
  PT1LDPHYCLKGCTL_IIO_CXL_STRUCT         Pt1LDPhyClkGCtl;
  PT2LDPHYCLKGCTL_IIO_CXL_STRUCT         Pt2LDPhyClkGCtl;
  PT3LDPHYCLKGCTL_IIO_CXL_STRUCT         Pt3LDPhyClkGCtl;
  CFGPCIECLKGCTL_IIO_CXL_STRUCT          CfgPcieClkGCtl;


  IIO_D_LOG ("CXL_IO_CLOCK_GATING_START\n");
  //
  //  [id: 2006652770]: ICX Redefinition: Clock gating programming on CXL
  //
  IosfDevClkGCtl.Data = IioReadCpuCsr16 (IioIndex, CxlIndex, IOSFDEVCLKGCTL_IIO_CXL_REG);
  IosfDevClkGCtl.Bits.icge = 1;
  if (!IosfDevClkGCtl.Bits.ict) {
    IosfDevClkGCtl.Bits.ict = 16;  //16 (default value) is the minimum number of clocks recommended
  }
  IioWriteCpuCsr16 (IioIndex, CxlIndex, IOSFDEVCLKGCTL_IIO_CXL_REG, IosfDevClkGCtl.Data);

  SbDevClkGCtl.Data = IioReadCpuCsr16 (IioIndex, CxlIndex, SBDEVCLKGCTL_IIO_CXL_REG);
  SbDevClkGCtl.Bits.icge = 1;
  if (!SbDevClkGCtl.Bits.ict) {
    SbDevClkGCtl.Bits.ict = 16;  //16 (default value) is the minimum number of clocks recommended
  }
  IioWriteCpuCsr16 (IioIndex, CxlIndex, SBDEVCLKGCTL_IIO_CXL_REG, SbDevClkGCtl.Data);

  PtNTxnClkGCtl.Data = IioReadCpuCsr16 (IioIndex, CxlIndex, PT0TXNCLKGCTL_IIO_CXL_REG );
  PtNTxnClkGCtl.Bits.icge = 1;
  if (!PtNTxnClkGCtl.Bits.ict) {
    PtNTxnClkGCtl.Bits.ict = 16;  //16 (default value) is the minimum number of clocks recommended
  }
  IioWriteCpuCsr16 (IioIndex, CxlIndex, PT0TXNCLKGCTL_IIO_CXL_REG , PtNTxnClkGCtl.Data);

  PtNLnkClkGCtl.Data = IioReadCpuCsr16 (IioIndex, CxlIndex, PT0LNKCLKGCTL_IIO_CXL_REG );
  PtNLnkClkGCtl.Bits.icge = 1;
  if (!PtNLnkClkGCtl.Bits.ict) {
    PtNLnkClkGCtl.Bits.ict = 16;  //16 (default value) is the minimum number of clocks recommended
  }
  IioWriteCpuCsr16 (IioIndex, CxlIndex, PT0LNKCLKGCTL_IIO_CXL_REG , PtNLnkClkGCtl.Data);

  Pt0LDPhyClkGCtl.Data = IioReadCpuCsr16 (IioIndex, CxlIndex, PT0LDPHYCLKGCTL_IIO_CXL_REG);
  Pt0LDPhyClkGCtl.Bits.icge = 1;
  if (!Pt0LDPhyClkGCtl.Bits.ict) {
    Pt0LDPhyClkGCtl.Bits.ict = 16;  //16 (default value) is the minimum number of clocks recommended
  }
  IioWriteCpuCsr16 (IioIndex, CxlIndex, PT0LDPHYCLKGCTL_IIO_CXL_REG, Pt0LDPhyClkGCtl.Data);

  Pt1LDPhyClkGCtl.Data = IioReadCpuCsr16 (IioIndex, CxlIndex, PT1LDPHYCLKGCTL_IIO_CXL_REG);
  Pt1LDPhyClkGCtl.Bits.icge = 1;
  if (!Pt1LDPhyClkGCtl.Bits.ict) {
    Pt1LDPhyClkGCtl.Bits.ict = 16;  //16 (default value) is the minimum number of clocks recommended
  }
  IioWriteCpuCsr16 (IioIndex, CxlIndex, PT1LDPHYCLKGCTL_IIO_CXL_REG, Pt1LDPhyClkGCtl.Data);

  Pt2LDPhyClkGCtl.Data = IioReadCpuCsr16 (IioIndex, CxlIndex, PT2LDPHYCLKGCTL_IIO_CXL_REG);
  Pt2LDPhyClkGCtl.Bits.icge = 1;
  if (!Pt2LDPhyClkGCtl.Bits.ict) {
    Pt2LDPhyClkGCtl.Bits.ict = 16;  //16 (default value) is the minimum number of clocks recommended
  }
  IioWriteCpuCsr16 (IioIndex, CxlIndex, PT2LDPHYCLKGCTL_IIO_CXL_REG, Pt2LDPhyClkGCtl.Data);

  Pt3LDPhyClkGCtl.Data = IioReadCpuCsr16 (IioIndex, CxlIndex, PT3LDPHYCLKGCTL_IIO_CXL_REG);
  Pt3LDPhyClkGCtl.Bits.icge = 1;
  if (!Pt3LDPhyClkGCtl.Bits.ict) {
    Pt3LDPhyClkGCtl.Bits.ict = 16;  //16 (default value) is the minimum number of clocks recommended
  }
  IioWriteCpuCsr16 (IioIndex, CxlIndex, PT3LDPHYCLKGCTL_IIO_CXL_REG, Pt3LDPhyClkGCtl.Data);

  CfgPcieClkGCtl.Data = IioReadCpuCsr16 (IioIndex, CxlIndex, CFGPCIECLKGCTL_IIO_CXL_REG);
  CfgPcieClkGCtl.Bits.icge = 1;
  if (!CfgPcieClkGCtl.Bits.ict) {
    CfgPcieClkGCtl.Bits.ict = 16;  //16 (default value) is the minimum number of clocks recommended
  }
  IioWriteCpuCsr16 (IioIndex, CxlIndex, CFGPCIECLKGCTL_IIO_CXL_REG, CfgPcieClkGCtl.Data);

  IIO_D_LOG ("CXL_IO_CLOCK_GATING_END\n");

}

/**

  Setup CXL.IO TC/VC Mapping.

  @param None

  @retval None

**/
VOID
IioCxlTcVcMapping (
  VOID
  )
{
  // Add tags for uBIOS, emulation team can add CXL.IO TC/VC mapping WA here.
  IIO_D_LOG ("CXL_IO_TC_VC_MAPPING_START\n");
  IIO_D_LOG ("CXL_IO_TC_VC_MAPPING_END\n");
}

/**
  Per CXL.IO port initialization before link training.

  @param[in] IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
  @param[in] IioIndex           - Index of CPU/IIO

**/
VOID
CxlIoPortPreTrainInit (
  IN IIO_GLOBALS                 *IioGlobalData,
  IN UINT8                       IioIndex
  )
{
  UINT8                       StackIndex;
  UINT8                       PortIndex;
  PI5_MODES                   StackMode;
  IALPMMCTL_IIO_PCIE_G5_STRUCT  IalPmmCtl;

  IIO_D_LOG ("CXL_IO_PRE_TRAIN_INIT_START\n");

  for (StackIndex = 1; StackIndex < (GetMaxIouNumPerSocket () + 1); StackIndex++) {
    StackMode = GetStackMode (IioGlobalData, IioIndex, StackIndex);
    if (PI5_MODE_IAL != StackMode) {
      continue;
    }

    PortIndex = GetPortIndexbyStack (StackIndex, IioPortA);

    //
    // Change the vendor message encoding in CXL PM VDM.
    //
    IalPmmCtl.Data = IioReadCpuCsr32 (IioIndex, PortIndex, IALPMMCTL_IIO_PCIE_G5_REG);
    IalPmmCtl.Bits.vmeb15 = 0x68;
    IioWriteCpuCsr32 (IioIndex, PortIndex, IALPMMCTL_IIO_PCIE_G5_REG, IalPmmCtl.Data);
  }

  IIO_D_LOG ("CXL_IO_PRE_TRAIN_INIT_END\n");
}

/**

  Per CXL.IO port initialization.

  @param[in] IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
  @param[in] IioIndex           - Index of CPU/IIO

  @retval None

**/
VOID
CxlIoPortInit (
  IN IIO_GLOBALS                 *IioGlobalData,
  IN UINT8                       IioIndex
  )
{
  UINT8                       CxlIndex;
  UINT8                       StackIndex;
  LINKCTL_IIO_CXL_STRUCT      LnkCtlCxl;
  XPTDEF2_IIO_CXL_STRUCT      XptDef2Cxl;
  PCICMD_IIO_CXL_STRUCT       PciCmdCxl;
  DEVCTL_IIO_CXL_STRUCT       DevCtrlCxl;
  DEVCTL2_IIO_CXL_STRUCT      DevCtrl2Cxl;
  XPTDEF5_IIO_CXL_STRUCT      XptDef5Cxl;
  LINKSTS_IIO_CXL_STRUCT      LnkStsCxl;
  SLOTSTS_IIO_CXL_STRUCT      SlotStsCxl;

  IIO_D_LOG ("CXL_IO_INIT_START\n");
  for ( CxlIndex = 0; CxlIndex < GetMaxIouNumPerSocket (); CxlIndex++) {
    StackIndex = CxlIndex + 1;
    if (GetCxlStatus (IioIndex, StackIndex) != AlreadyInCxlMode) {
      continue;
    }

    // Enable the command/data parity check for incoming commands into the IOSF Target block.
    XptDef2Cxl.Data = IioReadCpuCsr32 (IioIndex, CxlIndex, XPTDEF2_IIO_CXL_REG);
    XptDef2Cxl.Bits.tcpar_chk_dis = 0;
    XptDef2Cxl.Bits.tdpar_chk_dis = 0;
    IioWriteCpuCsr32 (IioIndex, CxlIndex, XPTDEF2_IIO_CXL_REG, XptDef2Cxl.Data);

    PciCmdCxl.Data = IioReadCpuCsr16 (IioIndex, CxlIndex, PCICMD_IIO_CXL_REG);
    // Disable legacy interrupt generation.
    PciCmdCxl.Bits.intxd = 1;
    // Enable memory space and Bus master.
    PciCmdCxl.Bits.mse = 1;
    PciCmdCxl.Bits.bme = 1;
    IioWriteCpuCsr16 (IioIndex, CxlIndex, PCICMD_IIO_CXL_REG, PciCmdCxl.Data);

    DevCtrlCxl.Data = IioReadCpuCsr16 (IioIndex, CxlIndex, DEVCTL_IIO_CXL_REG);
    // Set Max Payload Size to 128 byte as default.
    //SPR_TODO: Update this code to config mps according to attached CXL device.
    DevCtrlCxl.Bits.mps = 0;
    // Enable a function to use an 8-bit tag field as a Requester.
    DevCtrlCxl.Bits.etfe = 1;
    IioWriteCpuCsr16 (IioIndex, CxlIndex, DEVCTL_IIO_CXL_REG, DevCtrlCxl.Data);

    // Permit requester to use 10-Bit Tags.
    DevCtrl2Cxl.Data = IioReadCpuCsr16 (IioIndex, CxlIndex, DEVCTL2_IIO_CXL_REG);
    DevCtrl2Cxl.Bits.tenbtre = 1;
    IioWriteCpuCsr16 (IioIndex, CxlIndex, DEVCTL2_IIO_CXL_REG, DevCtrl2Cxl.Data);

    IioCxlTcVcMapping ();

    IioCxlClkGatingEn (IioIndex, CxlIndex);

    // Enable L1 Entry.
    LnkCtlCxl.Data = IioReadCpuCsr16 (IioIndex, CxlIndex, LINKCTL_IIO_CXL_REG);
    LnkCtlCxl.Bits.aspmctl = 2;
    IioWriteCpuCsr16 (IioIndex, CxlIndex, LINKCTL_IIO_CXL_REG, LnkCtlCxl.Data);

    // Enable SAI/RS TLP Prefix support.
    XptDef5Cxl.Data = IioReadCpuCsr16 (IioIndex, CxlIndex, XPTDEF5_IIO_CXL_REG);
    if (IsSiliconWorkaroundEnabled ("S1808609346")) {
      XptDef5Cxl.Bits.sairstpd = 1;
    } else {
      XptDef5Cxl.Bits.sairstpd = 0;
    }
    IioWriteCpuCsr16 (IioIndex, CxlIndex, XPTDEF5_IIO_CXL_REG, XptDef5Cxl.Data);

    // Clear link status bandwidth notification bits.
    LnkStsCxl.Data = IioReadCpuCsr16 (IioIndex, CxlIndex, LINKSTS_IIO_CXL_REG);
    LnkStsCxl.Bits.labs = 1;
    LnkStsCxl.Bits.lbms = 1;
    IioWriteCpuCsr16 (IioIndex, CxlIndex, LINKSTS_IIO_CXL_REG, LnkStsCxl.Data);

    SlotStsCxl.Data = IioReadCpuCsr16 (IioIndex, CxlIndex, SLOTSTS_IIO_CXL_REG);
    SlotStsCxl.Bits.dllscs = 1;
    IioWriteCpuCsr16 (IioIndex, CxlIndex, SLOTSTS_IIO_CXL_REG, SlotStsCxl.Data);

  }
  IIO_D_LOG ("CXL_IO_INIT_END\n");
}

/**
  Set Arb-Mux Clock Gating Enable bits: Stcge, Sdcge, Dlcge, Ltcge

  @param SocId                   - CPU Socket Node number (Socket ID)
  @param Stack                   - Stack number, 0 based

  @retval

**/
VOID
EFIAPI
ConfigureArbMuxCge (
  IN UINT8              Socket,
  IN UINT8              Stack
  )
{
  ARBMUXCTL0_SB_CXL_AM_STRUCT     CxlArbMuxCtl0;

  CxlArbMuxCtl0.Data = IioReadCpuCsr32 (Socket, Stack, ARBMUXCTL0_SB_CXL_AM_REG);
  CxlArbMuxCtl0.Bits.stcge = 1;
  CxlArbMuxCtl0.Bits.sdcge = 1;
  CxlArbMuxCtl0.Bits.dlcge = 1;
  CxlArbMuxCtl0.Bits.lctge = 1;
  IioWriteCpuCsr32 (Socket, Stack, ARBMUXCTL0_SB_CXL_AM_REG, CxlArbMuxCtl0.Data);
}

/**
  Enable clock gating for ARB/MUX

  @param IioGlobalData    Pointer to IIO_GLOBALS
  @param IioIndex         Index to Socket

  @retval IIO_SUCCESS Initialization completed without problems
--*/
VOID
EFIAPI
ArbMuxClockGatingEn (
  IN OUT IIO_GLOBALS      *IioGlobalData,
  IN UINT8                IioIndex
  )
{
  UINT8               MaxIioStack;
  UINT8               Stack;
  PI5_MODES           StackMode;

  MaxIioStack = GetMaxIouNumPerSocket () + 1; // PCIE + DMI

  for (Stack = 0; Stack < MaxIioStack; Stack++) {
    if ((IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][Stack] != TYPE_UBOX_IIO)) {
      continue;
    }

    StackMode = GetStackMode (IioGlobalData, IioIndex, Stack);
    switch (StackMode) {
    case PI5_MODE_DMI:
    case PI5_MODE_NTB:
    case PI5_MODE_GEN4_ONLY:
      break;
    case PI5_MODE_IAL:
    case PI5_MODE_GEN5_AND_GEN4:
       ConfigureArbMuxCge (IioIndex, Stack);
      break;
    default:
      break;
    }
  }
}
