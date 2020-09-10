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
#include <Memory/MemoryCheckpointCodes.h>
#include <Library/AdvMemTestLib.h>
#include <Library/MemRcLib.h>
#include <Library/SiliconWorkaroundLib.h>

//
// Code section
//

/**

  Lookup geometry information for all logical ranks

  @param Host                - Pointer to sysHost
  @param socket              - Socket ID
  @param cBits               - number of column bits
  @param rBits               - number of row bits
  @param bBits               - number of bank bits
  @param rankEnabled         - logical rank enabled
  @param dimmRank            - dimm containing logicalRank
  @param subrankCnt          - number of subRank
  @param maxEnabledRank      - max logical rank enabled on node
  @param maxEnabledSubRank   - max logical sub rank enabled on node
  @param logical2Physical[MAX_RANK_CH][MAX_CH] - Geometery for logical rank within channel

  @retval n/a

**/
VOID
MemRankGeometry (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     cBits[MAX_RANK_CH][MAX_CH],
  UINT8     rBits[MAX_RANK_CH][MAX_CH],
  UINT8     bBits[MAX_RANK_CH][MAX_CH],
  UINT8     rankEnabled[MAX_RANK_CH][MAX_CH],
  UINT8     dimmRank[MAX_RANK_CH][MAX_CH],
  UINT8     subRankCnt[MAX_RANK_CH][MAX_CH],
  UINT8     *maxEnabledRank,
  UINT8     *maxEnabledSubRank,
  UINT8     logical2Physical[MAX_RANK_CH][MAX_CH]
  )
{
  UINT8               dimm;
  UINT8               rank;
  UINT8               logicalRank;
  UINT8               ch;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  UINT8               MaxChDdr;
  UINT8               EnabledSubRank;
  UINT8               MaxEnabledSubRank;

  *maxEnabledRank = 0;
  MaxEnabledSubRank = 0;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {

    dimmNvList = GetDimmNvList (Host, socket, ch);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      if (PcdGetBool (PcdNvDimmEn)) {
        if (Host->var.mem.subBootMode == NvDimmResume && ((*dimmNvList)[dimm].NvDimmNPresent) && ((*dimmNvList)[dimm].NvDimmStatus & STATUS_RESTORE_SUCCESSFUL)) {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "Excluded from dimm map.\n");
          continue;
        }
      }

      rankList = GetRankNvList (Host, socket, ch, dimm);

      //
      // find the subrank count
      //
      EnabledSubRank = 0;
      if (((*channelNvList)[ch].encodedCSMode == 2) &&
          ((*dimmNvList)[dimm].dieCount > 0))
      {
        EnabledSubRank = (*dimmNvList)[dimm].dieCount - 1;
      }

      if (MaxEnabledSubRank < EnabledSubRank) {
        MaxEnabledSubRank = EnabledSubRank;
      }

      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {

        //
        // Update logical rank geometry information
        //
        if (CheckRank (Host, socket, ch, dimm, rank, CHECK_MAPOUT)) {
          continue;
        }

        logicalRank = (*rankList)[rank].logicalRank;
        if (logicalRank > *maxEnabledRank) {
          *maxEnabledRank = logicalRank;
        }
        cBits[logicalRank][ch] = (*dimmNvList)[dimm].numColBits;
        rBits[logicalRank][ch] = (*dimmNvList)[dimm].numRowBits;
        bBits[logicalRank][ch] = (*dimmNvList)[dimm].numBankGroupBits + (*dimmNvList)[dimm].numBankAddressBits;

        if (((*dimmNvList)[dimm].SPDSecondaryDeviceType & SPD_HALF_SIZE_SECOND_RANK) && ((rank) % 2)) {
          rBits[logicalRank][ch] = rBits[logicalRank][ch] - 1;
        }
        if (((*dimmNvList)[dimm].SPDSecondaryDeviceType & SPD_QUARTER_SIZE_SECOND_RANK) && ((rank) % 2)) {
          rBits[logicalRank][ch] = rBits[logicalRank][ch] - 2;
        }

        dimmRank[logicalRank][ch] = dimm;
        logical2Physical[logicalRank][ch] = rank;
        subRankCnt[logicalRank][ch] = EnabledSubRank;
        if ((*dimmNvList)[dimm].DcpmmPresent) {
          rankEnabled[logicalRank][ch] = 0;
        } else {
          rankEnabled[logicalRank][ch] = 1;
        }

        if (FeaturePcdGet (PcdCteBuild) == TRUE) {
          //
          // Test only 1 bank and 2 rows in AdvMemTest for CTE
          //
          bBits[logicalRank][ch] = 1;
          if (rBits[logicalRank][ch] > 0) {
            rBits[logicalRank][ch] = 2;
          }
        }
      } // for rank
    } // for dimm
  } // for ch

  if (maxEnabledSubRank != NULL) {
    *maxEnabledSubRank = MaxEnabledSubRank;
  } // if maxEnabledSubRank != NULL

  return;
} // MemRankGeometry


/**

  Executes a step of Advanced Memory test on given row address, size, unmasked DQ lanes and logs results.
  If the testType is ADV_MT_TYPE_LTEST_SCRAM, then a test failure will retry once before logging error.
  Otherwise, the failing DRAM address will be logged.

  @param Host                - Pointer to sysHost
  @param socket              - Socket Id
  @param rankChEnMap         - Bitmask of channels to test for given logical rank, subrank
  @param rankChEnOut         - Pointer to return bitmask of channels to test next time
  @param dimmRank[MAX_RANK_CH][MAX_CH]         - DIMM containing logicalRank
  @param logical2Physical[MAX_RANK_CH][MAX_CH] - Geometery for logical rank within channel
  @param logicalRank         - Logical rank on the channel
  @param logicalSubRank      - Logical sub rank on the channel
  @param columnBits[MAX_RANK_CH][MAX_CH] - number of column bits per logical rank
  @param rowBits[MAX_RANK_CH][MAX_CH]    - number of row bits per logical rank
  @param bankBits[MAX_RANK_CH][MAX_CH]   - number of bank bits
  @param testType                        - Memory test type
  @param mode                            - Type of sequence MT_CPGC_WRITE, MT_CPGC_READ, or MT_CPGC_READ_WRITE
  @param direction                       - Sequential address direction MT_ADDR_DIR_UP, MT_ADDR_DIR_DN
  @param bank                            - Current bank address
  @param baseBits                        - Number of least significant bank bits used in SW loop
  @param BaseRow[MAX_CH]                 - Row address to start test
  @param RangeSize[MAX_CH]               - Row range size to test
  @param DeviceMask[MAX_CH][3]           - Bit mask of DQ lanes to exclude from test results
  @param TestStatus[MAX_CH]              - Pass/fail status for the test per channel
  @param seqDataInv[MT_MAX_SUBSEQ]       - Specifies whether data pattern should be inverted per subsequence

  @retval status - SUCCESS/FAILURE

**/
UINT32
CpgcMemTestRowRange (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    rankChEnMap,
  UINT32    *rankChEnOut,
  UINT8     dimmRank[MAX_RANK_CH][MAX_CH],
  UINT8     logical2Physical[MAX_RANK_CH][MAX_CH],
  UINT8     logicalRank,
  UINT8     logicalSubRank,
  UINT8     columnBits[MAX_RANK_CH][MAX_CH],
  UINT8     rowBits[MAX_RANK_CH][MAX_CH],
  UINT8     bankBits[MAX_RANK_CH][MAX_CH],
  UINT8     testType,
  UINT8     mode,
  UINT8     AddressMode,
  UINT8     direction,
  UINT8     bank,
  UINT8     baseBits,
  UINT32    BaseRow[MAX_CH],
  UINT32    RangeSize[MAX_CH],
  UINT32    DeviceMask[MAX_CH][3],
  UINT8     TestStatus[MAX_CH],
  UINT8     seqDataInv[MT_MAX_SUBSEQ]
  )
{
  UINT8     ch;
  INT32     i;
  UINT8     done;
  UINT32    TempRow[MAX_CH];
  UINT32    TempSize[MAX_CH];
  UINT32    RemSize[MAX_CH];
  BOOLEAN   secondPass;
  ROW_FAIL_RANGE  LastFail[MAX_CH];
  UINT32    TestRow[MAX_CH];
  UINT32    TestSize[MAX_CH];
  UINT8     LineMask[MAX_CH];
  UINT8     StopOnErr[MAX_CH];
  UINT8     TestMode;
  UINT8     RetryState;
  UINT8     GlobalTestStatus;
  UINT32    TempRankChEnMap;
  UINT8     MaxChDdr;

  ZeroMem ((UINT8 *)TestSize, sizeof (TestSize));
  ZeroMem ((UINT8 *)TempRow,  sizeof (TempRow));
  ZeroMem ((UINT8 *)RemSize,  sizeof (RemSize));
  ZeroMem ((UINT8 *)TestRow,  sizeof (TestRow));
  ZeroMem ((UINT8 *)TempSize, sizeof (TempSize));
  ZeroMem ((UINT8 *)LineMask, sizeof (LineMask));

  secondPass = FALSE;
  TestMode = mode;

  MaxChDdr = GetMaxChDdr ();

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((rankChEnMap & (1 << ch)) == 0) {
      continue;
    }
    TempRow[ch] = BaseRow[ch];
    RemSize[ch] = RangeSize[ch];
  }

  do {
    RetryState = GetAmtRetryState (Host);
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((rankChEnMap & (1 << ch)) == 0) {
        continue;
      }
      TempSize[ch] = 0;
      for (i = 31; i >= 0; i--) {
        if (RemSize[ch] & (1 << i)) {
          TempSize[ch] = (1 << i);
          break;
        }
      }
      if (RetryState == 0) {
        if (direction == MT_ADDR_DIR_DN) {
          TempRow[ch] -= TempSize[ch] - 1;
        }
      }

      //
      // If retry 0, then test full range
      // Else if retry 1, then test LastFail row by doing MT_CPGC_READ mode and masking cacheline associated with upper bg = 0
      // Else if retry 2, then test LastFail row by doing MT_CPGC_READ mode and masking cacheline associated with upper bg = 1
      //
      GetAmtLastFail (Host, ch, &LastFail[ch]);

      //
      // Legacy Memtest accumulates DQ errors; Advanced Memtest stops on 1st error
      //
      if (testType == ADV_MT_TYPE_LTEST_SCRAM) {
        StopOnErr[ch] = 0;
      } else {
        StopOnErr[ch] = 1;
      }

      if (RetryState) {
        //
        // RetryState > 0 will do a read of the failing bank pair and attempt to isolate the failure to a specific BG by using cache line mask feature
        // The failing BG will be saved in UpperBgMask bit-field of the ROW_ADDR
        // If the reads do not show error, then we assume a transient read error occured and the original failure will not be logged.
        //
        TestRow[ch] = LastFail[ch].addr.Bits.row;
        TestMode = MT_CPGC_READ;
        if (LastFail[ch].addr.Bits.valid) {
          TestSize[ch] = 1;
        } else {
          TestSize[ch] = 0;
        }
        //
        // Accumulate DQ errors on retry so that we can make robust decision on correctable vs. uncorrectable error per page
        //
        if (testType == ADV_MT_TYPE_RANGE_TEST_SCRAM) {
          StopOnErr[ch] = 0;
        }

        LineMask[ch] = 0x55;  // Mask odd lines
        if (((direction == MT_ADDR_DIR_UP) && (RetryState == 2)) || ((direction == MT_ADDR_DIR_DN) && (RetryState == 1))) {
          // For MT_ADDR_DIR_UP: Upper BG = 0 is even line, Upper BG = 1 is odd line
          // For MT_ADDR_DIR_DN: Upper BG = 1 is even line, Upper BG = 0 is odd line
          LineMask[ch] <<= 1;
        }
      } else {

        TestRow[ch] = TempRow[ch];
        TestSize[ch] = TempSize[ch];
        TestMode = mode;
        LineMask[ch] = 0xFF;
      }// RetryState
    } // ch

    //
    // Mask off channels that are not being tested
    //
    TempRankChEnMap = rankChEnMap;
    for (ch = 0; ch < MaxChDdr; ch++) {
      if (TestSize[ch] == 0) {
        TempRankChEnMap &= ~(1 << ch);
      }
    }
    CpgcMemTestRankSetupMATSRowRange (Host, socket, TempRankChEnMap, logicalRank, logicalSubRank,
      columnBits[logicalRank], rowBits[logicalRank], bankBits[logicalRank],
      TestMode, AddressMode, direction, bank, baseBits, TestRow, TestSize, DeviceMask, LineMask, StopOnErr, seqDataInv);

    CpgcAdvTrainingExecTest (Host, socket, TempRankChEnMap, DDR_CPGC_MEM_TEST_PATTERN);

    //
    // For READ test must check for failures
    //
    if (mode == MT_CPGC_READ_WRITE || mode == MT_CPGC_READ) {

      if (testType == ADV_MT_TYPE_LTEST_SCRAM) {

        //
        // This is legacy Memtest error handling path. Retry and map out rank if uncorrectable error repeats
        //
        if (CheckTestResults (Host, socket, TempRankChEnMap, rankChEnOut, dimmRank, logical2Physical, logicalRank, logicalSubRank, secondPass) == FAILURE) {
          secondPass = TRUE;
        } else {
          secondPass = FALSE;
        }

      } else {
        //
        // Advanced Memtest error handling path
        //
        if (CheckTestResultsMATS (Host, socket, TempRankChEnMap, dimmRank, logical2Physical, logicalRank, rowBits[logicalRank], baseBits, testType, TestStatus, direction) == FAILURE) {
          //
          // Failure means that row fail range list is full
          //
          OutputCheckpoint (CHECKPOINT_MAJOR_MEMBIST, CHECKPOINT_MINOR_MT_STEP9, 0);
          SetAmtRetryState (Host, 0);
          return FAILURE;
        }

        GlobalTestStatus = 0;
        for (ch = 0; ch < MaxChDdr; ch++) {
          if ((TempRankChEnMap & (1 << ch)) && TestStatus[ch]) {
            GlobalTestStatus = 1;

            if (RetryState) {
              //
              // Retry detected failure. Caller restarts test at LastFail row, so do not continue testing this channel
              //
              RemSize[ch] = 0;
            }
          }
        } //ch

        //
        // Read Retry FSM
        // 0 no error -> 0
        // 0 error    -> 1 read retry of even lines
        // 1          -> 2 read retry of odd lines
        // 2 error    -> 0, Fail list updated, return and caller to restart at LastFail row
        // 2 no error -> 0, Fail list not updated, return and caller to restart at LastFail row
        //
        if (RetryState == 2) {
          //
          // Reset retry state
          //
          SetAmtRetryState (Host, 0);
        } else if (RetryState || GlobalTestStatus) {
          //
          // Move to next retry state
          //
          SetAmtRetryState (Host, RetryState + 1);

        } // RetryState
      } // testType
    } // if READ

    RetryState = GetAmtRetryState (Host);
    if (RetryState > 0) {
      done = 0;

    } else {

      done = 1;
      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((rankChEnMap & (1 << ch)) == 0) {
          continue;
        }
        if (secondPass == TRUE) {
          //
          // Retry the same range
          //
          done = 0;

        } else if (RemSize[ch] > 0) {
          //
          // Move to next 2^n range until done
          //
          if (direction == MT_ADDR_DIR_UP) {
            TempRow[ch] += TempSize[ch];
            if (TempRow[ch] < BaseRow[ch] + RangeSize[ch]) {
              done = 0;
            }
          } else {
            TempRow[ch]--;       // this could result in -1
            if ((INT32) TempRow[ch] > (INT32) (BaseRow[ch] - RangeSize[ch])) {
              done = 0;
            }
          }
          RemSize[ch] &= ~TempSize[ch];
        } // secondPass
      } // ch
    }
  } while (!done);

  return SUCCESS;

} //CpgcMemTestRowRange

/**

  Executes a step of Advanced Memory test using CPGC engine and logs results

  @param Host                         - Pointer to sysHost
  @param socket                       - Socket Id
  @param ddrChEnabled                 - List of channels to be tested
  @param testType                     - Memory test type
  @param mode                         - Type of sequence MT_CPGC_WRITE, MT_CPGC_READ, or MT_CPGC_READ_WRITE
  @param AddressMode                  - FAST_X or FAST_Y
  @param BgInterleave                 - BG_INTER_1 or BG_INTER_2
  @param seqDataInv[MT_MAX_SUBSEQ]    - Enables pattern inversion per subsequence
  @param patternQW[MT_PATTERN_DEPTH]  - 64-bit wide data pattern to use per UI
  @param IsUseInvtPat                 - Info to indicate whether or not patternQW is inverted by comparing original pattern
  @param uiShl[MT_PATTERN_DEPTH]      - Enables pattern rotation per UI
  @param numCacheLines                - Number of cachelines to use in WDB
  @param patternDepth                 - Number of UI to use in patternQW
  @param direction                    - Sequential address direction MT_ADDR_DIR_UP, MT_ADDR_DIR_DN

  @retval status - SUCCESS/FAILURE

**/
UINT32
CpgcMemTestMATSGlobalSetup (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    ddrChEnMap,
  UINT8     testType,
  UINT8     mode,
  UINT8     AddressMode,
  UINT8     BgInterleave,
  UINT8     seqDataInv[MT_MAX_SUBSEQ],
  UINT64_STRUCT patternQW[MT_PATTERN_DEPTH],
  BOOLEAN   IsUseInvtPat,
  UINT8     uiShl[MT_PATTERN_DEPTH],
  UINT8     numCacheLines,
  UINT8     patternDepth,
  UINT8     direction
)
{
  UINT8   ch;
  UINT8   loopCount;
  UINT32  status = SUCCESS;
  UINT8   MaxChDdr;

  //
  // Phase 1: Global CPGC setup
  //
  loopCount = 1;
  MaxChDdr = GetMaxChDdr ();
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((ddrChEnMap & (1 << ch)) == 0) {
      continue;
    }

    CpgcMemTestGlobalSetupMATS (Host, socket, ch, 0, loopCount, testType, mode, seqDataInv, patternQW, IsUseInvtPat, uiShl, numCacheLines, patternDepth, direction);

    //
    // Program WDB data with given pattern
    //
    WriteWDBFixedPatternMATS (Host, socket, ch, mode, seqDataInv, patternQW, IsUseInvtPat, uiShl, numCacheLines, patternDepth, 0, direction, GetAdvMemtestErrInjMask (Host));

  } // ch loop

  return status;
} // CpgcMemTestMATSGlobalSetup

/**

  Returns the number of low order bank group and bank address bits that are not included in the BG interleave

  @param[in] Host                - Pointer to sysHost
  @param[in] Socket              - Socket Id
  @param[in] TestType            - Memory test type
  @param[in] DdrChEnabled        - List of channels to be tested
  @param[in] BgInterleave        - Specifies the number of BG interleaved during the test

  @retval BaseBits               - Number of bank bits for software bank loop

**/
UINT8
GetBaseBits (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     TestType,
  IN UINT32    DdrChEnMap,
  IN UINT8     BgInterleave
  )
{
  UINT8               Ch;
  UINT8               BaseBits = 0;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  if (IsDdr5Present (Host, Socket)) {

    // Note: Currently assuming 16Gb or greater density.
    // 8Gb density only has one BA bit, so baseBits would need to be decreased by 1
    if (BgInterleave > 1) {

      // Number of bank bits for HW sequence to use
      // Test DDR5 per pair of bank address with BG[2] interleave
      // This provides a good balance between fault-tolerance and performance with tCCD = 4

      BaseBits = 4;

      for (Ch = 0; Ch < MaxChDdr; Ch++) {
        if ((*ChannelNvList)[Ch].features & X16_PRESENT) {
          // Number of bank bits for HW sequence to use
          // Test DDR5 per pair of bank address with BG[1] interleave
          // This provides a good balance between fault-tolerance and performance with tCCD = 4
          // Per-bank testing (i.e. baseBits = 3) requires tCCD_L and a significant latency increase
          BaseBits = 3;
        }
      }
    }
    if (FeaturePcdGet (PcdCteBuild) == TRUE) {
      // Test only 1 bank in AdvMemTest for CTE
      BaseBits = 0;
    }
  } // DDR5

  else {

    if (BgInterleave > 1) {

      // Number of bank bits for HW sequence to use
      // Test DDR4 per pair of bank address with BG[1] interleave
      // This provides a good balance between fault-tolerance and performance with tCCD = 4
      // Per-bank testing (i.e. baseBits = 4) requires tCCD_L and a significant latency increase
      BaseBits = 3;

      for (Ch = 0; Ch < MaxChDdr; Ch++) {
        if ((*ChannelNvList)[Ch].features & X16_PRESENT) {
          // Number of bank bits for HW sequence to use
          // Test DDR4 per pair of bank address with BG[0] interleave
          // This provides a good balance between fault-tolerance and performance with tCCD = 4
          // Per-bank testing (i.e. baseBits = 3) requires tCCD_L and a significant latency increase
          BaseBits = 2;
        }
      }
    } else {

      BaseBits = 4;
      for (Ch = 0; Ch < MaxChDdr; Ch++) {
        if ((*ChannelNvList)[Ch].features & X16_PRESENT) {
          BaseBits = 3;   // TODO: make baseBits a ch array
        }
      }
    }
    if (FeaturePcdGet (PcdCteBuild) == TRUE) {
      // Test only 1 bank in AdvMemTest for CTE
      BaseBits = 0;
    }
  }


  return BaseBits;

} // GetBaseBits

/**

  Executes a step of Advanced Memory test using CPGC engine and logs results

  @param Host                         - Pointer to sysHost
  @param socket                       - Socket Id
  @param ddrChEnabled                 - List of channels to be tested
  @param testType                     - Memory test type
  @param mode                         - Type of sequence MT_CPGC_WRITE, MT_CPGC_READ, or MT_CPGC_READ_WRITE
  @param AddressMode                  - FAST_X or FAST_Y
  @param BgInterleave                 - BG_INTER_1 or BG_INTER_2
  @param seqDataInv[MT_MAX_SUBSEQ]    - Enables pattern inversion per subsequence
  @param patternQW[MT_PATTERN_DEPTH]  - 64-bit wide data pattern to use per UI
  @param IsUseInvtPat                 - Info to indicate whether or not patternQW is inverted by comparing original pattern
  @param uiShl[MT_PATTERN_DEPTH]      - Enables pattern rotation per UI
  @param numCacheLines                - Number of cachelines to use in WDB
  @param patternDepth                 - Number of UI to use in patternQW
  @param direction                    - Sequential address direction MT_ADDR_DIR_UP, MT_ADDR_DIR_DN

  @retval status - SUCCESS/FAILURE

**/
UINT32
CpgcMemTestMATS (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    ddrChEnMap,
  UINT8     testType,
  UINT8     mode,
  UINT8     AddressMode,
  UINT8     BgInterleave,
  UINT8     seqDataInv[MT_MAX_SUBSEQ],
  UINT64_STRUCT patternQW[MT_PATTERN_DEPTH],
  BOOLEAN   IsUseInvtPat,
  UINT8     uiShl[MT_PATTERN_DEPTH],
  UINT8     numCacheLines,
  UINT8     patternDepth,
  UINT8     direction
)
{
  UINT32  rankChEnMap;
  UINT32  rankChEnOut;
  UINT8   logicalRank;
  UINT8   logicalSubRank;
  UINT8   ch;
  UINT8   columnBits[MAX_RANK_CH][MAX_CH];
  UINT8   rowBits[MAX_RANK_CH][MAX_CH];
  UINT8   bankBits[MAX_RANK_CH][MAX_CH];
  UINT8   rankEnabled[MAX_RANK_CH][MAX_CH];
  UINT8   dimmRank[MAX_RANK_CH][MAX_CH];
  UINT8   subRankCnt[MAX_RANK_CH][MAX_CH];
  UINT8   logical2Physical[MAX_RANK_CH][MAX_CH];
  UINT8   maxEnabledRank;
  UINT8   maxEnabledSubRank;
  UINT8   bank, baseBits;
  ROW_FAIL_RANGE *FailRangePtr;
  struct channelNvram (*channelNvList)[MAX_CH];
  UINT32 status;
  UINT32 RetVal = SUCCESS;
  ROW_ADDR CurAddr[MAX_CH];
  ROW_ADDR FailAddr[MAX_CH];
  ROW_FAIL_RANGE LastFail[MAX_CH];
  UINT32 FailSize[MAX_CH];
  INT32 FailIndex;
  UINT32 FailMax;
  UINT32 MaxRow[MAX_CH];
  UINT32 CurrentRow[MAX_CH];
  UINT32 TestSize[MAX_CH];
  UINT32 DeviceMask[MAX_CH][3];
  UINT8  TestStatus[MAX_CH];
  UINT32 RunTest;
  UINT32 done;
  UINT8  MaxChDdr;
  UINT32 TempIndex;

  ZeroMem (columnBits, sizeof (columnBits));
  ZeroMem (rowBits, sizeof (rowBits));
  ZeroMem (bankBits, sizeof (bankBits));
  ZeroMem (rankEnabled, sizeof (rankEnabled));
  ZeroMem (dimmRank, sizeof (dimmRank));
  ZeroMem (subRankCnt, sizeof (subRankCnt));
  ZeroMem (logical2Physical, sizeof (logical2Physical));
  ZeroMem (LastFail, sizeof (LastFail));

  // Appease Klockwork
  FailRangePtr = &LastFail[0];

  RcDebugPrintWithDevice (SDBG_CPGC, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "CpgcMemTestMATS: ddrChEnMap = 0x%x, mode = 0x%x\n", ddrChEnMap, mode);

  //
  // Phase 1: Global CPGC setup
  //
  CpgcMemTestMATSGlobalSetup (Host, socket, ddrChEnMap, testType, mode, AddressMode, BgInterleave, seqDataInv, patternQW,
                              IsUseInvtPat, uiShl, numCacheLines, patternDepth, direction);

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);

  //
  // Phase 2: Per logical rank setup and test
  //

  //
  // Find geometery for logical rank within channel.
  //
  MemRankGeometry (Host, socket, columnBits, rowBits, bankBits, rankEnabled, dimmRank, subRankCnt, &maxEnabledRank, &maxEnabledSubRank, logical2Physical);

  //
  // Appease KW
  //
  if (maxEnabledRank >= MAX_RANK_CH) {
    maxEnabledRank = MAX_RANK_CH - 1;
  }

  //
  // Loop over all ranks and subranks
  //
  for (logicalSubRank = 0; logicalSubRank <= maxEnabledSubRank; logicalSubRank++) {
    for (logicalRank = 0; logicalRank <= maxEnabledRank; logicalRank++) {

      baseBits = GetBaseBits (Host, socket, testType, ddrChEnMap, BgInterleave);
      rankChEnMap = ddrChEnMap;
      for (ch = 0; ch < MaxChDdr; ch++) {
        rankChEnMap &= ~((rankEnabled[logicalRank][ch] == 0) << ch);
        rankChEnMap &= ~((subRankCnt[logicalRank][ch] < logicalSubRank) << ch);
#ifdef LRDIMM_SUPPORT
        if ((IsLrdimmPresent (socket, ch, 0) && ((*channelNvList)[ch].encodedCSMode == 0) && logicalSubRank > 0)) {
          rankChEnMap &= ~(1 << ch);
        }
#endif
      } //ch

      //
      // Skip to next rank if no ranks enabled on any channel
      //
      if (rankChEnMap == 0) {
        continue;
      }
      rankChEnOut = rankChEnMap;

      //
      // For each Bank address
      //
      for (bank = 0; (bank < (1 << baseBits)) && rankChEnMap; bank++) {
        //
        // Default test status to passing
        //
        ZeroMem (TestStatus, sizeof (TestStatus));
        ZeroMem (MaxRow, sizeof (MaxRow));

        for (ch = 0; ch < MaxChDdr; ch++) {

          //
          // Inititalize upper address
          //
          CurAddr[ch].Data = 0;
          CurAddr[ch].Bits.bankPair = bank;
          CurAddr[ch].Bits.logicalSubRank = logicalSubRank;
          CurAddr[ch].Bits.logicalRank = logicalRank;
          CurAddr[ch].Bits.valid = 1;

          //
          // Get max row bits
          //
          if (rowBits[logicalRank][ch]) {
            MaxRow[ch] = 1 << rowBits[logicalRank][ch];
          }

          //
          // Search both directions
          //
          if (direction == MT_ADDR_DIR_UP) {
            for (TempIndex = 0; TempIndex < GetRowFailMax (Host, ch); TempIndex++) {
              if (CurrentAddrMatch (CurAddr[ch], GetRowFailRange (Host, ch, TempIndex)->addr)) {
                break;
              }
            }
            SetRowFailIndex (Host, ch, TempIndex);
            CurrentRow[ch] = 0;
          } else {
            for (TempIndex = GetRowFailMax (Host, ch) - 1; (INT32)TempIndex >= 0; TempIndex--) {
              if (CurrentAddrMatch (CurAddr[ch], GetRowFailRange (Host, ch, TempIndex)->addr)) {
                break;
              }
            }
            SetRowFailIndex (Host, ch, TempIndex);
            CurrentRow[ch] = MaxRow[ch] - 1;
          }
        } //ch

        do {
          RunTest = 0;
          for (ch = 0; ch < MaxChDdr; ch++) {

            if ((rankChEnMap & (1 << ch)) == 0) {
              continue;
            }

            //
            // FailIndex and FailMax can be updated when Fail range list changes in subordinate functions
            //
            FailIndex = GetRowFailIndex (Host, ch);
            FailMax = GetRowFailMax (Host, ch);
            if (FailIndex >= 0) {
              FailRangePtr = GetRowFailRange (Host, ch, FailIndex);
            }
            //
            // Initialize to invalid fail address
            //
            FailAddr[ch].Data = 0;
            FailSize[ch] = 0;

            //
            // Get current fail address
            //
            if ((FailIndex >= 0) && (FailIndex < (INT32) FailMax) &&
                (testType != ADV_MT_TYPE_LTEST_SCRAM) &&
                (testType != ADV_MT_TYPE_LINIT_SCRAM) &&
                (mode != MT_CPGC_WRITE)) {
              FailRangePtr = GetRowFailRange (Host, ch, FailIndex);
              FailAddr[ch] = FailRangePtr->addr;
              FailSize[ch] = FailRangePtr->size;
            }

            //
            // Check valid bit and matching upper address
            //
            TestSize[ch] = 0;
            if (CurrentAddrMatch (CurAddr[ch], FailAddr[ch])) {

              if (direction == MT_ADDR_DIR_UP) {
                //
                // In passing range searching upward
                //
                if (CurrentRow[ch] < FailAddr[ch].Bits.row) {
                  TestSize[ch] = FailAddr[ch].Bits.row - CurrentRow[ch];
                  DeviceMask[ch][0] = 0;
                  DeviceMask[ch][1] = 0;
                  DeviceMask[ch][2] = 0;
                } else {
                  //
                  //  In failing range searching upward
                  //
                  if (TestStatus[ch] == 0) {
                    CurrentRow[ch] = FailAddr[ch].Bits.row;
                  }
                  //
                  // If previous test failed, then current row is last fail row
                  //
                  TestSize[ch] = FailSize[ch] - (CurrentRow[ch] - FailAddr[ch].Bits.row);
                  FailRangePtr = GetRowFailRange (Host, ch, FailIndex);
                  DeviceMask[ch][0] = FailRangePtr->mask[0];
                  DeviceMask[ch][1] = FailRangePtr->mask[1];
                  DeviceMask[ch][2] = FailRangePtr->mask[2];
                }

              } else { // direction

                //
                // In passing range searching downward
                //
                if (CurrentRow[ch] >= FailAddr[ch].Bits.row + FailSize[ch]) {
                  TestSize[ch] = CurrentRow[ch] - (FailAddr[ch].Bits.row + FailSize[ch] - 1);
                  DeviceMask[ch][0] = 0;
                  DeviceMask[ch][1] = 0;
                  DeviceMask[ch][2] = 0;
                } else {
                  //
                  //  In failing range searching downward
                  //
                  if (TestStatus[ch] == 0) {
                    CurrentRow[ch] = FailAddr[ch].Bits.row + FailSize[ch] - 1;
                  }
                  //
                  // If previous test failed, then current row is last fail row
                  //
                  TestSize[ch] = CurrentRow[ch] + 1 - FailAddr[ch].Bits.row;
                  FailRangePtr = GetRowFailRange (Host, ch, FailIndex);
                  DeviceMask[ch][0] = FailRangePtr->mask[0];
                  DeviceMask[ch][1] = FailRangePtr->mask[1];
                  DeviceMask[ch][2] = FailRangePtr->mask[2];
                }
              } // direction

            } else if ((FailIndex >= 0) && (FailIndex < (INT32) FailMax) &&
                       (testType != ADV_MT_TYPE_LTEST_SCRAM) &&
                       (testType != ADV_MT_TYPE_LINIT_SCRAM) &&
                       (mode != MT_CPGC_WRITE)) {

              if (direction == MT_ADDR_DIR_UP) {
                SetRowFailIndex (Host, ch, FailIndex + 1);
              } else {
                SetRowFailIndex (Host, ch, FailIndex - 1);
              }

            } else {

              //
              // No more failures to process
              // ADV_MT_TYPE_LTEST_SCRAM, ADV_MT_TYPE_LINIT_SCRAM, MT_CPGC_WRITE also take this path
              //
              if (direction == MT_ADDR_DIR_UP) {
                TestSize[ch] = MaxRow[ch] - CurrentRow[ch];
              } else {
                TestSize[ch] = CurrentRow[ch] + 1;
              }
              DeviceMask[ch][0] = 0;
              DeviceMask[ch][1] = 0;
              DeviceMask[ch][2] = 0;
            }

            if (TestSize[ch]) {
              RunTest = 1;
            }
          } //ch

          if (RunTest) {
            //
            // Execute concurrent ch test on current row, test size and device mask
            // TestStatus is set for failure and LastFail will contain failing address
            //
            status = CpgcMemTestRowRange (Host, socket, rankChEnMap, &rankChEnOut, dimmRank, logical2Physical, logicalRank, logicalSubRank,
                       columnBits, rowBits, bankBits, testType, mode, AddressMode, direction, bank, baseBits,
                       CurrentRow, TestSize, DeviceMask, TestStatus, seqDataInv);
            //
            // Failure means that FailRange list is full
            //
            if (status == FAILURE) {
              RcDebugPrintWithDevice (SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "CpgcMemTestRowRange returned failure - FailRange List Full!\n");

              return status;
            }

            //
            // Evaluate test results
            //
            rankChEnMap = rankChEnOut;
            for (ch = 0; ch < MaxChDdr; ch++) {

              //
              // Handle rank that was disabled
              //
              rankEnabled[logicalRank][ch] = ((rankChEnOut >> ch) & 1);
              if ((rankChEnMap & (1 << ch)) == 0) {
                continue;
              }

              //
              // If the test ran on this channel
              //
              if (TestSize[ch]) {
                //
                // If test passed
                //
                if (TestStatus[ch] == 0) {
                  //
                  // If completed testing a fail range
                  //
                  if ((CurrentRow[ch] >= FailAddr[ch].Bits.row) && (CurrentRow[ch] < FailAddr[ch].Bits.row + FailSize[ch])) {
                    //
                    // Move to next fail range
                    //
                    FailIndex = GetRowFailIndex (Host, ch);
                    if (direction == MT_ADDR_DIR_UP) {
                      if (FailIndex < MAX_FAIL_RANGE) {
                        SetRowFailIndex (Host, ch, FailIndex + 1);
                      }
                    } else {
                      if (FailIndex >= 0) {
                        SetRowFailIndex (Host, ch, FailIndex - 1);
                      }
                    }
                  }
                  //
                  // Test passed so update CurrentRow
                  //
                  if (direction == MT_ADDR_DIR_UP) {
                    CurrentRow[ch] += TestSize[ch];
                  } else {
                    CurrentRow[ch] -= TestSize[ch];  // could result in -1, so casting is needed on exit path
                  }
                } else {

                  GetAmtLastFail (Host, ch, &LastFail[ch]);

                  //
                  // Assert that the last fail address is within the current test range
                  //
                  if (((direction == MT_ADDR_DIR_UP) && ((LastFail[ch].addr.Bits.row < CurrentRow[ch]) || (LastFail[ch].addr.Bits.row >= CurrentRow[ch] + TestSize[ch]))) ||
                      ((direction == MT_ADDR_DIR_DN) && ((LastFail[ch].addr.Bits.row > CurrentRow[ch]) || ((INT32)LastFail[ch].addr.Bits.row <= (INT32)(CurrentRow[ch] - TestSize[ch]))))) {
                    //
                    // Something went wrong if we got failure on a row, mask that was not part of the current test range
                    //
                    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                    "CpgcMemTestMATS: Failure occured outside of test range: LastFail row = 0x%08x, CurrentRow = 0x%08x, TestSize = 0x%08x, direction = %d\n",
                                    LastFail[ch].addr.Bits.row, CurrentRow[ch], TestSize[ch], direction);

                    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_99);
                  }
                  //
                  // Test failed so restart at last fail address!
                  //
                  CurrentRow[ch] = LastFail[ch].addr.Bits.row;
                }
              }
            } //ch
          } //if RunTest

          //
          // Check if all rows done
          //
          done = 1;
          for (ch = 0; ch < MaxChDdr; ch++) {
            if ((rankChEnMap & (1 << ch)) == 0) {
              continue;
            }
            if (direction == MT_ADDR_DIR_UP) {
              //
              // Searching upward
              //
              if (CurrentRow[ch] < MaxRow[ch]) {
                done = 0;
                break;
              }
            } else {
              //
              // Searching downward
              //
              if ((INT32) CurrentRow[ch] >= 0) {
                done = 0;
                break;
              }
            } // direction
          } // ch
        } while (!done);

      } //for bank
    } // for logicalRank
  } // for logicalSubRank

  //
  // Print progress indicator in MIN message level to let user know that boot is not hung
  //
  RcDebugPrint (SDBG_MINMAX, ".");

  return RetVal;
} // CpgcMemTestMATS


/**

  Check row failure list and PPR resource list to determine if repairs are required

  @param Host                - Pointer to sysHost
  @param socket              - Socket Id
  @param ddrChEnabled        - List of channels to be tested

  @retval status - TRUE/FALSE

**/
BOOLEAN
IsPprRepairRequired (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT32    ChBitmask,
  UINT8     DimmRank[MAX_RANK_CH][MAX_CH],
  UINT8     Logical2Physical[MAX_RANK_CH][MAX_CH],
  UINT8     BaseBits
)
{
  ROW_FAIL_RANGE      *FailRangePtr;
  UINT32              FailIndex;
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               LogicalRank;
  UINT8               LogicalSubRank;
  UINT8               Rank;
  UINT8               Bank;
  UINT8               BankGroup;
  UINT8               PprBank;
  UINT8               NumBaBits;
  UINT8               Nibble;
  UINT8               Dram;
  UINT8               NibblesPerDram;
  UINT32              DeviceDqMask;
  UINT32              RowFailMax;
  struct dimmNvram   (*dimmNvList)[MAX_DIMM];
  UINT8               MaxChDdr;
  SYS_SETUP          *Setup;
  UINT8               MSVx4 = GetMaxStrobeValid(Host);

  Setup = GetSysSetupPointer();

  //
  // Check setup option to enable PPR
  //
  if (Setup->mem.pprType != PPR_DISABLED) {

    //
    // Inspect test results for given logicalRank, logicalSubRank, bank
    //
    MaxChDdr = GetMaxChDdr ();
    for (Ch = 0; Ch < MaxChDdr; Ch++) {

      //
      // Was this channel part of the test group?
      //
      if (ChBitmask & (1 << Ch)) {

        //
        // Loop through all row failure ranges
        //
        RowFailMax = GetRowFailMax (Host, Ch);
        for (FailIndex = 0; FailIndex < RowFailMax; FailIndex++) {

          FailRangePtr = GetRowFailRange (Host, Ch, FailIndex);

          LogicalSubRank = (UINT8) FailRangePtr->addr.Bits.logicalSubRank;
          LogicalRank = (UINT8) FailRangePtr->addr.Bits.logicalRank;
          Dimm = DimmRank[LogicalRank][Ch];
          Rank = Logical2Physical[LogicalRank][Ch];
          Bank = (UINT8) FailRangePtr->addr.Bits.bankPair;

          for (BankGroup = 0; BankGroup < 2; BankGroup++) {
            if (FailRangePtr->addr.Bits.upperBgMask & (1 << BankGroup)) {
              PprBank = (BankGroup << BaseBits) + Bank;

              // TODO - make number of BA bits dynamic for DDR5
              NumBaBits = 2;

              //
              // Construct DRAM Mask
              //
              dimmNvList = GetDimmNvList (Host, Socket, Ch);
              if ((*dimmNvList)[Dimm].x4Present) {
                NibblesPerDram = 1;
                DeviceDqMask = 0xF;
              } else {
                NibblesPerDram = 2;
                DeviceDqMask = 0xFF;
              }

              for (Dram = 0; Dram < MSVx4 / NibblesPerDram; Dram++) {
                Nibble = Dram * NibblesPerDram;
                if ((FailRangePtr->mask[Nibble / 8] >> ((Nibble % 8) * 4)) & DeviceDqMask) {

                  if (GetPprResourceAvailable (Host, Socket, Ch, Dimm, Rank, LogicalSubRank, Dram, PprBank >> NumBaBits)) {
                    return 1;
                  } // GetPprResourceAvailable
                } // FailRangePtr->mask
              } // for Dram
            } // if FailRangePtr->addr.Bits.upperBgMask
          } // for BankGroup
        } // for FailIndex
      } // if ChBitmask
    } // for Ch
  } // if Host->setup.mem.pprType

  return 0;

} // IsPprRepairRequired


  /**

  Executes a step of Advanced Memory test using CPGC engine and logs results

  @param Host                - Pointer to sysHost
  @param socket              - Socket Id
  @param ddrChEnabled        - List of channels to be tested
  @param testType            - Memory test type
  @param seqDataInv[MT_MAX_SUBSEQ]    - Enables pattern inversion per subsequence
  @param patternQW[MT_PATTERN_DEPTH]  - 64-bit wide data pattern to use per UI
  @param uiShl[MT_PATTERN_DEPTH]      - Enables pattern rotation per UI
  @param numCacheLines                - Number of cachelines to use in WDB
  @param patternDepth                 - Number of UI to use in patternQW
  @param direction                    - Sequential address direction MT_ADDR_DIR_UP, MT_ADDR_DIR_DN

  @retval status - SUCCESS/FAILURE

  **/
UINT32
CpgcMemTestDispositionFailRange (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    ddrChEnMap,
  UINT8     testType,
  UINT8     BgInterleave,
  UINT8     *RepairDone
)
{
  UINT32  rankChEnMap;
  UINT32  rankChEnOut;
  UINT8   logicalRank;
  UINT8   logicalSubRank;
  UINT8   ch;
  UINT8   columnBits[MAX_RANK_CH][MAX_CH];
  UINT8   rowBits[MAX_RANK_CH][MAX_CH];
  UINT8   bankBits[MAX_RANK_CH][MAX_CH];
  UINT8   rankEnabled[MAX_RANK_CH][MAX_CH];
  UINT8   dimmRank[MAX_RANK_CH][MAX_CH];
  UINT8   subRankCnt[MAX_RANK_CH][MAX_CH];
  UINT8   logical2Physical[MAX_RANK_CH][MAX_CH];
  UINT8   maxEnabledRank;
  UINT8   maxEnabledSubRank;
  UINT8   baseBits;
  UINT8   RowFailuresFound;
  struct channelNvram (*channelNvList)[MAX_CH];
  AMT_TEST_CONDITION PrevAmtTestCond;
  UINT32 RetVal = SUCCESS;
  BOOLEAN  PprRequired;
  SYS_SETUP         *Setup;
  UINT8 MaxChDdr;

  ZeroMem (columnBits, sizeof (columnBits));
  ZeroMem (rowBits, sizeof (rowBits));
  ZeroMem (bankBits, sizeof (bankBits));
  ZeroMem (rankEnabled, sizeof (rankEnabled));
  ZeroMem (dimmRank, sizeof (dimmRank));
  ZeroMem (subRankCnt, sizeof (subRankCnt));
  ZeroMem (logical2Physical, sizeof (logical2Physical));
  *RepairDone = 0;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  //
  // Inject error in row fail list for testing purposes
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    if (ddrChEnMap & (1 << ch)) {
      InjectErrorRowFailList (Host, socket, ch);
      break;
    }
  }

  //
  // Are there any failures in the list?
  //
  RowFailuresFound = 0;
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((ddrChEnMap & (1 << ch)) == 0) {
      continue;
    }
    if (GetRowFailMax (Host, ch) > 0) {
      RowFailuresFound = 1;
    }
  }

  if (RowFailuresFound) {

    //
    // Find geometery for logical rank within channel.
    //
    MemRankGeometry (Host, socket, columnBits, rowBits, bankBits, rankEnabled, dimmRank, subRankCnt, &maxEnabledRank, &maxEnabledSubRank, logical2Physical);
    baseBits = GetBaseBits(Host, socket, testType, ddrChEnMap, BgInterleave);

    //
    // Set test condition for PPR flow
    //
    PprRequired = IsPprRepairRequired (Host, socket, ddrChEnMap, dimmRank, logical2Physical, baseBits);
    if (PprRequired) {
      SetAdvMemTestCondition (Host, socket, ddrChEnMap, ADV_MT_TYPE_PPR, AMT_SET_TC, &PrevAmtTestCond);

      RcDebugPrint (SDBG_MAX,
        "\nExecute PPR flow to repair row failures\n");
    }

    //
    // Appease KW
    //
    if (maxEnabledRank >= MAX_RANK_CH) {
      maxEnabledRank = MAX_RANK_CH - 1;
    }

    //
    // Loop over all ranks and subranks
    //
    channelNvList = GetChannelNvList (Host, socket);
    for (logicalSubRank = 0; logicalSubRank <= maxEnabledSubRank; logicalSubRank++) {
      for (logicalRank = 0; logicalRank <= maxEnabledRank; logicalRank++) {

        rankChEnMap = ddrChEnMap;
        for (ch = 0; ch < MaxChDdr; ch++) {
          rankChEnMap &= ~((rankEnabled[logicalRank][ch] == 0) << ch);
          rankChEnMap &= ~((subRankCnt[logicalRank][ch] < logicalSubRank) << ch);
#ifdef LRDIMM_SUPPORT
          if ((IsLrdimmPresent (socket, ch, 0) && ((*channelNvList)[ch].encodedCSMode == 0) && logicalSubRank > 0)) {
            rankChEnMap &= ~(1 << ch);
          }
#endif
        } //ch

          //
          // Skip to next rank if no ranks enabled on any channel
          //
        if (rankChEnMap == 0) {
          continue;
        }
        //
        // Search row fail list and apply PPR repairs as needed
        //
        if (PprRequired) {
          DispositionFailRangesWithPprFlow (Host, socket, rankChEnMap, dimmRank, logical2Physical, logicalRank, logicalSubRank, baseBits, RepairDone);
        }

        //
        // Handle Advanced Memtest option to use row fail ranges to apply legacy logic to tolerate correctable errors or map out uncorrectable errors
        //
        if (testType == ADV_MT_TYPE_RANGE_TEST_SCRAM) {
          DispositionFailRanges (Host, socket, rankChEnMap, &rankChEnOut, dimmRank, logical2Physical, logicalRank, logicalSubRank, baseBits);
          for (ch = 0; ch < MaxChDdr; ch++) {
            rankEnabled[logicalRank][ch] = ((rankChEnOut >> ch) & 1);
          }
        }

      } // for logicalRank
    } // for logicalSubRank

    //
    // Restore test condition for retry
    //
    if (PprRequired) {
      SetAdvMemTestCondition (Host, socket, ddrChEnMap, ADV_MT_TYPE_PPR, AMT_RESTORE_TC, &PrevAmtTestCond);
    }
    //
    // Any repair done?
    //
    if (*RepairDone) {
      RcDebugPrint (SDBG_MAX, "\nPPR repair done so retry MemTest!\n");
    }
  } // RowFailuresFound

  return RetVal;
} // CpgcMemTestDispositionFailRange
