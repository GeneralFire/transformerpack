/** @file
  Power Management policy

@copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2018 Intel Corporation.

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
#ifndef _PM_CONFIG_H_
#define _PM_CONFIG_H_

#define PM_CONFIG_REVISION 2
extern EFI_GUID gPmConfigGuid;

#pragma pack (push,1)

/**
  This structure allows to customize PCH wake up capability from S5 or DeepSx by WOL, LAN, PCIE wake events.
**/
typedef struct {
  /**
    Corresponds to the PME_B0_S5_DIS bit in the General PM Configuration B (GEN_PMCON_B) register.
    When set to 1, this bit blocks wake events from PME_B0_STS in S5, regardless of the state of PME_B0_EN.
    When cleared (default), wake events from PME_B0_STS are allowed in S5 if PME_B0_EN = 1. <b>0: Disable</b>; 1: Enable.
  **/
  UINT32  PmeB0S5Dis         :  1;
  UINT32  WolEnableOverride  :  1;      ///< Corresponds to the "WOL Enable Override" bit in the General PM Configuration B (GEN_PMCON_B) register. 0: Disable; <b>1: Enable</b>.
  UINT32  PcieWakeFromDeepSx :  1;      ///< Determine if enable PCIe to wake from deep Sx. <b>0: Disable</b>; 1: Enable.
  UINT32  WoWlanEnable       :  1;      ///< Determine if WLAN wake from Sx, corresponds to the "HOST_WLAN_PP_EN" bit in the PWRM_CFG3 register. <b>0: Disable</b>; 1: Enable.
  UINT32  WoWlanDeepSxEnable :  1;      ///< Determine if WLAN wake from DeepSx, corresponds to the "DSX_WLAN_PP_EN" bit in the PWRM_CFG3 register. <b>0: Disable</b>; 1: Enable.
  UINT32  LanWakeFromDeepSx  :  1;      ///< Determine if enable LAN to wake from deep Sx. 0: Disable; <b>1: Enable</b>.
  UINT32  RsvdBits0          : 26;
} PCH_WAKE_CONFIG;

/**
  This PCH_GBL2HOST_EN specifes enable bits related to the "Convert Global Resets to Host Resets" (G2H) feature
**/
typedef union {
  struct {
    UINT32  G2H_FEA        :  1;     ///< G2H Feature Enable: 0: Disable; <b>1: Enable</b>.
    UINT32  LTRESET        :  1;     ///< LT RESET G2H Enable: 0: Disable; <b>1: Enable</b>.
    UINT32  PMCGBL         :  1;     ///< PMC FW-Initiated Global Reset G2H Enable: 0: Disable; <b>1: Enable</b>.
    UINT32  CPUTHRM        :  1;     ///< CPU Thermal Trip G2H Enable: 0: Disable; <b>1: Enable</b>.
    UINT32  PCHTHRM        :  1;     ///< PCH Internal Thermal Trip G2H Enable: Disable; <b>1: Enable</b>.
    UINT32  PBO            :  1;     ///< Power Button Override G2H Enable: 0: Disable; <b>1: Enable</b>.
    UINT32  MEPBO          :  1;     ///< ME-Initiated Power Button Override G2H: 0: Disable; <b>1: Enable</b>.
    UINT32  MEWDT          :  1;     ///< ME FW Watchdog Timer G2H Enable: 0: Disable; <b>1: Enable</b>.
    UINT32  MEGBL          :  1;     ///< ME-Initiated Global Reset G2H Enable: Disable; <b>1: Enable</b>.
    UINT32  CTWDT          :  1;     ///< CPU Thermal Watchdog Timer G2H Enable: Disable; <b>1: Enable</b>.
    UINT32  PMCWDT         :  1;     ///< PMC FW Watchdog Timer G2H Enable: Disable; <b>1: Enable</b>.
    UINT32  ME_UERR        :  1;     ///< ME Uncorrectable Error G2H Enable: Disable; <b>1: Enable</b>.
    UINT32  SYSPWR         :  1;     ///< SYS_PWROK Failure G2H Enable: Disable; <b>1: Enable</b>.
    UINT32  OCWDT          :  1;     ///< Over-Clocking WDT G2H Enable: Disable; <b>1: Enable</b>.
    UINT32  PMC_PARERR     :  1;     ///< PMC Parity Error G2H Enable: 0: Disable; <b>1: Enable</b>.
    UINT32  Reserved       :  1;     ///< Reserved
    UINT32  IEPBO          :  1;     ///< IE-Initiated Power Button Override G2H: 0: Disable; <b>1: Enable</b>.
    UINT32  IEWDT          :  1;     ///< IE FW Watchdog Timer G2H Enable: 0: Disable; <b>1: Enable</b>.
    UINT32  IEGBLN         :  1;     ///< IE-Initiated Global Reset G2H Enable: 0: Disable; <b>1: Enable</b>.
    UINT32  IE_UERRN       :  1;     ///< IE Uncorrectable Error G2H Enable: 0: Disable; <b>1: Enable</b>.
    UINT32  ACRU_ERR_2H_EN :  1;     ///< AC RU Error G2H Enable: 0: Disable; <b>1: Enable</b>.
  } Bits;
  UINT32 Value;
} PCH_GBL2HOST_EN;

/**
  This PCH_DWR_CFG specifies Dirty Warm Reset configuration
**/
typedef struct {
  UINT8                   DirtyWarmReset;        ///< DirtyWarmReset enable
  UINT8                   StallDirtyWarmReset;   ///< Stall during DWR
  UINT8                   Dwr_BmcRootPort;
  PCH_GBL2HOST_EN         PchGbl2HostEn;
  UINT8                   Reserved[1];
} PCH_DWR_CFG;
typedef enum {
  PchDeepSxPolDisable,
  PchDpS5BatteryEn,
  PchDpS5AlwaysEn,
  PchDpS4S5BatteryEn,
  PchDpS4S5AlwaysEn,
} PCH_DEEP_SX_CONFIG;

typedef enum {
  PchSlpS360us = 1,
  PchSlpS31ms,
  PchSlpS350ms,
  PchSlpS32s
} PCH_SLP_S3_MIN_ASSERT;

typedef enum {
  PchSlpS4PchTime,     ///< The time defined in PCH EDS Power Sequencing and Reset Signal Timings table
  PchSlpS41s,
  PchSlpS42s,
  PchSlpS43s,
  PchSlpS44s
} PCH_SLP_S4_MIN_ASSERT;

typedef enum {
  PchSlpSus0ms = 1,
  PchSlpSus500ms,
  PchSlpSus1s,
  PchSlpSus4s,
} PCH_SLP_SUS_MIN_ASSERT;

typedef enum {
  PchSlpA0ms = 1,
  PchSlpA4s,
  PchSlpA98ms,
  PchSlpA2s,
} PCH_SLP_A_MIN_ASSERT;

typedef enum {
  S0ixDisQNoChange,
  S0ixDisQDciOob,
  S0ixDisQUsb2Dbc,
  S0ixDisQMax,
} S0IX_DISQ_PROBE_TYPE;

/**
  The PCH_PM_CONFIG block describes expected miscellaneous power management settings.
  The PowerResetStatusClear field would clear the Power/Reset status bits, please
  set the bits if you want PCH Init driver to clear it, if you want to check the
  status later then clear the bits.

  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Added CPPM ForcedAlignmentEnable
**/
typedef struct {
  CONFIG_BLOCK_HEADER     Header;                           ///< Config Block Header

  PCH_WAKE_CONFIG         WakeConfig;                       ///< Specify Wake Policy
  UINT32                  PchDeepSxPol       :  4;          ///< Deep Sx Policy. Refer to PCH_DEEP_SX_CONFIG for each value. Default is <b>PchDeepSxPolDisable</b>.
  UINT32                  PchSlpS3MinAssert  :  4;          ///< SLP_S3 Minimum Assertion Width Policy. Refer to PCH_SLP_S3_MIN_ASSERT for each value. Default is <b>PchSlpS350ms</b>.
  UINT32                  PchSlpS4MinAssert  :  4;          ///< SLP_S4 Minimum Assertion Width Policy. Refer to PCH_SLP_S4_MIN_ASSERT for each value. Default is <b>PchSlpS44s</b>.
  UINT32                  PchSlpSusMinAssert :  4;          ///< SLP_SUS Minimum Assertion Width Policy. Refer to PCH_SLP_SUS_MIN_ASSERT for each value. Default is <b>PchSlpSus4s</b>.
  UINT32                  PchSlpAMinAssert   :  4;          ///< SLP_A Minimum Assertion Width Policy. Refer to PCH_SLP_A_MIN_ASSERT for each value. Default is <b>PchSlpA2s</b>.
  UINT32                  RsvdBits0          : 12;
  /**
    This member describes whether or not the LPC ClockRun feature of PCH should
    be enabled. <b>0: Disable</b>; 1: Enable
  **/
  UINT32                  SlpStrchSusUp        :  1;        ///< <b>0: Disable</b>; 1: Enable SLP_X Stretching After SUS Well Power Up
  /**
    Enable/Disable SLP_LAN# Low on DC Power. 0: Disable; <b>1: Enable</b>.
    Configure On DC PHY Power Diable according to policy SlpLanLowDc.
    When this is enabled, SLP_LAN# will be driven low when ACPRESENT is low.
    This indicates that LAN PHY should be powered off on battery mode.
    This will override the DC_PP_DIS setting by WolEnableOverride.
  **/
  UINT32                  SlpLanLowDc          :  1;
  /**
    PCH power button override period.
    000b-4s, 001b-6s, 010b-8s, 011b-10s, 100b-12s, 101b-14s
    <b>Default is 0: 4s</b>
  **/
  UINT32                  PwrBtnOverridePeriod :  3;
  /**
    <b>(Test)</b>
    Disable/Enable PCH to CPU enery report feature. <b>0: Disable</b>; 1: Enable.
    Enery Report is must have feature. Wihtout Energy Report, the performance report
    by workloads/benchmarks will be unrealistic because PCH's energy is not being accounted
    in power/performance management algorithm.
    If for some reason PCH energy report is too high, which forces CPU to try to reduce
    its power by throttling, then it could try to disable Energy Report to do first debug.
    This might be due to energy scaling factors are not correct or the LPM settings are not
    kicking in.
  **/
  UINT32                  DisableEnergyReport  :  1;
  /**
    When set to Disable, PCH will internal pull down AC_PRESENT in deep SX and during G3 exit.
    When set to Enable, PCH will not pull down AC_PRESENT.
    This setting is ignored when DeepSx is not supported.
    Default is <b>0:Disable</b>
  **/
  UINT32                  DisableDsxAcPresentPulldown  :  1;
  /**
    Power button native mode disable.
    While FALSE, the PMC's power button logic will act upon the input value from the GPIO unit, as normal.
    While TRUE, this will result in the PMC logic constantly seeing the power button as de-asserted.
    <b>Default is FALSE.</b>
  **/
  UINT32                  DisableNativePowerButton     :  1;
  /**
    Indicates whether SLP_S0# is to be asserted when PCH reaches idle state.
    When set to one SLP_S0# will be asserted in idle state.
    When set to zero SLP_S0# will not toggle and is always drivern high.
    0:Disable, <b>1:Enable</b>

    If a platform is using SLP_S0 to lower PCH voltage the below policy must be disabled.
  **/
  UINT32                  SlpS0Enable                  :  1;
  /**
    S0i3 support
    Determine if platform can get to S0i3 state if required conditions are met.
    In "Disable" platform will go to S0i2 only. If "Enable" S0i3 state will be set
    in runtime (ASL) if required conditions are met (HDAudio D3/Disabled, Display OFF, CNVi WiFi/BT D3/Disabled)
    0: Disable; <b>1: Enable;</b>;
  **/
  UINT32                  S0i3Support                  :  1;

  /**
    Decide if SLP_S0# needs to be overriden (de-asserted) when system is in debug mode.
    Enabled will toggle SLP_S0# assertion; Disabled will enable SLP_S0# assertion when debug is enabled.
    </b>0: Disable<b>; 1: Enable
  **/
  UINT32                  SlpS0Override                :  2;
  /**
    SLP_S0# disqualify for debug prode
    used to configure power management setting per debug probe to be disqualified from S0ix.
    Reminder: USB3 DbC supports S0 only. DCI OOB (aka BSSB) uses CCA probe
    'No Change' will keep PMC BWG settings. Or select the desired debug probe type for S0ix Override settings.\nReminder: USB3 DbC supports S0 only. DCI OOB (aka BSSB) uses CCA probe.
    please refer to S0IX_DISQ_PROBE_TYPE
    </b>0: No Change<b>; 1: DCI OOB; 2: USB2 DbC
  **/
  UINT32                  SlpS0DisQForDebug            :  3;
  UINT32                  MeWakeSts                    :  1;     ///< Clear the ME_WAKE_STS bit in the Power and Reset Status (PRSTS) register. 0: Disable; <b>1: Enable</b>.
  UINT32                  WolOvrWkSts                  :  1;     ///< Clear the WOL_OVR_WK_STS bit in the Power and Reset Status (PRSTS) register. 0: Disable; <b>1: Enable</b>.
  /*
    Set true to enable TCO timer.
    When FALSE, it disables PCH ACPI timer, and stops TCO timer.
    @note: This will have significant power impact when it's enabled.
    If TCO timer is disabled, uCode ACPI timer emulation must be enabled,
    and WDAT table must not be exposed to the OS.
    <b>0: Disable</b>, 1: Enable
  */
  UINT32                  EnableTcoTimer               : 1;
  /*
    When VRAlert# feature pin is enabled and its state is '0',
    the PMC requests throttling to a T3 Tstate to the PCH throttling unit.
    <b>0: Disable</b>; 1: Enable.
  */
  UINT32                  VrAlert                      : 1;
  /**
    Decide if PS_ON is to be enabled. This is available on desktop only.
    PS_ON is a new C10 state from the CPU on desktop SKUs that enables a
    lower power target that will be required by the California Energy
    Commission (CEC). When FALSE, PS_ON is to be disabled.}
    </b>0: Disable<b>; 1: Enable.
  **/
  UINT32                  PsOnEnable                   :  1;
  /**
    Enable/Disable platform support for CPU_C10_GATE# pin to control gating
    of CPU VccIO and VccSTG rails instead of SLP_S0# pin. This policy needs
    to be set if board design includes support for CPU_C10_GATE# pin.
    0: Disable; <b>1: Enable</b>
  **/
  UINT32                  CpuC10GatePinEnable          :  1;
  /**
    Control whether to enable PMC debug messages to Trace Hub.
    When Enabled, PMC HW will send debug messages to trace hub;
    When Disabled, PMC HW will never send debug meesages to trace hub.
    @note: When enabled, system may not enter S0ix
    </b>0: Disable<b>; 1: Enable.
  **/
  UINT32                  PmcDbgMsgEn                  :  1;
  /**
    Enable/Disable ModPHY SUS Power Domain Dynamic Gating.
    EXT_PWR_GATE# signal (if supported on platform) can be used to
    control external FET for power gating ModPHY
    @note: This setting is not supported and ignored on PCH-H
    0: Disable; <b>1: Enable</b>.
  **/
  UINT32                  ModPhySusPgEnable            :  1;
  /**
    <b>(Test)</b>
    This policy option enables USB2 PHY SUS Well Power Gating functionality.
    @note: This setting is not supported and ignored on PCH-H
    <b>0: disable USB2 PHY SUS Well Power Gating</b>
    1: enable USB2 PHY SUS Well Power Gating
  **/
  UINT32                  Usb2PhySusPgEnable           :  1;
  /**
    Enable Os Idle Mode.
    @note: This feature is supported on ICP-N and ICP-H only
    0: Disable; <b>1: Enable</b>.
  **/
  UINT32                  OsIdleEnable                 :  1;
  /**
    Enable or Disable CrashLog Dump.
    0: Disable; <b>1: Enable</b>
  **/
  UINT32                  CrashLogEnable               :  1;
  /**
    Enable or Disable CPPM ForcedAlignment.
    Enable: PMC allows stalling of the backbone, blocking the DMI transmit arbiter and
    preventing CPU wakes (via PM_SYNC messages) when PCH idle (FAIDLEWAIT and
    FACSMEPGQDIS) and CPU is in the memory closed state. It ceases the stall when
    the maximum wake time point is met.
    Disable: PMC does not allow stalling of the backbone or blocking the DMI transmit arbiter.
    0: Disable; <b>1: Enable</b>
  **/
  UINT32                  ForcedAlignmentEnable        :  1;
  UINT32                  RsvdBits1                    :  5;
  /*
    Power button debounce configuration
    Debounce time can be specified in microseconds. Only certain values according
    to below formula are supported:
     DebounceTime = (2 ^ PADCFG_DW2.DEBOUNCE)*(glitch filter clock period).
    RTC clock with f = 32 KHz is used for glitch filter.
     DebounceTime = (2 ^ PADCFG_DW2.DEBOUNCE)*(31.25 us).
    Supported DebounceTime values are following:
     DebounceTime = 0 -> Debounce feature disabled
     DebounceTime > 0 && < 250us -> Not supported
     DebounceTime = 250us - 1024000us -> Supported range (DebounceTime = 250us * 2^n)
    For values not supported by HW, they will be rounded down to closest supported one
    <b>Default is 0</b>
  */
  UINT32                  PowerButtonDebounce;
  /**
    Reset Power Cycle Duration could be customized in the unit of second. Please refer to EDS
    for all support settings. PCH HW default is 4 seconds, and range is 1~4 seconds, where
    <b>0 is default</b>, 1 is 1 second, 2 is 2 seconds, ... 4 is 4 seconds.
    And make sure the setting correct, which never less than the following register.
    - GEN_PMCON_B.SLP_S3_MIN_ASST_WDTH
    - GEN_PMCON_B.SLP_S4_MIN_ASST_WDTH
    - PWRM_CFG.SLP_A_MIN_ASST_WDTH
    - PWRM_CFG.SLP_LAN_MIN_ASST_WDTH
  **/
  UINT8                   PchPwrCycDur;
  /**
    Specifies the Pcie Pll Spread Spectrum Percentage
    The value of this policy is in 1/10th percent units.
    Valid spread range is 0-20. A value of 0xFF is reserved for AUTO.
    A value of 0 is SSC of 0.0%. A value of 20 is SSC of 2.0%
    The default is <b>0xFF: AUTO - No BIOS override</b>.
  **/
  UINT8                   PciePllSsc;
  UINT8                   Rsvd0[2];                             ///< Reserved bytes

  PCH_DWR_CFG             Dwr;

} PCH_PM_CONFIG;

#pragma pack (pop)

#endif // _PM_CONFIG_H_
