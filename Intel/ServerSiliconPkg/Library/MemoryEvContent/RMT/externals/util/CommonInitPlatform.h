/** @file
  CommonInitPlatform.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2015 Intel Corporation. <BR>

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

#ifndef __COMMON_INIT_PLATFORM_H__
#define __COMMON_INIT_PLATFORM_H__

#include "ssabios.h"
#include "MemCfg.h"
#include "MemPointTest.h"
#include "Platform.h"
#include <Library/MemoryEvContentLib.h>

/**
  This function performs page-open policy initialization for running EV
  margining tests.  When the SetupCleanup parameter is set to Setup, the
  page-open policy is set per the given PageOpenOverrideMode parameter value
  and (if PageOpenOverrideMode parameter is not DontTouchPageOpen then) the
  previous settings are returned in the PrevPageOpenEnables parameter.  When
  the SetupCleanup parameter is set to Cleanup (and the PageOpenOverrideMode
  parameter is not DontTouchPageOpen), the values in the PrevPageOpenEnables
  parameter are restored.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
  @param[in]       SetupCleanup       Specifies setup or cleanup action.
  @param[in]       PageOpenOverrideMode   Specifies mode for overriding the page-open policy.
  @param[in, out]  PrevPageOpenEnables   Pointer to array of previous page-open policy enable values.

  @retval  Nothing.
**/
VOID InitPageOpen(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN SETUP_CLEANUP SetupCleanup,
  IN PAGE_OPEN_OVERRIDE_MODE PageOpenOverrideMode,
  IN OUT BOOLEAN PrevPageOpenEnables[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT]);

/**
  This function performs ZQCal initialization for running EV margining tests.
  When SetupCleanup is set to Setup, the ZQCal is disabled and the previous
  settings are returned in the PrevZQCalEnables parameter.  When SetupCleanup
  is set to Cleanup, the values in the PrevZQCalEnables are restored.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
  @param[in]       SetupCleanup       Specifies setup or cleanup action.
  @param[in, out]  PrevZQCalEnables   Pointer to array of previous ZQCal enable values.

  @retval  Nothing.
**/
VOID InitZQCal(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN SETUP_CLEANUP SetupCleanup,
  IN OUT BOOLEAN PrevZQCalEnables[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT]);

/**
  This function performs RComp update initialization for running EV margining tests.
  When SetupCleanup is set to Setup, the RComp update is disabled and the previous
  settings are returned in the PrevRCompUpdateEnables parameter.  When SetupCleanup
  is set to Cleanup, the values in the PrevRCompUpdateEnables are restored.

  @param[in, out]  SsaServicesHandle       Pointer to SSA services.
  @param[in]       pSystemInfo             Pointer to system information structure.
  @param[in]       pMemCfg                 Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg        Pointer to generic memory point test configuration structure.
  @param[in]       SetupCleanup            Specifies setup or cleanup action.
  @param[in, out]  PrevRCompUpdateEnables  Pointer to array of previous RComp update enable values.

  @retval  Nothing.
**/
VOID InitRCompUpdate(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN SETUP_CLEANUP SetupCleanup,
  IN OUT BOOLEAN PrevRCompUpdateEnables[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT]);

#endif  // __COMMON_INIT_PLATFORM_H__

// end file CommonInitPlatform.h
