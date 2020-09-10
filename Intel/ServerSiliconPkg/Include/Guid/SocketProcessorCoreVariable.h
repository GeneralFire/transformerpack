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

#ifndef   __SOCKET_PROCESSORCORE_CONFIGURATION_DATA_H__
#define   __SOCKET_PROCESSORCORE_CONFIGURATION_DATA_H__

#include <UncoreCommonIncludes.h>
#include "SocketConfiguration.h"

extern EFI_GUID gEfiSocketProcessorCoreVarGuid;
#define SOCKET_PROCESSORCORE_CONFIGURATION_NAME L"SocketProcessorCoreConfig"

#pragma pack(1)

typedef struct {

  UINT8   CpuidMaxValue;

  UINT8   ProcessorHyperThreadingDisable;   // Hyper Threading [ALL]
  UINT8   ProcessorVmxEnable;               // Enabling VMX
  UINT8   ProcessorSmxEnable;               // Enabling SMX (TXT LT)
  UINT8   ProcessorLtsxEnable;              // Enabling TXT (TXT LT-SX)
  UINT8   ThreeStrikeTimer;                 // Disable 3strike timer
  UINT8   FastStringEnable;                 // Fast String
  UINT8   MachineCheckEnable;               // Machine Check
  UINT8   MlcStreamerPrefetcherEnable;      // Hardware Prefetch
  UINT8   MlcSpatialPrefetcherEnable;       // Adjacent Cache Line Prefetch
  UINT8   DCUStreamerPrefetcherEnable;      // DCU Streamer Prefetcher
  UINT8   DCUIPPrefetcherEnable;            // DCU IP Prefetcher
  UINT8   DCUModeSelection;                 // DCU Mode Selection
  UINT8   ProcessorX2apic;                  // Enable Processor XAPIC
  UINT8   BspSelection;                     // Select BSP
  UINT8   IedTraceSize;                     // IED trace size
  UINT8   TsegSize;                         // TSEG size
  UINT8   AllowMixedPowerOnCpuRatio;        // Allow Mixed PowerOn CpuRatio
  UINT8   CheckCpuBist;                     // check and disable BIST faile core or ignore
  UINT8   CoreFailover;                     // Enable spare core(s) in place of core(s) that fail BIST
  UINT64  DfxBistFailureEmulation;          // Emulation of failed cores to test core failover
  UINT8   ProcessorFlexibleRatio;           // Non-Turbo Mode Processor Core Ratio Multiplier
  UINT8   ProcessorFlexibleRatioOverrideEnable;         // Non-Turbo Mode Processor Core Ratio Multiplier Enable
  UINT8   ForcePhysicalModeEnable;          // Force physical destionation mode
  UINT8   LlcPrefetchEnable;                // LLC Prefetch
  UINT8   CpuMtoIWa;                        // MtoI Workaround;
  UINT8   ProcessorVirtualWireMode;

  UINT8   ProcessorMsrLockControl;          // MSR Lock Bit Control
  UINT8   ProcessorMsrPkgCstConfigControlLock; // MSR PKG_CST_CONFIG_CONTROL lock
  UINT8   DebugInterface;                   // IA32_DEBUG_INTERFACE_MSR
  UINT8   AesEnable;
  UINT8   PpinControl;                      // PPIN Control MSR
  UINT8   LockChipset;                      // Lock Chipset
  UINT8   BiosAcmErrorReset;                // Disable LT-SX and reset system when BIOS ACM error occurs
  UINT8   AcmType;                          // 0x80 = debug signed ACM; 0x40 = NPW production signed ACM; 0x00 = PW production signed ACM

  // SecurityPolicy Stalagmite
  #include <Guid/SecurityPolicy_Flat.h>

  UINT8   SkipStopPbet;                     // Skip StopPbet

  UINT64  CoreDisableMask[MAX_SOCKET];      // one for each CPU socket

  // IOT/OCLA configs
#ifndef OCLA_TOR_ENTRY_MAX
    #define OCLA_TOR_ENTRY_MIN         0
    #define OCLA_TOR_ENTRY_MAX      0x11      // 15 or 17 depending on Isoch on/off
    #define OCLA_TOR_ENTRY_DEFAULT     1
    #define OCLA_WAY_MIN               0
    #define OCLA_WAY_MAX               8      // max 8 LLC ways out of 11 can be reserved for OCLA
    #define OCLA_WAY_DEFAULT           1
#endif
  UINT8   IotEn[MAX_SOCKET];
  UINT8   OclaMaxTorEntry[MAX_SOCKET];
  UINT8   OclaMinWay[MAX_SOCKET];
  UINT32  IioLlcWaysMask;                         // MSR CBO_SLICE0_CR_IIO_LLC_WAYS bitmask. - Only Bits[22:0] are used
  UINT32  ExpandedIioLlcWaysMask;                 // MSR INGRESS_SPARE[10:0] bitmask. - Only Bits[10:0] are used
  UINT32  RemoteWaysMask;                         // MSR INGRESS_SPARE[26:16] bitmask. - Only Bits[10:0] are used

  UINT8   PCIeDownStreamPECIWrite;

  //
  // SMM Blocked and SMM Delayed
  //
  UINT8 SmmBlockedDelayed;

  //
  // eSMM Save State Mode
  //
  UINT8 eSmmSaveState;

  UINT8 PeciInTrustControlBit; //On Setup

  UINT8  Poison;
  UINT8  Viral;
  UINT8  EVMode;
  UINT8  SmbusErrorRecovery;
  UINT8  CpuL1NextPagePrefetcherDisable;
  UINT8  CpuPaLimit;
  UINT8  RdtCatOpportunisticTuning;
  UINT8  CpuDbpEnable;         // Enable/Disable DBP-F
  UINT8  GlobalPsmiEnable;
  UINT8  PsmiTrace[MAX_SOCKET];
  UINT8  PsmiHandlerSize[MAX_SOCKET];
  UINT8  PsmiTraceRegion0[MAX_SOCKET];
  UINT8  PsmiTraceBufferSizeRegion0[MAX_SOCKET];
  UINT8  PsmiTraceMemTypeRegion0[MAX_SOCKET];
  UINT8  PsmiTraceRegion1[MAX_SOCKET];
  UINT8  PsmiTraceBufferSizeRegion1[MAX_SOCKET];
  UINT8  PsmiTraceMemTypeRegion1[MAX_SOCKET];
  UINT8  PsmiTraceRegion2[MAX_SOCKET];
  UINT8  PsmiTraceBufferSizeRegion2[MAX_SOCKET];
  UINT8  PsmiTraceMemTypeRegion2[MAX_SOCKET];
  UINT8  PsmiTraceRegion3[MAX_SOCKET];
  UINT8  PsmiTraceBufferSizeRegion3[MAX_SOCKET];
  UINT8  PsmiTraceMemTypeRegion3[MAX_SOCKET];
  UINT8  PsmiTraceRegion4[MAX_SOCKET];
  UINT8  PsmiTraceBufferSizeRegion4[MAX_SOCKET];
  UINT8  PsmiTraceMemTypeRegion4[MAX_SOCKET];
  UINT8  L2RfoPrefetchDisable;      // L2 RFO Prefetch
  UINT8  AmpPrefetchEnable;
  UINT8  TscResetEnable;
  UINT8  AcExceptionOnSplitLockEnable;
  // FuSa (SAF) Start
  UINT8   SafSupport;
  UINT8   EnableSaf;
  // FuSa (SAF) End
  UINT8  CFRS3mEnable;
  UINT8  CFRS3mManualCommit;
  UINT8  CFRPucodeEnable;
  UINT8  CFRPucodeManualCommit;
} SOCKET_PROCESSORCORE_CONFIGURATION;
#pragma pack()

#endif
