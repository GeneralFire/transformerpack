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

typedef UINT8 (*GetNibbleOffsetPosition) (COMMON_1D_PARAMETERS *CommonParameters, UINT8 StrobeLogical);

/*

  Callback function to configure the engine before start doing the tests

  @param[in]      CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       Callback function executed properly

*/
MRC_STATUS
EFIAPI
SetupSweepAdvance (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
)
{
  UINT8   ChannelIndex;
  UINT8   MaxChDdr;
  UINT8   LogRank;
  UINT8   SubRank;
  UINT32  Pattern;
  UINT32  ChannelDdrEn;

  MaxChDdr = CommonParameters->MaxChDdr;

  for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    LogRank = GetLogicalRank (
                CommonParameters->Host,
                CommonParameters->Socket,
                ChannelIndex,
                CommonParameters->Dimm[ChannelIndex],
                CommonParameters->Rank[ChannelIndex]
                );

    if (CheckCMDGroup (CommonParameters->Group) || CheckCTLGroup (CommonParameters->Group)) {

      SubRank = 0;
      Pattern = DDR_CPGC_ADV_CMD_PATTERN;

    } else {

      SubRank = CommonParameters->SubRank[ChannelIndex];
      Pattern = DDR_CPGC_DQ_PATTERN;

    }


    if (IsBrsPresent (CommonParameters->Host, CommonParameters->Socket)) {
      //
      // BRS always assume that training are executed with ChannelMask == 0xAAAAAAAA
      //
      ChannelDdrEn = (1 << ChannelIndex) | (1 << (ChannelIndex + 1));

    } else {

      ChannelDdrEn = (1 << ChannelIndex);

    }

    CpgcAdvTrainingSetup (
      CommonParameters->Host,
      CommonParameters->Socket,
      ChannelDdrEn,
      CommonParameters->Dimm[ChannelIndex],
      LogRank,
      SubRank,
      Pattern,
      PatternLengthToCpgcLoopCnt (64)
      );

  }

  return MRC_STATUS_SUCCESS;
}

/*

  Callback function to configure and/or start the test for most advanced training algorithms

  @param[in]      CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       Callback function executed properly

*/
MRC_STATUS
EFIAPI
ExecuteSweepAdvance (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
)
{
  UINT8   ChannelIndex;
  UINT8   MaxChDdr;
  UINT32  ChannelToCheck;
  MaxChDdr = CommonParameters->MaxChDdr;

  IO_Reset (
    CommonParameters->Host,
    CommonParameters->Socket
    );

  if (CommonParameters->Group == TxVref && CommonParameters->Level == DdrLevel) {
    //
    // TxVref uses the engine to program the value for the group, so it
    // is required to reconfigure the entire engine before executing the test
    //
    if (CommonParameters->SetupTest != NULL) {
      CommonParameters->SetupTest (CommonParameters);
    }
  }

  ChannelToCheck = 0;
  for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    ChannelToCheck |= 1 << ChannelIndex;
  }

  if (IsBrsPresent (CommonParameters->Host, CommonParameters->Socket)) {
    ChannelToCheck = ChannelToCheck | (ChannelToCheck << 1);
  }

  CpgcAdvTrainingExecTestGroup (CommonParameters->Host, CommonParameters->Socket, ChannelToCheck, 0);

  return MRC_STATUS_SUCCESS;
}

/*

  Return the Nibble Offset Position for DDR4 based on StrobeLogical
  usage model:

  Considering,
  RegisterA  =   0x76543210
  RegisterB  =   0xFEDCBA98
  RegisterC  =   0x76543210

  If for a particular StrobeLogical,
  Nibble Offset Position is  3, RegisterA.Nibble3 (Value = 3) should be accessed
  Nibble Offset Position is 15, RegisterB.Nibble7 (Value = F) should be accessed
  Nibble Offset Position is 16, RegisterC.Nibble0 (Value = 0) should be accessed

  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      StrobeLogical     Current logical strobe under test (0-based)

  @return  Pointer to Nibble Offset Position for DDR4

*/
UINT8
GetStrobeToNibbleOffsetPositionForDdr4 (
  IN          COMMON_1D_PARAMETERS  *CommonParameters,
  IN          UINT8                 StrobeLogical
)
{
  UINT8   StrobeToNibbleOffsetInErrorRegistersDdr4[] = {
            0,  // Strobe 0  --> [3:0]
            2,  // Strobe 1  --> [11:8]
            4,  // Strobe 2  --> [19:16]
            6,  // Strobe 3  --> [27:24]
            8,  // Strobe 4  --> [35:32]
            10, // Strobe 5  --> [43:40]
            12, // Strobe 6  --> [51:48]
            14, // Strobe 7  --> [59:56]
            16, // Strobe 8  --> [67:64] ECC
            1,  // Strobe 9  --> [7:4]
            3,  // Strobe 10 --> [15:12]
            5,  // Strobe 11 --> [23:20]
            7,  // Strobe 12 --> [31:28]
            9,  // Strobe 13 --> [39:36]
            11, // Strobe 14 --> [47:44]
            13, // Strobe 15 --> [53:52]
            15, // Strobe 16 --> [63:60]
            17  // Strobe 17 --> [71:68] ECC
          };

  if (StrobeLogical >= MAX_STROBE_DDR4) {

    RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_019);

  }

  return StrobeToNibbleOffsetInErrorRegistersDdr4[StrobeLogical];

}

/*

  Return the Nibble Offset Position for DDR5 based on StrobeLogical
  usage model:

  Considering,
  RegisterA  =   0x76543210
  RegisterB  =   0xFEDCBA98
  RegisterC  =   0x76543210

  If for a particular StrobeLogical,
  Nibble Offset Position is  3, RegisterA.Nibble3 (Value = 3) should be accessed
  Nibble Offset Position is 15, RegisterB.Nibble7 (Value = F) should be accessed
  Nibble Offset Position is 16, RegisterC.Nibble0 (Value = 0) should be accessed

  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      StrobeLogical     Current logical strobe under test (0-based)

  @return  Pointer to Nibble Offset Position for DDR5

*/
UINT8
GetStrobeToNibbleOffsetPositionForDdr5 (
  IN          COMMON_1D_PARAMETERS  *CommonParameters,
  IN          UINT8                 StrobeLogical
)
{
  UINT8   StrobeToNibbleOffsetInErrorRegistersDdr5[] = {
            //
            // Sub Channel 0
            //
            0,  // Strobe 0  --> [3:0]
            2,  // Strobe 1  --> [11:8]
            4,  // Strobe 2  --> [19:16]
            6,  // Strobe 3  --> [27:24]
            16, // Strobe 4  --> [35:32] ECC
            1,  // Strobe 5  --> [7:4]
            3,  // Strobe 6  --> [15:12]
            5,  // Strobe 7  --> [23:20]
            7,  // Strobe 8  --> [31:28]
            17, // Strobe 9  --> [39:36] ECC
            //
            // Sub Channel 1
            //
            0,  // Strobe 0  --> [3:0]
            2,  // Strobe 1  --> [11:8]
            4,  // Strobe 2  --> [19:16]
            6,  // Strobe 3  --> [27:24]
            16, // Strobe 4  --> [35:32] ECC
            1,  // Strobe 5  --> [7:4]
            3,  // Strobe 6  --> [15:12]
            5,  // Strobe 7  --> [23:20]
            7,  // Strobe 8  --> [31:28]
            17, // Strobe 9  --> [39:36] ECC
          };

  if (StrobeLogical >= MAX_STROBE_DDR5) {

    RC_FATAL_ERROR (FALSE, ERR_RC_SWEEP_LIB_INTERNAL, RC_FATAL_ERROR_SWEEP_LIB_MINOR_CODE_020);

  }

  return StrobeToNibbleOffsetInErrorRegistersDdr5[StrobeLogical];

}

/*

  Return the odd and even status based on the CPGC error collection

  @param[in,out]  CommonParamaters  Pointer to the context information
  @param[in]      CpgcErrorStatus   Multidimensional array to CPGC error collection
  @param[in]      StrobeLogical     Current logical strobe under test (0-based)
  @param[in]      BitIndex          Current bit under test (0-based)
  @param[out]     EvenStatus        Even status for current bit
  @param[out]     OddStatus         Odd status for current bit

*/
VOID
GetBitStatus (
  IN          COMMON_1D_PARAMETERS  *CommonParameters,
  IN          CPGC_ERROR_STATUS     CpgcErrorStatus[SUB_CH][MAX_CH],
  IN          UINT8                 ChannelIndex,
  IN          UINT8                 StrobeLogical,
  IN          UINT8                 BitIndex,
      OUT     UINT8                 *EvenStatus,
      OUT     UINT8                 *OddStatus
)
{
  UINT32  ErrorStrobeEven;
  UINT32  ErrorStrobeOdd;
  UINT8   NibbleOffsetPosition;
  GetNibbleOffsetPosition GetNibbleOffsetPositionPtr;
  CPGC_ERROR_STATUS   *CpgcErrorStatusPtr;

  ErrorStrobeEven   = 0;
  ErrorStrobeOdd    = 0;

  if (CommonParameters->DdrType == Ddr4Type) {

    GetNibbleOffsetPositionPtr = GetStrobeToNibbleOffsetPositionForDdr4;
    CpgcErrorStatusPtr = CpgcErrorStatus[0];

  } else {

    GetNibbleOffsetPositionPtr = GetStrobeToNibbleOffsetPositionForDdr5;

    if (StrobeLogical >= (MAX_STROBE_DDR5 / 2)) {

      //
      // Sub-Channel 1 results
      //
      CpgcErrorStatusPtr = CpgcErrorStatus[1];

    } else {

      //
      // Sub-Channel 0 results
      //
      CpgcErrorStatusPtr = CpgcErrorStatus[0];

    }

  }

  NibbleOffsetPosition = GetNibbleOffsetPositionPtr (CommonParameters, StrobeLogical);

  if (NibbleOffsetPosition <= 7) {

    ErrorStrobeEven = CpgcErrorStatusPtr[ChannelIndex].cpgcErrDat0S;
    ErrorStrobeOdd  = CpgcErrorStatusPtr[ChannelIndex].cpgcErrDat2S;

  } else if (NibbleOffsetPosition <= 15) {

    ErrorStrobeEven = CpgcErrorStatusPtr[ChannelIndex].cpgcErrDat1S;
    ErrorStrobeOdd  = CpgcErrorStatusPtr[ChannelIndex].cpgcErrDat3S;

  } else {

    //
    // For DDR4 ECC strobes {8, 17}
    // For DDR5 ECC strobes {4, 9, 14, 19}
    //
    ErrorStrobeEven = CpgcErrorStatusPtr[ChannelIndex].cpgcErrEccS;
    ErrorStrobeOdd  = (CpgcErrorStatusPtr[ChannelIndex].cpgcErrEccS >> 8) & 0xFF;

  }

  ErrorStrobeEven >>= ((NibbleOffsetPosition % 8) * BITS_PER_NIBBLE);
  ErrorStrobeOdd  >>= ((NibbleOffsetPosition % 8) * BITS_PER_NIBBLE);

  *EvenStatus = (ErrorStrobeEven >> BitIndex) & 0x1;
  *OddStatus  = (ErrorStrobeOdd  >> BitIndex) & 0x1;

}

/*

  Callback to interpret the HW results after each sample test completes for most advanced training algorithms

  @param[in]      CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       Callback function executed properly

*/
MRC_STATUS
EFIAPI
ResultProcessingAdvanceTargetHw (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
)
{
  UINT8   ChannelIndex;
  UINT8   SubChannelIndex;
  UINT8   MaxChDdr;
  UINT8   StrobeLogical;
  UINT8   StrobeLinear;
  UINT8   BitIndex;
  UINT8   KnobIndex;
  UINT8   EvenStatus;
  UINT8   OddStatus;
  
  CPGC_ERROR_STATUS CpgcErrorStatus[SUB_CH][MAX_CH];

  UINT32  ChannelToCheck;
  RESULTS_1D_DEFINITION PassCondition;
  RESULTS_1D_DEFINITION FailCondition;
  RESULTS_1D_CONDITION  *ResultConditionPtr;

  MaxChDdr = CommonParameters->MaxChDdr;

  ChannelToCheck = 0;
  for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    ChannelToCheck |= 1 << ChannelIndex;
  }

  if (CommonParameters->DdrType == Ddr4Type) {

    CpgcAdvTrainingErrorStatus (
      CommonParameters->Host,
      CommonParameters->Socket,
      ChannelToCheck,
      CpgcErrorStatus[0]
      );

  } else {
#ifdef DDR5_SUPPORT
    CpgcAdvTrainingErrorStatusAdapter (
      CommonParameters->Host,
      CommonParameters->Socket,
      ChannelToCheck,
      CpgcErrorStatus
      );
#endif // #ifdef DDR5_SUPPORT
  }

  for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {
      for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
        for (StrobeLinear = 0; StrobeLinear < CommonParameters->MaxStrobeAvailable; StrobeLinear++) {
          //
          // Tag::StrobeLinearBreak::No Break
          // Not needed the strobe break as every single lane needs to be populated accordingly
          //
          StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);

          for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {

            ResultConditionPtr = GetResultConditionPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, BitIndex);

            PassCondition = PASS_CONDITION;
            FailCondition = FAIL_CONDITION;

            GetBitStatus (CommonParameters, CpgcErrorStatus, ChannelIndex, StrobeLogical, BitIndex, &EvenStatus, &OddStatus);

            if (CommonParameters->Group == RxDqsDelay || CommonParameters->Group == RxSamplerEvenOdd || (CommonParameters->FlagsResults & ResultsEvenOdd)) {
              if (KnobIndex == EvenChunkSide) {
                //
                // RxDqsPDelay OR RxSamplerEven or Even
                //
                if (EvenStatus == 1) {
                  ResultConditionPtr->Current = FailCondition;
                } else {
                  ResultConditionPtr->Current = PassCondition;
                }
              } else {
                //
                // RxDqsNDelay OR RxSamplerOdd or Odd
                //
                if (OddStatus == 1) {
                  ResultConditionPtr->Current = FailCondition;
                } else {
                  ResultConditionPtr->Current = PassCondition;
                }
              }
            } else if (CommonParameters->Group == RxDqsPDelay || CommonParameters->Group == RxSamplerEven) {
              if (EvenStatus == 1) {
                ResultConditionPtr->Current = FailCondition;
              } else {
                ResultConditionPtr->Current = PassCondition;
              }
            } else if (CommonParameters->Group == RxDqsNDelay || CommonParameters->Group == RxSamplerOdd) {
              if (OddStatus == 1) {
                ResultConditionPtr->Current = FailCondition;
              } else {
                ResultConditionPtr->Current = PassCondition;
              }
            } else {
              if (EvenStatus == 0 && OddStatus == 0) {

                ResultConditionPtr->Current = PassCondition;

              } else {

                ResultConditionPtr->Current = FailCondition;

              }
            }

          } // BitIndex loop ...

        } // StrobeLinear loop ...
      } // SubChannelIndex loop ...
    } // KnobIndex loop ...

  } // ChannelIndex loop ...

  return MRC_STATUS_SUCCESS;
}

/*

  Callback to interpret the HW/RcSim results after each sample test completes for most advanced training algorithms

  @param[in]      CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       Callback function executed properly

*/
MRC_STATUS
EFIAPI
ResultProcessingAdvance (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
)
{

  ResultProcessingAdvanceTargetHw (CommonParameters);
  ResultProcessingAdvanceTargetRcSim (CommonParameters, NULL);

  return MRC_STATUS_SUCCESS;
}

/*

  Callback to be used always when the horizontal sweep will be executed in a 2D sweep

  @param[in]      CommonParameters  Pointer to the context information for 2D usage (Pointer to COMMON_1D_PARAMETERS_FOR_2D)

  @retval         MRC_STATUS_SUCCESS       Callback function executed properly

*/
MRC_STATUS
EFIAPI
PreSweepHorizontal2D (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
)
{
  COMMON_1D_PARAMETERS_FOR_2D *CommonParametersFor2D;

  CommonParametersFor2D = (COMMON_1D_PARAMETERS_FOR_2D *)CommonParameters;

  CommonParameters->IndentationLevel = CommonParametersFor2D->IndentationLevel;
  //
  // In the 2D sweep CommonParametersFor2D->PreSweep1DH is forced to 
  // use PreSweepHorizontal2D when using ProcessHorizontalSweep as callback
  // To avoid infinite recursion we need to use the PreSweep1DH specified by original API call
  // the one that is populated as part of the 2D worker (CommonParameters2D->PreSweep1DH)
  //
  if (CommonParametersFor2D->CommonParameters2D != NULL) {
    if (CommonParametersFor2D->CommonParameters2D->PreSweep1DH != NULL) {
      CommonParametersFor2D->CommonParameters2D->PreSweep1DH (CommonParameters);
    }
  }

  return MRC_STATUS_SUCCESS;
}


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
)
{

  COMMON_1D_PARAMETERS_FOR_2D *CommonParametersFor2D;

  CommonParametersFor2D = (COMMON_1D_PARAMETERS_FOR_2D *)CommonParameters;

  ResultProcessingAdvanceTargetHw (CommonParameters);
  ResultProcessingAdvanceTargetRcSim (CommonParameters, CommonParametersFor2D->CommonParameters2D);

  return MRC_STATUS_SUCCESS;
}

/*

  Function performs common IO reset and JedecInit.

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       Callback function executed properly

*/
VOID
CommonIoResetAndJedecInit (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  UINT8       ChannelIndex;
  UINT8       MaxChDdr;
  UINT8       Socket;
  PSYSHOST    Host;

  MaxChDdr = CommonParameters->MaxChDdr;
  Socket = CommonParameters->Socket;
  Host = CommonParameters->Host;

  IO_Reset (Host, Socket);

  for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {
    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    IODdrtReset (CommonParameters->Host, CommonParameters->Socket, CommonParameters->Dimm[ChannelIndex]);
  }

  FifoTrainReset (Host, Socket);

  JedecInitSequence (Host, Socket, CH_BITMASK);

}
/*

  Callback to be executed after error in Cmd/Ctl test

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       Callback function executed properly

*/
MRC_STATUS
EFIAPI
ErrorHandlerAdvanceCmdCtl (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  UINT8       ChannelIndex;
  UINT8       MaxChDdr;
  UINT8       Socket;
  PSYSHOST    Host;

  MaxChDdr = CommonParameters->MaxChDdr;
  Socket = CommonParameters->Socket;
  Host = CommonParameters->Host;

  CommonIoResetAndJedecInit (CommonParameters);

  for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {
    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }
    if (IsDcpmmPresentDimm (Socket, ChannelIndex, CommonParameters->Dimm[ChannelIndex])) {
      RPQDrain (Host, Socket, ChannelIndex, CommonParameters->Dimm[ChannelIndex], CommonParameters->Rank[ChannelIndex]);
    }
  }

  if (IsDcpmmPresentSocket (Socket)) {
    ClrPcheMiss (Host, Socket);
  }

  return MRC_STATUS_SUCCESS;
}

/*

  Callback to be executed after detecting error in TxVref test

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS       Callback function executed properly

*/
MRC_STATUS
EFIAPI
ErrorHandlerAdvanceTxVref (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{

  CommonIoResetAndJedecInit (CommonParameters);

  return MRC_STATUS_SUCCESS;
}
