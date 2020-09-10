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
)
{
  UINT8   MaxChDdr;
  UINT8   ChannelIndex;
  UINT8   SubChannelIndex;
  UINT8   StrobeLogical;
  UINT8   StrobeLinear;
  BOOLEAN AllAtEndState;

  MaxChDdr = CommonParameters->MaxChDdr;
  AllAtEndState = TRUE;

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
        // All states are handled at a strobe level
        //
        StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);

        if (State == ST_XX_END) {

          if (CommonParameters->CurrentState[ChannelIndex][StrobeLogical] != ST_END &&
              CommonParameters->CurrentState[ChannelIndex][StrobeLogical] != ST_LE_END &&
              CommonParameters->CurrentState[ChannelIndex][StrobeLogical] != ST_RE_END) {

            AllAtEndState = FALSE;
            break;

          }

        } else if (CommonParameters->CurrentState[ChannelIndex][StrobeLogical] != State) {

          AllAtEndState = FALSE;
          break;

        }

      } // StrobeLinear loop ...
    } // SubChannelIndex loop ...

    if (!AllAtEndState) {
      break;
    }

  } // ChannelIndex loop ...


  return AllAtEndState;
}

/*

  Set an specified state across all channels and strobes

  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      State             FSM to be set across all channels/strobes

*/
VOID
PropagateStateAcrossChannelsStrobes (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         STATES_1D               State
)
{
  UINT8   MaxChDdr;
  UINT8   ChannelIndex;
  UINT8   SubChannelIndex;
  UINT8   StrobeLogical;
  UINT8   StrobeLinear;

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
        // All states are handled at a strobe level
        //
        StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);

        CommonParameters->CurrentState[ChannelIndex][StrobeLogical] = State;

      } // StrobeLinear loop ...
    } // SubChannelIndex loop ...

  } // ChannelIndex loop ...
}

/*

  Executes current actions based on the current state for each channel/strobe

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       Function was executed without errors

*/
MRC_STATUS
ExecuteStateAction (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
)
{
  UINT8   ChannelIndex;
  UINT8   SubChannelIndex;
  UINT8   MaxChDdr;
  UINT8   Mode;
  UINT8   StrobeLogical;
  UINT8   StrobeLinear;
  UINT8   KnobIndex;
  INT16   *DelayOffsetPtr;
  INT16   OffsetLeftCached;
  INT16   OffsetRightCached;
  INT16   OffsetLeft;
  INT16   OffsetRight;

  MaxChDdr = CommonParameters->MaxChDdr;

  //
  // Clear delays offsets
  //
  ClearDelay (CommonParameters, CommonParameters->DelayOffset);

  //
  // Default mode
  //
  Mode = GSM_WRITE_OFFSET | GSM_UPDATE_CACHE;

  for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {

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
          // All states are handled at a strobe level
          //
          StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);

          DelayOffsetPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, CommonParameters->DelayOffset);

          switch (CommonParameters->CurrentState[ChannelIndex][StrobeLogical]) {
          case ST_AC_INIT:

            GetOffsetLeftRight (CommonParameters, ChannelIndex, StrobeLogical, &OffsetLeftCached, &OffsetRightCached);

            if (OffsetLeftCached < OffsetRightCached) {

              OffsetLeft  = OffsetLeftCached;
              OffsetRight = OffsetRightCached;

            } else {

              OffsetLeft  = CommonParameters->OffsetLeft;
              OffsetRight = CommonParameters->OffsetRight;

            }

            if (CommonParameters->CurrentEdge == LEFT_EDGE) {
              *DelayOffsetPtr = OffsetLeft;
            } else {
              *DelayOffsetPtr = OffsetRight;
            }

            if (CommonParameters->FlagsOffsets & UseOffsetAsAbsolute) {
              Mode = GSM_FORCE_WRITE | GSM_UPDATE_CACHE;
            }
            break;
          case ST_AC_FIRST_SAMPLE:
            *DelayOffsetPtr = 0;
            break;
          case ST_LE_AC_INC_SE_PASS:
          case ST_RE_AC_INC_SE_FAIL:
          case ST_RE_AC_RSO_AC_INC_SE_PASS:
          case ST_LE_AC_INC_SE_PASS_TS:
          case ST_RE_AC_INC_SE_FAIL_TS:
            *DelayOffsetPtr = CommonParameters->StepSize;
            break;
          case ST_LE_FN_AC_DEC_SE_FAIL:
            *DelayOffsetPtr = -SWEEP_FINE_STEP_SIZE;
            break;
          case ST_RE_FN_AC_INC_SE_FAIL:
            *DelayOffsetPtr = SWEEP_FINE_STEP_SIZE;
            break;
          case ST_LE_AC_DEC_SE_FAIL:
          case ST_RE_AC_DEC_SE_PASS:
          case ST_LE_AC_RSO_AC_DEC_SE_PASS:
            *DelayOffsetPtr = 0 - CommonParameters->StepSize;
            break;
          case ST_END:
            if ((CommonParameters->FlagsFsm & TrackingSweep) == TrackingSweep) {
              //
              // Keep incrementing until all strobes are at ST_END
              //
              *DelayOffsetPtr = CommonParameters->StepSize;
            }
            break;
          case ST_LE_END:
            if ((CommonParameters->FlagsFsm & ContinousMode) && (CommonParameters->FlagsFsm & TrackingSweep) != TrackingSweep) {
              *DelayOffsetPtr = 0 - CommonParameters->StepSize;
            }
            break;
          case ST_RE_END:
            if ((CommonParameters->FlagsFsm & ContinousMode) && (CommonParameters->FlagsFsm & TrackingSweep) != TrackingSweep) {
              *DelayOffsetPtr = CommonParameters->StepSize;
            }
            break;
          default:
            RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_015);
            break;
          }

        } // StrobeLinear loop ...

      } // SubChannelIndex loop ...

    } // ChannelIndex loop

  } // KnobIndex loop ...

  VerifyAndSetOffset (CommonParameters, Mode, CommonParameters->DelayOffset);

  return MRC_STATUS_SUCCESS;
} // ExecuteStateAction

/*

  Define the next state based on the current state for each channel/strobe
  and the passing or failing conditions

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       Function was executed without errors

*/
MRC_STATUS
GetNextState (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
)
{
  UINT8   ChannelIndex;
  UINT8   SubChannelIndex;
  UINT8   MaxChDdr;
  UINT8   StrobeLogical;
  UINT8   StrobeLogicalForStrobeLinear0;
  UINT8   StrobeLinear;
  UINT8   KnobIndex;
  INT16   OffsetLeftCached;
  INT16   OffsetRightCached;
  INT16   OffsetLeft;
  INT16   OffsetRight;
  INT16   JitterGap;
  INT16   *OriginalDelayPtr;
  INT16   *CurrentDelayPtr;
  INT16   *DelayOffsetPtr;
  STATES_1D NextState;
  STATES_1D CurrentState;
  STATES_1D EndState;

  MaxChDdr = CommonParameters->MaxChDdr;

  //
  // Clear delays offsets
  //
  ClearDelay (CommonParameters, CommonParameters->DelayOffset);

  for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
      for (StrobeLinear = 0; StrobeLinear < CommonParameters->MaxStrobeAvailable; StrobeLinear++) {

        if (SkipExecutionOnStrobe (CommonParameters, ChannelIndex, StrobeLinear)) {
          continue;
        }

        if (IsSingleToMultipleDelay (CommonParameters) && StrobeLinear >= 1) {
          continue;
        }

        StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);
        //
        // Next state by default
        //
        CurrentState  = CommonParameters->CurrentState[ChannelIndex][StrobeLogical];
        NextState     = CurrentState;

        if (IsLeftState (CommonParameters, CommonParameters->CurrentState[ChannelIndex][StrobeLogical])) {
          EndState = ST_LE_END;
        } else {
          EndState = ST_RE_END;
        }

        switch (CommonParameters->CurrentState[ChannelIndex][StrobeLogical]) {
        case ST_AC_INIT:
          NextState = ST_AC_FIRST_SAMPLE;
          break;
        case ST_AC_FIRST_SAMPLE:
          if ((CommonParameters->FlagsFsm & TrackingSweep) == TrackingSweep) {

            NextState = ST_LE_AC_INC_SE_PASS_TS;

          } else if (CommonParameters->CurrentEdge == LEFT_EDGE) {

            if (CommonParameters->FlagsResults & FocusOnTransition) {

              NextState = ST_LE_AC_DEC_SE_FAIL;

            } else if (StrobePassAcrossAllKnobs (CommonParameters, ChannelIndex, StrobeLogical)) {

              NextState = ST_LE_AC_DEC_SE_FAIL;

            } else {
              NextState = ST_LE_AC_INC_SE_PASS;
            }

          } else { // RIGHT_EDGE

            if (CommonParameters->FlagsResults & FocusOnTransition) {
              NextState = ST_RE_AC_INC_SE_FAIL;
            } else if (StrobePassAcrossAllKnobs (CommonParameters, ChannelIndex, StrobeLogical)) {
              NextState = ST_RE_AC_INC_SE_FAIL;
            } else {
              NextState = ST_RE_AC_DEC_SE_PASS;
            }

          }
          break;
        case ST_LE_FN_AC_DEC_SE_FAIL:
        case ST_LE_AC_DEC_SE_FAIL:
        case ST_RE_FN_AC_INC_SE_FAIL:
        case ST_RE_AC_INC_SE_FAIL:
        case ST_RE_AC_INC_SE_FAIL_TS:
          if (IsStrobeAtLimit (CommonParameters, ChannelIndex, StrobeLogical)) {
            if (CurrentState == ST_RE_AC_INC_SE_FAIL_TS) {
              NextState = ST_END;
            } else {
              NextState = EndState;
            }
          }
          if ((CommonParameters->FlagsResults & ResultsPerSubChannel) != 0 && IsSubChannelFailed (CommonParameters, ChannelIndex, SubChannelIndex)) {

            NextState = EndState;

          } else if (CommonParameters->FlagsResults & ResultsPerBit) {
            if (AllBitFailAcrossAllKnobs (CommonParameters, ChannelIndex, StrobeLogical)) {
              if (CurrentState == ST_RE_AC_INC_SE_FAIL_TS) {
                NextState = ST_END;
              } else {
                NextState = EndState;
              }
            }
          } else {
            if (AnyBitFailAcrossAllKnobs (CommonParameters, ChannelIndex, StrobeLogical)) {
              if (CurrentState == ST_RE_AC_INC_SE_FAIL_TS) {
                NextState = ST_END;
              } else {
                NextState = EndState;
              }
            }
          }

          if ((CommonParameters->FlagsFsm & TransitionFineFsm) != 0 &&
              (CommonParameters->FlagsResults & ResultsPerSubChannel) == 0) {

            if ((CurrentState == ST_LE_AC_DEC_SE_FAIL || CurrentState == ST_RE_AC_INC_SE_FAIL) &&
                NextState == EndState) {

              if (CommonParameters->CurrentEdge == LEFT_EDGE) {

                NextState = ST_LE_AC_INC_SE_PASS;

              } else { // RIGHT_EDGE

                NextState = ST_RE_AC_DEC_SE_PASS;

              }

            }

          }

          break;
        case ST_LE_AC_INC_SE_PASS:
        case ST_RE_AC_DEC_SE_PASS:
        case ST_LE_AC_INC_SE_PASS_TS:
          if (IsStrobeAtLimit (CommonParameters, ChannelIndex, StrobeLogical) &&
              !StrobePassAcrossAllKnobs (CommonParameters, ChannelIndex, StrobeLogical)) {

            if (CurrentState == ST_LE_AC_INC_SE_PASS_TS) {
              NextState = ST_END;
            } else if (CommonParameters->FlagsFsm & OneWayOnly) {
              NextState = EndState;
            } else if (CommonParameters->CurrentEdge == LEFT_EDGE) {
              NextState = ST_LE_AC_RSO_AC_DEC_SE_PASS;
            } else {
              NextState = ST_RE_AC_RSO_AC_INC_SE_PASS;
            }

            for (KnobIndex = 0;
                KnobIndex < CommonParameters->KnobsPerGroup && CurrentState != ST_LE_AC_INC_SE_PASS_TS;
                KnobIndex++
                ) {

              OriginalDelayPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, CommonParameters->OriginalDelays);
              CurrentDelayPtr  = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, CommonParameters->CurrentDelays);
              DelayOffsetPtr   = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, CommonParameters->DelayOffset);

              *DelayOffsetPtr = *OriginalDelayPtr - *CurrentDelayPtr;

              GetOffsetLeftRight (CommonParameters, ChannelIndex, StrobeLogical, &OffsetLeftCached, &OffsetRightCached);

              if (OffsetLeftCached < OffsetRightCached) {

                OffsetLeft  = OffsetLeftCached;
                OffsetRight = OffsetRightCached;

              } else {

                OffsetLeft  = CommonParameters->OffsetLeft;
                OffsetRight = CommonParameters->OffsetRight;

              }

              if ((CommonParameters->FlagsOffsets & UseOffsetAsAbsolute) == 0) {
                if (CommonParameters->CurrentEdge == LEFT_EDGE) {
                  *DelayOffsetPtr += OffsetLeft;
                } else {
                  *DelayOffsetPtr += OffsetRight;
                }
              } else {
                if (CommonParameters->CurrentEdge == LEFT_EDGE) {
                  *DelayOffsetPtr = OffsetLeft  - *CurrentDelayPtr;
                } else {
                  *DelayOffsetPtr = OffsetRight - *CurrentDelayPtr;
                }
              }

            } // KnobIndex loop ...

            break;

          }
          //
          // For PerBit results or if it a IsSingleToMultipleDelay we need to search
          // now the passing to failing transition to get more accurate results
          // Tag::IsSingleToMultipleDelay::Pass2Fail
          //
          if ((CommonParameters->FlagsResults & ResultsPerBit) || IsSingleToMultipleDelay (CommonParameters)) {
            if (StrobePassAcrossAllKnobs (CommonParameters, ChannelIndex, StrobeLogical) &&
                AllBitMatchesDither (CommonParameters, ChannelIndex, StrobeLogical)) {

              if (CurrentState == ST_LE_AC_INC_SE_PASS_TS) {

                NextState = ST_LE_END;

              } else if (CommonParameters->CurrentEdge == LEFT_EDGE) {

                if (CommonParameters->FlagsFsm & TransitionFineFsm) {

                  NextState = ST_LE_FN_AC_DEC_SE_FAIL;

                } else {

                  NextState = ST_LE_AC_DEC_SE_FAIL;

                }

              } else {

                if (CommonParameters->FlagsFsm & TransitionFineFsm) {

                  NextState = ST_RE_FN_AC_INC_SE_FAIL;

                } else {

                  NextState = ST_RE_AC_INC_SE_FAIL;

                }

              }

              for (KnobIndex = 0;
                  KnobIndex < CommonParameters->KnobsPerGroup &&
                  CurrentState != ST_LE_AC_INC_SE_PASS_TS &&
                  ((CommonParameters->FlagsFsm & TransitionFineFsm) == 0);
                  KnobIndex++
                  ) {

                DelayOffsetPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, CommonParameters->DelayOffset);

                if (CommonParameters->CurrentEdge == LEFT_EDGE) {
                  JitterGap = JITTER_GAP + CommonParameters->StepSize;
                } else {
                  JitterGap = 0 - JITTER_GAP - CommonParameters->StepSize;
                }

                *DelayOffsetPtr = JitterGap;

              } // KnobIndex loop ...
            }
          } else {
            if (StrobePassAcrossAllKnobs (CommonParameters, ChannelIndex, StrobeLogical) &&
                AllBitMatchesDither (CommonParameters, ChannelIndex, StrobeLogical)) {
              if (CurrentState == ST_LE_AC_INC_SE_PASS_TS) {

                NextState = ST_LE_END;

              } else {
                if (((CommonParameters->FlagsFsm & TransitionFineFsm) != 0) &&
                    (CurrentState == ST_LE_AC_INC_SE_PASS || CurrentState == ST_RE_AC_DEC_SE_PASS)) {

                  if (CommonParameters->CurrentEdge == LEFT_EDGE) {

                    NextState = ST_LE_FN_AC_DEC_SE_FAIL;

                  } else {

                    NextState = ST_RE_FN_AC_INC_SE_FAIL;

                  }

                } else {

                  NextState = EndState;

                }

              }
            }
          }

          if (NextState == EndState) {
            AdjustDitherResults1D (
              CommonParameters,
              ChannelIndex,
              StrobeLogical,
              NextState
              );
            if (AreResultsAtByteLevel (CommonParameters, ChannelIndex)) {
              AdjustDitherResults1D (
                CommonParameters,
                ChannelIndex,
                StrobeLogical + CommonParameters->NextNibbleOffset,
                NextState
                );
            }
          }

          break;
        case ST_RE_AC_RSO_AC_INC_SE_PASS:
        case ST_LE_AC_RSO_AC_DEC_SE_PASS:
          if (IsStrobeAtLimit (CommonParameters, ChannelIndex, StrobeLogical)) {
            NextState = EndState;
            break;
          }
          if (StrobePassAcrossAllKnobs (CommonParameters, ChannelIndex, StrobeLogical) &&
              AllBitMatchesDither (CommonParameters, ChannelIndex, StrobeLogical)) {
            if (CommonParameters->CurrentEdge == LEFT_EDGE) {
              if (CommonParameters->FlagsFsm & TransitionFineFsm) {

                NextState = ST_LE_FN_AC_DEC_SE_FAIL;

              } else {

                NextState = ST_LE_AC_DEC_SE_FAIL;

              }

            } else {

              if (CommonParameters->FlagsFsm & TransitionFineFsm) {

                NextState = ST_RE_FN_AC_INC_SE_FAIL;

              } else {

                NextState = ST_RE_AC_INC_SE_FAIL;

              }

            }
            for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {

              DelayOffsetPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, CommonParameters->DelayOffset);

              if (CommonParameters->CurrentEdge == LEFT_EDGE) {
                JitterGap = 0 - JITTER_GAP + CommonParameters->StepSize;
              } else {
                JitterGap = JITTER_GAP - CommonParameters->StepSize;
              }

              *DelayOffsetPtr = JitterGap;
            } // KnobIndex loop ...
          }

          break;
        case ST_LE_END:
          if ((CommonParameters->FlagsFsm & TrackingSweep) == TrackingSweep) {
            NextState = ST_RE_AC_INC_SE_FAIL_TS;
          } else if ((CommonParameters->FlagsFsm & ContinousMode)) {
            if (IsStrobeAtLimit (CommonParameters, ChannelIndex, StrobeLogical)) {
              NextState = ST_END;
            } else {
              NextState = ST_LE_END;
            }
          } else {
            NextState = ST_END;
          }
          break;
        case ST_RE_END:
          if ((CommonParameters->FlagsFsm & ContinousMode)) {
            if (IsStrobeAtLimit (CommonParameters, ChannelIndex, StrobeLogical)) {
              NextState = ST_END;
            } else {
              NextState = ST_RE_END;
            }
          } else {
            NextState = ST_END;
          }
          break;
        case ST_END:
          break;
        default:
          RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_016);
          break;
        }

        if (AreResultsAtByteLevel (CommonParameters, ChannelIndex)) {
          //
          // Copy Current Next state into second nibble
          //
          CommonParameters->CurrentState[ChannelIndex][StrobeLogical + CommonParameters->NextNibbleOffset] = NextState;
        }
        //
        // Update to next state if different than current state
        //
        if (CurrentState != NextState) {

          CommonParameters->CurrentState[ChannelIndex][StrobeLogical] = NextState;

          ClearStrobeLimit (CommonParameters, ChannelIndex, StrobeLogical);
        }

      } // StrobeLinear loop ...
    } // SubChannelIndex loop ...

    if (IsSingleToMultipleDelay (CommonParameters)) {
      for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
        for (StrobeLinear = 1; StrobeLinear < CommonParameters->MaxStrobeAvailable; StrobeLinear++) {
          //
          // Copy State from StrobeLinear 0 to Current StrobeLinear
          //
          StrobeLogicalForStrobeLinear0 = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, 0);
          StrobeLogical= GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);

          if (CommonParameters->CurrentState[ChannelIndex][StrobeLogical] !=
              CommonParameters->CurrentState[ChannelIndex][StrobeLogicalForStrobeLinear0]) {

            ClearStrobeLimit (CommonParameters, ChannelIndex, StrobeLogical);

          }

          CommonParameters->CurrentState[ChannelIndex][StrobeLogical] = CommonParameters->CurrentState[ChannelIndex][StrobeLogicalForStrobeLinear0];

        } // StrobeLinear loop ...
      } // SubChannelIndex loop ...
    } // IsSingleToMultipleDelay statement
  } // ChannelIndex loop

  if ((CommonParameters->FlagsFsm & ContinousMode)) {
    //
    // Loop in the ST_xx_END state until all strobes reach the same state
    //
    if (AreAllChannelsAtState (CommonParameters, ST_XX_END)) {
      PropagateStateAcrossChannelsStrobes (CommonParameters, ST_END);
    }
  }


  if (!IsDelayZero (CommonParameters, CommonParameters->DelayOffset)) {
    //
    // Apply only when Delay is different than zero in any channel
    //
    VerifyAndSetOffset (CommonParameters, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, CommonParameters->DelayOffset);
  }


  return MRC_STATUS_SUCCESS;
} // GetNextState

/*

  Based on the current result condition this function updates (if required),
  first condition, previous condition and dither value counter

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       Function was executed without errors

*/
MRC_STATUS
ResultsConditionByState (
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
  RESULTS_1D_DEFINITION LookUpCondition;
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
            // meaningful result condition
            //
            continue;
          }

          StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);

          for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {

            ResultConditionPtr = GetResultConditionPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, BitIndex);

            if (CommonParameters->CurrentState[ChannelIndex][StrobeLogical] == ST_AC_FIRST_SAMPLE) {

              ResultConditionPtr->First     = ResultConditionPtr->Current;
              ResultConditionPtr->Previous  = ResultConditionPtr->Current;

            } else if (IsSamplingState (CommonParameters, CommonParameters->CurrentState[ChannelIndex][StrobeLogical])) {

              if (IsFailingState (CommonParameters, CommonParameters->CurrentState[ChannelIndex][StrobeLogical])) {

                LookUpCondition = GetFailingCondition (CommonParameters, ResultConditionPtr);

              } else {

                LookUpCondition = GetPassingCondition (CommonParameters, ResultConditionPtr);

              }

              if (ResultConditionPtr->Current == LookUpCondition) {

                if (ResultConditionPtr->Previous  == ResultConditionPtr->Current) {

                  if (ResultConditionPtr->DitherCounter < (MAX_DITHER - 1)){

                    ResultConditionPtr->DitherCounter++;

                  } else {

                    RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_017);

                  }

                } else {

                  ResultConditionPtr->DitherCounter = 1;

                }

              } else {

                ResultConditionPtr->DitherCounter = 0;

              }

              ResultConditionPtr->Previous  = ResultConditionPtr->Current;

            } // IsSamplingState

          } // BitIndex loop ...

        } // StrobeLinear loop ...

      } // SubChannelIndex loop ...

    } // KnobIndex loop ...

  } // ChannelIndex loop ...

  return MRC_STATUS_SUCCESS;
}

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
)
{
  UINT8   ChannelIndex;
  UINT8   SubChannelIndex;
  UINT8   MaxChDdr;
  UINT8   StrobeLogical;
  UINT8   StrobeLogicalStrobe0;
  UINT8   StrobeLinear;
  UINT8   KnobIndex;


  if (IsSingleToMultipleDelay (CommonParameters) && ((CommonParameters->FlagsFsm & TrackingSweep) != TrackingSweep)) {

    MaxChDdr = CommonParameters->MaxChDdr;

    for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {

      if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
        continue;
      }

      for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {
        for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
          for (StrobeLinear = 1; StrobeLinear < CommonParameters->MaxStrobeAvailable; StrobeLinear++) {

            if (SkipExecutionOnStrobe (CommonParameters, ChannelIndex, StrobeLinear)) {
              continue;
            }

            StrobeLogicalStrobe0 = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, 0);
            StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);
            //
            // Propagate the results of current strobe to logical strobe 0
            //
            PropagateErrorsByState (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, StrobeLogicalStrobe0);

            if (AreResultsAtByteLevel (CommonParameters, ChannelIndex)) {
              //
              // Propagate the results of current strobe to logical strobe 0 + NextNibble
              //
              PropagateErrorsByState (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, StrobeLogicalStrobe0 + CommonParameters->NextNibbleOffset);
              //
              // Propagate the results of current strobe + NexNibble to logical strobe 0
              //
              PropagateErrorsByState (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical + CommonParameters->NextNibbleOffset, StrobeLogicalStrobe0);
              //
              // Propagate the results of current strobe + NexNibble to logical strobe 0 + NextNibble
              //
              PropagateErrorsByState (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical + CommonParameters->NextNibbleOffset, StrobeLogicalStrobe0 + CommonParameters->NextNibbleOffset);
            }

          } // StrobeLinear loop ...

        } // SubChannelIndex loop ...

      } // KnobIndex loop ...

    } // ChannelIndex loop ...

  } // IsSingleToMultipleDelay statement

  return MRC_STATUS_SUCCESS;
}

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
)
{
   if (State >= ST_AC_FIRST_SAMPLE &&
       State < ST_END) {
     return TRUE;
   }

   return FALSE;
}

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
)
{
  if (State >= ST_AC_FIRST_SAMPLE) {
    return TRUE;
  }
  return FALSE;
}

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
)
{
  UINT8   ChannelIndex;
  UINT8   SubChannelIndex;
  UINT8   StrobeLogical;
  UINT8   StrobeLinear;
  BOOLEAN AllActive = TRUE;
  UINT8   MaxChDdr;

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
        // All states are handled at a strobe level
        //

        StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);

        if (!IsActiveState (CommonParameters, CommonParameters->CurrentState[ChannelIndex][StrobeLogical])) {
          AllActive = FALSE;
        }

        break;

      } // StrobeLinear loop ...
    } // SubChannelIndex loop ...

    if (!AllActive) {
      break;
    }
  }

  return AllActive;
}

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
)
{
   if (State >= ST_LE_AC_INC_SE_PASS &&
       State <= ST_LE_AC_DEC_SE_FAIL) {
     return TRUE;
   }

   return FALSE;
}

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
)
{
  if (State >= ST_LE_AC_INC_SE_PASS &&
      State <= ST_LE_AC_RSO_AC_DEC_SE_PASS) {
    return TRUE;
  }

  return FALSE;

}

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
)
{
   if (State >= ST_RE_AC_DEC_SE_PASS &&
       State <= ST_RE_AC_INC_SE_FAIL_TS) {
     return TRUE;
   }

   return FALSE;
}

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
)
{
  if (State >= ST_RE_AC_DEC_SE_PASS &&
      State <= ST_RE_AC_RSO_AC_INC_SE_PASS) {
    return TRUE;
  }

  return FALSE;

}

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
)
{
  BOOLEAN FailingState;

  FailingState = FALSE;

  if (State == ST_LE_FN_AC_DEC_SE_FAIL ||
      State == ST_RE_FN_AC_INC_SE_FAIL ||
      State == ST_LE_AC_DEC_SE_FAIL ||
      State == ST_RE_AC_INC_SE_FAIL ||
      State == ST_RE_AC_INC_SE_FAIL_TS) {
    return TRUE;
  }

  return FailingState;
}

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
)
{
  BOOLEAN NormalPassState;

  NormalPassState = FALSE;

  if ((IsLeftSearchPassState (CommonParameters, State) ||
      IsRightSearchPassState (CommonParameters, State))) {
    NormalPassState = TRUE;
  }

  return NormalPassState;
}

