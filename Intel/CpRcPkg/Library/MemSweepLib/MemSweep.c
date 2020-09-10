/** @file

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


#include "MemSweep.h"

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
  )
{
  CHAR8                 *GroupString;
  MRC_MST               MemSsType;
  COMMON_1D_PARAMETERS    *CommonParameters;
  FLAGS_CATEGORY_OFFSETS  FlagsOffsets;
  FLAGS_CATEGORY_FSM      FlagsFsm;

  CommonParameters  = RcAllocatePool (sizeof (*CommonParameters));
  if (CommonParameters == NULL) {
    return MRC_STATUS_FAILURE;
  }

  MemSsType     = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  FlagsOffsets  = LimitSearch | StartOnPassingEdges;
  FlagsFsm      = DefaultFsm;

  if (BitResults != NULL || BitResultsEvenOdd != NULL) {
    FlagsResults |= ResultsPerBit;
  }

  if (StrobeResultsEvenOdd != NULL || BitResultsEvenOdd != NULL || RankResultsEvenOdd != NULL) {
    FlagsResults |= ResultsEvenOdd;
  }

  GetGroupString (Group, &GroupString);
  RcDebugPrint (SDBG_MAX, "Run MemSweep on %a \n", GroupString);

  ProcessWorkerAllRanks1D (
    CommonParameters,         ///< COMMON_1D_PARAMETERS
    MemSsType,                ///< MRC_MST
    Socket,                   ///< Socket
    0,                        ///< ChannelMask
    0,                        ///< RankMask
    ALL_SUBCH,                ///< SubChannel
    Level,                    ///< MRC_LT Level
    Group,                    ///< MRC_GT Group
    gsmCsnDelim,              ///< GSM_CSN Signal
    1,                        ///< StepSize
    1,                        ///< DitherValue
    -1,                       ///< OffsetLeft
    0,                        ///< OffsetRight
    ONE_QCLK,                 ///< LimitOffset
    FlagsOffsets,             ///< FLAGS_CATEGORY_OFFSETS
    FlagsResults,             ///< FLAGS_CATEGORY_RESULTS
    FlagsFsm,                 ///< FLAGS_CATEGORY_FSM
    NULL,                     ///< Callback1D  PreSweep1D
    SetupSweepAdvance,        ///< Callback1D  SetupTest
    ExecuteSweepAdvance,      ///< Callback1D  ExecuteTest
    NULL,                     ///< Callback1D  ErrorHandler
    NULL,                     ///< Callback1D  PostSweep1D
    ResultProcessingAdvance,  ///< Callback1D  ResultProcessing

    StrobeResults,            ///< struct strobeMargin *StrobeResults
    BitResults,               ///< struct bitMargin    *BitResults
    RankResults,              ///< struct rankMargin   *RankResults
    StrobeResultsEvenOdd,     ///< struct strobeMargin StrobeResultsEvenOdd[MaxChunkSides]
    BitResultsEvenOdd,        ///< struct bitMargin    BitResultsEvenOdd[MaxChunkSides]
    RankResultsEvenOdd        ///< struct rankMargin   RankResultsEvenOdd[MaxChunkSides]
    );

  RcFreePool (CommonParameters);
  return MRC_STATUS_SUCCESS;
}

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
  )
{
  CHAR8                 *GroupString;
  MRC_MST               MemSsType;
  COMMON_1D_PARAMETERS    *CommonParameters;
  FLAGS_CATEGORY_OFFSETS  FlagsOffsets;
  FLAGS_CATEGORY_RESULTS  FlagsResults;
  FLAGS_CATEGORY_FSM      FlagsFsm;

  CommonParameters  = RcAllocatePool (sizeof (*CommonParameters));
  if (CommonParameters == NULL) {
    return MRC_STATUS_FAILURE;
  }

  MemSsType     = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  FlagsOffsets  = LimitSearch | StartOnPassingEdges;
  FlagsFsm      = DefaultFsm;
  FlagsResults  = ResultsRelative;

  if (BitResults != NULL) {
    FlagsResults |= ResultsPerBit;
  }

  GetGroupString (Group, &GroupString);
  RcDebugPrint (SDBG_MAX, "Run MemSweep on %a \n", GroupString);

  ProcessWorkerAllRanks1D (
    CommonParameters,         ///< COMMON_1D_PARAMETERS
    MemSsType,                ///< MRC_MST
    Socket,                   ///< Socket
    0,                        ///< ChannelMask
    0,                        ///< RankMask
    ALL_SUBCH,                ///< SubChannel
    Level,                    ///< MRC_LT Level
    Group,                    ///< MRC_GT Group
    gsmCsnDelim,              ///< GSM_CSN Signal
    1,                        ///< StepSize
    1,                        ///< DitherValue
    -1,                       ///< OffsetLeft
    0,                        ///< OffsetRight
    ONE_QCLK,                 ///< LimitOffset
    FlagsOffsets,             ///< FLAGS_CATEGORY_OFFSETS
    FlagsResults,             ///< FLAGS_CATEGORY_RESULTS
    FlagsFsm,                 ///< FLAGS_CATEGORY_FSM
    NULL,                     ///< Callback1D  PreSweep1D
    SetupSweepAdvance,        ///< Callback1D  SetupTest
    ExecuteSweepAdvance,      ///< Callback1D  ExecuteTest
    NULL,                     ///< Callback1D  ErrorHandler
    NULL,                     ///< Callback1D  PostSweep1D
    ResultProcessingAdvance,  ///< Callback1D  ResultProcessing

    StrobeResults,            ///< struct strobeMargin *StrobeResults
    BitResults,               ///< struct bitMargin    *BitResults
    RankResults,              ///< struct rankMargin   *RankResults
    NULL,                     ///< struct strobeMargin StrobeResultsEvenOdd[MaxChunkSides]
    NULL,                     ///< struct bitMargin    BitResultsEvenOdd[MaxChunkSides]
    NULL                      ///< struct rankMargin   RankResultsEvenOdd[MaxChunkSides]
    );

  RcFreePool (CommonParameters);

  return MRC_STATUS_SUCCESS;
}

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

  )
{
  UINT8   KnobIndex;
  RESULTS_1D            Results1D;
  struct strobeMargin   *StrobeResultsEvenOddPtr;
  struct bitMargin      *BitResultsEvenOddPtr;
  struct rankMargin     *RankResultsEvenOddPtr;

  CreateContextParameters (
    CommonParameters,
    MemSsType,
    Socket,
    ChannelMask,
    RankMask,
    SubChannel,
    DimmRank,
    Level,
    Group,
    Signal,
    StepSize,
    DitherValue,
    OffsetLeft,
    OffsetRight,
    LimitOffset,
    FlagsOffsets,
    FlagsResults,
    FlagsFsm
  );

  AllocatePoolForResults1D (
    CommonParameters,
    &Results1D
    );

  ProcessSweep1D (
    CommonParameters,
    PreSweep1D,
    SetupTest,
    ExecuteTest,
    ErrorHandler,
    PostSweep1D,
    ResultProcessing,
    &Results1D
    );

  PrintCurrentResults1D (CommonParameters, &Results1D);

  ClearLegacyResults (
    CommonParameters,
    DimmRank,
    StrobeResults,
    BitResults,
    RankResults,
    StrobeResultsEvenOdd,
    BitResultsEvenOdd,
    RankResultsEvenOdd
    );

  ConvertResults1D (
    CommonParameters,
    &Results1D,
    StrobeResults,
    BitResults,
    RankResults,
    StrobeResultsEvenOdd,
    BitResultsEvenOdd,
    RankResultsEvenOdd
    );

  PrintLegacyResults (CommonParameters, NO_KNOB_INDEX, StrobeResults, BitResults, RankResults);

  for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {

    StrobeResultsEvenOddPtr = NULL;
    BitResultsEvenOddPtr    = NULL;
    RankResultsEvenOddPtr   = NULL;

    if (StrobeResultsEvenOdd != NULL) {
      StrobeResultsEvenOddPtr = &StrobeResultsEvenOdd[KnobIndex];
    }

    if (BitResultsEvenOdd != NULL) {
      BitResultsEvenOddPtr    = &BitResultsEvenOdd[KnobIndex];
    }

    if (RankResultsEvenOdd != NULL) {
      RankResultsEvenOddPtr   = &RankResultsEvenOdd[KnobIndex];
    }


    PrintLegacyResults (
      CommonParameters,
      KnobIndex,
      StrobeResultsEvenOddPtr,
      BitResultsEvenOddPtr,
      RankResultsEvenOddPtr
      );
  }

  ReleaseResources1D (CommonParameters, &Results1D);

  return MRC_STATUS_SUCCESS;
}

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
  )
{
  UINT8   ChannelIndex;
  UINT8   DimmIndex;
  UINT8   RankIndex;
  UINT8   LogicalRank;
  UINT8   MaxChDdr;
  UINT32  BackupChannelMask;
  MRC_RT  DimmRank[MAX_CH];
  BOOLEAN ExecutedDimmRank[MAX_DIMM][MAX_RANK_DIMM];
  PSYSHOST  Host;

  CHANNEL_NVRAM_STRUCT (*ChannelNvList)[MAX_CH];

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();

  ChannelNvList = GetChannelNvList (Host, Socket);

  BackupChannelMask = ChannelMask;

  for (DimmIndex = 0; DimmIndex < MAX_DIMM; DimmIndex++) {
    for (RankIndex = 0; RankIndex < MAX_RANK_DIMM; RankIndex++) {

      ExecutedDimmRank[DimmIndex][RankIndex] = FALSE;

      //
      // Restore the original input from the user
      //
      ChannelMask = BackupChannelMask;

      LogicalRank = GetLogicalRank (Host, Socket, 0, DimmIndex, RankIndex);

      if ((RankMask & (1 << LogicalRank)) != 0) {
        continue;
      }

      for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {

        if (!IsChannelEnabled (Socket, ChannelIndex)) {
          //
          // Mask any channels which are not enabled.
          // Note: This was only done in order ChannelMask is used as
          // a decision maker if ProcessSweep1D needs to be executed or not
          //
          ChannelMask |= 1 << ChannelIndex;
        }

        DimmRank[ChannelIndex].Dimm = DimmIndex;
        DimmRank[ChannelIndex].Rank = RankIndex;

        if (IsRankPresent (Socket, ChannelIndex, DimmIndex, RankIndex)) {

          if (Level == LrbufLevel && !IsLrdimmPresent (Socket, ChannelIndex, DimmIndex)) {
            //
            // Mask channels that do not have any LRDIMM installed and LrbufLevel
            //
            ChannelMask |= 1 << ChannelIndex;
          }

        } else {

          //
          // Mask channels that do not have any Dimm/Rank present
          //
          ChannelMask |= 1 << ChannelIndex;

        }

      } // ChannelIndex loop ...

      if ((ChannelMask & CH_BITMASK) != CH_BITMASK) {

        ExecutedDimmRank[DimmIndex][RankIndex] = TRUE;

        ProcessWorker1D (
          CommonParameters,
          MemSsType,
          Socket,
          ChannelMask,
          RankMask,
          SubChannel,
          DimmRank,
          Level,
          Group,
          Signal,
          StepSize,
          DitherValue,
          OffsetLeft,
          OffsetRight,
          LimitOffset,
          FlagsOffsets,
          FlagsResults,
          FlagsFsm,

          PreSweep1D,
          SetupTest,
          ExecuteTest,
          ErrorHandler,
          PostSweep1D,
          ResultProcessing,

          StrobeResults,
          BitResults,
          RankResults,

          StrobeResultsEvenOdd,
          BitResultsEvenOdd,
          RankResultsEvenOdd
          );

      }

    } // RankIndex loop ...
  } // DimmIndex loop ...

  return MRC_STATUS_SUCCESS;
}


/*

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
*/
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
)
{
  UINT8 ChannelIndex;
  CHANNEL_NVRAM_STRUCT (*ChannelNvList)[MAX_CH];

  SetMem (CommonParameters, sizeof (COMMON_1D_PARAMETERS), 0);
  //
  // Global Inputs
  //
  CommonParameters->DdrType      = Ddr4Type;
#ifdef DDR5_SUPPORT
  CommonParameters->DdrType      = Ddr5Type;
#endif // #ifdef DDR5_SUPPORT
  CommonParameters->Socket       = Socket;
  CommonParameters->ChannelMask  = ChannelMask;
  CommonParameters->RankMask     = RankMask;
  CommonParameters->SubChannel   = SubChannel;

  if (CommonParameters->DdrType == Ddr4Type) {

    CommonParameters->MinSubChannelIndex = 0;
    CommonParameters->MaxSubChannelIndex = 1;

  } else {

    if (CommonParameters->SubChannel == ALL_SUBCH) {

      CommonParameters->MinSubChannelIndex = 0;
      CommonParameters->MaxSubChannelIndex = SUB_CH;

    } else {

      CommonParameters->MinSubChannelIndex = CommonParameters->SubChannel;
      CommonParameters->MaxSubChannelIndex = CommonParameters->SubChannel + 1;

    }

  }

  CommonParameters->Level        = Level;
  CommonParameters->Group        = Group;
  CommonParameters->Signal       = Signal;
  CommonParameters->StepSize     = StepSize;
  CommonParameters->DitherValue  = DitherValue;
  CommonParameters->OffsetRight  = OffsetRight;
  CommonParameters->OffsetLeft   = OffsetLeft;
  CommonParameters->LimitOffset  = LimitOffset;
  CommonParameters->FlagsOffsets = FlagsOffsets;
  CommonParameters->FlagsResults = FlagsResults;
  CommonParameters->FlagsFsm     = FlagsFsm;
  //
  // Local Inputs
  //
  CommonParameters->Host           = GetSysHostPointer ();
  CommonParameters->Setup          = GetSysSetupPointer ();
  CommonParameters->KnobsPerGroup  = GetNumberOfKnobsPerGroup (CommonParameters);
  CommonParameters->MaxStrobeAvailable = GetMaxStrobeAvailable (CommonParameters);
  CommonParameters->NextNibbleOffset   = GetNextNibbleOffset (CommonParameters);

  if (CommonParameters->FlagsResults & SingleBitTrack) {
    CommonParameters->BitsToTrack  = 1;
  } else {
    CommonParameters->BitsToTrack  = BITS_PER_NIBBLE;
  }

  if (CommonParameters->FlagsOffsets & ForceCsrRead) {
    CommonParameters->ReadMode =  GSM_READ_CSR;
  } else {
    CommonParameters->ReadMode =  0;
  }

  CommonParameters->MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (CommonParameters->Host, CommonParameters->Socket);
  for (ChannelIndex = 0; ChannelIndex < CommonParameters->MaxChDdr; ChannelIndex++) {
    if (ChannelIndex >= MAX_CH) {
      continue;
    }
    if (DimmRank != NULL) {
      //
      // Populate Dimm/Rank based on API input
      // Populate SubRank based on current encoded mode
      //
      CommonParameters->Dimm[ChannelIndex]    = DimmRank[ChannelIndex].Dimm;
      CommonParameters->Rank[ChannelIndex]    = DimmRank[ChannelIndex].Rank;
      if ((*ChannelNvList)[ChannelIndex].encodedCSMode) {
        CommonParameters->SubRank[ChannelIndex] = DimmRank[ChannelIndex].Rank >> 1;
      } else {
        CommonParameters->SubRank[ChannelIndex] = 0;
      }
    }


    if (IsChannelEnabled (CommonParameters->Socket, ChannelIndex)) {
      CommonParameters->ChannelEnabledLocal[ChannelIndex] = TRUE;
    } else {
      CommonParameters->ChannelEnabledLocal[ChannelIndex] = FALSE;
    }
  } // ChannelIndex loop ...
  CommonParameters->IsEccEnabled    = IsEccEn ();
  CommonParameters->DebugLevel      = GetDebugLevel ();

  CommonParameters->OriginalDelays  = AllocatePoolForDelays (CommonParameters);
  CommonParameters->CurrentDelays   = AllocatePoolForDelays (CommonParameters);
  CommonParameters->DelayOffset     = AllocatePoolForDelays (CommonParameters);
  CommonParameters->MinMaxDeltaBitValue  = AllocatePoolForDelays (CommonParameters);

  CommonParameters->ResultCondition = AllocatePoolForResultCondition (CommonParameters);

  return MRC_STATUS_SUCCESS;
}

/*

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

*/
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
)
{
  EDGES_1D    SideIndex;
  EDGES_1D    SideStart;
  EDGES_1D    SideStop;

  CommonParameters->PreSweep1D  = PreSweep1D;
  CommonParameters->SetupTest   = SetupTest;
  CommonParameters->Results     = Results;

  //
  // Clear results
  //
  ClearResults1D (CommonParameters);

  //
  // Pre-requisites for IO/MC/etc
  //
  if (PreSweep1D != NULL) {
    (*PreSweep1D) (CommonParameters);
  }

  //
  // Configure engine to do a test on current HW
  //
  if (SetupTest != NULL) {
    (*SetupTest) (CommonParameters);
  }

  //
  // Store original values for all knobs
  //
  GetSetAcrossChannelsKnobs (
    CommonParameters,
    GSM_READ_ONLY | GSM_READ_CSR | GSM_UPDATE_CACHE,
    FALSE,
    FALSE,
    CommonParameters->OriginalDelays,
    NULL,
    NULL
    );
  //
  // Get the minimum and maximum delta across the nibble
  // so we can use it to determine the maximum value to sweep
  // without violating the tether value for all bits within each nibble
  //
  GetMinMaxDeltaValuePerNibble (CommonParameters);

  if (CommonParameters->FlagsFsm & LeftSideOnly) {
    SideStart = LEFT_EDGE;
    SideStop  = SideStart+1;
  } else if (CommonParameters->FlagsFsm & RightSideOnly) {
    SideStart = RIGHT_EDGE;
    SideStop  = SideStart+1;
  } else {
    SideStart = LEFT_EDGE;
    SideStop  = MAX_EDGES;
  }

  for (SideIndex = SideStart; SideIndex < SideStop; SideIndex++) {
    //
    // Print header
    //
    START_1D_TRACK
    PrintHeaderByStrobes (CommonParameters, FormatResultsCondition);
    STOP_1D_TRACK(PrintL0)
    //
    // Store current edge being swept
    //
    CommonParameters->CurrentEdge = SideIndex;

    //
    // Reset current state to initial for each side (left and right)
    //
    SetMem (CommonParameters->CurrentState, sizeof (CommonParameters->CurrentState), ST_AC_INIT);
    ClearResultsCondition (CommonParameters);

    //
    // Reset strobe state
    //
    ResetStrobeState (CommonParameters);

    do {
      //
      // Execute action based on the current state
      //
      START_1D_TRACK
      ExecuteStateAction (CommonParameters);
      STOP_1D_TRACK(ExecuteStateActionL0)

      if (!AreAllAtActiveState (CommonParameters) ||
          (CommonParameters->FlagsOffsets & DisableFastTracking) != 0) {
        //
        // Get Current delay values after updated values
        // If channel/strobe are at active state there is no need to read the current values
        // as this is already handled in the VerifyAndSetOffset function ('Tag::GetSetOptimize::Active')
        //
        GetSetAcrossChannelsKnobs (CommonParameters, GSM_READ_ONLY | CommonParameters->ReadMode, FALSE, FALSE, CommonParameters->CurrentDelays, NULL, NULL);
      }

      //
      // Execute test
      //
      START_1D_TRACK
      if (ExecuteTest != NULL) {
        (*ExecuteTest) (CommonParameters);
      }
      STOP_1D_TRACK(ExecuteTestL0)
      //
      // Read results
      //
      START_1D_TRACK
      if (ResultProcessing != NULL) {
        (*ResultProcessing) (CommonParameters);
      } else {
        RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_000);
      }
      STOP_1D_TRACK(ResultProcessingL0)

      //
      // Track results based on the current state,
      // first result and previous result
      //
      START_1D_TRACK
      ResultsConditionByState (CommonParameters);
      STOP_1D_TRACK(ResultsConditionByStateL0)

      //
      // Track 1D results
      //
      START_1D_TRACK
      TrackingResults1D (CommonParameters);
      STOP_1D_TRACK(TrackingResults1DL0)

      //
      // Print current state
      //
      START_1D_TRACK
      PrintCurrentStatus (CommonParameters);
      STOP_1D_TRACK(PrintL0)

      //
      // Override results based on the results
      //
      START_1D_TRACK
      OverrideResultConditionByGroup (CommonParameters);
      STOP_1D_TRACK(OverrideResultConditionByGroupL0)

      //
      // Determine the next state
      //
      START_1D_TRACK
      GetNextState (CommonParameters);
      STOP_1D_TRACK(GetNextStateL0)

    } while (!AreAllChannelsAtState (CommonParameters, ST_END));

    //
    // Restore original values for all knobs
    //
    START_1D_TRACK
    RestoreOriginalDelays (CommonParameters);
    STOP_1D_TRACK(RestoreOriginalL0)

    //
    // Error handler after current side is done
    //
    START_1D_TRACK
    if (ErrorHandler != NULL) {
      (*ErrorHandler) (CommonParameters);
    }
    STOP_1D_TRACK(ErrorHandlerL0)

  } // SideIndex loop ...

  //
  // Post processing
  //
  START_1D_TRACK
  if (PostSweep1D != NULL) {
    (*PostSweep1D) (CommonParameters);
  }
  STOP_1D_TRACK(Post1DSweepL0)

  if (CommonParameters->FlagsResults & ResultsRelative) {
    AdjustRelativeResults1D (CommonParameters);
    //
    // Update left/right edges
    //
    UpdateLeftRightEdges (CommonParameters);
  } else {
    AdjustPerBitResults1D (CommonParameters);
  }

  return MRC_STATUS_SUCCESS;
}

/*

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

*/
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
)
{
  UINT8   ChannelIndex;
  UINT8   DimmIndex;
  UINT8   RankIndex;
  UINT8   MaxChDdr;
  UINT32  BackupChannelMask;
  CHANNEL_NVRAM_STRUCT (*ChannelNvList)[MAX_CH];

  MaxChDdr = CommonParameters->MaxChDdr;
  ChannelNvList = GetChannelNvList (CommonParameters->Host, CommonParameters->Socket);

  BackupChannelMask = CommonParameters->ChannelMask;

  for (DimmIndex = 0; DimmIndex < MAX_DIMM; DimmIndex++) {
    for (RankIndex = 0; RankIndex < MAX_RANK_DIMM; RankIndex++) {
      //
      // Restore the original input from the user
      //
      CommonParameters->ChannelMask = BackupChannelMask;

      if ((CommonParameters->RankMask & (1 << (DimmIndex * MAX_RANK_DIMM + RankIndex))) != 0) {
        continue;
      }

      for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {

        if (!CommonParameters->ChannelEnabledLocal[ChannelIndex]) {
          //
          // Mask any channels which are not enabled.
          // Note: This was only done in order ChannelMask is used as
          // a decision maker if ProcessSweep1D needs to be executed or not
          //
          CommonParameters->ChannelMask |= 1 << ChannelIndex;
        }

        CommonParameters->Dimm[ChannelIndex] = DimmIndex;
        CommonParameters->Rank[ChannelIndex] = RankIndex;

        if ((*ChannelNvList)[ChannelIndex].encodedCSMode) {

          CommonParameters->SubRank[ChannelIndex] = RankIndex >> 1;

        } else {

          CommonParameters->SubRank[ChannelIndex] = 0;

        }
        if (IsRankPresent (CommonParameters->Socket, ChannelIndex, DimmIndex, RankIndex)) {

          if (CommonParameters->Level == LrbufLevel && !IsLrdimmPresent (CommonParameters->Socket, ChannelIndex, DimmIndex)) {
            //
            // Mask channels that do not have any LRDIMM installed and LrbufLevel
            //
            CommonParameters->ChannelMask |= 1 << ChannelIndex;
          }

        } else {

          //
          // Mask channels that do not have any Dimm/Rank present
          //
          CommonParameters->ChannelMask |= 1 << ChannelIndex;

        }

      } // ChannelIndex loop ...

      if ((CommonParameters->ChannelMask & CH_BITMASK) != CH_BITMASK) {

        ProcessSweep1D (
          CommonParameters,
          PreSweep1D,
          SetupTest,
          ExecuteTest,
          ErrorHandler,
          PostSweep1D,
          ResultProcessing,
          &Results[DimmIndex * MAX_RANK_DIMM + RankIndex]
          );

      }

    } // RankIndex loop ...
  } // DimmIndex loop ...

  //
  // Restore the original input from the user
  //
  CommonParameters->ChannelMask = BackupChannelMask;

  return MRC_STATUS_SUCCESS;
}

/**

  Function in charge to do the horizontal sweep

  @param[in]      CommonParameters  Pointer to the context information for 2D usage (Pointer to COMMON_2D_PARAMETERS)

  @retval         MRC_STATUS_SUCCESS       1D sweep function was executed without errors

**/
MRC_STATUS
EFIAPI
ProcessHorizontalSweep (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
)
{
  UINT8   FirstChannel;
  INT8    VerticalIndex;
  COMMON_2D_PARAMETERS *CommonParameters2D;
  COMMON_1D_PARAMETERS *CommonParametersV;
  COMMON_1D_PARAMETERS_FOR_2D *CommonParametersH;
  struct strobeMargin   *StrobeResults;
  struct bitMargin      *BitResults;
  struct rankMargin     *RankResults;

  CommonParameters2D = (COMMON_2D_PARAMETERS *) CommonParameters;

  CommonParametersH = &CommonParameters2D->CommonParametersH;
  CommonParametersV = &CommonParameters2D->CommonParametersV;

  CommonParametersH->CommonParameters2D = CommonParameters2D;
  CommonParametersH->IndentationLevel   = CommonParametersV->IndentationLevel + 1;

  StrobeResults = NULL;
  BitResults    = NULL;
  RankResults   = NULL;

  if (CommonParameters2D->StrobeResults != NULL) {
    StrobeResults = RcAllocatePool (sizeof (*StrobeResults));
  }

  if (CommonParameters2D->BitResults != NULL) {
    BitResults    = RcAllocatePool (sizeof (*BitResults));
  }

  if (CommonParameters2D->RankResults != NULL) {
    RankResults   = RcAllocatePool (sizeof (*RankResults));
  }

  ProcessWorker1D (
    (COMMON_1D_PARAMETERS *) CommonParametersH,
    CommonParameters2D->MemSsTypeH,
    CommonParameters2D->SocketH,
    CommonParameters2D->ChannelMaskH,
    CommonParameters2D->RankMaskH,
    CommonParameters2D->SubChannelH,
    CommonParameters2D->DimmRankH,
    CommonParameters2D->LevelH,
    CommonParameters2D->GroupH,
    CommonParameters2D->SignalH,
    CommonParameters2D->StepSizeH,
    CommonParameters2D->DitherValueH,
    CommonParameters2D->OffsetLeftH,
    CommonParameters2D->OffsetRightH,
    CommonParameters2D->LimitOffsetH,
    CommonParameters2D->FlagsOffsetsH,
    CommonParameters2D->FlagsResultsH,
    CommonParameters2D->FlagsFsmH,

    PreSweepHorizontal2D,
    CommonParameters2D->SetupTestH,
    CommonParameters2D->ExecuteTestH,
    CommonParameters2D->ErrorHandlerH,
    CommonParameters2D->PostSweep1DH,
    CommonParameters2D->ResultProcessingH,

    StrobeResults,                                       ///< struct strobeMargin *StrobeResults, OPTIONAL
    BitResults,                                          ///< struct bitMargin    *BitResults,    OPTIONAL
    RankResults,                                         ///< struct rankMargin   *RankResults,   OPTIONAL
                                                         ///<
    NULL,                                                ///< struct strobeMargin StrobeResultsEvenOdd[MaxChunkSides], OPTIONAL
    NULL,                                                ///< struct bitMargin    BitResultsEvenOdd[MaxChunkSides],    OPTIONAL
    NULL                                                 ///< struct rankMargin   RankResultsEvenOdd[MaxChunkSides]    OPTIONAL
    );

  GetFirstValidChannel (CommonParametersV, &FirstChannel);

  VerticalIndex = GetVerticalIndex (
                    CommonParameters2D,
                    FirstChannel
                    );

  CopyResults (
    CommonParameters2D,
    VerticalIndex,

    CommonParameters2D->StrobeResults,
    StrobeResults,

    CommonParameters2D->BitResults,
    BitResults,

    CommonParameters2D->RankResults,
    RankResults
    );

  if (CommonParameters2D->StrobeResults != NULL) {
    RcFreePool (StrobeResults);
  }

  if (CommonParameters2D->BitResults != NULL) {
    RcFreePool (BitResults);
  }

  if (CommonParameters2D->RankResults != NULL) {
    RcFreePool (RankResults);
  }

  return MRC_STATUS_SUCCESS;
}



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

  )
{
  INT16   LimitOffsetV;

  LimitOffsetV = NumberOfSamples * StepSizeV;

  //
  // Vertical parameters
  //
  CommonParameters2D->MaxVerticalEntries  = 2 * NumberOfSamples + 1;
  CommonParameters2D->StrobeResults       = StrobeResults;
  CommonParameters2D->BitResults          = BitResults;
  CommonParameters2D->RankResults         = RankResults;
  //
  // Horizontal input parameters
  //
  CommonParameters2D->MemSsTypeH          = MemSsType;
  CommonParameters2D->SocketH             = Socket;
  CommonParameters2D->ChannelMaskH        = ChannelMask;
  CommonParameters2D->RankMaskH           = RankMask;
  CommonParameters2D->SubChannelH         = SubChannel;
  CommonParameters2D->DimmRankH           = DimmRank;
  CommonParameters2D->LevelH              = LevelH;
  CommonParameters2D->GroupH              = GroupH;
  CommonParameters2D->SignalH             = SignalH;
  CommonParameters2D->StepSizeH           = StepSizeH;
  CommonParameters2D->DitherValueH        = DitherValueH;
  CommonParameters2D->OffsetLeftH         = OffsetLeftH;
  CommonParameters2D->OffsetRightH        = OffsetRightH;
  CommonParameters2D->LimitOffsetH        = LimitOffsetH;
  CommonParameters2D->FlagsOffsetsH       = FlagsOffsetsH;
  CommonParameters2D->FlagsResultsH       = FlagsResultsH;
  CommonParameters2D->FlagsFsmH           = FlagsFsmH;

  CommonParameters2D->PreSweep1DH         = PreSweep1DH;
  CommonParameters2D->SetupTestH          = SetupTestH;
  CommonParameters2D->ExecuteTestH        = ExecuteTestH;
  CommonParameters2D->ErrorHandlerH       = ErrorHandlerH;
  CommonParameters2D->PostSweep1DH        = PostSweep1DH;
  CommonParameters2D->ResultProcessingH   = ResultProcessingH;

  ProcessWorker1D (
    &CommonParameters2D->CommonParametersV,              ///< COMMON_1D_PARAMETERS  *CommonParameters,
    MemSsType,                                           ///< MRC_MST     MemSsType,
    Socket,                                              ///< UINT8       Socket,
    ChannelMask,                                         ///< UINT32      ChannelMask,
    RankMask,                                            ///< UINT32      RankMask,
    SubChannel,                                          ///< UINT8       SubChannel,
    DimmRank,                                            ///< MRC_RT      DimmRank[MAX_CH],
    LevelV,                                              ///< MRC_LT      Level,
    GroupV,                                              ///< MRC_GT      Group,
    SignalV,                                             ///< GSM_CSN     Signal,           OPTIONAL
    StepSizeV,                                           ///< UINT8       StepSize,
    1,                                                   ///< UINT8       DitherValue,
    0,                                                   ///< INT16       OffsetLeft,
    0,                                                   ///< INT16       OffsetRight,
    LimitOffsetV,                                        ///< INT16       LimitOffset,
    LimitSearch,                                         ///< FLAGS_CATEGORY_OFFSETS  FlagsOffsets,
    DefaultResults,                                      ///< FLAGS_CATEGORY_RESULTS  FlagsResults,
    DefaultFsm,                                          ///< FLAGS_CATEGORY_FSM      FlagsFsm,
                                                         ///<
    NULL,                                                ///< Callback1D  PreSweep1D,           OPTIONAL
    NULL,                                                ///< Callback1D  SetupTest,            OPTIONAL
    NULL,                                                ///< Callback1D  ExecuteTest,          OPTIONAL
    NULL,                                                ///< Callback1D  ErrorHandler,         OPTIONAL
    NULL,                                                ///< Callback1D  PostSweep1D,          OPTIONAL
    ProcessHorizontalSweep,                              ///< Callback1D  ResultProcessing,
                                                         ///<
    NULL,                                                ///< struct strobeMargin *StrobeResults, OPTIONAL
    NULL,                                                ///< struct bitMargin    *BitResults,    OPTIONAL
    NULL,                                                ///< struct rankMargin   *RankResults,   OPTIONAL
                                                         ///<
    NULL,                                                ///< struct strobeMargin StrobeResultsEvenOdd[MaxChunkSides], OPTIONAL
    NULL,                                                ///< struct bitMargin    BitResultsEvenOdd[MaxChunkSides],    OPTIONAL
    NULL                                                 ///< struct rankMargin   RankResultsEvenOdd[MaxChunkSides]    OPTIONAL
    );

  PrintLegacyResults2D (CommonParameters2D, StrobeResults, BitResults, RankResults);

  return MRC_STATUS_SUCCESS;

}

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

  )
{
  UINT8   ChannelIndex;
  UINT8   DimmIndex;
  UINT8   RankIndex;
  UINT8   LogicalRank;
  UINT8   MaxChDdr;
  UINT32  BackupChannelMask;
  MRC_RT  DimmRank[MAX_CH];
  BOOLEAN ExecutedDimmRank[MAX_DIMM][MAX_RANK_DIMM];
  PSYSHOST  Host;

  CHANNEL_NVRAM_STRUCT (*ChannelNvList)[MAX_CH];

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();

  ChannelNvList = GetChannelNvList (Host, Socket);

  BackupChannelMask = ChannelMask;

  for (DimmIndex = 0; DimmIndex < MAX_DIMM; DimmIndex++) {
    for (RankIndex = 0; RankIndex < MAX_RANK_DIMM; RankIndex++) {

      ExecutedDimmRank[DimmIndex][RankIndex] = FALSE;

      //
      // Restore the original input from the user
      //
      ChannelMask = BackupChannelMask;

      LogicalRank = GetLogicalRank (Host, Socket, 0, DimmIndex, RankIndex);

      if ((RankMask & (1 << LogicalRank)) != 0) {
        continue;
      }

      for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {

        if (!IsChannelEnabled (Socket, ChannelIndex)) {
          //
          // Mask any channels which are not enabled.
          // Note: This was only done in order ChannelMask is used as
          // a decision maker if ProcessSweep1D needs to be executed or not
          //
          ChannelMask |= 1 << ChannelIndex;
        }

        DimmRank[ChannelIndex].Dimm = DimmIndex;
        DimmRank[ChannelIndex].Rank = RankIndex;

        if (IsRankPresent (Socket, ChannelIndex, DimmIndex, RankIndex)) {

          if (LevelV == LrbufLevel && !IsLrdimmPresent (Socket, ChannelIndex, DimmIndex)) {
            //
            // Mask channels that do not have any LRDIMM installed and LrbufLevel
            //
            ChannelMask |= 1 << ChannelIndex;
          }

        } else {

          //
          // Mask channels that do not have any Dimm/Rank present
          //
          ChannelMask |= 1 << ChannelIndex;

        }

      } // ChannelIndex loop ...

      if ((ChannelMask & CH_BITMASK) != CH_BITMASK) {

        ExecutedDimmRank[DimmIndex][RankIndex] = TRUE;

        ProcessWorker2D (
          CommonParameters2D,
          MemSsType,
          Socket,
          ChannelMask,
          RankMask,
          SubChannel,
          DimmRank,

          LevelV,
          GroupV,
          SignalV,
          StepSizeV,
          NumberOfSamples,

          LevelH,
          GroupH,
          SignalH,
          StepSizeH,
          DitherValueH,
          OffsetLeftH,
          OffsetRightH,
          LimitOffsetH,
          FlagsOffsetsH,
          FlagsResultsH,
          FlagsFsmH,

          PreSweep1DH,           OPTIONAL
          SetupTestH,            OPTIONAL
          ExecuteTestH,          OPTIONAL
          ErrorHandlerH,         OPTIONAL
          PostSweep1DH,          OPTIONAL
          ResultProcessingH,

          StrobeResults, OPTIONAL
          BitResults,    OPTIONAL
          RankResults

          );
      }
    } // RankIndex loop ...
  } // DimmIndex loop ...

  return MRC_STATUS_SUCCESS;

}
