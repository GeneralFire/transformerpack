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
#include <Memory/JedecDefinitions.h>
#include <Library/MemoryCoreLib.h>
#include <Memory/CpgcDefinitions.h>
#include <Library/RcMemAllocLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Memory/Ddr5RcwRegs.h>
#include <Library/SimulationServicesLib.h>
#include "Ddr5DfeSettings.h"
#include <Library/SiliconWorkaroundLib.h>

typedef enum {
  EYEHEIGHT = 0, //Eye Height
  EYEWIDTH = 1, //Eye Width
  EYEHEIGHT_EYEWIDTH = 2, //Eye Height and Eye Width
  DCA_NULL = 3, //DCA DFE NULL
  DCA_DFE_VREF = 4, //DCA DFE VREF
  DCA_VREFCA = 5, //DCA VREFCA
} FigureofMerit;

typedef enum {
  REAPEAT_TEST = 0,
  TAP_TEST = 1,
} MARGIN_TEST;

//
// TxDfe Sweep Range
//

#define TX_DFE_TAP1_BEGIN          -40
#define TX_DFE_TAP1_END            10
#define TX_DFE_TAP2_BEGIN          -15
#define TX_DFE_TAP2_END            15
#define TX_DFE_TAP3_BEGIN          -6
#define TX_DFE_TAP3_END             6
#define TX_DFE_TAP4_BEGIN          -9
#define TX_DFE_TAP4_END            9

#define DCA_DFE_TAP1_BEGIN          -50
#define DCA_DFE_TAP1_END            15
#define DCA_DFE_TAP2_BEGIN          -20
#define DCA_DFE_TAP2_END            15
#define DCA_DFE_TAP3_BEGIN          -15
#define DCA_DFE_TAP3_END            15
#define DCA_DFE_TAP4_BEGIN          -15
#define DCA_DFE_TAP4_END            15
#define MAX_NIBBLE_PER_SUBCH        MAX_BITS_DDR5 / (2 * BITS_PER_NIBBLE)
#define DDR5_RCD_DFETM_INNER_AND_OUTER_LOOP_STEP_SIZE_REG_OFFSET  1
#define MR30_READ_DFE_INIT_VALUE    0xFF
#define NIBBLE_MIN_VALUE            127
//
// DB DFE Tap Range
//
#define DB_DFE_TAP1_BEGIN          -50
#define DB_DFE_TAP1_END            15
#define DB_DFE_TAP2_BEGIN          -20
#define DB_DFE_TAP2_END            15
#define DB_DFE_TAP3_BEGIN          -15
#define DB_DFE_TAP3_END            15
#define DB_DFE_TAP4_BEGIN          -15
#define DB_DFE_TAP4_END            15
#define DB_DFE_STEP_SIZE           2
#define DB_TAP1_NUM_OF_STEPS       (DB_DFE_TAP1_END - DB_DFE_TAP1_BEGIN) / DB_DFE_STEP_SIZE
#define DB_DFE_VREF_MARGIN_START   -200
#define DB_DFE_VREF_MARGIN_END     200
#define DB_DFE_VREF_MARGIN_STEP_SIZE 1
#define DB_DFE_VREF_MARGIN_NUM_OF_STEPS  (DB_DFE_VREF_MARGIN_END - DB_DFE_VREF_MARGIN_START) / DB_DFE_VREF_MARGIN_STEP_SIZE

#define DFE_TAP_SWEEP_STEP_SIZE_FIRST     2
#define DFE_TAP_SWEEP_STEP_SIZE_NON_FIRST 1

//
// The maximum and minimum vref in baseMargin
//
#define MAX_BIT_VREF_MARGIN         0x5F
#define MIN_BIT_VREF_MARGIN         -0x5F
#define MARGIN_LIMIT                0x7F
#define DQN_DFE_PIN_SELECTION_ALL   0xFF
#define RW_CONTROL_WORD_WRITE_BROADCASTED_ENABLE 1


typedef struct {
  UINT8 BitErrorResult[DB_DFE_VREF_MARGIN_NUM_OF_STEPS + 1][MAX_BITS / 8];    ///< each UINT8 holds 8 bits of results.
} DB_BIT_ERROR_RESULTS_WR_DDR5;

/*++

  DDR5 DFE Training

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number
  @param[in] Group   - Parameter to center
                       RxVref    0: Center Rx Vref
                       TxVref    1: Center Tx Vref

  @retval SUCCESS

--*/
UINT32
DfeTrainingDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN MRC_GT    Group
  );

/**

Finds the margin for the Vref sweep.

@param[in]  Host                   - Pointer to sysHost
@param[in]  Socket                 - Socket number
@param[in]  ChBitmask              - Bitmask of channels
@param[in]  Dimm                   - Dimm number
@param[in]  VrefNumberOfSteps      - The number of Vref margin steps
@param[in]  ErrorResults           - Test results
@param[out] VrefMarginPerBit       - Pointer to structure to store margin results

@reval N/A

**/
VOID
RecordVrefMargin (
  IN     PSYSHOST             Host,
  IN     UINT8                Socket,
  IN     UINT32               ChBitmask,
  IN     UINT8                Dimm,
  IN     UINT16               VrefNumberOfSteps,
  IN     DB_BIT_ERROR_RESULTS_WR_DDR5   (*ErrorResults) [MAX_CH][SUB_CH],
  OUT    struct bitMargin (*VrefMarginPerBit)
  );

/**

Programs the Data Buffer DFE Training Accelerator Training Mode Control Word.

@param[in] Host            - Pointer to sysHost
@param[in] Socket          - Socket number
@param[in] ChBitmask       - Bitmask of channels to be programmed
@param[in] Dimm            - DIMM number
@param[in] Rank            - Rank number on DIMM
@param[in] Enable          - Enable the TA or not
@param[in] TapToSweep      - which tap to run
@param[in] VrefToSweep     - What vref to be used for margin.

@retval  - MRC_STATUS_SUCCESS - programming successful
         - MRC_STATUS_INVALID_PARAMETER - bad input paramater

**/
MRC_STATUS
ProgramDbDfeTaTrainingMode (
  IN PSYSHOST          Host,
  IN UINT8             Socket,
  IN UINT32            ChBitmask,
  IN UINT8             Dimm,
  IN UINT8             Rank,
  IN DB_DFE_TA_ENABLE  Enable,
  IN UINT8             TapToSweep,
  IN DB_DFE_VREF_TYPE  VrefToSweep
  );

/**

Programs the Data Buffer DFE Training Accelerator to the input sweep paramaters.

@param[in]  Host               - Pointer to sysHost
@param[in]  Socket             - Socket number
@param[in]  TapIndex           - Whicn index tap to run
@param[in]  VrefToSweep        - Type of Vref to sweep
@param[in]  TapStartValue      - Starting outer loop (Tap) value
@param[in]  TapStepSize        - The increment added to the Tap coefficient after every loop
@param[in]  TapNumberOfSteps   - Number of times outer loop (Tap) should execute
@param[in]  VrefStartValue     - Starting inner loop (Vref) value
@param[in]  VrefStepSize       - The increment added to the VrefStartValue after every loop
@param[in]  VrefNumberOfSteps  - Number of times inner loop (Vref) should execute

@retval - MRC_STATUS_SUCCESS - No errors detected
        - MRC_STATUS_INVALID_PARAMETER - Sweep out of range given inputs

**/
MRC_STATUS
ConfigureDbDfeTraining (
  IN     PSYSHOST          Host,
  IN     UINT8             Socket,
  IN     UINT8             TapIndex,
  IN     DB_DFE_VREF_TYPE  VrefToSweep,
  IN     INT8              TapStartValue,
  IN     UINT8             TapStepSize,
  IN     UINT8             TapNumberOfSteps,
  IN     INT16             VrefStartValue,
  IN     UINT8             VrefStepSize,
  IN     UINT16            VrefNumberOfSteps
  );

/**

Programs the Data Buffer DFE Training Accelerator to the input sweep paramaters. Starts the CPGC test
the appropriate number of times (based off of input paramaters). Records errors for each test and for
each outer loop step, stores the coefficient value and corresponding figure of merit in the SweepResults
structure.

@param[in]  Host               - Pointer to sysHost
@param[in]  Socket             - Socket number
@param[in]  TapIndex           - Which index tap to use
@param[in]  VrefToSweep        - Type of Vref to sweep

@retval - MRC_STATUS_SUCCESS - No errors detected
        - MRC_STATUS_INVALID_PARAMETER - Sweep out of range given inputs

**/
MRC_STATUS
SetupDbDfeTraining (
  IN     PSYSHOST          Host,
  IN     UINT8             Socket,
  IN     UINT8             TapIndex,
  IN     DB_DFE_VREF_TYPE  VrefToSweep
  );

/*++

  Program Read Dfe Tap Bias

  @param[in] Host                      - Pointer to sysHost
  @param[in] Socket                    - Socket nubmer
  @param[in] DfeTap                    - Dfe Tap
  @param[in] TapBiasOffsetLoopCount    - Tap Bias Offset Loop Count
  @param[in] DfeTapSweepStepSize       - Dfe Tap Sweep Step Size
  @param[in] DfeTapSweepRange          - Dfe Tap Sweep Range
  @param[in] IterationLoop             - Iteration Loop
  @param[in] TapBiasSweepBeginValue    - Tap Bias Sweep Begin Value
  @param[in] Mode                      - Mode
  @param[in] BestTapBias               - Best Tap Bias

  @retval SUCCESS

--*/
VOID
ProgramReadDfeTapBias (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT8                 DfeTapIndex,
  IN INT8                  TapBiasOffsetLoopCount,
  IN UINT8                 DfeTapSweepStepSize,
  IN UINT8                 DfeTapSweepRange,
  IN UINT8                 IterationLoop,
  IN INT8                  TapBiasSweepBeginValue,
  IN DfeTapBiasProgramMode Mode,
  IN TapBias               (*BestTapBias) [MAX_CH][MAX_RANK_CH][MAX_BITS][MAX_TAP_NUM][MAX_ITERATION_LOOP]
  );


/**

Sets the DB DFE Gain offset adjustment value.  This function can program all
bits to a given gain value (all DQs, all DBs to same value) or will accept a
gain value for a specific bit within the channel (per DQ, per DB).

@param[in] Host            - Pointer to sysHost
@param[in] Socket          - Socket number
@param[in] Channel         - Channel number
@param[in] Dimm            - DIMM number
@param[in] Rank            - Rank number
@param[in] Bit             - Bit within channel to be programmed
@param[in] IsGainRegFormat - TRUE  - Input gain value is in register format (0-7)
FALSE - Input gain value is in offset format (-6,-4,-2,0,2,4,6)
@param[in] GainValue       - Gain offset value to be programmed

@retval - MRC_STATUS_SUCCESS - programming successful
**/
MRC_STATUS
SetDbDfeGain (
  IN PSYSHOST     Host,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN UINT8        Dimm,
  IN UINT8        Rank,
  IN UINT8        Bit,
  IN BOOLEAN      IsGainRegFormat,
  IN INT8         GainValue
  );

/**

Collects the errors from the CPGC after a test has been executed.

@param[in] Host                 - Pointer to sysHost
@param[in] Socket               - Socket number
@param[in] ChBitmask            - Bitmask of channels to collect results for
@param[in] Dimm                 - DIMM number
@param[in] VrefStep             - Current step number of Vref sweep
@param[in] VrefNumberOfSteps    - The number of vref margin steps
@param[in] ErrorResults         - Pointer to structure to store error results

@retval N/A

**/
VOID
CollectDbDfeTestResults (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT32                ChBitmask,
  IN UINT8                 Dimm,
  IN UINT16                VrefStep,
  IN UINT16                VrefNumberOfSteps,
  OUT DB_BIT_ERROR_RESULTS_WR_DDR5 (*ErrorResults) [MAX_CH][SUB_CH]
  );

/**

Sets the Tap Coefficient value for a given Tap number.

@param[in] Host        - Pointer to sysHost
@param[in] Socket      - Socket number
@param[in] Channel     - Channel number
@param[in] SubChannel  - SubChannel number
@param[in] Dimm        - DIMM number
@param[in] Rank        - Rank number on DIMM
@param[in] Bit         - Bit in channel
@param[in] Mode        - Bit-field of read/write modes
@param[in] TapNumber   - Tap number to be programmed
@param[in] CoeffValue  - Coefficient value to be programmed

@retval  - MRC_STATUS_SUCCESS - programming successful
- MRC_STATUS_INVALID_PARAMETER - bad input paramater

**/
MRC_STATUS
SetDbDfeTapCoefficient (
  IN PSYSHOST     Host,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN UINT8        Dimm,
  IN UINT8        Rank,
  IN UINT8        Bit,
  IN UINT8        DfeTapIndex,
  IN INT8         CoeffValue
  );

/*++

  DDR5 Rx DFE

  @param[in] Host    - Pointer to sysHost

  @retval SUCCESS

--*/
UINT32
EFIAPI
RxDfeDdr5 (
  IN PSYSHOST  Host
  )
{
  UINT32       Status;
  UINT8        Socket;
  SYS_SETUP    *Setup;
  UINT8        MaxChDdr;
  UINT8        Ch = 0;
  MRC_GT       TapGroups[] = { RxDfeCoefficientTap1, RxDfeCoefficientTap2, RxDfeCoefficientTap3, RxDfeCoefficientTap4,
                               RxDfeCoefficientTap1PerBit, RxDfeCoefficientTap2PerBit, RxDfeCoefficientTap3PerBit, RxDfeCoefficientTap4PerBit };
  UINT8        TapGroupIndex;

  MaxChDdr = GetMaxChDdr ();
  Socket = Host->var.mem.currentSocket;
  Setup = GetSysSetupPointer ();

  if (!IsMemFlowEnabled (RxDfeDdr5Training)) {
    return SUCCESS;
  }
  //
  // Pattern Init.
  //
  ReadDfeDramPatternInit (Host);

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    //
    // Set mpr_train_ddr_on in host side to filter ACT/PRE.
    //
    SetMprModeFilterActPre (Host, Socket, Ch, FILTER_ACT_PRE_ENABLE);
  }  // Ch loop

  SetCurrentTestType (Socket, ReadDfeTest);
  DoCompDdrioPerSetupOption (Socket, TRAINING_COMP_EVERY_TRAINING);
  Status = DfeTrainingDdr5 (Host, Socket, RxDfe);

  //
  // Display read dfe training results
  //
  for (TapGroupIndex = 0; TapGroupIndex < sizeof (TapGroups) / sizeof (TapGroups[0]); TapGroupIndex++) {
    DisplayResultsDdr5 (Host, Socket, TapGroups[TapGroupIndex]);
  }

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    //
    // Clear mpr_train_ddr_on in host side to enable ACT/PRE.
    //
    SetMprModeFilterActPre (Host, Socket, Ch, FILTER_ACT_PRE_DISABLE);
  }  // Ch loop
  SetCurrentTestType (Socket, 0);
  return Status;
} // RxDfeDdr5

/**
  Returns desired status for the Tx DFE feature

  @param Host          - Pointer to sysHost
  @param Socket        - Socket Id

  @retval TRUE if enabled, FALSE if disabled

**/
BOOLEAN
GetTxDfeEnable (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  //
  // Enable it for all configs.
  //
  return TRUE;
} //GetTxDfeEnable

/*++

  DDR5 Tx DFE

  @param[in] Host    - Pointer to sysHost

  @retval SUCCESS

--*/
UINT32
EFIAPI
TxDfeDdr5 (
  IN PSYSHOST  Host
  )
{
  UINT32       Status;
  UINT8        Socket;
  SYS_SETUP    *Setup;

  Socket = Host->var.mem.currentSocket;
  Setup = GetSysSetupPointer ();

  if ((!((IsMemFlowEnabled (TxDfeDdr5Training)) && (GetTxDfeEnable (Host, Socket)))) ||
      (Host->nvram.mem.socket[Socket].lrDimmPresent == 1)) {
    return SUCCESS;
  }
  SetCurrentTestType (Socket, WriteDfeTest);
  DoCompDdrioPerSetupOption (Socket, TRAINING_COMP_EVERY_TRAINING);
  Status = DfeTrainingDdr5 (Host, Socket, TxDfe);
  SetCurrentTestType (Socket, 0);
  return Status;
} // TxDfeDdr5


/*++

  DDR5 Write DB DFE

  @param[in] Host    - Pointer to sysHost

  @retval SUCCESS

--*/
UINT32
EFIAPI
TxDbDfeDdr5 (
  IN PSYSHOST  Host
  )
{
  UINT32       Status;
  UINT8        Socket;
  SYS_SETUP    *Setup;

  Socket = Host->var.mem.currentSocket;
  Setup = GetSysSetupPointer ();

  if ((!IsMemFlowEnabled (DbDfeDdr5Training)) ||
      (Host->nvram.mem.socket[Socket].lrDimmPresent == 0)) {
    return SUCCESS;
  }
  SetCurrentTestType (Socket, WriteDbDfeTest);
  DoCompDdrioPerSetupOption (Socket, TRAINING_COMP_EVERY_TRAINING);
  Status = DfeTrainingDdr5 (Host, Socket, DbDfe);
  SetCurrentTestType (Socket, DefaultTest);
  return Status;
} // TxDbDfeDdr5

/**
  Initialize eyeSizeScaling for DFE (RxDFE, TxDFE) for RCSIM.

  @param[in]      Host                - Pointer to sysHost
  @param[in]      Socket              - Socket number
  @param[in]      TapIndex            - Tap Inex value
  @param[in]      SettingIndex        - Index into the Tap bias setting array
  @param[in]      IterationLoopIndex  - Iteration Loop Index
  @param[in]      GroupInput          - Group Input Value

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetEyeSizeScalingDfeDdr5Sim (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      UINT8             TapIndex,
  IN      UINT8             SettingIndex,
  IN      UINT8             IterationLoopIndex,
  IN      MRC_GT            GroupInput
  )
{
  MRC_STATUS          Status = MRC_STATUS_SUCCESS;
  return Status;
}


/*++

  DDR5 Tx DFE init.

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket number
  @param[in] DfeTap    - Dfe Tap

  @retval N/A

--*/
VOID
TxDfeInit (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN MRC_GT    DfeTap
  )
{
#if USE_LATEST_DRAM_SPEC
  UINT8                         Dimm;
  UINT8                         Rank;
  UINT8                         Ch;
  UINT8                         MaxChDdr;
  struct dimmNvram              (*DimmNvList) [MAX_DIMM];
  DDR5_MODE_REGISTER_111_STRUCT Ddr5ModeRegister111;

  MaxChDdr = GetMaxChDdr ();
  Ddr5ModeRegister111.Data = 0;
  //
  // Disable the Global DFE Gain and Tap 1, Tap2, Tap3, Tap4.
  //
  Ddr5ModeRegister111.Bits.global_dfe_gain_enable = 1;
  Ddr5ModeRegister111.Bits.global_dfe_tap_1_enable = 1;
  Ddr5ModeRegister111.Bits.global_dfe_tap_2_enable = 1;
  Ddr5ModeRegister111.Bits.global_dfe_tap_3_enable = 1;
  Ddr5ModeRegister111.Bits.global_dfe_tap_4_enable = 1;

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }

        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }

        //
        // Program DFE Gain Bias as 0.
        //
        if ((DfeTap == TxDfeTapAll) || (DfeTap == TxDfeTap1)) {
          //
          // Enable Tap 1.
          //
          Ddr5ModeRegister111.Bits.global_dfe_tap_1_enable = 0;
        }
        if ((DfeTap == TxDfeTapAll) || (DfeTap == TxDfeTap2)) {
          //
          // Enable Tap 2.
          //
          Ddr5ModeRegister111.Bits.global_dfe_tap_2_enable = 0;
        }
        if ((DfeTap == TxDfeTapAll) || (DfeTap == TxDfeTap3)) {
          //
          // Enable Tap 3.
          //
          Ddr5ModeRegister111.Bits.global_dfe_tap_3_enable = 0;
        }
        if ((DfeTap == TxDfeTapAll) || (DfeTap == TxDfeTap4)) {
          //
          // Enable Tap 4.
          //
          Ddr5ModeRegister111.Bits.global_dfe_tap_4_enable = 0;
        }
        //
        // Enable global DFE gain.
        //
        Ddr5ModeRegister111.Bits.global_dfe_gain_enable = 0;
        //
        // Enable Global DFE.
        //
        DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_111_REG, Ddr5ModeRegister111.Data);

      } // Ch
    } // Rank
  } // dimm
#endif //USE_LATEST_DRAM_SPEC
} // TxDfeInit

/*++

  DDR5 DB DFE init.
  Enable the DFE Feature, DFE_VREF, and Error Counters in RWA0
  Enable Global DFE Training Mode, with Training Source set to Monitor Slicer (RWA0[1]=00)

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket number
  @param[in] DfeTap    - Dfe Tap

  @retval N/A

--*/
VOID
DbDfeInit (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN MRC_GT    DfeTap
  )
{
#ifdef USE_LATEST_DB_SPEC
  UINT8                         Dimm;
  UINT8                         Rank = 0;
  UINT8                         Ch;
  UINT8                         MaxChDdr;
  struct dimmNvram              (*DimmNvList) [MAX_DIMM];
  DDR5_DATA_BUFFER_RWA0_STRUCT  Ddr5DataBufferRwa0;
  DDR5_DATA_BUFFER_RWA1_STRUCT  Ddr5DataBufferRwa1;
  DDR5_DATA_BUFFER_RWA2_STRUCT  Ddr5DataBufferRwa2;

  MaxChDdr = GetMaxChDdr ();
  Ddr5DataBufferRwa0.Data = 0;
  Ddr5DataBufferRwa1.Data = 0;
  Ddr5DataBufferRwa2.Data = 0;
  //
  // Enable DFE DQ PIN Selection
  //
  Ddr5DataBufferRwa2.Bits.dqn_dfe_pin_selection = DQN_DFE_PIN_SELECTION_ALL;
  //
  // Enable the DFE Feature, DFE_VREF, and Error Counters in RWA0
  //
  Ddr5DataBufferRwa0.Bits.dfe_feature_enable = 1;
  Ddr5DataBufferRwa0.Bits.dfe_vref_enable = 1;
  Ddr5DataBufferRwa0.Bits.error_counter_enable = 1;
  Ddr5DataBufferRwa0.Bits.rw_control_word_writes_broadcasted = RW_CONTROL_WORD_WRITE_BROADCASTED_ENABLE;

  if ((DfeTap == DbDfeTapAll) || (DfeTap == DbDfeTap1)) {
    //
    // Enable Tap 1.
    //
    Ddr5DataBufferRwa0.Bits.tap_1_enable_bit_for_all_dq = 1;
  }
  if ((DfeTap == DbDfeTapAll) || (DfeTap == DbDfeTap2)) {
    //
    // Enable Tap 2.
    //
    Ddr5DataBufferRwa0.Bits.tap_2_enable_bit_for_all_dq = 1;
  }
  if ((DfeTap == DbDfeTapAll) || (DfeTap == DbDfeTap3)) {
    //
    // Enable Tap 3.
    //
    Ddr5DataBufferRwa0.Bits.tap_3_enable_bit_for_all_dq = 1;
  }
  if ((DfeTap == DbDfeTapAll) || (DfeTap == DbDfeTap4)) {
    //
    // Enable Tap 4.
    //
    Ddr5DataBufferRwa0.Bits.tap_4_enable_bit_for_all_dq = 1;
  }

  //
  // Enable Global DFE Training Mode, with Training Source set to Monitor Slicer (RWA0[1]=00)
  //

  Ddr5DataBufferRwa1.Bits.global_dfe_training_mode_enable = 1;
  Ddr5DataBufferRwa1.Bits.training_source = 0;

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }

      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
        continue;
      }
      //
      // Select all data buffers
      //
      DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);

      //
      // Enable DB DFE.
      //
      DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RWA0_REG, Ddr5DataBufferRwa0.Data);
      //
      // Enable DB DFE Training Mode.
      //
      DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RWA1_REG,  Ddr5DataBufferRwa1.Data);
      //
      // Enable DB DFE DQ PIN Selection
      //
      DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RWA2_REG, Ddr5DataBufferRwa2.Data);
    } // Ch
  } // dimm
#endif
} // DbDfeInit

/*++

  DDR5 DB DFE Clean Up.

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket number
  @param[in] DfeTap    - Dfe Tap

  @retval N/A

--*/
VOID
DbDfeCleanUp (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN MRC_GT    DfeTap
  )
{
#ifdef USE_LATEST_DB_SPEC
  UINT8                         Dimm;
  UINT8                         Rank = 0;
  UINT8                         Ch;
  UINT8                         MaxChDdr;
  struct dimmNvram              (*DimmNvList) [MAX_DIMM];
  DDR5_DATA_BUFFER_RWA0_STRUCT  Ddr5DataBufferRwa0;
  DDR5_DATA_BUFFER_RWA1_STRUCT  Ddr5DataBufferRwa1;
  DDR5_DATA_BUFFER_PG9_RWE0_STRUCT  DfeTaOuterStartE0;

  MaxChDdr = GetMaxChDdr ();
  Ddr5DataBufferRwa0.Data = 0;
  Ddr5DataBufferRwa1.Data = 0;
  //
  // Program dfe_vref_enable = 0
  //
  Ddr5DataBufferRwa0.Bits.dfe_feature_enable = 1;
  Ddr5DataBufferRwa0.Bits.error_counter_enable = 1;
  Ddr5DataBufferRwa0.Bits.rw_control_word_writes_broadcasted = 1;

  if ((DfeTap == DbDfeTapAll) || (DfeTap == DbDfeTap1)) {
    //
    // Enable Tap 1.
    //
    Ddr5DataBufferRwa0.Bits.tap_1_enable_bit_for_all_dq = 1;
  }
  if ((DfeTap == DbDfeTapAll) || (DfeTap == DbDfeTap2)) {
    //
    // Enable Tap 2.
    //
    Ddr5DataBufferRwa0.Bits.tap_2_enable_bit_for_all_dq = 1;
  }
  if ((DfeTap == DbDfeTapAll) || (DfeTap == DbDfeTap3)) {
    //
    // Enable Tap 3.
    //
    Ddr5DataBufferRwa0.Bits.tap_3_enable_bit_for_all_dq = 1;
  }
  if ((DfeTap == DbDfeTapAll) || (DfeTap == DbDfeTap4)) {
    //
    // Enable Tap 4.
    //
    Ddr5DataBufferRwa0.Bits.tap_4_enable_bit_for_all_dq = 1;
  }

  //
  // Disable the DFE training mode. And Clear the training_source field.
  //

  Ddr5DataBufferRwa1.Bits.global_dfe_training_mode_enable = 0;
  Ddr5DataBufferRwa1.Bits.training_source = 0;

  //
  // Disable the training accelerator
  //
  DfeTaOuterStartE0.Data = 0;

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }

      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
        continue;
      }

      if (IsLrdimmPresent (Socket, Ch, Dimm) == 0) {
        continue;
      }

      //
      // Select all data buffers now that programming is done
      //
      DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);

      //
      // Program RWA0
      //
      DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RWA0_REG, Ddr5DataBufferRwa0.Data);
      //
      // Program RWA1
      //
      DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RWA1_REG, Ddr5DataBufferRwa1.Data);

      //
      // Program PG9RWE0, Select DB page 9 for Training Accelerator programming
      //
      DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, DB_TA_PAGE);

      DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG9_RWE0_REG, DfeTaOuterStartE0.Data);
    } // Ch
  } // dimm
#endif
} // DbDfeInit

/*++

  Get Dfe Tap Sweep Parameter.
  @param[in]      Host                             - Pointer to sysHost
  @param[in]      Socket                           - Socket Number
  @param[in]      DfeTap                           - Dfe Tap
  @param[in]      IterationLoop                    - Iteration Loop
  @param[in][out] DfeTapSweepRange                 - Sweep Range
  @param[in][out] DfeTapSweepStepSize              - Dfe Tap Sweep step size
  @param[in][out] TapBiasCenter                    - Tap Bias Center
  @param[in]      DfeTapBiasSweepRangeInitValue    - Dfe Tap Bias Sweep Range Init Value
  @param[in][out] TapBiasSweepBegin                - Tap Bias Sweep Begin

  @retval SUCCESS

--*/
VOID
GetDfeTapSweepParameter (
  IN     PSYSHOST                Host,
  IN     UINT8                   Socket,
  IN     UINT8                   DfeTapIndex,
  IN     UINT8                   IterationLoop,
  IN OUT UINT8                   *DfeTapSweepRange,
  IN OUT UINT8                   *DfeTapSweepStepSize,
  IN OUT INT8                    (*TapBiasCenter) [MAX_CH][MAX_RANK_CH][MAX_BITS][MAX_TAP_NUM][MAX_ITERATION_LOOP],
  IN     DfeTapBiasSweepRange    (*DfeTapBiasSweepRangeInitValue) [MAX_TAP_NUM],
  IN OUT INT8                    *TapBiasSweepBegin
  )
{
  UINT8               NewRange = 0;
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               Rank;
  struct channelNvram (*ChannelNvList) [MAX_CH];
  struct dimmNvram    (*DimmNvList) [MAX_DIMM];
  UINT8               Bit = 0;
  INT8                TapSweepBegin = 0;
  INT8                TapSweepEnd = 0;
  INT8                TapSweepBeginMin = 0;
  INT8                TapSweepEndMax = 0;
  struct ddrRank      (*RankList) [MAX_RANK_DIMM];

  ChannelNvList = GetChannelNvList (Host, Socket);

  if (IterationLoop == FIRST_ITERATION_LOOP) {
    switch (DfeTapIndex) {
      case TAP1_INDEX:
        *DfeTapSweepRange = (*DfeTapBiasSweepRangeInitValue) [0].End - (*DfeTapBiasSweepRangeInitValue) [0].Begin;
        *TapBiasSweepBegin = (*DfeTapBiasSweepRangeInitValue) [0].Begin;
        break;
      case TAP2_INDEX:
        *DfeTapSweepRange = (*DfeTapBiasSweepRangeInitValue) [1].End - (*DfeTapBiasSweepRangeInitValue) [1].Begin;
        *TapBiasSweepBegin = (*DfeTapBiasSweepRangeInitValue) [1].Begin;
        break;
      case TAP3_INDEX:
        *DfeTapSweepRange = (*DfeTapBiasSweepRangeInitValue) [2].End - (*DfeTapBiasSweepRangeInitValue) [2].Begin;
        *TapBiasSweepBegin = (*DfeTapBiasSweepRangeInitValue) [2].Begin;
        break;
      case TAP4_INDEX:
        *DfeTapSweepRange = (*DfeTapBiasSweepRangeInitValue) [3].End - (*DfeTapBiasSweepRangeInitValue) [3].Begin;
        *TapBiasSweepBegin = (*DfeTapBiasSweepRangeInitValue) [3].Begin;
        break;
      default:
        *DfeTapSweepRange = (*DfeTapBiasSweepRangeInitValue) [0].End - (*DfeTapBiasSweepRangeInitValue) [0].Begin;
        *TapBiasSweepBegin = (*DfeTapBiasSweepRangeInitValue) [0].Begin;
        break;
    }
    //
    // FIRST_ITERATION_STEP_SIZE = 2. It's around 10mV.
    //
    *DfeTapSweepStepSize = DFE_TAP_SWEEP_STEP_SIZE_FIRST;
  } else {
    if (IterationLoop >= ITERATION_CHANGE_RANGE) {
      switch (DfeTapIndex) {
        case TAP1_INDEX:
          NewRange = ((*DfeTapBiasSweepRangeInitValue) [0].End - (*DfeTapBiasSweepRangeInitValue) [0].Begin) / (CHANGE_RANGE_AMOUNT);
          break;
        case TAP2_INDEX:
          NewRange = ((*DfeTapBiasSweepRangeInitValue) [1].End - (*DfeTapBiasSweepRangeInitValue) [1].Begin) / (CHANGE_RANGE_AMOUNT);
          break;
        case TAP3_INDEX:
          NewRange = ((*DfeTapBiasSweepRangeInitValue) [2].End - (*DfeTapBiasSweepRangeInitValue) [2].Begin) / (CHANGE_RANGE_AMOUNT);
          break;
        case TAP4_INDEX:
          NewRange = ((*DfeTapBiasSweepRangeInitValue) [3].End - (*DfeTapBiasSweepRangeInitValue) [3].Begin) / (CHANGE_RANGE_AMOUNT);
          break;
        default:
          NewRange = ((*DfeTapBiasSweepRangeInitValue) [0].End - (*DfeTapBiasSweepRangeInitValue) [0].Begin) / (CHANGE_RANGE_AMOUNT);
          break;
      }
      *DfeTapSweepRange = NewRange;
      //
      // It's around 5mV.
      //
      *DfeTapSweepStepSize = DFE_TAP_SWEEP_STEP_SIZE_NON_FIRST;
    }
  }

  for (Ch = 0; Ch < MAX_CH; Ch++) {
    //
    // Skip if channel is disabled
    //
    if ((*ChannelNvList) [Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*DimmNvList) [Dimm].dimmPresent == 0) {
        continue;
      }
      RankList = GetRankNvList (Host, Socket, Ch, Dimm);
      for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
        //
        // Skip if no rank or if it is mapped out
        //
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        TapSweepBeginMin = (((*TapBiasCenter) [Ch][ (*RankList) [Rank].rankIndex][0][DfeTapIndex][IterationLoop]) - ((*DfeTapSweepRange) / 2));
        TapSweepEndMax = (((*TapBiasCenter) [Ch][ (*RankList) [Rank].rankIndex][0][DfeTapIndex][IterationLoop]) + ((*DfeTapSweepRange) / 2));
        for (Bit = 0; Bit < MAX_BITS_DDR5; Bit++) {
          if (IterationLoop == FIRST_ITERATION_LOOP) {
            switch (DfeTapIndex) {
              case TAP1_INDEX:
                (*TapBiasCenter) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTapIndex][IterationLoop] = ((*DfeTapBiasSweepRangeInitValue) [0].End + (*DfeTapBiasSweepRangeInitValue) [0].Begin) / 2;
                break;
              case TAP2_INDEX:
                (*TapBiasCenter) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTapIndex][IterationLoop] = ((*DfeTapBiasSweepRangeInitValue) [1].End + (*DfeTapBiasSweepRangeInitValue) [1].Begin) / 2;
                break;
              case TAP3_INDEX:
                (*TapBiasCenter) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTapIndex][IterationLoop] = ((*DfeTapBiasSweepRangeInitValue) [2].End + (*DfeTapBiasSweepRangeInitValue) [2].Begin) / 2;
                break;
              case TAP4_INDEX:
                (*TapBiasCenter) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTapIndex][IterationLoop] = ((*DfeTapBiasSweepRangeInitValue) [3].End + (*DfeTapBiasSweepRangeInitValue) [3].Begin) / 2;
                break;
              default:
                (*TapBiasCenter) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTapIndex][IterationLoop] = ((*DfeTapBiasSweepRangeInitValue) [0].End + (*DfeTapBiasSweepRangeInitValue) [0].Begin) / 2;
                break;
            }
          } else {
            TapSweepBegin = (((*TapBiasCenter) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTapIndex][IterationLoop]) - ((*DfeTapSweepRange) / 2));
            TapSweepEnd = (((*TapBiasCenter) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTapIndex][IterationLoop]) + ((*DfeTapSweepRange) / 2));
            if (TapSweepBegin < TapSweepBeginMin) {
              TapSweepBeginMin = TapSweepBegin;
            }
            if (TapSweepEnd > TapSweepEndMax) {
              TapSweepEndMax = TapSweepEnd;
            }
            if (TapSweepBeginMin < MIN_TAP_BIAS) {
              TapSweepBeginMin = MIN_TAP_BIAS;
            }
            if (TapSweepEnd > MAX_TAP_BIAS) {
              TapSweepEnd = MAX_TAP_BIAS;
            }
            *TapBiasSweepBegin = TapSweepBeginMin;
          }
        } //bit loop
      } // rank loop
    } // dimm loop
  } // ch loop
  *DfeTapSweepRange = TapSweepEndMax - TapSweepBeginMin;
} // GetDfeTapSweepParameter

/*++

  Program Dfe Gain Bias.

  @param[in] Host                - Pointer to sysHost
  @param[in] Socket              - Socket nubmer
  @param[in] DfeGainBiasValue    - Dfe Gain Bias Value
  @param[in] GroupInput          - Group Input

  @retval SUCCESS

--*/
VOID
ProgramDfeGainBias (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     DfeGainBiasValue,
  IN MRC_GT    GroupInput
  )
{
  UINT8                         Dimm;
  UINT8                         Rank;
  UINT8                         Ch;
  struct dimmNvram              (*DimmNvList) [MAX_DIMM];
  UINT8                         MaxChDdr;
  UINT8                         RankPresent;
  struct rankDevice             (*RankStruct) [MAX_RANK_DIMM];
  struct ddrRank                (*RankList) [MAX_RANK_DIMM];
  DDR5_MODE_REGISTER_112_STRUCT Ddr5ModeRegister112;
  UINT8                         Bit = 0;
  UINT8                         MaxRankOnDimm = 1;
  UINT8                         MBV = GetMaxBitsValid (Host);
#ifdef USE_LATEST_RCD_SPEC
  DDR5_RCD_PG0_RW60_DPAR_AND_DCA_6_0_RECEIVER_DFE_GAIN_OFFSET_STRUCT Ddr5DcaDfeGainOffset;
  Ddr5DcaDfeGainOffset.Data = 0;
#endif //USE_LATEST_RCD_SPEC
  MaxChDdr = GetMaxChDdr ();
  Ddr5ModeRegister112.Data = 0;
  Ddr5ModeRegister112.Bits.dfe_gain_bias = DfeGainBiasValue;
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Program Dfe Gain Bias  %2d\n", DfeGainBiasValue);

  if (GroupInput == RxDfe) {
    ProgramRxGainBias (Socket, DfeGainBiasValue);
  } else if ((GroupInput == TxDfe) || (GroupInput == DcaDfe) || (GroupInput == DbDfe)) {
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (GroupInput == DbDfe) {
        MaxRankOnDimm = 1;
      } else {
        MaxRankOnDimm = Host->var.mem.socket[Socket].maxRankDimm;
      }
      for (Rank = 0; Rank < MaxRankOnDimm; Rank++) {
        //
        // Initialize to no ranks present
        //
        RankPresent = 0;
        for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
          RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);
          if (IsChannelEnabled (Socket, Ch) == FALSE) {
            continue;
          }
          RankList = GetRankNvList (Host, Socket, Ch, Dimm);
          DimmNvList = GetDimmNvList (Host, Socket, Ch);
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
            continue;
          }
          //
          // Program DFE Gain Bias value.
          //
          if (GroupInput == TxDfe) {
            DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_112_REG, Ddr5ModeRegister112.Data);
          } else if (GroupInput == DbDfe) {
            for (Bit = 0; Bit < MBV; Bit++) {
              //
              // Skip if this is an ECC strobe when ECC is disabled
              //
              if (IsBitNotValid (Host, Bit)) {
                continue;
              }

              SetDbDfeGain (Host, Socket, Ch, Dimm, Rank, Bit, FALSE, DfeGainBiasValue);
            }
          } else {
#ifdef USE_LATEST_RCD_SPEC
            if (IsDcpmmPresentDimm (Socket, Ch, Dimm)) {
              ProgramCwvRxBiasGain (Host, Socket, Ch, Dimm, DfeGainBiasValue);
            } else {
              //
              // Translate the value into register bit field value.
              //
              switch (DfeGainBiasValue) {
                case -6:
                  Ddr5DcaDfeGainOffset.Bits.flat_band_dc_gain_adjustment = 7;
                  break;
                case -4:
                  Ddr5DcaDfeGainOffset.Bits.flat_band_dc_gain_adjustment = 6;
                  break;
                case -2:
                  Ddr5DcaDfeGainOffset.Bits.flat_band_dc_gain_adjustment = 5;
                  break;
                case 0:
                  Ddr5DcaDfeGainOffset.Bits.flat_band_dc_gain_adjustment = 0;
                  break;
                case 2:
                  Ddr5DcaDfeGainOffset.Bits.flat_band_dc_gain_adjustment = 3;
                  break;
                case 4:
                  Ddr5DcaDfeGainOffset.Bits.flat_band_dc_gain_adjustment = 2;
                  break;
                case 6:
                  Ddr5DcaDfeGainOffset.Bits.flat_band_dc_gain_adjustment = 1;
                  break;
                default:
                  Ddr5DcaDfeGainOffset.Bits.flat_band_dc_gain_adjustment = 0;
                  break;
              }
              RcdControlWordWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_RCD_PG0_RW60_DPAR_AND_DCA_6_0_RECEIVER_DFE_GAIN_OFFSET_REG, Ddr5DcaDfeGainOffset.Data);//SPR_todo. HSD 1507737594.
            }
#endif //USE_LATEST_RCD_SPEC
          }
        } // Ch loop
      } // Rank loop
    }// Dimm loop
  } else {
    RcDebugPrint (SDBG_ERROR, "\n Error: Group parameter is not valid when calling DfeTrainingDdr5\n");
    ASSERT (FALSE);
    return;
  }
} // ProgramDfeGainBias

/*++

  Check the DQ pin has the same program value across devices or not.

  @param[in] Host                      - Pointer to sysHost
  @param[in] Socket                    - Socket nubmer
  @param[in] Ch                        - Channel nubmer
  @param[in] Dimm                      - Dimm nubmer
  @param[in] Rank                      - Rank nubmer
  @param[in] DfeTapIndex               - Dfe Tap Index
  @param[in] IterationLoop             - Iteration Loop
  @param[in] BestTapBias               - Best Tap Bias
  @param[in] TheSameValue              - Array used to store the flag that indicates the pin has the same value across devices or not

  @retval NA

--*/
VOID
CheckValueForPda (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT8                 Ch,
  IN UINT8                 Dimm,
  IN UINT8                 Rank,
  IN UINT8                 DfeTapIndex,
  IN UINT8                 IterationLoop,
  IN TapBias               (*BestTapBias) [MAX_CH][MAX_RANK_CH][MAX_BITS][MAX_TAP_NUM][MAX_ITERATION_LOOP],
  IN UINT8                 TheSameValue[SUB_CH][MAX_NIBBLE_PER_SUBCH]
  )
{
  UINT8                         Bit;
  UINT8                         MaxBits = MAX_BITS_DDR5 / 2;
  UINT8                         SubCh = 0;
  INT8                          DfeTapBiasValue = 0;
  struct ddrRank      (*RankList) [MAX_RANK_DIMM];

  RankList = GetRankNvList (Host, Socket, Ch, Dimm);
  if (BestTapBias == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_MRC_POINTER, RC_FATAL_ERROR_MRC_POINTER_MINOR_CODE_000);
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Error: Invalid NULL BestTapBias Pointer\n");
    return;
  }
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (IsX4Dimm (Socket, Ch, Dimm) == FALSE) {
      for (Bit = 0; Bit < MAX_BITS_IN_BYTE; Bit++) {
        //
        // Skip if this is an ECC strobe when ECC is disabled
        //
        if (IsBitNotValidSubChDdr5 (Host, Bit)) {
          continue;
        }

        DfeTapBiasValue = (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex];
        if (((*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex] ==
             (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + MAX_BITS_IN_BYTE + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex]) &&
            ((*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex] ==
             (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + 2 * MAX_BITS_IN_BYTE + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex]) &&
            ((*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex] ==
             (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + 3 * MAX_BITS_IN_BYTE + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex])&&
            ((*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex] ==
             (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + 4 * MAX_BITS_IN_BYTE + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex])) {
          TheSameValue[SubCh][Bit] = 1;
        }
      }
    } else {
      for (Bit = 0; Bit < BITS_PER_NIBBLE; Bit++) {
        //
        // Skip if this is an ECC strobe when ECC is disabled
        //
        if (IsBitNotValidSubChDdr5 (Host, Bit)) {
          continue;
        }

        DfeTapBiasValue = (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex];
        if (((*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex] ==
             (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + BITS_PER_NIBBLE + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex]) &&
            ((*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex] ==
             (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + 2 * BITS_PER_NIBBLE + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex]) &&
            ((*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex] ==
             (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + 3 * BITS_PER_NIBBLE + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex])&&
            ((*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex] ==
             (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + 4 * BITS_PER_NIBBLE + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex]) &&
            ((*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex] ==
             (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + 5 * BITS_PER_NIBBLE + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex]) &&
            ((*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex] ==
             (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + 6 * BITS_PER_NIBBLE + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex]) &&
            ((*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex] ==
             (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + 7 * BITS_PER_NIBBLE + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex]) &&
            ((*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex] ==
             (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + 8 * BITS_PER_NIBBLE + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex]) &&
            (((*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex] ==
              (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + 9 * BITS_PER_NIBBLE + (SubCh * (MaxBits / 2)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex]))) {
          TheSameValue[SubCh][Bit] = 1;
        }
      }
    }
  }

  return;
} // CheckValueForPda

/*++

  Program Dfe Tap Bias

  @param[in] Host                      - Pointer to sysHost
  @param[in] Socket                    - Socket nubmer
  @param[in] DfeTap                    - Dfe Tap
  @param[in] TapBiasOffsetLoopCount    - Tap Bias Offset Loop Count
  @param[in] DfeTapSweepStepSize       - Dfe Tap Sweep Step Size
  @param[in] DfeTapSweepRange          - Dfe Tap Sweep Range
  @param[in] IterationLoop             - Iteration Loop
  @param[in] TapBiasSweepBeginValue    - Tap Bias Sweep Begin Value
  @param[in] Mode                      - Mode
  @param[in] BestTapBias               - Best Tap Bias

  @retval SUCCESS

--*/
VOID
ProgramDfeTapBias (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT8                 DfeTapIndex,
  IN INT8                  TapBiasOffsetLoopCount,
  IN UINT8                 DfeTapSweepStepSize,
  IN UINT8                 DfeTapSweepRange,
  IN UINT8                 IterationLoop,
  IN INT8                  TapBiasSweepBeginValue,
  IN DfeTapBiasProgramMode Mode,
  IN TapBias               (*BestTapBias) [MAX_CH][MAX_RANK_CH][MAX_BITS][MAX_TAP_NUM][MAX_ITERATION_LOOP]
  )
{
#ifdef USE_LATEST_RCD_SPEC
  UINT8                         Dimm;
  UINT8                         Rank;
  UINT8                         Ch;
  UINT8                         Bit;
  UINT8                         MaxBits = MAX_BITS_DDR5 / 2;
  struct dimmNvram              (*DimmNvList) [MAX_DIMM];
  UINT8                         MaxChDdr;
  UINT8                         SubCh = 0;
  UINT8                         Nibble = 0;
  UINT8                         EncodedNibble = 0;
  UINT8                         DfeTapBiasValueProgram;
  UINT8                         DfeTapBiasMrAddessProgram;
  UINT8                         RankPresent;
  struct rankDevice             (*RankStruct) [MAX_RANK_DIMM];
  struct ddrRank                (*RankList) [MAX_RANK_DIMM];
  INT8                          DfeTapBiasValue = 0;
  UINT8                         DfeTapMrwAddress = 0;
  DDR5_MODE_REGISTER_129_STRUCT Ddr5ModeRegister129;
  DDR5_MODE_REGISTER_130_STRUCT Ddr5ModeRegister130;
  DDR5_MODE_REGISTER_131_STRUCT Ddr5ModeRegister131;
  DDR5_MODE_REGISTER_132_STRUCT Ddr5ModeRegister132;
  DDR5_RCD_DFETM_OUTER_LOOP_START_VALUE_STRUCT          Ddr5RcdDfetmOuterLoop;
  DDR5_RCD_DFETM_INNER_AND_OUTER_LOOP_STEP_SIZE_STRUCT  Ddr5RcdDfetmInnerOuterLoopStepSize;
  DDR5_RCD_DFETM_OUTER_LOOP_NUMBER_OF_INCREMENTS_STRUCT Ddr5RcdDfetmOuterLoopNumberOfIncrements;
  UINT32                        PrevDebugLevel;
  UINT8                         TheSameValue[2][10];
  UINT8                         BitNew = 0;
  UINT8                         DRAM = 0;
  UINT8                         MBV = GetMaxBitsValid (Host);
  UINT8                         SignBitValue = 0;
  UINT8                         MaxRankOnDimm = 0;



  MaxChDdr = GetMaxChDdr ();
  //
  // Save the debug message level.
  //
  PrevDebugLevel = GetDebugLevel ();
  if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
    SetDebugLevel (SDBG_ERROR | SDBG_MEM_TRAIN);
  }
  if (GetCurrentTestType (Socket) == ReadDfeTest) {
    ProgramReadDfeTapBias (Host, Socket, DfeTapIndex, TapBiasOffsetLoopCount, DfeTapSweepStepSize, DfeTapSweepRange, IterationLoop, TapBiasSweepBeginValue, Mode, BestTapBias);
  } else {
    if (GetCurrentTestType (Socket) == WriteDbDfeTest) {
      MaxRankOnDimm = 1;
    } else {
      MaxRankOnDimm = Host->var.mem.socket[Socket].maxRankDimm;
    }
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      for (Rank = 0; Rank < MaxRankOnDimm; Rank++) {
        //
        // Initialize to no ranks present
        //
        RankPresent = 0;
        for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
          RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);
          if (IsChannelEnabled (Socket, Ch) == FALSE) {
            continue;
          }
          RankList = GetRankNvList (Host, Socket, Ch, Dimm);
          DimmNvList = GetDimmNvList (Host, Socket, Ch);
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
            continue;
          }

          //
          // Program the outer loop
          //
          if (GetCurrentTestType (Socket) == DcaDfeDdr5Test) {
            if (IsDcpmmPresentDimm (Socket, Ch, Dimm) == FALSE) {
              if ((DfeTapIndex == TAP1_INDEX) || (DfeTapIndex == TAP2_INDEX) || (DfeTapIndex == TAP3_INDEX) || (DfeTapIndex == TAP4_INDEX)) {
                //
                // Program the start value
                //
                Ddr5RcdDfetmOuterLoop.Data = 0;
                Ddr5RcdDfetmOuterLoop.Bits.outer_loop_start_value_1_2 = TapBiasSweepBeginValue & DDR5_RCD_DFETM_OUTER_LOOP_START_VALUE_MASK;
                RcdControlWordWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_RCD_DFETM_OUTER_LOOP_START_VALUE_REG, Ddr5RcdDfetmOuterLoop.Data);

                //
                // Program the step size
                //
                Ddr5RcdDfetmInnerOuterLoopStepSize.Data = 0;
                Ddr5RcdDfetmInnerOuterLoopStepSize.Bits.outer_loop_step_size = DfeTapSweepStepSize - DDR5_RCD_DFETM_INNER_AND_OUTER_LOOP_STEP_SIZE_REG_OFFSET;
                RcdControlWordWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_RCD_DFETM_INNER_AND_OUTER_LOOP_STEP_SIZE_REG, Ddr5RcdDfetmInnerOuterLoopStepSize.Data);

                //
                // Program Outer Loop Number of Increments
                //
                Ddr5RcdDfetmOuterLoopNumberOfIncrements.Data = 0;
                if (DfeTapSweepStepSize != 0) {
                  Ddr5RcdDfetmOuterLoopNumberOfIncrements.Bits.outer_loop_number_of_increments = DfeTapSweepRange / DfeTapSweepStepSize;
                } else {
                  RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                    "Error: DfeTapSweepStepSize value 0 is invalid\n");
                  RC_FATAL_ERROR (FALSE, ERR_INVALID_REG_ACCESS, ERR_INVALID_WRITE_REG_SIZE);
                  return;
                }
                RcdControlWordWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_RCD_DFETM_OUTER_LOOP_NUMBER_OF_INCREMENTS_REG, Ddr5RcdDfetmOuterLoopNumberOfIncrements.Data);
              }
            } else {
              Ddr5RcdDfetmOuterLoop.Data = 0;
              Ddr5RcdDfetmOuterLoop.Bits.outer_loop_start_value_1_2 = CWV_DCA_DFE_OUTER_LOOP_START;
              RcdControlWordWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_RCD_DFETM_OUTER_LOOP_START_VALUE_REG, Ddr5RcdDfetmOuterLoop.Data);

              Ddr5RcdDfetmInnerOuterLoopStepSize.Data = 0;
              Ddr5RcdDfetmInnerOuterLoopStepSize.Bits.outer_loop_step_size = CWV_DCA_DFE_OUTER_LOOP_STEP_SIZE;
              RcdControlWordWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_RCD_DFETM_INNER_AND_OUTER_LOOP_STEP_SIZE_REG, Ddr5RcdDfetmInnerOuterLoopStepSize.Data);

              Ddr5RcdDfetmOuterLoopNumberOfIncrements.Data = 0;
              switch (DfeTapIndex) {
                case TAP1_INDEX:
                  Ddr5RcdDfetmOuterLoopNumberOfIncrements.Bits.outer_loop_number_of_increments = CWV_DCA_DFE_OUTER_LOOP_SWEEP_RANGE_TAP1;
                  break;
                case TAP2_INDEX:
                  Ddr5RcdDfetmOuterLoopNumberOfIncrements.Bits.outer_loop_number_of_increments = CWV_DCA_DFE_OUTER_LOOP_SWEEP_RANGE_TAP2;
                  break;
                case TAP3_INDEX:
                  Ddr5RcdDfetmOuterLoopNumberOfIncrements.Bits.outer_loop_number_of_increments = CWV_DCA_DFE_OUTER_LOOP_SWEEP_RANGE_TAP3;
                  break;
                case TAP4_INDEX:
                  Ddr5RcdDfetmOuterLoopNumberOfIncrements.Bits.outer_loop_number_of_increments = CWV_DCA_DFE_OUTER_LOOP_SWEEP_RANGE_TAP4;
                  break;
                default:
                  break;
              } // IsDcpmmPresentDimm
              RcdControlWordWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_RCD_DFETM_OUTER_LOOP_NUMBER_OF_INCREMENTS_REG, Ddr5RcdDfetmOuterLoopNumberOfIncrements.Data);
            }
          } else if ((GetCurrentTestType (Socket) == WriteDbDfeTest) && ((Mode == NORMAL_PROGRAM_MODE))) {
            for (Bit = 0; Bit < MBV; Bit++) {
              //
              // Skip if this is an ECC strobe when ECC is disabled
              //
              if (IsBitNotValid (Host, Bit)) {
                continue;
              }

              if (BestTapBias == NULL) {
                RC_FATAL_ERROR (FALSE, ERR_MRC_POINTER, RC_FATAL_ERROR_MRC_POINTER_MINOR_CODE_002);
                RcDebugPrintWithDevice (SDBG_ERROR, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Error: Invalid NULL BestTapBias Pointer\n");
                return;
              }
              DfeTapBiasValue = (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + (SubCh * (MaxBits)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex];


              SetDbDfeTapCoefficient (Host, Socket, Ch, Dimm, Rank, Bit, DfeTapIndex, DfeTapBiasValue);

            }


          } else if (GetCurrentTestType (Socket) == WriteDfeTest) {
            if (Mode == SWEEP_PROGRAM_MODE) {
              //
              // Program DFE Tap Bias value. To add the x16 support.SPR_todo. HSD 1707143273
              //
              RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Program Dfe DfeTap %d with value %d\n", DfeTapIndex + 1, TapBiasSweepBeginValue + (TapBiasOffsetLoopCount * DfeTapSweepStepSize));
            }

            if (Mode == NORMAL_PROGRAM_MODE) {

              ZeroMem ((UINT8 *) (TheSameValue), sizeof (TheSameValue));

              if (BestTapBias == NULL) {
                RC_FATAL_ERROR (FALSE, ERR_MRC_POINTER, RC_FATAL_ERROR_MRC_POINTER_MINOR_CODE_001);
                RcDebugPrintWithDevice (SDBG_ERROR, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Error: Invalid NULL BestTapBias Pointer\n");
                return;
              }
              if (FeaturePcdGet (PcdDdr5MrSameValueCheckEnable) == TRUE) {
                CheckValueForPda (Host, Socket, Ch, Dimm, Rank, DfeTapIndex, IterationLoop, BestTapBias, TheSameValue);
              }
            }


            for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
              for (Bit = 0; Bit < MaxBits; Bit++) {
                //
                // Skip if this is an ECC strobe when ECC is disabled
                //
                if (IsBitNotValidSubChDdr5 (Host, Bit)) {
                  continue;
                }
                //
                // How about DML and DMU? SPR_todo. To confirm with EV
                //
                if (Mode == SWEEP_PROGRAM_MODE) {
                  DfeTapBiasValue = TapBiasSweepBeginValue + (TapBiasOffsetLoopCount * DfeTapSweepStepSize);
                  BitNew = Bit;
                } else if (Mode == NORMAL_PROGRAM_MODE) {
                  if (BestTapBias == NULL) {
                    RC_FATAL_ERROR (FALSE, ERR_MRC_POINTER, RC_FATAL_ERROR_MRC_POINTER_MINOR_CODE_002);
                    RcDebugPrintWithDevice (SDBG_ERROR, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "Error: Invalid NULL BestTapBias Pointer\n");
                    return;
                  }
                  DfeTapBiasValue = (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + (SubCh * (MaxBits)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex];

                  BitNew = (*RankList) [Rank].DeDqSwzDdr5[SubCh][Bit];
                  if (IsX4Dimm (Socket, Ch, Dimm)) {
                    BitNew = BitNew + (Bit / BITS_PER_NIBBLE) * BITS_PER_NIBBLE;
                    DRAM = GetStrobeFromNibble (Host,(BitNew / BITS_PER_NIBBLE));

                    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, Bit,
                      "CPU bit %d bit %d DRAM %d inside bit %d\n", Bit, BitNew, DRAM, (BitNew % BITS_PER_NIBBLE));

                  } else {
                    BitNew = BitNew + (Bit / MAX_BITS_IN_BYTE) * MAX_BITS_IN_BYTE;
                    DRAM = BitNew / MAX_BITS_IN_BYTE;
                    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "CPU bit %d bit %d DRAM %d inside bit %d\n", Bit, BitNew, DRAM, (BitNew % MAX_BITS_IN_BYTE));
                  }

                }
                if ((Host->nvram.mem.dramType == SPD_TYPE_DDR5) && (((*DimmNvList) [Dimm].SPDDramMfgId) == MFGID_MICRON)) {
                  if (DfeTapBiasValue < 0) {
                    SignBitValue = 0;
                  } else {
                    SignBitValue = 1;
                  }
                } else {
                  if (DfeTapBiasValue < 0) {
                    SignBitValue = 1;
                  } else {
                    SignBitValue = 0;
                  }
                }

                if (DfeTapIndex == TAP1_INDEX) {
                  Ddr5ModeRegister129.Data = 0;
                  Ddr5ModeRegister129.Bits.enable_disable_dfe_tap_1 = 1;
                  if (DfeTapBiasValue < 0) {
                    Ddr5ModeRegister129.Bits.sign_bit_dfe_tap_1_bias = SignBitValue;
                    Ddr5ModeRegister129.Bits.dfe_tap_1_bias = -DfeTapBiasValue;
                  } else {
                    Ddr5ModeRegister129.Bits.sign_bit_dfe_tap_1_bias = SignBitValue;
                    Ddr5ModeRegister129.Bits.dfe_tap_1_bias = DfeTapBiasValue;
                  }
                  DfeTapBiasMrAddessProgram = DDR5_MODE_REGISTER_129_REG;
                  DfeTapBiasValueProgram = Ddr5ModeRegister129.Data;
                } else if (DfeTapIndex == TAP2_INDEX) {
                  Ddr5ModeRegister130.Data = 0;
                  Ddr5ModeRegister130.Bits.enable_disable_dfe_tap_2 = 1;
                  if (DfeTapBiasValue < 0) {
                    Ddr5ModeRegister130.Bits.sign_bit_dfe_tap_2_bias = SignBitValue;
                    Ddr5ModeRegister130.Bits.dfe_tap_2_bias = -DfeTapBiasValue;
                  } else {
                    Ddr5ModeRegister130.Bits.sign_bit_dfe_tap_2_bias = SignBitValue;
                    Ddr5ModeRegister130.Bits.dfe_tap_2_bias = DfeTapBiasValue;
                  }
                  DfeTapBiasMrAddessProgram = DDR5_MODE_REGISTER_130_REG;
                  DfeTapBiasValueProgram = Ddr5ModeRegister130.Data;
                } else if (DfeTapIndex == TAP3_INDEX) {
                  Ddr5ModeRegister131.Data = 0;
                  Ddr5ModeRegister131.Bits.enable_disable_dfe_tap_3 = 1;
                  if (DfeTapBiasValue < 0) {
                    Ddr5ModeRegister131.Bits.sign_bit_dfe_tap_3_bias = SignBitValue;
                    Ddr5ModeRegister131.Bits.dfe_tap_3_bias = -DfeTapBiasValue;
                  } else {
                    Ddr5ModeRegister131.Bits.sign_bit_dfe_tap_3_bias = SignBitValue;
                    Ddr5ModeRegister131.Bits.dfe_tap_3_bias = DfeTapBiasValue;
                  }
                  DfeTapBiasMrAddessProgram = DDR5_MODE_REGISTER_131_REG;
                  DfeTapBiasValueProgram = Ddr5ModeRegister131.Data;
                } else if (DfeTapIndex == TAP4_INDEX) {
                  Ddr5ModeRegister132.Data = 0;
                  Ddr5ModeRegister132.Bits.enable_disable_dfe_tap_4 = 1;
                  if (DfeTapBiasValue < 0) {
                    Ddr5ModeRegister132.Bits.sign_bit_dfe_tap_4_bias = SignBitValue;
                    Ddr5ModeRegister132.Bits.dfe_tap_4_bias = -DfeTapBiasValue;
                  } else {
                    Ddr5ModeRegister132.Bits.sign_bit_dfe_tap_4_bias = SignBitValue;
                    Ddr5ModeRegister132.Bits.dfe_tap_4_bias = DfeTapBiasValue;
                  }
                  DfeTapBiasMrAddessProgram = DDR5_MODE_REGISTER_132_REG;
                  DfeTapBiasValueProgram = Ddr5ModeRegister132.Data;
                } else {
                  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "Input parameter DfeTapIndex is out of range!! InputDfeTapIndex = 0x%x, MinDfeTapIndex = 0x%x, MaxDfeTapIndex = 0x%x\n", DfeTapIndex, TAP1_INDEX, TAP4_INDEX);
                  return;
                }
                Nibble = BitNew / BITS_PER_NIBBLE;
                if ((*DimmNvList) [Dimm].x4Present == 0) {
                  EncodedNibble = GetStrobeFromNibble (Host, Nibble);
                  EncodedNibble = EncodedNibble % (MAX_STROBE_DDR5 / 4);
                  DfeTapMrwAddress = DfeTapBiasMrAddessProgram + ((BitNew % MAX_BITS_IN_BYTE) * DELTA_BETWEEN_DQ_DFE_TAP_MR_ADDRESS);
                } else {
                  EncodedNibble = GetStrobeFromNibble (Host, Nibble);
                  DfeTapMrwAddress = DfeTapBiasMrAddessProgram + ((BitNew % BITS_PER_NIBBLE) * DELTA_BETWEEN_DQ_DFE_TAP_MR_ADDRESS);
                }

                if (Mode == SWEEP_PROGRAM_MODE) {

                  if ((*DimmNvList) [Dimm].x4Present == 0) {
                    if ((Bit < MAX_BITS_IN_BYTE)) {
                      DramModeRegWrite (Socket, Ch, 1 << SubCh, Dimm, Rank, DfeTapMrwAddress, DfeTapBiasValueProgram);
                    }
                  } else {
                    if ((Bit < BITS_PER_NIBBLE)) {
                      DramModeRegWrite (Socket, Ch, 1 << SubCh, Dimm, Rank, DfeTapMrwAddress, DfeTapBiasValueProgram);
                    }
                  }

                } else if (Mode == NORMAL_PROGRAM_MODE) {
                  //
                  // Program DFE Tap Bias value. To add the x16 support.SPR_todo. HSD 1707143273
                  //
                  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, Bit, "Program Dfe DfeTap %d with value %d\n", DfeTapIndex + 1, DfeTapBiasValue);
                  if ((*DimmNvList) [Dimm].x4Present == 0) {
                    if (((Bit / MAX_BITS_IN_BYTE) == 0) &&
                        (TheSameValue[SubCh][Bit % MAX_BITS_IN_BYTE] == 1) &&
                        (FeaturePcdGet (PcdDdr5MrSameValueCheckEnable) == TRUE)) {
                      DramModeRegWrite (Socket, Ch, 1 << SubCh, Dimm, Rank, DfeTapMrwAddress, DfeTapBiasValueProgram);
                    } else if ((TheSameValue[SubCh][Bit % MAX_BITS_IN_BYTE] == 1) &&
                               (FeaturePcdGet (PcdDdr5MrSameValueCheckEnable) == TRUE)) {
                      RcDebugPrintWithDevice (SDBG_REG_ACCESS, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, Bit, "Skip Program Dfe DfeTap %d with value %d\n", DfeTapIndex + 1, DfeTapBiasValue);

                      continue;
                    } else {
                      DramModeRegWritePda (Socket, Ch, 1 << SubCh, Dimm, Rank, EncodedNibble, DfeTapMrwAddress, DfeTapBiasValueProgram);
                    }
                  } else {
                    if (((Bit / BITS_PER_NIBBLE) == 0) &&
                        ((TheSameValue[SubCh][Bit % BITS_PER_NIBBLE] == 1)) &&
                        (FeaturePcdGet (PcdDdr5MrSameValueCheckEnable) == TRUE)) {

                      DramModeRegWrite (Socket, Ch, 1 << SubCh, Dimm, Rank, DfeTapMrwAddress, DfeTapBiasValueProgram);
                    } else if ((TheSameValue[SubCh][Bit % BITS_PER_NIBBLE] == 1) &&
                               (FeaturePcdGet (PcdDdr5MrSameValueCheckEnable) == TRUE)) {
                      RcDebugPrintWithDevice (SDBG_REG_ACCESS, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, Bit, "Skip Program Dfe DfeTap %d with value %d\n", DfeTapIndex + 1, DfeTapBiasValue);

                      continue;
                    } else {
                      DramModeRegWritePda (Socket, Ch, 1 << SubCh, Dimm, Rank, EncodedNibble, DfeTapMrwAddress, DfeTapBiasValueProgram);
                    }
                  }

                }
              } //Bit loop
            } //SubCh loop
          }
        } // Ch loop
      } // Rank loop
    }// Dimm loop
  }

  //
  // Restore the debug message level.
  //
  SetDebugLevel (PrevDebugLevel);
#endif
  return;
} // ProgramDfeTapBias


/*++

  Enable all the taps for Write DFE.

  @param[in] Host               - Pointer to sysHost
  @param[in] Socket             - Socket nubmer
  @param[in] DfeTap             - The Tap to be enabled

  @retval NA

--*/
VOID
EnableTxDfeAllTaps (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN MRC_GT                DfeTap
  )
{

  UINT8                         Dimm;
  UINT8                         Rank;
  UINT8                         Ch;
  UINT8                         Bit;
  UINT8                         MaxBits = MAX_BITS_DDR5 / 2;
  struct dimmNvram              (*DimmNvList) [MAX_DIMM];
  UINT8                         MaxChDdr;
  UINT8                         SubCh = 0;
  UINT8                         Nibble = 0;
  UINT8                         EncodedNibble = 0;
  UINT8                         DfeTapBiasValueProgram;
  UINT8                         DfeTapBiasMrAddessProgram = DDR5_MODE_REGISTER_129_REG;
  UINT8                         RankPresent;
  struct rankDevice             (*RankStruct) [MAX_RANK_DIMM];
  struct ddrRank                (*RankList) [MAX_RANK_DIMM];
  UINT8                         DfeTapMrwAddress = 0;
  DDR5_MODE_REGISTER_129_STRUCT Ddr5ModeRegister129;
  DDR5_MODE_REGISTER_130_STRUCT Ddr5ModeRegister130;
  DDR5_MODE_REGISTER_131_STRUCT Ddr5ModeRegister131;
  DDR5_MODE_REGISTER_132_STRUCT Ddr5ModeRegister132;
  UINT32                        PrevDebugLevel;

  MaxChDdr = GetMaxChDdr ();
  if ((DfeTap != TxDfeTap1) &&
      (DfeTap != TxDfeTap2) &&
      (DfeTap != TxDfeTap3) &&
      (DfeTap != TxDfeTap4)) {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Error: DfeTap parameter value is invalid in function EnableTxDfeAllTaps\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_196);
    return;
  }
  //
  // Save the debug message level.
  //
  PrevDebugLevel = GetDebugLevel ();
  if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
    SetDebugLevel (SDBG_ERROR | SDBG_MEM_TRAIN);
  }
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      //
      // Initialize to no ranks present
      //
      RankPresent = 0;
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }

        //
        // Enable the Tap
        //
        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Enable the Tap %d\n", DfeTap - TxDfeTap1 + 1);

        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          for (Bit = 0; Bit < MaxBits; Bit++) {
            //
            // Skip if this is an ECC strobe when ECC is disabled
            //
            if (IsBitNotValidSubChDdr5 (Host, Bit)) {
              continue;
            }
            //
            // Init the Tap MR value.
            //
            if ((DfeTap == TxDfeTap1)) {
              Ddr5ModeRegister129.Data = 0;
              Ddr5ModeRegister129.Bits.enable_disable_dfe_tap_1 = 1;
              DfeTapBiasMrAddessProgram = DDR5_MODE_REGISTER_129_REG;
              DfeTapBiasValueProgram = Ddr5ModeRegister129.Data;
            }
            if ((DfeTap == TxDfeTap2)) {
              Ddr5ModeRegister130.Data = 0;
              Ddr5ModeRegister130.Bits.enable_disable_dfe_tap_2 = 1;
              DfeTapBiasMrAddessProgram = DDR5_MODE_REGISTER_130_REG;
              DfeTapBiasValueProgram = Ddr5ModeRegister130.Data;
            }
            if ((DfeTap == TxDfeTap3)) {
              Ddr5ModeRegister131.Data = 0;
              Ddr5ModeRegister131.Bits.enable_disable_dfe_tap_3 = 1;
              DfeTapBiasMrAddessProgram = DDR5_MODE_REGISTER_131_REG;
              DfeTapBiasValueProgram = Ddr5ModeRegister131.Data;
            }
            if ((DfeTap == TxDfeTap4)) {
              Ddr5ModeRegister132.Data = 0;
              Ddr5ModeRegister132.Bits.enable_disable_dfe_tap_4 = 1;
              DfeTapBiasMrAddessProgram = DDR5_MODE_REGISTER_132_REG;
              DfeTapBiasValueProgram = Ddr5ModeRegister132.Data;
            }
            //
            // Program the MR
            //
            Nibble = Bit / BITS_PER_NIBBLE;
            if ((*DimmNvList) [Dimm].x4Present == 0) {
              EncodedNibble = GetStrobeFromNibble (Host, Nibble);
              EncodedNibble = EncodedNibble % (MAX_STROBE_DDR5 / 4);
              DfeTapMrwAddress = DfeTapBiasMrAddessProgram + ((Bit % MAX_BITS_IN_BYTE) * DELTA_BETWEEN_DQ_DFE_TAP_MR_ADDRESS);
            } else {
              EncodedNibble = GetStrobeFromNibble (Host, Nibble);
              DfeTapMrwAddress = DfeTapBiasMrAddessProgram + ((Bit % BITS_PER_NIBBLE) * DELTA_BETWEEN_DQ_DFE_TAP_MR_ADDRESS);
            }
            if ((*DimmNvList) [Dimm].x4Present == 0) {
              if ((Bit < MAX_BITS_IN_BYTE)) {
                DramModeRegWrite (Socket, Ch, 1 << SubCh, Dimm, Rank, DfeTapMrwAddress, DfeTapBiasValueProgram);
              }
            } else {
              if ((Bit < BITS_PER_NIBBLE)) {
                DramModeRegWrite (Socket, Ch, 1 << SubCh, Dimm, Rank, DfeTapMrwAddress, DfeTapBiasValueProgram);
              }
            }
          } //Bit loop
        } //SubCh loop
      } //Ch
    } //Rank
  }//Dimm
  //
  // Restore the debug message level.
  //
  SetDebugLevel (PrevDebugLevel);

  return;
} // EnableTxDfeAllTaps

/*++

  Program Dfe Tap Bias Per bit

  @param[in] Host               - Pointer to sysHost
  @param[in] Socket             - Socket nubmer
  @param[in] Ch                 - Channel number
  @param[in] Dimm               - Dimm number
  @param[in] Rank               - Rnak number
  @param[in] Bit                - Bit number
  @param[in] FinalTapBiasValue  - Final Tap Bias Value

  @retval SUCCESS

--*/
VOID
ProgramDfeTapBiasPerbit (
  IN PSYSHOST               Host,
  IN UINT8                  Socket,
  IN UINT8                  Ch,
  IN UINT8                  Dimm,
  IN UINT8                  Rank,
  IN UINT8                  Bit,
  IN TapBiasValue           (*FinalTapBiasValue) [MAX_CH][MAX_RANK_CH][MAX_BITS]
  )
{
  struct dimmNvram    (*DimmNvList) [MAX_DIMM];
  UINT8 SubCh = 0;
  UINT8  Nibble = 0;
  UINT8  EncodedNibble = 0;
  UINT8  DfeTapBiasValueProgram;
  UINT8  DfeTapBiasMrAddessProgram;

  INT8 DfeTapBiasValue = 0;
  UINT8 DfeTapMrwAddress = 0;
  DDR5_MODE_REGISTER_129_STRUCT Ddr5ModeRegister129;
  DDR5_MODE_REGISTER_130_STRUCT Ddr5ModeRegister130;
  DDR5_MODE_REGISTER_131_STRUCT Ddr5ModeRegister131;
  DDR5_MODE_REGISTER_132_STRUCT Ddr5ModeRegister132;
  MRC_GT              DfeTap = TxDfeTap1;
  struct ddrRank      (*RankList) [MAX_RANK_DIMM];

  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  RankList = GetRankNvList (Host, Socket, Ch, Dimm);
  for (DfeTap = TxDfeTap1; DfeTap <= TxDfeTap4; DfeTap++) {
    //
    // How about DML and DMU?
    //
    DfeTapBiasValue = (*FinalTapBiasValue) [Ch][ (*RankList) [Rank].rankIndex][Bit].TapBiasValue[TAP_NUM_INDEX (DfeTap,TxDfeTap1) ];
    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Program Dfe DfeTap %d with value %d\n", DfeTap - TxDfeTap1 + 1, DfeTapBiasValue);

    if (DfeTap == TxDfeTap1) {
      Ddr5ModeRegister129.Data = 0;
      Ddr5ModeRegister129.Bits.enable_disable_dfe_tap_1 = 1;
      if (DfeTapBiasValue < 0) {
        Ddr5ModeRegister129.Bits.sign_bit_dfe_tap_1_bias = 1;
        Ddr5ModeRegister129.Bits.dfe_tap_1_bias = -DfeTapBiasValue;
      } else {
        Ddr5ModeRegister129.Bits.sign_bit_dfe_tap_1_bias = 0;
        Ddr5ModeRegister129.Bits.dfe_tap_1_bias = DfeTapBiasValue;
      }
      DfeTapBiasMrAddessProgram = DDR5_MODE_REGISTER_129_REG;
      DfeTapBiasValueProgram = Ddr5ModeRegister129.Data;
    } else if (DfeTap == TxDfeTap2) {
      Ddr5ModeRegister130.Data = 0;
      Ddr5ModeRegister130.Bits.enable_disable_dfe_tap_2 = 1;
      if (DfeTapBiasValue < 0) {
        Ddr5ModeRegister130.Bits.sign_bit_dfe_tap_2_bias = 1;
        Ddr5ModeRegister130.Bits.dfe_tap_2_bias = -DfeTapBiasValue;
      } else {
        Ddr5ModeRegister130.Bits.sign_bit_dfe_tap_2_bias = 0;
        Ddr5ModeRegister130.Bits.dfe_tap_2_bias = DfeTapBiasValue;
      }
      DfeTapBiasMrAddessProgram = DDR5_MODE_REGISTER_130_REG;
      DfeTapBiasValueProgram = Ddr5ModeRegister130.Data;
    } else if (DfeTap == TxDfeTap3) {
      Ddr5ModeRegister131.Data = 0;
      Ddr5ModeRegister131.Bits.enable_disable_dfe_tap_3 = 1;
      if (DfeTapBiasValue < 0) {
        Ddr5ModeRegister131.Bits.sign_bit_dfe_tap_3_bias = 1;
        Ddr5ModeRegister131.Bits.dfe_tap_3_bias = -DfeTapBiasValue;
      } else {
        Ddr5ModeRegister131.Bits.sign_bit_dfe_tap_3_bias = 0;
        Ddr5ModeRegister131.Bits.dfe_tap_3_bias = DfeTapBiasValue;
      }
      DfeTapBiasMrAddessProgram = DDR5_MODE_REGISTER_131_REG;
      DfeTapBiasValueProgram = Ddr5ModeRegister131.Data;
    } else if (DfeTap == TxDfeTap4) {
      Ddr5ModeRegister132.Data = 0;
      Ddr5ModeRegister132.Bits.enable_disable_dfe_tap_4 = 1;
      if (DfeTapBiasValue < 0) {
        Ddr5ModeRegister132.Bits.sign_bit_dfe_tap_4_bias = 1;
        Ddr5ModeRegister132.Bits.dfe_tap_4_bias = -DfeTapBiasValue;
      } else {
        Ddr5ModeRegister132.Bits.sign_bit_dfe_tap_4_bias = 0;
        Ddr5ModeRegister132.Bits.dfe_tap_4_bias = DfeTapBiasValue;
      }
      DfeTapBiasMrAddessProgram = DDR5_MODE_REGISTER_132_REG;
      DfeTapBiasValueProgram = Ddr5ModeRegister132.Data;
    }
    Nibble = Bit / 4;
    //
    // Program DFE Tap Bias value. To add the x16 support.
    //
    if ((*DimmNvList) [Dimm].x4Present == 0) {
      EncodedNibble = GetStrobeFromNibble (Host, Nibble);
      EncodedNibble = EncodedNibble % (MAX_STROBE_DDR5 / 4);
      DfeTapMrwAddress = DfeTapBiasMrAddessProgram + ((Bit % MAX_BITS_IN_BYTE) * DELTA_BETWEEN_DQ_DFE_TAP_MR_ADDRESS);
      DramModeRegWritePda (Socket, Ch, 1 << SubCh, Dimm, Rank, EncodedNibble, DfeTapMrwAddress, DfeTapBiasValueProgram);
    } else {
      EncodedNibble = GetStrobeFromNibble (Host, Nibble);
      DfeTapMrwAddress = DfeTapBiasMrAddessProgram + ((Bit % BITS_PER_NIBBLE) * DELTA_BETWEEN_DQ_DFE_TAP_MR_ADDRESS);
      DramModeRegWritePda (Socket, Ch, 1 << SubCh, Dimm, Rank, EncodedNibble, DfeTapMrwAddress, DfeTapBiasValueProgram);
    }
  } //DfeTap loop
} // ProgramDfeTapBiasPerbit

/*++

  Check Vref Results.

  @param[in]  Host                - Pointer to sysHost
  @param[in]  Socket              - Socket number
  @param[in]  Group               - Parameter to center
                                    RxVref 0: Center Rx Vref
                                    TxVref 1: Center Tx Vref
  @param[in]  PerBit              - Per Bit
  @param[in]  VrefMarginBitInput  - Vref Margin Bit Input
  @param[in]  TapSweepIndex       - Tap Sweep Index
  @param[out] FigureofMerit       - Figure of Merit
  @param[out] CenterVref          - Whether to do vref centering or not

  @retval SUCCESS

--*/
UINT32
CheckVrefResults (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN MRC_GT           Group,
  IN UINT8            PerBit,
  IN struct bitMargin (*VrefMarginBitInput),
  IN UINT8            TapSweepIndex,
  OUT EYEPOINT        (*FigureofMerit) [MAX_CH][MAX_RANK_CH][MAX_BITS][MAX_TAP_SWEEP_RANGE],
  IN BOOLEAN           CenterVref
  )
{
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               Strobe = 0;
  UINT8               Bit;
  UINT8               BitPerNibble = 0;
  UINT8               MaxStrobe;
  INT16               VrefHi = MAX_BIT_VREF_MARGIN;
  INT16               VrefLo = MIN_BIT_VREF_MARGIN;
  INT16               Offset;
  struct channelNvram (*ChannelNvList) [MAX_CH];
  struct dimmNvram    (*DimmNvList) [MAX_DIMM];
  struct ddrRank      (*RankList) [MAX_RANK_DIMM];
  struct bitMargin    (*VrefMarginBit);
  UINT8               MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8               MBV = GetMaxBitsValid (Host);
  SYS_SETUP           *Setup;
  UINT8               MaxRankPerDimm;


  Setup = GetSysSetupPointer ();
  VrefMarginBit = VrefMarginBitInput;
  ChannelNvList = GetChannelNvList (Host, Socket);
  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "                  vrefLo  vrefHi    offset,   FigureofMerit\n");
  if (PerBit == PER_BIT) {
    for (Bit = 0; Bit < MBV; Bit++) {
      //
      // Skip if this is an ECC strobe when ECC is disabled
      //
      if (IsBitNotValid (Host, Bit)) {
        continue;
      }
      for (Ch = 0; Ch < MAX_CH; Ch++) {
        //
        // Skip if channel is disabled
        //
        if ((*ChannelNvList) [Ch].enabled == 0) {
          continue;
        }
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
          //
          // Skip if no DIMM present
          //
          if ((*DimmNvList) [Dimm].dimmPresent == 0) {
            continue;
          }
          RankList = GetRankNvList (Host, Socket, Ch, Dimm);
          if (Group == DbDfeVrefGroup) {
            MaxRankPerDimm = 1;
          } else {
            MaxRankPerDimm = (*DimmNvList) [Dimm].numRanks;
          }
          for (Rank = 0; Rank < MaxRankPerDimm; Rank++) {
            //
            // Skip if no rank
            //
            if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
              continue;
            }
            VrefHi = MAX_BIT_VREF_MARGIN;
            VrefLo = MIN_BIT_VREF_MARGIN;
            Offset = (VrefHi + VrefLo) / 2;
            //
            // Calculate the center.
            //
            VrefHi = ((*VrefMarginBit).bits[Ch][ (*RankList) [Rank].rankIndex][Bit].p);
            VrefLo = ((*VrefMarginBit).bits[Ch][ (*RankList) [Rank].rankIndex][Bit].n);
            if ((VrefHi - VrefLo) < 0) {
              VrefHi = 0;
              VrefLo = 0;
            }
            (*FigureofMerit) [Ch][ (*RankList) [Rank].rankIndex][Bit][TapSweepIndex].FigureofMerit = (UINT8) (VrefHi - VrefLo);
            Offset = (VrefHi + VrefLo) / 2;
            (*FigureofMerit) [Ch][ (*RankList) [Rank].rankIndex][Bit][TapSweepIndex].Offset = Offset;
            RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, Bit, "  %2d      %2d       %2d,      %2d\n", VrefLo, VrefHi, Offset, VrefHi - VrefLo);
            MaxStrobe = GetMaxStrobe (Host, Socket, Ch, 0, RxVref, DdrLevel);
            if (MaxStrobe != 0) {
              Strobe = (((Bit % MAX_BITS_IN_BYTE) / (MBV / MaxStrobe) * MSVx8) + (Bit / MAX_BITS_IN_BYTE));
              BitPerNibble = Bit % (MBV / MaxStrobe);
            }
            if (CenterVref == TRUE) {
              GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, Bit, DdrLevel, Group,//SPR_todo. To check the bit parameter
                GSM_UPDATE_CACHE | GSM_WRITE_OFFSET , &Offset);
            }
          } // Rank loop
        } // Dimm loop
      } // Ch loop
    } // Bit loop
  }
  IO_Reset (Host, Socket);
  return SUCCESS;
} // CheckVrefResults

/**

  Display Dfe Tap Results.

  @param[in] Host          - Pointer to sysHost, the system Host (root) structure struct
  @param[in] Socket        - Socket Id
  @param[in] Ch            - Channel number (0-based)
  @param[in] SubCh         - Current sub channel
  @param[in] Dimm          - Current dimm
  @param[in] Rank          - Rank number (0-based)
  @param[in] DfeTapIndex   - Dfe Tap Index
  @param[in] FigureofMerit - Figure of Merit
  @param[in] DfeTap        - Dfe Tap

  @retval None

**/
VOID
DisplayDfeTapResultsChDdr5 (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN UINT8      SubCh,
  IN  UINT8     Dimm,
  IN UINT8      Rank,
  IN UINT8      DfeTapIndex,
  IN EYEPOINT  (*FigureofMerit) [MAX_CH][MAX_RANK_CH][MAX_BITS][MAX_TAP_SWEEP_RANGE],
  IN INT8       DfeTap
  )
{
  UINT8   Bit;
  UINT8   MaxBits;

  AcquirePrintControl ();

  MaxBits = MAX_BITS_DDR5 /2;
  RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT, "%3d", DfeTap);
  RcDebugPrint (SDBG_DEFAULT, " |");
  struct ddrRank      (*RankList) [MAX_RANK_DIMM];
  RankList = GetRankNvList (Host, Socket, Ch, Dimm);
  for (Bit = 0; Bit < MaxBits; Bit++) {
    if (IsBitNotValidSubChDdr5 (Host, Bit)) {
      continue;
    }
    if ((Bit % 8) == 0) {
      RcDebugPrint (SDBG_DEFAULT, " ");
    }
    RcDebugPrint (SDBG_DEFAULT, "%3d ", (*FigureofMerit) [Ch][ (*RankList) [Rank].rankIndex][Bit + SubCh * (MAX_BITS_DDR5 /2) ][DfeTapIndex].FigureofMerit);
  } // Bit loop
  RcDebugPrint (SDBG_DEFAULT, " |\n");
  ReleasePrintControl ();
} // DisplayDfeTapResultsChDdr5

/**

  Dfe Tap Bias Sweep Range Init.

  @param[in] DfeTapBiasSweepRangeInitValue    - Dfe Tap Bias Sweep Range Init Value
  @param[in] GroupInput                       - Group Input

  @retval None

**/
VOID
DfeTapBiasSweepRangeInit (
  IN DfeTapBiasSweepRange (*DfeTapBiasSweepRangeInitValue) [MAX_TAP_NUM],
  IN MRC_GT    GroupInput
  )
{
  if (GroupInput == TxDfe) {
    //
    // Tap 1
    //
    (*DfeTapBiasSweepRangeInitValue) [0].Begin = TX_DFE_TAP1_BEGIN;
    (*DfeTapBiasSweepRangeInitValue) [0].End = TX_DFE_TAP1_END;
    //
    // Tap 2
    //
    (*DfeTapBiasSweepRangeInitValue) [1].Begin = TX_DFE_TAP2_BEGIN;
    (*DfeTapBiasSweepRangeInitValue) [1].End = TX_DFE_TAP2_END;
    //
    // Tap 3
    //
    (*DfeTapBiasSweepRangeInitValue) [2].Begin = TX_DFE_TAP3_BEGIN;
    (*DfeTapBiasSweepRangeInitValue) [2].End = TX_DFE_TAP3_END;
    //
    // Tap 4
    //
    (*DfeTapBiasSweepRangeInitValue) [3].Begin = TX_DFE_TAP4_BEGIN;
    (*DfeTapBiasSweepRangeInitValue) [3].End = TX_DFE_TAP4_END;
  } else if (GroupInput == RxDfe) {
    //
    // Tap 1
    //
    (*DfeTapBiasSweepRangeInitValue) [0].Begin = 0;
    (*DfeTapBiasSweepRangeInitValue) [0].End = 31;
    //
    // Tap 2
    //
    (*DfeTapBiasSweepRangeInitValue) [1].Begin = 0;
    (*DfeTapBiasSweepRangeInitValue) [1].End = 15;
    //
    // Tap 3
    //
    (*DfeTapBiasSweepRangeInitValue) [2].Begin = 0;
    (*DfeTapBiasSweepRangeInitValue) [2].End = 15;
    //
    // Tap 4
    //
    (*DfeTapBiasSweepRangeInitValue) [3].Begin = 0;
    (*DfeTapBiasSweepRangeInitValue) [3].End = 15;
  } else if (GroupInput == DcaDfe) {
    //
    // Tap 1
    //
    (*DfeTapBiasSweepRangeInitValue) [0].Begin = DCA_DFE_TAP1_BEGIN;
    (*DfeTapBiasSweepRangeInitValue) [0].End = DCA_DFE_TAP1_END;
    //
    // Tap 2
    //
    (*DfeTapBiasSweepRangeInitValue) [1].Begin = DCA_DFE_TAP2_BEGIN;
    (*DfeTapBiasSweepRangeInitValue) [1].End = DCA_DFE_TAP2_END;
    //
    // Tap 3
    //
    (*DfeTapBiasSweepRangeInitValue) [2].Begin = DCA_DFE_TAP3_BEGIN;
    (*DfeTapBiasSweepRangeInitValue) [2].End = DCA_DFE_TAP3_END;
    //
    // Tap 4
    //
    (*DfeTapBiasSweepRangeInitValue) [3].Begin = DCA_DFE_TAP4_BEGIN;
    (*DfeTapBiasSweepRangeInitValue) [3].End = DCA_DFE_TAP4_END;
  } else if (GroupInput == DbDfe) {
    //
    // Tap 1
    //
    (*DfeTapBiasSweepRangeInitValue) [0].Begin = DB_DFE_TAP1_BEGIN;
    (*DfeTapBiasSweepRangeInitValue) [0].End = DB_DFE_TAP1_END;
    //
    // Tap 2
    //
    (*DfeTapBiasSweepRangeInitValue) [1].Begin = DB_DFE_TAP2_BEGIN;
    (*DfeTapBiasSweepRangeInitValue) [1].End = DB_DFE_TAP2_END;
    //
    // Tap 3
    //
    (*DfeTapBiasSweepRangeInitValue) [2].Begin = DB_DFE_TAP3_BEGIN;
    (*DfeTapBiasSweepRangeInitValue) [2].End = DB_DFE_TAP3_END;
    //
    // Tap 4
    //
    (*DfeTapBiasSweepRangeInitValue) [3].Begin = DB_DFE_TAP4_BEGIN;
    (*DfeTapBiasSweepRangeInitValue) [3].End = DB_DFE_TAP4_END;
  } else {
    RcDebugPrint (SDBG_MAX, "\n Warning: GroupInput parameter is not valid when calling DfeTapBiasSweepRangeInit\n");
  }
  return;
} // DfeTapBiasSweepRangeInit

/**

  Display The Best Dfe Tap.

  @param[in] Host          - Pointer to sysHost, the system Host (root) structure
  @param[in] socket        - current socket
  @param[in] ch            - channle number
  @param[in] Subch         - Sub Channel number (0-based)
  @param[in] dimm          - dimm number
  @param[in] rank          - rank number (0-based)
  @param[in] DfeTapIndex   - Dfe Tap Index
  @param[in] BestTapBias   - Best Tap Bias
  @param[in] FigureofMerit - Figure of Merit
  @param[in] SweepStepSize - Sweep Step Size
  @param[in] IterationLoop - Iteration Loop

  @retval None

**/
VOID
DisplayTheBestDfeTapDdr5 (
  IN PSYSHOST            Host,
  IN UINT8               Socket,
  IN UINT8               Ch,
  IN UINT8               SubCh,
  IN UINT8               Dimm,
  IN UINT8               Rank,
  IN UINT8               DfeTapIndex,
  IN TapBias             (*BestTapBias) [MAX_CH][MAX_RANK_CH][MAX_BITS][MAX_TAP_NUM][MAX_ITERATION_LOOP],
  IN EYEPOINT            (*FigureofMerit) [MAX_CH][MAX_RANK_CH][MAX_BITS][MAX_TAP_SWEEP_RANGE],
  IN UINT8               SweepStepSize,
  IN UINT8               IterationLoop
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8               Bit;
  UINT8               MaxBits;
  struct dimmNvram    (*DimmNvList) [MAX_DIMM];
  UINT8               MBV = MAX_BITS_DDR5 / 2;
  UINT8               MaxStrobe = MAX_STROBE_DDR5 / 2;
  struct ddrRank      (*RankList) [MAX_RANK_DIMM];
  RankList = GetRankNvList (Host, Socket, Ch, Dimm);


  //
  // Check the debug message level
  //
  if (!(GetDebugLevel () & SDBG_MEM_TRAIN)) {
    return;
  }
  AcquirePrintControl ();
  RcDebugPrintLine (SDBG_MEM_TRAIN, (MaxStrobe * 20), SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  //
  // Skip if no DIMM present
  //
  if ((*DimmNvList) [Dimm].dimmPresent) {
    if (Host->nvram.mem.eccEn == 0) {
      MaxBits = MBV - 8;
    } else {
      MaxBits = MBV;
    }
    RcDebugPrint (SDBG_MEM_TRAIN, "Max FigureofMerit:    ");
    for (Bit = 0; Bit < MaxBits; Bit++) {
      if ((Bit % 8) == 0) {
        RcDebugPrint (SDBG_MEM_TRAIN, " ");
      }
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        " %3d", (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + SubCh * (MAX_BITS_DDR5 / 2) ][DfeTapIndex][IterationLoop].FigureofMerit);
    } // Bit loop
    RcDebugPrint (SDBG_MEM_TRAIN, "\nMax Score:            ");
    for (Bit = 0; Bit < MaxBits; Bit++) {
      if ((Bit % 8) == 0) {
        RcDebugPrint (SDBG_MEM_TRAIN, " ");
      }
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        " %3d", (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + SubCh * (MAX_BITS_DDR5 / 2) ][DfeTapIndex][IterationLoop].Score);
    } // Bit loop
    RcDebugPrint (SDBG_MEM_TRAIN, "\nThe Best Dfe Tap Bias:");
    for (Bit = 0; Bit < MaxBits; Bit++) {
      if ((Bit % 8) == 0) {
        RcDebugPrint (SDBG_MEM_TRAIN, " ");
      }
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        " %3d", (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit + SubCh * (MAX_BITS_DDR5 /2) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex]);
    } // Bit loop
    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "\n");
  }

  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK
} // DisplayTheBestDfeTapDdr5

/**

  Displa yDfe Tap Sweep Results.

  @param[in] Host                   - Pointer to sysHost
  @param[in] Socket                 - Socket number
  @param[in] TapBiasCenter          - Tap Bias Center
  @param[in] DfeTapSweepRange       - Dfe Tap Sweep Range
  @param[in] DfeTapSweepStepSize    - Dfe Tap Sweep Step Size
  @param[in] FigureofMerit          - Figure of Merit
  @param[in] BestTapBias            - Best Tap Bias
  @param[in] DfeTapNum              - Dfe Tap Number
  @param[in] IterationLoop          - Iteration Loop
  @param[in] DfeGainBias            - Dfe Gain Bias
  @param[in] MaxTapBiasLoop         - Max Tap Bias Loop
  @param[in] TapBiasSweepBegin      - Tap Bias Sweep Begin

  @retval None

**/
STATIC
VOID
DisplayDfeTapSweepResults (
  IN PSYSHOST            Host,
  IN UINT8               Socket,
  IN INT8                (*TapBiasCenter) [MAX_CH][MAX_RANK_CH][MAX_BITS][MAX_TAP_NUM][MAX_ITERATION_LOOP],
  IN UINT8               DfeTapSweepRange,
  IN UINT8               DfeTapSweepStepSize,
  IN EYEPOINT            (*FigureofMerit) [MAX_CH][MAX_RANK_CH][MAX_BITS][MAX_TAP_SWEEP_RANGE],
  IN TapBias             (*BestTapBias) [MAX_CH][MAX_RANK_CH][MAX_BITS][MAX_TAP_NUM][MAX_ITERATION_LOOP],
  IN UINT8               DfeTapNum,
  IN UINT8               IterationLoop,
  IN UINT8               DfeGainBias,
  IN UINT8               MaxTapBiasLoop,
  IN INT8                TapBiasSweepBegin,
  IN MRC_GT              GroupInput
  )
{
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               SubCh;
  INT8                DfeTapSweep;
  UINT32              ErrResult;
  UINT8               ChStatus;
  UINT32              BwSerrEvenOdd[MAX_CH][SUB_CH][2][3];
  struct channelNvram (*ChannelNvList) [MAX_CH];
  struct dimmNvram    (*DimmNvList) [MAX_DIMM];
  UINT8               MaxChDdr;
  struct ddrRank      (*RankList) [MAX_RANK_DIMM];
  UINT8               TapBiasOffsetLoopCount = 0;
  UINT8               MaxRankPerDimm = 0;

  MaxChDdr = GetMaxChDdr ();
  ZeroMem ((UINT8 *) (&BwSerrEvenOdd), sizeof (BwSerrEvenOdd));
  ChannelNvList = GetChannelNvList (Host, Socket);
  ErrResult = 0;
  ChStatus = 0;

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    //
    // Skip if channel is disabled
    //
    if ((*ChannelNvList) [Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*DimmNvList) [Dimm].dimmPresent == 0) {
        continue;
      }
      RankList = GetRankNvList (Host, Socket, Ch, Dimm);
      if (GroupInput == DbDfe) {
        MaxRankPerDimm = 1;
      } else {
        MaxRankPerDimm = (*DimmNvList) [Dimm].numRanks;
      }
      for (Rank = 0; Rank < MaxRankPerDimm; Rank++) {
        //
        // Skip if no rank
        //
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }
        if (GroupInput == TxDfe) {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
            "Ddr5 Tx Dfe Tap Scaning Iteration: %d, DfeGainBias: %d\n", IterationLoop, DfeGainBias);
        } else if (GroupInput == DbDfe) {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
            "Ddr5 Db Dfe Tap Scaning Iteration: %d, DfeGainBias: %d\n", IterationLoop, DfeGainBias);
        } else {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
            "Ddr5 Rx Dfe Tap Scaning Iteration: %d, DfeGainBias: %d\n", IterationLoop, DfeGainBias);
        }
        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          if (SubCh == SUB_CH_A) {
            RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
              "      TAP %d |                                                                               Sub-channel A                                                          \n", DfeTapNum + 1);
          } else {
            RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
              "      TAP %d |                                                                              Sub-channel B                                                    |\n", DfeTapNum + 1);
          }
          RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
            "DQ:         |  0                           7    8                           15   16                          23   24                          31   32                          39 |\n");
          for (TapBiasOffsetLoopCount = 0; TapBiasOffsetLoopCount <= MaxTapBiasLoop; TapBiasOffsetLoopCount++) {
            DfeTapSweep = TapBiasSweepBegin + (TapBiasOffsetLoopCount * DfeTapSweepStepSize);
#ifdef DEBUG_CODE_BLOCK
            if (GetDebugLevel () & SDBG_MAX) {
              DisplayDfeTapResultsChDdr5 (Host, Socket, Ch, SubCh, Dimm, Rank, TapBiasOffsetLoopCount, FigureofMerit, DfeTapSweep);
            }
#endif // DEBUG_CODE_BLOCK
          } //DfeTapSweep loop
          DisplayTheBestDfeTapDdr5 (Host, Socket, Ch, SubCh, Dimm, Rank, DfeTapNum, BestTapBias, FigureofMerit, DfeTapSweepStepSize, IterationLoop);
        } //subCh loop
      } // rank loop
    } // dimm loop
  } // ch loop
} // DisplayDfeTapSweepResults

/**

  Display Dfe Tap Final Results.

  @param[in] Host                - Pointer to sysHost
  @param[in] Socket              - Socket number
  @param[in] FinalTapBiasValue   - Final Tap BiasValue

  @retval None

**/
STATIC
VOID
DisplayDfeTapFinalResults (
  IN PSYSHOST     Host,
  IN UINT8        Socket,
  IN MRC_GT       Group,
  IN TapBiasValue (*FinalTapBiasValue) [MAX_CH][MAX_RANK_CH][MAX_BITS]
  )
{
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               Rank;
  struct channelNvram (*ChannelNvList) [MAX_CH];
  struct dimmNvram    (*DimmNvList) [MAX_DIMM];
  UINT8               MemSsType;
  UINT8               MaxChDdr;
  UINT8               TapIndex = 0;
  struct ddrRank      (*RankList) [MAX_RANK_DIMM];
  UINT8               Bit;
  UINT8               MaxBits = MAX_BITS_DDR5;
  UINT8               MaxRankPerDimm = 0;

  MaxChDdr = GetMaxChDdr ();
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MAX_CH; Ch++) {
    //
    // Skip if channel is disabled
    //
    if ((*ChannelNvList) [Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*DimmNvList) [Dimm].dimmPresent == 0) {
        continue;
      }
      RankList = GetRankNvList (Host, Socket, Ch, Dimm);
      if (Group == DbDfeVrefGroup) {
        MaxRankPerDimm = 1;
      } else {
        MaxRankPerDimm = (*DimmNvList) [Dimm].numRanks;
      }
      for (Rank = 0; Rank < MaxRankPerDimm; Rank++) {
        //
        // Skip if no rank
        //
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }
        RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
          "Ddr5 Dfe Training Final Results\n");
        RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
          "          DQ | TAP   1   2   3   4                                                        \n");
        for (Bit = 0; Bit < MaxBits; Bit++) {
          if (IsBitNotValid (Host, Bit)) {
            continue;
          }
          RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, Bit, "%3d        ", Bit);
          for (TapIndex = 0; TapIndex < MAX_TAP_NUM; TapIndex++) {
            RcDebugPrint (SDBG_DEFAULT, "%3d ", (*FinalTapBiasValue) [Ch][ (*RankList) [Rank].rankIndex][Bit].TapBiasValue[TapIndex]);
          }
          RcDebugPrint (SDBG_DEFAULT, "\n");
        } //Bit loop
      } // rank loop
    } // dimm loop
  } // ch loop
} // DisplayDfeTapFinalResults

/*++

  Get DFE Gain Bias settings

  @param[in]  GroupInput         - Group Input
  @param[out] MaxGainBiasValue   - Max Gain Bias Value
  @param[out] GainBiasValueBegin - Gain Bias Value Begin

  @retval None

--*/
VOID
GetDfeGainBias (
  IN  MRC_GT    GroupInput,
  OUT UINT8     *MaxGainBiasValue,
  OUT UINT8     *GainBiasValueBegin
  )
{

  if (GroupInput == RxDfe) {
    *MaxGainBiasValue = MAX_RX_DFE_GAIN_BIAS;
    *GainBiasValueBegin = RX_DFE_GAIN_BIAS_BEGIN;
  } else if (GroupInput == TxDfe) {
    //
    // If post silicon shows that we can hardcode the value for all the cases, then the below value can be changed to a PCD.
    //
    *MaxGainBiasValue = MAX_TX_DFE_GAIN_BIAS;
    *GainBiasValueBegin = TX_DFE_GAIN_BIAS_BEGIN;
  } else if (GroupInput == DcaDfe) {
    *MaxGainBiasValue = MAX_DCA_DFE_GAIN_BIAS;
    *GainBiasValueBegin = DCA_DFE_GAIN_BIAS_BEGIN;
  } else if (GroupInput == DbDfe) {
    *MaxGainBiasValue = MAX_DB_DFE_GAIN_BIAS;
    *GainBiasValueBegin = DB_DFE_GAIN_BIAS_BEGIN;
  } else {
    RcDebugPrint (SDBG_ERROR, "\n Error: GroupInput parameter is not valid when calling EnableDfe\n");
    ASSERT (FALSE);
    return;
  }
  return;
} //GetDfeGainBias

/**
  Enable DFE settings

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Socket number
  @param[in] GroupInput   - Group Input

  @retval MRC_STATUS_SUCCESS  Successfully enabled
  @retval other               An error occurred
**/
MRC_STATUS
EnableDfe (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN MRC_GT    GroupInput
  )
{
  MRC_STATUS  Status;

  Status = MRC_STATUS_SUCCESS;

  if (GroupInput == RxDfe) {
    EnableRxDfe (Socket);
  } else if (GroupInput == TxDfe) {
    TxDfeInit (Host, Socket, TxDfeTapAll);
    //
    // Enable the Tx DFE Tap 1
    //
    EnableTxDfeAllTaps (Host, Socket, TxDfeTap1);
    //
    // Enable the Tx DFE Tap 2
    //
    EnableTxDfeAllTaps (Host, Socket, TxDfeTap2);
    //
    // Enable the Tx DFE Tap 3
    //
    EnableTxDfeAllTaps (Host, Socket, TxDfeTap3);
    //
    // Enable the Tx DFE Tap 4
    //
    EnableTxDfeAllTaps (Host, Socket, TxDfeTap4);

  } else if (GroupInput == DbDfe) {
    //
    // Enable DB DFE.
    //
    DbDfeInit (Host, Socket, DbDfeTapAll);
  } else {
    RcDebugPrint (SDBG_ERROR, "\n Error: GroupInput parameter is not valid when calling EnableDfe\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_186);
    Status = MRC_STATUS_INVALID_PARAMETER;
  }

  return Status;
} // EnableDfe


/**
  Get Figure Of Merit for DB DFE training

  @param[in] Host              - Pointer to sysHost
  @param[in] Socket            - Socket number
  @param[in] TapIndex          - The index of the tap that will be used.
  @param[in] VrefMarginBit     - The structure to store the margin result.

  @retval NA
**/

VOID
GetDbDfeFigureOfMerit (
  IN     PSYSHOST          Host,
  IN     UINT8             Socket,
  IN     UINT8             TapIndex,
  OUT    struct bitMargin  (*VrefMarginBit)

  )
{
  UINT8                Channel;
  UINT32               ChBitmask;
  UINT8                MaxChDdr;
  UINT8                Dimm;
  UINT8                Rank = 0;  // Only target Rank 0 per DIMM
  MRC_MST              MemSsType;
  UINT16               MarginStep;
  UINT8                LogRank = 0;
  DB_BIT_ERROR_RESULTS_WR_DDR5   (*ErrorResults) [MAX_CH][SUB_CH];

  ErrorResults = RcAllocatePool (sizeof (*ErrorResults));
  if (ErrorResults == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return;
  }

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  MaxChDdr = GetMaxChDdr ();

  //
  // Set up DB DFE Training
  //
  SetupDbDfeTraining  (Host, Socket, TapIndex, DbDfeVref);

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    //
    // Create a bit mask of channels that have at least 1 rank present for this dimm and rank
    //
    ChBitmask = GetChBitmaskLrdimm (Socket, Dimm, Rank);
    if (ChBitmask == 0) {
      continue;
    }
    for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
      if (!((1 << Channel) & ChBitmask)) {
        continue;
      }
      //
      // Get the logical rank #
      //
      LogRank = GetLogicalRank (Host, Socket, Channel, Dimm, Rank);

      SelectCPGCRanks (Host, Socket, Channel, 1 << LogRank, 0, 0);
    }
    //
    // Clear error results before starting inner loop
    //
    ZeroMem ((UINT8 *) (*ErrorResults), sizeof ((*ErrorResults)));

    ProgramDbDfeTaTrainingMode (Host, Socket, ChBitmask, Dimm, Rank, DbDfeTaStart, TapIndex, DbDfeVref);
    for (MarginStep = 0; MarginStep <= DB_DFE_VREF_MARGIN_NUM_OF_STEPS; MarginStep++) {
      //
      // Start Test
      //
      TestControl (Host, Socket, MemSsType, ChBitmask, NO_SUBCH, WriteDbDfeTest, 1, START_TEST, NOT_STOP_TEST);
      CollectDbDfeTestResults (Host, Socket, ChBitmask, Dimm, MarginStep, DB_DFE_VREF_MARGIN_NUM_OF_STEPS, ErrorResults);
    }
    //
    // Each outer loop step, record the margin results
    //
    RecordVrefMargin (Host, Socket, ChBitmask, Dimm, DB_DFE_VREF_MARGIN_NUM_OF_STEPS, ErrorResults, VrefMarginBit);
  } // Dimm loop

  RcFreePool (ErrorResults);
  return;
}


/**

  Get the figure of merit based on the input paramter Group.

  @param[in] Host                   - Pointer to sysHost
  @param[in] Socket                 - Socket number
  @param[in] Group                  - Group
  @param[in] TimingGroup            - TimingGroup
  @param[in] TimingProgramGroup            - TimingProgramGroup
  @param[in] TapBiasOffsetLoopCount - Tap Bias Offset Loop Count
  @param[in] PerBit                 - Per Bit
  @param[in] FigureOfMeritTap       - Figure of Merit

  @retval Status

**/
MRC_STATUS
GetFigureOfMerit (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN MRC_GT   Group,
  IN MRC_GT   TimingGroup,
  IN MRC_GT   TimingProgramGroup,
  IN UINT8    TapBiasOffsetLoopCount,
  IN UINT8    PerBit,
  IN EYEPOINT (*FigureOfMeritTap) [MAX_CH][MAX_RANK_CH][MAX_BITS][MAX_TAP_SWEEP_RANGE],
  IN MARGIN_TEST MarginTest,
  IN UINT8 TapIndex
  )
{
  UINT8                Ch;
  UINT8                Dimm;
  UINT8                Rank;
  UINT8                BitIndex;
  UINT8                Bit;
  UINT8                Nibble;
  UINT8                EncodedNibble;
  UINT8                SubCh = 0;
  UINT8                SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  struct bitMargin     Results;
  struct bitMargin     VrefMarginBit;
  struct dimmNvram     (*DimmNvList) [MAX_DIMM];
  struct ddrRank       (*RankList) [MAX_RANK_DIMM];
  INT16                TOffset[BITS_PER_NIBBLE];
  DB_DFE_TAPS          DbDfeTap;
#ifdef DEBUG_CODE_BLOCK
  UINT8                MaxRankPerDimm = 0;
#endif //DEBUG_CODE_BLOCK
  UINT8                MaxChDdr = GetMaxChDdr ();
  SYS_SETUP           *Setup = GetSysSetupPointer ();
  UINT8                MaxBitIndex;

  if (GetCurrentTestType (Socket) == ReadDfeTest) {
    MaxBitIndex = BITS_PER_NIBBLE;
  } else {
    MaxBitIndex = 1;
  }

  ZeroMem (&Results, sizeof (Results));
  ZeroMem (&VrefMarginBit, sizeof (VrefMarginBit));
  ZeroMem (TOffset, sizeof (TOffset));



  if (MarginTest == REAPEAT_TEST) {
    //
    // Setting the Tap parameter as NULL for the first time to get the figure of merit.
    //
    DbDfeTap = DbTapNull;
  } else {
    DbDfeTap = TapIndex;
  }

  if (Group != DbDfeVrefGroup) {
    for (BitIndex = 0; BitIndex < MaxBitIndex; BitIndex++) {
      Host->var.mem.socket[Socket].TrainingStatus.Bits.BitIndex = BitIndex;
      UpdateDramSeed (Socket);
      GetDfeDataMargins (Socket, DdrLevel, TimingGroup, NULL, &Results, NULL);
    }

#ifdef DEBUG_CODE_BLOCK
    if ((GetDebugLevel () & SDBG_MAX))  {
      DisplayResultsDdr5 (Host, Socket, TimingProgramGroup);
    }
#endif // DEBUG_CODE_BLOCK
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Recenter timing\n");
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }
      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

            for (Nibble = 0; Nibble < SubChMSVx4; Nibble++) {
              //
              // Skip if this is an ECC nibble when ECC is disabled
              //
              if (IsNibbleNotValidDdr5 (Host, Nibble)) {
                continue;
              }
              EncodedNibble = GetStrobeFromNibble (Host, Nibble);
              for (Bit = 0; Bit < BITS_PER_NIBBLE; Bit++) {
                BitIndex = ((Nibble + SubCh * SubChMSVx4) * BITS_PER_NIBBLE) + Bit;
                if (((Results).bits[Ch][ (*RankList) [Rank].rankIndex][BitIndex].n) >= ((Results).bits[Ch][ (*RankList) [Rank].rankIndex][BitIndex].p)) {
                  TOffset[Bit] = 0;
                } else {
                  TOffset[Bit] = ((Results).bits[Ch][ (*RankList) [Rank].rankIndex][BitIndex].p + (Results).bits[Ch][ (*RankList) [Rank].rankIndex][BitIndex].n) / 2;
                }
              }
              GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, EncodedNibble, ALL_BITS, DdrLevel, TimingProgramGroup, GSM_UPDATE_CACHE + GSM_WRITE_OFFSET,
                TOffset);
            } //Nibble loop
          } //SubCh loop
        } // rank loop
      } // dimbecm loop
    } // ch loop

#ifdef DEBUG_CODE_BLOCK
    if ((GetDebugLevel () & SDBG_MAX))  {
      DisplayResultsDdr5 (Host, Socket, TimingProgramGroup);
    }
#endif // DEBUG_CODE_BLOCK
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Get the figure of merit at the current timing\n");
  }
  //
  // Get the figure of merit at the current timing
  //
  if (Group == DbDfeVrefGroup) {
    //
    // Get figure of merit of DB DFE.
    //
    GetDbDfeFigureOfMerit  (Host, Socket, DbDfeTap, &VrefMarginBit);
  } else {

    for (BitIndex = 0; BitIndex < MaxBitIndex; BitIndex++) {
      Host->var.mem.socket[Socket].TrainingStatus.Bits.BitIndex = BitIndex;
      UpdateDramSeed (Socket);
      GetDfeDataMargins (Socket, DdrLevel, Group, NULL, &VrefMarginBit, NULL);
    }

  }
  //
  // Display the per bit results.
  //
#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    AcquirePrintControl ();

    if (Group == RxSamplerEvenOdd) {
      RcDebugPrint (SDBG_DEFAULT, "START_RX_SAMPLER_CENTER\n");
    } else {
      RcDebugPrint (SDBG_DEFAULT, "START_TX_VREF_CENTER\n");
    }
    for (Ch = 0; Ch < MAX_CH; Ch++) {
      //
      // Skip if channel is disabled
      //
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }
      DimmNvList = GetDimmNvList (Host, Socket, Ch);

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        //
        // Skip if no DIMM present
        //
        if ((*DimmNvList) [Dimm].dimmPresent == 0) {
          continue;
        }
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        if (Group == DbDfeVrefGroup) {
          MaxRankPerDimm = 1;
        } else {
          MaxRankPerDimm = (*DimmNvList) [Dimm].numRanks;
        }
        for (Rank = 0; Rank < MaxRankPerDimm; Rank++) {
          //
          // Skip if no rank or if it is mapped out
          //
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          if (IsLrdimmPresent (Socket, Ch, Dimm)&& (Group == TxVref) && (Rank > 0)) {
            continue;
          }
          RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "vref Margins      -          0          +\n");
          for (Bit = 0; Bit < MAX_BITS_DDR5; Bit++) {
            //
            // Skip if this is an ECC strobe when ECC is disabled
            //
            if (IsBitNotValid (Host, Bit)) {
              continue;
            }
            if ((VrefMarginBit).bits[Ch][ (*RankList) [Rank].rankIndex][Bit].n >=
                (VrefMarginBit).bits[Ch][ (*RankList) [Rank].rankIndex][Bit].p) {
              (VrefMarginBit).bits[Ch][ (*RankList) [Rank].rankIndex][Bit].p = 0;
              (VrefMarginBit).bits[Ch][ (*RankList) [Rank].rankIndex][Bit].n = 0;
            }
            RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, Bit, " %3d : %3d\n",
              (VrefMarginBit).bits[Ch][ (*RankList) [Rank].rankIndex][Bit].n,
              (VrefMarginBit).bits[Ch][ (*RankList) [Rank].rankIndex][Bit].p
              );
          } // bit loop
          RcDebugPrint (SDBG_MAX, "\n");
        } // rank loop
      } // dimm loop
    } // ch loop
    if (Group == RxSamplerEvenOdd) {
      RcDebugPrint (SDBG_DEFAULT, "STOP_RX_SAMPLER_CENTER\n");
    } else {
      RcDebugPrint (SDBG_DEFAULT, "STOP_TX_VREF_CENTER\n");
    }
    ReleasePrintControl ();
  }
#endif  // DEBUG_CODE_BLOCK
  if ((Group == TxVref) && (Setup->mem.optionsExt & PDA_EN) && (CheckDimmType (Host) == SUCCESS)) {
    Host->nvram.mem.socket[Socket].TrainingStepDone.TxVref = 1;
  }
  //
  // Get the figure of merit.
  //
  CheckVrefResults (Host, Socket, Group, PerBit, &VrefMarginBit, TapBiasOffsetLoopCount, (FigureOfMeritTap), FALSE);
  return MRC_STATUS_SUCCESS;
}


/*++

  DDR5 DFE Training

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Socket number
  @param[in] GroupInput   - Group Input
                            RxDfe : Do Rx Dfe Training
                            TxDfe : Do Tx Dfe Training

  @retval SUCCESS

--*/
UINT32
DfeTrainingDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN MRC_GT    GroupInput
  )
{

  UINT8                Ch;
  UINT8                Dimm;
  UINT8                Rank;
  UINT8                Strobe;
  INT16                TOffset[BITS_PER_NIBBLE];
  INT16                LowMargin;
  INT16                HighMargin;
  UINT8                IterationLoop = 0;
  MRC_GT               TGroup = 0;
  MRC_GT               TLocalGroup = 0;
  UINT8                DfeTap = TAP1_INDEX;
  UINT8                DfeTapTemp = TAP1_INDEX;
  INT8                 DfeTapSweepBegin = 0;
  UINT8                DfeTapSweepMax = 0;
  UINT8                DfeTapSweepStepSize = 0;
  UINT8                DfeTapSweepRange = 0;
  UINT8                MaxTapBiasLoop = 0;
  UINT8                TapBiasOffsetLoopCount = 0;
  struct bitMargin     (*VrefMarginBit);
  UINT8                Bit = 0;
  UINT16               TapBiasBestCore = 0;
  UINT16               TapBiasScoreTemp = 0;
  UINT16               MaxTapBiasScore = 0;
  INT8                 TapBiasSweepBegin = 0;
  UINT16                MaxFigureofMerit = 0;
  UINT8                IterationLoopTemp = 0;
  MRC_GT               FinalBestTap = 0;
  UINT8                FinalBestIterationLoop = 0;
  UINT8                RankDone = 1;
  UINT8                AllChDone = 1;
  UINT8                ChDone[MAX_CH];
  EYEPOINT             (*FigureOfMeritTap) [MAX_CH][MAX_RANK_CH][MAX_BITS][MAX_TAP_SWEEP_RANGE];
  TapBias              (*BestTapBias) [MAX_CH][MAX_RANK_CH][MAX_BITS][MAX_TAP_NUM][MAX_ITERATION_LOOP];
  TapBiasValue         (*FinalTapBiasValue) [MAX_CH][MAX_RANK_CH][MAX_BITS];
  INT8                 (*TapBiasCenter) [MAX_CH][MAX_RANK_CH][MAX_BITS][MAX_TAP_NUM][MAX_ITERATION_LOOP];
  INT8                 MinTapBias = 0;
  INT8                 MaxTapBias = 0;
  DfeTapBiasSweepRange (*DfeTapBiasSweepRangeInitValue) [MAX_TAP_NUM];
  INT8                 TapBiasValueTemp[MAX_TAP_NUM];
  UINT8                (*ChRankDone) [MAX_CH][MAX_RANK_CH];
  UINT8                (*ChRankBitDone) [MAX_CH][MAX_RANK_CH][MAX_BITS];
  UINT8                BitDone = 0;
  UINT8                TapIndex = 0;
  UINT8                BestFigureofMerit = 0;
  UINT8                PerBit = PER_BIT;
  UINT8                BitPerNibble = 0;
  struct bitMask       (*Filter);
  struct rankMargin    (*RankResults);
  struct bitMargin     (*Results);
  struct channelNvram  (*ChannelNvList) [MAX_CH];
  struct dimmNvram     (*DimmNvList) [MAX_DIMM];
  struct ddrRank       (*RankList) [MAX_RANK_DIMM];
  BOOLEAN              BestTapBiasCheck = TRUE;
  BOOLEAN              RestoreOriginalTapBias = FALSE;

  UINT8                MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8                MaxChDdr;
  SYS_SETUP           *Setup;
  MRC_GT               Group;
  INT16                VrefHi = 0x5F;
  INT16                VrefLo = -0x5F;
  UINT8                MaxStrobe = 0;
  UINT8                DfeGainBias = 0;
  UINT8                MBV = MAX_BITS_DDR5;
  UINT8                MaxGainBiasValue = 0;
  UINT8                GainBiasBeginValue = 0;
  INT8                 BestTapBiasValueTemp = 0;
  UINT8                IterationLoop2 = 0;
  INT8                 LastBestTapBiasValue = 0;
  UINT8                MaxTapIndex = 0;
  UINT8                MaxRankPerDimm = 0;
  UINT8                FigureofMerit;
  UINT8                Filtering;

  Setup = GetSysSetupPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  FigureofMerit = FIGURE_OF_MERIT;
  Filtering = FILTERING;

  if (Host->nvram.mem.socket[Socket].maxDimmPop == 0) {
    return SUCCESS;
  }

  VrefMarginBit = RcAllocatePool (sizeof (*VrefMarginBit));
  if (VrefMarginBit == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return FAILURE;
  }

  FigureOfMeritTap = RcAllocatePool (sizeof (*FigureOfMeritTap));
  if (FigureOfMeritTap == NULL) {
    ASSERT (FALSE);
    return FAILURE;
  }

  BestTapBias = RcAllocatePool (sizeof (*BestTapBias));
  if (BestTapBias == NULL) {
    ASSERT (FALSE);
    return FAILURE;
  }

  TapBiasCenter = RcAllocatePool (sizeof (*TapBiasCenter));
  if (TapBiasCenter == NULL) {
    ASSERT (FALSE);
    return FAILURE;
  }

  FinalTapBiasValue = RcAllocatePool (sizeof (*FinalTapBiasValue));
  if (FinalTapBiasValue == NULL) {
    ASSERT (FALSE);
    return FAILURE;
  }

  DfeTapBiasSweepRangeInitValue = RcAllocatePool (sizeof (*DfeTapBiasSweepRangeInitValue));
  if (DfeTapBiasSweepRangeInitValue == NULL) {
    ASSERT (FALSE);
    return FAILURE;
  }

  ChRankDone = RcAllocatePool (sizeof (*ChRankDone));
  if (ChRankDone == NULL) {
    ASSERT (FALSE);
    return FAILURE;
  }

  ChRankBitDone = RcAllocatePool (sizeof (*ChRankBitDone));
  if (ChRankBitDone == NULL) {
    ASSERT (FALSE);
    return FAILURE;
  }

  Filter = RcAllocatePool (sizeof (*Filter));
  if (Filter == NULL) {
    ASSERT (FALSE);
    return FAILURE;
  }

  RankResults = RcAllocatePool (sizeof (*RankResults));
  if (RankResults == NULL) {
    ASSERT (FALSE);
    return FAILURE;
  }

  Results = RcAllocatePool (sizeof (*Results));
  if (Results == NULL) {
    ASSERT (FALSE);
    return FAILURE;
  }

  ZeroMem ((UINT8 *) (VrefMarginBit), sizeof ((*VrefMarginBit)));
  ZeroMem ((UINT8 *) (*FigureOfMeritTap), sizeof (*FigureOfMeritTap));
  ZeroMem ((UINT8 *) (*BestTapBias), sizeof (*BestTapBias));
  ZeroMem ((UINT8 *) (TapBiasCenter), sizeof (*TapBiasCenter));
  ZeroMem ((UINT8 *) (FinalTapBiasValue), sizeof (*FinalTapBiasValue));
  ZeroMem ((UINT8 *) (DfeTapBiasSweepRangeInitValue), sizeof (*DfeTapBiasSweepRangeInitValue));
  ZeroMem ((UINT8 *) (ChRankDone), sizeof (*ChRankDone));
  ZeroMem ((UINT8 *) (ChRankBitDone), sizeof (*ChRankBitDone));
  ZeroMem ((UINT8 *) (ChDone), sizeof (ChDone));
  ZeroMem ((UINT8 *) (Filter), sizeof (*Filter));
  ZeroMem ((UINT8 *) (RankResults), sizeof (*RankResults));
  ZeroMem ((UINT8 *) (Results), sizeof (*Results));
  ZeroMem ((UINT8 *) (TOffset), sizeof (TOffset));
  ZeroMem ((UINT8 *) (TapBiasValueTemp), sizeof (TapBiasValueTemp));

  if (FigureofMerit == EYEHEIGHT) {
    if (GroupInput == TxDfe) {
      Group = TxVref;
      TGroup = TxDqDelay;
      TLocalGroup = TxDqBitDelay;
    } else if (GroupInput == RxDfe) {
      if (IsSiliconWorkaroundEnabled ("S22010823763")) {
        Group = RxVref;
      } else {
        Group = RxSamplerEvenOdd;
      }
      TGroup = RxDqsDelay;
      TLocalGroup = RxDqsBitDelay;
    } else if (GroupInput == DbDfe) {
      Group = DbDfeVrefGroup;
      TGroup = TxDqDelay;
      TLocalGroup = TxDqBitDelay;
    } else {
      RcDebugPrint (SDBG_ERROR, "\n Error: GroupInput parameter is not valid when calling DfeTrainingDdr5\n");
      ASSERT (FALSE);
      return FAILURE;
    }
  } else if (FigureofMerit == EYEWIDTH) {
    if (GroupInput == TxDfe) {
      Group = TxDqDelay;
    } else if (GroupInput == RxDfe) {
      Group = RxDqsDelay;
    } else {
      RcDebugPrint (SDBG_ERROR, "\n Error: GroupInput parameter is not valid when calling DfeTrainingDdr5\n");
      ASSERT (FALSE);
      return FAILURE;
    }
  }

#ifdef DEBUG_CODE_BLOCK
  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "DfeTrainingDdr5 starts\n");
  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "\nPrevious Settings ");

  if (Group == RxSamplerEvenOdd) {
    DisplayResultsDdr5 (Host, Socket, RxSamplerEven);
    DisplayResultsDdr5 (Host, Socket, RxSamplerOdd);
    DisplayResultsDdr5 (Host, Socket, RxDqsBitDelay);
  } else if (Group == RxVref) {
    DisplayResultsDdr5 (Host, Socket, RxVref);
    DisplayResultsDdr5 (Host, Socket, RxDqsBitDelay);
  } else if (Group == TxVref) {
    DisplayResultsDdr5 (Host, Socket, TxVref);
    DisplayResultsDdr5 (Host, Socket, TxDqBitDelay);
  }
#endif //DEBUG_CODE_BLOCK

  ClearFilter (Host, Socket, Filter);

  MaxChDdr = GetMaxChDdr ();
  //
  // Init MaxTapIndex
  //
  if (GroupInput == DbDfe) {
    //
    // DB DFE uses the Training Accelerator.
    //
    MaxTapIndex = TAP4_INDEX;
  } else if (GroupInput == TxDfe) {
    MaxTapIndex = TAP3_INDEX;
  } else {
    MaxTapIndex = TAP4_INDEX;
  }
  //
  // Tx DFE init.
  //
  DfeTapBiasSweepRangeInit (DfeTapBiasSweepRangeInitValue, GroupInput);
  //
  // Enable DFE
  //
  if (GroupInput != TxDfe) {
    EnableDfe (Host, Socket, GroupInput);
  }
  RcDebugPrint (SDBG_DEFAULT, "Repeatability Test Start\n");
  //
  // Get the figure of merit.
  //
  if (GroupInput == DbDfe) {
    //
    // Configure the TA
    //
    ConfigureDbDfeTraining (Host, Socket, DbTapNull, DbDfeVref, 0, 0,
      0, DB_DFE_VREF_MARGIN_START, DB_DFE_VREF_MARGIN_STEP_SIZE,
      DB_DFE_VREF_MARGIN_NUM_OF_STEPS);
  }
  GetFigureOfMerit (Host, Socket, Group, TGroup, TLocalGroup, TapBiasOffsetLoopCount, PerBit, FigureOfMeritTap, REAPEAT_TEST, 0);

  RcDebugPrint (SDBG_DEFAULT, "Repeatability Test End\n");
  //
  // Get DFE GAIN BIAS Value.
  //
  GetDfeGainBias (GroupInput, &MaxGainBiasValue, &GainBiasBeginValue);
  //
  // Dfe Gain Bias Loop
  //
  for (DfeGainBias = GainBiasBeginValue; DfeGainBias <= MaxGainBiasValue; DfeGainBias++) {
    //
    // Program the DFE Gain Bias.
    //
    ProgramDfeGainBias (Host, Socket, DfeGainBias, GroupInput);
    //
    // Iteration Loop
    //
    for (IterationLoop = FIRST_ITERATION_LOOP; IterationLoop < MAX_ITERATION_LOOP; IterationLoop++) {
      //
      // Tap Loop
      //
      for (DfeTap = TAP1_INDEX; DfeTap <= MaxTapIndex; DfeTap++) {
        //
        // Get the DFE tap sweep begin, step size and range.
        //
        GetDfeTapSweepParameter (Host, Socket, DfeTap, IterationLoop, &DfeTapSweepRange, &DfeTapSweepStepSize, TapBiasCenter, DfeTapBiasSweepRangeInitValue, &TapBiasSweepBegin);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Dfe Tap  %d Iteration Loop  %d Tap Sweep starts  %d, ends  %d with step size  %d\n", DfeTap + 1, IterationLoop, DfeTapSweepBegin, DfeTapSweepMax, DfeTapSweepStepSize);
        MaxTapBiasLoop = DfeTapSweepRange / DfeTapSweepStepSize;
        if (GroupInput == DbDfe) {
          //
          // Configure the TA
          //
          ConfigureDbDfeTraining (Host, Socket, DfeTap, DbDfeVref, TapBiasSweepBegin, DfeTapSweepStepSize,
            MaxTapBiasLoop, DB_DFE_VREF_MARGIN_START, DB_DFE_VREF_MARGIN_STEP_SIZE,
            DB_DFE_VREF_MARGIN_NUM_OF_STEPS);
        }
        for (TapBiasOffsetLoopCount = 0; TapBiasOffsetLoopCount <= MaxTapBiasLoop; TapBiasOffsetLoopCount++) {
          //
          // Program the Tap DFE coefficent.
          //
          ProgramDfeTapBias (Host, Socket, DfeTap, TapBiasOffsetLoopCount, DfeTapSweepStepSize, DfeTapSweepRange, IterationLoop, TapBiasSweepBegin, SWEEP_PROGRAM_MODE, NULL);
          SetEyeSizeScalingDfeDdr5Sim (Host, Socket, DfeTap, TapBiasOffsetLoopCount, IterationLoop, GroupInput);
          //
          // DRAM DFE mode register update delay time.
          //
          FixedDelayMicroSecond (1);
          //
          // Run the test to get the figure of merit.
          //
          GetFigureOfMerit (Host, Socket, Group, TGroup, TLocalGroup, TapBiasOffsetLoopCount, PerBit, FigureOfMeritTap, TAP_TEST, DfeTap);


          for (Ch = 0; Ch < MaxChDdr; Ch++) {
            if ((*ChannelNvList) [Ch].enabled == 0) {
              continue;
            }
            LowMargin = -MARGIN_LIMIT;
            HighMargin = MARGIN_LIMIT;
            DimmNvList = GetDimmNvList (Host, Socket, Ch);
            for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
              RankList = GetRankNvList (Host, Socket, Ch, Dimm);
              for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
                if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
                  continue;
                }
                if (IsLrdimmPresent (Socket, Ch, Dimm) && (Rank > 0) && (Group == TxVref)) {
                  continue;
                }
                TxDqWaAdv (Socket, Ch, SUB_CH_A, Dimm, Rank, TRUE);
                TxDqWaAdv (Socket, Ch, SUB_CH_B, Dimm, Rank, TRUE);
              } // rank loop
            } // dimm loop
          } // ch loop
        }//TapBiasOffsetLoopCount
        //
        // Get the best tap bias setting
        //
        //
        // Program the best tap gain bias.
        //
        if (GroupInput == TxDfe) {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "TxDfe Program the best tap gain bias\n");
        } else if (GroupInput == RxDfe) {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "RxDfe Program the best tap gain bias\n");
        } else {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "DbDfe Program the best tap gain bias\n");
        }
        for (Bit = 0; Bit < MBV; Bit++) {
          BestFigureofMerit = 0;
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if (IsBitNotValid (Host, Bit)) {
            continue;
          }
          for (Ch = 0; Ch < MAX_CH; Ch++) {
            //
            // Skip if channel is disabled
            //
            if (IsChannelEnabled (Socket, Ch) == FALSE) {
              continue;
            }
            VrefHi = 0x5F;
            VrefLo = -0x5F;
            DimmNvList = GetDimmNvList (Host, Socket, Ch);
            for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
              //
              // Skip if no DIMM present
              //
              if ((*DimmNvList) [Dimm].dimmPresent == 0) {
                continue;
              }
              RankList = GetRankNvList (Host, Socket, Ch, Dimm);

              if (Group == DbDfeVrefGroup) {
                MaxRankPerDimm = 1;
              } else {
                MaxRankPerDimm = (*DimmNvList) [Dimm].numRanks;
              }
              for (Rank = 0; Rank < MaxRankPerDimm; Rank++) {
                //
                // Skip if no rank
                //
                if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
                  continue;
                }
                if (Filtering == 1) {
                  TapBiasBestCore = (*FigureOfMeritTap) [Ch][ (*RankList) [Rank].rankIndex][Bit][0].FigureofMerit +
                    (*FigureOfMeritTap) [Ch][ (*RankList) [Rank].rankIndex][Bit][0].FigureofMerit +
                    (*FigureOfMeritTap) [Ch][ (*RankList) [Rank].rankIndex][Bit][0].FigureofMerit;
                } else if (Filtering == 3) {
                  TapBiasBestCore = (*FigureOfMeritTap) [Ch][ (*RankList) [Rank].rankIndex][Bit][0].FigureofMerit +
                    (*FigureOfMeritTap) [Ch][ (*RankList) [Rank].rankIndex][Bit][0].FigureofMerit +
                    (*FigureOfMeritTap) [Ch][ (*RankList) [Rank].rankIndex][Bit][1].FigureofMerit;
                }
                BestFigureofMerit = (*FigureOfMeritTap) [Ch][ (*RankList) [Rank].rankIndex][Bit][0].FigureofMerit;
                (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTap][IterationLoop].TapBiasValue[DfeTap] = TapBiasSweepBegin;
                (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTap][IterationLoop].FigureofMerit = BestFigureofMerit;
                (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTap][IterationLoop].Score = TapBiasBestCore;
                LastBestTapBiasValue = TapBiasSweepBegin;
                RestoreOriginalTapBias = TRUE;
                for (TapBiasOffsetLoopCount = 0; TapBiasOffsetLoopCount <= MaxTapBiasLoop; TapBiasOffsetLoopCount++) {
                  if ((*FigureOfMeritTap) [Ch][ (*RankList) [Rank].rankIndex][Bit][TapBiasOffsetLoopCount].FigureofMerit != 0) {
                    RestoreOriginalTapBias = FALSE;
                  }
                  //
                  // Find the best figure of merit
                  //
                  if (Filtering == 1) {
                    if (BestFigureofMerit < (*FigureOfMeritTap) [Ch][ (*RankList) [Rank].rankIndex][Bit][TapBiasOffsetLoopCount].FigureofMerit) {
                      BestFigureofMerit = (*FigureOfMeritTap) [Ch][ (*RankList) [Rank].rankIndex][Bit][TapBiasOffsetLoopCount].FigureofMerit;
                      (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTap][IterationLoop].TapBiasValue[DfeTap] = TapBiasSweepBegin + (TapBiasOffsetLoopCount * DfeTapSweepStepSize);
                      (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTap][IterationLoop].FigureofMerit = BestFigureofMerit;
                    }
                  } else if (Filtering == 3) {
                    if (TapBiasOffsetLoopCount == 0) {
                      TapBiasScoreTemp = (*FigureOfMeritTap) [Ch][ (*RankList) [Rank].rankIndex][Bit][0].FigureofMerit +
                        (*FigureOfMeritTap) [Ch][ (*RankList) [Rank].rankIndex][Bit][0].FigureofMerit +
                        (*FigureOfMeritTap) [Ch][ (*RankList) [Rank].rankIndex][Bit][1].FigureofMerit;
                      if ((*FigureOfMeritTap) [Ch][ (*RankList) [Rank].rankIndex][Bit][0].FigureofMerit != 0) {
                        BestTapBiasCheck = TRUE;
                      } else {
                        BestTapBiasCheck = FALSE;
                      }
                    } else if ((TapBiasOffsetLoopCount > 0) && (TapBiasOffsetLoopCount < MaxTapBiasLoop)) {
                      TapBiasScoreTemp = (*FigureOfMeritTap) [Ch][ (*RankList) [Rank].rankIndex][Bit][TapBiasOffsetLoopCount - 1].FigureofMerit +
                        (*FigureOfMeritTap) [Ch][ (*RankList) [Rank].rankIndex][Bit][TapBiasOffsetLoopCount].FigureofMerit +
                        (*FigureOfMeritTap) [Ch][ (*RankList) [Rank].rankIndex][Bit][TapBiasOffsetLoopCount + 1].FigureofMerit;

                      if ((*FigureOfMeritTap) [Ch][ (*RankList) [Rank].rankIndex][Bit][TapBiasOffsetLoopCount].FigureofMerit != 0) {
                        BestTapBiasCheck = TRUE;
                      } else {
                        BestTapBiasCheck = FALSE;
                      }
                    } else if (TapBiasOffsetLoopCount == MaxTapBiasLoop) {
                      TapBiasScoreTemp = (*FigureOfMeritTap) [Ch][ (*RankList) [Rank].rankIndex][Bit][TapBiasOffsetLoopCount - 1].FigureofMerit +
                        (*FigureOfMeritTap) [Ch][ (*RankList) [Rank].rankIndex][Bit][TapBiasOffsetLoopCount].FigureofMerit +
                        (*FigureOfMeritTap) [Ch][ (*RankList) [Rank].rankIndex][Bit][TapBiasOffsetLoopCount].FigureofMerit;

                      if ((*FigureOfMeritTap) [Ch][ (*RankList) [Rank].rankIndex][Bit][TapBiasOffsetLoopCount].FigureofMerit != 0) {
                        BestTapBiasCheck = TRUE;
                      } else {
                        BestTapBiasCheck = FALSE;
                      }
                    }
                    if (BestTapBiasCheck == TRUE) {
                      if (TapBiasScoreTemp > TapBiasBestCore) {
                        TapBiasBestCore = TapBiasScoreTemp;
                        BestFigureofMerit = (*FigureOfMeritTap) [Ch][ (*RankList) [Rank].rankIndex][Bit][TapBiasOffsetLoopCount].FigureofMerit;
                        (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTap][IterationLoop].TapBiasValue[DfeTap] = TapBiasSweepBegin + (TapBiasOffsetLoopCount * DfeTapSweepStepSize);
                        (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTap][IterationLoop].FigureofMerit = BestFigureofMerit;
                        (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTap][IterationLoop].Score = TapBiasBestCore;
                        LastBestTapBiasValue = (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTap][IterationLoop].TapBiasValue[DfeTap];
                        if (IterationLoop < MAX_ITERATION_LOOP - 1) {
                          (*TapBiasCenter) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTap][IterationLoop + 1] = (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTap][IterationLoop].TapBiasValue[DfeTap];
                        }
                      } else if (TapBiasScoreTemp == TapBiasBestCore) {
                        //
                        // If the two tap bias value have the same figure of merit, will choose the one that is close to 0.
                        //
                        if (ABS (LastBestTapBiasValue - 0) > ABS (TapBiasSweepBegin + (TapBiasOffsetLoopCount * DfeTapSweepStepSize) - 0)) {
                          TapBiasBestCore = TapBiasScoreTemp;
                          BestFigureofMerit = (*FigureOfMeritTap) [Ch][ (*RankList) [Rank].rankIndex][Bit][TapBiasOffsetLoopCount].FigureofMerit;
                          (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTap][IterationLoop].TapBiasValue[DfeTap] = TapBiasSweepBegin + (TapBiasOffsetLoopCount * DfeTapSweepStepSize);
                          (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTap][IterationLoop].FigureofMerit = BestFigureofMerit;
                          (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTap][IterationLoop].Score = TapBiasBestCore;
                          //
                          // Record the best tap bias value.
                          //
                          LastBestTapBiasValue = (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTap][IterationLoop].TapBiasValue[DfeTap];
                          if (IterationLoop < MAX_ITERATION_LOOP - 1) {
                            (*TapBiasCenter) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTap][IterationLoop + 1] = (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTap][IterationLoop].TapBiasValue[DfeTap];
                          }
                        }
                      }
                    }//if check
                  }
                }
                //
                // Initialzie the Tap Bias value for the iteration loop
                //
                BestTapBiasValueTemp = (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTap][IterationLoop].TapBiasValue[DfeTap];
                for (DfeTapTemp = DfeTap; DfeTapTemp <= TAP4_INDEX; DfeTapTemp++) {
                  (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTapTemp][IterationLoop].TapBiasValue[DfeTap] = BestTapBiasValueTemp;
                  if ((RestoreOriginalTapBias == TRUE) && (GroupInput == TxDfe)) {
                    //
                    // Restore the DRAM DFE Tap Bias value as 0.
                    //
                    (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTapTemp][IterationLoop].TapBiasValue[DfeTap] = 0;
                  }
                }


                MaxStrobe = GetMaxStrobe (Host, Socket, Ch, 0, RxVref, DdrLevel);
                if (MaxStrobe != 0) {
                  Strobe = (((Bit % MAX_BITS_IN_BYTE) / (MBV / MaxStrobe) * MSVx8) + (Bit / MAX_BITS_IN_BYTE));
                  BitPerNibble = Bit % (MBV / MaxStrobe);
                }

                DfeTapSweepBegin = TapBiasSweepBegin;
              } // rank loop
            } // dimm loop
          } // ch loop
        } // bit loop
        //
        // Program the Best Tap Bias Value for this Tap
        //

        ProgramDfeTapBias (Host, Socket, DfeTap, 0, 0, 0, IterationLoop, 0, NORMAL_PROGRAM_MODE, BestTapBias);
        //
        // Display the training value
        //
#ifdef DEBUG_CODE_BLOCK
        if (GetDebugLevel () & SDBG_MAX) {
          DisplayDfeTapSweepResults (Host, Socket, TapBiasCenter, DfeTapSweepRange, DfeTapSweepStepSize, FigureOfMeritTap, BestTapBias, DfeTap, IterationLoop, DfeGainBias, MaxTapBiasLoop, TapBiasSweepBegin, GroupInput);
        }
#endif // DEBUG_CODE_BLOCK

      }//DfeTap loop
      //
      // Check the break loop condition.
      //
      if (IterationLoop >= NO_CHANGE_ITERATIONS - 1) {
        AllChDone = 1;
        for (Ch = 0; Ch < MAX_CH; Ch++) {
          //
          // Skip if channel is disabled
          //
          if ((*ChannelNvList) [Ch].enabled == 0) {
            continue;
          }
          VrefHi = 0x5F;
          VrefLo = -0x5F;
          DimmNvList = GetDimmNvList (Host, Socket, Ch);
          RankDone = 1;
          for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
            //
            // Skip if no DIMM present
            //
            if ((*DimmNvList) [Dimm].dimmPresent == 0) {
              continue;
            }
            RankList = GetRankNvList (Host, Socket, Ch, Dimm);
            if (Group == DbDfeVrefGroup) {
              MaxRankPerDimm = 1;
            } else {
              MaxRankPerDimm = (*DimmNvList) [Dimm].numRanks;
            }
            for (Rank = 0; Rank < MaxRankPerDimm; Rank++) {
              //
              // Skip if no rank
              //
              if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
                continue;
              }
              BitDone = 0;
              for (Bit = 0; Bit < MBV; Bit++) {
                BestFigureofMerit = 0;
                //
                // Skip if this is an ECC strobe when ECC is disabled
                //
                if (IsBitNotValid (Host, Bit)) {
                  continue;
                }
                if ((*ChRankBitDone) [Ch][ (*RankList) [Rank].rankIndex][Bit] == 1) {
                  BitDone = BitDone + 1;
                  continue;
                }
                MaxFigureofMerit = (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][TAP1_INDEX][0].FigureofMerit;
                for (IterationLoopTemp = 0; IterationLoopTemp <= IterationLoop ; IterationLoopTemp++) {
                  for (DfeTap = TAP1_INDEX; DfeTap <= TAP4_INDEX; DfeTap++) {

                    if ((*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTap][IterationLoopTemp].FigureofMerit > MaxFigureofMerit) {
                      MaxFigureofMerit = (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTap][IterationLoopTemp].FigureofMerit;
                    }
                  }
                }
                if (MaxFigureofMerit > FIGURE_OF_MERIT_MASK) {
                  (*ChRankBitDone) [Ch][ (*RankList) [Rank].rankIndex][Bit] = 1;
                  BitDone = BitDone + 1;
                } else {
                  MaxTapBias = (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][TAP1_INDEX ][IterationLoop].TapBiasValue[TAP1_INDEX ];
                  MinTapBias = (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][TAP1_INDEX ][IterationLoop].TapBiasValue[TAP1_INDEX ];
                  for (IterationLoopTemp = IterationLoop - (NO_CHANGE_ITERATIONS - 1); IterationLoopTemp <= IterationLoop; IterationLoopTemp++) {
                    for (DfeTap = TAP1_INDEX; DfeTap <= TAP4_INDEX; DfeTap++) {
                      if (MaxTapBias < (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTap][IterationLoop].TapBiasValue[DfeTap]) {
                        MaxTapBias = (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTap][IterationLoop].TapBiasValue[DfeTap];
                      }
                      if (MinTapBias > (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTap][IterationLoop].TapBiasValue[DfeTap]) {
                        MinTapBias = (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTap][IterationLoop].TapBiasValue[DfeTap];
                      }
                    } //DfeTap
                  }
                  if ((MaxTapBias - MinTapBias) < TAP_CHANGE_THRESHOLD) {
                    (*ChRankBitDone) [Ch][ (*RankList) [Rank].rankIndex][Bit] = 1;
                    BitDone = BitDone + 1;
                  }
                }
                //
                // Need to update the starting Tx/Rx Vref offsets for later use by GetMargins.
                //
              } // bit loop
              if (Host->nvram.mem.eccEn) {
                if (BitDone == MBV) {
                  (*ChRankDone) [Ch][ (*RankList) [Rank].rankIndex] = 1;
                } else {
                  (*ChRankDone) [Ch][ (*RankList) [Rank].rankIndex] = 0;
                  RankDone = 0;
                }
              } else {
                if (BitDone == MBV - 16) {
                  (*ChRankDone) [Ch][ (*RankList) [Rank].rankIndex] = 1;
                } else {
                  (*ChRankDone) [Ch][ (*RankList) [Rank].rankIndex] = 0;
                  RankDone = 0;
                }
              }
            }// rank loop
          } // dimm loop
          if (RankDone == 1) {
            ChDone[Ch] = 1;
          } else {
            ChDone[Ch] = 0;
            AllChDone = 0;
          }
        } // ch loop

        if (AllChDone == 1) {
          //
          // All the Ch, Rank Bits' figure of merit are greater than the FIGURE_OF_MERIT_MASK
          //
          break;
        }
      } //if check
    } //IterationLoop
    if ((AllChDone == 1) && (MAX_ITERATION_LOOP != 1)) {
      //
      // Get the best tap bias value and program it into the register.
      // Program the final trained value
      //
      for (Ch = 0; Ch < MAX_CH; Ch++) {
        //
        // Skip if channel is disabled
        //
        if ((*ChannelNvList) [Ch].enabled == 0) {
          continue;
        }
        VrefHi = 0x5F;
        VrefLo = -0x5F;
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
          //
          // Skip if no DIMM present
          //
          if ((*DimmNvList) [Dimm].dimmPresent == 0) {
            continue;
          }
          RankList = GetRankNvList (Host, Socket, Ch, Dimm);

          if (Group == DbDfeVrefGroup) {
            MaxRankPerDimm = 1;
          } else {
            MaxRankPerDimm = (*DimmNvList) [Dimm].numRanks;
          }
          for (Rank = 0; Rank < MaxRankPerDimm; Rank++) {
            //
            // Skip if no rank
            //
            if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
              continue;
            }
            for (Bit = 0; Bit < MBV; Bit++) {
              BestFigureofMerit = 0;
              //
              // Skip if this is an ECC strobe when ECC is disabled
              //
              if (IsBitNotValid (Host, Bit)) {
                continue;
              }
              MaxFigureofMerit = (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][TAP1_INDEX ][0].FigureofMerit;
              MaxTapBiasScore = (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][TAP1_INDEX ][0].Score;
              FinalBestTap = TAP1_INDEX;
              if (IterationLoop < MAX_ITERATION_LOOP) {
                IterationLoop2 = IterationLoop;
              } else {
                IterationLoop2 = MAX_ITERATION_LOOP - 1;
              }

              for (IterationLoopTemp = 0; IterationLoopTemp <= IterationLoop2 ; IterationLoopTemp++) {
                for (DfeTap = TAP1_INDEX; DfeTap <= TAP4_INDEX; DfeTap++) {
                  if ((*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTap][IterationLoopTemp].Score > MaxTapBiasScore) {
                    MaxFigureofMerit = (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTap][IterationLoopTemp].FigureofMerit;
                    MaxTapBiasScore = (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][DfeTap][IterationLoopTemp].Score;
                    FinalBestTap = DfeTap;
                    FinalBestIterationLoop = IterationLoopTemp;
                  }
                }
              }
              RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, Bit,
                "The best Iteration Loop %d, Tap %d\n", FinalBestIterationLoop, FinalBestTap + 1);
              for (TapIndex = 0; TapIndex < MAX_TAP_NUM; TapIndex++) {
                (*FinalTapBiasValue) [Ch][ (*RankList) [Rank].rankIndex][Bit].TapBiasValue[TapIndex] = (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][Bit][FinalBestTap][FinalBestIterationLoop].TapBiasValue[TapIndex];
              }
              //
              // Program the final results.
              //
              ProgramDfeTapBiasPerbit (Host, Socket, Ch, Dimm, Rank, Bit, FinalTapBiasValue);
            } // bit loop
          }// rank loop
        } // dimm loop
      } // ch loop
      //
      // display the final trained value
      //
      DisplayDfeTapFinalResults (Host, Socket, Group, FinalTapBiasValue);
      break;
    }
  } //DfeGainBias loop
  RcDebugPrint (SDBG_DEFAULT, "Repeatability Test Start 2nd\n");
  //
  // Get the figure of merit at the end of training.
  //
  GetFigureOfMerit (Host, Socket, Group, TGroup, TLocalGroup, TapBiasOffsetLoopCount, PerBit, FigureOfMeritTap, TAP_TEST, DfeTap);
  RcDebugPrint (SDBG_DEFAULT, "Repeatability Test End 2nd\n");
  //
  // Track the best tap bias settings for each gain bias loop.
  //
  //
  // After going through the whole sweep loop, still some DQ can't meet the requirement, treat them as a fail and halt here?: keep going ahead and program
  // the best setting or last settings for those DQs.
  //
  RcDebugPrintWithDevice (SDBG_TRACE, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Update starting Tx/Rx Vref offsets for later use by GetMargin\n");
  //
  // Update starting Tx/Rx Vref offsets for later use by GetMargins
  //
  UpdateStartingOffset (Host, Socket, Group, DdrLevel, SCOPE_RANK, (VOID *) RankResults);
  if (Group == TxVref) {
    UpdateSafeTxVref (Host, Socket, DdrLevel);
  }
#ifdef DEBUG_CODE_BLOCK
  RcDebugPrint (SDBG_MEM_TRAIN, "\nNew Settings");
  if (Group == RxSamplerEvenOdd) {
    DisplayResultsDdr5 (Host, Socket, RxSamplerEven);
    DisplayResultsDdr5 (Host, Socket, RxSamplerOdd);
    DisplayResultsDdr5 (Host, Socket, RxDqsBitDelay);
  } else if (Group == RxVref) {
    DisplayResultsDdr5 (Host, Socket, RxVref);
    DisplayResultsDdr5 (Host, Socket, RxDqsBitDelay);
  } else if (Group == TxVref) {
    DisplayResultsDdr5 (Host, Socket, TxVref);
    DisplayResultsDdr5 (Host, Socket, TxDqBitDelay);
  }
#endif //DEBUG_CODE_BLOCK
  if (GroupInput == DbDfe) {
    DbDfeCleanUp  (Host, Socket, DbDfeTapAll);
  }
  RcFreePool (VrefMarginBit);
  RcFreePool (FigureOfMeritTap);
  RcFreePool (BestTapBias);
  RcFreePool (TapBiasCenter);
  RcFreePool (FinalTapBiasValue);
  RcFreePool (DfeTapBiasSweepRangeInitValue);
  RcFreePool (ChRankDone);
  RcFreePool (ChRankBitDone);
  RcFreePool (Filter);
  RcFreePool (RankResults);
  RcFreePool (Results);

  IO_Reset (Host, Socket);
  FifoTrainReset (Host, Socket);

  return SUCCESS;
} // DfeTrainingDdr5

#ifdef LRDIMM_SUPPORT
/**

  Retrieves the register offset for Tap 4 based on the DQ number.

  @param[in]  DqNumber     - DQ number to be programmed
  @param[out] TapCoeffReg  - Pointer to register offset

  @retval N/A

**/
VOID
GetDbTap4Reg (
  IN  UINT8   DqNumber,
  OUT UINT16  *TapCoeffReg
  )
{

  // DQ (0,4), (1,5) ... have same register offsets - only difference is
  // page number which is accounted for elsewhere.
  switch (DqNumber) {
    case 0:
    case 4:
      *TapCoeffReg = DDR5_DATA_BUFFER_PG4_RWE4_REG;
      break;

    case 1:
    case 5:
      *TapCoeffReg = DDR5_DATA_BUFFER_PG4_RWEC_REG;
      break;

    case 2:
    case 6:
      *TapCoeffReg = DDR5_DATA_BUFFER_PG4_RWF4_REG;
      break;

    case 3:
    case 7:
      *TapCoeffReg = DDR5_DATA_BUFFER_PG4_RWFC_REG;
      break;

    default:
      // Should never get here
      RcDebugPrint (SDBG_MAX, "Invalid DQ Number input into GetTap3Reg\n");
  }
}

/**

  Retrieves the register offset for Tap 3 based on the DQ number.

  @param[in]  DqNumber     - DQ number to be programmed
  @param[out] TapCoeffReg  - Pointer to register offset

  @retval N/A

**/
VOID
GetDbTap3Reg (
  IN  UINT8   DqNumber,
  OUT UINT16  *TapCoeffReg
  )
{

  // DQ (0,4), (1,5) ... have same register offsets - only difference is
  // page number which is accounted for elsewhere.
  switch (DqNumber) {
    case 0:
    case 4:
      *TapCoeffReg = DDR5_DATA_BUFFER_PG4_RWE3_REG;
      break;

    case 1:
    case 5:
      *TapCoeffReg = DDR5_DATA_BUFFER_PG4_RWEB_REG;
      break;

    case 2:
    case 6:
      *TapCoeffReg = DDR5_DATA_BUFFER_PG4_RWF3_REG;
      break;

    case 3:
    case 7:
      *TapCoeffReg = DDR5_DATA_BUFFER_PG4_RWFB_REG;
      break;

    default:
      // Should never get here
      RcDebugPrint (SDBG_MAX, "Invalid DQ Number input into GetTap3Reg\n");
  }
}

/**

  Retrieves the register offset for Tap 2 based on the DQ number.

  @param[in]  DqNumber     - DQ number to be programmed
  @param[out] TapCoeffReg  - Pointer to register offset

  @retval N/A

**/
VOID
GetDbTap2Reg (
  IN  UINT8   DqNumber,
  OUT UINT16  *TapCoeffReg
  )
{

  // DQ (0,4), (1,5) ... have same register offsets - only difference is
  // page number which is accounted for elsewhere.
  switch (DqNumber) {
    case 0:
    case 4:
      *TapCoeffReg = DDR5_DATA_BUFFER_PG4_RWE2_REG;
      break;

    case 1:
    case 5:
      *TapCoeffReg = DDR5_DATA_BUFFER_PG4_RWEA_REG;
      break;

    case 2:
    case 6:
      *TapCoeffReg = DDR5_DATA_BUFFER_PG4_RWF2_REG;
      break;

    case 3:
    case 7:
      *TapCoeffReg = DDR5_DATA_BUFFER_PG4_RWFA_REG;
      break;

    default:
      // Should never get here
      RcDebugPrint (SDBG_MAX, "Invalid DQ Number input into GetTap2Reg\n");
  }
}

/**

  Retrieves the register offset for Tap 1 based on the DQ number.

  @param[in]  DqNumber     - DQ number to be programmed
  @param[out] TapCoeffReg  - Pointer to register offset

  @retval N/A

**/
VOID
GetDbTap1Reg (
  IN  UINT8   DqNumber,
  OUT UINT16  *TapCoeffReg
  )
{

  // DQ (0,4), (1,5) ... have same register offsets - only difference is
  // page number which is accounted for elsewhere.
  switch (DqNumber) {
    case 0:
    case 4:
      *TapCoeffReg = DDR5_DATA_BUFFER_PG4_RWE1_REG;
      break;

    case 1:
    case 5:
      *TapCoeffReg = DDR5_DATA_BUFFER_PG4_RWE9_REG;
      break;

    case 2:
    case 6:
      *TapCoeffReg = DDR5_DATA_BUFFER_PG4_RWF1_REG;
      break;

    case 3:
    case 7:
      *TapCoeffReg = DDR5_DATA_BUFFER_PG4_RWF9_REG;
      break;

    default:
      // Should never get here
      RcDebugPrint (SDBG_MAX, "Invalid DQ Number input into GetTap1Reg\n");
  }
}

/**

  Returns the Register offset and Register data to be programmed for a
  given DB Tap number.

  @param[in]  TapNumber        - Tap number to be programmed
  @param[in]  Bit              - Bit in SubChannel to be programmed
  @param[out] DbPage           - Pointer to DB page number
  @param[out] TapCoeffReg      - Pointer to Register offset
  @param[out] TapCoeffRegData  - Pointer to data to be programmed
  @param[in]  CoeffValue       - Coefficient value to program

  @retval N/A

**/
VOID
GetDbTapRegInfo (
  IN  UINT8        DfeTapIndex,
  IN  UINT8        DqBit,
  OUT UINT8       *DbPage,
  OUT UINT16      *TapCoeffReg,
  OUT UINT8       *TapCoeffRegData,
  IN  INT8         CoeffValue
  )
{
  DDR5_DATA_BUFFER_PG4_RWE1_STRUCT  Tap1;
  DDR5_DATA_BUFFER_PG4_RWE2_STRUCT  Tap2;
  DDR5_DATA_BUFFER_PG4_RWE3_STRUCT  Tap3;
  DDR5_DATA_BUFFER_PG4_RWE4_STRUCT  Tap4;

  // If DQ number is 0-3, Page number is 4. Otherwise 5.
  if (DqBit < 4) {
    *DbPage = LRDIMM_DB_PAGE4;
  } else {
    *DbPage = LRDIMM_DB_PAGE5;
  }

  switch (DfeTapIndex) {
    case TAP1_INDEX:
      // Retrieve register offset
      GetDbTap1Reg (DqBit, TapCoeffReg);

      // Program data
      Tap1.Data = 0;
      Tap1.Bits.tap_1_dfe_coefficient = (UINT8) (ABS (CoeffValue));
      if (CoeffValue < 0) {
        Tap1.Bits.tap_1_coefficient_sign_bit = 1;
      }
      *TapCoeffRegData = Tap1.Data;
      break;

    case TAP2_INDEX:
      // Retrieve register offset
      GetDbTap2Reg (DqBit, TapCoeffReg);

      // Program data
      Tap2.Data = 0;
      Tap2.Bits.tap_2_dfe_coefficient = (UINT8) (ABS (CoeffValue));
      if (CoeffValue < 0) {
        Tap2.Bits.tap_2_coefficient_sign_bit = 1;
      }
      *TapCoeffRegData = Tap2.Data;
      break;

    case TAP3_INDEX:
      // Retrieve register offset
      GetDbTap3Reg (DqBit, TapCoeffReg);

      // Program data
      Tap3.Data = 0;
      Tap3.Bits.tap_3_dfe_coefficient = (UINT8) (ABS (CoeffValue));
      if (CoeffValue < 0) {
        Tap3.Bits.tap_3_coefficient_sign_bit = 1;
      }
      *TapCoeffRegData = Tap3.Data;
      break;

    case TAP4_INDEX:
      // Retrieve register offset
      GetDbTap4Reg (DqBit, TapCoeffReg);

      // Program data
      Tap4.Data = 0;
      Tap4.Bits.tap_4_dfe_coefficient = (UINT8) (ABS (CoeffValue));
      if (CoeffValue < 0) {
        Tap4.Bits.tap_4_coefficient_sign_bit = 1;
      }
      *TapCoeffRegData = Tap4.Data;
      break;

    default:
      // Should never get here
      RcDebugPrint (SDBG_MAX, "Invalid Tap Number in GetTapRegInfo!\n");
      break;

  }
}

/**

  Retrieves the Frontside swizzle (Host to DB) information and returns the swizzled bit
  within the data buffer (0-7).

  @param[in] Host        - Poiter to sysHost
  @param[in] Channel     - Channel number
  @param[in] Dimm        - DIMM number
  @param[in] SubChannel  - SubChannel number
  @param]in] Bit         - Bit within channel to be programmed
  @param[in] SwizzleBit  - Pointer to value which stores the swizzled bit

  @retval - MRC_STATUS_SUCCESS - no errors
          - MRC_STATUS_FAILURE - error retrieving HostToDbDqMap

**/
MRC_STATUS
GetDbSwizzledBit (
  IN PSYSHOST  Host,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT8     SubChannel,
  IN UINT8     Bit,
  IN UINT8     *SwizzleBit
  )
{
  MRC_STATUS                 Status = MRC_STATUS_SUCCESS;
  UINT8                      Dword;
  UINT8                      DataBuffer;
  UINT8                      MaxValidBitsSubCh;
  UINT8                      BitInSubCh;
  UINT8                      DbDqLane;
  UINT8                      SwizzleMask = 0x7;  // Each entry in HostToDb is 3 bits wide
  DDR5_LRDIMM_TRAINING_DATA  *TrainingData;
  UINT32                     (*HostToDbDqMap) [MAX_CH][MAX_DIMM][SUB_CH][SWIZZLE_RESULTS_LEN];

  // Retrieve Host to DB DQ swizzle mapping
  Status = GetDdr5LrdimmTrainingData (&TrainingData);
  if (Status != MRC_STATUS_SUCCESS) {
    return Status;
  }
  HostToDbDqMap = &(TrainingData->HostToDbDqMap);

  MaxValidBitsSubCh = GetMaxBitsValid (Host) / 2;

  // Find appropriate data buffer (each data buffer has 8 DQ lanes)
  BitInSubCh = Bit % MaxValidBitsSubCh;
  DataBuffer = BitInSubCh / 8;
  DbDqLane = BitInSubCh % 8;

  // Each Dword can fit 10 entries (3 bits per entry) - find appropriate Dword to store data
  Dword = (DbDqLane + (DataBuffer * DB_DQ_LANE)) / 10;
  // Each data buffer has 8 entries, add in the DbDqLane. There are 10 entries per Dword, and each entry is 3 bits wide.
  *SwizzleBit = (UINT8) ((*HostToDbDqMap) [Channel][Dimm][SubChannel][Dword] >> (((DbDqLane + (DataBuffer * DB_DQ_LANE)) % 10) * 3));
  *SwizzleBit &= SwizzleMask;

  return Status;
}


/**

  Sets the Tap Coefficient value for a given Tap number.

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket number
  @param[in] Channel     - Channel number
  @param[in] SubChannel  - SubChannel number
  @param[in] Dimm        - DIMM number
  @param[in] Rank        - Rank number on DIMM
  @param[in] Bit         - Bit in channel
  @param[in] Mode        - Bit-field of read/write modes
  @param[in] TapNumber   - Tap number to be programmed
  @param[in] CoeffValue  - Coefficient value to be programmed

  @retval  - MRC_STATUS_SUCCESS - programming successful
           - MRC_STATUS_INVALID_PARAMETER - bad input paramater

**/
MRC_STATUS
SetDbDfeTapCoefficient (
  IN PSYSHOST     Host,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN UINT8        Dimm,
  IN UINT8        Rank,
  IN UINT8        Bit,
  IN UINT8        DfeTapIndex,
  IN INT8         CoeffValue
  )
{
  MRC_STATUS  Status = MRC_STATUS_SUCCESS;
  UINT8       SubChannel;
  UINT8       MaxValidBits;
  UINT8       MaxValidBitsSubCh;
  UINT8       BitInSubCh;
  UINT8       SwizzleBit;
  UINT8       DataBuffer;
  UINT8       DbPage;
  UINT8       TapCoeffRegData;
  UINT16      TapCoeffReg;

  MaxValidBits = GetMaxBitsValid (Host);
  MaxValidBitsSubCh = GetMaxBitsValid (Host) / 2;

  if (DfeTapIndex > TAP4_INDEX) {
    RcDebugPrint (SDBG_MAX, "GetSetDbDfeTapCoefficient Tap Number is out of range!\n");
    return MRC_STATUS_INVALID_PARAMETER;
  }

  if (Bit >= MaxValidBits) {
    RcDebugPrint (SDBG_MAX, "GetSetDbDfeTapCoefficient Bit Number is out of range!\n");
    return MRC_STATUS_INVALID_PARAMETER;
  }

  // Find SubChannel - 40 bits per SubChannel
  SubChannel = Bit / MaxValidBitsSubCh;
  BitInSubCh = Bit % MaxValidBitsSubCh;

  // Select Appropriate data buffer (each data buffer has 8 DQ lanes)
  DataBuffer = BitInSubCh / 8;

  // Retrieve the swizzled bit (0-7 based) based on the input bit
  GetDbSwizzledBit (Host, Channel, Dimm, SubChannel, Bit, &SwizzleBit);

  GetDbTapRegInfo (DfeTapIndex, SwizzleBit, &DbPage, &TapCoeffReg, &TapCoeffRegData, CoeffValue);

  // Select appropriate data buffer
  DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, DataBuffer);

  // Select page
  DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, RDIMM_RWDF, DbPage);

  // Program Coefficient
  DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, (UINT8) TapCoeffReg, TapCoeffRegData);

  // Select all devices
  DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);

  return Status;
}

/**

  Programs the DB DFE Gain offset to the input GainValue for all DQs on all Data Buffers.

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket number
  @param[in] Channel     - Channel number
  @param[in] Dimm        - DIMM number
  @param[in] Rank        - Rank number on DIMM
  @param[in] GainValue   - Gain Value (Register based - 0-7 are valid values)

  retval N/A

**/
VOID
ProgramDbGainOnAllBits (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     GainValue
  )
{
  UINT8                             RegIndex;
  DDR5_DATA_BUFFER_PG4_RWE0_STRUCT  DbGain;
  UINT8                             DbDfeGainReg[] = {DDR5_DATA_BUFFER_PG4_RWE0_REG,
                                                      DDR5_DATA_BUFFER_PG4_RWE8_REG,
                                                      DDR5_DATA_BUFFER_PG4_RWF0_REG,
                                                      DDR5_DATA_BUFFER_PG4_RWF8_REG};

  DbGain.Data = 0;
  DbGain.Bits.flat_band_dc_gain_adjustment = GainValue;

  // Selet all devices
  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);

  // Register offsets are the same for Page 4 and Page 5 (E0, E8, F0, F8)
  // First 4 DQs are programmed via Page 4
  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF,  LRDIMM_DB_PAGE4);
  // Program DQ 0-3 to Gain value
  for (RegIndex = 0; RegIndex < ARRAY_SIZE (DbDfeGainReg); RegIndex++) {
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DbDfeGainReg[RegIndex],  DbGain.Data);
  }

  // Last 4 DQs are programmed via page 5
  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, LRDIMM_DB_PAGE5);
  // Program DQ 4-7 to Gain value
  for (RegIndex = 0; RegIndex < ARRAY_SIZE (DbDfeGainReg); RegIndex++) {
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DbDfeGainReg[RegIndex], DbGain.Data);
  }
}

/**

  Gets the DB DFE Gain value to be programmed in the register based on actual
  Gain Adjustment values (+6, + 4, + 2, 0, -2, -4, -6) OR converts programmable
  register values to actual Gain Adjustment values.

  @param[in] ToRegValue - TRUE  - convert from actual Gain Adjustment value to reg value
                          FALSE - convert from reg value to actual Gain Adjustment value
  @param[in] GainValue  - Pointer to Gain value to convert

  @retval  - MRC_STATUS_SUCCESS - No error
           - MRC_STATUS_INVALID_PARAMETER - bad input paramater

**/
MRC_STATUS
ConvertDbGainValue (
  IN BOOLEAN  ToRegValue,
  IN INT8     *GainValue
  )
{
  MRC_STATUS  Status = MRC_STATUS_SUCCESS;
  INT8        GainValueTemp;

  // Range conversion: ConvertedValue = (((OrigValue - OrigMin) * ConvertedRange) / OrigRange) + NewMin
  // Ranges are (1 to 7, 3 to -3) or (6, -6), so (ConvertedRange / OrigRange) = -1

  // Convert from actual Gain Adjustment value to programmable register value
  if (ToRegValue == TRUE) {

    // Check input value, must be within range and be even (-6,-4,-2,0,2,4,6)
    if ((*GainValue > MAX_DB_GAIN) || (*GainValue < MIN_DB_GAIN)) {
      return MRC_STATUS_INVALID_PARAMETER;
    }
    if (*GainValue % 2 != 0) {
      return MRC_STATUS_INVALID_PARAMETER;
    }

    // First divide value by 2 to put it in range (-3 to 3)
    GainValueTemp = *GainValue / 2;
    // Apply above range conversion
    GainValueTemp = (GainValueTemp - 3) * -1;
    GainValueTemp = GainValueTemp + 1;

  } else {
    // Convert from Register Value (1 to 7) to Gain Values (-6,-4,-2,0,2,4,6)
    // Check input value
    if ((*GainValue > MAX_DB_GAIN_REG) || (*GainValue < MIN_DB_GAIN_REG)) {
      return MRC_STATUS_INVALID_PARAMETER;
    }
    // If input value is 0, return - a 0 reg value is a 0 Gain Adjustment
    if (*GainValue == 0) {
      return Status;
    }
    // Apply above range conversion
    GainValueTemp = (*GainValue - 1) * -1;
    GainValueTemp = GainValueTemp + 3;
    // Range is now (3 to -3).  Multiply by 2 to get (-6,-4,-2,0,2,4,6)
    GainValueTemp = GainValueTemp * 2;
  }

  *GainValue = GainValueTemp;
  return Status;
}

/**

  Returns the Register offset and Register data to be programmed for a
  given Gain offset value.

  @param[in]  DqBit              - DQ Bit to be programmed
  @param[in]  GainValue          - Gain Value to be programmed
  @param[out] DbPage             - Pointer to DB page number
  @param[out] GainOffsetReg      - Pointer to Register offset
  @param[out] GainOffsetRegData  - Pointer to data to be programmed

  @retval N/A

**/
VOID
GetDbGainRegInfo (
  IN  UINT8   DqBit,
  IN  INT8    GainValue,
  OUT UINT8   *DbPage,
  OUT UINT16  *GainOffsetReg,
  OUT UINT8   *GainOffsetRegData
  )
{
  DDR5_DATA_BUFFER_PG4_RWE0_STRUCT  DbGain;


  // If DQ number is 0-3, Page number is 4. Otherwise 5.
  if (DqBit < 4) {
    *DbPage = LRDIMM_DB_PAGE4;
  } else {
    *DbPage = LRDIMM_DB_PAGE5;
  }

  // Program Gain Value (already in register format) to register data
  DbGain.Data = 0;
  DbGain.Bits.flat_band_dc_gain_adjustment = (UINT8) GainValue;
  *GainOffsetRegData = DbGain.Data;

  switch (DqBit) {
    case 0:
    case 4:
      // DQ 0,4 share same register offset
      *GainOffsetReg = DDR5_DATA_BUFFER_PG4_RWE0_REG;
      break;

    case 1:
    case 5:
      // DQ 1,5 share same register offset
      *GainOffsetReg = DDR5_DATA_BUFFER_PG4_RWE8_REG;
      break;

    case 2:
    case 6:
      // DQ 2,6 share same register offset
      *GainOffsetReg = DDR5_DATA_BUFFER_PG4_RWF0_REG;
      break;

    case 3:
    case 7:
      // DQ 3,7 share same register offset
      *GainOffsetReg = DDR5_DATA_BUFFER_PG4_RWF8_REG;
      break;

    default:
      // Should never get here
      RcDebugPrint (SDBG_MAX, "Invalid DQ Number in GetGainRegInfo\n");
      break;
  }
}

/**

  Sets the DB DFE Gain offset adjustment value.  This function can program all
  bits to a given gain value (all DQs, all DBs to same value) or will accept a
  gain value for a specific bit within the channel (per DQ, per DB).

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number
  @param[in] Channel         - Channel number
  @param[in] Dimm            - DIMM number
  @param[in] Rank            - Rank number
  @param[in] Bit             - Bit within channel to be programmed
  @param[in] IsGainRegFormat - TRUE  - Input gain value is in register format (0-7)
                               FALSE - Input gain value is in offset format (-6,-4,-2,0,2,4,6)
  @param[in] GainValue       - Gain offset value to be programmed

  @retval - MRC_STATUS_SUCCESS - programming successful
**/
MRC_STATUS
SetDbDfeGain (
  IN PSYSHOST     Host,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN UINT8        Dimm,
  IN UINT8        Rank,
  IN UINT8        Bit,
  IN BOOLEAN      IsGainRegFormat,
  IN INT8         GainValue
  )
{
  MRC_STATUS                 Status = MRC_STATUS_SUCCESS;
  UINT8                      SubChannel;
  UINT8                      MaxValidBits;
  UINT8                      MaxValidBitsSubCh;
  UINT8                      BitInSubCh;
  UINT8                      DataBuffer;
  UINT8                      DbPage;
  UINT8                      GainOffsetRegData;
  UINT16                     GainOffsetReg;
  UINT8                      SwizzleBit;

  MaxValidBits = GetMaxBitsValid (Host);
  MaxValidBitsSubCh = GetMaxBitsValid (Host) / 2;

  if ((Bit >= MaxValidBits) && (Bit != ALL_BITS)) {
    RcDebugPrint (SDBG_MAX, "GetSetDbDfeTapCoefficient Bit Number is out of range!\n");
    return MRC_STATUS_INVALID_PARAMETER;
  }

  // If input Gain value is not in register format, convert it
  if (IsGainRegFormat == FALSE) {
    Status = ConvertDbGainValue (TRUE, &GainValue);
    if (Status != MRC_STATUS_SUCCESS) {
      return Status;
    }
  }

  // Make sure Gain value is within range
  if ((GainValue < MIN_DB_GAIN_REG) || (GainValue > MAX_DB_GAIN_REG)) {
    RcDebugPrint (SDBG_MAX, "Invalid Gain Value: %d in SetDbDfeGain!\n", GainValue);
    return MRC_STATUS_INVALID_PARAMETER;
  }

  // If ALL_BITS is selected, brodcast gain value to all data buffers and all DQs
  if (Bit == ALL_BITS) {
    ProgramDbGainOnAllBits (Host, Socket, Channel, Dimm, Rank, GainValue);
    return Status;
  } else {

    // Find SubChannel - 40 bits per SubChannel
    SubChannel = Bit / MaxValidBitsSubCh;
    BitInSubCh = Bit % MaxValidBitsSubCh;
    DataBuffer = BitInSubCh / 8;  // Each DB contains 8 DQ lanes

    // Retrieve the swizzled bit (0-7 based) based on the input bit
    GetDbSwizzledBit (Host, Channel, Dimm, SubChannel, Bit, &SwizzleBit);

    GetDbGainRegInfo (SwizzleBit, GainValue, &DbPage, &GainOffsetReg, &GainOffsetRegData);

    // Select appropriate data buffer
    DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, DataBuffer);

    // Select page
    DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, RDIMM_RWDF, DbPage);

    // Program Gain
    DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, (UINT8) GainOffsetReg, GainOffsetRegData);

    // Select all devices
    DbControlWordWrite (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);
  }

  return Status;
}

/**

  Programs the Inner Loop Start Value Control Words.

  @param[in] Host                - Pointer to sysHost
  @param[in] Socket              - Socket number
  @param[in] Channel             - Channel number
  @param[in] Dimm                - DIMM number
  @param[in] Rank                - Rank number on DIMM
  @param[in] InnerLoopStartValue - Inner Loop Start value to be programmed

  @retval  - MRC_STATUS_SUCCESS - programming successful
           - MRC_STATUS_INVALID_PARAMETER - bad input paramater

**/
MRC_STATUS
ProgramDbTaInnerLoopStart (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN INT16     InnerLoopStartValue
  )
{
  MRC_STATUS                        Status = MRC_STATUS_SUCCESS;
  DDR5_DATA_BUFFER_PG9_RWE1_STRUCT  DfeTaInnerStartE1;
  DDR5_DATA_BUFFER_PG9_RWE2_STRUCT  DfeTaInnerStartE2;

  // Program Inner Loop Start Value (RWE1 RWE2)

  // If input value is greater or less than max programmable value, return with error
  if (InnerLoopStartValue < INNER_LOOP_START_MIN) {
    RcDebugPrint (SDBG_MAX, "DB DFE TA Inner Loop Start value %d less than min value: %d \n",
      InnerLoopStartValue, INNER_LOOP_START_MIN);
    return MRC_STATUS_INVALID_PARAMETER;
  }
  if (InnerLoopStartValue > INNER_LOOP_START_MAX) {
    RcDebugPrint (SDBG_MAX, "DB DFE TA Inner Loop Start value %d greater than max value: %d \n",
      InnerLoopStartValue, INNER_LOOP_START_MAX);
    return MRC_STATUS_INVALID_PARAMETER;
  }
  DfeTaInnerStartE1.Data = 0;
  DfeTaInnerStartE2.Data = 0;

  // If Value is less than 0, set sign bit. If incoming number is negative, it is already in
  // 2's complement form so just truncate off the upper byte.
  if (InnerLoopStartValue < 0) {
    DfeTaInnerStartE2.Bits.inner_loop_start_value = 1;
  }
  DfeTaInnerStartE1.Bits.inner_loop_start_value = (UINT8) InnerLoopStartValue;

  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG9_RWE1_REG, DfeTaInnerStartE1.Data);
  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG9_RWE2_REG, DfeTaInnerStartE2.Data);

  return Status;
}

/**

  Programs the Outer Loop Start Value Control Word.

  @param[in] Host                 - Pointer to sysHost
  @param[in] Socket               - Socket number
  @param[in] Channel              - Channel number
  @param[in] Dimm                 - DIMM number
  @param[in] Rank                 - Rank number on DIMM
  @param[in] OuterLoopStartValue  - Outer Loop Start value to be programmed

  @retval  - MRC_STATUS_SUCCESS - programming successful
           - MRC_STATUS_INVALID_PARAMETER - bad input paramater

**/
MRC_STATUS
ProgramDbTaOuterLoopStart (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN INT8      OuterLoopStartValue
  )
{
  MRC_STATUS                        Status = MRC_STATUS_SUCCESS;
  DDR5_DATA_BUFFER_PG9_RWE3_STRUCT  DfeTaOuterStartE3;

  // Program Outer Loop Start Value (RWE3)

  // If input value is greater then max programmable value, return with error
  if (OuterLoopStartValue > OUTER_LOOP_START_MAX) {
    RcDebugPrint (SDBG_MAX, "DB DFE TA Outer Loop Start value %d greater than max value: %d \n",
      OuterLoopStartValue, OUTER_LOOP_START_MAX);
    return MRC_STATUS_INVALID_PARAMETER;
  }

  DfeTaOuterStartE3.Data = 0;
  // Incoming value is already in 2's complement form.
  DfeTaOuterStartE3.Bits.outer_loop_start_value = (UINT8) OuterLoopStartValue;

  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG9_RWE3_REG, DfeTaOuterStartE3.Data);

  return Status;
}

/**

  Programs the Inner and Outer Loop Step Size Control Word.
  NOTE: A value of 0 may be input for either loop step size and that value of 0 will be programmed,
        however, a value of 0 indicates a step size of 1.  A value of 1 also indicates a step size
        of 1. A value of 16 indicates a step size of 16.

  @param[in] Host               - Pointer to sysHost
  @param[in] Socket             - Socket number
  @param[in] Channel            - Channel number
  @param[in] Dimm               - DIMM number
  @param[in] Rank               - Rank number on DIMM
  @param[in] InnerLoopStepSize  - Inner Loop Step size to be programmed (valid values 0-16)
  @param[in] OuterLoopStepSize  - Outer Loop Step size to be programmed (valid values 0-16)

  @retval  - MRC_STATUS_SUCCESS - programming successful
           - MRC_STATUS_INVALID_PARAMETER - bad input paramater

**/
MRC_STATUS
ProgramDbTaStepSize (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     InnerLoopStepSize,
  IN UINT8     OuterLoopStepSize
  )
{
  MRC_STATUS                        Status = MRC_STATUS_SUCCESS;
  DDR5_DATA_BUFFER_PG9_RWE7_STRUCT  DfeTaStepSizeE7;

  // Program Inner and Outer Loop Step Size (RWE7)

  // Return error if either input value is greater than maximum programmable size
  if ((InnerLoopStepSize > DB_TA_MAX_STEP_SIZE) || (OuterLoopStepSize > DB_TA_MAX_STEP_SIZE)) {
    RcDebugPrint (SDBG_MAX, "DB DFE TA Step Size value greater than max value: %d \n",
      DB_TA_MAX_STEP_SIZE);
    return MRC_STATUS_INVALID_PARAMETER;
  }

  DfeTaStepSizeE7.Data = 0;
  // If input step size is anything but zero, subtract 1 to calculate value to be programmed in register
  if (InnerLoopStepSize != 0) {
    DfeTaStepSizeE7.Bits.inner_loop_step_size = InnerLoopStepSize - 1;
  }
  if (OuterLoopStepSize != 0) {
    DfeTaStepSizeE7.Bits.outer_loop_step_size = OuterLoopStepSize - 1;
  }

  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG9_RWE7_REG, DfeTaStepSizeE7.Data);

  return Status;
}

/**

  Programs the Inner Loop Number of Increments Control Words.

  @param[in] Host                - Pointer to sysHost
  @param[in] Socket              - Socket number
  @param[in] Channel             - Channel number
  @param[in] Dimm                - DIMM number
  @param[in] Rank                - Rank number on DIMM
  @param[in] InnerLoopIncrValue  - Inner Loop number of increments to be programmed

  @retval  - MRC_STATUS_SUCCESS - programming successful
           - MRC_STATUS_INVALID_PARAMETER - bad input paramater
**/
MRC_STATUS
ProgramDbTaInnerLoopIncr (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT16    InnerLoopIncrValue
  )
{
  MRC_STATUS                        Status = MRC_STATUS_SUCCESS;
  DDR5_DATA_BUFFER_PG9_RWE8_STRUCT  DfeTaInnerIncrE8;
  DDR5_DATA_BUFFER_PG9_RWE9_STRUCT  DfeTaInnerIncrE9;

  // Program Inner Loop Number of Increments Value (RWE8 RWE9)

  // If input value is greater then max programmable value, return with error
  if (InnerLoopIncrValue > INNER_LOOP_INCR_MAX) {
    RcDebugPrint (SDBG_MAX, "DB DFE TA Inner Loop Increment value %d greater than max value: %d \n",
      InnerLoopIncrValue, INNER_LOOP_INCR_MAX);
    return MRC_STATUS_INVALID_PARAMETER;
  }
  DfeTaInnerIncrE8.Data = 0;
  DfeTaInnerIncrE9.Data = 0;

  // If value is less than 256, value can be programmed directly into RWE8
  if (InnerLoopIncrValue < DB_TA_MAX_REG_VALUE) {
    DfeTaInnerIncrE8.Bits.inner_loop_number_of_increments = (UINT8) InnerLoopIncrValue;
  } else {
    // Otherwise, set upper bit and program lower
    DfeTaInnerIncrE8.Bits.inner_loop_number_of_increments = (UINT8) (InnerLoopIncrValue - DB_TA_MAX_REG_VALUE);
    DfeTaInnerIncrE9.Bits.inner_loop_number_of_increments = 1;
  }

  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG9_RWE8_REG, DfeTaInnerIncrE8.Data);
  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG9_RWE9_REG, DfeTaInnerIncrE9.Data);

  return Status;
}

/**

  Programs the Outer Loop Number of Increments Control Word.

  @param[in] Host                 - Pointer to sysHost
  @param[in] Socket               - Socket number
  @param[in] Channel              - Channel number
  @param[in] Dimm                 - DIMM number
  @param[in] Rank                 - Rank number on DIMM
  @param[in] OuterLoopIncrValue   - Outer Loop number of increments to be programmed

  @retval  - MRC_STATUS_SUCCESS - programming successful
           - MRC_STATUS_INVALID_PARAMETER - bad input paramater

**/
MRC_STATUS
ProgramDbTaOuterLoopIncr (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     OuterLoopIncrValue
  )
{
  MRC_STATUS                        Status = MRC_STATUS_SUCCESS;
  DDR5_DATA_BUFFER_PG9_RWEA_STRUCT  DfeTaOuterStartEA;

  // Program Outer Loop Number of Increments (RWEA)

  // If input value is greater then max programmable value, return with error
  if (OuterLoopIncrValue > OUTER_LOOP_INCR_MAX) {
    RcDebugPrint (SDBG_MAX, "DB DFE TA Outer Loop Increment value %d greater than max value: %d \n",
      OuterLoopIncrValue, OUTER_LOOP_START_MAX);
    return MRC_STATUS_INVALID_PARAMETER;
  }

  DfeTaOuterStartEA.Data = 0;
  DfeTaOuterStartEA.Bits.outer_loop_number_of_increments = OuterLoopIncrValue;

  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG9_RWEA_REG, DfeTaOuterStartEA.Data);

  return Status;
}

/**

  Programs the Lower and Upper Byte Write Limit Control Words.
  NOTE: Valid Write Limit values are 1 through 65536. A value of 0 will be programmed
        as 0, but it indicates a write limit of 1.

  @param[in] Host                 - Pointer to sysHost
  @param[in] Socket               - Socket number
  @param[in] Channel              - Channel number
  @param[in] Dimm                 - DIMM number
  @param[in] Rank                 - Rank number on DIMM
  @param[in] WriteLimit           - Write Limit to be programmed

  @retval  - MRC_STATUS_SUCCESS - programming successful
           - MRC_STATUS_INVALID_PARAMETER - bad input paramater

**/
MRC_STATUS
ProgramDbTaWriteLimit (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT32    WriteLimit
  )
{
  MRC_STATUS  Status = MRC_STATUS_SUCCESS;
  UINT8       WriteLimitLowerByte;
  UINT8       WriteLimitUpperByte;

  // If input value is greater then max programmable value, return with error
  if (WriteLimit > WRITE_LIMIT_MAX) {
    RcDebugPrint (SDBG_MAX, "DB DFE TA Write Limit value %d greater than max value: %d \n",
      WriteLimit, WRITE_LIMIT_MAX);
    return MRC_STATUS_INVALID_PARAMETER;
  }

  WriteLimitLowerByte = 0;
  WriteLimitUpperByte = 0;

  // Value to be programmed is the value programmed in the reigsters + 1, so subtract 1 from
  // the input to find the register value.
  if (WriteLimit > 0) {
    WriteLimit--;
  }
  // If value is less than 256, value can be programmed directly into RWEE
  if (WriteLimit < DB_TA_MAX_REG_VALUE) {
    WriteLimitLowerByte = (UINT8) WriteLimit;
  } else {
    WriteLimitLowerByte = (UINT8) (WriteLimit & BYTE_MASK);            // mask off upper byte for value to be programmed in lower byte
    WriteLimitUpperByte = (UINT8) (WriteLimit / DB_TA_MAX_REG_VALUE);  // divide by 256 to find value to be programmed in upper byte
    // Otherwise, calculate values to be programmed in lower and upper bytes
  }

  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, LRDIMM_RWEE, WriteLimitLowerByte);
  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, LRDIMM_RWEF, WriteLimitUpperByte);

  return Status;
}

/**

  Programs the Data Buffer DFE Training Accelerator Training Mode Control Word.

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number
  @param[in] TapIndex        - The index of the tap that will be used

  @retval  - MRC_STATUS_SUCCESS - programming successful
           - MRC_STATUS_INVALID_PARAMETER - bad input paramater

**/
VOID
DbDfeTaTrainingStart (
  IN PSYSHOST          Host,
  IN UINT8             Socket,
  IN UINT8             TapIndex
  )
{
  UINT8 Dimm = 0;
  UINT32 ChBitmask = 0;
  UINT8 Rank = 0;
  UINT8 Channel = 0;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    //
    // Create a bit mask of channels that have at least 1 rank present for this dimm and rank
    //
    ChBitmask = GetChBitmaskLrdimm (Socket, Dimm, Rank);
    if (ChBitmask == 0) {
      continue;
    }

    for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
      if (((1 << Channel) & ChBitmask) == 0) {
        continue;
      }
      ProgramDbDfeTaTrainingMode (Host, Socket, ChBitmask, Dimm, Rank, DbDfeTaStart, TapIndex, DbDfeVref);
    }
  }
  return;

}

/**

Programs the Data Buffer DFE Training Accelerator Training Mode Control Word.

@param[in] Host            - Pointer to sysHost
@param[in] Socket          - Socket number
@param[in] ChBitmask       - Bitmask of channels to be programmed
@param[in] Dimm            - DIMM number
@param[in] Rank            - Rank number on DIMM
@param[in] Enable          - Enable the TA or not
@param[in] TapToSweep      - which tap to run
@param[in] VrefToSweep     - What vref to be used for margin.

@retval  - MRC_STATUS_SUCCESS - programming successful
         - MRC_STATUS_INVALID_PARAMETER - bad input paramater

**/
MRC_STATUS
ProgramDbDfeTaTrainingMode (
  IN PSYSHOST          Host,
  IN UINT8             Socket,
  IN UINT32            ChBitmask,
  IN UINT8             Dimm,
  IN UINT8             Rank,
  IN DB_DFE_TA_ENABLE  Enable,
  IN UINT8             TapToSweep,
  IN DB_DFE_VREF_TYPE  VrefToSweep
  )
{
  MRC_STATUS                        Status = MRC_STATUS_SUCCESS;
  UINT8                             Channel;
  UINT8                             MaxChDdr;
  DDR5_DATA_BUFFER_PG9_RWE0_STRUCT  DfeTaOuterStartE0;

  MaxChDdr = GetMaxChDdr ();
  for (Channel = 0; Channel < MAX_CH; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }

    // Check input values
    if (Enable >= DbDfeTaMax) {
      RcDebugPrint (SDBG_MAX, "DB DFE TA Training Accelerator Start/Stop %d is an invalid value.\n", Enable);
      Status = MRC_STATUS_INVALID_PARAMETER;
    }

    if (VrefToSweep >= DbVrefTypeMax) {
      RcDebugPrint (SDBG_MAX, "DB DFE TA Training Accelerator Inner loop parameter %d is an invalid value.\n", VrefToSweep);
      Status = MRC_STATUS_INVALID_PARAMETER;
    }

    if (TapToSweep >= DbTapMax) {
      RcDebugPrint (SDBG_MAX, "DB DFE TA Training Accelerator Outer loop parameter %d is an invalid value.\n", TapToSweep);
      Status = MRC_STATUS_INVALID_PARAMETER;
    }

    if (Status != MRC_STATUS_SUCCESS) {
      return Status;
    }
    //
    // Select DB page 9 for Training Accelerator programming
    //
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, DB_TA_PAGE);

    // Program register
    DfeTaOuterStartE0.Data = 0;
    DfeTaOuterStartE0.Bits.dfe_training_accelerator = Enable;
    DfeTaOuterStartE0.Bits.select_inner_loop_parameter = VrefToSweep;
    if (TapToSweep == DbTapNull) {
      DfeTaOuterStartE0.Bits.select_outer_loop_parameter = 0;
    } else {
      DfeTaOuterStartE0.Bits.select_outer_loop_parameter = TapToSweep + 1;
    }

    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG9_RWE0_REG, DfeTaOuterStartE0.Data);
  } // Channel loop

  return Status;
}

/**

  Programs the Data Buffer DFE Training Accelerator with the values provided in the
  DB_DFE_TA_LOOP_CONTROL structure.

  @param[in] Host              - Pointer to sysHost
  @param[in] Socket            - Socket number
  @param[in] ChBitmask         - Bitmask of channels to be programmed
  @param[in] Dimm              - DIMM number
  @param[in] Rank              - Rank number on DIMM
  @param[in] TapStartValue     - Starting Tap value to be programmed in Outer loop start value
  @param[in] TapStepSize       - Tap Step size to be programmed in Outer loop step size
  @param[in] TapNumberOfSteps  - Number of Tap steps to be programmed in Outer loop number of increments
  @param[in] VrefStartValue    - Starting Vref value to be programmed in Inner loop start value
  @param[in] VrefStepSize      - Vref step size to be programmed in Inner loop step size
  @param[in] VrefNumberOfSteps - Number of Vref steps to be programmed in Inner loop number of increments
  @param[in] WriteLimit        - Number of writes training accelerator will count before moving to next step

  @retval - MRC_STATUS_SUCCESS - No errors detected
          - MRC_STATUS_INVALID_PARAMETER - Sweep out of range given inputs

**/
MRC_STATUS
ProgramDbDfeTALoopSettings (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChBitmask,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN INT8      TapStartValue,
  IN UINT8     TapStepSize,
  IN UINT8     TapNumberOfSteps,
  IN INT16     VrefStartValue,
  IN UINT8     VrefStepSize,
  IN UINT16    VrefNumberOfSteps,
  IN UINT32    WriteLimit
  )
{
  MRC_STATUS  Status = MRC_STATUS_SUCCESS;
  UINT8       Channel;
  UINT8       MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  for (Channel = 0; Channel < MAX_CH; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }

    // Select DB page 9 for Training Accelerator programming
    DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, DB_TA_PAGE);

    // Program Inner Loop Start Value (RWE1 RWE2)
    Status = ProgramDbTaInnerLoopStart (Host, Socket, Channel, Dimm, Rank, VrefStartValue);
    if (Status != MRC_STATUS_SUCCESS) {
      return Status;
    }

    // Program Outer Loop Start Value (RWE3)
    Status = ProgramDbTaOuterLoopStart (Host, Socket, Channel, Dimm, Rank, TapStartValue);
    if (Status != MRC_STATUS_SUCCESS) {
      return Status;
    }

    // Program Inner and Outer Loop Step Size (RWE7)
    Status = ProgramDbTaStepSize (Host, Socket, Channel, Dimm, Rank, VrefStepSize, TapStepSize);
    if (Status != MRC_STATUS_SUCCESS) {
      return Status;
    }

    // Program Inner Loop Number of Increments (RWE8, RWE9)
    Status = ProgramDbTaInnerLoopIncr (Host, Socket, Channel, Dimm, Rank, VrefNumberOfSteps);
    if (Status != MRC_STATUS_SUCCESS) {
      return Status;
    }

    // Program Outer Loop Number of Increments (RWEA)
    Status = ProgramDbTaOuterLoopIncr (Host, Socket, Channel, Dimm, Rank, TapNumberOfSteps);
    if (Status != MRC_STATUS_SUCCESS) {
      return Status;
    }

    // Program Write Limit Value (RWEE, RWEF)
    Status = ProgramDbTaWriteLimit (Host, Socket, Channel, Dimm, Rank, WriteLimit);
    if (Status != MRC_STATUS_SUCCESS) {
      return Status;
    }
  } // Channel loop

  return Status;
}

/**

  Checks the sweep range of the Tap sweep to verify that is not out of range of the Data
  Buffer spec. Tap 1 valid range is -150mV (-50) to 45mV (15 reg value). Tap 2 valid range
  is -60mV (-20) to 45mV (15). Tap 3 and 4 valid range is -45mV (-15) to 45mV (15).

  @param[in] TapToSweep        - Tap number to be swept
  @param[in] TapStartValue     - Starting value of the tap setting
  @param[in] TapStepSize       - Value by which tap value will be incremented through each loop
  @param[in] TapNumberOfSteps  - Number of times the tap value will be incremented

  @retval - MRC_STATUS_SUCCESS - No errors detected
          - MRC_STATUS_INVALID_PARAMETER - Sweep out of range given inputs

**/
MRC_STATUS
CheckDbTapParameters (
  IN DB_DFE_TAPS  TapToSweep,
  IN INT8         TapStartValue,
  IN UINT8        TapStepSize,
  IN UINT8        TapNumberOfSteps
  )
{
  MRC_STATUS  Status = MRC_STATUS_SUCCESS;
  UINT32      MaxTapValue;

  // No need to check if input tap is NULL (0)
  if (TapToSweep == DbTapNull) {
    return MRC_STATUS_SUCCESS;
  }

  // Return if invalid tap is input
  if (TapToSweep >= DbTapMax) {
    RcDebugPrint (SDBG_MAX, "Tap%d input in to DB DFE training accelerator is invalid\n", TapToSweep);
    return MRC_STATUS_INVALID_PARAMETER;
  }

  switch (TapToSweep) {
    case DbTap1:

      // Check start value
      if (TapStartValue < TAP1_MIN || TapStartValue > TAP_MAX) {
        RcDebugPrint (SDBG_MAX, "Tap%d Start Value %d is out of range Min: %d Max: %d\n",
          TapToSweep, TapStartValue, TAP1_MIN, TAP_MAX);
        return MRC_STATUS_INVALID_PARAMETER;
      }
      break;

    case DbTap2:

      // Check start value
      if (TapStartValue < TAP2_MIN || TapStartValue > TAP_MAX) {
        RcDebugPrint (SDBG_MAX, "Tap%d Start Value %d is out of range Min: %d Max: %d\n",
          TapToSweep, TapStartValue, TAP2_MIN, TAP_MAX);
        return MRC_STATUS_INVALID_PARAMETER;
      }
      break;

    case DbTap3:
    case DbTap4:

      // Check start value
      if (TapStartValue < TAP34_MIN || TapStartValue > TAP_MAX) {
        RcDebugPrint (SDBG_MAX, "Tap%d Start Value %d is out of range Min: %d Max: %d\n",
          TapToSweep, TapStartValue, TAP34_MIN, TAP_MAX);
        return MRC_STATUS_INVALID_PARAMETER;
      }
      break;

    default:
      // Should never get here
      RcDebugPrint (SDBG_MAX, "Tap%d input in to DB DFE training accelerator is invalid\n", TapToSweep);
      return MRC_STATUS_INVALID_PARAMETER;

      break;
  }

  // Make sure range value is less than max valid range
  MaxTapValue = TapStartValue + (TapStepSize * TapNumberOfSteps);
  if (MaxTapValue > TAP_MAX) {
    RcDebugPrint (SDBG_MAX, "Tap%d sweep range exceeds max tap value of %d\n", TapToSweep, TAP_MAX);
    Status = MRC_STATUS_INVALID_PARAMETER;
  }

  return Status;
}

/**

  Checks the sweep range of the Vref sweep to verify that is not out of range of the Data
  Buffer spec. Valid Vref DFE range is -500mV (-200 reg value) to 500mV (200). Valid VrefDQ
  is 97.5% of VDD (0 reg value) to 35% (125).

  @param[in] VrefToSweep        - Vref to be swept (Vref DFE, VrefDq)
  @param[in] VrefStartValue     - Starting value of the Vref setting
  @param[in] VrefStepSize       - Value by which Vref value will be incremented through each loop
  @param[in] VrefNumberOfSteps  - Number of times the Vref value will be incremented

  @retval - MRC_STATUS_SUCCESS - No errors detected
          - MRC_STATUS_INVALID_PARAMETER - Sweep out of range given inputs

**/
MRC_STATUS
CheckDbVrefParameters (
  IN DB_DFE_VREF_TYPE  VrefToSweep,
  IN INT16             VrefStartValue,
  IN UINT8             VrefStepSize,
  IN UINT16            VrefNumberOfSteps
  )
{
  MRC_STATUS  Status = MRC_STATUS_SUCCESS;
  UINT32      MaxVrefValue = 0;

  // No need to check if input tap is NULL (0)
  if (VrefToSweep == DbVrefNull) {
    return MRC_STATUS_SUCCESS;
  }

  // Return if invalid tap is input
  if (VrefToSweep >= DbVrefTypeMax) {
    RcDebugPrint (SDBG_MAX, "Vref input type to DB DFE training accelerator invalid\n");
    return MRC_STATUS_INVALID_PARAMETER;
  }

  switch (VrefToSweep) {
    case DbDfeVref:

      // Check start value
      if (VrefStartValue < VREF_DFE_MIN || VrefStartValue > VREF_DFE_MAX) {
        RcDebugPrint (SDBG_MAX, "DFE Vref Start Value %d is out of range Min: %d Max: %d\n",
          VrefStartValue, VREF_DFE_MIN, VREF_DFE_MAX);
        return MRC_STATUS_INVALID_PARAMETER;
      }

      // Make sure range value is less than max valid range
      MaxVrefValue = VrefStartValue + (VrefStepSize * VrefNumberOfSteps);
      if (MaxVrefValue > VREF_DFE_MAX) {
        RcDebugPrint (SDBG_MAX, "DFE Vref sweep range exceeds max Vref value of %d\n", VREF_DFE_MAX);
        Status = MRC_STATUS_INVALID_PARAMETER;
      }
      break;

    case DbVrefDq:

      // Check start value
      if (VrefStartValue < VREF_DQ_MIN || VrefStartValue > VREF_DQ_MAX) {
        RcDebugPrint (SDBG_MAX, "DB Vref DQ Start Value %d is out of range Min: %d Max: %d\n",
          VrefStartValue, VREF_DQ_MIN, VREF_DQ_MAX);
        return MRC_STATUS_INVALID_PARAMETER;
      }

      // Make sure range value is less than max valid range
      MaxVrefValue = VrefStartValue + (VrefStepSize * VrefNumberOfSteps);
      if (MaxVrefValue > VREF_DQ_MAX) {
        RcDebugPrint (SDBG_MAX, "DFE Vref sweep range exceeds max Vref value of %d\n", VREF_DQ_MAX);
        Status = MRC_STATUS_INVALID_PARAMETER;
      }
      break;

    default:
      // Should never get here
      RcDebugPrint (SDBG_MAX, "Vref input type to DB DFE training accelerator invalid\n");
      Status = MRC_STATUS_INVALID_PARAMETER;
      break;
  }

  return Status;
}

/**

  Collects the errors from the CPGC after a test has been executed.

  @param[in] Host          - Pointer to sysHost
  @param[in] Socket        - Socket number
  @param[in] ChBitmask     - Bitmask of channels to collect results for
  @param[in] Dimm          - DIMM number
  @param[in] VrefStep      - Current step number of Vref sweep
  @param[in] ErrorResults  - Pointer to structure to store error results

  @retval N/A

**/
VOID
CollectDbDfeTestResults (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT32                ChBitmask,
  IN UINT8                 Dimm,
  IN UINT16                VrefStep,
  IN UINT16                VrefNumberOfSteps,
  OUT DB_BIT_ERROR_RESULTS_WR_DDR5 (*ErrorResult) [MAX_CH][SUB_CH]
  )
{
  UINT8       MaxChDdr;
  UINT8       Bit;
  UINT32      BwSerr[MAX_CH][SUB_CH][3];
  UINT8       PiIndex;
  UINT32      ErrResult;
  UINT8       SubCh = 0;
  UINT8       Ch = 0;
  UINT8       MBV = GetMaxBitsValid (Host);
  UINT8       ChStatus;
  //
  // Each DB_DFE_ERRORS array can hold results for 32 vref steps.  So divide current VrefStep
  // by 32 to find the correct index into the result array
  //
  MaxChDdr = GetMaxChDdr ();
  ErrResult = 0;
  ChStatus = 0;
  for (Ch = 0; Ch  < MaxChDdr; Ch += GetChInc (Host)) {
    if (!((1 << Ch) & ChBitmask)) {
      continue;
    }
    ChStatus = 1 << Ch;


    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      BwSerr[Ch][SubCh][0] = 0;
      BwSerr[Ch][SubCh][1] = 0;
      BwSerr[Ch][SubCh][2] = 0;
    }
    //
    // Collect test results
    //
    CollectTestResultsDdr5 (Host, Socket, ChStatus, BwSerr);

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {


#ifdef DEBUG_CODE_BLOCK
      if (GetDebugLevel () & SDBG_MAX) {
        DisplayBwSerrDdr5Ch (Host, Socket, Ch, SubCh, Dimm, 0, BwSerr[Ch][SubCh], (INT16) VrefStep);
      }
#endif // DEBUG_CODE_BLOCK

      // Loop over every bit in the subchannel to find errors for each bit
      for (Bit = 0; Bit < MBV; Bit++) {
        if (IsBitNotValid (Host, Bit)) {
          continue;
        }
        // First 32 bits or error info are contained in array 0, last 8 are in array 2
        if (Bit < 32) {
          ErrResult = BwSerr[Ch][SubCh][0];
        } else {
          ErrResult = BwSerr[Ch][SubCh][2];
        }

        // DqErrors will contain either 32 bits or 8 bits of error info. Check the
        // appropriate bit (1-32) or (1-8) and record if an error was seen for that
        // bit.

        for (PiIndex = 0; PiIndex < DB_DFE_VREF_MARGIN_STEP_SIZE; PiIndex++) {
          //
          // Any failure is a failure for the bit
          //
          if (ErrResult & (BIT0 << (Bit % 32))) {
            (*ErrorResult) [Ch][SubCh].BitErrorResult[ (VrefStep % (DB_DFE_VREF_MARGIN_NUM_OF_STEPS + 1)) + PiIndex][Bit / MAX_BITS_IN_BYTE] |= (BIT0 << (Bit % MAX_BITS_IN_BYTE));
          } else {
            (*ErrorResult) [Ch][SubCh].BitErrorResult[ (VrefStep % (DB_DFE_VREF_MARGIN_NUM_OF_STEPS + 1)) + PiIndex][Bit / MAX_BITS_IN_BYTE] &= ~(BIT0 << (Bit % MAX_BITS_IN_BYTE));
          }
        } // piIndex loop
      } // bit loop

    }
  }
}

/**

  Records the Tap Coefficient in the results structure. The Tap Coefficient is calculated
  based on the starting value, step size, and iteration number.

  @param[in]  Host           - Pointer to sysHost
  @param[in]  Socket         - Socket number
  @param[in]  ChBitmask      - Bitmask of channels
  @param[in]  TapStartValue  - Starting tap value
  @param[in]  TapStepSize    - Tap step size
  @param[in]  TapStepNumber  - Tap iteration number (in the sweep)
  @param[out] SweepResults   - Pointer to results structure to store tap coefficient

  retval N/A

**/
VOID
RecordTapCoefficient (
  IN     PSYSHOST          Host,
  IN     UINT8             Socket,
  IN     UINT32            ChBitmask,
  IN     INT8              TapStartValue,
  IN     UINT8             TapStepSize,
  IN     UINT32            TapStepNumber,
  OUT DB_SWEEP_RESULTS  (*SweepResults) [MAX_CH][MAX_BITS][MAX_TAP_SWEEP_RANGE]
  )
{
  UINT8  Channel;
  UINT8  MaxChDdr;
  UINT8  MaxValidBits;
  UINT8  Bit;

  MaxChDdr = GetMaxChDdr ();
  MaxValidBits = GetMaxBitsValid (Host);

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }
    for (Bit = 0; Bit < MaxValidBits; Bit++) {
      if (IsBitNotValid (Host, Bit)) {
        continue;
      }

      // The coefficient value is the Starting value plus (step size * step number)
      (*SweepResults) [Channel][Bit][TapStepNumber].TapCoeff = (INT8) (TapStartValue + (TapStepSize * TapStepNumber));
    }
  }
}

/**

  The function gets number of error samples and finds the middle of zero error

  @param[in]  Host              - Pointer to sysHost
  @param[in]  Socket            - Socket number
  @param[in]  Channel           - Channel number
  @param[in]  Dimm              - DIMM number
  @param[in]  Rank              - Rank number
  @param[in]  ErrorSamples      - Array containing an error status for every element
  @param[in]  ErrorSamplesSize  - Number of samples to check
  @param[out] ErrorStart        - Pointer to location for error start
  @param[out] ErrorEnd          - Pointer to location for error end

  @retval status

**/
MRC_STATUS
FindErrorEdges (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  UINT8     Dimm,
  IN  UINT8     Rank,
  IN  UINT32    *ErrorSamples,
  IN  UINT16    ErrorSamplesSize,
  OUT UINT16    *ErrorStart,
  OUT UINT16    *ErrorEnd
  )
{
  UINT16  ZeroSize;
  UINT16  SampleNum;
  UINT16  TempErrorEnd;
  UINT16  TempErrorStart;
  UINT32  Status;
  UINT8   BitMask = 1;
  UINT8   DWord = 0;
  UINT8   PrevDWord;
  UINT8   Inject2NdError;

  TempErrorEnd = ErrorSamplesSize;
  TempErrorStart = ErrorSamplesSize;
  Status = MRC_STATUS_SUCCESS;
  Inject2NdError = FAULTY_PARTS_TRACKING_INJECT_2ND_ERROR;

  *ErrorEnd = ErrorSamplesSize;
  *ErrorStart = ErrorSamplesSize;

  //
  // If the first sample passed and the last sample failed
  //
  if ((ErrorSamples[0] & BitMask) == 0) {
    TempErrorEnd = 0;
  }
  //
  // Loop through each sample
  //
  for (SampleNum = 1; SampleNum < ErrorSamplesSize; SampleNum++) {
    // Find appropriate element of array
    DWord = (UINT8) (SampleNum / 32);
    PrevDWord = (UINT8) ((SampleNum - 1) / 32);
    //
    // Find the place of error start
    //
    if (TempErrorEnd == ErrorSamplesSize) {
      //
      // If this sample passed and the previous sample failed
      //
      if ((((ErrorSamples[DWord] >> (SampleNum % 32)) & BitMask) == 0) && (((ErrorSamples[PrevDWord] >> ((SampleNum - 1) % 32)) & BitMask) == 1)) {
        TempErrorEnd = SampleNum;

        //
        // Check if this is the last sample and it passed
        //
        if (SampleNum == (ErrorSamplesSize - 1)) {
          TempErrorStart = SampleNum;

          //
          // Update error start and error end
          //
          if ((*ErrorStart - *ErrorEnd) < (TempErrorStart - TempErrorEnd)) {
            *ErrorEnd = TempErrorEnd;
            *ErrorStart = TempErrorStart;

            //
            // Clean the temp variable
            //
            TempErrorEnd = ErrorSamplesSize;
            TempErrorStart = ErrorSamplesSize;
          }
        }
      }
    } else {
      //
      // TempErrorStart != ErrorSamplesSize // Find the place of error end
      //

      //
      // If this sample failed and the previous sample passed
      //
      if ((((ErrorSamples[DWord] >> (SampleNum % 32)) & BitMask) == 1) && (((ErrorSamples[PrevDWord] >> ((SampleNum - 1) % 32)) & BitMask) == 0)) {
        //
        // We found the first error sample
        //
        TempErrorStart = (SampleNum - 1);

        //
        // Update error start and error end
        //
        if ((*ErrorStart - *ErrorEnd) <= (TempErrorStart - TempErrorEnd)) {
          *ErrorEnd = TempErrorEnd;
          *ErrorStart = TempErrorStart;
        }

        //
        // Clean the temp variable
        //
        TempErrorEnd = ErrorSamplesSize;
        TempErrorStart = ErrorSamplesSize;
      }
      //
      // Check if this is the last sample and it passed
      //
      if ((SampleNum == (ErrorSamplesSize - 1)) && ((((ErrorSamples[DWord] >> (SampleNum % 32)) & BitMask) == 0) && ((ErrorSamples[0] & BitMask) == 1))) {
        //
        // We found the first error sample
        //
        TempErrorStart = SampleNum;

        //
        // Update error start and error end
        //
        if ((*ErrorStart - *ErrorEnd) < (TempErrorStart - TempErrorEnd)) {
          *ErrorEnd = TempErrorEnd;
          *ErrorStart = TempErrorStart;

          //
          // Clean the temp variable
          //
          TempErrorEnd = ErrorSamplesSize;
          TempErrorStart = ErrorSamplesSize;
        }
      }
    }
  } // ErrorSamplesSize loop

  //
  // in case of cycle data like #######EEEEEEEEEEEE ########.
  //
  if ((TempErrorEnd < ErrorSamplesSize) && (TempErrorStart == ErrorSamplesSize)) {
    //
    // If the first sample failed
    //
    if ((ErrorSamples[0] & BitMask) == 1) {
      TempErrorStart = (ErrorSamplesSize - 1);
      //
      // the case is like the next: EEEEEEEEEEEE##############
      //
    } else {
      for (SampleNum = 1; SampleNum < ErrorSamplesSize; SampleNum++) {
        //
        // If this sample failed and the previous sample passed
        //
        if ((((ErrorSamples[DWord] >> (SampleNum % 32)) & BitMask) == 1) && (((ErrorSamples[PrevDWord] >> ((SampleNum - 1) % 32)) & BitMask) == 0)) {
          TempErrorStart = (SampleNum - 1);
          //
          // Break because we found the start of the errors
          //
          break;
        }
      } // ErrorSamplesSize loop
    }

    if ((*ErrorStart - *ErrorEnd) < ((TempErrorStart + ErrorSamplesSize) - TempErrorEnd)) {
      *ErrorEnd = TempErrorEnd;
      *ErrorStart = TempErrorStart;
    }
  }
  if (Inject2NdError == 1) {
    if ((Channel == 0) && (Rank == 0)) {
      *ErrorEnd = *ErrorStart;
    }
  } // FAULTY_PARTS_TRACKING_INJECT_2ND_ERROR

  //
  // Result correction checking and find the Pi position
  //
  if (*ErrorEnd == ErrorSamplesSize || *ErrorStart == ErrorSamplesSize || *ErrorEnd == *ErrorStart) {

    Status = MRC_STATUS_FAILURE;
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "Dq/Dqs Pi not found. Using default\n");
    *ErrorStart = 42;
    *ErrorEnd = 30;
  } else {
    if (*ErrorStart < *ErrorEnd) {
      ZeroSize = ((ErrorSamplesSize - (UINT8)*ErrorEnd) + (UINT8)*ErrorStart);
    } else {
      ZeroSize = (UINT8) (*ErrorStart - *ErrorEnd);
    }
  }

  return Status;
}

/**

  Finds the margin for the Vref sweep.

  @param[in]  Host               - Pointer to sysHost
  @param[in]  Socket             - Socket number
  @param[in]  ChBitmask          - Bitmask of channels
  @param[in]  Dimm               - Dimm number
  @param[in]  TapStepNumber      - Current iteration of the Tap sweep
  @param[in]  VrefNumberOfSteps  - Number of times inner loop (Vref) should execute
  @param[out] VrefMarginPerBit   - Pointer to structure to store margin results

  @reval N/A

**/
VOID
RecordVrefMargin (
  IN     PSYSHOST             Host,
  IN     UINT8                Socket,
  IN     UINT32               ChBitmask,
  IN     UINT8                Dimm,
  IN     UINT16               VrefNumberOfSteps,
  IN     DB_BIT_ERROR_RESULTS_WR_DDR5   (*ErrorResult) [MAX_CH][SUB_CH],
  OUT    struct bitMargin     (*VrefMarginPerBit)
  )
{
  UINT32      Status = 0;
  UINT8       Channel;
  UINT8       MaxChDdr;
  UINT8       SubChannel;
  UINT8       Rank = 0; // Only target rank 0 per DIMM
  UINT8       RankInCh;
  UINT8       MaxValidBitsSubCh;
  UINT8       ChannelBit;
  DB_DFE_EYE  DbDfeEye;
  UINT8       Nibble = 0;
  UINT8       Bit;
  UINT8       EncodedNibble;
  UINT8       MBV = MAX_BITS_DDR5 / 2;
  UINT16      Index = 0;
  PerBitPi    ErrorStart[MAX_CH];
  PerBitPi    ErrorEnd[MAX_CH];
  PerBitPi    PiPosition[MAX_CH];
  UINT8       (*ErrorSamples) [DB_DFE_VREF_MARGIN_NUM_OF_STEPS + 1];
  MaxChDdr = GetMaxChDdr ();
  MaxValidBitsSubCh = GetMaxBitsValid (Host) / 2;  // Max bits in subchannel
  ErrorSamples = RcAllocatePool (sizeof (*ErrorSamples));
  if (ErrorSamples == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return;
  }
  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }

    RankInCh = GetLogicalRank (Host, Socket, Channel, Dimm, Rank);

    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      for (Bit = 0; Bit < MBV; Bit++) {
        Nibble = Bit / 4;
        EncodedNibble = GetStrobeFromNibble (Host, Nibble);
        if (IsBitNotValidSubChDdr5 (Host, Bit)) {
          continue;
        }
        // Initialize eye data to 0
        DbDfeEye.LeftEdge = 0;
        DbDfeEye.RightEdge = 0;

        //
        // Copy the bit-packed error data into an array for FindErrorGraphMin
        //
        for (Index = 0; Index < (DB_DFE_VREF_MARGIN_NUM_OF_STEPS + 1); Index++) {
          if (((*ErrorResult) [Channel][SubChannel].BitErrorResult[Index][Bit / MAX_BITS_IN_BYTE] & (BIT0 << (Bit % MAX_BITS_IN_BYTE))) == 0) {
            (*ErrorSamples) [Index] = 0;
          } else {
            (*ErrorSamples) [Index] = 1;
          }
        }

        Status = FindErrorGraphMin (Host, Socket, Channel, Dimm, 0, EncodedNibble, &(*ErrorSamples) [0],
                   (DB_DFE_VREF_MARGIN_NUM_OF_STEPS + 1), &ErrorStart[Channel].PiPosition[Bit],
                   &ErrorEnd[Channel].PiPosition[Bit], &PiPosition[Channel].PiPosition[Bit]);
        // Record margin in result structure
        ChannelBit = Bit + (MaxValidBitsSubCh * SubChannel);
        (*VrefMarginPerBit).bits[Channel][RankInCh][ChannelBit].n = ErrorEnd[Channel].PiPosition[Bit];
        (*VrefMarginPerBit).bits[Channel][RankInCh][ChannelBit].p = ErrorStart[Channel].PiPosition[Bit];

      } // Bit loop
    } // SubChanel loop
  } // Channel loop
  RcFreePool (ErrorSamples);
}

/**

Programs the Data Buffer DFE Training Accelerator to the input sweep paramaters.

@param[in]  Host               - Pointer to sysHost
@param[in]  Socket             - Socket number
@param[in]  TapIndex           - Whicn index tap to run
@param[in]  VrefToSweep        - Type of Vref to sweep
@param[in]  TapStartValue      - Starting outer loop (Tap) value
@param[in]  TapStepSize        - The increment added to the Tap coefficient after every loop
@param[in]  TapNumberOfSteps   - Number of times outer loop (Tap) should execute
@param[in]  VrefStartValue     - Starting inner loop (Vref) value
@param[in]  VrefStepSize       - The increment added to the VrefStartValue after every loop
@param[in]  VrefNumberOfSteps  - Number of times inner loop (Vref) should execute

@retval - MRC_STATUS_SUCCESS - No errors detected
        - MRC_STATUS_INVALID_PARAMETER - Sweep out of range given inputs

**/
MRC_STATUS
ConfigureDbDfeTraining (
  IN     PSYSHOST          Host,
  IN     UINT8             Socket,
  IN     UINT8             TapIndex,
  IN     DB_DFE_VREF_TYPE  VrefToSweep,
  IN     INT8              TapStartValue,
  IN     UINT8             TapStepSize,
  IN     UINT8             TapNumberOfSteps,
  IN     INT16             VrefStartValue,
  IN     UINT8             VrefStepSize,
  IN     UINT16            VrefNumberOfSteps
  )
{
  MRC_STATUS           Status = MRC_STATUS_SUCCESS;
  UINT8                Channel;
  UINT32               ChBitmask;
  UINT8                MaxChDdr;
  UINT8                Dimm;
  UINT8                Rank = 0;  // Only target Rank 0 per DIMM
  UINT32               WriteLimit = 0;
  MRC_MST              MemSsType;

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  MaxChDdr = GetMaxChDdr ();
  //
  // Make sure Vref related parameters are within spec defined ranges
  //
  Status = CheckDbVrefParameters (VrefToSweep, VrefStartValue, VrefStepSize, VrefNumberOfSteps);
  if (Status != MRC_STATUS_SUCCESS) {
    return Status;
  }
  //
  // Get number of writes for each CPGC test
  //
  WriteLimit = GetDbDfeNumCpgcWrites ();

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    //
    // Create a bit mask of channels that have at least 1 rank present for this dimm and rank
    //
    ChBitmask = GetChBitmaskLrdimm (Socket, Dimm, Rank);
    if (ChBitmask == 0) {
      continue;
    }

    for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
      if (!((1 << Channel) & ChBitmask)) {
        continue;
      }
      //
      // Program Inner/Outer loop settings
      //
      Status = ProgramDbDfeTALoopSettings (Host, Socket, ChBitmask, Dimm, Rank, TapStartValue, TapStepSize,
                 TapNumberOfSteps, VrefStartValue, VrefStepSize, VrefNumberOfSteps, WriteLimit);
      if (Status != MRC_STATUS_SUCCESS) {
        return Status;
      }
      //
      // Configure the Training Accelerator
      //
      Status = ProgramDbDfeTaTrainingMode (Host, Socket, ChBitmask, Dimm, Rank, DbDfeTaStop, TapIndex, VrefToSweep);
      if (Status != MRC_STATUS_SUCCESS) {
        return Status;
      }
    }// Channel loop
  } // Dimm loop

  return Status;
}


/**

Programs the Data Buffer DFE Training Accelerator to the input sweep paramaters. Starts the CPGC test
the appropriate number of times (based off of input paramaters). Records errors for each test and for
each outer loop step, stores the coefficient value and corresponding figure of merit in the SweepResults
structure.

@param[in]  Host               - Pointer to sysHost
@param[in]  Socket             - Socket number
@param[in]  TapIndex           - Which index tap to use
@param[in]  VrefToSweep        - Type of Vref to sweep

@retval - MRC_STATUS_SUCCESS - No errors detected
        - MRC_STATUS_INVALID_PARAMETER - Sweep out of range given inputs

**/
MRC_STATUS
SetupDbDfeTraining (
  IN     PSYSHOST          Host,
  IN     UINT8             Socket,
  IN     UINT8             TapIndex,
  IN     DB_DFE_VREF_TYPE  VrefToSweep
  )
{
  MRC_STATUS           Status = MRC_STATUS_SUCCESS;
  UINT8                Channel;
  UINT32               ChBitmask;
  UINT8                MaxChDdr;
  UINT8                Dimm;
  UINT8                Rank = 0;  // Only target Rank 0 per DIMM
  MRC_MST              MemSsType;
  MRC_RT               MrcRankList;
  UINT32               RankListSize = 1;

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  MaxChDdr = GetMaxChDdr ();


  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    //
    // Create a bit mask of channels that have at least 1 rank present for this dimm and rank
    //
    ChBitmask = GetChBitmaskLrdimm (Socket, Dimm, Rank);
    if (ChBitmask == 0) {
      continue;
    }

    for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
      if (!((1 << Channel) & ChBitmask)) {
        continue;
      }

      MrcRankList.Dimm = Dimm;
      MrcRankList.Rank = Rank;

      SetupTest (Host, Socket, MemSsType, ChBitmask, RankListSize, &MrcRankList, DdrLevel, WriteDbDfeTest, gsmCsnDelim, 0, 0);

      //
      // Program the Training Accelerator
      //
      Status = ProgramDbDfeTaTrainingMode (Host, Socket, ChBitmask, Dimm, Rank, DbDfeTaStart, TapIndex, VrefToSweep);
      if (Status != MRC_STATUS_SUCCESS) {
        return Status;
      }
    }// Channel loop
  } // Dimm loop

  return Status;
}

#endif // #ifdef LRDIMM_SUPPORT

/**

 Update the seed value in DRAM

  @param[in] Socket                - Socket to train

  @retval    NA

**/
VOID
UpdateDramSeed (
  IN UINT8                    Socket
  )
{

  PSYSHOST                 Host;

  MRC_TT                   TestType;
  UINT8                    Dimm;
  UINT8                Ch;
  UINT8                Rank;
  struct dimmNvram     (*DimmNvList) [MAX_DIMM];
  struct ddrRank       (*RankList) [MAX_RANK_DIMM];
  UINT8                MaxChDdr;
  Host = GetSysHostPointer ();
  TestType = GetCurrentTestType (Socket);

  UINT8 Bit = 0;
  DDR5_MODE_REGISTER_30_STRUCT Ddr5Mr30;
  Ddr5Mr30.Data = 0;
  UINT8 SubCh = 0;
  UINT8 Nibble = 0;
  UINT8 MSVx4 = 0;
  UINT8 BitsNum = 0;
  UINT8 BitNew = 0;
  UINT8 NewBitIndex = 0;
  UINT8 EncodedNibble = 0;
  MaxChDdr = GetMaxChDdr ();

  if ((GetCurrentTestType (Socket) == ReadDfeTest)) {

    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }
      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
            if (IsX4Dimm (Socket, Ch, Dimm) == FALSE) {
              MSVx4 = MAX_STROBE_DDR5 / 4;
              BitsNum = MAX_BITS_IN_BYTE;
            } else {
              MSVx4 = MAX_STROBE_DDR5 / 2;
              BitsNum = BITS_PER_NIBBLE;
            }
            for (Nibble = 0; Nibble < MSVx4; Nibble++) {
              Ddr5Mr30.Data = MR30_READ_DFE_INIT_VALUE;
              for (Bit = 0; Bit < BitsNum; Bit++) {
                NewBitIndex = Nibble*BitsNum + Bit;

                BitNew = (*RankList) [Rank].DeDqSwzDdr5[SubCh][NewBitIndex];
                RcDebugPrintWithDevice (SDBG_CPGC, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "CPU bit %d  DRAM bit %d \n", NewBitIndex, BitNew);

                if ((NewBitIndex % BITS_PER_NIBBLE) == Host->var.mem.socket[Socket].TrainingStatus.Bits.BitIndex) {
                  if (IsX4Dimm (Socket, Ch, Dimm) == FALSE) {

                    if (BitNew == 0) {
                      Ddr5Mr30.Bits.lfsr_assignment_dql0_dqu0 = 0;
                    } else if (BitNew == 1) {
                      Ddr5Mr30.Bits.lfsr_assignment_dql1_dqu1 = 0;
                    } else if (BitNew == 2) {
                      Ddr5Mr30.Bits.lfsr_assignment_dql2_dqu2 = 0;
                    } else if (BitNew == 3) {
                      Ddr5Mr30.Bits.lfsr_assignment_dql3_dqu3 = 0;
                    } else if (BitNew == 4) {
                      Ddr5Mr30.Bits.lfsr_assignment_dql4_dqu4 = 0;
                    } else if (BitNew == 5) {
                      Ddr5Mr30.Bits.lfsr_assignment_dql5_dqu5 = 0;
                    } else if (BitNew == 6) {
                      Ddr5Mr30.Bits.lfsr_assignment_dql6_dqu6 = 0;
                    } else if (BitNew == 7) {
                      Ddr5Mr30.Bits.lfsr_assignment_dql7_dqu7 = 0;
                    }
                  } else {
                    if (BitNew == 0) {
                      Ddr5Mr30.Bits.lfsr_assignment_dql0_dqu0 = 0;
                    } else if (BitNew == 1) {
                      Ddr5Mr30.Bits.lfsr_assignment_dql1_dqu1 = 0;
                    } else if (BitNew == 2) {
                      Ddr5Mr30.Bits.lfsr_assignment_dql2_dqu2 = 0;
                    } else if (BitNew == 3) {
                      Ddr5Mr30.Bits.lfsr_assignment_dql3_dqu3 = 0;
                    }
                  }
                }
              } //Bit loop

              if (IsX4Dimm (Socket, Ch, Dimm) == FALSE) {
                EncodedNibble = Nibble;
              } else {
                EncodedNibble = GetStrobeFromNibble (Host, Nibble);
              }

              SendMrwPda (Host, Socket, Ch, 1 << SubCh, Dimm, Rank, EncodedNibble, DDR5_MODE_REGISTER_30_REG, Ddr5Mr30.Data);
            }//Nibble loop
          } //SubCh loop
        } //rank loop
      } //dimm loop
    } //ch loop
  }//if
} //UpdateDramSeed



/*++

  Program Read Dfe Tap Bias

  @param[in] Host                      - Pointer to sysHost
  @param[in] Socket                    - Socket nubmer
  @param[in] DfeTap                    - Dfe Tap
  @param[in] TapBiasOffsetLoopCount    - Tap Bias Offset Loop Count
  @param[in] DfeTapSweepStepSize       - Dfe Tap Sweep Step Size
  @param[in] DfeTapSweepRange          - Dfe Tap Sweep Range
  @param[in] IterationLoop             - Iteration Loop
  @param[in] TapBiasSweepBeginValue    - Tap Bias Sweep Begin Value
  @param[in] Mode                      - Mode
  @param[in] BestTapBias               - Best Tap Bias

  @retval SUCCESS

--*/
VOID
ProgramReadDfeTapBias (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT8                 DfeTapIndex,
  IN INT8                  TapBiasOffsetLoopCount,
  IN UINT8                 DfeTapSweepStepSize,
  IN UINT8                 DfeTapSweepRange,
  IN UINT8                 IterationLoop,
  IN INT8                  TapBiasSweepBeginValue,
  IN DfeTapBiasProgramMode Mode,
  IN TapBias               (*BestTapBias) [MAX_CH][MAX_RANK_CH][MAX_BITS][MAX_TAP_NUM][MAX_ITERATION_LOOP]
  )
{
  UINT8                         Dimm;
  UINT8                         Rank;
  UINT8                         Ch;
  UINT8                         Bit;
  UINT8                         MaxBits = MAX_BITS_DDR5 / 2;
  struct dimmNvram              (*DimmNvList) [MAX_DIMM];
  UINT8                         MaxChDdr;
  UINT8                         SubCh = 0;
  UINT8                         Nibble = 0;
  UINT8                         EncodedNibble = 0;
  UINT8                         RankPresent;
  struct rankDevice             (*RankStruct) [MAX_RANK_DIMM];
  struct ddrRank                (*RankList) [MAX_RANK_DIMM];
  INT8                          DfeTapBiasValue = 0;
  UINT8                         BitNew = 0;
  INT16                         BitNibbleMin = 0;
  UINT8                         BitIdx = 0;
  UINT16                        BitCenter[8];
  INT16                         BitSkewMax = 0;
  INT16                         BitSkew[BITS_PER_NIBBLE];
  UINT8                         SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  MaxChDdr = GetMaxChDdr ();


  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      //
      // Initialize to no ranks present
      //
      RankPresent = 0;
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }

        if (Mode == SWEEP_PROGRAM_MODE) {
          RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Program Dfe DfeTap %d with value %d\n", DfeTapIndex + 1, TapBiasSweepBeginValue + (TapBiasOffsetLoopCount * DfeTapSweepStepSize));
        }

        if (Mode == NORMAL_PROGRAM_MODE) {

          if (BestTapBias == NULL) {
            RC_FATAL_ERROR (FALSE, ERR_MRC_POINTER, RC_FATAL_ERROR_MRC_POINTER_MINOR_CODE_006);
            RcDebugPrintWithDevice (SDBG_ERROR, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              "Error: Invalid NULL BestTapBias Pointer\n");
            return;
          }
        }

        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          for (Nibble = 0; Nibble < SubChMSVx4; Nibble++) {
            //
            // Skip if this is an ECC Nibble when ECC is disabled
            //
            if (IsNibbleNotValid (Host, Nibble)) {
              continue;
            }
            EncodedNibble = GetStrobeFromNibble (Host, Nibble);

            if (Mode == SWEEP_PROGRAM_MODE) {
              DfeTapBiasValue = TapBiasSweepBeginValue + (TapBiasOffsetLoopCount * DfeTapSweepStepSize);

              BitNibbleMin = DfeTapBiasValue;
              if (DfeTapIndex == TAP1_INDEX) {
                GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, EncodedNibble, 0, DdrLevel, RxDfeCoefficientTap1, GSM_UPDATE_CACHE, &BitNibbleMin);
              } else if (DfeTapIndex == TAP2_INDEX) {
                GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, EncodedNibble, 0, DdrLevel, RxDfeCoefficientTap2, GSM_UPDATE_CACHE, &BitNibbleMin);
              } else if (DfeTapIndex == TAP3_INDEX) {
                GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, EncodedNibble, 0, DdrLevel, RxDfeCoefficientTap3, GSM_UPDATE_CACHE, &BitNibbleMin);
              } else if (DfeTapIndex == TAP4_INDEX) {
                GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, EncodedNibble, 0, DdrLevel, RxDfeCoefficientTap4, GSM_UPDATE_CACHE, &BitNibbleMin);
              }


            } else if (Mode == NORMAL_PROGRAM_MODE) {
              if (BestTapBias == NULL) {
                RC_FATAL_ERROR (FALSE, ERR_MRC_POINTER, RC_FATAL_ERROR_MRC_POINTER_MINOR_CODE_006);
                RcDebugPrintWithDevice (SDBG_ERROR, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Error: Invalid NULL BestTapBias Pointer\n");
                return;
              }
              BitNibbleMin = NIBBLE_MIN_VALUE;
              for (Bit = 0; Bit < BITS_PER_NIBBLE; Bit++) {
                BitIdx = (Nibble * BITS_PER_NIBBLE) + Bit;
                DfeTapBiasValue = (*BestTapBias) [Ch][ (*RankList) [Rank].rankIndex][BitIdx + (SubCh * (MaxBits)) ][DfeTapIndex][IterationLoop].TapBiasValue[DfeTapIndex];
                BitNew = BitIdx;
                BitCenter[Bit] = DfeTapBiasValue;
                if (BitCenter[Bit] < BitNibbleMin) {
                  BitNibbleMin = BitCenter[Bit];
                }
              }
              RcDebugPrint (SDBG_MAX,
                "  %3d   ", BitNibbleMin);

              if (DfeTapIndex == TAP1_INDEX) {
                GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, EncodedNibble, 0, DdrLevel, RxDfeCoefficientTap1, GSM_UPDATE_CACHE, &BitNibbleMin);
              } else if (DfeTapIndex == TAP2_INDEX) {
                GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, EncodedNibble, 0, DdrLevel, RxDfeCoefficientTap2, GSM_UPDATE_CACHE, &BitNibbleMin);
              } else if (DfeTapIndex == TAP3_INDEX) {
                GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, EncodedNibble, 0, DdrLevel, RxDfeCoefficientTap3, GSM_UPDATE_CACHE, &BitNibbleMin);
              } else if (DfeTapIndex == TAP4_INDEX) {
                GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, EncodedNibble, 0, DdrLevel, RxDfeCoefficientTap4, GSM_UPDATE_CACHE, &BitNibbleMin);
              }
              BitSkewMax = 0;
              for (Bit = 0; Bit < BITS_PER_NIBBLE; Bit++) {
                BitSkew[Bit] = BitCenter[Bit] - BitNibbleMin;
                if (BitSkew[Bit] > BitSkewMax) {
                  BitSkewMax = BitSkew[Bit];
                }
                RcDebugPrint (SDBG_MAX,
                  "%3d ", BitSkew[Bit]);

              } // for (Bit)

              if (DfeTapIndex == TAP1_INDEX) {
                GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, EncodedNibble, ALL_BITS, DdrLevel, RxDfeCoefficientTap1PerBit, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET,
                  BitSkew);
              } else if (DfeTapIndex == TAP2_INDEX) {
                GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, EncodedNibble, ALL_BITS, DdrLevel, RxDfeCoefficientTap2PerBit, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET,
                  BitSkew);
              } else if (DfeTapIndex == TAP3_INDEX) {
                GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, EncodedNibble, ALL_BITS, DdrLevel, RxDfeCoefficientTap3PerBit, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET,
                  BitSkew);
              } else if (DfeTapIndex == TAP4_INDEX) {
                GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, EncodedNibble, ALL_BITS, DdrLevel, RxDfeCoefficientTap4PerBit, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET,
                  BitSkew);
              }
            }
          } //Bit loop
        } //subch loop
      } //Ch
    } //Rank
  }//Dimm

  return;
} // ProgramReadDfeTapBias

/**


  This function is to init the Tx dfe tap value before Tx DFE training starts.

  @param[IN] Host                - Pointer to sysHost
  @param[IN] Socket              - Socket number

  @retval N/A

**/

VOID
TxDfeTapInitDdr5 (
  IN PSYSHOST           Host,
  IN UINT8              Socket
  )
{
  MRC_STATUS  Status;
  MRC_TT                  OriginalTestType;
  UINT8 DfeTap = 0;
  INT8                 TapBiasSweepBegin = 0;

  Status = EnableDfe (Host, Socket, TxDfe);
  if (Status != MRC_STATUS_SUCCESS) {
    RcDebugPrintWithDevice (
      SDBG_ERROR, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Warning: failed to enable DRAM DFE circuit before Basic Host Tx DQ DQS sweep!\n"
      );
  }
  //
  // Program the Tap DFE coefficent.
  //
  OriginalTestType = GetCurrentTestType (Socket);
  SetCurrentTestType (Socket, WriteDfeTest);
  for (DfeTap = TAP1_INDEX; DfeTap <= TAP4_INDEX; DfeTap++) {
    if (DfeTap == TAP1_INDEX) {
      TapBiasSweepBegin = TXDFE_TAP1_INIT_VALUE;
    } else {
      TapBiasSweepBegin = TXDFE_TAP2_3_4_INIT_VALUE;
    }
    ProgramDfeTapBias (Host, Socket, DfeTap, 0, 0, 0, 0, TapBiasSweepBegin, SWEEP_PROGRAM_MODE, NULL);
  }
  SetCurrentTestType (Socket, OriginalTestType);
} // TxDfeTapInitDdr5