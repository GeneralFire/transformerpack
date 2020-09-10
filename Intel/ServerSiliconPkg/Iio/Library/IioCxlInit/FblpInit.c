/** @file
  This file initialize the FBLP device on IIO

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

#include <Library/IioDataHubLib.h>
#include <Library/IioAccessLib.h>
#include <LibraryPrivate/IioDebug.h>
#include <RcRegs.h>
#include <Library/KtiApi.h>
#include <Library/CxlIpLib.h>
#include <Library/IioSysInfoLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/RcrbAccessLib.h>

/**
  Program clock configuration for Flex Bus Log Phy

  For SPR FBLP

  @param IioIndex      - Socket number of the port being configured.
  @param PortIndex     - Port number being configured.
**/
VOID
IioFblpCommonClockInit (
  IN  UINT8                                          IioIndex,
  IN  UINT8                                          PortIndex
)
{
  IIO_CONFIG                          *SetupData = NULL;
  UINT8                               MaxPortNumberPerSocket = 0;
  LINKCTL_SB_FBLP_STRUCT              LinkCtl;

  SetupData = GetIioConfig ();
  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);

  LinkCtl.Data = IioReadCpuCsr16 (IioIndex, PortIndex, LINKCTL_SB_FBLP_REG);
  if ((IIO_OPTION_DISABLE == SetupData->SRIS[IioIndex * MaxPortNumberPerSocket + PortIndex]) &&
      (IIO_OPTION_ENABLE == SetupData->PcieCommonClock[IioIndex * MaxPortNumberPerSocket + PortIndex])) {
    LinkCtl.Bits.cccfg = 1;
  } else {
    LinkCtl.Bits.cccfg = 0;
  }
  IioWriteCpuCsr16 (IioIndex, PortIndex, LINKCTL_SB_FBLP_REG, LinkCtl.Data);
}

/**
  Enable clock gating configuration settings for Flex Bus Log Phy

  For SPR FBLP

  @param IioIndex      - Socket number of the port being configured.
  @param PortIndex     - Port number being configured.
**/
VOID
IioFblpClkGatingEn (
  IN  UINT8                                          IioIndex,
  IN  UINT8                                          PortIndex
)
{
  PT0PHYCLKGCTL_SB_FBLP_STRUCT           PtNPhyClkGCtl;
  FBLP_CFGPCIECLKGCTL_SB_FBLP_STRUCT     CfgPcieClkGCtl;
  PLLSTBYCTL_SB_FBLP_STRUCT              PllStbyCtl;
  CSR_SBDEVCLKGCTL_SB_FBLP_STRUCT        CsrSbDevClkGCtl;
  STAGGERCTL_SB_FBLP_STRUCT              StagGerCtl;
  UINT8                                  lnk = 0;
  UINT8                                  BoxInst = 0;

  BoxInst = PortIndex;
  for (lnk = 0; lnk < PORT_LINK_WIDTH_x4; lnk++) {
    PtNPhyClkGCtl.Data = IioReadCpuCsr16 (IioIndex, BoxInst, PT0PHYCLKGCTL_SB_FBLP_REG + (sizeof(UINT16) * lnk));
    PtNPhyClkGCtl.Bits.icge = 1;
    if (!PtNPhyClkGCtl.Bits.ict) {
      PtNPhyClkGCtl.Bits.ict = 16;  //16 (default value) is the minimum number of clocks recommended
    }
    IioWriteCpuCsr16 (IioIndex, BoxInst, PT0PHYCLKGCTL_SB_FBLP_REG + (sizeof(UINT16) * lnk), PtNPhyClkGCtl.Data);
  }
  //
  // HSD 2209372841 Implement 24ns lane to stagger
  //
  StagGerCtl.Data = IioReadCpuCsr16 (IioIndex, BoxInst, STAGGERCTL_SB_FBLP_REG);
  StagGerCtl.Bits.mode = 1;
  StagGerCtl.Bits.rxstaggeren = 1;
  StagGerCtl.Bits.dlycnt = 3;
  StagGerCtl.Bits.dlyunit = 1;
  StagGerCtl.Bits.lnrststaggeren = 0;
  IioWriteCpuCsr16 (IioIndex, BoxInst, STAGGERCTL_SB_FBLP_REG, StagGerCtl.Data);

  CfgPcieClkGCtl.Data = IioReadCpuCsr16 (IioIndex, BoxInst, FBLP_CFGPCIECLKGCTL_SB_FBLP_REG);
  CfgPcieClkGCtl.Bits.icge = 1;
  if (!CfgPcieClkGCtl.Bits.ict) {
    CfgPcieClkGCtl.Bits.ict = 16;  //16 (default value) is the minimum number of clocks recommended
  }
  IioWriteCpuCsr16 (IioIndex, BoxInst, FBLP_CFGPCIECLKGCTL_SB_FBLP_REG, CfgPcieClkGCtl.Data);

  PllStbyCtl.Data = IioReadCpuCsr32 (IioIndex, BoxInst, PLLSTBYCTL_SB_FBLP_REG);
  PllStbyCtl.Bits.pclk_trunk_cge = 1;
  IioWriteCpuCsr32 (IioIndex, BoxInst, PLLSTBYCTL_SB_FBLP_REG, PllStbyCtl.Data);

  CsrSbDevClkGCtl.Data = IioReadCpuCsr16 (IioIndex, BoxInst, CSR_SBDEVCLKGCTL_SB_FBLP_REG);
  CsrSbDevClkGCtl.Bits.icge = 1;
  IioWriteCpuCsr16 (IioIndex, BoxInst, CSR_SBDEVCLKGCTL_SB_FBLP_REG, CsrSbDevClkGCtl.Data);
}

/**

  FBLP Pre Train initialization.

  @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
  @param IioIndex           - Index of CPU/IIO

  @retval None

**/
VOID
FblpPreTrainInit (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  )
{
  IIO_CONFIG                                             *SetupData = NULL;
  UINT8                                                   PortIndex = 0;
  UINT8                                                   StackIndex = 0;
  UINT8                                                   MaxPortNumberPerSocket = 0;
  UINT8                                                   RetimerConnectCount = 0;
  PI5_MODES                                               StackMode;
  IAPCTL_SB_FBLP_STRUCT                                   IapCtl;
  IAPCTL2_SB_FBLP_SPRB0_SPRHBM_SPRC0_SPRMCC_SPRUCC_STRUCT IapCtl2;
  IALPMMCTL_IIO_CXL_STRUCT                                IalpMmctl;
  DESKEWCTL_SB_FBLP_STRUCT                                DeskewCtl;
  FLXBUSPTCTL_SB_FBLP_STRUCT                              FxlBusPtCtl;
  G5CAP_SB_FBLP_STRUCT                                    G5Cap;
  G5CTL_SB_FBLP_STRUCT                                    G5Ctl;
  LTSSMSTATEJMP_SB_FBLP_STRUCT                            LtssmStateJmp;
  RTLLCTL_SB_FBLP_STRUCT                                  RtllCtl;
  LTSSMTRNCTL2_SB_FBLP_STRUCT                             LtssmTrnCtl2;
  APCTL_SB_FBLP_SPRC0_SPRMCC_SPRUCC_STRUCT                ApctlC0Stepping;
  EFI_STATUS                                              Status;


  IIO_D_LOG ("FBLP_PRE_TRAIN_INIT_START\n");

  SetupData = GetIioConfig ();
  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);

  for (PortIndex = 1; PortIndex < MaxPortNumberPerSocket; PortIndex++) {
    if (!IioVData_ConfigurePciePort (IioGlobalData,IioIndex, PortIndex)) {
      continue;
    }

    if (IIO_GEN5 != IioPortControllerGen (IioGlobalData, IioIndex, PortIndex)) {
      continue;
    }

    Status = GetPcieStackPerPort (IioIndex, PortIndex, &StackIndex);
    if (Status != EFI_SUCCESS) {
      continue;
    }

    StackMode = GetStackMode (IioGlobalData, IioIndex, StackIndex);

    RetimerConnectCount = IioGlobalData->IioVar.IioOutData.RetimerConnectCount[IioIndex * MaxPortNumberPerSocket + PortIndex];

    LtssmStateJmp.Data = IioReadCpuCsr32 (IioIndex, PortIndex, LTSSMSTATEJMP_SB_FBLP_REG);
    if (IioIsHpEnabledOnPort (IioIndex, PortIndex)) {
      LtssmStateJmp.Bits.no_compliance = 1;
    } else {
      LtssmStateJmp.Bits.no_compliance = 0;
    }
    IioWriteCpuCsr32 (IioIndex, PortIndex, LTSSMSTATEJMP_SB_FBLP_REG, LtssmStateJmp.Data);

    if (!IsSiliconWorkaroundEnabled ("S14010882657")) { // This workaround will init RtllCtll after link training.
      RtllCtl.Data = IioReadCpuCsr32 (IioIndex, PortIndex, RTLLCTL_SB_FBLP_REG);
      RtllCtl.Bits.enable = ((0 == RetimerConnectCount)?0:3);
      IioWriteCpuCsr32 (IioIndex, PortIndex, RTLLCTL_SB_FBLP_REG, RtllCtl.Data);
    }
    if (IsSuperClusterPort(PortIndex) && (PI5_MODE_IAL == StackMode)) {

      DeskewCtl.Data = IioReadCpuCsr32 (IioIndex, PortIndex, DESKEWCTL_SB_FBLP_REG);
      // SPR_TODO: Need to enable low latency mode support after power on.
      // DeskewCtl.Bits.idben = 1;
      // DeskewCtl.Bits.rxslipen = 1;
      IioWriteCpuCsr32 (IioIndex, PortIndex, DESKEWCTL_SB_FBLP_REG, DeskewCtl.Data);

      G5Cap.Data = IioReadCpuCsr32 (IioIndex, PortIndex, G5CAP_SB_FBLP_REG);
      G5Cap.Bits.modtsusgmd2sup = 1;
      IioWriteCpuCsr32 (IioIndex, PortIndex, G5CAP_SB_FBLP_REG, G5Cap.Data);
      if (IsCpuAndRevisionOrLater (CPU_SPRSP, REV_C0)) {
        ApctlC0Stepping.Data = IioReadCpuCsr32 (IioIndex, PortIndex, APCTL_SB_FBLP_REG);
        ApctlC0Stepping.Bits.apnge = 1;
        IioWriteCpuCsr32 (IioIndex, PortIndex, APCTL_SB_FBLP_REG, ApctlC0Stepping.Data);
      }
      G5Ctl.Data = IioReadCpuCsr32 (IioIndex, PortIndex, G5CTL_SB_FBLP_REG);
      if (IsSiliconWorkaroundEnabled ("S22010581486")) {
        G5Ctl.Bits.modtsusgsel = 0;
      } else {
        G5Ctl.Bits.modtsusgsel = 2;
      }
      IioWriteCpuCsr32 (IioIndex, PortIndex, G5CTL_SB_FBLP_REG, G5Ctl.Data);

      FxlBusPtCtl.Data = IioReadCpuCsr16 (IioIndex, PortIndex, FLXBUSPTCTL_SB_FBLP_REG);
      FxlBusPtCtl.Bits.cache_en = 1;
      FxlBusPtCtl.Bits.mem_en = 1;
      if ((IIO_OPTION_DISABLE == SetupData->SRIS[IioIndex * MaxPortNumberPerSocket + PortIndex]) &&
          (IIO_OPTION_ENABLE == SetupData->PcieCommonClock[IioIndex * MaxPortNumberPerSocket + PortIndex])) {
        FxlBusPtCtl.Bits.synchdrbypen = 1;
      } else {
        FxlBusPtCtl.Bits.synchdrbypen = 0;
      }
      FxlBusPtCtl.Bits.rt1_pres = ((0 == RetimerConnectCount)?0:1);
      FxlBusPtCtl.Bits.rt2_pres = ((1 >= RetimerConnectCount)?0:1);
      IioWriteCpuCsr16 (IioIndex, PortIndex, FLXBUSPTCTL_SB_FBLP_REG, FxlBusPtCtl.Data);

      IapCtl.Data = IioReadCpuCsr32 (IioIndex, PortIndex, IAPCTL_SB_FBLP_REG);
      IapCtl.Bits.ial_en = 1;
      if (SetupData->PcieSubSystemMode[IioIndex][StackIndex - 1] == IIO_MODE_FORCE_CXL) {
        IapCtl.Bits.force_ial = 1;
        IapCtl.Bits.mem = 1;
        IapCtl.Bits.cache = 1;
        IapCtl.Bits.rt1ial = ((0 == RetimerConnectCount)?0:1);
        IapCtl.Bits.rt2ial = ((1 >= RetimerConnectCount)?0:1);
        if ((IIO_OPTION_DISABLE == SetupData->SRIS[IioIndex * MaxPortNumberPerSocket + PortIndex]) &&
            (IIO_OPTION_ENABLE == SetupData->PcieCommonClock[IioIndex * MaxPortNumberPerSocket + PortIndex])) {
          IapCtl.Bits.comclk = 1;
          IapCtl.Bits.hdrbyp = 1;
          if (IsSiliconWorkaroundEnabled ("S22010581486")) {
            if (!SetupData->CxlHeaderBypass) {
              IapCtl.Bits.hdrbyp = 0;
            }
          }
        } else {
          IapCtl.Bits.comclk = 0;
          IapCtl.Bits.hdrbyp = 0;
        }
      } else {
        //
        // Default value for sticky register
        //
        IapCtl.Bits.force_ial = 0;
        IapCtl.Bits.mem = 0;
        IapCtl.Bits.cache = 0;
        IapCtl.Bits.rt1ial = 0;
        IapCtl.Bits.rt2ial = 0;
        IapCtl.Bits.comclk = 0;
        IapCtl.Bits.hdrbyp = 0;
      }
      IioWriteCpuCsr32 (IioIndex, PortIndex, IAPCTL_SB_FBLP_REG, IapCtl.Data);
    }

    if (IsSuperClusterPort(PortIndex) && IsSiliconWorkaroundEnabled("S22010274542") && OverrideCxlVid()) {
      IapCtl2.Data = IioReadCpuCsr32 (IioIndex, PortIndex, IAPCTL2_SB_FBLP_REG);
      IapCtl2.Bits.vid = 0x1E98;
      IioWriteCpuCsr32 (IioIndex, PortIndex, IAPCTL2_SB_FBLP_REG, IapCtl2.Data);

      SetRcrbBar (IioIndex, StackIndex, TYPE_CXL_RCRB, IioGlobalData->IioVar.IioVData.SocketStackMmiolBase[IioIndex][StackIndex]);
      IalpMmctl.Data = IioReadCpuCsr32 (IioIndex, StackIndex - 1, IALPMMCTL_IIO_CXL_REG);
      IalpMmctl.Bits.vid = 0x1E98;
      IioWriteCpuCsr32 (IioIndex, StackIndex - 1, IALPMMCTL_IIO_CXL_REG, IalpMmctl.Data);
      ClearRcrbBar (IioIndex, StackIndex, TYPE_CXL_RCRB);
    }

    if (IsSiliconWorkaroundEnabled ("S14010544275")) {
      LtssmTrnCtl2.Data = IioReadCpuCsr32 (IioIndex, PortIndex, LTSSMTRNCTL2_SB_FBLP_REG);
      LtssmTrnCtl2.Bits.txrxdetpolatt = 3;
      IioWriteCpuCsr32 (IioIndex, PortIndex, LTSSMTRNCTL2_SB_FBLP_REG, LtssmTrnCtl2.Data);
    }
  }

  IIO_D_LOG ("FBLP_PRE_TRAIN_INIT_END\n");
  return;
}

/**

  FBLP Post Train initialization.

  @param IioGlobalData      - Pointer to IIO Universal Data Structure (UDS)
  @param IioIndex           - Index of CPU/IIO

  @retval None

**/
VOID
FblpPostTrainInit (
  IIO_GLOBALS                        *IioGlobalData,
  UINT8                              IioIndex
  )
{
  IIO_CONFIG                          *SetupData = NULL;
  UINT8                               PortIndex = 0;
  UINT8                               StackIndex = 0;
  UINT8                               MaxPortNumberPerSocket = 0;
  IAPCTL_SB_FBLP_STRUCT               IapCtl;
  EFI_STATUS                          Status;

  IIO_D_LOG ("FBLP_POST_TRAIN_INIT_START\n");

  SetupData = GetIioConfig ();
  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);

  for (PortIndex = 1; PortIndex < MaxPortNumberPerSocket; PortIndex++) {
    if (!IioVData_ConfigurePciePort (IioGlobalData,IioIndex, PortIndex)) {
      continue;
    }
    if (IsSuperClusterPort (PortIndex) && (IIO_GEN5 == IioPortControllerGen (IioGlobalData, IioIndex, PortIndex))) {
      Status = GetPcieStackPerPort (IioIndex, PortIndex, &StackIndex);
      if (Status != EFI_SUCCESS) {
        continue;
      }
      // Disable ial_en if not CXL mode.
      if (GetCxlStatus (IioIndex, StackIndex) != AlreadyInCxlMode) {
        IapCtl.Data = IioReadCpuCsr32 (IioIndex, PortIndex, IAPCTL_SB_FBLP_REG);
        IapCtl.Bits.ial_en = 0;
        //
        // Default value for sticky register if not in CXL mode
        //
        IapCtl.Bits.force_ial = 0;
        IapCtl.Bits.mem = 0;
        IapCtl.Bits.cache = 0;
        IapCtl.Bits.rt1ial = 0;
        IapCtl.Bits.rt2ial = 0;
        IapCtl.Bits.comclk = 0;
        IapCtl.Bits.hdrbyp = 0;
        IioWriteCpuCsr32 (IioIndex, PortIndex, IAPCTL_SB_FBLP_REG, IapCtl.Data);
      }
    }
  }

  for (PortIndex = 0; PortIndex < MaxPortNumberPerSocket; PortIndex++) {
    if ((PortIndex == 0) && IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {  // for port 0 on every socket
      if (!IfStackPresent (IioIndex, 0)){
        continue;
      }
    } else {
      if (!IioVData_ConfigurePciePort (IioGlobalData,IioIndex, PortIndex)) {
        continue;
      }
    }
    if ((PortIndex & BIT0) || (PortIndex == 0)) { // Program for Gen5/CXL and DMI
      IioFblpCommonClockInit (IioIndex, PortIndex);
      if (IIO_OPTION_ENABLE == SetupData->IioPcieConfig.PciePortClkGateEnable[PortIndex]) {
        IioFblpClkGatingEn (IioIndex, PortIndex);
      }
    }
  }

  IIO_D_LOG ("FBLP_POST_TRAIN_INIT_END\n");
  return;
}
