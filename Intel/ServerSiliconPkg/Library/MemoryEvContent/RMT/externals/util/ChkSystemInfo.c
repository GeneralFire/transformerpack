/** @file
  ChkSystemInfo.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2018 Intel Corporation. <BR>

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
#include "ChkSystemInfo.h"
#include "Platform.h"

// flag to enable verbose error messages
#define ENBL_VERBOSE_ERROR_MSGS (0)

/**
@brief
  This function checks the given system information values against the
  corresponding preprocessor definition values.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[out]      pTestStat          Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 ChkSystemInfo(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN MRC_SYSTEM_INFO *pSystemInfo,
  OUT TEST_STATUS *pTestStat)
{
  if ((pSystemInfo->MaxNumberSockets > MAX_SOCKET_CNT) ||
    (pSystemInfo->MaxNumberControllers > MAX_CONTROLLER_CNT) ||
    (pSystemInfo->MaxNumberChannels > MAX_CHANNEL_CNT) ||
    (pSystemInfo->MaxNumberDimms > MAX_DIMM_CNT) ||
    (pSystemInfo->MaxNumberRanks > MAX_RANK_CNT) ||
    (pSystemInfo->BusWidth > MAX_BUS_WIDTH)) {
#if ENBL_VERBOSE_ERROR_MSGS
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "ERROR: SystemInfo value(s) exceed preprocessor value(s).\n");
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "MaxNumberSockets=%u MAX_SOCKET_CNT=%u\n"
      "MaxNumberControllers=%u MAX_CONTROLLER_CNT=%u\n",
      pSystemInfo->MaxNumberSockets, MAX_SOCKET_CNT,
      pSystemInfo->MaxNumberControllers, MAX_CONTROLLER_CNT);
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "MaxNumberChannels=%u MAX_CHANNEL_CNT=%u\n"
      "MaxNumberDimms=%u MAX_DIMM_CNT=%u\n",
      pSystemInfo->MaxNumberChannels, MAX_CHANNEL_CNT,
      pSystemInfo->MaxNumberDimms, MAX_DIMM_CNT);
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "MaxNumberRanks=%u MAX_RANK_CNT=%u\n"
      "BusWidth=%u MAX_BUS_WIDTH=%u\n",
      pSystemInfo->MaxNumberRanks, MAX_RANK_CNT,
      pSystemInfo->BusWidth, MAX_BUS_WIDTH);
#endif
    ProcError (SsaServicesHandle, Failure, __FILE__, __LINE__, pTestStat);
    return 1;
  }

  return 0;
} // end function ChkSystemInfo

// end file ChkSystemInfo.c
