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

#include "Ddr5Core.h"
#include <Library/Ddr5CoreLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Memory/Ddr5RcwRegs.h>
#include <Memory/JedecDefinitions.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <MemHostChipCommon.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemSweepLib.h>
#include <Library/OdtTableLib.h>
#include <Base.h>


typedef enum {
  CaVrefAtMax = 0,
  CaVrefAtMin,
  CaVrefAtNom,
  CaVrefLevelMax
} CA_VREF_LEVEL;


#define CA_WRAP_AROUND_VALUE  256
#define BACKSIDE_CA_WRAP_AROUND_VALUE  128
#define QCA_EXTRA_SWEEP_RANGE 128
#define GET_QCS_INDEX(SubChannel, Rank, Dimm) SubChannel * 4 + Rank + Dimm * 2


#define MAX_OUTPUT_DELAY_CONTROL_FOR_QXCA   64
#define X8_WA_OFFSET                        25
#define RCD_VREF_ANALOG_VALUE(VrefCode)  (975 - (VrefCode * 5)) // VrefCA as % of VDD. For example 975 means 97.5%.
#define RCD_VREF_ENCODE(VrefAnalogValue) ((975 - VrefAnalogValue) / 5)

#define FIXED_POINT_MULTIPLIER    100
#define MAX_SLEW_RATE_DELTA       200
#define MAX_SLEW_RATE_ADJ   7
#define PATTERN_LOOP_CNT    2
#define PATTERN_COMPLEX_LOOP_CNT    1

//
// Max number of slew rate sample combinations for UP and DOWN
// Loopcount    UP_ADJ    DOWN_ADJ
//      0         0         8
//      1         0         7
//      2         0         6
//      3         0         5
//      4         0         4
//      5         0         3
//      6         0         2
//      7         0         1
//      8         0         0
//      9         1         0
//     10         2         0
//     11         3         0
//     12         4         0
//     13         5         0
//     14         6         0
//     15         7         0
//
#define MAX_SLEW_RATE_SAMPLES       16

extern GSM_CSN   CsToSweep[NUM_CS];
extern GSM_CSN   CsToSweepBackSide[NUM_CS];

#pragma pack (push, 1)

GSM_CSN SignalsToSweepDDR5[NUM_CA_SIGNALS_DDR5] = CHIP_GSM_CSN_SIGNALSTOSWEEP_SPR;
GSM_CSN SignalsToSweepBacksideDDR5[NUM_CA_SIGNALS_DDR5] = CHIP_GSM_CSN_BACKSIDE_SIGNALSTOSWEEP_SPR;

QCX_SIGNAL_RAW_CARD_MAP SRx8Qca[2] = {
  {{DDR5_RCD_QACA_OUTPUT_DELAY_REG, RDIMM_RW_INVALID},                1}, ///< Rank 0 accesses QACA
  {{RDIMM_RW_INVALID,               RDIMM_RW_INVALID},                0}, ///< Rank 1 does not exist
};

QCX_SIGNAL_RAW_CARD_MAP SRx4Qca[2] = {
  {{DDR5_RCD_QACA_OUTPUT_DELAY_REG, RDIMM_RW_INVALID},                1}, ///< Rank 0 accesses QACA
  {{RDIMM_RW_INVALID,               RDIMM_RW_INVALID},                0}, ///< Rank 1 does not exist
};

QCX_SIGNAL_RAW_CARD_MAP DRx8Qca[2] = {
  {{DDR5_RCD_QACA_OUTPUT_DELAY_REG, RDIMM_RW_INVALID},                1}, ///< Rank 0 accesses QACA
  {{DDR5_RCD_QACA_OUTPUT_DELAY_REG, RDIMM_RW_INVALID},                1}, ///< Rank 1 accesses QACA
};

QCX_SIGNAL_RAW_CARD_MAP DRx4Qca[2] = {
  {{DDR5_RCD_QACA_OUTPUT_DELAY_REG, DDR5_RCD_QBCA_OUTPUT_DELAY_REG},  2}, ///< Rank 0 accesses QACA & QBCA
  {{DDR5_RCD_QACA_OUTPUT_DELAY_REG, DDR5_RCD_QBCA_OUTPUT_DELAY_REG},  2}, ///< Rank 1 accesses QACA & QBCA
};

QCX_SIGNAL_RAW_CARD_MAP *RawCardsQca[MAX_RAW_CARDS] = {
  SRx8Qca,
  SRx4Qca,
  DRx8Qca,
  DRx4Qca
};

typedef struct _SSA_MARGIN_CA {
  COMMON_1D_PARAMETERS      CommonParameters;
  MRC_TT                    TestType;
  UINT32                    CountWindow;
} SSA_MARGIN_CA;

#pragma pack (pop)

//
// Local Prototypes
//

INT8
GetCaVrefInitValue (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  UINT8     Dimm,
  IN  UINT8     Rank,
  IN  INT8      CaVrefStepCount,
  IN  INT8      CaVrefStepSize
  );

/**
  Sweep DCA signals and calculate the common margin per subchannel

  @param[in, out] DcaResults       - Pointer to a Tco training results data structure

  @retval MRC_STATUS_SUCCESS
**/
MRC_STATUS
SweepDcaPerSubCh (
  IN OUT  INT16             (*DcaResults)[MAX_CH][SUB_CH]
  );

/**
  Copy Tco setting values from the input structure to an output structure of just setting values

  @param[in]  BestTcoResults   - Pointer to a Tco training results data structure
  @param[out] Settings         - Pointer the current Setting Values in the structure

  @retval None
**/
VOID
GetTcoSettingValues (
  IN      SETTINGVALUE_EYEWIDTH_PAIR  (*BestTcoResults)[MAX_CH][SUB_CH],
     OUT  INT16                       (*Settings)[MAX_CH][SUB_CH]
);

/**

  This exits the CA training mode.

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number

  @retval N/A

**/
VOID
ExitCaTrainingModeDdr5 (
  IN PSYSHOST          Host,
  IN UINT8             Socket
  );

/**
  Program the training parameter setting for all subchannels based
  on input data in a TcoResults structure.

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in]      Param         - Pointer a specific parameter structures
  @param[in]      BestTcoResults   - Pointer to a Tco training results data structure

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetDcaTcoPerSubChToBestResults (
  IN     PSYSHOST                    Host,
  IN     UINT8                       Socket,
  IN     POWER_TRAIN_PARAM           *Param,
  IN     SETTINGVALUE_EYEWIDTH_PAIR  (*BestTcoResults)[MAX_CH][SUB_CH]
  );

/**
  DCA Tco Coarse Training Algorithm

  @param[in, out] BestTcoResults   - Pointer to a Tco training results data structure

  @retval MRC_STATUS_SUCCESS
**/
MRC_STATUS
DcaTcoCoarseTraining (
  IN OUT SETTINGVALUE_EYEWIDTH_PAIR  (*BestTcoResults)[MAX_CH][SUB_CH]
  );

/**
  DCA Tco Fine Training Algorithm

  @param[in, out] BestTcoResults   - Pointer to a Dca Tco training results data structure

  @retval SUCCESS
**/
MRC_STATUS
DcaTcoFineTraining (
  IN OUT SETTINGVALUE_EYEWIDTH_PAIR  (*BestTcoResults)[MAX_CH][SUB_CH]
  );

/**
  Get raw card mapping

  @param[in]  Host              - Pointer to sysHost
  @param[in]  Socket            - Current Socket
  @param[in]  Ch                - Channel number
  @param[in]  SubCh             - Sub Channel number
  @param[in]  Dimm              - Dimm number on Ch
  @param[in]  BacksideCopyNum   - Backside Copy Number
  @param[OUT] RawCardStrobe     - Raw Card Strober

  @retval N/A
**/
VOID
GetRawCardMapping (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     SubCh,
  IN  UINT8     Dimm,
  IN  UINT8     BacksideCopyNum,
  OUT UINT8     RawCardStrobe[MAX_A_B_STROBE]
  );

/**

  This sets the CA training mode in the register.

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number
  @param[in] Ch              - Channel number
  @param[in] SubChannelMask  - Sub Channel Mask
  @param[in] Dimm            - Dimm number
  @param[in] Rank            - Rank number
  @param[in] Enable          - Enable Ca Training Mode
  @param[in] DcaSamplingEdge - Dca Sampling Edge

  @retval N/A

**/
VOID
SetCaTrainingModeDdr5 (
  IN PSYSHOST          Host,
  IN UINT8             Socket,
  IN UINT8             Ch,
  IN UINT8             SubChannelMask,
  IN UINT8             Dimm,
  IN UINT8             Rank,
  IN BOOLEAN           Enable,
  IN DCA_SAMPLING_EDGE DcaSamplingEdge
  );

UINT32 CollectParityResults (PSYSHOST Host, UINT8 Socket, UINT32 ChToCheck, UINT8 Dimm, UINT32 BwSerr[MAX_CH][3]);
VOID   ClearParityResults (PSYSHOST Host, UINT8 Socket, UINT32 ChToCheck, UINT8 Dimm);
VOID   ClearParityResultsAlt (PSYSHOST Host, UINT8 Socket, UINT32 ChToCheck, UINT8 Rank, UINT8 Dimm);
VOID   ClearBwSerr (UINT32 BwSerr[MAX_CH][3]);
VOID   OffsetGroups (PSYSHOST Host, UINT8 Socket, UINT8 Ch, MRC_GT Group, INT16 Offset);
VOID   GetCtlClkResults (PSYSHOST Host, UINT8 Socket, UINT8 Dimm, UINT8 Rank, UINT32 StrobeFeedback[MAX_CH]);
VOID   GetCtlMarginsSweep (PSYSHOST Host, UINT8 Socket, UINT8 Dimm, UINT8 Rank, GSM_CSN Sig[MAX_CH], INT16 *Le, INT16 *Re);
VOID   GetChipBwSerrErrors (UINT32 ErrMask, UINT8 Ch, UINT8 ErrorResult[MAX_CH][CHIP_CMD_CLK_TEST_RANGE], INT16 PiIndex, INT16 Offset, PCHIP_GET_CMD_MGN_SWP_STRUCT ChipGetCmdMgnSwpStruct);
UINT32 EarlyCaClkVrefCA (PSYSHOST  Host);
UINT32 EarlyCaClkTimingMargin (PSYSHOST  Host);
UINT32 EarlyCaClkRcdVrefCA (PSYSHOST  Host, UINT8 TrainingMode);
UINT32 EarlyCaClkRcdTimingMargin (PSYSHOST  Host, UINT8 TrainingMode);
UINT32 EarlyCaClkVrefCABackside (PSYSHOST  Host);
UINT32
EarlyCaClkTimingMarginBackside (
  PSYSHOST  Host
  );

/**

  Determine the DCA margins for specified SubChannel/Dimm/Rank across all channnels

  @param[in]    Host        Pointer to syshost structure
  @param[in]    Socket      Current socket under test (0-based)
  @param[in]    SubChannel  Current subchannel under test (0-based)
  @param[in]    ChannelMask One hot bit to indicate if a channel has to be skipped or executed (1: to skip, 0: to execute)
  @param[in]    Dimm        Current dimm under test (0-based)
  @param[in]    Rank        Current rank under test (0-based)
  @param[in]    Signal      Current signal under test (valid from 0 to RCD_CA_SIGNALS_TO_SWEEP)
  @param[in]    DisplayIntermediateResults  Indication to enable debug prints
  @param[in]    ResultsFlags Modify the format for the output results (for information in MemSweepLib.h)
  @param[out]   Le          Pointer to report the margins for the left side
  @param[out]   Re          Pointer to report the margins for the right side

  @retval       MRC_STATUS_SUCCESS    When this function completes execution

**/
MRC_STATUS
GetDcaComplexMargins (
  IN        PSYSHOST        Host,
  IN        UINT8           Socket,
  IN        UINT32          ChannelMask,
  IN        UINT8           SubChannel,
  IN        UINT8           Dimm,
  IN        UINT8           Rank,
  IN        MRC_GT          Group,
  IN        UINT8           Signal,
  IN        BOOLEAN         DisplayIntermediateResults,
  IN        FLAGS_CATEGORY_RESULTS  ResultFlags,
      OUT   INT16           Le[MAX_CH][SUB_CH],
      OUT   INT16           Re[MAX_CH][SUB_CH]
);

/**

  Executes DCA and DCK Duty cycle training.

  @param[in] Host         - Pointer to sysHost
  @param[in] TrainingMode - Training Mode

  @retval N/A

**/
UINT32
RcdDcaDckDutyCycleTraining (
  IN PSYSHOST  Host,
  IN UINT8     TrainingMode
  );

/**
  Display Backside Edges for DDR5

  @param[in] Host              - Pointer to sysHost
  @param[in] Socket            - Current Socket
  @param[in] Ch                - Channel number
  @param[in] PulseWidth        - data of the pulse width
  @param[in] LeftEdge          - data of the left edge
  @param[in] RightEdge         - data of the right edge
  @param[in] SubChannel        - sub channel number
  @param[in] RawCardStrobe     - Raw Card Strobe

  @retval N/A
**/
VOID
DisplayBacksideEdgesDDR5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT16    PulseWidth[SUB_CH][MAX_STROBE_DDR5 / 2],
  IN INT16     LeftEdge[SUB_CH][MAX_STROBE_DDR5 / 2],
  IN INT16     RightEdge[SUB_CH][MAX_STROBE_DDR5 / 2],
  IN UINT8     SubChannel,
  IN UINT8     RawCardStrobe[MAX_A_B_STROBE]
  );

/**
  Display Backside Edges for DDR5

  @param[in]  LeftEdge          - data of the left edge
  @param[in]  RightEdge         - data of the right edge
  @param[out] CaLeft            - Ca Left
  @param[out] CaRight           - Ca Right
  @param[in]  RawCardStrobe     - Raw Card Strobe

  @retval N/A
**/
VOID
GetCaClkCompositeEyeBackside (
  IN  INT16     LeftEdge[MAX_STROBE_DDR5 / 2],
  IN  INT16     RightEdge[MAX_STROBE_DDR5 / 2],
  OUT INT16*    CaLeft,
  OUT INT16*    CaRight,
  IN  UINT8     RawCardStrobe[MAX_A_B_STROBE]
  );

/**

  Get CA CLK Backside RCSIM Results.

  @param Dimm                       - DIMM number
  @param Subchannel                 - Sub channel number
  @param Rank                       - Rank number
  @param VrefIndex                  - Vref Index
  @param Signal                     - Signal
  @param RcSimCaClkBacksideResults  - RcSim Ca Clk Results

  @retval N/A

**/
VOID
GetCaClkRcSimResultsBackside (
  UINT8     Dimm,
  UINT8     Subchannel,
  UINT8     Rank,
  UINT8     VrefIndex,
  GSM_CSN   Signal,
  UINT32    (**RcSimCaClkResults) [MAX_STROBE_DDR5 / 4][BACKSIDE_RCSIM_FEEDBACK_LEN]
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
  @param[in]  Vref            - Vref value for each channel
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
  IN  INT8      Vref[MAX_CH],
  OUT INT16     Le[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  OUT INT16     Re[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN  INT16     EyeWidthSC[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN  struct    ClkTrainingResults (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN  INT16     CaLeftCopyNum[MAX_CH][SUB_CH][COPY_NUM],
  IN  INT16     CaRightCopyNum[MAX_CH][SUB_CH][COPY_NUM]
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

  Find ca margins for the given Signal for rdimm.

  @param[in]  Host                 - Pointer to sysHost
  @param[in]  Socket               - Socket number
  @param[in]  MemSsType            - Mmemory subsystem type
  @param[in]  Channel              - Channel number. ALL_CH for training all channels in parallel.
  @param[in]  Dimm                 - DIMM number
  @param[in]  Subch                - sub channel number.
  @param[in]  Rank                 - Rank number
  @param[in]  Signal               - Signal
  @param[in]  CaVrefValue          - The CA VREF value.
  @param[out] Le                   - Left edge
  @param[out] Re                   - Right edge
  @param[in]  TrainingMode         - The training mode. (parralele or serial)
  @param[in]  RetrainSubChMask     - Retraining sub channel mask
  @param[in]  RetrainLoopCount     - The retraining loop count.
  @param[in]  PatternLoop          - Pattern Loop

  @retval N/A

**/
VOID
GetCaClkRcdMarginsSweepVref (
  IN  PSYSHOST      Host,
  IN  UINT8         Socket,
  IN  MRC_MST       MemSsType,
  IN  UINT8         Channel,
  IN  UINT8         Dimm,
  IN  UINT8         Subch,
  IN  UINT8         Rank,
  IN  GSM_CSN       Signal,
  OUT INT16         Le[MAX_CH][SUB_CH],
  OUT INT16         Re[MAX_CH][SUB_CH],
  IN  UINT8         TrainingMode,
  IN  UINT8         RetrainSubChMask,
  IN  UINT8         RetrainLoopCount,
  IN  UINT8         Patternloop
  );

/**

  Callback to be executed before DCA sweep is done

  @param[in]      CommonParameters    Pointer to the context information
  @param[in]      TestType            Current Test Type
  @param[in]      TrainingPatternMode Pattern mode

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
PreSweepDca1DWorker(
  IN        COMMON_1D_PARAMETERS    *CommonParameters,
  IN        MRC_TT                  TestType,
  IN        UINT8                   TrainingPatternMode
);

/**
  Callback to configure the engine to do the DCA test by test type

  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      TestType          Indicates the test type to be configured
  @param[in]      CountWindow       Count Window Value to program

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetupTestDcaWorker (
  IN COMMON_1D_PARAMETERS    *CommonParameters,
  IN MRC_TT                  TestType,
  IN UINT32                  CountWindow
  );

/**
  Callback to configure the engine to do the DCA test

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetupTestDcaComplex (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
);

/**
  Callback to configure the engine to do the DCA test with Simple pattern

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetupTestDcaSimple (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  );

/**
  Callback to configure the engine to do the RMT DCA test

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetupTestRmtDca (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  );

/**
  Callback to configure and start the DCA test

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
ExecuteTestDca (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
);

/**
  Callback to interpret the HW results after each sample test completes

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
ResultsProcessingDca (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
);

/**
  Callback to be executed when all sides are done

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
PostSweepDca1D (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
);

/**
  Returns TRUE if the DCA margin feedback needs to be overridden, otherwise
  it calls through to GetCaMarginsSweepErrorResFeedback and sets
  the feedback from that function

  @param[in]  CommonParamaters    - Pointer to the context information
  @param[in]  Channel             - Channel number
  @param[in]  SubChannel          - SubChannel number
  @param[in]  PiIndex             - pi index
  @param[out] *Feedback           - pointer to variable to store override data

  @retval TRUE if Feedback variable updated with desired override

**/
BOOLEAN
DcaMarginsSweepErrorResFeedbackOverride (
  IN     COMMON_1D_PARAMETERS    *CommonParameters,
  IN     UINT8                   Channel,
  IN     UINT8                   SubChannel,
  IN     INT16                   PiIndex,
     OUT UINT8                   *Feedback
  );

/**
  Callback to be executed before QCA sweep is done

  @param[in]      CommonParameters    Pointer to the context information
  @param[in]      TestType            Current Test Type

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
PreSweepQca1DWorker(
  IN        COMMON_1D_PARAMETERS    *CommonParameters,
  IN        MRC_TT                  TestType
  );

/**
  Callback to be executed before RMT QCA sweep is done

  @param[in]      CommonParameters    Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
PreSweepQca1DRmt (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  );


/**
  Callback to configure the engine to do the QCA test by test type

  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      TestType          Indicates the test type to be configured
  @param[in]      CountWindow       Count Window Value to program

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetupTestQcaWorker (
  IN COMMON_1D_PARAMETERS    *CommonParameters,
  IN MRC_TT                  TestType,
  IN UINT32                  CountWindow
  );

/**
  Callback to configure the engine to do the RMT QCA test

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetupTestQcaRmt (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  );

/**
  Callback to configure and start the QCA test

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
ExecuteTestQca (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
);

/**
  Callback to interpret the HW results after each sample test completes

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
ResultsProcessingQca (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
);

/**
  Callback to be executed when all sides are done

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
PostSweepQca1D (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
);

/**
  Function to enable DCA DFE Vref monitor for margin test.
  It enables DCA DFE training mode and turns on the DCA DFE Vref.

  @param[in]      Socket            Socket number
  @param[in]      Chananel          Channel number inside the socket
  @param[in]      Dimm              Dimm slot number
  @param[in]      Rank              Rank index inside the Dimm

  @retVal N/A
**/
STATIC
VOID
EnableDcaDfeVrefMonitor (
  IN UINT8   Socket,
  IN UINT8   Channel,
  IN UINT8   Dimm,
  IN UINT8   Rank
  );

//
//End Local Prototypes
//

/**

  Get the final CA signal based on the sub-channel and GSM_CSN signal (A0A --> A13A, PARA)

  @param[in]       SubChannelIndex        Current sub-channel under test (0-based)
  @param[in]       Signal                 Current signal under test (sub-channel 0 based).
                                          Allowed signals: A0A --> PARA

  @return   A0A --> A13A,PARA (SubChannel 0) or A0B --> A13B, PARB (SubChannel 1)

**/
GSM_CSN
GetRcdCaSignal (
  IN        UINT8             SubChannelIndex,
  IN        GSM_CSN           Signal
)
{
  UINT8   SignalIndex;

  if (!((Signal >= A0A) && (Signal <= PARA))) {
    RC_FATAL_ERROR ((Signal >= A0A) && (Signal <= PARA), ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_216);
    return A0A;
  }

  for (SignalIndex = 0; SignalIndex < NUM_CA_SIGNALS_DDR5; SignalIndex++) {
    if (SignalsToSweepDDR5[SignalIndex] == Signal) {
      break;
    }
  }

  if (SignalIndex >= NUM_CA_SIGNALS_DDR5) {
    RC_FATAL_ERROR (SignalIndex < NUM_CA_SIGNALS_DDR5, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_217);
    return A0A;
  }

  return SignalsToSweepDDR5[SignalIndex + SubChannelIndex * ((RCD_CA_SIGNALS_TO_SWEEP * 2) - 1)];

}

/**

  Get the final QCA signal based on the sub-channel and GSM_CSN signal (Q0ACA0 --> Q0ACA13)

  @param[in]       SubChannelIndex        Current sub-channel under test (0-based)
  @param[in]       Signal                 Current signal under test (sub-channel 0 based).
                                          Allowed signals: Q0ACA0 --> Q0ACA13

  @return   Q0ACA0 --> Q0ACA13 (SubChannel 0) or Q1ACA0 --> Q1ACA13 (SubChannel 1)

**/
GSM_CSN
GetQcaSignal (
  IN        UINT8             SubChannelIndex,
  IN        GSM_CSN           Signal
)
{
  UINT8   SignalIndex;

  if (!((Signal >= Q0ACA0) && (Signal <= Q0ACA13))) {
    RC_FATAL_ERROR ((Signal >= Q0ACA0) && (Signal <= Q0ACA13), ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_251);
    return Q0ACA0;
  }

  for (SignalIndex = 0; SignalIndex < NUM_CA_SIGNALS_DDR5; SignalIndex++) {
    if (SignalsToSweepBacksideDDR5[SignalIndex] == Signal) {
      break;
    }
  }

  if (SignalIndex >= NUM_CA_SIGNALS_DDR5) {
    RC_FATAL_ERROR (SignalIndex < NUM_CA_SIGNALS_DDR5, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_251);
    return Q0ACA0;
  }

  return SignalsToSweepBacksideDDR5[SignalIndex + SubChannelIndex * BACKSIDE_CA_SIGNALS_TO_SWEEP];
}

/**

For each data module, a 128 bit vector of zeros and ones holding the tests results is
evaluated which is a concatenation of the two reads from the feedback CRs.
BIOS locates the middle of the longest consecutive stream of ones (when the vector is evaluated as cyclic,
this location in the vector indicates the PI setting and logic delay for that data module).

@param Host              - Pointer to sysHost
@param Socket            - Current Socket
@param Ch                - Channel number
@param Dimm              - Dimm number on Ch
@param Rank              - Rank on Dimm
@param Strobe            - Strobe number
@param RisingEdge        - Pointer to save the rising edge
@param CenterPoint       - Pointer to save the center point
@param FallingEdge       - Pointer to save the falling edge
@param PulseWidth        - Pointer to save the pulse width
@param Step              - The PI Step to work on:
PI_CS_CLK_TYPE
@param SubChannel        - sub channel number
@param TrainResinput     - Pointer to ClkTrainingResults


@retval N/A

**/
VOID
EvaluatePiSettingResultsCaClk (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT8     Rank,
  UINT8     Strobe,
  UINT16    RisingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  UINT16    CenterPoint[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  UINT16    FallingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  UINT16    PulseWidth[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  UINT8     Step,
  UINT8     SubChannel,
  struct    ClkTrainingResults (*TrainResinput) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT16                     PiSetting;
  UINT16                     PiStart;
  UINT32                     Register;
  UINT32                     Bit;
  UINT8                      CyclicCase;
  UINT16                     OneCount;
  UINT16                     OneStart;
  UINT16                     NumberOfCycle;
  UINT16                     MaxOneCount;
  UINT8                      DWord;
  MRC_TT                     CurrentTrainingTestType;
  struct ddrRank (*RankList) [MAX_RANK_DIMM];
  struct ClkTrainingResults (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];

  CurrentTrainingTestType = GetCurrentTestType (Socket);
  Register = 0;
  OneStart = 0;

  Bit = BIT0;
  OneCount = 0;
  MaxOneCount = 0;
  CyclicCase = 0;

  TrainRes = TrainResinput;
  RisingEdge[Ch][SubChannel][Strobe] = 0;
  FallingEdge[Ch][SubChannel][Strobe] = 0;
  PulseWidth[Ch][SubChannel][Strobe] = 0;

  RankList = GetRankNvList (Host, Socket, Ch, Dimm);

  //
  // Start at bit 0
  //
  CaClkSetupPiSettingsParams (Host, Socket, Ch, Step, &PiSetting, &PiStart, &NumberOfCycle, CurrentTrainingTestType, &TrainRes);
  Host->var.mem.piSettingStopFlag[Ch] = GetStrobeMask (Host); //to make sure all the Strobe will be handled.
  while (Host->var.mem.piSettingStopFlag[Ch] & (1 << Strobe)) {

    DWord = GetTrainingResultIndexDDR5 (Step, PiSetting, PiStart);

    //
    // Get the proper training results
    //
    if ((PiSetting % 32) == 0) {
      Register = (*TrainRes) [Ch][SubChannel][Strobe].results[DWord];
      Bit = BIT0;
    }
    //
    // algo start
    //
    if (Register & Bit) {
      //
      // this Index is 1
      //
      if (OneCount == 0) {
        //
        // This first one so save the starting point
        //
        OneStart = PiSetting;
      }

      OneCount++;
    } else {
      //
      // this Index is 0
      //
      if (OneCount > 0) {
        //
        // We just moved from a one to a zero so check if that was the largest stream of one's
        //
        if (MaxOneCount < OneCount) {
          //
          // TBD  if i have two max value equals
          //
          MaxOneCount = OneCount;
          RisingEdge[Ch][SubChannel][Strobe] = OneStart;
          FallingEdge[Ch][SubChannel][Strobe] = PiSetting;
        }
        //
        // Start counting again at 0
        //
        OneCount = 0;
      }
    }

    //
    // Go to the next bit
    //
    PiSetting++;
    Bit = Bit << 1;

    //
    // Check if we'Re at the end of the cycle
    //
    if (PiSetting == NumberOfCycle) {
      if (OneCount > 0) {
        //
        // Cyclic case continue to scan
        // Start back over with bit 0 if the last bit is a 1
        //
        PiSetting = StartBackOverDdr5 (Step, PiStart);
        CyclicCase = 1;
      } else {
        //
        // stop the scan
        // We've checked all bits so save the results
        //
        Host->var.mem.piSettingStopFlag[Ch] &= ~(1 << Strobe);
        PulseWidth[Ch][SubChannel][Strobe] = FallingEdge[Ch][SubChannel][Strobe] - RisingEdge[Ch][SubChannel][Strobe];
        CenterPoint[Ch][SubChannel][Strobe] = ((PulseWidth[Ch][SubChannel][Strobe] / 2) + RisingEdge[Ch][SubChannel][Strobe]) % (NumberOfCycle - PiStart);
      }
    }
    //
    // Check if this is a cyclic case and if we are at the end of the one's
    //
    if ((CyclicCase == 1) && (OneCount == 0)) {
      Host->var.mem.piSettingStopFlag[Ch] &= ~(1 << Strobe);
      if (RisingEdge[Ch][SubChannel][Strobe] > FallingEdge[Ch][SubChannel][Strobe]) {
        PulseWidth[Ch][SubChannel][Strobe] = ((NumberOfCycle - RisingEdge[Ch][SubChannel][Strobe]) + FallingEdge[Ch][SubChannel][Strobe]) - PiStart;
        CenterPoint[Ch][SubChannel][Strobe] = (((PulseWidth[Ch][SubChannel][Strobe] / 2) + RisingEdge[Ch][SubChannel][Strobe])) % (NumberOfCycle - PiStart);
      } else {
        //
        // last bit is 1 and first bit after cycle is 0 bit 127 = 1 and bit 0 = 0
        //
        PulseWidth[Ch][SubChannel][Strobe] = FallingEdge[Ch][SubChannel][Strobe] - RisingEdge[Ch][SubChannel][Strobe];
        CenterPoint[Ch][SubChannel][Strobe] = ((PulseWidth[Ch][SubChannel][Strobe] / 2) + RisingEdge[Ch][SubChannel][Strobe]) % (NumberOfCycle - PiStart);
      }
    }
  } // while loop

#if FAULTY_PARTS_TRACKING_INJECT_ERROR == 1
  if ((Ch == 0) && (Dimm == 0) && (Rank == 1) && (Strobe == 5)) {
    PulseWidth[Ch][SubChannel][Strobe] = 1;
    CenterPoint[Ch][SubChannel][Strobe] = ((PulseWidth[Ch][SubChannel][Strobe] / 2) + RisingEdge[Ch][SubChannel][Strobe]) % (NumberOfCycle - PiStart);
  }
#endif   // FAULTY_PARTS_TRACKING_INJECT_ERROR

  //
  // If the PulseWidth (ones stream) is smaller than threshold need to sample again
  //
  if (PulseWidth[Ch][SubChannel][Strobe] <= RE_EDGE_SIZE_THRESHOLD) { //DDR5_TODO. the DDR5 CS CLK min pulse width is also RE_EDGE_SIZE_THRESHOLD ?
    //
    // This is a fatal error condition so map out the entire channel
    //
    Host->nvram.mem.socket[Socket].FaultyPartsFlag[Ch] |= (1 << Strobe);
    if (Step == PI_CS_CLK_TYPE) {
      (*RankList) [Rank].faultyParts[Strobe] |= FPT_REC_ENABLE_FAILED;

      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
        "Failed Ca Clk Pi\n");
    } else if (Step == PI_BCOM_CLK_TYPE) {
      (*RankList) [Rank].faultyParts[Strobe] |= FPT_REC_ENABLE_FAILED;

      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
        "Failed Bcom Clk Pi\n");
    }
  }
}

/**

Display Composite Ca Eye for DDR5.

@param Host      - Pointer to sysHost
@param Socket        - Current Socket
@param Ch        - Channel number
@param LeftEdge    - Pointer to save the LeftEdge
@param RightEdge   - Pointer to save the RightEdge
@param EyeWidth    - Pointer to save the EyeWidth
@param EyeCenterPoint   - Pointer to save EyeCenterPoint
@param SubChannel   - SubChannel number
@param MaxStrobe     - Maximum number of strobes

@retval N/A

**/
VOID
DisplayCompositeCaEyeDDR5 (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  INT16     LeftEdge,
  INT16     RightEdge,
  UINT16    EyeWidth[SUB_CH],
  UINT8     SubChannel,
  UINT8     MaxStrobe
  )
{
#ifdef DEBUG_CODE_BLOCK
  if (!(GetDebugLevel () & SDBG_MAX)) {
    return;
  }

  AcquirePrintControl ();

  RcDebugPrintLine (SDBG_MAX, (MaxStrobe * 4) + 4, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

  // Print Composite Left Edge
  RcDebugPrint (SDBG_DEFAULT, "Composite Left Edge:  ");
  RcDebugPrint (SDBG_DEFAULT, " %3d", LeftEdge);


  // Print Composite Right Edge
  RcDebugPrint (SDBG_DEFAULT, "\nComposite Right Edge: ");
  RcDebugPrint (SDBG_DEFAULT, " %3d", RightEdge);


  // Print Composite Eye Width
  RcDebugPrint (SDBG_DEFAULT, "\nComposite Eye Width: ");
  RcDebugPrint (SDBG_DEFAULT, "  %3d", EyeWidth[SubChannel]);

  RcDebugPrint (SDBG_DEFAULT, "\n\n");

  ReleasePrintControl ();
#endif //DEBUG_CODE_BLOCK
} // DisplayCompositecaEyeDDR5

/**

This function print the sample array

@param Host      - Pointer to sysHost
@param Socket      - Socket number
@param Ch        - Channel number
@param Dimm      - DIMM number
@param ReadMode  - 5 = LRDIMM Write MWD Delay
4 = LRDIMM Read DQ DQS
3 = LRDIMM Read Recieve Enable Phase
2 = LRDIMM Write DWL
1 = Read DqDqs
0 = Write DqDqs

@retval N/A

**/

VOID
PrintSampleArrayCaClk (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Ch,
  UINT8    Dimm,
  UINT8    ReadMode,
  UINT8    SubChannel,
  UINT16   Length,
  struct   ClkTrainingResults (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8   Strobe;
  UINT8   MaxStrobe;
  UINT32  PiSetting;
  struct dimmNvram (*DimmNvList) [MAX_DIMM];
  UINT16           LengthInDwords;
  UINT16           ArrayResultsSize;

  LengthInDwords   = Length / MAX_BITS_IN_DWORD;
  ArrayResultsSize = ARRAY_SIZE ((*TrainRes) [0][0][0].results);
  if ( LengthInDwords > ArrayResultsSize) {
    RcDebugPrint (SDBG_DEFAULT,
                  "Input parameter Length is out of range, LengthInDwords = 0x%0x, ArrayResultsSize = 0x%0x\n",
                  LengthInDwords, ArrayResultsSize);
    return;
  }

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  if ((ReadMode == 0) && !(*DimmNvList) [Dimm].x4Present) {
    RcDebugPrintLine (SDBG_DEFAULT, 23, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
    if (Host->nvram.mem.eccEn) {
      RcDebugPrint (SDBG_DEFAULT, "       0   1   2   3\n");
    } else {
      RcDebugPrint (SDBG_DEFAULT, "       0   1   2   3   4\n");
    }
    MaxStrobe = MAX_STROBE_DDR5 / 4;
  } else {
    RcDebugPrintLine (SDBG_DEFAULT, 48, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
    if (Host->nvram.mem.eccEn) {
      RcDebugPrint (SDBG_DEFAULT, "       0   1   2   3   4   5   6   7   8   9\n");
    } else {
      RcDebugPrint (SDBG_DEFAULT, "       0   1   2   3   4   5   6   7\n");
    }
    MaxStrobe = MAX_STROBE_DDR5 / 2;
  }

  for (PiSetting = 0; PiSetting < Length; PiSetting++) {
    if ((ReadMode > 1) && (PiSetting % 2)) {
      continue;
    }
    if (((ReadMode == PRINT_ARRAY_TYPE_LRDIMM_MDQ_RD_DELAY) || (ReadMode == PRINT_ARRAY_TYPE_LRDIMM_MDQ_WR_DELAY)) && (PiSetting % 4)) continue;
    if ((ReadMode == PRINT_ARRAY_TYPE_LRDIMM_MDQ_RD_DELAY) || (ReadMode == PRINT_ARRAY_TYPE_LRDIMM_MDQ_WR_DELAY)) {
      RcDebugPrint (SDBG_DEFAULT, "%d ", (((INT32) PiSetting / 4) - 15));

      if ((((INT32) PiSetting / 4 - 15) >= 0) && (((INT32) PiSetting / 4 - 15) < 10)) {
        RcDebugPrint (SDBG_DEFAULT, "  ");
      }

      if ((((INT32) PiSetting / 4 - 15) < 100) && (((INT32) PiSetting / 4 - 15) > 9)) {
        RcDebugPrint (SDBG_DEFAULT, " ");
      }

      if ((((INT32) PiSetting / 4 - 15) < 0) && (((INT32) PiSetting / 4 - 15) > -10)) {
        RcDebugPrint (SDBG_DEFAULT, " ");
      }

    } else {
      RcDebugPrint (SDBG_DEFAULT, "%d ", PiSetting);

      if (PiSetting < 10) {
        RcDebugPrint (SDBG_DEFAULT, "  ");
      }

      if ((PiSetting < 100) && (PiSetting > 9)) {
        RcDebugPrint (SDBG_DEFAULT, " ");
      }
    }
    for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
      if (IsDimmStrobeNotValidDdr5 (&(*DimmNvList) [Dimm], Strobe)) {
        continue;
      }

      if (((1 << PiSetting) & (*TrainRes) [Ch][SubChannel][Strobe].results[PiSetting / 32]) != 0) {
        RcDebugPrint (SDBG_DEFAULT, "   1");
      } else {
        RcDebugPrint (SDBG_DEFAULT, "   0");
      }
    } // Strobe loop

    RcDebugPrint (SDBG_DEFAULT, "\n");
  } // PiSetting loop
#endif //DEBUG_CODE_BLOCK
} // PrintSampleArrayCaClk


/**

  Programs the initial CaVref RCD values for the CA signals

  @param[in] Host   - pointer to SysHost
  @param[in] Socket - Socket number

  @retval None

**/
VOID
ProgramInitialRcdCaVref (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT8                 SubChannel;
  UINT8                 RankIndex;
  UINT8                 MaxChDdr;
  UINT8                 InitialRcdCaVref = 0;
  GSM_CSN               SignalToProgram;
  CHANNEL_NVRAM_STRUCT  (*ChannelNvList) [MAX_CH];
  struct dimmNvram      (*DimmNvList) [MAX_DIMM];

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Channel);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList) [Dimm].dimmPresent == 0) {
        continue;
      }
      //
      // Grab RankIndex with Rank 0 for each Dimm since DCA Vref is per Dimm from KIT ODT Table
      //
      RankIndex = GetRankIndex (Socket, Channel, Dimm, 0);
      InitialRcdCaVref = (*ChannelNvList)[Channel].DcaVrefRegValue[RankIndex];

      //
      //  DCA Vref is already initialized by CWV MMRC
      //
      if ((*DimmNvList) [Dimm].DcpmmPresent == 1) {
        continue;
      }

      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

        // Program RCD DCA0 - DCA6 initial internal vref for each DIMM
        for (SignalToProgram = A0A; SignalToProgram < A7A; SignalToProgram++) {
          ProgramSignalCaVref (Host, Socket, Channel, Dimm, SubChannel, SignalToProgram, InitialRcdCaVref);
        }
        // Program RCD DPAR initial internal vref for each DIMM
        ProgramSignalCaVref (Host, Socket, Channel, Dimm, SubChannel, PAR, InitialRcdCaVref);
      }
    }
  }
}

/**
  Callback to be executed before DCA sweep is done

  @param[in]      CommonParameters    Pointer to the context information
  @param[in]      TestType            Current Test Type
  @param[in]      TrainingPatternMode Pattern mode

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
PreSweepDca1DWorker (
  IN        COMMON_1D_PARAMETERS    *CommonParameters,
  IN        MRC_TT                  TestType,
  IN        UINT8                   TrainingPatternMode
  )
{
  UINT8                   ChannelIndex;
  UINT8                   MaxCh;
  CHIP_ERLY_CMD_CK_STRUCT ChipErlyCmdCKStruct;

  MaxCh = CommonParameters->MaxChDdr;
  ZeroMem ((UINT8 *)&ChipErlyCmdCKStruct, sizeof (ChipErlyCmdCKStruct));

  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {
    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    TrnOnChipCMDADDOpEn (
      CommonParameters->Host,
      CommonParameters->Socket,
      ChannelIndex,
      &ChipErlyCmdCKStruct
      );
  }

  SetCurrentTestType (CommonParameters->Socket, TestType);
  CommonParameters->Host->var.mem.socket[CommonParameters->Socket].TrainingStatus.Bits.TrainingPatternMode = TrainingPatternMode;

  IO_Reset (
    CommonParameters->Host,
    CommonParameters->Socket
    );

  //
  // Do the DDRIO INIT before starting CS CLK training.
  //
  CsClkDdrioInit (
    CommonParameters->Host,
    CommonParameters->Socket);
  //
  // Set QLBD and QLBS IO Direction.
  //
  SetQlbdQlbsIodirection (
    CommonParameters->Host,
    CommonParameters->Socket,
    RX_PATH_UNGATE_DISABLE);

  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {
    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    SetRcdTrainFeedbackSelectionByChannel (
      CommonParameters->Host,
      CommonParameters->Socket,
      ChannelIndex,
      CommonParameters->Dimm[ChannelIndex],
      CommonParameters->Rank[ChannelIndex],
      FIRST_ITERATION_LOOP);
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Callback to be executed before DCA sweep is done (simple pattern)

  @param[in]      CommonParameters    Pointer to the context information
  @param[in]      TestType            Current Test Type
  @param[in]      TrainingPatternMode          Current Test Type

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
PreSweepDca1DSimple (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{

  PreSweepDca1DWorker (CommonParameters, EarlyRcdCaLoopbackTest, CA_TRAINING_PATTERN_SIMPLE);
  return MRC_STATUS_SUCCESS;
}

/**
  Callback to be executed before DCA sweep is done (complex pattern)

  @param[in]      CommonParameters    Pointer to the context information
  @param[in]      TestType            Current Test Type
  @param[in]      TrainingPatternMode          Current Test Type

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
PreSweepDca1DComplex (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{

  PreSweepDca1DWorker (CommonParameters, EarlyRcdCaLoopbackTest, CA_TRAINING_PATTERN_XTALK);
  return MRC_STATUS_SUCCESS;
}

/**
  Callback to be executed before DCA sweep is done (complex pattern)

  @param[in]      CommonParameters    Pointer to the context information
  @param[in]      TestType            Current Test Type
  @param[in]      TrainingPatternMode          Current Test Type

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
PreSweepDcaTco1DComplex (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{

  PreSweepDca1DWorker (CommonParameters, DcaDckDutyCycleTest, DCA_DCK_DUTYE_CYCLE_TRAINING_PATTERN_COMPLEX);
  return MRC_STATUS_SUCCESS;
}

/**
  Function to enable DCA DFE Vref monitor for margin test.
  It enables DCA DFE training mode and turns on the DCA DFE Vref.

  @param[in]      Socket            Socket number
  @param[in]      Chananel          Channel number inside the socket
  @param[in]      Dimm              Dimm slot number
  @param[in]      Rank              Rank index inside the Dimm

  @retVal N/A
**/
STATIC
VOID
EnableDcaDfeVrefMonitor (
  IN UINT8   Socket,
  IN UINT8   Channel,
  IN UINT8   Dimm,
  IN UINT8   Rank
  )
{
#ifdef USE_LATEST_RCD_SPEC
  UINT8 SubChannelIndex;
  DDR5_RCD_DPAR_AND_DCA_6_0_DFE_TRAINING_MODE_STRUCT Ddr5RcdDparDcaDfeTrainingMode;
  DDR5_RCD_DFE_CONFIGURATION_STRUCT                  Ddr5RcdDfeConfiguration;

  for (SubChannelIndex = 0; SubChannelIndex < SUB_CH; SubChannelIndex++) {

    //
    // Enable DFE VREF (RW31)
    //
    ReadRcdCwCache (Socket, Channel, SubChannelIndex, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_DFE_CONFIGURATION_REG, &Ddr5RcdDfeConfiguration.Data);
    Ddr5RcdDfeConfiguration.Bits.dfe_feature_enable_control = 1;
    Ddr5RcdDfeConfiguration.Bits.dfe_vref_enable = 1;
    Ddr5RcdDfeConfiguration.Bits.error_counter_enable = 0;
    Ddr5RcdDfeConfiguration.Bits.rw_control_word_writes_broadcast = 0;
    RcdControlWordWrite (Socket, Channel, 1 << SubChannelIndex, Dimm, Rank, DDR5_RCD_DFE_CONFIGURATION_REG, Ddr5RcdDfeConfiguration.Data);

    //
    // Enter the DCA DFE training Mode. (RW32)
    //
    ReadRcdCwCache (Socket, Channel, SubChannelIndex, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_DPAR_AND_DCA_6_0_DFE_TRAINING_MODE_REG, &Ddr5RcdDparDcaDfeTrainingMode.Data);
    Ddr5RcdDparDcaDfeTrainingMode.Bits.global_dfe_training_mode_enable = 1;
    Ddr5RcdDparDcaDfeTrainingMode.Bits.training_source = 1; // Monitor XOR slicer output
    Ddr5RcdDparDcaDfeTrainingMode.Bits.dcn_dfe_pin_selection = 0;
    Ddr5RcdDparDcaDfeTrainingMode.Bits.rw_in_band_update_feature_enable = 0;
    Ddr5RcdDparDcaDfeTrainingMode.Bits.exit_from_rw_in_band_updated_mode = 1;
    RcdControlWordWrite (Socket, Channel, 1 << SubChannelIndex, Dimm, Rank, DDR5_RCD_DPAR_AND_DCA_6_0_DFE_TRAINING_MODE_REG, Ddr5RcdDparDcaDfeTrainingMode.Data);
  }

#endif
}

/**
  Callback to configure the engine to do the DCA test by test type

  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      TestType          Indicates the test type to be configured
  @param[in]      CountWindow       Count Window Value to program

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetupTestDcaWorker (
  IN COMMON_1D_PARAMETERS    *CommonParameters,
  IN MRC_TT                  TestType,
  IN UINT32                  CountWindow
  )
{
  MRC_MST MemSsType;
  MRC_RT  MrcRankList;
  UINT8   ChannelIndex;
  UINT8   SubChannelIndex;
  UINT32  ChannelMask;
  UINT8   MaxCh;
  BOOLEAN EnableCaTrainingMode = TRUE;

  ZeroMem ((UINT8*)&MrcRankList, sizeof (MrcRankList));

  MaxCh = CommonParameters->MaxChDdr;
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[CommonParameters->Socket];

  //
  // DCA DFE Vref monitor margin test must NOT in CA training mode.
  //
  if (CommonParameters->Group == DcaDfeVref) {
    EnableCaTrainingMode = FALSE;
  }
  MrcRankList.Dimm = 0;
  MrcRankList.Rank = 0;

  ChannelMask = 0;
  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    ChannelMask |= 1 << ChannelIndex;

    MrcRankList.Dimm = CommonParameters->Dimm[ChannelIndex];
    MrcRankList.Rank = CommonParameters->Rank[ChannelIndex];

    SetCaTrainingModeDdr5 (
      CommonParameters->Host,
      CommonParameters->Socket,
      ChannelIndex,
      TWO_SUBCH_MASK,
      MrcRankList.Dimm,
      MrcRankList.Rank,
      EnableCaTrainingMode,
      Rising_Edge
    );

    //
    // Enable RCD DFE training mode for DcaDfeVref (DFE Vref monitor) margin test.
    //
    if (CommonParameters->Group == DcaDfeVref) {
      EnableDcaDfeVrefMonitor (
        CommonParameters->Socket,
        ChannelIndex,
        MrcRankList.Dimm,
        MrcRankList.Rank
      );
    }
  }

  for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
    SetupTest (
      CommonParameters->Host,
      CommonParameters->Socket,
      MemSsType,
      ChannelMask,
      1,
      &MrcRankList,
      DdrLevel,
      TestType,
      GetRcdCaSignal (SubChannelIndex, CommonParameters->Signal),
      0,
      0
      );

    TestControl (
      CommonParameters->Host,
      CommonParameters->Socket,
      MemSsType,
      ChannelMask,
      SubChannelIndex,
      TestType,
      0,
      START_TEST,
      NOT_STOP_TEST
      );
  }

  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    SetCountingWindowByChannel (
      CommonParameters->Host,
      CommonParameters->Socket,
      ChannelIndex,
      CountWindow
      );

    EnableCsaTriggerByChannel (
      CommonParameters->Host,
      CommonParameters->Socket,
      ChannelIndex,
      CSA_TRIGGER_DISABLE
      );
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Callback to configure the engine to do the DCA test

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetupTestDcaComplex (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  return SetupTestDcaWorker (CommonParameters, DcaDckDutyCycleTest, DCA_DUTY_CYCLE_COUNTING_WINDOW_VALUE);
}

/**
  Callback to configure the engine to do the DCA test with Simple pattern

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetupTestDcaSimple (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  return SetupTestDcaWorker (CommonParameters, EarlyCaLoopbackTest, CSCA_TF_COUNTING_WINDOW_VALUE);
}

/**
  Callback to configure the engine to do the Rmt DCA test

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetupTestRmtDca (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  //
  // RMT shall select test type based on its private data, or override the counting window size based on CPGC configure.
  //
  SSA_MARGIN_CA *PrivateDca;
  PrivateDca = (SSA_MARGIN_CA *)CommonParameters;

  if (CommonParameters->Group == DcaDfeVref) {
    CommonParameters->Host->var.mem.socket[CommonParameters->Socket].TrainingStatus.Bits.TrainingPatternMode = DCA_DCK_DUTYE_CYCLE_TRAINING_PATTERN_COMPLEX_CS_ALWAYS_LOW;
  }

  return SetupTestDcaWorker (CommonParameters, PrivateDca->TestType, PrivateDca->CountWindow);
}

/**
  Callback to configure and start the DCA test

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
ExecuteTestDca (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
)
{
  UINT8   ChannelIndex;
  UINT8   MaxCh;

  MaxCh = CommonParameters->MaxChDdr;

  //
  // Generate an IO Reset. This is required because the pi delays changed.
  //
  IO_Reset (
    CommonParameters->Host,
    CommonParameters->Socket
    );

  //
  // Enabling the counter Mode.
  //
 for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {

  if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
    continue;
  }

  EnableDdrioTrainingCounterByChannel (
    CommonParameters->Host,
    CommonParameters->Socket,
    ChannelIndex
    );
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Returns TRUE if the feedback needs to be overridden

  @param[in]  CommonParamaters    - Pointer to the context information
  @param[in]  Channel             - Channel number
  @param[in]  SubChannel          - SubChannel number
  @param[in]  PiIndex             - pi index
  @param[out] *Feedback           - pointer to variable to store override data

  @retval TRUE if Feedback variable updated with desired override

**/
BOOLEAN
DcaVrefMarginsSweepErrorResFeedbackOverride (
  IN     COMMON_1D_PARAMETERS    *CommonParameters,
  IN     UINT8                   Channel,
  IN     UINT8                   SubChannel,
  IN     INT16                   PiIndex,
     OUT UINT8                   *Feedback
  )
{
  return FALSE;
}


/**
  Returns TRUE if the feedback needs to be overridden, otherwise
  it calls through to GetCaMarginsSweepErrorResFeedback and sets
  the feedback from that function

  @param[in]  CommonParamaters    - Pointer to the context information
  @param[in]  Channel             - Channel number
  @param[in]  SubChannel          - SubChannel number
  @param[in]  PiIndex             - pi index
  @param[out] *Feedback           - pointer to variable to store override data

  @retval TRUE if Feedback variable updated with desired override

**/
BOOLEAN
DcaMarginsSweepErrorResFeedbackOverride (
  IN     COMMON_1D_PARAMETERS    *CommonParameters,
  IN     UINT8                   Channel,
  IN     UINT8                   SubChannel,
  IN     INT16                   PiIndex,
     OUT UINT8                   *Feedback
  )
{
  return FALSE;
}


/**
  Callback to interpret the HW results after each sample test completes

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
ResultsProcessingDca (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
)
{
  MRC_MST MemSsType;
  UINT8   ChannelIndex;
  UINT8   SubChannelIndex;
  UINT32  ChannelMask;
  UINT8   MaxCh;
  UINT32  Err;
  UINT32  ValidMask;
  UINT8   StrobeLogical;
  UINT8   BitIndex;
  RESULTS_1D_CONDITION  *ResultConditionPtr;
  INT16   *CurrentDelayPtr;
  UINT8   Feedback;

  MaxCh = CommonParameters->MaxChDdr;
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[CommonParameters->Socket];

  ChannelMask = 0;
  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    ChannelMask = 1 << ChannelIndex;
    for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
      GetTrainResultStatus (
        CommonParameters->Host,
        CommonParameters->Socket,
        MemSsType,
        ChannelMask,
        1 << SubChannelIndex,
        1,
        &Err,
        &ValidMask,
        0,
        CommonParameters->Dimm[ChannelIndex]
        );

      StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, 0);

      for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {

        ResultConditionPtr = GetResultConditionPtr (CommonParameters, 0, ChannelIndex, StrobeLogical, BitIndex);

        if (GetCurrentTestType (CommonParameters->Socket) == DcaDckDutyCycleTest ||
            (CommonParameters->Host->var.mem.socket[CommonParameters->Socket].TrainingStatus.Bits.TrainingPatternMode == CA_TRAINING_PATTERN_XTALK && GetCurrentTestType (CommonParameters->Socket) == EarlyRcdCaLoopbackTest)) {

          if (Err > 0) {
            ResultConditionPtr->Current = FAIL_CONDITION;
          } else {
            ResultConditionPtr->Current = PASS_CONDITION;
          }

        } else if (GetCurrentTestType (CommonParameters->Socket) == EarlyRcdCaLoopbackTest) {

          if (Err > (CSCA_TF_COUNTING_WINDOW_VALUE / 2)) {
            ResultConditionPtr->Current = FAIL_CONDITION;
          } else {
            ResultConditionPtr->Current = PASS_CONDITION;
          }

        } else {

          RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_232);

        }

        CurrentDelayPtr = GetDelayPtr (CommonParameters, 0, ChannelIndex, StrobeLogical, CommonParameters->CurrentDelays);

        if (CommonParameters->Group == RcdDcaVref) {
          if (DcaVrefMarginsSweepErrorResFeedbackOverride (CommonParameters, ChannelIndex, SubChannelIndex, *CurrentDelayPtr, &Feedback)) {
            if (Feedback == 0) {
              ResultConditionPtr->Current = PASS_CONDITION;
            } else {
              ResultConditionPtr->Current = FAIL_CONDITION;
            }
          }
        } else {
          if (DcaMarginsSweepErrorResFeedbackOverride (CommonParameters, ChannelIndex, SubChannelIndex, *CurrentDelayPtr, &Feedback)) {
            if (Feedback == 0) {
              ResultConditionPtr->Current = PASS_CONDITION;
            } else {
              ResultConditionPtr->Current = FAIL_CONDITION;
            }
          }
        }
      } // BitIndex loop ...
    } // SunChannelIndex loop ...
  } // ChannelIndex loop ...

  return MRC_STATUS_SUCCESS;
}

/**
  Callback to be executed when all sides are done

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
PostSweepDca1D (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
)
{
  MRC_MST                 MemSsType;
  MRC_RT                  MrcRankList;
  UINT8                   ChannelIndex;
  UINT8                   SubChannelIndex;
  UINT32                  ChannelMask;
  UINT8                   MaxCh;

  MaxCh = CommonParameters->MaxChDdr;
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[CommonParameters->Socket];

  ChannelMask = 0;
  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    ChannelMask |= 1 << ChannelIndex;

    MrcRankList.Dimm = CommonParameters->Dimm[ChannelIndex];
    MrcRankList.Rank = CommonParameters->Rank[ChannelIndex];

    SetCaTrainingModeDdr5 (
      CommonParameters->Host,
      CommonParameters->Socket,
      ChannelIndex,
      TWO_SUBCH_MASK,
      MrcRankList.Dimm,
      MrcRankList.Rank,
      FALSE,
      Rising_Edge
      );

  }
  for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
    TestControl (
      CommonParameters->Host,
      CommonParameters->Socket,
      MemSsType,
      ChannelMask,
      SubChannelIndex,
      DcaDckDutyCycleTest,
      0,
      NOT_START_TEST,
      STOP_TEST
      );
  }

  //
  // Restore QLBD and QLBS IO Direction.
  //
  SetQlbdQlbsIodirection (
    CommonParameters->Host,
    CommonParameters->Socket,
    RX_PATH_UNGATE_ENABLE);

  SetCurrentTestType (CommonParameters->Socket, DefaultTest);
  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {
    DisableCADBAlwaysOnMode (CommonParameters->Socket, ChannelIndex);
  }

  return MRC_STATUS_SUCCESS;
}

/**

  VREF CA 1D sweep centering algorithm

**/
VOID
EarlyCaClkRcdVrefCa1D (
  VOID
  )
{
  PSYSHOST  Host;
  UINT8     Socket;
  UINT8     ChannelIndex;
  UINT8     MaxChDdr;
  UINT8     SubChannelIndex;
  UINT8     DimmIndex;
  UINT8     Signal;
  BOOLEAN   AnyDimmPopulatedAcrossChannels;
  INT16     WriteOffset;
  INT16     Le[MAX_CH][SUB_CH];
  INT16     Re[MAX_CH][SUB_CH];

  Host    = GetSysHostPointer ();
  Socket  = Host->var.mem.currentSocket;
  MaxChDdr  = GetMaxChDdr ();

  SetAepTrainingMode (Host, Socket, CHECKPOINT_MINOR_EARLY_CMD_CLK);

  for (DimmIndex = 0; DimmIndex < MAX_DIMM; DimmIndex++) {

    AnyDimmPopulatedAcrossChannels = FALSE;
    for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {
      if (!IsChannelEnabled (Socket, ChannelIndex)) {
        continue;
      }
      if (CheckRank (Host, Socket, ChannelIndex, DimmIndex, 0, DONT_CHECK_MAPOUT) == SUCCESS) {
        AnyDimmPopulatedAcrossChannels = TRUE;
        break;
      }
    } // ChannelIndex loop ...

    if (!AnyDimmPopulatedAcrossChannels) {
      continue;
    }

    for (Signal = 0; Signal < RCD_CA_SIGNALS_TO_SWEEP; Signal++ ) {
      for (SubChannelIndex = 0; SubChannelIndex < SUB_CH; SubChannelIndex++) {

        RcDebugPrintWithDevice (
          SDBG_MAX,
          Socket, NO_CH, NO_DIMM, SubChannelIndex, NO_RANK, NO_STROBE, NO_BIT,
          "%a\n",
          GetSignalStrDdr5 (GetRcdCaSignal (SubChannelIndex, SignalsToSweepDDR5[Signal]))
          );

        GetDcaComplexMargins (
          Host,
          Socket,
          0,
          SubChannelIndex,
          DimmIndex,
          0,                              ///< Only Rank 0
          RcdDcaVref,
          Signal,
          TRUE,
          ResultsRelative | SingleBitTrack,
          Le,
          Re
          );

        for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {
          if (!IsChannelEnabled (Socket, ChannelIndex)) {
            continue;
          }
          if (CheckRank (Host, Socket, ChannelIndex, DimmIndex, 0, DONT_CHECK_MAPOUT) != SUCCESS) {
            continue;
          }

          WriteOffset = (Re[ChannelIndex][SubChannelIndex] + Le[ChannelIndex][SubChannelIndex]) / 2;
          RcDebugPrintWithDevice (
            SDBG_MAX,
            Socket, ChannelIndex, NO_DIMM, SubChannelIndex, NO_RANK, NO_STROBE, NO_BIT,
            "WriteOffset %2d - ",
            WriteOffset
            );

          GetSetRcdDcaVref (
            Host,
            Socket,
            ChannelIndex,
            SubChannelIndex,
            DimmIndex,
            SignalsToSweepDDR5[Signal],
            GSM_READ_CSR | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE,
            &WriteOffset
            );

          GetSetRcdDcaVref (
            Host,
            Socket,
            ChannelIndex,
            SubChannelIndex,
            DimmIndex,
            SignalsToSweepDDR5[Signal],
            GSM_READ_CSR | GSM_READ_ONLY,
            &WriteOffset
            );
          RcDebugPrintWithDevice (
            SDBG_MAX,
            Socket, ChannelIndex, NO_DIMM, SubChannelIndex, NO_RANK, NO_STROBE, NO_BIT,
            "Final Value (Read) %3d\n",
            WriteOffset
            );
        } // ChannelIndex loop ...
      } // SubChannelIndex loop ...
    } // Signal loop ...
  } // DimmIndex loop ...

  SetAepTrainingMode (Host, Socket, DISABLE_TRAINING_STEP);

}

/**

  Trains command and address signals relative to clock

  @param Host  - Pointer to sysHost

  @retval Status

**/
UINT32
EarlyCaClk (
  PSYSHOST  Host
  )
{
  UINT8                               Socket;
  UINT8                               Ch = 0;
  MRC_STATUS                          Status;
  CHIP_ERLY_CMD_CK_STRUCT             ChipErlyCmdCKStruct;
  UINT8                               TrainingMode = 0;
  UINT8                               Dimm, Rank;
  struct dimmNvram (*DimmNvList) [MAX_DIMM];
  UINT8                               MaxChDdr;
  MaxChDdr = GetMaxChDdr ();
  ZeroMem ((UINT8 *)&ChipErlyCmdCKStruct, sizeof (ChipErlyCmdCKStruct));

  Socket = Host->var.mem.currentSocket;

  Host->var.mem.checkMappedOutRanks = 1;
  //
  // Return if DDR5 MemSs is not present.
  //
  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "EarlyCaClk Starts\n");
  if (!IsDdr5Present (Host, Socket)) {
    return MRC_STATUS_FAILURE;
  }
  //
  // Return if this Socket is disabled
  //
  if (IsSocketDimmPopulated (Host, Socket) != TRUE) {
    return SUCCESS;
  }

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_EARLY_CA_CLK, Socket));

  if (!IsMemFlowEnabled (CaClkTraining)) {
    return SUCCESS;
  }
  Status = TrainingModeSelect (CaClkTraining, &TrainingMode);
  if (Status != MRC_STATUS_SUCCESS) {
    return Status;
  }

  DoCompDdrioPerSetupOption (Socket, TRAINING_COMP_EVERY_TRAINING);

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    //
    // Turn on command/address output enable
    //
    TrnOnChipCMDADDOpEn (Host, Socket, Ch, &ChipErlyCmdCKStruct);
  } // Ch loop

  IO_Reset (Host, Socket);

  //
  // Execute RCD Loopback for RDIMM, LRDIMM, NVM DIMM configs
  //
  if (CheckRCDLoopback (Host)) {
    if (FeaturePcdGet (PcdCteBuild) == TRUE) {
      //
      // Do the DDRIO INIT before starting CS CLK training.
      //
      CsClkDdrioInit (Host, Socket);
      //
      // Set QLBD and QLBS IO Direction.
      //
      SetQlbdQlbsIodirection (Host, Socket, RX_PATH_UNGATE_DISABLE);
    }

    ProgramInitialRcdCaVref (Host, Socket);
    EnableDisableBcom (Host, Socket, TRUE);

    //
    // If DCA DFE is going to be trained, enable the circuit now so any timing
    // effects are taken into account during the DCA simple timing sweep.
    //
    if (IsMemFlowEnabled (RcdDcaDfeDdr5)) {
      Status = DcaDfeInitDdr5 (Host, Socket, DcaDfeEnableCircuit, A0A);
      if (Status != MRC_STATUS_SUCCESS) {
        RcDebugPrintWithDevice (
          SDBG_ERROR, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "Warning: failed to enable RCD DFE circuit before DCA simple timing sweep!\n"
          );
      }
    }

    SetCurrentTestType (Socket, EarlyRcdCaLoopbackTest);
    //
    // Get the CA CLK Timing Margin by simple training pattern. Execute the timing margin sweep for RDIMM, LRDIMM, NVDIMM configs.
    //
    Host->var.mem.socket[Socket].TrainingStatus.Bits.TrainingPatternMode = CA_TRAINING_PATTERN_SIMPLE;
    RcDebugPrint (
        SDBG_MEM_TRAIN,
      "DCA SIMPLE PATTERN TIMING\n"
      );
    EarlyCaClkRcdTimingMargin (Host, TrainingMode);

    if (!IsBrsPresent (Host, Socket)) {
      //
      // DCA VREF Training
      //
      RcDebugPrint (
          SDBG_MEM_TRAIN,
        "DCA SIMPLE PATTERN VREF\n"
        );

      EarlyCaClkRcdVrefCa1D ();

      //
      // Get the CA CLK Timing Margin with xtalk pattern. Execute the timing margin sweep for RDIMM, LRDIMM, NVDIMM configs.
      //
#if FixedPcdGetBool(PcdMcrSupport) == 0 // Xtalk and complex pattern not enabled for MCR yet
      if (FeaturePcdGet (PcdCteBuild) == FALSE) {
        //
        // Setup the test
        //
        SetCurrentTestType (Socket, EarlyRcdCaLoopbackTest);
        //
        // Set the CA Pattern to xtalk pattern.
        //
        Host->var.mem.socket[Socket].TrainingStatus.Bits.TrainingPatternMode = CA_TRAINING_PATTERN_XTALK;
        //
        // DCA Training with Complex pattern.
        //
        RcDebugPrint (
          SDBG_MEM_TRAIN,
          "DCA COMPLEX PATTERN TIMING\n"
          );
        EarlyCaClkRcdTimingMargin (Host, TrainingMode);
        //
        // Setup the test
        //
        SetCurrentTestType (Socket, EarlyRcdCaLoopbackTest);
        //
        // Set the CA Pattern to xtalk pattern.
        //
        Host->var.mem.socket[Socket].TrainingStatus.Bits.TrainingPatternMode = CA_TRAINING_PATTERN_XTALK;
        //
        // DCA VREF Training
        //
        RcDebugPrint (
            SDBG_MEM_TRAIN,
          "DCA COMPLEX PATTERN VREF\n"
          );

        EarlyCaClkRcdVrefCa1D ();

      }
#endif // FixedPcdGetBool(PcdMcrSupport)

    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

        for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
          if (IsChannelEnabled (Socket, Ch) == FALSE) {
            continue;
          }
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          DimmNvList = GetDimmNvList (Host, Socket, Ch);
          if (Rank >= (*DimmNvList) [Dimm].numRanks) {
            continue;
          }
          SetCaTrainingModeDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, FALSE, Rising_Edge);
        }
      }
    }
    EnableDisableBcom (Host, Socket, FALSE);
  } else {
    SetCurrentTestType (Socket, EarlyCaLoopbackTest);
    //
    // Get the CA CLK Timing Margin with simple CA training pattern.
    //
    Host->var.mem.socket[Socket].TrainingStatus.Bits.TrainingPatternMode = CA_TRAINING_PATTERN_SIMPLE;
    EarlyCaClkTimingMargin (Host);
    if (!IsBrsPresent (Host, Socket)) {
      //
      // Set the CA Pattern to xtalk pattern.
      //
      Host->var.mem.socket[Socket].TrainingStatus.Bits.TrainingPatternMode = CA_TRAINING_PATTERN_XTALK;
      //
      // Set CA vref for each DRAM device.
      //
      EarlyCaClkVrefCA (Host);
      //
      // Get the CA CLK Timing Margin with xtalk pattern.
      //
      EarlyCaClkTimingMargin (Host);
    }
    Host->var.mem.socket[Socket].TrainingStatus.Bits.QcaUcaTrainingDone = 1;
    //
    // Program the CL for UDIMM
    //
    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }
      ProgramCasLDdr5 (Host, Socket, Ch);
    } //Ch
  }

  //
  // Disable CADB always on Mode
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += 1) {
    DisableCADBAlwaysOnMode (Socket, Ch);
  }
  Host->var.mem.checkMappedOutRanks = 0;
  SetCurrentTestType (Socket, 0);

  return SUCCESS;
} // EarlyCaClk

/**

  Executes Rcd Dca Dck Duty Cycle Training

  @param[in] Host  - Pointer to sysHost

  @retval Status

**/
UINT32
EFIAPI
RcdDcaDckDutyCycle (
  IN PSYSHOST  Host
  )
{
  UINT8                               Socket;
  UINT8                               Ch = 0;
  MRC_STATUS                          Status;
  CHIP_ERLY_CMD_CK_STRUCT             ChipErlyCmdCKStruct;
  UINT8                               TrainingMode = 0;
  UINT8                               Dimm, Rank;
  struct dimmNvram                    (*DimmNvList) [MAX_DIMM];
  UINT8                               MaxChDdr;
  struct channelNvram                 (*ChannelNvList) [MAX_CH];

  MaxChDdr = GetMaxChDdr ();
  ZeroMem ((UINT8 *)&ChipErlyCmdCKStruct, sizeof (ChipErlyCmdCKStruct));
  Socket = Host->var.mem.currentSocket;
  Host->var.mem.checkMappedOutRanks = 1;
  //
  // Return if DDR5 MemSs is not present.
  //
  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "RcdDcaDckDutyCycle Starts\n");
  if (!IsDdr5Present (Host, Socket)) {
    return MRC_STATUS_FAILURE;
  }
  //
  // Return if this Socket is disabled
  //
  if (IsSocketDimmPopulated (Host, Socket) != TRUE) {
    return SUCCESS;
  }
  //
  // Execute the function for RDIMM, LRDIMM, NVM DIMM configs
  //
  if (!CheckRCDLoopback (Host)) {
    return SUCCESS;
  }
  ChannelNvList = GetChannelNvList (Host, Socket);

  if ((*ChannelNvList) [Ch].timingMode != TIMING_1N) {
    return SUCCESS;
  }

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_RCD_DCA_DCK_DUTY_CYCLE_TRAINING, Socket));

  if (!IsMemFlowEnabled (DcaDckDutyCycleTraining)) {
    return SUCCESS;
  }
  Status = TrainingModeSelect (DcaDckDutyCycleTraining, &TrainingMode);
  if (Status != MRC_STATUS_SUCCESS) {
    return Status;
  }

  DoCompDdrioPerSetupOption (Socket, TRAINING_COMP_EVERY_TRAINING);

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    //
    // Turn on command/address output enable
    //
    TrnOnChipCMDADDOpEn (Host, Socket, Ch, &ChipErlyCmdCKStruct);
  } // Ch loop

  IO_Reset (Host, Socket);

  //
  // Do the DDRIO INIT before starting CS CLK training.
  //
  CsClkDdrioInit (Host, Socket);
  //
  // Set QLBD and QLBS IO Direction.
  //
  SetQlbdQlbsIodirection (Host, Socket, RX_PATH_UNGATE_DISABLE);
  SetCurrentTestType (Socket, DcaDckDutyCycleTest);
  //
  // Run the DCA DCK Duty Cycyle Training for RDIMM, LRDIMM, NVDIMM configs.
  //
  Host->var.mem.socket[Socket].TrainingStatus.Bits.TrainingPatternMode = DCA_DCK_DUTYE_CYCLE_TRAINING_PATTERN_COMPLEX;
  RcdDcaDckDutyCycleTraining (Host, TrainingMode);


  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if (Rank >= (*DimmNvList) [Dimm].numRanks) {
          continue;
        }
        SetCaTrainingModeDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, FALSE, Rising_Edge);
      }
    }
  }

  //
  // Disable CADB always on Mode
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += 1) {
    DisableCADBAlwaysOnMode (Socket, Ch);
  }
  Host->var.mem.checkMappedOutRanks = 0;
  SetCurrentTestType (Socket, 0);

  return SUCCESS;
} // RcdDcaDckDutyCycle

/**

  Search the data collected in Results for the margin width and put eyewidth in a channel/subchannel array
  WARNING:
  This function MUST be executed only after context was already created (CreateContextParameters).
  This function MUST be executed only after resources are allocated (AllocatePoolForResults1D || AllocatePoolForResults1DAllRanks).

  @param[in]      CommonParameters  Pointer to the context information
  @param[in]      Results1D         Pointer to 1D results for all ranks
  @param[out]     MarginsPerSubCh   Pointer that contains per sub-channel minimum margin widths

**/
VOID
GetEyewidthPerSubChannelFromCurrentResults1D (
  IN        COMMON_1D_PARAMETERS    *CommonParameters,
  IN        RESULTS_1D              Results1D[MAX_RANK_CH],
      OUT   INT16                   (*EyewidthPerSubCh)[MAX_CH][SUB_CH]
)
{
  UINT8             ChannelIndex;
  UINT8             SubChannelIndex;
  UINT8             DimmIndex;
  UINT8             RankIndex;
  UINT8             LogicalRank;
  UINT8             MaxChDdr;
  UINT8             StrobeLogical;
  INT16             LeftComposite;
  INT16             RightComposite;
  struct baseMargin *StrobeMargin;
  BOOLEAN           EyeWidthUpdated;


  MaxChDdr = CommonParameters->MaxChDdr;

  for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex++) {

    if (!CommonParameters->ChannelEnabledLocal[ChannelIndex]) {
      continue;
    }

    for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {

      StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, 0);

      LeftComposite = MIN_INT16;
      RightComposite = MAX_INT16;

      EyeWidthUpdated = FALSE;

      for (DimmIndex = 0; DimmIndex < MAX_DIMM; DimmIndex++) {
        for (RankIndex = 0; RankIndex < MAX_RANK_DIMM; RankIndex++) {

          LogicalRank = GetLogicalRank (CommonParameters->Host, CommonParameters->Socket, ChannelIndex, DimmIndex, RankIndex);

          if (CommonParameters->RankMask & (1 << (LogicalRank))) {
            continue;
          }
          if (!IsRankPresent (CommonParameters->Socket, ChannelIndex, DimmIndex, RankIndex)) {
            continue;
          }
          StrobeMargin = GetResultPtrAtStrobeLevel (
                           CommonParameters,
                           0,
                           ChannelIndex,
                           StrobeLogical,
                           &(Results1D[LogicalRank])
                         );
          //if currently zero, just pick the margin, pick the smaller value to get common margins for all dimms
          if ((*EyewidthPerSubCh)[ChannelIndex][SubChannelIndex] == 0) {
            (*EyewidthPerSubCh)[ChannelIndex][SubChannelIndex] = ABS (StrobeMargin->p - StrobeMargin->n);
            LeftComposite = StrobeMargin->n;
            RightComposite = StrobeMargin->p;
            EyeWidthUpdated = TRUE;
          } else {
            //for non-zero results, pick the smaller value to get common margins for all dimms
            if ((*EyewidthPerSubCh)[ChannelIndex][SubChannelIndex] > (ABS (StrobeMargin->p - StrobeMargin->n))) {
              (*EyewidthPerSubCh)[ChannelIndex][SubChannelIndex] = ABS (StrobeMargin->p - StrobeMargin->n);
              LeftComposite = StrobeMargin->n;
              RightComposite = StrobeMargin->p;
              EyeWidthUpdated = TRUE;
            }
          }
        } //RankIndex
      } //DimmIndex

      RcDebugPrintWithDeviceInternal (
        SDBG_MAX, NO_SOCKET, ChannelIndex, NO_DIMM, SubChannelIndex, NO_RANK, NO_STROBE, NO_BIT,
        "MinEyeWidth(%a) = %d ", GetSignalStr (GetRcdCaSignal(SubChannelIndex, CommonParameters->Signal)),
        (*EyewidthPerSubCh)[ChannelIndex][SubChannelIndex]
      );
      if (EyeWidthUpdated) {
        RcDebugPrint(
          SDBG_MAX, 
          "LE = %4d RE = %4d\n",
          LeftComposite,
          RightComposite
          );
      } else {
        RcDebugPrint (
          SDBG_MAX,
          "No update in MinEyeWidth\n"
          );
      }

    } // SubChannelIndex loop ...

  } // ChannelIndex loop

}

/**
  Sweep DCA signals and calculate the common margin per subchannel

  @param[in, out] DcaResults       - Pointer to a Tco training results data structure

  @retval MRC_STATUS_SUCCESS
**/
MRC_STATUS
SweepDcaPerSubCh (
  IN OUT  INT16             (*DcaResults)[MAX_CH][SUB_CH]
)
{
  UINT8         Socket;
  UINT8         TrainingMode;
  UINT8         SignalIndex;
  UINT8         SubChannelIndex;
  UINT8         DimmIndex;
  UINT8         RankIndex;
  UINT32        RankMask;
  UINT8         SubChannelParameter;
  COMMON_1D_PARAMETERS   *CommonParameters;
  RESULTS_1D    Results[MAX_RANK_CH];
  INT16         (*EyewidthPerSubCh)[MAX_CH][SUB_CH];

  EyewidthPerSubCh = RcAllocatePool (sizeof (*EyewidthPerSubCh));
  if (EyewidthPerSubCh == NULL) {
    return FAILURE;
  }

  ZeroMem ((UINT8 *) (EyewidthPerSubCh), sizeof (*EyewidthPerSubCh));

  CommonParameters = RcAllocatePool (sizeof (*CommonParameters));
  if (CommonParameters == NULL) {
    return FAILURE;
  }

  ZeroMem ((UINT8 *) (CommonParameters), sizeof (*CommonParameters));

  Socket = GetCurrentSocketId ();

  TrainingModeSelect (CaClkTraining, &TrainingMode);

  for (SubChannelIndex = 0; SubChannelIndex < SUB_CH; SubChannelIndex++) {
    for (SignalIndex = 0; SignalIndex < RCD_CA_SIGNALS_TO_SWEEP; SignalIndex++) {

      if (TrainingMode == RUN_IN_PARALLEL) {
        SubChannelParameter = ALL_SUBCH;
      } else {
        SubChannelParameter = SubChannelIndex;
      }
      //
      // Only test rank 0 of each dimm, so mask of other ranks
      //
      RankMask = 0;
      for (DimmIndex = 0; DimmIndex < MAX_DIMM; DimmIndex++) {
        for (RankIndex = 0; RankIndex < MAX_RANK_DIMM; RankIndex++) {
          if (RankIndex == 0) {
            continue;
          } else {
            RankMask |= (1 << (DimmIndex * MAX_RANK_DIMM + RankIndex));
          }
        } //RankIndex
      } //DimmIndex

      CreateContextParameters (
        CommonParameters,
        0,
        Socket,
        0,
        RankMask,
        SubChannelParameter,
        NULL,
        DdrLevel,
        DcaDelay,
        SignalsToSweepDDR5[SignalIndex],
        CA_CLK_STEP_SIZE, 1,
        0, 0, CA_CLK_TEST_RANGE,
        DefaultStartOffset | LimitSearch,
        DefaultResults | SingleBitTrack | ResultsRelative,
        DefaultFsm
        );

      AllocatePoolForResultsAllRanks1D (CommonParameters, Results);

      ProcessSweepAllRanks1D (
        CommonParameters,
        PreSweepDcaTco1DComplex,
        SetupTestDcaComplex,
        ExecuteTestDca,
        NULL,
        PostSweepDca1D,
        ResultsProcessingDca,
        Results
        );

      GetEyewidthPerSubChannelFromCurrentResults1D (CommonParameters, Results, EyewidthPerSubCh);

      UpdateMinimumEyewidthInStructure (CommonParameters, EyewidthPerSubCh, DcaResults);

      ReleaseResourcesAllRanks1D (CommonParameters, Results);

    } // Signal loop ...

    if (TrainingMode == RUN_IN_PARALLEL) {
      break;
    }

  } // SubChannelIndex loop ...

  RcFreePool (CommonParameters);
  RcFreePool (EyewidthPerSubCh);
  return MRC_STATUS_SUCCESS;
}

/**

  Search the data collected in Results and update margin results in channel/subchannel/signal array
  WARNING:
  This function MUST be executed only after context was already created (CreateContextParameters).
  This function MUST be executed only after resources are allocated (AllocatePoolForResults1D ).

  @param[in]      CommonParameters  Pointer to the context information
  @param[in]      Results1D         Pointer to 1D results
  @param[in]      SignalIndex       The index of signal in DcaResults arrray
  @param[out]     DcaResults        Pointer that contains per channel, subchannel and signal margin result.

**/
VOID
UpdateDcaMarginResultsFromCurrentResults1D (
  IN     COMMON_1D_PARAMETERS    *CommonParameters,
  IN     RESULTS_1D              *Results1D,
  IN     UINT8                   SignalIndex,
     OUT struct baseMargin       *DcaResults
  )
{
  UINT8             ChannelIndex;
  UINT8             SubChannelIndex;
  UINT8             StrobeLogical;
  struct baseMargin *StrobeMargin;
  struct baseMargin *DcaResultForCurrentSignal;

  for (ChannelIndex = 0; ChannelIndex < CommonParameters->MaxChDdr; ChannelIndex++) {
    if (!CommonParameters->ChannelEnabledLocal[ChannelIndex]) {
      continue;
    }

    for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
      StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, 0);

      StrobeMargin = GetResultPtrAtStrobeLevel (CommonParameters, 0, ChannelIndex, StrobeLogical, Results1D);

      //
      // DcaResults is indexed as [MAX_CH][SUB_CH][SIGNAL_CNT]
      //
      DcaResultForCurrentSignal = &DcaResults[ChannelIndex * ((SUB_CH)* RCD_CA_SIGNALS_TO_SWEEP) + SubChannelIndex * RCD_CA_SIGNALS_TO_SWEEP + SignalIndex];

      DcaResultForCurrentSignal->p = StrobeMargin->p;
      DcaResultForCurrentSignal->n = StrobeMargin->n;

      RcDebugPrint (SDBG_SWEEP_L0, "\nCh %d, SubCh %d, Signal %a, Margin low %d high %d\n",
                    ChannelIndex, SubChannelIndex, GetSignalStr (CommonParameters->Signal), DcaResultForCurrentSignal->n, DcaResultForCurrentSignal->p);

    } // SubChannelIndex loop ...
  } // ChannelIndex loop
}

/**

  Determine the DCA margins for specified SubChannel/Dimm/Rank across all channnels

  @param[in]    Host        Pointer to syshost structure
  @param[in]    Socket      Current socket under test (0-based)
  @param[in]    SubChannel  Current subchannel under test (0-based)
  @param[in]    ChannelMask One hot bit to indicate if a channel has to be skipped or executed (1: to skip, 0: to execute)
  @param[in]    Dimm        Current dimm under test (0-based)
  @param[in]    Rank        Current rank under test (0-based)
  @param[in]    Signal      Current signal under test (valid from 0 to RCD_CA_SIGNALS_TO_SWEEP)
  @param[in]    DisplayIntermediateResults  Indication to enable debug prints
  @param[in]    ResultsFlags Modify the format for the output results (for information in MemSweepLib.h)
  @param[out]   Le          Pointer to report the margins for the left side
  @param[out]   Re          Pointer to report the margins for the right side

  @retval       MRC_STATUS_SUCCESS    When this function completes execution

**/
MRC_STATUS
GetDcaComplexMargins (
  IN        PSYSHOST        Host,
  IN        UINT8           Socket,
  IN        UINT32          ChannelMask,
  IN        UINT8           SubChannel,
  IN        UINT8           Dimm,
  IN        UINT8           Rank,
  IN        MRC_GT          Group,
  IN        UINT8           Signal,
  IN        BOOLEAN         DisplayIntermediateResults,
  IN        FLAGS_CATEGORY_RESULTS  ResultFlags,
      OUT   INT16           Le[MAX_CH][SUB_CH],
      OUT   INT16           Re[MAX_CH][SUB_CH]
)
{
  UINT8         MaxChDdr;
  UINT8         ChannelIndex;
  UINT8         StrobeLogical;
  UINT32        DebugLevel;
  INT16         MinLeftMargin;
  INT16         MinRightMargin;
  INT16         RelativeOffsetP;
  INT16         RelativeOffsetN;
  INT16         *OriginalDelayPtr;
  MRC_RT        DimmRank[MAX_CH];
  GSM_CSN       SweepSignal;
  RESULTS_1D    Results1D;
  COMMON_1D_PARAMETERS  CommonParameters;

  DebugLevel = GetDebugLevel ();
  MaxChDdr = GetMaxChDdr ();
  SweepSignal = SignalsToSweepDDR5[Signal];

  for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex += GetChInc (Host)) {
    DimmRank[ChannelIndex].Dimm = Dimm;
    DimmRank[ChannelIndex].Rank = Rank;
  }

  CreateContextParameters(
    &CommonParameters,
    Ddr5MemSs,
    Socket,
    ChannelMask,
    0,
    SubChannel,
    DimmRank,
    DdrLevel,
    Group,
    SweepSignal,
    4,
    1,
    -15,
    15,
    0,
    DefaultStartOffset,
    ResultFlags,
    DefaultFsm
    );

  AllocatePoolForResults1D (&CommonParameters, &Results1D);
  //
  // Get the timing margin for the given Signal with the specified vref.
  //
  RcDebugPrintWithDevice (
    SDBG_MAX, Socket, NO_CH, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
    "Calling ProcessSweep1D for %a Coarse\n",
    GetSignalStrDdr5 (GetRcdCaSignal (SubChannel, SweepSignal))
    );

  ProcessSweep1D (
    &CommonParameters,
    PreSweepDca1DComplex,
    SetupTestDcaComplex,
    ExecuteTestDca,
    NULL,
    PostSweepDca1D,
    ResultsProcessingDca,
    &Results1D
    );

  MinLeftMargin  = 0 - ONE_DCLK;
  MinRightMargin = ONE_DCLK;
  for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex += GetChInc (Host)) {

    if (IsChannelEnabled (Socket, ChannelIndex) == FALSE ||
        (ChannelMask & (1 << ChannelIndex)) != 0) {
      continue;
    }

    StrobeLogical     = GetStrobeLogical (&CommonParameters, ChannelIndex, SubChannel, 0);
    OriginalDelayPtr  = GetDelayPtr (&CommonParameters, 0, ChannelIndex, StrobeLogical, CommonParameters.OriginalDelays);

    RelativeOffsetP = Results1D.ResultsPerStrobe[0][ChannelIndex][StrobeLogical].p;
    RelativeOffsetN = Results1D.ResultsPerStrobe[0][ChannelIndex][StrobeLogical].n;

    if ((ResultFlags & ResultsRelative) == 0) {

      RelativeOffsetP -= *OriginalDelayPtr;
      RelativeOffsetN -= *OriginalDelayPtr;

    }
    if (RelativeOffsetN > 0) {
      MinLeftMargin = -1;
    } else if (MinLeftMargin < RelativeOffsetN) {
      MinLeftMargin = RelativeOffsetN;
    }
    if (RelativeOffsetP < 0) {
      MinRightMargin = +1;
    } else if (MinRightMargin > RelativeOffsetP) {
      MinRightMargin = RelativeOffsetP;
    }

  } // Ch loop ...

  RcDebugPrintWithDevice (
    SDBG_MAX, Socket, NO_CH, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
    "Calling ProcessSweep1D for %a Fine MinLeftMargin = %3d MinRightMargin = %3d\n",
    GetSignalStrDdr5 (GetRcdCaSignal (SubChannel, SweepSignal)),
    MinLeftMargin,
    MinRightMargin
    );

  CommonParameters.StepSize     = 1;
  CommonParameters.OffsetLeft   = MinLeftMargin;
  CommonParameters.OffsetRight  = MinRightMargin;

  ProcessSweep1D (
    &CommonParameters,
    PreSweepDca1DComplex,
    SetupTestDcaComplex,
    ExecuteTestDca,
    NULL,
    PostSweepDca1D,
    ResultsProcessingDca,
    &Results1D
    );

  PrintCurrentResults1D (&CommonParameters, &Results1D);

  SetDebugLevel (DebugLevel);

  for (ChannelIndex = 0; ChannelIndex < MaxChDdr; ChannelIndex += GetChInc (Host)) {

    if (IsChannelEnabled (Socket, ChannelIndex) == FALSE ||
        (ChannelMask & (1 << ChannelIndex)) != 0) {
      continue;
    }

    if (IsDimmPresent (Socket, ChannelIndex, Dimm) == FALSE) {
      continue;
    }

     StrobeLogical = GetStrobeLogical (&CommonParameters, ChannelIndex, SubChannel, 0);

     Le[ChannelIndex][SubChannel] = Results1D.ResultsPerStrobe[0][ChannelIndex][StrobeLogical].n;
     Re[ChannelIndex][SubChannel] = Results1D.ResultsPerStrobe[0][ChannelIndex][StrobeLogical].p;

     if (DisplayIntermediateResults) {
       RcDebugPrintWithDevice (
         SDBG_MAX, Socket, ChannelIndex, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
         "Results ProcessSweep1D for %a Left Edge = %3d Right Edge = %3d\n",
         GetSignalStrDdr5 (GetRcdCaSignal (SubChannel, SweepSignal)),
         Le[ChannelIndex][SubChannel],
         Re[ChannelIndex][SubChannel]
         );
     }
  } // Ch loop ...

  ReleaseResources1D (&CommonParameters, &Results1D);

  return MRC_STATUS_SUCCESS;
}

/**
  Sweep DCA signals and return margin results for EV tests.

  @param[in]      TestType:         Indicates the test type to be configured. It can use to customize the EV margin test setup
  @param[in]      Socket            Processor socket within the system (0-based)
  @param[in]      ChannelMask       One-hot encoding to mask a channel (0: executes if enabled/populated, 1: skip/masked)
  @param[in]      DimmRank          Structure to specify current dimm and rank under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test
  @param[in]      Direction         Specifies the sweep direction: low side only, high side only, or both side
  @param[in]      StepSize          Specifies the StepSize to do the margining
  @param[in]      CountWindow       Count Window Value
  @param[in]      SignalCount       Specifies the maximum signal count in the margin results structure - "DcaResults"
  @param[in, out] DcaResults        Pointer to margin results data structure which is indexed as [MAX_CH][SUB_CH][Signal]

  @retval MRC_STATUS_SUCCESS
**/
MRC_STATUS
EFIAPI
GetDcaMargins (
  IN     MRC_TT            TestType,
  IN     UINT8             Socket,
  IN     UINT32            ChannelBitMask,
  IN     MRC_RT            DimmRank[MAX_CH],
  IN     MRC_LT            Level,
  IN     MRC_GT            Group,
  IN     UINT8             Direction,
  IN     UINT8             StepSize,
  IN     UINT32            CountWindow,
  IN     UINT8             SignalCount,
  IN OUT struct baseMargin *DcaResults
  )
{
  UINT8                   TrainingMode;
  UINT8                   SignalIndex;
  UINT8                   ChannelIndex;
  UINT8                   SubChannelIndex;
  UINT8                   SubChannelParameter;
  SSA_MARGIN_CA           PrivateDca;
  RESULTS_1D              Results;
  MRC_MST                 MemSsType;
  FLAGS_CATEGORY_OFFSETS  FlagsOffsets;
  FLAGS_CATEGORY_RESULTS  FlagsResults;
  FLAGS_CATEGORY_FSM      FlagsFsm;
  PSYSHOST                Host;
  //
  // Save/Restore
  //
  UINT8                   MaxCh;
  DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_STRUCT         RcdParityAlert[MAX_CH];
  DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_STRUCT HostInterfaceTrainingMode[MAX_CH];
  DDR5_RCD_DPAR_AND_DCA_6_0_DFE_TRAINING_MODE_STRUCT  Ddr5RcdDparDcaDfeTrainingMode[MAX_CH][SUB_CH];
  DDR5_RCD_DFE_CONFIGURATION_STRUCT                   Ddr5RcdDfeConfiguration[MAX_CH][SUB_CH];

  ZeroMem ((UINT8 *) RcdParityAlert, sizeof (RcdParityAlert));
  ZeroMem ((UINT8 *) HostInterfaceTrainingMode, sizeof (HostInterfaceTrainingMode));
  ZeroMem ((UINT8 *) Ddr5RcdDparDcaDfeTrainingMode, sizeof (Ddr5RcdDparDcaDfeTrainingMode));
  ZeroMem ((UINT8 *) Ddr5RcdDfeConfiguration, sizeof (Ddr5RcdDfeConfiguration));

  //
  // Make sure output storage size have enough space to store margin results of all signals
  //
  RC_FATAL_ERROR (SignalCount >= RCD_CA_SIGNALS_TO_SWEEP, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_220);

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

  MaxCh = GetMaxChDdr ();

  Host = GetSysHostPointer ();

  PrivateDca.TestType = TestType;
  PrivateDca.CountWindow = CountWindow;

  //
  // Save
  //
  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {
    if (!IsChannelEnabled (Socket, ChannelIndex)) {
      continue;
    }
    if (ChannelBitMask & (1 << ChannelIndex)) {
      continue;
    }

    ReadRcdCwCache (Socket, ChannelIndex, SUB_CH_A, DimmRank[ChannelIndex].Dimm, RDIMM_CW_PAGE0, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, &RcdParityAlert[ChannelIndex].Data);
    ReadRcdCwCache (Socket, ChannelIndex, SUB_CH_A, DimmRank[ChannelIndex].Dimm, RDIMM_CW_PAGE0, DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_REG, &HostInterfaceTrainingMode[ChannelIndex].Data);

    if (Group == DcaDfeVref) {
      for (SubChannelIndex = 0; SubChannelIndex < SUB_CH; SubChannelIndex++) {
        ReadRcdCwCache (Socket, ChannelIndex, SubChannelIndex, DimmRank[ChannelIndex].Dimm, RDIMM_CW_PAGE0, DDR5_RCD_DPAR_AND_DCA_6_0_DFE_TRAINING_MODE_REG, &Ddr5RcdDparDcaDfeTrainingMode[ChannelIndex][SubChannelIndex].Data);
        ReadRcdCwCache (Socket, ChannelIndex, SubChannelIndex, DimmRank[ChannelIndex].Dimm, RDIMM_CW_PAGE0, DDR5_RCD_DFE_CONFIGURATION_REG, &Ddr5RcdDfeConfiguration[ChannelIndex][SubChannelIndex].Data);
      }
    }
  }

  TrainingModeSelect (CaClkTraining, &TrainingMode);

  //
  // Turn off BCOM
  //
  EnableDisableBcom (Host, Socket, TRUE);

  FlagsOffsets = DefaultStartOffset;
  FlagsResults = DefaultResults | SingleBitTrack | ResultsRelative;
  if (Direction == SWEEP_NEG_SIDE_ONLY) {
    FlagsFsm = LeftSideOnly;
  } else if (Direction == SWEEP_POS_SIDE_ONLY) {
    FlagsFsm = RightSideOnly;
  } else {
    FlagsFsm = DefaultFsm; // both side.
  }

  for (SubChannelIndex = 0; SubChannelIndex < SUB_CH; SubChannelIndex++) {
    for (SignalIndex = 0; SignalIndex < RCD_CA_SIGNALS_TO_SWEEP; SignalIndex++) {

      if (TrainingMode == RUN_IN_PARALLEL) {
        SubChannelParameter = ALL_SUBCH;
      } else {
        SubChannelParameter = SubChannelIndex;
      }

      RcDebugPrint (SDBG_SWEEP_L0, "**********Signal %a Group %d\n", GetSignalStr (SignalsToSweepDDR5[SignalIndex]), Group);

      CreateContextParameters (
        (COMMON_1D_PARAMETERS *)&PrivateDca,
        MemSsType,
        Socket,
        ChannelBitMask,
        0, // RankMask
        SubChannelParameter,
        DimmRank,
        Level,
        Group,
        SignalsToSweepDDR5[SignalIndex],
        StepSize,
        1,  // DitherValue
        -1, // OffsetLeft
        0,  // OffsetRight
        0,  // LimitOffset
        FlagsOffsets, FlagsResults, FlagsFsm
      );

      AllocatePoolForResults1D ((COMMON_1D_PARAMETERS *)&PrivateDca, &Results);

      ProcessSweep1D (
        (COMMON_1D_PARAMETERS *)&PrivateDca,
        PreSweepDcaTco1DComplex,
        SetupTestRmtDca,
        ExecuteTestDca,
        NULL,
        PostSweepDca1D,
        ResultsProcessingDca,
        &Results
      );

      UpdateDcaMarginResultsFromCurrentResults1D ((COMMON_1D_PARAMETERS *)&PrivateDca, &Results, SignalIndex, DcaResults);

      ReleaseResources1D ((COMMON_1D_PARAMETERS *)&PrivateDca, &Results);

    } // Signal loop ...

    if (TrainingMode == RUN_IN_PARALLEL) {
      break;
    }

  } // SubChannelIndex loop ...

  //
  // Turn on BCOM
  //
  EnableDisableBcom (Host, Socket, FALSE);

  //
  // Restore
  //
  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {
    if (!IsChannelEnabled (Socket, ChannelIndex)) {
      continue;
    }
    if (ChannelBitMask & (1 << ChannelIndex)) {
      continue;
    }

    RcdControlWordWrite (Socket, ChannelIndex, 1 << SUB_CH_A, DimmRank[ChannelIndex].Dimm, DimmRank[ChannelIndex].Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, RcdParityAlert[ChannelIndex].Data);
    RcdControlWordWrite (Socket, ChannelIndex, 1 << SUB_CH_A, DimmRank[ChannelIndex].Dimm, DimmRank[ChannelIndex].Rank, DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_REG, HostInterfaceTrainingMode[ChannelIndex].Data);

    if (Group == DcaDfeVref) {
      for (SubChannelIndex = 0; SubChannelIndex < SUB_CH; SubChannelIndex++) {
        RcdControlWordWrite (Socket, ChannelIndex, 1 << SubChannelIndex, DimmRank[ChannelIndex].Dimm, DimmRank[ChannelIndex].Rank, DDR5_RCD_DPAR_AND_DCA_6_0_DFE_TRAINING_MODE_REG, Ddr5RcdDparDcaDfeTrainingMode[ChannelIndex][SubChannelIndex].Data);
        RcdControlWordWrite (Socket, ChannelIndex, 1 << SubChannelIndex, DimmRank[ChannelIndex].Dimm, DimmRank[ChannelIndex].Rank, DDR5_RCD_DFE_CONFIGURATION_REG, Ddr5RcdDfeConfiguration[ChannelIndex][SubChannelIndex].Data);
      }
    }
  }

  return MRC_STATUS_SUCCESS;
} // GetDcaMargins

/**
  DCA Tco Coarse Training Algorithm

  @param[in, out] BestTcoResults   - Pointer to a Tco training results data structure

  @retval MRC_STATUS_SUCCESS
**/
MRC_STATUS
DcaTcoCoarseTraining (
  IN OUT SETTINGVALUE_EYEWIDTH_PAIR  (*BestTcoResults)[MAX_CH][SUB_CH]
)
{
  PSYSHOST             Host;
  UINT8                Socket;
  POWER_TRAIN_PARAM    Param;
  UINT8                Settings;
  MRC_STATUS           Status;
  INT16                (*PerParamTcoResults)[MAX_CH][SUB_CH];
  INT16                SettingValues[MAX_CH][SUB_CH];


  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;
  // Reuse power training parameter structure

  Param.ParamGsm = LinearCntlDcaTco;
  Status = GetPowerParameterSettings (Host, Socket, &Param);
  if (Status != MRC_STATUS_SUCCESS) {
    return Status;
  }

  PerParamTcoResults = RcAllocatePool (sizeof (*PerParamTcoResults));
  if (PerParamTcoResults == NULL) {
    return FAILURE;
  }

  //
  // Entering Early Command Clock Training Mode for DCPMM.
  // This will prevent the valley from decoding any commands. While training the DDRT2 CA bus
  //
  SetAepTrainingMode (Host, Socket, CHECKPOINT_MINOR_EARLY_CMD_CLK);

  for (Settings = 0; Settings < Param.NumSettings; Settings++) {
    RcDebugPrint (SDBG_MAX,
      "DcaTco Coarse Sweep Linear Setting %d (%d)\n",
      Param.SettingList[Settings],
      GetIpTcoSetting (Param.SettingList[Settings])
      );
    //
    // Set parameters for each Settings
    //
    SetParamPerSubCh (&Param, Settings);

    SetAllTcoSettingValues (Param.SettingList[Settings], &SettingValues);
    //
    // Initialize the per parameter results to 0
    //
    ZeroMem ((UINT8 *) (PerParamTcoResults), sizeof (*PerParamTcoResults));

    SweepDcaPerSubCh (PerParamTcoResults);

    UpdateBestTcoResults (&Param, &SettingValues, PerParamTcoResults, BestTcoResults);

  } //Settings

  SetTcoPerSubChToBestResults (&Param, BestTcoResults);

  //
  // Exiting Early Command Clock Training Mode for DCPMM.
  //
  SetAepTrainingMode (Host, Socket, DISABLE_TRAINING_STEP);

  RcFreePool (PerParamTcoResults);

  return MRC_STATUS_SUCCESS;
}

/**
  Copy Tco setting values from the input structure to an output structure of just setting values

  @param[in]  BestTcoResults   - Pointer to a Tco training results data structure
  @param[out] Settings         - Pointer the current Setting Values in the structure

  @retval None
**/
VOID
GetTcoSettingValues (
  IN      SETTINGVALUE_EYEWIDTH_PAIR  (*BestTcoResults)[MAX_CH][SUB_CH],
     OUT  INT16                       (*Settings)[MAX_CH][SUB_CH]
)
{
  PSYSHOST            Host;
  UINT8               Ch;
  UINT8               SubCh;
  UINT8               MaxChDdr;
  UINT8               Socket;

  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;
  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      (*Settings)[Ch][SubCh] = (*BestTcoResults)[Ch][SubCh].SettingValue;
    } //SubCh
  } //Ch
}

/**
  DCA Tco Fine Training Algorithm

  @param[in, out] BestTcoResults   - Pointer to a Dca Tco training results data structure

  @retval SUCCESS
**/
MRC_STATUS
DcaTcoFineTraining (
  IN OUT SETTINGVALUE_EYEWIDTH_PAIR  (*BestTcoResults)[MAX_CH][SUB_CH]
)
{
  PSYSHOST             Host;
  UINT8                Socket;
  INT16                InitialOffset = -4;
  INT16                OffsetAdjustment;
  INT16                SettingValues[MAX_CH][SUB_CH];
  UINT8                FineStepOffsetSize = 1;
  UINT8                Step;
  POWER_TRAIN_PARAM    Param;
  INT16                (*PerParamTcoResults)[MAX_CH][SUB_CH];
  MRC_STATUS           Status;


  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;
  // Reuse power training parameter structure

  Param.ParamGsm = LinearCntlDcaTco;
  Status = GetPowerParameterSettings (Host, Socket, &Param);
  if (Status != MRC_STATUS_SUCCESS) {
    return Status;
  }

  PerParamTcoResults = RcAllocatePool (sizeof (*PerParamTcoResults));
  if (PerParamTcoResults == NULL) {
    return FAILURE;
  }

  GetTcoSettingValues (BestTcoResults, &SettingValues);

  //
  // Entering Early Command Clock Training Mode for DCPMM.
  // This will prevent the valley from decoding any commands. While training the DDRT2 CA bus
  //
  SetAepTrainingMode (Host, Socket, CHECKPOINT_MINOR_EARLY_CMD_CLK);


  for (Step = 0; Step < NUM_TCO_FINE_SETTINGS; Step += FineStepOffsetSize) {
    RcDebugPrint (SDBG_MAX,
      "DcaTco Fine Sweep Offset %d\n", ((Step * FineStepOffsetSize) + InitialOffset));
    if (Step == 0) {
      //The algorithm starts from -4 from the existing position and sweeps forward to +3
      OffsetAdjustment = InitialOffset;
    } else {
      OffsetAdjustment = FineStepOffsetSize;
    }
    OffsetAllTcoSettingValues (Param.ParamGsm, OffsetAdjustment, &SettingValues);

    ApplyOffsetToParamPerSubCh (&Param, OffsetAdjustment);

    ZeroMem ((UINT8 *) (PerParamTcoResults), sizeof (*PerParamTcoResults));

    //The incoming offset of 0 was tested in coarse training, so only run test if
    //this condition is not true
    if (((Step * FineStepOffsetSize) + InitialOffset) != 0) {

      SweepDcaPerSubCh (PerParamTcoResults);

      UpdateBestTcoResults (&Param, &SettingValues, PerParamTcoResults, BestTcoResults);

    }

  }

  SetTcoPerSubChToBestResults (&Param, BestTcoResults);

  //
  // Exiting Early Command Clock Training Mode for DCPMM.
  //
  SetAepTrainingMode (Host, Socket, DISABLE_TRAINING_STEP);

  RcFreePool (PerParamTcoResults);

  return MRC_STATUS_SUCCESS;
}

/**
  Optimizes DCA TCO by margining DCA eye width at various settings for DCA TCO.

  @param[in]      Host  - Pointer to sysHost structure

  @retval SUCCESS
**/
UINT32
TrainDcaTco (
  IN     PSYSHOST Host
  )
{
  UINT8                       Socket;
  SETTINGVALUE_EYEWIDTH_PAIR  (*BestTcoResults)[MAX_CH][SUB_CH];

  Socket = Host->var.mem.currentSocket;

  //
  // Return if this socket is disabled
  // or if memory flow is disabled
  //
  if ((Host->nvram.mem.socket[Socket].enabled == 0)
      || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)
      || (!IsMemFlowEnabled (DcaTcoTraining))) {
    return SUCCESS;
  }

  //Only run for RDIMM and LRDIMM
  if (!CheckRCDLoopback (Host)) {
    return SUCCESS;
  }

  BestTcoResults = RcAllocatePool (sizeof (*BestTcoResults));
  if (BestTcoResults == NULL) {
    ASSERT (FALSE);
    return FAILURE;
  }

  ZeroMem ((UINT8 *) (BestTcoResults), sizeof (*BestTcoResults));

  DoCompDdrioPerSetupOption (Socket, TRAINING_COMP_EVERY_TRAINING);

  DcaTcoCoarseTraining (BestTcoResults);

  DcaTcoFineTraining (BestTcoResults);

  DisplayResultsDdr5 (Host, Socket, DcaTco);

  RcFreePool (BestTcoResults);
  return SUCCESS;

} // TrainTcoComp


/**

  Restore the QCS trained value for CTE

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket Number
  @param[in] Ch      - Ch Number
  @param[in] Dimm    - Dimm Number
  @param[in] Rank    - Rank Number

  @retval NA

**/
VOID
RestoreQcsCte (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
#ifdef USE_LATEST_RCD_SPEC
  struct dimmNvram                     (*DimmNvList) [MAX_DIMM];
  UINT8                                SubChannel = 0;
  UINT32                               QcsTrainedValue = 0;
  DDR5_RCD_QACS0_N_OUTPUT_DELAY_STRUCT Ddr5RcdQacs0;
  DDR5_RCD_QACS1_N_OUTPUT_DELAY_STRUCT Ddr5RcdQacs1;
  DDR5_RCD_QBCS0_N_OUTPUT_DELAY_STRUCT Ddr5RcdQbcs0;
  DDR5_RCD_QBCS1_N_OUTPUT_DELAY_STRUCT Ddr5RcdQbcs1;
  CHAR8                                *CteOptionQaCh0Sc0[4] = {"ch0q0acs0_trained_value", "ch0q0acs1_trained_value", "ch0q0acs2_trained_value", "ch0q0acs3_trained_value"};
  CHAR8                                *CteOptionQaCh0Sc1[4] = {"ch0q1acs0_trained_value", "ch0q1acs1_trained_value", "ch0q1acs2_trained_value", "ch0q1acs3_trained_value"};
  CHAR8                                *CteOptionQaCh1Sc0[4] = {"ch1q0acs0_trained_value", "ch1q0acs1_trained_value", "ch1q0acs2_trained_value", "ch1q0acs3_trained_value"};
  CHAR8                                *CteOptionQaCh1Sc1[4] = {"ch1q1acs0_trained_value", "ch1q1acs1_trained_value", "ch1q1acs2_trained_value", "ch1q1acs3_trained_value"};
  CHAR8                                *CteOptionQbCh0Sc0[4] = {"ch0q0bcs0_trained_value", "ch0q0bcs1_trained_value", "ch0q0bcs2_trained_value", "ch0q0bcs3_trained_value"};
  CHAR8                                *CteOptionQbCh0Sc1[4] = {"ch0q1bcs0_trained_value", "ch0q1bcs1_trained_value", "ch0q1bcs2_trained_value", "ch0q1bcs3_trained_value"};
  CHAR8                                *CteOptionQbCh1Sc0[4] = {"ch1q0bcs0_trained_value", "ch1q0bcs1_trained_value", "ch1q0bcs2_trained_value", "ch1q0bcs3_trained_value"};
  CHAR8                                *CteOptionQbCh1Sc1[4] = {"ch1q1bcs0_trained_value", "ch1q1bcs1_trained_value", "ch1q1bcs2_trained_value", "ch1q1bcs3_trained_value"};

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
    if (Ch == 0) {
      if (SubChannel == 0) {
        CteGetOptions (CteOptionQaCh0Sc0[ (Dimm << 1) + Rank], &QcsTrainedValue);
      } else {
        CteGetOptions (CteOptionQaCh0Sc1[ (Dimm << 1) + Rank], &QcsTrainedValue);
      }
    } else {
      if (SubChannel == 0) {
        CteGetOptions (CteOptionQaCh1Sc0[ (Dimm << 1) + Rank], &QcsTrainedValue);
      } else {
        CteGetOptions (CteOptionQaCh1Sc1[ (Dimm << 1) + Rank], &QcsTrainedValue);
      }
    }

    if (Rank == 0) {
      Ddr5RcdQacs0.Data = 0;
      Ddr5RcdQacs0.Bits.output_delay_feature_enable_for_qacs0_n = 1;
      Ddr5RcdQacs0.Bits.full_cycle_delay = (UINT8) QcsTrainedValue / 64;
      Ddr5RcdQacs0.Bits.output_delay_control_for_qacs0_n_1_2 = (UINT8) QcsTrainedValue % 64;
      RcdControlWordWrite (Socket, Ch, 1 << SubChannel, Dimm, Rank, DDR5_RCD_QACS0_N_OUTPUT_DELAY_REG, Ddr5RcdQacs0.Data);
    } else {
      Ddr5RcdQacs1.Data = 0;
      Ddr5RcdQacs1.Bits.output_delay_feature_enable_for_qacs1_n = 1;
      Ddr5RcdQacs1.Bits.full_cycle_delay = (UINT8) QcsTrainedValue / 64;
      Ddr5RcdQacs1.Bits.output_delay_control_for_qacs1_n = (UINT8) QcsTrainedValue % 64;
      RcdControlWordWrite (Socket, Ch, 1 << SubChannel, Dimm, Rank, DDR5_RCD_QACS1_N_OUTPUT_DELAY_REG, Ddr5RcdQacs1.Data);
    }

    if ((*DimmNvList) [Dimm].x4Present) {
      if (Ch == 0) {
        if (SubChannel == 0) {
          CteGetOptions (CteOptionQbCh0Sc0[ (Dimm << 1) + Rank], &QcsTrainedValue);

        } else {
          CteGetOptions (CteOptionQbCh0Sc1[ (Dimm << 1) + Rank], &QcsTrainedValue);

        }
      } else {
        if (SubChannel == 0) {
          CteGetOptions (CteOptionQbCh1Sc0[ (Dimm << 1) + Rank], &QcsTrainedValue);

        } else {
          CteGetOptions (CteOptionQbCh1Sc1[ (Dimm << 1) + Rank], &QcsTrainedValue);

        }
      }

      if (Rank == 0) {
        Ddr5RcdQbcs0.Data = 0;
        Ddr5RcdQbcs0.Bits.output_delay_feature_enable_for_qbcs0_n_output_signals = 1;
        Ddr5RcdQbcs0.Bits.full_cycle_delay = (UINT8) QcsTrainedValue / 64;
        Ddr5RcdQbcs0.Bits.output_delay_control_for_qbcs0_n_1_2 = (UINT8) QcsTrainedValue % 64;
        RcdControlWordWrite (Socket, Ch, 1 << SubChannel, Dimm, Rank, DDR5_RCD_QBCS0_N_OUTPUT_DELAY_REG, Ddr5RcdQbcs0.Data);
      } else {
        Ddr5RcdQbcs1.Data = 0;
        Ddr5RcdQbcs1.Bits.output_delay_feature_enable_for_qbcs1_n_output_signals = 1;
        Ddr5RcdQbcs1.Bits.full_cycle_delay = (UINT8) QcsTrainedValue / 64;
        Ddr5RcdQbcs1.Bits.output_delay_control_for_qbcs1_n = (UINT8) QcsTrainedValue % 64;
        RcdControlWordWrite (Socket, Ch, 1 << SubChannel, Dimm, Rank, DDR5_RCD_QBCS1_N_OUTPUT_DELAY_REG, Ddr5RcdQbcs1.Data);
      }
    }
  } //SubChannel Loop
#endif //USE_LATEST_RCD_SPEC
  return;
} // RestoreQcsCte

/**

  Trains backside command and address signals relative to clock

  @param Host  - Pointer to sysHost

  @retval Status

**/
UINT32
EarlyCaClkBackside (
  PSYSHOST  Host
  )
{
  UINT8                                Socket;
  UINT8                                Ch = 0;
  UINT8                                Dimm = 0;
  UINT8                                Rank = 0;
  UINT8                                RankEnabled = 0;
  struct dimmNvram                     (*DimmNvList) [MAX_DIMM];
  CHIP_ERLY_CMD_CK_STRUCT              ChipErlyCmdCKStruct;
  INT16                                PiDelay;
  struct channelNvram                  (*ChannelNvList) [MAX_CH];
  UINT8                                Status;
  UINT8                                MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ZeroMem ((UINT8 *)&ChipErlyCmdCKStruct, sizeof (ChipErlyCmdCKStruct));

  Socket = Host->var.mem.currentSocket;

  Host->var.mem.checkMappedOutRanks = 1;

  if (!IsMemFlowEnabled (CaClkTrainingBackSide)) {
    return SUCCESS;
  }

  if (!CheckRCDLoopback (Host)) {
    return SUCCESS;
  }
  //
  // Return if DDR5 MemSs is not present.
  //
  if (!IsDdr5Present (Host, Socket)) {
    return MRC_STATUS_FAILURE;
  }
  //
  // Return if this Socket is disabled
  //
  if (IsSocketDimmPopulated (Host, Socket) != TRUE) {
    return SUCCESS;
  }

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_EARLY_CA_CLK_BACKSIDE, Socket));

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {
    //
    // CTE 1N BFM WA.
    //
    ChannelNvList = GetChannelNvList (Host, Socket);
    //
    // Write PI delay
    //
    Status = GetSetSignalDdr5 (Host, 0, 0, DdrLevel, CS0_A_N,  GSM_READ_ONLY, &PiDelay);
    Status = GetSetSignalDdr5 (Host, 0, 0, DdrLevel, CS1_A_N,  GSM_FORCE_WRITE, &PiDelay);

    Status = GetSetSignalDdr5 (Host, 0, 0, DdrLevel, CS0_B_N,  GSM_READ_ONLY, &PiDelay);
    Status = GetSetSignalDdr5 (Host, 0, 0, DdrLevel, CS1_B_N,  GSM_FORCE_WRITE, &PiDelay);
  }
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    //
    // Turn on command/address output enable
    //
    TrnOnChipCMDADDOpEn (Host, Socket, Ch, &ChipErlyCmdCKStruct);
  } // Ch loop

  IO_Reset (Host, Socket);

  //
  // Execute RCD Loopback for RDIMM, LRDIMM, NVM DIMM configs
  //
  SetCurrentTestType (Socket, EarlyCaBacksideLoopbackTest);
  Host->var.mem.socket[Socket].TrainingStatus.Bits.TrainingPatternMode = QCA_TRAINING_PATTERN_SIMPLE;
  //
  // Shift the CS, CK by 1tck.
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

      RankEnabled = 0;

      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if ((*DimmNvList) [Dimm].dimmPresent == 0) {
          continue;
        }
        if (((*DimmNvList) [Dimm].DcpmmPresent) && ((*DimmNvList) [Dimm].fmcType == FMC_CWV_TYPE)) {
          continue;
        }
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (Rank >= (*DimmNvList) [Dimm].numRanks) {
          continue;
        }
        //
        // Init DRAM interface.
        //
        if (FeaturePcdGet (PcdCteBuild) == TRUE) {
          //
          // Restore the QCS trained value for CTE.
          //
          RestoreQcsCte (Host, Socket, Ch, Dimm,Rank);
          InitDramInterface (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, Rank);
        }
      }
    }
  }
  //
  // First, set the PDA enuemrate ID.
  //
  SetPdaEnumerateIdDdr5 (Host, Socket, CH_BITMASK);
  //
  // Do the DDRIO INIT before starting QCA CLK training.
  //
  if (FeaturePcdGet (PcdCteBuild) == TRUE) {
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      QcsQcaClkDdrioInit (Host, Socket, Dimm);
    }
  }
  //
  // Enter sense amp training Mode to all the channels here to latch results.
  //
  SetTrainingModeLatchResult (Host, Socket, CHECKPOINT_MINOR_SENSE_AMP);
  //
  // Set CA vref for each DRAM device.
  //
  EarlyCaClkVrefCABackside (Host);

  //
  // Get the CA CLK Timing Margin.
  //
  //EarlyCaClkTimingMarginBackside (Host);
  //
  // Disable CADB always on Mode
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += 1) {
    DisableCADBAlwaysOnMode (Socket, Ch);
  }
  //
  // Exit sense amp training Mode
  //
  SetTrainingModeLatchResult (Host, Socket, CHECKPOINT_MINOR_DISABLE_SENSE_AMP);
  Host->var.mem.socket[Socket].TrainingStatus.Bits.QcaUcaTrainingDone = 1;
  //
  // Program the CL for RDIMM/LRDIMM
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    //    ProgramCasLDdr5 (Host, Socket, Ch);
    //
    // Disable CADB Mode
    //
    SetCadbModeEn (Host, Socket, 1 << Ch, FALSE);
    //
    // Add some delay to make the cadb mode en effective.
    //
    TrainingDelay (Host, 5 * 16, 1);
  }
  Host->var.mem.checkMappedOutRanks = 0;
  SetCurrentTestType (Socket, 0);

  // Program DRAM MR after QCA delay settings
  BacksideDramInitAfterQca (Host, Socket, CH_BITMASK);

  // Program CAS Latency for DDRT2 for PBA enumerate
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }
      if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
        continue;
      }

      ProgramMr0Ddrt2 (Host, Socket, Ch, Dimm, 0);
    }
  }

  //
  // Indicate CMD training is done
  //
  Host->nvram.mem.socket[Host->var.mem.currentSocket].cmdClkTrainingDone = 1;

  return SUCCESS;
} // EarlyCaClkBackside

/**

Exectues early CA/clock training to set the best CA Vref.

@param Host - Pointer to sysHost

@retval N/A

**/
UINT32
EarlyCaClkVrefCA (
  PSYSHOST  Host
  )
{

  GSM_CSN                         Signaldecode;
  UINT8                           Ch = 0;
  UINT8                           RankEnabled;
  struct ClkTrainingResults       TrainRes[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT8                           Socket;
  UINT8                           Dimm;
  UINT8                           Rank;
  UINT8                           RankIndex;
  UINT8                           Signal;
  UINT8                           Index = 0;
  UINT8                           CaVrefStep;
  UINT8                           BestCaVrefStep;
  INT8                            CaVrefInitValue;
  INT8                            CaVrefValue;
  INT8                            BestCaVrefValue;
  UINT8                           SubChannel = 0;
  struct dimmNvram                (*DimmNvList) [MAX_DIMM];
  GSM_CSN                         SignalToSweepSubCh[MAX_CH][SUB_CH];
  GSM_CSEDGE_CA                   (*SignalEdgeData)[MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE_DDR5 / 2][DRAM_CA_SIGNALS_TO_SWEEP][CA_VREF_STEP_COUNT];
  UINT8                           Strobe = 0;
  UINT8                           SubChannelMask;
  UINT8                           MinEyeSignal[CA_VREF_STEP_COUNT];
  UINT16                          MinEyeWidth[CA_VREF_STEP_COUNT];
  UINT16                          MaxSumMinEyeWidth;
  UINT16                          MaxSumMinEyeWidthTemp;
  INT16                           Le[MAX_CH][SUB_CH][MAX_STROBE_DDR5/2];
  INT16                           Re[MAX_CH][SUB_CH][MAX_STROBE_DDR5/2];
  INT16                           EyeWidthSC[MAX_CH][SUB_CH][MAX_STROBE_DDR5/2];
  UINT8                           MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Socket = Host->var.mem.currentSocket;

  SignalEdgeData = RcAllocatePool (sizeof (*SignalEdgeData));
  if (SignalEdgeData == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return FAILURE;
  }
  ZeroMem ((UINT8 *) SignalEdgeData, sizeof (*SignalEdgeData));

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "EarlyCaClkVrefCA Starts\n");

  Index = 0;
  //
  // First, set the PDA enuemrate ID.
  //
  SetPdaEnumerateIdDdr5 (Host, Socket, CH_BITMASK);
  //
  // Test each Signal on every Rank on every Dimm on each Vref.
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

      RankEnabled = 0;

      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if (Rank >= (*DimmNvList) [Dimm].numRanks) {
          continue;
        }
        RankEnabled = 1;
        break;
      } // Ch loop

      //
      // Continue to the next Rank if this one is not populated
      //
      if (RankEnabled == 0) {
        continue;
      }
      //
      // Program the CA Vref value and CA training Mode.
      //
      for (CaVrefStep = 0; CaVrefStep < CA_VREF_STEP_COUNT; CaVrefStep++) {
        //
        // Program Ca Vref Value and DRAM Training Mode.
        //
        for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
          if (IsChannelEnabled (Socket, Ch) == FALSE) {
            continue;
          }
          SubChannelMask = 0;
          for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
            //
            // Check Subchannel is enabled or not. DDR5_TODO
            //
            SubChannelMask = SubChannelMask | (1 << SubChannel);
          } //sub channel loop

          CaVrefInitValue = GetCaVrefInitValue (Host, Socket, Ch, Dimm, Rank, CA_VREF_STEP_COUNT, CA_VREF_STEP_SIZE);
          CaVrefValue = CaVrefInitValue + (CaVrefStep * CA_VREF_STEP_SIZE);

          //
          //Set DRAM CA Vref Value.--DIMM sub channel A and B at the same time.
          //
          WriteVrefCa (Host, Socket, Ch, Dimm, SubChannelMask, Rank, ALL_STROBES, CaVrefValue, TIMING_MULTI_CS, TRUE); //program both Subchannel 0 and Subchannel 1
          WriteMPC (Host, Socket, Ch, Dimm, SubChannelMask, Rank, APPLY_VREFCA_RTT_CA_CS_CK, TIMING_4N, TRUE);
          RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
            "CaVrefValue 0x%x \n", CaVrefValue);
          //
          // Enter DRAM CA Training Mode.--DIMM sub channel A and B at the same time.
          //
          WriteMPC (Host, Socket, Ch, Dimm, SubChannelMask, Rank, Enter_CA_Training_Mode, TIMING_4N, TRUE);//DDR5_TODO. program both Subchannel 0 and Subchannel 1.
          RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
            "Enter CA Training Mode \n");
        } // Ch loop

        for (Signal = 0; Signal < DRAM_CA_SIGNALS_TO_SWEEP; Signal++) {

          Signaldecode = SignalsToSweepDDR5[Signal];

          for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
            //
            // initialize to no Signal
            //
            SignalToSweepSubCh[Ch][0] = NO_SIGNAL;
            SignalToSweepSubCh[Ch][1] = NO_SIGNAL;
            if (IsChannelEnabled (Socket, Ch) == FALSE) {
              continue;
            }
            for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
              Index = Signal + SubChannel*DRAM_CA_SIGNALS_TO_SWEEP; //DDR5_TODO : how to determin the Signal number based on the Dimm, Rank, Subchannel number.
              SignalToSweepSubCh[Ch][SubChannel] = SignalsToSweepDDR5[Index];
            } // SubChannel loop
          } // Ch loop

          //
          // Get CMD margins for current Rank and Signal
          //
          //
          // Init TrainRes.Clear the train result or it will impact the next loop test.
          //
          ZeroMem ((UINT8 *) TrainRes, sizeof (TrainRes));
          GetCaClkMarginsSweepVref (Host, Socket, Dimm, Rank, SignalToSweepSubCh, CaVrefStep, Le, Re, EyeWidthSC, &TrainRes);
          for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
            if (IsChannelEnabled (Socket, Ch) == FALSE) {
              continue;
            }

            for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
              if (SignalToSweepSubCh[Ch][SubChannel] == NO_SIGNAL) {
                continue;
              }

              RankIndex = GetRankIndex (Socket, Ch, Dimm, Rank);
              //
              // Save the results.
              //
              for (Strobe = 0; Strobe < MAX_STROBE_DDR5 / 2; Strobe++) {
                (*SignalEdgeData)[Ch][SubChannel][RankIndex][Strobe][Signal][CaVrefStep].signal = SignalToSweepSubCh[Ch][SubChannel];
                (*SignalEdgeData)[Ch][SubChannel][RankIndex][Strobe][Signal][CaVrefStep].le = Le[Ch][SubChannel][Strobe];
                (*SignalEdgeData)[Ch][SubChannel][RankIndex][Strobe][Signal][CaVrefStep].re = Re[Ch][SubChannel][Strobe];
                (*SignalEdgeData)[Ch][SubChannel][RankIndex][Strobe][Signal][CaVrefStep].EyeWidth = EyeWidthSC[Ch][SubChannel][Strobe];
                (*SignalEdgeData)[Ch][SubChannel][RankIndex][Strobe][Signal][CaVrefStep].toSample = 1;
              } // Strobe loop
            } // SubChannel Loop
          } // Ch loop

        } // Signal loop

      } // CaVrefStep loop
    } // Rank loop
  } // Dimm loop

  //
  // Determine the best CA vref.
  //
  RcDebugPrint (SDBG_MAX, "\nSummary: \n");
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) continue;
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      //DDR5_TODO--may need to check whether sub channel is enabled or not.
      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList) [Dimm].dimmPresent == 0) {
          continue;
        }

        for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          RankIndex = GetRankIndex (Socket, Ch, Dimm, Rank);
          CaVrefInitValue = GetCaVrefInitValue (Host, Socket, Ch, Dimm, Rank, CA_VREF_STEP_COUNT, CA_VREF_STEP_SIZE);

          for (Strobe = 0; Strobe < MAX_STROBE_DDR5 / 2; Strobe++) {
            if (IsStrobeNotValidDdr5 (Host, Strobe)) {
              continue;
            }

            for (CaVrefStep = 0; CaVrefStep < CA_VREF_STEP_COUNT; CaVrefStep++) {

              if ((*SignalEdgeData)[Ch][SubChannel][RankIndex][Strobe][0][CaVrefStep].toSample == 0) {
                continue;
              }

              CaVrefValue = CaVrefInitValue + (CaVrefStep * CA_VREF_STEP_SIZE);

              MinEyeWidth[CaVrefStep] = (*SignalEdgeData)[Ch][SubChannel][RankIndex][Strobe][0][CaVrefStep].EyeWidth;
              MinEyeSignal[CaVrefStep] = 0;
              RcDebugPrint (SDBG_MAX, "\n");
              for (Signal = 0; Signal < DRAM_CA_SIGNALS_TO_SWEEP; Signal++) {

                if ((*SignalEdgeData)[Ch][SubChannel][RankIndex][Strobe][Signal][CaVrefStep].toSample == 0) {
                  continue;
                }

                RcDebugPrintWithDevice (
                  SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
                  "DRAM Device %2d, CA Vref Value %3d, Signal %6a, eye width %3d, left %3d, right %3d \n",
                  Strobe,
                  CaVrefValue,
                  GetSignalStrDdr5 ((*SignalEdgeData)[Ch][SubChannel][RankIndex][Strobe][Signal][CaVrefStep].signal),
                  (*SignalEdgeData)[Ch][SubChannel][RankIndex][Strobe][Signal][CaVrefStep].EyeWidth,
                  (*SignalEdgeData)[Ch][SubChannel][RankIndex][Strobe][Signal][CaVrefStep].le,
                  (*SignalEdgeData)[Ch][SubChannel][RankIndex][Strobe][Signal][CaVrefStep].re
                  );

                if (MinEyeWidth[CaVrefStep] > (*SignalEdgeData)[Ch][SubChannel][RankIndex][Strobe][Signal][CaVrefStep].EyeWidth) {
                  MinEyeWidth[CaVrefStep] = (*SignalEdgeData)[Ch][SubChannel][RankIndex][Strobe][Signal][CaVrefStep].EyeWidth;
                  MinEyeSignal[CaVrefStep] = Signal;
                }
              } //Signal loop

              RcDebugPrint (SDBG_MAX, "\n");
              RcDebugPrintWithDevice (
                SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
                "DRAM device %2d, CA Vref Value %3d,  Min eye width is %3d, left %3d, right %3d, Min Eye Signal %a\n",
                Strobe,
                CaVrefValue,
                MinEyeWidth[CaVrefStep],
                (*SignalEdgeData)[Ch][SubChannel][RankIndex][Strobe][MinEyeSignal[CaVrefStep]][CaVrefStep].le,
                (*SignalEdgeData)[Ch][SubChannel][RankIndex][Strobe][MinEyeSignal[CaVrefStep]][CaVrefStep].re,
                GetSignalStrDdr5 ((*SignalEdgeData)[Ch][SubChannel][RankIndex][Strobe][MinEyeSignal[CaVrefStep]][CaVrefStep].signal)
                );

            } // CaVrefStep loop

            //
            // Calculate the maximum min eye width.
            //
            RcDebugPrint (SDBG_MAX, "\nVref Calculation Summary:\n");
            BestCaVrefStep = 0;
            BestCaVrefValue = CaVrefInitValue;
            MaxSumMinEyeWidth = 0;

            for (CaVrefStep = 0; CaVrefStep < CA_VREF_STEP_COUNT; CaVrefStep++) {

              if ((*SignalEdgeData)[Ch][SubChannel][RankIndex][Strobe][0][CaVrefStep].toSample == 0) {
                continue;
              }

              CaVrefValue = CaVrefInitValue + (CaVrefStep * CA_VREF_STEP_SIZE);

#if CA_VREF_STEP_COUNT == 1
              MaxSumMinEyeWidthTemp = (MinEyeWidth[CaVrefStep] +
                                       MinEyeWidth[CaVrefStep] +
                                       MinEyeWidth[CaVrefStep]);
#else
              if (CaVrefStep == 0) {
                MaxSumMinEyeWidthTemp = (MinEyeWidth[CaVrefStep] +
                                         MinEyeWidth[CaVrefStep] +
                                         MinEyeWidth[CaVrefStep + 1]);
              } else if ((CaVrefStep + 1) < CA_VREF_STEP_COUNT) {
                MaxSumMinEyeWidthTemp = (MinEyeWidth[CaVrefStep - 1] +
                                         MinEyeWidth[CaVrefStep] +
                                         MinEyeWidth[CaVrefStep + 1]);
              } else {
                MaxSumMinEyeWidthTemp = (MinEyeWidth[CaVrefStep - 1] +
                                         MinEyeWidth[CaVrefStep] +
                                         MinEyeWidth[CaVrefStep]);
              }
#endif
              RcDebugPrintWithDevice (
                SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
                "DRAM device %2d, CA Vref Value %3d,  Min eye width  %3d, Sum min eye width  %3d, left %3d, right %3d, Min Eye Signal %a\n",
                Strobe,
                CaVrefValue,
                MinEyeWidth[CaVrefStep],
                MaxSumMinEyeWidthTemp,
                (*SignalEdgeData)[Ch][SubChannel][RankIndex][Strobe][MinEyeSignal[CaVrefStep]][CaVrefStep].le,
                (*SignalEdgeData)[Ch][SubChannel][RankIndex][Strobe][MinEyeSignal[CaVrefStep]][CaVrefStep].re,
                GetSignalStrDdr5 ((*SignalEdgeData)[Ch][SubChannel][RankIndex][Strobe][MinEyeSignal[CaVrefStep]][CaVrefStep].signal)
                );

              if (MaxSumMinEyeWidthTemp > MaxSumMinEyeWidth) {
                MaxSumMinEyeWidth = MaxSumMinEyeWidthTemp;
                BestCaVrefStep = CaVrefStep;
                BestCaVrefValue = CaVrefValue;
              }

            } // CaVrefStep loop

            if ((*SignalEdgeData)[Ch][SubChannel][RankIndex][Strobe][0][BestCaVrefStep].toSample != 0) {
              RcDebugPrint (SDBG_MAX, "\n");
              RcDebugPrintWithDevice (
                SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
                "Final Vref Setting: DRAM device %2d, CA Vref Value %3d,  Max Min eye width is %3d, Max sum min eye width  %3d\n",
                Strobe,
                BestCaVrefValue,
                MinEyeWidth[BestCaVrefStep],
                MaxSumMinEyeWidth
                );
            }

            //
            // Set CA vref for each dram device.
            //
            //
            // Program the PDA select ID.
            //
            WriteMPC (Host, Socket, Ch, Dimm, 1 << SubChannel, Rank, PDA_SELECT_ID | Strobe, TIMING_4N, TRUE);//DDR5_TODO. SPR_TODO. To do two Subchannel one time in SPR.
            //
            // Program the DRAM's CaVref.
            //
            WriteVrefCa (Host, Socket, Ch, Dimm, 1 << SubChannel, Rank, Strobe, BestCaVrefValue, TIMING_MULTI_CS, TRUE);
            WriteMPC (Host, Socket, Ch, Dimm, 1 << SubChannel, Rank, APPLY_VREFCA_RTT_CA_CS_CK, TIMING_4N, TRUE);
            RcDebugPrint (SDBG_MAX, "\n");
          }//Strobe loop
        } //Rank loop
      } //Dimm loop
    } //sub channel loop
  } //Ch loop
  RcDebugPrint (SDBG_MAX, "\n");
  RcFreePool (SignalEdgeData);

  return SUCCESS;//DDR5_TODO.
}

/*

 Returns an index to access a backside CA signal in the SignalsToSweepBacksideDDR5[] array

 @param[in]   Channel     Current Channel under test (0-based)
 @param[in]   SubChannel  Current SubChannel under test (0-based)
 @param[in]   Dimm        Current Dimm under test (0-based)
 @param[in]   Rank        Current Rank under test (0-based)
 @param[in]   SignalIndex This is the logical index and MUST be between 0 to (BACKSIDE_CA_SIGNALS_TO_SWEEP - 1)

 @return      An index value to be used to access an element in the SignalsToSweepBacksideDDR5[] array

*/
UINT8
GetBacksideCaSignalIndex (
  IN        UINT8     Channel,
  IN        UINT8     SubChannel,
  IN        UINT8     Dimm,
  IN        UINT8     Rank,
  IN        UINT8     SignalIndex

  )
{
  return SignalIndex + SubChannel * BACKSIDE_CA_SIGNALS_TO_SWEEP; //DDR5_TODO : How to determin the Signal number based on the Dimm, Rank, Subchannel number.
}

/**

Exectues early CA/clock backside training to set the best DRAM CA Vref.

@param[in] Host - Pointer to sysHost

@retval N/A

**/
UINT32
EarlyCaClkVrefCABackside (
  IN PSYSHOST  Host
  )
{

  UINT8                           Ch;
  UINT8                           RankEnabled;
  struct ClkTrainingResults       TrainRes[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT8                           Socket;
  UINT8                           Dimm;
  UINT8                           Rank;
  UINT8                           Signal;
  UINT8                           Index = 0;
  INT8                            QcaVrefValue[MAX_CH];
  INT8                            QcaVrefIndex;
  UINT8                           SubChannel;
  struct dimmNvram (*DimmNvList)  [MAX_DIMM];
  GSM_CSN                         SignalToSweepSubCh[MAX_CH][SUB_CH];
  GSM_CSEDGE_CA                   (*SignalEdgeData) [MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE_DDR5 / 2][BACKSIDE_CA_SIGNALS_TO_SWEEP][QCA_VREF_STEP_COUNT];
  struct ddrRank (*RankList) [MAX_RANK_DIMM];
  UINT8                           Strobe = 0;
  UINT8                           SubChannelMask;
  INT16                           Le[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16                           Re[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16                           EyeWidthSC[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16                           LeCopyNum[MAX_CH][SUB_CH][COPY_NUM];
  INT16                           ReCopyNum[MAX_CH][SUB_CH][COPY_NUM];
  GSM_CSEDGE_CA                   (*SignalEdgeDataCopyNum) [MAX_CH][SUB_CH][MAX_RANK_CH][COPY_NUM][BACKSIDE_CA_SIGNALS_TO_SWEEP];
  UINT8                           BacksideCopy = 0;
  UINT8                           SignalToTrain = 0;
  UINT8                           MaxChDdr;
  UINT32                          ChannelBitMask = 0;
  MaxChDdr = GetMaxChDdr ();
  Socket = Host->var.mem.currentSocket;

  SignalEdgeData = RcAllocatePool (sizeof (*SignalEdgeData));
  if (SignalEdgeData == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return FAILURE;
  }
  ZeroMem ((UINT8 *) SignalEdgeData, sizeof (*SignalEdgeData));

  SignalEdgeDataCopyNum = RcAllocatePool (sizeof (*SignalEdgeDataCopyNum));
  if (SignalEdgeDataCopyNum == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return FAILURE;
  }

  ZeroMem ((UINT8 *) SignalEdgeDataCopyNum, sizeof (*SignalEdgeDataCopyNum));
  ZeroMem ((UINT8 *) EyeWidthSC, sizeof (EyeWidthSC));
  ZeroMem ((UINT8 *) LeCopyNum, sizeof (LeCopyNum));
  ZeroMem ((UINT8 *) ReCopyNum, sizeof (ReCopyNum));

  //
  // Initilaize Le and Re arrays to min and max values
  //
  for (Ch = 0; Ch < MAX_CH; Ch++) {
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      for (Strobe = 0; Strobe < MAX_STROBE_DDR5 / 2; Strobe++) {
        Le[Ch][SubChannel][Strobe] = -UNMARGINED_CMD_EDGE;
        Re[Ch][SubChannel][Strobe] = UNMARGINED_CMD_EDGE;
      } // Strobe
    } // SubChannel
  } //Ch

#ifdef DEBUG_CODE_BLOCK
  AcquirePrintControl ();
#endif // DEBUG_CODE_BLOCK
  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "EarlyCaClkVrefCABackside Starts\n");

  Index = 0;

  //
  // Test each Signal on every Rank on every Dimm on each Vref.
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

    QcsQcaClkDdrioInit (Host, Socket, Dimm);

    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

      //
      // Build a bit map of the channels with Rank enabled
      // Skipping training for channels without the Rank enabled
      //
      ChannelBitMask = 0;
      RankEnabled = 0;
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if ((*DimmNvList) [Dimm].dimmPresent == 0) {
          continue;
        }
        if (((*DimmNvList) [Dimm].DcpmmPresent) && ((*DimmNvList) [Dimm].fmcType == FMC_CWV_TYPE)) {
          continue;
        }
        if (Rank >= (*DimmNvList) [Dimm].numRanks) {
          continue;
        }
        ChannelBitMask |= (1 << Ch);
        RankEnabled = 1;
        ConfigQcaSampleSelection (Socket, Ch, Dimm, Rank);
      } // Ch loop

      //
      // Continue to the next Rank if this one is not populated
      //
      if (RankEnabled == 0) {
        continue;
      }
      //
      // Program the CA Vref value and CA training Mode.
      //
      for (QcaVrefIndex = 0; QcaVrefIndex < QCA_VREF_STEP_COUNT; QcaVrefIndex++) {
        //
        // Program Ca Vref Value and DRAM Training Mode.
        //
        ZeroMem ((UINT8 *)QcaVrefValue, sizeof (QcaVrefValue));
        for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
          if ((IsChannelEnabled (Socket, Ch) == FALSE) || (((1 << Ch) & ChannelBitMask) == 0)) {
            continue;
          }

          EnterRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);

          GetQcaVrefValueOdt (Socket, Ch, Dimm, &(QcaVrefValue[Ch]));
          QcaVrefValue[Ch] = QcaVrefValue[Ch] + (QcaVrefIndex * QCA_VREF_STEP_SIZE);

          if (QcaVrefIndex != 0) {
            //
            // Exit DRAM CA Training Mode.
            //
            SendNop (Host, Socket, Ch, 1 << SUB_CH_A, Dimm, Rank, 0, TRUE, TRUE);
            SendNop (Host, Socket, Ch, 1 << SUB_CH_B, Dimm, Rank, 0, TRUE, TRUE);
          }
          for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
            //
            // Check Subchannel is enabled or not. DDR5_TODO
            //
            SubChannelMask = 0;
            SubChannelMask = SubChannelMask | (1 << SubChannel);

            //
            // Parity checking must be disabled prior to and during pass-through Mode. It's disabled by default. So, need to make sure it's disabled after jedec reset.
            // Probably, the below code can be commented out. DDR5_TODO. to confirm?
            //
            // DDR5_TODO. Change it to use MRW command. It can use MRW command.
            //
            //
            // As the parity is disabled by default. No need to disable it separately.
            //
            //
            // Firstly, enable pass through Mode and then set DRAM CA Vref Value.--DIMM sub channel A and B at the same time.
            //
            WriteVrefCa (Host, Socket, Ch, Dimm, SubChannelMask, Rank, ALL_STROBES, QcaVrefValue[Ch], TIMING_MULTI_CS, TRUE); //program both Subchannel 0 and Subchannel 1
            WriteMPC (Host, Socket, Ch, Dimm, SubChannelMask, Rank, APPLY_VREFCA_RTT_CA_CS_CK, TIMING_4N, TRUE);
            RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
              "DRAM QcaVrefValue 0x%x \n", QcaVrefValue[Ch]);
            //
            // Enter DRAM CA Training Mode.--DIMM sub channel A and B at the same time.
            //
            WriteMPC (Host, Socket, Ch, Dimm, SubChannelMask, Rank, Enter_CA_Training_Mode, TIMING_4N, TRUE);//DDR5_TODO. program both Subchannel 0 and Subchannel 1. Support?
            RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
              "Enter DRAM CA Training Mode \n");
          } //sub channel loop
          //
          // Enter DRAM CA Training Mode.--DIMM sub channel A and B at the same time.
          //
          WriteMPC (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, Rank, Enter_CA_Training_Mode, TIMING_4N, TRUE);

        } //Ch loop.

        SignalToTrain = BACKSIDE_CA_SIGNALS_TO_SWEEP -1; //DDR5_todo.Toupdate the condition check.

        for (Signal = 0; Signal < SignalToTrain; Signal++) {
          for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
            //
            // initialize to no Signal
            //
            for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
              SignalToSweepSubCh[Ch][SubChannel] = NO_SIGNAL;
            }

            if ((IsChannelEnabled (Socket, Ch) == FALSE) || (((1 << Ch) & ChannelBitMask) == 0)) {
              continue;
            }

            for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
              Index = GetBacksideCaSignalIndex (Ch, SubChannel, Dimm, Rank, Signal);
              SignalToSweepSubCh[Ch][SubChannel] = SignalsToSweepBacksideDDR5[Index];
            } //Subchannel loop
          }//Ch loop

          //
          // Get CMD margins for current Rank and Signal
          //
          //
          // Init TrainRes.Clear the train result or it will impact the next loop test.
          //
          ZeroMem ((UINT8 *) TrainRes, sizeof (TrainRes));
          GetCaClkMarginsSweepVrefBackside (Host, Socket, Dimm, Rank, SignalToSweepSubCh, QcaVrefValue, Le, Re, EyeWidthSC, &TrainRes, LeCopyNum, ReCopyNum);
          for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
            if ((IsChannelEnabled (Socket, Ch) == FALSE) || (((1 << Ch) & ChannelBitMask) == 0)) {
              continue;
            }

            for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
              if (SignalToSweepSubCh[Ch][SubChannel] == NO_SIGNAL) {
                continue;
              }

              RankList = GetRankNvList (Host, Socket, Ch, Dimm);
              //
              // Save the results.
              //
              for (Strobe = 0; Strobe < MAX_STROBE_DDR5 / 2; Strobe++) {
                (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Strobe][Signal][QcaVrefIndex].signal = SignalToSweepSubCh[Ch][SubChannel];
                (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Strobe][Signal][QcaVrefIndex].le = Le[Ch][SubChannel][Strobe];
                (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Strobe][Signal][QcaVrefIndex].re = Re[Ch][SubChannel][Strobe];
                (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Strobe][Signal][QcaVrefIndex].EyeWidth = EyeWidthSC[Ch][SubChannel][Strobe];
                (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Strobe][Signal][QcaVrefIndex].toSample = 1;
              }//Strobe loop
              for (BacksideCopy = 0; BacksideCopy < COPY_NUM; BacksideCopy++) {
                //
                // save the results.
                //
                (*SignalEdgeDataCopyNum) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][BacksideCopy][Signal].signal = SignalToSweepSubCh[Ch][SubChannel] + (QCA_DISTANCE_A_B_COPY * BacksideCopy);
                (*SignalEdgeDataCopyNum) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][BacksideCopy][Signal].le = LeCopyNum[Ch][SubChannel][BacksideCopy];
                (*SignalEdgeDataCopyNum) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][BacksideCopy][Signal].re = ReCopyNum[Ch][SubChannel][BacksideCopy];
                (*SignalEdgeDataCopyNum) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][BacksideCopy][Signal].toSample = 1;
              }
            } //SubChannel Loop
          } // Ch loop
        }//Signal loop
      } // QcaVrefIndex loop

      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if ((IsChannelEnabled (Socket, Ch) == FALSE) || (((1 << Ch) & ChannelBitMask) == 0)) {
          continue;
        }

        //
        // Exit DRAM CA Training Mode.
        //
        SendNop (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, 0, TRUE, TRUE);

        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
          "Exit Backside DRAM CA Training Mode \n");

      } //Ch loop

      //
      // Exit CA Pass through Mode.
      //
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if ((IsChannelEnabled (Socket, Ch) == FALSE) || (((1 << Ch) & ChannelBitMask) == 0)) {
          continue;
        }

        ExitRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);
      } //Ch loop
    } // Rank loop
  }// Dimm loop

  RcDebugPrint (SDBG_MAX, "\n");

  //
  // clear to insure we take the right path for setting final PI values
  //
  Host->var.mem.earlyCaClkExecuted = 0;
  //
  // Set the timing margin data.
  //
  SetCombinedCaGroupDdr5 (Host, Socket, Ch, SubChannel, DdrLevel, GSM_COMBINE_EDGES, CMDCTL_SIGNAL_DELAY, BACKSIDE_CA_SIGNALS_TO_SWEEP,
    SignalEdgeDataCopyNum, sizeof (*SignalEdgeDataCopyNum));
  //
  // Set to indicate early CA CLK training has executed
  //
  Host->var.mem.earlyCaClkExecuted = 1;
  // Execute JEDEC reset/init sequence
  FifoTrainReset (Host, Socket);
  IO_Reset (Host, Socket);

#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    DisplayCCCResults (Host, Socket);
  }
  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK

#ifdef SPR_PO_FLAG
  ExitCaTrainingModeDdr5 (Host, Socket);
#endif //SPR_PO_FLAG

  Host->var.mem.checkMappedOutRanks = 0;
  RcFreePool (SignalEdgeData);
  RcFreePool (SignalEdgeDataCopyNum);
  return SUCCESS;
} //EarlyCaClkVrefCABackside

/**

Exectues early CA/clock training to set the best CA Vref for RDIMM.

@param Host - Pointer to sysHost

@retval N/A

**/
UINT32
EarlyCaClkRcdVrefCA (
  PSYSHOST  Host,
  UINT8     TrainingMode
  )
{
  UINT8                           Ch = 0;
  UINT8                           RankEnabled;
  struct  ClkTrainingResults      TrainRes[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT8                           Socket;
  UINT8                           Dimm;
  UINT8                           Rank;
  UINT8                           RankIndex;
  MRC_MST                         MemSsType;
  UINT8                           Signal;
  UINT8                           Index = 0;
  UINT8                           CaVrefStep;
  UINT8                           BestCaVrefStep;
  INT8                            CaVrefInitValue;
  INT8                            CaVrefValue;
  CHANNEL_NVRAM_STRUCT            (*ChannelNvList)[MAX_CH];
  UINT8                           SubChannel = 0;
  struct dimmNvram                (*DimmNvList)[MAX_DIMM];
  GSM_CSN                         SignalToSweepCh;
  GSM_CSEDGE_CA                   (*SignalEdgeData)[MAX_CH][SUB_CH][MAX_RANK_CH][RCD_CA_SIGNALS_TO_SWEEP][CA_VREF_STEP_COUNT];
  UINT16                          MaxEyeWidthSum = 0;
  UINT16                          MaxSumEyeWidthTemp = 0;
  INT8                            MaxEyeVref[MAX_CH][SUB_CH][MAX_RANK_CH][RCD_CA_SIGNALS_TO_SWEEP];
  INT16                           Le[MAX_CH][SUB_CH];
  INT16                           Re[MAX_CH][SUB_CH];
  UINT8                           SubChannelMask = 0;
  UINT8                           SubChTemp = 0;
  UINT8                           MaxChDdr;
  UINT8                           Patternloop = 0;
  UINT8                           PatternLoopEnd;
  BOOLEAN                         FailSignal = FALSE;

  MaxChDdr = GetMaxChDdr ();
  Socket = Host->var.mem.currentSocket;
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

   if (Host->var.mem.socket[Socket].TrainingStatus.Bits.TrainingPatternMode == CA_TRAINING_PATTERN_XTALK) {
     PatternLoopEnd = PATTERN_COMPLEX_LOOP_CNT;
   } else {
     PatternLoopEnd = PATTERN_LOOP_CNT;
   }

  SignalEdgeData = RcAllocatePool (sizeof (*SignalEdgeData));
  if (SignalEdgeData == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return FAILURE;
  }
  ZeroMem ((UINT8 *) SignalEdgeData, sizeof (*SignalEdgeData));

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "EarlyCaClkRcdVrefCA Starts\n");

  ZeroMem ((UINT8 *) MaxEyeVref, sizeof (MaxEyeVref));
  ZeroMem ((UINT8 *) TrainRes, sizeof (TrainRes));
  SetAepTrainingMode (Host, Socket, CHECKPOINT_MINOR_EARLY_CMD_CLK);
  Index = 0;
  if (TrainingMode == RUN_IN_PARALLEL) {
    SubChannelMask = TWO_SUBCH_MASK;
  }
  //
  // Test each Signal on every Rank on every Dimm on each Vref.
  //
  for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
    if ((TrainingMode == RUN_IN_PARALLEL) && (SubChannel == SUB_CH_B)) {
      continue;
    }
    if (TrainingMode != RUN_IN_PARALLEL) {
      SubChannelMask = 1 << SubChannel;
    }
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (Dimm == 1) {
        for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
          if (IsChannelEnabled (Socket, Ch) == FALSE) {
            continue;
          }
          DimmNvList = GetDimmNvList (Host, Socket, Ch);
          if ((*DimmNvList) [Dimm].dimmPresent == 0) {
            continue;
          }
          SetCaTrainingModeDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, 0, 0, FALSE, Rising_Edge);
        }
      }
      for (Signal = 0; Signal < RCD_CA_SIGNALS_TO_SWEEP; Signal++) {
        for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

          RankEnabled = 0;

          for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
            if (IsChannelEnabled (Socket, Ch) == FALSE) {
              continue;
            }
            if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
              continue;
            }
            DimmNvList = GetDimmNvList (Host, Socket, Ch);
            if (Rank >= (*DimmNvList) [Dimm].numRanks) {
              continue;
            }
            RankEnabled = 1;
            break;
          } // Ch loop

          //
          // Continue to the next Rank if this one is not populated
          //
          if (RankEnabled == 0) {
            continue;
          }

          //
          // Get CA CLK margins for current Rank and Signal
          //
          for (CaVrefStep = 0; CaVrefStep < CA_VREF_STEP_COUNT; CaVrefStep++) {
            for (Patternloop = 0; Patternloop < PatternLoopEnd; Patternloop++) {
              //
              // Get CA CLK margins for current Rank and Signal
              //
              if (SubChannel == 0) {
                Index = Signal;
              } else {
                Index = Signal + ((RCD_CA_SIGNALS_TO_SWEEP * 2) - 1);
              }
              SignalToSweepCh = SignalsToSweepDDR5[Index];

              for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
                if (IsChannelEnabled (Socket, Ch) == FALSE) {
                  continue;
                }

                if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
                  continue;
                }

                DimmNvList = GetDimmNvList (Host, Socket, Ch);
                if (Rank >= (*DimmNvList) [Dimm].numRanks) {
                  continue;
                }

                CaVrefInitValue = GetCaVrefInitValue (Host, Socket, Ch, Dimm, Rank, CA_VREF_STEP_COUNT, CA_VREF_STEP_SIZE);
                CaVrefValue = CaVrefInitValue + (CaVrefStep * CA_VREF_STEP_SIZE);

                if (Rank == 0) {

                  //
                  // Program each Signal CA vref value.
                  //
                  ProgramSignalCaVref (Host, Socket, Ch, Dimm, SubChannel, SignalToSweepCh, CaVrefValue);

                  if (TrainingMode == RUN_IN_PARALLEL) {
                    ProgramSignalCaVref (Host, Socket, Ch, Dimm, SUB_CH_B, SignalToSweepCh, CaVrefValue);
                  }

                  RcDebugPrintWithDevice (
                    SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
                    "ProgramSignalCaVref for %a --CA Vref %d\n",
                    GetSignalStrDdr5 (SignalToSweepCh),
                    CaVrefValue
                    );

                  //
                  // Enter the DCA training Mode.
                  //
                  SetCaTrainingModeDdr5 (Host, Socket, Ch, SubChannelMask, Dimm, Rank, TRUE, Rising_Edge); //DDR5_TODO.

                } else { // Rank != 0
                  RankIndex = GetRankIndex (Socket, Ch, Dimm, 0);

                  for (SubChTemp = 0; SubChTemp < SUB_CH; SubChTemp++) {
                    if ((SubChannelMask & (1 << SubChTemp)) == 0) {
                      continue;
                    }
                    Le[Ch][SubChTemp] = (*SignalEdgeData) [Ch][SubChTemp][RankIndex][Signal][CaVrefStep] .le;
                    Re[Ch][SubChTemp] = (*SignalEdgeData) [Ch][SubChTemp][RankIndex][Signal][CaVrefStep] .re;
                  } //SubChTemp

                }
              }

              if (Rank == 0) {
                if (Host->var.mem.socket[Socket].TrainingStatus.Bits.TrainingPatternMode == CA_TRAINING_PATTERN_SIMPLE) {
                  //
                  // Get the timing margin for the given Signal with the specified vref.
                  //
                  RcDebugPrintWithDevice (
                    SDBG_MAX, Socket, NO_CH, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
                    "Calling GetCaClkRcdMarginsSweepVref for %a\n",
                    GetSignalStrDdr5 (SignalToSweepCh)
                    );
                  GetCaClkRcdMarginsSweepVref (Host, Socket, MemSsType, ALL_CH, Dimm, SubChannel, Rank, SignalToSweepCh, Le, Re, TrainingMode, 0, 0, Patternloop);
                } else {
                  GetDcaComplexMargins (
                    Host,
                    Socket,
                    0,
                    SubChannel,
                    Dimm,
                    Rank,
                    DcaDelay,
                    Signal,
                    TRUE,
                    ResultsRelative | SingleBitTrack,
                    Le,
                    Re
                  );
                }
              }

              if (SignalToSweepCh == NO_SIGNAL) {
                continue;
              }
              for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
                if (IsChannelEnabled (Socket, Ch) == FALSE) {
                  continue;
                }

                RankIndex = GetRankIndex (Socket, Ch, Dimm, Rank);

                for (SubChTemp = 0; SubChTemp < SUB_CH; SubChTemp++) {
                  if ((SubChannelMask & (1 << SubChTemp)) == 0) {
                    continue;
                  }
                  //
                  // Save the results.
                  //
                  if (SubChannelMask == TWO_SUBCH_MASK) {
                    (*SignalEdgeData) [Ch][SubChTemp][RankIndex][Signal][CaVrefStep] .signal = SignalToSweepCh + SubChTemp * ((RCD_CA_SIGNALS_TO_SWEEP*2) - 1);
                  } else {
                    (*SignalEdgeData) [Ch][SubChTemp][RankIndex][Signal][CaVrefStep] .signal = SignalToSweepCh;
                  }
                  FailSignal = FALSE;
                  if (Patternloop == SECOND_ITERATION_LOOP) {
                    if (((*SignalEdgeData) [Ch][SubChTemp][RankIndex][Signal][CaVrefStep] .le == (*SignalEdgeData) [Ch][SubChTemp][RankIndex][Signal][CaVrefStep] .re) ||
                        (Le[Ch][SubChTemp] == Re[Ch][SubChTemp])) {
                      (*SignalEdgeData) [Ch][SubChTemp][RankIndex][Signal][CaVrefStep] .EyeWidth = 0;
                      FailSignal = TRUE;
                      RcDebugPrintWithDevice (
                        SDBG_MEM_TRAIN, Socket, Ch, Dimm, SubChTemp, NO_RANK, NO_STROBE, NO_BIT,
                        "%a Left edge == Right edge, eye is too small\n", GetSignalStrDdr5 ((*SignalEdgeData) [Ch][SubChTemp][RankIndex][Signal][CaVrefStep].signal));

                    } else {
                      if ((*SignalEdgeData) [Ch][SubChTemp][RankIndex][Signal][CaVrefStep] .le < Le[Ch][SubChTemp]) {
                        (*SignalEdgeData) [Ch][SubChTemp][RankIndex][Signal][CaVrefStep] .le = Le[Ch][SubChTemp];
                      }
                      if ((*SignalEdgeData) [Ch][SubChTemp][RankIndex][Signal][CaVrefStep] .re > Re[Ch][SubChTemp] ) {
                        (*SignalEdgeData) [Ch][SubChTemp][RankIndex][Signal][CaVrefStep] .re = Re[Ch][SubChTemp];
                      }
                      RcDebugPrintWithDevice (
                        SDBG_MEM_TRAIN, Socket, Ch, Dimm, SubChTemp, NO_RANK, NO_STROBE, NO_BIT,
                        "%a  Composite Left Edge %d  Composite Right Edge %d\n",
                        GetSignalStrDdr5 ((*SignalEdgeData) [Ch][SubChTemp][RankIndex][Signal][CaVrefStep].signal),
                        (*SignalEdgeData) [Ch][SubChTemp][RankIndex][Signal][CaVrefStep] .le,
                        (*SignalEdgeData) [Ch][SubChTemp][RankIndex][Signal][CaVrefStep] .re
                        );
                    }
                  } else {
                    (*SignalEdgeData) [Ch][SubChTemp][RankIndex][Signal][CaVrefStep] .le = Le[Ch][SubChTemp];
                    (*SignalEdgeData) [Ch][SubChTemp][RankIndex][Signal][CaVrefStep] .re = Re[Ch][SubChTemp];
                    (*SignalEdgeData) [Ch][SubChTemp][RankIndex][Signal][CaVrefStep] .EyeWidth = ABS (Re[Ch][SubChTemp] - Le[Ch][SubChTemp]);
                  }
                  if (FailSignal == TRUE) {
                    (*SignalEdgeData) [Ch][SubChTemp][RankIndex][Signal][CaVrefStep].EyeWidth = 0;
                  } else {
                    (*SignalEdgeData) [Ch][SubChTemp][RankIndex][Signal][CaVrefStep].EyeWidth = ABS ((*SignalEdgeData) [Ch][SubChTemp][RankIndex][Signal][CaVrefStep].re -
                                                                                                   (*SignalEdgeData) [Ch][SubChTemp][RankIndex][Signal][CaVrefStep].le);
                  }
                  (*SignalEdgeData) [Ch][SubChTemp][RankIndex][Signal][CaVrefStep].toSample = 1;
                } // SubChTemp loop
                if (FeaturePcdGet (PcdCteBuild) == TRUE) {
                  //
                  // Exit the CA CLK training Mode.
                  //
                  if ((Signal == 6) && (Rank == 0)) {
                    SetCaTrainingModeDdr5 (Host, Socket, Ch, SubChannelMask, Dimm, Rank, FALSE, Rising_Edge);
                  }
                }

                if (Rank == 0) {
                  RcDebugPrint (
                    SDBG_MAX,
                    "Restore CA VREF\n"
                    );

                  if (SubChannel == 0) {
                    Index = Signal;
                  } else {
                    Index = Signal + ((RCD_CA_SIGNALS_TO_SWEEP * 2) - 1);
                  }
                  SignalToSweepCh = SignalsToSweepDDR5[Index];
                  ChannelNvList = GetChannelNvList (Host, Socket);
                  CaVrefInitValue = (*ChannelNvList)[Ch].DcaVrefRegValue[RankIndex];
                  //
                  // Program each Signal CA vref value.
                  //
                  ProgramSignalCaVref (Host, Socket, Ch, Dimm, SubChannel, SignalToSweepCh, CaVrefInitValue);
                }
              } // Ch Loop
            } //Pattern loop
          } // CaVrefStep loop
        }// Rank loop
      }// Signal loop
    } // Dimm loop
  } // Subchannel loop

  //
  // For each Signal, find its CA vref value with the best timing margin.
  //
  RcDebugPrint (SDBG_MAX, "\nSummary: \n");
  for (Ch = 0; Ch < MaxChDdr;Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList) [Dimm].dimmPresent == 0) {
        continue;
      }
      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          //
          //skip if Rank > 0. DDR5_TODO. to check whether it's ok.
          //
          if (Rank > 0) {
            continue;
          }

          RankIndex = GetRankIndex (Socket, Ch, Dimm, Rank);
          CaVrefInitValue = GetCaVrefInitValue (Host, Socket, Ch, Dimm, Rank, CA_VREF_STEP_COUNT, CA_VREF_STEP_SIZE);

          for (Signal = 0; Signal < RCD_CA_SIGNALS_TO_SWEEP; Signal++) {

            BestCaVrefStep = 0;
            MaxEyeVref[Ch][SubChannel][RankIndex][Signal] = CaVrefInitValue;
            MaxEyeWidthSum = 0;

            for (CaVrefStep = 0; CaVrefStep < CA_VREF_STEP_COUNT; CaVrefStep++) {

              if ((*SignalEdgeData)[Ch][SubChannel][RankIndex][0][CaVrefStep].toSample == 0) {
                continue;
              }

#if CA_VREF_STEP_COUNT == 1
              MaxSumEyeWidthTemp = ((*SignalEdgeData)[Ch][SubChannel][RankIndex][Signal][CaVrefStep].EyeWidth +
                                    (*SignalEdgeData)[Ch][SubChannel][RankIndex][Signal][CaVrefStep].EyeWidth +
                                    (*SignalEdgeData)[Ch][SubChannel][RankIndex][Signal][CaVrefStep].EyeWidth);
#else
              if (CaVrefStep == 0) {
                MaxSumEyeWidthTemp = ((*SignalEdgeData)[Ch][SubChannel][RankIndex][Signal][CaVrefStep].EyeWidth +
                                      (*SignalEdgeData)[Ch][SubChannel][RankIndex][Signal][CaVrefStep].EyeWidth +
                                      (*SignalEdgeData)[Ch][SubChannel][RankIndex][Signal][CaVrefStep + 1].EyeWidth);
              } else if ((CaVrefStep + 1) < CA_VREF_STEP_COUNT) {
                MaxSumEyeWidthTemp = ((*SignalEdgeData)[Ch][SubChannel][RankIndex][Signal][CaVrefStep - 1].EyeWidth +
                                      (*SignalEdgeData)[Ch][SubChannel][RankIndex][Signal][CaVrefStep].EyeWidth +
                                      (*SignalEdgeData)[Ch][SubChannel][RankIndex][Signal][CaVrefStep + 1].EyeWidth);
              } else {
                MaxSumEyeWidthTemp = ((*SignalEdgeData)[Ch][SubChannel][RankIndex][Signal][CaVrefStep - 1].EyeWidth +
                                      (*SignalEdgeData)[Ch][SubChannel][RankIndex][Signal][CaVrefStep].EyeWidth +
                                      (*SignalEdgeData)[Ch][SubChannel][RankIndex][Signal][CaVrefStep].EyeWidth);
              }
#endif
              CaVrefValue = CaVrefInitValue + (CaVrefStep * CA_VREF_STEP_SIZE);

              RcDebugPrintWithDevice (
                SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
                " Signal %6a, CA Vref Value %3d, Eye width %3d, Eye width Sum %3d, left %3d, right %3d\n",
                GetSignalStrDdr5 ((*SignalEdgeData)[Ch][SubChannel][RankIndex][Signal][CaVrefStep].signal),
                CaVrefValue,
                (*SignalEdgeData)[Ch][SubChannel][RankIndex][Signal][CaVrefStep].EyeWidth,
                MaxSumEyeWidthTemp,
                (*SignalEdgeData)[Ch][SubChannel][RankIndex][Signal][CaVrefStep].le,
                (*SignalEdgeData)[Ch][SubChannel][RankIndex][Signal][CaVrefStep].re
                );

              if (MaxSumEyeWidthTemp > MaxEyeWidthSum) {
                MaxEyeWidthSum = MaxSumEyeWidthTemp;
                BestCaVrefStep = CaVrefStep;
                MaxEyeVref[Ch][SubChannel][RankIndex][Signal] = CaVrefValue;
              }

            } // CaVrefStep loop

            RcDebugPrint (SDBG_MAX, "\n");
            RcDebugPrintWithDevice (
              SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
              "Signal %a, Max eye width Sum is %3d, Best CA Vref Value %3d, left %3d, right %3d\n\n",
              GetSignalStrDdr5 ((*SignalEdgeData)[Ch][SubChannel][RankIndex][Signal][BestCaVrefStep].signal),
              MaxEyeWidthSum,
              MaxEyeVref[Ch][SubChannel][RankIndex][Signal],
              (*SignalEdgeData)[Ch][SubChannel][RankIndex][Signal][BestCaVrefStep].le,
              (*SignalEdgeData)[Ch][SubChannel][RankIndex][Signal][BestCaVrefStep].re
              );

            //
            // Program the Vref Result.
            //
            ProgramSignalCaVref (Host, Socket, Ch, Dimm, SubChannel, (*SignalEdgeData)[Ch][SubChannel][RankIndex][Signal][BestCaVrefStep].signal, MaxEyeVref[Ch][SubChannel][RankIndex][Signal]);

          } // Signal loop
        } // Rank loop
      } // SubChannel loop
    } // Dimm loop
  } // Ch loop

  SetAepTrainingMode (Host, Socket, DISABLE_TRAINING_STEP);
  RcFreePool (SignalEdgeData);
  return SUCCESS;
}



/**

Exectues early CA/clock training to get/set the timing margin for UDIMM.

@param Host - Pointer to sysHost

@retval N/A

**/
UINT32
EarlyCaClkTimingMargin (
  PSYSHOST  Host
  )
{

  GSM_CSN                             Signaldecode;
  UINT8                               Ch = 0;
  UINT8                               RankEnabled;
  struct  ClkTrainingResults          TrainRes[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT8                               Socket;
  UINT8                               Dimm;
  UINT8                               Rank;
  UINT8                               Signal;
  UINT8                               Index = 0;
  UINT8                               SubChannel = 0;
  UINT32                              PrevDebugLevel;
  struct dimmNvram                    (*DimmNvList) [MAX_DIMM];
  GSM_CSN                             SignalToSweepSubCh[MAX_CH][SUB_CH];
  GSM_CSEDGE_CA                       (*SignalEdgeData) [MAX_CH][SUB_CH][MAX_RANK_CH][DRAM_CA_SIGNALS_TO_SWEEP];
  struct ddrRank                      (*RankList) [MAX_RANK_DIMM];
  INT16                               Le[MAX_CH][SUB_CH];
  INT16                               Re[MAX_CH][SUB_CH];
  UINT8                               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host->var.mem.checkMappedOutRanks = 1;
  Socket = Host->var.mem.currentSocket;

  //
  // Return if this Socket is disabled
  //
  if (IsSocketDimmPopulated (Host, Socket) != TRUE) {
    return SUCCESS;
  }

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_EARLY_CA_CLK, Socket));

  SignalEdgeData = RcAllocatePool (sizeof (*SignalEdgeData));
  if (SignalEdgeData == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return FAILURE;
  }
  ZeroMem ((UINT8 *) SignalEdgeData, sizeof (*SignalEdgeData));
#ifdef DEBUG_CODE_BLOCK
  AcquirePrintControl ();
#endif // DEBUG_CODE_BLOCK

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Early Ca Clk Timing Margin Starts\n");

  Index = 0;

  //
  // Test each Signal on every Rank on every Dimm on each Vref.
  //
  for (Signal = 0; Signal < DRAM_CA_SIGNALS_TO_SWEEP; Signal++) {
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      Signaldecode = SignalsToSweepDDR5[Signal];
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        //
        // initialize to no Signal
        //
        SignalToSweepSubCh[Ch][0] = NO_SIGNAL;
        SignalToSweepSubCh[Ch][1] = NO_SIGNAL;
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }
        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          Index = Signal + SubChannel*DRAM_CA_SIGNALS_TO_SWEEP; //DDR5_TODO : how to determin the Signal number based on the Dimm, Rank, Subchannel number.
          SignalToSweepSubCh[Ch][SubChannel] = SignalsToSweepDDR5[Index];
        } //Subchannel loop
      }//Ch loop

      for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

        RankEnabled = 0;

        for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
          if (IsChannelEnabled (Socket, Ch) == FALSE) {
            continue;
          }
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          DimmNvList = GetDimmNvList (Host, Socket, Ch);
          if (Rank >= (*DimmNvList) [Dimm].numRanks) {
            continue;
          }
          RankEnabled = 1;
          break;
        } // Ch loop

        //
        // Continue to the next Rank if this one is not populated
        //
        if (RankEnabled == 0) {
          continue;
        }

        //
        // Get CMD margins for current Rank and Signal
        //
        //
        // Init TrainRes.Clear the train result or it will impact the next loop test. //can move the trainres to the GetCaClkMarginsSweepTiming internal function.
        //
        ZeroMem ((UINT8 *) TrainRes, sizeof (TrainRes));
        GetCaClkMarginsSweepTiming (Host, Socket, Dimm, Rank, SignalToSweepSubCh, Le, Re, &TrainRes);

        for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
          if (IsChannelEnabled (Socket, Ch) == FALSE) {
            continue;
          }

          for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
            if (SignalToSweepSubCh[Ch][SubChannel] == NO_SIGNAL) {
              continue;
            }

            RankList = GetRankNvList (Host, Socket, Ch, Dimm);
            if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
              continue;
            }
            DimmNvList = GetDimmNvList (Host, Socket, Ch);
            if (Rank >= (*DimmNvList) [Dimm].numRanks) {
              continue;
            }
            //
            // save the results.
            //
            (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Signal].signal = SignalToSweepSubCh[Ch][SubChannel];
            (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Signal].le = Le[Ch][SubChannel];
            (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Signal].re = Re[Ch][SubChannel];
            (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Signal].toSample = 1;
          } //SubChannel Loop
        } // Ch loop
      } // Rank loop
    } // Dimm loop

    RcDebugPrint (SDBG_MAX, "\n");
  } // Signal loop

  //
  // clear to insure we take the right path for setting final PI values
  //
  Host->var.mem.earlyCaClkExecuted = 0;
  //
  // Set the timing margin data.
  //
  SetCombinedCaGroupDdr5 (Host, Socket, Ch, SubChannel, DdrLevel, GSM_COMBINE_EDGES, CMDCTL_SIGNAL_DELAY, DRAM_CA_SIGNALS_TO_SWEEP,
    SignalEdgeData, sizeof (*SignalEdgeData));


  //
  // Set to indicate early CA CLK training has executed
  //
  Host->var.mem.earlyCaClkExecuted = 1;

  // Execute JEDEC reset/init sequence
  FifoTrainReset (Host, Socket);
  PrevDebugLevel = GetDebugLevel ();
  if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
    SetDebugLevel (SDBG_ERROR);
  }
  IO_Reset (Host, Socket);
  SetDebugLevel (PrevDebugLevel);

#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    DisplayCCCResults (Host, Socket);
  }
  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK

  Host->var.mem.checkMappedOutRanks = 0;
  RcFreePool (SignalEdgeData);
  return SUCCESS;//DDR5_TODO.
} //EarlyCaClkTimingMargin

/**

  Finds CA CLK backside margins for the given Signal.

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
GetCaClkMarginsSweepTimingBackside (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Dimm,
  IN  UINT8     Rank,
  IN  GSM_CSN   SignalSubCh[MAX_CH][SUB_CH],
  OUT INT16     CaLeft[MAX_CH][SUB_CH][COPY_NUM],
  OUT INT16     CaRight[MAX_CH][SUB_CH][COPY_NUM],
  IN  struct    ClkTrainingResults (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8                        Ch;
  UINT16                       PiIndex;
  UINT8                        Offset;
  UINT8                        OrgDelay[MAX_CH][SUB_CH];
  UINT32                       RankMask[MAX_CH];
  UINT32                       ChBitmask = 0;
  UINT32                       BwSerr[MAX_CH][3];
  UINT8                        SubChannel = 0;
  UINT8                        Strobe;
  UINT8                        DWord;
  UINT8                        SubChMSVx8;
  UINT32                       ValidMask[MAX_STROBE_DDR5];
  INT16                        EyeWidthSC[MAX_CH][SUB_CH];
  INT16                        RisingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16                        CenterPoint[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16                        FallingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16                        PulseWidth[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16                        LeftEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16                        RightEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT32                       DqSampleResultStrobeSubCh[SUB_CH][MAX_STROBE_DDR5 / 2];
  struct dimmNvram             (*DimmNvList) [MAX_DIMM];
  UINT8                        Step;
  UINT32                       DqSampleResultStrobe[MAX_STROBE_DDR5 / 2];
  CHIP_GET_CMD_MGN_SWP_STRUCT  ChipGetCmdMgnSwpStruct;
  MRC_MST                      MemSsType;
  UINT8                        SubChannelMask;
  MRC_RT                       MrcRankList;
  UINT32                       OneChBitmask = 0;
  UINT8                        BacksideCopy = 0;
  UINT8                        MaxStrobe = 0;
  UINT8                        SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8                        MaxChDdr;
  UINT16                       PiIndexTemp;
  GSM_CSN                      SignalToShift = 0;
  INT16                        QCsDelay;
  UINT8                        RawCardStrobe[MAX_A_B_STROBE];
  DDR5_RCD_QACA_OUTPUT_DELAY_STRUCT QxcaOutputDelay;  // "QACA" structure works for both QACA and QBCA

  MaxChDdr = GetMaxChDdr ();
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  SubChMSVx8 = GetSubChMaxStrobeValid (Host) / 2;
  ZeroMem ((UINT8 *) RawCardStrobe, sizeof (RawCardStrobe));
  ZeroMem ((UINT32 *) DqSampleResultStrobe, sizeof (DqSampleResultStrobe));

  //
  // Init CaLeft,  CaRight, EyeWidthSC, LeftEdge and RightEdge
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      for (BacksideCopy = 0; BacksideCopy < COPY_NUM; BacksideCopy++) {
        CaLeft[Ch][SubChannel][BacksideCopy] = -UNMARGINED_CMD_EDGE;
        CaRight[Ch][SubChannel][BacksideCopy] = UNMARGINED_CMD_EDGE;
        EyeWidthSC[Ch][SubChannel] = 0;
      }
      for (Strobe = 0; Strobe < MAX_STROBE_DDR5 / 2; Strobe++) {
        LeftEdge[Ch][SubChannel][Strobe] = -UNMARGINED_CMD_EDGE;
        RightEdge[Ch][SubChannel][Strobe] = UNMARGINED_CMD_EDGE;
      } // Strobe
    }
  }
  for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
    for (Strobe = 0; Strobe < MAX_STROBE_DDR5 / 2; Strobe++) {
      DqSampleResultStrobeSubCh[SubChannel][Strobe] = 0;
    }
  }
  GetCmdMarginsSweepInit (&ChipGetCmdMgnSwpStruct);

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      RankMask[Ch] = 0;
      OrgDelay[Ch][SubChannel] = 0;
    }

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    if (Rank >= (*DimmNvList) [Dimm].numRanks) {
      continue;
    }

    // Skip CIDS if not NVMDIMM
    /*for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
    if (isCmdCtlTrainedDdr5 (Host, Socket, Ch, Dimm, SignalSubCh[Ch][SubChannel]) != (UINT32) CHECKPOINT_MINOR_EARLY_CA_CLK) {
    continue;
    }
    }*/
    //DDR5_TODO

    // Initialize Ch Rank mask
    RankMask[Ch] = 1;

    //
    // Get the original delay. The QACA and QBCA default value should be 0? DDR5_TODO.
    //
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      if (SignalSubCh[Ch][SubChannel] <= Q0ACA13) {
        ReadRcdCwCache (Socket, Ch, SubChannel, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_QACA_OUTPUT_DELAY_REG, &QxcaOutputDelay.Data);
      } else if (SignalSubCh[Ch][SubChannel] >= Q0BCA0) {
        ReadRcdCwCache (Socket, Ch, SubChannel, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_QBCA_OUTPUT_DELAY_REG, &QxcaOutputDelay.Data);
      } //DDR5_TODO. Merge it into GetSetSignalDdr5.
      OrgDelay[Ch][SubChannel] = QxcaOutputDelay.Bits.output_delay_control_for_qaca_output_signals;
      if (QxcaOutputDelay.Bits.full_cycle_delay == 1) {
        OrgDelay[Ch][SubChannel] += MAX_OUTPUT_DELAY_CONTROL_FOR_QXCA;
      }
      //GetSetSignalDdr5 (Host, Socket, Ch, DdrLevel, SignalSubCh[Ch][SubChannel], GSM_READ_ONLY, &OrgDelay[Ch][SubChannel]);
    }


    //
    // Setup CADB
    //
    OneChBitmask  = 0;
    OneChBitmask |= 1 << Ch;

    // Indicate Rank present and enabled on this Ch
    ChBitmask |= 1 << Ch;
    MrcRankList.Dimm = Dimm;
    MrcRankList.Rank = Rank;
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      if (Host->DdrioUltSupport) {
        CteSetupPattern ("earlyca_loopback_backside", (SignalSubCh[Ch][SubChannel] >= Q0BCA0) ? SignalSubCh[Ch][SubChannel] - Q0BCA0: SignalSubCh[Ch][SubChannel] - Q1ACA0, ChBitmask, (SignalSubCh[Ch][SubChannel] >= Q1BCA0) ? 0x2 : 0x1, GetPhyRank (Dimm, Rank));
      } else {
        SetupTest (Host, Socket, MemSsType, OneChBitmask, 1, &MrcRankList, DdrLevel, EarlyCaBacksideLoopbackTest, SignalSubCh[Ch][SubChannel], 0, FIRST_ITERATION_LOOP);
      }
    }
    ChipGetCmdMgnSwpStruct.eyeSize[Ch] = 0;
    ChipGetCmdMgnSwpStruct.chEyemask |= 1 << Ch;
    ChipGetCmdMgnSwpStruct.chDonemask |= 1 << Ch;

  } // Ch loop

  if (FeaturePcdGet (PcdCteBuild) == FALSE) {
    //
    // Real silicon does not need to re-start test when CA PI delay changes. So start/stop test outside the PI delay loop.
    //
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      TestControl (Host, Socket, MemSsType, ChBitmask, SubChannel, EarlyCaBacksideLoopbackTest, 1, START_TEST, NOT_STOP_TEST);
    }
  }
  //
  // Program counting window settings //DDR5_TODO
  //
  SetDataTrainCountWindow (Host, Socket, Dimm, Rank, DQ_COUNTING_WINDOW_VALUE);
  SetDqCountingWindow (Host, Socket, Dimm, Rank);
  //
  // Disable CS Trigger DDR5_TODO.
  //

  Offset = CA_CLK_STEP_SIZE_BACKSIDE;

  //
  // Sweep Pi delays
  //
  for (PiIndex = CA_CLK_TEST_START_BACKSIDE; PiIndex < ((CA_CLK_TEST_RANGE_BACKSIDE - QCA_EXTRA_SWEEP_RANGE) + CA_CLK_TEST_START_BACKSIDE); PiIndex += Offset) {

    if (ChBitmask == 0) {
      continue;
    }
    if (PiIndex == CA_CLK_TEST_START_BACKSIDE + 128) {
      //
      // Add the CS by 1tck.
      //
      for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }
        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          //
          // shift the CS by 1TCK delay.
          //
          //for (CopySide = 0; CopySide < COPY_NUM; CopySide++) {

          SignalToShift = CsToSweepBackSide [ GetBacksideCsSignalIndex (Ch, SubChannel, Dimm, Rank) ];
          QCsDelay = - BACKSIDE_1TCK;
          for (BacksideCopy = 0; BacksideCopy < GetMaxBacksideCopyNum (Socket, Ch, Dimm); BacksideCopy++) {
            GetSetQcsDelay (Host, Socket, Ch, Dimm, Rank, SignalToShift, BacksideCopy, GSM_WRITE_OFFSET | GSM_FORCE_WRITE, &QCsDelay, NULL, NULL);
          }
        }
      }
    }
    if (PiIndex >= 128) {
      PiIndexTemp = PiIndex - 128;
    } else {
      PiIndexTemp = PiIndex;
    }
    //
    // Shift timings on each channel
    //
    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if ((RankMask[Ch] == 0)) {
        continue;
      }
      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        ProgramQcaDelay (Host, Socket, Ch, Dimm, SubChannel, Rank, SignalSubCh[Ch][SubChannel], GSM_FORCE_WRITE , PiIndexTemp&0xFF, TRUE);
        //GetSetSignalDdr5 (Host, Socket, Ch, DdrLevel, SignalSubCh[Ch][SubChannel], GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &PiIndex);
      }//Subchannel loop
    } // Ch loop
    //
    // Run concurrent tests
    //
    if (FeaturePcdGet (PcdCteBuild) == TRUE) {
      if (Host->DdrioUltSupport) {
        CteStartPattern (ChBitmask, TWO_SUBCH_MASK, GetPhyRank (Dimm, Rank)); //Ch, sc, Rank
      } else {
        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          TestControl (Host, Socket, MemSsType, ChBitmask, SubChannel, EarlyCaBacksideLoopbackTest, 1, START_TEST, NOT_STOP_TEST);
        }
      }
    }
    //
    // Generate an IO Reset. This is required because the pi delays changed.
    //
    IO_Reset (Host, Socket); //should not need.
    //
    // Enabling the counter Mode. DDR5_TODO.
    //
    EnableDdrioTrainingCounter (Host, Socket, Dimm, Rank);

    ClearBwSerr (BwSerr);

    //
    // Get Test Result
    //
    DWord = (UINT8) ((PiIndex - CS_CLK_PI_START) / 32);
    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if ((RankMask[Ch] == 0)) {
        continue;
      }
      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      //
      // Continue to the next Rank if this one is disabled
      //
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
        continue;
      }
      //
      // Get Training Results.
      //
      GetTrainResultStatus (Host, Socket, 0, ChBitmask, NO_SUBCH, sizeof (DqSampleResultStrobeSubCh), (UINT32*) &DqSampleResultStrobeSubCh, ValidMask, 0, Dimm);
      //
      // Stop the pattern.
      //
      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        for (Strobe = 0; Strobe < MAX_STROBE_DDR5 / 2; Strobe++) {
          if (IsDimmStrobeNotValidDdr5 (&(*DimmNvList) [Dimm], Strobe)) {
            continue;
          }
          if ((DqSampleResultStrobe[Strobe] & 0x1FF) >= DQ_COUNTING_WINDOW_VALUE / 2) {
            for (Step = 0; Step < CA_CLK_STEP_SIZE_BACKSIDE; Step++) {
              (*TrainRes) [Ch][SubChannel][Strobe].results[DWord] = (*TrainRes) [Ch][SubChannel][Strobe].results[DWord] | (1 << ((PiIndex + Step) % 32));
            } // Step loop
          } //if
        } //Strobe loop
      }//Subchannel loop

      // Fill unused feedback entries //DDR5_TODO
       /*for (i = 1; i < CS_CLK_STEP_SIZE; i++) {
       DqFeedback[Ch][PiIndex + i] = DqFeedback[Ch][PiIndex];
       }*/
    } //Ch loop
  } // PiIndex loop

  //
  // Stop Test
  //
  if (Host->DdrioUltSupport) {
    CteStopPattern ();
  } else {
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      TestControl (Host, Socket, MemSsType, ChBitmask, SubChannel, EarlyCaBacksideLoopbackTest, 1, NOT_START_TEST, STOP_TEST);
    }
  }

  //
  // Exit CA Training Mode.
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    SubChannelMask = 0;
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      //
      // Check Subchannel is enabled or not. DDR5_TODO
      //
      SubChannelMask = SubChannelMask | (1 << SubChannel);
    } //sub channel loop
    //
    // Exit DRAM CA Training Mode.--DIMM sub channel A and B at the same time.
    //
    if (SignalSubCh[Ch][0] == Q0ACA13) {
      SendNop (Host, Socket, Ch, SubChannelMask, Dimm, Rank, 0, TRUE, TRUE);
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "Exit Backside DRAM CA Training Mode \n");
    }
  } //Ch loop.

  //
  // Hanlde the training result.
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    if (!(*DimmNvList) [Dimm].x4Present) {
      MaxStrobe = SubChMSVx8;
    } else {
      MaxStrobe = SubChMSVx4;
    }
    //
    // Continue to the next Rank if this one is disabled
    //
    if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) continue;
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      if (GetDebugLevel () & SDBG_MAX) {
        AcquirePrintControl ();
        RcDebugPrint (SDBG_DEFAULT, "\nSummary: Early Ca Clk Backside Training Pi \nS%d, Ch%d, DIMM%d, SC%d, Rank%d, %a\n", Socket, Ch, Dimm, SubChannel, Rank, GetSignalStrDdr5 (SignalSubCh[Ch][SubChannel]));
        PrintSampleArrayCaClk (Host, Socket, Ch, Dimm, 0, SubChannel, CA_CLK_TEST_RANGE_BACKSIDE, TrainRes);
        ReleasePrintControl ();
      }

      for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {//DDR5_TODO. For SPR, this function needs to be updated.
          continue;
        }
        EvaluatePiSettingResultsCaClk (Host, Socket, Ch, Dimm, Rank, Strobe, RisingEdge, CenterPoint, FallingEdge, PulseWidth,
          PI_CS_CLK_TYPE, SubChannel, TrainRes);
        //
        // Calculate left edge and right edge
        //
        LeftEdge[Ch][SubChannel][Strobe] = MIN (FallingEdge[Ch][SubChannel][Strobe], RisingEdge[Ch][SubChannel][Strobe]);
        RightEdge[Ch][SubChannel][Strobe] = MAX (FallingEdge[Ch][SubChannel][Strobe], RisingEdge[Ch][SubChannel][Strobe]);
        if (LeftEdge[Ch][SubChannel][Strobe] == RightEdge[Ch][SubChannel][Strobe]) {
          LeftEdge[Ch][SubChannel][Strobe] = 0;
          RightEdge[Ch][SubChannel][Strobe] = 0;
        }
      } // Strobe loop

      //
      // Display the left edge, right edge and eye width.
      //
      DisplayEdgesDDR5 (Host, Socket, Ch, PulseWidth[Ch], LeftEdge[Ch], RightEdge[Ch], SubChannel, MaxStrobe);
      //DisplayCompositeCaEyeDDR5(Host, Socket, Ch, CaLeft[Ch], CaRight[Ch], EyeWidthSC[Ch], SubChannel, MAX_STROBE_DDR5 / 2);
      //
      // Get the Strobe mapping. And pick up the A, B copy related Strobe results into A, B result array.//DDR5_TODO.
      //
      for (BacksideCopy = 0; BacksideCopy < COPY_NUM; BacksideCopy++) {
        //
        // Get the raw card mapping.
        //
        if ((!((*DimmNvList) [Dimm].x4Present)) && (BacksideCopy == COPY_B)) {
          continue;
        }
        GetRawCardMapping (Host, Socket, Ch, SubChannel, Dimm, BacksideCopy, RawCardStrobe);

        //
        // Display the left edge, right edge and eye width.
        //
        if (BacksideCopy == 0) {
          RcDebugPrint (SDBG_MAX, "A Copy Signal--%a\n", GetSignalStrDdr5 (SignalSubCh[Ch][SubChannel]));
        } else {
          RcDebugPrint (SDBG_MAX, "B Copy Signal--%a\n", GetSignalStrDdr5 (SignalSubCh[Ch][SubChannel] + QCA_DISTANCE_A_B_COPY));
        }
        DisplayBacksideEdgesDDR5 (Host, Socket, Ch, PulseWidth[Ch], LeftEdge[Ch], RightEdge[Ch], SubChannel, RawCardStrobe);
        //DisplayCompositeCaEyeDDR5(Host, Socket, Ch, CaLeft[Ch], CaRight[Ch], EyeWidthSC[Ch], SubChannel, MAX_STROBE_DDR5 / 2);
        //
        // Calculate the composite eye for the A, B copy signals.
        //
        GetCaClkCompositeEyeBackside (LeftEdge[Ch][SubChannel], RightEdge[Ch][SubChannel], &CaLeft[Ch][SubChannel][BacksideCopy], &CaRight[Ch][SubChannel][BacksideCopy], RawCardStrobe);
        EyeWidthSC[Ch][SubChannel] = ABS (CaRight[Ch][SubChannel][BacksideCopy] - CaLeft[Ch][SubChannel][BacksideCopy]);
        //
        // Display the left edge, right edge and eye width.
        //
        DisplayCompositeCaEyeDDR5 (Host, Socket, Ch, CaLeft[Ch][SubChannel][BacksideCopy], CaRight[Ch][SubChannel][BacksideCopy], EyeWidthSC[Ch], SubChannel, MaxStrobe);
        //
        // Offset results relative to the starting point of the sweep
        //
        CaLeft[Ch][SubChannel][BacksideCopy] = CaLeft[Ch][SubChannel][BacksideCopy] - OrgDelay[Ch][SubChannel];
        CaRight[Ch][SubChannel][BacksideCopy] = CaRight[Ch][SubChannel][BacksideCopy] - OrgDelay[Ch][SubChannel];

        //
        // Display the left edge Offset, right edge Offset and eye width.
        //
        RcDebugPrint (SDBG_MAX, "After Offset:\n");
        DisplayCompositeCaEyeDDR5 (Host, Socket, Ch, CaLeft[Ch][SubChannel][BacksideCopy], CaRight[Ch][SubChannel][BacksideCopy], EyeWidthSC[Ch], SubChannel, MaxStrobe);
      }//BacksideCopy
    }//Subchannel loop
  } // Ch loop

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if ((RankMask[Ch] == 0)) {
      continue;
    }
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }
        //
        // Check eye width.
        //
        if ((RightEdge[Ch][SubChannel][Strobe] - LeftEdge[Ch][SubChannel][Strobe]) <= 0) {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
            "ERROR: Signal %a, No eye found.\n", GetSignalStrDdr5 (SignalSubCh[Ch][SubChannel]));
          LeftEdge[Ch][SubChannel][Strobe] = 0;
          RightEdge[Ch][SubChannel][Strobe] = 0;
        }///if
      } //Strobe loop
    } //Subchannel loop
    //
    // Restore Signal under test to its initial setting
    //
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    if (Rank >= (*DimmNvList) [Dimm].numRanks) {
      continue;
    }
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      ProgramQcaDelay (Host, Socket, Ch, Dimm, SubChannel, Rank, SignalSubCh[Ch][SubChannel], GSM_FORCE_WRITE , OrgDelay[Ch][SubChannel], TRUE);
      //GetSetSignalDdr5 (Host, Socket, Ch, DdrLevel, SignalSubCh[Ch][SubChannel], GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &OrgDelay[Ch][SubChannel]);
    }
  } // Ch loop
} // GetCaClkMarginsSweepTimingBackside


/*

  This function margins the backside signals in the RCD for DDR5

  Examples to call this function:

  RESULTS_PER_BACKSIDESIGNAL_CH_SUBCH_DIMM_RANK_AB ResultsA;
  RESULTS_PER_CH_SUBCH_DIMM_RANK_AB ResultsB;
  RESULTS_PER_CH_SUBCH_DIMM_RANK    ResultsC;

  GetQxCaCsMargins (Socket, QnxCAm, sizeof (ResultsA), (VOID *) &ResultsA); //EFI_SUCCESS
  GetQxCaCsMargins (Socket, Q0ACA0, sizeof (ResultsA), (VOID *) &ResultsA); //EFI_SUCCESS
  GetQxCaCsMargins (Socket, Q0ACA0, sizeof (ResultsB), (VOID *) &ResultsB); //EFI_SUCCESS
  GetQxCaCsMargins (Socket, QnxCAm, 'DC', (VOID *) NULL);                   //EFI_SUCCESS

  GetQxCaCsMargins (Socket, Q[0:1]BCA[0:13], 'DC', (VOID *) 'DC');          //RETURN_UNSUPPORTED
  GetQxCaCsMargins (Socket, QnxCAm, sizeof (ResultsB), (VOID *) &ResultsB); //RETURN_BUFFER_TOO_SMALL

  GetQxCaCsMargins (Socket, QnCSm, sizeof (ResultsC), (VOID *) &ResultsC);   //EFI_SUCCESS
  GetQxCaCsMargins (Socket, Q0CS0_N, sizeof (ResultsC), (VOID *) &ResultsC); //RETURN_UNSUPPORTED


  'DC' - Dont Care

  @param[in]  Socket    Current socket under test (0-based)
  @param[in]  Signal    Current signal under test, allowed signals are:

                        QnxCAm - All backside CA signals (margins sweeps are still gotten individually)
                        Q[0:1]ACA[0:13] - individual signals for QnACA
                        Note: Q[0:1]BCA[0:13] are not allowed as these are already provided as a backside copy of A

                        QnCSm - All (based on enabled Dimm/Rank) backside CS signals (margins sweeps are still gotten individually)

  @param[in]  ArraySize Array size reserved for *Results
  @param[out] Results   Pointer to a preallocated memory to provide the results related with the backside RCD signals
                        Preallocated memory MUST be at least:
                        * RESULTS_PER_CH_SUBCH_DIMM_RANK_AB size for any allowed individual signal for QnACA.
                        * RESULTS_PER_BACKSIDESIGNAL_CH_SUBCH_DIMM_RANK_AB size for QnxCAm signal.
                        * RESULTS_PER_CH_SUBCH_DIMM_RANK size for QnCSm signal.
                        * NULL if there is no need to retrieve results

  @retval   EFI_SUCCESS               When training is able to executed correctly based on input parameters
  @retval   RETURN_UNSUPPORTED        Signal is not supported
  @retval   RETURN_BUFFER_TOO_SMALL   Buffer size to provide the results is not big enough

 */
EFI_STATUS
EFIAPI
GetQxCaCsMargins (
  IN  UINT8   Socket,
  IN  GSM_CSN Signal,
  IN  UINT16  ArraySize,
  OUT VOID    *Results    OPTIONAL
  )
{
  PSYSHOST  Host;
  UINT8     Channel;
  UINT8     SubChannel;
  UINT8     Dimm;
  UINT8     Rank;
  UINT8     SignalIndex;
  UINT8     SignalIndexBegin;
  UINT8     SignalIndexEnd;
  UINT8     BacksideCopy;
  INT16     Left[MAX_CH][SUB_CH][COPY_NUM];
  INT16     Right[MAX_CH][SUB_CH][COPY_NUM];
  INT16     CsLeft[MAX_CH][SUB_CH][COPY_NUM];
  INT16     CsRight[MAX_CH][SUB_CH][COPY_NUM];
  INT16     EyeWidth[MAX_CH][SUB_CH][COPY_NUM];
  INT16     CsValue[MAX_CH][SUB_CH][COPY_NUM];
  UINT32    PreviousDebugLevel;
  GSM_CSN   SignalSubCh[MAX_CH][SUB_CH];
  BOOLEAN   AnyRankEnabled;
  BOOLEAN   QnCaSignal; ///< TRUE: QnCA signal, FALSE: otherwise
  BOOLEAN   QnCsSignal; ///< TRUE: QnCS signal, FALSE: otherwise
  UINT8     MaxChDdr;
  struct dimmNvram                (*DimmNvList) [MAX_DIMM];
  struct ClkTrainingResults       TrainRes[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];


  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  QnCaSignal = FALSE;
  QnCsSignal = FALSE;

  if ((Signal == QnxCAm) ||
      (Signal >= Q0ACA0 && Signal <= Q0ACA13) ||
      (Signal >= Q1ACA0 && Signal <= Q1ACA13)) {

    QnCaSignal = TRUE;

  } else if (Signal == QnCSm) {

    QnCsSignal = TRUE;

  } else {
    //
    // Illegal Signal input parameter
    //
    return RETURN_UNSUPPORTED;
  }

  PreviousDebugLevel = GetDebugLevel ();
  SetDebugLevel (SDBG_NONE);

  if (Results != NULL) {
    if (Signal == QnxCAm) {

      if (sizeof (RESULTS_PER_BACKSIDESIGNAL_CH_SUBCH_DIMM_RANK_AB) > ArraySize) {
        return RETURN_BUFFER_TOO_SMALL;
      }
      ZeroMem ((UINT8 *) Results, sizeof (RESULTS_PER_BACKSIDESIGNAL_CH_SUBCH_DIMM_RANK_AB));

    } else if (QnCaSignal) {

      if (sizeof (RESULTS_PER_CH_SUBCH_DIMM_RANK_AB) > ArraySize) {
        return RETURN_BUFFER_TOO_SMALL;
      }
      ZeroMem ((UINT8 *) Results, sizeof (RESULTS_PER_CH_SUBCH_DIMM_RANK_AB));

    } else if (QnCsSignal) {

      if (sizeof (RESULTS_PER_CH_SUBCH_DIMM_RANK) > ArraySize) {
        return RETURN_BUFFER_TOO_SMALL;
      }
      ZeroMem ((UINT8 *) Results, sizeof (RESULTS_PER_CH_SUBCH_DIMM_RANK));

    }
  }

  if (QnCaSignal) {
    SetCurrentTestType (Socket, EarlyCaBacksideLoopbackTest);
  } else if (QnCsSignal) {
    SetCurrentTestType (Socket, EarlyCsBacksideLoopbackTest);
  }

  //
  // Enter sense amp training Mode to all the channels here to latch results.
  //
  SetTrainingModeLatchResult (Host, Socket, CHECKPOINT_MINOR_SENSE_AMP);

  IO_Reset (Host, Socket);

  if (Signal == QnxCAm) {
    SignalIndexBegin = 0;
    SignalIndexEnd = BACKSIDE_CA_SIGNALS_TO_SWEEP;
  } else {
    SignalIndexBegin = 0;
    SignalIndexEnd = 1;
  }

  for (SignalIndex = SignalIndexBegin; SignalIndex < SignalIndexEnd; SignalIndex++) {
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {

        AnyRankEnabled = FALSE;

        for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {

          for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
            SignalSubCh[Channel][SubChannel] = NO_SIGNAL;
          } // SubChannel loop...

          if (IsChannelEnabled (Socket, Channel) == FALSE) {
            continue;
          }

          if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          AnyRankEnabled = TRUE;

          DimmNvList = GetDimmNvList (Host, Socket, Channel);
          for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
            if (Signal == QnxCAm) {
              SignalSubCh[Channel][SubChannel] = SignalsToSweepBacksideDDR5[ GetBacksideCaSignalIndex (Channel, SubChannel, Dimm, Rank, SignalIndex) ];
            } else if (QnCaSignal) {
              SignalSubCh[Channel][SubChannel] = Signal;
            } else if (QnCsSignal) {
              SignalSubCh[Channel][SubChannel] = CsToSweepBackSide[ GetBacksideCsSignalIndex (Channel, SubChannel, Dimm, Rank)  ];
            }
          } // SubChannel loop...

          if (QnCsSignal) {
            EnterRcdCaPassThroughMode (Host, Socket, Channel, Dimm, Rank);
            //
            // Set DRAM CS Training Mode
            //
            WriteMPC (Host, Socket, Channel, Dimm, SUB_CH_A_MASK, Rank, Enter_CS_Training_Mode, TIMING_4N, TRUE);
            WriteMPC (Host, Socket, Channel, Dimm, SUB_CH_B_MASK, Rank, Enter_CS_Training_Mode, TIMING_4N, TRUE);
          }

        } // Channel loop ...

        if (!AnyRankEnabled) {
          continue;
        }

        ZeroMem ((UINT8 *) Left, sizeof (Left));
        ZeroMem ((UINT8 *) Right, sizeof (Right));
        ZeroMem ((UINT8 *) CsLeft, sizeof (CsLeft));
        ZeroMem ((UINT8 *) CsRight, sizeof (CsRight));
        ZeroMem ((UINT8 *) TrainRes, sizeof (TrainRes));

        if (QnCaSignal) {
          GetCaClkMarginsSweepTimingBackside (Host, Socket, Dimm, Rank, SignalSubCh, Left, Right, &TrainRes);
        } else if (QnCsSignal) {
          GetCsBacksideMarginsSweep (Host, Socket, Dimm, Rank, SignalSubCh, CsLeft, CsRight, EyeWidth, CsValue, 0, &TrainRes);
        }

        if (QnCsSignal) {
          for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {

            if (IsChannelEnabled (Socket, Channel) == FALSE) {
              continue;
            }

            if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
              continue;
            }
            //
            // Exit DRAM CS Training mode.
            //
            WriteMPC (Host, Socket, Channel, Dimm, SUB_CH_A_MASK, Rank, Exit_CS_Training_Mode, TIMING_4N, TRUE);
            WriteMPC (Host, Socket, Channel, Dimm, SUB_CH_B_MASK, Rank, Exit_CS_Training_Mode, TIMING_4N, TRUE);

            ExitRcdCaPassThroughMode (Host, Socket, Channel, Dimm, Rank);
          } // Channel loop ...
        } // QnCsSignal
        if (Results != NULL) {
          for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
            if (IsChannelEnabled (Socket, Channel) == FALSE) {
              continue;
            }
            if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
              continue;
            }
            for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

              //
              // Only loop thru the backside copy when doing the sweep of any QnACAm signal
              //
              if (QnCaSignal) {
                for (BacksideCopy = 0; BacksideCopy < COPY_NUM; BacksideCopy++) {
                  if (Signal == QnxCAm) {
                    ((RESULTS_PER_BACKSIDESIGNAL_CH_SUBCH_DIMM_RANK_AB *) Results)->Backside[SignalIndex][Channel][SubChannel][Dimm][Rank][BacksideCopy].n = Left[Channel][SubChannel][BacksideCopy];
                    ((RESULTS_PER_BACKSIDESIGNAL_CH_SUBCH_DIMM_RANK_AB *) Results)->Backside[SignalIndex][Channel][SubChannel][Dimm][Rank][BacksideCopy].p = Right[Channel][SubChannel][BacksideCopy];
                  } else {
                    ((RESULTS_PER_CH_SUBCH_DIMM_RANK_AB *) Results)->Rank[Channel][SubChannel][Dimm][Rank][BacksideCopy].n = Left[Channel][SubChannel][BacksideCopy];
                    ((RESULTS_PER_CH_SUBCH_DIMM_RANK_AB *) Results)->Rank[Channel][SubChannel][Dimm][Rank][BacksideCopy].p = Right[Channel][SubChannel][BacksideCopy];
                  }
                } // BacksideCopy loop ...
              } else if (QnCsSignal) {
                for (BacksideCopy = 0; BacksideCopy < COPY_NUM; BacksideCopy++) {
                  ((RESULTS_PER_CH_SUBCH_DIMM_RANK_AB *) Results)->Rank[Channel][SubChannel][Dimm][Rank][BacksideCopy].n = CsLeft[Channel][SubChannel][BacksideCopy];
                  ((RESULTS_PER_CH_SUBCH_DIMM_RANK_AB *) Results)->Rank[Channel][SubChannel][Dimm][Rank][BacksideCopy].p = CsRight[Channel][SubChannel][BacksideCopy];
                }
              }
            } // SubChannel loop ...
          } // Channel loop ...
        } // Results != NULL statement
      } // Rank loop ...
    } // Dimm loop ...
  } // SignalIndex loop...

  //
  // Disable CADB always on Mode
  //
  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    DisableCADBAlwaysOnMode (Socket, Channel);
  }
  //
  // Exit sense amp training Mode
  //
  SetTrainingModeLatchResult (Host, Socket, CHECKPOINT_MINOR_DISABLE_SENSE_AMP);

  SetCurrentTestType (Socket, 0);

  FifoTrainReset (Host, Socket);
  IO_Reset (Host, Socket);
  JedecInitSequence (Host, Socket, CH_BITMASK);

  SetDebugLevel (PreviousDebugLevel);

  return EFI_SUCCESS;
}

/**
  Display Backside Edges for DDR5

  @param[in]  LeftEdge          - data of the left edge
  @param[in]  RightEdge         - data of the right edge
  @param[out] CaLeft            - Ca Left
  @param[out] CaRight           - Ca Right
  @param[in]  RawCardStrobe     - Raw Card Strobe

  @retval N/A
**/
VOID
GetCaClkCompositeEyeBackside (
  IN  INT16     LeftEdge[MAX_STROBE_DDR5 / 2],
  IN  INT16     RightEdge[MAX_STROBE_DDR5 / 2],
  OUT INT16*    CaLeft,
  OUT INT16*    CaRight,
  IN  UINT8     RawCardStrobe[MAX_A_B_STROBE]
  )
{
  PSYSHOST  Host;
  UINT8 Strobe = 0;
  INT16 LeftEdgeTemp = -255;
  INT16 RightEdgeTemp = 255;
  Host = GetSysHostPointer ();

  for (Strobe = 0; Strobe < MAX_A_B_STROBE; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, RawCardStrobe[Strobe])) {
      continue;
    }
    if (LeftEdgeTemp < LeftEdge[RawCardStrobe[Strobe]]) {
      LeftEdgeTemp = LeftEdge[RawCardStrobe[Strobe]];
    }
    if (RightEdgeTemp > RightEdge[RawCardStrobe[Strobe]]) {
      RightEdgeTemp = RightEdge[RawCardStrobe[Strobe]];
    }
  }
  *CaLeft = LeftEdgeTemp;
  *CaRight = RightEdgeTemp;
}

/**
  Display Backside Edges for DDR5

  @param[in] Host              - Pointer to sysHost
  @param[in] Socket            - Current Socket
  @param[in] Ch                - Channel number
  @param[in] PulseWidth        - data of the pulse width
  @param[in] LeftEdge          - data of the left edge
  @param[in] RightEdge         - data of the right edge
  @param[in] SubChannel        - sub channel number
  @param[in] RawCardStrobe     - Raw Card Strobe

  @retval N/A
**/
VOID
DisplayBacksideEdgesDDR5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT16    PulseWidth[SUB_CH][MAX_STROBE_DDR5 / 2],
  IN INT16     LeftEdge[SUB_CH][MAX_STROBE_DDR5 / 2],
  IN INT16     RightEdge[SUB_CH][MAX_STROBE_DDR5 / 2],
  IN UINT8     SubChannel,
  IN UINT8     RawCardStrobe[MAX_A_B_STROBE]
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8 Strobe;

  if (!(GetDebugLevel () & SDBG_MAX)) {
    return;
  }

  AcquirePrintControl ();

  RcDebugPrintLine (SDBG_MAX, ((MAX_A_B_STROBE) * 4) + 4, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  // Print left edges
  RcDebugPrint (SDBG_DEFAULT, "LE: ");
  for (Strobe = 0; Strobe < MAX_A_B_STROBE; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, RawCardStrobe[Strobe])) {
      continue;
    }
    if (RawCardStrobe[Strobe] > (MAX_STROBE_DDR5 / 2)) {
      RC_FATAL_ERROR (FALSE, ERR_MRC_STRUCT, ERR_ARRAY_OUT_OF_BOUNDS);
      return;
    }
    RcDebugPrint (SDBG_DEFAULT, " %3d", LeftEdge[SubChannel][RawCardStrobe[Strobe]]);
  } // Strobe loop
  // Print right edges
  RcDebugPrint (SDBG_DEFAULT, "\nRE: ");
  for (Strobe = 0; Strobe < MAX_A_B_STROBE; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, RawCardStrobe[Strobe])) {
      continue;
    }
    if (RawCardStrobe[Strobe] > (MAX_STROBE_DDR5 / 2)) {
      RC_FATAL_ERROR (FALSE, ERR_MRC_STRUCT, ERR_ARRAY_OUT_OF_BOUNDS);
      return;
    }
    RcDebugPrint (SDBG_DEFAULT, " %3d", RightEdge[SubChannel][RawCardStrobe[Strobe]]);
  } // Strobe loop

  // Print pulse width
  RcDebugPrint (SDBG_DEFAULT, "\nPW: ");
  for (Strobe = 0; Strobe < MAX_A_B_STROBE; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, RawCardStrobe[Strobe])) {
      continue;
    }
    if (RawCardStrobe[Strobe] > (MAX_STROBE_DDR5 / 2)) {
      RC_FATAL_ERROR (FALSE, ERR_MRC_STRUCT, ERR_ARRAY_OUT_OF_BOUNDS);
      return;
    }
    RcDebugPrint (SDBG_DEFAULT, " %3d", PulseWidth[SubChannel][RawCardStrobe[Strobe]]);
  } // Strobe loop

  RcDebugPrint (SDBG_DEFAULT, "\n\n");

  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK
} // DisplayBacksideEdgesDDR5


/**

Exectues early backside CA/clock training to get/set the timing margin.

@param Host - Pointer to sysHost

@retval N/A

**/
UINT32
EarlyCaClkTimingMarginBackside (
  PSYSHOST  Host
  )
{

  UINT8                               Ch = 0;
  UINT8                               RankEnabled;
  struct  ClkTrainingResults          TrainRes[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT8                               Socket;
  UINT8                               Dimm;
  UINT8                               Rank;
  UINT8                               Signal;
  UINT8                               Index = 0;
  UINT8                               SubChannel = 0;
  UINT8                               BacksideCopy = 0;
  UINT32                              PrevDebugLevel;
  struct dimmNvram (*DimmNvList) [MAX_DIMM];
  GSM_CSN                             SignalToSweepSubCh[MAX_CH][SUB_CH];
  GSM_CSEDGE_CA                       (*SignalEdgeData) [MAX_CH][SUB_CH][MAX_RANK_CH][COPY_NUM][BACKSIDE_CA_SIGNALS_TO_SWEEP];
  struct ddrRank (*RankList) [MAX_RANK_DIMM];
  INT16                               Le[MAX_CH][SUB_CH][COPY_NUM];
  INT16                               Re[MAX_CH][SUB_CH][COPY_NUM];
  UINT8                               MaxChDdr;
  INT8                                QcaVrefValue;

  UINT32  SmbData = 0;
  UINT8   SubChannelMask;
  MaxChDdr = GetMaxChDdr ();
  Host->var.mem.checkMappedOutRanks = 1;
  Socket = Host->var.mem.currentSocket;

  //
  // Return if this Socket is disabled
  //

  if (IsSocketDimmPopulated (Host, Socket) != TRUE) {
    return SUCCESS;
  }

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_EARLY_CA_CLK, Socket));

  SignalEdgeData = RcAllocatePool (sizeof (*SignalEdgeData));
  if (SignalEdgeData == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return FAILURE;
  }
  ZeroMem ((UINT8 *) SignalEdgeData, sizeof (*SignalEdgeData));

#ifdef DEBUG_CODE_BLOCK
  AcquirePrintControl ();
#endif // DEBUG_CODE_BLOCK
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Early Ca Clk Backside Timing Margin Starts\n");

  Index = 0;

  //
  // Test each Signal on every Rank on every Dimm on each Vref.
  //
  for (Signal = 0; Signal < BACKSIDE_CA_SIGNALS_TO_SWEEP; Signal++) {
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        //
        // initialize to no Signal
        //
        SignalToSweepSubCh[Ch][0] = NO_SIGNAL;
        SignalToSweepSubCh[Ch][1] = NO_SIGNAL;
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }
        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          Index = GetBacksideCaSignalIndex (Ch, SubChannel, Dimm, Rank, Signal);
          SignalToSweepSubCh[Ch][SubChannel] = SignalsToSweepBacksideDDR5[Index];
        } //Subchannel loop
      }//Ch loop

      for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

        RankEnabled = 0;

        for (Ch = 0; Ch < MaxChDdr;Ch += GetChInc (Host)) {
          if (IsChannelEnabled (Socket, Ch) == FALSE) {
            continue;
          }
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          DimmNvList = GetDimmNvList (Host, Socket, Ch);
          if ((*DimmNvList) [Dimm].dimmPresent == 0) {
            continue;
          }
          if (((*DimmNvList) [Dimm].DcpmmPresent) && ((*DimmNvList) [Dimm].fmcType == FMC_CWV_TYPE)) {
            continue;
          }
          if (Rank >= (*DimmNvList) [Dimm].numRanks) {
            continue;
          }
          RankEnabled = 1;
          break;
        } // Ch loop

        //
        // Continue to the next Rank if this one is not populated
        //
        if (RankEnabled == 0) {
          continue;
        }
        for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
          if (IsChannelEnabled (Socket, Ch) == FALSE) {
            continue;
          }

          //
          // Enter RCD CA pass through Mode
          //
          ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_GLOBAL_FEATURES_REG, (UINT8 *)&SmbData);
          SmbData = (BIT2 | (Rank << 3));
          WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, SUB_CH_A_MASK, BYTE_WRITE, RDIMM_CW_PAGE0, DDR5_RCD_GLOBAL_FEATURES_REG, &SmbData);

          for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
            //
            // Check Subchannel is enabled or not. DDR5_todo
            //
            SubChannelMask = 0;
            SubChannelMask = SubChannelMask | (1 << SubChannel);

            //
            // Parity checking must be disabled prior to and during pass-through Mode. It's disabled by default. So, need to make sure it's disabled after jedec reset.
            // Probably, the below code can be commented out. DDR5_todo. to confirm?
            //
            // DDR5_todo. Change it to use MRW command. It can use MRW command.
            //
            //
            // As the parity is disabled by default. No need to disable it separately.
            //
            //
            // Firstly, enable pass through Mode and then set DRAM CA Vref Value.--DIMM sub channel A and B at the same time.
            //

            GetQcaVrefValueOdt (Socket, Ch, Dimm, &QcaVrefValue);
            WriteVrefCa (Host, Socket, Ch, Dimm, SubChannelMask, Rank, ALL_STROBES, QcaVrefValue, TIMING_MULTI_CS, TRUE); //program both Subchannel 0 and Subchannel 1
            WriteMPC (Host, Socket, Ch, Dimm, SubChannelMask, Rank, APPLY_VREFCA_RTT_CA_CS_CK, TIMING_4N, TRUE);
            RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
              "DRAM QcaVrefValue 0x%x \n", QcaVrefValue);
            //
            // Enter DRAM CA Training Mode.--DIMM sub channel A and B at the same time.
            //
            WriteMPC (Host, Socket, Ch, Dimm, SubChannelMask, Rank, Enter_CA_Training_Mode, TIMING_4N, TRUE);//DDR5_todo. program both Subchannel 0 and Subchannel 1. Support?
            RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
              "Enter DRAM CA Training Mode \n");
          } //sub channel loop
          //
          // Enter DRAM CA Training Mode.--DIMM sub channel A and B at the same time.
          //
          WriteMPC (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, Rank, Enter_CA_Training_Mode, TIMING_4N, TRUE);

        } //Ch loop.
        //
        // Get CMD margins for current Rank and Signal
        //
        //
        // Init TrainRes.Clear the train result or it will impact the next loop test. //can move the trainres to the GetCaClkMarginsSweepTiming internal function.
        //
        ZeroMem ((UINT8 *) TrainRes, sizeof (TrainRes));
        GetCaClkMarginsSweepTimingBackside (Host, Socket, Dimm, Rank, SignalToSweepSubCh, Le, Re, &TrainRes);

        for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
          if (IsChannelEnabled (Socket, Ch) == FALSE) {
            continue;
          }

          for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
            if (SignalToSweepSubCh[Ch][SubChannel] == NO_SIGNAL) {
              continue;
            }

            RankList = GetRankNvList (Host, Socket, Ch, Dimm);
            if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
              continue;
            }
            DimmNvList = GetDimmNvList (Host, Socket, Ch);
            if (((*DimmNvList) [Dimm].DcpmmPresent) && ((*DimmNvList) [Dimm].fmcType == FMC_CWV_TYPE)) {
              continue;
            }
            if (Rank >= (*DimmNvList) [Dimm].numRanks) {
              continue;
            }
            for (BacksideCopy = 0; BacksideCopy < COPY_NUM; BacksideCopy++) {
              //
              // save the results.
              //
              (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][BacksideCopy][Signal].signal = SignalToSweepSubCh[Ch][SubChannel] + (QCA_DISTANCE_A_B_COPY * BacksideCopy);
              (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][BacksideCopy][Signal].le = Le[Ch][SubChannel][BacksideCopy];
              (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][BacksideCopy][Signal].re = Re[Ch][SubChannel][BacksideCopy];
              (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][BacksideCopy][Signal].toSample = 1;
            }
          } //SubChannel Loop
        } // Ch loop
      } // Rank loop
    } // Dimm loop

    RcDebugPrint (SDBG_MAX, "\n");
  } // Signal loop

  //
  // clear to insure we take the right path for setting final PI values
  //
  Host->var.mem.earlyCaClkExecuted = 0;
  //
  // Set the timing margin data.
  //
  //DDR5_TODO.
  SetCombinedCaGroupDdr5 (Host, Socket, Ch, SubChannel, DdrLevel, GSM_COMBINE_EDGES, CMDCTL_SIGNAL_DELAY, BACKSIDE_CA_SIGNALS_TO_SWEEP,
    SignalEdgeData, sizeof (*SignalEdgeData));


  //
  // Set to indicate early CA CLK training has executed
  //
  Host->var.mem.earlyCaClkExecuted = 1;

  // Execute JEDEC reset/init sequence
  FifoTrainReset (Host, Socket);
  PrevDebugLevel = GetDebugLevel ();
  if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
    SetDebugLevel (SDBG_ERROR);
  }
  IO_Reset (Host, Socket);
  SetDebugLevel (PrevDebugLevel);

#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    DisplayCCCResults (Host, Socket);
  }
  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK

  Host->var.mem.checkMappedOutRanks = 0;
  RcFreePool (SignalEdgeData);
  return SUCCESS;//DDR5_TODO.
} //EarlyCaClkTimingMarginBackside

//------------------------------------------------------------------------------------------

/**

  Finds if a give CID or CS is trained

  @param Host        - Pointer to sysHost
  @param Socket      - Socket number
  @param Dimm        - DIMM number
  @param Signal      - Signal

  @retval N/A

**/
UINT32
isCmdCtlTrainedDdr5 (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  GSM_CSN   Signal
  )
{
  UINT8               Status;
  struct channelNvram (*ChannelNvList) [MAX_CH];
  struct dimmNvram    (*DimmNvList) [MAX_DIMM];

  ChannelNvList = GetChannelNvList (Host, Socket);
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  Status = CHECKPOINT_MINOR_EARLY_CMD_CLK;

  // CTL signals use earlyCTLCLK for RDIMM or NVMDIMM
  if ((Host->nvram.mem.dimmTypePresent == RDIMM) || (*DimmNvList) [Dimm].DcpmmPresent) {
    switch (Signal) {
      case CKE0:
      case CKE1:
      case CKE2:
      case CKE3:
      case ODT0:
      case ODT1:
      case ODT2:
      case ODT3:
      case CS0_N:
      case CS1_N:
      case CS4_N:
      case CS5_N:
        Status = CHECKPOINT_MINOR_EARLY_CTL_CLK;
        break;
      default:
        break;
    }
  }

  //NVMDIMM use earlyCmdClk
  if ((*DimmNvList) [Dimm].DcpmmPresent) {
    switch (Signal) {
      case CS2_N:
      case CS3_N:
      case CS6_N:
      case CS7_N:
      case C0:
      case C1:
      case C2:
        Status = CHECKPOINT_MINOR_EARLY_CMD_CLK;
        break;
      default:
        break;
    }
  }

  // Standard LRDIMM trains C0 with earlyCtlClk, NOT NVMDIMM or 3DS
  if (((IsLrdimmPresent (Socket, Ch, Dimm) || ((*DimmNvList) [Dimm].numRanks == 4)) && ((*DimmNvList) [Dimm].DcpmmPresent == 0) && (*ChannelNvList) [Ch].encodedCSMode != 2)) {
    switch (Signal) {
      case C0:
      case C1:
      case CS2_N:
      case CS3_N:
        if ((*DimmNvList) [Dimm].numRanks == 2) {
          Status = 0; // don't train DR LRDIMM
        } else {
          Status = CHECKPOINT_MINOR_EARLY_CTL_CLK;
        }
        break;
      case CS6_N:
      case CS7_N:
        Status = CHECKPOINT_MINOR_EARLY_CTL_CLK;
        break;
      case C2:
        Status = 0;
        break;
      default:
        break;
    }
  }

  // 3DS LRDIMM or RDIMM, carefull this a channel level so we could have 3DS with NVMDIMM
  if (((*ChannelNvList) [Ch].encodedCSMode == 2) && ((*DimmNvList) [Dimm].DcpmmPresent == 0)) {
    //always train these for 3DS

    if (((Signal == C1) || (Signal == CS3_N) || (Signal == CS7_N)) && (((*ChannelNvList) [Ch].cidBitMap & BIT1) == 0)) {
      Status = 0; // Do not train it
    } // else it will

    if ((Signal == C2) && (((*ChannelNvList) [Ch].cidBitMap & BIT2) == 0)) {
      Status = 0; // Do not train it
    }
  }

  // SKIP CID's or upper CS#  if RDIMM / UDIMM (NOT LRDIMM and NOT NVMDIMM and NOT 3DS)
  if ((IsLrdimmPresent (Socket, Ch, Dimm) == 0) && ((*DimmNvList) [Dimm].DcpmmPresent == 0) && ((*ChannelNvList) [Ch].encodedCSMode != 2) &&
      ((*DimmNvList) [Dimm].numRanks < 4)) {
    switch (Signal) {
      case C0:
      case C1:
      case C2:
      case CS2_N:
      case CS3_N:
      case CS6_N:
      case CS7_N:
        Status = 0;
        break;
      default:
        break;
    }
  }
  return Status;
}

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
  )
{
  UINT8                         Ch;
  INT16                         PiIndex;
  INT16                         Offset;
  INT16                         OrgDelay[MAX_CH][SUB_CH];
  UINT32                        RankMask[MAX_CH];
  UINT8                         ErrorResult[MAX_CH][CA_CLK_TEST_RANGE];
  UINT32                        ChBitmask = 0;
  UINT32                        BwSerr[MAX_CH][3];
  UINT8                         SubChannel = 0;
  UINT8                         Strobe;
  UINT8                         DWord;
  UINT8                         SubChannelMask;
  UINT8                         SubChMSVx8;
  UINT32                        ValidMask[MAX_STROBE_DDR5];
  INT16                         RisingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16                         CenterPoint[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16                         FallingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16                         PulseWidth[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT32                        DqSampleResultStrobeSubCh[SUB_CH][MAX_STROBE_DDR5 / 2];
  struct dimmNvram              (*DimmNvList)[MAX_DIMM];
  UINT8                         Step;
  UINT32                        DqSampleResultStrobe[MAX_STROBE_DDR5 / 2];
  CHIP_GET_CMD_MGN_SWP_STRUCT   ChipGetCmdMgnSwpStruct;
  MRC_MST                       MemSsType;
  MRC_RT                        MrcRankList;
  UINT32                        OneChBitmask = 0;
  UINT8                         MaxStrobe = 0;
  UINT8                         SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8                         MaxChDdr;
  INT8                          CaVrefInitValue;
  INT8                          CaVrefValue;

  MaxChDdr = GetMaxChDdr ();
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  SubChMSVx8 = GetSubChMaxStrobeValid (Host) /2;

  ZeroMem ((UINT32 *) DqSampleResultStrobe, sizeof (DqSampleResultStrobe));

  for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
    for (Strobe = 0; Strobe < MAX_STROBE_DDR5 / 2; Strobe++) {
      DqSampleResultStrobeSubCh[SubChannel][Strobe] = 0;
    }
  }
  GetCmdMarginsSweepInit (&ChipGetCmdMgnSwpStruct);

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      RankMask[Ch] = 0;
      OrgDelay[Ch][SubChannel] = 0;
    }

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    if (Rank >= (*DimmNvList) [Dimm].numRanks) {
      continue;
    }

    // Skip CIDS if not NVMDIMM
    /*for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) { //DDR5_TODO.
      if (isCmdCtlTrainedDdr5 (Host, Socket, Ch, Dimm, SignalSubCh[Ch][SubChannel]) != (UINT32) CHECKPOINT_MINOR_EARLY_CA_CLK) {
        continue;
      }
    }*/

    // Initialize Ch Rank mask
    RankMask[Ch] = 1;
    //
    // Get the original delay
    //
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      GetSetSignalDdr5 (Host, Socket, Ch, DdrLevel, SignalSubCh[Ch][SubChannel], GSM_READ_ONLY, &OrgDelay[Ch][SubChannel]);
    }

    for (PiIndex = 0; PiIndex < CA_CLK_TEST_RANGE; PiIndex++) {
      ErrorResult[Ch][PiIndex] = 1;
    } //PiIndex loop
    //
    // Setup for CA CLK test
    //
    //
    // Indicate Rank present and enabled on this Ch.
    //
    OneChBitmask = 0;
    OneChBitmask |= 1 << Ch;
    MrcRankList.Dimm = Dimm;
    MrcRankList.Rank = Rank;
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      if (SignalSubCh[Ch][SubChannel] == A0A) {
        if (Host->DdrioUltSupport) {
          CteSetupPattern ("earlyca_loopback", SignalSubCh[Ch][SubChannel] % 14, OneChBitmask, SignalSubCh[Ch][SubChannel] > 13, GetPhyRank (Dimm, Rank));
        } else {
          SetupTest (Host, Socket, MemSsType, OneChBitmask, 1, &MrcRankList, DdrLevel, EarlyCaLoopbackTest, SignalSubCh[Ch][SubChannel], 0, FIRST_ITERATION_LOOP);
        }
      }
    }
    ChipGetCmdMgnSwpStruct.eyeSize[Ch] = 0;
    ChipGetCmdMgnSwpStruct.chEyemask |= 1 << Ch;
    ChipGetCmdMgnSwpStruct.chDonemask |= 1 << Ch;

    // Indicate Rank present and enabled on this Ch
    ChBitmask |= 1 << Ch;
  } // Ch loop
  //
  // Run concurrent tests
  //
  EarlyCmdSignalTest (Host, Socket, ChBitmask);
  //
  // Program counting window settings and enable receivers/ODT. //DDR5_TODO
  //
  SetDqCountingWindow (Host, Socket, Dimm, Rank);

  Offset = CA_CLK_STEP_SIZE;
  //
  // Sweep Pi delays
  //
  for (PiIndex = CA_CLK_TEST_START; PiIndex < (CA_CLK_TEST_RANGE + CA_CLK_TEST_START); PiIndex += Offset) {

    if (ChBitmask == 0) {
      continue;
    }
    //
    // Shift timings on each channel
    //
    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if ((RankMask[Ch] == 0)) {
        continue;
      }
      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        GetSetSignalDdr5 (Host, Socket, Ch, DdrLevel, SignalSubCh[Ch][SubChannel], GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &PiIndex);
      }//Subchannel loop
    } // Ch loop

    //
    // Generate an IO Reset. This is required because the pi delays changed.
    //
    IO_Reset (Host, Socket);
    //
    // Enabling the counter Mode. DDR5_TODO.
    //
    EnableDdrioTrainingCounter (Host, Socket, Dimm, Rank);

    ClearBwSerr (BwSerr);

    //
    // Get Test Result
    //
    DWord = (UINT8) ((PiIndex - CS_CLK_PI_START) / 32);
    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if ((RankMask[Ch] == 0)) {
        continue;
      }
      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      //
      // Continue to the next Rank if this one is disabled
      //
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
        continue;
      }

      GetTrainResultStatus (Host, Socket, 0, ChBitmask, NO_SUBCH, sizeof (DqSampleResultStrobeSubCh), (UINT32*) &DqSampleResultStrobeSubCh, ValidMask, 0, Dimm);

      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        for (Strobe = 0; Strobe < MAX_STROBE_DDR5 / 2; Strobe++) {
          if (IsDimmStrobeNotValidDdr5 (&(*DimmNvList) [Dimm], Strobe)) {
            continue;
          }
          if ((DqSampleResultStrobe[Strobe] & 0x1FF) >= CSCA_TF_COUNTING_WINDOW_VALUE / 2) {
            for (Step = 0; Step < CS_CLK_STEP_SIZE; Step++) {
              (*TrainRes) [Ch][SubChannel][Strobe].results[DWord] = (*TrainRes) [Ch][SubChannel][Strobe].results[DWord] | (1 << ((PiIndex + Step) % 32));
            } // Step loop
          } //if
        } //Strobe loop
      }//Subchannel loop

      // Fill unused feedback entries //DDR5_TODO
      /*for (i = 1; i < CS_CLK_STEP_SIZE; i++) {
      DqFeedback[Ch][PiIndex + i] = DqFeedback[Ch][PiIndex];
      }*/
    } //Ch loop
  } // PiIndex loop

  //
  // Exit CA Training Mode.
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    SubChannelMask = 0;
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      //
      // Check Subchannel is enabled or not. DDR5_TODO
      //
      SubChannelMask = SubChannelMask | (1 << SubChannel);
    } //sub channel loop
    //
    // Exit DRAM CA Training Mode.--DIMM sub channel A and B at the same time.
    //
    if (SignalSubCh[Ch][0] == A13A) {
      SendNop (Host, Socket, Ch, SubChannelMask, Dimm, Rank, 0, TRUE, TRUE);
      RcDebugPrintWithDevice (
        SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "Exit CA Training Mode\n"
        );
    }
  }

  //
  // Hanlde the training result.
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    if (!(*DimmNvList) [Dimm].x4Present) {
      MaxStrobe = SubChMSVx8;
    } else {
      MaxStrobe = SubChMSVx4;
    }
    //
    // Continue to the next Rank if this one is disabled
    //
    if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
      continue;
    }

    CaVrefInitValue = GetCaVrefInitValue (Host, Socket, Ch, Dimm, Rank, CA_VREF_STEP_COUNT, CA_VREF_STEP_SIZE);
    CaVrefValue = CaVrefInitValue + (CaVrefStep * CA_VREF_STEP_SIZE);

    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      if (GetDebugLevel () & SDBG_MAX) {
        AcquirePrintControl ();
        RcDebugPrint (SDBG_MAX, "\nSummary: Early Ca Clk training Pi\n");
        RcDebugPrintWithDevice (
          SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
          "%a, Vref %d\n",
          GetSignalStrDdr5 (SignalSubCh[Ch][SubChannel]),
          CaVrefValue
          );
        PrintSampleArrayCaClk (Host, Socket, Ch, Dimm, 0, SubChannel, CA_CLK_TEST_RANGE, TrainRes);
        ReleasePrintControl ();
      }

      for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }
        EvaluatePiSettingResultsCaClk (Host, Socket, Ch, Dimm, Rank, Strobe, RisingEdge, CenterPoint, FallingEdge, PulseWidth,
          PI_CS_CLK_TYPE, SubChannel, TrainRes);
        //
        // Calculate left edge and right edge
        //
        Le[Ch][SubChannel][Strobe] = MIN (FallingEdge[Ch][SubChannel][Strobe], RisingEdge[Ch][SubChannel][Strobe]);
        Re[Ch][SubChannel][Strobe] = MAX (FallingEdge[Ch][SubChannel][Strobe], RisingEdge[Ch][SubChannel][Strobe]);
        EyeWidthSC[Ch][SubChannel][Strobe] = Re[Ch][SubChannel][Strobe] - Le[Ch][SubChannel][Strobe];
      } // Strobe loop

      //
      // Display the left edge, right edge and CS PI value.
      //
      DisplayEdgesDDR5 (Host, Socket, Ch, EyeWidthSC[Ch], Le[Ch], Re[Ch], SubChannel, MaxStrobe);
    }//Subchannel loop
  } // Ch loop
  //
  // Restore initial pi delay
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if ((RankMask[Ch] == 0)) {
      continue;
    }
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }
        if ((Re[Ch][SubChannel][Strobe] - Le[Ch][SubChannel][Strobe]) <= 0) {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
            "ERROR: Signal %a, No eye found.\n", GetSignalStrDdr5 (SignalSubCh[Ch][SubChannel]));
          Le[Ch][SubChannel][Strobe] = 0;
          Re[Ch][SubChannel][Strobe] = 0;
        }///if
      } //Strobe loop
    } //Subchannel loop
    //
    // Restore Signal under test to its initial setting
    //
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    if (Rank >= (*DimmNvList) [Dimm].numRanks) {
      continue;
    }
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      GetSetSignalDdr5 (Host, Socket, Ch, DdrLevel, SignalSubCh[Ch][SubChannel], GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &OrgDelay[Ch][SubChannel]);
    }
  } // Ch loop
} // GetCaClkMarginsSweepVref

//
// QACS, QBCS, QACA, QBCA, QACK, QBCK, QCCK, QDCK mapping.Rank0 and Rank 1 have the same mapping.
//
UINT8 QA_CSCA_STROBE_MAPPING_2Rx4[SUB_CH] [MAX_A_B_STROBE] = {
  {0, 1, 2, 3, 4},
#if SUB_CH > 1
  {5, 6, 7, 8, 4}
#endif
};

UINT8 QB_CSCA_STROBE_MAPPING_2Rx4[SUB_CH] [MAX_A_B_STROBE] = {
  {5, 6, 7, 8, 9},
#if SUB_CH > 1
  {0, 1, 2, 3, 9}
#endif
};

UINT8 QACS0_RANK0_STROBE_MAPPING_1Rx4[SUB_CH] [MAX_A_B_STROBE] = {
  {0, 1, 2, 3, 4},
#if SUB_CH > 1
  {5, 6, 7, 8, 4}
#endif
};

UINT8 QBCS0_RANK0_STROBE_MAPPING_1Rx4[SUB_CH] [MAX_A_B_STROBE] = {
  {5, 6, 7, 8, 9},
#if SUB_CH > 1
  {0, 1, 2, 3, 9}
#endif
};

UINT8 QA_STROBE_MAPPING_x8[SUB_CH] [MAX_A_B_STROBE] = {
  {0, 1, 2, 3, 4},
#if SUB_CH > 1
  {0, 1, 2, 3, 4}
#endif
};


/**
  Get raw card mapping

  @param[in]  Host              - Pointer to sysHost
  @param[in]  Socket            - Current Socket
  @param[in]  Ch                - Channel number
  @param[in]  SubCh             - Sub Channel number
  @param[in]  Dimm              - Dimm number on Ch
  @param[in]  BacksideCopyNum   - Backside Copy Number
  @param[out] RawCardStrobe     - Raw Card Strober

  @retval N/A
**/
VOID
GetRawCardMapping (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     SubCh,
  IN  UINT8     Dimm,
  IN  UINT8     BacksideCopyNum,
  OUT UINT8     RawCardStrobe[MAX_A_B_STROBE]
  )
{
  UINT8 Strobe;
  struct dimmNvram (*DimmNvList) [MAX_DIMM];
  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  if ((*DimmNvList) [Dimm].x4Present) {
    if ((*DimmNvList) [Dimm].numRanks == 2) {
      if (BacksideCopyNum == 0) {
        for (Strobe = 0; Strobe < MAX_A_B_STROBE; Strobe++) {
          RawCardStrobe [Strobe] = QA_CSCA_STROBE_MAPPING_2Rx4[SubCh][Strobe];
        }
      } else {
        for (Strobe = 0; Strobe < MAX_A_B_STROBE; Strobe++) {
          RawCardStrobe [Strobe] = QB_CSCA_STROBE_MAPPING_2Rx4[SubCh][Strobe];
        }
      }
    } else if ((*DimmNvList) [Dimm].numRanks == 1) {
      if (BacksideCopyNum == 0) {
        for (Strobe = 0; Strobe < MAX_A_B_STROBE; Strobe++) {
          RawCardStrobe [Strobe] = QACS0_RANK0_STROBE_MAPPING_1Rx4[SubCh][Strobe];
        }
      } else {
        for (Strobe = 0; Strobe < MAX_A_B_STROBE; Strobe++) {
          RawCardStrobe [Strobe] = QBCS0_RANK0_STROBE_MAPPING_1Rx4[SubCh][Strobe];
        }
      }
    }
  } else {
    for (Strobe = 0; Strobe < MAX_A_B_STROBE; Strobe++) {
      RawCardStrobe [Strobe] = QA_STROBE_MAPPING_x8[SubCh][Strobe];
    }
  }
}

/**

  Finds CA CLK margins for the given Signal by the CA Vref setting.

  @param[in]  Host            - Pointer to sysHost
  @param[in]  Socket          - Socket number
  @param[in]  Dimm            - DIMM number
  @param[in]  Rank            - Rank number
  @param[in]  SignalSubCh     - Sub channel signal
  @param[in]  Vref            - Vref value for each channel
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
  IN  INT8      Vref[MAX_CH],
  OUT INT16     Le[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  OUT INT16     Re[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN  INT16     EyeWidthSC[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN  struct    ClkTrainingResults (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN  INT16     CaLeftCopyNum[MAX_CH][SUB_CH][COPY_NUM],
  IN  INT16     CaRightCopyNum[MAX_CH][SUB_CH][COPY_NUM]
  )
{
  UINT8                        Ch;
  UINT16                       PiIndex;
  UINT16                       PiIndexTemp;
  UINT8                        Offset;
  UINT8                        OrgDelay[MAX_CH][SUB_CH];
  UINT32                       RankMask[MAX_CH];
  UINT8                        ErrorResult[MAX_CH][CA_CLK_TEST_RANGE_BACKSIDE];
  UINT32                       ChBitmask = 0;
  UINT32                       BwSerr[MAX_CH][3];
  EFI_STATUS                   Status = 0;
  UINT8                        SubChannel = 0;
  UINT8                        Strobe;
  UINT8                        DWord;
  UINT8                        SubChMSVx8;
  UINT32                       ValidMask[MAX_STROBE_DDR5];
  INT16                        RisingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16                        CenterPoint[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16                        FallingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16                        PulseWidth[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT32                       DqSampleResultStrobeSubCh[SUB_CH][MAX_STROBE_DDR5 / 2];
  struct dimmNvram (*DimmNvList) [MAX_DIMM];
  UINT8                        Step;
  CHIP_GET_CMD_MGN_SWP_STRUCT  ChipGetCmdMgnSwpStruct;
  MRC_MST                      MemSsType;
  MRC_RT                       MrcRankList;
  struct channelNvram          (*ChannelNvList) [MAX_CH];
  UINT32                       OneChBitmask = 0;
  UINT8                        MaxStrobe = 0;
  UINT8                        SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8                        MaxChDdr;
  GSM_CSN                      SignalToShift = 0;
  INT16                        QCsDelay;
  UINT16                       QcaSweepRange = CA_CLK_TEST_RANGE_BACKSIDE_2N;
  UINT8                        BacksideCopy;
  INT16                        EyeWidthSCCopyNum[MAX_CH][SUB_CH];
  UINT8                        RawCardStrobe[MAX_A_B_STROBE];
  UINT32                       UseI3cMrw;
  BOOLEAN                      EnableI3cMrw = TRUE;
  DDR5_RCD_QACA_OUTPUT_DELAY_STRUCT QxcaOutputDelay;  // "QACA" structure works for both QACA and QBCA

  MaxChDdr = GetMaxChDdr ();
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  SubChMSVx8 = GetSubChMaxStrobeValid (Host) / 2;
  ChannelNvList = GetChannelNvList (Host, Socket);

  ZeroMem ((UINT8 *) RawCardStrobe, sizeof (RawCardStrobe));
  ZeroMem ((UINT32 *) DqSampleResultStrobeSubCh, sizeof (DqSampleResultStrobeSubCh));

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {
    Status = CteGetOptions ("use_i3c", &UseI3cMrw);
    if (!EFI_ERROR (Status)) {
      if (UseI3cMrw != 0) {
        EnableI3cMrw = TRUE;
      } else {
        EnableI3cMrw = FALSE;
      }
    } else {
      EnableI3cMrw = FALSE;
    }
  }

  GetCmdMarginsSweepInit (&ChipGetCmdMgnSwpStruct);

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      RankMask[Ch] = 0;
      OrgDelay[Ch][SubChannel] = 0;
    }

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    if ((*ChannelNvList) [Ch].timingMode == TIMING_1N) {
      QcaSweepRange = CA_CLK_TEST_RANGE_BACKSIDE_1N;
    } else {
      QcaSweepRange = CA_CLK_TEST_RANGE_BACKSIDE_2N;
    }
    if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    if (Rank >= (*DimmNvList) [Dimm].numRanks) {
      continue;
    }

    // Skip CIDS if not NVMDIMM
    /*for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) { //DDR5_TODO.
    if (isCmdCtlTrainedDdr5 (Host, Socket, Ch, Dimm, SignalSubCh[Ch][SubChannel]) != (UINT32) CHECKPOINT_MINOR_EARLY_CA_CLK) {
    continue;
    }
    }*/

    // Initialize Ch Rank mask
    RankMask[Ch] = 1;

    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      //
      // Get the original delay. The QACA and QBCA default value should be 0? DDR5_TODO.
      //
      if (SignalSubCh[Ch][SubChannel] <= Q0ACA13) {
        ReadRcdCwCache (Socket, Ch, SubChannel, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_QACA_OUTPUT_DELAY_REG, &QxcaOutputDelay.Data);
      } else if (SignalSubCh[Ch][SubChannel] >= Q0BCA0) {
        ReadRcdCwCache (Socket, Ch, SubChannel, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_QBCA_OUTPUT_DELAY_REG, &QxcaOutputDelay.Data);
      } //DDR5_TODO. Merge it into GetSetSignalDdr5.
      OrgDelay[Ch][SubChannel] = QxcaOutputDelay.Bits.output_delay_control_for_qaca_output_signals;
      if (QxcaOutputDelay.Bits.full_cycle_delay == 1) {
        OrgDelay[Ch][SubChannel] += MAX_OUTPUT_DELAY_CONTROL_FOR_QXCA;
      }
    }

    for (PiIndex = 0; PiIndex < CA_CLK_TEST_RANGE_BACKSIDE; PiIndex++) {
      ErrorResult[Ch][PiIndex] = 1;
    } //PiIndex loop
    //
    // Setup for CA CLK test
    //
    //
    // Indicate Rank present and enabled on this Ch.
    //
    OneChBitmask = 0;
    OneChBitmask |= 1 << Ch;
    //
    // Indicate Rank present and enabled on this Ch
    //
    ChBitmask |= 1 << Ch;
    MrcRankList.Dimm = Dimm;
    MrcRankList.Rank = Rank;
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      if (Host->DdrioUltSupport) {
        CteSetupPattern ("earlyca_backside_loopback", (SignalSubCh[Ch][SubChannel] >= Q1ACA0) ? SignalSubCh[Ch][SubChannel] - Q1ACA0 : SignalSubCh[Ch][SubChannel] - Q0ACA0, ChBitmask, (SignalSubCh[Ch][SubChannel] >= Q1ACA0) ? 0x2 : 0x1, GetPhyRank (Dimm, Rank));
      } else {
        SetupTest (Host, Socket, MemSsType, OneChBitmask, 1, &MrcRankList, DdrLevel, EarlyCaBacksideLoopbackTest, SignalSubCh[Ch][SubChannel], 0, FIRST_ITERATION_LOOP);
      }
    }
    ChipGetCmdMgnSwpStruct.eyeSize[Ch] = 0;
    ChipGetCmdMgnSwpStruct.chEyemask |= 1 << Ch;
    ChipGetCmdMgnSwpStruct.chDonemask |= 1 << Ch;
  } // Ch loop

  if ((FeaturePcdGet (PcdCteBuild) != TRUE) || (EnableI3cMrw)) {
    if (Host->DdrioUltSupport) {
      CteStartPattern (ChBitmask, TWO_SUBCH_MASK, GetPhyRank (Dimm, Rank)); //Ch, sc, Rank
    } else {
      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        TestControl (Host, Socket, MemSsType, ChBitmask, SubChannel, EarlyCaBacksideLoopbackTest, 1, START_TEST, NOT_STOP_TEST);
      }
    }
  }
  //
  // Set the DQ count window
  //
  SetDataTrainCountWindow (Host, Socket, Dimm, Rank, DQ_COUNTING_WINDOW_VALUE);
  SetDqCountingWindow (Host, Socket, Dimm, Rank);
  Offset = CA_CLK_STEP_SIZE_BACKSIDE;
  //
  // Sweep Pi delays
  //
  for (PiIndex = CA_CLK_TEST_START_BACKSIDE; PiIndex < (QcaSweepRange + CA_CLK_TEST_START_BACKSIDE); PiIndex += Offset) {

    if (ChBitmask == 0) {
      continue;
    }
    if (PiIndex == CA_CLK_TEST_START_BACKSIDE + QCA_EXTRA_SWEEP_RANGE) {
      //
      // Add the CS by 1tck.
      //
      for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }
        if ((*ChannelNvList) [Ch].timingMode == TIMING_2N) {
          for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
            //
            // shift the CS by 1TCK delay. DDR5_todo. To cosnide the backside copy side?
            //
            SignalToShift = QCS0_A_n + GET_QCS_INDEX (SubChannel, Rank, Dimm);
            QCsDelay = - BACKSIDE_1TCK;
            for (BacksideCopy = 0; BacksideCopy < GetMaxBacksideCopyNum (Socket, Ch, Dimm); BacksideCopy++) {
              GetSetQcsDelay (Host, Socket, Ch, Dimm, Rank, SignalToShift, BacksideCopy, GSM_WRITE_OFFSET | GSM_FORCE_WRITE, &QCsDelay, NULL, NULL);
            }
            SignalToShift = Q0ACK + GET_QCS_INDEX (SubChannel, Rank, Dimm);
          }
        }
      }
    }
    //

    //
    // Shift timings on each channel
    //
    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if ((RankMask[Ch] == 0)) {
        continue;
      }
      //
      //Set the pi first, and then kick off the CADBpattern, get training result. disable the pattern. program the pi....
      //
      PiIndexTemp = PiIndex;
      if ((*ChannelNvList) [Ch].timingMode == TIMING_2N) {
        if (PiIndex >= QCA_EXTRA_SWEEP_RANGE) {
          PiIndexTemp = PiIndex - QCA_EXTRA_SWEEP_RANGE;
        }
      }
      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        ProgramQcaDelay (Host, Socket, Ch, Dimm, SubChannel, Rank, SignalSubCh[Ch][SubChannel], GSM_FORCE_WRITE, PiIndexTemp&0xFF, TRUE);
      }
    } // Ch loop
    //

    if ((FeaturePcdGet (PcdCteBuild) == TRUE) && (!EnableI3cMrw)) {

      //
      // Setup test first
      //
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if ((RankMask[Ch] == 0)) {
          continue;
        }
        OneChBitmask = 0;
        OneChBitmask |= 1 << Ch;
        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          if (Host->DdrioUltSupport) {
            CteSetupPattern ("earlyca_backside_loopback", (SignalSubCh[Ch][SubChannel] >= Q1ACA0) ? SignalSubCh[Ch][SubChannel] - Q1ACA0 : SignalSubCh[Ch][SubChannel] - Q0ACA0, ChBitmask, (SignalSubCh[Ch][SubChannel] >= Q1ACA0) ? 0x2 : 0x1, GetPhyRank (Dimm, Rank));
          } else {
            SetupTest (Host, Socket, MemSsType, OneChBitmask, 1, &MrcRankList, DdrLevel, EarlyCaBacksideLoopbackTest, SignalSubCh[Ch][SubChannel], 0, FIRST_ITERATION_LOOP);
          }
        }
      } //Ch loop

      if (Host->DdrioUltSupport) {
        CteStartPattern (ChBitmask, TWO_SUBCH_MASK, GetPhyRank (Dimm, Rank)); //Ch, sc, Rank
      } else {
        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          //
          //start the pattern here.
          //
          TestControl (Host, Socket, MemSsType, ChBitmask, SubChannel, EarlyCaBacksideLoopbackTest, 1, START_TEST, NOT_STOP_TEST);
        }
      }

    }
    //
    // Generate an IO Reset.
    //
    IO_Reset (Host, Socket); // The MC reset is needed to enlarge the count window assert pulse to make sure the count winodw value can be reset to 0.//SPR_TODO. SPR should also need this function?
    //
    // Enabling the counter Mode.
    //
    EnableDdrioTrainingCounter (Host, Socket, Dimm, Rank);

    ClearBwSerr (BwSerr);

    //
    // Get Test Result
    //
    DWord = (UINT8) ((PiIndex - CA_CLK_TEST_START_BACKSIDE) / 32);
    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if ((RankMask[Ch] == 0)) {
        continue;
      }
      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      //
      // Continue to the next Rank if this one is disabled
      //
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
        continue;
      }
      //
      // Get Training Results.
      //
      GetTrainResultStatus (Host, Socket, 0, ChBitmask, NO_SUBCH, sizeof (DqSampleResultStrobeSubCh), (UINT32*) &DqSampleResultStrobeSubCh, ValidMask, 0, Dimm);
      //
      // Stop the pattern.
      //
      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        for (Strobe = 0; Strobe < MAX_STROBE_DDR5 / 2; Strobe++) {
          if (IsDimmStrobeNotValidDdr5 (&(*DimmNvList) [Dimm], Strobe)) {
            continue;
          }
          if ((DqSampleResultStrobeSubCh[SubChannel][Strobe] & 0x1FF) >= DQ_COUNTING_WINDOW_VALUE / 2) {
            for (Step = 0; Step < CA_CLK_STEP_SIZE_BACKSIDE; Step++) {
              (*TrainRes) [Ch][SubChannel][Strobe].results[DWord] = (*TrainRes) [Ch][SubChannel][Strobe].results[DWord] | (1 << ((PiIndex + Step) % 32));
            } // Step loop
          } //if
        } //Strobe loop
      }//Subchannel loop

      // Fill unused feedback entries //DDR5_TODO
       /*for (i = 1; i < CS_CLK_STEP_SIZE; i++) {
       DqFeedback[Ch][PiIndex + i] = DqFeedback[Ch][PiIndex];
       }*/
    } //Ch loop

    if ((FeaturePcdGet (PcdCteBuild) == TRUE)&& (!EnableI3cMrw)) {
      //
      // Stop Test
      //
      if (Host->DdrioUltSupport) {
        CteStopPattern ();
      } else {
        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          TestControl (Host, Socket, MemSsType, ChBitmask, SubChannel, EarlyCaBacksideLoopbackTest, 1, NOT_START_TEST, STOP_TEST);
        }
      }
    }
  } // PiIndex loop

  //
  // Stop Test
  //
  if (Host->DdrioUltSupport) {
    CteStopPattern ();
  } else {
    if ((FeaturePcdGet (PcdCteBuild) != TRUE) || (EnableI3cMrw)) {
      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        TestControl (Host, Socket, MemSsType, ChBitmask, SubChannel, EarlyCaBacksideLoopbackTest, 1, NOT_START_TEST, STOP_TEST);
      }
    }
  }
  RcDebugPrint (SDBG_MAX, "Add the CS by 1tck.\n");
  //
  // Add the CS by 1tck.
  //
  for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    if ((*ChannelNvList) [Ch].timingMode == TIMING_2N) {
      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        //
        // shift the CS by 1TCK delay.
        //
        //for (CopySide = 0; CopySide < COPY_NUM; CopySide++) {
        SignalToShift = CsToSweepBackSide [ GetBacksideCsSignalIndex (Ch, SubChannel, Dimm, Rank) ];
        QCsDelay = BACKSIDE_1TCK;
        for (BacksideCopy = 0; BacksideCopy < GetMaxBacksideCopyNum (Socket, Ch, Dimm); BacksideCopy++) {
          GetSetQcsDelay (Host, Socket, Ch, Dimm, Rank, SignalToShift, BacksideCopy, GSM_WRITE_OFFSET | GSM_FORCE_WRITE, &QCsDelay, NULL, NULL);
        }
        SignalToShift = Q0ACK + SubChannel * 4 + Rank + Dimm * 2;
      }
    }
  }
  //
  // Exit CA Training Mode.
  //

  //
  // Hanlde the training result.
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    if (!(*DimmNvList) [Dimm].x4Present) {
      MaxStrobe = SubChMSVx8;
    } else {
      MaxStrobe = SubChMSVx4;
    }
    //
    // Continue to the next Rank if this one is disabled
    //
    if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) continue;
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      if (GetDebugLevel () & SDBG_MAX) {
        AcquirePrintControl ();
        RcDebugPrint (SDBG_DEFAULT, "\nSummary: Early Ca Clk Backside Training Pi\nS%d, Ch%d, DIMM%d, SC%d, Rank%d, %a, Vref %d\n",
          Socket, Ch, Dimm, SubChannel, Rank, GetSignalStrDdr5 (SignalSubCh[Ch][SubChannel]), Vref[Ch]);
        PrintSampleArrayCaClk (Host, Socket, Ch, Dimm, 0, SubChannel, QcaSweepRange, TrainRes);
        ReleasePrintControl ();
      }

      for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }
        EvaluatePiSettingResultsCaClk (Host, Socket, Ch, Dimm, Rank, Strobe, RisingEdge, CenterPoint, FallingEdge, PulseWidth,
          PI_CS_CLK_TYPE, SubChannel, TrainRes);
        //
        // Calculate left edge and right edge
        //
        Le[Ch][SubChannel][Strobe] = MIN (FallingEdge[Ch][SubChannel][Strobe], RisingEdge[Ch][SubChannel][Strobe]);
        Re[Ch][SubChannel][Strobe] = MAX (FallingEdge[Ch][SubChannel][Strobe], RisingEdge[Ch][SubChannel][Strobe]);
        EyeWidthSC[Ch][SubChannel][Strobe] = Re[Ch][SubChannel][Strobe] - Le[Ch][SubChannel][Strobe];
      } // Strobe loop
      //
      // Display the left edge, right edge and CS PI value.
      //
      DisplayEdgesDDR5 (Host, Socket, Ch, EyeWidthSC[Ch], Le[Ch], Re[Ch], SubChannel, MaxStrobe);
      if ((*ChannelNvList) [Ch].timingMode == TIMING_2N) {
        RcDebugPrint (SDBG_MAX, "After Recalculation\n");


        for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
          //
          // Skip if this is an ECC Strobe when ECC is disabled
          //
          if (IsStrobeNotValidDdr5 (Host, Strobe)) {
            continue;
          }
          //
          // Calculate right edge and eyewidth
          //
          Re[Ch][SubChannel][Strobe] = Re[Ch][SubChannel][Strobe] - 64;
          EyeWidthSC[Ch][SubChannel][Strobe] = Re[Ch][SubChannel][Strobe] - Le[Ch][SubChannel][Strobe];
        } // Strobe loop

        //
        // Display the left edge, right edge and CS PI value.
        //
        DisplayEdgesDDR5 (Host, Socket, Ch, EyeWidthSC[Ch], Le[Ch], Re[Ch], SubChannel, MaxStrobe);
      }


      //
      // Get the Strobe mapping. And pick up the A, B copy related Strobe results into A, B result array.
      //
      for (BacksideCopy = 0; BacksideCopy < COPY_NUM; BacksideCopy++) {
        //
        // Get the raw card mapping.
        //
        if ((!((*DimmNvList) [Dimm].x4Present)) && (BacksideCopy == COPY_B)) {
          continue;
        }
        GetRawCardMapping (Host, Socket, Ch, SubChannel, Dimm, BacksideCopy, RawCardStrobe);
        //
        // Display the left edge, right edge and eye width.
        //
        if (((*DimmNvList) [Dimm].x4Present) && ((*DimmNvList) [Dimm].numRanks == 1)) {
          if (BacksideCopy == 0) {
            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              "A Copy Signal--%a with QACK\n", GetSignalStrDdr5 (SignalSubCh[Ch][SubChannel]));
          } else {
            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              "A Copy Signal--%a with QCCK\n", GetSignalStrDdr5 (SignalSubCh[Ch][SubChannel]));
          }
        } else {
          if (BacksideCopy == 0) {
            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              "A Copy Signal--%a\n", GetSignalStrDdr5 (SignalSubCh[Ch][SubChannel]));
          } else {
            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              "B Copy Signal--%a\n", GetSignalStrDdr5 (SignalSubCh[Ch][SubChannel] + QCA_DISTANCE_A_B_COPY));
          }
        }
        DisplayBacksideEdgesDDR5 (Host, Socket, Ch, EyeWidthSC[Ch], Le[Ch], Re[Ch], SubChannel, RawCardStrobe);

        //
        // Calculate the composite eye for the A, B copy signals.
        //
        GetCaClkCompositeEyeBackside (Le[Ch][SubChannel], Re[Ch][SubChannel], &CaLeftCopyNum[Ch][SubChannel][BacksideCopy], &CaRightCopyNum[Ch][SubChannel][BacksideCopy], RawCardStrobe);
        EyeWidthSCCopyNum[Ch][SubChannel] = ABS (CaRightCopyNum[Ch][SubChannel][BacksideCopy] - CaLeftCopyNum[Ch][SubChannel][BacksideCopy]);
        //
        // Display the left edge, right edge and eye width.
        //
        DisplayCompositeCaEyeDDR5 (Host, Socket, Ch, CaLeftCopyNum[Ch][SubChannel][BacksideCopy], CaRightCopyNum[Ch][SubChannel][BacksideCopy], EyeWidthSCCopyNum[Ch], SubChannel, MaxStrobe);
      }//BacksideCopy
      //
      // Get the composite eye for 1Rx4 config.
      //
      if (((*DimmNvList) [Dimm].x4Present) && ((*DimmNvList) [Dimm].numRanks == 1)) {
        if (CaLeftCopyNum[Ch][SubChannel][COPY_A] < CaLeftCopyNum[Ch][SubChannel][COPY_B]) {
          CaLeftCopyNum[Ch][SubChannel][COPY_A] = CaLeftCopyNum[Ch][SubChannel][COPY_B];
        }
        if (CaRightCopyNum[Ch][SubChannel][COPY_A] > CaRightCopyNum[Ch][SubChannel][COPY_B]) {
          CaRightCopyNum[Ch][SubChannel][COPY_A] = CaRightCopyNum[Ch][SubChannel][COPY_B];
        }
        EyeWidthSCCopyNum[Ch][SubChannel] = ABS (CaRightCopyNum[Ch][SubChannel][COPY_A] - CaLeftCopyNum[Ch][SubChannel][COPY_A]);
        //
        // Display the results.
        // Print Final Composite Left Edge
        //
        RcDebugPrint (SDBG_MEM_TRAIN, " Final Composite Left Edge:  ");
        RcDebugPrint (SDBG_MEM_TRAIN, " %3d", CaLeftCopyNum[Ch][SubChannel][COPY_A]);

        //
        // Print Final Composite Right Edge
        //
        RcDebugPrint (SDBG_MEM_TRAIN, "\nFinal Composite Right Edge: ");
        RcDebugPrint (SDBG_MEM_TRAIN, " %3d", CaRightCopyNum[Ch][SubChannel][COPY_A]);

        //
        // Print Final Composite Eye Width
        //
        RcDebugPrint (SDBG_MEM_TRAIN, "\nFinal Composite Eye Width: ");
        RcDebugPrint (SDBG_MEM_TRAIN, "  %3d", EyeWidthSCCopyNum[Ch][SubChannel]);

      }

    }//Subchannel loop
  } // Ch loop

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if ((RankMask[Ch] == 0)) {
      continue;
    }
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }
        //
        // Check the EW.
        //
        if ((Re[Ch][SubChannel][Strobe] - Le[Ch][SubChannel][Strobe]) <= MinimumMargin (Host, QCA)) {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
            "ERROR: Signal %a, No eye found.\n", GetSignalStrDdr5 (SignalSubCh[Ch][SubChannel]));
          Le[Ch][SubChannel][Strobe] = 0;
          Re[Ch][SubChannel][Strobe] = 0;
        }///if
      } //Strobe loop
    } //Subchannel loop
    //
    // Restore Signal under test to its initial setting
    //
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    if (Rank >= (*DimmNvList) [Dimm].numRanks) {
      continue;
    }
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      ProgramQcaDelay (Host, Socket, Ch, Dimm, SubChannel, Rank, SignalSubCh[Ch][SubChannel], GSM_FORCE_WRITE , OrgDelay[Ch][SubChannel], TRUE);
      //GetSetSignalDdr5(Host, Socket, Ch, DdrLevel, SignalSubCh[Ch][SubChannel], GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &OrgDelay[Ch][SubChannel]);
    } //Subchannel loop.
  } // Ch loop
} // GetCaClkMarginsSweepVrefBackside

/*

  Return the cache value for the current QCA signal

  @param[in]      Host    Pointer to SysHost
  @param[in]      Socket  Current socket under test (0-based)
  @param[in]      Channel Current channel under test (0-based)
  @param[in]      Dimm    Current dimm under test (0-based)
  @param[in]      Rank    Current rank under test (0-based)
  @param[in]      Signal  Current signal under test (expected to be between Q0ACA0-->Q1ACA13)
  @param[in]      UpdateCache   TRUE: Updates cache in global Structure, FALSE: Skips update
  @param[in,out]  Value   Pointer to memory location that contains the value to be written

  @return   The cache value for the current QCA signal

*/
UINT8
GetUpdateCachedQcaDelay (
  IN  OUT   PSYSHOST    Host,
  IN        UINT8       Socket,
  IN        UINT8       Channel,
  IN        UINT8       Dimm,
  IN        UINT8       Rank,
  IN        GSM_CSN     Signal,
  IN        BOOLEAN     UpdateCache,
  IN        UINT8       *Value    OPTIONAL
  )
{
  DDR5_RCD_QACA_OUTPUT_DELAY_STRUCT QxcaOutputDelay;
  UINT8                             RetVal;

  //
  // Program Q0ACA. DDR5_TODO. Update it once getting the final mapping from MIO.
  //

  ReadRcdCwCache (Socket, Channel, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_QACA_OUTPUT_DELAY_REG, &QxcaOutputDelay.Data);

  if (UpdateCache && Value != NULL) {
    if (*Value >= MAX_OUTPUT_DELAY_CONTROL_FOR_QXCA) {
      QxcaOutputDelay.Bits.output_delay_control_for_qaca_output_signals = *Value - MAX_OUTPUT_DELAY_CONTROL_FOR_QXCA;
      QxcaOutputDelay.Bits.full_cycle_delay = 1;
    } else {
      QxcaOutputDelay.Bits.output_delay_control_for_qaca_output_signals = *Value;
    }
    WriteRcdCwCache (Socket, Channel, TWO_SUBCH_MASK, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_QACA_OUTPUT_DELAY_REG, QxcaOutputDelay.Data);
  }

  RetVal = QxcaOutputDelay.Bits.output_delay_control_for_qaca_output_signals;
  RetVal += QxcaOutputDelay.Bits.full_cycle_delay * MAX_OUTPUT_DELAY_CONTROL_FOR_QXCA;
  return RetVal;
}

/**

  Program QCA Delay.

  @param Host                  - Pointer to sysHost
  @param Socket                - Socket number
  @param Ch                    - Channel number
  @param Dimm                  - Dimm number
  @param SubCh                 - Subchannel number
  @param Rank                  - Rank number
  @param Signal                - The signal to program
  @param Mode                  - Mode
  @param Delay                 - Delay value
  @param PassThroughMode       - It's in pass through mode or not

  @retval SUCCESS or FAILURE

**/
UINT32
ProgramQcaDelay (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     SubCh,
  IN UINT8     Rank,
  IN GSM_CSN   Signal,
  IN UINT8     Mode,
  IN INT8      Delay,
  IN BOOLEAN   PassThroughMode
  )
{
  UINT32                  Status = SUCCESS;
  UINT8                   QxcaDelay;
  UINT8                   SubChMask;
  struct dimmNvram        (*DimmNvList) [MAX_DIMM];
  UINT8                   LocalRank = 0;
  UINT8                   RcdRegisterAddress;
  UINT8                   RcdSelector;
  RAW_CARD_SELECTION      RawCardSelection;
  QCX_SIGNAL_RAW_CARD_MAP *SelectedRawCard;
  DDR5_RCD_QACA_OUTPUT_DELAY_STRUCT QxcaOutputDelay;  // "QACA" structure works for both QACA and QBCA

  if (SubCh > SUB_CH) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT,
      "ProgramQcaDelay: Invalid SubCh");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_93);
  }
  SubChMask = 1 << SubCh;

  if (PassThroughMode) {
    if (Rank == 0) {
      LocalRank = 1;
    } else {
      LocalRank = 0;
    }
  } else {
    LocalRank = Rank;
  }

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  RawCardSelection.Data = 0;

  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  if ((*DimmNvList) [Dimm].numRanks >= 2) {
    RawCardSelection.Bits.DRSR = 1;
  }
  if ((*DimmNvList) [Dimm].x4Present) {
    RawCardSelection.Bits.X4X8 = 1;
  }

  SelectedRawCard = RawCardsQca[RawCardSelection.Data];

  for (RcdSelector = 0; RcdSelector < SelectedRawCard[Rank].MaxRcdRegisters; RcdSelector++) {

    RcdRegisterAddress = SelectedRawCard[Rank].RcdRegisters[RcdSelector];

    QxcaOutputDelay.Data = 0;

    if (Mode & GSM_WRITE_OFFSET) {
      ReadRcdCwCache (Socket, Ch, SubCh, Dimm, RDIMM_CW_PAGE0, RcdRegisterAddress, &QxcaOutputDelay.Data);
      QxcaDelay = QxcaOutputDelay.Bits.output_delay_control_for_qaca_output_signals + Delay;
    } else {
      QxcaDelay = Delay;
    }

    if (QxcaDelay >= MAX_OUTPUT_DELAY_CONTROL_FOR_QXCA) {
      QxcaDelay -= MAX_OUTPUT_DELAY_CONTROL_FOR_QXCA;
      QxcaOutputDelay.Bits.full_cycle_delay = 1;
    } else {
      QxcaOutputDelay.Bits.full_cycle_delay = 0;
    }

    QxcaOutputDelay.Bits.output_delay_control_for_qaca_output_signals = QxcaDelay;
    QxcaOutputDelay.Bits.output_delay_feature_enable_for_qaca = 1;

    WriteRcdCwCache (Socket, Ch, SubChMask, Dimm, RDIMM_CW_PAGE0, RcdRegisterAddress, QxcaOutputDelay.Data);
    RcdControlWordWrite (Socket, Ch, SubChMask, Dimm, Rank, RcdRegisterAddress, QxcaOutputDelay.Data);
  }
  //
  // When feature is enabled the delay settings in RW1B[6:0] require a time of tODU for the delay to become stable on the outputs
  //
  //DDR5_TODO. To confirm--need to add a delay here so that it becomes stable?
  return Status;
}


/**

  Program Final QCA Delay result.

  @param[in] Host                  - Pointer to sysHost
  @param[in] Socket                - Socket number
  @param[in] Ch                    - Channel number
  @param[in] Dimm                  - Dimm number
  @param[in] SubCh                 - Subchannel number
  @param[in] Rank                  - Rank number
  @param[in] BacksideCopy          - The signal backside copy number
  @param[in] Mode                  - Mode
  @param[in] Delay                 - Delay value
  @param[in] PassThroughMode       - It's in pass through mode or not

  @retval SUCCESS or FAILURE

**/
UINT32
EFIAPI
ProgramFinalQcaDelayResult (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     SubCh,
  IN UINT8     Rank,
  IN UINT8     BacksideCopy,
  IN UINT8     Mode,
  IN INT8      Delay,
  IN BOOLEAN   PassThroughMode
  )
{
  UINT32                  Status = SUCCESS;
  UINT8                   QcxValue;
  UINT8                   SubChMask;
  struct dimmNvram        (*DimmNvList) [MAX_DIMM];
  UINT8                   FullCycleDelay = 0;
  UINT32                  SmbData = 0;
  UINT8                   LocalRank = 0;
  UINT8                   RcdRegisterAddress;
  RAW_CARD_SELECTION      RawCardSelection;

  if (SubCh >= SUB_CH) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT,
      "ProgramQcaDelay: Invalid SubCh");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_93);
  }
  SubChMask = 1 << SubCh;

  if (PassThroughMode) {
    if (Rank == 0) {
      LocalRank = 1;
    } else {
      LocalRank = 0;
    }
  } else {
    LocalRank = Rank;
  }

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  RawCardSelection.Data = 0;

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  if (((*DimmNvList) [Dimm].x4Present) && ((*DimmNvList) [Dimm].numRanks == 2)) {
    if (BacksideCopy == COPY_A) {
#ifdef USE_LATEST_RCD_SPEC
      RcdRegisterAddress = DDR5_RCD_QACA_OUTPUT_DELAY_REG;
#else
      RcdRegisterAddress = RDIMM_RW1B;
#endif
    } else {
      RcdRegisterAddress = DDR5_RCD_QBCA_OUTPUT_DELAY_REG;
    }
  } else {
#ifdef USE_LATEST_RCD_SPEC
    RcdRegisterAddress = DDR5_RCD_QACA_OUTPUT_DELAY_REG;
#else
    RcdRegisterAddress = RDIMM_RW1B;
#endif
  }


  if (Mode & GSM_WRITE_OFFSET) {

    ReadRcdCwCache (Socket, Ch, SubCh, Dimm, RDIMM_CW_PAGE0, RcdRegisterAddress, &QcxValue);
    QcxValue += Delay;

    if (QcxValue > 0x3F) {
      QcxValue = 0x3F;
    }

    WriteRcdCwCache (Socket, Ch, SubChMask, Dimm, RDIMM_CW_PAGE0, RcdRegisterAddress, QcxValue);

    SmbData = QcxValue | BIT7;
    RcdControlWordWrite (Socket, Ch, SubChMask, Dimm, LocalRank, RcdRegisterAddress, (UINT8) SmbData);

  } else {
    //
    // direct delay value program.
    //
    if (Delay >= 64) {
      Delay = Delay - 64;
      FullCycleDelay = BIT6;
    }

    QcxValue = ((Delay | FullCycleDelay) | BIT7);

    WriteRcdCwCache (Socket, Ch, SubChMask, Dimm, RDIMM_CW_PAGE0, RcdRegisterAddress, QcxValue);

    SmbData = QcxValue;
    RcdControlWordWrite (Socket, Ch, SubChMask, Dimm, LocalRank, RcdRegisterAddress, (UINT8) SmbData);
  }
  //
  // When feature is enabled the delay settings in RW1B[6:0] require a time of tODU for the delay to become stable on the outputs
  //
  TrainingDelay (Host, 100, 1);
  return Status;
}

/**

Find ca margins for the given Signal for rdimm.

  @param[in]  Host                 - Pointer to sysHost
  @param[in]  Socket               - Socket number
  @param[in]  MemSsType            - Mmemory subsystem type
  @param[in]  Channel              - Channel number. ALL_CH for training all channels in parallel.
  @param[in]  Dimm                 - DIMM number
  @param[in]  Subch                - sub channel number.
  @param[in]  Rank                 - Rank number
  @param[in]  Signal               - Signal
  @param[in]  CaVrefValue          - The CA VREF value.
  @param[out] Le                   - Left edge
  @param[out] Re                   - Right edge
  @param[in]  TrainingMode         - The training mode. (parralele or serial)
  @param[in]  RetrainSubChMask     - Retraining sub channel mask
  @param[in]  RetrainLoopCount     - The retraining loop count.
  @param[in]  PatternLoop          - Pattern Loop

@retval N/A

**/
VOID
GetCaClkRcdMarginsSweepVref (
  IN  PSYSHOST      Host,
  IN  UINT8         Socket,
  IN  MRC_MST       MemSsType,
  IN  UINT8         Channel,
  IN  UINT8         Dimm,
  IN  UINT8         Subch,
  IN  UINT8         Rank,
  IN  GSM_CSN       Signal,
  OUT INT16         Le[MAX_CH][SUB_CH],
  OUT INT16         Re[MAX_CH][SUB_CH],
  IN  UINT8         TrainingMode,
  IN  UINT8         RetrainSubChMask,
  IN  UINT8         RetrainLoopCount,
  IN  UINT8         Patternloop
  )
{
  UINT8                       Ch;
  INT16                       PiIndex;
  INT16                       Offset;
  UINT16                      EyeWidth;
  MRC_RT                      MrcRankList;
  INT16                       OrgDelay[MAX_CH][SUB_CH];
  UINT32                      RankMask[MAX_CH];
  UINT8                       ErrorResult[MAX_CH][SUB_CH][CA_CLK_TEST_RANGE];
  UINT16                      PiPosition[MAX_CH];
  UINT32                      ChBitmask = 0;
  UINT32                      Status = 0;
  UINT32                      OnechBitmask = 0;
  UINT8                       AlertFeedback[MAX_CH][SUB_CH][CA_CLK_TEST_RANGE];
  UINT32                      ChannelMask = 0;
  UINT32                      Err = 0;
  UINT32                      ValidMask = 0;
  struct cmdEye               CurEye[MAX_CH][SUB_CH];
  struct dimmNvram            (*DimmNvList) [MAX_DIMM];
  CHIP_GET_CMD_MGN_SWP_STRUCT ChipGetCmdMgnSwpStruct;
  GSM_CSN                     SignalLocal;
  UINT8                       SubChMask = 0;
  UINT8                       SubChTemp = 0;
  UINT8                       CaSubChGap = 0;
  UINT16                      Step = 0;
  UINT8                       InvertFlag[SUB_CH];
  UINT8                       MaxChDdr;
  MRC_TT                      TestType;

  MaxChDdr = GetMaxChDdr ();
  GetCmdMarginsSweepInit (&ChipGetCmdMgnSwpStruct);//DDR5_TODO.

  if (GetCurrentTestType (Socket) == DcaDckDutyCycleTest) {
    TestType = DcaDckDutyCycleTest;
  } else {
    TestType = EarlyCaLoopbackTest;
  }

  //
  // Program the RCD feedback selection.
  //
  SetRcdTrainFeedbackSelection (Host, Socket, Dimm, Rank, Patternloop);

  if (TrainingMode == RUN_IN_PARALLEL) {
    SubChMask = TWO_SUBCH_MASK;
    CaSubChGap = A0B - A0A;
    if (RetrainLoopCount != 0) {
      //
      // It's not the first training loop.
      //
      SubChMask = RetrainSubChMask;
    }
  } else {
    SubChMask = 1 << Subch;
    CaSubChGap = 0;
  }
  ZeroMem ((UINT8 *) OrgDelay, sizeof (OrgDelay));
  ZeroMem ((UINT8 *) AlertFeedback, sizeof (AlertFeedback));
  ZeroMem ((UINT8 *) CurEye, sizeof (CurEye));
  ZeroMem ((UINT8 *) InvertFlag, sizeof (InvertFlag));
  ZeroMem ((UINT8 *) RankMask, sizeof (RankMask));

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

    if (Channel != ALL_CH) {
      //
      // If not run all channles in parallel, select the target channel.
      //
      if (Ch != Channel) {
        continue;
      }
    }

    RankMask[Ch] = 0;
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    if (Rank >= (*DimmNvList) [Dimm].numRanks) {
      continue;
    }

    if (Signal == NO_SIGNAL) {
      continue;
    }
    // Initialize Ch Rank mask
    RankMask[Ch] = 1;
    for (SubChTemp = 0; SubChTemp < SUB_CH; SubChTemp++) {
      Re[Ch][SubChTemp] = UNMARGINED_CMD_EDGE;
      Le[Ch][SubChTemp] = -UNMARGINED_CMD_EDGE;
      if (((SubChMask) & (1 << SubChTemp)) == 0) {
        continue;
      }
      //
      // Get the original delay
      //
      GetSetSignalDdr5 (Host, Socket, Ch, DdrLevel, Signal + SubChTemp * CaSubChGap, GSM_READ_ONLY, &OrgDelay[Ch][SubChTemp]);
    }

    SetMem ((UINT8 *) ErrorResult[Ch][0], sizeof (ErrorResult[Ch][0]), 1);
    SetMem ((UINT8 *) ErrorResult[Ch][1], sizeof (ErrorResult[Ch][1]), 1);
    //
    // Indicate Rank present and enabled on this Ch.
    //
    OnechBitmask |= 1 << Ch;
    MrcRankList.Dimm = Dimm;
    MrcRankList.Rank = Rank;
    //
    // Setup test
    //
    if (Host->DdrioUltSupport) {
      if (GetCurrentTestType (Socket) == DcaDckDutyCycleTest) {
        CteSetupPattern ("dca_duty_cycle", (Signal >= A0B) ? Signal - A0B : Signal - A0A, OnechBitmask, TWO_SUBCH_MASK, GetPhyRank (Dimm, Rank));
      } else {
        CteSetupPattern ("earlyca_loopback", (Signal >= A0B) ? Signal - A0B : Signal - A0A, OnechBitmask, TWO_SUBCH_MASK, GetPhyRank (Dimm, Rank));
      }
    } else {
      SetupTest (Host, Socket, MemSsType, OnechBitmask, 1, &MrcRankList, DdrLevel, TestType, Signal, 0, Patternloop);
    }

    OnechBitmask = 0;
    ChipGetCmdMgnSwpStruct.eyeSize[Ch] = 0;
    ChipGetCmdMgnSwpStruct.chEyemask |= 1 << Ch;
    ChipGetCmdMgnSwpStruct.chDonemask |= 1 << Ch;
  } // Ch loop

  if (Channel == ALL_CH) {
    ChBitmask = GetChBitmask (Host, Socket, Dimm, Rank);
  } else {
    ChBitmask = 1 << Channel;
  }

  //
  // Delay if this is a CTE build, otherwise do nothing.
  //
  CteDelayDclk (300);
  //
  // Run concurrent tests
  //
  if (Host->DdrioUltSupport) {
    CteStartPattern (ChBitmask, TWO_SUBCH_MASK, GetPhyRank (Dimm, Rank)); //Ch, sc, Rank
  } else {
    for (SubChTemp = 0; SubChTemp < SUB_CH; SubChTemp++) {
      if (((SubChMask) & (1 << SubChTemp)) == 0) {
        continue;
      }
      TestControl (Host, Socket, MemSsType, ChBitmask, SubChTemp, EarlyCaLoopbackTest, 0, START_TEST, NOT_STOP_TEST);
    }
  }
  //
  // Program counting window settings and enable receivers/ODT.
  //
  if (GetCurrentTestType (Socket) == DcaDckDutyCycleTest ||
      (Host->var.mem.socket[Socket].TrainingStatus.Bits.TrainingPatternMode == CA_TRAINING_PATTERN_XTALK && GetCurrentTestType (Socket) == EarlyRcdCaLoopbackTest)) {
    SetCountingWindow (Host, Socket, Dimm, Rank, DCA_DUTY_CYCLE_COUNTING_WINDOW_VALUE);
  } else {
    SetCountingWindow (Host, Socket, Dimm, Rank, CSCA_TF_COUNTING_WINDOW_VALUE);
  }
  //
  // Disable CS assertion trigger.
  //
  EnableCsaTrigger (Host, Socket, Dimm, Rank, CSA_TRIGGER_DISABLE);
  //
  // Delay if this is a CTE build, otherwise do nothing.
  //

  CteDelayDclk (300);

  Offset = CA_CLK_STEP_SIZE;
  //
  //  Sweep Pi delays
  //  The following code assumes CA_CLK_TEST_START is always <= the absolute minimum CMD setting
  //  and CA_CLK_TEST_RANGE + CA_CLK_TEST_START is always < the maximum CMD setting
  //
  for (PiIndex = CA_CLK_TEST_START; PiIndex < (CA_CLK_TEST_RANGE + CA_CLK_TEST_START); PiIndex += Offset) {

    if (ChBitmask == 0) {
      continue;
    }
    //
    // Shift timings on each channel
    //
    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

      if (Channel != ALL_CH) {
        //
        // If not run all channles in parallel, select the target channel.
        //
        if (Ch != Channel) {
          continue;
        }
      }

      if ((RankMask[Ch] == 0)) {
        continue;
      }
      for (SubChTemp = 0; SubChTemp < SUB_CH; SubChTemp++) {
        if (((SubChMask) & (1 << SubChTemp)) == 0) {
          continue;
        }
        GetSetSignalDdr5 (Host, Socket, Ch, DdrLevel, Signal + SubChTemp * CaSubChGap, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &PiIndex);
      }
    } // Ch loop
    //
    // Generate an IO Reset. This is required because the pi delays changed.
    //
    IO_Reset (Host, Socket);
    //
    // Enabling the counter Mode.
    //
    EnableDdrioTrainingCounter (Host, Socket, Dimm, Rank);

    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

      if (Channel != ALL_CH) {
        //
        // If not run all channles in parallel, select the target channel.
        //
        if (Ch != Channel) {
          continue;
        }
      }

      if ((RankMask[Ch] == 0)) {
        continue;
      }
      ChannelMask = 1 << Ch;
      for (SubChTemp = 0; SubChTemp < SUB_CH; SubChTemp++) {
        if (((SubChMask) & (1 << SubChTemp)) == 0) {
          continue;
        }
        SignalLocal = Signal + SubChTemp * CaSubChGap;
        GetTrainResultStatus (Host, Socket, 0, ChannelMask, 1 << SubChTemp, 1, &Err, &ValidMask, 0, Dimm);
        //
        // To read the Train Feedback.
        //
        if (GetCurrentTestType (Socket) == DcaDckDutyCycleTest ||
            (Host->var.mem.socket[Socket].TrainingStatus.Bits.TrainingPatternMode == CA_TRAINING_PATTERN_XTALK && GetCurrentTestType (Socket) == EarlyRcdCaLoopbackTest)) {
          if (Err > 0) {
            AlertFeedback[Ch][SubChTemp][PiIndex] = 1;
          } else {
            AlertFeedback[Ch][SubChTemp][PiIndex] = 0;
          }
        } else {
          if (Err > (CSCA_TF_COUNTING_WINDOW_VALUE / 2)) {
            AlertFeedback[Ch][SubChTemp][PiIndex] = 1;
          } else {
            AlertFeedback[Ch][SubChTemp][PiIndex] = 0;
          }
        }

        if ((PiIndex == CA_CLK_TEST_START) && (AlertFeedback[Ch][SubChTemp][PiIndex] == 0)) {
          InvertFlag[SubChTemp] = 1;
        }
        if (InvertFlag[SubChTemp] == 1) {
          if (AlertFeedback[Ch][SubChTemp][PiIndex] == 1) {
            AlertFeedback[Ch][SubChTemp][PiIndex] = 0;
          } else {
            AlertFeedback[Ch][SubChTemp][PiIndex] = 1;
          }
        }

        //UpdateCmdMarginMasks (Host, Socket, Ch, SubChTemp, PiIndex, Offset, AlertFeedback, &ChipGetCmdMgnSwpStruct);
        //
        // Fill in results for skipped Pi delays
        //
        for (Step = 0; Step < Offset; Step++) {
          if ((PiIndex + Step) >= CA_CLK_TEST_RANGE) {
            continue;
          }

          AlertFeedback[Ch][SubChTemp][PiIndex + Step] = AlertFeedback[Ch][SubChTemp][PiIndex];
        }

      } //subchtemp
    } // Ch loop
  } // PiIndex loop
  //
  // Stop Test
  //
  if (Host->DdrioUltSupport) {
    CteStopPattern ();
  } else {
    for (SubChTemp = 0; SubChTemp < SUB_CH; SubChTemp++) {
      if (((SubChMask) & (1 << SubChTemp)) == 0) {
        continue;
      }
      TestControl (Host, Socket, MemSsType, ChBitmask, SubChTemp, EarlyCaLoopbackTest, 0, NOT_START_TEST, STOP_TEST);
    }
  }

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

    if (Channel != ALL_CH) {
      //
      // If not run all channles in parallel, select the target channel.
      //
      if (Ch != Channel) {
        continue;
      }
    }

    if ((RankMask[Ch] == 0)) {
      continue;
    }
    for (SubChTemp = 0; SubChTemp < SUB_CH; SubChTemp++) {
      if (((SubChMask) & (1 << SubChTemp)) == 0) {
        continue;
      }

      RcDebugPrintWithDevice (
        SDBG_MAX, Socket, Ch, Dimm, SubChTemp, Rank, NO_STROBE, NO_BIT,
        "Sweep results for %5a -> ",
        GetSignalStrDdr5 (Signal + (SubChTemp * CaSubChGap))
        );

      for (PiIndex = 0; PiIndex < CA_CLK_TEST_RANGE; PiIndex++) {

        if (AlertFeedback[Ch][SubChTemp][PiIndex] == 1) {
          RcDebugPrint (SDBG_MAX, "1");
        } else {
          RcDebugPrint (SDBG_MAX, "0");
        }

      } // PiIndex loop

      RcDebugPrint (SDBG_MAX, "\n");

      // Right -> Index End
      // Left -> Index Beginning
      Status = FindErrorGraphMin (Host, Socket, Ch, Dimm, Rank, 0, &AlertFeedback[Ch][SubChTemp][0], CA_CLK_TEST_RANGE,
                 (UINT16 *)&CurEye[Ch][SubChTemp].right, (UINT16 *)&CurEye[Ch][SubChTemp].left, (UINT16 *)&PiPosition[Ch]);

      if ((Status == FAILURE) || (CurEye[Ch][SubChTemp].left == CurEye[Ch][SubChTemp].right)) {
        CurEye[Ch][SubChTemp].left = 0;
        CurEye[Ch][SubChTemp].right = 0;
      }
      //
      // Offset results relative to the starting point of the sweep
      //
      CurEye[Ch][SubChTemp].right = CurEye[Ch][SubChTemp].right - OrgDelay[Ch][SubChTemp];
      CurEye[Ch][SubChTemp].left = CurEye[Ch][SubChTemp].left - OrgDelay[Ch][SubChTemp];
    }//subchtemp
  }//Ch loop

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

    if (Channel != ALL_CH) {
      //
      // If not run all channles in parallel, select the target channel.
      //
      if (Ch != Channel) {
        continue;
      }
    }

    if ((RankMask[Ch] == 0)) {
      continue;
    }
    for (SubChTemp = 0; SubChTemp < SUB_CH; SubChTemp++) {
      if (((SubChMask) & (1 << SubChTemp)) == 0) {
        continue;
      }
      //
      // Update return parameters
      //
      Re[Ch][SubChTemp] = CurEye[Ch][SubChTemp].right;
      Le[Ch][SubChTemp] = CurEye[Ch][SubChTemp].left;
      //
      // apply minimum eye width check here
      //
      EyeWidth = ABS ((Re[Ch][SubChTemp] - Le[Ch][SubChTemp]));
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChTemp, Rank, NO_STROBE, NO_BIT,
        "Left Edge: %d, Right Edge: %d\n", Le[Ch][SubChTemp], Re[Ch][SubChTemp]);

      if (EyeWidth <= MinimumMargin (Host, CA)) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChTemp, Rank, NO_STROBE, NO_BIT,
          "Eye width is too small: %d\n", EyeWidth);
        //
        // As MRC does the VREF sweep, doesn't disable the channel here.
        //
      } else if (Re[Ch][SubChTemp] < Le[Ch][SubChTemp]) {
        // wrap around case
        Re[Ch][SubChTemp] += 256;
      }
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChTemp, Rank, NO_STROBE, NO_BIT,
        "Le = %d - Re = %d\n", Le[Ch][SubChTemp], Re[Ch][SubChTemp]);
      //
      // Restore Signal under test to its initial setting
      //
      GetSetSignalDdr5 (Host, Socket, Ch, DdrLevel, (Signal + SubChTemp * CaSubChGap), GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &OrgDelay[Ch][SubChTemp]);
    }//subchtemp
  } //Ch loop

} // GetCaClkRcdMarginsSweepVref

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
  )
{
  UINT8                        Ch;
  INT16                        PiIndex;
  INT16                        Offset;
  INT16                        OrgDelay[MAX_CH][SUB_CH];
  UINT32                       RankMask[MAX_CH];
  UINT8                        ErrorResult[MAX_CH][CA_CLK_TEST_RANGE];
  UINT32                       ChBitmask = 0;
  UINT32                       BwSerr[MAX_CH][3];
  UINT8                        SubChannel = 0;
  UINT8                        Strobe;
  UINT8                        DWord;
  UINT16                       MinimumLeftEdge = 0;
  INT16                        Tempvar;
  UINT32                       ValidMask[MAX_STROBE_DDR5];
  INT16                        EyeWidthSC[MAX_CH][SUB_CH];
  INT16                        RisingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16                        CenterPoint[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16                        FallingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16                        PulseWidth[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16                        LeftEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16                        RightEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT32                       DqSampleResultStrobeSubCh[SUB_CH][MAX_STROBE_DDR5 / 2];
  struct dimmNvram             (*DimmNvList) [MAX_DIMM];
  UINT8                        Step;
  UINT32                       DqSampleResultStrobe[MAX_STROBE_DDR5 / 2];
  CHIP_GET_CMD_MGN_SWP_STRUCT  ChipGetCmdMgnSwpStruct;
  MRC_MST                      MemSsType;
  UINT8                        SubChannelMask;
  MRC_RT                       MrcRankList;
  UINT32                       OneChBitmask = 0;
  UINT8                        MaxStrobe = 0;
  UINT8                        SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8                        SubChMSVx8 = GetSubChMaxStrobeValid (Host) / 2;
  UINT8                        MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

  ZeroMem ((UINT32 *) DqSampleResultStrobe, sizeof (DqSampleResultStrobe));
  //
  // Init CaLeft,  CaRight, EyeWidthSC
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      CaLeft[Ch][SubChannel] = -UNMARGINED_CMD_EDGE;
      CaRight[Ch][SubChannel] = UNMARGINED_CMD_EDGE;
      EyeWidthSC[Ch][SubChannel] = 0;
    }
  }
  for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
    for (Strobe = 0; Strobe < MAX_STROBE_DDR5 / 2; Strobe++) {
      DqSampleResultStrobeSubCh[SubChannel][Strobe] = 0;
    }
  }
  GetCmdMarginsSweepInit (&ChipGetCmdMgnSwpStruct);

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      RankMask[Ch] = 0;
      OrgDelay[Ch][SubChannel] = 0;
    }

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    if (Rank >= (*DimmNvList) [Dimm].numRanks) {
      continue;
    }

    // Skip CIDS if not NVMDIMM
    /*for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
    if (isCmdCtlTrainedDdr5 (Host, Socket, Ch, Dimm, SignalSubCh[Ch][SubChannel]) != (UINT32) CHECKPOINT_MINOR_EARLY_CA_CLK) {
    continue;
    }
    }*/
    //DDR5_TODO

    // Initialize Ch Rank mask
    RankMask[Ch] = 1;

    //
    // Get the original delay
    //
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      GetSetSignalDdr5 (Host, Socket, Ch, DdrLevel, SignalSubCh[Ch][SubChannel], GSM_READ_ONLY, &OrgDelay[Ch][SubChannel]);
    }

    for (PiIndex = 0; PiIndex < CA_CLK_TEST_RANGE; PiIndex++) {
      ErrorResult[Ch][PiIndex] = 1;
    } //PiIndex loop

    //
    // Setup CADB
    //
    OneChBitmask = 0;
    OneChBitmask |= 1 << Ch;
    MrcRankList.Dimm = Dimm;
    MrcRankList.Rank = Rank;
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      if (SignalSubCh[Ch][SubChannel] == A0A) {
        if (Host->DdrioUltSupport) {
          CteSetupPattern ("earlyca_loopback", (SignalSubCh[Ch][SubChannel] >= A0B) ? SignalSubCh[Ch][SubChannel] - A0B : SignalSubCh[Ch][SubChannel] - A0A, ChBitmask, (SignalSubCh[Ch][SubChannel] >= A0B) ? 0x2 : 0x1, GetPhyRank (Dimm, Rank));
        } else {
          SetupTest (Host, Socket, MemSsType, OneChBitmask, 1, &MrcRankList, DdrLevel, EarlyCaLoopbackTest, SignalSubCh[Ch][SubChannel], 0, 0);
        }
      }
    }
    ChipGetCmdMgnSwpStruct.eyeSize[Ch] = 0;
    ChipGetCmdMgnSwpStruct.chEyemask |= 1 << Ch;
    ChipGetCmdMgnSwpStruct.chDonemask |= 1 << Ch;

    // Indicate Rank present and enabled on this Ch
    ChBitmask |= 1 << Ch;
  } // Ch loop
  //
  // Program counting window settings //DDR5_TODO
  //
  SetDqCountingWindow (Host, Socket, Dimm, Rank);
  //
  // Disable CS Trigger DDR5_TODO.
  //
  //
  // Run concurrent tests
  //
  EarlyCmdSignalTest (Host, Socket, ChBitmask);

  Offset = CA_CLK_STEP_SIZE;

  //
  // Sweep Pi delays
  //
  for (PiIndex = CA_CLK_TEST_START; PiIndex < (CA_CLK_TEST_RANGE + CA_CLK_TEST_START); PiIndex += Offset) {

    if (ChBitmask == 0) {
      continue;
    }
    //
    // Shift timings on each channel
    //
    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if ((RankMask[Ch] == 0)) {
        continue;
      }
      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        GetSetSignalDdr5 (Host, Socket, Ch, DdrLevel, SignalSubCh[Ch][SubChannel], GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &PiIndex);
      }//Subchannel loop
    } // Ch loop

    //
    // Generate an IO Reset. This is required because the pi delays changed.
    //
    IO_Reset (Host, Socket);
    //
    // Enabling the counter Mode. DDR5_TODO.
    //
    EnableDdrioTrainingCounter (Host, Socket, Dimm, Rank);

    ClearBwSerr (BwSerr);

    //
    // Get Test Result
    //
    DWord = (UINT8) ((PiIndex - CS_CLK_PI_START) / 32);
    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if ((RankMask[Ch] == 0)) {
        continue;
      }
      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      //
      // Continue to the next Rank if this one is disabled
      //
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
        continue;
      }

      GetTrainResultStatus (Host, Socket, 0, ChBitmask, NO_SUBCH, sizeof (DqSampleResultStrobeSubCh), (UINT32*) &DqSampleResultStrobeSubCh, ValidMask, 0, Dimm);

      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        for (Strobe = 0; Strobe < MAX_STROBE_DDR5 / 2; Strobe++) {
          if (IsDimmStrobeNotValidDdr5 (&(*DimmNvList) [Dimm], Strobe)) {
            continue;
          }
          if ((DqSampleResultStrobe[Strobe] & 0x1FF) >= CSCA_TF_COUNTING_WINDOW_VALUE / 2) {
            for (Step = 0; Step < CS_CLK_STEP_SIZE; Step++) {
              (*TrainRes) [Ch][SubChannel][Strobe].results[DWord] = (*TrainRes) [Ch][SubChannel][Strobe].results[DWord] | (1 << ((PiIndex + Step) % 32));
            } // Step loop
          } //if
        } //Strobe loop
      }//Subchannel loop

      // Fill unused feedback entries //DDR5_TODO
       /*for (i = 1; i < CS_CLK_STEP_SIZE; i++) {
       DqFeedback[Ch][PiIndex + i] = DqFeedback[Ch][PiIndex];
       }*/
    } //Ch loop
  } // PiIndex loop

  //
  // Exit CA Training Mode.
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    SubChannelMask = 0;
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      //
      // Check Subchannel is enabled or not. DDR5_TODO
      //
      SubChannelMask = SubChannelMask | (1 << SubChannel);
    } //sub channel loop
    //
    // Exit DRAM CA Training Mode.--DIMM sub channel A and B at the same time.
    //
    if (SignalSubCh[Ch][0] == A13A) {
      SendNop (Host, Socket, Ch, SubChannelMask, Dimm, Rank, 0, TRUE, TRUE);
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "Exit CA Training Mode \n");
    }
  }

  //
  // Hanlde the training result.
  //
  for (Ch = 0; Ch < MaxChDdr;Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    if (!(*DimmNvList) [Dimm].x4Present) {
      MaxStrobe = SubChMSVx8;
    } else {
      MaxStrobe = SubChMSVx4;
    }
    //
    // Continue to the next Rank if this one is disabled
    //
    if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) continue;
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      if (GetDebugLevel () & SDBG_MAX) {
        AcquirePrintControl ();
        RcDebugPrint (SDBG_DEFAULT, "\nSummary: Early Ca Clk training Pi \nS%d, Ch%d, DIMM%d, SC%d, Rank%d, %a\n",
          Socket, Ch, Dimm, SubChannel, Rank, GetSignalStrDdr5 (SignalSubCh[Ch][SubChannel]));
        PrintSampleArrayCaClk (Host, Socket, Ch, Dimm, 0, SubChannel, CA_CLK_TEST_RANGE, TrainRes);
        ReleasePrintControl ();
      }

      for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {//DDR5_TODO. For SPR, this function needs to be updated.
          continue;
        }
        EvaluatePiSettingResultsCaClk (Host, Socket, Ch, Dimm, Rank, Strobe, RisingEdge, CenterPoint, FallingEdge, PulseWidth,
          PI_CS_CLK_TYPE, SubChannel, TrainRes);
        //
        // Calculate left edge and right edge
        //
        LeftEdge[Ch][SubChannel][Strobe] = MIN (FallingEdge[Ch][SubChannel][Strobe], RisingEdge[Ch][SubChannel][Strobe]);
        RightEdge[Ch][SubChannel][Strobe] = MAX (FallingEdge[Ch][SubChannel][Strobe], RisingEdge[Ch][SubChannel][Strobe]);
        if (LeftEdge[Ch][SubChannel][Strobe] == RightEdge[Ch][SubChannel][Strobe]) {
          LeftEdge[Ch][SubChannel][Strobe] = 0;
          RightEdge[Ch][SubChannel][Strobe] = 0;
        }
      } // Strobe loop
      //
      // Calculate the composite eye
      //
      //
      // Find the minumum left edge
      //
      MinimumLeftEdge = LeftEdge[Ch][SubChannel][0];
      for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
        if (MinimumLeftEdge > LeftEdge[Ch][SubChannel][Strobe]) {
          MinimumLeftEdge = LeftEdge[Ch][SubChannel][Strobe];
        }
      }
      //
      // Determin the right edge needs to be wrapped or not.
      //
      for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
        if ((LeftEdge[Ch][SubChannel][Strobe] - MinimumLeftEdge) > (MinimumLeftEdge - (RightEdge[Ch][SubChannel][Strobe] - 256))) {
          Tempvar = RightEdge[Ch][SubChannel][Strobe];
          RightEdge[Ch][SubChannel][Strobe] = LeftEdge[Ch][SubChannel][Strobe];
          LeftEdge[Ch][SubChannel][Strobe] = Tempvar - CA_WRAP_AROUND_VALUE;
        }
        PulseWidth[Ch][SubChannel][Strobe] = RightEdge[Ch][SubChannel][Strobe] - LeftEdge[Ch][SubChannel][Strobe];
      }
      //
      // Calculate the composite eye
      //
      for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
        if (LeftEdge[Ch][SubChannel][Strobe] > CaLeft[Ch][SubChannel]) {
          CaLeft[Ch][SubChannel] = LeftEdge[Ch][SubChannel][Strobe];
        }
        if (CaRight[Ch][SubChannel] > RightEdge[Ch][SubChannel][Strobe]) {
          CaRight[Ch][SubChannel] = RightEdge[Ch][SubChannel][Strobe];
        }
      }
      EyeWidthSC[Ch][SubChannel] = ABS (CaRight[Ch][SubChannel] - CaLeft[Ch][SubChannel]);
      //
      // Make sure the eye width is large enough--the check is in the EvaluateCMDMarginsDdr5.
      //
      //
      // Display the left edge, right edge and eye width.
      //
      DisplayEdgesDDR5 (Host, Socket, Ch, PulseWidth[Ch], LeftEdge[Ch], RightEdge[Ch], SubChannel, MaxStrobe);
      DisplayCompositeCaEyeDDR5 (Host, Socket, Ch, CaLeft[Ch][SubChannel], CaRight[Ch][SubChannel], EyeWidthSC[Ch], SubChannel, MaxStrobe);
      //
      // Offset results relative to the starting point of the sweep
      //
      CaLeft[Ch][SubChannel] = CaLeft[Ch][SubChannel] - OrgDelay[Ch][SubChannel];
      CaRight[Ch][SubChannel] = CaRight[Ch][SubChannel] - OrgDelay[Ch][SubChannel];
      //
      // Display the left edge Offset, right edge Offset and eye width.
      //
      RcDebugPrint (SDBG_MAX, "After Offset:\n");
      DisplayCompositeCaEyeDDR5 (Host, Socket, Ch, CaLeft[Ch][SubChannel], CaRight[Ch][SubChannel], EyeWidthSC[Ch], SubChannel, MaxStrobe);

    }//Subchannel loop
  } // Ch loop
  //
  // Restore initial pi delay
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if ((RankMask[Ch] == 0)) {
      continue;
    }
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }
        if ((RightEdge[Ch][SubChannel][Strobe] - LeftEdge[Ch][SubChannel][Strobe]) <= 0) {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
            "ERROR: Signal %a, No eye found.\n", GetSignalStrDdr5 (SignalSubCh[Ch][SubChannel]));
          LeftEdge[Ch][SubChannel][Strobe] = 0;
          RightEdge[Ch][SubChannel][Strobe] = 0;
        }///if
      } //Strobe loop
    } //Subchannel loop
    //
    // Restore Signal under test to its initial setting
    //
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    if (Rank >= (*DimmNvList) [Dimm].numRanks) {
      continue;
    }
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      GetSetSignalDdr5 (Host, Socket, Ch, DdrLevel, SignalSubCh[Ch][SubChannel], GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &OrgDelay[Ch][SubChannel]);
    }
  } // Ch loop
} // GetCaClkMarginsSweepTiming


/**

  Finds command margins for the given command Group

  @param Host        - Pointer to sysHost
  @param Socket        - Socket number

  @retval N/A

**/
VOID
UpdateEdgesDdr5 (
  INT16 Le,
  INT16 Re,
  INT16 *CompLe,
  INT16 *CompRe
  )
{
  if (Le > *CompLe) {
    *CompLe = Le;
  }
  if (Re < *CompRe) {
    *CompRe = Re;
  }
} // UpdateEdges

/**

  Enables normal CMD timing

  @param Host      - Pointer to sysHost
  @param Socket    - Processor Socket

  @retval N/A

**/
VOID
SetNormalCMDTimingDdr5 (
  PSYSHOST  Host,
  UINT8     Socket
  )
{
  UINT8                   Ch;
  struct channelNvram     (*ChannelNvList) [MAX_CH];
  struct dimmNvram        (*DimmNvList) [MAX_DIMM];
  struct RankCh           (*RankPerCh) [MAX_RANK_CH];
  CHIP_SET_NORMAL_CMD_TIMING_STRUCT ChipSetNormalCMDTimingStruct;
  UINT8                   MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    RankPerCh = &(*ChannelNvList) [Ch].rankPerCh;

    //
    // Set normal CMD timing
    //
    if (ShouldChipSetNormalCMDTiming (Host, Socket, Ch, &ChipSetNormalCMDTimingStruct)) {

      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Setting cmd timing to %d\n", (*ChannelNvList) [Ch].timingMode);
    } // if timing needs to be changed
  } // Ch loop
} // SetNormalCMDTimingDdr5

/**
  Update the DCS trained value.
  @param [in]  Host          - Pointer to sysHost
  @param [in]  Socket        - Socket number
  @param [in]  DcsOffset     - DCS offset that needs to be pushed.

  @retval - NA

**/
void
UpdateDcsTrainedValue (
  IN     PSYSHOST           Host,
  IN     UINT8              Socket,
  IN     INT16              DcsOffset,
  IN     BOOLEAN (*ChSubChRankReTrain) [MAX_CH][SUB_CH][MAX_RANK_CH]
  )
{
  UINT8                                 Dimm;
  UINT8                                 Ch;
  UINT8                                 Rank;
  struct dimmNvram                      (*DimmNvList) [MAX_DIMM];
  struct ddrRank (*RankList) [MAX_RANK_DIMM];
  UINT8 SubChannel;

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

      for (Ch = 0; Ch < MAX_CH; Ch++) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }
        DimmNvList = GetDimmNvList (Host, Socket, Ch);

        if ((*DimmNvList) [Dimm].dimmPresent == 0) {
          continue;
        }
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (Rank >= (*DimmNvList) [Dimm].numRanks) {
          continue;
        }
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          if ((*ChSubChRankReTrain) [Ch][SubChannel][ (*RankList) [Rank].rankIndex] == FALSE) {
            continue;
          }

          GetSetSignalDdr5 (Host, Socket, Ch, DdrLevel, CS0_A_N + SubChannel* (CS0_B_N - CS0_A_N) + Dimm * MAX_DIMM, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &DcsOffset);
          GetSetSignalDdr5 (Host, Socket, Ch, DdrLevel, CS1_A_N + SubChannel* (CS1_B_N - CS1_A_N) + Dimm * MAX_DIMM, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &DcsOffset);
        }
      } // Ch
    } // rank
  } //dimm
  return;
}

/**
  Program the RCD train feedback selection for all the ranks in all the dimms.
  @param [in]  Host          - Pointer to sysHost
  @param [in]  Socket        - Socket number

  @retval - NA

**/
VOID
EFIAPI
SetRcdTrainFeedbackSelectionAll (
  IN     PSYSHOST           Host,
  IN     UINT8              Socket
  )
{
  UINT8                                 Dimm;
  UINT8                                 Ch;
  UINT8                                 Rank;
  UINT8                                 RankEnabled;
  struct dimmNvram                      (*DimmNvList) [MAX_DIMM];
  struct channelNvram                   (*ChannelNvList) [MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

      RankEnabled = 0;

      for (Ch = 0; Ch < MAX_CH; Ch++) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if (Rank >= (*DimmNvList) [Dimm].numRanks) {
          continue;
        }
        RankEnabled = 1;
        break;
      } // Ch loop

      //
      // Continue to the next Rank if this one is not populated
      //
      if (RankEnabled == 0) {
        continue;
      }

      //
      // Program the RCD feedback selection.
      //
      SetRcdTrainFeedbackSelection (Host, Socket, Dimm, Rank, FIRST_ITERATION_LOOP);
    } //rank
  }//dimm
  return;
}

/**

Exectues early CA/clock training to get/set the timing margin for RDIMM.

@param Host - Pointer to sysHost

@retval N/A

**/
UINT32
EarlyCaClkRcdTimingMargin (
  IN PSYSHOST  Host,
  IN UINT8     TrainingMode
  )
{

  UINT8               Ch = 0;
  UINT8               RankEnabled;
  UINT8               Socket;
  UINT8               Dimm;
  UINT8               Rank;
  MRC_MST             MemSsType;
  UINT8               Signal;
  UINT8               Index = 0;
  UINT8               SubChannel = 0;
  UINT32              PrevDebugLevel;
  struct dimmNvram (*DimmNvList) [MAX_DIMM];
  GSM_CSN             SignalToSweepCh;
  GSM_CSEDGE_CA      (*SignalEdgeData) [MAX_CH][SUB_CH][MAX_RANK_CH][RCD_CA_SIGNALS_TO_SWEEP];
  BOOLEAN (*ChSubChRankReTrain) [MAX_CH][SUB_CH][MAX_RANK_CH];
  struct ddrRank (*RankList) [MAX_RANK_DIMM];
  INT16               Le[MAX_CH][SUB_CH];
  INT16               Re[MAX_CH][SUB_CH];
  UINT8               SubChannelMask = 0;
  UINT8               SubChTemp = 0;
  UINT8               MaxChDdr;
  UINT8               DcaRetrainLoopCount;
  BOOLEAN             NeedRetrain = FALSE;
  BOOLEAN             SignalRetrain = FALSE;
  INT16               OffsetValue = 0;
  UINT8               RetrainSubChMask = 0;
  UINT8               MaxDcaRetrainLoopCount = 0;

  MaxChDdr = GetMaxChDdr ();
  Socket = Host->var.mem.currentSocket;
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

  SignalEdgeData = RcAllocatePool (sizeof (*SignalEdgeData));
  if (SignalEdgeData == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return FAILURE;
  }
  ZeroMem ((UINT8 *) SignalEdgeData, sizeof (*SignalEdgeData));

  ChSubChRankReTrain = RcAllocatePool (sizeof (*ChSubChRankReTrain));
  if (ChSubChRankReTrain == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return FAILURE;
  }
  ZeroMem ((UINT8 *) ChSubChRankReTrain, sizeof (*ChSubChRankReTrain));

#ifdef DEBUG_CODE_BLOCK
  AcquirePrintControl ();
#endif // DEBUG_CODE_BLOCK
  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "EarlyCaClkRcdTimingMargin Starts with CA_CLK_STEP_SIZE %d\n", CA_CLK_STEP_SIZE);

  SetAepTrainingMode (Host, Socket, CHECKPOINT_MINOR_EARLY_CMD_CLK);
  Index = 0;
  if (TrainingMode == RUN_IN_PARALLEL) {
    SubChannelMask = TWO_SUBCH_MASK;
  }

  if (!UseSmbusMrw (Host, Socket)) {
    //
    // Set the RCD train feedback selection.
    //
    SetRcdTrainFeedbackSelectionAll (Host, Socket);
  }

  MaxDcaRetrainLoopCount = GetMaxDcaRetrainLoopCount ();
  for (DcaRetrainLoopCount = 0; DcaRetrainLoopCount < MaxDcaRetrainLoopCount; DcaRetrainLoopCount++) {
    NeedRetrain = FALSE;
    //
    // Update the DCS trained value.
    //
    if (DcaRetrainLoopCount != 0) {

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        for (Signal = 0; Signal < RCD_CA_SIGNALS_TO_SWEEP; Signal++) {
          for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

            RankEnabled = 0;

            for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
              if (IsChannelEnabled (Socket, Ch) == FALSE) {
                continue;
              }
              if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
                continue;
              }
              DimmNvList = GetDimmNvList (Host, Socket, Ch);
              if (Rank >= (*DimmNvList) [Dimm].numRanks) {
                continue;
              }
              RankEnabled = 1;
              break;
            } // Ch loop
            //
            // Continue to the next Rank if this one is not populated
            //
            if (RankEnabled == 0) {
              continue;
            }

            if (Rank == 0) {
              for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
                if (IsChannelEnabled (Socket, Ch) == FALSE) {
                  continue;
                }
                if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
                  continue;
                }
                RankList = GetRankNvList (Host, Socket, Ch, Dimm);
                DimmNvList = GetDimmNvList (Host, Socket, Ch);
                if (Rank >= (*DimmNvList) [Dimm].numRanks) {
                  continue;
                }
                RetrainSubChMask = 0;
                for (SubChTemp = 0; SubChTemp < SUB_CH; SubChTemp++) {
                  if ((*SignalEdgeData) [Ch][SubChTemp][ (*RankList) [Rank].rankIndex][Signal].Retrain == 1) {
                    (*ChSubChRankReTrain) [Ch][SubChTemp][ (*RankList) [Rank].rankIndex] = TRUE;
                  }
                }
              } // Ch loop
            } //if
          } //rank
        } //signal
      } //dimm

      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "EarlyCaClkRcdTimingMargin Starts Loop %d\n", DcaRetrainLoopCount);
      OffsetValue = 128;
      UpdateDcsTrainedValue (Host, Socket, OffsetValue, ChSubChRankReTrain);
    }

    //
    // Test each Signal on every Rank on every Dimm.
    //
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      if ((TrainingMode == RUN_IN_PARALLEL) && (SubChannel == 1)) {
        continue;
      }
      if (TrainingMode != RUN_IN_PARALLEL) {
        SubChannelMask = 1 << SubChannel;
      }
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (Dimm == 1) {
          for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
            if (IsChannelEnabled (Socket, Ch) == FALSE) {
              continue;
            }
            DimmNvList = GetDimmNvList (Host, Socket, Ch);
            if ((*DimmNvList) [Dimm].dimmPresent == 0) {
              continue;
            }
            SetCaTrainingModeDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, 0, 0, FALSE, Rising_Edge);
          }
        }
        for (Signal = 0; Signal < RCD_CA_SIGNALS_TO_SWEEP; Signal++) {
          SignalRetrain = FALSE;
          for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

            RankEnabled = 0;

            for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
              if (IsChannelEnabled (Socket, Ch) == FALSE) {
                continue;
              }
              if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
                continue;
              }
              DimmNvList = GetDimmNvList (Host, Socket, Ch);
              if (Rank >= (*DimmNvList) [Dimm].numRanks) {
                continue;
              }
              RankEnabled = 1;
              break;
            } // Ch loop
            //
            // Continue to the next Rank if this one is not populated
            //
            if (RankEnabled == 0) {
              continue;
            }

            //
            // Get CA CLK margins for current Rank and Signal
            //
            if (SubChannel == 0) {
              Index = Signal;
            } else {
              Index = Signal + ((RCD_CA_SIGNALS_TO_SWEEP * 2) - 1);
            }
            SignalToSweepCh = SignalsToSweepDDR5[Index];

            if ((DcaRetrainLoopCount != 0) && (Rank == 0)) {
              for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
                if (IsChannelEnabled (Socket, Ch) == FALSE) {
                  continue;
                }
                if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
                  continue;
                }
                RankList = GetRankNvList (Host, Socket, Ch, Dimm);
                if (Rank >= (*DimmNvList) [Dimm].numRanks) {
                  continue;
                }
                RetrainSubChMask = 0;
                for (SubChTemp = 0; SubChTemp < SUB_CH; SubChTemp++) {
                  if ((*ChSubChRankReTrain) [Ch][SubChTemp][ (*RankList) [Rank].rankIndex] == FALSE) {
                    continue;
                  }
                  if ((*SignalEdgeData) [Ch][SubChTemp][ (*RankList) [Rank].rankIndex][Signal].Retrain == 1) {
                    SignalRetrain = TRUE;
                    RetrainSubChMask = RetrainSubChMask | (1<< SubChTemp);
                  } else {
                    //
                    // Apply the offset to the trained pass CA signals.
                    //
                    (*SignalEdgeData) [Ch][SubChTemp][ (*RankList) [Rank].rankIndex][Signal].le += OffsetValue;
                    (*SignalEdgeData) [Ch][SubChTemp][ (*RankList) [Rank].rankIndex][Signal].re += OffsetValue;

                  }
                }
              } // Ch loop
              if (SignalRetrain == FALSE) {
                continue;
              }
            }
            if (Rank > 0) {
              for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
                if (IsChannelEnabled (Socket, Ch) == FALSE) {
                  continue;
                }
                RankList = GetRankNvList (Host, Socket, Ch, Dimm);
                if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
                  continue;
                }
                DimmNvList = GetDimmNvList (Host, Socket, Ch);
                if (Rank >= (*DimmNvList) [Dimm].numRanks) {
                  continue;
                }
                for (SubChTemp = 0; SubChTemp < SUB_CH; SubChTemp++) {
                  if ((SubChannelMask & (1 << SubChTemp)) == 0) {
                    continue;
                  }
                  Le[Ch][SubChTemp] = (*SignalEdgeData) [Ch][SubChTemp][ (*RankList) [0].rankIndex][Signal].le;
                  Re[Ch][SubChTemp] = (*SignalEdgeData) [Ch][SubChTemp][ (*RankList) [0].rankIndex][Signal].re;
                }//SubChTemp
              } //Ch loop
            } else {
              //
              // Get the timing margin for the given Signal.
              //

              for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
                if (IsChannelEnabled (Socket, Ch) == FALSE) {
                  continue;
                }

                DimmNvList = GetDimmNvList (Host, Socket, Ch);
                //
                // Enter the DCA training Mode.
                //
                SetCaTrainingModeDdr5 (Host, Socket, Ch, SubChannelMask, Dimm, Rank, TRUE, Rising_Edge);
              } //Ch

              if (Host->var.mem.socket[Socket].TrainingStatus.Bits.TrainingPatternMode == CA_TRAINING_PATTERN_SIMPLE) {
                RcDebugPrintWithDevice (
                  SDBG_MAX, Socket, NO_CH, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
                  "Calling GetCaClkRcdMarginsSweepVref start \n"
                  );
                GetCaClkRcdMarginsSweepVref (Host, Socket, MemSsType, ALL_CH, Dimm, SubChannel, Rank, SignalToSweepCh, Le, Re, TrainingMode, RetrainSubChMask, DcaRetrainLoopCount, FIRST_ITERATION_LOOP);

              } else {

                GetDcaComplexMargins (
                  Host,
                  Socket,
                  0,
                  SubChannel,
                  Dimm,
                  Rank,
                  DcaDelay,
                  Signal,
                  FALSE,
                  ResultsRelative | SingleBitTrack,
                  Le,
                  Re
                );

              }
            }
            if (DcaRetrainLoopCount != 0) {
              SubChannelMask = RetrainSubChMask;
            }
            if (SignalToSweepCh == NO_SIGNAL) {
              continue;
            }
            for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
              if (IsChannelEnabled (Socket, Ch) == FALSE) {
                continue;
              }
              if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
                continue;
              }
              for (SubChTemp = 0; SubChTemp < SUB_CH; SubChTemp++) {
                if ((SubChannelMask & (1 << SubChTemp)) == 0) {
                  continue;
                }
                RankList = GetRankNvList (Host, Socket, Ch, Dimm);
                //
                // Save the results.
                //

                if (SubChTemp == 0) {
                  Index = Signal;
                } else {
                  Index = Signal + ((RCD_CA_SIGNALS_TO_SWEEP * 2) - 1);
                }
                (*SignalEdgeData) [Ch][SubChTemp][ (*RankList) [Rank].rankIndex][Signal].signal = SignalsToSweepDDR5[Index];

                (*SignalEdgeData) [Ch][SubChTemp][ (*RankList) [Rank].rankIndex][Signal].le = Le[Ch][SubChTemp];
                (*SignalEdgeData) [Ch][SubChTemp][ (*RankList) [Rank].rankIndex][Signal].re = Re[Ch][SubChTemp];
                (*SignalEdgeData) [Ch][SubChTemp][ (*RankList) [Rank].rankIndex][Signal].EyeWidth = ABS (Re[Ch][SubChTemp] - Le[Ch][SubChTemp]);
                (*SignalEdgeData) [Ch][SubChTemp][ (*RankList) [Rank].rankIndex][Signal].toSample = 1;

                RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChTemp, Rank, NO_STROBE, NO_BIT,
                  "Left Edge: %d, Right Edge: %d  signal %5a \n",
                  Le[Ch][SubChTemp],
                  Re[Ch][SubChTemp],
                  GetSignalStrDdr5 ((*SignalEdgeData) [Ch][SubChTemp][ (*RankList) [Rank].rankIndex][Signal].signal));

                if ((Le[Ch][SubChTemp] == 0) || (Re[Ch][SubChTemp] == 0) || (Re[Ch][SubChTemp] == (CA_CLK_TEST_RANGE - 1))) {
                  //
                  // Can't find the edge for the signal and it needs to be retrained with CS shift.
                  //
                  (*SignalEdgeData) [Ch][SubChTemp][ (*RankList) [Rank].rankIndex][Signal].Retrain = 1;
                  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChTemp, Rank, NO_STROBE, NO_BIT,
                    "Can't find the edge for signal %5a with left edge %d and right edge %d and it needs to be retrained\n",
                    GetSignalStrDdr5 ((*SignalEdgeData) [Ch][SubChTemp][ (*RankList) [Rank].rankIndex][Signal].signal),
                    Le[Ch][SubChTemp],
                    Re[Ch][SubChTemp]);
                  NeedRetrain = TRUE;
                }
              } //SubChTemp
              if (FeaturePcdGet (PcdCteBuild) == TRUE) {
                //
                // Exit the CA CLK training Mode.
                //
                if ((Signal == 6) && (Rank == 0)) {
                  SetCaTrainingModeDdr5 (Host, Socket, Ch, SubChannelMask, Dimm, Rank, FALSE, Rising_Edge);
                }
              }
            } // Ch Loop
          } // Rank loop
        } // Signal loop

        RcDebugPrint (SDBG_MAX, "\n");
      } // Dimm loop
    } //Subchannel loop
    if (NeedRetrain == FALSE) {
      break;
    }
  }//DcaRetrainLoopCount
  //
  // clear to insure we take the right path for setting final PI values
  //
  Host->var.mem.earlyCaClkExecuted = 0;
  //
  // Set the timing margin data.
  //
  SetCombinedCaGroupDdr5 (Host, Socket, Ch, SubChannel, DdrLevel, GSM_COMBINE_EDGES, CMDCTL_SIGNAL_DELAY, RCD_CA_SIGNALS_TO_SWEEP, //DDR5_TODO. the Subchannel parameter can be removed.
    SignalEdgeData, sizeof (*SignalEdgeData));
  //
  // Set to indicate early CA CLK training has executed
  //
  Host->var.mem.earlyCaClkExecuted = 1;

  //DDR5 timing Mode defintion is different from DDR4.
  // Execute JEDEC reset/init sequence
  FifoTrainReset (Host, Socket);
  PrevDebugLevel = GetDebugLevel ();
  if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
    SetDebugLevel (SDBG_ERROR);
  }
  IO_Reset (Host, Socket);
  SetDebugLevel (PrevDebugLevel);
  SetAepTrainingMode (Host, Socket, DISABLE_TRAINING_STEP);

#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    DisplayCCCResults (Host, Socket);
  }
  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK

  Host->var.mem.checkMappedOutRanks = 0;
  RcFreePool (SignalEdgeData);
  RcFreePool (ChSubChRankReTrain);
  return SUCCESS;
}//EarlyCaClkRcdTimingMargin

/**

  Executes DCA and DCK Duty cycle training.

  @param[in] Host         - Pointer to sysHost
  @param[in] TrainingMode - Training Mode

  @retval N/A

**/
UINT32
RcdDcaDckDutyCycleTraining (
  IN PSYSHOST  Host,
  IN UINT8     TrainingMode
  )
{
  DCA_SAMPLING_EDGE   DcaSamplingEdge;
  UINT8               SamplingLoop = 0;
  UINT8               Ch = 0;
  UINT8               RankEnabled;
  UINT8               Socket;
  UINT8               Dimm;
  UINT8               Rank;
  MRC_MST             MemSsType;
  UINT8               Signal;
  UINT8               Index = 0;
  UINT8               SubChannel = 0;
  struct dimmNvram    (*DimmNvList) [MAX_DIMM];
  GSM_CSN             SignalToSweepCh;
  GSM_CSEDGE_CA       (*SignalEdgeData) [MAX_CH][SUB_CH][MAX_SAMPLING_LOOP][MAX_RANK_CH][RCD_CA_SIGNALS_TO_SWEEP];
  struct ddrRank      (*RankList) [MAX_RANK_DIMM];
  INT16               Le[MAX_CH][SUB_CH];
  INT16               Re[MAX_CH][SUB_CH];
  UINT8               SubChannelMask = 0;
  UINT8               SubChTemp = 0;
  UINT8               MaxChDdr;
  BOOLEAN             SignalRetrain = FALSE;
  UINT8               RetrainSubChMask = 0;

  MaxChDdr = GetMaxChDdr ();
  Socket = Host->var.mem.currentSocket;
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  SignalEdgeData = RcAllocatePool (sizeof (*SignalEdgeData));
  if (SignalEdgeData == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return FAILURE;
  }
  ZeroMem ((UINT8 *) SignalEdgeData, sizeof (*SignalEdgeData));

#ifdef DEBUG_CODE_BLOCK
  AcquirePrintControl ();
#endif // DEBUG_CODE_BLOCK
  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "RcdDcaDckDutyCycleTraining Starts with CA_CLK_STEP_SIZE %d\n", CA_CLK_STEP_SIZE);

  SetAepTrainingMode (Host, Socket, CHECKPOINT_MINOR_EARLY_CMD_CLK);
  Index = 0;
  if (TrainingMode == RUN_IN_PARALLEL) {
    SubChannelMask = TWO_SUBCH_MASK;
  }

  if (!UseSmbusMrw (Host, Socket)) {
    //
    // Set the RCD train feedback selection.
    //
    SetRcdTrainFeedbackSelectionAll (Host, Socket);
  }

  for (SamplingLoop = 0; SamplingLoop < MAX_SAMPLING_LOOP; SamplingLoop++) {
    if (SamplingLoop == 0) {
      RcDebugPrint (SDBG_MAX, "Rising Edge\n");
      DcaSamplingEdge = Rising_Edge;
    } else {
      RcDebugPrint (SDBG_MAX, "Falling Edge\n");
      DcaSamplingEdge = Falling_Edge;
    }
    //
    // Test each Signal on every Rank on every Dimm.
    //
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      if ((TrainingMode == RUN_IN_PARALLEL) && (SubChannel == 1)) {
        continue;
      }
      if (TrainingMode != RUN_IN_PARALLEL) {
        SubChannelMask = 1 << SubChannel;
      }
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (Dimm == 1) {
          for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
            if (IsChannelEnabled (Socket, Ch) == FALSE) {
              continue;
            }
            DimmNvList = GetDimmNvList (Host, Socket, Ch);
            if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
              continue;
            }
            SetCaTrainingModeDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, 0, 0, FALSE, DcaSamplingEdge);
          }
        }
        for (Signal = 0; Signal < RCD_CA_SIGNALS_TO_SWEEP; Signal++) {
          SignalRetrain = FALSE;
          for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
            RankEnabled = 0;
            for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
              if (IsChannelEnabled (Socket, Ch) == FALSE) {
                continue;
              }
              if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
                continue;
              }
              DimmNvList = GetDimmNvList (Host, Socket, Ch);
              if (Rank >= GetNumberOfRanksOnDimm (Socket, Ch, Dimm)) {
                continue;
              }
              RankEnabled = 1;
              break;
            } // Ch loop
            //
            // Continue to the next Rank if this one is not populated
            //
            if (RankEnabled == 0) {
              continue;
            }
            //
            // Get CA CLK margins for current Rank and Signal
            //
            if (SubChannel == 0) {
              Index = Signal;
            } else {
              Index = Signal + ((RCD_CA_SIGNALS_TO_SWEEP * 2) - 1);
            }
            SignalToSweepCh = SignalsToSweepDDR5[Index];
            if (Rank > 0) {
              //
              // Copy the Rank 0 results to Rank 1.
              //
              for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
                if (IsChannelEnabled (Socket, Ch) == FALSE) {
                  continue;
                }
                RankList = GetRankNvList (Host, Socket, Ch, Dimm);
                if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
                  continue;
                }
                DimmNvList = GetDimmNvList (Host, Socket, Ch);
                if (Rank >= GetNumberOfRanksOnDimm (Socket, Ch, Dimm)) {
                  continue;
                }
                for (SubChTemp = 0; SubChTemp < SUB_CH; SubChTemp++) {
                  if ((SubChannelMask & (1 << SubChTemp)) == 0) {
                    continue;
                  }
                  Le[Ch][SubChTemp] = (*SignalEdgeData) [Ch][SubChTemp][SamplingLoop][ (*RankList) [0].rankIndex][Signal].le;
                  Re[Ch][SubChTemp] = (*SignalEdgeData) [Ch][SubChTemp][SamplingLoop][ (*RankList) [0].rankIndex][Signal].re;
                }//SubChTemp
              } //Ch loop
            } else {
              //
              // Get the timing margin for the given Signal.
              //
              RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
                "Calling GetCaClkRcdMarginsSweepVref start \n");

              for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
                if (IsChannelEnabled (Socket, Ch) == FALSE) {
                  continue;
                }

                DimmNvList = GetDimmNvList (Host, Socket, Ch);
                //
                // Enter the DCA training Mode.
                //
                SetCaTrainingModeDdr5 (Host, Socket, Ch, SubChannelMask, Dimm, Rank, TRUE, DcaSamplingEdge);
              } //Ch

              GetCaClkRcdMarginsSweepVref (Host, Socket, MemSsType, ALL_CH, Dimm, SubChannel, Rank, SignalToSweepCh, Le, Re, TrainingMode, RetrainSubChMask, 0, FIRST_ITERATION_LOOP);
            }
            if (SignalToSweepCh == NO_SIGNAL) {
              continue;
            }
            for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
              if (IsChannelEnabled (Socket, Ch) == FALSE) {
                continue;
              }
              if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
                continue;
              }
              for (SubChTemp = 0; SubChTemp < SUB_CH; SubChTemp++) {
                if ((SubChannelMask & (1 << SubChTemp)) == 0) {
                  continue;
                }
                RankList = GetRankNvList (Host, Socket, Ch, Dimm);
                //
                // Save the results.
                //
                if (SubChannelMask == TWO_SUBCH_MASK) {
                  (*SignalEdgeData) [Ch][SubChTemp][SamplingLoop][ (*RankList) [Rank].rankIndex][Signal].signal = SignalToSweepCh + SubChTemp * ((RCD_CA_SIGNALS_TO_SWEEP*2) - 1);
                } else {
                  (*SignalEdgeData) [Ch][SubChTemp][SamplingLoop][ (*RankList) [Rank].rankIndex][Signal].signal = SignalToSweepCh;
                }
                (*SignalEdgeData) [Ch][SubChTemp][SamplingLoop][ (*RankList) [Rank].rankIndex][Signal].le = Le[Ch][SubChTemp];
                (*SignalEdgeData) [Ch][SubChTemp][SamplingLoop][ (*RankList) [Rank].rankIndex][Signal].re = Re[Ch][SubChTemp];
                (*SignalEdgeData) [Ch][SubChTemp][SamplingLoop][ (*RankList) [Rank].rankIndex][Signal].EyeWidth = ABS (Re[Ch][SubChTemp] - Le[Ch][SubChTemp]);
                (*SignalEdgeData) [Ch][SubChTemp][SamplingLoop][ (*RankList) [Rank].rankIndex][Signal].toSample = 1;

                RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChTemp, Rank, NO_STROBE, NO_BIT,
                  "Left Edge: %d, Right Edge: %d  signal %5a \n",
                  Le[Ch][SubChTemp],
                  Re[Ch][SubChTemp],
                  GetSignalStrDdr5 ((*SignalEdgeData) [Ch][SubChTemp][SamplingLoop][ (*RankList) [Rank].rankIndex][Signal].signal));
              } //SubChTemp
              if (FeaturePcdGet (PcdCteBuild) == TRUE) {
                //
                // Exit the CA CLK training Mode.
                //
                if ((Signal == 6) && (Rank == 0)) {
                  SetCaTrainingModeDdr5 (Host, Socket, Ch, SubChannelMask, Dimm, Rank, FALSE, DcaSamplingEdge);
                }
              }
            } // Ch Loop
          } // Rank loop
        } // Signal loop
        RcDebugPrint (SDBG_MAX, "\n");
      } // Dimm loop
    } //Subchannel loop
  } //SamplingLoop
  //
  // clear to insure we take the right path for setting final PI values
  //
  Host->var.mem.earlyCaClkExecuted = 0;
  //
  // Set the timing margin data.
  //
  SetCombinedCaGroupDdr5 (Host, Socket, Ch, SubChannel, DdrLevel, GSM_COMBINE_EDGES, CMDCTL_SIGNAL_DELAY, RCD_CA_SIGNALS_TO_SWEEP,
    SignalEdgeData, sizeof (*SignalEdgeData));
  //
  // Set to indicate early CA CLK training has executed
  //
  Host->var.mem.earlyCaClkExecuted = 1;
  FifoTrainReset (Host, Socket);
  IO_Reset (Host, Socket);
  //
  // If the CLK is moved, does BIOS need to do anything for DDR5? JEDEC reset/init sequence?
  //
  SetAepTrainingMode (Host, Socket, DISABLE_TRAINING_STEP);
  DeassertDdrtErrAndReq (Host, Socket);

#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    DisplayCCCResults (Host, Socket);
  }
  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK

  Host->var.mem.checkMappedOutRanks = 0;
  RcFreePool (SignalEdgeData);
  return SUCCESS;
}//RcdDcaDckDutyCycleTraining

/**
  Display Slew Rates.

  @param  Host          Pointer to sysHost.
  @param  SlewRate      Slew rate array to be displayed.

  @retval SUCESS        Succeed to display slew rates.
**/
UINT32
DisplaySlewRates (
 IN PSYSHOST  Host,
 IN INT16     (*SlewRate) [MAX_CH][SUB_CH][MAX_RANK_CH][RCD_CA_SIGNALS_TO_SWEEP]
  )
{
  UINT8                           Ch;
  UINT8                           SubChannel;
  UINT8                           Socket;
  UINT8                           Dimm;
  UINT8                           Rank;
  UINT8                           Signal;
  UINT8                           Index;
  UINT8                           MaxChDdr;
  GSM_CSN                         SignalToSweepCh;
  struct dimmNvram                (*DimmNvList) [MAX_DIMM];
  struct ddrRank                  (*RankList) [MAX_RANK_DIMM];

  MaxChDdr = GetMaxChDdr ();
  Socket = GetCurrentSocketId ();

  RcDebugPrint (SDBG_MAX,"             ");
  for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
    for (Signal = 0; Signal < RCD_CA_SIGNALS_TO_SWEEP; Signal++) {
       Index = Signal + SubChannel * ((RCD_CA_SIGNALS_TO_SWEEP * 2) - 1);
       SignalToSweepCh = SignalsToSweepDDR5[Index];
       RcDebugPrint (SDBG_MAX,"%6a", GetSignalStrDdr5 (SignalToSweepCh));
    }
  }
  RcDebugPrint (SDBG_MAX,"      \n");

  //
  // Display Slew Rate Delta of each signal.
  //
  for (Ch = 0; Ch < MaxChDdr;Ch += GetChInc (Host)) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
        continue;
      }

      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
        if (IsRankValid (Socket, Ch, Dimm, Rank, TRUE) == FALSE) {
          continue;
        }
        if (Rank > 0) {
          //
          // Skip if Rank > 0. All ranks on the same DIMM share the same RCD DCA.
          //
          break;
        }

        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT," ");
        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          for (Signal = 0; Signal < RCD_CA_SIGNALS_TO_SWEEP; Signal++) {
            RcDebugPrint (SDBG_MAX, "  %3d ", (*SlewRate) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Signal]);
          } // Signal
        }
        RcDebugPrint (SDBG_MAX,"      \n");
      }
    }
  }

  return SUCCESS;
}

/**
  Get RCD DCA Vref margin. This function returns nominal Vref, 70% of Max Vref and 70% of Min Vref of each RCD DCA signal.

  @param  Host          Pointer to sysHost.
  @param  TrainingMode  Paralell or Serial training mode of sub-channels.
  @param  VrefNominal   Nominal Vref of each RCD DCA.
  @param  VrefMax0p7    70% of Max Vref.
  @param  VrefMin0p7    70% of Min Vref.

  @retval FAILURE       Fails to get RDC DCA Vref margin.
  @retval SUCESS        Succeed to get RCD DCA Vref margin.
**/
UINT32
GetDcaVrefMargin (
  IN      PSYSHOST  Host,
  IN      UINT8     TrainingMode,
  IN OUT  UINT8      (*VrefNominal)[MAX_CH][SUB_CH][MAX_RANK_CH][RCD_CA_SIGNALS_TO_SWEEP],
  IN OUT  UINT8     (*VrefMax0p7)[MAX_CH][SUB_CH][MAX_RANK_CH][RCD_CA_SIGNALS_TO_SWEEP],
  IN OUT  UINT8     (*VrefMin0p7)[MAX_CH][SUB_CH][MAX_RANK_CH][RCD_CA_SIGNALS_TO_SWEEP]
  )
{
  UINT8         Socket;
  UINT8         Ch;
  UINT8         SubChannel;
  UINT8         Dimm;
  UINT8         Rank;
  UINT8         RankIndex;
  UINT8         MaxChDdr;
  UINT8         Signal;
  INT16         Value16;
  UINT32        BackupMessage;
  GSM_CSN       RcdSignal;
  INT16         Le[MAX_CH][SUB_CH];
  INT16         Re[MAX_CH][SUB_CH];
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];

  BackupMessage = GetDebugLevel ();

  Socket = Host->var.mem.currentSocket;
  MaxChDdr = GetMaxChDdr ();

  for (Signal = 0; Signal < RCD_CA_SIGNALS_TO_SWEEP; Signal++) {


    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

        if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
          continue;
        }

        DimmNvList = GetDimmNvList (Host, Socket, Ch);

        for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

          if (IsRankValid (Socket, Ch, Dimm, Rank, TRUE) == FALSE) {
            continue;
          }

          RankIndex = GetRankIndex (Socket, Ch, Dimm, Rank);

          RcdSignal = SignalsToSweepDDR5[Signal];

          for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

            GetDcaComplexMargins (
              Host,
              Socket,
              0,
              SubChannel,
              Dimm,
              Rank,
              RcdDcaVref,
              Signal,
              FALSE,
              ResultsRelative | SingleBitTrack,
              Le,
              Re
              );

            GetSetRcdDcaVref(Host, Socket, Ch, SubChannel, Dimm, RcdSignal, GSM_READ_ONLY, &Value16);
            (*VrefNominal)[Ch][SubChannel][RankIndex][Signal]  = (UINT8)(Value16);
            (*VrefMin0p7)[Ch][SubChannel][RankIndex][Signal]   = (UINT8)(Value16 + (Le[Ch][SubChannel] * 70) / 100);
            (*VrefMax0p7)[Ch][SubChannel][RankIndex][Signal]   = (UINT8)(Value16 + (Re[Ch][SubChannel] * 70) / 100);
            RcDebugPrintWithDevice (
              SDBG_MAX,
              Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
              " Signal %6a, VrefNominal = %3d, VrefMin0p7 = %3d VrefMax0p7 = %3d\n",
              GetSignalStrDdr5 (GetRcdCaSignal (SubChannel, RcdSignal)),
              (*VrefNominal)[Ch][SubChannel][RankIndex][Signal],
              (*VrefMin0p7)[Ch][SubChannel][RankIndex][Signal],
              (*VrefMax0p7)[Ch][SubChannel][RankIndex][Signal]
              );
          }
        } // Rank loop ...
      } // Dimm loop ...
    } // Ch loop ...
  } // Signal loop ...

  return SUCCESS;
}

/**

  Calculates the rising edge based on VrefMax0.7, VrefMin0.7 and T5,T4,T2,T6

  @param[in]      Host              Pointer to syshost structure
  @param[in]      Socket            Current socket under test (0-based)
  @param[in]      Ch                Current channel under test (0-based)
  @param[in]      SubChannel        Current subchannel under test (0-based)
  @param[in]      Dimm              Current dimm under test (0-based)
  @param[in]      RankIndex         Current rank under test (0-based within the socket)
  @param[in]      Signal            Current signal under test (valid from 0 to RCD_CA_SIGNALS_TO_SWEEP)
  @param[in]      VrefMax0p7Delta   Represents the 70% of the max margin
  @param[in]      VrefMin0p7Delta   Represents the 70% of the min margin
  @param[in]      T5                Right margin at nominal
  @param[in]      T4                Right margin at vmin
  @param[in]      T2                Left margin at vmax
  @param[in]      T6                Left margin at nominal
  @param[out]     SlewRateRisingEdge  Pointer to return the current slew rate
  @param[out]     RisingEdgeSlewRate  Pointer that stores the current slew rate for all channels/subchannels/ranks/signals

**/
VOID
CalculateRisingSlewRate (
  IN        PSYSHOST        Host,
  IN        UINT8           Socket,
  IN        UINT8           Ch,
  IN        UINT8           SubChannel,
  IN        UINT8           Dimm,
  IN        UINT8           RankIndex,
  IN        UINT8           Signal,
  IN        UINT16          VrefMax0p7Delta,
  IN        UINT16          VrefMin0p7Delta,
  IN  OUT   INT16           T5[SUB_CH],
  IN  OUT   INT16           T4[SUB_CH],
  IN  OUT   INT16           T2[SUB_CH],
  IN  OUT   INT16           T6[SUB_CH],
      OUT   INT16           *SlewRateRisingEdge,
      OUT   INT16           RisingEdgeSlewRate[MAX_CH][SUB_CH][MAX_RANK_CH][RCD_CA_SIGNALS_TO_SWEEP]
  )
{
  //
  // iv.  Rising slew rate = (VrefNom - VrefMin0p7)/(t5-t4) + (VrefMax0p7 - VrefNom)/(t2-t6)
  // If there is two timing margins with the same value (which would create division by zero)
  // then choose the lower voltage and pull in the margin by 1 tick to give a non vertical slope.
  //
  RcDebugPrint (
    SDBG_MAX,
    "Rising slew rate = VrefMin0p7Delta / (t5-t4) + VrefMax0p7Delta / (t2-t6)\n"
    );
  if (T2[SubChannel] == T6[SubChannel]) {
    T6[SubChannel]--;
    RcDebugPrintWithDevice (
      SDBG_MAX, Socket, Ch, Dimm, SubChannel, NO_RANK, NO_STROBE, NO_BIT,
      " (T6)-- %3d\n",
      T6[SubChannel]
     );
  }
  if (T5[SubChannel] == T4[SubChannel]) {
    T4[SubChannel]--;
    RcDebugPrintWithDevice (
      SDBG_MAX, Socket, Ch, Dimm, SubChannel, NO_RANK, NO_STROBE, NO_BIT,
      " (T4)-- %3d\n",
      T4[SubChannel]
     );
  }
  *SlewRateRisingEdge = VrefMin0p7Delta / (T5[SubChannel] - T4[SubChannel]) + VrefMax0p7Delta / (T2[SubChannel] - T6[SubChannel]);
  RisingEdgeSlewRate[Ch][SubChannel][RankIndex][Signal] = *SlewRateRisingEdge;

  if (*SlewRateRisingEdge < 0) {
    RcDebugPrint (
      SDBG_MAX,
      "SlewRateRisingEdge < 0\n"
      );
  }

}

/**

  Calculates the falling edge based on VrefMax0.7, VrefMin0.7 and T6,T3,T1,T5

  @param[in]      Host              Pointer to syshost structure
  @param[in]      Socket            Current socket under test (0-based)
  @param[in]      Ch                Current channel under test (0-based)
  @param[in]      SubChannel        Current subchannel under test (0-based)
  @param[in]      Dimm              Current dimm under test (0-based)
  @param[in]      RankIndex         Current rank under test (0-based within the socket)
  @param[in]      Signal            Current signal under test (valid from 0 to RCD_CA_SIGNALS_TO_SWEEP)
  @param[in]      VrefMax0p7Delta   Represents the 70% of the max margin
  @param[in]      VrefMin0p7Delta   Represents the 70% of the min margin
  @param[in]      T6                Left margin at nominal
  @param[in]      T3                Left margin at vmin
  @param[in]      T1                Right margin at vmax
  @param[in]      T5                Right margin at nominal
  @param[out]     SlewRateFallingEdge  Pointer to return the current slew rate
  @param[out]     SlewRateFallingEdge  Pointer that stores the current slew rate for all channels/subchannels/ranks/signals

**/
VOID
CalculateFallingSlewRate (
  IN        PSYSHOST        Host,
  IN        UINT8           Socket,
  IN        UINT8           Ch,
  IN        UINT8           SubChannel,
  IN        UINT8           Dimm,
  IN        UINT8           RankIndex,
  IN        UINT8           Signal,
  IN        UINT16          VrefMax0p7Delta,
  IN        UINT16          VrefMin0p7Delta,
  IN  OUT   INT16           T6[SUB_CH],
  IN  OUT   INT16           T3[SUB_CH],
  IN  OUT   INT16           T1[SUB_CH],
  IN  OUT   INT16           T5[SUB_CH],
      OUT   INT16           *SlewRateFallingEdge,
      OUT   INT16           FallingEdgeSlewRate[MAX_CH][SUB_CH][MAX_RANK_CH][RCD_CA_SIGNALS_TO_SWEEP]
  )
{

  //
  // v. Falling slew rate = (VrefNom - VrefMin0p7)/(t6-t3) + (VrefMax0p7 - VrefNom)/(t1-t5)
  // If there is two timing margins with the same value (which would create division by zero)
  // then choose the lower voltage and push in the margin by 1 tick to give a non vertical slope.
  //
  RcDebugPrint (
    SDBG_MAX,
    "Falling slew rate = VrefMin0p7Delta / (t6-t3) + VrefMax0p7Delta / (t1-t5)\n"
    );
  if (T6[SubChannel] == T3[SubChannel]) {
    T3[SubChannel]++;
    RcDebugPrintWithDevice (
      SDBG_MAX, Socket, Ch, Dimm, SubChannel, NO_RANK, NO_STROBE, NO_BIT,
      " (T3)++ %3d\n",
      T3[SubChannel]
      );
  }
  if (T1[SubChannel] == T5[SubChannel]) {
    T5[SubChannel]++;
    RcDebugPrintWithDevice (
      SDBG_MAX, Socket, Ch, Dimm, SubChannel, NO_RANK, NO_STROBE, NO_BIT,
      " (T5)++ %3d\n",
      T5[SubChannel]
     );
  }

  *SlewRateFallingEdge = VrefMin0p7Delta / (T6[SubChannel] - T3[SubChannel]) + VrefMax0p7Delta / (T1[SubChannel] - T5[SubChannel]);
  FallingEdgeSlewRate[Ch][SubChannel][RankIndex][Signal] = *SlewRateFallingEdge;

  if (*SlewRateFallingEdge > 0) {
    RcDebugPrint (
      SDBG_MAX,
      "SlewRateFallingEdge > 0\n"
      );
  }

}

/**

  Calculates the DCA margins at a given DCA VREF value

  @param[in]      Host              Pointer to syshost structure
  @param[in]      Socket            Current socket under test (0-based)
  @param[in]      Ch                Current channel under test (0-based)
  @param[in]      SubChannel        Current subchannel under test (0-based)
  @param[in]      Dimm              Current dimm under test (0-based)
  @param[in]      Rank              Current rank under test (0-based within the dimm)
  @param[in]      Signal            Current signal under test (valid from 0 to RCD_CA_SIGNALS_TO_SWEEP)
  @param[in]      CaVrefValue       DCA VREF to test
  @param[in]      CaVrefLevel       TAG for DCA VREF (CaVrefAtNominal, CaVrefAtMin, or CaVrefAtMax)
  @param[out]     Right             Pointer to return the right edge for each subchannel
  @param[out]     Left              Pointer to return the left edge for each subchannel

**/
VOID
GetMarginsAtCaVref (
  IN        PSYSHOST        Host,
  IN        UINT8           Socket,
  IN        UINT8           Ch,
  IN        UINT8           SubChannel,
  IN        UINT8           Dimm,
  IN        UINT8           Rank,
  IN        UINT8           Signal,
  IN        INT8            CaVrefValue,
  IN        CA_VREF_LEVEL   CaVrefLevel,
      OUT   INT16           Right[SUB_CH],
      OUT   INT16           Left[SUB_CH]
  )
{
  INT16  Le[MAX_CH][SUB_CH];
  INT16  Re[MAX_CH][SUB_CH];
  CHAR8  *LeftStr[CaVrefLevelMax + 1]  = {"T2", "T3", "T6", "UNK"};
  CHAR8  *RightStr[CaVrefLevelMax + 1] = {"T1", "T4", "T5", "UNK"};

  if (CaVrefLevel >= CaVrefLevelMax) {
    CaVrefLevel = CaVrefLevelMax;
  }

  ProgramSignalCaVref (Host, Socket, Ch, Dimm, SubChannel, SignalsToSweepDDR5[Signal], CaVrefValue);

  GetDcaComplexMargins (
    Host, Socket,
    (UINT32)~(1 << Ch),
    SubChannel,
    Dimm,
    Rank,
    DcaDelay,
    Signal,
    FALSE,
    SingleBitTrack,
    Le,
    Re
    );

  Right[SubChannel] = Re[Ch][SubChannel];
  Left[SubChannel]  = Le[Ch][SubChannel];

  RcDebugPrintWithDevice (
    SDBG_MAX,
    Socket, Ch, Dimm, SubChannel, NO_RANK, NO_STROBE, NO_BIT,
    "Signal %6a, Vref %3d, Eye width %3d, left (%a) %3d, right (%a) %3d \n",
    GetSignalStrDdr5 (GetRcdCaSignal (SubChannel, SignalsToSweepDDR5[Signal])),
    CaVrefValue,
    (Right[SubChannel] - Left[SubChannel]),
    LeftStr[CaVrefLevel],
    Left[SubChannel],
    RightStr[CaVrefLevel],
    Right[SubChannel]
    );

}


/**
  Executes DCA Slew Rate Training.

  @param[in]  Host          Pointer to sysHost.
  @param[in]  TrainingMode  Parallel or Serial training mode of sub-channels.

  @retval FAILURE       Fails to train DCA slew rate.
  @retval SUCESS        Succeed to train DCA slwe rate.
**/
UINT32
DcaSlewRateTraining (
  IN PSYSHOST  Host,
  IN UINT8     TrainingMode
  )
{
  UINT8                           Ch = 0;
  UINT8                           Socket;
  UINT8                           Dimm;
  UINT8                           Rank;
  UINT8                           RankIndex;
  MRC_MST                         MemSsType;
  UINT8                           Signal;
  UINT8                           Index = 0;
  UINT8                           SubChannel = 0;
  GSM_CSN                         SignalToSweepCh;
  struct dimmNvram                (*DimmNvList)[MAX_DIMM];
  UINT8                           MaxChDdr;
  INT16                           T1[SUB_CH];
  INT16                           T2[SUB_CH];
  INT16                           T3[SUB_CH];
  INT16                           T4[SUB_CH];
  INT16                           T5[SUB_CH];
  INT16                           T6[SUB_CH];
  INT8                            (*VrefNominal)[MAX_CH][SUB_CH][MAX_RANK_CH][RCD_CA_SIGNALS_TO_SWEEP];
  UINT8                           (*VrefMax0p7)[MAX_CH][SUB_CH][MAX_RANK_CH][RCD_CA_SIGNALS_TO_SWEEP];
  UINT8                           (*VrefMin0p7)[MAX_CH][SUB_CH][MAX_RANK_CH][RCD_CA_SIGNALS_TO_SWEEP];
  INT16                           SlewRateRisingEdge;
  INT16                           SlewRateFallingEdge;
  UINT16                          VrefMax0p7Delta; // 70% Vref Max
  UINT16                          VrefMin0p7Delta; // 70% Vref Min
  INT16                           RisingEdgeSlewRate[MAX_CH][SUB_CH][MAX_RANK_CH][RCD_CA_SIGNALS_TO_SWEEP];
  INT16                           FallingEdgeSlewRate[MAX_CH][SUB_CH][MAX_RANK_CH][RCD_CA_SIGNALS_TO_SWEEP];
  INT16                           SlewRateDelta[MAX_CH][SUB_CH][MAX_RANK_CH][RCD_CA_SIGNALS_TO_SWEEP]; // (Rising Edge - Falling Edge)
  INT16                           MaxSlewRateDeltaPerSubCh[MAX_CH][SUB_CH];
  INT16                           PreviousMaxSlewRateDeltaPerSubCh[MAX_CH][SUB_CH];
  BOOLEAN                         AllChannelDone;
  UINT8                           ScompDownAdj[MAX_SLEW_RATE_SAMPLES] = {8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0};
  UINT8                           ScompUpAdj[MAX_SLEW_RATE_SAMPLES]   = {0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7};
  UINT8                           LoopCount;
  BOOLEAN                         SlewRateTrainingDone[MAX_CH][SUB_CH];
  UINT8                           SubChannelMask;

  //
  // Local parameters initialization.
  //

  MaxChDdr = GetMaxChDdr ();
  Socket = GetCurrentSocketId ();
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

  VrefNominal = RcAllocatePool (sizeof (*VrefNominal));
  if (VrefNominal == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return FAILURE;
  }

  VrefMax0p7 = RcAllocatePool (sizeof (*VrefMax0p7));
  if (VrefNominal == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return FAILURE;
  }

  VrefMin0p7 = RcAllocatePool (sizeof (*VrefMin0p7));
  if (VrefNominal == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return FAILURE;
  }

  AllChannelDone = FALSE;
  LoopCount = 0;

  ZeroMem ((UINT8 *) SlewRateTrainingDone, sizeof (SlewRateTrainingDone));
  ZeroMem ((UINT8 *) PreviousMaxSlewRateDeltaPerSubCh, sizeof(PreviousMaxSlewRateDeltaPerSubCh));

  //
  // Entering Early Command Clock Training Mode for DCPMM.
  // This will prevent the valley from decoding any commands. While training the DDRT2 CA bus
  //
  SetAepTrainingMode (Host, Socket, CHECKPOINT_MINOR_EARLY_CMD_CLK);

  //
  // Initialize chip specific Scomp.
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      InitializeCcScompChip (Host, Socket, Ch, SubChannel);
    }
  }

  //
  // Get Nominal Vref, 70% Vref Max and 70% Vref Min of each DCA signal.
  // 1. Get CA Vref margins (Vmax, Vmin)
  //
  GetDcaVrefMargin (Host, TrainingMode, VrefNominal, VrefMax0p7, VrefMin0p7);

  //
  // Per sub-channel DCA slew rate traning loop.
  //
  while (LoopCount < MAX_SLEW_RATE_SAMPLES) {

    RcDebugPrint (SDBG_MEM_TRAIN, "Ca Slew Rate Training Loop %d \n", LoopCount);
    ZeroMem ((UINT8 *) RisingEdgeSlewRate, sizeof (RisingEdgeSlewRate));
    ZeroMem ((UINT8 *) FallingEdgeSlewRate, sizeof (FallingEdgeSlewRate));
    ZeroMem ((UINT8 *) SlewRateDelta, sizeof (SlewRateDelta));
    ZeroMem ((UINT8 *) MaxSlewRateDeltaPerSubCh, sizeof (MaxSlewRateDeltaPerSubCh));

    for (Ch = 0; Ch < MaxChDdr;Ch += GetChInc (Host)) {

      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }

      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

        GetSetCcScompUpDownAdj (Host, Socket, Ch, SubChannel, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, FALSE, &ScompDownAdj[LoopCount]);
        GetSetCcScompUpDownAdj (Host, Socket, Ch, SubChannel, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, TRUE,  &ScompUpAdj[LoopCount]);

        RcDebugPrintWithDevice (
          SDBG_MAX,
          Socket, Ch, NO_DIMM, SubChannel, NO_RANK, NO_STROBE, NO_BIT,
          "ScompDownAdj = %3d ScompUpAdj = %3d\n",
          ScompDownAdj[LoopCount],
          ScompUpAdj[LoopCount]
          );
      } // SubChannel loop
    } // Ch loop

    //
    // Step 1: Calculate max slew rate of each sub channel.
    //
    RcDebugPrint (SDBG_MAX, "Calculate max slew rate of each sub channel.\n\n");

    for (Ch = 0; Ch < MaxChDdr;Ch += GetChInc (Host)) {

      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }

      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

        //
        // Skip sub-channel that has been trained.
        //
        if (SlewRateTrainingDone[Ch][SubChannel] == TRUE) {
          continue;
        }

        for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

          if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
            continue;
          }

          DimmNvList = GetDimmNvList (Host, Socket, Ch);

          for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {

            if (IsRankValid (Socket, Ch, Dimm, Rank, TRUE) == FALSE) {
              continue;
            }

            if (Rank > 0) {
              //
              // Skip if Rank > 0. All ranks on the same DIMM share the same RCD DCA.
              //
              break;
            }

            RankIndex = GetRankIndex (Socket, Ch, Dimm, Rank);

            //
            // Enter the DCA training Mode.
            //
            SubChannelMask = 1 << SubChannel;
            if (TrainingMode == RUN_IN_PARALLEL) {
              SubChannelMask = TWO_SUBCH_MASK;
            }
            SetCaTrainingModeDdr5 (Host, Socket, Ch, SubChannelMask, Dimm, Rank, TRUE, Rising_Edge);

            for (Signal = 0; Signal < RCD_CA_SIGNALS_TO_SWEEP; Signal++) {

              Index = Signal + SubChannel * ((RCD_CA_SIGNALS_TO_SWEEP * 2) - 1);
              SignalToSweepCh = SignalsToSweepDDR5[Index];

              //
              // Get new timing margin under Vref Max 70%.
              // i. 0.7*Vmax: (t1, t2)
              //
              GetMarginsAtCaVref (
                Host,
                Socket,
                Ch,
                SubChannel,
                Dimm,
                Rank,
                Signal,
                (*VrefMax0p7)[Ch][SubChannel][RankIndex][Signal],
                CaVrefAtMax,
                T1,
                T2
                );
              //
              // Get new timing margin under Vref Min 70%.
              // ii.  0.7*Vmin: (t4, t3)
              //
              GetMarginsAtCaVref (
                Host,
                Socket,
                Ch,
                SubChannel,
                Dimm,
                Rank,
                Signal,
                (*VrefMin0p7)[Ch][SubChannel][RankIndex][Signal],
                CaVrefAtMin,
                T4,
                T3
                );
              //
              // Get new timing margin under Vref Nominal.
              // iii. Nominal: (t5, t6)
              //
              GetMarginsAtCaVref (
                Host,
                Socket,
                Ch,
                SubChannel,
                Dimm,
                Rank,
                Signal,
                (*VrefNominal)[Ch][SubChannel][RankIndex][Signal],
                CaVrefAtNom,
                T5,
                T6
                );

              VrefMax0p7Delta = ((*VrefMax0p7)[Ch][SubChannel][RankIndex][Signal] - (*VrefNominal)[Ch][SubChannel][RankIndex][Signal]) * FIXED_POINT_MULTIPLIER;
              VrefMin0p7Delta = ((*VrefNominal)[Ch][SubChannel][RankIndex][Signal] - (*VrefMin0p7)[Ch][SubChannel][RankIndex][Signal]) * FIXED_POINT_MULTIPLIER;

              RcDebugPrintWithDevice (
                SDBG_MAX, Socket, Ch, Dimm, SubChannel, NO_RANK, NO_STROBE, NO_BIT,
                "Signal %6a,  VrefMax0p7Delta = (VrefMax0p7 - VrefNom) * FIXED_POINT_MULTIPLIER = %3d, "\
                "VrefMin0p7Delta = (VrefNom - VrefMin0p7) * FIXED_POINT_MULTIPLIER = %03d\n",
                GetSignalStrDdr5 (GetRcdCaSignal (SubChannel, SignalsToSweepDDR5[Signal])),
                VrefMax0p7Delta,
                VrefMin0p7Delta
                );

              //
              // Calculate rising edge slew rate
              //
              CalculateRisingSlewRate (
                Host,
                Socket,
                Ch,
                SubChannel,
                Dimm,
                RankIndex,
                Signal,
                VrefMax0p7Delta,
                VrefMin0p7Delta,
                T5,
                T4,
                T2,
                T6,
                &SlewRateRisingEdge,
                RisingEdgeSlewRate
                );
              //
              // Calculate falling edge slew rate
              //
              CalculateFallingSlewRate (
                Host,
                Socket,
                Ch,
                SubChannel,
                Dimm,
                RankIndex,
                Signal,
                VrefMax0p7Delta,
                VrefMin0p7Delta,
                T6,
                T3,
                T1,
                T5,
                &SlewRateFallingEdge,
                FallingEdgeSlewRate
                );
              //
              // c. Find the delta for each signal between the slew rates (rising – falling)[signal]
              // Changed to +, after post-silicon
              //
              SlewRateDelta[Ch][SubChannel][RankIndex][Signal] = SlewRateRisingEdge + SlewRateFallingEdge;

              RcDebugPrintWithDevice (
                SDBG_MAX, Socket, Ch, Dimm, SubChannel, NO_RANK, NO_STROBE, NO_BIT,
                "Signal %6a, SlewRateRisingEdge %3d SlewRateFallingEdge %3d SlewRateDelta %3d\n",
                GetSignalStrDdr5 (SignalToSweepCh),
                SlewRateRisingEdge,
                SlewRateFallingEdge,
                SlewRateDelta[Ch][SubChannel][RankIndex][Signal]
               );
              //
              // Record the max slew rate delta that has been found for current sub-channel.
              //
              if (ABS(MaxSlewRateDeltaPerSubCh[Ch][SubChannel]) < ABS (SlewRateDelta[Ch][SubChannel][RankIndex][Signal])) {
                MaxSlewRateDeltaPerSubCh[Ch][SubChannel] = SlewRateDelta[Ch][SubChannel][RankIndex][Signal];
              }

              //
              // Re-center the RCD Vref to Vref Nominal.
              //
              ProgramSignalCaVref (Host, Socket, Ch, Dimm, SubChannel, SignalsToSweepDDR5[Signal], (*VrefNominal)[Ch][SubChannel][RankIndex][Signal]);

              RcDebugPrint (SDBG_MAX, "\n");

            } // Signal
          } // Rank
        } // Dimm
      } //SubChannel
    } // Ch loop

    RcDebugPrint (SDBG_MAX, "\nSummary: Rising Edge Slew Rate");
    RcDebugPrintLine (SDBG_MEM_TRAIN, 118, SINGLE_LINE, CRLF_FLAG, CRLF_FLAG);
    DisplaySlewRates (Host, &RisingEdgeSlewRate);

    RcDebugPrint (SDBG_MAX, "\nSummary: Falling Edge Slew Rate");
    RcDebugPrintLine (SDBG_MEM_TRAIN, 118, SINGLE_LINE, CRLF_FLAG, CRLF_FLAG);
    DisplaySlewRates (Host, &FallingEdgeSlewRate);

    RcDebugPrint (SDBG_MAX, "\nSummary: Slew Rate Delat (Rising Edge - Falling Edge)");
    RcDebugPrintLine (SDBG_MEM_TRAIN, 118, SINGLE_LINE, CRLF_FLAG, CRLF_FLAG);
    DisplaySlewRates (Host, &SlewRateDelta);

    //
    // Step 2: Evaluate max slew rate delta per sub-channel. Adjust slew rate if necessary.
    //

    RcDebugPrint (SDBG_MAX, "\nSummary: DCA Max Slew Rate Delta Per Sub Channel");
    RcDebugPrintLine (SDBG_MEM_TRAIN, 54, SINGLE_LINE, CRLF_FLAG, CRLF_FLAG);

    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        //
        // Skip the disabled channel.
        //
        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          SlewRateTrainingDone[Ch][SubChannel] = TRUE;
        }

        continue;
      }

      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

        //
        // Skip sub-channel that has been trained.
        //
        if (SlewRateTrainingDone[Ch][SubChannel] == TRUE) {
          continue;
        }

        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubChannel, NO_RANK, NO_STROBE, NO_BIT, "  Max Delta  = %d \n", MaxSlewRateDeltaPerSubCh[Ch][SubChannel]);

        if (ABS (MaxSlewRateDeltaPerSubCh[Ch][SubChannel]) <= MAX_SLEW_RATE_DELTA) {
          //
          // This sub-channel is done.
          //
          SlewRateTrainingDone[Ch][SubChannel] = TRUE;
          continue;
        }

      } // SubChannel  loop

    } // Ch loop
    //
    // Step 3: Check if all channels are done.
    //

    RcDebugPrint (SDBG_MAX, "\nSummary: DCA Slew Rate Training Result");
    RcDebugPrintLine (SDBG_MEM_TRAIN, 118, SINGLE_LINE, CRLF_FLAG, CRLF_FLAG);
    AllChannelDone = TRUE;
    for (Ch = 0; Ch < MaxChDdr;Ch += GetChInc (Host)) {
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }
      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        if (SlewRateTrainingDone[Ch][SubChannel] == FALSE) {
          AllChannelDone = FALSE;
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubChannel, NO_RANK, NO_STROBE, NO_BIT, "     Continue\n");
        } else {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubChannel, NO_RANK, NO_STROBE, NO_BIT, "     Finished\n");

        }
      }
    }
    RcDebugPrint (SDBG_MAX, "\n\n");

    //
    // Stop slew rate training if all channels are done.
    //
    if (AllChannelDone == TRUE) {
      break;
    }

    LoopCount ++;

  } // End while.

  //
  // Exiting Early Command Clock Training Mode for DCPMM.
  //
  SetAepTrainingMode (Host, Socket, DISABLE_TRAINING_STEP);

  RcDebugPrint (SDBG_MAX, "DCA slew rate training ALL sub-Channels finished.\n\n");

  RcFreePool (VrefMin0p7);
  RcFreePool (VrefMax0p7);
  RcFreePool (VrefNominal);

  return SUCCESS;
}


/**

  Trains DCA Slew Rate.

  @param  Host     Pointer to sysHost

  @retval SUCCESS  DCA slew rate training succeeds.
  @retval FAILURE  DCA slew rate training fails.

**/
UINT32
CaSlewRate (
  IN PSYSHOST  Host
  )
{
  UINT8                               Socket;
  UINT8                               Ch = 0;
  MRC_STATUS                          Status;
  CHIP_ERLY_CMD_CK_STRUCT             ChipErlyCmdCKStruct;
  UINT8                               TrainingMode = 0;
  UINT8                               Dimm;
  UINT8                               Rank;
  UINT8                               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ZeroMem ((UINT8 *)&ChipErlyCmdCKStruct, sizeof (ChipErlyCmdCKStruct));
  Socket = GetCurrentSocketId ();

  RcDebugPrint (SDBG_MAX, "Socket%x CaSlewRate Starts\n", Socket);
  if (!IsDdr5Present (Host, Socket)) {
    return FAILURE;
  }

  //
  // Return if this Socket is disabled
  //
  if (IsSocketDimmPopulated (Host, Socket) != TRUE) {
    return SUCCESS;
  }

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_CA_SLEW_RATE, Socket));

  //
  // This is a sub step of CA training.
  //
  if (!IsMemFlowEnabled (CaSlewRateTraining)) {
    return SUCCESS;
  }

  //
  // Use the same training mode as CA/CLK training.
  //
  Status = TrainingModeSelect (CaSlewRateTraining, &TrainingMode);
  if (Status != MRC_STATUS_SUCCESS) {
    return FAILURE;
  }

  Host->var.mem.checkMappedOutRanks = 1;

  DoCompDdrioPerSetupOption (Socket, TRAINING_COMP_EVERY_TRAINING);

  //
  // Enable normal CMD timing
  //
  SetNormalCMDTimingDdr5 (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    //
    // Turn on command/address output enable
    //
    TrnOnChipCMDADDOpEn (Host, Socket, Ch, &ChipErlyCmdCKStruct);
  } // Ch loop

  IO_Reset (Host, Socket);

  //
  // Only RDIMM and LRDIMM needs CA slew rate training step.
  //
  if (CheckRCDLoopback (Host)) {



    EnableDisableBcom (Host, Socket, TRUE);
    SetCurrentTestType (Socket, EarlyRcdCaLoopbackTest);

    //
    // Set the CA Pattern to xtalk pattern.
    //
    Host->var.mem.socket[Socket].TrainingStatus.Bits.TrainingPatternMode = CA_TRAINING_PATTERN_XTALK;
    DcaSlewRateTraining (Host, TrainingMode);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

      for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

        for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {

          if (IsChannelEnabled (Socket, Ch) == FALSE) {
            continue;
          }

          if (IsRankValid(Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) == FALSE) {
            continue;
          }

          SetCaTrainingModeDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, FALSE, Rising_Edge);
        }
      }
    }

    EnableDisableBcom (Host, Socket, FALSE);
  }

  //
  // Disable CADB always on Mode
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += 1) {
    DisableCADBAlwaysOnMode (Socket, Ch);
  }

  Host->var.mem.checkMappedOutRanks = 0;
  SetCurrentTestType (Socket, 0);

  return SUCCESS;
}

/**
Set QCA training results.
@param[in] Host                 - Pointer to Host structure
@param[in] socket               - Processor socket within the system (0-based)
@param[in] ch                   - DDR channel number within the processor socket (0-based)
@param[in] level                - IO level to access
@param[in] mode                 - Bit-field of different modes
@param[in] listType             - Selects type of each list
@param[in] listSize             - Number of entries in each list
@param[in] RcdBacksidesignalListEdgeCh  - Array of pointers to listType

@retval MRC_STATUS

**/
MRC_STATUS
EFIAPI
SetCombinedCaClkBacksideDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     socket,
  IN UINT8     ch,
  IN UINT8     subch,
  IN MRC_LT    level,
  IN UINT8     mode,
  IN UINT8     listType,
  IN UINT16    listSize,
  IN GSM_CSEDGE_CA (*RcdBacksidesignalListEdgeChInput) [SUB_CH][MAX_RANK_CH][COPY_NUM][BACKSIDE_CA_SIGNALS_TO_SWEEP]
  )
{

  UINT8                 dimm;
  UINT8                 rank;
  UINT8                 clk;
  UINT8                 listIndex;
  UINT8                 cmdPiGroup;
  UINT8                 ckEnabled[MAX_CLK_BS_SUBCH_DDR5];
  UINT8                 ctlIndex[MAX_CLK_BS_SUBCH_DDR5];
  INT16                 cmdLeft[DELAY_GROUP_BS_SUBCH_DDR5][MAX_CLK_BS_SUBCH_DDR5];
  INT16                 cmdRight[DELAY_GROUP_BS_SUBCH_DDR5][MAX_CLK_BS_SUBCH_DDR5];
  MRC_GT                cmdGroup[COPY_NUM][MAX_CLK_BS_SUBCH_DDR5];
  GSM_CSN               cmdSignal[COPY_NUM][MAX_CLK_BS_SUBCH_DDR5];
  INT16                 cmdOffset[COPY_NUM];
  UINT8                 SubChannel = 0;
  UINT8                 BacksideCopy = 0;
  MRC_STATUS            status;
  INT16                 MaxLeft;
  INT16                 MinRight;
  UINT32                SmbusData;
  INT8                  QcaDelayVal;
  GSM_CSEDGE_CA (*RcdBacksidesignalListEdgeCh) [SUB_CH][MAX_RANK_CH][COPY_NUM][BACKSIDE_CA_SIGNALS_TO_SWEEP];
  struct ddrRank (*rankList) [MAX_RANK_DIMM];
  struct dimmNvram (*dimmNvList) [MAX_DIMM];
  struct channelNvram (*ChannelNvList) [MAX_CH];

  MaxLeft = -UNMARGINED_CMD_EDGE;

  MinRight = UNMARGINED_CMD_EDGE;
  status = MRC_STATUS_SUCCESS;
  RcdBacksidesignalListEdgeCh = RcdBacksidesignalListEdgeChInput;
  dimmNvList = GetDimmNvList (Host, socket, ch);
  ChannelNvList = GetChannelNvList (Host, socket);

  for (clk = 0; clk < MAX_CLK_BS_SUBCH_DDR5; clk++) {//DDR5 8 clks
    ckEnabled[clk] = 0;
    ctlIndex[clk] = 0;
  } // clk loop

  //
  // Initialize the edges for each CMD pi group
  //
  for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
    for (cmdPiGroup = 0; cmdPiGroup < DELAY_GROUP_BS_SUBCH_DDR5; cmdPiGroup++) {
      //
      // Init CK composite edges to 255
      //
      for (clk = 0; clk < MAX_CLK_BS_SUBCH_DDR5; clk++) {
        cmdLeft[cmdPiGroup][clk] = -UNMARGINED_CMD_EDGE;
        cmdRight[cmdPiGroup][clk] = UNMARGINED_CMD_EDGE;
      } // clk loop
      cmdOffset[cmdPiGroup] = 0;
    } // cmdPiGroup loop.
  } //subchannel loop.
  if (listType == CMDCTL_SIGNAL_DELAY) {
    //
    // Combine edges if necessary
    //
    if (mode == GSM_COMBINE_EDGES) {
      RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Calculate CA Backside Composite edges per copy per sub channel\n");

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

          rankList = GetRankNvList (Host, socket, ch, dimm);
          //
          // Initialize the edges for each CMD pi group
          //
          for (cmdPiGroup = 0; cmdPiGroup < DELAY_GROUP_BS_SUBCH_DDR5; cmdPiGroup++) {
            //
            // Init CK composite edges to 255
            //
            for (clk = 0; clk < MAX_CLK_BS_SUBCH_DDR5; clk++) {
              cmdLeft[cmdPiGroup][clk] = -UNMARGINED_CMD_EDGE;
              cmdRight[cmdPiGroup][clk] = UNMARGINED_CMD_EDGE;
            } // clk loop
            cmdOffset[cmdPiGroup] = 0;
          } // cmdPiGroup loop.

          for (BacksideCopy = 0; BacksideCopy < COPY_NUM; BacksideCopy++) {
            if ((!(((*dimmNvList) [dimm].x4Present) && ((*dimmNvList) [dimm].numRanks == 2))) && (BacksideCopy == COPY_B)) {
              continue;
            }
            for (rank = 0; rank < (*dimmNvList) [dimm].numRanks; rank++) {

              if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
                continue;
              }
              //
              // Loop through the list to create the composite left and right edges for each CMD group (A, B)
              //
              for (listIndex = 0; listIndex < listSize - 1; listIndex++) {
                //
                // QACK = 0, QBCK = 1 , QCCK = 2 , QDCK = 3
                //
                clk = rank*2 + BacksideCopy; //DDR5_TODO. To confirm the CLK mapping.
                ckEnabled[clk] = 1;
                //
                // Update the composite left and right edges for the current CMD pi group relative to the clock
                //
                if ((*RcdBacksidesignalListEdgeCh) [SubChannel][ (*rankList) [rank].rankIndex][BacksideCopy][listIndex].toSample != 1) {
                  continue;
                }
                UpdateEdges ((*RcdBacksidesignalListEdgeCh) [SubChannel][ (*rankList) [rank].rankIndex][BacksideCopy][listIndex].le, (*RcdBacksidesignalListEdgeCh) [SubChannel][ (*rankList) [rank].rankIndex][BacksideCopy][listIndex].re,
                  &cmdLeft[BacksideCopy][clk], &cmdRight[BacksideCopy][clk]);
                cmdGroup[BacksideCopy][clk] = MrcGtDelim;
                cmdSignal[BacksideCopy][clk] = (*RcdBacksidesignalListEdgeCh) [SubChannel][ (*rankList) [rank].rankIndex][BacksideCopy][listIndex].signal;
                RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, SubChannel, rank, NO_STROBE, NO_BIT,
                  "%8a: Backside CA PI Group %2d, clk: %d, le = %3d re = %3d, Left = %3d Right = %3d\n",
                  GetSignalStrDdr5 ((*RcdBacksidesignalListEdgeCh) [SubChannel][ (*rankList) [rank].rankIndex][BacksideCopy][listIndex].signal), BacksideCopy, clk,
                  (*RcdBacksidesignalListEdgeCh) [SubChannel][ (*rankList) [rank].rankIndex][BacksideCopy][listIndex].le, (*RcdBacksidesignalListEdgeCh) [SubChannel][ (*rankList) [rank].rankIndex][BacksideCopy][listIndex].re,
                  cmdLeft[BacksideCopy][clk], cmdRight[BacksideCopy][clk]);
                if (MaxLeft < cmdLeft[BacksideCopy][clk]) {
                  MaxLeft = cmdLeft[BacksideCopy][clk];
                }
                if (MinRight > cmdRight[BacksideCopy][clk]) {
                  MinRight = cmdRight[BacksideCopy][clk];
                }
              } // listIndex loop
              RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "\n");
              SmbusData = ((MinRight + MaxLeft) / 2);
              if (!(*dimmNvList) [dimm].x4Present &&
                  (*dimmNvList) [dimm].numRanks == 1 &&
                  SmbusData > X8_WA_OFFSET &&
                  (*ChannelNvList) [ch].timingMode != TIMING_1N) {
                //
                // X8 DIMM WA
                //
                SmbusData = SmbusData - X8_WA_OFFSET;
              }
              RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, SubChannel, rank, NO_STROBE, NO_BIT,
                "Final setting: Left Edge %d, Right Edge %d, Eye Center %d\n", MaxLeft,  MinRight, SmbusData);

              //
              // Make sure the eye width is large enough
              //
              if ((MinRight - MaxLeft) < MIN_QCA_EYE) {
                RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Eye width is too small: %d\n", (MinRight - MaxLeft));
                EwlOutputType3 (WARN_FPT_CORRECTABLE_ERROR, WARN_CMD_PI_GROUP_SMALL_EYE, socket, ch, NO_DIMM, NO_RANK, EwlSeverityWarning, CmdAll, DdrLevel, cmdSignal[cmdPiGroup][clk], (UINT8) (cmdRight[cmdPiGroup][clk] - cmdLeft[cmdPiGroup][clk]));
                DisableChannelSw (Host, socket, ch);
              } else {
                QcaDelayVal = (INT8) SmbusData;
                ProgramFinalQcaDelayResult (Host, socket, ch, dimm, SubChannel, rank, BacksideCopy, GSM_FORCE_WRITE, QcaDelayVal, TRUE);
              }
            } // rank loop
          } //backside copy loop

          //EvaluateCaBacksideMargins (Host, socket, ch, ckEnabled, ctlIndex, cmdLeft, cmdRight, cmdGroup, cmdSignal, cmdOffset); //SPR_todo. HSD 1507417455
        }//subchannel loop
      } // dimm loop
    } else {
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "ERROR: This SetCombinedCmd mode not supported!\n");
    }
  } else {
    status = MRC_STATUS_GROUP_NOT_SUPPORTED;
  }
  return status;
} // SetCombinedCaClkBacksideDdr5

/**

  This sets the CA training mode in the register.

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number
  @param[in] Ch              - Channel number
  @param[in] SubChannelMask  - Sub Channel Mask
  @param[in] Dimm            - Dimm number
  @param[in] Rank            - Rank number
  @param[in] Enable          - Enable Ca Training Mode
  @param[in] DcaSamplingEdge - Dca Sampling Edge

  @retval N/A

**/
VOID
SetCaTrainingModeDdr5 (
  IN PSYSHOST          Host,
  IN UINT8             Socket,
  IN UINT8             Ch,
  IN UINT8             SubChannelMask,
  IN UINT8             Dimm,
  IN UINT8             Rank,
  IN BOOLEAN           Enable,
  IN DCA_SAMPLING_EDGE DcaSamplingEdge
  )
{
  UINT8                                               Value = 0;
  DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_STRUCT HostInterfaceTrainingMode;

  ReadRcdCwCache(Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_REG, &HostInterfaceTrainingMode.Data);
  if (Enable) {
    if (DcaSamplingEdge == Rising_Edge) {
      Value = 1;
    } else if (DcaSamplingEdge == Falling_Edge) {
      Value = 2;
    } else {
      Value = 0;
    }
  }

#ifdef USE_LATEST_RCD_SPEC
  HostInterfaceTrainingMode.Bits.dca_training_xor_sampling_edge = Value;
  HostInterfaceTrainingMode.Bits.host_interface_training_mode_selection_ch_a = Enable ? (SubChannelMask & BIT0) : 0;
  HostInterfaceTrainingMode.Bits.host_interface_training_mode_selection_ch_b = Enable ? (SubChannelMask & BIT1) >> 1 : 0;
#else
  HostInterfaceTrainingMode.Bits.dca_training_xor_sampling_edge_1 = Value;
  HostInterfaceTrainingMode.Bits.host_interface_tm_selection_cha = Enable ? (SubChannelMask & BIT0) : 0;
  HostInterfaceTrainingMode.Bits.host_interface_tm_selection_chb = Enable ? (SubChannelMask & BIT1) >> 1 : 0;
#endif

  if (IsDcpmmPresentDimm (Socket, Ch, Dimm)) {

    //
    // Update RCD cache before writing to CWV RW02 Interface register
    //
    WriteRcdCwCache (Socket, Ch, SUB_CH_A_MASK, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_REG, HostInterfaceTrainingMode.Data);

    ChipSetCTLLoopbackFmc (Host, Socket, Ch, Dimm, TRUE);

  } else {

    //
    // Firstly, send the NOP command to exit CATM.
    //
    if (FeaturePcdGet (PcdCteBuild) && Enable) {
      SendNop (Host, Socket, Ch, SubChannelMask, Dimm, Rank, 1, FALSE, TRUE);
    }
    RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_REG, HostInterfaceTrainingMode.Data);
  }
} // SetCaTrainingModeDdr5


/**

  This exits the CA training mode.

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number

  @retval N/A

**/
VOID
ExitCaTrainingModeDdr5 (
  IN PSYSHOST          Host,
  IN UINT8             Socket
  )
{
  UINT8                           Dimm;
  UINT8                           Rank;
  UINT8                           Ch;
  UINT8                           RankEnabled;
  struct dimmNvram (*DimmNvList)  [MAX_DIMM];
  UINT8                           MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  //
  // Put All the DIMMS and All the Ranks in normal mode.
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

      RankEnabled = 0;

      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if ((*DimmNvList) [Dimm].dimmPresent == 0) {
          continue;
        }
        if (((*DimmNvList) [Dimm].DcpmmPresent) && ((*DimmNvList) [Dimm].fmcType == FMC_CWV_TYPE)) {
          continue;
        }
        if (Rank >= (*DimmNvList) [Dimm].numRanks) {
          continue;
        }
        RankEnabled = 1;
        break;
      } // Ch loop

      //
      // Continue to the next Rank if this one is not populated
      //
      if (RankEnabled == 0) {
        continue;
      }

      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }

        EnterRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);

        //
        // Exit DRAM CA Training Mode.
        //
        SendNop (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, 0, TRUE, TRUE);

        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
          "Exit Backside DRAM CA Training Mode \n");

      } //Ch loop

      //
      // Exit CA Pass through Mode.
      //
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }

        ExitRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);
      } //Ch loop
    } // Rank loop
  }// Dimm loop
} // ExitCaTrainingModeDdr5

/**
  Get CA Vref initial value.

  The initial value (i.e. the value to use for the first step of the sweep) is
  based on the nominal value, the step count, and the step size. The nominal
  value comes from the ODT table, via the channel NV list. The step count and
  size are macros.

  @param[in]  Host        Pointer to sysHost.
  @param[in]  Socket      Socket number.
  @param[in]  Channel     Channel number on socket.
  @param[in]  Dimm        DIMM number on channel.
  @param[in]  Rank        Rank number on DIMM.

  @return   CA Vref initial value.
**/
INT8
GetCaVrefInitValue (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  UINT8     Dimm,
  IN  UINT8     Rank,
  IN  INT8      CaVrefStepCount,
  IN  INT8      CaVrefStepSize
  )
{
  CHANNEL_NVRAM_STRUCT  (*ChannelNvList)[MAX_CH];
  UINT8                 RankIndex;
  INT8                  Nominal;
  INT8                  Initial;

  ChannelNvList = GetChannelNvList (Host, Socket);
  RankIndex = GetRankIndex (Socket, Channel, Dimm, Rank);

  Nominal = (*ChannelNvList)[Channel].DcaVrefRegValue[RankIndex];
  Initial = Nominal - ((CaVrefStepCount / 2) * CaVrefStepSize);

  //
  // Negative values are not valid. Choose the smallest positive that will
  // still land exactly on the nominal for one of the steps.
  //
  if (Initial < 0) {
    Initial +=  CaVrefStepSize * ((-Initial + CaVrefStepSize - 1) / CaVrefStepSize);
  }

  return Initial;
}

/**
  Callback to be executed before QCA sweep is done

  @param[in]      CommonParameters    Pointer to the context information
  @param[in]      TestType            Current Test Type

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
PreSweepQca1DWorker (
  IN        COMMON_1D_PARAMETERS    *CommonParameters,
  IN        MRC_TT                  TestType
  )
{
  UINT8                   ChannelIndex;
  UINT8                   MaxCh;
  CHIP_ERLY_CMD_CK_STRUCT ChipErlyCmdCKStruct;

  MaxCh = CommonParameters->MaxChDdr;
  ZeroMem ((UINT8 *)&ChipErlyCmdCKStruct, sizeof (ChipErlyCmdCKStruct));

  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {
    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    TrnOnChipCMDADDOpEn (
      CommonParameters->Host,
      CommonParameters->Socket,
      ChannelIndex,
      &ChipErlyCmdCKStruct
      );
  }

  SetCurrentTestType (CommonParameters->Socket, TestType);
  CommonParameters->Host->var.mem.socket[CommonParameters->Socket].TrainingStatus.Bits.TrainingPatternMode = QCA_TRAINING_PATTERN_SIMPLE;

  IO_Reset (
    CommonParameters->Host,
    CommonParameters->Socket
    );

  //
  // Enter sense amp training Mode to all the channels here to latch results.
  //
  SetTrainingModeLatchResult (
    CommonParameters->Host,
    CommonParameters->Socket,
    CHECKPOINT_MINOR_SENSE_AMP);

  IO_Reset (
    CommonParameters->Host,
    CommonParameters->Socket
    );

  //
  // Below was refectored from the QcsQcaClkDdrioInit () of the EarlyCaClkVrefCABackside()
  // to program the setting per channel.
  //
  SetTrainingModeDdr5 (CommonParameters->Socket, TmQcsQcaClk);

  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {
    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    //
    // Set up sample_sel, dq_sample_mask
    //
    QcsQcaClkDdrioInitByChannel (
      CommonParameters->Socket,
      ChannelIndex,
      CommonParameters->Dimm[ChannelIndex]);

    ConfigQcaSampleSelection (
      CommonParameters->Socket,
      ChannelIndex,
      CommonParameters->Dimm[ChannelIndex],
      CommonParameters->Rank[ChannelIndex]);
  }

  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    //
    // Enter DB DQ pass-through mode for read so that the CATM feedback can be
    // sent back to host.
    //
    SetDbDqPassThroughModeForRead (
      CommonParameters->Socket,
      ChannelIndex,
      CommonParameters->Dimm[ChannelIndex],
      CommonParameters->Rank[ChannelIndex],
      TRUE);

    EnterRcdCaPassThroughMode (
      CommonParameters->Host,
      CommonParameters->Socket,
      ChannelIndex,
      CommonParameters->Dimm[ChannelIndex],
      CommonParameters->Rank[ChannelIndex]
    );

     //
     // Enter DRAM CA Training Mode.--DIMM sub channel A and B at the same time.
     //
     WriteMPC (CommonParameters->Host,
      CommonParameters->Socket,
      ChannelIndex,
      CommonParameters->Dimm[ChannelIndex],
      TWO_SUBCH_MASK,
      CommonParameters->Rank[ChannelIndex],
      Enter_CA_Training_Mode,
      TIMING_4N, TRUE);
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Callback to be executed before RMT QCA sweep is done

  @param[in]      CommonParameters    Pointer to the context information
  @param[in]      TestType            Current Test Type
  @param[in]      TrainingPatternMode          Current Test Type

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
PreSweepQca1DRmt (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{

  SSA_MARGIN_CA *PrivateQca;
  PrivateQca = (SSA_MARGIN_CA *)CommonParameters;
  PreSweepQca1DWorker (CommonParameters, PrivateQca->TestType);

  return MRC_STATUS_SUCCESS;
}

/**
  Callback to configure the engine to do the QCA test by test type

  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      TestType          Indicates the test type to be configured
  @param[in]      CountWindow       Count Window Value to program

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetupTestQcaWorker (
  IN COMMON_1D_PARAMETERS    *CommonParameters,
  IN MRC_TT                  TestType,
  IN UINT32                  CountWindow
  )
{
  MRC_MST MemSsType;
  MRC_RT  MrcRankList;
  UINT8   ChannelIndex;
  UINT8   SubChannelIndex;
  UINT32  ChannelMask;
  UINT8   MaxCh;

  ZeroMem ((UINT8*)&MrcRankList, sizeof (MrcRankList));

  MaxCh = CommonParameters->MaxChDdr;
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[CommonParameters->Socket];

  ChannelMask = 0;
  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    ChannelMask |= 1 << ChannelIndex;

    MrcRankList.Dimm = CommonParameters->Dimm[ChannelIndex];
    MrcRankList.Rank = CommonParameters->Rank[ChannelIndex];

    for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
      SetupTest (
        CommonParameters->Host,
        CommonParameters->Socket,
        MemSsType,
        (1 << ChannelIndex),
        1,
        &MrcRankList,
        DdrLevel,
        TestType,
        GetQcaSignal(SubChannelIndex, CommonParameters->Signal),
        0,
        0
      );
    } // SubChannelIndex
  } // ChannelIndex

  for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
    TestControl (
      CommonParameters->Host,
      CommonParameters->Socket,
      MemSsType,
      ChannelMask,
      SubChannelIndex,
      TestType,
      1,
      START_TEST,
      NOT_STOP_TEST
      );
  }

  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    //
    // Program counting window settings //DDR5_TODO
    //
    SetDataTrainCountWindowByChannel (
      CommonParameters->Socket,
      ChannelIndex,
      (UINT8)CountWindow
      );

    SetDqCountingWindowByChannel (
      CommonParameters->Socket,
      ChannelIndex
      );
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Callback to configure the engine to do the Rmt QCA test

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetupTestQcaRmt (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  //
  // RMT shall select test type based on its private data, or override the counting window size based on CPGC configure.
  //
  SSA_MARGIN_CA *PrivateQca;
  PrivateQca = (SSA_MARGIN_CA *)CommonParameters;

  return SetupTestQcaWorker (CommonParameters, PrivateQca->TestType, PrivateQca->CountWindow);
}

/**
  Callback to configure and start the QCA test

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
ExecuteTestQca (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  UINT8   ChannelIndex;
  UINT8   MaxCh;

  MaxCh = CommonParameters->MaxChDdr;

  //
  // Generate an IO Reset. This is required because the pi delays changed.
  //
  IO_Reset (
    CommonParameters->Host,
    CommonParameters->Socket
    );

  //
  // Enabling the counter Mode.
  //
 for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {

  if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
    continue;
  }

  EnableDdrioTrainingCounterByChannel (
    CommonParameters->Host,
    CommonParameters->Socket,
    ChannelIndex
    );
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Override the QCA feedback results for RcSim.

  @param[in]  CommonParamaters            - Pointer to the context information
  @param[in]  Channel                     - Channel number
  @param[out] DqSampleResultStrobeSubCh   - An array of feedback results. Index by [SUB_CH][STROBE]

  @retval N/A

**/
VOID
QcaMarginsSweepErrorResFeedbackOverride (
  IN     COMMON_1D_PARAMETERS    *CommonParameters,
  IN     UINT8                   Channel,
     OUT UINT32                  DqSampleResultStrobeSubCh[SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
}


/**
  Callback to interpret the HW results after each sample test completes

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
ResultsProcessingQca (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  MRC_MST               MemSsType;
  UINT8                 ChannelIndex;
  UINT8                 SubChannelIndex;
  UINT32                ChannelMask;
  UINT8                 MaxCh;
  UINT32                DqSampleResultStrobeSubCh[SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT32                ValidMask[MAX_STROBE_DDR5];
  UINT8                 StrobeLogical;
  UINT8                 StrobeLinear;
  UINT8                 BitIndex;
  RESULTS_1D_CONDITION  *ResultConditionPtr;

  MaxCh = CommonParameters->MaxChDdr;
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[CommonParameters->Socket];

  ZeroMem ((UINT32 *) DqSampleResultStrobeSubCh, sizeof (DqSampleResultStrobeSubCh));

  ChannelMask = 0;
  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    ChannelMask = 1 << ChannelIndex;
    GetTrainResultStatus (
      CommonParameters->Host,
      CommonParameters->Socket,
      MemSsType,
      ChannelMask,
      NO_SUBCH,
      sizeof (DqSampleResultStrobeSubCh),
      (UINT32*) &DqSampleResultStrobeSubCh,
      ValidMask,
      0,
      CommonParameters->Dimm[ChannelIndex]
      );


    for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {

      for (StrobeLinear = 0; StrobeLinear < CommonParameters->MaxStrobeAvailable; StrobeLinear++) {

        StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);

        for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {

          ResultConditionPtr = GetResultConditionPtr (CommonParameters, 0, ChannelIndex, StrobeLogical, BitIndex);

          if (GetCurrentTestType (CommonParameters->Socket) == EarlyCaBacksideLoopbackTest) {

            if (DqSampleResultStrobeSubCh[SubChannelIndex][StrobeLinear] > 0) {
              ResultConditionPtr->Current = FAIL_CONDITION;
            } else {
              ResultConditionPtr->Current = PASS_CONDITION;
            }

          } else {

            RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_224);

          }
        } // BitIndex loop ...
      } // StrobeLinear
    } // SuBChannelIndex loop ...
  } // ChannelIndex loop ...

  return MRC_STATUS_SUCCESS;
}

/**
  Callback to be executed when all sides are done

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
PostSweepQca1D (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  MRC_MST                 MemSsType;
  MRC_RT                  MrcRankList;
  UINT8                   ChannelIndex;
  UINT8                   SubChannelIndex;
  UINT32                  ChannelMask;
  UINT8                   MaxCh;

  MaxCh = CommonParameters->MaxChDdr;
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[CommonParameters->Socket];

  ChannelMask = 0;
  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    ChannelMask |= 1 << ChannelIndex;

  }

  for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
    TestControl (
      CommonParameters->Host,
      CommonParameters->Socket,
      MemSsType,
      ChannelMask,
      SubChannelIndex,
      EarlyCaBacksideLoopbackTest,
      1,
      NOT_START_TEST,
      STOP_TEST
      );
  }

  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    MrcRankList.Dimm = CommonParameters->Dimm[ChannelIndex];
    MrcRankList.Rank = CommonParameters->Rank[ChannelIndex];

    //
    // Exit CATM
    //
    SendNop (
      CommonParameters->Host,
      CommonParameters->Socket,
      ChannelIndex,
      TWO_SUBCH_MASK,
      MrcRankList.Dimm,
      MrcRankList.Rank,
      0,
      TRUE,
      TRUE
      );

    //
    // Exit CA Pass through Mode.
    //
    ExitRcdCaPassThroughMode (
      CommonParameters->Host,
      CommonParameters->Socket,
      ChannelIndex,
      CommonParameters->Dimm[ChannelIndex],
      CommonParameters->Rank[ChannelIndex]
    );
  }

  FifoTrainReset (
    CommonParameters->Host,
    CommonParameters->Socket);

  IO_Reset (
    CommonParameters->Host,
    CommonParameters->Socket);

  ExitCaTrainingModeDdr5 (
    CommonParameters->Host,
    CommonParameters->Socket);

  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }
    DisableCADBAlwaysOnMode (
      CommonParameters->Socket,
      ChannelIndex);
  }

  //
  // Exit sense amp training Mode
  //
  SetTrainingModeLatchResult (
    CommonParameters->Host,
    CommonParameters->Socket,
    CHECKPOINT_MINOR_DISABLE_SENSE_AMP);

  //
  // Exit DB DQ pass-through mode for read so that the CATM feedback can be
  // sent back to host.
  //
  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }
    SetDbDqPassThroughModeForRead (
      CommonParameters->Socket,
      ChannelIndex,
      CommonParameters->Dimm[ChannelIndex],
      CommonParameters->Rank[ChannelIndex],
      FALSE);
  }

  return MRC_STATUS_SUCCESS;
}

/**

  Search the data collected in Results and update margin results in channel/subchannel/signal array
  WARNING:
  This function MUST be executed only after context was already created (CreateContextParameters).
  This function MUST be executed only after resources are allocated (AllocatePoolForResults1D ).

  @param[in]      CommonParameters  Pointer to the context information
  @param[in]      Results1D         Pointer to 1D results
  @param[in]      SignalIndex       The index of signal in DcaResults arrray
  @param[out]     QcaResults        Pointer that contains per channel, subchannel and signal margin result.

**/
VOID
UpdateQcaMarginResultsFromCurrentResults1D (
  IN     COMMON_1D_PARAMETERS    *CommonParameters,
  IN     RESULTS_1D              *Results1D,
  IN     UINT8                   SignalIndex,
     OUT struct baseMargin       *QcaResults
  )
{
  UINT8             ChannelIndex;
  UINT8             SubChannelIndex;
  UINT8             StrobeLogical;
  struct baseMargin *StrobeMargin;
  struct baseMargin *QcaResultForCurrentSignal;
  UINT8             BacksideSignalSweepIndex;

  for (ChannelIndex = 0; ChannelIndex < CommonParameters->MaxChDdr; ChannelIndex++) {
    if (!CommonParameters->ChannelEnabledLocal[ChannelIndex]) {
      continue;
    }

    for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {

      StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, 0);

      StrobeMargin = GetResultPtrAtStrobeLevel (CommonParameters, 0, ChannelIndex, StrobeLogical, Results1D);

      //
      // QcaResults is indexed as [MAX_CH][SUB_CH][SIGNAL_CNT]
      //
      QcaResultForCurrentSignal = &QcaResults[ChannelIndex * ((SUB_CH)* BACKSIDE_CA_SIGNALS_TO_SWEEP) + SubChannelIndex * BACKSIDE_CA_SIGNALS_TO_SWEEP + SignalIndex];

      QcaResultForCurrentSignal->p = StrobeMargin->p;
      QcaResultForCurrentSignal->n = StrobeMargin->n;

      BacksideSignalSweepIndex = SignalIndex + SubChannelIndex * BACKSIDE_CA_SIGNALS_TO_SWEEP;
      RcDebugPrint (SDBG_SWEEP_L0, "\nCh %d, SubCh %d, Signal %a, Margin low %d high %d\n",
                    ChannelIndex, SubChannelIndex, GetSignalStr (SignalsToSweepBacksideDDR5[BacksideSignalSweepIndex]), QcaResultForCurrentSignal->n, QcaResultForCurrentSignal->p);

    } // SubChannelIndex loop ...
  } // ChannelIndex loop
}

/**
  Sweep QCA signals and return margin results for EV tests.

  @param[in]      TestType:         Indicates the test type to be configured. It can use to customize the EV margin test setup
  @param[in]      Socket            Processor socket within the system (0-based)
  @param[in]      ChannelMask       One-hot encoding to mask a channel (0: executes if enabled/populated, 1: skip/masked)
  @param[in]      DimmRank          Structure to specify current dimm and rank under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test
  @param[in]      Direction         Specifies the sweep direction: low side only, high side only, or both side
  @param[in]      StepSize          Specifies the StepSize to do the margining
  @param[in]      CountWindow       Count Window Value
  @param[in]      SignalCount       Specifies the maximum signal count in the margin results structure - "QcaResults"
  @param[in, out] DcaResults        Pointer to margin results data structure which is indexed as [MAX_CH][SUB_CH][Signal]

  @retval MRC_STATUS_SUCCESS
**/
MRC_STATUS
EFIAPI
GetQcaMargins (
  IN     MRC_TT            TestType,
  IN     UINT8             Socket,
  IN     UINT32            ChannelBitMask,
  IN     MRC_RT            DimmRank[MAX_CH],
  IN     MRC_LT            Level,
  IN     MRC_GT            Group,
  IN     UINT8             Direction,
  IN     UINT8             StepSize,
  IN     UINT32            CountWindow,
  IN     UINT8             SignalCount,
  IN OUT struct baseMargin *QcaResults
  )
{
  UINT8                   TrainingMode;
  UINT8                   SignalIndex;
  UINT8                   ChannelIndex;
  UINT8                   SubChannelIndex;
  UINT8                   SubChannelParameter;
  SSA_MARGIN_CA           PrivateQca;
  RESULTS_1D              Results;
  MRC_MST                 MemSsType;
  FLAGS_CATEGORY_OFFSETS  FlagsOffsets;
  FLAGS_CATEGORY_RESULTS  FlagsResults;
  FLAGS_CATEGORY_FSM      FlagsFsm;
  PSYSHOST                Host;
  UINT8                   MaxCh;
  DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_STRUCT         RcdParityAlert[MAX_CH];
  DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_STRUCT HostInterfaceTrainingMode[MAX_CH];

  //
  //  Use for debugging sweep
  //
  //UINT32                  PrevDebugLevel;


  ZeroMem ((UINT8 *) RcdParityAlert, sizeof (RcdParityAlert));
  ZeroMem ((UINT8 *) HostInterfaceTrainingMode, sizeof (HostInterfaceTrainingMode));

  //
  // Make sure output storage size have enough space to store margin results of all signals
  //
  RC_FATAL_ERROR (SignalCount <= BACKSIDE_CA_SIGNALS_TO_SWEEP, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_238);

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

  MaxCh = GetMaxChDdr ();

  Host = GetSysHostPointer ();

  PrivateQca.TestType = TestType;
  PrivateQca.CountWindow = CountWindow;

  //
  // Save
  //
  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {
    if (!IsChannelEnabled (Socket, ChannelIndex)) {
      continue;
    }
    if (ChannelBitMask & (1 << ChannelIndex)) {
      continue;
    }

    ReadRcdCwCache (Socket, ChannelIndex, SUB_CH_A, DimmRank[ChannelIndex].Dimm, RDIMM_CW_PAGE0, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, &RcdParityAlert[ChannelIndex].Data);
    ReadRcdCwCache (Socket, ChannelIndex, SUB_CH_A, DimmRank[ChannelIndex].Dimm, RDIMM_CW_PAGE0, DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_REG, &HostInterfaceTrainingMode[ChannelIndex].Data);
  }

  TrainingModeSelect (CaClkTraining, &TrainingMode);

  FlagsOffsets = DefaultStartOffset | LimitSearch;

  //
  // SingleBitTrack: because the DQ counter checks the feedback of the first bit of each nibble/byte
  // ResultsPerSubChannel: because the margin result is per rank. Should stop at the first failure of any of the nibble/byte.
  //
  FlagsResults = DefaultResults | SingleBitTrack | ResultsPerSubChannel | ResultsRelative;
  if (Direction == SWEEP_NEG_SIDE_ONLY) {
    FlagsFsm = LeftSideOnly;
  } else if (Direction == SWEEP_POS_SIDE_ONLY) {
    FlagsFsm = RightSideOnly;
  } else {
    FlagsFsm = DefaultFsm; // both side.
  }

  for (SubChannelIndex = 0; SubChannelIndex < SUB_CH; SubChannelIndex++) {
    for (SignalIndex = 0; SignalIndex < SignalCount; SignalIndex++) {

      if (TrainingMode == RUN_IN_PARALLEL) {
        SubChannelParameter = ALL_SUBCH;
      } else {
        SubChannelParameter = SubChannelIndex;
      }

      RcDebugPrint (SDBG_MAX, "**********Signal %a Group %d\n", GetSignalStr (GetQcaSignal(SubChannelIndex, SignalsToSweepBacksideDDR5[SignalIndex])), Group);

      CreateContextParameters (
        (COMMON_1D_PARAMETERS *)&PrivateQca,
        MemSsType,
        Socket,
        ChannelBitMask,
        0, // RankMask
        SubChannelParameter,
        DimmRank,
        Level,
        Group,
        SignalsToSweepBacksideDDR5[SignalIndex], // Q0ACA0 - Q0ACA12
        StepSize,
        1,  // DitherValue
        -1, // OffsetLeft
        0,  // OffsetRight
        64,  // LimitOffset
        FlagsOffsets, FlagsResults, FlagsFsm
      );

      AllocatePoolForResults1D ((COMMON_1D_PARAMETERS *)&PrivateQca, &Results);

      //
      // Enable sweep debug log
      //
      //PrevDebugLevel = GetDebugLevel ();
      //((COMMON_1D_PARAMETERS *)&PrivateQca)->DebugLevel |= SDBG_SWEEP_L3 | SDBG_SWEEP_L2 | SDBG_SWEEP_L1 |SDBG_SWEEP_L0;
      //SetDebugLevel (SDBG_SWEEP_L3 | SDBG_SWEEP_L2 | SDBG_SWEEP_L1 |SDBG_SWEEP_L0 |PrevDebugLevel);

      ProcessSweep1D (
        (COMMON_1D_PARAMETERS *)&PrivateQca,
        PreSweepQca1DRmt,
        SetupTestQcaRmt,
        ExecuteTestQca,
        NULL,
        PostSweepQca1D,
        ResultsProcessingQca,
        &Results
      );

      UpdateQcaMarginResultsFromCurrentResults1D ((COMMON_1D_PARAMETERS *)&PrivateQca, &Results, SignalIndex, QcaResults);

      ReleaseResources1D ((COMMON_1D_PARAMETERS *)&PrivateQca, &Results);

      //
      // Restore debug level
      //
      //SetDebugLevel (PrevDebugLevel);

    } // Signal loop ...

    if (TrainingMode == RUN_IN_PARALLEL) {
      break;
    }

  } // SubChannelIndex loop ...

  //
  // Restore
  //
  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {
    if (!IsChannelEnabled (Socket, ChannelIndex)) {
      continue;
    }
    if (ChannelBitMask & (1 << ChannelIndex)) {
      continue;
    }

    RcdControlWordWrite (Socket, ChannelIndex, 1 << SUB_CH_A, DimmRank[ChannelIndex].Dimm, DimmRank[ChannelIndex].Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, RcdParityAlert[ChannelIndex].Data);
    RcdControlWordWrite (Socket, ChannelIndex, 1 << SUB_CH_A, DimmRank[ChannelIndex].Dimm, DimmRank[ChannelIndex].Rank, DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_REG, HostInterfaceTrainingMode[ChannelIndex].Data);
  }

  return MRC_STATUS_SUCCESS;
} // GetQcaMargins
