/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include "MemTestFunc.h"
#include <Library/MemRcLib.h>

//
// Code section
//


//
// Hooks to test error handling functionality
//
#define INJECT_AMT_FAILURES_DIMM_L1             0
#define INJECT_AMT_FAILURES_DIMM_L2             0
#define INJECT_AMT_FAILURES_TEST_RESULTS        0
#define INJECT_AMT_FAILURES_TEST_RESULTS2       0
#define INJECT_AMT_FAILURES_TEST_RESULTS3       0
#define INJECT_AMT_FAILURES_TEST_RESULTS4       0
#define INJECT_AMT_FAILURES_TEST_RESULTS5       0
#define INJECT_AMT_FAILURES_TEST_RESULTS6       0
#define INJECT_AMT_FAILURES_TEST_RESULTS7       0
#define INJECT_AMT_FAILURES_TEST_RESULTS8       0
#define INJECT_AMT_FAILURES_TEST_RESULTS9       0
#define INJECT_AMT_FAILURES_ROW_LIST            0


/**

  Runs Advanced Memory Tests based on the legacy Scan algorithm with DDR scrambler enabled

  @param Host                         - Pointer to sysHost
  @param socket                       - Socket Id
  @param ddrChEnabled                 - List of channels to be tested
  @param testType                     - Memory test type
  @param logicalRank                  - Logical rank on the channel
  @param logicalSubRank               - Logical sub rank on the channel
  @param bank                         - Current bank address
  @param CurrentRow[MAX_CH]           - Row address to start write
  @param TestSize[MAX_CH]             - Row range size to write
  @param ErrInjMask[3]                - Bitmask of DQ lanes to inject error
  @param patternQW[MT_PATTERN_DEPTH]  - 64-bit wide data pattern to use per UI
  @param IsUseInvtPat                 - Info to indicate whether or not patternQW is inverted by comparing original pattern
  @param uiShl[MT_PATTERN_DEPTH]      - Enables pattern rotation per UI
  @param numCl                        - Number of cachelines to use in WDB
  @param direction                    - Sequential address direction MT_ADDR_DIR_UP, MT_ADDR_DIR_DN

  @retval None

**/
VOID
InjectMemtestError (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    ddrChEnabled,
  UINT8     testType,
  UINT8     logicalRank,
  UINT8     logicalSubRank,
  UINT8     bank,
  UINT32    CurrentRow[MAX_CH],
  UINT32    TestSize[MAX_CH],
  UINT32    ErrInjMask[3],
  UINT64_STRUCT patternQW[MT_PATTERN_DEPTH],
  BOOLEAN   IsUseInvtPat,
  UINT8     uiShl[MT_PATTERN_DEPTH],
  UINT8     numCl,
  UINT8     direction
)
{
  UINT8   columnBits[MAX_RANK_CH][MAX_CH];
  UINT8   rowBits[MAX_RANK_CH][MAX_CH];
  UINT8   bankBits[MAX_RANK_CH][MAX_CH];
  UINT8   rankEnabled[MAX_RANK_CH][MAX_CH];
  UINT8   dimmRank[MAX_RANK_CH][MAX_CH];
  UINT8   subRankCnt[MAX_RANK_CH][MAX_CH];
  UINT8   logical2Physical[MAX_RANK_CH][MAX_CH];
  UINT8   maxEnabledRank;
  UINT8   maxEnabledSubRank;
  UINT32  rankChEnMap;
  UINT32  rankChEnOut;
  UINT8   TestStatus[MAX_CH];
  UINT8   seqDataInv[MT_MAX_SUBSEQ];
  UINT8   baseBits;
  UINT8   ch;
  UINT8   mode;
  UINT8   patternDepth;
  UINT32 DeviceMask[MAX_CH][3];
  UINT64_STRUCT AdvMemtestErrInjMask;
  UINT8  MaxChDdr;

  ZeroMem (columnBits, sizeof (columnBits));
  ZeroMem (rowBits, sizeof (rowBits));
  ZeroMem (bankBits, sizeof (bankBits));
  ZeroMem (rankEnabled, sizeof (rankEnabled));
  ZeroMem (dimmRank, sizeof (dimmRank));
  ZeroMem (subRankCnt, sizeof (subRankCnt));
  ZeroMem (logical2Physical, sizeof (logical2Physical));

  MaxChDdr = GetMaxChDdr ();
  RcDebugPrint (SDBG_MINMAX,
    "Adv Memtest Error Inject starts\n");

  //
  // Don't mask any devices
  //
  ZeroMem (DeviceMask, sizeof (DeviceMask));

  seqDataInv[0] = 0;
  seqDataInv[1] = 0;
  baseBits = 3;
  mode = MT_CPGC_WRITE;
  patternDepth = 1;

  //
  // This is done globally to avoid changing multiple API layers under CpgcMemTestGlobalSetupMATS
  //
  AdvMemtestErrInjMask.hi = ErrInjMask[1];
  AdvMemtestErrInjMask.lo = ErrInjMask[0];
  SetAdvMemtestErrInjMask (Host, AdvMemtestErrInjMask);

  MemRankGeometry (Host, socket, columnBits, rowBits, bankBits, rankEnabled, dimmRank, subRankCnt, &maxEnabledRank, &maxEnabledSubRank, logical2Physical);
  rankChEnMap = ddrChEnabled;
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((ddrChEnabled & (1 << ch)) == 0) {
      continue;
    }
    rankChEnMap &= ~((rankEnabled[logicalRank][ch] == 0) << ch);
    CpgcMemTestGlobalSetupMATS (Host, socket, ch, 0, 1, testType, mode, seqDataInv, patternQW, IsUseInvtPat, uiShl, numCl, patternDepth, direction);
    SetInjectErrorMATS (Host, ch, 1);

    //
    // Program WDB data with given pattern
    //
    WriteWDBFixedPatternMATS (Host, socket, ch, mode, seqDataInv, patternQW, IsUseInvtPat, uiShl, numCl, patternDepth, bank >> 3, direction, GetAdvMemtestErrInjMask (Host));  //TODO: DDR5 support for different # bank bits
  }

  CpgcMemTestRowRange (Host, socket, rankChEnMap, &rankChEnOut, dimmRank, logical2Physical, logicalRank, logicalSubRank,
    columnBits, rowBits, bankBits, testType, mode, FAST_Y, direction, bank, baseBits,
    CurrentRow, TestSize, DeviceMask, TestStatus, seqDataInv);

  AdvMemtestErrInjMask.hi = 0;
  AdvMemtestErrInjMask.lo = 0;
  SetAdvMemtestErrInjMask (Host, AdvMemtestErrInjMask);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((ddrChEnabled & (1 << ch)) == 0) {
      continue;
    }
    SetInjectErrorMATS (Host, ch, 0);
  }

  RcDebugPrint (SDBG_MINMAX,
    "Adv Memtest Error Inject ends\n");
}

/**

  Injects Advanced Memtest error before MATSN step 2

  @param[in] Host                - Pointer to sysHost
  @param[in] socket              - Socket Id
  @param[in] ddrChEnabled        - List of channels to be tested
  @param[in] testType            - Memory test type
  @param[in] patternQW           - 64-bit wide data pattern to use per UI
  @param[in] IsUseInvtPat        - Info to indicate whether or not patternQW is inverted by comparing original pattern
  @param[in] uiShl               - Enables pattern rotation per UI
  @param[in] numCl               - Number of cachelines to use in WDB
  @param[in] TestBit             - Dq lane to be tested

  @retval SUCCESS/FAILURE

**/
UINT32
InjectErrorBeforeMatsnStep2 (
  IN PSYSHOST         Host,
  IN UINT8            socket,
  IN UINT32           ddrChEnabled,
  IN UINT8            testType,
  IN UINT64_STRUCT    patternQW[MT_PATTERN_DEPTH],
  IN BOOLEAN          IsUseInvtPat,
  IN UINT8            uiShl[MT_PATTERN_DEPTH],
  IN UINT8            numCl,
  IN UINT32           TestBit
)
{
#if INJECT_AMT_FAILURES_DIMM_L1
  UINT8     logicalRank;
  UINT8     logicalSubRank;
  UINT8     bank;
  UINT32    ErrInjMask[3];
  UINT32    CurrentRow[MAX_CH];
  UINT32    InjTestSize[MAX_CH];
  UINT8     Ch;
  UINT8     ddrChEnabledTemp;
  UINT8     MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  ErrInjMask[0] = 0xf00000;
  ErrInjMask[1] = 0;
  ErrInjMask[2] = 0;

  logicalSubRank = 0;
  logicalRank = 0;
  bank = 3;

  SetMem ((UINT8 *)CurrentRow, sizeof (CurrentRow), 0);
  SetMem ((UINT8 *)InjTestSize, sizeof (InjTestSize), 0);
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (ddrChEnabled & (1 << Ch)) {
      break;
    }
  }
  ddrChEnabledTemp = 1 << Ch;

  //
  // Just inject one error so we don't use up all PPR resources
  //
  CurrentRow[Ch] = 6;
  InjTestSize[Ch] = 1;
  if (Host->var.mem.AmtErrInjDone == 0) {
    InjectMemtestError (Host, socket, ddrChEnabledTemp, testType, logicalRank, logicalSubRank, bank, CurrentRow, InjTestSize, ErrInjMask, patternQW, IsUseInvtPat, uiShl, numCl, MT_ADDR_DIR_UP);
    Host->var.mem.AmtErrInjDone = 1;
  }

#if INJECT_AMT_FAILURES_DIMM_L2
  //
  // Inject more errors to test row fail list logic
  //
  bank = TestBit % 8;
  CurrentRow[Ch] = 11;
  InjTestSize[Ch] = 1;
  InjectMemtestError (Host, socket, ddrChEnabledTemp, testType, logicalRank, logicalSubRank, bank + 8, CurrentRow, InjTestSize, ErrInjMask, patternQW, IsUseInvtPat, uiShl, numCl, MT_ADDR_DIR_UP);

  CurrentRow[Ch] = 12;
  InjTestSize[Ch] = 1;
  InjectMemtestError (Host, socket, ddrChEnabledTemp, testType, logicalRank, logicalSubRank, bank, CurrentRow, InjTestSize, ErrInjMask, patternQW, IsUseInvtPat, uiShl, numCl, MT_ADDR_DIR_UP);

  CurrentRow[Ch] = 15;
  InjTestSize[Ch] = 1;
  InjectMemtestError (Host, socket, ddrChEnabledTemp, testType, logicalRank, logicalSubRank, bank, CurrentRow, InjTestSize, ErrInjMask, patternQW, IsUseInvtPat, uiShl, numCl, MT_ADDR_DIR_UP);

  CurrentRow[Ch] = 25;
  InjTestSize[Ch] = 1;
  InjectMemtestError (Host, socket, ddrChEnabledTemp, testType, logicalRank, logicalSubRank, bank, CurrentRow, InjTestSize, ErrInjMask, patternQW, IsUseInvtPat, uiShl, numCl, MT_ADDR_DIR_UP);

#endif  //INJECT_AMT_FAILURES_DIMM_L2
#endif  //INJECT_AMT_FAILURES_DIMM_L1

  return SUCCESS;
} // InjectErrorBeforeMatsnStep2

/**

  Injects Advanced Memtest error before MATSN step 3

  @param[in] Host                - Pointer to sysHost
  @param[in] socket              - Socket Id
  @param[in] ddrChEnabled        - List of channels to be tested
  @param[in] testType            - Memory test type
  @param[in] patternQW           - 64-bit wide data pattern to use per UI
  @param[in] IsUseInvtPat        - Info to indicate whether or not patternQW is inverted by comparing original pattern
  @param[in] uiShl               - Enables pattern rotation per UI
  @param[in] numCl               - Number of cachelines to use in WDB
  @param[in] TestBit             - Dq lane to be tested

  @retval SUCCESS/FAILURE

**/
UINT32
InjectErrorBeforeMatsnStep3 (
  IN PSYSHOST         Host,
  IN UINT8            socket,
  IN UINT32           ddrChEnabled,
  IN UINT8            testType,
  IN UINT64_STRUCT    patternQW[MT_PATTERN_DEPTH],
  IN BOOLEAN          IsUseInvtPat,
  IN UINT8            uiShl[MT_PATTERN_DEPTH],
  IN UINT8            numCl,
  IN UINT32           TestBit
)
{
#if INJECT_AMT_FAILURES_DIMM_L2
  IN UINT8     logicalRank;
  IN UINT8     logicalSubRank;
  IN UINT8     bank;
  IN UINT32    ErrInjMask[3];
  IN UINT32    CurrentRow[MAX_CH];
  IN UINT32    InjTestSize[MAX_CH];
  IN UINT8     Ch;
  IN UINT8     ddrChEnabledTemp;
  IN UINT8     MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  ErrInjMask[0] = 0xf00;
  ErrInjMask[1] = 0;
  ErrInjMask[2] = 0;

  logicalSubRank = 0;
  logicalRank = 0;
  bank = TestBit % 8;

  SetMem ((UINT8 *)CurrentRow, sizeof (CurrentRow), 0);
  SetMem ((UINT8 *)InjTestSize, sizeof (InjTestSize), 0);
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (ddrChEnabled & (1 << Ch)) {
      break;
    }
  }
  ddrChEnabledTemp = 1 << Ch;

  CurrentRow[Ch] = 16;
  InjTestSize[Ch] = 1;
  InjectMemtestError (Host, socket, ddrChEnabledTemp, testType, logicalRank, logicalSubRank, bank + 8, CurrentRow, InjTestSize, ErrInjMask, patternQW, IsUseInvtPat, uiShl, numCl, MT_ADDR_DIR_DN);

  CurrentRow[Ch] = 24;
  InjTestSize[Ch] = 1;
  InjectMemtestError (Host, socket, ddrChEnabledTemp, testType, logicalRank, logicalSubRank, bank, CurrentRow, InjTestSize, ErrInjMask, patternQW, IsUseInvtPat, uiShl, numCl, MT_ADDR_DIR_DN);
#endif  //INJECT_AMT_FAILURES_DIMM_L2

  return SUCCESS;
} // InjectErrorBeforeMatsnStep3

/**

  Injects Advanced Memtest error before Scram step 2

  @param[in] Host                - Pointer to sysHost
  @param[in] socket              - Socket Id
  @param[in] ddrChEnabled        - List of channels to be tested
  @param[in] testType            - Memory test type
  @param[in] patternQW           - 64-bit wide data pattern to use per UI
  @param[in] IsUseInvtPat        - Info to indicate whether or not patternQW is inverted by comparing original pattern
  @param[in] uiShl               - Enables pattern rotation per UI
  @param[in] numCl               - Number of cachelines to use in WDB

  @retval SUCCESS/FAILURE

**/
UINT32
InjectErrorBeforeScramStep2 (
  IN PSYSHOST         Host,
  IN UINT8            socket,
  IN UINT32           ddrChEnabled,
  IN UINT8            testType,
  IN UINT64_STRUCT    patternQW[MT_PATTERN_DEPTH],
  IN BOOLEAN          IsUseInvtPat,
  IN UINT8            uiShl[MT_PATTERN_DEPTH],
  IN UINT8            numCl
)
{
#if INJECT_AMT_FAILURES_DIMM_L2
  UINT8     logicalRank;
  UINT8     logicalSubRank;
  UINT8     bank;
  UINT32    ErrInjMask[3];
  UINT32    CurrentRow[MAX_CH];
  UINT32    InjTestSize[MAX_CH];

  ErrInjMask[0] = 0xf00;
  ErrInjMask[1] = 0;
  ErrInjMask[2] = 0;

  // Assume ch 0 present
  SetMem ((UINT8 *)CurrentRow, sizeof (CurrentRow), 0);
  SetMem ((UINT8 *)InjTestSize, sizeof (InjTestSize), 0);

  CurrentRow[0] = 17;
  InjTestSize[0] = 1;
  logicalSubRank = 0;
  logicalRank = 0;
  bank = 0;

  //
  // Inject error
  //
  InjectMemtestError (Host, socket, ddrChEnabled, testType, logicalRank, logicalSubRank, bank, CurrentRow, InjTestSize, ErrInjMask, patternQW, IsUseInvtPat, uiShl, numCl, MT_ADDR_DIR_UP);
#endif  //INJECT_AMT_FAILURES_DIMM_L2

  return SUCCESS;
} // InjectErrorBeforeScramStep2

/**

  Inject error after CpgcAdvTrainingErrorStatusMATS

  @param[in] Host                    - Pointer to sysHost
  @param[in] socket                  - Memory controller number
  @param[in] ch                      - channel number
  @param[in] logicalRank             - logical rank number
  @param[in, out] cpgcErrorStatus[MAX_CH] - array of error status

  @retval   SUCCESS/FAILURE

**/
UINT32
InjectErrorTestResultsMATS (
  IN     PSYSHOST               Host,
  IN     UINT8                  socket,
  IN     UINT8                  ch,
  IN     UINT8                  logicalRank,
  IN OUT CPGC_ERROR_STATUS_MATS cpgcErrorStatus[MAX_CH]
)
{
#if INJECT_AMT_FAILURES_TEST_RESULTS

  if (ch == 0 && Host->var.mem.AmtErrInjDone < 11) {

    switch (Host->var.mem.AmtErrInjDone) {
    case 0:
    case 1:
    case 2:
      cpgcErrorStatus[ch].cpgcErrDat0S = 0;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0x4000;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x11;
      cpgcErrorStatus[ch].cpgcErrRow = 0x0000;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;

    case 3:
    case 4:
    case 5:
      cpgcErrorStatus[ch].cpgcErrDat0S = 0;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0x3000;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x11;
      cpgcErrorStatus[ch].cpgcErrRow = 0x0000;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;

    case 6:
      cpgcErrorStatus[ch].cpgcErrDat0S = 0;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x11;
      cpgcErrorStatus[ch].cpgcErrRow = 0x0000;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;

    case 7:
    case 8:
    case 9:
      cpgcErrorStatus[ch].cpgcErrDat0S = 0;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0x4000;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x11;
      cpgcErrorStatus[ch].cpgcErrRow = 0x0001;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;

    default:
      break;
    }
    Host->var.mem.AmtErrInjDone++;
  }
#endif  // INJECT_AMT_FAILURES_TEST_RESULTS

#if INJECT_AMT_FAILURES_TEST_RESULTS2

#define AMT_INJ_CNT_PER_STEP      17
#define AMT_INJ_CNT_NUM_RD_STEPS  4

  if (ch == 0 && logicalRank == 0 && bank == 0 && Host->var.mem.AmtErrInjDone < (12 + (AMT_INJ_CNT_PER_STEP * AMT_INJ_CNT_NUM_RD_STEPS * 3))) {

    switch (Host->var.mem.AmtErrInjDone) {
    case 0:
    case 1:
    case 2:
    case 3 + (AMT_INJ_CNT_PER_STEP * AMT_INJ_CNT_NUM_RD_STEPS) :
    case 4 + (AMT_INJ_CNT_PER_STEP * AMT_INJ_CNT_NUM_RD_STEPS) :
    case 5 + (AMT_INJ_CNT_PER_STEP * AMT_INJ_CNT_NUM_RD_STEPS) :
    case 6 + (AMT_INJ_CNT_PER_STEP * AMT_INJ_CNT_NUM_RD_STEPS * 2) :
    case 7 + (AMT_INJ_CNT_PER_STEP * AMT_INJ_CNT_NUM_RD_STEPS * 2) :
    case 8 + (AMT_INJ_CNT_PER_STEP * AMT_INJ_CNT_NUM_RD_STEPS * 2) :
    case 9 + (AMT_INJ_CNT_PER_STEP * AMT_INJ_CNT_NUM_RD_STEPS * 3) :
    case 10 + (AMT_INJ_CNT_PER_STEP * AMT_INJ_CNT_NUM_RD_STEPS * 3) :
    case 11 + (AMT_INJ_CNT_PER_STEP * AMT_INJ_CNT_NUM_RD_STEPS * 3) :
      cpgcErrorStatus[ch].cpgcErrDat0S = 0x4000;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0x4000;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x11;
      cpgcErrorStatus[ch].cpgcErrRow = 0x0000;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;

    default:
      cpgcErrorStatus[ch].cpgcErrDat0S = 0;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x11;
      cpgcErrorStatus[ch].cpgcErrRow = 0x0000;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;
    }
    Host->var.mem.AmtErrInjDone++;
  }
#endif  // INJECT_AMT_FAILURES_TEST_RESULTS2

#if INJECT_AMT_FAILURES_TEST_RESULTS3

  if (ch == 0 && logicalRank == 0 && bank == 0 && Host->var.mem.AmtErrInjDone < 3) {

    switch (Host->var.mem.AmtErrInjDone) {
    case 0:
    case 1:
    case 2:
      cpgcErrorStatus[ch].cpgcErrDat0S = 0x4000;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0x4000;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x11;
      cpgcErrorStatus[ch].cpgcErrRow = 0x0ffff;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;

    default:
      cpgcErrorStatus[ch].cpgcErrDat0S = 0;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x11;
      cpgcErrorStatus[ch].cpgcErrRow = 0x0000;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;
    }
    Host->var.mem.AmtErrInjDone++;
  }
#endif  // INJECT_AMT_FAILURES_TEST_RESULTS3

#if INJECT_AMT_FAILURES_TEST_RESULTS4

  if (ch == 0 && logicalRank == 0 && bank == 1 && Host->var.mem.AmtErrInjDone < 42) {

    switch (Host->var.mem.AmtErrInjDone) {
    case 0:
    case 2:
    //case 39:
    //case 41:
      cpgcErrorStatus[ch].cpgcErrDat0S = 0;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0x00100000;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x10;
      cpgcErrorStatus[ch].cpgcErrRow = 0x00017725;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;

    default:
      cpgcErrorStatus[ch].cpgcErrDat0S = 0;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x11;
      cpgcErrorStatus[ch].cpgcErrRow = 0x0000;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;
    }
    Host->var.mem.AmtErrInjDone++;
  }
#endif  // INJECT_AMT_FAILURES_TEST_RESULTS4

#if INJECT_AMT_FAILURES_TEST_RESULTS5

#define AMT_INJ_CNT_PER_STEP      18
#define AMT_INJ_CNT_NUM_RD_STEPS  1

  if (ch == 0 && logicalRank == 0 && bank == 0 && Host->var.mem.AmtErrInjDone < (12 + (AMT_INJ_CNT_PER_STEP * AMT_INJ_CNT_NUM_RD_STEPS * 3))) {

    switch (Host->var.mem.AmtErrInjDone) {
    case 0:
    case 1:
    case 2:
    case 3 + (AMT_INJ_CNT_PER_STEP * AMT_INJ_CNT_NUM_RD_STEPS) :
    case 4 + (AMT_INJ_CNT_PER_STEP * AMT_INJ_CNT_NUM_RD_STEPS) :
    case 5 + (AMT_INJ_CNT_PER_STEP * AMT_INJ_CNT_NUM_RD_STEPS) :
    case 6 + (AMT_INJ_CNT_PER_STEP * AMT_INJ_CNT_NUM_RD_STEPS * 2) :
    case 7 + (AMT_INJ_CNT_PER_STEP * AMT_INJ_CNT_NUM_RD_STEPS * 2) :
    case 8 + (AMT_INJ_CNT_PER_STEP * AMT_INJ_CNT_NUM_RD_STEPS * 2) :
    case 9 + (AMT_INJ_CNT_PER_STEP * AMT_INJ_CNT_NUM_RD_STEPS * 3) :
    case 10 + (AMT_INJ_CNT_PER_STEP * AMT_INJ_CNT_NUM_RD_STEPS * 3) :
    case 11 + (AMT_INJ_CNT_PER_STEP * AMT_INJ_CNT_NUM_RD_STEPS * 3) :
      cpgcErrorStatus[ch].cpgcErrDat0S = 0x4000;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0x4000;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x11;
      cpgcErrorStatus[ch].cpgcErrRow = 0x0000;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;

    default:
      cpgcErrorStatus[ch].cpgcErrDat0S = 0;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x11;
      cpgcErrorStatus[ch].cpgcErrRow = 0x0000;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;
    }
    Host->var.mem.AmtErrInjDone++;
  }
#endif  // INJECT_AMT_FAILURES_TEST_RESULTS5

#if INJECT_AMT_FAILURES_TEST_RESULTS7

  if (ch == 0 && Host->var.mem.AmtErrInjDone < 3) {

    switch (Host->var.mem.AmtErrInjDone) {
    case 0:
      cpgcErrorStatus[ch].cpgcErrDat0S = 0x4000;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x1;
      cpgcErrorStatus[ch].cpgcErrRow = 0x0000;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;
    case 1:
      cpgcErrorStatus[ch].cpgcErrDat0S = 0x2000;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x1;
      cpgcErrorStatus[ch].cpgcErrRow = 0x0000;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;
    case 2:
      cpgcErrorStatus[ch].cpgcErrDat0S = 0x1000;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x1;
      cpgcErrorStatus[ch].cpgcErrRow = 0x0000;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;

    default:
      cpgcErrorStatus[ch].cpgcErrDat0S = 0;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x11;
      cpgcErrorStatus[ch].cpgcErrRow = 0x0000;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;
    }
    Host->var.mem.AmtErrInjDone++;
  }
#endif  // INJECT_AMT_FAILURES_TEST_RESULTS7

#if INJECT_AMT_FAILURES_TEST_RESULTS8

  if (ch == 0 && (bank == 2 || bank == 10) && Host->var.mem.AmtErrInjDone < 3) {

    switch (Host->var.mem.AmtErrInjDone) {
    case 0:
    case 1:
    case 2:
      cpgcErrorStatus[ch].cpgcErrDat0S = 0x4000;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x1;
      cpgcErrorStatus[ch].cpgcErrRow = 0x1200;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;

    default:
      cpgcErrorStatus[ch].cpgcErrDat0S = 0;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x11;
      cpgcErrorStatus[ch].cpgcErrRow = 0x0000;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;
    }
    Host->var.mem.AmtErrInjDone++;
  }
#endif  // INJECT_AMT_FAILURES_TEST_RESULTS8

#if INJECT_AMT_FAILURES_TEST_RESULTS9

  if (ch == 0 && Host->var.mem.AmtErrInjDone < 24) {

    switch (Host->var.mem.AmtErrInjDone) {
    case 0:
    case 1:
      cpgcErrorStatus[ch].cpgcErrRow = 0x1200;
      cpgcErrorStatus[ch].cpgcErrDat0S = 0x4000;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0x2000;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrBank = 0;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x1;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;
    case 2:
      cpgcErrorStatus[ch].cpgcErrRow = 0x1200;
      cpgcErrorStatus[ch].cpgcErrDat0S = 0x4000;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0x2000;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrBank = 8;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x1;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;

    case 0 + 4 * 1:
    case 1 + 4 * 1:
      cpgcErrorStatus[ch].cpgcErrRow = 0x11ff;
      cpgcErrorStatus[ch].cpgcErrDat0S = 0x4080;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrBank = 0;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x1;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;
    case 2 + 4 * 1:
      cpgcErrorStatus[ch].cpgcErrRow = 0x11ff;
      cpgcErrorStatus[ch].cpgcErrDat0S = 0x4080;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrBank = 8;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x1;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;

    case 0 + 4 * 2:
    case 1 + 4 * 2:
      cpgcErrorStatus[ch].cpgcErrRow = 0x11fe;
      cpgcErrorStatus[ch].cpgcErrDat0S = 0x4000;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrBank = 0;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x1;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;
    case 2 + 4 * 2:
      cpgcErrorStatus[ch].cpgcErrRow = 0x11fe;
      cpgcErrorStatus[ch].cpgcErrDat0S = 0x4000;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrBank = 8;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x1;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;

    case 0 + 4 * 3:
    case 1 + 4 * 3:
      cpgcErrorStatus[ch].cpgcErrRow = 0x11fd;
      cpgcErrorStatus[ch].cpgcErrDat0S = 0x4000;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrBank = 0;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x1;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;
    case 2 + 4 * 3:
      cpgcErrorStatus[ch].cpgcErrRow = 0x11fd;
      cpgcErrorStatus[ch].cpgcErrDat0S = 0x4000;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrBank = 8;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x1;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;

    case 0 + 4 * 4:
    case 1 + 4 * 4:
      cpgcErrorStatus[ch].cpgcErrRow = 0x11fc;
      cpgcErrorStatus[ch].cpgcErrDat0S = 0x4000;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrBank = 0;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x1;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;
    case 2 + 4 * 4:
      cpgcErrorStatus[ch].cpgcErrRow = 0x11fc;
      cpgcErrorStatus[ch].cpgcErrDat0S = 0x4000;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrBank = 8;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x1;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;

    case 0 + 4 * 5:
    case 1 + 4 * 5:
      cpgcErrorStatus[ch].cpgcErrRow = 0x11fb;
      cpgcErrorStatus[ch].cpgcErrDat0S = 0x4000;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrBank = 0;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x1;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;
    case 2 + 4 * 5:
      cpgcErrorStatus[ch].cpgcErrRow = 0x11fb;
      cpgcErrorStatus[ch].cpgcErrDat0S = 0x4000;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrBank = 8;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x1;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;

    default:
      cpgcErrorStatus[ch].cpgcErrDat0S = 0;
      cpgcErrorStatus[ch].cpgcErrDat1S = 0;
      cpgcErrorStatus[ch].cpgcErrDat2S = 0;
      cpgcErrorStatus[ch].cpgcErrDat3S = 0;
      cpgcErrorStatus[ch].cpgcErrEccS = 0;
      cpgcErrorStatus[ch].cpgcSubrank = 0;
      cpgcErrorStatus[ch].cpgcErrRank = 0;
      cpgcErrorStatus[ch].cpgcErrBank = 0;
      cpgcErrorStatus[ch].cpgcErrChunk = 0x11;
      cpgcErrorStatus[ch].cpgcErrRow = 0x0000;
      cpgcErrorStatus[ch].cpgcErrColumn = 0;
      break;
    }
    Host->var.mem.AmtErrInjDone++;
  }
#endif  // INJECT_AMT_FAILURES_TEST_RESULTS9

  return SUCCESS;
} // InjectErrorTestResultsMATS

/**

  Inject errors into Row Fail List during CpgcMemTestDispositionFailRange

  @param[in] Host                    - Pointer to sysHost
  @param[in] socket                  - Memory controller number
  @param[in] ch                      - channel number

  @retval   SUCCESS/FAILURE

**/
UINT32
InjectErrorRowFailList (
  IN     PSYSHOST               Host,
  IN     UINT8                  socket,
  IN     UINT8                  ch
)
{
#if INJECT_AMT_FAILURES_ROW_LIST
  ROW_FAIL_RANGE NewFail;

  //
  // Just inject one error so we don't use up all PPR resources
  //
  if (Host->var.mem.AmtErrInjDone == 0) {
    //
    // New fail address
    //
    NewFail.addr.Data = 0;
    NewFail.addr.Bits.upperBgMask = 1;
    NewFail.addr.Bits.row = 0x10;
    NewFail.addr.Bits.bankPair = 4;
    NewFail.addr.Bits.logicalSubRank = 0;
    NewFail.addr.Bits.logicalRank = 1;
    NewFail.addr.Bits.valid = 1;
    NewFail.mask[0] = 0x00100000;
    NewFail.mask[1] = 0;
    NewFail.mask[2] = 0;
    UpdateRowFailList (Host, socket, ch, FALSE, NewFail);
    Host->var.mem.AmtErrInjDone = 1;

    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "InjectErrorRowFailList\n");

  }
#endif  // INJECT_AMT_FAILURES_ROW_LIST

  return SUCCESS;
}
