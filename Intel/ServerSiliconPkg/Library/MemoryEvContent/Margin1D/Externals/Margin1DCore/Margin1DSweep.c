/** @file
  Margin1DSweep.c

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

#include "Margin1DSweep.h"
#include <Library/MemoryCoreLib.h>
#include <Library/MemoryServicesLib.h>

#include <ReferenceCodeFatalErrors.h>

//
// Special value used for error count to indicate that an overflow occurred
//
#define CPGC_ERR_CNT_OVERFLOW_VAL 0xFFFFFFFF

/*

  Helper function to initial or clean up margin parameters setting before/after margin test
  It mostly perfroms the De/ReNoramlization.

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval         MRC_STATUS_SUCCESS

*/
MRC_STATUS
EFIAPI
Margin1DInitMarginParam (
  IN  COMMON_1D_PARAMETERS    *CommonParameters,
  IN  SETUP_CLEANUP            SetupCleanup
  )
{
  MARGIN_1D_PARAMETER *Margin1DSweep;
  MRC_LT               Level;
  MRC_GT               Group;
  UINT8                Socket;
  SSA_STATUS           RetStat;
  SHMOO_DIRECTION      ShmooDir = LoDirection;

  Margin1DSweep = (MARGIN_1D_PARAMETER *)CommonParameters;

  Socket = Margin1DSweep->CommonParameters.Socket;
  Level = Margin1DSweep->CommonParameters.Level;
  Group = Margin1DSweep->CommonParameters.Group;

  //
  // Get the margin direction. The "SetupCleanup" parameter of the InitMarginParam has the margin
  // direction encoded in it.
  //
  if (CommonParameters->FlagsFsm & LeftSideOnly) {
    ShmooDir = LoDirection;
  } else if (CommonParameters->FlagsFsm & RightSideOnly) {
    ShmooDir = HiDirection;
  }

  RetStat = Margin1DSweep->SsaServicesHandle->SsaMemoryConfig->InitMarginParam (
    Margin1DSweep->SsaServicesHandle, Socket, Level, Group, (SetupCleanup | (ShmooDir << 1)));

  if (ChkRetStat (Margin1DSweep->SsaServicesHandle, RetStat, __FILE__, __LINE__, Margin1DSweep->CoreTestData->pTestStat)) {
    return MRC_STATUS_FAILURE;
  }

  return MRC_STATUS_SUCCESS;
}
/*

  Callback function to be executed before any sweep is started

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
Margin1DPre1DSweep (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  return Margin1DInitMarginParam (CommonParameters, Setup);
}

/*

  Callback function to be executed after any sweep is done

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
Margin1DPost1DSweep (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  return Margin1DInitMarginParam (CommonParameters, Cleanup);
}

/*

  Callback function to set up CPGC test

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
Margin1DSetupTest (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  MARGIN_1D_PARAMETER     *Margin1DSweep;
  CORE_TEST_DATA          *CoreTestData;
  CPGC_POINT_TEST_CONFIG  *CpgcPointTestCfg;
  MRC_RT                  MrcRankList;
  MRC_MST                 MemSsType;
  UINT32                  ChBitMask;
  UINT8                   Socket;
  UINT8                   Controller;
  UINT8                   Channel;
  UINT8                   ChannelInSocket;
  PSYSHOST                Host;
  MRC_RT                  DimmRank[MAX_CH];

  Margin1DSweep = (MARGIN_1D_PARAMETER *)CommonParameters;
  CoreTestData = Margin1DSweep->CoreTestData;
  CpgcPointTestCfg = &CoreTestData->CpgcPointTestCfg;
  Socket = Margin1DSweep->CommonParameters.Socket;
  Host = Margin1DSweep->CommonParameters.Host;

  //
  // Update the error checker configuration fields of the CPGC_POINT_TEST_CONFIG from MemPointTestCfg
  //
  CopyMem (CpgcPointTestCfg->LaneValBitmasks, CoreTestData->SelectMemPointTestCfg.LaneValBitmasks, sizeof (CpgcPointTestCfg->LaneValBitmasks));
  CopyMem (CpgcPointTestCfg->ChunkValBitmasks, CoreTestData->SelectMemPointTestCfg.ChunkValBitmasks, sizeof (CpgcPointTestCfg->ChunkValBitmasks));

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

  //
  // Get the list of DIMM/RANK
  //
  GetTargetedDimmRank (Margin1DSweep->SsaServicesHandle, CoreTestData, Socket, &DimmRank);

  for (Controller = 0; Controller < MAX_IMC; Controller++) {
    for (Channel = 0; Channel < MAX_MC_CH; Channel++) {
      if (!IsChannelTestingEnabled (Margin1DSweep->SsaServicesHandle, &CoreTestData->SystemInfo, &CoreTestData->MemCfg,
        &CoreTestData->SelectMemPointTestCfg, Socket, Controller, Channel)) {
        continue;
      }

      ChannelInSocket = Controller * MAX_MC_CH + Channel;

      // TODO Turnaround test case are not supported!
      MrcRankList.Dimm = DimmRank[ChannelInSocket].Dimm;
      MrcRankList.Rank = DimmRank[ChannelInSocket].Rank;

      ChBitMask = 0x1 << ChannelInSocket;

      //
      // Set up the test sequence and address
      //
      SetupBssaCpgcTest (Host, Socket, MemSsType, ChBitMask, 1, &MrcRankList, (CPGC_CONFIGURE_HEADER*)CpgcPointTestCfg, TRUE, TRUE);

   } // Channel
  } // Controller

  return MRC_STATUS_SUCCESS;
}

/*

  Callback function to start CPGC test

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
Margin1DExecuteTest (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  MARGIN_1D_PARAMETER    *Margin1DSweep;
  CORE_TEST_DATA         *CoreTestData;
  MRC_MST                 MemSsType;
  MRC_TT                  TestType;
  UINT32                  ChBitMask;
  UINT8                   Socket;
  UINT8                   Controller;
  UINT8                   Channel;
  PSYSHOST                Host;

  Margin1DSweep = (MARGIN_1D_PARAMETER *)CommonParameters;
  CoreTestData = Margin1DSweep->CoreTestData;
  Socket = CommonParameters->Socket;
  Host = CommonParameters->Host;
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  TestType = RmtDqTest;
  ChBitMask = 0;

  for (Controller = 0; Controller < MAX_IMC; Controller++) {
    for (Channel = 0; Channel < MAX_MC_CH; Channel++) {
      if (!IsChannelTestingEnabled (Margin1DSweep->SsaServicesHandle, &CoreTestData->SystemInfo, &CoreTestData->MemCfg,
        &CoreTestData->SelectMemPointTestCfg, Socket, Controller, Channel)) {
        continue;
      }

      ChBitMask |= 0x1 << (Controller * MAX_MC_CH + Channel);
    }// Channel
  } // Controller

  //
  // Start test
  //
  TestControl (Host, Socket, MemSsType, ChBitMask, NO_SUBCH, TestType, TRUE, START_TEST, NOT_STOP_TEST);

  return MRC_STATUS_SUCCESS;
}


/*

  Helper function to get next result element entry

  @param[in ]  SsaServicesHandle  Pointer to SSA services.
  @param[in ]  pCoreTestData      Pointer to Margin1D core test data.
  @param[out]  pElement           Pointer to the next result element location

  @retval   MRC_STATUS_SUCCESS

*/
STATIC
MRC_STATUS
GetResultElement (
  IN SSA_BIOS_SERVICES_PPI      *SsaServicesHandle,
  IN CORE_TEST_DATA             *pCoreTestData,
  OUT MARGIN_1D_RESULT_COLUMNS  **pElement
  )
{
  SSA_STATUS RetStat;

  RetStat = SsaServicesHandle->SsaResultsConfig->GetNextResultElement (SsaServicesHandle, pElement);

  //
  // Increase result buffer because the existing result buffer running out of space
  //
  if (RetStat != Success) {

    RcDebugPrint (SDBG_DEFAULT, "Increase result buffer by 32 ... \n");

    if (SsaServicesHandle->SsaResultsConfig->IncreaseResultBuffer (SsaServicesHandle, 32) != Success) {
      RcDebugPrint (SDBG_DEFAULT, "Failed to increase the result buffer, ran out of memory space.\n");
      RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    }

    //
    // Update the result buffer related fields of the core test data
    //
    SsaServicesHandle->SsaResultsConfig->GetMetadata (SsaServicesHandle, &pCoreTestData->pMetadata);

    pCoreTestData->pTestStat = (TEST_STATUS*)&pCoreTestData->pMetadata->StatusCode;

    //
    // Get the next element entry
    //
    SsaServicesHandle->SsaResultsConfig->GetNextResultElement (SsaServicesHandle, pElement);
  }

  return MRC_STATUS_SUCCESS;
}

/*

  Callback function to interpret the CPGC test result

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
Margin1DResultHandler (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  MARGIN_1D_PARAMETER *Margin1DSweep;
  PSYSHOST             Host;
  MRC_GT               Group;
  UINT8                Socket;
  UINT8                Channel;
  UINT8                MaxChDdr;
  UINT8                Knob;
  UINT8                Bit;
  UINT8                BitErrorStatus[MAX_BITS / 8];
  BOOLEAN              IsAnyBitFail;
  INT16                Offset;
  MRC_MST              MemSsType;
  MRC_TT               TestType;
  RESULTS_1D_CONDITION *Condition;
  MARGIN_1D_RESULT_COLUMNS *pElement;
  CPGC_ERROR_COUNT_STATUS   CpgcErrorCounterStatus;

  Margin1DSweep = (MARGIN_1D_PARAMETER *)CommonParameters;
  MaxChDdr = CommonParameters->MaxChDdr;
  Host = CommonParameters->Host;
  Socket = CommonParameters->Socket;
  Group = CommonParameters->Group;

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  TestType = RmtDqTest;

  //
  // First call the "standard/common" callback function to get CPGC test results and update
  // the 1D sweep result condition.
  //
  ResultProcessingAdvance (CommonParameters);

  //
  // Save failing lane as bit mask format, associated delay offset and error count to SSA result. 
  //
  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (((1 << Channel) & CommonParameters->ChannelMask) != 0) {
      continue;
    }
    IsAnyBitFail = FALSE;
    ZeroMem (BitErrorStatus, sizeof (BitErrorStatus));

    for (Bit = 0; Bit < MAX_BITS; Bit++) {

      for (Knob = 0; Knob < CommonParameters->KnobsPerGroup; Knob++) {
        Condition = GetResultConditionPtrAtRawBitLevel (CommonParameters, Knob, Channel, Bit);
        if (Condition->Current == FAIL_CONDITION) {
          IsAnyBitFail = TRUE;
          BitErrorStatus[Bit / 8] |= 0x1 << (Bit % 8);
        }
      } // Knob
    } // Bit

    // If detected any CPGC bit error, save to result buffer.
    if (IsAnyBitFail == FALSE) {
      continue;
    }

    //
    // Get the current offset
    //
    Offset = 0;
    GetChannelDelayOffset (CommonParameters, Channel, &Offset);
    GetResultElement (Margin1DSweep->SsaServicesHandle, Margin1DSweep->CoreTestData, &pElement);

    //
    // Get the channel error count which was in the error counter 0
    //
    GetErrorCount (Host, Socket, MemSsType, Channel, TestType, 0, &CpgcErrorCounterStatus);

    pElement->Socket = Socket;
    pElement->Controller = Channel / MAX_MC_CH;
    pElement->Channel = Channel % MAX_MC_CH;
    pElement->DimmA = CommonParameters->Dimm[Channel];
    pElement->RankA = CommonParameters->Rank[Channel];
    pElement->DimmB = 0;
    pElement->RankB = 0;
    pElement->RankCombinationIndex = Margin1DSweep->CoreTestData->CurrentRankCombinationIndex;
    pElement->Offset = Offset;

    ZeroMem (pElement->LaneErrorStatus, sizeof (pElement->LaneErrorStatus));
    CopyMem (pElement->LaneErrorStatus, BitErrorStatus, (MAX_BITS / 8));

    if (CpgcErrorCounterStatus.Overflow) {
      pElement->ErrorCount = CPGC_ERR_CNT_OVERFLOW_VAL;
    } else {
      pElement->ErrorCount = CpgcErrorCounterStatus.Count;
    }

  } // Channel

  return MRC_STATUS_SUCCESS;
}

/**
  This function is used to save the Margin1D test result with 1D sweep in CA training mode.

  It repurposes the CPGC lane error status to store the DCA per signal margin result. The bits[7:0] are mapped
  to the A0A---A6A and PARA of Sub-Channel A, while the bits[47:40] are mapped to those signals of sub-Channel B.

  @param[in]       SsaServicesHandle    Pointer to SSA services.
  @param[in, out]  pCoreTestData        Pointer to RMT core test data.
  @param[in]       ShmooDir             Margin direction.
  @param[in]       Socket               Processor socket within the system (0-based)
  @param[in]       ChannelMask          One-hot encoding to mask a channel (0: executes if enabled/populated, 1: skip/masked)
  @param[in]       DimmRank             Structure to specify current dimm and rank under test (0-based)
  @param[in]       SweepResults         Pointer to margin results data structure which is indexed as [MAX_CH][SUB_CH][Signal]

  @retval  N/A
**/
VOID
SaveMargin1DResultInCaTrainingMode (
  IN     SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT CORE_TEST_DATA        *pCoreTestData,
  IN     SHMOO_DIRECTION        ShmooDir,
  IN     UINT8                  Socket,
  IN     UINT32                 ChannelBitMask,
  IN     MRC_RT                 DimmRank[MAX_CH],
  IN     struct baseMargin      (*SweepResults)[MAX_CH][SUB_CH][RCD_CA_SIGNALS_TO_SWEEP]
  )
{
  UINT8                    MaxChDdr;
  UINT8                    Channel;
  UINT8                    SubChannel;
  UINT8                    Bit;
  UINT8                    BitErrorStatus[MAX_BITS / 8];
  INT16                    MinMargin;
  INT16                    MaxMargin;
  INT16                    Margin;
  BOOLEAN                  FailAtThisMarginOffset;
  BOOLEAN                  IsAnyBitFailInThisChannel;
  MARGIN_1D_RESULT_COLUMNS *pElement;

  MaxChDdr = GetMaxChDdr();

  //
  // Find the min/max margin of the sweep results.
  //
  MinMargin = MAX_INT16;
  MaxMargin = MIN_INT16;
  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (((1 << Channel) & ChannelBitMask) != 0) {
      continue;
    }

    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      for (Bit = 0; Bit < RCD_CA_SIGNALS_TO_SWEEP; Bit++) {
        if (ShmooDir == LoDirection) {
          Margin = (*SweepResults)[Channel][SubChannel][Bit].n;
        } else {
          Margin = (*SweepResults)[Channel][SubChannel][Bit].p;
        }
        if (Margin > MaxMargin) {
          MaxMargin = Margin;
        }
        if (Margin < MinMargin) {
          MinMargin = Margin;
        }
      } // Bit
    } // SubChannel
  } // Channel

  //
  // Because the sweep results are the last pass offset, adjust the outer bounday so that the last
  // fail can be covered.
  //
  if (ShmooDir == LoDirection) {
    MinMargin -= 1;
  } else {
    MaxMargin += 1;
  }

  //
  // Save failing lane as bit mask format. Construct the bit mask by iterating the margin offset
  // from min to max and set the bit to 1 if its margin is bigger or less the offset value based
  // on the margin direction.
  //
  for (Margin = MinMargin; Margin <= MaxMargin; Margin++) {
    for (Channel = 0; Channel < MaxChDdr; Channel++) {
      if (((1 << Channel) & ChannelBitMask) != 0) {
        continue;
      }

      ZeroMem (BitErrorStatus, sizeof (BitErrorStatus));
      IsAnyBitFailInThisChannel = FALSE;
      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        for (Bit = 0; Bit < RCD_CA_SIGNALS_TO_SWEEP; Bit++) {
          FailAtThisMarginOffset = FALSE;
          if (ShmooDir == LoDirection) {
            if ((*SweepResults)[Channel][SubChannel][Bit].n > Margin) {
              FailAtThisMarginOffset = TRUE;
            }
          } else {
            if ((*SweepResults)[Channel][SubChannel][Bit].p < Margin) {
              FailAtThisMarginOffset = TRUE;
            }
          }
          if (FailAtThisMarginOffset) {
            IsAnyBitFailInThisChannel = TRUE;
            BitErrorStatus[(Bit + SubChannel * (MAX_BITS / 2))  / 8] |= 0x1 << ((Bit + SubChannel * (MAX_BITS / 2)) % 8);
          }
        } // Bit
      } // SubChannel

      if (IsAnyBitFailInThisChannel == FALSE) {
        continue;
      }

      GetResultElement (SsaServicesHandle, pCoreTestData, &pElement);

      pElement->Socket = Socket;
      pElement->Controller = Channel / MAX_MC_CH;
      pElement->Channel = Channel % MAX_MC_CH;
      pElement->DimmA = DimmRank[Channel].Dimm;
      pElement->RankA = DimmRank[Channel].Rank;
      pElement->DimmB = 0;
      pElement->RankB = 0;
      pElement->RankCombinationIndex = pCoreTestData->CurrentRankCombinationIndex;
      pElement->Offset = Margin;

      ZeroMem (pElement->LaneErrorStatus, sizeof (pElement->LaneErrorStatus));
      CopyMem (pElement->LaneErrorStatus, BitErrorStatus, (MAX_BITS / 8));

      //
      // We don't track error count in CA traing mode. Set it to 1 to indicate there was error.
      //
      pElement->ErrorCount = 1;

    } // Channel
  } // Margin
}

/**
  This function is used to save the Margin1D test result with 1D sweep in CS training mode.

  It repurposes the CPGC lane error status to store the DCA per signal margin result. The bit0 is mapped
  to the CS0 of Sub-Channel A, while the bit40 is mapped to CS0 of sub-Channel B.

  @param[in]       SsaServicesHandle    Pointer to SSA services.
  @param[in, out]  pCoreTestData        Pointer to RMT core test data.
  @param[in]       ShmooDir             Margin direction.
  @param[in]       Socket               Processor socket within the system (0-based)
  @param[in]       ChannelMask          One-hot encoding to mask a channel (0: executes if enabled/populated, 1: skip/masked)
  @param[in]       DimmRank             Structure to specify current dimm and rank under test (0-based)
  @param[in]       SweepResults         Pointer to margin results data structure which is indexed as [MAX_CH][SUB_CH]

  @retval  N/A
**/
VOID
SaveMargin1DResultInCsTrainingMode (
  IN     SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT CORE_TEST_DATA        *pCoreTestData,
  IN     SHMOO_DIRECTION        ShmooDir,
  IN     UINT8                  Socket,
  IN     UINT32                 ChannelBitMask,
  IN     MRC_RT                 DimmRank[MAX_CH],
  IN     struct baseMargin      (*SweepResults)[MAX_CH][SUB_CH]
  )
{
  UINT8                    MaxChDdr;
  UINT8                    Channel;
  UINT8                    SubChannel;
  UINT8                    BitErrorStatus[MAX_BITS / 8];
  INT16                    MinMargin;
  INT16                    MaxMargin;
  INT16                    Margin;
  BOOLEAN                  FailAtThisMarginOffset;
  BOOLEAN                  IsAnyBitFailInThisChannel;
  MARGIN_1D_RESULT_COLUMNS *pElement;

  MaxChDdr = GetMaxChDdr();

  //
  // Find the min/max margin of the sweep results.
  //
  MinMargin = MAX_INT16;
  MaxMargin = MIN_INT16;
  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (((1 << Channel) & ChannelBitMask) != 0) {
      continue;
    }

    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      if (ShmooDir == LoDirection) {
        Margin = (*SweepResults)[Channel][SubChannel].n;
      } else {
        Margin = (*SweepResults)[Channel][SubChannel].p;
      }
      if (Margin > MaxMargin) {
        MaxMargin = Margin;
      }
      if (Margin < MinMargin) {
        MinMargin = Margin;
      }
    } // SubChannel
  } // Channel

  //
  // Because the sweep results are the last pass offset, adjust the outer bounday so that the last
  // fail can be covered.
  //
  if (ShmooDir == LoDirection) {
    MinMargin -= 1;
  } else {
    MaxMargin += 1;
  }

  //
  // Save failing lane as bit mask format. Construct the bit mask by iterating the margin offset
  // from min to max and set the bit to 1 if its margin is bigger or less the offset value based
  // on the margin direction.
  //
  for (Margin = MinMargin; Margin <= MaxMargin; Margin++) {
    for (Channel = 0; Channel < MaxChDdr; Channel++) {
      if (((1 << Channel) & ChannelBitMask) != 0) {
        continue;
      }

      ZeroMem (BitErrorStatus, sizeof (BitErrorStatus));
      IsAnyBitFailInThisChannel = FALSE;
      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        FailAtThisMarginOffset = FALSE;
        if (ShmooDir == LoDirection) {
          if ((*SweepResults)[Channel][SubChannel].n > Margin) {
            FailAtThisMarginOffset = TRUE;
          }
        } else {
          if ((*SweepResults)[Channel][SubChannel].p < Margin) {
            FailAtThisMarginOffset = TRUE;
          }
        }
        if (FailAtThisMarginOffset) {
          IsAnyBitFailInThisChannel = TRUE;
          BitErrorStatus[(SubChannel * (MAX_BITS / 2))  / 8] |= 0x1 << ((SubChannel * (MAX_BITS / 2)) % 8);
        }
      } // SubChannel

      if (IsAnyBitFailInThisChannel == FALSE) {
        continue;
      }

      GetResultElement (SsaServicesHandle, pCoreTestData, &pElement);

      pElement->Socket = Socket;
      pElement->Controller = Channel / MAX_MC_CH;
      pElement->Channel = Channel % MAX_MC_CH;
      pElement->DimmA = DimmRank[Channel].Dimm;
      pElement->RankA = DimmRank[Channel].Rank;
      pElement->DimmB = 0;
      pElement->RankB = 0;
      pElement->RankCombinationIndex = pCoreTestData->CurrentRankCombinationIndex;
      pElement->Offset = Margin;

      ZeroMem (pElement->LaneErrorStatus, sizeof (pElement->LaneErrorStatus));
      CopyMem (pElement->LaneErrorStatus, BitErrorStatus, (MAX_BITS / 8));

      //
      // We don't track error count in CS traing mode. Set it to 1 to indicate there was error.
      //
      pElement->ErrorCount = 1;

    } // Channel
  } // Margin
}

