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
#include <Memory/CpgcDefinitions.h>
#include "Ddr5Core.h"
#include <Library/Ddr5CoreLib.h>
#include <Memory/Ddr5Cmds.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CteNetLib.h>
#include "MemHostChipCommonDdr5.h"
#include <Library/SystemInfoLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/RcMemAllocLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/OdtTableLib.h>
#include <Memory/Ddr5RcwRegs.h>
#include <Memory/JedecDefinitions.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Library/SiliconWorkaroundPMemLib.h>

#define NO_SIGNAL                         0xff
#define CS_Training_Counting_Window       0x10
#define CS_WRAP_AROUND_VALUE              256
#define CS_FULL_EYE_WIDTH                 128
#define BACKSIDE_CS_WRAP_AROUND_VALUE     128
#define BACKSIDE_CS_FULL_EYE_WIDTH        64
#define CS_PIN_INDEX(DIMM, SUBCH, RANK) (DIMM * 4 + SUBCH * 2 + RANK)
#define DCLK_PI                           128
#define DCS_DELAY_GAURDBAND               0
#define MAX_DCS_DELAY_PI                  CS_WRAP_AROUND_VALUE - DCS_DELAY_GAURDBAND
#define MIN_DCS_DELAY_PI                  DCS_DELAY_GAURDBAND
//
// Local Prototypes
//
INT8
GetCsVrefInitValue (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  UINT8     Dimm,
  IN  UINT8     Rank
  );
VOID   GetCsMarginsSweep (PSYSHOST Host, UINT8 Socket, UINT8 Dimm, UINT8 Rank, GSM_CSN Sig[MAX_CH][SUB_CH], INT16 Le[MAX_CH][SUB_CH], INT16 Re[MAX_CH][SUB_CH], INT16 EyeWidth[MAX_CH][SUB_CH], INT16 EyeCenterPoint[MAX_CH][SUB_CH], INT8 Vref, struct    ClkTrainingResults (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]);
UINT32 EarlyCsClkLoopback (PSYSHOST  Host);
UINT32 EarlyCsClkRcdLoopback (IN PSYSHOST Host, IN UINT8 TrainingMode);
VOID   EnableMcParityChecking (PSYSHOST Host, UINT8 Socket);
UINT32 EarlyCsClkBacksideLoopback (PSYSHOST  Host);
/**

Checks if a given DCS needs to be trained for a given SubCh, Dimm, Rank

@param[in] Signal      - Signal
@param[in] SubChMask   - bit map sub channel mask
@param[in] Dimm        - DIMM number
@param[in] Rank        - Rank number

@retval TRUE: signal needs to be trained for this configuration
        FALSE: signal does not need to be trained
**/
BOOLEAN
DcsSignalAppliesToThisDimmRank (
  IN GSM_CSN   Signal,
  IN UINT8     SubChMask,
  IN UINT8     Dimm,
  IN UINT8     Rank
  );

#define SETUP_FOR_RE_TEST BIT0
#define TEAR_DOWN_RE_TEST BIT1
#define EXECUTE_RE_TEST   BIT2

GSM_CSN   CsToSweep[NUM_CS] = {
  CS0_A_N, CS1_A_N,
  CS0_B_N, CS1_B_N,
  CS2_A_N, CS3_A_N,
  CS2_B_N, CS3_B_N
};

//
// This is the shared signal name between A copy and B copy of each QCS signal.
//
GSM_CSN   CsToSweepBackSide[NUM_CS] = {
  QCS0_A_n, QCS1_A_n,  // D0 R0 SUB CH A; D0 R1 SUB CH A
  QCS0_B_n, QCS1_B_n,  // D0 R0 SUB CH B; D0 R1 SUB CH B
  QCS2_A_n, QCS3_A_n,  // D1 R0 SUB CH A; D1 R1 SUB CH A
  QCS2_B_n, QCS3_B_n,  // D1 R0 SUB CH B; D1 R1 SUB CH B
};


#pragma pack (push, 1)
typedef struct _SSA_MARGIN_CS {
  COMMON_1D_PARAMETERS      CommonParameters;
  MRC_TT                    TestType;
  UINT32                    CountWindow;
} SSA_MARGIN_CS;

#pragma pack (pop)

/**
  Callback to be executed before RMT DCS sweep is done

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
PreSweepDcs1D (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
);

/**
  Callback to configure the engine to do the DCS test by test type

  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      TestType          Indicates the test type to be configured
  @param[in]      CountWindow       Count Window Value to program

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetupTestDcsWorker (
  IN COMMON_1D_PARAMETERS    *CommonParameters,
  IN MRC_TT                  TestType,
  IN UINT32                  CountWindow
  );

/**
  Callback to configure the engine to do the DCS test

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetupTestDcs (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
);

/**
  Callback to configure the engine to do the RMT DCS test

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetupTestRmtDcs (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  );

/**
  Callback to configure and start the DCS test

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
ExecuteTestDcs (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
);

/**
  Callback to interpret the HW results after each sample test completes

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
ResultsProcessingDcs (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
);

/**
  Callback to be executed when all sides are done

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
PostSweepDcs1D (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
);

/**
  Callback to be executed before QCS sweep is done

  @param[in]      CommonParameters    Pointer to the context information
  @param[in]      TestType            Current Test Type

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
PreSweepQcs1DWorker(
  IN        COMMON_1D_PARAMETERS    *CommonParameters,
  IN        MRC_TT                  TestType
  );

/**
  Callback to be executed before RMT QCS sweep is done

  @param[in]      CommonParameters    Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
PreSweepQcs1DRmt (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  );

/**
  Callback to configure the engine to do the QCS test by test type

  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      TestType          Indicates the test type to be configured
  @param[in]      CountWindow       Count Window Value to program

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetupTestQcsWorker (
  IN COMMON_1D_PARAMETERS    *CommonParameters,
  IN MRC_TT                  TestType,
  IN UINT32                  CountWindow
  );

/**
  Callback to configure the engine to do the RMT QCS test

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetupTestRmtQcs (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  );

/**
  Callback to configure and start the QCS test

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
ExecuteTestQcs (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
);

/**
  Callback to interpret the HW results after each QCA sample test completes

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
ResultsProcessingQcs (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
);

/**
  Callback to be executed QCS sweep is done

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
PostSweepQcs1D (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
);

/**

Exectues early cs/clock training

@param Host - Pointer to sysHost

@retval SUCCESS

**/
UINT32
EarlyCsClk (
  PSYSHOST  Host
  )
{
  UINT8               Socket;
  UINT8               Ch;
  UINT8               MaxChDdr;
  MRC_STATUS          Status;
  UINT8               TrainingMode = 0;

  if (!IsMemFlowEnabled (EarlyCsClkTraining)) {
    return SUCCESS;
  }

  Socket = Host->var.mem.currentSocket;
  //
  // Return if this Socket is disabled
  //
  if (IsSocketDimmPopulated (Host, Socket) != TRUE) {
    return SUCCESS;
  }
  //
  // Return if DDR5 MemSs is not present.
  //
  if (!IsDdr5Present (Host, Socket)) {
    return MRC_STATUS_FAILURE;
  }

  Status = TrainingModeSelect (EarlyCsClkTraining, &TrainingMode);
  if (Status != MRC_STATUS_SUCCESS) {
    return Status;
  }
  //
  // Push the inital clocks by 32 ticks
  //
  InitializeClkDelay (32);

  DoCompDdrioPerSetupOption (Socket, TRAINING_COMP_EVERY_TRAINING);

  if (CheckRCDLoopback (Host)) {

    SetCurrentTestType (Socket, EarlyRcdCsLoopbackTest);
    //
    // Early CS CLK training for Host side. Execute RCD Loopback for RDIMM, LRDIMM, DDRT2, and NVM DIMM configs
    //
    EarlyCsClkRcdLoopback (Host, TrainingMode);
  } else {
    SetCurrentTestType (Socket, EarlyCsLoopbackTest);
    //
    // For UDIMM
    //
    EarlyCsClkLoopback (Host);
  }
  MaxChDdr = GetMaxChDdr ();
  //
  // Disable CADB always on mode
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    DisableCADBAlwaysOnMode (Socket, Ch);
  }
  SetCurrentTestType (Socket, 0);

  JedecInitPreTraining (Host, Socket);

  return SUCCESS;
} // EarlyCsClk

/**

  Exectues early backside cs/clock training

  @param Host - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
EarlyCsClkBackside (
  PSYSHOST  Host
  )
{
  UINT8               Socket;
  UINT8               MaxChDdr;

  if (!IsMemFlowEnabled (EarlyBacksideCsClkTraining)) {
    return SUCCESS;
  }
  if (!(CheckRCDLoopback (Host))) {
    return SUCCESS;
  }
  Socket = Host->var.mem.currentSocket;
  MaxChDdr = GetMaxChDdr ();
  //
  // Return if DDR5 MemSs is not present.
  //
  if (!IsDdr5Present (Host, Socket)) {
    return MRC_STATUS_FAILURE;
  }

  Host->var.mem.checkMappedOutRanks = 1;
  SetCurrentTestType (Socket, EarlyCsBacksideLoopbackTest);

  //
  // Return if this Socket is disabled
  //

  if (IsSocketDimmPopulated (Host, Socket) != TRUE) {
    return SUCCESS;
  }

  //
  // Early CS CLK training for backside. Execute RCD Loopback for RDIMM, LRDIMM, NVM DIMM configs.
  //
  EarlyCsClkBacksideLoopback (Host);

  Host->var.mem.checkMappedOutRanks = 0;
  SetCurrentTestType (Socket, 0);
  return SUCCESS;
} // EarlyCsClkBackside

/**

  Displays the training results

  @param[in] Host   - Pointer to sysHost, the system Host (root) structure struct
  @param[in] socket - Socket Id

  @retval None

**/
VOID
EFIAPI
DisplayBacksideCCCResults (
  IN  PSYSHOST Host,
  IN  UINT8    Socket
  )
{
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               SubCh;
  UINT8               BacksideCopy;
  UINT8               Index;
  INT16               Delay;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  RcDebugPrint (SDBG_MEM_TRAIN, "\nSTART_DATA_RCD_QCA_QCS_DELAY\n");

  RcDebugPrint (SDBG_MEM_TRAIN,
    "SIGNAL:         QACS0   QBCS0   QACS1   QBCS1\n");

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < DdrDimmsPerChannel (Socket, Ch); Dimm++) {

      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, SubCh, NO_RANK, NO_STROBE, NO_BIT, "");

        for (Rank = 0; Rank < GetNumberOfRanksOnDimm(Socket, Ch, Dimm); Rank++) {
          for (BacksideCopy = 0; BacksideCopy < GetMaxBacksideCopyNum (Socket, Ch, Dimm); BacksideCopy++) {
            Index = GetBacksideCsSignalIndex (Ch, SubCh, Dimm, Rank);
            GetSetQcsDelay (Host, Socket, Ch, Dimm, Rank, CsToSweepBackSide[Index], BacksideCopy, GSM_READ_ONLY, &Delay, NULL, NULL);
            RcDebugPrint (SDBG_MEM_TRAIN, "%3d     ", Delay);
          }
        }
        RcDebugPrint (SDBG_MEM_TRAIN, "\n");
      }
    }
  }


  RcDebugPrint (SDBG_MEM_TRAIN,
    "STOP_DATA_RCD_QCA_QCS_DELAY\n");
} // DisplayCCCResults

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
@param centerPoint       - Pointer to save the center point
@param FallingEdge       - Pointer to save the falling edge
@param PulseWidth        - Pointer to save the pulse width
@param Step              - The PI Step to work on:
                           PI_CS_CLK_TYPE
@param SubChannel        - sub channel number
@param TrainResinput     - Pointer to ClkTrainingResults


@retval N/A

**/
VOID
EvaluatePiSettingResultsCsClk (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT8     Rank,
  UINT8     Strobe,
  UINT16    RisingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5/2],
  UINT16    FallingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5/2],
  UINT8     Step,
  UINT8     SubChannel,
  MRC_TT    CurrentTestType,
  struct    ClkTrainingResults (*TrainResinput) [MAX_CH][SUB_CH][MAX_STROBE_DDR5/2]
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
  struct ddrRank (*RankList) [MAX_RANK_DIMM];
  struct ClkTrainingResults (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5/2];

  Register = 0;
  OneStart = 0;

  Bit = BIT0;
  OneCount = 0;
  MaxOneCount = 0;
  CyclicCase = 0;

  TrainRes = TrainResinput;
  RisingEdge[Ch][SubChannel][Strobe] = 0;
  FallingEdge[Ch][SubChannel][Strobe] = 0;


  RankList = GetRankNvList (Host, Socket, Ch, Dimm);

  //
  // Start at bit 0
  //
  CsClkSetupPiSettingsParams (Host, Socket, Step, &PiSetting, &PiStart, &NumberOfCycle, CurrentTestType, &TrainRes);
  Host->var.mem.piSettingStopFlag[Ch] = GetStrobeMask(Host); //to make sure all the Strobe will be handled.
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
        // PulseWidth[Ch][SubChannel][Strobe] = FallingEdge[Ch][SubChannel][Strobe] - RisingEdge[Ch][SubChannel][Strobe];
        // centerPoint[Ch][SubChannel][Strobe] = ((PulseWidth[Ch][SubChannel][Strobe] / 2) + RisingEdge[Ch][SubChannel][Strobe]) % (NumberOfCycle - PiStart);
      }
    }
    //
    // Check if this is a cyclic case and if we are at the end of the one's
    //
    if ((CyclicCase == 1) && (OneCount == 0)) {
      Host->var.mem.piSettingStopFlag[Ch] &= ~(1 << Strobe);
     /* if (RisingEdge[Ch][SubChannel][Strobe] > FallingEdge[Ch][SubChannel][Strobe]) {
        //PulseWidth[Ch][SubChannel][Strobe] = ((NumberOfCycle - RisingEdge[Ch][SubChannel][Strobe]) + FallingEdge[Ch][SubChannel][Strobe]) - PiStart;
        //centerPoint[Ch][SubChannel][Strobe] = (((PulseWidth[Ch][SubChannel][Strobe] / 2) + RisingEdge[Ch][SubChannel][Strobe])) % (NumberOfCycle - PiStart);
      } else {
        //
        // last bit is 1 and first bit after cycle is 0 bit 127 = 1 and bit 0 = 0
        //
        //PulseWidth[Ch][SubChannel][Strobe] = FallingEdge[Ch][SubChannel][Strobe] - RisingEdge[Ch][SubChannel][Strobe];
        //centerPoint[Ch][SubChannel][Strobe] = ((PulseWidth[Ch][SubChannel][Strobe] / 2) + RisingEdge[Ch][SubChannel][Strobe]) % (NumberOfCycle - PiStart);
      }*/
    }
  } // while loop

  /*if (FAULTY_PARTS_TRACKING_INJECT_ERROR == 1) {
    if ((Ch == 0) && (Dimm == 0) && (Rank == 1) && (Strobe == 5)) {
      //PulseWidth[Ch][SubChannel][Strobe] = 1;
      //centerPoint[Ch][SubChannel][Strobe] = ((PulseWidth[Ch][SubChannel][Strobe] / 2) + RisingEdge[Ch][SubChannel][Strobe]) % (NumberOfCycle - PiStart);
    }
  }*/
  // FAULTY_PARTS_TRACKING_INJECT_ERROR

  //
  // If the PulseWidth (ones stream) is smaller than threshold need to sample again
  //
 /* if (PulseWidth[Ch][SubChannel][Strobe] <= RE_EDGE_SIZE_THRESHOLD) { //DDR5_TODO. the DDR5 CS CLK min pulse width is also RE_EDGE_SIZE_THRESHOLD ?
    //
    // This is a fatal error condition so map out the entire channel
    //
    Host->nvram.mem.socket[Socket].FaultyPartsFlag[Ch] |= (1 << Strobe);
    if (Step == PI_CS_CLK_TYPE) {
      (*RankList) [Rank].faultyParts[Strobe] |= FPT_REC_ENABLE_FAILED;

      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
                      "Failed Cs Clk Pi\n");
    }
  }*/
}


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
PrintSampleArrayCsClk (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Ch,
  UINT8    Dimm,
  UINT8    ReadMode,
  UINT8    SubChannel,
  UINT16   Length,
  struct   ClkTrainingResults (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5/2]
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8   Strobe;
  UINT8   MaxStrobe;
  UINT32  PiSetting;
  struct dimmNvram (*DimmNvList) [MAX_DIMM];
  UINT16  LengthInDwords;
  UINT16  ArrayResultsSize;

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
      RcDebugPrint (SDBG_DEFAULT, "       0   1   2   3   4\n");
    } else {
      RcDebugPrint (SDBG_DEFAULT, "       0   1   2   3\n");
    }
    MaxStrobe = MAX_STROBE_DDR5/4;
  } else {
    RcDebugPrintLine (SDBG_DEFAULT, 48, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
    if (Host->nvram.mem.eccEn) {
      RcDebugPrint (SDBG_DEFAULT, "       0   1   2   3   4   5   6   7   8   9\n");
    } else {
      RcDebugPrint (SDBG_DEFAULT, "       0   1   2   3   4   5   6   7\n");
    }
    MaxStrobe = MAX_STROBE_DDR5/2;
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
} // PrintSampleArrayCsClk



/**

Display Composite Eye

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
DisplayCompositeEyeDDR5 (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  INT16     LeftEdge[SUB_CH],
  INT16     RightEdge[SUB_CH],
  UINT16    EyeWidth[SUB_CH],
  INT16     EyeCenterPoint[SUB_CH],
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
  RcDebugPrint (SDBG_DEFAULT, " %3d", LeftEdge[SubChannel]);


  // Print Composite Right Edge
  RcDebugPrint (SDBG_DEFAULT, "\nComposite Right Edge: ");
  RcDebugPrint (SDBG_DEFAULT, " %3d", RightEdge[SubChannel]);


  // Print Composite Eye Width
  RcDebugPrint (SDBG_DEFAULT, "\nComposite Eye Width: ");
  RcDebugPrint (SDBG_DEFAULT, "  %3d", EyeWidth[SubChannel]);


  // Print Eye Center Point
  RcDebugPrint (SDBG_DEFAULT, "\nEye Center Point:    ");
  RcDebugPrint (SDBG_DEFAULT, " %3d", EyeCenterPoint[SubChannel]);

  RcDebugPrint (SDBG_DEFAULT, "\n\n");

  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK
} // DisplayCompositeEyeDDR5
/**

Prints the edges found

@param Host      - Pointer to sysHost
@param Socket        - Current Socket
@param Ch        - Channel number
@param PulseWidth    - Pointer to save the pulse width
@param FallingEdge   - Pointer to save the falling edge
@param RisingEdge    - Pointer to save the rising edge
@param centerPoint   - Pointer to save the center point
@param MaxStrobe     - Maximum number of strobes

@retval N/A

**/
VOID
DisplayEdgesDDR5 (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT16    PulseWidth[SUB_CH][MAX_STROBE_DDR5/2],
  INT16     LeftEdge[SUB_CH][MAX_STROBE_DDR5/2],
  INT16     RightEdge[SUB_CH][MAX_STROBE_DDR5/2],
  UINT8     SubChannel,
  UINT8     MaxStrobe
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8 Strobe;

  if (!(GetDebugLevel () & SDBG_MAX)) {
    return;
  }

  AcquirePrintControl ();

  RcDebugPrintLine (SDBG_MAX, (MaxStrobe * 4) + 4, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  // Print left edges
  RcDebugPrint (SDBG_DEFAULT, "LE: ");
  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    //
    // Skip if this is an ECC Strobe when ECC is disabled
    //
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, " %3d", LeftEdge[SubChannel][Strobe]);
  } // Strobe loop
  // Print right edges
  RcDebugPrint (SDBG_DEFAULT, "\nRE: ");
  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    //
    // Skip if this is an ECC Strobe when ECC is disabled
    //
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, " %3d", RightEdge[SubChannel][Strobe]);
  } // Strobe loop




  // Print pulse width
  RcDebugPrint (SDBG_DEFAULT, "\nPW: ");
  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    //
    // Skip if this is an ECC Strobe when ECC is disabled
    //
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, " %3d", PulseWidth[SubChannel][Strobe]);
  } // Strobe loop

  RcDebugPrint (SDBG_DEFAULT, "\n\n");

  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK
} // DisplayEdgesDDR5

/**

  Exectues early CS/clock training

  @param Host - Pointer to sysHost

  @retval N/A

**/
UINT32
EarlyCsClkLoopback (
  PSYSHOST  Host
  )
{
  UINT8               Socket;
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               RankPresent;
  UINT8               Signal;
  UINT8               Index;
  UINT8               CsVrefStep;
  UINT8               BestCsVrefStep;
  INT8                CsVrefInitValue;
  INT8                CsVrefValue;
  INT8                MinAverageEyeOffsetVref[MAX_CH][SUB_CH][MAX_RANK_CH][NUM_CS];
  UINT8               SubChannel = 0;
  UINT16              MinAverageEyeOffset;
  UINT16              MinAverageEyeOffsetTemp;
  struct              ClkTrainingResults TrainRes[MAX_CH][SUB_CH][MAX_STROBE_DDR5/2];
  GSM_CSN             SignalToSweep[MAX_CH];
  GSM_CSN             SignalToSweepSubCh[MAX_CH][SUB_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  INT16               Le[MAX_CH][SUB_CH];
  INT16               Re[MAX_CH][SUB_CH];
  INT16               EyeWidthSC[MAX_CH][SUB_CH];
  INT16               EyeCenterPoint[MAX_CH][SUB_CH];
  GSM_CSEDGE_CTL      (*SignalEdgeData)[MAX_CH][SUB_CH][MAX_RANK_CH][NUM_CS][CS_VREF_STEP_COUNT];
  UINT32              Status = SUCCESS;
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  UINT8               SubChannelMask = 0;
  INT8                MinAverageEyeOffsetVrefValue;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  SignalEdgeData = RcAllocatePool (sizeof (*SignalEdgeData));
  if (SignalEdgeData == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return FAILURE;
  }

  Socket = Host->var.mem.currentSocket;

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "EarlyCsClkLoopback Starts\n");
  //
  // Init FinalPiDelay, EyeWidth, TrainRes.
  //
  ZeroMem (TrainRes, sizeof (TrainRes));
  ZeroMem ((UINT8 *) MinAverageEyeOffsetVref, sizeof (MinAverageEyeOffsetVref));

  Index = 0;
  //
  // Init MinAverageEyeOffsetVref, (*SignalEdgeData).
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      for (Rank = 0; Rank < MAX_RANK_CH; Rank++) {
        for (Signal = 0; Signal < NUM_CS; Signal++) {
          for (CsVrefStep = 0; CsVrefStep < CS_VREF_STEP_COUNT; CsVrefStep++) {
            (*SignalEdgeData)[Ch][SubChannel][Rank][Signal][CsVrefStep].toSample = 0;
            (*SignalEdgeData)[Ch][SubChannel][Rank][Signal][CsVrefStep].le = 0;
            (*SignalEdgeData)[Ch][SubChannel][Rank][Signal][CsVrefStep].re = 0;
            (*SignalEdgeData)[Ch][SubChannel][Rank][Signal][CsVrefStep].EyeWidth = 0;
            (*SignalEdgeData)[Ch][SubChannel][Rank][Signal][CsVrefStep].CenterPoint = 0;
            (*SignalEdgeData)[Ch][SubChannel][Rank][Signal][CsVrefStep].EyeOffset = 0xFF;
            (*SignalEdgeData)[Ch][SubChannel][Rank][Signal][CsVrefStep].signal = CS0_N;
          } // CsVrefStep loop
        } //Signal loop
      } //Rank loop
    } //subchannel loop
  }//Ch loop

  // Test every Rank on every Dimm on each Vref.
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    //
    // Do the DDRIO INIT before UDIMM CS CLK training.
    //
    QcsQcaClkDdrioInit (Host, Socket, Dimm);

    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

      OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_EARLY_CS_CLK, (UINT16) ((Socket << 8) | (Dimm << 4) | Rank)));
      RankPresent = 0;
      //
      // Starting by sampling all the SDRAM present
      //
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }

        DimmNvList = GetDimmNvList (Host, Socket, Ch);

        if ((*DimmNvList)[Dimm].dimmPresent == 0) {
          Host->var.mem.piSettingStopFlag[Ch] = 0;
          continue;
        }

        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList)[Dimm].numRanks)) {
          Host->var.mem.piSettingStopFlag[Ch] = 0;
          continue;
        }
        Host->var.mem.piSettingStopFlag[Ch] = GetStrobeMask (Host);
      } // Ch loop


      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if ((*DimmNvList)[Dimm].dimmPresent == 0) {
          continue;
        }
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList)[Dimm].numRanks)) {
          continue;
        }
        //
        // Indicate this Rank is present on at least one channel
        //
        RankPresent = 1;
        break;
      } // Ch loop

      //
      // Continue to the next Rank if this one is not populated
      //
      if (RankPresent == 0) {
        continue;
      }
      //
      // Handle two CS in one sub channle of one DIMM.
      //
      if (Rank >= 2) {
        continue;
      }
      //
      //GSM_CSN   CsToSweep[NUM_CS] = { //DDR5_TODO what's the pin map connection in SPR?
      // CS0_N, CS1_N, CS2_N, CS3_N, CS4_N, CS5_N, CS6_N, CS7_N,
      // Assume CS0--D0 SC0 R0
      //        CS1--D0 SC0 R1
      //        CS2--D0 SC1 R0
      //        CS3--D0 SC1 R1
      //        CS4--D1 SC0 R0
      //        CS5--D1 SC0 R1
      //        CS6--D1 SC1 R0
      //        CS7--D1 SC1 R1
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        SignalToSweep[Ch] = NO_SIGNAL; // initialize to no Signal
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }
        Index = (Dimm * 4 + Rank); //DDR5_TODO: how to determin the cs# number based on the Dimm, Rank, subchannel number.
        SignalToSweep[Ch] = CsToSweep[Index];
        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          Index = CS_PIN_INDEX (Dimm, SubChannel, Rank);
          SignalToSweepSubCh[Ch][SubChannel] = CsToSweep[Index];
        } //subchannel loop
      }//channel loop

      if (FeaturePcdGet (PcdCteBuild) == TRUE) {
        JedecInitSequence (Host, Socket, CH_BITMASK);
      }

      for (CsVrefStep = 0; CsVrefStep < CS_VREF_STEP_COUNT; CsVrefStep++) {
        for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
          if (IsChannelEnabled (Socket, Ch) == FALSE) {
            continue;
          }
          CsVrefInitValue = GetCsVrefInitValue (Host, Socket, Ch, Dimm, Rank);
          CsVrefValue = CsVrefInitValue + (CsVrefStep * CS_VREF_STEP_SIZE);
          SubChannelMask = 0;
          for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
            SubChannelMask = SubChannelMask | (1 << SubChannel);
          } // SubChannel loop

          //
          //Set DRAM CS Vref Value.--DIMM sub channel A and B at the same time.
          //
          WriteVrefCs (Host, Socket, Ch, Dimm, SubChannelMask, Rank, ALL_STROBES, CsVrefValue, TIMING_MULTI_CS, TRUE);
          WriteMPC (Host, Socket, Ch, Dimm, SubChannelMask, Rank, APPLY_VREFCA_RTT_CA_CS_CK, TIMING_4N, TRUE);
          RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
            "CsVrefValue 0x%x \n", CsVrefValue);
          //
          //Set DRAM CS Training Mode.--DIMM sub channel A and B at the same time.
          //
          WriteMPC(Host, Socket, Ch, Dimm, SubChannelMask, Rank, Enter_CS_Training_Mode, TIMING_4N, TRUE);
        }
        // SignalToSweepSubCh is one Signal input, but will convert it to two signals in two subchannels as the training is going in two sub channels in parrael.
        // SignalToSweepSubCh means the Host structure SignalToSweepSubCh address
        //
        // Clear the training results
        //
        ZeroMem (TrainRes, sizeof (TrainRes));
        //
        // Get the composite eye results of the Signal.
        //
        GetCsMarginsSweep (Host, Socket, Dimm, Rank, SignalToSweepSubCh, Le, Re, EyeWidthSC, EyeCenterPoint, CsVrefValue, &TrainRes);
        for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
          if (IsChannelEnabled (Socket, Ch) == FALSE) {
            continue;
          }
          if (SignalToSweep[Ch] == NO_SIGNAL) {
            continue;
          }

          for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

            RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
              "Signal %a,  left %d,  right %d, CA Vref Value %d,  eye width %d, CS delay value %d\n",
              GetSignalStrDdr5 (SignalToSweepSubCh[Ch][SubChannel]), Le[Ch][SubChannel], Re[Ch][SubChannel],
              CsVrefValue, EyeWidthSC[Ch][SubChannel], EyeCenterPoint[Ch][SubChannel]);

            RankList = GetRankNvList (Host, Socket, Ch, Dimm);
            //
            // Convert results from absolute pi values to offset values and save the results.
            //
            Index = 0;
            Index = CS_PIN_INDEX (Dimm, SubChannel, Rank);
            (*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][CsVrefStep].signal = SignalToSweepSubCh[Ch][SubChannel];
            (*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][CsVrefStep].le = Le[Ch][SubChannel];
            (*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][CsVrefStep].re = Re[Ch][SubChannel];
            (*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][CsVrefStep].EyeWidth = EyeWidthSC[Ch][SubChannel];
            (*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][CsVrefStep].CenterPoint = EyeCenterPoint[Ch][SubChannel];
            (*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][CsVrefStep].toSample = 1;
            (*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][CsVrefStep].EyeOffset = ABS (CS_FULL_EYE_WIDTH - EyeWidthSC[Ch][SubChannel]);
          } //SubChannel Loop
        } // Ch loop
      } // CsVrefStep loop
    } //Rank loop
    //Restore Normal Operating mode after sweeping CS for DIMM.
    if (FeaturePcdGet (PcdCteBuild) == TRUE) {
      JedecInitSequence (Host, Socket, CH_BITMASK);
    }
  }//Dimm loop
  //
  // Find the Minimum average eye offset by going through each vref training result.
  //
  RcDebugPrint (SDBG_MAX, "\nSummary: \n");
  for (Index = 0; Index < NUM_CS; Index++) {
    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if (IsChannelEnabled (Socket, Ch) == FALSE) continue;
      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        //DDR5_TODO--may need to check whether sub channel is enabled or not.
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
          if ((*DimmNvList)[Dimm].dimmPresent == 0) {
            continue;
          }
          RankList = GetRankNvList (Host, Socket, Ch, Dimm);
          for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
            if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
              continue;
            }
            if ((*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][0].toSample == 0) {
              continue;
            }
            MinAverageEyeOffsetVref[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index] = 0;
            MinAverageEyeOffset = MAX_UINT16;
            CsVrefInitValue = GetCsVrefInitValue (Host, Socket, Ch, Dimm, Rank);
            for (CsVrefStep = 0; CsVrefStep < CS_VREF_STEP_COUNT; CsVrefStep++) {
              CsVrefValue = CsVrefInitValue + (CsVrefStep * CS_VREF_STEP_SIZE);
              if ((*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][CsVrefStep].toSample == 0) {
                continue;
              }
              if (CS_VREF_STEP_COUNT == 1) {
                MinAverageEyeOffsetTemp = ((*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][CsVrefStep].EyeOffset +
                                           (*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][CsVrefStep].EyeOffset +
                                           (*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][CsVrefStep].EyeOffset);
              } else if (CsVrefStep == 0) {
                MinAverageEyeOffsetTemp = ((*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][CsVrefStep].EyeOffset +
                                           (*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][CsVrefStep].EyeOffset +
                                           (*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][CsVrefStep + 1].EyeOffset);
              } else if ((CsVrefStep + 1) < CS_VREF_STEP_COUNT) {
                MinAverageEyeOffsetTemp = ((*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][CsVrefStep - 1].EyeOffset +
                                           (*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][CsVrefStep].EyeOffset +
                                           (*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][CsVrefStep + 1].EyeOffset);
              } else {
                MinAverageEyeOffsetTemp = ((*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][CsVrefStep - 1].EyeOffset +
                                           (*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][CsVrefStep].EyeOffset +
                                           (*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][CsVrefStep].EyeOffset);
              }
              RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
                "Signal %a, CA Vref Value %3d,  eye width %3d,  eye offset %3d, Min Sum Eye offset %3d, left %3d, right %3d, PI delay %3d \n",
                GetSignalStrDdr5 ((*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][0].signal),
                CsVrefValue,
                (*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][CsVrefStep].EyeWidth,
                (*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][CsVrefStep].EyeOffset,
                MinAverageEyeOffsetTemp,
                (*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][CsVrefStep].le,
                (*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][CsVrefStep].re,
                (*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][CsVrefStep].CenterPoint);

              if (MinAverageEyeOffsetTemp < MinAverageEyeOffset) {
                MinAverageEyeOffset = MinAverageEyeOffsetTemp;
                MinAverageEyeOffsetVref[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index] = CsVrefValue;
              }
            } // CsVrefStep loop
            MinAverageEyeOffsetVrefValue = MinAverageEyeOffsetVref[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index];
            BestCsVrefStep = (MinAverageEyeOffsetVrefValue - CsVrefInitValue) / CS_VREF_STEP_SIZE;
            RcDebugPrint (SDBG_MAX, "Final Setting: \n");
            RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
              "Signal %a: CA Vref Value %3d,  eye width %3d,  Min Sum Eye offset %3d, left %3d, right %3d, PI delay %3d\n",
              GetSignalStrDdr5 ((*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][0].signal),
              MinAverageEyeOffsetVref[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index],
              (*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][BestCsVrefStep].EyeWidth,
              MinAverageEyeOffset,
              (*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][BestCsVrefStep].le,
              (*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][BestCsVrefStep].re,
              (*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][BestCsVrefStep].CenterPoint);
            //
            // Apply minimum eye width check
            //
            if ((*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][BestCsVrefStep].EyeWidth < MinimumMargin (Host, CS)) {
              RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
                "Signal %a: Best Eye Width %3d is smaller than %3d, disable channel\n",
                GetSignalStrDdr5 ((*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][0].signal),
                (*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][BestCsVrefStep].EyeWidth,
                MinimumMargin (Host, CS));
              EwlOutputType3 (WARN_MEMORY_TRAINING, WARN_CS_CLK_LOOPBACK_TRAINING, Socket, Ch, Dimm, Rank, EwlSeverityWarning, MrcGtDelim, DdrLevel, (*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][0].signal, (UINT8) ((*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][BestCsVrefStep].EyeWidth));
              DisableChannelSw (Host, Socket, Ch);
            } else {
              //
              // Write PI delay
              //
              Status = GetSetSignalDdr5 (Host, Socket, Ch, DdrLevel, (*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][BestCsVrefStep].signal, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &((*SignalEdgeData)[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index][BestCsVrefStep].CenterPoint));
              //
              //Set Best DRAM CS Vref Value
              //
              SubChannelMask = (1 << SubChannel);
              WriteVrefCs (Host, Socket, Ch, Dimm, SubChannelMask, Rank, ALL_STROBES,
                MinAverageEyeOffsetVref[Ch][SubChannel][(*RankList)[Rank].rankIndex][Index], TIMING_MULTI_CS, TRUE); //DDR5_TODO. in future, it can support program both subchannel 0 and subchannel 1 at the same time.
              WriteMPC (Host, Socket, Ch, Dimm, SubChannelMask, Rank, APPLY_VREFCA_RTT_CA_CS_CK, TIMING_4N, TRUE);
            }
            RcDebugPrint (SDBG_MAX, "\n");
          } //Rank loop
        } //Dimm loop
      } //sub channel loop
    } //Ch loop
  } //Signal Index loop
  RcDebugPrint (SDBG_MAX, "\n");

  if (GetDebugLevel () & SDBG_MAX) {
    DisplayCCCResults (Host, Socket);
  }

  FifoTrainReset (Host, Socket);
  RcFreePool (SignalEdgeData);
  return Status;
} // EarlyCsClkLoopback

/*

 Returns an index to access a backside CS signal in the CsToSweepBackSide[] array

 @param[in]   Channel     Current Channel under test (0-based)
 @param[in]   SubChannel  Current SubChannel under test (0-based)
 @param[in]   Dimm        Current Dimm under test (0-based)
 @param[in]   Rank        Current Rank under test (0-based)

 @return      An index value to be used to access an element in the CsToSweepBackSide[] array

*/
UINT8
GetBacksideCsSignalIndex (
  IN        UINT8     Channel,
  IN        UINT8     SubChannel,
  IN        UINT8     Dimm,
  IN        UINT8     Rank
  )
{
  if (SubChannel == NO_SUBCH) {
    return (Dimm * 4 + Rank); // This can only return Sub-ChA CS#.
  } else {
    return CS_PIN_INDEX (Dimm, SubChannel, Rank);
  }
}

/**

  Save the QCS trained value for CTE

  @param[in] Host                - Pointer to sysHost
  @param[in] Socket              - Socket Number
  @param[in] Ch                  - Ch Number
  @param[in] Dimm                - Dimm Number
  @param[in] Rank                - Rank Number
  @param[in] FinalCenterPoint    - Qcs Trained Final Center Point Value

  @retval NA

**/
VOID
CteSaveQcsTrainedValue (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN INT16     FinalCenterPoint
  )
{
  struct dimmNvram  (*DimmNvList) [MAX_DIMM];
  UINT8             SubChannel = 0;
  CHAR8             *CteOptionQaCh0Sc0[4] = {"ch0q0acs0_trained_value", "ch0q0acs1_trained_value", "ch0q0acs2_trained_value", "ch0q0acs3_trained_value"};
  CHAR8             *CteOptionQaCh0Sc1[4] = {"ch0q1acs0_trained_value", "ch0q1acs1_trained_value", "ch0q1acs2_trained_value", "ch0q1acs3_trained_value"};
  CHAR8             *CteOptionQaCh1Sc0[4] = {"ch1q0acs0_trained_value", "ch1q0acs1_trained_value", "ch1q0acs2_trained_value", "ch1q0acs3_trained_value"};
  CHAR8             *CteOptionQaCh1Sc1[4] = {"ch1q1acs0_trained_value", "ch1q1acs1_trained_value", "ch1q1acs2_trained_value", "ch1q1acs3_trained_value"};
  CHAR8             *CteOptionQbCh0Sc0[4] = {"ch0q0bcs0_trained_value", "ch0q0bcs1_trained_value", "ch0q0bcs2_trained_value", "ch0q0bcs3_trained_value"};
  CHAR8             *CteOptionQbCh0Sc1[4] = {"ch0q1bcs0_trained_value", "ch0q1bcs1_trained_value", "ch0q1bcs2_trained_value", "ch0q1bcs3_trained_value"};
  CHAR8             *CteOptionQbCh1Sc0[4] = {"ch1q0bcs0_trained_value", "ch1q0bcs1_trained_value", "ch1q0bcs2_trained_value", "ch1q0bcs3_trained_value"};
  CHAR8             *CteOptionQbCh1Sc1[4] = {"ch1q1bcs0_trained_value", "ch1q1bcs1_trained_value", "ch1q1bcs2_trained_value", "ch1q1bcs3_trained_value"};
  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  if (Ch == 0) {
    if (SubChannel == 0) {
      CteSetOptions (CteOptionQaCh0Sc0[ (Dimm << 1) + Rank], NO_CARE_PARAMETER, NO_CARE_PARAMETER, NO_CARE_PARAMETER, FinalCenterPoint);
    } else {
      CteSetOptions (CteOptionQaCh0Sc1[ (Dimm << 1) + Rank], NO_CARE_PARAMETER, NO_CARE_PARAMETER, NO_CARE_PARAMETER, FinalCenterPoint);
    }
  } else {
    if (SubChannel == 0) {
      CteSetOptions (CteOptionQaCh1Sc0[ (Dimm << 1) + Rank], NO_CARE_PARAMETER, NO_CARE_PARAMETER, NO_CARE_PARAMETER, FinalCenterPoint);
    } else {
      CteSetOptions (CteOptionQaCh1Sc1[ (Dimm << 1) + Rank], NO_CARE_PARAMETER, NO_CARE_PARAMETER, NO_CARE_PARAMETER, FinalCenterPoint);
    }
  }
  if ((*DimmNvList) [Dimm].x4Present) {
    if (Ch == 0) {
      if (SubChannel == 0) {
        CteSetOptions (CteOptionQbCh0Sc0[ (Dimm << 1) + Rank], NO_CARE_PARAMETER, NO_CARE_PARAMETER, NO_CARE_PARAMETER, FinalCenterPoint);
      } else {
        CteSetOptions (CteOptionQbCh0Sc1[ (Dimm << 1) + Rank], NO_CARE_PARAMETER, NO_CARE_PARAMETER, NO_CARE_PARAMETER, FinalCenterPoint);
      }
    } else {
      if (SubChannel == 0) {
        CteSetOptions (CteOptionQbCh1Sc0[ (Dimm << 1) + Rank], NO_CARE_PARAMETER, NO_CARE_PARAMETER, NO_CARE_PARAMETER, FinalCenterPoint);
      } else {
        CteSetOptions (CteOptionQbCh1Sc1[ (Dimm << 1) + Rank], NO_CARE_PARAMETER, NO_CARE_PARAMETER, NO_CARE_PARAMETER, FinalCenterPoint);
      }
    }
  }

  return;
} // CteSaveQcsTrainedValue

/**

Exectues early backside CS/clock training.

@param Host - Pointer to sysHost

@retval N/A

**/
UINT32
EarlyCsClkBacksideLoopback (
  PSYSHOST  Host
  )
{
  UINT8               Socket;
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               MaxRank;
  UINT8               RankPresent;
  UINT8               Signal;
  UINT8               Index = 0;
  UINT8               CsVrefIndex = 0;
  INT8                CsVrefValue = 0;
  INT8                FinalCsVrefIndex = 0;
  UINT8               SubChannel = 0;
  UINT16              MinAverageEyeOffset = 0;
  UINT16              MinAverageEyeOffsetTemp = 0;
  struct              ClkTrainingResults TrainRes[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  GSM_CSN             SignalToSweep[MAX_CH];
  GSM_CSN             SignalToSweepSubCh[MAX_CH][SUB_CH];
  struct channelNvram (*ChannelNvList) [MAX_CH];
  struct dimmNvram    (*DimmNvList) [MAX_DIMM];
  INT16               Le[MAX_CH][SUB_CH][COPY_NUM];
  INT16               Re[MAX_CH][SUB_CH][COPY_NUM];
  INT16               EyeWidthSC[MAX_CH][SUB_CH][COPY_NUM];
  INT16               EyeCenterPoint[MAX_CH][SUB_CH][COPY_NUM];
  GSM_CSN             SignalGsmCsn;
  INT16               FinalLeftEdge;
  INT16               FinalRightEdge;
  UINT16              FinalEyeWidth;
  INT16               FinalCenterPoint;
  GSM_CSEDGE_CTL      (*SignalEdgeData) [MAX_CH][SUB_CH][MAX_RANK_CH][NUM_CS][COPY_NUM][QCS_VREF_STEP_COUNT];
  UINT32              Status = SUCCESS;
  struct ddrRank      (*RankList) [MAX_RANK_DIMM];
  UINT8               SubChannelMask = 0;
  UINT8               MaxChDdr;
  UINT8               BacksideCopy;
  INT8                QcsVrefValue;

  MaxChDdr = GetMaxChDdr ();
  SignalEdgeData = RcAllocatePool (sizeof (*SignalEdgeData));
  if (SignalEdgeData == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return FAILURE;
  }

  Socket = Host->var.mem.currentSocket;
  ChannelNvList = GetChannelNvList (Host, Socket);

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "EarlyCsClkBacksideLoopback Starts\n");
  //
  // Init FinalPiDelay, EyeWidth, TrainRes.
  //
  ZeroMem (TrainRes, sizeof (TrainRes));

  Emulation1nCmdTimingWorkaround (Host, Socket);

  //
  // Enter sense amp training mode to all the channels here to latch results.
  //
  SetTrainingModeLatchResult (Host, Socket, CHECKPOINT_MINOR_SENSE_AMP);
  //
  // Init (*SignalEdgeData).
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      for (Rank = 0; Rank < MAX_RANK_CH; Rank++) {
        for (Signal = 0; Signal < NUM_CS; Signal++) {
          for (BacksideCopy = 0; BacksideCopy < COPY_NUM; BacksideCopy++) {
            for (CsVrefIndex = 0; CsVrefIndex < QCS_VREF_STEP_COUNT; CsVrefIndex ++) {
              (*SignalEdgeData) [Ch][SubChannel][Rank][Signal][BacksideCopy][CsVrefIndex].toSample = 0;
              (*SignalEdgeData) [Ch][SubChannel][Rank][Signal][BacksideCopy][CsVrefIndex].le = 0;
              (*SignalEdgeData) [Ch][SubChannel][Rank][Signal][BacksideCopy][CsVrefIndex].re = 0;
              (*SignalEdgeData) [Ch][SubChannel][Rank][Signal][BacksideCopy][CsVrefIndex].EyeWidth = 0;
              (*SignalEdgeData) [Ch][SubChannel][Rank][Signal][BacksideCopy][CsVrefIndex].CenterPoint = 0;
              (*SignalEdgeData) [Ch][SubChannel][Rank][Signal][BacksideCopy][CsVrefIndex].EyeOffset = 0xFF;
              (*SignalEdgeData) [Ch][SubChannel][Rank][Signal][BacksideCopy][CsVrefIndex].signal = CS0_N;
            }
          }
        } //Signal loop
      } //Rank loop
    } //subchannel loop
  }//Ch loop

  //
  // Test every Rank on every Dimm on each Vref.
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    //
    // Do the DDRIO INIT before starting QCS CLK training.
    //
    QcsQcaClkDdrioInit (Host, Socket, Dimm);
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

      OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_EARLY_CS_CLK_BACKSIDE, (UINT16) ((Socket << 8) | (Dimm << 4) | Rank)));
      RankPresent = 0;
      //
      // Starting by sampling all the SDRAM present
      //
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }

        DimmNvList = GetDimmNvList (Host, Socket, Ch);

        if ((*DimmNvList) [Dimm].dimmPresent == 0) {
          Host->var.mem.piSettingStopFlag[Ch] = 0;
          continue;
        }

        if (((*DimmNvList) [Dimm].DcpmmPresent) && ((*DimmNvList) [Dimm].fmcType == FMC_CWV_TYPE)) {
          continue;
        }

        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          Host->var.mem.piSettingStopFlag[Ch] = 0;
          continue;
        }
        Host->var.mem.piSettingStopFlag[Ch] = GetStrobeMask (Host);
      } // Ch loop


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
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }
        //
        // Indicate this Rank is present on at least one channel
        //
        RankPresent = 1;
        break;
      } // Ch loop

      //
      // Continue to the next Rank if this one is not populated
      //
      if (RankPresent == 0) {
        continue;
      }
      //
      // Handle two CS in one sub channle of one DIMM.
      //
      if (Rank >= 2) {
        continue;
      }

      //
      // QCS0_A_N, QCS1_A_N,  // D0 R0 SUB CH A; D0 R1 SUB CH A
      // QCS0_B_N, QCS1_B_N,  // D0 R0 SUB CH B; D0 R1 SUB CH B
      // QCS2_A_N, QCS3_A_N,  // D1 R0 SUB CH A; D1 R1 SUB CH A
      // QCS2_B_N, QCS3_B_N,  // D1 R0 SUB CH B; D1 R1 SUB CH B
      //
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        SignalToSweep[Ch] = NO_SIGNAL; // initialize to no Signal
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }
        Index = GetBacksideCsSignalIndex (Ch, NO_SUBCH, Dimm, Rank);
        SignalToSweep[Ch] = CsToSweepBackSide[Index];

        //
        // Init DRAM interface
        //
        InitDramInterface (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, Rank);

        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          Index = GetBacksideCsSignalIndex (Ch, SubChannel, Dimm, Rank);

          SignalToSweepSubCh[Ch][SubChannel] = CsToSweepBackSide[Index];
        } //subchannel loop
      }//channel loop
      FixedDelayMicroSecond (10);
      ProgramBacksideOdt (Host, Socket);
      BacksideDramInit (Host, Socket, CH_BITMASK);

      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "Start CsVrefValue sweep \n");
      for (CsVrefIndex = 0; CsVrefIndex < QCS_VREF_STEP_COUNT; CsVrefIndex ++) {
        //
        // Power down mode is disabled by default. So, no code here to disable. Make sure it's disabled after jedec reset.  DDR5_TODO.
        //
        for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
          if (IsChannelEnabled (Socket, Ch) == FALSE) {
            continue;
          }

          MaxRank = GetNumberOfRanksOnDimm (Socket, Ch, Dimm);
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= MaxRank)) {
            continue;
          }

          EnterRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);

          for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
            SubChannelMask = (1 << SubChannel);
            if ((*ChannelNvList) [Ch].timingMode == TIMING_1N) {
              WriteMPC (Host, Socket, Ch, Dimm, SubChannelMask, Rank, SET_1N_CMD_TIMING, TIMING_4N, TRUE);
            }

            //
            //Set DRAM CS Vref Value.--DIMM sub channel A and B at the same time.
            //
            GetQcsVrefValueOdt (Socket, Ch, Dimm, &QcsVrefValue);
            CsVrefValue = QcsVrefValue + (CsVrefIndex + QCS_VREF_STEP_START) * QCS_VREF_STEP_SIZE;
            WriteVrefCs (Host, Socket, Ch, Dimm, SubChannelMask, Rank, ALL_STROBES, CsVrefValue, TIMING_MULTI_CS, TRUE);
            WriteMPC (Host, Socket, Ch, Dimm, SubChannelMask, Rank, APPLY_VREFCA_RTT_CA_CS_CK, TIMING_4N, TRUE);
            RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
              "CsVrefValue = 0x%x \n", CsVrefValue);

            //
            //Set DRAM CS Training Mode.--DIMM sub channel A and B at the same time.
            //
            WriteMPC (Host, Socket, Ch, Dimm, SubChannelMask, Rank, Enter_CS_Training_Mode, TIMING_4N, TRUE);
          }

          ExitRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);
        } //Ch loop
        // SignalToSweepSubCh is one Signal input, but will convert it to two signals in two subchannels as the training is going in two sub channels in parrael.
        // SignalToSweepSubCh means the Host structure SignalToSweepSubCh address
        //
        // Clear the training results
        //
        ZeroMem (TrainRes, sizeof (TrainRes));
        //
        // Get the composite eye results of the Signal.
        //
        GetCsBacksideMarginsSweep (Host, Socket, Dimm, Rank, SignalToSweepSubCh, Le, Re, EyeWidthSC, EyeCenterPoint, CsVrefValue, &TrainRes);
        for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
          if (IsChannelEnabled (Socket, Ch) == FALSE) {
            continue;
          }
          if (SignalToSweep[Ch] == NO_SIGNAL) {
            continue;
          }
          MaxRank = GetNumberOfRanksOnDimm (Socket, Ch, Dimm);
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= MaxRank)) {
            continue;
          }

          for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

            for (BacksideCopy = 0; BacksideCopy < GetMaxBacksideCopyNum (Socket, Ch, Dimm); BacksideCopy++) {

              RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, NO_RANK, NO_STROBE, NO_BIT,
                "Signal %a, left %d,  right %d, CS Vref Value 0x%x,  eye width %d, CS delay value %d\n",
                GetQcsSignalStrDdr5 (Rank, BacksideCopy), Le[Ch][SubChannel][BacksideCopy], Re[Ch][SubChannel][BacksideCopy],
                CsVrefValue, EyeWidthSC[Ch][SubChannel][BacksideCopy], EyeCenterPoint[Ch][SubChannel][BacksideCopy]);

              RankList = GetRankNvList (Host, Socket, Ch, Dimm);
              //
              // Convert results from absolute pi values to offset values and save the results.
              //
              Index = 0;
              Index = GetBacksideCsSignalIndex (Ch, SubChannel, Dimm, Rank);

              (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][CsVrefIndex].signal = SignalToSweepSubCh[Ch][SubChannel];
              (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][CsVrefIndex].le = Le[Ch][SubChannel][BacksideCopy];
              (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][CsVrefIndex].re = Re[Ch][SubChannel][BacksideCopy];
              (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][CsVrefIndex].EyeWidth = EyeWidthSC[Ch][SubChannel][BacksideCopy];
              (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][CsVrefIndex].CenterPoint = EyeCenterPoint[Ch][SubChannel][BacksideCopy];
              (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][CsVrefIndex].toSample = 1;
              (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][CsVrefIndex].EyeOffset = ABS (BACKSIDE_CS_FULL_EYE_WIDTH - EyeWidthSC[Ch][SubChannel][BacksideCopy]);
            }
          } //SubChannel Loop

          RcDebugPrintWithDevice (
            SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
            "Disable on Rank QCS training %d\n",
            Rank
            );

          //
          // Exit DRAM CS Training mode.
          //
          WriteMPC (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, Rank, Exit_CS_Training_Mode, TIMING_4N, TRUE);
        } // Channel loop
      } // CsVrefIndex loop

      //Restore Normal Operating mode after sweeping CS for DIMM.
      if (FeaturePcdGet (PcdCteBuild) == TRUE) {
        JedecInitSequence (Host, Socket, CH_BITMASK);
      }
    } // Rank loop
  } // Dimm loop

  //
  // Find the Minimum eye offset sum by going through each vref training result.
  //
  RcDebugPrint (SDBG_MAX, "\nSummary: \n");
  for (Index = 0; Index < NUM_CS; Index++) {
    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if (IsChannelEnabled (Socket, Ch) == FALSE) continue;
      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
          if ((*DimmNvList) [Dimm].dimmPresent == 0) {
            continue;
          }
          if (((*DimmNvList) [Dimm].DcpmmPresent) && ((*DimmNvList) [Dimm].fmcType == FMC_CWV_TYPE)) {
            continue;
          }
          RankList = GetRankNvList (Host, Socket, Ch, Dimm);
          for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
            if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
              continue;
            }

            for (BacksideCopy = 0; BacksideCopy < GetMaxBacksideCopyNum (Socket, Ch, Dimm); BacksideCopy++) {

              if ((*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][0].toSample == 0) {
                continue;
              }

              MinAverageEyeOffset = MAX_UINT16;
              for (CsVrefIndex = 0; CsVrefIndex < QCS_VREF_STEP_COUNT; CsVrefIndex ++) {
                if ((*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][CsVrefIndex].toSample == 0) {
                  continue;
                }

                //
                // The strategy of MinAverageEye determination is taking the previous and next into account it supposed to help filter out anomalous results.
                // If one step had a very small eyeoffset due to some transient issue, it wouldn't necessarily get chosen if the steps before and after were large
                // If step loop is 0 -> pick itself
                // Else
                //     For 1st setting:      sum of (Index,   Index, Index+1)
                //     For 2nd to (N-1)th:   sum of (Index-1, Index, Index+1)
                //     For Nth setting:      sum of (Index-1, Index, Index  )
                //
                if (QCS_VREF_STEP_COUNT == 1) {
                  MinAverageEyeOffsetTemp = ((*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][CsVrefIndex].EyeOffset +
                                             (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][CsVrefIndex].EyeOffset +
                                             (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][CsVrefIndex].EyeOffset);
                } else if (CsVrefIndex == 0) {
                  MinAverageEyeOffsetTemp = ((*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][CsVrefIndex].EyeOffset +
                                             (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][CsVrefIndex].EyeOffset +
                                             (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][CsVrefIndex + 1].EyeOffset);
                } else if (CsVrefIndex + 1 < QCS_VREF_STEP_COUNT) {
                  MinAverageEyeOffsetTemp = ((*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][CsVrefIndex - 1].EyeOffset +
                                             (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][CsVrefIndex].EyeOffset +
                                             (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][CsVrefIndex + 1].EyeOffset);
                } else {
                  MinAverageEyeOffsetTemp = ((*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][CsVrefIndex - 1].EyeOffset +
                                             (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][CsVrefIndex].EyeOffset +
                                             (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][CsVrefIndex].EyeOffset);
                }

                GetQcsVrefValueOdt (Socket, Ch, Dimm, &QcsVrefValue);
                CsVrefValue = (INT8) QcsVrefValue + (CsVrefIndex + QCS_VREF_STEP_START) * QCS_VREF_STEP_SIZE;
                RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, NO_RANK, NO_STROBE, NO_BIT,
                  "Signal %a, CS Vref Value 0x%x,  eye width %3d,  eye offset %3d, Min Sum Eye offset %3d, left %3d, right %3d, PI delay %3d \n",
                  GetQcsSignalStrDdr5 (Rank, BacksideCopy),
                  CsVrefValue,
                  (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][CsVrefIndex].EyeWidth,
                  (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][CsVrefIndex].EyeOffset,
                  MinAverageEyeOffsetTemp,
                  (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][CsVrefIndex].le,
                  (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][CsVrefIndex].re,
                  (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][CsVrefIndex].CenterPoint);

                if (MinAverageEyeOffsetTemp < MinAverageEyeOffset) {
                  MinAverageEyeOffset = MinAverageEyeOffsetTemp;
                  FinalCsVrefIndex = CsVrefIndex;
                }
              }

              SignalGsmCsn = (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][0].signal;
              FinalEyeWidth = (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][FinalCsVrefIndex].EyeWidth;
              FinalLeftEdge = (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][FinalCsVrefIndex].le;
              FinalRightEdge = (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][FinalCsVrefIndex].re;
              FinalCenterPoint = (*SignalEdgeData) [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Index][BacksideCopy][FinalCsVrefIndex].CenterPoint;
              RcDebugPrint (SDBG_MAX, "Final Setting: \n");

              GetQcsVrefValueOdt (Socket, Ch, Dimm, &QcsVrefValue);
              CsVrefValue = QcsVrefValue + (FinalCsVrefIndex + QCS_VREF_STEP_START) * QCS_VREF_STEP_SIZE;
              RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, NO_RANK, NO_STROBE, NO_BIT,
                "Signal %a: CS Vref Value 0x%x,  eye width %3d,  Min Sum Eye offset %3d, left %3d, right %3d, PI delay %3d\n",
                GetQcsSignalStrDdr5 (Rank, BacksideCopy),
                CsVrefValue,
                FinalEyeWidth,
                MinAverageEyeOffset,
                FinalLeftEdge,
                FinalRightEdge,
                FinalCenterPoint);

              //
              // Apply minimum eye width check
              //
              if (FinalEyeWidth < MinimumMargin (Host, QCS)) {
                RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, NO_RANK, NO_STROBE, NO_BIT,
                  "Signal %a: Best Eye Width %3d is smaller than %3d, disable channel\n", GetQcsSignalStrDdr5 (Rank, BacksideCopy), FinalEyeWidth, MinimumMargin (Host, QCS));
                EwlOutputType3 (WARN_MEMORY_TRAINING, WARN_CS_CLK_LOOPBACK_TRAINING, Socket, Ch, Dimm, Rank, EwlSeverityWarning, MrcGtDelim, DdrLevel, SignalGsmCsn, (UINT8) FinalEyeWidth);
                DisableChannelSw (Host, Socket, Ch);
              } else {
                //
                // Write QCS delay
                //
                GetSetQcsDelay (Host, Socket, Ch, Dimm, Rank, SignalGsmCsn, BacksideCopy, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &FinalCenterPoint, NULL, NULL);
                SubChannelMask = (1 << SubChannel);
                if (FeaturePcdGet (PcdCteBuild) == TRUE) {
                  CteSaveQcsTrainedValue (Host, Socket, Ch, Dimm, Rank, FinalCenterPoint);
                }

                //
                // Set Best DRAM CS Vref Value. Should use PDA to set per-DRAM CS Vref.
                //
                WriteVrefCs (Host, Socket, Ch, Dimm, SubChannelMask, Rank, ALL_STROBES, CsVrefValue, TIMING_MULTI_CS, TRUE);
                WriteMPC (Host, Socket, Ch, Dimm, SubChannelMask, Rank, APPLY_VREFCA_RTT_CA_CS_CK, TIMING_4N, TRUE);
              }
              RcDebugPrint (SDBG_MAX, "\n");
            } // Copy loop
          } // Rank loop
        } // Dimm loop
      } // SubChannel loop
    } // Channel loop
  } // Signal Index loop

  RcDebugPrint (SDBG_MAX, "\n");
  for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
    if ((*ChannelNvList) [Ch].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList) [Dimm].dimmPresent == 0) {
        continue;
      }
      if (((*DimmNvList) [Dimm].DcpmmPresent) && ((*DimmNvList) [Dimm].fmcType == FMC_CWV_TYPE)) {
        continue;
      }
      RankList = GetRankNvList (Host, Socket, Ch, Dimm);
      for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        //
        // Exit DRAM CS Training mode.
        //
        WriteMPC (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, Rank, Exit_CS_Training_Mode, TIMING_4N, TRUE);
      } // Rank loop
    } // Dimm loop
  } // Channel loop

  //
  // Exit sense amp training mode
  //
  SetTrainingModeLatchResult (Host, Socket, CHECKPOINT_MINOR_DISABLE_SENSE_AMP);
  if (GetDebugLevel () & SDBG_MAX) {
    DisplayBacksideCCCResults (Host, Socket);
  }

  FifoTrainReset(Host, Socket);//DDRT2_TODO.
  RcFreePool (SignalEdgeData);
  return Status;
} // EarlyCsClkBacksideLoopback

/**

Checks if a given DCS needs to be trained for a given SubCh, Dimm, Rank

@param[in] Signal      - Signal
@param[in] SubChMask   - bit map sub channel mask
@param[in] Dimm        - DIMM number
@param[in] Rank        - Rank number

@retval TRUE: signal needs to be trained for this configuration
        FALSE: signal does not need to be trained
**/
BOOLEAN
DcsSignalAppliesToThisDimmRank (
  IN GSM_CSN   Signal,
  IN UINT8     SubChMask,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
  UINT8   Index;
  UINT8   SubChIndex;
  BOOLEAN SignalApplies = FALSE;

  for (SubChIndex = 0; SubChIndex < SUB_CH; SubChIndex++) {
    if ((SubChMask & (1 << SubChIndex)) == 0) {
      continue;
    }
    Index = CS_PIN_INDEX (Dimm, SubChIndex, Rank);
    if (Signal == CsToSweep[Index]) {
      SignalApplies = TRUE;
    }
  }

  return SignalApplies;
}

/**

Finds CS margins for the given Signal

@param[in] Host         - Pointer to sysHost
@param[in] Socket       - Socket number
@param[in] Dimm         - DIMM number
@param[in] Subch        - sub channel
@param[in] Rank         - Rank number
@param[in] Signal       - Signal
@param[in] *Le          - Left edge
@param[in] *Re          - Right edge
@param[in] CsVrefStep   - DCS VREF Training Step
@param[in] TrainingMode - Serial or Parallel subchannel training mode

@retval N/A

**/
VOID
GetCsMarginsRcdSweep (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Dimm,
  IN UINT8     Subch,
  IN UINT8     Rank,
  IN GSM_CSN   Signal,
  IN INT16     Le[MAX_CH][SUB_CH],
  IN INT16     Re[MAX_CH][SUB_CH],
  IN UINT8     CsVrefStep,
  IN UINT8     TrainingMode
  )
{
  UINT8               Ch;
  INT16               PiIndex;
  UINT16              i;
  UINT8               AlertFeedback[MAX_CH][SUB_CH][CS_CLK_TEST_RANGE];
  INT16               OrgDelay[MAX_CH][SUB_CH];
  UINT32              RankMask[MAX_CH];
  UINT8               AlertCentering[MAX_CH];
  UINT32              ChBitmask = 0;
  struct cmdEye       CurEye[MAX_CH][SUB_CH];
  UINT16              PiPosition[MAX_CH];
  UINT32              Status = 0;
  UINT32              Err = 0;
  UINT16              EyeWidth;
  MRC_MST             MemSsType;
  UINT32              ChannelMask = 0;
  UINT32              ValidMask = 0;
  UINT8               SubChMask = 0;
  UINT8               SubChTemp = 0;
  UINT8               CsSubChGap = 0;
  GSM_CSN             SignalLocal;
  UINT8               MaxChDdr;
  UINT8               InvertFlag[SUB_CH];
  INT8                CsVrefInitValue;

  MaxChDdr = GetMaxChDdr ();
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "GetCsMarginsRcdSweep starts\n");
  if (TrainingMode == RUN_IN_PARALLEL) {
    SubChMask = TWO_SUBCH_MASK;
    CsSubChGap = CS0_B_N - CS0_A_N;
  } else {
    SubChMask = 1 << Subch;
    CsSubChGap = 0;
  }
  ZeroMem ((UINT8 *) AlertCentering, sizeof (AlertCentering));
  ZeroMem ((UINT8 *) InvertFlag, sizeof (InvertFlag));
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

  if (UseSmbusMrw (Host, Socket)) {
    //
    // Program the RCD feedback selection.
    //
    SetRcdTrainFeedbackSelection (Host, Socket, Dimm, Rank, FIRST_ITERATION_LOOP);

  }
  CteDelayDclk (300);
  ZeroMem ((UINT8 *) OrgDelay, sizeof (OrgDelay));
  ZeroMem ((UINT8 *) AlertFeedback, sizeof (AlertFeedback));
  ZeroMem ((UINT8 *) CurEye, sizeof (CurEye));
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

    RankMask[Ch] = 0;

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
      continue;
    }
    if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
      continue;
    }

    if (DcsSignalAppliesToThisDimmRank (Signal, SubChMask, Dimm, Rank) == FALSE) {
      continue;
    }
    //
    // Initialize Ch Rank mask
    //
    RankMask[Ch] = 1; // Rank is enabled
    //
    // Get the original delay
    //
    for (SubChTemp = 0; SubChTemp < SUB_CH; SubChTemp++) {
      Re[Ch][SubChTemp] = UNMARGINED_CMD_EDGE;
      Le[Ch][SubChTemp] = -UNMARGINED_CMD_EDGE;
      if (((SubChMask) & (1 << SubChTemp)) == 0) {
        continue;
      }
      GetSetSignalDdr5 (Host, Socket, Ch, DdrLevel, (Signal + SubChTemp * CsSubChGap), GSM_READ_ONLY, &OrgDelay[Ch][SubChTemp]);
    }
    //
    // Clear Dimm error results
    //
    // Indicate Rank present and enabled on this Ch
    ChBitmask |= 1 << Ch;
    //
    //Set CADB Pattern for each Channel
    //
    if (Host->DdrioUltSupport) {
      RcDebugPrint (SDBG_MEM_TRAIN, "EarlyCS: %0d %0d %0d\n", Signal, CS0_A_N, CS0_B_N);
      CteSetupPattern ("earlycs_loopback", (Signal >= CS0_B_N) ? Signal - CS0_B_N : Signal - CS0_A_N, ChBitmask, TWO_SUBCH_MASK, GetPhyRank (Dimm, Rank));
    } else {
      SetupTest (Host, Socket, MemSsType, ChBitmask, 0, NULL, DdrLevel, EarlyCsLoopbackTest, Signal, 0, FIRST_ITERATION_LOOP);
    }
    ChBitmask = 0;
  }// Ch loop
  //
  // Delay if this is a CTE build, otherwise do nothing.
  //
  CteDelayDclk (300);
  //
  // Run concurrent tests
  //
  ChBitmask = GetChBitmask (Host, Socket, Dimm, Rank);

  if (Host->DdrioUltSupport) {
    CteStartPattern (ChBitmask, TWO_SUBCH_MASK, GetPhyRank (Dimm, Rank));
  } else {
    for (SubChTemp = 0; SubChTemp < SUB_CH; SubChTemp++) {
      if (((SubChMask) & (1 << SubChTemp)) == 0) {
        continue;
      }
      TestControl (Host, Socket, MemSsType, ChBitmask, SubChTemp, EarlyCsLoopbackTest, 0, START_TEST, NOT_STOP_TEST);
    }
  }
  //
  // Program counting window settings and enable receivers/ODT.
  //
  SetCountingWindow (Host, Socket, Dimm, Rank, CSCA_TF_COUNTING_WINDOW_VALUE);
  //
  // Enable CS assertion trigger.
  //
  EnableCsaTrigger (Host, Socket, Dimm, Rank, CSA_TRIGGER_DISABLE);
  //
  // Delay if this is a CTE build, otherwise do nothing.
  //

  CteDelayDclk (1000);

  //
  // Assert that CS_CLK_TEST_RANGE is not evenly divisible by CS_CLK_STEP_SIZE
  //
  if (CS_CLK_TEST_RANGE % CS_CLK_STEP_SIZE) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_70);
  }
  //
  // Sweep Pi delays
  //
  for (PiIndex = 0; PiIndex < CS_CLK_TEST_RANGE; PiIndex += CS_CLK_STEP_SIZE) {
    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if ((RankMask[Ch] == 0)) {
        continue;
      }

      //
      //Set Pi Value
      //
      for (SubChTemp = 0; SubChTemp < SUB_CH; SubChTemp++) {
        if (((SubChMask) & (1 << SubChTemp)) == 0) {
          continue;
        }
        if (PiIndex == 0) {
          GetSetSignalDdr5 (Host, Socket, Ch, DdrLevel, (Signal + SubChTemp * CsSubChGap), GSM_READ_CSR | GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &PiIndex);
        } else {
          GetSetSignalDdr5 (Host, Socket, Ch, DdrLevel, (Signal + SubChTemp * CsSubChGap), GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &PiIndex);
        }
      } //subchtemp loop
    } //Ch loop

    //
    // Generate an IO Reset. This is required because the pi delays changed and reset the IO counter.
    //
    IO_Reset (Host, Socket);

    //
    // Enabling the counter mode.
    //
    EnableDdrioTrainingCounter (Host, Socket, Dimm, Rank);

    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if ((RankMask[Ch] == 0)) {
        continue;
      }
      CsVrefInitValue = GetCsVrefInitValue (Host, Socket, Ch, Dimm, Rank);
      ChannelMask = 1 << Ch;
      for (SubChTemp = 0; SubChTemp < SUB_CH; SubChTemp++) {
        if (((SubChMask) & (1 << SubChTemp)) == 0) {
          continue;
        }
        SignalLocal = Signal + SubChTemp * CsSubChGap;
        GetTrainResultStatus (Host, Socket, 0, ChannelMask, 1 << SubChTemp, 1, &Err, &ValidMask, 0, Dimm);
        //
        // Result fill.
        //
        if (Err > (CSCA_TF_COUNTING_WINDOW_VALUE/2)) {
          AlertFeedback[Ch][SubChTemp][PiIndex] = 1;
        } else {
          AlertFeedback[Ch][SubChTemp][PiIndex] = 0;
        }

        if ((PiIndex == 0) && (AlertFeedback[Ch][SubChTemp][PiIndex] == 0)) {
          InvertFlag[SubChTemp] = 1;
        }
        if (InvertFlag[SubChTemp] == 1) {
          if (AlertFeedback[Ch][SubChTemp][PiIndex] == 1) {
            AlertFeedback[Ch][SubChTemp][PiIndex] = 0;
          } else {
            AlertFeedback[Ch][SubChTemp][PiIndex] = 1;
          }
        }
        //
        // Fill unused feedback entries
        //
        for (i = 1; i < CS_CLK_STEP_SIZE; i++) {
          AlertFeedback[Ch][SubChTemp][PiIndex + i] = AlertFeedback[Ch][SubChTemp][PiIndex];
        }
      } //subchtemp loop
    } //Ch loop
  }//PiIndex loop

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
      TestControl (Host, Socket, MemSsType, ChBitmask, SubChTemp, EarlyCsLoopbackTest, 0, NOT_START_TEST, STOP_TEST);
    }
  }
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if ((RankMask[Ch] == 0)) {
      continue;
    }
    for (SubChTemp = 0; SubChTemp < SUB_CH; SubChTemp++) {
      if (((SubChMask) & (1 << SubChTemp)) == 0) {
        continue;
      }
      CsVrefInitValue = GetCsVrefInitValue (Host, Socket, Ch, Dimm, Rank);
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch,  Dimm, SubChTemp, Rank, NO_STROBE, NO_BIT,
        "Sweep results for %a  Vref %d :\n", GetSignalStrDdr5 ((Signal + SubChTemp * CsSubChGap)), (CsVrefInitValue + (CsVrefStep * CS_VREF_STEP_SIZE)));
      for (PiIndex = 0; PiIndex < CS_CLK_TEST_RANGE; PiIndex++) {

        if (AlertFeedback[Ch][SubChTemp][PiIndex]) {
          // '1'
          RcDebugPrint (SDBG_MAX, "1");
        } else {
          // '0'
          RcDebugPrint (SDBG_MAX, "0");
        }
      } // PiIndex loop

      RcDebugPrint (SDBG_MAX, "\n");
      // Right -> Index End
      // Left -> Index Beginning
      Status = FindErrorGraphMin (Host, Socket, Ch, Dimm, Rank, 0, &AlertFeedback[Ch][SubChTemp][0], CS_CLK_TEST_RANGE,
                 (UINT16 *)&CurEye[Ch][SubChTemp].right, (UINT16 *)&CurEye[Ch][SubChTemp].left, (UINT16 *)&PiPosition[Ch]);

      if ((Status == FAILURE) || (CurEye[Ch][SubChTemp].left == CurEye[Ch][SubChTemp].right)) {
        CurEye[Ch][SubChTemp].left = 0;
        CurEye[Ch][SubChTemp].right = 0;
      }

    } //subchtemp loop
  }//Ch loop

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if ((RankMask[Ch] == 0)) {
      continue;
    }
    for (SubChTemp = 0; SubChTemp < SUB_CH; SubChTemp++) {
      if (((SubChMask) & (1 << SubChTemp)) == 0) {
        continue;
      }
      // Update return parameters
      Re[Ch][SubChTemp] = CurEye[Ch][SubChTemp].right;
      Le[Ch][SubChTemp] = CurEye[Ch][SubChTemp].left;

      // apply minimum eye width check here
      EyeWidth = ABS ((Re[Ch][SubChTemp] - Le[Ch][SubChTemp]));
      if (EyeWidth <= MinimumMargin (Host, CS)) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChTemp, Rank, NO_STROBE, NO_BIT,
          "Eye width is too small: %d\n", EyeWidth);
        //
        // As MRC does the VREF sweep, doesn't disable the channel here.
        //
        return;
      }

      if (Re[Ch][SubChTemp] < Le[Ch][SubChTemp]) {
        // wrap around case
        Re[Ch][SubChTemp] += 256;
      }
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChTemp, Rank, NO_STROBE, NO_BIT,
        "Le = %d - Re = %d\n", Le[Ch][SubChTemp], Re[Ch][SubChTemp]);
      //
      // Restore Signal under test to its initial setting
      //
      GetSetSignalDdr5 (Host, Socket, Ch, DdrLevel, (Signal + SubChTemp * CsSubChGap), GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &OrgDelay[Ch][SubChTemp]);
    } //subchtemp loop
  } //Ch loop

}//GetCsMarginsRcdSweep

  /**

  Enables MC parity checking.

  @param Host:         Pointer to sysHost
  @param Socket:       Processor Socket to check

  **/
VOID
EnableMcParityChecking (
  PSYSHOST  Host,
  UINT8     Socket
  )
{
  EnableCAParityRuntime(Host, Socket);//DDR5_TODO. to update for spr.
} // EnableMcParityCheckingDdr5


/**

Disables MC parity checking

@param Host:         Pointer to sysHost
@param Socket:       Processor Socket to check

**/
VOID
DisableMcParityChecking (
  PSYSHOST  Host,
  UINT8     Socket
  )
{
  UINT8                             Ch;
  CHIP_PARITY_CHECKING_STRUCT       ChipDisableParityCheckingStruct;
  UINT8 MaxChDdr;

  if (!DoesChipSupportParityChecking (Host)) {
    return;
  }

  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    ChipSpecificParityCheckDisablePerCh (Host, Socket, Ch, &ChipDisableParityCheckingStruct);

    ChipSpecificErrorEnable(Host, Socket, Ch, &ChipDisableParityCheckingStruct); //DDR5_TODO. It's needed or not?

  } // Ch loop
} // DisableMcParityChecking

/**
  Enables DCS Training Mode for a given Vref training step in all the DDR5 RCDs
  for a given Dimm and Rank and SubChannel.  Will enable for both subchannels
  if parallel training mode is input.

  @param[in]  Socket          - Socket number
  @param[in]  SubChannel      - Subchannel within the channel
  @param[in]  Dimm            - Dimm number
  @param[in]  Rank            - Rank number
  @param[in]  CsVrefStep      - Particular Vref step to be used for training
  @param[in]  TrainingMode     - Serial or Parallel subchannel training mode

  @retval n/a
**/
VOID
EnableDcsTrainingModeForVrefStep (
  IN UINT8  Socket,
  IN UINT8  SubChannel,
  IN UINT8  Dimm,
  IN UINT8  Rank,
  IN UINT8  CsVrefStep,
  IN UINT8  TrainingMode
  )
{
  PSYSHOST                                            Host;
  UINT32                                              SmbData = 0;
  UINT8                                               Ch;
  UINT8                                               SubCh;
  UINT8                                               MaxChDdr;
  UINT8                                               Index;
  GSM_CSN                                             SignalToSweep;
  UINT8                                               RcwNum = 0;
  UINT8                                               ModeSelection;
  UINT8                                               SubChannelMask = 0;
  INT8                                                CsVrefInitValue;
  INT8                                                CsVrefValue;
  DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_STRUCT HostInterfaceTrainingMode;


  Host = GetSysHostPointer ();

  if (TrainingMode == RUN_IN_PARALLEL) {
    SubChannelMask = TWO_SUBCH_MASK;
  }
  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    if (!(IsDimmPresent (Socket, Ch, Dimm))) {
      continue;
    }

    CsVrefInitValue = GetCsVrefInitValue (Host, Socket, Ch, Dimm, Rank);
    CsVrefValue = CsVrefInitValue + (CsVrefStep * CS_VREF_STEP_SIZE);

    Index = (CS_PIN_INDEX (Dimm, SubChannel, Rank));
    SignalToSweep = CsToSweep[Index];
    //
    // Program CS Vref
    //
    if (Index % 2 == 0) {
      RcwNum = RDIMM_RW48;
    } else {
      RcwNum = RDIMM_RW49;
    }

    SmbData = CsVrefValue;

    if (SubChannelMask == TWO_SUBCH_MASK) {
      if (IsDcpmmPresentDimm (Socket, Ch, Dimm)) {
        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          GetSetDcsVrefDdrt2 (Host, Socket, Ch, Dimm, SubCh, SignalToSweep, GSM_FORCE_WRITE, (UINT8 *) &SmbData);
        }
        WriteRcdCwCache (Socket, Ch, SubChannelMask, Dimm, RDIMM_CW_PAGE0, RcwNum, (UINT8) SmbData);
      } else {
        WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, SubChannelMask, BYTE_WRITE, RDIMM_CW_PAGE0, RcwNum, &SmbData);
      }
    } else {
      if (IsDcpmmPresentDimm (Socket, Ch, Dimm)) {
        GetSetDcsVrefDdrt2 (Host, Socket, Ch, Dimm, SubChannel, SignalToSweep, GSM_FORCE_WRITE, (UINT8 *) &SmbData);
        WriteRcdCwCache (Socket, Ch, 1 << SubChannel, Dimm, RDIMM_CW_PAGE0, RcwNum, (UINT8) SmbData);
      } else {
        WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, 1 << SubChannel, BYTE_WRITE, RDIMM_CW_PAGE0, RcwNum, &SmbData);
      }
    }

    ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_REG, &HostInterfaceTrainingMode.Data);
    //
    // Enter the training mode to program to RCD.
    //
    if (Rank == 0) {
      ModeSelection = TRAINING_MODE_DCS0_AB_N;
    } else { // Rank == 1
      ModeSelection = TRAINING_MODE_DCS1_AB_N;
    }
    if (SubChannelMask == TWO_SUBCH_MASK) {
#ifdef USE_LATEST_RCD_SPEC
      HostInterfaceTrainingMode.Bits.host_interface_training_mode_selection_ch_a = ModeSelection;
      HostInterfaceTrainingMode.Bits.host_interface_training_mode_selection_ch_b = ModeSelection;
#else // #ifdef USE_LATEST_RCD_SPEC
      HostInterfaceTrainingMode.Bits.host_interface_tm_selection_cha = ModeSelection;
      HostInterfaceTrainingMode.Bits.host_interface_tm_selection_chb = ModeSelection;
#endif // #ifdef USE_LATEST_RCD_SPEC
    } else {
      if (SubChannel == 0) {
#ifdef USE_LATEST_RCD_SPEC
        HostInterfaceTrainingMode.Bits.host_interface_training_mode_selection_ch_a = ModeSelection;
#else // #ifdef USE_LATEST_RCD_SPEC
        HostInterfaceTrainingMode.Bits.host_interface_tm_selection_cha = ModeSelection;
#endif // #ifdef USE_LATEST_RCD_SPEC
      } else {
#ifdef USE_LATEST_RCD_SPEC
        HostInterfaceTrainingMode.Bits.host_interface_training_mode_selection_ch_b = ModeSelection;
#else // #ifdef USE_LATEST_RCD_SPEC
        HostInterfaceTrainingMode.Bits.host_interface_tm_selection_chb = ModeSelection;
#endif // #ifdef USE_LATEST_RCD_SPEC
        if (IsPmemSiliconWorkaroundEnabled(Host, Socket, Ch, Dimm, "S22011122815")) {
#ifdef USE_LATEST_RCD_SPEC
          HostInterfaceTrainingMode.Bits.host_interface_training_mode_selection_ch_a = ModeSelection;
#else // #ifdef USE_LATEST_RCD_SPEC
          HostInterfaceTrainingMode.Bits.host_interface_tm_selection_cha = ModeSelection;
#endif // #ifdef USE_LATEST_RCD_SPEC
        }
      }
    }
    if ((FeaturePcdGet (PcdCteBuild) == TRUE) && (FeaturePcdGet (PcdCosimBuild) == FALSE)) {
      RcdControlWordWrite (Socket, Ch, 1 << SubChannel, Dimm, Rank, DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_REG, HostInterfaceTrainingMode.Data);
    }

    if (FeaturePcdGet (PcdCteBuild) == FALSE) {
      ChipSetCsLoopbackDdr5 (Host, Socket, Ch, Dimm, HostInterfaceTrainingMode.Data);
    }
  } //Ch
}

/**
  Disables DCS Training Mode in all the DDR5 RCDs for a given Dimm and Rank and
  SubChannel.  Will disable for both subchannels in parallel training mode.

  @param[in]  Socket          - Socket number
  @param[in]  SubChannel      - Subchannel within the channel
  @param[in]  Dimm            - Dimm number
  @param[in]  Rank            - Rank number
  @param[in] TrainingMode - Serial or Parallel subchannel training mode

  @retval n/a
**/
VOID
DisableDcsTrainingMode (
  IN UINT8  Socket,
  IN UINT8  SubChannel,
  IN UINT8  Dimm,
  IN UINT8  Rank,
  IN UINT8  TrainingMode
  )
{
  PSYSHOST                                            Host;
  UINT8                                               Ch;
  UINT8                                               MaxChDdr;
  UINT8                                               SubChannelMask = 0;
  DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_STRUCT HostInterfaceTrainingMode;


  Host = GetSysHostPointer ();

  if (TrainingMode == RUN_IN_PARALLEL) {
    SubChannelMask = TWO_SUBCH_MASK;
  }
  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    if (!(IsDimmPresent (Socket, Ch, Dimm))) {
      continue;
    }

    ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_REG, &HostInterfaceTrainingMode.Data);
    if (SubChannel == 0) {
#ifdef USE_LATEST_RCD_SPEC
      HostInterfaceTrainingMode.Bits.host_interface_training_mode_selection_ch_a = TRAINING_MODE_NORMAL_OPERATION;
      if (SubChannelMask == TWO_SUBCH_MASK) {
        HostInterfaceTrainingMode.Bits.host_interface_training_mode_selection_ch_b = TRAINING_MODE_NORMAL_OPERATION;
      }
#else // #ifdef USE_LATEST_RCD_SPEC
      HostInterfaceTrainingMode.Bits.host_interface_tm_selection_cha = TRAINING_MODE_NORMAL_OPERATION;
      if (SubChannelMask == TWO_SUBCH_MASK) {
        HostInterfaceTrainingMode.Bits.host_interface_tm_selection_chb = TRAINING_MODE_NORMAL_OPERATION;
      }
#endif // #ifdef USE_LATEST_RCD_SPEC
    } else {
#ifdef USE_LATEST_RCD_SPEC
      if (SubChannelMask == TWO_SUBCH_MASK) {
        HostInterfaceTrainingMode.Bits.host_interface_training_mode_selection_ch_a = TRAINING_MODE_NORMAL_OPERATION;
      }
      HostInterfaceTrainingMode.Bits.host_interface_training_mode_selection_ch_b = TRAINING_MODE_NORMAL_OPERATION;
#else // #ifdef USE_LATEST_RCD_SPEC
      if (SubChannelMask == TWO_SUBCH_MASK) {
        HostInterfaceTrainingMode.Bits.host_interface_tm_selection_cha = TRAINING_MODE_NORMAL_OPERATION;
      }
      HostInterfaceTrainingMode.Bits.host_interface_tm_selection_chb = TRAINING_MODE_NORMAL_OPERATION;
#endif // #ifdef USE_LATEST_RCD_SPEC
    }
    WriteRcdCwCache (Socket, Ch, SUB_CH_A_MASK, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_REG, HostInterfaceTrainingMode.Data);
    if ((FeaturePcdGet (PcdCteBuild) == TRUE) && (FeaturePcdGet (PcdCosimBuild) == FALSE)) {
      RcdControlWordWrite (Socket, Ch, 1 << SubChannel, Dimm, Rank, DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_REG, HostInterfaceTrainingMode.Data);//DDR5_todo.
    }

    if (FeaturePcdGet (PcdCteBuild) == FALSE) {
      ChipSetCsLoopbackDdr5 (Host, Socket, Ch, Dimm, HostInterfaceTrainingMode.Data);
    }
  } //Ch
} //DisableDcsTrainingMode


/**
  Normalize DCS delays to minimize the differences while maintaining gaurdbands

  @param[in]       Socket               - Socket number
  @param[in]       Ch                   - Channel number
  @param[in]       Dimm                 - Dimm number
  @param[in, out]  SignalEdgeData       - Array of Early DCS training results
  @param[in]       BestCsVrefStepArray  - Array containing DCS Vref Step training results
  @param[in]       EarliestPerDimmDelay - Smallest Training delay result for a DIMM
  @param[in]       LatestPerDimmDelay   - Largest Training delay result for a DIMM

  @retval n/a

**/
VOID
EarlyDcsNormalization (
  IN     UINT8          Socket,
  IN     UINT8          Ch,
  IN     UINT8          Dimm,
  IN OUT GSM_CSEDGE_CTL (*SignalEdgeData)[MAX_CH][SUB_CH][MAX_DIMM][MAX_RANK_DIMM][CS_VREF_STEP_COUNT],
  IN     UINT8          (*BestCsVrefStepArray)[MAX_CH][SUB_CH][MAX_DIMM][MAX_RANK_DIMM],
  IN     INT16          EarliestPerDimmDelay,
  IN     INT16          LatestPerDimmDelay
  )
{
  PSYSHOST  Host;
  BOOLEAN   PullInToNormalize = FALSE;
  UINT8     SubChannel = 0;
  UINT8     Rank = 0;
  UINT8     BestCsVrefStep = 0;
  INT16     NewDelayTarget = 0;


  Host = GetSysHostPointer ();

  // For normalization, if the smallest delay is greater than the range minus the
  // largest delay then we pull in the delays, otherwise we push out the delays.

  if (EarliestPerDimmDelay > (CS_WRAP_AROUND_VALUE - LatestPerDimmDelay)) {
    PullInToNormalize = TRUE;
    NewDelayTarget = EarliestPerDimmDelay;
  } else {
    PullInToNormalize = FALSE;
    NewDelayTarget = LatestPerDimmDelay;
  }

  for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
    for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
        continue;
      }

      BestCsVrefStep = (*BestCsVrefStepArray)[Ch][SubChannel][Dimm][Rank];

      //if signal is less than half a dclk away from the new centerpoint target, then we do not want to move it

      if ((ABS((*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][BestCsVrefStep].CenterPoint - NewDelayTarget)) > (DCLK_PI / 2)) {

        //if signal is more than half a dclk away from the new centerpoint target, so we need to move it

        if (PullInToNormalize) {

          //Here we expect to pull in the target subchannel/rank signal, but only pull it in if there is sufficient gaurdband

          if (((*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][BestCsVrefStep].CenterPoint - DCLK_PI) >= MIN_DCS_DELAY_PI) {
            (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][BestCsVrefStep].CenterPoint -= DCLK_PI;
          } //gaurdband check
        } else {

          //Here we expect to push out the target subchannel/rank signal, but only push it out if there is sufficient gaurdband

          if (((*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][BestCsVrefStep].CenterPoint + DCLK_PI) <= MAX_DCS_DELAY_PI) {
            (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][BestCsVrefStep].CenterPoint += DCLK_PI;
          } //gaurdband check
        } //PullInToNormalize

      } //Distance from new target > 64
    } //Rank loop
  } //sub channel loop
}
/**

  Executes early CS/clock training for DDR5 RDIMM.

  @param[in] Host         - Pointer to sysHost
  @param[in] TrainingMode - Serial or Parallel subchannel training mode

  @retval N/A

**/
UINT32
EarlyCsClkRcdLoopback (
  IN PSYSHOST  Host,
  IN UINT8     TrainingMode
  )
{
  UINT8                                               Socket;
  UINT8                                               Ch;
  UINT8                                               Dimm;
  UINT8                                               Rank;
  UINT8                                               RankPresent;
  UINT8                                               Index;
  UINT8                                               CsVrefStep;
  UINT8                                               BestCsVrefStep;
  UINT8                                               BestCsVrefStepArray[MAX_CH][SUB_CH][MAX_DIMM][MAX_RANK_DIMM];
  INT8                                                CsVrefInitValue;
  INT8                                                CsVrefValue;
  INT8                                                MinAverageEyeOffsetVref[MAX_CH][SUB_CH][MAX_DIMM][MAX_RANK_DIMM];
  INT16                                               FinalCsVrefValue;
  UINT8                                               SubChannel = 0;
  UINT16                                              MinAverageEyeOffset = 0;
  UINT16                                              MinAverageEyeOffsetTemp = 0;
  UINT16                                              PreviousVrefEyeOffset;
  UINT16                                              ThisVrefEyeOffset;
  UINT16                                              NextVrefEyeOffset;
  struct                                              ClkTrainingResults TrainRes[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  GSM_CSN                                             SignalToSweep;
  INT16                                               Le[MAX_CH][SUB_CH];
  INT16                                               Re[MAX_CH][SUB_CH];
  GSM_CSEDGE_CTL                                      (*SignalEdgeData)[MAX_CH][SUB_CH][MAX_DIMM][MAX_RANK_DIMM][CS_VREF_STEP_COUNT];
  UINT32                                              Status = SUCCESS;
  UINT8                                               SubChannelMask = 0;
  UINT8                                               SubChTemp = 0;
  UINT8                                               CsSubChGap = CS0_B_N - CS0_A_N;
  INT8                                                MinAverageEyeOffsetVrefValue = 0;
  UINT8                                               MaxChDdr;
  INT16                                               EarliestPerDimmDelay;
  INT16                                               LatestPerDimmDelay;

  MaxChDdr = GetMaxChDdr ();
  SignalEdgeData = RcAllocatePool (sizeof (*SignalEdgeData));
  if (SignalEdgeData == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return FAILURE;
  }

  Socket = Host->var.mem.currentSocket;

  if (TrainingMode == RUN_IN_PARALLEL) {
    SubChannelMask = TWO_SUBCH_MASK;
  }
  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "EarlyCsClkRcdLoopback Starts with CS_CLK_STEP_SIZE %d\n", CS_CLK_STEP_SIZE);
  //
  // Init FinalPiDelay, EyeWidth, TrainRes.
  //
  ZeroMem ((UINT8 *) TrainRes, sizeof (TrainRes));
  ZeroMem ((UINT8 *) MinAverageEyeOffsetVref, sizeof (MinAverageEyeOffsetVref));
  //
  // Set QLBD and QLBS IO Direction.
  //
  SetQlbdQlbsIodirection (Host, Socket, RX_PATH_UNGATE_DISABLE);
  Index = 0;
  //
  // Init MinAverageEyeOffsetVref, (*SignalEdgeData).
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {
          Index = (CS_PIN_INDEX (Dimm, SubChannel, Rank));
          for (CsVrefStep = 0; CsVrefStep < CS_VREF_STEP_COUNT; CsVrefStep++) {
            (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][CsVrefStep].toSample = 0;
            (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][CsVrefStep].le = 0;
            (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][CsVrefStep].re = 0;
            (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][CsVrefStep].EyeWidth = 0;
            (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][CsVrefStep].CenterPoint = 0;
            (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][CsVrefStep].EyeOffset = 0xFF;
            (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][CsVrefStep].signal = CsToSweep[Index];
          } // CsVrefStep loop
        } //Rank loop
      } //Dimm loop
    } //subchannel loop
  }//Ch loop

  //
  // Do the DDRIO INIT before starting CS CLK training.
  //
  CsClkDdrioInit (Host, Socket);
  EnableDisableBcom (Host, Socket, TRUE);

  //
  // Test every Rank on every Dimm on each Vref.
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {
      OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_EARLY_CS_CLK, (UINT16) ((Socket << 8) | (Dimm << 4) | Rank)));
      RankPresent = 0;
      //
      // Starting by sampling all the SDRAM present
      //
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }

        if (!(IsDimmPresent (Socket, Ch, Dimm))) {
          Host->var.mem.piSettingStopFlag[Ch] = 0;
          continue;
        }

        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          Host->var.mem.piSettingStopFlag[Ch] = 0;
          continue;
        }
        //
        // Indicate a Rank is present on at least one channel
        //
        RankPresent = 1;
        //
        //Set the Stop Flag to the strobe mask
        Host->var.mem.piSettingStopFlag[Ch] = GetStrobeMask (Host);
      } // Ch loop

      //
      // Continue to the next Rank if this one is not populated on any channel
      //
      if (RankPresent == 0) {
        continue;
      }
      if (Rank >= 2) {
        continue;
      }

      if (!UseSmbusMrw (Host, Socket)) {
        //
        // Program the RCD feedback selection.
        //
        SetRcdTrainFeedbackSelection (Host, Socket, Dimm, Rank, FIRST_ITERATION_LOOP);
      }

      for (CsVrefStep = 0; CsVrefStep < CS_VREF_STEP_COUNT; CsVrefStep++) {
        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          if ((TrainingMode == RUN_IN_PARALLEL) && (SubChannel == 1)) {
            continue;
          }
          if (TrainingMode != RUN_IN_PARALLEL) {
            SubChannelMask = 1 << SubChannel;
            CsSubChGap = 0;
          }
          //
          // GSM_CSN   CsToSweep[NUM_CS] = {
          // CS0_A_N, CS1_A_N, CS0_B_N, CS1_B_N, CS2_A_N, CS3_A_N, CS2_B_N, CS3_B_N,
          //        CS0_A_N--D0 SC0 R0
          //        CS1_A_N--D0 SC0 R1
          //        CS0_B_N--D0 SC1 R0
          //        CS1_B_N--D0 SC1 R1
          //        CS2_A_N--D1 SC0 R0
          //        CS3_A_N--D1 SC0 R1
          //        CS2_B_N--D1 SC1 R0
          //        CS3_B_N--D1 SC1 R1
          //
          EnableDcsTrainingModeForVrefStep (Socket, SubChannel, Dimm, Rank, CsVrefStep, TrainingMode);
          Index = (CS_PIN_INDEX (Dimm, SubChannel, Rank));
          SignalToSweep = CsToSweep[Index];
          //
          // Clear the training results
          //
          ZeroMem ((UINT8 *) TrainRes, sizeof (TrainRes));
          //
          // Get the eye result of the Signal.
          //
          GetCsMarginsRcdSweep (Host, Socket, Dimm, SubChannel, Rank, SignalToSweep, Le, Re, CsVrefStep, TrainingMode);
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

              if (DcsSignalAppliesToThisDimmRank ((*SignalEdgeData)[Ch][SubChTemp][Dimm][Rank][CsVrefStep].signal, SubChannelMask, Dimm, Rank) == FALSE) {
                continue;
              }
              CsVrefValue = GetCsVrefInitValue (Host, Socket, Ch, Dimm, Rank) + (CsVrefStep * CS_VREF_STEP_SIZE);

              RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChTemp, Rank, NO_STROBE, NO_BIT,
                "Signal %a,  left %d,  right %d, CS Vref Value %d \n\n",
                GetSignalStrDdr5 ((SignalToSweep + SubChTemp * CsSubChGap)),
                Le[Ch][SubChTemp], Re[Ch][SubChTemp], CsVrefValue);

              //
              // save the results.
              //
              (*SignalEdgeData)[Ch][SubChTemp][Dimm][Rank][CsVrefStep].le = Le[Ch][SubChTemp];
              (*SignalEdgeData)[Ch][SubChTemp][Dimm][Rank][CsVrefStep].re = Re[Ch][SubChTemp];
              (*SignalEdgeData)[Ch][SubChTemp][Dimm][Rank][CsVrefStep].EyeWidth = ABS (Re[Ch][SubChTemp] - Le[Ch][SubChTemp]);
              (*SignalEdgeData)[Ch][SubChTemp][Dimm][Rank][CsVrefStep].CenterPoint = ((Re[Ch][SubChTemp] + Le[Ch][SubChTemp]) / 2) % CS_WRAP_AROUND_VALUE;
              (*SignalEdgeData)[Ch][SubChTemp][Dimm][Rank][CsVrefStep].toSample = 1;
              (*SignalEdgeData)[Ch][SubChTemp][Dimm][Rank][CsVrefStep].EyeOffset = ABS (CS_FULL_EYE_WIDTH - ABS (Re[Ch][SubChTemp] - Le[Ch][SubChTemp]));
            }//subchtemp loop
          } //Ch Loop
          //
          // Disable the training mode.
          //
          DisableDcsTrainingMode (Socket, SubChannel, Dimm, Rank, TrainingMode);
        } //Sub Channel loop
      } // CsVrefStep loop
    } //Rank loop
  }//Dimm loop
  //
  // Find the Minimum average eye offset by going through each vref training result.
  //
  RcDebugPrint (SDBG_MAX, "\nSummary: \n");
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) continue;
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (!(IsDimmPresent (Socket, Ch, Dimm))) {
        continue;
      }
      // These variables are used to keep track of the earliest and latest delays for use later in normization
      EarliestPerDimmDelay = CS_WRAP_AROUND_VALUE;
      LatestPerDimmDelay = 0;
      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          //normally, CheckRank will avoid non-populated ranks, except that CheckRank has been modified
          // such that for test type EarlyRcdCsLoopbackTest, it will do all ranks.  However, this is a
          // a problem for ranks >=2, so just putting a check here to skip ranks >=2
          if (Rank >= 2) {
            continue;
          }
          MinAverageEyeOffsetVref[Ch][SubChannel][Dimm][Rank] = 0;
          MinAverageEyeOffset = MAX_UINT16;
          CsVrefInitValue = GetCsVrefInitValue (Host, Socket, Ch, Dimm, Rank);
          for (CsVrefStep = 0; CsVrefStep < CS_VREF_STEP_COUNT; CsVrefStep++) {
            CsVrefValue = CsVrefInitValue + (CsVrefStep * CS_VREF_STEP_SIZE);
            if ((*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][CsVrefStep].toSample == 0) {
              continue;
            }
            if (CsVrefStep == 0) {
              PreviousVrefEyeOffset = (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][CsVrefStep].EyeOffset;
            } else {
              PreviousVrefEyeOffset = (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][CsVrefStep - 1].EyeOffset;
            }
            ThisVrefEyeOffset = (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][CsVrefStep].EyeOffset;
            if ((CsVrefStep + 1) < CS_VREF_STEP_COUNT) {
              NextVrefEyeOffset = (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][CsVrefStep + 1].EyeOffset;
            } else {
              NextVrefEyeOffset = (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][CsVrefStep].EyeOffset;
            }
            MinAverageEyeOffsetTemp = (PreviousVrefEyeOffset + ThisVrefEyeOffset + NextVrefEyeOffset) / 3;

            RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
              "Signal %a, CS Vref Value %3d,  eye width %3d,  eye offset %3d, Min Average Eye offset %3d, left %3d, right %3d, PI delay %3d \n",
              GetSignalStrDdr5 ((*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][CsVrefStep].signal),
              CsVrefValue,
              (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][CsVrefStep].EyeWidth,
              (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][CsVrefStep].EyeOffset,
              MinAverageEyeOffsetTemp,
              (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][CsVrefStep].le,
              (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][CsVrefStep].re,
              (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][CsVrefStep].CenterPoint);

            if (MinAverageEyeOffsetTemp < MinAverageEyeOffset) {
              MinAverageEyeOffset = MinAverageEyeOffsetTemp;
              MinAverageEyeOffsetVref[Ch][SubChannel][Dimm][Rank] = CsVrefValue;
            }
          } // CsVrefStep loop
          MinAverageEyeOffsetVrefValue = MinAverageEyeOffsetVref[Ch][SubChannel][Dimm][Rank];
          BestCsVrefStep = (MinAverageEyeOffsetVrefValue - CsVrefInitValue) / CS_VREF_STEP_SIZE;
          RcDebugPrint (SDBG_MAX, "Final Setting: \n");
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
            "Signal %a: CS Vref Value %3d,  eye width %3d,  Min Average Eye offset %3d, left %3d, right %3d, PI delay %3d\n",
            GetSignalStrDdr5 ((*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][BestCsVrefStep].signal),
            MinAverageEyeOffsetVref[Ch][SubChannel][Dimm][Rank],
            (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][BestCsVrefStep].EyeWidth, MinAverageEyeOffset,
            (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][BestCsVrefStep].le,
            (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][BestCsVrefStep].re,
            (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][BestCsVrefStep].CenterPoint);
          //
          // Apply minimum eye width check
          //
          if ((*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][BestCsVrefStep].EyeWidth < MinimumMargin (Host, CS)) {

            RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
              "Signal %a: Best Eye Width %3d is smaller than %3d, disable channel\n",
              GetSignalStrDdr5 ((*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][BestCsVrefStep].signal),
              (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][BestCsVrefStep].EyeWidth,
              MinimumMargin (Host, CS));
            //
            // Disable the channel.
            //
            EwlOutputType3 (WARN_MEMORY_TRAINING, WARN_CS_CLK_LOOPBACK_TRAINING, Socket, Ch, Dimm, Rank, EwlSeverityWarning,
                            MrcGtDelim, DdrLevel, (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][BestCsVrefStep].signal,
                            (UINT8) ((*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][BestCsVrefStep].EyeWidth));
            DisableChannelSw (Host, Socket, Ch);
          } else {
            //
            // Save the best vref step in an array
            //
            BestCsVrefStepArray[Ch][SubChannel][Dimm][Rank] = BestCsVrefStep;
            //
            // Keep track of the earliest and latest delays on this sub channel
            //
            if (EarliestPerDimmDelay > (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][BestCsVrefStep].CenterPoint) {
              EarliestPerDimmDelay = (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][BestCsVrefStep].CenterPoint;
            }
            if (LatestPerDimmDelay < (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][BestCsVrefStep].CenterPoint) {
              LatestPerDimmDelay = (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][BestCsVrefStep].CenterPoint;
            }
            //
            // Program the Vref Result.
            //
            FinalCsVrefValue = MinAverageEyeOffsetVref[Ch][SubChannel][Dimm][Rank];
            GetSetRcdDcsVref (
              Host,
              Socket,
              Ch,
              SubChannel,
              Dimm,
              (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][BestCsVrefStep].signal,
              GSM_READ_CSR | GSM_FORCE_WRITE | GSM_UPDATE_CACHE,
              &FinalCsVrefValue
              );
            RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
              "Signal %a: vref value %3d\n",
              GetSignalStrDdr5 ((*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][BestCsVrefStep].signal),
              MinAverageEyeOffsetVref[Ch][SubChannel][Dimm][Rank]);
          }
          RcDebugPrint (SDBG_MAX, "\n");
        } //Rank loop
      } //sub channel loop
      //
      // After looping through the all the subchannels, and ranks on a channel, normalize the shared signals on the dimm for this channel
      //
      // DCS normalzation is needed if the difference between the smallest delay and largest delay of shared signals is >= 32
      //
      if ((LatestPerDimmDelay - EarliestPerDimmDelay) > (DCLK_PI / 4)) {
        EarlyDcsNormalization (Socket, Ch, Dimm, SignalEdgeData, &BestCsVrefStepArray, EarliestPerDimmDelay, LatestPerDimmDelay);
      } // DCS normalization

    } //Dimm loop

    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (!(IsDimmPresent (Socket, Ch, Dimm))) {
          continue;
        }
        for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          //normally, CheckRank will avoid non-populated ranks, except that CheckRank has been modified
          // such that for test type EarlyRcdCsLoopbackTest, it will do all ranks.  However, this is a
          // a problem for ranks >=2, so just putting a check here to skip ranks >=2
          if (Rank >= 2) {
            continue;
          }
          BestCsVrefStep = BestCsVrefStepArray[Ch][SubChannel][Dimm][Rank];
          Status = GetSetSignalDdr5 (Host, Socket, Ch, DdrLevel, (*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][BestCsVrefStep].signal,
                                      GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &((*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][BestCsVrefStep].CenterPoint));
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
            "Signal %a: delay value %3d\n",
            GetSignalStrDdr5 ((*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][BestCsVrefStep].signal),
            ((*SignalEdgeData)[Ch][SubChannel][Dimm][Rank][BestCsVrefStep].CenterPoint));
        } //Rank loop
      } //Dimm loop
    } //sub channel loop
  } //Ch loop

  RcDebugPrint (SDBG_MAX, "\n");

  if (GetDebugLevel () & SDBG_MAX) {
    DisplayCCCResults (Host, Socket);
  }
  EnableDisableBcom (Host, Socket, FALSE);
  DeassertDdrtErrAndReq (Host, Socket);
  FifoTrainReset (Host, Socket);
  RcFreePool (SignalEdgeData);
  return Status;
} // EarlyCsClkRcdLoopback

/**

  Finds cs margins for the given Signal

  @param Host        - Pointer to sysHost
  @param Socket      - Socket number
  @param Dimm        - DIMM number
  @param Rank        - Rank number
  @param Signal      - Signal
  @param *Le         - Left edge
  @param *Re         - Right edge

  @retval N/A

**/
VOID
GetCsMarginsSweep (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Dimm,
  UINT8     Rank,
  GSM_CSN   SignalSubCh[MAX_CH][SUB_CH],
  INT16     CsLeft[MAX_CH][SUB_CH],
  INT16     CsRight[MAX_CH][SUB_CH],
  INT16     EyeWidth[MAX_CH][SUB_CH],
  INT16     CsValue[MAX_CH][SUB_CH],
  INT8      Vref,
  struct    ClkTrainingResults (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5/2]
  )
{
  UINT8               Ch;
  INT16               PiIndex;
  UINT32              ValidMask[MAX_STROBE_DDR5];
  UINT32              DqFeedback[MAX_CH][CS_CLK_TEST_RANGE];
  GSM_CSN             Signal[MAX_CH];
  INT16               RisingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5/2];
  INT16               FallingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5/2];
  INT16               PulseWidth[MAX_CH][SUB_CH][MAX_STROBE_DDR5/2];
  INT16               LeftEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5/2];
  INT16               RightEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5/2];
  INT16               OrgDelay[MAX_CH][SUB_CH];
  UINT32              RankMask[MAX_CH];
  UINT32              DqCentering[MAX_CH];
  UINT32              ChBitmask = 0;
  UINT8               SubChannelMask;
  struct cmdEye       CurEye[MAX_CH][MAX_STROBE_DDR5/2];
  UINT32              DqSampleResultStrobeSubCh[SUB_CH][MAX_STROBE_DDR5/2];
  MRC_MST             MemSsType;
  UINT8               Strobe;
  UINT8               DWord;
  UINT8               Step;
  UINT16              MinimumLeftEdge;
  INT16               Tempvar;
  struct dimmNvram (*DimmNvList) [MAX_DIMM];
  UINT8               SubChannel = 0;
  UINT8               MaxChDdr;
  BOOLEAN             IsNoSignal;

  MaxChDdr = GetMaxChDdr ();
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
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
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      CsLeft[Ch][SubChannel] = -UNMARGINED_CMD_EDGE;
      CsRight[Ch][SubChannel] = UNMARGINED_CMD_EDGE;
      CsValue[Ch][SubChannel] = 0;
      EyeWidth[Ch][SubChannel] = 0;
    }
  }
  for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
    for (Strobe = 0; Strobe < MAX_STROBE_DDR5/2; Strobe++) {
      DqSampleResultStrobeSubCh[SubChannel][Strobe] = 0;
    }
  }

  //
  // Init the original delay
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      OrgDelay[Ch][SubChannel] = 0;
    }
  }
  CteDelayDclk (300);
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    ChBitmask = 0;
    RankMask[Ch] = 0;

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
      continue;
    }

    //
    // Skip the channel if any of its subchannel has NO_SIGNAL
    //
    IsNoSignal = FALSE;
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      Signal[Ch] = SignalSubCh[Ch][SubChannel];
      if (Signal[Ch] == NO_SIGNAL) {
        IsNoSignal = TRUE;
        break;
      };
    }

    if (IsNoSignal ) {
      continue;
    }

    // Initialize Rank mask on this channel.
    RankMask[Ch] = 1; // Rank is enabled
    //for Strobe
    for (Strobe = 0; Strobe < MAX_STROBE_DDR5; Strobe++) {
      CurEye[Ch][Strobe].left = 0;
      CurEye[Ch][Strobe].right = 0;
    }

    // Indicate Ch present
    ChBitmask = 1 << Ch;

    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      //
      //Signal[Ch] will be CS0/CS2, or CS1/CS3, ....
      //
      Signal[Ch] = SignalSubCh[Ch][SubChannel];
      //
      // Get the original delay
      //
      GetSetSignalDdr5 (Host, Socket, Ch, DdrLevel, Signal[Ch], GSM_READ_ONLY, &OrgDelay[Ch][SubChannel]);

      // support larger Step sizes
      for (PiIndex = 0; PiIndex < CS_CLK_TEST_RANGE; PiIndex++) {
        DqFeedback[Ch][PiIndex] = 0; // Clear
      } //PiIndex loop
      //
      //Set CADB Pattern for each Channel
      //
      if (Host->DdrioUltSupport) {
        RcDebugPrint (SDBG_MEM_TRAIN, "EarlyCS: %0d %0d %0d\n", Signal[Ch], CS0_A_N, CS0_B_N);
        CteSetupPattern ("earlycs_loopback", (Signal[Ch] >= CS0_B_N) ? Signal[Ch] - CS0_B_N : Signal[Ch] - CS0_A_N, ChBitmask, (Signal[Ch] >= CS0_B_N) ? 0x2 : 0x1, GetPhyRank (Dimm, Rank));
      } else {
        SetupTest (Host, Socket, MemSsType, ChBitmask, 0, NULL, DdrLevel, EarlyCsLoopbackTest, Signal[Ch], 0, FIRST_ITERATION_LOOP);
      }
    }//SubChannel loop
  }// Ch loop

  //
  // Run concurrent tests
  //
  ChBitmask = GetChBitmask (Host, Socket, Dimm, Rank);
  if (Host->DdrioUltSupport) {
    CteStartPattern (ChBitmask, 0x3, GetPhyRank (Dimm, Rank));
  } else {
    TestControl (Host, Socket, MemSsType, ChBitmask, NO_SUBCH, EarlyCsLoopbackTest, 0, START_TEST, NOT_STOP_TEST);
  }
  //
  // Set the DQ count window.
  //
  SetDataTrainCountWindow (Host, Socket, Dimm, Rank, DQ_COUNTING_WINDOW_VALUE);
  //
  // Delay if this is a CTE build, otherwise do nothing.
  //

  CteDelayDclk (200);

  //
  // Sweep Pi delays
  //
  for (PiIndex = CS_CLK_PI_START; PiIndex < CS_CLK_TEST_RANGE + CS_CLK_PI_START; PiIndex += CS_CLK_STEP_SIZE) {//DDR5_TODO  CTL_CLK_STEPSIZE is 2?
    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if ((RankMask[Ch] == 0)) {
        continue;
      }

      //
      //Set Pi Value
      //
      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        GetSetSignalDdr5 (Host, Socket, Ch, DdrLevel, SignalSubCh[Ch][SubChannel], GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &PiIndex);
      }

    } //Ch loop

    //
    // Generate an IO Reset. This is required because the pi delays changed.
    //
    IO_Reset (Host, Socket);//DDR5_TODO. IO Reset may not be necessary. Need to check.
    //
    // Enabling the counter mode. DDR5_TODO.
    //
    EnableDdrioTrainingCounter (Host, Socket, Dimm, Rank);
    //
    // Get Test Result
    //
    DWord = (UINT8) ((PiIndex - CS_CLK_PI_START) / 32);
    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if ((RankMask[Ch] == 0)) {
        continue;
      }
      DimmNvList = GetDimmNvList (Host, Socket, Ch);

      // Continue to the next Rank if this one is disabled
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
        continue;
      }

      GetTrainResultStatus (Host, Socket, 0, ChBitmask, NO_SUBCH, sizeof (DqSampleResultStrobeSubCh), (UINT32*) DqSampleResultStrobeSubCh, ValidMask, 0, Dimm);

      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        for (Strobe = 0; Strobe < MAX_STROBE_DDR5/2; Strobe++) {
          if (IsDimmStrobeNotValidDdr5 (&(*DimmNvList) [Dimm], Strobe)) {
            continue;
          }

          if ((DqSampleResultStrobeSubCh[SubChannel][Strobe] & 0x1FF) >= CSCA_TF_COUNTING_WINDOW_VALUE / 2) {
            for (Step = 0; Step < CS_CLK_STEP_SIZE; Step++) {
              (*TrainRes) [Ch][SubChannel][Strobe].results[DWord] = (*TrainRes) [Ch][SubChannel][Strobe].results[DWord] | (1 << ((PiIndex + Step) % 32));
            } // Step loop
          }
        } //Strobe loop
      }//subchannel loop

      // Fill unused feedback entries //DDR5_TODO
      /*for (i = 1; i < CS_CLK_STEP_SIZE; i++) {
        DqFeedback[Ch][PiIndex + i] = DqFeedback[Ch][PiIndex];
      }*/

    } //Ch loop
  }//PiIndex loop

  //
  // Stop Test
  //
  if (Host->DdrioUltSupport) {
    CteStopPattern ();
  } else {
    TestControl (Host, Socket, MemSsType, ChBitmask, NO_SUBCH, EarlyCsLoopbackTest, 0, NOT_START_TEST, STOP_TEST);
  }
  //
  // Handle the training result.
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    //
    // Continue to the next Rank if this one is disabled
    //
    if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) continue;
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      if (GetDebugLevel () & SDBG_MAX) {
        AcquirePrintControl ();

        RcDebugPrint (SDBG_DEFAULT, "\nSummary: Early Cs Clk training Pi\nS%d, Ch%d, DIMM%d, SC%d, Rank%d, %a, Vref %d\n",
          Socket, Ch, Dimm, SubChannel, Rank, GetSignalStrDdr5 (SignalSubCh[Ch][SubChannel]), Vref);
        PrintSampleArrayCsClk (Host, Socket, Ch, Dimm, 0, SubChannel, CS_CLK_TEST_RANGE, TrainRes);
        ReleasePrintControl ();
      }

      for (Strobe = 0; Strobe < MAX_STROBE_DDR5/2; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }

        EvaluatePiSettingResultsCsClk (Host, Socket, Ch, Dimm, Rank, Strobe, RisingEdge, FallingEdge, PI_CS_CLK_TYPE, SubChannel, EarlyCsLoopbackTest, TrainRes);
        //
        // Calculate left edge and right edge
        //
        LeftEdge[Ch][SubChannel][Strobe] = MIN (FallingEdge[Ch][SubChannel][Strobe], RisingEdge[Ch][SubChannel][Strobe]);
        RightEdge[Ch][SubChannel][Strobe] = MAX (FallingEdge[Ch][SubChannel][Strobe], RisingEdge[Ch][SubChannel][Strobe]);
      } // Strobe loop
      //
      // Make sure the eye width is large enough--check the eye pass criteria.
      //
      //
      // Calculate the composite eye
      //
      //
      // Find the minumum left edge
      //
      MinimumLeftEdge = LeftEdge[Ch][SubChannel][0];
      for (Strobe = 0; Strobe < MAX_STROBE_DDR5/2; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }
        if (MinimumLeftEdge > LeftEdge[Ch][SubChannel][Strobe]) {
          MinimumLeftEdge = LeftEdge[Ch][SubChannel][Strobe];
        }
      }
      //
      // Determin the right edge needs to be wrapped or not.
      //
      for (Strobe = 0; Strobe < MAX_STROBE_DDR5/2; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }
        if ((LeftEdge[Ch][SubChannel][Strobe] - MinimumLeftEdge) > (MinimumLeftEdge - (RightEdge[Ch][SubChannel][Strobe] - 256))) {
          Tempvar = RightEdge[Ch][SubChannel][Strobe];
          RightEdge[Ch][SubChannel][Strobe] = LeftEdge[Ch][SubChannel][Strobe];
          LeftEdge[Ch][SubChannel][Strobe] = Tempvar - CS_WRAP_AROUND_VALUE;
        }
        //
        // Calculate the pulse width.
        //
        PulseWidth[Ch][SubChannel][Strobe] = RightEdge[Ch][SubChannel][Strobe] - LeftEdge[Ch][SubChannel][Strobe];
      }
      //
      // Calculate the composite eye
      //

      for (Strobe = 0; Strobe < MAX_STROBE_DDR5/2; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }
        if (LeftEdge[Ch][SubChannel][Strobe] > CsLeft[Ch][SubChannel]) {
          CsLeft[Ch][SubChannel] = LeftEdge[Ch][SubChannel][Strobe];
        }
        if (CsRight[Ch][SubChannel] > RightEdge[Ch][SubChannel][Strobe]) {
          CsRight[Ch][SubChannel] = RightEdge[Ch][SubChannel][Strobe];
        }
      }
      CsValue[Ch][SubChannel] = ((CsLeft[Ch][SubChannel] + CsRight[Ch][SubChannel]) / 2) % 256;
      EyeWidth[Ch][SubChannel] = ABS (CsRight[Ch][SubChannel] - CsLeft[Ch][SubChannel]);
      //
      // Display the left edge, right edge and CS PI value.
      //
      DisplayEdgesDDR5 (Host, Socket, Ch, PulseWidth[Ch], LeftEdge[Ch], RightEdge[Ch], SubChannel, MAX_STROBE_DDR5/2);
      DisplayCompositeEyeDDR5 (Host, Socket, Ch, CsLeft[Ch], CsRight[Ch], EyeWidth[Ch], CsValue[Ch], SubChannel, MAX_STROBE_DDR5/2);

    }//subchannel loop
  } // Ch loop
  //
  // Restore Signal under test to its initial setting
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if ((RankMask[Ch] == 0)) {
      continue;
    }
    //
    // Restore Signal under test to its initial setting
    //
    SubChannelMask = 0;
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      //
      //Signal[Ch] will be CS0/CS2, or CS1/CS3, ....
      //
      Signal[Ch] = SignalSubCh[Ch][SubChannel];
      GetSetSignalDdr5 (Host, Socket, Ch, DdrLevel, Signal[Ch], GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &OrgDelay[Ch][SubChannel]);
      //
      // Check Subchannel is enabled or not. DDR5_TODO
      //
      SubChannelMask = SubChannelMask | (1 << SubChannel);
    }//sub channel loop
    //
    // Exit DRAM CS Training mode.
    //
    WriteMPC (Host, Socket, Ch, Dimm, SubChannelMask, Rank, Exit_CS_Training_Mode, TIMING_4N, TRUE);
    //
    // Disable DDRIO Receiver and ODT. DDR5_TODO.
    //
  } //Ch loop
}//GetCsMarginsSweep

/**

Finds cs backside margins for the given Signal

@param Host        - Pointer to sysHost
@param Socket      - Socket number
@param Dimm        - DIMM number
@param Rank        - Rank number
@param Signal      - Signal
@param *Le         - Left edge
@param *Re         - Right edge

@retval N/A

**/
VOID
GetCsBacksideMarginsSweep (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Dimm,
  UINT8     Rank,
  GSM_CSN   SignalSubCh[MAX_CH][SUB_CH],
  INT16     CsLeft[MAX_CH][SUB_CH][COPY_NUM],
  INT16     CsRight[MAX_CH][SUB_CH][COPY_NUM],
  INT16     EyeWidth[MAX_CH][SUB_CH][COPY_NUM],
  INT16     CsValue[MAX_CH][SUB_CH][COPY_NUM],
  INT8      CsVrefValue,
  struct    ClkTrainingResults (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8               Ch;
  UINT8               PiIndex;
  UINT32              ValidMask[MAX_STROBE_DDR5];
  UINT32              DqFeedback[MAX_CH][CS_CLK_TEST_RANGE];
  GSM_CSN             Signal[MAX_CH];
  INT16               RisingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16               FallingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16               PulseWidth[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16               LeftEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16               RightEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16               OrgDelay[MAX_CH][SUB_CH][COPY_NUM];
  INT16               CsDelay;
  UINT32              RankMask[MAX_CH];
  UINT32              DqCentering[MAX_CH];
  UINT32              ChBitmask = 0;
  UINT32              SmbData = 0;
  UINT8               SubChannelMask;
  struct cmdEye       CurEye[MAX_CH][MAX_STROBE_DDR5 / 2];
  UINT32              DqSampleResultStrobeSubCh[SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT32              DqSampleResultStrobe[MAX_STROBE_DDR5 / 2];
  MRC_MST             MemSsType;
  UINT8               Strobe;
  UINT8               DWord;
  UINT8               Step;
  UINT16              MinimumLeftEdge;
  INT16               Tempvar;
  UINT8               MaxStrobe = 0;
  UINT8               SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8               SubChMSVx8 = GetSubChMaxStrobeValid (Host) / 2;
  MRC_RT              MrcRankList;
  struct dimmNvram    (*DimmNvList) [MAX_DIMM];
  UINT8               SubChannel = 0;
  UINT8               MaxChDdr;
  UINT8               MaxRank = 0;
  UINT8               BacksideCopy;
  UINT8               RawCardStrobe[MAX_A_B_STROBE];
  BOOLEAN             IsNoSignal;
  DDR5_RCD_QACA_OUTPUT_DELAY_STRUCT QxcaOutputDelay[COPY_NUM][SUB_CH];

  MaxChDdr = GetMaxChDdr ();
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  //
  // Init DqCentering array and DqSampleResult array.
  //
  ZeroMem (DqCentering, sizeof (DqCentering));
  //
  // Init CsLeft,  CsRight, CsValue
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      for (BacksideCopy = 0; BacksideCopy < GetMaxBacksideCopyNum (Socket, Ch, Dimm); BacksideCopy++) {
        CsLeft[Ch][SubChannel][BacksideCopy] = -UNMARGINED_CMD_EDGE;
        CsRight[Ch][SubChannel][BacksideCopy] = UNMARGINED_CMD_EDGE;
        CsValue[Ch][SubChannel][BacksideCopy] = 0;
        EyeWidth[Ch][SubChannel][BacksideCopy] = 0;
      }
    }
  }

  ZeroMem (DqSampleResultStrobeSubCh, sizeof (DqSampleResultStrobeSubCh));
  ZeroMem (DqSampleResultStrobe, sizeof (DqSampleResultStrobe));

  //
  // Init the original delay
  //
  ZeroMem (OrgDelay, sizeof (OrgDelay));

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    ChBitmask = 0;
    RankMask[Ch] = 0;

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    MaxRank = GetNumberOfRanksOnDimm (Socket, Ch, Dimm);
    if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= MaxRank)) {
      continue;
    }

    //
    // Skip the channel if any of its subchannel has NO_SIGNAL
    //
    IsNoSignal = FALSE;
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      Signal[Ch] = SignalSubCh[Ch][SubChannel];
      if (Signal[Ch] == NO_SIGNAL) {
        IsNoSignal = TRUE;
        break;
      };
    }

    if (IsNoSignal ) {
      continue;
    }

    // Initialize Rank mask on this channel.
    RankMask[Ch] = 1; // Rank is enabled
    //for Strobe
    ZeroMem (CurEye, sizeof (CurEye));

    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    //
    // Apply workaround for Entry
    //
    RcDebugPrintWithDevice (
      SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "WEntry\n"
      );
    CsDelay = 0x7F;
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

      ReadRcdSmbByteDdr5 (Host, Socket, Ch, Dimm, SubChannel, RDIMM_CW_PAGE0, DDR5_RCD_QACA_OUTPUT_DELAY_REG, &QxcaOutputDelay[COPY_A][SubChannel].Data);
      QxcaOutputDelay[COPY_A][SubChannel].Bits.output_delay_control_for_qaca_output_signals = 0;

      ReadRcdSmbByteDdr5 (Host, Socket, Ch, Dimm, SubChannel, RDIMM_CW_PAGE0, DDR5_RCD_QBCA_OUTPUT_DELAY_REG, &QxcaOutputDelay[COPY_B][SubChannel].Data);
      QxcaOutputDelay[COPY_B][SubChannel].Bits.output_delay_control_for_qaca_output_signals = 0;

      RcDebugPrintWithDevice (
        SDBG_MEM_TRAIN, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
        "QACA = %4d QBCA = %4d CsDelay = 0x%02x\n",
        QxcaOutputDelay[COPY_A][SubChannel].Data,
        QxcaOutputDelay[COPY_B][SubChannel].Data,
        CsDelay
        );

      for (BacksideCopy = 0; BacksideCopy < GetMaxBacksideCopyNum (Socket, Ch, Dimm); BacksideCopy++) {
        GetSetQcsDelay (
          Host, Socket, Ch, Dimm, Rank,
          CsToSweepBackSide[GetBacksideCsSignalIndex (Ch, SubChannel, Dimm, Rank)],
          BacksideCopy,
          GSM_FORCE_WRITE,
          &CsDelay,
          NULL,
          NULL
          );
      } // BacksideCopy loop ...

    } // SubChannel loop ...

    // Indicate Ch present
    ChBitmask |= 1 << Ch;
    //
    //Set CADB Pattern for each Channel.
    //
    if (Host->DdrioUltSupport) {
      RcDebugPrint (SDBG_MEM_TRAIN, "EarlyCS: %0d %0d %0d\n", Signal[Ch], QCS0_A_n, QCS0_B_n);
      CteSetupPattern ("earlycs_loopback_backside", (Signal[Ch] >= QCS0_B_n) ? Signal[Ch] - QCS0_B_n : Signal[Ch] - QCS0_A_n, ChBitmask, (Signal[Ch] >= QCS0_B_n) ? 0x2 : 0x1, GetPhyRank (Dimm, Rank));
      SmbData = BIT0 | (Rank << 1);
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, SUB_CH_A_MASK, BYTE_WRITE, RDIMM_CW_PAGE0, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, &SmbData);
    } else {
      MrcRankList.Dimm = Dimm;
      MrcRankList.Rank = Rank;
      SetupTest (Host, Socket, MemSsType, ChBitmask, 1, &MrcRankList, DdrLevel, EarlyCsBacksideLoopbackTest, Signal[Ch], 0, FIRST_ITERATION_LOOP);
    }

    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      //
      //Signal[Ch] will be CS0/CS2, or CS1/CS3, ....
      //
      Signal[Ch] = SignalSubCh[Ch][SubChannel];

      for (BacksideCopy = 0; BacksideCopy < GetMaxBacksideCopyNum (Socket, Ch, Dimm); BacksideCopy++) {
        //
        // Get the original delay
        //
        if (SUCCESS != GetSetQcsDelay (Host, Socket, Ch, Dimm, Rank, Signal[Ch], BacksideCopy, GSM_READ_CSR | GSM_READ_ONLY | GSM_UPDATE_CACHE, &CsDelay, NULL, NULL)) {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
            "Read the Signal %a delay fail\n", GetSignalStrDdr5 (Signal[Ch]));
          return;
        }
        OrgDelay[Ch][SubChannel][BacksideCopy] = CsDelay;
      }

      ZeroMem (DqFeedback, sizeof (DqFeedback));
    }//SubChannel loop
  }// Ch loop

  //
  // Get Ch bit mask.
  //
  ChBitmask = GetChBitmask (Host, Socket, Dimm, Rank);
  //
  // Set the DQ count window.
  //
  SetDataTrainCountWindow (Host, Socket, Dimm, Rank, DQ_COUNTING_WINDOW_VALUE);

  //
  // Delay if this is a CTE build, otherwise do nothing.
  //

  CteDelayDclk (200);

  //
  // Sweep Pi delays
  //
  for (PiIndex = BS_CS_CLK_PI_START; PiIndex < BS_CS_CLK_TEST_RANGE + BS_CS_CLK_PI_START; PiIndex += CS_CLK_STEP_SIZE) {//DDR5_TODO  CTL_CLK_STEPSIZE is 2?
    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if ((RankMask[Ch] == 0)) {
        continue;
      }
      //
      //Set QCS Value
      //
      CsDelay = PiIndex;
      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        for (BacksideCopy = 0; BacksideCopy < GetMaxBacksideCopyNum (Socket, Ch, Dimm); BacksideCopy++) {
          GetSetQcsDelay (Host, Socket, Ch, Dimm, Rank, SignalSubCh[Ch][SubChannel], BacksideCopy, GSM_FORCE_WRITE, &CsDelay, NULL, NULL);
        }
      }
    } //Ch loop

    //
    // Generate an IO Reset. This is required because the pi delays changed.
    //
    IO_Reset(Host, Socket);//DDR5_TODO. IO Reset may not be necessary. Need to check.
    //
    // Enabling the counter mode.
    //
    EnableDdrioTrainingCounter (Host, Socket, Dimm, Rank);
    //
    // Get Test Result
    //
    DWord = (UINT8) ((PiIndex - CS_CLK_PI_START) / 32);
    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if ((RankMask[Ch] == 0)) {
        continue;
      }
      DimmNvList = GetDimmNvList (Host, Socket, Ch);

      // Continue to the next Rank if this one is disabled
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
        continue;
      }

      GetTrainResultStatus (Host, Socket, 0, ChBitmask, NO_SUBCH, sizeof (DqSampleResultStrobeSubCh), (UINT32*) DqSampleResultStrobeSubCh, ValidMask, 0, Dimm);

      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        for (Strobe = 0; Strobe < MAX_STROBE_DDR5 / 2; Strobe++) {
          if (IsDimmStrobeNotValidDdr5 (&(*DimmNvList) [Dimm], Strobe)) {
            continue;
          }
          if ((DqSampleResultStrobeSubCh[SubChannel][Strobe] & 0x1FF) >= CSCA_TF_COUNTING_WINDOW_VALUE / 2) {
            for (Step = 0; Step < CS_CLK_STEP_SIZE; Step++) {
              (*TrainRes) [Ch][SubChannel][Strobe].results[DWord] = (*TrainRes) [Ch][SubChannel][Strobe].results[DWord] | (1 << ((PiIndex + Step) % 32));
            } // Step loop
          }
        } //Strobe loop
      }//subchannel loop

      // Fill unused feedback entries //DDR5_TODO
       /*for (i = 1; i < CS_CLK_STEP_SIZE; i++) {
       DqFeedback[Ch][PiIndex + i] = DqFeedback[Ch][PiIndex];
       }*/
    } //Ch loop
  }//PiIndex loop

  //
  // Stop the test --disable QCS Training Mode.
  //
#if 0
  ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, (UINT8 *)&SmbData);
  SmbData &= ~(BIT0);
  WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, SUB_CH_A_MASK, BYTE_WRITE, RDIMM_CW_PAGE0, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, &SmbData);
#endif //if 0

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    RcDebugPrintWithDevice (
      SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "WExit\n"
      );

    CsDelay = 0;
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

      SmbData = QxcaOutputDelay[COPY_A][SubChannel].Data;
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, 1 << SubChannel, BYTE_WRITE, RDIMM_CW_PAGE0, DDR5_RCD_QACA_OUTPUT_DELAY_REG, &SmbData);

      SmbData = QxcaOutputDelay[COPY_B][SubChannel].Data;
      WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, 1 << SubChannel, BYTE_WRITE, RDIMM_CW_PAGE0, DDR5_RCD_QBCA_OUTPUT_DELAY_REG, &SmbData);

      RcDebugPrintWithDevice (
        SDBG_MEM_TRAIN, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
        "QACA = %4d QBCA = %4d CsDelay = 0x%02x\n",
        QxcaOutputDelay[COPY_A][SubChannel].Data,
        QxcaOutputDelay[COPY_B][SubChannel].Data,
        CsDelay
        );
      for (BacksideCopy = 0; BacksideCopy < GetMaxBacksideCopyNum (Socket, Ch, Dimm); BacksideCopy++) {
        GetSetQcsDelay (
          Host, Socket, Ch, Dimm, Rank,
          CsToSweepBackSide[GetBacksideCsSignalIndex (Ch, SubChannel, Dimm, Rank)],
          BacksideCopy,
          GSM_FORCE_WRITE,
          &CsDelay,
          NULL,
          NULL
          );
      } // BacksideCopy loop ...
    } // SubChannel loop ...
  } // Ch loop ...
  //
  // Handle the training result.
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

    //
    // Stop the test --disable QCS Training Mode.
    //
    SmbData = 0;
    WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, SUB_CH_A_MASK, BYTE_WRITE, RDIMM_CW_PAGE0, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, &SmbData);
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      if (GetDebugLevel () & SDBG_MAX) {
        AcquirePrintControl ();

        RcDebugPrint (SDBG_DEFAULT, "\nSummary: Early Backside Cs Clk training Pi\nS%d, Ch%d, DIMM%d, SC%d, %a CsVrefValue 0x%x\n", Socket, Ch, Dimm, SubChannel,
          GetSignalStrDdr5 (SignalSubCh[Ch][SubChannel]), CsVrefValue);
        PrintSampleArrayCsClk (Host, Socket, Ch, Dimm, 0, SubChannel, BS_CS_CLK_TEST_RANGE, TrainRes);
        ReleasePrintControl ();
      }

      for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }

        EvaluatePiSettingResultsCsClk (Host, Socket, Ch, Dimm, Rank, Strobe, RisingEdge, FallingEdge, PI_CS_CLK_TYPE, SubChannel, EarlyCsBacksideLoopbackTest, TrainRes);
        //
        // Calculate left edge and right edge
        //
        LeftEdge[Ch][SubChannel][Strobe] = MIN (FallingEdge[Ch][SubChannel][Strobe], RisingEdge[Ch][SubChannel][Strobe]);
        RightEdge[Ch][SubChannel][Strobe] = MAX (FallingEdge[Ch][SubChannel][Strobe], RisingEdge[Ch][SubChannel][Strobe]);
        PulseWidth[Ch][SubChannel][Strobe] = RightEdge[Ch][SubChannel][Strobe] - LeftEdge[Ch][SubChannel][Strobe];
      } // Strobe loop
      DisplayEdgesDDR5 (Host, Socket, Ch, PulseWidth[Ch], LeftEdge[Ch], RightEdge[Ch], SubChannel, MaxStrobe);

      //
      // Calculate the composite eye
      //
      for (BacksideCopy = 0; BacksideCopy < GetMaxBacksideCopyNum (Socket, Ch, Dimm); BacksideCopy++) {

        GetRawCardMapping (Host, Socket, Ch, SubChannel, Dimm, BacksideCopy, RawCardStrobe);

        MinimumLeftEdge = LeftEdge[Ch][SubChannel][RawCardStrobe[0]];
        for (Strobe = 0; Strobe < MAX_A_B_STROBE; Strobe++) {
          //
          // Skip if this is an ECC Strobe when ECC is disabled
          //
          if (IsStrobeNotValidDdr5 (Host, RawCardStrobe[Strobe])) {
            continue;
          }
          if (MinimumLeftEdge > LeftEdge[Ch][SubChannel][RawCardStrobe[Strobe]]) {
            MinimumLeftEdge = LeftEdge[Ch][SubChannel][RawCardStrobe[Strobe]];
          }
        }
        //
        // Determin the right edge needs to be wrapped or not.
        //
        for (Strobe = 0; Strobe < MAX_A_B_STROBE; Strobe++) {
          //
          // Skip if this is an ECC Strobe when ECC is disabled
          //
          if (IsStrobeNotValidDdr5 (Host, RawCardStrobe[Strobe])) {
            continue;
          }
          if ((LeftEdge[Ch][SubChannel][RawCardStrobe[Strobe]] - MinimumLeftEdge) > (MinimumLeftEdge - (RightEdge[Ch][SubChannel][RawCardStrobe[Strobe]] - BACKSIDE_CS_WRAP_AROUND_VALUE))) {
            Tempvar = RightEdge[Ch][SubChannel][RawCardStrobe[Strobe]];
            RightEdge[Ch][SubChannel][RawCardStrobe[Strobe]] = LeftEdge[Ch][SubChannel][RawCardStrobe[Strobe]];
            LeftEdge[Ch][SubChannel][RawCardStrobe[Strobe]] = Tempvar - BACKSIDE_CS_WRAP_AROUND_VALUE;
          }
          //
          // Calculate the pulse width.
          //
          PulseWidth[Ch][SubChannel][RawCardStrobe[Strobe]] = RightEdge[Ch][SubChannel][RawCardStrobe[Strobe]] - LeftEdge[Ch][SubChannel][RawCardStrobe[Strobe]];
        }

        //
        // Calculate the composite eye
        //
        for (Strobe = 0; Strobe < MAX_A_B_STROBE; Strobe++) {
          //
          // Skip if this is an ECC Strobe when ECC is disabled
          //
          if (IsStrobeNotValidDdr5 (Host, RawCardStrobe[Strobe])) {
            continue;
          }
          if (LeftEdge[Ch][SubChannel][RawCardStrobe[Strobe]] > CsLeft[Ch][SubChannel][BacksideCopy]) {
            CsLeft[Ch][SubChannel][BacksideCopy] = LeftEdge[Ch][SubChannel][RawCardStrobe[Strobe]];
          }
          if (CsRight[Ch][SubChannel][BacksideCopy] > RightEdge[Ch][SubChannel][RawCardStrobe[Strobe]]) {
            CsRight[Ch][SubChannel][BacksideCopy] = RightEdge[Ch][SubChannel][RawCardStrobe[Strobe]];
          }
        }
        CsValue[Ch][SubChannel][BacksideCopy] = ((CsLeft[Ch][SubChannel][BacksideCopy] + CsRight[Ch][SubChannel][BacksideCopy]) / 2) % BACKSIDE_CS_WRAP_AROUND_VALUE;
        EyeWidth[Ch][SubChannel][BacksideCopy] = ABS (CsRight[Ch][SubChannel][BacksideCopy] - CsLeft[Ch][SubChannel][BacksideCopy]);
        //
        // Display the left edge, right edge and CS PI value.
        //
        RcDebugPrint (SDBG_MAX, "After Wrapping Determination\n");

        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, NO_RANK, NO_STROBE, NO_BIT, "%a\n", GetQcsSignalStrDdr5 (Rank, BacksideCopy));

        RcDebugPrint (SDBG_MEM_TRAIN, "DQS:");
        for (Strobe = 0; Strobe < MAX_A_B_STROBE; Strobe++) {
          if (IsStrobeNotValidDdr5 (Host, RawCardStrobe[Strobe])) {
            continue;
          }
          RcDebugPrint (SDBG_MEM_TRAIN, " %3d", RawCardStrobe[Strobe]);
        }
        RcDebugPrint (SDBG_MEM_TRAIN, "\n");

        DisplayBacksideEdgesDDR5 (Host, Socket, Ch, PulseWidth[Ch], LeftEdge[Ch], RightEdge[Ch], SubChannel, RawCardStrobe);

        RcDebugPrintLine (SDBG_MAX, (MaxStrobe * 4) + 4, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
        RcDebugPrint (SDBG_MEM_TRAIN, "Composite Left Edge:  ");
        RcDebugPrint (SDBG_MEM_TRAIN, " %3d", CsLeft[Ch][SubChannel][BacksideCopy]);
        RcDebugPrint (SDBG_MEM_TRAIN, "\nComposite Right Edge: ");
        RcDebugPrint (SDBG_MEM_TRAIN, " %3d", CsRight[Ch][SubChannel][BacksideCopy]);
        RcDebugPrint (SDBG_MEM_TRAIN, "\nComposite Eye Width: ");
        RcDebugPrint (SDBG_MEM_TRAIN, "  %3d", EyeWidth[Ch][SubChannel][BacksideCopy]);
        RcDebugPrint (SDBG_MEM_TRAIN, "\nEye Center Point:    ");
        RcDebugPrint (SDBG_MEM_TRAIN, " %3d", CsValue[Ch][SubChannel][BacksideCopy]);
        RcDebugPrint (SDBG_MEM_TRAIN, "\n\n");
      } // BacksideCopy
    }//subchannel loop
  } // Ch loop
  //
  // Restore Signal under test to its initial setting
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if ((RankMask[Ch] == 0)) {
      continue;
    }
    //
    // Restore Signal under test to its initial setting //BRS_TODO. this is not needed?
    //
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      SubChannelMask = 0;
      //
      //Signal[Ch] will be Q0CS0_N/Q0CS2_N, or Q0CS1_N/Q0CS3_N, ....
      //
      Signal[Ch] = SignalSubCh[Ch][SubChannel];

      for (BacksideCopy = 0; BacksideCopy < GetMaxBacksideCopyNum (Socket, Ch, Dimm); BacksideCopy++) {
        GetSetQcsDelay (Host, Socket, Ch, Dimm, Rank, Signal[Ch], BacksideCopy, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &OrgDelay[Ch][SubChannel][BacksideCopy], NULL, NULL);
      }

      //
      // Check Subchannel is enabled or not. DDR5_TODO
      //
      SubChannelMask = SubChannelMask | (1 << SubChannel);

    } // SubChannel loop
    //
    // Disable DDRIO Receiver and ODT. DDR5_TODO.
    //
  } // Channel loop
} // GetCsBacksideMarginsSweep

/**
  Get CS Vref initial value.

  The initial value (i.e. the value to use for the first step of the sweep) is
  based on the nominal value, the step count, and the step size. The nominal
  value comes from the ODT table, via the channel NV list. The step count and
  size are macros.

  @param[in]  Host        Pointer to sysHost.
  @param[in]  Socket      Socket number.
  @param[in]  Channel     Channel number on socket.
  @param[in]  Dimm        DIMM number on channel.
  @param[in]  Rank        Rank number on DIMM.

  @return   CS Vref initial value.
**/
INT8
GetCsVrefInitValue (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  UINT8     Dimm,
  IN  UINT8     Rank
  )
{
  CHANNEL_NVRAM_STRUCT  (*ChannelNvList)[MAX_CH];
  UINT8                 RankIndex;
  INT8                  Nominal;
  INT8                  Initial;

  ChannelNvList = GetChannelNvList (Host, Socket);
  RankIndex = GetRankIndex (Socket, Channel, Dimm, Rank);

  Nominal = (*ChannelNvList)[Channel].DcsVrefRegValue[RankIndex];
  Initial = Nominal - ((CS_VREF_STEP_COUNT / 2) * CS_VREF_STEP_SIZE);

  //
  // Negative values are not valid. Choose the smallest positive that will
  // still land exactly on the nominal for one of the steps.
  //
  if (Initial < 0) {
    Initial +=  CS_VREF_STEP_SIZE * ((-Initial + CS_VREF_STEP_SIZE - 1) / CS_VREF_STEP_SIZE);
  }

  return Initial;
}

/**
  Callback to be executed before DCS sweep is done

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
PreSweepDcs1D (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  SetCurrentTestType (CommonParameters->Socket, EarlyRcdCsLoopbackTest);

  CsClkDdrioInit (
    CommonParameters->Host,
    CommonParameters->Socket);

  EnableDisableBcom (
    CommonParameters->Host,
    CommonParameters->Socket,
    TRUE);

  SetQlbdQlbsIodirection (
    CommonParameters->Host,
    CommonParameters->Socket,
    RX_PATH_UNGATE_DISABLE);

  return MRC_STATUS_SUCCESS;
}

/**
  Callback to configure the engine to do the DCS test by test type

  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      TestType          Indicates the test type to be configured
  @param[in]      CountWindow       Count Window Value to program

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetupTestDcsWorker (
  IN COMMON_1D_PARAMETERS    *CommonParameters,
  IN MRC_TT                  TestType,
  IN UINT32                  CountWindow
  )
{
  MRC_MST MemSsType;
  UINT8   ChannelIndex;
  UINT8   SubChannelIndex;
  UINT32  ChannelMask;
  UINT8   ModeSelection;
  DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_STRUCT HostInterfaceTrainingMode;

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[CommonParameters->Socket];

  //
  // Enter the training mode.
  //
  for (ChannelIndex = 0; ChannelIndex < CommonParameters->MaxChDdr; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    if (CommonParameters->Rank[ChannelIndex] == 0) {
      ModeSelection = TRAINING_MODE_DCS0_AB_N;
    } else { // Rank == 1
      ModeSelection = TRAINING_MODE_DCS1_AB_N;
    }

    for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {

     ReadRcdCwCache (
        CommonParameters->Socket,
        ChannelIndex,
        SubChannelIndex ,
        CommonParameters->Dimm[ChannelIndex],
        RDIMM_CW_PAGE0,
        DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_REG,
        &HostInterfaceTrainingMode.Data);

      if (SubChannelIndex == 0) {
#ifdef USE_LATEST_RCD_SPEC
        HostInterfaceTrainingMode.Bits.host_interface_training_mode_selection_ch_a = ModeSelection;
#else // #ifdef USE_LATEST_RCD_SPEC
        HostInterfaceTrainingMode.Bits.host_interface_tm_selection_cha = ModeSelection;
#endif // #ifdef USE_LATEST_RCD_SPEC
      } else {
#ifdef USE_LATEST_RCD_SPEC
        HostInterfaceTrainingMode.Bits.host_interface_training_mode_selection_ch_b = ModeSelection;
#else // #ifdef USE_LATEST_RCD_SPEC
        HostInterfaceTrainingMode.Bits.host_interface_tm_selection_chb = ModeSelection;
#endif // #ifdef USE_LATEST_RCD_SPEC
      }

      ChipSetCsLoopbackDdr5 (
        CommonParameters->Host,
        CommonParameters->Socket,
        ChannelIndex,
        CommonParameters->Dimm[ChannelIndex],
        HostInterfaceTrainingMode.Data);
    } //SubChannelIndex
  } //ChannelIndex

  for (ChannelIndex = 0; ChannelIndex < CommonParameters->MaxChDdr; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    //
    // Set up feedback.
    //
    SetRcdTrainFeedbackSelectionByChannel (
      CommonParameters->Host,
      CommonParameters->Socket,
      ChannelIndex,
      CommonParameters->Dimm[ChannelIndex],
      CommonParameters->Rank[ChannelIndex],
      SECOND_ITERATION_LOOP);
  }

  //
  // Set up CPGC
  //
  ChannelMask = 0;
  for (ChannelIndex = 0; ChannelIndex < CommonParameters->MaxChDdr; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    ChannelMask |= 1 << ChannelIndex;

    for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {

      //
      //Set CADB Pattern for each Channel
      //
      SetupTest (
        CommonParameters->Host,
        CommonParameters->Socket,
        MemSsType,
        1 << ChannelIndex,
        0,
        NULL,
        DdrLevel,
        TestType, // not the EarlyRcdCsLoopbackTest
        GetRcdCsSignal (CommonParameters->Dimm[ChannelIndex], CommonParameters->Rank[ChannelIndex], SubChannelIndex),
        0,
        0);
    } //SubChannelIndex
  } //ChannelIndex

  for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
    TestControl (
      CommonParameters->Host,
      CommonParameters->Socket,
      MemSsType,
      ChannelMask,
      SubChannelIndex,
      TestType, // not the EarlyRcdCsLoopbackTest
      0,
      START_TEST,
      NOT_STOP_TEST
      );
  }

  for (ChannelIndex = 0; ChannelIndex < CommonParameters->MaxChDdr; ChannelIndex++) {

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
  Callback to configure the engine to do the Rmt DCS test

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetupTestRmtDcs (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  //
  // RMT shall select test type based on its private data, or override the counting window size based on CPGC configure.
  //
  SSA_MARGIN_CS *PrivateDcs;
  PrivateDcs = (SSA_MARGIN_CS *)CommonParameters;

  return SetupTestDcsWorker (CommonParameters, PrivateDcs->TestType, PrivateDcs->CountWindow);
}

/**
  Callback to configure and start the DCS test

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
ExecuteTestDcs (
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
  Callback to be executed when all sides are done

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
PostSweepDcs1D (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
)
{
  MRC_MST                 MemSsType;
  UINT8                   ChannelIndex;
  UINT8                   SubChannelIndex;
  UINT32                  ChannelMask;
  DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_STRUCT HostInterfaceTrainingMode;

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[CommonParameters->Socket];

  //
  //  Stop CPGC
  //
  ChannelMask = 0;
  for (ChannelIndex = 0; ChannelIndex < CommonParameters->MaxChDdr; ChannelIndex++) {

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
      DcaDckDutyCycleTest,
      0,
      NOT_START_TEST,
      STOP_TEST
      );
  }

  //
  // Exit the training mode.
  //
  for (ChannelIndex = 0; ChannelIndex < CommonParameters->MaxChDdr; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {

      ReadRcdCwCache (
        CommonParameters->Socket,
        ChannelIndex,
        SubChannelIndex ,
        CommonParameters->Dimm[ChannelIndex],
        RDIMM_CW_PAGE0,
        DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_REG,
        &HostInterfaceTrainingMode.Data);

      if (SubChannelIndex == 0) {
#ifdef USE_LATEST_RCD_SPEC
        HostInterfaceTrainingMode.Bits.host_interface_training_mode_selection_ch_a = TRAINING_MODE_NORMAL_OPERATION;
#else // #ifdef USE_LATEST_RCD_SPEC
        HostInterfaceTrainingMode.Bits.host_interface_tm_selection_cha = TRAINING_MODE_NORMAL_OPERATION;
#endif // #ifdef USE_LATEST_RCD_SPEC
      } else {
#ifdef USE_LATEST_RCD_SPEC
        HostInterfaceTrainingMode.Bits.host_interface_training_mode_selection_ch_b = TRAINING_MODE_NORMAL_OPERATION;
#else // #ifdef USE_LATEST_RCD_SPEC
        HostInterfaceTrainingMode.Bits.host_interface_tm_selection_chb = TRAINING_MODE_NORMAL_OPERATION;
#endif // #ifdef USE_LATEST_RCD_SPEC
      }

      ChipSetCsLoopbackDdr5 (
        CommonParameters->Host,
        CommonParameters->Socket,
        ChannelIndex,
        CommonParameters->Dimm[ChannelIndex],
        HostInterfaceTrainingMode.Data);
    } // SubChannelIndex
  }

  //
  // Restore QLBD and QLBS IO Direction.
  //
  SetQlbdQlbsIodirection (
    CommonParameters->Host,
    CommonParameters->Socket,
    RX_PATH_UNGATE_ENABLE);

  for (ChannelIndex = 0; ChannelIndex < CommonParameters->MaxChDdr; ChannelIndex++) {
    DisableCADBAlwaysOnMode (CommonParameters->Socket, ChannelIndex);
  }

  EnableDisableBcom (
    CommonParameters->Host,
    CommonParameters->Socket,
    FALSE);

  DeassertDdrtErrAndReq (
    CommonParameters->Host,
    CommonParameters->Socket);

  FifoTrainReset (
    CommonParameters->Host,
    CommonParameters->Socket);

  SetCurrentTestType (CommonParameters->Socket, DefaultTest);

  return MRC_STATUS_SUCCESS;
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
DcsMarginsSweepErrorResFeedbackOverride (
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
ResultsProcessingDcs (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
)
{
  MRC_MST MemSsType;
  UINT8   ChannelIndex;
  UINT8   SubChannelIndex;
  UINT32  ChannelMask;
  UINT32  Err = 0;
  UINT32  ValidMask;
  UINT8   StrobeLogical;
  UINT8   BitIndex;
  RESULTS_1D_CONDITION  *ResultConditionPtr;
  INT16   *CurrentDelayPtr;
  UINT8   Feedback;

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[CommonParameters->Socket];

  ChannelMask = 0;
  for (ChannelIndex = 0; ChannelIndex < CommonParameters->MaxChDdr; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    ChannelMask = 1 << ChannelIndex;
    for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
      GetTrainResultStatus (
        CommonParameters->Host,
        CommonParameters->Socket,
        0, //MemSsType,
        ChannelMask,
        1 << SubChannelIndex,
        1,
        &Err,
        &ValidMask,
        0,
        CommonParameters->Dimm[ChannelIndex]);

      StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, 0);

      for (BitIndex = 0; BitIndex < CommonParameters->BitsToTrack; BitIndex++) {

        ResultConditionPtr = GetResultConditionPtr (CommonParameters, 0, ChannelIndex, StrobeLogical, BitIndex);

        if (Err > 0) {
          ResultConditionPtr->Current = FAIL_CONDITION;
        } else {
          ResultConditionPtr->Current = PASS_CONDITION;
        }

        CurrentDelayPtr = GetDelayPtr (CommonParameters, 0, ChannelIndex, StrobeLogical, CommonParameters->CurrentDelays);

        if (DcsMarginsSweepErrorResFeedbackOverride (CommonParameters, ChannelIndex, SubChannelIndex, *CurrentDelayPtr, &Feedback)) {
          if (Feedback == 0) {
            ResultConditionPtr->Current = PASS_CONDITION;
          } else {
            ResultConditionPtr->Current = FAIL_CONDITION;
          }
        }
      } // BitIndex loop ...
    } // SunChannelIndex loop ...
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
  @param[in]      SignalIndex       The index of signal in DcaResults arrray
  @param[out]     DcaResults        Pointer that contains per channel, subchannel and signal margin result.

**/
VOID
UpdateDcsMarginResultsFromCurrentResults1D (
  IN     COMMON_1D_PARAMETERS    *CommonParameters,
  IN     RESULTS_1D              *Results1D,
  IN     UINT8                   SignalIndex,
     OUT struct baseMargin       *DcsResults
  )
{
  UINT8             ChannelIndex;
  UINT8             SubChannelIndex;
  UINT8             StrobeLogical;
  struct baseMargin *StrobeMargin;
  struct baseMargin *DcsResultForCurrentSignal;

  for (ChannelIndex = 0; ChannelIndex < CommonParameters->MaxChDdr; ChannelIndex++) {
    if (!CommonParameters->ChannelEnabledLocal[ChannelIndex]) {
      continue;
    }

    for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
      StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, 0);

      StrobeMargin = GetResultPtrAtStrobeLevel (CommonParameters, 0, ChannelIndex, StrobeLogical, Results1D);

      //
      // DcsResults is indexed as [MAX_CH][SUB_CH]
      //
      DcsResultForCurrentSignal = &DcsResults[ChannelIndex * SUB_CH + SubChannelIndex];

      DcsResultForCurrentSignal->p = StrobeMargin->p;
      DcsResultForCurrentSignal->n = StrobeMargin->n;

      RcDebugPrint (SDBG_SWEEP_L0, "\nCh %d, SubCh %d, signal %a Margin low %d high %d\n",
        ChannelIndex, SubChannelIndex,
        GetSignalStr (GetRcdCsSignal (CommonParameters->Dimm[ChannelIndex], CommonParameters->Rank[ChannelIndex], SubChannelIndex)),
        DcsResultForCurrentSignal->n, DcsResultForCurrentSignal->p);

    } // SubChannelIndex loop ...
  } // ChannelIndex loop
}

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
  )
{
  UINT8                   TrainingMode;
  UINT8                   SignalIndex = 0;
  UINT8                   ChannelIndex;
  UINT8                   SubChannelIndex;
  UINT8                   SubChannelParameter;
  SSA_MARGIN_CS           PrivateDcs;
  RESULTS_1D              Results;
  MRC_MST                 MemSsType;
  FLAGS_CATEGORY_OFFSETS  FlagsOffsets;
  FLAGS_CATEGORY_RESULTS  FlagsResults;
  FLAGS_CATEGORY_FSM      FlagsFsm;
  //
  // Save/Restore
  //
  UINT8                   MaxCh;
  DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_STRUCT         RcdParityAlert[MAX_CH];
  DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_STRUCT HostInterfaceTrainingMode[MAX_CH];

  ZeroMem ((UINT8 *) RcdParityAlert, sizeof (RcdParityAlert));
  ZeroMem ((UINT8 *) HostInterfaceTrainingMode, sizeof (HostInterfaceTrainingMode));

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

  MaxCh = GetMaxChDdr ();

  PrivateDcs.TestType = TestType;
  PrivateDcs.CountWindow = CountWindow;

  TrainingModeSelect (EarlyCsClkTraining, &TrainingMode);

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

  FlagsOffsets = DefaultStartOffset;
  FlagsResults = DefaultResults | SingleBitTrack | ResultsRelative;
  if (Direction == SWEEP_NEG_SIDE_ONLY) {
    FlagsFsm = LeftSideOnly;
  } else if (Direction == SWEEP_POS_SIDE_ONLY) {
    FlagsFsm = RightSideOnly;
  } else {
    FlagsFsm = DefaultFsm; // both side.
  }
  FlagsFsm |= AvoidBoundaryCross;

  for (SubChannelIndex = 0; SubChannelIndex < SUB_CH; SubChannelIndex++) {

    if (TrainingMode == RUN_IN_PARALLEL) {
      SubChannelParameter = ALL_SUBCH;
    } else {
      SubChannelParameter = SubChannelIndex;
    }

    CreateContextParameters (
      (COMMON_1D_PARAMETERS *)&PrivateDcs,
      MemSsType,
      Socket,
      ChannelBitMask,
      0, // RankMask
      SubChannelParameter,
      DimmRank,
      Level,
      Group,
      gsmCsnDelim,
      StepSize,
      1,  // DitherValue
      -1, // OffsetLeft
      0,  // OffsetRight
      0,  // LimitOffset
      FlagsOffsets, FlagsResults, FlagsFsm
    );

    AllocatePoolForResults1D ((COMMON_1D_PARAMETERS *)&PrivateDcs, &Results);

    ProcessSweep1D (
      (COMMON_1D_PARAMETERS *)&PrivateDcs,
      PreSweepDcs1D,
      SetupTestRmtDcs,
      ExecuteTestDcs,
      NULL,
      PostSweepDcs1D,
      ResultsProcessingDcs,
      &Results
    );

    UpdateDcsMarginResultsFromCurrentResults1D ((COMMON_1D_PARAMETERS *)&PrivateDcs, &Results, SignalIndex, DcsResults);

    ReleaseResources1D ((COMMON_1D_PARAMETERS *)&PrivateDcs, &Results);

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
} // GetDcsMargins

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
  IN UINT8 SubChannel
  )
{
    //
    // GSM_CSN   CsToSweep[NUM_CS] = {
    // CS0_A_N, CS1_A_N, CS0_B_N, CS1_B_N, CS2_A_N, CS3_A_N, CS2_B_N, CS3_B_N}
    //        CS0_A_N--D0 SC0 R0
    //        CS1_A_N--D0 SC0 R1
    //        CS0_B_N--D0 SC1 R0
    //        CS1_B_N--D0 SC1 R1
    //        CS2_A_N--D1 SC0 R0
    //        CS3_A_N--D1 SC0 R1
    //        CS2_B_N--D1 SC1 R0
    //        CS3_B_N--D1 SC1 R1
    //
    RC_FATAL_ERROR (((Dimm < MAX_DIMM) && (Rank < MAX_RANK_DIMM) && (SubChannel < SUB_CH)), ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_226);

    UINT8 Index = (CS_PIN_INDEX (Dimm, SubChannel, Rank));
    return CsToSweep[Index];
}

/**

  Get the QCS signal based on the Dimm, Rank and sub-channel
  @param[in]       Dimm                   Dimm slot index (0-based)
  @param[in]       Rank                   Rank index (0-based)
  @param[in]       SubChannel             Current sub-channel under test (0-based)

  @return   QCS0_A_n, QCS1_A_n, QCS0_B_n, QCS1_B_n, QCS2_A_n, QCS3_A_n, QCS2_B_n, QCS3_B_n

**/
GSM_CSN
GetBacksideCsSignal (
  IN UINT8 Dimm,
  IN UINT8 Rank,
  IN UINT8 SubChannel
  )
{
  //QCS0_A_n, QCS1_A_n,  // D0 R0 SUB CH A; D0 R1 SUB CH A
  //QCS0_B_n, QCS1_B_n,  // D0 R0 SUB CH B; D0 R1 SUB CH B
  //QCS2_A_n, QCS3_A_n,  // D1 R0 SUB CH A; D1 R1 SUB CH A
  //QCS2_B_n, QCS3_B_n,  // D1 R0 SUB CH B; D1 R1 SUB CH B

  RC_FATAL_ERROR (((Dimm < MAX_DIMM) && (Rank < MAX_RANK_DIMM) && (SubChannel < SUB_CH)), ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_221);

  UINT8 Index = (CS_PIN_INDEX (Dimm, SubChannel, Rank));
  return CsToSweepBackSide[Index];
}

/**

  Search the data collected in Results and update margin results in channel/subchannel/signal array
  WARNING:
  This function MUST be executed only after context was already created (CreateContextParameters).
  This function MUST be executed only after resources are allocated (AllocatePoolForResults1D ).

  @param[in]      CommonParameters  Pointer to the context information
  @param[in]      Results1D         Pointer to 1D results
  @param[in]      CopylIndex        The index of A_copy or B_copy
  @param[out]     QcaResults        Pointer that contains per channel, subchannel and signal margin result.

**/
VOID
UpdateQcsMarginResultsFromCurrentResults1D (
  IN     COMMON_1D_PARAMETERS    *CommonParameters,
  IN     RESULTS_1D              *Results1D,
  IN     UINT8                   CopyIndex,
     OUT struct baseMargin       *QcsResults
  )
{
  UINT8             ChannelIndex;
  UINT8             SubChannelIndex;
  UINT8             StrobeLogical;
  struct baseMargin *StrobeMargin;
  struct baseMargin *QcsResultForCurrentSignal;


  for (ChannelIndex = 0; ChannelIndex < CommonParameters->MaxChDdr; ChannelIndex++) {
    if (!CommonParameters->ChannelEnabledLocal[ChannelIndex]) {
      continue;
    }

    for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {

      StrobeLogical = GetStrobeLogical (CommonParameters, ChannelIndex, SubChannelIndex, 0);

      StrobeMargin = GetResultPtrAtStrobeLevel (CommonParameters, 0, ChannelIndex, StrobeLogical, Results1D);

      //
      // QcsResults is indexed as [MAX_CH][SUB_CH]
      //
      QcsResultForCurrentSignal = &QcsResults[ChannelIndex * SUB_CH + SubChannelIndex];

      QcsResultForCurrentSignal->p = StrobeMargin->p;
      QcsResultForCurrentSignal->n = StrobeMargin->n;

      RcDebugPrint (SDBG_SWEEP_L0, "\nCh %d, SubCh %d, signal %a Margin low %d high %d\n",
        ChannelIndex, SubChannelIndex,
        GetSignalStr (GetBacksideCsSignal (CommonParameters->Dimm[ChannelIndex], CommonParameters->Rank[ChannelIndex], SubChannelIndex)),
        QcsResultForCurrentSignal->n, QcsResultForCurrentSignal->p);

    } // SubChannelIndex loop ...
  } // ChannelIndex loop
}

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
  )
{
  UINT8                   TrainingMode;
  UINT8                   SignalIndex = 0;
  UINT8                   ChannelIndex;
  UINT8                   SubChannelIndex;
  UINT8                   SubChannelParameter;
  SSA_MARGIN_CS           PrivateQcs;
  RESULTS_1D              Results;
  MRC_MST                 MemSsType;
  FLAGS_CATEGORY_OFFSETS  FlagsOffsets;
  FLAGS_CATEGORY_RESULTS  FlagsResults;
  FLAGS_CATEGORY_FSM      FlagsFsm;

  //
  // Save/Restore
  //
  UINT8                   MaxCh;
  DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_STRUCT         RcdParityAlert[MAX_CH];
  DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_STRUCT HostInterfaceTrainingMode[MAX_CH];

  //
  // Enable sweep debug log
  //
  //UINT32      PrevDebugLevel;


  ZeroMem ((UINT8 *) RcdParityAlert, sizeof (RcdParityAlert));
  ZeroMem ((UINT8 *) HostInterfaceTrainingMode, sizeof (HostInterfaceTrainingMode));

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

  MaxCh = GetMaxChDdr ();

  PrivateQcs.TestType = TestType;
  PrivateQcs.CountWindow = CountWindow;

  TrainingModeSelect (CaClkTrainingBackSide, &TrainingMode);

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

  FlagsOffsets = DefaultStartOffset | LimitSearch;

  //
  // CS training focus on the transition, the feedback result at the nominal can be "pass" or "fail"
  // (aka. "0" or "1").
  // Use the "FocusOnTransition" to force MemSweep lib to search for the edge of transition.
  // SingleBitTrack: because the DQ counter checks the feedback of the first bit of each nibble/byte
  // ResultsPerSubChannel: because the margin result is per rank. Should stop at the first failure of any of the nibble/byte.
  //
  FlagsResults = DefaultResults | SingleBitTrack | ResultsRelative | FocusOnTransition | ResultsPerSubChannel;

  if (Direction == SWEEP_NEG_SIDE_ONLY) {
    FlagsFsm = LeftSideOnly;
  } else if (Direction == SWEEP_POS_SIDE_ONLY) {
    FlagsFsm = RightSideOnly;
  } else {
    FlagsFsm = DefaultFsm; // both side.
  }

  for (SubChannelIndex = 0; SubChannelIndex < SUB_CH; SubChannelIndex++) {

    if (TrainingMode == RUN_IN_PARALLEL) {
      SubChannelParameter = ALL_SUBCH;
    } else {
      SubChannelParameter = SubChannelIndex;
    }

    RcDebugPrint (SDBG_MAX, "**********Signal %a Group %d\n", GetSignalStr (CsToSweepBackSide[0]), Group);

    CreateContextParameters (
      (COMMON_1D_PARAMETERS *)&PrivateQcs,
      MemSsType,
      Socket,
      ChannelBitMask,
      0, // RankMask
      SubChannelParameter,
      DimmRank,
      Level,
      Group,
      gsmCsnDelim,
      StepSize,
      1,  // DitherValue
      0,  // OffsetLeft
      0,  // OffsetRight
      64,  // LimitOffset
      FlagsOffsets, FlagsResults, FlagsFsm
    );

    AllocatePoolForResults1D ((COMMON_1D_PARAMETERS *)&PrivateQcs, &Results);


    //
    // Enable sweep debug log
    //
    //PrevDebugLevel = GetDebugLevel ();
    //((COMMON_1D_PARAMETERS *)&PrivateQcs)->DebugLevel |= SDBG_SWEEP_L3 | SDBG_SWEEP_L2 | SDBG_SWEEP_L1 |SDBG_SWEEP_L0;
    //SetDebugLevel (SDBG_SWEEP_L3 | SDBG_SWEEP_L2 | SDBG_SWEEP_L1 |SDBG_SWEEP_L0 |PrevDebugLevel);


    ProcessSweep1D (
      (COMMON_1D_PARAMETERS *)&PrivateQcs,
      PreSweepQcs1DRmt,
      SetupTestRmtQcs,
      ExecuteTestQcs,
      NULL,
      PostSweepQcs1D,
      ResultsProcessingQcs,
      &Results
    );

    UpdateQcsMarginResultsFromCurrentResults1D ((COMMON_1D_PARAMETERS *)&PrivateQcs, &Results, SignalIndex, QcsResults);

    ReleaseResources1D ((COMMON_1D_PARAMETERS *)&PrivateQcs, &Results);


    //
    // Restore debug level
    //
    //SetDebugLevel (PrevDebugLevel);


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
} //  GetQcsMargins

/**
  Callback to be executed before QCS sweep is done

  @param[in]      CommonParameters    Pointer to the context information
  @param[in]      TestType            Current Test Type

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
PreSweepQcs1DWorker(
  IN        COMMON_1D_PARAMETERS    *CommonParameters,
  IN        MRC_TT                  TestType
  )
{
  UINT8                   ChannelIndex;
  UINT8                   MaxCh;

  MaxCh = CommonParameters->MaxChDdr;

  SetCurrentTestType (CommonParameters->Socket, TestType);

  //
  // Enter sense amp training Mode to all the channels here to latch results.
  //
  SetTrainingModeLatchResult (
    CommonParameters->Host,
    CommonParameters->Socket,
    CHECKPOINT_MINOR_SENSE_AMP);

  //
  // Below code was refactored from QcsQcaClkDdrioInit () of the EarlyCaClkVrefCABackside()
  // to make it possible to programm the setting by individual channel.
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

    //
    // Need to enter RCD CA pass through mode before program the DRAM to CS training mode.
    //
    EnterRcdCaPassThroughMode (
      CommonParameters->Host,
      CommonParameters->Socket,
      ChannelIndex,
      CommonParameters->Dimm[ChannelIndex],
      CommonParameters->Rank[ChannelIndex]
    );

     //
     // Enter DRAM CS Training Mode.--DIMM sub channel A and B at the same time.
     //
     WriteMPC (CommonParameters->Host,
      CommonParameters->Socket,
      ChannelIndex,
      CommonParameters->Dimm[ChannelIndex],
      TWO_SUBCH_MASK,
      CommonParameters->Rank[ChannelIndex],
      Enter_CS_Training_Mode,
      TIMING_4N, TRUE);

    //
    // Exit RCD CA pass through mode after program the DRAM to CS training mode.
    //
    ExitRcdCaPassThroughMode (
      CommonParameters->Host,
      CommonParameters->Socket,
      ChannelIndex,
      CommonParameters->Dimm[ChannelIndex],
      CommonParameters->Rank[ChannelIndex]
    );
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Callback to be executed before RMT QCS sweep is done

  @param[in]      CommonParameters    Pointer to the context information
  @param[in]      TestType            Current Test Type
  @param[in]      TrainingPatternMode          Current Test Type

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
PreSweepQcs1DRmt (
  IN COMMON_1D_PARAMETERS    *CommonParameters
  )
{

  SSA_MARGIN_CS *PrivateQcs;
  PrivateQcs = (SSA_MARGIN_CS *)CommonParameters;
  PreSweepQcs1DWorker (CommonParameters, PrivateQcs->TestType);

  return MRC_STATUS_SUCCESS;
}

/**
  Callback to configure the engine to do the QCS test by test type

  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      TestType          Indicates the test type to be configured
  @param[in]      CountWindow       Count Window Value to program

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetupTestQcsWorker (
  IN COMMON_1D_PARAMETERS    *CommonParameters,
  IN MRC_TT                  TestType,
  IN UINT32                  CountWindow
  )
  {
  MRC_MST MemSsType;
  MRC_RT  MrcRankList;
  UINT8   ChannelIndex;
  UINT8   SubChannelIndex;
  UINT8   MaxCh;

  ZeroMem ((UINT8*)&MrcRankList, sizeof (MrcRankList));

  MaxCh = CommonParameters->MaxChDdr;
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[CommonParameters->Socket];

  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    MrcRankList.Dimm = CommonParameters->Dimm[ChannelIndex];
    MrcRankList.Rank = CommonParameters->Rank[ChannelIndex];

    for (SubChannelIndex = CommonParameters->MinSubChannelIndex; SubChannelIndex < CommonParameters->MaxSubChannelIndex; SubChannelIndex++) {
      //
      // Enable RCD QCS Training Mode so that the RCD drives QCS pattern to DRAM
      // NOTE: RCD QCS training mode is controlled by a RCD RW register and has nothing to do with CPGC setup.
      //
      SetupTest (
        CommonParameters->Host,
        CommonParameters->Socket,
        MemSsType,
        (1 << ChannelIndex),
        1,
        &MrcRankList,
        DdrLevel,
        TestType,
        GetBacksideCsSignal (MrcRankList.Dimm, MrcRankList.Rank, SubChannelIndex),
        0,
        0
      );
    } // SubChannelIndex
  } // ChannelIndex

  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    //
    // Program counting window settings to sample the DRAM QCS training feedbacks which
    // return as static DQ signals.
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
  Callback to configure the engine to do the RMT QCS test

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetupTestRmtQcs (
  IN COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  //
  // RMT shall select test type based on its private data, or override the counting window size based on CPGC configure.
  //
  SSA_MARGIN_CS *PrivateQcs;
  PrivateQcs = (SSA_MARGIN_CS *)CommonParameters;

  return SetupTestQcsWorker (CommonParameters, PrivateQcs->TestType, PrivateQcs->CountWindow);
}

/**
  Callback to configure and start the QCS test

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
ExecuteTestQcs (
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
  // Enabling the counter
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
  Override the QCS feedback results for RcSim.

  @param[in]  CommonParamaters            - Pointer to the context information
  @param[in]  Channel                     - Channel number
  @param[out] DqSampleResultStrobeSubCh   - An array of feedback results. Index by [SUB_CH][STROBE]

  @retval N/A

**/
VOID
QcsMarginsSweepErrorResFeedbackOverride (
  IN     COMMON_1D_PARAMETERS    *CommonParameters,
  IN     UINT8                   Channel,
     OUT UINT32                  DqSampleResultStrobeSubCh[SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
}


/**
  Callback to interpret the HW results after each QCA sample test completes

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
ResultsProcessingQcs (
  IN COMMON_1D_PARAMETERS    *CommonParameters
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

          if (GetCurrentTestType (CommonParameters->Socket) == EarlyCsBacksideLoopbackTest) {

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
  Callback to be executed QCS sweep is done

  @param[in]      CommonParamaters  Pointer to the context information

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
PostSweepQcs1D (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  )
{
  MRC_MST                                                 MemSsType;
  UINT8                                                   ChannelIndex;
  UINT8                                                   MaxCh;
  DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_STRUCT DbInterfaceTrainingModes;

  MaxCh = CommonParameters->MaxChDdr;
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[CommonParameters->Socket];

  for (ChannelIndex = 0; ChannelIndex < MaxCh; ChannelIndex++) {

    if (SkipChannelExecution (CommonParameters, ChannelIndex)) {
      continue;
    }

    //
    // Disable RCD QCS Training Mode.
    //
    ReadRcdCwCache (CommonParameters->Socket,
      ChannelIndex, SUB_CH_A, CommonParameters->Dimm[ChannelIndex], RDIMM_CW_PAGE0,
      DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, &DbInterfaceTrainingModes.Data);

    DbInterfaceTrainingModes.Bits.qcs_training_mode_enable = 0;
    DbInterfaceTrainingModes.Bits.qcs_training_rank_selection = 0;

    RcdControlWordWrite (CommonParameters->Socket,
      ChannelIndex, SUB_CH_A_MASK, CommonParameters->Dimm[ChannelIndex], CommonParameters->Rank[ChannelIndex],
      DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, DbInterfaceTrainingModes.Data);

    //
    // Enter CA pass through mode
    //
    EnterRcdCaPassThroughMode (
      CommonParameters->Host,
      CommonParameters->Socket,
      ChannelIndex,
      CommonParameters->Dimm[ChannelIndex],
      CommonParameters->Rank[ChannelIndex]
    );

    //
    // Exit DRAM CS Training Mode.--DIMM sub channel A and B at the same time.
    //
    WriteMPC (
      CommonParameters->Host,
      CommonParameters->Socket,
      ChannelIndex,
      CommonParameters->Dimm[ChannelIndex],
      TWO_SUBCH_MASK,
      CommonParameters->Rank[ChannelIndex],
      Exit_CS_Training_Mode,
      TIMING_4N, TRUE);

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

    //
    // Exit DB DQ pass-through mode for read so that the CATM feedback can be
    // sent back to host.
    //
    SetDbDqPassThroughModeForRead (
      CommonParameters->Socket,
      ChannelIndex,
      CommonParameters->Dimm[ChannelIndex],
      CommonParameters->Rank[ChannelIndex],
      FALSE);
  }

  //
  // Exit sense amp training Mode
  //
  SetTrainingModeLatchResult (
    CommonParameters->Host,
    CommonParameters->Socket,
    CHECKPOINT_MINOR_DISABLE_SENSE_AMP);

  FifoTrainReset (
    CommonParameters->Host,
    CommonParameters->Socket);

  return MRC_STATUS_SUCCESS;
}