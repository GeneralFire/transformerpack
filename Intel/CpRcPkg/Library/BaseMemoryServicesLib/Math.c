/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2017 Intel Corporation. <BR>

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

#include <Library/MemoryCoreLib.h>


/**

  Finds greatest common denominator

  @param x - First Value
  @param y - Second Value

  @retval Greatest common denominator

**/
UINT32
GetGCD (
       UINT32 x,
       UINT32 y
       )
{
  UINT32 t;

  do {
    if (x < y) {
      t = x;
      x = y;
      y = t;
    }
    x -= y;
  } while (x);

  return y;
}

/**

  Multiplies two 32 bit values

  @param dw1 - first 32 bit value
  @param dw2 - second 32 bit value

  @retval 64 bit value resulting from the multiplication of two given values

**/
UINT64_STRUCT
MulDw64 (
        UINT32 dw1,
        UINT32 dw2
        )
{
  UINT64_STRUCT RetVal;
  UINT64 Value;

  Value = MultU64x32 (dw1, dw2);
  RetVal.lo = (UINT32) Value;
  RetVal.hi = (UINT32) RShiftU64 (Value, 32);

  return RetVal;
}

/**

  divides 64 bit value by 2

  @param qw1 - 64 bit value to divide

  @retval result of qw1 / 2

**/
UINT64_STRUCT
DivBy2Qw64 (
           UINT64_STRUCT qw1
           )
{
  UINT64_STRUCT RetVal;
  UINT64 Value;

  Value = LShiftU64 (qw1.hi, 32) | qw1.lo;
  Value = RShiftU64 (Value, 1);
  RetVal.lo = (UINT32) Value;
  RetVal.hi = (UINT32) RShiftU64 (Value, 32);

  return RetVal;
}

/**

  This routine allows a 64 bit value to be left shifted by 32 bits and returns
  the shifted value. Count is valid up 63. (Only Bits 0-5 is valid for Count)

  @param Data  - value to be shifted
  @param Count - number of time to shift

  @retval Value shifted left identified by the Count.

**/
UINT64_STRUCT
LShiftUINT64 (
             UINT64_STRUCT Data,
             UINT32        Count
             )
{
  UINT64_STRUCT RetData;
  UINT64 Value;

  Value = LShiftU64 (Data.hi, 32) | Data.lo;
  Value = LShiftU64 (Value, Count);
  RetData.lo = (UINT32) Value;
  RetData.hi = (UINT32) RShiftU64 (Value, 32);

  return RetData;
}

/**

  This routine allows a 64 bit value to be multiplied with a 32 bit
  value returns 64bit result. No checking if the result is greater than 64bits.

  @param Multiplicand  - Multiplicand
  @param Multiplier    - what to multiply by

  @retval result of Multiplicand * Multiplier

**/
UINT64_STRUCT
MultUINT64x32 (
              UINT64_STRUCT Multiplicand,
              UINT32        Multiplier
              )
{
  UINT64_STRUCT RetData;
  UINT64 Value;

  Value = LShiftU64 (Multiplicand.hi, 32) | Multiplicand.lo;
  Value = MultU64x32 (Value,  Multiplier);
  RetData.lo = (UINT32) Value;
  RetData.hi = (UINT32) RShiftU64 (Value, 32);

  return RetData;
}

/**

  This routine allows a 64 bit value to be divided with a 32 bit value returns
  64bit result and the Remainder.

  @param Dividend  - Value to divide into
  @param Divisor   - Value to divide by
  @param Remainder - Pointer to location to write remainder

  @retval Remainder of Dividend / Divisor

**/
UINT64_STRUCT
DivUINT64x32 (
             UINT64_STRUCT Dividend,
             UINT32        Divisor,
             UINT32        *Remainder
             )
{
  UINT64 Value;

  Value = LShiftU64 (Dividend.hi, 32) | Dividend.lo;
  Value = DivU64x32Remainder (Value, Divisor, Remainder);
  Dividend.lo = (UINT32) Value;
  Dividend.hi = (UINT32) RShiftU64 (Value, 32);

  return Dividend;
}

/**

  Calculates the largest integer that is both
  a power of two and less than Input

  @param Input - value to calculate power of two

  @retval The largest integer that is both  a power of two and less than Input

**/
UINT64_STRUCT
GetPowerOfTwo (
              UINT64_STRUCT Input
              )
{
  UINT64_STRUCT RetData;
  UINT64 Value;

  Value = LShiftU64 (Input.hi, 32) | Input.lo;
  Value = GetPowerOfTwo64 (Value);
  RetData.lo = (UINT32) Value;
  RetData.hi = (UINT32) RShiftU64 (Value, 32);

  return RetData;
}

/**

  Calculates the largest integer that is both
  a power of two and less than or equal to input

  @param Input - value to calculate power of two

  @retval The largest integer that is both a power of two and less than or equal to Input

**/
UINT32
GetPowerOfTwox32 (
              UINT32 Input
              )
{
  return GetPowerOfTwo32 (Input);
}

/**

  This function calculates floor(log2(x)) + 1
  by finding the index of the most significant bit
  and adding 1

  @param Value     - Input Value

  @retval Returns floor(log2(x)) + 1 or 0, if the input was 0 or negative

**/
UINT8
Log2x32 (
        UINT32 Value
        )
{
  // Return 0 if Value is nonpositive, HighBitSet32() could return -1 otherwise
  if ((INT32)Value <= 0) {
    return 0;
  }

  return (UINT8) HighBitSet32 (Value);
}

/**

  This routine allows a 64 bit value to be right shifted by 32 bits and returns the
  shifted value. Count is valid up 63. (Only Bits 0-5 is valid for Count)

  @param Operand - Value to be shifted
  @param Count   - Number of times to shift right.

  @retval Value shifted right identified by the Count.

**/
UINT64_STRUCT
RShiftUINT64 (
             UINT64_STRUCT Operand,
             UINT32        Count
             )
{
  UINT64_STRUCT RetData;
  UINT64 Value;

  Value = LShiftU64 (Operand.hi, 32) | Operand.lo;
  Value = RShiftU64 (Value, Count);

  RetData.lo = (UINT32) Value;
  RetData.hi = (UINT32) RShiftU64 (Value, 32);

  return RetData;
}

/**

  Adds two UINT64 values

  @param Operand1  - first UINT64 to add
  @param Operand2  - second UINT64 to add

  @retval sum of operand1 and operand2

**/
UINT64_STRUCT
AddUINT64 (
          UINT64_STRUCT Operand1,
          UINT64_STRUCT Operand2
          )
{
  UINT64_STRUCT RetData;
  UINT64 Value1;
  UINT64 Value2;

  Value1 = LShiftU64 (Operand1.hi, 32) | Operand1.lo;
  Value2 = LShiftU64 (Operand2.hi, 32) | Operand2.lo;
  Value1 = Value1 + Value2;
  RetData.lo = (UINT32) Value1;
  RetData.hi = (UINT32) RShiftU64 (Value1, 32);

  return RetData;
}

/**

  Add UINT64 and UINT32

  @param Operand1  - UINT64 to add
  @param Operand2  - UINT32 to add

  @retval sum of operand1 and operand2

**/
UINT64_STRUCT
AddUINT64x32 (
             UINT64_STRUCT Operand1,
             UINT32        Operand2
             )
{
  UINT64_STRUCT RetData;
  UINT64 Value1;

  Value1 = LShiftU64 (Operand1.hi, 32) | Operand1.lo;
  Value1 = Value1 + (UINT64) Operand2;
  RetData.lo = (UINT32) Value1;
  RetData.hi = (UINT32) RShiftU64 (Value1, 32);

  return RetData;
}

/**

  Subtract two UINT64s

  @param Operand1  - first UINT64
  @param Operand2  - second UINT64

  @retval operand1 - operand2

**/
UINT64_STRUCT
SubUINT64 (
          UINT64_STRUCT Operand1,
          UINT64_STRUCT Operand2
          )
{
  UINT64_STRUCT RetData;
  UINT64 Value1;
  UINT64 Value2;

  Value1 = LShiftU64 (Operand1.hi, 32) | Operand1.lo;
  Value2 = LShiftU64 (Operand2.hi, 32) | Operand2.lo;
  if (Value1 > Value2) {
    Value1 = Value1 - Value2;
  } else {
    Value1 = Value2 - Value1;
  }
  RetData.lo = (UINT32) Value1;
  RetData.hi = (UINT32) RShiftU64 (Value1, 32);

  return RetData;
}

/**

  Subtract an UINT32 from an UINT64

  @param Operand1  -  UINT64
  @param Operand2  -  UINT32

  @retval operand1 - operand2

**/
UINT64_STRUCT
SubUINT64x32 (
             UINT64_STRUCT Operand1,
             UINT32        Operand2
             )
{
  UINT64_STRUCT RetData;
  UINT64 Value1;

  Value1 =  LShiftU64 (Operand1.hi, 32) | Operand1.lo;
  if (Value1 > (UINT64) Operand2) {
    Value1 = Value1 - (UINT64) Operand2;
  } else {
    Value1 = (UINT64) Operand2 - Value1;
  }
  RetData.lo = (UINT32) Value1;
  RetData.hi = (UINT32) RShiftU64 (Value1, 32);

  return RetData;
}

/**

  Raise 10 to the power of Power, and multiply the result with Operand

  @param Operand - multiplicand
  @param Power   - value for 10 ^ Power

  @retval Operand * (10 ^ Power)

**/
UINT64_STRUCT
Power10UINT64 (
  UINT64_STRUCT Operand,
  UINT32        Power
  )
{
   while(Power--) {
      Operand = MultUINT64x32(Operand, 10);
   }
   return Operand;
}


/**

  This function shifts a 32 bit INT32 either positive or negative

  @param Value     - Input value to be shifted
  @param Shift       -Number of bits places to be shifted.

  @retval Value    - Updated 32 bit value.

**/
UINT32
BitShift (
  UINT32 Value,
  INT8 ShiftAmount
  )
{
  if ((ShiftAmount > 31) || (ShiftAmount < -31)) {
    return 0;
  }

  if (ShiftAmount > 0) {
    return (Value << ShiftAmount);
  }
  else {
    return (Value >> (-1*ShiftAmount));
  }
}

/**

  This function Sign extends 7 to 8 Bits (Bit 6 to Bit 7)

  @param CurrentValue     - Input value to be shifted

  @retval CurrentValue    - Updated 32 bit value.

**/
UINT8
SignExt (
  UINT8 CurrentValue
  )
{
  CurrentValue |= ((CurrentValue & 0x40) << 1);
  return CurrentValue;
}

/**

  This function calculates the Natural Log of the Input parameter using integers

  @param Input        - 100 times a number to get the Natural log from.
                        - Max Input Number is 40,000 (without 100x)

  @retval Output      - 100 times the actual result. Accurate within +/- 2

**/
UINT32
NaturalLog (
  UINT32 Input
  )
{
  UINT32  Output = 0;

  while (Input > 271) {
    Input = (Input*1000)/2718;
    Output += 100;
  }

  Output += ((-16*Input*Input+11578*Input-978860)/10000);

  return Output;
}

/**

  This function calculates e to the power of of the Input parameter using integers

  @param Input        - 100 times a number to elevate e to.

  @retval Output      - 100 times the actual result. Accurate within +/- 2

**/
UINT32
Eexp (
  UINT32 Input
  )
{
  UINT32  Extra100 = 0;
  UINT32  Output = 1;

  while (Input > 100){
    Input -= 100;
    Output = (Output*2718)/10;
    if (Extra100) {
      Output /= 100;
    }
    Extra100 = 1;
  }

  Output = ((Output*(8*Input*Input+900*Input+101000))/1000);

  if (Extra100) {
    Output /= 100;
  }

  return Output;
}

BOOLEAN
IsPowerOfTwo (
  UINT32  value
  )
/**

  Description: Find if a value is a power of 2

  @param value  - value to be examined


  @retval TRUE - the value is a power of two
  @retval FALSE-  the value is not a power of two

**/
{
  if((value != 0) && !(value & (value - 1))) {
    return  TRUE;
  }
  else {
    return  FALSE;
  }
}

/**
  Rotates 64-bit data left by given count

  @param Data             - 64-bit data to rotate
  @param Count            - Number of bit positions to rotate

  @retval Rotated 64-bit result

**/
UINT64_STRUCT
RotateLeftUINT64 (
  UINT64_STRUCT Data,
  UINT32        Count
  )
{
  UINT64_STRUCT RetData;
  UINT64 DataValue;
  UINT64 ReturnValue;

  DataValue = LShiftU64 (Data.hi, 32) | Data.lo;
  ReturnValue = LRotU64 (DataValue, Count);
  RetData.lo = (UINT32) ReturnValue;
  RetData.hi = (UINT32) RShiftU64 (ReturnValue, 32);

  return RetData;
}

/**
  Rotates 64-bit data right by given count

  @param Data             - 64-bit data to rotate
  @param Count            - Number of bit positions to rotate

  @retval Rotated 64-bit result

**/
UINT64_STRUCT
RotateRightUINT64 (
  UINT64_STRUCT Data,
  UINT32        Count
  )
{
  UINT64_STRUCT RetData;
  UINT64 DataValue;
  UINT64 ReturnValue;

  DataValue = LShiftU64 (Data.hi, 32) | Data.lo;
  ReturnValue = RRotU64 (DataValue, Count);
  RetData.lo = (UINT32) ReturnValue;
  RetData.hi = (UINT32) RShiftU64 (ReturnValue, 32);

  return RetData;
}

UINT32
Saturate (
  UINT32    maxVal,
  UINT32    val
  )
{
  if (maxVal < val) {
    return maxVal;
  } else {
    return val;
  }
} // Saturate

/**
  Collect each bit in a 16 bit mask into adjacent least significant bits

  @param[in]  BitMask
  @param[in]  InputValue

  @retval[out] right justified bits

**/
UINT16
EFIAPI
RightJustifySparse16BitField (
  IN  UINT16 BitMask,
  IN  UINT16 InputValue
  )
{
  UINT16 ReturnValue = 0;
  UINT8  InputBitIndex;
  UINT8  RightJustifiedBitIndex;

  RightJustifiedBitIndex = 0;
  for (InputBitIndex = 0; InputBitIndex < (sizeof (InputValue) * 8); InputBitIndex++) {
    if ((BitMask & (1 << InputBitIndex)) != 0) {
      if ((InputValue & (1 << InputBitIndex)) != 0) {
        ReturnValue |= (1 << RightJustifiedBitIndex);
      }
      RightJustifiedBitIndex++;
    }
  }
  return ReturnValue;
}


/**
  Distribute each bit in a 16 bit right justified value into a sparse bit mask

  @param[in]  BitMask
  @param[in]  InputValue

  @retval[out] value with distributed bits

**/
UINT16
EFIAPI
DistributeSparse16BitField (
  IN  UINT16 BitMask,
  IN  UINT16 InputValue
  )
{
  UINT16 ReturnValue = 0;
  UINT8  InputBitIndex;
  UINT8  RightJustifiedBitIndex;

  RightJustifiedBitIndex = 0;
  for (InputBitIndex = 0; InputBitIndex < (sizeof (InputValue) * 8); InputBitIndex++) {
    if ((BitMask & (1 << InputBitIndex)) != 0) {
      if ((InputValue & (1 << RightJustifiedBitIndex)) != 0) {
        ReturnValue |= (1 << InputBitIndex);
      }
      RightJustifiedBitIndex++;
    }
  }
  return ReturnValue;
}


