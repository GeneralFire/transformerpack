/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include <Gen2/MemDecodeNode.h>
#include <Include/MemMapHost.h>
#include <Library/MemMapDataLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/SecurityPolicyLib.h>
#include <Library/MemRcLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemRcLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/MemoryServicesLib.h>

/**

  Description: Provide total NM size in 2LM mode per socket

  @param [IN] MemMapData Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param [IN] socketNum  socket id

  @retval TotalMemSize   Total Near Memory size per socket

**/
UINT32
TotalNmSize (
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           SocketNum
  )
{
  UINT8     Mc;
  UINT8     Ch;
  UINT8     MaxImc;
  UINT8     MaxChPerMc;
  UINT32    TotalMemSize;
  SYSHOST   *Host;

  Host = GetSysHostPointer ();
  MaxImc = GetMaxImc ();
  MaxChPerMc = GetNumChannelPerMc ();
  TotalMemSize = 0;

  // Find the number of channels with ddr memory populated and add the size in each iMC
  for (Mc = 0; Mc < MaxImc; Mc++) {
    for (Ch = 0; Ch < MaxChPerMc; Ch++) {
      TotalMemSize += GetChannelMemSize (SocketNum, Ch + (Mc * MaxChPerMc));
    }
  }
  return TotalMemSize;
}

/**
  Get memory size in 64MB unit for a specified channel of near memory

  @param[in] Host       Pointers to global Host data structure.
  @param[in] MemMapData Pointer to the memory map private data structure
  @param[in] Sckt       The socket id
  @param[in] Ch         The channel id on socket

  @return the memory size in 64MB unit for the specified channel

**/
UINT32
GetNmChannelMemSize (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           Sckt,
  IN UINT8           Ch
  )
{
  return GetChannelMemSize (Sckt, Ch);
}

/**

  Find minimum NM size per channel for this MC.

  @param[in] Host       Pointer to sysHost
  @param[in] MemMapData Pointer to the memory map private data structure
  @param[in] Sckt       Socket number
  @param[in] Mc         MC index

  @retval Min NM size per channel for this MC

**/
UINT32
FindMinNmSize (
  IN PSYSHOST         Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8            Sckt,
  IN UINT8            Mc
  )
{
    return FindMinNmChSizePwrOf2 (Host, Sckt, Mc);
}

/** Performs CPU specific functions to initialize memory map parameters

  Performs 10nm Wave1/Wave2 specific memory map private data structure initialization besides the common part
  implemented in InitMemMapParams ().

  @param[in] Host       - Pointers to global Host data structure.
  @param[in] MemMapData - The private data structure for memory map.

  @retval    None

**/
VOID
InitMemMapParamsEx (
  IN SYSHOST          *Host,
  IN MEMORY_MAP_DATA  *MemMapData
  )
{
  UINT8                  Sckt;
  CAPID1_PCU_FUN3_STRUCT PcuCAPID1;
  SECURITY_POLICY        *SecurityPolicy;
  MEMORY_MAP_HOST        *MemMapHost;
  SYS_SETUP              *Setup;

  MemMapHost = GetMemMapHostPointer ();
  Setup = GetSysSetupPointer ();

  MemMapHost->PcatCapability.MemModeCapabilities = MODE_1LM | MODE_2LM | MODE_AD;

  //Adjust the memory modes if the the silicon has reduced capability
  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {
    if (!CheckSocketEnabled (Sckt)) {
      continue;
    }
    PcuCAPID1.Data = Host->var.common.procCom[Sckt].capid1;
    if (PcuCAPID1.Bits.disable_2lm == 1) {
      RcDebugPrint (SDBG_MAX, "Silicon capability does not support 2LM, forcing to 1LM.\n");
      Host->var.mem.volMemMode = VOL_MEM_MODE_1LM;
      MemMapHost->PcatCapability.MemModeCapabilities &= ~MODE_2LM;
    }
    if (PcuCAPID1.Bits.disable_cr_pmem == 1) {
      RcDebugPrint (SDBG_MAX, "Silicon capability does not support persistent modes, forcing to non-persistent mode.\n");
      Host->var.mem.dfxMemVars.dfxPerMemMode = NON_PER_MEM_MODE;
      MemMapHost->PcatCapability.MemModeCapabilities &= ~MODE_AD;
    }
  }

  SecurityPolicy = GetSecurityPolicy ();
  ASSERT (SecurityPolicy != NULL);

  //
  // Disable persistent memory if TME is enabled
  //
  if (SecurityPolicy != NULL && SecurityPolicy->EnableTme != SECURITY_POLICY_DISABLE
      && SecurityPolicy->DfxEnableTmePOC == SECURITY_POLICY_DFX_DISABLE) {
    RcDebugPrint (SDBG_MAX, "TME is enabled, disabling Crystal Ridge feature.\n");
    Host->var.mem.volMemMode = VOL_MEM_MODE_1LM;
    Host->var.mem.dfxMemVars.dfxPerMemMode = NON_PER_MEM_MODE;
    OutputWarning (WARN_SILICON_CAPABILITY_LIMITATION, WARN_TME_ENABLED_CRYSTAL_RIDGE_NOT_SUPPORTED, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK);
  }

  //
  // Disable Crystal Ridge feature if SGX is enabled
  //
  if (SecurityPolicy != NULL && SecurityPolicy->EnableSgx == SECURITY_POLICY_ENABLE) {
    RcDebugPrint (SDBG_MAX, "SGX is enabled, disabling Crystal Ridge feature.\n");
    Host->var.mem.volMemMode = VOL_MEM_MODE_1LM;
    Host->var.mem.dfxMemVars.dfxPerMemMode = NON_PER_MEM_MODE;
    OutputWarning (WARN_SILICON_CAPABILITY_LIMITATION, WARN_SGX_ENABLED_CRYSTAL_RIDGE_NOT_SUPPORTED, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK);
  }

  //
  // ICX does not support 256B 4 way NM interleave for 2LM configs such as
  // 8+8 in SNC2/Hemi,8+4 in SNC2/Hemi and 4+4 in All2All.
  // Hence,enforcing 4KB only for ICX.
  //
  Host->var.mem.dfxMemVars.DfxDdrtChInterleaveGran = MEM_INT_GRAN_CH_4KB_TGT_4KB;
  MemMapData->DdrtIntlvGranularity = INTERLEAVE_GRANULARITY_4KB;

  //
  // Wave1/Wave2 uses RT2LM for the 4K in-tile 2LM
  //
  if (Host->var.mem.volMemMode == VOL_MEM_MODE_2LM) {
    MemMapData->Is2LmUsingRt2lm = TRUE;
  }

  MemMapHost->DdrtIntlvGranularity = MemMapData->DdrtIntlvGranularity;

  //
  // Init. max local and remote DRAM rules count
  //
  MemMapData->MaxSadRules = SAD_RULES_10NM;
  MemMapData->MaxRemoteSadRules = MAX_REMOTE_SAD_RULES;

  if (Host->var.mem.PmemCaching == PMEM_WBCACHE_ENABLED) {
    MemMapData->PmemCaching = LegacyPmCaching;
  }

  //
  // Enable XOR decoder by default for power of two interleaving.
  //
  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {
    if (!CheckSocketEnabled (Sckt)) {
      continue;
    }
    MemMapHost->SocketInfo[Sckt].XorMode = CHANNEL_XOR | CLUSTER_XOR;

    if (Setup->mem.dfxMemSetup.DfxBankXorEn != 0) {
      MemMapHost->SocketInfo[Sckt].XorMode |= BANK_XOR;
    } else {
      MemMapHost->SocketInfo[Sckt].XorMode &= ~BANK_XOR;
    }
  }

  MemMapData->UmaBasedClustersPerSocket = GetKtiOutputUmaClusterSetting ();

  if (IfFeatureEnabled (TypeSnc)) {
    MemMapData->UmaBasedClustersPerSocket = UMABASEDCLUSTERING_DISABLED;
  }

  if (MemMapData->UmaBasedClustersPerSocket > UMABASEDCLUSTERING_HEMISPHERE) {
    MemMapData->UmaBasedClustersPerSocket = UMABASEDCLUSTERING_HEMISPHERE;
  }

  MemMapHost->MaxSadRules = MemMapData->MaxSadRules;
}

/** Performs CPU specific functions to check memory address map setup options

  Performs 10nm wave1/wave2 CPU specific setup option check besides the common part
  implemented in CheckMemAddrMapSetupOptionsValues().

  @param[in] Host       - Pointers to global Host data structure.
  @param[in] MemMapData - The private data structure for memory map.

  @retval    None

**/
VOID
CheckMemAddrMapSetupOptionsValuesEx (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData
  )
{
  UINT8             Index;
#ifdef DDRT_SUPPORT
  UINT8             Dimm;
#endif // DDRT_SUPPORT
  UINT8             TempCh;
  UINT8             MaxImc;
  UINT8             TempSckt;
  SYS_SETUP         *Setup;
  UINT8             CacheMemType;

  Setup = GetSysSetupPointer ();

  if (Setup->mem.dfxMemSetup.dfxOptions & HIGH_ADDR_EN)  {
    if ((Setup->mem.dfxMemSetup.dfxHighAddrBitStart >= HIGH_ADDR_START_BIT_POSITION_33) &&
        (Setup->mem.dfxMemSetup.dfxHighAddrBitStart <= HIGH_ADDR_START_BIT_POSITION_51)) {
      Host->var.mem.dfxMemVars.dfxHighAddrBitStart = Setup->mem.dfxMemSetup.dfxHighAddrBitStart;
    } else {
      RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "\nWARNING!!! Invalid option value for HighAddressStartBitPosition.\n");
      Setup->mem.dfxMemSetup.dfxOptions &= ~HIGH_ADDR_EN;
      Host->var.mem.dfxMemVars.dfxHighAddrBitStart = 0;
    }
  }

  TempCh = 0;
  TempSckt = 0;
  MaxImc = GetMaxImc ();
  if (UbiosGenerationOrHsleEnabled ()) {
#ifdef DDRT_SUPPORT
    if (!CheckDdrtParitioningSpdBytesDdrt (Host, &TempSckt, &TempCh, &Dimm)) {
      RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "SPD PMemM Dimm partitioning bytes mismatch between PMEM DIMMS in the system.\n");
      FatalError (ERR_NGN, NGN_BYTES_MISMATCH);
    }
#endif // DDRT_SUPPORT
    for (Index = 0; Index < MAX_SOCKET * MaxImc; Index++) {
      Host->var.mem.dfxMemVars.dfxPartitionRatio[Index] = Setup->mem.dfxMemSetup.dfxPartitionRatio[Index];
      RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "\nOverride partitionRatio[%02X] = %02X\n", Index, Host->var.mem.dfxMemVars.dfxPartitionRatio[Index]);
    }
  }

  if (((MaxImc > 2) && (Setup->mem.memInterleaveGran1LM == MEM_INT_GRAN_1LM_4KB_256B_256B)) ||
      (Setup->mem.memInterleaveGran1LM == MEM_INT_GRAN_1LM_256B_256B)) {
    Host->var.mem.memInterleaveGran1LM = Setup->mem.memInterleaveGran1LM;
  } else {
    RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "\nWARNING!!! Invalid option value for 1LM Memory Interleave Granularity.\n");
    Host->var.mem.memInterleaveGran1LM = MEM_INT_GRAN_1LM_DEFAULT;
  }

  if (Setup->mem.dfxMemSetup.dfxRankInter == RANK_1WAY || Setup->mem.dfxMemSetup.dfxRankInter == RANK_2WAY ||
    Setup->mem.dfxMemSetup.dfxRankInter == RANK_4WAY || Setup->mem.dfxMemSetup.dfxRankInter == RANK_8WAY) {
    Host->var.mem.rankInter = Setup->mem.dfxMemSetup.dfxRankInter;
  } else {
    RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "\nWARNING!!! Invalid option value for Rank Interleave.Setting to Maximum Rank Interleave.\n");
    Host->var.mem.rankInter = RANK_8WAY;
  }

  CacheMemType = Setup->mem.CacheMemType;
  if (CacheMemType != CACHE_TYPE_DDR_CACHE_PMEM) {
    RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
       "\nWARNING!!! Invalid option value for Cache Memory Type for this SKU. Setting to DDR.\n");
    Host->var.mem.CacheMemType = CACHE_TYPE_DDR_CACHE_PMEM;

  }
}


/**
  Determine if the CPU cluster is based on north-south or west-east.

  This function depends on CpuTypeLib to get the cpu type and subtype
  and determine the cluster type.
    1. For SPR HCC        It is north-south.
    2. For SPR XCC & LCC  It is west-east.
    3. For ICX CPU        It is always west-east.

  @retval TRUE      CPU cluster is based on north-south.
  @retval FALSE     CPU cluster is based on east-west.

**/
BOOLEAN
IsNorthSouthCluster (
  VOID
  )
{
  //
  // It is always west-east cluster for ICX.
  //
  return FALSE;
}


/**
  Get near memory patrol sparing range.

  @param[in]  Host        Pointer to the sysHost structure.
  @param[in]  MemMapData  Pointer to memory map private data structure.
  @param[in]  SocketId    Socket number.
  @param[in]  McIndex     Memory controller index.
  @param[in]  SadIndex    Sad table index
  @param[out] PtrPsBase   Pointer to output UINT32 buffer for patrol sparing range.
  @param[out] PtrPsLimit  Pointer to output UINT32 buffer for patrol sparing limit.

**/
VOID
GetNmPsRange (
  IN     SYSHOST          *Host,
  IN     MEMORY_MAP_DATA  *MemMapData,
  IN     UINT8            SocketId,
  IN     UINT8            McIndex,
  IN     UINT8            SadIndex,
     OUT UINT32           *PtrPsBase,
     OUT UINT32           *PtrPsLimit
  )
{
  UINT32                  NmSize;
  UINT32                  PsBase;
  UINT32                  PsLimit;
  SAD_TABLE               *SadEntry;

  //
  // There should be 1 DDR4 TAD programmed to support patrol/sparing in 2LM (DDR4 cache DDRT).
  // The TAD base and limit should cover only NM size (where the index of the
  // TAD base should be 0) and the TAD range should be above MMIO hole.
  // This TAD will not be used by address decoder to decode the system address
  // but will be used to generate system address for patrol and sparing.
  // The TAD base must be aligned with the NmCachingOffset, which has a 32GB granularity
  //
  SadEntry = &Host->var.mem.socket[SocketId].SAD[SadIndex];
  NmSize = SadEntry->ways * SadEntry->NmChWays * FindMinNmSize (Host, MemMapData, SocketId, McIndex);
  PsBase = (SadEntry->Base & 0xfffffe00) + NmSize;
  PsLimit = PsBase + NmSize;
  *PtrPsBase = PsBase;
  *PtrPsLimit = PsLimit;
}

/**
  Return whether memory decoder must program near memory dram rule for the input SAD ID.
  NM_DRAM_RULE is programmed only for SA based patrol/sparing and is not used for CA based patrol/sparing.
  10nm wave1/wave2 can support only SA based patrol/sparing for all DDR memory modes.

  @param[in]  Host        Pointer to the sysHost structure.
  @param[in]  SocketId    Socket number.
  @param[in]  McIndex     Memory controller index.
  @param[in]  SadIndex    Sad table index.

  @retval TRUE            Need to program near memory dram rule.
  @retval FALSE           No need to program near memory dram rule.

**/
BOOLEAN
IsSystemAddressBasedPatrolSparing (
  IN SYSHOST         *Host,
  IN UINT8           SocketId,
  IN UINT8           McIndex,
  IN UINT8           SadIndex
  )
{
  SAD_TABLE         *SadEntry;

  SadEntry = &Host->var.mem.socket[SocketId].SAD[SadIndex];
  if (((SadEntry->imcInterBitmap & (BIT0 << McIndex)) == 0)) {
    return FALSE;
  }
  if ((SadEntry->type != MemType1lmDdr) && !IsMemType2lm (SadEntry->type)) {
    return FALSE;
  }
  return TRUE;
}

/**
  Check if 2-way NM$ is supported.

  This function checks if the biased 2-way near memory cache is supported on system.
  The result will be stored in MemMapHost for future consumption.

**/
VOID
CheckTwoWayNmCacheSupported (
  VOID
  )
{
  //
  // Stub function as 2-way NM$ is not supported on 10nm wave 1/2.
  //
  return;
}

#ifdef DDRT_SUPPORT
/**

  This function adds SoC-specific Intel PMem performance knobs into the performance knob list.

  @param[in]     Host           Pointer to the sysHost structure.
  @param[in]     SocketId       Socket index.
  @param[in]     ChId           Channel index within socket.
  @param[in]     DimmId         DIMM index within channel.
  @param[out]    SetPerfKnob    The list of performance knobs to be set on PMem module.
  @param[in,out] PerfKnobNum    The number of performance knobs to be set on PMem module.

**/
VOID
AddPmemPerfKnobs(
  IN     SYSHOST             *Host,
  IN     UINT8               SocketId,
  IN     UINT8               ChId,
  IN     UINT8               DimmId,
     OUT PERFORMANCE_KNOB    SetPerfKnob[INTEL_PMEM_PERF_KNOBS_CNT],
  IN OUT UINT8               *PerfKnobNum
  )
{
  DIMM_NVRAM_STRUCT          (*DimmNvList)[MAX_DIMM];

  if ((*PerfKnobNum) >= INTEL_PMEM_PERF_KNOBS_CNT) {
    RcDebugPrint (SDBG_MINMAX, "Cannot add more knobs into the performance knob list\n");
    return;
  }

  DimmNvList = GetDimmNvList (Host, SocketId, ChId);

  //
  // Check if FW performance knob 6+ (FIS perf knob 7+) is supported.
  //
  if (((*DimmNvList)[DimmId].firmwareRevision.majorVersion < PERF_KNOB7_FW_MAJOR_REVISION) &&
      ((*DimmNvList)[DimmId].firmwareRevision.minorVersion < PERF_KNOB7_FW_MINOR_REVISION) &&
      ((*DimmNvList)[DimmId].firmwareRevision.buildVersion < PERF_KNOB7_FW_REVISION)) {
    RcDebugPrintWithDevice (SDBG_MINMAX, SocketId, ChId, DimmId, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "FW performance knob 6+ is not supported in FIS:%x\n",
      Host->var.mem.socket[SocketId].channelList[ChId].FisVersion[DimmId]
      );
    RcDebugPrintWithDevice (SDBG_MAX, SocketId, ChId, DimmId, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "%02X %02X %02X %04X\n",
      (*DimmNvList)[DimmId].firmwareRevision.majorVersion,
      (*DimmNvList)[DimmId].firmwareRevision.minorVersion,
      (*DimmNvList)[DimmId].firmwareRevision.hotfixVersion,
      (*DimmNvList)[DimmId].firmwareRevision.buildVersion
      );
    return;
  }

  //
  // Set performance knob 7 on Intel DCPMM.
  //
  SetPerfKnob[*PerfKnobNum].PerfKnobId = PERF_KNOB7_ID;
  SetPerfKnob[*PerfKnobNum].PerfKnobValue = PERF_KNOB7_VALUE;
  (*PerfKnobNum)++;
}

/**
  This function checks if fADR is supported on system.

  @param[in] Host       Pointer to sysHost.

  @retval N/A
**/
VOID
CheckFadrSupported (
  IN SYSHOST            *Host
  )
{
  //
  // Stub function as fADR is not supported on 10nm wave 1/2.
  //
  return;
}

/**
  This programs CSRs related to MKTME

  @param VOID

  @retval N/A

**/
VOID
WriteMktmeCsrs (
  VOID
  )
{
  UINT8                ScktId;
  UINT8                McId;
  UINT8                ChId;
  UINT8                MaxImc;
  UINT8                NumChPerMc;
  SECURITY_POLICY      *SecurityPolicy;

  SecurityPolicy = GetSecurityPolicy ();
  ASSERT (SecurityPolicy != NULL);
  MaxImc = GetMaxImc ();
  NumChPerMc = GetNumChannelPerMc ();

  //
  // Only execute if TME/MKTME is enabled
  //
  if ((SecurityPolicy != NULL) &&
      (SecurityPolicy->EnableTme == SECURITY_POLICY_DISABLE) &&
      (SecurityPolicy->EnableMktme == SECURITY_POLICY_DISABLE)) {
    return;
  }

  //
  // S1909300079:
  // BIOS must set sbmcactl.nmfillwrcamerrdis=0x1 when
  // MKTME is enabled and a channel has DDRT populated.
  //
  if (IsSiliconWorkaroundEnabled ("S1909300079")) {
    for (ScktId = 0; ScktId < MAX_SOCKET; ScktId++) {
      if (CheckSocketEnabled (ScktId) == 0) {
        continue;
      }
      for (McId = 0; McId < MaxImc; McId++) {
        if (!CheckSocketMcEnabled (ScktId, McId)) {
          continue;
        }
        for (ChId = 0; ChId < NumChPerMc; ChId++) {
          if (!AepDimmPresent (ScktId, ChId + (McId * NumChPerMc), NULL)) {
            continue;
          }

          //
          // Set sbmcactl.nmfillwrcamerrdis=0x1 on this channel
          //
          McDisableNmFillWrCamErrViralTriggering (ScktId, McId, ChId);

        }  // Ch loop
      }  // Mc loop
    } // Sckt Loop
  }
  return;
}
#endif // DDRT_SUPPORT

/**
  Fill in 10nm wave X specific CSRs for memory map from config info in Host structure besides
  the common part implemented in WriteMemoryMapCsrs().

  @param[in] Host         Pointer to sysHost
  @param[in] MemMapData   Pointer to the memory map private data structure

  @retval N/A

**/
VOID
WriteMemoryMapCsrsEx (
  IN PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData
  )
{
#ifdef DDRT_SUPPORT
  WriteMktmeCsrs ();
#endif // DDRT_SUPPORT
}

#ifdef DDRT_SUPPORT
/** Calculate the far memory controller target.

  The function calculates the far memory controller target for the input memory controller for a
  SAD entry.
  If the input memory controller is not involved in 2LM mode or In-Tile cache, itself is returned.
  If the input memory controller is involved in 2LM and it is X-tile cache mode, it returns the
  corresponding far memory controller ID.

  @param[in]  MemMapData     - Pointer to the memory map private data structure.
  @param[in]  SocketId       - The socket index.
  @param[in]  McId           - The MC index.

  @return The far memory target ID in a socket.

**/
UINT8
GetFmTarget (
  IN  MEMORY_MAP_DATA    *MemMapData,
  IN  UINT8              SocketId,
  IN  UINT8              McId
  )
{
  return McId;
}

/**
  This routine checks the POR configuration for X-tile 2LM for mod3

  @param[in] McBitmap         Far memory memory controller bit map.
  @param[in] ChannelBitmap    Far memory channel bit map.
  @param[in] NmMcBitmap       Near memory memory controller bit map.
  @param[in] NmChannelBitmap  Near memory channel bit map.

  @retval FALSE        The memory configuration is not mod3 X-tile 2LM configuration.

**/
BOOLEAN
CheckMod3XTile2lmPorConfig (
  IN UINT8    McBitmap,
  IN UINT8    ChannelBitmap[MAX_IMC],
  IN UINT8    NmMcBitmap,
  IN UINT8    NmChannelBitmap[MAX_IMC]
  )
{
  return FALSE;
}

/**
  This routine checks the POR configurations for X-tile 2LM

  @param[in] Host         Pointer to host pointer.
  @param[in] MemMapData   Pointer to the memory map private data structure.
  @param[in] SocketId     Id for the socket to check.

  @retval FALSE        The memory configuration is not POR X-tile 2LM configuration.

**/
BOOLEAN
CheckXTile2lmPorConfig (
  IN SYSHOST          *Host,
  IN MEMORY_MAP_DATA  *MemMapData,
  IN UINT8            SocketId
  )
{
  return FALSE;
}

/**
  This routine does pre-calculation of far memory partition in XTile 2LM in SNC mode.
  The related near memory bitmap for each cluster and far memory partition info is saved in
  memory map private data structure in future interleave.

  @param[in]      Host         Pointer to host pointer.
  @param[in,out]  MemMapData   Pointer to memory map private data structure.
  @param[in]      SocketId     Id for the socket to check.

**/
VOID
CalculateXTile2lmSncFarMemoryPartition (
  IN     SYSHOST          *Host,
  IN OUT MEMORY_MAP_DATA  *MemMapData,
  IN     UINT8            SocketId
  )
{
  return;
}

/**

  This routine supports any SOC-specific restrictions on DCPMM AppDirect interleaving.

  @param [in] MemMapData - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param [in] Socket     - Socket number
  @param [in] Channel    - Channel number
  @param [in] Dimm       - Dimm number
  @param [in] Interleave - Interleave request id
  @param [in] RecordType - Type of the record validated CFG_CUR/CFG_IN

  @retval N/A

**/
VOID
CountAdx1DcpmmInterleave (
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           Socket,
  IN UINT8           Channel,
  IN UINT8           Dimm,
  IN UINT8           Interleave,
  IN UINT8           RecordType
  )
{
  DDRT_DIMM_DEVICE_INFO_STRUCT *DdrtDimmInfo;
  UINT8                        NumOfDimmsInInterleaveSet;
  UINT8                        BitShift;
  UINT8                        AppDirectByOneCount = 0;
  UINT8                        MaxDdrCh;

  if ((NULL == MemMapData) || (Socket >= MAX_SOCKET) || (Channel >= MAX_CH) ||
      (Dimm >= MAX_DIMM) || (Interleave >= MAX_UNIQUE_NGN_DIMM_INTERLEAVE)) {
    return;
  }

  MemMapData->MemMapSocket[Socket].UseRT0forAdx1 = FALSE;

  DdrtDimmInfo = GetDdrtDimmInfo (Socket, Channel, Dimm);
  MaxDdrCh = GetMaxChDdr ();

  if (RecordType == CFG_IN) {
    NumOfDimmsInInterleaveSet = DdrtDimmInfo->NgnCfgReq.interleave[Interleave].NumOfDimmsInInterleaveSet;
  } else {
    NumOfDimmsInInterleaveSet = DdrtDimmInfo->NgnCfgCur.interleave[Interleave].NumOfDimmsInInterleaveSet;
  }
  if (NumOfDimmsInInterleaveSet == 1) {
    for (BitShift = 0; BitShift < MaxDdrCh; BitShift++) {
      if (BitShift != Channel) {
        if ((MemMapData->MemMapSocket[Socket].AppDirectByOneChannelBitmap >> BitShift) & BIT0) {
          AppDirectByOneCount++;
        }
      }
    }
    if (AppDirectByOneCount >= 6) {
      MemMapData->MemMapSocket[Socket].UseRT0forAdx1 = TRUE;
    }
  }
}

/**

  This routine supports any SOC-specific restrictions on DCPMM AppDirect DFX interleaving.

  @param[in] Host         - Pointer to sysHost
  @param[in] MemMapData   - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval N/A

**/
VOID
CountAdx1DcpmmInterleaveDfxSetupOption (
  IN PSYSHOST         Host,
  IN MEMORY_MAP_DATA  *MemMapData
  )
{
  UINT8                  BitShift;
  UINT8                  AppDirectByOneCount = 0;
  UINT8                  MaxDdrCh;
  UINT8                  Channel;
  UINT8                  Socket;

  if ((NULL == Host) || (NULL == MemMapData)) {
    return;
  }

  MaxDdrCh = GetMaxChDdr ();

  if (Host->var.mem.imcInter == IMC_1WAY && Host->var.mem.chInter == CH_1WAY) {
    //
    // For each socket
    //
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if (!CheckSocketEnabled (Socket)) {
        continue;
      }
      //
      // Set initial value for each socket
      //
      MemMapData->MemMapSocket[Socket].UseRT0forAdx1 = FALSE;
      //
      // For each channel
      //
      for (Channel = 0; Channel < MaxDdrCh; Channel++) {
        //
        // Mark ADx1 presence on the channel
        //
        MemMapData->MemMapSocket[Socket].AppDirectByOneChannelBitmap |= (BIT0 << Channel);
        //
        // Count number of ADx1 regions per socket
        //
        for (BitShift = 0; BitShift < MaxDdrCh; BitShift++) {
          if (BitShift != Channel) {
            if ((MemMapData->MemMapSocket[Socket].AppDirectByOneChannelBitmap >> BitShift) & BIT0) {
              AppDirectByOneCount++;
            }
          }
        }
        if (AppDirectByOneCount >= 6) {
          MemMapData->MemMapSocket[Socket].UseRT0forAdx1 = TRUE;
        }
      }
    }
  }
}

/**
  Check if the input value is aligned to the input boundary.

  @param[in] Boundary    The address boundary used to check for alignment. Must be a power of 2.
  @param[in] Address     The Address to check for alignment.

  @retval TRUE  The input value is aligned.
  @retval FALSE The input value is not aligned.
**/
BOOLEAN
IsAddressAligned (
  IN UINT32 Boundary,
  IN UINT32 Address
  )
{
  return ((Address & (Boundary - 1)) == 0);
}

/**
  Check if the input SAD rule is preceded by a 2LM rule.

  @param[in] MemMapData       Pointer to the memory map private data structure.
  @param[in] Sckt             The socket index that the input SadIndex belongs to.
  @param[in] SadIndex         The SadIndex of the SAD rule to check.

  @retval TRUE                The previous SAD rule is a 2LM type.
  @retval FALSE               The previos SAD rule is a not a 2LM type.
**/
BOOLEAN
IsPreviousSadType2lm (
  IN MEMORY_MAP_DATA    *MemMapData,
  IN UINT8              Sckt,
  IN UINT8              SadIndex
  )
{
  UINT8     SocketLoopIndex;
  UINT8     SadLoopIndex;
  SAD_TABLE *SadEntry;
  SAD_TABLE *SadEntryLoopInstance;

  //
  // There are no SAD rules that precede Socket 0, SAD Rule 0
  //
  if (Sckt == 0 && SadIndex == 0) {
    return FALSE;
  }

  SadEntry = GetSadTableEntry (Sckt, SadIndex);
  for (SocketLoopIndex = 0; SocketLoopIndex < MAX_SOCKET; SocketLoopIndex++) {
    //
    // Check if socket is valid
    //
    if (!CheckSocketEnabled (SocketLoopIndex)) {
      continue;
    }
    for (SadLoopIndex = 0; SadLoopIndex < MemMapData->MaxSadRules; SadLoopIndex++) {
      SadEntryLoopInstance = GetSadTableEntry (SocketLoopIndex, SadLoopIndex);
      if (SadEntryLoopInstance->Enable == 0) {
        //
        // A disabled SAD entry indicates that there are no more SAD entries in this SocketLoopIndex
        //
        break;
      }
      if (SadEntryLoopInstance->Limit == SadEntry->Base) {
        return IsMemType2lm(SadEntryLoopInstance->type);
      }
    }
  }

  return FALSE;
}

/**

  Get the number of SAD rules present in the input SAD table.

  @param[in, out]  MemMapData        Pointer to the memory map private data structure.
  @param[in]  SadTable    The SAD table to count.

  @return The number of enabled entries in the input SAD table.

**/
UINT8
CountSadRules (
  IN MEMORY_MAP_DATA *MemMapData,
  IN SAD_TABLE       (*SadTable)[MAX_DRAM_CLUSTERS * MAX_SAD_RULES]
  )
{
  UINT8 SadIndex;
  UINT8 Count;

  Count = 0;

  for (SadIndex = 0; SadIndex < MemMapData->MaxSadRules; SadIndex++) {
    //
    // Stop counting when the first disabled SAD rule is found
    //
    if ((*SadTable)[SadIndex].Enable == 0) {
      break;
    }
    Count++;
  }
  return Count;
}

/**

  Insert an NXM in the input SadTable at the specified SadIndex. When this function
  returns the SAD table will be updated such that an NXM rule is present at the input
  SadIndex and any rules previously present at SadIndex or above will be shifted up in the
  address map.

  This function assumes that the caller has already verified that the maxiumum number
  of DRAM rules for the current platform type will not be exceeded after the additional
  of the NXM rule.

  @param[in, out]  Host              Pointer to sysHost.
  @param[in, out]  MemMapData        Pointer to the memory map private data structure.
  @param[in]       Socket            Socket index for the input SadIndex.
  @param[in]       SadIndex          SAD index where the NXM will be inserted.
  @param[in]       Size              The size of the NXM to be inserted in 64MB units.

  @retval EFI_SUCCESS                The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER      The function failed due to some illegal input.
  @retval EFI_OUT_OF_RESOURCES       There are not enough entries in the SAD Table to insert the NXM.

**/
EFI_STATUS
EFIAPI
InsertNxmRule (
  IN OUT SYSHOST         *Host,
  IN OUT MEMORY_MAP_DATA *MemMapData,
  IN     UINT8           Socket,
  IN     UINT8           SadIndex,
  IN     UINT32          Size
  )
{
  MEMORY_MAP_HOST *MemMapHost;
  UINT8           NewSadTableCount;
  UINT32          MaxSadLimit;
  UINT32          NxmBaseAddress;
  UINT32          NxmLimit;
  INT8            CurrentSad;
  UINT8           SocketIndex;
  RSAD_TABLE      (*RemoteSadTable)[MAX_REMOTE_SAD_RULES];
  SAD_TABLE       (*SadTable)[MAX_DRAM_CLUSTERS * MAX_SAD_RULES];
  UINT8           (*SadIntList)[MAX_DRAM_CLUSTERS * MAX_SAD_RULES][MC_MAX_NODE];

  MemMapHost = GetMemMapHostPointer ();
  SadTable = &Host->var.mem.socket[Socket].SAD;
  SadIntList = &MemMapHost->SocketInfo[Socket].SadIntList;

  //
  // Check the inputs
  //
  if (SadIndex >= MemMapData->MaxSadRules) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check that the new SAD Table count is still within bounds
  //
  NewSadTableCount = CountSadRules (MemMapData, SadTable) + 1;
  if (NewSadTableCount > MemMapData->MaxSadRules) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Determine whether we have room to map the NXM rule
  //
  if (MemMapHost->BlockDecoderData.BlockSocketEnable == 0) {
    MaxSadLimit = Host->var.mem.mmiohBase;
  } else {
    MaxSadLimit = MemMapHost->BlockDecoderData.BlockDecoderBase;
  }
  if ((GetSadLimit (Host, MemMapData) + Size) > MaxSadLimit) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Shift the SAD rules up by one index to make room for the NXM rule.
  //
  for (CurrentSad = (MemMapData->MaxSadRules - 2); CurrentSad >= SadIndex; CurrentSad--) {
    //
    // Skip rules that are disabled
    //
    if ((*SadTable)[CurrentSad].Enable == 0) {
      continue;
    }

    //
    // Copy the SAD rules to their new index locations
    //
    CopyMem (&(*SadTable)[CurrentSad + 1], &(*SadTable)[CurrentSad], sizeof ((*SadTable)[CurrentSad]));
    CopyMem (&(*SadIntList)[CurrentSad + 1], &(*SadIntList)[CurrentSad], sizeof ((*SadIntList)[CurrentSad]));

  } // SAD Loop

  //
  // Create the new NXM rule
  //
  NxmBaseAddress = (*SadTable)[SadIndex].Base;
  NxmLimit = NxmBaseAddress + Size;
  ZeroMem (
    (UINT8 *) &(*SadTable)[SadIndex],
    sizeof ((*SadTable)[SadIndex])
    );
  ZeroMem (
    (UINT8 *) &(*SadIntList)[SadIndex],
    sizeof ((*SadIntList)[SadIndex])
    );
  (*SadTable)[SadIndex].Enable = 1;
  (*SadTable)[SadIndex].type = MemTypeNxm;
  (*SadTable)[SadIndex].local = 0;
  (*SadTable)[SadIndex].Base = NxmBaseAddress;
  (*SadTable)[SadIndex].Limit = NxmLimit;

  //
  // Adjust SAD and Remote SAD rules
  //
  for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++) {
    //
    // Check if socket is valid
    //
    if (!CheckSocketEnabled (SocketIndex)) {
      continue;
    }
    SadTable = &Host->var.mem.socket[SocketIndex].SAD;
    //
    // Adjust the SAD rules for every socket
    //
    for (CurrentSad = 0; CurrentSad < MemMapData->MaxSadRules; CurrentSad++) {
      if (!(*SadTable)[CurrentSad].Enable) {
        break;
      }

      //
      // Don't modify the NMX rule we just created.
      //
      if (SocketIndex == Socket && CurrentSad == SadIndex) {
        continue;
      }

      //
      // Move up all of the SAD rules that are now above the new NXM rule
      //
      if ((*SadTable)[CurrentSad].Base >= NxmBaseAddress) {
        (*SadTable)[CurrentSad].Base += Size;
        (*SadTable)[CurrentSad].Limit += Size;
      }

    } // SAD Loop


    RemoteSadTable = &MemMapData->MemMapSocket[SocketIndex].RSad;
    //
    // Adjust remote SAD rules for every socket, including the one where the NXM
    // was inserted
    //
    for (CurrentSad = 0; CurrentSad < MemMapData->MaxRemoteSadRules; CurrentSad++) {
      if (!(*RemoteSadTable)[CurrentSad].Enable) {
        break;
      }
      //
      // If the RSAD sits above the new NXM rule, increment the base address to
      // move the RSAD rule without changing the size.
      //
      if ((*RemoteSadTable)[CurrentSad].Base >= NxmBaseAddress) {
        (*RemoteSadTable)[CurrentSad].Base += Size;
      }

      //
      // Increase the limit of the RSADs that have a limit above the NXM Base.
      // This will result in a size increase for the RSAD where the NXM was
      // inserted. Additional RSADs higher in the address space will be shifted
      // up without a change in size since their base address will also be
      // incremented.
      //
      if ((*RemoteSadTable)[CurrentSad].Limit >= NxmBaseAddress) {
        (*RemoteSadTable)[CurrentSad].Limit += Size;
      }
    } // Remote SAD Loop
  } // Socket Loop

  return EFI_SUCCESS;
}

/**

  Apply the Silicon Workaround S2206200728

  @param[in, out]  Host              Pointer to sysHost.
  @param[in, out]  MemMapData        Pointer to the memory map private data structure.

  @retval EFI_SUCCESS                The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER      The function failed due to some illegal input.
  @retval EFI_OUT_OF_RESOURCES       There are not enough SAD Rules to implement the workaround.

**/
EFI_STATUS
EFIAPI
ApplyWaS2206200728 (
  IN OUT SYSHOST          *Host,
  IN OUT MEMORY_MAP_DATA  *MemMapData
  )
{
  MEMORY_MAP_HOST               *MemMapHost;
  UINT8                         Socket;
  UINT8                         SadIndex;
  SAD_TABLE                     (*SadTable)[MAX_DRAM_CLUSTERS * MAX_SAD_RULES];
  EFI_STATUS                    Status;
  UINT8                         SadCount[MAX_SOCKET];
  UINT32                        Size;

  MemMapHost = GetMemMapHostPointer ();

  //
  // This WA only applies when mixed 1LM + 2LM is enabled
  //
  if (Host->var.mem.volMemMode != VOL_MEM_MODE_MIX_1LM2LM) {
    return EFI_SUCCESS;
  }

  //
  // First count the SAD rules currently in use so that we can indentify when we run out
  // of available SAD rules.
  //
  ZeroMem (SadCount, sizeof (SadCount));
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    SadCount[Socket] = CountSadRules (MemMapData, &Host->var.mem.socket[Socket].SAD);
  }

  //
  // Find unaligned 2LM DRAM rules and insert an NXM rule to align the base to 32GB
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    SadTable = &Host->var.mem.socket[Socket].SAD;
    for (SadIndex = 0; SadIndex < MemMapData->MaxSadRules; SadIndex++) {
      //
      // Move to the next socket when all SAD rules in this socket are processed
      //
      if ((*SadTable)[SadIndex].Enable == 0) {
        break;
      }

      //
      // The first SAD rule does not need to be adjusted
      //
      if ((*SadTable)[SadIndex].Base == 0) {
        continue;
      }

      //
      // Only 2LM SAD rules need to be adjusted
      //
      if (!IsMemType2lm((*SadTable)[SadIndex].type)) {
        continue;
      }

      //
      // Only adjust if the 2LM SAD base is not 32GB Aligned
      //
      if (IsAddressAligned (MEM_32GB_IN_64MB_UNIT, (*SadTable)[SadIndex].Base)) {
        continue;
      }

      //
      // With mixed 1LM+2LM, it is still possible to have no 1LM rules present. This
      // condition occurs when the total DDR4 channel size is equal to the requested
      // DdrCacheSize. If this is the case, then there is no need to insert the NXM rule
      // between contiguous 2LM regions.
      //
      if (IsPreviousSadType2lm (MemMapData, Socket, SadIndex)) {
        continue;
      }

      //
      // At this point we will need to insert an NXM rule to align the 2LM SAD Base.
      // Increment the SAD rule counter and make sure we have not run out of SAD rules.
      //
      SadCount[Socket]++;
      if (SadCount[Socket] >= MemMapData->MaxSadRules) {
        return EFI_OUT_OF_RESOURCES;
      }

      //
      // Calculate the size of the NXM rule to be inserted
      //
      Size = ALIGN_VALUE ((*SadTable)[SadIndex].Base, MEM_32GB_IN_64MB_UNIT) - (*SadTable)[SadIndex].Base;

      //
      // Insert the NXM Rule
      //
      Status = InsertNxmRule (
                 Host,
                 MemMapData,
                 Socket,
                 SadIndex,
                 Size
                 );
      if (EFI_ERROR (Status)) {
        return Status;
      }
    }
  } // Socket loop

  return EFI_SUCCESS;
}
#endif // DDRT_SUPPORT

/**
  This function writes Mesh2Mem and MC CSRs required to DISABLE critical chunk support.

  @param[in] SocketId    Socket Index.

  @retval N/A
**/
VOID
WriteCriticalChunkCsrs (
  IN UINT8      SocketId
  )
{
  UINT8           ChId;
  UINT8           McId;
  UINT8           MaxImc;
  UINT8           NumChPerMc;
  BOOLEAN         IsSgxEnabled;
  SECURITY_POLICY *SecurityPolicy = NULL;

  MaxImc = GetMaxImc ();
  SecurityPolicy = GetSecurityPolicy ();
  ASSERT (SecurityPolicy != NULL);
  NumChPerMc = GetNumChannelPerMc ();

  IsSgxEnabled = FALSE;
  if ((SecurityPolicy != NULL) && (SecurityPolicy->EnableSgx != SECURITY_POLICY_DISABLE)) {
    IsSgxEnabled = TRUE;
  }

  for (McId = 0; McId < MaxImc; McId++) {
    if (!CheckSocketMcEnabled (SocketId, McId)) {
      continue;
    }

    //
    // Set DiscritChunk to disable critical chunk when NmCaching is enabled on this MC or SGX is enabled.
    //
    if (IsSgxEnabled || IsMemTypePresentMcFunction (SocketId, McId, IsMemType2lm)) {
      SetM2mDefeatures0DiscritChunk (SocketId, McId, TRUE);
    }

    if (IsSgxEnabled) {
      //
      // Wait4BothHalves is only required to be set when SGX is enabled for ICX.
      //
      SetM2mDefeatures0Wait4BothHalves (SocketId, McId, TRUE);
      for (ChId = 0; ChId < NumChPerMc; ChId++) {
        if (!IsChannelEnabled (SocketId, ChId + (McId * NumChPerMc))) {
          continue;
        }

        //
        // The copy of wait4bothhalves in mcchan_amap should be set to match the one in m2m_defeatures0 for ICX.
        //
        McSetWaitForBothHalves (SocketId, McId, ChId);
      }
    }
  }
 }

/**

  Adjust the Memory Address Map for any required silicon workarounds.

  @param[in, out]  Host          Pointer to sysHost.
  @param[in, out]  MemMapData    Pointer to the memory map private data structure.

  @retval SUCCESS                The function is executed successfully without any error.
  @retval FAILURE                The function failed to apply a workaround.

**/
UINT32
AdjustMemAddrMapForSiliconWorkaround (
  IN OUT SYSHOST          *Host,
  IN OUT MEMORY_MAP_DATA  *MemMapData
  )
{
#ifdef DDRT_SUPPORT
  EFI_STATUS Status;

  if (IsSiliconWorkaroundEnabled ("S2206200728")) {
    Status = ApplyWaS2206200728 (Host, MemMapData);
    if (EFI_ERROR (Status)) {
      RcDebugPrint (SDBG_ERROR, "Failed to apply S2206200728\n");
      return FAILURE;
    }
  }
#endif // DDRT_SUPPORT

  return SUCCESS;
}

/**
  This function programs 10nm wave 1 specific CSRs that are suggested as workarounds.

  @param[in] Host         Pointer to sysHost.
  @param[in] MemMapData   Pointer to memory map private data structure.
  @param[in] ScktId       Socket index.

  @retval N/A
**/
VOID
WriteWorkAroundCsrs (
  IN SYSHOST            *Host,
  IN MEMORY_MAP_DATA    *MemMapData,
  IN UINT8              Sckt
  )
{
  CHANNEL_NVRAM_STRUCT                  (*ChannelNvList)[MAX_CH];
  UINT8                                 ChIndex;
  UINT8                                 McIndex;
  UINT32                                AndMask;
  UINT32                                OrMask;
  UINT8                                 MaxImc;
  UINT8                                 NumChPerMc;

  MaxImc = GetMaxImc ();
  NumChPerMc = GetNumChannelPerMc ();
  ChannelNvList = &Host->nvram.mem.socket[Sckt].channelList;
  AndMask = 0xFFFF;
  OrMask = 0x0000;

  for (McIndex = 0; McIndex < MaxImc; McIndex++) {
    if (!CheckSocketMcEnabled(Sckt, McIndex)) {
      continue;
    }

    for (ChIndex = 0; ChIndex < NumChPerMc; ChIndex++) {
      if ((*ChannelNvList)[ChIndex + (McIndex * NumChPerMc)].enabled == 0) {
        continue;
      }

      if (Host->var.mem.volMemMode == VOL_MEM_MODE_1LM) {
        OrMask |= BIT0;
      } else {
        AndMask &= ~BIT0;
      }

      //
      //  Bit 0 has to be 0 for AppDirect
      //
      if (Host->var.mem.dfxMemVars.dfxPerMemMode != NON_PER_MEM_MODE &&
        Host->var.mem.socket[Sckt].channelList[ChIndex + (McIndex * NumChPerMc)].NonVolSize != 0) {
        AndMask &= ~BIT0;
      }

      McSetScratchBits (
        Sckt,
        McIndex,
        ChIndex,
        AndMask,
        OrMask
        );
    }
  }

  for (McIndex = 0; McIndex < MaxImc; McIndex++) {
    //Disable DDR4 Critical data chunk
    if (IsSiliconWorkaroundEnabled ("S1409883482")) {
      SetM2mDefeatures0DiscritChunk(Sckt, McIndex, TRUE);
    }
  }
}

/**

  Adjust SAD indexes used across SNC clusters to insure that there are not any disabled local DRAM rules between any
  enabled local DRAM rules.

  DRAM multicast feature is used to enable local DRAM rules for memory that resides in SNC clusters.
  DRAM broadcast feature to enable local DRAM rules for memory that does not reside in SNC clusters.
  Where there exists both SNC and non-SNC ranges the function patches NXM(s) to insure Local DRAM Rules
  are Contiguous when Mapped Memory is split between SNC and Non-SNC Ranges.

  @param[in] Host                - Pointer to sysHost
  @param[in] MemMapData          - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param[in] socketNum           - Socket number

  @retval Status

**/
UINT32
AdjustSadTableForSncMulticast (
  IN PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           SocketNum
  )
{
  return SUCCESS;
}

/**

  Returns the number of clusters at system level for volatile memory for route tables.

  @param[in] MemMapData Pointer to the memory map private data structure.
  @param[in] Sckt       Socket Id

  @return System cluster level

**/
UINT8
RtGetTotalClusterVolMem (
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           Sckt
  )
{
  if (IfFeatureEnabled (TypeSnc)) {
    return GetNumOfClusterPerSystem ();
  } else if (MemMapData->UmaBasedClustersPerSocket != UMABASEDCLUSTERING_DISABLED) {
    return MemMapData->UmaBasedClustersPerSocket;
  } else {
    return 1;
  }
}

#ifdef DDRT_SUPPORT
/**

  Returns the number of clusters at system level for PMem for route tables.

  @param[in] MemMapData Pointer to the memory map private data structure.
  @param[in] Sckt       Socket Id

  @return System cluster level

**/
UINT8
RtGetTotalClusterPMem (
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           Sckt
  )
{
  return 1;
}
#endif

/**

  This routine returns the number of CHA DRAM decoder clusters supported.

 @retval Number of clusters for CHA DRAM CSR's

**/
UINT8
EFIAPI
GetNumOfDramClustersPerSystem (
  VOID
  )
{
  return 1;
}

/**

  This routine abstracts the rank target id for rank interleave list.

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket number
  @param[in] Ch          - Channel number
  @param[in] Dimm        - Dimm number
  @param[in] Rank        - Rank number

**/
UINT8
GetRirRankTarget (
  IN SYSHOST            *Host,
  IN UINT8              Socket,
  IN UINT8              Ch,
  IN UINT8              Dimm,
  IN UINT8              Rank
  )
{
  struct ddrRank (*RankList)[MAX_RANK_DIMM];

  RankList = GetRankNvList (Host, Socket, Ch, Dimm);

  return (*RankList)[Rank].phyRank;
}

#ifdef DDRT_SUPPORT
/**

  set MC.AMAP and force latency on Wave 1 and Wave 2 platforms if PMem present

  @param[in] Sckt     Socket number.
  @param[in] McIndex    Index of memory controller.
  @param[in] NumChPerMc Number of channels per memory controller.

  @retval None

**/
VOID
ForceLatencyIfRequired (
  IN UINT8  Sckt,
  IN UINT8  McIndex,
  IN UINT8  NumChPerMc
  )
{
  UINT8 ChannelInterleaveBitMap;
  UINT8 ChIndex;

  if (CheckSocketDcpmmPresent (Sckt)) {
    ChannelInterleaveBitMap = 0;
    if (IsMemTypePresentMcFunction (Sckt, McIndex, IsMemType2lm)) {
      //
      // If DDRT is present on the MC and is configured for 2LM, then each channel
      // populated with DDR4 or DDRT on the MC will be programmed with AMAP.force_lat = 1
      //
      ChannelInterleaveBitMap = FindNmBitMap (Sckt, McIndex);
    }
    //
    // Each channel populated with DDRT will be programmed with force_lat = 1
    //
    for (ChIndex = 0; ChIndex < NumChPerMc; ChIndex++) {
      if ((ChannelInterleaveBitMap & (BIT0 << ChIndex)) == 0 &&
        (AepDimmPresent (Sckt, ChIndex + (McIndex * NumChPerMc), NULL)) == 0) {
        continue;
      }

      McSetForceLatency (
        MemTechDdr,
        Sckt,
        McIndex,
        ChIndex
        );
    }
  }

  return;
}
#endif // DDRT_SUPPORT

/**

  Process value based on platform and config

  @param Host         Pointer to sysHost
  @param Sckt         Socket number
  @param[in] InValue  Value to consider.

  @return  Updated value based on platform and config

**/
UINT32
GetAdjustedRirValue (
  PSYSHOST Host,
  UINT8    Sckt,
  UINT32   InValue
  )
{
  return InValue;
}

/**

  This routine provides a hook to perform any request after Sad and Tad are Setup in Memory map.

  @param[in] Host         Pointer to sysHost.
  @param[in] MemMapData   Pointer to the memory map private data structure.

  @retval None

**/
VOID
PostMemoryMapConfig (
  IN PSYSHOST         Host,
  IN MEMORY_MAP_DATA  *MemMapData
  )
{
  EFI_STATUS   Status;

  Status = MemReservePsmiBuffers (Host, MemMapData);
  if (Status != EFI_SUCCESS) {
    RcDebugPrint (SDBG_MAX, "\n Memory could not be reserved for PSMI buffers \n");
  }

  return;
}

/**
  Write FZM related CSRs.

  This function checks if the boot-time fast zero memory (aka FZM or BZM) feature is supported on
  each DRAM rule. And the FZM related CHA registers will be programmed accordingly.

  @param[in] Host                 Pointer to the sysHost structure.
  @param[in] MemMapData           Pointer to the memory map private data structure.
  @param[in] SocketId             Socket index.
  @param[in] BeginSadId           Index of the first SAD entry on cluster.

**/
VOID
WriteFzmCsrs (
  IN SYSHOST          *Host,
  IN MEMORY_MAP_DATA  *MemMapData,
  IN UINT8            SocketId,
  IN UINT8            BeginSadId
  )
{
  //
  // Stub function as FZM is not supported on 10nm wave 1/2.
  //
  return;
}

/**

This routine checks SOC specific values of the Memory address mapping setup options.

  @param[in] Host        - Pointer to sysHost

  @retval N/A

**/
VOID
ReInitializeMemAddrMapInterleavingValues (
  IN SYSHOST            *Host
  )
{
  UINT8      MaxImc;
  UINT8      NumChPerMc;
  UINT8      VolMemMode;

  MaxImc = GetMaxImc ();
  NumChPerMc = GetNumChannelPerMc ();
  VolMemMode = GetVolMemMode ();

  if (VolMemMode == VOL_MEM_MODE_2LM) {
    if (Host->var.mem.imcInter == IMC_1WAY && Host->var.mem.chInter == CH_1WAY) {
      Host->var.mem.imcInter = MaxImc;
      Host->var.mem.chInter = NumChPerMc;
    } else if (NumChPerMc == CH_2WAY && Host->var.mem.chInter == CH_3WAY) {
      Host->var.mem.chInter = NumChPerMc;
    }
  }

  RcDebugPrint (SDBG_MINMAX, "\nValue set for Channel Interleave to %d\n", Host->var.mem.chInter);
  RcDebugPrint (SDBG_MINMAX, "\nValue set for Imc Interleave to %d\n", Host->var.mem.imcInter);
}

/** Create remote DRAM rules to target FPGA memory

  Add a remote SAD entry in all sockets.

  @param[in, out] Host       - Pointer to sysHost.
  @param[in, out] MemMapData - Pointer to memory map private data structure.
  @param[in]      SocketNum  - Socket ID.
  @param[in]      MemType    - Type of memory.
  @param[in]      ClusterId  - Cluster Id for the multicast access
  @param[in]	  NodeLimit  - FPGA memory Size in 64MB blocks

  @retval SUCCESS      -  Remote DRAM rule added.
  @retval FAILURE      -  Remote DRAM rules exceed maximum count.

**/

UINT32
AddRemoteSadEntryForFPGAMemory (
  IN  SYSHOST         *Host,
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  UINT8           SocketNum,
  IN  MEM_TYPE        MemType,
  IN  UINT8           ClusterId,
  IN UINT32           FpgaMemSize
  )
{

  UINT8       SadIndex;
  UINT32      Limit;
  UINT8       Sckt;
  UINT8       RemoteSadIndex;
  RSAD_TABLE  *RemoteSadEntry;

  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {
    //
    // If local socket or remote socket disabled, then skip socket
    //
    if (Sckt == SocketNum || (CheckSocketEnabled (Sckt) == 0)) {
      continue;
    }

    //
    // Get index of next available SAD entry.
    //
    SadIndex = FindSadIndex (Host, MemMapData, Sckt, ClusterId);

    //
    // Get index of next available remote SAD entry.
    //
    RemoteSadIndex = FindRemoteSadIndex (MemMapData, Sckt);

    //
    // check for previous socket is FPGA type and skip it if exists
    // For this type there wont be any local or remote DRAM rule exists
    //
    if (SadIndex == 0 && RemoteSadIndex == 0) {
		continue;
    }

    if (RemoteSadIndex >= MemMapData->MaxRemoteSadRules) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Remote SAD rules exceed %d.\n", MemMapData->MaxRemoteSadRules);
      OutputWarning (WARN_INTERLEAVE_FAILURE, WARN_SAD_RULES_EXCEEDED, SocketNum, 0xFF, 0xFF, 0xFF);
      return FAILURE;
    }

    //
    // Get SAD base, limit from Local SAD Rule
    //
    Limit = Host->var.mem.socket[Sckt].SAD[SadIndex - 1].Limit;

    //
    // Compare local Dram rule limit with remote and get the highest limit
    //
    if (RemoteSadIndex > 0) {
      if (MemMapData->MemMapSocket[Sckt].RSad[RemoteSadIndex - 1].Limit > Limit) {
        Limit = MemMapData->MemMapSocket[Sckt].RSad[RemoteSadIndex - 1].Limit;
      }
    }
    RemoteSadEntry = &MemMapData->MemMapSocket[Sckt].RSad[RemoteSadIndex];
    RemoteSadEntry->Enable = 1;
    RemoteSadEntry->Attr = 0;
    RemoteSadEntry->Base = Limit;
    RemoteSadEntry->NodeId = SocketNum;
    RemoteSadEntry->Limit = Limit + FpgaMemSize;
    RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Added Base = 0x%x, Limit = 0x%x\n", RemoteSadEntry->Base, RemoteSadEntry->Limit);
  }
  return SUCCESS;
}
