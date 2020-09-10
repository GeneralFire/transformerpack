/** @file
  Implementation of MC decode.

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
#include <Library/CpuCsrAccessLib.h>
#include <Library/AddressDecodeLib.h>
#include <Library/RasRuntimeStatusLib.h>
#include "AddressDecodeInternal.h"
#include "Mirror.h"

extern AD_SOCKET_INFO              *mSadTadScope;


/**
  calculate channel address per tad info and system address.

   @param[in]    Sa    --   system address
   @param[in]    TadOffset        --   tad offset
   @param[in]    TargetWay   --   target way
   @param[in]    TargetGran --  target granularity
   @param[in]    ChWay  -- channel ways
   @param[in]    ChGran -- Channel granularity
   @param[in]    Sign -- Tad sign

   @retval        channel address

**/
UINT64
EFIAPI
ForwardCalcCa (
  IN     UINT64       Sa,
  IN     UINT64       TadOffset,
  IN     UINT8        TargetWay,
  IN     UINT8        TargetGran,
  IN     UINT8        ChWay,
  IN     UINT8        ChGran,
  IN     UINT8        Sign
  )
{
  UINT64            Ta;
  UINT8             Z1;
  UINT8             Z2;
  UINT8             W1;
  UINT8             W2;
  UINT64            Temp;
  UINT64            Ca;


  if (TargetGran >= ChGran) {
    Z1 = ChGran;
    W1 = ChWay;
    Z2 = TargetGran;
    W2 = TargetWay;
  } else {
    Z1 = TargetGran;
    W1 = TargetWay;
    Z2 = ChGran;
    W2 = ChWay;
  }

  if (Sign == 1) {
    Ta = Sa + TadOffset;
  } else {
    Ta = Sa - TadOffset;
  }

  Temp = DivU64x32 (RShiftU64 (Ta, Z2), W2);
  Ca = LShiftU64 (Temp, Z2) | BitFieldRead64 (Ta, 0, Z2-1);

  Temp = DivU64x32 (RShiftU64 (Ca, Z1), W1);
  Ca = LShiftU64 (Temp, Z1) | BitFieldRead64 (Ca, 0, Z1-1);

  DEBUG ((EFI_D_INFO, "[ForwardCalcCa] Sa:%lx  TadOff:%lx  TargetWay:%d  TargetGran:%d  ChWay:%d  ChGran:%d  Sign:%d \n", Sa, TadOffset, TargetWay, TargetGran, ChWay, ChGran, Sign));
  DEBUG ((EFI_D_INFO, "    channel address  -- %lx \n", Ca));

  return Ca;
}

/**
  Get target ways and channel ways per tad.


   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index
   @param[in]    TadIndex  --   tad index
   @param[in, out]  TargetWays -- target ways
   @param[in, out]  ChannelWays -- channel ways

   @retval       channel address.

**/
VOID
EFIAPI
GetTadInfo (
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Channel,
  IN      UINT8     TadIndex,
  IN OUT  UINT8    *TargetWays,
  IN OUT  UINT8    *ChannelWays
  )
{
  if (TargetWays != NULL) {
    *TargetWays = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].TadInfo[TadIndex].TargetWay;
  }

  if (ChannelWays != NULL) {
    *ChannelWays = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].TadInfo[TadIndex].ChWay;
  }
}


/**
  Get channel address per tad info and system address.

   @param[in]    Sa        --   System address
   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index
   @param[in]    TadIndex  --   tad index

   @retval          channel address.

**/
UINT64
EFIAPI
Forward1lmDecodeGetCa (
  IN      UINT64    Sa,
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Channel,
  IN      UINT8     TadIndex
  )
{
  UINT64    ChannelAddress;

  if(TadIndex >= MAX_TAD){
    return (UINT64) -1;
  }

  if (mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].TadInfo[TadIndex].TadValid) {
    DEBUG ((EFI_D_ERROR, "[Forward1lmDecodeGetCa]  TAD skt:%d  Mc:%d  Chn:%d  TadIndex:%d \n", Socket, Mc, Channel, TadIndex));
    ChannelAddress = ForwardCalcCa (Sa,
           mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].TadInfo[TadIndex].TadOffset,
           mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].TadInfo[TadIndex].TargetWay,
           mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].TadInfo[TadIndex].TargetGran,
           mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].TadInfo[TadIndex].ChWay,
           mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].TadInfo[TadIndex].ChGran,
           mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].TadInfo[TadIndex].TadSign
           );
  } else {
    DEBUG ((EFI_D_ERROR, "[Forward1lmDecodeGetCa] wrong TAD skt:%d  Mc:%d  Chn:%d  TadIndex:%d \n", Socket, Mc, Channel, TadIndex));
    return (UINT64) -1;
  }

  ChannelAddress = MirrorPriCa(Sa, Socket, Mc, ChannelAddress);
  return ChannelAddress;
}



/**
  calculate NM channel address per tad info and system address.

   @param[in]    Sa    --   system address
   @param[in]    TadOffset        --   tad offset
   @param[in]    TargetWay   --   target way
   @param[in]    TargetWay --   target ways
   @param[in]    TargetGran --  target granularity
   @param[in]    ChWay  -- channel ways
   @param[in]    ChGran -- Channel granularity
   @param[in]    Sign -- Tad sign

   @retval       nm channel address

**/
UINT64
EFIAPI
Forward2lmCalcCa (
  IN     UINT64    Sa,
  IN     UINT8     NmTargetWay,
  IN     UINT8     NmChnWay,
  IN     UINT8     NmTargetGran,
  IN     UINT8     NmChnGran,
  IN     UINT64    NmCachingOffset,
  IN     UINT8     NmChnCap,
  IN     UINT8     NmRatioChnCap
  )
{
  UINT64            Ta;
  UINT64            Ca;
  UINT8             TagIndex;
  UINT8             NmTargetBitWide;
  UINT8             NmChnBitWide;
  UINT8             NmCapBitWide;
  UINT64            NmCap = 0;
  UINT64            ChnCap;
  UINT8             BlockNum;

  UINT64            Temp;
  UINT64            CaTemp;

  Ta = Sa - NmCachingOffset;

  NmTargetBitWide = (UINT8)HighBitSet64 (NmTargetWay);
  NmChnBitWide = (UINT8)HighBitSet64 (NmChnWay);

  ChnCap = LShiftU64 (1, NmChnCap);
  if (NmRatioChnCap <= 32) {
    NmCap = DivU64x32 (ChnCap, NmRatioChnCap);  //it is power_of_2.
    NmCapBitWide = (UINT8)HighBitSet64 (NmCap);
    TagIndex = NmCapBitWide + NmTargetBitWide + NmChnBitWide;
  } else {
    NmCapBitWide = (UINT8)HighBitSet64 (ChnCap);
    TagIndex = NmCapBitWide + NmTargetBitWide + NmChnBitWide;
    BlockNum = (UINT8) BitFieldRead64 (Ta, TagIndex - 5, TagIndex - 1);
    if (BlockNum <= NmRatioChnCap - 32) {
      TagIndex --;
    }
  }


  DEBUG ((EFI_D_INFO, "  [Forward2lmCalcCa] TagIndex:%d NmWide:%d   Ta:%lx\n", TagIndex, NmCapBitWide, Ta));


  CaTemp = BitFieldRead64 (Ta, 0, TagIndex -1);

  Temp =  DivU64x32 (RShiftU64 (CaTemp, NmChnGran), NmChnWay);
  Ca = LShiftU64 (Temp, NmChnGran) | BitFieldRead64 (CaTemp, 0, NmChnGran-1);

  Temp = DivU64x32 (RShiftU64 (Ca, NmTargetGran), NmTargetWay);
  Ca = LShiftU64 (Temp, NmTargetGran) | BitFieldRead64 (Ca, 0, NmTargetGran-1);

  return Ca;
}

/**
  Get NM channel address per 2LM decode.

   @param[in]    Sa        --   System address
   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index

   @retval       NM channel address.

**/
UINT64
EFIAPI
Forward2lmDecodeGetCa (
  IN      UINT64    Sa,
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Channel
  )
{
  if (mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmTadValid) {
    return Forward2lmCalcCa (Sa,
           mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmTargetWay,
           mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmChnWay,
           mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmTargetGran,
           mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmChnGran,
           mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmCachingOffset,
           mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmChnCap,
           mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmRatioChnCap
           );
  } else {
    DEBUG ((EFI_D_ERROR, "[Forward2lmDecodeGetCa] NM tad is not exist \n"));
    return (UINT64)(-1);
  }
}



/**
  This routine is to get FM chn ID.

   @param[in]    Sa    --   system address
   @param[in]    FmTargetWay  -- far mc controller ways
   @param[in]    FmChnWay  -- far mc chn way
   @param[in]    FmChnGran  -- far mc chn gran
   @param[in]    FmChnL2Pid  -- far mc logic to phyical id map.

   @ret  FM chn ID.
**/
UINT8
EFIAPI
GetFmChn (
  IN    UINT64      Sa,
  IN    UINT8       FmTargetWay,
  IN    UINT8       FmChnWay,
  IN    UINT8       FmChnGran,
  IN    UINT32      FmChnL2Pid
  )
{
  UINT8        StartBit;
  UINT8        LogicId;
  UINT8        ChnWay;

  StartBit = FmChnGran + (UINT8)HighBitSet32(FmTargetWay);

  ChnWay = FmChnWay;
  if (FmChnWay == 1) {
    LogicId = 0;
  } else {
    if (FmChnWay == 3) {
      ChnWay = 4;
    }

    LogicId = (UINT8)BitFieldRead64 (Sa, StartBit, StartBit + (UINT8)HighBitSet32(ChnWay));
    LogicId = LogicId % 3;
  }

  return (UINT8)BitFieldRead32(FmChnL2Pid, (LogicId * 2), (LogicId * 2) + 1);
}


/**
  Get FM mc/chn from NM channel info

   @param[in]    Sa        --   System address
   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index
   @param[in, out]  FmMc   --   Far mc
   @param[in, out]  FmChn  --   Far chn

   @retval       TRUE -- get far mc/chn successfully

**/
BOOLEAN
EFIAPI
GetFmInfoFromNmChn (
  IN      UINT64    Sa,
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Channel,
  IN      UINT8     *FmMc,
  IN      UINT8     *FmChn
  )
{
  UINT8      McDramRuleIndex;

  for (McDramRuleIndex = 0; McDramRuleIndex < MAX_MC_DRAM_RULE; McDramRuleIndex ++) {
   if (mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].ChSad[McDramRuleIndex].McDramRuleValid == FALSE) {
     continue;
   }

   if ((Sa >= mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].ChSad[McDramRuleIndex].FmBase) &&
      (Sa <= mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].ChSad[McDramRuleIndex].FmLimit)) {
        if (FmMc != NULL) {
          *FmMc = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].ChSad[McDramRuleIndex].FmTarget;
        }

        if (FmChn != NULL) {
          *FmChn = GetFmChn (Sa,
                 mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].ChSad[McDramRuleIndex].FmTargetWay,
                 mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].ChSad[McDramRuleIndex].FmChnWay,
                 mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].ChSad[McDramRuleIndex].FmChnGran,
                 mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].ChSad[McDramRuleIndex].FmChnL2Pid);
        }
        return TRUE;
      }
  }

  return FALSE;

}


/**
  Calculate rank address.

   @param[in]    Sa        --   System address
   @param[in]    ClosePg   --   close page enabled
   @param[in]    RirWay    --   RIR way
   @param[in]    RirOffset --   RIR offset
   @param[in]    PchRemove --   TRUE - remove subchannel info

   @retval       Rank address

**/
UINT64
EFIAPI
CalcRa (
  IN          UINT64     Ca,
  IN          UINT8      ClosePg,
  IN          UINT8      RirWay,
  IN          UINT64     RirOffset
  )
{
  UINT8        ShiftBit;
  UINT64       Temp;
  UINT64       Ra;


  if (ClosePg == TRUE) {
    ShiftBit = CLOSE_PG;
  } else {
    ShiftBit = OPEN_PG;
  }

  Temp = DivU64x32(RShiftU64(Ca, ShiftBit), RirWay);
  Ra = LShiftU64 (Temp, ShiftBit) | BitFieldRead64(Ca, 0, ShiftBit - 1);
  Ra -= RirOffset;

  return Ra;
}




/**
  Get rank interlave index.


   @param[in]    Ca        --   channel address, it could be Sub CA for DDR5/HBM.
   @param[in]    ClosePg   --   close page enabled
   @param[in]    RankXorEnable  -- Rank xor enable.
   @param[in]    RirWay    --   RIR way

   @retval       rank interleave Bit

**/
UINT8
EFIAPI
GetRankInterleaveIndex (
  IN        UINT64      Ca,
  IN        UINT8       ClosePg,
  IN        UINT8       RankXorEnable,
  IN        UINT8       RirWay
  )
{
  UINT8        ShiftBit;
  UINT8        InterleaveBit;


  if (ClosePg == TRUE) {
    ShiftBit = CLOSE_PG;
  } else {
    ShiftBit = OPEN_PG;
  }

  InterleaveBit = (UINT8)ModU64x32(RShiftU64 (Ca, ShiftBit), RirWay);


  return InterleaveBit;
}


/**
  calculate PCH value per system address and xor.

   @param[in]    Sa        --   System address
   @param[in]    PchXor    --   whether pch xor is enabled

   @retval       PCH

**/
UINT8
EFIAPI
CalcPch (
  IN      UINT64    Sa,
  IN      UINT8     PchXor
  )
{
  UINT8     Pch;

  Pch = (UINT8)BitFieldRead64 (Sa, 6, 6);
  if (PchXor == TRUE) {
    Pch = (UINT8) (BitFieldRead64 (Sa, 6, 6) ^ BitFieldRead64 (Sa, 13, 13) ^ BitFieldRead64 (Sa, 21, 21));
  }
  return Pch;
}


/**
  calculate HBM rank, and rank address

   @param[in]    Sa        --   System address
   @param[in]    Ca        --   Channel address
   @param[in]    ClosePg   --   close page enable
   @param[in]    PchXorEnable -- pch xor enable
   @param[in, out]  HbmRank   --   point to hbm rank
   @param[in, out]  HbmRa     --   point to hbm rank address

   @retval       none

**/
VOID
EFIAPI
GetHbmRankAddress (
  IN      UINT64    Sa,
  IN      UINT64    Ca,
  IN      UINT8     ClosePg,
  IN      UINT8     PchXorEnable,
  IN OUT  UINT8 *   HbmRank,
  IN OUT  UINT64 *  HbmRa,
  IN      UINT8     SidNum
  )
{
  UINT8     Pch;
  UINT8     Sid = 0;
  UINT64    Temp;

  Pch = CalcPch (Sa, PchXorEnable);

  //
  //SID bit is rank[1]
  //
  if (SidNum != 0) {
    if (ClosePg) {
      Sid = (UINT8)BitFieldRead64 (Ca, 10, 10);
    } else {
      Sid = (UINT8)BitFieldRead64 (Ca, 14, 14);
    }
  }

  if (HbmRank != NULL) {
    *HbmRank = (Sid << 1) | Pch;
  }

  if (ClosePg) {
    Temp = BitFieldRead64 (Ca, 0, 9) | LShiftU64(BitFieldRead64 (Ca, 11, 63), 10);
  } else {
    Temp = BitFieldRead64 (Ca, 0, 13) | LShiftU64(BitFieldRead64 (Ca, 15, 63), 14);
  }
  Temp = BitFieldRead64 (Temp, 0, 5) | LShiftU64(BitFieldRead64 (Temp, 7, 63), 6);

  if (HbmRa != NULL) {
    *HbmRa = Temp;
  }
}

/**
  forward RIR decode for HBM/DDR5.

   @param[in]    Sa        --   System address
   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index
   @param[in]    Ca        --   Channel address
   @param[in, out]  Rank   --   point to rank
   @param[in, out]  Ra     --   point to rank address

   @retval       TRUE -- RIR decode succcessfully.

**/
BOOLEAN
EFIAPI
ForwardRirDecode (
  IN      UINT64    Sa,
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Channel,
  IN      UINT64    Ca,
  IN OUT  UINT8    *Rank,
  IN OUT  UINT64   *Ra
  )
{
  UINT8           RirIndex;
  UINT8           InterleaveIndex;
  UINT8           ClosePg;
  UINT8           RankXorEnable;
  UINT8           RirWay;
  UINT64          RirOffset;
  UINT8           HbmType;
  UINT8           PchXorEnable;
  UINT8           HbmRank;
  UINT64          HbmRa;
  UINT64          SubCa;
  UINT8           TempRank;
  UINT8           DimmXorEnable;
  UINT8           SidNum;

  HbmType = IsHbmDimm (Socket, Mc);
  ClosePg = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].ClosePg;
  RankXorEnable = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RankXorEnable;
  DimmXorEnable = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DimmXorEnable;
  PchXorEnable = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].PchXorEnable;
  SidNum = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DimmInfo[0].Sid;  //assume HBM ia at DIMM 0.

  if (HbmType) {
    GetHbmRankAddress (Sa, Ca, ClosePg, PchXorEnable, &HbmRank, &HbmRa, SidNum);
    if (Rank != NULL) {
      *Rank = HbmRank;
    }

    if (Ra != NULL) {
      *Ra = HbmRa;
    }

    return TRUE;
  }

  SubCa = BitFieldRemove64 (Ca, 6, 6);
  DEBUG ((EFI_D_ERROR, "    Sub Channel address:%lx \n", SubCa));
  //
  //scan RIR to figure out which entity meet.
  //
  for (RirIndex = 0; RirIndex < MAX_RIR; RirIndex ++) {
    if ((SubCa >= mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RirInfo[RirIndex].RirBase) &&
       (SubCa <= mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RirInfo[RirIndex].RirLimit)) {
         break;
       }
  }

  if (RirIndex >= MAX_RIR) {
    DEBUG ((EFI_D_ERROR, "[ForwardRirDecode] Rir scan fail Socket:%d  Mc:%d  Channel:%d  Channel Address:%lx \n", Socket, Mc, Channel, Ca));
    return FALSE;
  }


  RirWay = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RirInfo[RirIndex].RirWay;
  InterleaveIndex = GetRankInterleaveIndex (SubCa, ClosePg, RankXorEnable, RirWay);



  DEBUG ((EFI_D_INFO, " [ForwardRirDecode]Skt:%d Mc:%d Chn:%d RirIndex:%d ClosePg:%d RankXor:%d RirWay:%d InterleaveIndex:%d\n",
    Socket, Mc, Channel, RirIndex, ClosePg, RankXorEnable,  RirWay, InterleaveIndex));


  if (Rank != NULL) {

      TempRank= (mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RirInfo[RirIndex].RirIlv[InterleaveIndex].RankId);

      //
      //rank[1] ^ SA[27]
      //
      if (RankXorEnable) {
        TempRank = BitFieldWrite8 (TempRank, 1, 1, (UINT8) (BitFieldRead64(Sa, 27, 27) ^ BitFieldRead64(TempRank, 1, 1)));
      }

      //
      //rank[2] ^ SA[28]
      //
      if (DimmXorEnable) {
        TempRank = BitFieldWrite8 (TempRank, 2, 2, (UINT8) (BitFieldRead64(Sa, 28, 28) ^ BitFieldRead64(TempRank, 2, 2)));
      }

      //
      //Rank[0] = PCH bit. Other bank means real rank number on channal.
      //
      *Rank = TempRank  | CalcPch (Sa, PchXorEnable);
      DEBUG ((EFI_D_ERROR, "    rank :%d \n", *Rank));
  }

  if (Ra != NULL) {
      RirOffset = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RirInfo[RirIndex].RirIlv[InterleaveIndex].RirOffset;
      *Ra = CalcRa (SubCa, ClosePg, RirWay, RirOffset);
      DEBUG ((EFI_D_ERROR, "    rank address:%lx \n", *Ra));
  }

  return TRUE;
}



/**
  forward DDRT RIR decode for HBM/DDR5.

   @param[in]    Sa        --   System address
   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index
   @param[in]    Ca        --   Channel address
   @param[in, out]  Rank   --   point to rank
   @param[in, out]  Ra     --   point to rank address

   @retval       TRUE -- RIR decode succcessfully.

**/
BOOLEAN
EFIAPI
ForwardDdrtRirDecode (
  IN      UINT64    Sa,
  IN      UINT8     Socket,
  IN      UINT8     Mc,
  IN      UINT8     Channel,
  IN      UINT64    Ca,
  IN OUT  UINT8    *Rank,
  IN OUT  UINT64   *Ra
  )
{
  UINT8           RirIndex;
  UINT8           InterleaveIndex;
  UINT8           ClosePg;
  UINT8           RankXorEnable;
  UINT8           RirWay;
  UINT64          RirOffset;

  //
  //scan RIR to figure out which entity meet.
  //
  for (RirIndex = 0; RirIndex < MAX_DDRT_RIR; RirIndex ++) {
    if ((Ca >= mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DdrtRirInfo[RirIndex].RirBase) &&
       (Ca <= mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DdrtRirInfo[RirIndex].RirLimit)) {
         break;
       }
  }

  if (RirIndex >= MAX_DDRT_RIR) {
    DEBUG ((EFI_D_ERROR, "[ForwardFmRirDecode] Rir scan fail Socket:%d  Mc:%d  Channel:%d  Channel Address:%lx \n", Socket, Mc, Channel, Ca));
    return FALSE;
  }

  ClosePg = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].ClosePg;
  RankXorEnable = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RankXorEnable;
  RirWay = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DdrtRirInfo[RirIndex].RirWay;
  InterleaveIndex = GetRankInterleaveIndex ( Ca,ClosePg, RankXorEnable, RirWay);

  if (Rank != NULL) {
    *Rank = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DdrtRirInfo[RirIndex].RirIlv[InterleaveIndex].RankId;
  }

  if (Ra != NULL) {
    RirOffset = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DdrtRirInfo[RirIndex].RirIlv[InterleaveIndex].RirOffset;
    *Ra = CalcRa (Ca, ClosePg, RirWay, RirOffset);
  }
  return TRUE;
}


/**

Get the DIMM Type for the given DIMM info

   @param[in]    Socket    --   socket index
   @param[in]    Mc        --   MC index
   @param[in]    Channel   --   Channel index
   @param[in]    Rank      --   Rank index on channel

@retval DIMM type
**/
UINT8
EFIAPI
GetDimmType(
  IN    UINT8     Skt,
  IN    UINT8     Mc,
  IN    UINT8     Channel,
  IN    UINT8     Rank
  )
{
  UINT8     DimmType;
  UINT8     DimmIndex;

  DimmIndex = Rank >> 2;

  if (IsHbmDimm (Skt, Mc)) {
    DimmType = HbmdimmType;
  } else {
    if (mSadTadScope[Skt].McInfo[Mc].ChInfo[Channel].DimmInfo[DimmIndex].DdrtExist) {
      DimmType = ddrtdimmType;
    } else if (mSadTadScope[Skt].McInfo[Mc].ChInfo[Channel].DimmInfo[DimmIndex].HbmDdr5Exist) {
      DimmType = ddr4dimmType;
    } else {
      DimmType = dimmTypeUnknown;
    }
  }

  return DimmType;
}


/**
 this is for AMAP forward decode.

 @param[in]            Ra   -- rank address
 @param[in]            Socket    --   socket index
 @param[in]            Mc        --   MC index
 @param[in]            Channel   --   Channel index
 @param[in]            DimmIndex --   Dimm index
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- columen

 @ret none

**/
VOID
EFIAPI
ForwardAmapDecode (
  IN         UINT64      Ra,
  IN         UINT8       Socket,
  IN         UINT8       Mc,
  IN         UINT8       Channel,
  IN         UINT8       DimmIndex,
  IN OUT     UINT8      *SubRank,
  IN OUT     UINT8      *Bank,
  IN OUT     UINT32     *Row,
  IN OUT     UINT32     *Column
  )
{
  UINT8       HbmType;
  UINT8       Width;
  UINT8       Density;
  UINT8       ClosePg;
  UINT8       Cgbg;
  UINT8       BankXorEnable;
  UINT8       SidXorEnable;
  UINT8       SidNum;

  HbmType = IsHbmDimm (Socket, Mc);
  Width = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DimmInfo[DimmIndex].Width;
  Density = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DimmInfo[DimmIndex].Density;
  ClosePg = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].ClosePg;
  Cgbg = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].Cgbg;
  BankXorEnable = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].BankXorEnable;
  SidXorEnable = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].SidXorEnable;
  SidNum = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DimmInfo[DimmIndex].Sid;

  DEBUG ((EFI_D_INFO, " \n[ForwardAmapDecode] socket:%d  Mc:%d  Channel:%d Dimm:%d\n", Socket, Mc, Channel, DimmIndex));
  DEBUG ((EFI_D_INFO, " HbmType:%d Width:%d  Density:%d  ClosePg:%d  Cgbg:%d  BankXor:%d SidXor:%d  SidNum:%d\n",
          HbmType, Width, Density, ClosePg, Cgbg, BankXorEnable, SidXorEnable, SidNum));

  ForwardAmap (Ra,
               HbmType,
               Width,
               Density,
               ClosePg,
               Cgbg,
               BankXorEnable,
               SidXorEnable,
               SidNum,
               SubRank,
               Bank,
               Row,
               Column);

}



/**
 this is for get chn address on the normal tad.

 @param[in]            Ra   -- rank address
 @param[in]            Socket    --   socket index
 @param[in]            Mc        --   MC index
 @param[in]            Channel   --   Channel index

 @ret chn address

**/
UINT64
EFIAPI
GetNormalTadChannelAddress (
  IN         UINT64      Sa,
  IN         UINT8       Socket,
  IN         UINT8       Mc,
  IN         UINT8       Channel
  )
{
  UINT8    TadIndex;

  TadIndex = GetChTad (Sa, Socket, Mc);
  DEBUG ((EFI_D_INFO, "[GetNormalTadChannelAddress] ch tad:%d found \n", TadIndex));

  if (TadIndex == 0xff) {
    DEBUG ((EFI_D_ERROR, "wrong Tad index got \n"));
    return EFI_INVALID_PARAMETER;
  }

  ASSERT (TadIndex < MAX_TAD);
  return Forward1lmDecodeGetCa (Sa, Socket, Mc, Channel, TadIndex);
}
