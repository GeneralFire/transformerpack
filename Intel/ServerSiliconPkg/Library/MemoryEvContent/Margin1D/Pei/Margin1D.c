/** @file
Margin1D.c

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
#include "CpgcPointTest.h" // must go before auto-generated files
#include "InitPlatform.h"
#include "MarginParam.h"
#include "MemCfg.h"
#include "MemPointTest.h"
#include "MemShmoo.h" // must go before auto-generated files
#include "Margin1dResultMetadata.h"
#include "Margin1dResultColumns.h"
#include "Margin1DCore.h"
#include "Margin1D.h"
#include <Library/BaseMemoryLib.h>
#include <Library/SsaUtilLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemMcIpLib.h>
#include "EvContentInternal.h"

// flag to enable BIOS-SSA debug messages
#define ENBL_BIOS_SSA_API_DEBUG_MSGS (0)

// flag to enable verbose log messages
#define ENBL_VERBOSE_LOG_MSGS (0)

// flag to enable verbose error messages
#define ENBL_VERBOSE_ERROR_MSGS (0)

#pragma pack(push, 1)

typedef struct {
  // previous platform settings
  BOOLEAN PrevScramblerEnables[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT];
  BOOLEAN PrevSelfRefreshEnables[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT];
} TEST_DATA;

#pragma pack(pop)

/**
  This function initializes the product specific elements in the given results
  metadata structure.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData       Pointer to Margin2D core test data structure.
**/
VOID InitSpecificMetadata(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT CORE_TEST_DATA *pCoreTestData)
{
  UINT8 Controller, Channel;

#ifdef __STUB__
  ASSERT (sizeof(pCoreTestData->pMetadata->LoopCounts) ==
    sizeof(pCoreTestData->pTestCfg->LoopCounts));
#endif // __STUB__

  // FOR each controller:
  for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
    // FOR each channel:
    for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
      // convert the test configuration's exponential loop count value to a
      // linear value in the metadata (per the register definition)
      pCoreTestData->pMetadata->LoopCounts[Controller][Channel] = \
        1 << (pCoreTestData->pTestCfg->LoopCounts[Controller][Channel] - 1);
    } // end for each channel
  } // end for each controller
} // end function InitSpecificMetadata

/**
@brief
  This function is the Margin1D test entry point.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       Config             Pointer to test configuration structure.

  @retval EFI_SUCCESS
          EFI_UNSUPPORTED
**/
EFI_STATUS
EFIAPI
Margin1DTestEntryPoint(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN MARGIN_1D_CONFIG *Config
  )
{
  TEST_DATA TestData;
  CORE_TEST_DATA CoreTestData;
  MRC_SERVER_CPGC_INFO CpgcInfo;
  UINT32 ResultEleCnt;
  UINT32 ExpectedRevision;
  PSYSHOST Host = GetSysHostPointer ();

  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "Starting Margin1D test v%u.%u.%u.%u\n",
    BSSA_TC_MAJOR_VER, BSSA_TC_MINOR_VER, BSSA_TC_REV_VER, BSSA_TC_BUILD_VER);

  ZeroMem (&TestData, sizeof(TestData));
  ZeroMem (&CoreTestData, sizeof(CoreTestData));
  CoreTestData.pTestCfg = Config;
  CoreTestData.pCpgcInfo = &CpgcInfo;

  //
  // Allocate 64 result entries as the initial result buffer
  //
  ResultEleCnt = 64;

  if (SetupMargin1DResults (SsaServicesHandle, &CoreTestData, ResultEleCnt,
    &CoreTestData.pMetadata)) {
    return EFI_UNSUPPORTED;
  }
  CoreTestData.pTestStat = (TEST_STATUS*) &CoreTestData.pMetadata->StatusCode;

  if (Config == NULL) {
#if ENBL_VERBOSE_ERROR_MSGS
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "ERROR: Config is NULL!\n");
#endif
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
    "  GetSystemInfo()\n");
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
    "  GetCpgcInfo()\n");
#endif
  SsaServicesHandle->SsaMemoryServerConfig->GetCpgcInfo(SsaServicesHandle,
    CoreTestData.pCpgcInfo);
#if ENBL_VERBOSE_LOG_MSGS
  LogCpgcInfo(SsaServicesHandle, CoreTestData.pCpgcInfo);
#endif
  if (ChkCpgcInfo(SsaServicesHandle, CoreTestData.pCpgcInfo,
    CoreTestData.pTestStat)) {
    return EFI_UNSUPPORTED;
  }

  if (GetMemCfg(SsaServicesHandle, &CoreTestData.SystemInfo, &CoreTestData.MemCfg,
    CoreTestData.pTestStat)) {
    return EFI_UNSUPPORTED;
  }
#if ENBL_VERBOSE_LOG_MSGS
  LogMemCfg(SsaServicesHandle, &CoreTestData.SystemInfo, &CoreTestData.MemCfg);
#endif

  CoreTestData.HasDdrtDevices = SSAIsDdrtDimmPresent (Host);

#if ENBL_VERBOSE_LOG_MSGS
  LogMargin1DTestCfg(SsaServicesHandle, (MARGIN_1D_CONFIG *) CoreTestData.pTestCfg);
#endif
  if (ChkMargin1DTestCfg(SsaServicesHandle, &CoreTestData)) {
    return EFI_UNSUPPORTED;
  }

  // setup the platform
  InitPlatform(SsaServicesHandle, &CoreTestData.SystemInfo,
    &CoreTestData.MemCfg, &CoreTestData.SelectMemPointTestCfg, Setup,
    CoreTestData.pTestCfg->ScramblerOverrideMode,
    TestData.PrevScramblerEnables, TestData.PrevSelfRefreshEnables);

  if (RunMargin1DTest(SsaServicesHandle, &CoreTestData, InitSpecificMetadata)) {
    return EFI_UNSUPPORTED;
  }

  // cleanup the platform
  InitPlatform(SsaServicesHandle, &CoreTestData.SystemInfo,
    &CoreTestData.MemCfg, &CoreTestData.SelectMemPointTestCfg, Cleanup,
    CoreTestData.pTestCfg->ScramblerOverrideMode,
    TestData.PrevScramblerEnables, TestData.PrevSelfRefreshEnables);

  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "Margin1D test v%u.%u.%u.%u complete\n",
    BSSA_TC_MAJOR_VER, BSSA_TC_MINOR_VER, BSSA_TC_REV_VER, BSSA_TC_BUILD_VER);

  return EFI_SUCCESS;
} // end function Margin1DTestEntryPoint


// end file Margin1D.c
