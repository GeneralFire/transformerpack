/** @file
  Margin2D.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2020 Intel Corporation. <BR>

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
#include "ChkRetStat.h"
#include "CommonInitPlatform.h"
#include "CpgcPointTest.h" // must go before auto-generated files
#include "InitPlatform.h"
#include "MarginParam.h"
#include "MemCfg.h"
#include "MemPointTest.h"
#include "MemShmoo.h" // must go before auto-generated files
#include "Margin2DCore.h"
#include "Margin2dResultMetadata.h"
#include "Margin2dResultColumns.h"
#include "Margin2D.h"
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryEvContentLib.h>
#include <Library/SsaUtilLib.h>
#include <Library/MemoryCoreLib.h>

// flag to enable BIOS-SSA debug messages
#define ENBL_BIOS_SSA_API_DEBUG_MSGS (0)

// flag to enable function trace debug messages
#define ENBL_FUNCTION_TRACE_DEBUG_MSGS (0)

// flag to enable verbose log messages
#define ENBL_VERBOSE_LOG_MSGS (0)

// flag to enable verbose error messages
#define ENBL_VERBOSE_ERROR_MSGS (0)

#pragma pack(push, 1)

typedef struct {
  // previous platform settings
  BOOLEAN PrevScramblerEnables[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT];
  BOOLEAN PrevSelfRefreshEnables[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT];
  BOOLEAN PrevZQCalEnables[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT];
  BOOLEAN PrevRCompUpdateEnables[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT];
} TEST_DATA;

#pragma pack(pop)

/**
@brief
  This function is the Margin2D test entry point.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       Config             Pointer to test configuration structure.

  @retval EFI_SUCCESS
          EFI_UNSUPPORTED
**/
EFI_STATUS
EFIAPI
Margin2DTestEntryPoint (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN MARGIN_2D_CONFIG *Config
  )
{
  MRC_SERVER_CPGC_INFO CpgcInfo;
  MARGIN2D_CORE_DATA CoreTestData;
  TEST_DATA TestData;
  UINT32 ResultEleCnt;
  BOOLEAN IsFirstCall, InitDqdbContent, IsLastCall;
  UINT8 RankCombinationIndex;
  UINT32 ExpectedRevision;

  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "Starting Margin2D test v%u.%u.%u.%u\n",
    BSSA_TC_MAJOR_VER, BSSA_TC_MINOR_VER, BSSA_TC_REV_VER, BSSA_TC_BUILD_VER);

  ZeroMem (&TestData, sizeof(TestData));
  ZeroMem (&CoreTestData, sizeof(CoreTestData));
  CoreTestData.pTestCfg = Config;
  CoreTestData.pCpgcInfo = &CpgcInfo;

  // get metadata with no columnar results just in case we have an error
  ResultEleCnt = 1;
  if (SetupMargin2DResults(SsaServicesHandle, ResultEleCnt, &CoreTestData.pMetadata)) {
    return EFI_UNSUPPORTED;
  }
  CoreTestData.pTestStat = (TEST_STATUS*) &CoreTestData.pMetadata->StatusCode;

  if (CoreTestData.pTestCfg == NULL) {
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "ERROR: pTestCfg is NULL!\n");
    ProcError(SsaServicesHandle, Failure, __FILE__, __LINE__, CoreTestData.pTestStat);
    return EFI_UNSUPPORTED;
  }

#ifdef SSA_REVISION_BIOS
  ExpectedRevision = SSA_REVISION_BIOS;
#else // must be rev 1.0
  ExpectedRevision = 0x01000000;
#endif
  if (SsaServicesHandle->Revision != ExpectedRevision) {
#if ENBL_VERBOSE_ERROR_MSGS
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "ERROR: BIOS-SSA API revision (0x%08X) not expected value (0x%08X)\n",
      SsaServicesHandle->Revision, ExpectedRevision);
#endif
    ProcError(SsaServicesHandle, Failure, __FILE__, __LINE__, CoreTestData.pTestStat);
    return EFI_UNSUPPORTED;
  }

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "GetSystemInfo()\n");
#endif
  SsaServicesHandle->SsaMemoryConfig->GetSystemInfo(SsaServicesHandle,
    &CoreTestData.SystemInfo);
#if ENBL_VERBOSE_LOG_MSGS
  LogSystemInfo(SsaServicesHandle, &CoreTestData.SystemInfo);
#endif
  if (ChkSystemInfo(SsaServicesHandle, &CoreTestData.SystemInfo,
    CoreTestData.pTestStat)) {
    return EFI_UNSUPPORTED;
  }

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "GetCpgcInfo()\n");
#endif
  SsaServicesHandle->SsaMemoryServerConfig->GetCpgcInfo(SsaServicesHandle,
    &CpgcInfo);
#if ENBL_VERBOSE_LOG_MSGS
  LogCpgcInfo(SsaServicesHandle, &CpgcInfo);
#endif
  if (ChkCpgcInfo(SsaServicesHandle, CoreTestData.pCpgcInfo,
    CoreTestData.pTestStat)) {
    return EFI_UNSUPPORTED;
  }

  if (GetMemCfg(SsaServicesHandle, &CoreTestData.SystemInfo,
    &CoreTestData.MemCfg, CoreTestData.pTestStat)) {
    return EFI_UNSUPPORTED;
  }
#if ENBL_VERBOSE_LOG_MSGS
  LogMemCfg(SsaServicesHandle, &CoreTestData.SystemInfo, &CoreTestData.MemCfg);
#endif

#if ENBL_VERBOSE_LOG_MSGS
  LogMargin2DTestCfg(SsaServicesHandle, CoreTestData.pTestCfg);
#endif
  if (ChkMargin2DTestCfg(SsaServicesHandle, &CoreTestData)) {
    return EFI_UNSUPPORTED;
  }

  // set up the platform
  InitPlatform(SsaServicesHandle, &CoreTestData.SystemInfo,
    &CoreTestData.MemCfg, &CoreTestData.MemPointTestCfg, Setup,
    CoreTestData.pTestCfg->ScramblerOverrideMode,
    TestData.PrevScramblerEnables, TestData.PrevSelfRefreshEnables);

  InitRCompUpdate(SsaServicesHandle, &CoreTestData.SystemInfo,
    &CoreTestData.MemCfg, &CoreTestData.MemPointTestCfg, Setup,
    TestData.PrevRCompUpdateEnables);

  InitZQCal(SsaServicesHandle, &CoreTestData.SystemInfo, &CoreTestData.MemCfg,
    &CoreTestData.MemPointTestCfg, Setup, TestData.PrevZQCalEnables);

  IsFirstCall = TRUE;
  InitDqdbContent = TRUE;
  if (SetupCpgcPointTest(SsaServicesHandle, &CoreTestData.SystemInfo,
    &CpgcInfo, &CoreTestData.MemCfg, &CoreTestData.MemPointTestCfg,
    &CoreTestData.CpgcPointTestCfg, &Margin2DPointTestResultHandler, &CoreTestData,
    CoreTestData.pTestCfg->IoLevel, CoreTestData.CoreTestCfg.IsDdrT,
    CoreTestData.pTestCfg->TestStatusLogLevel, IsFirstCall, InitDqdbContent,
    &CoreTestData.pCpgcPointTestData, CoreTestData.pTestStat)) {
    return EFI_UNSUPPORTED;
  }

  RankCombinationIndex = 0;
  if (SetCpgcPointTestRankCombinationIndex(SsaServicesHandle,
    &CoreTestData.SystemInfo, &CoreTestData.MemCfg,
    &CoreTestData.MemPointTestCfg, &CoreTestData.CpgcPointTestCfg,
    CoreTestData.pCpgcPointTestData, RankCombinationIndex,
    FALSE, // no need to update HW register setting.
    CoreTestData.pTestStat)) {
    return EFI_UNSUPPORTED;
  }

  if (SetupMemShmoo(SsaServicesHandle, &CoreTestData.SystemInfo,
    &CoreTestData.MemCfg, &CoreTestData.MemShmooCfg,
    &CoreTestData.MemPointTestCfg, &GetCpgcPointTestDimmRanks,
    &SetCpgcPointTestInBinarySearchState, &RunCpgcPointTest, NULL,
    &PreDimmInitHandler, &Margin2DPostDimmInitHandler, &PreRunPointTestHandler,
    &CoreTestData.CoreTestCfg, &CoreTestData,
    CoreTestData.pTestCfg->TestStatusLogLevel, &CoreTestData.pMemShmooData,
    CoreTestData.pTestStat)) {
    return EFI_UNSUPPORTED;
  }

  if (RunMargin2DWith1DSweep(SsaServicesHandle, &CoreTestData, NULL)) {
    return EFI_UNSUPPORTED;
  }

  // clean up the shmoo test
  if (CleanupMemShmoo(SsaServicesHandle, &CoreTestData.SystemInfo,
    &CoreTestData.MemCfg, &CoreTestData.MemShmooCfg,
    &CoreTestData.MemPointTestCfg, &CoreTestData.pMemShmooData,
    CoreTestData.pTestStat)) {
    return EFI_UNSUPPORTED;
  }

  // clean up the CPGC point test
  IsLastCall = TRUE;
  CleanupCpgcPointTest(SsaServicesHandle, &CoreTestData.SystemInfo,
    &CoreTestData.MemCfg, &CoreTestData.MemPointTestCfg, IsLastCall,
    &CoreTestData.pCpgcPointTestData);

  // perform cleanup JEDEC initializations (if required)
  Margin2DCleanupJedecInit(SsaServicesHandle, &CoreTestData);

  // clean up the platform
  InitZQCal(SsaServicesHandle, &CoreTestData.SystemInfo,
    &CoreTestData.MemCfg, &CoreTestData.MemPointTestCfg, Cleanup,
    TestData.PrevZQCalEnables);

  InitRCompUpdate(SsaServicesHandle, &CoreTestData.SystemInfo,
    &CoreTestData.MemCfg, &CoreTestData.MemPointTestCfg, Cleanup,
    TestData.PrevRCompUpdateEnables);

  InitPlatform(SsaServicesHandle, &CoreTestData.SystemInfo,
    &CoreTestData.MemCfg, &CoreTestData.MemPointTestCfg, Cleanup,
    CoreTestData.pTestCfg->ScramblerOverrideMode,
    TestData.PrevScramblerEnables, TestData.PrevSelfRefreshEnables);

  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "Margin2D test v%u.%u.%u.%u complete\n",
    BSSA_TC_MAJOR_VER, BSSA_TC_MINOR_VER, BSSA_TC_REV_VER, BSSA_TC_BUILD_VER);

  return EFI_SUCCESS;
} // end function TestEntryPoint

// end file Margin2D.c
