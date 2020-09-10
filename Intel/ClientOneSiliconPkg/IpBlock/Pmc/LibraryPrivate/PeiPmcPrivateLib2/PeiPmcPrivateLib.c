/** @file
  PCH private PEI PMC Library for all PCH generations.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

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

@par Specification Reference:
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PmcLib.h>
#include <Library/PmcPrivateLib.h>
#include <Register/PmcRegs.h>
#include <Register/PmcRegsFivr.h>
#include <PmcHandle.h>
#include <PchPolicyCommon.h> // To import FORCE_DISABLE/FORCE_ENABLE definitions

/**
  This function locks FIVR Configuration
**/
VOID
PmcLockFivrConfig (
  VOID
  )
{
  //
  // GEN_PMCON_B[17] set after configuring FIVR external VR register
  //
  MmioOr32 (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_B, B_PMC_PWRM_GEN_PMCON_B_FIVR_LOCK);
}

/**
  Configure PM Timer.
  Once PM Timer is disabled, TCO timer stops counting.
  This must be executed after uCode PM timer emulation.

  @param[in] SiPolicy                  The SI Policy PPI instance
**/
VOID
PmcConfigurePmTimer (
  IN PMC_HANDLE  *PmcHandle
  )
{
  if (PmcHandle->PmConfig->EnableTcoTimer) {
    return;
  }

  DEBUG ((DEBUG_INFO, "PmcConfigurePmTimer - Disable ACPI timer.\n"));
  //
  // For CNL: Disable ACPI timer (set PwrmBase + 0x18FC[1] = 1), and the TCO timer will also stop.
  //
  MmioWrite32 (PmcHandle->PwrmBase + R_PMC_PWRM_ACPI_TMR_CTL, B_PMC_PWRM_ACPI_TMR_CTL_DIS);
}

/**
  Lock down PMC settings

  @param[in] SiPolicy   The SI Policy PPI instance
**/
VOID
PmcLockSettings (
  IN PMC_HANDLE  *PmcHandle
  )
{
  ///
  /// Set PWRMBASE Offset 0x1048 [24]
  ///
  MmioOr32 (PmcHandle->PwrmBase + R_PMC_PWRM_ETR3, BIT24);

  ///
  /// PM_SYNC_LOCK
  /// Set PWRMBASE Offset 0x18C8 [15]
  ///
  MmioOr32 (PmcHandle->PwrmBase + R_PMC_PWRM_PMSYNC_MISC_CFG, B_PMC_PWRM_PMSYNC_PM_SYNC_LOCK);

  //
  // Lock S0IX_LDO_OFF_COUNT register
  //
  MmioOr32 (PmcHandle->PwrmBase + R_PMC_PWRM_S0IX_LDO_OFF_COUNT, B_PMC_PWRM_S0IX_LDO_OFF_COUNT_LOCK);

  //
  // Lock S0_LDO_OFF_COUNT register
  //
  MmioOr32 (PmcHandle->PwrmBase + R_PMC_PWRM_S0_LDO_OFF_COUNT, B_PMC_PWRM_S0_LDO_OFF_COUNT_LOCK);
}

/**
  Configure deep Sx settings

  @param[in] PchPwrmBase  PMC MMIO
  @param[in] PmConfig     Pointer to PM config

  @retval EFI_SUCCESS                   The function completed successfully
**/
STATIC
VOID
ConfigureDeepSxSettings (
  IN UINT32         PchPwrmBase,
  IN PCH_PM_CONFIG  *PmConfig
  )
{
  UINT32                                S3Data32;
  UINT32                                S4Data32;
  UINT32                                S5Data32;

  ///
  /// PCH BIOS Spec Section 19.11  Deep Sx Power Policies
  /// The System BIOS can perform the following register programming guidelines to enable system
  /// enter Deep S3, Deep S4 or Deep S5.
  ///
  ///                            DPS3_EN_DC         DPS3_EN_AC         DPS4_EN_DC         DPS4_EN_AC         DPS5_EN_DC          DPS5_EN_AC
  ///                         S3_PWRGATE_POL[1]  S3_PWRGATE_POL[0]  S4_PWRGATE_POL[1]  S4_PWRGATE_POL[0]  S5_PWRGATE_POL[15]  S5_PWRGATE_POL[14]
  /// Deep Sx disabled                 0                0                  0                  0                  0                   0
  ///
  /// Enabled in S5                    0                0                  0                  0                  1                   1
  ///
  /// Enabled in S4 and S5             0                0                  1                  1                  1                   1
  ///
  /// Enabled in S3, S4 and S5         1                1                  1                  1                  1                   1
  ///
  /// Configuration supported by MOBILE:
  /// Enabled in S5                    0                0                  0                  0                  1                   0
  /// (Battery mode)
  /// Enabled in S4 and S5             0                0                  1                  0                  1                   0
  /// (Battery Mode)
  /// Enabled in S3, S4 and S5         1                0                  1                  0                  1                   0
  /// (Battery Mode)
  ///
  /// NOTE: Mobile platforms support Deep S3/S4/S5 in DC ONLY,
  /// Desktop and Intel C206 Chipset (LPC Dev ID 0x1C56) platforms support Deep S3/S4/S5 in AC ONLY,
  /// Intel C204 Chipset (LPC Dev ID 0x1C54) and Intel C202 Chipset (LPC Dev ID 0x1C52) platforms DO NOT support Deep S4/S5.
  ///
  /// Deep Sx disabled                 0                0                  0                  0                  0                   0
  ///
  switch (PmConfig->PchDeepSxPol) {
    case PchDpS5AlwaysEn:
      ///
      /// Configuration 2: Enabled in S5/AC-DC
      /// DEEP_S3_POL.DPS3_EN_DC = 0; DEEP_S3_POL.DPS3_EN_AC = 0;
      /// DEEP_S4_POL.DPS4_EN_DC = 0; DEEP_S4_POL.DPS4_EN_AC = 0;
      /// DEEP_S5_POL.DPS5_EN_DC = 1; DEEP_S5_POL.DPS5_EN_AC = 1;
      ///
      S3Data32  = 0;
      S4Data32  = 0;
      S5Data32  = (UINT32) (B_PMC_PWRM_S5_PWRGATE_POL_S5AC_GATE_SUS | B_PMC_PWRM_S5_PWRGATE_POL_S5DC_GATE_SUS);
      break;

    case PchDpS4S5AlwaysEn:
      ///
      /// Configuration 4: Enabled only in S4-S5
      /// DEEP_S3_POL.DPS3_EN_DC = 0; DEEP_S3_POL.DPS3_EN_AC = 0;
      /// DEEP_S4_POL.DPS4_EN_DC = 1; DEEP_S4_POL.DPS4_EN_AC = 1;
      /// DEEP_S5_POL.DPS5_EN_DC = 1; DEEP_S5_POL.DPS5_EN_AC = 1;
      ///
      S3Data32  = 0;
      S4Data32  = (UINT32) (B_PMC_PWRM_S4_PWRGATE_POL_S4AC_GATE_SUS | B_PMC_PWRM_S4_PWRGATE_POL_S4DC_GATE_SUS);
      S5Data32  = (UINT32) (B_PMC_PWRM_S5_PWRGATE_POL_S5AC_GATE_SUS | B_PMC_PWRM_S5_PWRGATE_POL_S5DC_GATE_SUS);
      break;

    case PchDpS3S4S5AlwaysEn:
      ///
      /// Configuration 6: Enabled only in S3-S4-S5
      /// DEEP_S3_POL.DPS3_EN_DC = 1; DEEP_S3_POL.DPS3_EN_AC = 1;
      /// DEEP_S4_POL.DPS4_EN_DC = 1; DEEP_S4_POL.DPS4_EN_AC = 1;
      /// DEEP_S5_POL.DPS5_EN_DC = 1; DEEP_S5_POL.DPS5_EN_AC = 1;
      ///
      S3Data32  = (UINT32) (B_PMC_PWRM_S3_PWRGATE_POL_S3AC_GATE_SUS | B_PMC_PWRM_S3_PWRGATE_POL_S3DC_GATE_SUS);
      S4Data32  = (UINT32) (B_PMC_PWRM_S4_PWRGATE_POL_S4AC_GATE_SUS | B_PMC_PWRM_S4_PWRGATE_POL_S4DC_GATE_SUS);
      S5Data32  = (UINT32) (B_PMC_PWRM_S5_PWRGATE_POL_S5AC_GATE_SUS | B_PMC_PWRM_S5_PWRGATE_POL_S5DC_GATE_SUS);
      break;

    case PchDpS5BatteryEn:
      ///
      /// Configuration 1: Enabled in S5/Battery only
      /// DEEP_S3_POL.DPS3_EN_DC = 0; DEEP_S3_POL.DPS3_EN_AC = 0;
      /// DEEP_S4_POL.DPS4_EN_DC = 0; DEEP_S4_POL.DPS4_EN_AC = 0;
      /// DEEP_S5_POL.DPS5_EN_DC = 1; DEEP_S5_POL.DPS5_EN_AC = 0;
      ///
      S3Data32  = 0;
      S4Data32  = 0;
      S5Data32  = (UINT32) (B_PMC_PWRM_S5_PWRGATE_POL_S5DC_GATE_SUS);
      break;

    case PchDpS4S5BatteryEn:
      ///
      /// Configuration 3: Enabled only in S4-S5/Battery Mode
      /// DEEP_S3_POL.DPS3_EN_DC = 0; DEEP_S3_POL.DPS3_EN_AC = 0;
      /// DEEP_S4_POL.DPS4_EN_DC = 1; DEEP_S4_POL.DPS4_EN_AC = 0;
      /// DEEP_S5_POL.DPS5_EN_DC = 1; DEEP_S5_POL.DPS5_EN_AC = 0;
      ///
      S3Data32  = 0;
      S4Data32  = (UINT32) (B_PMC_PWRM_S4_PWRGATE_POL_S4DC_GATE_SUS);
      S5Data32  = (UINT32) (B_PMC_PWRM_S5_PWRGATE_POL_S5DC_GATE_SUS);
      break;

    case PchDpS3S4S5BatteryEn:
      ///
      /// Configuration 5: Enabled only in S4-S5/Battery Mode
      /// DEEP_S3_POL.DPS3_EN_DC = 1; DEEP_S3_POL.DPS3_EN_AC = 0;
      /// DEEP_S4_POL.DPS4_EN_DC = 1; DEEP_S4_POL.DPS4_EN_AC = 0;
      /// DEEP_S5_POL.DPS5_EN_DC = 1; DEEP_S5_POL.DPS5_EN_AC = 0;
      ///
      S3Data32  = (UINT32) (B_PMC_PWRM_S3_PWRGATE_POL_S3DC_GATE_SUS);
      S4Data32  = (UINT32) (B_PMC_PWRM_S4_PWRGATE_POL_S4DC_GATE_SUS);
      S5Data32  = (UINT32) (B_PMC_PWRM_S5_PWRGATE_POL_S5DC_GATE_SUS);
      break;

    case PchDeepSxPolDisable:
    default:
      ///
      /// Configuration 5: DeepSx Disabled
      /// DEEP_S3_POL.DPS3_EN_DC = 0; DEEP_S3_POL.DPS3_EN_AC = 0;
      /// DEEP_S4_POL.DPS4_EN_DC = 0; DEEP_S4_POL.DPS4_EN_AC = 0;
      /// DEEP_S5_POL.DPS5_EN_DC = 0; DEEP_S5_POL.DPS5_EN_AC = 0;
      ///
      S3Data32  = 0;
      S4Data32  = 0;
      S5Data32  = 0;
      break;
  }
  MmioWrite32 ((PchPwrmBase + R_PMC_PWRM_S3_PWRGATE_POL), S3Data32);
  MmioWrite32 ((PchPwrmBase + R_PMC_PWRM_S4_PWRGATE_POL), S4Data32);
  MmioWrite32 ((PchPwrmBase + R_PMC_PWRM_S5_PWRGATE_POL), S5Data32);
}


/**
  Configure PCH to CPU energy report

  @param[in] PwrmBase      PwrmBase
  @param[in] PmConfig      PM config
  @param[in] PmcSocConfig  PMC SoC configuration
**/
STATIC
VOID
ConfigureEnergyReport (
  IN UINT32          PwrmBase,
  IN PCH_PM_CONFIG   *PmConfig,
  IN PMC_SOC_CONFIG  *PmcSocConfig
  )
{
  if (PmConfig->DisableEnergyReport) {
    return;
  }

  //
  // Enable Energy Reporting
  // PM_CFG (0x1818h[2]) = 1b
  // PM_CFG (0x1818h[24]) = 1b  Must perform this setting as a separate write
  //
  MmioOr32 (PwrmBase + R_PMC_PWRM_CFG, B_PMC_PWRM_CFG_ER_EN);
  MmioOr32 (PwrmBase + R_PMC_PWRM_CFG, B_PMC_PWRM_CFG_ER_LOCK);

}

/**
  This function sets IRQ number used for SCI interrupt

  @param[in] PwrmBase  PMC MMIO
  @param[in] Irq       Irq number (9, 10, 11, 20, 21, 22, 23)
**/
VOID
PmcSetSciIrq (
  IN UINT32  PwrmBase,
  IN UINT8   Irq
  )
{
  UINT32  Data32Or;
  //
  // IRQx    Value to set in register
  // IRQ9    000b
  // IRQ10   001b
  // IRQ11   010b
  // IRQ20   100b
  // IRQ21   101b
  // IRQ22   110b
  // IRQ23   111b
  //
  switch (Irq) {
    case 9:  Data32Or = V_PMC_PWRM_ACPI_CNT_SCIS_IRQ9;  break;
    case 10: Data32Or = V_PMC_PWRM_ACPI_CNT_SCIS_IRQ10; break;
    case 11: Data32Or = V_PMC_PWRM_ACPI_CNT_SCIS_IRQ11; break;
    case 20: Data32Or = V_PMC_PWRM_ACPI_CNT_SCIS_IRQ20; break;
    case 21: Data32Or = V_PMC_PWRM_ACPI_CNT_SCIS_IRQ21; break;
    case 22: Data32Or = V_PMC_PWRM_ACPI_CNT_SCIS_IRQ22; break;
    case 23: Data32Or = V_PMC_PWRM_ACPI_CNT_SCIS_IRQ23; break;
    default:
      //
      // Unsupported SCI IRQ
      //
      ASSERT (FALSE);
      return;
  }
  MmioAndThenOr32 (
    PwrmBase + R_PMC_PWRM_ACPI_CNT,
    ~(UINT32)B_PMC_PWRM_ACPI_CNT_SCIS,
    Data32Or
    );
}

/**
  Configures PM_SYNC messaging interface. Configuration involves
  changing PM_SYNC pin to serial message mode, which events should trigger
  PM_SYNC messages with which indications, how long after de-assertion on
  PM_SYNC event should be seen as asserted(hysteresis control) and which
  messages should be sent over PM_SYNC.

  @param[in] PmcHandle  PMC handle
**/
STATIC
VOID
PmcConfigurePmSyncMessaging (
  IN PMC_HANDLE  *PmcHandle
  )
{
  //
  // Enable the PM_SYNC in serial message mode
  //
  MmioOr32 (PmcHandle->PwrmBase + R_PMC_PWRM_BM_CX_CNF, B_PMC_PWRM_BM_CX_CNF_PM_SYNC_MSG_MODE);

  if (PmcHandle->PmcCallback->PmSyncInitMessages) {
    PmcHandle->PmcCallback->PmSyncInitMessages (PmcHandle);
  }
}

/**
  Configures fabric power gating settings

  @param[in] PwrmBase  PMC MMIO
**/
STATIC
VOID
PmcConfigureFabricPowerGating (
  IN UINT32          PwrmBase,
  IN PMC_SOC_CONFIG  *PmcSocConfig
  )
{
  UINT32  Data32Or;

  if (PmcSocConfig->FabricPowerGatingCppmQualificationEnable) {
    Data32Or = BIT31 | BIT30 | (2 << 6) | 2;
  } else if (PmcSocConfig->UsbDbcConnected) {
    Data32Or = 2;
  } else {
    Data32Or = BIT31 | 2;
  }
  MmioAndThenOr32 (
    PwrmBase + R_PMC_PWRM_1E00,
    (UINT32) ~(BIT29 | (0x1F << 6) | 0xF),
    Data32Or
    );

  MmioAndThenOr32 (
    PwrmBase + R_PMC_PWRM_1E04,
    (UINT32) ~((0x1F << 6) | 0xF),
    (BIT29 | (2 << 6) | 2)
    );
}

/**
  Programs the CPPM clock gating policy.

  @param[in] PwrmBase      Power management base.
  @param[in] PmcSocConfig  Pointer to PMC SOC configuration.
**/
VOID
PmcSetCppmCgPolicy (
  IN UINT32          PwrmBase,
  IN PMC_SOC_CONFIG  *PmcSocConfig
  )
{
  UINT32  RegisterOffset;
  UINT32  Data32And;
  UINT32  Data32Or;

  //
  /// CPPM_CG_POL1A (0x1B24h[30])  = 1,    CPPM Shutdown Qualifier Enable for Clock Source Group 1 (CPPM_G1_QUAL)
  // CPPM_CG_POL1A (0x1B24h[8:0]) = 008h, LTR Threshold for Clock Source Group 1 (LTR_G1_THRESH)
  //
  MmioAndThenOr32 (
    PwrmBase + R_PMC_PWRM_CPPM_CG_POL1A,
    (UINT32) ~B_PMC_PWRM_CPPM_CG_POLXA_LTR_GX_THRESH,
    B_PMC_PWRM_CPPM_CG_POLXA_CPPM_GX_QUAL |
    0x008
    );

  //
  // CPPM_CG_POL2A (0x1B40h[30])  = 1,    CPPM Shutdown Qualifier Enable for Clock Source Group 2 (CPPM_G2_QUAL)
  // CPPM_CG_POL2A (0x1B40h[29])  = 1,    ASLT/PLT Selection for Clock Source Group 1
  // CPPM_CG_POL2A (0x1B40h[8:0]) = 008h, LTR Threshold for Clock Source Group 2 (LTR_G2_THRESH)
  //
  if (PmcSocConfig->CppmCgInterfaceVersion == 2) {
    RegisterOffset = R_PMC_PWRM_CPPM_CG_POL2A_V2;
  } else {
    RegisterOffset = R_PMC_PWRM_CPPM_CG_POL2A;
  }
  MmioAndThenOr32 (
    PwrmBase + RegisterOffset,
    (UINT32) ~B_PMC_PWRM_CPPM_CG_POLXA_LTR_GX_THRESH,
    B_PMC_PWRM_CPPM_CG_POLXA_CPPM_GX_QUAL | B_PMC_PWRM_CPPM_CG_POLXA_LT_GX_SEL |
    0x008
    );

  //
  // CPPM_CG_POL3A (0x1BA8[30])  = 1,    CPPM Shutdown Qualifier Enable for Clock Source Group 3 (CPPM_G3_QUAL)
  // CPPM_CG_POL3A (0x1BA8[29])  = 1,    ASLT/PLT Selection for Clock Source Group 1
  // CPPM_CG_POL3A (0x1BA8[8:0]) = 029h, LTR Threshold for Clock Source Group 3 (LTR_G3_THRESH)
  //
  Data32And = (UINT32) ~B_PMC_PWRM_CPPM_CG_POLXA_LTR_GX_THRESH;
  Data32Or = B_PMC_PWRM_CPPM_CG_POLXA_CPPM_GX_QUAL | B_PMC_PWRM_CPPM_CG_POLXA_LT_GX_SEL | 0x029;

  if (PmcSocConfig->CppmCgInterfaceVersion == 2) {
    RegisterOffset = R_PMC_PWRM_CPPM_CG_POL3A_V2;
  } else {
    RegisterOffset = R_PMC_PWRM_CPPM_CG_POL3A;
  }
  MmioAndThenOr32 (
    PwrmBase + RegisterOffset,
    Data32And,
    Data32Or
    );
}

/**
  Configure Low Power Mode S0ix sub-state support

  @param[in] PwrmBase      Power management base.
  @param[in] PmConfig      Pointer to PM config.
  @param[in] PmcSocConfig  Pointer to PMC SOC configuration.
**/
VOID
PmcConfigureLpmS0ixSupport (
  IN UINT32          PwrmBase,
  IN PCH_PM_CONFIG   *PmConfig,
  IN PMC_SOC_CONFIG  *PmcSocConfig
  )
{
  PMC_LPM_S0IX_SUB_STATE_EN  LpmS0ixSubStateEnable;
  UINT32                     RegisterOffset;

  //
  // LPM_EN[0] - LPM0 - s0i2.0
  // LPM_EN[1] - LPM1 - s0i2.1
  // LPM_EN[2] - LPM2 - s0i2.2. Requires control of v1p05-PHY FET using EXT_PWR_GATE# pin
  // LPM_EN[3] - LPM3 - s0i3.0
  // LPM_EN[4] - LPM4 - s0i3.1
  // LPM_EN[5] - LPM5 - s0i3.2
  // LPM_EN[6] - LPM6 - s0i3.3. Requires control of v1p05-PHY FET using EXT_PWR_GATE# pin
  // LPM_EN[7] - LPM7 - s0i3.4. Requires control of v1p05-IS FET using EXT_PWR_GATE2# pin
  //
  LpmS0ixSubStateEnable.Val = PmConfig->LpmS0ixSubStateEnable.Val;

  //
  // Respective S0ix sub-states need to be disabled if
  // related v1p05-PHY and v1p05-IS FETs are not supported by platform
  //
  if (!PmConfig->V1p05PhyExtFetControlEn) {
    LpmS0ixSubStateEnable.Field.S0i2p2En = 0;
    LpmS0ixSubStateEnable.Field.S0i3p3En = 0;
  }
  if (!PmConfig->V1p05IsExtFetControlEn) {
    LpmS0ixSubStateEnable.Field.S0i3p4En = 0;
  }

  if (PmcSocConfig->LpmInterfaceVersion == 2) {
    RegisterOffset = R_PMC_PWRM_LPM_EN_V2;
  } else {
    RegisterOffset = R_PMC_PWRM_LPM_EN;
  }
  MmioWrite32 (PwrmBase + RegisterOffset, LpmS0ixSubStateEnable.Val);

  //
  // LPM_PRI[3:0]   - LPM0 - s0i2.0
  // LPM_PRI[7:4]   - LPM1 - s0i2.1
  // LPM_PRI[11:8]  - LPM2 - s0i2.2
  // LPM_PRI[15:12] - LPM3 - s0i3.0
  // LPM_PRI[19:16] - LPM4 - s0i3.1
  // LPM_PRI[23:20] - LPM5 - s0i3.2
  // LPM_PRI[27:24] - LPM6 - s0i3.3
  // LPM_PRI[31:28] - LPM7 - s0i3.4
  //
  if (PmcSocConfig->LpmInterfaceVersion == 2) {
    RegisterOffset = R_PMC_PWRM_LPM_PRI_V2;
  } else {
    RegisterOffset = R_PMC_PWRM_LPM_PRI;
  }
  MmioWrite32 (PwrmBase + RegisterOffset, 0x01346257);
}

/**
  Programs PMC LPM

  @param[in] PwrmBase      Power management base.
  @param[in] PmConfig      Pointer to PM config.
  @param[in] PmcSocConfig  Pointer to PMC SOC configuration.
**/
VOID
PmcConfigureLpm (
  IN UINT32          PwrmBase,
  IN PCH_PM_CONFIG   *PmConfig,
  IN PMC_SOC_CONFIG  *PmcSocConfig
  )
{
  UINT32  RegisterOffset;

  if (!PmcSocConfig->LpmSupported) {
    return;
  }

  if (PmConfig->S0ixAutoDemotion) {
    if (PmcSocConfig->LpmInterfaceVersion == 2) {
      RegisterOffset = R_PMC_PWRM_LPM_ADEM_HOST_CTRL_V2;
    } else {
      RegisterOffset = R_PMC_PWRM_LPM_ADEM_HOST_CTRL;
    }
    MmioOr32 (
      PwrmBase + RegisterOffset,
      B_PMC_PWRM_LPM_ADEM_HOST_CTRL_EN
      );
  }

  PmcConfigureLpmS0ixSupport (PwrmBase, PmConfig, PmcSocConfig);
}

/**
  Enable OS IDLE Mode

  @param[in] PwrmBase  PMC MMIO
**/
VOID
PmcEnableOsIdleMode (
  IN UINT32  PwrmBase
  )
{
  MmioOr32 (PwrmBase + R_PMC_PWRM_WINIDLE_BIOS_ENABLE, B_PMC_PWRM_WINIDLE_BIOS_ENABLE_WI_ENABLE);
}

/**
  Lock OS IDLE Mode register

  @param[in] PwrmBase  PMC MMIO
**/
VOID
PmcLockOsIdleMode (
  IN UINT32  PwrmBase
  )
{
  MmioOr32 (PwrmBase + R_PMC_PWRM_WINIDLE_BIOS_ENABLE, B_PMC_PWRM_WINIDLE_BIOS_ENABLE_WI_LOCK);
}

/**
  Perform PCH PMC initialization

  @param[in] PmcHandle  Handle to PMC controller
**/
VOID
PmcInit (
  IN PMC_HANDLE  *PmcHandle
  )
{
  UINT32                    PchPwrmBase;
  UINT32                    Data32;
  UINT32                    Data32And;
  UINT32                    Data32Or;
  PCH_PM_CONFIG             *PmConfig;
  PMC_SOC_CONFIG            *PmcSocConfig;
  UINT32                    PmConA;
  UINT32                    PmConB;

  DEBUG ((DEBUG_INFO, "PmcInit start\n"));

  PmcSocConfig = PmcHandle->PmcSocConfig;
  PmConfig = PmcHandle->PmConfig;

  if (PmcSocConfig == NULL || PmConfig == NULL) {
    DEBUG ((DEBUG_ERROR, "PM policy can't be NULL\n"));
    return;
  }

  PchPwrmBase = PmcHandle->PwrmBase;
  //
  // Perform PM recommendation settings
  //

  ///
  /// GEN_PMCON_A[22] = 1, Allow OPI PLL Shutdown in C0 (ALLOW_OPI_PLL_SD_INC0)
  /// GEN_PMCON_A[20] = 1, Allow SPXB Clock Gating in C0 (ALLOW_SPXB_CG_INC0)
  /// GEN_PMCON_A[19] = 1, Allow L1.LOW Entry during C0 (ALLOW_L1LOW_C0)
  /// GEN_PMCON_A[17] = 1, Allow L1.LOW Entry with OPI Voltage ON (ALLOW_L1LOW_OPI_ON)
  /// GEN_PMCON_A[13] = 1, Allow L1.LOW Entry with CPU BCLK REQ Asserted (ALLOW_L1LOW_BCLKREQ_ON)
  ///
  //
  // Do not clear those RW/1C bits by accident.
  //
  MmioAndThenOr32 (
    PchPwrmBase + R_PMC_PWRM_GEN_PMCON_A,
    (UINT32) ~(B_PMC_PWRM_GEN_PMCON_A_GBL_RST_STS |
               B_PMC_PWRM_GEN_PMCON_A_MS4V |
               B_PMC_PWRM_GEN_PMCON_A_SUS_PWR_FLR |
               B_PMC_PWRM_GEN_PMCON_A_PWR_FLR |
               B_PMC_PWRM_GEN_PMCON_A_HOST_RST_STS),
    B_PMC_PWRM_GEN_PMCON_A_ALLOW_OPI_PLL_SD_INC0 |
    B_PMC_PWRM_GEN_PMCON_A_ALLOW_SPXB_CG_INC0 |
    B_PMC_PWRM_GEN_PMCON_A_ALLOW_L1LOW_C0 |
    B_PMC_PWRM_GEN_PMCON_A_ALLOW_L1LOW_OPI_ON |
    B_PMC_PWRM_GEN_PMCON_A_ALLOW_L1LOW_BCLKREQ_ON
    );

  ///
  /// PWRMBASE + BM_CX_CNF[14, 10] to all 1
  ///
  MmioOr32 (
    PchPwrmBase + R_PMC_PWRM_BM_CX_CNF,
    B_PMC_PWRM_BM_CX_CNF_PHOLD_BM_STS_BLOCK |
    B_PMC_PWRM_BM_CX_CNF_BM_STS_ZERO_EN
    );

  PmcConfigurePmSyncMessaging (PmcHandle);

  ///
  /// PWRMBASE + 0x1814 [24,18,16,15,13,11,10,9,7,6,5,3,1,0] = all 1s
  ///
  MmioOr32 (
    PchPwrmBase + R_PMC_PWRM_1814,
    BIT24 | BIT18 | BIT16 | BIT15 | BIT13 | BIT11 | BIT10 | BIT9 | BIT7 | BIT6 | BIT5 | BIT3 | BIT1 | BIT0
    );

  if (PmcSocConfig->DisableIosfSbClockGating) {
    MmioAnd32 (
      PchPwrmBase + R_PMC_PWRM_1814,
      (UINT32)~BIT7
      );
  }

  ///
  /// PWRMBASE + PM_CFG2[31:29]
  /// For PCH power button override period, 0=4sec, 011b=10sec
  ///
  Data32Or = (PmConfig->PwrBtnOverridePeriod) << N_PMC_PWRM_CFG2_PBOP;
  ///
  /// PWRMBASE + PM_CFG2[28] for power button native mode disable
  ///
  if (PmConfig->DisableNativePowerButton) {
    Data32Or |= B_PMC_PWRM_CFG2_PB_DIS;
  }
  //
  // Set debounce timer for pin that supports PWRBTN
  // Do this even if PWRBTN is not used in case debounce feature
  // of this pin is to be used for a different purposes
  //
  if (PmConfig->PowerButtonDebounce > 0 && PmcHandle->PmcCallback->PmcConfigurePowerButtonDebounce) {
    PmcHandle->PmcCallback->PmcConfigurePowerButtonDebounce (PmcHandle);
  }
  if (PmConfig->PmcDbgMsgEn) {
    Data32Or |= B_PMC_PWRM_CFG2_EN_DBG_MSG;
  }
  Data32And = (UINT32)~(B_PMC_PWRM_CFG2_PBOP | B_PMC_PWRM_CFG2_PB_DIS | B_PMC_PWRM_CFG2_EN_DBG_MSG);
  MmioAndThenOr32 (PchPwrmBase + R_PMC_PWRM_CFG2, Data32And, Data32Or);


  ///
  /// If eSPI boot, PWRMBASE + 0x18E0 = 0x01040009
  /// If SPI boot, PWRMBASE + 0x18E0 = 0x01040001
  /// If CPU_C10_GATE# pin is used, PWRMBASE + 0x18E0[2] = 1
  /// For wake on WLAN from S3/S4/S5, set PWRM_CFG3 [BIT17], HOST_WLAN_PP_EN = 1
  /// For wake on WLAN from DeepSx S3/S4/S5, set PWRM_CFG3 [BIT16], DSX_WLAN_PP_EN = 1
  ///
  if (PmcSocConfig->EspiBoot) {
    Data32Or = 0x01040009;
  } else {
    Data32Or = 0x01040001;
  }

  if (PmConfig->CpuC10GatePinEnable) {
    Data32Or |= B_PMC_PWRM_CFG3_HOST_MISC_CORE_CFG_CPU_VCC_MAP;
  }

  if (PmConfig->WakeConfig.WoWlanEnable == TRUE) {
    Data32Or |= B_PMC_PWRM_CFG3_HOST_WLAN_PP_EN;
    if (PmConfig->WakeConfig.WoWlanDeepSxEnable == TRUE) {
      Data32Or |= B_PMC_PWRM_CFG3_DSX_WLAN_PP_EN;
    }
  }

  Data32And = (UINT32) B_PMC_PWRM_CFG3_BIOS_FIVR_DYN_EN;

  MmioAndThenOr32(
    PchPwrmBase + R_PMC_PWRM_CFG3,
    Data32And,
    Data32Or
  );

  if (PmConfig->CpuC10GatePinEnable && PmcHandle->PmcCallback->PmcConfigureCpuC10Gate) {
    PmcHandle->PmcCallback->PmcConfigureCpuC10Gate (PmcHandle);
  }

  //
  // Enable USB2/TS LDO dynamic shutdown and enable CNVi LDO low power mode for all SKUs
  //
  MmioOr32 (PchPwrmBase + R_PMC_PWRM_PMLDOCTRL, B_PMC_PWRM_PMLDOCTRL_CNVIP24LDOLPEN | B_PMC_PWRM_PMLDOCTRL_USB2TS1P3LDODSEN);

  if (PmcHandle->PmcCallback->PmcConfigureModPhySusPg) {
    PmcHandle->PmcCallback->PmcConfigureModPhySusPg (PmcHandle);
  }

  ///
  /// For PCH-LP and USB2 PHY SUS Well Power Gating enabled:
  ///    PM_CFG4 (0x18E8h[30])  = 1, USB2 PHY SUS Well Power Gating Enable (U2_PHY_PG_EN)
  ///
  /// For PCH-H and PS_ON Enabled:
  ///   Note: PS_ON allows support for a new C10 state from the CPU on desktop SKUs that
  ///         enables a lower power target that will be required by the California Energy
  ///         Commission (CEC).
  /// PM_CFG4 (0x18E8h[31])  = 1,    CEC_EN
  /// PM_CFG4 (0x18E8h[29:23]) = 1Fh (15.5 ms), SLPS0_PSON_TMR
  /// PM_CFG4 (0x18E8h[22:18]) = 1Fh (620 ms), PSON_SLPS0_TMR
  ///

  //
  // Set the CPU IOVR ramp duration according to SoC recommendation. The register
  // is in the unit of 10us.
  //
  Data32And = (UINT32) ~B_PMC_PWRM_CFG4_CPU_IOVR_RAMP_DUR;
  Data32Or  = (PmcSocConfig->CpuIovrRampTime << N_PMC_PWRM_CFG4_CPU_IOVR_RAMP_DUR);

  ///
  /// USB2 PHY SUS Well Power Gating must be done after USB2 PHY programming
  ///
  if (PmConfig->Usb2PhySusPgEnable) {
    Data32Or |= B_PMC_PWRM_CFG4_U2_PHY_PG_EN;
  }

  if (PmcSocConfig->PsOnSupported && PmConfig->PsOnEnable) {
    Data32And &= (UINT32) ~(B_PMC_PWRM_CFG4_SLPS0_PSON_TMR | B_PMC_PWRM_CFG4_PSON_SLPS0_TMR);
    Data32Or  |= (V_PMC_PWRM_CFG4_SLPS0_PSON_TMR_15_5MS << N_PMC_PWRM_CFG4_SLPS0_PSON_TMR) |
                 (V_PMC_PWRM_CFG4_PSON_SLPS0_TMR_620MS << N_PMC_PWRM_CFG4_PSON_SLPS0_TMR);
  }

  MmioAndThenOr32 (
    PchPwrmBase + R_PMC_PWRM_CFG4,
    Data32And,
    Data32Or
    );

  PmcSetCppmCgPolicy (PchPwrmBase, PmcSocConfig);

  ///
  /// PWRMBASE + 0x1B14 = 0x1E0A4616
  ///
  MmioWrite32 (PchPwrmBase + R_PMC_PWRM_OBFF_CFG, 0x1E0A4616);

  if (PmcHandle->PmcCallback->PmcConfigureSlpS0) {
    PmcHandle->PmcCallback->PmcConfigureSlpS0 (PmcHandle);
  }

  ///
  /// CS_SD_CTL1 (0x1BE8[22:20]) = 010, Clock Source 5 Control Configuration (CS5_CTL_CFG)
  ///
  MmioAndThenOr32 (
    PchPwrmBase + R_PMC_PWRM_CS_SD_CTL1,
    (UINT32) ~(B_PMC_PWRM_CS_SD_CTL1_CS5_CTL_CFG | B_PMC_PWRM_CS_SD_CTL1_CS1_CTL_CFG),
    (2 << N_PMC_PWRM_CS_SD_CTL1_CS5_CTL_CFG)
    );

  PmcConfigureLpm (PchPwrmBase, PmConfig, PmcSocConfig);

  PmcConfigureFabricPowerGating (PchPwrmBase, PmcSocConfig);

  if (PmcHandle->PmcCallback->PmcConfigureCppmForceAlignment) {
    PmcHandle->PmcCallback->PmcConfigureCppmForceAlignment (PmcHandle);
  }

  if (PmcSocConfig->SkipModPhyGatingPolicy) {
    ///
    /// CPPM_MPG_POL1A (0x10E0h[30])  = 1,    CPPM Shutdown Qualifier Enable for ModPHY (CPPM_MODPHY_QUAL)
    /// CPPM_MPG_POL1A (0x10E0h[29])  = 1,    ASLT/PLT Selection for ModPHY (LT_MODPHY_SEL)
    /// CPPM_MPG_POL1A (0x10E0h[8:0]) = 069h, LTR Treshold for ModPHY (LTR_MODPHY_THRESH)
    ///
    MmioAndThenOr32 (
      PchPwrmBase + R_PMC_PWRM_CPPM_MPG_POL1A,
      (UINT32) ~B_PMC_PWRM_CPPM_MPG_POL1A_LTR_MODPHY_THRESH,
      B_PMC_PWRM_CPPM_MPG_POL1A_CPPM_MODPHY_QUAL |
      B_PMC_PWRM_CPPM_MPG_POL1A_LT_MODPHY_SEL |
      0x69
      );
  }

  if (PmConfig->LatchEventsC10Exit) {
    MmioOr32 (PchPwrmBase + R_PMC_PWRM_ETR3, B_PMC_PWRM_ETR3_LATCH_EVENTS_C10_EXIT);
  } else {
    MmioAnd32 (PchPwrmBase + R_PMC_PWRM_ETR3, (UINT32)~B_PMC_PWRM_ETR3_LATCH_EVENTS_C10_EXIT);
  }

  ///
  /// Handling Status Registers
  /// System BIOS must set 1b to clear the following registers during power-on
  /// and resuming from Sx sleep state.
  /// - PWRMBASE + PRSTS[0] = 1b
  /// - PWRMBASE + PRSTS[4] = 1b
  /// - PWRMBASE + PRSTS[5] = 1b
  ///
  Data32 = B_PMC_PWRM_PRSTS_FIELD_1;
  ///
  /// Clear power / reset status bits on PCH Corporate
  ///
  if (PmConfig->MeWakeSts) {
    Data32 |= B_PMC_PWRM_PRSTS_ME_WAKE_STS;
  }
  if (PmConfig->WolOvrWkSts) {
    Data32 |= B_PMC_PWRM_PRSTS_WOL_OVR_WK_STS;
  }

  MmioOr32 (PchPwrmBase + R_PMC_PWRM_PRSTS, Data32);


  ///
  /// We need to enable LAN_WAKE_PIN_DSX_EN for Wake from both SX and DSX
  ///
  Data32 = MmioRead32 (PchPwrmBase + R_PMC_PWRM_DSX_CFG);
  if (PmConfig->WakeConfig.LanWakeFromDeepSx == TRUE) {
    Data32 |= B_PMC_PWRM_DSX_CFG_LAN_WAKE_EN;
  } else {
    Data32 &= ~B_PMC_PWRM_DSX_CFG_LAN_WAKE_EN;
  }
  //
  // Disable PCH internal AC PRESENT pulldown
  //
  if (PmConfig->DisableDsxAcPresentPulldown) {
    Data32 |= B_PMC_PWRM_DSX_CFG_ACPRES_PD_DSX_DIS;
  } else {
    Data32 &= ~B_PMC_PWRM_DSX_CFG_ACPRES_PD_DSX_DIS;
  }
  ///
  /// Enable WAKE_PIN__DSX_EN for Wake
  ///
  if (PmConfig->WakeConfig.PcieWakeFromDeepSx) {
    Data32 |= B_PMC_PWRM_DSX_CFG_WAKE_PIN_DSX_EN;
  } else {
    Data32 &= ~B_PMC_PWRM_DSX_CFG_WAKE_PIN_DSX_EN;
  }
  MmioWrite32 (PchPwrmBase + R_PMC_PWRM_DSX_CFG, Data32);

  ///
  /// Handle wake policy
  ///
  PmConA = MmioRead32 (PchPwrmBase + R_PMC_PWRM_GEN_PMCON_A);
  PmConB = MmioRead32 (PchPwrmBase + R_PMC_PWRM_GEN_PMCON_B);

  //
  // Don't clear B_PMC_PWRM_GEN_PMCON_A_GBL_RST_STS, B_PMC_PWRM_GEN_PMCON_A_MS4V, B_PMC_PWRM_GEN_PMCON_A_SUS_PWR_FLR
  // B_PMC_PWRM_GEN_PMCON_A_PWR_FLR, B_PMC_PWRM_GEN_PMCON_A_HOST_RST_STS by accident since those are RW/1C.
  //
  PmConA &= (UINT32) ~(B_PMC_PWRM_GEN_PMCON_A_GBL_RST_STS | B_PMC_PWRM_GEN_PMCON_A_MS4V | B_PMC_PWRM_GEN_PMCON_A_SUS_PWR_FLR | B_PMC_PWRM_GEN_PMCON_A_PWR_FLR | B_PMC_PWRM_GEN_PMCON_A_HOST_RST_STS);

  PmConA &= (UINT32) ~(B_PMC_PWRM_GEN_PMCON_A_PME_B0_S5_DIS);
  PmConB &= (UINT32) ~(B_PMC_PWRM_GEN_PMCON_B_WOL_EN_OVRD);

  if (PmConfig->WakeConfig.PmeB0S5Dis) {
    PmConA |= B_PMC_PWRM_GEN_PMCON_A_PME_B0_S5_DIS;
  }

  if (PmConfig->WakeConfig.WolEnableOverride) {
    ///
    /// Wake-On-LAN (WOL) Implementation
    /// Step 1
    /// Clear PWRMBASE + GEN_PMCON_A[30] = 0b to ensure the LAN PHY will be powered for WOL
    /// when the power source is either the AC or the DC battery.
    ///
    PmConA &= (UINT32) ~B_PMC_PWRM_GEN_PMCON_A_DC_PP_DIS;

    ///
    /// Step 2
    /// Clear PWRMBASE + GEN_PMCON_A[29] = 0b to ensure the LAN PHY will be powered for WOL in Deep Sx.
    ///
    PmConA &= (UINT32) ~B_PMC_PWRM_GEN_PMCON_A_DSX_PP_DIS;

    ///
    /// Step 3
    /// Set PWRMBASE + GEN_PMCON_A[28] = 1b to ensure the LAN PHY will be powered for WOL after a G3 transition.
    ///
    PmConA |= (UINT32) B_PMC_PWRM_GEN_PMCON_A_AG3_PP_EN;

    ///
    /// Step 4
    /// Set PWRMBASE + GEN_PMCON_A[27] = 1b to ensure the LAN PHY will be powered for WOL from Sx.
    ///
    PmConA |= (UINT32) B_PMC_PWRM_GEN_PMCON_A_SX_PP_EN;

    ///
    /// Step 5
    /// "PME_B0_EN", ABASE + Offset 28h[13], bit must be programmed to enable wakes
    /// from S1-S4 at the Power Management Controller
    /// Done in ASL code(_PRW)
    ///
    ///
    /// Step 6
    /// Set "WOL Enable Override", PWRMBASE + GEN_PMCON_B[13], bit to 1b to guarantee the
    /// LAN-Wakes are enabled at the Power Management Controller, even in surprise
    /// S5 cases such as power loss/return and Power Button Override
    ///
    PmConB |= B_PMC_PWRM_GEN_PMCON_B_WOL_EN_OVRD;

    ///
    /// Step 7
    /// Moreover, system BIOS also require to enables in the LAN device by performing
    /// the WOL configuration requirements in the GbE region of the SPI flash.
    /// Done in PchSmmSxGoToSleep() SMM handler.
    ///
  } else {
    ///
    /// PWRMBASE + GEN_PMCON_A[30:27] and PWRMBASE + GEN_PMCON_B[13] are all in RTC or DSW well, so BIOS also
    /// needs to program them while WOL setup option is disabled.
    ///
    PmConA &= (UINT32) ~(B_PMC_PWRM_GEN_PMCON_A_AG3_PP_EN | B_PMC_PWRM_GEN_PMCON_A_SX_PP_EN);
    PmConA |= (B_PMC_PWRM_GEN_PMCON_A_DC_PP_DIS | B_PMC_PWRM_GEN_PMCON_A_DSX_PP_DIS);

    PmConB &= (UINT32) ~(B_PMC_PWRM_GEN_PMCON_B_WOL_EN_OVRD);
  }

  ///
  /// Configure On DC PHY Power Disable according to policy SlpLanLowDc.
  /// When this bit is set, SLP_LAN# will be driven low when ACPRESENT is low.
  /// This indicates that LAN PHY should be powered off on battery mode.
  /// This will override the DC_PP_DIS setting by WolEnableOverride.
  ///
  if (PmConfig->SlpLanLowDc) {
    PmConA |= (UINT32) (B_PMC_PWRM_GEN_PMCON_A_DC_PP_DIS);
  } else {
    PmConA &= (UINT32) ~(B_PMC_PWRM_GEN_PMCON_A_DC_PP_DIS);
  }

  ///
  /// Enabling SLP_S3# and SLP_S4# Stretch
  /// PWRMBASE + GEN_PMCON_A[12:10]
  /// PWRMBASE + GEN_PMCON_A[5:3]
  ///
  PmConA &= (UINT32) ~(B_PMC_PWRM_GEN_PMCON_A_SLP_S3_MAW | B_PMC_PWRM_GEN_PMCON_A_SLP_S4_MAW);

  switch (PmConfig->PchSlpS3MinAssert) {
    case PchSlpS360us:
      PmConA |= V_PMC_PWRM_GEN_PMCON_A_SLP_S3_MAW_60US;
      break;

    case PchSlpS31ms:
      PmConA |= V_PMC_PWRM_GEN_PMCON_A_SLP_S3_MAW_1MS;
      break;

    case PchSlpS350ms:
    default:
      PmConA |= V_PMC_PWRM_GEN_PMCON_A_SLP_S3_MAW_50MS;
      break;

    case PchSlpS32s:
      PmConA |= V_PMC_PWRM_GEN_PMCON_A_SLP_S3_MAW_2S;
      break;
  }

  switch (PmConfig->PchSlpS4MinAssert) {
    case PchSlpS4PchTime:
      PmConA &= (UINT32) (~B_PMC_PWRM_GEN_PMCON_A_SLP_S4_ASE);
      break;

    case PchSlpS41s:
      PmConA |= V_PMC_PWRM_GEN_PMCON_A_SLP_S4_MAW_1S | B_PMC_PWRM_GEN_PMCON_A_SLP_S4_ASE;
      break;

    case PchSlpS42s:
      PmConA |= V_PMC_PWRM_GEN_PMCON_A_SLP_S4_MAW_2S | B_PMC_PWRM_GEN_PMCON_A_SLP_S4_ASE;
      break;

    case PchSlpS43s:
      PmConA |= V_PMC_PWRM_GEN_PMCON_A_SLP_S4_MAW_3S | B_PMC_PWRM_GEN_PMCON_A_SLP_S4_ASE;
      break;

    case PchSlpS44s:
    default:
      PmConA |= V_PMC_PWRM_GEN_PMCON_A_SLP_S4_MAW_4S | B_PMC_PWRM_GEN_PMCON_A_SLP_S4_ASE;
      break;
  }

  if (PmConfig->SlpStrchSusUp == FALSE) {
    PmConA |= B_PMC_PWRM_GEN_PMCON_A_DISABLE_SX_STRETCH;
  } else {
    PmConA &= (UINT32)~B_PMC_PWRM_GEN_PMCON_A_DISABLE_SX_STRETCH;
  }
  MmioWrite32 (PchPwrmBase + R_PMC_PWRM_GEN_PMCON_A, PmConA);
  MmioWrite32 (PchPwrmBase + R_PMC_PWRM_GEN_PMCON_B, PmConB);

  ///
  /// Enable OS Idle Mode   : PWRMBASE + 0x1400[0]  = 1b
  /// Lock WinIdle Register : PWRMBASE + 0x1400[31] = 1b
  ///
  ///
  if (PmcSocConfig->OsIdleSupported) {
    if (PmConfig->OsIdleEnable) {
      PmcEnableOsIdleMode (PchPwrmBase);
    }
    PmcLockOsIdleMode (PchPwrmBase);
  }

  ///
  /// Set CF9LOCK (PWRMBASE + ETR3[31]  = 1b)
  /// Done in Intel Management Engine Framework Reference Code
  ///

  ///
  /// Note: PWRMBASE + PM_CFG[19:16] are platform dependent settings (0Fh provides longest assertion),
  /// please consult with your board design engineers for correct values to be programmed to.
  ///
  /// For PWRMBASE + PM_CFG[9:8] Reset Power Cycle Duration could be customized, please refer to EDS
  /// and make sure the setting correct, which never less than the following register.
  /// - GEN_PMCON_3.SLP_S3_MIN_ASST_WDTH
  /// - GEN_PMCON_3.SLP_S4_MIN_ASST_WDTH
  /// - PM_CFG.SLP_A_MIN_ASST_WDTH
  /// - PM_CFG.SLP_LAN_MIN_ASST_WDTH
  ///
  Data32 = MmioRead32 (PchPwrmBase + R_PMC_PWRM_CFG);
  Data32 &= (UINT32) ~(B_PMC_PWRM_CFG_SSMAW_MASK | B_PMC_PWRM_CFG_SAMAW_MASK | B_PMC_PWRM_CFG_RPCD_MASK);
  switch (PmConfig->PchSlpSusMinAssert) {
    case PchSlpSus0ms:
      Data32 |= V_PMC_PWRM_CFG_SSMAW_0S;
      break;

    case PchSlpSus500ms:
      Data32 |= V_PMC_PWRM_CFG_SSMAW_0_5S;
      break;

    case PchSlpSus1s:
      Data32 |= V_PMC_PWRM_CFG_SSMAW_1S;
      break;

    case PchSlpSus4s:
    default:
      Data32 |= V_PMC_PWRM_CFG_SSMAW_4S;
      break;
  }
  switch (PmConfig->PchSlpAMinAssert) {
    case PchSlpA0ms:
      Data32 |= V_PMC_PWRM_CFG_SAMAW_0S;
      break;

    case PchSlpA4s:
      Data32 |= V_PMC_PWRM_CFG_SAMAW_4S;
      break;

    case PchSlpA98ms:
      Data32 |= V_PMC_PWRM_CFG_SAMAW_98ms;
      break;

    case PchSlpA2s:
    default:
      Data32 |= V_PMC_PWRM_CFG_SAMAW_2S;
      break;
  }
  switch (PmConfig->PchPwrCycDur) {
    case 0:  // treat as PCH default
      Data32 |= V_PMC_PWRM_CFG_RPCD_4S;
      break;

    case 1:
      Data32 |= V_PMC_PWRM_CFG_RPCD_1S;
      break;

    case 2:
      Data32 |= V_PMC_PWRM_CFG_RPCD_2S;
      break;

    case 3:
      Data32 |= V_PMC_PWRM_CFG_RPCD_3S;
      break;

    case 4:
      Data32 |= V_PMC_PWRM_CFG_RPCD_4S;
      break;

    default:
      Data32 |= V_PMC_PWRM_CFG_RPCD_4S;
      DEBUG ((DEBUG_ERROR, "Invalid value for PchPwrCycDur. Using 4Sec as the default value.\n"));
      break;
  }

  //
  // Disable Global Reset on uncorrectable error on PMC SRAM interface. PM_CFG[23] = 0 for TGP+
  // Allow 24MHz Crystal Oscillator Shutdown, Please note: below programming must be done after USB2 GLOBAL PORT 2 programming
  // Allow USB2 PHY Core Power Gating
  // PMC reads disabling is moved to PchOnPciEnumCompleteCommon()
  //
  Data32 |= (B_PMC_PWRM_CFG_ALLOW_USB2_CORE_PG | B_PMC_PWRM_CFG_ALLOW_24_OSC_SD);

  //
  // Clear CPU OC Strap
  //
  Data32 &= (UINT32) ~(B_PMC_PWRM_CFG_COCS);

  MmioWrite32 (PchPwrmBase + R_PMC_PWRM_CFG, Data32);


  //
  // Configure VrAlert GPIO pin
  //
  if (PmConfig->VrAlert) {
    MmioOr32 (PchPwrmBase + R_PMC_PWRM_THROT_1, B_PMC_PWRM_THROT_1_VR_ALERT);
    if (PmcHandle->PmcCallback->PmcConfigureVrAlert) {
      PmcHandle->PmcCallback->PmcConfigureVrAlert (PmcHandle);
    }
  } else {
    MmioAnd32 (PchPwrmBase + R_PMC_PWRM_THROT_1, ~(UINT32)B_PMC_PWRM_THROT_1_VR_ALERT);
  }

  //
  // Configure Energy Report
  //
  ConfigureEnergyReport (PchPwrmBase, PmConfig, PmcSocConfig);

  //
  // Deep Sx Enabling
  //
  ConfigureDeepSxSettings (PchPwrmBase, PmConfig);

  //
  // Configure SCI interrupt
  //
  PmcSetSciIrq (PchPwrmBase, PmcSocConfig->SciIrq);
}
