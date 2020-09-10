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

#include <Memory/JedecDefinitions.h>
#include "Ddr5Core.h"
//
// DCA DFE
//
#define DCA_DFE_COUNTING_WINDOW_VALUE 0x100

//
// DCA DFE Inner and outer loop definition
//
#define DCA_DFE_INNER_LOOP_STEP_SIZE                 1
#define DCA_DFE_INNER_LOOP_START_REPEATABILITY       0x139
#define DCA_DFE_INNER_LOOP_NUMBER_REPEATABILITY      398
#define DCA_DFE_INNER_LOOP_START_REPEATABILITY_CWV   0
#define DCA_DFE_INNER_LOOP_NUMBER_REPEATABILITY_CWV  63

#define DCA_DFE_GAIN_STEP_SIZE       2

#define DFE_TAP_STEP_SIZE                      1
#define DCA_DFE_REPEATABILITY                  3
#define DCA_DFE_OUTER_LOOP_START_REPEATABILITY 3

#define START_DFE_TRAINING_ACCELERATOR  1
#define STOP_DFE_TRAINING_ACCELERATOR   0
#define SELECT_INNER_LOOP_NULL          0
#define SELECT_INNER_LOOP_DFE_VREF      1
#define SELECT_INNER_LOOP_VREFCA        2
#define SELECT_OUTER_LOOP_NULL          0xFF
#define SELECT_OUTER_LOOP_TAP1          1
#define SELECT_OUTER_LOOP_TAP2          2
#define SELECT_OUTER_LOOP_TAP3          3
#define SELECT_OUTER_LOOP_TAP4          4
#define SELECT_OUTER_LOOP_UI            5

#define DCA_DFE_INNER_LOOP_255          0xFF
#define DCA_DFE_INNER_LOOP_256          0x100

#define TARGET_SEED_LFSR_5A             0x5A
#define NON_TARGET_SEED_LFSR_3C         0x3C

#define DCADFE_ITERATION_CHANGE_RANGE   1 //change the sweep range on the 2nd iteration. //HSD 16010478187

#define BITS0TO6                        0x7F
#ifdef USE_LATEST_RCD_SPEC
#define PAGE0OFFEST(Index, Offset)     (DDR5_RCD_PG0_RW60_DPAR_AND_DCA_6_0_RECEIVER_DFE_GAIN_OFFSET_REG + Index + Offset * 8)
#define PAGE1OFFEST(Index, Offset)     (DDR5_RCD_PG0_RW60_DPAR_AND_DCA_6_0_RECEIVER_DFE_GAIN_OFFSET_REG + Index + (A0A + Offset - A4A) * 8)
#endif //USE_LATEST_RCD_SPEC

GSM_CSN DcaSignalsToSweepDDR5[NUM_CA_SIGNALS_DDR5] = CHIP_GSM_CSN_SIGNALSTOSWEEP_SPR;
/*++

  Program Dfe Tap Bias

  @param[in] Host                   - Pointer to sysHost
  @param[in] Socket                 - Socket nubmer
  @param[in] DfeTap                 - Dfe Tap
  @param[in] StepSize               - Dfe Tap Sweep Step Size

  @retval N/A

--*/
VOID
ProgramDcaDfeInnerLoop (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT8                 DfeTap,
  IN UINT8                 StepSize
  );

/**

  Run the DDR5 DCA DFE Training.

  @param[in] Host         - Pointer to sysHost
  @param[in] TrainingMode - Training Mode

  @retval MRC_STATUS

**/
MRC_STATUS
DcaDfeTrainingDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     TrainingMode
  );

/*++

  DDR5 DFE Training

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Socket number
  @param[in] MemSsType    - Group Input
  @param[in] Dimm         - Dimm number
  @param[in] SubChMask    - Sub Channel Mask
  @param[in] Rank         - Rank number
  @param[in] Signal       - Signal
  @param[in] TrainingMode - Training Mode
  @param[in] GroupInput   - Group Input

  @retval MRC_STATUS

--*/
MRC_STATUS
DcaDfeTrainingMarginDdr5 (
  IN PSYSHOST      Host,
  IN UINT8         Socket,
  IN MRC_MST       MemSsType,
  IN UINT8         Dimm,
  IN UINT8         SubChMask,
  IN UINT8         Rank,
  IN GSM_CSN       Signal,
  IN UINT8         TrainingMode,
  IN MRC_GT        GroupInput
  );

/**

  This sets the DCA DFE training mode in the register.

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number
  @param[in] Ch              - Channel number
  @param[in] SubChannelMask  - Sub Channel Mask
  @param[in] Dimm            - Dimm number
  @param[in] Rank            - Rank number
  @param[in] Enable          - Enable Ca Training Mode

  @retval N/A

**/
VOID
SetDcaDfeTrainingModeDdr5 (
  IN PSYSHOST          Host,
  IN UINT8             Socket,
  IN UINT8             Ch,
  IN UINT8             SubChannelMask,
  IN UINT8             Dimm,
  IN UINT8             Rank,
  IN UINT8             Signal,
  IN BOOLEAN           Enable
  );
/*++

  Program Dfe Tap Bias

  @param[in] Host                      - Pointer to sysHost
  @param[in] Socket                    - Socket nubmer
  @param[in] DfeTrainingAccelerator    - Dfe Training Accelerator
  @param[in] SelectInnerLoopParameter  - Select Inner Loop Parameter
  @param[in] SelectOuterLoopParameter  - Select Outer Loop Parameter

  @retval N/A

--*/
VOID
ProgramLfsrDfeTrainingMode (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT8                 DfeTrainingAccelerator,
  IN UINT8                 SelectInnerLoopParameter,
  IN UINT8                 SelectOuterLoopParameter
  );

/**

  Find ca margins for the given Signal for rdimm.

  @param[in] Host                 - Pointer to sysHost
  @param[in] Socket               - Socket number
  @param[in] MemSsType            - Mmemory subsystem type
  @param[in] Dimm                 - DIMM number
  @param[in] Subch                - sub channel number.
  @param[in] Rank                 - Rank number
  @param[in] Signal               - Signal
  @param[in] TrainingMode         - The training mode. (parrallel or serial)

  @retval N/A

**/
VOID
GetDcaDfeMarginsSweep (
  IN PSYSHOST      Host,
  IN UINT8         Socket,
  IN MRC_MST       MemSsType,
  IN UINT8         Dimm,
  IN UINT8         Subch,
  IN UINT8         Rank,
  IN GSM_CSN       Signal,
  IN UINT8         TrainingMode
  );

/**

This routine is to program mode register value at the end of DDR5 Dca Dfe training.

@param[in] Host         - Pointer to sysHost
@param[in] socket       - Processor to initialize

@retval N/A

**/
VOID
ProgramLFSRSeed (
  IN PSYSHOST      Host,
  IN UINT8         Socket,
  IN UINT8         Ch,
  IN UINT8         Dimm,
  IN GSM_CSN       Signal
  );


/**

This routine is to program mode register value at the end of DDR5 Dca Dfe training.

@param[in] Host         - Pointer to sysHost
@param[in] socket       - Processor to initialize

@retval N/A

**/
VOID
ProgramModeRegisterValueDcaDfeEndDdr5 (
  IN PSYSHOST Host,
  IN UINT8 Socket
  )
{
#ifdef USE_LATEST_RCD_SPEC
  UINT8                                 Dimm;
  UINT8                                 Rank;
  struct dimmNvram                      (*DimmNvList) [MAX_DIMM];
  UINT8                                 Ch = 0;
  DDR5_RCD_DFE_CONFIGURATION_STRUCT     Ddr5RcdDfeConfigureation;
  DDR5_RCD_DPAR_AND_DCA_6_0_DFE_TRAINING_MODE_STRUCT Ddr5RcdDparDcaDfeTrainingMode;
  UINT32                                RcwValue = 0;

  Ddr5RcdDfeConfigureation.Data = 0;
  Ddr5RcdDfeConfigureation.Bits.dfe_feature_enable_control = 1;
  Ddr5RcdDfeConfigureation.Bits.tap_1_enable = 1;
  Ddr5RcdDfeConfigureation.Bits.tap_2_enable = 1;
  Ddr5RcdDfeConfigureation.Bits.tap_3_enable = 1;
  Ddr5RcdDfeConfigureation.Bits.tap_4_enable = 1;

  Ddr5RcdDparDcaDfeTrainingMode.Data = 0;
  Ddr5RcdDparDcaDfeTrainingMode.Bits.exit_from_rw_in_band_updated_mode = 1;

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

      for (Ch = 0; Ch < MAX_CH; Ch++) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if (((*DimmNvList) [Dimm].dimmPresent == 0)) {
          continue;
        }
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (Rank >= (*DimmNvList) [Dimm].numRanks) {
          continue;
        }
        RcwValue = Ddr5RcdDfeConfigureation.Data;
        WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE0, DDR5_RCD_DFE_CONFIGURATION_REG, &RcwValue);

        RcwValue = Ddr5RcdDparDcaDfeTrainingMode.Data;
        WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE0, DDR5_RCD_DPAR_AND_DCA_6_0_DFE_TRAINING_MODE_REG, &RcwValue);

      } //ch
    } //rank
  } //dimm
#endif
} //ProgramModeRegisterValueDcaDfeEndDdr5

/**

  DCA DFE Training for DDR5.

  @param[in] Host  - Pointer to sysHost

  @retval MRC_STATUS

**/
UINT32
EFIAPI
DcaDfeDdr5 (
  IN PSYSHOST  Host
  )
{
  UINT8                               Socket;
  UINT8                               Ch = 0;
  MRC_STATUS                          Status;
  CHIP_ERLY_CMD_CK_STRUCT             ChipErlyCmdCKStruct;
  UINT8                               TrainingMode = 0;
  UINT8                               MaxChDdr;
  CHANNEL_NVRAM_STRUCT (*ChannelNvList) [MAX_CH];


  MaxChDdr = GetMaxChDdr ();
  ZeroMem ((UINT8 *)&ChipErlyCmdCKStruct, sizeof (ChipErlyCmdCKStruct));

  Socket = Host->var.mem.currentSocket;

  Host->var.mem.checkMappedOutRanks = 1;
  //
  // Return if DDR5 MemSs is not present.
  //
  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "DcaDfeDdr5 Starts\n");
  if (!IsDdr5Present (Host, Socket)) {
    return FAILURE;
  }
  //
  // Return if this Socket is disabled
  //
  if (IsSocketDimmPopulated (Host, Socket) != TRUE) {
    return SUCCESS;
  }

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_DCA_DFE, Socket));

  //
  // If the mem flow bit is not enabled, or no 2DPC in this system, return.
  //
  if ((!IsMemFlowEnabled (RcdDcaDfeDdr5)) || (!GetTrainingStepEnableChip (Socket, CHECKPOINT_MINOR_DCA_DFE))) {
    return SUCCESS;
  }

  Status = TrainingModeSelect (RcdDcaDfeDdr5, &TrainingMode);
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
  // Execute the function for RDIMM, LRDIMM, NVM DIMM configs
  //
  if (!CheckRCDLoopback (Host)) {
    return SUCCESS;
  }
  ChannelNvList = GetChannelNvList (Host, Socket);

  if ((*ChannelNvList) [Ch].timingMode != TIMING_1N) {
    return SUCCESS;
  }
  //
  // Do the DDRIO INIT before starting training.
  //
  CsClkDdrioInit (Host, Socket);
  //
  // Set QLBD and QLBS IO Direction.
  //
  SetQlbdQlbsIodirection (Host, Socket, RX_PATH_UNGATE_DISABLE);

  EnableDisableBcom (Host, Socket, TRUE);
  SetCurrentTestType (Socket, DcaDfeDdr5Test);
  Host->var.mem.socket[Socket].TrainingStatus.Bits.TrainingPatternMode = CA_TRAINING_PATTERN_XTALK;
  //
  // Start the DCA DFE Training
  //
  DcaDfeTrainingDdr5 (Host, TrainingMode);
  EnableDisableBcom (Host, Socket, FALSE);
  Host->var.mem.checkMappedOutRanks = 0;
  SetCurrentTestType (Socket, 0);

  return SUCCESS;
} // DcaDfeDdr5

/**

  Display Dca Dfe Trainning Results.

  @param[in] Host   - Pointer to sysHost
  @param[in] Socket - Socket

  @retval MRC_STATUS

**/
VOID
DisplayDcaDfeTrainningResults (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
#ifdef USE_LATEST_RCD_SPEC
  UINT8                                                              Index;
  UINT8                                                              SignalOffset;
  UINT8                                                              RcwAddress;
  UINT8                                                              PageValue;
  UINT8                                                              RcwValue;
  UINT8                                                              Ch;
  UINT8                                                              Dimm;
  UINT8                                                              SubCh;
  UINT8                                                              MaxChDdr = GetMaxChDdr ();
  struct dimmNvram                                                   (*DimmNvList) [MAX_DIMM];
  CHAR8                                                             *Str[] = {"GAIN", "TAP1", "TAP2", "TAP3", "TAP4"};
  INT8                                                               OrigianlGain[] = {0, 6, 4, 2, 0, -2, -4, -6};
  DDR5_RCD_PG0_RW60_DPAR_AND_DCA_6_0_RECEIVER_DFE_GAIN_OFFSET_STRUCT Ddr5DcaDfeGainOffset;


  for (Index = 0; Index < sizeof (Str) / sizeof (Str[0]); Index++) {
    RcDebugPrint (SDBG_MEM_TRAIN, "START_DATA_RCD_DCA_DFE_%a\n", Str[Index]);
    RcDebugPrint (SDBG_MEM_TRAIN, "Signal:           A0   A1   A2   A3   A4   A5   A6  PAR\n");

    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }
      DimmNvList = GetDimmNvList (Host, Socket, Ch);

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList) [Dimm].dimmPresent == 0) {
          continue;
        }

        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, SubCh, NO_RANK, NO_STROBE, NO_BIT, "");

          for (SignalOffset = 0; SignalOffset < RCD_CA_SIGNALS_TO_SWEEP; SignalOffset++) {
            PageValue = SignalOffset > A3A - A0A ? RDIMM_CW_PAGE1 : RDIMM_CW_PAGE0;
            if (A0A + SignalOffset <= A3A) {
              RcwAddress = PAGE0OFFEST (Index, SignalOffset);
            } else if (A0A + SignalOffset <= A6A) {
              RcwAddress = PAGE1OFFEST (Index, SignalOffset);
            } else {
              RcwAddress = DDR5_RCD_PG0_RW78_DPAR_AND_DCA_6_0_RECEIVER_DFE_GAIN_OFFSET_REG + Index;
            }
            ReadRcdSmbByteDdr5 (Host, Socket, Ch, Dimm, SubCh, PageValue, RcwAddress, &RcwValue);
            if (AsciiStrCmp (Str[Index], "GAIN") == 0) {
              Ddr5DcaDfeGainOffset.Data = RcwValue;
              RcDebugPrint (SDBG_MEM_TRAIN, "%5d", OrigianlGain[Ddr5DcaDfeGainOffset.Bits.flat_band_dc_gain_adjustment]);
            } else {
              RcDebugPrint (SDBG_MEM_TRAIN, "%5d", RcwValue & BIT7 ? -(RcwValue & BITS0TO6) : RcwValue);
            }
          } //signal
          RcDebugPrint (SDBG_MEM_TRAIN, "\n");
        } //subch
      } //dimm
    } //ch
    RcDebugPrint (SDBG_MEM_TRAIN, "STOP_DATA_RCD_DCA_DFE_%a\n\n", Str[Index]);
  }
#endif //USE_LATEST_RCD_SPEC
}

/**

  Run the DDR5 DCA DFE Training.

  @param[in] Host         - Pointer to sysHost
  @param[in] TrainingMode - Training Mode

  @retval MRC_STATUS

**/
MRC_STATUS
DcaDfeTrainingDdr5 (
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
  struct dimmNvram (*DimmNvList) [MAX_DIMM];
  GSM_CSN             SignalToSweepCh;
  GSM_CSEDGE_CA      (*SignalEdgeData) [MAX_CH][SUB_CH][MAX_RANK_CH][RCD_CA_SIGNALS_TO_SWEEP];
  struct ddrRank (*RankList) [MAX_RANK_DIMM];
  INT16               Le[MAX_CH][SUB_CH];
  INT16               Re[MAX_CH][SUB_CH];
  UINT8               SubChannelMask = 0;
  UINT8               SubChTemp = 0;
  UINT8               MaxChDdr;

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
    "DcaDfeTrainingDdr5 Starts\n");

  Index = 0;
  if (TrainingMode == RUN_IN_PARALLEL) {
    SubChannelMask = TWO_SUBCH_MASK;
  }

  if (!UseSmbusMrw (Host, Socket)) {
    //
    // Set the RCD train feedback selection.
    //
    SetRcdTrainFeedbackSelectionAll (Host, Socket);
    //
    // Enable DFE, DFE VREF, Error Counter and Tap 1, 2, 3, 4.
    //
    DcaDfeInitDdr5 (Host, Socket, DcaDfeEnableTraining, A0A);
  }

  SetAepTrainingMode (Host, Socket, CHECKPOINT_MINOR_EARLY_CMD_CLK);

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
          //
          // Disable the Dimm 0 training state.
          //
          SetDcaDfeTrainingModeDdr5 (Host, Socket, Ch, SubChannelMask, 0, 0, 0, FALSE);
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
          // Get the signal index.
          //
          if (SubChannel == 0) {
            Index = Signal;
          } else {
            Index = Signal + ((RCD_CA_SIGNALS_TO_SWEEP * 2) - 1);
          }
          SignalToSweepCh = DcaSignalsToSweepDDR5[Index];

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
            RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
              "Calling GetDcaDfeMarginsSweep start \n");

            for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
              if (IsChannelEnabled (Socket, Ch) == FALSE) {
                continue;
              }

              DimmNvList = GetDimmNvList (Host, Socket, Ch);
              //
              // Enter the DCA DFE training Mode.
              //
              SetDcaDfeTrainingModeDdr5 (Host, Socket, Ch, SubChannelMask, Dimm, Rank, Signal, TRUE);
            } //Ch

            GetDcaDfeMarginsSweep (Host, Socket, MemSsType, Dimm, SubChannel, Rank, SignalToSweepCh, TrainingMode);
          }

          if (SignalToSweepCh == NO_SIGNAL) {
            continue;
          }
        } // Rank loop
      } // Signal loop

      RcDebugPrint (SDBG_MAX, "\n");
    } // Dimm loop
  } //Subchannel loop

  DisplayDcaDfeTrainningResults (Host, Socket);

#ifdef DEBUG_CODE_BLOCK
  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK

  //
  // Program the mode register value at the end of DCA DFE Training.
  //
  ProgramModeRegisterValueDcaDfeEndDdr5 (Host, Socket);
  FifoTrainReset (Host, Socket);
  IO_Reset (Host, Socket);
  SetAepTrainingMode (Host, Socket, DISABLE_TRAINING_STEP);
  Host->var.mem.checkMappedOutRanks = 0;
  RcFreePool (SignalEdgeData);
  return SUCCESS;
}//DcaDfeTrainingDdr5

/**
  This sets the CA training mode in the register.

  Supported modes are:

    DcaDfeEnableCircuit     - enable DFE ciruit for early training steps
    DcaDfeEnableTraining    - enable DFE training
    DcaDfeDisable           - disable DFE

  @param[in] Host           - Pointer to sysHost
  @param[in] Socket         - Socket number
  @param[in] Mode           - Enable/disable mode

  @retval MRC_STATUS_SUCCESS  Successfully enabled
  @retval other               An error occurred
**/
MRC_STATUS
DcaDfeInitDdr5 (
  IN PSYSHOST          Host,
  IN UINT8             Socket,
  IN DCA_DFE_MODE      Mode,
  IN GSM_CSN           Signal
  )
{
#ifdef USE_LATEST_RCD_SPEC
  UINT8                                               Dimm;
  UINT8                                               Ch;
  UINT8                                               Rank;
  UINT8                                               DfeFeatureEnableControl;
  UINT8                                               TapXEnable;
  UINT8                                               DfeVrefEnable;
  UINT8                                               ErrorCounterEnable;
  BOOLEAN                                             TrainingModeEnable;
  struct dimmNvram                                    (*DimmNvList) [MAX_DIMM];
  UINT8                                               RankEnabled;
  struct channelNvram                                 (*ChannelNvList) [MAX_CH];
  BOOLEAN                                             Enable;
  UINT8 DfePinSelection = 0;

  DDR5_RCD_DFE_CONFIGURATION_STRUCT Ddr5RcdDfeConfiguration;
  DDR5_RCD_DPAR_AND_DCA_6_0_DFE_TRAINING_MODE_STRUCT Ddr5RcdDparDcaDfeTrainingMode;

  ChannelNvList = GetChannelNvList (Host, Socket);

  if (Mode >= DcaDfeModeMax) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_187);
    return MRC_STATUS_INVALID_PARAMETER;
  }

  if (Mode == DcaDfeEnableCircuit || Mode == DcaDfeEnableTraining) {
    DfeFeatureEnableControl = 1;
    TapXEnable = 1;
  } else {
    DfeFeatureEnableControl = 0;
    TapXEnable = 0;
  }

  if (Mode == DcaDfeEnableTraining) {
    DfeVrefEnable = 1;
    ErrorCounterEnable = 1;
    TrainingModeEnable = TRUE;
    Enable = TRUE;

    if ((Signal == A0A) || (Signal == A0B)) {
      DfePinSelection = 0;
    } else if ((Signal == A1A) || (Signal == A1B)) {
      DfePinSelection = 1;
    }else if ((Signal == A2A) || (Signal == A2B)) {
      DfePinSelection = 2;
    }else if ((Signal == A3A) || (Signal == A3B)) {
      DfePinSelection = 3;
    }else if ((Signal == A4A) || (Signal == A4B)) {
      DfePinSelection = 4;
    }else if ((Signal == A5A) || (Signal == A5B)) {
      DfePinSelection = 5;
    }else if ((Signal == A6A) || (Signal == A6B)) {
      DfePinSelection = 6;
    }else if ((Signal == PARA) || (Signal == PARB)) {
      DfePinSelection = 7;
    }

  } else {
    DfeVrefEnable = 0;
    ErrorCounterEnable = 0;
    TrainingModeEnable = FALSE;
    Enable = FALSE;
  }

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
        ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_DFE_CONFIGURATION_REG, &Ddr5RcdDfeConfiguration.Data);
        ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_DPAR_AND_DCA_6_0_DFE_TRAINING_MODE_REG, &Ddr5RcdDparDcaDfeTrainingMode.Data);

        Ddr5RcdDfeConfiguration.Bits.dfe_feature_enable_control = DfeFeatureEnableControl;
        Ddr5RcdDfeConfiguration.Bits.dfe_vref_enable = DfeVrefEnable;
        Ddr5RcdDfeConfiguration.Bits.error_counter_enable = ErrorCounterEnable;
        Ddr5RcdDfeConfiguration.Bits.rw_control_word_writes_broadcast = 0;
        Ddr5RcdDfeConfiguration.Bits.tap_1_enable = TapXEnable;
        Ddr5RcdDfeConfiguration.Bits.tap_2_enable = TapXEnable;
        Ddr5RcdDfeConfiguration.Bits.tap_3_enable = TapXEnable;
        Ddr5RcdDfeConfiguration.Bits.tap_4_enable = TapXEnable;
        SendMrw (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_RCD_DFE_CONFIGURATION_REG, CW_RCD_DB, Ddr5RcdDfeConfiguration.Data);
        if (Mode != DcaDfeEnableCircuit) {

          Ddr5RcdDparDcaDfeTrainingMode.Bits.global_dfe_training_mode_enable = Enable ? 1 : 0;
          Ddr5RcdDparDcaDfeTrainingMode.Bits.training_source = Enable ? 2 : 0;
          Ddr5RcdDparDcaDfeTrainingMode.Bits.dcn_dfe_pin_selection = Enable ? DfePinSelection : 0;
          Ddr5RcdDparDcaDfeTrainingMode.Bits.rw_in_band_update_feature_enable = 0;
          Ddr5RcdDparDcaDfeTrainingMode.Bits.exit_from_rw_in_band_updated_mode = 1;
          SendMrw (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_RCD_DPAR_AND_DCA_6_0_DFE_TRAINING_MODE_REG, 1, Ddr5RcdDparDcaDfeTrainingMode.Data);
        }
        //
        //  Additional Filtering for DFE Training Mode Control Word. Use the default value 0. Don't program it.
        //
      } // Ch loop
    } // Rank loop
  } // Dimm loop
  return MRC_STATUS_SUCCESS;
#else // #ifdef USE_LATEST_RCD_SPEC
  return MRC_FUNCTION_NOT_IMPLEMENTED;
#endif // #ifdef USE_LATEST_RCD_SPEC
} // DcaDfeInitDdr5

/**

  This sets the DCA DFE training mode in the register.

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number
  @param[in] Ch              - Channel number
  @param[in] SubChannelMask  - Sub Channel Mask
  @param[in] Dimm            - Dimm number
  @param[in] Rank            - Rank number
  @param[in] Signal          - Signal
  @param[in] Enable          - Enable Ca Training Mode

  @retval N/A

**/
VOID
SetDcaDfeTrainingModeDdr5 (
  IN PSYSHOST          Host,
  IN UINT8             Socket,
  IN UINT8             Ch,
  IN UINT8             SubChannelMask,
  IN UINT8             Dimm,
  IN UINT8             Rank,
  IN UINT8             Signal,
  IN BOOLEAN           Enable
  )
{
#ifdef USE_LATEST_RCD_SPEC
  UINT8  SubChMaskLocal = SUB_CH_A_MASK;
  DDR5_RCD_DPAR_AND_DCA_6_0_DFE_TRAINING_MODE_STRUCT Ddr5RcdDparDcaDfeTrainingMode;
  if (SubChannelMask == TWO_SUBCH_MASK) {
    SubChMaskLocal = SUB_CH_A_MASK;
  } else {
    SubChMaskLocal = SubChannelMask;
  }
  ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_DPAR_AND_DCA_6_0_DFE_TRAINING_MODE_REG, &Ddr5RcdDparDcaDfeTrainingMode.Data);

  Ddr5RcdDparDcaDfeTrainingMode.Bits.global_dfe_training_mode_enable = Enable ? 1 : 0;
  Ddr5RcdDparDcaDfeTrainingMode.Bits.training_source = Enable ? 2 : 0;
  Ddr5RcdDparDcaDfeTrainingMode.Bits.dcn_dfe_pin_selection = Enable ? Signal : 0;
  Ddr5RcdDparDcaDfeTrainingMode.Bits.rw_in_band_update_feature_enable = 0;
  Ddr5RcdDparDcaDfeTrainingMode.Bits.exit_from_rw_in_band_updated_mode = 1;
  SendMrw (Host, Socket, Ch, SubChannelMask, Dimm, Rank, DDR5_RCD_DPAR_AND_DCA_6_0_DFE_TRAINING_MODE_REG, CW_RCD_DB, Ddr5RcdDparDcaDfeTrainingMode.Data);
#endif
} // SetDcaDfeTrainingModeDdr5

/**

  Get the DCA DFE training result.

  @param[in] Host                 - Pointer to sysHost
  @param[in] Socket               - Socket number
  @param[in] MemSsType            - Memory subsystem type
  @param[in] Dimm                 - DIMM number
  @param[in] Subch                - sub channel number.
  @param[in] Rank                 - Rank number
  @param[in] Signal               - Signal
  @param[in] TrainingMode         - The training mode. (parrallel or serial)

  @retval N/A

**/
VOID
GetDcaDfeMarginsSweep (
  IN PSYSHOST      Host,
  IN UINT8         Socket,
  IN MRC_MST       MemSsType,
  IN UINT8         Dimm,
  IN UINT8         Subch,
  IN UINT8         Rank,
  IN GSM_CSN       Signal,
  IN UINT8         TrainingMode
  )
{
  UINT8                       Ch;
  INT16                       PiIndex;
  UINT16                      EyeWidth;
  MRC_RT                      MrcRankList;
  INT16                       OrgDelay[MAX_CH][SUB_CH];
  UINT32                      RankMask[MAX_CH];
  UINT8                       ErrorResult[MAX_CH][SUB_CH][CA_CLK_TEST_RANGE];
  UINT16                      PiPosition[MAX_CH];
  UINT32                      ChBitmask = 0;
  UINT32                      Status = 0;
  UINT32                      OnechBitmask = 0;
  UINT8                       AlertFeedback[MAX_CH][SUB_CH][DCA_DFE_INNER_LOOP_NUMBER_REPEATABILITY + 1];
  UINT32                      ChannelMask = 0;
  UINT32                      Err = 0;
  UINT32                      ValidMask = 0;
  struct cmdEye               CurEye[MAX_CH][SUB_CH];
  struct dimmNvram            (*DimmNvList) [MAX_DIMM];
  GSM_CSN                     SignalLocal;
  UINT8                       SubChMask = 0;
  UINT8                       SubChTemp = 0;
  UINT8                       CaSubChGap = 0;
  UINT8                       InvertFlag[SUB_CH];
  UINT8                       MaxChDdr;
  UINT32                      CwvChBitmask = 0;
  UINT32                      RcdChBitmask = 0;
  INT16                       Le[MAX_CH][SUB_CH];
  INT16                       Re[MAX_CH][SUB_CH];
  UINT16                      OuterInnerLoopNumber = 0;

  MaxChDdr = GetMaxChDdr ();
  //
  // Program the RCD feedback selection.
  //
  SetRcdTrainFeedbackSelection (Host, Socket, Dimm, Rank, FIRST_ITERATION_LOOP);
  //
  // Enable DFE, DFE VREF, Error Counter and Tap 1, 2, 3, 4.
  //
  DcaDfeInitDdr5 (Host, Socket, DcaDfeEnableTraining, Signal);

  if (TrainingMode == RUN_IN_PARALLEL) {
    SubChMask = TWO_SUBCH_MASK;
    CaSubChGap = A0B - A0A;
  } else {
    SubChMask = 1 << Subch;
    CaSubChGap = 0;
  }
  ZeroMem ((UINT8 *) OrgDelay, sizeof (OrgDelay));
  ZeroMem ((UINT8 *) AlertFeedback, sizeof (AlertFeedback));
  ZeroMem ((UINT8 *) CurEye, sizeof (CurEye));
  ZeroMem ((UINT8 *) InvertFlag, sizeof (InvertFlag));

  IO_Reset (Host, Socket);
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
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

    //
    // Initialize Ch Rank mask
    //
    RankMask[Ch] = 1;
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
      CteSetupPattern ("DcaDfeDdr5Test", (Signal >= A0B) ? Signal - A0B : Signal - A0A, OnechBitmask, TWO_SUBCH_MASK, GetPhyRank (Dimm, Rank));
    } else {
      SetupTest (Host, Socket, MemSsType, OnechBitmask, 1, &MrcRankList, DdrLevel, DcaDfeDdr5Test, Signal, 0, FIRST_ITERATION_LOOP);
    }
    OnechBitmask = 0;

    //
    // Program the LFSR seed.
    //
    if (IsDcpmmPresentDimm (Socket, Ch, Dimm) == TRUE) {
      ProgramCaLfsrSeed (Host, Socket, Ch, Dimm, Signal);
    } else {
      ProgramLFSRSeed (Host, Socket, Ch, Dimm, Signal);
    }
  } // Ch loop
  //
  // Start the repeatability test
  //
  ProgramDfeGainBias (Host, Socket, 0, DcaDfe);

  //
  // Program the outer loop
  //
  ProgramDfeTapBias (Host, Socket, TAP1_INDEX, 0, DFE_TAP_STEP_SIZE, DCA_DFE_REPEATABILITY, 0, DCA_DFE_OUTER_LOOP_START_REPEATABILITY, SWEEP_PROGRAM_MODE, NULL);

  //
  // Program the inner loop value
  //
  ProgramDcaDfeInnerLoop (Host, Socket, TAP1_INDEX, DCA_DFE_INNER_LOOP_STEP_SIZE);

  //
  // Select the outer loop as none, inner loop as Figure of merit, and start the training accelerator.
  //
  ProgramLfsrDfeTrainingMode (Host, Socket, START_DFE_TRAINING_ACCELERATOR, SELECT_INNER_LOOP_DFE_VREF, SELECT_OUTER_LOOP_NULL);

  //
  // Wait 2 us for training accelerator to start
  //
  FixedDelayMicroSecond (2);

  //
  // Program counting window settings.
  //
  SetCountingWindow (Host, Socket, Dimm, Rank, DCA_DFE_COUNTING_WINDOW_VALUE);

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    //
    // Construct CwvChBitmask and RcdChBitmask for later consume
    //
    if (IsDcpmmPresentDimm (Socket, Ch, Dimm) == TRUE) {
      CwvChBitmask |= (1 << Ch);
    } else {
      RcdChBitmask |= (1 << Ch);
    }
  } // Ch loop

  //
  // OuterInnerLoopNumber = DCA_DFE_REPEATABILITY * DCA_DFE_INNER_LOOP_NUMBER_REPEATABILITY?
  //
  for (OuterInnerLoopNumber = 0; OuterInnerLoopNumber <= DCA_DFE_INNER_LOOP_NUMBER_REPEATABILITY; OuterInnerLoopNumber++) {
    //
    // Enable CS assertion trigger.
    //
    EnableCsaTrigger (Host, Socket, Dimm, Rank, CSA_TRIGGER_ENABLE);
    //
    // Enabling the counter Mode.
    //
    EnableDdrioTrainingCounter (Host, Socket, Dimm, Rank);

    ChBitmask = GetChBitmask (Host, Socket, Dimm, Rank);

    //
    // Stop TestControl on CWV slot channel once inner loop reach to CWV Tap inner loop range
    //
    if (OuterInnerLoopNumber >= DCA_DFE_INNER_LOOP_NUMBER_REPEATABILITY_CWV) {
      ChBitmask &= ~(CwvChBitmask);
    }

    //
    // Delay if this is a CTE build, otherwise do nothing.
    //
    CteDelayDclk (300);
    //
    // Reset the train counter value.
    //
    IO_Reset (Host, Socket);
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
        TestControl (Host, Socket, MemSsType, ChBitmask, SubChTemp, DcaDfeDdr5Test, 0, START_TEST, NOT_STOP_TEST);
      }
    }

    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
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
        if (Err > 0) { //to confirm with EV
          //
          // Fail
          //
          AlertFeedback[Ch][SubChTemp][OuterInnerLoopNumber] = 1;
        } else {
          //
          // Pass
          //
          AlertFeedback[Ch][SubChTemp][OuterInnerLoopNumber] = 0;
        }
        //
        // Add the RCSIM Feedback
        //
      } //subchtemp
    } // Ch loop
  } //OuterInnerLoopNumber
  //
  // Stop the training accelerator.
  //

  ProgramLfsrDfeTrainingMode (Host, Socket, STOP_DFE_TRAINING_ACCELERATOR, SELECT_INNER_LOOP_NULL, SELECT_OUTER_LOOP_NULL);
  //
  // Handle the training results.
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    if ((RankMask[Ch] == 0)) {
      continue;
    }
    for (SubChTemp = 0; SubChTemp < SUB_CH; SubChTemp++) {
      if (((SubChMask) & (1 << SubChTemp)) == 0) {
        continue;
      }
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChTemp, Rank, NO_STROBE, NO_BIT,
        "Sweep results for %5a -> ", GetSignalStrDdr5 ((Signal + SubChTemp * CaSubChGap)));
      for (PiIndex = 0; PiIndex < DCA_DFE_INNER_LOOP_NUMBER_REPEATABILITY + 1; PiIndex++) {

        if (AlertFeedback[Ch][SubChTemp][PiIndex]) {
          // '1'
          RcDebugPrint (SDBG_MAX, "1");
        } else {
          // '0'
          RcDebugPrint (SDBG_MAX, "0");
        }
      } // PiIndex loop

      RcDebugPrint (SDBG_MAX, "\n");
      //
      // Right -> Index End
      // Left -> Index Beginning
      //
      Status = FindErrorGraphMin (Host, Socket, Ch, Dimm, Rank, 0, &AlertFeedback[Ch][SubChTemp][0], DCA_DFE_INNER_LOOP_NUMBER_REPEATABILITY + 1,
                 (UINT16 *)&CurEye[Ch][SubChTemp].right, (UINT16 *)&CurEye[Ch][SubChTemp].left, (UINT16 *)&PiPosition[Ch]);

      if ((Status == FAILURE) || (CurEye[Ch][SubChTemp].left == CurEye[Ch][SubChTemp].right)) {
        CurEye[Ch][SubChTemp].left = 0;
        CurEye[Ch][SubChTemp].right = 0;
      }

    }//subchtemp
  }//Ch loop

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
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
    }//subchtemp
  } //Ch loop
  //
  // Start the normal DCA DFE training.
  //
  DcaDfeTrainingMarginDdr5 (Host, Socket, MemSsType, Dimm, SubChMask, Rank, Signal, TrainingMode, DcaDfe);
} // GetDcaDfeMarginsSweep

/*++

  DDR5 DCA DFE Training

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Socket number
  @param[in] MemSsType    - Memory subsystem type
  @param[in] Dimm         - Dimm number
  @param[in] SubChMask    - Sub channel number
  @param[in] Rank         - Rank number
  @param[in] Signal       - Signal
  @param[in] TrainingMode - Training Mode
  @param[in] GroupInput   - Group Input
                            DcaDfe : Do Dca Dfe Training


  @retval MRC_STATUS

--*/
MRC_STATUS
DcaDfeTrainingMarginDdr5 (
  IN PSYSHOST      Host,
  IN UINT8         Socket,
  IN MRC_MST       MemSsType,
  IN UINT8         Dimm,
  IN UINT8         SubChMask,
  IN UINT8         Rank,
  IN GSM_CSN       Signal,
  IN UINT8         TrainingMode,
  IN IN MRC_GT     GroupInput
  )
{
  UINT8                Ch = 0;
  UINT8                IterationLoop = 0;
  MRC_GT               DfeTap = TxDfeTap1;
  MRC_GT               DfeTapTemp = TxDfeTap1;
  INT8                 DfeTapSweepBegin = 0;
  UINT8                DfeTapSweepStepSize = 0;
  UINT8                DfeTapSweepRange = 0;
  UINT8                MaxTapBiasLoop = 0;
  UINT8                CwvTapSweepRange = 0;
  UINT8                TapBiasOffsetLoopCount = 0;
  UINT8                Bit = 0;
  UINT16               TapBiasBestCore = 0;
  UINT16               TapBiasScoreTemp = 0;
  UINT16               MaxTapBiasScore = 0;
  INT8                 TapBiasSweepBegin = 0;
  UINT16               MaxFigureofMerit = 0;
  UINT8                IterationLoopTemp = 0;
  MRC_GT               FinalBestTap = 0;
  UINT8                FinalBestIterationLoop = 0;
  UINT8                RankDone = 1;
  UINT8                AllChDone = 1;
  UINT8                ChDone[MAX_CH];
  DCADFE_EYEPOINT      (*FigureofMeritTap) [MAX_CH] [MAX_TAP_SWEEP_RANGE_DCADFE];
  DcaDfe_TapBias       (*BestTapBias) [MAX_CH] [MAX_TAP_NUM][MAX_ITERATION_LOOP_DCADFE];
  TapBiasValue         (*FinalTapBiasValue) [MAX_CH];
  INT8                 (*TapBiasCenter) [MAX_CH] [MAX_TAP_NUM][MAX_ITERATION_LOOP_DCADFE];
  INT8                 MinTapBias = 0;
  INT8                 MaxTapBias = 0;
  DfeTapBiasSweepRange (*DfeTapBiasSweepRangeInitValue) [MAX_TAP_NUM];
  INT8                 TapBiasValueTemp[MAX_TAP_NUM];
  UINT8                (*ChRankDone) [MAX_CH];
  UINT8                (*ChRankBitDone) [MAX_CH];
  UINT8                BitDone = 0;
  UINT8                TapIndex = 0;
  UINT16               BestFigureofMerit = 0;
  struct rankMargin    (*RankResults);
  struct channelNvram  (*ChannelNvList) [MAX_CH];
  struct dimmNvram     (*DimmNvList) [MAX_DIMM];
  UINT8                MaxChDdr = 0;
  SYS_SETUP           *Setup;
  GSM_CSN              SignalLocal;
  INT16                VrefHi = 0x5F;
  INT16                VrefLo = -0x5F;
  UINT8                DfeGainBias = 0;
  UINT8                MBV = MAX_BITS_DDR5;
  UINT8                MaxGainBiasValue = 0;
  UINT8                GainBiasBeginValue = 0;
  INT8                 BestTapBiasValueTemp = 0;
  UINT16               OuterInnerLoopNumber = 0;
  UINT32               ChBitmask = 0;
  UINT32               CwvChBitmask = 0;
  UINT32               RcdChBitmask = 0;
  UINT8                SubChTemp = 0;
  UINT32               ChannelMask = 0;
  UINT8                CaSubChGap = 0;
  UINT32               Err = 0;
  UINT32               ValidMask = 0;
  UINT8                LocalLoop = 0;
  UINT8                (*AlertFeedback) [MAX_CH][SUB_CH][DCA_DFE_INNER_LOOP_NUMBER_REPEATABILITY + 1];
  UINT32               RcwValue = 0;
  UINT8                IterationLoop2 = 0;
  UINT16                PiIndex;
  UINT32               Status = 0;
  struct cmdEye        CurEye[MAX_CH][SUB_CH];
  UINT16               PiPosition[MAX_CH];
  INT16                Le[MAX_CH][SUB_CH];
  INT16                Re[MAX_CH][SUB_CH];
  UINT16               EyeWidth;
  UINT8                BestLocalLoop = 0;
  UINT8                SubChInput = 0;
  UINT8                RcwAddressTap = 0;
  UINT8                PageValue = 0;
  UINT16               InnerLoopRange = 0;
  BOOLEAN              MaxTapBiasLoopIsOverride = FALSE;

  if (Signal < A0B) {
    SubChInput = 0;
  } else {
    SubChInput = 1;
  }
  Setup = GetSysSetupPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  if (Host->nvram.mem.socket[Socket].maxDimmPop == 0) {
    return SUCCESS;
  }

  FigureofMeritTap = RcAllocatePool (sizeof (*FigureofMeritTap));
  if (FigureofMeritTap == NULL) {
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

  RankResults = RcAllocatePool (sizeof (*RankResults));
  if (RankResults == NULL) {
    ASSERT (FALSE);
    return FAILURE;
  }

  AlertFeedback = RcAllocatePool (sizeof (*AlertFeedback));
  if (AlertFeedback == NULL) {
    ASSERT (FALSE);
    return FAILURE;
  }

  ZeroMem ((UINT8 *) (*FigureofMeritTap), sizeof (*FigureofMeritTap));
  ZeroMem ((UINT8 *) (*BestTapBias), sizeof (*BestTapBias));
  ZeroMem ((UINT8 *) (TapBiasCenter), sizeof (*TapBiasCenter));
  ZeroMem ((UINT8 *) (FinalTapBiasValue), sizeof (*FinalTapBiasValue));
  ZeroMem ((UINT8 *) (DfeTapBiasSweepRangeInitValue), sizeof (*DfeTapBiasSweepRangeInitValue));
  ZeroMem ((UINT8 *) (ChRankDone), sizeof (*ChRankDone));
  ZeroMem ((UINT8 *) (ChRankBitDone), sizeof (*ChRankBitDone));
  ZeroMem ((UINT8 *) (ChDone), sizeof (ChDone));
  ZeroMem ((UINT8 *) (RankResults), sizeof (*RankResults));
  ZeroMem ((UINT8 *) (TapBiasValueTemp), sizeof (TapBiasValueTemp));
  ZeroMem ((UINT8 *) AlertFeedback, sizeof (*AlertFeedback));

#ifdef DEBUG_CODE_BLOCK
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "DcaDfeTrainingMarginDdr5 starts\n");
#endif //DEBUG_CODE_BLOCK

  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    //
    // Program the LFSR seed.
    //
    if (IsDcpmmPresentDimm (Socket, Ch, Dimm) == TRUE) {
      ProgramCaLfsrSeed (Host, Socket, Ch, Dimm, Signal);
    } else {
      ProgramLFSRSeed (Host, Socket, Ch, Dimm, Signal);
    }

    //
    // Construct CwvChBitmask and RcdChBitmask for later consume
    //
    if (IsDcpmmPresentDimm (Socket, Ch, Dimm) == TRUE) {
      CwvChBitmask |= (1 << Ch);
    } else {
      RcdChBitmask |= (1 << Ch);
    }
  } // Ch loop

  //
  // DCA DFE init.
  //
  DfeTapBiasSweepRangeInit (DfeTapBiasSweepRangeInitValue, GroupInput);


  //
  // Get DFE GAIN BIAS Value.
  //
  GetDfeGainBias (GroupInput, &MaxGainBiasValue, &GainBiasBeginValue);

  //
  // Dfe Gain Bias Loop
  //
  for (DfeGainBias = GainBiasBeginValue; DfeGainBias <= MaxGainBiasValue; DfeGainBias =+ DCA_DFE_GAIN_STEP_SIZE) {
    //
    // Program the DFE Gain Bias.
    //
    ProgramDfeGainBias (Host, Socket, DfeGainBias, GroupInput);

    //
    // Iteration Loop
    //
    for (IterationLoop = FIRST_ITERATION_LOOP; IterationLoop < MAX_ITERATION_LOOP_DCADFE; IterationLoop++) {
      if (IterationLoop >= 1) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "DcaDfeTrainingMarginDdr5 IterationLoop >=1 starts\n");
      }
      //
      // Tap Loop
      //
      for (DfeTap = TAP_NUM_INDEX (DcaDfeTap1, DcaDfeTap1); DfeTap <= TAP_NUM_INDEX (DcaDfeTap4, DcaDfeTap1); DfeTap++) {
        //
        // Get the DFE tap sweep begin, step size and range.
        //
        GetDcaDfeTapSweepParameter (Host, Socket, DfeTap, IterationLoop, &DfeTapSweepRange, &DfeTapSweepStepSize, TapBiasCenter, DfeTapBiasSweepRangeInitValue, &TapBiasSweepBegin);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Dfe Tap  %d Iteration Loop  %d Tap Sweep starts  %d, ends  %d with step size  %d\n", DfeTap + 1, IterationLoop, TapBiasSweepBegin, DfeTapSweepRange, DfeTapSweepStepSize);

        MaxTapBiasLoop = (DfeTapSweepRange / DfeTapSweepStepSize);
        MaxTapBiasLoopIsOverride = FALSE;
        if (IsDcpmmPresentSocket (Socket) == TRUE) {
          switch (DfeTap + DcaDfeTap1) {
            case DcaDfeTap1:
              CwvTapSweepRange = CWV_DCA_DFE_OUTER_LOOP_SWEEP_RANGE_TAP1;
              break;
            case DcaDfeTap2:
              CwvTapSweepRange = CWV_DCA_DFE_OUTER_LOOP_SWEEP_RANGE_TAP2;
              break;
            case DcaDfeTap3:
              CwvTapSweepRange = CWV_DCA_DFE_OUTER_LOOP_SWEEP_RANGE_TAP3;
              break;
            case DcaDfeTap4:
              CwvTapSweepRange = CWV_DCA_DFE_OUTER_LOOP_SWEEP_RANGE_TAP4;
              break;
            default:
              break;
          }
          if ((DfeTapSweepRange / DfeTapSweepStepSize) < CwvTapSweepRange) {
            MaxTapBiasLoop = CwvTapSweepRange;
            MaxTapBiasLoopIsOverride = TRUE;
          }
        }

        //
        // Program the outer loop start, step size and number of increments.
        //
        ProgramDfeTapBias (Host, Socket, DfeTap, TapBiasOffsetLoopCount, DfeTapSweepStepSize, DfeTapSweepRange, IterationLoop, TapBiasSweepBegin, SWEEP_PROGRAM_MODE, NULL);

        //
        // Program the inner loop value
        //
        ProgramDcaDfeInnerLoop (Host, Socket, DfeTap, DCA_DFE_INNER_LOOP_STEP_SIZE);

        //
        // Wait 2 us for training accelerator to start
        //
        FixedDelayMicroSecond (2);

        //
        // Select the outer loop, inner loop as Figure of merit, and start the training accelerator.
        //
        ProgramLfsrDfeTrainingMode (Host, Socket, START_DFE_TRAINING_ACCELERATOR, SELECT_INNER_LOOP_DFE_VREF, DfeTap);

        //
        // Program counting window settings.
        //
        SetCountingWindow (Host, Socket, Dimm, Rank, DCA_DFE_COUNTING_WINDOW_VALUE);
        for (LocalLoop = 0; LocalLoop < MaxTapBiasLoop; LocalLoop++) {
          //
          // OuterInnerLoopNumber = DCA_DFE_REPEATABILITY * DCA_DFE_INNER_LOOP_NUMBER_REPEATABILITY?
          //
          RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, SubChInput, Rank, NO_STROBE, NO_BIT,
            "Tap: %d Loop Number: %d, RCD Tap Value: %d \n", DfeTap + 1, LocalLoop, TapBiasSweepBegin + LocalLoop*DfeTapSweepStepSize);
          if ((IsDcpmmPresentSocket (Socket) == TRUE) && (LocalLoop < CwvTapSweepRange)) {
            RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, SubChInput, Rank, NO_STROBE, NO_BIT,
              "CWV Tap: %d Value: %d \n", DfeTap + 1, LocalLoop + CWV_DCA_DFE_OUTER_LOOP_START);
          }
          for (OuterInnerLoopNumber = 0; OuterInnerLoopNumber <= DCA_DFE_INNER_LOOP_NUMBER_REPEATABILITY; OuterInnerLoopNumber++) {
            //
            // Enable CS assertion trigger.
            //
            EnableCsaTrigger (Host, Socket, Dimm, Rank, CSA_TRIGGER_ENABLE);

            //
            // Enabling the counter Mode.
            //
            EnableDdrioTrainingCounter (Host, Socket, Dimm, Rank);
            ChBitmask = GetChBitmask (Host, Socket, Dimm, Rank);

            //
            // Stop TestControl on RCD slot channel if current outer loop is greater than Rcd Tap outer range
            //
            if ((MaxTapBiasLoopIsOverride) && (LocalLoop >= (DfeTapSweepRange / DfeTapSweepStepSize))) {
              ChBitmask &= ~(RcdChBitmask);
            }

            //
            // Stop TestControl on CWV slot channel if current outer loop is greater than CWV Tap outer range
            //
            if (LocalLoop >= CwvTapSweepRange) {
              ChBitmask &= ~(CwvChBitmask);
            }

            //
            // Stop TestControl on CWV slot channel once inner loop reach to CWV Tap inner loop range
            //
            if (OuterInnerLoopNumber >= DCA_DFE_INNER_LOOP_NUMBER_REPEATABILITY_CWV) {
              ChBitmask &= ~(CwvChBitmask);
            }

            //
            // Delay if this is a CTE build, otherwise do nothing.
            //
            CteDelayDclk (300);
            IO_Reset (Host, Socket);

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
                TestControl (Host, Socket, MemSsType, ChBitmask, SubChTemp, DcaDfeDdr5Test, 0, START_TEST, NOT_STOP_TEST);
              }
            }

            for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
              if (IsChannelEnabled (Socket, Ch) == FALSE) {
                continue;
              }
              //
              // Stop collecting feedback once CWV Dimm channel reach CWV inner sweep range (63)
              //
              if ((IsDcpmmPresentDimm (Socket, Ch, Dimm) == TRUE) && (OuterInnerLoopNumber > DCA_DFE_INNER_LOOP_NUMBER_REPEATABILITY_CWV)) {
                continue;
              }

              //
              // Stop collecting feedback once RCD Dimm channel reach RCD outer sweep range
              //
              if (MaxTapBiasLoopIsOverride) {
                if ((IsDcpmmPresentDimm (Socket, Ch, Dimm) == FALSE) && (LocalLoop >= (DfeTapSweepRange / DfeTapSweepStepSize))) {
                  continue;
                }
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
                if (Err > 0) { //to confirm with EV
                  //
                  // Fail
                  //
                  (*AlertFeedback) [Ch][SubChTemp][OuterInnerLoopNumber] = 1;
                } else {
                  //
                  // Pass
                  //
                  (*AlertFeedback) [Ch][SubChTemp][OuterInnerLoopNumber] = 0;
                }
                //
                // To add the RCSIM Feedback.
                //
              } //subchtemp
            } // Ch loop
          } // OuterInnerLoopNumber

          //
          // Handle the training results.
          //
          for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
            if (IsChannelEnabled (Socket, Ch) == FALSE) {
              continue;
            }
            if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
              continue;
            }

            //
            // Stop Handle the training results once CWV Dimm channel reach CWV inner sweep range (63)
            //
            if ((IsDcpmmPresentDimm (Socket, Ch, Dimm) == TRUE) && (LocalLoop >= CwvTapSweepRange)) {
              continue;
            }

            //
            // Stop Handle the training results once RCD Dimm channel reach RCD outer sweep range
            //
            if (MaxTapBiasLoopIsOverride) {
              if ((IsDcpmmPresentDimm (Socket, Ch, Dimm) == FALSE) && (LocalLoop >= (DfeTapSweepRange / DfeTapSweepStepSize))) {
                continue;
              }
            }

            //
            // Update sweep range for result print out
            //
            if (IsDcpmmPresentDimm (Socket, Ch, Dimm) == TRUE) {
              InnerLoopRange = DCA_DFE_INNER_LOOP_NUMBER_REPEATABILITY_CWV;
            } else {
              InnerLoopRange = DCA_DFE_INNER_LOOP_NUMBER_REPEATABILITY;
            }

            for (SubChTemp = 0; SubChTemp < SUB_CH; SubChTemp++) {
              if (((SubChMask) & (1 << SubChTemp)) == 0) {
                continue;
              }
              RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChTemp, Rank, NO_STROBE, NO_BIT,
                "Sweep results for %5a -> ", GetSignalStrDdr5 ((Signal + SubChTemp * CaSubChGap)));
              for (PiIndex = 0; PiIndex <= InnerLoopRange; PiIndex++) {

                if ((*AlertFeedback) [Ch][SubChTemp][PiIndex]) {
                  // '1'
                  RcDebugPrint (SDBG_MAX, "1");
                } else {
                  // '0'
                  RcDebugPrint (SDBG_MAX, "0");
                }
              } // PiIndex loop

              RcDebugPrint (SDBG_MAX, "\n");
              //
              // Right -> Index End
              // Left -> Index Beginning
              //
              Status = FindErrorGraphMin (Host, Socket, Ch, Dimm, Rank, 0, &(*AlertFeedback) [Ch][SubChTemp][0], InnerLoopRange + 1,
                         (UINT16 *)&CurEye[Ch][SubChTemp].right, (UINT16 *)&CurEye[Ch][SubChTemp].left, (UINT16 *)&PiPosition[Ch]);

              if ((Status == FAILURE) || (CurEye[Ch][SubChTemp].left == CurEye[Ch][SubChTemp].right)) {
                CurEye[Ch][SubChTemp].left = 0;
                CurEye[Ch][SubChTemp].right = 0;
              }
            } // Subchtemp loop
          } // Ch loop

          for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
            if (IsChannelEnabled (Socket, Ch) == FALSE) {
              continue;
            }
            if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
              continue;
            }
            for (SubChTemp = 0; SubChTemp < SUB_CH; SubChTemp++) {
              if (((SubChMask) & (1 << SubChTemp)) == 0) {
                continue;
              }

              //
              // Stop Handle the training results once CWV Dimm channel reach CWV inner sweep range (63)
              //
              if ((IsDcpmmPresentDimm (Socket, Ch, Dimm) == TRUE) && (LocalLoop >= CwvTapSweepRange)) {
                continue;
              }

              //
              // Stop Handle the training results once RCD Dimm channel reach RCD outer sweep range
              //
              if (MaxTapBiasLoopIsOverride) {
                if ((IsDcpmmPresentDimm (Socket, Ch, Dimm) == FALSE) && (LocalLoop >= (DfeTapSweepRange / DfeTapSweepStepSize))) {
                  continue;
                }
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
                "Left Edge: %d, Right Edge: %d  Eye Width: %d\n", Le[Ch][SubChTemp], Re[Ch][SubChTemp], EyeWidth);

              if (EyeWidth <= MinimumMargin (Host, CA)) {//To update this check.SPR to do.
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
              (*FigureofMeritTap)[Ch][LocalLoop].FigureofMerit = (UINT8)(Re[Ch][SubChTemp] - Le[Ch][SubChTemp]); //to update
            } // Subchtemp loop
          } // Ch loop
        } // LocalLoop

        //
        // Get the best tap bias setting and program the best tap gain bias.
        //
        RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "DcaDfe Program the best tap gain bias\n");

        ProgramLfsrDfeTrainingMode (Host, Socket, STOP_DFE_TRAINING_ACCELERATOR, SELECT_INNER_LOOP_NULL, SELECT_OUTER_LOOP_NULL);
        for (Ch = 0; Ch < MAX_CH; Ch++) {
          //
          // Skip if channel is disabled
          //
          if (IsChannelEnabled (Socket, Ch) == FALSE) {
            continue;
          }
          if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
            continue;
          }
          VrefHi = 0x5F;
          VrefLo = -0x5F;
          if (FILTERING == 1) {
            TapBiasBestCore = (*FigureofMeritTap) [Ch][0].FigureofMerit +
              (*FigureofMeritTap) [Ch][0].FigureofMerit +
              (*FigureofMeritTap) [Ch][0].FigureofMerit;
          } else if (FILTERING == 3) {
            TapBiasBestCore = (*FigureofMeritTap) [Ch][0].FigureofMerit +
              (*FigureofMeritTap) [Ch][0].FigureofMerit +
              (*FigureofMeritTap) [Ch][1].FigureofMerit;
          }
          BestFigureofMerit = (*FigureofMeritTap) [Ch][0].FigureofMerit;
          (*BestTapBias) [Ch][DfeTap][IterationLoop].FigureofMerit = BestFigureofMerit;
          (*BestTapBias) [Ch][DfeTap][IterationLoop].Score = TapBiasBestCore;

          if (IsDcpmmPresentDimm (Socket, Ch, Dimm) == FALSE) {
            (*BestTapBias) [Ch][DfeTap][IterationLoop].TapBiasValue[DfeTap] = TapBiasSweepBegin;
            MaxTapBiasLoop = (DfeTapSweepRange / DfeTapSweepStepSize);
          } else {
            (*BestTapBias) [Ch][DfeTap][IterationLoop].TapBiasValue[DfeTap] = CWV_DCA_DFE_OUTER_LOOP_START;
            MaxTapBiasLoop = CwvTapSweepRange;
          }
          for (TapBiasOffsetLoopCount = 0; TapBiasOffsetLoopCount <= MaxTapBiasLoop; TapBiasOffsetLoopCount++) {
            //
            // Find the best figure of merit
            //
            if (FILTERING == 1) {
              if (BestFigureofMerit < (*FigureofMeritTap) [Ch][TapBiasOffsetLoopCount].FigureofMerit) {
                BestFigureofMerit = (*FigureofMeritTap) [Ch][TapBiasOffsetLoopCount].FigureofMerit;
                if (IsDcpmmPresentDimm (Socket, Ch, Dimm) == FALSE) {
                  (*BestTapBias) [Ch][DfeTap][IterationLoop].TapBiasValue[DfeTap] = TapBiasSweepBegin + (TapBiasOffsetLoopCount * DfeTapSweepStepSize);
                } else {
                  (*BestTapBias) [Ch][DfeTap][IterationLoop].TapBiasValue[DfeTap] = CWV_DCA_DFE_OUTER_LOOP_START + (TapBiasOffsetLoopCount * DfeTapSweepStepSize);
                }
                (*BestTapBias) [Ch][DfeTap][IterationLoop].FigureofMerit = BestFigureofMerit;
              }
            } else if (FILTERING == 3) {
              if (TapBiasOffsetLoopCount == 0) {
                TapBiasScoreTemp = (*FigureofMeritTap) [Ch][0].FigureofMerit +
                  (*FigureofMeritTap) [Ch][0].FigureofMerit +
                  (*FigureofMeritTap) [Ch][1].FigureofMerit;
              } else if ((TapBiasOffsetLoopCount > 0) && (TapBiasOffsetLoopCount < MaxTapBiasLoop)) {
                TapBiasScoreTemp = (*FigureofMeritTap) [Ch][TapBiasOffsetLoopCount - 1].FigureofMerit +
                  (*FigureofMeritTap) [Ch][TapBiasOffsetLoopCount].FigureofMerit +
                  (*FigureofMeritTap) [Ch][TapBiasOffsetLoopCount + 1].FigureofMerit;
              } else if (TapBiasOffsetLoopCount == MaxTapBiasLoop) {
                TapBiasScoreTemp = (*FigureofMeritTap) [Ch][TapBiasOffsetLoopCount - 1].FigureofMerit +
                  (*FigureofMeritTap) [Ch][TapBiasOffsetLoopCount].FigureofMerit +
                  (*FigureofMeritTap) [Ch][TapBiasOffsetLoopCount].FigureofMerit;
              }
              if (TapBiasScoreTemp > TapBiasBestCore) {
                TapBiasBestCore = TapBiasScoreTemp;
                BestFigureofMerit = (*FigureofMeritTap) [Ch][TapBiasOffsetLoopCount].FigureofMerit;
                if (IsDcpmmPresentDimm (Socket, Ch, Dimm) == FALSE) {
                  (*BestTapBias) [Ch][DfeTap][IterationLoop].TapBiasValue[DfeTap] = TapBiasSweepBegin + (TapBiasOffsetLoopCount * DfeTapSweepStepSize);
                } else {
                  (*BestTapBias) [Ch][DfeTap][IterationLoop].TapBiasValue[DfeTap] = CWV_DCA_DFE_OUTER_LOOP_START + (TapBiasOffsetLoopCount * DfeTapSweepStepSize);
                }
                (*BestTapBias) [Ch][DfeTap][IterationLoop].FigureofMerit = BestFigureofMerit;
                (*BestTapBias) [Ch][DfeTap][IterationLoop].Score = TapBiasBestCore;
                BestLocalLoop = TapBiasOffsetLoopCount;
#if MAX_ITERATION_LOOP_DCADFE > 1
                if (IterationLoop < MAX_ITERATION_LOOP_DCADFE - 1) {
                  (*TapBiasCenter) [Ch][DfeTap][IterationLoop + 1] = (*BestTapBias) [Ch][DfeTap][IterationLoop].TapBiasValue[DfeTap];
                }
#endif
              }
            }
          }
          //
          // Initialzie the Tap Bias value for the iteration loop
          //
          BestTapBiasValueTemp = (*BestTapBias) [Ch][DfeTap][IterationLoop].TapBiasValue[DfeTap];

          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChInput, Rank, NO_STROBE, NO_BIT,
            "The Best Tap Settings for Tap %d: %d, Loop %d\n", DfeTap + 1, BestTapBiasValueTemp, BestLocalLoop);
          (*BestTapBias) [Ch][DfeTapTemp][IterationLoop].TapBiasValue[DfeTap] = BestTapBiasValueTemp;
          //
          // Program the best tap coeffcient.
          //
          if (Signal <= A3A)  {
            RcwAddressTap = DDR5_RCD_PG0_RW61_DPAR_AND_DCA_6_0_RECEIVER_DFE_TAP_1_COEFFICIENTS_REG + DfeTap + ((Signal -A0A) * 8) & 0xFF;
            PageValue = RDIMM_CW_PAGE0;
          } else if (Signal <= A6A) {
            RcwAddressTap = DDR5_RCD_PG0_RW61_DPAR_AND_DCA_6_0_RECEIVER_DFE_TAP_1_COEFFICIENTS_REG + DfeTap + ((Signal -A4A) * 8) & 0xFF;
            PageValue = RDIMM_CW_PAGE1;
          } else if (Signal <= PARA) {
            RcwAddressTap = DDR5_RCD_PG0_RW79_DPAR_AND_DCA_6_0_RECEIVER_DFE_TAP_1_COEFFICIENTS_REG + DfeTap & 0xFF;
            PageValue = RDIMM_CW_PAGE1;
          } else  if (Signal <= A3B)  {
            RcwAddressTap = DDR5_RCD_PG0_RW61_DPAR_AND_DCA_6_0_RECEIVER_DFE_TAP_1_COEFFICIENTS_REG + DfeTap + ((Signal -A0B) * 8) & 0xFF;
            PageValue = RDIMM_CW_PAGE0;
          } else if (Signal <= A6B) {
            RcwAddressTap = DDR5_RCD_PG0_RW61_DPAR_AND_DCA_6_0_RECEIVER_DFE_TAP_1_COEFFICIENTS_REG + DfeTap + ((Signal -A4B) * 8) & 0xFF;
            PageValue = RDIMM_CW_PAGE1;
          } else if (Signal <= PARB) {
            RcwAddressTap = DDR5_RCD_PG0_RW79_DPAR_AND_DCA_6_0_RECEIVER_DFE_TAP_1_COEFFICIENTS_REG + DfeTap & 0xFF;
            PageValue = RDIMM_CW_PAGE1;
          } else {
            RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, SubChInput, NO_RANK, NO_STROBE, NO_BIT,
              "Signal %d is not valid\n", Signal);
            RC_FATAL_ERROR (FALSE, ERR_RC_DCA_DFE, ERR_INVALID_SIGNAL);
          }

          RcwValue = BestTapBiasValueTemp >= 0 ? BestTapBiasValueTemp : (-BestTapBiasValueTemp) | 0x80;
          WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, 1 << SubChInput, BYTE_WRITE, PageValue, RcwAddressTap & 0xFF, &RcwValue);
          //
          // Need to program the new  vref center
          //
          DfeTapSweepBegin = TapBiasSweepBegin;
        } // ch loop
        //} // bit loop
        //
        // Program the Best Tap Bias Value for this Tap
        //
        //HSD HSD 1507720418To create a dcadfe dedicated function ProgramDfeTapBias
        //
        // Display the training value
        //
#ifdef DEBUG_CODE_BLOCK
        if (GetDebugLevel () & SDBG_MAX) {
          //HSD 1507720418 To create a dcadfe dedicated function  to DisplayDfeTapSweepResults
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
          if ((*ChRankBitDone) [Ch] == 1) {
            BitDone = BitDone + 1;
            continue;
          }
          MaxFigureofMerit = (*BestTapBias) [Ch][TAP1_INDEX][0].FigureofMerit;
          for (IterationLoopTemp = 0; IterationLoopTemp <= IterationLoop ; IterationLoopTemp++) {
            for (DfeTap = TAP1_INDEX; DfeTap <= TAP4_INDEX; DfeTap++) {

              if ((*BestTapBias) [Ch][DfeTap][IterationLoopTemp].FigureofMerit > MaxFigureofMerit) {
                MaxFigureofMerit = (*BestTapBias) [Ch][DfeTap][IterationLoopTemp].FigureofMerit;
              }
            }
          }
          if (MaxFigureofMerit > FIGURE_OF_MERIT_MASK) {
            (*ChRankBitDone) [Ch] = 1;
            BitDone = BitDone + 1;
          } else {
            MaxTapBias = (*BestTapBias) [Ch][TAP1_INDEX][IterationLoop].TapBiasValue[TAP1_INDEX];
            MinTapBias = (*BestTapBias) [Ch][TAP1_INDEX][IterationLoop].TapBiasValue[TAP1_INDEX];
            for (IterationLoopTemp = IterationLoop - (NO_CHANGE_ITERATIONS - 1); IterationLoopTemp <= IterationLoop; IterationLoopTemp++) {
              for (DfeTap = TAP1_INDEX; DfeTap <= TAP4_INDEX; DfeTap++) {
                if (MaxTapBias < (*BestTapBias) [Ch] [DfeTap][IterationLoop].TapBiasValue[DfeTap]) {
                  MaxTapBias = (*BestTapBias) [Ch] [DfeTap][IterationLoop].TapBiasValue[DfeTap];
                }
                if (MinTapBias > (*BestTapBias) [Ch] [DfeTap][IterationLoop].TapBiasValue[DfeTap]) {
                  MinTapBias = (*BestTapBias) [Ch] [DfeTap][IterationLoop].TapBiasValue[DfeTap];
                }
              } //DfeTap
            }
            if ((MaxTapBias - MinTapBias) < TAP_CHANGE_THRESHOLD) {
              (*ChRankBitDone) [Ch] = 1;
              BitDone = BitDone + 1;
            }
          }
          if (Host->nvram.mem.eccEn) { //SPR_todo.HSD 1507720418
            if (BitDone == MBV) {
              (*ChRankDone) [Ch] = 1;
            } else {
              (*ChRankDone) [Ch] = 0;
              RankDone = 0;
            }
          } else {
            if (BitDone == MBV - 16) {
              (*ChRankDone) [Ch] = 1;
            } else {
              (*ChRankDone) [Ch] = 0;
              RankDone = 0;
            }
          }
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

    if (AllChDone == 1) {
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
        if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
          continue;
        }
        VrefHi = 0x5F;
        VrefLo = -0x5F;
        MaxFigureofMerit = (*BestTapBias) [Ch][TAP1_INDEX][0].FigureofMerit;
        MaxTapBiasScore = (*BestTapBias) [Ch][TAP1_INDEX][0].Score;
        FinalBestTap = TAP1_INDEX;
        if (IterationLoop < MAX_ITERATION_LOOP_DCADFE) {
          IterationLoop2 = IterationLoop;
        } else {
          IterationLoop2 = MAX_ITERATION_LOOP_DCADFE - 1;
        }
        for (IterationLoopTemp = 0; IterationLoopTemp <= IterationLoop2; IterationLoopTemp++) {
          for (DfeTap = TAP1_INDEX; DfeTap <= TAP4_INDEX; DfeTap++) {
            if ((*BestTapBias) [Ch] [DfeTap][IterationLoopTemp].Score > MaxTapBiasScore) {
              MaxFigureofMerit = (*BestTapBias) [Ch] [DfeTap][IterationLoopTemp].FigureofMerit;
              MaxTapBiasScore = (*BestTapBias) [Ch] [DfeTap][IterationLoopTemp].Score;
              FinalBestTap = DfeTap;
              FinalBestIterationLoop = IterationLoopTemp;
            }
          }
        }
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, Bit,
          "The best Iteration Loop %d, Tap %d\n", FinalBestIterationLoop, FinalBestTap + 1);
        for (TapIndex = 0; TapIndex < MAX_TAP_NUM; TapIndex++) {
          (*FinalTapBiasValue) [Ch].TapBiasValue[TapIndex] = (*BestTapBias) [Ch][FinalBestTap][FinalBestIterationLoop].TapBiasValue[TapIndex];
        }
        //
        // Program the final results.
        //
        // HSD 1507720418 to create dcadfe dedicated function  ProgramDfeTapBiasPerbit
      } // ch loop
      //
      // display the final trained value
      //
      //HSD 1507720418to create dedicated function DisplayDfeTapFinalResults
      break;
    }
  } //DfeGainBias loop
  RcFreePool (FigureofMeritTap);
  RcFreePool (BestTapBias);
  RcFreePool (TapBiasCenter);
  RcFreePool (FinalTapBiasValue);
  RcFreePool (DfeTapBiasSweepRangeInitValue);
  RcFreePool (ChRankDone);
  RcFreePool (ChRankBitDone);
  RcFreePool (RankResults);
  RcFreePool (AlertFeedback);

  return SUCCESS;
} // DcaDfeTrainingMarginDdr5

/*++

  Program Dfe Tap Bias

  @param[in] Host                   - Pointer to sysHost
  @param[in] Socket                 - Socket nubmer
  @param[in] DfeTap                 - Dfe Tap
  @param[in] StepSize               - Dfe Tap Sweep Step Size

  @retval N/A

--*/
VOID
ProgramDcaDfeInnerLoop (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT8                 DfeTapIndex,
  IN UINT8                 StepSize
  )
{
#ifdef USE_LATEST_RCD_SPEC
  UINT8                         Dimm;
  UINT8                         Rank;
  UINT8                         Ch;
  struct dimmNvram              (*DimmNvList) [MAX_DIMM];
  UINT8                         MaxChDdr;
  struct rankDevice             (*RankStruct) [MAX_RANK_DIMM];
  struct ddrRank                (*RankList) [MAX_RANK_DIMM];
  UINT8                         RW36InnerLoopStart = 0;
  UINT8                         RW37InnerLoopStart = 0;
  UINT16                        LocalLoopNumber;


  DDR5_RCD_DFETM_INNER_LOOP_START_VALUE_STRUCT Ddr5RcdDfetmInnerLoop;
  DDR5_RCD_DFETM_INNER_AND_OUTER_LOOP_STEP_SIZE_STRUCT Ddr5RcdDfetmInnerOuterLoopStepSize;
  DDR5_RCD_DFETM_INNER_LOOP_NUMBER_OF_INCREMENTS_STRUCT Ddr5RcdDfetmInnerLoopNumberOfIncrements;
  DDR5_RCD_DFETM_OUTER_LOOP_START_VALUE_STRUCT Ddr5RcdDfetmOutlerLoopStartValue;
  DDR5_RCD_DFETM_OUTER_LOOP_NUMBER_OF_INCREMENTS_STRUCT Ddr5RcdDfetmOuterLoopnumber;

  MaxChDdr = GetMaxChDdr ();

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }
        RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }

        //
        // Fill in InnerLoopStartValue setting for either RCD or CWV
        //
        if (IsDcpmmPresentDimm (Socket, Ch, Dimm) == FALSE) {
          RW37InnerLoopStart = ((DCA_DFE_INNER_LOOP_START_REPEATABILITY & BIT8) == BIT8) ? 1 : 0;
          RW36InnerLoopStart = DCA_DFE_INNER_LOOP_START_REPEATABILITY & 0xFF;
          LocalLoopNumber = DCA_DFE_INNER_LOOP_NUMBER_REPEATABILITY;
        } else {
          RW36InnerLoopStart = DCA_DFE_INNER_LOOP_START_REPEATABILITY_CWV;
          LocalLoopNumber = DCA_DFE_INNER_LOOP_NUMBER_REPEATABILITY_CWV;
        }

        if ((DfeTapIndex == TAP1_INDEX) || (DfeTapIndex == TAP2_INDEX) || (DfeTapIndex == TAP3_INDEX) || (DfeTapIndex == TAP4_INDEX)) {
          //
          // Program the start value
          //
          Ddr5RcdDfetmInnerLoop.Data = 0;
          Ddr5RcdDfetmInnerLoop.Bits.inner_loop_start_value = RW36InnerLoopStart;
          SendMrw (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_RCD_DFETM_INNER_LOOP_START_VALUE_REG, CW_RCD_DB, Ddr5RcdDfetmInnerLoop.Data);

          //
          // CWV does not have additional range
          //
          if (IsDcpmmPresentDimm (Socket, Ch, Dimm) == FALSE) {
            ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_DFETM_OUTER_LOOP_START_VALUE_REG, &Ddr5RcdDfetmOutlerLoopStartValue.Data);
            Ddr5RcdDfetmOutlerLoopStartValue.Bits.rw_36_inner_loop_start_value = RW37InnerLoopStart;
            SendMrw (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_RCD_DFETM_OUTER_LOOP_START_VALUE_REG, CW_RCD_DB, Ddr5RcdDfetmOutlerLoopStartValue.Data);
          }

          //
          // Program the step size
          //
          ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_DFETM_INNER_AND_OUTER_LOOP_STEP_SIZE_REG, &Ddr5RcdDfetmInnerOuterLoopStepSize.Data);
          Ddr5RcdDfetmInnerOuterLoopStepSize.Bits.inner_loop_step_size = StepSize - 1;
          SendMrw (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_RCD_DFETM_INNER_AND_OUTER_LOOP_STEP_SIZE_REG, CW_RCD_DB, Ddr5RcdDfetmInnerOuterLoopStepSize.Data);

          //
          // Program Inner Loop Number of Increments for RCD only since CWV didn't support extend range of inner loop
          //
          if (IsDcpmmPresentDimm (Socket, Ch, Dimm) == FALSE) {
            if (DCA_DFE_INNER_LOOP_NUMBER_REPEATABILITY > DCA_DFE_INNER_LOOP_255) {
              ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_DFETM_OUTER_LOOP_NUMBER_OF_INCREMENTS_REG, &Ddr5RcdDfetmOuterLoopnumber.Data);
              Ddr5RcdDfetmOuterLoopnumber.Bits.rw3b_8_inner_loop_number_of_increments = 1;
              SendMrw (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_RCD_DFETM_OUTER_LOOP_NUMBER_OF_INCREMENTS_REG, CW_RCD_DB, Ddr5RcdDfetmOuterLoopnumber.Data);
              LocalLoopNumber = DCA_DFE_INNER_LOOP_NUMBER_REPEATABILITY - DCA_DFE_INNER_LOOP_256;
            }
          }
          Ddr5RcdDfetmInnerLoopNumberOfIncrements.Data = 0;
          Ddr5RcdDfetmInnerLoopNumberOfIncrements.Bits.inner_loop_number_of_increments = LocalLoopNumber & DCA_DFE_INNER_LOOP_255;
          SendMrw (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_RCD_DFETM_INNER_LOOP_NUMBER_OF_INCREMENTS_REG, CW_RCD_DB, Ddr5RcdDfetmInnerLoopNumberOfIncrements.Data);
        }
      } // Ch loop
    } // Rank loop
  }// Dimm loop
#endif
  return;
} // ProgramDcaDfeInnerLoop

/*++

  Program Dfe Tap Bias

  @param[in] Host                      - Pointer to sysHost
  @param[in] Socket                    - Socket nubmer
  @param[in] DfeTrainingAccelerator    - Dfe Training Accelerator
  @param[in] SelectInnerLoopParameter  - Select Inner Loop Parameter
  @param[in] SelectOuterLoopParameter  - Select Outer Loop Parameter

  @retval N/A

--*/
VOID
ProgramLfsrDfeTrainingMode (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT8                 DfeTrainingAccelerator,
  IN UINT8                 SelectInnerLoopParameter,
  IN UINT8                 SelectOuterLoopParameter
  )
{
  UINT8                         Dimm;
  UINT8                         Rank;
  UINT8                         Ch;
  struct dimmNvram              (*DimmNvList) [MAX_DIMM];
  UINT8                         MaxChDdr;
  struct rankDevice             (*RankStruct) [MAX_RANK_DIMM];
  struct ddrRank                (*RankList) [MAX_RANK_DIMM];
  UINT8 OuterLoopValue = 0;

  DDR5_RCD_LFSR_DFE_TRAINING_MODE_STRUCT Ddr5RcdLfsrDfeTrainingMode;

  MaxChDdr = GetMaxChDdr ();

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }
        RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }
        //
        // Program the DDR5 RCD LFSR DFE TRAINING MODE
        //
        ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_LFSR_DFE_TRAINING_MODE_REG, &Ddr5RcdLfsrDfeTrainingMode.Data);
        Ddr5RcdLfsrDfeTrainingMode.Bits.dfe_training_accelerator = DfeTrainingAccelerator;

        if (IsDcpmmPresentDimm (Socket, Ch, Dimm) == FALSE) {
          //
          // Pick DFE_Vref for RCD Margin Monitoy
          //
          Ddr5RcdLfsrDfeTrainingMode.Bits.select_inner_loop_parameter = SelectInnerLoopParameter;
        } else {
          //
          // Update even_odd_sampler_offset BIT7 to enable/disable CWV odd/even sample offset
          //
          if (SelectInnerLoopParameter == SELECT_INNER_LOOP_NULL) {
            Ddr5RcdLfsrDfeTrainingMode.Bits.select_inner_loop_parameter = SelectInnerLoopParameter;
            Ddr5RcdLfsrDfeTrainingMode.Data &= ~(BIT7);
          } else {
            Ddr5RcdLfsrDfeTrainingMode.Data |= BIT7;
          }
        }

        if (SelectOuterLoopParameter == SELECT_OUTER_LOOP_NULL) {
          OuterLoopValue = 0;
        } else {
          OuterLoopValue = SelectOuterLoopParameter + 1;
        }
        Ddr5RcdLfsrDfeTrainingMode.Bits.select_outer_loop_parameter = OuterLoopValue;
        SendMrw (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_RCD_LFSR_DFE_TRAINING_MODE_REG, CW_RCD_DB, Ddr5RcdLfsrDfeTrainingMode.Data);
      } // Ch loop
    } // Rank loop
  }// Dimm loop
  return;
} // ProgramLfsrDfeTrainingMode

/**

  Program LFSR Seed.

  @param[in] Host                 - Pointer to sysHost
  @param[in] Socket               - Socket number
  @param[in] Ch                   - Ch number
  @param[in] Dimm                 - DIMM number
  @param[in] Signal               - Signal

  @retval N/A

**/
VOID
ProgramLFSRSeed (
  IN PSYSHOST      Host,
  IN UINT8         Socket,
  IN UINT8         Ch,
  IN UINT8         Dimm,
  IN GSM_CSN       Signal
  )
{
  UINT32 TargetValue = TARGET_SEED_LFSR_5A;
  UINT32 NonTargetValue = NON_TARGET_SEED_LFSR_3C;
  switch (Signal) {
    case A0A:
    case A0B:
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW63_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &TargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW67_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW6B_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW6F_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW73_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW77_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW7B_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW7F_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      break;
    case A1A:
    case A1B:
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW63_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW67_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &TargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW6B_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW6F_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW73_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW77_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW7B_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW7F_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      break;
    case A2A:
    case A2B:
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW63_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW67_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW6B_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &TargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW6F_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW73_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW77_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW7B_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW7F_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      break;
    case A3A:
    case A3B:
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW63_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW67_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW6B_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW6F_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &TargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW73_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW77_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW7B_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW7F_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      break;
    case A4A:
    case A4B:
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW63_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW67_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW6B_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW6F_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW73_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &TargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW77_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW7B_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW7F_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      break;
    case A5A:
    case A5B:
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW63_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW67_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW6B_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW6F_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW73_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW77_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &TargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW7B_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW7F_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      break;
    case A6A:
    case A6B:
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW63_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW67_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW6B_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW6F_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW73_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW77_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW7B_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &TargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW7F_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      break;
    case PARA:
    case PARB:
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW63_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW67_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW6B_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW6F_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW73_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW77_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW7B_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &NonTargetValue);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE2, DDR5_RCD_PG2_RW7F_LFSR_SEED_FOR_DFE_TRAINING_MODE_REG, &TargetValue);
      break;
    default:
      break;
  }
}


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
GetDcaDfeTapSweepParameter (
  IN     PSYSHOST                Host,
  IN     UINT8                   Socket,
  IN     UINT8                   DfeTapIndex,
  IN     UINT8                   IterationLoop,
  IN OUT UINT8                   *DfeTapSweepRange,
  IN OUT UINT8                   *DfeTapSweepStepSize,
  IN OUT INT8                    (*TapBiasCenter) [MAX_CH][MAX_TAP_NUM][MAX_ITERATION_LOOP_DCADFE],
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
  INT8                TapSweepBegin = 0;
  INT8                TapSweepEnd = 0;
  INT8                TapSweepBeginMin = 0;
  INT8                TapSweepEndMax = 0;

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
    *DfeTapSweepStepSize = DFE_TAP_STEP_SIZE;
  } else {
    if (IterationLoop >= DCADFE_ITERATION_CHANGE_RANGE) {
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
      *DfeTapSweepStepSize = DFE_TAP_STEP_SIZE;
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
      for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
        //
        // Skip if no rank or if it is mapped out
        //
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        TapSweepBeginMin = (((*TapBiasCenter) [Ch][DfeTapIndex][IterationLoop]) - ((*DfeTapSweepRange) / 2));
        TapSweepEndMax = (((*TapBiasCenter) [Ch][DfeTapIndex][IterationLoop]) + ((*DfeTapSweepRange) / 2));
        if (IterationLoop == FIRST_ITERATION_LOOP) {
          switch (DfeTapIndex) {
            case TAP1_INDEX:
              (*TapBiasCenter) [Ch][DfeTapIndex][IterationLoop] = ((*DfeTapBiasSweepRangeInitValue) [0].End + (*DfeTapBiasSweepRangeInitValue) [0].Begin) / 2;
              break;
            case TAP2_INDEX:
              (*TapBiasCenter) [Ch][DfeTapIndex][IterationLoop] = ((*DfeTapBiasSweepRangeInitValue) [1].End + (*DfeTapBiasSweepRangeInitValue) [1].Begin) / 2;
              break;
            case TAP3_INDEX:
              (*TapBiasCenter) [Ch][DfeTapIndex][IterationLoop] = ((*DfeTapBiasSweepRangeInitValue) [2].End + (*DfeTapBiasSweepRangeInitValue) [2].Begin) / 2;
              break;
            case TAP4_INDEX:
              (*TapBiasCenter) [Ch][DfeTapIndex][IterationLoop] = ((*DfeTapBiasSweepRangeInitValue) [3].End + (*DfeTapBiasSweepRangeInitValue) [3].Begin) / 2;
              break;
            default:
              (*TapBiasCenter) [Ch][DfeTapIndex][IterationLoop] = ((*DfeTapBiasSweepRangeInitValue) [0].End + (*DfeTapBiasSweepRangeInitValue) [0].Begin) / 2;
              break;
          }
        } else {
          TapSweepBegin = (((*TapBiasCenter) [Ch][DfeTapIndex][IterationLoop]) - ((*DfeTapSweepRange) / 2));
          TapSweepEnd = (((*TapBiasCenter) [Ch][DfeTapIndex][IterationLoop]) + ((*DfeTapSweepRange) / 2));
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
      } // rank loop
    } // dimm loop
  } // ch loop
  *DfeTapSweepRange = TapSweepEndMax - TapSweepBeginMin;
} // GetDcaDfeTapSweepParameter
