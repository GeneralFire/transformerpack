/** @file
  Pei HSIO common Library

@copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification
**/

#include <Library/DebugLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchSbiAccessLib.h>
#include <Library/PchPcrLib.h>
#include <Register/PchPcrRegs.h>
#include <Register/PchRegsHsio.h>
#include <Register/PchRegsHsioVer3.h>
#include <Library/PeiHsioLib.h>
#include <Library/HsioSocLib.h>
#include <Library/ChipsetInitLib.h>
#include <Library/SataSocLib.h>
#include <Library/PchFiaLib.h>    // TODO: remove this dependency
#include <Library/PchInfoLib.h>   // TODO: remove when EBG case will handled at SoC layer

/**
  Initialize HSIO lane for non-common clock PCIe port.

  @param[in] HsiLane   HSIO Lane
**/
VOID
HsioPcieNccLaneInit (
  CONST HSIO_LANE     *HsioLane
  )
{
  UINT32  Data32Or;
  UINT32  Mask32;

  DEBUG ((DEBUG_INFO, "HsioPcieNccLaneInit(%d)\n", HsioLane->Index));

  Mask32    = B_HSIO_PCR_PCS_DWORD8_CRI_RXEB_LOWATER_4_0 |
    B_HSIO_PCR_PCS_DWORD8_CRI_RXEB_HIWATER_4_0;
  Data32Or  = ( 9 << N_HSIO_PCR_PCS_DWORD8_CRI_RXEB_LOWATER_4_0) |
    (24 << N_HSIO_PCR_PCS_DWORD8_CRI_RXEB_HIWATER_4_0);

  HsioLaneAndThenOr32 (HsioLane, R_HSIO_PCR_PCS_DWORD8, ~Mask32, Data32Or);
}

VOID
PchHsioEnablePcieComplianceMode (
  VOID
  )
{
  UINT32     MaxPcieLanes;
  UINT32     PcieIndex;
  UINT8      HsioLaneNo;
  HSIO_LANE  HsioLane;

  MaxPcieLanes = GetPchMaxPciePortNum ();
  for (PcieIndex = 0; PcieIndex < MaxPcieLanes; ++PcieIndex) {
    if (PchFiaGetPcieLaneNum (PcieIndex, &HsioLaneNo)) {
      DEBUG ((DEBUG_INFO, "PCIe compliance mode for PCIe%d, HSIO%d\n", PcieIndex + 1, HsioLaneNo));
      HsioGetLane (HsioLaneNo, &HsioLane);
      HsioLaneAndThenOr32WithSusWrite (&HsioLane, R_HSIO_PCR_PCS_DWORD4, (UINT32) ~BIT23, 0);

      HsioLaneAndThenOr32WithSusWrite (
        &HsioLane, R_HSIO_PCR_RX_DWORD24,
        (UINT32) ~B_HSIO_PCR_RX_DWORD24_O_CALIB_RECAL_TARG_35_32,
        V_HSIO_PCR_RX_DWORD24_O_CALIB_RECAL_TARG_COMPLIANCE
        );

      HsioLaneAndThenOr32WithSusWrite (
        &HsioLane, R_HSIO_PCR_RX_DWORD2,
        (UINT32) ~B_HSIO_PCR_RX_DWORD2_O_CALIB_RECAL_TARG_7_0,
        V_HSIO_PCR_RX_DWORD2_O_CALIB_RECAL_COMPLIANCE
        );
    }
  }
}

/**
  Tune the USB 3.0 signals quality.

  @param[in]  Usb3HsioConfig      The USB3_HSIO_CONFIG policy instance

**/
VOID
XhciUsb3HsioTuning (
  IN  USB3_HSIO_CONFIG      *Usb3HsioConfig
  )
{
  HSIO_LANE  HsioLane;
  UINT8      Index;
  UINT8      LaneNum;
  UINT32     Data32And;
  UINT32     Data32Or;

  for (Index = 0; Index < GetPchXhciMaxUsb3PortNum (); Index++) {
    if (PchFiaGetUsb3LaneNum (Index, &LaneNum)) {
      HsioGetLane (LaneNum, &HsioLane);
      //
      //Step 1: Make changes to any of the TX (Transmit) ModPHY Register Bit Fields
      //
      //
      // USB 3.0 TX Output -3.5dB De-Emphasis Adjustment Setting (ow2tapgen2deemph3p5)
      // HSIO_TX_DWORD5[21:16]
      //
      if (Usb3HsioConfig->Port[Index].HsioTxDeEmphEnable == TRUE) {
        HsioLaneAndThenOr32WithSusWrite (
          &HsioLane,
          R_HSIO_PCR_TX_DWORD5,
          (UINT32) ~B_HSIO_PCR_TX_DWORD5_OW2TAPGEN2DEEMPH3P5_5_0,
          (UINT32) (Usb3HsioConfig->Port[Index].HsioTxDeEmph << N_HSIO_PCR_TX_DWORD5_OW2TAPGEN2DEEMPH3P5_5_0)
          );
      }

      //
      // USB 3.0 TX Output Downscale Amplitude Adjustment (orate01margin)
      // HSIO_TX_DWORD8[21:16]
      //
      if (Usb3HsioConfig->Port[Index].HsioTxDownscaleAmpEnable == TRUE) {
        HsioLaneAndThenOr32WithSusWrite (
          &HsioLane,
          R_HSIO_PCR_TX_DWORD8,
          (UINT32) ~B_HSIO_PCR_TX_DWORD8_ORATE01MARGIN_5_0,
          (UINT32) (Usb3HsioConfig->Port[Index].HsioTxDownscaleAmp << N_HSIO_PCR_TX_DWORD8_ORATE01MARGIN_5_0)
          );
      }

      if (!IsEbgPch ()) { // TODO: Remove this dependency
        Data32And = ~0u;
        Data32Or  = 0;

        //
        // USB 3.0 TX Unique Transition Bit Scale for rate 3 (rate3UniqTranScale)
        // HSIO_TX_DWORD9[6:0]
        //
        if (Usb3HsioConfig->Port[Index].HsioTxRate3UniqTranEnable == TRUE) {
          Data32And &= (UINT32) ~B_HSIO_PCR_TX_DWORD9_RATE3UNIQTRANSCALE_6_0;
          Data32Or  |= (UINT32) (B_HSIO_PCR_TX_DWORD9_RATE3UNIQTRANENABLE | (Usb3HsioConfig->Port[Index].HsioTxRate3UniqTran << N_HSIO_PCR_TX_DWORD9_RATE3UNIQTRANSCALE_6_0));
        }

        //
        // USB 3.0 TX Unique Transition Bit Scale for rate 2 (rate2UniqTranScale)
        // HSIO_TX_DWORD9[14:8]
        //
        if (Usb3HsioConfig->Port[Index].HsioTxRate2UniqTranEnable == TRUE) {
          Data32And &= (UINT32) ~B_HSIO_PCR_TX_DWORD9_RATE2UNIQTRANSCALE_6_0;
          Data32Or  |= (UINT32) (B_HSIO_PCR_TX_DWORD9_RATE2UNIQTRANENABLE | (Usb3HsioConfig->Port[Index].HsioTxRate2UniqTran << N_HSIO_PCR_TX_DWORD9_RATE2UNIQTRANSCALE_6_0));
        }

        //
        // USB 3.0 TX Unique Transition Bit Scale for rate 1 (rate1UniqTranScale)
        // HSIO_TX_DWORD9[22:16]
        //
        if (Usb3HsioConfig->Port[Index].HsioTxRate1UniqTranEnable == TRUE) {
          Data32And &= (UINT32) ~B_HSIO_PCR_TX_DWORD9_RATE1UNIQTRANSCALE_6_0;
          Data32Or  |= (UINT32) (B_HSIO_PCR_TX_DWORD9_RATE1UNIQTRANENABLE | (Usb3HsioConfig->Port[Index].HsioTxRate1UniqTran << N_HSIO_PCR_TX_DWORD9_RATE1UNIQTRANSCALE_6_0));
        }

        //
        // USB 3.0 TX Unique Transition Bit Scale for rate 0 (rate0UniqTranScale)
        // HSIO_TX_DWORD9[30:24]
        //
        if (Usb3HsioConfig->Port[Index].HsioTxRate0UniqTranEnable == TRUE) {
          Data32And &= (UINT32) ~B_HSIO_PCR_TX_DWORD9_RATE0UNIQTRANSCALE_6_0;
          Data32Or  |= (UINT32) (B_HSIO_PCR_TX_DWORD9_RATE0UNIQTRANENABLE | (Usb3HsioConfig->Port[Index].HsioTxRate0UniqTran << N_HSIO_PCR_TX_DWORD9_RATE0UNIQTRANSCALE_6_0));
        }

        if (Usb3HsioConfig->Port[Index].HsioTxRate3UniqTranEnable ||
            Usb3HsioConfig->Port[Index].HsioTxRate2UniqTranEnable ||
            Usb3HsioConfig->Port[Index].HsioTxRate1UniqTranEnable ||
            Usb3HsioConfig->Port[Index].HsioTxRate0UniqTranEnable) {
          HsioLaneAndThenOr32WithSusWrite (
            &HsioLane,
            R_HSIO_PCR_TX_DWORD9,
            Data32And,
            Data32Or
            );
        }
      }

      //
      // Signed Magnitude number added to the CTLE code.(ctle_adapt_offset_cfg_4_0)
      // HSIO_RX_DWORD25 [20:16]
      //
      if (Usb3HsioConfig->Port[Index].HsioCtrlAdaptOffsetCfgEnable == TRUE) {
        HsioLaneAndThenOr32WithSusWrite (
          &HsioLane,
          R_HSIO_PCR_RX_DWORD25,
          (UINT32) ~B_HSIO_PCR_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0,
          (UINT32) (Usb3HsioConfig->Port[Index].HsioCtrlAdaptOffsetCfg << N_HSIO_PCR_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0)
          );
      }

      Data32And = ~0u;
      Data32Or  = 0;

      //
      // LFPS filter select for n (filter_sel_n_2_0)
      // HSIO_RX_DWORD51 [29:27]
      //
      if (Usb3HsioConfig->Port[Index].HsioFilterSelNEnable == TRUE) {
        Data32And &= (UINT32)~B_HSIO_PCR_RX_DWORD51_FILTER_SEL_N_2_0;
        Data32Or  |= (Usb3HsioConfig->Port[Index].HsioFilterSelN << N_HSIO_PCR_RX_DWORD51_FILTER_SEL_N_2_0);
      }

      //
      // LFPS filter select for p (filter_sel_p_2_0)
      // HSIO_RX_DWORD51 [26:24]
      //
      if (Usb3HsioConfig->Port[Index].HsioFilterSelPEnable == TRUE) {
        Data32And &= (UINT32)~B_HSIO_PCR_RX_DWORD51_FILTER_SEL_P_2_0;
        Data32Or  |= (Usb3HsioConfig->Port[Index].HsioFilterSelP << N_HSIO_PCR_RX_DWORD51_FILTER_SEL_P_2_0);
      }

      //
      // Controls the input offset (olfpscfgpullupdwnres_sus_usb_2_0)
      // HSIO_RX_DWORD51 [2:0]
      //
      if (Usb3HsioConfig->Port[Index].HsioOlfpsCfgPullUpDwnResEnable == TRUE) {
        Data32And &= (UINT32)~B_HSIO_PCR_RX_DWORD51_OLFPSCFGPULLUPDWNRES_SUS_USB_2_0;
        Data32Or  |= (UINT32) Usb3HsioConfig->Port[Index].HsioOlfpsCfgPullUpDwnRes;
      }

      if (Usb3HsioConfig->Port[Index].HsioOlfpsCfgPullUpDwnResEnable ||
          Usb3HsioConfig->Port[Index].HsioFilterSelNEnable ||
          Usb3HsioConfig->Port[Index].HsioFilterSelPEnable) {
        HsioLaneAndThenOr32WithSusWrite (
          &HsioLane,
          R_HSIO_PCR_RX_DWORD51,
          Data32And,
          Data32Or
          );
      }

      //
      // Step 2: Clear HSIO_TX_DWORD19 Bit[1] (o_calcinit bit) to 0b
      //
      HsioLaneAndThenOr32WithSusWrite (
        &HsioLane,
        R_HSIO_PCR_TX_DWORD19,
        (UINT32) ~(BIT1),
        (UINT32) (0)
        );

      //
      // Step 3: Set HSIO_TX_DWORD19 Bit[1] (o_calcinit) to 1b
      //
      HsioLaneAndThenOr32WithSusWrite (
        &HsioLane,
        R_HSIO_PCR_TX_DWORD19,
        (UINT32) ~(0),
        (UINT32) (BIT1)
        );
    }
  }
}

/**
  Enables SATA test mode in Modular PHY

  @param[in]  SataCtrlIndex     SATA controlller index
  @param[in]  CtrlPortNum       SATA controlller number of ports

**/
VOID
HsioSataTestModeEnable (
  IN  UINT32    SataCtrlIndex
  )
{
  UINT32                Data32And;
  UINT32                Data32Or;
  HSIO_LANE             HsioLane;
  UINT8                 HsioRxEqBoostMagAd;
  UINT32                Index;
  UINT8                 LaneNum;

  //
  // If Loopback mode, the EB inside the PHY logic is in 'half-empty' mode.
  // Each SATA port/lane in the HSIO space must be written with these values.if Sata Test Mode is enabled
  //    i.)  cri_rxeb_lowater[4:0]   = 5'h0A
  //    ii.) cri_rxeb_ptr_init[4:0]  = 5'h10
  //
  Data32And = (UINT32) ~(B_HSIO_PCR_PCS_DWORD8_CRI_RXEB_PTR_INIT_4_0 | B_HSIO_PCR_PCS_DWORD8_CRI_RXEB_LOWATER_4_0);
  Data32Or  = (UINT32) (0x100A0000);
  for (Index = 0; Index < MaxSataPortNum (SataCtrlIndex); Index++) {
    if (PchFiaGetSataLaneNum (SataCtrlIndex , Index, &LaneNum)) {
      HsioGetLane (LaneNum, &HsioLane);
      HsioLaneAndThenOr32 (&HsioLane, R_HSIO_PCR_PCS_DWORD8, Data32And, Data32Or);
      HsioRxEqBoostMagAd = HsioLaneRead32 (&HsioLane, R_HSIO_PCR_RX_DWORD20) >> N_HSIO_PCR_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0;
      HsioLaneAndThenOr32 (
        &HsioLane,
        R_HSIO_PCR_RX_DWORD20,
        (UINT32) ~B_HSIO_PCR_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0,
        (UINT32) ((HsioRxEqBoostMagAd+3) << N_HSIO_PCR_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0)
        );
    }
  }
}

/**
  The function programs the Pcie Pll SSC registers.

  @param[in]  SiPolicyPpi        The SI Policy PPI instance
**/
VOID
PchPciePllSscProg (
  IN  SI_POLICY_PPI      *SiPolicyPpi
  )
{
  UINT8                       PciePllSsc;
  EFI_STATUS                  Status;
  PCH_PM_CONFIG               *PmConfig;
  UINT8                       PciePllSscTable[21];
  UINT32                      OffsetDword2Register;
  UINT32                      OffsetDword3Register;

  if (IsEbgPch ()) {    // TODO: Handle this at SoC level
    OffsetDword2Register = R_HSIO_VER3_PCR_PLL_SSC_DWORD2;
    OffsetDword3Register = R_HSIO_VER3_PCR_PLL_SSC_DWORD3;
  } else {
    OffsetDword2Register = R_HSIO_PCR_PLL_SSC_DWORD2;
    OffsetDword3Register = R_HSIO_PCR_PLL_SSC_DWORD3;
  }

  PciePllSscTable[0] = 0x0;  // 0%
  PciePllSscTable[1] = 0x6;  // 0.1%
  PciePllSscTable[2] = 0xD;  // 0.2%
  PciePllSscTable[3] = 0x14; // 0.3%
  PciePllSscTable[4] = 0x1A; // 0.4%
  PciePllSscTable[5] = 0x21; // 0.5%
  PciePllSscTable[6] = 0x28; // 0.6%
  PciePllSscTable[7] = 0x2E; // 0.7%
  PciePllSscTable[8] = 0x35; // 0.8%
  PciePllSscTable[9] = 0x3C; // 0.9%
  PciePllSscTable[10] = 0x42; // 1.0%
  PciePllSscTable[11] = 0x49; // 1.1%
  PciePllSscTable[12] = 0x50; // 1.2%
  PciePllSscTable[13] = 0x56; // 1.3%
  PciePllSscTable[14] = 0x5D; // 1.4%
  PciePllSscTable[15] = 0x64; // 1.5%
  PciePllSscTable[16] = 0x6A; // 1.6%
  PciePllSscTable[17] = 0x71; // 1.7%
  PciePllSscTable[18] = 0x78; // 1.8%
  PciePllSscTable[19] = 0x7E; // 1.9%
  PciePllSscTable[20] = 0x85; // 2.0%

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPmConfigGuid, (VOID *) &PmConfig);
  ASSERT_EFI_ERROR (Status);

  if (PmConfig->PciePllSsc == PCIE_PLL_SSC_AUTO) {
    return;
  }

  PciePllSsc = PmConfig->PciePllSsc;
  if (PciePllSsc > MAX_PCIE_PLL_SSC_PERCENT) {
    PciePllSsc = MAX_PCIE_PLL_SSC_PERCENT;
  }

  ///
  /// Step 1:  Clear i_sscen_h to 0b
  ///
  PchPcrAndThenOr32WithSusWrite (
    PID_MODPHY0,
    OffsetDword2Register,
    (UINT32) ~B_HSIO_PCR_PLL_SSC_DWORD2_SSCSEN,
    0
    );

  ///
  /// Step 2:  Write the desired Down Spread % bit values in i_sscstepsize_7_0
  ///
  PchPcrAndThenOr32WithSusWrite (
    PID_MODPHY0,
    OffsetDword2Register,
    (UINT32) ~B_HSIO_PCR_PLL_SSC_DWORD2_SSCSTEPSIZE_7_0,
    (UINT32) (PciePllSscTable[PciePllSsc] << N_HSIO_PCR_PLL_SSC_DWORD2_SSCSTEPSIZE_7_0)
    );

  ///
  /// Step 3:  Set i_ssc_propagate_h to 1b in the LCPLL_CFG1 register
  ///
  PchPcrAndThenOr32WithSusWrite (
    PID_MODPHY0,
    OffsetDword3Register,
    (UINT32) ~B_HSIO_PCR_PLL_SSC_DWORD3_SSC_PROPAGATE,
    (UINT32) B_HSIO_PCR_PLL_SSC_DWORD3_SSC_PROPAGATE
    );

  ///
  /// Step 4:  Set i_sscen_h to 1b
  ///
  PchPcrAndThenOr32WithSusWrite (
    PID_MODPHY0,
    OffsetDword2Register,
    (UINT32) ~B_HSIO_PCR_PLL_SSC_DWORD2_SSCSEN,
    (UINT32) B_HSIO_PCR_PLL_SSC_DWORD2_SSCSEN
    );

  DEBUG ((DEBUG_INFO, "HSIO : PortID = 0x%02x, Offset = 0x%04x, PciePllSsc = 0x%08x\n", PID_MODPHY0, OffsetDword2Register, PchPcrRead32 (PID_MODPHY0, OffsetDword2Register)));
}

/**
  The function programs the Pcie Pll PwrGating registers.
**/
VOID
PchHsioPllPwrGatingProg (
  VOID
  )
{
  ///
  /// Set o_cfg_pwr_gating_ctrl to 1b
  ///
  PchPcrAndThenOr32WithSusWrite (
    PID_MODPHY1,
    R_HSIO_PCR_PLL_DWORD12,
    (UINT32) ~B_HSIO_PCR_PLL_DWORD12_O_CFG_PWR_GATING_CTRL,
    (UINT32) B_HSIO_PCR_PLL_DWORD12_O_CFG_PWR_GATING_CTRL
    );

  if (!IsEbgPch ()) {   // TODO: Handle this at SoC level
    ///
    /// Set DWORD15_O_CFG_CMN_DYNCLKGATE_MODE to 11b
    ///
    PchPcrAndThenOr32WithSusWrite (
      PID_MODPHY1,
      R_HSIO_PCR_PLL_DWORD15,
      (UINT32) ~B_HSIO_PCR_PLL_DWORD15_O_CFG_CMN_DYNCLKGATE_MODE_1_0,
      (UINT32) B_HSIO_PCR_PLL_DWORD15_O_CFG_CMN_DYNCLKGATE_MODE_1_0
      );
  }
}

/**
  The function program HSIO registers.

  @param[in] SiPolicyPpi               The SI Policy PPI instance

**/
VOID
PchHsioConfigure (
  IN  SI_POLICY_PPI    *SiPolicyPpi
  )
{
  EFI_STATUS                         Status;
  PCH_PCIE_CONFIG                    *PchPcieConfig;
  DEBUG ((DEBUG_INFO, "PchHsioConfigure() Start\n"));

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPchPcieConfigGuid, (VOID *) &PchPcieConfig);
  ASSERT_EFI_ERROR (Status);

  if (PchPcieConfig->PcieCommonConfig.ComplianceTestMode) {
    PchHsioEnablePcieComplianceMode ();
  }
  PchPciePllSscProg (SiPolicyPpi);
  PchHsioPllPwrGatingProg ();

  DEBUG ((DEBUG_INFO, "PchHsioConfigure() End\n"));
}