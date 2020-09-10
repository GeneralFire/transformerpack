/** @file
  ChkRetStat.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2018 Intel Corporation. <BR>

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

#include "ssabios.h"
#include "ChkRetStat.h"

#include <Library/BaseMemoryLib.h>

/**
@brief
  This function processes the given error information.  The error status and
  string are logged and placed in the given metadata structure.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       RetStat            Return status value to use.
  @param[in]       FileName           File name.
  @param[in]       LineNumber         Line number.
  @param[out]      pTestStat          Pointer to test status in results metadata structure (or NULL).
**/
VOID ProcError(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN SSA_STATUS RetStat,
  IN CHAR8* FileName,
  IN UINT32 LineNumber,
  OUT TEST_STATUS *pTestStat)
{
#if !defined(SILENT_MODE)
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "ERROR: File=%a Line=%u RetStat=%d\n", FileName, LineNumber, RetStat);

  if (pTestStat != NULL) {
    INT32 StringLen, Offset;
    UINT32 Length = sizeof(pTestStat->FileName);

    // get the string length
    for (StringLen = 0; FileName[StringLen] != 0; StringLen++) {
    }

    // get the offset into the string such that the least significant portion
    // will be placed in the character array
    Offset = StringLen - sizeof(pTestStat->FileName);

    // IF the offset is negative (could happen is string is shorter than
    // character array)
    if (Offset < 0) {
      // copy the full string and no more
      Length += Offset;
      Offset = 0;
    }

    pTestStat->StatusCode = (UINT8)RetStat;
    pTestStat->LineNumber = (UINT16)LineNumber;
    CopyMem (pTestStat->FileName, &FileName[Offset], Length);
  }
#else
  if (pTestStat != NULL) {
    pTestStat->StatusCode = (UINT8)RetStat;
    pTestStat->LineNumber = (UINT16)LineNumber;
  }
#endif // !defined(SILENT_MODE)
} // end function ProcError

/**
@brief
  This function checks the given return status value for error values.  If an
  error is detected, a message is logged and the status and function name are
  placed in the given metadata structure.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       RetStat            Return status value to check.
  @param[in]       FileName           File name.
  @param[in]       LineNumber         Line number.
  @param[out]      pTestStat          Pointer to test status in results metadata structure (or NULL).

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 ChkRetStat(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN SSA_STATUS RetStat,
  IN CHAR8* FileName,
  IN UINT32 LineNumber,
  OUT TEST_STATUS *pTestStat)
{
  if (RetStat != Success) {
    ProcError(SsaServicesHandle, RetStat, FileName, LineNumber, pTestStat);

    return 1;
  }

  return 0;
} // end function ChkRetStat

// end file ChkRetStat.c
