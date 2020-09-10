/** @file
  MemTestFunc.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018-2020 Intel Corporation. <BR>

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

#ifndef  _MEM_TEST_FUNC_H_
#define  _MEM_TEST_FUNC_H_

//
// AdvMemTest external dependencies
//

#include <ReferenceCodeDataTypes.h>
#include "SysHost.h"
#include <Library/SysHostPointerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/IoAccessLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/TimerLib.h>

#define MAX_BAD_ROW_LIMIT_PER_BANK  8

typedef enum {
  ErrorTypeCorrectable = 0,
  ErrorTypeUncorrectable =1
} MEM_TEST_ERROR_TYPE;

//
// AdvMemTest private functions
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
  );


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
);

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
  );

/**

  Checks the results of the preceding advanced memory test

  @param[in]  Host                    - Pointer to sysHost, the system Host (root) structure
  @param[in]  socket                  - Socket Id
  @param[in]  chBitmask               - List of channels that were tested
  @param[in]  dimmRank                - DIMM containing logicalRank
  @param[in]  logical2Physical        - Geometery for logical rank within channel
  @param[in]  logicalRank             - Logical rank
  @param[in]  rowBits                 - Number of row bits supported by current logical rank
  @param[in]  baseBits                - Number of bank bits in SW loop
  @param[in]  testType                - Memory test type for logging purposes
  @param[out] TestStatus[MAX_CH]      - Pass/fail status for the test per channel
  @param[in]  Direction               - Sequential address direction MT_ADDR_DIR_UP, MT_ADDR_DIR_DN

  @retval status - SUCCESS/FAILURE

**/
UINT32
CheckTestResultsMATS (
  IN     PSYSHOST Host,
  IN     UINT8     socket,
  IN     UINT32    chBitmask,
  IN     UINT8     dimmRank[MAX_RANK_CH][MAX_CH],
  IN     UINT8     logical2Physical[MAX_RANK_CH][MAX_CH],
  IN     UINT8     logicalRank,
  IN     UINT8     rowBits[MAX_CH],
  IN     UINT8     baseBits,
  IN     UINT8     testType,
     OUT UINT8     TestStatus[MAX_CH],
  IN     UINT8     Direction
  );

/**

  Checks results of the preceding memory test

  @param Host             - Pointer to sysHost, the system Host (root) structure
  @param socket           - Socket Id
  @param chBitmask        - List of channels that were tested
  @param chBitmaskOut     - Returned list of channels that still have rank enabled
  @param dimmRank         - DIMM containing logicalRank
  @param logical2Physical - Geometery for logical rank within channel
  @param logicalRank      - Logical rank
  @param logicalSubRank   - SubRank
  @param secondPass       - Second Pass check: 0 = 1st Pass, 1 = 2nd Pass

  @retval status - SUCCESS/FAILURE

**/
UINT32
CheckTestResults (
  PSYSHOST Host,                                            // Pointer to the system Host (root) structure
  UINT8     socket,                                         // Socket Id
  UINT32    chBitmask,                                      // List of channels that were tested
  UINT32   *chBitmaskOut,                                   // List of channels that were tested and still have rank enabled
  UINT8     dimmRank[MAX_RANK_CH][MAX_CH],                  // DIMM containing logicalRank
  UINT8     logical2Physical[MAX_RANK_CH][MAX_CH],          // Geometery for logical rank within channel
  UINT8     logicalRank,                                    // Logical rank
  UINT8     logicalSubRank,                                 // SubRank
  BOOLEAN   secondPass                                      // Second Pass check: 0 = 1st Pass, 1 = 2nd Pass
  );

/**

  Searches the Row failure list to classify errors as correctable/uncorrectable and
  apply error handling policy like CheckTestResults

  @param Host             - Pointer to sysHost, the system Host (root) structure
  @param socket           - Socket Id
  @param chBitmask        - List of channels that have rank enabled
  @param chBitmaskOut     - Returned list of channels that still have rank enabled
  @param dimmRank         - DIMM containing logicalRank
  @param logical2Physical - Geometery for logical rank within channel
  @param logicalRank      - Logical rank
  @param logicalSubRank   - SubRank
  @param baseBits         - Number of bank bits in SW loop

  @retval   None

**/
VOID
DispositionFailRanges (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    chBitmask,
  UINT32    *chBitmaskOut,
  UINT8     dimmRank[MAX_RANK_CH][MAX_CH],
  UINT8     logical2Physical[MAX_RANK_CH][MAX_CH],
  UINT8     logicalRank,
  UINT8     logicalSubRank,
  UINT8     baseBits
  );

typedef struct {
  BOOLEAN  OpenPageMode;
  UINT16   VddLevel;
  UINT8    TwrValue;
  UINT8    Mr0TwrValue;
  UINT8    ScramblerEn;
  UINT16   TrefiValue;
  UINT32   ScrambleEnConfig;
} AMT_TEST_CONDITION;

#define AMT_SET_TC      0
#define AMT_RESTORE_TC  1

/**

  Set Advanced Memory Test condition

  @param Host                - Pointer to sysHost
  @param socket              - Socket Id
  @param ddrChEnabled        - List of channels to be tested
  @param testType            - Memory test type
  @param testSize            - Memory test size

  @retval status             - SUCCESS/FAILURE

**/
UINT32
SetAdvMemTestCondition (
  PSYSHOST Host,
  UINT8    socket,
  UINT32   ddrChEnabled,
  UINT8    TestType,
  UINT8    Operation,
  AMT_TEST_CONDITION *PrevAmtTestCond
);

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
);

/**

  Searches the Row failure list to classify errors as correctable/uncorrectable and
  apply error handling policy like CheckTestResults

  @param Host             - Pointer to sysHost, the system Host (root) structure
  @param socket           - Socket Id
  @param chBitmask        - List of channels that have rank enabled
  @param chBitmaskOut     - Returned list of channels that still have rank enabled
  @param dimmRank         - DIMM containing logicalRank
  @param logical2Physical - Geometery for logical rank within channel
  @param logicalRank      - Logical rank
  @param logicalSubRank   - SubRank
  @param baseBits         - Number of bank bits in SW loop

  @retval   None

**/
UINT32
DispositionFailRangesWithPprFlow (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    chBitmask,
  UINT8     dimmRank[MAX_RANK_CH][MAX_CH],
  UINT8     logical2Physical[MAX_RANK_CH][MAX_CH],
  UINT8     logicalRank,
  UINT8     logicalSubRank,
  UINT8     baseBits,
  UINT8     *RepairDone
);

/**

  Searches the failure tracking list for NewFail and returns bit mask of the DQ bits with
  number of row failures greater or equal to the given threshoold.

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param socket             - Socket Id
  @param ch                 - Channel of the new failure
  @param NewFail            - New failure information
  @param Threshold          - Number of errors per DQ where mask bit is set
  @param DqMaskOver         - Bitmask of DQ having number of row errors in bank greater or equal to threshold

  @retval status - TRUE if any DQ has number of row errors greater or equal to threshold

**/
BOOLEAN
GetDqMaskOverThreshold (
  PSYSHOST          Host,
  UINT8             socket,
  UINT8             ch,
  ROW_FAIL_RANGE    NewFail,
  UINT32            Threshold,
  UINT32            DqMaskOver[3]
  );

/**

  Compare most significant channel address bits excluding row bits

  @param CurAddr          - First address to compare
  @param RowAddr          - Second address to compare

  @retval   TRUE for match, otherwise FALSE

**/
BOOLEAN
CurrentAddrMatch (
  ROW_ADDR CurAddr,
  ROW_ADDR RowAddr
  );

/**

  Updates Row failure list with last failure, coalescing failure ranges where possible

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param socket             - Socket Id
  @param ddrChEnMap         - List of channels with rank that was tested
  @param cpgcErrorStatus[MAX_CH] - the failure information per channel
  @param TestStatus[MAX_CH] - Pass/fail status for the test per channel

  @retval status - SUCCESS/FAILURE

**/
UINT32
RemoveRowFailRange (
  PSYSHOST          Host,
  UINT8             Socket,
  UINT8             Ch,
  UINT32            FailIndex
);

/**

  Updates Row failure list with last failure, coalescing failure ranges where possible

  @param[in]  Host                    - Pointer to sysHost, the system Host (root) structure
  @param[in]  socket                  - Socket Id
  @param[in]  ddrChEnMap              - List of channels with rank that was tested
  @param[in]  cpgcErrorStatus[MAX_CH] - the failure information per channel
  @param[in]  rowBits                 - Number of row bits supported by current logical rank
  @param[in]  baseBits                - Number of bank bits in SW loop
  @param[out] TestStatus[MAX_CH]      - Pass/fail status for the test per channel
  @param[in]  Direction               - Sequential address direction MT_ADDR_DIR_UP, MT_ADDR_DIR_DN

  @retval status - SUCCESS/FAILURE

**/
UINT32
UpdateRowFailures (
  IN     PSYSHOST          Host,
  IN     UINT8             socket,
  IN     UINT32            ddrChEnMap,
  IN     CPGC_ERROR_STATUS_MATS cpgcErrorStatus[MAX_CH],
  IN     UINT8             rowBits[MAX_CH],
  IN     UINT8             baseBits,
     OUT UINT8             TestStatus[MAX_CH],
  IN     UINT8             Direction
  );

/**

  Get status whether PPR resource is available

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param socket             - Socket
  @param Ch                 - Channel number
  @param Dimm               - Dimm number
  @param Rank               - Rank number
  @param SubRank            - SubRank number
  @param Nibble             - Nibble number used to identify DRAM
  @param BankGroup          - Bank Group number

  @retval TRUE if PPR resource available; FALSE if not available

**/
BOOLEAN
GetPprResourceAvailable (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT8     Rank,
  UINT8     SubRank,
  UINT8     Nibble,
  UINT8     BankGroup
);

/**

  Set status whether PPR resource is available

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param socket             - Socket
  @param Ch                 - Channel number
  @param Dimm               - Dimm number
  @param Rank               - Rank number
  @param SubRank            - SubRank number
  @param Nibble             - Nibble number used to identify DRAM
  @param BankGroup          - Bank Group number
  @param ResourceAvailable  - TRUE/FALSE whether PPR resource available

  @retval n/a

**/
VOID
SetPprResourceAvailable (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT8     Rank,
  UINT8     SubRank,
  UINT8     Nibble,
  UINT8     BankGroup,
  BOOLEAN   ResourceAvailable
);

/**

  Get a pointer to Row fail range entry at the given list index

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param ch                 - Channel number
  @param FailIndex          - index into the Row fail list

  @retval Pointer to Row fail range entry

**/
ROW_FAIL_RANGE *
GetRowFailRange (
  PSYSHOST  Host,
  UINT8     ch,
  INT32     FailIndex
  );

/**

  Get the number of entries in the Row fail list for given channel

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param ch                 - Channel number

  @retval Number of entries in the Row fail list

**/
UINT32
GetRowFailMax (
  PSYSHOST  Host,
  UINT8     ch
  );

/**

  Set the number of entries in the Row fail list for given channel

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param ch                 - Channel number
  @param FailMax            - Number of entries in Row fail list

  @retval   None

**/
VOID
SetRowFailMax (
  PSYSHOST  Host,
  UINT8     ch,
  UINT32    FailMax
  );

/**

  Get the current index in the Row fail list for given channel

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param ch                 - Channel number

  @retval Current row fail range index

**/
UINT32
GetRowFailIndex (
  PSYSHOST  Host,
  UINT8     ch
  );

/**

  Set the current index in the Row fail list for given channel

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param ch                 - Channel number
  @param FailIndex          - Current row fail range index

  @retval   None

**/
VOID
SetRowFailIndex (
  PSYSHOST  Host,
  UINT8     ch,
  UINT32    FailIndex
  );

/**

  Get the last failure info

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param Ch                 - Channel number
  @param LastFail           - Structure containing info about last test failure

  @retval n/a

**/
VOID
GetAmtLastFail (
  PSYSHOST  Host,
  UINT8     Ch,
  ROW_FAIL_RANGE *LastFail
);

/**

  set the last failure info

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param Ch                 - Channel number
  @param LastFail           - Structure containing info about last test failure

  @retval n/a

**/
VOID
SetAmtLastFail (
  PSYSHOST  Host,
  UINT8     Ch,
  ROW_FAIL_RANGE  LastFail
);

/**

  Get the current retry state

  @param Host               - Pointer to sysHost, the system Host (root) structure

  @retval Current retry state

**/
UINT8
GetAmtRetryState (
  PSYSHOST  Host
  );

/**

  Set the current retry state

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param RetryCount         - AMT retry state

  @retval n/a

**/
VOID
SetAmtRetryState (
  PSYSHOST  Host,
  UINT8     RetryState
);

/**

  Get the Error injection mask for Advanced Memtest pattern

  @param Host               - Pointer to sysHost, the system Host (root) structure

  @retval  Bitmask of DQ lanes to inject error

**/
UINT64_STRUCT
GetAdvMemtestErrInjMask (
  PSYSHOST  Host
  );

/**

  Set the Error injection mask for Advanced Memtest pattern

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param AdvMemtestErrInjMask - Bitmask of DQ lanes to inject error

  @retval   None

**/
VOID
SetAdvMemtestErrInjMask (
  PSYSHOST  Host,
  UINT64_STRUCT AdvMemtestErrInjMask
  );

/**

  Set the Error injection flag for Advanced Memtest operation

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param InjectErrorMATS    - Flag to enable/disable error injection

  @retval   None

**/
VOID
SetInjectErrorMATS (
  PSYSHOST  Host,
  UINT8     Ch,
  UINT8     InjectErrorMATS
  );

/**

  Initializes data structures for the AMT feature

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param Socket             - Socket Id

  @retval   None

**/
VOID
InitAmtDataStructures (
  PSYSHOST  Host,
  UINT8     Socket
  );

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
);

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
);

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
);

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
);

/**

  Updates Row failure list with last failure, coalescing failure ranges where possible

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param socket             - Socket Id
  @param ch                 - Channel of the new failure
  @param CheckMaskError     - Check for previously masked range
  @param NewFail            - New failure information

  @retval status - SUCCESS/FAILURE

**/
UINT32
UpdateRowFailList (
  PSYSHOST          Host,
  UINT8             socket,
  UINT8             ch,
  BOOLEAN           CheckMaskError,
  ROW_FAIL_RANGE    NewFail
);

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
);

/**

  Validation hook to Reset row fail list (useful for testing multiple options)

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param Socket             - Socket Id

  @retval   None

**/
VOID
ResetRowFailureList (
  PSYSHOST  Host,
  UINT8     Socket
);

/**

  Set Advanced Memory Test condition

  @param Host                - Pointer to sysHost
  @param socket              - Socket Id
  @param ddrChEnabled        - List of channels to be tested
  @param testType            - Memory test type
  @param testSize            - Memory test size

  @retval status             - SUCCESS/FAILURE

**/
UINT32
SetAdvMemTestCondition (
  PSYSHOST Host,
  UINT8    socket,
  UINT32   ddrChEnabled,
  UINT8    TestType,
  UINT8    Operation,
  AMT_TEST_CONDITION *PrevAmtTestCond
);

/**

  Pause condition for Advanced Memory Test

  @param Host                - Pointer to sysHost
  @param socket              - Socket Id
  @param ddrChEnabled        - List of channels to be tested
  @param testType            - Memory test type

  @retval status             - SUCCESS/FAILURE

**/
UINT32
AdvMemTestPauseCondition (
  PSYSHOST Host,
  UINT8    Socket,
  UINT32   DdrChEnabled,
  UINT8    TestType
);

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
);

#endif // #ifndef  _MEM_TEST_FUNC_H_
