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

#include <KtiSetupDefinitions.h>
#include <Library/MemMcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Gen2/MemDecodeNode.h>
#include <Library/PcuIpLib.h>
#include <Library/MemRcLib.h>
#include <Library/OobIpLib.h>
#include <Include/MemMapHost.h>
#include <Library/MemTypeLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemSmeeIpLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemoryServicesLib.h>
#include <Chip/Include/FnvAccessCommon.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Guid/SocketCommonRcVariable.h>
#include <Library/MemDecodeHbmLib.h>
#include <Library/AdvMemTestLib.h>
#include <MemDecodeRas.h>

//
// Module variables
//
#define OOB_MAILBOX_DPA         0x102000   // OOB Small Mailbox DPA: 1MB + 8KB ~ 1MB + 12KB

//
// Mailbox offset in system address space needs to consider interleave removed bits
//
#define OOB_MAILBOX_OFFSET      (OOB_MAILBOX_DPA << BLOCK_DECODER_BITS)

//
// CPUID bit indicates if BZM is supported or not in the processor
//
#define BZM_CPUID_BIT           BIT15

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
  if (IS_HBM_2LM_CACHE_DDR (Host)) {
    return GetHbmCacheSize (MemMapData, Sckt);
  } else {
    return GetChannelMemSize (Sckt, Ch);
  }
}

/** Performs CPU specific functions to initialize memory map parameters

  Performs 10nm Wave3 specific memory map private data structure initialization besides the common part
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
  MEMORY_MAP_HOST        *MemMapHost;
  SYS_SETUP              *Setup;
  BOOLEAN                IsPmemFuseDisabled;

  Setup = GetSysSetupPointer ();
  MemMapHost = GetMemMapHostPointer ();

  if (Host == NULL || MemMapData == NULL) {
    return;
  }

  MemMapHost->PcatCapability.MemModeCapabilities = MODE_1LM | MODE_2LM | MODE_AD;
  IsPmemFuseDisabled = McCheckCrPmemFuseDisabled ();

  //
  // Force volatile memory mode to 1LM if DDR5 9x4 DRAM is mixed with DDRT2.0
  //
  if (Check9x4Ddr5DimmPresent() && IS_DDR_2LM_CACHE(Host)) {
    Host->var.mem.volMemMode = VOL_MEM_MODE_1LM;
    RcDebugPrint (SDBG_MAX, "DDR5 9x4 Dimm is present, forcing to 1LM.\n");
    MemMapHost->PcatCapability.MemModeCapabilities &= ~MODE_2LM;
  }

  //Adjust the memory modes if the the silicon has reduced capability
  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {
    if (!CheckSocketEnabled (Sckt)) {
      continue;
    }
    PcuCAPID1.Data = Host->var.common.procCom[Sckt].capid1;
    if (PcuCAPID1.Bits.disable_2lm == 1) {
      RcDebugPrint (SDBG_MAX, "Silicon capability does not support 2LM, forcing to 1LM.\n");
      Host->var.mem.volMemMode = VOL_MEM_MODE_1LM;
      Host->var.mem.CacheMemType = CACHE_TYPE_DEFAULT;
      MemMapHost->PcatCapability.MemModeCapabilities &= ~MODE_2LM;
    }
    if ((PcuCAPID1.Bits.disable_cr_pmem == 1) || (IsPmemFuseDisabled)) {
      RcDebugPrint (SDBG_MAX, "Silicon capability does not support persistent modes, forcing to non-persistent mode.\n");
      Host->var.mem.dfxMemVars.dfxPerMemMode = NON_PER_MEM_MODE;
      MemMapHost->PcatCapability.MemModeCapabilities &= ~MODE_AD;
    }
  }

  //
  // Wave3 can support only DDRT channel granularity 256B and route table used for address decoding is the same.
  // Override user selection here.
  //
  Host->var.mem.dfxMemVars.DfxDdrtChInterleaveGran = MEM_INT_GRAN_CH_256B_TGT_4KB;
  MemMapData->DdrtIntlvGranularity = INTERLEAVE_GRANULARITY_256B;
  MemMapData->Is2LmUsingRt2lm = FALSE;

  MemMapHost->DdrtIntlvGranularity = MemMapData->DdrtIntlvGranularity;

  //
  // Init. max local and remote DRAM rules count
  //
  MemMapData->MaxSadRules = SAD_RULES_10NM;
  MemMapData->MaxRemoteSadRules = REMOTE_SAD_RULES_10NM_WAVE1;

  if (Host->var.mem.PmemCaching == PMEM_WBCACHE_ENABLED) {
    MemMapData->PmemCaching = LegacyPmCaching;
  }
  MemMapHost->PcatCapability.MmCacheCapabilities = (InTile | XTile);

  MemMapHost->IsFadrEnabled = (BOOLEAN) (Setup->mem.FadrSupport == FADR_ENABLED);

  InitHbmMemoryMap (MemMapData);

  //
  // Initialize DDR XOR bitmap according to setup policy.
  //
  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {
    if (!CheckSocketEnabled (Sckt)) {
      continue;
    }
    MemMapHost->SocketInfo[Sckt].XorMode = 0;
    if (Setup->mem.dfxMemSetup.DfxClusterXorEn != 0) {
      MemMapHost->SocketInfo[Sckt].XorMode |= CLUSTER_XOR;
    }
    if (Setup->mem.dfxMemSetup.DfxChannelXorEn != 0) {
      MemMapHost->SocketInfo[Sckt].XorMode |= CHANNEL_XOR;
    }
    if (Setup->mem.dfxMemSetup.DfxPseudoChannelXorEn != 0) {
      MemMapHost->SocketInfo[Sckt].XorMode |= PSEUDO_CHANNEL_XOR;
    }
    if (Setup->mem.dfxMemSetup.DfxDimmXorEn != 0) {
      MemMapHost->SocketInfo[Sckt].XorMode |= DIMM_XOR;
    }
    if (Setup->mem.dfxMemSetup.DfxRankXorEn != 0) {
      MemMapHost->SocketInfo[Sckt].XorMode |= RANK_XOR;
    }
    if (Setup->mem.dfxMemSetup.DfxSidXorEn != 0) {
      MemMapHost->SocketInfo[Sckt].XorMode |= SID_XOR;
    }
    if (Setup->mem.dfxMemSetup.DfxBankXorEn != 0) {
      MemMapHost->SocketInfo[Sckt].XorMode |= BANK_XOR;
    }

    //
    // XTile must be disabled if PMEM mode is not allowed since FM transactions
    // act like 1LM access to MC when XTile 2LM flow is used.
    //
    MemMapData->MemMapSocket[Sckt].XTile2lm = TRUE;
    if ((IsPmemFuseDisabled) || (!IS_DDR_2LM_OR_MIXED (Host))) {
      MemMapData->MemMapSocket[Sckt].XTile2lm = FALSE;
    }
  }

  MemMapData->UmaBasedClustersPerSocket = GetKtiOutputUmaClusterSetting ();

  if (IfFeatureEnabled (TypeSnc)) {
    MemMapData->UmaBasedClustersPerSocket = UMABASEDCLUSTERING_DISABLED;
  }

  MemMapHost->MaxSadRules = MemMapData->MaxSadRules;
}

/** Performs CPU specific functions to check memory address map setup options

  Performs 10nm Wave3 CPU specific setup option check besides the common part
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
  if (IsHbmSku ()) { // Cache memory type only used in HBM SKU
    if ((CacheMemType == CACHE_TYPE_DDR_CACHE_PMEM) || (CacheMemType == CACHE_TYPE_HBM_CACHE_DDR)) {
      Host->var.mem.CacheMemType = CacheMemType;
    } else {
      RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
         "\nWARNING!!! Invalid option value for Cache Memory Type. Setting to Default.\n");
      Host->var.mem.CacheMemType = CACHE_TYPE_DEFAULT;
    }

    if ((CacheMemType == CACHE_TYPE_HBM_CACHE_DDR) && (Host->var.mem.volMemMode != VOL_MEM_MODE_2LM)) {
      RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "\nWARNING!!! Cache type set to HBM but system not in 2LM mode. Setting Cache memory type to DDR.\n");
      Host->var.mem.CacheMemType = CACHE_TYPE_DDR_CACHE_PMEM;
    }
  } else if (CacheMemType != CACHE_TYPE_DDR_CACHE_PMEM) {
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
  // Use the ChopType of Socket 0 as the subtype of all CPUs.
  //
  if (GetChopType (0) == TypeHcc) {
    return TRUE;
  }

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
  *PtrPsBase = 0;
  *PtrPsLimit = FindMinNmSize (Host, MemMapData, SocketId, McIndex);
}

/**
  Return whether memory decoder must program near memory dram rule for the input SAD ID.
  NM_DRAM_RULE is programmed only for SA based patrol/sparing and is not used for CA based patrol/sparing.
  10nm wave3 can support SA based patrol/sparing for 1LM DDR memory and CA based patrol/sparing for 2LM NM.

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
  if ((SadEntry->type != MemType1lmDdr) && (SadEntry->type != MemType2lmHbmCacheDdr)) {
    return FALSE;
  }
  if (((SadEntry->imcInterBitmap & (BIT0 << McIndex)) == 0)) {
    return FALSE;
  }
  if (GetDdrPsLimit (Host, SocketId, McIndex, SadIndex) == 0) {
    return FALSE;
  }
  return TRUE;
}

/**
  Check if the near memory cache is sufficient to support the biased 2-way near memory cache feature.

  This function checks if there is enough near memory on each channel interleaved for 2LM to support the
  biased 2-way near memory cache feature. If the near memory cache size is less than 8GB while the granularity
  is 4GB (that is, equals to 4GB) on a channel, this feature will be disabled for the entire system.


  @retval TRUE           The near memory cache is sufficient.
  @retval FALSE          The near memory cache is insufficient.
**/
BOOLEAN
IsNmCacheSufficient (
  VOID
  )
{
  UINT8                 SocketId;
  UINT8                 McId;
  UINT8                 ChId;
  UINT8                 MaxImc;
  UINT8                 VolMemMode;
  UINT8                 NumChPerMc;

  MaxImc = GetMaxImc ();
  VolMemMode = GetVolMemMode ();
  NumChPerMc = GetNumChannelPerMc ();

  //
  // Skip the check of near memory cache size if the volatile memory mode is not 2LM or mixed 1LM+2LM.
  // Logic that follows is designed for DDR
  //
  if ((VolMemMode != VOL_MEM_MODE_MIX_1LM2LM) && (VolMemMode != VOL_MEM_MODE_2LM) &&
    GetCacheMemType () != CACHE_TYPE_DDR_CACHE_PMEM) {
    return FALSE;
  }

  for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
    if (!CheckSocketEnabled (SocketId)) {
      continue;
    }

    for (McId = 0; McId < MaxImc; McId++) {
      if (!CheckSocketMcEnabled (SocketId, McId)) {
        continue;
      }

      for (ChId = 0; ChId < NumChPerMc; ChId++) {
        if (GetChannelMemSize (SocketId, ChId + (McId * NumChPerMc)) == 0) {
          continue;  // Continue if there is not DDR dimm populated on this channel.
        }

        //
        // The NM cache is insufficient to support the biased 2-way NM cache feature
        // if the NM cache size on a channel is less than 8GB (that is, equals to 4GB).
        //
        if (GetNmCacheSize (SocketId, McId, ChId) == 0x0) {
          return FALSE;
        }
      }
    }
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
  SYS_SETUP           *Setup;
  MEMORY_MAP_HOST     *MemMapHost;

  Setup = GetSysSetupPointer ();
  MemMapHost = GetMemMapHostPointer ();

  //
  // Biased 2-way NM cache is not supported for the entire system if the NM cache size is insufficient on a channel.
  //
  MemMapHost->IsTwoWayNmCacheEn = IsNmCacheSufficient () && (BOOLEAN) Setup->mem.EnableTwoWayNmCache;
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
  return;
}

/**
  This function configure NvDimm mailbox related CSRs to support DDRT-2 MCTP bridge.

  @param[in] Host      Pointer to sysHost.
  @param[in] ScktId    Socket index.

  @retval N/A
**/
VOID
WriteNvDimmMailboxCsrs (
  IN  PSYSHOST      Host,
  IN  UINT8         ScktId
  )
{
  UINT8                 ChId;
  UINT8                 MaxCh;
  UINT32                BlockDecoderBase;
  MEMORY_MAP_HOST       *MemMapHost;
  OOB_NVDIMM_MAILBOX    OobNvdimmMailbox;

  MaxCh = GetMaxImc () * GetNumChannelPerMc ();
  MemMapHost = GetMemMapHostPointer ();
  BlockDecoderBase = MemMapHost->BlockDecoderData.BlockDecoderBase;

  if (BlockDecoderBase == 0 || Host->nvram.mem.socket[ScktId].DcpmmPresent == 0) {
    return;
  }

  OobNvdimmMailbox.OobAddress = LShiftU64 (BlockDecoderBase, CONVERT_64MB_TO_B) + OOB_MAILBOX_OFFSET +
                                (ScktId << (BLOCK_DECODER_SOCKET_TARGET_SHIFT));

  OobNvdimmMailbox.InterleaveWays = BIT0 << BLOCK_DECODER_BITS;
  OobNvdimmMailbox.InterleaveSize = BLOCK_DECODER_MC_ID_SHIFT;

  OobNvdimmMailbox.DimmPresence = 0;

  for (ChId = 0; ChId < MaxCh; ChId++) {
    if (IsDcpmmPresentChannel (ScktId, ChId)) {
      OobNvdimmMailbox.DimmPresence |= (BIT0 << ChId);
    }
  }

  RcDebugPrint (SDBG_MAX,
    "\nOobNvdimmMailbox: OobAddress: 0x%lx, InterleaveWays: 0x%x, InterleaveSize: %d, DimmPresence: 0x%x\n",
    OobNvdimmMailbox.OobAddress,
    OobNvdimmMailbox.InterleaveWays,
    OobNvdimmMailbox.InterleaveSize,
    OobNvdimmMailbox.DimmPresence
    );

  //
  // Program NvDimm mailbox CSRs.
  //
  OobSetNvDimmMailBox (ScktId, &OobNvdimmMailbox);

  return;
}

/**
  This function fetches the base and limit of AppDirect and/or AppDirect-WB from all the available
  SAD entries and programs them to the related CSRs for MK-TME.

  @param[in] Host           Pointer to sysHost.
  @param[in] MemMapData     Pointer to the memory map private data structure.
  @param[in] ScktId         Socket Index.

  @retval N/A

**/
VOID
WriteMktmeCsrs (
  IN  PSYSHOST         Host,
  IN  MEMORY_MAP_DATA  *MemMapData,
  IN  UINT8            ScktId
  )
{
  UINT8                ClusterId;
  UINT8                BeginSadId;
  UINT8                EndSadId;
  UINT8                SadId;
  SAD_TABLE            *SadEntry;
  UINT8                McInterBitMap;
  UINT8                ChInterBitMap;
  UINT8                McId;
  UINT8                ChId;
  MKTME_PMEM_DATA      MktmePmemData;
  UINT8                MaxImc;
  UINT8                NumChPerMc;
  UINT8                PmemNum;

  MaxImc = GetMaxImc ();
  NumChPerMc = GetNumChannelPerMc ();

  for (McId = 0; McId < MaxImc; McId++) {
    if (!CheckSocketMcEnabled (ScktId, McId)) {
      continue;
    }

    for (ChId = 0; ChId < NumChPerMc; ChId++) {
      if (!IsDcpmmPresentChannel (ScktId, ChId + McId * NumChPerMc)) {
        continue;
      }

      //
      // Count the number of persistent memory region on this channel (Not greater than 2).
      //
      PmemNum = 0;
      ZeroMem (&MktmePmemData, sizeof (MktmePmemData));

      for (ClusterId = 0; ClusterId < MemMapData->NumberofChaDramClusters; ClusterId++) {
        BeginSadId = ClusterId * MemMapData->MaxSadRules;
        EndSadId = BeginSadId + MemMapData->MaxSadRules;
        if (Host->var.mem.socket[ScktId].SAD[BeginSadId].Enable == 0) {
          break;
        }

        for (SadId = BeginSadId; SadId < EndSadId; SadId++) {
          SadEntry = &Host->var.mem.socket[ScktId].SAD[SadId];
          if (SadEntry->Enable == 0) {
            break;  // break if there is no more available SAD.
          }

          //
          // Get the interleave bit map of MC and FM channel from SAD entry.
          // Assume that AppDirect-WB is in-tile only.
          //
          McInterBitMap = SadEntry->imcInterBitmap;
          ChInterBitMap = SadEntry->FMchannelInterBitmap[McId];
          if ((!IsMemTypeAppDirect (SadEntry->type)) ||
              ((McInterBitMap & (BIT0 << McId)) == 0) ||
              ((ChInterBitMap & (BIT0 << ChId)) == 0)) {
            continue;  // continue if this is not a AppDirect/AppDirect-WB SAD entry, or the Mc/Ch is not interleaved.
          }

          //
          // Save the base and limit of AppDirect/AppDirect-WB region in 1GB units.
          //
          if (PmemNum == 0) {
            MktmePmemData.Pmem1Base = SadEntry->Base >> CONVERT_64MB_TO_GB_GRAN;
            MktmePmemData.Pmem1Limit = SadEntry->Limit >> CONVERT_64MB_TO_GB_GRAN;
            PmemNum++;
          } else if (PmemNum == 1) {
            MktmePmemData.Pmem2Base = SadEntry->Base >> CONVERT_64MB_TO_GB_GRAN;
            MktmePmemData.Pmem2Limit = SadEntry->Limit >> CONVERT_64MB_TO_GB_GRAN;
            break;  // There are up to two persistent memory region on one channel.
          }
        }  // SAD loop
      }  // Cluster loop

      //
      // Programs the MK-TME related CSRs if there is any persistent memory region found on this channel.
      //
      if (PmemNum != 0) {
        SmeeSetMktmePmemData (ScktId, McId, ChId, &MktmePmemData);
      }
    }  // Ch loop
  }  // Mc loop

  return;
}

/**
  This function writes CSRs required by biased 2-way near memory cache feature (M2M, MC and CHA),
  where part of the MC CSR programming has been covered by the 2LM Decoder CSR programming of MemDecodeLib.

  @param[in] ScktId    Socket index.

  @retval N/A
**/
VOID
WriteTwoWayNmCacheCsrs (
  IN UINT8            ScktId
  )
{
  UINT8               McId;
  UINT8               ChId;
  UINT8               ChaId;
  UINT8               MaxImc;
  UINT8               TotalCha;
  UINT8               MaxChPerMc;
  BOOLEAN             IsTwoWayNmCacheEn;
  SYS_SETUP           *Setup;
  M2M_TWO_WAY_CACHE   M2mTwoWayCache;

  MaxImc = GetMaxImc ();
  TotalCha = GetTotChaCount (ScktId);
  MaxChPerMc = GetNumChannelPerMc ();
  Setup = GetSysSetupPointer ();

  IsTwoWayNmCacheEn = IsTwoWayNmCacheEnabled ();

  //
  // M2M CSR programming for biased 2-way NM cache.
  //
  M2mTwoWayCache.Enable = IsTwoWayNmCacheEn;
  M2mTwoWayCache.NonPreferredWayMask = Setup->mem.NonPreferredWayMask;
  M2mTwoWayCache.PreferredReadFirst = Setup->mem.PreferredReadFirst;

  RcDebugPrint (SDBG_MAX,
    "M2mTwoWayCache: Enable %d  NonPreferredWayMask 0x%03x  PreferredReadFirst %d\n",
    M2mTwoWayCache.Enable,
    M2mTwoWayCache.NonPreferredWayMask,
    M2mTwoWayCache.PreferredReadFirst
    );

  for (McId = 0; McId < MaxImc; McId++) {
    if (!CheckSocketMcEnabled (ScktId, McId)) {
      continue;
    }

    SetM2mTwoWayNmCache (ScktId, McId, &M2mTwoWayCache);

    //
    // Part of MC CSR programming for biased 2-way NM cache.
    //
    for (ChId = 0; ChId < MaxChPerMc; ChId++) {
      if (!IsChannelEnabled (ScktId, ChId + (McId * MaxChPerMc))) {
        continue;
      }
      //
      // Enable VC preemption to avoid the deadlock: VC0 txns stuck in the pending queues and VC1 txns
      // in the bank schedulers that then couldn’t issue due to no RIDs available.
      //
      McSetVcPreemptionBit (ScktId, McId, ChId, IsTwoWayNmCacheEn);
    }
  }

  //
  // CHA CSR programming for biased 2-way NM cache.
  //
  for (ChaId = 0; ChaId < TotalCha; ChaId++) {
    ChaSetTwoWayNmCache (ScktId, ChaId, IsTwoWayNmCacheEn);
  }
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
  SYS_SETUP             *Setup;
  MEMORY_MAP_HOST       *MemMapHost;

  Setup = GetSysSetupPointer ();
  MemMapHost = GetMemMapHostPointer ();

  //
  // Disable fADR support for entire system, if
  //   1. The setup policy of fADR is not enabled.
  //   2. There is no DCPMM dimm present on system.
  //   3. There is no SAD entry with AppDirect memory type on system.
  //
  if ((Setup->mem.FadrSupport != FADR_ENABLED) ||
      (Host->nvram.mem.DcpmmPresent == 0) ||
      (!IsMemTypePresentSystemFunction(IsMemTypeAppDirect))) {
    MemMapHost->IsFadrEnabled = FALSE;
  }
}

/**
  This function writes CSRs required by fADR workaround.

  @param[in] Host       Pointer to sysHost.
  @param[in] MemMapData Pointer to memory map private data structure.
  @param[in] ScktId     Socket index.

  @retval N/A
**/
VOID
WriteFadrWorkaroundCsrs (
  IN SYSHOST            *Host,
  IN MEMORY_MAP_DATA    *MemMapData,
  IN UINT8              ScktId
  )
{
  UINT8                 McId;
  UINT8                 MaxImc;
  UINT8                 SadId;
  UINT8                 ClusterId;
  UINT8                 BeginSadId;
  UINT8                 EndSadId;
  UINT8                 MaxChPerMc;
  UINT8                 ChBitMask;
  UINT8                 NmFlushEnBitMap;
  SAD_TABLE             *SadEntry;

  MaxImc = GetMaxImc ();
  MaxChPerMc = GetNumChannelPerMc ();
  ChBitMask = (BIT0 << MaxChPerMc) - 1;

  if (!IsFadrEnabled ()) {
    return;  // return if fADR is disabled.
  }

  //
  // Get the bitmap of channels where NM flush should be enabled from all SAD entries.
  // One bit corresponds to one channel.
  //
  NmFlushEnBitMap = 0;
  for (ClusterId = 0; ClusterId < MemMapData->NumberofChaDramClusters; ClusterId++) {
    BeginSadId = ClusterId * MemMapData->MaxSadRules;
    EndSadId = BeginSadId + MemMapData->MaxSadRules;

    SadEntry = &Host->var.mem.socket[ScktId].SAD[BeginSadId];
    if (SadEntry->Enable == 0) {
      break;
    }

    for (SadId = BeginSadId; SadId < EndSadId; SadId++) {
      SadEntry = &Host->var.mem.socket[ScktId].SAD[SadId];
      if (SadEntry->Enable == 0) {
        break;
      }

      if ((!SadEntry->local) || (SadEntry->type != MemType2lmDdrWbCacheAppDirect)) {
        continue;
      }

      //
      // NM flush should be enabled on channels that are interleaved for 2LM AD-WB NM.
      //
      for (McId = 0; McId < MaxImc; McId++) {
        if ((SadEntry->NmImcInterBitmap & (BIT0 << McId)) != 0) {
          NmFlushEnBitMap |= ((SadEntry->channelInterBitmap[McId] & ChBitMask) << (McId * MaxChPerMc));
        }
      }
    }  // SAD loop
  }  // Cluster loop
  RcDebugPrint (SDBG_MAX, "NmFlushEnBitMap: 0x%x\n", NmFlushEnBitMap);

  //
  // Program MC CSRs to disable the ADR actions on channels excluded from NM flush.
  //
  for (McId = 0; McId < MaxImc; McId++) {
    McSetAdrDisableBits (ScktId, McId, (~(NmFlushEnBitMap >> (McId * MaxChPerMc))) & ChBitMask);
  }
}

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
  if (MemMapData->MemMapSocket[SocketId].XTile2lm) {
    return MemMapData->MemMapSocket[SocketId].XTile2lmFmTarget[McId];
  }
  return McId;
}

/**
  This routine checks the POR configuration for X-tile 2LM for mod3

  @param[in] McBitmap         Far memory memory controller bit map.
  @param[in] ChannelBitmap    Far memory channel bit map.
  @param[in] NmMcBitmap       Near memory memory controller bit map.
  @param[in] NmChannelBitmap  Near memory channel bit map.

  @retval TRUE         The memory configuration is mod3 X-tile 2LM configuration.
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
  ROUTE_TABLE_HALVES  Half;
  UINT8               NumberOfChannels;
  UINT8               MaxImc;
  UINT8               McIndex;
  UINT8               MaxChPerMc;
  UINT8               ChannelIndex;

  //
  // Mod3 X-Tile 2LM does not support SNC4
  //
  if (IfFeatureEnabled (TypeSnc) && GetNumOfClusterPerSystem () == KTI_SNC4) {
    return FALSE;
  }

  MaxImc = GetMaxImc ();
  MaxChPerMc = GetNumChannelPerMc ();
  for (Half = RtHalf0; Half < MaxRtHalves; Half++) {
    NumberOfChannels = 0;
    for (McIndex = 0; McIndex < MaxImc; McIndex++) {
      if (GetHalfForMc (McIndex) != Half) {
        continue;
      }
      for (ChannelIndex = 0; ChannelIndex < MaxChPerMc; ChannelIndex++) {
        if ((NmChannelBitmap[McIndex] & (BIT0 << ChannelIndex)) != 0) {
          if ((ChannelBitmap[McIndex] & (BIT0 << ChannelIndex)) != 0) {
            //
            // One channel can not have both NM and FM
            //
            return FALSE;
          }
          NumberOfChannels++;
        }
      } // ChannelIndex
    } // McIndex
    if (NumberOfChannels != CH_3WAY) {
      //
      // Each Half should have 3 channel populated with DDR near memory.
      //
      return FALSE;
    }
  }

  return TRUE;
}

/**
  This routine checks the POR configuration for X-tile 2LM in SNC mode.

  @param[in]      SocketId     Id for the socket to check.
  @param[in]      McBitmap     Far memory controller bit map.
  @param[in]      NmMcBitmap   Near memory controller bit map.
  @param[in]      FmChCount    Number of channel with Far memory

  @retval TRUE         The memory configuration is POR X-tile 2LM SNC configuration.
  @retval FALSE        The memory configuration is not POR X-tile 2LM SNC configuration.

**/
BOOLEAN
CheckXTile2lmSncPorConfig (
  IN UINT8            SocketId,
  IN UINT8            McBitmap,
  IN UINT8            NmMcBitmap,
  IN UINT8            FmChCount
  )
{
  UINT8               Cluster;
  UINT8               ClustersPerSocket;
  UINT8               ClusterWithFm;
  UINT8               SncNmBitmapInternal[MAX_CLUSTERS];
  BOOLEAN             SncFmExist[MAX_CLUSTERS];
  UINT8               McBitMaskForCluster;
  UINT8               RtHalf;

  ClustersPerSocket = GetNumOfClusterPerSystem ();
  ClusterWithFm = 0;
  for (Cluster = 0; Cluster < ClustersPerSocket; Cluster++) {
    McBitMaskForCluster = GetMcBitMaskForCluster (Cluster, ClustersPerSocket);
    SncNmBitmapInternal[Cluster] = McBitMaskForCluster & NmMcBitmap;
    if (SncNmBitmapInternal[Cluster] == 0) {
      return FALSE; // In SNC mode, we require each cluster should have NM installed.
    }
    if ((McBitMaskForCluster & McBitmap) != 0) {
      ClusterWithFm++;
      SncFmExist[Cluster] = TRUE;
    } else {
      SncFmExist[Cluster] = FALSE;
    }
  }

  //
  // Cluster with far memory populated must be dividable by total custer
  //
  if (ClusterWithFm == 0 || ((ClustersPerSocket % ClusterWithFm) != 0)) {
    return FALSE;
  }

  if (FmChCount > 2) {
    return FALSE; // In SNC mode, we can only support 1 or 2 DDRTs.
  }

  if (FmChCount == 2) {
    //
    // If we have 2 DDRTs, each half should have DDRT populated.
    //
    for (RtHalf = RtHalf0; RtHalf < MaxRtHalves; RtHalf++) {
      if ((GetMcBitMaskForHalf (RtHalf) & McBitmap) == 0) {
        return FALSE;
      }
    }
  }

  return TRUE;
}


/**
  This routine checks the POR configurations for X-tile 2LM. It also pre-calculates
  near memory to far memory mapping for X-Tile 2LM and save in memory map private data structure.

  @param[in]      Host              Pointer to host pointer.
  @param[in, out] MemMapData        Pointer to the memory map private data structure.
  @param[in]      SocketId          Id for the socket to check.

  @retval TRUE         The memory configuration is POR X-tile 2LM configuration.
  @retval FALSE        The memory configuration is not POR X-tile 2LM configuration.

**/
BOOLEAN
CheckXTile2lmPorConfig (
  IN     SYSHOST          *Host,
  IN OUT MEMORY_MAP_DATA  *MemMapData,
  IN     UINT8            SocketId
  )
{
  UINT8               McBitmap;
  UINT8               ChBitmap[MAX_IMC];
  UINT8               NmMcBitmap;
  UINT8               NmChBitmap[MAX_IMC];
  UINT8               NmChNum[MAX_IMC];
  UINT8               MaxImc;
  UINT8               McIndex;
  UINT8               ChIndex;
  SYS_SETUP           *Setup;
  UINT8               NumChPerMc;
  UINT8               NmChCount;
  UINT8               FmChCount;
  UINT8               FmChHalf0Count;
  UINT8               FirstNmMc;
  UINT8               FmMcHalf[MaxRtHalves];
  ROUTE_TABLE_HALVES  Half;

  MaxImc = GetMaxImc ();
  NumChPerMc = GetNumChannelPerMc ();

  NmChCount = 0;
  FmChCount = 0;
  FmChHalf0Count = 0;
  McBitmap = 0;
  NmMcBitmap = 0;
  ZeroMem (ChBitmap, sizeof (ChBitmap));
  ZeroMem (NmChBitmap, sizeof (NmChBitmap));
  ZeroMem (NmChNum, sizeof (NmChNum));
  ZeroMem (FmMcHalf, sizeof (FmMcHalf));
  //
  // If AD-WB caching request then 2LM can be in-tile only
  //
  if ((Host->var.mem.socket[SocketId].WbCachePerSize != 0) &&
      (MemMapData->PmemCaching == NativePmCaching)) {
    return FALSE;
  }

  //
  // Collect memory topology
  //
  for (McIndex = 0; McIndex < MaxImc; McIndex++) {
    for (ChIndex = 0; ChIndex < NumChPerMc; ChIndex++) {
      if (GetChannelMemSize(SocketId, ChIndex + McIndex * NumChPerMc) > 0) {
        NmChBitmap[McIndex] |= BIT0 << ChIndex;
        NmChNum[McIndex] += 1;
        NmMcBitmap |= BIT0 << McIndex;
        NmChCount += 1;
      }
      Half = GetHalfForMc (McIndex);
      if (Host->var.mem.socket[SocketId].channelList[ChIndex + McIndex * NumChPerMc].volSize > 0) {
        ChBitmap[McIndex] |= BIT0 << ChIndex;
        if ((McBitmap & (BIT0 << McIndex)) == 0) {
          McBitmap |= BIT0 << McIndex;
          FmMcHalf[Half] = McIndex;
        }
        if (Half == RtHalf0) {
          FmChHalf0Count += 1;
        }
        FmChCount += 1;
      }
    }
  }

  //
  // Each MC with DDRT have to have near memory installed
  // with the exception of manufacture configure:
  // - Setup enforce POR is disabled
  // - 1 DDR channel + 1 DDRT channel
  //
  if ((McBitmap & ~NmMcBitmap) != 0) {
    Setup = GetSysSetupPointer ();
    if ((Setup->mem.enforcePOR != ENFORCE_POR_DIS) || (FmChCount != 1) || (NmChCount != 1)) {
      return FALSE;
    }
  }
  //
  // Check whether it is the in-tile configuration that
  //
  if (McBitmap == NmMcBitmap) {
    return FALSE;
  }
  //
  // Total far memory channel must be 1 or 2.
  //
  if ((FmChCount != 1) && (FmChCount != 2)) {
    return FALSE;
  }
  //
  // If we have only 2 far memory channel, they have to be distributed across halves.
  //
  if ((FmChCount == 2) && (FmChHalf0Count != 1)) {
    return FALSE;
  }
  //
  // Check for Mod3 configuration first
  //
  if (!CheckMod3XTile2lmPorConfig (McBitmap, ChBitmap, NmMcBitmap, NmChBitmap)) {
    //
    // Check NM configuration must be equal on all memory controllers that have NM installed.
    //
    FirstNmMc = (UINT8) LowBitSet32 (NmMcBitmap);
    if (FirstNmMc >= MaxImc) {
      return FALSE;
    }
    for (McIndex = 0; McIndex < MaxImc; McIndex++) {
      if ((NmMcBitmap & (BIT0 << McIndex)) != 0) {
        if (NmChNum[McIndex] != NmChNum[FirstNmMc]) {
          return FALSE;
        }
      }
    }
  }
  //
  // Check X-Tile 2LM configure in SNC
  //
  if (IfFeatureEnabled (TypeSnc) && !CheckXTile2lmSncPorConfig (SocketId, McBitmap, NmMcBitmap, FmChCount)) {
    return FALSE;
  }

  //
  // Calculate NM Far memory mapping and save to memory map provide data structure when it is valid X-Tile 2LM.
  //
  RcDebugPrint (SDBG_MAX, "XTile 2LM NM Far memory mapping:\nNM FM\n");
  for (McIndex = 0; McIndex < MaxImc; McIndex++) {
    if (((BIT0 << McIndex) & NmMcBitmap) == 0) {
      continue;
    }
    if (FmChCount == 1) {
      MemMapData->MemMapSocket[SocketId].XTile2lmFmTarget[McIndex] = (UINT8) LowBitSet32 (McBitmap);
    } else {
      Half = GetHalfForMc (McIndex);
      MemMapData->MemMapSocket[SocketId].XTile2lmFmTarget[McIndex] = FmMcHalf[Half];
    }
    RcDebugPrint (SDBG_MAX, "%2d %2d\n", McIndex, MemMapData->MemMapSocket[SocketId].XTile2lmFmTarget[McIndex]);
  }

  return TRUE;
}

/**
  Calculates XTile 2LM far memory bitmap.

  @param[in]  Host                 - Pointer to system host pointer.
  @param[in]  MemMapData           - Pointer to memory map private data structure.
  @param[in]  SocketId             - The socket index.
  @param[out] FmExistClusterBitmap - Pointer to bitmap of clusters which contain far memory.
  @param[out] FmBitmap             - Array of bitmap of far memory bitmap.

  @return far memory partition number

**/
UINT8
CalculateXTile2lmFmBitmap (
  IN     SYSHOST                  *Host,
  IN     MEMORY_MAP_DATA          *MemMapData,
  IN     UINT8                    SocketId,
     OUT UINT8                    *FmExistClusterBitmap,
     OUT UINT8                    FmBitmap[MAX_IMC]
  )
{
  UINT8            ClustersPerSocket;
  UINT8            Cluster;
  UINT8            ClusterWithFm;
  UINT8            Mc;
  UINT8            MaxImc;
  UINT8            McCh;
  UINT8            MaxChPerMc;

  //
  // Calculates far memory cluster exist bitmap and collect all volatile channel memory size
  //
  *FmExistClusterBitmap = 0;
  ClusterWithFm = 0;
  MaxImc = GetMaxImc ();
  MaxChPerMc = GetNumChannelPerMc ();
  for (Mc = 0; Mc < MaxImc; Mc++) {
    FmBitmap[Mc] = 0;
    for (McCh = 0; McCh < MaxChPerMc; McCh++) {
      if (Host->var.mem.socket[SocketId].channelList[Mc * MaxChPerMc + McCh].volSize > 0) {
        Cluster = GetClusterForMc (MemMapData, Mc);
        if ((*FmExistClusterBitmap & (BIT0 << Cluster)) == 0) {
          *FmExistClusterBitmap |= BIT0 << Cluster;
          ClusterWithFm++;
        }
        FmBitmap[Mc] |= 1 << Mc;
      }
    }
  }

  //
  // return far memory partition number can be either 1, 2 or 4
  //
  ClustersPerSocket = GetNumOfClusterPerSystem ();
  if (ClusterWithFm == 0 || ClusterWithFm == ClustersPerSocket) {
    return 1;
  } else {
    return ClustersPerSocket / ClusterWithFm;
  }
}

/**
  Extends XTile 2lm far memory bitmap across cluster(s) with no far memory.

  @param[in]      MemMapData           - Pointer to memory map private data structure.
  @param[in]      PartitionNum         - The number of partition.
  @param[in]      FmExistClusterBitmap - Bitmap of clusters which contain far memory.
  @param[in, out] FmBitmap             - Array of far memory bitmap.

**/
VOID
ExtendXTile2lmBitmap (
  IN     MEMORY_MAP_DATA       *MemMapData,
  IN     UINT8                 PartitionNum,
  IN     UINT8                 FmExistClusterBitmap,
  IN OUT UINT8                 FmBitmap[MAX_IMC]
  )
{
  UINT8                       McWithNoFm;
  UINT8                       McWithFm;
  UINT8                       McCount;
  UINT8                       MaxImc;

  MaxImc = GetMaxImc ();
  for (McWithNoFm = 0; McWithNoFm < MaxImc; McWithNoFm++) {
    if ((FmBitmap[McWithNoFm] & (BIT0 << McWithNoFm)) != 0) {
      continue;
    }
    //
    // Found a memory controller with no far memory, we will seek for memory controller with far memory
    //
    for (McWithFm = 0; McWithFm < MaxImc; McWithFm++) {
      if (GetClusterForMc (MemMapData, McWithNoFm) == GetClusterForMc (MemMapData, McWithFm) ||
         (FmBitmap[McWithFm] & (BIT0 << McWithFm)) == 0) {
        //
        // Skip memory controller in the same cluster or memory has no far memory
        //
        continue;
      }
      //
      // If the memory with far memory can still be extended, we will associate the memory to it.
      //
      McCount = CalculateInterleaveWaysfromBitmap (FmBitmap[McWithFm]);
      if (McCount < PartitionNum) {
        FmBitmap[McWithFm] |= BIT0 << McWithNoFm;
        break;
      }
    }
  }
}

/**
  Partition the far memory evenly among the memory controller based on the XTile 2LM far memory bitmap.

  @param[in]      Host                 - Pointer to system host data structure.
  @param[in, out] MemMapData           - Pointer to the memory map private data structure.
  @param[in]      SocketId             - The socket index.
  @param[in]      PartitionNum         - The number of partition.
  @param[in]      FmBitmap             - Array of far memory bit map.

**/
VOID
PartitionXTile2lmFm (
  IN     SYSHOST               *Host,
  IN OUT MEMORY_MAP_DATA       *MemMapData,
  IN     UINT8                 SocketId,
  IN     UINT8                 PartitionNum,
  IN     UINT8                 FmBitmap[MAX_IMC]
  )
{
  UINT8            Mc;
  UINT8            McWithFm;
  UINT8            McCh;
  UINT8            LastMc;
  UINT8            MaxImc;
  UINT8            McFmBitmap;
  UINT32           ChRemSize[MAX_MC_CH];
  UINT32           AlignedChMemSize;
  UINT8            FmCh;
  UINT8            MaxChPerMc;

  MaxImc = GetMaxImc ();
  MaxChPerMc = GetNumChannelPerMc ();
  RcDebugPrint (SDBG_MAX, "XTile 2LM SNC Partition Info:\nMc Partition\n");
  for (McWithFm = 0; McWithFm < MaxImc; McWithFm++) {
    McFmBitmap = FmBitmap[McWithFm];
    RcDebugPrint (SDBG_MAX, "%2d 0x%X\n", McWithFm, McFmBitmap);
    if (McFmBitmap == 0) {
      continue;
    }
    LastMc = (UINT8) HighBitSet32 (McFmBitmap);
    for (McCh = 0; McCh < MaxChPerMc; McCh++) {
      FmCh = McWithFm * MaxChPerMc + McCh;
      ChRemSize[McCh] = Host->var.mem.socket[SocketId].channelList[FmCh].volSize;
      for (Mc = 0; Mc < MaxImc; Mc++) {
        if (((BIT0 << Mc) & McFmBitmap) == 0) {
          continue;
        }
        //
        // Try to align each channel memory to GB to prevent memory loss first. The partition does not
        // need to be strictly even.
        //
        AlignedChMemSize = (Host->var.mem.socket[SocketId].channelList[FmCh].volSize / PartitionNum) & GB_BOUNDARY_ALIGN;
        if (Mc == LastMc) {
          MemMapData->MemMapSocket[SocketId].XTile2lmSncVolSize[Mc * MaxChPerMc + McCh] = ChRemSize[McCh];
        } else {
          MemMapData->MemMapSocket[SocketId].XTile2lmSncVolSize[Mc * MaxChPerMc + McCh] = AlignedChMemSize;
          ChRemSize[McCh] -= AlignedChMemSize;
        }
      }
    }
  }
  MemMapData->MemMapSocket[SocketId].XTile2lmSncFmPartition = TRUE;
}

/**
  Calculates XTile 2LM far memory bit map and adjust the far memory across clusters in the memory map
  private data structure if required.

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
  UINT8            FmBitmap[MAX_IMC];
  UINT8            FmExistClusterBitmap;
  UINT8            PartitionNum;

  if (MemMapData->MemMapSocket[SocketId].XTile2lm) {
    PartitionNum = CalculateXTile2lmFmBitmap (Host, MemMapData, SocketId, &FmExistClusterBitmap, FmBitmap);
    if (PartitionNum > 1) {
      ExtendXTile2lmBitmap (MemMapData, PartitionNum, FmExistClusterBitmap, FmBitmap);
      PartitionXTile2lmFm (Host, MemMapData, SocketId, PartitionNum, FmBitmap);
    }
  }
}
#endif // DDRT_SUPPORT

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
  if (IS_HBM_2LM_CACHE_DDR (Host)) {
    return GetHbmCacheSize (MemMapData, SocketNum);
  }

  MaxImc = GetMaxImc ();
  MaxChPerMc = GetNumChannelPerMc ();
  TotalMemSize = 0;

  // Find the number of channels with ddr memory populated and add the size in each iMC
  for (Mc = 0; Mc < MaxImc; Mc++) {
    for (Ch = 0; Ch < MaxChPerMc; Ch++) {
      if (Host->var.mem.volMemMode == VOL_MEM_MODE_MIX_1LM2LM) {
        TotalMemSize += Host->var.mem.socket[SocketNum].channelList[Ch + (Mc * MaxChPerMc)].DdrCacheSize;
      } else {
        TotalMemSize += GetChannelMemSize (SocketNum, Ch + (Mc * MaxChPerMc));
      }
    }
  }
  return TotalMemSize;
}

/**

  Find minimum NM size per channel  for this MC.

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
#ifdef DDRT_SUPPORT
    //
    // When mixed 1LM + 2LM is enabled, the min NM Size is the
    // requested DdrCacheSize.
    //
    if (Host->var.mem.volMemMode == VOL_MEM_MODE_MIX_1LM2LM) {
      return GetRequestedDdrCacheSize ();
    }
#endif // DDRT_SUPPORT

    if (Host->var.mem.CacheMemType == CACHE_TYPE_HBM_CACHE_DDR) {
      return GetHbmCacheSize (MemMapData, Sckt);
    }

    return FindMinNmChSizePwrOf2 (Host, Sckt, Mc);
}

/**

  This function writes M2M and MC CSRs to enable or disable the early completion.

  @param[in] SocketId          Socket index.

**/
VOID
WriteEarlyCmpCsrs (
  IN UINT8   SocketId
  )
{
  UINT8      McId;
  UINT8      MaxImc;
  UINT8      ChId;
  UINT8      MaxChPerMc;
  UINT8      EarlyCmpEn;
  SYS_SETUP  *Setup;

  EarlyCmpEn = 0x0;
  Setup = GetSysSetupPointer ();
  MaxImc = GetMaxImc ();
  MaxChPerMc = GetNumChannelPerMc ();

  //
  // S14011736159: EarlyCmp feature needs to be disabled for all SPR-SP steppings.
  // In HBM SKUs, a DFX knob is provide to do performance testing with EarlyCmp enabled for DDR MCs (Disabled for non-HBM SKUs).
  //
  if (!IsSiliconWorkaroundEnabled ("S14011736159") || (Setup->mem.dfxMemSetup.DfxDdrEarlyCmpEn == DDR_EARLY_CMP_ENABLED)) {
    EarlyCmpEn = 0x1;
  }

  //
  // For 1S system set akearlycmpen to 1, for 2S and up set it to 0. akearlycmpen is only supported on 1S system.
  // If biased 2-way NM$ is enabled, akearlycmpen must be set to 0 to avoid data corruption.
  //
  if (!CheckSysConfig (SYS_CONFIG_1S) || IsTwoWayNmCacheEnabled ()) {
    EarlyCmpEn = 0x0;
  }

  for (McId = 0; McId < MaxImc; McId++) {
    if (!CheckSocketMcEnabled (SocketId, McId)) {
      continue;
    }
    //
    // Enable or disable EarlyCmp in Mesh2Mem.
    //
    GetSetM2mAkEarlyCompletionsEnabled (SocketId, McId, GSM_FORCE_WRITE, &EarlyCmpEn);

    //
    // SysFeatures0.AkEarlyCmpEn relies on D2C to occur so DisableDir4AppDirect and AllowDirUpdToBlock
    // need to be set to 0x0 and 0x1 respectively to prevent D2C from getting squashed.
    //
    if (EarlyCmpEn != 0x0) {
      SetM2mDisableDir4AppDirect (SocketId, McId, 0x0);
      SetM2mAllowDirUpdToBlock (SocketId, McId, 0x1);
    }

    for (ChId = 0; ChId < MaxChPerMc; ChId++) {
      if (!IsChannelEnabled (SocketId, ChId + (McId * MaxChPerMc))) {
        continue;
      }
      //
      // Enable or disable EarlyCmp in MC.
      //
      McSetEnEarlyCmp (MemTechDdr, SocketId, ChId + (McId * MaxChPerMc), EarlyCmpEn);

      //
      // MC must be set consistently with Mesh2Mem for DisableDir4AppDirect and AllowDirUpdToBlock.
      //
      if (EarlyCmpEn != 0x0) {
        McDisableDirForAppDirect (MemTechDdr, SocketId, ChId + (McId * MaxChPerMc), 0x0);
        McSetAllowDirUpdToBlock (MemTechDdr, SocketId, ChId + (McId * MaxChPerMc), 0x1);
      }
    }
  }

  HbmWriteEarlyCmpCsrs (SocketId);
}

/**

  This function writes the 2LM directory optimization related CSRs. Some of the Mesh2Mem and CHA
  programming have been done in WriteMesh2MemCsrs().

  @param[in] Host            Pointer to the sysHost structure.
  @param[in] SocketId        Socket index.

**/
VOID
Write2lmDirOptCsr (
  IN SYSHOST  *Host,
  IN UINT8    SocketId
  )
{
  UINT8       ChaId;
  UINT8       TotalCha;
  SYS_SETUP   *Setup;

  Setup = GetSysSetupPointer ();
  TotalCha = GetTotChaCount (SocketId);

  //
  // mktme_config_cfg.discleandirupdate should be set to 0x1 when 2S 2LM directory optimization feature is enabled.
  // Then CHA can sends the relevant opcodes to M2M when it receives NM_Clean indication from M2M for directory update only writes.
  //
  if (IS_DDR_2LM_OR_MIXED (Host) && (!CheckSysConfig (SYS_CONFIG_1S)) && (Setup->mem.EliminateDirectoryInFarMemory == ELIMINATE_DIRECTORY_IN_FM_ENABLE)) {
    for (ChaId = 0; ChaId < TotalCha; ChaId++) {
      ChaSetDisCleanDirUpdate (SocketId, ChaId, 0x1);
    }
  }
}

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
  UINT8           ScktId;

  for (ScktId = 0; ScktId < MAX_SOCKET; ScktId++) {
    if (CheckSocketEnabled (ScktId) == 0) {
      continue;
    }
    WriteNvDimmMailboxCsrs (Host, ScktId);
    WriteMktmeCsrs (Host, MemMapData, ScktId);
    WriteTwoWayNmCacheCsrs (ScktId);
    WriteEarlyCmpCsrs (ScktId);
    Write2lmDirOptCsr (Host, ScktId);
  }
#endif // DDRT_SUPPORT

  ConfigureCxlMemDevices (MemMapData);

  return;
}

#ifdef DDRT_SUPPORT
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
  return;
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
  return;
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
  UINT8         ChId;
  UINT8         McId;
  UINT8         MaxImc;
  UINT8         NumChPerMc;

  MaxImc = GetMaxImc ();
  NumChPerMc = GetNumChannelPerMc ();

  for (McId = 0; McId < MaxImc; McId++) {
    if (!CheckSocketMcEnabled (SocketId, McId)) {
      continue;
    }

    //
    // Disable critical chunk support directly since this support was only possible
    // for 1LM x4 SDDC mode. Now this mode will use the enhanced SDDC ECC for SPR.
    //
    SetM2mDefeatures0DiscritChunk (SocketId, McId, TRUE);
    for (ChId = 0; ChId < NumChPerMc; ChId++) {
      if (!IsChannelEnabled (SocketId, ChId + (McId * NumChPerMc))) {
        continue;
      }

      McSetWaitForBothHalves (SocketId, McId, ChId);
    }
  }
 }

/**
  This function writes CSRs required by 2lm FM related silicon workaround.

  @param[in] MemMapData  Pointer to memory map private data structure.
  @param[in] SocketId    Socket index.
  @param[in] McId        Memory controller index.
  @param[in] ChId        Channel index on memory controller.

  @retval N/A
**/
VOID
Write2lmFmWorkaroundCsrs (
  IN MEMORY_MAP_DATA  *MemMapData,
  IN UINT8            SocketId,
  IN UINT8            McId,
  IN UINT8            ChId
  )
{
  UINT8               TadId;
  UINT8               MaxChPerMc;

  //
  // logic below is for DDR only
  //
  if ((GetVolMemMode () == VOL_MEM_MODE_1LM) ||
    (MemMapData->MemMapSocket[SocketId].XTile2lm) ||
    (GetCacheMemType () == CACHE_TYPE_HBM_CACHE_DDR)) {
    return;  // This W/A is required when 2LM is enabled in an in-tile configuration.
  }

  //
  // UEFI FW must program fm_target to 0 for all instances of the DRAM_RULE_CFG register,
  // when the DCPMM is populated on either MC1, CH1 or MC3, CH1.
  //
  MaxChPerMc = GetNumChannelPerMc ();
  if (IsDcpmmPresentChannel (SocketId, 1 + (1 * MaxChPerMc)) || IsDcpmmPresentChannel (SocketId, 1 + (3 * MaxChPerMc))) {
    for (TadId = 0; TadId < TAD_RULES; TadId++) {
      McClearFmTarget (SocketId, McId, ChId, TadId);
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
  return SUCCESS;
}

/**
  This function programs 10nm wave 3 specific CSRs that are suggested as workarounds.

  @param[in] Host         Pointer to sysHost.
  @param[in] MemMapData   Pointer to memory map private data structure.
  @param[in] ScktId       Socket index.

  @retval N/A
**/
VOID
WriteWorkAroundCsrs (
  IN SYSHOST            *Host,
  IN MEMORY_MAP_DATA    *MemMapData,
  IN UINT8              ScktId
  )
{
  UINT8       McId;
  UINT8       ChId;
  UINT8       MaxImc;
  UINT8       NumChPerMc;

  MaxImc = GetMaxImc ();
  NumChPerMc = GetNumChannelPerMc ();

  for (McId = 0; McId < MaxImc; McId++) {
    if (!CheckSocketMcEnabled (ScktId, McId)) {
      continue;
    }

    for (ChId = 0; ChId < NumChPerMc; ChId++) {
      if (!IsChannelEnabled (ScktId, ChId + (McId * NumChPerMc))) {
        continue;
      }

      //
      // S1409873101: Enable metadata ECC correction by default.
      //
      if (IsSiliconWorkaroundEnabled ("S1409873101")) {
        McSetMetaEccCorrBit (ScktId, McId, ChId);
      }

      //
      // S14010073297: Far channel ID calculation for in tile 2LM is incorrect in m2mem
      //
      if (IsSiliconWorkaroundEnabled ("S14010073297")) {
        Write2lmFmWorkaroundCsrs (MemMapData, ScktId, McId, ChId);
      }
    }

    //
    // S14010455369: 2LM with KTI prefetch in 2S is broken
    // MemDecode workaround gets applied for multi socket 2LM in-tile only configurations
    //
    if (IS_DDR_2LM_OR_MIXED (Host) && !(MemMapData->MemMapSocket[ScktId].XTile2lm)) {
      if (IsSiliconWorkaroundEnabled ("S14010455369") && !CheckSysConfig (SYS_CONFIG_1S)) {
        SetM2mSysFeatures0FarMcMode (ScktId, McId, 0);
      }
    }
  }

  //
  // S2209269055: ADR - Control bit for NM flush actions in MC missing.
  //
  if (IsSiliconWorkaroundEnabled ("S2209269055")) {
    WriteFadrWorkaroundCsrs (Host, MemMapData, ScktId);
  }

  //
  // S14011507287: X-Tile 2way 2LM hung transactions due to entry not getting de-allocated in MC scoreboard.
  // This workaround needs to be applied for all X-Tile 2LM configurations
  //
  if (IS_DDR_2LM_OR_MIXED (Host) && MemMapData->MemMapSocket[ScktId].XTile2lm) {
    if (IsSiliconWorkaroundEnabled ("S14011507287")) {
      //
      // Disable local MemInvItoX in CHA.
      //
      ChaSetLocMemInvItoX (ScktId, TRUE);
      for (McId = 0; McId < MaxImc; McId++) {
        if (!CheckSocketMcEnabled (ScktId, McId)) {
          continue;
        }
        //
        // Disable Wcil optimization in Mesh2Mem.
        //
        SetM2mDefeatures0WcilOptDis (ScktId, McId, TRUE);
        for (ChId = 0; ChId < NumChPerMc; ChId++) {
          if (!IsChannelEnabled (ScktId, ChId + (McId * NumChPerMc))) {
            continue;
          }
          //
          // Disable Wcilf optimization in MC.
          //
          McSetNmCachingCfgWcilfOptDis (ScktId, ChId + (McId * NumChPerMc), TRUE);
        }
      }
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
  UINT8               SadCount[MAX_DRAM_CLUSTERS];
  UINT8               ClusterId;
  UINT8               SadIndex;
  UINT8               BeginSadIndex;
  UINT8               EndSadIndex;
  UINT8               HighestDramRuleUsed;
  SOCKET_INFO_STRUCT  *Socket;

  //
  // Check SNC is disabled or multicast SNC is unsupported
  //
  if (!IfFeatureEnabled (TypeSnc) || MemMapData->NumberofChaDramClusters == 1) {
    return SUCCESS;
  }

  HighestDramRuleUsed = 0;

  //
  // First count the SAD rules currently in use per DRAM cluster so that we can indentify when we run out
  // of available SAD rules.
  //
  ZeroMem (SadCount, sizeof (SadCount));
  for (ClusterId = 0; ClusterId < MemMapData->NumberofChaDramClusters; ClusterId++) {
    BeginSadIndex = (ClusterId * MemMapData->MaxSadRules);
    EndSadIndex = (BeginSadIndex + MemMapData->MaxSadRules);
    //
    // Find the next available Sad Index in the DRAM cluster and adjust the SadIndex local to that DRAM cluster
    //
    SadCount[ClusterId] = FindSadIndex (Host, MemMapData, SocketNum, ClusterId);
    if (SadCount[ClusterId] >= EndSadIndex) {
      return FAILURE;
    }
    SadCount[ClusterId] = (SadCount[ClusterId] % MemMapData->MaxSadRules);
    //
    // Get the highest SadIndex used across all the DRAM clusters in a socket.
    //
    if (SadCount[ClusterId] > HighestDramRuleUsed) {
      HighestDramRuleUsed = SadCount[ClusterId];
    }
  }

  //
  // After counting the number of enabled SAD's per cluster, duplicate the last enabled SAD entry into NXM(s)
  // up to HighestDramRuleUsed and balance DRAM rules used across clusters.
  //
  Socket = &Host->var.mem.socket[SocketNum];
  for (ClusterId = 0; ClusterId < MemMapData->NumberofChaDramClusters; ClusterId++) {
    //
    // If this cluster has the highest DRAM rule index already populated then skip it
    //
    if (SadCount[ClusterId] == HighestDramRuleUsed) {
      continue;
    }
    //
    // Create NXM rule(s) from next available SAD index within a cluster up to the highest DRAM rule index across
    // all clusters.
    //
    BeginSadIndex = (ClusterId * MemMapData->MaxSadRules) + SadCount[ClusterId];
    EndSadIndex = (BeginSadIndex + (HighestDramRuleUsed - SadCount[ClusterId]));
    for (SadIndex = BeginSadIndex; SadIndex < EndSadIndex; SadIndex++) {
      Socket->SAD[SadIndex].Enable = 1;
      Socket->SAD[SadIndex].type = MemTypeNxm;
      Socket->SAD[SadIndex].local = 0;
      Socket->SAD[SadIndex].Base = Socket->SAD[SadIndex - 1].Limit;
      Socket->SAD[SadIndex].Limit = Socket->SAD[SadIndex - 1].Limit;
      Socket->SAD[SadIndex].Cluster = ClusterId;
    }
  }

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
  } else if (IsHbmInQuadrantMode (MemMapData, Sckt)) {
    return UMABASEDCLUSTERING_QUADRANT;
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
  if (IfFeatureEnabled (TypeSnc)) {
    return GetNumOfClusterPerSystem ();
  } else {
    return 1;
  }
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

  if (!IsDdr5Present (Host, Socket)) {
    RankList = GetRankNvList (Host, Socket, Ch, Dimm);
    return (*RankList)[Rank].phyRank;
  } else {
    //
    // For DDR5 DIMM with PCH support, the desired RIR rank target should
    // be retrieved based on the following mapping rank instead of physical rank.
    //
    // CS  Dimm  Rank phyRank rir_rnk_tgt0
    //  0  0     0    0       0
    //  2  0     1    1       2
    //  4  1     0    4       4
    //  6  1     1    5       6
    //
    return Dimm * 4 + Rank * 2;
  }
}

/**

  set MC.AMAP and force latency on Wave 1 and Wave 2 platforms if PMem present

  @param[in] Socket     Socket number.
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
  return;
}

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

  if (IsDdr5MemSsPresent (Host, Sckt)) {
    return InValue / 2;
  }

  return InValue;
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
  UINT8                     ChId;
  UINT8                     DimmId;
  UINT8                     MaxChPerMc;
  UINT8                     McId;
  UINT8                     MaxImc;
  UINT8                     SadId;
  UINT16                    FzmCapabilityVector;
  SYS_SETUP                 *Setup;
  SAD_TABLE                 *SadEntry;
  MEMORY_MAP_HOST           *MemMapHost;
  FEATURE_STATUS_REGISTER   FeatureStatusReg;
  BOOLEAN                   IsCpuidBzmEn;

  //
  // Boot-time fast zero memory is not supported for entire system, if
  //   1. The setup policy of fast zero memory is disabled.
  //   2. NUMA interleaving is disabled.
  //   3. CPUID.7.ECX[Bit15(FZM)] reports fast zero memory is not supported.
  //
  Setup = GetSysSetupPointer ();
  IsCpuidBzmEn = (Host->var.common.procCom[SocketId].ExtCpuid7.Ecx & BZM_CPUID_BIT) == 0 ? FALSE : TRUE;
  if ((Setup->mem.FastZeroMemSupport != FAST_ZERO_MEM_ENABLED) || (!MemMapData->IsNumaEn) || (!IsCpuidBzmEn)) {
    RcDebugPrint (SDBG_DEFAULT, "\nBZM is disabled, FastZeroMemSupport = 0x%x, IsNumaEn = 0x%x, IsCpuidBzmEn = 0x%x\n",
      Setup->mem.FastZeroMemSupport,
      MemMapData->IsNumaEn,
      IsCpuidBzmEn
      );
    return;
  }

  MaxImc = GetMaxImc ();
  MaxChPerMc = GetNumChannelPerMc ();
  MemMapHost = GetMemMapHostPointer ();
  FzmCapabilityVector = 0;

  for (SadId = BeginSadId; SadId < (BeginSadId + MemMapData->MaxSadRules); SadId++) {
    SadEntry = GetSadTableEntry (SocketId, SadId);
    if (SadEntry->Enable == 0) {
      break;  // Break if we have reached the end of the SAD table.
    }

    //
    // Skip if this is not a local SAD entry.
    // Memory region below 4GB cannot be enabled as boot-time fast zero memory region.
    //
    if ((!SadEntry->local) || (SadEntry->Limit == SAD_4GB)) {
      continue;
    }

    switch (SadEntry->type) {

    //
    // Boot-time fast zero memory is supported on the 1LM or HBM-cached DDR memory regions zeroed out by MRC.
    //
    case MemType1lmDdr:
    case MemType2lmHbmCacheDdr:
      if (IsDdrMemInitDone ()) {
        FzmCapabilityVector |= (BIT0 << (SadId - BeginSadId));
      }
      break;

    //
    // Boot-time fast zero memory is supported on the 2LM memory mode regions unless any of the DCPMM
    // DIMMs interleaved in these regions don't enable the BZM capability on the current boot.
    //
    case MemType2lmDdrCacheMemoryMode:
    case MemType2lmHbmCacheMemoryMode:
      for (McId = 0; McId < MaxImc; McId++) {
        if ((SadEntry->imcInterBitmap & (BIT0 << McId)) == 0) {
          continue;
        }
        for (ChId = 0; ChId < MaxChPerMc; ChId++) {
          if ((SadEntry->FMchannelInterBitmap[McId] & (BIT0 << ChId)) == 0) {
            continue;
          }
          for (DimmId = 0; DimmId < MAX_DIMM; DimmId++) {
            if ((!IsDimmPresent (SocketId, ChId, DimmId)) || (!IsDcpmmPresentDimm (SocketId, ChId, DimmId))) {
              continue;
            }
            //
            // The boot-time fast zero memory is supported on this dimm, if
            //   1. This dimm is CPS dimm or next generations of DCPMM dimm.
            //   2. The BZM capability is enabled on the FMC of this dimm.
            //
            ReadFeatureStatusRegister (Host, SocketId, ChId, DimmId, &FeatureStatusReg);
            if (FeatureStatusReg.Bits.BootZeroMem != BZM_ENABLED) {
              break;
            }
          }
        }
      }
      FzmCapabilityVector |= (BIT0 << (SadId - BeginSadId));
      break;

    default:
      break;
    }
  }

  //
  // Save FZM enable/disable status for future consumption.
  //
  if (FzmCapabilityVector != 0) {
    MemMapHost->IsFastZeroMemEn = TRUE;
  }

  //
  // Write FZM_CAPABILITY register to enumerate memory regions that are fast-zero capable.
  //
  RcDebugPrint (SDBG_MINMAX, "\nProgram FZM capability: CapabilityVector = 0x%04x\n", FzmCapabilityVector);
  ChaSetFzmCapability (SocketId, FzmCapabilityVector);
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

  PostMemMapSgxData (MemMapData);
  PublishCxlNodeHob (MemMapData);

  Status = MemReservePsmiBuffers (Host, MemMapData);
  if (Status != EFI_SUCCESS) {
    RcDebugPrint (SDBG_MAX, "\n Memory could not be reserved for PSMI buffers \n");
  }

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

  if ((GetCacheMemType () == CACHE_TYPE_DDR_CACHE_PMEM) &&
    (VolMemMode == VOL_MEM_MODE_2LM || VolMemMode == VOL_MEM_MODE_MIX_1LM2LM)) {
    if (Host->var.mem.imcInter == IMC_1WAY && Host->var.mem.chInter == CH_1WAY) {
      Host->var.mem.imcInter = MaxImc;
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
  return SUCCESS;
}
