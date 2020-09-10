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
#include <Library/MemDdrioIpLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/SimulationServicesLib.h>
#include <Memory/JedecDefinitions.h>
#include "Include/BaseMemoryCoreInternal.h"
#include <Memory/MemoryCheckpointCodes.h>
#include <Memory/MemCmdCtlClkCommon.h>

//
// A guardband for RTL optimization, which is 20 pi ticks delay for all signals.
//
#define RT_INITIAL_PI_DELAY   20;

//
// Local function prototypes
//
STATIC
VOID
FindPiSetting (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     dimm,
  UINT8     rank,
  UINT16    risingEdge[MAX_CH][MAX_STROBE],
  UINT16    centerPoint[MAX_CH][MAX_STROBE],
  UINT16    fallingEdge[MAX_CH][MAX_STROBE],
  UINT16    pulseWidth[MAX_CH][MAX_STROBE]
  );
STATIC
VOID
PISettingAdjustment (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     dimm,
  UINT8     rank
  );

STATIC
VOID
FindRoundTripLatency (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     dimm,
  UINT8     rank,
  UINT16    centerPoint[MAX_CH][MAX_STROBE]
  );

VOID
GetResults (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 dimm,
  UINT8 rank,
  UINT16 piDelay
  );

VOID
FindMaxMinLogic (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     MaxSdramInDimm,
  UINT8     *MaxLogicDelay,
  UINT8     *MinLogicDelay
  );



/**

  New hook to abstract DDR4/5 programming differences

  @param Host  - Pointer to sysHost

  @retval status

**/
VOID
SetDramReceiveEnableMode (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT8     Rank,
  UINT8     Enable
  )
{
#ifndef DDR5_SUPPORT
  struct rankDevice (*RankStruct)[MAX_RANK_DIMM];
  RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);

  //
  // Enable Receive Enable Mode in Dram
  //

  if (Enable) {
    WriteMRS (Host, Socket, Ch, Dimm, Rank, (*RankStruct)[Rank].MR4 | MR4_RE, BANK4);
  } else {
    WriteMRS (Host, Socket, Ch, Dimm, Rank, (*RankStruct)[Rank].MR4, BANK4);
  }
  if (GetPreambleState (Socket, READ_PREAMBLE) == PREAMBLE_1TCLK) {
    SetLongReadPreamble (Host, Socket, Ch, Enable);
  }
#endif // !DDR5_SUPPORT
} // SetDdrReceiveEnableMode


/**

  New hook to abstract DDR4/5 programming differences

  @param Host  - Pointer to sysHost

  @retval status

**/
VOID
SetDbReceiveEnableMode (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT8     Rank,
  UINT8     Enable
  )
{
#if defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)
  UINT8  ControlWordData;
  struct dimmNvram (*DimmNvList)[MAX_DIMM];
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  //
  // Enable Receive Enable Mode in Data Buffer
  //
  if (IsLrdimmPresent (Socket, Ch, Dimm)) {

#ifdef DDR5_SUPPORT
    if (IsDdr5Present (Host, Socket)) {
      // BRS_TODO
    } else
#endif
    {
      //F0BC1x = 00010000b
      // enable read preamble training mode in the buffer
      if (Enable) {
        (*DimmNvList)[Dimm].lrBuf_BC1x = (*DimmNvList)[Dimm].lrBuf_BC1x | LRDIMM_RD_PREAMBLE_TRAINING_MODE;
      } else {
        (*DimmNvList)[Dimm].lrBuf_BC1x = (*DimmNvList)[Dimm].lrBuf_BC1x & ~LRDIMM_RD_PREAMBLE_TRAINING_MODE;
      }
      ControlWordData = (*DimmNvList)[Dimm].lrBuf_BC1x;
      WriteLrbuf (Host, Socket, Ch, Dimm, 0, ControlWordData, LRDIMM_F0, LRDIMM_BC1x);

      // IDT WA for GP B0
      if (((*DimmNvList)[Dimm].SPDRegVen == MFGID_IDT) && ((*DimmNvList)[Dimm].lrbufRid == LRBUF_IDT_DB_RID_GPB0)) {
        WriteLrbuf (Host, Socket, Ch, Dimm, 0, LRDIMM_NORMAL_MODE, LRDIMM_F0, LRDIMM_BC0C);
      }
      // END IDT WA for GP B0
    }
  } // DDR4/DDRT
#endif // defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)
} // SetDbReceiveEnableMode


/**

  Perform Receive Enable Training

  @param Host  - Pointer to sysHost

  @retval status

  **/
UINT32
ReceiveEnable (
  PSYSHOST  Host
  )
{
  UINT8                             socket;
  UINT8                             ch;
  UINT8                             dimm;
  UINT8                             rank;
  UINT8                             logRank = 0;
  UINT8                             strobe = 0;
  UINT8                             TempStrobe = 0;
#ifdef DEBUG_CODE_BLOCK
  INT16                             piDelay;
#endif // DEBUG_CODE_BLOCK
  UINT8                             numRanks;
  UINT8                             temp;
  UINT8                             rtError;
  UINT8                             RoundTrip;
  UINT8                             IoLatency;
  UINT8                             ioCompDiff;
  UINT8                             MaxLogicDelay;
  UINT8                             MinLogicDelay;
  UINT8                             rankPresent;
  UINT16                            risingEdge[MAX_CH][MAX_STROBE];
  UINT16                            centerPoint[MAX_CH][MAX_STROBE];
  UINT16                            fallingEdge[MAX_CH][MAX_STROBE];
  UINT16                            pulseWidth[MAX_CH][MAX_STROBE];
  struct ddrRank                    (*rankList)[MAX_RANK_DIMM];
  struct rankDevice                 (*rankStruct)[MAX_RANK_DIMM];
  ACT_TO_ACT_SAVE_STRUCT            ActToActSave[MAX_CH];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  EFI_STATUS                        RtRet;
  VOID *                            ChipDataPtr = NULL;
  UINT8                             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  socket = Host->var.mem.currentSocket;

  //
  // Return if socket is disable or No DIMMs found or RECVEN training disabled
  //
  if ((!IsMemFlowEnabled (ReceiveEnableTraining)) ||
      (IsSocketEnabled (socket) == FALSE)         ||
      (GetMaxDimmPop (socket) == 0)) {
    return SUCCESS;
  }

  //
  // Save state and initialize any silicon programming needed before executing Recieve Enable
  //
  SaveAndInitChipRxEnableParams (&ChipDataPtr);

  ioCompDiff = MEM_CHIP_POLICY_VALUE (Host, mrcRoundTripIoComp) - MEM_CHIP_POLICY_VALUE (Host, mrcRoundTripIoCompStart);

  for (ch = 0; ch < MaxChDdr; ch++) {
    ActToActSave[ch].TRrdSOrg = 0;
    ActToActSave[ch].TRrdLOrg = 0;
    ActToActSave[ch].TRcdRdOrg = 0;
    ActToActSave[ch].TRrsrOrg = 0;
    ActToActSave[ch].TRrSgOrg = 0;
  }

  //
  // Turn on MPR pattern for all ranks
  //
  SetAllRankMPR (Host, socket, MR3_MPR);

  ReceiveEnableInitAllRanks (Host, socket);

  //
  // Train each DIMM
  //
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    //
    // Train each rank
    //
    numRanks = Host->var.mem.socket[socket].maxRankDimm;
    for (rank = 0; rank < numRanks; rank++) {
      OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_RECEIVE_ENABLE_BASIC, (UINT16) ((socket << 8) | (dimm << 4) | rank)));
      //
      // Initialize to no ranks present
      //
      rankPresent = 0;

      for (ch = 0; ch < MaxChDdr; ch++) {

        if (IsChannelEnabled (socket, ch) == FALSE) {
          continue;
        }

        rankStruct = GetRankStruct (Host, socket, ch, dimm);

        if (IsRankValid (socket, ch, dimm, rank, FALSE) == FALSE) {
          continue;
        }
        //
        // Get the logical rank #
        //
        logRank = GetLogicalRank (Host, socket, ch, dimm, rank);

        SetActToActPeriod (Host, socket, ch, ActToActSave);
        //
        // Indicate this rank is present on at least one channel
        //
        rankPresent = 1;

        SelectCPGCRanks (Host, socket, ch, 1 << logRank, 0, 0);

        //
        // Enable Receive Enable mode in DRAM
        //
        SetDramReceiveEnableMode (Host, socket, ch, dimm, rank, 1);
        SetDbReceiveEnableMode (Host, socket, ch, dimm, rank, 1);

        SetMprModeFilterActPre (Host, socket, ch, !IsDcpmmPresentDimm (socket, ch, dimm));
      } // ch loop

      //
      // Go to the next rank if this rank is not present on any channels
      //
      if (!rankPresent) {
        continue;
      }

      SetTrainingMode (Host, socket, dimm, rank, CHECKPOINT_MINOR_RECEIVE_ENABLE_BASIC);

      //
      // Find the DQS high phase
      //
      FindPiSetting (Host, socket, dimm, rank, risingEdge, centerPoint, fallingEdge, pulseWidth);
      DetectLowMargin (Host, NO_CH, dimm, rank, pulseWidth, RecEnDelay, NON_PER_BIT);

#ifdef DEBUG_CODE_BLOCK
      if (GetDebugLevel () & SDBG_MAX) {
        AcquirePrintControl ();
      }
#endif // DEBUG_CODE_BLOCK
      for (ch = 0; ch < MaxChDdr; ch++) {

        if (IsChannelEnabled (socket, ch) == FALSE) {
          continue;
        }

        if (IsRankValid (socket, ch, dimm, rank, FALSE) == FALSE) {
          continue;
        }

        //
        // Write the PI setting to MC
        //
        for (strobe = 0; strobe < MSVx4; strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if (IsStrobeNotValid (Host, strobe)) {
            continue;
          }

          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, RecEnDelay, GSM_UPDATE_CACHE,
            (INT16 *)&centerPoint[ch][strobe]);
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                         "Rec En Delay %d\n", centerPoint[ch][strobe]);
        } // strobe loop

        GetIoLatency (socket, ch, logRank, GSM_READ_ONLY, &IoLatency);
        DecIoLatency (Host, socket, ch, dimm, rank, &IoLatency, centerPoint);
        SetIOLatency (socket, ch, logRank, IoLatency);

        GetRoundTrip (socket, ch, logRank, GSM_READ_ONLY, &RoundTrip);
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                                "Round trip latency = %d\nIO latency = %d\n",
                                RoundTrip, IoLatency);
      } // ch loop

      //
      // Find out the round trip latency
      //
      RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                     "\nRound trip latency\n");
#ifdef DEBUG_CODE_BLOCK
      if (GetDebugLevel () & SDBG_MAX) {
        ReleasePrintControl ();
      }
#endif // DEBUG_CODE_BLOCK
      FindRoundTripLatency (Host, socket, dimm, rank, centerPoint);

      for (ch = 0; ch < MaxChDdr; ch++) {

        if (IsChannelEnabled (socket, ch) == FALSE) {
          continue;
        }

        rankList = GetRankNvList (Host, socket, ch, dimm);

        if (IsRankValid (socket, ch, dimm, rank, FALSE) == FALSE) {
          continue;
        }

        logRank = GetLogicalRank (Host, socket, ch, dimm, rank);

        if (FeaturePcdGet (PcdCteBuild) == FALSE) {
          SetMprModeFilterActPre (Host, socket, ch, 0);
        }
        //
        // Find diff IO latency
        //
        FindMaxMinLogic (Host, socket, ch, dimm, rank, MSVx4, &MaxLogicDelay, &MinLogicDelay);

        //
        // Setup the new Pi setting
        //
        for (strobe = 0; strobe < MSVx4; strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if (IsStrobeNotValid (Host, strobe)) {
            continue;
          }
          if (GetPreambleState (socket, READ_PREAMBLE) == PREAMBLE_1TCLK) {
            centerPoint[ch][strobe] = centerPoint[ch][strobe] + 32;
          } else {
            if (centerPoint[ch][strobe] < 32) {
              //
              // Need to adjust the center point down but there is not enough range, so subtract 1 qclk from roundtrip
              //  and add corresponding number pi ticks equal to a cycle to all strobes center values for this rank
              //
              GetRoundTrip (socket, ch, logRank, GSM_READ_ONLY, &RoundTrip);
              RoundTrip = RoundTrip - 2;
              SetRoundTrip (socket, ch, logRank, RoundTrip);
              for (TempStrobe = 0; TempStrobe < MSVx4; TempStrobe++) {
                centerPoint[ch][TempStrobe] = centerPoint[ch][TempStrobe] + 128;
                GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, TempStrobe, 0, DdrLevel, RecEnDelay, GSM_UPDATE_CACHE,
                  (INT16 *)&centerPoint[ch][TempStrobe]);
              } //TempStrobe
            }
            centerPoint[ch][strobe] = centerPoint[ch][strobe] - 32;
          } //readPreamble
          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, RecEnDelay, GSM_UPDATE_CACHE,
            (INT16 *)&centerPoint[ch][strobe]);
        } // strobe loop

        FindMaxMinLogic (Host, socket, ch, dimm, rank, MSVx4, &MaxLogicDelay, &MinLogicDelay);

        //
        // Disable Receive Enable Mode in Data Buffer
        //
        SetDbReceiveEnableMode (Host, socket, ch, dimm, rank, 0);

        //
        // If smallest logic-delay 2 or more error in procedure
        //
        if (MinLogicDelay > 2) {
          (*rankList)[rank].faultyParts[0] |= FPT_REC_ENABLE_FAILED;
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                         "ERROR: smallest logic-delay > 2, it is %d\n", MinLogicDelay);
          DisableChannelSw (Host, socket, ch);
          EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_RD_RCVEN, socket, ch, NO_DIMM, NO_RANK, EwlSeverityWarning, NO_STROBE, NO_BIT, RecEnDelay, DdrLevel, (UINT8) pulseWidth[ch][strobe]);

        }

#ifdef DEBUG_CODE_BLOCK
        //
        // Summarization
        //
        if (GetDebugLevel () & SDBG_MAX) {
          AcquirePrintControl ();
          if (Host->var.mem.earlyCtlClkSerialDebugFlag) {
            RcDebugPrint (SDBG_DEFAULT, "\n Early CTL CLK Receive Enable Summary\n");
          } else {
            RcDebugPrint (SDBG_DEFAULT, "\n Receive Enable Summary\n");
          }
          RcDebugPrint (SDBG_DEFAULT, "------------------------\n");
          if (Host->var.mem.earlyCtlClkSerialDebugFlag) {
            RcDebugPrint (SDBG_DEFAULT, "START_DATA_EARLY_CTL_CLK_REC_EN\n");
          } else {
            RcDebugPrint (SDBG_DEFAULT, "START_DATA_REC_EN_BASIC\n");
          }
          for (strobe = 0; strobe < MSVx4; strobe++) {
            //
            // Skip if this is an ECC strobe when ECC is disabled
            //
            if (IsStrobeNotValid (Host, strobe)) {
              continue;
            }

            GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, RecEnDelay, GSM_READ_ONLY, &piDelay);
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT, "Pi setting = %d\n", piDelay);
          } // strobe loop

          GetIoLatency (socket, ch, logRank, GSM_READ_ONLY, &IoLatency);
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                        "IO Latency = %d\n", IoLatency);
          GetRoundTrip (socket, ch, logRank, GSM_READ_ONLY, &RoundTrip);
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                        "Round Trip = %d\n", RoundTrip + ioCompDiff);
          if (Host->var.mem.earlyCtlClkSerialDebugFlag) {
            RcDebugPrint (SDBG_DEFAULT, "STOP_DATA_EARLY_CTL_CLK_REC_EN\n");
          } else {
            RcDebugPrint (SDBG_DEFAULT, "STOP_DATA_REC_EN_BASIC\n");
          }
          ReleasePrintControl ();
        }
#endif // DEBUG_CODE_BLOCK
      } // ch loop

      //
      // Notify IO to move out of RCVEN training mode
      //
      SetTrainingMode (Host, socket, dimm, rank, CLEAR_MODE);

      //
      // Disable Receive Enable Mode in DRAM
      // BRS_TODO: Could this be moved earlier where DB disable occurs?
      //
      for (ch = 0; ch < MaxChDdr; ch++) {

        if (IsChannelEnabled (socket, ch) == FALSE) {
          continue;
        }

        if (IsRankValid (socket, ch, dimm, rank, FALSE) == FALSE) {
          continue;
        }

        SetDramReceiveEnableMode (Host, socket, ch, dimm, rank, 0);
      } // ch loop
    } // rank loop
  } // dimm loop

  //
  // Set IO compensation delay to the final value
  //
  for (ch = 0; ch < MaxChDdr; ch++) {

    if (IsChannelEnabled (socket, ch) == FALSE) {
      continue;
    }

    SetMprModeFilterActPre (Host, socket, ch, 0);
    rtError = 0;

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {

      for (rank = 0; rank < GetNumberOfRanksOnDimm (socket, ch, dimm); rank++) {

        if (IsRankValid (socket, ch, dimm, rank, FALSE) == FALSE) {
          continue;
        }
        //
        // Get the logical rank #
        //
        logRank = GetLogicalRank (Host, socket, ch, dimm, rank);

        GetRoundTrip (socket, ch, logRank, GSM_READ_ONLY, &temp);

        // If RT value + IO comp diff <= 55, then update RT = RT + IO comp diff
        temp = temp + ioCompDiff;
        RtRet = SetRoundTrip (socket, ch, logRank, temp);
        if (RtRet == EFI_OUT_OF_RESOURCES) {
          EwlOutputType2 (WARN_FPT_CORRECTABLE_ERROR, WARN_ROUNDTRIP_EXCEEDED, socket, ch, dimm, rank, EwlSeverityWarning, NO_STROBE, NO_BIT, RecEnDelay, DdrLevel, (UINT8) pulseWidth[ch][strobe]);
          rtError = 1;
        }
      } // rank loop
    } // dimm loop
    SetIOLatencyComp (Host, socket, ch, NO_SUBCH, rtError, ActToActSave);
  } // ch loop

  //
  // Turn off MPR pattern for all ranks
  //
  SetAllRankMPR (Host, socket, 0);

  //
  // Normalize RT/IO latency
  //
  NormalizeRoundTripAndIoLatency (socket);

  //
  // Set SenseAmpDelay and ODT Delay and durations
  //
  SetSenseAmpODTDelays (Host, socket);

  //
  //  Average Rcven delays
  //
  AverageRecEnableDelay (socket);

  //
  // Silicon Specific hook for any silicon programming after Recieve Enable training
  //
  RestoreChipRxEnableParams (&ChipDataPtr);

  //
  // Reset DDR IO
  //
  IO_Reset (Host, socket);
  FifoTrainReset (Host, socket);

  return SUCCESS;
} // ReceiveEnable


/**

  Find the edges of the signal

  @param Host              - Pointer to sysHost
  @param socket              - Socket number
  @param dimm              - DIMM number
  @param rank              - Rank number
  @param risingEdge        - Pointer to save the rising edge
  @param centerPoint       - Pointer to save the center point
  @param fallingEdge       - Pointer to save the falling edge
  @param pulseWidth        - Pointer to save the pulse width

  @retval N/A

**/
STATIC
VOID
FindPiSetting (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     dimm,
  UINT8     rank,
  UINT16    risingEdge[MAX_CH][MAX_STROBE],
  UINT16    centerPoint[MAX_CH][MAX_STROBE],
  UINT16    fallingEdge[MAX_CH][MAX_STROBE],
  UINT16    pulseWidth[MAX_CH][MAX_STROBE]
  )
{
  UINT8               ch;
  UINT8               strobe;
  UINT8               faultyPartsStatus;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  faultyPartsStatus = FPT_NO_ERROR;

  //
  // Starting by sampling all the SDRAM present
  //
  for (ch = 0; ch < MaxChDdr; ch++) {

    if (IsChannelEnabled (socket, ch) == FALSE) {
      continue;
    }

    if (IsDimmPresent (socket, ch, dimm) == FALSE) {
      Host->var.mem.piSettingStopFlag[ch] = 0;
      continue;
    }

    if (IsRankValid (socket, ch, dimm, rank, FALSE) == FALSE) {
      Host->var.mem.piSettingStopFlag[ch] = 0;
      continue;
    }

    Host->var.mem.piSettingStopFlag[ch] = GetStrobeMask (Host);
  } // ch loop

  PISettingAdjustment (Host, socket, dimm, rank);

  for (ch = 0; ch < MaxChDdr; ch++) {

    if (IsChannelEnabled (socket, ch) == FALSE) {
      continue;
    }

    if (IsRankValid (socket, ch, dimm, rank, FALSE) == FALSE) {
      continue;
    }

#ifdef DEBUG_CODE_BLOCK
    if (GetDebugLevel () & SDBG_MAX) {
      AcquirePrintControl ();
      if (Host->var.mem.earlyCtlClkSerialDebugFlag) {
        RcDebugPrint (SDBG_DEFAULT, "\nSummary: Early Ctl Clk Receive Enable Pi\nS%d, Ch%d, DIMM%d, Rank%d\n", socket, ch, dimm, rank);
      } else {
        RcDebugPrint (SDBG_DEFAULT, "\nSummary: Receive Enable Pi\nS%d, Ch%d, DIMM%d, Rank%d\n", socket, ch, dimm, rank);
      }
      PrintSampleArray (Host, socket, ch, dimm, 1);
      ReleasePrintControl ();
    }
#endif // DEBUG_CODE_BLOCK
    for (strobe = 0; strobe < MSVx4; strobe++) {
      //
      // Skip if this is an ECC strobe when ECC is disabled
      //
      if (IsStrobeNotValid (Host, strobe)) {
        continue;
      }

      EvaluatePiSettingResults (Host, socket, ch, dimm, rank, strobe, risingEdge, centerPoint, fallingEdge, pulseWidth,
        FPT_PI_READ_TYPE);

      if (Host->nvram.mem.socket[socket].FaultyPartsFlag[ch] & (1 << strobe)) {
        //
        // Add this error to the warning log for both correctable and uncorrectable errors.
        //
        faultyPartsStatus = EvaluateFaultyParts (Host, socket, ch);

        switch (faultyPartsStatus) {
        case FPT_NO_ERROR:
          break;
        case FPT_CORRECTABLE_ERROR:
          // if correctable  log the warning -> OutputWarning
          // careful for cases when warning gets promoted to error !!!
          EwlOutputType2 (WARN_FPT_CORRECTABLE_ERROR, WARN_FPT_MINOR_RD_RCVEN, socket, ch, dimm, rank, EwlSeverityWarning, strobe, NO_BIT, RecEnDelay, DdrLevel, (UINT8) pulseWidth[ch][strobe]);
          break;
        default:
          RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                         "RecEn training failure!!!\n");
          //
          // Add this error to the warning log for both correctable and uncorrectable errors.
          //
          Host->var.mem.piSettingStopFlag[ch] |= (1 << strobe);
          DisableChannelSw (Host, socket, ch);
          EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_RD_RCVEN, socket, ch, dimm, rank, EwlSeverityWarning, strobe, NO_BIT, RecEnDelay, DdrLevel, (UINT8) pulseWidth[ch][strobe]);
          break;
        }
      }
    } // strobe loop

#ifdef DEBUG_CODE_BLOCK
    DisplayEdges (Host, socket, ch, pulseWidth[ch], fallingEdge[ch], risingEdge[ch], centerPoint[ch], MSVx4);
#endif // DEBUG_CODE_BLOCK
  } // ch loop

  return;
} // FindPiSetting

/**

  This function change 128 times the pi setting for each rank channel.

  @param Host              - Pointer to sysHost
  @param socket              - Socket number
  @param dimm              - DIMM number
  @param rank              - Rank number

  @retval N/A

**/
STATIC
VOID
PISettingAdjustment (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    dimm,
  UINT8    rank
  )
{
  UINT8                   ch;
  UINT32                  chBitmask;
  UINT16                  fineDelay;
  UINT16                  fineDelayEnd;
  struct TrainingResults (*trainRes)[MAX_CH][MAX_STROBE];
  UINT8                   DumArr[7] = { 1, 1, 1, 1, 1, 1, 1 };
  UINT8                   MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  trainRes = &Host->var.mem.trainRes;

  //
  // Clear training results
  //
  ZeroMem ((UINT8 *) trainRes, sizeof (*trainRes));

  chBitmask = GetChBitmask (Host, socket, dimm, rank);

  SetupRecEnFineTest (Host, socket, chBitmask, dimm, rank);

  //
  //
  //
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                 "RecEn Pi Scanning:\n");

  fineDelayEnd = REC_EN_PI_START + REC_EN_PI_RANGE;

  for (fineDelay = REC_EN_PI_START; fineDelay < fineDelayEnd; fineDelay += REC_EN_STEP_SIZE) {

    //
    // Change the Receive Enable PI Setting
    //
    for (ch = 0; ch < MaxChDdr; ch++) {

      if (IsChannelEnabled (socket, ch) == FALSE) {
        continue;
      }

      if (IsRankValid (socket, ch, dimm, rank, FALSE) == FALSE) {
        continue;
      }

      GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, 0, DdrLevel, RecEnDelay, GSM_UPDATE_CACHE,
        (INT16 *)&fineDelay);
    } // ch loop

    //
    // Send a burst of 7 read commands back to back (4 DCLK apart) using MC_SPECIAL_COMMAND CRs
    //

    if (Host->DdrioUltSupport) {

      CteSetOptions ("io_reset", chBitmask, 0x1, 0x0, 1);
      CteDelayDclk (8);
      CteSetOptions ("io_reset", chBitmask, 0x1, 0x0, 0);
      CteStartPattern (chBitmask, 0x1, 0);//Set the rank parameter as 0 (Acutually, the rank parameter here is not needed.) and set the sub channel mask value as 1 for ddr4.

    } else {
      RunIOTest (Host, socket, chBitmask, BasicVA, DumArr, 1, 0);

    }

    if (Host->DdrioUltSupport) {
      CteWaitPatternDone ();
    }

    //
    // Delay if this is a CTE build, otherwise do nothing.
    //

    CteDelayDclk (100); // To prevent IO_RESET in the middle of the data burst corrupting previous sequence.

    GetResults (Host, socket, dimm, rank, fineDelay);

  } // fineDelay loop

  return;
} // PISettingAdjustment

/**

  This function gets the test engine ready to run tests for Receive Enable Fine training

  @param[in] Host       - Pointer to sysHost, the system Host (root) structure struct
  @param[in] Socket     - Current Socket
  @param[in] ChBitmask  - Bitmask of channels to test
  @param[in] Dimm       - Dimm to test
  @param[in] Rank       - Rank to test

  @retval None

**/
VOID
SetupRecEnFineTest (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT32   ChBitmask,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  UINT8               LoopCount;
  UINT8               NumCl;
  MRC_RT              MrcRankList;
  UINT8               MemSsType;

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  MrcRankList.Dimm = Dimm;
  MrcRankList.Rank = Rank;
  //
  // Only take the DdrioUltSupport path on CTE builds
  //

  if (Host->DdrioUltSupport) {

    LoopCount = 2;
    NumCl = 32;

    CteSetupPattern ("rcven", (LoopCount) | (NumCl << 4), ChBitmask, 1, Rank);  //set the subchannelmask as 1 for ddr4.
  } else {
    SetupTest (Host, Socket, MemSsType, ChBitmask, 1, &MrcRankList, DdrLevel, RcvenFineTest, gsmCsnDelim, 0, FIRST_ITERATION_LOOP);
  }

  return;

} // SetupRecEnFineTest

/**

 For each data module, a 128 bit vector of zeros and ones holding the tests results is
 evaluated which is a concatenation of the two reads from the feedback CRs.
 BIOS locates the middle of the longest consecutive stream of ones (when the vector is evaluated as cyclic,
 this location in the vector indicates the PI setting and logic delay for that data module).

  @param Host              - Pointer to sysHost
  @param socket            - Current Socket
  @param ch                - Channel number
  @param dimm              - Dimm number on ch
  @param rank              - Rank on dimm
  @param strobe            - Strobe number
  @param risingEdge        - Pointer to save the rising edge
  @param centerPoint       - Pointer to save the center point
  @param fallingEdge       - Pointer to save the falling edge
  @param pulseWidth        - Pointer to save the pulse width
  @param step              - The PI Step to work on:
                               FPT_PI_READ_TYPE FPT_PI_WRITE_TYPE
                               FPT_PI_LRDIMM_READ_TYPE
                               FPT_PI_LRDIMM_WRITE_TYPE
                               FPT_PI_LRDIMM_RD_MRD_TYPE
                               FPT_PI_LRDIMM_WR_MRD_TYPE

  @retval N/A

**/
VOID
EvaluatePiSettingResults (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT16    risingEdge[MAX_CH][MAX_STROBE],
  UINT16    centerPoint[MAX_CH][MAX_STROBE],
  UINT16    fallingEdge[MAX_CH][MAX_STROBE],
  UINT16    pulseWidth[MAX_CH][MAX_STROBE],
  UINT8     step
  )
{
  UINT16                  piSetting;
  UINT16                  piStart;
  UINT32                  Register;
  UINT32                  Bit;
  UINT8                   CyclicCase;
  UINT16                  OneCount;
  UINT16                  OneStart;
  UINT16                  NumberOfCycle;
  UINT16                  MaxOneCount;
  UINT8                   dWord;
  struct ddrRank          (*rankList)[MAX_RANK_DIMM];
  struct TrainingResults  (*trainRes)[MAX_CH][MAX_STROBE];

  Register = 0;
  OneStart = 0;

  Bit = BIT0;
  OneCount = 0;
  MaxOneCount = 0;
  CyclicCase = 0;
  trainRes = &Host->var.mem.trainRes;

  risingEdge[ch][strobe] = 0;
  fallingEdge[ch][strobe] = 0;
  pulseWidth[ch][strobe] = 0;

  rankList = GetRankNvList (Host, socket, ch, dimm);

  //
  // Start at bit 0
  //
  SetupPiSettingsParams (Host, socket, step, &piSetting, &piStart, &NumberOfCycle, &trainRes);

  while (Host->var.mem.piSettingStopFlag[ch] & (1 << strobe)) {

    dWord = GetTrainingResultIndex (step, piSetting, piStart);

    //
    // Get the proper training results
    //
    if ((piSetting % 32) == 0) {
      Register = (*trainRes)[ch][strobe].results[dWord];
      Bit = BIT0;
    }
    //
    // algo start
    //
    if (Register & Bit) {
      //
      // this index is 1
      //
      if (OneCount == 0) {
        //
        // This first one so save the starting point
        //
        OneStart = piSetting;
      }

      OneCount++;
    } else {
      //
      // this index is 0
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
          risingEdge[ch][strobe] = OneStart;
          fallingEdge[ch][strobe] = piSetting;
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
    piSetting++;
    Bit = Bit << 1;

    //
    // Check if we're at the end of the cycle
    //
    if (piSetting == NumberOfCycle) {
      if (OneCount > 0) {
        //
        // Cyclic case continue to scan
        // Start back over with bit 0 if the last bit is a 1
        //
        piSetting = StartBackOver (step, piStart);
        CyclicCase = 1;
      } else {
        //
        // stop the scan
        // We've checked all bits so save the results
        //
        Host->var.mem.piSettingStopFlag[ch] &= ~(1 << strobe);
        pulseWidth[ch][strobe] = fallingEdge[ch][strobe] - risingEdge[ch][strobe];
        centerPoint[ch][strobe] = ((pulseWidth[ch][strobe] / 2) + risingEdge[ch][strobe]) % (NumberOfCycle - piStart);
      }
    }
    //
    // Check if this is a cyclic case and if we are at the end of the one's
    //
    if ((CyclicCase == 1) && (OneCount == 0)) {
      Host->var.mem.piSettingStopFlag[ch] &= ~(1 << strobe);
      if (risingEdge[ch][strobe] > fallingEdge[ch][strobe]) {
        pulseWidth[ch][strobe] = ((NumberOfCycle - risingEdge[ch][strobe]) + fallingEdge[ch][strobe]) - piStart;
        centerPoint[ch][strobe] = (((pulseWidth[ch][strobe] / 2) + risingEdge[ch][strobe])) % (NumberOfCycle - piStart);
      } else {
        //
        // last bit is 1 and first bit after cycle is 0 bit 127 = 1 and bit 0 = 0
        //
        pulseWidth[ch][strobe] = fallingEdge[ch][strobe] - risingEdge[ch][strobe];
        centerPoint[ch][strobe] = ((pulseWidth[ch][strobe] / 2) + risingEdge[ch][strobe]) % (NumberOfCycle - piStart);
      }
    }
  } // while loop

  if (FAULTY_PARTS_TRACKING_INJECT_ERROR == 1) {
    if ((ch == 0) && (dimm == 0) && (rank == 1) && (strobe == 5)) {
      //_asm jmp $;
      pulseWidth[ch][strobe] = 1;
      centerPoint[ch][strobe] = ((pulseWidth[ch][strobe] / 2) + risingEdge[ch][strobe]) % (NumberOfCycle - piStart);
    }
  } // FAULTY_PARTS_TRACKING_INJECT_ERROR

  //
  // If the pulseWidth (ones stream) is smaller than threshold need to sample again
  //
  if ((step == FPT_PI_READ_TYPE) || (step == FPT_PI_WRITE_TYPE)) {
    if (pulseWidth[ch][strobe] <= RE_EDGE_SIZE_THRESHOLD) {

      //
      // This is a fatal error condition so map out the entire channel
      //
      Host->nvram.mem.socket[socket].FaultyPartsFlag[ch] |= (1 << strobe);
      if (step == FPT_PI_READ_TYPE) {
        (*rankList)[rank].faultyParts[strobe] |= FPT_REC_ENABLE_FAILED;

        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                       "Failed RecEn Pi\n");
      }
      if (step == FPT_PI_WRITE_TYPE) {
        (*rankList)[rank].faultyParts[strobe] |= FPT_WR_LEVELING_FAILED;
        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                       "Failed WL Pi\n");
        // Evaluate here for correctable error  ... possibly disable channel
      }
    }
  }

#ifdef LRDIMM_SUPPORT
  //
  // HSD4929028: Error checking for LRDIMM
  //
  if ((step == FPT_PI_LRDIMM_READ_TYPE) ||
      (step == FPT_PI_LRDIMM_WRITE_TYPE)) {
    if (pulseWidth[ch][strobe] <= RE_LRDIMM_EDGE_SMALL_THRESHOLD_PHASE) {
      //
      // De-swizzle NVMDIMM backkside strobe for FPT tracking
      //

      if (IsDcpmmPresentDimm (socket, ch, dimm)) {
        strobe = CheckBacksideSwizzle (Host, socket, ch, dimm, strobe);
      }

      Host->nvram.mem.socket[socket].FaultyPartsFlag[ch] |= (1 << strobe);
      (*rankList)[rank].faultyParts[strobe] |= FPT_LRDIMM_TRAINING_FAILED;
#ifdef DDRT_SUPPORT

      if (IsDcpmmPresentDimm (socket, ch, dimm) == TRUE) {
        if (step == FPT_PI_LRDIMM_READ_TYPE) {
          RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                       "Failed PMem Pi in Backside Phase Training(RX)\n");
        } else {
          RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                       "Failed PMem Pi in Backside Fine WL Training(TX)\n");
        }
      }
#endif // DDRT_SUPPORT

      if (IsDcpmmPresentDimm (socket, ch, dimm) == FALSE) {
        if (step == FPT_PI_LRDIMM_READ_TYPE) {
          RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                       "Failed LRDIMM Pi in Backside Phase Training(RX)\n");
        } else {
          RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                       "Failed LRDIMM Pi in Backside Fine WL Training(TX)\n");
        }
      }
    }
  }
  if ((step == FPT_PI_LRDIMM_RD_MRD_TYPE) ||
      (step == FPT_PI_LRDIMM_WR_MRD_TYPE)) {
    if (pulseWidth[ch][strobe] <= RE_LRDIMM_EDGE_SIZE_THRESHOLD) {
      //
      // De-swizzle NVMDIMM backside strobe for FPT tracking
      //
      if (IsDcpmmPresentDimm (socket, ch, dimm) == TRUE) {
        strobe = CheckBacksideSwizzle (Host, socket, ch, dimm, strobe);
      }

      Host->nvram.mem.socket[socket].FaultyPartsFlag[ch] |= (1 << strobe);
      (*rankList)[rank].faultyParts[strobe] |= FPT_LRDIMM_TRAINING_FAILED;
#ifdef DDRT_SUPPORT

      if (IsDcpmmPresentDimm (socket, ch, dimm) == TRUE) {
        if (step == FPT_PI_LRDIMM_RD_MRD_TYPE) {
          RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                       "Failed PMem Pi in Backside Delay Training(RX)\n");
        } else {
          RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                       "Failed PMem Pi in Backside Delay Training(TX)\n");
        }
      }
#endif // DDRT_SUPPORT

      if (IsDcpmmPresentDimm (socket, ch, dimm) == FALSE) {
        if (step == FPT_PI_LRDIMM_RD_MRD_TYPE) {
          RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                       "Failed LRDIMM Pi in Backside Delay Training(RX)\n");
        } else {
          RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                       "Failed LRDIMM Pi in Backside Delay Training(TX)\n");
        }
      }
    }
  }
#endif //Lrdimm_support
}

/**

  Once the DQS high phase has been found (for each DRAM) the next stage is to find out the round trip latency,
  by locating the preamble cycle. This is achieved by trying smaller and smaller roundtrip
  values until the strobe sampling is done on the preamble cycle.

  @param Host              - Pointer to sysHost
  @param socket              - Socket number
  @param dimm              - DIMM number
  @param rank              - Rank number
  @param centerPoint       - Pointer to center point

  @retval N/A

**/
STATIC
VOID
FindRoundTripLatency (
  PSYSHOST                            Host,
  UINT8                               socket,
  UINT8                               dimm,
  UINT8                               rank,
  UINT16                              centerPoint[MAX_CH][MAX_STROBE]
  )
{
  UINT8               ch;
  UINT32              chBitmask;
  UINT8               strobe;
  UINT8               logRank = 0;
  UINT8               stopFlag;
  UINT8               haveZero;
  UINT8               RoundTrip;
  UINT8               IoLatency;
  UINT16              tmpPiDelay;
  UINT32              feedBack = 0;
  UINT32              zeroFlag;
  UINT32              allZeros[MAX_CH];
  UINT32              ShiftIndex;
  UINT32              Register;
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  UINT8               aux_strobe;
  UINT8               DumArr[7] = { 1, 1, 1, 1, 1, 1, 1 };
  UINT16              maxLimit = 0;
  UINT16              minLimit = 0;
  UINT16              usDelay = 0;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Register = 0;

  for (ch = 0; ch < MaxChDdr; ch++) {
    allZeros[ch] = 0;

    if (IsChannelEnabled (socket, ch) == FALSE) {
      continue;
    }

    if (IsRankValid (socket, ch, dimm, rank, FALSE) == FALSE) {
      continue;
    }

    //
    // Get the logical rank #
    //
    logRank = GetLogicalRank (Host, socket, ch, dimm, rank);

    allZeros[ch] = GetStrobeMask (Host);
  } // ch loop

  chBitmask = GetChBitmask (Host, socket, dimm, rank);
  GetDataGroupLimits(Host, DdrLevel, RecEnDelay, &minLimit, &maxLimit, &usDelay); // move this out of loops

  stopFlag = 0;

  while (stopFlag == 0) {
    //
    // Send a burst of 16 read commands back to back (4 DCLK apart)
    //
    RunIOTest (Host, socket, chBitmask, BasicVA, DumArr, 1, 0);

    if (FeaturePcdGet (PcdCosimBuild) == TRUE) {
      CteDelayDclk (100); // To prevent IO_RESET in the middle of the data burst corrupting previous sequence
    }


    stopFlag = 1;

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((chBitmask & (1 << ch)) == 0) {
        continue;
      }

      //
      // Continue if this rank is not present
      //
      rankList = GetRankNvList (Host, socket, ch, dimm);

      if (IsRankValid (socket, ch, dimm, rank, FALSE) == FALSE) {
        continue;
      }

      haveZero = 0;
      zeroFlag = 0;

      //
      // BIOS reads result from IO CRs and extract the relevant bit for each data module.
      //
      for (strobe = 0; strobe < MSVx4; strobe++) {
        //
        // Skip if this is an ECC strobe when ECC is disabled
        //
        if (IsStrobeNotValid (Host, strobe)) {
          continue;
        }

        ShiftIndex = centerPoint[ch][strobe] % RE_TRAINING_RESULT_HIGH_IN_BITS;

        if (Host->DdrioUltSupport) {
          CteWaitPatternDone ();
        }

        //
        // Read the correct register to work on
        //
        feedBack = ReadTrainFeedback (Host, socket, ch, strobe);

        //if (feedBack >= (UINT8)(1 << (REC_EN_LOOP_COUNT - 1))) {
        if (feedBack >= 1) {
          Register = 1 << ShiftIndex;
        } else {
          Register = 0;
        }
        if ((*rankList)[rank].faultyParts[strobe] != 0) {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                         "Skip RecEn haveZero & zeroFlag\n");
          // do nothing here we will fool the system that the faulty part is working later when we see other strobes to be working too
        } else {
          //
          // Test if DQS value is 0 or 1
          //
          if ((Register & (1 << ShiftIndex)) == 0) {
            //
            // SDRAM i index value is 0
            //
            haveZero = 1;
            zeroFlag |= 1 << strobe;

            // if some other strobe is ok lets append the faulty strobe as working too - to advance the algorithm
            for (aux_strobe = 0; aux_strobe < MSVx4; aux_strobe++) {
              if ((*rankList)[rank].faultyParts[aux_strobe] != 0) {
                RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, aux_strobe, NO_BIT,
                               "Adding faulty strobe RecEn\n");
                zeroFlag |= 1 << aux_strobe;
              }
            } // aux_strobe loop
          }
        }
      } // strobe loop

      if (haveZero) {
        if (zeroFlag != allZeros[ch]) {
          stopFlag = 0;

          //
          // increase the Rank # IO latency by 2 and continue to next iteration.
          //
          GetIoLatency (socket, ch, logRank, GSM_READ_ONLY, &IoLatency);
          IoLatency = IoLatency + 2;
          SetIOLatency (socket, ch, logRank, IoLatency);
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                                  "IO Latency = %d\nzeroFlag = 0x%x, allZeros[%d] = 0x%x\n",
                                  IoLatency, zeroFlag, ch, allZeros[ch]);

          //  If some are zero, on next iteration increase their RX RcvEnable Logic Delay settings by 2
          //
          for (strobe = 0; strobe < MSVx4; strobe++) {
            //
            // Skip if this is an ECC strobe when ECC is disabled
            //
            if (IsStrobeNotValid (Host, strobe)) {
              continue;
            }

            if (zeroFlag & (1 << strobe)) {
              //
              // Increase logic delay by 2
              //
              GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, RecEnDelay, GSM_READ_ONLY,
                (INT16 *)&tmpPiDelay);
              tmpPiDelay = tmpPiDelay + (2 * 64);

              //
              // TODO: Check against a max pi delay instead of of max logic delay
              //
              if (tmpPiDelay > maxLimit) {
                tmpPiDelay = maxLimit;
                RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                               "ERROR: Rec En Logic Delay is %d > %d *64!!!\n", tmpPiDelay, RE_MAX_LOGIC_DELAY);
                EwlOutputType2 (WARN_FPT_CORRECTABLE_ERROR, WARN_RCVEN_PI_DELAY_EXCEEDED, socket, ch, dimm, rank, EwlSeverityWarning, strobe, NO_BIT, RecEnDelay, DdrLevel, 0xFF);
                DisableChannelSw (Host, socket, ch);
              }

              centerPoint[ch][strobe] = tmpPiDelay;
              GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, RecEnDelay, GSM_UPDATE_CACHE,
                (INT16 *)&tmpPiDelay);
              RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                             "Rec En Delay = %d\n", tmpPiDelay);
            } // ZeroFlag
          } // strobe loop
        } // IterationStop
        else {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                         "Round trip latency: Found all zeros\n");
          //
          // if all are 0 stop the loop
          // No need to test this channel anymore
          //
          chBitmask &= ~(1 << ch);
        }
      } else {
        //
        // If all bits are one, decrement roundtrip value by DECREMENT_ROUNDTRIP_VALUE, and continue to next iteration.
        //
        stopFlag = 0;
        GetRoundTrip (socket, ch, logRank, GSM_READ_ONLY, &RoundTrip);
        if (RoundTrip < DECREMENT_ROUNDTRIP_VALUE) {
          //
          // No need to test this channel anymore
          //
          chBitmask &= ~(1 << ch);
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                         "WARNING: Unable to find round trip latency!!!\n");
          DisableChannelSw (Host, socket, ch);
          EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_RD_RCVEN, socket, ch, dimm, rank, EwlSeverityWarning, NO_STROBE, NO_BIT, RecEnDelay, DdrLevel, 0xFF);
        } else {
          RoundTrip -= DECREMENT_ROUNDTRIP_VALUE;
        }
        SetRoundTrip (socket, ch, logRank, RoundTrip);
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                                "Round trip latency: No zeros found\nRound trip latency = %d\n",
                                RoundTrip);
      }
    } // ch loop
  } // while loop

  return;
} // FindRoundTripLatency

/**

  This function find the logic delay max and logic delay min

  @param Host            - Pointer to sysHost
  @param socket            - Socket number
  @param ch              - Channel number
  @param dimm            - DIMM number
  @param rank            - Rank number
  @param MaxSdramInDimm  - Number of sdram in the current DIMM
  MaxLogicDelay   - Return logic delay max value
  MinLogicDelay   - Return logic delay min value

  @retval N/A

**/
VOID
FindMaxMinLogic (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     MaxSdramInDimm,
  UINT8     *MaxLogicDelay,
  UINT8     *MinLogicDelay
  )
{
  UINT8   strobe;
  UINT16  tmpLogicDelay;

  *MaxLogicDelay = 0;
  *MinLogicDelay = 16;

  for (strobe = 0; strobe < MaxSdramInDimm; strobe++) {
    //
    // Get this logic delay
    //
    GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, RecEnDelay, GSM_READ_ONLY, (INT16 *)&tmpLogicDelay);
    tmpLogicDelay = tmpLogicDelay / 64;

    //
    // Check if this delay is less than the minimum delay
    //
    if ((UINT8) tmpLogicDelay < *MinLogicDelay) {
      *MinLogicDelay = (UINT8) tmpLogicDelay;
    }

    //
    // Check if this delay is more than the maximum delay
    //
    if (*MaxLogicDelay < (UINT8) tmpLogicDelay) {
      *MaxLogicDelay = (UINT8) tmpLogicDelay;
    }
  } // strobe loop
} // FindMaxMinLogic

#ifdef DEBUG_CODE_BLOCK
/**

  This function print the sample array

  @param Host      - Pointer to sysHost
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
PrintSampleArray (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    ReadMode
  )
{
  UINT8   strobe;
  UINT8   maxStrobe;
  UINT32  piSetting;
  struct TrainingResults (*trainRes)[MAX_CH][MAX_STROBE];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;

  trainRes = &Host->var.mem.trainRes;
#ifdef LRDIMM_SUPPORT
  if (ReadMode == PRINT_ARRAY_TYPE_LRDIMM_MDQ_WR_DELAY) {
    trainRes = &Host->var.mem.lrMwdTrainRes;
  }
  if (ReadMode == PRINT_ARRAY_TYPE_LRDIMM_MDQ_RD_DELAY) {
    trainRes = &Host->var.mem.lrMrdTrainRes;
  }
  if (ReadMode == PRINT_ARRAY_TYPE_LRDIMM_RCV_EN_PHASE) {
    trainRes = &Host->var.mem.lrTrainRes;
  }
  if (ReadMode == PRINT_ARRAY_TYPE_LRDIMM_WL_PHASE) {
    trainRes = &Host->var.mem.lrDwlTrainRes;
  }
#endif // LRDIMM_SUPPORT

  if ((ReadMode == 0) && (IsX4Dimm (socket, ch, dimm) == FALSE)) {
    maxStrobe = MSVx8;
    RcDebugPrintLine (SDBG_DEFAULT, (maxStrobe * 4) + 3, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  } else {
    maxStrobe = MSVx4;
    RcDebugPrintLine (SDBG_DEFAULT, (maxStrobe * 4) + 13, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  }
  RcDebugPrint (SDBG_DEFAULT, "    ");
  for (strobe = 0; strobe < maxStrobe; strobe++) {
    if (!IsStrobeNotValid (Host, strobe)) {
      RcDebugPrint (SDBG_DEFAULT, "%4d", strobe);
    }
  }
  RcDebugPrint (SDBG_DEFAULT, "\n");

  for (piSetting = 0; piSetting < (MAX_PHASE_IN_FINE_ADJUSTMENT * 2); piSetting++) {
    if ((ReadMode > 1) && (piSetting % 2)) {
      continue;
    }
    if (((ReadMode == PRINT_ARRAY_TYPE_LRDIMM_MDQ_RD_DELAY) || (ReadMode == PRINT_ARRAY_TYPE_LRDIMM_MDQ_WR_DELAY)) && (piSetting % 4)) {
      continue;
    }
    if ((ReadMode == PRINT_ARRAY_TYPE_LRDIMM_MDQ_RD_DELAY) || (ReadMode == PRINT_ARRAY_TYPE_LRDIMM_MDQ_WR_DELAY)) {
      RcDebugPrint (SDBG_DEFAULT, "%d ", (((INT32)piSetting / 4) - 15));

      if ((((INT32) piSetting / 4 - 15) >= 0) && (((INT32) piSetting / 4 - 15) < 10)) {
        RcDebugPrint (SDBG_DEFAULT, "  ");
      }

      if ((((INT32) piSetting / 4 - 15) < 100) && (((INT32) piSetting / 4 - 15) > 9)) {
        RcDebugPrint (SDBG_DEFAULT, " ");
      }

      if ((((INT32) piSetting / 4 - 15) < 0) && (((INT32) piSetting / 4 - 15) > -10)) {
        RcDebugPrint (SDBG_DEFAULT, " ");
      }

    } else {
      RcDebugPrint (SDBG_DEFAULT, "%d ", piSetting);

      if (piSetting < 10) {
        RcDebugPrint (SDBG_DEFAULT, "  ");
      }

      if ((piSetting < 100) && (piSetting > 9)) {
        RcDebugPrint (SDBG_DEFAULT, " ");
      }
    }
    for (strobe = 0; strobe < maxStrobe; strobe++) {
      if (IsStrobeNotValid (Host, strobe)) {
        continue;
      }
      if (((1 << piSetting) & (*trainRes)[ch][strobe].results[piSetting / 32]) != 0) {
        RcDebugPrint (SDBG_DEFAULT, "   1");
      } else {
        RcDebugPrint (SDBG_DEFAULT, "   0");
      }
    } // strobe loop

    RcDebugPrint (SDBG_DEFAULT, "\n");
  } // piSetting loop
} // PrintSampleArray


/**

  Prints the edges found

  @param Host      - Pointer to sysHost
  @param socket        - Current Socket
  @param ch        - Channel number
  @param pulseWidth    - Pointer to save the pulse width
  @param fallingEdge   - Pointer to save the falling edge
  @param risingEdge    - Pointer to save the rising edge
  @param centerPoint   - Pointer to save the center point
  @param maxStrobe     - Maximum number of strobes

  @retval N/A

**/
VOID
DisplayEdges (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT16    pulseWidth[MAX_STROBE],
  UINT16    fallingEdge[MAX_STROBE],
  UINT16    risingEdge[MAX_STROBE],
  UINT16    centerPoint[MAX_STROBE],
  UINT8     maxStrobe
  )
{
  UINT8 strobe;

  if (!(GetDebugLevel () & SDBG_MAX)) {
    return;
  }

  AcquirePrintControl ();

  RcDebugPrintLine (SDBG_MAX, (maxStrobe * 4) + 4, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

  // Print rising edges
  RcDebugPrint (SDBG_DEFAULT, "RE: ");
  for (strobe = 0; strobe < maxStrobe; strobe++) {
    if (IsStrobeNotValid (Host, strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, " %3d", risingEdge[strobe]);
  } // strobe loop

  // Print center points
  RcDebugPrint (SDBG_DEFAULT, "\nCP: ");
  for (strobe = 0; strobe < maxStrobe; strobe++) {
    if (IsStrobeNotValid (Host, strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, " %3d", centerPoint[strobe]);
  } // strobe loop

  // Print falling edges
  RcDebugPrint (SDBG_DEFAULT, "\nFE: ");
  for (strobe = 0; strobe < maxStrobe; strobe++) {
    if (IsStrobeNotValid (Host, strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, " %3d", fallingEdge[strobe]);
  } // strobe loop

  // Print pulse width
  RcDebugPrint (SDBG_DEFAULT, "\nPW: ");
  for (strobe = 0; strobe < maxStrobe; strobe++) {
    if (IsStrobeNotValid (Host, strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, " %3d", pulseWidth[strobe]);
  } // strobe loop
  RcDebugPrint (SDBG_DEFAULT, "\n\n");

  ReleasePrintControl ();
} // DisplayEdges


/**

  Prints the edges found for LRDIMMs

  @param Host      - Pointer to sysHost
  @param socket        - Current Socket
  @param ch        - Channel number
  @param pulseWidth    - Pointer to save the pulse width
  @param fallingEdge   - Pointer to save the falling edge
  @param risingEdge    - Pointer to save the rising edge
  @param centerPoint   - Pointer to save the center point
  @param maxStrobe     - Maximum number of strobes

  @retval N/A

**/
VOID
DisplayLrdimmMrdEdges (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT16    pulseWidth[MAX_STROBE],
  UINT16    fallingEdge[MAX_STROBE],
  UINT16    risingEdge[MAX_STROBE],
  UINT16    centerPoint[MAX_STROBE],
  UINT8     maxStrobe
  )
{
  UINT8 Strobe;

  if (!(GetDebugLevel () & SDBG_MAX)) {
    return;
  }

  AcquirePrintControl ();

  RcDebugPrintLine (SDBG_MAX, 76, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

  // Print rising edges
  RcDebugPrint (SDBG_DEFAULT, "RE: ");
  for (Strobe = 0; Strobe < maxStrobe; Strobe++) {
    if (IsStrobeNotValid (Host, Strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, " %3d", ((INT32) risingEdge[Strobe] / 4 - 15));
  } // Strobe loop

  // Print center points
  RcDebugPrint (SDBG_DEFAULT, "\nCP: ");
  for (Strobe = 0; Strobe < maxStrobe; Strobe++) {
    if (IsStrobeNotValid (Host, Strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, " %3d", ((INT32) centerPoint[Strobe] / 4 - 15));
  } // Strobe loop

  // Print falling edges
  RcDebugPrint (SDBG_DEFAULT, "\nFE: ");
  for (Strobe = 0; Strobe < maxStrobe; Strobe++) {
    if (IsStrobeNotValid (Host, Strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, " %3d", ((INT32) fallingEdge[Strobe] / 4 - 15));
  } // Strobe loop

  // Print pulse width
  RcDebugPrint (SDBG_DEFAULT, "\nPW: ");
  for (Strobe = 0; Strobe < maxStrobe; Strobe++) {
    if (IsStrobeNotValid (Host, Strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, " %3d", ((INT32) pulseWidth[Strobe] / 4));
  } // Strobe loop
  RcDebugPrint (SDBG_DEFAULT, "\n\n");

  ReleasePrintControl ();
} // DisplayLrdimmMrdEdges
#endif // DEBUG_CODE_BLOCK


/**

  Get's the resutls from the current test

  @param Host      - Pointer to sysHost
  @param socket    - Current Socket
  @param dimm      - DIMM number
  @param rank      - rank number of the DIMM
  @param piDelay   - CLK adjustment setting

  @retval N/A

**/
VOID
GetResults (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     dimm,
  UINT8     rank,
  UINT16    piDelay
  )
{
  UINT8               ch;
  UINT8               strobe;
  UINT32              csrReg;
  UINT8               MSVx4 = GetMaxStrobeValid (Host);
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  for (ch = 0; ch < MaxChDdr; ch++) {

    if (IsChannelEnabled (socket, ch) == FALSE) {
      continue;
    }

    if (IsRankValid (socket, ch, dimm, rank, FALSE) == FALSE) {
      continue;
    }

    for (strobe = 0; strobe < MSVx4; strobe++) {
      csrReg = ReadTrainFeedback (Host, socket, ch, strobe);

      RecEnTrainFeedback (Host, socket, ch, strobe, csrReg, piDelay);
    } // strobe loop
  } // ch loop
} // GetResults

/**

  Optimizes round trip latency.

  This function support DDR4 and some DDR5 iMC. For DDR5, this function only supports iMC
  which requires sub-channel A and B to use equal RTL latency.

  @param Host  - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
RoundTripOptimize (
  PSYSHOST  Host
  )
{
  UINT8                             socket;
  UINT8                             ch;
  UINT8                             dimm;
  UINT8                             rank;
  UINT8                             logRank;
  UINT8                             strobe;
  UINT16                            minDelay;
  UINT16                            maxDelay;
  UINT8                             clk;
  INT16                             piDelay[MAX_CH];
  INT16                             curDelay;
  UINT8                             ckEnabled[MAX_CLK];
  UINT8                             ctlIndex[MAX_CLK];
  UINT8                             worstMargin;
  UINT8                             eyeWidth[MAX_CH][MAX_RANK_CH][MAX_STROBE];
  UINT8                             goodRT[MAX_CH][MAX_RANK_CH];
  UINT8                             goodIO[MAX_CH][MAX_RANK_CH];
  UINT8                             orgRT[MAX_CH][MAX_RANK_CH];
  UINT8                             orgIO[MAX_CH][MAX_RANK_CH];
  UINT8                             rankDone[MAX_CH][MAX_RANK_CH];
  UINT8                             RoundTripLatency;
  UINT8                             IoLatency;
#ifdef DDR5_SUPPORT
  UINT8                             SubChannel;
  UINT8                             SubChGoodRT[MAX_CH][SUB_CH][MAX_RANK_CH];
  UINT8                             SubChGoodIO[MAX_CH][SUB_CH][MAX_RANK_CH];
  UINT8                             SubChOrgRT[MAX_CH][SUB_CH][MAX_RANK_CH];
  UINT8                             SubChOrgIO[MAX_CH][SUB_CH][MAX_RANK_CH];
#endif
  UINT8                             keepGoing;
  UINT8                             failed;
  UINT8                             minIoLatency;
  UINT8                             rtGuardBand;
  UINT8                             orgrunningRmt;
  UINT16                            minLimit;
  UINT16                            usDelay;
  UINT16                            maxCmdDelay;
  UINT16                            maxCtlDelay;
  UINT16                            maxRecEnDelay;
  UINT16                            maxTxDqDelay;
  UINT16                            maxTxDqsDelay;
  struct bitMask                    filter;
  struct strobeMargin               timeMargin;
  struct ddrRank                    (*rankList)[MAX_RANK_DIMM];
  UINT8                             MSVx4 = GetMaxStrobeValid (Host);
  UINT8                             MSVx4SubCh;
  SYS_SETUP                         *Setup;
  UINT8                             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  if (!IsMemFlowEnabled (RoundTripOptimizeTraining)) {
    return SUCCESS;
  }

  ZeroMem ((UINT8 *) piDelay, sizeof (piDelay));
  socket = Host->var.mem.currentSocket;
  MSVx4SubCh = GetSubChMaxStrobeValid(Host);

  if (IsDdr5Present (Host, socket)) {
#ifdef DDR5_SUPPORT
    if (IsEqualSubChRtlRequired () == FALSE) {
      //
      // This function only support DDR5 iMC which requires sub-channel A and B to use equal RTL.
      //
      RcDebugPrint (SDBG_MINMAX, "ERROR: sub-channel A and B do not use equal Round Trip Latency.\n");
      return SUCCESS;
    }
#endif // DDR5_SUPPORT
  }

  //
  // Return if this socket is disabled
  //

  if (IsSocketDimmPopulated (Host, socket) != TRUE) {
    return SUCCESS;
  }

  DoCompDdrioPerSetupOption (socket, TRAINING_COMP_EVERY_TRAINING);

  //
  // Get the max delay limits of CK, CTL, CMD, RcvEn, TxDqs and TxDq signals.
  //
  LimitRoundTripAsNeeded (socket);
  GetCmdGroupLimits (socket, ALL_CH, 0, DdrLevel, CmdAll, &minLimit, &maxCmdDelay);
  GetCmdGroupLimits (socket, ALL_CH, 0, DdrLevel, CtlAll, &minLimit, &maxCtlDelay);
  GetDataGroupLimits (Host, DdrLevel, RecEnDelay, &minLimit, &maxRecEnDelay, &usDelay);
  GetDataGroupLimits (Host, DdrLevel, TxDqsDelay, &minLimit, &maxTxDqsDelay, &usDelay);
  GetDataGroupLimits (Host, DdrLevel, TxDqDelay, &minLimit, &maxTxDqDelay, &usDelay);

  //
  // Save Orginal runningRmt setting before set to 1
  //
  orgrunningRmt = Host->var.mem.runningRmt;

  //
  // Set runningRmt = 1 so that we margin all ranks for host RxDqsDelay. Previously, we only trained rank 0.
  //
  Host->var.mem.runningRmt = 1;

  rtGuardBand = RT_GUARDBAND;
  minIoLatency = MEM_CHIP_POLICY_VALUE (Host, minIoLatency);

  for (ch = 0; ch < MaxChDdr; ch++) {

    if (IsChannelEnabled (socket, ch) == FALSE) {
      continue;
    }

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {

      if (IsDimmPresent (socket, ch, dimm) == FALSE) {
        continue;
      }

      rankList = GetRankNvList (Host, socket, ch, dimm);

      // Loop for each rank
      for (rank = 0; rank < GetNumberOfRanksOnDimm (socket, ch, dimm); rank++) {
        OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_ROUND_TRIP_OPTIMIZE, (UINT16) ((socket << 8) | (dimm << 4) | rank)));

        if (IsRankValid (socket, ch, dimm, rank, FALSE) == FALSE) {
          continue;
        }

        for (strobe = 0; strobe < MSVx4; strobe++) {
          timeMargin.strobe[ch][ (*rankList)[rank].rankIndex][strobe].n = 0;
          timeMargin.strobe[ch][ (*rankList)[rank].rankIndex][strobe].p = 0;
        } // strobe loop

        //
        // Get the logical rank #
        //
        logRank = GetLogicalRank (Host, socket, ch, dimm, rank);

        if (IsDdr5Present (Host, socket)) {
#ifdef DDR5_SUPPORT
          for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
            GetRoundTripDdr5 (socket, ch, SubChannel, logRank, GSM_READ_CSR, &SubChOrgRT[ch][SubChannel][(*rankList)[rank].rankIndex]);
            GetIoLatencyDdr5 (socket, ch, SubChannel, logRank, GSM_READ_CSR, &SubChOrgIO[ch][SubChannel][(*rankList)[rank].rankIndex]);
            SubChGoodRT[ch][SubChannel][(*rankList)[rank].rankIndex] = SubChOrgRT[ch][SubChannel][(*rankList)[rank].rankIndex];
            SubChGoodIO[ch][SubChannel][(*rankList)[rank].rankIndex] = SubChOrgIO[ch][SubChannel][(*rankList)[rank].rankIndex];
          }

          //
          // For DDR5 iMC which requires two sub-channel have the same RTL.
          //
          if (SubChOrgRT[ch][SUB_CH_A][(*rankList)[rank].rankIndex] == SubChOrgRT[ch][SUB_CH_B][(*rankList)[rank].rankIndex]) {
            orgRT[ch][(*rankList)[rank].rankIndex]  = SubChOrgRT[ch][SUB_CH_A][(*rankList)[rank].rankIndex];
            //
            // Use the shorter IO latency between the two sub-channels as limitation of later channel level RTL training.
            //
            orgIO[ch][(*rankList)[rank].rankIndex] = SubChOrgIO[ch][SUB_CH_A][(*rankList)[rank].rankIndex];
            if (SubChOrgIO[ch][SUB_CH_B][(*rankList)[rank].rankIndex] < SubChOrgIO[ch][SUB_CH_A][(*rankList)[rank].rankIndex]) {
              orgIO[ch][(*rankList)[rank].rankIndex] = SubChOrgIO[ch][SUB_CH_B][(*rankList)[rank].rankIndex];
            }

            goodRT[ch][(*rankList)[rank].rankIndex] = orgRT[ch][ (*rankList)[rank].rankIndex];
            goodIO[ch][(*rankList)[rank].rankIndex] = orgIO[ch][ (*rankList)[rank].rankIndex];
          } else {
            RcDebugPrint (SDBG_MINMAX, "ERROR: Require sub-channel A and B to set equal Round Trip Latency.\n");
            return FAILURE;
          }
#endif // DDR5_SUPPORT
        } else {
          GetRoundTrip (socket, ch, logRank, GSM_READ_CSR, &orgRT[ch][ (*rankList)[rank].rankIndex]);
          GetIoLatency (socket, ch, logRank, GSM_READ_CSR, &orgIO[ch][ (*rankList)[rank].rankIndex]);
          goodRT[ch][(*rankList)[rank].rankIndex] = orgRT[ch][ (*rankList)[rank].rankIndex];
          goodIO[ch][(*rankList)[rank].rankIndex] = orgIO[ch][ (*rankList)[rank].rankIndex];
        }
      } // rank loop
    } // dimm loop

    //
    // Avoid clock movement for BPS A0
    //
    if (FmcAvoidClockMovementInChannel (Host, socket, ch)) {
      continue;
    }

    //
    // Step 1: Delay all pins on the channel (CLK, CMD, CTL, TX DQ, TX DQS, RCVEN) by a fixed amount of 20 pi ticks or
    //         a common maximal pi ticks. This will directly delay the read return data by 20 pi ticks or a common  maximal
    //         pi ticks, to provide an additional form of guardband to ensure the last passing value is a truly robust
    //         setting.
    //

    minDelay = MAX_CMD_MARGIN;
    maxDelay = 0;
    piDelay[ch] = RT_INITIAL_PI_DELAY; // An additional guardband of 20 pi ticks delay.
    for (clk = 0; clk < MAX_CLK; clk++) {
      ckEnabled[clk] = 0;
      ctlIndex[clk] = 0;
    } // clk loop

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      rankList = GetRankNvList (Host, socket, ch, dimm);

      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {

        if (IsRankValid (socket, ch, dimm, rank, FALSE) == FALSE) {
          continue;
        }

        clk = (*rankList)[rank].ckIndex;
        ctlIndex[clk] = (*rankList)[rank].ctlIndex;
        ckEnabled[clk] = 1;
      } // rank loop
    } // dimm loop


    //
    // Check whether additional 20 PI ticks piDelay can be accommodated by all signals.
    // Otherwise, adjust piDelay to a common maximal delay so that none of the signals in the channel fail.
    //

    //
    // Limit channel level common piDelay with PI delay limits of of CTL signals.
    //
    for (clk = 0; clk < MAX_CLK; clk++) {
      if (ckEnabled[clk] == 0) {
        continue;
      }

      GetSetCtlGroupDelay (Host, socket, ch, ctlIndex[clk] + CtlGrp0, GSM_READ_ONLY, &curDelay, &minDelay, &maxDelay);
      if ((maxDelay + piDelay[ch]) > maxCtlDelay) {
        piDelay[ch] = maxCtlDelay - maxDelay;
      }
    } // clk loop

    //
    // Limit channel level common piDelay with PI delay limits of of CMD signals.
    //
    GetSetCmdGroupDelay (Host, socket, ch, CmdAll, GSM_READ_ONLY, &curDelay, &minDelay, &maxDelay);
    if ((maxDelay + piDelay[ch]) > maxCmdDelay) {
      piDelay[ch] = maxCmdDelay - maxDelay;
    }

    //
    // Limit channel level common piDelay with PI delay limits of data signals (RCVEN, TXDQ and TXDQS).
    //
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {

        if (IsRankValid (socket, ch, dimm, rank, FALSE) == FALSE) {
          continue;
        }

        for (strobe = 0; strobe < MSVx4; strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if (IsStrobeNotValid (Host, strobe)) {
            continue;
          }

          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, RecEnDelay, GSM_READ_ONLY, &curDelay);
          if ((curDelay + piDelay[ch]) > maxRecEnDelay) {
            piDelay[ch] = maxRecEnDelay - curDelay;
          }
          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, &curDelay);
          if ((curDelay + piDelay[ch]) > maxTxDqsDelay) {
            piDelay[ch] = maxTxDqsDelay - curDelay;
          }
          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, TxDqDelay, GSM_READ_ONLY, &curDelay);
          if ((curDelay + piDelay[ch]) > maxTxDqDelay) {
            piDelay[ch] = maxTxDqDelay - curDelay;
          }
        } // strobe loop
      } // rank loop
    } // dimm loop

    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Number of offset PI ticks %2d\n", piDelay[ch]);

    //
    // Offset all signals (CLK, CTL, CMD, RcvEn, TxDqs and TxDq) in the channel by channel level common piDelay, which is
    // above trained value that is not greater than 20 pi ticks.
    //

    for (clk = 0; clk < MAX_CLK; clk++) {
      if (ckEnabled[clk] == 0) {
        continue;
      }

      GetSetClkDelay (Host, socket, ch, clk, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &piDelay[ch]);

      GetSetCtlGroupDelay (Host, socket, ch, ctlIndex[clk] + CtlGrp0, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &piDelay[ch], &minDelay, &maxDelay);
    } // clk loop

    GetSetCmdGroupDelay (Host, socket, ch, CmdAll, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &piDelay[ch], &minDelay, &maxDelay);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {

        if (IsRankValid (socket, ch, dimm, rank, FALSE) == FALSE) {
          continue;
        }

        for (strobe = 0; strobe < MSVx4; strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if (IsStrobeNotValid (Host, strobe)) {
            continue;
          }

          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, RecEnDelay, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE,
            (INT16 *)&piDelay[ch]);
          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, TxDqsDelay, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE,
            (INT16 *)&piDelay[ch]);
          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, TxDqDelay, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE,
            (INT16 *)&piDelay[ch]);
        } // strobe loop
      } // rank loop
    } // dimm loop
  } // ch loop

  IO_Reset (Host, socket);
  if (IsDdr5Present (Host, socket) == FALSE) {
    JedecInitSequence (Host, socket, CH_BITMASK);
  }

  //
  // Step 2: Measure the per-strobe RX timing margin for the rank at nominal Vref.
  //         Store the total EW for future reference.
  //

  //
  // Clear the filter bits to enable error checking on every bit
  //
  ClearFilter (Host, socket, &filter);

  RcDebugPrint (SDBG_MAX, "\nGet Initial RTL, IOL and Eye widths:\n");

  //
  // Get initial Rx timing width
  //
  if ((IsDdr5Present (Host, socket) == TRUE) && (IsBrsPresent (Host, socket) == FALSE)) {
#ifdef DDR5_SUPPORT
    SetCurrentTestType (socket, RmtDqTest);
    GetAdvDataMargins (
      socket, DdrLevel, RxDqsDelay, ResultsRelative,
      &timeMargin, NULL, NULL,
      NULL, NULL, NULL
      );
    SetCurrentTestType (socket, 0);
#endif
  } else {
    GetMargins (Host, socket, DdrLevel, RxDqsDelay, MODE_VIC_AGG, SCOPE_STROBE, &filter, &timeMargin, TRAIN_RD_DQS_PL, 0, 0, 0,
      WDB_BURST_LENGTH, RmtDqTest, NULL);
  }

#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {

    if (IsDdr5Present (Host, socket)) {
#ifdef DDR5_SUPPORT
      RcDebugPrint (SDBG_DEFAULT, "    ");
#endif // DDR5_SUPPORT
    }

    RcDebugPrint (SDBG_DEFAULT, "              RT  IOL ");

    for (strobe = 0; strobe < (MSVx4 / SUB_CH); strobe++) {
      RcDebugPrint (SDBG_DEFAULT, "  %2d", strobe);
    }
    RcDebugPrint (SDBG_DEFAULT, "\n");
    RcDebugPrintLine (SDBG_MAX, 21 + (MSVx4 / SUB_CH) * 4, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  }
#endif // DEBUG_CODE_BLOCK
  worstMargin = 0xFF;

  //
  // Calculate eye widths
  //
  for (ch = 0; ch < MaxChDdr; ch++) {

    if (IsChannelEnabled (socket, ch) == FALSE) {
      continue;
    }

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {

      if (IsDimmPresent (socket, ch, dimm) == FALSE) {
        continue;
      }

      rankList = GetRankNvList (Host, socket, ch, dimm);

      // Loop for each rank
      for (rank = 0; rank < GetNumberOfRanksOnDimm (socket, ch, dimm); rank++) {

        if (IsRankValid (socket, ch, dimm, rank, FALSE) == FALSE) {
          continue;
        }

        for (strobe = 0; strobe < MSVx4; strobe++) {
          if (IsStrobeNotValid (Host, strobe)) {
            continue;
          }
          eyeWidth[ch][ (*rankList)[rank].rankIndex][strobe] = (UINT8) (timeMargin.strobe[ch][ (*rankList)[rank].rankIndex][strobe].p -
                                                                        timeMargin.strobe[ch][ (*rankList)[rank].rankIndex][strobe].n);

          //
          // Track the worst margin
          //
          if (worstMargin > eyeWidth[ch][ (*rankList)[rank].rankIndex][strobe]) {
            worstMargin = eyeWidth[ch][ (*rankList)[rank].rankIndex][strobe];
          }
        } // strobe loop

        //
        // Print out RTL, IOL and Eye Widths of current rank.
        //
        if (IsDdr5Present (Host, socket)) {
#ifdef DDR5_SUPPORT
          for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

              RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, SubChannel, rank, NO_STROBE, NO_BIT,
                "%2d  %2d  :", SubChOrgRT[ch][SubChannel][(*rankList)[rank].rankIndex],
                SubChOrgIO[ch][SubChannel][(*rankList)[rank].rankIndex]);

              for (strobe = 0; strobe < MSVx4SubCh; strobe++) {
                if (IsStrobeNotValid (Host, ((MSVx4SubCh * SubChannel) + strobe))) {
                  continue;
                }
                RcDebugPrint (SDBG_MAX, "%3d ", eyeWidth[ch][(*rankList)[rank].rankIndex][(MSVx4SubCh * SubChannel) + strobe]);
              }
             RcDebugPrint (SDBG_MAX, "\n");
          }
#endif // DDR5_SUPPORT
        } else {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
            "%2d  %2d  :", orgRT[ch][(*rankList)[rank].rankIndex], orgIO[ch][(*rankList)[rank].rankIndex]);

          for (strobe = 0; strobe < MSVx4; strobe++) {
            if (IsStrobeNotValid (Host, strobe)) {
              continue;
            }
            RcDebugPrint (SDBG_MAX, "%3d ", eyeWidth[ch][(*rankList)[rank].rankIndex][strobe]);
          }
          RcDebugPrint (SDBG_MAX, "\n");
        }


        //
        // Do not try to optimize if IO latency is below the minimum
        //
        if (orgIO[ch][ (*rankList)[rank].rankIndex] <= minIoLatency) {
          rankDone[ch][ (*rankList)[rank].rankIndex] = 1;
        } else {
          rankDone[ch][ (*rankList)[rank].rankIndex] = 0;
        }
      } // rank loop
    } // dimm loop
  } // ch loop

  //
  // If the starting margins are too low, skip this step.
  //
  if (worstMargin <= rtGuardBand) {
    keepGoing = 0;
  } else {
    keepGoing = 1;
  }

  //
  // Step 3: Reduce round trip on each rank until we find the best round trip setting.
  //         Until final RT Latency is found, do the following:
  //         1. Reduce the rank's RT Latency setting by 1 DCLK
  //            and reduce the rank's IOLatency setting by 1 DCLK.
  //         2. Measure the per-strobe RX timing margins for the rank. Compare to the previous
  //            EW measurements at the beginning of training. If the difference is within 4
  //            PI ticks, store this RT Latency setting, and then repeat this step.
  //         3. If the EW reduction was worse than 4 PI ticks, training is complete.
  //            Restore the RT Latency for last "passing" case.
  //

  while (keepGoing) {

    keepGoing = 0;

    //
    // Reduce the rank's RT Latency setting by 1 DCLK
    // and reduce the rank's IOLatency setting by 1 DCLK.
    //
    for (ch = 0; ch < MaxChDdr; ch++) {

      if (IsChannelEnabled (socket, ch) == FALSE) {
        continue;
      }

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        if (IsDimmPresent (socket, ch, dimm) == FALSE) {
          continue;
        }

        rankList = GetRankNvList (Host, socket, ch, dimm);

        // Loop for each rank
        for (rank = 0; rank < GetNumberOfRanksOnDimm (socket, ch, dimm); rank++) {

          if (IsRankValid (socket, ch, dimm, rank, FALSE) == FALSE) {
            continue;
          }
          //
          // Do not adjust this rank if it is done
          //
          if (rankDone[ch][ (*rankList)[rank].rankIndex]) {
            continue;
          }
          //
          // Get the logical rank #
          //
          logRank = GetLogicalRank (Host, socket, ch, dimm, rank);

          if (IsDdr5Present (Host, socket)) {
#ifdef DDR5_SUPPORT
            //
            // Reduce IOL and RTL of both sub-channels of DDR5 by the same step unit.
            //
            for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
              RoundTripLatency = SubChGoodRT[ch][SubChannel][(*rankList)[rank].rankIndex]  - GetRoundTripStepUnit ();
              IoLatency = SubChGoodIO[ch][SubChannel][(*rankList)[rank].rankIndex] - GetIoLatStepUnit ();

              SetRoundTripDdr5 (socket, ch, SubChannel, logRank, RoundTripLatency);
              SetIoLatencyDdr5 (socket, ch, SubChannel, logRank, IoLatency);
            }
#endif // DDR5_SUPPORT
          } else {
            RoundTripLatency = goodRT[ch][(*rankList)[rank].rankIndex] - GetRoundTripStepUnit ();
            IoLatency = goodIO[ch][(*rankList)[rank].rankIndex] - GetIoLatStepUnit ();
            SetRoundTrip (socket, ch, logRank, RoundTripLatency);
            SetIOLatency (socket, ch, logRank, IoLatency);
          }

          for (strobe = 0; strobe < MSVx4; strobe++) {
            timeMargin.strobe[ch][ (*rankList)[rank].rankIndex][strobe].n = 0;
            timeMargin.strobe[ch][ (*rankList)[rank].rankIndex][strobe].p = 0;
          } // strobe loop
        } // rank loop
      } // dimm loop
    } // ch loop

    //
    // Clear the filter bits to enable error checking on every bit
    //
    ClearFilter (Host, socket, &filter);

    IO_Reset (Host, socket);

    //
    // Measure the per-strobe RX timing margins for the rank. Compare to the previous EW measurements at the beginning of training.
    // 1. If the EW reduction was worse than 4 PI ticks, training is complete for the rank,
    //    restore the RT Latency for last "passing" case.
    // 2. If the difference is within 4 PI ticks, store this RT Latency setting.
    //    If IOL has reached to minimal IOL limitation, training is complete for the rank;
    //    Otherwise, continue reducing IOL and repeat this step.
    //
    if ((IsDdr5Present (Host, socket) == TRUE) && (IsBrsPresent (Host, socket) == FALSE)) {
#ifdef DDR5_SUPPORT
    SetCurrentTestType (socket, RmtDqTest);
    GetAdvDataMargins (
      socket, DdrLevel, RxDqsDelay, ResultsRelative,
      &timeMargin, NULL, NULL,
      NULL, NULL, NULL
      );
    SetCurrentTestType (socket, 0);
#endif
    } else {
      GetMargins (Host, socket, DdrLevel, RxDqsDelay, MODE_VIC_AGG, SCOPE_STROBE, &filter, &timeMargin, TRAIN_RD_DQS_PL, 0, 0, 0,
        WDB_BURST_LENGTH, RmtDqTest, NULL);
    }

    RcDebugPrint (SDBG_MAX,
                   "\nCurrent Test Results:\n");
#ifdef DEBUG_CODE_BLOCK
    if (GetDebugLevel () & SDBG_MAX) {

      if (IsDdr5Present (Host, socket)) {
#ifdef DDR5_SUPPORT
        RcDebugPrint (SDBG_DEFAULT, "    ");
#endif
      }
      RcDebugPrint (SDBG_DEFAULT, "              RT  IOL ");

      for (strobe = 0; strobe < (MSVx4 / SUB_CH); strobe++) {
        RcDebugPrint (SDBG_DEFAULT, "  %2d", strobe);
      }
      RcDebugPrint (SDBG_DEFAULT, "\n");
      RcDebugPrintLine (SDBG_MAX, 21 + (MSVx4 / SUB_CH) * 4, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
    }
#endif // DEBUG_CODE_BLOCK


    //
    // Calculate eye widths
    //
    for (ch = 0; ch < MaxChDdr; ch++) {

      if (IsChannelEnabled (socket, ch) == FALSE) {
        continue;
      }

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        if (IsDimmPresent (socket, ch, dimm) == FALSE) {
          continue;
        }

        rankList = GetRankNvList (Host, socket, ch, dimm);

        // Loop for each rank
        for (rank = 0; rank < GetNumberOfRanksOnDimm (socket, ch, dimm); rank++) {

          if (IsRankValid (socket, ch, dimm, rank, FALSE) == FALSE) {
            continue;
          }

          if (rankDone[ch][ (*rankList)[rank].rankIndex]) {
            continue;
          }

          for (strobe = 0; strobe < MSVx4; strobe++) {
            if (IsStrobeNotValid (Host, strobe)) {
              continue;
            }
            if (timeMargin.strobe[ch][ (*rankList)[rank].rankIndex][strobe].n > timeMargin.strobe[ch][ (*rankList)[rank].rankIndex][strobe].p) {
              timeMargin.strobe[ch][ (*rankList)[rank].rankIndex][strobe].n = 0;
              timeMargin.strobe[ch][ (*rankList)[rank].rankIndex][strobe].p = 0;
            }

          } // strobe loop

          //
          // Print out RTL, IOL and Eye Widths of current rank.
          //
          if (IsDdr5Present (Host, socket)) {
#ifdef DDR5_SUPPORT
             for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
               RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, SubChannel, rank, NO_STROBE, NO_BIT,
                 "%2d  %2d  :",
                 SubChGoodRT[ch][SubChannel][(*rankList)[rank].rankIndex] - GetRoundTripStepUnit (),
                 SubChGoodIO[ch][SubChannel][(*rankList)[rank].rankIndex] - GetIoLatStepUnit ());

               for (strobe = 0; strobe < MSVx4SubCh; strobe++) {
                 if (IsStrobeNotValid (Host, ((MSVx4SubCh * SubChannel) + strobe))) {
                   continue;
                 }
                 RcDebugPrint (SDBG_MAX, "%3d ",
                              timeMargin.strobe[ch][(*rankList)[rank].rankIndex][(MSVx4SubCh * SubChannel) + strobe].p -
                              timeMargin.strobe[ch][(*rankList)[rank].rankIndex][(MSVx4SubCh * SubChannel) + strobe].n);
               }
               RcDebugPrint (SDBG_MAX, "\n");
             }
#endif // DDR5_SUPPORT
           } else {
              RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                "%2d  %2d  :",
                (goodRT[ch][(*rankList)[rank].rankIndex] - GetRoundTripStepUnit ()),
                (goodIO[ch][(*rankList)[rank].rankIndex] - GetIoLatStepUnit ())
                );

              for (strobe = 0; strobe < MSVx4; strobe++) {
                if (IsStrobeNotValid (Host, strobe)) {
                  continue;
                }
                RcDebugPrint (SDBG_MAX, "%3d ", eyeWidth[ch][(*rankList)[rank].rankIndex][strobe]);
              }
              RcDebugPrint (SDBG_MAX, "\n");
            }

        } // rank loop
      } // dimm loop
    } // ch loop

    RcDebugPrint (SDBG_MAX,
                   "\n");

    //
    // Check eye widths
    //
    for (ch = 0; ch < MaxChDdr; ch++) {

      if (IsChannelEnabled (socket, ch) == FALSE) {
        continue;
      }

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        if (IsDimmPresent (socket, ch, dimm) == FALSE) {
          continue;
        }

        rankList = GetRankNvList (Host, socket, ch, dimm);

        // Loop for each rank
        for (rank = 0; rank < GetNumberOfRanksOnDimm (socket, ch, dimm); rank++) {

          if (IsRankValid (socket, ch, dimm, rank, FALSE) == FALSE) {
            continue;
          }

          if (rankDone[ch][ (*rankList)[rank].rankIndex]) {
            continue;
          }

          failed = 0;

          //
          // If the EW reduction was worse than 4 PI ticks, training is complete for the rank,
          // restore the RT Latency for last "passing" case.
          //
          for (strobe = 0; strobe < MSVx4; strobe++) {
            if (IsStrobeNotValid (Host, strobe)) {
              continue;
            }
            if ((eyeWidth[ch][ (*rankList)[rank].rankIndex][strobe] - rtGuardBand) >
                (timeMargin.strobe[ch][ (*rankList)[rank].rankIndex][strobe].p -
                 timeMargin.strobe[ch][ (*rankList)[rank].rankIndex][strobe].n)) {

              RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                                      "Fail: eyeWidth = %d, current width = %d\n", eyeWidth[ch][ (*rankList)[rank].rankIndex][strobe],
                                      timeMargin.strobe[ch][ (*rankList)[rank].rankIndex][strobe].p -
                                      timeMargin.strobe[ch][(*rankList)[rank].rankIndex][strobe].n);
              failed = 1;
              break;
            }
          } // strobe loop

          if (failed) {
            rankDone[ch][ (*rankList)[rank].rankIndex] = 1;
          } else {
            //
            // These values passed, so save them.
            //
            goodRT[ch][ (*rankList)[rank].rankIndex] = goodRT[ch][ (*rankList)[rank].rankIndex] - GetRoundTripStepUnit ();
            goodIO[ch][ (*rankList)[rank].rankIndex] = goodIO[ch][ (*rankList)[rank].rankIndex] - GetIoLatStepUnit ();

            if (IsDdr5Present(Host, socket)) {
#ifdef DDR5_SUPPORT
              for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
                SubChGoodRT[ch][SubChannel][ (*rankList)[rank].rankIndex] = SubChGoodRT[ch][SubChannel][ (*rankList)[rank].rankIndex] - GetRoundTripStepUnit ();
                SubChGoodIO[ch][SubChannel][ (*rankList)[rank].rankIndex] = SubChGoodIO[ch][SubChannel][ (*rankList)[rank].rankIndex] - GetIoLatStepUnit ();
              }
#endif // DDR5_SUPPORT
            }
            //
            // If IOL has reached to minimal IOL limitation, training is complete for current rank.
            //
            if (goodIO[ch][ (*rankList)[rank].rankIndex] <= minIoLatency) {
              rankDone[ch][ (*rankList)[rank].rankIndex] = 1;
            } else {
              keepGoing = 1;
            }

            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                                    "New RT value passed saving. RT = %d, IO = %d, KeepGoing = %d\n", goodRT[ch][ (*rankList)[rank].rankIndex],
                                    goodIO[ch][(*rankList)[rank].rankIndex], (!(rankDone[ch][ (*rankList)[rank].rankIndex])));
          }
        } // rank loop
      } // dimm loop
    } // ch loop
  } // while loop

  //
  // Step 4: For some DDR5 platform only:
  //         Find the maximal per-rank optimal RTL among all ranks in a channel and record it as channel level optimal RTL.
  //         All ranks in the channel align their RTL to this common channel level RTL.
  //
  if (IsDdr5Present(Host, socket)) {
#ifdef DDR5_SUPPORT
    RcDebugPrint (SDBG_MEM_TRAIN, "\nRTL Before Channel Level Adjusment:\n");
    RcDebugPrint (SDBG_MEM_TRAIN, "                   org RT : new RT  org IOL : new IOL\n");
    for (ch = 0; ch < MaxChDdr; ch++) {
      if (IsChannelEnabled (socket, ch) == FALSE) {
        continue;
      }

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        if (IsDimmPresent (socket, ch, dimm) == FALSE) {
          continue;
        }

        rankList = GetRankNvList (Host, socket, ch, dimm);

        for (rank = 0; rank < GetNumberOfRanksOnDimm (socket, ch, dimm); rank++) {

          if (IsRankValid (socket, ch, dimm, rank, FALSE) == FALSE) {
            continue;
          }

          logRank = GetLogicalRank (Host, socket, ch, dimm, rank);

          for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

            RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, SubChannel, rank, NO_STROBE, NO_BIT,
               "   %2d   :   %2d       %2d   :   %2d\n",
               SubChOrgRT[ch][SubChannel][(*rankList)[rank].rankIndex],
               SubChGoodRT[ch][SubChannel][(*rankList)[rank].rankIndex],
               SubChOrgIO[ch][SubChannel][(*rankList)[rank].rankIndex],
               SubChGoodIO[ch][SubChannel][(*rankList)[rank].rankIndex]);
          }
        } // rank loop
      } // dimm loop
    } // ch

    //
    // Do channel level RTL adjusment to set all ranks within a channel with the same RTL.
    //
    AdjustChannelRoundTrip (socket, &SubChGoodRT, &SubChGoodIO);
#endif // DDR5_SUPPORT
  }

  //
  // Step 5: The optimal RTL and IOL have been found. Adjust RTL and IOL to optimal value.
  //

  RcDebugPrint (SDBG_MEM_TRAIN, "\n");
  if (IsDdr5Present (Host, socket)) {
#ifdef DDR5_SUPPORT
    RcDebugPrint (SDBG_MEM_TRAIN, "    ");
#endif // DDR5_SUPPORT
  }
  RcDebugPrint (SDBG_MEM_TRAIN, "               org RT : new RT  org IOL : new IOL\n");

  for (ch = 0; ch < MaxChDdr; ch++) {

    if (IsChannelEnabled (socket, ch) == FALSE) {
      continue;
    }

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {

      if (IsDimmPresent (socket, ch, dimm) == FALSE) {
        continue;
      }

      rankList = GetRankNvList (Host, socket, ch, dimm);

      // Loop for each rank
      for (rank = 0; rank < GetNumberOfRanksOnDimm (socket, ch, dimm); rank++) {

        if (IsRankValid (socket, ch, dimm, rank, FALSE) == FALSE) {
          continue;
        }
        //
        // Get the logical rank #
        //
        logRank = GetLogicalRank (Host, socket, ch, dimm, rank);

        if (IsDdr5Present (Host, socket)) {
#ifdef DDR5_SUPPORT
          for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
            SetRoundTripDdr5 (socket, ch, SubChannel, logRank, SubChGoodRT[ch][SubChannel][ (*rankList)[rank].rankIndex]);
            SetIoLatencyDdr5 (socket, ch, SubChannel, logRank, SubChGoodIO[ch][SubChannel][ (*rankList)[rank].rankIndex]);
          }
#endif // DDR5_SUPPORT
        } else {
          SetRoundTrip (socket, ch, logRank, goodRT[ch][ (*rankList)[rank].rankIndex]);
          SetIOLatency (socket, ch, logRank, goodIO[ch][ (*rankList)[rank].rankIndex]);
        }

        if (IsDdr5Present (Host, socket)) {
#ifdef DDR5_SUPPORT
          for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

            RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, SubChannel, rank, NO_STROBE, NO_BIT,
               "   %2d   :   %2d       %2d   :   %2d\n",
               SubChOrgRT[ch][SubChannel][(*rankList)[rank].rankIndex],
               SubChGoodRT[ch][SubChannel][(*rankList)[rank].rankIndex],
               SubChOrgIO[ch][SubChannel][(*rankList)[rank].rankIndex],
               SubChGoodIO[ch][SubChannel][(*rankList)[rank].rankIndex]);
          }
#endif // DDR5_SUPPORT
        } else {
          RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
            "   %2d   :   %2d       %2d   :   %2d\n", orgRT[ch][(*rankList)[rank].rankIndex], goodRT[ch][(*rankList)[rank].rankIndex], orgIO[ch][(*rankList)[rank].rankIndex], goodIO[ch][(*rankList)[rank].rankIndex]);
        }
      } // rank loop
    } // dimm loop

    //
    // Avoid clock movement for BPS A0
    //
    if (FmcAvoidClockMovementInChannel (Host, socket, ch)) {
      continue;
    }

    for (clk = 0; clk < MAX_CLK; clk++) {
      ckEnabled[clk] = 0;
      ctlIndex[clk] = 0;
    } // clk loop

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      rankList = GetRankNvList (Host, socket, ch, dimm);

      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {

        if (IsRankValid (socket, ch, dimm, rank, FALSE) == FALSE) {
          continue;
        }

        clk = (*rankList)[rank].ckIndex;
        ctlIndex[clk] = (*rankList)[rank].ctlIndex;
        ckEnabled[clk] = 1;
      } // rank loop
    } // dimm loop

    //
    // Step 6: Restore CK, CTL, CMD, TX DQ, TX DQS, and RCVEN PI settings of the channel back to original values.
    //

    piDelay[ch] = -piDelay[ch];
    for (clk = 0; clk < MAX_CLK; clk++) {
      if (ckEnabled[clk] == 0) {
        continue;
      }

      GetSetClkDelay (Host, socket, ch, clk, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &piDelay[ch]);

      GetSetCtlGroupDelay (Host, socket, ch, ctlIndex[clk] + CtlGrp0, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &piDelay[ch], &minDelay, &maxDelay);
    } // clk loop

    GetSetCmdGroupDelay (Host, socket, ch, CmdAll, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &piDelay[ch], &minDelay, &maxDelay);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {

        if (IsRankValid (socket, ch, dimm, rank, FALSE) == FALSE) {
          continue;
        }

        for (strobe = 0; strobe < MSVx4; strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if (IsStrobeNotValid (Host, strobe)) {
            continue;
          }

          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, RecEnDelay, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE,
            (INT16 *)&piDelay[ch]);
          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, TxDqsDelay, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE,
            (INT16 *)&piDelay[ch]);
          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, TxDqDelay, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE,
            (INT16 *)&piDelay[ch]);
        } // strobe loop
      } // rank loop
    } // dimm loop
  } // ch loop

  //
  // Normalize RT/IO latency
  //
  NormalizeRoundTripAndIoLatency (socket);

  IO_Reset (Host, socket);
  if (IsDdr5Present (Host, socket) == FALSE) {
    JedecInitSequence (Host, socket, CH_BITMASK);
  }
  //
  // Restore runningRmt value
  //
  Host->var.mem.runningRmt = orgrunningRmt;
  return SUCCESS;
} // RoundTripOptimize
