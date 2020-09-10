/** @file
  This file provides routines for PCIe/DMI ports clock gating configuration.
@copyright
INTEL CONFIDENTIAL
Copyright 2019 - 2020 Intel Corporation. <BR>

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
#include <Library/SiliconWorkaroundLib.h>
#include <RcRegs.h>
#include <Library/IioSysInfoLib.h>

#include <LibraryPrivate/IioDebug.h>
#include <LibraryPrivate/IioPchLinkPwrConf.h>
#include <LibraryPrivate/IioPciePwrConf.h>
#include <Library/KtiApi.h>
#include <Library/IioDataHubLib.h>


/**
  Enable access to PCIE port configuration

  For EIP gen4 based controllers

  @param[in] IioIndex      - Socket number of the port being configured.
  @param[in] PortIndex     - Port number being configured.
**/
VOID
IioPcieEnableAccessGen4 (
  IN  UINT8  IioIndex,
  IN  UINT8  PortIndex
  )
{
  UINT32     AcpvRd;
  UINT32     AcpvWr;
  UINT8      BoxInst;
  UINT8      MaxPortPerIou;

  MaxPortPerIou = GetMaxPortNumPerIou ();
  if (MaxPortPerIou == 0) {
    return;
  }
  BoxInst = (PortIndex + (MaxPortPerIou - 1)) / MaxPortPerIou;

  AcpvRd = IioReadCpuCsr32 (IioIndex, BoxInst, ACRDPR5_N0_SB_PRVT_PCIE_G4_REG);
  AcpvWr = IioReadCpuCsr32 (IioIndex, BoxInst, ACWRPR5_N0_SB_PRVT_PCIE_G4_REG);

  //Add HOSTIA_BOOT_SAI to DFX1
  AcpvRd |= BIT4;
  AcpvWr |= BIT4;

  IioWriteCpuCsr32 (IioIndex, BoxInst, ACRDPR5_N0_SB_PRVT_PCIE_G4_REG, AcpvRd);
  IioWriteCpuCsr32 (IioIndex, BoxInst, ACWRPR5_N0_SB_PRVT_PCIE_G4_REG, AcpvWr);

}



/**
  Enable access to PCIE port configuration

  For EIP gen4 based controllers

  @param[in] IioIndex      - Socket number of the port being configured.
  @param[in] PortIndex     - Port number being configured.
**/
VOID
IioPcieEnableAccessGen5 (
  IN  UINT8  IioIndex,
  IN  UINT8  PortIndex
  )
{
  UINT32     AcpvRd;
  UINT32     AcpvWr;
  UINT8      BoxInst;
  UINT8      MaxPortPerIou;

  MaxPortPerIou = GetMaxPortNumPerIou ();
  if (MaxPortPerIou == 0) {
    return;
  }
  BoxInst = (PortIndex + (MaxPortPerIou - 1)) / MaxPortPerIou;

  AcpvRd = IioReadCpuCsr32 (IioIndex, BoxInst, ACRDPR5_N0_SB_PRVT_PCIE_G5_REG);
  AcpvWr = IioReadCpuCsr32 (IioIndex, BoxInst, ACWRPR5_N0_SB_PRVT_PCIE_G5_REG);

  //Add HOSTIA_BOOT_SAI to DFX1
  AcpvRd |= BIT4;
  AcpvWr |= BIT4;

  IioWriteCpuCsr32 (IioIndex, BoxInst, ACRDPR5_N0_SB_PRVT_PCIE_G5_REG, AcpvRd);
  IioWriteCpuCsr32 (IioIndex, BoxInst, ACWRPR5_N0_SB_PRVT_PCIE_G5_REG, AcpvWr);

  AcpvRd = IioReadCpuCsr32 (IioIndex, BoxInst, ACRDPR5_N0_SB_PRVT_FBLP_REG);
  AcpvWr = IioReadCpuCsr32 (IioIndex, BoxInst, ACWRPR5_N0_SB_PRVT_FBLP_REG);

  //Add HOSTIA_BOOT_SAI to DFX1
  AcpvRd |= BIT4;
  AcpvWr |= BIT4;

  IioWriteCpuCsr32 (IioIndex, BoxInst, ACRDPR5_N0_SB_PRVT_FBLP_REG, AcpvRd);
  IioWriteCpuCsr32 (IioIndex, BoxInst, ACWRPR5_N0_SB_PRVT_FBLP_REG, AcpvWr);
}


VOID
IioPcieEnableAccess (
  IN  IIO_GLOBALS                                   *IioGlobalData,
  IN  UINT8                                          IioIndex,
  IN  UINT8                                          PortIndex
)
{
  if (PortIndex & BIT0){
    IioPcieEnableAccessGen5 (IioIndex, PortIndex); // Port 1/3/5/7...is Gen5 port
  } else {
    IioPcieEnableAccessGen4 (IioIndex, PortIndex); // Port 0/2/4/6... is Gen4 port
  }
}


/**
  Enable access to PCH link port configuration

  For EIP gen4 based controllers

  @param[in] IioGlobalData - Pointer to IIO global data.
  @param[in] IioIndex      - Socket number of the port being configured.
  @param[in] PortIndex     - Port number being configured.
**/
VOID
IioEnablePchLinkPortDfxAccess (
  IN  IIO_GLOBALS                                   *IioGlobalData,
  IN  UINT8                                          IioIndex,
  IN  UINT8                                          PortIndex
)
{
  IioPcieEnableAccess (IioGlobalData, IioIndex, PortIndex);
}

/**
  Restrict access to PCIE port configuration

  For EIP gen4 based controllers

  @param[in] IioIndex      - Socket number of the port being configured.
  @param[in] PortIndex     - Port number being configured.
**/
VOID
IioPcieDisableAccessGen4 (
  IN  UINT8  IioIndex,
  IN  UINT8  PortIndex
  )
{
  UINT32     Acpv;
  UINT8      BoxInst;
  UINT8      MaxPortPerIou;

  MaxPortPerIou = GetMaxPortNumPerIou ();
  if (MaxPortPerIou == 0) {
    return;
  }
  BoxInst = (PortIndex + (MaxPortPerIou - 1)) / MaxPortPerIou;

  //Remove HOSTIA_BOOT_SAI from DFX1 SAI policy
  Acpv = IioReadCpuCsr32 (IioIndex, BoxInst, ACRDPR5_N0_SB_PRVT_PCIE_G4_REG);
  Acpv &= ~BIT4;
  IioWriteCpuCsr32 (IioIndex, BoxInst, ACRDPR5_N0_SB_PRVT_PCIE_G4_REG, Acpv);
  Acpv = IioReadCpuCsr32 (IioIndex, BoxInst, ACWRPR5_N0_SB_PRVT_PCIE_G4_REG);
  Acpv &= ~BIT4;
  IioWriteCpuCsr32 (IioIndex, BoxInst, ACWRPR5_N0_SB_PRVT_PCIE_G4_REG, Acpv);

}
/**
  Restrict access to PCIE port configuration

  For EIP gen4 based controllers

  @param[in] IioIndex      - Socket number of the port being configured.
  @param[in] PortIndex     - Port number being configured.
**/
VOID
IioPcieDisableAccessGen5 (
  IN  UINT8  IioIndex,
  IN  UINT8  PortIndex
  )
{
  UINT32     Acpv;
  UINT8      BoxInst;
  UINT8      MaxPortPerIou;

  MaxPortPerIou = GetMaxPortNumPerIou ();
  if (MaxPortPerIou == 0) {
    return;
  }
  BoxInst = (PortIndex + (MaxPortPerIou - 1)) / MaxPortPerIou;

  //Remove HOSTIA_BOOT_SAI from Gen5 DFX1 SAI policy
  Acpv = IioReadCpuCsr32 (IioIndex, BoxInst, ACRDPR5_N0_SB_PRVT_PCIE_G5_REG);
  Acpv &= ~BIT4;
  IioWriteCpuCsr32 (IioIndex, BoxInst, ACRDPR5_N0_SB_PRVT_PCIE_G5_REG, Acpv);
  Acpv = IioReadCpuCsr32 (IioIndex, BoxInst, ACWRPR5_N0_SB_PRVT_PCIE_G5_REG);
  Acpv &= ~BIT4;
  IioWriteCpuCsr32 (IioIndex, BoxInst, ACWRPR5_N0_SB_PRVT_PCIE_G5_REG, Acpv);

  //Remove HOSTIA_BOOT_SAI from FBLP DFX1 SAI policy
  Acpv = IioReadCpuCsr32 (IioIndex, BoxInst, ACRDPR5_N0_SB_PRVT_FBLP_REG);
  Acpv &= ~BIT4;
  IioWriteCpuCsr32 (IioIndex, BoxInst, ACRDPR5_N0_SB_PRVT_FBLP_REG, Acpv);
  Acpv = IioReadCpuCsr32 (IioIndex, BoxInst, ACWRPR5_N0_SB_PRVT_FBLP_REG);
  Acpv &= ~BIT4;
  IioWriteCpuCsr32 (IioIndex, BoxInst, ACWRPR5_N0_SB_PRVT_FBLP_REG, Acpv);

}


VOID
IioPcieDisableAccess (
  IN  IIO_GLOBALS                                   *IioGlobalData,
  IN  UINT8                                          IioIndex,
  IN  UINT8                                          PortIndex
)
{
  if (PortIndex & BIT0){
    IioPcieDisableAccessGen5 (IioIndex, PortIndex); // Port 1/3/5/7...is Gen5 port
  } else {
    IioPcieDisableAccessGen4 (IioIndex, PortIndex); // Port 0/2/4/6... is Gen4 port
  }

}
/**
  Restrict access to PCH link port configuration

  For EIP gen4 based controllers

  @param[in] IioGlobalData - Pointer to IIO global data.
  @param[in] IioIndex      - Socket number of the port being configured.
  @param[in] PortIndex     - Port number being configured.
**/
VOID
IioRestrictPchLinkPortDfxAccess (
  IN  IIO_GLOBALS                                   *IioGlobalData,
  IN  UINT8                                          IioIndex,
  IN  UINT8                                          PortIndex
)
{
  IioPcieDisableAccess (IioGlobalData, IioIndex, PortIndex);
}

/**
  Enable clock gating configuration settings for PCIe Gen4 Link ports

  For SPR PCIe Gen4 controllers

  @param[in] IioIndex      - Socket number of the port being configured.
  @param[in] StackIndex    - Stack number being configured.
**/
VOID
IioPcieGen4ClkGatingEn (
  IN  UINT8                                          IioIndex,
  IN  UINT8                                          StackIndex
)
{
  IOSFDEVCLKGCTL_SB_PRVT_PCIE_G4_STRUCT          IosfDevClkGCtl;
  SBDEVCLKGCTL_SB_PRVT_PCIE_G4_STRUCT            SbDevClkGCtl;
  PT0TXNCLKGCTL_SB_PRVT_PCIE_G4_STRUCT           PtNTxnClkGCtl;
  PT0LNKCLKGCTL_SB_PRVT_PCIE_G4_STRUCT           PtNLnkClkGCtl;
  PT0PHYCLKGCTL_SB_PRVT_PCIE_G4_STRUCT           PtNPhyClkGCtl;
  CFGPCIECLKGCTL_SB_PRVT_PCIE_G4_STRUCT          CfgPcieClkGCtl;
  PLLSTBYCTL_SB_PRVT_PCIE_G4_STRUCT              PllStbyCtl;
  UINT8                                          lnk;
  UINT8                                          BoxInst;

  BoxInst = StackIndex;

  //
  //  [id: 2006652770]: ICX Redefinition: Clock gating programming on PCIE gen4
  //
  IosfDevClkGCtl.Data = IioReadCpuCsr16 (IioIndex, BoxInst, IOSFDEVCLKGCTL_SB_PRVT_PCIE_G4_REG);
  IosfDevClkGCtl.Bits.icge = 1;
  if (!IosfDevClkGCtl.Bits.ict) {
    IosfDevClkGCtl.Bits.ict = 16;  //16 (default value) is the minimum number of clocks recommended
  }
  IioWriteCpuCsr16 (IioIndex, BoxInst, IOSFDEVCLKGCTL_SB_PRVT_PCIE_G4_REG, IosfDevClkGCtl.Data);

  SbDevClkGCtl.Data = IioReadCpuCsr16 (IioIndex, BoxInst, SBDEVCLKGCTL_SB_PRVT_PCIE_G4_REG);
  SbDevClkGCtl.Bits.icge = 1;
  if (!SbDevClkGCtl.Bits.ict) {
    SbDevClkGCtl.Bits.ict = 16;  //16 (default value) is the minimum number of clocks recommended
  }
  IioWriteCpuCsr16 (IioIndex, BoxInst, SBDEVCLKGCTL_SB_PRVT_PCIE_G4_REG, SbDevClkGCtl.Data);

  for (lnk = 0; lnk < PORT_LINK_WIDTH_x4; lnk++) {
    PtNTxnClkGCtl.Data = IioReadCpuCsr16 (IioIndex, BoxInst, PT0TXNCLKGCTL_SB_PRVT_PCIE_G4_REG + (sizeof(UINT16) * lnk));
    PtNTxnClkGCtl.Bits.icge = 1;
    if (!PtNTxnClkGCtl.Bits.ict) {
      PtNTxnClkGCtl.Bits.ict = 16;  //16 (default value) is the minimum number of clocks recommended
    }
    IioWriteCpuCsr16 (IioIndex, BoxInst, PT0TXNCLKGCTL_SB_PRVT_PCIE_G4_REG + (sizeof(UINT16) * lnk), PtNTxnClkGCtl.Data);

    PtNLnkClkGCtl.Data = IioReadCpuCsr16 (IioIndex, BoxInst, PT0LNKCLKGCTL_SB_PRVT_PCIE_G4_REG + (sizeof(UINT16) * lnk));
    PtNLnkClkGCtl.Bits.icge = 1;
    if (!PtNLnkClkGCtl.Bits.ict) {
      PtNLnkClkGCtl.Bits.ict = 16;  //16 (default value) is the minimum number of clocks recommended
    }
    IioWriteCpuCsr16 (IioIndex, BoxInst, PT0LNKCLKGCTL_SB_PRVT_PCIE_G4_REG + (sizeof(UINT16) * lnk), PtNLnkClkGCtl.Data);

    PtNPhyClkGCtl.Data = IioReadCpuCsr16 (IioIndex, BoxInst, PT0PHYCLKGCTL_SB_PRVT_PCIE_G4_REG + (sizeof(UINT16) * lnk));
    PtNPhyClkGCtl.Bits.icge = 1;
    if (!PtNPhyClkGCtl.Bits.ict) {
      PtNPhyClkGCtl.Bits.ict = 16;  //16 (default value) is the minimum number of clocks recommended
    }
    IioWriteCpuCsr16 (IioIndex, BoxInst, PT0PHYCLKGCTL_SB_PRVT_PCIE_G4_REG + (sizeof(UINT16) * lnk), PtNPhyClkGCtl.Data);
  }


  CfgPcieClkGCtl.Data = IioReadCpuCsr16 (IioIndex, BoxInst, CFGPCIECLKGCTL_SB_PRVT_PCIE_G4_REG);
  CfgPcieClkGCtl.Bits.icge = 1;
  if (!CfgPcieClkGCtl.Bits.ict) {
    CfgPcieClkGCtl.Bits.ict = 16;  //16 (default value) is the minimum number of clocks recommended
  }
  IioWriteCpuCsr16 (IioIndex, BoxInst, CFGPCIECLKGCTL_SB_PRVT_PCIE_G4_REG, CfgPcieClkGCtl.Data);

  PllStbyCtl.Data = IioReadCpuCsr32 (IioIndex, BoxInst, PLLSTBYCTL_SB_PRVT_PCIE_G4_REG);
  PllStbyCtl.Bits.pclk_trunk_cge = 1;
  IioWriteCpuCsr32 (IioIndex, BoxInst, PLLSTBYCTL_SB_PRVT_PCIE_G4_REG, PllStbyCtl.Data);
}


/**
  Enable clock gating configuration settings for PCIe Gen5 Link ports

  For SPR PCIe Gen5 controllers

  @param[in] IioIndex      - Socket number of the port being configured.
  @param[in] StackIndex    - Stack number being configured.
**/

VOID
IioPcieGen5ClkGatingEn (
  IN  UINT8                                          IioIndex,
  IN  UINT8                                          StackIndex
)
{
  IOSFDEVCLKGCTL_SB_PRVT_PCIE_G5_STRUCT          IosfDevClkGCtl;
  SBDEVCLKGCTL_SB_PRVT_PCIE_G5_STRUCT            SbDevClkGCtl;
  PT0TXNCLKGCTL_SB_PRVT_PCIE_G5_STRUCT           PtNTxnClkGCtl;
  PT0LNKCLKGCTL_SB_PRVT_PCIE_G5_STRUCT           PtNLnkClkGCtl;
  PT0LDPHYCLKGCTL_SB_PRVT_PCIE_G5_STRUCT         PtLdPhyClkGCtl;
  CFGPCIECLKGCTL_SB_PRVT_PCIE_G5_STRUCT          CfgPcieClkGCtl;
  UINT8                                          lnk;
  UINT8                                          BoxInst;

  BoxInst = StackIndex;

  IosfDevClkGCtl.Data = IioReadCpuCsr16 (IioIndex, BoxInst, IOSFDEVCLKGCTL_SB_PRVT_PCIE_G5_REG);
  IosfDevClkGCtl.Bits.icge = 1;
  if (!IosfDevClkGCtl.Bits.ict) {
    IosfDevClkGCtl.Bits.ict = 16;  //16 (default value) is the minimum number of clocks recommended
  }
  IioWriteCpuCsr16 (IioIndex, BoxInst, IOSFDEVCLKGCTL_SB_PRVT_PCIE_G5_REG, IosfDevClkGCtl.Data);
  SbDevClkGCtl.Data = IioReadCpuCsr16 (IioIndex, BoxInst, SBDEVCLKGCTL_SB_PRVT_PCIE_G5_REG);
  SbDevClkGCtl.Bits.icge = 1;
  if (!SbDevClkGCtl.Bits.ict) {
    SbDevClkGCtl.Bits.ict = 16;  //16 (default value) is the minimum number of clocks recommended
  }
  IioWriteCpuCsr16 (IioIndex, BoxInst, SBDEVCLKGCTL_SB_PRVT_PCIE_G5_REG, SbDevClkGCtl.Data);
  for (lnk = 0; lnk < PORT_LINK_WIDTH_x4; lnk++) {
    PtNTxnClkGCtl.Data = IioReadCpuCsr16 (IioIndex, BoxInst, PT0TXNCLKGCTL_SB_PRVT_PCIE_G5_REG + (sizeof(UINT16) * lnk));
    PtNTxnClkGCtl.Bits.icge = 1;
    if (!PtNTxnClkGCtl.Bits.ict) {
      PtNTxnClkGCtl.Bits.ict = 16;  //16 (default value) is the minimum number of clocks recommended
    }
    IioWriteCpuCsr16 (IioIndex, BoxInst, PT0TXNCLKGCTL_SB_PRVT_PCIE_G5_REG + (sizeof(UINT16) * lnk), PtNTxnClkGCtl.Data);

    PtNLnkClkGCtl.Data = IioReadCpuCsr16 (IioIndex, BoxInst, PT0LNKCLKGCTL_SB_PRVT_PCIE_G5_REG + (sizeof(UINT16) * lnk));
    PtNLnkClkGCtl.Bits.icge = 1;
    if (!PtNLnkClkGCtl.Bits.ict) {
      PtNLnkClkGCtl.Bits.ict = 16;  //16 (default value) is the minimum number of clocks recommended
    }
    IioWriteCpuCsr16 (IioIndex, BoxInst, PT0LNKCLKGCTL_SB_PRVT_PCIE_G5_REG + (sizeof(UINT16) * lnk), PtNLnkClkGCtl.Data);

    PtLdPhyClkGCtl.Data = IioReadCpuCsr16 (IioIndex, BoxInst, PT0LDPHYCLKGCTL_SB_PRVT_PCIE_G5_REG + (sizeof(UINT16) * lnk));
    PtLdPhyClkGCtl.Bits.icge = 1;
    if (!PtLdPhyClkGCtl.Bits.ict) {
      PtLdPhyClkGCtl.Bits.ict = 16;  //16 (default value) is the minimum number of clocks recommended
    }
    IioWriteCpuCsr16 (IioIndex, BoxInst, PT0LDPHYCLKGCTL_SB_PRVT_PCIE_G5_REG + (sizeof(UINT16) * lnk), PtLdPhyClkGCtl.Data);
  }

  CfgPcieClkGCtl.Data = IioReadCpuCsr16 (IioIndex, BoxInst, CFGPCIECLKGCTL_SB_PRVT_PCIE_G5_REG);
  CfgPcieClkGCtl.Bits.icge = 1;
  if (!CfgPcieClkGCtl.Bits.ict) {
    CfgPcieClkGCtl.Bits.ict = 16;  //16 (default value) is the minimum number of clocks recommended
  }
  IioWriteCpuCsr16 (IioIndex, BoxInst, CFGPCIECLKGCTL_SB_PRVT_PCIE_G5_REG, CfgPcieClkGCtl.Data);
}

/**
  Enable clock gating configuration settings for All PSF.

  For SPR PSF

  @param[in] IioIndex      - Socket number of the port being configured.
**/
VOID
IioPsfClkGatingEn (
  IN  UINT8                                          IioIndex
)
{
  PSF_1_PSF_GLOBAL_CONFIG_SB_PSF_STRUCT            Psf1GlobalConfig;
  PSF_1_PSF_GLOBAL_CONFIG_SB_PSF_DINO_STRUCT       Psf1GlobalConfigDino;
  PSF_2_PSF_GLOBAL_CONFIG_SB_PSF_HCX_STRUCT        Psf2GlobalConfigHcx;
  UINT8                                            StackIndex;
  UINT8                                            BoxInst;

  for (StackIndex = 0; StackIndex < MAX_IIO_STACK_SPR; StackIndex++) {
    if (IfStackPresent (IioIndex, StackIndex) == FALSE) {
      continue;
    }
    switch (StackIndex) {
    case IIO_STACK0:
    case IIO_STACK1:
    case IIO_STACK2:
    case IIO_STACK3:
    case IIO_STACK4:
    case IIO_STACK5:
      BoxInst = StackIndex;
      Psf1GlobalConfig.Data = IioReadCpuCsr32 (IioIndex, BoxInst, PSF_1_PSF_GLOBAL_CONFIG_SB_PSF_REG);
      Psf1GlobalConfig.Bits.entcg = 1;
      IioWriteCpuCsr32 (IioIndex, BoxInst, PSF_1_PSF_GLOBAL_CONFIG_SB_PSF_REG, Psf1GlobalConfig.Data);
      break;

    case IIO_STACK8:
    case IIO_STACK9:
    case IIO_STACK10:
    case IIO_STACK11:
      BoxInst = StackIndex - IIO_STACK8;
      Psf1GlobalConfigDino.Data = IioReadCpuCsr32 (IioIndex, BoxInst, PSF_1_PSF_GLOBAL_CONFIG_SB_PSF_DINO_REG);
      Psf1GlobalConfigDino.Bits.entcg = 1;
      IioWriteCpuCsr32 (IioIndex, BoxInst, PSF_1_PSF_GLOBAL_CONFIG_SB_PSF_DINO_REG, Psf1GlobalConfigDino.Data);
      if (!IsHbmSku()) {  // No HCx on HBM SKU
        Psf2GlobalConfigHcx.Data = IioReadCpuCsr32 (IioIndex, BoxInst, PSF_2_PSF_GLOBAL_CONFIG_SB_PSF_HCX_REG);
        Psf2GlobalConfigHcx.Bits.entcg = 1;
        IioWriteCpuCsr32 (IioIndex, BoxInst, PSF_2_PSF_GLOBAL_CONFIG_SB_PSF_HCX_REG, Psf2GlobalConfigHcx.Data);
      }
      break;

    default:
      break;
    }
  }
}

/**
  Enable clock gating configuration settings for North Bridge DMI ports

  For SPR DMI controllers

  @param[in] IioGlobalData - Pointer to IIO global data.
  @param[in] IioIndex      - Socket number of the port being configured.
  @param[in] PortIndex     - Port number being configured.
**/
VOID
IioNorthBridgePortClkGatingEn (
  IN  IIO_GLOBALS                                   *IioGlobalData,
  IN  UINT8                                          IioIndex,
  IN  UINT8                                          PortIndex
)
{
  IioPsfClkGatingEn (IioIndex);
  IioPcieGen4ClkGatingEn (IioIndex, IIO_STACK0); // Gen4 clock gating
}

/**
  Enabling clock gating configuration settings for PCIe ports

  For PCIe controllers

  @param[in] IioGlobalData - Pointer to IIO Globals
  @param[in] IioIndex      - Socket number of the port being configured.
  @param[in] PortIndex     - Port number being configured.
**/
VOID
IioPcieClkGatingEn (
  IN  IIO_GLOBALS                                   *IioGlobalData,
  IN  UINT8                                          IioIndex,
  IN  UINT8                                          PortIndex
)
{
  UINT8                                              StackIndex = 0;
  EFI_STATUS                                         Status;

  if (IsSuperClusterPort(PortIndex)) {
    Status = GetPcieStackPerPort (IioIndex, PortIndex, &StackIndex);
    if (Status != EFI_SUCCESS) {
      return;
    }
    if (GetStackMode (IioGlobalData, IioIndex, StackIndex) != PI5_MODE_GEN4_ONLY) {
      IioPcieGen5ClkGatingEn (IioIndex, StackIndex);
    }
    IioPcieGen4ClkGatingEn (IioIndex, StackIndex);
  }
}

/**
  Enable ASPM L1 configuration settings for North Bridge DMI

  @param[in] IioGlobalData - Pointer to IIO global data.
  @param[in] IioIndex      - Socket number of the port being configured.
  @param[in] PortIndex     - Port number being configured.
**/
VOID
IioNorthBridgePortL1En (
  IN  IIO_GLOBALS                                   *IioGlobalData,
  IN  UINT8                                          IioIndex,
  IN  UINT8                                          PortIndex
)
{
  DEBUG ((DEBUG_VERBOSE,"No DMI specific programming, IioIndex = 0x%x, PortIndex = 0x%x\n", IioIndex, PortIndex));
}
