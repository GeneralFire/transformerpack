/** @file
  PCH private PEI PMC Library for PCH FIVR.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation.

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
#include <Library/PchInfoLib.h>
#include <Library/PmcLib.h>
#include <Library/PmcPrivateLib.h>
#include <Register/PmcRegs.h>
#include <Register/PmcRegsFivr.h>
#include <PmcHandle.h>

#define C10_LATENCY  1048

/**
  External FET Ramp Time Configuration

  @param[in] FetRampTime         External Fet Ramp time increment is 31us (ie. 0x4=124us)
  @param[in] IsFetRampTime       External Is Fet Ramp time increment is 10us (ie. 0x4=40us)

**/
VOID
PmcExtFetRampConfig (
  IN UINT8 FetRampTime,
  IN UINT8 IsFetRampTime
  )
{
  UINT32     PchPwrmBase;
  PchPwrmBase = PmcGetPwrmBase ();

  //
  // Configure Fet Ramp Time
  //
  MmioAndThenOr32 (
    PchPwrmBase + R_PMC_PWRM_EXT_FET_RAMP_CONFIG,
    (UINT32) ~B_PMC_PWRM_EXT_FET_RAMP_CONFIG_V105_PHY_FET_RAMPTIME,
    FetRampTime
    );
  //
  // Configure Is Fet Ramp Time
  //
  MmioAndThenOr32 (
    PchPwrmBase + R_PMC_PWRM_EXT_FET_RAMP_CONFIG,
    (UINT32) ~B_PMC_PWRM_EXT_FET_RAMP_CONFIG_V105_IS_FET_RAMPTIME,
    IsFetRampTime << N_PMC_PWRM_EXT_FET_RAMP_CONFIG_V105_IS_FET_RAMPTIME_SHIFT
    );
  //
  // Lock both IS_FET and FET_RAMP
  //
  MmioOr32 (
    PchPwrmBase + R_PMC_PWRM_EXT_FET_RAMP_CONFIG,
    B_PMC_PWRM_EXT_FET_RAMP_CONFIG_V105_PHY_FRT_LOCK | B_PMC_PWRM_EXT_FET_RAMP_CONFIG_V105_PHY_IS_FRT_LOCK
    );
}

/**
  Perform FIVR initialization
  This function does configuration for below settings:
   - external V1p05 and Vnn rails
   - characteristics of VCCIN_AUX voltage rail
   - CPPM interaction with FIVR
  Many of those settings reside in RTC power well.

  @param[in] SiPolicy                  The SI Policy PPI instance
**/
VOID
PmcPchFivrInit (
  IN PMC_FIVR_HANDLE  *PmcFivrHandle
  )
{
  UINT32                                PchPwrmBase;
  UINT32                                Data32And;
  UINT32                                Data32Or;
  UINT32                                Data32;
  PCH_FIVR_CONFIG                       *FivrConfig;
  PCH_PM_CONFIG                         *PmConfig;
  UINT16                                IccMax;
  UINT16                                ExtV1p05RailIccMaximum;
  UINT16                                ExtVnnRailIccMaximum;

  PmConfig = PmcFivrHandle->PmConfig;
  FivrConfig = PmcFivrHandle->FivrConfig;
  PchPwrmBase = PmcFivrHandle->PwrmBase;

  //
  // Enable external bypass for Vnn and V1p05 rails in S0i1+2/S0i3/S3/S4/S5
  //
  Data32Or = (FivrConfig->ExtV1p05Rail.EnabledStates << N_PMC_PWRM_EXT_RAIL_CONFIG_V1P05_S0IX_SX) |
    (FivrConfig->ExtVnnRail.EnabledStates << N_PMC_PWRM_EXT_RAIL_CONFIG_VNN_S0IX_SX) |
    (FivrConfig->ExtV1p05Rail.SupportedVoltageStates << N_PMC_PWRM_EXT_RAIL_CONFIG_V1P05_SUPPORTED) |
    (FivrConfig->ExtVnnRail.SupportedVoltageStates << N_PMC_PWRM_EXT_RAIL_CONFIG_VNN_SUPPORTED);
  Data32And = (UINT32)~(B_PMC_PWRM_EXT_RAIL_CONFIG_V1P05_S0IX_SX | B_PMC_PWRM_EXT_RAIL_CONFIG_VNN_S0IX_SX |
                        B_PMC_PWRM_EXT_RAIL_CONFIG_V1P05_SUPPORTED | B_PMC_PWRM_EXT_RAIL_CONFIG_VNN_SUPPORTED);

  MmioAndThenOr32 (
    PchPwrmBase + R_PMC_PWRM_EXT_RAIL_CONFIG,
    Data32And,
    Data32Or
    );

  //
  // Voltage level and ICC_max for external V1p05 VR rail
  // Register itself holds the value in U2.14 format which
  // is a unsigned fixed point representation of the ICC max in volts.
  // with 2 integer bits and 14 fractional bits.
  // To convert we multiply the IccMax in mV(integer value) by 2^13
  // and divide by 1000 to convert mV to V.
  //
  if (FivrConfig->ExtV1p05Rail.IccMaximum == 0 &&
    FivrConfig->ExtV1p05Rail.IccMax != 0) {
    ExtV1p05RailIccMaximum = (UINT16)FivrConfig->ExtV1p05Rail.IccMax;
  } else {
    ExtV1p05RailIccMaximum = (UINT16)FivrConfig->ExtV1p05Rail.IccMaximum;
  }

  IccMax = (UINT16) ((ExtV1p05RailIccMaximum * (2 << 13))/1000);
  Data32Or = (FivrConfig->ExtV1p05Rail.Voltage << N_PMC_PWRM_EXT_V1P05_VR_CONFIG_VOLTAGE) |
    (IccMax << N_PMC_PWRM_EXT_V1P05_VR_CONFIG_ICC_MAX);
  Data32And = (UINT32)~(B_PMC_PWRM_EXT_V1P05_VR_CONFIG_VOLTAGE | B_PMC_PWRM_EXT_V1P05_VR_CONFIG_ICC_MAX);

  MmioAndThenOr32 (
    PchPwrmBase + R_PMC_PWRM_EXT_V1P05_VR_CONFIG,
    Data32And,
    Data32Or
    );

  //
  // Voltage level and ICC_max for external Vnn VR rail
  //
  if (FivrConfig->ExtVnnRail.IccMaximum == 0 &&
    FivrConfig->ExtVnnRail.IccMax != 0) {
    ExtVnnRailIccMaximum = (UINT16)FivrConfig->ExtVnnRail.IccMax;
  } else {
    ExtVnnRailIccMaximum = (UINT16)FivrConfig->ExtVnnRail.IccMaximum;
  }

  IccMax = (UINT16) ((ExtVnnRailIccMaximum * (2 << 13))/1000);
  Data32Or = (FivrConfig->ExtVnnRail.Voltage << N_PMC_PWRM_EXT_VNN_VR_CONFIG_VOLTAGE) |
    (IccMax << N_PMC_PWRM_EXT_VNN_VR_CONFIG_ICC_MAX);
  Data32And = (UINT32)~(B_PMC_PWRM_EXT_VNN_VR_CONFIG_VOLTAGE | B_PMC_PWRM_EXT_VNN_VR_CONFIG_ICC_MAX);

  MmioAndThenOr32 (
    PchPwrmBase + R_PMC_PWRM_EXT_VNN_VR_CONFIG,
    Data32And,
    Data32Or
    );

  //
  // V1p05 control ramp timer and VNN control ramp timer
  //
  Data32 = (FivrConfig->ExtV1p05Rail.CtrlRampTmr << N_PMC_PWRM_EXT_VNN_V1P05_CTRL_HOLD_OFF_V1P05_CTRL_RAMP_TMR) |
    (FivrConfig->ExtVnnRail.CtrlRampTmr << N_PMC_PWRM_EXT_VNN_V1P05_CTRL_HOLD_OFF_VNN_CTRL_RAMP_TMR);

  MmioWrite32 (PchPwrmBase + R_PMC_PWRM_EXT_VNN_V1P05_CTRL_HOLD_OFF, Data32);

  //
  // Vccin_AUX Configuration 1
  //
  Data32Or = (FivrConfig->VccinAux.LowToHighCurModeVolTranTime << N_PMC_PWRM_VCCIN_AUX_CFG1_LCM_HCM_VOLT_TRANS_TIME) |
    (FivrConfig->VccinAux.RetToHighCurModeVolTranTime << N_PMC_PWRM_VCCIN_AUX_CFG1_RMV_HCM_VOLT_TRANS_TIME) |
    (FivrConfig->VccinAux.RetToLowCurModeVolTranTime << N_PMC_PWRM_VCCIN_AUX_CFG1_RMV_LCM_VOLT_TRANS_TIME);
  Data32And = (UINT32)~(B_PMC_PWRM_VCCIN_AUX_CFG1_LCM_HCM_VOLT_TRANS_TIME |
    B_PMC_PWRM_VCCIN_AUX_CFG1_RMV_HCM_VOLT_TRANS_TIME |
    B_PMC_PWRM_VCCIN_AUX_CFG1_RMV_LCM_VOLT_TRANS_TIME);

  MmioAndThenOr32 (
    PchPwrmBase + R_PMC_PWRM_VCCIN_AUX_CFG1,
    Data32And,
    Data32Or
    );

  //
  // Vccin_AUX Configuration 2
  //
  Data32Or = (FivrConfig->VccinAux.OffToHighCurModeVolTranTime << N_PMC_PWRM_VCCIN_AUX_CFG2_OFF_HCM_VOLT_TRANS_TIME);
  Data32And = (UINT32)~B_PMC_PWRM_VCCIN_AUX_CFG2_OFF_HCM_VOLT_TRANS_TIME;

  MmioAndThenOr32 (
    PchPwrmBase + R_PMC_PWRM_VCCIN_AUX_CFG2,
    Data32And,
    Data32Or
    );

  //
  // PCH RFI Configuration in the FFFC through PMC IPC1 interface
  // is done by CPU RC
  //

  //
  // CPPM FIVR configuration
  //
  Data32Or = B_PMC_PWRM_CPPM_FIVR_POL1A_CPPM_FIVR_VOLT_QUAL |
             B_PMC_PWRM_CPPM_FIVR_POL1A_LTR_FIVR_VOLT_SEL |
             V_PMC_PWRM_CPPM_FIVR_POL1A_LTR_FIVR_VOLT_THRESH;
  Data32And = (UINT32)~B_PMC_PWRM_CPPM_FIVR_POL1A_LTR_FIVR_VOLT_THRESH;
  MmioAndThenOr32 (
    PchPwrmBase + R_PMC_PWRM_CPPM_FIVR_POL1A,
    Data32And,
    Data32Or
    );

  //
  // Vccin_AUX_CFG2 Off to 1.8V ramp time + C10 exit Latency + FIVR fuse download time
  // Note: We divide Data32Or by 10 because calculations are done in 1us
  // but register R_PMC_PWRM_CPPM_FIVR_POL1B expects values in 10us
  // We add 5 to round up the result.
  //
  Data32Or = FivrConfig->VccinAux.OffToHighCurModeVolTranTime + C10_LATENCY + PmcFivrHandle->PmcFivrSocConfig->FuseDownloadDelayUs;
  Data32Or = (Data32Or + 5) / 10;
  Data32And = (UINT32)~B_PMC_PWRM_CPPM_FIVR_POL1B_TNTE_FIVR_VOLT_PRE_WAKE;
  MmioAndThenOr32 (
    PchPwrmBase + R_PMC_PWRM_CPPM_FIVR_POL1B,
    Data32And,
    Data32Or
    );

  Data32Or = 0;
  if (FivrConfig->FivrDynPm) {
    MmioOr32 (PchPwrmBase + R_PMC_PWRM_CFG3, B_PMC_PWRM_CFG3_BIOS_FIVR_DYN_EN);
  } else {
    MmioAndThenOr32 (
      PchPwrmBase + R_PMC_PWRM_CFG3,
      (UINT32) ~B_PMC_PWRM_CFG3_BIOS_FIVR_DYN_EN,
      Data32Or
    );
  }

  if (PmcFivrHandle->PmcFivrSocConfig->OverrideFetRampTime) {
    PmcExtFetRampConfig (PmcFivrHandle->PmcFivrSocConfig->FetRampTime, PmcFivrHandle->PmcFivrSocConfig->IsFetRampTime);
  }
}

/**
  This function configures PCH FIVR RFI Control4 setting

  @param[in] RfiControl4  FIVR RFI Control4 setting value
**/
VOID
PmcConfigureFivrRfiControl4 (
  IN UINT32  RfiControl4
  )
{
  EFI_STATUS                Status;
  PMC_IPC_COMMAND_BUFFER    Wbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));

  Wbuf.Buf0 = V_PMC_PWRM_IPC_SUBCMD_RFI_CTRL4_LOGIC;
  Wbuf.Buf1 = RfiControl4;
  Status = PmcSendCommand (V_PMC_PWRM_IPC_CMD_COMMAND_FIVR, 1, 8, &Wbuf, NULL);
  ASSERT_EFI_ERROR (Status);
}

/**
  This function configures PCH FIVR EMI Control0 setting

  @param[in] EmiControl0  FIVR EMI Control0 setting value
**/
VOID
PmcConfigureFivrEmiControl0 (
  IN UINT32  EmiControl0
  )
{
  EFI_STATUS                Status;
  PMC_IPC_COMMAND_BUFFER    Wbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));

  Wbuf.Buf0 = V_PMC_PWRM_IPC_SUBCMD_EMI_CTRL0_LOGIC;
  Wbuf.Buf1 = EmiControl0;
  Status = PmcSendCommand (V_PMC_PWRM_IPC_CMD_COMMAND_FIVR, 1, 8, &Wbuf, NULL);
  ASSERT_EFI_ERROR (Status);
}
