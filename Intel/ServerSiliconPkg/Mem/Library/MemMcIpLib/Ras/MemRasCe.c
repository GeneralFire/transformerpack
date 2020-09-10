/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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
#include "Include/MemMcRegs.h"
#include "MemHostChipCommon.h"
#include <SiliconSetting.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/MemMcIpRasLib.h>
#include <Include/Ras/RasCommon.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/PcuMailBoxRasLib.h>

#ifndef GNRSRF_HOST // GNRSRF_TODO - 22011066572
CONST UINT32 mPclsN0Offset[MAX_PCLS_REGION] = {
  PCLS_0_CFG_DATA_INFO_N0_MCDDC_DP_REG,  PCLS_1_CFG_DATA_INFO_N0_MCDDC_DP_REG,
  PCLS_2_CFG_DATA_INFO_N0_MCDDC_DP_REG,  PCLS_3_CFG_DATA_INFO_N0_MCDDC_DP_REG,
  PCLS_4_CFG_DATA_INFO_N0_MCDDC_DP_REG,  PCLS_5_CFG_DATA_INFO_N0_MCDDC_DP_REG,
  PCLS_6_CFG_DATA_INFO_N0_MCDDC_DP_REG,  PCLS_7_CFG_DATA_INFO_N0_MCDDC_DP_REG,
  PCLS_8_CFG_DATA_INFO_N0_MCDDC_DP_REG,  PCLS_9_CFG_DATA_INFO_N0_MCDDC_DP_REG,
  PCLS_10_CFG_DATA_INFO_N0_MCDDC_DP_REG, PCLS_11_CFG_DATA_INFO_N0_MCDDC_DP_REG,
  PCLS_12_CFG_DATA_INFO_N0_MCDDC_DP_REG, PCLS_13_CFG_DATA_INFO_N0_MCDDC_DP_REG,
  PCLS_14_CFG_DATA_INFO_N0_MCDDC_DP_REG, PCLS_15_CFG_DATA_INFO_N0_MCDDC_DP_REG
};

CONST UINT32 mPclsN1Offset[MAX_PCLS_REGION] = {
  PCLS_0_CFG_DATA_INFO_N1_MCDDC_DP_REG,  PCLS_1_CFG_DATA_INFO_N1_MCDDC_DP_REG,
  PCLS_2_CFG_DATA_INFO_N1_MCDDC_DP_REG,  PCLS_3_CFG_DATA_INFO_N1_MCDDC_DP_REG,
  PCLS_4_CFG_DATA_INFO_N1_MCDDC_DP_REG,  PCLS_5_CFG_DATA_INFO_N1_MCDDC_DP_REG,
  PCLS_6_CFG_DATA_INFO_N1_MCDDC_DP_REG,  PCLS_7_CFG_DATA_INFO_N1_MCDDC_DP_REG,
  PCLS_8_CFG_DATA_INFO_N1_MCDDC_DP_REG,  PCLS_9_CFG_DATA_INFO_N1_MCDDC_DP_REG,
  PCLS_10_CFG_DATA_INFO_N1_MCDDC_DP_REG, PCLS_11_CFG_DATA_INFO_N1_MCDDC_DP_REG,
  PCLS_12_CFG_DATA_INFO_N1_MCDDC_DP_REG, PCLS_13_CFG_DATA_INFO_N1_MCDDC_DP_REG,
  PCLS_14_CFG_DATA_INFO_N1_MCDDC_DP_REG, PCLS_15_CFG_DATA_INFO_N1_MCDDC_DP_REG
};
#endif // !GNRSRF_HOST

#define ADDDC_REGION_CONTROL_MC_MAIN_LIST   {ADDDC_REGION0_CONTROL_MC_MAIN_REG, ADDDC_REGION1_CONTROL_MC_MAIN_REG, \
                                              ADDDC_REGION2_CONTROL_MC_MAIN_REG, ADDDC_REGION3_CONTROL_MC_MAIN_REG}

#define CORRERRTHRSHLD_MCDDC_DP_LIST        {CORRERRTHRSHLD_0_MCDDC_DP_REG, CORRERRTHRSHLD_1_MCDDC_DP_REG, CORRERRTHRSHLD_2_MCDDC_DP_REG, \
                                              CORRERRTHRSHLD_3_MCDDC_DP_REG}

#define TADBASE_MC_MAIN_LIST                {TADBASE_0_MC_MAIN_REG, TADBASE_1_MC_MAIN_REG, TADBASE_2_MC_MAIN_REG, \
                                              TADBASE_3_MC_MAIN_REG, TADBASE_4_MC_MAIN_REG, TADBASE_5_MC_MAIN_REG, \
                                                TADBASE_6_MC_MAIN_REG, TADBASE_7_MC_MAIN_REG}

#define TADWAYNESS_LIST                     {TADWAYNESS_0_MC_MAIN_REG, TADWAYNESS_1_MC_MAIN_REG, TADWAYNESS_2_MC_MAIN_REG,\
                                               TADWAYNESS_3_MC_MAIN_REG, TADWAYNESS_4_MC_MAIN_REG, TADWAYNESS_5_MC_MAIN_REG,\
                                                 TADWAYNESS_6_MC_MAIN_REG, TADWAYNESS_7_MC_MAIN_REG}

#define ADDDC_REGION_MCDDC_DP_LIST          {ADDDC_REGION0_MCDDC_DP_REG, ADDDC_REGION1_MCDDC_DP_REG, \
                                              ADDDC_REGION2_MCDDC_DP_REG, ADDDC_REGION3_MCDDC_DP_REG}

#define SCHEDULER_ADDDC_REGION_LIST           {SCHEDULER_ADDDC_REGION_CONTROL_0_MCDDC_CTL_REG, SCHEDULER_ADDDC_REGION_CONTROL_1_MCDDC_CTL_REG, \
                                                SCHEDULER_ADDDC_REGION_CONTROL_2_MCDDC_CTL_REG, SCHEDULER_ADDDC_REGION_CONTROL_3_MCDDC_CTL_REG}


UINT32 mAdddcRegionMcddcDpReg[] = ADDDC_REGION_MCDDC_DP_LIST;
UINT32 mSchAdddcRegionMcddcDpReg[] = SCHEDULER_ADDDC_REGION_LIST;
UINT32 mAdddcCtrlReg[]          = ADDDC_REGION_CONTROL_MC_MAIN_LIST;
UINT32 mTadbaseMcMainReg[]      = TADBASE_MC_MAIN_LIST;
UINT32 mTadWayness[]            = TADWAYNESS_LIST;

#define CORRERRCNT_MCDDC_DP_LIST    {CORRERRCNT_0_MCDDC_DP_REG, CORRERRCNT_1_MCDDC_DP_REG, \
                                     CORRERRCNT_2_MCDDC_DP_REG, CORRERRCNT_3_MCDDC_DP_REG}


/**
  Get DIMM alert information.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.

  @return     DIMM alert information.
**/
UINT32
EFIAPI
GetDimmAlert (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt
  )
{
  CORRERRORSTATUS_MCDDC_DP_STRUCT     CorrErrSts;

  CorrErrSts.Data = ReadCpuCsr (Skt, ChOnSkt, CORRERRORSTATUS_MCDDC_DP_REG);
  return CorrErrSts.Bits.dimm_alert;
}

/**
  Clear CAP error status.

  @param[in]  Socket          Socket index.
  @param[in]  ChOnSkt         Channel on socket.
**/
VOID
EFIAPI
ClearCAPErrorPerCh (
  IN  UINT8   Socket,
  IN  UINT8   ChOnSkt
  )
{
  CORRERRORSTATUS_MCDDC_DP_STRUCT           CorrErrSts;
  LINK_MCA_CTL_MCDDC_DP_STRUCT              LinkMcaCtl;

  LinkMcaCtl.Data = ReadCpuCsr (Socket, ChOnSkt, LINK_MCA_CTL_MCDDC_DP_REG);
  LinkMcaCtl.Bits.err4_log = 1;
  WriteCpuCsr (Socket, ChOnSkt, LINK_MCA_CTL_MCDDC_DP_REG, LinkMcaCtl.Data);

  CorrErrSts.Data = ReadCpuCsr (Socket, ChOnSkt, CORRERRORSTATUS_MCDDC_DP_REG);
  CorrErrSts.Bits.dimm_alert = 0;
  //
  // the err_overflow_stat bit is per rank and W1C; RMW can actually clear the bits, zero out the bits
  //
  CorrErrSts.Bits.err_overflow_stat = 0;
  WriteCpuCsr (Socket, ChOnSkt, CORRERRORSTATUS_MCDDC_DP_REG, CorrErrSts.Data);
}


/**
  Get current channel and rank info of patrol scrub complete.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[out] ChOnSkt         Channel on socket.
  @param[out] Rank            Rank index.
**/
EFI_STATUS
EFIAPI
GetScrubbedRank (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  OUT UINT8   *ChOnSkt,
  OUT UINT8   *Rank
  )
{
  UINT8  ChOnMc;
  UINT32 PtlComplete;
  SCRUBADDRESSHI_MC_MAIN_STRUCT   ScrubAddrHiReg;
  SPARING_PATROL_STATUS_MC_MAIN_STRUCT  SsrStatusReg;

  //
  // Only One channel in MC can have an active scrub/spare in progress. Exit on finding first ch
  //
  for (ChOnMc = 0; ChOnMc< MAX_MC_CH; ChOnMc++) {
    *ChOnSkt = NODECHA_TO_SKTCHA(Mc,ChOnMc);
    GetScrubCtlForPatrol (Skt, Mc, *ChOnSkt, &PtlComplete, NULL, NULL); // Skt,Mc,ChOnSkt,PtlCmpl,PtlStopped,ScrubIssued
    SsrStatusReg.Data = GetSparingPatrolStatus (Skt, Mc, *ChOnSkt);
    if (GetScrubEnable (Skt, Mc, *ChOnSkt)) {
      if (PtlComplete && (SsrStatusReg.Bits.patrol_complete)) {
        ScrubAddrHiReg.Data = ReadCpuCsr (Skt, *ChOnSkt, SCRUBADDRESSHI_MC_MAIN_REG);
        *Rank = (UINT8) ScrubAddrHiReg.Bits.rank;
        return EFI_SUCCESS;
      }
    }
  }
  return EFI_INVALID_PARAMETER;
}

/**
  Get tad range for the TAD.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         channel on socket.
  @param[in]  TadIndex        Tad index.
  @param[out] TadBase         Tad base.
  @param[out] TadLimit        Tad limit.
**/
VOID
EFIAPI
GetTadInfoRange (
  IN     UINT8      Skt,
  IN     UINT8      ChOnSkt,
  IN     UINT8      TadIndex,
  IN OUT UINT64    *TadBase,
  IN OUT UINT64    *TadLimit
  )
{
  TADBASE_0_MC_MAIN_STRUCT          TadBaseReg;
  TADWAYNESS_0_MC_MAIN_STRUCT       TadWayness;

  if (TadBase != NULL) {
    TadBaseReg.Data = ReadCpuCsr (Skt, ChOnSkt, mTadbaseMcMainReg[TadIndex]);

    *TadBase = LShiftU64 (TadBaseReg.Bits.base, 26);
  }

  if (TadLimit != NULL) {
    TadWayness.Data = ReadCpuCsr (Skt, ChOnSkt, mTadWayness[TadIndex]);

    *TadLimit = LShiftU64 (TadWayness.Bits.tad_limit + 1, 26);
  }
}

/**
  Set ADDDC region register in decoder.

  Failed bank and non-failed bank are optional; if 0xff is passed in, the
  corresponding field is not updated.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  RegionNum       ADDDC region index.
  @param[in]  FainRank        Failed rank.
  @param[in]  FailSubRank     Failed sub-rank.
  @param[in]  NonFailRank     Non-failed rank.
  @param[in]  NonFailSubRank  Non-failed sub-rank.
  @param[in]  RegionSize      Region size.
  @param[in]  FailedBank      Failed bank.
  @param[in]  NonFailedBank   Non-failed bank.
**/
VOID
EFIAPI
SetAdddcRegion (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT8   RegionNum,
  IN  UINT8   FailRank,
  IN  UINT8   FailSubRank,
  IN  UINT8   NonFailRank,
  IN  UINT8   NonFailSubRank,
  IN  UINT8   RegionSize,
  IN  UINT8   FailedBank,       OPTIONAL
  IN  UINT8   NonFailedBank     OPTIONAL
  )
{
  ADDDC_REGION0_CONTROL_MC_MAIN_STRUCT        AdddcCtrRegion;
  UINT32                                      AdddcCtrlRegionoffset;

  if(RegionNum < (sizeof(mAdddcCtrlReg) / sizeof(mAdddcCtrlReg[0]))) {
    AdddcCtrlRegionoffset = (mAdddcCtrlReg[RegionNum]);

    AdddcCtrRegion.Data = ReadCpuCsr (Skt, ChOnSkt, AdddcCtrlRegionoffset);
    AdddcCtrRegion.Bits.region_enable    = 1;

    AdddcCtrRegion.Bits.failed_cs        = FailRank;
    AdddcCtrRegion.Bits.failed_c         = FailSubRank;
    AdddcCtrRegion.Bits.nonfailed_cs     = NonFailRank;
    AdddcCtrRegion.Bits.nonfailed_c      = NonFailSubRank;
    AdddcCtrRegion.Bits.region_size      = RegionSize;

    if (FailedBank != 0xff) {
      AdddcCtrRegion.Bits.failed_ba        = BANK_IN_GROUP (FailedBank);
      AdddcCtrRegion.Bits.failed_bg        = BANK_TO_GROUP (FailedBank);
    }

    if (NonFailedBank != 0xff) {
      AdddcCtrRegion.Bits.nonfailed_ba     = BANK_IN_GROUP (NonFailedBank);
      AdddcCtrRegion.Bits.nonfailed_bg     = BANK_TO_GROUP (NonFailedBank);
    }

    AdddcCtrRegion.Bits.copy_in_progress = 1;
    WriteCpuCsr (Skt, ChOnSkt, AdddcCtrlRegionoffset, AdddcCtrRegion.Data);
  }
}

/**
  Set ADDDC region register in scheduler.

  Failed bank and non-failed bank are optional; if 0xff is passed in, the
  corresponding field is not updated.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  RegionNum       ADDDC region index.
  @param[in]  FainRank        Failed rank.
  @param[in]  FailSubRank     Failed sub-rank.
  @param[in]  NonFailRank     Non-failed rank.
  @param[in]  NonFailSubRank  Non-failed sub-rank.
  @param[in]  RegionSize      Region size.
  @param[in]  FailedBank      Failed bank.
  @param[in]  NonFailedBank   Non-failed bank.
**/
VOID
EFIAPI
SetSchedulerAdddcRegion (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT8   RegionNum,
  IN  UINT8   FailRank,
  IN  UINT8   FailSubRank,
  IN  UINT8   NonFailRank,
  IN  UINT8   NonFailSubRank,
  IN  UINT8   RegionSize,
  IN  UINT8   FailedBank,       OPTIONAL
  IN  UINT8   NonFailedBank     OPTIONAL
  )
{
  SCHEDULER_ADDDC_REGION_CONTROL_0_MCDDC_CTL_STRUCT        SchAdddcCtrRegion;
  UINT32                                               SchAdddcCtrlRegionoffset;

  if(RegionNum < (sizeof(mSchAdddcRegionMcddcDpReg) / sizeof(mSchAdddcRegionMcddcDpReg[0]))) {
    SchAdddcCtrlRegionoffset = (mSchAdddcRegionMcddcDpReg[RegionNum]);

    SchAdddcCtrRegion.Data = ReadCpuCsr (Skt, ChOnSkt, SchAdddcCtrlRegionoffset);
    SchAdddcCtrRegion.Bits.region_enable    = 1;

    SchAdddcCtrRegion.Bits.failed_cs        = FailRank;
    SchAdddcCtrRegion.Bits.failed_c         = FailSubRank;
    SchAdddcCtrRegion.Bits.nonfailed_cs     = NonFailRank;
    SchAdddcCtrRegion.Bits.nonfailed_c      = NonFailSubRank;
    SchAdddcCtrRegion.Bits.region_size      = RegionSize;

    if (FailedBank != 0xff) {
      SchAdddcCtrRegion.Bits.failed_ba        = BANK_IN_GROUP (FailedBank);
      SchAdddcCtrRegion.Bits.failed_bg        = BANK_TO_GROUP (FailedBank);
    }

    if (NonFailedBank != 0xff) {
      SchAdddcCtrRegion.Bits.nonfailed_ba     = BANK_IN_GROUP (NonFailedBank);
      SchAdddcCtrRegion.Bits.nonfailed_bg     = BANK_TO_GROUP (NonFailedBank);
    }

    SchAdddcCtrRegion.Bits.copy_in_progress = 1;
    WriteCpuCsr (Skt, ChOnSkt, SchAdddcCtrlRegionoffset, SchAdddcCtrRegion.Data);
  }
}


/**
  Get Failed device to be mapped for this ADDDC region.


  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  RegionNum       ADDDC region index.
  @param[OUT] Faildevice      Failed device to be mapped out for this adddc region.
**/
UINT8
EFIAPI
GetAdddcRegionFaildevice (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT8   RegionNum
  )
{
  UINT8 Faildevice;

  Faildevice = ((UINT8) ReadCpuCsr (Skt, ChOnSkt, mAdddcRegionMcddcDpReg[RegionNum])) & 0x3F;

  return Faildevice;
}




/**
  Clear the 'region_enable' bit of the ADDDC region control register.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  RegionNum       ADDDC region index.
**/
VOID
EFIAPI
FreeAdddcRegion (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT8   RegionNum
  )
{
  ADDDC_REGION0_CONTROL_MC_MAIN_STRUCT            AdddcCtrRegion;
  UINT32                                          AdddcCtrlRegionoffset;


  if(RegionNum < (sizeof(mAdddcCtrlReg) / sizeof(mAdddcCtrlReg[0]))) {
    AdddcCtrlRegionoffset = (mAdddcCtrlReg[RegionNum]);

    AdddcCtrRegion.Data = ReadCpuCsr (Skt, ChOnSkt, AdddcCtrlRegionoffset);
    if (AdddcCtrRegion.Bits.region_enable == 1) {
      AdddcCtrRegion.Bits.region_enable = 0;
      WriteCpuCsr (Skt, ChOnSkt, AdddcCtrlRegionoffset, AdddcCtrRegion.Data);
    }
  }
}

/**
  Get ADDDC region register.

  All output parameters are optional. The caller may pass NULL for parameters
  that it does not need.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  RegionNum       ADDDC region index.
  @param[out] RegionEnable    ADDDC region enable.
  @param[out] FailRank        Failed rank.
  @param[out] FailSubRank     Failed sub-rank.
  @param[out] NonFailRank     Non-failed rank.
  @param[out] NonFailSubRank  Non-failed sub-rank.
  @param[out] RegionSize      Region size.
  @param[out] FailedBank      Failed bank.
  @param[out] NonFailedBank   Non-failed bank.
**/
VOID
EFIAPI
GetAdddcRegion (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT8   RegionNum,
  OUT UINT8   *RegionEnable,    OPTIONAL
  OUT UINT8   *FailRank,        OPTIONAL
  OUT UINT8   *FailSubRank,     OPTIONAL
  OUT UINT8   *NonFailRank,     OPTIONAL
  OUT UINT8   *NonFailSubRank,  OPTIONAL
  OUT UINT8   *RegionSize,      OPTIONAL
  OUT UINT8   *FailedBank,      OPTIONAL
  OUT UINT8   *NonFailedBank    OPTIONAL
  )
{
  ADDDC_REGION0_CONTROL_MC_MAIN_STRUCT        AdddcCtrRegion;
  UINT32                                      AdddcCtrlRegionoffset;

  if(RegionNum < (sizeof(mAdddcCtrlReg) / sizeof(mAdddcCtrlReg[0]))) {
    AdddcCtrlRegionoffset = (mAdddcCtrlReg[RegionNum]);

    AdddcCtrRegion.Data = ReadCpuCsr (Skt, ChOnSkt, AdddcCtrlRegionoffset);

    if (RegionEnable != NULL) {
      *RegionEnable = (UINT8)AdddcCtrRegion.Bits.region_enable;
    }

    if (FailSubRank != NULL) {
      *FailSubRank = (UINT8)AdddcCtrRegion.Bits.failed_c;
    }

    if (FailRank != NULL) {
      *FailRank = (UINT8)AdddcCtrRegion.Bits.failed_cs;
    }

    if (NonFailRank != NULL) {
      *NonFailRank = (UINT8)AdddcCtrRegion.Bits.nonfailed_cs;
    }

    if (NonFailSubRank != NULL) {
      *NonFailSubRank = (UINT8)AdddcCtrRegion.Bits.nonfailed_c;
    }

    if (RegionSize != NULL) {
      *RegionSize = (UINT8)AdddcCtrRegion.Bits.region_size;
    }

    if (FailedBank != NULL) {
      *FailedBank = (UINT8)AdddcCtrRegion.Bits.failed_ba | (UINT8)(AdddcCtrRegion.Bits.failed_bg << 2);
    }

    if (NonFailedBank != NULL) {
      *NonFailedBank = (UINT8)AdddcCtrRegion.Bits.nonfailed_ba | (UINT8)(AdddcCtrRegion.Bits.nonfailed_bg << 2);
    }
  }
}

/**
  Set error injection high match register.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  Addr            Match address to be set.
**/
VOID
EFIAPI
SetErrorInjMatchHi (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT64  Addr
  )
{
  RSP_FUNC_ADDR_MATCH_HI_MC_MAIN_STRUCT         MatchHiReg;

  MatchHiReg.Data = ReadCpuCsr (Skt, ChOnSkt, RSP_FUNC_ADDR_MATCH_HI_MC_MAIN_REG);
  MatchHiReg.Bits.addr_match_higher = (UINT32) RShiftU64 (Addr, 0x23);
  MatchHiReg.Bits.rsp_func_addr_match_en  = 01;
  WriteCpuCsr (Skt, ChOnSkt, RSP_FUNC_ADDR_MATCH_HI_MC_MAIN_REG, MatchHiReg.Data );
}

/**
  Set error injection high mask register.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  Mask            Mask to be set.
**/
VOID
EFIAPI
SetErrorInjMaskHi (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt,
  IN  UINT64  Mask
  )
{
  RSP_FUNC_ADDR_MASK_HI_MC_MAIN_STRUCT         MaskHiReg;

  MaskHiReg.Data = ReadCpuCsr (Skt, ChOnSkt, RSP_FUNC_ADDR_MASK_HI_MC_MAIN_REG);
  MaskHiReg.Bits.addr_mask_higher = 00;
  WriteCpuCsr (Skt, ChOnSkt, RSP_FUNC_ADDR_MASK_HI_MC_MAIN_REG, MaskHiReg.Data);

}

/**
  Set dis_tgr_inj in RANK_BANK_MATCH Register

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.

**/
VOID
EFIAPI
SetDisTgrInj (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt
  )
{
  RSP_FUNC_RANK_BANK_MATCH_MC_MAIN_STRUCT       RankBankMatchReg;

  RankBankMatchReg.Data = ReadCpuCsr (Skt, ChOnSkt, RSP_FUNC_RANK_BANK_MATCH_MC_MAIN_REG);
  RankBankMatchReg.Bits.dis_tgr_inj = 1;
  WriteCpuCsr (Skt, ChOnSkt, RSP_FUNC_RANK_BANK_MATCH_MC_MAIN_REG, RankBankMatchReg.Data );
}

/**
  Set ADDDC error injection register.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
**/
VOID
EFIAPI
SetAdddcErrorInj (
  IN  UINT8   Skt,
  IN  UINT8   ChOnSkt
  )
{
  RSP_FUNC_CRC_ERR_INJ_EXTRA_MCDDC_DP_STRUCT  AdddcErrInj;

  AdddcErrInj.Data = ReadCpuCsr (Skt, ChOnSkt, RSP_FUNC_CRC_ERR_INJ_EXTRA_MCDDC_DP_REG);
  AdddcErrInj.Bits.crc_err_inj_adddc_primary0 = 0;
  AdddcErrInj.Bits.crc_err_inj_adddc_buddy0   = 0;
  AdddcErrInj.Bits.crc_err_inj_adddc_primary1 = 0;
  AdddcErrInj.Bits.crc_err_inj_adddc_buddy1   = 0;
  WriteCpuCsr (Skt, ChOnSkt, RSP_FUNC_CRC_ERR_INJ_EXTRA_MCDDC_DP_REG, AdddcErrInj.Data);
}

/**
  Get VLS t_ccd_wr value.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.

  @return     VLS t_ccd_wr value.
**/
UINT32
EFIAPI
GetVlsSparingTccdWr (
  IN  UINT8    Skt,
  IN  UINT8    ChOnSkt
  )
{
  return 0;
}

/**
  Check if sparing engine is currently doing ADDDC sparing copy.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @retval     TRUE            Doing ADDDC sparing copy.
  @retval     FALSE           Not doing ADDDC sparing copy.
**/
BOOLEAN
EFIAPI
IsAdddcSparing (
  IN  UINT8       Skt,
  IN  UINT8       Mc,
  IN  UINT8       ChOnSkt
  )
{
  SPARING_CONTROL_MC_MAIN_STRUCT              SpareCtlReg;

  SpareCtlReg.Data  = ReadCpuCsr (Skt, ChOnSkt, SPARING_CONTROL_MC_MAIN_REG);

  return (SpareCtlReg.Bits.adddc_sparing == 1);
}

/**
  Check if sparing engine is currently doing rank sparing copy.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @retval     TRUE            Doing rank sparing copy.
  @retval     FALSE           Not doing rank sparing copy.
**/
BOOLEAN
EFIAPI
IsRankSparing (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  )
{
#ifndef GNRSRF_HOST // GNRSRF_TODO - 22011066572
  SPARING_CONTROL_MC_MAIN_STRUCT              SpareCtlReg;

  SpareCtlReg.Data  = ReadCpuCsr (Skt, ChOnSkt, SPARING_CONTROL_MC_MAIN_REG);

  return (SpareCtlReg.Bits.rank_sparing == 1);
#else // !GNRSRF_HOST
  return FALSE;
#endif // !GNRSRF_HOST
}

/**
  Check if sparing engine is currently doing SDDC sparing copy.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @retval     TRUE            Doing SDDC sparing copy.
  @retval     FALSE           Not doing SDDC sparing copy.
**/
BOOLEAN
EFIAPI
IsSddcSparing (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  )
{
#ifndef GNRSRF_HOST // GNRSRF_TODO - 22011066572
  SPARING_CONTROL_MC_MAIN_STRUCT              SpareCtlReg;

  SpareCtlReg.Data  = ReadCpuCsr (Skt, ChOnSkt, SPARING_CONTROL_MC_MAIN_REG);

  return (SpareCtlReg.Bits.sddc_sparing == 1);
#else // !GNRSRF_HOST
  return FALSE;
#endif // !GNRSRF_HOST
}

/**
  Check if patrol scrub is complete.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @retval     TRUE            Patrol scrub is complete.
  @retval     FALSE           Patrol scrub is not complete.
**/
BOOLEAN
EFIAPI
IsPatrolScrubComplete (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  )
{
  SCRUBCTL_MC_MAIN_STRUCT           ScrubCtlReg;
  ScrubCtlReg.Data  = ReadCpuCsr (Skt, ChOnSkt, SCRUBCTL_MC_MAIN_REG);

  return (ScrubCtlReg.Bits.stop_on_cmpl && ScrubCtlReg.Bits.scrub_en == 1);
}

/**
  Check if mirror mode is enabled.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.

  @retval     TRUE            Mirror mode is enabled.
  @retval     FALSE           Mirror mode is disabled.
**/
BOOLEAN
EFIAPI
IsMirrorEanbled (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt
  )
{
  RASENABLES_MC_MAIN_STRUCT         RasEnables;

  RasEnables.Data  = ReadCpuCsr (Skt, ChOnSkt, RASENABLES_MC_MAIN_REG);

  return (RasEnables.Bits.mirroren == 1);
}

/**
  Set corrected error threshold to max.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[out] Rank            Rank index.
**/
VOID
EFIAPI
SetThresholdMax (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  UINT8   Rank
  )
{
   CORRERRTHRSHLD_0_MCDDC_DP_STRUCT  imcCORRERRTHRSHLD01;
   UINT32                            CorrErrThrshldMcddcReg[] = CORRERRTHRSHLD_MCDDC_DP_LIST;

  if((Rank >> 1) < (sizeof(CorrErrThrshldMcddcReg) / sizeof(CorrErrThrshldMcddcReg[0]))) {
    imcCORRERRTHRSHLD01.Data = ReadCpuCsr (Skt, ChOnSkt, CorrErrThrshldMcddcReg[(Rank >> 1)]);
    if(Rank % 2){
      imcCORRERRTHRSHLD01.Bits.cor_err_th_1 = 0x7fff;
    }else{
      imcCORRERRTHRSHLD01.Bits.cor_err_th_0 = 0x7fff;
    }
    WriteCpuCsr (Skt, ChOnSkt, CorrErrThrshldMcddcReg[(Rank >> 1)], imcCORRERRTHRSHLD01.Data);
  }
}

/**
  Disable rank corrected error SMI.

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket.
  @param[out] Rank            Rank index.
**/
VOID
EFIAPI
DisableRankCorrError (
  IN  UINT8   Skt,
  IN  UINT8   Mc,
  IN  UINT8   ChOnSkt,
  IN  UINT8   Rank
  )
{
  DIS_CORR_ERR_LOG_MCDDC_DP_STRUCT     CorrErrLog;
  UINT32                               Command;

  if (IsSiliconWorkaroundEnabled ("S1409866138")) {
    Command = MAILBOX_BIOS_CR_READ_CMD (Mc, SKTCH_TO_NODECH (ChOnSkt), MAILBOX_BIOS_CMD_CR_PROXY_DIS_CORR_ERR_LOG_MCDDC_DP_INDEX);
    ReadWritePcuMailbox (Skt, Command, MAILBOX_BIOS_CMD_ZERO, &CorrErrLog.Data, 0, 0);
  } else {
    CorrErrLog.Data = ReadCpuCsr (Skt, ChOnSkt, DIS_CORR_ERR_LOG_MCDDC_DP_REG);
  }

  CorrErrLog.Data |= (1 << Rank);

  if (IsSiliconWorkaroundEnabled ("S1409866138")) {
    Command = MAILBOX_BIOS_CR_WRITE_CMD (Mc, SKTCH_TO_NODECH (ChOnSkt), MAILBOX_BIOS_CMD_CR_PROXY_DIS_CORR_ERR_LOG_MCDDC_DP_INDEX);
    ReadWritePcuMailbox (Skt, MAILBOX_BIOS_CMD_ZERO, Command, &CorrErrLog.Data, 0, 0);
  } else {
    WriteCpuCsr (Skt, ChOnSkt, DIS_CORR_ERR_LOG_MCDDC_DP_REG, CorrErrLog.Data);
  }
}

/**
  Enable PCLS region.

  @param[in]  Socket          Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  PclsIndex       Index of PCLS register.
  @param[in]  ChipSelect      Rank ID.
  @param[in]  Cbit            Sub-rank ID.
  @param[in]  Bg              Bank group.
  @param[in]  Ba              Bank address.
  @param[in]  Row             Row.
  @param[in]  Col             Column.
  @param[in]  DataOffset      Offset of nibble to be spared.
**/
VOID
EFIAPI
EnablePcls (
  IN  UINT8   Socket,
  IN  UINT8   ChOnSkt,
  IN  UINT16  PclsIndex,
  IN  UINT32  ChipSelect,
  IN  UINT32  Cbit,
  IN  UINT32  Bg,
  IN  UINT32  Ba,
  IN  UINT32  Row,
  IN  UINT32  Col,
  IN  UINT32  DataOffset
  )
{
#ifndef GNRSRF_HOST // GNRSRF_TODO - 22011066572
  PCLS_0_CFG_DATA_INFO_N0_MCDDC_DP_STRUCT PclsN0;
  PCLS_0_CFG_DATA_INFO_N1_MCDDC_DP_STRUCT PclsN1;

  PclsN0.Data  = ReadCpuCsr (Socket, ChOnSkt, mPclsN0Offset[PclsIndex]);
  PclsN1.Data  = ReadCpuCsr (Socket, ChOnSkt, mPclsN1Offset[PclsIndex]);

  PclsN0.Bits.pcls_enable   = 1;
  PclsN0.Bits.chip_select   = ChipSelect;
  PclsN0.Bits.cbit          = Cbit;
  PclsN0.Bits.bg            = Bg;
  PclsN0.Bits.ba            = Ba;
  PclsN0.Bits.row           = Row;
  PclsN1.Bits.col           = Col;
  PclsN1.Bits.data_offset   = DataOffset;

  WriteCpuCsr (Socket, ChOnSkt, mPclsN1Offset[PclsIndex], PclsN1.Data);
  WriteCpuCsr (Socket, ChOnSkt, mPclsN0Offset[PclsIndex], PclsN0.Data);
  return;
#endif // !GNRSRF_HOST
}

/**
  Disable (reverse) PCLS region.

  @param[in]  Socket          Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  PclsIndex       Index of PCLS register.
**/
VOID
EFIAPI
DisablePcls (
  IN  UINT8   Socket,
  IN  UINT8   ChOnSkt,
  IN  UINT16  PclsIndex
  )
{
#ifndef GNRSRF_HOST // GNRSRF_TODO - 22011066572
  PCLS_0_CFG_DATA_INFO_N0_MCDDC_DP_STRUCT PclsN0;

  PclsN0.Data  = ReadCpuCsr (Socket, ChOnSkt, mPclsN0Offset[PclsIndex]);
  PclsN0.Bits.pcls_enable   = 0;
  WriteCpuCsr (Socket, ChOnSkt, mPclsN0Offset[PclsIndex], PclsN0.Data);
  return;
#endif // !GNRSRF_HOST
}

/**
  Check if it is duplicate PCLS.

  @param[in]  NodeId          Node index.
  @param[in]  ChOnNode        Channel on node.
  @param[in]  ChipSelect      Rank ID.
  @param[in]  Cbit            Sub-rank ID.
  @param[in]  Bg              Bank group.
  @param[in]  Ba              Bank address.
  @param[in]  Row             Row.
  @param[in]  Col             Column.

  @retval     TRUE            Duplicate PCLS.
  @retval     FALSE           Non-duplicate PCLS.
**/
BOOLEAN
EFIAPI
IsDuplicatePcls (
  IN  UINT8   NodeId,
  IN  UINT8   ChOnNode,
  IN  UINT32  ChipSelect,
  IN  UINT32  Cbit,
  IN  UINT32  Bg,
  IN  UINT32  Ba,
  IN  UINT32  Row,
  IN  UINT32  Col
  )
{
#ifndef GNRSRF_HOST // GNRSRF_TODO - 22011066572
  PCLS_0_CFG_DATA_INFO_N0_MCDDC_DP_STRUCT PclsN0;
  PCLS_0_CFG_DATA_INFO_N1_MCDDC_DP_STRUCT PclsN1;
  UINT8                                   PclsIndex;
  UINT8                                   Socket;
  UINT8                                   ChOnSkt;

  Socket  = NODE_TO_SKT (NodeId);
  ChOnSkt = NODECHA_TO_SKTCHA (NodeId, ChOnNode);

  for (PclsIndex = 0; PclsIndex < MAX_PCLS_REGION ; PclsIndex++) {
    PclsN0.Data  = ReadCpuCsr (Socket, ChOnSkt, mPclsN0Offset[PclsIndex]);
    if (PclsN0.Bits.pcls_enable) {
      PclsN1.Data  = ReadCpuCsr (Socket, ChOnSkt, mPclsN1Offset[PclsIndex]);
      if ((ChipSelect == PclsN0.Bits.chip_select) && (Cbit == PclsN0.Bits.cbit) && (Bg == PclsN0.Bits.bg) \
          && (Ba == PclsN0.Bits.ba) && (Row == PclsN0.Bits.row) && (Col == PclsN1.Bits.col)) {
        return TRUE;
      }
    }
  }
#endif // !GNRSRF_HOST

  return FALSE;
}

/**
  Workaround to downgrade patrol scrubbing, correctable -> uncorreactable errors for this TAD range for ICX

  @param[in]  Socket          - Socket number
  @param[in]  Mc              - Mc number

  @retval EFI_SUCCESS  if successful
  @retval EFI_UNSUPPORTED if unsuccessful

**/
UINT8
EFIAPI
TadRestorePsSeverity (
  UINT8 Socket,
  UINT8 Mc
  )
{
  UINT8                             TadIndex;
  UINT8                             Ch;
  TADBASE_0_MC_MAIN_STRUCT          TadBase;

  for (Ch = 0; Ch < MAX_MC_CH; Ch++) {
    for (TadIndex = 0; TadIndex < TAD_RULES; TadIndex++) {
      if (TadIndex < ARRAY_SIZE (mTadbaseMcMainReg)) {
        TadBase.Data = ReadCpuCsr (Socket, NODECHA_TO_SKTCHA (Mc, Ch), mTadbaseMcMainReg[TadIndex]);
        if (TadBase.Bits.mirror_en && TadBase.Bits.en_failover && TadBase.Bits.ign_ptrl_uc) {
          //
          // Workaround for TADdowngrade
          //
          TadBase.Bits.ign_ptrl_uc = 0;

          WriteCpuCsr (Socket, NODECHA_TO_SKTCHA (Mc, Ch), mTadbaseMcMainReg[TadIndex], TadBase.Data);
        } // Mirror enabled & failover on Tad
      } else {
        break;
      }
    } // Tad loop
  } //Ch loop

  return 0;
}

/**
  Is this current TAD mirrored

  @param[in]  Socket          - Socket number
  @param[in]  Mc              - Mc number
  @param[in]  ChOnSkt         - Ch on socket number
  @param[in]  TadIndex        - Tad Index

  @retval TRUE  if TAD is mirrored
  @retval FALSE if TAD is not mirrored

**/
BOOLEAN
EFIAPI
IsTadMirrored (
  UINT8 Socket,
  UINT8 Mc,
  UINT8 ChOnSkt,
  UINT8 TadIndex
  )
{
  TADBASE_0_MC_MAIN_STRUCT          TadBase;

  TadBase.Data = ReadCpuCsr (Socket, ChOnSkt, mTadbaseMcMainReg[TadIndex]);

  if (TadBase.Bits.mirror_en) {
    return TRUE;
  }
  return FALSE;
}

/**
  Is this current TAD valid

  @param[in]  Socket          - Socket number
  @param[in]  Mc              - Mc number
  @param[in]  ChOnSkt         - Ch on socket number
  @param[in]  TadIndex        - Tad Index

  @retval TRUE  if TAD is Valid
  @retval FALSE if TAD is not Valid

**/
BOOLEAN
EFIAPI
IsTadValid (
  UINT8 Socket,
  UINT8 Mc,
  UINT8 ChOnSkt,
  UINT8 TadIndex
  )
{
  TADWAYNESS_0_MC_MAIN_STRUCT       McTadWayness;

  McTadWayness.Data = ReadCpuCsr (Socket, ChOnSkt, mTadWayness[TadIndex]);

  if (McTadWayness.Bits.tad_limit > 0) { //Tad valid
    return TRUE;
  }
  return FALSE;
}

/**
  Set error threshold value for per rank threshold register to the input threhsold value

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  Rank            Rank number
  @param[in]  ErrThreshold    Threshold value to be programmed into the per rank threshold register

  @retval N/A

**/
VOID
EFIAPI
SetErrorThresholdPerRank (
  IN  UINT8   Socket,
  IN  UINT8   ChOnSkt,
  IN  UINT8   Rank,
  IN  UINT32  ErrThrshld
  )
{
  UINT32  CorErrThrshldList [] = CORRERRTHRSHLD_MCDDC_DP_LIST;

  CORRERRTHRSHLD_0_MCDDC_DP_STRUCT  CorrErrThrshld;
  if ((Rank >> 1) < ARRAY_SIZE (CorErrThrshldList)) {
    CorrErrThrshld.Data  = ReadCpuCsr (Socket, ChOnSkt, CorErrThrshldList[Rank >> 1]);
    if (Rank % 2) {
      CorrErrThrshld.Bits.cor_err_th_1 = ErrThrshld;
    } else {
      CorrErrThrshld.Bits.cor_err_th_0 = ErrThrshld;
    }
    WriteCpuCsr (Socket, ChOnSkt, CorErrThrshldList[Rank >> 1], CorrErrThrshld.Data);
  }
  return;
}

/**
Enable VmseMcaCtl Err0

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket

  @retval N/A
**/
VOID
EFIAPI
EnableVmseMcaCtlErr0 (
  IN  UINT8   Socket,
  IN  UINT8   ChOnSkt
  )
{
  LINK_MCA_CTL_MCDDC_DP_STRUCT              VmseMcaCtl;

  VmseMcaCtl.Data = ReadCpuCsr (Socket, ChOnSkt, LINK_MCA_CTL_MCDDC_DP_REG);
  VmseMcaCtl.Bits.err0_en = 1;
  VmseMcaCtl.Bits.err0_log = 1;
  VmseMcaCtl.Bits.err0_smi = 1;
  WriteCpuCsr (Socket, ChOnSkt, LINK_MCA_CTL_MCDDC_DP_REG, VmseMcaCtl.Data);
}

/**
Check if VmseMcaCtl Err0 is Enabled

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket

  @retval TRUE  if VmseMcaCtl Err0 is Enabled
  @retval FALSE if VmseMcaCtl Err0 is Disabled
**/
BOOLEAN
EFIAPI
IsVmseMcaCtlErr0Enabled (
  IN  UINT8   Socket,
  IN  UINT8   ChOnSkt
  )
{
  LINK_MCA_CTL_MCDDC_DP_STRUCT              VmseMcaCtl;

  VmseMcaCtl.Data = ReadCpuCsr (Socket, ChOnSkt, LINK_MCA_CTL_MCDDC_DP_REG);
  if (VmseMcaCtl.Bits.err0_log == 1) {
    return TRUE;
  }

  return FALSE;
}

/**
Clear VmseMcaCtl Err0

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket

  @retval N/A
**/
VOID
EFIAPI
ClearVmseMcaCtlErr0Log (
  IN  UINT8   Socket,
  IN  UINT8   ChOnSkt
  )
{
  LINK_MCA_CTL_MCDDC_DP_STRUCT              VmseMcaCtl;

  VmseMcaCtl.Data = ReadCpuCsr (Socket, ChOnSkt, LINK_MCA_CTL_MCDDC_DP_REG);
  VmseMcaCtl.Bits.err0_log = 1;
  WriteCpuCsr (Socket, ChOnSkt, LINK_MCA_CTL_MCDDC_DP_REG, VmseMcaCtl.Data);
}

/**
Update TadBaseForMirrorFailover register

  @param[in]  Skt             Socket index.
  @param[in]  Mc              Memory controller on socket.
  @param[in]  ChOnSkt         Channel on socket
  @param[in]  FailedCh        Failed Channel
  @param[in]  TadIndex        Tad Index

  @retval N/A
**/
VOID
EFIAPI
UpdateTadBaseForMirrorFailover (
  IN  UINT8 Socket,
  IN  UINT8 ChOnSkt,
  IN  UINT8 FailedCh,
  IN  UINT8 TadIndex
  )
{
  TADBASE_0_MC_MAIN_STRUCT            TadBase;

  if (TadIndex < ARRAY_SIZE (mTadbaseMcMainReg)) {
    TadBase.Data = ReadCpuCsr (Socket, ChOnSkt, mTadbaseMcMainReg[TadIndex]);
    if (TadBase.Bits.mirror_en) {

      TadBase.Bits.en_failover = 1;
      TadBase.Bits.failed_ch = FailedCh;

      WriteCpuCsr (Socket, ChOnSkt, mTadbaseMcMainReg[TadIndex], TadBase.Data);
    } // Mirror enabled on Tad
  }
}

/**
Disable LinkFail

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket


  @retval N/A
**/
VOID
EFIAPI
DisableLinkFail (
  IN  UINT8 Socket,
  IN  UINT8 ChOnSkt
  )
{
  LINK_RETRY_ERR_LIMITS_MCDDC_DP_STRUCT   LinkRetryErrLimit;

  LinkRetryErrLimit.Data = ReadCpuCsr (Socket, ChOnSkt, LINK_RETRY_ERR_LIMITS_MCDDC_DP_REG);
  LinkRetryErrLimit.Bits.sb_err_enable = 0;
  WriteCpuCsr (Socket, ChOnSkt, LINK_RETRY_ERR_LIMITS_MCDDC_DP_REG, LinkRetryErrLimit.Data);
}


/**
Get EFIAPILinkErrFsm Current State

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket

  @retval Current State of LinkErrFsmState
**/
UINT32
EFIAPI
GetLinkErrFsmCurrentState (
  IN  UINT8 Socket,
  IN  UINT8 ChOnSkt
  )
{
  LINK_ERR_FSM_STATE_MCDDC_DP_STRUCT      LinkErrFsmState;
  LinkErrFsmState.Data = ReadCpuCsr (Socket, ChOnSkt, LINK_ERR_FSM_STATE_MCDDC_DP_REG);
  return LinkErrFsmState.Bits.current_state;
}

/**
Get DdrtRetryFsm Current State

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket

  @retval Current State of DdrtRetryFsm
**/
UINT32
EFIAPI
GetDdrtRetryFsmCurrentState (
  IN  UINT8 Socket,
  IN  UINT8 ChOnSkt
  )
{
  DDRT_RETRY_FSM_STATE_MC_2LM_STRUCT      DdrtRetryFsm;
  DdrtRetryFsm.Data = ReadCpuCsr (Socket, ChOnSkt, DDRT_RETRY_FSM_STATE_MC_2LM_REG);
  return DdrtRetryFsm.Bits.current_state;
}

/**
Adjust DDRT RD Credit Value

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket
  @param[in]  RdCreditValue   RD Credit Value

  @retval N/A
**/
VOID
EFIAPI
AdjustDdrtRdCreditValue (
  IN  UINT8   Socket,
  IN  UINT8   ChOnSkt,
  IN  UINT32  RdCreditValue
  )
{
  DDRT_CREDIT_LIMIT_MC_2LM_STRUCT     DdrtCreditLimit;

  DdrtCreditLimit.Data = ReadCpuCsr (Socket, ChOnSkt, DDRT_CREDIT_LIMIT_MC_2LM_REG);
  DdrtCreditLimit.Bits.ddrt_rd_credit = RdCreditValue;
  WriteCpuCsr (Socket, ChOnSkt, DDRT_CREDIT_LIMIT_MC_2LM_REG, DdrtCreditLimit.Data);
}

/**
Update Scrub Ctl value and Return Previous value

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket
  @param[in]  ScrubEn         ScrubEn Value

  @retval ScrubEnSave
**/
UINT32
EFIAPI
UpdateScrubCtl (
  IN  UINT8   Socket,
  IN  UINT8   ChOnSkt,
  IN  UINT32  ScrubEn
  )
{
  SCRUBCTL_MC_MAIN_STRUCT  ScrubCtl;
  UINT32                   ScrubEnSave;

  ScrubCtl.Data = ReadCpuCsr (Socket, ChOnSkt, SCRUBCTL_MC_MAIN_REG);
  ScrubEnSave = ScrubCtl.Bits.scrub_en;
  ScrubCtl.Bits.scrub_en = ScrubEn;
  WriteCpuCsr (Socket, ChOnSkt, SCRUBCTL_MC_MAIN_REG, ScrubCtl.Data);

  return ScrubEnSave;
}

/**
  Set error counter value for the rank.

  @param[in]  Socket          Socket index.
  @param[in]  ChOnSkt         Channel on socket.
  @param[in]  Rank            Rank number
  @param[in]  ErrorCount      ErrorCount value to be programmed.

  @retval None

**/
VOID
SetErrorCounterPerRank (
  IN  UINT8   Socket,
  IN  UINT8   ChOnSkt,
  IN  UINT8   Rank,
  IN  UINT16  ErrorCount
  )
{
  CORRERRCNT_0_MCDDC_DP_STRUCT    CorrErrCntReg;
  UINT32                          CorrErrCntRegBaseAddrList[] = CORRERRCNT_MCDDC_DP_LIST;

  if ((Rank >> 1) < ARRAY_SIZE (CorrErrCntRegBaseAddrList)) {
    CorrErrCntReg.Data = ReadCpuCsr (Socket, ChOnSkt, CorrErrCntRegBaseAddrList[(Rank >> 1)]);
    if (Rank % 2) {
      CorrErrCntReg.Bits.cor_err_cnt_1 = 0;
    } else {
      CorrErrCntReg.Bits.cor_err_cnt_0 = 0;
    }
    WriteCpuCsr (Socket, ChOnSkt, CorrErrCntRegBaseAddrList[(Rank >> 1)], CorrErrCntReg.Data);
  }
}
