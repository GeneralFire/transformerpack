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

#define PRINT_TAB_SIZE    4

/*

  Print N number of blank spaces

  @param[in]      DebugLevel        Debug level to be used to print blank spaces
  @param[in]      NumberOfSpaces    Number of blank spaces to print

*/
VOID
PrintLineBlankSpace (
  IN         UINT32                  DebugLevel,
  IN         UINT16                  NumberOfSpaces
)
{
  UINT16 Counter;

  for (Counter = 0; Counter < NumberOfSpaces; Counter++) {
    RcDebugPrint (DebugLevel, " ");
  }
}

/*

  Print strobe label

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      SubChannelIndex   Current sub-channel under test (0-based)
  @param[in]      StrobeLinear      Current linear strobe under test (0-based)
  @param[in]      FormatType        Format to be used

*/
VOID
PrintStrobeLabel (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   SubChannelIndex,
  IN         UINT8                   StrobeLinear,
  IN         FORMAT_TYPE             FormatType
  )
{
  //
  // Print all the label for available strobes
  //
  if (IsEccStrobe (CommonParameters->Host, StrobeLinear)) {
    RcDebugPrint (SDBG_SWEEP_L0, " e");
  } else {
    RcDebugPrint (SDBG_SWEEP_L0, " _");
  }

  RcDebugPrint (SDBG_SWEEP_L0, "S%02d", StrobeLinear);

}


/*

  Print each strobe in the header

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      SubChannelIndex   Current sub-channel under test (0-based)
  @param[in]      StrobeLinear      Current linear strobe under test (0-based)
  @param[in]      FormatType        Format to be used

*/
VOID
PrintEachStrobeOnHeader (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   SubChannelIndex,
  IN         UINT8                   StrobeLinear,
  IN         FORMAT_TYPE             FormatType
)
{
  UINT8   KnobIndex;
  UINT8   BitIndex;

  if (IsResultAtRankLevel (CommonParameters) && StrobeLinear >= 1) {
    //
    // Tag::StrobeLinearBreak::AtRankLevel
    // Only print information related with strobe 0
    //
    return;
  }

  if (FormatType == FormatResultsCondition) {
    //
    // Blank spaces for current values per knob (xxxx,xxxx,...)
    //
    PrintLineBlankSpace (SDBG_SWEEP_L1, 1);
    for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {
      PrintLineBlankSpace (SDBG_SWEEP_L1, 5);
    } // KnobIndex loop ...
    PrintLineBlankSpace (SDBG_SWEEP_L1, 1);

    //
    // Blank spaces for current dither counter per Bit/knob [b0k0:b0k1:...,b1k0:b1k1:...,]
    //
    PrintLineBlankSpace (SDBG_SWEEP_L3, 1);
    for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {
      for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {
        PrintLineBlankSpace (SDBG_SWEEP_L3, 3);
      } // KnobIndex loop ...
      PrintLineBlankSpace (SDBG_SWEEP_L3, 1);
    } // BitIndex loop ...
    //
    // Blank spaces for ']'
    //
    PrintLineBlankSpace (SDBG_SWEEP_L3, 1);
    //
    // Blank spaces for '{xx}'
    //
    PrintLineBlankSpace (SDBG_SWEEP_L2, 4);
  }

  PrintStrobeLabel (CommonParameters, SubChannelIndex, StrobeLinear, FormatType);

  if (FormatType == FormatResults1D) {
    //
    // Add blank spaces for strobe results Kzz(xxx x,yyyy)
    //
    //
    PrintLineBlankSpace (SDBG_SWEEP_L0, 6);

    if (CommonParameters->FlagsResults & ResultsPerBit) {
      //
      // Add blank spaces for [xxxx:yyyy,]
      //
      PrintLineBlankSpace (SDBG_SWEEP_L0, (10 * CommonParameters->BitsToTrack) + 2);
    }

  }

}
/*
  Print all strobes in the header

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      FormatType        Format to be used

*/
VOID
PrintHeaderByStrobes (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         FORMAT_TYPE             FormatType
)
{
  UINT8   StrobeLinear;
  UINT8   SubChannelIndex;

  if ((CommonParameters->DebugLevel & SDBG_SWEEP_MASK) == 0) {
    return;
  }

  PrintLineBlankSpace (SDBG_SWEEP_L0, PRINT_TAB_SIZE * CommonParameters->IndentationLevel);

  RcDebugPrintWithDeviceInternal (
    SDBG_SWEEP_L0,
    CommonParameters->Socket,
    NO_CH,
    NO_DIMM,
    NO_SUBCH,
    NO_RANK,
    NO_STROBE,
    NO_BIT,
    "              "
    );

  if (FormatType == FormatResultsCondition) {
    //
    // Blank spaces for Strobe Limit
    // -0x00000000-
    //
    PrintLineBlankSpace (SDBG_SWEEP_L2, 12);
  } else if (FormatType == FormatResults1D) {
    //
    // Blank spaces Kxx
    //
    PrintLineBlankSpace (SDBG_SWEEP_L0, 3);
  }

  for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
    for (StrobeLinear = 0; StrobeLinear < GetMaxStrobeToPrint (CommonParameters); StrobeLinear++) {

      PrintEachStrobeOnHeader (CommonParameters, SubChannelIndex, StrobeLinear, FormatType);
      PrintEachStrobeOnHeader (CommonParameters, SubChannelIndex, StrobeLinear + CommonParameters->NextNibbleOffset, FormatType);

    } // StrobeLinear loop ...
    //
    // Results are printed by subchannel,
    // while during sweep results are by channel
    //
    if (FormatType == FormatResults1D) {
      break;
    }

  } // SubChannelIndex loop ...

  RcDebugPrint (SDBG_SWEEP_L0, "\n");
}

/*

  Prints each strobe status.

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      SubChannelIndex   Current sub-channel under test (0-based)
  @param[in]      StrobeLinear      Current linear strobe under test (0-based)
  @param[in]      DelayInput        Pointer to delay values

*/
VOID
PrintEachStrobeCurrentStatus (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   SubChannelIndex,
  IN         UINT8                   StrobeLinear,
  IN         INT16                   (*DelayInput)[MAX_CH][MAX_STROBE]
)
{
  UINT8   KnobIndex;
  UINT8   BitIndex;
  UINT8   StrobeLogical;
  INT16   *CurrentDelayPtr;
  BOOLEAN AnyKnobPassed;
  BOOLEAN AllKnobPassed;
  RESULTS_1D_DEFINITION   PassCondition;
  RESULTS_1D_DEFINITION   FailCondition;
  RESULTS_1D_CONDITION    *ResultConditionPtr;


  if (IsResultAtRankLevel (CommonParameters) && StrobeLinear >= 1) {
    //
    // Tag::StrobeLinearBreak::AtRankLevel
    // Only print information related with strobe 0
    //
    return;
  }

  StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);

  //
  // Print current values per knob (xxxx,xxxx,...)
  //
  RcDebugPrint (SDBG_SWEEP_L1, "(");
  for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {

    CurrentDelayPtr   = GetDelayPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, DelayInput);
    if (IsSingleToMultipleDelay (CommonParameters) && StrobeLinear >= 1) {
      RcDebugPrint (SDBG_SWEEP_L1, "%4d-", *CurrentDelayPtr);
    } else if (AreResultsAtByteLevel (CommonParameters, ChannelIndex) && StrobeLinear >= (CommonParameters->MaxStrobeAvailable / 2)) {
      RcDebugPrint (SDBG_SWEEP_L1, "%4d.", *CurrentDelayPtr);
    } else {
      RcDebugPrint (SDBG_SWEEP_L1, "%4d,", *CurrentDelayPtr);
    }


  } // KnobIndex loop ...
  RcDebugPrint (SDBG_SWEEP_L1, ")");

  //
  // Print current dither counter per Bit/knob [b0k0:b0k1...,b1k0:b1k1...]
  //
  RcDebugPrint (SDBG_SWEEP_L3, "[");
  for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {
    for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {
      ResultConditionPtr = GetResultConditionPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, BitIndex);
      RcDebugPrint (SDBG_SWEEP_L3, "%2d:", ResultConditionPtr->DitherCounter);
    } // KnobIndex loop ...
    RcDebugPrint (SDBG_SWEEP_L3, ",");
  } // BitIndex loop ...
  RcDebugPrint (SDBG_SWEEP_L3, "]");
  //
  // Print current state
  //
  RcDebugPrint (SDBG_SWEEP_L2, "{%02d}", CommonParameters->CurrentState[ChannelIndex][StrobeLogical]);

  RcDebugPrint (SDBG_SWEEP_L0, " ");
  for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {
    AnyKnobPassed = FALSE;
    AllKnobPassed = TRUE;
    for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {

      ResultConditionPtr = GetResultConditionPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, BitIndex);

      PassCondition = PASS_CONDITION;
      FailCondition = FAIL_CONDITION;

      if (ResultConditionPtr->Current == PassCondition) {
        AnyKnobPassed = TRUE;
      }

      if (ResultConditionPtr->Current == FailCondition) {
        AllKnobPassed = FALSE;
      }

    } // KnobIndex loop ...

    if (AllKnobPassed) {
      RcDebugPrint (SDBG_SWEEP_L0, ".");
    } else if (AnyKnobPassed) {
      RcDebugPrint (SDBG_SWEEP_L0, "+");
    } else {
      RcDebugPrint (SDBG_SWEEP_L0, "#");
    }

  } // BitIndex loop ...

  if (BitIndex < BITS_PER_NIBBLE) {
    PrintLineBlankSpace (SDBG_SWEEP_L0, BITS_PER_NIBBLE - BitIndex);
  }

}

/*

  Print status for all strobes

  @param[in,out]  CommonParamaters  Pointer to the context information

*/
VOID
PrintCurrentStatus (
  IN         COMMON_1D_PARAMETERS    *CommonParameters
)
{
  UINT8   ChannelIndex;
  UINT8   SubChannelIndex;
  UINT8   StrobeLinear;
  UINT8   MaxChDdr;

  if ((CommonParameters->DebugLevel & SDBG_SWEEP_MASK) == 0) {
    return;
  }

  MaxChDdr = CommonParameters->MaxChDdr;

  for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    PrintLineBlankSpace (SDBG_SWEEP_L0, PRINT_TAB_SIZE * CommonParameters->IndentationLevel);

    RcDebugPrintWithDeviceInternal (
      SDBG_SWEEP_L0,
      CommonParameters->Socket,
      ChannelIndex,
      CommonParameters->Dimm[ChannelIndex],
      NO_SUBCH,
      CommonParameters->Rank[ChannelIndex],
      NO_STROBE,
      NO_BIT,
      "    "
      );

    RcDebugPrint (SDBG_SWEEP_L2, "-0x%08x-", CommonParameters->StrobeLimit[ChannelIndex]);

    for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
      for (StrobeLinear = 0; StrobeLinear < GetMaxStrobeToPrint (CommonParameters); StrobeLinear++) {

        PrintEachStrobeCurrentStatus (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear, CommonParameters->CurrentDelays);
        PrintEachStrobeCurrentStatus (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear + CommonParameters->NextNibbleOffset, CommonParameters->CurrentDelays);

      } // StrobeLinear loop ...
    } // SubChannelIndex loop ...

    RcDebugPrint (SDBG_SWEEP_L0, "\n");

  } // ChannelIndex loop ...

}

/*

  Print the data collected in Results on knob, channel and strobe basis

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      KnobIndex         Current knob under test (0-based)
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      SubChannelIndex   Current sub-channel under test (0-based)
  @param[in]      StrobeLinear      Current linear strobe under test (0-based)

*/
VOID
PrintCurrentStrobeResults1D (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   KnobIndex,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   SubChannelIndex,
  IN         UINT8                   StrobeLinear
)
{
  UINT8   StrobeLogical;
  UINT8   BitIndex;

  struct baseMargin *StrobeMargin;
  struct baseMargin *BitMargin;

  if (IsResultAtRankLevel (CommonParameters) && StrobeLinear >= 1) {
    //
    // Tag::StrobeLinearBreak::AtRankLevel
    // Only print information related with strobe 0
    //
    return;
  }

  StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);

  StrobeMargin = GetResultPtrAtStrobeLevel (
                   CommonParameters,
                   KnobIndex,
                   ChannelIndex,
                   StrobeLogical,
                   CommonParameters->Results
                   );

  RcDebugPrint (SDBG_SWEEP_L0, "(%4d:%4d)", StrobeMargin->n, StrobeMargin->p);

  if (CommonParameters->FlagsResults & ResultsPerBit) {

    RcDebugPrint (SDBG_SWEEP_L0, "[");
    for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {

      BitMargin = GetResultPtrAtBitLevel (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, BitIndex, CommonParameters->Results);

      if (BitMargin == NULL) {
        RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_001);
        continue;
      }

      RcDebugPrint (SDBG_SWEEP_L0, "%4d:%4d,", BitMargin->n, BitMargin->p);

    }
    RcDebugPrint (SDBG_SWEEP_L0, "]");
  }

}

/*

  Print the data collected in Results
  WARNING:
  This function MUST be executed only after context was already created (CreateContextParameters).
  This function MUST be executed only after resources are allocated (AllocatePoolForResults1D || AllocatePoolForResults1DAllRanks).


  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      Results           Pointer that contains the final results

*/
VOID
PrintCurrentResults1D (
  IN        COMMON_1D_PARAMETERS    *CommonParameters,
      OUT   RESULTS_1D  *Results
)
{
  UINT8   ChannelIndex;
  UINT8   SubChannelIndex;
  UINT8   MaxChDdr;
  UINT8   StrobeLinear;
  UINT8   KnobIndex;
  UINT32  CurrentDebugLevel;


  CurrentDebugLevel = GetDebugLevel ();
  CommonParameters->DebugLevel = CurrentDebugLevel;

  if ((CurrentDebugLevel & SDBG_SWEEP_MASK) == 0) {
    return;
  }

  if (CurrentDebugLevel & SDBG_SWEEP_RESULTS) {
    SetDebugLevel (SDBG_SWEEP_L0);
    CommonParameters->DebugLevel = GetDebugLevel ();
  }

  MaxChDdr = CommonParameters->MaxChDdr;

  CommonParameters->Results = Results;

  PrintHeaderByStrobes (CommonParameters, FormatResults1D);

  for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {

      for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {

        PrintLineBlankSpace (SDBG_SWEEP_L0, PRINT_TAB_SIZE * CommonParameters->IndentationLevel);

        RcDebugPrintWithDeviceInternal(
          SDBG_SWEEP_L0,
          CommonParameters->Socket,
          ChannelIndex,
          CommonParameters->Dimm[ChannelIndex],
          SubChannelIndex,
          CommonParameters->Rank[ChannelIndex],
          NO_STROBE,
          NO_BIT,
          " K%02d",
          KnobIndex
        );

        for (StrobeLinear = 0; StrobeLinear < GetMaxStrobeToPrint (CommonParameters); StrobeLinear++) {

          PrintCurrentStrobeResults1D (CommonParameters, KnobIndex, ChannelIndex, SubChannelIndex, StrobeLinear);
          PrintCurrentStrobeResults1D (CommonParameters, KnobIndex, ChannelIndex, SubChannelIndex, StrobeLinear + CommonParameters->NextNibbleOffset);

        } // StrobeLinear loop ...

        RcDebugPrint (SDBG_SWEEP_L0, "\n");

      } // SubChannelIndex loop ...
    } // KnobIndex loop ...

  } // ChannelIndex loop

  SetDebugLevel (CurrentDebugLevel);

}

/**

  Print the data collected in legacy results structures on channel for current strobe

  @param[in,out]  CommonParameters  Pointer to the context information
  @param[in]      VerticalSampleIndex  Current vertical index (only used for 2D sweep), for 1D sweep always use 0
  @param[in]      ChannelIndex      Current channel under test (0-based)
  @param[in]      SubChannelIndex   Current sub-channel under test (0-based)
  @param[in]      StrobeLinear      Current linear strobe under test (0-based)

**/
VOID
PrintCurrentLegacyStrobeResults (
  IN         COMMON_1D_PARAMETERS    *CommonParameters,
  IN         UINT8                   VerticalSampleIndex,
  IN         UINT8                   ChannelIndex,
  IN         UINT8                   SubChannelIndex,
  IN         UINT8                   StrobeLinear,
      OUT    struct strobeMargin     *StrobeResults, OPTIONAL
      OUT    struct bitMargin        *BitResults,    OPTIONAL
      OUT    struct rankMargin       *RankResults    OPTIONAL
)
{
  UINT8   LogicalRank;
  UINT8   StrobeLogical;
  UINT8   BitIndex;
  UINT8   BitIndexWithinChannel;

  if (IsResultAtRankLevel (CommonParameters) && StrobeLinear >= 1) {
    //
    // Tag::StrobeLinearBreak::AtRankLevel
    // Only print information related with strobe 0
    //
    return;
  }

  LogicalRank = GetLogicalRank (
                  CommonParameters->Host,
                  CommonParameters->Socket,
                  ChannelIndex,
                  CommonParameters->Dimm[ChannelIndex],
                  CommonParameters->Rank[ChannelIndex]
                  );


  StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);

  if (StrobeResults != NULL) {
    RcDebugPrint (
      SDBG_SWEEP_L0,
      "(%4d:%4d)",
      StrobeResults[VerticalSampleIndex].strobe[ChannelIndex][LogicalRank][StrobeLogical].n,
      StrobeResults[VerticalSampleIndex].strobe[ChannelIndex][LogicalRank][StrobeLogical].p
      );
  } else {
    RcDebugPrint (
      SDBG_SWEEP_L0,
      "(NULL:NULL)"
      );
  }


  if (CommonParameters->FlagsResults & ResultsPerBit) {

    RcDebugPrint (SDBG_SWEEP_L0, "[");

    for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {

      BitIndexWithinChannel = GetBitIndexWithinChannel (
                                CommonParameters,
                                ChannelIndex,
                                SubChannelIndex,
                                StrobeLogical,
                                BitIndex
                                );

      if (BitResults != NULL) {
        RcDebugPrint (
          SDBG_SWEEP_L0,
          "%4d:%4d,",
          BitResults[VerticalSampleIndex].bits[ChannelIndex][LogicalRank][BitIndexWithinChannel].n,
          BitResults[VerticalSampleIndex].bits[ChannelIndex][LogicalRank][BitIndexWithinChannel].p
          );
      } else {
        RcDebugPrint (
          SDBG_SWEEP_L0,
          "NULL:NULL,"
          );
      }

    }
    RcDebugPrint (SDBG_SWEEP_L0, "]");
  }

}

/**

  Print the data collected in legacy results structures on channel, strobe and rank basis

  @param[in,out]  CommonParameters  Pointer to the context information
  @param[in]      KnobIndex         Current knob under test (0-based)
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
)
{

  UINT8   ChannelIndex;
  UINT8   LogicalRank;
  UINT8   SubChannelIndex;
  UINT8   MaxChDdr;
  UINT8   StrobeLinear;
  UINT32  CurrentDebugLevel;

  CurrentDebugLevel = GetDebugLevel ();
  CommonParameters->DebugLevel = CurrentDebugLevel;

  if ((CurrentDebugLevel & SDBG_SWEEP_MASK) == 0) {
    return;
  }

  if (CurrentDebugLevel & SDBG_SWEEP_RESULTS) {
    SetDebugLevel (SDBG_SWEEP_L0);
    CommonParameters->DebugLevel = GetDebugLevel ();
  }


  MaxChDdr = CommonParameters->MaxChDdr;

  PrintHeaderByStrobes (CommonParameters, FormatResults1D);

  for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {

      PrintLineBlankSpace (SDBG_SWEEP_L0, PRINT_TAB_SIZE * CommonParameters->IndentationLevel);
      if (KnobIndex == NO_KNOB_INDEX) {

        RcDebugPrintWithDeviceInternal(
          SDBG_SWEEP_L0,
          CommonParameters->Socket,
          ChannelIndex,
          CommonParameters->Dimm[ChannelIndex],
          SubChannelIndex,
          CommonParameters->Rank[ChannelIndex],
          NO_STROBE,
          NO_BIT,
          "    "
        );

      } else {

        RcDebugPrintWithDeviceInternal(
          SDBG_SWEEP_L0,
          CommonParameters->Socket,
          ChannelIndex,
          CommonParameters->Dimm[ChannelIndex],
          SubChannelIndex,
          CommonParameters->Rank[ChannelIndex],
          NO_STROBE,
          NO_BIT,
          "K%02d ",
          KnobIndex
        );

      }

      for (StrobeLinear = 0; StrobeLinear < GetMaxStrobeToPrint (CommonParameters); StrobeLinear++) {

        PrintCurrentLegacyStrobeResults (CommonParameters, 0, ChannelIndex, SubChannelIndex, StrobeLinear, StrobeResults, BitResults, RankResults);
        PrintCurrentLegacyStrobeResults (CommonParameters, 0, ChannelIndex, SubChannelIndex, StrobeLinear + CommonParameters->NextNibbleOffset, StrobeResults, BitResults, RankResults);

      } // StrobeLinear loop ...

      RcDebugPrint (SDBG_SWEEP_L0, "\n");

    } // SubChannelIndex loop ...

    PrintLineBlankSpace (SDBG_SWEEP_L0, PRINT_TAB_SIZE * CommonParameters->IndentationLevel);

    LogicalRank = GetLogicalRank (
                    CommonParameters->Host,
                    CommonParameters->Socket,
                    ChannelIndex,
                    CommonParameters->Dimm[ChannelIndex],
                    CommonParameters->Rank[ChannelIndex]
                    );
    if (KnobIndex == NO_KNOB_INDEX) {

      RcDebugPrintWithDeviceInternal(
        SDBG_SWEEP_L0,
        CommonParameters->Socket,
        ChannelIndex,
        CommonParameters->Dimm[ChannelIndex],
        NO_SUBCH,
        CommonParameters->Rank[ChannelIndex],
        NO_STROBE,
        NO_BIT,
        "        "
      );

    } else {

      RcDebugPrintWithDeviceInternal(
        SDBG_SWEEP_L0,
        CommonParameters->Socket,
        ChannelIndex,
        CommonParameters->Dimm[ChannelIndex],
        NO_SUBCH,
        CommonParameters->Rank[ChannelIndex],
        NO_STROBE,
        NO_BIT,
        "    K%02d ",
        KnobIndex
      );

    }
    if (RankResults != NULL) {

      RcDebugPrint (
        SDBG_SWEEP_L0,
        "(%4d:%4d)\n",
        RankResults->rank[ChannelIndex][LogicalRank].n,
        RankResults->rank[ChannelIndex][LogicalRank].p
      );

    } else {

      RcDebugPrint (
        SDBG_SWEEP_L0,
        "(NULL:NULL)\n"
      );

    }

  } // ChannelIndex loop

  SetDebugLevel (CurrentDebugLevel);

}

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
)
{
  INT8    VerticalSampleIndex;
  UINT8   ChannelIndex;
  UINT8   LogicalRank;
  UINT8   SubChannelIndex;
  UINT8   MaxChDdr;
  UINT8   StrobeLinear;
  INT16   OffsetZeroFromVerticalSamples;
  UINT32  CurrentDebugLevel;
  COMMON_1D_PARAMETERS *CommonParametersV;

  CommonParametersV = &CommonParameters2D->CommonParametersV;

  CurrentDebugLevel = GetDebugLevel ();
  CommonParametersV->DebugLevel = CurrentDebugLevel;

  if ((CurrentDebugLevel & SDBG_SWEEP_MASK) == 0) {
    return;
  }

  if (CurrentDebugLevel & SDBG_SWEEP_RESULTS) {
    SetDebugLevel (SDBG_SWEEP_L0);
    CommonParametersV->DebugLevel = GetDebugLevel ();
  }

  OffsetZeroFromVerticalSamples = (CommonParameters2D->MaxVerticalEntries - 1) / 2;

  MaxChDdr = CommonParametersV->MaxChDdr;

  PrintHeaderByStrobes (CommonParametersV, FormatResults1D);

  for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {

    if (SkipChannelExecution (CommonParametersV, ChannelIndex)) {
      continue;
    }

    for (VerticalSampleIndex = 0; VerticalSampleIndex < CommonParameters2D->MaxVerticalEntries; VerticalSampleIndex++) {

      PrintLineBlankSpace (SDBG_SWEEP_L0, PRINT_TAB_SIZE * CommonParametersV->IndentationLevel);

      RcDebugPrintWithDeviceInternal(
                SDBG_SWEEP_L0,
                CommonParametersV->Socket,
                ChannelIndex,
                CommonParametersV->Dimm[ChannelIndex],
                NO_SUBCH,
                CommonParametersV->Rank[ChannelIndex],
                NO_STROBE,
                NO_BIT,
                "Vertical Offset = %03d\n",
                (VerticalSampleIndex - OffsetZeroFromVerticalSamples) * CommonParametersV->StepSize
              );

      for (SubChannelIndex = CommonParametersV->MinSubChannelIndex; SubChannelIndex < CommonParametersV->MaxSubChannelIndex; SubChannelIndex++) {

        PrintLineBlankSpace (SDBG_SWEEP_L0, PRINT_TAB_SIZE * CommonParametersV->IndentationLevel);

        RcDebugPrintWithDeviceInternal(
          SDBG_SWEEP_L0,
          CommonParametersV->Socket,
          ChannelIndex,
          CommonParametersV->Dimm[ChannelIndex],
          SubChannelIndex,
          CommonParametersV->Rank[ChannelIndex],
          NO_STROBE,
          NO_BIT,
          "    "
        );

        for (StrobeLinear = 0; StrobeLinear < GetMaxStrobeToPrint (CommonParametersV); StrobeLinear++) {

          PrintCurrentLegacyStrobeResults (CommonParametersV, VerticalSampleIndex, ChannelIndex, SubChannelIndex, StrobeLinear, StrobeResults, BitResults, RankResults);
          PrintCurrentLegacyStrobeResults (CommonParametersV, VerticalSampleIndex, ChannelIndex, SubChannelIndex, StrobeLinear + CommonParametersV->NextNibbleOffset, StrobeResults, BitResults, RankResults);

        } // StrobeLinear loop ...

        RcDebugPrint (SDBG_SWEEP_L0, "\n");

      } // SubChannelIndex loop ...

      PrintLineBlankSpace (SDBG_SWEEP_L0, PRINT_TAB_SIZE * CommonParametersV->IndentationLevel);

      LogicalRank = GetLogicalRank (
                      CommonParametersV->Host,
                      CommonParametersV->Socket,
                      ChannelIndex,
                      CommonParametersV->Dimm[ChannelIndex],
                      CommonParametersV->Rank[ChannelIndex]
                      );

      RcDebugPrintWithDeviceInternal(
        SDBG_SWEEP_L0,
        CommonParametersV->Socket,
        ChannelIndex,
        CommonParametersV->Dimm[ChannelIndex],
        NO_SUBCH,
        CommonParametersV->Rank[ChannelIndex],
        NO_STROBE,
        NO_BIT,
        "        "
      );

      if (RankResults != NULL) {

        RcDebugPrint (
          SDBG_SWEEP_L0,
          "(%4d:%4d)\n",
          RankResults[VerticalSampleIndex].rank[ChannelIndex][LogicalRank].n,
          RankResults[VerticalSampleIndex].rank[ChannelIndex][LogicalRank].p
        );

      } else {

        RcDebugPrint (
          SDBG_SWEEP_L0,
          "(NULL:NULL)\n"
        );

      }

    }
  } // ChannelIndex loop

  SetDebugLevel (CurrentDebugLevel);

}
