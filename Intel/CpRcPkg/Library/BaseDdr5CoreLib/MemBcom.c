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
#include <Library/MemoryCoreLib.h>
#include <Library/MemoryServicesLib.h>
#include <Memory/Ddr5RcwRegs.h>
#include <Memory/JedecDefinitions.h>
#include <Library/MemRcLib.h>
#include <Base.h>

#define CA_WRAP_AROUND_VALUE  256
#define BACKSIDE_CA_WRAP_AROUND_VALUE  128
#define PER_GROUP             0
#define COMPOSITE_GROUPS      1
#define NO_SIGNAL             0xff
#define BACKSIDE_BCOM_WRAP_AROUND_VALUE     128
#define BACKSIDE_BCOM_FULL_EYE_WIDTH          64

// Counting window is programmed to DQ_COUNTING_WINDOW_VALUE / 2
// For BCOM training, feedback is sent on DQ only after 4 samples of BCOM signals,
//  so we need to divide the window by another 4
#define MAX_BCOM_FEEDBACK_OUTPUT    DQ_COUNTING_WINDOW_VALUE / 8

//
// Min and Max values we have BCOM VREF feedback for in RcSim.
//

#define MIN_RCSIM_BCOM_VREF_VALUE   74  // 74% of Vdd
#define MAX_RCSIM_BCOM_VREF_VALUE   76  // 76% of Vdd

//
// Default value for BCOM VREF as defined
// in the training HAS.
//

#define BCOM_VREF_DEFAULT_VALUE     75  // 75% of Vdd

//
// NUM_BCOM_VREF_VALUES is used to set the number of records we reserve memory for.
//

#define NUM_BCOM_VREF_VALUES (MAX_BCOM_VREF_VALUE - BCOM_VREF_INIT_VALUE + 1)

typedef struct _SSA_MARGIN_BCOM {
  COMMON_1D_PARAMETERS      CommonParameters;
  MRC_TT                    TestType;
  UINT32                    CountWindow;
  struct baseMargin         BcomTimimg[MAX_CH][SUB_CH];
} SSA_MARGIN_BCOM;

/**

This function print the sample array

@param[in] Host      - Pointer to sysHost
@param[in] Socket    - Socket number
@param[in] Ch        - Channel number
@param[in] Dimm      - DIMM number
@param[in] ReadMode  - 5 = LRDIMM Write MWD Delay
                   4 = LRDIMM Read DQ DQS
                   3 = LRDIMM Read Recieve Enable Phase
                   2 = LRDIMM Write DWL
                   1 = Read DqDqs
                   0 = Write DqDqs
@param[in] Subchannel - Sub channel number
@param[in] Length     - Print range
@param[in] *TrainRes  - Training result
@retval N/A

**/
extern VOID
PrintSampleArrayCsClk (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    ReadMode,
  IN UINT8    Subchannel,
  IN UINT16   Length,
  IN struct   ClkTrainingResults(*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

  /**

For each data module, a 128 bit vector of zeros and ones holding the tests results is
evaluated which is a concatenation of the two reads from the feedback CRs.
BIOS locates the middle of the longest consecutive stream of ones (when the vector is evaluated as cyclic,
this location in the vector indicates the PI setting and logic delay for that data module).

@param[in] Host              - Pointer to sysHost
@param[in] Socket            - Current Socket
@param[in] Ch                - Channel number
@param[in] Dimm              - Dimm number on Ch
@param[in] Rank              - Rank on Dimm
@param[in] Strobe            - Strobe number
@param[in] RisingEdge        - Pointer to save the rising edge
@param[in] centerPoint       - Pointer to save the center point
@param[in] FallingEdge       - Pointer to save the falling edge
@param[in] PulseWidth        - Pointer to save the pulse width
@param[in] Step              - The PI Step to work on:
                               PI_CS_CLK_TYPE
@param[in] SubChannel        - sub channel number
@param[in] *TrainResinput    - Pointer to ClkTrainingResults


@retval N/A

**/
extern VOID
EvaluatePiSettingResultsCsClk (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     Strobe,
  IN UINT16    RisingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN UINT16    FallingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN UINT8     Step,
  IN UINT8     Subchannel,
  IN MRC_TT    CurrentTestType,
  IN struct    ClkTrainingResults(*TrainResinput) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

GSM_CSN SignalsToSweepBcomDDR5[2] = CHIP_GSM_CSN_BCOM_SIGNALSTOSWEEP_SPR;
//
// Local Prototypes
//
/**
  Clears bwSerr

  @param[in out] bwSerr:       Error status

  @retval N/A
**/
extern VOID
ClearBwSerr (
  IN OUT UINT32 BwSerr[MAX_CH][3]
  );

/**

Exectues the BCOM training.

@param[in] Host - Pointer to sysHost

@retval Status

**/
UINT32 VrefBcomTraining (
  IN PSYSHOST  Host
  );

/**

Finds CA CLK margins for the given Signal by the CA Vref setting.

@param[in]      Host        - Pointer to sysHost
@param[in]      Socket      - Socket number
@param[in]      Dimm        - Dimm number
@param[in]      Rank        - Rank number
@param[in]      SignalSubCh - Sub channel signal
@param[in]      Vref        - Voltage reference
@param[in]      CsLeft      - Left edge
@param[in]      CsRight     - Right edge
@param[in]      EyeWidth    - Eye width
@param[in]      CsValue     - Cs value
@param[in][out] *TrainRes   - Training result

@retval N/A

**/
VOID
GetBcomClkMarginsSweepVref (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN GSM_CSN   SignalSubCh[MAX_CH][SUB_CH],
  IN INT8      Vref,
  IN INT16     CsLeft[MAX_CH][SUB_CH],
  IN INT16     CsRight[MAX_CH][SUB_CH],
  IN INT16     EyeWidth[MAX_CH][SUB_CH],
  IN INT16     CsValue[MAX_CH][SUB_CH],
  IN struct    ClkTrainingResults(*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

/**

This function print the sample array

@param[in] Host       - Pointer to sysHost
@param[in] Socket     - Socket number
@param[in] Ch         - Channel number
@param[in] Dimm       - DIMM number
@param[in] ReadMode   - 5 = LRDIMM Write MWD Delay
                    4 = LRDIMM Read DQ DQS
                    3 = LRDIMM Read Recieve Enable Phase
                    2 = LRDIMM Write DWL
                    1 = Read DqDqs
                    0 = Write DqDqs
@param[in] Subchannel - Sub channel number
@param[in] Length     - Print range
@param[in] *TrainRes  - Training result

@retval N/A

**/
VOID
PrintSampleArrayBcomClk (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    ReadMode,
  IN UINT8    Subchannel,
  IN UINT16   Length,
  IN struct   ClkTrainingResults(*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8            Strobe;
  UINT8            MaxStrobe;
  UINT32           PiSetting;
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

  RcDebugPrintLine (SDBG_DEFAULT, 23, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  if (!(Host->nvram.mem.eccEn)) {
    RcDebugPrint(SDBG_DEFAULT, "       0   1   2   3\n");
  } else {
    RcDebugPrint(SDBG_DEFAULT, "       0   1   2   3   4\n");
  }
  MaxStrobe = MAX_STROBE_DDR5 / 4;

  for (PiSetting = 0; PiSetting < Length; PiSetting++) {
    if ((ReadMode > 1) && (PiSetting % 2)) {
      continue;
    }
    if (((ReadMode == PRINT_ARRAY_TYPE_LRDIMM_MDQ_RD_DELAY) || (ReadMode == PRINT_ARRAY_TYPE_LRDIMM_MDQ_WR_DELAY)) && (PiSetting % 4)) continue;
    if ((ReadMode == PRINT_ARRAY_TYPE_LRDIMM_MDQ_RD_DELAY) || (ReadMode == PRINT_ARRAY_TYPE_LRDIMM_MDQ_WR_DELAY)) {
      RcDebugPrint(SDBG_DEFAULT, "%d ", (((INT32) PiSetting / 4) - 15));

      if ((((INT32) PiSetting / 4 - 15) >= 0) && (((INT32) PiSetting / 4 - 15) < 10)) {
        RcDebugPrint(SDBG_DEFAULT, "  ");
      }

      if ((((INT32) PiSetting / 4 - 15) < 100) && (((INT32) PiSetting / 4 - 15) > 9)) {
        RcDebugPrint(SDBG_DEFAULT, " ");
      }

      if ((((INT32) PiSetting / 4 - 15) < 0) && (((INT32) PiSetting / 4 - 15) > -10)) {
        RcDebugPrint(SDBG_DEFAULT, " ");
      }

    } else {
      RcDebugPrint(SDBG_DEFAULT, "%d ", PiSetting);

      if (PiSetting < 10) {
        RcDebugPrint(SDBG_DEFAULT, "  ");
      }

      if ((PiSetting < 100) && (PiSetting > 9)) {
        RcDebugPrint(SDBG_DEFAULT, " ");
      }
    }
    for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
      if (IsDimmStrobeNotValidDdr5 (&(*DimmNvList) [Dimm], Strobe)) {
        continue;
      }

      if (((1 << PiSetting) & (*TrainRes) [Ch][Subchannel][Strobe].results[PiSetting / 32]) != 0) {
        RcDebugPrint(SDBG_DEFAULT, "   1");
      } else {
        RcDebugPrint(SDBG_DEFAULT, "   0");
      }
    } // Strobe loop

    RcDebugPrint(SDBG_DEFAULT, "\n");
  } // PiSetting loop
} // PrintSampleArrayBcomClk


/**

  Performs the Montage Data Buffer B0 workaround after BCOM training if applicable.

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number

  @retval N/A

**/
VOID
MontageDdr5DbErrata (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  UINT8             Channel;
  UINT8             MaxChDdr;
  UINT8             Dimm;
  UINT8             Rank = 0;
  struct dimmNvram  (*DimmNvList) [MAX_DIMM];

  MaxChDdr = GetMaxChDdr ();

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
        continue;
      }
      DimmNvList = GetDimmNvList (Host, Socket, Channel);
      if (((*DimmNvList)[Dimm].SPDLrbufVen == MFGID_MONTAGE) && ((*DimmNvList)[Dimm].lrbufRid == LRBUF_MONTAGE_DB_RID_B0)) {
        // Apply workaround
        DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, LRDIMM_DB_PAGECC);
        DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, LRDIMM_RWE6, 0x3A);
        DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, LRDIMM_DB_PAGE83);
        DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, LRDIMM_RWEE, 0x03);
        DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, LRDIMM_DB_PAGE83);
        DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, LRDIMM_RWEE, 0x00);
        DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, LRDIMM_DB_PAGECC);
        DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, LRDIMM_RWE6, 0x00);

        // Restore page to 0 after workaround
        DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, LRDIMM_DB_PAGE0);
      }
    }
  }
}

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
  UINT8                      Socket;
  UINT8                      Ch = 0;
  UINT8                      Dimm = 0;
  UINT8                      Rank = 0;
  CHIP_ERLY_CMD_CK_STRUCT    ChipErlyCmdCKStruct;
  UINT8                      MaxChDdr;
  MRC_STATUS                 Status;
  DDR5_LRDIMM_TRAINING_DATA  *TrainingData;

  MaxChDdr = GetMaxChDdr ();
  ZeroMem ((UINT8 *)&ChipErlyCmdCKStruct, sizeof (ChipErlyCmdCKStruct));

  Socket = Host->var.mem.currentSocket;
  SetCurrentTestType (Socket, BcomTrainingTest);
  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "BcomTraining Starts\n");
  Host->var.mem.checkMappedOutRanks = 1;

  if (!IsMemFlowEnabled(LrdimmBcomTraining)) {
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
  if ((Host->nvram.mem.socket[Socket].enabled == 0) ||
      (Host->nvram.mem.socket[Socket].maxDimmPop == 0) ||
      (Host->nvram.mem.socket[Socket].lrDimmPresent == 0)) {
    return SUCCESS;
  }

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_EARLY_BCOM_CLK, Socket));

  //
  // Enable normal CMD timing.
  //
  //BRS: DRAM--2N, MC-2N.  SPR: DRAM--1N, MC--1N. DDR5_TODO.
  // DB timing Mode by default is 2N. For BRS, just leave it as default value:2N.
  //SPR: Set it to DB CMD 1N.For BRS, set the DB CMD to 2N, by default, it's 2N, so, no need to program it. Just keep the default value.
  //
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
  // Execute RCD Loopback for RDIMM, LRDIMM, NVM Dimm configs
  //
  SetCurrentTestType (Socket, BcomTrainingTest);

  //
  // Enter sense amp training Mode to all the channels here to latch results.
  //
  SetTrainingModeLatchResult (Host, Socket, CHECKPOINT_MINOR_SENSE_AMP);
  //
  // Start BCOM Training
  //
  VrefBcomTraining (Host);
  //
  // Disable CADB always on Mode
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    DisableCADBAlwaysOnMode (Socket, Ch);
  }
  //
  // Exit sense amp training Mode
  //
  SetTrainingModeLatchResult (Host, Socket, CHECKPOINT_MINOR_DISABLE_SENSE_AMP);
  Host->var.mem.checkMappedOutRanks = 0;
  SetCurrentTestType (Socket, 0);

  // Indicate BCOM training is complete
  Status = GetDdr5LrdimmTrainingData (&TrainingData);
  if (Status == MRC_STATUS_SUCCESS) {
    TrainingData->BcomTrainingDone = TRUE;
  }

  // DB settings only need to be set per DIMM. Leave rank set to 0.
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }
      if (IsLrdimmPresent (Socket, Ch, Dimm) == FALSE) {
        continue;
      }

      //
      // Init DB interface.
      //
      InitDbInterface (Host, Socket, Ch, Dimm, 3, Rank);//DDR5_TODO. Update part function for CTE only.
    } // Ch loop
  }

  // Apply Montage data buffer workaround ater BCOM training if necessary
  MontageDdr5DbErrata (Host, Socket);
  return EFI_SUCCESS;
} // BcomTraining

/**

  Set BCOM[2:0] according to mode.

  @param[in] Socket         - Socket number
  @param[in] ChannelBitMask - Bitmask of channels to reset
  @param[in] Dimm           - Dimm number
  @param[in] Rank           - Rank number
  @param[in] BcomMode       - Bcom mode
                              - BcomResetMode
                              - Other modes

  @retval  MRC_STATUS_SUCCESS   Set BCOM mode successfully
  @retval  MRC_STATUS_FAILURE   Invalid input parameter

**/
MRC_STATUS
EFIAPI
SetBcomMode (
  IN UINT8     Socket,
  IN UINT32    ChannelBitMask,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN BCOM_MODE BcomMode
  )
{
  PSYSHOST                      Host;
  UINT8                         Channel;
  UINT8                         MaxChDdr;
  MRC_STATUS                    Status;
  DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_STRUCT  DbInterfaceTrainingModes;

  Host      = GetSysHostPointer ();
  MaxChDdr  = GetMaxChDdr ();
  Status    = MRC_STATUS_FAILURE;
  //
  //  Set static values for BCOM[2:0] according to the input BCOM mode.
  //
  switch (BcomMode) {
    case BcomResetMode:
      //
      // Set BCOM[2:0] as 111 to make reset valid when BRST_n is low (assert).
      //
      DbInterfaceTrainingModes.Data = 0;
      DbInterfaceTrainingModes.Bits.static_control_value_for_bcom2 = 1;
      DbInterfaceTrainingModes.Bits.static_control_value_for_bcom1 = 1;
      DbInterfaceTrainingModes.Bits.static_control_value_for_bcom0 = 1;
      DbInterfaceTrainingModes.Bits.static_control_value_for_bcs_n = 1; // Set BCS_n HIGH
      DbInterfaceTrainingModes.Bits.static_bcom_control_enable = 1;
      Status = MRC_STATUS_SUCCESS;
      break;
    case BcomNormalMode:
      DbInterfaceTrainingModes.Data = 0;
      Status = MRC_STATUS_SUCCESS;
      break;
    default:
      break;
  }

  if (Status == MRC_STATUS_FAILURE) {
    RcDebugPrint (SDBG_ERROR, "Error: invalid input parameter BcomMode!\n");
    return Status;
  }

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }
    if ((ChannelBitMask & (1 << Channel)) == 0) {
      continue;
    }
    RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, DbInterfaceTrainingModes.Data);
  }
  return Status;
}

/**

  Toggles BRST_A/BRST_B pins to de-assert BRST_n

  @param[in] Host           - Pointer to SysHost
  @param[in] Socket         - Socket number
  @param[in] Channel        - Channel number
  @param[in] SubChannelMask - Sub Channel bit-mask
  @param[in] Dimm           - Dimm number
  @param[in] Rank           - Rank number

  @retval  N/A

**/
VOID
ToggleBReset (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Channel,
  UINT8     SubChannelMask,
  UINT8     Dimm,
  UINT8     Rank
  )
{
  DDR5_RCD_COMMAND_SPACE_STRUCT CommandSpace;

  CommandSpace.Data = 0;

  //
  // Toggle SubChannel A BRST if necessary
  //
  if ((SubChannelMask == SUB_CH_A_MASK) || (SubChannelMask == TWO_SUBCH_MASK)) {
    //
    // Toggle BRST_A active (low).
    //
    CommandSpace.Bits.command = RCD_COMMAND_CH_A_DB_RESET;
    RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_COMMAND_SPACE_REG, CommandSpace.Data);
    FixedDelayMicroSecond (2);
    //
    // Toggle BRST_A inactive (high).
    //
    CommandSpace.Bits.command = RCD_COMMAND_CLEAR_CH_A_DB_RESET;
    RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_COMMAND_SPACE_REG, CommandSpace.Data);
    FixedDelayMicroSecond (2);
  }

  //
  // Toggle SubChannel B BRST if necessary
  //
  if ((SubChannelMask == SUB_CH_B_MASK) || (SubChannelMask == TWO_SUBCH_MASK)) {
    //
    // Toggle BRST_B active (low).
    //
    CommandSpace.Bits.command = RCD_COMMAND_CH_B_DB_RESET;
    RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_COMMAND_SPACE_REG, CommandSpace.Data);
    FixedDelayMicroSecond (2);
    //
    // Toggle BRST_B inactive (high).
    //
    CommandSpace.Bits.command = RCD_COMMAND_CLEAR_CH_B_DB_RESET;
    RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_COMMAND_SPACE_REG, CommandSpace.Data);
    FixedDelayMicroSecond (2);
  }
}

/**

  Programs the BCOM VREF according to the passed in value

  BCOM VREF value is a percentange of Vdd. Note that the
  default value is 75%.

  @param[in] Host           - Pointer to SysHost
  @param[in] Socket         - Socket number
  @param[in] Channel        - Channel number
  @param[in] SubChannelMask - Sub Channel bit-mask
  @param[in] Dimm           - Dimm number
  @param[in] Rank           - Rank number
  @param[in] BcomVrefValue  - VREF value (see header for description)

  @retval  N/A

**/
VOID
ProgramBcomVref (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    SubChannelMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    BcomVrefValue
  )
{
  DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_STRUCT DbInterfaceTrainingModes;
  INT8 Index = 0;

  //
  // Program default value VREF value for BCOM first
  //

  DbInterfaceTrainingModes.Data = 0;
  DbInterfaceTrainingModes.Bits.static_control_value_for_bcom2 = 1;
  DbInterfaceTrainingModes.Bits.static_control_value_for_bcom1 = 0;
  DbInterfaceTrainingModes.Bits.static_control_value_for_bcom0 = 0;
  DbInterfaceTrainingModes.Bits.static_bcom_control_enable = 1;

  RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, DbInterfaceTrainingModes.Data);
  ToggleBReset (Host, Socket, Channel, SubChannelMask, Dimm, Rank);

  if (BcomVrefValue == BCOM_VREF_DEFAULT_VALUE) {

    //
    // If the requested value is the default, we're done.
    //

    return;

  } else if (BcomVrefValue - BCOM_VREF_DEFAULT_VALUE < 0) {

    //
    // Handle the case of less than 75% Vdd. We decrement by 1% each time
    // we call SendMrw, so repeat SendMrw's until we hit the requested %.
    //

    DbInterfaceTrainingModes.Bits.static_control_value_for_bcom1 = 1; // decrement BCOM VREF 1% from default

    for (Index = BCOM_VREF_DEFAULT_VALUE; Index > BcomVrefValue; Index--) {

      RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, DbInterfaceTrainingModes.Data);
      ToggleBReset (Host, Socket, Channel, SubChannelMask, Dimm, Rank);

    }

  } else {

    //
    // Handle the case of greater than 75% Vdd. We increment by 1% each time
    // we call SendMrw, so repeat SendMrw's until we hit the requested %.
    //

    DbInterfaceTrainingModes.Bits.static_control_value_for_bcom0 = 1; // increment BCOM VREF 1% from default

    for (Index = BCOM_VREF_DEFAULT_VALUE; Index < BcomVrefValue; Index++) {

      RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, DbInterfaceTrainingModes.Data);
      ToggleBReset (Host, Socket, Channel, SubChannelMask, Dimm, Rank);

    }

  }

  return;

} // ProgramBcomVref

/**

  Programs BCK delay to the specified delay by walking the value in 1/64tCK steps.

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket number
  @param[in] Channel     - Channel number
  @param[in] Dimm        - DIMM number
  @param[in] SubChMask   - SubChannel mask of subchannels to be programmed
  @param[in] InputDelay  - Delay value to be programmed

  @retval N/A

**/
VOID
ProgramBCKDelay (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT8     SubChMask,
  IN INT16     InputDelay
  )
{
  UINT8   SubChannel;
  UINT8   RegDelay;
  INT8    AdjustValue = 1;
  UINT32  SmbData = 0;

  for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
    if (((1 << SubChannel) & SubChMask) == 0) {
      continue;
    }

    if (IsDcpmmPresentDimm (Socket, Channel, Dimm)) {

      //
      // Program DDRT2 Bclk Delay
      //
      ProgramDdrt2BclkDelay (Host, Socket, Channel, SubChannel, Dimm, InputDelay);

    } else {
      ReadRcdCwCache (Socket, Channel, SubChannel, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_BCK_OUTPUT_DELAY_REG, &RegDelay);
      // Mask off bit 7 to retrieve the delay value
      RegDelay &= ~(BIT7);

      if (InputDelay < RegDelay) {
        AdjustValue = -1;
      }

      while (RegDelay != InputDelay) {
        RegDelay += AdjustValue;
        // Set bit 7 to enable output delay feature for BCK
        SmbData = RegDelay | BIT7;
        WriteRcdSmbDdr5 (Host, Socket, Channel, Dimm, (1 << SubChannel), BYTE_WRITE, RDIMM_CW_PAGE0, DDR5_RCD_BCK_OUTPUT_DELAY_REG, &SmbData);
        // When feature is enabled the delay settings in RW1E[5:0] require a time of tODU for the delay to become stable on the outputs
        FixedDelayMicroSecond (1);
      }

      // Update cache with new value
      WriteRcdCwCache (Socket, Channel, (1 << SubChannel), Dimm, RDIMM_CW_PAGE0, DDR5_RCD_BCK_OUTPUT_DELAY_REG, (UINT8) SmbData);
    }
  }
}

/**

  Exectues the BCOM training.

  @param[in] Host - Pointer to sysHost

  @retval Status

**/
UINT32
VrefBcomTraining (
  IN PSYSHOST  Host
  )
{

  UINT8                      Ch = 0;
  struct ClkTrainingResults  TrainRes[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT8                      Socket;
  UINT8                      Dimm;
  BOOLEAN                    DimmPresent;
  UINT8                      Rank;
  UINT8                      Signal;
  UINT8                      Index = 0;
  INT8                       BcomVrefValue;
  UINT8                      Subchannel = 0;
  CHANNEL_NVRAM_STRUCT       (*ChannelNvList)[MAX_CH];
  struct dimmNvram           (*DimmNvList) [MAX_DIMM];
  GSM_CSN                    SignalToSweepSubCh[MAX_CH][SUB_CH];
  GSM_CSEDGE_CTL             (*SignalEdgeData) [MAX_CH][MAX_DIMM][SUB_CH][BACKSIDE_CA_SIGNALS_TO_SWEEP][NUM_BCOM_VREF_VALUES];
  UINT8                      SubChannelMask;
  UINT8                      MaxEWBcomVref = 0;
  UINT8                      MaxEwBcomVrefIndex = 0;
  UINT16                     MaxSumMinEyeWidth;
  INT16                      Le[MAX_CH][SUB_CH];
  INT16                      Re[MAX_CH][SUB_CH];
  INT16                      EyeWidthSC[MAX_CH][SUB_CH];
  INT16                      EyeCenterPoint[MAX_CH][SUB_CH];
  UINT8                      MaxChDdr;
  UINT8                      BcomVrefIndex = 0;
  INT16                      AdjustedCenter;
  UINT8                      BckDelay = 0;
  DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_STRUCT                     DbInterfaceTrainingModes;
  DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_STRUCT                                 ParityAndAlert;
  DDR5_RCD_GLOBAL_FEATURES_STRUCT                                             GlobalFeatures;
  DDR5_RCD_DATA_BUFFER_INTERFACE_AND_LOOPBACK_DRIVER_CHARACTERISTICS_STRUCT   DbDriveStrength;

  MaxChDdr = GetMaxChDdr ();
  Socket = Host->var.mem.currentSocket;
  ChannelNvList = GetChannelNvList (Host, Socket);

  SignalEdgeData = RcAllocatePool (sizeof (*SignalEdgeData));
  if (SignalEdgeData == NULL) {
    RC_FATAL_ERROR(FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return FAILURE;
  }
  ZeroMem ((UINT8 *) SignalEdgeData, sizeof (*SignalEdgeData));

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "VrefBcomTraining Starts\n");

  Index = 0;

  //
  // Bcom training ddrio init
  //
  BcomDdrioInit (Host, Socket);

  //
  // RCD CW writes always go to rank 0 in this function
  //
  Rank = 0;
  //
  // Test each Signal on every Dimm on each Vref.
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    DimmPresent = TRUE;

    // Program starting value for BCK delay
    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }

      if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
        DimmPresent = FALSE;
        continue;
      }

      ProgramBCKDelay (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, BCOM_BCK_DELAY);
    }

    if (DimmPresent == FALSE) {
      continue;  // DIMM not present, skip training
    }
    //
    // BCOM Vref Loop. We increment both the percentage of Vdd and the index of our
    // Signal edge data storage in this for loop.
    //
    for (BcomVrefValue = BCOM_VREF_INIT_VALUE, BcomVrefIndex = 0; BcomVrefValue <= MAX_BCOM_VREF_VALUE; BcomVrefValue += BCOM_VREF_STEP_SIZE, BcomVrefIndex++) {

      //
      // Program BCOM Vref Value and BCOM Training Mode.
      //
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }
        DimmNvList = GetDimmNvList (Host, Socket, Ch);

        //
        // Forward all commands to DB
        //
        ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, &ParityAndAlert.Data);
#ifdef USE_LATEST_RCD_SPEC
        ParityAndAlert.Bits.data_buffer_interface_forward_all_cmds = 1;
#else // #ifdef USE_LATEST_RCD_SPEC
        ParityAndAlert.Bits.data_buffer_interface_block_all_cmds = 1;
#endif // #ifdef USE_LATEST_RCD_SPEC
        RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, ParityAndAlert.Data);

        if ((*ChannelNvList) [Ch].timingMode == TIMING_1N) {
          //
          // Set 1N BCOM CMD Timing - BCOM[2:0] 011
          //
          DbInterfaceTrainingModes.Data = 0;
          DbInterfaceTrainingModes.Bits.static_control_value_for_bcom2 = 0;
          DbInterfaceTrainingModes.Bits.static_control_value_for_bcom1 = 1;
          DbInterfaceTrainingModes.Bits.static_control_value_for_bcom0 = 1;
          DbInterfaceTrainingModes.Bits.static_bcom_control_enable = 1;
          RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, DbInterfaceTrainingModes.Data);
          ToggleBReset (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, 0);
        }

        //
        // Enter BCOMTM - BCOM[2:0] 001
        //
        DbInterfaceTrainingModes.Data = 0;
        DbInterfaceTrainingModes.Bits.static_control_value_for_bcom2 = 0;
        DbInterfaceTrainingModes.Bits.static_control_value_for_bcom1 = 0;
        DbInterfaceTrainingModes.Bits.static_control_value_for_bcom0 = 1;
        DbInterfaceTrainingModes.Bits.static_bcom_control_enable = 1;
        RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, DbInterfaceTrainingModes.Data);
        ToggleBReset (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, 0);

        ProgramBcomVref (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, 0, BcomVrefValue);

        //
        // Exit RCD Static BCOM Mode
        //
        ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, &DbInterfaceTrainingModes.Data);
        DbInterfaceTrainingModes.Bits.static_bcom_control_enable = 0;
        RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, DbInterfaceTrainingModes.Data);

        //
        // Block commands to DRAM
        //
        ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, &ParityAndAlert.Data);
#ifdef USE_LATEST_RCD_SPEC
        ParityAndAlert.Bits.dram_interface_forward_all_cmds = 0;
#else // #ifdef USE_LATEST_RCD_SPEC
        ParityAndAlert.Bits.dram_interface_block_all_cmds = 0;
#endif // #ifdef USE_LATEST_RCD_SPEC
        RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, ParityAndAlert.Data);

        //
        // BCOM Drive stength
        //
        ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_DATA_BUFFER_INTERFACE_AND_LOOPBACK_DRIVER_CHARACTERISTICS_REG, &DbDriveStrength.Data);
#ifdef USE_LATEST_RCD_SPEC
        DbDriveStrength.Bits.bck_t_bck_c_driver_strength = 1;
        DbDriveStrength.Bits.bcom_2_0_bcs_n_driver_strength = 1;
        RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_DATA_BUFFER_INTERFACE_AND_LOOPBACK_DRIVER_CHARACTERISTICS_REG, DbDriveStrength.Data);
#endif // #ifdef USE_LATEST_RCD_SPEC

        //
        // Enter BCOM pass through Mode, set SDR2 (for non CTE)
        //
        ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_GLOBAL_FEATURES_REG, &GlobalFeatures.Data);
        if (FeaturePcdGet (PcdCteBuild) != TRUE) {
          GlobalFeatures.Bits.sdr_modes_1 = 1;
        }
        GlobalFeatures.Bits.bcom_pass_through_mode = 1;
        RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_GLOBAL_FEATURES_REG, GlobalFeatures.Data);

        for (Subchannel = 0; Subchannel < SUB_CH; Subchannel++) {
          //
          // Check Subchannel is enabled or not.
          //
          SubChannelMask = 0;
          SubChannelMask = SubChannelMask | (1 << Subchannel);

          //
          // Parity checking must be disabled prior to and during pass-through Mode. It's disabled by default. So, need to make sure it's disabled after jedec reset.
          // Probably, the below code can be commented out. DDR5_TODO. to confirm?
          //
          // DDR5_TODO. Change it to use MRW command. It can use MRW command.
          //
          //
          // As the parity is disabled by default. No need to disable it separately.
          //
        } //sub channel loop
      } //Ch loop.
      for (Signal = 0; Signal < BCOM_SIGNALS_TO_SWEEP; Signal++) {

        for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
          //
          // initialize to no Signal
          //
          SignalToSweepSubCh[Ch][0] = SignalsToSweepBcomDDR5[0];
          SignalToSweepSubCh[Ch][1] = SignalsToSweepBcomDDR5[1];
        }//Ch loop

        //
        // Get CMD margins for current Signal
        //
        //
        // Init TrainRes.Clear the train result or it will impact the next loop test.
        //
        ZeroMem ((UINT8 *) TrainRes, sizeof (TrainRes));

        GetBcomClkMarginsSweepVref (Host, Socket, Dimm, 0, SignalToSweepSubCh, BcomVrefValue, Le, Re, EyeWidthSC, EyeCenterPoint, &TrainRes);
        for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
          if (IsChannelEnabled (Socket, Ch) == FALSE) {
            continue;
          }

          for (Subchannel = 0; Subchannel < SUB_CH; Subchannel++) {
            if (SignalToSweepSubCh[Ch][Subchannel] == NO_SIGNAL) {
              continue;
            }

            //
            // Save the results.
            //
            (*SignalEdgeData) [Ch][Dimm][Subchannel][Signal][BcomVrefIndex].signal = SignalToSweepSubCh[Ch][Subchannel];
            (*SignalEdgeData) [Ch][Dimm][Subchannel][Signal][BcomVrefIndex].le = Le[Ch][Subchannel];
            (*SignalEdgeData) [Ch][Dimm][Subchannel][Signal][BcomVrefIndex].re = Re[Ch][Subchannel];
            (*SignalEdgeData) [Ch][Dimm][Subchannel][Signal][BcomVrefIndex].EyeWidth = EyeWidthSC[Ch][Subchannel];
            (*SignalEdgeData) [Ch][Dimm][Subchannel][Signal][BcomVrefIndex].CenterPoint = EyeCenterPoint[Ch][Subchannel];
            (*SignalEdgeData) [Ch][Dimm][Subchannel][Signal][BcomVrefIndex].toSample = 1;
            (*SignalEdgeData) [Ch][Dimm][Subchannel][Signal][BcomVrefIndex].EyeOffset = ABS (BACKSIDE_BCOM_FULL_EYE_WIDTH - EyeWidthSC[Ch][Subchannel]);
          } //Subchannel Loop
        } // Ch loop
      }//Signal loop
    } // BcomVrefValue loop
    //Restore Normal Operating Mode after sweeping BCOM for Dimm.
    if (FeaturePcdGet (PcdCteBuild) == TRUE) {
      JedecInitSequence (Host, Socket, CH_BITMASK);
    }

  }// Dimm loop

  RcDebugPrint (SDBG_MAX,
                  "\n");
  //
  // Determine the best BCOM vref.
  //
  RcDebugPrint (SDBG_MAX,
                  "Summary: \n");
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) continue;
    for (Subchannel = 0; Subchannel < SUB_CH; Subchannel++) {
      //DDR5_TODO--may need to check whether sub channel is enabled or not.
      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList) [Dimm].dimmPresent == 0) {
          continue;
        }

        //
        // Calculate the maximum composite eye width.
        //
        RcDebugPrint (SDBG_MAX,
                        "\n");
        RcDebugPrintWithDevice ( SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "Vref Calculation Summary:\n");
        MaxSumMinEyeWidth = (*SignalEdgeData) [Ch][Dimm][Subchannel][0][0].EyeWidth;
        MaxEWBcomVref = BCOM_VREF_INIT_VALUE;

        for (BcomVrefValue = BCOM_VREF_INIT_VALUE, BcomVrefIndex = 0; BcomVrefValue <= MAX_BCOM_VREF_VALUE; BcomVrefValue += BCOM_VREF_STEP_SIZE, BcomVrefIndex++) {
          if ((*SignalEdgeData) [Ch][Dimm][Subchannel][0][BcomVrefIndex].toSample == 0) {
            continue;
          }

          RcDebugPrintWithDevice ( SDBG_MAX, Socket, Ch, Dimm, Subchannel, NO_RANK, NO_STROBE, NO_BIT,
                              "BCOM Vref Value %3d,  left %3d, right %3d, Eye Width %3d\n",
                              BcomVrefValue,
                              (*SignalEdgeData) [Ch][Dimm][Subchannel][0][BcomVrefIndex].le,
                              (*SignalEdgeData) [Ch][Dimm][Subchannel][0][BcomVrefIndex].re,
                              (*SignalEdgeData) [Ch][Dimm][Subchannel][0][BcomVrefIndex].EyeWidth);

          if (MaxSumMinEyeWidth < (*SignalEdgeData) [Ch][Dimm][Subchannel][0][BcomVrefIndex].EyeWidth) {
            MaxSumMinEyeWidth = (*SignalEdgeData) [Ch][Dimm][Subchannel][0][BcomVrefIndex].EyeWidth;
            MaxEWBcomVref = BcomVrefValue;
            MaxEwBcomVrefIndex = BcomVrefIndex; // Index into SignalEdgeData
          }
        } //BcomVrefValue loop
        RcDebugPrint (SDBG_MAX,
                        "\n");
        RcDebugPrintWithDevice ( SDBG_MAX, Socket, Ch, Dimm, Subchannel, NO_RANK, NO_STROBE, NO_BIT,
                            "Final Vref Setting: BCOM Vref Value %3d, Max eye width  %3d, Eye Centre %3d\n",
                            MaxEWBcomVref,
                            MaxSumMinEyeWidth,
                            (*SignalEdgeData) [Ch][Dimm][Subchannel][0][MaxEwBcomVrefIndex].CenterPoint);
        //
        // Program the final BCOM delay
        //  - Subtract one cycle (64) from center
        AdjustedCenter = (*SignalEdgeData) [Ch][Dimm][Subchannel][0][MaxEwBcomVrefIndex].CenterPoint - BCOM_CLOCK_CYCLE;

        //  - If new center < 0, set BCOM/BCS delay to 0 and increase BCK: BCK += |new center|
        if (AdjustedCenter < 0) {
          ProgramBcomDelay (Host, Socket, Ch, Dimm, (1 << Subchannel), 0, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, 0, TRUE);
          ReadRcdCwCache (Socket, Ch, Subchannel, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_BCK_OUTPUT_DELAY_REG, &BckDelay);
          // Mask off bit 7 to retrieve the delay value
          BckDelay &= ~(BIT7);
          AdjustedCenter = ABS (AdjustedCenter) + BckDelay;
          ProgramBCKDelay (Host, Socket, Ch, Dimm, (1 << Subchannel), AdjustedCenter);
        } else {
          // Set BCOM/BCS delay to the new center, no need to change BCK
          ProgramBcomDelay (Host, Socket, Ch, Dimm, (1 << Subchannel), 0, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, AdjustedCenter, TRUE);
        }

        //
        // If only sweep one vref, no need to program the BCOM vref again.
        //
        if (MAX_BCOM_VREF_VALUE != BCOM_VREF_INIT_VALUE + 1) {
          //
          // Progrom the final BCOM VREF.
          //
          //
          // SET RCD static values for BVREF other than default; Assign the static value of '100' or '101' or '110'
          //

          RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "BCOM BcomVrefValue %d \n", MaxEWBcomVref);

          ProgramBcomVref (Host, Socket, Ch, (1 << Subchannel), Dimm, 0, MaxEWBcomVref);

          RcDebugPrint (SDBG_MAX,
                        "\n");
        }//if

      } //Dimm loop
    } //sub channel loop
  } //Ch loop
  RcDebugPrint (SDBG_MAX,
                  "\n");
  //
  // Exit BCOM pass through mode.
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList) [Dimm].dimmPresent == 0) {
        continue;
      }
      //
      // Exit BCOMTM -- BCOM[2:0] 010
      //
      DbInterfaceTrainingModes.Data = 0;
      DbInterfaceTrainingModes.Bits.static_control_value_for_bcom2 = 0;
      DbInterfaceTrainingModes.Bits.static_control_value_for_bcom1 = 1;
      DbInterfaceTrainingModes.Bits.static_control_value_for_bcom0 = 0;
      DbInterfaceTrainingModes.Bits.static_bcom_control_enable = 1;
      RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, DbInterfaceTrainingModes.Data);
      ToggleBReset (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, 0);

      //
      // Exit RCD Static BCOM Mode
      //
      ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, &DbInterfaceTrainingModes.Data);
      DbInterfaceTrainingModes.Bits.static_bcom_control_enable = 0;
      RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, DbInterfaceTrainingModes.Data);

      //
      // Disable BCOM pass thru mode, restore SDR mode
      //
      ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_GLOBAL_FEATURES_REG, &GlobalFeatures.Data);
      GlobalFeatures.Bits.bcom_pass_through_mode = 0;
      GlobalFeatures.Bits.sdr_modes_1 = 0;
      RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_GLOBAL_FEATURES_REG, GlobalFeatures.Data);

      //
      // Forward all commands to DRAM/DB
      //
      ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, &ParityAndAlert.Data);
#ifdef USE_LATEST_RCD_SPEC
      ParityAndAlert.Bits.dram_interface_forward_all_cmds = 1;
      ParityAndAlert.Bits.data_buffer_interface_forward_all_cmds = 1;
#else // #ifdef USE_LATEST_RCD_SPEC
      ParityAndAlert.Bits.dram_interface_block_all_cmds = 1;
      ParityAndAlert.Bits.data_buffer_interface_block_all_cmds = 1;
#endif // #ifdef USE_LATEST_RCD_SPEC
      RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, ParityAndAlert.Data);

      //
      // Disable training modes
      //
      DbInterfaceTrainingModes.Data = 0;
      RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, DbInterfaceTrainingModes.Data);

      //
      // Issue a ZQCal (DDRT2 Only)
      // A ZQCal is needed to set the ODT and Drive strength on MDQS and MDQS interfaces.
      //
      if (IsDcpmmPresentDimm (Socket, Ch, Dimm) == TRUE) {
        WriteMPC (Host, Socket, Ch, Dimm, SUB_CH_A_MASK, 0, ZQCAL_START, TIMING_1N, FALSE);
        FixedDelayMicroSecond (2); // delay at least tZQCAL (== 1 us), fixeddelay is roughly 1 microsec
        WriteMPC (Host, Socket, Ch, Dimm, SUB_CH_A_MASK, 0, ZQCAL_LATCH, TIMING_1N, FALSE);
        FixedDelayMicroSecond (1); // delay at least tZQLAT (< 1 us)
      }
    }
  }

  // DDRIO programming post training
  PostBcomDdrioProgramming (Host, Socket);

  RcFreePool (SignalEdgeData);
  return SUCCESS;
} //VrefBcomTraining

//------------------------------------------------------------------------------------------


/**

Finds CA CLK margins for the given Signal by the CA Vref setting.

@param[in]      Host        - Pointer to sysHost
@param[in]      Socket      - Socket number
@param[in]      Dimm        - Dimm number
@param[in]      Rank        - Rank number
@param[in]      SignalSubCh - Sub channel signal
@param[in]      Vref        - Voltage reference
@param[in]      CsLeft      - Left edge
@param[in]      CsRight     - Right edge
@param[in]      EyeWidth    - Eye width
@param[in]      CsValue     - Cs value
@param[in][out] *TrainRes   - Training result

@retval N/A

**/
VOID
GetBcomClkMarginsSweepVref (
  IN PSYSHOST                       Host,
  IN UINT8                          Socket,
  IN UINT8                          Dimm,
  IN UINT8                          Rank,
  IN GSM_CSN                        SignalSubCh[MAX_CH][SUB_CH],
  IN INT8                           Vref,
  IN INT16                          CsLeft[MAX_CH][SUB_CH],
  IN INT16                          CsRight[MAX_CH][SUB_CH],
  IN INT16                          EyeWidth[MAX_CH][SUB_CH],
  OUT INT16                         CsValue[MAX_CH][SUB_CH],
  IN OUT struct ClkTrainingResults  (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8                        Ch;
  INT16                        PiIndex;
  UINT8                        Offset;
  UINT32                       RankMask[MAX_CH];
  UINT8                        ErrorResult[MAX_CH][CA_CLK_TEST_RANGE_BACKSIDE];
  UINT32                       ChBitmask = 0;
  UINT32                       BwSerr[MAX_CH][3];
  UINT8                        Subchannel = 0;
  UINT8                        Strobe;
  UINT8                        DWord;
  UINT8                        SubChMSVx8;
  UINT32                       SmbData = 0;
  UINT32                       ValidMask[MAX_STROBE_DDR5];
  INT16                        RisingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16                        FallingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT32                       DqSampleResultStrobeSubCh[SUB_CH][MAX_STROBE_DDR5 / 2];
  struct dimmNvram             (*DimmNvList) [MAX_DIMM];
  UINT8                        Step;
  UINT32                       DqSampleResultStrobe[MAX_STROBE_DDR5 / 2];
  CHIP_GET_CMD_MGN_SWP_STRUCT  ChipGetCmdMgnSwpStruct;
  MRC_MST                      MemSsType;
  MRC_RT                       MrcRankList;
  UINT32                       OnechBitmask = 0;
  UINT8                        MaxStrobe = 0;
  INT16                        PulseWidth[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16                        LeftEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16                        RightEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT32                       DqCentering[MAX_CH];
  UINT16                       MinimumLeftEdge;
  INT16                        Tempvar;
  UINT8                        MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  SubChMSVx8 = GetSubChMaxStrobeValid (Host) / 2;
  //
  // Init DqCentering array and DqSampleResult array.
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    DqCentering[Ch] = 0;
  }
  //
  // Init CsLeft,  CsRight, CsValue
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    for (Subchannel = 0; Subchannel < SUB_CH; Subchannel++) {
      CsLeft[Ch][Subchannel] = -UNMARGINED_CMD_EDGE;
      CsRight[Ch][Subchannel] = UNMARGINED_CMD_EDGE;
      CsValue[Ch][Subchannel] = 0;
      EyeWidth[Ch][Subchannel] = 0;
    }
  }
  for (Subchannel = 0; Subchannel < SUB_CH; Subchannel++) {
    for (Strobe = 0; Strobe < MAX_STROBE_DDR5/2; Strobe++) {
      DqSampleResultStrobeSubCh[Subchannel][Strobe] = 0;
    }
  }
  for (Strobe = 0; Strobe < MAX_STROBE_DDR5/2; Strobe++) {
    DqSampleResultStrobe[Strobe] = 0;
  }

  for (Subchannel = 0; Subchannel < SUB_CH; Subchannel++) {
    for (Strobe = 0; Strobe < MAX_STROBE_DDR5 / 2; Strobe++) {
      DqSampleResultStrobeSubCh[Subchannel][Strobe] = 0;
    }
  }
  GetCmdMarginsSweepInit (&ChipGetCmdMgnSwpStruct);

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    for (Subchannel = 0; Subchannel < SUB_CH; Subchannel++) {
      RankMask[Ch] = 0;
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

    // Initialize Ch Rank mask
    RankMask[Ch] = 1;

    for (PiIndex = 0; PiIndex < CA_CLK_TEST_RANGE_BACKSIDE; PiIndex++) {
      ErrorResult[Ch][PiIndex] = 1;
    } //PiIndex loop
    //
    // Setup for BCOM CLK test
    //
    //
    // Indicate Rank present and enabled on this Ch.
    //
    OnechBitmask = 0;
    OnechBitmask |= 1 << Ch;
    MrcRankList.Dimm = Dimm;
    MrcRankList.Rank = Rank;
    for (Subchannel = 0; Subchannel < SUB_CH; Subchannel++) {
      if (Host->DdrioUltSupport) {
        CteSetupPattern ("early_bcom_loopback", (SignalSubCh[Ch][Subchannel] >= A0B) ? SignalSubCh[Ch][Subchannel] - A0B : SignalSubCh[Ch][Subchannel] - A0A, ChBitmask, (SignalSubCh[Ch][Subchannel] >= A0B) ? 0x2 : 0x1, GetPhyRank (Dimm, Rank));
      } else {
        SetupTest (Host, Socket, MemSsType, OnechBitmask, 1, &MrcRankList, DdrLevel, BcomTrainingTest, SignalSubCh[Ch][Subchannel], 0, FIRST_ITERATION_LOOP);
      }
    }
    ChipGetCmdMgnSwpStruct.eyeSize[Ch] = 0;
    ChipGetCmdMgnSwpStruct.chEyemask |= 1 << Ch;
    ChipGetCmdMgnSwpStruct.chDonemask |= 1 << Ch;

    // Indicate Rank present and enabled on this Ch
    ChBitmask |= 1 << Ch;
  } // Ch loop

  if (FeaturePcdGet (PcdCteBuild) != TRUE) {
    for (Subchannel = 0; Subchannel < SUB_CH; Subchannel++) {
      TestControl(Host, Socket, MemSsType, ChBitmask, Subchannel, BcomTrainingTest, 1, START_TEST, NOT_STOP_TEST);//BRS_TODO. To use sub channel mask.
    }
  }
  //
  // Set the DQ count window
  //
  SetDataTrainCountWindow (Host, Socket, Dimm, Rank, DQ_COUNTING_WINDOW_VALUE);
  SetDqCountingWindow (Host, Socket, Dimm, Rank);

  Offset = BCOM_CLK_STEP_SIZE;
  //
  // Sweep Pi delays
  //
  for (PiIndex = BCOM_CLK_TEST_START; PiIndex < (BCOM_CLK_TEST_RANGE + BCOM_CLK_TEST_START); PiIndex += Offset) {

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
      //
      //Set the pi first, and then kick off the CADBpattern, get training result. disable the pattern. program the pi.
      //
        ProgramBcomDelay (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, Rank, GSM_FORCE_WRITE , PiIndex, TRUE);
    } // Ch loop
    if (FeaturePcdGet (PcdCteBuild) == TRUE) {

      //
      // Setup test first
      //
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if ((RankMask[Ch] == 0)) {
          continue;
        }
        OnechBitmask = 0;
        OnechBitmask |= 1 << Ch;
        //
        // CADB, 2nd setup, will start in even or odd clk. Review with Nate.
        //
        for (Subchannel = 0; Subchannel < SUB_CH; Subchannel++) {

          SetupTest (Host, Socket, MemSsType, OnechBitmask, 1, &MrcRankList, DdrLevel, BcomTrainingTest, SignalSubCh[Ch][Subchannel], 0, FIRST_ITERATION_LOOP);
        }
      } //Ch loop
      for (Subchannel = 0; Subchannel < SUB_CH; Subchannel++) {
        //
        // start
        //
        TestControl (Host, Socket, MemSsType, ChBitmask, Subchannel, BcomTrainingTest, 1, START_TEST, NOT_STOP_TEST);
      }

    }
    //
    // Add some delay between the BCOM pattern start and setting the cacsbacksidetrainen bit.
    //
      FixedDelayMicroSecond (10);

    //
    // Generate an IO Reset. This is required because the pi delays changed.
    // The MC reset is needed to enlarge the count window assert pulse to make sure the count winodw value can be reset to 0.
    //
    IO_Reset(Host, Socket);
    //
    // Enabling the counter Mode.
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
      for (Subchannel = 0; Subchannel < SUB_CH; Subchannel++) {
        for (Strobe = 0; Strobe < MAX_STROBE_DDR5 / 2; Strobe++) {
          if (IsDimmStrobeNotValidDdr5 (&(*DimmNvList) [Dimm], Strobe)) {
            continue;
          }
          if ((DqSampleResultStrobeSubCh[Subchannel][Strobe] & 0x1FF) >= MAX_BCOM_FEEDBACK_OUTPUT) {
            for (Step = 0; Step < BCOM_CLK_STEP_SIZE; Step++) {
              (*TrainRes) [Ch][Subchannel][Strobe].results[DWord] = (*TrainRes) [Ch][Subchannel][Strobe].results[DWord] | (1 << ((PiIndex + Step) % 32));
            } // Step loop
          } //if
        } //Strobe loop
      }//Subchannel loop
    } //Ch loop
    //BRS_CTE_WA_START
    //
    // Stop Test to allow sending mrw to change the pi in cte, for BCOM.
    //
    if (FeaturePcdGet (PcdCteBuild) == TRUE) {
      for (Subchannel = 0; Subchannel < SUB_CH; Subchannel++) {
        TestControl (Host, Socket, MemSsType, ChBitmask, Subchannel, BcomTrainingTest, 1, NOT_START_TEST, STOP_TEST);
      }
    }

    //BRS_CTE_WA_END
  } // PiIndex loop
  //
  // Stop Test
  //
  for (Subchannel = 0; Subchannel < SUB_CH; Subchannel++) {
    TestControl (Host, Socket, MemSsType, ChBitmask, Subchannel, BcomTrainingTest, 1, NOT_START_TEST, STOP_TEST);
  }
  //
  //BRS_TODO. Update the code to calculate the best BCOM VREF and Timing.
  //
  //
  // Handle the training result.
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    MaxStrobe = SubChMSVx8;
    //
    // Continue to the next Rank if this one is disabled
    //
    if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
      continue;
    }
    //
    // Stop the test --disable RCD QCS Training Mode.
    //

    SmbData = 0;
    WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, SUB_CH_A_MASK, BYTE_WRITE, RDIMM_CW_PAGE0, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, &SmbData);

    for (Subchannel = 0; Subchannel < SUB_CH; Subchannel++) {

      if (GetDebugLevel () & SDBG_MAX) {
        AcquirePrintControl ();
        RcDebugPrint (SDBG_DEFAULT, "\nSummary: Bcom Clk training Pi\nS%d, Ch%d, Dimm%d, SC%d, Rank%d, %a Vref %d\n",
          Socket, Ch, Dimm, Subchannel, Rank, GetSignalStrDdr5 (SignalSubCh[Ch][Subchannel]), Vref);
        PrintSampleArrayBcomClk (Host, Socket, Ch, Dimm, 0, Subchannel, BCOM_CLK_TEST_RANGE, TrainRes);
        ReleasePrintControl ();
      }

      for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }

        EvaluatePiSettingResultsCsClk (Host, Socket, Ch, Dimm, Rank, Strobe, RisingEdge, FallingEdge, PI_CS_CLK_TYPE, Subchannel, EarlyCsBacksideLoopbackTest, TrainRes);
        //
        // Calculate left edge and right edge
        //
        LeftEdge[Ch][Subchannel][Strobe] = MIN (FallingEdge[Ch][Subchannel][Strobe], RisingEdge[Ch][Subchannel][Strobe]);
        RightEdge[Ch][Subchannel][Strobe] = MAX (FallingEdge[Ch][Subchannel][Strobe], RisingEdge[Ch][Subchannel][Strobe]);
        PulseWidth[Ch][Subchannel][Strobe] = RightEdge[Ch][Subchannel][Strobe] - LeftEdge[Ch][Subchannel][Strobe];
      } // Strobe loop

      DisplayEdgesDDR5 (Host, Socket, Ch, PulseWidth[Ch], LeftEdge[Ch], RightEdge[Ch], Subchannel, MaxStrobe);

      //
      // Make sure the eye width is large enough--check the eye pass criteria.
      //
      //
      // Calculate the composite eye
      //
      //
      // Find the minumum left edge
      //

      MinimumLeftEdge = LeftEdge[Ch][Subchannel][0];
      for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }
        if (MinimumLeftEdge > LeftEdge[Ch][Subchannel][Strobe]) {
          MinimumLeftEdge = LeftEdge[Ch][Subchannel][Strobe];
        }
      }
      //
      // Determin the right edge needs to be wrapped or not.
      //
      for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }
        if ((LeftEdge[Ch][Subchannel][Strobe] - MinimumLeftEdge) > (MinimumLeftEdge - (RightEdge[Ch][Subchannel][Strobe] - BACKSIDE_BCOM_WRAP_AROUND_VALUE))) {
          Tempvar = RightEdge[Ch][Subchannel][Strobe];
          RightEdge[Ch][Subchannel][Strobe] = LeftEdge[Ch][Subchannel][Strobe];
          LeftEdge[Ch][Subchannel][Strobe] = Tempvar - BACKSIDE_BCOM_WRAP_AROUND_VALUE;
        }
        //
        // Calculate the pulse width.
        //
        PulseWidth[Ch][Subchannel][Strobe] = RightEdge[Ch][Subchannel][Strobe] - LeftEdge[Ch][Subchannel][Strobe];
      }
      //
      // Calculate the composite eye
      //
      for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }
        if (LeftEdge[Ch][Subchannel][Strobe] > CsLeft[Ch][Subchannel]) {
          CsLeft[Ch][Subchannel] = LeftEdge[Ch][Subchannel][Strobe];
        }
        if (CsRight[Ch][Subchannel] > RightEdge[Ch][Subchannel][Strobe]) {
          CsRight[Ch][Subchannel] = RightEdge[Ch][Subchannel][Strobe];
        }
      }

      CsValue[Ch][Subchannel] = ((CsLeft[Ch][Subchannel] + CsRight[Ch][Subchannel]) / 2) % BACKSIDE_BCOM_WRAP_AROUND_VALUE;
      EyeWidth[Ch][Subchannel] = ABS (CsRight[Ch][Subchannel] - CsLeft[Ch][Subchannel]);

      //
      // Display the left edge, right edge and CS PI value.
      //
      RcDebugPrint (SDBG_MAX,
                          "After Wrapping Determination\n");
      DisplayEdgesDDR5 (Host, Socket, Ch, PulseWidth[Ch], LeftEdge[Ch], RightEdge[Ch], Subchannel, MaxStrobe);
      DisplayCompositeEyeDDR5 (Host, Socket, Ch, CsLeft[Ch], CsRight[Ch], EyeWidth[Ch], CsValue[Ch], Subchannel, MaxStrobe);

    }//Subchannel loop
  } // Ch loop
} // GetBcomClkMarginsSweepVref

/**

Program BCOM Delay.

@param[in] Host            - Pointer to sysHost
@param[in] SocketSocket    - Socket number
@param[in] Ch              - Channel number
@param[in] Dimm            - Dimm number
@param[in] SubchMask       - Sub channel mask
@param[in] Rank            - Rank number
@param[in] Mode            - Get set mode
@param[in] Delay           - Delay
@param[in] PassThroughMode - Pass through mode

@retval Status

**/
UINT32
ProgramBcomDelay (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     SubchMask,
  IN UINT8     Rank,
  IN UINT8     Mode,
  IN INT16     Delay,
  IN BOOLEAN   PassThroughMode
  )
{
  UINT32            Status = SUCCESS;
  struct dimmNvram  (*DimmNvList) [MAX_DIMM];
  UINT8             FullCycleDelay = 0;
  UINT32            SmbData = 0;
  UINT8             LocalRank = 0;
  UINT8             SubCh;

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

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & SubchMask) == 0) {
      continue;
    }
    //
    // direct delay value program.
    //
    if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
      if (Delay >= 64) {
          Delay = Delay - 64;
          FullCycleDelay = BIT6;
      }
      //
      // Program BCOM delay
      //
      SmbData = (Delay | FullCycleDelay) | BIT7;
      Status = WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, (1 << SubCh), BYTE_WRITE, RDIMM_CW_PAGE0, DDR5_RCD_BCS_N_AND_BCOM_2_0_OUTPUT_DELAY_REG, &SmbData);
    } else {
      WriteCwvBufferDelay (Host, Socket, Ch, SubchMask, Dimm, (UINT16) Delay);
    }

    if ((Mode & GSM_UPDATE_CACHE) != 0) {
      WriteRcdCwCache (Socket, Ch, 1 << SubCh, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_BCS_N_AND_BCOM_2_0_OUTPUT_DELAY_REG, (UINT8) SmbData);
    }
  } // SubCh
  //
  // When feature is enabled the delay settings in RW1D[6:0] require a time of tODU for the delay to become stable on the outputs
  //
  FixedDelayMicroSecond (1);
  return Status;
}


//
// Below functions are for BSSA RMT.
//

/**
  Callback to be executed before RMT BCOM sweep starts.

  @param[in]      CommonParameters    Pointer to the context information

  @retVal         MRC_STATUS_SUCCESS
**/
MRC_STATUS
RmtPreSweepBcom1D (
  IN  COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  PSYSHOST                   Host;
  UINT8                      Ch = 0;
  UINT8                      Socket;
  UINT8                      Dimm;
  UINT8                      Rank;
  CHANNEL_NVRAM_STRUCT       (*ChannelNvList)[MAX_CH];
  UINT8                      MaxChDdr;
  DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_STRUCT                     DbInterfaceTrainingModes;
  DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_STRUCT                                 ParityAndAlert;
  DDR5_RCD_GLOBAL_FEATURES_STRUCT                                             GlobalFeatures;
  DDR5_RCD_DATA_BUFFER_INTERFACE_AND_LOOPBACK_DRIVER_CHARACTERISTICS_STRUCT   DbDriveStrength;
  CHIP_ERLY_CMD_CK_STRUCT                                                     ChipErlyCmdCKStruct;
  SSA_MARGIN_BCOM                                                             *PrivateBcom;

  PrivateBcom= (SSA_MARGIN_BCOM *) CommonParameters;

  Host = CommonParameters->Host;
  MaxChDdr = GetMaxChDdr ();
  Socket = Host->var.mem.currentSocket;
  ChannelNvList = GetChannelNvList (Host, Socket);


  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (SkipChannelExecution (CommonParameters, Ch)) {
      continue;
    }

    //
    // Turn on command/address output enable
    //
    TrnOnChipCMDADDOpEn (Host, Socket, Ch, &ChipErlyCmdCKStruct);
  } // Ch loop

  IO_Reset (Host, Socket);

  //
  // Execute RCD Loopback for RDIMM, LRDIMM, NVM Dimm configs
  //
  SetCurrentTestType (Socket, PrivateBcom->TestType);

  //
  // Bcom training ddrio init
  //
  BcomDdrioInit (Host, Socket);

  //
  // RCD CW writes always go to rank 0 in this function
  //
  Rank = 0;

  //
  // Program BCOM Vref Value and BCOM Training Mode.
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (SkipChannelExecution (CommonParameters, Ch)) {
      continue;
    }

    //DimmNvList = GetDimmNvList (Host, Socket, Ch);
    Dimm = CommonParameters->Dimm[Ch];

    //
    // Forward all commands to DB
    //
    ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, &ParityAndAlert.Data);
#ifdef USE_LATEST_RCD_SPEC
    ParityAndAlert.Bits.data_buffer_interface_forward_all_cmds = 1;
#else // #ifdef USE_LATEST_RCD_SPEC
    ParityAndAlert.Bits.data_buffer_interface_block_all_cmds = 1;
#endif // #ifdef USE_LATEST_RCD_SPEC
    RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, ParityAndAlert.Data);

    //
    // Enter BCOMTM - BCOM[2:0] 001
    //
    DbInterfaceTrainingModes.Data = 0;
    DbInterfaceTrainingModes.Bits.static_control_value_for_bcom2 = 0;
    DbInterfaceTrainingModes.Bits.static_control_value_for_bcom1 = 0;
    DbInterfaceTrainingModes.Bits.static_control_value_for_bcom0 = 1;
    DbInterfaceTrainingModes.Bits.static_bcom_control_enable = 1;
    RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, DbInterfaceTrainingModes.Data);
    ToggleBReset (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, 0);

    //
    // Exit RCD Static BCOM Mode
    //
    ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, &DbInterfaceTrainingModes.Data);
    DbInterfaceTrainingModes.Bits.static_bcom_control_enable = 0;
    RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, DbInterfaceTrainingModes.Data);

    //
    // Block commands to DRAM
    //
    ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, &ParityAndAlert.Data);
#ifdef USE_LATEST_RCD_SPEC
    ParityAndAlert.Bits.dram_interface_forward_all_cmds = 0;
#else // #ifdef USE_LATEST_RCD_SPEC
    ParityAndAlert.Bits.dram_interface_block_all_cmds = 0;
#endif // #ifdef USE_LATEST_RCD_SPEC
    RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, ParityAndAlert.Data);

    //
    // BCOM Drive stength
    //
    ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_DATA_BUFFER_INTERFACE_AND_LOOPBACK_DRIVER_CHARACTERISTICS_REG, &DbDriveStrength.Data);
#ifdef USE_LATEST_RCD_SPEC
    DbDriveStrength.Bits.bck_t_bck_c_driver_strength = 1;
    DbDriveStrength.Bits.bcom_2_0_bcs_n_driver_strength = 1;
    RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_DATA_BUFFER_INTERFACE_AND_LOOPBACK_DRIVER_CHARACTERISTICS_REG, DbDriveStrength.Data);
#endif // #ifdef USE_LATEST_RCD_SPEC

    //
    // Enter BCOM pass through Mode, set SDR2 (for non CTE)
    //
    ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_GLOBAL_FEATURES_REG, &GlobalFeatures.Data);
    if (FeaturePcdGet (PcdCteBuild) != TRUE) {
      GlobalFeatures.Bits.sdr_modes_1 = 1;
    }
    GlobalFeatures.Bits.bcom_pass_through_mode = 1;
    RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_GLOBAL_FEATURES_REG, GlobalFeatures.Data);
  } //Ch loop.

  return MRC_STATUS_SUCCESS;
}

/**
  Callback to configure the engine to do the Rmt BCOM test

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
RmtSetupTestBcom (
   IN   COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  UINT8                        Ch;
  UINT8                        Subchannel = 0;
  MRC_MST                      MemSsType;
  MRC_RT                       MrcRankList;
  UINT32                       OnechBitmask = 0;
  UINT8                        MaxChDdr;
  UINT8                        Dimm;
  UINT8                        Rank;
  PSYSHOST                     Host;
  UINT8                        Socket;
#if SUB_CH > 1
  GSM_CSN                      SignalSubCh[SUB_CH] = {BCOM_A, BCOM_B};
#else
  GSM_CSN                      SignalSubCh[SUB_CH] = {BCOM_A};
#endif
  SSA_MARGIN_BCOM              *PrivateBcom;

  PrivateBcom= (SSA_MARGIN_BCOM *) CommonParameters;

  Host = CommonParameters->Host;
  Socket = CommonParameters->Socket;
  MaxChDdr = GetMaxChDdr ();
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

    if (SkipChannelExecution (CommonParameters, Ch)) {
      continue;
    }

    Dimm = CommonParameters->Dimm[Ch];
    Rank = CommonParameters->Rank[Ch];

    //
    // Setup for BCOM CLK test
    //
    //
    // Indicate Rank present and enabled on this Ch.
    //
    OnechBitmask = 0;
    OnechBitmask |= 1 << Ch;
    MrcRankList.Dimm = Dimm;
    MrcRankList.Rank = Rank;
    for (Subchannel = 0; Subchannel < SUB_CH; Subchannel++) {
      SetupTest (Host, Socket, MemSsType, OnechBitmask, 1, &MrcRankList, DdrLevel, PrivateBcom->TestType, SignalSubCh[Subchannel], 0, 0);
    }

    //
    // Set the DQ count window
    //
    SetDataTrainCountWindowByChannel (Socket, Ch, (UINT8)(PrivateBcom->CountWindow));
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Cleanup the BCOM training.

  @param[in] Host - Pointer to sysHost

  @retval Status
**/
UINT32
RmtPostSweepBcomWorker (
  IN   COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  UINT8                      Ch = 0;
  UINT8                      Socket;
  UINT8                      Dimm;
  UINT8                      Rank = 0;
  UINT8                      MaxChDdr;
  DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_STRUCT      DbInterfaceTrainingModes;
  DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_STRUCT                  ParityAndAlert;
  DDR5_RCD_GLOBAL_FEATURES_STRUCT                              GlobalFeatures;
  PSYSHOST                                                     Host;

  MaxChDdr = GetMaxChDdr ();
  Host = CommonParameters->Host;
  Socket = Host->var.mem.currentSocket;

  //
  // Exit BCOM pass through mode.
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

    if (SkipChannelExecution (CommonParameters, Ch)) {
      continue;
    }

    Dimm = CommonParameters->Dimm[Ch];
    Rank = 0;

    //
    // Disable training modes
    //
    DbInterfaceTrainingModes.Data = 0;
    RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, DbInterfaceTrainingModes.Data);

    //
    // Exit BCOMTM -- BCOM[2:0] 010
    //
    DbInterfaceTrainingModes.Data = 0;
    DbInterfaceTrainingModes.Bits.static_control_value_for_bcom2 = 0;
    DbInterfaceTrainingModes.Bits.static_control_value_for_bcom1 = 1;
    DbInterfaceTrainingModes.Bits.static_control_value_for_bcom0 = 0;
    DbInterfaceTrainingModes.Bits.static_bcom_control_enable = 1;
    RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, DbInterfaceTrainingModes.Data);
    ToggleBReset (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, 0);

    //
    // Exit RCD Static BCOM Mode
    //
    ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, &DbInterfaceTrainingModes.Data);
    DbInterfaceTrainingModes.Bits.static_bcom_control_enable = 0;
    RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, DbInterfaceTrainingModes.Data);

    //
    // Disable BCOM pass thru mode, restore SDR mode
    //
    ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_GLOBAL_FEATURES_REG, &GlobalFeatures.Data);
    GlobalFeatures.Bits.bcom_pass_through_mode = 0;
    GlobalFeatures.Bits.sdr_modes_1 = 0;
    RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_GLOBAL_FEATURES_REG, GlobalFeatures.Data);

    //
    // Forward all commands to DRAM/DB
    //
    ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, &ParityAndAlert.Data);
#ifdef USE_LATEST_RCD_SPEC
    ParityAndAlert.Bits.dram_interface_forward_all_cmds = 1;
    ParityAndAlert.Bits.data_buffer_interface_forward_all_cmds = 1;
#else // #ifdef USE_LATEST_RCD_SPEC
    ParityAndAlert.Bits.dram_interface_block_all_cmds = 1;
    ParityAndAlert.Bits.data_buffer_interface_block_all_cmds = 1;
#endif // #ifdef USE_LATEST_RCD_SPEC
    RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, ParityAndAlert.Data);

    //
    // Disable training modes
    //
    DbInterfaceTrainingModes.Data = 0;
    RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, DbInterfaceTrainingModes.Data);

    //
    // Issue a ZQCal (DDRT2 Only)
    // A ZQCal is needed to set the ODT and Drive strength on MDQS and MDQS interfaces.
    //
    if (IsDcpmmPresentDimm (Socket, Ch, Dimm) == TRUE) {
      WriteMPC (Host, Socket, Ch, Dimm, SUB_CH_A_MASK, 0, ZQCAL_START, TIMING_1N, FALSE);
      FixedDelayMicroSecond (2); // delay at least tZQCAL (== 1 us), fixeddelay is roughly 1 microsec
      WriteMPC (Host, Socket, Ch, Dimm, SUB_CH_A_MASK, 0, ZQCAL_LATCH, TIMING_1N, FALSE);
      FixedDelayMicroSecond (1); // delay at least tZQLAT (< 1 us)
    }
  }

  PostBcomDdrioProgramming (Host, Socket);

  return SUCCESS;
}

/**
  Callback to configure the engine to do the Rmt BCOM test

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
RmtExecuteTestBcomTiming (
  IN  COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  MRC_MST                 MemSsType;
  UINT8                   Ch;
  UINT8                   SubChannel;
  UINT32                  ChBitMask;
  UINT8                   MaxChDdr;
  SSA_MARGIN_BCOM         *PrivateBcom;

  PrivateBcom= (SSA_MARGIN_BCOM *) CommonParameters;

  MaxChDdr = CommonParameters->MaxChDdr;
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[CommonParameters->Socket];

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (CommonParameters->Host)) {
    if (SkipChannelExecution (CommonParameters, Ch)) {
      continue;
    }

    ChBitMask = 1 << Ch;

    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      TestControl(
        CommonParameters->Host,
        CommonParameters->Socket,
        MemSsType,
        ChBitMask,
        SubChannel,
        PrivateBcom->TestType,
        1,
        START_TEST,
        NOT_STOP_TEST
      );
    }

    SetDataTrainCountWindowByChannel (CommonParameters->Socket, Ch, (UINT8)(PrivateBcom->CountWindow));
  }

  FixedDelayMicroSecond (10);

  //
  // Generate an IO Reset. This is required because the pi delays changed.
  // The MC reset is needed to enlarge the count window assert pulse to make sure the count winodw value can be reset to 0.
  //
  IO_Reset(CommonParameters->Host, CommonParameters->Socket);

  //
  // Enabling the counter Mode.
  //

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (CommonParameters->Host)) {
    if (SkipChannelExecution (CommonParameters, Ch)) {
      continue;
    }
    EnableDdrioTrainingCounterByChannel (CommonParameters->Host, CommonParameters->Socket, Ch);
  }

  return MRC_STATUS_SUCCESS;

}

/**
  Callback to configure the engine to do the Rmt BCOM Vref test

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
RmtExecuteTestBcomVref (
  IN  COMMON_1D_PARAMETERS    *CommonParameters
  )
{

  UINT8             ChannelIndex;
  UINT32            ChannelMask;
  UINT8             MaxCh;
  MRC_RT            DimmRank [MAX_CH];
  UINT32            PrevDebugLevel;
  SSA_MARGIN_BCOM   *PrivateBcom;
  struct baseMargin *BcomTimimg;

  PrivateBcom = (SSA_MARGIN_BCOM *) CommonParameters;
  BcomTimimg  = (struct baseMargin *)&(PrivateBcom->BcomTimimg[0][0]);

  PrevDebugLevel = GetDebugLevel ();
  SetDebugLevel (SDBG_NONE);

  MaxCh = CommonParameters->MaxChDdr;

  //
  // Run GetBcomTimingMargins to get the DIMM composite BCOM eye width as training result.
  //

  ChannelMask = 0xFFFFFFF;
  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    DimmRank[ChannelIndex].Dimm = CommonParameters->Dimm[ChannelIndex];
    DimmRank[ChannelIndex].Rank= CommonParameters->Rank[ChannelIndex];
    ChannelMask = ChannelMask & (UINT32)(~(1 << ChannelIndex));
  }

  GetBcomTimingMargins (
    PrivateBcom->TestType,
    CommonParameters->Socket,
    ChannelMask,
    DimmRank,
    CommonParameters->Level,
    BcomDelay,
    SWEEP_BOTH_POS_NEG_SIDE,
    2,
    PrivateBcom->CountWindow,
    BcomTimimg
    );

  SetDebugLevel (PrevDebugLevel);

  return MRC_STATUS_SUCCESS;

}

/**
  Callback to be executed when all sides are done

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
RmtPostSweepBcom (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  UINT8                   SubChannel;
  MRC_MST MemSsType;
  UINT8  ChannelIndex;
  UINT8  MaxCh;
  SSA_MARGIN_BCOM              *PrivateBcom;

  PrivateBcom= (SSA_MARGIN_BCOM *) CommonParameters;

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[CommonParameters->Socket];
  MaxCh = CommonParameters->MaxChDdr;

  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex += GetChInc (CommonParameters->Host)) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      TestControl (CommonParameters->Host, CommonParameters->Socket, MemSsType, (1 << ChannelIndex), SubChannel, PrivateBcom->TestType, 1, NOT_START_TEST, STOP_TEST);
    }
  }

  //
  // Disable CADB always on Mode
  //
  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex += GetChInc (CommonParameters->Host)) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    DisableCADBAlwaysOnMode (CommonParameters->Socket, ChannelIndex);
  }

  RmtPostSweepBcomWorker (CommonParameters);

  SetCurrentTestType (CommonParameters->Socket, 0);

  return MRC_STATUS_SUCCESS;
}


/**
  Callback to interpret the HW results after each sample test completes

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
RmtResultsProcessingBcomTiming (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  MRC_MST MemSsType;
  UINT8   ChannelIndex;
  UINT8   SubChannelIndex;
  UINT32  ChannelMask;
  UINT8   MaxCh;
  UINT32  DqSampleResultStrobeSubCh[SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT32  ValidMask[MAX_STROBE_DDR5];
  UINT8   StrobeLogical;
  UINT8   StrobeLinear;
  UINT8   BitIndex;
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
          if (DqSampleResultStrobeSubCh[SubChannelIndex][StrobeLinear] > 0) { // Report feedback
            ResultConditionPtr->Current = FAIL_CONDITION;
          } else {
            ResultConditionPtr->Current = PASS_CONDITION;
          }
        } // BitIndex loop ...
      } // StrobeLinear
    } // SuBChannelIndex loop ...
  } // ChannelIndex loop ...

  return MRC_STATUS_SUCCESS;
}

/**
  Callback to interpret the HW results after each sample test completes.
  This function use BCOM eye width as feedback to check if BVref is valid.

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
RmtResultsProcessingBcomVref (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  RESULTS_1D_CONDITION  *ResultConditionPtr;
  UINT8             ChannelIndex;
  UINT8             SubChannelIndex;
  UINT8             MaxCh;
  UINT8             StrobeLogical;
  UINT8             StrobeLinear;
  UINT8             BitIndex;
  INT16             EyeWidth;
  SSA_MARGIN_BCOM  *PrivateBcom;
  struct baseMargin *BcomTimimg;

  PrivateBcom = (SSA_MARGIN_BCOM *) CommonParameters;
  BcomTimimg  = (struct baseMargin *)&(PrivateBcom->BcomTimimg[0][0]);
  MaxCh = CommonParameters->MaxChDdr;

  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    //
    // Use BCOM eye width as feedback to check if BVref is valid.
    //
    for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {


      EyeWidth = BcomTimimg[ChannelIndex * SUB_CH + SubChannelIndex].p - BcomTimimg[ChannelIndex * SUB_CH + SubChannelIndex].n;

      RcDebugPrintWithDevice (SDBG_MAX, CommonParameters->Socket, ChannelIndex, CommonParameters->Dimm[ChannelIndex], SubChannelIndex, NO_RANK, NO_STROBE, NO_BIT,
        "BCOM EyeWidth %d\n", EyeWidth);

      for (StrobeLinear = 0; StrobeLinear < CommonParameters->MaxStrobeAvailable; StrobeLinear++) {

        if (SkipExecutionOnStrobe (CommonParameters, ChannelIndex, StrobeLinear)) {
          continue;
        }

        StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);

        for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {
          ResultConditionPtr = GetResultConditionPtr (CommonParameters, 0, ChannelIndex, StrobeLogical, BitIndex);
          if ((EyeWidth == 0) || (EyeWidth > BACKSIDE_BCOM_FULL_EYE_WIDTH)) {
            //
            // Failure: Eye width is too small or no eye detected, which means Bvref has reached top or bottom of eye.
            //
            ResultConditionPtr->Current = FAIL_CONDITION;
          } else {
            //
            // Pass.
            //
            ResultConditionPtr->Current = PASS_CONDITION;
          }
       } // BitIndex loop ...
      } // StrobeLinear
    } // SubChannelIndex loop ...

  } // ChannelIndex loop ...

  return MRC_STATUS_SUCCESS;
}

/**

  Search the data collected in Results and update margin results in channel/subchannel/signal array
  WARNING:
  This function MUST be executed only after context was already created (CreateContextParameters).
  This function MUST be executed only after resources are allocated (AllocatePoolForResults1D ).

  @param[in]      CommonParameters  Pointer to the context information
  @param[in]      Results1D         Pointer to 1D results
  @param[in]      SignalIndex       The index of signal in BcomResults arrray
  @param[out]     BcomResults       Pointer that contains per channel, subchannel and signal margin result.

**/
VOID
RmtUpdateBcomMarginResultsFromCurrentResults1D (
  IN     COMMON_1D_PARAMETERS    *CommonParameters,
  IN     RESULTS_1D              *Results1D,
  OUT    struct baseMargin       *BcomResults
  )
{
  UINT8             ChannelIndex;
  UINT8             SubChannelIndex;
  UINT8             StrobeLogical;
  UINT8             StrobeLinear;
  struct baseMargin *StrobeMargin;
  struct baseMargin *BcomResultForCurrentSignal;
  INT16             LeftCompositeMargin;
  INT16             RightCompositeMargin;
  UINT16            PiIndex;
  INT16             *CurrentDelayPtr;
  UINT16            MinLimit;
  UINT16            MaxLimit;
  UINT16            Delay;
  CHAR8             *GroupString;

  GetGroupString (CommonParameters->Group, &GroupString);

  for (ChannelIndex = 0; ChannelIndex < CommonParameters->MaxChDdr; ChannelIndex++) {
    if (!CommonParameters->ChannelEnabledLocal[ChannelIndex]) {
      continue;
    }

    for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {

      if ((CommonParameters->FlagsResults & ResultsPerSubChannel) == ResultsPerSubChannel) {
        //
        // ResultsPerSubChannel is used, get result of the first strobe is enough. BCOM Vref would use this.
        //

        StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, 0);

        StrobeMargin = GetResultPtrAtStrobeLevel (CommonParameters, 0, ChannelIndex, StrobeLogical, Results1D);

        BcomResultForCurrentSignal = &BcomResults[ChannelIndex * SUB_CH + SubChannelIndex];
        BcomResultForCurrentSignal->p = StrobeMargin->p;
        BcomResultForCurrentSignal->n = StrobeMargin->n;
      } else {

        //
        // BCOM timing margining would use this.
        //

        LeftCompositeMargin = MIN_INT16;
        RightCompositeMargin = MAX_INT16;

        CurrentDelayPtr = GetDelayPtr (CommonParameters, 0, ChannelIndex, 0, CommonParameters->CurrentDelays);
        PiIndex = *CurrentDelayPtr;

        GetDimmParamGroupLimits (LrbufLevel, CommonParameters->Group, &MinLimit, &MaxLimit, &Delay);

        if (PiIndex >= MaxLimit || (PiIndex <= MinLimit) ) {

            //
            // No edge found. Report that no eye detected.
            //
            LeftCompositeMargin = 0;
            RightCompositeMargin = 0;
        } else {

          //
          // Valid edges found. Update composite margin.
          //
          for (StrobeLinear = 0; StrobeLinear < CommonParameters->MaxStrobeAvailable; StrobeLinear++) {
            if (SkipExecutionOnStrobe (CommonParameters, ChannelIndex, StrobeLinear)) {
              continue;
            }

            StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, StrobeLinear);
            StrobeMargin = GetResultPtrAtStrobeLevel (CommonParameters, 0, ChannelIndex, StrobeLogical, Results1D);

            if (StrobeMargin->n > LeftCompositeMargin) {
              LeftCompositeMargin = StrobeMargin->n;
            }

            if (StrobeMargin->p < RightCompositeMargin) {
              RightCompositeMargin = StrobeMargin->p;
            }
          } // StrobeLinear
        } // if

        BcomResultForCurrentSignal = &BcomResults[ChannelIndex * SUB_CH + SubChannelIndex];
        BcomResultForCurrentSignal->p = RightCompositeMargin;
        BcomResultForCurrentSignal->n = LeftCompositeMargin;
      }

      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, CommonParameters->Socket, ChannelIndex, CommonParameters->Dimm[ChannelIndex], SubChannelIndex, NO_RANK, NO_STROBE, NO_BIT,
        "%a  Margin low %d high %d \n", GroupString, BcomResultForCurrentSignal->n, BcomResultForCurrentSignal->p);
    } // SubChannelIndex loop ...
  } // ChannelIndex loop
}

/**
  Sweep BCOM PI delay signals and return rank (DIMM) level BCOM timing margin results for EV tests.

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
  UINT8                   ChannelIndex;
  UINT8                   SubChannelIndex;
  UINT8                   SubChannelParameter;
  SSA_MARGIN_BCOM         PrivateBcom;
  RESULTS_1D              Results;
  MRC_MST                 MemSsType;
  FLAGS_CATEGORY_OFFSETS  FlagsOffsets;
  FLAGS_CATEGORY_RESULTS  FlagsResults;
  FLAGS_CATEGORY_FSM      FlagsFsm;
  PSYSHOST                Host;
  //UINT32                  PrevDebugLevel;
  UINT8                   MaxCh;
  DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_STRUCT              RcdParityAlert[MAX_CH];
  DDR5_RCD_GLOBAL_FEATURES_STRUCT                          GlobalFeatures[MAX_CH];
  DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_STRUCT  DbInterfaceTrainingModes[MAX_CH];
  DDR5_RCD_BCS_N_AND_BCOM_2_0_OUTPUT_DELAY_STRUCT          BcomOutputDelay;
  INT16                                                    AdjustOffset;

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  MaxCh = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  PrivateBcom.TestType = TestType;
  PrivateBcom.CountWindow = CountWindow;

  //
  // Save orignal RCD CWs and push out BCOM delay.
  //
  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {
    if (!IsChannelEnabled (Socket, ChannelIndex)) {
      continue;
    }
    if (ChannelBitMask & (1 << ChannelIndex)) {
      continue;
    }

    ReadRcdCwCache (Socket, ChannelIndex, SUB_CH_A, DimmRank[ChannelIndex].Dimm, RDIMM_CW_PAGE0, DDR5_RCD_GLOBAL_FEATURES_REG, &GlobalFeatures[ChannelIndex].Data);
    ReadRcdCwCache (Socket, ChannelIndex, SUB_CH_A, DimmRank[ChannelIndex].Dimm, RDIMM_CW_PAGE0, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, &RcdParityAlert[ChannelIndex].Data);
    ReadRcdCwCache (Socket, ChannelIndex, SUB_CH_A, DimmRank[ChannelIndex].Dimm, RDIMM_CW_PAGE0, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, &DbInterfaceTrainingModes[ChannelIndex].Data);

    //
    // Need to push out the BCOM delay by one BCOM_CLOCK_CYCLE because the final programmed value was already
    // reduced by BCOM_CLOCK_CYCLE from orignal center at end of the BCOM training in VrefBcomTraining(). Othewise
    // there is no room or no enough room to margin the negative side.
    //
    for (SubChannelIndex = 0; SubChannelIndex < SUB_CH; SubChannelIndex++) {
      ReadRcdCwCache (Socket, ChannelIndex, SubChannelIndex, DimmRank[ChannelIndex].Dimm, RDIMM_CW_PAGE0, DDR5_RCD_BCS_N_AND_BCOM_2_0_OUTPUT_DELAY_REG, &BcomOutputDelay.Data);
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, ChannelIndex, DimmRank[ChannelIndex].Dimm, SubChannelIndex, DimmRank[ChannelIndex].Rank, NO_STROBE, NO_BIT,
        "Before BCOM adjustment setting: 0x%X\n", BcomOutputDelay.Data);
      AdjustOffset = BCOM_CLOCK_CYCLE;
      GetSetBcomDelay (Host, Socket, ChannelIndex, SubChannelIndex, DimmRank[ChannelIndex].Dimm, 0, GSM_WRITE_OFFSET, &AdjustOffset);
      ReadRcdCwCache (Socket, ChannelIndex, SubChannelIndex, DimmRank[ChannelIndex].Dimm, RDIMM_CW_PAGE0, DDR5_RCD_BCS_N_AND_BCOM_2_0_OUTPUT_DELAY_REG, &BcomOutputDelay.Data);
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, ChannelIndex, DimmRank[ChannelIndex].Dimm, SUB_CH_A, DimmRank[ChannelIndex].Rank, NO_STROBE, NO_BIT,
        "After BCOM adjustment +64 setting: 0x%X\n", BcomOutputDelay.Data);
    }
  }



  //
  // 1D Sweep.
  //

  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {
    if (!IsChannelEnabled (Socket, ChannelIndex)) {
      ChannelBitMask |= 1 << ChannelIndex;
      continue;
    }
  }

  FlagsOffsets = DefaultStartOffset;
  FlagsResults = DefaultResults | SingleBitTrack | ResultsRelative | FocusOnTransition;
  if (Direction == SWEEP_NEG_SIDE_ONLY) {
    FlagsFsm = LeftSideOnly;
  } else if (Direction == SWEEP_POS_SIDE_ONLY) {
    FlagsFsm = RightSideOnly;
  } else {
    FlagsFsm = DefaultFsm; // both side.
  }

  SubChannelParameter = ALL_SUBCH;

  CreateContextParameters (
    (COMMON_1D_PARAMETERS *)&PrivateBcom,
    MemSsType,
    Socket,
    ChannelBitMask,
    0x00,
    SubChannelParameter,
    DimmRank,
    Level,
    Group,
    BCOM_A,
    StepSize,
    1,  // DitherValue
    -1, // OffsetLeft
    0,  // OffsetRight
    0,  // LimitOffset
    FlagsOffsets, FlagsResults, FlagsFsm
  );

  AllocatePoolForResults1D ((COMMON_1D_PARAMETERS *)&PrivateBcom, &Results);

  //
  // Enable sweep debug log
  //
  // PrevDebugLevel = GetDebugLevel ();
  // ((COMMON_1D_PARAMETERS *)&PrivateBcom)->DebugLevel |= SDBG_SWEEP_L3 | SDBG_SWEEP_L2 | SDBG_SWEEP_L1 |SDBG_SWEEP_L0;
  // SetDebugLevel (SDBG_SWEEP_L3 | SDBG_SWEEP_L2 | SDBG_SWEEP_L1 |SDBG_SWEEP_L0 |PrevDebugLevel);
  //

  //
  // Trigger 1D Sweep.
  //
  ProcessSweep1D (
    (COMMON_1D_PARAMETERS *)&PrivateBcom,
    RmtPreSweepBcom1D,
    RmtSetupTestBcom,
    RmtExecuteTestBcomTiming,
    NULL,
    RmtPostSweepBcom,
    RmtResultsProcessingBcomTiming,
    &Results
  );

  RmtUpdateBcomMarginResultsFromCurrentResults1D ((COMMON_1D_PARAMETERS *)&PrivateBcom, &Results, BcomResults);
  ReleaseResources1D ((COMMON_1D_PARAMETERS *)&PrivateBcom, &Results);

  //
  // Restore original debug level.
  //
  // SetDebugLevel (PrevDebugLevel);
  //

  //
  // Restore orignal RCD CWs and pull in BCOM delay.
  //
  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {
    if (!IsChannelEnabled (Socket, ChannelIndex)) {
      continue;
    }
    if (ChannelBitMask & (1 << ChannelIndex)) {
      continue;
    }

    RcdControlWordWrite (Socket, ChannelIndex, SUB_CH_A_MASK, DimmRank[ChannelIndex].Dimm, DimmRank[ChannelIndex].Rank, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, DbInterfaceTrainingModes[ChannelIndex].Data);
    RcdControlWordWrite (Socket, ChannelIndex, SUB_CH_A_MASK, DimmRank[ChannelIndex].Dimm, DimmRank[ChannelIndex].Rank, DDR5_RCD_GLOBAL_FEATURES_REG, GlobalFeatures[ChannelIndex].Data);
    RcdControlWordWrite (Socket, ChannelIndex, SUB_CH_A_MASK, DimmRank[ChannelIndex].Dimm, DimmRank[ChannelIndex].Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, RcdParityAlert[ChannelIndex].Data);

    //
    // Need to pull in one BCOM_CLOCK_CYCLE to return the original setting.
    //
    for (SubChannelIndex = 0; SubChannelIndex < SUB_CH; SubChannelIndex++) {
      ReadRcdCwCache (Socket, ChannelIndex, SubChannelIndex, DimmRank[ChannelIndex].Dimm, RDIMM_CW_PAGE0, DDR5_RCD_BCS_N_AND_BCOM_2_0_OUTPUT_DELAY_REG, &BcomOutputDelay.Data);
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, ChannelIndex, DimmRank[ChannelIndex].Dimm, SubChannelIndex, DimmRank[ChannelIndex].Rank, NO_STROBE, NO_BIT,
        "Restore: Before BCOM adjustment setting: 0x%X\n", BcomOutputDelay.Data);
      AdjustOffset = -BCOM_CLOCK_CYCLE;
      GetSetBcomDelay (Host, Socket, ChannelIndex, SubChannelIndex, DimmRank[ChannelIndex].Dimm, 0, GSM_WRITE_OFFSET, &AdjustOffset);
      ReadRcdCwCache (Socket, ChannelIndex, SubChannelIndex, DimmRank[ChannelIndex].Dimm, RDIMM_CW_PAGE0, DDR5_RCD_BCS_N_AND_BCOM_2_0_OUTPUT_DELAY_REG, &BcomOutputDelay.Data);
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, ChannelIndex, DimmRank[ChannelIndex].Dimm, SUB_CH_A, DimmRank[ChannelIndex].Rank, NO_STROBE, NO_BIT,
        "Restore: After BCOM adjustment -64 setting: 0x%X\n", BcomOutputDelay.Data);
    }
  }

  return MRC_STATUS_SUCCESS;
}


/**
  Sweep BCOM Vref signals and return rank (DIMM) level BCOM Vref margin results for EV tests.

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
  UINT8                   ChannelIndex;
  UINT8                   SubChannelParameter;
  SSA_MARGIN_BCOM         PrivateBcom;
  RESULTS_1D              Results;
  MRC_MST                 MemSsType;
  FLAGS_CATEGORY_OFFSETS  FlagsOffsets;
  FLAGS_CATEGORY_RESULTS  FlagsResults;
  FLAGS_CATEGORY_FSM      FlagsFsm;
  PSYSHOST                Host;
  UINT32                  PrevDebugLevel;
  UINT8                   MaxCh;
  DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_STRUCT  DbInterfaceTrainingModes[MAX_CH];

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  MaxCh = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  PrivateBcom.TestType = TestType;
  PrivateBcom.CountWindow = CountWindow;

  //
  // Enable sweep debug log
  //
  PrevDebugLevel = GetDebugLevel ();
  //((COMMON_1D_PARAMETERS *)&PrivateBcom)->DebugLevel |= SDBG_SWEEP_L3 | SDBG_SWEEP_L2 | SDBG_SWEEP_L1 |SDBG_SWEEP_L0;
  //SetDebugLevel (SDBG_SWEEP_L3 | SDBG_SWEEP_L2 | SDBG_SWEEP_L1 |SDBG_SWEEP_L0 | PrevDebugLevel);

  //
  // Save some RCD CWs before training.
  //
  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {
    if (!IsChannelEnabled (Socket, ChannelIndex)) {
      continue;
    }
    if (ChannelBitMask & (1 << ChannelIndex)) {
      continue;
    }

    ReadRcdCwCache (
      Socket,
      ChannelIndex,
      SUB_CH_A,
      DimmRank[ChannelIndex].Dimm,
      RDIMM_CW_PAGE0,
      DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG,
      &DbInterfaceTrainingModes[ChannelIndex].Data
      );
  }

  //
  // 1D Sweep.
  //

  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {
    if (!IsChannelEnabled (Socket, ChannelIndex)) {
      ChannelBitMask |= 1 << ChannelIndex;
      continue;
    }
  }

  FlagsOffsets = DefaultStartOffset;
  FlagsResults = DefaultResults | SingleBitTrack | ResultsRelative | ResultsPerSubChannel;
  if (Direction == SWEEP_NEG_SIDE_ONLY) {
    FlagsFsm = LeftSideOnly;
  } else if (Direction == SWEEP_POS_SIDE_ONLY) {
    FlagsFsm = RightSideOnly;
  } else {
    FlagsFsm = DefaultFsm; // both side.
  }

  SubChannelParameter = ALL_SUBCH;

  CreateContextParameters (
    (COMMON_1D_PARAMETERS *)&PrivateBcom,
    MemSsType,
    Socket,
    ChannelBitMask,
    0x00,
    SubChannelParameter,
    DimmRank,
    Level,
    Group,
    BCOM_A,
    StepSize,
    1,  // DitherValue
    -1, // OffsetLeft
    0,  // OffsetRight
    0,  // LimitOffset
    FlagsOffsets, FlagsResults, FlagsFsm
  );

  AllocatePoolForResults1D ((COMMON_1D_PARAMETERS *)&PrivateBcom, &Results);

  //
  // Trigger 1D Sweep.
  //
  ProcessSweep1D (
    (COMMON_1D_PARAMETERS *)&PrivateBcom,
    NULL,
    NULL,
    RmtExecuteTestBcomVref,
    NULL,
    NULL,
    RmtResultsProcessingBcomVref,
    &Results
  );

  RmtUpdateBcomMarginResultsFromCurrentResults1D ((COMMON_1D_PARAMETERS *)&PrivateBcom, &Results, BcomResults);
  ReleaseResources1D ((COMMON_1D_PARAMETERS *)&PrivateBcom, &Results);

  //
  // Restore original debug level.
  //
  SetDebugLevel (PrevDebugLevel);

  //
  // Restore orignal RCD CWs.
  //
  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {
    if (!IsChannelEnabled (Socket, ChannelIndex)) {
      continue;
    }
    if (ChannelBitMask & (1 << ChannelIndex)) {
      continue;
    }

    RcdControlWordWrite (
      Socket,
      ChannelIndex,
      SUB_CH_A_MASK,
      DimmRank[ChannelIndex].Dimm,
      DimmRank[ChannelIndex].Rank,
      DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG,
      DbInterfaceTrainingModes[ChannelIndex].Data
      );
  }

  return MRC_STATUS_SUCCESS;
}
