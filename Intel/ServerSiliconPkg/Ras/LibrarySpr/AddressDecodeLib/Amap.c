/** @file
  Implementation of Amap decode.

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
#include "AddressDecodeInternal.h"



/**
 this is routine is to do bank xor.

 @param [in]     Bank  -- bank with bank group. [BG2, BG1, BG0, BA1, BA0]
 @param [in]     BankXor  -- bank xor seed. [BG2, BA1, BA0, BG1, BG0]

 @ret   target bank with bank group
**/
UINT8
EFIAPI
DoBankXor (
  IN     UINT8      Bank,
  IN     UINT8      BankXor
  )
{
  UINT8    BankSeed;

  BankSeed = BitFieldRead8 (BankXor, 0, 1) << 2 |  BitFieldRead8 (BankXor, 4, 4) << 4 | BitFieldRead8 (BankXor, 2, 3);


  return Bank ^ BankSeed;
}


/**
 this is rank decode for DDR5 x8 (16/24/32/64Gb) close page mode.

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
Ddr5ClosePgMostX8 (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT8              SubRankTemp = 0xff;
  UINT8              SidXor;
  UINT32             RowTemp;

  //
  //RA6~8 - BG0~2  RA9~10 - BA0~1
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 6, 8), 2) | BitFieldRead64 (Ra, 9, 10));
  if (BankXorEnable == TRUE) {
    BankTemp = DoBankXor(BankTemp, (UINT8)BitFieldRead64(Ra, 26, 30));  // RA26~30
  }
  if (Bank != NULL) {
    *Bank = BankTemp;
  }

  if (SidNum != 0) {
    //
    //RA17 - SID0  RA15 - SID1  RA16 - SID2
    //
    if (SidNum <= 1) {
      SubRankTemp = (UINT8)BitFieldRead64 (Ra, 17, 17);
    }

    if (SidNum <= 2) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 15, 15) << 1;
    }

    if (SidNum <= 3) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 16, 16) << 2;
    }

    if (SidXorEnable == TRUE) {
      //
      //RA31 - SID XOR 0  RA32 - SID XOR 1   RA33 - SID XOR 2
      //
      SidXor = (UINT8)BitFieldRead64 (Ra, 31, 31 + SidNum - 1);
      SubRankTemp = SubRankTemp ^ SidXor;
    }

  }


  if (SubRank != NULL) {
    *SubRank = SubRankTemp;
  }

  //
  //RA17 - R0  RA15 - R1 RA16-R2 RA20~23- R3~6  RA11~13-R7~9 RA18-R10 RA28-R11 RA30-R12 RA29-R13 RA31~34 - R14~17
  //
  RowTemp = (UINT32)(BitFieldRead64 (Ra, 17, 17) | LShiftU64(BitFieldRead64 (Ra, 15, 15), 1) | LShiftU64(BitFieldRead64 (Ra, 16, 16), 2)
              | LShiftU64(BitFieldRead64 (Ra, 20, 23), 3) |  LShiftU64(BitFieldRead64 (Ra, 11, 13),7) | LShiftU64(BitFieldRead64 (Ra, 18, 18), 10)
              | LShiftU64(BitFieldRead64 (Ra, 28, 28), 11) | LShiftU64(BitFieldRead64 (Ra, 30, 30),12) |LShiftU64(BitFieldRead64 (Ra, 29, 29),13)
              | LShiftU64(BitFieldRead64 (Ra, 31, 34),14));

  if (SidNum != 0) {
    //
    //RA31 - R0 RA32-R1 RA33-R2
    //
    RowTemp = BitFieldRead32 (RowTemp, SidNum, 13) << SidNum;
    RowTemp |= (UINT32)BitFieldRead64 (Ra, 31, 31 + SidNum -1);
    RowTemp |= (UINT32)(LShiftU64 (BitFieldRead64 (Ra, 31 + SidNum,  31 + SidNum + 3 + 5),14));
  }

  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA2~5-C0~3  RA14-C4 RA19-C5 RA24~27 - C6~9
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 2, 5) | LShiftU64(BitFieldRead64 (Ra, 14, 14),4) |
              LShiftU64(BitFieldRead64 (Ra, 19, 19), 5) | LShiftU64(BitFieldRead64 (Ra, 24, 27), 6));
  }
}


/**
 this is rank decode for DDR5 x8 (16/24/32/64Gb) open page Cgbg_int

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
Ddr5OpenPgMostX8Int (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT8              SubRankTemp = 0xff;
  UINT8              SidXor;
  UINT32             RowTemp;

  //
  //RA12 - BG0   RA16~17-BG1~2  RA18~19 - BA0~1
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 12, 12), 2) | LShiftU64 (BitFieldRead64 (Ra, 16, 17), 3) | BitFieldRead64 (Ra, 18, 19));
  if (BankXorEnable == TRUE) {
    BankTemp = DoBankXor(BankTemp, (UINT8)BitFieldRead64(Ra, 26, 30));  // RA26~30
  }
  if (Bank != NULL) {
    *Bank = BankTemp;
  }

  if (SidNum != 0) {
    //
    //RA14 - SID0  RA15 - SID1  RA13 - SID2
    //
    if (SidNum <= 1) {
      SubRankTemp = (UINT8)BitFieldRead64 (Ra, 14, 14);
    }

    if (SidNum <= 2) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 15, 15) << 1;
    }

    if (SidNum <= 3) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 13, 13) << 2;
    }

    if (SidXorEnable == TRUE) {
      //
      //RA31 - SID XOR 0  RA32 - SID XOR 1   RA33 - SID XOR 2
      //
      SidXor = (UINT8)BitFieldRead64 (Ra, 31, 31 + SidNum - 1);
      SubRankTemp = SubRankTemp ^ SidXor;
    }
  }

  if (SubRank != NULL) {
    *SubRank = SubRankTemp;
  }

  //
  //RA14 - R0  RA15 - R1 RA13-R2  RA20~28 - R3~11   RA30 - R12 RA29 - R13   RA31~34 -R14~17
  //
  RowTemp = (UINT32)(BitFieldRead64 (Ra, 14, 14) | LShiftU64(BitFieldRead64 (Ra, 15, 15), 1) | LShiftU64(BitFieldRead64 (Ra, 13, 13), 2)
            | LShiftU64(BitFieldRead64 (Ra, 20, 28), 3) | LShiftU64(BitFieldRead64 (Ra, 30, 30), 12) | LShiftU64(BitFieldRead64 (Ra, 29, 29),13)
            | LShiftU64(BitFieldRead64 (Ra, 31, 34), 14));

  if (SidNum != 0) {
    //
    //RA31 - R0 RA32-R1 RA33-R2
    //
    RowTemp = BitFieldRead32 (RowTemp, SidNum, 13) << SidNum;
    RowTemp |= (UINT32)BitFieldRead64 (Ra, 31, 31 + SidNum -1);
    RowTemp |= (UINT32)(LShiftU64 (BitFieldRead64 (Ra, 31 + SidNum,  31 + SidNum + 3 + 5),14));
  }

  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA2~5-C0~3  RA7~11-C4~8  RA6 - C9
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 2, 5) | LShiftU64(BitFieldRead64 (Ra, 7, 11),4) |
              LShiftU64(BitFieldRead64 (Ra, 6, 6), 9));
  }
}



/**
 this is rank decode for DDR5 x8 (16/24/32/64Gb) open page INT2

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
Ddr5OpenPgMostX8Int2 (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT8              SubRankTemp = 0xff;
  UINT8              SidXor;
  UINT32             RowTemp;

  //
  //RA7 - BG0   RA16~17-BG1~2  RA18~19 - BA0~1
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 7, 7), 2) | LShiftU64 (BitFieldRead64 (Ra, 16, 17), 3) | BitFieldRead64 (Ra, 18, 19));
  if (BankXorEnable == TRUE) {
    BankTemp = DoBankXor(BankTemp, (UINT8)BitFieldRead64(Ra, 26, 30));  // RA26~30
  }
  if (Bank != NULL) {
    *Bank = BankTemp;
  }

  if (SidNum != 0) {
    //
    //RA14 - SID0  RA15 - SID1  RA13 - SID2
    //
    if (SidNum <= 1) {
      SubRankTemp = (UINT8)BitFieldRead64 (Ra, 14, 14);
    }

    if (SidNum <= 2) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 15, 15) << 1;
    }

    if (SidNum <= 3) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 13, 13) << 2;
    }

    if (SidXorEnable == TRUE) {
      //
      //RA31 - SID XOR 0  RA32 - SID XOR 1   RA33 - SID XOR 2
      //
      SidXor = (UINT8)BitFieldRead64 (Ra, 31, 31 + SidNum - 1);
      SubRankTemp = SubRankTemp ^ SidXor;
    }

  }

  if (SubRank != NULL) {
    *SubRank = SubRankTemp;
  }

  //
  //RA14 - R0  RA15 - R1 RA13-R2  RA20~28 - R3~11  RA30 - R12  RA29 - R13     RA31~34 - R14~17
  //
  RowTemp = (UINT32)(BitFieldRead64 (Ra, 14, 14) | LShiftU64(BitFieldRead64 (Ra, 15, 15), 1) | LShiftU64(BitFieldRead64 (Ra, 13, 13), 2)
            | LShiftU64(BitFieldRead64 (Ra, 20, 28), 3) | LShiftU64(BitFieldRead64 (Ra, 29, 29),13) | LShiftU64(BitFieldRead64 (Ra, 30, 30), 12)
            | LShiftU64(BitFieldRead64 (Ra, 31, 34), 14));

  if (SidNum != 0) {
    //
    //RA31 - R0 RA32-R1 RA33-R2
    //
    RowTemp = BitFieldRead32 (RowTemp, SidNum, 13) << SidNum;
    RowTemp |= (UINT32)BitFieldRead64 (Ra, 31, 31 + SidNum -1);
    RowTemp |= (UINT32)(LShiftU64 (BitFieldRead64 (Ra, 31 + SidNum,  31 + SidNum + 3 + 5), 14));
  }

  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA2~6-C0~4  RA8~12-C5~9
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 2, 6) | LShiftU64(BitFieldRead64 (Ra, 8, 12),5));
  }
}



/**
 this is rank decode for DDR5 x8 (16/24/32/64Gb) open page INT8

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
Ddr5OpenPgMostX8Int8 (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT8              SubRankTemp = 0xff;
  UINT8              SidXor;
  UINT32             RowTemp;

  //
  //RA6~8 - BG0~2     RA18~19 - BA0~1
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 6, 8), 2) | BitFieldRead64 (Ra, 18, 19));
  if (BankXorEnable == TRUE) {
    BankTemp = DoBankXor(BankTemp, (UINT8)BitFieldRead64(Ra, 26, 30));  // RA26~30
  }
  if (Bank != NULL) {
    *Bank = BankTemp;
  }

  if (SidNum != 0) {
    //
    //RA17 - SID0  RA15 - SID1  RA16 - SID2
    //
    if (SidNum <= 1) {
      SubRankTemp = (UINT8)BitFieldRead64 (Ra, 17, 17);
    }

    if (SidNum <= 2) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 15, 15) << 1;
    }

    if (SidNum <= 3) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 16, 16) << 2;
    }

    if (SidXorEnable == TRUE) {
      //
      //RA31 - SID XOR 0  RA32 - SID XOR 1   RA33 - SID XOR 2
      //
      SidXor = (UINT8)BitFieldRead64 (Ra, 31, 31 + SidNum - 1);
      SubRankTemp = SubRankTemp ^ SidXor;
    }

  }

  if (SubRank != NULL) {
    *SubRank = SubRankTemp;
  }

  //
  //RA17 - R0  RA15 - R1 RA16-R2  RA20~28 - R3~11    RA30 - R12  RA29 - R13  RA31~34 - R14~17
  //
  RowTemp = (UINT32)(BitFieldRead64 (Ra, 17, 17) | LShiftU64(BitFieldRead64 (Ra, 15, 15), 1) | LShiftU64(BitFieldRead64 (Ra, 16, 16), 2)
            | LShiftU64(BitFieldRead64 (Ra, 20, 28), 3) | LShiftU64(BitFieldRead64 (Ra, 30, 30), 12) | LShiftU64(BitFieldRead64 (Ra, 29, 29),13)
            | LShiftU64(BitFieldRead64 (Ra, 31, 34), 14));

  if (SidNum != 0) {
    //
    //RA31 - R0 RA32-R1 RA33-R2
    //
    RowTemp = BitFieldRead32 (RowTemp, SidNum, 13) << SidNum;
    RowTemp |= (UINT32)BitFieldRead64 (Ra, 31, 31 + SidNum -1);
    RowTemp |= (UINT32)(LShiftU64 (BitFieldRead64 (Ra, 31 + SidNum,  31 + SidNum + 3 + 5),14));
  }

  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA2~5-C0~3  RA14-C4  RA13-C5  RA9~12-C6~9
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 2, 5) | LShiftU64(BitFieldRead64 (Ra, 14, 14),4)
               | LShiftU64(BitFieldRead64 (Ra, 13, 13),5)  | LShiftU64(BitFieldRead64 (Ra, 9, 12),6));
  }
}



/**
 this is rank decode for DDR5 x8 8Gb close page mode.

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
Ddr5ClosePg8GbX8 (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT8              SubRankTemp = 0xff;
  UINT8              SidXor;
  UINT32             RowTemp;

  //
  //RA6~8 - BG0~2   RA9 - BA0
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 6, 8), 2) | BitFieldRead64 (Ra, 9, 9));
  if (BankXorEnable == TRUE) {
    BankTemp = DoBankXor(BankTemp, (UINT8)BitFieldRead64(Ra, 26, 30));  // RA26~30
    BankTemp &= ~BIT1;
  }
  if (Bank != NULL) {
    *Bank = BankTemp;
  }

  if (SidNum != 0) {
    //
    //RA17 - SID0  RA15 - SID1  RA16 - SID2
    //
    if (SidNum <= 1) {
      SubRankTemp = (UINT8)BitFieldRead64 (Ra, 17, 17);
    }

    if (SidNum <= 2) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 15, 15) << 1;
    }

    if (SidNum <= 3) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 16, 16) << 2;
    }

    if (SidXorEnable == TRUE) {
      //
      //RA31 - SID XOR 0  RA32 - SID XOR 1   RA33 - SID XOR 2
      //
      SidXor = (UINT8)BitFieldRead64 (Ra, 31, 31 + SidNum - 1);
      SubRankTemp = SubRankTemp ^ SidXor;
    }

  }



  if (SubRank != NULL) {
    *SubRank = SubRankTemp;
  }

  //
  //RA17 - R0  RA15 - R1 RA16-R2 RA20~23- R3~6  RA11~13-R7~9 RA18-R10 RA28-R11 RA10-R12 RA29~31-R13~15  <MAX R15>
  //
  RowTemp = (UINT32)(BitFieldRead64 (Ra, 17, 17) | LShiftU64(BitFieldRead64 (Ra, 15, 15), 1) | LShiftU64(BitFieldRead64 (Ra, 16, 16), 2)
                   | LShiftU64(BitFieldRead64 (Ra, 20, 23), 3) | LShiftU64(BitFieldRead64 (Ra, 11, 13),7) | LShiftU64(BitFieldRead64 (Ra, 18, 18), 10)
                   | LShiftU64(BitFieldRead64 (Ra, 28, 28), 11) | LShiftU64(BitFieldRead64 (Ra, 10, 10),12) | LShiftU64(BitFieldRead64 (Ra, 29, 31),13));

  if (SidNum != 0) {
    //
    //RA31 - R0 RA32-R1 RA33-R2
    //
    RowTemp = BitFieldRead32 (RowTemp, SidNum, 14) << SidNum;
    RowTemp |= (UINT32)BitFieldRead64 (Ra, 31, 31 + SidNum -1);
    RowTemp |= (UINT32)(LShiftU64 (BitFieldRead64 (Ra, 31 + SidNum,  31 + SidNum),15));  //since R15
  }

  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA2~5-C0~3  RA14-C4 RA19-C5 RA24~27 - C6~9
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 2, 5) | LShiftU64(BitFieldRead64 (Ra, 14, 14),4) |
              LShiftU64(BitFieldRead64 (Ra, 19, 19), 5) | LShiftU64(BitFieldRead64 (Ra, 24, 27), 6));
  }
}



/**
 this is rank decode for DDR5 x8 8Gb open page mode - int.

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
Ddr5OpenPg8GbX8Int (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT8              SubRankTemp = 0xff;
  UINT8              SidXor;
  UINT32             RowTemp;

  //
  //RA12 - BG0   RA16~17-BG1~2  RA18 - BA0
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 12, 12), 2) | LShiftU64 (BitFieldRead64 (Ra, 16, 17), 3) | BitFieldRead64 (Ra, 18, 18));
  if (BankXorEnable == TRUE) {
    BankTemp = DoBankXor(BankTemp, (UINT8)BitFieldRead64(Ra, 26, 30));  // RA26~30
    BankTemp &= ~BIT1;
  }
  if (Bank != NULL) {
    *Bank = BankTemp;
  }

  if (SidNum != 0) {
    //
    //RA14 - SID0  RA15 - SID1  RA13 - SID2
    //
    if (SidNum <= 1) {
      SubRankTemp = (UINT8)BitFieldRead64 (Ra, 14, 14);
    }

    if (SidNum <= 2) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 15, 15) << 1;
    }

    if (SidNum <= 3) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 13, 13) << 2;
    }

    if (SidXorEnable == TRUE) {
      //
      //RA31 - SID XOR 0  RA32 - SID XOR 1   RA33 - SID XOR 2
      //
      SidXor = (UINT8)BitFieldRead64 (Ra, 31, 31 + SidNum - 1);
      SubRankTemp = SubRankTemp ^ SidXor;
    }

  }



  if (SubRank != NULL) {
    *SubRank = SubRankTemp;
  }

  //
  //RA14 - R0  RA15 - R1 RA13-R2  RA20~28 - R3~11  RA19-R12  RA29~31 - R13~15
  //
  RowTemp = (UINT32)(BitFieldRead64 (Ra, 14, 14) | LShiftU64(BitFieldRead64 (Ra, 15, 15), 1) | LShiftU64(BitFieldRead64 (Ra, 13, 13), 2)
            | LShiftU64(BitFieldRead64 (Ra, 20, 28), 3) | LShiftU64(BitFieldRead64 (Ra, 19, 19), 12) | LShiftU64(BitFieldRead64 (Ra, 29, 31),13));

  if (SidNum != 0) {
    //
    //RA31 - R0 RA32-R1 RA33-R2
    //
    RowTemp = BitFieldRead32 (RowTemp, SidNum, 14) << SidNum;  //to r14
    RowTemp |= (UINT32)BitFieldRead64 (Ra, 31, 31 + SidNum -1);
    RowTemp |= (UINT32)(LShiftU64 (BitFieldRead64 (Ra, 31 + SidNum,  31 + SidNum),15)); //since R15
  }

  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA2~5-C0~3  RA7~11-C4~8  RA6 - C9
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 2, 5) | LShiftU64(BitFieldRead64 (Ra, 7, 11),4)
             | LShiftU64(BitFieldRead64 (Ra, 6, 6), 9));
  }
}



/**
 this is rank decode for DDR5 x8 8Gb open page INT2

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
Ddr5OpenPg8GbX8Int2 (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT8              SubRankTemp = 0xff;
  UINT8              SidXor;
  UINT32             RowTemp;

  //
  //RA7 - BG0   RA16~17-BG1~2  RA18 - BA0
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 7, 7), 2) | LShiftU64 (BitFieldRead64 (Ra, 16, 17), 3) | BitFieldRead64 (Ra, 18, 18));
  if (BankXorEnable == TRUE) {
    BankTemp = DoBankXor(BankTemp, (UINT8)BitFieldRead64(Ra, 26, 30));  // RA26~30
    BankTemp &= ~BIT1;
  }
  if (Bank != NULL) {
    *Bank = BankTemp;

  }

  if (SidNum != 0) {
    //
    //RA14 - SID0  RA15 - SID1  RA13 - SID2
    //
    if (SidNum <= 1) {
      SubRankTemp = (UINT8)BitFieldRead64 (Ra, 14, 14);
    }

    if (SidNum <= 2) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 15, 15) << 1;
    }

    if (SidNum <= 3) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 13, 13) << 2;
    }

    if (SidXorEnable == TRUE) {
      //
      //RA31 - SID XOR 0  RA32 - SID XOR 1   RA33 - SID XOR 2
      //
      SidXor = (UINT8)BitFieldRead64 (Ra, 31, 31 + SidNum - 1);
      SubRankTemp = SubRankTemp ^ SidXor;
    }

  }



  if (SubRank != NULL) {
    *SubRank = SubRankTemp;
  }

  //
  //RA14 - R0  RA15 - R1 RA13-R2  RA20~28 - R3~11  RA19-R12  RA29~31 - R13~15
  //
  RowTemp = (UINT32)(BitFieldRead64 (Ra, 14, 14) | LShiftU64(BitFieldRead64 (Ra, 15, 15), 1) | LShiftU64(BitFieldRead64 (Ra, 13, 13), 2)
            | LShiftU64(BitFieldRead64 (Ra, 20, 28), 3) | LShiftU64(BitFieldRead64 (Ra, 19, 19), 12) | LShiftU64(BitFieldRead64 (Ra, 29, 31),13));

  if (SidNum != 0) {
    //
    //RA31 - R0 RA32-R1 RA33-R2
    //
    RowTemp = BitFieldRead32 (RowTemp, SidNum, 14) << SidNum; //TO R14
    RowTemp |= (UINT32)BitFieldRead64 (Ra, 31, 31 + SidNum -1);
    RowTemp |= (UINT32)(LShiftU64 (BitFieldRead64 (Ra, 31 + SidNum,  31 + SidNum),15)); //SINCE R15
  }

  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA2~6-C0~4  RA8~12-C5~9
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 2, 6) | LShiftU64(BitFieldRead64 (Ra, 8, 12), 5));
  }
}




/**
 this is rank decode for DDR5 x8 8Gb open page INT8

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
Ddr5OpenPg8GbX8Int8 (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT8              SubRankTemp = 0xff;
  UINT8              SidXor;
  UINT32             RowTemp;

  //
  //RA6~8 - BG0~2     RA18 - BA0
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 6, 8), 2) | BitFieldRead64 (Ra, 18, 18));
  if (BankXorEnable == TRUE) {
    BankTemp = DoBankXor(BankTemp, (UINT8)BitFieldRead64(Ra, 26, 30));  // RA26~30
    BankTemp &= ~BIT1;
  }
  if (Bank != NULL) {
    *Bank = BankTemp;
  }

  if (SidNum != 0) {
    //
    //RA17 - SID0  RA15 - SID1  RA16 - SID2
    //
    if (SidNum <= 1) {
      SubRankTemp = (UINT8)BitFieldRead64 (Ra, 17, 17);
    }

    if (SidNum <= 2) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 15, 15) << 1;
    }

    if (SidNum <= 3) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 16, 16) << 2;
    }

    if (SidXorEnable == TRUE) {
      //
      //RA31 - SID XOR 0  RA32 - SID XOR 1   RA33 - SID XOR 2
      //
      SidXor = (UINT8)BitFieldRead64 (Ra, 31, 31 + SidNum - 1);
      SubRankTemp = SubRankTemp ^ SidXor;
    }

  }



  if (SubRank != NULL) {
    *SubRank = SubRankTemp;
  }

  //
  //RA17 - R0  RA15 - R1 RA16-R2  RA20~28 - R3~11   RA19-R12  RA29~31 - R13~15
  //
  RowTemp = (UINT32)(BitFieldRead64 (Ra, 17, 17) | LShiftU64(BitFieldRead64 (Ra, 15, 15), 1) | LShiftU64(BitFieldRead64 (Ra, 16, 16), 2)
            | LShiftU64(BitFieldRead64 (Ra, 20, 28), 3) | LShiftU64(BitFieldRead64 (Ra, 19, 19), 12)| LShiftU64(BitFieldRead64 (Ra, 29, 31),13));

  if (SidNum != 0) {
    //
    //RA31 - R0 RA32-R1 RA33-R2
    //
    RowTemp = BitFieldRead32 (RowTemp, SidNum, 14) << SidNum;  //TO R14
    RowTemp |= (UINT32)BitFieldRead64 (Ra, 31, 31 + SidNum -1);
    RowTemp |= (UINT32)(LShiftU64 (BitFieldRead64 (Ra, 31 + SidNum,  31 + SidNum),15));  //SINCE R15
  }

  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA2~5-C0~3  RA14-C4  RA13-C5  RA9~12-C6~9
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 2, 5) | LShiftU64(BitFieldRead64 (Ra, 14, 14),4)
               | LShiftU64(BitFieldRead64 (Ra, 13, 13),5)  | LShiftU64(BitFieldRead64 (Ra, 9, 12),6));
  }
}



/**
 this is rank decode for DDR5 x4(16/24/32/64Gb) close page mode.

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none
 
**/
VOID
EFIAPI
Ddr5ClosePgMostX4 (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT8              SubRankTemp = 0xff;
  UINT8              SidXor;
  UINT32             RowTemp;

  //
  //RA6~8 - BG0~2  RA9~10 - BA0~1
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 6, 8), 2) | BitFieldRead64 (Ra, 9, 10));
  if (BankXorEnable == TRUE) {
    BankTemp = DoBankXor(BankTemp, (UINT8)BitFieldRead64(Ra, 26, 30));  // RA26~30
  }
  if (Bank != NULL) {
    *Bank = BankTemp;
  }

  if (SidNum != 0) {
    //
    //RA17 - SID0  RA15 - SID1  RA16 - SID2
    //
    if (SidNum <= 1) {
      SubRankTemp = (UINT8)BitFieldRead64 (Ra, 17, 17);
    }

    if (SidNum <= 2) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 15, 15) << 1;
    }

    if (SidNum <= 3) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 16, 16) << 2;
    }

      if (SidXorEnable == TRUE) {
      //
      //RA32 - SID XOR 0  RA33 - SID XOR 1   RA34 - SID XOR 2
      //
      SidXor = (UINT8)BitFieldRead64 (Ra, 32, 32 + SidNum - 1);
      SubRankTemp = SubRankTemp ^ SidXor;
      }

  }



  if (SubRank != NULL) {
    *SubRank = SubRankTemp;
  }

  //
  //RA17 - R0  RA15 - R1 RA16-R2  RA31- R3  RA21~23- R4~6  RA11~13-R7~9 RA18-R10 RA28-R11 RA30-R12 RA29-R13 RA32~35 - R14~17
  //
  RowTemp = (UINT32)(BitFieldRead64 (Ra, 17, 17) | LShiftU64(BitFieldRead64 (Ra, 15, 15), 1) | LShiftU64(BitFieldRead64 (Ra, 16, 16), 2)
       | LShiftU64(BitFieldRead64 (Ra, 31, 31), 3) | LShiftU64(BitFieldRead64 (Ra, 21, 23), 4) | LShiftU64(BitFieldRead64 (Ra, 11, 13),7)
       | LShiftU64(BitFieldRead64 (Ra, 18, 18), 10) | LShiftU64(BitFieldRead64 (Ra, 28, 28), 11)
       | LShiftU64(BitFieldRead64 (Ra, 30, 30),12) | LShiftU64(BitFieldRead64 (Ra, 29, 29),13) | LShiftU64(BitFieldRead64 (Ra, 32, 35),14));

  if (SidNum != 0) {
    //
    //RA32 - R0 RA33-R1 RA34-R2
    //
    RowTemp = BitFieldRead32 (RowTemp, SidNum, 13) << SidNum;  //TO R13
    RowTemp |= (UINT32)BitFieldRead64 (Ra, 32, 32 + SidNum -1);
    RowTemp |= (UINT32)(LShiftU64 (BitFieldRead64 (Ra, 32 + SidNum,  32 + SidNum + 3 +4),14));  //SINCE R14
  }

  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA2~5-C0~3  RA14-C4 RA19-C5 RA24~27 - C6~9   RA20-C10
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 2, 5) | LShiftU64(BitFieldRead64 (Ra, 14, 14),4) |
              LShiftU64(BitFieldRead64 (Ra, 19, 19), 5) | LShiftU64(BitFieldRead64 (Ra, 24, 27), 6) | LShiftU64(BitFieldRead64 (Ra, 20, 20),10));
  }
}




/**
 this is rank decode for DDR5 x4 (16/24/32/64Gb) open page Cgbg_int

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
Ddr5OpenPgMostX4Int (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT8              SubRankTemp = 0xff;
  UINT8              SidXor;
  UINT32             RowTemp;

  //
  //RA12 - BG0   RA16~17-BG1~2  RA18~19 - BA0~1
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 12, 12), 2) | LShiftU64 (BitFieldRead64 (Ra, 16, 17), 3) | BitFieldRead64 (Ra, 18, 19));
  if (BankXorEnable == TRUE) {
    BankTemp = DoBankXor(BankTemp, (UINT8)BitFieldRead64(Ra, 26, 30));  // RA26~30
  }
  if (Bank != NULL) {
    *Bank = BankTemp;
  }

  if (SidNum != 0) {
    //
    //RA14 - SID0  RA15 - SID1  RA13 - SID2
    //
    if (SidNum <= 1) {
      SubRankTemp = (UINT8)BitFieldRead64 (Ra, 14, 14);
    }

    if (SidNum <= 2) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 15, 15) << 1;
    }

    if (SidNum <= 3) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 13, 13) << 2;
    }

    if (SidXorEnable == TRUE) {
      //
      //RA32 - SID XOR 0  RA33 - SID XOR 1   RA34 - SID XOR 2
      //
      SidXor = (UINT8)BitFieldRead64 (Ra, 32, 32 + SidNum - 1);
      SubRankTemp = SubRankTemp ^ SidXor;
    }

  }



  if (SubRank != NULL) {
    *SubRank = SubRankTemp;
  }

  //
  //RA14 - R0  RA15 - R1 RA13-R2  RA31-R3 RA21~28 - R4~11  RA30 - R12 RA29 - R13   RA32~35 - R14~17
  //
  RowTemp = (UINT32)(BitFieldRead64 (Ra, 14, 14) | LShiftU64(BitFieldRead64 (Ra, 15, 15), 1) | LShiftU64(BitFieldRead64 (Ra, 13, 13), 2)
           | LShiftU64(BitFieldRead64 (Ra, 31, 31), 3)  | LShiftU64(BitFieldRead64 (Ra, 21, 28), 4) |  LShiftU64(BitFieldRead64 (Ra, 30, 30), 12)
           | LShiftU64(BitFieldRead64 (Ra, 29, 29),13)  | LShiftU64(BitFieldRead64 (Ra, 32, 35),14));

  if (SidNum != 0) {
    //
    //RA32 - R0 RA33-R1 RA34-R2
    //
    RowTemp = BitFieldRead32 (RowTemp, SidNum, 13) << SidNum;
    RowTemp |= (UINT32)BitFieldRead64 (Ra, 32, 32 + SidNum -1);
    RowTemp |= (UINT32)(LShiftU64 (BitFieldRead64 (Ra, 32 + SidNum,  32 + SidNum + 3 + 4),14));
  }

  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA2~5-C0~3   RA7~11-C4~8  RA6 - C9    RA20-C10
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 2, 5) | LShiftU64(BitFieldRead64 (Ra, 7, 11),4)
              | LShiftU64(BitFieldRead64 (Ra, 6, 6), 9) | LShiftU64(BitFieldRead64 (Ra, 20, 20),10));
  }
}





/**
 this is rank decode for DDR5 x4 (16/24/32/64Gb) open page INT2

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
Ddr5OpenPgMostX4Int2 (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT8              SubRankTemp = 0xff;
  UINT8              SidXor;
  UINT32             RowTemp;

  //
  //RA7 - BG0   RA16~17-BG1~2  RA18~19 - BA0~1
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 7, 7), 2) | LShiftU64 (BitFieldRead64 (Ra, 16, 17), 3) | BitFieldRead64 (Ra, 18, 19));
  if (BankXorEnable == TRUE) {
    BankTemp = DoBankXor(BankTemp, (UINT8)BitFieldRead64(Ra, 26, 30));  // RA26~30
  }
  if (Bank != NULL) {
    *Bank = BankTemp;
  }

  if (SidNum != 0) {
    //
    //RA14 - SID0  RA15 - SID1  RA13 - SID2
    //
    if (SidNum <= 1) {
      SubRankTemp = (UINT8)BitFieldRead64 (Ra, 14, 14);
    }

    if (SidNum <= 2) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 15, 15) << 1;
    }

    if (SidNum <= 3) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 13, 13) << 2;
    }

    if (SidXorEnable == TRUE) {
      //
      //RA32 - SID XOR 0  RA33 - SID XOR 1   RA34 - SID XOR 2
      //
      SidXor = (UINT8)BitFieldRead64 (Ra, 32, 32 + SidNum - 1);
      SubRankTemp = SubRankTemp ^ SidXor;
    }

  }



  if (SubRank != NULL) {
    *SubRank = SubRankTemp;
  }

  //
  //RA14 - R0  RA15 - R1 RA13-R2  RA31-R3   RA21~28 - R4~11   RA30 - R12   RA29 - R13  R32~35 - R14~17
  //
  RowTemp = (UINT32)(BitFieldRead64 (Ra, 14, 14) | LShiftU64(BitFieldRead64 (Ra, 15, 15), 1) | LShiftU64(BitFieldRead64 (Ra, 13, 13), 2)
            | LShiftU64(BitFieldRead64 (Ra, 31, 31), 3) | LShiftU64(BitFieldRead64 (Ra, 21, 28), 4) | LShiftU64(BitFieldRead64 (Ra, 30, 30), 12)
            | LShiftU64(BitFieldRead64 (Ra, 29, 29),13) | LShiftU64(BitFieldRead64 (Ra, 32, 35),14));

  if (SidNum != 0) {
    //
    //RA32 - R0 RA33-R1 RA34-R2
    //
    RowTemp = BitFieldRead32 (RowTemp, SidNum, 13) << SidNum; //TO R13
    RowTemp |= (UINT32)BitFieldRead64 (Ra, 32, 32 + SidNum -1);
    RowTemp |= (UINT32)(LShiftU64 (BitFieldRead64 (Ra, 32 + SidNum,  32 + SidNum + 3 + 4),14));  //SINCE R14
  }

  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA2~6-C0~4  RA8~12-C5~9   R20-C10
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 2, 6) | LShiftU64(BitFieldRead64 (Ra, 8, 12),5)  | LShiftU64(BitFieldRead64 (Ra, 20, 20),10) );
  }
}





/**
 this is rank decode for DDR5 x4 (16/24/32/64Gb) open page INT8

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
Ddr5OpenPgMostX4Int8 (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT8              SubRankTemp = 0xff;
  UINT8              SidXor;
  UINT32             RowTemp;

  //
  //RA6~8 - BG0~2     RA18~19 - BA0~1
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 6, 8), 2) | BitFieldRead64 (Ra, 18, 19));
  if (BankXorEnable == TRUE) {
    BankTemp = DoBankXor(BankTemp, (UINT8)BitFieldRead64(Ra, 26, 30));  // RA26~30
  }
  if (Bank != NULL) {
    *Bank = BankTemp;
  }

  if (SidNum != 0) {
    //
    //RA17 - SID0  RA15 - SID1  RA16 - SID2
    //
    if (SidNum <= 1) {
      SubRankTemp = (UINT8)BitFieldRead64 (Ra, 17, 17);
    }

    if (SidNum <= 2) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 15, 15) << 1;
    }

    if (SidNum <= 3) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 16, 16) << 2;
    }

    if (SidXorEnable == TRUE) {
      //
      //RA32 - SID XOR 0  RA33 - SID XOR 1   RA34 - SID XOR 2
      //
      SidXor = (UINT8)BitFieldRead64 (Ra, 32, 32 + SidNum - 1);
      SubRankTemp = SubRankTemp ^ SidXor;
    }

  }



  if (SubRank != NULL) {
    *SubRank = SubRankTemp;
  }

  //
  //RA17 - R0  RA15 - R1 RA16-R2  RA31 - R3   RA21~28 - R4~11   RA30 - R12  RA29 - R13  R32~35 - R14~17
  //
  RowTemp = (UINT32)(BitFieldRead64 (Ra, 17, 17) | LShiftU64(BitFieldRead64 (Ra, 15, 15), 1) | LShiftU64(BitFieldRead64 (Ra, 16, 16), 2)
             | LShiftU64(BitFieldRead64 (Ra, 31, 31), 3) | LShiftU64(BitFieldRead64 (Ra, 21, 28), 4)  | LShiftU64(BitFieldRead64 (Ra, 30, 30), 12)
             | LShiftU64(BitFieldRead64 (Ra, 29, 29),13) | LShiftU64(BitFieldRead64 (Ra, 32, 35),14));

  if (SidNum != 0) {
    //
    //RA31 - R0 RA32-R1 RA33-R2
    //
    RowTemp = BitFieldRead32 (RowTemp, SidNum, 13) << SidNum;  //TO R13
    RowTemp |= (UINT32)BitFieldRead64 (Ra, 32, 32 + SidNum -1);
    RowTemp |= (UINT32)(LShiftU64 (BitFieldRead64 (Ra, 32 + SidNum,  32 + SidNum + 3 + 4),14)); //SINCE R14
  }

  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA2~5-C0~3  RA14-C4  RA13-C5  RA9~12-C6~9  R20-C10
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 2, 5) | LShiftU64(BitFieldRead64 (Ra, 14, 14),4)
               | LShiftU64(BitFieldRead64 (Ra, 13, 13),5)  | LShiftU64(BitFieldRead64 (Ra, 9, 12),6) | LShiftU64(BitFieldRead64 (Ra, 20, 20),10));
  }
}




/**
 this is rank decode for DDR5 x4 8Gb close page mode.

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
Ddr5ClosePg8GbX4 (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT8              SubRankTemp = 0xff;
  UINT8              SidXor;
  UINT32             RowTemp;

  //
  //RA6~8 - BG0~2   RA9 - BA0
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 6, 8), 2) | BitFieldRead64 (Ra, 9, 9));
  if (BankXorEnable == TRUE) {
    BankTemp = DoBankXor(BankTemp, (UINT8)BitFieldRead64(Ra, 26, 30));  // RA26~30
    BankTemp &= ~BIT1;
  }
  if (Bank != NULL) {
    *Bank = BankTemp;
  }

  if (SidNum != 0) {
    //
    //RA17 - SID0  RA15 - SID1  RA16 - SID2
    //
    if (SidNum <= 1) {
      SubRankTemp = (UINT8)BitFieldRead64 (Ra, 17, 17);
    }

    if (SidNum <= 2) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 15, 15) << 1;
    }

    if (SidNum <= 3) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 16, 16) << 2;
    }

    if (SidXorEnable == TRUE) {
      //
      //RA32 - SID XOR 0  RA33 - SID XOR 1   RA34 - SID XOR 2
      //
      SidXor = (UINT8)BitFieldRead64 (Ra, 32, 32 + SidNum - 1);
      SubRankTemp = SubRankTemp ^ SidXor;
    }

  }



  if (SubRank != NULL) {
    *SubRank = SubRankTemp;
  }

  //
  //RA17 - R0  RA15 - R1 RA16-R2   RA31-R3 RA21~23- R4~6  RA11~13-R7~9   RA18-R10   RA28-R11   RA10-R12   RA29~30-R13~14  RA32-R15  <MAX R15>
  //
  RowTemp = (UINT32)(BitFieldRead64 (Ra, 17, 17) | LShiftU64(BitFieldRead64 (Ra, 15, 15), 1) | LShiftU64(BitFieldRead64 (Ra, 16, 16), 2)
           | LShiftU64(BitFieldRead64 (Ra, 31, 31), 3) | LShiftU64(BitFieldRead64 (Ra, 21, 23), 4) |   LShiftU64(BitFieldRead64 (Ra, 11, 13),7)
           | LShiftU64(BitFieldRead64 (Ra, 18, 18), 10) | LShiftU64(BitFieldRead64 (Ra, 28, 28), 11) | LShiftU64(BitFieldRead64 (Ra, 10, 10),12) |
            LShiftU64(BitFieldRead64 (Ra, 29, 30),13) | LShiftU64(BitFieldRead64 (Ra, 32, 32),15));

  if (SidNum != 0) {
    //
    //RA32 - R0 RA33-R1 RA34-R2
    //
    RowTemp = BitFieldRead32 (RowTemp, SidNum, 14) << SidNum; //TO R14
    RowTemp |= (UINT32)BitFieldRead64 (Ra, 32, 32 + SidNum -1);
    RowTemp |= (UINT32)(LShiftU64 (BitFieldRead64 (Ra, 32 + SidNum,  32 + SidNum),15));  //since R15
  }

  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA2~5-C0~3  RA14-C4 RA19-C5 RA24~27 - C6~9  RA20-C10
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 2, 5) | LShiftU64(BitFieldRead64 (Ra, 14, 14),4) | LShiftU64(BitFieldRead64 (Ra, 19, 19), 5)
                  | LShiftU64(BitFieldRead64 (Ra, 24, 27), 6) | LShiftU64(BitFieldRead64 (Ra, 20, 20),10));
  }
}



/**
 this is rank decode for DDR5 x4 8Gb open page mode - int.

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
Ddr5OpenPg8GbX4Int (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT8              SubRankTemp = 0xff;
  UINT8              SidXor;
  UINT32             RowTemp;

  //
  //RA12 - BG0   RA16~17-BG1~2  RA18 - BA0
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 12, 12), 2) | LShiftU64 (BitFieldRead64 (Ra, 16, 17), 3) | BitFieldRead64 (Ra, 18, 18));
  if (BankXorEnable == TRUE) {
    BankTemp = DoBankXor(BankTemp, (UINT8)BitFieldRead64(Ra, 26, 30));  // RA26~30
    BankTemp &= ~BIT1;
  }
  if (Bank != NULL) {
    *Bank = BankTemp;
  }

  if (SidNum != 0) {
    //
    //RA14 - SID0  RA15 - SID1  RA13 - SID2
    //
    if (SidNum <= 1) {
      SubRankTemp = (UINT8)BitFieldRead64 (Ra, 14, 14);
    }

    if (SidNum <= 2) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 15, 15) << 1;
    }

    if (SidNum <= 3) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 13, 13) << 2;
    }

    if (SidXorEnable == TRUE) {
      //
      //RA32 - SID XOR 0  RA33 - SID XOR 1   RA34 - SID XOR 2
      //
      SidXor = (UINT8)BitFieldRead64 (Ra, 32, 32 + SidNum - 1);
      SubRankTemp = SubRankTemp ^ SidXor;
    }
  }



  if (SubRank != NULL) {
    *SubRank = SubRankTemp;
  }

  //
  //RA14 - R0  RA15 - R1 RA13-R2   RA31-R3  RA21~28 - R4~11  RA19-R12  RA29~30-R13~14  RA32-R15
  //
  RowTemp = (UINT32)(BitFieldRead64 (Ra, 14, 14) | LShiftU64(BitFieldRead64 (Ra, 15, 15), 1) | LShiftU64(BitFieldRead64 (Ra, 13, 13), 2)
             | LShiftU64(BitFieldRead64 (Ra, 31, 31), 3) | LShiftU64(BitFieldRead64 (Ra, 21, 28), 4) | LShiftU64(BitFieldRead64 (Ra, 19, 19), 12)
             | LShiftU64(BitFieldRead64 (Ra, 29, 30),13) | LShiftU64(BitFieldRead64 (Ra, 32, 32), 15));

  if (SidNum != 0) {
    //
    //RA32 - R0 RA33-R1 RA34-R2
    //
    RowTemp = BitFieldRead32 (RowTemp, SidNum, 14) << SidNum;  //to r14
    RowTemp |= (UINT32)BitFieldRead64 (Ra, 32, 32 + SidNum -1);
    RowTemp |= (UINT32)(LShiftU64 (BitFieldRead64 (Ra, 32 + SidNum,  32 + SidNum),15)); //since R15
  }

  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA2~5-C0~3 RA7~11-C4~8  RA6 - C9    R20-C10
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 2, 5) | LShiftU64(BitFieldRead64 (Ra, 7, 11),4)
             | LShiftU64(BitFieldRead64 (Ra, 6, 6), 9) | LShiftU64(BitFieldRead64 (Ra, 20, 20), 10));
  }
}



/**
 this is rank decode for DDR5 x4 8Gb open page INT2

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
Ddr5OpenPg8GbX4Int2 (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT8              SubRankTemp = 0xff;
  UINT8              SidXor;
  UINT32             RowTemp;

  //
  //RA7 - BG0   RA16~17-BG1~2  RA18 - BA0
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 7, 7), 2) | LShiftU64 (BitFieldRead64 (Ra, 16, 17), 3) | BitFieldRead64 (Ra, 18, 18));
  if (BankXorEnable == TRUE) {
    BankTemp = DoBankXor(BankTemp, (UINT8)BitFieldRead64(Ra, 26, 30));  // RA26~30
    BankTemp &= ~BIT1;
  }
  if (Bank != NULL) {
    *Bank = BankTemp;
  }

  if (SidNum != 0) {
    //
    //RA14 - SID0  RA15 - SID1  RA13 - SID2
    //
    if (SidNum <= 1) {
      SubRankTemp = (UINT8)BitFieldRead64 (Ra, 14, 14);
    }

    if (SidNum <= 2) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 15, 15) << 1;
    }

    if (SidNum <= 3) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 13, 13) << 2;
    }

    if (SidXorEnable == TRUE) {
      //
      //RA32 - SID XOR 0  RA33 - SID XOR 1   RA34 - SID XOR 2
      //
      SidXor = (UINT8)BitFieldRead64 (Ra, 32, 32 + SidNum - 1);
      SubRankTemp = SubRankTemp ^ SidXor;
    }

  }


  if (SubRank != NULL) {
    *SubRank = SubRankTemp;
  }

  //
  //RA14 - R0  RA15 - R1 RA13-R2  RA31-R3   RA21~28 - R4~11  RA19-R12  RA29~30-R13~14  RA32-R15
  //
  RowTemp = (UINT32)(BitFieldRead64 (Ra, 14, 14) | LShiftU64(BitFieldRead64 (Ra, 15, 15), 1) | LShiftU64(BitFieldRead64 (Ra, 13, 13), 2)
            | LShiftU64(BitFieldRead64 (Ra, 31, 31), 3) | LShiftU64(BitFieldRead64 (Ra, 21, 28), 4) | LShiftU64(BitFieldRead64 (Ra, 19, 19), 12)
            | LShiftU64(BitFieldRead64 (Ra, 29, 30),13) | LShiftU64(BitFieldRead64 (Ra, 32, 32), 15) );

  if (SidNum != 0) {
    //
    //RA32 - R0 RA33-R1 RA34-R2
    //
    RowTemp = BitFieldRead32 (RowTemp, SidNum, 14) << SidNum; //TO R14
    RowTemp |= (UINT32)BitFieldRead64 (Ra, 32, 32 + SidNum -1);
    RowTemp |= (UINT32)(LShiftU64 (BitFieldRead64 (Ra, 32 + SidNum,  32 + SidNum),15)); //SINCE R15
  }

  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA2~6-C0~4  RA8~12-C5~9  RA20-C10
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 2, 6) | LShiftU64(BitFieldRead64 (Ra, 8, 12),5) | LShiftU64(BitFieldRead64 (Ra, 20, 20),10));
  }
}




/**
 this is rank decode for DDR5 x4 8Gb open page INT8

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
Ddr5OpenPg8GbX4Int8 (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT8              SubRankTemp = 0xff;
  UINT8              SidXor;
  UINT32             RowTemp;

  //
  //RA6~8 - BG0~2     RA18 - BA0
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 6, 8), 2) | BitFieldRead64 (Ra, 18, 18));
  if (BankXorEnable == TRUE) {
    BankTemp = DoBankXor(BankTemp, (UINT8)BitFieldRead64(Ra, 26, 30));  // RA26~30
    BankTemp &= ~BIT1;
  }
  if (Bank != NULL) {
    *Bank = BankTemp;
  }

  if (SidNum != 0) {
    //
    //RA17 - SID0  RA15 - SID1  RA16 - SID2
    //
    if (SidNum <= 1) {
      SubRankTemp = (UINT8)BitFieldRead64 (Ra, 17, 17);
    }

    if (SidNum <= 2) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 15, 15) << 1;
    }

    if (SidNum <= 3) {
      SubRankTemp = SubRankTemp | (UINT8)BitFieldRead64 (Ra, 16, 16) << 2;
    }
    if (SidXorEnable == TRUE) {
      //
      //RA32 - SID XOR 0  RA33 - SID XOR 1   RA34 - SID XOR 2
      //
      SidXor = (UINT8)BitFieldRead64 (Ra, 32, 32 + SidNum - 1);
      SubRankTemp = SubRankTemp ^ SidXor;
    }
  }



  if (SubRank != NULL) {
    *SubRank = SubRankTemp;
  }

  //
  //RA17 - R0  RA15 - R1 RA16-R2   RA31 - R3 RA21~28 - R4~11   RA19-R12  RA29~30-R13~14  RA32-R15
  //
  RowTemp = (UINT32)(BitFieldRead64 (Ra, 17, 17) | LShiftU64(BitFieldRead64 (Ra, 15, 15), 1) | LShiftU64(BitFieldRead64 (Ra, 16, 16), 2)
            | LShiftU64(BitFieldRead64 (Ra, 31, 31), 3)  | LShiftU64(BitFieldRead64 (Ra, 21, 28), 4) | LShiftU64(BitFieldRead64 (Ra, 19, 19), 12)
            | LShiftU64(BitFieldRead64 (Ra, 29, 30),13)  | LShiftU64(BitFieldRead64 (Ra, 32, 32), 15));

  if (SidNum != 0) {
    //
    //RA32 - R0 RA33-R1 RA34-R2
    //
    RowTemp = BitFieldRead32 (RowTemp, SidNum, 14) << SidNum;  //TO R14
    RowTemp |= (UINT32)BitFieldRead64 (Ra, 32, 32 + SidNum -1);
    RowTemp |= (UINT32)(LShiftU64 (BitFieldRead64 (Ra, 32 + SidNum,  32 + SidNum),15));  //SINCE R15
  }

  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA2~5-C0~3  RA14-C4  RA13-C5  RA9~12-C6~9   RA20-C10
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 2, 5) | LShiftU64(BitFieldRead64 (Ra, 14, 14),4)
               | LShiftU64(BitFieldRead64 (Ra, 13, 13),5)  | LShiftU64(BitFieldRead64 (Ra, 9, 12),6) | LShiftU64(BitFieldRead64 (Ra, 20, 20),10));
  }
}


/**
 this is rank decode for DDR5 x16 16/24/32/64Gb close page mode.

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
Ddr5ClosePgMostX16 (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT32             RowTemp;

  //
  //RA6~7 - BG0~1   RA9~10 - BA0~1
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 6, 7), 2) | BitFieldRead64 (Ra, 9, 10));

  if (Bank != NULL) {
    *Bank = BankTemp;
  }

  if (SubRank != NULL) {
    *SubRank = 0xff;
  }

  //
  //RA17 - R0  RA15 - R1 RA16-R2   RA20~23- R3~6  RA11~13-R7~9   RA18-R10   RA28-R11  RA8-R12    RA29~33-R13~17
  //
  RowTemp = (UINT32)(BitFieldRead64 (Ra, 17, 17) | LShiftU64(BitFieldRead64 (Ra, 15, 15), 1) | LShiftU64(BitFieldRead64 (Ra, 16, 16), 2)
           |  LShiftU64(BitFieldRead64 (Ra, 20, 23), 3) |   LShiftU64(BitFieldRead64 (Ra, 11, 13),7)
           | LShiftU64(BitFieldRead64 (Ra, 18, 18), 10) | LShiftU64(BitFieldRead64 (Ra, 28, 28), 11) | LShiftU64(BitFieldRead64 (Ra, 8, 8), 12)
           | LShiftU64(BitFieldRead64 (Ra, 29, 33),13));



  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA2~5-C0~3  RA14-C4 RA19-C5 RA24~27 - C6~9
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 2, 5) | LShiftU64(BitFieldRead64 (Ra, 14, 14),4) |
              LShiftU64(BitFieldRead64 (Ra, 19, 19), 5) | LShiftU64(BitFieldRead64 (Ra, 24, 27), 6) );
  }
}



/**
 this is rank decode for DDR5  x16 16/24/32/64Gb open page mode - int.

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
Ddr5OpenPgMostX16Int (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT32             RowTemp;

  //
  //RA12 - BG0   RA16 - BG1   RA18~19 - BA0~1
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 12, 12), 2) | LShiftU64 (BitFieldRead64 (Ra, 16, 16), 3) | BitFieldRead64 (Ra, 18, 19));

  if (Bank != NULL) {
    *Bank = BankTemp;
  }

  if (SubRank != NULL) {
    *SubRank = 0xff;
  }

  //
  //RA14 - R0  RA15 - R1 RA13-R2   RA20~28- R3~11  RA17 - R12  RA29~33 - R13~17
  //
  RowTemp = (UINT32)(BitFieldRead64 (Ra, 14, 14) | LShiftU64(BitFieldRead64 (Ra, 15, 15), 1) | LShiftU64(BitFieldRead64 (Ra, 13, 13), 2)
           | LShiftU64(BitFieldRead64 (Ra, 20, 28), 3) | LShiftU64(BitFieldRead64 (Ra, 17, 17), 12) | LShiftU64(BitFieldRead64 (Ra, 29, 33), 13));

  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA2~5-C0~3   RA7~11 - C4~8   RA6-C9
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 2, 5) | LShiftU64(BitFieldRead64 (Ra, 7, 11),4) |
              LShiftU64(BitFieldRead64 (Ra, 6, 6), 9) );
  }
}



/**
 this is rank decode for DDR5  x16 16/24/32/64Gb open page INT2

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
Ddr5OpenPgMostX16Int2 (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT32             RowTemp;

  //
  //RA7 - BG0   RA16 - BG1   RA18~19 - BA0~1
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 7, 7), 2) | LShiftU64 (BitFieldRead64 (Ra, 16, 16), 3) | BitFieldRead64 (Ra, 18, 19));

  if (Bank != NULL) {
    *Bank = BankTemp;
  }

  if (SubRank != NULL) {
    *SubRank = 0xff;
  }

  //
  //RA14 - R0  RA15 - R1 RA13-R2  RA20~28- R3~11  RA17 - R12  RA29~33 - R13~17
  //
  RowTemp = (UINT32)(BitFieldRead64 (Ra, 14, 14) | LShiftU64(BitFieldRead64 (Ra, 15, 15), 1) | LShiftU64(BitFieldRead64 (Ra, 13, 13), 2)
           |  LShiftU64(BitFieldRead64 (Ra, 20, 28), 3) | LShiftU64(BitFieldRead64 (Ra, 17, 17), 12) | LShiftU64(BitFieldRead64 (Ra, 29, 33), 13));

  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA2~6-C0~4   RA8~12 - C5~9
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 2, 6) | LShiftU64(BitFieldRead64 (Ra, 8, 12),5) );
  }
}




/**
 this is rank decode for DDR5  x16 16/24/32/64Gb open page INT4

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
Ddr5OpenPgMostX16Int4 (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT32             RowTemp;

  //
  //RA6~7 - BG0~1    RA18~19 - BA0~1
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 6, 7), 2) |  BitFieldRead64 (Ra, 18, 19));

  if (Bank != NULL) {
    *Bank = BankTemp;
  }

  if (SubRank != NULL) {
    *SubRank = 0xff;
  }

  //
  //RA17 - R0  RA15 - R1 RA16-R2   RA20~28- R3~11  RA8 - R12  RA29~33 - R13~17
  //
  RowTemp = (UINT32)(BitFieldRead64 (Ra, 17, 17) | LShiftU64(BitFieldRead64 (Ra, 15, 15), 1) | LShiftU64(BitFieldRead64 (Ra, 16, 16), 2)
           |  LShiftU64(BitFieldRead64 (Ra, 20, 28), 3) | LShiftU64(BitFieldRead64 (Ra, 8, 8), 12) | LShiftU64(BitFieldRead64 (Ra, 29, 33), 13));

  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA2~5-C0~3    RA14-C4         RA13-C5    RA9~12 - C6~9
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 2, 5) | LShiftU64(BitFieldRead64 (Ra, 14, 14),4)  | LShiftU64(BitFieldRead64 (Ra, 13, 13),5)
               |LShiftU64(BitFieldRead64 (Ra, 9, 12),6));
  }
}



/**
 this is rank decode for DDR5 x16 8Gb close page mode.

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
Ddr5ClosePg8GbX16 (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT32             RowTemp;

  //
  //RA6~7 - BG0~1   RA9- BA0
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 6, 7), 2) | BitFieldRead64 (Ra, 9, 9));

  if (Bank != NULL) {
    *Bank = BankTemp;
  }

  if (SubRank != NULL) {
    *SubRank = 0xff;
  }

  //
  //RA17 - R0  RA15 - R1 RA16-R2   RA20~23- R3~6  RA11~13-R7~9   RA18-R10   RA28-R11  RA8-R12  RA10-R13   RA29~32-R14~17
  //
  RowTemp = (UINT32)(BitFieldRead64 (Ra, 17, 17) | LShiftU64(BitFieldRead64 (Ra, 15, 15), 1) | LShiftU64(BitFieldRead64 (Ra, 16, 16), 2)
           |  LShiftU64(BitFieldRead64 (Ra, 20, 23), 3) |   LShiftU64(BitFieldRead64 (Ra, 11, 13),7)
           | LShiftU64(BitFieldRead64 (Ra, 18, 18), 10) | LShiftU64(BitFieldRead64 (Ra, 28, 28), 11) | LShiftU64(BitFieldRead64 (Ra, 8, 8), 12)
           |LShiftU64(BitFieldRead64 (Ra, 10, 10),13) | LShiftU64(BitFieldRead64 (Ra, 29, 32),14));

  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA2~5-C0~3  RA14-C4 RA19-C5 RA24~27 - C6~9
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 2, 5) | LShiftU64(BitFieldRead64 (Ra, 14, 14),4) |
              LShiftU64(BitFieldRead64 (Ra, 19, 19), 5) | LShiftU64(BitFieldRead64 (Ra, 24, 27), 6) );
  }
}



/**
 this is rank decode for DDR5  x16 8Gb open page mode - int.

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
Ddr5OpenPg8GbX16Int (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT32             RowTemp;

  //
  //RA12 - BG0   RA16 - BG1   RA18 - BA0
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 12, 12), 2) | LShiftU64 (BitFieldRead64 (Ra, 16, 16), 3) | BitFieldRead64 (Ra, 18, 18));

  if (Bank != NULL) {
    *Bank = BankTemp;
  }

  if (SubRank != NULL) {
    *SubRank = 0xff;
  }

  //
  //RA14 - R0  RA15 - R1 RA13-R2   RA20~28- R3~11    RA17-R12    RA19-R13  RA29~32 -R14~17
  //
  RowTemp = (UINT32)(BitFieldRead64 (Ra, 14, 14) | LShiftU64(BitFieldRead64 (Ra, 15, 15), 1) | LShiftU64(BitFieldRead64 (Ra, 13, 13), 2)
           |  LShiftU64(BitFieldRead64 (Ra, 20, 28), 3)  | LShiftU64(BitFieldRead64 (Ra, 17, 17), 12)  | LShiftU64(BitFieldRead64 (Ra, 19, 19), 13)
           | LShiftU64(BitFieldRead64 (Ra, 29, 32), 14));



  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA2~5-C0~3   RA7~11 - C4~8   RA6-C9
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 2, 5) | LShiftU64(BitFieldRead64 (Ra, 7, 11),4) |
              LShiftU64(BitFieldRead64 (Ra, 6, 6), 9) );
  }
}



/**
 this is rank decode for DDR5  x16 8Gb open page INT2

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
Ddr5OpenPg8GbX16Int2 (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT32             RowTemp;

  //
  //RA7 - BG0   RA16 - BG1   RA18 - BA0
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 7, 7), 2) | LShiftU64 (BitFieldRead64 (Ra, 16, 16), 3) | BitFieldRead64 (Ra, 18, 18));

  if (Bank != NULL) {
    *Bank = BankTemp;
  }

  if (SubRank != NULL) {
    *SubRank = 0xff;
  }

  //
  //RA14 - R0  RA15 - R1 RA13-R2   RA20~28- R3~11    RA17-R12    RA19-R13  RA29~32 -R14~17
  //
  RowTemp = (UINT32)(BitFieldRead64 (Ra, 14, 14) | LShiftU64(BitFieldRead64 (Ra, 15, 15), 1) | LShiftU64(BitFieldRead64 (Ra, 13, 13), 2)
           |  LShiftU64(BitFieldRead64 (Ra, 20, 28), 3)  | LShiftU64(BitFieldRead64 (Ra, 17, 17), 12)  | LShiftU64(BitFieldRead64 (Ra, 19, 19), 13)
           | LShiftU64(BitFieldRead64 (Ra, 29, 32), 14));

  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA2~6-C0~4   RA8~12 - C5~9
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 2, 6) | LShiftU64(BitFieldRead64 (Ra, 8, 12),5) );
  }
}




/**
 this is rank decode for DDR5  x16 8Gb open page INT4

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
Ddr5OpenPg8GbX16Int4 (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT32             RowTemp;

  //
  //RA6~7 - BG0~1    RA18 - BA0
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 6, 7), 2) |  BitFieldRead64 (Ra, 18, 18));

  if (Bank != NULL) {
    *Bank = BankTemp;
  }

  if (SubRank != NULL) {
    *SubRank = 0xff;
  }

  //
  //RA17 - R0  RA15 - R1 RA16-R2   RA20~28- R3~11    RA8-R12     RA19-R13    RA29~32- R14~17
  //
  RowTemp = (UINT32)(BitFieldRead64 (Ra, 17, 17) | LShiftU64(BitFieldRead64 (Ra, 15, 15), 1) | LShiftU64(BitFieldRead64 (Ra, 16, 16), 2)
           |  LShiftU64(BitFieldRead64 (Ra, 20, 28), 3) | LShiftU64(BitFieldRead64 (Ra, 8, 8), 12)  | LShiftU64(BitFieldRead64 (Ra, 19, 19), 13)
            | LShiftU64(BitFieldRead64 (Ra, 29, 32), 14));

  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA2~5-C0~3    RA14-C4         RA13-C5    RA9~12 - C6~9
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 2, 5) | LShiftU64(BitFieldRead64 (Ra, 14, 14),4)  | LShiftU64(BitFieldRead64 (Ra, 13, 13),5)
               |LShiftU64(BitFieldRead64 (Ra, 9, 12),6));
  }
}


/**
 this is rank decode for HBM cgbg0

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
HbmCgbg0 (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT8              SubRankTemp = 0xff;
  UINT32             RowTemp;

  //
  //RA6~7 - BG0~1   RA8~9 - BA0~1
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 6, 7), 2) | BitFieldRead64 (Ra, 8, 9));

  if (Bank != NULL) {
    *Bank = BankTemp;
  }

  if (SidNum != 0) {
    //
    //RA10 - SID0
    //
    if (SidNum <= 1) {
      SubRankTemp = (UINT8)BitFieldRead64 (Ra, 10, 10);
    }
  }

  if (SubRank != NULL) {
    *SubRank = SubRankTemp;
  }

  if (SidNum == 0) {
    //
    //RA10~13 - R0~3  RA15~18 - R4~7   RA20~22-R8~10    RA25~28-R11~14
    //
    RowTemp = (UINT32)(BitFieldRead64 (Ra, 10, 13) | LShiftU64(BitFieldRead64 (Ra, 15, 18), 4) | LShiftU64(BitFieldRead64 (Ra, 20, 22), 8)
           | LShiftU64(BitFieldRead64 (Ra, 25, 28), 11) );
  } else {
    //
    //RA11~13 - R0~2  RA15~18 - R3~6   RA20~22-R7~9    RA25~29-R10~14
    //
    RowTemp = (UINT32)(BitFieldRead64 (Ra, 11, 13) | LShiftU64(BitFieldRead64 (Ra, 15, 18), 3) | LShiftU64(BitFieldRead64 (Ra, 20, 22), 7)
           | LShiftU64(BitFieldRead64 (Ra, 25, 29), 10) );
  }

  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA4~5-C0~1  RA14-C2 RA19-C3 RA23~24 - C4~5
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 4, 5) | LShiftU64(BitFieldRead64 (Ra, 14, 14),2) |
              LShiftU64(BitFieldRead64 (Ra, 19, 19), 3) | LShiftU64(BitFieldRead64 (Ra, 23, 24), 4) );
  }
}



/**
 this is rank decode for HBM cgbg1

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
HbmCgbg1 (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT8              SubRankTemp = 0xff;
  UINT32             RowTemp;

  //
  //RA7 - BG0  RA10-BG1      RA13 - BA1  RA11 - BA0
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 7, 7), 2) | LShiftU64 (BitFieldRead64 (Ra, 10, 10), 3) | LShiftU64 (BitFieldRead64 (Ra, 13, 13), 1)
             |BitFieldRead64 (Ra, 11, 11));

  if (Bank != NULL) {
    *Bank = BankTemp;
  }

  if (SidNum != 0) {
    //
    //RA14 - SID0
    //
    if (SidNum <= 1) {
      SubRankTemp = (UINT8)BitFieldRead64 (Ra, 14, 14);
    }
  }

  if (SubRank != NULL) {
    *SubRank = SubRankTemp;
  }

  if (SidNum == 0) {
    //
    //RA14~28 - R0~14
    //
    RowTemp = (UINT32)(BitFieldRead64 (Ra, 14, 28));
  } else {
    //
    //RA15~29 - R0~14
    //
    RowTemp = (UINT32)(BitFieldRead64 (Ra, 15, 29));
  }

  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA4~6-C0~2  RA8~9-C3~4 RA12~C5
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 4, 6) | LShiftU64(BitFieldRead64 (Ra, 8, 9),3) |
              LShiftU64(BitFieldRead64 (Ra, 12, 12), 5));
  }
}




/**
 this is rank decode for HBM cgbg2

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
HbmCgbg2 (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT8              SubRankTemp = 0xff;
  UINT32             RowTemp;

  //
  //RA8 - BG0  RA10-BG1   RA13 - BA1  RA11 - BA0
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 8, 8), 2) | LShiftU64 (BitFieldRead64 (Ra, 10, 10), 3) | LShiftU64 (BitFieldRead64 (Ra, 13, 13), 1)
             |BitFieldRead64 (Ra, 11, 11));

  if (Bank != NULL) {
    *Bank = BankTemp;
  }

  if (SidNum != 0) {
    //
    //RA14 - SID0
    //
    if (SidNum <= 1) {
      SubRankTemp = (UINT8)BitFieldRead64 (Ra, 14, 14);
    }
  }

  if (SubRank != NULL) {
    *SubRank = SubRankTemp;
  }

  if (SidNum == 0) {
    //
    //RA14~28 - R0~14
    //
    RowTemp = (UINT32)(BitFieldRead64 (Ra, 14, 28));
  } else {
    //
    //RA15~29 - R0~14
    //
    RowTemp = (UINT32)(BitFieldRead64 (Ra, 15, 29));
  }

  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA4~7-C0~3  RA9-C4 RA12~C5
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 4, 7) | LShiftU64(BitFieldRead64 (Ra, 9, 9),4) |
              LShiftU64(BitFieldRead64 (Ra, 12, 12), 5));
  }
}




/**
 this is rank decode for HBM cgbg3

 @param[in]            Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
HbmCgbg3 (
  IN     UINT64          Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN OUT UINT8          *SubRank,
  IN OUT UINT8          *Bank,
  IN OUT UINT32         *Row,
  IN OUT UINT32         *Column
  )
{
  UINT8              BankTemp;
  UINT8              SubRankTemp = 0xff;
  UINT32             RowTemp;

  //
  //RA6 - BG0  RA11-BG1   RA12~13 - BA0~1
  //
  BankTemp = (UINT8) (LShiftU64 (BitFieldRead64 (Ra, 6, 6), 2) | LShiftU64 (BitFieldRead64 (Ra, 11, 11), 3) | LShiftU64 (BitFieldRead64 (Ra, 12, 13), 0) );

  if (Bank != NULL) {
    *Bank = BankTemp;
  }

  if (SidNum != 0) {
    //
    //RA14 - SID0
    //
    if (SidNum <= 1) {
      SubRankTemp = (UINT8)BitFieldRead64 (Ra, 14, 14);
    }
  }

  if (SubRank != NULL) {
    *SubRank = SubRankTemp;
  }

  if (SidNum == 0) {
    //
    //RA14~28 - R0~14
    //
    RowTemp = (UINT32)(BitFieldRead64 (Ra, 14, 28));
  } else {
    //
    //RA15~29 - R0~14
    //
    RowTemp = (UINT32)(BitFieldRead64 (Ra, 15, 29));
  }

  if (Row != NULL) {
    *Row = RowTemp;
  }

  if (Column != NULL) {
    //
    //RA4~5-C0~1  RA7~10-C2~5
    //
    *Column = (UINT32)(BitFieldRead64 (Ra, 4, 5) | LShiftU64(BitFieldRead64 (Ra, 7, 10),2) );
  }
}



/**
 this is for AMAP forward decode.

 @param[in]            Ra   -- rank address
 @param[in]            HbmType  -- indicate HBM type
 @param[in]            Width    -- indicate width
 @param[in]            Density  -- indicate density
 @param[in]            ClosePg  -- indicate close page
 @param[in]            Cgbg  -- indicate CGBG type
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in, out]       SubRank  -- subrank
 @param[in, out]       Bank  -- bank
 @param[in, out]       Row -- row
 @param[in, out]       Column  -- column

 @ret none

**/
VOID
EFIAPI
ForwardAmap (
  IN         UINT64      Ra,
  IN         UINT8       HbmType,
  IN         UINT8       Width,
  IN         UINT8       Density,
  IN         UINT8       ClosePg,
  IN         UINT8       Cgbg,
  IN         UINT8       BankXorEnable,
  IN         UINT8       SidXorEnable,
  IN         UINT8       SidNum,
  IN OUT     UINT8      *SubRank,
  IN OUT     UINT8      *Bank,
  IN OUT     UINT32     *Row,
  IN OUT     UINT32     *Column
  )
{
  if (HbmType == TRUE ){
    if (ClosePg == TRUE) {
      HbmCgbg0 (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
    } else if (Cgbg ==  1) {
      //
      //CGBG_INT
      //
      HbmCgbg1  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
    } else if (Cgbg ==  2) {
      //
      //FGBG_INT2
      //
      HbmCgbg2 (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
    } else if (Cgbg ==  3) {
      //
      //FGBG_INT8
      //
      HbmCgbg3  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
    } else {
      DEBUG ((EFI_D_ERROR, "[ForwardAmap] HBM CGBG fail \n"));
    }
  } else {
    switch (Width) {
      case X8_WIDTH:
        if (Density == Density_8Gb) {
          if (ClosePg == TRUE) {
            Ddr5ClosePg8GbX8 (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  0) {
            //
            //CGBG_INT
            //
            Ddr5OpenPg8GbX8Int  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  1) {
            //
            //FGBG_INT2
            //
            Ddr5OpenPg8GbX8Int2  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  2) {
            //
            //FGBG_INT8
            //
            Ddr5OpenPg8GbX8Int8  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else {
            DEBUG ((EFI_D_ERROR, "[ForwardAmap] DDR5 X8 8Gb CGBG fail \n"));
          }
        } else {
          if (ClosePg == TRUE) {
            Ddr5ClosePgMostX8 (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  0) {
            //
            //CGBG_INT
            //
            Ddr5OpenPgMostX8Int  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  1) {
            //
            //FGBG_INT2
            //
            Ddr5OpenPgMostX8Int2  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  2) {
            //
            //FGBG_INT8
            //
            Ddr5OpenPgMostX8Int8  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else {
            DEBUG ((EFI_D_ERROR, "[ForwardAmap] DDR5 X8 16/24/32/64Gb CGBG fail \n"));
          }
        }
        break;
      case X4_WIDTH:
        if (Density == Density_8Gb) {
          if (ClosePg == TRUE) {
            Ddr5ClosePg8GbX4 (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  0) {
            //
            //CGBG_INT
            //
            Ddr5OpenPg8GbX4Int  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  1) {
            //
            //FGBG_INT2
            //
            Ddr5OpenPg8GbX4Int2  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  2) {
            //
            //FGBG_INT8
            //
            Ddr5OpenPg8GbX4Int8  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else {
            DEBUG ((EFI_D_ERROR, "[ForwardAmap] DDR5 X4 8Gb CGBG fail \n"));
          }
        } else {
          if (ClosePg == TRUE) {
            Ddr5ClosePgMostX4 (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  0) {
            //
            //CGBG_INT
            //
            Ddr5OpenPgMostX4Int  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  1) {
            //
            //FGBG_INT2
            //
            Ddr5OpenPgMostX4Int2  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  2) {
            //
            //FGBG_INT8
            //
            Ddr5OpenPgMostX4Int8  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else {
            DEBUG ((EFI_D_ERROR, "[ForwardAmap] DDR5 X4 16/24/32/64Gb CGBG fail \n"));
          }
        }
        break;
      case X16_WIDTH:
        if (Density == Density_8Gb) {
          if (ClosePg == TRUE) {
            Ddr5ClosePg8GbX16 (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  0) {
            //
            //CGBG_INT
            //
            Ddr5OpenPg8GbX16Int  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  1) {
            //
            //FGBG_INT2
            //
            Ddr5OpenPg8GbX16Int2  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  2) {
            //
            //FGBG_INT4
            //
            Ddr5OpenPg8GbX16Int4  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else {
            DEBUG ((EFI_D_ERROR, "[ForwardAmap] DDR5 X16 8Gb CGBG fail \n"));
          }
        } else {
          if (ClosePg == TRUE) {
            Ddr5ClosePgMostX16 (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  0) {
            //
            //CGBG_INT
            //
            Ddr5OpenPgMostX16Int  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  1) {
            //
            //FGBG_INT2
            //
            Ddr5OpenPgMostX16Int2  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  2) {
            //
            //FGBG_INT4
            //
            Ddr5OpenPgMostX16Int4  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else {
            DEBUG ((EFI_D_ERROR, "[ForwardAmap] DDR5 X16 16/24/32/64Gb CGBG fail \n"));
          }
        }
        break;
      default:
        DEBUG ((EFI_D_ERROR, "[ForwardAmap] wrong DDR5 width\n"));
    }
  }

}
