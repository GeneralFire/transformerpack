/** @file
  BitMask.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright2018 Intel Corporation. <BR>

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

#include <Uefi.h>
#include <Library/BitMaskLib.h>

/**
  This helper function returns the number of 1's and location of the first 1 in
  the given 8-bit bitmask.

  @param[in]   Bitmask             Bitmask.
  @param[in]   MaxBitCount         Maximum bit count.  The value must be more than or equal to 1 and less than or equal to 32.
  @param[out]  pBitCnt             Pointer to the location where the count of 1's will be placed.
  @param[out]  pLowestBitLocation  Pointer to the location where the location of the lowest 1 will be placed or a value of MaxBitCount if there is no bit set.
**/
VOID
GetBitSetCntInMask (
  IN UINT32 Bitmask,
  IN UINT8 MaxBitCount,
  OUT UINT8 *pBitCnt,
  OUT UINT8 *pLowestBitLocation
  )
{
  UINT8 Bit;
  UINT8 BitCnt = 0;

  for (Bit = 0; Bit < MaxBitCount; Bit++){
    if ((Bitmask & (1 << Bit)) != 0) {
      BitCnt += 1;
    }
  }

  for (Bit = 0; Bit < MaxBitCount; Bit++){
    if ((Bitmask & (1 << Bit)) != 0) {
      break;
    }
  }

  *pBitCnt = BitCnt;
  *pLowestBitLocation = Bit;
} // end function GetBitSetCntInMask

/**
  This function is used to check if the given bit is the highest bit set in the
  given mask.

  @param[in]   Bit       Bit to check.
  @param[in]   Bitmask   Bitmask.

  @retval  TRUE/FALSE.
**/
BOOLEAN
IsHighestBitSetInMask  (
  IN UINT8 Bit,
  IN UINT32 Bitmask
  )
{
  return ((Bitmask & ~(1 << Bit)) < ((UINT32) 1 << Bit));
} //  end function IsHighestBitSetInMask

/**
  This function is used to check if the given bit is the highest bit set in the
  given mask excluding the given special bit.

  @param[in]   Bit       Bit to check.
  @param[in]   Bitmask   Bitmask.
  @param[in]   ExclBit   Bit to exclude.

  @retval  TRUE/FALSE.
**/
BOOLEAN
IsHighestBitSetInMaskExcl (
  IN UINT8 Bit,
  IN UINT32 Bitmask,
  IN UINT8 ExclBit
  )
{
  Bitmask &= ~(1 << ExclBit);

  return IsHighestBitSetInMask(Bit, Bitmask);
} // end function IsHighestBitSetInMaskExcl

/**
  This function is used to get the highest bit set in the given mask.

  @param[in]   Bitmask       Bitmask.
  @param[in]   MaxBitCount   Maximum bit count.  The value must be more than or equal to 1 and less than or equal to 32.

  @retval  Offset of highest bit set in mask or a value of 0 if there is no bit set.
**/
UINT8
GetHighestBitSetInMask (
  IN UINT32 Bitmask,
  IN UINT8 MaxBitCount
  )
{
  INT8 Bit;

  // FOR each bit (starting from the highest):
  for (Bit = (MaxBitCount - 1); Bit > 0; Bit--) {
    if (Bitmask & (1 << Bit)) {
      break;
    }
  }

  // IF there were no bits set THEN return 0
  if (Bit < 0) {
    Bit = 0;
  }

  return (UINT8) Bit;
} // end function GetHighestBitSetInMask

/**
  This function is used to get the lowest bit set in the given mask.

  @param[in]   Bitmask       Bitmask.
  @param[in]   MaxBitCount   Maximum bit count.  The value must be more than or equal to 1 and less than or equal to 32.

  @retval  Offset of lowest bit set in mask or a value of MaxBitCount if there is no bit set.
**/
UINT8
GetLowestBitSetInMask (
  IN UINT32 Bitmask,
  IN UINT8 MaxBitCount
  )
{
  UINT8 Bit;

  for (Bit = 0; Bit < MaxBitCount; Bit++) {
    if (Bitmask & (1 << Bit)) {
      break;
    }
  }

  return Bit;
} //  end function GetLowestBitSetInMask

/**
  This function is used to get the lowest bit set in the given mask that is
  also not equal to the given value.

  @param[in]   Bitmask       Bitmask.
  @param[in]   MaxBitCount   Maximum bit count.  The value must be more than or equal to 1 and less than or equal to 32.
  @param[in]   NotEqBit      Bit that the returned value must not be equal to.

  @retval  Offset of lowest bit set in mask that is also not equal to the given value or a value of MaxBitCount if there is no bit set.
**/
UINT8
GetLowestBitSetInMaskThatIsNotEq (
  IN UINT32 Bitmask,
  IN UINT8 MaxBitCount,
  IN UINT8 NotEqBit
  )
{
  Bitmask &= ~(1 << NotEqBit);

  return GetLowestBitSetInMask(Bitmask, MaxBitCount);
} //  end function GetLowestBitSetInMaskThatIsNotEq

/**
  This function is used to get the next higher bit set in the given mask.

  @param[in]   CrntBit       Current bit.
  @param[in]   Bitmask       Bitmask.
  @param[in]   MaxBitCount   Maximum bit count.  The value must be more than or equal to 1 and less than or equal to 32.

  @retval  Offset of next higher bit set in mask or a value of MaxBitCount if there is no next bit set.
**/
UINT8
GetNextHigherBitSetInMask (
  IN UINT8 CrntBit,
  IN UINT32 Bitmask,
  IN UINT8 MaxBitCount
  )
{
  UINT8 Bit = CrntBit + 1;

  for (Bit = CrntBit + 1; Bit < MaxBitCount; Bit++) {
    if (Bitmask & (1 << Bit)) {
      break;
    }
  }

  return Bit;
} //  end function GetNextHigherBitSetInMask

/**
  This function is used to get the next higher bit set in the given mask that
  is also not equal to the given value.

  @param[in]   CrntBit       Current bit.
  @param[in]   Bitmask       Bitmask.
  @param[in]   MaxBitCount   Maximum bit count.  The value must be more than or equal to 1 and less than or equal to 32.
  @param[in]   NotEqBit      Bit that the returned value must not be equal to.

  @retval  Offset of next higher bit set in mask that is also not equal to the given value or a value of MaxBitCount if there is no next bit set.
**/
UINT8
GetNextHigherBitSetInMaskThatIsNotEq (
  IN UINT8 CrntBit,
  IN UINT32 Bitmask,
  IN UINT8 MaxBitCount,
  IN UINT8 NotEqBit
  )
{
  Bitmask &= ~(1 << NotEqBit);

  return GetNextHigherBitSetInMask(CrntBit, Bitmask, MaxBitCount);
} //  end function GetNextBitSetInMaskThatIsNotEq

// end file BitMask.c
