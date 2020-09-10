/** @file
  RmtCore.h

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

#ifndef __RMT_CORE_H__
#define __RMT_CORE_H__

#include "ssabios.h"

#include "CpgcPointTest.h"
#include "MemCfg.h"
#include "MemPointTest.h"
#include "MemShmoo.h"
#include "EvContentInternal.h"
#include <Library/MemoryEvContentLib.h>
#include "RmtResultColumns.h"
#include "RmtResultMetadata.h"

// number of margin parameter group array elements
#define MARGIN_GROUP_ARRAY_ELE_CNT (4)

// number of margin parameter groups
#ifdef DDR5_SUPPORT
  #if SUPPORT_FOR_DDRT
  #define MARGIN_GROUP_ARRAY_CNT (4)
  #else
  #define MARGIN_GROUP_ARRAY_CNT (3)
  #endif // !SUPPORT_FOR_DDRT
#else
  #if SUPPORT_FOR_DDRT
  #define MARGIN_GROUP_ARRAY_CNT (3)
  #else
  #define MARGIN_GROUP_ARRAY_CNT (2)
  #endif // !SUPPORT_FOR_DDRT
#endif // DDR5_SUPPORT

//
// Index to the margin parameter group table - MarginGroups[MARGIN_GROUP_ARRAY_CNT][MARGIN_GROUP_ARRAY_ELE_CNT]
//
#define RMT_DATA_SIGNAL_GROUP    0
#define RMT_CMD_SIGNAL_GROUP     1
#ifdef DDR5_SUPPORT
#define RMT_BCOM_SIGNAL_GROUP    2
  #if SUPPORT_FOR_DDRT
  #define RMT_DDRT_SIGNAL_GROUP  3
  #endif
#else
  #if SUPPORT_FOR_DDRT
  #define RMT_DDRT_SIGNAL_GROUP  2
  #endif
#endif // DDR5_SUPPORT

//
// SKIP_TEST/MrcGtMax is used to indicate that it is an invalid/skip margin group
//
#define SKIP_TEST   MrcGtMax

typedef enum {
  LoRmtShmooDir,                  ///< low side
  HiRmtShmooDir,                  ///< high side
  RmtShmooDirMax,                 ///< RMT_SHMOO_DIR enumeration maximum value.
} RMT_SHMOO_DIR;

typedef enum {
  DDR,                  ///< DDR4
  DDRT,                  ///< DDRT
  MemTechMax,                 ///< MEM_TECH enumeration maximum value.
} MEM_TECH;

// RMT core test data
typedef struct {
  // pointer to RMT test configuration structure
  RMT_CONFIG *pTestCfg;

  // pointer to result metadata structure
  RMT_RESULT_METADATA *pMetadata;

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

  // memory point test configuration structure with select devices
  MEM_POINT_TEST_CONFIG SelectMemPointTestCfg;

#if SUPPORT_FOR_EXTENDED_RANGE
  // memory point test configuration structure with current devices
  MEM_POINT_TEST_CONFIG CurrentMemPointTestCfg;
#endif // SUPPORT_FOR_EXTENDED_RANGE

  // CPGC point test configuration structure
  CPGC_POINT_TEST_CONFIG CpgcPointTestCfg;

  // pointer to MemShmoo test data structure
  VOID *pMemShmooData;

  // pointer to CpgcPointTest test data structure
  VOID *pCpgcPointTestData;

  // function pointers
  GET_MEM_POINT_TEST_DIMM_RANKS GetPointTestDimmRanks;
  SET_MEM_POINT_TEST_RANK_COMBINATION_INDEX SetPointTestRankCombinationIndex;
  SET_MEM_POINT_TEST_IN_BINARY_SEARCH_STATE SetPointTestInBinarySearchState;
  RUN_MEM_POINT_TEST RunPointTest;

  // number of result elements used
  UINT16 ResultElementCount;

  // pointer to array of result elements
  RMT_RESULT_COLUMNS *pResultElements;

  // turnarounds rank pair mode
  TURNAROUNDS_PAIR_MODE TurnaroundsPairMode;

  // number of rank combinations per channel
  UINT8 RankCombinationCnts[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT][MAX_CHANNEL_CNT];

  // maximum rank combinations value
  UINT8 MaxRankCombinationCnt;

  // current rank combinations index
  UINT8 CurrentRankCombinationIndex;

  // DIMM+rank pairs
  DIMM_RANK_PAIR DimmRankAs[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT][MAX_CHANNEL_CNT];
  DIMM_RANK_PAIR DimmRankBs[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT][MAX_CHANNEL_CNT];

#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
  // bitmasks of whether DIMMs support backside command margining
  UINT16 BacksideCmdBitmasks[MAX_SOCKET_CNT];
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT

  // current flag indicating whether we're currently testing DDR-T devices
  BOOLEAN CurrentIsDdrT;

  // current I/O level
  MRC_LT CurrentIoLevel;

  // Pointer of margin groups
  MRC_GT MarginGroups[MARGIN_GROUP_ARRAY_CNT][MARGIN_GROUP_ARRAY_ELE_CNT];

  // current margin group array subindex
  UINT8 CurrentMarginGroupSubIndex;

  // current margin group
  MRC_GT CurrentMarginGroup;

  // current shmoo direction
  SHMOO_DIRECTION CurrentShmooDirection;

  // margin parameter offset we're testing at
  INT16 TestingOffset;

  // limit (min/max) offsets
  UINT8 LimitOffsets[MemTechMax][RmtShmooDirMax]; // 0: DDR; 1: DDRT.

  // last pass margin parameter offsets
  UINT8 LastPassLaneOffsets[RmtShmooDirMax][MAX_SOCKET_CNT][MAX_CONTROLLER_CNT][MAX_CHANNEL_CNT][MAX_BUS_WIDTH];

  // last pass found masks
  UINT8 LastPassFoundMasks[RmtShmooDirMax][MAX_SOCKET_CNT][MAX_CONTROLLER_CNT][MAX_CHANNEL_CNT][MAX_BUS_WIDTH/8];

  // flag indicating first call to SetupCpgcPointTest function
  BOOLEAN IsFirstCallToSetupCpgcPointTest;

  // number of result elements used (for debug)
  UINT32 UsedResultEleCount;
} RMT_CORE_TEST_DATA;

/**
  This function is used to setup same rank traffic for data signal margining.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to RMT core test data.
  @param[in]       IoLevel            I/O level.
  @param[in]       IsDdrT             Flag indicating whether DDR-T devices are being tested.
  @param[in]       EnableLaneMargin   Flag indicating whether lane margin data is being collected.

  @retval  0  success
  @retval  1  error occurred
**/
typedef UINT8 (*SETUP_RMT_SAME_RANK_FOR_DATA_MARGIN)(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT RMT_CORE_TEST_DATA *pCoreTestData,
  IN MRC_LT IoLevel,
  IN BOOLEAN IsDdrT,
  IN BOOLEAN EnableLaneMargin);

/**
  This function is used to setup same rank traffic for command/address/control
  signal margining.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to RMT core test data.
  @param[in]       IoLevel            I/O level.
  @param[in]       IsDdrT             Flag indicating whether DDR-T devices are being tested.

  @retval  0  success
  @retval  1  error occurred
**/
typedef UINT8 (*SETUP_RMT_SAME_RANK_FOR_CMD_MARGIN)(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT RMT_CORE_TEST_DATA *pCoreTestData,
  IN MRC_LT IoLevel,
  IN BOOLEAN IsDdrT);

#if SUPPORT_FOR_TURNAROUNDS
/**
  This function is used to setup rank-to-rank turnaround traffic for data
  signal margining.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to RMT core test data.
  @param[in]       IoLevel            I/O level.
  @param[in]       IsDdrT             Flag indicating whether DDR-T devices are being tested.
  @param[in]       TrafficMode        Memory traffic mode.

  @retval  0  success
  @retval  1  error occurred
**/
typedef UINT8 (*SETUP_RMT_TURNAROUND_FOR_DATA_MARGIN)(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT RMT_CORE_TEST_DATA *pCoreTestData,
  IN MRC_LT IoLevel,
  IN BOOLEAN IsDdrT,
  IN TRAFFIC_MODE TrafficMode);
#endif // SUPPORT_FOR_TURNAROUNDS

#if SUPPORT_FOR_DDRT
/**
  This function is used to setup same rank traffic for DDR-T Early Read ID
  signal margining.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to RMT core test data.

  @retval  0  success
  @retval  1  error occurred
**/
typedef UINT8 (*SETUP_RMT_SAME_RANK_FOR_ERID_MARGIN)(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT RMT_CORE_TEST_DATA *pCoreTestData);
#endif // SUPPORT_FOR_DDRT

/**
  Function used to initialize the product specific elements in the given
  results metadata structure.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to RMT core test data.
**/
typedef VOID (*INIT_SPECIFIC_METADATA)(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT RMT_CORE_TEST_DATA *pCoreTestData);

/**
  This function is used to get the mask value for the given margin group.  The
  mask value is used for the *Mask test configuration values.

  @param[in]  MarginGroup  Margin group.

  @retval Mask value
**/
UINT16 GetMarginGroupMask(
  MRC_GT MarginGroup);

/**
  This function is used to set up the results.  It creates the results buffer
  and gets a pointer to the metadata structure.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to RMT core test data.
  @param[in]       ResultEleCnt       Number of RMT result elements required.
  @param[out]      ppMetadata         Pointer to pointer to results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 SetupResults(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT RMT_CORE_TEST_DATA *pCoreTestData,
  IN UINT32 ResultEleCnt,
  OUT RMT_RESULT_METADATA **ppMetadata);

/**
  This function is used to log the RMT test configuration.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pRmtTestCfg        Pointer to RMT test configuration structure.
**/
VOID LogRmtTestCfg(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST RMT_CONFIG *pRmtTestCfg);

/**
  This function is used to check the RMT test configuration for correctness.
  Some test configuration values may be updated based on their value and the
  given memory configuration.

  @param[in, out]  SsaServicesHandle          Pointer to SSA services.
  @param[in, out]  pCoreTestData              Pointer to RMT core test data.
  @param[in]       MaxIoLevelBitMask          Maximum I/O level bitmask value.
  @param[in]       IsLoopCountExponential     Boolean value indicating whether or not loop count value is exponential.
  @param[in]       MaxLoopCountVal            The maximum loop count value.  If IsLoopCountExponential is TRUE then the MaxLoopCountVal value is the maximum exponent value.
  @param[in]       MaxBurstLengthExponentVal  The maximum burst length exponent value.  For a CPGC engine that doesn't support exponential burst length values, this value should be 0.
  @param[in]       MaxBurstLengthLinearVal    The maximum burst length linear value.  Some CPGC engines use linear for smaller values and exponential (2^n) for larger values.  For a CPGC engine that doesn't support linear burst length values, this value should be 0.

  @retval  0  success
  @retval  1  error occurred
**/
SSA_STATUS ChkRmtTestCfg(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT RMT_CORE_TEST_DATA *pCoreTestData,
  IN UINT8 MaxIoLevelBitMask,
  IN BOOLEAN IsLoopCountExponential,
  IN UINT32  MaxLoopCountVal,
  IN UINT8   MaxBurstLengthExponentVal,
  IN UINT32  MaxBurstLengthLinearVal);


/**
  @brief
  This function get the RMT test config ERID setting.

  @param[in]  pTestCfg   Pointer to RMT test configuration structure.

  @retval TRUE/FALSE - ERID enable or disable
  **/
BOOLEAN
GetRmtTestCfgErid (
  IN RMT_CONFIG *pTestCfg
  );

/**
  @brief
  This function initializes the MarginGroups array with the project specific groups.

  @param[in]       IoLevel        I/O level.
  @param[in, out]  MarginGroups   An array structure with the margin groups.

  @retval N/A
**/
VOID
InitMarginGroups (
  IN     MRC_LT IoLevel,
  IN OUT MRC_GT (*MarginGroups)[MARGIN_GROUP_ARRAY_CNT][MARGIN_GROUP_ARRAY_ELE_CNT]
  );

/**
  @brief
  This function is the RMT memory point test result handler function.

  This function conforms to the MEM_POINT_TEST_RESULT_HANDLER typedef in MemPointTest.h.

  @param[in, out]  SsaServicesHandle     Pointer to SSA services.
  @param[in]       pSystemInfo           Pointer to system information structure.
  @param[in, out]  pVoidTestData         Pointer to client test data.
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
UINT8 RmtPointTestResultHandler(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN OUT VOID *pVoidTestData,
  IN UINT8 Socket,
  IN UINT8 Controller,
  IN UINT8 Channel,
  IN DIMM_RANK_PAIR DimmRankA,
  IN DIMM_RANK_PAIR DimmRankB,
  IN UINT8 RankCombinationIndex,
  IN CONST UINT8 *pLaneErrorStatus,
  IN ERROR_COUNT_32BITS ErrorCount,
  OUT TEST_STATUS *pTestStat);

/**
  This function is used to initialize the select memory point test
  configuration structure in the given core test data structure.

  The MarginGroup input parameter is to support the special case where the
  frontside CmdVref and CtlAll margining needs to support all ranks for LRDIMMs
  whereas all other margin parameters are only tested on the first rank.  Use a
  value of CmdVref for the special case and use a value of MrcGtMax for all
  other cases.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to RMT core test data.
  @param[in]       IoLevel            I/O level.
  @param[in]       MarginGroup        Margin group.
  @param[in]       RankCombination    Rank combination.
  @param[in]       IsDdrT             Flag indicating that DDR-T devices are being tested.
  @param[in]       InitAsAll          Flag indicating whether to initialize as all devices

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 InitSelectMemPointTestCfg(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT RMT_CORE_TEST_DATA *pCoreTestData,
  IN MRC_LT IoLevel,
  IN MRC_GT MarginGroup,
  IN RANK_TEST_MODE RankTestMode,
  IN BOOLEAN IsDdrT,
  IN BOOLEAN InitAsAll);

/**
  The function is used to run the RMT test.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to RMT core test data.
  @param[in]       SetupSameRankForDataMargin  Pointer to function to setup point test for same rank traffic for data signal margining.
  @param[in]       SetupSameRankForCmdMargin  Pointer to function to setup point test for same rank traffic for command/address/control signal margining.
  @param[in]       SetupTurnaroundForDataMargin  Pointer to function to setup point test for rank-to-rank turnaround traffic for data signal margining.
  @param[in]       SetupSameRankForEridMargin  Pointer to function to setup point test for same rank traffic for DDR-T Early Read ID signal margining.
  @param[in]       InitSpecificMetadata  Pointer to function to initialize product specific metadata.  A NULL value is acceptable.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 RunRmt(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT RMT_CORE_TEST_DATA *pCoreTestData,
  IN SETUP_RMT_SAME_RANK_FOR_DATA_MARGIN SetupSameRankForDataMargin,
  IN SETUP_RMT_SAME_RANK_FOR_CMD_MARGIN SetupSameRankForCmdMargin,
#if SUPPORT_FOR_TURNAROUNDS
  IN SETUP_RMT_TURNAROUND_FOR_DATA_MARGIN SetupTurnaroundForDataMargin,
#endif
#if SUPPORT_FOR_DDRT
  IN SETUP_RMT_SAME_RANK_FOR_ERID_MARGIN SetupSameRankForEridMargin,
#endif
  IN INIT_SPECIFIC_METADATA InitSpecificMetadata);

#endif // __RMT_CORE_H__

// end file RmtCore.h
