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

#ifndef  _Ddr5CoreLib_h
#define  _Ddr5CoreLib_h

#include <ReferenceCodeDataTypes.h>
#include "SysHost.h"
#include <Library/MemSweepLib.h>

#define CWLC_DDR5   CWLC_DDR4 //DDR5_TODO.

#define  LOGICAL_STROBE_0   0
#define  LOGICAL_STROBE_1   1
#define  LOGICAL_STROBE_2   2
#define  LOGICAL_STROBE_3   3
#define  LOGICAL_STROBE_4   4
#define  LOGICAL_STROBE_5   5
#define  LOGICAL_STROBE_6   6
#define  LOGICAL_STROBE_7   7
#define  LOGICAL_STROBE_8   8
#define  LOGICAL_STROBE_9   9
#define  LOGICAL_STROBE_10  10
#define  LOGICAL_STROBE_11  11
#define  LOGICAL_STROBE_12  12
#define  LOGICAL_STROBE_13  13
#define  LOGICAL_STROBE_14  14
#define  LOGICAL_STROBE_15  15
#define  LOGICAL_STROBE_16  16
#define  LOGICAL_STROBE_17  17
#define  LOGICAL_STROBE_18  18
#define  LOGICAL_STROBE_19  19

//
// Used to supress ACT and PRE commands for MRR
//
#define FILTER_ACT_PRE_ENABLE               1
#define FILTER_ACT_PRE_DISABLE              0

//
// Training specific defines
//
#define DDR5_TRAIN_RESULTS_LEN            10 // The number of DWORDs required to store training information
#define BACKSIDE_RCSIM_FEEDBACK_LEN        4
#define DDR5_TRAIN_RESULTS_DWL_PHASE_LEN  64
#define DDR5_TRAIN_RESULTS_MDQ_LEN        32
#define CSCA_TF_COUNTING_WINDOW_VALUE     64
#define DCA_DUTY_CYCLE_COUNTING_WINDOW_VALUE 4096
#define DQ_COUNTING_WINDOW_VALUE          64
#define REQ_COUNTING_WINDOW_VALUE         32
#define WR_LVL_FINE_STEP_SIZE_DDR5        1
#define WR_LVL_FINE_PI_START_DDR5         -64
#define WR_LVL_FINE_PI_END_DDR5           96
#define CA_CLK_TEST_RANGE                 512
#define CA_CLK_TEST_RANGE_BACKSIDE        256
#define CA_CLK_TEST_RANGE_BACKSIDE_2N     256
#define CA_CLK_TEST_RANGE_BACKSIDE_1N     128
#define CA_TRAINING_PATTERN_SIMPLE         0
#define CA_TRAINING_PATTERN_XTALK          1
#define QCA_TRAINING_PATTERN_SIMPLE        2
#define QCA_TRAINING_PATTERN_XTALK         3
#define DCA_DCK_DUTYE_CYCLE_TRAINING_PATTERN_COMPLEX               4
//
// Pattern used for the DCA DFE VREF margin. It has the same DCA patterns as DCA duty
// cycle test except the DCS_n is always driving low.
//
#define DCA_DCK_DUTYE_CYCLE_TRAINING_PATTERN_COMPLEX_CS_ALWAYS_LOW 5
#define NUM_CA_SIGNALS_DDR5                30
#define SWIZZLE_RESULTS_LEN                4


//
// The following need to move to SOC specific library
//
#define CHIP_GSM_CSN_SIGNALSTOSWEEP_SPR {A0A, A1A, A2A, A3A, A4A, A5A, A6A, PARA, A7A, A8A, A9A, A10A, A11A, A12A, A13A, A0B, A1B, A2B, A3B, A4B, A5B, A6B, PARB, A7B, A8B, A9B, A10B, A11B, A12B, A13B} //DDR5_TODO
#define CHIP_GSM_CSN_BACKSIDE_SIGNALSTOSWEEP_SPR {Q0ACA0, Q0ACA1, Q0ACA2, Q0ACA3, Q0ACA4, Q0ACA5, Q0ACA6, Q0ACA7, Q0ACA8, Q0ACA9, Q0ACA10, Q0ACA11, Q0ACA12, Q0ACA13, Q1ACA0, Q1ACA1, Q1ACA2, Q1ACA3, Q1ACA4, Q1ACA5, Q1ACA6, Q1ACA7, Q1ACA8, Q1ACA9, Q1ACA10, Q1ACA11, Q1ACA12, Q1ACA13} // DDR5_TODO
#define RCD_CA_SIGNALS_TO_SWEEP       8
#define DRAM_CA_SIGNALS_TO_SWEEP      NUM_CA_SIGNALS_DDR5/2
#define BACKSIDE_CA_SIGNALS_TO_SWEEP  14
#define CS_CLK_PI_START      0
#define CS_CLK_TEST_RANGE    256
#define BS_CS_CLK_PI_START   0
#define BS_CS_CLK_TEST_RANGE 128
#define CA_FULL_EYE_WIDTH    128
#define BACKSIDE_1TCK        64
#define QCA_VREF_INIT_VALUE   0x45  //62.9% according to DDR5_module_specific_SPD--backside_KIT_Draft1_01062020
#define NO_SIGNAL             0xff
#define MDQ_VREF_INIT_VALUE_DR  0x2A               // 76.56% according to "MDQ-BCOM" sheet of the ODT_spr_ver9.37_backside_only.xlsx.  125 - (765.6 -350) /5 = 42 = 0x2A
#define MDQ_VREF_INIT_VALUE_SR  0x3B               // 68.09% according to "MDQ-BCOM" sheet of the ODT_spr_ver9.37_backside_only.xlsx.  125 - (680.9 -350) /5 = 59 = 0x3B
#define BACKSIDE_DRAM_DQ_VREF_INIT_VALUE_DR  0x36  // 70.73% according to "MDQ-BCOM" sheet of the ODT_spr_ver9.37_backside_only.xlsx.  125 - (707.3 -350) /5 = 54 = 0x36
#define BACKSIDE_DRAM_DQ_VREF_INIT_VALUE_SR  0x3B  // 68.09% according to "MDQ-BCOM" sheet of the ODT_spr_ver9.37_backside_only.xlsx.  125 - (680.9 -350) /5 = 59 = 0x3B
#define B_VREF_INIT_VALUE       0x47
#define B_VREF_CPS_INIT_VALUE   0x37

#define CS_VREF_STEP_COUNT    5
  #ifdef SPR_PO_FLAG
    #define QCS_VREF_STEP_START   0
    #define QCS_VREF_STEP_COUNT   1
  #else
    #define QCS_VREF_STEP_START   -2
    #define QCS_VREF_STEP_COUNT   5
  #endif   // #ifdef SPR_PO_FLAG

#define CS_VREF_STEP_SIZE     7     // 3.5% per step
#define QCS_VREF_STEP_SIZE    7     // 3.5% per step


#define CA_VREF_STEP_COUNT   34
#define CA_VREF_STEP_SIZE    3

//
// DCA Vref sweep for Slew Rate Training on real silicon
//
#define CA_VREF_STEP_COUNT_FOR_SLEW_RATE   CA_VREF_STEP_COUNT
#define CA_VREF_STEP_SIZE_FOR_SLEW_RATE    CA_VREF_STEP_SIZE


#define QCA_VREF_STEP_SIZE    1
#define MIN_QCA_EYE          20 // DDR5_TODO
#define CHIP_GSM_CSN_BCOM_SIGNALSTOSWEEP_SPR {BCOM_A, BCOM_B}

#define BCOM_VREF_STEP_SIZE                  1    // Step by 1% of Vdd
#define BCOM_VREF_STEP_SIZE_ENCODED_VALUE    2    // Step by 2 (1% Vdd) in DB PG[2]RWFA BVref Control Word
#define BCOM_SIGNALS_TO_SWEEP  1
#define BCOM_BCK_DELAY        32


#define BCOM_CLK_STEP_SIZE     1

#define BCOM_VREF_INIT_VALUE   74  // 74% of Vdd
#define MAX_BCOM_VREF_VALUE    76  // 76% of Vdd

#define CA_CLK_STEP_SIZE       1
#define CS_CLK_STEP_SIZE       1
#define REQ_CLK_STEP_SIZE      1


#define QCA_VREF_STEP_COUNT    1
#define BCOM_CLK_TEST_START    0
#define BCOM_CLK_TEST_RANGE    128
#define BCOM_CLOCK_CYCLE       64

//RCD Size Variables
#define BYTE_WRITE    1
#define BLOCK_WRITE   4
#define MAX_SIZE      4

#ifdef USE_LATEST_DRAM_SPEC
//
// Max range based on 0.87 DDR5 spec
// Linear number of steps
//
#define DDR5_MAX_VREF_RANGE       125
#else
//
// Max range based on 0.78 DDR5 spec
// Linear number of steps
//
#define DDR5_MAX_VREF_RANGE       105
#endif //USE_LATEST_DRAM_SPEC

#define COPY_NUM                  2
#define COPY_A                    0
#define COPY_B                    1
#define MAX_SAMPLING_LOOP         2
#define EVEN_SAMPLING             0
#define ODD_SAMPLING              1

#define CSA_TRIGGER_ENABLE        1
#define CSA_TRIGGER_DISABLE       0

#define CW_DRAM                   0
#define CW_RCD_DB                 1

#define DB_PER_SUBCH              5 // 5 databuffers per subchannel
#define DB_DQ_LANE                8 // 8 DQ lanes per DB
//
// Sweep direction for DCA/DCS margin sweep
//
#define SWEEP_NEG_SIDE_ONLY       0
#define SWEEP_POS_SIDE_ONLY       1
#define SWEEP_BOTH_POS_NEG_SIDE   2

//
// Assert/Deassert DB BRST_n
//
#define DB_BRST_ASSERT TRUE
#define DB_BRST_DEASSERT FALSE

//
// Structures used to track data while training
//

//
// DDR5 training data structure used to store training information
//
#pragma pack (push,1)
typedef struct {
  BOOLEAN  BcomTrainingDone;                                                            // Indicates whether or not BCOM training is complete
  BOOLEAN  NibbleSwap[MAX_CH][MAX_DIMM][SUB_CH][MAX_STROBE_DDR5 / 2];                   // Used to store LRDIMM (low/high) nibble swap information
  UINT32   DbToDramDqMap[MAX_CH][MAX_DIMM][MAX_RANK_DIMM][SUB_CH][SWIZZLE_RESULTS_LEN]; // Used to store Data Buffer to DRAM DQ map
  UINT32   HostToDbDqMap[MAX_CH][MAX_DIMM][SUB_CH][SWIZZLE_RESULTS_LEN];                // Used to store Data Buffer to Host DQ map
} DDR5_LRDIMM_TRAINING_DATA;

struct ClkTrainingResults {
  UINT32  results[8];
};

struct CaClkTrainingResultsBackside {
  UINT32  results[BACKSIDE_RCSIM_FEEDBACK_LEN];
};

struct TrainingResultsDdr5 {
  UINT32  results[DDR5_TRAIN_RESULTS_LEN];
};

struct DwlTrainingResults {
  INT8  Results;
  INT8  ShowResults[DDR5_TRAIN_RESULTS_DWL_PHASE_LEN];
};

struct MdqTrainingResults {
  UINT32  Results[DDR5_TRAIN_RESULTS_MDQ_LEN];
};

//
// Results per each Rank
// Each rank also contains 2 backside copies QnACAx and QnBCAx
//
typedef struct _RESULTS_PER_CH_SUBCH_DIMM_RANK_AB {
  struct baseMargin Rank[MAX_CH][SUB_CH][MAX_DIMM][MAX_RANK_DIMM][COPY_NUM];
} RESULTS_PER_CH_SUBCH_DIMM_RANK_AB;

//
// Results per each Rank for all QnACAx signals
// Each rank also contains 2 backside copies QnACAx and QnBCAx
//
typedef struct _RESULTS_PER_BACKSIDESIGNAL_CH_SUBCH_DIMM_RANK_AB {
  struct baseMargin Backside[BACKSIDE_CA_SIGNALS_TO_SWEEP][MAX_CH][SUB_CH][MAX_DIMM][MAX_RANK_DIMM][COPY_NUM];
} RESULTS_PER_BACKSIDESIGNAL_CH_SUBCH_DIMM_RANK_AB;

//
// Results per each Rank for all QnCSx signals
//
typedef struct _RESULTS_PER_CH_SUBCH_DIMM_RANK {
  struct baseMargin CsSignal[MAX_CH][SUB_CH][MAX_DIMM][MAX_RANK_DIMM];
} RESULTS_PER_CH_SUBCH_DIMM_RANK;

//structure that associates a given setting value with an eye width
typedef struct {
  INT16         SettingValue;
  INT16         EyeWidth;
} SETTINGVALUE_EYEWIDTH_PAIR;

#pragma pack (pop)

//
// Write Leveling Modes
//
typedef enum {
  DISABLE_WRITE_LEVELING_MODE = 0,
  EXTERNAL_WRITE_LEVELING_MODE = 1,
  INTERNAL_WRITE_LEVELING_MODE = 2
  } WriteLevelingMode;

//
// WL Marco definition
//
#define WR_LVL_COARSE_STEP_SIZE_DDR5       32
#define WR_LVL_WRAP_STEP_SIZE_DDR5         96
#define WR_LVL_PI_START_DDR5               64
#define WR_LVL_PI_RANGE_DDR5               448
#define WR_LVL_PI_FULL_RANGE_DDR5          1023
#define T_CWL_ADJ_INITIAL_VALUE           -3
#define TX_DQS_DELAY_RANGE                 640
#define TX_DQS_DELAY_WL_FINE_SWEEP_RANGE   160
#define WR_LVL_FINE_PI_START_EVAL_DDR5     0
#define WR_LVL_FINE_PI_DDRT_0P5tCK         64   // WL_ADJ_end = 0.5 tCK
#define WR_LVL_FINE_PI_DDRT_1P25tCK        160  // WL_ADJ_end = 1.25 tCK
#define WR_LVL_FINE_PI_DDRT_1P75tCK        224  // WL_ADJ_end = 1.75 tCK

//
// WL Internal Write Timing
//
#define WL_ADJ_START_1TCK          0
#define WL_ADJ_END_1TCK            64  //(0.5*128)
#define WL_ADJ_START_2TCK         -96  //-(0.75*128)
#define WL_ADJ_END_2TCK            160 //1.25*128
#define WL_ADJ_START_3TCK         -160 //-(1.25*128)
#define WL_ADJ_END_3TCK            224 //(1.75*128)
#define WL_ADJ_START_4TCK         -224 //-(1.75*128)
#define WL_ADJ_END_4TCK            288 //(2.25*128)
#define INTERNAL_CYCLE_ALM_START  0
#define MAX_INTERNAL_CYCLE        15

#define WR_LVL_INTERNAL_FINE_STEP_SIZE_DDR5            1
#define WR_LVL_INTERNAL_FINE_PI_START_DDR5             0
#define WR_LVL_INTERNAL_FINE_PI_END_DDR5               128
#define WR_LVL_INTERNAL_FINE_PI_EXT_RANGE              16
#define WR_LVL_INTERNAL_FINE_PI_ADDITIONAL             32
#define MIN_CWL_ADJ_DDR5                               -3//DDR5_TODO. To update the value
#define MAX_CWL_ADJ_DDR5                               7 //DDR5_TODO. To update the value


#define PDA_PBA_DQS_DELAY    23

#define CA_CLK_TEST_START                 0
#define CA_CLK_STEP_SIZE_BACKSIDE         1
#define CA_CLK_TEST_START_BACKSIDE        0

#define TX_RETRAIN_ENABLE         1
#define TX_RETRAIN_DISABLE        0
#define DQS2DQ_INITIALIZATION     0
#define DQS2DQ_TRACKING           1
//
// External API for Base DDR5 Core Library
//

/**

  Perform Receive Enable Training for Ddr5

  @param[in] Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
ReceiveEnableDdr5 (
  IN PSYSHOST  Host
  );

/**

  Executes early cs/clock training

  @param[in] Host - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
EarlyCsClk (
  IN PSYSHOST  Host
  );

/**

Disable DRAM External Write Timing.

@param [IN] Host    - Pointer to sysHost
@param [IN] Socket  - Socket number
@param [IN] Ch      - Channel number
@param [IN] Dimm    - DIMM number
@param [IN] Rank    - Rank number

retval : none
**/
VOID
DisableExternalWriteLeveling (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    Rank
  );

/**

  Train BCOM signals relative to clock

  @param[in] Host  - Pointer to sysHost

  @retval Status

**/
UINT32
BcomTraining (
  IN PSYSHOST  Host
  );

/**

  Take DB in or out of reset.

  @param[in] Socket         - Socket number
  @param[in] ChannelBitMask - Bitmask of channels to reset
  @param[in] IsAssert       - Assert/Deassert BRST_n for DB

  @retval  MRC_STATUS_SUCCESS

**/
MRC_STATUS
EFIAPI
SetDbReset (
  IN UINT8     Socket,
  IN UINT32    ChannelBitMask,
  IN BOOLEAN   IsAssert
  );

/**

  Determine if the given DQ bit number is valid for ECC mode on current socket for DDR5

  @param[in]  Host    Pointer to sysHost
  @param[in]  Bit     DQ bit number to check

  @retVal   TRUE      DQ bit number is not valid
  @retVal   FALSE     DQ bit number is valid

**/
BOOLEAN
IsBitNotValidSubChDdr5 (
  IN      PSYSHOST        Host,
  IN      UINT8           Bit
  );
/**

Set PBA Enumerate ID.

@param[in] Host        - Pointer to sysHost

@retval status

**/
UINT32
PbaEnumerateIdDdr5 (
  IN PSYSHOST  Host
  );

/**

  Collects the results of the previous test

  @param[in]      Host      - Pointer to sysHost, the system Host (root) structure
  @param[in]      socket    - Processor socket within the system (0-based)
  @param[in]      chToCheck - channel to check
  @param[in, out] bwSerr    - Test error status; 72 bits per channel for ECC modules

  @retval SUCCESS
  @retval Error Code - Non-zero if any failure was detected

**/
UINT32
CollectTestResultsDdr5 (
  IN PSYSHOST    Host,                     // Pointer to Host structure (outside scope of this spec)
  IN UINT8       Socket,                   // Processor socket within the system (0-based)
  IN UINT32      ChToCheck,                // channel to check
  IN OUT UINT32  BwSerr[MAX_CH][SUB_CH][3] // Test error status; 72 bits per channel for ECC modules
  );

/**

  Collects the results of the previous test

  @param[in]      Host      - Pointer to sysHost, the system Host (root) structure
  @param[in]      socket    - Processor socket within the system (0-based)
  @param[in]      chToCheck - channel to check
  @param[in, out] bwSerr    - Test error status; 72 bits per channel for ECC modules

  @retval SUCCESS
  @retval Error Code - Non-zero if any failure was detected

**/
UINT32
CollectTestResultsEvenOddDdr5 (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT32      ChToCheck,
  IN OUT UINT32  BwSerr[MAX_CH][SUB_CH][2][3]
  );

/**

  Executes early cs/clock backsdie training

  @param[in] Host - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
EarlyCsClkBackside (
  IN PSYSHOST  Host
  );

/**

  Trains DCA Slew Rate.

  @param Host  - Pointer to sysHost

  @retval Status

**/
UINT32
CaSlewRate (
  IN PSYSHOST  Host
  );

/**

  Trains Tx DQ Slew Rate.

  @param  Host     Pointer to sysHost

  @retval SUCCESS  DQ slew rate training succeeds.
  @retval FAILURE  DQ slew rate training fails.

**/
UINT32
TxDqSlewRate (
  IN PSYSHOST  Host
  );

/**

  Executes early ca clock training

  @param[in] Host - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
EarlyCaClk (
  IN PSYSHOST  Host
  );

/**

  Executes Rcd Dca Dck Duty Cycle Training

  @param[in] Host - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
EFIAPI
RcdDcaDckDutyCycle (
  IN PSYSHOST  Host
  );


/**
  Optimizes DCA TCO by margining DCA eye width at various settings for DCA TCO.

  @param[in]      Host  - Pointer to sysHost structure

  @retval SUCCESS
**/
UINT32
EFIAPI
TrainDcaTco (
  IN     PSYSHOST Host
  );

/**
  Optimizes TCO by margining TX DQ/DQS eye width at various settings for TCO.

  @param[in]      Host  - Pointer to sysHost structure

  @retval SUCCESS
**/
UINT32
TrainTcoDqDqs (
  IN     PSYSHOST Host
  );

/**
  Calculates (or trains) the final settings for ODTLon_WR_Offset and ODTLoff_WR_Offset

  @param[in]      Host  - Pointer to sysHost structure

  @retval SUCCESS
**/
UINT32
WriteOdtLatencyTraining (
  IN     PSYSHOST Host
  );

/**

  Executes early ca clock backside training

  @param[in] Host - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
EarlyCaClkBackside (
  IN PSYSHOST  Host
  );

/**

  Detects DQ swizzling on board routing for NVMDIMM DIMMs

  @param[in] Host  - Pointer to sysHost

  @retval Status

**/
UINT32
DqSwizzleDiscoveryDdr5 (
  IN PSYSHOST Host
  );

/**

  Detect DDR4 configuration

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] socket  - Processor socket within the system (0-based)
  @param[in] ch      - DDR channel number within the processor socket (0-based)
  @param[in] dimm    - DIMM number within a DDR channel (0-based, farthest slot == 0)

  @retval SUCCESS

**/
UINT32
DetectDDR5DIMMConfig (
  IN PSYSHOST    Host,   // Pointer to Host structure (outside scope of this spec)
  IN UINT8       socket, // Processor socket within the system (0-based)
  IN UINT8       ch,     // DDR channel number within the processor socket (0-based)
  IN UINT8       dimm    // DIMM number within a DDR channel (0-based, farthest slot == 0)
  );

/**

  Collects data from SPD for DDR5 LRDIMM

  @param[in] Host   - Point to sysHost
  @param[in] Socket - CPU socket ID
  @param[in] Ch     - Channel to be accessed
  @param[in] Dimm   - DIMM to be accessed

  @retval SUCCESS

**/
UINT32
GetLrdimmSpdDataDdr5 (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT8       Ch,
  IN UINT8       Dimm
  );

/*++

  Collects data from SPD for DDR5

  @retval SUCCESS

--*/
UINT32
GatherSPDDataDDR5 (
  VOID
  );

/**

  Perform Write Leveling training for DDR5.

  @param[in] Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
WriteLevelingDdr5 (
  IN PSYSHOST  Host  // Pointer to Host structure (outside scope of this spec)
  );

/**

 Update the seed value in DRAM

  @param[in] Socket                - Socket to train

  @retval    NA

**/
VOID
UpdateDramSeed (
  IN UINT8                    Socket
  );

/**

  Perform Write DQDQS training for DDR5.

  @param[in] Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
WriteDqDqsPerBitDdr5 (
  IN PSYSHOST Host
  );

/*++

  DDR5 Rx DFE

  @param Host    - Pointer to sysHost

  @retval SUCCESS

--*/
UINT32
EFIAPI
RxDfeDdr5 (
  PSYSHOST  Host
  );

/*++

  DDR5 Tx DFE

  @param Host    - Pointer to sysHost

  @retval SUCCESS

--*/
UINT32
EFIAPI
TxDfeDdr5 (
  PSYSHOST  Host
  );

/*++

  DDR5 Write DB DFE

  @param[in] Host    - Pointer to sysHost

  @retval SUCCESS

--*/
UINT32
EFIAPI
TxDbDfeDdr5 (
  PSYSHOST  Host
  );

/**

  Perform Coarse ReadDqDqs training for BRS DDR5.

  @param[in] Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
CoarseReadDqDqsPerBitBrs (
  IN PSYSHOST Host
  );

/**

  Perform Coarse ReadDqDqs training for DDR5.

  @param[in] Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
CoarseReadDqDqsPerBit (
  IN PSYSHOST Host
  );

/*++

  Turn around timing training.

  @param[in] Host      - Pointer to Host structure

  @retval EFI_SUCCESS

--*/
UINT32
EFIAPI
TurnaroundTrain (
  IN PSYSHOST  Host
  );

/**

  Display Right Edge and Left Edge margin results for DDR5.

  @param[in] Host          - Pointer to sysHost, the system Host (root) structure
  @param[in] socket        - current socket
  @param[in] ch            - channle number
  @param[in] Subch         - Sub Channel number (0-based)
  @param[in] dimm          - dimm number
  @param[in] rank          - rank number (0-based)
  @param[in] resultsBit    - Per bit margin results
  @param[in] group         - Command group

  @retval None

**/
VOID
DisplayREandLEMarginsDdr5 (
  IN PSYSHOST            Host,
  IN UINT8               Socket,
  IN UINT8               Ch,
  IN UINT8               SubCh,
  IN UINT8               Dimm,
  IN UINT8               Rank,
  IN struct bitMarginCh  *ResultsBit,
  IN MRC_GT              Group
  );

/**

  This is a DDR5 wrapper for GetMargins.  GetMargins is only called when DDR5 is installed.

  @param[in] Host           - Pointer to sysHost
  @param[in] Socket         - Processor socket to check
  @param[in] Level          - DDR or LRDIMM Host side, LRDIMM Backside, etc...
  @param[in] Group          - Parameter to be margined
  @param[in] Mode           - Test mode to use
  @param[in] Scope          - Margin granularity
  @param[in] Mask           - Mask of bits to exclude from testing
  @param[in out] MarginData - Pointer to the structure to store the margin results. The structure type varies based on scope.
  @param[in] PatternLength  - RankMarginTest Pattern length
  @param[in] Update         - Update starting margins so the next pass will run faster or not
  @param[in] ChMask         - Bit Mask of channels to not be used
  @param[in] RankMask       - Bit Mask of ranks to not be used
  @param[in] BurstLength    - Number of cachelines to test for each write/read operation
  @param[in] TestType       - Type of test to be run

  @retval None

**/
VOID
GetLegacyRmtMarginsDdr5 (
  IN PSYSHOST        Host,
  IN UINT8           Socket,
  IN MRC_LT          Level,
  IN MRC_GT          Group,
  IN UINT16          Mode,
  IN UINT8           Scope,
  IN struct bitMask  *Mask,
  IN OUT VOID        *MarginData,
  IN UINT32          PatternLength,
  IN UINT8           Update,
  IN UINT8           ChMask,
  IN UINT8           RankMask,
  IN UINT8           BurstLength,
  IN MRC_TT          TestType
  );

/**

  Writes GetMargin results to Debug log.  Results are only displayed when DDR5 is installed.

  @param[in] Host           - Pointer to sysHost
  @param[in] Socket         - Processor socket
  @param[in] ResultsQcs     - Pointer to array which contains QCS test results
  @param[in] ResultsQca     - Pointer to array which contains QCA test results
  @param[in] ResultsRxDqs   - Pointer to array which contains RxDqs test results
  @param[in] ResultsTxDq    - Pointer to array which contains TxDq test results
  @param[in] ResultsRxVref  - Pointer to array which contains RxVref test results
  @param[in] ResultsTxVref  - Pointer to array which contains TxVref test results
  @param[in] ResultsCmd     - Pointer to array which contains Cmd test results
  @param[in] ResultsCtl     - Pointer to array which contains Ctl test results
  @param[in] NumSubCh       - Number of elements in ResultsQcs and ResultsQca

**/
VOID
DisplayLegacyRmtMarginsDdr5 (
  IN PSYSHOST           Host,
  IN UINT8              Socket,
  IN struct rankMargin  *ResultsQcs,
  IN struct rankMargin  *ResultsQca,
  IN struct rankMargin  *ResultsRxDqs,
  IN struct rankMargin  *ResultsTxDq,
  IN struct rankMargin  *ResultsRxVref,
  IN struct rankMargin  *ResultsTxVref,
  IN struct rankMargin  *ResultsCmd,
  IN struct rankMargin  *ResultsCtl,
  IN UINT8              NumSubCh
  );

/**

  Determine if a DDR5 DIMM is present on the given socket.

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number to check

  @retVal   1     DDR5 is present
  @retVal   0     DDR5 is not present

**/
BOOLEAN
IsDdr5Present (
  PSYSHOST        Host,
  UINT32          Socket
  );

/**

  Determine if the given Strobe number is valid for ECC mode on current socket for DDR5.

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Strobe        - Strobe number to check

  @retVal   1     Strobe number is not valid
  @retVal   0     Strobe number is valid

**/
BOOLEAN
IsStrobeNotValidDdr5 (
  PSYSHOST        Host,
  UINT8           Strobe
  );

/**

  Determine if the given Strobe number is valid for ECC mode on current socket for DDR5 full channel.

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Strobe        - Strobe number to check

  @retVal   1     Strobe number is not valid
  @retVal   0     Strobe number is valid

**/
BOOLEAN
EFIAPI
IsStrobeNotValidChDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Strobe
  );

/**

  Determine if the given Dimm and Strobe number is valid for ECC mode on current socket for DDR5.

  @param[in]  DimmNvramStructPtr  Pointer to dimmNvram structure to check
  @param[in]  Strobe              Strobe number to check

  @retVal   TRUE    Strobe number is not valid
  @retVal   FALSE   Strobe number is valid

**/
BOOLEAN
EFIAPI
IsDimmStrobeNotValidDdr5 (
  IN struct dimmNvram *DimmNvramStructPtr,
  IN UINT8            Strobe
  );

/**

  Perform DDR5 LRDIMM Backside receive enable phase and cycle Training (MRE)

  @param[in] Host  - Pointer to sysHost

  @retval status

**/
UINT32
EFIAPI
Ddr5LrdimmMrePhaseCycleTraining (
  IN PSYSHOST  Host
  );

/**

  Perform DDR5 LRDIMM Backside Read Delay Training (RX)

  @param[in] Host  - Pointer to sysHost

  @retval status

**/
UINT32
EFIAPI
Ddr5LrdimmMrdReadDelayTraining (
  IN PSYSHOST  Host
  );

/**

  Determine if the given nibble is valid for ECC mode on current socket for DDR5 full channel.

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Nibble        - Nibble to check

  @retVal   1     Strobe number is not valid
  @retVal   0     Strobe number is valid

**/
BOOLEAN
IsNibbleNotValidDdr5 (
  IN      PSYSHOST        Host,
  IN      UINT8           Nibble
  );

/**

  Determines a bit mask of valid strobe numbers for ECC mode on current socket for DDR5.

  @param[in]      Host          - Pointer to sysHost

  @retVal   UINT32    Bit mask of valid Strobe numbers

**/
UINT32
GetSubChStrobeMaskDdr5 (
  IN      PSYSHOST        Host
  );

/**
  Return the preamble timing for DDR5

  @param[in] Socket         - Socket number
  @param[in] PreambleType   - READ_PREAMBLE, WRITE_PREAMBLE or READ_OR_WRITE_PREAMBLE

  @retval   PREAMBLE_1TCLK, PREAMBLE_2TCLK, PREAMBLE_3TCLK, PREAMBLE_4TCLK
**/

UINT8
GetPreambleStateDdr5 (
  IN      UINT8   Socket,
  IN      UINT8   PreambleType
  );

/**

  Perform Data Buffer to Memory Write Leveling Training (TX)

  @param[in] Host  - Pointer to sysHost

  @retval status

**/
UINT32
EFIAPI
LrdimmBacksideWriteLeveling (
  PSYSHOST  Host
  );

/**

  Perform DDR5 LRDIMM Backside Write Delay Training (TX).

  @param[in] Host  - Pointer to sysHost

  @retval Status - Non-zero if any failure detected

**/
UINT32
EFIAPI
LrdimmBacksideWriteDelay (
  IN PSYSHOST  Host
  );

/**

Writes Multi-Purpose Command

@param Host    - Pointer to sysHost, the system Host (root) structure
@param socket  - Socket Id
@param ch      - Channel number (0-based)
@param dimm    - DIMM number (0-based)
@param rank    - Rank number (0-based)
@param opcode    - MPC command opcode
@param timingmode    - MPC command timingmode. It supports 1N, 2N, 4N.

@retval None

**/
VOID
WriteMPC (
  IN PSYSHOST Host,
  IN UINT8    socket,
  IN UINT8    ch,
  IN UINT8    dimm,
  IN UINT8    SubChannelMask,
  IN UINT8    rank,
  IN UINT16   opcode,
  IN UINT8    timingmode,
  IN BOOLEAN  PassThroughMode  //PassThrough mode or not
  );

/**

  Return the time required for the parity recovery for DDR5 in DCLKs

  @param[in]    Socket        Current socket under test (0-based)
  @param[in]    RatioIndex    Current ratio index (frequency)

  @return       return the parity recovery in DCLKs

**/
UINT32
EFIAPI
GetParityRecoveryDdr5 (
  IN        UINT8           Socket,
  IN        UINT8           RatioIndex
);

/**

  Send MRW Command.

  @param[in] Host     - Pointer to sysHost
  @param[in] socket   - Socket Id
  @param[in] ch       - Channel number (0-based)
  @param[in] dimm     - DIMM number (0-based)
  @param[in] rank     - Rank number (0-based)
  @param[in] address  - address for the MRS write
  @param[in] cw       - CW flag
  @param[in] data     - Data to write

  @retval SUCCESS

**/
UINT32
SendMrw (
  IN PSYSHOST Host,
  IN UINT8    socket,
  IN UINT8    ch,
  IN UINT8    SubChMask,
  IN UINT8    dimm,
  IN UINT8    rank,
  IN UINT16   address,
  IN UINT8    cw,
  IN UINT8    data
  );

/**
 Send MRW Command by PBA.

@param Host      - Pointer to sysHost
@param Socket    - Socket Id
@param Ch        - Channel number (0-based)
@param SubChMask - Sub Channel Mask
@param Dimm      - DIMM number (0-based)
@param Rank      - Rank number (0-based)
@param Buffer    - Data Buffer device number (0-based)
@param Address   - Address for the MRW write
@param Data      - Data to write

@retval NA

**/
VOID
EFIAPI
SendMrwPba (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Buffer,
  IN UINT8    Address,
  IN UINT8    Data
  );

/**

  Send MRW Command to DRAM and update the MRW cache

  @param[in] Socket   - Socket Id
  @param[in] Ch       - Channel number (socket-based)
  @param[in] Dimm     - DIMM number (Channel-based)
  @param[in] Rank     - Rank number (DIMM-based)
  @param[in] Address  - Address of the mode register
  @param[in] Data     - Data to write

  @retval MRC_STATUS_SUCCESS if read is successful
  @retval MRC_STATUS_FAILURE otherwise

**/
MRC_STATUS
EFIAPI
DramModeRegWrite (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Address,
  IN UINT8    Data
  );

/**

  This function sends MRR command and read MR value back from DIMM instead of from software caches,
  and update software caches if the reads is successful.
  The reason it only accesses hardware instead of software caches is to reflect to the current use scenario:
  a. dump dram mode registers
  b. read MR42 for supported DCA training assist mode
  c. issue MRR on MR46 and MR47 and DDRIO will snoop MRR value and record in Tx periodic retraining

  @param[in]  Socket          - Socket Id
  @param[in]  Ch              - Channel number (0-based)
  @param[in]  SubChMask       - Sub-channel bit mask
  @param[in]  Dimm            - DIMM number (0-based)
  @param[in]  Rank            - Rank number (0-based)
  @param[in]  Address         - Mode register Address
  @param[out] Data            - Data array to store the returned result for all dram devices

  @retval MRC_STATUS_SUCCESS if read is successful
  @retval MRC_STATUS_FAILURE otherwise

  **/
MRC_STATUS
EFIAPI
DramModeRegRead (
  IN     UINT8        Socket,
  IN     UINT8        Ch,
  IN     UINT8        SubChMask,
  IN     UINT8        Dimm,
  IN     UINT8        Rank,
  IN     UINT8        Address,
     OUT UINT8        (*Data) [SUB_CH][MAX_STROBE_DDR5 / 2]
  );

/**

Send NOP Command.

@param Host             - Pointer to sysHost
@param Socket           - Socket Id
@param Ch               - Channel number (0-based)
@param SubChMask        - SubCh Mask
@param Dimm             - dimm number (0-based)
@param Rank             - Rank number (0-based)
@param Cw               - CW flag
@param PassThroughMode  - PassThroughMode or not

@retval N/A

**/
UINT32
SendNop (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Cw,
  IN BOOLEAN  PassThroughMode,
  IN BOOLEAN  ExitCATM
  );

/**

  Send MRW PDA Command to DRAM and update the MRW cache

  @param[in] Socket     - Socket Id
  @param[in] Ch         - Channel number (socket-based)
  @param[in] SubChMask  - Mask of subchannel numbers to be updated
  @param[in] Dimm       - DIMM number (Channel-based)
  @param[in] Rank       - Rank number (DIMM-based)
  @param[in] Dram       - Dram device number (0-based)
  @param[in] Address    - Address of the mode register
  @param[in] Data       - Data to write

  @retval MRC_STATUS_SUCCESS if read is successful
  @retval MRC_STATUS_FAILURE otherwise
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
DramModeRegWritePda (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Dram,
  IN UINT8    Address,
  IN UINT8    Data
  );

/**

  Start the JEDEC initialization sequence before training.

  @param[in] Host    - Pointer to sysHost
  @param[in] socket  - Socket to initizlize

  @retval SUCCESS

**/
UINT32
JedecInitPreTraining (
  IN PSYSHOST  Host,
  IN UINT8     socket
  );


/**
This function will setup CKEMask with Selected Chip Selects. This is used for reset and self-refresh exit

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket Id

@retVal None
**/
VOID
SetCSMask (
  IN PSYSHOST Host,
  IN UINT8    socket
  );

/**
  Determine if the given DQ nibble number is valid for ECC mode on current socket for DDR5 CH.

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Nibble        - DQ nibble number to check

  @retVal   TRUE     DQ nibble number is not valid
  @retVal   FALSE    DQ nibble number is valid
**/
BOOLEAN
IsNibbleNotValidChDdr5 (
  IN      PSYSHOST        Host,
  IN      UINT8           Nibble
  );

/**
  Determine the maximum number of DQ bits supported per DDR channel in current socket for DDR5.

  @param[in]      Host          - Pointer to sysHost

  @retVal UINT8   Number of DQ bits per DDR channel
**/
UINT8
GetMaxBitsValidDdr5 (
  IN      PSYSHOST          Host
  );

/**

  Choose QLBD/QLBS As The Feedback

  @param[in]  Socket          - Socket number
  @param[in]  Channel         - Channel number
  @param[in]  Dimm            - Dimm number
  @param[in]  Rank            - Rank number

  @retval: None
**/
VOID
EFIAPI
ChooseQLBAsFeedback (
  IN      UINT8    Socket,
  IN      UINT8    Channel,
  IN      UINT8    Dimm,
  IN      UINT8    Rank
  );

/**

  Disable parity checking on the DDR5 RCD

  @param[in]  Socket:  Processor Socket to check
  @param[in]  Ch:      Channel number

  @retval: None
**/
VOID
EFIAPI
DisableParityCheckingDdr5 (
  IN UINT8     Socket,
  IN UINT8     Ch
  );

/**

  Enables parity checking on the DDR5 RCD

  @param[in]  Socket:  Processor socket to check
  @param[in]  Ch:      Channel number

  @retval: None
**/
VOID
EnableParityCheckingDdr5 (
  IN UINT8     Socket,
  IN UINT8     Ch
  );

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
  IN        UINT8   Socket,
  IN        GSM_CSN Signal,
  IN        UINT16  ArraySize,
  OUT       VOID    *Results    OPTIONAL
  );

/*

  This function Gets or Sets the QCA backside signals

  @param[in]      Host    Pointer to SysHost
  @param[in]      Socket  Current socket under test (0-based)
  @param[in]      Channel Current channel under test (0-based)
  @param[in]      Dimm    Current dimm under test (0-based)
  @param[in]      Rank    Current rank under test (0-based)
  @param[in]      Level   DDR level. LrbufLevel is the only allowed parameter
  @param[in]      Group   Group to be accessed (allowed: CmdAll, Cmd0All, Cmd1All)
  @param[in]      Mode    Mode of operation:
                          read (GSM_READ_ONLY <| GSM_READ_CSR>),
                          write (GSM_WRITE_OFFSET | GSM_FORCE_WRITE),
                          update cache (GSM_UPDATE_CACHE)
  @param[in,out]  Value   Pointer to memory location that contains the value to be written
                          or the location where the read value will be placed
  @param[in,out]  MinVal  Pointer to a memory location that keep track of the minimum value
  @param[in,out]  MaxVal  Pointer to a memory location that keep track of the maximum value

  @retval   EFI_INVALID_PARAMETER     if Level is not supported
  @retval   EFI_INVALID_PARAMETER     if Socket or Channel or Dimm are not within the allowed limits
  @retval   EFI_INVALID_PARAMETER     if any pointer to Value or MinValue or MaxValue is NULL
  @retval   EFI_UNSUPPORTED           for not supported groups
  @retval   EFI_UNSUPPORTED           if Max or Min thresholds are not met

*/
EFI_STATUS
EFIAPI
GetSetCaBacksideDdr5 (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Channel,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value,
  IN OUT  UINT16    *MinVal,
  IN OUT  UINT16    *MaxVal
  );

/*

  This function Gets or Sets the QCS backside signals based on Dimm/Rank

  @param[in]      Host    Pointer to SysHost
  @param[in]      Socket  Current socket under test (0-based)
  @param[in]      Channel Current channel under test (0-based)
  @param[in]      Dimm    Current dimm under test (0-based)
  @param[in]      Rank    Current rank under test (0-based)
  @param[in]      Level   DDR level. LrbufLevel is the only allowed parameter
  @param[in]      Group   Group to be accessed (allowed: CtlAll, Ctl0Grp, Ctl1Grp)
  @param[in]      Mode    Mode of operation:
                          read (GSM_READ_ONLY <| GSM_READ_CSR>),
                          write (GSM_WRITE_OFFSET | GSM_FORCE_WRITE),
                          update cache (GSM_UPDATE_CACHE)
  @param[in,out]  Value   Pointer to memory location that contains the value to be written
                          or the location where the read value will be placed
  @param[in,out]  MinVal  Pointer to a memory location that keep track of the minimum value
  @param[in,out]  MaxVal  Pointer to a memory location that keep track of the maximum value

  @retval   EFI_INVALID_PARAMETER     if Level is not supported
  @retval   EFI_INVALID_PARAMETER     if Socket or Channel or Dimm are not within the allowed limits
  @retval   EFI_INVALID_PARAMETER     if any pointer to Value or MinValue or MaxValue is NULL
  @retval   EFI_UNSUPPORTED           for not supported groups
  @retval   EFI_UNSUPPORTED           if Max or Min thresholds are not met

*/
EFI_STATUS
EFIAPI
GetSetCsBacksideDdr5 (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Channel,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value,
  IN OUT  UINT16    *MinVal,
  IN OUT  UINT16    *MaxVal
  );

/**

  Programs CAS Latency for DDR5 devices

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Ch number

  @retval N/A

**/
VOID
ProgramCasLDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  );

VOID
ProgramMr0Ddr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank
  );

/**

  Add training Delay

  @param[in] Host           - Pointer to sysHost
  @param[in] CteDelay       - CTE Delay
  @param[in] PhysicalDelay  - Delay in real platform

  @retval N/A

**/
VOID
EFIAPI
TrainingDelay (
  IN PSYSHOST Host,
  IN UINT32   CteDelay,
  IN UINT32   PhysicalDelay
  );

/**

  Determine if Backside CMD Margin is enabled for DDR5

  @param  VOID

  @retval TRUE  - Backside CMD Margin is enabled
  @retval FALSE - Backside CMD Margin is not enabled

**/
BOOLEAN
IsDdr5BacksideCmdMarginEnabled (
  VOID
  );

/*
  Write RCD DDR5

  @param[in] Host         - Pointer to sysHost
  @param[in] socket       - Socket Id
  @param[in] ch           - Channel number(0 - based)
  @param[in] dimm         - DIMM number(0 - based)
  @param[in] SubChMask    - SubChannel mask
  @param[in] size         - Number of bytes to be written (Use only BYTE (1) and BLOCK (4) writes)
  @param[in] CwRegPage    - RCD control word page number
  @param[in] CwRegAddress - RCD control word number

  @retval SUCCESS
  @retval FAILURE

*/
UINT32
WriteRcdSmbDdr5 (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket,
  IN      UINT8       Ch,
  IN      UINT8       Dimm,
  IN      UINT8       SubChMask,
  IN      UINT8       Size,
  IN      UINT8       CwRegPage,
  IN      UINT8       CwRegAddress,
  IN      UINT32      *CwData
  );

/**

  Read a DWORD from DDR5 RCD over SMBUS
  Note the protocol only allows accesses to addresses on
  DWORD boundaries. The RCD will mask off the lower 2 address
  bits.
  To read a specific byte, use ReadRcdSmbByteDdr5.

  @param[in]  Host         - Pointer to sysHost
  @param[in]  Socket       - Socket Id
  @param[in]  Ch           - Channel number (0-based)
  @param[in]  Dimm         - DIMM number (0-based)
  @param[in]  SubCh        - SubChannel number (0-based)
  @param[in]  Rank         - Rank number (0-based)
  @param[in]  CwRegPage    - RCD control word page number
  @param[in]  CwRegAddress - RCD control word number
  @param[out] CwData       - Pointer to the data buffer

  @retval SUCCESS
  @retval FAILURE

**/
UINT32
ReadRcdSmbDdr5 (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket,
  IN      UINT8       Ch,
  IN      UINT8       Dimm,
  IN      UINT8       SubCh,
  IN      UINT8       CwRegPage,
  IN      UINT8       CwRegAddress,
  OUT     UINT32      *CwData
  );

/**

  Read a BYTE from DDR5 RCD over SMBUS

  @param[in]  Host         - Pointer to sysHost
  @param[in]  Socket       - Socket Id
  @param[in]  Ch           - Channel number (0-based)
  @param[in]  Dimm         - DIMM number (0-based)
  @param[in]  SubCh        - SubChannel number (0-based)
  @param[in]  Rank         - Rank number (0-based)
  @param[in]  CwRegPage    - RCD control word page number
  @param[in]  CwRegAddress - RCD control word number
  @param[out] CwData       - Pointer to the data buffer

  @retval SUCCESS
  @retval FAILURE

**/
UINT32
ReadRcdSmbByteDdr5 (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket,
  IN      UINT8       Ch,
  IN      UINT8       Dimm,
  IN      UINT8       SubCh,
  IN      UINT8       CwRegPage,
  IN      UINT8       CwRegAddress,
  OUT     UINT8       *CwData
  );

/**
  Store the common DDR5 DIMM configuration in the Host structure

  @param Host    - Pointer to sysHost, the system Host (root) structure struct
  @param socket  - Socket Id
  @param ch      - Channel number (0-based)
  @param dimm    - Current dimm

  @retval None

**/
VOID
GetCommonDDR5DIMMConfig (
  IN OUT PSYSHOST Host,
  IN UINT8        Socket,
  IN UINT8        Ch,
  IN UINT8        Dimm
  );

/*

  Switch to 40b DDR5 channels

  @param[in, out]    Host      Pointer to SysHost structure

  @retval    SUCCESS

*/
UINT32
SwitchTo40bDdr5 (
  IN OUT  PSYSHOST  Host
  );

/*

  Switch to 80b DDR5 channels

  @param[in, out]    Host      Pointer to SysHost structure

  @retval    SUCCESS

*/
UINT32
SwitchTo80bDdr5 (
  IN OUT  PSYSHOST  Host
  );


/**
Enables Write Leveling Mode for DDR5 ranks

@param[in] Host              - Pointer to sysHost
@param[in] Socket            - Socket Id
@param[in] Ch                - Ch number
@param[in] Dimm              - Dimm number (zero-based)
@param[in] Rank              - Rank number (zero-based)
@param[in] Mode              - Write Leveling Mode

@retval N/A

**/
VOID
ConfigureDramForWriteLeveling (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     Mode
  );

/**

  Start backside dram init.

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket to initialize
  @param[in] ChannelMask      - Bitmask of channels to init (1 in a bit position = init that channel number)

  @retval SUCCESS

**/
UINT32
BacksideDramInit (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChannelMask
  );

/**

  Start backside dram init after QCA.

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket to initialize
  @param[in] ChannelMask      - Bitmask of channels to init (1 in a bit position = init that channel number)

  @retval SUCCESS

**/
UINT32
BacksideDramInitAfterQca (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChannelMask
  );

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
  IN UINT8     SubchMask,
  IN UINT8     Rank,
  IN GSM_CSN   Signal,
  IN UINT8     Mode,
  IN INT8      Delay,
  IN BOOLEAN   PassThroughMode
  );

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
  );


/*
  Convert a given Tx DQ VREF percentage value to a linear Tx DQ VREF value
  Where:
  Percentage(%)     Linear
      45              0
      45.5            1
      50              2
      50.5            3
      ...
      96.5            103
      97              104
      97.5            105

  @param[in]      TxVrefPercentage      Tx VREF percentage (45% - 97.5%) represented with a fixed point of 1 decimal (multiplied by 10).
                                        45.0% is represented as 450
                                        45.5% is represented as 455

  @return         Linear value from percentage

*/
INT16
ConvertPercentageToLinear (
  IN        UINT16     TxVrefPercentage
  );

/*
  Convert a given Tx DQ VREF physical value to percentage and then finally to a linear Tx DQ VREF value
  Where:
  Physical    Percentage(%)   Linear
    0x69          45            0
    0x68          45.5          1
    0x67          46            2
    0x66          46.5          3
    ...
    0x02          96.5          103
    0x01          97            104
    0x00          97.5          105

  @param[in]      Physical      Tx VREF percentage (0x00 - 0x69)

  @return         Linear value from physical

*/
INT16
EFIAPI
ConvertPhysicalToLinear (
  IN        INT16   Physical
  );

/*
  Convert a given Tx DQ VREF linear value to physical Tx DQ VREF value
  Where:
  Linear        Physical
    0             0x69
    1             0x68
    2             0x67
    3             0x66
    ...           ...
    103           0x02
    104           0x01
    105           0x00

  @param[in]      Linear      Value between (0-105) that represents an specific VREF DQ percentage

  @return         Physical value from linear

*/
INT16
EFIAPI
ConvertLinearToPhysical (
  IN        INT16   Linear
  );

/**

  Lookup DDR5 rank in the technology table

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket to initizlize
  @param[in] Ch      - Channel on the socket
  @param[in] Dimm    - Dimm on the channel

  @retval Tech table index

**/
UINT8
GetTechIndexDdr5 (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm
  );

/**

  Calculate package rank size (in 64 MB unit) for asymmetric or symmeteric DIMM gived the target package rank number.

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number
  @param[in] Dimm              - Dimm number
  @param[in] Rank              - Package rank number
  @param[out] RankSize         - Pointer to rank size in 64MB unit

  @retval MRC_STATUS_SUCCESS

**/

MRC_STATUS
EFIAPI
CalculateDimmPackageRankSizeDdr5 (
  IN     UINT8   Socket,
  IN     UINT8   Ch,
  IN     UINT8   Dimm,
  IN     UINT8   Rank,
     OUT UINT16  *RankSize
  );

/**
Enables Write Leveling Mode for DDR5 ranks

@param[in] Host              - Pointer to sysHost
@param[in] Socket            - Socket Id
@param[in] Ch                - Ch number
@param[in] Dimm              - Dimm number (zero-based)
@param[in] Rank              - Rank number (zero-based)
@param[in] Mode              - Write Leveling Mode

@retval N/A

**/
VOID
ProgramWlMr2Ddr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     Mode
  );

/**
  Program DQS Interval Control (MR45).

  @param[in]  Host          Pointer to sysHost
  @param[in]  Socket        Socket number
  @param[in]  Channel       Channel number on socket
  @param[in]  TimerRunTime  DQS interval timer run time (see MR45 spec)
**/
VOID
EFIAPI
ProgramDqsIntervalControl (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    TimerRunTime
  );

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
  IN GSM_CSEDGE_CA (*RcdBacksidesignalListEdgeCh) [SUB_CH][MAX_RANK_CH][COPY_NUM][BACKSIDE_CA_SIGNALS_TO_SWEEP]
  );

/*++

  Write DQDQS Pre DFE 2D Centering

  @param[in] Host      - Pointer to Host structure

  @retval EFI_SUCCESS

--*/

UINT32
EFIAPI
WriteDqDqsPreDfe2DCentering (
  IN PSYSHOST  Host
  );

/*++

  Write DQDQS Post DFE 2D Centering

  @param[in] Host      - Pointer to Host structure

  @retval EFI_SUCCESS

--*/

UINT32
EFIAPI
WriteDqDqsPostDfe2DCentering (
  IN PSYSHOST  Host
  );

/**


  DCA DFE Training for DDR5.

  @param Host  - Pointer to sysHost

  @retval Status

**/
UINT32
EFIAPI
DcaDfeDdr5 (
  PSYSHOST  Host
  );

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
  );

/*++

  Read DQDQS Pre DFE 2D Centering

  @param[in] Host      - Pointer to Host structure

  @retval EFI_SUCCESS

--*/

UINT32
EFIAPI
ReadDqDqsPreDfe2DCentering (
  IN PSYSHOST  Host
  );

/*++

  Read DQDQS Post DFE 2D Centering

  @param[in] Host      - Pointer to Host structure

  @retval EFI_SUCCESS

--*/

UINT32
EFIAPI
ReadDqDqsPostDfe2DCentering (
  IN PSYSHOST  Host
  );

/**

  Command  Refresh Mode      Value
  REFab    Normal            tREFI
  REFab    Fine Granularity  tREFI2
  REFsb    Fine Granularity  tREFI / (2 * n)

**/
  typedef enum {
  RefabNormal,
  RefabFineGrain,
  RefsbFineGrain,
  RefreshModeMax
} REFRESH_MODE;

/**

  This function returns the value of tREFI

  @param[in] Host         - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket       - Socket Id
  @param[in] Ch           - Channel number
  @param[in] RefreshMode  - Refresh Mode
                              RefabNormal
                              RefabFineGrain
                              RefsbFineGrain

  @retval UINT32 Value of tREFI

**/
UINT32
GettREFIDdr5 (
  IN PSYSHOST       Host,       // Pointer to the system Host (root) structure
  IN UINT8          Socket,     // Socket Id
  IN UINT8          Ch,         // Channel number
  IN REFRESH_MODE   RefreshMode // Refresh Mode
  );

/**

  Retrieves the Ddr5LrdimmTrainingData structure pointer from the static pointer table.  If
  it has not yet been initialized, resources are allocated and the pointer is set.

  @param[in,out] Ddr5LrdimmTrainingData  - Pointer to the DDR5_LRDIMM_TRAINING_DATA structure

  @retval MRC_STATUS_SUCCESS  DDR5 Training Data structure pointer retrieved
  @retval MRC_STATUS_FAILURE  Cannot retrieve DDR5 Training Data

**/
MRC_STATUS
EFIAPI
GetDdr5LrdimmTrainingData (
  IN OUT DDR5_LRDIMM_TRAINING_DATA  **Ddr5LrdimmTrainingData
  );

/**

Perform Periodic Tx Retraining

@param[in] Host - Pointer to sysHost, the system Host (root) structure

@retval SUCCESS

**/
UINT32
EFIAPI
PeriodicTxRetrain (
  IN PSYSHOST Host
  );

/**

  Configures the data buffer to DRAM tDQS2DQ Tracking Initialization Mode or DRAM
  tDQS2DQ Tracking Mode based on input paramater.

  @param[in] Socket        - Socket number
  @param[in] Channel       - Channel number
  @param[in] Dimm          - DIMM number in Channel
  @param[in] Rank          - Rank number on DIMM
  @param[in] TrackingMode  - 0 for Tracking Initialization
                           - 1 for Tracking Mode

  @retval N/A

**/
VOID
EFIAPI
ConfigureDbDqs2Dq (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    TrackingMode
  );

/**

  Enables Write CRC in the DRAM

  @param[in] Socket   - Socket number
  @param[in] Ch       - Channel number within the socket
  @param[in] Dimm     - DIMM number within the channel
  @param[in] Rank     - Rank number within the DIMM

  @retval N/A

**/
VOID
EFIAPI
Ddr5DramEnableWrCrc (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT8     Rank
  );

/**

  Tx DQ and DQS Duty cycle correction

  @param[in] Host       - Pointer to sysHost
  @param[in] socket     - Socket number

  @retval N/A

**/
VOID
EFIAPI
TxDqDqsDutyCycleCorrection (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  );

  /**

  Prints the DDR5 WL Coarse Results

  @param[in] Host          - Pointer to sysHost
  @param[in] Socket        - Current Socket
  @param[in] Ch            - Channel number
  @param[in] Dimm          - Dimm number
  @param[in] Rank          - Rank number
  @param[in] SubChannel    - subchannel number
  @param[in] MaxStrobe     - Maximum number of strobes

  @retval N/A

  **/
VOID
EFIAPI
DisplayWlCoarseResultsDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     SubChannel,
  IN UINT8     MaxStrobe
  );

/**
  Validate the DRAM data structure backing store

  @retval MRC_STATUS_FAILURE if not enough memory allocated for the buffer
          MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented
          MRC_STATUS_SUCCESS otherwise
**/
MRC_STATUS
EFIAPI
ValidateDramDataStucture (
  VOID
  );

/**

  Prints all DRAM, Register and Buffer CSR data from all DIMMs on the socket.
  Note: this function assumes that the read training has been successfully
  completed in order to read DRAM and Buffer data.

  @param[in] Socket  - Socket Id

  @retval N/A

**/
VOID
EFIAPI
DumpDimmStateDdr5 (
  IN  UINT8     Socket
  );

/**
  Return the cached value of the per-rank MR specified by Address.
  If the MR is in the cache list, return the contents in the caller's Data buffer.
  If the MR is not in the cache list, do not modify the caller's Data buffer.

  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubCh     - Subchannel number
  @param[in]  Dimm      - DIMM number within the channel
  @param[in]  Rank      - Rank number within the DIMM
  @param[in]  Address   - The MRW address
  @param[out] Data      - Pointer to caller's buffer

  @retval MRC_STATUS_SUCCESS cached data is returned
  @retval MRC_STATUS_FAILURE if the mode register is not in the cache list
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
ReadDramModeRegCachePerRank (
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  UINT8       SubCh,
  IN  UINT8       Dimm,
  IN  UINT8       Rank,
  IN  UINT8       Address,
  OUT UINT8       *Data
  );

/**
  Update the MRW cache for a per-rank MRW specified by Address

  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubChMask - Mask of subchannel numbers to be updated
  @param[in]  Dimm      - DIMM number within the channle
  @param[in]  Rank      - Rank number within the DIMM
  @param[in]  Address   - The MRW address
  @param[in]  Data      - New value to write to the cache

  @retval MRC_STATUS_SUCCESS if cache is updated
  @retval MRC_STATUS_FAILURE if the cache is not updated
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
WriteDramModeRegCachePerRank (
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  UINT8       SubChMask,
  IN  UINT8       Dimm,
  IN  UINT8       Rank,
  IN  UINT8       Address,
  IN  UINT8       Data
  );

/**
  Return the cached value of the per-device MR specified by Address.
  If the MR is in the cache list, return the contents in the caller's Data buffer.
  If the MR is not in the cache list, do not modify the caller's Data buffer.
  ASSERT if the MR is not in the cache list.

  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubCh     - Subchannel number
  @param[in]  Dimm      - DIMM number within the channel
  @param[in]  Dram      - Dram device number (sub-channel-based)
  @param[in]  Rank      - Rank number within the DIMM
  @param[in]  Address   - The MRW address
  @param[out] Data      - Pointer to caller's buffer

  @retval MRC_STATUS_SUCCESS if cached data is returned
  @retval MRC_STATUS_FAILURE if the data is not available
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
ReadDramModeRegCachePerDevice (
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  UINT8       SubCh,
  IN  UINT8       Dimm,
  IN  UINT8       Dram,
  IN  UINT8       Rank,
  IN  UINT8       Address,
  OUT UINT8       *Data
  );

/**
  Update the MRW cache for a per-device MRW specified by Address
  ASSERT if the MR is not in the cache list

  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubChMask - Mask of subchannel numbers to be updated
  @param[in]  Dimm      - DIMM number within the channle
  @param[in]  Dram      - Dram device number (sub-channel-based)
  @param[in]  Rank      - Rank number within the DIMM
  @param[in]  Address   - The MRW address
  @param[in]  Data      - New value to write to the cache

  @retval MRC_STATUS_SUCCESS if cache is updated
  @retval MRC_STATUS_FAILURE if the cache is not updated
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
WriteDramModeRegCachePerDevice (
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  UINT8       SubChMask,
  IN  UINT8       Dimm,
  IN  UINT8       Dram,
  IN  UINT8       Rank,
  IN  UINT8       Address,
  IN  UINT8       Data
  );

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
  );

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
  );

/**

  Get the CS signal based on the Dimm, Rank and sub-channel
  @param[in]       Dimm                   Dimm slot index (0-based)
  @param[in]       Rank                   Rank index (0-based)
  @param[in]       SubChannel             Current sub-channel under test (0-based)

  @return   CS0_A_N, CS1_A_N, CS2_A_N, CS3_A_N, CS0_B_N, CS1_B_N, CS2_B_N, CS3_B_N

**/
GSM_CSN
GetRcdCsSignal (
  IN UINT8 Dimm,
  IN UINT8 Rank,
  IN UINT8 SubChannelIndex
  );

/**
  Encode an IBT impedance to the corresponding register value.

  The value returned is meant to be written to one of the fields of the RCD's
  IBT Global Control Word (RW10).

  @param[in]  ImpedanceInOhms       Impedance value to be encoded.
  @param[out] EncodedValue          Encoded IBT impedance value per RCD spec.

  @retval     MRC_STATUS_SUCCESS    Encoded successfully.
  @retval     MRC_STATUS_NOT_FOUND  Could not find encoding, default returned.
**/
MRC_STATUS
EFIAPI
EncodeIbtValueToRegister (
  IN  UINT16  ImpedanceInOhms,
  OUT UINT8   *EncodedValue
  );

/**

  Starts the JEDEC initialization sequence for DDR5

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket number
  @param[in] ChBitMask  - Bit Mask of Channels to operate on

  @retval MRC_STATUS_SUCCESS if Jedec Init sequences is successful
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if function is not implemented
  @retval MRC_STATUS_FAILURE otherwise

**/
MRC_STATUS
EFIAPI
JedecInitSequenceDdr5 (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT32    ChBitMask
  );

/**

  Executes the JEDEC initialization sequence for each channel on the provided socket number

  @param Host      - Pointer to sysHost
  @param Socket    - Socket Id
  @param ChBitMask - Bit Mask of Channels Present on the Socket

  @retval MRC_STATUS_SUCCESS if sequence is successful
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if function is not implemented
  @retval MRC_STATUS_FAILURE otherwise

**/
MRC_STATUS
EFIAPI
JedecInitDdrAllDdr5 (
  PSYSHOST      Host,
  UINT8         Socket,
  UINT32        ChBitMask
  );

/**

  Executes the JEDEC initialization sequence for the given channel

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket number
  @param[in] Ch       - Channel number within the socket

  @retval MRC_STATUS_SUCCESS if sequence is successful
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if function is not implemented
  @retval MRC_STATUS_FAILURE otherwise

**/
MRC_STATUS
EFIAPI
JedecInitDdrChannelDdr5 (
  IN  PSYSHOST    Host,
  IN  UINT8       Socket,
  IN  UINT8       Ch
  );

/**
  Displays the training results for all parameters

  @param[in] Host:         Pointer to sysHost

  @retval N/A
**/
UINT32
EFIAPI
DisplayTrainResultsDdr5 (
  IN PSYSHOST  Host
  );


/**

  Program all DRAM in a channel with the refresh mode

  @param[in] Socket       - Socket Id
  @param[in] Ch           - Channel number
  @param[in] RefreshMode  - Refresh Mode
                              RefabNormal
                              RefabFineGrain
                              RefsbFineGrain

  @retval MRC_STATUS_SUCCSS if refresh mode is programmed
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
SetDramRefreshMode (
  IN  UINT8           Socket,
  IN  UINT8           Ch,
  IN  REFRESH_MODE    RefreshMode
  );

/**

  Return the DDR5 Refresh Mode policy setting

  @retval Refresh Mode - RefabNormal
                       - RefabFineGrain
                       - RefsbFineGrain

**/
REFRESH_MODE
EFIAPI
GetRefreshModePolicy (
  VOID
  );

/**

  Get the limits for the group or level requested. The adjustment of the group is done
  by the RCD, DB buffer or DRAM.

  @param Level     - Level to program (DDR, VMSE, Buffer)
  @param Group     - Parameter to program
  @param MinLimit  - Minimum delay value allowed
  @param MaxLimit  - Maximum absolute value allowed
  @param Delay     - Time in usec required for new setting to take effect

  @retval MRC_STATUS

**/
MRC_STATUS
GetDimmParamGroupLimits (
  IN  MRC_LT    Level,
  IN  MRC_GT    Group,
  OUT UINT16    *MinLimit,
  OUT UINT16    *MaxLimit,
  OUT UINT16    *Delay
  );

/**
  Validate the RCD data structure backing store

  Displays the DDR5 training results

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure struct
  @param[in] socket  - Socket Id
  @param[in] group   - MRC_GT group

  @retval None

**/
VOID
EFIAPI
DisplayResultsDdr5 (
  IN PSYSHOST Host,
  IN UINT8 socket,
  IN  MRC_GT group
  );

/**

  Program all DRAM in a channel with the refresh mode
  @retval MRC_STATUS_FAILURE if not enough memory allocated for the buffer
          MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented
          MRC_STATUS_SUCCESS otherwise
**/
MRC_STATUS
EFIAPI
ValidateRcdDataStucture (
  VOID
  );

/**

  Send MRW Command to RCD and update the RCD CW cache

  @param[in] Socket     - Socket Id
  @param[in] Ch         - Channel number (socket-based)
  @param[in] SubChMask  - Mask of subchannel numbers to be updated
  @param[in] Dimm       - DIMM number within the channel
  @param[in] Rank       - Rank number within the DIMM
  @param[in] Address    - Address of the mode register
  @param[in] Data       - Data to write

  @retval MRC_STATUS_SUCCESS if read is successful
  @retval MRC_STATUS_FAILURE otherwise

**/
MRC_STATUS
EFIAPI
RcdControlWordWrite (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Address,
  IN UINT8    Data
  );

/**
  Write an entry in the RCD CW cache

  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubChMask - Subchannel mask within the channel
  @param[in]  Dimm      - DIMM number within the channel
  @param[in]  Page      - The CW page number
  @param[in]  Address   - The MRW address
  @param[in]  Data      - New value to write to the cache

  @retval MRC_STATUS_SUCCESS if cache is updated
  @retval MRC_STATUS_FAILURE if the cache is not updated
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented
**/
MRC_STATUS
EFIAPI
WriteRcdCwCache (
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  UINT8       SubChMask,
  IN  UINT8       Dimm,
  IN  UINT8       Page,
  IN  UINT8       Address,
  IN  UINT8       Data
  );

/**

  Send MRW Command to DB and update the DB CW cache

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket Id
  @param[in] Ch         - Channel number (socket-based)
  @param[in] SubChMask  - Mask of subchannel numbers to be updated
  @param[in] Dimm       - DIMM number within the channel
  @param[in] Rank       - Rank number within the DIMM
  @param[in] Address    - Address of the mode register
  @param[in] Data       - Data to write

  @retval MRC_STATUS_SUCCESS if read is successful
  @retval MRC_STATUS_FAILURE otherwise

**/
MRC_STATUS
EFIAPI
DbControlWordWrite (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Address,
  IN UINT8    Data
  );

/**
  Write an entry in the DB control word cache

  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubCh     - SubCh number
  @param[in]  Dimm      - DIMM number within the channel
  @param[in]  Strobe    - Data buffer number per sub channel
  @param[in]  Page      - Page number
  @param[in]  Address   - The MRW address
  @param[in]  Data      - New value to write to the cache

  @retval MRC_STATUS_SUCCESS if cache is updated
  @retval MRC_STATUS_FAILURE if the cache is not updated
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented
**/
MRC_STATUS
EFIAPI
WriteDbCwCache (
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  UINT8       SubCh,
  IN  UINT8       Dimm,
  IN  UINT8       Strobe,
  IN  UINT8       Page,
  IN  UINT8       Address,
  IN  UINT8       Data
  );


/**
  Restore cached DB control words to the DB

  @param[in]  Socket     - Socket number
  @param[in]  Ch         - Channel number within the socket
  @param[in]  Dimm       - DIMM number within the channel

  @retval   MRC_STATUS_SUCCESS if registers are restored
  @retval   MRC_STATUS_FAILURE otherwise
  @retval   MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
RestoreLrbufferControlWordsFromCache (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm
  );

  /**
  Return an entry from the RCD CW cache

  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubCh     - Subchannel within the channel
  @param[in]  Dimm      - DIMM number within the channel
  @param[in]  Page      - The CW page number
  @param[in]  Address   - The MRW address
  @param[out] Data      - Poitner to caller's data buffer

  @retval MRC_STATUS_SUCCESS if cache entry is returned
  @retval MRC_STATUS_FAILURE if the cache entry is not supported
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented
**/
MRC_STATUS
EFIAPI
ReadRcdCwCache (
  IN      UINT8       Socket,
  IN      UINT8       Ch,
  IN      UINT8       SubCh,
  IN      UINT8       Dimm,
  IN      UINT8       Page,
  IN      UINT8       Address,
     OUT  UINT8       *Data
  );


/**
  Return an entry from the DB control word cache

  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number (80-bit channel)
  @param[in]  SubCh     - Subchannel within the channel
  @param[in]  Dimm      - DIMM number within the channel
  @param[in]  Strobe    - Data buffer number per sub channel
  @param[in]  Page      - The CW page number
  @param[in]  Address   - The MRW address
  @param[out] Data      - Poitner to caller's data buffer

  @retval MRC_STATUS_SUCCESS if cache entry is returned
  @retval MRC_STATUS_FAILURE if the cache entry is not supported
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented
**/
MRC_STATUS
EFIAPI
ReadDbCwCache (
  IN      UINT8       Socket,
  IN      UINT8       Ch,
  IN      UINT8       SubCh,
  IN      UINT8       Dimm,
  IN      UINT8       Strobe,
  IN      UINT8       Page,
  IN      UINT8       Address,
     OUT  UINT8       *Data
  );

/**
  Perform DDR5 Rambus D0 errata

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket number

  @retval  TRUE  - Errata is applied
           FALSE - Errata is not applied
**/
BOOLEAN
EFIAPI
Ddr5RambusD0Errata (
  IN  PSYSHOST Host,
  IN  UINT8    Socket
  );

/**
  Multi-use function to either get or set RCD command DFE vref based on the provided siganl number

  @param[in] Host        - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket      - Socket number
  @param[in] Channel     - Channel number
  @param[in] SubChannel  - SubChannel number
  @param[in] Dimm        - DIMM number within a DDR channel (0-based, farthest slot == 0)
  @param[in] Signal      - Enumerated signal name
  @param[in] Mode        - GSM_READ_CSR - Read the data from hardware and not cache
                         -  GSM_READ_ONLY - Do not write
                         -  GSM_WRITE_OFFSET - Write offset and not value
                         -  GSM_FORCE_WRITE - Force the write
  @param[in, out ] Value - Value to program

  @retval Status

**/
MRC_STATUS
EFIAPI
GetSetDcaDfeVref (
  IN     PSYSHOST Host,
  IN     UINT8    Socket,
  IN     UINT8    Channel,
  IN     UINT8    SubChannel,
  IN     UINT8    Dimm,
  IN     GSM_CSN  Signal,
  IN     UINT8    Mode,
  IN OUT INT16    *Value
  );

/**

  Get set RCD DCS vref based on the provided signal number

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number
  @param[in] Dimm             - DIMM number
  @param[in] SubChannel       - SubChannel number (0-based)
  @param[in] Signal           - Enumerated signal name
  @param[in] Mode             - GSM_READ_CSR - Read the data from hardware and not cache
                              - GSM_READ_ONLY - Do not write
                              - GSM_WRITE_OFFSET - Write offset and not value
                              - GSM_FORCE_WRITE - Force the write
  @param[in, out] Value      - Value to set or get

  @retval Status
**/
MRC_STATUS
EFIAPI
GetSetRcdDcsVref (
  IN     PSYSHOST Host,
  IN     UINT8    Socket,
  IN     UINT8    Channel,
  IN     UINT8    SubChannel,
  IN     UINT8    Dimm,
  IN     GSM_CSN  Signal,
  IN     UINT8    Mode,
  IN OUT INT16   *Value
  );

/**

  Get/Set DDR5 QCSx Delay of all copies (A and B)
  QCSx is the CSx Signal from the RCD to the DRAM
  Programs QCS delay depending on the Raw Card detected
  This function shall program A, both A and B copies if applicable based on the Dimm type.

  @param[in]      Host         - Pointer to SysHost
  @param[in]      Socket       - Socket number
  @param[in]      Channel      - Channel number
  @param[in]      SubChannel   - SubChannel number
  @param[in]      Dimm         - DIMM number within the channel
  @param[in]      Rank         - DIMM Rank number
  @param[in]      Mode         - Access Mode
  @param[in,out]  Delay        - Pointer to Delay value
  @param[in,out]  MinVal       - Pointer to a memory location that keep track of the minimum value of all copies
  @param[in,out]  MaxVal       - Pointer to a memory location that keep track of the maximum value of all copies
                                 The MinVal and MaxVal can be used by the MemSweep lib to keep track the delta of
                                 the initial trained values of both copies. It helps the MemSweep lib to adjust the
                                 sweep range.

  @retval   - MRC_STATUS_SUCCESS if command completed successfully
  @retval   - !MRC_STATUS_SUCCESS otherwise

**/
MRC_STATUS
EFIAPI
GetSetQcsAllCopiesDelay (
  IN      PSYSHOST        Host,
  IN      UINT8           Socket,
  IN      UINT8           Channel,
  IN      UINT8           SubChannel,
  IN      UINT8           Dimm,
  IN      UINT8           Rank,
  IN      UINT8           Mode,
  IN OUT  INT16           *Delay,
  IN OUT  INT16           *MinVal,  OPTIONAL
  IN OUT  INT16           *MaxVal   OPTIONAL
  );

/**

  Get the limits for the group or level requested. The adjustment of the group is done
  by the RCD, DB buffer or DRAM.

  @param Level     - Level to program (DDR, VMSE, Buffer)
  @param Group     - Parameter to program
  @param MinLimit  - Minimum delay value allowed
  @param MaxLimit  - Maximum absolute value allowed
  @param Delay     - Time in usec required for new setting to take effect

  @retval MRC_STATUS

**/
MRC_STATUS
GetDimmParamGroupLimits (
  IN  MRC_LT    Level,
  IN  MRC_GT    Group,
  OUT UINT16    *MinLimit,
  OUT UINT16    *MaxLimit,
  OUT UINT16    *Delay
  );

/*

  This function Gets or Sets the QCA delay signals

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number
  @param[in] SubChannel       - SubChannel number
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number
  @param[in] Signal           - Enumerated signal name
  @param[in] Mode             - GSM_READ_CSR - Read the data from hardware and not cache
                              - GSM_READ_ONLY - Do not write
                              - GSM_WRITE_OFFSET - Write offset and not value
                              - GSM_FORCE_WRITE - Force the write
  @param[in, out ] Value      - Value to program

*/
MRC_STATUS
EFIAPI
GetSetQcaDelay (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Channel,
  IN      UINT8     SubChannel,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      GSM_CSN   Signal,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  );

/*

  This function Gets or Sets the DRAM CA/CS Vref which are the backside QCA/QCS Vref. 

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number
  @param[in] Strobe           - Strobe number inside the channel. Range from 0 to 19, or ALL_STROBES
  @param[in] Group            - Margin Group. Valid value DramCaVref or DramCsVref 
  @param[in] Mode             - GSM_READ_CSR - Read the data from hardware and not cache
                              - GSM_READ_ONLY - Do not write
                              - GSM_WRITE_OFFSET - Write offset and not value
                              - GSM_FORCE_WRITE - Force the write
  @param[in, out ] Value      - Value to program

*/
MRC_STATUS
EFIAPI
GetSetDramCaCsVref (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Channel,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      UINT8     Strobe,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  );

/**

  Checks to see of supplied group is a member of the "Dimm" parameter group which is adjuested
  by the RCD, DB or DRAM.

  @param group - Group item to check

  @retval TRUE  - Is a member of DIMM Group
  @retval FALSE - Is not a member of DIMM Group

**/
BOOLEAN
CheckDimmParamGroupDdr5 (
  MRC_GT  Group
  );

/**
  Multi-use function to either get or set RCD DCA vref based on the provided siganl number

  @param[in] Host        - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket      - Socket number
  @param[in] Channel     - Channel number
  @param[in] SubChannel  - SubChannel number
  @param[in] Dimm        - DIMM number within a DDR channel (0-based, farthest slot == 0)
  @param[in] Signal      - Enumerated signal name
  @param[in] Mode        - GSM_READ_CSR - Read the data from hardware and not cache
                         - GSM_READ_ONLY - Do not write
                         - GSM_WRITE_OFFSET - Write offset and not value
                         - GSM_FORCE_WRITE - Force the write
  @param[in, out] Value  - Value to get or set

  @retval MRC_STATUS_SUCCESS if cache entry is returned
  @retval MRC_STATUS_FAILURE if the cache entry is not supported
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
GetSetRcdDcaVref (
  IN     PSYSHOST Host,
  IN     UINT8    Socket,
  IN     UINT8    Channel,
  IN     UINT8    SubChannel,
  IN     UINT8    Dimm,
  IN     GSM_CSN  Signal,
  IN     UINT8    Mode,
  IN OUT INT16    *Value
  );

/*

  This function Gets or Sets the BCOM signals.
  This function assumes all DB in the same DIMM use the same BCOM Vref.

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number
  @param[in] SubChannel       - SubChannel number
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number
  @param[in] Strobe           - DB index
  @param[in] Mode             - GSM_READ_CSR - Read the data from hardware and not cache
                              - GSM_READ_ONLY - Do not write
                              - GSM_WRITE_OFFSET - Write offset and not value
                              - GSM_FORCE_WRITE - Force the write
  @param[in, out ] Value      - Value to program into DB PG[2]RWFA.
                                When Mode is GSM_WRITE_OFFSET, the offset step size is 2,
                                which is 1% Vdd.

*/
MRC_STATUS
EFIAPI
GetSetBcomVref (
  IN      UINT8     Socket,
  IN      UINT8     Channel,
  IN      UINT8     SubChannel,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      UINT8     Strobe,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  );

/*

  This function Gets or Sets the BCOM  signals

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number
  @param[in] SubChannel       - SubChannel number
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number
  @param[in] Signal           - Enumerated signal name
  @param[in] Mode             - GSM_READ_CSR - Read the data from hardware and not cache
                              - GSM_READ_ONLY - Do not write
                              - GSM_WRITE_OFFSET - Write offset and not value
                              - GSM_FORCE_WRITE - Force the write
  @param[in, out ] Value      - Value to program

*/
MRC_STATUS
EFIAPI
GetSetBcomDelay (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Channel,
  IN      UINT8     SubChannel,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  );

/**

  Displays Advanced Centering data

  @param[in] Host:         Pointer to sysHost
  @param[in] Socket:       Processor socket
  @param[in] Ch            Channel number
  @param[in] Sub-Ch        SubChannel number
  @param[in] Dimm          DIMM number
  @param[in] Rank          Rank number
  @param[in] Group         Parameter to center
  @param[in] Results       Pointer to the structure to store the margin results
  @param[in] Delay         Pointer to the delay value
  @param[in] OldDelay      Pointer to the old delay value
  @param[in] MaxVref       Max vref
  @param[in] StepSize      Setp size
  @param[in] NumPoints     Num points

  @retval N/A

**/
VOID
DisplayAdvancedCenteringResultsDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Subchannel,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN MRC_GT    Group,
  IN VOID      *Results,
  IN INT16     *Delay,
  IN INT16     *OldDelay,
  IN UINT8     MaxVref,
  IN UINT8     StepSize,
  IN UINT8     NumPoints
  );

/**

  Displays And Adjust Vref Centering data

  @param[in] Host              Pointer to sysHost
  @param[in] Socket            Processor socket
  @param[in] Group             Parameter to group
  @param[in] BitResults        Pointer to the bit results
  @param[in] StrobeResults     Pointer to the strobe results
  @param[in] PerBit            Vref training mode perbit

  @retval N/A

**/
VOID
DisplayAndAdjustVrefCenteringResultsDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN MRC_GT    Group,
  IN VOID      *BitResults,
  IN VOID      *StrobeResults,
  IN UINT8     PerBit
);

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
  );

/*++

  GetMargins function support based on New Sweep Lib.

  @param[in]  Host         - Pointer to sysHost
  @param[in]  Group        - Group value
  @param[in]  Scope        - Scope value
  @param[in]  ChunkSelect  - CPGC chuck/UI select mask. value of 1 enables checking the corresponding chunk/UI
  #param[in]  FlagsResults - Flags for the sweep lib to handle the margin results.
  @param[out] Results      - Pointer to the Results

  @retval NA

--*/
VOID
EFIAPI
GetMarginsNewSweep (
  IN  PSYSHOST                Host,
  IN  MRC_GT                  Group,
  IN  UINT8                   Scope,
  IN  UINT32                  ChunkSelect,
  IN  FLAGS_CATEGORY_RESULTS  FlagsResults,
  OUT VOID                    *OldResult
  );

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
  @param[in, out] QcaResults        Pointer to margin results data structure which is indexed as [MAX_CH][SUB_CH][Signal]

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
  );

/**

  DRAM Duty Cycle Adjuster Training entry

  @param  Host     Pointer to sysHost

  @retval SUCCESS  Training succeeds or not enabled
  @retval FAILURE  Training fails

**/
UINT32
DramDutyCycleAdjuster (
  IN PSYSHOST  Host
  );

/**
  Sweep DCS signals and return margin results for EV tests.

  @param[in]      TestType:         Indicates the test type to be configured. It can use to customize the EV margin test setup
  @param[in]      Socket            Processor socket within the system (0-based)
  @param[in]      ChannelMask       One-hot encoding to mask a channel (0: executes if enabled/populated, 1: skip/masked)
  @param[in]      DimmRank          Structure to specify current dimm and rank under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test
  @param[in]      Direction         Specifies the sweep direction: low side only, high side only, or both side
  @param[in]      StepSize          Specifies the StepSize to do the margining
  @param[in]      CountWindow       Count Window Value
  @param[in, out] DcsResults        Pointer to margin results data structure which is indexed as [MAX_CH][SUB_CH]

  @retval MRC_STATUS_SUCCESS
**/
MRC_STATUS
EFIAPI
GetDcsMargins (
  IN     MRC_TT            TestType,
  IN     UINT8             Socket,
  IN     UINT32            ChannelBitMask,
  IN     MRC_RT            DimmRank[MAX_CH],
  IN     MRC_LT            Level,
  IN     MRC_GT            Group,
  IN     UINT8             Direction,
  IN     UINT8             StepSize,
  IN     UINT32            CountWindow,
  IN OUT struct baseMargin *DcsResults
  );

/**
  Sweep QCS signals and return margin results for EV tests.

  @param[in]      TestType:         Indicates the test type to be configured. It can use to customize the EV margin test setup
  @param[in]      Socket            Processor socket within the system (0-based)
  @param[in]      ChannelMask       One-hot encoding to mask a channel (0: executes if enabled/populated, 1: skip/masked)
  @param[in]      DimmRank          Structure to specify current dimm and rank under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test
  @param[in]      Direction         Specifies the sweep direction: low side only, high side only, or both side
  @param[in]      StepSize          Specifies the StepSize to do the margining
  @param[in]      CountWindow       Count Window Value
  @param[in, out] QcsResults        Pointer to margin results data structure which is indexed as [MAX_CH][SUB_CH]

  @retval MRC_STATUS_SUCCESS
**/
MRC_STATUS
EFIAPI
GetQcsMargins (
  IN     MRC_TT            TestType,
  IN     UINT8             Socket,
  IN     UINT32            ChannelBitMask,
  IN     MRC_RT            DimmRank[MAX_CH],
  IN     MRC_LT            Level,
  IN     MRC_GT            Group,
  IN     UINT8             Direction,
  IN     UINT8             StepSize,
  IN     UINT32            CountWindow,
  IN OUT struct baseMargin *QcsResults
  );

/**
  Sweep BCOM timing and return margin results for EV tests.

  @param[in]      TestType:         Indicates the test type to be configured. It can use to customize the EV margin test setup
  @param[in]      Socket            Processor socket within the system (0-based)
  @param[in]      ChannelMask       One-hot encoding to mask a channel (0: executes if enabled/populated, 1: skip/masked)
  @param[in]      DimmRank          Structure to specify current dimm and rank under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test
  @param[in]      Direction         Specifies the sweep direction: low side only, high side only, or both side
  @param[in]      StepSize          Specifies the StepSize to do the margining
  @param[in]      CountWindow       Count Window Value
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
  );

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
  );

/**

  Do pushing in/out for TxDq/TxDqs

  @param[in] Socket     - Current socket
  @param[in] Direction  - DENORMALIZE (-1) or RENORMALIZE (1)

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
EFIAPI
WrLvlPushOutDdr5 (
  IN UINT8 Socket,
  IN INT8 Direction
  );

/**

  Normalize CMD training.
  Normalize CLK/CA/CS/TxDq/TxDqs to reduce turnaround latency.

  @param[in] Host   - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
EFIAPI
NormalizeCmd (
  IN PSYSHOST  Host
  );

/**

  DeNormalize CMD.
  Function should be invoked after CmdNormalize training step finishes. It has 2 directions.

  By DENORMALIZE direction, all signals will be pushed out back to the original value, same
  as the status before CmdNormalize training step.

  By RENORMALIZE direction, all signals will be pushed in again after DENORMALIZE operation.

  @param[in] Socket     - Current Socket
  @param[in] Direction  - DENORMALIZE (-1) or RENORMALIZE (1)

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
EFIAPI
DeNormalizeCmd (
  IN UINT8     Socket,
  IN INT8      Direction
  );

/**

  Set burst chop

  @param[in] Socket          - Socket Id
  @param[in] Ch              - Channel number (0-based)
  @param[in] Dimm            - DIMM number (0-based)
  @param[in] Rank            - Rank number (0-based)

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
EFIAPI
SetBurstChopDdr5 (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    Rank
  );

/**

  This function returns the value of tSRX2SRX.
  The minimum number of valid input clock cycles between SRX command to release
  QCS and SRX to take DRAM out of Self Refresh (DDR5 JEDEC RCD Specification)

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tSRX2SRX in clocks

**/
UINT32
EFIAPI
GettCKSRX2SRX (
  IN UINT8    Socket
  );

/**

  This function returns the value of tCKACT.
  Maximum time from DCS_n transition HIGH to DCK start
  (DDR5 JEDEC RCD Specification)

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tCKACT in clocks

**/
UINT32
EFIAPI
GettCKACT (
  IN UINT8    Socket
  );

/**

  This function returns the value of tCSSR.
  Minimum DCS[1:0]_n LOW time (after SRE) when entering DCK stop
  (DDR5 JEDEC RCD Specification)

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tCSSR in clocks

**/
UINT32
EFIAPI
GettCSSR (
  IN UINT8    Socket
  );

/**
  Validate the DB data structure backing store

  @retval MRC_STATUS_FAILURE if not enough memory allocated for the buffer
          MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented
          MRC_STATUS_SUCCESS otherwise
**/
MRC_STATUS
EFIAPI
ValidateDbDataStucture (
  VOID
  );

/**

  Get set Data Buffer DFE vref based on the provided group number of a give DQ pin

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number
  @param[in] Dimm             - DIMM number
  @param[in] Group            - Enumerated group name
  @param[in] Mode             - GSM_READ_CSR - Read the data from hardware and not cache
                              - GSM_READ_ONLY - Do not write
                              - GSM_WRITE_OFFSET - Write offset and not value
                              - GSM_FORCE_WRITE - Force the write
  @param[in, out ] Value      - Value to program

  @retval Status
**/
MRC_STATUS
GetSetDbDfeVrefDqn (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT8     Channel,
  IN    UINT8     Dimm,
  IN    UINT8     Strobe,
  IN    MRC_GT    Group,
  IN    UINT8     Mode,
  IN OUT INT16    *Value
  );


/**
  Get Host frequency
  @param[in]  Socket        Processor Socket

  @retval Encoded Host frequency
**/
UINT8
EFIAPI
GetHostFrequency (
  IN UINT8    Socket
  );

/**
  Determine if MCR is present in the given Socket, Ch

  @param[in]      Socket        Processor Socket
  @param[in]      Channel       Channel

  @retval TRUE if MCR Present
  @retval FALSE if MCR is not present
**/

BOOLEAN
EFIAPI
IsMcrPresentChannel (
  IN UINT8    Socket,
  IN UINT8    Channel
  );

/**
  Determine if a MCR is present in the given Socket

  @param[in]      Socket        Processor Socket

  @retval TRUE if MCR Present
  @retval FALSE if MCR is not present
**/
BOOLEAN
EFIAPI
IsMcrPresentSocket (
  IN UINT8    Socket
  );

#if FixedPcdGetBool(PcdMcrSupport)
/**
  Get Host Rank ID (DCS0/1 CID2) mapping for MCR DIMM

  @param[in]      Socket        Processor Socket
  @param[in]      Ch            Channel
  @param[in]      Dimm          DIMM ID
  @param[in]      Rank          Rank ID within DIMM
  @param[in]      PsCh          Pseudo-channel

  @retval Host Rank ID
**/

UINT8
EFIAPI
GetHostRankId (
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN UINT8 Dimm,
  IN UINT8 Rank,
  IN UINT8 PsCh
  );

/**

Writes Multi-Purpose Command for DDR5 MCR

@param[in] Socket               - Socket Id
@param[in] Ch                   - Channel number (0-based)
@param[in] Dimm                 - DIMM number (0-based)
@param[in] Subchannelmask       - sub channel mask
@param[in] PsChMask             - Pseudo Ch Mask
@param[in] Rank                 - Rank number (0-based)
@param[in] Opcode               - Opcode for the MPC command
@param[in] Timingmode           - support MPC_1N, MPC_2N, MPC_4N.

@retval None

**/
VOID
EFIAPI
WriteMpcMcr (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    SubChannelMask,
  IN UINT8    PsChMask,
  IN UINT8    Rank,
  IN UINT16   Opcode,
  IN UINT8    Timingmode,
  IN BOOLEAN  PassThroughMode
  );

/**
  Enter RCD CA pass through broadcast mode.

  @param[in]  Socket    Socket ID
  @param[in]  Channel   Channel number
  @param[in]  Dimm      DIMM number
  @param[in]  Rank      Rank number

  @retval N/A
**/

VOID
EFIAPI
EnterRcdCaBcastPassThroughMode (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  );

/**
  Exit RCD CA pass through broadcast mode.

  @param[in]  Socket    Socket ID
  @param[in]  Channel   Channel number
  @param[in]  Dimm      DIMM number
  @param[in]  Rank      Rank number

  @retval N/A
**/

VOID
EFIAPI
ExitRcdCaBcastPassThroughMode (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  );

/**

  Send NOP Command to MCR / DDR5

  @param[in] Socket   - Socket Id
  @param[in] Ch       - Channel number (0-based)
  @param[in] SubChMask - Sub Channel Mask
  @param[in] PsChMask - Pseudo CH Mask
  @param[in] Dimm     - DIMM number (0-based)
  @param[in] Rank     - Rank number (0-based)
  @param[in] Address  - Address for the MRS write
  @param[in] Cw       - CW flag
  @param[in] PassThroughMode - Pass Through Mode
  @param[in] ExitCATM - Exit CATM
  @param[in] NoOfCsClks - Assert CS atleast for this number of DRAM CLKS

  @retval N/A

**/
UINT32
EFIAPI
SendNopMcr (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    PsChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Cw,
  IN BOOLEAN  PassThroughMode,
  IN BOOLEAN  ExitCATM,
  IN UINT8    NoOfCsClks
  );

/**

Send MRW Command to MCR / DDR5

@param[in] Socket   - Socket Id
@param[in] Ch       - Channel number (0-based)
@param[in] SubChMask- Sub Channel mask
@param[in] PsChMask - Pseudo CH Mask
@param[in] Dimm     - DIMM number (0-based)
@param[in] Rank     - Rank number (0-based)
@param[in] Address  - Address for the MRS write
@param[in] Cw       - CW flag
@param[in] Data     - Data to write

@retval N/A

**/
UINT32
EFIAPI
SendMrwMcr (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    PsChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT16   Address,
  IN UINT8    Cw,
  IN UINT8    Data
  );
#endif // FixedPcdGetBool(PcdMcrSupport)
#endif   // _Ddr5CoreLib_h
