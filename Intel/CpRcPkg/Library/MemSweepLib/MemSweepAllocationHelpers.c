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

  Return the number of knobs that will be use to reserve the size of some arrays.
  This number may be optimize based on the context information (Group, Level,...)

  @param[in,out]  CommonParamaters  Pointer to the context information

  @return   The number of knobs use to reserve the size of the arrays

*/
UINT8
GetFinalKnobsForNemUsage (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
)
{
  UINT8   KnobsPerGroup;
  UINT8   MinimizedKnobs;
  UINT8   FinalKnobs;

  KnobsPerGroup   = GetNumberOfKnobsPerGroup (CommonParameters);
  MinimizedKnobs  = GetMinimizedKnobs (KnobsPerGroup);

  if (MinimizeNemUsage (CommonParameters->Level, CommonParameters->Group)) {
    FinalKnobs = MinimizedKnobs;
  } else {
    FinalKnobs = KnobsPerGroup;
  }

  return FinalKnobs;
}

/*

  Return the number of knobs required, considering that each strobe can
  be used to store the value of 1 knob

  @param[in]  KnobsPerGroup  Knobs that current context requires

  @return   The number of knobs required based on the number of strobes

*/
UINT8
GetMinimizedKnobs (
  IN        UINT8         KnobsPerGroup
)
{
  return (KnobsPerGroup / MAX_STROBE) + 1;
}

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
)
{
  BOOLEAN   MinimizeUsage;

  MinimizeUsage = FALSE;

  switch (Group) {
  case MrcGtDelim:
    MinimizeUsage = TRUE;
    break;
  default:
    MinimizeUsage = FALSE;
  }

  return MinimizeUsage;
}

/*

  Allocate resources to store the results generated after running the 1D Sweep
  WARNING: This function MUST be executed only after context was already created (CreateContextParameters).

  @param[in]      CommonParamaters  Pointer to the context information
  @param[in,out]  Results           Pointer that contains the final results

  @retval         MRC_STATUS_SUCCESS       Allocated resources for final results

*/
MRC_STATUS
AllocatePoolForResults1D (
  IN        COMMON_1D_PARAMETERS  *CommonParameters,
  IN  OUT   RESULTS_1D      *Results
)
{
  UINT8   FinalKnobs;

  FinalKnobs = GetFinalKnobsForNemUsage (CommonParameters);

  Results->ResultsPerStrobe = RcAllocatePool ( FinalKnobs * sizeof (*Results->ResultsPerStrobe) );
  SetMem (Results->ResultsPerStrobe, FinalKnobs * sizeof (*Results->ResultsPerStrobe), 0xFF);

  Results->ResultsPerBit    = NULL;

  if (CommonParameters->FlagsResults & ResultsPerBit) {
    Results->ResultsPerBit  = RcAllocatePool ( FinalKnobs * sizeof (*Results->ResultsPerBit) );
    SetMem (Results->ResultsPerBit, FinalKnobs * sizeof (*Results->ResultsPerBit), 0xFF);
  }

  return MRC_STATUS_SUCCESS;

}

/*

  Allocate resources to store the results generated after running the 1D Sweep
  WARNING: This function MUST be executed only after context was already created (CreateContextParameters).

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in,out]  Results           Pointer that contains the final results

  @retval         MRC_STATUS_SUCCESS       Allocated resources for final results

*/
MRC_STATUS
AllocatePoolForResultsAllRanks1D (
  IN        COMMON_1D_PARAMETERS  *CommonParameters,
  IN  OUT   RESULTS_1D      Results[MAX_RANK_CH]
)
{
  UINT8 RankIndex;

  for (RankIndex = 0; RankIndex < MAX_RANK_CH; RankIndex++) {
    AllocatePoolForResults1D (CommonParameters, &Results[RankIndex]);
  }

  return MRC_STATUS_SUCCESS;

}

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
)
{
  if (Results != NULL) {
    if (Results->ResultsPerBit != NULL) {
      RcFreePool (Results->ResultsPerBit);
    }

    RcFreePool (Results->ResultsPerStrobe);
  }


  return MRC_STATUS_SUCCESS;

}

/*

  Releases resources used to run 1D Sweep
  WARNING:
  This function MUST be executed only after context was already created (CreateContextParameters).
  This function MUST be executed only after resources are allocated (AllocatePoolForResults1D || AllocatePoolForResults1DAllRanks).


  @param[in]      CommonParamaters  Pointer to the context information
  @param[in,out]  Results           Pointer that contains the final results

  @retval         MRC_STATUS_SUCCESS       Resources released successfully

*/
MRC_STATUS
EFIAPI
ReleaseResources1D (
  IN        COMMON_1D_PARAMETERS  *CommonParameters,
  IN  OUT   RESULTS_1D            *Results
)
{

  FreePoolForResults1D (CommonParameters, Results);

  if (CommonParameters->OriginalDelays != NULL) {
    RcFreePool (CommonParameters->OriginalDelays);
    RcFreePool (CommonParameters->CurrentDelays);
    RcFreePool (CommonParameters->DelayOffset);
    RcFreePool (CommonParameters->MinMaxDeltaBitValue);
    RcFreePool (CommonParameters->ResultCondition);

    CommonParameters->OriginalDelays  = NULL;
    CommonParameters->CurrentDelays   = NULL;
    CommonParameters->DelayOffset     = NULL;
    CommonParameters->MinMaxDeltaBitValue     = NULL;
    CommonParameters->ResultCondition = NULL;
  }

  return MRC_STATUS_SUCCESS;
}

/*

  Releases resources used to run 1D Sweep
  WARNING:
  This function MUST be executed only after context was already created (CreateContextParameters).
  This function MUST be executed only after resources are allocated (AllocatePoolForResults1D || AllocatePoolForResults1DAllRanks).


  @param[in]      CommonParamaters  Pointer to the context information
  @param[in,out]  Results           Pointer that contains the final results

  @retval         MRC_STATUS_SUCCESS       Resources released successfully

*/
MRC_STATUS
EFIAPI
ReleaseResourcesAllRanks1D (
  IN        COMMON_1D_PARAMETERS  *CommonParameters,
  IN  OUT   RESULTS_1D            Results[MAX_RANK_CH]
  )
{

  UINT8 RankIndex;
  if (Results == NULL) {
    for (RankIndex = 0; RankIndex < MAX_RANK_CH; RankIndex++) {
      ReleaseResources1D (CommonParameters, NULL);
    }
  } else {
    for (RankIndex = 0; RankIndex < MAX_RANK_CH; RankIndex++) {
      ReleaseResources1D (CommonParameters, &Results[RankIndex]);
    }
  }

  return MRC_STATUS_SUCCESS;
}


/*

  Allocate resources for current condition

  @param[in,out]  CommonParamaters  Pointer to the context information

  @return   Pointer to memory reserved
*/
VOID *
AllocatePoolForResultCondition (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
)
{
  UINT8   FinalKnobs;

  FinalKnobs = GetFinalKnobsForNemUsage (CommonParameters);

  return RcAllocatePool (FinalKnobs * sizeof (*CommonParameters->ResultCondition));
}

/*

  Allocate resources to store the Delay values

  @param[in,out]  CommonParamaters  Pointer to the context information

  @return   Pointer to memory reserved
*/
VOID *
AllocatePoolForDelays (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
)
{
  UINT8   FinalKnobs;

  FinalKnobs = GetFinalKnobsForNemUsage (CommonParameters);

  return RcAllocatePool (FinalKnobs * sizeof (*CommonParameters->OriginalDelays));
}

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
)
{

  UINT8   ChannelIndex;
  UINT8   SubChannelIndex;
  UINT8   LogicalRank;
  UINT8   StrobeLinear;
  UINT8   StrobeLogical;
  UINT8   BitIndex;
  UINT8   MaxCh;
  UINT8   BitIndexWithinChannel;

  COMMON_1D_PARAMETERS  *CommonParametersV;

  CommonParametersV = &CommonParameters2D->CommonParametersV;
  MaxCh = GetMaxChDdr ();

  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {

    LogicalRank = GetLogicalRank (
                    CommonParametersV->Host,
                    CommonParametersV->Socket,
                    ChannelIndex,
                    CommonParametersV->Dimm[ChannelIndex],
                    CommonParametersV->Rank[ChannelIndex]
                    );
    for (SubChannelIndex = CommonParametersV->MinSubChannelIndex; SubChannelIndex < CommonParametersV->MaxSubChannelIndex; SubChannelIndex++) {
      for (StrobeLinear = 0; StrobeLinear < CommonParametersV->MaxStrobeAvailable; StrobeLinear++) {

        StrobeLogical = GetStrobeLogical (CommonParametersV, ChannelIndex, SubChannelIndex, StrobeLinear);

        if (StrobeResultsDestination != NULL && StrobeResultsSource != NULL) {
          SetStrobeLegacyResult (
            CommonParametersV,
            VerticalSampleIndex,
            0,
            ChannelIndex,
            LogicalRank,
            SubChannelIndex,
            StrobeLogical,
            FALSE,
            StrobeResultsSource->strobe[ChannelIndex][LogicalRank][StrobeLogical].n,
            StrobeResultsSource->strobe[ChannelIndex][LogicalRank][StrobeLogical].p,
            StrobeResultsDestination,
            NULL
            );
        }


        if (BitResultsDestination != NULL && BitResultsSource != NULL) {
          for (BitIndex = 0; BitIndex < BITS_PER_NIBBLE; BitIndex++) {

            BitIndexWithinChannel = GetBitIndexWithinChannel (
                                      CommonParametersV,
                                      ChannelIndex,
                                      SubChannelIndex,
                                      StrobeLogical,
                                      BitIndex
                                      );

            SetBitLegacyResult (
              CommonParametersV,
              VerticalSampleIndex,
              0,
              ChannelIndex,
              LogicalRank,
              SubChannelIndex,
              StrobeLogical,
              BitIndex,
              FALSE,
              BitResultsSource->bits[ChannelIndex][LogicalRank][BitIndexWithinChannel].n,
              BitResultsSource->bits[ChannelIndex][LogicalRank][BitIndexWithinChannel].p,
              BitResultsDestination,
              NULL
              );

          } // BitIndex loop
        }
      } // StrobeLinear loop ...
    } // SubChannelIndex loop ...

    if (RankResultsDestination != NULL && RankResultsSource != NULL) {
      RankResultsDestination[VerticalSampleIndex].rank[ChannelIndex][LogicalRank].n = RankResultsSource->rank[ChannelIndex][LogicalRank].n;
      RankResultsDestination[VerticalSampleIndex].rank[ChannelIndex][LogicalRank].p = RankResultsSource->rank[ChannelIndex][LogicalRank].p;
    }

  } // ChannelIndex loop ...

}
