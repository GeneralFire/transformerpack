/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2019 Intel Corporation. <BR>

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

#include "Ddr5Core.h"
#include <Library/MemoryCoreLib.h>
#include <Library/MemoryServicesLib.h>

#define CA_WRAP_AROUND_VALUE  256
#define BACKSIDE_CA_WRAP_AROUND_VALUE  128
#define PER_GROUP             0
#define COMPOSITE_GROUPS      1
#define NO_SIGNAL             0xff
#define BACKSIDE_BCOM_WRAP_AROUND_VALUE     128
#define BACKSIDE_BCOM_FULL_EYE_WIDTH          64
#define RDIMM_RW1B 0x1B //SPR_TODO. Will remove it once the new RCW header is checked in.
extern VOID
PrintSampleArrayCsClk (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    ReadMode,
  UINT8    SubChannel,
  UINT16   Length,
  struct   ClkTrainingResults(*trainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

extern VOID
EvaluatePiSettingResultsCsClk (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT16    risingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  UINT16    fallingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  UINT8     step,
  UINT8     SubChannel,
  MRC_TT    CurrentTestType,
  struct    ClkTrainingResults(*trainResinput) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

GSM_CSN signalsToSweepBcomDDR5[2] = CHIP_GSM_CSN_BCOM_SIGNALSTOSWEEP_SPR;
extern GSM_CSN   CsToSweep[NUM_CS];
extern UINT32
ProgramQcaDelay (
  IN PSYSHOST  Host,
  IN UINT8     socket,
  IN UINT8     ch,
  IN UINT8     dimm,
  IN UINT8     subch,
  IN UINT8     rank,
  IN GSM_CSN   signal,
  IN UINT8     mode,
  IN INT8      Delay,
  IN BOOLEAN   PassThroughMode
  );
extern VOID
PrintSampleArrayCaClk (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    ReadMode,
  UINT8    SubChannel,
  UINT16   Length,
  struct   ClkTrainingResults(*trainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

extern VOID
EvaluatePiSettingResultsCaClk (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT16    risingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  UINT16    centerPoint[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  UINT16    fallingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  UINT16    pulseWidth[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  UINT8     step,
  UINT8     SubChannel,
  struct    ClkTrainingResults(*trainResinput) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );
//
// Local Prototypes
//
UINT32 CollectParityResults (PSYSHOST Host, UINT8 socket, UINT32 chToCheck, UINT8 dimm, UINT32 bwSerr[MAX_CH][3]);
VOID   ClearParityResults (PSYSHOST Host, UINT8 socket, UINT32 chToCheck, UINT8 dimm);
VOID   ClearParityResultsAlt (PSYSHOST Host, UINT8 socket, UINT32 chToCheck, UINT8 rank, UINT8 dimm);
VOID   ClearBwSerr (UINT32 bwSerr[MAX_CH][3]);
VOID   OffsetGroups (PSYSHOST Host, UINT8 socket, UINT8 ch, MRC_GT group, INT16 offset);
VOID   GetCtlClkResults (PSYSHOST Host, UINT8 socket, UINT8 dimm, UINT8 rank, UINT32 strobeFeedback[MAX_CH]);
VOID   GetCtlMarginsSweep (PSYSHOST Host, UINT8 socket, UINT8 dimm, UINT8 rank, GSM_CSN sig[MAX_CH], INT16 *le, INT16 *re);
VOID   GetChipBwSerrErrors (UINT32 errMask, UINT8 ch, UINT8 errorResult[MAX_CH][CHIP_CMD_CLK_TEST_RANGE], INT16 piIndex, INT16 offset, PCHIP_GET_CMD_MGN_SWP_STRUCT chipGetCmdMgnSwpStruct);
UINT32 EarlyCaClkVrefCA (PSYSHOST  Host);
UINT32 EarlyCaClkTimingMargin (PSYSHOST  Host);
UINT32 EarlyCaClkRcdVrefCA (PSYSHOST  Host, UINT8 TrainingMode);
UINT32 EarlyCaClkRcdTimingMargin (PSYSHOST  Host, UINT8 TrainingMode);
UINT32 VrefBcomTraining (PSYSHOST  Host);
UINT32
EarlyCaClkTimingMarginBackside (
  PSYSHOST  Host
  );

VOID
GetCaClkCompositeEyeBackside (
  INT16     LeftEdge[],
  INT16     RightEdge[],
  INT16*    CaLeft,
  INT16*    CaRight,
  UINT8     StartStrobe,
  UINT8     EndStrobe
  );

VOID
GetCaClkRcSimResultsBackside (
  UINT8     dimm,
  UINT8     subchannel,
  UINT8     rank,
  INT8      Vref,
  GSM_CSN   signal,
  UINT32    (**RcSimCaClkResults) [MAX_STROBE_DDR5 / 4][4]
  );

/**

  Finds CA CLK margins for the given Signal by the CA Vref setting.

  @param[in]  Host        - Pointer to sysHost
  @param[in]  Socket      - Socket number
  @param[in]  Dimm        - DIMM number
  @param[in]  Rank        - Rank number
  @param[in]  SignalSubCh - Sub channel signal
  @param[in]  CaVrefStep  - Ca vred step
  @param[out] *Le         - Left edge
  @param[out] *Re         - Right edge
  @param[in]  EyeWidthSC  - Sub channel Eye width
  @param[in]  *TrainRes   - Training result

  @retval N/A

**/
VOID
GetCaClkMarginsSweepVref (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Dimm,
  IN  UINT8     Rank,
  IN  GSM_CSN   SignalSubCh[MAX_CH][SUB_CH],
  IN  UINT8     CaVrefStep,
  OUT INT16     Le[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  OUT INT16     Re[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN  INT16     EyeWidthSC[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN  struct    ClkTrainingResults (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

/**

  Finds CA CLK margins for the given Signal by the CA Vref setting.

  @param[in]  Host            - Pointer to sysHost
  @param[in]  Socket          - Socket number
  @param[in]  Dimm            - DIMM number
  @param[in]  Rank            - Rank number
  @param[in]  SignalSubCh     - Sub channel signal
  @param[in]  Vref            - Vref value
  @param[out] *Le             - Left edge
  @param[out] *Re             - Right edge
  @param[in]  EyeWidthSC      - Sub channel EyeWidth
  @param[in]  *TrainRes       - Training result
  @param[in]  CaLeftCopyNum   - Left Copy number
  @param[in]  CaRightCopyNum  - Right Copy number

  @retval N/A

**/
VOID
GetCaClkMarginsSweepVrefBackside (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Dimm,
  IN  UINT8     Rank,
  IN  GSM_CSN   SignalSubCh[MAX_CH][SUB_CH],
  IN  INT8      Vref,
  OUT INT16     Le[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  OUT INT16     Re[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN  INT16     EyeWidthSC[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN  struct    ClkTrainingResults (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN  INT16     CaLeftCopyNum[MAX_CH][SUB_CH][COPY_NUM],
  IN  INT16     CaRightCopyNum[MAX_CH][SUB_CH][COPY_NUM]
  );

VOID
GetBcomClkMarginsSweepVref (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Dimm,
  UINT8     Rank,
  GSM_CSN   SignalSubCh[MAX_CH][SUB_CH],
  INT8      Vref,
  INT16     CsLeft[MAX_CH][SUB_CH],
  INT16     CsRight[MAX_CH][SUB_CH],
  INT16     EyeWidth[MAX_CH][SUB_CH],
  INT16     CsValue[MAX_CH][SUB_CH],
  struct    ClkTrainingResults(*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

UINT32
ProgramBcomDelay (
  IN PSYSHOST  Host,
  IN UINT8     socket,
  IN UINT8     ch,
  IN UINT8     dimm,
  IN UINT8     subch,
  IN UINT8     rank,
  IN GSM_CSN   signal,
  IN UINT8     mode,
  IN INT16      Delay,
  IN BOOLEAN   PassThroughMode
  );

/**

  Finds CA CLK margins for the given Signal.

  @param[in]  Host        - Pointer to sysHost
  @param[in]  Socket      - Socket number
  @param[in]  Dimm        - DIMM number
  @param[in]  Rank        - Rank number
  @param[in]  SignalSubCh - Sub channel signal
  @param[out] *CaLeft     - Left edge
  @param[out] *CaRight    - Right edge
  @param[in]  *TrainRes   - Training result

  @retval N/A

**/
VOID
GetCaClkMarginsSweepTiming (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Dimm,
  IN  UINT8     Rank,
  IN  GSM_CSN   SignalSubCh[MAX_CH][SUB_CH],
  OUT INT16     CaLeft[MAX_CH][SUB_CH],
  OUT INT16     CaRight[MAX_CH][SUB_CH],
  IN  struct    ClkTrainingResults (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

/**

This function print the sample array

@param host      - Pointer to sysHost
@param socket      - Socket number
@param ch        - Channel number
@param dimm      - DIMM number
@param ReadMode  - 5 = LRDIMM Write MWD Delay
4 = LRDIMM Read DQ DQS
3 = LRDIMM Read Recieve Enable Phase
2 = LRDIMM Write DWL
1 = Read DqDqs
0 = Write DqDqs

@retval N/A

**/

VOID
PrintSampleArrayBcomClk (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    ReadMode,
  UINT8    SubChannel,
  UINT16   Length,
  struct   ClkTrainingResults(*trainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  return;
} // PrintSampleArrayBcomClk

/**

  Do the BCOM training.

  @param[in] Host  - Pointer to sysHost

  @retval Status

**/
UINT32
BcomTraining (
  IN PSYSHOST  Host
  )
{
  return SUCCESS;
} // BcomTraining

/**

  Exectues the BCOM training.

  @param[in] Host - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
VrefBcomTraining (
  IN PSYSHOST  Host
  )
{
  return SUCCESS;
} //VrefBcomTraining

/**

Finds CA CLK margins for the given signal by the CA Vref setting.

@param Host        - Pointer to sysHost
@param socket      - Socket number
@param dimm        - DIMM number
@param rank        - rank number
@param signal      - Signal
@param *le         - Left edge
@param *re         - Right edge

@retval N/A

**/
VOID
GetBcomClkMarginsSweepVref (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Dimm,
  UINT8     Rank,
  GSM_CSN   SignalSubCh[MAX_CH][SUB_CH],
  INT8      Vref,
  INT16     CsLeft[MAX_CH][SUB_CH],
  INT16     CsRight[MAX_CH][SUB_CH],
  INT16     EyeWidth[MAX_CH][SUB_CH],
  INT16     CsValue[MAX_CH][SUB_CH],
  struct    ClkTrainingResults(*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  return;
} // GetBcomClkMarginsSweepVref

  /**

  Program BCOM Delay.

  @param rank        - rank number
  @param signal      - Signal
  @param Delay       - Delay

  @retval SUCCESS

  **/
UINT32
ProgramBcomDelay (
  IN PSYSHOST  Host,
  IN UINT8     socket,
  IN UINT8     ch,
  IN UINT8     dimm,
  IN UINT8     subchMask,
  IN UINT8     rank,
  IN GSM_CSN   signal,
  IN UINT8     mode,
  IN INT16     Delay,
  IN BOOLEAN   PassThroughMode
  )
{
  return SUCCESS;
}

/**
  Sweep BCOM signals and return margin results for EV tests.

  @param[in]      TestType:         Indicates the test type to be configured. It can use to customize the EV margin test setup
  @param[in]      Socket            Processor socket within the system (0-based)
  @param[in]      ChannelMask       One-hot encoding to mask a channel (0: executes if enabled/populated, 1: skip/masked)
  @param[in]      DimmRank          Structure to specify current dimm and rank under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test
  @param[in]      Direction         Specifies the sweep direction: low side only, high side only, or both side
  @param[in]      StepSize          Specifies the StepSize to do the margining
  @param[in]      CountWindow       Count Window Value
  @param[in]      SignalCount       Specifies the maximum signal count in the margin results structure - "BcomResults"
  @param[in, out] BcomResults       Pointer to margin results data structure which is indexed as [MAX_CH][SUB_CH]

  @retval MRC_STATUS_SUCCESS
**/
MRC_STATUS
EFIAPI
GetBcomTimingMargins (
  IN     MRC_TT            TestType,
  IN     UINT8             Socket,
  IN     UINT32            ChannelBitMask,
  IN     MRC_RT            DimmRank[MAX_CH],
  IN     MRC_LT            Level,
  IN     MRC_GT            Group,
  IN     UINT8             Direction,
  IN     UINT8             StepSize,
  IN     UINT32            CountWindow,
  IN OUT struct baseMargin *BcomResults
  )
{
  return MRC_STATUS_SUCCESS;
}

/**
  Sweep BCOM Vref and return margin results for EV tests.

  @param[in]      TestType:         Indicates the test type to be configured. It can use to customize the EV margin test setup
  @param[in]      Socket            Processor socket within the system (0-based)
  @param[in]      ChannelMask       One-hot encoding to mask a channel (0: executes if enabled/populated, 1: skip/masked)
  @param[in]      DimmRank          Structure to specify current dimm and rank under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test
  @param[in]      Direction         Specifies the sweep direction: low side only, high side only, or both side
  @param[in]      StepSize          Specifies the StepSize to do the margining
  @param[in]      CountWindow       Count Window Value
  @param[in]      SignalCount       Specifies the maximum signal count in the margin results structure - "BcomResults"
  @param[in, out] BcomResults       Pointer to margin results data structure which is indexed as [MAX_CH][SUB_CH]

  @retval MRC_STATUS_SUCCESS
**/
MRC_STATUS
EFIAPI
GetBcomVrefMargins (
  IN     MRC_TT            TestType,
  IN     UINT8             Socket,
  IN     UINT32            ChannelBitMask,
  IN     MRC_RT            DimmRank[MAX_CH],
  IN     MRC_LT            Level,
  IN     MRC_GT            Group,
  IN     UINT8             Direction,
  IN     UINT8             StepSize,
  IN     UINT32            CountWindow,
  IN OUT struct baseMargin *BcomResults
  )
{
  return MRC_STATUS_SUCCESS;
}
