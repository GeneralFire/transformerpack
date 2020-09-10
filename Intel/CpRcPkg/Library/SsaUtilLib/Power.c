/** @file
  Power.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2017 Intel Corporation. <BR>

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

#include "SsaUtilInternal.h"
#include <Library/BitMaskLib.h>

/**
  This function returns the value to power of exponent

  @param[in]   value
  @param[in]   exponent

  retval   UINT8 - Power value calculated
**/
UINT32
Power (
  IN UINT32 Value,
  IN UINT32 Exponent
  )
{
//Check condition, if fails return 0.

  if (Exponent == 0) {
    return 1;
  }
  if (Exponent == 1) {
    return Value;
  }
  else
    return Value * Power (Value, Exponent -1);
}

/**
  This function check if exponential calculation is possible

  @param[in]   value
  @param[in]   exponent

  retval TRUE            - Calculating exponential value is possible
  retval FALSE           - Calculating exponential will cause overflow
 
**/
BOOLEAN
IsExponentialSafe (
  UINT32 Value,
  UINT32 Exponent
  )
{
  UINT8 BitPosition = GetHighestBitSetInMask (Value, 32);
  return ((BitPosition * Exponent) <= 32)? TRUE:FALSE;
}

/**
  This function returns the value to power of exponent

  @param[in]   value
  @param[in]   exponent
  @param[out]  *Result  - Pointer to result value

  retval EFI_SUCCESS            - Return value is valid
  retval EFI_INVALID_PARAMETER - Input parameter is not valid or caused overflow.
 
**/
EFI_STATUS
FindPowerOf (
  IN UINT32 Value,
  IN UINT32 Exponent,
  OUT UINT32 *Result
  )
{
  EFI_STATUS Status = EFI_INVALID_PARAMETER;
  if (IsExponentialSafe (Value, Exponent)) {
    *Result = Power (Value, Exponent);
    Status = EFI_SUCCESS;
    RcDebugPrint (SDBG_INFO, "Exponential Result = %d \n", *Result);
  } else {
    RcDebugPrint (SDBG_INFO, "Exponential Result Overflow\n");
  }
  return Status;
}

// end file Power.c
