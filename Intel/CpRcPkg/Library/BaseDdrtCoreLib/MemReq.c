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
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/MemoryServicesLib.h>
#include <Memory/JedecDefinitions.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <MemHostChipCommon.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemDdrioIpLib.h>

#define REQ_ONE_DCLK       128
#define REQ_CLK_TEST_START 0
#define REQ_CLK_TEST_RANGE 128
#define CLK_EDGES_REQ 2
#define EVEN_EDGE_REQ 0
#define ODD_EDGE_REQ 1

UINT32 RcSimOddReqFeedback[4] = {0xFFFFFFFF, 0x0, 0x0, 0xFFFFFFFF};
UINT32 RcSimEvenReqFeedback[4] = {0x0, 0xFFFFFFFF, 0xFFFFFFFF, 0x0 };

//
// Local Prototypes
//
VOID
GetReqMargins (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT32    ChBitMask,
  UINT8     Dimm,
  INT16     LeftEdge[MAX_CH][MAX_DIMM],
  INT16     RightEdge[MAX_CH][MAX_DIMM]
  );

VOID
ReadReqFeedback (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Ch,
  UINT8    Dimm,
  UINT8    OddOrEven,
  UINT32   *Err
  );

/**

  Trains Request Pin relative to clock

  @param Host  - Pointer to sysHost

  @retval Status

**/
UINT32
EarlyReqClk (
  PSYSHOST  Host
  )
{
  UINT8                               Socket;
  UINT8                               Ch;
  UINT8                               Dimm;
  GSM_CSN                             Signal;
  INT16                               CenterPoint;
  UINT32                              ChBitMask;
  INT16                               LeftEdge[MAX_CH][MAX_DIMM];
  INT16                               RightEdge[MAX_CH][MAX_DIMM];
  struct channelNvram                 (*ChannelNvList)[MAX_CH];
  struct dimmNvram                    (*DimmNvList)[MAX_DIMM];

  Socket = Host->var.mem.currentSocket;
  //
  // Return if this Socket is disabled
  //

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_REQ_CLK, Socket));
  ChannelNvList = GetChannelNvList (Host, Socket);

  if (IsSocketDimmPopulated (Host, Socket) != TRUE) {
    return SUCCESS;
  }

  if (!IsMemFlowEnabled (EarlyReqClkTraining)) {
    return SUCCESS;
  }

  ZeroMem ((UINT8 *)LeftEdge, sizeof (LeftEdge));
  ZeroMem ((UINT8 *)RightEdge, sizeof (RightEdge));

  DoCompDdrioPerSetupOption (Socket, TRAINING_COMP_EVERY_TRAINING);

  //
  // CurrentTestType controls the flow of gathering feedback
  //
  SetCurrentTestType (Socket, ReqClkTest);
  //
  // 1. Enter Req Training Mode CWV
  //
  SetAepTrainingMode (Host, Socket, CHECKPOINT_MINOR_REQ_CLK);

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    ChBitMask = 0;
    for (Ch = 0; Ch < MAX_CH; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }
      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }
      if (((*DimmNvList)[Dimm].DcpmmPresent == 1) && ((*DimmNvList)[Dimm].fmcType == FMC_CWV_TYPE)) {
        ChBitMask = 1 << Ch;
      }
    } // Ch Loop

    if (ChBitMask == 0) {
      continue;
    }

    //
    // 2. Setup Counting Window (0x20)
    //
    SetReqCountingWindow (Host, Socket, ChBitMask, Dimm);

    //
    // 3. Initialize Host DDRIO settings
    //
    ReqClkDdrioInit (Host, Socket, ChBitMask, Dimm);

    GetReqMargins (Host, Socket, ChBitMask, Dimm, LeftEdge, RightEdge);
  } // Dimm Loop

  SetAepTrainingMode (Host, Socket, DISABLE_TRAINING_STEP);

  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      if (((*DimmNvList)[Dimm].DcpmmPresent == 1) && ((*DimmNvList)[Dimm].fmcType == FMC_CWV_TYPE)) {
        if (Dimm == 0) {
          Signal = RSPA_SA;
        } else {
          Signal = RSPB_SA;
        }
        CenterPoint = (RightEdge[Ch][Dimm] + LeftEdge[Ch][Dimm]) / 2;
        GetSetSignalDdr5 (Host, Socket, Ch, DdrLevel, Signal, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &CenterPoint);
        CleanupReqClk (Host, Socket, Ch, Dimm);
      }
    } // Dimm Loop
  } // Ch Loop

  return SUCCESS;
} // EarlyReqClk

VOID
GetReqMargins (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT32    ChBitMask,
  UINT8     Dimm,
  INT16     LeftEdge[MAX_CH][MAX_DIMM],
  INT16     RightEdge[MAX_CH][MAX_DIMM]
  )
{
  INT16               PiIndex;
  UINT8               Ch;
  GSM_CSN             Signal;
  INT16               Offset;
  UINT32              Err = 0;
  UINT8               AlertFeedback[MAX_CH][REQ_CLK_TEST_RANGE][CLK_EDGES_REQ];
  UINT8               Step = 0;
  UINT16              EyeWidth;
  BOOLEAN             LeftInvertFlag;
  BOOLEAN             RightInvertFlag;
  BOOLEAN             FindLeftEdge = FALSE;
  BOOLEAN             FindRightEdge = FALSE;

  ZeroMem ((UINT8 *)AlertFeedback, sizeof (AlertFeedback));

  if (Dimm == 0) {
    Signal = RSPA_SA;
  } else {
    Signal = RSPB_SA;
  }

  //
  // For CTE where we can reduce execution time by making step size = 8
  //
  Offset = REQ_CLK_STEP_SIZE;
  //
  //  Sweep Req Receive PI Delay
  //
  for (PiIndex = REQ_CLK_TEST_START; PiIndex < (REQ_CLK_TEST_RANGE + REQ_CLK_TEST_START); PiIndex += Offset) {

    for (Ch = 0; Ch < MAX_CH; Ch++) {
      if (!((1 << Ch) & ChBitMask)) {
        continue;
      }
      //
      // 4. PI sweep
      //
      GetSetSignalDdr5 (Host, Socket, Ch, DdrLevel, Signal, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &PiIndex);
    }

    IO_Reset (Host, Socket);

    CteDelayDclk (64);
    //
    // Start the counter
    //
    EnableDdrioTrainingCounter (Host, Socket, Dimm, 0);

    for (Ch = 0; Ch < MAX_CH; Ch++) {
      if (!((1 << Ch) & ChBitMask)) {
        continue;
      }

      //
      // Poll for completion and read the counter
      //
      ReadReqFeedback (Host, Socket, Ch, SUB_CH_A, EVEN_EDGE_REQ, &Err);
      //
      // If we sample more than half of the counter window, then we consider it as high
      //
      if (Err >= (REQ_COUNTING_WINDOW_VALUE / 4)) {
        AlertFeedback[Ch][PiIndex][EVEN_EDGE_REQ] = 1;
      } else {
        AlertFeedback[Ch][PiIndex][EVEN_EDGE_REQ] = 0;
      }

      ReadReqFeedback (Host, Socket, Ch, SUB_CH_A, ODD_EDGE_REQ, &Err);
      //
      // If we sample more than half of the counter window, then we consider it as high
      //
      if (Err >= (REQ_COUNTING_WINDOW_VALUE / 4)) {
        AlertFeedback[Ch][PiIndex][ODD_EDGE_REQ] = 1;
      } else {
        AlertFeedback[Ch][PiIndex][ODD_EDGE_REQ] = 0;
      }

      //
      // Fill in results for skipped Pi delays
      //
      for (Step = 0; Step < Offset; Step++) {
        if ((PiIndex + Step) >= REQ_CLK_TEST_RANGE) {
          continue;
        }
        AlertFeedback[Ch][PiIndex + Step][EVEN_EDGE_REQ] = AlertFeedback[Ch][PiIndex][EVEN_EDGE_REQ];
        AlertFeedback[Ch][PiIndex + Step][ODD_EDGE_REQ] = AlertFeedback[Ch][PiIndex][ODD_EDGE_REQ];
      }
    } // Ch Loop

  } // PiIndex Loop

  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if (!((1 << Ch) & ChBitMask)) {
      continue;
    }
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "Sweep results for Req Even-> ");

    LeftInvertFlag = FALSE;
    for (PiIndex = 0; PiIndex < REQ_CLK_TEST_RANGE; PiIndex++) {

      // Invert if the even edge is starting with 1
      if ((PiIndex == 0) && (AlertFeedback[Ch][PiIndex][EVEN_EDGE_REQ] == 1)) {
        LeftInvertFlag = TRUE;
      }
      if (LeftInvertFlag) {
        if (AlertFeedback[Ch][PiIndex][EVEN_EDGE_REQ] == 1) {
          AlertFeedback[Ch][PiIndex][EVEN_EDGE_REQ] = 0;
        } else {
          AlertFeedback[Ch][PiIndex][EVEN_EDGE_REQ] = 1;
        }
      }

      if (AlertFeedback[Ch][PiIndex][EVEN_EDGE_REQ]) {
        // '1'
        RcDebugPrint (SDBG_MAX, "1");
      } else {
        // '0'
        RcDebugPrint (SDBG_MAX, "0");
      }
    } // PiIndex loop

    RcDebugPrint (SDBG_MAX, "\n");

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "Sweep results for Req Odd-> ");

    RightInvertFlag = FALSE;
    for (PiIndex = 0; PiIndex < REQ_CLK_TEST_RANGE; PiIndex++) {

      // Invert if the odd edge is starting with 0
      if ((PiIndex == 0) && (AlertFeedback[Ch][PiIndex][ODD_EDGE_REQ] == 0)) {
        RightInvertFlag = TRUE;
      }
      if (RightInvertFlag) {
        if (AlertFeedback[Ch][PiIndex][ODD_EDGE_REQ] == 1) {
          AlertFeedback[Ch][PiIndex][ODD_EDGE_REQ] = 0;
        } else {
          AlertFeedback[Ch][PiIndex][ODD_EDGE_REQ] = 1;
        }
      }

      if (AlertFeedback[Ch][PiIndex][ODD_EDGE_REQ]) {
        // '1'
        RcDebugPrint (SDBG_MAX, "1");
      } else {
        // '0'
        RcDebugPrint (SDBG_MAX, "0");
      }
    } // PiIndex loop

    for (PiIndex = 0; PiIndex < REQ_CLK_TEST_RANGE; PiIndex++) {

      if (AlertFeedback[Ch][PiIndex][EVEN_EDGE_REQ] == 1) {
        LeftEdge[Ch][Dimm] = PiIndex;
        FindLeftEdge = TRUE;
        break;
      }
    }

    for (PiIndex = 0; PiIndex < REQ_CLK_TEST_RANGE; PiIndex++) {

      if (AlertFeedback[Ch][PiIndex][ODD_EDGE_REQ] == 0) {
        RightEdge[Ch][Dimm] = PiIndex + REQ_ONE_DCLK;
        FindRightEdge = TRUE;
        break;
      }
    }

    if ((LeftEdge[Ch][Dimm] == RightEdge[Ch][Dimm])) {
      LeftEdge[Ch][Dimm] = 0;
      RightEdge[Ch][Dimm] = 0;
    }

    // all 0's found in even clock edge and all 1's found in odd clock edge
    // exclude when it is inverted in even or odd clock edge, in case missed when all 0's or all 1's found in even and odd clock edge
    if (!FindLeftEdge && !FindRightEdge && ((LeftInvertFlag && RightInvertFlag) || (!LeftInvertFlag && !RightInvertFlag))) {
      LeftEdge[Ch][Dimm] = 0;
      RightEdge[Ch][Dimm] = REQ_ONE_DCLK;
    }

    // all 1's found in odd clock edge
    if (FindLeftEdge && !FindRightEdge) {
      RightEdge[Ch][Dimm] = REQ_ONE_DCLK - LeftEdge[Ch][Dimm];
    }

    // all 0's found in even clock edge
    if (!FindLeftEdge && FindRightEdge) {
      LeftEdge[Ch][Dimm] = 2 * REQ_ONE_DCLK - (RightEdge[Ch][Dimm] - REQ_ONE_DCLK);
    }

    RcDebugPrint (SDBG_MAX, "\n");

    //
    // apply minimum eye width check here
    //
    EyeWidth = ABS ((RightEdge[Ch][Dimm] - LeftEdge[Ch][Dimm]));
    if (EyeWidth <= MinimumMargin (Host, Req)) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "Eye width is too small: %d\n", EyeWidth);
      EwlOutputType3 (WARN_MEMORY_TRAINING, WARN_REQ_CLK_TRAINING, Socket, Ch, Dimm, 0, EwlSeverityWarning, MrcGtDelim, DdrLevel, Signal, (UINT8)(EyeWidth));
      DisableChannelSw (Host, Socket, Ch);
      return;
    }

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "LeftEdge = %d - RightEdge = %d\n", LeftEdge[Ch][Dimm], RightEdge[Ch][Dimm]);

  } // Ch loop

}

VOID
ReadReqFeedback (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Ch,
  UINT8    Dimm,
  UINT8    OddOrEven,
  UINT32   *Err
  )
{
  //
  // OddOrEven selects from result0 or result1
  // result0 = even
  // result1 = odd
  // Dimm selects subchannel
  // SubCh 0 counts for Dimm slot 0
  // SubCh 1 counts for Dimm slot 1
  //

  GetDdr5IntfCmdTrainCtrlFeedbackTarget (Host, Socket, Ch, Dimm, 0, OddOrEven, Err);
}