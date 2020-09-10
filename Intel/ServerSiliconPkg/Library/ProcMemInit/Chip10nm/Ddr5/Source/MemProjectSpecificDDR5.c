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

#include <Library/MemoryCoreLib.h>
#include <SysHostChip.h>
#include <Library/MemCpgcIpLib.h>
#include "Chip10nm/Include/MemHostChip10nm.h"
#include "Chip10nm/Ddr5/Include/MemProjectSpecificDdr5.h"
#include <UncoreCommonIncludes.h>
#include <Library/MemFmcIpLib.h>
#include <SysRegs.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/SystemInfoLib.h>
#include <ScratchpadList.h>
#include <Library/KtiApi.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/IoAccessLib.h>
#include <Library/Ddr5CoreLib.h>
#include <Library/MemRoutingLib.h>
#include <Library/MemRcLib.h>
#include <Library/SimulationServicesLib.h>
#include <Memory/JedecDefinitions.h>
#include <Memory/Ddr5RcwRegs.h>
#include <Library/SysHostPointerLib.h>

extern UINT8 DCLKs[MAX_SUP_FREQ];
extern UINT8 BCLKs[MAX_SUP_FREQ];
extern UINT8 bclkfreq[2];

#define DDR5_WL_COARSE_RCSIM_ARRAY_SIZE  30
#define DDR5_WL_FINE_RCSIM_ARRAY_SIZE    5

#define DDR5_WL_INTERNAL_COARSE_RCSIM_ARRAY_SIZE  61


UINT8
GetMinDdr5WlDqFeedback (
  IN  PSYSHOST Host,
  IN  UINT8    Socket
  )
{
    return 16;
}

/**
  Get or Set CA Vref Value for Error Result Feedback.

  On first access, this function initializes the whole table of CA Vref values
  per socket, channel, DIMM, and sub-channel to known initial values.

  @param[in]      Socket        CPU Socket Node number (Socket ID)
  @param[in]      Channel       Channel number
  @param[in]      Dimm          DIMM number
  @param[in]      SubChannel    Sub-channel number
  @param[in]      Signal        Enumerated signal value
  @param[in]      DoSet         TRUE to set the value; FALSE to get the value
  @param[in, out] CaVrefValue   Pointer to the CA Vref value
**/
VOID
GetSetCaVrefValueForFeedback (
  IN      UINT8     Socket,
  IN      UINT8     Channel,
  IN      UINT8     Dimm,
  IN      UINT8     SubChannel,
  IN      GSM_CSN   Signal,
  IN      BOOLEAN   DoSet,
  IN OUT  INT8      *CaVrefValue
  )
{
}

/**
  Prime CA Margins Sweep Error Result Feedback.

  @param[in]  Socket            - CPU Socket Node number (Socket ID)
  @param[in]  Channel           - Channel number
  @param[in]  Dimm              - DIMM number
  @param[in]  SubChannel        - Sub-channel number
  @param[in]  Signal            - Enumerated signal value
  @param[in]  CaVrefValue       - CA Vref value
**/
VOID
EFIAPI
PrimeCaMarginsSweepErrorResFeedback (
  IN  UINT8   Socket,
  IN  UINT8   Channel,
  IN  UINT8   Dimm,
  IN  UINT8   SubChannel,
  IN  GSM_CSN Signal,
  IN  INT8    CaVrefValue
  )
{
  GetSetCaVrefValueForFeedback (Socket, Channel, Dimm, SubChannel, Signal, TRUE, &CaVrefValue);
}

/**
  Get CA Margins Sweep Error Result Feedback when using DCA complex pattern.

  @param[in]  Socket            - CPU Socket Node number (Socket ID)
  @param[in]  Channel           - Channel number
  @param[in]  Dimm              - DIMM number
  @param[in]  SubChannel        - Sub-channel number
  @param[in]  Rank              - Rank number
  @param[in]  Signal            - Signal
  @param[in]  PiIndex           - PI index
  @param[in]  CurError          - Current error
  @param[in]  RetrainLoopCount  - Retrain loop count

  @retval error result of margin sweep
**/
UINT8
EFIAPI
GetCaMarginsSweepXtalkErrorResFeedback (
  IN  UINT8   Socket,
  IN  UINT8   Channel,
  IN  UINT8   Dimm,
  IN  UINT8   SubChannel,
  IN  UINT8   Rank,
  IN  GSM_CSN Signal,
  IN  INT16   PiIndex,
  IN  UINT8   CurError,
  IN  UINT8   RetrainLoopCount
  )
{
  UINT8                 ErrorResult;


  ErrorResult = 0;


  return ErrorResult;
}

/**
  Get CA Margins Sweep Error Result Feedback.

  @param[in]  Socket            - CPU Socket Node number (Socket ID)
  @param[in]  Channel           - Channel number
  @param[in]  Dimm              - DIMM number
  @param[in]  SubChannel        - Sub-channel number
  @param[in]  Rank              - Rank number
  @param[in]  Signal            - Signal
  @param[in]  PiIndex           - PI index
  @param[in]  CurError          - Current error
  @param[in]  RetrainLoopCount  - Retrain loop count
  @param[in]  PatternLoop       - Pattern Loop

  @retval error result of margin sweep
**/
UINT8
EFIAPI
GetCaMarginsSweepErrorResFeedback (
  IN  UINT8   Socket,
  IN  UINT8   Channel,
  IN  UINT8   Dimm,
  IN  UINT8   SubChannel,
  IN  UINT8   Rank,
  IN  GSM_CSN Signal,
  IN  INT16   PiIndex,
  IN  UINT8   CurError,
  IN  UINT8   RetrainLoopCount,
  IN  UINT8   PatternLoop
  )
{
  UINT8                 ErrorResult;


  ErrorResult = 0;


  return ErrorResult;
}

/**
  Get DCA DFE Margins Sweep Error Result Feedback.

  @param[in]  Socket            - CPU Socket Node number (Socket ID)
  @param[in]  Channel           - Channel number
  @param[in]  Dimm              - DIMM number
  @param[in]  SubChannel        - Sub-channel number
  @param[in]  Rank              - Rank number
  @param[in]  Signal            - Signal
  @param[in]  PiIndex           - PI index
  @param[in]  CurError          - Current error
  @param[in]  RetrainLoopCount  - Retrain loop count
  @param[in]  PatternLoop       - Pattern loop
  @param[in]  Loopnumber        - Loop number
  @param[in]  TapIndex          - Tap index

  @retval error result of margin sweep
**/
UINT8
EFIAPI
GetCaMarginsSweepErrorResFeedbackDcaDfe (
  IN  UINT8   Socket,
  IN  UINT8   Channel,
  IN  UINT8   Dimm,
  IN  UINT8   SubChannel,
  IN  UINT8   Rank,
  IN  GSM_CSN Signal,
  IN  INT16   PiIndex,
  IN  UINT8   CurError,
  IN  UINT8   RetrainLoopCount,
  IN  UINT8   PatternLoop,
  IN  UINT8   Loopnumber,
  IN UINT8   TapIndex
  )
{
  UINT8                 ErrorResult;


  ErrorResult = 0;


  return ErrorResult;
}
/**
Chip specific code path to get the starting value requested


@param step     - Current PI step
@param piStart  - PI start value

@retval (UINT16)Starting value

**/
UINT16
StartBackOverDdr5 (
  UINT8  step,
  UINT16 piStart
  )
{
  UINT16 piSetting;

  piSetting = CS_CLK_PI_START;//DDR5_TODO.Make it for WL fine sweep.

  return piSetting;
} // StartBackOver

/**

Chip specific code path to get the correct index into the training results

@param step      - training step
@param piSetting - Pi value
@param piStart   - pi start point
Strobe (0-based)
@retval (UINT8)index value

**/
UINT8
GetTrainingResultIndexDDR5 (
  UINT8  step,
  UINT16 piSetting,
  UINT16 piStart
  )
{
  UINT8 dWord = 0;
  if (step == PI_CS_CLK_TYPE) {
    dWord = (UINT8) ((piSetting - CS_CLK_PI_START) / 32);
  } else if (step == PI_WL_FINE_DDR5_TYPE) {
    dWord = (UINT8) ((piSetting - WR_LVL_FINE_PI_START_EVAL_DDR5) / 32);
  } else if (step == InternalWlFineSweepTest) {
    dWord = (UINT8) ((piSetting - WR_LVL_FINE_PI_START_EVAL_DDR5) / 32);
  }
  return dWord;
} // GetTrainingResultIndex

/**

  This sets the CTL loopback mode in the buffer

  @param[in] Host       - Pointer to sysHost
  @param[in] socket     - Socket number
  @param[in] ch         - Channel number
  @param[in] dimm       - Dimm number
  @param[in] ControlWordData  - Data to be written

**/
VOID
ChipSetCsLoopbackDdr5 (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    ControlWordData
  )
{
  UINT32                 SmbData = 0;
  struct dimmNvram (*dimmNvList) [MAX_DIMM];

  dimmNvList = GetDimmNvList (Host, socket, ch);

  if ((*dimmNvList) [dimm].DcpmmPresent) {

    //
    // Update RCD cache before writing to CWV RW02 Interface register
    //
    WriteRcdCwCache (socket, ch, SUB_CH_A_MASK, dimm, RDIMM_CW_PAGE0, DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_REG, ControlWordData);

    ChipSetCTLLoopbackFmc (Host, socket, ch, dimm, TRUE);

  } else {

    SmbData = ControlWordData;
    WriteRcdSmbDdr5 (Host, socket, ch, dimm, SUB_CH_A_MASK, BYTE_WRITE, RDIMM_CW_PAGE0, DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_REG, &SmbData);
  }
} // ChipSetCsLoopbackDdr5

/**

Setup PI settings parameters

@param Host             - Pointer to sysHost, the system Host (root) structure
@param socket           - CPU Socket Node number (Socket ID)
@param step             - The PI step to work on
@param piSetting        - The PI setting returned
@param piStart          - Test starting PI
@param NumberOfCycle    - Number of cycles
@param trainRes         - Training results

@retval None

**/
VOID
CsClkSetupPiSettingsParams (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    step,
  UINT16   *piSetting,
  UINT16   *piStart,
  UINT16   *NumberOfCycle,
  MRC_TT   CurrentTestType,
  struct ClkTrainingResults (**trainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5/2]
  )
{
  if (CurrentTestType == EarlyCsLoopbackTest) {
    *piSetting = CS_CLK_PI_START;
    *piStart = CS_CLK_PI_START;
    *NumberOfCycle = CS_CLK_PI_START + CS_CLK_TEST_RANGE;
  } else if (CurrentTestType == EarlyCsBacksideLoopbackTest) {
    *piSetting = BS_CS_CLK_PI_START;
    *piStart = BS_CS_CLK_PI_START;
    *NumberOfCycle = BS_CS_CLK_PI_START + BS_CS_CLK_TEST_RANGE;
  }
} // CsClkSetupPiSettingsParams

  /**

  Setup PI settings parameters

  @param Host             - Pointer to sysHost, the system Host (root) structure
  @param socket           - CPU Socket Node number (Socket ID)
  @param Ch               - Channel number
  @param step             - The PI step to work on
  @param piSetting        - The PI setting returned
  @param piStart          - Test starting PI
  @param NumberOfCycle    - Number of cycles
  @param trainRes         - Training results--for LRDIMM use.

  @retval None

  **/
VOID
CaClkSetupPiSettingsParams (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    Ch,
  UINT8    step,
  UINT16   *piSetting,
  UINT16   *piStart,
  UINT16   *NumberOfCycle,
  MRC_TT   CurrentTestType,
  struct   ClkTrainingResults (**trainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  struct channelNvram(*ChannelNvList)[MAX_CH];
  ChannelNvList = GetChannelNvList(Host, socket);

  if (CurrentTestType == EarlyCaLoopbackTest) {
    *piSetting = CA_CLK_TEST_START;
    *piStart = CA_CLK_TEST_START;
    *NumberOfCycle = CA_CLK_TEST_START + CA_CLK_TEST_RANGE;
  } else if (CurrentTestType == EarlyCaBacksideLoopbackTest) {
    *piSetting = CA_CLK_TEST_START_BACKSIDE;
    *piStart = CA_CLK_TEST_START_BACKSIDE;
    if ((*ChannelNvList) [Ch].timingMode == TIMING_2N) {
      *NumberOfCycle = CA_CLK_TEST_START_BACKSIDE + CA_CLK_TEST_RANGE_BACKSIDE_2N;
    } else {
      *NumberOfCycle = CA_CLK_TEST_START_BACKSIDE + CA_CLK_TEST_RANGE_BACKSIDE_1N;
    }
  }
} // CaClkSetupPiSettingsParams

/**

  Dq Train Feedback Calculation

  @param Host             - Pointer to sysHost, the system Host (root) structure
  @param socket           - CPU Socket Node number (Socket ID)
  @param ch               - Channel number
  @param csrReg           - DQ train feedback register value
  @param LogicResult      - Logic result for DQ train feedback register value

  @retval None

  **/
VOID
DqTrainFeedbackCalculation (
  IN      PSYSHOST Host,
  IN      UINT8    socket,
  IN      UINT8    ch,
  IN      UINT32   csrReg[MAX_STROBE],
  IN OUT  UINT32   LogicResult[MAX_STROBE]
  )
{
  UINT8  minGood;
  UINT8  strobe;

  minGood = DQ_COUNTING_WINDOW_VALUE / 2;//DDR5_TODO

  for (strobe = 0; strobe < MAX_STROBE; strobe++) {
    if ((csrReg[strobe] & 0x1FF) >= minGood) {
      LogicResult[strobe] = 1;
    } else {
      LogicResult[strobe] = 0;
    }
  }
} // DqTrainFeedbackCalculation

VOID
GetTxPiSamplepiDelayStartandEndDdr5 (
  PSYSHOST Host,
  UINT8 socket,
  INT16* piDelayStart,
  UINT16* piDelayEnd
  )
{
  *piDelayEnd = WR_LVL_PI_START_DDR5 + WR_LVL_PI_FULL_RANGE_DDR5;
  *piDelayStart = WR_LVL_PI_START_DDR5;
}

/**
Pattern used on DDR5 external write leveling training.

@retval PatWr
PatWr
**/
UINT8
GetChipPatternExternalWriteLevelingDdr5 (
  VOID
  )
{
  return PatWr;
}

/*++

Routine Description:

Get's the results from the current test for DDR5 WL.

Arguments:

Host      - Pointer to sysHost
dimm      - DIMM number
rank      - rank number of the DIMM

Returns:

N/A

--*/
VOID
GetResultsWLDdr5 (
  PSYSHOST                   Host,
  UINT8                      socket,
  UINT8                      dimm,
  UINT8                      rank,
  INT16                      piDelay [MAX_CH][SUB_CH],
  INT16                      TempCwlAdj,
  UINT32                     StrobePass [MAX_CH][SUB_CH],
  INT16                      CwlAdj [MAX_CH][SUB_CH][MAX_RANK_CH],
  INT16                      CwlAdjRecord [2][MAX_CH][SUB_CH],
  UINT8                      LockCwlAdj [MAX_CH][SUB_CH],
  INT16                      TxDqsDelay [MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE_DDR5 / 2],
  struct TrainingResultsDdr5 trainRes [MAX_CH][SUB_CH][MAX_STROBE_DDR5/2]
  )
{
  UINT8                ch;
  UINT8                strobe;
  UINT8                dWord;
  UINT32               csrReg;
  UINT16               piDelayStart;
  struct channelNvram  (*channelNvList) [MAX_CH];
  struct dimmNvram     (*dimmNvList) [MAX_DIMM];
  UINT16               PiDelayLocal = 0;
  struct ddrRank       (*rankList) [MAX_RANK_DIMM];
  UINT8                subch = 0;
  UINT8                Index = 0;
  UINT8                MaxStrobe = 0;
  UINT8                ChannelLocal = 0;
  UINT32               chBitmask = 0;
  UINT8                SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8                SubChMSVx8 = GetSubChMaxStrobeValid (Host) / 2;
  UINT8                MaxChDdr;
  UINT32               WlFlybyLoopCount;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);
  piDelayStart = WR_LVL_PI_START_DDR5;
  chBitmask = GetChBitmask (Host, socket, dimm, rank);
  //
  // Enter sense amp training mode to all the channels here to latch results.
  //
  //SetTrainingMode (Host, socket, dimm, rank, CHECKPOINT_MINOR_SENSE_AMP);

  GetWlFlyByLoopCount (&WlFlybyLoopCount);
  TrainingDelay (Host, ((WlFlybyLoopCount * (WL_WR_TO_WR_DLY + 8) * 2)), 26);

  //
  // Exit sense amp training mode
  //
  //SetTrainingMode (Host, socket, dimm, rank, CHECKPOINT_MINOR_DISABLE_SENSE_AMP);

  //
  // Stop the CPGC test in prep for the next phase setting.
  //
  if (IsBrsPresent (Host, socket)) {
    CpgcGlobalTestStop (Host, socket, chBitmask, NO_SUBCH);
  }

  for (ch = 0; ch < MaxChDdr; ch += GetChInc (Host)) {
    if ((*channelNvList) [ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, socket, ch);
    rankList = GetRankNvList (Host, socket, ch, dimm);
    if (!(*dimmNvList) [dimm].x4Present) {
      MaxStrobe = SubChMSVx8;
    } else {
      MaxStrobe = SubChMSVx4;
    }
    // Continue to the next rank if this one is disabled
    if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT) || (rank >= (*dimmNvList) [dimm].numRanks)) {
      continue;
    }
    for (subch = 0; subch < SUB_CH; subch++) {
      PiDelayLocal = piDelay [ch][subch] + (CwlAdj [ch][subch][ (*rankList) [rank].rankIndex] - T_CWL_ADJ_INITIAL_VALUE) * 128;
      dWord = (UINT8) ((PiDelayLocal - piDelayStart) / 32);
      Index = dWord / 32;

      for (strobe = 0; strobe < MaxStrobe; strobe++) {

        //
        // Skip if this is an ECC strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, strobe)) {
          continue;
        }
        if (StrobePass [ch][subch] & (1 << strobe)) {
          continue;
        }
        ChannelLocal = ch;
        GetChannelMapping (ch, subch, &ChannelLocal);
        if (IsBrsPresent (Host, socket)) {
          csrReg = LrDimmReadTrainFeedback (Host, socket, ChannelLocal, strobe, 0xFF);
        } else {
          csrReg = ReadTrainFeedbackDdr5 (Host, socket, ch, subch, strobe, dimm);
        }


        if ((csrReg & 0x1FF) >= GetMinDdr5WlDqFeedback (Host, socket)) {
          trainRes [ch][subch][strobe].results[Index] |= 1 << dWord;
          //
          //  Record the result.
          //
          StrobePass [ch][subch] |= (1 << strobe);
          if (IsBrsPresent (Host, socket)) {
            LockCwlAdj[ch][subch] = 1;
          } else {
            LockCwlAdj[ch][0] = 1;
            LockCwlAdj[ch][1] = 1;
          }
          TxDqsDelay [ch][subch][ (*rankList) [rank].rankIndex][strobe] = piDelay [ch][subch];
        }
      } // strobe loop
    } //subch loop.
  } // ch loop
} // GetResultsWLDdr5


/*++

Routine Description:

Get's the results from the current test for DDR5 Internal WL Coarse Sweep.

Arguments:

@param[in]  Host                         - Pointer to sysHost
@param[in]  ChBitMask                    - Ch Bit mask for which test should be setup for.
@param[in]  dimm                         - DIMM number
@param[in]  rank                         - rank number of the DIMM
@param[out] StrobePass                   - strobe pass number
@param[in]  InternalCycleAlignmentValue  - internal cycle delay to be tested
@param[out] InternalCycleAlignment       - contains the final cycle alignment
@param[out] trainRes                     - contain training result
Returns:

N/A

--*/
VOID
GetResultsInternalWLDdr5 (
  IN  PSYSHOST                    Host,
  IN  UINT8                       socket,
  IN  UINT32                      ChBitmask,
  IN  UINT8                       dimm,
  IN  UINT8                       rank,
  OUT UINT32                      StrobePass[MAX_CH][SUB_CH],
  IN  UINT8                       InternalCycleAlignmentValue,
  OUT INT16                       InternalCycleAlignment[MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE_DDR5 / 2],
  OUT struct TrainingResultsDdr5  trainRes[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8               ch;
  UINT8               strobe;
  UINT8               dWord;
  UINT32              csrReg;
  UINT16               piDelayStart;
  struct channelNvram (*channelNvList) [MAX_CH];
  struct dimmNvram (*dimmNvList) [MAX_DIMM];
  UINT16              PiDelayLocal = 0;
  struct ddrRank (*rankList) [MAX_RANK_DIMM];
  UINT8                subch = 0;
  UINT8                Index = 0;
  UINT8                MaxStrobe = 0;
  UINT8                ChannelLocal = 0;
  UINT32               chBitmask = 0;
  UINT8                SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8                SubChMSVx8 = GetSubChMaxStrobeValid (Host) / 2;
  UINT8                MaxChDdr;
  UINT32               WlFlybyLoopCount;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);
  piDelayStart = WR_LVL_PI_START_DDR5;
  chBitmask = GetChBitmask (Host, socket, dimm, rank);

  PiDelayLocal = (InternalCycleAlignmentValue - INTERNAL_CYCLE_ALM_START) * 128;
  Index = InternalCycleAlignmentValue - INTERNAL_CYCLE_ALM_START;
  dWord = (UINT8) (Index / 32);
  //
  // Enter sense amp training mode here to latch results
  //
  //SetTrainingMode (Host, socket, dimm, rank, CHECKPOINT_MINOR_SENSE_AMP);

  GetWlFlyByLoopCount (&WlFlybyLoopCount);
  TrainingDelay (Host, ((WlFlybyLoopCount * (WL_WR_TO_WR_DLY + 8)*2)), 26);

  //
  // Exit sense amp training mode
  //
  //SetTrainingMode (Host, socket, dimm, rank, CHECKPOINT_MINOR_DISABLE_SENSE_AMP);

  //
  // Stop the CPGC test in prep for the next phase setting.
  //
  if (IsBrsPresent (Host, socket)) {
    CpgcGlobalTestStop (Host, socket, chBitmask, NO_SUBCH);
  }
  for (ch = 0; ch < MaxChDdr; ch += GetChInc (Host)) {
    if ((*channelNvList) [ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, socket, ch);
    rankList = GetRankNvList (Host, socket, ch, dimm);
    if (!(*dimmNvList) [dimm].x4Present) {
      MaxStrobe = SubChMSVx8;
    } else {
      MaxStrobe = SubChMSVx4;
    }
    // Continue to the next rank if this one is disabled
    if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT) || (rank >= (*dimmNvList) [dimm].numRanks)) {
      continue;
    }
    if (IsLrdimmPresent (socket, ch, dimm)) {
      continue;
    }
    for (subch = 0; subch < SUB_CH; subch++) {
      for (strobe = 0; strobe < MaxStrobe; strobe++) {
        //
        // Skip if this is an ECC strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, strobe)) {
          continue;
        }
        if (StrobePass [ch][subch] & (1 << strobe)) {
          continue;
        }

        GetChannelMapping (ch, subch, &ChannelLocal);

        if (IsBrsPresent (Host, socket)) {
          //
          // Leverage the LRDIMM function to read the train feedback.
          //
          csrReg = LrDimmReadTrainFeedback (Host, socket, ChannelLocal, strobe, 0xFF);
        } else {
          csrReg = ReadTrainFeedbackDdr5 (Host, socket, ch, subch, strobe, dimm);
        }


        if ((csrReg & 0x1FF) >= GetMinDdr5WlDqFeedback (Host, socket)) {
          trainRes [ch][subch][strobe].results[dWord] |= (1 << (Index%32));
          //
          //  Record the result.
          //
          StrobePass [ch][subch] |= (1 << strobe);
          InternalCycleAlignment [ch][subch][ (*rankList) [rank].rankIndex][strobe] = InternalCycleAlignmentValue;
        }
      } // strobe loop
    } //subch loop.
  } // ch loop
} // GetResultsInternalWLDdr5

  /*++

  Routine Description:

  Get's the results from the current test for DDR5 Internal WL Fine Sweep.

  Arguments:

  Host      - Pointer to sysHost
  dimm      - DIMM number
  rank      - rank number of the DIMM

  Returns:

  N/A

  --*/
VOID
GetResultsInternalWLFineSweepDdr5 (
  PSYSHOST                   Host,
  UINT8                      socket,
  UINT8                      dimm,
  UINT8                      rank,
  UINT32                     StrobePass [MAX_CH][SUB_CH],
  INT16                      Offsets,
  INT16                      InternalCycleAlignment [MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE_DDR5 / 2],
  INT16                      TxDqsDelay [MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE_DDR5 / 2],
  struct TrainingResultsDdr5 trainRes [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8               ch;
  UINT8               strobe;
  UINT8               dWord;
  UINT32              csrReg;
  UINT16               piDelayStart;
  struct channelNvram (*channelNvList) [MAX_CH];
  struct dimmNvram (*dimmNvList) [MAX_DIMM];
  struct ddrRank (*rankList) [MAX_RANK_DIMM];
  UINT8                subch = 0;
  UINT8                MaxStrobe = 0;
  UINT8               swizzleStrobe = 0;
  UINT8                ChannelLocal = 0;
  UINT32               chBitmask = 0;
  UINT8                step = 0;
  UINT8                SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8                SubChMSVx8 = GetSubChMaxStrobeValid (Host) / 2;
  UINT8                MaxChDdr;
  UINT32               WlFlybyLoopCount;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);
  piDelayStart = WR_LVL_PI_START_DDR5;
  chBitmask = GetChBitmask (Host, socket, dimm, rank);

  GetWlFlyByLoopCount (&WlFlybyLoopCount);
  TrainingDelay (Host, ((WlFlybyLoopCount * (WL_WR_TO_WR_DLY + 8)*2)), 26);


  //
  // Stop the CPGC test in prep for the next phase setting.
  //
  if (IsBrsPresent (Host, socket)) {
    CpgcGlobalTestStop (Host, socket, chBitmask, NO_SUBCH);
  }

  for (ch = 0; ch < MaxChDdr; ch += GetChInc (Host)) {
    if ((*channelNvList) [ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, socket, ch);
    rankList = GetRankNvList (Host, socket, ch, dimm);
    if (!(*dimmNvList) [dimm].x4Present) {
      MaxStrobe = SubChMSVx8;
    } else {
      MaxStrobe = SubChMSVx4;
    }
    // Continue to the next rank if this one is disabled
    if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT) || (rank >= (*dimmNvList) [dimm].numRanks)) {
      continue;
    }
    // Skip internal write leveling if LRDIMM present
    if (IsLrdimmPresent (socket, ch, dimm)) {
      continue;
    }
    for (subch = 0; subch < SUB_CH; subch++) {
      for (strobe = 0; strobe < MaxStrobe; strobe++) {
        dWord = (UINT8) ((Offsets - WR_LVL_INTERNAL_FINE_PI_START_DDR5) / 32);
        //
        // Skip if this is an ECC strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, strobe)) {
          continue;
        }
        if (StrobePass [ch][subch] & (1 << strobe)) {
          continue;
        }

        GetChannelMapping (ch, subch, &ChannelLocal);

        if (IsBrsPresent (Host, socket)) {
          swizzleStrobe = CheckBacksideSwizzle (Host, socket, ChannelLocal, dimm, strobe);
          csrReg = LrDimmReadTrainFeedback (Host, socket, ChannelLocal, swizzleStrobe, 0xFF);
        } else {
          csrReg = ReadTrainFeedbackDdr5 (Host, socket, ch, subch, strobe, dimm);
        }


        if ((csrReg & 0x1FF) >= GetMinDdr5WlDqFeedback (Host, socket)) {
          for (step = 0; step < WR_LVL_FINE_STEP_SIZE_DDR5; step++) {
            trainRes [ch][subch][strobe].results[dWord] |= (1 << (((Offsets - WR_LVL_INTERNAL_FINE_PI_START_DDR5) + step) % 32));
          }
        }
      } // strobe loop
    } //subch loop.
  } // ch loop
} // GetResultsInternalWLFineSweepDdr5

VOID
RecEnTrainFeedbackDdr5 (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    subch,
  UINT8    strobe,
  UINT32   csrReg,
  UINT16   piDelay,
  struct   TrainingResultsDdr5 (*trainResInput) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8  step;
  UINT8  dWord;
  UINT8  minGood;
  struct TrainingResultsDdr5  (*trainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5/2];

  trainRes = trainResInput;
  minGood = SetMinGoodInit (Host);
  dWord = (UINT8) ((piDelay - REC_EN_PI_START) / 32);

  if ((csrReg & 0x1FF) >= minGood) {
    for (step = 0; step < REC_EN_STEP_SIZE; step++) {
      (*trainRes) [ch][subch][strobe].results[dWord] = (*trainRes) [ch][subch][strobe].results[dWord] | (1 << ((piDelay + step) % 32));
    } // step loop
  }

} // RecEnTrainFeedbackDdr5


  /*++

  Routine Description:

  Get's the results from the current test

  Arguments:

  Host      - Pointer to sysHost
  dimm      - DIMM number
  rank      - rank number of the DIMM

  Returns:

  N/A

  --*/
VOID
GetResultsWLFineSweep (
  PSYSHOST                   Host,
  UINT8                      socket,
  UINT8                      dimm,
  UINT8                      rank,
  INT16                      offset,
  INT16                      CwlAdj [MAX_CH][SUB_CH][MAX_RANK_CH],
  INT16                      TxDqsDelay [MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE_DDR5 / 2],
  struct TrainingResultsDdr5 trainRes [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8               ch;
  UINT8               strobe;
  UINT8               dWord;
  UINT8               step;
  UINT32              csrReg;
  UINT8               subch = 0;
  INT16               piDelayStart;
  struct channelNvram (*channelNvList) [MAX_CH];
  struct dimmNvram (*dimmNvList) [MAX_DIMM];
  struct ddrRank (*rankList) [MAX_RANK_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8               swizzleStrobe = 0;
  UINT8 ChannelLocal = 0;
  UINT32               chBitmask = 0;
  UINT8                MaxChDdr;
  UINT32               WlFlybyLoopCount;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);
  chBitmask = GetChBitmask (Host, socket, dimm, rank);

  piDelayStart = WR_LVL_FINE_PI_START_DDR5;

  GetWlFlyByLoopCount (&WlFlybyLoopCount);
  TrainingDelay (Host, ((WlFlybyLoopCount * (WL_WR_TO_WR_DLY + 8)*2)), 26);

  //
  // Stop the CPGC test in prep for the next phase setting.
  //
  if (IsBrsPresent (Host, socket)) {
    CpgcGlobalTestStop (Host, socket, chBitmask, NO_SUBCH);
  }

  for (ch = 0; ch < MaxChDdr; ch += GetChInc (Host)) {
    if ((*channelNvList) [ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, socket, ch);
    rankList = GetRankNvList (Host, socket, ch, dimm);
    // Continue to the next rank if this one is disabled
    if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT) || (rank >= (*dimmNvList) [dimm].numRanks)) {
      continue;
    }
    for (subch = 0; subch < SUB_CH; subch++) {
      //SKX change for upper strobes in N1 registers
      for (strobe = 0; strobe < MSVx4; strobe++) {

        if (IsDimmStrobeNotValidDdr5 (&(*dimmNvList) [dimm], strobe)) {
          continue;
        }

        ChannelLocal = ch;
        GetChannelMapping (ch, subch, &ChannelLocal);

        if (IsBrsPresent (Host, socket)) {
          swizzleStrobe = CheckBacksideSwizzle (Host, socket, ChannelLocal, dimm, strobe);
          csrReg = LrDimmReadTrainFeedback (Host, socket, ChannelLocal, swizzleStrobe, 0xFF);
        } else {
          csrReg = ReadTrainFeedbackDdr5 (Host, socket, ch, subch, strobe, dimm);
        }

        dWord = (UINT8) ((offset - piDelayStart) / 32);

        if ((csrReg & 0x1FF) >= GetMinDdr5WlDqFeedback (Host, socket)) {
          for (step = 0; step < WR_LVL_FINE_STEP_SIZE_DDR5; step++) {
            trainRes [ch][subch][strobe].results[dWord] |= (1 << (((offset - piDelayStart) + step) % 32));
          }
        }
      } // strobe loop
    }//subchannel loop
  } // ch loop
} // GetResultsWLFineSweep

  /*++
  Get the maximum number of data groups based on DIMM and group type for DDR5.

  @param Host  - Pointer to sysHost
  @param socket  - Socket
  @param ch    - Channel
  @param dimm  - DIMM
  @param group - Group type
  @param level - Level type

  @retval Max strobe

  --*/
UINT8
GetMaxStrobeScDdr5 (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  MRC_GT    group,
  MRC_LT    level
  )
{
  struct dimmNvram (*dimmNvList) [MAX_DIMM];
  UINT8  SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8  SubChMSVx8 = GetSubChMaxStrobeValid (Host) / 2;

  dimmNvList = &Host->nvram.mem.socket[socket].channelList[ch].dimmList;

#ifdef LRDIMM_SUPPORT
  if ((IsLrdimmPresent (socket, ch, dimm)) && (group == TxVref) && (level == DdrLevel)) {
    return SubChMSVx8;
  }
  if ((IsLrdimmPresent (socket, ch, dimm)) && (group == TxVref) && (level == LrbufLevel)) {
    return SubChMSVx4;
  }
  if ((IsLrdimmPresent (socket, ch, dimm)) && (group == RxVref) && (level == LrbufLevel)) {
    return SubChMSVx8;
  }
#endif
  //
  // Setup variables based on x4 or x8 DIMM or chipset specific
  //
  if ((group == TxEq) || (group == TxImode) || (group == RxEq) || (group == TxTco) || (group == RxCtleC) || (group == RxCtleR) ||
      (group == TxEqTap2) || ((group == RxVref) && (level == DdrLevel))) {
    return SubChMSVx8;
  } else if ((*dimmNvList) [dimm].x4Present ||
             (group == RxDqsPDelay) || (group == RxDqsNDelay) || (group == RxDqDqsDelay) ||
             (group == RxDqDelay) || (group == RxDqsDelay) || (group == RecEnDelay) ||
             (group == TxDqDelay) || (group == RxVref) || (group == RxEq) ||
             (group == TxEq) || (group == TxEqTap2)) {
    return SubChMSVx4;
  } else {
    return SubChMSVx8;
  }
} // GetMaxStrobeScDdr5

  /**

  This function writes CWL_ADJ.

  @param Host                 - Pointer to sysHost
  @param socket               - socket number
  @param ch                   - ch number
  @param dimm                 - dimm number
  @param channelNvList        - channelNvList
  @param CRAddDelay           - Additive Delay
  @param WLChipCleanUpStruct  - WL Delays


  @retval NA
  **/
VOID
UpdateChipMCDelayDdr5 (
  PSYSHOST                Host,
  UINT8                   socket,
  UINT8                   channel,
  UINT8                   Dimm,
  UINT8                   SubChannel,
  struct channelNvram (*channelNvList) [MAX_CH],
  INT16                   CRAddDelay,
  PCHIP_WL_CLEANUP_STRUCT WLChipCleanUpStruct
  )
{
  UINT8 ch;

  ch = channel;
  GetChannelMapping (channel, SubChannel, &ch);
  if ((*channelNvList) [ch].dimmList[Dimm].DcpmmPresent) {
    SetCwlAdj (Host, socket, ch, CWLADJ_DDRT, CRAddDelay);
  } else {
    SetCwlAdj (Host, socket, ch, CWLADJ_DDR4, CRAddDelay);
  }
}

/*

  Switch to 40b DDR5 channels

  @param[in, out]    Host      Pointer to SysHost structure

  @retval    SUCCESS

*/
UINT32
SwitchTo40bDdr5 (
  IN OUT  PSYSHOST  Host
  )
{

  Host->ChannelModeOperation = ModeDdr5SubChannel;

  return SUCCESS;
}

/*

  Switch to 80b DDR5 channels

  @param[in, out]    Host      Pointer to SysHost structure

  @retval    SUCCESS

*/
UINT32
SwitchTo80bDdr5 (
  IN OUT  PSYSHOST  Host
  )
{

  Host->ChannelModeOperation = ModeDdr5FullChannel;

  return SUCCESS;
}
