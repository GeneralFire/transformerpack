/** @file
  Implementation of Amap reverse decode.

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
  Check DIMM parameter size, if it exceed its size, report error.

 @param[in]       Row      -- row index
 @param[in]       Column   -- column
 @param[in]       HighestRowBits  -- highest bit of row
 @param[in]       HighestColumnbits -- highest bit of column.

 @ret             TRUE -- The parameter is good; FALSE -- wrong parameter.
**/
BOOLEAN
EFIAPI
CheckDimmParameter (
  IN     UINT32     Row,
  IN     UINT32     Column,
  IN     UINT8      HighestRowBits,
  IN     UINT8      HighestColumnbits
  )
{
  if ((HighBitSet64 (Row) > HighestRowBits) ||
     (HighBitSet64 (Column) > HighestColumnbits)) {
       DEBUG ((EFI_D_ERROR, "[CheckDimmParameter] Wrong at Row:%x Column:%x HighestRowBit:%d  HighestColumnBit:%d\n",
                             Row, Column, HighestRowBits, HighestColumnbits));
       return FALSE;
     } else {
       return TRUE;
     }
}

/**
 this is routine is to do bank xor.

 @param [in]     Bank  -- Input bank with bank group. [BG2, BG1, BG0, BA1, BA0]
 @param [in]     BankXor  -- bank xor seed. [BG2, BA1, BA0, BG1, BG0]

 @ret   bank value in rank address
**/
UINT8
EFIAPI
DoBankXorReverse (
  IN     UINT8      Bank,
  IN     UINT8      BankXor
  )
{
  UINT8    BankSeed;

  BankSeed = BitFieldRead8 (BankXor, 0, 1) << 2 |  BitFieldRead8 (BankXor, 4, 4) << 4 | BitFieldRead8 (BankXor, 2, 3);


  return Bank ^ BankSeed;
}


/**
 this is rank reverse decode for DDR5 x8 (16/24/32/64Gb) close page mode.

 @param[in, out]  Ra   -- rank address
 @param[in]       BankXorEnable  -- bank xor enable
 @param[in]       SidXorEnable  -- sid xor enable
 @param[in]       SidNum  -- sid number
 @param[in]       SubRank  -- subrank
 @param[in]       Bank  -- bank
 @param[in]       Row -- row
 @param[in]       Column  -- column

 @ret none

**/
BOOLEAN
EFIAPI
Ddr5ClosePgMostX8Reverse (
  IN OUT UINT64         *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT8              BankTemp;
  UINT64             RaTemp;
  UINT8              BankXor;
  UINT8              SidTemp;

  if (CheckDimmParameter (Row, Column, 17, 9) == FALSE) {
    return FALSE;
  }

  BankTemp = Bank;
  if (BankXorEnable) {
    //
    // c8 ~9 - BankXor0~1, r11 - BankXor2,  r13 - BankXor3, r12 - BankXor4
    //
    BankXor =  (UINT8)(BitFieldRead64(Column, 8, 9) | LShiftU64 (BitFieldRead64 (Row, 11, 11), 2)  | LShiftU64 (BitFieldRead64 (Row, 13, 13), 3) | LShiftU64 (BitFieldRead64 (Row, 12, 12), 4));
    BankTemp = DoBankXorReverse (Bank, BankXor);
  }

  //
  //RA6~8 - BG0~2  RA9~10 - BA0~1
  //
  RaTemp =  LShiftU64 (BitFieldRead64 (BankTemp, 2, 4), 6) | LShiftU64 (BitFieldRead64 (BankTemp, 0,1), 9);


  if (SidNum != 0) {
    SidTemp = SubRank;

    if (SidXorEnable) {
      SidTemp = SubRank ^ ((UINT8)BitFieldRead64 (Row, 0, 2));
    }
    //
    //RA17 - SID0  RA15 - SID1  RA16 - SID2 RA31~33 - R0~2 if SID.
    //
    if (SidNum == 1) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 17) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 16) | LShiftU64 (BitFieldRead64 (Row, 0, 0), 31);
    }

    if (SidNum == 2) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 17) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 16)  | LShiftU64 (BitFieldRead64 (Row, 0, 1), 31) ;
    }

    if (SidNum == 3) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 17) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (SidTemp, 2, 2), 16)  | LShiftU64 (BitFieldRead64 (Row, 0, 2), 31);
    }
  } else {
    RaTemp |= LShiftU64 (BitFieldRead64 (Row, 0, 0), 17) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 16);
  }


  //
  //RA2~5 - C0~3, RA11~13 - R7~9, RA14 - C4, RA18 - R10, RA19 - C5, RA20~23 - R3~6, RA24~27 - C6~9, RA28 - R11, RA29 - R13, RA30 - R12
  //
  RaTemp |=  LShiftU64 (BitFieldRead64 (Column, 0, 3), 2) | LShiftU64 (BitFieldRead64 (Row, 7, 9), 11) |  LShiftU64 (BitFieldRead64 (Column, 4, 4), 14) 
            | LShiftU64 (BitFieldRead64 (Row, 10, 10), 18) | LShiftU64 (BitFieldRead64 (Column, 5, 5), 19) |  LShiftU64 (BitFieldRead64 (Row, 3, 6), 20)
            | LShiftU64 (BitFieldRead64 (Column, 6, 9), 24) | LShiftU64 (BitFieldRead64 (Row, 11, 11), 28)  |  LShiftU64 (BitFieldRead64 (Row, 13, 13), 29)
            |  LShiftU64 (BitFieldRead64 (Row, 12, 12), 30);

  //RA31 ~34 W/SID - R14~17.
  RaTemp |= LShiftU64 (BitFieldRead64 (Row, 14, 17), 31 + SidNum);

  if (Ra != NULL) {
    *Ra = RaTemp;
  }
  return TRUE;
}


/**
 this is rank reverse decode for DDR5 x8 (16/24/32/64Gb) open page Cgbg_int

 @param[in, out]       Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- column

 @ret none

**/
BOOLEAN
EFIAPI
Ddr5OpenPgMostX8IntReverse (
  IN OUT UINT64         *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT8              BankTemp;
  UINT64             RaTemp;
  UINT8              BankXor;
  UINT8              SidTemp;

  if (CheckDimmParameter (Row, Column, 17, 9) == FALSE) {
    return FALSE;
  }

  BankTemp = Bank;
  if (BankXorEnable) {
    //
    // R9 ~ 11 - BankXor0~2, r13 - BankXor3, r12 - BankXor4
    //
    BankXor =  (UINT8)(BitFieldRead64(Row, 9, 11) | LShiftU64 (BitFieldRead64 (Row, 13, 13), 3) | LShiftU64 (BitFieldRead64 (Row, 12, 12), 4));
    BankTemp = DoBankXorReverse (Bank, BankXor);
  }

  //
  //RA12 - BG0,   RA16~ 17 - BG1~2    RA18~19 - BA0~1
  //
  RaTemp =  LShiftU64 (BitFieldRead64 (BankTemp, 2, 2), 12) | LShiftU64 (BitFieldRead64 (BankTemp, 3,4), 16) | LShiftU64 (BitFieldRead64 (BankTemp, 0, 1), 18);


  if (SidNum != 0) {
    SidTemp = SubRank;

    if (SidXorEnable) {
      SidTemp = SubRank ^ ((UINT8)BitFieldRead64 (Row, 0, 2));
    }
    //
    //RA14 - SID0  RA15 - SID1  RA13 - SID2 RA31~33 - R0~2 if SID.
    //
    if (SidNum == 1) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 14) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13) | LShiftU64 (BitFieldRead64 (Row, 0, 0), 31);
    }

    if (SidNum == 2) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 14) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13)  | LShiftU64 (BitFieldRead64 (Row, 0, 1), 31) ;
    }

    if (SidNum == 3) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 14) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (SidTemp, 2, 2), 13)  | LShiftU64 (BitFieldRead64 (Row, 0, 2), 31);
    }
  } else {
    RaTemp |= LShiftU64 (BitFieldRead64 (Row, 0, 0), 14) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13);
  }

  //
  //RA2~5 - C0~3, RA6 - C9, RA7~11 - C4~8, RA20~28 - R3~R11, RA29 - R13, RA30 - R12
  //
  RaTemp |=  LShiftU64 (BitFieldRead64 (Column, 0, 3), 2) | LShiftU64 (BitFieldRead64 (Column, 9, 9), 6) |  LShiftU64 (BitFieldRead64 (Column, 4, 8), 7)
            | LShiftU64 (BitFieldRead64 (Row, 3, 11), 20) | LShiftU64 (BitFieldRead64 (Row, 13, 13), 29) |  LShiftU64 (BitFieldRead64 (Row, 12, 12), 30);

  //RA31 ~34 W/SID - R14~17.
  RaTemp |= LShiftU64 (BitFieldRead64 (Row, 14, 17), 31 + SidNum);

  if (Ra != NULL) {
    *Ra = RaTemp;
  }
  return TRUE;
}


/**
 this is rank reverse decode for DDR5 x8 (16/24/32/64Gb) open page INT2

 @param[in, out]       Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- column

 @ret none

**/
BOOLEAN
EFIAPI
Ddr5OpenPgMostX8Int2Reverse (
  IN OUT UINT64         *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT8              BankTemp;
  UINT64             RaTemp;
  UINT8              BankXor;
  UINT8              SidTemp;

  if (CheckDimmParameter (Row, Column, 17, 9) == FALSE) {
    return FALSE;
  }

  BankTemp = Bank;
  if (BankXorEnable) {
    //
    // R9 ~ 11 - BankXor0~2, r13 - BankXor3, r12 - BankXor4
    //
    BankXor =  (UINT8)(BitFieldRead64(Row, 9, 11) | LShiftU64 (BitFieldRead64 (Row, 13, 13), 3) | LShiftU64 (BitFieldRead64 (Row, 12, 12), 4));
    BankTemp = DoBankXorReverse (Bank, BankXor);
  }

  //
  // RA7-BG0  RA16~ 17 - BG1~2    RA18~19 - BA0~1
  //
  RaTemp =   LShiftU64 (BitFieldRead64 (BankTemp, 2, 2), 7) | LShiftU64 (BitFieldRead64 (BankTemp, 3,4), 16) | LShiftU64 (BitFieldRead64 (BankTemp, 0, 1), 18);


  if (SidNum != 0) {
    SidTemp = SubRank;

    if (SidXorEnable) {
      SidTemp = SubRank ^ ((UINT8)BitFieldRead64 (Row, 0, 2));
    }
    //
    //RA14 - SID0  RA15 - SID1  RA13 - SID2 RA31~33 - R0~2 if SID.
    //
    if (SidNum == 1) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 14) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13) | LShiftU64 (BitFieldRead64 (Row, 0, 0), 31);
    }

    if (SidNum == 2) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 14) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13)  | LShiftU64 (BitFieldRead64 (Row, 0, 1), 31) ;
    }

    if (SidNum == 3) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 14) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (SidTemp, 2, 2), 13)  | LShiftU64 (BitFieldRead64 (Row, 0, 2), 31);
    }
  } else {
    RaTemp |= LShiftU64 (BitFieldRead64 (Row, 0, 0), 14) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13);
  }

  //
  //RA2~6 - C0~4, RA8~12 - C5~9, RA20~28 - R3~R11, RA29 - R13, RA30 - R12
  //
  RaTemp |=  LShiftU64 (BitFieldRead64 (Column, 0, 4), 2) | LShiftU64 (BitFieldRead64 (Column, 5, 9), 8)
            | LShiftU64 (BitFieldRead64 (Row, 3, 11), 20) | LShiftU64 (BitFieldRead64 (Row, 13, 13), 29) |  LShiftU64 (BitFieldRead64 (Row, 12, 12), 30);

  //RA31 ~34 W/SID - R14~17.
  RaTemp |= LShiftU64 (BitFieldRead64 (Row, 14, 17), 31 + SidNum);

  if (Ra != NULL) {
    *Ra = RaTemp;
  }
  return TRUE;
}


/**
 this is rank reverse decode for DDR5 x8 (16/24/32/64Gb) open page INT8

 @param[in, out]       Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- column

 @ret none

**/
BOOLEAN
EFIAPI
Ddr5OpenPgMostX8Int8Reverse (
  IN OUT UINT64         *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT8              BankTemp;
  UINT64             RaTemp;
  UINT8              BankXor;
  UINT8              SidTemp;

  if (CheckDimmParameter (Row, Column, 17, 9) == FALSE) {
    return FALSE;
  }

  BankTemp = Bank;
  if (BankXorEnable) {
    //
    // R9 ~ 11 - BankXor0~2, r13 - BankXor3, r12 - BankXor4
    //
    BankXor =  (UINT8)(BitFieldRead64(Row, 9, 11) | LShiftU64 (BitFieldRead64 (Row, 13, 13), 3) | LShiftU64 (BitFieldRead64 (Row, 12, 12), 4));
    BankTemp = DoBankXorReverse (Bank, BankXor);
  }

  //
  // RA6~8-BG0~2   RA18~19 - BA0~1
  //
  RaTemp =   LShiftU64 (BitFieldRead64 (BankTemp, 0, 2), 6) |  LShiftU64 (BitFieldRead64 (BankTemp, 0, 1), 18);


  if (SidNum != 0) {
    SidTemp = SubRank;

    if (SidXorEnable) {
      SidTemp = SubRank ^ ((UINT8)BitFieldRead64 (Row, 0, 2));
    }
    //
    //RA17 - SID0  RA15 - SID1  RA16 - SID2 RA31~33 - R0~2 if SID.
    //
    if (SidNum == 1) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 17) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 16) | LShiftU64 (BitFieldRead64 (Row, 0, 0), 31);
    }

    if (SidNum == 2) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 17) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 16)  | LShiftU64 (BitFieldRead64 (Row, 0, 1), 31) ;
    }

    if (SidNum == 3) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 17) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (SidTemp, 2, 2), 16)  | LShiftU64 (BitFieldRead64 (Row, 0, 2), 31);
    }
  } else {
    RaTemp |= LShiftU64 (BitFieldRead64 (Row, 0, 0), 17) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 16);
  }

  //
  //RA2~5 - C0~3, RA9~12 - C6~9, RA13 - C5, RA14 - c4,  RA20~28 - R3~R11,   RA29 - R13, RA30 - R12
  //
  RaTemp |=  LShiftU64 (BitFieldRead64 (Column, 0, 3), 2) | LShiftU64 (BitFieldRead64 (Column, 6, 9), 9) | LShiftU64 (BitFieldRead64 (Column, 5, 5), 13)
            | LShiftU64 (BitFieldRead64 (Column, 4, 4), 14)
            | LShiftU64 (BitFieldRead64 (Row, 3, 11), 20) | LShiftU64 (BitFieldRead64 (Row, 13, 13), 29) |  LShiftU64 (BitFieldRead64 (Row, 12, 12), 30);

  //RA31 ~34 W/SID - R14~17.
  RaTemp |= LShiftU64 (BitFieldRead64 (Row, 14, 17), 31 + SidNum);

  if (Ra != NULL) {
    *Ra = RaTemp;
  }
  return TRUE;
}



/**
 this is rank reverse decode for DDR5 x8 8Gb close page mode.

 @param[in, out]       Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- column

 @ret none

**/
BOOLEAN
EFIAPI
Ddr5ClosePg8GbX8Reverse (
  IN OUT UINT64         *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT8              BankTemp;
  UINT64             RaTemp;
  UINT8              BankXor;
  UINT8              SidTemp;

  if (CheckDimmParameter (Row, Column, 15, 9) == FALSE) {
    return FALSE;
  }

  BankTemp = Bank;
  if (BankXorEnable) {
    //
    // c8 ~9 - BankXor0~1, r11 - BankXor2,  r13 - BankXor3, r14- BankXor4
    //
    BankXor =  (UINT8)(BitFieldRead64(Column, 8, 9) | LShiftU64 (BitFieldRead64 (Row, 11, 11), 2)  | LShiftU64 (BitFieldRead64 (Row, 13, 13), 3) | LShiftU64 (BitFieldRead64 (Row, 14, 14), 4));
    BankTemp = DoBankXorReverse (Bank, BankXor);
  }

  //
  //RA6~8 - BG0~2  RA9 - BA0
  //
  RaTemp =  LShiftU64 (BitFieldRead64 (BankTemp, 2, 4), 6) | LShiftU64 (BitFieldRead64 (BankTemp, 0,0), 9);


  if (SidNum != 0) {
    SidTemp = SubRank;

    if (SidXorEnable) {
      SidTemp = SubRank ^ ((UINT8)BitFieldRead64 (Row, 0, 2));
    }
    //
    //RA17 - SID0  RA15 - SID1  RA16 - SID2   RA31~33 - R0~2 if SID.
    //
    if (SidNum == 1) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 17) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 16) | LShiftU64 (BitFieldRead64 (Row, 0, 0), 31);
    }

    if (SidNum == 2) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 17) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 16)  | LShiftU64 (BitFieldRead64 (Row, 0, 1), 31) ;
    }

    if (SidNum == 3) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 17) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (SidTemp, 2, 2), 16)  | LShiftU64 (BitFieldRead64 (Row, 0, 2), 31);
    }
  } else {
    RaTemp |= LShiftU64 (BitFieldRead64 (Row, 0, 0), 17) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 16);
  }


  //
  //RA2~5 - C0~3, RA10 - R12,  RA11~13 - R7~9, RA14 - C4, RA18 - R10, RA19 - C5, RA20~23 - R3~6, RA24~27 - C6~9, RA28 - R11, RA29 - R13, RA30 - R14
  //
  RaTemp |=  LShiftU64 (BitFieldRead64 (Column, 0, 3), 2) | LShiftU64 (BitFieldRead64 (Row, 12, 12), 10)  | LShiftU64 (BitFieldRead64 (Row, 7, 9), 11) 
            |  LShiftU64 (BitFieldRead64 (Column, 4, 4), 14)
            | LShiftU64 (BitFieldRead64 (Row, 10, 10), 18) | LShiftU64 (BitFieldRead64 (Column, 5, 5), 19) |  LShiftU64 (BitFieldRead64 (Row, 3, 6), 20)
            | LShiftU64 (BitFieldRead64 (Column, 6, 9), 24) | LShiftU64 (BitFieldRead64 (Row, 11, 11), 28)  |  LShiftU64 (BitFieldRead64 (Row, 13, 13), 29)
            |  LShiftU64 (BitFieldRead64 (Row, 14, 14), 30);

  //RA31  W/SID - R15
  RaTemp |= LShiftU64 (BitFieldRead64 (Row, 15, 15), 31 + SidNum);

  if (Ra != NULL) {
    *Ra = RaTemp;
  }
  return TRUE;
}



/**
 this is rank  reverse decode for DDR5 x8 8Gb open page mode - int.

 @param[in, out]       Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- column

 @ret none

**/
BOOLEAN
EFIAPI
Ddr5OpenPg8GbX8IntReverse (
  IN OUT UINT64         *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT8              BankTemp;
  UINT64             RaTemp;
  UINT8              BankXor;
  UINT8              SidTemp;

  if (CheckDimmParameter (Row, Column, 15, 9) == FALSE) {
    return FALSE;
  }

  BankTemp = Bank;
  if (BankXorEnable) {
    //
    // r9~11 - BankXor0~2, r13~14 - BankXor3~4
    //
    BankXor =  (UINT8)(BitFieldRead64(Row, 9, 11) | LShiftU64 (BitFieldRead64 (Row, 13, 14), 3));
    BankTemp = DoBankXorReverse (Bank, BankXor);
  }
  
  //
  //RA12 - BG0  RA16~17 - BG1~2  RA18 - BA0
  //
  RaTemp =  LShiftU64 (BitFieldRead64 (BankTemp, 2, 2), 12) | LShiftU64 (BitFieldRead64 (BankTemp, 3,4), 16) | LShiftU64 (BitFieldRead64 (BankTemp, 0,0), 18);


  if (SidNum != 0) {
    SidTemp = SubRank;

    if (SidXorEnable) {
      SidTemp = SubRank ^ ((UINT8)BitFieldRead64 (Row, 0, 2));
    }
    //
    //RA14 - SID0  RA15 - SID1  RA13 - SID2   RA31~33 - R0~2 if SID.
    //
    if (SidNum == 1) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 14) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13) | LShiftU64 (BitFieldRead64 (Row, 0, 0), 31);
    }

    if (SidNum == 2) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 14) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13)  | LShiftU64 (BitFieldRead64 (Row, 0, 1), 31) ;
    }

    if (SidNum == 3) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 14) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (SidTemp, 2, 2), 13)  | LShiftU64 (BitFieldRead64 (Row, 0, 2), 31);
    }
  } else {
    RaTemp |= LShiftU64 (BitFieldRead64 (Row, 0, 0), 14) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13);
  }


  //
  //RA2~5 - C0~3, RA6 - C9,  RA7~11 - C4~8, RA19 - R12, RA20~28 - R3~11,  RA29~30 - R13~14
  //
  RaTemp |=  LShiftU64 (BitFieldRead64 (Column, 0, 3), 2) | LShiftU64 (BitFieldRead64 (Column, 9, 9), 6)  | LShiftU64 (BitFieldRead64 (Column, 4, 8), 7)
            |  LShiftU64 (BitFieldRead64 (Row, 12, 12), 19) | LShiftU64 (BitFieldRead64 (Row, 3, 11), 20) | LShiftU64 (BitFieldRead64 (Row, 13, 14), 29);

  //RA31  W/SID - R15
  RaTemp |= LShiftU64 (BitFieldRead64 (Row, 15, 15), 31 + SidNum);

  if (Ra != NULL) {
    *Ra = RaTemp;
  }
  return TRUE;
}


/**
 this is rank reverse decode for DDR5 x8 8Gb open page INT2

 @param[in, out]       Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- column

 @ret none

**/
BOOLEAN
EFIAPI
Ddr5OpenPg8GbX8Int2Reverse (
  IN OUT UINT64         *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT8              BankTemp;
  UINT64             RaTemp;
  UINT8              BankXor;
  UINT8              SidTemp;

  if (CheckDimmParameter (Row, Column, 15, 9) == FALSE) {
    return FALSE;
  }

  BankTemp = Bank;
  if (BankXorEnable) {
    //
    // r9~11 - BankXor0~2, r13~14 - BankXor3~4
    //
    BankXor =  (UINT8)(BitFieldRead64(Row, 9, 11) | LShiftU64 (BitFieldRead64 (Row, 13, 14), 3));
    BankTemp = DoBankXorReverse (Bank, BankXor);
  }

  //
  //RA7 - BG0  RA16~17 - BG1~2  RA18 - BA0
  //
  RaTemp =  LShiftU64 (BitFieldRead64 (BankTemp, 2, 2), 7) | LShiftU64 (BitFieldRead64 (BankTemp, 3,4), 16) | LShiftU64 (BitFieldRead64 (BankTemp, 0,0), 18);


  if (SidNum != 0) {
    SidTemp = SubRank;

    if (SidXorEnable) {
      SidTemp = SubRank ^ ((UINT8)BitFieldRead64 (Row, 0, 2));
    }
    //
    //RA14 - SID0  RA15 - SID1  RA13 - SID2   RA31~33 - R0~2 if SID.
    //
    if (SidNum == 1) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 14) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13) | LShiftU64 (BitFieldRead64 (Row, 0, 0), 31);
    }

    if (SidNum == 2) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 14) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13)  | LShiftU64 (BitFieldRead64 (Row, 0, 1), 31) ;
    }

    if (SidNum == 3) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 14) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (SidTemp, 2, 2), 13)  | LShiftU64 (BitFieldRead64 (Row, 0, 2), 31);
    }
  } else {
    RaTemp |= LShiftU64 (BitFieldRead64 (Row, 0, 0), 14) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13);
  }


  //
  //RA2~6 - C0~4, RA8~12 - C5~9,  RA19 - R12, RA20~28 - R3~11,  RA29~30 - R13~14
  //
  RaTemp |=  LShiftU64 (BitFieldRead64 (Column, 0, 4), 2) | LShiftU64 (BitFieldRead64 (Column, 5, 9), 8) | LShiftU64 (BitFieldRead64 (Row, 12, 12), 19)
             | LShiftU64 (BitFieldRead64 (Row, 3, 11), 20) | LShiftU64 (BitFieldRead64 (Row, 13, 14), 29);

  //RA31  W/SID - R15
  RaTemp |= LShiftU64 (BitFieldRead64 (Row, 15, 15), 31 + SidNum);

  if (Ra != NULL) {
    *Ra = RaTemp;
  }
  return TRUE;
}




/**
 this is rank reverse decode for DDR5 x8 8Gb open page INT8

 @param[in, out]       Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- column

 @ret none

**/
BOOLEAN
EFIAPI
Ddr5OpenPg8GbX8Int8Reverse (
  IN OUT UINT64         *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT8              BankTemp;
  UINT64             RaTemp;
  UINT8              BankXor;
  UINT8              SidTemp;

  if (CheckDimmParameter (Row, Column, 15, 9) == FALSE) {
    return FALSE;
  }
  BankTemp = Bank;
  if (BankXorEnable) {
    //
    // r9~11 - BankXor0~2, r13~14 - BankXor3~4
    //
    BankXor =  (UINT8)(BitFieldRead64(Row, 9, 11) | LShiftU64 (BitFieldRead64 (Row, 13, 14), 3));
    BankTemp = DoBankXorReverse (Bank, BankXor);
  }
  
  //
  //RA6~8 - BG0~2  RA18 - BA0
  //
  RaTemp =  LShiftU64 (BitFieldRead64 (BankTemp, 2, 4), 6) | LShiftU64 (BitFieldRead64 (BankTemp, 0,0), 18);


  if (SidNum != 0) {
    SidTemp = SubRank;

    if (SidXorEnable) {
      SidTemp = SubRank ^ ((UINT8)BitFieldRead64 (Row, 0, 2));
    }
    //
    //RA17 - SID0  RA15 - SID1  RA16 - SID2   RA31~33 - R0~2 if SID.
    //
    if (SidNum == 1) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 17) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 16) | LShiftU64 (BitFieldRead64 (Row, 0, 0), 31);
    }

    if (SidNum == 2) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 17) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 16)  | LShiftU64 (BitFieldRead64 (Row, 0, 1), 31) ;
    }

    if (SidNum == 3) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 17) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (SidTemp, 2, 2), 16)  | LShiftU64 (BitFieldRead64 (Row, 0, 2), 31);
    }
  } else {
    RaTemp |= LShiftU64 (BitFieldRead64 (Row, 0, 0), 17) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 16);
  }


  //
  //RA2~5 - C0~3, RA9~12 - C6~9,  RA13 - C5,  RA14 - C4， RA19 - R12, RA20~28 - R3~11,  RA29~30 - R13~14
  //
  RaTemp |=  LShiftU64 (BitFieldRead64 (Column, 0, 3), 2) | LShiftU64 (BitFieldRead64 (Column, 6, 9), 9) | LShiftU64 (BitFieldRead64 (Column, 5, 5), 13)
             | LShiftU64 (BitFieldRead64 (Column, 4, 4), 14) | LShiftU64 (BitFieldRead64 (Row, 12, 12), 19)
             | LShiftU64 (BitFieldRead64 (Row, 3, 11), 20) | LShiftU64 (BitFieldRead64 (Row, 13, 14), 29);

  //RA31  W/SID - R15
  RaTemp |= LShiftU64 (BitFieldRead64 (Row, 15, 15), 31 + SidNum);

  if (Ra != NULL) {
    *Ra = RaTemp;
  }

  return TRUE;
}



/**
 this is rank reverse decode for DDR5 x4(16/24/32/64Gb) close page mode.

 @param[in, out]       Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- column

 @ret none
 
**/
BOOLEAN
EFIAPI
Ddr5ClosePgMostX4Reverse (
  IN OUT UINT64         *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT8              BankTemp;
  UINT64             RaTemp;
  UINT8              BankXor;
  UINT8              SidTemp;

  if (CheckDimmParameter (Row, Column, 17, 10) == FALSE) {
    return FALSE;
  }

  BankTemp = Bank;
  if (BankXorEnable) {
    //
    // C8~9 - BankXor0~1, R11 - BankXor2, R13 - BankXor3, R12 - BankXor4
    //
    BankXor =  (UINT8)(BitFieldRead64(Column, 8, 9) | LShiftU64 (BitFieldRead64 (Row, 11, 11), 2) | LShiftU64 (BitFieldRead64 (Row, 13, 13), 3) | LShiftU64 (BitFieldRead64 (Row, 12, 12), 4));
    BankTemp = DoBankXorReverse (Bank, BankXor);
  }

  //
  //RA6~8 - BG0~2  RA9~10 - BA0~1
  //
  RaTemp =  LShiftU64 (BitFieldRead64 (BankTemp, 2, 4), 6) | LShiftU64 (BitFieldRead64 (BankTemp, 0,1), 9);


  if (SidNum != 0) {
    SidTemp = SubRank;

    if (SidXorEnable) {
      SidTemp = SubRank ^ ((UINT8)BitFieldRead64 (Row, 0, 2));
    }
    //
    //RA17 - SID0  RA15 - SID1  RA16 - SID2   RA32~34 - R0~2 if SID.
    //
    if (SidNum == 1) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 17) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 16) | LShiftU64 (BitFieldRead64 (Row, 0, 0), 32);
    }

    if (SidNum == 2) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 17) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 16)  | LShiftU64 (BitFieldRead64 (Row, 0, 1), 32) ;
    }

    if (SidNum == 3) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 17) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (SidTemp, 2, 2), 16)  | LShiftU64 (BitFieldRead64 (Row, 0, 2), 32);
    }
  } else {
    RaTemp |= LShiftU64 (BitFieldRead64 (Row, 0, 0), 17) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 16);
  }


  //
  //RA2~5 - C0~3, RA11~13 - R7~9,  RA14 - C4， RA18 - R10, R19 - C5, RA20 - C10, RA21~23- R4~6, RA24~27 - C6~9, RA28 - R11,  RA29 - R13, RA30 - R12, RA31 - R3
  //
  RaTemp |=  LShiftU64 (BitFieldRead64 (Column, 0, 3), 2) | LShiftU64 (BitFieldRead64 (Row, 7, 9), 11) | LShiftU64 (BitFieldRead64 (Column, 4, 4), 14)
             | LShiftU64 (BitFieldRead64 (Row, 10, 10), 18) | LShiftU64 (BitFieldRead64 (Column, 5, 5), 19) | LShiftU64 (BitFieldRead64 (Column, 10, 10), 20)
             | LShiftU64 (BitFieldRead64 (Row, 4, 6), 21) | LShiftU64 (BitFieldRead64 (Column, 6, 9), 24) | LShiftU64 (BitFieldRead64 (Row, 11, 11), 28)
             | LShiftU64 (BitFieldRead64 (Row, 13, 13), 29) | LShiftU64 (BitFieldRead64 (Row, 12, 12), 30) | LShiftU64 (BitFieldRead64 (Row, 3, 3), 31);

  //RA32~35  W/SID - R14~17
  RaTemp |= LShiftU64 (BitFieldRead64 (Row, 14, 17), 32 + SidNum);

  if (Ra != NULL) {
    *Ra = RaTemp;
  }

  return TRUE;
}




/**
 this is rank reverse decode for DDR5 x4 (16/24/32/64Gb) open page Cgbg_int

 @param[in, out]       Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- column

 @ret none

**/
BOOLEAN
EFIAPI
Ddr5OpenPgMostX4IntReverse (
  IN OUT UINT64         *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT8              BankTemp;
  UINT64             RaTemp;
  UINT8              BankXor;
  UINT8              SidTemp;

  if (CheckDimmParameter (Row, Column, 17, 10) == FALSE) {
    return FALSE;
  }

  BankTemp = Bank;
  if (BankXorEnable) {
    //
    // R9~11 - BankXor0~2, R13 - BankXor3, R12 - BankXor4
    //
    BankXor =  (UINT8)(BitFieldRead64(Row, 9, 11) | LShiftU64 (BitFieldRead64 (Row, 13, 13), 3) | LShiftU64 (BitFieldRead64 (Row, 12, 12), 4));
    BankTemp = DoBankXorReverse (Bank, BankXor);
  }

  //
  //RA12 - BG0  RA16~17 - BG1~2  RA18~19 - BA0~1
  //
  RaTemp =  LShiftU64 (BitFieldRead64 (BankTemp, 2, 2), 12) | LShiftU64 (BitFieldRead64 (BankTemp, 3,4), 16) | LShiftU64 (BitFieldRead64 (BankTemp, 0,1), 18);


  if (SidNum != 0) {
    SidTemp = SubRank;

    if (SidXorEnable) {
      SidTemp = SubRank ^ ((UINT8)BitFieldRead64 (Row, 0, 2));
    }
    //
    //RA14 - SID0  RA15 - SID1  RA13 - SID2   RA32~34 - R0~2 if SID.
    //
    if (SidNum == 1) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 14) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13) | LShiftU64 (BitFieldRead64 (Row, 0, 0), 32);
    }

    if (SidNum == 2) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 14) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13)  | LShiftU64 (BitFieldRead64 (Row, 0, 1), 32) ;
    }

    if (SidNum == 3) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 14) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (SidTemp, 2, 2), 13)  | LShiftU64 (BitFieldRead64 (Row, 0, 2), 32);
    }
  } else {
    RaTemp |= LShiftU64 (BitFieldRead64 (Row, 0, 0), 14) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13);
  }


  //
  //RA2~5 - C0~3, RA6 - C9,  RA7~11 - C4~8，  RA20 - C10, RA21~28- R4~11, RA29 - R13, RA30 - R12, RA31 - R3
  //
  RaTemp |=  LShiftU64 (BitFieldRead64 (Column, 0, 3), 2) | LShiftU64 (BitFieldRead64 (Column, 9, 9), 6) | LShiftU64 (BitFieldRead64 (Column, 4, 8), 7)
             | LShiftU64 (BitFieldRead64 (Column, 10, 10), 20) | LShiftU64 (BitFieldRead64 (Row, 4, 11), 21)
             | LShiftU64 (BitFieldRead64 (Row, 13, 13), 29) | LShiftU64 (BitFieldRead64 (Row, 12, 12), 30) | LShiftU64 (BitFieldRead64 (Row, 3, 3), 31);

  //RA32~35  W/SID - R14~17
  RaTemp |= LShiftU64 (BitFieldRead64 (Row, 14, 17), 32 + SidNum);

  if (Ra != NULL) {
    *Ra = RaTemp;
  }
  return TRUE;
}




/**
 this is rank reverse decode for DDR5 x4 (16/24/32/64Gb) open page INT2

 @param[in, out]       Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- column

 @ret none

**/
BOOLEAN
EFIAPI
Ddr5OpenPgMostX4Int2Reverse (
  IN OUT UINT64         *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT8              BankTemp;
  UINT64             RaTemp;
  UINT8              BankXor;
  UINT8              SidTemp;

  if (CheckDimmParameter (Row, Column, 17, 10) == FALSE) {
    return FALSE;
  }

  BankTemp = Bank;
  if (BankXorEnable) {
    //
    // R9~11 - BankXor0~2, R13 - BankXor3, R12 - BankXor4
    //
    BankXor =  (UINT8)(BitFieldRead64(Row, 9, 11) | LShiftU64 (BitFieldRead64 (Row, 13, 13), 3) | LShiftU64 (BitFieldRead64 (Row, 12, 12), 4));
    BankTemp = DoBankXorReverse (Bank, BankXor);
  }

  //
  //RA7 - BG0  RA16~17 - BG1~2  RA18~19 - BA0~1
  //
  RaTemp =  LShiftU64 (BitFieldRead64 (BankTemp, 2, 2), 7) | LShiftU64 (BitFieldRead64 (BankTemp, 3,4), 16) | LShiftU64 (BitFieldRead64 (BankTemp, 0,1), 18);


  if (SidNum != 0) {
    SidTemp = SubRank;

    if (SidXorEnable) {
      SidTemp = SubRank ^ ((UINT8)BitFieldRead64 (Row, 0, 2));
    }
    //
    //RA14 - SID0  RA15 - SID1  RA13 - SID2   RA32~34 - R0~2 if SID.
    //
    if (SidNum == 1) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 14) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13) | LShiftU64 (BitFieldRead64 (Row, 0, 0), 32);
    }

    if (SidNum == 2) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 14) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13)  | LShiftU64 (BitFieldRead64 (Row, 0, 1), 32) ;
    }

    if (SidNum == 3) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 14) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (SidTemp, 2, 2), 13)  | LShiftU64 (BitFieldRead64 (Row, 0, 2), 32);
    }
  } else {
    RaTemp |= LShiftU64 (BitFieldRead64 (Row, 0, 0), 14) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13);
  }


  //
  //RA2~6 - C0~4, RA8~12 - C5~9, RA20 - C10, RA21~28- R4~11, RA29 - R13, RA30 - R12, RA31 - R3
  //
  RaTemp |=  LShiftU64 (BitFieldRead64 (Column, 0, 4), 2) | LShiftU64 (BitFieldRead64 (Column, 5, 9), 8) | LShiftU64 (BitFieldRead64 (Column, 10, 10), 20)
              | LShiftU64 (BitFieldRead64 (Row, 4, 11), 21) | LShiftU64 (BitFieldRead64 (Row, 13, 13), 29)
              | LShiftU64 (BitFieldRead64 (Row, 12, 12), 30) | LShiftU64 (BitFieldRead64 (Row, 3, 3), 31);

  //RA32~35  W/SID - R14~17
  RaTemp |= LShiftU64 (BitFieldRead64 (Row, 14, 17), 32 + SidNum);

  if (Ra != NULL) {
    *Ra = RaTemp;
  }

  return TRUE;
}





/**
 this is rank reverse decode for DDR5 x4 (16/24/32/64Gb) open page INT8

 @param[in, out]       Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- column

 @ret none

**/
BOOLEAN
EFIAPI
Ddr5OpenPgMostX4Int8Reverse (
  IN OUT UINT64         *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT8              BankTemp;
  UINT64             RaTemp;
  UINT8              BankXor;
  UINT8              SidTemp;

  if (CheckDimmParameter (Row, Column, 17, 10) == FALSE) {
    return FALSE;
  }

  BankTemp = Bank;
  if (BankXorEnable) {
    //
    // R9~11 - BankXor0~2, R13 - BankXor3, R12 - BankXor4
    //
    BankXor =  (UINT8)(BitFieldRead64(Row, 9, 11) | LShiftU64 (BitFieldRead64 (Row, 13, 13), 3) | LShiftU64 (BitFieldRead64 (Row, 12, 12), 4));
    BankTemp = DoBankXorReverse (Bank, BankXor);
  }
  
  //
  //RA6~8 - BG0~2  RA18~19 - BA0~1
  //
  RaTemp =  LShiftU64 (BitFieldRead64 (BankTemp, 2, 4), 6) | LShiftU64 (BitFieldRead64 (BankTemp, 0,1), 18);


  if (SidNum != 0) {
    SidTemp = SubRank;

    if (SidXorEnable) {
      SidTemp = SubRank ^ ((UINT8)BitFieldRead64 (Row, 0, 2));
    }
    //
    //RA17 - SID0  RA15 - SID1  RA16 - SID2   RA32~34 - R0~2 if SID.
    //
    if (SidNum == 1) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 17) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 16) | LShiftU64 (BitFieldRead64 (Row, 0, 0), 32);
    }

    if (SidNum == 2) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 17) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 16)  | LShiftU64 (BitFieldRead64 (Row, 0, 1), 32) ;
    }

    if (SidNum == 3) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 17) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (SidTemp, 2, 2), 16)  | LShiftU64 (BitFieldRead64 (Row, 0, 2), 32);
    }
  } else {
    RaTemp |= LShiftU64 (BitFieldRead64 (Row, 0, 0), 17) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 16);
  }


  //
  //RA2~5 - C0~3, RA9~12 - C6~9, RA13 - C5, RA14- C4, R20 - C10, RA21~28- R4~11, RA29 - R13, RA30 - R12, RA31 - R3
  //
  RaTemp |=  LShiftU64 (BitFieldRead64 (Column, 0, 3), 2) | LShiftU64 (BitFieldRead64 (Column, 6, 9), 9) | LShiftU64 (BitFieldRead64 (Column, 5, 5), 13)
             | LShiftU64 (BitFieldRead64 (Column, 4, 4), 14)| LShiftU64 (BitFieldRead64 (Column, 10, 10), 20) | LShiftU64 (BitFieldRead64 (Row, 4, 11), 21)
             | LShiftU64 (BitFieldRead64 (Row, 13, 13), 29) | LShiftU64 (BitFieldRead64 (Row, 12, 12), 30) | LShiftU64 (BitFieldRead64 (Row, 3, 3), 31);

  //RA32~35  W/SID - R14~17
  RaTemp |= LShiftU64 (BitFieldRead64 (Row, 14, 17), 32 + SidNum);

  if (Ra != NULL) {
    *Ra = RaTemp;
  }

  return TRUE;
}




/**
 this is rank reverse decode for DDR5 x4 8Gb close page mode.

 @param[in, out]       Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- column

 @ret none

**/
BOOLEAN
EFIAPI
Ddr5ClosePg8GbX4Reverse (
  IN OUT UINT64          *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT8              BankTemp;
  UINT64             RaTemp;
  UINT8              BankXor;
  UINT8              SidTemp;

  if (CheckDimmParameter (Row, Column, 15, 10) == FALSE) {
    return FALSE;
  }

  BankTemp = Bank;
  if (BankXorEnable) {
    //
    // C8~9 - BankXor0~1, R11 - BankXor2, R13~14 - BankXor3~4
    //
    BankXor =  (UINT8)(BitFieldRead64(Column, 8, 9) | LShiftU64 (BitFieldRead64 (Row, 11, 11), 2) | LShiftU64 (BitFieldRead64 (Row, 13, 14), 3));
    BankTemp = DoBankXorReverse (Bank, BankXor);
  }

  //
  //RA6~8 - BG0~2  RA9 - BA0
  //
  RaTemp =  LShiftU64 (BitFieldRead64 (BankTemp, 2, 4), 6) | LShiftU64 (BitFieldRead64 (BankTemp, 0, 0), 9);


  if (SidNum != 0) {
    SidTemp = SubRank;

    if (SidXorEnable) {
      SidTemp = SubRank ^ ((UINT8)BitFieldRead64 (Row, 0, 2));
    }
    //
    //RA17 - SID0  RA15 - SID1  RA16 - SID2   RA32~34 - R0~2 if SID.
    //
    if (SidNum == 1) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 17) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 16) | LShiftU64 (BitFieldRead64 (Row, 0, 0), 32);
    }

    if (SidNum == 2) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 17) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 16)  | LShiftU64 (BitFieldRead64 (Row, 0, 1), 32) ;
    }

    if (SidNum == 3) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 17) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (SidTemp, 2, 2), 16)  | LShiftU64 (BitFieldRead64 (Row, 0, 2), 32);
    }
  } else {
    RaTemp |= LShiftU64 (BitFieldRead64 (Row, 0, 0), 17) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 16);
  }


  //
  //RA2~5 - C0~3, RA10 - R12, RA11~13 - R7~9,  RA14 - C4， RA18 - R10, RA19 - C5, RA20 - C10, RA21~23- R4~6, RA24~27 - C6~9, RA28 - R11,  RA29 - R13, RA30 - R14, RA31 - R3
  //
  RaTemp |=  LShiftU64 (BitFieldRead64 (Column, 0, 3), 2) | LShiftU64 (BitFieldRead64 (Row, 12, 12), 10) | LShiftU64 (BitFieldRead64 (Row, 7, 9), 11) | LShiftU64 (BitFieldRead64 (Column, 4, 4), 14)
             | LShiftU64 (BitFieldRead64 (Row, 10, 10), 18) | LShiftU64 (BitFieldRead64 (Column, 5, 5), 19) | LShiftU64 (BitFieldRead64 (Column, 10, 10), 20)
             | LShiftU64 (BitFieldRead64 (Row, 4, 6), 21) | LShiftU64 (BitFieldRead64 (Column, 6, 9), 24) | LShiftU64 (BitFieldRead64 (Row, 11, 11), 28)
             | LShiftU64 (BitFieldRead64 (Row, 13, 13), 29) | LShiftU64 (BitFieldRead64 (Row, 14, 14), 30) | LShiftU64 (BitFieldRead64 (Row, 3, 3), 31);

  //RA32  W/SID - R15
  RaTemp |= LShiftU64 (BitFieldRead64 (Row, 15, 15), 32 + SidNum);

  if (Ra != NULL) {
    *Ra = RaTemp;
  }
  return TRUE;
}



/**
 this is rank reverse decode for DDR5 x4 8Gb open page mode - int.

 @param[in, out]       Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- column

 @ret none

**/
BOOLEAN
EFIAPI
Ddr5OpenPg8GbX4IntReverse (
  IN OUT UINT64         *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT8              BankTemp;
  UINT64             RaTemp;
  UINT8              BankXor;
  UINT8              SidTemp;

  if (CheckDimmParameter (Row, Column, 15, 10) == FALSE) {
    return FALSE;
  }
  BankTemp = Bank;
  if (BankXorEnable) {
    //
    // R9~11 - BankXor0~2, R13~14 - BankXor3~4
    //
    BankXor =  (UINT8)(BitFieldRead64(Row, 9, 11) | LShiftU64 (BitFieldRead64 (Row, 13, 14), 3));
    BankTemp = DoBankXorReverse (Bank, BankXor);
  }

  //
  //RA12 - BG0  RA16~17 - BG1~2  RA18 - BA0
  //
  RaTemp =  LShiftU64 (BitFieldRead64 (BankTemp, 2, 2), 12) | LShiftU64 (BitFieldRead64 (BankTemp, 3,4), 16) | LShiftU64 (BitFieldRead64 (BankTemp, 0,0), 18);


  if (SidNum != 0) {
    SidTemp = SubRank;

    if (SidXorEnable) {
      SidTemp = SubRank ^ ((UINT8)BitFieldRead64 (Row, 0, 2));
    }
    //
    //RA14 - SID0  RA15 - SID1  RA13 - SID2   RA32~34 - R0~2 if SID.
    //
    if (SidNum == 1) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 14) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13) | LShiftU64 (BitFieldRead64 (Row, 0, 0), 32);
    }

    if (SidNum == 2) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 14) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13)  | LShiftU64 (BitFieldRead64 (Row, 0, 1), 32) ;
    }

    if (SidNum == 3) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 14) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (SidTemp, 2, 2), 13)  | LShiftU64 (BitFieldRead64 (Row, 0, 2), 32);
    }
  } else {
    RaTemp |= LShiftU64 (BitFieldRead64 (Row, 0, 0), 14) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13);
  }


  //
  //RA2~5 - C0~3, RA6 - C9,  RA7~11 - C4~8，  RA19 - R12, RA20 - C10, RA21~28- R4~11, RA29 - R13, RA30 - R14, RA31 - R3
  //
  RaTemp |=  LShiftU64 (BitFieldRead64 (Column, 0, 3), 2) | LShiftU64 (BitFieldRead64 (Column, 9, 9), 6) | LShiftU64 (BitFieldRead64 (Column, 4, 8), 7)
             | LShiftU64 (BitFieldRead64 (Row, 12, 12), 19) | LShiftU64 (BitFieldRead64 (Column, 10, 10), 20) | LShiftU64 (BitFieldRead64 (Row, 4, 11), 21)
             | LShiftU64 (BitFieldRead64 (Row, 13, 13), 29) | LShiftU64 (BitFieldRead64 (Row, 14, 14), 30) | LShiftU64 (BitFieldRead64 (Row, 3, 3), 31);

  //RA32  W/SID - R15
  RaTemp |= LShiftU64 (BitFieldRead64 (Row, 15, 15), 32 + SidNum);

  if (Ra != NULL) {
    *Ra = RaTemp;
  }
  return TRUE;
}



/**
 this is rank reverse decode for DDR5 x4 8Gb open page INT2

 @param[in, out]       Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- column

 @ret none

**/
BOOLEAN
EFIAPI
Ddr5OpenPg8GbX4Int2Reverse (
  IN OUT UINT64         *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT8              BankTemp;
  UINT64             RaTemp;
  UINT8              BankXor;
  UINT8              SidTemp;

  if (CheckDimmParameter (Row, Column, 15, 10) == FALSE) {
    return FALSE;
  }
  BankTemp = Bank;
  if (BankXorEnable) {
    //
    // R9~11 - BankXor0~2, R13~14 - BankXor3~4
    //
    BankXor =  (UINT8)(BitFieldRead64(Row, 9, 11) | LShiftU64 (BitFieldRead64 (Row, 13, 14), 3));
    BankTemp = DoBankXorReverse (Bank, BankXor);
  }

  //
  //RA7 - BG0  RA16~17 - BG1~2  RA18~ - BA0
  //
  RaTemp =  LShiftU64 (BitFieldRead64 (BankTemp, 2, 2), 7) | LShiftU64 (BitFieldRead64 (BankTemp, 3,4), 16) | LShiftU64 (BitFieldRead64 (BankTemp, 0,0), 18);


  if (SidNum != 0) {
    SidTemp = SubRank;

    if (SidXorEnable) {
      SidTemp = SubRank ^ ((UINT8)BitFieldRead64 (Row, 0, 2));
    }
    //
    //RA14 - SID0  RA15 - SID1  RA13 - SID2   RA32~34 - R0~2 if SID.
    //
    if (SidNum == 1) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 14) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13) | LShiftU64 (BitFieldRead64 (Row, 0, 0), 32);
    }

    if (SidNum == 2) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 14) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13)  | LShiftU64 (BitFieldRead64 (Row, 0, 1), 32) ;
    }

    if (SidNum == 3) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 14) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (SidTemp, 2, 2), 13)  | LShiftU64 (BitFieldRead64 (Row, 0, 2), 32);
    }
  } else {
    RaTemp |= LShiftU64 (BitFieldRead64 (Row, 0, 0), 14) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13);
  }


  //
  //RA2~6 - C0~4, RA8~12 - C5~9, RA19 - R12, RA20 - C10, RA21~28- R4~11, RA29 - R13, RA30 - R14, RA31 - R3
  //
  RaTemp |=  LShiftU64 (BitFieldRead64 (Column, 0, 4), 2) | LShiftU64 (BitFieldRead64 (Column, 5, 9), 8) | LShiftU64 (BitFieldRead64 (Row, 12, 12), 19)
             | LShiftU64 (BitFieldRead64 (Column, 10, 10), 20) | LShiftU64 (BitFieldRead64 (Row, 4, 11), 21) | LShiftU64 (BitFieldRead64 (Row, 13, 13), 29)
             | LShiftU64 (BitFieldRead64 (Row, 14, 14), 30) | LShiftU64 (BitFieldRead64 (Row, 3, 3), 31);

  //RA32  W/SID - R15
  RaTemp |= LShiftU64 (BitFieldRead64 (Row, 15, 15), 32 + SidNum);

  if (Ra != NULL) {
    *Ra = RaTemp;
  }
  return TRUE;
}




/**
 this is rank reverse decode for DDR5 x4 8Gb open page INT8

 @param[in, out]       Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- column
 @ret none

**/
BOOLEAN
EFIAPI
Ddr5OpenPg8GbX4Int8Reverse (
  IN OUT UINT64         *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT8              BankTemp;
  UINT64             RaTemp;
  UINT8              BankXor;
  UINT8              SidTemp;

  if (CheckDimmParameter (Row, Column, 15, 10) == FALSE) {
    return FALSE;
  }

  BankTemp = Bank;
  if (BankXorEnable) {
    //
    // R9~11 - BankXor0~2, R13~14 - BankXor3~4
    //
    BankXor =  (UINT8)(BitFieldRead64(Row, 9, 11) | LShiftU64 (BitFieldRead64 (Row, 13, 14), 3));
    BankTemp = DoBankXorReverse (Bank, BankXor);
  }

  //
  //RA6~8 - BG0~2  RA18 - BA0
  //
  RaTemp =  LShiftU64 (BitFieldRead64 (BankTemp, 2, 4), 6) | LShiftU64 (BitFieldRead64 (BankTemp, 0, 0), 18);


  if (SidNum != 0) {
    SidTemp = SubRank;

    if (SidXorEnable) {
      SidTemp = SubRank ^ ((UINT8)BitFieldRead64 (Row, 0, 2));
    }
    //
    //RA17 - SID0  RA15 - SID1  RA16 - SID2   RA32~34 - R0~2 if SID.
    //
    if (SidNum == 1) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 17) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 16) | LShiftU64 (BitFieldRead64 (Row, 0, 0), 32);
    }

    if (SidNum == 2) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 17) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 16)  | LShiftU64 (BitFieldRead64 (Row, 0, 1), 32) ;
    }

    if (SidNum == 3) {
      RaTemp |= LShiftU64 (BitFieldRead64 (SidTemp, 0, 0), 17) | LShiftU64 (BitFieldRead64 (SidTemp, 1, 1), 15) | LShiftU64 (BitFieldRead64 (SidTemp, 2, 2), 16)  | LShiftU64 (BitFieldRead64 (Row, 0, 2), 32);
    }
  } else {
    RaTemp |= LShiftU64 (BitFieldRead64 (Row, 0, 0), 17) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15) | LShiftU64 (BitFieldRead64 (Row, 2, 2), 16);
  }


  //
  //RA2~5 - C0~3, RA9~12 - C6~9, RA13 - C5, RA14- C4, RA19 - R12， RA20 - C10, RA21~28- R4~11, RA29 - R13, RA30 - R14, RA31 - R3
  //
  RaTemp |=  LShiftU64 (BitFieldRead64 (Column, 0, 3), 2) | LShiftU64 (BitFieldRead64 (Column, 6, 9), 9) | LShiftU64 (BitFieldRead64 (Column, 5, 5), 13)
             | LShiftU64 (BitFieldRead64 (Column, 4, 4), 14) | LShiftU64 (BitFieldRead64 (Row, 12, 12), 19) | LShiftU64 (BitFieldRead64 (Column, 10, 10), 20)
             | LShiftU64 (BitFieldRead64 (Row, 4, 11), 21) | LShiftU64 (BitFieldRead64 (Row, 13, 13), 29) | LShiftU64 (BitFieldRead64 (Row, 14, 14), 30)
             | LShiftU64 (BitFieldRead64 (Row, 3, 3), 31);

  //RA32  W/SID - R15
  RaTemp |= LShiftU64 (BitFieldRead64 (Row, 15, 15), 32 + SidNum);

  if (Ra != NULL) {
    *Ra = RaTemp;
  }
  return TRUE;
}


/**
 this is rank reverse decode for DDR5 x16 16/24/32/64Gb close page mode.

 @param[in, out]       Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- column

 @ret none

**/
BOOLEAN
EFIAPI
Ddr5ClosePgMostX16Reverse (
  IN OUT UINT64         *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT64             RaTemp;
  if (CheckDimmParameter (Row, Column, 17, 9) == FALSE) {
    return FALSE;
  }
  //
  //RA6~7 - BG0~1   RA9~10 - BA0~1
  //
  RaTemp =  LShiftU64 (BitFieldRead64 (Bank, 2, 3), 6) | LShiftU64 (BitFieldRead64 (Bank, 0, 1), 9);

  //
  //RA2~5-C0~3, RA8 - R12, RA11~13 - R7~9, RA14 - C4， RA15~16 - R1~2, RA17 - R0， RA18 - R10， RA19 - C5， RA20~23 - R3~R6， RA24~27 - C6-C9， RA28 - R11， RA29~33 - R13~17
  RaTemp |= LShiftU64 (BitFieldRead64 (Column, 0, 3), 2) | LShiftU64 (BitFieldRead64 (Row, 12, 12), 8) | LShiftU64 (BitFieldRead64 (Row, 7, 9), 11)
            | LShiftU64 (BitFieldRead64 (Column, 4, 4), 14) | LShiftU64 (BitFieldRead64 (Row, 1, 2), 15) | LShiftU64 (BitFieldRead64 (Row, 0, 0), 17)
            | LShiftU64 (BitFieldRead64 (Row, 10, 10), 18) | LShiftU64 (BitFieldRead64 (Column, 5, 5), 19) | LShiftU64 (BitFieldRead64 (Row, 3, 6), 20)
            | LShiftU64 (BitFieldRead64 (Column, 6, 9), 24) | LShiftU64 (BitFieldRead64 (Row, 11, 11), 28) | LShiftU64 (BitFieldRead64 (Row, 13, 17), 29);

  if (Ra != NULL) {
    *Ra = RaTemp;
  }
  return TRUE;
}



/**
 this is rank reverse decode for DDR5  x16 16/24/32/64Gb open page mode - int.

 @param[in, out]       Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- column

 @ret none

**/
BOOLEAN
EFIAPI
Ddr5OpenPgMostX16IntReverse (
  IN OUT UINT64         *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT64             RaTemp;

  if (CheckDimmParameter (Row, Column, 17, 9) == FALSE) {
    return FALSE;
  }
  //
  //RA12 - BG0   RA16 - BG1  RA18~19 - BA0~1
  //
  RaTemp =  LShiftU64 (BitFieldRead64 (Bank, 2, 2), 12) | LShiftU64 (BitFieldRead64 (Bank, 3, 3), 16) | LShiftU64 (BitFieldRead64 (Bank, 0, 1), 18);

  //
  //RA2~5-C0~3, RA6 - C9, RA7~11 - C4~8, RA13 - R2, RA14 - R0, RA15 - R1, RA17 - R12, RA20~28 - R3~11, RA29~33 - R13~17
  RaTemp |= LShiftU64 (BitFieldRead64 (Column, 0, 3), 2) | LShiftU64 (BitFieldRead64 (Column, 9, 9), 6) | LShiftU64 (BitFieldRead64 (Column, 4, 8), 7)
            | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13) | LShiftU64 (BitFieldRead64 (Row, 0, 0), 14) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15)
            | LShiftU64 (BitFieldRead64 (Row, 12, 12), 17) | LShiftU64 (BitFieldRead64 (Row, 3, 11), 20) | LShiftU64 (BitFieldRead64 (Row, 13, 17), 29);

  if (Ra != NULL) {
    *Ra = RaTemp;
  }
  return TRUE;
}



/**
 this is rank reverse decode for DDR5  x16 16/24/32/64Gb open page INT2

 @param[in, out]       Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- column

 @ret none

**/
BOOLEAN
EFIAPI
Ddr5OpenPgMostX16Int2Reverse (
  IN OUT UINT64         *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT64             RaTemp;

  if (CheckDimmParameter (Row, Column, 17, 9) == FALSE) {
    return FALSE;
  }
  //
  //RA7 - BG0   RA16 - BG1  RA18~19 - BA0~1
  //
  RaTemp =  LShiftU64 (BitFieldRead64 (Bank, 2, 2), 7) | LShiftU64 (BitFieldRead64 (Bank, 3, 3), 16) | LShiftU64 (BitFieldRead64 (Bank, 0, 1), 18);

  //
  //RA2~6 - C0~4,  RA8~12 - C5~9, RA13 - R2, RA14 - R0, RA15 - R1, RA17 - R12, RA20~28 - R3~11, RA29~33 - R13~17
  RaTemp |= LShiftU64 (BitFieldRead64 (Column, 0, 4), 2) | LShiftU64 (BitFieldRead64 (Column, 5, 9), 8)
            | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13) | LShiftU64 (BitFieldRead64 (Row, 0, 0), 14) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15)
            | LShiftU64 (BitFieldRead64 (Row, 12, 12), 17) | LShiftU64 (BitFieldRead64 (Row, 3, 11), 20) | LShiftU64 (BitFieldRead64 (Row, 13, 17), 29);

  if (Ra != NULL) {
    *Ra = RaTemp;
  }
  return TRUE;
}




/**
 this is rank reverse decode for DDR5  x16 16/24/32/64Gb open page INT4

 @param[in, out]       Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- column

 @ret none

**/
BOOLEAN
EFIAPI
Ddr5OpenPgMostX16Int4Reverse (
  IN OUT UINT64         *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT64             RaTemp;
  if (CheckDimmParameter (Row, Column, 17, 9) == FALSE) {
    return FALSE;
  }
  //
  //RA6~7 - BG0~1  RA18~19 - BA0~1
  //
  RaTemp =  LShiftU64 (BitFieldRead64 (Bank, 2, 3), 6) | LShiftU64 (BitFieldRead64 (Bank, 0, 1), 18);

  //
  //RA2~5 - C0~3,  RA8 - R12,  RA9~12 - C6~9, RA13 - C5, RA14 - C4,  RA15~16 - R1~2,  RA17 - R0,  RA20~28 - R3~11, RA29~33 - R13~17
  RaTemp |= LShiftU64 (BitFieldRead64 (Column, 0, 3), 2) | LShiftU64 (BitFieldRead64 (Row, 12, 12), 8) | LShiftU64 (BitFieldRead64 (Row, 6, 9), 9)
            | LShiftU64 (BitFieldRead64 (Column, 5, 5), 13) | LShiftU64 (BitFieldRead64 (Column, 4, 4), 14) | LShiftU64 (BitFieldRead64 (Row, 1, 2), 15)
            | LShiftU64 (BitFieldRead64 (Row, 0, 0), 17) | LShiftU64 (BitFieldRead64 (Row, 3, 11), 20) | LShiftU64 (BitFieldRead64 (Row, 13, 17), 29);

  if (Ra != NULL) {
    *Ra = RaTemp;
  }
  return TRUE;
}



/**
 this is rank reverse decode for DDR5 x16 8Gb close page mode.

 @param[in, out]       Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- column

 @ret none

**/
BOOLEAN
EFIAPI
Ddr5ClosePg8GbX16Reverse (
  IN OUT UINT64         *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT64             RaTemp;
  if (CheckDimmParameter (Row, Column, 17, 9) == FALSE) {
    return FALSE;
  }
  //
  //RA6~7 - BG0~1   RA9 - BA0
  //
  RaTemp =  LShiftU64 (BitFieldRead64 (Bank, 2, 3), 6) | LShiftU64 (BitFieldRead64 (Bank, 0, 0), 9);

  //
  //RA2~5-C0~3, RA8 - R12, RA10 - R13,  RA11~13 - R7~9, RA14 - C4， RA15~16 - R1~2, RA17 - R0， RA18 - R10， RA19 - C5， RA20~23 - R3~6， RA24~27 - C6~9， RA28 - R11， RA29~32 - R14~17
  RaTemp |= LShiftU64 (BitFieldRead64 (Column, 0, 3), 2) | LShiftU64 (BitFieldRead64 (Row, 12, 12), 8) | LShiftU64 (BitFieldRead64 (Row, 13, 13), 10)
            | LShiftU64 (BitFieldRead64 (Row, 7, 9), 11) | LShiftU64 (BitFieldRead64 (Column, 4, 4), 14) | LShiftU64 (BitFieldRead64 (Row, 1, 2), 15)
            | LShiftU64 (BitFieldRead64 (Row, 0, 0), 17) | LShiftU64 (BitFieldRead64 (Row, 10, 10), 18) | LShiftU64 (BitFieldRead64 (Column, 5, 5), 19)
            | LShiftU64 (BitFieldRead64 (Row, 3, 6), 20) | LShiftU64 (BitFieldRead64 (Column, 6, 9), 24) | LShiftU64 (BitFieldRead64 (Row, 11, 11), 28)
            | LShiftU64 (BitFieldRead64 (Row, 14, 17), 29);

  if (Ra != NULL) {
    *Ra = RaTemp;
  }
  return TRUE;
}



/**
 this is rank reverse decode for DDR5  x16 8Gb open page mode - int.

 @param[in, out]       Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- column

 @ret none

**/
BOOLEAN
EFIAPI
Ddr5OpenPg8GbX16IntReverse (
  IN OUT UINT64         *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT64             RaTemp;
  if (CheckDimmParameter (Row, Column, 17, 9) == FALSE) {
    return FALSE;
  }
  //
  //RA12 - BG0   RA16 - BG1  RA18 - BA0
  //
  RaTemp =  LShiftU64 (BitFieldRead64 (Bank, 2, 2), 12) | LShiftU64 (BitFieldRead64 (Bank, 3, 3), 16) | LShiftU64 (BitFieldRead64 (Bank, 0, 0), 18);

  //
  //RA2~5-C0~3, RA6 - C9, RA7~11 - C4~8, RA13 - R2, RA14 - R0, RA15 - R1, RA17 - R12,  RA19 - R13, RA20~28 - R3~11, RA29~32 - R14~17
  RaTemp |= LShiftU64 (BitFieldRead64 (Column, 0, 3), 2) | LShiftU64 (BitFieldRead64 (Column, 9, 9), 6) | LShiftU64 (BitFieldRead64 (Column, 4, 8), 7)
            | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13) | LShiftU64 (BitFieldRead64 (Row, 0, 0), 14) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15)
            | LShiftU64 (BitFieldRead64 (Row, 12, 12), 17) | LShiftU64 (BitFieldRead64 (Row, 13, 13), 19) | LShiftU64 (BitFieldRead64 (Row, 3, 11), 20)
            | LShiftU64 (BitFieldRead64 (Row, 14, 17), 29);

  if (Ra != NULL) {
    *Ra = RaTemp;
  }
  return TRUE;
}



/**
 this is rank reverse decode for DDR5  x16 8Gb open page INT2

 @param[in, out]       Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- column

 @ret none

**/
BOOLEAN
EFIAPI
Ddr5OpenPg8GbX16Int2Reverse (
  IN OUT UINT64         *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT64             RaTemp;
  if (CheckDimmParameter (Row, Column, 17, 9) == FALSE) {
    return FALSE;
  }
  //
  //RA7 - BG0   RA16 - BG1  RA18 - BA0
  //
  RaTemp =  LShiftU64 (BitFieldRead64 (Bank, 2, 2), 7) | LShiftU64 (BitFieldRead64 (Bank, 3, 3), 16) | LShiftU64 (BitFieldRead64 (Bank, 0, 0), 18);

  //
  //RA2~6-C0~4, RA8~12 - C5~9, RA13 - R2, RA14 - R0, RA15 - R1, RA17 - R12,  RA19 - R13, RA20~28 - R3~11, RA29~32 - R14~17
  RaTemp |= LShiftU64 (BitFieldRead64 (Column, 0, 4), 2) | LShiftU64 (BitFieldRead64 (Column, 5, 9), 8)
            | LShiftU64 (BitFieldRead64 (Row, 2, 2), 13) | LShiftU64 (BitFieldRead64 (Row, 0, 0), 14) | LShiftU64 (BitFieldRead64 (Row, 1, 1), 15)
            | LShiftU64 (BitFieldRead64 (Row, 12, 12), 17) | LShiftU64 (BitFieldRead64 (Row, 13, 13), 19) | LShiftU64 (BitFieldRead64 (Row, 3, 11), 20)
            | LShiftU64 (BitFieldRead64 (Row, 14, 17), 29);

  if (Ra != NULL) {
    *Ra = RaTemp;
  }
  return TRUE;
}




/**
 this is rank reverse decode for DDR5  x16 8Gb open page INT4

 @param[in, out]       Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- column

 @ret none

**/
BOOLEAN
EFIAPI
Ddr5OpenPg8GbX16Int4Reverse (
  IN OUT UINT64         *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT64             RaTemp;
  if (CheckDimmParameter (Row, Column, 17, 9) == FALSE) {
    return FALSE;
  }
  //
  //RA6~7 - BG0~1  RA18 - BA0
  //
  RaTemp =  LShiftU64 (BitFieldRead64 (Bank, 2, 3), 6) | LShiftU64 (BitFieldRead64 (Bank, 0, 0), 18);

  //
  //RA2~5-C0~3, RA8 - R12, RA9~12 - C6~9, RA13 - C5, RA14 - C4, RA15~16 - R1~2, RA17 - R0,  RA19 - R13, RA20~28 - R3~11, RA29~32 - R14~17
  //
  RaTemp |= LShiftU64 (BitFieldRead64 (Column, 0, 3), 2) | LShiftU64 (BitFieldRead64 (Row, 12, 12), 8) | LShiftU64 (BitFieldRead64 (Column, 6, 9), 9)
            | LShiftU64 (BitFieldRead64 (Column, 5, 5), 13) | LShiftU64 (BitFieldRead64 (Column, 4, 4), 14) | LShiftU64 (BitFieldRead64 (Row, 1, 2), 15)
            | LShiftU64 (BitFieldRead64 (Row, 0, 0), 17) | LShiftU64 (BitFieldRead64 (Row, 13, 13), 19) | LShiftU64 (BitFieldRead64 (Row, 3, 11), 20)
            | LShiftU64 (BitFieldRead64 (Row, 14, 17), 29);

  if (Ra != NULL) {
    *Ra = RaTemp;
  }
  return TRUE;
}


/**
 this is rank reverse decode for HBM cgbg0

 @param[in, out]       Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- column

 @ret none

**/
BOOLEAN
EFIAPI
HbmCgbg0Reverse (
  IN OUT UINT64         *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT64             RaTemp;

  if (CheckDimmParameter (Row, Column, 14, 5) == FALSE) {
    return FALSE;
  }
  //
  //RA6~7 - BG0~1   RA8~9 - BA0~1
  //
  RaTemp = LShiftU64 (BitFieldRead64 (Bank, 2, 3), 6) | LShiftU64 (BitFieldRead64 (Bank, 0, 1), 8);

  if (SidNum == 0) {
    //
    //RA4~5 - C0~1, RA10~13 - R0~3,  RA14 - C2, RA15~18 - R4~7, RA19 - C3, RA20~22 - R8~R10,  RA23~24 - C4~5,  RA25~28 - R11~14
    //
    RaTemp |= LShiftU64 (BitFieldRead64 (Column, 0, 1), 4) | LShiftU64 (BitFieldRead64 (Row, 0, 3), 10) | LShiftU64 (BitFieldRead64 (Column, 2, 2), 14)
              | LShiftU64 (BitFieldRead64 (Row, 4, 7), 15) | LShiftU64 (BitFieldRead64 (Column, 3, 3), 19) | LShiftU64 (BitFieldRead64 (Row, 8, 10), 20)
              | LShiftU64 (BitFieldRead64 (Column, 4, 5), 23) | LShiftU64 (BitFieldRead64 (Row, 11, 14), 25);

  }

  if (SidNum == 1) {
    //
    //RA4~5 - C0~1, RA10 - SID0, RA11~13 - R0~2,  RA14 - C2, RA15~18 - R3~6, RA19 - C3, RA20~22 - R7~9,  RA23~24 - C4~5,  RA25~29 - R10~14
    //
    RaTemp |= LShiftU64 (BitFieldRead64 (Column, 0, 1), 4) | LShiftU64 (BitFieldRead64 (SubRank, 0, 0), 10) | LShiftU64 (BitFieldRead64 (Row, 0, 2), 11)
              | LShiftU64 (BitFieldRead64 (Column, 2, 2), 14) | LShiftU64 (BitFieldRead64 (Row, 3, 6), 15) | LShiftU64 (BitFieldRead64 (Column, 3, 3), 19)
              | LShiftU64 (BitFieldRead64 (Row, 7, 9), 20) | LShiftU64 (BitFieldRead64 (Column, 4, 5), 23) | LShiftU64 (BitFieldRead64 (Row, 10, 14), 25);
  }

  if (Ra != NULL) {
    *Ra = RaTemp;
  }
  return TRUE;
}



/**
 this is rank reverse decode for HBM cgbg1

 @param[in, out]       Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- column

 @ret none

**/
BOOLEAN
EFIAPI
HbmCgbg1Reverse (
  IN OUT UINT64         *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT64             RaTemp;
  if (CheckDimmParameter (Row, Column, 14, 5) == FALSE) {
    return FALSE;
  }
  //
  //RA7 - BG0   RA10 - BG1  RA11 - BA0  RA13 - BA1
  //
  RaTemp = LShiftU64 (BitFieldRead64 (Bank, 2, 2), 7) | LShiftU64 (BitFieldRead64 (Bank, 3, 3), 10) | LShiftU64 (BitFieldRead64 (Bank, 0, 0), 11)
           | LShiftU64 (BitFieldRead64 (Bank, 1, 1), 13);

  if (SidNum == 0) {
    //
    //RA4~6 - C0~2, RA8~9 - C3~4,  RA12 - C5, RA14~28 - R0~14
    //
    RaTemp |= LShiftU64 (BitFieldRead64 (Column, 0, 2), 4) | LShiftU64 (BitFieldRead64 (Column, 3, 4), 8) | LShiftU64 (BitFieldRead64 (Column, 5, 5), 12)
              | LShiftU64 (BitFieldRead64 (Row, 0, 14), 14);

  }

  if (SidNum == 1) {
    //
    //RA4~6 - C0~2, RA8~9 - C3~4, RA12 - C5,  RA14 - SID0, RA15~29 - R0~14
    //
    RaTemp |= LShiftU64 (BitFieldRead64 (Column, 0, 2), 4) | LShiftU64 (BitFieldRead64 (Column, 3, 4), 8) | LShiftU64 (BitFieldRead64 (Column, 5, 5), 12)
              | LShiftU64 (BitFieldRead64 (SubRank, 0, 0), 14) | LShiftU64 (BitFieldRead64 (Row, 0, 14), 15);
  }

  if (Ra != NULL) {
    *Ra = RaTemp;
  }
  return TRUE;
}




/**
 this is rank reverse decode for HBM cgbg2

 @param[in, out]       Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- column

 @ret none

**/
BOOLEAN
EFIAPI
HbmCgbg2Reverse (
  IN OUT UINT64         *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT64             RaTemp;
  if (CheckDimmParameter (Row, Column, 14, 5) == FALSE) {
    return FALSE;
  }
  //
  //RA8 - BG0   RA10 - BG1  RA11 - BA0  RA13 - BA1
  //
  RaTemp = LShiftU64 (BitFieldRead64 (Bank, 2, 2), 8) | LShiftU64 (BitFieldRead64 (Bank, 3, 3), 10) | LShiftU64 (BitFieldRead64 (Bank, 0, 0), 11)
           | LShiftU64 (BitFieldRead64 (Bank, 1, 1), 13);

  if (SidNum == 0) {
    //
    //RA4~7 - C0~3, RA9 - C4,  RA12 - C5, RA14~28 - R0~14
    //
    RaTemp |= LShiftU64 (BitFieldRead64 (Column, 0, 3), 4) | LShiftU64 (BitFieldRead64 (Column, 4, 4), 9) | LShiftU64 (BitFieldRead64 (Column, 5, 5), 12)
              | LShiftU64 (BitFieldRead64 (Row, 0, 14), 14);

  }

  if (SidNum == 1) {
    //
    //RA4~7 - C0~3, RA9 - C4, RA12 - C5,  RA14 - SID0, RA15~29 - R0~14
    //
    RaTemp |= LShiftU64 (BitFieldRead64 (Column, 0, 3), 4) | LShiftU64 (BitFieldRead64 (Column, 4, 4), 9) | LShiftU64 (BitFieldRead64 (Column, 5, 5), 12)
              | LShiftU64 (BitFieldRead64 (SubRank, 0, 0), 14) | LShiftU64 (BitFieldRead64 (Row, 0, 14), 15);
  }

  if (Ra != NULL) {
    *Ra = RaTemp;
  }
  return TRUE;
}




/**
 this is rank reverse decode for HBM cgbg3

 @param[in, out]       Ra   -- rank address
 @param[in]            BankXorEnable  -- bank xor enable
 @param[in]            SidXorEnable  -- sid xor enable
 @param[in]            SidNum  -- sid number
 @param[in]            SubRank  -- subrank
 @param[in]            Bank  -- bank
 @param[in]            Row -- row
 @param[in]            Column  -- column

 @ret none

**/
BOOLEAN
EFIAPI
HbmCgbg3Reverse (
  IN OUT UINT64         *Ra,
  IN     UINT8           BankXorEnable,
  IN     UINT8           SidXorEnable,
  IN     UINT8           SidNum,
  IN     UINT8           SubRank,
  IN     UINT8           Bank,
  IN     UINT32          Row,
  IN     UINT32          Column
  )
{
  UINT64             RaTemp;
  if (CheckDimmParameter (Row, Column, 14, 5) == FALSE) {
    return FALSE;
  }
  //
  //RA6 - BG0   RA11 - BG1  RA12~13 - BA0~1
  //
  RaTemp = LShiftU64 (BitFieldRead64 (Bank, 2, 2), 6) | LShiftU64 (BitFieldRead64 (Bank, 3, 3), 11) | LShiftU64 (BitFieldRead64 (Bank, 0, 1), 12);

  if (SidNum == 0) {
    //
    //RA4~5 - C0~1, RA7~10 - C2~5,  RA14~28 - R0~14
    //
    RaTemp |= LShiftU64 (BitFieldRead64 (Column, 0, 1), 4) | LShiftU64 (BitFieldRead64 (Column, 2, 5), 7) | LShiftU64 (BitFieldRead64 (Row, 0, 14), 14);
  }

  if (SidNum == 1) {
    //
    //RA4~5 - C0~1, RA7~10 - C2~5,  RA14 - SID0, RA15~29 - R0~14
    //
    RaTemp |= LShiftU64 (BitFieldRead64 (Column, 0, 1), 4) | LShiftU64 (BitFieldRead64 (Column, 2, 5), 7) | LShiftU64 (BitFieldRead64 (SubRank, 0, 0), 14)
              | LShiftU64 (BitFieldRead64 (Row, 0, 14), 15);
  }

  if (Ra != NULL) {
    *Ra = RaTemp;
  }
  return TRUE;
}



/**
 this is for AMAP forward reverse decode.

 @param[in, out]  Ra   -- rank address
 @param[in]       HbmType  -- indicate HBM type
 @param[in]       Width    -- indicate width
 @param[in]       Density  -- indicate density
 @param[in]       ClosePg  -- indicate close page
 @param[in]       Cgbg  -- indicate CGBG type
 @param[in]       BankXorEnable  -- bank xor enable
 @param[in]       SidXorEnable  -- sid xor enable
 @param[in]       SidNum  -- sid number
 @param[in]       SubRank  -- subrank
 @param[in]       Bank  -- bank
 @param[in]       Row -- row
 @param[in]       Column  -- column

 @ret TRUE -- reverse AMAP successfully.

**/
BOOLEAN
EFIAPI
ReverseAmap (
  IN OUT     UINT64     *Ra,
  IN         UINT8       HbmType,
  IN         UINT8       Width,
  IN         UINT8       Density,
  IN         UINT8       ClosePg,
  IN         UINT8       Cgbg,
  IN         UINT8       BankXorEnable,
  IN         UINT8       SidXorEnable,
  IN         UINT8       SidNum,
  IN         UINT8       SubRank,
  IN         UINT8       Bank,
  IN         UINT32      Row,
  IN         UINT32      Column
  )
{
  if (HbmType == TRUE ){
    if (ClosePg == TRUE) {
      return HbmCgbg0Reverse (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
    } else if (Cgbg ==  1) {
      //
      //CGBG_INT
      //
      return HbmCgbg1Reverse  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
    } else if (Cgbg ==  2) {
      //
      //FGBG_INT2
      //
      return HbmCgbg2Reverse (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
    } else if (Cgbg ==  3) {
      //
      //FGBG_INT8
      //
      return HbmCgbg3Reverse  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
    } else {
      DEBUG ((EFI_D_ERROR, "[ReverseAmap] HBM CGBG fail \n"));
    }
  } else {
    switch (Width) {
      case X8_WIDTH:
        if (Density == Density_8Gb) {
          if (ClosePg == TRUE) {
            return Ddr5ClosePg8GbX8Reverse (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  0) {
            //
            //CGBG_INT
            //
            return Ddr5OpenPg8GbX8IntReverse  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  1) {
            //
            //FGBG_INT2
            //
            return Ddr5OpenPg8GbX8Int2Reverse  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  2) {
            //
            //FGBG_INT8
            //
            return Ddr5OpenPg8GbX8Int8Reverse  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else {
            DEBUG ((EFI_D_ERROR, "[ReverseAmap] DDR5 X8 8Gb CGBG fail \n"));
          }
        } else {
          if (ClosePg == TRUE) {
            return Ddr5ClosePgMostX8Reverse (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  0) {
            //
            //CGBG_INT
            //
            return Ddr5OpenPgMostX8IntReverse  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  1) {
            //
            //FGBG_INT2
            //
            return Ddr5OpenPgMostX8Int2Reverse  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  2) {
            //
            //FGBG_INT8
            //
            return Ddr5OpenPgMostX8Int8Reverse  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else {
            DEBUG ((EFI_D_ERROR, "[ReverseAmap] DDR5 X8 16/24/32/64Gb CGBG fail \n"));
          }
        }
        break;
      case X4_WIDTH:
        if (Density == Density_8Gb) {
          if (ClosePg == TRUE) {
            return Ddr5ClosePg8GbX4Reverse (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  0) {
            //
            //CGBG_INT
            //
            return Ddr5OpenPg8GbX4IntReverse  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  1) {
            //
            //FGBG_INT2
            //
            return Ddr5OpenPg8GbX4Int2Reverse  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  2) {
            //
            //FGBG_INT8
            //
            return Ddr5OpenPg8GbX4Int8Reverse  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else {
            DEBUG ((EFI_D_ERROR, "[ReverseAmap] DDR5 X4 8Gb CGBG fail \n"));
          }
        } else {
          if (ClosePg == TRUE) {
            return Ddr5ClosePgMostX4Reverse (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  0) {
            //
            //CGBG_INT
            //
            return Ddr5OpenPgMostX4IntReverse  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  1) {
            //
            //FGBG_INT2
            //
            return Ddr5OpenPgMostX4Int2Reverse  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  2) {
            //
            //FGBG_INT8
            //
            return Ddr5OpenPgMostX4Int8Reverse  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else {
            DEBUG ((EFI_D_ERROR, "[ReverseAmap] DDR5 X4 16/24/32/64Gb CGBG fail \n"));
          }
        }
        break;
      case X16_WIDTH:
        if (Density == Density_8Gb) {
          if (ClosePg == TRUE) {
            return Ddr5ClosePg8GbX16Reverse (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  0) {
            //
            //CGBG_INT
            //
            return Ddr5OpenPg8GbX16IntReverse  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  1) {
            //
            //FGBG_INT2
            //
            return Ddr5OpenPg8GbX16Int2Reverse  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  2) {
            //
            //FGBG_INT4
            //
            return Ddr5OpenPg8GbX16Int4Reverse  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else {
            DEBUG ((EFI_D_ERROR, "[ReverseAmap] DDR5 X16 8Gb CGBG fail \n"));
          }
        } else {
          if (ClosePg == TRUE) {
            return Ddr5ClosePgMostX16Reverse (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  0) {
            //
            //CGBG_INT
            //
            return Ddr5OpenPgMostX16IntReverse  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  1) {
            //
            //FGBG_INT2
            //
            return Ddr5OpenPgMostX16Int2Reverse  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else if (Cgbg ==  2) {
            //
            //FGBG_INT4
            //
            return Ddr5OpenPgMostX16Int4Reverse  (Ra, BankXorEnable, SidXorEnable, SidNum, SubRank, Bank, Row, Column);
          } else {
            DEBUG ((EFI_D_ERROR, "[ReverseAmap] DDR5 X16 16/24/32/64Gb CGBG fail \n"));
          }
        }
        break;
      default:
        DEBUG ((EFI_D_ERROR, "[ReverseAmap] wrong DDR5 width\n"));
    }
  }
  return FALSE;
}
