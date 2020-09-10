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
)
{

  UINT8   ChannelIndex;
  UINT8   SubChannelIndex;
  UINT8   MaxChDdr;
  UINT8   StrobeLogical;
  UINT8   StrobeLinear;
  UINT8   KnobIndex;
  INT16   *OriginalDelayPtr;
  INT16   *CurrentDelayPtr;
  INT16   *DelayOffsetPtr;
  INT16   *MinMaxDeltaBitValuePtr;
  INT16   MinLimit;
  INT16   MaxLimit;
  INT16   MinLimitDelay;
  INT16   MaxLimitDelay;
  INT16   MinLimitByOriginalDelay;
  INT16   MaxLimitByOriginalDelay;
  UINT16  UsDelay;
  INT16   NewDelayOffset;
  INT16   FirstOffset;
  BOOLEAN SameOffsetAll;


  MaxChDdr = CommonParameters->MaxChDdr;
  FirstOffset = 0;

  if (CheckCMDGroup (CommonParameters->Group) || CheckCTLGroup (CommonParameters->Group)) {
    GetCmdGroupLimits (
      CommonParameters->Socket,
      ALL_CH,
      MAX_DIMM,
      CommonParameters->Level,
      CommonParameters->Group,
      &MinLimitDelay,
      &MaxLimitDelay
      );
  } else if (CheckDimmParamGroup (CommonParameters->Group)) {
    GetDimmParamGroupLimits (
      CommonParameters->Level,
      CommonParameters->Group,
      &MinLimitDelay,
      &MaxLimitDelay,
      &UsDelay
      );
  } else {

    GetDataGroupLimits (
      CommonParameters->Host,
      CommonParameters->Level,
      CommonParameters->Group,
      &MinLimitDelay,
      &MaxLimitDelay,
      &UsDelay
      );
  }


  if (!AreAllAtActiveState (CommonParameters)) {
    //
    // Get Current delay values when FSM is not in an active state
    // This is done in order to update the current delays only when starting sweep
    // After the first sample (or in active state) the values are getting updated
    // accordingly without the need to read the register again ('Tag::GetSetOptimize::Active')
    //
    GetSetAcrossChannelsKnobs (CommonParameters, GSM_READ_ONLY | CommonParameters->ReadMode, FALSE, FALSE, CommonParameters->CurrentDelays, NULL, NULL);
  }

  SameOffsetAll = TRUE;

  if (CommonParameters->FlagsOffsets & PerStrobeAccess) {
    SameOffsetAll = FALSE;
  }

  if (IsSingleToMultipleDelay (CommonParameters)) {
    //
    // Tag::IsSingleToMultipleDelay::SameOffsetAll
    // As we are handling a single strobe, it is not desired to use later the ALL_STROBES
    // as some offsets related to strobes 1->x may be modified which is not the expectation
    //
    SameOffsetAll = FALSE;
  }

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
          // Only at this point it is created a flag of which strobes have reached the limit, so
          // every single valid strobe needs to be examined to make sure the FSM detects
          // later if the limit has been reached or not
          //

          StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);

          CurrentDelayPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, CommonParameters->CurrentDelays);
          DelayOffsetPtr  = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, DelayInput);
          MinMaxDeltaBitValuePtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, CommonParameters->MinMaxDeltaBitValue);

          if (CommonParameters->CurrentState[ChannelIndex][StrobeLogical] == ST_AC_INIT &&
              (CommonParameters->FlagsOffsets & UseOffsetAsAbsolute)) {
            //
            // Verify the initial absolute values are within the valid limits
            //
            if (*DelayOffsetPtr < MinLimitDelay) {
              *DelayOffsetPtr = MinLimitDelay;
            }
            if (*DelayOffsetPtr > MaxLimitDelay) {
              *DelayOffsetPtr = MaxLimitDelay;
            }

          } else if (IsStrobeAtLimit (CommonParameters, ChannelIndex, StrobeLogical)) {

            *DelayOffsetPtr = 0;

          } else {

            if (CommonParameters->FlagsOffsets & LimitSearch) {

              if ((CommonParameters->FlagsOffsets & UseOffsetAsAbsolute) == 0) {
                //
                // IF FLAGS_CATEGORY_OFFSETS.UseOffsetAsAbsolute is not set
                // The range will be [Original - LimitOffset, Original + LimitOffset]
                //
                OriginalDelayPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, CommonParameters->OriginalDelays);
                MaxLimitByOriginalDelay = *OriginalDelayPtr + CommonParameters->LimitOffset;
                MinLimitByOriginalDelay = *OriginalDelayPtr - CommonParameters->LimitOffset;

              } else {
                //
                // IF FLAGS_CATEGORY_OFFSETS.UseOffsetAsAbsolute is set
                // The range will be [OffsetLeft - LimitOffset, OffsetRight + LimitOffset]
                //
                MinLimitByOriginalDelay = CommonParameters->OffsetLeft  - CommonParameters->LimitOffset;
                MaxLimitByOriginalDelay = CommonParameters->OffsetRight + CommonParameters->LimitOffset;

              }
              MaxLimit = MIN (MaxLimitByOriginalDelay, MaxLimitDelay - *MinMaxDeltaBitValuePtr);
              MinLimit = MAX (MinLimitByOriginalDelay, MinLimitDelay);

            } else {

              MaxLimit = MaxLimitDelay - *MinMaxDeltaBitValuePtr;
              MinLimit = MinLimitDelay;

            }

            if (((CommonParameters->FlagsFsm & AvoidBoundaryCross) == AvoidBoundaryCross) &&
                ((CommonParameters->FlagsFsm & TrackingSweep) != TrackingSweep) &&
                (CommonParameters->CurrentState[ChannelIndex][StrobeLogical] != ST_AC_INIT)) {
              //
              // Apply boundary check only if TrackingSweep mode is disabled and if
              // state is different than INIT
              // Left side should only sweep from [MinLimitDelay, OriginalValue]
              // Right side should only sweep from [OriginalValue, MaxLimitDelay]
              //

              OriginalDelayPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, CommonParameters->OriginalDelays);

              if (CommonParameters->CurrentEdge == LEFT_EDGE) {

                MaxLimit = *OriginalDelayPtr;

              } else {

                MinLimit = *OriginalDelayPtr;

              }

            }

            NewDelayOffset =  *CurrentDelayPtr + *DelayOffsetPtr;

            if (NewDelayOffset < MinLimit || NewDelayOffset > MaxLimit) {
              *DelayOffsetPtr = 0;
              SetStrobeLimit (CommonParameters, ChannelIndex, StrobeLogical);
            }

            if (IsActiveState (CommonParameters, CommonParameters->CurrentState[ChannelIndex][StrobeLogical])) {
              //
              // Tag::GetSetOptimize::Active Update the current delay, only if we are in active state
              // Doing the update here will avoid register read later as the CurrentDelayPtr will have the updated value
              //
              *CurrentDelayPtr = *CurrentDelayPtr + *DelayOffsetPtr;
            }
          }

          if (StrobeLinear == 0 && SubChannelIndex == CommonParameters->MinSubChannelIndex) {
            FirstOffset = *DelayOffsetPtr;
          } else if (FirstOffset != *DelayOffsetPtr) {
            SameOffsetAll = FALSE;
          }

        } // StrobeLinear loop ...
      } // SubChannelIndex loop ...

    } // ChannelIndex loop

  } // KnobIndex loop ...

  GetSetAcrossChannelsKnobs (CommonParameters, Mode | CommonParameters->ReadMode, FALSE, SameOffsetAll, DelayInput, NULL, NULL);

  return MRC_STATUS_SUCCESS;
} // VerifyAndSetOffset


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
)
{
  UINT8   ChannelIndex;
  UINT8   ChannelIndexLocal;
  UINT8   SubChannelIndex;
  UINT8   MaxChDdr;
  UINT8   StrobeLogical;
  UINT8   StrobeLinear;
  UINT8   StrobeFinal;
  UINT8   KnobIndex;
  INT16   LocalMinValue;
  INT16   LocalMaxValue;
  INT16   *DelayPtr;
  INT16   *DelayCopyPtr;
  INT16   *CurrentDelayPtr;
  INT16   *CurrentDelayCopyPtr;
  INT16   *CurrentDelayPtrFromStrobeLinear0;
  MRC_GT  KnobGroup;
  BOOLEAN UseMulticast;

  MaxChDdr = CommonParameters->MaxChDdr;


  for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {
      for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
        //
        // Get the channel mapping swizzle
        //
        GetChannelMapping (ChannelIndex, SubChannelIndex, &ChannelIndexLocal);

        for (StrobeLinear = 0; StrobeLinear < CommonParameters->MaxStrobeAvailable; StrobeLinear++) {

          if (SkipExecutionOnStrobe (CommonParameters, ChannelIndex, StrobeLinear)) {
            continue;
          }

          if (IsSingleToMultipleDelay (CommonParameters) && StrobeLinear >= 1 && (Mode & GSM_READ_ONLY) == 0) {
            //
            // Tag::IsSingleToMultipleDelay::ReadMode
            // For reading there is no need to skip StrobeLinear >= 1
            //
            continue;
          }

          StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);

          LocalMaxValue = 0;
          LocalMinValue = MAX_INT16;
          if ((KnobIndex == EvenChunkSide && (CommonParameters->FlagsResults & ResultsEvenOdd)) ||
              ((CommonParameters->FlagsResults & ResultsEvenOdd) == 0)) {
            //
            // Program the register any time when FLAGS_CATEGORY_RESULTS.ResultsEvenOdd is cleared
            // Although only execute for KnobIndex '0' when is set, as this is a single knob delay
            //
            switch (CommonParameters->Group) {
            case CmdAll:
            case CtlAll:

              DelayPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, DelayInput);

              if (CommonParameters->FlagsResults & CmdCtlAllKnobs) {

                GetSetCmdCtlIoKnob (
                  CommonParameters->Host,
                  CommonParameters->Socket,
                  ChannelIndexLocal,
                  KnobIndex,
                  CommonParameters->Group,
                  Mode,
                  DelayPtr
                );

              } else if (CommonParameters->Group == CmdAll) {
                GetSetCmdGroupDelayCore (
                  CommonParameters->Host,
                  CommonParameters->Socket,
                  ChannelIndexLocal,
                  CommonParameters->Dimm[ChannelIndex],
                  CommonParameters->Level,
                  CommonParameters->Group,
                  Mode,
                  DelayPtr,
                  &LocalMinValue,
                  &LocalMaxValue
                );

                *DelayPtr = LocalMinValue;
              } else {
                GetSetCtlGroupDelayCore (
                  CommonParameters->Host,
                  CommonParameters->Socket,
                  ChannelIndexLocal,
                  CommonParameters->Dimm[ChannelIndex],
                  CommonParameters->Level,
                  CommonParameters->Group,
                  Mode,
                  DelayPtr,
                  &LocalMinValue,
                  &LocalMaxValue
                );

                *DelayPtr = LocalMinValue;
              }
              break;
            case CmdVref:

              DelayPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, DelayInput);

              GetSetCmdVrefCore (
                CommonParameters->Host,
                CommonParameters->Socket,
                ChannelIndexLocal,
                CommonParameters->Dimm[ChannelIndex],
                CommonParameters->Level,
                Mode,
                DelayPtr
              );

              break;

#ifdef DDR5_SUPPORT
            case RcdDcaVref:

              DelayPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, DelayInput);

              GetSetRcdDcaVref (
                CommonParameters->Host,
                CommonParameters->Socket,
                ChannelIndex,
                SubChannelIndex,
                CommonParameters->Dimm[ChannelIndex],
                CommonParameters->Signal,
                Mode,
                DelayPtr
              );

              break;
            case DcaDelay:

              DelayPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, DelayInput);

              GetSetSignalDdr5 (
                CommonParameters->Host,
                CommonParameters->Socket,
                ChannelIndex,     // Swizzling is done in the lower layers
                DdrLevel,
                GetRcdCaSignal (SubChannelIndex, CommonParameters->Signal),
                Mode,
                DelayPtr
              );

              break;

            case DcaDfeVref:
              DelayPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, DelayInput);

              GetSetDcaDfeVref (
                CommonParameters->Host,
                CommonParameters->Socket,
                ChannelIndexLocal,
                SubChannelIndex,
                CommonParameters->Dimm[ChannelIndex],
                CommonParameters->Signal,
                Mode,
                DelayPtr
              );

              break;

            case DcsDelay:

              DelayPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, DelayInput);

              GetSetSignalDdr5 (
                CommonParameters->Host,
                CommonParameters->Socket,
                ChannelIndex,
                DdrLevel,
                GetRcdCsSignal (CommonParameters->Dimm[ChannelIndex], CommonParameters->Rank[ChannelIndex], SubChannelIndex),
                Mode,
                DelayPtr
              );

              break;

            case QcaDelay:

              DelayPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, DelayInput);

              GetSetQcaDelay (
                CommonParameters->Host,
                CommonParameters->Socket,
                ChannelIndex,
                SubChannelIndex,
                CommonParameters->Dimm[ChannelIndex],
                CommonParameters->Rank[ChannelIndex],
                GetQcaSignal (SubChannelIndex, CommonParameters->Signal),
                Mode,
                DelayPtr
              );

              break;

            case QcsDelay:

              DelayPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, DelayInput);

              GetSetQcsAllCopiesDelay (
                CommonParameters->Host,
                CommonParameters->Socket,
                ChannelIndex,
                SubChannelIndex,
                CommonParameters->Dimm[ChannelIndex],
                CommonParameters->Rank[ChannelIndex],
                Mode,
                DelayPtr,
                NULL, NULL
              );

              break;

            case DramCaVref:
            case DramCsVref:

              DelayPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, DelayInput);

              
              if (SameOffsetAll) {

                StrobeFinal = ALL_STROBES;

              } else {

                StrobeFinal = StrobeLogical;

              }

              if ((SameOffsetAll && StrobeLinear == 0) || !SameOffsetAll) {

                GetSetDramCaCsVref (
                  CommonParameters->Host,
                  CommonParameters->Socket,
                  ChannelIndex,
                  CommonParameters->Dimm[ChannelIndex],
                  CommonParameters->Rank[ChannelIndex],
                  StrobeFinal,
                  CommonParameters->Group,
                  Mode,
                  DelayPtr
                );
              }

            break;

          case BcomDelay:

            DelayPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, DelayInput);

            GetSetBcomDelay (
              CommonParameters->Host,
              CommonParameters->Socket,
              ChannelIndex,
              SubChannelIndex,
              CommonParameters->Dimm[ChannelIndex],
              CommonParameters->Rank[ChannelIndex],
              Mode,
              DelayPtr
              );
            break;

          case BcomVref:

            DelayPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, DelayInput);
            GetSetBcomVref (
              CommonParameters->Socket,
              ChannelIndex,
              SubChannelIndex,
              CommonParameters->Dimm[ChannelIndex],
              CommonParameters->Rank[ChannelIndex],
              ALL_STROBES,
              Mode,
              DelayPtr
              );
            break;
  #endif

            default:
              if ((CommonParameters->Group == RxDqsDelay ||
                  CommonParameters->Group == RxDqsPDelay ||
                  CommonParameters->Group == RxDqsNDelay) && CommonParameters->Level == DdrLevel) {

                if (KnobIndex == EvenChunkSide) {

                  KnobGroup = RxDqsPDelay;

                } else {

                  KnobGroup = RxDqsNDelay;

                }

              } else if (CommonParameters->Group == RxSamplerEvenOdd ||
                         CommonParameters->Group == RxSamplerEven ||
                         CommonParameters->Group == RxSamplerOdd) {

                if (KnobIndex == EvenChunkSide) {

                  KnobGroup = RxSamplerEven;

                } else {

                  KnobGroup = RxSamplerOdd;

                }

              } else {

                KnobGroup = CommonParameters->Group;

              }

              DelayPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, DelayInput);

              if (SameOffsetAll) {

                StrobeFinal = ALL_STROBES;

              } else {

                StrobeFinal = StrobeLogical;

              }

              UseMulticast = UseDelayMulticastOffset (StrobeFinal, ALL_BITS, CommonParameters->Level, CommonParameters->Group, Mode);

              if (UseMulticast) {
                //
                // If the multicast value will be used we need the original delay group
                // RxDqsDelay, instead of RxDqsPDelay or RxDqsNDelay
                // RxSamplerEvenOdd, instead of RxSamplerEven or RxSamplerOdd.
                // Multicast register touches both the Even/Odd or P/N
                //
                KnobGroup = CommonParameters->Group;

              }

              if (!UseMulticast ||
                  (UseMulticast && (KnobIndex == EvenChunkSide))) {
                //
                // When Multicast is enabled, any KnobIndex greater than 0 can be skipped,
                // because Multicast register touches both the Even/Odd or P/N
                //
                if ((SameOffsetAll && StrobeLinear == 0) || !SameOffsetAll) {
                  GetSetDataGroup (
                    CommonParameters->Host,
                    CommonParameters->Socket,
                    ChannelIndexLocal,
                    NO_SUBCH,
                    CommonParameters->Dimm[ChannelIndex],
                    CommonParameters->Rank[ChannelIndex],
                    StrobeFinal,
                    ALL_BITS,
                    CommonParameters->Level,
                    KnobGroup,
                    Mode,
                    DelayPtr
                    );
                }
              }

              if (UpdateMinMax && MinValue != NULL && MaxValue != NULL) {
                UpdateMinMaxUint (
                  *DelayPtr,
                  &MinValue[ChannelIndex][StrobeLogical],
                  &MaxValue[ChannelIndex][StrobeLogical]
                  );
              }
              break;
            } // switch (CommonParameters->Group)

          } else {
            //
            // (KnobIndex > 0 && (CommonParameters->FlagsResults & ResultsEvenOdd))
            // Then this delay is a single knob, and there is no need to read the value from register, it is already in KnobIndex 0
            //
            DelayPtr              = GetDelayPtr (CommonParameters, 0, ChannelIndex, StrobeLogical, DelayInput);
            CurrentDelayCopyPtr   = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, DelayInput);

            *CurrentDelayCopyPtr = *DelayPtr;
          }

        } // StrobeLinear loop ...

        //
        // ALL_STROBES has different meanings across platform, for example:
        // For ICX means all strobes within the Channel
        // For BRS means all strobes within the Sub-Channel
        // For SPR means all strobes within the Channel (both Sub-Channels)
        //
        if (CommonParameters->DdrType == Ddr5Type && SameOffsetAll) {
          if (IsAllStrobesAcrossSubCh (CommonParameters->Host, CommonParameters->Socket, CommonParameters->Level, CommonParameters->Group)) {
            //
            // ALL_STROBES will target both sub-channels,
            // so there is no need to do the second SubChannelIndexloop
            //
            break;
          }
        }

      } // SubChannelIndex loop ...

      if (IsSingleToMultipleDelay (CommonParameters)) {
        for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
          for (StrobeLinear = 1; StrobeLinear < CommonParameters->MaxStrobeAvailable; StrobeLinear++) {
            //
            // Tag::IsSingleToMultipleDelay::CopyDelay
            // Copy current delay from StrobeLinear 0 to all strobes
            //
            StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, 0);
            CurrentDelayPtrFromStrobeLinear0 = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, CommonParameters->CurrentDelays);

            StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);
            CurrentDelayPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, CommonParameters->CurrentDelays);

            *CurrentDelayPtr = *CurrentDelayPtrFromStrobeLinear0;
          }
        } // SubChannelIndex loop ...
      }
      if (AreResultsAtByteLevel (CommonParameters, ChannelIndex)) {
        for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
          for (StrobeLinear = 0; StrobeLinear < CommonParameters->MaxStrobeAvailable; StrobeLinear++) {

            if (SkipExecutionOnStrobe (CommonParameters, ChannelIndex, StrobeLinear)) {
              continue;
            }
            //
            // Copy DelayInput (As Input Parameters) into second nibble
            // Copy CurrentDelayInput into second nibble
            // Both of these actions will keep the second nibble in sync if this a delay is at a Byte Level
            //
            StrobeLogical   = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);
            DelayPtr        = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, DelayInput);
            CurrentDelayPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, CommonParameters->CurrentDelays);

            DelayCopyPtr         = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical + CommonParameters->NextNibbleOffset, DelayInput);
            CurrentDelayCopyPtr  = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical + CommonParameters->NextNibbleOffset, CommonParameters->CurrentDelays);

            *DelayCopyPtr         = *DelayPtr;
            *CurrentDelayCopyPtr  = *CurrentDelayPtr;
          }
        } // SubChannelIndex loop ...
      }
    } // KnobIndex loop ...
  } // ChannelIndex loop


  return MRC_STATUS_SUCCESS;
}

/*
  Get Min/Max bit delta value for each nibble

  @param[in,out]  CommonParameters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       New offset was set correctly

*/
MRC_STATUS
GetMinMaxDeltaValuePerNibble (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  UINT8   ChannelIndex;
  UINT8   ChannelIndexLocal;
  UINT8   SubChannelIndex;
  UINT8   MaxChDdr;
  UINT8   StrobeLogical;
  UINT8   BitIndex;
  UINT8   StrobeLinear;
  UINT8   KnobIndex;
  INT16   LocalMinValue;
  INT16   LocalMaxValue;
  INT16   *CurrentDelayPtr;
  INT16   *MinMaxDeltaBitValuePtr;
  MRC_GT  KnobGroup;
  INT16   Delay;

  MaxChDdr = CommonParameters->MaxChDdr;

  LocalMinValue = 0;
  LocalMaxValue = 0;

  for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {
      for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
        //
        // Get the channel mapping swizzle
        //
        GetChannelMapping (ChannelIndex, SubChannelIndex, &ChannelIndexLocal);

        for (StrobeLinear = 0; StrobeLinear < CommonParameters->MaxStrobeAvailable; StrobeLinear++) {

          if (SkipExecutionOnStrobe (CommonParameters, ChannelIndex, StrobeLinear)) {
            continue;
          }

          StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);

          switch (CommonParameters->Group) {
          case RxSamplerEvenOdd:
          case RxSamplerEven:
          case RxSamplerOdd:
          case TxDqDelay:
          case RxVref:
            if (CommonParameters->Group == RxSamplerEvenOdd ||
                CommonParameters->Group == RxSamplerEven ||
                CommonParameters->Group == RxSamplerOdd) {

              if (KnobIndex == EvenChunkSide) {

                KnobGroup = RxSamplerEven;

              } else {

                KnobGroup = RxSamplerOdd;

              }

            } else if (CommonParameters->Group == TxDqDelay) {

              KnobGroup = TxDqBitDelay;

            } else {

              KnobGroup = RxVref;

            }


            CurrentDelayPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, CommonParameters->CurrentDelays);

            for (BitIndex = 0; BitIndex < BITS_PER_NIBBLE; BitIndex++) {

              GetSetDataGroup (
                CommonParameters->Host,
                CommonParameters->Socket,
                ChannelIndexLocal,
                NO_SUBCH,
                CommonParameters->Dimm[ChannelIndex],
                CommonParameters->Rank[ChannelIndex],
                StrobeLogical,
                BitIndex,
                CommonParameters->Level,
                KnobGroup,
                GSM_READ_ONLY | CommonParameters->ReadMode,
                CurrentDelayPtr
                );

              if (BitIndex == 0) {

                LocalMinValue = *CurrentDelayPtr;
                LocalMaxValue = *CurrentDelayPtr;

              } else {

                if (LocalMinValue > *CurrentDelayPtr) {
                  LocalMinValue = *CurrentDelayPtr;
                }
                if (LocalMaxValue < *CurrentDelayPtr) {
                  LocalMaxValue = *CurrentDelayPtr;
                }

              }
            }

            break;
          case QcsDelay:
              GetSetQcsAllCopiesDelay (
              CommonParameters->Host,
              CommonParameters->Socket,
              ChannelIndex,
              SubChannelIndex,
              CommonParameters->Dimm[ChannelIndex],
              CommonParameters->Rank[ChannelIndex],
              GSM_READ_ONLY,
              &Delay,
              &LocalMinValue,
              &LocalMaxValue
              );

            break;

          default:

            LocalMinValue = 0;
            LocalMaxValue = 0;

            break;
          } // switch (CommonParameters->Group)

          MinMaxDeltaBitValuePtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, CommonParameters->MinMaxDeltaBitValue);

          *MinMaxDeltaBitValuePtr = LocalMaxValue - LocalMinValue;

        } // StrobeLinear loop ...
      } // SubChannelIndex loop ...
    } // KnobIndex loop ...
  } // ChannelIndex loop
  return MRC_STATUS_SUCCESS;
}
/*

  Return the number of knobs for current context information

  @param[in,out]  CommonParamaters  Pointer to the context information

  @return         Number of knobs used for current context information

*/
UINT8
GetNumberOfKnobsPerGroup (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
)
{
  PSYSHOST Host;
  UINT8 MaxIoGroups;

  Host = CommonParameters->Host;
  MaxIoGroups = 1;

  switch (CommonParameters->Group) {
  case CmdAll:
  case CtlAll:
    if (CommonParameters->FlagsResults & CmdCtlAllKnobs) {
      GetNumberOfKnobsPerCmdCtlGroups (CommonParameters->Host, CommonParameters->Group, &MaxIoGroups);
    } else {
      MaxIoGroups = 1;
    }
    break;
  case RxDqsDelay:
  case RxDqsPDelay:
  case RxDqsNDelay:
  case RxSamplerEvenOdd:
  case RxSamplerEven:
  case RxSamplerOdd:
    MaxIoGroups = 2;
    break;
  default:
    MaxIoGroups = 1;
    break;
  }

  //
  // Override for LrbufLevel, if needed
  //
  if (CommonParameters->Level == LrbufLevel) {
    switch (CommonParameters->Group) {
      case RxDqsDelay:
        MaxIoGroups = 1;
        break;
    }
  }

  if (MaxIoGroups == 2) {

    CommonParameters->FlagsResults &= ~ResultsEvenOdd;

  } else if (MaxIoGroups > 2 && (CommonParameters->FlagsResults & ResultsEvenOdd)) {
    RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_022);
    CommonParameters->FlagsResults &= ~ResultsEvenOdd;
  }

  if (CommonParameters->FlagsResults & ResultsEvenOdd) {
    MaxIoGroups = 2;
  }

  return MaxIoGroups;
}

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
  )
{
  UINT8 MaxStrobes;

  MaxStrobes = GetSubChMaxStrobeValid (CommonParameters->Host);

  return MaxStrobes;
}

/*

  Return the number of strobes to be printed

  @param[in,out]  CommonParamaters  Pointer to the context information

  @return         Number of strobes to be printed (usually Bytes within a Sub-Channel)

*/
UINT8
GetMaxStrobeToPrint (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  UINT8 MaxStrobes;

  MaxStrobes = GetSubChMaxStrobeValid (CommonParameters->Host) / 2;

  if (IsResultAtRankLevel (CommonParameters)) {
    //
    // Tag::StrobeLinearBreak::AtRankLevel
    // Only print information related with strobe 0
    //
    MaxStrobes = 1;
  }

  return MaxStrobes;
}

/*

  Returns the offset to calculate the second nibble
  within the byte with respect the first nibble

  @param[in,out]  CommonParamaters  Pointer to the context information

  @return         Returns offset to the second nibble

*/
UINT8
GetNextNibbleOffset (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  UINT8 MaxStrobes;

  MaxStrobes = GetSubChMaxStrobeValid (CommonParameters->Host);

  return MaxStrobes / 2;
}

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
)
{
  BOOLEAN IsPerRankDelayButStrobeResults;

  IsPerRankDelayButStrobeResults  = FALSE;

  switch (CommonParameters->Group) {
  case CmdAll:
  case CtlAll:
  case CmdVref:
  case QcaDelay:
  case QcsDelay:
  case BcomDelay:
    IsPerRankDelayButStrobeResults = TRUE;
    break;
  case TxVref:
  case DramCaVref:
  case DramCsVref:
    if (CommonParameters->Host->nvram.mem.socket[CommonParameters->Socket].TrainingStepDone.TxVref == 1 &&
       ((CommonParameters->Setup->mem.optionsExt & PDA_EN) != 0)) {
      IsPerRankDelayButStrobeResults = FALSE;
    } else {
      IsPerRankDelayButStrobeResults = TRUE;
    }
    break;
  case BcomVref:
    //
    // Current code use RCD BCOM Staic Value to control BcomVref. RCD BCOM Staic Value
    // is per-rank(DIMM) setting.
    //
    IsPerRankDelayButStrobeResults = TRUE;
  }

  if (IsPerRankDelayButStrobeResults && IsResultAtRankLevel (CommonParameters)) {
    RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_003);
  }

  return IsPerRankDelayButStrobeResults;
}

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
)
{
  BOOLEAN   SingleStrobe;

  SingleStrobe = FALSE;

  switch (CommonParameters->Group) {
  case MrcGtDelim:
  case DcaDelay:
  case RcdDcaVref:
  case DcaDfeVref:
  case DcsDelay:
    SingleStrobe = TRUE;
    break;
  default:
    SingleStrobe = FALSE;
  }

  return SingleStrobe;
}

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
)
{
  BOOLEAN IsGroupAtByte;
  UINT8   MaxStrobeSubCh;

  IsGroupAtByte = FALSE;

  MaxStrobeSubCh = GetMaxStrobeWithinSubCh (
                         CommonParameters->Host,
                         CommonParameters->Socket,
                         ChannelIndex,
                         CommonParameters->Dimm[ChannelIndex],
                         CommonParameters->Group,
                         CommonParameters->Level
                         );

  if (CommonParameters->DdrType == Ddr5Type) {
    if (MaxStrobeSubCh == (MAX_STROBE_DDR5 / 4)) {
      IsGroupAtByte = TRUE;
    }
  } else if (MaxStrobeSubCh == (MAX_STROBE_DDR4 / 2)) {
    IsGroupAtByte = TRUE;
  }

  if (IsResultAtRankLevel (CommonParameters)) {
    //
    // A delay already defined with results at IsResultAtRankLevel
    // cannot be at byte level, so it is always considered at nibble level
    //
    IsGroupAtByte = FALSE;
  }

  return IsGroupAtByte;
}

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
)
{

  if (IsResultAtRankLevel (CommonParameters) && StrobeLinear >= 1) {
    return TRUE;
  }

  if (AreResultsAtByteLevel (CommonParameters, ChannelIndex) && StrobeLinear >= (CommonParameters->MaxStrobeAvailable / 2)) {
    return TRUE;
  }

  if (StrobeLinear >= CommonParameters->MaxStrobeAvailable) {
    return TRUE;
  }

  if (!CommonParameters->IsEccEnabled && IsEccStrobe (CommonParameters->Host, StrobeLinear)) {
    return TRUE;
  }

  return FALSE;
}

/*

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

*/
UINT8
GetStrobeLogical (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   SubChannelIndex,
  IN         UINT8                   StrobeLinear
  )
{
  UINT8   StrobeLogical;

  StrobeLogical = StrobeLinear;

  if (CommonParameters->DdrType == Ddr5Type) {
    if (SubChannelIndex >= SUB_CH) {

      RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_018);
      return 0;

    }
    StrobeLogical = StrobeLinear  + (SubChannelIndex * (MAX_STROBE_DDR5 / 2));

  }

  if (StrobeLogical >= MAX_STROBE) {

    RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_031);
    StrobeLogical = 0;

  }

  return StrobeLogical;
}

/*

  This function restores the original value for all delays

  @param[in]      CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       If the delays got restored properly

*/
MRC_STATUS
RestoreOriginalDelays (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
)
{
  UINT8 KnobIndex;
  UINT8   ChannelIndex;
  UINT8   SubChannelIndex;
  UINT8   MaxChDdr;
  UINT8   StrobeLogical;
  UINT8   StrobeLinear;
  INT16   *OriginalDelayPtr;
  INT16   *CurrentDelayPtr;
  INT16   *DelayOffsetPtr;

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
        //
        // Tag::IsSingleToMultipleDelay::NoBreak
        // No need to add the IsSingleToMultipleDelay() && StrobeLinear >= 1 break here
        // GetSetAcrossChannelsKnobs already access a single register
        //
        StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);

        for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {

          OriginalDelayPtr = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, CommonParameters->OriginalDelays);
          CurrentDelayPtr  = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, CommonParameters->CurrentDelays);
          DelayOffsetPtr   = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, CommonParameters->DelayOffset);

          *DelayOffsetPtr = *OriginalDelayPtr - *CurrentDelayPtr;

        } // KnobIndex loop ...
      } // StrobeLinear loop ...
    } // SubChannelIndex loop ...
  } // ChannelIndex loop ...

  GetSetAcrossChannelsKnobs (
    CommonParameters,
    GSM_FLUSH_OFFSET | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE | CommonParameters->ReadMode,
    FALSE,
    FALSE,
    CommonParameters->DelayOffset,
    NULL,
    NULL
    );

  return MRC_STATUS_SUCCESS;
} // RestoreOriginalDelays
