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

*/
struct baseMargin *
GetResultPtrAtStrobeLevel (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   KnobIndex,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   StrobeLogical,
  IN         RESULTS_1D              *Results
)
{
  struct baseMargin *ResultPtrAtStrobe;

  if (MinimizeNemUsage (CommonParameters->Level, CommonParameters->Group)) {
    ResultPtrAtStrobe = &Results->ResultsPerStrobe[KnobIndex / MAX_STROBE][ChannelIndex][KnobIndex % MAX_STROBE];
  } else {
    ResultPtrAtStrobe = &Results->ResultsPerStrobe[KnobIndex][ChannelIndex][StrobeLogical];
  }

  return ResultPtrAtStrobe;
}

/*

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

*/
struct baseMargin *
GetResultPtrAtBitLevel (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   KnobIndex,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   StrobeLogical,
  IN         UINT8                   BitIndex,
  IN         RESULTS_1D              *Results
)
{
  struct baseMargin *ResultPtrAtBit;

  if (MinimizeNemUsage (CommonParameters->Level, CommonParameters->Group)) {
    if (Results->ResultsPerBit == NULL) {

      ResultPtrAtBit = NULL;

    } else {

      ResultPtrAtBit = &Results->ResultsPerBit[KnobIndex / MAX_STROBE][ChannelIndex][KnobIndex % MAX_STROBE][BitIndex];

    }

  } else {
    if (Results->ResultsPerBit == NULL) {

      ResultPtrAtBit = NULL;

    } else {

      ResultPtrAtBit = &Results->ResultsPerBit[KnobIndex][ChannelIndex][StrobeLogical][BitIndex];

    }

  }

  return ResultPtrAtBit;
}

/*

  Clear the 1D results


  @param[in,out]  CommonParamaters  Pointer to the context information

*/
VOID
ClearResults1D (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
)
{
  UINT8   KnobIndex;
  UINT8   FinalKnobs;

  FinalKnobs = GetFinalKnobsForNemUsage (CommonParameters);

  for (KnobIndex = 0; KnobIndex < FinalKnobs; KnobIndex++) {

    SetMem (&CommonParameters->Results->ResultsPerStrobe[KnobIndex][0][0], sizeof (*CommonParameters->Results->ResultsPerStrobe), 0xFF);

    if (CommonParameters->Results->ResultsPerBit != NULL) {
      SetMem (CommonParameters->Results->ResultsPerBit[KnobIndex], sizeof (*CommonParameters->Results->ResultsPerBit), 0xFF);
    }
  }
}

/*

  Clear current result condition

  @param[in,out]  CommonParamaters  Pointer to the context information

*/
VOID
ClearResultsCondition (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
)
{
  UINT8   FinalKnobs;

  FinalKnobs = GetFinalKnobsForNemUsage (CommonParameters);

  SetMem (CommonParameters->ResultCondition, FinalKnobs * sizeof (*CommonParameters->ResultCondition), PASS_CONDITION);
}

/*

  Returns the pointer to the current results condition which is related to the channel, knob, logical strobe and bit specified

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      KnobIndex         Current knob under test (0-based)
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      StrobeLogical     Current logical strobe under test (0-based)
  @param[in]      BitIndex          Current bit under test (0-based)

  @return  Pointer to &ResultCondition[KnobIndex][ChannelIndex][StrobeLogical][BitIndex]

*/
RESULTS_1D_CONDITION *
GetResultConditionPtr (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   KnobIndex,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   StrobeLogical,
  IN         UINT8                   BitIndex
)
{
  RESULTS_1D_CONDITION *ResultConditionPtr;

  if (MinimizeNemUsage (CommonParameters->Level, CommonParameters->Group)) {
    ResultConditionPtr = &CommonParameters->ResultCondition[KnobIndex / MAX_STROBE][ChannelIndex][KnobIndex % MAX_STROBE][BitIndex];
  } else {
    ResultConditionPtr = &CommonParameters->ResultCondition[KnobIndex][ChannelIndex][StrobeLogical][BitIndex];
  }

  return ResultConditionPtr;
}

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
)
{
  UINT8   BitIndex;
  RESULTS_1D_CONDITION  *ResultConditionPtr;

  *MinDither = 0xFFFF;
  *MaxDither = 0x0000;

  for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {

    ResultConditionPtr = GetResultConditionPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, BitIndex);

    if (ResultConditionPtr->DitherCounter < *MinDither) {
      *MinDither = ResultConditionPtr->DitherCounter;
    }

    if (ResultConditionPtr->DitherCounter > *MaxDither) {
      *MaxDither = ResultConditionPtr->DitherCounter;
    }

  }

  return MRC_STATUS_SUCCESS;
}
/*

  Zeroes the entire array provided as input parameter

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      DelayInput        Pointer to delay values

*/
VOID
ClearDelay (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN  OUT    INT16                   (*DelayInput)[MAX_CH][MAX_STROBE]
)
{
  UINT8   FinalKnobs;

  FinalKnobs = GetFinalKnobsForNemUsage (CommonParameters);

  SetMem (DelayInput, FinalKnobs * sizeof (*DelayInput), 0);
}

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
)
{
  UINT8   FinalKnobs;

  FinalKnobs = GetFinalKnobsForNemUsage (CommonParameters);

  return IsZeroBuffer (DelayInput, FinalKnobs * sizeof (*DelayInput));
}

/*

  Returns the pointer to input delay which is related to the channel, knob and logical strobe specified

  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      KnobIndex         Current knob under test (0-based)
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      StrobeLogical     Current logical strobe under test (0-based)
  @param[in]      DelayInput        Pointer to delay values

  @return  Pointer to &DelayInput[KnobIndex][ChannelIndex][StrobeLogical]

*/
INT16 *
GetDelayPtr (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   KnobIndex,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   StrobeLogical,
  IN         INT16                   (*DelayInput)[MAX_CH][MAX_STROBE]
)
{
  INT16 *PtrVal;

  if (ChannelIndex >= MAX_CH) {
    RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_004);
    return &DelayInput[0][0][0];
  }

  if (MinimizeNemUsage (CommonParameters->Level, CommonParameters->Group)) {
    PtrVal = &DelayInput[KnobIndex / MAX_STROBE][ChannelIndex][KnobIndex % MAX_STROBE];
  } else {
    PtrVal = &DelayInput[KnobIndex][ChannelIndex][StrobeLogical];
  }

  return PtrVal;
}

/*

  Zeroes all strobe states

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS     Executed correctly

*/
MRC_STATUS
ResetStrobeState (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
)
{

  SetMem (CommonParameters->StrobeLimit, sizeof (CommonParameters->StrobeLimit), 0);

  return MRC_STATUS_SUCCESS;
}

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
)
{
  if (ChannelIndex >= MAX_CH) {
    RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_005);
    return FALSE;
  }
  return (CommonParameters->StrobeLimit[ChannelIndex] & (1 << StrobeLogical)) ? TRUE : FALSE;
}

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
)
{
  CommonParameters->StrobeLimit[ChannelIndex] |= 1 << StrobeLogical;
}

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
)
{
  CommonParameters->StrobeLimit[ChannelIndex] &= ~(1 << StrobeLogical);
}

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
  IN       COMMON_1D_PARAMETERS       *CommonParameters,
  IN        MRC_RT                    DimmRank[MAX_CH],
      OUT   struct strobeMargin       *StrobeResults, OPTIONAL
      OUT   struct bitMargin          *BitResults,    OPTIONAL
      OUT   struct rankMargin         *RankResults,   OPTIONAL

      OUT   struct strobeMargin       StrobeResultsEvenOdd[MaxChunkSides], OPTIONAL
      OUT   struct bitMargin          BitResultsEvenOdd[MaxChunkSides],    OPTIONAL
      OUT   struct rankMargin         RankResultsEvenOdd[MaxChunkSides]    OPTIONAL
  )
{
  UINT8   ChunkIndex;
  UINT8   ChannelIndex;
  UINT8   LogicalRank;
  UINT8   StrobeIndex;
  UINT8   BitIndex;


  for (ChannelIndex = 0; ChannelIndex < CommonParameters->MaxChDdr; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    LogicalRank = GetLogicalRank (
                    CommonParameters->Host,
                    CommonParameters->Socket,
                    ChannelIndex,
                    CommonParameters->Dimm[ChannelIndex],
                    CommonParameters->Rank[ChannelIndex]
                    );

    if (LogicalRank >= MAX_RANK_CH) {
      RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_025);
      LogicalRank = 0;
    }

    for (StrobeIndex = 0; StrobeIndex < MAX_STROBE; StrobeIndex++) {
      if (StrobeResults != NULL) {
        StrobeResults->strobe[ChannelIndex][LogicalRank][StrobeIndex].n = 0;
        StrobeResults->strobe[ChannelIndex][LogicalRank][StrobeIndex].p = 0;
      }
      if (StrobeResultsEvenOdd != NULL) {
        for (ChunkIndex = 0; ChunkIndex < MaxChunkSides; ChunkIndex++) {
          StrobeResultsEvenOdd[ChunkIndex].strobe[ChannelIndex][LogicalRank][StrobeIndex].n = 0;
          StrobeResultsEvenOdd[ChunkIndex].strobe[ChannelIndex][LogicalRank][StrobeIndex].p = 0;
        }
      }
    }

    for (BitIndex = 0; BitIndex < MAX_BITS; BitIndex++) {
      if (BitResults != NULL) {
        BitResults->bits[ChannelIndex][LogicalRank][BitIndex].n = 0;
        BitResults->bits[ChannelIndex][LogicalRank][BitIndex].p = 0;
      }
      if (BitResultsEvenOdd != NULL) {
        for (ChunkIndex = 0; ChunkIndex < MaxChunkSides; ChunkIndex++) {
          BitResultsEvenOdd[ChunkIndex].bits[ChannelIndex][LogicalRank][BitIndex].n = 0;
          BitResultsEvenOdd[ChunkIndex].bits[ChannelIndex][LogicalRank][BitIndex].p = 0;
        }
      }
    }

    if (RankResults != NULL) {
      RankResults->rank[ChannelIndex][LogicalRank].n = 0;
      RankResults->rank[ChannelIndex][LogicalRank].p = 0;
    }

    if (RankResultsEvenOdd != NULL) {
      for (ChunkIndex = 0; ChunkIndex < MaxChunkSides; ChunkIndex++) {
        RankResultsEvenOdd[ChunkIndex].rank[ChannelIndex][LogicalRank].n = 0;
        RankResultsEvenOdd[ChunkIndex].rank[ChannelIndex][LogicalRank].p = 0;
      }
    }

  } // ChannelIndex

}
/*

  Skip channel execution

  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      ChannelIndex      Current channel under test (0-based)

  @retval         TRUE              Skips channel execution
  @retval         FALSE             Current channel will be executed

*/
BOOLEAN
SkipChannelExecution (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   ChannelIndex
  )
{
  BOOLEAN SkipChannel = FALSE;

  if (ChannelIndex >= MAX_CH) {
    SkipChannel = TRUE;
    RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_002);
    return SkipChannel;
  }

  if (!IsRankPresent (CommonParameters->Socket, ChannelIndex, CommonParameters->Dimm[ChannelIndex], CommonParameters->Rank[ChannelIndex])) {
    SkipChannel = TRUE;
  }

  if (!CommonParameters->ChannelEnabledLocal[ChannelIndex]) {
    SkipChannel = TRUE;
  }

  if (((1 << ChannelIndex) & CommonParameters->ChannelMask) != 0) {
    SkipChannel = TRUE;
  }

  return SkipChannel;
}

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
  )
{
  UINT8 Strobe;
  UINT8 StrobeLogical;

  Strobe = BitIndex / BITS_PER_NIBBLE;

  StrobeLogical = Strobe;

  if (CommonParameters->DdrType == Ddr4Type) {

    StrobeLogical = Strobe / 2;

    if ((Strobe % 2) == 1) {
      //
      // CommonParameters->NextNibbleOffset is 9 for DDR4
      //
      StrobeLogical += CommonParameters->NextNibbleOffset;
    }
  } else if (CommonParameters->DdrType == Ddr5Type) {
    //
    // CommonParameters->MaxStrobeAvailable is 10 for DDR5 both in BRS and SPR
    //
    if (CommonParameters->MaxStrobeAvailable > 0) {
      StrobeLogical = (Strobe % CommonParameters->MaxStrobeAvailable) / 2;
    }

    if ((Strobe % 2) == 1) {
      //
      // CommonParameters->NextNibbleOffset is 5 for DDR5
      //
      StrobeLogical += CommonParameters->NextNibbleOffset;
    }

    if (BitIndex >= (BITS_PER_NIBBLE * CommonParameters->MaxStrobeAvailable)) {
      StrobeLogical += CommonParameters->MaxStrobeAvailable;
    }
  } // Ddr5Type

  return StrobeLogical;
}

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
  )
{

  if (CommonParameters->MaxStrobeAvailable > 0) {
    StrobeLogical %= CommonParameters->MaxStrobeAvailable;
  }

  if (StrobeLogical < (CommonParameters->MaxStrobeAvailable / 2)) {
    return (StrobeLogical * MAX_BITS_IN_BYTE) + (BitIndex % BITS_PER_NIBBLE) + SubChannelIndex * (MAX_BITS / 2);
  } else {
    StrobeLogical -= (CommonParameters->MaxStrobeAvailable / 2);
    return (StrobeLogical * MAX_BITS_IN_BYTE) + (BitIndex % BITS_PER_NIBBLE) + BITS_PER_NIBBLE + SubChannelIndex * (MAX_BITS / 2);
  }
}

/*

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

*/
struct baseMargin *
EFIAPI
GetResultPtrAtRawBitLevel (
    IN         COMMON_1D_PARAMETERS    *CommonParameters,
    IN         UINT8                   KnobIndex,
    IN         UINT8                   ChannelIndex,
    IN         UINT8                   BitIndex,
    IN         RESULTS_1D              *Results
  )
{
  UINT8   StrobeLogical;
  UINT8   BitIndexInNibble;

  BitIndexInNibble = BitIndex % BITS_PER_NIBBLE;
  StrobeLogical = GetLogicalStrobeFromBit (CommonParameters, BitIndex);

  return GetResultPtrAtBitLevel (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, BitIndexInNibble, Results);
}

/*

  Returns the pointer to the current results condition which is related to the channel, knob and bit specified
  The bit is the bit index inside the channel which is range from 0 to MAX_BITS-1.

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      KnobIndex         Current knob under test (0-based)
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      BitIndex          Current bit under test (0-based) inside the channel. Range from 0 to MAX_BITS-1

  @return  Pointer to &ResultCondition[KnobIndex][ChannelIndex][StrobeLogical][BitIndex]

*/
RESULTS_1D_CONDITION *
EFIAPI
GetResultConditionPtrAtRawBitLevel (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   KnobIndex,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   BitIndex
  )
{
  UINT8   StrobeLogical;
  UINT8   BitIndexInNibble;

  BitIndexInNibble = BitIndex % BITS_PER_NIBBLE;
  StrobeLogical = GetLogicalStrobeFromBit (CommonParameters, BitIndex);

  return GetResultConditionPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, BitIndexInNibble);
}

/*

  Returns the current delay offset from its orignal delay for the channel, knob and strobe specified

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      KnobIndex         Current knob under test (0-based)
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      SubChannelIndex   Current subchannel under test (0-based)
  @param[in]      StrobeLinear      Current strobe under test (0-based)
  @param[out]     Offset            Offet

  @return  MRC_STATUS_SUCCESS,  MRC_STATUS_FAILURE

*/
MRC_STATUS
EFIAPI
GetStrobeDelayOffset (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   KnobIndex,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   SubChannelIndex,
  IN         UINT8                   StrobeLinear,
  OUT        INT16                   *Offset
  )
{
  UINT8   StrobeLogical;
  INT16   *OriginalDelayPtr;
  INT16   *CurrentDelayPtr;

  if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
    return MRC_STATUS_FAILURE;
  }

  if (IsResultAtRankLevel (CommonParameters) && StrobeLinear >= 1) {
    return MRC_STATUS_FAILURE;
  }

  if (SkipExecutionOnStrobe (CommonParameters, ChannelIndex, StrobeLinear)) {
    return MRC_STATUS_FAILURE;
  }

  StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);

  if (IsSingleToMultipleDelay (CommonParameters)) {
    //
    // Tag::IsSingleToMultipleDelay::NoBreak
    // We read original values only from the first strobe
    //
    StrobeLinear = 0;
  } 
  StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);
  OriginalDelayPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, CommonParameters->OriginalDelays);

  CurrentDelayPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, CommonParameters->CurrentDelays);

  *Offset = *CurrentDelayPtr - *OriginalDelayPtr;

  return MRC_STATUS_SUCCESS;
}

/*

  Returns the current delay offset from its orignal delay for the specified channel.
  The returned dalay offset is the maximum offset of all knobs and strobes in the channel.

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[out]     Offset            Offet

  @return  MRC_STATUS_SUCCESS,  MRC_STATUS_FAILURE

*/
MRC_STATUS
EFIAPI
GetChannelDelayOffset (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   ChannelIndex,
  OUT        INT16                   *Offset
  )
{
  UINT8   KnobIndex;;
  UINT8   StrobeLinear;
  INT16   ChannelOffset = 0;
  INT16   StrobeOffset = 0;
  UINT8   SubChannelIndex;

  if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
    return MRC_STATUS_FAILURE;
  }

  for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {
    for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
      for (StrobeLinear = 0; StrobeLinear < CommonParameters->MaxStrobeAvailable; StrobeLinear++) {

        if (GetStrobeDelayOffset (CommonParameters, KnobIndex, ChannelIndex, SubChannelIndex, StrobeLinear, &StrobeOffset) == MRC_STATUS_SUCCESS) {

          if (CommonParameters->CurrentEdge == LEFT_EDGE) {
            if (StrobeOffset < ChannelOffset) {
              ChannelOffset = StrobeOffset;
            }
          } else {
            if (StrobeOffset > ChannelOffset) {
              ChannelOffset = StrobeOffset;
            }
          }
        } // MRC_STATUS_SUCCESS
      } //StrobeLinear
    } // SubChannelIndex
  } //KnobIndex

  *Offset = ChannelOffset;

  return MRC_STATUS_SUCCESS;
}

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
  )
{
  INT16   DeltaOffset;
  INT8   VerticalIndex;
  COMMON_1D_PARAMETERS *CommonParametersV;

  DeltaOffset = 0;
  CommonParametersV = &CommonParameters2D->CommonParametersV;

  GetChannelDelayOffset (CommonParametersV, ChannelIndex, &DeltaOffset);

  VerticalIndex = (INT8)((DeltaOffset + CommonParametersV->LimitOffset) / (CommonParametersV->StepSize));

  if (VerticalIndex < 0 || VerticalIndex >= CommonParameters2D->MaxVerticalEntries) {
    RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_010);
    return 0;
  }

  return VerticalIndex;
}

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
)
{
  UINT8   ChannelIndex;

  *FirstChannel = 0;

  for (ChannelIndex = 0; ChannelIndex < CommonParameters->MaxChDdr; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    break;
  }

  if (ChannelIndex >= CommonParameters->MaxChDdr) {
    RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_011);
    return MRC_STATUS_FAILURE;
  }

  *FirstChannel = ChannelIndex;

  return MRC_STATUS_SUCCESS;

}

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
)
{
  UINT8   LogicalRank;
  struct TrainingVariable *TrainingVar;

  TrainingVar = MemTrainingVar ();

  if (OffsetLeftCached == NULL || OffsetRightCached == NULL) {
    return MRC_STATUS_INVALID_PARAMETER;
  }

  if ((CommonParameters->FlagsResults & ResultsRelative) == 0) {
    *OffsetLeftCached   = 0;
    *OffsetRightCached  = 0;
    return MRC_STATUS_UNSUPPORTED;
  }

  if ((CommonParameters->FlagsOffsets & StartOnPassingEdges) == 0) {
    *OffsetLeftCached   = 0;
    *OffsetRightCached  = 0;
    return MRC_STATUS_UNSUPPORTED;
  }

  if (CommonParameters->FlagsOffsets & UseOffsetAsAbsolute) {
    *OffsetLeftCached   = 0;
    *OffsetRightCached  = 0;
    return MRC_STATUS_UNSUPPORTED;
  }

  *OffsetLeftCached   = 0;
  *OffsetRightCached  = 0;

  LogicalRank = GetLogicalRank (
                    CommonParameters->Host,
                    CommonParameters->Socket,
                    ChannelIndex,
                    CommonParameters->Dimm[ChannelIndex],
                    CommonParameters->Rank[ChannelIndex]
                    );

  if (CommonParameters->Level == DdrLevel) {
    switch (CommonParameters->Group) {
    case TxDqDelay:
      *OffsetLeftCached   = TrainingVar->TxDqLeft[ChannelIndex][LogicalRank][StrobeLogical];
      *OffsetRightCached  = TrainingVar->TxDqRight[ChannelIndex][LogicalRank][StrobeLogical];
      break;
    case TxVref:
      *OffsetLeftCached   = TrainingVar->TxVrefLow[ChannelIndex][LogicalRank][StrobeLogical];
      *OffsetRightCached  = TrainingVar->TxVrefHigh[ChannelIndex][LogicalRank][StrobeLogical];
      break;
    case RxDqsDelay:
    case RxDqsNDelay:
    case RxDqsPDelay:
      *OffsetLeftCached   = TrainingVar->RxDqsLeft[ChannelIndex][LogicalRank][StrobeLogical];
      *OffsetRightCached  = TrainingVar->RxDqsRight[ChannelIndex][LogicalRank][StrobeLogical];
      break;
    case RxVref:
    case RxSamplerEvenOdd:
    case RxSamplerEven:
    case RxSamplerOdd:
      *OffsetLeftCached   = TrainingVar->RxVrefLow[ChannelIndex][LogicalRank][StrobeLogical];
      *OffsetRightCached  = TrainingVar->RxVrefHigh[ChannelIndex][LogicalRank][StrobeLogical];
    }
  }

  *OffsetLeftCached   += SWEEP_FINE_STEP_SIZE;
  *OffsetRightCached  -= SWEEP_FINE_STEP_SIZE;

  if (*OffsetLeftCached >= *OffsetRightCached) {
    *OffsetLeftCached   = 0;
    *OffsetRightCached  = 0;
  }


  return MRC_STATUS_SUCCESS;
}

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
)
{
  UINT8   LogicalRank;
  struct TrainingVariable *TrainingVar;

  TrainingVar = MemTrainingVar ();

  if ((CommonParameters->FlagsResults & ResultsRelative) == 0) {
    return MRC_STATUS_UNSUPPORTED;
  }

  if ((CommonParameters->FlagsOffsets & StartOnPassingEdges) == 0) {
    return MRC_STATUS_UNSUPPORTED;
  }

  if (CommonParameters->FlagsOffsets & UseOffsetAsAbsolute) {
    return MRC_STATUS_UNSUPPORTED;
  }

  LogicalRank = GetLogicalRank (
                    CommonParameters->Host,
                    CommonParameters->Socket,
                    ChannelIndex,
                    CommonParameters->Dimm[ChannelIndex],
                    CommonParameters->Rank[ChannelIndex]
                    );

  if (OffsetLeftCached >= OffsetRightCached) {
    OffsetLeftCached   = 0;
    OffsetRightCached  = 0;
  }

  if (CommonParameters->Level == DdrLevel) {
    switch (CommonParameters->Group) {
    case TxDqDelay:
      TrainingVar->TxDqLeft[ChannelIndex][LogicalRank][StrobeLogical]  = OffsetLeftCached;
      TrainingVar->TxDqRight[ChannelIndex][LogicalRank][StrobeLogical] = OffsetRightCached;
      break;
    case TxVref:
      TrainingVar->TxVrefLow[ChannelIndex][LogicalRank][StrobeLogical]  = OffsetLeftCached;
      TrainingVar->TxVrefHigh[ChannelIndex][LogicalRank][StrobeLogical] = OffsetRightCached;
      break;
    case RxDqsDelay:
    case RxDqsNDelay:
    case RxDqsPDelay:
      TrainingVar->RxDqsLeft[ChannelIndex][LogicalRank][StrobeLogical]  = OffsetLeftCached;
      TrainingVar->RxDqsRight[ChannelIndex][LogicalRank][StrobeLogical] = OffsetRightCached;
      break;
    case RxVref:
    case RxSamplerEvenOdd:
    case RxSamplerEven:
    case RxSamplerOdd:
      TrainingVar->RxVrefLow[ChannelIndex][LogicalRank][StrobeLogical]  = OffsetLeftCached;
      TrainingVar->RxVrefHigh[ChannelIndex][LogicalRank][StrobeLogical] = OffsetRightCached;
      break;

    }
  }

 return MRC_STATUS_SUCCESS;
}

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
  )
{

  UINT8   KnobIndex;
  UINT8   ChannelIndex;
  UINT8   SubChannelIndex;
  UINT8   MaxChDdr;
  UINT8   StrobeLogical;
  UINT8   StrobeLinear;
  INT16   OffsetLeftCached;
  INT16   OffsetRightCached;
  struct baseMargin *StrobeMargin;


  if ((CommonParameters->FlagsResults & ResultsRelative) == 0) {
    return MRC_STATUS_UNSUPPORTED;
  }

  if ((CommonParameters->FlagsOffsets & StartOnPassingEdges) == 0) {
    return MRC_STATUS_UNSUPPORTED;
  }

  if (CommonParameters->FlagsOffsets & UseOffsetAsAbsolute) {
    return MRC_STATUS_UNSUPPORTED;
  }


  MaxChDdr = CommonParameters->MaxChDdr;

  for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }
    for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
      for (StrobeLinear = 0; StrobeLinear < CommonParameters->MaxStrobeAvailable; StrobeLinear++) {

        if (SkipExecutionOnStrobe (CommonParameters, ChannelIndex, StrobeLinear)) {
          continue;
        }
        //
        // Tag::IsSingleToMultipleDelay::NoBreak
        // No need to add the IsSingleToMultipleDelay() && StrobeLinear >= 1 break here
        //
        StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);

        OffsetLeftCached  = MIN_INT16;
        OffsetRightCached = MAX_INT16;

        for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {

          StrobeMargin = GetResultPtrAtStrobeLevel (
                           CommonParameters,
                           KnobIndex,
                           ChannelIndex,
                           StrobeLogical,
                           CommonParameters->Results
                           );

          if (StrobeMargin->n > OffsetLeftCached) {
            OffsetLeftCached  = StrobeMargin->n;
          }

          if (StrobeMargin->p < OffsetRightCached) {
            OffsetRightCached = StrobeMargin->p;
          }

          if (AreResultsAtByteLevel (CommonParameters, ChannelIndex)) {

            StrobeMargin = GetResultPtrAtStrobeLevel (
                             CommonParameters,
                             KnobIndex,
                             ChannelIndex,
                             StrobeLogical + CommonParameters->NextNibbleOffset,
                             CommonParameters->Results
                             );

            if (StrobeMargin->n > OffsetLeftCached) {
              OffsetLeftCached  = StrobeMargin->n;
            }

            if (StrobeMargin->p < OffsetRightCached) {
              OffsetRightCached = StrobeMargin->p;
            }

          }

        } // KnobIndex loop ...

        SetOffsetLeftRight (CommonParameters, ChannelIndex, StrobeLogical, OffsetLeftCached, OffsetRightCached);

      } // StrobeLinear loop ...
    } // SubChannelIndex loop ...
  } // ChannelIndex loop ...

  return MRC_STATUS_SUCCESS;
}
