/** @file
Margin1DCore.h

Common core Margin1D algorithms.

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

#ifndef __MARGIN1D_CORE_H__
#define __MARGIN1D_CORE_H__



#include "ssabios.h"

#include "ChkRetStat.h"
#include "CpgcPointTest.h" // must go before auto-generated files
#include "InitPlatform.h"
#include "MemPointTest.h"
#include "MemShmoo.h"
#include <Library/MemoryEvContentLib.h>
#include "Margin1dResultMetadata.h"
#include "Margin1dResultColumns.h"

#pragma pack(push, 1)

typedef struct {
  // pointer to Margin1D test configuration structure
  CONST MARGIN_1D_CONFIG *pTestCfg;

  // pointer to result metadata structure
  MARGIN_1D_RESULT_METADATA *pMetadata;

  // pointer to test status structure
  TEST_STATUS *pTestStat;

  // system information structure
  MRC_SYSTEM_INFO SystemInfo;

  // pointer to CPGC information structure
  VOID *pCpgcInfo;

  // memory configuration structure
  MEM_CFG MemCfg;

  // memory shmoo test configuration structure
  MEM_SHMOO_CONFIG MemShmooCfg;

  // memory point test configuration structure with selected devices
  MEM_POINT_TEST_CONFIG SelectMemPointTestCfg;

  // memory point test configuration structure with current devices
  MEM_POINT_TEST_CONFIG CurrentMemPointTestCfg;

  // CPGC point test configuration structure
  CPGC_POINT_TEST_CONFIG CpgcPointTestCfg;

  // pointer to MemShmoo test data structure
  VOID *pMemShmooData;

  // pointer to CpgcPointTest test data structure
  VOID *pCpgcPointTestData;

  // margin parameter offset being tested
  INT16 TestingOffset;

  // per-channel array of rank combination counts - index as [IsDdrT]
  RANK_COMBINATION_CNTS RankCombinationCnts[2];

  // maximum rank combination count - index as [IsDdrT]
  UINT8 MaxRankCombinationsCnts[2];

  // current rank combinations index
  UINT8 CurrentRankCombinationIndex;

  // number of result elements used
  UINT32 ResultElementCount;

#if SUPPORT_FOR_EXTENDED_RANGE
  UINT32 FixupStartResultElementIndex;
#endif // SUPPORT_FOR_EXTENDED_RANGE

  // pointer to array of result elements
  MARGIN_1D_RESULT_COLUMNS *pResultElements;

  // flag indicating whether the system has DDR-T devices
  BOOLEAN HasDdrtDevices;

  // current flag indicating whether we're currently testing DDR-T devices
  BOOLEAN CurrentIsDdrT;

  // flag indicating whether we're testing backside Cmd/Ctl
  BOOLEAN IsBacksideCmdCtl;

#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
  // bitmask of whether DIMMs support backside command margining
  UINT16 BacksideCmdBitmasks[MAX_SOCKET_CNT];
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT

  // minimum/maximum margin parameter offsets - index as [IsDdrT]
  INT16 MinOffsets[2];
  INT16 MaxOffsets[2];

  // margin parameter step unit - index as [IsDdrT]
  UINT16 StepUnits[2];

  SHMOO_DIRECTION ShmooDirs[2];
  UINT8 ShmooDirCount;
  UINT8 CurrentShmooDirIdx;

  // arrays of previous platform configuration values
  BOOLEAN PrevZQCalEnables[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT];
  BOOLEAN PrevRCompUpdateEnables[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT];

#if SUPPORT_FOR_DDRT
  BOOLEAN HasTestedChnlWithMixedDdrtDdr4;
#endif // SUPPORT_FOR_DDRT
} CORE_TEST_DATA;

#pragma pack(pop)

/**
  Function used to initialize the product specific elements in the given
  results metadata structure.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to core test data structure.
**/
typedef VOID (*INIT_SPECIFIC_METADATA)(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT CORE_TEST_DATA *pCoreTestData);

/**
@brief
  This function checks the test configuration for correctness/consistency.
  It also fills in the given memory configuration structure.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to core test data structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 ChkMargin1DTestCfg(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT CORE_TEST_DATA *pCoreTestData);

/**
  @brief
  This function sets up the results.  It creates the results buffer and gets a
  pointer to the metadata structure.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to core test data structure.
  @param[in]       ResultEleCnt       Number of result elements required.
  @param[out]      ppMetadata         Pointer to pointer to results metadata structure.
**/
UINT8
SetupMargin1DResults (
  IN OUT SSA_BIOS_SERVICES_PPI      *SsaServicesHandle,
  IN OUT CORE_TEST_DATA             *pCoreTestData,
  IN     UINT32                      ResultEleCnt,
  OUT    MARGIN_1D_RESULT_METADATA **ppMetadata
  );

/*
@brief
  This function is used to run a Margin1D test.

  @param[in, out]  SsaServicesHandle     Pointer to SSA services.
  @param[in, out]  pCoreTestData         Pointer to core test data structure.
  @param[in]       InitSpecificMetadata  Pointer to function to initialize product specific metadata.  A NULL value is acceptable.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 RunMargin1DTest(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT CORE_TEST_DATA *pCoreTestData,
  IN INIT_SPECIFIC_METADATA InitSpecificMetadata);

/**
  This function is used to get the current targted DIMM/RANK from the core test data.
  The info is needed to report test results and set up the 1D sweep.

  @param[in ]  SsaServicesHandle  Pointer to SSA services.
  @param[in ]  pCoreTestData      Pointer to RMT core test data.
  @param[in ]  Socket             Socket number
  @param[out]  DimmRank           Pointer to the Dimm/rank array

**/
VOID
GetTargetedDimmRank (
  IN  SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN  CORE_TEST_DATA        *pCoreTestData,
  IN  UINT8                  Socket,
  OUT MRC_RT                (*DimmRank)[MAX_CH]
  );

#endif // __MARGIN1D_CORE_H__

// end file Margin1DCore.h
