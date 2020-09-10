/** @file
  RMT.c

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
#include "CommonInitPlatform.h"
#include "CpgcPointTest.h"
#include "InitPlatform.h"
#include "MemPointTest.h"
#include "Platform.h"
#include "RmtResultColumns.h"
#include "RmtResultMetadata.h"
#include "RmtCore.h"
#include "RMT.h"
#include <Library/BaseMemoryLib.h>
#include <Library/SsaUtilLib.h>
#include <Library/MemoryServicesLib.h>
#include <ReferenceCodeFatalErrors.h>

// flag to enable BIOS-SSA debug messages
#define ENBL_BIOS_SSA_API_DEBUG_MSGS (0)

// flag to enable function trace debug messages
#define ENBL_FUNCTION_TRACE_DEBUG_MSGS (0)

// flag to enable verbose log messages
#define ENBL_VERBOSE_LOG_MSGS (0)

// flag to enable results debug messages
#define ENBL_RESULTS_DEBUG_LOG_MSGS (0)

#pragma pack(push, 1)

// RMT product specific test data
typedef struct {
  // previous platform settings
  BOOLEAN PrevScramblerEnables[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT];
  BOOLEAN PrevSelfRefreshEnables[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT];
  BOOLEAN PrevZQCalEnables[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT];
  BOOLEAN PrevRCompUpdateEnables[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT];
} RMT_PRODUCT_SPECIFIC_TEST_DATA;

#pragma pack(pop)

/**
  This function is used to initialize the given CPGC point test configuration
  structure.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to RMT core test data.
  @param[in]       RankCombination    Rank combination.
  @param[in]       TrafficMode        Memory traffic mode.

  @retval  0  success
  @retval  1  error occurred
**/
static UINT8 InitCpgcPointTestCfg(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT RMT_CORE_TEST_DATA *pCoreTestData,
  IN RANK_TEST_MODE RankTestMode,
  IN TRAFFIC_MODE TrafficMode)
{
  CONST RMT_CONFIG *pTestCfg = pCoreTestData->pTestCfg;
  CPGC_POINT_TEST_CONFIG *pCpgcPointTestCfg = &pCoreTestData->CpgcPointTestCfg;
  UINT8 Controller, Channel;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "RMT.InitCpgcPointTestCfg()\n");
#endif

  // initialize the CPGC test configuration to default values
  SetCpgcPointTestCfgDefaults(pCpgcPointTestCfg);

  // we're not counting errors so as son as all lanes fail we can stop
  pCpgcPointTestCfg->CpgcStopMode = AllLanesFailCpgcStopMode;

  // copy test configuration values to memory point test configuration

  // FOR each controller:
  for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
    // FOR each channel:
    for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
      pCpgcPointTestCfg->TrafficModes[Controller][Channel] = TrafficMode;

#if SUPPORT_FOR_TURNAROUNDS
      if (RankTestMode == Turnarounds) {
        pCpgcPointTestCfg->BurstLengths[Controller][Channel] = \
          pTestCfg->TurnaroundsBurstLength;
        pCpgcPointTestCfg->LoopCounts[Controller][Channel] = \
          pTestCfg->TurnaroundsLoopCount;
        pCpgcPointTestCfg->BinarySearchLoopCounts[Controller][Channel] = \
          pTestCfg->TurnaroundsBinarySearchLoopCount;
      }
      else
#endif // SUPPORT_FOR_TURNAROUNDS
      {
        if (pTestCfg->EnableChannelSpecificLoopCountsBurstlengths){
          pCpgcPointTestCfg->BurstLengths[Controller][Channel] = \
            pTestCfg->BurstLengths[Channel];
          pCpgcPointTestCfg->LoopCounts[Controller][Channel] = \
            pTestCfg->LoopCounts[Channel];
        } else {
          pCpgcPointTestCfg->BurstLengths[Controller][Channel] = \
            pTestCfg->SinglesBurstLength;
          pCpgcPointTestCfg->LoopCounts[Controller][Channel] = \
            pTestCfg->SinglesLoopCount;
        }
        pCpgcPointTestCfg->BinarySearchLoopCounts[Controller][Channel] = \
          pTestCfg->SinglesBinarySearchLoopCount;
      }
    } // end for each channel
  } // end for each controller

  if (ChkCpgcPointTestCfg(SsaServicesHandle, &pCoreTestData->SystemInfo,
    pCoreTestData->pCpgcInfo, &pCoreTestData->MemCfg,
    &pCoreTestData->SelectMemPointTestCfg, pCpgcPointTestCfg,
    pCoreTestData->pTestStat)) {
    return 1;
  }

  return 0;
} // end function InitCpgcPointTestCfg

/**
  This function is used to setup same rank traffic for data signal margining.

  This function conforms to the SETUP_RMT_SAME_RANK_FOR_DATA_MARGIN typedef in RmtCore.h.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to RMT core test data.
  @param[in]       IoLevel            I/O level.
  @param[in]       IsDdrT             Flag indicating whether DDR-T devices are being tested.
  @param[in]       EnableLaneMargin   Flag indicating whether lane margin data is being collected.

  @retval  0  success
  @retval  1  error occurred
**/
static UINT8 SetupSameRankForDataMargin(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT RMT_CORE_TEST_DATA *pCoreTestData,
  IN MRC_LT IoLevel,
  IN BOOLEAN IsDdrT,
  IN BOOLEAN EnableLaneMargin)
{
  BOOLEAN InitDqdbContent, IsLastCall;
  BOOLEAN InitAsAll;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "RMT.SetupSameRankForDataMargin(IoLevel=%u IsDdrT=%u EnableLaneMargin=%u)\n",
    IoLevel, IsDdrT, EnableLaneMargin);
#endif

  if (!pCoreTestData->IsFirstCallToSetupCpgcPointTest) {
    // clean up previous configuration
    IsLastCall = FALSE;
    CleanupCpgcPointTest(SsaServicesHandle, &pCoreTestData->SystemInfo,
      &pCoreTestData->MemCfg, &pCoreTestData->SelectMemPointTestCfg,
      IsLastCall, &pCoreTestData->pCpgcPointTestData);
  }

  // initialize the point test configurations to default values
  InitAsAll = FALSE;
  if (InitSelectMemPointTestCfg(SsaServicesHandle, pCoreTestData, IoLevel,
    MrcGtMax, Singles, IsDdrT, InitAsAll)) {
    return 1;
  }

  if (InitCpgcPointTestCfg(SsaServicesHandle, pCoreTestData, Singles,
    LoopbackTrafficMode)) {
    return 1;
  }

  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // make changes specific to same rank data signal margining here
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // IF we're not collecting lane margin data THEN
  if (!EnableLaneMargin) {
    // we can stop the test as soon as the first error is detected on any lane
    pCoreTestData->CpgcPointTestCfg.CpgcStopMode = NthAnyLaneFailCpgcStopMode;
  }

#if ENBL_VERBOSE_LOG_MSGS
  LogCpgcPointTestCfg(SsaServicesHandle, &pCoreTestData->CpgcPointTestCfg);
#endif

  // setup the point test
  InitDqdbContent = pCoreTestData->IsFirstCallToSetupCpgcPointTest;
  if (SetupCpgcPointTest(SsaServicesHandle, &pCoreTestData->SystemInfo,
    pCoreTestData->pCpgcInfo, &pCoreTestData->MemCfg,
    &pCoreTestData->SelectMemPointTestCfg, &pCoreTestData->CpgcPointTestCfg,
    RmtPointTestResultHandler, pCoreTestData, IoLevel, IsDdrT,
    pCoreTestData->pTestCfg->TestStatusLogLevel,
    pCoreTestData->IsFirstCallToSetupCpgcPointTest, InitDqdbContent,
    &pCoreTestData->pCpgcPointTestData, pCoreTestData->pTestStat)) {
    return 1;
  }

  pCoreTestData->IsFirstCallToSetupCpgcPointTest = FALSE;

  return 0;
} // end function SetupSameRankForDataMargin

/**
  This function is used to setup same rank traffic for command/address/control
  signal margining.

  This function conforms to the SETUP_RMT_SAME_RANK_FOR_CMD_MARGIN typedef in RmtCore.h.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to RMT core test data.
  @param[in]       IoLevel            I/O level.
  @param[in]       IsDdrT             Flag indicating whether DDR-T devices are being tested.

  @retval  0  success
  @retval  1  error occurred
**/
static UINT8 SetupSameRankForCmdMargin(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT RMT_CORE_TEST_DATA *pCoreTestData,
  IN MRC_LT IoLevel,
  IN BOOLEAN IsDdrT)
{
  UINT8 Controller, Channel;
  BOOLEAN InitDqdbContent, IsLastCall;
  BOOLEAN InitAsAll;
  //CPU_INFO CpuInfo;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "RMT.SetupSameRankForCmdMargin(IoLevel=%u)\n", IoLevel);
#endif

  //GetCpuInfo(SsaServicesHandle, &CpuInfo);

  if (!pCoreTestData->IsFirstCallToSetupCpgcPointTest) {
    // clean up previous configuration
    IsLastCall = FALSE;
    CleanupCpgcPointTest(SsaServicesHandle, &pCoreTestData->SystemInfo,
      &pCoreTestData->MemCfg, &pCoreTestData->SelectMemPointTestCfg,
      IsLastCall, &pCoreTestData->pCpgcPointTestData);
  }

  // initialize the point test configuration to default values
  InitAsAll = FALSE;
  if (InitSelectMemPointTestCfg(SsaServicesHandle, pCoreTestData, IoLevel,
    CmdVref, Singles, IsDdrT, InitAsAll)) {
    return 1;
  }

  if (InitCpgcPointTestCfg(SsaServicesHandle, pCoreTestData, Singles,
    LoopbackTrafficMode)) {
    return 1;
  }

#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
  // IF this is not the frontside:
  if (IoLevel != DdrLevel) {
    // remove DIMMs that don't support backside command margining
    RemoveNonBacksideCmdDimms(SsaServicesHandle, &pCoreTestData->SystemInfo,
      &pCoreTestData->MemCfg, pCoreTestData->BacksideCmdBitmasks,
      &pCoreTestData->SelectMemPointTestCfg);
  } // end if this is not the frontside
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT

  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // make changes specific to same rank command/address/control signal margining here
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  pCoreTestData->CpgcPointTestCfg.CpgcStopMode = NthAnyLaneFailCpgcStopMode;

  // FOR each controller:
  for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
    // FOR each channel:
    for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
      // use active addressing
      pCoreTestData->CpgcPointTestCfg.AddrModes[Controller][Channel] = \
        ActiveAddrMode;

      // CADB patterns are not supported for DDR-T in SKX
      // IF we're testing DDR-T devices
      if (IsDdrT /*&& (CpuInfo.Stepping < SKX_B0_STEPPING) */) {
        pCoreTestData->CpgcPointTestCfg.CadbModes[Controller][Channel] = \
          NormalCadbMode;
      }
      else {
        pCoreTestData->CpgcPointTestCfg.CadbModes[Controller][Channel] = \
          OnDeselectCadbMode;
      } // end else we're testing not DDR-T devices
    } // end for each channel
  } // end for each controller

#if ENBL_VERBOSE_LOG_MSGS
  LogCpgcPointTestCfg(SsaServicesHandle, &pCoreTestData->CpgcPointTestCfg);
#endif

  // setup the point test
  InitDqdbContent = pCoreTestData->IsFirstCallToSetupCpgcPointTest;
  if (SetupCpgcPointTest(SsaServicesHandle, &pCoreTestData->SystemInfo,
    pCoreTestData->pCpgcInfo, &pCoreTestData->MemCfg,
    &pCoreTestData->SelectMemPointTestCfg, &pCoreTestData->CpgcPointTestCfg,
    RmtPointTestResultHandler, pCoreTestData, IoLevel, IsDdrT,
    pCoreTestData->pTestCfg->TestStatusLogLevel,
    pCoreTestData->IsFirstCallToSetupCpgcPointTest, InitDqdbContent,
    &pCoreTestData->pCpgcPointTestData, pCoreTestData->pTestStat)) {
    return 1;
  }

  pCoreTestData->IsFirstCallToSetupCpgcPointTest = FALSE;

  return 0;
} // end function SetupSameRankForCmdMargin

#if SUPPORT_FOR_TURNAROUNDS
/**
  This function is used to setup rank-to-rank turnaround traffic for data
  signal margining.

  This function conforms to the SETUP_RMT_TURNAROUND_FOR_DATA_MARGIN typedef in RmtCore.h.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to RMT core test data.
  @param[in]       IoLevel            I/O level.
  @param[in]       IsDdrT             Flag indicating whether DDR-T devices are being tested.
  @param[in]       TrafficMode        Memory traffic mode.

  @retval  0  success
  @retval  1  error occurred
**/
static UINT8 SetupTurnaroundForDataMargin(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT RMT_CORE_TEST_DATA *pCoreTestData,
  IN MRC_LT IoLevel,
  IN BOOLEAN IsDdrT,
  IN TRAFFIC_MODE TrafficMode)
{
  BOOLEAN InitDqdbContent, IsLastCall;
  BOOLEAN InitAsAll;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "RMT.SetupTurnaroundForDataMargin(IoLevel=%u)\n", IoLevel);
#endif

  if (!pCoreTestData->IsFirstCallToSetupCpgcPointTest) {
    // clean up previous configuration
    IsLastCall = FALSE;
    CleanupCpgcPointTest(SsaServicesHandle, &pCoreTestData->SystemInfo,
      &pCoreTestData->MemCfg, &pCoreTestData->SelectMemPointTestCfg,
      IsLastCall, &pCoreTestData->pCpgcPointTestData);
  }

  // initialize the point test configuration to default values
  InitAsAll = FALSE;
  if (InitSelectMemPointTestCfg(SsaServicesHandle, pCoreTestData, IoLevel,
    GsmGtMax, Turnarounds, IsDdrT, InitAsAll)) {
    return 1;
  }

  if (InitCpgcPointTestCfg(SsaServicesHandle, pCoreTestData, Turnarounds,
    TrafficMode)) {
    return 1;
  }

  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // make changes specific to turnaround data signal margining here
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  pCoreTestData->CpgcPointTestCfg.CpgcStopMode = NthAnyLaneFailCpgcStopMode;

#if ENBL_VERBOSE_LOG_MSGS
  LogCpgcPointTestCfg(SsaServicesHandle, &pCoreTestData->CpgcPointTestCfg);
#endif

  // setup the point test
  InitDqdbContent = pCoreTestData->IsFirstCallToSetupCpgcPointTest;
  if (SetupCpgcPointTest(SsaServicesHandle, &pCoreTestData->SystemInfo,
    pCoreTestData->pCpgcInfo, &pCoreTestData->MemCfg,
    &pCoreTestData->SelectMemPointTestCfg, &pCoreTestData->CpgcPointTestCfg,
    RmtPointTestResultHandler, pCoreTestData, IoLevel, IsDdrT,
    pCoreTestData->pTestCfg->TestStatusLogLevel,
    pCoreTestData->IsFirstCallToSetupCpgcPointTest, InitDqdbContent,
    &pCoreTestData->pCpgcPointTestData, pCoreTestData->pTestStat)) {
    return 1;
  }

  pCoreTestData->IsFirstCallToSetupCpgcPointTest = FALSE;

  return 0;
} // end function SetupTurnaroundForDataMargin
#endif // SUPPORT_FOR_TURNAROUNDS

#if SUPPORT_FOR_DDRT
/**
  This function is used to setup same rank traffic for DDR-T Early Read ID
  signal margining.

  This function conforms to the SETUP_RMT_SAME_RANK_FOR_ERID_MARGIN typedef in RmtCore.h.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to RMT core test data.

  @retval  0  success
  @retval  1  error occurred
**/
static UINT8 SetupSameRankForEridMargin(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT RMT_CORE_TEST_DATA *pCoreTestData)
{
  UINT8 Controller, Channel;
  BOOLEAN InitDqdbContent, IsLastCall;
  BOOLEAN InitAsAll;
  BOOLEAN IsDdrT = TRUE;
  MRC_LT IoLevel = DdrLevel;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "RMT.SetupSameRankForEridMargin()\n");
#endif

  if (!pCoreTestData->IsFirstCallToSetupCpgcPointTest) {
    // clean up previous configuration
    IsLastCall = FALSE;
    CleanupCpgcPointTest(SsaServicesHandle, &pCoreTestData->SystemInfo,
      &pCoreTestData->MemCfg, &pCoreTestData->SelectMemPointTestCfg,
      IsLastCall, &pCoreTestData->pCpgcPointTestData);
  }

  // initialize the point test configurations to default values
  InitAsAll = FALSE;
  if (InitSelectMemPointTestCfg(SsaServicesHandle, pCoreTestData, IoLevel,
    MrcGtMax, Singles, IsDdrT, InitAsAll)) {
    return 1;
  }

#if SUPPORT_FOR_DDRT
  // remove non-DDRT DIMMs which don't support ERID margining
  RemoveNonDdrtDimms(SsaServicesHandle, &pCoreTestData->SystemInfo,
    &pCoreTestData->MemCfg,
    &pCoreTestData->SelectMemPointTestCfg);
#endif // SUPPORT_FOR_DDRT

  if (InitCpgcPointTestCfg(SsaServicesHandle, pCoreTestData, Singles,
    LoopbackTrafficMode)) {
    return 1;
  }

  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // make changes specific to same rank DDR-T Early Read ID signal margining here
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  SetCpgcPointTestCfgErid (&(pCoreTestData->CpgcPointTestCfg), TRUE);

  // preset the lane validation masks to no lanes
  ZeroMem (pCoreTestData->SelectMemPointTestCfg.LaneValBitmasks,
    sizeof (pCoreTestData->SelectMemPointTestCfg.LaneValBitmasks));

  // FOR each controller:
  for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
    // FOR each channel:
    for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
      // set the lane validation masks to the least significant two lanes
      pCoreTestData->SelectMemPointTestCfg.LaneValBitmasks[Controller][Channel][0] = \
        (1 << MAX_ERID_SIGNAL_CNT) - 1;
    } // end for each channel
  } // end for each controller

#if ENBL_VERBOSE_LOG_MSGS
  LogCpgcPointTestCfg(SsaServicesHandle, &pCoreTestData->CpgcPointTestCfg);
#endif

  // setup the point test
  InitDqdbContent = pCoreTestData->IsFirstCallToSetupCpgcPointTest;
  if (SetupCpgcPointTest(SsaServicesHandle, &pCoreTestData->SystemInfo,
    pCoreTestData->pCpgcInfo, &pCoreTestData->MemCfg,
    &pCoreTestData->SelectMemPointTestCfg, &pCoreTestData->CpgcPointTestCfg,
    RmtPointTestResultHandler, pCoreTestData, IoLevel, IsDdrT,
    pCoreTestData->pTestCfg->TestStatusLogLevel,
    pCoreTestData->IsFirstCallToSetupCpgcPointTest, InitDqdbContent,
    &pCoreTestData->pCpgcPointTestData, pCoreTestData->pTestStat)) {
    return 1;
  }

  pCoreTestData->IsFirstCallToSetupCpgcPointTest = FALSE;

  return 0;
} // end function SetupSameRankForEridMargin
#endif // SUPPORT_FOR_DDRT

/**
  This function is the RMT test entry point.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       Config             Pointer to test configuration structure.

  @retval EFI_SUCCESS
          EFI_UNSUPPORTED
  **/
EFI_STATUS
EFIAPI
RmtTestEntryPoint(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN RMT_CONFIG *Config
  )
{
  RMT_PRODUCT_SPECIFIC_TEST_DATA TestData;
  RMT_CORE_TEST_DATA *CoreTestData;
  MRC_SERVER_CPGC_INFO CpgcInfo;
  UINT32 ResultEleCnt;
  BOOLEAN IsLastCall;
  BOOLEAN IsDdrT, InitAsAll;
  UINT32 ExpectedRevision;

  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "Starting RMT test v%u.%u.%u.%u\n",
    BSSA_TC_MAJOR_VER, BSSA_TC_MINOR_VER, BSSA_TC_REV_VER, BSSA_TC_BUILD_VER);

  CoreTestData = (RMT_CORE_TEST_DATA *) SsaServicesHandle->SsaCommonConfig->Malloc(
            SsaServicesHandle, sizeof (RMT_CORE_TEST_DATA));
  if (CoreTestData == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_233);
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&TestData, sizeof (TestData));
  ZeroMem (CoreTestData, sizeof (RMT_CORE_TEST_DATA));

  if (Config == NULL) {
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "ERROR: Config is NULL!\n");
    ProcError(SsaServicesHandle, Failure, __FILE__, __LINE__, CoreTestData->pTestStat);
    SsaServicesHandle->SsaCommonConfig->Free(SsaServicesHandle, CoreTestData);
    return EFI_UNSUPPORTED;
  }

  CoreTestData->pTestCfg = Config;
  CoreTestData->pCpgcInfo = &CpgcInfo;
#if SUPPORT_FOR_TURNAROUNDS
  CoreTestData.TurnaroundsPairMode = OrderIndependent;
#endif // SUPPORT_FOR_TURNAROUNDS
  CoreTestData->GetPointTestDimmRanks = GetCpgcPointTestDimmRanks;
  CoreTestData->SetPointTestRankCombinationIndex = SetCpgcPointTestRankCombinationIndex;
  CoreTestData->SetPointTestInBinarySearchState = SetCpgcPointTestInBinarySearchState;
  CoreTestData->RunPointTest = RunCpgcPointTest;
  CoreTestData->IsFirstCallToSetupCpgcPointTest = TRUE;

  // get metadata with no columnar results just in case we have an error
  ResultEleCnt = 1;
  if (SetupResults(SsaServicesHandle, CoreTestData, ResultEleCnt,
    &CoreTestData->pMetadata)) {
    SsaServicesHandle->SsaCommonConfig->Free(SsaServicesHandle, CoreTestData);
    return EFI_UNSUPPORTED;
  }

  CoreTestData->pTestStat = (TEST_STATUS*) &CoreTestData->pMetadata->StatusCode;

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
    ProcError(SsaServicesHandle, Failure, __FILE__, __LINE__, CoreTestData->pTestStat);
    SsaServicesHandle->SsaCommonConfig->Free(SsaServicesHandle, CoreTestData);
    return EFI_UNSUPPORTED;
  }

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "GetSystemInfo()\n");
#endif
  // get system information
  SsaServicesHandle->SsaMemoryConfig->GetSystemInfo(SsaServicesHandle,
    &CoreTestData->SystemInfo);
#if ENBL_VERBOSE_LOG_MSGS
  LogSystemInfo (&CoreTestData->SystemInfo);
#endif
  if (ChkSystemInfo(SsaServicesHandle, &CoreTestData->SystemInfo,
    CoreTestData->pTestStat)) {
    SsaServicesHandle->SsaCommonConfig->Free(SsaServicesHandle, CoreTestData);
    return EFI_UNSUPPORTED;
  }

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "GetCpgcInfo()\n");
#endif
  // get system information
  SsaServicesHandle->SsaMemoryServerConfig->GetCpgcInfo(SsaServicesHandle,
    &CpgcInfo);
#if ENBL_VERBOSE_LOG_MSGS
  LogCpgcInfo(SsaServicesHandle, &CpgcInfo);
#endif
  if (ChkCpgcInfo(SsaServicesHandle, CoreTestData->pCpgcInfo,
    CoreTestData->pTestStat)) {
    SsaServicesHandle->SsaCommonConfig->Free(SsaServicesHandle, CoreTestData);
    return EFI_UNSUPPORTED;
  }

  // get the memory configuration
  if (GetMemCfg(SsaServicesHandle, &CoreTestData->SystemInfo,
    &CoreTestData->MemCfg, CoreTestData->pTestStat)) {
    SsaServicesHandle->SsaCommonConfig->Free(SsaServicesHandle, CoreTestData);
    return EFI_UNSUPPORTED;
  }

#if ENBL_VERBOSE_LOG_MSGS
  LogMemCfg(SsaServicesHandle, &CoreTestData->SystemInfo, &CoreTestData->MemCfg);
#endif

  IsDdrT = FALSE;
  InitAsAll = TRUE;
  if (InitSelectMemPointTestCfg(SsaServicesHandle, CoreTestData, DdrLevel,
    MrcGtMax, Singles, IsDdrT, InitAsAll)) {
    SsaServicesHandle->SsaCommonConfig->Free(SsaServicesHandle, CoreTestData);
    return EFI_UNSUPPORTED;
  }

#if ENBL_VERBOSE_LOG_MSGS
  LogRmtTestCfg(SsaServicesHandle, CoreTestData->pTestCfg);
#endif
  // check the RMT test configuration
  if (ChkRmtTestCfg(SsaServicesHandle, CoreTestData,
    ((1 << DdrLevel) | (1 << LrbufLevel)), CpgcInfo.IsLoopCountExponential,
    CpgcInfo.MaxLoopCountVal, CpgcInfo.MaxBurstLengthExponentVal,
    CpgcInfo.MaxBurstLengthLinearVal)) {
    SsaServicesHandle->SsaCommonConfig->Free(SsaServicesHandle, CoreTestData);
    return EFI_UNSUPPORTED;
  }

  // set up the platform
  InitPlatform(SsaServicesHandle, &CoreTestData->SystemInfo,
    &CoreTestData->MemCfg, &CoreTestData->SelectMemPointTestCfg, Setup,
    CoreTestData->pTestCfg->ScramblerOverrideMode,
    TestData.PrevScramblerEnables, TestData.PrevSelfRefreshEnables);

  InitRCompUpdate(SsaServicesHandle, &CoreTestData->SystemInfo,
    &CoreTestData->MemCfg, &CoreTestData->SelectMemPointTestCfg, Setup,
    TestData.PrevRCompUpdateEnables);

  InitZQCal(SsaServicesHandle, &CoreTestData->SystemInfo, &CoreTestData->MemCfg,
    &CoreTestData->SelectMemPointTestCfg, Setup, TestData.PrevZQCalEnables);

  // initialize memory point test configuration to empty
  ZeroMem (&CoreTestData->SelectMemPointTestCfg,
    sizeof (CoreTestData->SelectMemPointTestCfg));

  // run the RMT
  if (RunRmt(SsaServicesHandle, CoreTestData, SetupSameRankForDataMargin,
    SetupSameRankForCmdMargin,
#if SUPPORT_FOR_TURNAROUNDS
    SetupTurnaroundForDataMargin,
#endif // SUPPORT_FOR_TURNAROUNDS
#if SUPPORT_FOR_DDRT
    SetupSameRankForEridMargin,
#endif // SUPPORT_FOR_DDRT
    NULL)) {
    SsaServicesHandle->SsaCommonConfig->Free(SsaServicesHandle, CoreTestData);
    return EFI_UNSUPPORTED;
  }

  // clean up the CPGC point test
  IsLastCall = TRUE;
  CleanupCpgcPointTest(SsaServicesHandle, &CoreTestData->SystemInfo,
    &CoreTestData->MemCfg, &CoreTestData->SelectMemPointTestCfg, IsLastCall,
    &CoreTestData->pCpgcPointTestData);

  IsDdrT = FALSE;
  InitAsAll = TRUE;
  if (InitSelectMemPointTestCfg(SsaServicesHandle, CoreTestData, DdrLevel,
    MrcGtMax, Singles, IsDdrT, InitAsAll)) {
    SsaServicesHandle->SsaCommonConfig->Free(SsaServicesHandle, CoreTestData);
    return EFI_UNSUPPORTED;
  }

  // clean up the platform
  InitZQCal(SsaServicesHandle, &CoreTestData->SystemInfo, &CoreTestData->MemCfg,
    &CoreTestData->SelectMemPointTestCfg, Cleanup, TestData.PrevZQCalEnables);

  InitRCompUpdate(SsaServicesHandle, &CoreTestData->SystemInfo,
    &CoreTestData->MemCfg, &CoreTestData->SelectMemPointTestCfg, Cleanup,
    TestData.PrevRCompUpdateEnables);

  InitPlatform(SsaServicesHandle, &CoreTestData->SystemInfo,
    &CoreTestData->MemCfg, &CoreTestData->SelectMemPointTestCfg, Cleanup,
    CoreTestData->pTestCfg->ScramblerOverrideMode,
    TestData.PrevScramblerEnables, TestData.PrevSelfRefreshEnables);

  SsaServicesHandle->SsaCommonConfig->Free(SsaServicesHandle, CoreTestData);

  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "RMT test v%u.%u.%u.%u complete\n",
    BSSA_TC_MAJOR_VER, BSSA_TC_MINOR_VER, BSSA_TC_REV_VER, BSSA_TC_BUILD_VER);

  return EFI_SUCCESS;
} // end function RmtTestEntryPoint

// end file RMT.c
