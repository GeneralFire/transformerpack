/** @file
  Mem1DSweep.h

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

#ifndef __MEMSWEEP_H__
#define __MEMSWEEP_H__

#include <ReferenceCodeDataTypes.h>
#include <ReferenceCodeFatalErrors.h>

//
// Core
//
#include <Library/IoAccessLib.h>
#include <Library/TimerLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/RcMemAllocLib.h>
#include <Library/SpdAccessLib.h>
#include <Library/Ddr5CoreLib.h>
#include <Library/MemSweepLib.h>
//
// Silicon
//
#include <Library/MemDdrioIpLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemRoutingLib.h>
#include <Library/MemRcLib.h>
//
// When a delay/result does not depend on knob index
//
#define   NO_KNOB_INDEX   0xFF
//
// Default fine step size
//
#define SWEEP_FINE_STEP_SIZE  1
//
// Jitter GAP definition
//
#define JITTER_GAP    3
//
// Max Dither value
//
#define MAX_DITHER    8191
//
// Profiling
//
#define ENABLE_1D_PROFILING  0
#if ENABLE_1D_PROFILING
  #define START_1D_TRACK  CommonParameters->StartCnt = GetPerformanceCounter ();
  #define STOP_1D_TRACK(Label) \
    CommonParameters->StopCnt  = GetPerformanceCounter (); \
    CommonParameters->Performance1DProfile[Label] += TimeDiff (CommonParameters->StartCnt, CommonParameters->StopCnt, TDIFF_UNIT_US);
#else
  #define START_1D_TRACK
  #define STOP_1D_TRACK(Label)
#endif

typedef enum _FORMAT_TYPE{
  FormatResultsCondition = 0,
  FormatResults1D,
  MaxFormatTypes
} FORMAT_TYPE;

//
// MemSweep1DGetSet.c local APIs
//
/*
  Verify the offset to program on each knob, channel, strobe

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      Mode              Mode to access the register
                                    GSM_READ_CSR: Read register directly instead of cache value
                                    GSM_READ_ONLY: Only read, skip write portion
                                    GSM_WRITE_OFFSET: Use the value as an offset to original
                                    GSM_UPDATE_CACHE: Update cache
  @param[in]      DelayInput        Pointer to delay values

  @retval         MRC_STATUS_SUCCESS       New offset was set correctly
*/
MRC_STATUS
VerifyAndSetOffset (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   Mode,
  IN  OUT    INT16                   (*DelayInput)[MAX_CH][MAX_STROBE]
);

/*
  Get or Set the new offset on each knob, channel, strobe

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      Mode              Mode to access the register
                                    GSM_READ_CSR: Read register directly instead of cache value
                                    GSM_READ_ONLY: Only read, skip write portion
                                    GSM_WRITE_OFFSET: Use the value as an offset to original
                                    GSM_UPDATE_CACHE: Update cache
  @param[in]      UpdateMinMax      Request to update the MinValue and MaxValue
  @param[in]      SameOffsetAll     If the offset to be programmed into the register is the same across knobs, channels
                                    and strobes, them the process is optimize using ALL_STROBES as parameter
  @param[in]      DelayInput        Pointer to delay values
  @param[out]     MinValue          Pointer to update the minimum value
  @param[out]     MaxValue          Pointer to update the maximum value

  @retval         MRC_STATUS_SUCCESS       New offset was set correctly

*/
MRC_STATUS
GetSetAcrossChannelsKnobs (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   Mode,
  IN         BOOLEAN                 UpdateMinMax,
  IN         BOOLEAN                 SameOffsetAll,
  IN  OUT    INT16                   (*DelayInput)[MAX_CH][MAX_STROBE],
      OUT    UINT16                  MinValue[MAX_CH][MAX_STROBE],    OPTIONAL
      OUT    UINT16                  MaxValue[MAX_CH][MAX_STROBE]     OPTIONAL
);

/*
  Get Min/Max bit delta value for each nibble

  @param[in,out]  CommonParameters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       New offset was set correctly

*/
MRC_STATUS
GetMinMaxDeltaValuePerNibble (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
  );

/*

  Return the number of knobs for current context information

  @param[in,out]  CommonParamaters  Pointer to the context information

  @return         Number of knobs used for current context information

*/
UINT8
GetNumberOfKnobsPerGroup (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
);

/*

  Return the number of Max available strobe in the bus, no matter if will be trained or not
  This information is linked directly with the MemSsList

  DDR5/4 SubCh MaxStrobeAvailable
     4    DC          18
     5    DC          10

  @param[in,out]  CommonParamaters  Pointer to the context information

  @return         Number of max available strobes

*/
UINT8
GetMaxStrobeAvailable (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
);

/*

  Return the number of strobes to be printed

  @param[in,out]  CommonParamaters  Pointer to the context information

  @return         Number of strobes to be printed (usually Bytes within a Sub-Channel)

*/
UINT8
GetMaxStrobeToPrint (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  );

/*

  Returns the offset to calculate the second nibble
  within the byte with respect the first nibble

  @param[in,out]  CommonParamaters  Pointer to the context information

  @return         Returns offset to the second nibble

*/
UINT8
GetNextNibbleOffset (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  );

/*

  Special case where a group at rank level get independent meaningful results on each strobe
  WARNING: This function is mutually exclusive with IsResultAtRankLevel,
  so if IsSingleToMultipleDelay returns TRUE IsResultAtRankLevel must return FALSE

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval   TRUE      If the current context involves a delay which is at a rank level,
                      but results are meaningful on each strobe
  @retval   FALSE     Otherwise

*/
BOOLEAN
IsSingleToMultipleDelay (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
);

/*

  Results at rank level (Multi-strobe results are not meaningful)

  WARNING: This function is mutually exclusive with IsSingleToMultipleDelay,
  so if IsResultAtRankLevel returns TRUE IsSingleToMultipleDelay must return FALSE

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval   TRUE      If the current context involves a delay which is at a rank level
  @retval   FALSE     Otherwise

*/
BOOLEAN
IsResultAtRankLevel (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
);

/*

  Results at byte level

  DDR5/4 SubCh MaxStrobeWithinSubCh AreResultsAtByteLevel
     4    DC        18                FALSE
     4    DC         9                TRUE

     5    0/1       10                FALSE
     5    0/1        5                TRUE

     5    ALL        5                TRUE
     5    ALL       10                FALSE

     5    DC         5                TRUE  - BRS
     5    DC        10                FALSE - BRS

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      ChannelIndex      Current channel under test (0-based)

  @retval     TRUE      Current Group/Level should be margin at a Byte Level
  @retval     FALSE     Current Group/Level should be margin at a Nibble Level

*/
BOOLEAN
AreResultsAtByteLevel (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   ChannelIndex
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
SkipExecutionOnStrobe (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   StrobeLinear
);


/*

  This function restores the original value for all delays

  @param[in]      CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       If the delays got restored properly

*/
MRC_STATUS
RestoreOriginalDelays (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
  );

//
// MemSweep1DState.c local APIs
//
/*

  Returns TRUE if all channels/strobes are at specified state

  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      State             FSM to be compared across all channels/strobes

  @retval         TRUE              If all channels/strobes are at ST_END
  @retval         FALSE             Otherwise

*/
BOOLEAN
AreAllChannelsAtState (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         STATES_1D               State
);

/*

  Set an specified state across all channels and strobes

  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      State             FSM to be set across all channels/strobes

*/
VOID
PropagateStateAcrossChannelsStrobes (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         STATES_1D               State
);

/*

  Executes current actions based on the current state for each channel/strobe

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       Function was executed without errors

*/
MRC_STATUS
ExecuteStateAction (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
);

/*

  Define the next state based on the current state for each channel/strobe
  and the passing or failing conditions

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       Function was executed without errors

*/
MRC_STATUS
GetNextState (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
);

/*

  Based on the current result condition this function updates (if required),
  first condition, previous condition and dither value counter

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       Function was executed without errors

*/
MRC_STATUS
ResultsConditionByState (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
);

/*

  This function propagates the 'failing' condition given by a single strobe into logical strobe 0 when
  searching for a 'passing' condition, if the is looking for a failing condition, then this propagates
  the 'passing' condition given by a single strobe into logical strobe 0.
  This function is only applicable when the context is true for a IsSingleToMultipleDelay

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       Function was executed without errors

*/
MRC_STATUS
OverrideResultConditionByGroup (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
);

/*

  This function returns TRUE when FSM is ready to start tracking the passing/failing windows

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      State             Current state under evaluation

  @retval         TRUE              When FSM is ready to start tracking data
  @retval         FALSE             Otherwise

*/
BOOLEAN
IsSamplingState (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         STATES_1D               State
);

/*

  This function returns TRUE when FSM is an active state (any state that is not xx_INIT)

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      State             Current state under evaluation

  @retval         TRUE              When FSM is an active state
  @retval         FALSE             Otherwise

*/
BOOLEAN
IsActiveState (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         STATES_1D               State
);

/*

  This function returns TRUE when FSM for all channel/strobes are in an active state (any state that is not xx_INIT)

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      State             Current state under evaluation

  @retval         TRUE              When FSM for all channel/strobes are in an active state
  @retval         FALSE             Otherwise

*/
BOOLEAN
AreAllAtActiveState (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
);

/*

  This function returns TRUE when FSM is in an state related to the LEFT side

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      State             Current state under evaluation

  @retval         TRUE              When FSM is in an state related to the LEFT side
  @retval         FALSE             Otherwise

*/
BOOLEAN
IsLeftState (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         STATES_1D               State
);

/*

  This function returns TRUE when FSM is in an state related to the LEFT side
  and searching for a passing condition

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      State             Current state under evaluation

  @retval         TRUE              When FSM is in an state related to the LEFT side
                                    and searching for a passing condition
  @retval         FALSE             Otherwise

*/
BOOLEAN
IsLeftSearchPassState (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         STATES_1D               State
);

/*

  This function returns TRUE when FSM is in an state related to the RIGHT side

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      State             Current state under evaluation

  @retval         TRUE              When FSM is in an state related to the RIGHT side
  @retval         FALSE             Otherwise

*/
BOOLEAN
IsRightState (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         STATES_1D               State
);

/*

  This function returns TRUE when FSM is in an state related to the RIGHT side
  and searching for a passing condition

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      State             Current state under evaluation

  @retval         TRUE              When FSM is in an state related to the RIGHT side
                                    and searching for a passing condition
  @retval         FALSE             Otherwise

*/
BOOLEAN
IsRightSearchPassState (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         STATES_1D               State
);

/*

  This function returns TRUE when FSM is in an state searching for a failing condition

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      State             Current state under evaluation

  @retval         TRUE              When FSM is in an state searching for a failing condition
  @retval         FALSE             Otherwise

*/
BOOLEAN
IsFailingState (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         STATES_1D               State
);

/*

  This function returns TRUE when FSM is in an state searching for a passing condition

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      State             Current state under evaluation

  @retval         TRUE              When FSM is in an state searching for a passing condition
  @retval         FALSE             Otherwise

*/
BOOLEAN
IsSearchPassState (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         STATES_1D               State
);

//
// MemSweep1DAllocationHelpers.c local APIs
//
/*

  Return the number of knobs that will be use to reserve the size of some arrays.
  This number may be optimize based on the context information (Group, Level,...)

  @param[in,out]  CommonParamaters  Pointer to the context information

  @return   The number of knobs use to reserve the size of the arrays

*/
UINT8
GetFinalKnobsForNemUsage (
  IN        COMMON_1D_PARAMETERS   *CommonParameters
);

/*

  Return the number of knobs required, considering that each strobe can
  be used to store the value of 1 knob

  @param[in]  KnobsPerGroup  Knobs that current context requires

  @return   The number of knobs required based on the number of strobes

*/
UINT8
GetMinimizedKnobs (
  IN        UINT8         KnobsPerGroup
);

/*

  Return TRUE or FALSE if current Group/Level is intended to minimize the NEM usage

  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test

  @retval         TRUE              Current Group/Level is intended to minimize the NEM usage
  @retval         FALSE             Otherwise

*/
BOOLEAN
MinimizeNemUsage (
  IN        MRC_LT          Level,
  IN        MRC_GT          Group
);

/*

  Releases resources used to store the 1D results

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      Results           Pointer that contains the final results

  @retval         MRC_STATUS_SUCCESS       Resources released successfully

*/
MRC_STATUS
FreePoolForResults1D (
  IN        COMMON_1D_PARAMETERS  *CommonParameters,
  IN  OUT   RESULTS_1D      *Results
);

/*

  Allocate resources for current condition

  @param[in,out]  CommonParamaters  Pointer to the context information

  @return   Pointer to memory reserved
*/
VOID *
AllocatePoolForResultCondition (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
);

/*

  Allocate resources to store the Delay values

  @param[in,out]  CommonParamaters  Pointer to the context information

  @return   Pointer to memory reserved
*/
VOID *
AllocatePoolForDelays (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
);

/**

  Copy results to the global

  @param[in]      CommonParameters2D          Pointer to the context information that includes vertical and horizontal axis
  @param[in]      VerticalSampleIndex         Current vertical index (only used for 2D sweep), for 1D sweep always use 0
  @param[out]     StrobeResultsDestination    Pointer to return the results at strobe level (this array must be able to holds a valid entry for each vertical sampling point)
  @param[in]      StrobeResultsSource         Pointer that contains the result for current entry
  @param[out]     BitResultsDestination       Pointer to return the results at bit level (this array must be able to holds a valid entry for each vertical sampling point)
  @param[in]      BitResultsSource            Pointer that contains the result for current entry
  @param[out]     RankResultsDestination      Pointer to return the results at rank level (this array must be able to holds a valid entry for each vertical sampling point)
  @param[in]      RankResultsSource           Pointer that contains the result for current entry

**/
VOID
CopyResults (
  IN        COMMON_2D_PARAMETERS    *CommonParameters2D,
  IN        UINT8                   VerticalSampleIndex,
      OUT   struct strobeMargin     *StrobeResultsDestination,  OPTIONAL
  IN        struct strobeMargin     *StrobeResultsSource,       OPTIONAL
      OUT   struct bitMargin        *BitResultsDestination,     OPTIONAL
  IN        struct bitMargin        *BitResultsSource,          OPTIONAL
      OUT   struct rankMargin       *RankResultsDestination,    OPTIONAL
  IN        struct rankMargin       *RankResultsSource          OPTIONAL
);
//
// MemSweep1DHelpers.c local APIs
//
/*

  Clear the 1D results


  @param[in,out]  CommonParamaters  Pointer to the context information

*/
VOID
ClearResults1D (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
);

/*

  Clear current result condition

  @param[in,out]  CommonParamaters  Pointer to the context information

*/
VOID
ClearResultsCondition (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
);


/*

  Provide the information about the minimum and maximum dither value across the
  bits for the specified knob, channel and logical strobe

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      KnobIndex         Current knob under test (0-based)
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      StrobeLogical     Current logical strobe under test (0-based)
  @param[in]      MinDither         Pointer to return the minimum dither value across bits
  @param[in]      MaxDither         Pointer to return the maximum dither value across bits

  @retval         MRC_STATUS_SUCCESS       Executed without issues

*/
MRC_STATUS
GetMinMaxDitherCounterAcrossBits (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   KnobIndex,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   StrobeLogical,
      OUT    UINT16                  *MinDither,
      OUT    UINT16                  *MaxDither
);

/*

  Zeroes the entire array provided as input parameter

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      DelayInput        Pointer to delay values

*/
VOID
ClearDelay (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN  OUT    INT16                   (*DelayInput)[MAX_CH][MAX_STROBE]
);

/*

  Check if all elements for current array is zeroed or not

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      DelayInput        Pointer to delay values

  @retval         TRUE              If all elements are zeroed
  @retval         FALSE             Otherwise

*/
BOOLEAN
IsDelayZero (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN  OUT    INT16                   (*DelayInput)[MAX_CH][MAX_STROBE]
);

/*

  Zeroes all strobe states

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS     Executed correctly

*/
MRC_STATUS
ResetStrobeState (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
);

/*

  Return TRUE or FALSE of limit has been reached for specific channel and logical strobe

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      StrobeLogical     Current logical strobe under test (0-based)

  @retval         TRUE              If current channel/strobe reached the limit
  @retval         FALSE             Otherwise

*/
BOOLEAN
IsStrobeAtLimit (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   StrobeLogical
);

/*

  Sets the limit for an specific channel and logical strobe

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      StrobeLogical     Current logical strobe under test (0-based)

*/
VOID
SetStrobeLimit (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   StrobeLogical
);

/*

  Clear the limit for an specific channel and logical strobe

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      StrobeLogical     Current logical strobe under test (0-based)

*/
VOID
ClearStrobeLimit (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   StrobeLogical
);

/**

  Zero all legacy structure to store margins

  @param[in]      CommonParameters  Pointer to the context information
  @param[in]      DimmRank          Structure to specify current dimm and rank under test (0-based)
  @param[out]     StrobeResults     Pointer to return the results at strobe level
  @param[out]     BitResults        Pointer to return the results at bit level
  @param[out]     RankResults       Pointer to return the results at rank level

  @param[out]     StrobeResultsEvenOdd  Pointer to return the results at strobe level for EVEN/ODD
  @param[out]     BitResultsEvenOdd     Pointer to return the results at bit level for EVEN/ODD
  @param[out]     RankResultsEvenOdd    Pointer to return the results at rank level for EVEN/ODD

**/
VOID
ClearLegacyResults (
  IN        COMMON_1D_PARAMETERS      *CommonParameters,
  IN        MRC_RT                    DimmRank[MAX_CH],
      OUT   struct strobeMargin       *StrobeResults, OPTIONAL
      OUT   struct bitMargin          *BitResults,    OPTIONAL
      OUT   struct rankMargin         *RankResults,   OPTIONAL

      OUT   struct strobeMargin       StrobeResultsEvenOdd[MaxChunkSides], OPTIONAL
      OUT   struct bitMargin          BitResultsEvenOdd[MaxChunkSides],    OPTIONAL
      OUT   struct rankMargin         RankResultsEvenOdd[MaxChunkSides]    OPTIONAL
  );

/**

  Function returns the bit index within a full channel.
  The bit index range from 0 to MAX_BITS-1.

  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      SubChannelIndex   Current subchannel under test (0-based)
  @param[in]      StrobeLogical     Current strobe under test (0-based)
  @param[in]      BitIndex          Current bit under test (0-based)

  @return BitIndex within the channel

**/
UINT8
EFIAPI
GetBitIndexWithinChannel (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   SubChannelIndex,
  IN         UINT8                   StrobeLogical,
  IN         UINT8                   BitIndex
  );

/*

  Returns the index that matches the correct location for current Delta Offset (Current Delay Value - Original Delay Value) in the Results2D array

  @param[in]      CommonParameters2D  Pointer to the context information that includes vertical and horizontal axis
  @param[in]      ChannelIndex      Current channel under test (0-based)

  @retval         MRC_STATUS_SUCCESS

*/
INT8
EFIAPI
GetVerticalIndex (
  IN         COMMON_2D_PARAMETERS    *CommonParameters2D,
  IN         UINT8          ChannelIndex
  );

/**

  Return the first valid channel (enabled and not masked out) for current Dimm/Rank

  @param[in]     CommonParameters    Pointer to the context information
  @param[out]    FirstChannel        Return the first valid channel

  @retval        MRC_STATUS_SUCCESS  First Channel was found
  @retval        MRC_STATUS_FAILURE  No valid channels detected

**/
MRC_STATUS
GetFirstValidChannel (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
      OUT    UINT8          *FirstChannel
  );

/*

  Provides the initial offset for left or right
  Only applicable when
  FLAGS_CATEGORY_RESULTS.ResultsRelative is set
  FLAGS_CATEGORY_RESULTS.UseOffsetAsAbsolute is cleared
  FLAGS_CATEGORY_OFFSETS.StartOnPassingEdges is set

  @param[in]      CommonParameters  Pointer to the context information
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      StrobeLogical     Current logical strobe under test (0-based)
  @param[in]      OffsetLeftCached  Pointer to specify the current Left Offset
  @param[in]      OffsetRightCached Pointer to specify the current Right Offset

  @retval         MRC_STATUS_SUCCESS  Values for Left and Right were successfully retrieved
  @retval         MRC_STATUS_INVALID_PARAMETER  Invalid pointers for OffsetLeftCached or OffsetRightCached
  @retval         MRC_STATUS_UNSUPPORTED        FLAGS_CATEGORY_RESULTS.ResultsRelative is not set
  @retval         MRC_STATUS_UNSUPPORTED        FLAGS_CATEGORY_OFFSETS.StartOnPassingEdges is not set
  @retval         MRC_STATUS_UNSUPPORTED        FLAGS_CATEGORY_RESULTS.UseOffsetAsAbsolute
*/
MRC_STATUS
GetOffsetLeftRight (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   StrobeLogical,
      OUT    INT16                   *OffsetLeftCached,
      OUT    INT16                   *OffsetRightCached
);

/*

  Updates the initial offset for left or right
  Only applicable when
  FLAGS_CATEGORY_RESULTS.ResultsRelative is set
  FLAGS_CATEGORY_RESULTS.UseOffsetAsAbsolute is cleared
  FLAGS_CATEGORY_OFFSETS.StartOnPassingEdges is set

  @param[in]      CommonParameters  Pointer to the context information
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      StrobeLogical     Current logical strobe under test (0-based)
  @param[in]      OffsetLeftCached  Value to specify the current Left Offset
  @param[in]      OffsetRightCached Value to specify the current Right Offset

  @retval         MRC_STATUS_SUCCESS  Values for Left and Right were successfully stored
  @retval         MRC_STATUS_UNSUPPORTED        FLAGS_CATEGORY_RESULTS.ResultsRelative is not set
  @retval         MRC_STATUS_UNSUPPORTED        FLAGS_CATEGORY_OFFSETS.StartOnPassingEdges is not set
  @retval         MRC_STATUS_UNSUPPORTED        FLAGS_CATEGORY_RESULTS.UseOffsetAsAbsolute

*/
MRC_STATUS
SetOffsetLeftRight (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   StrobeLogical,
  IN         INT16                   OffsetLeftCached,
  IN         INT16                   OffsetRightCached
);

/*

  Update the left/right edges for all valid channels, strobes and dimm/rank

  @param[in]      CommonParameters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS  Values for Left and Right were successfully updated
  @retval         MRC_STATUS_UNSUPPORTED        FLAGS_CATEGORY_RESULTS.ResultsRelative is not set
  @retval         MRC_STATUS_UNSUPPORTED        FLAGS_CATEGORY_OFFSETS.StartOnPassingEdges is not set
  @retval         MRC_STATUS_UNSUPPORTED        FLAGS_CATEGORY_RESULTS.UseOffsetAsAbsolute

*/
MRC_STATUS
UpdateLeftRightEdges (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
  );

//
// MemSweep1DResultsHelpers.c local APIs
//
/*

  Keeps track of the delay which is passing for each knob/channel/strobe and stores it in the Results
  Also it keeps track at a bit level if FLAGS_CATEGORY_RESULTS.ResultsPerBit is set

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       Function was executed without errors

*/
MRC_STATUS
TrackingResults1D (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
);

/*

  Adjust the final Results 1D based on the dither value

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      StrobeLogical     Current logical strobe under test (0-based)
  @param[in]      NextState         Next state for current channel/strobe

  @retval         MRC_STATUS_SUCCESS       Function was executed without errors
*/
MRC_STATUS
AdjustDitherResults1D (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   StrobeLogical,
  IN         STATES_1D               NextState
);

/*

  Adjust the final Results 1D based relative to the original value

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       Function was executed without errors

*/
MRC_STATUS
AdjustRelativeResults1D (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
  );

/*

  Adjust the per bit final Results 1D.

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       Function was executed without errors

*/
MRC_STATUS
AdjustPerBitResults1D (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
  );

/*

  Returns the value that is 'considered' a passing condition for current channel, knob, logical strobe and bit (embedded in ResultConditionPtr)

  @param[in,out]  CommonParamaters    Pointer to the context information
  @param[in]      ResultConditionPtr  Pointer to the current results condition which is related to the channel, knob, logical strobe and bit


  @return  The value considered a 'passing' condition

*/
RESULTS_1D_DEFINITION
GetPassingCondition (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         RESULTS_1D_CONDITION    *ResultConditionPtr
);

/*

  Returns the value that is 'considered' a failing condition for current channel, knob, logical strobe and bit (embedded in ResultConditionPtr)

  @param[in,out]  CommonParamaters    Pointer to the context information
  @param[in]      ResultConditionPtr  Pointer to the current results condition which is related to the channel, knob, logical strobe and bit


  @return  The value considered a 'failing' condition

*/
RESULTS_1D_DEFINITION
GetFailingCondition (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         RESULTS_1D_CONDITION    *ResultConditionPtr
);

/*

  Returns TRUE if the nibble under test passed on current knob

  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      KnobIndex         Current knob under test (0-based)
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      NibbleLogical     Current logical nibble under test (0-based)

  @retval         TRUE              If the strobe under test passed on current knob
  @retval         FALSE             Otherwise

*/
BOOLEAN
NibblePassOnCurrentKnob (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   KnobIndex,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   NibbleLogical
);

/*

  Returns TRUE if the strobe under test passed on current knob


  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      KnobIndex         Current knob under test (0-based)
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      StrobeLogical     Current logical strobe under test (0-based)

  @retval         TRUE              If the strobe under test passed on current knob
  @retval         FALSE             Otherwise

*/
BOOLEAN
StrobePassOnCurrentKnob (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   KnobIndex,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   StrobeLogical
);

/*

  Returns TRUE if the strobe under test passed on all knobs


  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      StrobeLogical     Current logical strobe under test (0-based)

  @retval         TRUE              If the strobe under test passed on all knobs
  @retval         FALSE             Otherwise

*/
BOOLEAN
StrobePassAcrossAllKnobs (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   StrobeLogical
);

/*

  Returns TRUE if any bit within the strobe under test failed across all its knobs


  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      StrobeLogical     Current logical strobe under test (0-based)

  @retval         TRUE              If any bit within the strobe under test failed across all its knobs
  @retval         FALSE             Otherwise

*/
BOOLEAN
AnyBitFailAcrossAllKnobs (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   StrobeLogical
);

/*

  Returns TRUE if all bits within the strobe under test failed across all its knobs

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      StrobeLogical     Current logical strobe under test (0-based)

  @retval         TRUE              If all bits within the strobe under test failed across all its knobs
  @retval         FALSE             Otherwise

*/
BOOLEAN
AllBitFailAcrossAllKnobs (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   StrobeLogical
);

/*

  Detects if any bit within the subchannel failed

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      SubChannelIndex   Current sub-channel under test (0-based)

  @retval         TRUE              If any bit within the sub-channel failed
  @retval         FALSE             Otherwise

*/
BOOLEAN
IsSubChannelFailed (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   SubChannelIndex
  );

/*

  Returns TRUE if Dither counter matches expected Dither value
  on any bit within the strobe and in ALL knobs

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      StrobeLogical     Current logical strobe under test (0-based)

  @retval         TRUE              If Dither counter matches expected Dither value
                                    on any bit within the strobe and in ALL knobs
  @retval         FALSE             Otherwise


*/
BOOLEAN
AnyBitMatchesDither (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   StrobeLogical
);

/*

  Returns TRUE if Dither counter matches Dither value expected
  on all bit within the strobe and in all its knobs

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      StrobeLogical     Current logical strobe under test (0-based)

  @retval         TRUE              If Dither counter matches expected Dither value
                                    on all bit within the strobe and in ALL knobs
  @retval         FALSE             Otherwise

*/
BOOLEAN
AllBitMatchesDither (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   StrobeLogical
);

/*

  Propagate results from Source Logical Strobe to Target Logical Strobe


  @param[in,out]  CommonParamaters    Pointer to the context information
  @param[in]      KnobIndex           Current knob under test (0-based)
  @param[in]      ChannelIndex        Current channel under test (0-based)
  @param[in]      SourceStrobeLogical Source logical strobe(0-based)
  @param[in]      TargetStrobeLogical Target logical strobe(0-based)

*/
VOID
PropagateErrorsByState (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   KnobIndex,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   SourceStrobeLogical,
  IN         UINT8                   TargetStrobeLogical
  );

/**

  Setup the value for N and P margins for current strobe logical for the specified channel/subchannel/logical rank

  @param[in,out]  CommonParameters    Pointer to the context information
  @param[in]      VerticalSampleIndex Current vertical index (only used for 2D sweep), for 1D sweep always use 0
  @param[in]      KnobIndex           Current knob under test (0-based)
  @param[in]      ChannelIndex        Current channel under test (0-based)
  @param[in]      LogicalRank         Current logical rank under test (0-based within the channel)
  @param[in]      SubChannelIndex     Current sub-channel under test (0-based)
  @param[in]      StrobeLogical       Current logical strobe under test (0-based)
  @param[in]      UsePrevious         TRUE: When previous value need to be considered
                                            For N margin: MAX (PreviousN, CurrentN)
                                            For P margin: MIN (PreviousP, CurrentP)
                                      FALSE: Only current value is considered.
  @param[in]      CurrentN            Current value for N side
  @param[in]      CurrentP            Current value for P side
  @param[out]     StrobeResults         Pointer to return the results at strobe level
  @param[out]     StrobeResultsEvenOdd  Pointer to return the results at strobe level for EVEN/ODD

**/
VOID
SetStrobeLegacyResult (
  IN        COMMON_1D_PARAMETERS      *CommonParameters,
  IN        UINT8                     VerticalSampleIndex,
  IN        UINT8                     KnobIndex,
  IN        UINT8                     ChannelIndex,
  IN        UINT8                     LogicalRank,
  IN        UINT8                     SubChannelIndex,
  IN        UINT8                     StrobeLogical,
  IN        BOOLEAN                   UsePrevious,
  IN        INT16                     CurrentN,
  IN        INT16                     CurrentP,
    OUT    struct strobeMargin        *StrobeResults, OPTIONAL
    OUT    struct strobeMargin        StrobeResultsEvenOdd[MaxChunkSides] OPTIONAL
  );

/**

  Setup the value for N and P margins for current bit for the specified channel/subchannel/logical rank/strobe logical

  @param[in,out]  CommonParameters    Pointer to the context information
  @param[in]      VerticalSampleIndex Current vertical index (only used for 2D sweep), for 1D sweep always use 0
  @param[in]      KnobIndex           Current knob under test (0-based)
  @param[in]      ChannelIndex        Current channel under test (0-based)
  @param[in]      LogicalRank         Current logical rank under test (0-based within the channel)
  @param[in]      SubChannelIndex     Current sub-channel under test (0-based)
  @param[in]      StrobeLogical       Current logical strobe under test (0-based)
  @param[in]      BitIndex            Current bit under test (0-based)
  @param[in]      UsePrevious         TRUE: When previous value need to be considered
                                            For N margin: MAX (PreviousN, CurrentN)
                                            For P margin: MIN (PreviousP, CurrentP)
                                      FALSE: Only current value is considered.
  @param[in]      CurrentN            Current value for N side
  @param[in]      CurrentP            Current value for P side
  @param[out]     BitResults          Pointer to return the results at bit level
  @param[out]     BitResultsEvenOdd     Pointer to return the results at bit level for EVEN/ODD

**/
VOID
SetBitLegacyResult (
  IN        COMMON_1D_PARAMETERS      *CommonParameters,
  IN        UINT8                     VerticalSampleIndex,
  IN        UINT8                     KnobIndex,
  IN        UINT8                     ChannelIndex,
  IN        UINT8                     LogicalRank,
  IN        UINT8                     SubChannelIndex,
  IN        UINT8                     StrobeLogical,
  IN        UINT8                     BitIndex,
  IN        BOOLEAN                   UsePrevious,
  IN        INT16                     CurrentN,
  IN        INT16                     CurrentP,
      OUT   struct bitMargin          *BitResults,    OPTIONAL
      OUT   struct bitMargin          BitResultsEvenOdd[MaxChunkSides]    OPTIONAL
);

/**

  Convert results to legacy structures

  @param[in,out]  CommonParameters    Pointer to the context information
  @param[in]      Results             Pointer that contains the final results

  @param[out]     StrobeResults       Pointer to return the results at strobe level
  @param[out]     BitResults          Pointer to return the results at bit level
  @param[out]     RankResults         Pointer to return the results at rank level

  @param[out]     StrobeResultsEvenOdd  Pointer to return the results at strobe level for EVEN/ODD
  @param[out]     BitResultsEvenOdd     Pointer to return the results at bit level for EVEN/ODD
  @param[out]     RankResultsEvenOdd    Pointer to return the results at rank level for EVEN/ODD

  @retval         MRC_STATUS_SUCCESS  Results were properly converted

**/
MRC_STATUS
ConvertResults1D (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         RESULTS_1D              *Results1D,
      OUT    struct strobeMargin     *StrobeResults, OPTIONAL
      OUT    struct bitMargin        *BitResults,    OPTIONAL
      OUT    struct rankMargin       *RankResults,   OPTIONAL
      OUT    struct strobeMargin     StrobeResultsEvenOdd[MaxChunkSides], OPTIONAL
      OUT    struct bitMargin        BitResultsEvenOdd[MaxChunkSides],    OPTIONAL
      OUT    struct rankMargin       RankResultsEvenOdd[MaxChunkSides]    OPTIONAL
  );
//
// MemSweep1DDisplay.c local APIs
//
/*
  Print all strobes in the header

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      FormatType        Format to be used

*/
VOID
PrintHeaderByStrobes (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         FORMAT_TYPE             FormatType
);

/*

  Print status for all strobes

  @param[in,out]  CommonParamaters  Pointer to the context information

*/
VOID
PrintCurrentStatus (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
);

/**

  Print the data collected in legacy results structures on channel, strobe and rank basis

  @param[in,out]  CommonParameters  Pointer to the context information

  @param[out]     StrobeResults     Pointer to return the results at strobe level
  @param[out]     BitResults        Pointer to return the results at bit level
  @param[out]     RankResults       Pointer to return the results at rank level

**/
VOID
PrintLegacyResults (
  IN        COMMON_1D_PARAMETERS      *CommonParameters,
  IN        UINT8                     KnobIndex,
      OUT   struct strobeMargin       *StrobeResults, OPTIONAL
      OUT   struct bitMargin          *BitResults,    OPTIONAL
      OUT   struct rankMargin         *RankResults    OPTIONAL
);

/**

  Print the data collected in legacy results structures on channel, strobe and rank basis for 2D sweeps

  @param[in,out]  CommonParameters2D Pointer to the context information that includes vertical and horizontal axis

  @param[in]      StrobeResults     Pointer to return the results at strobe level (this array must be able to holds a valid entry for each vertical sampling point)
  @param[in]      BitResults        Pointer to return the results at bit level (this array must be able to holds a valid entry for each vertical sampling point)
  @param[in]      RankResults       Pointer to return the results at rank level (this array must be able to holds a valid entry for each vertical sampling point)

**/
VOID
PrintLegacyResults2D (
  IN        COMMON_2D_PARAMETERS      *CommonParameters2D,
      OUT   struct strobeMargin       *StrobeResults, OPTIONAL
      OUT   struct bitMargin          *BitResults,    OPTIONAL
      OUT   struct rankMargin         *RankResults    OPTIONAL
  );
//
// MemSweep1DRcSim.c local APIs
//
/*

  Injects passing/failing conditions for different strobes
  The passing/failing is also modulated based on channel/knob/rank

  @param[in]  CommonParametersH   Pointer to the context information for horizontal sweep
  @param[in]  CommonParameters2D  Pointer to the context information for horizontal and vertical sweeps

  @retval   MRC_STATUS_SUCCESS    Results were overriden correctly for RcSim environment

*/
MRC_STATUS
ResultProcessingAdvanceTargetRcSim (
  IN        COMMON_1D_PARAMETERS    *CommonParametersH,
  IN        COMMON_2D_PARAMETERS    *CommonParameters2D    OPTIONAL
);

/*

  Callback to be call always when the horizontal sweep will be executed in a 2D sweep

  @param[in]      CommonParameters  Pointer to the context information for 2D usage (Pointer to COMMON_1D_PARAMETERS_FOR_2D)

  @retval         MRC_STATUS_SUCCESS       Callback function executed properly

*/
MRC_STATUS
EFIAPI
PreSweepHorizontal2D (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  );

#endif // __MEMSWEEP_H__
