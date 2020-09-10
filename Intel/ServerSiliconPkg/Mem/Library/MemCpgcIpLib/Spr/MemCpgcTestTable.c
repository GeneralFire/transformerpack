/** @file
  CPGC Test Table for Memory Training

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 - 2020 Intel Corporation. <BR>

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

//
// Include files
//
#include <Library/MemCpgcIpLib.h>
#include "Include/MemCpgcTestTable.h"
#include "MrcCommonTypes.h"
#include <Library/MemoryServicesLib.h>
#include "Include/CpgcChip.h"
#include <Library/SysHostPointerLib.h>
#include <Library/MemMcIpLib.h>

extern CPGC_TEST_ENV_PARAM CpgcTestEnvParam[];
extern UINT8 CpgcTestEnvParamSize;
#define DB_DFE_RELOAD_LFSR_SEED_RATE  9 //reload the seed very 256 cache lines

//                                   Rank, Bank, Row, Col
TCPGCAddress BasicCpgcAddress =     { { 0, 0, 0, 0 },     // Start
                                      { 0, 3, 0, 0 },     // Stop
                                      { 3, 0, 2, 1 },     // Order
                                      { 0, 0, 0, 0 },     // IncRate
                                      { 0, 4, 0, 0 } };   // IncValue

//                                   Rank, Bank, Row, Col
TCPGCAddress DqSwzDiscCpgcAddress = { { 0, 0, 0, 31},     // Start
                                      { 0, 0, 0, 31},     // Stop
                                      { 0, 0, 0, 0 },     // Order
                                      { 0, 0, 0, 0 },     // IncRate
                                      { 0, 0, 0, 0 } };   // IncValue

//                                   Rank, Bank, Row, Col
TCPGCAddress DfeCpgcAddress =       { { 0, 0, 0, 0 },     // Start
                                      { 0, 3, 0, 31},     // Stop
                                      { 3, 0, 2, 1 },     // Order
                                      { 0, 0, 0, 0 },     // IncRate
                                      { 0, 0, 0, 0 } };   // IncValue

//                                   Rank, Bank, Row, Col
TCPGCAddress WlCpgcAddress =        { { 0, 0, 0, 0 },     // Start
                                      { 0, 0, 0, 0 },     // Stop
                                      { 3, 0, 1, 2 },     // Order
                                      { 0, 0, 0, 0 },     // IncRate
                                      { 0, 0, 0, 0 } };   // IncValue

//                                   Rank, Bank, Row, Col
TCPGCAddress WrDqCpgcAddress =      { { 0, 0, 0, 0 },     // Start
                                      { 0, 3, 0, 31},     // Stop
                                      { 3, 0, 2, 1 },     // Order
                                      { 0, 0, 0, 0 },     // IncRate
                                      { 0, 4, 0, 0 } };   // IncValue

//                                   Rank, Bank, Row, Col
TCPGCAddress AdvDqCpgcAddress =      { { 0, 0, 0, 0 },     // Start
                                      { 0, 3, 0, 31},     // Stop
                                      { 3, 0, 2, 1 },     // Order
                                      { 0, 0, 0, 0 },     // IncRate
                                      { 0, 1, 0, 0 } };   // IncValue

//                                    Rank, Bank, Row, Col
TCPGCAddress DbDfeCpgcAddress =      {{ 0, 0, 0, 0 },      // Start
                                      { 0, 15, 0, 63},     // Stop
                                      { 0, 1, 3, 2 },      // Order
                                      { 0, 0, 0, 0 },      // IncRate
                                      { 0, 4, 0, 0 }};     // IncValue

//                                   Rank, Bank, Row, Col
TCPGCAddress AdvCmdCpgcAddress =     {{ 0, 0, 0,      0},     // Start
                                      { 0, 3, 0xFFFF, 7},     // Stop
                                      { 3, 0, 2,      1},     // Order
                                      { 0, 0, 0,      0},     // IncRate
                                      { 0, 2, 0,      0} };   // IncValue

//
// Turn around, same rank, same bank group.
//
//                                   Rank, Bank, Row, Col
//
TCPGCAddress TaSgCpgcAddress   =     {{ 0, 0, 0,      0},     // Start
                                      { 0, 0, 0,      0},     // Stop
                                      { 1, 0, 3,      2},     // Order
                                      { 0, 0, 0,      0},     // IncRate
                                      { 0, 0, 0,      0} };   // IncValue

//
// Turn around, same rank, different bank group.
//
//                                   Rank, Bank, Row, Col
//
TCPGCAddress TaSrCpgcAddress   =     {{ 0, 0, 0,      0},     // Start
                                      { 0, 3, 0,      0},     // Stop
                                      { 1, 0, 3,      2},     // Order
                                      { 0, 0, 0,      0},     // IncRate
                                      { 0, 1, 0,      0} };   // IncValue

//
// Turn around, same DIMM, different rank, Wr-Wr, Rd-Rd, Wr-Rd.
//
//                                   Rank, Bank, Row, Col
//
TCPGCAddress TaDrDdWwRrWrCpgcAddress= {{ 0,  0, 0,      0},     // Start
                                      {  7,  3, 0,      0},     // Stop
                                      {  1,  0, 3,      2},     // Order
                                      {  0,  0, 0,      0},     // IncRate
                                      {  1,  1, 0,      0} };   // IncValue

//
// Turn around, diffent DIMM, Rd-Wr.
//
//                                   Rank, Bank, Row, Col
//
TCPGCAddress TaDrDdRdWrCpgcAddress = {{  0,  0, 0,      0},     // Start
                                      {  1,  3, 0,      0},     // Stop
                                      {  1,  0, 3,      2},     // Order
                                      {  0,  0, 0,      0},     // IncRate
                                      {  1,  1, 0,      0} };   // IncValue

CPGC_TEST_PARAM  CpgcTestParam[] = {
// TestType,                       CmdPat,            CpgcAddress,              Mux[0],           Mux[1],           Mux[2],           Poly[0],        Poly[1],        Poly[2],        Patbuf[0],                 Patbuf[1],                 Patbuf[2],  WDBPat:IncRate,Start,Stop,DQPat,DQRotate, IsGenPrbsAddr
  {RcvenFineTest,                  PatRd,             &BasicCpgcAddress,        BTBUFFER,         BTBUFFER,         BTBUFFER,         CPGC_DONT_CARE, CPGC_DONT_CARE, CPGC_DONT_CARE, RCVEN_PATTERN_DDR5_0,      RCVEN_PATTERN_DDR5_1,      RCVEN_PATTERN_DDR5_2,   {32, 0, 9, DqAllSamePat, 0}, PRBS_ADDR_DIS, {0,0}, RequestDataSize32B},
  {MdqsReceiveEnablePhaseTraining, PatRd,             &BasicCpgcAddress,        BTBUFFER,         BTBUFFER,         BTBUFFER,         CPGC_DONT_CARE, CPGC_DONT_CARE, CPGC_DONT_CARE, RCVEN_PATTERN_DDR5_0,      RCVEN_PATTERN_DDR5_1,      RCVEN_PATTERN_DDR5_2,   {32, 0, 9, DqAllSamePat, 0}, PRBS_ADDR_DIS, {0,0}, RequestDataSize32B},
  {MdqsCoarseReadDelayTraining,    PatRd,             &BasicCpgcAddress,        BTBUFFER,         BTBUFFER,         BTBUFFER,         CPGC_DONT_CARE, CPGC_DONT_CARE, CPGC_DONT_CARE, RCVEN_PATTERN_DDR5_0,      RCVEN_PATTERN_DDR5_1,      RCVEN_PATTERN_DDR5_2,   {32, 0, 9, DqAllSamePat, 0}, PRBS_ADDR_DIS, {0,0}, RequestDataSize32B},
  {BacksideDqSwizzleDiscovery,     PatRd,             &BasicCpgcAddress,        BTBUFFER,         BTBUFFER,         BTBUFFER,         CPGC_DONT_CARE, CPGC_DONT_CARE, CPGC_DONT_CARE, RCVEN_PATTERN_DDR5_0,      RCVEN_PATTERN_DDR5_1,      RCVEN_PATTERN_DDR5_2,   {32, 0, 9, DqAllSamePat, 0}, PRBS_ADDR_DIS, {0,0}, RequestDataSize32B},
  {FrontsideDqSwizzleDiscovery,    PatRd,             &BasicCpgcAddress,        BTBUFFER,         BTBUFFER,         BTBUFFER,         CPGC_DONT_CARE, CPGC_DONT_CARE, CPGC_DONT_CARE, RCVEN_PATTERN_DDR5_0,      RCVEN_PATTERN_DDR5_1,      RCVEN_PATTERN_DDR5_2,   {32, 0, 9, DqAllSamePat, 0}, PRBS_ADDR_DIS, {0,0}, RequestDataSize32B},
  {MdqsReadDelayDeskew,            PatRd,             &BasicCpgcAddress,        BTBUFFER,         BTBUFFER,         BTBUFFER,         CPGC_DONT_CARE, CPGC_DONT_CARE, CPGC_DONT_CARE, RCVEN_PATTERN_DDR5_0,      RCVEN_PATTERN_DDR5_1,      RCVEN_PATTERN_DDR5_2,   {32, 0, 9, DqAllSamePat, 0}, PRBS_ADDR_DIS, {0,0}, RequestDataSize32B},
  {MdqsWriteDelay,                 PatBack2BackWrRd,  &WrDqCpgcAddress,         BTBUFFER,         BTBUFFER,         BTBUFFER,         CPGC_DONT_CARE, CPGC_DONT_CARE, CPGC_DONT_CARE, DPAT_UNISEQ_ALL_1S,        DPAT_UNISEQ_ALL_1S,        DPAT_UNISEQ_ALL_1S,     { 0, 0, 0, DqAllSamePat, 4}, PRBS_ADDR_DIS, {0,1}, RequestDataSize64B},
  {DqSwzDiscTestDdr5,              PatRd,             &DqSwzDiscCpgcAddress,    BTBUFFER,         BTBUFFER,         BTBUFFER,         CPGC_DONT_CARE, CPGC_DONT_CARE, CPGC_DONT_CARE, DPAT_UNISEQ_ALL_1S,        0,                         0,                      {32, 0, 0, DqAllSamePat, 4}, PRBS_ADDR_DIS, {0,1}, RequestDataSize64B},
  {RdDqTest,                       PatRd,             &BasicCpgcAddress,        GALOIS_LFSR_MODE, GALOIS_LFSR_MODE, GALOIS_LFSR_MODE, GALOIS_POLY_16, GALOIS_POLY_16, GALOIS_POLY_16, RD_DQ_DQS_PATTERN_DDR5,    RD_DQ_DQS_PATTERN_DDR5,    RD_DQ_DQS_PATTERN_DDR5, { 0, 0, 0, DqAllSamePat, 0}, PRBS_ADDR_DIS, {0,1}, RequestDataSize64B},
  {ReadPreDfe2DCenteringTest,      PatRd,             &BasicCpgcAddress,        GALOIS_LFSR_MODE, GALOIS_LFSR_MODE, GALOIS_LFSR_MODE, GALOIS_POLY_16, GALOIS_POLY_16, GALOIS_POLY_16, RD_DQ_DQS_PREDFE_DDR5,     RD_DQ_DQS_PREDFE_DDR5,     RD_DQ_DQS_PREDFE_DDR5,  { 0, 0, 0, DqAllSamePat, 0}, PRBS_ADDR_DIS, {0,1}, RequestDataSize64B},
  {DramDutyCycleAdjusterTest,      PatRd,             &BasicCpgcAddress,        GALOIS_LFSR_MODE, GALOIS_LFSR_MODE, GALOIS_LFSR_MODE, GALOIS_POLY_16, GALOIS_POLY_16, GALOIS_POLY_16, DRAM_DCA_PATTERN_DDR5,     DRAM_DCA_PATTERN_DDR5,     DRAM_DCA_PATTERN_DDR5,  { 0, 0, 0, DqAllSamePat, 0}, PRBS_ADDR_DIS, {0,1}, RequestDataSize64B},
  {ReadDfeTest,                    PatRd,             &BasicCpgcAddress,        GALOIS_LFSR_MODE, GALOIS_LFSR_MODE, BTBUFFER,         GALOIS_POLY_8,  GALOIS_POLY_8,  CPGC_DONT_CARE, MR26_LFSR_0_DEFAULT_VALUE, MR27_LFSR_1_DEFAULT_VALUE, 0,                      { 0, 0, 0, DqAAAVAAAV,   0}, PRBS_ADDR_DIS, {0,1}, RequestDataSize64B},
  {ReadPostDfe2DCenteringTest,     PatRd,             &DfeCpgcAddress,          GALOIS_LFSR_MODE, GALOIS_LFSR_MODE, GALOIS_LFSR_MODE, GALOIS_POLY_16, GALOIS_POLY_16, GALOIS_POLY_16, RD_DQ_DQS_PATTERN_DDR5,    RD_DQ_DQS_PATTERN_DDR5,    RD_DQ_DQS_PATTERN_DDR5, { 0, 0, 0, DqAAAAAAAV,   0}, PRBS_ADDR_DIS, {0,1}, RequestDataSize64B},
  {WLDdr5,                         PatWr,             &WlCpgcAddress,           BTBUFFER,         BTBUFFER,         BTBUFFER,         CPGC_DONT_CARE, CPGC_DONT_CARE, CPGC_DONT_CARE, RD_DQ_DQS_PATTERN_DDR5,    CPGC_DONT_CARE,            CPGC_DONT_CARE,         { 0, 0, 3, DqAllSamePat, 4}, PRBS_ADDR_DIS, {0,1}, RequestDataSize64B},
  {WrDqTest,                       PatWrRd,           &WrDqCpgcAddress,         GALOIS_LFSR_MODE, GALOIS_LFSR_MODE, GALOIS_LFSR_MODE, GALOIS_POLY_16, GALOIS_POLY_16, GALOIS_POLY_16, RD_DQ_DQS_PATTERN_DDR5,    RD_DQ_DQS_PATTERN_DDR5,    RD_DQ_DQS_PATTERN_DDR5, { 0, 0, 0, DqAllSamePat, 0}, PRBS_ADDR_DIS, {0,1}, RequestDataSize64B},
  {WritePreDfe2DCenteringTest,     PatBack2BackWrRd,  &AdvDqCpgcAddress,        GALOIS_LFSR_MODE, GALOIS_LFSR_MODE, BTBUFFER,         GALOIS_POLY_8, GALOIS_POLY_8,   CPGC_DONT_CARE, CPGC_GALOIS_LFSR_SEED_0,   CPGC_GALOIS_LFSR_SEED_1,   0,                      { 0, 0, 0, DqAAAAAAAV,   4}, PRBS_ADDR_DIS, {0,1}, RequestDataSize64B},
  {WriteDfeTest,                   PatBack2BackWrRd,  &AdvDqCpgcAddress,        GALOIS_LFSR_MODE, GALOIS_LFSR_MODE, BTBUFFER,         GALOIS_POLY_8,  GALOIS_POLY_8,  CPGC_DONT_CARE, CPGC_GALOIS_LFSR_SEED_0,   CPGC_GALOIS_LFSR_SEED_1,   0,                      { 0, 0, 0, DqAAAAAAAV,   4}, PRBS_ADDR_DIS, {0,1}, RequestDataSize64B},
  {WriteDbDfeTest,                 PatBack2BackWrWrRdRd,  &DbDfeCpgcAddress,        GALOIS_LFSR_MODE, GALOIS_LFSR_MODE, GALOIS_LFSR_MODE,         GALOIS_POLY_PRBS_15,  GALOIS_POLY_PRBS_15,  GALOIS_POLY_PRBS_15, CPGC_GALOIS_LFSR_SEED_0,   CPGC_GALOIS_LFSR_SEED_1,   0,                      { 0, 0, 0, DqAAAAAAAV,   4}, PRBS_ADDR_DIS, {0,1}, RequestDataSize64B},
  {WritePostDfe2DCenteringTest,    PatBack2BackWrRd,  &AdvDqCpgcAddress,        GALOIS_LFSR_MODE, GALOIS_LFSR_MODE, BTBUFFER,         GALOIS_POLY_8,  GALOIS_POLY_8,  CPGC_DONT_CARE, CPGC_GALOIS_LFSR_SEED_0,   CPGC_GALOIS_LFSR_SEED_1,   0,                      { 0, 0, 0, DqAAAAAAAV,   4}, PRBS_ADDR_DIS, {0,1}, RequestDataSize64B},
  {RmtDqTest,                      PatBack2BackWrRd,  &AdvDqCpgcAddress,        GALOIS_LFSR_MODE, GALOIS_LFSR_MODE, BTBUFFER,         GALOIS_POLY_8,  GALOIS_POLY_8,  CPGC_DONT_CARE, CPGC_GALOIS_LFSR_SEED_0,   CPGC_GALOIS_LFSR_SEED_1,   0,                      { 0, 0, 0, DqAAAAAAAV,   4}, PRBS_ADDR_DIS, {0,1}, RequestDataSize64B},
  {RmtCmdTest,                     PatBack2BackWrRd,  &AdvCmdCpgcAddress,       GALOIS_LFSR_MODE, GALOIS_LFSR_MODE, BTBUFFER,         GALOIS_POLY_8,  GALOIS_POLY_8,  CPGC_DONT_CARE, CPGC_GALOIS_LFSR_SEED_0,   CPGC_GALOIS_LFSR_SEED_1,   0,                      { 0, 0, 0, DqAAAAAAAV,   4}, PRBS_ADDR_EN,  {0,1}, RequestDataSize64B},
  {TurnAroundSgTest,               PatTurnaroundSgSr, &TaSgCpgcAddress,         GALOIS_LFSR_MODE, GALOIS_LFSR_MODE, BTBUFFER,         GALOIS_POLY_8,  GALOIS_POLY_8,  CPGC_DONT_CARE, CPGC_GALOIS_LFSR_SEED_0,   CPGC_GALOIS_LFSR_SEED_1,   0,                      { 0, 0, 0, DqAAAAAAAV,   4}, PRBS_ADDR_DIS, {0,1}, RequestDataSize64B},
  {TurnAroundSrTest,               PatTurnaroundSgSr, &TaSrCpgcAddress,         GALOIS_LFSR_MODE, GALOIS_LFSR_MODE, BTBUFFER,         GALOIS_POLY_8,  GALOIS_POLY_8,  CPGC_DONT_CARE, CPGC_GALOIS_LFSR_SEED_0,   CPGC_GALOIS_LFSR_SEED_1,   0,                      { 0, 0, 0, DqAAAAAAAV,   4}, PRBS_ADDR_DIS, {0,1}, RequestDataSize64B},
  {TurnAroundDrDdWwRrWrTest,       PatTurnaroundDrDd, &TaDrDdWwRrWrCpgcAddress, GALOIS_LFSR_MODE, GALOIS_LFSR_MODE, BTBUFFER,         GALOIS_POLY_8,  GALOIS_POLY_8,  CPGC_DONT_CARE, CPGC_GALOIS_LFSR_SEED_0,   CPGC_GALOIS_LFSR_SEED_1,   0,                      { 0, 0, 0, DqAAAAAAAV,   4}, PRBS_ADDR_DIS, {0,1}, RequestDataSize64B},
  {TurnAroundDrDdRwTest,           PatTurnaroundDrDd, &TaDrDdRdWrCpgcAddress,   GALOIS_LFSR_MODE, GALOIS_LFSR_MODE, BTBUFFER,         GALOIS_POLY_8,  GALOIS_POLY_8,  CPGC_DONT_CARE, CPGC_GALOIS_LFSR_SEED_0,   CPGC_GALOIS_LFSR_SEED_1,   0,                      { 0, 0, 0, DqAAAAAAAV,   4}, PRBS_ADDR_DIS, {0,1}, RequestDataSize64B},
  {DqSlewRateTest,                 PatWrRd,           &WrDqCpgcAddress,         BTBUFFER,         BTBUFFER,         BTBUFFER,         CPGC_DONT_CARE, CPGC_DONT_CARE, CPGC_DONT_CARE, WR_DQ_DQS_PATTERN_DDR5,    WR_DQ_DQS_PATTERN_DDR5,    WR_DQ_DQS_PATTERN_DDR5, { 0, 0, 0, DqAllSamePat, 0}, PRBS_ADDR_DIS, {0,1}, RequestDataSize64B},
};

UINT8 CpgcTestParamSize = sizeof(CpgcTestParam) / sizeof(CPGC_TEST_PARAM);

/**
  Provide the caller with pointers to cpgc test parameter table

  @param[in]   TestType                          - Specify memory training steps
  @param[out]  CpgcTestTable                     - Pointer to pointer to the cpgc test parameter table

  @retval MRC_STATUS_SUCCESS                     - Find the table
          MRC_STATUS_FAILURE                     - Not find the table

**/
MRC_STATUS
GetCpgcTestParamPtrs (
  IN  MRC_TT             TestType,
  OUT CPGC_TEST_PARAM    **CpgcTestTable
  )
{
  UINT32 Index;

  for (Index = 0; Index < CpgcTestParamSize; Index++) {
    if (CpgcTestParam[Index].TestType == TestType) {
      *CpgcTestTable = &CpgcTestParam[Index];
      return MRC_STATUS_SUCCESS;
    }
  }

  return MRC_STATUS_FAILURE;
}

/**
  Provide the caller with pointers to cpgc test enrivonment table

  @param[in]   TestType                          - Specify memory training steps
  @param[out]  CpgcTestEnvTable                  - Pointer to pointer to the cpgc test environment table

  @retval MRC_STATUS_SUCCESS                     - Find the table
          MRC_STATUS_FAILURE                     - Not find the table

**/
MRC_STATUS
GetCpgcTestEnvPtrs (
  IN  MRC_TT               TestType,
  OUT CPGC_TEST_ENV_PARAM  **CpgcTestEnvTable
  )
{
  UINT32 Index;

  for (Index = 0; Index < CpgcTestEnvParamSize; Index++) {
    if (CpgcTestEnvParam[Index].TestType == TestType) {
      *CpgcTestEnvTable = &CpgcTestEnvParam[Index];
      return MRC_STATUS_SUCCESS;
    }
  }

  return MRC_STATUS_FAILURE;
}

/**

  Get Test Burst wait.

  @param[in]   TestType      - Specify memory training steps
  @param[out]  SubSeqWait    - Test burst wait
                             - Unit: native CPGC clock (1/4 the DDR frequency)

  @retval  EFI_SUCCESS       - This function completed execution with no warnings/errors
           EFI_NOT_FOUND     - Test is not supported

**/
EFI_STATUS
GetTestBurstWait (
  IN  MRC_TT  TestType,
  OUT UINT16  *SubSeqWait
  )
{
  CPGC_TEST_ENV_PARAM  *CpgcTestEnvPtr;

  if (GetCpgcTestEnvPtrs (TestType, &CpgcTestEnvPtr) == MRC_STATUS_FAILURE) {
    return EFI_NOT_FOUND;
  }

  *SubSeqWait = CpgcTestEnvPtr->SubSeqWait;

  return EFI_SUCCESS;
}

/**

  Get Test Loop Count.

  @param[in]   TestType      - Specify memory training steps
  @param[out]  LoopCount     - Test loop count.

  @retval  EFI_SUCCESS       - This function completed execution with no warnings/errors
           EFI_NOT_FOUND     - Test is not supported

**/
EFI_STATUS
GetTestLoopCount (
  IN  MRC_TT  TestType,
  OUT UINT32  *LoopCount
  )
{
  CPGC_TEST_ENV_PARAM  *CpgcTestEnvPtr;

  if (GetCpgcTestEnvPtrs (TestType, &CpgcTestEnvPtr) == MRC_STATUS_FAILURE) {
    return EFI_NOT_FOUND;
  }

  *LoopCount = CpgcTestEnvPtr->LoopCount;

  return EFI_SUCCESS;
}

/**

  Get Write Leveling Fly by Loop Count.

  @param[out]  LoopCount     - Write leveling fly by loop count.

  @retval  EFI_SUCCESS       - This function completed execution with no warnings/errors
           EFI_NOT_FOUND     - Write leveling Fly by loop count is not found

**/
EFI_STATUS
EFIAPI
GetWlFlyByLoopCount (
  OUT  UINT32  *LoopCount
  )
{
  return GetTestLoopCount (WLDdr5, LoopCount);
}

/**

  Get Receive Enable Loop Count.

  @param[out]  LoopCount     - Receive enable loop count.

  @retval  EFI_SUCCESS       - This function completed execution with no warnings/errors
           EFI_NOT_FOUND     - Receive enable loop count is not found

**/
EFI_STATUS
GetRcvEnLoopCount (
  OUT  UINT32  *LoopCount
  )
{
  return GetTestLoopCount (RcvenFineTest, LoopCount);
}

/**

  Get burst wait for reads

  @param[out]  SubSeqWait    - Burst wait for reads.
                             - Unit: native CPGC clock (1/4 the DDR frequency)

  @retval  EFI_SUCCESS       - This function completed execution with no warnings/errors
           EFI_NOT_FOUND     - Receive enable loop count is not found

**/
EFI_STATUS
GetRdDqBurstWait (
  OUT  UINT16  *SubSeqWait
  )
{
  return GetTestBurstWait (RdDqTest, SubSeqWait);
}

/**

  Get burst wait for writes, wirtes->reads, and reads->writes

  @param[out]  SubSeqWait    - Burst wait for writes, wirtes->reads, and reads->writes
                             - Unit: native CPGC clock (1/4 the DDR frequency)

  @retval  EFI_SUCCESS       - This function completed execution with no warnings/errors
           EFI_NOT_FOUND     - Receive enable loop count is not found

**/
EFI_STATUS
GetWrDqBurstWait (
  OUT  UINT16  *SubSeqWait
  )
{
  return GetTestBurstWait (WrDqTest, SubSeqWait);
}



/**

  Excute Cpgc Test
  It is used to setup training patterns for all channels defined by input parameter "ChannelMask"
  cpgc test worker procedual can be used for regular data path training and BSSA

  @param[in] Socket                - Socket to train
  @param[in] ChannelMask           - Ch Bit mask for which test should be setup for
  @param[in] LogicalRank           - Logical Rank number
  @param[in] SeqCnt                - Optional to override default loopcount for given test type
  @param[in] CPGC_TEST_PARAM       - Struct pointer to Cpgc Test Parameter
  @param[in] CPGC_TEST_ENV_PARAM   - Struct poniter to Cpgc Test Enviroment Parameter
  @param[in] BaseColRepeats        - Base Col Repeats (Alternate Base_Address_Loopcount)

  @retval    MRC_STATUS

**/
MRC_STATUS
CpgcTestWorker (
  IN UINT8                    Socket,
  IN UINT32                   ChannelMask,
  IN UINT8                    LogicalRank,
  IN UINT32                   SeqCnt,
  IN CPGC_TEST_PARAM          *CpgcTestParamPtr,
  IN CPGC_TEST_ENV_PARAM      *CpgcTestEnvPtr,
  IN UINT32                   BaseColRepeats
  )
{
  UINT32                   Status = SUCCESS;
  PSYSHOST                 Host;
  TCPGCAddress             *CpgcAddress;
  TWDBPattern              *WdbPattern;
  UINT8                    Dimm;

  Host        = GetSysHostPointer ();
  WdbPattern  = &CpgcTestParamPtr->WdbPattern;
  CpgcAddress = CpgcTestParamPtr->CpgcAddress;
  Dimm        = LogicalRank/MAX_RANK_DIMM;  //Get the dimm number the rank is on

  if (CpgcTestParamPtr->CmdPat == PatRd) {
    SetCadbModeEn (Host, Socket, ChannelMask, TRUE);
    SetupCadbMrr (Host, Socket, ChannelMask, LogicalRank);
  }

  if (SeqCnt != 0) {
    CpgcTestEnvPtr->LoopCount = SeqCnt;
  }

  SetupIOTest (Host, Socket, ChannelMask, CpgcTestParamPtr->CmdPat,
    CpgcTestEnvPtr->NumCacheLine, CpgcTestEnvPtr->LoopCount,
    CpgcAddress, NSOE, WdbPattern,
    0, 0, CpgcTestEnvPtr->SubSeqWait);
  if (BaseColRepeats != DEFAULT_BASE_COL_REPEATS) {
    //
    // Program the base col repeats
    //
    ProgramBaseColRepeats (Host, Socket, ChannelMask, BaseColRepeats);
    //
    // Program the reload seed rate
    //
    SetCpgcReloadSeedRate (Host, Socket, ChannelMask, DB_DFE_RELOAD_LFSR_SEED_RATE);
  }

  SetCpgcPatControl (Host, Socket, ChannelMask,
                     CpgcTestParamPtr->Mux[0],
                     CpgcTestParamPtr->Mux[1],
                     CpgcTestParamPtr->Mux[2],
                     CpgcTestParamPtr->Patbuf[0],
                     CpgcTestParamPtr->Patbuf[1],
                     CpgcTestParamPtr->Patbuf[2]);

  SetLfsrPolynomial (Host, Socket, ChannelMask,
                     CpgcTestParamPtr->Poly[0],
                     CpgcTestParamPtr->Poly[1],
                     CpgcTestParamPtr->Poly[2]);

  SetCpgcDqRotateRate (Host, Socket, ChannelMask, WdbPattern->DQRotate);

  SetWdbDqPattern (Host, Socket, ChannelMask, WdbPattern->DQPat);

  SetCpgcStartDelay (Host, Socket, ChannelMask, CpgcTestParamPtr->StartDelay);

  SetCpgcRequestDataSize (Host, Socket, ChannelMask, CpgcTestParamPtr->RequestDataSize);

  GenPrbsAddress (Host, Socket, ChannelMask, CpgcTestParamPtr->IsGenPrbsAddr);

  return Status;
}