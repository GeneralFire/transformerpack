/** @file
  Margin2DSweep.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include "Margin2DSweep.h"
#include <Library/MemoryCoreLib.h>
#include <Library/MemoryServicesLib.h>

#include <ReferenceCodeFatalErrors.h>

/*

  Callback function to initial or clean up margin parameters setting before/after margin test

  @param[in]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
Margin2DInitMarginParam (
  IN  COMMON_1D_PARAMETERS    *CommonParameters,
  IN  SETUP_CLEANUP            SetupCleanup
)
{
  MARGIN_2D_PARAMETER *Margin2DSweep;
  MRC_LT               Level;
  MRC_GT               Group;
  UINT8                Socket;
  SSA_STATUS           RetStat;
  SHMOO_DIRECTION      ShmooDir = LoDirection;

  Margin2DSweep = (MARGIN_2D_PARAMETER *)CommonParameters;

  Socket = CommonParameters->Socket;
  Level = CommonParameters->Level;
  Group = CommonParameters->Group;

  //
  // Get the margin direction. The "SetupCleanup" parameter of the InitMarginParam has the margin
  // direction encoded in it.
  //
  if (CommonParameters->FlagsFsm & LeftSideOnly) {
    ShmooDir = LoDirection;
  } else if (CommonParameters->FlagsFsm & RightSideOnly) {
    ShmooDir = HiDirection;
  }

  RetStat = Margin2DSweep->SsaServicesHandle->SsaMemoryConfig->InitMarginParam (
    Margin2DSweep->SsaServicesHandle, Socket, Level, Group, (SetupCleanup | (ShmooDir << 1)));

  if (ChkRetStat (Margin2DSweep->SsaServicesHandle, RetStat, __FILE__, __LINE__, Margin2DSweep->CoreTestData->pTestStat)) {
    return MRC_STATUS_FAILURE;
  }

  return MRC_STATUS_SUCCESS;
}
/*

  Callback function to be executed before any sweep is done for most advanced training algorithms

  @param[in]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
Margin2DPre1DSweep (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
)
{
  return Margin2DInitMarginParam (CommonParameters, Setup);
}

/*

Callback function to be executed after any sweep is done for most advanced training algorithms

@param[in]  CommonParamaters  Pointer to the context information

@retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
Margin2DPost1DSweep (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
)
{
  return Margin2DInitMarginParam (CommonParameters, Cleanup);
}

/**
  This function initializes the inner 1D Sweep parameters.
  NOTE: This function is called inside the outer 1D sweep context.

  It is called at each outer loop step

  @param[in, out]  CommonParamaters     Pointer to the 2D margin context information

**/
STATIC
VOID
InitializeInner1DSweepParameter (
  OUT    COMMON_1D_PARAMETERS   *CommonParameters
)
{
  MARGIN_2D_PARAMETER     *Margin2DSweep;
  COMMON_1D_PARAMETERS    *OuterCommonParameters;
  COMMON_1D_PARAMETERS    *InnerCommonParameters;
  MARGIN2D_CORE_DATA      *CoreTestData;
  UINT32                  ChannelBitMask;
  MRC_RT                  DimmRank[MAX_CH];
  MRC_LT                  Level;
  MRC_GT                  Group;
  UINT8                   Socket;
  UINT8                   StepSize;
  UINT8                   DitherValue;
  INT16                   OffsetLeft;
  INT16                   OffsetRight;
  INT16                   LimitOffset;
  FLAGS_CATEGORY_OFFSETS  FlagsOffsets;
  FLAGS_CATEGORY_RESULTS  FlagsResults;
  FLAGS_CATEGORY_FSM      FlagsFsm;
  MRC_MST                 MemSsType;


  Margin2DSweep = (MARGIN_2D_PARAMETER *)CommonParameters;
  OuterCommonParameters = &Margin2DSweep->OuterCommonParameters;
  InnerCommonParameters = &Margin2DSweep->InnerCommonParameters;
  CoreTestData = Margin2DSweep->CoreTestData;

  Socket = Margin2DSweep->OuterCommonParameters.Socket;

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

  ChannelBitMask = OuterCommonParameters->ChannelMask;

  //
  // Get the list of DIMM/RANK
  //
  GetMargin2DTargetedDimmRank (Margin2DSweep->SsaServicesHandle, CoreTestData, Socket, &DimmRank);

  //
  // Get the inner margin parameter
  //
  Level = CoreTestData->CoreTestCfg.IoLevel;
  Group = CoreTestData->CoreTestCfg.FastMarginGroup;
  StepSize = CoreTestData->CoreTestCfg.FastStepSize;
  DitherValue = 1;

  //
  // Use previous margin result as the starting offset to speed up the test.
  //
  OffsetLeft = Margin2DSweep->InnerLeft;
  OffsetRight = Margin2DSweep->InnerRight;
  LimitOffset = 0;

  RcDebugPrint (SDBG_DEFAULT, "Inner Left offset:%d  Right offset:%d \n", OffsetLeft, OffsetRight);

  //
  // Flags
  //
  FlagsOffsets = DefaultStartOffset;
  FlagsResults = ResultsPerBit | ResultsRelative;
  //
  // Margin to the first error for Cmd/Ctl
  //
  if (CheckCMDGroup (Group) || CheckCTLGroup (Group)) {
    FlagsResults |= ResultsPerSubChannel;
  }

  FlagsFsm = DefaultFsm;

  CreateContextParameters (InnerCommonParameters, MemSsType, Socket, ChannelBitMask, 0, ALL_SUBCH, DimmRank, Level, Group, gsmCsnDelim,
    StepSize, DitherValue, OffsetLeft, OffsetRight, LimitOffset, FlagsOffsets, FlagsResults, FlagsFsm);

}

/**
  This function to get the channel margins of the Inner 1D Sweep.
  They are the minimum margins of both sides.

  @param[in]  CommonParamaters     Pointer to the context information
  @param[in]  Results              Pointer to 1D sweep margin result
  @param[in]  Channel              Channel index in socket (0 based)
  @param[out] Left                 Ponter to "Left"/"Low"/"Negative" side margin
  @param[out] Right                Ponter to "Right"/"High"/"Positive" side margin

**/

STATIC
VOID
GetInner1DSweepMargins (
  IN    COMMON_1D_PARAMETERS   *CommonParameters,
  IN    RESULTS_1D             *Results,
  IN    UINT8                  Channel,
  OUT   INT16                  *Left,
  OUT   INT16                  *Right
  )
{
  UINT8 Lane;
  UINT8 Knob;
  INT16 MaxLeft;
  INT16 MinRight;
  struct baseMargin *BitMargin;

  MaxLeft = -1 * 0xFFF;
  MinRight = 0xFFF;

  for (Lane = 0; Lane < MAX_BITS; Lane++) {
    for (Knob = 0; Knob < CommonParameters->KnobsPerGroup; Knob++) {
      BitMargin = GetResultPtrAtRawBitLevel (CommonParameters, Knob, Channel, Lane, Results);

      if (BitMargin->n > MaxLeft) {
        MaxLeft = BitMargin->n;
      }

      if ( BitMargin->p < MinRight) {
        MinRight = BitMargin->p;
      }
    } // Knob
  } //Lane

  *Left = MaxLeft;
  *Right = MinRight;
}

/**
  This function to get the range of the channel margins of the Inner 1D Sweep
  They are the maximum margins of both sides.

  @param[in]  CommonParamaters     Pointer to the context information
  @param[in]  Results              Pointer to 1D sweep margin result
  @param[in]  Channel              Channel index in socket (0 based)
  @param[out] Left                 Ponter to "Left"/"Low"/"Negative" side limit
  @param[out] Right                Ponter to "Right"/"High"/"Positive" side limit

**/

STATIC
VOID
GetInner1DSweepMarginRange (
  IN    COMMON_1D_PARAMETERS   *CommonParameters,
  IN    RESULTS_1D             *Results,
  IN    UINT8                  Channel,
  OUT   INT16                  *Left,
  OUT   INT16                  *Right
  )
{
  UINT8 Lane;
  UINT8 Knob;
  INT16 MinLeft;
  INT16 MaxRight;
  struct baseMargin *BitMargin;

  MinLeft = 0xFFF;
  MaxRight = -1 * 0xFFF;

  for (Lane = 0; Lane < MAX_BITS; Lane++) {
    for (Knob = 0; Knob < CommonParameters->KnobsPerGroup; Knob++) {
      BitMargin = GetResultPtrAtRawBitLevel (CommonParameters, Knob, Channel, Lane, Results);

      if (BitMargin->n < MinLeft) {
        MinLeft = BitMargin->n;
      }

      if (BitMargin->p > MaxRight) {
        MaxRight = BitMargin->p;
      }
    } // Knob
  } //Lane

  *Left = MinLeft;
  *Right = MaxRight;
}

/**
  This function returns if a given offset outside the 1D sweep eye of a specified bit and channel.

  @param[in]  CommonParamaters     Pointer to the context information
  @param[in]  Results              Pointer to 1D sweep margin result
  @param[in]  Channel              Channel index in socket (0 based)
  @param[in]  Bit                  Bit index range from 0 to MAX_BITS -1
  @param[in]  Offset               Delay offset to be checked

  @retval TRUE: Outside 1D sweep eye. False: Inside 1D sweep eye.

**/

STATIC
BOOLEAN
IsOutside1DSweepEye (
  IN    COMMON_1D_PARAMETERS   *CommonParameters,
  IN    RESULTS_1D             *Results,
  IN    UINT8                   Channel,
  IN    UINT8                   Bit,
  IN    INT16                   Offset
  )
{
  UINT8 Knob;
  struct baseMargin *BitMargin;

  for (Knob = 0; Knob < CommonParameters->KnobsPerGroup; Knob++) {
    BitMargin = GetResultPtrAtRawBitLevel (CommonParameters, Knob, Channel, Bit, Results);

    if ((Offset < BitMargin->n) || (Offset > BitMargin->p)) {
      return TRUE;
    }
  } // Knob

  return FALSE;
}


/**
  This function is used to update the outer 1D Sweep condition.
  It forces all the strobe to the same state.

  @param[in]  CommonParamaters  Pointer to the context information
  @param[in]  ResultCondition   Result (PASS_CONDITION or FAIL_CONDITIONN) to be set

**/
STATIC
VOID
UpdateOuterCondition (
  IN OUT COMMON_1D_PARAMETERS    *CommonParameters,
  IN     RESULTS_1D_DEFINITION    ResultCondition
  )
{
  UINT8   ChannelIndex;
  UINT8   SubChannelIndex;
  UINT8   MaxChDdr;
  UINT8   StrobeLogical;
  UINT8   StrobeLinear;
  UINT8   BitIndex;
  UINT8   KnobIndex;
  RESULTS_1D_CONDITION  *ResultConditionPtr;

  MaxChDdr = CommonParameters->MaxChDdr;

  for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {

    if (!CommonParameters->ChannelEnabledLocal[ChannelIndex]) {
      continue;
    }

    if (((1 << ChannelIndex) & CommonParameters->ChannelMask) != 0) {
      continue;
    }

    for (KnobIndex = 0; KnobIndex < CommonParameters->KnobsPerGroup; KnobIndex++) {
      for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
        for (StrobeLinear = 0; StrobeLinear < CommonParameters->MaxStrobeAvailable; StrobeLinear++) {

          StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);

          for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {

            ResultConditionPtr = GetResultConditionPtr (CommonParameters, KnobIndex, ChannelIndex, StrobeLogical, BitIndex);

            ResultConditionPtr->Current = ResultCondition;

          } // BitIndex loop ...
        } // StrobeLinear loop ...
      } //SubChanelIndex
    } // KnobIndex loop ...
  } // ChannelIndex loop ...
}


/*

  Function to get the next result element entry

  @param[in ]  SsaServicesHandle  Pointer to SSA services.
  @param[in ]  pCoreTestData      Pointer to Margin2D core test data.
  @param[out]  pElement           Pointer to the next result element location

  @retval   MRC_STATUS_SUCCESS      Function executed properly

*/
STATIC
MRC_STATUS
GetResultElement (
  IN SSA_BIOS_SERVICES_PPI    *SsaServicesHandle,
  IN MARGIN2D_CORE_DATA       *pCoreTestData,
  OUT MARGIN2D_RESULT_COLUMNS **pElement
)
{
  SSA_STATUS RetStat;
  UINT32    *MetaDataSize;

  RetStat = SsaServicesHandle->SsaResultsConfig->GetNextResultElement (SsaServicesHandle, pElement);

  if (RetStat != Success) {
    RcDebugPrint (SDBG_DEFAULT, "Increase result buffer by 32 ... \n");

    // Increase the buffer
    if (SsaServicesHandle->SsaResultsConfig->IncreaseResultBuffer (SsaServicesHandle, 32) != Success) {
      RcDebugPrint (SDBG_DEFAULT, "Failed to increase the result buffer, ran out of memory space.\n");
      RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    }

    //
    // Update core test data
    //
    SsaServicesHandle->SsaResultsConfig->GetMetadata (SsaServicesHandle, &pCoreTestData->pMetadata);

    pCoreTestData->pTestStat = (TEST_STATUS*)&pCoreTestData->pMetadata->StatusCode;

    SsaServicesHandle->SsaResultsConfig->GetMetadataSize (SsaServicesHandle, &MetaDataSize);

    pCoreTestData->pResultElements = (MARGIN2D_RESULT_COLUMNS *)((UINTN)pCoreTestData->pMetadata + *MetaDataSize);

    SsaServicesHandle->SsaResultsConfig->GetNextResultElement (SsaServicesHandle, pElement);
  }

  if (pCoreTestData->ResultElementCount == 0) {
    pCoreTestData->pResultElements = *pElement;
  }

  pCoreTestData->ResultElementCount++;

  return MRC_STATUS_SUCCESS;
}

/**
  This function to reconstruct the lane error status from inner 1D sweep result and save it to result buffer

  @param[in]      SsaServicesHandle  Pointer to SSA services.
  @param[in]      CoreTestData       Pointer to Margin2D core test data.
  @param[in]      CommonParamaters   Pointer to the inner 1D sweep context information
  @param[in]      Results            Pointer to 1D sweep margin result
  @param[in]      OuterOffset        Current offset value of the outer loop margin group.

**/
STATIC
VOID
Save1DResult (
  IN OUT  SSA_BIOS_SERVICES_PPI  *SsaServicesHandle,
  IN      MARGIN2D_CORE_DATA     *CoreTestData,
  IN      COMMON_1D_PARAMETERS   *CommonParameters,
  IN      RESULTS_1D             *Results,
  IN      INT16                   OuterOffset
  )
{
  UINT8                Channel;
  UINT8                MaxChDdr;
  UINT8                Bit;
  UINT8                BitErrorStatus[MAX_BITS / 8];
  BOOLEAN              IsAnyBitFail;
  INT16                Offset;
  INT16                MinLeft;
  INT16                MaxRight;
  MARGIN2D_RESULT_COLUMNS *pElement;

  MaxChDdr = CommonParameters->MaxChDdr;

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (((1 << Channel) & CommonParameters->ChannelMask) != 0) {
      continue;
    }

    //
    // Get the inner 1D sweep margin range
    //
    GetInner1DSweepMarginRange (CommonParameters, Results, Channel, &MinLeft, &MaxRight);

    for (Offset = MinLeft; Offset <= MaxRight; Offset += CommonParameters->StepSize) {
      IsAnyBitFail = FALSE;

      //
      // Reconstruct the lane error status from the test result condition
      //
      ZeroMem (BitErrorStatus, sizeof (BitErrorStatus));
      for (Bit = 0; Bit < MAX_BITS; Bit++) {
        if (IsOutside1DSweepEye (CommonParameters, Results, Channel, Bit, Offset)) {
          IsAnyBitFail = TRUE;
          BitErrorStatus[Bit / 8] |= 0x1 << (Bit % 8);
        }
      } // Bit

      if (IsAnyBitFail == FALSE) {
        continue;
      }

      GetResultElement (SsaServicesHandle, CoreTestData, &pElement);

      //
      // Save to result buffer
      //
      pElement->FastOffset = Offset;
      pElement->SlowOffset = OuterOffset;
      CopyMem (pElement->LaneErrorStatus, BitErrorStatus, (MAX_BITS / 8));
    } // Offset
  } // Channel

}
/*

  Callback function to start and process inner 1D sweep test result.

  @param[in]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
Margin2DResultHandler (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  MARGIN_2D_PARAMETER  *Margin2DSweep;
  COMMON_1D_PARAMETERS *InnerCommonParameters;
  MARGIN2D_CORE_DATA   *CoreTestData;
  PSYSHOST             Host;
  MRC_GT               Group;
  UINT8                Socket;
  UINT8                Channel;
  UINT8                MaxChDdr;
  INT16                Offset;
  UINT32               PrevDebugLevel;
  RESULTS_1D           Results;
  Callback1D           ResultProcessor;
  Callback1D           ErrorHandler;
  MRC_LT               MarginGroup;
  RESULTS_1D_DEFINITION    ResultCondition;

  Margin2DSweep = (MARGIN_2D_PARAMETER *)CommonParameters;
  InnerCommonParameters = &Margin2DSweep->InnerCommonParameters;
  CoreTestData = Margin2DSweep->CoreTestData;

  MaxChDdr = CommonParameters->MaxChDdr;
  Host = CommonParameters->Host;
  Socket = CommonParameters->Socket;
  Group = CommonParameters->Group;

  //
  // Margin2D only targets one channel.
  //
  Channel = CoreTestData->CoreTestCfg.Controller * MAX_MC_CH + CoreTestData->CoreTestCfg.Channel;

  //
  // Get the current "slow"/"outer" offset
  //
  Offset = 0;
  GetChannelDelayOffset (CommonParameters, Channel, &Offset);
  Margin2DSweep->OuterOffset = Offset;

  RcDebugPrint (SDBG_DEFAULT, "Outer ResultHandler Channel:%d offset:%d \n", Channel, Offset);

  //
  // Perform inner 1D sweep
  //
  InitializeInner1DSweepParameter (CommonParameters);

  AllocatePoolForResults1D (InnerCommonParameters, &Results);

  //
  // Enable Inner 1D sweep debug log
  //
  PrevDebugLevel = GetDebugLevel ();
  (InnerCommonParameters)->DebugLevel |= SDBG_SWEEP_L0;
  SetDebugLevel (SDBG_SWEEP_L0 | SDBG_DEFAULT);

  //
  // Update Inner 1D callback functions based on inner margin group
  //
  ResultProcessor = ResultProcessingAdvance;
  ErrorHandler = NULL;

  MarginGroup = InnerCommonParameters->Group;

  if (CheckCMDGroup (MarginGroup) || CheckCTLGroup (MarginGroup)) {
    ErrorHandler = ErrorHandlerAdvanceCmdCtl;
  } else if (MarginGroup == TxVref) {
#ifndef DDR5_SUPPORT
    ErrorHandler = ErrorHandlerAdvanceTxVref;
#endif
  }
  //RcDebugPrint (SDBG_DEFAULT, "Inner 1D sweepp margin group:%d  \n", MarginGroup);
  RcDebugPrint (SDBG_DEFAULT, "Inner 1D sweepp start... \n");

  ProcessSweep1D (InnerCommonParameters, NULL, Inner1DSetupTest, Inner1DExecuteTest, ErrorHandler, NULL, ResultProcessor, &Results);

  //
  // Reconstruct per bit lane error status from 1D sweep and save it to the result buffer
  //
  Save1DResult (Margin2DSweep->SsaServicesHandle, CoreTestData, InnerCommonParameters, &Results, Margin2DSweep->OuterOffset);

  //
  // Save current margins as the next start offsets.
  //
  GetInner1DSweepMargins (InnerCommonParameters, &Results, Channel, &Margin2DSweep->InnerLeft, &Margin2DSweep->InnerRight);

  //
  // Restore debug level
  //
  SetDebugLevel (PrevDebugLevel);

  //
  // Relese 1D sweep result
  //
  ReleaseResources1D (InnerCommonParameters, &Results);

  //
  // Update test condition of the outer 1D sweep.
  // If the inner margin eye is open, set ALL strobe test condition to PASS, so that outer 1D sweep continues its search.
  // If the inner margin eye is close, set ALL strobe test condition to FAIL, so that outer 1D sweep stops its search.
  //
  if ((Margin2DSweep->InnerRight - Margin2DSweep->InnerLeft) > 1) {
    ResultCondition = PASS_CONDITION;
  } else {
    ResultCondition = FAIL_CONDITION;
  }

  UpdateOuterCondition (CommonParameters, ResultCondition);

  return MRC_STATUS_SUCCESS;
}

/*

  Callback function to set up inner 1D sweep CPGC test

  @param[in]  CommonParamaters  Pointer to the Margin2D context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
Inner1DSetupTest (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  MARGIN_2D_INNER_PARAMETER *Margin2DInnerSweep;
  COMMON_1D_PARAMETERS    *InnerCommonParameters;
  MARGIN2D_CORE_DATA      *CoreTestData;
  CPGC_POINT_TEST_CONFIG  *CpgcPointTestCfg;
  MRC_RT                  MrcRankList;
  UINT8                   MemSsType;
  UINT32                  ChBitMask;
  UINT8                   Socket;
  UINT8                   Controller;
  UINT8                   Channel;
  UINT8                   ChannelInSocket;
  PSYSHOST                Host;
  MRC_RT                  DimmRank[MAX_CH];

  Margin2DInnerSweep = (MARGIN_2D_INNER_PARAMETER *)CommonParameters;
  InnerCommonParameters = &Margin2DInnerSweep->InnerCommonParameters;
  CoreTestData = Margin2DInnerSweep->CoreTestData;
  CpgcPointTestCfg = &CoreTestData->CpgcPointTestCfg;
  Socket = InnerCommonParameters->Socket;
  Host = InnerCommonParameters->Host;

  //
  // Update the error checker configuration fields of the CPGC_POINT_TEST_CONFIG structre.
  // These fields were added lately and they were part of the  MEM_POINT_TEST_CONFIG.
  //
  CopyMem (CpgcPointTestCfg->LaneValBitmasks, CoreTestData->MemPointTestCfg.LaneValBitmasks, sizeof (CpgcPointTestCfg->LaneValBitmasks));
  CopyMem (CpgcPointTestCfg->ChunkValBitmasks, CoreTestData->MemPointTestCfg.ChunkValBitmasks, sizeof (CpgcPointTestCfg->ChunkValBitmasks));

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

  //
  // Get the list of DIMM/RANK
  //
  GetMargin2DTargetedDimmRank (Margin2DInnerSweep->SsaServicesHandle, CoreTestData, Socket, &DimmRank);

  for (Controller = 0; Controller < MAX_IMC; Controller++) {
    for (Channel = 0; Channel < MAX_MC_CH; Channel++) {
      if (!IsChannelTestingEnabled (Margin2DInnerSweep->SsaServicesHandle, &CoreTestData->SystemInfo, &CoreTestData->MemCfg,
        &CoreTestData->MemPointTestCfg, Socket, Controller, Channel)) {
        continue;
      }

      ChannelInSocket = Controller * MAX_MC_CH + Channel;

      // TODO Turnaround test case are not supported!
      MrcRankList.Dimm = DimmRank[ChannelInSocket].Dimm;
      MrcRankList.Rank = DimmRank[ChannelInSocket].Rank;

      ChBitMask = 0x1 << ChannelInSocket;

      //
      // Set up the test sequence and address scheme
      //
      SetupBssaCpgcTest (Host, Socket, MemSsType, ChBitMask, 1, &MrcRankList, (CPGC_CONFIGURE_HEADER*)CpgcPointTestCfg, TRUE, TRUE);

    } // Channel
  } // Controller

  return MRC_STATUS_SUCCESS;
}

/*

  Callback function to execute inner  1D sweep CPGC test

  @param[in]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
Inner1DExecuteTest (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  MARGIN_2D_INNER_PARAMETER    *Margin2DInnerSweep;
  COMMON_1D_PARAMETERS   *InnerCommonParameters;
  MARGIN2D_CORE_DATA     *CoreTestData;
  UINT8                   MemSsType;
  MRC_TT                  TestType;
  UINT32                  ChBitMask;
  UINT8                   Socket;
  UINT8                   Controller;
  UINT8                   Channel;
  PSYSHOST                Host;

  Margin2DInnerSweep = (MARGIN_2D_INNER_PARAMETER *)CommonParameters;
  InnerCommonParameters = &Margin2DInnerSweep->InnerCommonParameters;
  CoreTestData = Margin2DInnerSweep->CoreTestData;
  Socket = InnerCommonParameters->Socket;
  Host = InnerCommonParameters->Host;
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  TestType = RmtDqTest;
  ChBitMask = 0;

  for (Controller = 0; Controller < MAX_IMC; Controller++) {
    for (Channel = 0; Channel < MAX_MC_CH; Channel++) {
      if (!IsChannelTestingEnabled (Margin2DInnerSweep->SsaServicesHandle, &CoreTestData->SystemInfo, &CoreTestData->MemCfg,
        &CoreTestData->MemPointTestCfg, Socket, Controller, Channel)) {
        continue;
      }

      ChBitMask |= 0x1 << (Controller * MAX_MC_CH + Channel);
    }// Channel
  } // Controller

  // Start test
  TestControl (Host, Socket, MemSsType, ChBitMask, NO_SUBCH, TestType, TRUE, START_TEST, NOT_STOP_TEST);

  return MRC_STATUS_SUCCESS;
}



