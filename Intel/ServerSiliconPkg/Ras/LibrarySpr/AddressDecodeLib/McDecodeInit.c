/** @file
  Implementation of mc decode initialization.

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
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <IioUniversalData.h>
#include <RcRegs.h>
#include <Mem/Library/MemMcIpLib/Include/MemMcRegs.h>
#include <Library/CpuCsrAccessLib.h>
#include "AddressDecodeInternal.h"

extern AD_SOCKET_INFO              *mSadTadScope;

UINT32 mChTadRegs[MAX_TAD * 2] = {
  TADCHNILVOFFSET_0_N0_MC_MAIN_REG,  TADCHNILVOFFSET_0_N1_MC_MAIN_REG,
  TADCHNILVOFFSET_1_N0_MC_MAIN_REG,  TADCHNILVOFFSET_1_N1_MC_MAIN_REG,
  TADCHNILVOFFSET_2_N0_MC_MAIN_REG,  TADCHNILVOFFSET_2_N1_MC_MAIN_REG,
  TADCHNILVOFFSET_3_N0_MC_MAIN_REG,  TADCHNILVOFFSET_3_N1_MC_MAIN_REG,
  TADCHNILVOFFSET_4_N0_MC_MAIN_REG,  TADCHNILVOFFSET_4_N1_MC_MAIN_REG,
  TADCHNILVOFFSET_5_N0_MC_MAIN_REG,  TADCHNILVOFFSET_5_N1_MC_MAIN_REG,
  TADCHNILVOFFSET_6_N0_MC_MAIN_REG,  TADCHNILVOFFSET_6_N1_MC_MAIN_REG,
  TADCHNILVOFFSET_7_N0_MC_MAIN_REG,  TADCHNILVOFFSET_7_N1_MC_MAIN_REG,
  TADCHNILVOFFSET_8_N0_MC_MAIN_REG,  TADCHNILVOFFSET_8_N1_MC_MAIN_REG,
  TADCHNILVOFFSET_9_N0_MC_MAIN_REG,  TADCHNILVOFFSET_9_N1_MC_MAIN_REG,
  TADCHNILVOFFSET_10_N0_MC_MAIN_REG, TADCHNILVOFFSET_10_N1_MC_MAIN_REG,
  TADCHNILVOFFSET_11_N0_MC_MAIN_REG, TADCHNILVOFFSET_11_N1_MC_MAIN_REG
};

UINT32  mMcDramRuleRegs[MAX_MC_DRAM_RULE * 2] = {
  DRAM_RULE_CFG0_N0_MC_MAIN_REG,DRAM_RULE_CFG0_N1_MC_MAIN_REG,
  DRAM_RULE_CFG1_N0_MC_MAIN_REG,DRAM_RULE_CFG1_N1_MC_MAIN_REG,
  DRAM_RULE_CFG2_N0_MC_MAIN_REG,DRAM_RULE_CFG2_N1_MC_MAIN_REG,
  DRAM_RULE_CFG3_N0_MC_MAIN_REG,DRAM_RULE_CFG3_N1_MC_MAIN_REG,
  DRAM_RULE_CFG4_N0_MC_MAIN_REG,DRAM_RULE_CFG4_N1_MC_MAIN_REG,
  DRAM_RULE_CFG5_N0_MC_MAIN_REG,DRAM_RULE_CFG5_N1_MC_MAIN_REG,
  DRAM_RULE_CFG6_N0_MC_MAIN_REG,DRAM_RULE_CFG6_N1_MC_MAIN_REG,
  DRAM_RULE_CFG7_N0_MC_MAIN_REG,DRAM_RULE_CFG7_N1_MC_MAIN_REG,
};

UINT32 mRirWaynessLimit[MAX_RIR] = {
    RIRWAYNESSLIMIT_0_MC_MAIN_REG,
    RIRWAYNESSLIMIT_1_MC_MAIN_REG,
    RIRWAYNESSLIMIT_2_MC_MAIN_REG,
    RIRWAYNESSLIMIT_3_MC_MAIN_REG
};

UINT32  mRirInterleaveOffset[MAX_RIR * MAX_ILV] = {
  RIRILV0OFFSET_0_MC_MAIN_REG,  RIRILV1OFFSET_0_MC_MAIN_REG,  RIRILV2OFFSET_0_MC_MAIN_REG,  RIRILV3OFFSET_0_MC_MAIN_REG,
  RIRILV4OFFSET_0_MC_MAIN_REG,  RIRILV5OFFSET_0_MC_MAIN_REG,  RIRILV6OFFSET_0_MC_MAIN_REG,  RIRILV7OFFSET_0_MC_MAIN_REG,

  RIRILV0OFFSET_1_MC_MAIN_REG,  RIRILV1OFFSET_1_MC_MAIN_REG,  RIRILV2OFFSET_1_MC_MAIN_REG,  RIRILV3OFFSET_1_MC_MAIN_REG,
  RIRILV4OFFSET_1_MC_MAIN_REG,  RIRILV5OFFSET_1_MC_MAIN_REG,  RIRILV6OFFSET_1_MC_MAIN_REG,  RIRILV7OFFSET_1_MC_MAIN_REG,

  RIRILV0OFFSET_2_MC_MAIN_REG,  RIRILV1OFFSET_2_MC_MAIN_REG,  RIRILV2OFFSET_2_MC_MAIN_REG,  RIRILV3OFFSET_2_MC_MAIN_REG,
  RIRILV4OFFSET_2_MC_MAIN_REG,  RIRILV5OFFSET_2_MC_MAIN_REG,  RIRILV6OFFSET_2_MC_MAIN_REG,  RIRILV7OFFSET_2_MC_MAIN_REG,

  RIRILV0OFFSET_3_MC_MAIN_REG,  RIRILV1OFFSET_3_MC_MAIN_REG,  RIRILV2OFFSET_3_MC_MAIN_REG,  RIRILV3OFFSET_3_MC_MAIN_REG,
  RIRILV4OFFSET_3_MC_MAIN_REG,  RIRILV5OFFSET_3_MC_MAIN_REG,  RIRILV6OFFSET_3_MC_MAIN_REG,  RIRILV7OFFSET_3_MC_MAIN_REG
};


UINT32 mDdrtRirWaynessLimit[MAX_DDRT_RIR] = {
    FMRIRWAYNESSLIMIT_0_MC_2LM_REG ,
    FMRIRWAYNESSLIMIT_1_MC_2LM_REG ,
    FMRIRWAYNESSLIMIT_2_MC_2LM_REG ,
    FMRIRWAYNESSLIMIT_3_MC_2LM_REG
};


UINT32  mDdrtRirInterleaveOffset[MAX_DDRT_RIR * MAX_DDRT_ILV] = {
  FMRIRILV0OFFSET_0_MC_2LM_REG,  FMRIRILV1OFFSET_0_MC_2LM_REG,
  FMRIRILV0OFFSET_1_MC_2LM_REG,  FMRIRILV1OFFSET_1_MC_2LM_REG,
  FMRIRILV0OFFSET_2_MC_2LM_REG,  FMRIRILV1OFFSET_2_MC_2LM_REG,
  FMRIRILV0OFFSET_3_MC_2LM_REG,  FMRIRILV1OFFSET_3_MC_2LM_REG

};


UINT32 mDimmMtrOffset[MAX_DIMM_NUM] = {
    DIMMMTR_0_MC_MAIN_REG, DIMMMTR_1_MC_MAIN_REG
  };


/**
  check whether this DIMM belong to HBM type.

  @param[in]    Socket    --   socket index
  @param[in]    Mc        --   MC index


  @retval   TRUE -- HBM dimm
 **/
BOOLEAN
EFIAPI
IsHbmDimm (
  IN      UINT8     Socket,
  IN      UINT8     Mc
  )
{
  return mSadTadScope[Socket].McInfo[Mc].HbmMc;
}

/**
  Get channel tad info.

   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index
   @param[in,out]TargetWay --   target ways
   @param[in,out]TargetGran --  target granularity
   @param[in,out] ChWay  -- channel ways
   @param[in,out] ChGran -- Channel granularity
   @param[in,out] TadOffset -- tad offset
   @param[in,out] TadSig -- Tad sign

  @retval        None

**/
BOOLEAN
EFIAPI
GetTad (
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Channel,
  IN      UINT8     TadIndex,
  IN OUT  UINT8     *TargetWay,
  IN OUT  UINT8     *TargetGran,
  IN OUT  UINT8     *ChWay,
  IN OUT  UINT8     *ChGran,
  IN OUT  UINT64    *TadOffset,
  IN OUT  UINT8     *TadSign
  )
{
  TADCHNILVOFFSET_0_N0_MC_MAIN_STRUCT   TadChIlv0;
  TADCHNILVOFFSET_0_N1_MC_MAIN_STRUCT   TadChIlv1;
  UINT64                                TadData;

  if (TadIndex < MAX_TAD) {

    TadChIlv0.Data = ReadCpuCsr (Socket, GetSktCh(Mc, Channel), mChTadRegs[TadIndex * 2]);
    TadChIlv1.Data = ReadCpuCsr (Socket, GetSktCh(Mc, Channel), mChTadRegs[TadIndex * 2 + 1]);
  } else {
    return FALSE;
  }


  //DEBUG ((EFI_D_INFO, "  [GetTad] Skt:%d Mc:%d  Chn:%d  TadIndex:%d Tad0:%x  Tad1:%x \n", Socket, Mc, Channel, TadIndex, TadChIlv0.Data, TadChIlv1.Data));

  TadData = LShiftU64 (TadChIlv1.Data, 32) | TadChIlv0.Data;

  if (TargetWay != NULL) {
    switch (TadChIlv0.Bits.target_ways) {
      case 0:
        *TargetWay = 1;
        break;

      case 1:
        *TargetWay = 2;
        break;

      case 2:
        *TargetWay = 4;
        break;

      case 3:
        *TargetWay = 8;
        break;

      case 4:
        *TargetWay = 16;
        break;

      default:
       DEBUG ((EFI_D_ERROR, "[GetTad] Wrong target ways:%d \n", TadChIlv0.Bits.target_ways));
       return FALSE;
    }
  }

  if (TargetGran != NULL) {
    switch (TadChIlv0.Bits.target_gran) {
      case 1:
        *TargetGran = 8; //256 byte
        break;
      case 2:
        *TargetGran = 12; //4k byte
        break;

      case 7:
        *TargetGran = 11; //2k byte
        break;

      default:
       DEBUG ((EFI_D_ERROR, "[GetTad] Wrong target grans:%d \n", TadChIlv0.Bits.target_gran));
       return FALSE;
    }


  }

  if (ChWay != NULL) {

    switch (TadChIlv0.Bits.chn_ways) {
      case 0:
        *ChWay = 1;
        break;

      case 1:
        *ChWay = 2;
        break;

      case 2:
        *ChWay = 3;
        break;

      case 3:
        *ChWay = 8;
        break;

      case 4:
        *ChWay = 4;
        break;

      default:
        DEBUG ((EFI_D_ERROR, "[GetTad] Wrong channel ways:%d \n", TadChIlv0.Bits.chn_ways));
        return FALSE;
    }
  }
  if (ChGran != NULL) {
    switch (TadChIlv0.Bits.chn_gran) {
      case 1:
        *ChGran = 8; //256 byte
        break;
      case 2:
        *ChGran = 12; //4k byte
        break;

      case 7:
        *ChGran = 11; //2k byte
        break;

      default:
       DEBUG ((EFI_D_ERROR, "[GetTad] Wrong chn grans:%d \n", TadChIlv0.Bits.chn_gran));
       return FALSE;
    }
  }

  if (TadOffset != NULL) {
    *TadOffset = LShiftU64(BitFieldRead64(TadData, 14, 39) , SAD_UNIT) ;
  }

  if (TadSign != NULL) {
    *TadSign = (UINT8)TadChIlv1.Bits.tad_offset_sign;
  }
  return TRUE;
}


/**
  This routine is to get NM chache reg used for NM.

   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index
   @param[in,out]  NmTargetWay  -- nm target way
   @param[in,out]  NmChnWay  -- nm channel way
   @param[in,out]  NmTargetGran  -- nm target granularity
   @param[in,out]  NmChnGran  -- nm channel granularity
   @param[in,out]  NmTargetLid  -- nm target logic id
   @param[in,out]  NmChnLid  -- nm chn id
   @param[in,out]  NmCachingOffset  -- nm caching offset
   @param[in,out]  NmChnCap  -- nm channel cap
   @param[in,out]  NmRatioChnCap  -- nm chn cap ratio

   @ret   TRUE -- NM TAD valid.
 **/
BOOLEAN
EFIAPI
GetNmCaching (
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Channel,
  IN      UINT8     *NmTargetWay,
  IN      UINT8     *NmChnWay,
  IN      UINT8     *NmTargetGran,
  IN      UINT8     *NmChnGran,
  IN      UINT8     *NmTargetLid,
  IN      UINT8     *NmChnLid,
  IN      UINT64    *NmCachingOffset,
  IN      UINT8     *NmChnCap,
  IN      UINT8     *NmRatioChnCap
  )
{
  MCNMCACHINGINTLV_MC_2LM_STRUCT    NmCachingIntLv;
  MCNMCACHINGOFFSET_MC_2LM_STRUCT   NmCachingOffsetData;
  MCNMCACHINGCFG_MC_2LM_STRUCT      NmCachingCfg;

  NmCachingIntLv.Data = ReadCpuCsr (Socket, GetSktCh(Mc, Channel), MCNMCACHINGINTLV_MC_2LM_REG);
  NmCachingOffsetData.Data = ReadCpuCsr (Socket, GetSktCh(Mc, Channel), MCNMCACHINGOFFSET_MC_2LM_REG);
  NmCachingCfg.Data = ReadCpuCsr (Socket, GetSktCh(Mc, Channel), MCNMCACHINGCFG_MC_2LM_REG);
  DEBUG ((EFI_D_ERROR, "[GetNmCaching] NM caching Cfg:%x  OffData:%x  IntLv:%x \n", NmCachingCfg.Data, NmCachingOffsetData.Data, NmCachingIntLv.Data));

  //if (NmCachingOffsetData.Bits.mcnmcachingoffseten == 0) {
  //  return FALSE;
  //}

  if (NmTargetWay != NULL) {
    switch (NmCachingIntLv.Bits.nm_target_ways) {
      case 0:
        *NmTargetWay = 1;
        break;
      case 1:
        *NmTargetWay = 2;
        break;
      case 2:
        *NmTargetWay = 4;
        break;
      case 3:
        *NmTargetWay = 8;
        break;

      case 4:
        *NmTargetWay = 16;
        break;
      default:
        *NmTargetWay = 1;
        DEBUG ((EFI_D_ERROR, "[GetNmCaching] Wrong NM target ways:%d \n", NmCachingIntLv.Bits.nm_target_ways));
        return FALSE;
    }
  }

  if (NmChnWay != NULL) {
    switch (NmCachingIntLv.Bits.nm_chn_ways) {
      case 0:
        *NmChnWay = 1;  //3 = fast div-3
        break;
      case 1:
        *NmChnWay = 2;
        break;
      case 3:
        *NmChnWay = 8;
        break;
      case 4:
        *NmChnWay = 4;
        break;
      default:
        *NmChnWay = 1;
        DEBUG ((EFI_D_ERROR, "[GetNmCaching] Wrong NM chn ways:%d \n", NmCachingIntLv.Bits.nm_chn_ways));
        return FALSE;
    }
  }


  if (NmTargetGran != NULL) {
    switch (NmCachingIntLv.Bits.nm_target_gran) {
      case 0:
        *NmTargetGran = 6;  //64byte
        break;
      case 1:
        *NmTargetGran = 8;  //256
        break;
      case 2:
        *NmTargetGran = 12;  //4k
        break;
      case 4:
        *NmTargetGran = 7;  //128 byte
        break;
      case 5:
        *NmTargetGran = 9;  //512
        break;
      case 6:
        *NmTargetGran = 10;  //1KB
        break;
      case 7:
        *NmTargetGran = 11;  //2KB
        break;
      default:
        *NmTargetGran = 12;
        DEBUG ((EFI_D_ERROR, "[GetNmCaching] Wrong NM target gran:%d \n", NmCachingIntLv.Bits.nm_target_gran));
        return FALSE;
    }
  }

  if (NmChnGran != NULL) {
    switch (NmCachingIntLv.Bits.nm_chn_gran) {
      case 0:
        *NmChnGran = 6;  //64byte
        break;
      case 1:
        *NmChnGran = 8;  //256
        break;
      case 2:
        *NmChnGran = 12;  //4k
        break;
      case 4:
        *NmChnGran = 7;  //128 byte
        break;
      case 5:
        *NmChnGran = 9;  //512
        break;
      case 6:
        *NmChnGran = 10;  //1KB
        break;
      case 7:
        *NmChnGran = 11;  //2KB
        break;
      default:
        *NmChnGran = 1;
        DEBUG ((EFI_D_ERROR, "[GetNmCaching] Wrong NM chn gran:%d \n", NmCachingIntLv.Bits.nm_chn_gran));
        return FALSE;
    }
  }

  if (NmTargetLid != NULL) {
    *NmTargetLid = (UINT8)NmCachingIntLv.Bits.nm_target_lid;
  }

  if (NmChnLid != NULL) {
    *NmChnLid = (UINT8)NmCachingIntLv.Bits.nm_chn_lid;
  }

  if (NmCachingOffset != NULL) {
    if ( NmCachingOffsetData.Bits.mcnmcachingoffseten) {
      *NmCachingOffset = LShiftU64 (NmCachingOffsetData.Bits.mcnmcachingoffset, 35) | LShiftU64 (NmCachingOffsetData.Bits.mcnmcachingoffsetlower, 30);
    } else {
      *NmCachingOffset = 0;
    }
  }

  if (NmChnCap != NULL) {
    switch (NmCachingCfg.Bits.chn_cap) {
      case 0:
        *NmChnCap = 32;  //4GB
        break;
      case 1:
        *NmChnCap = 33;  //8GB
        break;
      case 2:
        *NmChnCap = 34;  //16GB
        break;
      case 3:
        *NmChnCap = 35;  //32 GB
        break;
      case 4:
        *NmChnCap = 36;  //64GB
        break;
      case 5:
        *NmChnCap = 37;  //128GB
        break;
      case 6:
        *NmChnCap = 38;  //256GB
        break;

      case 8:
        *NmChnCap = 28;  //256MB
        break;
      case 9:
        *NmChnCap = 29;  //512MB
        break;
      case 10:
        *NmChnCap = 30;  //1GB
        break;
      default:
        *NmChnCap = 32;
        DEBUG ((EFI_D_ERROR, "[GetNmCaching] Wrong NM cap:%d \n", NmCachingCfg.Bits.chn_cap));
        return FALSE;
    }
  }

  //
  //NmRatioChnCap <=32,   Nm size = chn_cap/NmRatioChnCap to div;
  //         else         Nm size = chn_cap * (NmRatioChnCap - 32) / 32;
  //
  if (NmRatioChnCap != NULL) {
    if (NmCachingCfg.Bits.nm_ratio_chn_cap == 0 ) {
      *NmRatioChnCap = 1;
    } else if (NmCachingCfg.Bits.nm_ratio_chn_cap == 1 ) {
      *NmRatioChnCap = 32;
    } else if (NmCachingCfg.Bits.nm_ratio_chn_cap == 2 ) {
      *NmRatioChnCap = 16;
    } else if (NmCachingCfg.Bits.nm_ratio_chn_cap == 3 ) {
      *NmRatioChnCap = 8;
    } else if (NmCachingCfg.Bits.nm_ratio_chn_cap == 4 ) {
      *NmRatioChnCap = 4;
    } else if (NmCachingCfg.Bits.nm_ratio_chn_cap == 5 ) {
      *NmRatioChnCap = 2;
    } else if ((NmCachingCfg.Bits.nm_ratio_chn_cap >= 33 ) && ( NmCachingCfg.Bits.nm_ratio_chn_cap <= 63)) {
      *NmRatioChnCap = (UINT8)NmCachingCfg.Bits.nm_ratio_chn_cap;
    } else {
      *NmRatioChnCap = 1;
      DEBUG ((EFI_D_ERROR, "[GetNmCaching] Wrong NM cap ratio:%d \n", NmCachingCfg.Bits.nm_ratio_chn_cap));
      return FALSE;

    }

  }

  return TRUE;
}

/**
  This routine is to calculate 1LM memory size from M2M TAD.
  add more strict condition check.

   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index

   @ret  TRUE -- the channel has NM M2M TAD.
**/
UINT64
EFIAPI
GetChannel1LmSize (
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Channel
  )
{
  UINT8     Sad2TadIndex;
  UINT8     TadIndex;
  UINT64    Size = 0;

  for (Sad2TadIndex = 0; Sad2TadIndex < MAX_SAD2TAD; Sad2TadIndex ++) {
    if (mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].Sad2TadValid &&
       (mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].NmCacheableVld == 0)) {
        TadIndex = mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].ChTad;
        //
        //here assume memory map channel tad is indexed per mc.
        //
        if (mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].TadInfo[TadIndex].TadValid ) {

          Size += (mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].Limit - mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].Base) + 1;
        }
        //DEBUG ((EFI_D_ERROR, "[GetChannel1LmSize]Sad2TadIndex:%d  TadIndex:%d  size:%lx\n", Sad2TadIndex, TadIndex, Size));
    }
  }
  return Size;
}

/**
  This routine is to get NM chache reg used for NM.

   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index
   @param[in]    McDramRuleIndex -- mc dram rule index
   @param[in,out] FmLimit  -- far mc controller
   @param[in,out] FmTarget  -- far mc controller
   @param[in,out] FmTargetWay  -- far mc controller ways
   @param[in,out] FmChnWay  -- far mc chn way
   @param[in,out] FmChnGran  -- far mc chn gran
   @param[in,out] FmChnL2Pid  -- far mc logic to phyical id map.

   @ret  TRUE -- this mc DRAM rule valid
**/
BOOLEAN
EFIAPI
GetMcDramRule (
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Channel,
  IN      UINT8     McDramRuleIndex,
  IN      UINT64   *FmLimit,
  IN      UINT8    *FmTarget,
  IN      UINT8    *FmTargetWay,
  IN      UINT8    *FmChnWay,
  IN      UINT8    *FmChnGran,
  IN      UINT32   *FmChnL2Pid
  )
{
  DRAM_RULE_CFG0_N0_MC_MAIN_STRUCT   McDramRuleN0;
  DRAM_RULE_CFG0_N1_MC_MAIN_STRUCT   McDramRuleN1;

  ASSERT (McDramRuleIndex < MAX_MC_DRAM_RULE);
  if (McDramRuleIndex >= MAX_MC_DRAM_RULE){
    return FALSE;
  }

  McDramRuleN0.Data = ReadCpuCsr (Socket, GetSktCh(Mc, Channel), mMcDramRuleRegs[McDramRuleIndex * 2]);
  McDramRuleN1.Data = ReadCpuCsr (Socket, GetSktCh(Mc, Channel), mMcDramRuleRegs[McDramRuleIndex * 2 + 1]);

  if (McDramRuleN0.Bits.rule_enable == 0) {
    return FALSE;
  }

  if (FmLimit != NULL) {
    *FmLimit = LShiftU64 (McDramRuleN0.Bits.limit +1, SAD_UNIT) -1;
  }

  if (FmTarget != NULL) {
    *FmTarget = (UINT8)McDramRuleN0.Bits.fm_target;
  }

  if (FmTargetWay != NULL) {
    switch (McDramRuleN1.Bits.fm_target_ways) {
      case 0:
        *FmTargetWay = 1;  //1-way
        break;
      case 1:
        *FmTargetWay = 2;  //2-way
        break;
      case 2:
        *FmTargetWay = 4;  //4-way
        break;
      case 3:
        *FmTargetWay = 8;  //8-way
        break;
      case 4:
        *FmTargetWay = 16;  //16-way
        break;

      default:
        *FmTargetWay = 1;
        DEBUG ((EFI_D_ERROR, "[GetMcDramRule] Wrong FM target way:%d \n", McDramRuleN1.Bits.fm_target_ways));
    }
  }

  if (FmChnWay != NULL) {
    switch (McDramRuleN0.Bits.fm_chn_ways) {
      case 0:
        *FmChnWay = 1;  //1-way
        break;
      case 1:
        *FmChnWay = 2;  //2-way
        break;
      case 2:
        *FmChnWay = 3;  //3-way
        break;
      case 3:
        *FmChnWay = 8;  //8-way
        break;
      default:
        *FmChnWay = 1;
        DEBUG ((EFI_D_ERROR, "[GetMcDramRule] Wrong FM chn way:%d \n", McDramRuleN0.Bits.fm_chn_ways));
    }
  }

  if (FmChnGran != NULL) {
    switch (McDramRuleN1.Bits.fm_chn_gran) {
      case 1:
        *FmChnGran = 8;  //256B
        break;
      case 2:
        *FmChnGran = 12;  //4KB
        break;
      case 3:
        *FmChnGran = 11;  //2KB
        break;
      default:
        *FmChnGran = 8;
        DEBUG ((EFI_D_ERROR, "[GetMcDramRule] Wrong FM chn grans:%d \n", McDramRuleN1.Bits.fm_chn_gran));
    }
  }

  if (FmChnL2Pid != NULL) {
    *FmChnL2Pid = McDramRuleN1.Bits.fm_chn_l2pid;
  }

  return TRUE;
}

/**
  This routine is to check the channel has NM capability from M2M tad.
  add more strict condition check.

   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index

   @ret  TRUE -- the channel has NM M2M TAD.
**/
BOOLEAN
EFIAPI
CheckChnNmCache (
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Channel
  )
{
  UINT8     Sad2TadIndex;
  UINT8     TadIndex;

  for (Sad2TadIndex = 0; Sad2TadIndex < MAX_SAD2TAD; Sad2TadIndex ++) {
    if (mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].Sad2TadValid &&
       mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].NmCacheableVld) {
        TadIndex = mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].ChTad;
        //
        //This assumes memory map will not set TAD on the channel which is NM.
        //
        if (mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].TadInfo[TadIndex].TadValid == 0) {
          return TRUE;
      }
    }
  }
  return FALSE;
}


/**
This is to calculate NM size.

  @param [in]   NmChnCapBits  --    highest bit of channel size.
  @param [in]   NmRatioChnCap --    ratio of channel size

  return NM size.
**/
UINT64
EFIAPI
CalCulateNmSize (
  IN      UINT8      NmChnCapBits,
  IN      UINT8      NmRatioChnCap
  )
{
  UINT64     NmSize;

  NmSize = LShiftU64 (1, NmChnCapBits);

  if (NmRatioChnCap == 0) {
    return 0;
  }

  //
  //NmRatioChnCap <=32,   Nm size = chn_cap/NmRatioChnCap to div;
  //         else         Nm size = chn_cap * (NmRatioChnCap - 32) / 32;
  //
  if (NmRatioChnCap <= 32) {
    NmSize = DivU64x32 (NmSize, NmRatioChnCap);
  } else {
    NmSize = DivU64x32 (MultU64x32 (NmSize, NmRatioChnCap - 32), 32);
  }
  return NmSize;
}

/**
  This routine is to get 2LM tad info.

   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index

   @ret  none.
**/
VOID
EFIAPI
Init2LmTad (
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Channel
  )
{
  UINT8     NmTargetWay;
  UINT8     NmChnWay;
  UINT8     NmTargetGran;
  UINT8     NmChnGran;
  UINT8     NmTargetLid;
  UINT8     NmChnLid;
  UINT64    NmCachingOffset;
  UINT8     NmChnCap;
  UINT8     NmRatioChnCap;

  //
  //In case this channel has NM TAD, it must have channel SAD for far memory decode.
  //
  if ((mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].ChSad[0].McDramRuleValid == TRUE) &&
      GetNmCaching (Socket, Mc, Channel, &NmTargetWay, &NmChnWay, &NmTargetGran,
                    &NmChnGran, &NmTargetLid, &NmChnLid, &NmCachingOffset, &NmChnCap, &NmRatioChnCap)) {
    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmTadValid  = TRUE;
    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmTargetWay = NmTargetWay;
    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmChnWay = NmChnWay;
    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmTargetGran = NmTargetGran;
    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmChnGran = NmChnGran;

    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmTargetLid = NmTargetLid;
    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmChnLid = NmChnLid;
    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmCachingOffset = NmCachingOffset;
    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmChnCap = NmChnCap;
    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmRatioChnCap = NmRatioChnCap;
    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmIndexStartAddr = GetChannel1LmSize (Socket, Mc, Channel); //This is for 1LM + NM.
    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmIndexEndAddr = CalCulateNmSize (NmChnCap, NmRatioChnCap); //This is for NM + 1LM.

    DEBUG ((EFI_D_INFO, "  [Init2LmTad] NmTargetWay:%d Chnway:%d TargetGran:%d ChnGran:%d ChnCap:%d ChnCapRatio:%d offset:%lx ",
           NmTargetWay, NmChnWay, NmTargetGran, NmChnGran, NmChnCap, NmRatioChnCap, NmCachingOffset));

    DEBUG ((EFI_D_INFO, "IndexStartAddr(1LM+NM):%lx  CacheSize(NM+1LM):%lx  \n",
        mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmIndexStartAddr, mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmIndexEndAddr));
  } else {
    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmTadValid  = FALSE;
  }
}

/**
  This routine is to get MC dram rule info.

   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index

   @ret  none.
**/
VOID
EFIAPI
InitMcDramRule (
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Channel
  )
{
  UINT64    FmLimit;
  UINT8     FmTarget;
  UINT8     FmTargetWay;
  UINT8     FmChnWay;
  UINT8     FmChnGran;
  UINT32    FmChnL2Pid;
  UINT8     McDramRuleIndex;
  UINT64    PreviousFmBase = 0;

  for (McDramRuleIndex = 0; McDramRuleIndex < MAX_MC_DRAM_RULE; McDramRuleIndex ++) {
    if (GetMcDramRule (Socket, Mc, Channel, McDramRuleIndex, &FmLimit, &FmTarget,&FmTargetWay,&FmChnWay,&FmChnGran, &FmChnL2Pid)) {

    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].ChSad[McDramRuleIndex].McDramRuleValid  = TRUE;
    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].ChSad[McDramRuleIndex].FmLimit = FmLimit;
    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].ChSad[McDramRuleIndex].FmBase = PreviousFmBase;
    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].ChSad[McDramRuleIndex].FmTarget = FmTarget;
    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].ChSad[McDramRuleIndex].FmTargetWay = FmTargetWay;
    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].ChSad[McDramRuleIndex].FmChnWay = FmChnWay;
    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].ChSad[McDramRuleIndex].FmChnGran = FmChnGran;
    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].ChSad[McDramRuleIndex].FmChnL2Pid = FmChnL2Pid;

    PreviousFmBase = FmLimit + 1;
    DEBUG ((EFI_D_INFO, "  [InitMcDramRule] MC dram rule Idx:%d Limit:%lx Target:%d TgtWy:%d ChnWay:%d ChnGran:%d Chn2Pid:%x\n",
                    McDramRuleIndex, FmLimit, FmTarget, FmTargetWay, FmChnWay, FmChnGran, FmChnL2Pid));
        }
   }
}

/**
  This routine is to get MC MTC register.

   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index
   @param[in]    ClosePg   --   close pg
   @param[in]    BankXorEnable  -- Bank xor enable
   @param[in]    RankXorEnabe   -- Rank xor enable
   @param[in]    SidXorEnable   -- sid xor enable
   @param[in]    PchXorEnable   -- PCH xor enable
   @param[in, out]  DimmXorEnable  -- DIMM xor enable

   @ret  none.
**/
VOID
EFIAPI
GetMtr (
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Channel,
  IN OUT  UINT8    *ClosePg,
  IN OUT  UINT8    *BankXorEnable,
  IN OUT  UINT8    *RankXorEnabe,
  IN OUT  UINT8    *SidXorEnable,
  IN OUT  UINT8    *Cgbg,
  IN OUT  UINT8    *PchXorEnable,
  IN OUT  UINT8    *DimmXorEnable
  )
{
  MCMTR_MC_MAIN_STRUCT                    McmtrMain;
  AMAP_MC_MAIN_STRUCT                     AmapReg;

  McmtrMain.Data = ReadCpuCsr (Socket, GetSktCh(Mc, Channel), MCMTR_MC_MAIN_REG);

  if (ClosePg != NULL) {
    *ClosePg = (UINT8)McmtrMain.Bits.close_pg;
  }

  if (BankXorEnable != NULL) {
    *BankXorEnable = (UINT8)McmtrMain.Bits.bank_xor_enable;
  }

  if (RankXorEnabe != NULL) {
    *RankXorEnabe = (UINT8)McmtrMain.Bits.rank_xor_enable;
  }

  if (DimmXorEnable != NULL) {
    *DimmXorEnable = (UINT8)McmtrMain.Bits.dimm_xor_enable;
  }

  if (SidXorEnable != NULL) {
    *SidXorEnable = (UINT8)McmtrMain.Bits.sid_xor_enable;
  }

  if (PchXorEnable != NULL) {
    *PchXorEnable = (UINT8)McmtrMain.Bits.pch_xor_enable;
  }

  AmapReg.Data = ReadCpuCsr (Socket, GetSktCh(Mc, Channel), AMAP_MC_MAIN_REG);
  if (Cgbg != NULL) {
    *Cgbg = (UINT8)AmapReg.Bits.cgbg_interleave;
  }

}


/**
  This routine is to get RIR info.

   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index
   @param[in]    RirIndex  --   Rir index
   @param[in]    RirWay    --   Rir way
   @param[in]    RirLimit  --   Rir Limit

   @ret  TRUE  -- get RIR info successfully.
**/
BOOLEAN
EFIAPI
GetRir (
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Channel,
  IN      UINT8     RirIndex,
  IN OUT  UINT8    *RirWay,
  IN OUT  UINT64   *RirLimit
  )
{
  RIRWAYNESSLIMIT_0_MC_MAIN_STRUCT        RirWaydata;


  RirWaydata.Data = ReadCpuCsr (Socket, GetSktCh(Mc, Channel), mRirWaynessLimit[RirIndex]);

  if (RirWaydata.Bits.rir_val == TRUE) {

    if (RirWay != NULL) {
      if (RirWaydata.Bits.rir_way == 0) {
        *RirWay = 1; //1-way
      }
      if (RirWaydata.Bits.rir_way == 1) {
        *RirWay = 2; //2-way
      }
      if (RirWaydata.Bits.rir_way == 2) {
        *RirWay = 4; //4-way
      }
      if (RirWaydata.Bits.rir_way == 3) {
        *RirWay = 8; //8-way
      }
    }

    if (RirLimit != NULL) {
      *RirLimit = LShiftU64 (RirWaydata.Bits.rir_limit + 1, RIR_UINT) -1;
    }

    return TRUE;
  } else {
    return FALSE;
  }
}


/**
  This routine is to get RIR interleave info.

   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index
   @param[in]    RirIndex  --   Rir index
   @param[in]    RirIlvIndex  -- Rir interleave index
   @param[in]    RankId    --   Rank
   @param[in]    RirOffset  --   Rir offset

   @ret  none
**/
VOID
EFIAPI
GetRirIlv (
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Channel,
  IN      UINT8     RirIndex,
  IN      UINT8     RirIlvIndex,
  IN OUT  UINT8    *RankId,
  IN OUT  UINT64   *RirOffset
  )
{
  RIRILV0OFFSET_0_MC_MAIN_STRUCT          RirIlv0Offset;
  UINT8                                   PysicalRank;

  RirIlv0Offset.Data = ReadCpuCsr (Socket, GetSktCh(Mc, Channel), mRirInterleaveOffset[RirIndex * MAX_ILV+ RirIlvIndex]);

  if (RankId != NULL) {
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
    PysicalRank = (UINT8)RirIlv0Offset.Bits.rir_rnk_tgt0;

    *RankId = PysicalRank;
  }

  if (RirOffset != NULL) {
    *RirOffset = LShiftU64(RirIlv0Offset.Bits.rir_offset0, SAD_UNIT);
  }
}

/**
  This routine is to initialize RIR relate info.

   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index

   @ret  none.
**/
VOID
EFIAPI
InitRir (
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Channel
  )
{
  UINT8                                   RirIndex;
  UINT8                                   RirWay;
  UINT64                                  RirLimit;
  UINT64                                  PreviousLimit;
  UINT8                                   RirIlvIndex;

  UINT8                                   ClosePg;
  UINT8                                   BankXorEnable;
  UINT8                                   RankXorEnabe;
  UINT8                                   DimmXorEnable;
  UINT8                                   SidXorEnable;
  UINT8                                   Cgbg;
  UINT8                                   PchXorEnable;

  UINT8                                   RankId;
  UINT64                                  RirOffset;



  GetMtr (Socket, Mc, Channel, &ClosePg, &BankXorEnable, &RankXorEnabe, &SidXorEnable, &Cgbg, &PchXorEnable, &DimmXorEnable);
  mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].ClosePg = ClosePg;
  mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].BankXorEnable = BankXorEnable;
  mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RankXorEnable = RankXorEnabe;
  mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DimmXorEnable = DimmXorEnable;
  mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].SidXorEnable = SidXorEnable;
  mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].Cgbg = Cgbg;
  mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].PchXorEnable = PchXorEnable;

  DEBUG ((EFI_D_INFO, " [Rir] socket:%d  Mc:%d  Channel:%d ClosePg:%d  BankXor:%d  RankXor:%d  DimmXor:%d  PchXor:%d\n",
       Socket, Mc, Channel, ClosePg, BankXorEnable, RankXorEnabe, DimmXorEnable, PchXorEnable));

  for (RirIndex = 0; RirIndex < MAX_RIR; RirIndex ++) {
   if (RirIndex == 0) {
     PreviousLimit = 0;
   }

   if (GetRir (Socket, Mc, Channel,RirIndex, &RirWay, &RirLimit) == TRUE) {
      mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RirInfo[RirIndex].RirValid = TRUE;
      mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RirInfo[RirIndex].RirBase =  PreviousLimit;
      mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RirInfo[RirIndex].RirWay = RirWay;
      mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RirInfo[RirIndex].RirLimit = RirLimit;
      PreviousLimit = RirLimit + 1;

      DEBUG ((EFI_D_INFO, " [Rir] Index:%d Base:%lx  RirLimit:%lx Rirway:%x RIROffset-Rank per rank interleave.\n  ", RirIndex, mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RirInfo[RirIndex].RirBase,
         RirLimit, RirWay));

      for (RirIlvIndex = 0; RirIlvIndex < RirWay; RirIlvIndex ++) {
        GetRirIlv (Socket, Mc, Channel, RirIndex, RirIlvIndex, &RankId, &RirOffset);
        mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RirInfo[RirIndex].RirIlv[RirIlvIndex].RirIlvValid = TRUE;
        mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RirInfo[RirIndex].RirIlv[RirIlvIndex].RirOffset = RirOffset;
        mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RirInfo[RirIndex].RirIlv[RirIlvIndex].RankId = RankId;

        DEBUG ((EFI_D_INFO, " %lx - %d", RirOffset, RankId));
      }
      DEBUG ((EFI_D_INFO, "\n"));
   } else {
     break;
   }
  }
}


/**
  This routine is to get DDRT RIR info.

   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index
   @param[in]    RirIndex  --   Rir index
   @param[in]    RirWay    --   Rir way
   @param[in]    RirLimit  --   Rir Limit

   @ret  TRUE  -- get RIR info successfully.
**/
BOOLEAN
EFIAPI
GetDdrtRir (
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Channel,
  IN      UINT8     RirIndex,
  IN OUT  UINT8    *RirWay,
  IN OUT  UINT64   *RirLimit
  )
{
  FMRIRWAYNESSLIMIT_0_MC_2LM_STRUCT       RirWaydata;

  RirWaydata.Data = ReadCpuCsr (Socket, GetSktCh(Mc, Channel), mDdrtRirWaynessLimit[RirIndex]);

  if (RirWaydata.Bits.rir_val == TRUE) {

    if (RirWay != NULL) {
      if (RirWaydata.Bits.rir_way == 0) {
        *RirWay = 1; //1-way
      }
      if (RirWaydata.Bits.rir_way == 1) {
        *RirWay = 2; //2-way
      }
      if (RirWaydata.Bits.rir_way == 2) {
        *RirWay = 4; //4-way
      }
      if (RirWaydata.Bits.rir_way == 3) {
        *RirWay = 8; //8-way
      }
    }

    if (RirLimit != NULL) {
      *RirLimit = LShiftU64 (RirWaydata.Bits.rir_limit + 1, RIR_UINT) -1;
    }

    return TRUE;
  } else {
    return FALSE;
  }
}


/**
  This routine is to get DDRT RIR interleave info.

   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index
   @param[in]    RirIndex  --   Rir index
   @param[in]    RirIlvIndex  -- Rir interleave index
   @param[in]    RankId    --   Rank
   @param[in]    RirOffset  --   Rir offset

   @ret  none
**/
VOID
EFIAPI
GetDdrtRirIlv (
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Channel,
  IN      UINT8     RirIndex,
  IN      UINT8     RirIlvIndex,
  IN OUT  UINT8    *RankId,
  IN OUT  UINT64   *RirOffset
  )
{
  FMRIRILV0OFFSET_0_MC_2LM_STRUCT           RirIlv0Offset;

  RirIlv0Offset.Data = ReadCpuCsr (Socket, GetSktCh(Mc, Channel), mDdrtRirInterleaveOffset[RirIndex * MAX_DDRT_ILV+ RirIlvIndex]);

  if (RankId != NULL) {
    *RankId = (UINT8)RirIlv0Offset.Bits.rir_rnk_tgt0;
  }

  if (RirOffset != NULL) {
    *RirOffset = LShiftU64(RirIlv0Offset.Bits.rir_offset0, SAD_UNIT);
  }
}



/**
  This routine is to initialize RIR relate info.

   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index

   @ret  none.
**/
VOID
EFIAPI
InitDdrtRir (
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Channel
  )
{
  UINT8                                   RirIndex;
  UINT8                                   RirWay;
  UINT64                                  RirLimit;
  UINT64                                  PreviousLimit;
  UINT8                                   RirIlvIndex;

  UINT8                                   RankId;
  UINT64                                  RirOffset;

  for (RirIndex = 0; RirIndex < MAX_DDRT_RIR; RirIndex ++) {
   if (RirIndex == 0) {
     PreviousLimit = 0;
   }

   if (GetDdrtRir (Socket, Mc, Channel,RirIndex, &RirWay, &RirLimit) == TRUE) {
      mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DdrtRirInfo[RirIndex].RirValid = TRUE;
      mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DdrtRirInfo[RirIndex].RirBase =  PreviousLimit;
      mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DdrtRirInfo[RirIndex].RirWay = RirWay;
      mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DdrtRirInfo[RirIndex].RirLimit = RirLimit;
      PreviousLimit = RirLimit + 1;

      DEBUG ((EFI_D_INFO, " [DDRT Rir] Index:%d Base:%lx  RirLimit:%lx Rirway:%x     RIROffset -  Rank per rank interleave.\n  ", RirIndex,
              mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DdrtRirInfo[RirIndex].RirBase, RirLimit, RirWay));

      for (RirIlvIndex = 0; RirIlvIndex < RirWay; RirIlvIndex ++) {
        GetDdrtRirIlv (Socket, Mc, Channel, RirIndex, RirIlvIndex, &RankId, &RirOffset);
        mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DdrtRirInfo[RirIndex].RirIlv[RirIlvIndex].RirIlvValid = TRUE;
        mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DdrtRirInfo[RirIndex].RirIlv[RirIlvIndex].RirOffset = RirOffset;
        mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DdrtRirInfo[RirIndex].RirIlv[RirIlvIndex].RankId = RankId;

        DEBUG ((EFI_D_INFO, " %lx  -  %d", RirOffset, RankId));
      }
      DEBUG ((EFI_D_INFO, "\n"));
   } else {
     break;
   }
  }
}


/**
  This routine is to initialize RIR relate info.

   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index
   @param[in]    DimmIndex --   Dimm index

   @ret  none.
**/
VOID
EFIAPI
InitDimmMtr (
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Channel,
  IN      UINT8     DimmIndex
  )
{

  DIMMMTR_0_MC_MAIN_STRUCT     DimmMtr;
  MCDDRTCFG_MC_MAIN_STRUCT     McDdrtCfg;

  DimmMtr.Data = ReadCpuCsr (Socket, GetSktCh(Mc, Channel), mDimmMtrOffset[DimmIndex]);

  if (DimmMtr.Bits.dimm_pop) {
    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DimmInfo[DimmIndex].HbmDdr5Exist = 1;
    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DimmInfo[DimmIndex].Density = (UINT8)DimmMtr.Bits.ddr3_dnsty;
    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DimmInfo[DimmIndex].Width = (UINT8)DimmMtr.Bits.ddr3_width;
    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DimmInfo[DimmIndex].Sid  = (UINT8)DimmMtr.Bits.ddr4_3dsnumranks_cs;
    if (IsHbmDimm(Socket, Mc)) {
      mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DimmInfo[DimmIndex].ColWidth  = 6;
    } else {
      mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DimmInfo[DimmIndex].ColWidth  = 10 + (UINT8)DimmMtr.Bits.ca_width;
    }
    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DimmInfo[DimmIndex].RowWidth  = 12 + (UINT8)DimmMtr.Bits.ra_width;
    DEBUG ((EFI_D_INFO, " [InitDimmMtr] socket%d Mc:%d Chn:%d Dimm:%d  Density:%d  Width:%d  Sid:%d  ColWidth:%d  RowWidth:%d\n", Socket, Mc, Channel, DimmIndex,
       mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DimmInfo[DimmIndex].Density, mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DimmInfo[DimmIndex].Width,
       mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DimmInfo[DimmIndex].Sid, mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DimmInfo[DimmIndex].ColWidth,
       mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DimmInfo[DimmIndex].RowWidth));
  }

  McDdrtCfg.Data = ReadCpuCsr (Socket, GetSktCh(Mc, Channel), MCDDRTCFG_MC_MAIN_REG);
  //DEBUG ((EFI_D_INFO, " DDRT CFG:%X\n", McDdrtCfg.Data));
  if (DimmIndex == 0) {
    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DimmInfo[DimmIndex].DdrtExist = (UINT8)McDdrtCfg.Bits.slot0;
  } else {
    mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DimmInfo[DimmIndex].DdrtExist = (UINT8)McDdrtCfg.Bits.slot1;
  }

}

/**
 This routine is to read channel TAD info.

 @param[in]    Socket    --   socket index


 @retval       none.

**/
VOID
EFIAPI
InitChTad (
  IN     UINT8       Socket
  )
{
  UINT8     Mc;
  UINT8     TadIndex;
  UINT8     Channel;
  UINT8     TargetWay;
  UINT8     TargetGran;
  UINT8     ChWay;
  UINT8     ChGran;
  UINT64    TadOffset;
  UINT8     TadSign;

  UINT8     DimmIndex;

  for (Mc = 0; Mc < MAX_SYS_IMC; Mc ++) {
    if (IsMcExist (Socket, Mc) == FALSE) {
      continue;
    }

    if (Mc >= 4) {
      mSadTadScope[Socket].McInfo[Mc].HbmMc = TRUE;
    }

    for (Channel = 0; Channel < MAX_CH_PER_MC; Channel ++) {
      if (IsChannelExist (Socket, Mc, Channel) == FALSE) {
        continue;
      }
      DEBUG ((EFI_D_INFO, "  Skt:%d  Mc:%d  Chn:%d \n", Socket, Mc, Channel));


      for (TadIndex = 0; TadIndex < MAX_TAD; TadIndex++) {

        if (GetTad (Socket, Mc, Channel, TadIndex, &TargetWay, &TargetGran, &ChWay, &ChGran, &TadOffset, &TadSign) == FALSE) {
          continue;
        }
        mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].TadInfo[TadIndex].TadValid = TRUE;
        mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].TadInfo[TadIndex].TargetWay = TargetWay;
        mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].TadInfo[TadIndex].TargetGran =  TargetGran;
        mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].TadInfo[TadIndex].ChWay = ChWay;
        mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].TadInfo[TadIndex].ChGran = ChGran;
        mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].TadInfo[TadIndex].TadOffset =  TadOffset;
        mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].TadInfo[TadIndex].TadSign = TadSign;
        DEBUG ((EFI_D_INFO, " [Tad] TadIndex:%d TgtWay:%d  TgtGran:%d  ChnWay:%d  ChnGran:%d  TadOff:%lx  TadSign:%d\n",
                    TadIndex, TargetWay, TargetGran, ChWay, ChGran, TadOffset, TadSign));
      }

      //
      //  It must put InitMcDramRule before Init2LmTad, otherwise failure in initialization.
      //
      InitMcDramRule (Socket, Mc, Channel);
      Init2LmTad (Socket, Mc, Channel);
      InitRir (Socket, Mc, Channel);
      if (!IsHbmDimm(Socket, Mc)) {
        InitDdrtRir (Socket, Mc, Channel);
      }
      for (DimmIndex = 0; DimmIndex < MAX_DIMM_NUM; DimmIndex ++) {
        InitDimmMtr(Socket, Mc, Channel,DimmIndex);
      }
    }
  }
}
