/** @file
  LogSystemInfo.c

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

#include "SsaUtilInternal.h"

/**
  This function logs the system information.

  @param[in]       pSystemInfo        Pointer to system information structure.

  @retval N/A
**/
VOID
LogSystemInfo (
  IN MRC_SYSTEM_INFO *pSystemInfo
  )
{
  RcDebugPrint (SDBG_ERROR,
    "\n"
    "System Information:\n");
  RcDebugPrint (SDBG_ERROR,
    "  MaxNumberSockets=%u\n", pSystemInfo->MaxNumberSockets);
  RcDebugPrint (SDBG_ERROR,
    "  MaxNumberControllers=%u\n", pSystemInfo->MaxNumberControllers);
  RcDebugPrint (SDBG_ERROR,
    "  MaxNumberChannels=%u\n", pSystemInfo->MaxNumberChannels);
  RcDebugPrint (SDBG_ERROR,
    "  MaxNumberDimms=%u\n", pSystemInfo->MaxNumberDimms);
  RcDebugPrint (SDBG_ERROR,
    "  MaxNumberRanks=%u\n", pSystemInfo->MaxNumberRanks);
  RcDebugPrint (SDBG_ERROR,
    "  SocketBitMask=0x%X\n", pSystemInfo->SocketBitMask);
  RcDebugPrint (SDBG_ERROR,
    "  BusWidth=%u\n", pSystemInfo->BusWidth);
  RcDebugPrint (SDBG_ERROR,
    "  BusFreq=%u MHz\n", pSystemInfo->BusFreq);
  RcDebugPrint (SDBG_ERROR,
    "  IsEccEnabled=%u\n", pSystemInfo->IsEccEnabled);
  RcDebugPrint (SDBG_ERROR,
    "  BootMode=%u\n", pSystemInfo->BootMode);
  RcDebugPrint (SDBG_ERROR,
    "\n");
} // end function LogSystemInfo

// end file LogSystemInfo.c
