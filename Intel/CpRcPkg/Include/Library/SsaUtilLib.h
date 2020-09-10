/** @file
  SsaContentSupportLib.h

  Library header with API to support SSA related utility

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

#ifndef _SSA_UTIL_LIB_H_
#define _SSA_UTIL_LIB_H_

#include <Ppi/SsaBiosServicesPpi.h>

/**
  This function logs the system information.

  @param[in]       pSystemInfo        Pointer to system information structure.
**/
VOID
LogSystemInfo (
  IN MRC_SYSTEM_INFO *pSystemInfo
  );

/**
  This function is used to log the given specificity value.

  @param[in]       Specificity        Specificity.

  @retval  Nothing.
**/
VOID
LogSpecificity (
  IN MARGIN_PARAM_SPECIFICITY Specificity
  );

/**
  This function logs the given array of bytes from highest to lowest in binary
  format.  This function does add a prefix or suffix (e.g., "b") to the logged
  binary data.

  @param[in]       pArray             Pointer to array of bytes to log.
  @param[in]       ByteCount          Number of bytes to log.

  @retval  Nothing.
**/
VOID
LogByteArrayAsBin (
  IN CONST UINT8 *pArray,
  IN UINT8 ByteCount
  );

/**
  This function logs the given array of bytes from highest to lowest as hex.
  This function does NOT start the log with "0x" or end it with "\n".

  @param[in]       pArray             Pointer to array of bytes to log.
  @param[in]       ByteCount          Number of bytes to log.

  @retval  Nothing.
**/
VOID
LogByteArrayAsHex (
  IN CONST UINT8 *pArray,
  IN UINT8 ByteCount
  );

/**
  This function is used to log the given specificity value.

  @param[in]       Specificity        Specificity.

  @retval  Nothing.
**/
VOID
LogSpecificity (
  IN MARGIN_PARAM_SPECIFICITY Specificity
  );


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
  );

#endif// _SSA_UTIL_LIB_H_
