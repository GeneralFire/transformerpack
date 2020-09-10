/** @file
  InitPlatform.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2020 Intel Corporation. <BR>

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

#ifndef __INIT_PLATFORM_H__
#define __INIT_PLATFORM_H__

#include "ssabios.h"
#include "MemCfg.h"
#include "MemPointTest.h"
#include "Platform.h"
#include <Library/MemoryEvContentLib.h>

/**
  This function performs product specific platform initialization for running
  EV margining tests.

  @param[in, out]  SsaServicesHandle       Pointer to SSA services.
  @param[in]       pSystemInfo             Pointer to system information structure.
  @param[in]       pMemCfg                 Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg        Pointer to generic memory point test configuration structure.
  @param[in]       SetupCleanup            Specifies setup or cleanup action.
  @param[in]       ScramblerOverrideMode   Specifies mode for overriding the scrambler.
  @param[in, out]  PrevScramblerEnables    Pointer to array of previous ZQCal enable values.
  @param[in, out]  PrevSelfRefreshEnables  Pointer to array of previous self-refresh enable values.

  @retval  Nothing.
**/
VOID InitPlatform (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN SETUP_CLEANUP SetupCleanup,
  IN SCRAMBLER_OVERRIDE_MODE ScramblerOverrideMode,
  IN OUT BOOLEAN PrevScramblerEnables[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT],
  IN OUT BOOLEAN PrevSelfRefreshEnables[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT]
  );

#endif  // __INIT_PLATFORM_H__

