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

#ifndef   __SOCKET_MP_LINK_CONFIG_DATA_H__
#define   __SOCKET_MP_LINK_CONFIG_DATA_H__

#include <UncoreCommonIncludes.h>
#include "SocketConfiguration.h"

extern EFI_GUID gEfiSocketMpLinkVariableGuid;
#define SOCKET_MP_LINK_CONFIGURATION_NAME L"SocketMpLinkConfig"

#pragma pack(1)
typedef struct {
  // SKXTODO: rename to Kti when removing HSX code
  UINT8  QpiSetupNvVariableStartTag;  // This must be the very first one of the whole KTI Setup NV variable!

  //
  // Used by the PciHostBridge DXE driver, these variables don't need to be exposed through setup options
  // The variables are used as a communication vehicle from the PciHostBridge DXE driver to an OEM hook
  // which updates the KTI resource map
  //
  //
  //  KTI Host structure inputs
  //
  UINT8  BusRatio[MAX_SOCKET];
  UINT8  D2KCreditConfig;      // 1 - Min, 2 - Med (Default), 3- Max
  UINT8  SnoopThrottleConfig;  // 0 - Disabled, 1 - Min, 2 - Med (Default), 3- Max
  UINT8  LegacyVgaSoc;         // Socket that claims the legacy VGA range; valid values are 0-3; 0 is default.
  UINT8  LegacyVgaStack;       // Stack that claims the legacy VGA range; valid values are 0-3; 0 is default.
  UINT8  P2pRelaxedOrdering;   // 1 - Enable; 0 - Disable
  UINT8  DebugPrintLevel;      // Bit 0 - Fatal, Bit1 - Warning, Bit2 - Info Summary; Bit 3 - Info detailed. 1 - Enable; 0 - Disable
  UINT8  DegradePrecedence;    // Use DEGRADE_PRECEDENCE definition; TOPOLOGY_PRECEDENCE is default
  UINT8  Degrade4SPreference;  // 4S1LFullConnect topology is default; another option is 4S2LRing topology.

  //
  // Phy/Link Layer Options
  //
  UINT8  QpiLinkSpeedMode;         // Link speed mode selection; 0 - Slow Speed; 1- Full Speed
  UINT8  QpiLinkSpeed;             // One of SPEED_REC_96GT, SPEED_REC_104GT, MAX_KTI_LINK_SPEED (default), FREQ_PER_LINK
  UINT8  KtiLinkL0pEn;             // 0 - Disable, 1 - Enable, 2- Auto (default)
  UINT8  KtiLinkL1En;              // 0 - Disable, 1 - Enable, 2- Auto (default)
  UINT8  KtiFailoverEn;            // 0 - Disable, 1 - Enable, 2- Auto (default)
  UINT8  KtiLbEn;                  // 0 - Disable(default), 1 - Enable
  UINT8  KtiCrcMode;               // 0 - 8 bit CRC 1 - 16 bit CRC Mode
  UINT8  QpiCpuSktHotPlugEn;       // 0 - Disable (default), 1 - Enable
  UINT8  KtiCpuSktHotPlugTopology; // 0 - 4S Topology (default), 1 - 8S Topology
  UINT8  KtiSkuMismatchCheck;      // 0 - No, 1 - Yes (default)
  UINT8  KtiLinkVnaOverride;       // 0x100 - per link, 0xff - max (default), 0x00 - min
  UINT8  SncEn;                    // 0 - Disable (default), 1 - Enable
  UINT8  IoDcMode;                 // 0 - Disable IODC,  1 - AUTO (default), 2 - IODC_EN_REM_INVITOM_PUSH, 3 - IODC_EN_REM_INVITOM_ALLOCFLOW
                                   // 4 - IODC_EN_REM_INVITOM_ALLOC_NONALLOC, 5 - IODC_EN_REM_INVITOM_AND_WCILF
  UINT8  DirectoryModeEn;          // 0 - Disable; 1 - Enable (default)
  UINT8  XptPrefetchEn;            // XPT Prefetch :  1 - Enable (Default); 0 - Disable
  UINT8  KtiPrefetchEn;            // KTI Prefetch :  1 - Enable (Default); 0 - Disable
  UINT8  XptRemotePrefetchEn;      // XPT Remote Prefetch :  1 - Enable (Default); 0 - Disable
  UINT8  RdCurForXptPrefetchEn;    // RdCur for XPT Prefetch :  0 - Disable, 1 - Enable, 2- Auto (default)
  UINT8  IrqThreshold;             // KTI IRQ Threshold setting
  UINT8  TorThresLoctoremNorm;     // TOR threshold - Loctorem threshold normal
  UINT8  TorThresLoctoremEmpty;    // TOR threshold - Loctorem threshold empty
  UINT8  TscSyncEn;                // TSC Sync Enable: 0 - Disable; 1 - Enable; 2 - AUTO (default)
  UINT8  StaleAtoSOptEn;           // HA A to S directory optimization
  UINT8  LLCDeadLineAlloc;         // Never fill dead lines in LLC: 1 - Enable, 0 - Disable
  UINT8  SplitLock;                // Setting this bit removes the Ubox PCU handshake during stopreq and startreq only
                                   // for splitlocks: - Enable, 0 - Disable
  UINT8  KtiAdaptationEn;          // 0 - Disable, 1 - Enable
  UINT8  KtiAdaptationSpeed;       // Use KTI_LINK_SPEED definition; MAX_KTI_LINK_SPEED - Auto (i.e BIOS choosen speed)

  //
  // KTI DFX variables
  //
  UINT8  DfxSystemWideParmStart;       // This must be the first DFX variable
  UINT8  DfxHaltLinkFailReset;         // 2 - Auto; 1 - Enable; 0 - Disable
  UINT8  DfxKtiMaxInitAbort;           // 0 - Disable; 1 - Enable; 2 - AUTO (default)
  UINT8  DfxLlcShareDrdCrd;            // Enable migration from SF to LLC and to leave shared lines in the LLC for Drd and Crd: 1 - Enable; 1 - Disable; 2 - Auto
  UINT8  DfxBiasFwdMode;               // 0 - Mode 0 (Fwd only when Hom != Req); 1 - Mode 1 (Fwd when Hom != Req & Hom != Local); 2 - Mode 2 (Disable Bias Fwd)
  UINT8  DfxSnoopFanoutEn;             // snoop fanout enable 0: disable 1: enable 2 - Auto (default) (EX only)
  UINT8  DfxHitMeEn;                   // CHA HitME$ Enable: 1 - Enable; 0 - Disable; 2 - Auto (Default)
  UINT8  DfxFrcfwdinv;                 // Enable alias all conflict flows to FwdInvItoE behaviour: 1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8  DfxDbpEnable;                 // Dbp Enable : 1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8  DfxCleanEvictAlwaysLive;      // Clean Evict Always Live: 1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8  DfxModifiedEvictAlwaysLive;   // Modified Evict Always Live: 1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8  DfxOsbEn;                     // OSB Enable: 1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8  DfxHitMeRfoDirsEn;            // Enable HitME DIR=S RFO optimization: 1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8  DfxGateOsbIodcAllocEn;        // When OSB indicates that there aren't enough snoop credits don't allocate IODC entry: 1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8  DfxDualLinksInterleavingMode; // In 2S 2KTI can: 2 - Auto - do nothing (default); 1 - use legacy CHA interleaving (disable SNC, turn off XOR interleave); 0 - disable D2C
  UINT8  DfxSystemDegradeMode;         // 0 - Degrade to 1S; 1 - Degarde to supported topology (default); 2 - Leave the topology as is.
  UINT8  DfxVn1En;                     // VN1 enable 0: disable 1: enable 2 - Auto (default) (EX only)
  UINT8  DfxD2cEn;                     // Direct To Core enable: 1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8  DfxD2kEn;                     // Direct To Kti enable: 1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8  DfxLockMaster;                // NodeId of the lock master: 0 - Socket0; 1 - Socket1; 2 - Socket2; 3 - Socket3; 4 - Auto (default)
  UINT8  DfxOsbLocRd;                  // OSB Local Rd Enable: 1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8  DfxOsbLocRdCur;               // OSB Local RdCur Enable: 1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8  DfxOsbRmtRd;                  // OSB Remote Rd Enable: 1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8  DfxM3KtiCountMismatchEn;      // Allow M3KTI count mismatch between SBSP and other sockets: 1 - Enable; 0 - Disable (default)
  UINT8  DfxSnoopFanoutChaInterleaveEn;// Balancing snoop fanout via CHA Interleave. 0 - Disable, 1 - Enable, 2 - AUTO.
  UINT8  DfxXptFifoEnabledCredit;      // XPT FIFO enabled credit number
  UINT8  DfxClockModulationEn;         // Clock Modulation Enabled. 0 - Disable, 1 - Enable, 2 - AUTO.
  UINT8  DfxMdfisAdvancedTrainingEn;   // MDFIS Advanced training Enabled. 0 - Disable, 1 - Enable, 2 - AUTO.
  UINT8  DfxCxlSecLvl;                 // CXL Security Level: 0 - Trusted CXL Device, 1 - Trusted for Device Attached Memory Range Only,
                                       // 2 - Untrusted IAL Device (default).
  UINT8  DfxCxlStcge;                  // CXL Arb-Mux Side Clock Trunk Clock Gating Enable: 1 - Enable, 0 - Disable
  UINT8  DfxCxlSdcge;                  // CXL Arb-Mux Side Clock Dynamic Clock Gating Enable: 1 - Enable, 0 - Disable
  UINT8  DfxCxlDlcge;                  // CXL Arb-Mux Dynamic Link Clock Gating Enable: 1 - Enable, 0 - Disable
  UINT8  DfxCxlLtcge;                  // CXL Arb-Mux Link Clock Trunk Clock Gating Enable: 1 - Enable, 0 - Disable
  UINT8  DfxCxlVid;                    // CXL Set Vid to 0x198: 0 - Disable, 1 - Enable, 2 - AUTO.
  UINT8  DfxPmonConfig;                // 1 - Uncore,MC,PCIe only, 2 - Uncore,MC,PCIe & M2IOSF,  3 - Auto (default), 0 - Disable
  UINT8  DfxM2IosfPmonAccessControl;   // 0 - M2IOSF PMON cannot be accessed, 1 - M2IOSF PMON can be accessed,  2 - Auto (default)
  UINT32 DfxIioStackEn;               //  MS2IOSF strap override: Auto(default),
                                       // Bitmap - order must match CAPID. 1 - behaves as defined by internal fuse 0 - given ms2iosf is disabled.
  UINT8  DfxHqmEn;                    // HQM strap override: 1 - Enable HQM; 0 - Disable HQM; 2 - Auto (default)
  UINT8  DfxSystemWideParmEnd;         // This must be the last DFX variable

#define  UPICPUPRTVARIABLE(x)       x##KtiPortDisable;x##KtiLinkSpeed;x##KtiLinkVnaOverride;

  UINT8 KtiCpuPerPortStartTag;
  UPICPUPRTVARIABLE (UINT8 Cpu0P0)
  UPICPUPRTVARIABLE (UINT8 Cpu0P1)
  UPICPUPRTVARIABLE (UINT8 Cpu0P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTVARIABLE (UINT8 Cpu0P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTVARIABLE (UINT8 Cpu0P4)
  UPICPUPRTVARIABLE (UINT8 Cpu0P5)
#endif
#if MAX_SOCKET > 1
  UPICPUPRTVARIABLE (UINT8 Cpu1P0)
  UPICPUPRTVARIABLE (UINT8 Cpu1P1)
  UPICPUPRTVARIABLE (UINT8 Cpu1P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTVARIABLE (UINT8 Cpu1P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTVARIABLE (UINT8 Cpu1P4)
  UPICPUPRTVARIABLE (UINT8 Cpu1P5)
#endif
#endif
#if MAX_SOCKET > 2
  UPICPUPRTVARIABLE (UINT8 Cpu2P0)
  UPICPUPRTVARIABLE (UINT8 Cpu2P1)
  UPICPUPRTVARIABLE (UINT8 Cpu2P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTVARIABLE (UINT8 Cpu2P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTVARIABLE (UINT8 Cpu2P4)
  UPICPUPRTVARIABLE (UINT8 Cpu2P5)
#endif
#endif
#if MAX_SOCKET > 3
  UPICPUPRTVARIABLE (UINT8 Cpu3P0)
  UPICPUPRTVARIABLE (UINT8 Cpu3P1)
  UPICPUPRTVARIABLE (UINT8 Cpu3P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTVARIABLE (UINT8 Cpu3P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTVARIABLE (UINT8 Cpu3P4)
  UPICPUPRTVARIABLE (UINT8 Cpu3P5)
#endif
#endif
#if (MAX_SOCKET > 4)
  UPICPUPRTVARIABLE (UINT8 Cpu4P0)
  UPICPUPRTVARIABLE (UINT8 Cpu4P1)
  UPICPUPRTVARIABLE (UINT8 Cpu4P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTVARIABLE (UINT8 Cpu4P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTVARIABLE (UINT8 Cpu4P4)
  UPICPUPRTVARIABLE (UINT8 Cpu4P5)
#endif
#endif
#if (MAX_SOCKET > 5)
  UPICPUPRTVARIABLE (UINT8 Cpu5P0)
  UPICPUPRTVARIABLE (UINT8 Cpu5P1)
  UPICPUPRTVARIABLE (UINT8 Cpu5P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTVARIABLE (UINT8 Cpu5P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTVARIABLE (UINT8 Cpu5P4)
  UPICPUPRTVARIABLE (UINT8 Cpu5P5)
#endif
#endif
#if (MAX_SOCKET > 6)
  UPICPUPRTVARIABLE (UINT8 Cpu6P0)
  UPICPUPRTVARIABLE (UINT8 Cpu6P1)
  UPICPUPRTVARIABLE (UINT8 Cpu6P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTVARIABLE (UINT8 Cpu6P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTVARIABLE (UINT8 Cpu6P4)
  UPICPUPRTVARIABLE (UINT8 Cpu6P5)
#endif
#endif
#if (MAX_SOCKET > 7)
  UPICPUPRTVARIABLE (UINT8 Cpu7P0)
  UPICPUPRTVARIABLE (UINT8 Cpu7P1)
  UPICPUPRTVARIABLE (UINT8 Cpu7P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTVARIABLE (UINT8 Cpu7P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTVARIABLE (UINT8 Cpu7P4)
  UPICPUPRTVARIABLE (UINT8 Cpu7P5)
#endif
#endif

#define UPICPUPRTDFXVARIABLE(x)    x##DfxCrcMode;x##DfxL0pEnable;x##DfxL1Enable;x##DfxKtiFailoverEn;

  UINT8 DfxKtiCpuPerPortStartTag;
  UPICPUPRTDFXVARIABLE (UINT8 Cpu0P0)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu0P1)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu0P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTDFXVARIABLE (UINT8 Cpu0P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTDFXVARIABLE (UINT8 Cpu0P4)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu0P5)
#endif
#if MAX_SOCKET > 1
  UPICPUPRTDFXVARIABLE (UINT8 Cpu1P0)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu1P1)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu1P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTDFXVARIABLE (UINT8 Cpu1P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTDFXVARIABLE (UINT8 Cpu1P4)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu1P5)
#endif
#endif
#if MAX_SOCKET > 2
  UPICPUPRTDFXVARIABLE (UINT8 Cpu2P0)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu2P1)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu2P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTDFXVARIABLE (UINT8 Cpu2P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTDFXVARIABLE (UINT8 Cpu2P4)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu2P5)
#endif
#endif
#if MAX_SOCKET > 3
  UPICPUPRTDFXVARIABLE (UINT8 Cpu3P0)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu3P1)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu3P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTDFXVARIABLE (UINT8 Cpu3P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTDFXVARIABLE (UINT8 Cpu3P4)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu3P5)
#endif
#endif
#if MAX_SOCKET > 4
  UPICPUPRTDFXVARIABLE (UINT8 Cpu4P0)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu4P1)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu4P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTDFXVARIABLE (UINT8 Cpu4P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTDFXVARIABLE (UINT8 Cpu4P4)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu4P5)
#endif
#endif
#if MAX_SOCKET > 5
  UPICPUPRTDFXVARIABLE (UINT8 Cpu5P0)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu5P1)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu5P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTDFXVARIABLE (UINT8 Cpu5P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTDFXVARIABLE (UINT8 Cpu5P4)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu5P5)
#endif
#endif
#if MAX_SOCKET > 6
  UPICPUPRTDFXVARIABLE (UINT8 Cpu6P0)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu6P1)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu6P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTDFXVARIABLE (UINT8 Cpu6P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTDFXVARIABLE (UINT8 Cpu6P4)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu6P5)
#endif
#endif
#if MAX_SOCKET > 7
  UPICPUPRTDFXVARIABLE (UINT8 Cpu7P0)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu7P1)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu7P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTDFXVARIABLE (UINT8 Cpu7P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTDFXVARIABLE (UINT8 Cpu7P4)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu7P5)
#endif
#endif

  UINT8  QpiSetupNvVariableEndTag;  // This must be the last one of the whole KTI Setup NV variable
} SOCKET_MP_LINK_CONFIGURATION;

#pragma pack()

#endif // __SOCKET_MP_LINK_CONFIG_DATA_H__

