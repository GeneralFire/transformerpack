/** @file
  Data format for Universal Data Structure

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#ifndef _ME_RC_CONFIG_DATA_H_
#define _ME_RC_CONFIG_DATA_H_

// for ME_RC_CONFIGURATION
extern EFI_GUID gEfiMeRcVariableGuid;

#define ME_RC_CONFIGURATION_NAME  L"MeRcConfiguration"

#define ME_SPS_PSU_NUM_MAX 4

#pragma pack (1)
typedef struct {
#ifdef AMT_SUPPORT
  //
  // ASF
  //
  UINT8   Asf;
  //
  // WatchDog
  //
  UINT8   WatchDog;
  UINT16  WatchDogTimerBios;
  UINT8   WatchDogOs;
  UINT16  WatchDogTimerOs;
  //
  // Intel AMT
  //
  UINT8   Amt;
#endif // AMT_SUPPORT

  //
  // ME Setup entries - start here
  //
#if ME_TESTMENU_FLAG
  UINT8   MeEnableHeciTraces;
  UINT8   SendDidMsg;                  // Whether DRAM Init Done should be sent to ME
  UINT8   DidInitStat;                 // Override DRAM status sent to ME
  UINT8   HsioMessagingEnabled;
  UINT8   EndOfPostMessage;
  UINT8   HeciCommunication;
  UINT8   HeciCommunication2;
  UINT8   HeciCommunication3;
  UINT8   HeciCommunication4;
  UINT8   FiaMuxMessageEnabled;
  UINT8   MeIderEnabled;
  UINT8   KtDeviceEnable;
  UINT8   DisableD0I3SettingForHeci;
  UINT8   HostResetNotification;
  UINT8   DelayedAuthenticationModeEnabled; // Determines if option is enabled
  UINT8   DelayedAuthenticationMode;        // DAM option
  UINT8   DelayedAuthenticationModeOverride;
#endif // ME_TESTMENU_FLAG

  UINT8   CoreBiosDoneEnabled;
  UINT8   MeFwDowngrade;
  UINT8   LocalFwUpdEnabled;

  UINT8   MePttEnabled;
  UINT8   MePttSupported;
  UINT8   MePttSuppressCommandSend;
#if ME_SPS_SUPPORT
  UINT16  SpsAltitude;
  UINT16  SpsMctpBusOwner;

  UINT8   SpsPmbusProxySupported;
  UINT8   SpsPsuAddress[ME_SPS_PSU_NUM_MAX];

  UINT8   PreDidMeResetEnabled;

  UINT8   MeHeci1HideInMe;
  UINT8   MeHeci2HideInMe;
  UINT8   MeHeci3HideInMe;
  UINT8   MeHeci4HideInMe;

  UINT8   NmPwrOptBoot;
  UINT8   NmCores2Disable;
#if ME_TESTMENU_FLAG
  UINT8   MeTimeout;

  UINT8   MeHmrfpoLockEnabled;
  UINT8   MeHmrfpoEnableEnabled;
  UINT8   MeHmrfpoViaHeci3;
  UINT32  MeHmrfpoEnableRegion;

  UINT8   MeGrLockEnabled;
  UINT8   MeGrPromotionEnabled;
  UINT8   BreakRtcEnabled;

  UINT8   SpsIccClkOverride;           // Enable overriding default ICC Clock configuration SPS
  UINT8   SpsIccClkSscSetting;

  UINT8   NmPwrOptBootOverride;
  UINT8   NmCores2DisableOverride;

  UINT8   NmPowerMsmtOverride;
  UINT8   NmPowerMsmtSupport;
  UINT8   NmHwChangeOverride;
  UINT8   NmHwChangeStatus;
  UINT8   NmPtuLoadOverride;           // For MROM-less systems only
  UINT8   MeWatchdogControlEnabled;    // Enable DualBios Watchdog Control msg sending to ME
  UINT8   MeWatchdogDisable;           // Enable DualBios Watchdog disabling
  UINT8   MeRegionSelectEnabled;       // Enable BiosUpdate RegionSelect msg sending to ME
  UINT8   MeDualBIOSSupport;
#endif // ME_TESTMENU_FLAG

#endif // ME_SPS_SUPPORT

  UINT8     MeType;
  UINT8     MeFirmwareMode;
  UINT8     MeImageType;
#if defined (AMT_SUPPORT) && AMT_SUPPORT
  UINT8     MngState;
  UINT8     MdesCapability;
  UINT8     MeIsCorporateWs;
  UINT8     FWProgress;
  UINT8     RemoteSessionActive;
  UINT8     AmtCiraRequest;
  UINT8     AmtCiraTimeout;
  UINT8     UnConfigureMe;
  UINT8     HideUnConfigureMeConfirm;
  UINT8     MebxDebugMsg;
  UINT8     USBProvision;
  UINT8     AmtbxSelectionScreen;
  UINT8     AmtbxHotKeyPressed;
  UINT8     MebxNonUiTextMode;
  UINT8     MebxUiTextMode;
  UINT8     MebxGraphicsMode;
  UINT8     MeStateControl;
#endif // AMT_SUPPORT

#ifdef ME_TESTMENU_FLAG
  UINT8   BiosReflash;
  UINT8   BiosSetup;
  UINT8   BiosPause;
  UINT8   SecureBootExposureToFw;
#endif // ME_TESTMENU_FLAG
  UINT8   MeFirmwareInfo;
  //
  // ME Setup entries - end here <><><><><>
  //

#if defined (WS_ICC_SUPPORT) && WS_ICC_SUPPORT
  UINT8   IccClkOverride;      // Enable overriding default ICC Clock configuration
                               // for AMT
  UINT8   IccProfile;
  UINT8   ShowProfile;
  UINT32  BClkFrequency;
  UINT32  BClkOverride;
  UINT8   BClkChangeWarmReset;
  UINT8   BClkChangeContinues;
#ifndef MAX_UI_CLOCKS
#define MAX_UI_CLOCKS 2
#endif // MAX_UI_CLOCKS
  UINT16 Frequency[MAX_UI_CLOCKS];
  UINT8  Spread[MAX_UI_CLOCKS];
  UINT8  ShowClock[MAX_UI_CLOCKS];
  UINT8  ShowFrequency[MAX_UI_CLOCKS];
  UINT8  ShowSpread[MAX_UI_CLOCKS];
  UINT8  RegLock;
  UINT8  AllowAdvancedOptions;
  UINT8  PlatformType;
#endif // WS_ICC_SUPPORT

  UINT8 ChangeEps;
  UINT8 TpmDeactivate;
  UINT8 DisableCpuReplacedPolling;
  UINT8 DisableMessageCheck;
  UINT8 SkipMbpHob;
  UINT8 MctpBroadcastCycle;
  UINT8 FipsModeSelect;
  UINT8 AutoArbSvnCommit;
  UINT8 InvokeArbSvnCommit;
  UINT8 AfterEoP;
  UINT8 MeasureBoot;
  UINT8 BootGuardSupport;
#ifdef AMT_SUPPORT
  UINT8 AsfSensorsTable;
#endif // AMT_SUPPORT
  UINT8 SolProvisioningState;
#ifdef ME_TESTMENU_FLAG
  UINT8 vProTbtDock;
#endif // ME_TESTMENU_FLAG

  UINT8 ExtendMeMeasuredBoot;
} ME_RC_CONFIGURATION;
#pragma pack ()

#endif // __ME_RC_CONFIG_DATA_H__


