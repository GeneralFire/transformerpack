/** @file
  Implementation of MC reverse decode.

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
#include <Library/BaseMemoryLib.h>
#include "AddressDecodeInternal.h"
#include "Mirror.h"

extern AD_SOCKET_INFO              *mSadTadScope;

/**
  Determines TargetId and ChannelId that maps given system Address
  Modifies the following fields in ADDRESS_TRANSATION structure:
    SocketId
    MemoryControllerId
    ChannelId

 @param  [in/out] pTranslatedAddress   -- pointer to the structure containing DIMM Address

  @retval EFI_SUCCESS                   SAD translation is successful
          EFI_INVALID_PARAMETER         SAD translation is not successful


**/
EFI_STATUS
EFIAPI
TranslateSad (
  IN TRANSLATED_ADDRESS * CONST TranslatedAddress,
  OUT UINT8 *SktId,
  OUT UINT8 *McId,
  OUT UINT8 *ChId
 );

/**
 this is for AMAP reverse decode.

 @param[in, out]       Ra   -- rank address
 @param[in]            Socket    --   socket index
 @param[in]            Mc        --   MC index
 @param[in]            Channel   --   Channel index
 @param[in]            DimmIndex --   Dimm index
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- columen

 @ret TRUE -- AMAP reverse successfully

**/
BOOLEAN
EFIAPI
ReverseAmapDecode (
  IN OUT     UINT64     *Ra,
  IN         UINT8       Socket,
  IN         UINT8       Mc,
  IN         UINT8       Channel,
  IN         UINT8       DimmIndex,
  IN         UINT8       SubRank,
  IN         UINT8       Bank,
  IN         UINT32      Row,
  IN         UINT32      Column
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

  DEBUG ((EFI_D_INFO, " \n[ReverseAmapDecode] socket:%d  Mc:%d  Channel:%d Dimm:%d\n", Socket, Mc, Channel, DimmIndex));
  DEBUG ((EFI_D_INFO, " HbmType:%d Width:%d  Density:%d  ClosePg:%d  Cgbg:%d  BankXor:%d SidXor:%d  SidNum:%d\n",
          HbmType, Width, Density, ClosePg, Cgbg, BankXorEnable, SidXorEnable, SidNum));

  return ReverseAmap (Ra,
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
 This is to calculate Sub channel address from rank address

 @param[in]            Ra        -- rank address
 @param[in]            RirOffset -- rir offset
 @param[in]            RirWay    -- rir ways
 @param[in]            RirIndex  -- rir interleave index
 @param[in]            ClosePg   -- TRUE means it is close page, otherwise open page

SubCa -> RA

         RA = (Sub CA >> PageShift) / RirWays.
         RA  = (RA << PageShift ) | SubCA [0, PageShift)
         RA = RA – RirOffset

RA -> SubCA
         M = RA + RirOffset
         K = M[PageShift, 63)
         SubCA = K << (PageShift + log2(Rirways)) | M [0, PageShift) | (RirIlvIndex << PageShift)



 @ret sub channel address.

**/
UINT64
EFIAPI
ReverseCalcSubCa (
  IN     UINT64     Ra,
  IN     UINT64     RirOffset,
  IN     UINT8      RirWay,
  IN     UINT8      RirIlvIndex,
  IN     UINT8      ClosePg
  )
{
  UINT64    TempRa;
  UINT8     ShiftBit;

  TempRa = Ra + RirOffset;

  if (ClosePg == TRUE) {
    ShiftBit = CLOSE_PG;
  } else {
    ShiftBit = OPEN_PG;
  }

  return LShiftU64(BitFieldRead64 (TempRa, ShiftBit, 63), ShiftBit + HighBitSet64(RirWay)) | BitFieldRead64 (TempRa, 0, ShiftBit - 1) | LShiftU64 ((UINT64)RirIlvIndex, ShiftBit);
}


/**
  This is to calculate channel address from sub channel address.

  @param[in]    SubCa   -- sub channel address
  @param[in]    Sa      -- system address
  @param[in]    PchXorEnable -- pch xor enable
  @param[in]    Rank    --  rank

  @ret          channel address
**/
UINT64
EFIAPI
SubCa2Ca (
  IN     UINT64   SubCa,
  IN     UINT64   Sa,
  IN     UINT8    PchXorEnable,
  IN     UINT8    Rank
  )
{
  UINT8     SubChnBit;

  if (PchXorEnable) {
    SubChnBit = (Rank & 1) ^ ((UINT8)(BitFieldRead64 (Sa, 6, 6) ^ BitFieldRead64 (Sa, 13, 13) ^ BitFieldRead64 (Sa, 21, 21)));
  } else {
    SubChnBit = Rank & 1;
  }

  return BitFieldRead64(SubCa, 0, 5) | LShiftU64(BitFieldRead64 (SubCa, 6, 63), 7) |  LShiftU64(SubChnBit & 1 , 6);
}

/**
 This is for RIR reverse decode for HBM

 @param[in, out]       Ca        --   channenl address
 @param[in]            Skt       --   socket index
 @param[in]            Mc        --   MC index
 @param[in]            Channel   --   Channel index
 @param[in]            Rank      --   Rank index
 @param[in]            Ra        --   rank address form HBM/DDR5

 @ret none

**/
VOID
EFIAPI
ReverseRirDecodeHbm (
  IN  OUT     UINT64       *Ca,
  IN          UINT8         Socket,
  IN          UINT8         Mc,
  IN          UINT8         Channel,
  IN          UINT8         Rank,
  IN          UINT64        SubRa
  )
{
  UINT8     PchXorEnable;

  UINT64    TempCa;
  UINT8     SidNum;
  UINT8     ClosePg;

  TempCa = SubRa;
  PchXorEnable = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].PchXorEnable;
  //
  //SID bit is rank[1]
  //
  SidNum = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DimmInfo[0].Sid;  //assume HBM ia at DIMM 0.

  if (SidNum != 0) {
    ClosePg = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].ClosePg;

    if (ClosePg == 1) {
      TempCa = BitFieldInsert64 (TempCa, 10 - 1, 10 - 1, BitFieldRead8(Rank, 1, 1));  // - 1 due to PCH bit is not insert
    } else {
      TempCa = BitFieldInsert64 (TempCa, 14 - 1, 14 - 1, BitFieldRead8(Rank, 1, 1));  // - 1 due to PCH bit is not insert
    }
  }

  //
  //assume PCH bit is 0, and insert it. We will  recalculate it once got SA.
  //
  TempCa = BitFieldInsert64 (TempCa, 6, 6, 0);

  if (Ca != NULL) {
    *Ca = TempCa;
  }

}

/**
 this is for RIR reverse decode for DDR5

 @param[in, out]       Ca   -- channenl address
 @param[in]            Skt    --   socket index
 @param[in]            Mc        --   MC index
 @param[in]            Channel   --   Channel index
 @param[in]            Rank      --   Rank index
 @param[in]            Ra        --   rank address form HBM/DDR5

 @ret      TRUE -- reverse decode successfully.

**/
BOOLEAN
EFIAPI
ReverseRirDecodeDdr5 (
  IN  OUT     UINT64       *Ca,
  IN          UINT8         Socket,
  IN          UINT8         Mc,
  IN          UINT8         Channel,
  IN          UINT8         Rank,
  IN          UINT64        Ra
  )
{
  UINT8           PotentialChipSelect[8];
  UINT8           RirIndex;
  UINT8           RirWay;
  UINT8           TempRank;
  UINT8           Index;
  UINT64          RirOffset;
  UINT8           RankId;
  BOOLEAN         FoundRirOffset;
  BOOLEAN         FoundSubCa = FALSE;
  UINT64          SubCa;
  UINT64          Sa;
  UINT8           RirIlvIndex;
  UINT8           RankXorEnable;
  UINT8           PchXorEnable;
  UINT8           DimmXorEnable;
  UINT8           ClosePg;


  ClosePg = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].ClosePg;
  RankXorEnable = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RankXorEnable;
  DimmXorEnable = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DimmXorEnable;
  PchXorEnable = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].PchXorEnable;

  //
  //at current point, not able to know system address, hence need to put all potential chip select here together.
  //
  TempRank = Rank & 0xfe; //clear sub channel bit.



  for (Index = 0; Index < 8; Index ++) {
    PotentialChipSelect[Index] = 0xff;
  }

  Index = 0;
  PotentialChipSelect[Index++] = TempRank;

  if (RankXorEnable) {
    PotentialChipSelect[Index++] = TempRank ^ 2;
  }

  if (DimmXorEnable) {
    PotentialChipSelect[Index++] = TempRank ^ 4;
  }

  if ((RankXorEnable != 0)  &&  (DimmXorEnable != 0)) {
    PotentialChipSelect[Index++] = TempRank ^ 6;
  }


  Index = 0;
  while (PotentialChipSelect[Index] != 0xff) {
    DEBUG ((EFI_D_INFO, "potencial rank:%d \n", PotentialChipSelect[Index]));

    if (FoundSubCa) {
      break;
    }
    for (RirIndex = 0; RirIndex < MAX_RIR; RirIndex ++) {
      if (mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RirInfo[RirIndex].RirValid == FALSE) {
        break;
      }

      RirWay = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RirInfo[RirIndex].RirWay;

      FoundRirOffset = FALSE;
      for (RirIlvIndex = 0; RirIlvIndex < RirWay; RirIlvIndex ++) {
        if ( mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RirInfo[RirIndex].RirIlv[RirIlvIndex].RirIlvValid == FALSE) {
          break;
        }
        RirOffset = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RirInfo[RirIndex].RirIlv[RirIlvIndex].RirOffset;
        RankId = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RirInfo[RirIndex].RirIlv[RirIlvIndex].RankId;
        DEBUG ((EFI_D_INFO, "RirIndex:%d   RirIlvIndex:%d   Rir offset:%x  RankId:%d \n", RirIndex, RirIlvIndex, RirOffset, RankId));
        //
        // check whether the chip select matches.
        //
        if (PotentialChipSelect[Index] == RankId) {
          DEBUG ((EFI_D_INFO, "rir interleave index:%d is found \n", RirIlvIndex));
          FoundRirOffset = TRUE;
          break;
        }
      }

      if (FoundRirOffset == FALSE) {
        continue;
      }

      SubCa = ReverseCalcSubCa (Ra, RirOffset, RirWay, RirIlvIndex, ClosePg);
      DEBUG ((EFI_D_INFO, "Get Sub channel address:%lx \n",SubCa));
      //
      // check this target SubCa locates the range
      //
      if ((SubCa < mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RirInfo[RirIndex].RirBase) ||
         (SubCa > mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RirInfo[RirIndex].RirLimit)) {
        DEBUG ((EFI_D_INFO, "Sub channel address:%lx  Rirbase:%lx  RirLimit:%lx \n", SubCa, mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RirInfo[RirIndex].RirBase,
                  mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RirInfo[RirIndex].RirLimit));
        continue;
      }

      if ((RankXorEnable == 0) && (DimmXorEnable == 0)) {
        DEBUG ((EFI_D_INFO, "Get Sub channel address:%lx found!\n",SubCa));
        FoundSubCa = TRUE;
        break;
      } else {
        Sa = ReverseTadDecode (Socket, Mc, Channel, Rank, BitFieldInsert64(SubCa, 6, 6, 0));
        if (Sa == (UINT64) -1) {
          DEBUG ((EFI_D_INFO, "Cannot find a system address with SubCa %lx\n", SubCa));
          continue;
        }
        DEBUG ((EFI_D_INFO, "Calculate system address:%lx\n",Sa));

        TempRank = PotentialChipSelect[Index];

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

        if (TempRank == (Rank & 0x6)) {
          DEBUG ((EFI_D_INFO, "Get Sub channel address:%lx found with xor!\n",SubCa));
          FoundSubCa = TRUE;
          break;
        }
      }
    }

    if (FoundSubCa) {
      break;
    }
    Index ++;
  }

  if (Ca != NULL) {
    if (FoundSubCa) {
      //
      //assume PCH bit value is 0, and calculate it later.
      //
      *Ca = BitFieldInsert64(SubCa, 6, 6, 0);
      return TRUE;
    }
  }

  return FALSE;
}


/**
 this is for RIR reverse decode for DDRT

 @param[in, out]       Ca   -- channenl address
 @param[in]            Skt    --   socket index
 @param[in]            Mc        --   MC index
 @param[in]            Channel   --   Channel index
 @param[in]            Rank      --   Rank index
 @param[in]            DPA        --   DPA for DDRT

 @ret      TRUE -- reverse decode successfully.

**/
BOOLEAN
EFIAPI
ReverseRirDecodeDcpmm (
  IN  OUT     UINT64       *Ca,
  IN          UINT8         Socket,
  IN          UINT8         Mc,
  IN          UINT8         Channel,
  IN          UINT8         Rank,
  IN          UINT64        Ra
  )
{
  UINT8           RirIndex;
  UINT8           RirWay;
  UINT8           TempRank;

  UINT64          RirOffset;
  UINT8           RankId;
  BOOLEAN         FoundRirOffset;
  BOOLEAN         FoundSubCa = FALSE;
  UINT64          SubCa;

  UINT8           RirIlvIndex;
  UINT8           RankXorEnable;
  UINT8           PchXorEnable;
  UINT8           DimmXorEnable;
  UINT8           ClosePg;


  ClosePg = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].ClosePg;
  RankXorEnable = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RankXorEnable;
  DimmXorEnable = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DimmXorEnable;
  PchXorEnable = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].PchXorEnable;

  //
  //at current point, not able to know system address, hence need to put all potential chip select here together.
  //
  TempRank = Rank ;

  for (RirIndex = 0; RirIndex < MAX_RIR; RirIndex ++) {
    if (mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DdrtRirInfo[RirIndex].RirValid == FALSE) {
      break;
    }

    RirWay = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DdrtRirInfo[RirIndex].RirWay;

    FoundRirOffset = FALSE;
    for (RirIlvIndex = 0; RirIlvIndex < RirWay; RirIlvIndex ++) {
      if ( mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DdrtRirInfo[RirIndex].RirIlv[RirIlvIndex].RirIlvValid == FALSE) {
        break;
      }
      RirOffset = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DdrtRirInfo[RirIndex].RirIlv[RirIlvIndex].RirOffset;
      RankId = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DdrtRirInfo[RirIndex].RirIlv[RirIlvIndex].RankId;
      DEBUG ((EFI_D_INFO, "RirIndex:%d   RirIlvIndex:%d   Rir offset:%x  RankId:%d \n", RirIndex, RirIlvIndex, RirOffset, RankId));
      //
      // check whether the chip select matches.
      //
      if (TempRank == RankId) {
        DEBUG ((EFI_D_INFO, "rir interleave index:%d is found \n", RirIndex));
        FoundRirOffset = TRUE;
        break;
      }
    }

    if (FoundRirOffset == FALSE) {
      continue;
    }

    SubCa = ReverseCalcSubCa (Ra, RirOffset, RirWay, RirIlvIndex, ClosePg);
    DEBUG ((EFI_D_INFO, "Get Sub channel address:%lx \n",SubCa));
    //
    // check this target SubCa locates the range
    //
    if ((SubCa < mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DdrtRirInfo[RirIndex].RirBase) ||
         (SubCa > mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].DdrtRirInfo[RirIndex].RirLimit)) {
      DEBUG ((EFI_D_INFO, "Sub channel address:%lx  Rirbase:%lx  RirLimit:%lx \n", SubCa, mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RirInfo[RirIndex].RirBase,
                  mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].RirInfo[RirIndex].RirLimit));
      continue;
    }


    DEBUG ((EFI_D_INFO, "Get Sub channel address:%lx found!\n",SubCa));
    FoundSubCa = TRUE;
    break;

  }


  if (Ca != NULL) {
    if (FoundSubCa) {

      *Ca = SubCa;
      return TRUE;
    }
  }

  return FALSE;
}

/**
 this is for RIR reverse decode for all memory type.

 @param[in, out]       Ca   -- channenl address
 @param[in]            Dimmtype -- dimm type
 @param[in]            Skt    --   socket index
 @param[in]            Mc        --   MC index
 @param[in]            Channel   --   Channel index
 @param[in]            Rank      --   Rank index
 @param[in]            Ra        --   rank address form HBM/DDR5
 @param[in]            Dpa       --   Dpa address for DCPMM.

 @ret none

**/
BOOLEAN
EFIAPI
ReverseRirDecode (
  IN  OUT     UINT64       *Ca,
  IN          UINT8         DimmType,
  IN          UINT8         Skt,
  IN          UINT8         Mc,
  IN          UINT8         Channel,
  IN          UINT8         Rank,
  IN          UINT64        Ra,
  IN          UINT64        Dpa
  )
{
  if (DimmType == ddr4dimmType) {
    return ReverseRirDecodeDdr5 (Ca, Skt, Mc, Channel, Rank, Ra);
  }

  if (DimmType == HbmdimmType) {
    ReverseRirDecodeHbm (Ca, Skt, Mc, Channel, Rank, Ra);
  }

  if (DimmType == ddrtdimmType) {
    return ReverseRirDecodeDcpmm (Ca, Skt, Mc, Channel, Rank, Dpa);
  }

  return TRUE;
}


/**
  This routine is to know whether the CA belong to NM TAD.

  @param [socket]   -- socket index
  @param [Mc]       -- memory controller index
  @param [Channel]  -- channel index per Mc
  @param [Ca]       -- channel address

  @ret   TRUE --  NM TAD. FALSE -- normal TAD.

 **/
BOOLEAN
EFIAPI
IsTargetAtNmTad (
  IN     UINT8   Socket,
  IN     UINT8   Mc,
  IN     UINT8   Channel,
  IN     UINT64  Ca
  )
{

  if (mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmTadValid) {
    //
    //when 1LM+2LM, the channel address map is NM + 1LM (cache is at LOW address), this is what mmemory map setting.
    //
    if(Ca < mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmIndexEndAddr) {
      return TRUE;
    } else {
      return FALSE;
    }
  } else {
    return FALSE;
  }
}


/**
  This routine is to reverse NM TAD. Assume Tag is 0.

  @param [socket]   -- socket index
  @param [Mc]       -- memory controller index
  @param [Channel]  -- channel index per Mc
  @param [rank]     -- rank index
  @param [Ca]       -- channel address

  @ret   System address.

 **/
UINT64
EFIAPI
ReverseNmTad (
  IN     UINT8   Socket,
  IN     UINT8   Mc,
  IN     UINT8   Channel,
  IN     UINT8   Rank,
  IN     UINT64  Ca
  )
{
  UINT8     NmTargetWay;
  UINT8     NmChnWay;
  UINT8     NmTargetGran;
  UINT8     NmChnGran;
  UINT64    NmCachingOffset;
  UINT8     NmChnCap;
  UINT8     NmRatioChnCap;
  UINT8     PotentialTarget;
  UINT8     PotentialChn;
  UINT8     PotencialPch;
  UINT64    TempSa;
  UINT8     PchXorEnable;
  UINT8     TargetSkt;
  UINT8     TargetMc;
  UINT8     TargetChn;
  TRANSLATED_ADDRESS  TargetAddr;
  BOOLEAN   FoundFlag = FALSE;

  NmTargetWay = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmTargetWay;
  NmChnWay = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmChnWay;
  NmTargetGran = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmTargetGran;
  NmChnGran = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmChnGran;
  NmCachingOffset = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmCachingOffset;
  NmChnCap = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmChnCap;
  NmRatioChnCap = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].NmRatioChnCap;
  PchXorEnable = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].PchXorEnable;

  //
  //As NM TAD forward drop bits for NmTargetWays/NmChnWays, we can try all these data
  //

  for (PotentialTarget = 0; PotentialTarget <  NmTargetWay; PotentialTarget ++) {
    if (FoundFlag) {
      break;
    }
    for (PotentialChn = 0; PotentialChn <  NmChnWay; PotentialChn ++ ) {
      if (FoundFlag) {
        break;
      }
      for (PotencialPch = 0; PotencialPch < 2; PotencialPch ++) {
        TempSa = Ca;

        //
        //update PCH bit.
        //
        TempSa = BitFieldWrite64 (TempSa, 6, 6, PotencialPch);

        //
        //from spr register, there is no 3-way/6-way interval on NM TAD. Hence we just insert target/ch. Otherwise need to do multiple operation.
        //in case same granularity, target at high bits.
        //
        if (NmTargetGran > NmChnWay) {

          if (NmChnWay != 1) {
            TempSa = BitFieldInsert64 (TempSa, NmChnGran, NmChnGran + HighBitSet64 (NmChnWay) - 1, PotentialChn);
          }

          if (NmTargetWay != 1) {
            TempSa = BitFieldInsert64 (TempSa, NmTargetGran, NmTargetGran + HighBitSet64 (NmTargetWay) - 1, PotentialTarget);
          }
        } else {
          if (NmTargetWay != 1) {
            TempSa = BitFieldInsert64 (TempSa, NmTargetGran, NmTargetGran + HighBitSet64 (NmTargetWay) - 1, PotentialTarget);
          }

          if (NmChnWay != 1) {
            TempSa = BitFieldInsert64 (TempSa, NmChnGran, NmChnGran + HighBitSet64 (NmChnWay) - 1, PotentialChn);
          }
        }

        TempSa += NmCachingOffset;

        if (CalcPch (TempSa, PchXorEnable) != (Rank & 1)) {
          continue;
        }

        ZeroMem (&TargetAddr, sizeof (TRANSLATED_ADDRESS));
        TargetAddr.SystemAddress = TempSa;
        if (TranslateSad (&TargetAddr, &TargetSkt, &TargetMc, &TargetChn) != EFI_SUCCESS) {
          DEBUG ((EFI_D_INFO, " forward sad decode result fail\n"));
          continue;
        }

        if ((TargetSkt != Socket) || (TargetMc != Mc) || (TargetChn != Channel)) {
          continue;
        } else {
          FoundFlag = TRUE;
          break;
        }
      }
    }
  }

  if (FoundFlag) {
    return TempSa;
  } else {
    return (UINT64) -1;
  }
}


/**
  This routine is to reverse normal TAD.

  @param [socket]   -- socket index
  @param [Mc]       -- memory controller index
  @param [Channel]  -- channel index per Mc
  @param [rank]     -- rank index
  @param [Ca]       -- channel address

  @ret   system address.

 **/
UINT64
EFIAPI
ReverseNormalTad (
  IN     UINT8   Socket,
  IN     UINT8   Mc,
  IN     UINT8   Channel,
  IN     UINT8   Rank,
  IN     UINT64  Ca
  )
{
  UINT8    TargetWay;
  UINT8    TargetGran;
  UINT8    ChWay;
  UINT8    ChGran;
  UINT64   TadOffset;
  UINT8    TadSign;
  UINT8     PotentialTarget;
  UINT8     potentialChn;
  UINT8     potencialPch;
  UINT64    TempSa;
  UINT8     PchXorEnable;
  UINT8     TargetSkt;
  UINT8     TargetMc;
  UINT8     TargetChn;
  TRANSLATED_ADDRESS  TargetAddr;
  BOOLEAN   FoundFlag = FALSE;
  UINT8     Sad2TadIndex;
  UINT8     TadIndex;
  UINT8     DimmType;
  BOOLEAN   BlockRegionFlag;
  UINT8     FmMc;
  UINT8     FmChn;
  BOOLEAN   PriSecBit;
  UINT64    SubCa;
  UINT64    TempCa;
  UINT64    UpdatedCa;
  UINT8     SecChn;
  UINT8     PriChn;

  PchXorEnable = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].PchXorEnable;

  for (Sad2TadIndex = 0; Sad2TadIndex < MAX_TAD; Sad2TadIndex ++) {
    if (FoundFlag) {
      break;
    }

    if (mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].Sad2TadValid == FALSE) {
      break;
    }

    //
    // block region
    //
    if (IsLocateBlockMode (Socket, Mc, Sad2TadIndex)) {
      BlockRegionFlag = TRUE;
    } else {
      BlockRegionFlag = FALSE;
    }

    //
    // Update channel address if mirror
    //
    SecChn = 0xff;
    PriChn = Channel;
    UpdatedCa = Ca;
    PriSecBit = 0;
    if (mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].Mirror) {
      SubCa = BitFieldRemove64 (Ca, 6, 6);
      PriSecBit = GetMirrorBitFromCa (Ca);
      SubCa = MirrorSubCa2SubCa (SubCa);

      UpdatedCa = BitFieldInsert64 (SubCa, 6, 6, (UINT32) BitFieldRead64(Ca, 6, 6)); //although PCH has not been determined yet, we still use the bit from Ca
      DEBUG ((EFI_D_INFO, "ReverseNormalTad: mirror support, remove BIT18 from SubCa, SubCa 0x%lx, Ca 0x%lx, updatedCa %lx\n", SubCa, Ca, UpdatedCa));

      if (PriSecBit) {
        //
        // it's secondary channel, to find the primary channel
        //
        SecChn = Channel;
        PriChn = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].PriCh;
        DEBUG ((EFI_D_ERROR, "ReverseNormalTad: mirror support, the input is secondary channel, change to primary one. %d->%d\n", SecChn, PriChn));
      } else {
        PriChn = Channel;
        SecChn = mSadTadScope[Socket].McInfo[Mc].ChInfo[Channel].SecCh;
      }
    }

    TadIndex = mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].ChTad;

    TadOffset = mSadTadScope[Socket].McInfo[Mc].ChInfo[PriChn].TadInfo[TadIndex].TadOffset;
    TargetWay = mSadTadScope[Socket].McInfo[Mc].ChInfo[PriChn].TadInfo[TadIndex].TargetWay;
    TargetGran = mSadTadScope[Socket].McInfo[Mc].ChInfo[PriChn].TadInfo[TadIndex].TargetGran;
    ChWay =  mSadTadScope[Socket].McInfo[Mc].ChInfo[PriChn].TadInfo[TadIndex].ChWay;
    ChGran = mSadTadScope[Socket].McInfo[Mc].ChInfo[PriChn].TadInfo[TadIndex].ChGran;
    TadSign = mSadTadScope[Socket].McInfo[Mc].ChInfo[PriChn].TadInfo[TadIndex].TadSign;

    for (PotentialTarget = 0; PotentialTarget <  TargetWay; PotentialTarget ++) {
      if (FoundFlag) {
        break;
      }
      for (potentialChn = 0; potentialChn <  ChWay; potentialChn ++ ) {
        if (FoundFlag) {
          break;
        }
        //
        // Dont try secondary channel in mirror mode when reverse
        //
        if (mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].Mirror) {
          if (SecChn == potentialChn) {
            continue;
          }
        }
        for (potencialPch = 0; potencialPch < 2; potencialPch ++) {
          TempSa = UpdatedCa;

          DimmType = GetDimmType (Socket, Mc, PriChn, Rank);

          //
          //Update PCH bit for DDR5/HBM.
          //
          if (DimmType != ddrtdimmType) {
            TempSa = BitFieldWrite64 (TempSa, 6, 6, potencialPch);
          }

          //
          //In case the same granularity for target and chn, higher bit is channel.
          //
          if (TargetGran >= ChGran) {
            if (ChWay != 1) {
              //
              //((CA[ChGran, 63] x ChWay )  + potentialChn ) << ChGran + CA[0, ChGran -1]
              //
              TempSa = LShiftU64((MultU64x32 (BitFieldRead64 (TempSa, ChGran, 63), ChWay) + potentialChn), ChGran) | BitFieldRead64 (TempSa, 0, ChGran - 1); //SPR reg has 3-way CH.
            }

            if (TargetWay != 1) {
              TempSa = BitFieldInsert64 (TempSa, TargetGran, TargetGran + HighBitSet64 (TargetWay) - 1, PotentialTarget);  //SPR silicon don't have 3-way/6-way.
            }
          } else {
            if (TargetWay != 1) {
              TempSa = BitFieldInsert64 (TempSa, TargetGran, TargetGran + HighBitSet64 (TargetWay) - 1, PotentialTarget);  //SPR silicon don't have 3-way/6-way.
            }

            if (ChWay != 1) {
              //
              //((CA[ChGran, 63] x ChWay )  + potentialChn ) << ChGran + CA[0, ChGran -1]
              //
              TempSa = LShiftU64((MultU64x32 (BitFieldRead64 (TempSa, ChGran, 63), ChWay) + potentialChn), ChGran) | BitFieldRead64 (TempSa, 0, ChGran - 1); //SPR reg has 3-way CH.
            }
          }

          if (TadSign == 1) {
            TempSa -= TadOffset;
          } else {
            TempSa += TadOffset;
          }
          DEBUG ((EFI_D_INFO, "Try Sad2TadIndex:%d  TadIndex:%d  potentialTarget:%d  potentialChn:%d   pch:%d TempSa:%lx\n",
                 Sad2TadIndex, TadIndex, PotentialTarget,potentialChn, potencialPch, TempSa));
          if (DimmType != ddrtdimmType) {
            if (CalcPch (TempSa, PchXorEnable) != (Rank & 1)) {
              continue;
            }
          }

          if (IsM2mTadRange (TempSa, Socket, Mc, Sad2TadIndex) == FALSE) {
            continue;
          }


          if (GetChTad (TempSa, Socket, Mc) != TadIndex) {
            continue;
          }

          if (BlockRegionFlag == FALSE) {
            ZeroMem (&TargetAddr, sizeof (TRANSLATED_ADDRESS));
            TargetAddr.SystemAddress = TempSa;
            if (TranslateSad (&TargetAddr, &TargetSkt, &TargetMc, &TargetChn) != EFI_SUCCESS) {
              DEBUG ((EFI_D_INFO, " forward sad decode result fail\n"));
              continue;
            }

            //
            //in case got NM mc/chn, get FM mc/chn
            //
            if ((TargetAddr.Flag & (NM_MC_VALID | NM_CHN_VALID)) == (NM_MC_VALID | NM_CHN_VALID)) {
              if (GetFmInfoFromNmChn (TempSa, Socket, TargetMc, TargetChn, &FmMc, &FmChn )) {
                TargetMc  = FmMc;
                TargetChn = FmChn;
              }
            }
          } else {
            if (ForwardDecodeBlockAddress (TempSa, &TargetSkt, &TargetMc, &TargetChn) == FALSE) {
              DEBUG ((EFI_D_INFO, " block decode result fail\n"));
              continue;
            }
          }

          if ((TargetSkt != Socket) || (TargetMc != Mc) || (TargetChn != PriChn)) {
            DEBUG ((EFI_D_INFO, " forward decode result doesn't match customer input\n"));
            continue;
          }

          //
          //bit 6 of input CA is not determined.
          //
          TempCa = GetNormalTadChannelAddress (TempSa, TargetSkt, TargetMc, TargetChn);
          if (mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].Mirror) {
            TempCa = MirrorCa2Ca (TempCa);
          }
          if ((TempCa & (~BIT6)) != (UpdatedCa & (~BIT6))) {
            DEBUG ((EFI_D_INFO, "forward chn address is not match to reverse, TempCa 0x%lx, UpdatedCa 0x%lx\n", TempCa, UpdatedCa));
            continue;
          } else {
            FoundFlag = TRUE;
            DEBUG ((EFI_D_INFO, " forward decode result matches customer input\n"));
            break;
          }
        }
      }
    }
  }

  if (FoundFlag) {
    return TempSa;
  } else {
    return (UINT64) -1;
  }
}

/**
  This routine is to do revserse decode from CA to SA.

  @param [socket]   -- socket index
  @param [Mc]       -- memory controller index
  @param [Channel]  -- channel index per Mc
  @param [rank]     -- rank index
  @param [Ca]       -- channel address

  @ret   system address.

 **/
UINT64
EFIAPI
ReverseTadDecode (
  IN     UINT8   Socket,
  IN     UINT8   Mc,
  IN     UINT8   Channel,
  IN     UINT8   Rank,
  IN     UINT64  Ca
  )
{

  if (IsTargetAtNmTad (Socket, Mc, Channel, Ca)  &&
      (GetDimmType (Socket, Mc, Channel, Rank) != ddrtdimmType)) {
    return ReverseNmTad (Socket, Mc, Channel, Rank, Ca);
  } else {
    return ReverseNormalTad (Socket, Mc, Channel, Rank, Ca);
  }
}
