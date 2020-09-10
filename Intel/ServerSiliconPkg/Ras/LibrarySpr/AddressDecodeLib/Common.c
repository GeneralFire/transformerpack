/** @file
  Implementation of CHA mmio decode.

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

extern UINT32                      mSocketPresentBitMap;
extern AD_SOCKET_INFO              *mSadTadScope;

/**
 This routine is to get socket channel.

 @param[in]   Mc  -- memory controller index
 @param[in]   McChn  -- chnanel in memory controller


 @ret  socket channel

**/
UINT8
EFIAPI
GetSktCh (
  IN    UINT8     Mc,
  IN    UINT8     McChn
  )
{
  return Mc * MAX_MC_CH + McChn;
}



/**
 This routine is to check whether mc is exist.

 @param[in]    Socket       --   socket index
 @param[in]    Mc           --   mc index

 @retval       TRUE-- this MC is exist.

**/
BOOLEAN
EFIAPI
IsMcExist (
  IN      UINT8    Socket,
  IN      UINT8    Mc
  )
{
  if (mSadTadScope[Socket].McInfo[Mc].McEnabled == TRUE) {
    return TRUE;
  } else {
    return FALSE;
  }

}


/**
 This routine is to check whether this chn is exist.

 @param[in]    Socket       --   socket index
 @param[in]    Mc           --   mc index
 @param[in]    Chn          --   chn index

 @retval       TRUE-- this MC is exist.
**/
BOOLEAN
EFIAPI
IsChannelExist (
  IN      UINT8    Socket,
  IN      UINT8    Mc,
  IN      UINT8    Chn
  )
{
  if (mSadTadScope[Socket].McInfo[Mc].ChInfo[Chn].ChEnabled == TRUE) {
    return TRUE;
  } else {
    return FALSE;
  }
}



/**
 This routine is to check whether this socket is exist


 @param[in]    Socket --  socket index

 @retval       TRUE -- this socket is presetn; FALSE --  this socket is non-exist.

**/
BOOLEAN
EFIAPI
IsSocketExist (
  IN   UINT8     Socket
  )
{
  if (mSocketPresentBitMap & (1 << Socket)) {
    return TRUE;
  } else {
    return FALSE;
  }

}


/**
  Returns a bit field from a 64-bit value with bitfield removed.

  Returns value with the bitfield specified by the StartBit and the EndBit  removed.


  If StartBit is greater than 63, then ASSERT().
  If EndBit is greater than 63, then ASSERT().
  If EndBit is less than StartBit, then ASSERT().

  @param  Operand   Operand on which to perform the bitfield operation.
  @param  StartBit  The ordinal of the least significant bit in the bit field.
                    Range 0..63.
  @param  EndBit    The ordinal of the most significant bit in the bit field.
                    Range 0..63.

  @return a bit field from a 64-bit value with bitfield removed.

**/
UINT64
EFIAPI
BitFieldRemove64 (
  IN      UINT64                    Operand,
  IN      UINTN                     StartBit,
  IN      UINTN                     EndBit
  )
{
  ASSERT (EndBit < 64);
  ASSERT (StartBit <= EndBit);
  return BitFieldRead64 (Operand, 0, StartBit - 1) |  LShiftU64 (BitFieldRead64 (Operand, EndBit + 1, 63), StartBit);
}


/**
  divive operand from particular bits, and return value after divide. 

  divive operand from particular bits, and return value after divide. 

  If 64-bit operations are not supported, then ASSERT().
  If StartBit is greater than 63, then ASSERT().


  @param  Operand   Operand on which to perform the bitfield operation.
  @param  StartBit  The ordinal of the least significant bit in the bit field.
                    Range 0..63.
  @param  Divisor   divisor
  @param  Remainer  remain value after divive.

  @return  value after divide

**/
UINT64
EFIAPI
BitFieldDiv64 (
  IN      UINT64                    Operand,
  IN      UINTN                     StartBit,
  IN      UINT32                    Divisor,
  IN OUT  UINT32                   *Remainer
  )
{
  UINT64              LowBits;
  UINT64              Temp;

  ASSERT (StartBit < 64);

  LowBits = BitFieldRead64 (Operand, 0, StartBit - 1);
  Temp = BitFieldRead64 (Operand, StartBit, 63);

  if (Remainer != NULL) {
    *Remainer = ModU64x32 (Operand, Divisor);
  }

  return LShiftU64 (DivU64x32 (Operand, Divisor), StartBit) | LowBits;

}



/**
  Insert data at range started from StartBit and end at EndBit.

  If 64-bit operations are not supported, then ASSERT().
  If StartBit is greater than 63, then ASSERT().

  @param  Operand   Operand on which to perform the bitfield operation.
  @param  StartBit  The ordinal of the least significant bit in the bit field.
                    Range 0..63.
  @param  EndBit    End bit index of the least significant.
  @param  InsertValue    insert data.

  @return  value after divide

**/
UINT64
EFIAPI
BitFieldInsert64 (
  IN      UINT64                    Operand,
  IN      UINTN                     StartBit,
  IN      UINTN                     EndBit,
  IN      UINT32                    InsertValue
  )
{
  UINT64              Temp;

  ASSERT (StartBit < 64);
  ASSERT (EndBit >= StartBit);

  Temp = BitFieldRead64 (Operand, 0, StartBit - 1) | LShiftU64 ( BitFieldRead64 (Operand, StartBit, 63), EndBit + 1);
  Temp |=  LShiftU64 ( BitFieldRead64 (InsertValue, 0, EndBit - StartBit),  StartBit);

  return Temp;
}
