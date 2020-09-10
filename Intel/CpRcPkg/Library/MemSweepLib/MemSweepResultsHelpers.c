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

  Keeps track of the delay which is passing for each knob/channel/strobe and stores it in the Results
  Also it keeps track at a bit level if FLAGS_CATEGORY_RESULTS.ResultsPerBit is set

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       Function was executed without errors

*/
MRC_STATUS
TrackingResults1D (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
)
{
  UINT8   ChannelIndex;
  UINT8   SubChannelIndex;
  UINT8   MaxChDdr;
  UINT8   StrobeLogical;
  UINT8   StrobeLinear;
  UINT8   BitIndex;
  UINT8   KnobIndex;
  INT16   *CurrentDelayPtr;
  struct baseMargin *StrobeMargin;
  struct baseMargin *BitMargin;
  RESULTS_1D_DEFINITION PassCondition;
  RESULTS_1D_CONDITION  *ResultConditionPtr;

  MaxChDdr = CommonParameters->MaxChDdr;

  for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {
      for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
        for (StrobeLinear = 0; StrobeLinear < CommonParameters->MaxStrobeAvailable; StrobeLinear++) {

          if (IsResultAtRankLevel (CommonParameters) && StrobeLinear >= 1) {
            //
            // Tag::StrobeLinearBreak::AtRankLevel
            // If operating a delay at Rank Level only strobe 0 will have
            // the final margin information
            //
            continue;
          }

          StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);

          StrobeMargin = GetResultPtrAtStrobeLevel (
                            CommonParameters,
                            KnobIndex,
                            ChannelIndex,
                            StrobeLogical,
                            CommonParameters->Results
                            );

          CurrentDelayPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, CommonParameters->CurrentDelays);
          //
          // Update the results only if the FSM is in SamplingState and current strobe passed
          //
          if (NibblePassOnCurrentKnob (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical) &&
              IsSamplingState (CommonParameters, CommonParameters->CurrentState[ChannelIndex][StrobeLogical])) {

            if (IsLeftState (CommonParameters, CommonParameters->CurrentState[ChannelIndex][StrobeLogical]) ||
                (CommonParameters->CurrentState[ChannelIndex][StrobeLogical] == ST_AC_FIRST_SAMPLE && CommonParameters->CurrentEdge == LEFT_EDGE)) {

              StrobeMargin->n = *CurrentDelayPtr;

            } else if (IsRightState (CommonParameters, CommonParameters->CurrentState[ChannelIndex][StrobeLogical]) ||
              (CommonParameters->CurrentState[ChannelIndex][StrobeLogical] == ST_AC_FIRST_SAMPLE && CommonParameters->CurrentEdge == RIGHT_EDGE)) {

              StrobeMargin->p = *CurrentDelayPtr;

            }

          }
          //
          // Update the results only if the FSM is in SamplingState and current strobe/bit passed
          //
          if (CommonParameters->FlagsResults & ResultsPerBit) {
            for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {

              ResultConditionPtr = GetResultConditionPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, BitIndex);
              BitMargin = GetResultPtrAtBitLevel (
                            CommonParameters,
                            KnobIndex,
                            ChannelIndex,
                            StrobeLogical,
                            BitIndex,
                            CommonParameters->Results
                            );

              if (BitMargin == NULL) {
                RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_012);
                continue;
              }

              PassCondition = GetPassingCondition (CommonParameters, ResultConditionPtr);

              if (ResultConditionPtr->Current == PassCondition &&
                  IsSamplingState (CommonParameters, CommonParameters->CurrentState[ChannelIndex][StrobeLogical])) {

                if (IsLeftState (CommonParameters, CommonParameters->CurrentState[ChannelIndex][StrobeLogical]) ||
                    (CommonParameters->CurrentState[ChannelIndex][StrobeLogical] == ST_AC_FIRST_SAMPLE && CommonParameters->CurrentEdge == LEFT_EDGE)) {

                  BitMargin->n = *CurrentDelayPtr;

                } else if (IsRightState (CommonParameters, CommonParameters->CurrentState[ChannelIndex][StrobeLogical]) ||
                          (CommonParameters->CurrentState[ChannelIndex][StrobeLogical] == ST_AC_FIRST_SAMPLE && CommonParameters->CurrentEdge == RIGHT_EDGE)) {

                  BitMargin->p = *CurrentDelayPtr;

                }

              }

            } // BitIndex loop ...

          } // CommonParameters->FlagsResults & ResultsPerBit

        } // StrobeLinear loop ...

      } // SubChannelIndex loop ...
    } // KnobIndex loop ...

  } // ChannelIndex loop ...

  return MRC_STATUS_SUCCESS;
}

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
)
{
  UINT8   KnobIndex;
  UINT16  MinDither;
  UINT16  MaxDither;
  UINT8   BitIndex;

  struct baseMargin *StrobeMargin;
  struct baseMargin *BitMargin;
  RESULTS_1D_CONDITION *ResultConditionPtr;

  //
  // Get Current delay values
  //
  for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {

    StrobeMargin = GetResultPtrAtStrobeLevel (
                     CommonParameters,
                     KnobIndex,
                     ChannelIndex,
                     StrobeLogical,
                     CommonParameters->Results
                     );

    GetMinMaxDitherCounterAcrossBits (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, &MinDither, &MaxDither);

    if (NextState == ST_LE_END) {
      if (StrobeMargin->n != 0) {
        StrobeMargin->n -= (MinDither - 1) * CommonParameters->StepSize;
      }
    } else if (NextState == ST_RE_END) {
      if (StrobeMargin->p != 0) {
        StrobeMargin->p += (MinDither - 1) * CommonParameters->StepSize;
      }
    }

    if (CommonParameters->FlagsResults & ResultsPerBit) {

      for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {

        ResultConditionPtr = GetResultConditionPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, BitIndex);

        BitMargin = GetResultPtrAtBitLevel (
                      CommonParameters,
                      KnobIndex,
                      ChannelIndex,
                      StrobeLogical,
                      BitIndex,
                      CommonParameters->Results
                      );

        if (BitMargin == NULL) {
          RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_013);
          continue;
        }

        if (NextState == ST_LE_END) {
          if (BitMargin->n != 0) {
            BitMargin->n -= (ResultConditionPtr->DitherCounter - 1) * CommonParameters->StepSize;
          }
        } else if (NextState == ST_RE_END) {
          if (BitMargin->p != 0) {
            BitMargin->p += (ResultConditionPtr->DitherCounter - 1) * CommonParameters->StepSize;
          }
        }
      } // BitIndex loop ...
    } // CommonParameters->FlagsResults & ResultsPerBit

  } // KnobIndex loop ...

  return MRC_STATUS_SUCCESS;
}

/*

  Adjust the final Results 1D based relative to the original value

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       Function was executed without errors

*/
MRC_STATUS
AdjustRelativeResults1D (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  UINT8   ChannelIndex;
  UINT8   SubChannelIndex;
  UINT8   MaxChDdr;
  UINT8   StrobeLogical;
  UINT8   StrobeLinear;
  UINT8   KnobIndex;
  UINT8   BitIndex;
  INT16   *OriginalDelayPtr;
  struct baseMargin *StrobeMargin;
  struct baseMargin *BitMargin;

  MaxChDdr = CommonParameters->MaxChDdr;

  for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {

    for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {

      if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
        continue;
      }
      for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
        for (StrobeLinear = 0; StrobeLinear < CommonParameters->MaxStrobeAvailable; StrobeLinear++) {

          if (IsResultAtRankLevel (CommonParameters) && StrobeLinear >= 1) {
            //
            // Tag::StrobeLinearBreak::AtRankLevel
            // If operating a delay at Rank Level only strobe 0 will have
            // the final margin information
            //
            continue;
          }

          if (SkipExecutionOnStrobe (CommonParameters, ChannelIndex, StrobeLinear)) {
            continue;
          }

          StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);

          OriginalDelayPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, CommonParameters->OriginalDelays);

          StrobeMargin = GetResultPtrAtStrobeLevel (
                           CommonParameters,
                           KnobIndex,
                           ChannelIndex,
                           StrobeLogical,
                           CommonParameters->Results
                           );

          if (StrobeMargin->n != -1) {
            StrobeMargin->n = StrobeMargin->n - *OriginalDelayPtr;
          }

          if (StrobeMargin->p != -1) {
            StrobeMargin->p = StrobeMargin->p - *OriginalDelayPtr;
          }

          if (AreResultsAtByteLevel (CommonParameters, ChannelIndex)) {
            StrobeMargin = GetResultPtrAtStrobeLevel (
                             CommonParameters,
                             KnobIndex,
                             ChannelIndex,
                             StrobeLogical + CommonParameters->NextNibbleOffset,
                             CommonParameters->Results
                             );
            if (StrobeMargin->n != -1) {
              StrobeMargin->n = StrobeMargin->n - *OriginalDelayPtr;
            }

            if (StrobeMargin->p != -1) {
              StrobeMargin->p = StrobeMargin->p - *OriginalDelayPtr;
            }
          }

          if (CommonParameters->FlagsResults & ResultsPerBit) {
            for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {

              BitMargin = GetResultPtrAtBitLevel (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, BitIndex, CommonParameters->Results);

              if (BitMargin == NULL) {
                RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_014);
                continue;
              }

              if (BitMargin->n != -1) {
                BitMargin->n = BitMargin->n - *OriginalDelayPtr;
              }

              if (BitMargin->p != -1) {
                BitMargin->p = BitMargin->p - *OriginalDelayPtr;
              }
              if (AreResultsAtByteLevel (CommonParameters, ChannelIndex)) {

                BitMargin = GetResultPtrAtBitLevel (
                              CommonParameters,
                              KnobIndex,
                              ChannelIndex,
                              StrobeLogical + CommonParameters->NextNibbleOffset,
                              BitIndex,
                              CommonParameters->Results
                              );

                if (BitMargin->n != -1) {
                  BitMargin->n = BitMargin->n - *OriginalDelayPtr;
                }

                if (BitMargin->p != -1) {
                  BitMargin->p = BitMargin->p - *OriginalDelayPtr;
                }
              }
            }
          }

        } // StrobeLinear loop ...
      } // SubChannelIndex loop ...
    } // ChannelIndex loop

  } // KnobIndex loop ...

  return MRC_STATUS_SUCCESS;
}

/*

  Adjust the per bit final Results 1D.

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       Function was executed without errors

*/
MRC_STATUS
AdjustPerBitResults1D (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  UINT8   ChannelIndex;
  UINT8   SubChannelIndex;
  UINT8   MaxChDdr;
  UINT8   StrobeLogical;
  UINT8   StrobeLinear;
  UINT8   KnobIndex;
  UINT8   BitIndex;
  INT16   *OriginalDelayPtr;
  MRC_GT  PerBitGroup;
  INT16   MinPerBitValue;
  INT16   PerBitValues[BITS_PER_NIBBLE];
  struct baseMargin *BitMargin;

  if (!(CommonParameters->FlagsResults & ResultsPerBit)) {
    return MRC_STATUS_SUCCESS;
  }

  if (!IsPerBitControlWhileActPerStrobe (CommonParameters->Host, CommonParameters->Socket, CommonParameters->Level, CommonParameters->Group)) {
    return MRC_STATUS_SUCCESS;
  }
  PerBitGroup = GetPerBitGroup (CommonParameters->Host, CommonParameters->Socket, CommonParameters->Level, CommonParameters->Group);

  MaxChDdr = CommonParameters->MaxChDdr;

  for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {

    for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {

      if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
        continue;
      }
      for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
        for (StrobeLinear = 0; StrobeLinear < CommonParameters->MaxStrobeAvailable; StrobeLinear++) {

          if (IsResultAtRankLevel (CommonParameters) && StrobeLinear >= 1) {
            //
            // Tag::StrobeLinearBreak::AtRankLevel
            // If operating a delay at Rank Level only strobe 0 will have
            // the final margin information
            //
            continue;
          }

          if (SkipExecutionOnStrobe (CommonParameters, ChannelIndex, StrobeLinear)) {
            continue;
          }

          StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);

          OriginalDelayPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, CommonParameters->OriginalDelays);

          MinPerBitValue = MAX_INT16;
          for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {
            GetSetDataGroup (
              CommonParameters->Host,
              CommonParameters->Socket,
              ChannelIndex,
              SubChannelIndex,
              CommonParameters->Dimm[ChannelIndex],
              CommonParameters->Rank[ChannelIndex],
              StrobeLogical,
              BitIndex,
              CommonParameters->Level,
              PerBitGroup,
              GSM_READ_ONLY,
              &PerBitValues[BitIndex]
            );

            if (PerBitValues[BitIndex] < MinPerBitValue) {
              MinPerBitValue = PerBitValues[BitIndex];
            }
          }

          RC_FATAL_ERROR ((MinPerBitValue == *OriginalDelayPtr), ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_021);

          for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {
            PerBitValues[BitIndex] -= MinPerBitValue;
          }

          for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {

            BitMargin = GetResultPtrAtBitLevel (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, BitIndex, CommonParameters->Results);

            if (BitMargin == NULL) {
              RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_014);
              continue;
            }

            if (BitMargin->n != -1) {
              BitMargin->n = BitMargin->n  + PerBitValues[BitIndex];
            }

            if (BitMargin->p != -1) {
              BitMargin->p = BitMargin->p + PerBitValues[BitIndex];
            }
          } // BitIndex

        } // StrobeLinear loop ...
      } // SubChannelIndex loop ...
    } // ChannelIndex loop

  } // KnobIndex loop ...

  return MRC_STATUS_SUCCESS;
}
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
)
{
  RESULTS_1D_DEFINITION PassCondition;

  PassCondition = PASS_CONDITION;

  if (CommonParameters->FlagsResults & FocusOnTransition) {
    //
    // Passing criteria is based on the first sample
    //
    PassCondition = ResultConditionPtr->First;
  }

  return PassCondition;
}

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
)
{
  RESULTS_1D_DEFINITION FailCondition;

  FailCondition = FAIL_CONDITION;

  if (CommonParameters->FlagsResults & FocusOnTransition) {
    //
    // Invert failing criteria based on the first sample
    //
    if (ResultConditionPtr->First == FAIL_CONDITION) {
      FailCondition = PASS_CONDITION;
    }
  }


  return FailCondition;
}

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
)
{
  UINT8   BitIndex;
  RESULTS_1D_DEFINITION FailCondition;
  RESULTS_1D_CONDITION *ResultConditionPtr;

  for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {

    ResultConditionPtr = GetResultConditionPtr (CommonParameters, KnobIndex, ChannelIndex, NibbleLogical, BitIndex);

    FailCondition = GetFailingCondition (CommonParameters, ResultConditionPtr);

    if (ResultConditionPtr->Current == FailCondition) {
      return FALSE;
    }

  } // BitIndex loop ...

  return TRUE;
}

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
)
{

  if (!NibblePassOnCurrentKnob (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical)) {
    return FALSE;
  }

  if (AreResultsAtByteLevel (CommonParameters, ChannelIndex)) {
    if (!NibblePassOnCurrentKnob (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical + CommonParameters->NextNibbleOffset)) {
      return FALSE;
    }
  }


  return TRUE;
}

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
)
{
  UINT8   KnobIndex;
  BOOLEAN PassOnAllKnobs;

  PassOnAllKnobs = TRUE;

  for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {

    if (!StrobePassOnCurrentKnob (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical)) {
      PassOnAllKnobs = FALSE;
      break;
    }

  } // KnobIndex loop ...

  return PassOnAllKnobs;
}

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
)
{
  UINT8   KnobIndex;
  UINT8   BitIndex;
  BOOLEAN AnyBitFailedOnAllKnobs;
  BOOLEAN BitFailedOnAllKnobs;
  RESULTS_1D_DEFINITION PassCondition;
  RESULTS_1D_CONDITION *ResultConditionPtr;

  AnyBitFailedOnAllKnobs = FALSE;

  for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {

    BitFailedOnAllKnobs = TRUE;

    for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {

      ResultConditionPtr = GetResultConditionPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, BitIndex);

      PassCondition = GetPassingCondition (CommonParameters, ResultConditionPtr);

      if (ResultConditionPtr->Current == PassCondition) {
        BitFailedOnAllKnobs = FALSE;
        break;
      }
    }

    if (BitFailedOnAllKnobs) {
      AnyBitFailedOnAllKnobs = TRUE;
      break;
    }

    if (AreResultsAtByteLevel (CommonParameters, ChannelIndex)) {

      BitFailedOnAllKnobs = TRUE;

      for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {

        ResultConditionPtr = GetResultConditionPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical + CommonParameters->NextNibbleOffset, BitIndex);

        PassCondition = GetPassingCondition (CommonParameters, ResultConditionPtr);

        if (ResultConditionPtr->Current == PassCondition) {
          BitFailedOnAllKnobs = FALSE;
          break;
        }

      } // KnobIndex loop ...

      if (BitFailedOnAllKnobs) {
        AnyBitFailedOnAllKnobs = TRUE;
        break;
      }

    }

  } // BitIndex loop ...

  return AnyBitFailedOnAllKnobs;

}

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
)
{
  UINT8   KnobIndex;
  UINT8   BitIndex;
  BOOLEAN AllBitFailed;
  BOOLEAN BreakLoop = FALSE;
  RESULTS_1D_DEFINITION PassCondition;
  RESULTS_1D_CONDITION *ResultConditionPtr;

  AllBitFailed = TRUE;

  for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {
    for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {

      ResultConditionPtr = GetResultConditionPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, BitIndex);

      PassCondition = GetPassingCondition (CommonParameters, ResultConditionPtr);

      if (ResultConditionPtr->Current == PassCondition) {
        AllBitFailed  = FALSE;
        BreakLoop     = TRUE;
        break;
      }

      if (AreResultsAtByteLevel (CommonParameters, ChannelIndex)) {

        ResultConditionPtr = GetResultConditionPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical + CommonParameters->NextNibbleOffset, BitIndex);

        PassCondition = GetPassingCondition (CommonParameters, ResultConditionPtr);

        if (ResultConditionPtr->Current == PassCondition) {
          AllBitFailed  = FALSE;
          BreakLoop     = TRUE;
          break;
        }
      }
    } // BitIndex loop ...

    if (BreakLoop) {
      break;
    }

  } // KnobIndex loop ...

  return AllBitFailed;

}

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
  )
{
  UINT8   StrobeLinear;
  UINT8   StrobeLogical;

  for (StrobeLinear = 0; StrobeLinear < CommonParameters->MaxStrobeAvailable; StrobeLinear++) {

    if (SkipExecutionOnStrobe (CommonParameters, ChannelIndex, StrobeLinear)) {
      continue;
    }

    StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);

    if (!StrobePassAcrossAllKnobs (CommonParameters, ChannelIndex, StrobeLogical)) {

      return TRUE;

    }

  } // StrobeLinear loop ...

  return FALSE;
}

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
)
{
  UINT8   KnobIndex;
  UINT8   BitIndex;
  BOOLEAN AnyBitMatchesDitherValue;
  BOOLEAN BitMatchesDitherValue;
  RESULTS_1D_CONDITION *ResultConditionPtr;

  AnyBitMatchesDitherValue = FALSE;

  for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {

    BitMatchesDitherValue = TRUE;

    for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {

      ResultConditionPtr = GetResultConditionPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, BitIndex);

      if (ResultConditionPtr->DitherCounter < CommonParameters->DitherValue) {
        BitMatchesDitherValue  = FALSE;
        break;
      }

      if (AreResultsAtByteLevel (CommonParameters, ChannelIndex)) {

        ResultConditionPtr = GetResultConditionPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical + CommonParameters->NextNibbleOffset, BitIndex);

        if (ResultConditionPtr->DitherCounter < CommonParameters->DitherValue) {
          BitMatchesDitherValue = FALSE;
          break;
        }
      }

    } // KnobIndex loop ...

    if (BitMatchesDitherValue) {
      AnyBitMatchesDitherValue = TRUE;
      break;
    }

  } // BitIndex loop ...


  return AnyBitMatchesDitherValue;

}

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
)
{
  UINT8   KnobIndex;
  UINT8   BitIndex;
  BOOLEAN AllBitMatchesDitherValue;
  BOOLEAN BreakLoop;
  RESULTS_1D_CONDITION *ResultConditionPtr;

  AllBitMatchesDitherValue = TRUE;
  BreakLoop = FALSE;

  for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {
    for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {

      ResultConditionPtr = GetResultConditionPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, BitIndex);

      if (ResultConditionPtr->DitherCounter < CommonParameters->DitherValue) {
        AllBitMatchesDitherValue = FALSE;
        BreakLoop = TRUE;
        break;
      }

      if (AreResultsAtByteLevel (CommonParameters, ChannelIndex)) {

        ResultConditionPtr = GetResultConditionPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical + CommonParameters->NextNibbleOffset, BitIndex);

        if (ResultConditionPtr->DitherCounter < CommonParameters->DitherValue) {
          AllBitMatchesDitherValue = FALSE;
          BreakLoop = TRUE;
          break;
        }
      }

    } // KnobIndex loop ...

    if (BreakLoop) {
      break;
    }

  } // BitIndex loop ...

  return AllBitMatchesDitherValue;

}

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
  )
{

  UINT8   BitIndex;
  RESULTS_1D_DEFINITION FailingCondition;
  RESULTS_1D_DEFINITION PassingCondition;
  RESULTS_1D_CONDITION  *ResultConditionPtr;
  RESULTS_1D_CONDITION  *TargetResultConditionPtr;

  if (IsSearchPassState (CommonParameters, CommonParameters->CurrentState[ChannelIndex][TargetStrobeLogical]) ||
      CommonParameters->CurrentState[ChannelIndex][TargetStrobeLogical] == ST_AC_FIRST_SAMPLE) {

    for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {

      ResultConditionPtr = GetResultConditionPtr (CommonParameters, KnobIndex, ChannelIndex, SourceStrobeLogical, BitIndex);

      FailingCondition = GetFailingCondition (CommonParameters, ResultConditionPtr);

      if (ResultConditionPtr->Current == FailingCondition) {
        //
        // Spread the result only across TargetStrobeLogical
        //
        TargetResultConditionPtr = GetResultConditionPtr (CommonParameters, KnobIndex, ChannelIndex, TargetStrobeLogical, BitIndex);

        FailingCondition = GetFailingCondition (CommonParameters, TargetResultConditionPtr);

        TargetResultConditionPtr->Current = FailingCondition;

      } // ResultConditionPtr->Current == FailingCondition

    } // BitIndex loop ...

  } else if (IsFailingState (CommonParameters, CommonParameters->CurrentState[ChannelIndex][TargetStrobeLogical])) {
    //
    // If while searching for a failing condition, it is found a 'passing' condition
    // that condition will be propagated to logical strobe 0
    //
    for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {

      ResultConditionPtr = GetResultConditionPtr (CommonParameters, KnobIndex, ChannelIndex, SourceStrobeLogical, BitIndex);

      PassingCondition = GetPassingCondition (CommonParameters, ResultConditionPtr);

      if (ResultConditionPtr->Current == PassingCondition) {

        TargetResultConditionPtr = GetResultConditionPtr (CommonParameters, KnobIndex, ChannelIndex, TargetStrobeLogical, BitIndex);

        PassingCondition = GetPassingCondition (CommonParameters, TargetResultConditionPtr);

        TargetResultConditionPtr->Current = PassingCondition;

      }

    } // BitIndex loop ...

  } // IsNormalFlowFailState

}

/**

  Setup the value for N and P margins for current strobe logical for the specified channel/subchannel/logical rank

  @param[in,out]  CommonParameters    Pointer to the context information
  @param[in]      KnobIndex           Current knob under test (0-based)
  @param[in]      UsePrevious         TRUE: When previous value need to be considered
                                            For N margin: MAX (PreviousN, CurrentN)
                                            For P margin: MIN (PreviousP, CurrentP)
                                      FALSE: Only current value is considered.
  @param[in]      CurrentN            Current value for N side
  @param[in]      CurrentP            Current value for P side
  @param[in]      RankResultsEvenOdd  Pointer to return the results at rank level for EVEN/ODD
  @param[out]     MaxLeftAcrossRankEvenOdd  Pointer for maximum value for left side for EVEN/ODD
  @param[out]     MinRightAcrossRankEvenOdd Pointer for minimum value for right side for EVEN/ODD
**/
VOID
UpdateMaxLeftMinRighEvenOdd (
  IN        COMMON_1D_PARAMETERS      *CommonParameters,
  IN        UINT8                     KnobIndex,
  IN        BOOLEAN                   UsePrevious,
  IN        INT16                     CurrentN,
  IN        INT16                     CurrentP,
  IN        struct rankMargin         RankResultsEvenOdd[MaxChunkSides],    OPTIONAL
      OUT   INT16                     MaxLeftAcrossRankEvenOdd[MaxChunkSides],
      OUT   INT16                     MinRightAcrossRankEvenOdd[MaxChunkSides]
)
{
  INT16   *MaxLeftAcrossRankEvenOddPtr;
  INT16   *MinRightAcrossRankEvenOddPtr;

  if (RankResultsEvenOdd == NULL) {
    return;
  }

  if (KnobIndex < MaxChunkSides) {

    MaxLeftAcrossRankEvenOddPtr   = &MaxLeftAcrossRankEvenOdd[KnobIndex];
    MinRightAcrossRankEvenOddPtr  = &MinRightAcrossRankEvenOdd[KnobIndex];

  } else {

    RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_029);
    MaxLeftAcrossRankEvenOddPtr   = &MaxLeftAcrossRankEvenOdd[EvenChunkSide];
    MinRightAcrossRankEvenOddPtr  = &MinRightAcrossRankEvenOdd[EvenChunkSide];

  }

  if (UsePrevious) {

    if (*MaxLeftAcrossRankEvenOddPtr < CurrentN) {

      *MaxLeftAcrossRankEvenOddPtr = CurrentN;

    }

    if (*MinRightAcrossRankEvenOddPtr > CurrentP) {

      *MinRightAcrossRankEvenOddPtr = CurrentP;

    }

  } else {

    *MaxLeftAcrossRankEvenOddPtr  = CurrentN;
    *MinRightAcrossRankEvenOddPtr = CurrentP;
  }

}

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
)
{
  struct strobeMargin   *StrobeResultsEvenOddPtr;

  if (StrobeResults != NULL) {
    if (UsePrevious) {
      StrobeResults[VerticalSampleIndex].strobe[ChannelIndex][LogicalRank][StrobeLogical].n = \
          MAX (StrobeResults[VerticalSampleIndex].strobe[ChannelIndex][LogicalRank][StrobeLogical].n, CurrentN);
      StrobeResults[VerticalSampleIndex].strobe[ChannelIndex][LogicalRank][StrobeLogical].p = \
          MIN (StrobeResults[VerticalSampleIndex].strobe[ChannelIndex][LogicalRank][StrobeLogical].p, CurrentP);
    } else {
      StrobeResults[VerticalSampleIndex].strobe[ChannelIndex][LogicalRank][StrobeLogical].n = CurrentN;
      StrobeResults[VerticalSampleIndex].strobe[ChannelIndex][LogicalRank][StrobeLogical].p = CurrentP;
    }
  }

  if (StrobeResultsEvenOdd != NULL) {

    if (KnobIndex < MaxChunkSides) {

      StrobeResultsEvenOddPtr = &StrobeResultsEvenOdd[KnobIndex];

    } else {

      RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_026);
      StrobeResultsEvenOddPtr = &StrobeResultsEvenOdd[EvenChunkSide];

    }

    if (UsePrevious) {
      StrobeResultsEvenOddPtr->strobe[ChannelIndex][LogicalRank][StrobeLogical].n = \
          MAX (StrobeResultsEvenOddPtr->strobe[ChannelIndex][LogicalRank][StrobeLogical].n, CurrentN);
      StrobeResultsEvenOddPtr->strobe[ChannelIndex][LogicalRank][StrobeLogical].p = \
          MIN (StrobeResultsEvenOddPtr->strobe[ChannelIndex][LogicalRank][StrobeLogical].p, CurrentP);
    } else {
      StrobeResultsEvenOddPtr->strobe[ChannelIndex][LogicalRank][StrobeLogical].n = CurrentN;
      StrobeResultsEvenOddPtr->strobe[ChannelIndex][LogicalRank][StrobeLogical].p = CurrentP;
    }
  }

}

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
)
{
  UINT8   BitIndexWithinChannel;
  struct bitMargin    *BitResultsEvenOddPtr;

  BitIndexWithinChannel = GetBitIndexWithinChannel (
                            CommonParameters,
                            ChannelIndex,
                            SubChannelIndex,
                            StrobeLogical,
                            BitIndex
                            );

  if (BitResults != NULL) {
    if (UsePrevious) {
      BitResults[VerticalSampleIndex].bits[ChannelIndex][LogicalRank][BitIndexWithinChannel].n = \
          MAX (BitResults[VerticalSampleIndex].bits[ChannelIndex][LogicalRank][BitIndexWithinChannel].n, CurrentN);
      BitResults[VerticalSampleIndex].bits[ChannelIndex][LogicalRank][BitIndexWithinChannel].p = \
          MIN (BitResults[VerticalSampleIndex].bits[ChannelIndex][LogicalRank][BitIndexWithinChannel].p, CurrentP);
    } else {
      BitResults[VerticalSampleIndex].bits[ChannelIndex][LogicalRank][BitIndexWithinChannel].n = CurrentN;
      BitResults[VerticalSampleIndex].bits[ChannelIndex][LogicalRank][BitIndexWithinChannel].p = CurrentP;
    }
  } // BitResults != NULL

  if (BitResultsEvenOdd != NULL) {

    if (KnobIndex < MaxChunkSides) {

      BitResultsEvenOddPtr = &BitResultsEvenOdd[KnobIndex];

    } else {

      RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_027);
      BitResultsEvenOddPtr = &BitResultsEvenOdd[EvenChunkSide];

    }

    if (UsePrevious) {
      BitResultsEvenOddPtr->bits[ChannelIndex][LogicalRank][BitIndexWithinChannel].n = \
          MAX (BitResultsEvenOddPtr->bits[ChannelIndex][LogicalRank][BitIndexWithinChannel].n, CurrentN);
      BitResultsEvenOddPtr->bits[ChannelIndex][LogicalRank][BitIndexWithinChannel].p = \
          MIN (BitResultsEvenOddPtr->bits[ChannelIndex][LogicalRank][BitIndexWithinChannel].p, CurrentP);
    } else {
      BitResultsEvenOddPtr->bits[ChannelIndex][LogicalRank][BitIndexWithinChannel].n = CurrentN;
      BitResultsEvenOddPtr->bits[ChannelIndex][LogicalRank][BitIndexWithinChannel].p = CurrentP;
    }

  }
}

/**

  Initialize the legacy strobe and bit structures

  @param[in,out]  CommonParameters    Pointer to the context information
  @param[in]      ChannelIndex        Current channel under test (0-based)
  @param[in]      LogicalRank         Current logical rank under test (0-based within the channel)
  @param[in]      SubChannelIndex     Current sub-channel under test (0-based)

  @param[out]     StrobeResults       Pointer to return the results at strobe level
  @param[out]     BitResults          Pointer to return the results at bit level

  @param[out]     StrobeResultsEvenOdd  Pointer to return the results at strobe level for EVEN/ODD
  @param[out]     BitResultsEvenOdd     Pointer to return the results at bit level for EVEN/ODD

**/
VOID
InitializeLegacyResults (
  IN  OUT   COMMON_1D_PARAMETERS      *CommonParameters,
  IN        UINT8                     ChannelIndex,
  IN        UINT8                     LogicalRank,
  IN        UINT8                     SubChannelIndex,
      OUT   struct strobeMargin       *StrobeResults, OPTIONAL
      OUT   struct bitMargin          *BitResults,    OPTIONAL
      OUT   struct strobeMargin       StrobeResultsEvenOdd[MaxChunkSides], OPTIONAL
      OUT   struct bitMargin          BitResultsEvenOdd[MaxChunkSides]     OPTIONAL
  )
{
  UINT8   KnobIndex;
  UINT8   StrobeLinear;
  UINT8   StrobeLogical;
  UINT8   BitIndex;

  for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {
    for (StrobeLinear = 0; StrobeLinear < CommonParameters->MaxStrobeAvailable; StrobeLinear++) {

      if (IsResultAtRankLevel (CommonParameters) && StrobeLinear >= 1) {
        //
        // Tag::StrobeLinearBreak::AtRankLevel
        // If operating a delay at Rank Level only strobe 0 will have
        // the final margin information
        //
        continue;
      }

      if (SkipExecutionOnStrobe (CommonParameters, ChannelIndex, StrobeLinear)) {
        continue;
      }

      StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);

      SetStrobeLegacyResult (
        CommonParameters,
        0,             ///< 1D usage, vertical index must be 0
        KnobIndex,
        ChannelIndex,
        LogicalRank,
        SubChannelIndex,
        StrobeLogical,
        FALSE,
        MIN_INT16,
        MAX_INT16,
        StrobeResults,
        StrobeResultsEvenOdd
        );

      if ((CommonParameters->FlagsResults & ResultsPerBit) != 0) {

        for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {

          SetBitLegacyResult (
            CommonParameters,
            0,             ///< 1D usage, vertical index must be 0
            KnobIndex,
            ChannelIndex,
            LogicalRank,
            SubChannelIndex,
            StrobeLogical,
            BitIndex,
            FALSE,
            MIN_INT16,
            MAX_INT16,
            BitResults,
            BitResultsEvenOdd
            );

          if (AreResultsAtByteLevel (CommonParameters, ChannelIndex)) {

            SetBitLegacyResult (
              CommonParameters,
              0,             ///< 1D usage, vertical index must be 0
              KnobIndex,
              ChannelIndex,
              LogicalRank,
              SubChannelIndex,
              StrobeLogical + CommonParameters->NextNibbleOffset,
              BitIndex,
              FALSE,
              MIN_INT16,
              MAX_INT16,
              BitResults,
              BitResultsEvenOdd
              );

          } // AreResultsAtByteLevel
        } // BitIndex loop ...
      } // (CommonParameters->FlagsResults & ResultsPerBit) != 0 && BitResults != NULL
    } // StrobeLinear loop ...
  } // KnobIndex loop ...

}

/**

  Cleanup the results for the legacy strobe and bit structures
  If N >= P, then the margin is considered '0'

  @param[in,out]  CommonParameters    Pointer to the context information
  @param[in]      ChannelIndex        Current channel under test (0-based)
  @param[in]      LogicalRank         Current logical rank under test (0-based within the channel)
  @param[in]      SubChannelIndex     Current sub-channel under test (0-based)

  @param[out]     StrobeResults       Pointer to return the results at strobe level
  @param[out]     BitResults          Pointer to return the results at bit level

  @param[out]     StrobeResultsEvenOdd  Pointer to return the results at strobe level for EVEN/ODD
  @param[out]     BitResultsEvenOdd     Pointer to return the results at bit level for EVEN/ODD


**/
VOID
CleanUpLegacyResults (
  IN  OUT   COMMON_1D_PARAMETERS      *CommonParameters,
  IN        UINT8                     ChannelIndex,
  IN        UINT8                     LogicalRank,
  IN        UINT8                     SubChannelIndex,
      OUT   struct strobeMargin       *StrobeResults, OPTIONAL
      OUT   struct bitMargin          *BitResults,    OPTIONAL
      OUT   struct strobeMargin       StrobeResultsEvenOdd[MaxChunkSides], OPTIONAL
      OUT   struct bitMargin          BitResultsEvenOdd[MaxChunkSides]     OPTIONAL
  )
{
  UINT8   KnobIndex;
  UINT8   StrobeLinear;
  UINT8   StrobeLogical;
  UINT8   BitIndex;
  UINT8   BitIndexWithinChannel;
  struct strobeMargin   *StrobeResultsEvenOddPtr;
  struct bitMargin      *BitResultsEvenOddPtr;

  for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {
    for (StrobeLinear = 0; StrobeLinear < CommonParameters->MaxStrobeAvailable; StrobeLinear++) {

      if (IsResultAtRankLevel (CommonParameters) && StrobeLinear >= 1) {
        //
        // Tag::StrobeLinearBreak::AtRankLevel
        // If operating a delay at Rank Level only strobe 0 will have
        // the final margin information
        //
        continue;
      }

      if (SkipExecutionOnStrobe (CommonParameters, ChannelIndex, StrobeLinear)) {
        continue;
      }

      StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);

      if (StrobeResults != NULL) {

        if (StrobeResults->strobe[ChannelIndex][LogicalRank][StrobeLogical].n >=
            StrobeResults->strobe[ChannelIndex][LogicalRank][StrobeLogical].p) {

          SetStrobeLegacyResult (
            CommonParameters,
            0,             ///< 1D usage, vertical index must be 0
            KnobIndex,
            ChannelIndex,
            LogicalRank,
            SubChannelIndex,
            StrobeLogical,
            FALSE,
            0,
            0,
            StrobeResults,
            NULL
            );
        }

        if (AreResultsAtByteLevel (CommonParameters, ChannelIndex)) {
          //
          // Same value for upper nibble, when x8 group is swept
          //
          SetStrobeLegacyResult (
            CommonParameters,
            0,             ///< 1D usage, vertical index must be 0
            KnobIndex,
            ChannelIndex,
            LogicalRank,
            SubChannelIndex,
            StrobeLogical + CommonParameters->NextNibbleOffset,
            FALSE,
            StrobeResults->strobe[ChannelIndex][LogicalRank][StrobeLogical].n,
            StrobeResults->strobe[ChannelIndex][LogicalRank][StrobeLogical].p,
            StrobeResults,
            NULL
            );

        }

      } // StrobeResults != NULL

      if (StrobeResultsEvenOdd != NULL) {

        if (KnobIndex < MaxChunkSides) {

          StrobeResultsEvenOddPtr = &StrobeResultsEvenOdd[KnobIndex];

        } else {

          RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_030);
          StrobeResultsEvenOddPtr = &StrobeResultsEvenOdd[EvenChunkSide];

        }

        if (StrobeResultsEvenOddPtr->strobe[ChannelIndex][LogicalRank][StrobeLogical].n >=
            StrobeResultsEvenOddPtr->strobe[ChannelIndex][LogicalRank][StrobeLogical].p) {

          SetStrobeLegacyResult (
            CommonParameters,
            0,             ///< 1D usage, vertical index must be 0
            KnobIndex,
            ChannelIndex,
            LogicalRank,
            SubChannelIndex,
            StrobeLogical,
            FALSE,
            0,
            0,
            NULL,
            StrobeResultsEvenOdd
            );
        }

        if (AreResultsAtByteLevel (CommonParameters, ChannelIndex)) {
          //
          // Same value for upper nibble, when x8 group is swept
          //
          SetStrobeLegacyResult (
            CommonParameters,
            0,             ///< 1D usage, vertical index must be 0
            KnobIndex,
            ChannelIndex,
            LogicalRank,
            SubChannelIndex,
            StrobeLogical + CommonParameters->NextNibbleOffset,
            FALSE,
            StrobeResultsEvenOddPtr->strobe[ChannelIndex][LogicalRank][StrobeLogical].n,
            StrobeResultsEvenOddPtr->strobe[ChannelIndex][LogicalRank][StrobeLogical].p,
            NULL,
            StrobeResultsEvenOdd
            );

        }

      } // StrobeResults != NULL

      if ((CommonParameters->FlagsResults & ResultsPerBit) != 0) {

        for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {

          BitIndexWithinChannel = GetBitIndexWithinChannel (
                                    CommonParameters,
                                    ChannelIndex,
                                    SubChannelIndex,
                                    StrobeLogical,
                                    BitIndex
                                    );
          if (BitResults != NULL) {
            if (BitResults->bits[ChannelIndex][LogicalRank][BitIndexWithinChannel].n >=
                BitResults->bits[ChannelIndex][LogicalRank][BitIndexWithinChannel].p) {

              SetBitLegacyResult (
                CommonParameters,
                0,             ///< 1D usage, vertical index must be 0
                KnobIndex,
                ChannelIndex,
                LogicalRank,
                SubChannelIndex,
                StrobeLogical,
                BitIndex,
                FALSE,
                0,
                0,
                BitResults,
                NULL
                );

            }

            if (AreResultsAtByteLevel (CommonParameters, ChannelIndex)) {

              BitIndexWithinChannel = GetBitIndexWithinChannel (
                                        CommonParameters,
                                        ChannelIndex,
                                        SubChannelIndex,
                                        StrobeLogical + CommonParameters->NextNibbleOffset,
                                        BitIndex
                                        );

              if (BitResults->bits[ChannelIndex][LogicalRank][BitIndexWithinChannel].n >=
                  BitResults->bits[ChannelIndex][LogicalRank][BitIndexWithinChannel].p) {

                SetBitLegacyResult (
                  CommonParameters,
                  0,             ///< 1D usage, vertical index must be 0
                  KnobIndex,
                  ChannelIndex,
                  LogicalRank,
                  SubChannelIndex,
                  StrobeLogical + CommonParameters->NextNibbleOffset,
                  BitIndex,
                  FALSE,
                  0,
                  0,
                  BitResults,
                  NULL
                  );
              }

            } // AreResultsAtByteLevel
          } // BitResults != NULL

          if (BitResultsEvenOdd != NULL) {

            if (KnobIndex < MaxChunkSides) {

              BitResultsEvenOddPtr = &BitResultsEvenOdd[KnobIndex];

            } else {

              RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_030);
              BitResultsEvenOddPtr = &BitResultsEvenOdd[EvenChunkSide];

            }

            if (BitResultsEvenOddPtr->bits[ChannelIndex][LogicalRank][BitIndexWithinChannel].n >=
                BitResultsEvenOddPtr->bits[ChannelIndex][LogicalRank][BitIndexWithinChannel].p) {

              SetBitLegacyResult (
                CommonParameters,
                0,             ///< 1D usage, vertical index must be 0
                KnobIndex,
                ChannelIndex,
                LogicalRank,
                SubChannelIndex,
                StrobeLogical,
                BitIndex,
                FALSE,
                0,
                0,
                NULL,
                BitResultsEvenOdd
                );

            }

            if (AreResultsAtByteLevel (CommonParameters, ChannelIndex)) {

              BitIndexWithinChannel = GetBitIndexWithinChannel (
                                        CommonParameters,
                                        ChannelIndex,
                                        SubChannelIndex,
                                        StrobeLogical + CommonParameters->NextNibbleOffset,
                                        BitIndex
                                        );

              if (BitResultsEvenOddPtr->bits[ChannelIndex][LogicalRank][BitIndexWithinChannel].n >=
                  BitResultsEvenOddPtr->bits[ChannelIndex][LogicalRank][BitIndexWithinChannel].p) {

                SetBitLegacyResult (
                  CommonParameters,
                  0,             ///< 1D usage, vertical index must be 0
                  KnobIndex,
                  ChannelIndex,
                  LogicalRank,
                  SubChannelIndex,
                  StrobeLogical + CommonParameters->NextNibbleOffset,
                  BitIndex,
                  FALSE,
                  0,
                  0,
                  NULL,
                  BitResultsEvenOdd
                  );
              }

            } // AreResultsAtByteLevel
          } // BitResultsEvenOdd != NULL
        } // BitIndex loop ...
      } // (CommonParameters->FlagsResults & ResultsPerBit) != 0 && BitResults != NULL
    } // StrobeLinear loop ...
  } // KnobIndex loop ...

}

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
  )
{
  UINT8   ChannelIndex;
  UINT8   SubChannelIndex;
  UINT8   MaxChDdr;
  UINT8   StrobeLogical;
  UINT8   StrobeLinear;
  UINT8   KnobIndex;
  UINT8   BitIndex;
  UINT8   LogicalRank;
  INT16   MaxLeftAcrossRank;
  INT16   MinRightAcrossRank;
  INT16   MaxLeftAcrossRankEvenOdd[MaxChunkSides];
  INT16   MinRightAcrossRankEvenOdd[MaxChunkSides];
  INT16   *MaxLeftAcrossRankEvenOddPtr;
  INT16   *MinRightAcrossRankEvenOddPtr;
  BOOLEAN UsePrevious;
  INT16   CurrentN;
  INT16   CurrentP;

  struct baseMargin *StrobeMargin;
  struct baseMargin *BitMargin;
  struct rankMargin *RankResultsEvenOddPtr;

  MaxChDdr = CommonParameters->MaxChDdr;


  for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {

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

    MaxLeftAcrossRank   = MIN_INT16;
    MinRightAcrossRank  = MAX_INT16;

    for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {
      if (KnobIndex < MaxChunkSides) {

        UpdateMaxLeftMinRighEvenOdd (
            CommonParameters,
            KnobIndex,
            FALSE,
            MIN_INT16,
            MAX_INT16,
            RankResultsEvenOdd,
            MaxLeftAcrossRankEvenOdd,
            MinRightAcrossRankEvenOdd
            );
      }
    } // KnobIndex loop ...

    for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {

      InitializeLegacyResults (
        CommonParameters,
        ChannelIndex,
        LogicalRank,
        SubChannelIndex,
        StrobeResults,
        BitResults,
        StrobeResultsEvenOdd,
        BitResultsEvenOdd
        );

      for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {
        for (StrobeLinear = 0; StrobeLinear < CommonParameters->MaxStrobeAvailable; StrobeLinear++) {

          if (IsResultAtRankLevel (CommonParameters) && StrobeLinear >= 1) {
            //
            // Tag::StrobeLinearBreak::AtRankLevel
            // If operating a delay at Rank Level only strobe 0 will have
            // the final margin information
            //
            continue;
          }

          if (SkipExecutionOnStrobe (CommonParameters, ChannelIndex, StrobeLinear)) {
            continue;
          }

          StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);


          StrobeMargin = GetResultPtrAtStrobeLevel (
                           CommonParameters,
                           KnobIndex,
                           ChannelIndex,
                           StrobeLogical,
                           CommonParameters->Results
                           );
          //
          // Results are only valid when the N edge is smaller than P edge
          //
          if (StrobeMargin->n < StrobeMargin->p) {

            UsePrevious = TRUE;
            CurrentN    = StrobeMargin->n;
            CurrentP    = StrobeMargin->p;

            if (MaxLeftAcrossRank < StrobeMargin->n) {

              MaxLeftAcrossRank = StrobeMargin->n;

            }

            if (MinRightAcrossRank > StrobeMargin->p) {

              MinRightAcrossRank = StrobeMargin->p;

            }

          } else {

            UsePrevious = FALSE;
            CurrentN    = MAX_INT16;
            CurrentP    = MIN_INT16;

            MaxLeftAcrossRank   = MAX_INT16;
            MinRightAcrossRank  = MIN_INT16;

          }

          SetStrobeLegacyResult (
            CommonParameters,
            0,             ///< 1D usage, vertical index must be 0
            KnobIndex,
            ChannelIndex,
            LogicalRank,
            SubChannelIndex,
            StrobeLogical,
            UsePrevious,
            CurrentN,
            CurrentP,
            StrobeResults,
            StrobeResultsEvenOdd
            );

          UpdateMaxLeftMinRighEvenOdd (
            CommonParameters,
            KnobIndex,
            UsePrevious,
            CurrentN,
            CurrentP,
            RankResultsEvenOdd,
            MaxLeftAcrossRankEvenOdd,
            MinRightAcrossRankEvenOdd
            );


          if (AreResultsAtByteLevel (CommonParameters, ChannelIndex)) {

            StrobeMargin = GetResultPtrAtStrobeLevel (
                             CommonParameters,
                             KnobIndex,
                             ChannelIndex,
                             StrobeLogical + CommonParameters->NextNibbleOffset,
                             CommonParameters->Results
                             );

            if (StrobeMargin->n < StrobeMargin->p) {

              UsePrevious = TRUE;
              CurrentN    = StrobeMargin->n;
              CurrentP    = StrobeMargin->p;

              if (MaxLeftAcrossRank < StrobeMargin->n) {

                MaxLeftAcrossRank = StrobeMargin->n;

              }

              if (MinRightAcrossRank > StrobeMargin->p) {

                MinRightAcrossRank = StrobeMargin->p;

              }

            } else {

              UsePrevious = FALSE;
              CurrentN    = MAX_INT16;
              CurrentP    = MIN_INT16;

              MaxLeftAcrossRank   = MAX_INT16;
              MinRightAcrossRank  = MIN_INT16;

            }

            SetStrobeLegacyResult (
              CommonParameters,
              0,             ///< 1D usage, vertical index must be 0
              KnobIndex,
              ChannelIndex,
              LogicalRank,
              SubChannelIndex,
              StrobeLogical,
              UsePrevious,
              CurrentN,
              CurrentP,
              StrobeResults,
              StrobeResultsEvenOdd
              );

            UpdateMaxLeftMinRighEvenOdd (
              CommonParameters,
              KnobIndex,
              UsePrevious,
              CurrentN,
              CurrentP,
              RankResultsEvenOdd,
              MaxLeftAcrossRankEvenOdd,
              MinRightAcrossRankEvenOdd
              );

          } // AreResultsAtByteLevel

          if ((CommonParameters->FlagsResults & ResultsPerBit) != 0) {

            for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {

              BitMargin = GetResultPtrAtBitLevel (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, BitIndex, CommonParameters->Results);

              if (BitMargin == NULL) {
                RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_023);
                continue;
              }


              if (BitMargin->n < BitMargin->p) {

                UsePrevious = TRUE;
                CurrentN    = BitMargin->n;
                CurrentP    = BitMargin->p;

              } else {

                UsePrevious = FALSE;
                CurrentN    = MAX_INT16;
                CurrentP    = MIN_INT16;

              }

              SetBitLegacyResult (
                CommonParameters,
                0,             ///< 1D usage, vertical index must be 0
                KnobIndex,
                ChannelIndex,
                LogicalRank,
                SubChannelIndex,
                StrobeLogical,
                BitIndex,
                UsePrevious,
                CurrentN,
                CurrentP,
                BitResults,
                BitResultsEvenOdd
                );


              if (AreResultsAtByteLevel (CommonParameters, ChannelIndex)) {

                BitMargin = GetResultPtrAtBitLevel (
                              CommonParameters,
                              KnobIndex,
                              ChannelIndex,
                              StrobeLogical + CommonParameters->NextNibbleOffset,
                              BitIndex,
                              CommonParameters->Results
                              );

                if (BitMargin == NULL) {
                  RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_024);
                  continue;
                }

                if (BitMargin->n < BitMargin->p) {

                  UsePrevious = TRUE;
                  CurrentN    = BitMargin->n;
                  CurrentP    = BitMargin->p;

                } else {

                  UsePrevious = FALSE;
                  CurrentN    = MAX_INT16;
                  CurrentP    = MIN_INT16;

                }

                SetBitLegacyResult (
                  CommonParameters,
                  0,             ///< 1D usage, vertical index must be 0
                  KnobIndex,
                  ChannelIndex,
                  LogicalRank,
                  SubChannelIndex,
                  StrobeLogical + CommonParameters->NextNibbleOffset,
                  BitIndex,
                  UsePrevious,
                  CurrentN,
                  CurrentP,
                  BitResults,
                  BitResultsEvenOdd
                  );

              } // AreResultsAtByteLevel
            } // BitIndex loop
          } // ResultsPerBit
        } // StrobeLinear loop ...
      } // KnobIndex loop ...

      CleanUpLegacyResults (
        CommonParameters,
        ChannelIndex,
        LogicalRank,
        SubChannelIndex,
        StrobeResults,
        BitResults,
        StrobeResultsEvenOdd,
        BitResultsEvenOdd
        );

    } // SubChannelIndex loop ...

    if (RankResults != NULL) {
      if (MaxLeftAcrossRank < MinRightAcrossRank) {

        RankResults->rank[ChannelIndex][LogicalRank].n = MaxLeftAcrossRank;
        RankResults->rank[ChannelIndex][LogicalRank].p = MinRightAcrossRank;

      } else {

        RankResults->rank[ChannelIndex][LogicalRank].n = 0;
        RankResults->rank[ChannelIndex][LogicalRank].p = 0;

      }
    } // RankResults != NULL

    if (RankResultsEvenOdd != NULL) {
      for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {
        if (KnobIndex < MaxChunkSides) {

          RankResultsEvenOddPtr         = &RankResultsEvenOdd[KnobIndex];
          MaxLeftAcrossRankEvenOddPtr   = &MaxLeftAcrossRankEvenOdd[KnobIndex];
          MinRightAcrossRankEvenOddPtr  = &MinRightAcrossRankEvenOdd[KnobIndex];

        } else {

          RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_028);
          RankResultsEvenOddPtr         = &RankResultsEvenOdd[EvenChunkSide];
          MaxLeftAcrossRankEvenOddPtr   = &MaxLeftAcrossRankEvenOdd[EvenChunkSide];
          MinRightAcrossRankEvenOddPtr  = &MinRightAcrossRankEvenOdd[EvenChunkSide];

        }

        if (*MaxLeftAcrossRankEvenOddPtr < *MinRightAcrossRankEvenOddPtr) {

          RankResultsEvenOddPtr->rank[ChannelIndex][LogicalRank].n  = *MaxLeftAcrossRankEvenOddPtr;
          RankResultsEvenOddPtr->rank[ChannelIndex][LogicalRank].p  = *MinRightAcrossRankEvenOddPtr;

        } else {

          RankResultsEvenOddPtr->rank[ChannelIndex][LogicalRank].n  = 0;
          RankResultsEvenOddPtr->rank[ChannelIndex][LogicalRank].p  = 0;

        }


      } // KnobIndex loop ...
    } // RankResultsEvenOdd != NULL

  } // ChannelIndex loop

  return MRC_STATUS_SUCCESS;
}
