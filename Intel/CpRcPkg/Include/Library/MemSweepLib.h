/** @file
  Mem1DLibrary.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#ifndef __MEMSWEEPLIBRARY_H__
#define __MEMSWEEPLIBRARY_H__

#include <ReferenceCodeDataTypes.h>

//
// Forward definitions
//
typedef struct _COMMON_1D_PARAMETERS            COMMON_1D_PARAMETERS;
typedef struct _COMMON_1D_PARAMETERS_FOR_2D     COMMON_1D_PARAMETERS_FOR_2D;
typedef struct _COMMON_2D_PARAMETERS            COMMON_2D_PARAMETERS;
//
// Callback definition
//
typedef MRC_STATUS (*Callback1D) (COMMON_1D_PARAMETERS *CommonParameters);

//
// FLAGS definition
// Category starting/limit offsets:
//
typedef enum _FLAGS_CATEGORY_OFFSETS {
  DefaultStartOffset  = 0,      ///< Starts from current register value, the limit is determined by the hardware capability
  StartOnPassingEdges = BIT0,   ///< Based on previous runs it will start Left/Right sweeps on the passing edges.
                                ///< Only takes effect if FLAGS_CATEGORY_RESULTS.ResultsRelative is set.
                                ///< Only takes effect if FLAGS_CATEGORY_RESULTS.UseOffsetAsAbsolute is cleared
                                ///< OffsetLeft and OffsetRight only takes effect if previous passing edges were zero
  UseOffsetAsAbsolute = BIT1,   ///< OffsetLow defines the initial absolute value for left edge, and OffsetHigh for right edge (FLAGS_START_ON_PASSING_EDGES is ignored)
  LimitSearch         = BIT2,   ///< If FLAGS_CATEGORY_OFFSETS.LimitSearch is set sweep range will be:
                                ///< [Original - LimitOffset, Original + LimitOffset]
                                ///< If FLAGS_CATEGORY_OFFSETS.LimitSearch and FLAGS_CATEGORY_OFFSETS.UseOffsetAsAbsolute are set sweep range will be:
                                ///< [OffsetLeft - LimitOffset, OffsetRight + LimitOffset]
  ForceCsrRead        = BIT3,   ///< All offsets are forced to read from CSR, instead of cache values (impact on boot time)
  DisableFastTracking = BIT4,   ///< Sweep will stop tracking the CurrentDelay, and will actually do the read (impact on boot time)
  PerStrobeAccess     = BIT5,   ///< Force the GetSet (...) to be executed for every strobe, default behavior is to use ALL_STROBES whenever is possible
  MaxFlagsCategoryOffsets
} FLAGS_CATEGORY_OFFSETS;
//
// Category result representation:
//
typedef enum _FLAGS_CATEGORY_RESULTS  {
  DefaultResults       = 0,      ///< Results are per nibble (absolute value representation)
  ResultsPerBit        = BIT0,   ///< ResultsPerBit are valid only when this flag is set
  ResultsRelative      = BIT1,   ///< Results are relative compared against original value
  FocusOnTransition    = BIT2,   ///< Define the passing or failing condition based on the result during the ST_AC_FIRST_SAMPLE
                                 ///< The ST_AC_FIRST_SAMPLE sample will be considered a passing condition and the algorithm will look for the transition
                                 ///< Highly recommended to use this flag in combination with FLAGS_CATEGORY_OFFSETS.UseOffsetAsAbsolute and/or OffsetLeft/OffsetRight
  SingleBitTrack       = BIT3,   ///< If this flags is set, only Bit0 of each strobe will be considered to search passing/failing cases
  CmdCtlAllKnobs       = BIT4,   ///< Enables that each knob inside CmdAll or CtlAll are set independently (Heap usage is increased considerably, ONLY for debug)
  ResultsPerSubChannel = BIT5,   ///< If there is any bit within the sub-channel, it will stop the sweep test immediately
                                 ///< For DDR4 assumes a single sub-channel
  ResultsEvenOdd       = BIT6,   ///< Force the results to be even and odd (Only applicable to single knob delay types)
  MaxFlagsCategoryResults
} FLAGS_CATEGORY_RESULTS;
//
// Category FSM:
//
typedef enum _FLAGS_CATEGORY_FSM {
  DefaultFsm           = 0,     ///< Searches both left and right edges
  OneWayOnly           = BIT0,  ///< Once the limit is reached, algorithm does NOT try to search a passing condition in a different direction, it only stops
  LeftSideOnly         = BIT1,  ///< Execute only left side
  RightSideOnly        = BIT2,  ///< Execute only right side
  TrackingSweep        = BIT3 | LeftSideOnly, ///< Enables an special sequence when it starts to search a passing condition in continuous incremental way
                                              ///< Once the passing condition is found the algorithm keeps incrementing and searches for a failing condition
  ContinousMode        = BIT4,                ///< When an strobe get into final state it keeps decrementing or incrementing until all strobes are done
                                              ///< This assumes that first sample at original value always passes
  FanMode              = BIT5 | ContinousMode,///< Fan mode is enabled
  AvoidBoundaryCross   = BIT6 | OneWayOnly,   ///< During margining the boundaries of the original values are no crossed
                                              ///< For Left Side the algorithm valid range will be [Minimum, Original]
                                              ///< For Right Side the algorithm valid range will be [Original, Maximum]
                                              ///< Does not take effect if FLAGS_CATEGORY_FSM.TrackingSweep is set
  TransitionFineFsm    = BIT7,  ///< FSM uses StepSize as coarse step, but once the transitions/limits
                                ///< are found it starts using a fine step of SWEEP_FINE_STEP_SIZE,
                                ///< not valid when FLAGS_CATEGORY_RESULTS.ResultsPerSubChannel is SET

  MaxFlagsCategoryFsm
} FLAGS_CATEGORY_FSM;

typedef enum {
  EvenChunkSide = 0,
  OddChunkSide  = 1,
  MaxChunkSides
} CHUNK_SIDES;

//
// QCLK defines in number of ticks
//
#define ONE_DCLK        128
#define ONE_QCLK        (ONE_DCLK / 2)
#define HALF_QCLK       (ONE_QCLK / 2)
#define QUARTER_QCLK    (ONE_QCLK / 4)
#define EIGHTH_QCLK     (ONE_QCLK / 8)

typedef struct _RESULTS_1D {
  //
  // Results per bit are only valid when flag ResultsPerBit is set
  //
  struct baseMargin (*ResultsPerStrobe)[MAX_CH][MAX_STROBE];
  struct baseMargin (*ResultsPerBit)[MAX_CH][MAX_STROBE][BITS_PER_NIBBLE]; /// OPTIONAL this is only required for Per Bit Margining
} RESULTS_1D;

//
// States naming convention.
// ST_[Edges]_AC_[Action To take]_SE_[Search For]
//
// ACTION is abbreviated as AC
// SEARCH is abbreviated as SE
//
// Edges:
// LEFT EDGE is abbreviated as LE
// RIGHT EDGE is abbreviated as RE
//
// Action to take:
// DECREMENT is abbreviated as DEC
// INCREMENT is abbreviated as INC
// RESTORED ORIGINAL is abbreviated as RSO
//
// Search for:
// PASS or FAIL
//
typedef enum _STATES_1D {
  ST_AC_INIT                    = 0,   //< Initial state all pre-processing
  ST_AC_INIT_PLUS               = 1,
  ST_AC_INIT_MINUS              = 2,
  ST_AC_FIRST_SAMPLE            = 10,   //< Take first sample result

  ST_LE_AC_INC_SE_PASS          = 20,   //< Left Edge INCrement until it finds PASS condition
  ST_LE_AC_INC_SE_PASS_TS       = 21,   //< Left Edge INCrement until it finds PASS condition during Tracking Sweep Mode
  ST_LE_AC_RSO_AC_DEC_SE_PASS   = 30,   //< Left Edge ReStored Original value and DECrement until it finds PASS condition
  ST_LE_FN_AC_DEC_SE_FAIL       = 39,   //< Left Edge DECrement until it finds FAIL condition (using fine step of 'SWEEP_FINE_STEP_SIZE')
  ST_LE_AC_DEC_SE_FAIL          = 40,   //< Left Edge DECrement until it finds FAIL condition

  ST_RE_AC_DEC_SE_PASS          = 50,   //< Right Edge DECrement until it finds PASS condition
  ST_RE_AC_RSO_AC_INC_SE_PASS   = 60,   //< Right Edge ReStored Original value and INCrement until it finds PASS condition
  ST_RE_FN_AC_INC_SE_FAIL       = 69,   //< Right Edge INCrement until it finds FAIL condition (using fine step of 'SWEEP_FINE_STEP_SIZE')
  ST_RE_AC_INC_SE_FAIL          = 70,   //< Right Edge INCrement until it finds FAIL condition
  ST_RE_AC_INC_SE_FAIL_TS       = 80,   //< Right Edge INCrement until it finds FAIL condition during Tracking Sweep Mode

  ST_END                        = 90,
  ST_LE_END                     = 91,
  ST_RE_END                     = 92,
  ST_XX_END                     = 93,   //< Use to indicate any END state (ST_END, ST_LE_END, ST_RE_END)
  MAX_1D_STATES
} STATES_1D;

typedef enum _EDGES_1D {
  LEFT_EDGE     = 0,
  RIGHT_EDGE    = 1,
  MAX_EDGES
} EDGES_1D;

typedef enum _RESULTS_1D_DEFINITION {
  PASS_CONDITION        = 0,
  FAIL_CONDITION        = 1,
  MAX_RESULTS_1D_CONDITION
} RESULTS_1D_DEFINITION;

typedef struct _RESULTS_1D_CONDITION {
  UINT16  Previous      :   1;
  UINT16  Current       :   1;
  UINT16  First         :   1;
  UINT16  DitherCounter :   13;
} RESULTS_1D_CONDITION;

typedef enum _TIME_INSTRUMENTATION_1D {
  PrintL0,
  ExecuteStateActionL0,
  ExecuteTestL0,
  ResultProcessingL0,
  ResultsConditionByStateL0,
  TrackingResults1DL0,
  OverrideResultConditionByGroupL0,
  GetNextStateL0,
  RestoreOriginalL0,
  ErrorHandlerL0,
  Post1DSweepL0,
  MAX_TIME_INSTRUMENTATION_1D
} TIME_INSTRUMENTATION_1D;

struct _COMMON_1D_PARAMETERS {
  //
  // Global inputs (usually remain unchanged)
  //
  DDR_TECHNOLOGY_TYPE DdrType;
  UINT8       Socket;
  UINT32      ChannelMask;
  UINT32      RankMask;
  UINT8       SubChannel;
  UINT8       Dimm[MAX_CH];
  UINT8       Rank[MAX_CH];
  UINT8       SubRank[MAX_CH];
  MRC_LT      Level;
  MRC_GT      Group;
  GSM_CSN     Signal;
  UINT8       StepSize;
  UINT8       DitherValue;
  INT16       OffsetLeft;
  INT16       OffsetRight;
  INT16       LimitOffset;
  FLAGS_CATEGORY_OFFSETS  FlagsOffsets;
  FLAGS_CATEGORY_RESULTS  FlagsResults;
  FLAGS_CATEGORY_FSM      FlagsFsm;
  //
  // Callbacks
  //
  Callback1D  PreSweep1D;
  Callback1D  SetupTest;
  RESULTS_1D  *Results;
  //
  // Indentation for printing information
  //
  UINT8       IndentationLevel;
  //
  // Local inputs (some values may change based on global inputs)
  //
  PSYSHOST    Host;
  SYS_SETUP   *Setup;
  UINT8       KnobsPerGroup;
  UINT8       MaxStrobeAvailable;   ///< Max strobe available within the sub-channel (based on DDR technology)
  UINT8       NextNibbleOffset;     ///< Next contiguous logical nibble within the byte
  UINT8       BitsToTrack;          ///< Number of bits to track (1 or 4)
  UINT8       ReadMode;             ///< Read from CSR or Cache (by default), use FLAGS_CATEGORY_OFFSETS.ForceCsrRead to force CSR
  UINT8       MinSubChannelIndex;
  UINT8       MaxSubChannelIndex;
  //
  // Cached parameters for boot time improvements
  //
  UINT8       MaxChDdr;
  BOOLEAN     ChannelEnabledLocal[MAX_CH];
  BOOLEAN     IsEccEnabled;
  UINT32      DebugLevel;
  //
  // Resource allocation for delays tracking
  //
  INT16       (*OriginalDelays)[MAX_CH][MAX_STROBE];
  INT16       (*CurrentDelays)[MAX_CH][MAX_STROBE];
  INT16       (*DelayOffset)[MAX_CH][MAX_STROBE];
  //
  // Resource allocation for Max bit delta per nibble
  //
  INT16       (*MinMaxDeltaBitValue)[MAX_CH][MAX_STROBE];
  //
  // Next variables are updated dynamically based on the FSM execution
  //
  EDGES_1D    CurrentEdge;
  STATES_1D   CurrentState[MAX_CH][MAX_STROBE];
  //
  // Behavior changes if based in ResultsPerBit and search current state (xxx_SE_PASS or xxx_SE_FAIL)
  // If results are being tracked by strobe basis (ResultsPerBit = 0):
  // PASS_CONDITION is gotten when ALL bits within the strobe met the passing criteria during xxx_SE_PASS state across ALL Knobs
  // FAIL_CONDITION is gotten when ANY bit  within the strobe met the failing criteria during xxx_SE_FAIL state across ALL Knobs
  // If results are being tracked by bit basis (ResultsPerBit = 1):
  // PASS_CONDITION is gotten when ALL bits within the strobe met the passing criteria during xxx_SE_PASS state across ALL Knobs
  // FAIL_CONDITION is gotten when ALL bits within the strobe met the failing criteria during xxx_SE_FAIL state across ALL Knobs
  //
  RESULTS_1D_CONDITION (*ResultCondition)[MAX_CH][MAX_STROBE][BITS_PER_NIBBLE];
  UINT32      StrobeLimit[MAX_CH];
  //
  // Time instrumentation
  //
  UINT32  Performance1DProfile[MAX_TIME_INSTRUMENTATION_1D];
  UINT64  StartCnt;
  UINT64  StopCnt;

};


struct _COMMON_1D_PARAMETERS_FOR_2D {
  //
  // Always first line
  //
  COMMON_1D_PARAMETERS  CommonParametersH;
  //
  // Private elements to only be used during 2D sweeps
  //
  UINT8                 IndentationLevel;
  COMMON_2D_PARAMETERS  *CommonParameters2D;
};

struct _COMMON_2D_PARAMETERS {
  //
  // Always first line
  //
  COMMON_1D_PARAMETERS  CommonParametersV;
  //
  // Vertical sweep parameters (private information)
  //
  INT16                 MaxVerticalEntries;   ///< 2 * Number Of Samples per side + 1
  struct strobeMargin   *StrobeResults;       ///< Pointer to an array that holds a valid entry for each vertical sampling point
  struct bitMargin      *BitResults;          ///< Pointer to an array that holds a valid entry for each vertical sampling point
  struct rankMargin     *RankResults;         ///< Pointer to an array that holds a valid entry for each vertical sampling point
  //
  // Horizontal sweep parameters (private information)
  //
  COMMON_1D_PARAMETERS_FOR_2D  CommonParametersH;
  MRC_MST               MemSsTypeH;
  UINT8                 SocketH;
  UINT32                ChannelMaskH;
  UINT32                RankMaskH;
  UINT8                 SubChannelH;
  MRC_RT                *DimmRankH;
  MRC_LT                LevelH;
  MRC_GT                GroupH;
  GSM_CSN               SignalH;
  UINT8                 StepSizeH;
  UINT8                 DitherValueH;
  INT16                 OffsetLeftH;
  INT16                 OffsetRightH;
  INT16                 LimitOffsetH;
  FLAGS_CATEGORY_OFFSETS  FlagsOffsetsH;
  FLAGS_CATEGORY_RESULTS  FlagsResultsH;
  FLAGS_CATEGORY_FSM      FlagsFsmH;

  Callback1D            PreSweep1DH;
  Callback1D            SetupTestH;
  Callback1D            ExecuteTestH;
  Callback1D            ErrorHandlerH;
  Callback1D            PostSweep1DH;
  Callback1D            ResultProcessingH;
};


/**

  Get Adv Dq margins

  @param[in]      Socket            Current socket under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test

  @param[in]      FlagsResults      Modify the format for the output results
                                    ResultsPerBit:
                                    It will ill collect the result on bit basis
                                    ResultsRelative:
                                    By default results are the delay values for Left and Right
                                    If ResultsRelative is set results are with respect to original delay value
                                    FocusOnTransition:
                                    Defines the passing or failing condition based on the result during the ST_AC_FIRST_SAMPLE
                                    The ST_AC_FIRST_SAMPLE sample will be always considered a failing condition and the algorithm will look for the transition
                                    Highly recommended to use this flag in combination with FLAGS_CATEGORY_OFFSETS.UseOffsetAsAbsolute and/or OffsetLeft/OffsetRight
                                    SingleBitTrack:
                                    Only tracks and display a single bit
                                    CmdCtlAllKnobs:
                                    Only applicable to CmdAll or CtlAll, this forces the training to be done on each Cmd/Ctl knob
                                    ResultsPerSubChannel:
                                    If there is any bit within the sub-channel, it will stop the sweep test immediately
                                    For DDR4 assumes a single sub-channel
                                    ResultsEvenOdd:
                                    Force the results to be tracked separately for even and odd (Only applicable to single knob delay types)


  @param[out]     StrobeResults     Pointer to return the results at strobe level
  @param[out]     BitResults        Pointer to return the results at bit level
  @param[out]     RankResults       Pointer to return the results at rank level

  @param[out]     StrobeResultsEvenOdd  Pointer to return the results at strobe level for EVEN/ODD
  @param[out]     BitResultsEvenOdd     Pointer to return the results at bit level for EVEN/ODD
  @param[out]     RankResultsEvenOdd    Pointer to return the results at rank level for EVEN/ODD

  @retval         MRC_STATUS_SUCCESS       Context was initialized without errors
  @retval         MRC_STATUS_FAILURE       Resources to create context failed to be allocated

**/
MRC_STATUS
EFIAPI
GetAdvDataMargins (
  IN        UINT8       Socket,
  IN        MRC_LT      Level,
  IN        MRC_GT      Group,
  IN        FLAGS_CATEGORY_RESULTS  FlagsResults,

      OUT   struct strobeMargin *StrobeResults, OPTIONAL
      OUT   struct bitMargin    *BitResults,    OPTIONAL
      OUT   struct rankMargin   *RankResults,   OPTIONAL

      OUT   struct strobeMargin StrobeResultsEvenOdd[MaxChunkSides], OPTIONAL
      OUT   struct bitMargin    BitResultsEvenOdd[MaxChunkSides],    OPTIONAL
      OUT   struct rankMargin   RankResultsEvenOdd[MaxChunkSides]    OPTIONAL
  );

/**

  Get Adv Dq margins during DFE training algorithms

  @param[in]      Socket            Current socket under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test

  @param[out]     StrobeResults     Pointer to return the results at strobe level
  @param[out]     BitResults        Pointer to return the results at bit level
  @param[out]     RankResults       Pointer to return the results at rank level

  @retval         MRC_STATUS_SUCCESS       Context was initialized without errors
  @retval         MRC_STATUS_FAILURE       Resources to create context failed to be allocated

**/
MRC_STATUS
EFIAPI
GetDfeDataMargins (
  IN        UINT8       Socket,
  IN        MRC_LT      Level,
  IN        MRC_GT      Group,

      OUT   struct strobeMargin *StrobeResults, OPTIONAL
      OUT   struct bitMargin    *BitResults,    OPTIONAL
      OUT   struct rankMargin   *RankResults
  );

/**

  Worker function to get margins on the specified rank for each channel

  @param[in,out]  CommonParameters  Pointer to the context information
  @param[in]      MemSsType         Current Memory Subsystem Type (DdrLevel, LrBuffLevel, HbmLevel, etc)
  @param[in]      Socket            Current socket under test (0-based)
  @param[in]      ChannelMask       One-hot encoding to mask a channel (0: executes if enabled/populated, 1: skip/masked)
  @param[in]      RankMask          One-hot encoding to mask a rank (0: executes if enabled/populated, 1: skip/masked)
  @param[in]      SubChannel        Subchannel under test
                                    DDR4: (Not applicable)
                                    DDR5: 0, 1 or ALL_SUBCH
  @param[in]      DimmRank          Structure to specify current dimm and rank under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test
  @param[in]      Signal            Specifies the GSM_CSN Signal (this paramater is only required for few groups)
  @param[in]      StepSize          Specifies the StepSize to do the margining
                                    When FLAGS_CATEGORY_FSM.TransitionFineFsm is set this value is used as the Coarse step size.
  @param[in]      DitherValue       The dither value specifies the number of passing consecutive sample, before consider a pass condition
  @param[in]      OffsetLeft        Used to determine the start point on the left side (Original + OffsetLeft)
                                    If Flag FLAGS_CATEGORY_OFFSETS.UseOffsetAsAbsolute is set, OffsetLeft is as an absolute start value for left side
  @param[in]      OffsetRight       Used to determine the start point on the right side (Original + OffsetRight)
                                    If Flag FLAGS_CATEGORY_OFFSETS.UseOffsetAsAbsolute is set, OffsetRight is as an absolute start value for right side
  @param[in]      LimitOffset       Limits the search to an specific range. This is only applicable when FLAGS_CATEGORY_OFFSETS.LimitSearch is set
                                    If FLAGS_CATEGORY_OFFSETS.LimitSearch is set sweep range will be:
                                    [Original - LimitOffset, Original + LimitOffset]
                                    If FLAGS_CATEGORY_OFFSETS.LimitSearch and FLAGS_CATEGORY_OFFSETS.UseOffsetAsAbsolute are set sweep range will be:
                                    [OffsetLeft - LimitOffset, OffsetRight + LimitOffset]

  @param[in]      FlagsOffsets      Modify the start/limit offset
                                    StartOnPassingEdges:
                                    Start point on the left side will be Original + LeftMargin
                                    Start point on the right side will be Original + RightMargin
                                    Only takes effect if FLAGS_CATEGORY_RESULTS.ResultsRelative is set.
                                    Only takes effect if FLAGS_CATEGORY_RESULTS.UseOffsetAsAbsolute is cleared
                                    OffsetLeft and OffsetRight only takes effect if previous passing edges were zero
                                    UseOffsetAsAbsolute:
                                    Start point on the left side will be OffsetLeft
                                    Start point on the right side will be OffsetRight
                                    LimitSearch:
                                    See LimitOffset description
                                    ForceCsrRead:
                                    Force all the read to be from CSR
                                    DisableFastTracking:
                                    Fast tracking is disable, so CurrentDelay need to be read from CACHE or CSR (IF ForceCsrRead is set)
                                    PerStrobeAccess:
                                    Set the delay for each strobe independently, ALL_STROBES is discouraged

  @param[in]      FlagsResults      Modify the format for the output results
                                    ResultsPerBit:
                                    It will ill collect the result on bit basis
                                    ResultsRelative:
                                    By default results are the delay values for Left and Right
                                    If ResultsRelative is set results are with respect to original delay value
                                    FocusOnTransition:
                                    Defines the passing or failing condition based on the result during the ST_AC_FIRST_SAMPLE
                                    The ST_AC_FIRST_SAMPLE sample will be always considered a failing condition and the algorithm will look for the transition
                                    Highly recommended to use this flag in combination with FLAGS_CATEGORY_OFFSETS.UseOffsetAsAbsolute and/or OffsetLeft/OffsetRight
                                    SingleBitTrack:
                                    Only tracks and display a single bit
                                    CmdCtlAllKnobs:
                                    Only applicable to CmdAll or CtlAll, this forces the training to be done on each Cmd/Ctl knob
                                    ResultsPerSubChannel:
                                    If there is any bit within the sub-channel, it will stop the sweep test immediately
                                    For DDR4 assumes a single sub-channel
                                    ResultsEvenOdd:
                                    Force the results to be tracked separately for even and odd (Only applicable to single knob delay types)

  @param[in]      FlagsFsm          Modify the FSM behavior
                                    OneWayOnly:
                                    Once the limit is reached, algorithm does NOT try to search a passing condition in a different direction, it only stops
                                    LeftSideOnly:
                                    Only sweeps for left side
                                    RightSideOnly:
                                    Only sweeps for right side
                                    TrackingSweep:
                                    Enables an special sequence when it starts to search a passing condition in continuous incremental way
                                    Once the passing condition is found the algorithm keeps incrementing and searches for a failing condition
                                    ContinousMode:
                                    When an strobe get into final state it keeps decrementing or incrementing until all strobes are done
                                    This assumes that first sample at original value always passes
                                    AvoidBoundaryCross:
                                    During margining the boundaries of the original values are no crossed
                                    For Left Side the algorithm valid range will be [Minimum, Original]
                                    For Right Side the algorithm valid range will be [Original, Maximum]
                                    Does not take effect if FLAGS_CATEGORY_FSM.TrackingSweep is set
                                    TransitionFineFsm:
                                    This flag is used to find the transitions Pass->Fail or Fail->Pass with a coarse step size of 'StepSize'
                                    Once the transitions are found then an step size of 'SWEEP_FINE_STEP_SIZE' is used to fine more accurate results
                                    Not applicable when FLAGS_CATEGORY_RESULTS.ResultsPerSubChannel is set

  @param[in]      PreSweep1D        Callback to be executed before any sweep is done
  @param[in]      SetupTest         Callback to configure the engine to do the test
  @param[in]      ExecuteTest       Callback to configure and start the test
  @param[in]      ErrorHandler      Callback to be executed after each sample test completes (used to handle special errors)
  @param[in]      PostSweep1D       Callback to be executed when all sides are done
  @param[in]      ResultProcessing  Callback to interpret the HW results after each sample test completes

  @param[out]     StrobeResults     Pointer to return the results at strobe level
  @param[out]     BitResults        Pointer to return the results at bit level
  @param[out]     RankResults       Pointer to return the results at rank level

  @param[out]     StrobeResultsEvenOdd  Pointer to return the results at strobe level for EVEN/ODD
  @param[out]     BitResultsEvenOdd     Pointer to return the results at bit level for EVEN/ODD
  @param[out]     RankResultsEvenOdd    Pointer to return the results at rank level for EVEN/ODD

  @retval         MRC_STATUS_SUCCESS       Context was initialized without errors
**/
MRC_STATUS
EFIAPI
ProcessWorker1D (
  IN  OUT   COMMON_1D_PARAMETERS  *CommonParameters,
  IN        MRC_MST     MemSsType,
  IN        UINT8       Socket,
  IN        UINT32      ChannelMask,
  IN        UINT32      RankMask,
  IN        UINT8       SubChannel,
  IN        MRC_RT      DimmRank[MAX_CH],
  IN        MRC_LT      Level,
  IN        MRC_GT      Group,
  IN        GSM_CSN     Signal,           OPTIONAL
  IN        UINT8       StepSize,
  IN        UINT8       DitherValue,
  IN        INT16       OffsetLeft,
  IN        INT16       OffsetRight,
  IN        INT16       LimitOffset,
  IN        FLAGS_CATEGORY_OFFSETS  FlagsOffsets,
  IN        FLAGS_CATEGORY_RESULTS  FlagsResults,
  IN        FLAGS_CATEGORY_FSM      FlagsFsm,

  IN        Callback1D  PreSweep1D,           OPTIONAL
  IN        Callback1D  SetupTest,            OPTIONAL
  IN        Callback1D  ExecuteTest,          OPTIONAL
  IN        Callback1D  ErrorHandler,         OPTIONAL
  IN        Callback1D  PostSweep1D,          OPTIONAL
  IN        Callback1D  ResultProcessing,

      OUT   struct strobeMargin *StrobeResults, OPTIONAL
      OUT   struct bitMargin    *BitResults,    OPTIONAL
      OUT   struct rankMargin   *RankResults,   OPTIONAL

      OUT   struct strobeMargin StrobeResultsEvenOdd[MaxChunkSides], OPTIONAL
      OUT   struct bitMargin    BitResultsEvenOdd[MaxChunkSides],    OPTIONAL
      OUT   struct rankMargin   RankResultsEvenOdd[MaxChunkSides]    OPTIONAL

  );

/**

  Worker function to get margins on all ranks for each channel

  @param[in,out]  CommonParameters  Pointer to the context information
  @param[in]      MemSsType         Current Memory Subsystem Type (DdrLevel, LrBuffLevel, HbmLevel, etc)
  @param[in]      Socket            Current socket under test (0-based)
  @param[in]      ChannelMask       One-hot encoding to mask a channel (0: executes if enabled/populated, 1: skip/masked)
  @param[in]      RankMask          One-hot encoding to mask a rank (0: executes if enabled/populated, 1: skip/masked)
  @param[in]      SubChannel        Subchannel under test
                                    DDR4: (Not applicable)
                                    DDR5: 0, 1 or ALL_SUBCH
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test
  @param[in]      Signal            Specifies the GSM_CSN Signal (this paramater is only required for few groups)
  @param[in]      StepSize          Specifies the StepSize to do the margining
                                    When FLAGS_CATEGORY_FSM.TransitionFineFsm is set this value is used as the Coarse step size.
  @param[in]      DitherValue       The dither value specifies the number of passing consecutive sample, before consider a pass condition
  @param[in]      OffsetLeft        Used to determine the start point on the left side (Original + OffsetLeft)
                                    If Flag FLAGS_CATEGORY_OFFSETS.UseOffsetAsAbsolute is set, OffsetLeft is as an absolute start value for left side
  @param[in]      OffsetRight       Used to determine the start point on the right side (Original + OffsetRight)
                                    If Flag FLAGS_CATEGORY_OFFSETS.UseOffsetAsAbsolute is set, OffsetRight is as an absolute start value for right side
  @param[in]      LimitOffset       Limits the search to an specific range. This is only applicable when FLAGS_CATEGORY_OFFSETS.LimitSearch is set
                                    If FLAGS_CATEGORY_OFFSETS.LimitSearch is set sweep range will be:
                                    [Original - LimitOffset, Original + LimitOffset]
                                    If FLAGS_CATEGORY_OFFSETS.LimitSearch and FLAGS_CATEGORY_OFFSETS.UseOffsetAsAbsolute are set sweep range will be:
                                    [OffsetLeft - LimitOffset, OffsetRight + LimitOffset]

  @param[in]      FlagsOffsets      Modify the start/limit offset
                                    StartOnPassingEdges:
                                    Start point on the left side will be Original + LeftMargin
                                    Start point on the right side will be Original + RightMargin
                                    Only takes effect if FLAGS_CATEGORY_RESULTS.ResultsRelative is set.
                                    Only takes effect if FLAGS_CATEGORY_RESULTS.UseOffsetAsAbsolute is cleared
                                    OffsetLeft and OffsetRight only takes effect if previous passing edges were zero
                                    UseOffsetAsAbsolute:
                                    Start point on the left side will be OffsetLeft
                                    Start point on the right side will be OffsetRight
                                    LimitSearch:
                                    See LimitOffset description
                                    ForceCsrRead:
                                    Force all the read to be from CSR
                                    DisableFastTracking:
                                    Fast tracking is disable, so CurrentDelay need to be read from CACHE or CSR (IF ForceCsrRead is set)
                                    PerStrobeAccess:
                                    Set the delay for each strobe independently, ALL_STROBES is discouraged

  @param[in]      FlagsResults      Modify the format for the output results
                                    ResultsPerBit:
                                    It will ill collect the result on bit basis
                                    ResultsRelative:
                                    By default results are the delay values for Left and Right
                                    If ResultsRelative is set results are with respect to original delay value
                                    FocusOnTransition:
                                    Defines the passing or failing condition based on the result during the ST_AC_FIRST_SAMPLE
                                    The ST_AC_FIRST_SAMPLE sample will be always considered a failing condition and the algorithm will look for the transition
                                    Highly recommended to use this flag in combination with FLAGS_CATEGORY_OFFSETS.UseOffsetAsAbsolute and/or OffsetLeft/OffsetRight
                                    SingleBitTrack:
                                    Only tracks and display a single bit
                                    CmdCtlAllKnobs:
                                    Only applicable to CmdAll or CtlAll, this forces the training to be done on each Cmd/Ctl knob
                                    ResultsPerSubChannel:
                                    If there is any bit within the sub-channel, it will stop the sweep test immediately
                                    For DDR4 assumes a single sub-channel
                                    ResultsEvenOdd:
                                    Force the results to be tracked separately for even and odd (Only applicable to single knob delay types)

  @param[in]      FlagsFsm          Modify the FSM behavior
                                    OneWayOnly:
                                    Once the limit is reached, algorithm does NOT try to search a passing condition in a different direction, it only stops
                                    LeftSideOnly:
                                    Only sweeps for left side
                                    RightSideOnly:
                                    Only sweeps for right side
                                    TrackingSweep:
                                    Enables an special sequence when it starts to search a passing condition in continuous incremental way
                                    Once the passing condition is found the algorithm keeps incrementing and searches for a failing condition
                                    ContinousMode:
                                    When an strobe get into final state it keeps decrementing or incrementing until all strobes are done
                                    This assumes that first sample at original value always passes
                                    AvoidBoundaryCross:
                                    During margining the boundaries of the original values are no crossed
                                    For Left Side the algorithm valid range will be [Minimum, Original]
                                    For Right Side the algorithm valid range will be [Original, Maximum]
                                    Does not take effect if FLAGS_CATEGORY_FSM.TrackingSweep is set
                                    TransitionFineFsm:
                                    This flag is used to find the transitions Pass->Fail or Fail->Pass with a coarse step size of 'StepSize'
                                    Once the transitions are found then an step size of 'SWEEP_FINE_STEP_SIZE' is used to fine more accurate results
                                    Not applicable when FLAGS_CATEGORY_RESULTS.ResultsPerSubChannel is set

  @param[in]      PreSweep1D        Callback to be executed before any sweep is done
  @param[in]      SetupTest         Callback to configure the engine to do the test
  @param[in]      ExecuteTest       Callback to configure and start the test
  @param[in]      ErrorHandler      Callback to be executed after each sample test completes (used to handle special errors)
  @param[in]      PostSweep1D       Callback to be executed when all sides are done
  @param[in]      ResultProcessing  Callback to interpret the HW results after each sample test completes

  @param[out]     StrobeResults     Pointer to return the results at strobe level
  @param[out]     BitResults        Pointer to return the results at bit level
  @param[out]     RankResults       Pointer to return the results at rank level

  @param[out]     StrobeResultsEvenOdd  Pointer to return the results at strobe level for EVEN/ODD
  @param[out]     BitResultsEvenOdd     Pointer to return the results at bit level for EVEN/ODD
  @param[out]     RankResultsEvenOdd    Pointer to return the results at rank level for EVEN/ODD

  @retval         MRC_STATUS_SUCCESS       Context was initialized without errors
**/
MRC_STATUS
EFIAPI
ProcessWorkerAllRanks1D (
  IN  OUT   COMMON_1D_PARAMETERS  *CommonParameters,
  IN        MRC_MST     MemSsType,
  IN        UINT8       Socket,
  IN        UINT32      ChannelMask,
  IN        UINT32      RankMask,
  IN        UINT8       SubChannel,
  IN        MRC_LT      Level,
  IN        MRC_GT      Group,
  IN        GSM_CSN     Signal,           OPTIONAL
  IN        UINT8       StepSize,
  IN        UINT8       DitherValue,
  IN        INT16       OffsetLeft,
  IN        INT16       OffsetRight,
  IN        INT16       LimitOffset,
  IN        FLAGS_CATEGORY_OFFSETS  FlagsOffsets,
  IN        FLAGS_CATEGORY_RESULTS  FlagsResults,
  IN        FLAGS_CATEGORY_FSM      FlagsFsm,

  IN        Callback1D  PreSweep1D,           OPTIONAL
  IN        Callback1D  SetupTest,            OPTIONAL
  IN        Callback1D  ExecuteTest,          OPTIONAL
  IN        Callback1D  ErrorHandler,         OPTIONAL
  IN        Callback1D  PostSweep1D,          OPTIONAL
  IN        Callback1D  ResultProcessing,

      OUT   struct strobeMargin *StrobeResults, OPTIONAL
      OUT   struct bitMargin    *BitResults,    OPTIONAL
      OUT   struct rankMargin   *RankResults,   OPTIONAL

      OUT   struct strobeMargin StrobeResultsEvenOdd[MaxChunkSides], OPTIONAL
      OUT   struct bitMargin    BitResultsEvenOdd[MaxChunkSides],    OPTIONAL
      OUT   struct rankMargin   RankResultsEvenOdd[MaxChunkSides]    OPTIONAL
  );


/**

  Create the context information that will be used to execute the 1D sweeps
  WARNING: This function must be executed prior any other 1D sweep function
  is executed.

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      MemSsType         Current Memory Subsystem Type (DdrLevel, LrBuffLevel, HbmLevel, etc)
  @param[in]      Socket            Current socket under test (0-based)
  @param[in]      ChannelMask       One-hot encoding to mask a channel (0: executes if enabled/populated, 1: skip/masked)
  @param[in]      RankMask          One-hot encoding to mask a rank (0: executes if enabled/populated, 1: skip/masked)
  @param[in]      SubChannel        Subchannel under test
                                    DDR4: (Not applicable)
                                    DDR5: 0, 1 or ALL_SUBCH
  @param[in]      DimmRank          Structure to specify current dimm and rank under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test
  @param[in]      Signal            Specifies the GSM_CSN Signal (this paramater is only required for few groups)
  @param[in]      StepSize          Specifies the StepSize to do the margining
                                    When FLAGS_CATEGORY_FSM.TransitionFineFsm is set this value is used as the Coarse step size.
  @param[in]      DitherValue       The dither value specifies the number of passing consecutive sample, before consider a pass condition
  @param[in]      OffsetLeft        Used to determine the start point on the left side (Original + OffsetLeft)
                                    If Flag FLAGS_CATEGORY_OFFSETS.UseOffsetAsAbsolute is set, OffsetLeft is as an absolute start value for left side
  @param[in]      OffsetRight       Used to determine the start point on the right side (Original + OffsetRight)
                                    If Flag FLAGS_CATEGORY_OFFSETS.UseOffsetAsAbsolute is set, OffsetRight is as an absolute start value for right side
  @param[in]      LimitOffset       Limits the search to an specific range. This is only applicable when FLAGS_CATEGORY_OFFSETS.LimitSearch is set
                                    If FLAGS_CATEGORY_OFFSETS.LimitSearch is set sweep range will be:
                                    [Original - LimitOffset, Original + LimitOffset]
                                    If FLAGS_CATEGORY_OFFSETS.LimitSearch and FLAGS_CATEGORY_OFFSETS.UseOffsetAsAbsolute are set sweep range will be:
                                    [OffsetLeft - LimitOffset, OffsetRight + LimitOffset]

  @param[in]      FlagsOffsets      Modify the start/limit offset
                                    StartOnPassingEdges:
                                    Start point on the left side will be Original + LeftMargin
                                    Start point on the right side will be Original + RightMargin
                                    Only takes effect if FLAGS_CATEGORY_RESULTS.ResultsRelative is set.
                                    Only takes effect if FLAGS_CATEGORY_RESULTS.UseOffsetAsAbsolute is cleared
                                    OffsetLeft and OffsetRight only takes effect if previous passing edges were zero
                                    UseOffsetAsAbsolute:
                                    Start point on the left side will be OffsetLeft
                                    Start point on the right side will be OffsetRight
                                    LimitSearch:
                                    See LimitOffset description
                                    ForceCsrRead:
                                    Force all the read to be from CSR
                                    DisableFastTracking:
                                    Fast tracking is disable, so CurrentDelay need to be read from CACHE or CSR (IF ForceCsrRead is set)
                                    PerStrobeAccess:
                                    Set the delay for each strobe independently, ALL_STROBES is discouraged

  @param[in]      FlagsResults      Modify the format for the output results
                                    ResultsPerBit:
                                    It will ill collect the result on bit basis
                                    ResultsRelative:
                                    By default results are the delay values for Left and Right
                                    If ResultsRelative is set results are with respect to original delay value
                                    FocusOnTransition:
                                    Defines the passing or failing condition based on the result during the ST_AC_FIRST_SAMPLE
                                    The ST_AC_FIRST_SAMPLE sample will be always considered a failing condition and the algorithm will look for the transition
                                    Highly recommended to use this flag in combination with FLAGS_CATEGORY_OFFSETS.UseOffsetAsAbsolute and/or OffsetLeft/OffsetRight
                                    SingleBitTrack:
                                    Only tracks and display a single bit
                                    CmdCtlAllKnobs:
                                    Only applicable to CmdAll or CtlAll, this forces the training to be done on each Cmd/Ctl knob
                                    ResultsPerSubChannel:
                                    If there is any bit within the sub-channel, it will stop the sweep test immediately
                                    For DDR4 assumes a single sub-channel
                                    ResultsEvenOdd:
                                    Force the results to be tracked separately for even and odd (Only applicable to single knob delay types)

  @param[in]      FlagsFsm          Modify the FSM behavior
                                    OneWayOnly:
                                    Once the limit is reached, algorithm does NOT try to search a passing condition in a different direction, it only stops
                                    LeftSideOnly:
                                    Only sweeps for left side
                                    RightSideOnly:
                                    Only sweeps for right side
                                    TrackingSweep:
                                    Enables an special sequence when it starts to search a passing condition in continuous incremental way
                                    Once the passing condition is found the algorithm keeps incrementing and searches for a failing condition
                                    ContinousMode:
                                    When an strobe get into final state it keeps decrementing or incrementing until all strobes are done
                                    This assumes that first sample at original value always passes
                                    AvoidBoundaryCross:
                                    During margining the boundaries of the original values are no crossed
                                    For Left Side the algorithm valid range will be [Minimum, Original]
                                    For Right Side the algorithm valid range will be [Original, Maximum]
                                    Does not take effect if FLAGS_CATEGORY_FSM.TrackingSweep is set
                                    TransitionFineFsm:
                                    This flag is used to find the transitions Pass->Fail or Fail->Pass with a coarse step size of 'StepSize'
                                    Once the transitions are found then an step size of 'SWEEP_FINE_STEP_SIZE' is used to fine more accurate results
                                    Not applicable when FLAGS_CATEGORY_RESULTS.ResultsPerSubChannel is set

  @retval         MRC_STATUS_SUCCESS       Context was initialized without errors
**/
MRC_STATUS
EFIAPI
CreateContextParameters (
  IN  OUT   COMMON_1D_PARAMETERS  *CommonParameters,
  IN        MRC_MST     MemSsType,
  IN        UINT8       Socket,
  IN        UINT32      ChannelMask,
  IN        UINT32      RankMask,
  IN        UINT8       SubChannel,
  IN        MRC_RT      DimmRank[MAX_CH], OPTIONAL
  IN        MRC_LT      Level,
  IN        MRC_GT      Group,
  IN        GSM_CSN     Signal,           OPTIONAL
  IN        UINT8       StepSize,
  IN        UINT8       DitherValue,
  IN        INT16       OffsetLeft,
  IN        INT16       OffsetRight,
  IN        INT16       LimitOffset,
  IN        FLAGS_CATEGORY_OFFSETS  FlagsOffsets,
  IN        FLAGS_CATEGORY_RESULTS  FlagsResults,
  IN        FLAGS_CATEGORY_FSM      FlagsFsm
);

/**
  Executes the FSM to find Left and Right margins across all available strobes on the current context
  WARNING:
  This function MUST be executed only after context was already created (CreateContextParameters).
  This function MUST be executed only after resources are allocated (AllocatePoolForResults1D || AllocatePoolForResults1DAllRanks).

  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      PreSweep1D        Callback to be executed before any sweep is done
  @param[in]      SetupTest         Callback to configure the engine to do the test
  @param[in]      ExecuteTest       Callback to configure and start the test
  @param[in]      ErrorHandler      Callback to be executed after each sample test completes (used to handle special errors)
  @param[in]      PostSweep1D       Callback to be executed when all sides are done
  @param[in]      ResultProcessing  Callback to interpret the HW results after each sample test completes
  @param[out]     Results           Pointer that contains the final results

  @retval         MRC_STATUS_SUCCESS       1D sweep function was executed without errors

**/
MRC_STATUS
EFIAPI
ProcessSweep1D (
  IN        COMMON_1D_PARAMETERS  *CommonParameters,
  IN        Callback1D  PreSweep1D,           OPTIONAL
  IN        Callback1D  SetupTest,            OPTIONAL
  IN        Callback1D  ExecuteTest,          OPTIONAL
  IN        Callback1D  ErrorHandler,         OPTIONAL
  IN        Callback1D  PostSweep1D,          OPTIONAL
  IN        Callback1D  ResultProcessing,
      OUT   RESULTS_1D  *Results
);

/**

  Executes the FSM to find Left and Right margins across all available strobes on the current context
  WARNING:
  This function MUST be executed only after context was already created (CreateContextParameters).
  This function MUST be executed only after resources are allocated (AllocatePoolForResults1D || AllocatePoolForResults1DAllRanks).

  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      PreSweep1D        Callback to be executed before any sweep is done
  @param[in]      SetupTest         Callback to configure the engine to do the test
  @param[in]      ExecuteTest       Callback to configure and start the test
  @param[in]      ErrorHandler      Callback to be executed after each sample test completes (used to handle special errors)
  @param[in]      PostSweep1D       Callback to be executed when all sides are done
  @param[in]      ResultProcessing  Callback to interpret the HW results after each sample test completes
  @param[out]     Results           Pointer that contains the final results

  @retval         MRC_STATUS_SUCCESS       1D sweep function was executed without errors

**/
MRC_STATUS
EFIAPI
ProcessSweepAllRanks1D (
  IN        COMMON_1D_PARAMETERS  *CommonParameters,
  IN        Callback1D  PreSweep1D,           OPTIONAL
  IN        Callback1D  SetupTest,            OPTIONAL
  IN        Callback1D  ExecuteTest,          OPTIONAL
  IN        Callback1D  ErrorHandler,         OPTIONAL
  IN        Callback1D  PostSweep1D,          OPTIONAL
  IN        Callback1D  ResultProcessing,
      OUT   RESULTS_1D  Results[MAX_RANK_CH]
);

/**

  Worker function to get 2D margins on the specified rank for each channel

  @param[in,out]  CommonParameters2D Pointer to the context information that includes vertical and horizontal axis
  @param[in]      MemSsType          Current Memory Subsystem Type (DdrLevel, LrBuffLevel, HbmLevel, etc)
  @param[in]      Socket             Current socket under test (0-based)
  @param[in]      ChannelMask        One-hot encoding to mask a channel (0: executes if enabled/populated, 1: skip/masked)
  @param[in]      RankMask           One-hot encoding to mask a rank (0: executes if enabled/populated, 1: skip/masked)
  @param[in]      SubChannel         Subchannel under test
                                     DDR4: (Not applicable)
                                     DDR5: 0, 1 or ALL_SUBCH
  @param[in]      DimmRank           Structure to specify current dimm and rank under test (0-based)

  @param[in]      LevelV             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      GroupV             Specifies the MRC_GT group that will be under test
  @param[in]      SignalV            Specifies the GSM_CSN Signal (this parameter is only required for few groups)
  @param[in]      StepSizeV          Specifies the StepSize to do the margining
  @param[in]      NumberOfSamples    Specifies the number of samples that will be taking in the vertical axis per side (left and right)
                                     Total number of samples will be 2*NumberOfSamples + 1

  @param[in]      LevelH             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      GroupH             Specifies the MRC_GT group that will be under test
  @param[in]      SignalH            Specifies the GSM_CSN Signal (this parameter is only required for few groups)
  @param[in]      StepSizeH          Specifies the StepSize to do the margining
                                     When FLAGS_CATEGORY_FSM.TransitionFineFsm is set this value is used as the Coarse step size.
  @param[in]      DitherValueH       The dither value specifies the number of passing consecutive sample, before consider a pass condition
  @param[in]      OffsetLeftH        Used to determine the start point on the left side (Original + OffsetLeft)
                                     If Flag FLAGS_CATEGORY_OFFSETS.UseOffsetAsAbsolute is set, OffsetLeft is as an absolute start value for left side
  @param[in]      OffsetRightH       Used to determine the start point on the right side (Original + OffsetRight)
                                     If Flag FLAGS_CATEGORY_OFFSETS.UseOffsetAsAbsolute is set, OffsetRight is as an absolute start value for right side
  @param[in]      LimitOffsetH       Limits the search to an specific range. This is only applicable when FLAGS_CATEGORY_OFFSETS.LimitSearch is set
                                     If FLAGS_CATEGORY_OFFSETS.LimitSearch is set sweep range will be:
                                     [Original - LimitOffset, Original + LimitOffset]
                                     If FLAGS_CATEGORY_OFFSETS.LimitSearch and FLAGS_CATEGORY_OFFSETS.UseOffsetAsAbsolute are set sweep range will be:
                                     [OffsetLeft - LimitOffset, OffsetRight + LimitOffset]

  @param[in]      FlagsOffsetsH      Modify the start/limit offset
                                     StartOnPassingEdges:
                                     Start point on the left side will be Original + LeftMargin
                                     Start point on the right side will be Original + RightMargin
                                     Only takes effect if FLAGS_CATEGORY_RESULTS.ResultsRelative is set.
                                     Only takes effect if FLAGS_CATEGORY_RESULTS.UseOffsetAsAbsolute is cleared
                                     OffsetLeft and OffsetRight only takes effect if previous passing edges were zero
                                     UseOffsetAsAbsolute:
                                     Start point on the left side will be OffsetLeft
                                     Start point on the right side will be OffsetRight
                                     LimitSearch:
                                     See LimitOffset description
                                     ForceCsrRead:
                                     Force all the read to be from CSR
                                     DisableFastTracking:
                                     Fast tracking is disable, so CurrentDelay need to be read from CACHE or CSR (IF ForceCsrRead is set)
                                     PerStrobeAccess:
                                     Set the delay for each strobe independently, ALL_STROBES is discouraged

  @param[in]      FlagsResultsH      Modify the format for the output results
                                     ResultsPerBit:
                                     It will ill collect the result on bit basis
                                     ResultsRelative:
                                     By default results are the delay values for Left and Right
                                     If ResultsRelative is set results are with respect to original delay value
                                     FocusOnTransition:
                                     Defines the passing or failing condition based on the result during the ST_AC_FIRST_SAMPLE
                                     The ST_AC_FIRST_SAMPLE sample will be always considered a failing condition and the algorithm will look for the transition
                                     Highly recommended to use this flag in combination with FLAGS_CATEGORY_OFFSETS.UseOffsetAsAbsolute and/or OffsetLeft/OffsetRight
                                     SingleBitTrack:
                                     Only tracks and display a single bit
                                     CmdCtlAllKnobs:
                                     Only applicable to CmdAll or CtlAll, this forces the training to be done on each Cmd/Ctl knob
                                     ResultsPerSubChannel:
                                     If there is any bit within the sub-channel, it will stop the sweep test immediately
                                     For DDR4 assumes a single sub-channel
                                     ResultsEvenOdd:
                                     Force the results to be tracked separately for even and odd (Only applicable to single knob delay types)

  @param[in]      FlagsFsmH          Modify the FSM behavior
                                     OneWayOnly:
                                     Once the limit is reached, algorithm does NOT try to search a passing condition in a different direction, it only stops
                                     LeftSideOnly:
                                     Only sweeps for left side
                                     RightSideOnly:
                                     Only sweeps for right side
                                     TrackingSweep:
                                     Enables an special sequence when it starts to search a passing condition in continuous incremental way
                                     Once the passing condition is found the algorithm keeps incrementing and searches for a failing condition
                                     ContinousMode:
                                     When an strobe get into final state it keeps decrementing or incrementing until all strobes are done
                                     This assumes that first sample at original value always passes
                                     AvoidBoundaryCross:
                                     During margining the boundaries of the original values are no crossed
                                     For Left Side the algorithm valid range will be [Minimum, Original]
                                     For Right Side the algorithm valid range will be [Original, Maximum]
                                     Does not take effect if FLAGS_CATEGORY_FSM.TrackingSweep is set
                                     TransitionFineFsm:
                                     This flag is used to find the transitions Pass->Fail or Fail->Pass with a coarse step size of 'StepSize'
                                     Once the transitions are found then an step size of 'SWEEP_FINE_STEP_SIZE' is used to fine more accurate results
                                     Not applicable when FLAGS_CATEGORY_RESULTS.ResultsPerSubChannel is set

  @param[in]      PreSweep1DH        Callback to be executed before any sweep is done
  @param[in]      SetupTestH         Callback to configure the engine to do the test
  @param[in]      ExecuteTestH       Callback to configure and start the test
  @param[in]      ErrorHandlerH      Callback to be executed after each sample test completes (used to handle special errors)
  @param[in]      PostSweep1DH       Callback to be executed when all sides are done
  @param[in]      ResultProcessingH  Callback to interpret the HW results after each sample test completes

                                    For each of the next parameters caller is responsible to reserve the
                                    number of total entries prior to call this function
                                    Total number of entries = 2 * NumberOfSamples + 1

  @param[out]     StrobeResults     Pointer to return the results at strobe level (this array must be able to holds a valid entry for each vertical sampling point)
  @param[out]     BitResults        Pointer to return the results at bit level (this array must be able to holds a valid entry for each vertical sampling point)
  @param[out]     RankResults       Pointer to return the results at rank level (this array must be able to holds a valid entry for each vertical sampling point)

  @retval         MRC_STATUS_SUCCESS       Context was initialized without errors
**/
MRC_STATUS
EFIAPI
ProcessWorker2D (
  IN  OUT   COMMON_2D_PARAMETERS  *CommonParameters2D,
  IN        MRC_MST     MemSsType,
  IN        UINT8       Socket,
  IN        UINT32      ChannelMask,
  IN        UINT32      RankMask,
  IN        UINT8       SubChannel,
  IN        MRC_RT      DimmRank[MAX_CH],

  IN        MRC_LT      LevelV,
  IN        MRC_GT      GroupV,
  IN        GSM_CSN     SignalV,           OPTIONAL
  IN        UINT8       StepSizeV,
  IN        UINT8       NumberOfSamples,

  IN        MRC_LT      LevelH,
  IN        MRC_GT      GroupH,
  IN        GSM_CSN     SignalH,           OPTIONAL
  IN        UINT8       StepSizeH,
  IN        UINT8       DitherValueH,
  IN        INT16       OffsetLeftH,
  IN        INT16       OffsetRightH,
  IN        INT16       LimitOffsetH,
  IN        FLAGS_CATEGORY_OFFSETS  FlagsOffsetsH,
  IN        FLAGS_CATEGORY_RESULTS  FlagsResultsH,
  IN        FLAGS_CATEGORY_FSM      FlagsFsmH,

  IN        Callback1D  PreSweep1DH,           OPTIONAL
  IN        Callback1D  SetupTestH,            OPTIONAL
  IN        Callback1D  ExecuteTestH,          OPTIONAL
  IN        Callback1D  ErrorHandlerH,         OPTIONAL
  IN        Callback1D  PostSweep1DH,          OPTIONAL
  IN        Callback1D  ResultProcessingH,

      OUT   struct strobeMargin *StrobeResults, OPTIONAL
      OUT   struct bitMargin    *BitResults,    OPTIONAL
      OUT   struct rankMargin   *RankResults

  );

/**

  Worker function to get 2D margins on all valid ranks for each channel

  @param[in,out]  CommonParameters2D Pointer to the context information that includes vertical and horizontal axis
  @param[in]      MemSsType          Current Memory Subsystem Type (DdrLevel, LrBuffLevel, HbmLevel, etc)
  @param[in]      Socket             Current socket under test (0-based)
  @param[in]      ChannelMask        One-hot encoding to mask a channel (0: executes if enabled/populated, 1: skip/masked)
  @param[in]      RankMask           One-hot encoding to mask a rank (0: executes if enabled/populated, 1: skip/masked)
  @param[in]      SubChannel         Subchannel under test
                                     DDR4: (Not applicable)
                                     DDR5: 0, 1 or ALL_SUBCH

  @param[in]      LevelV             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      GroupV             Specifies the MRC_GT group that will be under test
  @param[in]      SignalV            Specifies the GSM_CSN Signal (this parameter is only required for few groups)
  @param[in]      StepSizeV          Specifies the StepSize to do the margining
  @param[in]      NumberOfSamples    Specifies the number of samples that will be taking in the vertical axis per side (left and right)
                                     Total number of samples will be 2*NumberOfSamples + 1

  @param[in]      LevelH             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      GroupH             Specifies the MRC_GT group that will be under test
  @param[in]      SignalH            Specifies the GSM_CSN Signal (this parameter is only required for few groups)
  @param[in]      StepSizeH          Specifies the StepSize to do the margining
                                     When FLAGS_CATEGORY_FSM.TransitionFineFsm is set this value is used as the Coarse step size.
  @param[in]      DitherValueH       The dither value specifies the number of passing consecutive sample, before consider a pass condition
  @param[in]      OffsetLeftH        Used to determine the start point on the left side (Original + OffsetLeft)
                                     If Flag FLAGS_CATEGORY_OFFSETS.UseOffsetAsAbsolute is set, OffsetLeft is as an absolute start value for left side
  @param[in]      OffsetRightH       Used to determine the start point on the right side (Original + OffsetRight)
                                     If Flag FLAGS_CATEGORY_OFFSETS.UseOffsetAsAbsolute is set, OffsetRight is as an absolute start value for right side
  @param[in]      LimitOffsetH       Limits the search to an specific range. This is only applicable when FLAGS_CATEGORY_OFFSETS.LimitSearch is set
                                     If FLAGS_CATEGORY_OFFSETS.LimitSearch is set sweep range will be:
                                     [Original - LimitOffset, Original + LimitOffset]
                                     If FLAGS_CATEGORY_OFFSETS.LimitSearch and FLAGS_CATEGORY_OFFSETS.UseOffsetAsAbsolute are set sweep range will be:
                                     [OffsetLeft - LimitOffset, OffsetRight + LimitOffset]

  @param[in]      FlagsOffsetsH      Modify the start/limit offset
                                     StartOnPassingEdges:
                                     Start point on the left side will be Original + LeftMargin
                                     Start point on the right side will be Original + RightMargin
                                     Only takes effect if FLAGS_CATEGORY_RESULTS.ResultsRelative is set.
                                     Only takes effect if FLAGS_CATEGORY_RESULTS.UseOffsetAsAbsolute is cleared
                                     OffsetLeft and OffsetRight only takes effect if previous passing edges were zero
                                     UseOffsetAsAbsolute:
                                     Start point on the left side will be OffsetLeft
                                     Start point on the right side will be OffsetRight
                                     LimitSearch:
                                     See LimitOffset description
                                     ForceCsrRead:
                                     Force all the read to be from CSR
                                     DisableFastTracking:
                                     Fast tracking is disable, so CurrentDelay need to be read from CACHE or CSR (IF ForceCsrRead is set)
                                     PerStrobeAccess:
                                     Set the delay for each strobe independently, ALL_STROBES is discouraged

  @param[in]      FlagsResultsH      Modify the format for the output results
                                     ResultsPerBit:
                                     It will ill collect the result on bit basis
                                     ResultsRelative:
                                     By default results are the delay values for Left and Right
                                     If ResultsRelative is set results are with respect to original delay value
                                     FocusOnTransition:
                                     Defines the passing or failing condition based on the result during the ST_AC_FIRST_SAMPLE
                                     The ST_AC_FIRST_SAMPLE sample will be always considered a failing condition and the algorithm will look for the transition
                                     Highly recommended to use this flag in combination with FLAGS_CATEGORY_OFFSETS.UseOffsetAsAbsolute and/or OffsetLeft/OffsetRight
                                     SingleBitTrack:
                                     Only tracks and display a single bit
                                     CmdCtlAllKnobs:
                                     Only applicable to CmdAll or CtlAll, this forces the training to be done on each Cmd/Ctl knob
                                     ResultsPerSubChannel:
                                     If there is any bit within the sub-channel, it will stop the sweep test immediately
                                     For DDR4 assumes a single sub-channel
                                     ResultsEvenOdd:
                                     Force the results to be tracked separately for even and odd (Only applicable to single knob delay types)

  @param[in]      FlagsFsmH          Modify the FSM behavior
                                     OneWayOnly:
                                     Once the limit is reached, algorithm does NOT try to search a passing condition in a different direction, it only stops
                                     LeftSideOnly:
                                     Only sweeps for left side
                                     RightSideOnly:
                                     Only sweeps for right side
                                     TrackingSweep:
                                     Enables an special sequence when it starts to search a passing condition in continuous incremental way
                                     Once the passing condition is found the algorithm keeps incrementing and searches for a failing condition
                                     ContinousMode:
                                     When an strobe get into final state it keeps decrementing or incrementing until all strobes are done
                                     This assumes that first sample at original value always passes
                                     AvoidBoundaryCross:
                                     During margining the boundaries of the original values are no crossed
                                     For Left Side the algorithm valid range will be [Minimum, Original]
                                     For Right Side the algorithm valid range will be [Original, Maximum]
                                     Does not take effect if FLAGS_CATEGORY_FSM.TrackingSweep is set
                                     TransitionFineFsm:
                                     This flag is used to find the transitions Pass->Fail or Fail->Pass with a coarse step size of 'StepSize'
                                     Once the transitions are found then an step size of 'SWEEP_FINE_STEP_SIZE' is used to fine more accurate results
                                     Not applicable when FLAGS_CATEGORY_RESULTS.ResultsPerSubChannel is set

  @param[in]      PreSweep1DH        Callback to be executed before any sweep is done
  @param[in]      SetupTestH         Callback to configure the engine to do the test
  @param[in]      ExecuteTestH       Callback to configure and start the test
  @param[in]      ErrorHandlerH      Callback to be executed after each sample test completes (used to handle special errors)
  @param[in]      PostSweep1DH       Callback to be executed when all sides are done
  @param[in]      ResultProcessingH  Callback to interpret the HW results after each sample test completes

                                    For each of the next parameters caller is responsible to reserve the
                                    number of total entries prior to call this function
                                    Total number of entries = 2 * NumberOfSamples + 1

  @param[out]     StrobeResults     Pointer to return the results at strobe level (this array must be able to holds a valid entry for each vertical sampling point)
  @param[out]     BitResults        Pointer to return the results at bit level (this array must be able to holds a valid entry for each vertical sampling point)
  @param[out]     RankResults       Pointer to return the results at rank level (this array must be able to holds a valid entry for each vertical sampling point)

  @retval         MRC_STATUS_SUCCESS       Context was initialized without errors
**/
MRC_STATUS
EFIAPI
ProcessWorkerAllRanks2D (
  IN  OUT   COMMON_2D_PARAMETERS  *CommonParameters2D,
  IN        MRC_MST     MemSsType,
  IN        UINT8       Socket,
  IN        UINT32      ChannelMask,
  IN        UINT32      RankMask,
  IN        UINT8       SubChannel,

  IN        MRC_LT      LevelV,
  IN        MRC_GT      GroupV,
  IN        GSM_CSN     SignalV,           OPTIONAL
  IN        UINT8       StepSizeV,
  IN        UINT8       NumberOfSamples,

  IN        MRC_LT      LevelH,
  IN        MRC_GT      GroupH,
  IN        GSM_CSN     SignalH,           OPTIONAL
  IN        UINT8       StepSizeH,
  IN        UINT8       DitherValueH,
  IN        INT16       OffsetLeftH,
  IN        INT16       OffsetRightH,
  IN        INT16       LimitOffsetH,
  IN        FLAGS_CATEGORY_OFFSETS  FlagsOffsetsH,
  IN        FLAGS_CATEGORY_RESULTS  FlagsResultsH,
  IN        FLAGS_CATEGORY_FSM      FlagsFsmH,

  IN        Callback1D  PreSweep1DH,           OPTIONAL
  IN        Callback1D  SetupTestH,            OPTIONAL
  IN        Callback1D  ExecuteTestH,          OPTIONAL
  IN        Callback1D  ErrorHandlerH,         OPTIONAL
  IN        Callback1D  PostSweep1DH,          OPTIONAL
  IN        Callback1D  ResultProcessingH,

      OUT   struct strobeMargin *StrobeResults, OPTIONAL
      OUT   struct bitMargin    *BitResults,    OPTIONAL
      OUT   struct rankMargin   *RankResults

  );

/**

  Allocate resources to store the results generated after running the 1D Sweep
  WARNING: This function MUST be executed only after context was already created (CreateContextParameters).

  @param[in]      CommonParamaters  Pointer to the context information
  @param[in,out]  Results           Pointer that contains the final results

  @retval         MRC_STATUS_SUCCESS       Allocated resources for final results

**/
MRC_STATUS
EFIAPI
AllocatePoolForResults1D (
  IN        COMMON_1D_PARAMETERS  *CommonParameters,
  IN  OUT   RESULTS_1D      *Results
);

/**

  Allocate resources to store the results generated after running the 1D Sweep
  WARNING: This function MUST be executed only after context was already created (CreateContextParameters).

  @param[in]      CommonParamaters  Pointer to the context information
  @param[in,out]  Results           Pointer that contains the final results

  @retval         MRC_STATUS_SUCCESS       Allocated resources for final results

**/
MRC_STATUS
EFIAPI
AllocatePoolForResultsAllRanks1D (
  IN        COMMON_1D_PARAMETERS  *CommonParameters,
  IN  OUT   RESULTS_1D      Results[MAX_RANK_CH]
);

/*

  Releases resources used to run 1D Sweep
  WARNING:
  This function MUST be executed only after context was already created (CreateContextParameters).
  This function MUST be executed only after resources are allocated (AllocatePoolForResults1D || AllocatePoolForResults1DAllRanks).


  @param[in]      CommonParamaters  Pointer to the context information
  @param[in,out]  Results           Pointer that contains the final results

  @retval         MRC_STATUS_SUCCESS       Resources released successfully

**/
MRC_STATUS
EFIAPI
ReleaseResources1D (
  IN        COMMON_1D_PARAMETERS  *CommonParameters,
  IN  OUT   RESULTS_1D            *Results
);

/**

  Releases resources used to run 1D Sweep
  WARNING:
  This function MUST be executed only after context was already created (CreateContextParameters).
  This function MUST be executed only after resources are allocated (AllocatePoolForResults1D || AllocatePoolForResults1DAllRanks).


  @param[in]      CommonParamaters  Pointer to the context information
  @param[in,out]  Results           Pointer that contains the final results

  @retval         MRC_STATUS_SUCCESS       Resources released successfully

**/
MRC_STATUS
EFIAPI
ReleaseResourcesAllRanks1D (
  IN        COMMON_1D_PARAMETERS  *CommonParameters,
  IN  OUT   RESULTS_1D            Results[MAX_RANK_CH]
);

/*

  Returns the pointer to the results which are specific to the channel, knob and logical strobe specified
  WARNING:
  This function MUST be executed only after context was already created (CreateContextParameters).
  This function MUST be executed only after resources are allocated (AllocatePoolForResults1D || AllocatePoolForResults1DAllRanks).


  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      KnobIndex         Current knob under test (0-based)
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      StrobeLogical     Current logical strobe under test (0-based)
  @param[in]      Results           Pointer that contains the final results

  @return  Pointer to &Results->ResultsPerStrobe[KnobIndex][ChannelIndex][StrobeLogical]

**/
struct baseMargin *
EFIAPI
GetResultPtrAtStrobeLevel (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   KnobIndex,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   StrobeLogical,
  IN         RESULTS_1D              *Results
);

/**

  Returns the pointer to the results which are specific to the channel, knob, logical strobe and bit specified
  WARNING:
  This function MUST be executed only after context was already created (CreateContextParameters).
  This function MUST be executed only after resources are allocated (AllocatePoolForResults1D || AllocatePoolForResults1DAllRanks).


  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      KnobIndex         Current knob under test (0-based)
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      StrobeLogical     Current logical strobe under test (0-based)
  @param[in]      BitIndex          Current bit under test (0-based)
  @param[in]      Results           Pointer that contains the final results

  @return  Pointer to &Results->ResultsPerBit[KnobIndex][ChannelIndex][StrobeLogical][BitIndex]

**/
struct baseMargin *
EFIAPI
GetResultPtrAtBitLevel (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   KnobIndex,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   StrobeLogical,
  IN         UINT8                   BitIndex,
  IN         RESULTS_1D              *Results
);

/**

  Print the data collected in Results
  WARNING:
  This function MUST be executed only after context was already created (CreateContextParameters).
  This function MUST be executed only after resources are allocated (AllocatePoolForResults1D || AllocatePoolForResults1DAllRanks).


  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      Results           Pointer that contains the final results

**/
VOID
EFIAPI
PrintCurrentResults1D (
  IN        COMMON_1D_PARAMETERS    *CommonParameters,
      OUT   RESULTS_1D  *Results
);

/*

  Results TRUE if the current linear strobe should be skipped

  DDR5/4 SubCh MaxStrobeAvailable AreResultsAtByteLevel  TRUE IF:
     4    DC          18                FALSE            StrobeLogical >= 18
     4    DC          18                TRUE             StrobeLogical >= 9

     5    0/1         10                TRUE             StrobeLogical >= 5
     5    0/1         10                FALSE            StrobeLogical >= 10

     5    ALL         20                TRUE             StrobeLogical >= 10
     5    ALL         20                FALSE            StrobeLogical >= 20

     5    DC          10                TRUE - BRS       StrobeLogical >= 5
     5    DC          10                FALSE - BRS      StrobeLogical >= 10

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      StrobeLinear      Current linear strobe under test (0-based)

  @retval         TRUE      If current linear strobe needs to be skipped
  @retval         FALSE     If current linear strobe needs to be executed

*/
BOOLEAN
EFIAPI
SkipExecutionOnStrobe (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   StrobeLinear
);


/**

  Skip channel execution

  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      ChannelIndex      Current channel under test (0-based)

  @retval         TRUE              Skips channel execution
  @retval         FALSE             Current channel will be executed

**/
BOOLEAN
SkipChannelExecution (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   ChannelIndex
  );

/**

  Returns the pointer to input delay which is related to the channel, knob and logical strobe specified

  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      KnobIndex         Current knob under test (0-based, within the group delay)
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      StrobeLogical     Current logical strobe under test (0-based, same use as "strobe" but no swizzling)
  @param[in]      DelayInput        Pointer to delay values

  @return  Pointer to &DelayInput[KnobIndex][ChannelIndex][StrobeLogical]

**/
INT16 *
GetDelayPtr (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   KnobIndex,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   StrobeLogical,
  IN         INT16                   (*DelayInput)[MAX_CH][MAX_STROBE]
);

/**

  Callback function to configure the engine before start doing the tests

  @param[in]      CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       Callback function executed properly

**/
MRC_STATUS
EFIAPI
SetupSweepAdvance (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
);

/**

  Callback function to configure and/or start the test for most advanced training algorithms

  @param[in]      CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       Callback function executed properly

**/
MRC_STATUS
EFIAPI
ExecuteSweepAdvance (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
);

/**

  Callback to interpret the HW results after each sample test completes for most advanced training algorithms

  @param[in]      CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       Callback function executed properly

**/
MRC_STATUS
EFIAPI
ResultProcessingAdvance (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
);

/*

  Callback to interpret the HW/RcSim results after each sample test completes for most advanced training algorithms
  and operating in 2D mode

  @param[in]      CommonParameters  Pointer to the context information for 2D usage (Pointer to COMMON_1D_PARAMETERS_FOR_2D)

  @retval         MRC_STATUS_SUCCESS       Callback function executed properly

*/
MRC_STATUS
EFIAPI
ResultProcessingAdvance2D (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
);

/**

  This function does the conversion between linear and logical strobe.
  A linear strobe is only a increasing counter that determines the number of strobes to be tested
  The logical strobe is the actual one to be trained or processed.

  DDR4:
  Linear strobe could be 0->17 (nibble) or 0->8 (byte) --> the conversion is 1:1 mapping to logical strobe

  DDR5:
  Linear strobe to Logical strobe is based on ALL or Independent Sub-Channel operation,

  0->9  (nibbles)for Sub-Channel 0 --> the conversion is 1:1 mapping to logical strobe
  0->9  (nibbles)for Sub-Channel 1 --> the conversion needs to be adjusted to logical strobes 10-19

  0->4  (bytes)for Sub-Channel 0 --> the conversion is 1:1 mapping to logical strobe
  0->4  (bytes)for Sub-Channel 1 --> the conversion needs to be adjusted to logical strobes 10-14


  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      SubChannelIndex   Current sub-channel under test (0-based)
  @param[in]      StrobeLinear      Current linear strobe under test (0-based)

  @return   The logical strobe based on the Nibble/Byte operation and channel mode

**/
UINT8
EFIAPI
GetStrobeLogical (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   SubChannelIndex,
  IN         UINT8                   StrobeLinear
  );

/**

  Returns the pointer to the current results condition which is related to the channel, knob, logical strobe and bit specified

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      KnobIndex         Current knob under test (0-based)
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      StrobeLogical     Current logical strobe under test (0-based)
  @param[in]      BitIndex          Current bit under test (0-based)

  @return  Pointer to &ResultCondition[KnobIndex][ChannelIndex][StrobeLogical][BitIndex]

**/
RESULTS_1D_CONDITION *
EFIAPI
GetResultConditionPtr (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   KnobIndex,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   StrobeLogical,
  IN         UINT8                   BitIndex
  );

/*
  Function returns the logical strobe index for a specified bit which is indexed inside a channel.
  The bit index range from 0 to MAX_BITS-1. It is the index used in the CPGC bit error status.

  DDR5
  CPGC Strobe[Lane/Bit]   Logical Strobe
  0 [3:0]                 0
  1 [7:4]                 5
  2 [11:8]                1
  3 [15:12]               6
  4 [19:16]               2
  5 [23:20]               7
  6 [27:24]               3
  7 [31:28]               8
  8 [35:32]               4
  9 [39:36]               9
  10 [43:40]              10
  11 [47:44]              15
  12 [51:48]              11
  13 [55:52]              16
  14 [59:56]              12
  15 [63:60]              17
  16 [67:64]              13
  17 [71:68]              18
  18 [75:72]              14
  19 [79:76]              19

  DDR4
  CPGC Strobe[Lane/Bit]   Logical Strobe
  0 [3:0]                 0
  1 [7:4]                 9
  2 [11:8]                1
  3 [15:12]               10
  4 [19:16]               2
  5 [23:20]               11
  6 [27:24]               3
  7 [31:28]               12
  8 [35:32]               4
  9 [39:36]               13
  10 [43:40]              5
  11 [47:44]              14
  12 [51:48]              6
  13 [55:52]              15
  14 [59:56]              7
  15 [63:60]              16
  16 [67:64]              8
  17 [71:68]              17

  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      BitIndex          Current bit under test (0-based) inside the channel. Range from 0 to MAX_BITS-1

  @return Logical Strobe index

*/
UINT8
EFIAPI
GetLogicalStrobeFromBit (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   BitIndex
  );

/**

  Returns the pointer to the results which are specific to the channel and bit specified.
  The bit is the bit index inside the channel which is range from 0 to MAX_BITS-1.

  WARNING:
  This function MUST be executed only after context was already created (CreateContextParameters).
  This function MUST be executed only after resources are allocated (AllocatePoolForResults1D || AllocatePoolForResults1DAllRanks).


  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      KnobIndex         Current knob under test (0-based)
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      BitIndex          Current bit under test (0-based) inside the channel. Range from 0 to MAX_BITS-1
  @param[in]      Results           Pointer that contains the final results

  @return  Pointer to &Results->ResultsPerBit[KnobIndex][ChannelIndex][StrobeLogical][BitIndex]

**/
struct baseMargin *
EFIAPI
GetResultPtrAtRawBitLevel (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   KnobIndex,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   BitIndex,
  IN         RESULTS_1D              *Results
  );

/**

  Returns the pointer to the current results condition which is related to the channel, knob and bit specified
  The bit is the bit index inside the channel which is range from 0 to MAX_BITS-1.

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      KnobIndex         Current knob under test (0-based)
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      BitIndex          Current bit under test (0-based) inside the channel. Range from 0 to MAX_BITS-1

  @return  Pointer to &ResultCondition[KnobIndex][ChannelIndex][StrobeLogical][BitIndex]

**/
RESULTS_1D_CONDITION *
EFIAPI
GetResultConditionPtrAtRawBitLevel (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   KnobIndex,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   BitIndex
  );

/**

  Returns the current delay offset from its orignal delay for the channel, knob and strobe specified

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      KnobIndex         Current knob under test (0-based)
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      SubChannelIndex   Current subchannel under test (0-based)
  @param[in]      StrobeLinear      Current strobe under test (0-based)
  @param[out]     Offset            Offet

  @return  MRC_STATUS_SUCCESS,  MRC_STATUS_FAILURE

**/
MRC_STATUS
EFIAPI
GetStrobeDelayOffset (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   KnobIndex,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   SubChannelIndex,
  IN         UINT8                   StrobeLinear,
  OUT        INT16                   *Offset
  );

/**

  Returns the current delay offset from its orignal delay for the specified channel.
  The returned dalay offset is the maximum offset of all knobs and strobes in the channel.

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[out]     Offset            Offet

  @return  MRC_STATUS_SUCCESS,  MRC_STATUS_FAILURE

**/
MRC_STATUS
EFIAPI
GetChannelDelayOffset (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   ChannelIndex,
  OUT        INT16                   *Offset
  );

/**

  Callback to be executed after encountering error in Cmd/Ctl test

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       Callback function executed properly

**/
MRC_STATUS
EFIAPI
ErrorHandlerAdvanceCmdCtl (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  );

/**

  Callback to be executed after encountering error in TxVref test

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       Callback function executed properly

**/
MRC_STATUS
EFIAPI
ErrorHandlerAdvanceTxVref (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  );

#endif // __MEMSWEEPLIBRARY_H__

