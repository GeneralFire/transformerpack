/** @file
  Implementation of CHA DRAM decode.

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
#include <IioUniversalData.h>
#include <RcRegs.h>
#include <Library/CpuCsrAccessLib.h>
#include "AddressDecodeInternal.h"
#include <Library/ChaIpLib/Include/ChaRegs.h>

extern AD_SOCKET_INFO              *mSadTadScope;



UINT32 mRemoteDramRuleCfgCha[MAX_REMOTE_DRAM_RULE] = {
  REMOTE_DRAM_RULE_CFG_0_N0_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_1_N0_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_2_N0_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_3_N0_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_4_N0_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_5_N0_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_6_N0_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_7_N0_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_8_N0_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_9_N0_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_10_N0_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_11_N0_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_12_N0_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_13_N0_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_14_N0_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_15_N0_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_16_N0_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_17_N0_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_18_N0_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_19_N0_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_20_N0_CHABC_SAD_REG
};

UINT32 mRemoteDramRuleCfgChaN1[MAX_REMOTE_DRAM_RULE] = {
  REMOTE_DRAM_RULE_CFG_0_N1_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_1_N1_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_2_N1_CHABC_SAD_REG,   REMOTE_DRAM_RULE_CFG_3_N1_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_4_N1_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_5_N1_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_6_N1_CHABC_SAD_REG,   REMOTE_DRAM_RULE_CFG_7_N1_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_8_N1_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_9_N1_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_10_N1_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_11_N1_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_12_N1_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_13_N1_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_14_N1_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_15_N1_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_16_N1_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_17_N1_CHABC_SAD_REG, REMOTE_DRAM_RULE_CFG_18_N1_CHABC_SAD_REG,  REMOTE_DRAM_RULE_CFG_19_N1_CHABC_SAD_REG,
  REMOTE_DRAM_RULE_CFG_20_N1_CHABC_SAD_REG
};


UINT32 mDramRuleUcCha[MAX_LOCAL_DRAM_RULE] = {
  DRAM_RULE_CFG_UNICAST_GROUP0_0_CHA_MISC_REG, DRAM_RULE_CFG_UNICAST_GROUP0_1_CHA_MISC_REG, DRAM_RULE_CFG_UNICAST_GROUP0_2_CHA_MISC_REG,  DRAM_RULE_CFG_UNICAST_GROUP0_3_CHA_MISC_REG,
  DRAM_RULE_CFG_UNICAST_GROUP0_4_CHA_MISC_REG, DRAM_RULE_CFG_UNICAST_GROUP0_5_CHA_MISC_REG, DRAM_RULE_CFG_UNICAST_GROUP0_6_CHA_MISC_REG,  DRAM_RULE_CFG_UNICAST_GROUP0_7_CHA_MISC_REG,
  DRAM_RULE_CFG_UNICAST_GROUP0_8_CHA_MISC_REG, DRAM_RULE_CFG_UNICAST_GROUP0_9_CHA_MISC_REG, DRAM_RULE_CFG_UNICAST_GROUP0_10_CHA_MISC_REG, DRAM_RULE_CFG_UNICAST_GROUP0_11_CHA_MISC_REG,
  DRAM_RULE_CFG_UNICAST_GROUP0_12_CHA_MISC_REG,DRAM_RULE_CFG_UNICAST_GROUP0_13_CHA_MISC_REG,DRAM_RULE_CFG_UNICAST_GROUP0_14_CHA_MISC_REG, DRAM_RULE_CFG_UNICAST_GROUP0_15_CHA_MISC_REG
};


UINT32 mInterleaveListUcCha[MAX_LOCAL_DRAM_RULE] = {
  INTERLEAVE_LIST_CFG_UNICAST_GROUP0_0_CHA_MISC_REG,   INTERLEAVE_LIST_CFG_UNICAST_GROUP0_1_CHA_MISC_REG,   INTERLEAVE_LIST_CFG_UNICAST_GROUP0_2_CHA_MISC_REG,   INTERLEAVE_LIST_CFG_UNICAST_GROUP0_3_CHA_MISC_REG,
  INTERLEAVE_LIST_CFG_UNICAST_GROUP0_4_CHA_MISC_REG,   INTERLEAVE_LIST_CFG_UNICAST_GROUP0_5_CHA_MISC_REG,   INTERLEAVE_LIST_CFG_UNICAST_GROUP0_6_CHA_MISC_REG,   INTERLEAVE_LIST_CFG_UNICAST_GROUP0_7_CHA_MISC_REG,
  INTERLEAVE_LIST_CFG_UNICAST_GROUP0_8_CHA_MISC_REG,   INTERLEAVE_LIST_CFG_UNICAST_GROUP0_9_CHA_MISC_REG,   INTERLEAVE_LIST_CFG_UNICAST_GROUP0_10_CHA_MISC_REG,  INTERLEAVE_LIST_CFG_UNICAST_GROUP0_11_CHA_MISC_REG,
  INTERLEAVE_LIST_CFG_UNICAST_GROUP0_12_CHA_MISC_REG,  INTERLEAVE_LIST_CFG_UNICAST_GROUP0_13_CHA_MISC_REG,  INTERLEAVE_LIST_CFG_UNICAST_GROUP0_14_CHA_MISC_REG,  INTERLEAVE_LIST_CFG_UNICAST_GROUP0_15_CHA_MISC_REG
};

UINT32 mDramRuleCfg2Cha[MAX_LOCAL_DRAM_RULE] = {
  DRAM_RULE_CFG2_UNICAST_GROUP0_0_CHA_MISC_REG,   DRAM_RULE_CFG2_UNICAST_GROUP0_1_CHA_MISC_REG,   DRAM_RULE_CFG2_UNICAST_GROUP0_2_CHA_MISC_REG,    DRAM_RULE_CFG2_UNICAST_GROUP0_3_CHA_MISC_REG,
  DRAM_RULE_CFG2_UNICAST_GROUP0_4_CHA_MISC_REG,   DRAM_RULE_CFG2_UNICAST_GROUP0_5_CHA_MISC_REG,   DRAM_RULE_CFG2_UNICAST_GROUP0_6_CHA_MISC_REG,    DRAM_RULE_CFG2_UNICAST_GROUP0_7_CHA_MISC_REG,
  DRAM_RULE_CFG2_UNICAST_GROUP0_8_CHA_MISC_REG,   DRAM_RULE_CFG2_UNICAST_GROUP0_9_CHA_MISC_REG,   DRAM_RULE_CFG2_UNICAST_GROUP0_10_CHA_MISC_REG,   DRAM_RULE_CFG2_UNICAST_GROUP0_11_CHA_MISC_REG,
  DRAM_RULE_CFG2_UNICAST_GROUP0_12_CHA_MISC_REG,  DRAM_RULE_CFG2_UNICAST_GROUP0_13_CHA_MISC_REG,  DRAM_RULE_CFG2_UNICAST_GROUP0_14_CHA_MISC_REG,   DRAM_RULE_CFG2_UNICAST_GROUP0_15_CHA_MISC_REG
};


UINT32 mDramInterleaveWays[MAX_ROUTE_TABLE * 2] = {
  //
  //index: 3b{die, routeType}
  //
  DRAM_H0_RT0_MODE0_CFG_UNICAST_GROUP0_CHA_MISC_REG,DRAM_H0_RT0_MODE1_CFG_UNICAST_GROUP0_CHA_MISC_REG,
  DRAM_H0_RT1_MODE0_CFG_UNICAST_GROUP0_CHA_MISC_REG,DRAM_H0_RT1_MODE1_CFG_UNICAST_GROUP0_CHA_MISC_REG,
  DRAM_H0_RT2_MODE0_CFG_UNICAST_GROUP0_CHA_MISC_REG,DRAM_H0_RT2_MODE1_CFG_UNICAST_GROUP0_CHA_MISC_REG,
  DRAM_H0_RT2_MODE0_CFG_UNICAST_GROUP0_CHA_MISC_REG,DRAM_H0_RT2_MODE1_CFG_UNICAST_GROUP0_CHA_MISC_REG,
  DRAM_H1_RT0_MODE0_CFG_UNICAST_GROUP0_CHA_MISC_REG,DRAM_H1_RT0_MODE1_CFG_UNICAST_GROUP0_CHA_MISC_REG,
  DRAM_H1_RT1_MODE0_CFG_UNICAST_GROUP0_CHA_MISC_REG,DRAM_H1_RT1_MODE1_CFG_UNICAST_GROUP0_CHA_MISC_REG,
  DRAM_H1_RT2_MODE0_CFG_UNICAST_GROUP0_CHA_MISC_REG,DRAM_H1_RT2_MODE1_CFG_UNICAST_GROUP0_CHA_MISC_REG,
  DRAM_H1_RT2_MODE0_CFG_UNICAST_GROUP0_CHA_MISC_REG,DRAM_H1_RT2_MODE1_CFG_UNICAST_GROUP0_CHA_MISC_REG
};

UINT32 mDramRouteTable[MAX_ROUTE_TABLE * 2] = {
  //
  //index: 3b{die, routeType}
  //
  H0_TGT_ROUTE_TABLE_0_CHA_MISC_REG,   H0_CH_ROUTE_TABLE_0_CHA_MISC_REG,
  H0_TGT_ROUTE_TABLE_1_CHA_MISC_REG,   H0_CH_ROUTE_TABLE_1_CHA_MISC_REG,
  H0_TGT_ROUTE_TABLE_2_CHA_MISC_REG,   H0_CH_ROUTE_TABLE_2_CHA_MISC_REG,
  H0_TGT_ROUTE_TABLE_2LM_CHA_MISC_REG, H0_CH_ROUTE_TABLE_2_CHA_MISC_REG,
  H1_TGT_ROUTE_TABLE_0_CHA_MISC_REG,   H1_CH_ROUTE_TABLE_0_CHA_MISC_REG,
  H1_TGT_ROUTE_TABLE_1_CHA_MISC_REG,   H1_CH_ROUTE_TABLE_1_CHA_MISC_REG,
  H1_TGT_ROUTE_TABLE_2_CHA_MISC_REG,   H1_CH_ROUTE_TABLE_2_CHA_MISC_REG,
  H1_TGT_ROUTE_TABLE_2LM_CHA_MISC_REG, H1_CH_ROUTE_TABLE_2_CHA_MISC_REG

};


/**
  get XOR defeature mode.
  SPR XOR defeature mode disabled. Or could get it from KTILK_UMA_CLUSTER_CONFIG_KTI_LLPMON_STRUCT.defeature_xor

  @retval 0 -- XOR defeature mode disabled, cluster XOR enabled.
          1 -- XOR defeature mode enabled,  cluster XOR disabled.

**/
BOOLEAN
EFIAPI
GetXorDefeatureMode (
  VOID
  )
{
  return 0;
}

/**
  check whether is UMA based cluster mode

  @param[in] Socket   --   socket index

  @retval TRUE -- UBC mode.

**/
BOOLEAN
EFIAPI
IsUbcModeEnabled (
  IN  UINT8    Socket
  )
{
  return (mSadTadScope[Socket].NumUmaCluster > 1);
}

/**
  check whether is SNC based cluster mode

  @param[in] Socket   --   socket index

  @retval TRUE -- SNC mode.

**/
BOOLEAN
EFIAPI
IsSncModeEnabled (
  IN  UINT8    Socket
  )
{
  if ((mSadTadScope[Socket].NumUmaCluster == 0) && (mSadTadScope[Socket].NumCluster > 1)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  This routine is to get cluster number per socket. system could be one of following state (UBC, SNC, or no clustering).
  UBC and SNC are mutual exlusive.

  @param[in] Skt   --   socket index

  @retval cluster number per socket. Value 1 means no UBC or SNC.

**/
UINT32
EFIAPI
GetClusterNum (
  IN   UINT8   Skt
  )
{
  UINT32   Cluster = 1;

  if (mSadTadScope[Skt].NumCluster != 0) {
    Cluster = mSadTadScope[Skt].NumCluster;
  }

  return Cluster;
}

/**
  This routine is to get CHA index per socket and SNC index.

  Caller code should make sure that it is under SNC or UBC mode.

  Find out which CHA belong to the specific cluster.

  @param[in] Skt   --   socket index
  @param[in] SncIndex  -- cluster index

  @retval CHA index per the cluster

**/
UINT32
EFIAPI
GetChaIndexPerCluster (
  IN   UINT8   Skt,
  IN   UINT8   ClusterIndex
  )
{
  UINT32    TotalCha;
  UINT32    NumOfCluster;
  UINT32    ChaPerCluster;

  TotalCha = mSadTadScope[Skt].TotalCha;
  NumOfCluster =  GetClusterNum (Skt);

  ChaPerCluster = TotalCha/NumOfCluster;

  if (TotalCha%NumOfCluster != 0) {
    ChaPerCluster ++;
  }

  return ClusterIndex * ChaPerCluster;
}


/**
  This routine is to get DRAM rule info

  @param[in] Skt   --   socket index
  @param[in] Cluster  -- Cluster index
  @param[in] DramRuleIndex  -- Dram rule index
  @param[in, out]  AddressLimit -- system address limit
  @param[in, out]  NmCacheable  -- NM flag for this range.
  @param[in, out]  InterleaveMode -- interleave mode

  @retval CHA index per the cluster

**/

BOOLEAN
EFIAPI
GetDramRule (
  IN   UINT8       Socket,
  IN   UINT8       Cluster,
  IN   UINT8       DramRuleIndex,
  IN OUT UINT64    *AddressLimit,
  IN OUT UINT8     *NmCacheable,
  IN OUT UINT8     *InterleaveMode
  )
{
  DRAM_RULE_CFG_UNICAST_GROUP0_0_CHA_MISC_STRUCT    DramRuleCfg;
  UINT32                              ChaIndex;
  BOOLEAN                             DramRuleEnabledForMemory;

  ChaIndex = GetChaIndexPerCluster (Socket, Cluster);
  DramRuleCfg.Data = ReadCpuCsr (Socket, (UINT8)ChaIndex, mDramRuleUcCha[DramRuleIndex]);

  if ((DramRuleCfg.Bits.rule_enable == 1) && (DramRuleCfg.Bits.attr != 1)) {
    DramRuleEnabledForMemory = TRUE;
  } else {
    DramRuleEnabledForMemory = FALSE;
  }
  if (DramRuleEnabledForMemory == TRUE) {
    if (AddressLimit != NULL) {
      *AddressLimit = LShiftU64 ( DramRuleCfg.Bits.limit + 1, SAD_UNIT) - 1;
    }

    if (NmCacheable != NULL) {
      *NmCacheable = (UINT8)DramRuleCfg.Bits.nm_cacheable;
    }

    if (InterleaveMode != NULL) {
      if (DramRuleCfg.Bits.interleave_mode == 0) {
        *InterleaveMode = 6;
      }

      if (DramRuleCfg.Bits.interleave_mode == 1) {
        *InterleaveMode = 8;
      }

      if (DramRuleCfg.Bits.interleave_mode == 2) {
        *InterleaveMode = 12;
      }

      if (DramRuleCfg.Bits.interleave_mode == 3) {
        *InterleaveMode = 13;
      }
    }
  }

  return DramRuleEnabledForMemory;
}



/**
  This routine is to get DRAM rule info

  @param[in] Skt   --   socket index
  @param[in] Cluster  -- Cluster index
  @param[in] DramRuleIndex  -- Dram rule index
  @param[in, out]  AddressLimit -- system address limit
  @param[in, out]  NmCacheable  -- NM flag for this range.
  @param[in, out]  McInterleaveMode -- interleave mode of MC
  @param[in, out]  ChInterleaveMode -- interleave mode of channel
  @retval CHA index per the cluster

**/

VOID
EFIAPI
GetDramRuleAttr (
  IN   UINT8       Socket,
  IN   UINT8       Cluster,
  IN   UINT8       DramRuleIndex,
  IN OUT UINT8    *Cxl_accelerator_mem,
  IN OUT UINT8    *Cxl_mem_expander,
  IN OUT UINT8    *NxmFlag,
  IN OUT UINT8    *RouteTableType,
  IN OUT UINT8    *ChannelTableType,
  IN OUT UINT8    *McInterleaveShiftBit,
  IN OUT UINT8    *ChInterleaveShiftBit
  )
{
  DRAM_RULE_CFG2_UNICAST_GROUP0_0_CHA_MISC_STRUCT        DramRuleCfg2;
  DRAM_MC_TARGET_CFG_UNICAST_GROUP0_CHA_MISC_STRUCT      DramMcTarget;
  DRAM_MC_CHANNEL_CFG_UNICAST_GROUP0_CHA_MISC_STRUCT     DramMcChannel;
  DRAM_GLOBAL_INTERLEAVE_CFG_UNICAST_GROUP0_CHA_MISC_STRUCT  DramInterleaveMode;
  UINT8                                                  McRouteTableType;
  UINT8                                                  ChRouteTableType;

  UINT32                               ChaIndex;


  ChaIndex = GetChaIndexPerCluster (Socket, Cluster);

  DramRuleCfg2.Data = ReadCpuCsr (Socket, (UINT8)ChaIndex, mDramRuleCfg2Cha[DramRuleIndex]);

  if (Cxl_accelerator_mem != NULL) {
    *Cxl_accelerator_mem = (UINT8)DramRuleCfg2.Bits.cxl_accelerator_mem;
  }

  if (Cxl_mem_expander != NULL) {
    *Cxl_mem_expander = (UINT8)DramRuleCfg2.Bits.cxl_mem_expander;
  }

  if (NxmFlag != NULL) {
    *NxmFlag = (UINT8)DramRuleCfg2.Bits.nxm;
  }

  DramMcTarget.Data = ReadCpuCsr (Socket, (UINT8)ChaIndex, DRAM_MC_TARGET_CFG_UNICAST_GROUP0_CHA_MISC_REG);
  McRouteTableType = (UINT8) BitFieldRead64 (DramMcTarget.Data, DramRuleIndex * 2, DramRuleIndex * 2 + 1);
  if (RouteTableType != NULL) {
    *RouteTableType = McRouteTableType;
  }



  DramMcChannel.Data = ReadCpuCsr (Socket, (UINT8)ChaIndex, DRAM_MC_CHANNEL_CFG_UNICAST_GROUP0_CHA_MISC_REG);
  ChRouteTableType = (UINT8)BitFieldRead64 (DramMcChannel.Data, DramRuleIndex * 2, DramRuleIndex * 2 + 1);
  if (ChannelTableType != NULL) {
    *ChannelTableType = ChRouteTableType;
  }


  DramInterleaveMode.Data = ReadCpuCsr (Socket, (UINT8)ChaIndex, DRAM_GLOBAL_INTERLEAVE_CFG_UNICAST_GROUP0_CHA_MISC_REG);

  if (McInterleaveShiftBit != NULL) {
    //
    //if case MC route table type is 2LM, use route table 2 to get shift bits.
    //
    if (McRouteTableType == 3) {
      *McInterleaveShiftBit = (UINT8)BitFieldRead64 (DramInterleaveMode.Data, (McRouteTableType - 1) * 4, (McRouteTableType - 1) * 4 + 2);
    } else {
      *McInterleaveShiftBit = (UINT8)BitFieldRead64 (DramInterleaveMode.Data, McRouteTableType * 4, McRouteTableType * 4 + 2);
    }
  }

  if (ChInterleaveShiftBit != NULL) {
    //
    //if case channel route table type is 2LM, use route table 2 to get shift bits.
    //
    if (ChRouteTableType == 3) {
      *ChInterleaveShiftBit = (UINT8)BitFieldRead64 (DramInterleaveMode.Data, (ChRouteTableType - 1) * 4, (ChRouteTableType - 1) * 4 + 2);
    } else {
      *ChInterleaveShiftBit = (UINT8)BitFieldRead64 (DramInterleaveMode.Data, ChRouteTableType * 4, ChRouteTableType * 4 + 2);
    }
  }
}



/**
  This routine is to get DRAM interleave ways

  @param[in] Skt   --   socket index
  @param[in] Cluster  -- Cluster index
  @param[in]  RouteTableIndex  -- route table index. there are 8 route tables. index format {L/R, route type}
  @param[in] DramRuleIndex  -- Dram rule index

  @retval CHA index per the cluster

**/
UINT8
EFIAPI
InitInterleaveWays (
  IN   UINT8       Socket,
  IN   UINT8       Cluster,
  IN   UINT8       RouteTableIndex,
  IN   UINT8       DramRuleIndex
  )
{
  UINT32                                                 ChaIndex;
  DRAM_H0_RT0_MODE0_CFG_UNICAST_GROUP0_CHA_MISC_STRUCT   RtMode0;
  DRAM_H0_RT0_MODE1_CFG_UNICAST_GROUP0_CHA_MISC_STRUCT   RtMode1;
  UINT64                                                 RtMode;
  UINT8                                                  Mode;
  UINT8                                                  IntWays;

  ChaIndex = GetChaIndexPerCluster (Socket, Cluster);

  if(RouteTableIndex < MAX_ROUTE_TABLE){
    RtMode0.Data = ReadCpuCsr (Socket, (UINT8)ChaIndex, mDramInterleaveWays[RouteTableIndex * 2]);
    RtMode1.Data = ReadCpuCsr (Socket, (UINT8)ChaIndex, mDramInterleaveWays[RouteTableIndex * 2 + 1]);
  } else {
    DEBUG ((EFI_D_ERROR, "Array index out of bounds.\n"));
    return 0xFF;
  }

  RtMode = LShiftU64(RtMode1.Data, 32) | RtMode0.Data;

  Mode = (UINT8)BitFieldRead64 (RtMode, DramRuleIndex * 3, DramRuleIndex * 3 + 2);

  switch (Mode) {
    case 0:
      IntWays = INT_ONE_WAY;
      break;

    case 1:
      IntWays = INT_TWO_WAY;
      break;

    case 2:
      IntWays = INT_THREE_WAY;
      break;

    case 3:
      IntWays = INT_FOUR_WAY;
      break;

    case 5:
      IntWays = INT_SIX_WAY;
      break;

    case 6:
      IntWays = INT_EIGHT_WAY;
      break;

    default:
      IntWays = INT_ONE_WAY;
  }

  return  IntWays;
}


/**
  This routine is to get  target ID per socket-cluster-DRAM rule.

  @param[in] Skt   --   socket index
  @param[in] Cluster  -- Cluster index
  @param[in] DramRuleIndex -- dram rule index
  @param[in] InterleaveListIndex  -- interleave index

  @retval target id

**/
UINT8
EFIAPI
GetChaTargetId (
  IN   UINT8       Socket,
  IN   UINT8       Cluster,
  IN   UINT8       DramRuleIndex,
  IN   UINT8       InterleaveListIndex
  )
{
  UINT32                                                ChaIndex;
  INTERLEAVE_LIST_CFG_UNICAST_GROUP0_0_CHA_MISC_STRUCT  InterLeaveCfg;
  UINT8                                                 TargetId;

  ChaIndex = GetChaIndexPerCluster (Socket, Cluster);

  InterLeaveCfg.Data =  ReadCpuCsr (Socket, (UINT8)ChaIndex, mInterleaveListUcCha[DramRuleIndex]);
  TargetId = (UINT8)BitFieldRead64 (InterLeaveCfg.Data, InterleaveListIndex * 4, InterleaveListIndex * 4 + 3);

  return TargetId;
}


/**
  This routine is to get MC per socket-cluster-route table-target id

  HBM                          cluster 0                 cluster 1                    cluster 2                       cluster 3
  CHA                           MC4~ 7                   MC4~ 7                       MC4~ 7                           MC4~ 7
  M2M/MC                        MC4~7                    MC8~11                       MC12~15                          MC16~19                      Unified MC encode
  M2M/MC                        HBM MC0~3                HBM4~7                       HBM MC8~11                       HBM MC 12~15                 HBM MC encode

  @param[in] Skt   --   socket index
  @param[in] Cluster  -- Cluster index
  @param[in] RouteTableType -- route table type
  @param[in] RouteTableIndex  -- route table index

  @retval MC id

**/
UINT8
EFIAPI
GetRouteTableMc (
  IN   UINT8       Socket,
  IN   UINT8       Cluster,
  IN   UINT8       RouteTableType,
  IN   UINT8       RouteTableIndex
  )
{
  UINT32                                  ChaIndex;
  H0_TGT_ROUTE_TABLE_0_CHA_MISC_STRUCT    RouteTableMc;
  UINT8                                   Mc;
  UINT8                                   TargetMc;

  ChaIndex = GetChaIndexPerCluster (Socket, Cluster);

  if(RouteTableType < MAX_ROUTE_TABLE){
    RouteTableMc.Data =  ReadCpuCsr (Socket, (UINT8)ChaIndex, mDramRouteTable[RouteTableType * 2]);
  }else{
    DEBUG ((EFI_D_ERROR, "Array index out of bounds.\n"));
    return 0xFF;
  }


  Mc = (UINT8)BitFieldRead64(RouteTableMc.Data, RouteTableIndex * 4, RouteTableIndex * 4 + 3);

  if (Mc < 4) {
    TargetMc = Mc;
  } else if ((Mc >= 4) && (Mc <= 7)) {
    TargetMc = Cluster * 4 + Mc;
  } else {
    TargetMc = 0;
    DEBUG ((EFI_D_ERROR, "[GetRouteTableMc] Wrong MC ID Socket:%d  Cluser:%d   RouteTableType:%d   RouteTableIndex:%d \n", Socket, Cluster, RouteTableType, RouteTableIndex));
  }

  return TargetMc;
}


/**
  This routine is to get channel per socket-cluster-route table type -route table index

  @param[in] Skt   --   socket index
  @param[in] Cluster  -- Cluster index
  @param[in] RouteTableType -- route table type
  @param[in] RouteTableIndex  -- route table index

  @retval MC id

**/
UINT8
EFIAPI
GetRouteTableCh (
  IN   UINT8       Socket,
  IN   UINT8       Cluster,
  IN   UINT8       RouteTableType,
  IN   UINT8       RouteTableIndex
  )
{
  UINT32                                     ChaIndex;
  H0_CH_ROUTE_TABLE_0_CHA_MISC_STRUCT        RouteTableCh;

  ChaIndex = GetChaIndexPerCluster (Socket, Cluster);

  if(RouteTableType < MAX_ROUTE_TABLE){
    RouteTableCh.Data =  ReadCpuCsr (Socket, (UINT8)ChaIndex, mDramRouteTable[RouteTableType * 2 + 1]);
  }else{
    DEBUG ((EFI_D_ERROR, "Array index out of bounds.\n"));
    return 0xFF;
  }


  return (UINT8)BitFieldRead64(RouteTableCh.Data, RouteTableIndex * 2, RouteTableIndex * 2 + 1);
}

/**
  This routine is to get remote DRAM rule info

  @param[in] Skt   --   socket index
  @param[in] DramRuleIndex  -- Remote Dram rule index
  @param[in, out]  StartAddress -- start system address
  @param[in, out]  StartAddress -- end system address
  @param[in, out]  package -- package index

  @retval indicator of remote dram rule is valid and point for memory.

**/

BOOLEAN
EFIAPI
GetRemoteRule(
  IN   UINT8       Socket,
  IN   UINT8       RemoteDramRuleIndex,
  IN OUT UINT64    *StartAddress,
  IN OUT UINT64    *EndAddress,
  IN OUT UINT8     *Package
  )
{
  REMOTE_DRAM_RULE_CFG_0_N0_CHABC_SAD_STRUCT     RemoteDram;
  REMOTE_DRAM_RULE_CFG_0_N1_CHABC_SAD_STRUCT     RemoteDramN1;
  BOOLEAN                                        ValidFlag;

  RemoteDram.Data = ReadCpuCsr (Socket, 0, mRemoteDramRuleCfgCha[RemoteDramRuleIndex]);

  if ((RemoteDram.Bits.rule_enable == 1) && (RemoteDram.Bits.attr != 1)) {
    ValidFlag = TRUE;
  } else {
    ValidFlag = FALSE;
  }

  if (ValidFlag) {

    RemoteDramN1.Data = ReadCpuCsr (Socket, 0, mRemoteDramRuleCfgChaN1[RemoteDramRuleIndex]);

    if (StartAddress != NULL) {
      *StartAddress = LShiftU64 (RemoteDram.Bits.base, SAD_UNIT);
    }

    if (EndAddress != NULL) {
      *EndAddress = LShiftU64 (RemoteDramN1.Bits.limit + 1, SAD_UNIT) - 1;
    }

    if (Package != NULL) {
      *Package = (UINT8) RemoteDramN1.Bits.pkg;
    }
  }

  return  ValidFlag;
}




/**
 This routine is to initialize CHA route type (MC, channel) per socket - cluster.

 @param[in]    Socket    --   socket index
 @param[in]    Cluster   --   cluster index
 @param[in]    pChaArray  --  point to array of CHA.

 @retval       none.

**/
VOID
EFIAPI
InitChaRouteTable(
  IN   UINT8             Socket,
  IN   UINT8             Cluster,
  IN   CLUSTER_INFO    * pCluster
  )
{
  UINT8                                   RouteTableIndex;
  UINT8                                   TargetId;

  DEBUG ((EFI_D_INFO, "  [ChaDramRule] Route Table (MC/CH) by dec   Cluster:%d \n", Cluster));
  for (RouteTableIndex = 0; RouteTableIndex < MAX_ROUTE_TABLE; RouteTableIndex ++) {
    DEBUG ((EFI_D_INFO, "  Route Table %d: ", RouteTableIndex));
    for (TargetId = 0; TargetId < MAX_TARGET; TargetId ++) {
      pCluster[Cluster].McTable[RouteTableIndex][TargetId] = GetRouteTableMc (Socket, Cluster, RouteTableIndex, TargetId);
      pCluster[Cluster].ChTable[RouteTableIndex][TargetId] = GetRouteTableCh (Socket, Cluster, RouteTableIndex, TargetId);
      DEBUG ((EFI_D_INFO, " %d/%d  ",
            pCluster[Cluster].McTable[RouteTableIndex][TargetId], pCluster[Cluster].ChTable[RouteTableIndex][TargetId]));
    }
    DEBUG ((EFI_D_INFO, "\n"));
  }
}

/**
 This routine is to initialize CHA arrary at POST phase.

 @param[in]    Socket    --   socket index
 @param[in]    pChaArray  --  point to array of CHA.

 @retval       none.

**/
VOID
EFIAPI
InitChaDramRule (
  IN   UINT8             Socket,
  IN   CLUSTER_INFO    * pCluster
  )
{
  UINT8      Cluster;
  UINT8      LocalDramRule;
  UINT32     MaxCluster;
  UINT64     LimitAddress;
  UINT8      NmCacheable;
  UINT64     PreviousLimit;
  UINT8      InterleaveMode;

  UINT8      CxlAcceleratorMem;
  UINT8      CxlMemExpander;
  UINT8      NxmFlag;
  UINT8      RouteTableType;
  UINT8      ChannelTableType;
  UINT8      McInterleaveShiftBit;
  UINT8      ChInterleaveShiftBit;

  UINT8      RtTableIndex;
  UINT8      InterleaveWays;
  UINT8      InterleavelistIndex;

  PreviousLimit = 0;

  MaxCluster = GetClusterNum (Socket);

  for (Cluster = 0; Cluster < MaxCluster; Cluster ++) {

    InitChaRouteTable (Socket, Cluster, pCluster);

    if (IsUbcModeEnabled (Socket)) {
      PreviousLimit = 0;
    }

    for (LocalDramRule = 0; LocalDramRule < MAX_LOCAL_DRAM_RULE; LocalDramRule ++) {

      if (GetDramRule (Socket, Cluster, LocalDramRule, &LimitAddress, &NmCacheable, &InterleaveMode) == FALSE){
        continue;
      } else {
        pCluster[Cluster].DramRule[LocalDramRule].LocalDramRuleValid = TRUE;
        pCluster[Cluster].DramRule[LocalDramRule].StartAddress = PreviousLimit;
        pCluster[Cluster].DramRule[LocalDramRule].EndAddress =  LimitAddress;
        pCluster[Cluster].DramRule[LocalDramRule].Socket = Socket;
        pCluster[Cluster].DramRule[LocalDramRule].InterleaveMode = InterleaveMode;
        pCluster[Cluster].DramRule[LocalDramRule].NmCachable = NmCacheable;

        GetDramRuleAttr (Socket, Cluster, LocalDramRule, &CxlAcceleratorMem, &CxlMemExpander, &NxmFlag,
                                &RouteTableType, &ChannelTableType, &McInterleaveShiftBit, &ChInterleaveShiftBit);

        pCluster[Cluster].DramRule[LocalDramRule].CxlAcceleratorMem = CxlAcceleratorMem;
        pCluster[Cluster].DramRule[LocalDramRule].CxlMemExpander = CxlMemExpander;
        pCluster[Cluster].DramRule[LocalDramRule].NxmFlag = NxmFlag;
        pCluster[Cluster].DramRule[LocalDramRule].RouteTableType = RouteTableType;
        pCluster[Cluster].DramRule[LocalDramRule].ChannelTabletype = ChannelTableType;
        pCluster[Cluster].DramRule[LocalDramRule].McInterleaveShift = McInterleaveShiftBit;
        pCluster[Cluster].DramRule[LocalDramRule].ChInterleaveShift = ChInterleaveShiftBit;

        DEBUG ((EFI_D_INFO, "  [ChaDramRule] DramRule:%x Base:%lx End:%lx  IntMode:%x \n", LocalDramRule,
                         pCluster[Cluster].DramRule[LocalDramRule].StartAddress, LimitAddress, InterleaveMode));

        DEBUG ((EFI_D_INFO, "    Nm:%d, Cxl:%d CxlExp:%d Nxm:%d RT:%d ChT:%d McShift:%d  ChShift:%d\n", NmCacheable,
                         CxlAcceleratorMem, CxlMemExpander, NxmFlag, RouteTableType, ChannelTableType, McInterleaveShiftBit, ChInterleaveShiftBit));

         DEBUG ((EFI_D_INFO, "  Interleave Ways by route table index: "));
        for (RtTableIndex = 0; RtTableIndex < MAX_ROUTE_TABLE;  RtTableIndex ++) {
          InterleaveWays = InitInterleaveWays(Socket, Cluster, RtTableIndex, LocalDramRule);
          pCluster[Cluster].DramRule[LocalDramRule].InterleaveWays[RtTableIndex] = InterleaveWays;

          DEBUG ((EFI_D_INFO, "%x  ", InterleaveWays));
        }
         DEBUG ((EFI_D_INFO, " \n"));

        DEBUG ((EFI_D_INFO, " Target ID per interleaveList-- "));
        for (InterleavelistIndex = 0; InterleavelistIndex < MAX_TARGET;InterleavelistIndex ++) {
          pCluster[Cluster].DramRule[LocalDramRule].TargetTable[InterleavelistIndex] = GetChaTargetId (Socket, Cluster, LocalDramRule, InterleavelistIndex);
          DEBUG ((EFI_D_INFO, " %x ", pCluster[Cluster].DramRule[LocalDramRule].TargetTable[InterleavelistIndex]));
        }
         DEBUG ((EFI_D_INFO, " \n"));

        PreviousLimit = pCluster[Cluster].DramRule[LocalDramRule].EndAddress + 1;

      }

    }
  }
}

/**
 This routine is to check whether block mode is enabled.


 @retval       TRUE --  block mode is enabled

**/
BOOLEAN
EFIAPI
IsBlockmodeEnable (
  VOID
  )
{
  return mSadTadScope[0].BlockMode.BlockModeEn;
}

/**
 This routine is to initialize CHA block mode info.

 @param[in]    Skt    --   socket index


 @retval       none.

**/
VOID
EFIAPI
InitBlockMode (
  IN      UINT8      Skt
  )
{
  BLOCK_DECODER_ADDR_N0_CHABC_SAD1_STRUCT    BlockDecoder0;
  BLOCK_DECODER_ADDR_N1_CHABC_SAD1_STRUCT    BlockDecoder1;

  BlockDecoder0.Data = ReadCpuCsr (Skt, 0, BLOCK_DECODER_ADDR_N0_CHABC_SAD1_REG);
  BlockDecoder1.Data = ReadCpuCsr (Skt, 0, BLOCK_DECODER_ADDR_N1_CHABC_SAD1_REG);

  if (BlockDecoder1.Bits.block_rule_en == TRUE) {
    mSadTadScope[Skt].BlockMode.BlockModeEn = TRUE;
    mSadTadScope[Skt].BlockMode.BlockBase = LShiftU64 (BlockDecoder0.Bits.block_base, SAD_UNIT);
    mSadTadScope[Skt].BlockMode.BlockLimit =  LShiftU64 (BlockDecoder1.Bits.block_limit + 1, SAD_UNIT) - 1;
  }

}


/**
 This routine is to initialize socket 0 remote dram rule.

 @param[in]    pRemoteDramRule    --   point to remote dram rule info.


 @retval       none.

**/
VOID
EFIAPI
InitRemoteDramRule (
  IN     REMOTE_DRAM_RULE  * pRemoteDramRule
  )
{
  UINT8      RemoteDramRule;
  UINT64     StartAddress;
  UINT64     EndAddress;
  UINT8      Socket;

  //
  //Only init socket 0 remote DRAM rule.
  //
  for (RemoteDramRule = 0; RemoteDramRule < MAX_REMOTE_DRAM_RULE; RemoteDramRule++) {

    if(GetRemoteRule(0, RemoteDramRule, &StartAddress, &EndAddress, &Socket) == TRUE) {
      pRemoteDramRule[RemoteDramRule].RemoteDramRuleValid = TRUE;
      pRemoteDramRule[RemoteDramRule].StartAddress = StartAddress;
      pRemoteDramRule[RemoteDramRule].EndAddress = EndAddress;
      pRemoteDramRule[RemoteDramRule].Socket = Socket;
    }
  }
}
