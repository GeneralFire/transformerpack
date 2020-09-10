/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#include <Library/MemMcIpLib.h>
#include "Include/MemMcRegs.h"
#include <Library/KtiApi.h>
#include <Library/MemRcLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemAccessLib.h>
#include <MemDecodeCommonIncludes.h>
#include "Include/MemMcIpLibInternal.h"
#include <UncoreCommonIncludes.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/MemTypeLib.h>
#include <Library/RcDebugLib.h>
#include <Library/SiliconWorkaroundLib.h>

UINT32 mTadChnIlvOffsetN0[MAX_TAD_RULES_10NM] = {
  TADCHNILVOFFSET_0_N0_MC_MAIN_REG, TADCHNILVOFFSET_1_N0_MC_MAIN_REG, TADCHNILVOFFSET_2_N0_MC_MAIN_REG, TADCHNILVOFFSET_3_N0_MC_MAIN_REG,
  TADCHNILVOFFSET_4_N0_MC_MAIN_REG, TADCHNILVOFFSET_5_N0_MC_MAIN_REG, TADCHNILVOFFSET_6_N0_MC_MAIN_REG, TADCHNILVOFFSET_7_N0_MC_MAIN_REG,
  TADCHNILVOFFSET_8_N0_MC_MAIN_REG, TADCHNILVOFFSET_9_N0_MC_MAIN_REG, TADCHNILVOFFSET_10_N0_MC_MAIN_REG, TADCHNILVOFFSET_11_N0_MC_MAIN_REG
};

UINT32 mTadChnIlvOffsetN1[MAX_TAD_RULES_10NM] = {
  TADCHNILVOFFSET_0_N1_MC_MAIN_REG, TADCHNILVOFFSET_1_N1_MC_MAIN_REG, TADCHNILVOFFSET_2_N1_MC_MAIN_REG, TADCHNILVOFFSET_3_N1_MC_MAIN_REG,
  TADCHNILVOFFSET_4_N1_MC_MAIN_REG, TADCHNILVOFFSET_5_N1_MC_MAIN_REG, TADCHNILVOFFSET_6_N1_MC_MAIN_REG, TADCHNILVOFFSET_7_N1_MC_MAIN_REG,
  TADCHNILVOFFSET_8_N1_MC_MAIN_REG, TADCHNILVOFFSET_9_N1_MC_MAIN_REG, TADCHNILVOFFSET_10_N1_MC_MAIN_REG, TADCHNILVOFFSET_11_N1_MC_MAIN_REG
};

UINT32 mTadWaynessOffset[TAD_RULES] = {
  TADWAYNESS_0_MC_MAIN_REG, TADWAYNESS_1_MC_MAIN_REG, TADWAYNESS_2_MC_MAIN_REG, TADWAYNESS_3_MC_MAIN_REG,
  TADWAYNESS_4_MC_MAIN_REG, TADWAYNESS_5_MC_MAIN_REG, TADWAYNESS_6_MC_MAIN_REG, TADWAYNESS_7_MC_MAIN_REG
};

UINT32 mTadBaseOffset[TAD_RULES] = {
  TADBASE_0_MC_MAIN_REG, TADBASE_1_MC_MAIN_REG, TADBASE_2_MC_MAIN_REG, TADBASE_3_MC_MAIN_REG,
  TADBASE_4_MC_MAIN_REG, TADBASE_5_MC_MAIN_REG, TADBASE_6_MC_MAIN_REG, TADBASE_7_MC_MAIN_REG
};

UINT32 mNmDramRuleCfgN0[TAD_RULES] = {
  NM_DRAM_RULE_CFG0_N0_MC_MAIN_REG, NM_DRAM_RULE_CFG1_N0_MC_MAIN_REG, NM_DRAM_RULE_CFG2_N0_MC_MAIN_REG, NM_DRAM_RULE_CFG3_N0_MC_MAIN_REG,
  NM_DRAM_RULE_CFG4_N0_MC_MAIN_REG, NM_DRAM_RULE_CFG5_N0_MC_MAIN_REG, NM_DRAM_RULE_CFG6_N0_MC_MAIN_REG, NM_DRAM_RULE_CFG7_N0_MC_MAIN_REG
};

UINT32 mNmDramRuleCfgN1[TAD_RULES] = {
  NM_DRAM_RULE_CFG0_N1_MC_MAIN_REG, NM_DRAM_RULE_CFG1_N1_MC_MAIN_REG, NM_DRAM_RULE_CFG2_N1_MC_MAIN_REG, NM_DRAM_RULE_CFG3_N1_MC_MAIN_REG,
  NM_DRAM_RULE_CFG4_N1_MC_MAIN_REG, NM_DRAM_RULE_CFG5_N1_MC_MAIN_REG, NM_DRAM_RULE_CFG6_N1_MC_MAIN_REG, NM_DRAM_RULE_CFG7_N1_MC_MAIN_REG
};

UINT32 mMcDramRuleCfgN0[TAD_RULES] = {
  DRAM_RULE_CFG0_N0_MC_MAIN_REG, DRAM_RULE_CFG1_N0_MC_MAIN_REG, DRAM_RULE_CFG2_N0_MC_MAIN_REG, DRAM_RULE_CFG3_N0_MC_MAIN_REG,
  DRAM_RULE_CFG4_N0_MC_MAIN_REG, DRAM_RULE_CFG5_N0_MC_MAIN_REG, DRAM_RULE_CFG6_N0_MC_MAIN_REG, DRAM_RULE_CFG7_N0_MC_MAIN_REG
};

UINT32 mMcDramRuleCfgN1[TAD_RULES] = {
  DRAM_RULE_CFG0_N1_MC_MAIN_REG, DRAM_RULE_CFG1_N1_MC_MAIN_REG, DRAM_RULE_CFG2_N1_MC_MAIN_REG, DRAM_RULE_CFG3_N1_MC_MAIN_REG,
  DRAM_RULE_CFG4_N1_MC_MAIN_REG, DRAM_RULE_CFG5_N1_MC_MAIN_REG, DRAM_RULE_CFG6_N1_MC_MAIN_REG, DRAM_RULE_CFG7_N1_MC_MAIN_REG
};

UINT32 mRIRWaynessLimit[MAX_RIR] =
{ RIRWAYNESSLIMIT_0_MC_MAIN_REG, RIRWAYNESSLIMIT_1_MC_MAIN_REG, RIRWAYNESSLIMIT_2_MC_MAIN_REG, RIRWAYNESSLIMIT_3_MC_MAIN_REG };

UINT32 mRIRIlvOffset[MAX_RIR][MAX_RIR_WAYS] =
{ { RIRILV0OFFSET_0_MC_MAIN_REG, RIRILV1OFFSET_0_MC_MAIN_REG, RIRILV2OFFSET_0_MC_MAIN_REG, RIRILV3OFFSET_0_MC_MAIN_REG, RIRILV4OFFSET_0_MC_MAIN_REG, RIRILV5OFFSET_0_MC_MAIN_REG, RIRILV6OFFSET_0_MC_MAIN_REG, RIRILV7OFFSET_0_MC_MAIN_REG },
{ RIRILV0OFFSET_1_MC_MAIN_REG, RIRILV1OFFSET_1_MC_MAIN_REG, RIRILV2OFFSET_1_MC_MAIN_REG, RIRILV3OFFSET_1_MC_MAIN_REG, RIRILV4OFFSET_1_MC_MAIN_REG, RIRILV5OFFSET_1_MC_MAIN_REG, RIRILV6OFFSET_1_MC_MAIN_REG, RIRILV7OFFSET_1_MC_MAIN_REG },
{ RIRILV0OFFSET_2_MC_MAIN_REG, RIRILV1OFFSET_2_MC_MAIN_REG, RIRILV2OFFSET_2_MC_MAIN_REG, RIRILV3OFFSET_2_MC_MAIN_REG, RIRILV4OFFSET_2_MC_MAIN_REG, RIRILV5OFFSET_2_MC_MAIN_REG, RIRILV6OFFSET_2_MC_MAIN_REG, RIRILV7OFFSET_2_MC_MAIN_REG },
{ RIRILV0OFFSET_3_MC_MAIN_REG, RIRILV1OFFSET_3_MC_MAIN_REG, RIRILV2OFFSET_3_MC_MAIN_REG, RIRILV3OFFSET_3_MC_MAIN_REG, RIRILV4OFFSET_3_MC_MAIN_REG, RIRILV5OFFSET_3_MC_MAIN_REG, RIRILV6OFFSET_3_MC_MAIN_REG, RIRILV7OFFSET_3_MC_MAIN_REG } };

UINT32 mFMRIRWaynessLimit[MAX_RIR_DDRT] =
{ FMRIRWAYNESSLIMIT_0_MC_2LM_REG, FMRIRWAYNESSLIMIT_1_MC_2LM_REG, FMRIRWAYNESSLIMIT_2_MC_2LM_REG, FMRIRWAYNESSLIMIT_3_MC_2LM_REG };

UINT32 mFMRIRIlvOffset[MAX_RIR_DDRT][MAX_RIR_DDRT_WAYS] =
{ { FMRIRILV0OFFSET_0_MC_2LM_REG, FMRIRILV1OFFSET_0_MC_2LM_REG },
{ FMRIRILV0OFFSET_1_MC_2LM_REG, FMRIRILV1OFFSET_1_MC_2LM_REG },
{ FMRIRILV0OFFSET_2_MC_2LM_REG, FMRIRILV1OFFSET_2_MC_2LM_REG },
{ FMRIRILV0OFFSET_3_MC_2LM_REG, FMRIRILV1OFFSET_3_MC_2LM_REG } };

/**
  Enable WPQ Flush on all channels on the input MC

  @param [in] Sckt      - socket index
  @param [in] Mc        - MC on socket

  @retval             - None
 */
VOID
EnableMcWpqFlush (
  IN UINT8   Sckt,
  IN UINT8   Mc
  )
{
  UINT8                            Ch;
  DDRT_WPQ_FLUSH_CTL_MC_2LM_STRUCT DdrtWpqFlushCtl;

  //
  // Set DDRT_WPQ_FLUSH_CTL.wpqflush_enable = 1 on all channels on the input MC
  //
  for (Ch = NODECH_TO_SKTCH (Mc, 0); Ch < NODECH_TO_SKTCH (Mc, GetNumChannelPerMc ()); Ch++) {
    DdrtWpqFlushCtl.Data = MemReadPciCfgEp (Sckt, Ch, DDRT_WPQ_FLUSH_CTL_MC_2LM_REG);
    DdrtWpqFlushCtl.Bits.wpqflush_enable = 1;
    MemWritePciCfgEp (Sckt, Ch, DDRT_WPQ_FLUSH_CTL_MC_2LM_REG, DdrtWpqFlushCtl.Data);
  }
}

/**
  Programs MC 1LM registers for a specified TAD rule.

  @param [in] MemTechType            Memory technology type.
  @param [in] SocketId               Socket number ID (0 should always be the SBSP).
  @param [in] McIndex                Index of the memory controller on the socket.
  @param [in] ChIndex                Index of the channel on the memory controller.
  @param [in] TadIndex               The index of the TAD entry.
  @param [in] TadData                Information required to define a TAD rule region.

  @retval EFI_SUCCESS                The function is executed successfully without any error
  @retval EFI_INVALID_PARAMETER      The function failed due to some illegal input.

**/
EFI_STATUS
EFIAPI
McSet1lmTad (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          SocketId,
  IN UINT8          McIndex,
  IN UINT8          ChIndex,
  IN UINT8          TadIndex,
  IN TAD_1LM_DATA   *TadData
  )
{
  UINT8                                         NumChPerMc;
  TADCHNILVOFFSET_0_N0_MC_MAIN_STRUCT           TadChnIlvData0;
  TADCHNILVOFFSET_0_N1_MC_MAIN_STRUCT           TadChnIlvData1;
  INT32                                         TadOffset;
  UINT8                                         TadOffsetSign;
  UINT8                                         MaxImc;

  if (MemTechType == MemTechHbm) {
    NumChPerMc = GetNumChannelPerMcHbm ();
    MaxImc = GetMaxImcPerIoHbm () * GetMaxIoHbm ();
  } else {
    NumChPerMc = GetNumChannelPerMc ();
    MaxImc = GetMaxImc ();
  }

  if ((McIndex >= MaxImc) || (ChIndex >= NumChPerMc) || (TadIndex >= TAD_RULES) || (TadData == NULL)) {
    return EFI_INVALID_PARAMETER;
  }


  TadChnIlvData0.Data = 0;
  TadChnIlvData1.Data = 0;

  //
  // a. Program MCDECS_CRNODE_CH{#CH}_MC{#MC}_CR_TADCHNILVOFFSET[8:0]
  // Used for 1LM DDR4, for DDRT PMEM, Block and 2LM FM
  //
  TadChnIlvData0.Bits.target_ways     = TadData->TadWaysGran.EncTgtWays;
  TadChnIlvData0.Bits.target_gran     = TadData->TadWaysGran.EncTgtGran;
  TadChnIlvData0.Bits.chn_ways        = TadData->TadWaysGran.EncChWays;
  TadChnIlvData0.Bits.chn_gran        = TadData->TadWaysGran.EncChGran;

  //
  // If TadOffset is a negative 2's complement integer, convert to a positive
  // number and set the TadOffsetSign to subtract the offset.
  //
  if (TadData->TadOffset < 0) {
    TadOffset = -TadData->TadOffset;
    TadOffsetSign = 1;
  } else {
    TadOffset = TadData->TadOffset;
    TadOffsetSign = 0;
  }
  TadChnIlvData0.Bits.tad_offset      = TadOffset & 0x3FFFF; //Bits [17:0]
  TadChnIlvData1.Bits.tad_offset      = (TadOffset >> 18) & 0xFF; //Bits [25:18]
  TadChnIlvData1.Bits.tad_offset_sign = TadOffsetSign;

  ChRegisterWrite (
    MemTechType,
    SocketId,
    ChIndex + (McIndex * NumChPerMc),
    mTadChnIlvOffsetN0[TadIndex],
    TadChnIlvData0.Data
    );

  ChRegisterWrite (
    MemTechType,
    SocketId,
    ChIndex + (McIndex * NumChPerMc),
    mTadChnIlvOffsetN1[TadIndex],
    TadChnIlvData1.Data
    );

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, ChIndex, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    " TADCHNILVOFFSET[%d]:\n"
    "  target_ways: %d  chn_ways: %d  target_gran: %d  chn_gran: %d  tad_offset: 0x%x  tad_offset_sign: %d\n",
    TadIndex,
    TadChnIlvData0.Bits.target_ways,
    TadChnIlvData0.Bits.chn_ways,
    TadChnIlvData0.Bits.target_gran,
    TadChnIlvData0.Bits.chn_gran,
    (TadChnIlvData1.Bits.tad_offset << 18) | TadChnIlvData0.Bits.tad_offset,
    TadChnIlvData1.Bits.tad_offset_sign
    );

  return EFI_SUCCESS;
}

/**
  Programs 2LM TAD bits in MC near memory cache configuration.

  @param [in] MemTechType            Memory technology type.
  @param [in] SocketId               Socket number ID (0 should always be the SBSP).
  @param [in] McIndex                Index of the memory controller on the socket.
  @param [in] TadIndex2lm            Index of the TAD entry for 2LM NM.

  @retval EFI_SUCCESS                The function is executed successfully without any error
  @retval EFI_INVALID_PARAMETER      The function failed due to some illegal input.

**/
EFI_STATUS
EFIAPI
McSetNmCachingTad (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          SocketId,
  IN UINT8          McIndex,
  IN UINT8          TadIndex2lm
  )
{
  UINT8                                         ChIndex;
  UINT8                                         NumChPerMc;
  MCNMCACHINGCFG_MC_MAIN_STRUCT                 McNmCachCfg;
  UINT8                                         MaxImc;

  if (MemTechType == MemTechHbm) {
    NumChPerMc = GetNumChannelPerMcHbm ();
    MaxImc = GetMaxImcPerIoHbm () * GetMaxIoHbm ();
  } else {
    NumChPerMc = GetNumChannelPerMc ();
    MaxImc = GetMaxImc ();
  }

  if (McIndex >= MaxImc) {
    return EFI_INVALID_PARAMETER;
  }

  for (ChIndex = 0; ChIndex < NumChPerMc; ChIndex++) {

    McNmCachCfg.Data = ChRegisterRead (
      MemTechType,
      SocketId,
      ChIndex + (McIndex * NumChPerMc),
      MCNMCACHINGCFG_MC_MAIN_REG
      );

    //
    // Set the 2LM TAD fields
    //
    McNmCachCfg.Bits.nmcachingtaden = 1;
    McNmCachCfg.Bits.nmcachingtad = TadIndex2lm;

    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, ChIndex, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      " MCNMCACHINGCFG 2LM TAD:\n"
      "  nmcachingtaden: %d  nmcachingtad: %d\n",
      McNmCachCfg.Bits.nmcachingtaden,
      McNmCachCfg.Bits.nmcachingtad
      );

    ChRegisterWrite (
      MemTechType,
      SocketId,
      ChIndex + (McIndex * NumChPerMc),
      MCNMCACHINGCFG_MC_MAIN_REG,
      McNmCachCfg.Data
      );
  }
  return EFI_SUCCESS;
}

/**
  Programs MC near memory cache configuration to enabled memory modes for M2Mem traffic.

  @param [in] MemTechType            Memory technology type.
  @param [in] SocketId               Socket number ID (0 should always be the SBSP).
  @param [in] McIndex                Index of the memory controller on the socket.
  @param [in] IsNmTile               Indicates whether MC tile is populated with 2LM NM.

  @retval EFI_SUCCESS                The function is executed successfully without any error
  @retval EFI_INVALID_PARAMETER      The function failed due to some illegal input.

**/
EFI_STATUS
EFIAPI
McSetNmCachingCfg (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          SocketId,
  IN UINT8          McIndex,
  IN BOOLEAN        IsNmTile
  )
{
  UINT8                                         ChIndex;
  UINT8                                         NumChPerMc;
  MCNMCACHINGCFG_MC_MAIN_STRUCT                 McNmCachCfg;
  UINT8                                         MaxImc;

  if (MemTechType == MemTechHbm) {
    NumChPerMc = GetNumChannelPerMcHbm ();
    MaxImc = GetMaxImcPerIoHbm () * GetMaxIoHbm ();
  } else {
    NumChPerMc = GetNumChannelPerMc ();
    MaxImc = GetMaxImc ();
  }

  if (McIndex >= MaxImc) {
    return EFI_INVALID_PARAMETER;
  }

  for (ChIndex = 0; ChIndex < NumChPerMc; ChIndex++) {

    McNmCachCfg.Data = ChRegisterRead (
      MemTechType,
      SocketId,
      ChIndex + (McIndex * NumChPerMc),
      MCNMCACHINGCFG_MC_MAIN_REG
      );

    //
    // McNmCachingEnb enables the 2LM feature. It is required for m2mem traffic in 2LM mode to work.
    //
    if (IsNmTile) {
      McNmCachCfg.Bits.mcnmcachingenb = 1;
    }

    //
    // Set only if DDRT DIMM on channel for 1LM access to meta data region.
    //
    if ((MemTechType == MemTechDdr) && AepDimmPresent (SocketId, ChIndex + (McIndex * NumChPerMc), NULL)) {
      McNmCachCfg.Bits.mcpmemenb = 1;
    }

    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, ChIndex, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      " MCNMCACHINGCFG:\n"
      "  mcnmcachingenb: %d  mcpmemenb: %d  mcnmcachingpmemwt: %d\n",
      McNmCachCfg.Bits.mcnmcachingenb,
      McNmCachCfg.Bits.mcpmemenb,
      McNmCachCfg.Bits.mcnmcachingpmemwt
      );

    ChRegisterWrite (
      MemTechType,
      SocketId,
      ChIndex + (McIndex * NumChPerMc),
      MCNMCACHINGCFG_MC_MAIN_REG,
      McNmCachCfg.Data
      );
  }
  return EFI_SUCCESS;
}

/**
  Programs MC force_lat bit for a channel.

  @param [in] MemTechType            Memory technology type.
  @param [in] SocketId               Socket number ID (0 should always be the SBSP).
  @param [in] McIndex                Index of the memory controller on the socket.
  @param [in] ChIndex                Index of the channel on the memory controller.

  @retval EFI_SUCCESS                The function is executed successfully without any error
  @retval EFI_INVALID_PARAMETER      The function failed due to some illegal input.

**/
EFI_STATUS
EFIAPI
McSetForceLatency (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          SocketId,
  IN UINT8          McIndex,
  IN UINT8          ChIndex
  )
{
  AMAP_MC_MAIN_STRUCT      Amap;
  UINT8                    NumChPerMc;
  UINT8                    MaxImc;

  if (MemTechType == MemTechHbm) {
    NumChPerMc = GetNumChannelPerMcHbm ();
    MaxImc = GetMaxImcPerIoHbm () * GetMaxIoHbm ();
  } else {
    NumChPerMc = GetNumChannelPerMc ();
    MaxImc = GetMaxImc ();
  }

  if ((McIndex >= MaxImc) || (ChIndex >= NumChPerMc)) {
    return EFI_INVALID_PARAMETER;
  }

  Amap.Data = ChRegisterRead (
    MemTechType,
    SocketId,
    ChIndex + (McIndex * NumChPerMc),
    AMAP_MC_MAIN_REG
    );

  if (Amap.Bits.force_lat != 1) {
    Amap.Bits.force_lat = 1;
    ChRegisterWrite (
      MemTechType,
      SocketId,
      ChIndex + (McIndex * NumChPerMc),
      AMAP_MC_MAIN_REG,
      Amap.Data
      );
  }

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, ChIndex, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "CSR for force_lat bit : AMAP_MC_MAIN_REG: 0x%x\n", Amap.Data);

  return EFI_SUCCESS;
}

/**
  Programs MC wait4bothhalves bit for a channel.

  @param [in] SocketId               Socket number ID (0 should always be the SBSP).
  @param [in] McIndex                Index of the memory controller on the socket.
  @param [in] ChIndex                Index of the channel on the memory controller.

  @retval EFI_SUCCESS                The function is executed successfully without any error
  @retval EFI_INVALID_PARAMETER      The function failed due to some illegal input.

**/
EFI_STATUS
EFIAPI
McSetWaitForBothHalves (
  IN UINT8  SocketId,
  IN UINT8  McIndex,
  IN UINT8  ChIndex
  )
{
  AMAP_MC_MAIN_STRUCT      Amap;
  UINT8                    NumChPerMc;
  UINT8                    MaxImc;

  NumChPerMc = GetNumChannelPerMc ();
  MaxImc     = GetMaxImc ();

  if ((McIndex >= MaxImc) || (ChIndex >= NumChPerMc)) {
    return EFI_INVALID_PARAMETER;
  }

  Amap.Data = MemReadPciCfgEp (
    SocketId,
    ChIndex + (McIndex * NumChPerMc),
    AMAP_MC_MAIN_REG
    );

  if (Amap.Bits.wait4bothhalves != 1) {
    Amap.Bits.wait4bothhalves = 1;
    MemWritePciCfgEp (
      SocketId,
      ChIndex + (McIndex * NumChPerMc),
      AMAP_MC_MAIN_REG,
      Amap.Data
      );
  }

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, ChIndex, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "CSR for wait4bothhalves bit: AMAP_MC_MAIN_REG: 0x%x\n", Amap.Data);

  return EFI_SUCCESS;
}

/**
  Programs MC scratchpad register bits to support workarounds.

  @param [in] SocketId               Socket number ID (0 should always be the SBSP).
  @param [in] McIndex                Index of the memory controller on the socket.
  @param [in] ChIndex                Index of the channel on the memory controller.
  @param [in] AndMask                Bit field to AND with scratch register bits.
  @param [in] OrMask                 Bit field to OR with scratch register bits.

  @retval EFI_SUCCESS                The function is executed successfully without any error
  @retval EFI_INVALID_PARAMETER      The function failed due to some illegal input.

**/
EFI_STATUS
EFIAPI
McSetScratchBits (
  IN UINT8   SocketId,
  IN UINT8   McIndex,
  IN UINT8   ChIndex,
  IN UINT32  AndMask,
  IN UINT32  OrMask
  )
{
  SCRATCHPAD_2LMCNTL_MC_2LM_STRUCT    ScratchPad_2lmCtrl;
  UINT8                               NumChPerMc;
  UINT8                               MaxImc;

  NumChPerMc = GetNumChannelPerMc ();
  MaxImc     = GetMaxImc ();

  if ((McIndex >= MaxImc) || (ChIndex >= NumChPerMc)) {
    return EFI_INVALID_PARAMETER;
  }

  //Read the 2lm ctrl register for each channel
  ScratchPad_2lmCtrl.Data = MemReadPciCfgEp (
    SocketId,
    ChIndex + (McIndex * NumChPerMc),
    SCRATCHPAD_2LMCNTL_MC_2LM_REG
    );

  ScratchPad_2lmCtrl.Bits.bit_field |= OrMask;
  ScratchPad_2lmCtrl.Bits.bit_field &= AndMask;

  MemWritePciCfgEp (
    SocketId,
    ChIndex + (McIndex * NumChPerMc),
    SCRATCHPAD_2LMCNTL_MC_2LM_REG,
    ScratchPad_2lmCtrl.Data
    );

  RcDebugPrintWithDevice (SDBG_MAX, SocketId, ChIndex + (McIndex * NumChPerMc), NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "CSR : SCRATCHPAD_2LMCNTL_MC_2LM_REG: 0x%x\n", ScratchPad_2lmCtrl.Data);

  return EFI_SUCCESS;
}

/**
  Programs MC feature registers.

  @param [in] SocketId               Socket number ID (0 should always be the SBSP).
  @param [in] McIndex                Index of the memory controller on the socket.
  @param [in] ChIndex                Index of the channel on the memory controller.
  @param [in] FeatureBitmap          Bitmap of the features to enable on the memory controller.

  @retval EFI_SUCCESS                The function is executed successfully without any error
  @retval EFI_INVALID_PARAMETER      The function failed due to some illegal input.

**/
EFI_STATUS
EFIAPI
McSetFeatures (
  IN UINT8  SocketId,
  IN UINT8  McIndex,
  IN UINT8  ChIndex,
  IN UINT32 FeatureBitmap
  )
{
  SYSFEATURES0_MC_2LM_STRUCT    SysFeatures0_2lmCtrl;
  UINT8                         NumChPerMc;
  UINT8                         MaxImc;

  NumChPerMc = GetNumChannelPerMc ();
  MaxImc     = GetMaxImc ();

  if ((McIndex >= MaxImc) || (ChIndex >= NumChPerMc) || (FeatureBitmap == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  //Read the 2lm ctrl register for each channel
  SysFeatures0_2lmCtrl.Data = MemReadPciCfgEp (
    SocketId,
    ChIndex + (McIndex * NumChPerMc),
    SYSFEATURES0_MC_2LM_REG
    );

  //If we are in 2lm mode & in single socket config; set FrcDirI to 1.
  if ((FeatureBitmap & FEATURE_FORCE_DIR_INV) != 0) {
    SysFeatures0_2lmCtrl.Bits.frcdiri = 1;
  }

  // when directory mode is disabled, update the dirwrdisable, MC register to be consistent w/ M2MEM
  if ((FeatureBitmap & FEATURE_DIR_WR_DIS) != 0) {
    SysFeatures0_2lmCtrl.Bits.dirwrdisable = 1;
  }

  MemWritePciCfgEp (
    SocketId,
    ChIndex + (McIndex * NumChPerMc),
    SYSFEATURES0_MC_2LM_REG,
    SysFeatures0_2lmCtrl.Data
    );

  RcDebugPrintWithDevice (SDBG_MAX, SocketId, ChIndex, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "CSR : SYSFEATURES0_MC_2LM_REG: 0x%x\n", SysFeatures0_2lmCtrl.Data);

  return EFI_SUCCESS;
}

/**
  Programs MC patrol and sparing registers for a specified TAD rule.

  @param [in] MemTechType            Memory technology type.
  @param [in] SocketId               Socket number ID (0 should always be the SBSP).
  @param [in] McIndex                Index of the memory controller on the socket.
  @param [in] ChIndex                Index of the channel on the memory controller.
  @param [in] OffsetIndex            The index of this TAD rule.
  @param [in] RangeData              Information required to define a patrol/sparing region.

  @retval EFI_SUCCESS                The function is executed successfully without any error
  @retval EFI_INVALID_PARAMETER      The function failed due to some illegal input.

**/
EFI_STATUS
EFIAPI
McSetPatrolSparingRange (
  IN MEM_TECH_TYPE   MemTechType,
  IN UINT8           SocketId,
  IN UINT8           McIndex,
  IN UINT8           ChIndex,
  IN UINT8           OffsetIndex,
  IN PS_RANGE_DATA   *RangeData
  )
{
  TADWAYNESS_0_MC_MAIN_STRUCT            TadWayness;
  TADBASE_0_MC_MAIN_STRUCT               TadBase;
  UINT8                                  NumChPerMc;
  UINT8                                  MaxImc;

  if (MemTechType == MemTechHbm) {
    NumChPerMc = GetNumChannelPerMcHbm ();
    MaxImc     = GetMaxImcPerIoHbm () * GetMaxIoHbm ();
  } else {
    NumChPerMc = GetNumChannelPerMc ();
    MaxImc     = GetMaxImc ();
  }

  if ((McIndex >= MaxImc) || (ChIndex >= NumChPerMc) || (OffsetIndex >= TAD_RULES) || (RangeData == NULL)) {
    return EFI_INVALID_PARAMETER;
  }


  //
  // a. Program MC_DEC_CRNODE_CH{#CH}_MC{#MC}_CR_TADWAYNESS_[7:0]
  //
  TadWayness.Data = 0;

  TadWayness.Bits.tad_limit = RangeData->PsLimit;
  TadWayness.Bits.target_lid = RangeData->TgtLid;
  TadWayness.Bits.chn_lid = RangeData->ChLid;

  ChRegisterWrite (
    MemTechType,
    SocketId,
    ChIndex + (McIndex * NumChPerMc),
    mTadWaynessOffset[OffsetIndex],
    TadWayness.Data
    );

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, ChIndex, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    " TADWAYNESS[%d]:      \n"
    "  target_lid: %d  chn_lid: %d  tad_limit: 0x%x\n",
    OffsetIndex,
    TadWayness.Bits.target_lid,
    TadWayness.Bits.chn_lid,
    TadWayness.Bits.tad_limit
    );

  //
  // b. Program MC_DEC_CRNODE_CH{#CH}_MC{#MC}_CR_TADBASE_[7:0]
  //
  TadBase.Data = 0;

  TadBase.Bits.base = RangeData->PsBase;

  if ((RangeData->PsAttr & PS_ATTR_MIRROR) != 0) {
    TadBase.Bits.mirror_en = 1;
  }

  if ((RangeData->PsAttr & PS_ATTR_FAILOVER) != 0) {
    TadBase.Bits.en_failover = 1;
  }

  if ((RangeData->PsAttr & PS_ATTR_DOWNGRADE) != 0) {
    TadBase.Bits.ign_ptrl_uc = 1;
  }

  ChRegisterWrite (
    MemTechType,
    SocketId,
    ChIndex + (McIndex * NumChPerMc),
    mTadBaseOffset[OffsetIndex],
    TadBase.Data
    );

  if (IsSiliconWorkaroundEnabled ("S1409866097")) {
    if (TadBase.Bits.mirror_en == 1) {
      DisableOpenPageMode (MemTechType, SocketId, ChIndex + (McIndex * NumChPerMc));
    }
  }

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, ChIndex, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    " TADBASE[%d]:         \n"
    "  mirror_en: %d  en_failover: %d  ign_ptrl_uc: %d  base: 0x%x\n",
    OffsetIndex,
    TadBase.Bits.mirror_en,
    TadBase.Bits.en_failover,
    TadBase.Bits.ign_ptrl_uc,
    TadBase.Bits.base
    );

  return EFI_SUCCESS;
}

/**
  Programs MC patrol and sparing registers for a specified TAD rule.

  @param [in] MemTechType            Memory technology type.
  @param [in] SocketId               Socket number ID (0 should always be the SBSP).
  @param [in] McIndex                Index of the memory controller on the socket.
  @param [in] ChIndex                Index of the channel on the memory controller.
  @param [in] RegIndex               Register Index for TAD rule(10nm), SAD rule(14nm).
  @param [in] RuleData               Information required to define a patrol/sparing region.

  @retval EFI_SUCCESS                The function is executed successfully without any error
  @retval EFI_INVALID_PARAMETER      The function failed due to some illegal input.

**/
EFI_STATUS
EFIAPI
McSetPatrolSparingRule (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          SocketId,
  IN UINT8          McIndex,
  IN UINT8          ChIndex,
  IN UINT8          RegIndex,
  IN PS_RULE_DATA   *RuleData
  )
{
  NM_DRAM_RULE_CFG0_N0_MC_MAIN_STRUCT           NmDramRuleN0;
  NM_DRAM_RULE_CFG0_N1_MC_MAIN_STRUCT           NmDramRuleN1;
  UINT8                                         NumChPerMc;
  UINT8                                         MaxImc;

  if (MemTechType == MemTechHbm) {
    NumChPerMc = GetNumChannelPerMcHbm ();
    MaxImc     = GetMaxImcPerIoHbm () * GetMaxIoHbm ();
  } else {
    NumChPerMc = GetNumChannelPerMc ();
    MaxImc     = GetMaxImc ();
  }

  if ((McIndex >= MaxImc) || (ChIndex >= NumChPerMc) || (RegIndex >= TAD_RULES) || (RuleData == NULL)) {
    return EFI_INVALID_PARAMETER;
  }


  NmDramRuleN0.Data = 0;
  NmDramRuleN1.Data = 0;

  NmDramRuleN0.Bits.rule_enable    = 1;
  NmDramRuleN0.Bits.limit          = RuleData->PsLimit;
  NmDramRuleN0.Bits.nm_chn_ways    = RuleData->WaysGran.EncChWays;
  NmDramRuleN0.Bits.nm_chn_gran    = RuleData->WaysGran.EncChGran & (BIT0 | BIT1);
  NmDramRuleN1.Bits.nm_chn_gran    = (RuleData->WaysGran.EncChGran & (BIT2)) >> 2;
  NmDramRuleN1.Bits.nm_target_ways = RuleData->WaysGran.EncTgtWays;
  NmDramRuleN1.Bits.nm_chn_l2pid   = RuleData->EncChL2pid;

  if ((RuleData->WaysGran.EncTgtWays == 0) || (RuleData->WaysGran.EncChGran != RuleData->WaysGran.EncTgtGran)) {
    NmDramRuleN1.Bits.nm_gran_eq = 0;
  } else {
    NmDramRuleN1.Bits.nm_gran_eq = 1;
  }

  ChRegisterWrite (
    MemTechType,
    SocketId,
    ChIndex + (McIndex * NumChPerMc),
    mNmDramRuleCfgN0[RegIndex],
    NmDramRuleN0.Data
    );

  ChRegisterWrite (
    MemTechType,
    SocketId,
    ChIndex + (McIndex * NumChPerMc),
    mNmDramRuleCfgN1[RegIndex],
    NmDramRuleN1.Data
    );

  return EFI_SUCCESS;
}

/**
  Programs MC 2LM registers for a specified TAD rule.

  @param[in] MemTechType         Memory technology type.
  @param[in] SocketId            Socket number ID (0 should always be the SBSP).
  @param[in] McIndex             Index of the memory controller on the socket.
  @param[in] ChIndex             Index of the channel on the memory controller.
  @param[in] NmData              Information required to define a 2LM near memory region.

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function failed due to some illegal input.

**/
EFI_STATUS
EFIAPI
McSet2lmNm (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          SocketId,
  IN UINT8          McIndex,
  IN UINT8          ChIndex,
  IN NM_2LM_DATA    *NmData
  )
{
  MCNMCACHINGINTLV_MC_2LM_STRUCT                McNmCachIlv;
  UINT8                                         NumChPerMc;
  UINT8                                         MaxImc;

  if (MemTechType == MemTechHbm) {
    NumChPerMc = GetNumChannelPerMcHbm ();
    MaxImc = GetMaxImcPerIoHbm () * GetMaxIoHbm ();
  } else {
    NumChPerMc = GetNumChannelPerMc ();
    MaxImc = GetMaxImc ();
  }

  if ((McIndex >= MaxImc) || (ChIndex >= NumChPerMc) || (NmData == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // a. Program MC_2LMCNTL_CRNODE_CH{#CH}_MC{#MC}_CR_MCNMCACHINGINTLV
  //
  McNmCachIlv.Data = 0;

  McNmCachIlv.Bits.nm_target_ways = NmData->NmWaysGran.EncTgtWays;
  McNmCachIlv.Bits.nm_target_lid  = NmData->NmTgtLid;
  McNmCachIlv.Bits.nm_chn_ways    = NmData->NmWaysGran.EncChWays;

  //
  // 3 ways FastDiv3 == 1 way
  //
  if (McNmCachIlv.Bits.nm_chn_ways == 2) {
    McNmCachIlv.Bits.nm_chn_ways = 0;
  }

  McNmCachIlv.Bits.nm_target_gran = NmData->NmWaysGran.EncTgtGran;
  McNmCachIlv.Bits.nm_chn_gran    = NmData->NmWaysGran.EncChGran;
  McNmCachIlv.Bits.nm_chn_lid     = NmData->NmChLid;

  ChRegisterWrite (
    MemTechType,
    SocketId,
    ChIndex + (McIndex * NumChPerMc),
    MCNMCACHINGINTLV_MC_2LM_REG,
    McNmCachIlv.Data
    );

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, ChIndex, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    " MCNMCACHINGINTLV:\n"
    "nm_target_ways: %d  nm_chn_ways: %d  nm_target_lid: %d "
    "nm_target_gran: %d  nm_chn_gran: %x  nm_chn_lid: %d\n",
    McNmCachIlv.Bits.nm_target_ways,
    McNmCachIlv.Bits.nm_chn_ways,
    McNmCachIlv.Bits.nm_target_lid,
    McNmCachIlv.Bits.nm_target_gran,
    McNmCachIlv.Bits.nm_chn_gran,
    McNmCachIlv.Bits.nm_chn_lid
    );

  //
  // b. Program MC_2LMCNTL_CRNODE_CH{#CH}_MC{#MC}_CR_MCNMCachingCfg
  //
  McSetNm2lmCacheCfg (MemTechType, SocketId, McIndex, ChIndex, NmData);

  //
  // c. Program MC_2LMCNTL_CRNODE_CH{#CH}_MC{#MC}_CR_McNMCachingOffset
  //
  McSetNmCachingOffset (MemTechType, SocketId, McIndex, ChIndex, NmData);

  return EFI_SUCCESS;
}


/**
  Programs MC 2LM registers for far memory.

  @param [in] MemTechType        Memory technology type.
  @param [in] SocketId           Socket number ID(0 should always be the SBSP).
  @param [in] McIndex            Index of the memory controller on the socket.
  @param [in] ChIndex            Index of the channel on the memory controller.
  @param [in] RegIndex           Register Index for MC DRAM Rule(10nm), SAD rule(14nm).
  @param [in] FmData             Information required to define a 2LM far memory region.

  @retval EFI_SUCCESS            The function is executed successfully without any error
  @retval EFI_INVALID_PARAMETER  The function failed due to some illegal input.
  @retval EFI_OUT_OF_RESOURCES   The available registers are exausted.

**/
EFI_STATUS
EFIAPI
McSet2lmFm (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          SocketId,
  IN UINT8          McIndex,
  IN UINT8          ChIndex,
  IN UINT8          RegIndex,
  IN FM_2LM_DATA    *FmData
  )
{
  DRAM_RULE_CFG0_N0_MC_MAIN_STRUCT              McDramRuleN0;
  DRAM_RULE_CFG0_N1_MC_MAIN_STRUCT              McDramRuleN1;
  UINT8                                         FmChIndex;
  UINT8                                         FmChLid;
  UINT8                                         NumChPerMc;
  UINT8                                         MaxImc;

  if (MemTechType == MemTechHbm) {
    NumChPerMc = GetNumChannelPerMcHbm ();
    MaxImc = GetMaxImcPerIoHbm () * GetMaxIoHbm ();
  } else {
    NumChPerMc = GetNumChannelPerMc ();
    MaxImc = GetMaxImc ();
  }

  if ((McIndex >= MaxImc) || (ChIndex >= NumChPerMc) || (RegIndex >= TAD_RULES) || (FmData == NULL)) {
    return EFI_INVALID_PARAMETER;
  }


  //
  // d. Program MC_DEC_CRNODE_CH{#CH}_MC{#MC}_CR_DRAM_RULE_CFG[7:0]
  //

  McDramRuleN0.Data = 0;
  McDramRuleN1.Data = 0;

  McDramRuleN0.Bits.rule_enable    = 1;
  McDramRuleN0.Bits.limit          = FmData->TadLimit;
  McDramRuleN0.Bits.fm_target      = FmData->FmTarget;
  McDramRuleN0.Bits.fm_chn_ways    = FmData->FmWaysGran.EncChWays;
  McDramRuleN1.Bits.fm_target_ways = FmData->FmWaysGran.EncTgtWays;
  McDramRuleN1.Bits.fm_chn_gran    = FmData->FmWaysGran.EncChGran;

  if ((McDramRuleN1.Bits.fm_target_ways == 0) ||
    (FmData->FmWaysGran.EncChGran != FmData->FmWaysGran.EncTgtGran)) {
    McDramRuleN1.Bits.fm_gran_eq = 0;
  } else {
    McDramRuleN1.Bits.fm_gran_eq = 1;
  }

  //
  // Where the far memory channel granularity is 256B
  //
  if (FmData->FmWaysGran.EncChGran == INTERLEAVE_GRANULARITY_256B) {
    //
    // Channel ways for FM in 256B granularity is always 1
    // Program fm_chn_l2pid all 0's in channel 0 and all 1's in channel 1.
    //
    if (ChIndex == 0) {
      if ((FmData->FmChIntBitmap & BIT0) != 0) {
        McDramRuleN1.Bits.fm_chn_l2pid = 0x0;
      } else {
        for (FmChLid = 0; FmChLid < NumChPerMc; FmChLid++) {
          McDramRuleN1.Bits.fm_chn_l2pid |= 0x1 << (FmChLid * 2);
        }
      }
    } else {
      if ((FmData->FmChIntBitmap & BIT1) != 0) {
        for (FmChLid = 0; FmChLid < NumChPerMc; FmChLid++) {
          McDramRuleN1.Bits.fm_chn_l2pid |= 0x1 << (FmChLid * 2);
        }
      } else {
        McDramRuleN1.Bits.fm_chn_l2pid = 0x0;
      }
    }
  } else {
    FmChLid = 0;
    for (FmChIndex = 0; FmChIndex < NumChPerMc; FmChIndex++) {
      if ((FmData->FmChIntBitmap & (BIT0 << FmChIndex)) != 0) {
        McDramRuleN1.Bits.fm_chn_l2pid |= (FmChIndex & 0x3) << (FmChLid * 2);
        FmChLid++;
      }
    } // FM Channel loop
  }

  ChRegisterWrite (
    MemTechType,
    SocketId,
    ChIndex + (McIndex * NumChPerMc),
    mMcDramRuleCfgN0[RegIndex],
    McDramRuleN0.Data
    );

  ChRegisterWrite (
    MemTechType,
    SocketId,
    ChIndex + (McIndex * NumChPerMc),
    mMcDramRuleCfgN1[RegIndex],
    McDramRuleN1.Data
    );


  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, ChIndex, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    " DRAM_RULE[%d]:      \n"
    "  rule_enable: %d  limit: 0x%x  fm_target: %d  fm_chn_ways: %d  "
    "fm_chn_gran: %d  fm_target_ways: %d  fm_gran_eq: %d  fm_chn_l2pid: 0x%08x\n",
    RegIndex,
    McDramRuleN0.Bits.rule_enable,
    McDramRuleN0.Bits.limit,
    McDramRuleN0.Bits.fm_target,
    McDramRuleN0.Bits.fm_chn_ways,
    McDramRuleN1.Bits.fm_chn_gran,
    McDramRuleN1.Bits.fm_target_ways,
    McDramRuleN1.Bits.fm_gran_eq,
    McDramRuleN1.Bits.fm_chn_l2pid
    );

  return EFI_SUCCESS;
}

/**
  This function programs DRAM_RULE_CFG*.fm_target to 0.

  @param[in] ScktId           Socket index.
  @param[in] McId             Memory controller index on socket.
  @param[in] ChId             Channel index on memory controller.
  @param[in] RegId            Register Index of MC DRAM Rule.

  @retval N/A
**/
VOID
EFIAPI
McClearFmTarget (
  IN UINT8   SocketId,
  IN UINT8   McId,
  IN UINT8   ChId,
  IN UINT8   RegId
  )
{
  UINT8                             MaxChPerMc;
  DRAM_RULE_CFG0_N0_MC_MAIN_STRUCT  McDramRuleN0;

  MaxChPerMc = GetNumChannelPerMc ();

  McDramRuleN0.Data = ChRegisterRead (MemTechDdr, SocketId, ChId + (McId * MaxChPerMc), mMcDramRuleCfgN0[RegId]);
  McDramRuleN0.Bits.fm_target = 0;
  ChRegisterWrite (MemTechDdr, SocketId, ChId + (McId * MaxChPerMc), mMcDramRuleCfgN0[RegId], McDramRuleN0.Data);
}

/**
  Encode Limit field for RIR DRAM rules

  @param[in] Limit   Limit to encode

  @retval Encoded Limit
**/
UINT32
RirEncodeLimit (
  IN UINT32 Limit
  )
{
  if (Limit == 0) {
    return 0;
  } else {
    //
    // Adjust for 64BM -> 512MB granularity and for upper range Limit being <= Limit
    //
    return ((Limit - 1) >> 3);
  }
}

/**
  Encode Offset field for RIR DRAM rules

  @param[in] Offset   Offset to encode

  @retval Encoded Offset
**/
UINT32
RirEncodeOffset (
  IN UINT32 Offset
  )
{
  return Offset;
}

/**
  Programs MC RIR registers.

  @param [in] MemTechType        Memory technology type.
  @param [in] SocketId           Socket number ID (0 should always be the SBSP).
  @param [in] McIndex            Index of the memory controller on the socket.
  @param [in] ChIndex            Index of the channel on the memory controller.
  @param [in] DdrtPresent        Indicates if DDRT DIMM is present on the channel.
  @param [in] RirData            Information required to program the rank interleave registers for the channel.

  @retval EFI_SUCCESS            The function is executed successfully without any error
  @retval EFI_INVALID_PARAMETER  The function failed due to some illegal input.

**/
EFI_STATUS
EFIAPI
McSetRir (
  IN MEM_TECH_TYPE    MemTechType,
  IN UINT8            SocketId,
  IN UINT8            McIndex,
  IN UINT8            ChIndex,
  IN BOOLEAN          DdrtPresent,
  IN RIR_DATA         *RirData
  )
{
  UINT8                             Rir;
  UINT8                             Index;
  RIRWAYNESSLIMIT_0_MC_MAIN_STRUCT  ImcRIRWaynessLimit;
  RIRILV0OFFSET_0_MC_MAIN_STRUCT    ImcRIRIlvOffset;
  FMRIRWAYNESSLIMIT_0_MC_2LM_STRUCT ImcFMRIRWaynessLimit;
  FMRIRILV0OFFSET_0_MC_2LM_STRUCT   ImcFMRIRIlvOffset;
  UINT8                             NumChPerMc;
  UINT8                             MaxImc;

  if (MemTechType == MemTechHbm) {
    NumChPerMc = GetNumChannelPerMcHbm ();
    MaxImc     = GetMaxImcPerIoHbm () * GetMaxIoHbm ();
  } else {
    NumChPerMc = GetNumChannelPerMc ();
    MaxImc     = GetMaxImc ();
  }

  if ((McIndex >= MaxImc) || (ChIndex >= NumChPerMc) || (RirData == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  for (Rir = 0; Rir < MAX_RIR; Rir++) {
    //Update RIR wayness register
    ImcRIRWaynessLimit.Data = 0;

    ImcRIRWaynessLimit.Bits.rir_val   = RirData->NmRir[Rir].RirValid;
    ImcRIRWaynessLimit.Bits.rir_limit = RirEncodeLimit (RirData->NmRir[Rir].RirLimit);
    ImcRIRWaynessLimit.Bits.rir_way   = RirData->NmRir[Rir].EncRirWays;

    RcDebugPrint (SDBG_MAX,
      "    RIRWAYNESSLIMIT_%d_MC_MAIN_REG\n", Rir);
    RcDebugPrint (SDBG_MAX,
      "    \trir_val: %x\trir_limit: 0x%x\trir_way: 0x%x\n\n",
      ImcRIRWaynessLimit.Bits.rir_val, ImcRIRWaynessLimit.Bits.rir_limit, ImcRIRWaynessLimit.Bits.rir_way);

    ChRegisterWrite (
      MemTechType,
      SocketId,
      ChIndex + (McIndex * NumChPerMc),
      mRIRWaynessLimit[Rir],
      ImcRIRWaynessLimit.Data
      );

    //Update RIR Interleave offset register
    for (Index = 0; Index < MAX_RIR_WAYS; Index++) {
      ImcRIRIlvOffset.Data = 0;

      ImcRIRIlvOffset.Bits.rir_rnk_tgt0 = RirData->NmRir[Rir].RirTgtId[Index];
      ImcRIRIlvOffset.Bits.rir_offset0  = RirEncodeOffset (RirData->NmRir[Rir].RirOffset[Index]);

      RcDebugPrint (SDBG_MAX,
        "        RIRILV%dOFFSET_%d_MC_MAIN_REG\n", Index, Rir);
      RcDebugPrint (SDBG_MAX,
        "        \trir_rnk_tgt0: 0x%x\trir_offset0: 0x%x\n\n",
        ImcRIRIlvOffset.Bits.rir_rnk_tgt0, ImcRIRIlvOffset.Bits.rir_offset0);

      ChRegisterWrite (
        MemTechType,
        SocketId,
        ChIndex + (McIndex * NumChPerMc),
        mRIRIlvOffset[Rir][Index],
        ImcRIRIlvOffset.Data
        );
    }
  }

  // Write the FM RIR registers only if an NVMCTLR DIMM exists on this channel
  if (DdrtPresent && (MemTechType != MemTechHbm)) {

    for (Rir = 0; Rir < MAX_RIR_DDRT; Rir++) {

      // Update FM RIR wayness register
      ImcFMRIRWaynessLimit.Data = 0;

      ImcFMRIRWaynessLimit.Bits.rir_val   = RirData->FmRir[Rir].RirValid;
      ImcFMRIRWaynessLimit.Bits.rir_limit = RirEncodeLimit (RirData->FmRir[Rir].RirLimit);
      ImcFMRIRWaynessLimit.Bits.rir_way   = RirData->FmRir[Rir].EncRirWays;

      RcDebugPrint (SDBG_MAX,
        "    FMRIRWAYNESSLIMIT_%d_MC2LM_REG\n", Rir);
      RcDebugPrint (SDBG_MAX,
        "    \trir_val: 0x%x\trir_limit: %x\trir_way: 0x%x\n\n",
        ImcFMRIRWaynessLimit.Bits.rir_val, ImcFMRIRWaynessLimit.Bits.rir_limit, ImcFMRIRWaynessLimit.Bits.rir_way);

      ChRegisterWrite (
        MemTechType,
        SocketId,
        ChIndex + (McIndex * NumChPerMc),
        mFMRIRWaynessLimit[Rir],
        ImcFMRIRWaynessLimit.Data
        );

      //Update FM RIR Interleave offset register
      for (Index = 0; Index < MAX_RIR_DDRT_WAYS; Index++) {
        ImcFMRIRIlvOffset.Data = 0;

        ImcFMRIRIlvOffset.Bits.rir_rnk_tgt0 = RirData->FmRir[Rir].RirTgtId[Index];
        ImcFMRIRIlvOffset.Bits.rir_offset0  = RirEncodeOffset (RirData->FmRir[Rir].RirOffset[Index]);

        RcDebugPrint (SDBG_MAX,
          "        FMRIRILV%dOFFSET_%d_MC2LM_REG\n", Index, Rir);
        RcDebugPrint (SDBG_MAX,
          "        \trir_rnk_tgt0: 0x%x\trir_offset0: 0x%x\n\n",
          ImcFMRIRIlvOffset.Bits.rir_rnk_tgt0, ImcFMRIRIlvOffset.Bits.rir_offset0);

        ChRegisterWrite (
          MemTechType,
          SocketId,
          ChIndex + (McIndex * NumChPerMc),
          mFMRIRIlvOffset[Rir][Index],
          ImcFMRIRIlvOffset.Data
          );
      }
    }
  }

  return EFI_SUCCESS;

}

/**

  Mc Init for DisableDirForAppDirect

  @param[in] MemTechType            Memory technology type.
  @param[in] Socket                 Socket index.
  @param[in] Ch                     Channel index within socket.
  @param[in] DisableDir4AppDirect   Value to program disabledir4appdirect field.

  @retval N/A

**/
VOID
EFIAPI
McDisableDirForAppDirect (
  IN  MEM_TECH_TYPE      MemTechType,
  IN  UINT8              Socket,
  IN  UINT8              Ch,
  IN  UINT8              DisableDir4AppDirect
  )
{
  SBDEFEATURES0_MC_2LM_STRUCT SbdFeatures0;

  if ((Socket >= MAX_SOCKET) || (Ch >= GetMaxChPerTechType (MemTechType))) {
    return;
  }

  SbdFeatures0.Data = ChRegisterRead (MemTechType, Socket, Ch, SBDEFEATURES0_MC_2LM_REG);
  SbdFeatures0.Bits.disabledir4appdirect = DisableDir4AppDirect;
  ChRegisterWrite (MemTechType, Socket, Ch, SBDEFEATURES0_MC_2LM_REG, SbdFeatures0.Data);
}

/**

  This function sets the AllowDirUpdToBlock field of SbdFeatures0 register.

  @param[in] MemTechType            Memory technology type.
  @param[in] SocketId               Socket index.
  @param[in] ChId                   Channel index within socket.
  @param[in] AllowDirUpdToBlock     Data to be written to AllowDirUpdToBlock field.

**/
VOID
EFIAPI
McSetAllowDirUpdToBlock (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          SocketId,
  IN UINT8          ChId,
  IN UINT8          AllowDirUpdToBlock
  )
{
  SBDEFEATURES0_MC_2LM_STRUCT   SbdFeatures0Reg;

  if ((SocketId >= MAX_SOCKET) || (ChId >= GetMaxChPerTechType (MemTechType))) {
    return;
  }

  SbdFeatures0Reg.Data = ChRegisterRead (MemTechType, SocketId, ChId, SBDEFEATURES0_MC_2LM_REG);
  SbdFeatures0Reg.Bits.allowdirupdtoblock = AllowDirUpdToBlock;
  ChRegisterWrite (MemTechType, SocketId, ChId, SBDEFEATURES0_MC_2LM_REG, SbdFeatures0Reg.Data);
}

/**

  This function sets the EnEarlyCmp field of SysFeatures0 register.

  @param[in] MemTechType            Memory technology type.
  @param[in] SocketId               Socket index.
  @param[in] ChId                   Channel index within socket.
  @param[in] EnEarlyCmp             Data to be written to EnEarlyCmp field.

**/
VOID
EFIAPI
McSetEnEarlyCmp (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          SocketId,
  IN UINT8          ChId,
  IN UINT8          EnEarlyCmp
  )
{
  SYSFEATURES0_MC_2LM_STRUCT   Sysfeatures0Reg;

  if ((SocketId >= MAX_SOCKET) || (ChId >= GetMaxChPerTechType (MemTechType))) {
    return;
  }

  Sysfeatures0Reg.Data = ChRegisterRead (MemTechType, SocketId, ChId, SYSFEATURES0_MC_2LM_REG);
  Sysfeatures0Reg.Bits.enearlycmp = EnEarlyCmp;
  ChRegisterWrite (MemTechType, SocketId, ChId, SYSFEATURES0_MC_2LM_REG, Sysfeatures0Reg.Data);
}

/**
 Sets SBMCACTL_MC_2LM.nmfillwrcamerrdis = 1 on the input channel.

@param [in] SocketId   Socket number ID(0 should always be the SBSP).
@param [in] McIndex    Index of the memory controller on the socket.
@param [in] ChIndex    Index of the channel on the memory controller.

@retval none

**/
VOID
EFIAPI
McDisableNmFillWrCamErrViralTriggering (
  IN UINT8 SocketId,
  IN UINT8 McIndex,
  IN UINT8 ChIndex
  )
{
  UINT8                  NumChPerMc;
  SBMCACTL_MC_2LM_STRUCT SbMcaCtl;

  NumChPerMc = GetNumChannelPerMc ();

  SbMcaCtl.Data = MemReadPciCfgEp (
                    SocketId,
                    ChIndex + (McIndex * NumChPerMc),
                    SBMCACTL_MC_2LM_REG
                    );
  SbMcaCtl.Bits.nmfillwrcamerrdis = 1;
  MemWritePciCfgEp (
    SocketId,
    ChIndex + (McIndex * NumChPerMc),
    SBMCACTL_MC_2LM_REG,
    SbMcaCtl.Data
    );
}

/**

  Writes route table data to registers

  @param [in] SocketId           Socket number ID(0 should always be the SBSP).
  @param [in] McIndex            Index of the memory controller on the socket.
  @param [in] DdrtPresent        Indicates if DDRT DIMM is present on the channel.
  @param [in] RouteTableData     Information required to program the Route table CSR's.

  @retval EFI_SUCCESS            The function is executed successfully without any error
  @retval EFI_INVALID_PARAMETER  The function failed due to some illegal input.

**/

EFI_STATUS
EFIAPI
McSetRouteTableCsr (
  IN UINT8              SocketId,
  IN UINT8              McIndex,
  IN BOOLEAN            DdrtPresent,
  IN ROUTE_TABLE_INFO   *RouteTableData
  )
{

  //
  // No op on 10nm
  //

  return EFI_SUCCESS;

} // McSetRouteTableCsr

/**

  Programs MC Interleave registers for 14nm near/far memory.

  @param [in] SocketId           Socket number ID(0 should always be the SBSP).
  @param [in] McIndex            Index of the memory controller on the socket.
  @param [in] SadIndex           The index of this SAD rule.
  @param [in] FarMemory          Indicates far memory.
  @param [in] EncInterleaveData  Encoded Information indicating InterleaveList for near/far memory.

  @retval EFI_SUCCESS            The function is executed successfully without any error
  @retval EFI_INVALID_PARAMETER  The function failed due to some illegal input.

**/

EFI_STATUS
EFIAPI
McSetInterleaveList (
  IN UINT8         SocketId,
  IN UINT8         McIndex,
  IN UINT8         SadIndex,
  IN BOOLEAN       FarMemory,
  IN UINT32        EncInterleaveData
  )
{

  //
  // No op on 10nm
  //

  return EFI_SUCCESS;

} // McSetInterleaveList
