/** @file
  Implementation of patrol scrub of 10nm common

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#include <Library/CpuCsrAccessLib.h>
#include <Library/BaseLib.h>
#include <SiliconSetting.h>
#include "Include/MemMcRegs.h"
#include <Library/SiliconWorkaroundLib.h>
#include <Library/PcuMailBoxRasLib.h>
#include <Library/SecurityPolicyLib.h>
#include <Library/CpuAndRevisionLib.h>

#define TAD_RULE_ZERO 0

/**
  Check if the patrol scrub engine is in System Address (SA) mode.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @retval     TRUE            The patrol scrub engine is in SA mode.
  @retval     FALSE           The patrol scrub engine is in legacy mode.
**/
BOOLEAN
EFIAPI
IsPatrolScrubSaMode (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  )
{
  SCRUBADDRESSHI_MC_MAIN_STRUCT ScrubAddrHi;

  ScrubAddrHi.Data = ReadCpuCsr (Skt, ChOnSkt, SCRUBADDRESSHI_MC_MAIN_REG);

  return (ScrubAddrHi.Bits.ptl_sa_mode == 1);
}

/**
  Switch the patrol scrub engine to SA mode or legacy mode.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  LegacyMode      TRUE for legacy mode; FALSE for SA mode.
**/
VOID
EFIAPI
SwitchPatrolScrubMode (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  BOOLEAN LegacyMode
  )
{
  SCRUBADDRESSHI_MC_MAIN_STRUCT  ScrubAddrHi;

  if (LegacyMode) {
    if (IsPatrolScrubSaMode (Skt, Mc, ChOnSkt)) {
      ScrubAddrHi.Data = ReadCpuCsr (Skt, ChOnSkt, SCRUBADDRESSHI_MC_MAIN_REG);
      ScrubAddrHi.Bits.ptl_sa_mode = 0;
      WriteCpuCsr (Skt, ChOnSkt, SCRUBADDRESSHI_MC_MAIN_REG, ScrubAddrHi.Data);
    }
  } else {
    if (!IsPatrolScrubSaMode (Skt, Mc, ChOnSkt)) {
      ScrubAddrHi.Data = ReadCpuCsr (Skt, ChOnSkt, SCRUBADDRESSHI_MC_MAIN_REG);
      ScrubAddrHi.Bits.ptl_sa_mode = 1;
      WriteCpuCsr (Skt, ChOnSkt, SCRUBADDRESSHI_MC_MAIN_REG, ScrubAddrHi.Data);
    }
  }
}

/**
  Set the patrol scrub address.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  PhyAddr         Physical addresss.
**/
VOID
EFIAPI
SetPatrolScrubAddress (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  UINT64  PhyAddr
  )
{
  SCRUBADDRESSHI_MC_MAIN_STRUCT                 ScrubAddrHi;
  SCRUBADDRESSLO_MC_MAIN_STRUCT                 ScrubAddrLo;
  SCRUBADDRESS2LO_MC_MAIN_STRUCT                ScrubAddr2Lo;
  SCRUBADDRESS2HI_MC_MAIN_STRUCT                ScrubAddr2Hi;
  UINT64                                        TempAddr;
  UINT32                                        Command;

  ScrubAddrHi.Data = ReadCpuCsr(Skt, ChOnSkt, SCRUBADDRESSHI_MC_MAIN_REG);
  ScrubAddrHi.Bits.rankaddhi = (UINT32) (RShiftU64 (PhyAddr, 38));
  WriteCpuCsr (Skt, ChOnSkt, SCRUBADDRESSHI_MC_MAIN_REG, ScrubAddrHi.Data);

  ScrubAddr2Hi.Data = ReadCpuCsr (Skt, ChOnSkt, SCRUBADDRESS2HI_MC_MAIN_REG);
  ScrubAddr2Hi.Bits.baseaddhi = (UINT32) (RShiftU64 (PhyAddr, 38));

  if (IsSiliconWorkaroundEnabled ("S2209304250")) {
    Command = MAILBOX_BIOS_CR_WRITE_CMD (Mc, SKTCH_TO_NODECH (ChOnSkt), MAILBOX_BIOS_CMD_CR_PROXY_SCRUBADDRESS2HI_INDEX);
    ReadWritePcuMailbox (Skt, MAILBOX_BIOS_CMD_ZERO, Command, &ScrubAddr2Hi.Data, 0, 0);
  } else {
    WriteCpuCsr (Skt, ChOnSkt, SCRUBADDRESS2HI_MC_MAIN_REG, ScrubAddr2Hi.Data);
  }

  //
  // TODO: if 256 byte interleave, clear last 8 bits.
  //
  TempAddr = PhyAddr;

  ScrubAddrLo.Data  = ReadCpuCsr (Skt, ChOnSkt, SCRUBADDRESSLO_MC_MAIN_REG);
  ScrubAddrLo.Bits.rankadd = (UINT32) (RShiftU64 (TempAddr, 6));
  WriteCpuCsr (Skt, ChOnSkt, SCRUBADDRESSLO_MC_MAIN_REG, ScrubAddrLo.Data);

  ScrubAddr2Lo.Data  = ReadCpuCsr (Skt, ChOnSkt, SCRUBADDRESS2LO_MC_MAIN_REG);
  ScrubAddr2Lo.Bits.baseadd = (UINT32) (RShiftU64 (TempAddr, 6));

  if (IsSiliconWorkaroundEnabled ("S2209304250")) {
    Command = MAILBOX_BIOS_CR_WRITE_CMD (Mc, SKTCH_TO_NODECH (ChOnSkt), MAILBOX_BIOS_CMD_CR_PROXY_SCRUBADDRESS2LO_INDEX);
    ReadWritePcuMailbox (Skt, MAILBOX_BIOS_CMD_ZERO, Command, &ScrubAddr2Lo.Data, 0, 0);
  } else {
    WriteCpuCsr (Skt, ChOnSkt, SCRUBADDRESS2LO_MC_MAIN_REG, ScrubAddr2Lo.Data);
  }
}

/**
  Get ScrubAddressLo value

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @retval[out]  ScrubAddressLo         Value of ScrubAddrLo register
**/
UINT32
EFIAPI
GetScrubAddressLo (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  )
{
  SCRUBADDRESSLO_MC_MAIN_STRUCT  ScrubAddrLo;
  ScrubAddrLo.Data  = ReadCpuCsr (Skt, ChOnSkt, SCRUBADDRESSLO_MC_MAIN_REG);
  return ScrubAddrLo.Data;
}

/**
  Set the patrol scrub legacy (rank) address.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  Rank            Rank index.
  @param[in]  RankAddress     Rank address.
**/
VOID
EFIAPI
SetPatrolScrubLegacyAddress (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  UINT8   Rank,
  IN  UINT32  RankAddress
  )
{
  UINT8                  ChOnMc;
  SCRUBADDRESSHI_MC_MAIN_STRUCT  ScrubAddrHi;
  SCRUBADDRESSLO_MC_MAIN_STRUCT  ScrubAddrLo;

  ChOnMc = ChOnSkt % MAX_MC_CH;

  //
  // The Patrol Scrub mode is legacy mode, just setup the address in the low/high registers
  //
  ScrubAddrHi.Data = ReadCpuCsr (Skt, ChOnSkt, SCRUBADDRESSHI_MC_MAIN_REG);
  ScrubAddrHi.Bits.rank  = Rank;
  ScrubAddrHi.Bits.chnl  = ChOnMc;
  WriteCpuCsr (Skt, ChOnSkt, SCRUBADDRESSHI_MC_MAIN_REG, ScrubAddrHi.Data);

  ScrubAddrLo.Data  = ReadCpuCsr (Skt,ChOnSkt, SCRUBADDRESSLO_MC_MAIN_REG);
  ScrubAddrLo.Bits.rankadd = RankAddress;
  WriteCpuCsr (Skt, ChOnSkt, SCRUBADDRESSLO_MC_MAIN_REG, ScrubAddrLo.Data);
}

/**
  Check if patrol scrub engine is scrubbing a given target rank (legacy mode).

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  TargetChOnMc    Target channel on MC.
  @param[in]  TargetRank      Target rank.

  @retval     TRUE            Currently scrubbing the target rank.
  @retval     FALSE           Not legacy mode, or not scrubbing the target rank.
**/
BOOLEAN
EFIAPI
IsLegacyScrubbingRank (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  UINT8   TargetChOnMc,
  IN  UINT8   TargetRank
  )
{
  SCRUBADDRESSHI_MC_MAIN_STRUCT         ScrubAddrHi;

  ScrubAddrHi.Data= ReadCpuCsr (Skt, ChOnSkt, SCRUBADDRESSHI_MC_MAIN_REG);

  if ((ScrubAddrHi.Bits.chnl == TargetChOnMc) && (ScrubAddrHi.Bits.rank == TargetRank)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Set Scrub control register tad_rule based on the system address.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  System address  system address.
  @param[in]  Is2lmMode       TRUE if 2LM mode; FALSE if not.
**/
VOID
EFIAPI
UpdateCurrentTad (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  UINT64  Systemaddress,
  IN  BOOLEAN Is2lmMode
  )
{
  SCRUBADDRESSHI_MC_MAIN_STRUCT               ScrubAddrHiReg;
  TADWAYNESS_0_MC_MAIN_STRUCT                 TADWayness;
  UINT8                                       TadIndex;
  UINT64                                      TadLimit;

  UINT32 TadWayness[TAD_RULES] = {
      TADWAYNESS_0_MC_MAIN_REG, TADWAYNESS_1_MC_MAIN_REG, TADWAYNESS_2_MC_MAIN_REG, TADWAYNESS_3_MC_MAIN_REG,
      TADWAYNESS_4_MC_MAIN_REG, TADWAYNESS_5_MC_MAIN_REG, TADWAYNESS_6_MC_MAIN_REG, TADWAYNESS_7_MC_MAIN_REG
    };

  ScrubAddrHiReg.Data = ReadCpuCsr (Skt, ChOnSkt, SCRUBADDRESSHI_MC_MAIN_REG);

  //
  //calculate tad_rule
  //
  if (!Is2lmMode) {
    ScrubAddrHiReg.Bits.minimum_tad_rule = 0x0;
    for (TadIndex = 0; TadIndex < TAD_RULES; TadIndex++) {
      TADWayness.Data = ReadCpuCsr (Skt, ChOnSkt, TadWayness[TadIndex]);
      TadLimit = LShiftU64 (((UINT64) TADWayness.Bits.tad_limit + 1) , 26);
      if (Systemaddress <= TadLimit) {
        ScrubAddrHiReg.Bits.tad_rule = TadIndex;
        break;
      }
    }
  }
  WriteCpuCsr (Skt, ChOnSkt, SCRUBADDRESSHI_MC_MAIN_REG, ScrubAddrHiReg.Data);
}

/**
  Set Scrub control register TAD rule.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  Is2lmMode       TRUE if 2LM mode; FALSE if not.
**/
VOID
EFIAPI
SetScrubCtrlTadRule (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  BOOLEAN Is2lmMode
  )
{
  SCRUBADDRESSHI_MC_MAIN_STRUCT               ScrubAddrHiReg;
  TADBASE_0_MC_MAIN_STRUCT                    TADBase;
  TADWAYNESS_0_MC_MAIN_STRUCT                 TADWayness;
  UINT8                                       TadIndex;

  UINT32 TadBase[TAD_RULES] = {
      TADBASE_0_MC_MAIN_REG, TADBASE_1_MC_MAIN_REG, TADBASE_2_MC_MAIN_REG, TADBASE_3_MC_MAIN_REG,
      TADBASE_4_MC_MAIN_REG, TADBASE_5_MC_MAIN_REG, TADBASE_6_MC_MAIN_REG, TADBASE_7_MC_MAIN_REG
     } ;

  UINT32 TadWayness[TAD_RULES] = {
      TADWAYNESS_0_MC_MAIN_REG, TADWAYNESS_1_MC_MAIN_REG, TADWAYNESS_2_MC_MAIN_REG, TADWAYNESS_3_MC_MAIN_REG,
      TADWAYNESS_4_MC_MAIN_REG, TADWAYNESS_5_MC_MAIN_REG, TADWAYNESS_6_MC_MAIN_REG, TADWAYNESS_7_MC_MAIN_REG
    };

  ScrubAddrHiReg.Data = ReadCpuCsr (Skt, ChOnSkt, SCRUBADDRESSHI_MC_MAIN_REG);

  //
  //calculate min_tad_rule/max_tad_rule
  //
  if (!Is2lmMode) {
    ScrubAddrHiReg.Bits.minimum_tad_rule = 0x0;
    for (TadIndex = 0; TadIndex < TAD_RULES; TadIndex++) {
      TADBase.Data    = ReadCpuCsr (Skt, ChOnSkt, TadBase[TadIndex]);
      TADWayness.Data = ReadCpuCsr (Skt, ChOnSkt, TadWayness[TadIndex]);
      if (TADBase.Bits.base >= TADWayness.Bits.tad_limit) {
        break;
      }
    }
    ScrubAddrHiReg.Bits.maximum_tad_rule = TadIndex - 1;
  } else {
    //
    //There is only One TAD rule programmed in 2LM mode to support the entire 2LM memory.
    //The TadBase and limit will only cover NM size
    //
    ScrubAddrHiReg.Bits.minimum_tad_rule = TAD_RULE_ZERO;
    ScrubAddrHiReg.Bits.maximum_tad_rule = TAD_RULE_ZERO;
  }
  WriteCpuCsr (Skt, ChOnSkt, SCRUBADDRESSHI_MC_MAIN_REG, ScrubAddrHiReg.Data);
}




/**
  Get tad range for the TAD.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         channel on socket.
  @param[out] MinTad          Minital tad index for the patrol scrub.
  @param[out] MaxTad          Minital tad index for the patrol scrub.
**/
VOID
EFIAPI
GetPatrolScrubRange (
  IN  UINT8     Skt,
  IN  UINT8     ChOnSkt,
  IN OUT UINT8 *MinTad,
  IN OUT UINT8 *MaxTad
  )
{
  SCRUBADDRESSHI_MC_MAIN_STRUCT   ScrubAddrHiReg;
  ScrubAddrHiReg.Data = ReadCpuCsr (Skt, ChOnSkt, SCRUBADDRESSHI_MC_MAIN_REG);

  if (MinTad != NULL) {
    *MinTad = (UINT8)ScrubAddrHiReg.Bits.minimum_tad_rule;
  }

  if (MaxTad != NULL) {
    *MaxTad = (UINT8)ScrubAddrHiReg.Bits.maximum_tad_rule;
  }
}




/**
  Set sparing control register TAD rule.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  Is2lmMode       TRUE if 2LM mode; FALSE if not.
**/
VOID
EFIAPI
SetSparingCtrlTadRule (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  BOOLEAN Is2lmMode
  )
{
  SPARING_CONTROL_MC_MAIN_STRUCT              SpareCtlReg;
  TADBASE_0_MC_MAIN_STRUCT                    TADBase;
  TADWAYNESS_0_MC_MAIN_STRUCT                 TADWayness;
  UINT8                                       TadIndex;

  UINT32 TadBase[TAD_RULES] = {
      TADBASE_0_MC_MAIN_REG, TADBASE_1_MC_MAIN_REG, TADBASE_2_MC_MAIN_REG, TADBASE_3_MC_MAIN_REG,
      TADBASE_4_MC_MAIN_REG, TADBASE_5_MC_MAIN_REG, TADBASE_6_MC_MAIN_REG, TADBASE_7_MC_MAIN_REG
     } ;

  UINT32 TadWayness[TAD_RULES] = {
      TADWAYNESS_0_MC_MAIN_REG, TADWAYNESS_1_MC_MAIN_REG, TADWAYNESS_2_MC_MAIN_REG, TADWAYNESS_3_MC_MAIN_REG,
      TADWAYNESS_4_MC_MAIN_REG, TADWAYNESS_5_MC_MAIN_REG, TADWAYNESS_6_MC_MAIN_REG, TADWAYNESS_7_MC_MAIN_REG
    };

  SpareCtlReg.Data = ReadCpuCsr (Skt, ChOnSkt, SPARING_CONTROL_MC_MAIN_REG);
  SpareCtlReg.Data = ReadCpuCsr (Skt, ChOnSkt, SPARING_CONTROL_MC_MAIN_REG);

  //
  //calculate min_tad_rule/max_tad_rule in SPARING_CONTROL to take care of all cases for 3 channel complexity
  //
  if (!Is2lmMode) {
    SpareCtlReg.Bits.minimum_tad_rule = 0x0;
    for (TadIndex = 0; TadIndex < TAD_RULES; TadIndex++) {
      TADBase.Data    = ReadCpuCsr (Skt, ChOnSkt, TadBase[TadIndex]);
      TADWayness.Data = ReadCpuCsr (Skt, ChOnSkt, TadWayness[TadIndex]);
      if (TADBase.Bits.base >= TADWayness.Bits.tad_limit) {
        break;
      }
    }
    SpareCtlReg.Bits.maximum_tad_rule = TadIndex-1;
  } else {
    //
    //There is only One TAD rule programmed in 2LM mode to support the entire 2LM memory.
    //The TadBase and limit will only cover NM size
    //
    SpareCtlReg.Bits.minimum_tad_rule = TAD_RULE_ZERO;
    SpareCtlReg.Bits.maximum_tad_rule = TAD_RULE_ZERO;
  }
  WriteCpuCsr (Skt, ChOnSkt, SPARING_CONTROL_MC_MAIN_REG, SpareCtlReg.Data);
}

/**
  Set AMAP register for legacy patrol scrub engine.

  Spare rank is optional; if 0xff is passed in, the field is not updated.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  SpareRank       Rank index to scrub.
**/
VOID
EFIAPI
SetAmapForLegacyPatrol (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT8   SpareRank       OPTIONAL
  )
{
  AMAP_MC_MAIN_STRUCT             AMAPReg;
  UINT32                          SpareDimm;
  UINT32                          SpareRankPerDimm;

  AMAPReg.Data = ReadCpuCsr (Skt, ChOnSkt, AMAP_MC_MAIN_REG);

  AMAPReg.Bits.dimm0_pat_rank_disable = 0x0f;
  AMAPReg.Bits.dimm1_pat_rank_disable = 0x0f;

  if (SpareRank != 0xff) {
    SpareDimm = SpareRank / 4;
    SpareRankPerDimm = SpareRank % 4;

    if (SpareDimm == 0) {
      AMAPReg.Bits.dimm0_pat_rank_disable &= (~(1 << SpareRankPerDimm));
    }
    if (SpareDimm == 1) {
      AMAPReg.Bits.dimm1_pat_rank_disable &= (~(1 << SpareRankPerDimm));
    }
  }

  WriteCpuCsr (Skt, ChOnSkt, AMAP_MC_MAIN_REG, AMAPReg.Data);
}

/**
  Set AMAP register after patrol scrub legacy mode.

  Skipped rank is optional; if 0xff is passed in, the field is not updated.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  AmapData        Original AMAP register value.
  @param[in]  SpareRank       Rank index to scrub.
  @param[in]  SkppedRank      Rank index to be skipped by patrol scrub.
**/
VOID
EFIAPI
SetAmapFromLegacyPatrol (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT32  AmapData,
  IN  UINT8   SpareRank,
  IN  UINT8   SkppedRank      OPTIONAL
  )
{
  AMAP_MC_MAIN_STRUCT             AMAPReg;
  UINT32                          SpareDimm;
  UINT32                          SpareRankPerDimm;
  UINT8                           SkipDimm;
  UINT8                           SkipRankOnDimm;

  AMAPReg.Data = AmapData;

  SpareDimm = SpareRank / 4;
  SpareRankPerDimm = SpareRank % 4;

  if (SpareDimm == 0) {
    AMAPReg.Bits.dimm0_pat_rank_disable &= (~(1 << SpareRankPerDimm));
  }
  if (SpareDimm == 1) {
    AMAPReg.Bits.dimm1_pat_rank_disable &= (~(1 << SpareRankPerDimm));
  }

  if (SkppedRank != 0xff) {
    SkipDimm = SkppedRank / 4;
    SkipRankOnDimm = SkppedRank % 4;

    if (SkipDimm == 0) {
      AMAPReg.Bits.dimm0_pat_rank_disable |= (1 << SkipRankOnDimm);
    }
    if (SkipDimm == 1) {
      AMAPReg.Bits.dimm1_pat_rank_disable |= (1 << SkipRankOnDimm);
    }
  }

  WriteCpuCsr (Skt, ChOnSkt, AMAP_MC_MAIN_REG, AMAPReg.Data);
}

/**
  Check if sparing engine is currently doing sparing copy.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @retval     TRUE            Doing sparing copy.
  @retval     FALSE           Not doing sparing copy.
**/
BOOLEAN
EFIAPI
IsSparingEnable (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  )
{
  SPARING_CONTROL_MC_MAIN_STRUCT              SpareCtlReg;

  SpareCtlReg.Data  = ReadCpuCsr (Skt, ChOnSkt, SPARING_CONTROL_MC_MAIN_REG);

  return (SpareCtlReg.Bits.spare_enable == 1);
}

/**
  Checks for Sgx enabled in Security Policy

  @retval TRUE if SGX feature enabled in Security Policy
  @retval FALSE if SGX feature disabled in Security Policy
**/
BOOLEAN
IsSgxEnabled (
  )
{
  SECURITY_POLICY               *SecurityPolicy;
  SecurityPolicy = GetSecurityPolicy ();
  if ((SecurityPolicy != NULL) && (SecurityPolicy->EnableSgx == SECURITY_POLICY_ENABLE)) {
    return TRUE;
  }
  return FALSE;
}

/**
  Check if sparing engine currently has enable virtual lock step enabled.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @retval     TRUE            Virtual lockstep enabled.
  @retval     FALSE           Virtual lockstep disabled.
**/
BOOLEAN
EFIAPI
IsVirtualLockStepEnable (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  )
{
  return FALSE;
}

/**
  Configure Spare Interval based on the bw_limit_tf.

  @param[in] Socket  - Socket number
  @param[in] Mc      - Mc number in the Socket.
  @param[in] SktChBitMap - Socket channel bit map.

  @retval none
**/
VOID
SetSpareInterval (
  UINT8   Socket,
  UINT8   Mc,
  UINT16  SktChBitMap
  )
{
  return ;
} // SetSpareInterval


/**
  Set sparing control parameters.

  Virtual lockstep, reverse, mirror, and partial mirror are optional;
  if 0xff is passed in, the correpsonding field is not updated.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  RankSparing     Rank sparing flag.
  @param[in]  AdddcSparing    ADDDC sparing flag
  @param[in]  SddcSparing     SDDC Sparing flag
  @param[in]  RegionSize      Sparing region size
  @param[in]  VirtualLockStep Virtual lockstep flag.
  @param[in]  Reverse         Reverse flag.
  @param[in]  Mirror          Mirror flag.
  @param[in]  PartialMirror   Partial mirror flag.
**/
VOID
EFIAPI
SetSparingCtrl (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  BOOLEAN RankSparing,
  IN  BOOLEAN AdddcSparing,
  IN  BOOLEAN SddcSparing,
  IN  UINT8   RegionSize,
  IN  BOOLEAN SpareEnable,
  IN  UINT8   VirtualLockStep, OPTIONAL
  IN  UINT8   Reverse,        OPTIONAL
  IN  UINT8   Mirror,         OPTIONAL
  IN  UINT8   PartialMirror   OPTIONAL
  )
{
#ifndef GNRSRF_HOST // GNRSRF_TODO - 22011066572
  SPARING_CONTROL_MC_MAIN_STRUCT              SpareCtlReg;
  UINT32                                      Command;

  SpareCtlReg.Data = ReadCpuCsr (Skt, ChOnSkt, SPARING_CONTROL_MC_MAIN_REG);

 //   SpareCtlRegSkx.Bits.channel_select  = Ch;

  SpareCtlReg.Bits.rank_sparing    = RankSparing;
  SpareCtlReg.Bits.sddc_sparing    = SddcSparing;
  SpareCtlReg.Bits.adddc_sparing   = AdddcSparing;
  SpareCtlReg.Bits.region_size     = RegionSize;
  SpareCtlReg.Bits.spare_enable    = SpareEnable;

  if (Reverse != 0xff) {
    SpareCtlReg.Bits.spare_reverse = Reverse;
  }

  if (Mirror != 0xff) {
    SpareCtlReg.Bits.mirr_adddc_en = Mirror;
  }

  if (PartialMirror != 0xff) {
    SpareCtlReg.Bits.partial_mirr_en = PartialMirror;
  }

  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL) && IsSgxEnabled ()) {
    Command = MAILBOX_BIOS_CR_WRITE_CMD (Mc, SKTCH_TO_NODECH (ChOnSkt), MAILBOX_BIOS_CMD_CR_PROXY_SPARING_CONTROL_INDEX);
    ReadWritePcuMailbox (Skt, MAILBOX_BIOS_CMD_ZERO, Command, &SpareCtlReg.Data, 0, 0);
  } else {
    WriteCpuCsr (Skt, ChOnSkt, SPARING_CONTROL_MC_MAIN_REG, SpareCtlReg.Data);
  }
#endif // GNRSRF_HOST
}

/**
  Clear the 'reverse' bit in sparing control register.

  @param[in]  Socket          Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
**/
VOID
EFIAPI
ClearReverseBit (
  IN  UINT8   Socket,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  )
{
  SPARING_CONTROL_MC_MAIN_STRUCT              SpareCtlReg;
  UINT32                                      Command;

  SpareCtlReg.Data = ReadCpuCsr (Socket, ChOnSkt, SPARING_CONTROL_MC_MAIN_REG);

  if (SpareCtlReg.Bits.spare_reverse == 1) {
    SpareCtlReg.Bits.spare_reverse = 0;
    if (IsCpuAndRevision (CPU_SPRSP, REV_ALL) && IsSgxEnabled ()) {
      Command = MAILBOX_BIOS_CR_WRITE_CMD (Mc, SKTCH_TO_NODECH (ChOnSkt), MAILBOX_BIOS_CMD_CR_PROXY_SPARING_CONTROL_INDEX);
      ReadWritePcuMailbox (Socket, MAILBOX_BIOS_CMD_ZERO, Command, &SpareCtlReg.Data, 0, 0);
    } else {
      WriteCpuCsr (Socket, ChOnSkt, SPARING_CONTROL_MC_MAIN_REG, SpareCtlReg.Data);
    }
  }
}

/**
  Check if VLS is triggered.

  @param[in]  Socket          Socket index.
  @param[in]  Mc              Memory controller on socket.

  @retval     TRUE            VLS is triggered.
  @retval     FALSE           VLS is not triggered.
**/
BOOLEAN
EFIAPI
CheckVlsTriggered (
  IN  UINT8   Socket,
  IN  UINT8   Mc
  )
{
#ifndef GNRSRF_HOST // GNRSRF_TODO - 22011066572
  BOOLEAN                              IsVlsTriggered = FALSE;
  SPARING_CONTROL_MC_MAIN_STRUCT       SpareCtlReg;

  SpareCtlReg.Data = ReadCpuCsr (Socket, Mc * MAX_MC_CH, SPARING_CONTROL_MC_MAIN_REG);
  //
  // Do VLS only if: MC0_CR_SPARING_CONTROL adddc_sparing or sddc_sparing to check. And also to check spare_enable == 1.
  //
  if (SpareCtlReg.Bits.spare_enable && (SpareCtlReg.Bits.sddc_sparing || SpareCtlReg.Bits.adddc_sparing)) {
    IsVlsTriggered = TRUE;
  }

  return IsVlsTriggered;
#else // !GNRSRF_HOST
  return FALSE;
#endif // !GNRSRF_HOST
}

/**
  Set sparing control source register.

  Sub-rank, bank, buddy CS, and buddy CS enable are optional;
  if 0xff is passed in, the correpsonding field is not updated.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  Rank            Rank index to set.
  @param[in]  SubRank         Sub-rank index to set.
  @param[in]  Bank            Bank group in [3:2], index in group in [1:0].
  @param[in]  BuddyCs         Buddy rank.
  @param[in]  BuddyCsEn       Buddy rank enable.
**/
VOID
EFIAPI
SetSparingCtlSrc (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  UINT8   Rank,
  IN  UINT8   SubRank,        OPTIONAL
  IN  UINT8   Bank,           OPTIONAL
  IN  UINT8   BuddyCs,        OPTIONAL
  IN  UINT8   BuddyCsEn       OPTIONAL
  )
{
  SPARING_CONTROL_SOURCE_MC_MAIN_STRUCT   SparingCtlSrc;

  SparingCtlSrc.Data = ReadCpuCsr (Skt, ChOnSkt, SPARING_CONTROL_SOURCE_MC_MAIN_REG);

  SparingCtlSrc.Bits.source_cs = Rank;


  if (Bank != 0xff) {
    SparingCtlSrc.Bits.source_ba = Bank & 03;
    SparingCtlSrc.Bits.source_bg = (Bank & 0x0c ) >> 2;
  }

  if (BuddyCs != 0xff) {
    SparingCtlSrc.Bits.buddy_cs = BuddyCs;
  }

  if (BuddyCsEn != 0xff) {
    SparingCtlSrc.Bits.buddy_cs_en = BuddyCsEn;
  }

  WriteCpuCsr (Skt, ChOnSkt, SPARING_CONTROL_SOURCE_MC_MAIN_REG, SparingCtlSrc.Data);
}

/**
  Set sparing control destination register.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  Rank            Destination Rank for sparing copy.
**/
VOID
EFIAPI
SetSparingCtlDest (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  UINT8   Rank
  )
{
  SPARING_CONTROL_DESTINATION_MC_MAIN_STRUCT   SpareCtlDest;

  SpareCtlDest.Data = (UINT16) ReadCpuCsr (Skt, ChOnSkt, SPARING_CONTROL_DESTINATION_MC_MAIN_REG);
  SpareCtlDest.Bits.destination_cs = Rank;

  WriteCpuCsr (Skt, ChOnSkt, SPARING_CONTROL_DESTINATION_MC_MAIN_REG, SpareCtlDest.Data);
}

/**
  Set spare_rank_diff value of CMPL_TO_DATA_DELAY.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  SpareRankDiff   Spare rank different value to set.
**/
VOID
EFIAPI
SetSpareRankDiff (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT8   SpareRankDiff
  )
{
#ifndef GNRSRF_HOST // GNRSRF_TODO - 22011066572
  CMPL_TO_DATA_DELAY_MCDDC_DP_STRUCT        CmplToDataDelay;

  CmplToDataDelay.Data = ReadCpuCsr (Skt, ChOnSkt, CMPL_TO_DATA_DELAY_MCDDC_DP_REG);
  CmplToDataDelay.Bits.spare_rank_diff = SpareRankDiff;

  WriteCpuCsr (Skt, ChOnSkt, CMPL_TO_DATA_DELAY_MCDDC_DP_REG, CmplToDataDelay.Data);
#endif // !GNRSRF_HOST
}

/**
  Get link fail value of scrub mask.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @return     Current link fail value.
**/
UINT8
EFIAPI
GetLinkFail (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  )
{
  SCRUBMASK_MC_MAIN_STRUCT                ScrubMask;

  ScrubMask.Data =  ReadCpuCsr (Skt, ChOnSkt, SCRUBMASK_MC_MAIN_REG);
  return (UINT8)(ScrubMask.Bits.link_fail);
}

/**
  Set link fail value of scrub mask.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  LinkFail        Link fail value to set.
**/
VOID
EFIAPI
SetLinkFail (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  UINT8   LinkFail
  )
{
  SCRUBMASK_MC_MAIN_STRUCT                ScrubMask;

  ScrubMask.Data =  ReadCpuCsr (Skt, ChOnSkt, SCRUBMASK_MC_MAIN_REG);
  ScrubMask.Bits.link_fail = LinkFail;
  WriteCpuCsr (Skt, ChOnSkt, SCRUBMASK_MC_MAIN_REG, ScrubMask.Data);
}

/**
  Get scrub enable.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @return     Current scrub enable value.
 */
UINT8
EFIAPI
GetScrubEnable (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  )
{
  SCRUBCTL_MC_MAIN_STRUCT                 ScrubCtl;

  ScrubCtl.Data = ReadCpuCsr (Skt, ChOnSkt, SCRUBCTL_MC_MAIN_REG);
  return (UINT8)ScrubCtl.Bits.scrub_en;
}

/**
  Set scrub enable.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  ScrubEn         Scrub enable value to set.
 */
VOID
EFIAPI
SetScrubEnable (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  UINT8   ScrubEn
  )
{
  SCRUBCTL_MC_MAIN_STRUCT                 ScrubCtl;

  ScrubCtl.Data = ReadCpuCsr (Skt, ChOnSkt, SCRUBCTL_MC_MAIN_REG);
  ScrubCtl.Bits.scrub_en = ScrubEn;
  WriteCpuCsr (Skt, ChOnSkt, SCRUBCTL_MC_MAIN_REG, ScrubCtl.Data);
}

/**
  Get scrubCtl Address.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @return     The address of the ScrubCtl Register.
 */
UINT64
EFIAPI
GetScrubCtlAddress (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  )
{
  UINT8  Size = 4;
  return (GetCpuCsrAddress (Skt,  ChOnSkt, SCRUBCTL_MC_MAIN_REG, &Size));
}


/**
  Get scrub enable.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @return   scrubctl register value
 */
UINT32
EFIAPI
GetScrubCtl (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  )
{
  SCRUBCTL_MC_MAIN_STRUCT                 ScrubCtl;

  ScrubCtl.Data = ReadCpuCsr (Skt, ChOnSkt, SCRUBCTL_MC_MAIN_REG);
  return ScrubCtl.Data;
}

/**
  Get scrub enable.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @return  None
 */
VOID
EFIAPI
SetScrubCtl (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  UINT32  Data
  )
{
  WriteCpuCsr (Skt, ChOnSkt, SCRUBCTL_MC_MAIN_REG, Data);
  return;
}


/**
  Get scrub ctrl register information required for patrol.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[out]  *PtlCmpl,      return value PtlCmpl OPTIONAL
  @param[out]  *PtlStopped,   return value PtlStopped OPTIONAL
  @param[out]  *ScrubIssued,  return value ScrubIssued OPTIONAL

  @return  NONE
 */
VOID
EFIAPI
GetScrubCtlForPatrol (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  OUT UINT32  *PtlCmpl,         OPTIONAL
  OUT UINT32  *PtlStopped,       OPTIONAL
  OUT UINT32  *ScrubIssued       OPTIONAL
  )
{
  SCRUBCTL_MC_MAIN_STRUCT                 ScrubCtl;

  ScrubCtl.Data = ReadCpuCsr (Skt, ChOnSkt, SCRUBCTL_MC_MAIN_REG);


  if (PtlCmpl != NULL) {
    *PtlCmpl = ScrubCtl.Bits.ptl_cmpl;
  }

  if (PtlStopped != NULL) {
    *PtlStopped = ScrubCtl.Bits.ptl_stopped;
  }

  if (ScrubIssued != NULL) {
    *ScrubIssued = ScrubCtl.Bits.scrubissued;
  }

  return;
}

/**
  Set scrub ctrl register for patrol.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  StartScrub,     value to program startscrub OPTIONAL
  @param[in]  StopOnErr,      value to program stop_on_err OPTIONAL
  @param[in]  PtlCmpl,        value to program ptl_cmpl OPTIONAL
  @param[in]  StopOnCmpl,     value to program stop_on_cmpl OPTIONAL
  @param[in]  PtlStopped      value to program ptl_stopped OPTIONAL
  @param[in]  IssueOnce       value to program issueonce OPTIONAL

  @return    NONE

 */
VOID
EFIAPI
SetScrubCtlForPatrol (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN UINT32   StartScrub,      OPTIONAL
  IN UINT32   StopOnErr,       OPTIONAL
  IN UINT32   PtlCmpl,         OPTIONAL
  IN UINT32   StopOnCmpl,      OPTIONAL
  IN UINT32   IssueOnce,       OPTIONAL
  IN UINT32   StopOnRank       OPTIONAL
  )
{
  SCRUBCTL_MC_MAIN_STRUCT                 ScrubCtl;

  ScrubCtl.Data = ReadCpuCsr (Skt, ChOnSkt, SCRUBCTL_MC_MAIN_REG);

  if (StartScrub != 0xFF) {
    ScrubCtl.Bits.startscrub = StartScrub;
  }
  if (StopOnErr != 0xFF) {
    ScrubCtl.Bits.stop_on_err = StopOnErr;
  }

  if (PtlCmpl != 0xFF) {
    ScrubCtl.Bits.ptl_cmpl = PtlCmpl;
  }

  if (StopOnCmpl != 0xFF) {
    ScrubCtl.Bits.stop_on_cmpl = StopOnCmpl;
  }

  if (IssueOnce != 0xFF) {
    ScrubCtl.Bits.issueonce = IssueOnce;
  }

#ifndef DDR5_SUPPORT
  if (StopOnRank != 0xFF) {
    ScrubCtl.Bits.stop_on_rank = StopOnRank;
  }
#endif

  WriteCpuCsr (Skt, ChOnSkt, SCRUBCTL_MC_MAIN_REG, ScrubCtl.Data);

  return;
}

/**
  Set MCMTR register.

  ADDDC enable and CTL delay are optional; if 0xff is passed in, the
  correpsonding field is not updated.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  AdddcEn         ADDDC enable.
  @param[in]  CtlDelay        CTL delay enable.
**/
VOID
EFIAPI
SetMcmtr (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  UINT8   AdddcEn,        OPTIONAL
  IN  UINT8   CtlDelay        OPTIONAL
  )
{
  MCMTR_MC_MAIN_STRUCT                    McmtrMain;

  McmtrMain.Data = ReadCpuCsr (Skt, ChOnSkt, MCMTR_MC_MAIN_REG);
 // if (AdddcEn != 0xff) {
 //   McmtrMainSkx.Bits.adddc_mode = AdddcEn;
 // }

  if (CtlDelay != 0xff) {
    McmtrMain.Bits.ch23cmd_ctl_delay = CtlDelay;
  }

  WriteCpuCsr (Skt, ChOnSkt, MCMTR_MC_MAIN_REG, McmtrMain.Data);
}

/**
  Get sparing patrol status register.

  @param[in]  Skt       Socket index.
  @param[in]  Mc        Memory controller on socket.
  @param[in]  ChOnSkt   Channel on socket.

  @return               Sparing patrol status value
**/
UINT32
EFIAPI
GetSparingPatrolStatus (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  )
{
  SPARING_PATROL_STATUS_MC_MAIN_STRUCT      SsrStatusReg;

  SsrStatusReg.Data = ReadCpuCsr (Skt, ChOnSkt, SPARING_PATROL_STATUS_MC_MAIN_REG);

  return SsrStatusReg.Data;
}

/**
  Clear Patrol complete status

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @return  NONE
 */
VOID
EFIAPI
ClearPatrolStatus (
    IN  UINT8   Skt,
    IN  UINT8   Mc,
    IN  UINT8   ChOnSkt
  )
{
  SPARING_PATROL_STATUS_MC_MAIN_STRUCT        SsrStatusReg;

  SsrStatusReg.Data = ReadCpuCsr (Skt, ChOnSkt, SPARING_PATROL_STATUS_MC_MAIN_REG);
  SsrStatusReg.Bits.patrol_complete = 1;
  WriteCpuCsr (Skt, ChOnSkt, SPARING_PATROL_STATUS_MC_MAIN_REG, SsrStatusReg.Data);
}

/**
  Clear spare copy status

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @return  NONE
 */
VOID
EFIAPI
ClearSpareCopyStatus (
    IN  UINT8   Skt,
    IN  UINT8   Mc,
    IN  UINT8   ChOnSkt
  )
{
  SPARING_PATROL_STATUS_MC_MAIN_STRUCT        SsrStatusReg;

  SsrStatusReg.Data = ReadCpuCsr (Skt, ChOnSkt, SPARING_PATROL_STATUS_MC_MAIN_REG);
  SsrStatusReg.Bits.copy_complete = 0;
  WriteCpuCsr (Skt, ChOnSkt, SPARING_PATROL_STATUS_MC_MAIN_REG, SsrStatusReg.Data);
}

/**
  Configure patrol error log disable feature

  @param[in]  Skt              Socket index.
  @param[in]  Mc               Memory controller on socket.
  @param[in]  ChOnSkt          Channel on socket.
  @param[in]  Dis              True -  disable patrol error log
                               False - enable patrol error log

  @return  NONE
**/
VOID
EFIAPI
SetPatrolErrLogDisable (
  IN  UINT8     Skt,
  IN  UINT8     Mc,
  IN  UINT8     ChOnSkt,
  IN  BOOLEAN   Dis
  )
{
#ifdef SPR_HOST
  LINK_MCA_CTL_MCDDC_DP_STRUCT       LinkMcaCtlReg;
#else
  MCMAIN_CHKN_BITS_MC_MAIN_STRUCT    McMainChknReg;
#endif

#ifdef SPR_HOST
  LinkMcaCtlReg.Data = ReadCpuCsr (Skt, ChOnSkt, LINK_MCA_CTL_MCDDC_DP_REG);
  LinkMcaCtlReg.Bits.dis_ptrl_err_log = Dis;  // Disable patrol from MCA logging errors when scrubbing spare rank
  WriteCpuCsr (Skt, Mc * MAX_MC_CH, LINK_MCA_CTL_MCDDC_DP_REG, LinkMcaCtlReg.Data);
#else
  McMainChknReg.Data = ReadCpuCsr (Skt, ChOnSkt, MCMAIN_CHKN_BITS_MC_MAIN_REG);
  McMainChknReg.Bits.dis_ptrl_err_log = Dis;  // Disable patrol from MCA logging errors when scrubbing spare rank
  WriteCpuCsr (Skt, ChOnSkt, MCMAIN_CHKN_BITS_MC_MAIN_REG, McMainChknReg.Data);
#endif
}
