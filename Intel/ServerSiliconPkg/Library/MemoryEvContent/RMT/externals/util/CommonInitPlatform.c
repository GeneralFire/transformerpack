/** @file
  CommonInitPlatform.c

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

#include "CommonInitPlatform.h"

// flag to enable BIOS-SSA debug messages
#define ENBL_BIOS_SSA_API_DEBUG_MSGS (0)

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
  IN OUT BOOLEAN PrevPageOpenEnables[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT])
{
  UINT8 Socket, Controller;
  BOOLEAN PageOpenEnable = (PageOpenOverrideMode == EnablePageOpen) ? TRUE : FALSE;

  // FOR each socket:
  for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
    // FOR each controller:
    for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
      // IF this controller is not enabled for testing THEN skip it
      if (!IsControllerTestingEnabled(SsaServicesHandle, pSystemInfo, pMemCfg,
        pMemPointTestCfg, Socket, Controller)) {
        continue;
      }

      // IF this call is for setup
      if (SetupCleanup == Setup) {
        // IF the page-open policy is to be overridden
        if (PageOpenOverrideMode != DontTouchPageOpen) {
#if ENBL_BIOS_SSA_API_DEBUG_MSGS
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "SetPageOpenConfig(Socket=%u Controller=%u Enable=%u)\n",
            Socket, Controller, PageOpenEnable);
#endif
          SsaServicesHandle->SsaMemoryConfig->SetPageOpenConfig(SsaServicesHandle,
            Socket, Controller, PageOpenEnable, &PrevPageOpenEnables[Socket][Controller]);

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "  PrevEnable=%d\n",
            PrevPageOpenEnables[Socket][Controller]);
#endif
        } // end if the page-open policy is to be overridden
      }
      // ELSE this call is for cleanup
      else {
        // IF the page-open policy was overridden
        if (PageOpenOverrideMode != DontTouchPageOpen) {
          // IF the value was changed THEN restore it
          if (PrevPageOpenEnables[Socket][Controller] != PageOpenEnable) {
#if ENBL_BIOS_SSA_API_DEBUG_MSGS
            NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
              "SetPageOpenConfig(Socket=%u Controller=%u Enable=%d)\n",
              Socket, Controller, PrevPageOpenEnables[Socket][Controller]);
#endif
            SsaServicesHandle->SsaMemoryConfig->SetPageOpenConfig(SsaServicesHandle,
              Socket, Controller, PrevPageOpenEnables[Socket][Controller], NULL);
          }
        } // end if the page-open policy was overridden
      } // end else this call is for cleanup
    } // end for each controller
  } // end for each socket
} // end function InitPageOpen

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
  IN OUT BOOLEAN PrevZQCalEnables[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT])
{
  UINT8 Socket, Controller;

  // FOR each socket:
  for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
    // FOR each controller:
    for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
      // IF this controller is not enabled for testing THEN skip it
      if (!IsControllerTestingEnabled(SsaServicesHandle, pSystemInfo, pMemCfg,
        pMemPointTestCfg, Socket, Controller)) {
        continue;
      }

      // IF this call is for setup
      if (SetupCleanup == Setup) {
        // disable ZQCal
#if ENBL_BIOS_SSA_API_DEBUG_MSGS
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          "SetZQCalConfig(Socket=%u Controller=%u Enable=FALSE)\n",
          Socket, Controller);
#endif
        SsaServicesHandle->SsaMemoryConfig->SetZQCalConfig(SsaServicesHandle,
          Socket, Controller, FALSE, &PrevZQCalEnables[Socket][Controller]);

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          "  PrevEnable=%d\n",
          PrevZQCalEnables[Socket][Controller]);
#endif
      }
      // ELSE this call is for cleanup
      else {
        // IF the value was changed THEN restore it
        if (PrevZQCalEnables[Socket][Controller] != FALSE) {
#if ENBL_BIOS_SSA_API_DEBUG_MSGS
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "SetZQCalConfig(Socket=%u Controller=%u Enable=%d)\n",
            Socket, Controller, PrevZQCalEnables[Socket][Controller]);
#endif
          SsaServicesHandle->SsaMemoryConfig->SetZQCalConfig(SsaServicesHandle,
            Socket, Controller, PrevZQCalEnables[Socket][Controller], NULL);
        }
      } // end else this call is for cleanup
    } // end for each controller
  } // end for each socket
} // end function InitZQCal

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
  IN OUT BOOLEAN PrevRCompUpdateEnables[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT])
{
  UINT8 Socket, Controller;

  // FOR each socket:
  for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
    // FOR each controller:
    for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
      // IF this controller is not enabled for testing THEN skip it
      if (!IsControllerTestingEnabled(SsaServicesHandle, pSystemInfo, pMemCfg,
        pMemPointTestCfg, Socket, Controller)) {
        continue;
      }

      // IF this call is for setup
      if (SetupCleanup == Setup) {
        // disable RCompUpdate
#if ENBL_BIOS_SSA_API_DEBUG_MSGS
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          "SetRCompUpdateConfig(Socket=%u Controller=%u Enable=FALSE)\n",
          Socket, Controller);
#endif
        SsaServicesHandle->SsaMemoryConfig->SetRCompUpdateConfig(SsaServicesHandle,
          Socket, Controller, FALSE, &PrevRCompUpdateEnables[Socket][Controller]);

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          "  PrevEnable=%d\n",
          PrevRCompUpdateEnables[Socket][Controller]);
#endif
      }
      // ELSE this call is for cleanup
      else {
        // IF the value was changed THEN restore it
        if (PrevRCompUpdateEnables[Socket][Controller] != FALSE) {
#if ENBL_BIOS_SSA_API_DEBUG_MSGS
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "SetRCompUpdateConfig(Socket=%u Controller=%u Enable=%d)\n",
            Socket, Controller, PrevRCompUpdateEnables[Socket][Controller]);
#endif
          SsaServicesHandle->SsaMemoryConfig->SetRCompUpdateConfig(SsaServicesHandle,
            Socket, Controller, PrevRCompUpdateEnables[Socket][Controller], NULL);
        }
      } // end else this call is for cleanup
    } // end for each controller
  } // end for each socket
} // end function InitRCompUpdate

// end file CommonInitPlatform.c
