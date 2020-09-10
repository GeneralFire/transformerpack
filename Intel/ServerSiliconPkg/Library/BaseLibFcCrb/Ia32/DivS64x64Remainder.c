/** @file
  Integer division worker functions for Ia32.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2010 Intel Corporation. <BR>

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

#include "BaseLibInternals.h"

/**
  Worker function that Divides a 64-bit signed integer by a 64-bit signed integer and
  generates a  64-bit signed result and a optional 64-bit signed remainder.

  This function divides the 64-bit signed value Dividend by the 64-bit
  signed value Divisor and generates a 64-bit signed quotient. If Remainder
  is not NULL, then the 64-bit signed remainder is returned in Remainder.
  This function returns the 64-bit signed quotient.

  @param  Dividend  A 64-bit signed value.
  @param  Divisor   A 64-bit signed value.
  @param  Remainder A pointer to a 64-bit signed value. This parameter is
                    optional and may be NULL.

  @return Dividend / Divisor

**/
INT64
EFIAPI
InternalMathDivRemS64x64 (
  IN      INT64                     Dividend,
  IN      INT64                     Divisor,
  OUT     INT64                     *Remainder  OPTIONAL
  )
{
  INT64                             Quot;

  Quot = InternalMathDivRemU64x64 (
           (UINT64) (Dividend >= 0 ? Dividend : -Dividend),
           (UINT64) (Divisor >= 0 ? Divisor : -Divisor),
           (UINT64 *) Remainder
           );
  if (Remainder != NULL && Dividend < 0) {
    *Remainder = -*Remainder;
  }
  return (Dividend ^ Divisor) >= 0 ? Quot : -Quot;
}
