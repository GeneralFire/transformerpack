/** @file
  CPU Power Management Basic Config Block.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2020 Intel Corporation.

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
#ifndef _CPU_POWER_MGMT_BASIC_CONFIG_H_
#define _CPU_POWER_MGMT_BASIC_CONFIG_H_

#define CPU_POWER_MGMT_BASIC_CONFIG_REVISION 6

extern EFI_GUID gCpuPowerMgmtBasicConfigGuid;

#ifndef TURBO_RATIO_LIMIT_ARRAY_SIZE
#define TURBO_RATIO_LIMIT_ARRAY_SIZE 8
#endif

#pragma pack (push,1)

/**
  CPU Power Management Basic Configuration Structure.

  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Changed EnableItbm default to be disable
  - Deprecated EnableItbmDriver due to Platform doesn't have ITBMT OS driver
  <b>Revision 3</b>:
  - Add ApplyConfigTdp for TDP initialization settings based on non-cTDP or cTDP
  <b>Revision 4</b>:
  - Add Hwp Lock support
  <b>Revision 5</b>:
  - Add TurboRatioLimitRatio, TurboRatioLimitNumCore, AtomTurboRatioLimitRatio and AtomTurboRatioLimitNumCore
  <b>Revision 6</b>:
  - Add DualTauBoost
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  /**
  Sets the boot frequency starting from reset vector.
   - 0: Maximum battery performance.
   - 1: Maximum non-turbo performance.
   - <b>2: Turbo performance.</b>
  @note If Turbo is selected BIOS will start in max non-turbo mode and switch to Turbo mode.
  **/
  UINT32 BootFrequency                  : 2;       //@deprecated
  UINT32 SkipSetBootPState              : 1;      ///< Choose whether to skip SetBootPState function for all APs; <b>0: Do not skip</b>; 1: Skip.
  /**
  Enable or Disable Intel Speed Shift Technology.
  Enabling allows for processor control of P-state transitions.
  0: Disable; <b>1: Enable;</b> Bit 1 is ignored.
  @note Currently this feature is recommended to be enabled only on win10
  **/
  UINT32 Hwp                            : 2;
  /**
  Hardware Duty Cycle Control configuration. 0: Disabled; <b>1: Enabled</b> 2-3:Reserved
  HDC enables the processor to autonomously force components to enter into an idle state to lower effective frequency.
  This allows for increased package level C6 residency.
  @note Currently this feature is recommended to be enabled only on win10
  **/
  UINT32 HdcControl                     : 2;
  UINT32 PowerLimit2                    : 1;      ///< Enable or Disable short duration Power Limit (PL2). 0: Disable; <b>1: Enable</b>
  UINT32 TurboPowerLimitLock            : 1;      ///< MSR 0x610[63] and 0x618[63]: Locks all Turbo power limit settings to read-only; <b>0: Disable</b>; 1: Enable (Lock).
  UINT32 PowerLimit3DutyCycle           : 8;      ///< Package PL3 Duty Cycle. Specifies the PL3 duty cycle percentage, Range 0-100. <b>Default: 0</b>.
  UINT32 PowerLimit3Lock                : 1;      ///< Package PL3 MSR 615h lock; <b>0: Disable</b>; 1: Enable (Lock).
  UINT32 PowerLimit4Lock                : 1;      ///< Package PL4 MSR 601h lock; <b>0: Disable</b>; 1: Enable (Lock).
  /**
  Tcc Offset Clamp for Runtime Average Temperature Limit (RATL) allows CPU to throttle below P1.
  For Y SKU, the recommended default for this policy is <b>1: Enabled</b>, which indicates throttling below P1 is allowed.
  For all other SKUs the recommended default are  <b>0: Disabled</b>.
  **/
  UINT32 TccOffsetClamp                 : 1;
  UINT32 TccOffsetLock                  : 1;      ///< Tcc Offset Lock for Runtime Average Temperature Limit (RATL) to lock temperature target MSR 1A2h; 0: Disabled; <b>1: Enabled (Lock)</b>.
  UINT32 TurboMode                      : 1;      ///< Enable or Disable Turbo Mode. Disable; <b>1: Enable</b>
  UINT32 HwpInterruptControl            : 1;      ///< Set HW P-State Interrupts Enabled  for MISC_PWR_MGMT MSR 0x1AA[7]; <b>0: Disable</b>; 1: Enable.
  UINT32 ApplyConfigTdp                 : 1;      ///< Switch TDP applied setting based on non-cTDP or TDP; 0: non-cTDP; <b>1: cTDP</b>.
  UINT32 HwpLock                        : 1;      ///< HWP Lock in MISC PWR MGMT MSR 1AAh; <b>0: Disable</b>; 1: Enable (Lock).
  /**
  Dual Tau Base Performance Boost support. This will improve platform performance for desktop sku.
  When enabled, it exposes 2 MMIO PL1 and MMIO Tau registers, and also provides the default value.
  @note: This is only applicable for Desktop 35W/65W/125W sku.
  @note: When this is enabled, it will override the settings in policys PowerLimit1, PowerLimit1Time, PowerLimit2Power.
  <b>0: Disable</b>; 1: Enable
  **/
  UINT32 DualTauBoost                   : 1;
  UINT32 RsvdBits                       : 6;      ///< Reserved for future use.

  /**
  1-Core Ratio Limit: LFM to Fused 1-Core Ratio Limit. For overclocking parts: LFM to Fused 1-Core Ratio Limit + OC Bins.
  Default value is from hardware register MSR 0x1AD.
  Note: OC Bins = 7 means fully unlocked, so range is LFM to 83.
   - This 1-Core Ratio Limit Must be greater than or equal to 2-Core Ratio Limit, 3-Core Ratio Limit, 4-Core Ratio Limit.
   - This policy is for platforms only support MSR 0x1AD without MSR 0x1AE support.
  **/
  UINT8  OneCoreRatioLimit;
  /**
  2-Core Ratio Limit: LFM to Fused 2-Core Ratio Limit, For overclocking part: LFM to Fused 2-Core Ratio Limit + OC Bins.
  Default value is from hardware register MSR 0x1AD.
  Note: OC Bins = 7 means fully unlocked, so range is LFM to 83.
   - This 2-Core Ratio Limit Must be Less than or equal to 1-Core Ratio Limit.
   - This policy is for platforms only support MSR 0x1AD without MSR 0x1AE support.
  **/
  UINT8  TwoCoreRatioLimit;
  /**
  3-Core Ratio Limit: LFM to Fused 3-Core Ratio Limit, For overclocking part: LFM to Fused 3-Core Ratio Limit + OC Bins.
  Default value is from hardware register MSR 0x1AD.
  Note: OC Bins = 7 means fully unlocked, so range is LFM to 83.
   - This 3-Core Ratio Limit Must be Less than or equal to 1-Core Ratio Limit.
   - This policy is for platforms only support MSR 0x1AD without MSR 0x1AE support.
  **/
  UINT8  ThreeCoreRatioLimit;
  /**
  4-Core Ratio Limit: LFM to Fused 4-Core Ratio Limit, For overclocking part: LFM to Fused 4-Core Ratio Limit + OC Bins.
  Default value is from hardware register MSR 0x1AD.
  Note: OC Bins = 7 means fully unlocked, so range is LFM to 83.
   - This 4-Core Ratio Limit Must be Less than or equal to 1-Core Ratio Limit.
   - This policy is for platforms only support MSR 0x1AD without MSR 0x1AE support.
  **/
  UINT8  FourCoreRatioLimit;
  /**
   5-Core Ratio Limit: LFM to Fused 5-Core Ratio Limit, For overclocking part: LFM to Fused 5-Core Ratio Limit + OC Bins.
   Note: OC Bins = 7 means fully unlocked, so range is LFM to 83.
     - This 5-Core Ratio Limit Must be Less than or equal to 1-Core Ratio Limit.
     - This policy is for platforms only support MSR 0x1AD without MSR 0x1AE support.
  **/
  UINT8  FiveCoreRatioLimit;
  /**
  6-Core Ratio Limit: LFM to Fused 6-Core Ratio Limit, For overclocking part: LFM to Fused 6-Core Ratio Limit + OC Bins.
  Default value is from hardware register MSR 0x1AD.
  Note: OC Bins = 7 means fully unlocked, so range is LFM to 83.
   - This 6-Core Ratio Limit Must be Less than or equal to 1-Core Ratio Limit.
   - This policy is for platforms only support MSR 0x1AD without MSR 0x1AE support.
  **/
  UINT8  SixCoreRatioLimit;
  /**
  7-Core Ratio Limit: LFM to Fused 7-Core Ratio Limit, For overclocking part: LFM to Fused 7-Core Ratio Limit + OC Bins.
  Default value is from hardware register MSR 0x1AD.
  Note: OC Bins = 7 means fully unlocked, so range is LFM to 83.
   - This 7-Core Ratio Limit Must be Less than or equal to 1-Core Ratio Limit.
   - This policy is for platforms only support MSR 0x1AD without MSR 0x1AE support.
  **/
  UINT8  SevenCoreRatioLimit;
  /**
  8-Core Ratio Limit: LFM to Fused 8-Core Ratio Limit, For overclocking part: LFM to Fused 8-Core Ratio Limit + OC Bins.
  Default value is from hardware register MSR 0x1AD.
  Note: OC Bins = 7 means fully unlocked, so range is LFM to 83.
   - This 8-Core Ratio Limit Must be Less than or equal to 1-Core Ratio Limit.
   - This policy is for platforms only support MSR 0x1AD without MSR 0x1AE support.
  **/
  UINT8  EightCoreRatioLimit;
  /**
  TCC Activation Offset. Offset from factory set TCC activation temperature at which the Thermal Control Circuit must be activated.
  TCC will be activated at (TCC Activation Temperature - TCC Activation Offset), in degrees Celcius.
  For Y SKU, the recommended default for this policy is  <b>10</b>
  For all other SKUs the recommended default are <b>0</b>, causing TCC to activate at TCC Activation temperature.
  @note The policy is recommended for validation purpose only.
  **/
  UINT8  TccActivationOffset;
  /**
  Intel Turbo Boost Max Technology 3.0
  Enabling it on processors with OS support will allow OS to exploit the diversity in max turbo frequency of the cores.
  <b>0: Disable</b>; 1: Enable;
  **/
  UINT8  EnableItbm                     : 1;
  /**
  @deprecated: Platform doesn't have Intel Turbo Boost Max Technology 3.0 Driver
  Enabling it will load the driver upon ACPI device with HID = INT3510.
  <b> 0: Disable;</b> 1: Enable;
  **/
  UINT8  EnableItbmDriver               : 1;
  /**
  Per Core P State OS control mode
  Disabling will set PCU_MISC_CONFIG (Command 0x06) Bit 31 = 1. When set, the highest core request is used for all other core requests.
  0: Disable;<b> 1: Enable;</b>
  **/
  UINT8  EnablePerCorePState            : 1;
  /**
  HwP Autonomous Per Core P State
  Disabling will set Bit 30 = 1, command 0x11. When set, autonomous will request the same value
  for all cores all the time.
  0: Disable;<b> 1: Enable;</b>
  **/
  UINT8  EnableHwpAutoPerCorePstate     : 1;
  /**
  HwP Autonomous EPP grouping.
  Disabling will set Bit 29 = 1, command 0x11. When set, autonomous will not necesarrily request the same value
  for all cores with same EPP.
  Enabling will clean Bit 29 = 0, command 0x11. Autonomous will request same values for all cores with same EPP.
  0: Disable;<b> 1: Enable;</b>
  **/
  UINT8  EnableHwpAutoEppGrouping       : 1;
  /**
  EPB override over PECI
  Enable by sending pcode command 0x2b , subcommand 0x3 to 1.
  This will allow OOB EPB PECI override control.
  <b>0: Disable;</b> 1: Enable;
  **/
  UINT8  EnableEpbPeciOverride          : 1;
  /**
  Support for Fast MSR for IA32_HWP_REQUEST.
  On systems with HwP enabled, if this feature is available as indicated by MSR 0x65F[0] = 1,
  set MSR 0x657[0] = 1.
  0: Disable; <b> 1: Enable;</b>
  **/
  UINT8  EnableFastMsrHwpReq            : 1;
  UINT8  ReservedBits1                  : 1;      ///< Reserved for future use.
  UINT8  MinRingRatioLimit;                       ///< Minimum Ring Ratio Limit. Range from 0 to Max Turbo Ratio. 0 = AUTO/HW Default
  UINT8  MaxRingRatioLimit;                       ///< Maximum Ring Ratio Limit. Range from 0 to Max Turbo Ratio. 0 = AUTO/HW Default
  /**
  Package Long duration turbo mode power limit (PL1).
  Default is the TDP power limit of processor. Units are based on POWER_MGMT_CONFIG.CustomPowerUnit.
  **/
  UINT16 PowerLimit1;
  /**
  Package Short duration turbo mode power limit (PL2). Allows for short excursions above TDP power limit.
  Default = 1.25 * TDP Power Limit. Units are based on POWER_MGMT_CONFIG.CustomPowerUnit.
  **/
  UINT16 PowerLimit2Power;
  /**
  Package PL3 power limit. PL3 is the CPU Peak Power Occurences Limit.
  <b>Default: 0</b>. Range 0-65535. Units are based on POWER_MGMT_CONFIG.CustomPowerUnit.
  **/
  UINT16 PowerLimit3;
  /**
  Package PL4 power limit. PL4 is a Preemptive CPU Package Peak Power Limit, it will never be exceeded.
  Power is premptively lowered before limit is reached. <b>Default: 0</b>. Range 0-65535.
  Units are based on POWER_MGMT_CONFIG.CustomPowerUnit.
  **/
  UINT16 PowerLimit4;
  /**
  Package Long duration turbo mode power limit (PL1) time window in seconds.
  Used in calculating the average power over time.
  Mobile: <b> 28s</b>
  Desktop: <b> 8s</b>
  Range: 0 - 128s
  **/
  UINT32 PowerLimit1Time;
  UINT32 PowerLimit3Time;                         ///< Package PL3 time window. Range from 3ms to 64ms.
  /**
  Tcc Offset Time Window can range from 5ms to 448000ms for Runtime Average Temperature Limit (RATL).
  For Y SKU, the recommended default for this policy is <b>5000: 5 seconds</b>, For all other SKUs the recommended default are <b>0: Disabled</b>
  **/
  UINT32 TccOffsetTimeWindowForRatl;
  /**
  Turbo Ratio Limit Ratio array, TurboRatioLimitRatio[7-0] will pair with TurboRatioLimitNumCore[7-0]
  to determine the active core ranges for each frequency point.
  Default value is from hardware register MSR 0x1AD.
  Note:
    - These policies are for platforms support MSR pair 0x1AD and 0x1AE.
  **/
  UINT8  TurboRatioLimitRatio[TURBO_RATIO_LIMIT_ARRAY_SIZE];
  /**
  Turbo Ratio Limit Num Core array, TurboRatioLimitNumCore[7-0] will pair with TurboRatioLimitRatio[7-0]
  to determine the active core ranges for each frequency point.
  Default value is from hardware register MSR 0x1AE.
  Note:
    - These policies are for platforms support MSR pair 0x1AD and 0x1AE.
  **/
  UINT8  TurboRatioLimitNumCore[TURBO_RATIO_LIMIT_ARRAY_SIZE];
  /**
  ATOM Turbo Ratio Limit Ratio array, AtomTurboRatioLimitRatio[7-0] will pair with AtomTurboRatioLimitNumCore[7-0]
  to determine the active core ranges for each frequency point.
  Default value is from hardware register MSR 0x650.
  Note:
    - These policies are for platforms support ATOM MSR pair 0x650 and 0x651.
  **/
  UINT8  AtomTurboRatioLimitRatio[TURBO_RATIO_LIMIT_ARRAY_SIZE];
  /**
  ATOM Turbo Ratio Limit Num Core array, AtomTurboRatioLimitNumCore[7-0] will pair with AtomTurboRatioLimitRatio[7-0]
  to determine the active core ranges for each frequency point.
  Default value is from hardware register MSR 0x651.
  Note:
    - These policies are for platforms support ATOM MSR pair 0x650 and 0x651.
  **/
  UINT8  AtomTurboRatioLimitNumCore[TURBO_RATIO_LIMIT_ARRAY_SIZE];
} CPU_POWER_MGMT_BASIC_CONFIG;

#pragma pack (pop)

#endif // _CPU_POWER_MGMT_BASIC_CONFIG_H_
