/** @file
  LogByteArrayAsBin.c

  This file contains a function to log a byte array in binary format.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

/**
  This function logs the given array of bytes from highest to lowest in binary
  format.  This function does add a prefix or suffix (e.g., "b") to the logged
  binary data.

  @param[in]       pArray             Pointer to array of bytes to log.
  @param[in]       ByteCount          Number of bytes to log.

  @retval  N/A.
**/
VOID
LogByteArrayAsBin (
  IN CONST UINT8 *pArray,
  IN UINT8 ByteCount
  )
{
  INT8 ByteIdx;
  INT8 BitIdx;

  // FOR each byte from the highest to the lowest:
  for (ByteIdx = (ByteCount - 1); ByteIdx >= 0; ByteIdx--) {
    for (BitIdx = 7; BitIdx >= 0; BitIdx--) {
      RcDebugPrint (SDBG_ERROR,
        (pArray[ByteIdx] & (1 << BitIdx)) ? "1" : "0");
    }
  }
} // end function LogByteArrayAsBin

// end file LogByteArrayAsBin.c
