/** @file
Margin2DCore.h

Common core Margin2D algorithms.

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

#ifndef __MARGIN2D_CORE_H__
#define __MARGIN2D_CORE_H__


#include "ssabios.h"

#include "ChkRetStat.h"
#include "CpgcPointTest.h" // must go before auto-generated files
#include "InitPlatform.h"
#include "MemPointTest.h"
#include "MemShmoo.h"
#include <Library/MemoryEvContentLib.h>
#include "Margin2dResultMetadata.h"
#include "Margin2dResultColumns.h"

typedef struct {
  MRC_LT IoLevel;
  MRC_GT SlowMarginGroup;
  MRC_GT FastMarginGroup;

  UINT8 SlowStepSize;
  UINT8 FastStepSize;

  RANK_TEST_MODE RankTestMode;
  UINT8 Socket;
  UINT8 Controller;
  UINT8 Channel;
  UINT8 DimmA;
  UINT8 DimmB;
  UINT8 RankA;
  UINT8 RankB;
  UINT8 LaneValBitmasks[MAX_BITS / 8];
  BOOLEAN IsDdrT;

  BOOLEAN EnableMarginPlot;
  UINT8 TestStatusLogLevel;
} MARGIN2D_CORE_CONFIG;

typedef struct {
  // pointer to Margin2D test configuration structure
  MARGIN_2D_CONFIG *pTestCfg;

  // Margin2D core test configuration structure
  MARGIN2D_CORE_CONFIG CoreTestCfg;

  // pointer to result metadata structure
  MARGIN2D_RESULT_METADATA *pMetadata;

  // pointer to test status structure
  TEST_STATUS *pTestStat;

  // system information structure
  MRC_SYSTEM_INFO SystemInfo;

  // pointer to CPGC information structure
  VOID* pCpgcInfo;

  // memory configuration structure
  MEM_CFG MemCfg;

  // memory shmoo test configuration structure
  MEM_SHMOO_CONFIG MemShmooCfg;

  // memory point test configuration structure
  MEM_POINT_TEST_CONFIG MemPointTestCfg;

  // CPGC point test configuration structure
  CPGC_POINT_TEST_CONFIG CpgcPointTestCfg;

  // pointer to MemShmoo test data structure
  VOID *pMemShmooData;

  // pointer to CpgcPointTest test data structure
  VOID *pCpgcPointTestData;

  // shmoo parameter max/min offset
  INT16 FastMaxOffset;
  INT16 FastMinOffset;
  INT16 SlowMaxOffset;
  INT16 SlowMinOffset;

  // margin parameter settle delays
  UINT16 FastSettleDelay;
  UINT16 SlowSettleDelay;

  // margin parameter step units
  UINT16 FastStepUnit;
  UINT16 SlowStepUnit;

  // flags enabling incremental stepping
  BOOLEAN FastEnableIncrementalStepping;
  BOOLEAN SlowEnableIncrementalStepping;

  // current fast and slow shmoo directions
  SHMOO_DIRECTION FastShmooDirection;
  SHMOO_DIRECTION SlowShmooDirection;

  // testing fast and slow margin parameter offsets
  INT16 TestingFastOffset;
  INT16 TestingSlowOffset;

  // current fast and slow margin parameter offsets
  INT16 CurrentFastOffset;
  INT16 CurrentSlowOffset;

  // fast margin parameter offset of the first failure for the current shmoo
  INT16 FirstFailOffset;

  // lane error status at first fail offset for high and low shmoo directions
  UINT8 FirstFailLaneErrorStatusHi[MAX_BUS_WIDTH / 8];
  UINT8 FirstFailLaneErrorStatusLo[MAX_BUS_WIDTH / 8];

  // number of result elements used
  UINT32 ResultElementCount;

  // pointer to array of result elements
  MARGIN2D_RESULT_COLUMNS *pResultElements;
} MARGIN2D_CORE_DATA;

/**
  Function used to initialize the product specific elements in the given
  results metadata structure.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to Margin2D core test data structure.
**/
typedef VOID (*INIT_SPECIFIC_METADATA)(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT MARGIN2D_CORE_DATA *pCoreTestData);

/**
@brief
  This function checks the test configuration for correctness/consistency.
  It also fills in the given memory configuration structure.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to Margin2D core test data structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 ChkMargin2DTestCfg(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT MARGIN2D_CORE_DATA *pCoreTestData);

/**
@brief
  This is the pre-DIMM-initialization handler function.  The function will be
  called by the MemShmoo code immediately before a DIMM initialization is
  performed.

  This function conforms to the PRE_DIMM_INIT_HANDLER function typedef
  published in MemShmoo.h.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pClientTestCfg     Pointer to client test configuration structure.
  @param[in, out]  pClientTestData    Pointer to client test data structure.
  @param[out]      pTestStat          Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 PreDimmInitHandler(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST VOID *pClientTestCfg,
  IN OUT VOID *pClientTestData,
  OUT TEST_STATUS *pTestStat);

/**
@brief
  This is the post-DIMM-initialization handler function.  The function will be
  called by the MemShmoo code immediately after a DIMM initialization is
  performed.

  This function conforms to the POST_DIMM_INIT_HANDLER function typedef
  published in MemShmoo.h.

  @param[in, out]  SsaServicesHandle      Pointer to SSA services.
  @param[in]       pSystemInfo            Pointer to system information structure.
  @param[in]       pMemCfg                Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg       Pointer to generic memory point test configuration structure.
  @param[in]       pClientTestCfg         Pointer to client test configuration structure.
  @param[in, out]  pClientTestData        Pointer to client test data structure.
  @param[in]       IsLastRankCombination  Flag indicating that this is the last rank combination.
  @param[out]      pTestStat              Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 Margin2DPostDimmInitHandler(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN CONST VOID *pClientTestCfg,
  IN OUT VOID *pClientTestData,
  IN BOOLEAN IsLastRankCombination,
  OUT TEST_STATUS *pTestStat);

/**
@brief
  This is the pre-RunPointTest handler function.  The function will be called
  by the MemShmoo code immediately before a PointTest is performed.

  This function conforms to the PRE_RUN_POINT_TEST_HANDLER function typedef
  published in MemShmoo.h.

  @param[in, out]  SsaServicesHandle      Pointer to SSA services.
  @param[in]       pSystemInfo            Pointer to system information structure.
  @param[in]       pMemCfg                Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg       Pointer to generic memory point test configuration structure.
  @param[in]       pClientTestCfg         Pointer to client test configuration structure.
  @param[in, out]  pClientTestData        Pointer to client test data structure.
  @param[out]      pTestStat              Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 PreRunPointTestHandler(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN CONST VOID *pClientTestCfg,
  IN OUT VOID *pClientTestData,
  OUT TEST_STATUS *pTestStat);

/**
@brief
  This is the definition for the point test result handler function.

  This function conforms to the MEM_POINT_TEST_RESULT_HANDLER function typedef
  published in MemPointTest.h.

  @param[in, out]  SsaServicesHandle     Pointer to SSA services.
  @param[in]       pSystemInfo           Pointer to system information structure.
  @param[in]       pClientTestData       Pointer to client test data.
  @param[in]       Socket                Socket tested.
  @param[in]       Controller            Controller tested.
  @param[in]       Channel               Channel tested.
  @param[in]       DimmRankA             First DIMM+Rank tested.
  @param[in]       DimmRankB             Second DIMM+Rank tested.
  @param[in]       RankCombinationIndex  Rank combination index tested.
  @param[in]       pLaneErrorStatus      Pointer to array of lane error status masks.  Number of array elements is SystemInfo.BusWidth/8
  @param[in]       ErrorCount            Channel error count.
  @param[out]      pTestStat             Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 Margin2DPointTestResultHandler(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN VOID *pClientTestData,
  IN UINT8 Socket,
  IN UINT8 Controller,
  IN UINT8 Channel,
  IN DIMM_RANK_PAIR DimmRankA,
  IN DIMM_RANK_PAIR DimmRankB,
  IN UINT8 RankCombinationIndex,
  IN CONST UINT8 *pLaneErrorStatus,
  IN ERROR_COUNT_32BITS ErrorCount,
  OUT TEST_STATUS *pTestStat);

/*
@brief
  This function is used to run a margin 2D test.

  @param[in, out]  SsaServicesHandle   Pointer to SSA services.
  @param[in, out]  pCoreTestData       Pointer to Margin2D core test data structure.
  @param[in]       InitSpecificMetadata  Pointer to function to initialize product specific metadata.  A NULL value is acceptable.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 RunMargin2D(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT MARGIN2D_CORE_DATA *pCoreTestData,
  IN INIT_SPECIFIC_METADATA InitSpecificMetadata);

/*
  @brief
  This function is used to run a margin 2D test with 1D sweep.

  @param[in]       SsaServicesHandle   Pointer to SSA services.
  @param[in, out]  pCoreTestData       Pointer to Margin2D core test data structure.
  @param[in]       InitSpecificMetadata  Pointer to function to initialize product specific metadata.  A NULL value is acceptable.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8
RunMargin2DWith1DSweep (
  IN     SSA_BIOS_SERVICES_PPI  *SsaServicesHandle,
  IN OUT MARGIN2D_CORE_DATA     *pCoreTestData,
  IN     INIT_SPECIFIC_METADATA InitSpecificMetadata
  );

/**
@brief
  This function performs JEDEC initializations at the end of the test (if
  required) to ensure the DIMMs are left in a functional state.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to core test data structure.
**/
VOID Margin2DCleanupJedecInit(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT MARGIN2D_CORE_DATA *pCoreTestData);

/**
@brief
  This function sets up the results.  It creates the results buffer and gets a
  pointer to the metadata structure.

  @param[in, out]  SsaServicesHandle   Pointer to SSA services.
  @param[in]       ResultEleCnt        The number of result elements required.
  @param[out]      ppMetadata          Pointer to pointer to results metadata structure.
**/
UINT8 SetupMargin2DResults(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN UINT32 ResultEleCnt,
  OUT MARGIN2D_RESULT_METADATA **ppMetadata);

/**
  This function is used to get the current targted DIMM/RANK from the core test data.
  The info is needed to report test results and set up the 1D sweep

  @param[in ]  SsaServicesHandle  Pointer to SSA services.
  @param[in ]  pCoreTestData      Pointer to Margin2D core test data.
  @param[in ]  Socket             Socket number
  @param[out]  DimmRank           Pointer to the Dimm/rank array

**/
VOID
GetMargin2DTargetedDimmRank (
  IN SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN MARGIN2D_CORE_DATA    *pCoreTestData,
  IN UINT8                  Socket,
  OUT MRC_RT                (*DimmRank)[MAX_CH]
  );
#endif // __MARGIN2D_CORE_H__

// end file Margin2DCore.h
