/** @file
  CpgcPointTest.h

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

#ifndef __CPGC_POINT_TEST_H__
#define __CPGC_POINT_TEST_H__

#include "ssabios.h"
#include "ChkRetStat.h"
#include "MemPointTest.h"
#include <Library/MemoryEvContentLib.h>

#pragma pack(push, 1)


// number of user defined CADB pattern rows
#define USER_DEFINED_CADB_PATTERN_ROW_CNT 16

// maximum number of subsequences
#define MAX_SUBSEQ_CNT 10

// DDRT subseq wait time
#define DDRT_SUBSEQ_WAIT 0xF0


//////////////////////////////////////////////////////////////////////////
// CPGC Address scheme. Same struct was defined in MRC\include\cpgc.h
//////////////////////////////////////////////////////////////////////////

typedef enum {
  RankAddrField,
  BankAddrField,
  RowAddrField,
  ColAddrField,
  CpgcAddrFieldMax,                       ///< CPGC_ADDR_FIELD enumeration maximum value.
  CpgcAddrFieldDelim = MAX_INT32          ///< This values ensures that different environments use the same size for enum values.
} CPGC_ADDR_FIELD;

typedef struct {
  UINT16 Start[CpgcAddrFieldMax];                   // (4, uint16)    // Rank, Bank, Row, Col
  UINT32 Wrap[CpgcAddrFieldMax];                    // (4, uint16)    // Rank, Bank, Row, Col
  UINT8  Order[CpgcAddrFieldMax];                   // [4, uint8)     // Rank, Bank, Row, Col
  ADDR_INC_MODE IncMode[CpgcAddrFieldMax];          // (4, unit32)    // Rank, Bank, Row, Col
  UINT32 IncRate[CpgcAddrFieldMax];                 // (4, unit32)    // Rank, Bank, Row, Col
  INT16 IncVal[CpgcAddrFieldMax];                   // (4, unit8)     // Rank, Bank, Row, Col
  BOOLEAN CarryEnable[CpgcAddrFieldMax];            // (4, unit8)     // Rank, Bank, Row, Col
} CPGC_ADDRESS;

//////////////////////////////////////////////////////////////////////////
/// CPGC sequencer
//////////////////////////////////////////////////////////////////////////
typedef struct {
  UINT8 SubSequenceStart;
  UINT8 SubSequenceEnd;
  UINT8 SubSequenceType[MAX_SUBSEQ_CNT];
  BOOLEAN ResetBaseAddr[MAX_SUBSEQ_CNT];
} CPGC_SEQUENCER;

// memory point test data
typedef struct {
  // current DIMM and rank A/B values
  DIMM_RANK_PAIR CrntDimmRankA;
  DIMM_RANK_PAIR CrntDimmRankB;
  // current chunk validation mask, we need it for the GetDqErrorStatus()
  UINT8 CrntChunkSelectMask;
} CPGC_PER_CHNL_POINT_TEST_DATA;

// memory point test data
typedef struct {
  // test status logging level
  UINT8 TestStatusLogLevel;

  // the maximum number of rank combinations
  UINT8 MaxRankCombinationsCnt;

  // dynamically allocated array of per channel rank and chunk mask status
  // indexed based on the tested channel index starting from socket->controller->channel
  UINT8 TestedChnlCnt;
  CPGC_PER_CHNL_POINT_TEST_DATA *pChannelTestData;

  // the number of rank combinations per channel
  RANK_COMBINATION_CNTS RankCombinationsCnts;

  // the current rank combination index
  UINT8 RankCombinationsIndex;

  // lane failure status; indexed as [Socket][Controller][Channel]
  LANE_FAIL_STATS LaneFailStats;

  // flag indicating that at we're in the binary search state
  BOOLEAN InBinarySearchState;

  // function pointer for result handler
  MEM_POINT_TEST_RESULT_HANDLER ResultHandler;

  // pointer to client data - used during callbacks to client
  VOID *pClientTestData;

  // values saved from call to SetupCpgcPointTest function
  MRC_LT IoLevel;
  BOOLEAN IsDdrT;

#if SUPPORT_FOR_TURNAROUNDS
  // address schemas for turnaround traffic
  CPGC_ADDRESS TurnaroundCpgcAddr0; // for Wr2WrAndRd2Rd and the first part of Wr2Rd
  CPGC_ADDRESS TurnaroundCpgcAddr1; // for Rd2Wr and the second part of Wr2Rd
#endif // SUPPORT_FOR_TURNAROUNDS


} CPGC_POINT_TEST_DATA;

typedef union {
  UINT32 Uint32[2];
  UINT64 Uint64;
} UNION_64;

#if MAX_LOGICAL_RANK_CNT == 1
  #define MAX_RANK_ITER 1
#else
  #define MAX_RANK_ITER (MAX_LOGICAL_RANK_CNT * (MAX_LOGICAL_RANK_CNT - 1))
#endif

#pragma pack(pop)

/**
@brief
  This function checks the given CPGC information values against the
  corresponding preprocessor definition values.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pCpgcInfo          Pointer to CPGC information structure.
  @param[out]      pTestStat          Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 ChkCpgcInfo(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN MRC_SERVER_CPGC_INFO *pCpgcInfo,
  OUT TEST_STATUS *pTestStat);

/**
@brief
  This function logs the CPGC information.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pCpgcInfo          Pointer to CPGC information structure.
**/
VOID LogCpgcInfo(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN MRC_SERVER_CPGC_INFO *pCpgcInfo);

/**
@brief
  This function sets default values in the given CPGC point test configuration.

  @param[out]  pTestCfg           Pointer to CPGC point test configuration structure.
**/
VOID SetCpgcPointTestCfgDefaults(
  OUT CPGC_POINT_TEST_CONFIG *pTestCfg);

/**
@brief
  This function is used to check the CPGC point test configuration for
  correctness/consistency.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pCpgcInfo          Pointer to CPGC information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
  @param[in]       pCpgcPointTestCfg  Pointer to CPGC point test configuration structure.
  @param[out]      pTestStat          Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 ChkCpgcPointTestCfg(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MRC_SERVER_CPGC_INFO *pCpgcInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN CONST CPGC_POINT_TEST_CONFIG *pCpgcPointTestCfg,
  OUT TEST_STATUS *pTestStat);

/**
@brief
  This function is used to log the CPGC point test configuration.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pCpgcPointTestCfg  Pointer to test configuration structure.
**/
VOID LogCpgcPointTestCfg(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST CPGC_POINT_TEST_CONFIG *pCpgcPointTestCfg);

/**
@brief
  This function is used to set up the CPGC point test.

  This function conforms to the SETUP_MEM_POINT_TEST function typedef in
  MemPointTest.h.

  If the IsFirstCall input parameter is set to TRUE then this function will
  allocate memory for the CpgcPointTest's private test status data structure,
  otherwise it will treat the ppPointTestData parameter as an input.

  If the InitDqdbContent input parameter is set to TRUE then this function will
  initialize the contents of the WDB, otherwise, it won't.

  @param[in, out]  SsaServicesHandle   Pointer to SSA services.
  @param[in]       pSystemInfo         Pointer to system information structure.
  @param[in]       pCpgcInfo           Pointer to CPGC information structure.
  @param[in]       pMemCfg             Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg    Pointer to generic memory point test configuration structure.
  @param[in]       pCpgcPointTestCfg   Pointer to CPGC point test configuration structure.
  @param[in]       ResultHandler       Pointer to result handler function.
  @param[in]       pClientTestData     Pointer to client test data structure.
  @param[in]       IoLevel             I/O level.
  @param[in]       IsDdrT              Flag indicating whether DDR-T devices are being tested.
  @param[in]       TestStatusLogLevel  Test status logging level.
  @param[in]       IsFirstCall         Flag indicating whether the call is the first.
  @param[in]       InitDqdbContent     Flag indicating whether to initialize the DQDB contents.
  @param[in, out]  ppPointTestData     Pointer to pointer to specific memory point test data
  @param[out]      pTestStat           Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 SetupCpgcPointTest(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MRC_SERVER_CPGC_INFO *pCpgcInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN CONST CPGC_POINT_TEST_CONFIG *pCpgcPointTestCfg,
  IN MEM_POINT_TEST_RESULT_HANDLER ResultHandler,
  IN VOID *pClientTestData,
  IN MRC_LT IoLevel,
  IN BOOLEAN IsDdrT,
  IN UINT8 TestStatusLogLevel,
  IN BOOLEAN IsFirstCall,
  IN BOOLEAN InitDqdbContent,
  IN OUT VOID **ppPointTestData,
  OUT TEST_STATUS *pTestStat);

/**
@brief
  This function is used to get the size of required CpgcPointTest dynamic memory.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pCpgcPointTestCfg  Pointer to CPGC point test configuration structure.
  @param[in]       TestedChnlCnt      Number of channels being tested.

  @retval  dynamic memory size
**/
UINT32 GetCpgcPointTestDynamicMemSize(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST CPGC_POINT_TEST_CONFIG *pCpgcPointTestCfg,
  IN UINT8 TestedChnlCnt);

/**
@brief
  This function is used to get the DIMM/rank pairs for a given channel.

  This function conforms to the GET_MEM_POINT_TEST_DIMM_RANKS typedef in
  MemPointTest.h.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
  @param[in]       pPointTestCfg      Pointer to specific memory point test configuration structure.
  @param[in]       pPointTestData     Pointer to specific memory point test data structure.
  @param[in]       Socket             Zero based CPU socket number.
  @param[in]       Controller         Zero based controller number.
  @param[in]       Channel            Zero based channel number.
  @param[in, out]  pPairCount         Number of entries in the DimmRankPairs array.  Input=number of elements in pDimmRankPairs.  Output=number of populated elements in pDimmRankPairs.
  @param[out]      pDimmRankPairs     Array of DIMM and physical rank pair values.
  @param[out]      pTestStat          Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 GetCpgcPointTestDimmRanks(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN VOID *pVoidPointTestData,
  IN UINT8 Socket,
  IN UINT8 Controller,
  IN UINT8 Channel,
  IN OUT UINT8 *pPairCount,
  OUT DIMM_RANK_PAIR *pDimmRankPairs,
  OUT TEST_STATUS *pTestStat);

/**
@brief
  This function is used to set the rank combination index to the given value.
  This function configures that hardware to run the point test for the given
  rank index.

  This function conforms to the SET_MEM_POINT_TEST_RANK_COMBINATION_INDEX
  typedef in MemPointTest.h.

  @param[in, out]  SsaServicesHandle     Pointer to SSA services.
  @param[in]       pSystemInfo           Pointer to system information structure.
  @param[in]       pMemCfg               Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg      Pointer to generic memory point test configuration structure.
  @param[in]       pPointTestCfg         Pointer to specific memory point test configuration structure.
  @param[in]       pPointTestData        Pointer to specific memory point test data structure.
  @param[in]       RankCombinationIndex  Rank combination index.
  @param[in]       UpdateHwSetting       Flag to indicate if need to apply the setting to register or not
  @param[out]      pTestStat             Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 SetCpgcPointTestRankCombinationIndex(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN CONST VOID *pPointTestCfg,
  IN VOID *pPointTestData,
  IN UINT8 RankCombinationIndex,
  IN BOOLEAN UpdateHwSetting,
  OUT TEST_STATUS *pTestStat);

/**
@brief
  This function is used to set the flag indicating whether we're in the binary
  search state.  It is intended that the point test runs for a shorter period
  of time in the binary search state.

  This function conforms to the SET_MEM_POINT_TEST_IN_BINARY_SEARCH_STATE
  typedef in MemPointTest.h.

  @param[in, out]  SsaServicesHandle    Pointer to SSA services.
  @param[in]       pSystemInfo          Pointer to system information structure.
  @param[in]       pMemCfg              Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg     Pointer to generic memory point test configuration structure.
  @param[in]       pPointTestCfg        Pointer to specific memory point test configuration structure.
  @param[in]       pPointTestData       Pointer to specific memory point test data structure.
  @param[in]       InBinarySearchState  Flag indicating whether we're in the binary search state.
  @param[out]      pTestStat            Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 SetCpgcPointTestInBinarySearchState(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN CONST VOID *pPointTestCfg,
  IN VOID *pPointTestData,
  IN BOOLEAN InBinarySearchState,
  OUT TEST_STATUS *pTestStat);

/**
@brief
  This function is used to run a CPGC point test.

  This function conforms to the RUN_MEM_POINT_TEST typedef in MemPointTest.h.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
  @param[in]       pPointTestCfg      Pointer to specific memory point test configuration structure.
  @param[in]       pPointTestData     Pointer to specific memory point test data structure.
  @param[out]      pLaneFailStats     Pointer to lane failure status array.  Indexed as [Socket][Controller][Channel].  (Value may be NULL.)
  @param[out]      pTestStat          Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 RunCpgcPointTest(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN CONST VOID *pPointTestCfg,
  IN VOID *pPointTestData,
  OUT LANE_FAIL_STATS *pLaneFailStats,
  OUT TEST_STATUS *pTestStat);

/**
@brief
  This function is used to clean up the CPGC point test.

  This function conforms to the CLEANUP_MEM_POINT_TEST function typedef in
  MemPointTest.h.

  If the IsLastCall input parameter is set to TRUE then this function will free
  the memory for the CpgcPointTest's private test status data structure and set
  the pointer to NULL, otherwise it will not.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
  @param[in]       IsLastCall         Flag indicating whether the call is the last.
  @param[in, out]  ppPointTestData    Pointer to pointer to specific memory point test data structure.
**/
VOID CleanupCpgcPointTest(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN BOOLEAN IsLastCall,
  IN OUT VOID **ppPointTestData);

/**
@brief
  This function rotates the given DqDB chunk value to the left the given number
  of bits.  This function returns the rotated value.

  @param[in]  Pattern   Pattern to rotate.
  @param[in]  BitCount  Number of bit positions to rotate.

  @retval  Rotated pattern.
**/
UINT64 RotateDqdbChunk(
  IN UINT64 Pattern,
  IN UINT8 BitCount);

/**
@brief
  This function is used to set the given bits in the given buffer according to
  the given temporal pattern.

  @param[in]   SpatialMask         Spatial mask of bits to set in buffer.
  @param[in]   TemporalPattern     Temporal pattern.
  @param[in]   TemporalPatternLen  Temporal pattern length.
  @param[out]  pBfr                Pointer to buffer.
**/
VOID SetSpatialPattern(
  IN UINT64 SpatialMask,
  IN UINT8 TemporalPattern,
  IN UINT8 TemporalPatternLen,
  OUT UINT64* pBfr);

/**
@brief
  This function is used to initialize the DqDB pattern and unisequencers.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pCpgcInfo          Pointer to CPGC information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
  @param[in]       pCpgcPointTestCfg  Pointer to CPGC point test configuration structure.
  @param[in]       InitDqdbContent    Flag indicating whether to initialize the DQDB contents.
  @param[out]      pTestStat          Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 InitDqdb(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MRC_SERVER_CPGC_INFO *pCpgcInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN CONST CPGC_POINT_TEST_CONFIG *pCpgcPointTestCfg,
  IN BOOLEAN InitDqdbContent,
  OUT TEST_STATUS *pTestStat);

/**
@brief
  This function is used to change the CPGC point test result handler

  @param[in, out]  SsaServicesHandle        Pointer to SSA services.
  @param[in]       ResultHandler            Pointer to result handler function.
  @param[in, out]  pVoidPointTestData       pointer to CPCG point test data

  @retval  0  success
  @retval  1  error occurred
**/
UINT8
ChangeCpgcPointTestResultHandler (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN MEM_POINT_TEST_RESULT_HANDLER ResultHandler,
  IN OUT VOID *pVoidPointTestData
  );

/**
  @brief
  This function updates the CPGC point test ERID configuration setting

  @param[out]  pRunCpgcPointTestCfg   Pointer to RunCpgcPointTest test configuration structure.
  @param[in]   EnableErid             ERID setting.

**/
VOID
SetCpgcPointTestCfgErid (
  OUT CPGC_POINT_TEST_CONFIG *pCpgcPointTestCfg,
  IN  BOOLEAN                 EnableErid
  );

/**
  @brief
  This function get the CPGC point test ERID configuration setting

  @param[in]  pRunCpgcPointTestCfg   Pointer to RunCpgcPointTest test configuration structure.

  @retval TRUE/FALSE - ERID enable or disable
**/
BOOLEAN
GetCpgcPointTestCfgErid (
  IN CONST CPGC_POINT_TEST_CONFIG *pCpgcPointTestCfg
  );
#endif // __CPGC_POINT_TEST_H__

// end file CpgcPointTest.h
