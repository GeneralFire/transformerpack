/** @file
  Data format for Ioh Memory Config Data Structure

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

#ifndef   __SOCKET_MEMORY_CONFIG_DATA_H__
#define   __SOCKET_MEMORY_CONFIG_DATA_H__


#include <UncoreCommonIncludes.h>
#include "SocketConfiguration.h"
#include <Memory/MemDefaults.h>

extern EFI_GUID gEfiSocketMemoryVariableGuid;
#define SOCKET_MEMORY_CONFIGURATION_NAME L"SocketMemoryConfig"

#pragma pack(1)
typedef struct {
  UINT8                   DfxBankGroupInterleavingMode;
  UINT8                   MemoryHotPlugBase;
  UINT8                   MemoryHotPlugLen;
  UINT8                   Srat;
  UINT8                   SratMemoryHotPlug;
  UINT8                   SratCpuHotPlug;
  UINT8                   PagePolicy;
  UINT8                   PatrolScrub;
  UINT8                   PatrolScrubDuration;
  UINT8                   TempRefreshOption;
  UINT8                   HalfxRefreshValue;
  UINT8                   TwoxRefreshValue;
  UINT8                   FourxRefreshValue;
  UINT8                   NsddcEn;
  UINT8                   EsddcEn;
  UINT8                   SaiPolicyGroupWaBiosW;
  UINT8                   PatrolScrubAddrMode;
  UINT8                   partialmirrorsad0;
  UINT8                   PartialMirrorUefi;
  UINT16                  PartialMirrorUefiPercent;
  UINT16                  partialmirrorsize[MAX_PARTIAL_MIRROR]; // Array of sizes of different partial mirrors
  UINT8                   ImmediateFailoverAction;
  UINT8                   PlusOneEn;
  UINT8                   MemCeFloodPolicy;
  UINT16                  spareErrTh;
  UINT8                   TriggerSWErrThEn;
  UINT16                  SpareSwErrTh;
  UINT16                  timeWindow;
  UINT8                   DieSparing;
  UINT8                   PclsEn;
  UINT8                   ADDDCEn;
  UINT8                   DcpmmEccModeSwitch;
  UINT8                   AdddcErrInjEn;
  UINT8                   leakyBktTimeWindow;
  UINT8                   leakyBktLo;
  UINT8                   leakyBktHi;
  UINT16                  leakyBktHour;
  UINT8                   leakyBktMinute;
  UINT8                   CmdNormalization;
  UINT8                   LrDimmBacksideVrefEn;
  UINT8                   CmdVrefEnable;
  UINT8                   DramRonEn;
  UINT8                   McRonEn;
  UINT8                   RxCtleTrnEn;
  UINT8                   RxOdtEn;
  UINT8                   LrDimmWrVrefEn;
  UINT8                   LrDimmRdVrefEn;
  UINT8                   LrDimmTxDqCentering;
  UINT8                   LrDimmRxDqCentering;
  UINT8                   txEqCalibration;
  UINT8                   CmdTxEqCalibration;
  UINT8                   RxDfe;
  UINT8                   TxRiseFallSlewRate;
  UINT8                   iModeTraining;
  UINT8                   TcoCompTraining;
  UINT8                   RoundTripLatency;
  UINT8                   DutyCycleTraining;
  UINT8                   PxcTraining;
  UINT8                   DdjcTraining;
  UINT8                   refreshMode;
  UINT8                   dllResetTestLoops;
  UINT8                   DdrMemoryType;
  UINT8                   HwMemTest;
  UINT16                  MemTestLoops;
  UINT8                   TrainingResultOffsetFunctionEnable;
  UINT16                  OffsetTxDq;
  UINT16                  OffsetRxDq;
  UINT16                  OffsetTxVref;
  UINT16                  OffsetRxVref;
  UINT16                  OffsetCmdAll;
  UINT16                  OffsetCmdVref;
  UINT16                  OffsetCtlAll;
  UINT32                  AdvMemTestOptions;
  UINT8                   AdvMemTestResetList;
  UINT8                   AdvMemTestCondition;
  UINT16                  AdvMemTestCondVdd;
  UINT8                   AdvMemTestCondTwr;
  UINT16                  AdvMemTestCondTrefi;
  UINT32                  AdvMemTestCondPause;
  UINT8                   EccSupport;
  UINT8                   EccEnable;
  UINT8                   DfxTimingOverrideCtrl;
  UINT8                   DfxInputRt;
  UINT8                   DfxInputIoLatency;
  UINT8                   DfxInputIoLatencyIoComp;
  UINT16                  DfxInputTxDqDelay;
  UINT16                  DfxInputTxDqsDelay;
  UINT16                  DfxInputTxDqBitDelay;
  UINT16                  DfxInputDdrtTxDqsDelay;
  UINT16                  DfxInputDdrtTxDqBitDelay;
  UINT8                   DfxInputRxDqDelay;
  UINT16                  DfxInputRxDqsDelay;
  UINT8                   DfxInputRxDqBitDelay;
  UINT16                  DfxInputGnt2Erid;
  UINT8                   DfxCwlAdjPos;
  UINT8                   DfxCwlAdjNeg;
  UINT16                  DfxRcvEnDelay;
  UINT16                  DfxDdrtRcvEnDelay;
  UINT8                   DfxDdrtRt;
  UINT8                   DfxDdrtChInterleaveGran;
  UINT8                   dfxDimmManagement;
  UINT8                   DfxTmeCmiCplFabCreditWa;
  UINT8                   DfxMcDataParityCheck;
  UINT8                   dfxPartitionDDRTDimm;
  UINT8                   dfxCmdFubMarginging;
  UINT8                   DfxDdrtSchedulerDebug;
  UINT8                   dfxPartitionRatio[16];    //[MAX_SOCKET * MAX_IMC] = [8]
  UINT8                   volMemMode;
  UINT8                   CacheMemType; //Only valid if volMemMode is 2LM
  UINT8                   DdrCacheSize;
  UINT8                   PmemCaching;
  UINT8                   dfxPerMemMode;
  UINT8                   memInterleaveGran1LM;
  UINT8                   dfxMemInterleaveGranPMemUMA;
  UINT8                   dfxCfgMask2LM;
  UINT8                   DfxDimmInterleaving;
  UINT8                   DfxMod3Support;
  UINT8                   DfxRankInterleaving;
  UINT8                   CkeProgramming;
  UINT8                   SrefProgramming;
  UINT8                   PkgcSrefEn;
  UINT8                   CkeIdleTimer;
  UINT8                   ApdEn;
  UINT8                   PpdEn;
  UINT8                   DdrtCkeEn;
  UINT8                   OppSrefEn;
  UINT8                   OppSrefVisible;    //Setup variable to hide Opportunistic Self Refresh Knob
  UINT8                   DdrtSrefEn;
  UINT8                   DataDllOff;
  UINT8                   MdllOffEn;
  UINT8                   CkMode;
  UINT8                   MemTestOnColdFastBoot;
  UINT8                   AttemptFastBoot;
  UINT8                   AttemptFastBootCold;
  UINT8                   bdatEn;
  UINT8                   ScrambleEnDDRT;
  UINT8                   ScrambleEn;  // for ddr4
  UINT8                   allowCorrectableError;
  UINT8                   allowCorrectableMemTestError;
  UINT16                  ScrambleSeedLow;
  UINT16                  ScrambleSeedHigh;
  UINT8                   CustomRefreshRateEn;
  UINT8                   CustomRefreshRate;
  UINT8                   readVrefCenter;
  UINT8                   wrVrefCenter;
  UINT8                   haltOnMemErr;
  UINT8                   thermalthrottlingsupport;
  UINT8                   MemTripReporting;
  UINT8                   OffPkgMemToThermTrip;
  UINT8                   OffPkgMemToMemTrip;
  UINT8                   InPkgMemToThermTrip;
  UINT8                   InPkgMemToMemTrip;
  UINT8                   DimmTempStatValue;
  UINT8                   XMPProfilesSup;
  UINT8                   XMPMode;
  UINT8                   tCAS;
  UINT8                   tRP;
  UINT8                   tRCD;
  UINT8                   tRAS;
  UINT8                   tWR;
  UINT16                  tRFC;
  UINT8                   tRRD;
  UINT8                   tRRD_L;
  UINT8                   tRTP;
  UINT8                   tWTR;
  UINT8                   tFAW;
  UINT8                   tCWL;
  UINT8                   tRC;
  UINT8                   commandTiming;
  UINT16                  tREFI;
  UINT8                   DdrFreqLimit;
  UINT16                  Vdd;
  UINT8                   lrdimmModuleDelay;
  UINT32                  rmtPatternLength;
  UINT32                  rmtPatternLengthExt;
  UINT8                   RecEnDelayAverage;
  UINT8                   check_pm_sts;
  UINT8                   check_platform_detect;
  UINT8                   MemPwrSave;
  UINT8                   ElectricalThrottlingMode;
  UINT8                   MultiThreaded;
  UINT8                   promoteMrcWarnings;
  UINT8                   promoteWarnings;
  UINT8                   oppReadInWmm;
  UINT16                  normOppInterval;
  UINT8                   dfxRankMask[96]; //[MAX_SETUP_SOCKET * MAX_SETUP_IMC * MAX_SETUP_MC_CH] = [8 * 4 * 3 = 96]
  UINT8                   mdllSden;
  UINT8                   memhotSupport;
  UINT8                   MemHotIn;
  UINT8                   MemHotOut;
  UINT8                   MemhotOutputOnlyOpt;
  UINT8                   ADREn;
  UINT8                   RankMargin;
  UINT8                   EnableBacksideRMT;
  UINT8                   EnableBacksideCMDRMT;
  UINT8                   EnableNgnBcomMargining;
  UINT8                   dfxRankMaskEn;
  UINT8                   RankSparing;
  UINT8                   multiSparingRanks;
  UINT8                   dfxCAparity;
  UINT8                   dimmIsolation;
  UINT8                   smbSpeed;
  UINT8                   SmbSpdAccess;
  UINT8                   SpdPrintEn;
  UINT16                  SpdPrintLength;
  UINT8                   EnforcePOR;
  UINT8                   pda;
  UINT8                   turnaroundOpt;
  UINT8                   turnaroundOptDdrt;
  UINT8                   oneRankTimingMode;
  UINT8                   eyeDiagram;
  UINT8                   NvmdimmPerfConfig;
  UINT8                   EnableBiosSsaLoader;
  UINT8                   EnableBiosSsaRMT;
  UINT8                   RmtOnAdvancedTraining;
  UINT8                   EnableBiosSsaRMTonFCB;
  UINT8                   BiosSsaPerBitMargining;
  UINT8                   BiosSsaDisplayTables;
  UINT8                   BiosSsaPerDisplayPlots;
  UINT8                   BiosSsaLoopCount;
  UINT8                   BiosSsaBacksideMargining;
  UINT8                   BiosSsaEarlyReadIdMargining;
  UINT8                   BiosSsaStepSizeOverride;
  UINT8                   BiosSsaRxDqs;
  UINT8                   BiosSsaRxVref;
  UINT8                   BiosSsaTxDq;
  UINT8                   BiosSsaTxVref;
  UINT8                   BiosSsaCmdAll;
  UINT8                   BiosSsaCmdVref;
  UINT8                   BiosSsaCtlAll;
  UINT8                   BiosSsaEridDelay;
  UINT8                   BiosSsaEridVref;
  UINT8                   BiosSsaDebugMessages;
  UINT8                   DramRaplPwrLimitLockCsr;
  UINT8                   DramRaplEnable;
  UINT8                   BwLimitTfOvrd;
  UINT8                   perbitmargin;
  UINT8                   DramRaplExtendedRange;
  UINT8                   CmsEnableDramPm;
  UINT8                   logParsing;
  UINT8                   WritePreamble;
  UINT8                   ReadPreamble;
  UINT8                   WrCRC;
  UINT8                   AepOnSystem;
  UINT8                   EkvOnSystem;   // 0 => Do not suppress power management policy for BWV, 1 => suppress power management policy for BWV.
  UINT8                   BwvOnSystem;   // 0 => Do not suppress power management policy for EKV, 1 => suppress power management policy for EKV.
  // NGN options
  UINT8                   LockNgnCsr;
  UINT8                   NgnCmdTime;
  UINT8                   NgnEccCorr;
  UINT8                   NgnEccWrChk;
  UINT8                   NgnEccRdChk;
  UINT8                   NgnEccExitCorr;
  UINT8                   NgnDebugLock;
  UINT8                   NgnArsPublish;
  UINT8                   RmtOnColdFastBoot;
  UINT8                   LegacyRmt;
  UINT8                   mrcRepeatTest;
  UINT8                   dfxLowMemChannel;
  UINT8                   dfxHighAddressStartBitPosition;
  UINT8                   dfxStraddleHighAddressBit;
  UINT8                   staggerref;
  UINT32                  memFlows;
  UINT32                  memFlowsExt;
  UINT32                  memFlowsExt2;
  UINT32                  memFlowsExt3;
  UINT8                   Blockgnt2cmd1cyc;
  UINT8                   TrefiPerChannel;
  UINT8                   TrefiNumofRank;
  UINT16                  TrefiDelay;
  UINT8                   Disddrtopprd;
  UINT16                  NgnAveragePower;
  UINT8                   setSecureEraseAllDIMMs;
  UINT8                   setSecureEraseSktCh[64];  // [MAX_SOCKET * MAX_IMC * MAX_MC_CH]
  UINT8                   SetSecureEraseSktChHob[64]; // [MAX_SOCKET * MAX_IMC * MAX_MC_CH]
  UINT8                   AppDirectMemoryHole;
  UINT8                   LatchSystemShutdownState;
  UINT8                   ExtendedType17;
  //
  // PPR related
  //
  UINT8                   pprType;
  UINT8                   pprErrInjTest;
  // CR QoS Configuration Profiles
  UINT8                   FastGoConfig;
  // load NGN dimm mgmt driver
  UINT8                   dfxLoadDimmMgmtDriver;
  UINT8                   LegacyADRModeEn;
  UINT8                   MinNormalMemSize;
  UINT8                   ADRDataSaveMode;
  UINT8                   eraseArmNVDIMMS;
  UINT8                   restoreNVDIMMS;
  UINT8                   interNVDIMMS;
  UINT8                   imcBclk;
  UINT8                   spdCrcCheck;
  UINT8                   DfxDdrtTransactor;
  UINT8                   SETUP_REMOVE_SanitizeOverwriteNvmDimm;  // removed
  UINT8                   EliminateDirectoryInFarMemory;
  UINT8                   NvmdimmPowerCyclePolicy;

  //CMI Init option
  UINT8                   CmiInitOption;
  //
  // Cmd setup hold percent offset for 2n timing
  //
  UINT8                   cmdSetupPercentOffset;
  UINT8                   ShortStroke2GB;
  UINT8                   NvDimmEnergyPolicy;     //Energy Policy Management
  UINT8                   DfxDramRxEqTraining;
  UINT8                   DfxDramTxEcho;
  UINT8                   DfxDramLegacyRxDfe;
  UINT8                   DfxRxCtleExtendedRangeEn;
  UINT8                   DisableDirForAppDirect;
  UINT8                   NvmMediaStatusException;
  UINT8                   NvmQos;
  //
  // Ssa Loader Setup Question Visiblity based on Library inclusion.
  //
  UINT8                   SsaLoaderKnobVisible;      /// 0 => Hide SSA RMT Loader knob, 1 => Show knob
  UINT8                   LegacyRmtKnobVisible;      /// 0 => Hide Legacy RMT related knob, 1 => Show knob
  UINT8                   SsaBuiltInRmtKnobVisible;  ///< Ssa Built-In-Rmt Setup question visiblity based on Library inclusion.

  //
  // FIS 2.x
  //
  UINT16                  DcpmmAveragePowerLimit;        // Power limit in mW used for averaged power ( Valid range starts from 10000mW).
  UINT8                   DcpmmAveragePowerTimeConstant;    // Value used as a base time window for power usage measurements Turbo Mode Support(in mSec).
  UINT32                  DcpmmMbbAveragePowerTimeConstant; // Value used as a base time window for power usage measurements Memory Bandwidth Boost Support(in mSec).
  UINT8                   DcpmmMbbFeature;                  // Allows enabling and disabling the feature (Turbo Mode State/Memory Bandwidth Boost).
  UINT16                  DcpmmMbbMaxPowerLimit;            // Power limit in mW used for limiting the Turbo Mode/Memory Bandwidth Boost
                                                            // power consumption (Valid range starts from 15000mW).

  //
  // NFIT implementation debug options
  //
  UINT8                   DfxNfitDebugLogs;
  UINT8                   DfxNfitNvdimmSkuBased;
  UINT8                   DfxNfitMailboxWithPropGuid;

  UINT8                   DfxCrAcpiDebugInterface;
  UINT8                   LsxImplementation;
  UINT8                   FactoryResetClear;
  UINT8                   EadrSupport;
  UINT32                  NvdimmSmbusMaxAccessTime;
  UINT32                  NvdimmSmbusReleaseDelay;
  UINT8                   NfitPublishMailboxStructs;

  //
  // fADR setup option
  //
  UINT8                   FadrSupport;

  //
  // Biased 2-way near memory cache support options
  //
  UINT8                   EnableTwoWayNmCache;          // Enable or disable biased 2-way near memory cache.
  UINT16                  NonPreferredWayMask;          // A 10-bit mask to control the bias counter ratio.
  UINT8                   PreferredReadFirst;           // Reads are issued to the non-preferred or preferred way first.

  //
  // Boot-time fast zero memory setup option
  //
  UINT8                   FastZeroMemSupport;           // Enable or disable boot-time fast zero memory support.

  //
  // Early completion setup options
  //
  UINT8                   DfxDdrEarlyCmpEn;             // Enable or disable early completion for all DDR memory controllers.
  UINT8                   DfxHbmEarlyCmpEn;             // Enable or disable early completion for all HBM memory controllers.

  //
  // XOR decoder options
  //
  UINT8                   DfxClusterXorEn;              // Enable or disable cluster XOR for DDR.
  UINT8                   DfxChannelXorEn;              // Enable or disable channel XOR for DDR.
  UINT8                   DfxPseudoChannelXorEn;        // Enable or disable pseudo-channel XOR for DDR.
  UINT8                   DfxDimmXorEn;                 // Enable or disable DIMM XOR for DDR.
  UINT8                   DfxRankXorEn;                 // Enable or disable rank XOR for DDR.
  UINT8                   DfxSidXorEn;                  // Enable or disable subrank XOR for DDR.
  UINT8                   DfxBankXorEn;                 // Enable or disable bank XOR for DDR.

  UINT8                   DfxPostedCsrAccessAllowed;    // 0 => Do not allow posted CSR accesses, 1 => Allow posted CSR accesses
  UINT8                   DfxCpgcRegCacheAccessAllowed; // 0 => Do not allow CPGC register cache access, 1 => CPGC register cache access

  UINT8                   DfxSsaRmtRecvEnableDenormalize;  // 0 => Disable Denormalize before SSA RMT, 1 => Enable Denormalize before SSA RMT
  UINT16                  DfxInputCmdDelay;             // PI value of CMD delay for all channels.
  UINT8                   DfxCrSkipArsOnBoot;
  UINT8                   DfxUbiosSmbusTrace;
  UINT8                   DfxSmbusI3cMode;
  UINT8                   DfxCpgcGlobalSyncCtrl;        // Controls whether the synchronous CPGC start mechanism is enable
  UINT8                   DcpmmApiVersion200OnSystem;   // 0 => Suppress DcpmmAveragePowerTimeConstant, 1 => Do not suppress DcpmmAveragePowerTimeConstant.
  UINT8                   DcpmmApiVersion201OnSystem;   // 0 => Suppress DcpmmAveragePowerTimeConstant, 1 => Do not suppress DcpmmAveragePowerTimeConstant.
  UINT8                   DfxCrBackgroundArs;
  UINT8                   EnforcePopulationPor;

  //
  // DFE Path Finding
  //
  UINT8                   EnableTapSweep;

  UINT8                   DfeGainBias;

  UINT8                   Tap1Start;
  UINT8                   Tap1End;
  UINT8                   Tap1Size;

  UINT8                   Tap2Start;
  UINT8                   Tap2End;
  UINT8                   Tap2Size;

  UINT8                   Tap3Start;
  UINT8                   Tap3End;
  UINT8                   Tap3Size;

  UINT8                   Tap4Start;
  UINT8                   Tap4End;
  UINT8                   Tap4Size;

  UINT8                   TrainingCompOptions;          // Memory Training Comp Options Values.

  UINT8                   PeriodicRcomp;                // Memory Periodic Rcomp Auto/Enable/Disable.
  UINT8                   PeriodicRcompInterval;        // Memory Periodic Rcomp Interval.

  UINT8                   DfxPeriodicZqCal;             // DFX Periodic ZQCal enable/disable

  UINT8                   AepNotSupportedException;

  UINT8                   DfxSenseAmpCalibHwSwOption;   // Sense Amp Calibration can be trained using HW FSM or SW based algorithms

  UINT8                   DfxCrSeamlessOptIn;
  UINT8                   PanicWm;

  UINT16                  OffsetRecEn;

  UINT8                   EadrCacheFlushMode;
  UINT8                   DfxEadrDebugLogs;
  UINT8                   DfxEadrFlowTimeLogs;
  UINT8                   DfxEadrProtocolLogs;
  UINT8                   DfxDdr5RefreshMode;

  UINT8                   EarlyWrCRCDdrio;
  UINT8                   DfxDdr5SsaRxSamplerSelection;
  UINT8                   DfxSwXoverOption;             // SW Xover can be run in parallel or serial mode
  UINT8                   LrdimmDbDfeTraining;

  UINT8                   DfxFadrDebugLogs;
  UINT8                   DfxFadrFlowTimeLogs;
  UINT8                   DfxForceOdtOn;

  UINT8                   DramDutyCycleAdjusterTrainingKnob;

  UINT8                   RxDfeDdr5Training;
  UINT8                   TxDfeDdr5Training;
#ifdef CWV_A0_REMOVE_POST_PO
  UINT8                   DfxDdrtSkipMB2Command;
#endif
} SOCKET_MEMORY_CONFIGURATION;

#pragma pack()

#endif

