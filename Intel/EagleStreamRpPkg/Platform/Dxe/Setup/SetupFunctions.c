/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2018 Intel Corporation. <BR>

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

#include "SetupPlatform.h"

/**

  Converts an ascii string to unicode string 16 chars at a time.

  @param AsciiString    -  Address of Ascii string.
  @param UnicodeString  -  Address of Unicode String.

  @retval None.

**/
VOID
AsciiToUnicode (
  IN CHAR8     *AsciiString,
  IN CHAR16    *UnicodeString
  )
{
  UINT8 Index;

  Index = 0;
  while (AsciiString[Index] != 0) {
    UnicodeString[Index] = (CHAR16) AsciiString[Index];
    Index++;
  }
  UnicodeString[Index] = 0;
}

/**

  Swap order of nearby CHAR8 data.

  @param Data  -  The data to swap.
  @param Size  -  Size of data to swap.

  @retval None.

**/
VOID
SwapEntries (
  IN  CHAR8  *Data,
  IN  UINT16 Size
  )
{
  UINT16  Index;
  CHAR8   Temp8;

  for (Index = 0; (Index + 1) < Size; Index += 2) {
    Temp8           = Data[Index];
    Data[Index]     = Data[Index + 1];
    Data[Index + 1] = Temp8;
  }

  return ;
}
