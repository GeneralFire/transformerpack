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
#include <Library/BaseMemoryLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/CteNetLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/CheckpointLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/SysHostPointerLib.h>
#include <Memory/JedecDefinitions.h>
#include "Include/BaseMemoryCoreInternal.h"
#include <Memory/MemoryCheckpointCodes.h>
#include <Memory/MemCmdCtlClkCommon.h>
#include <Library/MemRcLib.h>

//------------------------------------------------------------------------------------------
// Local defines
GSM_CSN signalsToSweep[NUM_SIGNALS_TO_SWEEP] = CHIP_GSM_CSN_SIGNALSTOSWEEP;
GSM_CSN SignalsToSweepBackside[NUM_SIGNALS_TO_SWEEP_BACKSIDE] = CHIP_GSM_CSN_SIGNALSTOSWEEPBACKSIDE;

/*
 * Test code
INT16   le0[NUM_SIGNALS_TO_SWEEP] = {-53, -57, -57, -59, -51, -53, -47, -46, -63, -64, -43, -40, -52, -49, -58, -60, -47, -43, -44, -42, -59, -61, -64, -62};
INT16   re0[NUM_SIGNALS_TO_SWEEP] = { 40,  36,  36,  34,  42,  40,  46,  47,  30,  29,  50,  53,  41,  44,  35,  33,  46,  50,  49,  51,  34,  32,  29,  31};
INT16   le1[NUM_SIGNALS_TO_SWEEP] = {-30, -37, -40, -39, -26, -33, -29, -26, -44, -44, -22, -20, -32, -29, -34, -40, -24, -23, -18, -22, -40, -41, -48, -42};
INT16   re1[NUM_SIGNALS_TO_SWEEP] = { 63,  56,  53,  54,  67,  60,  64,  67,  49,  49,  71,  73,  61,  64,  59,  53,  69,  70,  75,  71,  53,  52,  45,  51};
 */


#define NO_SIGNAL         0xff

//
// Local Prototypes
//
UINT32 CollectParityResults (PSYSHOST Host, UINT8 socket, UINT32 chToCheck, UINT8 dimm, UINT32 bwSerr[MAX_CH][3]);
VOID   ClearParityResults (PSYSHOST Host, UINT8 socket, UINT32 chToCheck, UINT8 dimm);
VOID   ClearParityResultsAlt (PSYSHOST Host, UINT8 socket, UINT32 chToCheck, UINT8 rank, UINT8 dimm);
VOID   ClearBacksideParityResults (PSYSHOST Host, UINT8 Socket, UINT32 ChToCheck, UINT8 Rank, UINT8 Dimm);
VOID   ClearBwSerr (UINT32 bwSerr[MAX_CH][3]);
VOID   OffsetGroups (PSYSHOST Host, UINT8 socket, UINT8 ch, MRC_GT group, INT16 offset);
VOID   ExecuteCtlClkTest (PSYSHOST Host, UINT8 socket, UINT32 chBitmask, UINT8 dimm, UINT8 rank, UINT8 testFlags, UINT8 *DdrioPreambleStateSave);
VOID   GetCtlClkResults (PSYSHOST Host, UINT8 socket, UINT8 dimm, UINT8 rank, UINT32 strobeFeedback[MAX_CH]);
VOID   GetCtlMarginsSweep (PSYSHOST Host, UINT8 socket, UINT8 dimm, UINT8 rank, GSM_CSN sig[MAX_CH], INT16 *le, INT16 *re);
VOID   GetChipBwSerrErrors (UINT32 errMask, UINT8 ch, UINT8 errorResult[MAX_CH][CHIP_CMD_CLK_TEST_RANGE], INT16 piIndex, INT16 offset, PCHIP_GET_CMD_MGN_SWP_STRUCT chipGetCmdMgnSwpStruct);


UINT32
LateCmdClk (
  PSYSHOST  Host
  )
/*++

  Exectues command/clock training

  @param Host      - Pointer to sysHost

  @retval SUCCESS

--*/
{
  UINT32 Status = SUCCESS;
  BOOLEAN  CurrentVal = FALSE;

  CurrentVal = GetSkipPprRowTestFlag ();
  SetSkipPprRowTestFlag (TRUE);

  Status =  ChipLateCmdClk (Host);

  SetSkipPprRowTestFlag (CurrentVal);

  return Status;
}

UINT32
PerCMDGroup (
  PSYSHOST  Host,
  UINT8     socket,
  UINT16    testCase
  )
{
  UINT8                               ch;
  UINT8                               dimm;
  UINT8                               rank;
  //UINT8                               vIndex;
  UINT16                              mode;
  MRC_GT                              group;
  MRC_GT                              groupStart;
  MRC_GT                              groupStop;
  UINT32                              status = SUCCESS;
  UINT32                              patternLength = 1;
  UINT32                              PrevDebugLevel;
  struct rankMargin                   resultsRank;
  struct bitMask                      filter;
  struct channelNvram                 (*channelNvList)[MAX_CH];
  struct dimmNvram                    (*dimmNvList)[MAX_DIMM];
  struct ddrRank                      (*rankList)[MAX_RANK_DIMM];
  GSM_CGEDGE                          groupEdgeData[MAX_CH][MAX_RANK_CH][MAX_CMD];
  UINT8                               MaxChDdr;

  // Skip socket if not enabled
  if (Host->nvram.mem.socket[socket].enabled == 0) {
    return status;
  }

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);

  //
  // Enable normal CMD timing
  //
  SetNormalCMDTiming (Host, socket);

/*
  //
  // Code to test the SetCombinedCmdGroup function
  //
  ch = 0;
        //
        // Load the eye data
        //
        for (signal = 0; signal < NUM_SIGNALS_TO_SWEEP; signal++) {
          signalEdgeData[0][signal].signal = signalsToSweep[sig];
          signalEdgeData[0][signal].le = le0[signal];
          signalEdgeData[0][signal].re = re0[signal];
          signalEdgeData[4][signal].signal = signalsToSweep[signal];
          signalEdgeData[4][signal].le = le1[signal];
          signalEdgeData[4][signal].re = re1[signal];
        } // signal loop
  //
  // Call the combine function
  //
  status = (UINT32)SetCombinedCmdGroup(Host, socket, ch, DdrLevel, GSM_COMBINE_EDGES, CMD_SIGNAL_DELAY, NUM_SIGNALS_T0_SWEEP, signalEdgeData);
 */

#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MEM_TRAIN) {
    AcquirePrintControl ();
  }
#endif // DEBUG_CODE_BLOCK
  if (testCase == COMPOSITE_GROUPS) {
    RcDebugPrint (SDBG_MAX, "START_CMD_CLK_PER_GROUP_FINAL\n");
    //numPoints = 3;
    //stepSize = 12;
    mode = MODE_VA_DESELECT;

  } else {
    RcDebugPrint (SDBG_MAX, "START_CMD_CLK_PER_GROUP\n");
    //numPoints = 1;
    //stepSize = 1;
    mode = MODE_DESELECT;
  }

  patternLength = 32;

  //
  // Clear the filter bits to enable error checking on every bit
  //
  ClearFilter (Host, socket, &filter);

  ChipEnableCheckParity (Host, &mode);

#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MEM_TRAIN) {
    ReleasePrintControl ();
  }
#endif // DEBUG_CODE_BLOCK

  IO_Reset (Host, socket);

  if (testCase == COMPOSITE_GROUPS) {
    groupStart = CmdAll;
    groupStop = CmdAll;
  } else {
    groupStart = CmdGrp0;
    groupStop = CHIP_CMD_GRP_STOP;
  }

  for (group = groupStart; group <= groupStop; group++) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Calling GetMargins for %a\n", GetPlatformGroupStr (group));
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      dimmNvList = GetDimmNvList (Host, socket, ch);

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        rankList = GetRankNvList (Host, socket, ch, dimm);
        for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
          //for (vIndex = 0; vIndex < 3; vIndex++) {
          //  resultsRank[vIndex].rank[ch][(*rankList)[rank].rankIndex].n = 0;
          //  resultsRank[vIndex].rank[ch][(*rankList)[rank].rankIndex].p = 0;
          //}
          //for (vIndex = 0; vIndex < 3; vIndex++) {
          resultsRank.rank[ch][ (*rankList)[rank].rankIndex].n = 0;
          resultsRank.rank[ch][ (*rankList)[rank].rankIndex].p = 0;
          //}
        } // rank loop
      } // dimm loop
    } // ch loop

    //
    // Get CMD margins for current rank and group
    //
    //if(mode == MODE_DESELECT) {

    // Setting the CPGC DDRT RPQ credit limit on certain projects will cause the related timer to be enabled.
    SetDefaultCpgcRpqCreditLimitIfNeeded (Host, socket);
    GetMargins (Host, socket, DdrLevel, group, mode, SCOPE_RANK, &filter, (VOID *)&resultsRank, patternLength, 0, 0, 0,
      WDB_BURST_LENGTH, LateCmdTest, NULL);
    // Clearing the CPGC DDRT RPQ credit limit on certain projects will cause the related timer to be disabled.
    ClearCpgcRpqCreditLimitIfNeeded (Host, socket);

    //} else {
    //  patternLength = 32, stepsize = 12, numsteps = 3
    //  GetMultiVref (Host, socket, DdrLevel, group, mode, SCOPE_RANK, &filter, (void *)resultsRank, patternLength, 0, 0, stepSize, numPoints);
    //}

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      dimmNvList = GetDimmNvList (Host, socket, ch);
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        rankList = GetRankNvList (Host, socket, ch, dimm);
        for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {

          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

#if 0
          //
          // Get the total eye width
          //
          eyeWidth = resultsRank[vIndexCenter].rank[ch][ (*rankList)[rank].rankIndex].p -
            resultsRank[vIndexCenter].rank[ch][ (*rankList)[rank].rankIndex].n;
          //RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
          //              "EW = R0 + L0 = %d\n", eyeWidth);
          offset = eyeHeight * ((2 * resultsRank[vIndexCenter].rank[ch][ (*rankList)[rank].rankIndex].p) +
                                (2 * resultsRank[vIndexCenter].rank[ch][ (*rankList)[rank].rankIndex].n));
          //RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
          //              "Center = EH * (2*R0 + 2*L0) = %d\n", offset);
          offset += ((eyeWidth / 3) + ((2 * eyeHeight) / 3)) *
          (resultsRank[vIndexCenter + 1].rank[ch][ (*rankList)[rank].rankIndex].p +
           resultsRank[vIndexCenter - 1].rank[ch][ (*rankList)[rank].rankIndex].p +
           resultsRank[vIndexCenter + 1].rank[ch][ (*rankList)[rank].rankIndex].n +
           resultsRank[vIndexCenter - 1].rank[ch][ (*rankList)[rank].rankIndex].n);
          //RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
          //              "Center += (1/3EW + 2/3EH) * (R + 1 + R-1 + L + 1 + L-1) = %d\n", offset);
          /*
          offset += (((2 * eyeWidth) / 3) + (eyeHeight / 3)) *
                    (results[vIndexCenter + 2].strobe[ch][(*rankList)[rank].rankIndex][strobe].p +
                     results[vIndexCenter - 2].strobe[ch][(*rankList)[rank].rankIndex][strobe].p +
                     results[vIndexCenter + 2].strobe[ch][(*rankList)[rank].rankIndex][strobe].n +
                     results[vIndexCenter - 2].strobe[ch][(*rankList)[rank].rankIndex][strobe].n);
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                        "Center += (2/3EW + 1/3EH) * (R + 2 + R-2 + L + 2 + L-2) = %d\n", offset);
          offset += eyeWidth *
                    (results[vIndexCenter + 3].strobe[ch][(*rankList)[rank].rankIndex][strobe].p +
                     results[vIndexCenter - 3].strobe[ch][(*rankList)[rank].rankIndex][strobe].p +
                     results[vIndexCenter + 3].strobe[ch][(*rankList)[rank].rankIndex][strobe].n +
                     results[vIndexCenter - 3].strobe[ch][(*rankList)[rank].rankIndex][strobe].n);
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                        "Center += EW * (R + 3 + R-3 + L + 3 + L-3) = %d\n", offset);
           */
          offset = offset / (((4 * eyeWidth) + (4 * eyeHeight)) / 2);
          //RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
          //              "Center = Center / (4*EW + 4*EH) / 2 = %d\n", offset);
#endif
          //
          // Convert results from absolute pi values to offset values and save the results
          //
          if (testCase == COMPOSITE_GROUPS) {
            groupEdgeData[ch][ (*rankList)[rank].rankIndex][0].group = CmdAll;
            groupEdgeData[ch][ (*rankList)[rank].rankIndex][0].le = resultsRank.rank[ch][ (*rankList)[rank].rankIndex].n;
            groupEdgeData[ch][ (*rankList)[rank].rankIndex][0].re = resultsRank.rank[ch][ (*rankList)[rank].rankIndex].p;
          } else {
            groupEdgeData[ch][ (*rankList)[rank].rankIndex][group - CmdGrp0].group = group;
            groupEdgeData[ch][ (*rankList)[rank].rankIndex][group - CmdGrp0].le = resultsRank.rank[ch][ (*rankList)[rank].rankIndex].n;
            groupEdgeData[ch][ (*rankList)[rank].rankIndex][group - CmdGrp0].re = resultsRank.rank[ch][ (*rankList)[rank].rankIndex].p;
          }

        } // rank loop
      } // dimm loop
    } // ch loop
  } // group loop

  RcDebugPrint (SDBG_MAX, "\n");

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    //
    // Evaluate results and write them
    //
    if (testCase == COMPOSITE_GROUPS) {
      status = (UINT32) SetCombinedCmdGroup (Host, socket, ch, DdrLevel, GSM_COMBINE_EDGES, CMD_GRP_DELAY, 1,
                          groupEdgeData[ch]);
    } else {
      status = (UINT32) SetCombinedCmdGroup (Host, socket, ch, DdrLevel, GSM_COMBINE_EDGES, CMD_GRP_DELAY, MAX_CMD,
                          groupEdgeData[ch]);
    }
  } // ch loop

  if (testCase == COMPOSITE_GROUPS) {
    RcDebugPrint (SDBG_MAX, "STOP_CMD_CLK_PER_GROUP_FINAL\n");
  } else {
    RcDebugPrint (SDBG_MAX, "STOP_CMD_CLK_PER_GROUP\n");
  }

  // Execute JEDEC reset/init sequence
  PrevDebugLevel = GetDebugLevel ();
  if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
    SetDebugLevel (SDBG_ERROR);
  }
  IO_Reset (Host, socket);
  JedecInitSequence (Host, socket, CH_BITMASK);
  SetDebugLevel (PrevDebugLevel);

  return SUCCESS;
} // PerCMDGroup


/**

  Executes early command/clock training

  @param Host - Pointer to sysHost

  @retval N/A

**/
UINT32
EarlyCmdClk (
  PSYSHOST  Host
  )
{
  UINT8                               socket;
  UINT8                               signal;
  GSM_CSN                             signaldecode;
  UINT8                               ch = 0;
  UINT8                               dimm = 0;
  UINT8                               rank;
  UINT8                               rankEnabled;
  UINT32                              PrevDebugLevel;
  struct channelNvram                 (*channelNvList)[MAX_CH];
  struct dimmNvram                    (*dimmNvList)[MAX_DIMM];
  struct ddrRank                      (*rankList)[MAX_RANK_DIMM];
  GSM_CSEDGE                          signalEdgeData[MAX_CH][MAX_RANK_CH][NUM_SIGNALS_TO_SWEEP];
  CHIP_ERLY_CMD_CK_STRUCT             chipErlyCmdCKStruct;
  UINT8                               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ZeroMem ((UINT8 *)&chipErlyCmdCKStruct, sizeof (chipErlyCmdCKStruct));

  socket = Host->var.mem.currentSocket;

  Host->var.mem.checkMappedOutRanks = 1;

  //
  // Return if this socket is disabled
  //

  if (IsSocketDimmPopulated (Host, socket) != TRUE) {
    return SUCCESS;
  }

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_EARLY_CMD_CLK, socket));
  channelNvList = GetChannelNvList (Host, socket);

  if (!IsMemFlowEnabled (EarlyCmdClkTraining)) {

    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Skipping Early Cmd/Clk Training\n");

    if (IsDcpmmPresentSocket (socket) == TRUE) {
      SetNormalCMDTiming (Host, socket);
    }

    return SUCCESS;
  }


#ifdef DEBUG_CODE_BLOCK
  AcquirePrintControl ();
#endif // DEBUG_CODE_BLOCK
  RcDebugPrint (SDBG_MEM_TRAIN, "START_PARITY_CMD_CLK\n");

  //
  // Enable C2 of NVMDIMM since C2 is accumulated by parity check
  //
  ConfigNvmDimmC2 (Host, socket, FALSE);

  // Chip Workaround
  OffsetCCC (Host, socket, 64, 1);

  //
  // Enable normal CMD timing
  //
  SetNormalCMDTiming (Host, socket);
  ToggleBcomFlopEn (Host, socket, 1);
  ToggleBcom (Host, socket, 0);
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    //
    // Turn on command/address output enable
    //
    TrnOnChipCMDADDOpEn (Host, socket, ch, &chipErlyCmdCKStruct);

    // Disable 3DS mode
    if ((*channelNvList)[ch].encodedCSMode == 2) {
      Toggle3DSMode (Host, socket, ch, 0);
      //Tracks 3DS mode to skip some CSR
      Host->nvram.mem.threeDsModeDisabled = 1;
    }
  } // ch loop

  IO_Reset (Host, socket);

  //
  // Enable parity checking at the DIMM
  //
  EnableParityChecking (Host, socket);

  //
  // Configure NVMCTLR to enable Early CMD training mode if necessary
  //
  EnableChipFNV (Host, socket, ch, &chipErlyCmdCKStruct);

  //
  // Loop to train PAR, ACT_N then the address signals
  //
  for (signal = 0; signal < NUM_SIGNALS_TO_SWEEP; signal++) {

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {

      signaldecode = signalsToSweep[signal];
      // for NVM DIMMs, convert the CID0/1 to S2/S3
      if (signaldecode == C0) {
        if (dimm == 0) {
          signaldecode = CS2_N; //dimm0 CID0
        } else {
          signaldecode = CS6_N; //dimm1 CID0
        }
      } else if (signaldecode == C1) {
        if (dimm == 0) {
          signaldecode = CS3_N; //dimm0 CID1
        } else {
          signaldecode = CS7_N; //dimm1 CID1
        }
      }

      for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {

        rankEnabled = 0;

        for (ch = 0; ch < MaxChDdr; ch++) {
          if ((*channelNvList)[ch].enabled == 0) {
            continue;
          }
          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          dimmNvList = GetDimmNvList (Host, socket, ch);
          if (rank >= (*dimmNvList)[dimm].numRanks) {
            continue;
          }

          rankEnabled = 1;

          break;
        } // ch loop

        //
        // Continue to the next rank if this one is not populated
        //
        if (rankEnabled == 0) {
          continue;
        }

        //
        // Get CMD margins for current rank and signal
        //
        if ((Host->nvram.mem.dimmTypePresent != UDIMM) && (Host->nvram.mem.dimmTypePresent != SODIMM) && (rank > 0)) {
          for (ch = 0; ch < MaxChDdr; ch++) {
            if ((*channelNvList)[ch].enabled == 0) {
              continue;
            }
            rankList = GetRankNvList (Host, socket, ch, dimm);
            if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
              continue;
            }
            dimmNvList = GetDimmNvList (Host, socket, ch);
            if (rank >= (*dimmNvList)[dimm].numRanks) {
              continue;
            }
            chipErlyCmdCKStruct.le[ch] = signalEdgeData[ch][ (*rankList)[0].rankIndex][signal].le;
            chipErlyCmdCKStruct.re[ch] = signalEdgeData[ch][ (*rankList)[0].rankIndex][signal].re;
          } //ch loop
        } else {
          GetCmdMarginsSweep (Host, socket, dimm, rank, signaldecode, chipErlyCmdCKStruct.le, chipErlyCmdCKStruct.re);
        }

        for (ch = 0; ch < MaxChDdr; ch++) {
          if ((*channelNvList)[ch].enabled == 0) {
            continue;
          }
          rankList = GetRankNvList (Host, socket, ch, dimm);
          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          dimmNvList = GetDimmNvList (Host, socket, ch);
          if (rank >= (*dimmNvList)[dimm].numRanks) {
            continue;
          }

          //
          // Convert results from absolute pi values to offset values and save the results
          //
          signalEdgeData[ch][ (*rankList)[rank].rankIndex][signal].signal = signaldecode;
          signalEdgeData[ch][ (*rankList)[rank].rankIndex][signal].le = chipErlyCmdCKStruct.le[ch];
          signalEdgeData[ch][ (*rankList)[rank].rankIndex][signal].re = chipErlyCmdCKStruct.re[ch];

        } // ch loop

      } // rank loop
    } // dimm loop

    RcDebugPrint (SDBG_MAX, "\n");
  } // signal loop

  //
  // Configure NVMCTLR to disable Early CMD training mode if necessary
  //
  SetAepTrainingModeSMB (Host,socket, DISABLE_TRAINING_STEP);

  DisableParityChecking (Host, socket);

  //
  // clear to insure we take the right path for setting final PI values
  //
  Host->var.mem.earlyCmdClkExecuted = 0;

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    //
    // Evaluate results and write them
    //
    SetCombinedCmdGroup (Host, socket, ch, DdrLevel, GSM_COMBINE_EDGES, CMDCTL_SIGNAL_DELAY, NUM_SIGNALS_TO_SWEEP,
      signalEdgeData[ch]);

    //
    // Turn off command/address output enable
    //
    TrnOffChipCMDADDOpEn (Host, socket, ch, &chipErlyCmdCKStruct);

    // Restore 3DS mode
    if ((*channelNvList)[ch].encodedCSMode == 2) {
      Toggle3DSMode (Host, socket, ch, 1);
      Host->nvram.mem.threeDsModeDisabled = 0;
    }
  } // ch loop

  //
  // Set to indicate early CMD/CLK training has execute
  //
  Host->var.mem.earlyCmdClkExecuted = 1;

  // Chip WorkAround
  OffsetCCC (Host, socket, -64, 0);

  // Execute JEDEC reset/init sequence
  FifoTrainReset (Host, socket);
  PrevDebugLevel = GetDebugLevel ();
  if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
    SetDebugLevel (SDBG_ERROR);
  }
  IO_Reset (Host, socket);
  ToggleBcom (Host, socket, 1);
  ToggleBcomFlopEn (Host, socket, 0);
  //
  // Turn on Core BCOM for channel with LRDIMM/NVMDimm to mitigate large cycle delays between the 2 dimms
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    //
    // If BWV present, leave in IO BCOM mode
    //
    if (GetFmcType (socket, ch) == FMC_BWV_TYPE) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, socket, ch);
    if ((Host->nvram.mem.socket[socket].ddrFreq >= DDR_2666) && ((*dimmNvList)[0].lrDimmPresent == 1) && ((*dimmNvList)[1].DcpmmPresent == 1)) {
      ToggleBcomFlopEnPerCh (Host, socket, ch, 1);
    }
  }
  JedecInitSequence (Host, socket, CH_BITMASK);
  SetDebugLevel (PrevDebugLevel);

#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    DisplayCCCResults (Host, socket);
  }
  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK

  //
  // Disable C2 of NVMDIMM
  //
  ConfigNvmDimmC2 (Host, socket, TRUE);

  Host->var.mem.checkMappedOutRanks = 0;

  return SUCCESS;
} // EarlyCmdClk

/**

  Executes RCD QxCA Training

  @param[in] Host   Pointer to sysHost, the system host (root) structure

  @retval SUCCESS   Training was successful

**/
UINT32
QxCAClk (
  IN  PSYSHOST  Host
  )
{
  UINT8           Socket;
  UINT8           Signal;
  GSM_CSN         SignalDecode;
  UINT8           Ch = 0;
  UINT8           Dimm = 0;
  UINT8           Rank;
  UINT8           RankEnabled;
  UINT32          PrevDebugLevel;
  struct channelNvram       (*ChannelNvList)[MAX_CH];
  struct dimmNvram          (*DimmNvList)[MAX_DIMM];
  struct ddrRank            (*RankList)[MAX_RANK_DIMM];
  GSM_CSEDGE                SignalEdgeData[MAX_CH][MAX_RANK_CH][BACKSIDE_SIGNAL_RANGE];
  CHIP_ERLY_CMD_CK_STRUCT   ChipErlyCmdCKStruct;
  UINT8  MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ZeroMem ((UINT8 *) &ChipErlyCmdCKStruct, sizeof (ChipErlyCmdCKStruct));

  Socket = Host->var.mem.currentSocket;

  Host->var.mem.checkMappedOutRanks = 1;

  //
  // Skip if RCD QxCA Training is disabled or system frequency is less than DDR-2933
  //
  if ((!IsMemFlowEnabled (BacksideCmdCtlTraining)) || (Host->nvram.mem.socket[Socket].ddrFreq < DDR_2933)) {
    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Skipping Early QxCA Cmd/Clk Training\n");
    return SUCCESS;
  }

  //
  // Return if this Socket is disabled
  //
  if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
    return SUCCESS;
  }

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_QX_CA_CLK, Socket));

  RcDebugPrint (SDBG_MEM_TRAIN, "START_QxCA_CLK\n");

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    //
    // Turn on command/address output enable
    //
    TrnOnChipCMDADDOpEn (Host, Socket, Ch, &ChipErlyCmdCKStruct);

    // Disable 3DS mode
    if ((*ChannelNvList)[Ch].encodedCSMode == 2) {
      Toggle3DSMode (Host, Socket, Ch, 0);
      //Tracks 3DS mode to skip some CSR
      Host->nvram.mem.threeDsModeDisabled = 1;
    }
  } // Ch loop

  //
  // Enable Parity in the Host
  //
  EnableCAParityRuntime (Host, Socket);

  PrevDebugLevel = GetDebugLevel ();

  //
  // Loop to train PAR, ACT_N, Address Signals, and Chip IDs
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }
      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      if (!A17EnabledBackside (Host, Socket, Ch, Dimm)) {
        WriteRC (Host, Socket, Ch, Dimm, 0, ((*DimmNvList)[Dimm].rcCache[RDIMM_RC08] | BIT3), RDIMM_RC08);
      }
    }

    for (Signal = 0; Signal < BACKSIDE_SIGNAL_RANGE; Signal++) {

      SignalDecode = SignalsToSweepBackside[Signal];

      for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

        RankEnabled = 0;

        for (Ch = 0; Ch < MaxChDdr; Ch++) {

          if (((*ChannelNvList)[Ch].enabled == 0) || (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT))) {
            continue;
          }

          DimmNvList = GetDimmNvList (Host, Socket, Ch);
          if (Rank >= (*DimmNvList)[Dimm].numRanks) {
            continue;
          }

          RankEnabled = 1;
          break;
        } // Ch loop

        //
        // Continue to the next rank if this one is not populated
        //
        if (RankEnabled == 0) {
          continue;
        }

        //
        // Get CMD margins for current rank and signal
        //
        GetCmdMarginsSweepBackside (Host, Socket, Dimm, Rank, SignalDecode, ChipErlyCmdCKStruct.le, ChipErlyCmdCKStruct.re);

        //
        // Disable parity checking in the RCD
        //
        if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
          SetDebugLevel (SDBG_ERROR);
        }
        DisableParityCheckingBackside (Host, Socket, Dimm, Rank);
        SetDebugLevel (PrevDebugLevel);

        for (Ch = 0; Ch < MaxChDdr; Ch++) {

          if (((*ChannelNvList)[Ch].enabled == 0) || (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT))) {
            continue;
          }

          DimmNvList = GetDimmNvList (Host, Socket, Ch);
          if (Rank >= (*DimmNvList)[Dimm].numRanks) {
            continue;
          }

          RankList = GetRankNvList (Host, Socket, Ch, Dimm);

          //
          // Convert results from absolute pi values to offset values and save the results
          //
          SignalEdgeData[Ch][ (*RankList)[Rank].rankIndex][Signal].signal = SignalDecode;
          SignalEdgeData[Ch][ (*RankList)[Rank].rankIndex][Signal].le = ChipErlyCmdCKStruct.le[Ch];
          SignalEdgeData[Ch][ (*RankList)[Rank].rankIndex][Signal].re = ChipErlyCmdCKStruct.re[Ch];
        } // Ch loop
      } // Rank loop
    } // Signal loop
  } // Dimm loop

  RcDebugPrint (SDBG_MAX, "\n");

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    //
    // Evaluate final results and write them
    //
    SetCombinedBacksideCmdGroup (Host, Socket, Ch, BACKSIDE_SIGNAL_RANGE, &SignalEdgeData);

    //
    // Turn off command/address output enable
    //
    TrnOffChipCMDADDOpEn (Host, Socket, Ch, &ChipErlyCmdCKStruct);

    //
    // Restore 3DS mode
    //
    if ((*ChannelNvList)[Ch].encodedCSMode == 2) {
      Toggle3DSMode (Host, Socket, Ch, 1);
      Host->nvram.mem.threeDsModeDisabled = 0;
    }
  } // Ch loop

  //
  // Execute JEDEC reset/init sequence
  //
  if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
    SetDebugLevel (SDBG_ERROR);
  }
  IO_Reset (Host, Socket);
  JedecInitSequence (Host, Socket, CH_BITMASK);
  SetDebugLevel (PrevDebugLevel);

  Host->var.mem.checkMappedOutRanks = 0;

  return SUCCESS;
} // QxCAClk

//------------------------------------------------------------------------------------------
struct cmdEye {
  INT16 left;
  INT16 right;
};


/**

  Finds if a give CID or CS is trained

  @param Host        - Pointer to sysHost
  @param socket      - Socket number
  @param dimm        - DIMM number
  @param signal      - Signal

  @retval N/A

**/
UINT32
isCmdCtlTrained (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  GSM_CSN   signal
  )
{
  UINT8           status;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];

  channelNvList = GetChannelNvList (Host, socket);
  dimmNvList = GetDimmNvList (Host, socket, ch);

  status = CHECKPOINT_MINOR_EARLY_CMD_CLK;

  // CTL signals use earlyCTLCLK for RDIMM or NVMDIMM
  if ((Host->nvram.mem.dimmTypePresent == RDIMM) || (*dimmNvList)[dimm].DcpmmPresent) {
    switch (signal) {
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
      status = CHECKPOINT_MINOR_EARLY_CTL_CLK;
      break;
    default:
      break;
    }
  }

  //NVMDIMM use earlyCmdClk
  if ((*dimmNvList)[dimm].DcpmmPresent) {
    switch (signal) {
    case CS2_N:
    case CS3_N:
    case CS6_N:
    case CS7_N:
    case C0:
    case C1:
    case C2:
      status = CHECKPOINT_MINOR_EARLY_CMD_CLK;
      break;
    default:
      break;
    }
  }

  // Standard LRDIMM trains C0 with earlyCtlClk, NOT NVMDIMM or 3DS
  if (((IsLrdimmPresent (socket, ch, dimm) || ((*dimmNvList)[dimm].numRanks == 4)) && ((*dimmNvList)[dimm].DcpmmPresent == 0) && (*channelNvList)[ch].encodedCSMode != 2)) {
    switch (signal) {
    case C0:
    case C1:
    case CS2_N:
    case CS3_N:
      if ((*dimmNvList)[dimm].numRanks == 2) {
        status = 0; // don't train DR LRDIMM
      } else {
        status = CHECKPOINT_MINOR_EARLY_CTL_CLK;
      }
      break;
    case CS6_N:
    case CS7_N:
      status = CHECKPOINT_MINOR_EARLY_CTL_CLK;
      break;
    case C2:
      status = 0;
      break;
    default:
      break;
    }
  }

  // 3DS LRDIMM or RDIMM, carefull this a channel level so we could have 3DS with NVMDIMM
  if (((*channelNvList)[ch].encodedCSMode == 2) && ((*dimmNvList)[dimm].DcpmmPresent == 0)) {

    if ((signal == C1) ||
        (signal == CS3_N) ||
        (signal == CS7_N))
    {
      //if 1DPC Only 2H
      if (((*channelNvList)[ch].maxDimm == 1) &&
          (((*dimmNvList)[dimm].cidBitMap & BIT1) == 0))
      {
        status = 0; // Do not train it
      }
      //if 2DPC both 2H
      else if (((*channelNvList)[ch].maxDimm == 2) &&
               (((*dimmNvList)[0].cidBitMap & BIT1) == 0) &&
               (((*dimmNvList)[1].cidBitMap & BIT1) == 0))
      {
        status = 0; // Do not train it
      }
    }

    if ((signal == C2) &&
        (((*dimmNvList)[dimm].cidBitMap & BIT2) == 0))
    {
      status = 0; // Do not train it
    }

  }

  // SKIP CID's or upper CS#  if RDIMM / UDIMM (NOT LRDIMM and NOT NVMDIMM and NOT 3DS)
  if ((IsLrdimmPresent (socket, ch, dimm) == 0) && ((*dimmNvList)[dimm].DcpmmPresent == 0) && ((*channelNvList)[ch].encodedCSMode != 2) &&
      ((*dimmNvList)[dimm].numRanks < 4)) {
    switch (signal) {
    case C0:
    case C1:
    case C2:
    case CS2_N:
    case CS3_N:
    case CS6_N:
    case CS7_N:
      status = 0;
      break;
    default:
      break;
    }
  }

  if ((signal == A17) && !A17Enabled (Host, socket, ch, dimm)) {
    //
    // Skip A17 testing if the DIMM doesn't support it
    //
    status = 0;
  }

  if ((*channelNvList)[ch].features & X16_PRESENT) {
    switch (signal) {
    case BG1:
      status = 0;
      break;
    default:
      break;
    }
  }

  return status;
}

/**

  Finds if a give CID or CS is trained for Backside

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket number
  @param[in] Ch          - Channel number
  @param[in] Dimm        - DIMM number
  @param[in] Signal      - Signal

  @retval Status

**/
UINT32
isCmdCtlTrainedBackside (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  GSM_CSN   Signal
  )
{
  UINT8                Status;
  struct channelNvram  (*ChannelNvList)[MAX_CH];
  struct dimmNvram     (*DimmNvList)[MAX_DIMM];

  ChannelNvList = GetChannelNvList (Host, Socket);
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  Status = CHECKPOINT_MINOR_QX_CA_CLK;

  //
  // Check if A17 should be trained
  //
  if ((Signal == A17) &&
      !A17EnabledBackside(Host, Socket, Ch, Dimm))
  {
    Status = 0; //Don't train signal
  }

  if ((*ChannelNvList)[Ch].encodedCSMode == 2) {
    //
    // 3DS DIMM - QxCA trains CID (C0, C1 and C2) if used by the DIMM
    // C0 is always used, so just check if C1 and C2 are used.
    //
    if ((Signal == C1) &&
        (((*DimmNvList)[Dimm].cidBitMap & BIT1) == 0))
    {
      Status = 0; //Don't train signal
    }
    if ((Signal == C2) &&
        (((*DimmNvList)[Dimm].cidBitMap & BIT2) == 0))
    {
      Status = 0; //Don't train signal
    }
  } else {
    //
    // Skip CID for non-3DS DIMM
    //
    if ((Signal == C0) ||
        (Signal == C1) ||
        (Signal == C2))
    {
      Status = 0; //Don't train signal
    }

  }

  return Status;
}


/**

  Finds command margins for the given signal

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
GetCmdMarginsSweep (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     dimm,
  UINT8     rank,
  GSM_CSN   signal,
  INT16     *le,
  INT16     *re
  )
{
  UINT8               ch;
  UINT8               executeTest;
  INT16               piIndex;
  INT16               offset;
  INT16               orgDelay[MAX_CH];
  UINT32              rankMask[MAX_CH];
  UINT8               errorResult[MAX_CH][PAR_TEST_RANGE];
  UINT16              piPosition[MAX_CH];
  UINT32               chBitmask = 0;
  UINT32              bwSerr[MAX_CH][3];
  UINT32              status = 0;
  UINT32              PrevDebugLevel;
  struct cmdEye       curEye[MAX_CH];
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  CHIP_GET_CMD_MGN_SWP_STRUCT chipGetCmdMgnSwpStruct;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  GetCmdMarginsSweepInit (&chipGetCmdMgnSwpStruct);

  channelNvList = GetChannelNvList (Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    rankMask[ch] = 0;
    orgDelay[ch] = 0;

    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
      continue;
    }
    dimmNvList = GetDimmNvList (Host, socket, ch);
    if (rank >= (*dimmNvList)[dimm].numRanks) {
      continue;
    }

    // need to initialize these regardless if signal is trained.
    //curEye[ch].left = -UNMARGINED_CMD_EDGE;
    //curEye[ch].right = UNMARGINED_CMD_EDGE;
    re[ch] = UNMARGINED_CMD_EDGE; // UNMARGINED_CMD_EDGE = 511
    le[ch] = -UNMARGINED_CMD_EDGE;

    // Skip CIDS if not NVMDIMM
    if (isCmdCtlTrained (Host, socket, ch, dimm, signal) != (UINT32) CHECKPOINT_MINOR_EARLY_CMD_CLK) {
      continue;
    }

    // Initialize ch rank mask
    rankMask[ch] = 1;

    //
    // Get the original delay
    //
    GetSetSignal (Host, socket, ch, DdrLevel, signal, GSM_READ_ONLY, &orgDelay[ch]);

    SetMem (errorResult[ch], PAR_TEST_RANGE, 1);

    //
    // Setup for parity test
    //
    CpgcAdvCmdParity (Host, socket, ch, GetPhyRank (dimm, rank), 0, signal, NormalCadbSetup);

    chipGetCmdMgnSwpStruct.eyeSize[ch] = 0;
    chipGetCmdMgnSwpStruct.chEyemask |= 1 << ch;
    chipGetCmdMgnSwpStruct.chDonemask |= 1 << ch;

    // Indicate rank present and enabled on this ch
    chBitmask |= 1 << ch;
  } // ch loop

  offset = EARLY_CMD_STEP_SIZE;
  PrevDebugLevel = GetDebugLevel ();

  //
  // Sweep Pi delays
  //  The following code assumes PAR_TEST_START is always <= the absolute minimum CMD setting
  //  and PAR_TEST_RANGE + PAR_TEST_START is always < the maximum CMD setting
  //
  for (piIndex = PAR_TEST_START; piIndex < (PAR_TEST_RANGE + PAR_TEST_START); piIndex += offset) {

    if (chBitmask == 0) {
      continue;
    }

    // Shift timings on each channel
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((rankMask[ch] == 0)) {
        continue;
      }

      GetSetSignal (Host, socket, ch, DdrLevel, signal, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &piIndex);
      //
      //Call RestoreEarlyCmdPatterns or CpgAdvCmdParity depending on chip and depending on if the flow is RC_SIM or not
      //
      CpgcAdvCmdParity (Host, socket, ch, GetPhyRank (dimm, rank), 0, signal, RestoreCadbPattern);
    } // ch loop

    //
    // Generate an IO Reset. This is required because the pi delays changed.
    //
    IO_Reset (Host, socket);

    //
    // Run concurrent tests
    //
    EarlyCmdSignalTest (Host, socket, chBitmask);

    ClearBwSerr (bwSerr);

    //
    // Check for errors
    //
    status = CollectParityResults (Host, socket, chBitmask, dimm, bwSerr);

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((rankMask[ch] == 0)) {
        continue;
      }
      dimmNvList = GetDimmNvList (Host, socket, ch);
      if (rank >= (*dimmNvList)[dimm].numRanks) {
        continue;
      }

      if (bwSerr[ch][0] || bwSerr[ch][1] || bwSerr[ch][2]) {
        errorResult[ch][piIndex] = 1;
      } else {
        errorResult[ch][piIndex] = 0;
      }


      UpdateCmdMarginMasks (Host, socket, ch, piIndex, offset, errorResult, &chipGetCmdMgnSwpStruct);
    } // ch loop

    //
    // Clear errors if a parity error occurred
    //
    if (ShouldChipClearCMDMGNParityError (Host, status)) {

      //
      // Restore initial setting
      //
      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((rankMask[ch] == 0)) {
          continue;
        }
        dimmNvList = GetDimmNvList (Host, socket, ch);
        if (rank >= (*dimmNvList)[dimm].numRanks) {
          continue;
        }

        GetSetSignal (Host, socket, ch, DdrLevel, signal, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &orgDelay[ch]);
      } // ch loop

      //
      // Generate an IO Reset. This is required because the pi delays changed.
      //
      IO_Reset (Host, socket);

      //
      // Clear parity error status in the iMC and at the DIMM
      //
      if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
        SetDebugLevel (SDBG_ERROR);
      }
      ClearParityResultsAlt (Host, socket, chBitmask, rank, dimm);
      SetDebugLevel (PrevDebugLevel);

      //
      // Restore current offset
      //
      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((rankMask[ch] == 0)) {
          continue;
        }
        dimmNvList = GetDimmNvList (Host, socket, ch);
        if (rank >= (*dimmNvList)[dimm].numRanks) {
          continue;
        }

        if (signal == PAR) {
          //
          // Setup the test
          //
          CpgcAdvCmdParity (Host, socket, ch, GetPhyRank (dimm, rank), 0, signal, NormalCadbSetup);
        }
      } // ch loop
    } // if status

    //
    // Issue a precharge unless testing PAR
    //
    executeTest = 0;
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((rankMask[ch] == 0)) {
        continue;
      }
      dimmNvList = GetDimmNvList (Host, socket, ch);
      if (rank >= (*dimmNvList)[dimm].numRanks) {
        continue;
      }

      //
      // No need to do anything if NVMDIMM is not present and this is a parity signal
      //
      if (((*channelNvList)[ch].ddrtEnabled == 0) && (signal == PAR)) {
        continue;
      }

      if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
        if (signal == PAR) {
          //
          // Do nothing for PAR Signal
          //
          CpgcAdvCmdParity (Host, socket, ch, GetPhyRank (dimm, rank), 0, DESELECT, NormalCadbSetup);
        } else {
          //
          // Setup the test for a precharge
          //
          CpgcAdvCmdParity (Host, socket, ch, GetPhyRank (dimm, rank), 0, PRECHARGE, NormalCadbSetup);
          executeTest = 1;
        }
      } else {
        if (status) {
          //
          //NVMCTLR with Good Parity
          //
          CpgcAdvCmdParity (Host, socket, ch, GetPhyRank (dimm, rank), 0, FNV_GOOD_PARITY, NormalCadbSetup);
          executeTest = 1;
        } else {
          //
          //Deselect Pattern
          //
          CpgcAdvCmdParity (Host, socket, ch, GetPhyRank (dimm, rank), 0, DESELECT, NormalCadbSetup);
        }
      }
    } // ch loop

    // Issue precharge
    if (executeTest) {
      ExecuteCmdSignalTest (Host, socket, chBitmask, NO_SUBCH);
    }

    if (CheckChDoneMask (Host, &chipGetCmdMgnSwpStruct) == 0) {
      piIndex = PAR_TEST_RANGE;
    }
  } // piIndex loop

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((rankMask[ch] == 0)) {
      continue;
    }
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                  "%5a -> ", GetSignalStr (signal));
    for (piIndex = 0; piIndex < PAR_TEST_RANGE; piIndex++) {

      dimmNvList = GetDimmNvList (Host, socket, ch);
      if (rank >= (*dimmNvList)[dimm].numRanks) {
        continue;
      }

      if (errorResult[ch][piIndex]) {
        // Fail
        RcDebugPrint (SDBG_MAX, "*");
      } else {
        // Pass
        RcDebugPrint (SDBG_MAX, "0");
      }
    } // piIndex loop

    RcDebugPrint (SDBG_MAX, "\n");
  } // ch loop

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((rankMask[ch] == 0)) {
      continue;
    }

    status = FindErrorGraphMin (Host, socket, ch, dimm, rank, 0, &errorResult[ch][0], PAR_TEST_RANGE,
               (UINT16 *)&curEye[ch].right, (UINT16 *)&curEye[ch].left, (UINT16 *)&piPosition[ch]);

    if ((status == FAILURE) || (curEye[ch].left == curEye[ch].right)) {
      curEye[ch].left = 0;
      curEye[ch].right = 0;
    }


    //
    // Offset results relative to the starting point of the sweep
    //
    curEye[ch].right = curEye[ch].right - orgDelay[ch];
    curEye[ch].left = curEye[ch].left - orgDelay[ch];

    //
    // Convert edges to be CLK based
    //
    //curEye[ch].right = 0 - curEye[ch].right;
    //curEye[ch].left = 0 - curEye[ch].left;

  } // ch loop

  //
  // Restore initial pi delay
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((rankMask[ch] == 0)) {
      continue;
    }

    // Update return parameters
    re[ch] = curEye[ch].right;
    le[ch] = curEye[ch].left;

    if ((re[ch] - le[ch]) <= 0) {
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                    "ERROR: Signal %a, No eye found.\n", GetSignalStr (signal));
      le[ch] = 0;
      re[ch] = 0;
    }
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                  "le = %d - re = %d\n", le[ch], re[ch]);

    //
    // Restore signal under test to its initial setting
    //
    GetSetSignal (Host, socket, ch, DdrLevel, signal, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &orgDelay[ch]);
  } // ch loop

} // GetCmdMarginsSweep

/**

  Finds command margins for the given signal

  @param[in]  Host         Pointer to sysHost, the system host (root) structure
  @param[in]  Socket       Socket Id
  @param[in]  Dimm         DIMM number
  @param[in]  Rank         Rank number
  @param[in]  Signal       Signal
  @param[out] LeftEdge     Left edge
  @param[out] RightEdge    Right edge

  @retval None

**/
VOID
GetCmdMarginsSweepBackside (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Dimm,
  IN  UINT8     Rank,
  IN  GSM_CSN   Signal,
  OUT INT16     *LeftEdge,
  OUT INT16     *RightEdge
  )
{
  UINT8               Ch;
  UINT8               Channel;
  INT16               PiIndex;
  INT16               QxCAPiDelay;
  INT16               Offset;
  UINT32              RankMask[MAX_CH];
  UINT8               ErrorResult[MAX_CH][PAR_TEST_RANGE];
  UINT16              PiPosition[MAX_CH];
  UINT32              ChBitmask = 0;
  UINT32              BwSerr[MAX_CH][3];
  UINT32              ParityErrorStatus = 0;
  UINT32              Status;
  UINT16              MinVal = 0xFF;
  UINT16              MaxVal = 0;
  UINT32              PrevDebugLevel;
  struct cmdEye       CurEye[MAX_CH];
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  CHIP_GET_CMD_MGN_SWP_STRUCT ChipGetCmdMgnSwpStruct;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  GetCmdMarginsSweepInit (&ChipGetCmdMgnSwpStruct);

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    RankMask[Ch] = 0;

    if (((*ChannelNvList)[Ch].enabled == 0) || (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT))) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }

    //
    // Cannot run RCD QxCA Training on UDIMMs, DDRT DIMMs, or RCD_REV 1 DIMMs
    // UDIMM - There is no QxCa interface because there is no register
    // DDRT - There is no QxCa interface because the FMC serves as register and DRAM roles
    //
    if ((Host->nvram.mem.dimmTypePresent == UDIMM) ||
        (Host->nvram.mem.dimmTypePresent == SODIMM) ||
        ((*DimmNvList)[Dimm].DcpmmPresent == 1) ||
        ((*DimmNvList)[Dimm].DimmRevType < RCD_REV_2))
    {
      continue;
    }

    if (isCmdCtlTrainedBackside (Host, Socket, Ch, Dimm, Signal) != (UINT32)CHECKPOINT_MINOR_QX_CA_CLK) {
      //
      // Populate structure with known values for the skipped signal
      //
      for (Channel = 0; Channel < MaxChDdr; Channel++) {
        if ((*ChannelNvList)[Channel].enabled == 0) {
          continue;
        }
        RightEdge[Channel] = BACKSIDE_SWEEP_RANGE - 1;
        LeftEdge[Channel] = 0;
      } // Channel loop

      continue;
    }

    if (Rank >= (*DimmNvList)[Dimm].numRanks) {
      continue;
    }

    //
    // need to initialize these regardless if signal is trained.
    //
    RightEdge[Ch] = BACKSIDE_SWEEP_END;
    LeftEdge[Ch] = BACKSIDE_SWEEP_START;

    //
    // Initialize ch rank mask
    //
    RankMask[Ch] = 1;

    //
    // Initialze ErrorResults to all failure
    //
    SetMem (&ErrorResult, sizeof (ErrorResult), 1);

    //
    // Setup for parity test
    //
    CpgcAdvCmdParity (Host, Socket, Ch, GetPhyRank (Dimm, Rank), 0, Signal, BacksideCadbSetup);

    ChipGetCmdMgnSwpStruct.eyeSize[Ch] = 0;
    ChipGetCmdMgnSwpStruct.chEyemask |= 1 << Ch;
    ChipGetCmdMgnSwpStruct.chDonemask |= 1 << Ch;

    //
    // Indicate rank present and enabled on this ch
    //
    ChBitmask |= 1 << Ch;
  } // Ch loop

  Offset = BACKSIDE_STEP_SIZE;
  PrevDebugLevel = GetDebugLevel ();

  //
  //  Sweep Pi delays
  //  The following code assumes BACKSIDE_SWEEP_START is always <= the absolute minimum CMD setting
  //  and BACKSIDE_SWEEP_RANGE + BACKSIDE_SWEEP_START is always < the maximum CMD setting
  //
  for (PiIndex = BACKSIDE_SWEEP_START; PiIndex <= BACKSIDE_SWEEP_END; PiIndex += BACKSIDE_STEP_SIZE) {
    if (ChBitmask == 0) {
      continue;
    }

    //
    // If index is <0, shift out CLK and QxCS, then sweep QxCa from 0 to 31
    // Else, Sweep QxCa from 0 to 31 without shifting CLK and QxCs
    // Reenable Parity Checking after denormalizing/renormalizing
    //
    if ((PiIndex == BACKSIDE_SWEEP_START) && (PiIndex < 0)) {
      EarlyBacksideShift (Host, Socket, DENORMALIZE);
    } else if (PiIndex == 0) {
      EarlyBacksideShift (Host, Socket, RENORMALIZE);
    }

    //
    // Calculating QxCa for Sweep
    //
    if (PiIndex < 0) {
      QxCAPiDelay = BACKSIDE_SWEEP_END + PiIndex;
    } else {
      QxCAPiDelay = PiIndex;
    }

    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if (((ChBitmask & (1 << Ch)) == 0) || (RankMask[Ch] == 0)) {
        continue;
      }

      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      if (Rank >= (*DimmNvList)[Dimm].numRanks) {
        continue;
      }

      //
      // Delaying QxCa for Sweep
      // Disabling messages to surpress RCW prints
      //
      if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
        SetDebugLevel (SDBG_ERROR);
      }
      GetSetCmdGroupDelayCore (Host, Socket, Ch, Dimm, LrbufLevel, CmdAll, GSM_FORCE_WRITE, &QxCAPiDelay, &MinVal, &MaxVal);
      SetDebugLevel (PrevDebugLevel);
    } // Ch loop

    //
    // When shifting QxCa back to zero a parity error could be created therefore, clear parity
    //
    if (QxCAPiDelay == 0) {
      if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
        SetDebugLevel (SDBG_ERROR);
      }
      EnableParityCheckingBackside (Host, Socket, Dimm, Rank);
      ClearBacksideParityResults (Host, Socket, ChBitmask, Rank, Dimm);
      SetDebugLevel (PrevDebugLevel);
    }

    //
    // Restoring CADB test pattern after Delaying QxCA reprogrammed CPGC
    //
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }

      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      if (!(ChBitmask & (1 << Ch))) {
        //
        // Use DESELECT to make sure you don't send any patterns to UDIMMs, DDRT DIMMs, or RCD_REV 1 DIMM
        //
        CpgcAdvCmdParity (Host, Socket, Ch, GetPhyRank (Dimm, Rank), 0, DESELECT, BacksideCadbSetup);
      } else {
        CpgcAdvCmdParity (Host, Socket, Ch, GetPhyRank (Dimm, Rank), 0, Signal, BacksideCadbSetup);
      }
    }

    //
    // Run concurrent tests
    //
    EarlyCmdSignalTest (Host, Socket, ChBitmask);
    ClearBwSerr (BwSerr);

    //
    // Check for errors
    //
    ParityErrorStatus = CollectParityResults (Host, Socket, ChBitmask, Dimm, BwSerr);
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if (((ChBitmask & (1 << Ch)) == 0) || (RankMask[Ch] == 0)) {
        continue;
      }

      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      if (Rank >= (*DimmNvList)[Dimm].numRanks) {
        continue;
      }

      if (BwSerr[Ch][0] || BwSerr[Ch][1] || BwSerr[Ch][2]) {
        ErrorResult[Ch][PiIndex + BACKSIDE_SWEEP_END] = 1;
      } else {
        ErrorResult[Ch][PiIndex + BACKSIDE_SWEEP_END] = 0;
      }


      UpdateCmdMarginMasks (Host, Socket, Ch, PiIndex + BACKSIDE_SWEEP_END, Offset, ErrorResult, &ChipGetCmdMgnSwpStruct);
    } // Ch loop

    //
    // Restore initial setting before clearing Parity or setting new QxCa setting. Do not want the setting of new QxCA value to cause Parity error
    //
    for (Ch = 0; Ch < MaxChDdr; Ch++) {

      if (((ChBitmask & (1 << Ch)) == 0) || (RankMask[Ch] == 0)) {
        continue;
      }

      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      if (Rank >= (*DimmNvList)[Dimm].numRanks) {
        continue;
      }

      if (PiIndex<0) {
        QxCAPiDelay = BACKSIDE_SWEEP_END;
      } else {
        QxCAPiDelay = 0;
      }

      if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
        SetDebugLevel (SDBG_ERROR);
      }
      GetSetCmdGroupDelayCore (Host, Socket, Ch, Dimm, LrbufLevel, CmdAll, GSM_FORCE_WRITE, &QxCAPiDelay, &MinVal, &MaxVal);
      SetDebugLevel (PrevDebugLevel);

    } // Ch loop

    //
    // Clear errors if a parity error occurred
    //
    if (ShouldChipClearCMDMGNParityError (Host, ParityErrorStatus)) {
      //
      // Clear parity error status in the iMC and at the DIMM
      //
      if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
        SetDebugLevel (SDBG_ERROR);
      }
      ClearBacksideParityResults (Host, Socket, ChBitmask, Rank, Dimm);
      SetDebugLevel (PrevDebugLevel);
    }

    //
    // Issue a PREA for ACT, A14, A15, A16
    //
    if ((Signal == A14 || Signal == A15 || Signal == A16 || Signal == ACT_N)) {
      for (Ch = 0; Ch < MaxChDdr; Ch++) {

        if ((RankMask[Ch] == 0)) {
          continue;
        }

        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if (Rank >= (*DimmNvList)[Dimm].numRanks) {
          continue;
        }

        if (!(ChBitmask & (1 << Ch))) {
          //
          // Use DESELECT for populated slots that are not RDIMMs
          //
          CpgcAdvCmdParity (Host, Socket, Ch, GetPhyRank (Dimm, Rank), 0, DESELECT, BacksideCadbSetup);
        } else {
          CpgcAdvCmdParity (Host, Socket, Ch, GetPhyRank (Dimm, Rank), 0, PRECHARGE, BacksideCadbSetup);
        }
      } // Ch loop
      //
      // Issue PREA
      //
      ExecuteCmdSignalTest (Host, Socket, ChBitmask, NO_SUBCH);
    }
    if (CheckChDoneMask (Host, &ChipGetCmdMgnSwpStruct) == 0) {
      PiIndex = (BACKSIDE_SWEEP_RANGE + BACKSIDE_SWEEP_START);
    }
  } // PiIndex loop

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((RankMask[Ch] != 0)) {
      RcDebugPrint (SDBG_MAX, "\n");
      break;
    }
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (RankMask[Ch] == 0) {
      continue;
    }
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "%5a -> ", GetSignalStr (Signal));

    for (PiIndex = 0; PiIndex < (BACKSIDE_SWEEP_RANGE - 1); PiIndex++) {
      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      if (Rank >= (*DimmNvList)[Dimm].numRanks) {
        continue;
      }

      if (ErrorResult[Ch][PiIndex]) {
        //
        // Fail
        //
        RcDebugPrint (SDBG_MAX, "*");
      } else {
        //
        // Pass
        //
        RcDebugPrint (SDBG_MAX, "0");
      }
    } // PiIndex loop

    RcDebugPrint (SDBG_MAX, "\n");
  } // Ch loop

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (RankMask[Ch] == 0) {
      continue;
    }

    Status = FindErrorGraphMin (Host, Socket, Ch, Dimm, Rank, 0, &ErrorResult[Ch][0], BACKSIDE_SWEEP_RANGE,
               (UINT16 *) &CurEye[Ch].right, (UINT16 *) &CurEye[Ch].left, (UINT16 *) &PiPosition[Ch]);

    if ((Status == FAILURE) || (CurEye[Ch].left == CurEye[Ch].right)) {
      CurEye[Ch].left = 0;
      CurEye[Ch].right = 0;
    }
  } // Ch loop

  //
  // Check if eye found
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (RankMask[Ch] == 0) {
      continue;
    }

    //
    // Update return parameters
    //
    RightEdge[Ch] = CurEye[Ch].right;
    LeftEdge[Ch] = CurEye[Ch].left;

    if ((RightEdge[Ch] - LeftEdge[Ch]) <= 0) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "ERROR: Signal %a, No eye found.\n", GetSignalStr (Signal));
      LeftEdge[Ch] = 0;
      RightEdge[Ch] = 0;
      EwlOutputType3 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_QxCA_CLK_NO_EYE_FOUND, Socket, Ch, Dimm, Rank,
        EwlSeverityWarning, CmdAll, DdrLevel, Signal, 0);
      DisableChannelSw(Host, Socket, Ch);
    }
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "LeftEdge = %d - RightEdge = %d\n", LeftEdge[Ch], RightEdge[Ch]);
  } // Ch loop

} // GetCmdMarginsSweepBackside

/**

  Finds command margins for the given group

  @param Host        - Pointer to sysHost
  @param socket      - Socket number
  @param dimm        - DIMM number
  @param rank        - rank number
  @param group       - group to sweep
  @param *le         - Left edge
  @param *re         - Right edge

  @retval N/A

**/
VOID
OffsetGroups (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  MRC_GT    group,
  INT16     offset
  )
{
  UINT16 minVal;
  UINT16 maxVal;

  //
  // Check if we are moving all CMD and CTL groups
  //
  if (group == CmdCtlAll) {
    minVal = MAX_CMD_MARGIN;
    maxVal = 0;

    GetSetCmdGroupDelay (Host, socket, ch, CmdAll, GSM_WRITE_OFFSET | GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &offset, &minVal, &maxVal);

    minVal = MAX_CMD_MARGIN;
    maxVal = 0;

    GetSetCtlGroupDelay (Host, socket, ch, CtlAll, GSM_WRITE_OFFSET | GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &offset, &minVal, &maxVal);
  }

  //
  // Check if we are moving individual CMD groups
  //
  if (CheckCMDGroup (group)) {
    minVal = MAX_CMD_MARGIN;
    maxVal = 0;

    GetSetCmdGroupDelay (Host, socket, ch, group, GSM_WRITE_OFFSET | GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &offset, &minVal, &maxVal);
  }

  //
  // Check if we are moving individual CTL groups
  //
  if (CheckCTLGroup (group)) {
    minVal = MAX_CMD_MARGIN;
    maxVal = 0;

    GetSetCtlGroupDelay (Host, socket, ch, group, GSM_WRITE_OFFSET | GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &offset, &minVal, &maxVal);
  }
} // OffsetGroups

//------------------------------------------------------------------------------------------
/**

  Updates the composite edges

  @param le      - Left edge
  @param re      - Right edge
  @param *compLe - Composite left edge
  @param *compRe - Composite right edge

  @retval N/A

**/
VOID
UpdateEdges (
  INT16 le,
  INT16 re,
  INT16 *compLe,
  INT16 *compRe
  )
{
  if (le > *compLe) {
    *compLe = le;
  }
  if (re < *compRe) {
    *compRe = re;
  }


} // UpdateEdges

//-----------------------------------------------------------------------------------
/**
  Updates provided min and max values based on an unsigned input

  @param [in    ] Value  - New value to evaluate
  @param [in/out] MinVal - Current minimum value
  @param [in/out] MaxVal - Current maximum value

  @retval None
**/
VOID
UpdateMinMaxUint (
  IN     UINT16 Value,   // New value to evaluate
  IN OUT UINT16 *MinVal, // Current minimum value
  IN OUT UINT16 *MaxVal  // Current maximum value
  )
{
  if (Value < *MinVal) {
    *MinVal = Value;
  }
  if (Value > *MaxVal) {
    *MaxVal = Value;
  }
} // UpdateMinMaxUint

//-----------------------------------------------------------------------------------
/**
  Updates minimum and maximum variables based on a signed input

  @param val     - New value to evaluate
  @param minVal  - Current minimum value
  @param maxVal  - Current maximum value

  @retval N/A
**/
VOID
UpdateMinMaxInt (
  INT16 val,
  INT16 *minVal,
  INT16 *maxVal
  )
{
  if (val < *minVal) {
    *minVal = val;
  }
  if (val > *maxVal) {
    *maxVal = val;
  }
} // UpdateMinMaxInt

/**

  Updates hi and low values based on a value and a limit

  @param val     - Value to evaluate
  @param limit   - Limit
  @param *loVal  - Low value
  @param *hiVal  - High value
  @retval N/A

**/
VOID
UpdateIoDelayLimit (
  UINT16  val,
  UINT16  limit,
  UINT16  *loVal,
  UINT16  *hiVal
  )
{
  if (val < *loVal) {
    *loVal = val;
  }

  if (limit - val < *hiVal) {
    *hiVal = limit - val;
  }
} // UpdateIoDelayLimit

UINT32
CollectParityResults (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    chToCheck,
  UINT8 dimm, //DBG
  UINT32    bwSerr[MAX_CH][3]
  )
/*++
  Collects the results of the previous parity based test

  @param Host:         Pointer to sysHost
  @param socket:         Processor socket to check

  @retval status:       Non-zero if any failure was detected
--*/
{
  return ChipCollectParityResults (Host, socket, chToCheck, dimm, bwSerr);
}

/**
  Clears bwSerr

  @param Host:         Pointer to sysHost
  @param bwSerr:       Error status

  @retval status:       Non-zero if any failure was detected
**/
VOID
ClearBwSerr (
  UINT32    bwSerr[MAX_CH][3]
  )
{
  UINT8 ch;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  for (ch = 0; ch < MaxChDdr; ch++) {
    bwSerr[ch][0] = 0;
    bwSerr[ch][1] = 0;
    bwSerr[ch][2] = 0;
  } // ch loop
} // ClearBwSerr

VOID
ClearParityResults (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32     chToCheck,
  UINT8 dimm
  )
/*++
  Clears the results of the previous parity based test

  @param Host:         Pointer to sysHost
  @param socket:         Processor socket to check

  @retval status:       Non-zero if any failure was detected
--*/
{
  ChipClearParityResults (Host, socket, chToCheck, dimm);
}

VOID
ClearParityResultsAlt (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    chToCheck,
  UINT8     rank,
  UINT8     dimm
  )
/*++
  Clears the results of the previous parity based test

  @param Host:         Pointer to sysHost
  @param socket:       Processor socket to check
  @param rank:         Rank to clear
  @retval status:       Non-zero if any failure was detected
--*/
{
  ChipClearParityResultsAlt (Host, socket, chToCheck, rank, dimm);
}

/**

  Clears the results of the previous parity based test during RCD QxCA Training

  @param[in]  Host        Pointer to sysHost, the system host (root) structure
  @param[in]  Socket      CPU Socket Node number (Socket ID)
  @param[in]  ChToCheck   Channel number (0-based)
  @param[in]  Rank        Current rank
  @param[in]  Dimm        Current dimm

  @retval None

**/
VOID
ClearBacksideParityResults (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT32    ChToCheck,
  IN  UINT8     Rank,
  IN  UINT8     Dimm
  )
{
  ChipClearBacksideParityResults (Host, Socket, ChToCheck, Rank, Dimm);
}

//------------------------------------------------------------------------------------------

/**

  Returns the logical rank number of DDR4.

  @param[in] Host   - Pointer to sysHost.
  @param[in] socket - Processor socket to check.
  @param[in] ch     - channel number.
  @param[in] dimm   - DIMM number.
  @param[in] rank   - rank number.

  @retval Logical rank number.

**/
UINT8
GetLogicalRankDDR4 (
  IN PSYSHOST  Host,
  IN UINT8     socket,
  IN UINT8     ch,
  IN UINT8     dimm,
  IN UINT8     rank
  )
{
  UINT8 logRank;

  logRank = (dimm << 2) + rank;
  return logRank;

} // GetLogicalRank

/**
  Returns the logical rank number of DDR5 DIMM.

  @param[in] Host   - Pointer to sysHost.
  @param[in] Socket - Processor socket to check.
  @param[in] Ch     - Channel number.
  @param[in] Dimm   - DIMM number.
  @param[in] Rank   - Rank number.

  @retval Logical rank number.

**/
UINT8
GetLogicalRankDDR5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
  UINT8 LogicalRank;

  LogicalRank = (Dimm * MAX_RANK_DIMM) + (Rank & 1);

  return LogicalRank;

}


/**

  Returns the logical rank number.

  @param[in] Host   - Pointer to sysHost
  @param[in] Socket - Processor socket to check
  @param[in] Ch     - Channel number
  @param[in] Dimm   - DIMM number
  @param[in] Rank   - Rank number

  @retval Logical rank number

**/
UINT8
GetLogicalRank (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
  UINT8 LogicalRank;

#ifdef DDR5_SUPPORT
  if (IsDdr5Present (Host, Socket)) {
    LogicalRank = GetLogicalRankDDR5 (Host, Socket, Ch, Dimm, Rank);
  } else
#endif
  {
    LogicalRank = GetLogicalRankDDR4 (Host, Socket, Ch, Dimm, Rank);
  }

  return LogicalRank;
}

/**

  Enables parity checking on the DIMM

  @param[in] Host     Pointer to sysHost, the system host (root) structure
  @param[in] Socket   Socket Id
  @param[in] Dimm     DIMM number
  @param[in] Rank     rank number

  @retval None

**/
VOID
EnableParityCheckingBackside (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Dimm,
  IN  UINT8    Rank
  )
{
#ifndef DDR5_SUPPORT
  UINT8               Ch;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct rankDevice   (*RankStruct)[MAX_RANK_DIMM];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  if (!DoesChipSupportParityChecking (Host)) {
    return;
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {

    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    if (((*DimmNvList)[Dimm].dimmPresent == 0) || ((*DimmNvList)[Dimm].DcpmmPresent)) {
      continue;
    }

    RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);

    //
    // Set the following register in the RCD to disable parity checking in the RCD, but enable forwarding of the parity to the DRAMs
    //    a. RCW08[2] = 0 to enable QxPAR outputs
    //    b. RCW08[3] = 1 to Disable DA17 Input Buffer and QxA17 if A17 is not present for correct Parity
    //    c. RCW0E[3:0] = 0 to disable Register parity checking. The register will forward a command with bad parity to dram. Note that the dram will
    //                  still drive ALERT to the RCD ERROR_IN pin, which is forward to the host via the ALERT signal
    //    d. RCW0F[3:0] = 0 for 0 nCK latency adder
    //
    WriteRC (Host, Socket, Ch, Dimm, 0, ((*DimmNvList)[Dimm].rcCache[RDIMM_RC08] & (~BIT2)), RDIMM_RC08);

    if (!A17EnabledBackside (Host, Socket, Ch, Dimm)) {
      WriteRC (Host, Socket, Ch, Dimm, 0, ((*DimmNvList)[Dimm].rcCache[RDIMM_RC08] | BIT3), RDIMM_RC08);
    }

    WriteRC (Host, Socket, Ch, Dimm, 0, (0x0), RDIMM_RC0E);
    WriteRC (Host, Socket, Ch, Dimm, 0, (0x0), RDIMM_RC0F);

    //
    // The following settings need to be asserted in the DRAM
    //    a. MR5 A[2:0] = (1<<9) | (1<<0) for <2400 OR (1<<9)|(1<<1) for >= 2400
    //
    if (Host->nvram.mem.socket[Socket].ddrFreq < DDR_2400) {
      (*RankStruct)[Rank].MR5 = (*RankStruct)[Rank].MR5 | BIT0 | BIT9;
    } else if ((Host->nvram.mem.socket[Socket].ddrFreq >= DDR_2400) && (Host->nvram.mem.socket[Socket].ddrFreq < DDR_2933)) {
      (*RankStruct)[Rank].MR5 = (*RankStruct)[Rank].MR5 | BIT1 | BIT9;
    } else {
      (*RankStruct)[Rank].MR5 = (*RankStruct)[Rank].MR5 | BIT1 | BIT0 | BIT9;
    }

    WriteMRS (Host, Socket, Ch, Dimm, Rank, (*RankStruct)[Rank].MR5, BANK5);

  } // Ch loop
#endif // !DDR5_SUPPORT
} // EnableParityCheckingBackside

/**

  Disables parity checking on the DIMM

  @param[in] Host:         Pointer to sysHost
  @param[in] Socket:       Processor socket to check

**/
VOID
DisableParityChecking (
  PSYSHOST  Host,
  UINT8     Socket
  )
{
  UINT8                             Ch;
  struct channelNvram               (*channelNvList)[MAX_CH];
  CHIP_PARITY_CHECKING_STRUCT       chipDisableParityCheckingStruct;
  BootMode                          SysBootMode;
  UINT8                             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  SysBootMode = GetSysBootMode ();

  if (!DoesChipSupportParityChecking (Host)) {
    return;
  }

  channelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*channelNvList)[Ch].enabled == 0) {
      continue;
    }

    ChipSpecificParityCheckDisablePerCh (Host, Socket, Ch, &chipDisableParityCheckingStruct);

    if ((SysBootMode == NormalBoot) &&
        ((Host->var.mem.subBootMode == ColdBoot) || (Host->var.mem.subBootMode == ColdBootFast) || (Host->var.mem.subBootMode == NvDimmResume)) &&
        (Host->nvram.mem.dramType == SPD_TYPE_DDR4)) {

      DisableParityCheckingDdr4 (Socket, Ch);

    } else if ((SysBootMode == NormalBoot) &&
      ((Host->var.mem.subBootMode == ColdBoot) || (Host->var.mem.subBootMode == ColdBootFast) || (Host->var.mem.subBootMode == NvDimmResume)) &&
      (Host->nvram.mem.dramType == SPD_TYPE_DDR5)) {
#ifdef DDR5_SUPPORT
      DisableParityCheckingDdr5 (Socket, Ch);
#endif
    } // DDR5

    ChipSpecificErrorEnable (Host, Socket, Ch, &chipDisableParityCheckingStruct);

  } // Ch loop
} // DisableParityChecking

/**

  Disables parity checking on the DIMM

  @param[in] Host     Pointer to sysHost, the system host (root) structure
  @param[in] Socket   Socket Id
  @param[in] Dimm     DIMM number
  @param[in] Rank     rank number

  @retval None

**/
VOID
DisableParityCheckingBackside (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Dimm,
  IN  UINT8    Rank
  )
{
#ifndef DDR5_SUPPORT
  UINT8                       Ch;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram (*DimmNvList)[MAX_DIMM];
  struct rankDevice (*RankStruct)[MAX_RANK_DIMM];
  UINT8             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  if (!DoesChipSupportParityChecking (Host)) {
    return;
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {

    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    if (((*DimmNvList)[Dimm].dimmPresent == 0) || ((*DimmNvList)[Dimm].DcpmmPresent)) {
      continue;
    }

    RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);

    //
    // Set the following register in the RCD to disable parity checking in the RCD, but enable forwarding of the parity to the DRAMs
    //    a. RCW08[2] = 0 to enable QxPAR outputs
    //    b. RCW0E[0] = 0 to disable Register parity checking. The register will forward a command with bad parity to dram. Note that the dram will
    //                  still drive ALERT to the RCD ERROR_IN pin, which is forward to the host via the ALERT signal
    //     c. RCW0F[3:0] = 4 for 0 nCK latency adder
    //
    (*DimmNvList)[Dimm].rcCache[RDIMM_RC08] &= ~BIT2;
    (*DimmNvList)[Dimm].rcCache[RDIMM_RC0E] &= ~BIT0;
    (*DimmNvList)[Dimm].rcCache[RDIMM_RC0F] &= ~0xF;
    (*DimmNvList)[Dimm].rcCache[RDIMM_RC0F] |= 0x4;

    WriteRC (Host, Socket, Ch, Dimm, 0, (*DimmNvList)[Dimm].rcCache[RDIMM_RC08], RDIMM_RC08);
    WriteRC (Host, Socket, Ch, Dimm, 0, (*DimmNvList)[Dimm].rcCache[RDIMM_RC0E], RDIMM_RC0E);
    WriteRC (Host, Socket, Ch, Dimm, 0, (*DimmNvList)[Dimm].rcCache[RDIMM_RC0F], RDIMM_RC0F);

    //
    // The following settings need to be asserted in the DRAM
    //    a) MR5 A[2:0] = (1<<9) | (1<<0) for <2400 OR (1<<9)|(1<<1) for >= 2400
    //
    if (Host->nvram.mem.socket[Socket].ddrFreq < DDR_2400) {
      (*RankStruct)[Rank].MR5 &= ~(BIT0 | BIT9);
    } else if ((Host->nvram.mem.socket[Socket].ddrFreq >= DDR_2400) && (Host->nvram.mem.socket[Socket].ddrFreq < DDR_2933)) {
      (*RankStruct)[Rank].MR5 &= ~(BIT1 | BIT9);
    } else {
      (*RankStruct)[Rank].MR5 &= ~(BIT0 | BIT1 | BIT9);
    }
    WriteMRS (Host, Socket, Ch, Dimm, Rank, (*RankStruct)[Rank].MR5, BANK5);

  } // Ch loop
#endif // !DDR5_SUPPORT
} // DisableParityCheckingBackside

/**

  Enables normal CMD timing

  @param Host      - Pointer to sysHost
  @param socket    - Processor socket

  @retval N/A

**/
VOID
SetNormalCMDTiming (
  PSYSHOST  Host,
  UINT8     socket
  )
{
  UINT8                   ch;
  struct channelNvram     (*channelNvList)[MAX_CH];
  CHIP_SET_NORMAL_CMD_TIMING_STRUCT chipSetNormalCMDTimingStruct;
  UINT8                   MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    //
    // Set normal CMD timing
    //
    if (ShouldChipSetNormalCMDTiming (Host, socket, ch, &chipSetNormalCMDTimingStruct)) {

      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "Setting cmd timing to %d\n", (*channelNvList)[ch].timingMode);
    } // if timing needs to be changed
  } // ch loop
} // SetNormalCMDTiming

/**
  Routine Description:
  Normalize all PI's to reduce latency

  @param Host   - Pointer to sysHost
  @param socket - Processor socket

**/
UINT32
NormalizeCCC (
  PSYSHOST  Host,
  UINT8     socket
  )
{
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               strobe;
  UINT8               clk;
  INT16               piDelay = 0;
  UINT16              minDelay;
  UINT16              maxDelay;
  UINT32              PrevDebugLevel;
  UINT8               ckEnabled[MAX_CLK];
  UINT8               ctlIndex[MAX_CLK];
  struct channelNvram (*channelNvList)[MAX_CH];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  SYS_SETUP *Setup;
  UINT8     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  if (!IsMemFlowEnabled (CmdNormalization)) {
    return SUCCESS;
  }

  channelNvList = GetChannelNvList (Host, socket);

  RcDebugPrint (SDBG_MAX,    "\n");
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Normalizing Cmd/Ctl/Clk\n");

  //
  //Push out TX PI's to maximize CMD norm benefit
  //
  WLPushOut (Host,socket, DENORMALIZE, 0);

  // Show the current RecEn and TxDqDqs values (Comment out for upload)
  RcDebugPrint (SDBG_MAX, "\n BEFORE Normalization settings...\n");
#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    DisplayResults (Host, socket, TxDqsDelay);
    DisplayResults (Host, socket, TxDqDelay);
    DisplayCCCResults (Host, socket);
  }
#endif //DEBUG_CODE_BLOCK

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    minDelay = 255;
    maxDelay = 0;

    for (clk = 0; clk < MAX_CLK; clk++) {
      ckEnabled[clk] = 0;
      ctlIndex[clk] = 0;
    } // clk loop

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      rankList = GetRankNvList (Host, socket, ch, dimm);
      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
        // If rank enabled
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        clk = (*rankList)[rank].ckIndex;
        ctlIndex[clk] = (*rankList)[rank].ctlIndex;
        ckEnabled[clk] = 1;
      } // rank loop
    } // dimm loop

    //
    // Read all delays to find the minimum
    //
    for (clk = 0; clk < MAX_CLK; clk++) {
      if (ckEnabled[clk] == 0) {
        continue;
      }
      GetSetCtlGroupDelay (Host, socket, ch, ctlIndex[clk] + CtlGrp0, GSM_READ_ONLY, &piDelay, &minDelay, &maxDelay);
    } // clk loop

    GetSetCmdGroupDelay (Host, socket, ch, CmdAll, GSM_READ_ONLY, &piDelay, &minDelay, &maxDelay);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {

        // If rank enabled
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        for (strobe = 0; strobe < MSVx4; strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if (IsStrobeNotValid (Host, strobe)) {
            continue;
          }

          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY,
            (INT16 *)&piDelay);
          UpdateMinMaxInt (piDelay-EXTRAPIMARGIN, (INT16 *)&minDelay, (INT16 *)&maxDelay);

          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, TxDqDelay, GSM_READ_ONLY,
            (INT16 *)&piDelay);
          UpdateMinMaxInt (piDelay-EXTRAPIMARGIN, (INT16 *)&minDelay, (INT16 *)&maxDelay);
        } // strobe loop
      } // rank loop
    } // dimm loop

    //
    // Normalize to zero by subtracting the minimum
    //
    piDelay = -minDelay;


    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "Normalization values: Min PI = %-4d, Max PI = %-4d, normalization = %-4d ticks\n",
                            minDelay, maxDelay, piDelay);

    // unlikely corner case check this means we probably don't have enough margin to finish training.
    if  (minDelay < 16) {
      RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "ERROR: Could not normalize > 16 PI ticks, expecting >100\n");
    }

    //
    // Hook to adjust piDelay based on Receive Enable Averaging requirements
    //
    ReceiveEnableAveragePart2a (socket, ch, &piDelay);

    (*channelNvList)[ch].normalizationFactor = piDelay;

    for (clk = 0; clk < MAX_CLK; clk++) {
      if (ckEnabled[clk] == 0) {
        continue;
      }
      GetSetClkDelay (Host, socket, ch, clk, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &piDelay);
      GetSetCtlGroupDelay (Host, socket, ch, ctlIndex[clk] + CtlGrp0, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &piDelay, &minDelay, &maxDelay);
    } // clk loop

    GetSetCmdGroupDelay (Host, socket, ch, CmdAll, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &piDelay, &minDelay, &maxDelay);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
        // If rank enabled
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        for (strobe = 0; strobe < MSVx4; strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if (IsStrobeNotValid (Host, strobe)) {
            continue;
          }

          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, TxDqsDelay, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE,
            (INT16 *)&piDelay);
          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, TxDqDelay, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE,
            (INT16 *)&piDelay);
        } // strobe loop
      } // rank loop
    } // dimm loop
  } // ch loop

  Host->nvram.mem.socket[socket].normalizationFactorEn = 1;

  PrevDebugLevel = GetDebugLevel ();
  if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
    SetDebugLevel (SDBG_ERROR);
  }
  IO_Reset (Host, socket);
  JedecInitSequence (Host, socket, CH_BITMASK); // after moving CMD/CTL/CLK
  SetDebugLevel (PrevDebugLevel);

  //
  // Re-run Receive Enable training
  //
  if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
    SetDebugLevel (SDBG_ERROR);
  }
  ReceiveEnable (Host);
  SetDebugLevel (PrevDebugLevel);

  //
  //  Check all settings after normalization (might take out)
  //
  RcDebugPrint (SDBG_MAX, "\n AFTER normalize settings...\n");
#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    DisplayResults (Host, socket, RecEnDelay);
    DisplayResults (Host, socket, TxDqsDelay);
    DisplayResults (Host, socket, TxDqDelay);
    DisplayCCCResults (Host, socket);
  }
#endif //DEBUG_CODE_BLOCK

  return SUCCESS;
} // NormalizeCCC


/**
  Routine Description:
  DeNormalize all PI's so CMD/CTL's can be margined

  Arguments:

    Host      - Pointer to sysHost
    socket    - current socket
    direction - DENORMALIZE (-1) or RENORMALIZE (1)

**/
UINT32
DeNormalizeCCC (
  PSYSHOST  Host,
  UINT8     socket,
  INT8      direction
  )
{
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               strobe;
  UINT8               clk;
  INT16               piDelay;
  UINT8               ckEnabled[MAX_CLK];
  UINT8               ctlIndex[MAX_CLK];
  UINT16              minDelay;
  UINT16              maxDelay;
  UINT32              PrevDebugLevel;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  SYS_SETUP *Setup;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  if (!IsMemFlowEnabled (CmdNormalization)) {
    return SUCCESS;
  }

  //  Added conditional to identify the type of normalization being done
  RcDebugPrint (SDBG_MAX,    "\n");
  if (direction == DENORMALIZE) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "DeNormalize Cmd/Ctl/Clk\n");
  } else {
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "ReNormalize Cmd/Ctl/Clk\n");
  }

  channelNvList = GetChannelNvList (Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    piDelay = direction * (*channelNvList)[ch].normalizationFactor;

    minDelay = 255;
    maxDelay = 0;

    //  Check the value and see if it is the same value as the normalization factor
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Cmd normalization Factor %d\tDirection %d\n", (*channelNvList)[ch].normalizationFactor, direction);

    for (clk = 0; clk < MAX_CLK; clk++) {
      ckEnabled[clk] = 0;
      ctlIndex[clk] = 0;
    } // clk loop

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      rankList = GetRankNvList (Host, socket, ch, dimm);

      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {

        // If rank enabled
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        clk = (*rankList)[rank].ckIndex;
        ctlIndex[clk] = (*rankList)[rank].ctlIndex;
        ckEnabled[clk] = 1;
      } // rank loop
    } // dimm loop

    for (clk = 0; clk < MAX_CLK; clk++) {
      if (ckEnabled[clk] == 0) {
        continue;
      }

      GetSetClkDelay (Host, socket, ch, clk, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &piDelay);

      GetSetCtlGroupDelay (Host, socket, ch, ctlIndex[clk] + CtlGrp0, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &piDelay, &minDelay, &maxDelay);
    } // clk loop

    GetSetCmdGroupDelay (Host, socket, ch, CmdAll, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &piDelay, &minDelay, &maxDelay);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      rankList = GetRankNvList (Host, socket, ch, dimm);
      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {

        // If rank enabled
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        for (strobe = 0; strobe < MSVx4; strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if (IsStrobeNotValid (Host, strobe)) {
            continue;
          }
          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, TxDqsDelay, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE,
            (INT16 *)&piDelay);

          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, TxDqDelay, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE,
            (INT16 *)&piDelay);
        } // strobe loop
      } // rank loop
    } // dimm loop
  } // ch loop

  if (direction == DENORMALIZE) {
    Host->nvram.mem.socket[socket].normalizationFactorEn = 0;
  } else {
    Host->nvram.mem.socket[socket].normalizationFactorEn = 1;
  }

  PrevDebugLevel = GetDebugLevel ();
  if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
    SetDebugLevel (SDBG_ERROR);
  }
  IO_Reset (Host, socket);
  if (IsDdr5Present (Host, socket) == FALSE) {
    JedecInitSequence (Host, socket, CH_BITMASK); // after moving CMD/CTL/CLK
  }
  SetDebugLevel (PrevDebugLevel);

  //
  // Re-run Receive Enable training (messes up LRDIMMs when normalizing cmd/ctl)
  //
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "Re-run Receive Enable training\n");
  if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
    SetDebugLevel (SDBG_ERROR);
  }
#ifdef DDR5_SUPPORT
  if (IsDdr5Present (Host, socket)) {
    ReceiveEnableDdr5 (Host);
  } else
#endif //DDR5_SUPPORT
  {
    ReceiveEnable(Host);
  }
  RoundTripOptimize (Host);
  SetDebugLevel (PrevDebugLevel);

  //
  //  Check to see if the settings are restored
  //
  RcDebugPrint (SDBG_MAX, "\nShowing current settings...\n");
#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    DisplayResults (Host, socket, RecEnDelay);
    if (Setup->mem.trainingResultOffsetFunctionEnable && direction == RENORMALIZE) {
      //After RENORMALIZE and receive enable retraining, we need to apply offset again
      TrainingResultRecEnOffsetCfg (Host, socket);
      RcDebugPrint (SDBG_MAX, "\nShowing RecEnDelay settings after Applying TrainingOffset...\n");
      DisplayResults (Host, socket, RecEnDelay);
    }
    DisplayResults (Host, socket, TxDqsDelay);
    DisplayResults (Host, socket, TxDqDelay);
    DisplayCCCResults (Host, socket);
  }
#endif //DEBUG_CODE_BLOCK

  return SUCCESS;
} // DeNormalizeCCC



/**

  Checks to see of supplied group is a member of the CMD Group.

  @param group - Group item to check

  @retval 1 - Is a member of CMD Group
  @retval 0 - Is not a member of CMD Group

**/
UINT8
CheckCMDGroup (
  MRC_GT  group
  )
{
  if ((group == CmdAll) ||
      (group == CmdGrp0) || (group == CmdGrp1) || (group == CmdGrp2) ||
      (group == CmdGrp3) || (group == CmdGrp4) || (group == CmdGrp5) ||
      (group == CmdVref) || (group == CmdTco) ||
      (group == Cmd0All) || (group == Cmd1All)) {
    return 1;
  } else {
    return 0;
  }
} // CheckCMDGroup

/**

  Checks to see of supplied group is a member of the CTL Group.

  @param group - Group item to check

  @retval 1 - Is a member of CTL Group
  @retval 0 - Is not a member of CTL Group

**/
UINT8
CheckCTLGroup (
  MRC_GT  group
  )
{
  if ((group == CtlAll) ||
      (group == CtlGrp0) || (group == CtlGrp1) || (group == CtlGrp2) || (group == CtlGrp3) || (group == CtlGrp4) ||
      (group == Ctl0Grp) || (group == Ctl1Grp)) {
    return 1;
  } else {
    return 0;
  }
} // CheckCTLGroup


#define SETUP_FOR_RE_TEST BIT0
#define TEAR_DOWN_RE_TEST BIT1
#define EXECUTE_RE_TEST   BIT2



GSM_CSN   CtlToSweep[NUM_SIGNALS_TO_SWEEP_LB] = {
  CS0_N, CS1_N, CS2_N, CS3_N, CS4_N, CS5_N, CS6_N, CS7_N,
  CKE0, CKE1, CKE2, CKE3,
  ODT0, ODT1, ODT2, ODT3,
};

/**

  Exectues early control/clock training with Loopback mode

  @param Host - Pointer to sysHost

  @retval N/A

**/
UINT32
EarlyCtlClkLoopback (
  PSYSHOST  Host
  )
{
  UINT8               socket;
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               rankPresent;
  UINT8               controlWordData = 0;
  UINT8               signal;
  UINT8               index;
  GSM_CSN             signalToSweep[MAX_CH];
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  INT16               le[MAX_CH];
  INT16               re[MAX_CH];
  //  UINT32              lrbufData;
  GSM_CSEDGE_CTL      signalEdgeData[MAX_CH][MAX_RANK_CH][NUM_SIGNALS_TO_SWEEP_LB];
  UINT32              status = SUCCESS;
  UINT8               sweepSomething;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  socket = Host->var.mem.currentSocket;

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "EarlyCtlClkLoopback Starts\n");

  channelNvList = GetChannelNvList (Host, socket);
  index = 0;

  // Chip Workaround
  OffsetCCC (Host, socket, 64, 1);

  //
  // Retain runtime parity enabling so host can react to the ALERT (or ERR#) feedback.
  //
  EnableCAParityRuntime (Host, socket);

  ToggleBcom (Host, socket, 0);

  for (ch = 0; ch < MaxChDdr; ch++) {
    for (rank = 0; rank < MAX_RANK_CH; rank++) {
      for (signal = 0; signal < NUM_SIGNALS_TO_SWEEP_LB; signal++) {
        signalEdgeData[ch][rank][signal].toSample = 0; // Data structure containing CS, CKE, and ODT values
      }
    }
  }//ch loop

  //
  // Test every rank on every Dimm
  //
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {

      OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_EARLY_CTL_CLK, (UINT16) ((socket << 8) | (dimm << 4) |rank)));
      rankPresent = 0;

      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }
        dimmNvList = GetDimmNvList (Host, socket, ch);

        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        //
        // Indicate this rank is present on at least one channel
        //
        rankPresent = 1;
        break;
      } // ch loop

      //
      // Continue to the next rank if this one is not populated
      //
      if (rankPresent == 0) {
        continue;
      }

      //
      // S2#/S3# or CID0,1,2 for LRDIMM and 3DS are handled when rank = 0
      //
      if (rank >= 2) {
        continue;
      }

      //
      //RC0C,DA[3:0] : CS -- 001x, CKE -- 010x, ODT -- 011x, where x is rank.
      //Normal --- 0000, CK to CA -- 0001
      //
      // CS  -> signal 0
      // CKE -> signal 1
      // ODT -> signal 2
      // GNT -> signal 3
      // C0  -> signal 4
      // C1  -> signal 5
      // C2  -> signal 6
      // A17  -> signal 7
      // etc...
      //
      for (signal = 0; signal < 6 + NUM_CA_SIGNALS; signal++) {

        //
        //Once in Loopback mode, Dimm will no longer accept MRS commands until JEDEC Init is called
        //
        sweepSomething = 0;
        for (ch = 0; ch < MaxChDdr; ch++) {
          signalToSweep[ch] = NO_SIGNAL; // initialize to no signal
          if ((*channelNvList)[ch].enabled == 0) {
            continue;
          }

          dimmNvList = GetDimmNvList (Host, socket, ch);
          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          // Convert signal to controlWordData CS/CKE/ODT
          if (signal < 3) {
            // CTL signal loopback
            controlWordData = ((signal + 1) * 2) + rank;

          } else if (signal == 3) {
            // GNT loopback for NVM DIMMs
            if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
              continue;
            }
            controlWordData = 3;

          } else if (signal == 4) {
            // C0/S2# loopback for LRDIMM, Encoded QuadCS, or Encoded 3DS mode
            // skip if rank = 1, NOT LRDIMM or if NVMDIMM
            if (rank || (isCmdCtlTrained (Host, socket, ch, dimm, C0) != (UINT32) CHECKPOINT_MINOR_EARLY_CTL_CLK)) {
              continue;
            }
            //if (rank || (*dimmNvList)[dimm].DcpmmPresent || (IsLrdimmPresent (socket, ch, dimm)==0) ) continue;
            controlWordData = 1;

          } else if (signal == 5) {
            // C1/S3# loopback for Encoded 3DS mode and diecount > 2
            // skip if rank = 1, Direct or encoded LRDIMM, skip if NOT LRDIMM, or if NVMDIMM
            if (rank || (isCmdCtlTrained (Host, socket, ch, dimm, C1) != (UINT32) CHECKPOINT_MINOR_EARLY_CTL_CLK)) {
              continue;
            }
            //if (rank || (*dimmNvList)[dimm].DcpmmPresent || (*dimmNvList)[dimm].dieCount < 3) continue;
            controlWordData = 1;

          } else if (signal == 6) {
            // C2 loopback for Encoded 3DS mode and diecount > 4
            // Signal is shared, so all DIMMs need to use largest dieCount on the channel
            if (rank || (isCmdCtlTrained (Host, socket, ch, dimm, C2) != (UINT32) CHECKPOINT_MINOR_EARLY_CTL_CLK)) {
              continue;
            }
            //if (rank || (*dimmNvList)[dimm].DcpmmPresent || (*dimmNvList)[dimm].dieCount < 5 ) continue;
            controlWordData = 1;

          } else if (signal == 7) {
            // A17 loopback
            // Add support for 16Gb density, should be in earlycmdclk
            continue;

          } else {
            // Loopback all other CA signals
            if (rank) {
              continue;
            }
            controlWordData = 1;
          }

          switch (signal) {
          case 0:
            index = (dimm*4 + rank);
            break;
          case 1:
            index = (dimm*2 + rank + NUM_CS);
            break;
          case 2:
            index = (dimm*2 + rank + NUM_CS + NUM_CKE);
            break;
          case 3:
            //CS1 used for GNT
            index = (dimm*4 + rank + 1);
            break;
          case 4:
            //CS2 used for C0
            index = (dimm*4 + rank + 2);
            break;
          case 5:
            //CS3 used for C1
            index = (dimm*4 + rank + 3);
            break;
          default:
            index = signal - 6 + NUM_CTL_SIGNALS;
            break;
          }//signal conditional

          signalToSweep[ch] = CtlToSweep[index];
          sweepSomething = 1;

        } // ch loop

        if (!sweepSomething) {
          continue;
        }

        // Use SMBus access change loopback mode
        // Optional DDR4 method: use JedecInit to reset RCD state. Not compatible with NVMDIMM
        if (FeaturePcdGet (PcdCteBuild) == TRUE) {
          JedecInitSequence (Host, socket, CH_BITMASK);
        }
        for (ch = 0; ch < MaxChDdr; ch++) {
          if ((*channelNvList)[ch].enabled == 0) {
            continue;
          }

          if (FeaturePcdGet (PcdCteBuild) == TRUE) {
            WriteRC (Host, socket, ch, dimm, rank, controlWordData, RDIMM_RC0C);
          }
          dimmNvList = GetDimmNvList (Host, socket, ch);
          //
          // Initial Rc0c to 0 before programming ControlWord for next signal
          //
          (*dimmNvList)[dimm].rcCache[RDIMM_RC0C] = 0;
          ChipSetCTLLoopback (Host, socket, ch, dimm, rank);

          //
          // Initial FMC CS# to training mode to avoid unexpected behavior when train DDR4
          //
          ChipCS0ToLoopback (Host, socket, ch);
        }

        for (ch = 0; ch < MaxChDdr; ch++) {
          if ((*channelNvList)[ch].enabled == 0) {
            continue;
          }
          dimmNvList = GetDimmNvList (Host, socket, ch);
          //
          // Program Rc0c with desired ControlWord
          //
          (*dimmNvList)[dimm].rcCache[RDIMM_RC0C] = controlWordData;
          ChipSetCTLLoopback (Host, socket, ch, dimm, rank);
        } //ch

        GetCtlMarginsSweep (Host, socket, dimm, rank, signalToSweep, le, re);

        for (ch = 0; ch < MaxChDdr; ch++) {
          if ((*channelNvList)[ch].enabled == 0) {
            continue;
          }
          if (signalToSweep[ch] == NO_SIGNAL) {
            continue;
          }

          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                         "Loop %d, Signal %a,  left %d,  right %d\n", signal,GetSignalStr (signalToSweep[ch]), le[ch], re[ch]);

          rankList = GetRankNvList (Host, socket, ch, dimm);
          //
          // Convert results from absolute pi values to offset values and save the results
          //
          signalEdgeData[ch][ (*rankList)[rank].rankIndex][index].signal = signalToSweep[ch];
          signalEdgeData[ch][ (*rankList)[rank].rankIndex][index].le = le[ch];
          signalEdgeData[ch][ (*rankList)[rank].rankIndex][index].re = re[ch];
          signalEdgeData[ch][ (*rankList)[rank].rankIndex][index].toSample = 1;

        } // ch loop
      }//signal loop
    }//rank loop

    //Restore Normal Operating mode after sweeping CS/CKE/ODT for DIMM
    if (FeaturePcdGet (PcdCteBuild) == TRUE) {
      JedecInitSequence (Host, socket, CH_BITMASK);
    }
    controlWordData = 0;
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      dimmNvList = GetDimmNvList (Host, socket, ch);
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      (*dimmNvList)[dimm].rcCache[RDIMM_RC0C] = controlWordData;
      ChipSetCTLLoopback (Host, socket, ch, dimm, 0);
    } //ch
  }//dimm loop

  //
  // Restore FMC back to Normal Operating mode
  //
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    controlWordData = 0;
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      dimmNvList = GetDimmNvList (Host, socket, ch);
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      (*dimmNvList)[dimm].rcCache[RDIMM_RC0C] = controlWordData;
      ChipSetCTLLoopback (Host, socket, ch, dimm, 0);
    } //ch
  }//dimm loop

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    //
    // Evaluate results and write them
    //
    status = (UINT32) SetCombinedCtlGroup (Host, socket, ch, NUM_SIGNALS_TO_SWEEP_LB, signalEdgeData[ch]);

  }//ch loop

  // Chip Workaround
  OffsetCCC (Host, socket, -64, 1);

#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    DisplayCCCResults (Host, socket);
  }
#endif // DEBUG_CODE_BLOCK

  ToggleBcom (Host, socket, 1);

  //
  // Disable CADB always on mode
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    DisableCADBAlwaysOnMode (socket, ch);
  }

  FifoTrainReset (Host, socket);
  return status;
} // EarlyCtlClkLoopback


/**

  Finds control margins for the given signal

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
GetCtlMarginsSweep (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     dimm,
  UINT8     rank,
  GSM_CSN   signal[MAX_CH],
  INT16     *le,
  INT16     *re
  )
{
  UINT8               ch;
  INT16               piIndex;
  UINT16              i;
  UINT8               alertFeedback[MAX_CH][CTL_CLK_TEST_RANGE];
  INT16               orgDelay[MAX_CH];
  UINT32              rankMask[MAX_CH];
  UINT8               alertCentering[MAX_CH];
  UINT32              chBitmask = 0;
  struct cmdEye       curEye[MAX_CH];
  struct channelNvram (*channelNvList)[MAX_CH];
  UINT16              piPosition[MAX_CH];
  UINT32              status = 0;
  UINT8               err = 0;
  UINT16              eyeWidth;
  UINT8               MaxChDdr;
  //ALERTSIGNAL_MCDDC_DP_STRUCT alertSignal;
  //DDRT_ERROR_MC_2LM_STRUCT     errSignals;
  //RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
  //               "GetCtlMarginsSweep\n");

  MaxChDdr = GetMaxChDdr ();
  ZeroMem ((UINT8 *) alertCentering, sizeof (alertCentering));
  channelNvList = GetChannelNvList (Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {

    rankMask[ch] = 0;
    orgDelay[ch] = 0;

    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
      continue;
    }

    re[ch] = UNMARGINED_CMD_EDGE; // UNMARGINED_CMD_EDGE = 511
    le[ch] = -UNMARGINED_CMD_EDGE;

    //if (isCmdCtlTrained(Host, socket, ch, dimm, signal[ch]) != (UINT32)CHECKPOINT_MINOR_EARLY_CTL_CLK ) continue;
    if (signal[ch] == NO_SIGNAL) {
      continue;
    }

    // Initialize ch rank mask
    rankMask[ch] = 1; // rank is enabled

    curEye[ch].left = 0;
    curEye[ch].right = 0;

    //
    // Get the original delay
    //

    GetSetSignal (Host, socket, ch, DdrLevel, signal[ch], GSM_READ_ONLY, &orgDelay[ch]);

    // Clear dimm error results
    // support larger step sizes
    ZeroMem (alertFeedback[ch], CTL_CLK_TEST_RANGE);

    // Indicate rank present and enabled on this ch
    chBitmask |= 1 << ch;

    //
    //Set CADB Pattern for each Channel
    //
    CpgcCtlClkPattern (Host, socket, ch, signal[ch]);
  }// ch loop

  //
  // Run concurrent tests
  //
  CpgcGlobalTestStart (Host, socket, chBitmask, NO_SUBCH);

  //
  // Delay if this is a CTE build, otherwise do nothing.
  //

  CteDelayDclk (300);

  // Alternative method mod 128 + 128
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((rankMask[ch] == 0)) {
      continue;
    }

    alertCentering[ch] = ChipGetParityErrorStatus (Host, socket, ch, dimm);
  } //ch loop

  //
  // Assert that CTL_CLK_TEST_RANGE is evenly divisible by CTL_CLK_STEPSIZE
  //
  if (CTL_CLK_TEST_RANGE % CTL_CLK_STEPSIZE) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_70);
  }
  //
  // Sweep Pi delays
  //
  for (piIndex = 0; piIndex < CTL_CLK_TEST_RANGE; piIndex += CTL_CLK_STEPSIZE) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((rankMask[ch] == 0)) {
        continue;
      }

      //
      //Set Pi Value
      //

      GetSetSignal (Host, socket, ch, DdrLevel, signal[ch], GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &piIndex);

    } //ch loop

    //
    // Generate an IO Reset. This is required because the pi delays changed.
    //
    IO_Reset (Host, socket);

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((rankMask[ch] == 0)) {
        continue;
      }
      err = ChipGetParityErrorStatus (Host, socket, ch, dimm);

      if (alertCentering[ch]) {
        alertFeedback[ch][piIndex] = !err;
      } else {
        alertFeedback[ch][piIndex] = err;
      }

      // Fill unused feedback entries
      for (i = 1; i < CTL_CLK_STEPSIZE; i++) {
        alertFeedback[ch][piIndex + i] = alertFeedback[ch][piIndex];
      }

    } //ch loop
  }//piIndex loop

  //
  // Stop Test
  //
  CpgcGlobalTestStop (Host, socket, chBitmask, NO_SUBCH);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((rankMask[ch] == 0)) {
      continue;
    }
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                  "Sweep results for %a\n", GetSignalStr (signal[ch]));
    for (piIndex = 0; piIndex < CTL_CLK_TEST_RANGE; piIndex++) {

      if (alertFeedback[ch][piIndex]) {
        // '1'
        RcDebugPrint (SDBG_MAX, "1");
      } else {
        // '0'
        RcDebugPrint (SDBG_MAX, "0");
      }
    } // piIndex loop

    RcDebugPrint (SDBG_MAX, "\n");
    // Right -> Index End
    // Left -> Index Beginning
    status = FindErrorGraphMin (Host, socket, ch, dimm, rank, 0, &alertFeedback[ch][0], CTL_CLK_TEST_RANGE,
               (UINT16 *)&curEye[ch].right, (UINT16 *)&curEye[ch].left, (UINT16 *)&piPosition[ch]);

    if ((status == FAILURE) || (curEye[ch].left == curEye[ch].right)) {
      curEye[ch].left = 0;
      curEye[ch].right = 0;
    }

    RcDebugPrint (SDBG_MAX, "\n");
  }//ch loop

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((rankMask[ch] == 0)) {
      continue;
    }

    // Update return parameters
    re[ch] = curEye[ch].right;
    le[ch] = curEye[ch].left;

    // apply minimum eye width check here
    eyeWidth = ABS ((re[ch] - le[ch]));
    if (eyeWidth <= MinimumMargin (Host, CtlAll)) {
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
        "Eye width is too small: %d\n", eyeWidth);
      EwlOutputType3 (WARN_FPT_CORRECTABLE_ERROR, WARN_CTL_CLK_LOOPBACK_TRAINING, socket, ch, dimm, rank, EwlSeverityWarning, CtlAll, DdrLevel, signal[ch], (UINT8) (eyeWidth));
      DisableChannelSw (Host, socket, ch);
    }

    if (re[ch] < le[ch]) {
      // wrap around case
      re[ch] += 256;
    }
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                  "le = %d - re = %d\n", le[ch], re[ch]);
    //
    // Restore signal under test to its initial setting
    //
    GetSetSignal (Host, socket, ch, DdrLevel, signal[ch], GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &orgDelay[ch]);

  } //ch loop

}//GetCtlMarginsSweep

/**
  Loops through an array searching for bit values other than 1. Value
  other than 1 indicates search is not yet complete.

  @param IN RanksDone  - rank array to be checked
  @param IN NumMc      - number of memory controller elements to be checked

  @retval TRUE if complete, FALSE otherwise

**/
BOOLEAN
CheckRanksDone (
  IN UINT32  RanksDone[],
  IN UINT8   NumMc
  )
{
  UINT8 Imc;

  // Loop through MC array looking for bit values other than 1
  for (Imc = 0; Imc < NumMc; Imc++) {
    if (RanksDone[Imc] != 0xFFFFFFFF) {
      // Any 0 value indicates not complete
      return FALSE;
    }
  }
  return TRUE;
}

/**

  Exectues early control/clock training

  @param Host - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
EarlyCtlClk (
  PSYSHOST  Host
  )
{
  UINT8               socket;
  UINT8               ch;
  UINT8               imc = 0;
  UINT8               numRanks;
  UINT32              chBitmask;
  UINT8               dimm;
  UINT8               rank;
  UINT8               sign;
  UINT8               rtlOffset[2];
  UINT8               rtlIndex;
  UINT8               ctlIndex;
  UINT8               ctlEnabled;
  UINT32              strobeFeedback[2][MAX_CH];
  UINT32              strobeCompareFeedback;
  UINT8               roundTripLatency[MAX_CH];
  INT16               offset;
  INT16               currentOffset[MAX_CH];
  UINT16              minDelay;
  UINT16              maxDelay;
  INT16               ctlMargins[MAX_CH][MAX_RANK_CH][2];
  INT16               ctlOffset;
  INT16               ctlLeft;
  INT16               ctlRight;
  UINT8               rankPresent;
  UINT32              ranksDone[MAX_IMC];
  UINT32              PrevDebugLevel;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  BOOLEAN             RcvEnTrainingInitiallyEnabled = TRUE;
  UINT8               DdrioPreambleStateSave = 0;
  UINT8               MaxChDdr;
  UINT8               MaxImc;

  if (!IsMemFlowEnabled (EarlyCtlClkTraining)) {
    return SUCCESS;
  }

  MaxImc = GetMaxImc ();
  MaxChDdr = GetMaxChDdr ();
  Host->var.mem.checkMappedOutRanks = 1;

  //
  // Return if this socket is disabled
  //
  socket = Host->var.mem.currentSocket;

  if (IsSocketDimmPopulated (Host, socket) != TRUE) {
    return SUCCESS;
  }

  channelNvList = GetChannelNvList (Host, socket);

  // Execute Loopback for RDIMM, LRDIMM, NVM DIMM configs
  if (CheckCTLLoopback (Host)) {
    EarlyCtlClkLoopback (Host);
    Host->var.mem.checkMappedOutRanks = 0;
    return SUCCESS;
  }

  SetBufferMprOverrideMode (Host, socket, ENABLE_MPR_OVERRIDE);
  minDelay = 255;
  maxDelay = 0;

  InitRtlOffsetChip (rtlOffset);
  //
  // Initial Rec En training
  //
#ifdef DEBUG_CODE_BLOCK
  Host->var.mem.earlyCtlClkSerialDebugFlag = 1;
#endif //DEBUG_CODE_BLOCK

  //
  // Force receive enable to run if using earlyctlclk rcven method
  // If it is currently disabled, then enable it and set the flag so we
  // can reset it after running the training
  //
  if (!IsMemFlowEnabled (ReceiveEnableTraining)) {
    EnableMemFlow (ReceiveEnableTraining);
    RcvEnTrainingInitiallyEnabled = FALSE;
  }

  ReceiveEnable (Host);

  //
  // If ReceiveEnableTraining was initially disabled, reset to disabled
  //
  if (RcvEnTrainingInitiallyEnabled == FALSE) {
    DisableMemFlow (ReceiveEnableTraining);
  }

#ifdef DEBUG_CODE_BLOCK
  Host->var.mem.earlyCtlClkSerialDebugFlag = 0;
#endif //DEBUG_CODE_BLOCK

  for (ch = 0; ch < MaxChDdr; ch++) {
    for (rank = 0; rank < MAX_RANK_CH; rank++) {
      ctlMargins[ch][rank][0] = 0;
      ctlMargins[ch][rank][1] = 0;
    } // rank loop
  } // ch loop

  PrevDebugLevel = GetDebugLevel ();

  //
  // Train each DIMM
  //
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    //
    // Train ranks
    //
    numRanks = Host->var.mem.socket[socket].maxRankDimm;
    for (rank = 0; rank < numRanks; rank++) {

      OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_EARLY_CTL_CLK, (UINT16) ((socket << 8) | (dimm << 4) |rank)));

      //
      // Initialize to no ranks present
      //
      rankPresent = 0;
      chBitmask = GetChBitmask (Host, socket, dimm, rank);

      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }

        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        //
        // Indicate this rank is present on at least one channel
        //
        rankPresent = 1;

        GetOriginalRtlChip (Host, socket, ch, dimm, rank, RTL_INITIALIZE_ONCE, &roundTripLatency[ch]);
      } // ch loop

      //
      // Go to the next rank if this rank is not present on any channels
      //
      if (!rankPresent) {
        continue;
      }

      //
      //Set feedback comparison value depending on if ECC
      //
      strobeCompareFeedback = GetStrobeMask (Host);

      //
      // Search CTL from the center out to find both edges
      //
      for (sign = 0; sign < 2; sign++) {

        if (sign) {
          offset = -CTL_CLK_STEPSIZE;
        } else {
          offset = CTL_CLK_STEPSIZE;
        }

        //
        // Initialize to no failures
        //
        SetMem ((UINT8 *)ranksDone, sizeof (ranksDone), 0xFF);

        chBitmask = GetChBitmask (Host, socket, dimm, rank);

        //
        // Setup the test but do not execute it
        //
        ExecuteCtlClkTest (Host, socket, chBitmask, dimm, rank, SETUP_FOR_RE_TEST, &DdrioPreambleStateSave);

        for (ch = 0; ch < MaxChDdr; ch++) {
          currentOffset[ch] = 0;
          imc = GetMCID (Host, socket, ch);
          if ((*channelNvList)[ch].enabled == 0) {
            continue;
          }
          if ((chBitmask & (1 << ch)) == 0) {
            continue;
          }

          rankList = GetRankNvList (Host, socket, ch, dimm);

          // Indicate this rank is not done
          ranksDone[imc] &= ~(1 << ((*rankList)[rank].rankIndex + (GetMCCh (imc, ch) * 8)));

          strobeFeedback[0][ch] = 0;
          strobeFeedback[1][ch] = 0;
          GetOriginalRtlChip (Host, socket, ch, dimm, rank, RTL_RE_INITIALIZE, &roundTripLatency[ch]);
        } // ch loop

        // Loop until we either find an edge or reach the end of our range
        while (!CheckRanksDone (ranksDone, sizeof (ranksDone) / sizeof (ranksDone[0]))) {
          for (ch = 0; ch < MaxChDdr; ch++) {
            if ((chBitmask & (1 << ch)) == 0) {
              continue;
            }

            //
            // Offset CTL
            //
            GetSetCtlGroupDelay (Host, socket, ch, CtlAll, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &offset, &minDelay, &maxDelay);

            currentOffset[ch] += offset;
          } // ch loop


          IO_Reset (Host, socket);

          //
          // Get strobe levels at two RTL settings
          //
          for (rtlIndex = 0; rtlIndex < 2; rtlIndex++) {

            for (ch = 0; ch < MaxChDdr; ch++) {
              if ((chBitmask & (1 << ch)) == 0) {
                continue;
              }

              SetRoundTrip (socket, ch, GetLogicalRank (Host, socket, ch, dimm, rank),
                roundTripLatency[ch] + rtlOffset[rtlIndex]);
            } // ch loop

            //
            // Execute the CTL-CLK test
            //
            ExecuteCtlClkTest (Host, socket, chBitmask, dimm, rank, EXECUTE_RE_TEST, &DdrioPreambleStateSave);

            GetCtlClkResults (Host, socket, dimm, rank, strobeFeedback[rtlIndex]);
          } // rtlIndex loop

          //
          // Check results. If RTL offset 0 is not all zero's or RTL offset 2 is not all 1's then this is a failure
          //
          for (ch = 0; ch < MaxChDdr; ch++) {
            imc = GetMCID (Host, socket, ch);
            if ((chBitmask & (1 << ch)) == 0) {
              continue;
            }

            rankList = GetRankNvList (Host, socket, ch, dimm);

            // For debug only
            //RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
            //              "PI %3d Feedback = [%5x,%5x]\n", currentOffset[ch], strobeFeedback[0][ch], strobeFeedback[1][ch]);

            if (((strobeFeedback[0][ch] != 0) || (strobeFeedback[1][ch] != strobeCompareFeedback)) || (currentOffset[ch] < -127) || (currentOffset[ch] > 127)) {
              //
              // Edge found
              //
              ctlMargins[ch][ (*rankList)[rank].rankIndex][sign] = currentOffset[ch];

              //
              // Indicate this rank is done
              //
              ranksDone[imc] |= 1 << ((*rankList)[rank].rankIndex + (GetMCCh (imc, ch) * 8));
              chBitmask &= ~(1 << ch);
            }

            //
            // Restore RTL
            //
            SetRoundTrip (socket, ch, GetLogicalRank (Host, socket, ch, dimm, rank), roundTripLatency[ch]);

            if ((currentOffset[ch] < -128) || (currentOffset[ch] > 128)) {
              chBitmask &= ~(1 << ch);
              RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                            "No edge found\n");
            }
          } // ch loop

          if (chBitmask == 0) {
            for (imc = 0; imc < MaxImc; imc++) {
              ranksDone[imc] = 0xFFFFFFFF;
            }
          }

        } // while ranksDone

        for (ch = 0; ch < MaxChDdr; ch++) {
          if ((*channelNvList)[ch].enabled == 0) {
            continue;
          }

          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          currentOffset[ch] = 0 - currentOffset[ch];

          //
          // Restore CTL pi Delay
          //
          GetSetCtlGroupDelay (Host, socket, ch, CtlAll, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &currentOffset[ch], &minDelay, &maxDelay);
        } // ch loop

        if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
          SetDebugLevel (SDBG_ERROR);
        }
        IO_Reset (Host, socket);
        chBitmask = GetChBitmask (Host, socket, dimm, rank);
        //
        // Tear down the test and do not execute it
        //
        ExecuteCtlClkTest (Host, socket, chBitmask, dimm, rank, TEAR_DOWN_RE_TEST, &DdrioPreambleStateSave);
        JedecInitSequence (Host, socket, CH_BITMASK);
        SetDebugLevel (PrevDebugLevel);
      } // sign loop
    } // rank loop
  } // dimm loop

  //
  // Calculate average CTL across ranks
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      rankList = GetRankNvList (Host, socket, ch, dimm);
      numRanks = Host->var.mem.socket[socket].maxRankDimm;
      for (rank = 0; rank < numRanks; rank++) {

        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                      "le = %3d - re = %3d  width = %3d\n", ctlMargins[ch][(*rankList)[rank].rankIndex][1], ctlMargins[ch][(*rankList)[rank].rankIndex][0],ctlMargins[ch][(*rankList)[rank].rankIndex][0]-ctlMargins[ch][(*rankList)[rank].rankIndex][1]);
      } // rank loop
    } // dimm loop
  } // ch loop

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    for (ctlIndex = 0; ctlIndex < NUM_CTL_PLATFORM_GROUPS; ctlIndex++) {

      ctlLeft = -0xff;
      ctlRight = 0xff;
      ctlEnabled = 0;

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        rankList = GetRankNvList (Host, socket, ch, dimm);
        numRanks = Host->var.mem.socket[socket].maxRankDimm;
        for (rank = 0; rank < numRanks; rank++) {

          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          if ((*rankList)[rank].ctlIndex != ctlIndex) {
            continue;
          }

          ctlEnabled = 1;

          if (ctlLeft < ctlMargins[ch][ (*rankList)[rank].rankIndex][1]) {
            ctlLeft = ctlMargins[ch][ (*rankList)[rank].rankIndex][1];
          }

          if (ctlRight > ctlMargins[ch][ (*rankList)[rank].rankIndex][0]) {
            ctlRight = ctlMargins[ch][ (*rankList)[rank].rankIndex][0];
          }
        } // rank loop
      } // dimm loop

      if (ctlEnabled) {
        ctlOffset = (ctlRight + ctlLeft) / 2;
        GetSetCtlGroupDelay (Host, socket, ch, ctlIndex + CtlGrp0, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &ctlOffset, &minDelay, &maxDelay);
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "Ctl group %2d, left edge = %3d - right edge = %3d offset == %3d width =%3d\n", ctlIndex, ctlLeft, ctlRight, ctlOffset, ctlRight-ctlLeft);
      } // ctlEnabled

    } // ctlIndex loop
  } // ch loop

#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    DisplayCCCResults (Host, socket);
  }
#endif //DEBUG_CODE_BLOCK
  SetBufferMprOverrideMode (Host, socket, DISABLE_MPR_OVERRIDE);
  Host->var.mem.checkMappedOutRanks = 0;

  return SUCCESS;
} // EarlyCtlClk

/**

  Exectues control/clock point test

  @param[in]     Host                     - Pointer to sysHost
  @param[in]     socket                   - Processor socket
  @param[in]     chBitmask                - Bitmask of channels to test
  @param[in]     dimm                     - DIMM to test
  @param[in]     rank                     - Rank to test
  @param[in]     testFlags                - Flags
  @param[in,out] DdrioPreambleStateSave   - Pointer to save the DDRIO preamble state

  @retval N/A

**/
VOID
ExecuteCtlClkTest (
  IN     PSYSHOST  Host,
  IN     UINT8     socket,
  IN     UINT32    chBitmask,
  IN     UINT8     dimm,
  IN     UINT8     rank,
  IN     UINT8     testFlags,
  IN OUT UINT8     *DdrioPreambleStateSave
  )
{
#ifndef DDR5_SUPPORT
  UINT8                                 ch;
  UINT8                                 logRank;
  INT16                                 RecEnOffset;
  UINT8                                 DumArr[7] = {1,1,1,1,1,1,1};
  struct channelNvram                   (*channelNvList)[MAX_CH];
  struct rankDevice                     (*rankStruct)[MAX_RANK_DIMM];
  EXECUTE_CTL_CLK_TEST_CHIP_STRUCT      executeCtlCLKTestChipStruct;
  UINT8                                 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);

  if (testFlags & SETUP_FOR_RE_TEST) {
    //
    // Receive enable offset
    //
    EarlyCtlClkRecEnOffset (socket, &RecEnOffset);

    //
    // Turn on MPR pattern for all ranks
    //
    SetAllRankMPR (Host, socket, MR3_MPR);

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      if ((chBitmask & (1 << ch)) == 0) {
        continue;
      }

      rankStruct = GetRankStruct (Host, socket, ch, dimm);

      if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
        continue;
      }

      //
      // Get the logical rank #
      //
      logRank = GetLogicalRank (Host, socket, ch, dimm, rank);

      SelectCPGCRanks (Host, socket, ch, 1 << logRank, 0, 0);

      if (Host->nvram.mem.dramType == SPD_TYPE_DDR4) {
        //
        // Enable Receive Enable Mode
        //
        (*rankStruct)[rank].MR0 &= ~MR0_DLL;
        WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR4 | MR4_RE, BANK4);
        (*rankStruct)[rank].MR0 |= MR0_DLL;

        //
        // Save original preamble state and enable long preamble in the DDRIO
        //
        *DdrioPreambleStateSave = GetLongPreambleEnableDdrio (socket, ch);

        SetLongPreambleEnableDdrio (socket, ch, LONG_PREAMBLE_ENABLE);

        SetLrdimmPreambleTrMode (Host, socket, ch, dimm, ENABLE_PREAMBLE_TR_MODE);
      }

      PerformChipODTMPRDimmActivationStep (Host, socket, ch, dimm, &executeCtlCLKTestChipStruct);

      //
      // Chip specific IO latency step (Add 1 or subtract 1 from/to IO latency)
      //
      StepChipCtlCKIOLatency (socket, ch, logRank);

      //
      // Add offset to rec en (currently chip specific initial value)
      //
      GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, 0, DdrLevel, RecEnDelay, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET,
        &RecEnOffset);

    } // ch loop

    SetTrainingMode (Host, socket, dimm, rank, CHECKPOINT_MINOR_RECEIVE_ENABLE_BASIC);

    SetupRecEnFineTest (Host, socket, chBitmask, dimm, rank);
  } // Setup test

  if (testFlags & EXECUTE_RE_TEST) {
    //
    // Send a burst of 7 read commands back to back (4 DCLK apart) using MC_SPECIAL_COMMAND CRs
    //
    RunIOTest (Host, socket, chBitmask, BasicVA, DumArr, 1, 0);
  }

  if (testFlags & TEAR_DOWN_RE_TEST) {
    //
    // Receive enable offset
    //
    EarlyCtlClkRecEnOffset (socket, &RecEnOffset);
    RecEnOffset = -RecEnOffset;

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      // Continue to the next rank if this one is disabled
      if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
        continue;
      }

      DisableChipChlRecEnOffsetTrainingMode (Host, socket, ch, &executeCtlCLKTestChipStruct);

      SetLrdimmPreambleTrMode (Host, socket, ch, dimm, DISABLE_PREAMBLE_TR_MODE);
      if (Host->nvram.mem.dramType == SPD_TYPE_DDR4) {
        rankStruct = GetRankStruct (Host, socket, ch, dimm);

        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        //
        // Enable Receive Enable Mode
        //
        (*rankStruct)[rank].MR0 &= ~MR0_DLL;
        WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR4, BANK4);
        (*rankStruct)[rank].MR0 |= MR0_DLL;

        //
        // Restore original preamble state
        //
        SetLongPreambleEnableDdrio (socket, ch, *DdrioPreambleStateSave);

      } // if DDR4

      //
      // Get the logical rank #
      //
      logRank = GetLogicalRank (Host, socket, ch, dimm, rank);

      //
      // Restore IO latency
      //
      RestoreChipCtlCKIOLatency (socket, ch, logRank);

      //
      // Restore rec en
      //
      GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, 0, DdrLevel, RecEnDelay, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET,
        &RecEnOffset);
    } // ch loop

    //
    // Notify IO to move out of RCVEN training mode
    //
    SetTrainingMode (Host, socket, dimm, rank, CLEAR_MODE);

    //
    // Turn off MPR pattern for all ranks
    //
    SetAllRankMPR (Host, socket, 0);
  }
#endif // !DDR5_SUPPORT
} // ExecuteCtlClkTest

/**

  Get's the resutls from the current test

  @param Host      - Pointer to sysHost
  @param dimm      - DIMM number
  @param rank      - rank number of the DIMM

  @retval N/A

**/
VOID
GetCtlClkResults (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     dimm,
  UINT8     rank,
  UINT32    strobeFeedback[MAX_CH]
  )
{
  UINT8               ch;
  UINT8               strobe;
  UINT32              csrReg;
  struct channelNvram (*channelNvList)[MAX_CH];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    // Continue to the next rank if this one is disabled
    if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
      continue;
    }

    strobeFeedback[ch] = 0;
    //Platforms require varying logic change for upper nibble
    //BRS_TODO?
    for (strobe = 0; strobe < CHIP_GET_CTL_CLK_RES_MAX_STROBE; strobe++) {

      //Skip if non ecc
      if (IsStrobeNotValid (Host, strobe)) {
        continue;
      }

      //
      //Platform specific step for lower nibble
      //
      SetChipGetCtlClkResLowerNibbleStrobeFeedback (Host, socket, ch, strobe, &csrReg, &strobeFeedback[ch]);

    } // strobe loop
  } // ch loop
} // GetCtlClkResults

VOID
GetChipBwSerrErrors (
  UINT32                       errMask,
  UINT8                        ch,
  UINT8                        errorResult[MAX_CH][CHIP_CMD_CLK_TEST_RANGE],
  INT16                        piIndex,
  INT16                        offset,
  PCHIP_GET_CMD_MGN_SWP_STRUCT chipGetCmdMgnSwpStruct
  )
{
  if (errMask) {
    chipGetCmdMgnSwpStruct->eyeSize[ch] = 0;
  } else {
    errorResult[ch][piIndex] = 0;
    chipGetCmdMgnSwpStruct->eyeSize[ch] += offset;
  }

  if (chipGetCmdMgnSwpStruct->eyeSize[ch] > 64) {
    chipGetCmdMgnSwpStruct->chEyemask &= ~(1 << ch);
  }
  if (((chipGetCmdMgnSwpStruct->chEyemask && (1 << ch)) == 0) && (errorResult[ch][piIndex] == 1)) {
    chipGetCmdMgnSwpStruct->chDonemask &= ~(1 << ch);
  }
}
