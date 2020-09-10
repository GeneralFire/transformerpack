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
#include <Library/MemMcIpLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/OdtTableLib.h>
#include <Memory/JedecDefinitions.h>
#include "Include/BaseMemoryCoreInternal.h"
#include <Memory/MemoryCheckpointCodes.h>

//
// Local function prototypes
//

STATIC VOID
WriteLevelingEnd (
  PSYSHOST Host,
  UINT8    socket
  );

STATIC VOID
TxPiSample (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    dimm,
  UINT8    rank
  );


/**

  Perform Write Leveling training

  @param Host  - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
WriteLeveling (
  PSYSHOST Host
  )
{
  UINT8               socket;
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               strobe;
  UINT8               maxStrobe;
  UINT8               rankPresent;
#ifdef DEBUG_CODE_BLOCK
  UINT16              delay;
#endif // DEBUG_CODE_BLOCK
  UINT16              risingEdge[MAX_CH][MAX_STROBE];
  UINT16              centerPoint[MAX_CH][MAX_STROBE];
  UINT16              fallingEdge[MAX_CH][MAX_STROBE];
  UINT16              pulseWidth[MAX_CH][MAX_STROBE];
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  struct Socket       (*varSocket)[MAX_SOCKET];
  UINT8               faultyPartsStatus;
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8 MaxChDdr;

  if (!IsMemFlowEnabled (WriteLevelingTraining)) {
    return SUCCESS;
  }

  MaxChDdr = GetMaxChDdr ();
  socket = Host->var.mem.currentSocket;

  //
  // Return if this socket is disabled
  //

  if (IsSocketDimmPopulated (Host, socket) != TRUE) {
    return SUCCESS;
  }


  faultyPartsStatus = FPT_NO_ERROR;

  channelNvList = GetChannelNvList (Host, socket);
  varSocket = &Host->var.mem.socket;

  //
  // Train each DIMM
  //
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    //
    // Train each rank
    //
    for (rank = 0; rank < (*varSocket)[socket].maxRankDimm; rank++) {
      OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_WRITE_LEVELING_BASIC, (UINT16) ((socket << 8) | (dimm << 4) |rank)));
      //
      // Initialize to no ranks present
      //
      rankPresent = 0;

      for (ch = 0; ch < MaxChDdr; ch++) {
        // Go to next channel if this one is disabled
        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }

        rankList = GetRankNvList (Host, socket, ch, dimm);

        // Continue if this rank is disabled
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        dimmNvList = GetDimmNvList (Host, socket, ch);

        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }

        //
        // Inidicate this rank is present on at least one channel
        //
        rankPresent |= (*rankList)[rank].enabled;

        Host->var.mem.piSettingStopFlag[ch] = GetStrobeMask (Host);
        if (!(*dimmNvList)[dimm].x4Present) {
          Host->var.mem.piSettingStopFlag[ch] &= 0x1FF;
        } // if x4
      } // ch loop

      //
      // Go to the next rank if this rank is not present on any channels
      //
      if (!rankPresent) {
        continue;
      }

      //
      // Change Rtt_nom to Rtt_wr and enable DRAM Write Leveling mode
      //
#ifdef LRDIMM_SUPPORT
      Host->var.mem.InOvrRttPrkMode = 1;
#endif
      OverrideRttNom (Host, socket, dimm, rank);

      //
      // Reset the IO
      //
      //IO_Reset (Host, socket);

      // Sample all DQS Pi's
      TxPiSample (Host, socket, dimm, rank);

      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }

        dimmNvList = GetDimmNvList (Host, socket, ch);

        // Continue to the next rank if this one is disabled
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT) || (rank >= (*dimmNvList)[dimm].numRanks)) {
          continue;
        }

        maxStrobe = GetMaxStrobe (Host, socket, ch, dimm, WrLvlDelay, DdrLevel);

#ifdef DEBUG_CODE_BLOCK
        if (GetDebugLevel () & SDBG_MAX) {
          AcquirePrintControl ();
          RcDebugPrint (SDBG_DEFAULT, "\nSummary: Write Leveling Pi");
          RcDebugPrint (SDBG_DEFAULT, "\nS%d, Ch%d, DIMM%d, Rank%d\n", socket, ch, dimm, rank);
          PrintSampleArray (Host, socket, ch, dimm, 0);
          ReleasePrintControl ();
        }
#endif // DEBUG_CODE_BLOCK

        for (strobe = 0; strobe < maxStrobe; strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if (IsStrobeNotValid (Host, strobe)) {
            continue;
          }

          EvaluatePiSettingResults (Host, socket, ch, dimm, rank, strobe, risingEdge, centerPoint, fallingEdge, pulseWidth, FPT_PI_WRITE_TYPE);

          risingEdge[ch][strobe] = risingEdge[ch][strobe] - 64;

          //
          // For faulty strobe, x4: using setting of paired strobe, x8: using setting from neighbor strobe
          //
          if ((*rankList)[rank].faultyParts[strobe] != 0) {
            if ((*dimmNvList)[dimm].x4Present == 1) {
              if (strobe >= MSVx8) {
                risingEdge[ch][strobe] = risingEdge[ch][strobe - MSVx8];
              } else {
                risingEdge[ch][strobe] = risingEdge[ch][strobe + MSVx8];
              }
            } else {
              if (strobe == 8) {
                risingEdge[ch][strobe] = risingEdge[ch][strobe - 1];
              } else {
                risingEdge[ch][strobe] = risingEdge[ch][strobe + 1];
              }
            }
            faultyPartsStatus = EvaluateFaultyParts (Host, socket, ch);

            switch (faultyPartsStatus) {
            case FPT_NO_ERROR:
              break;
            case FPT_CORRECTABLE_ERROR:
              // if correctable log the warning -> OutputWarning
              // careful for cases when warning gets promoted to error !!!  - log the strobe
              EwlOutputType2 (WARN_FPT_CORRECTABLE_ERROR, WARN_FPT_MINOR_WR_LEVEL, socket, ch, dimm, rank, EwlSeverityWarning, strobe, NO_BIT, WrLvlDelay, DdrLevel, (UINT8) pulseWidth[ch][strobe]);
              break;
            default:
              Host->var.mem.piSettingStopFlag[ch] |= (1 << strobe);
              DisableChannelSw (Host, socket, ch);
              //
              // Add this error to the warning log for both correctable and uncorrectable errors.
              //
              EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_WR_LEVEL, socket, ch, dimm, rank, EwlSeverityWarning, strobe, NO_BIT, WrLvlDelay, DdrLevel, (UINT8) pulseWidth[ch][strobe]);
              break;
            }
          }

          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, TxDqsDelay, GSM_UPDATE_CACHE, (INT16 *)&risingEdge[ch][strobe]);
          if (!(*dimmNvList)[dimm].x4Present) {
            GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe + MSVx8, 0, DdrLevel, TxDqsDelay, GSM_UPDATE_CACHE, (INT16 *)&risingEdge[ch][strobe]);
          }
        } // strobe loop

#ifdef DEBUG_CODE_BLOCK
        // Display results
        DisplayEdges (Host, socket, ch, pulseWidth[ch], fallingEdge[ch], risingEdge[ch], centerPoint[ch], maxStrobe);
#endif // DEBUG_CODE_BLOCK

#ifdef DEBUG_CODE_BLOCK
        if (GetDebugLevel () & SDBG_MAX) {
          AcquirePrintControl ();
          RcDebugPrint (SDBG_DEFAULT, "START_DATA_WR_LVL_BASIC\n");
        }
        for (strobe = 0; strobe < maxStrobe; strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if (IsStrobeNotValid (Host, strobe)) {
            continue;
          }

          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, (INT16 *)&delay);
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                                  "WrLevel Delay = %d\n", delay);
        } // strobe loop
        if (GetDebugLevel () & SDBG_MAX) {
          RcDebugPrint (SDBG_DEFAULT, "STOP_DATA_WR_LVL_BASIC\n");
          ReleasePrintControl ();
        }
#endif // DEBUG_CODE_BLOCK
      } // ch loop
      DetectLowMargin (Host, NO_CH, dimm, rank, pulseWidth, TxDqsDelay, NON_PER_BIT);

      //
      // Restore Rtt_nom and disable DRAM Write Leveling mode
      //
#ifdef LRDIMM_SUPPORT
      Host->var.mem.InOvrRttPrkMode = 0;
#endif
      RestoreRttNom (Host, socket, dimm, rank);

    } // rank loop
  } // dimm loop

  WriteLevelingEnd (Host, socket);
  FifoTrainReset (Host, socket);

  return SUCCESS;
} // WriteLeveling

/**

  Clean up after Write Leveling training

  @param Host  - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/
STATIC
VOID
WriteLevelingEnd (
  PSYSHOST Host,
  UINT8    socket
  )
{
  UINT8               dimm;
  UINT8               rank;

  //
  // Enable all DIMM output buffers
  //

  //
  // Loop for each DIMM
  //
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    //
    // Loop for each rank
    //
    for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
      //
      // Change the Write level mode to 0 in the end of the algorithm
      //
      if (FeaturePcdGet (PcdCteBuild) == TRUE) {
        SetTrainingMode (Host, socket, dimm, rank, WR_LEVELING_CLEAR_MODE);
      } else {
        SetTrainingMode (Host, socket, dimm, rank, CLEAR_MODE);
      }

    } // rank loop
  } // dimm loop

  //
  // reset the IO
  //
  IO_Reset (Host, socket);
} // WriteLevelingEnd

/**

  Samples all DQS Pi's

  @param Host  - Pointer to sysHost
  @param socket  - Socket number
  @param dimm  - DIMM number
  @param rank   - Rank number
  @param nibble - 0 for the low nibble only, 1 for the high nibble, 2 for both nibbles

  @retval N/A

**/
STATIC
VOID
TxPiSample (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    dimm,
  UINT8    rank
  )
{
  UINT8                   ch;
  UINT32                  chBitmask;
  UINT8                   strobe;
  UINT8                   dWord;
  INT16                   piDelay;
  UINT16                  piDelayEnd;
  struct dimmNvram        (*dimmNvList)[MAX_DIMM];
  struct TrainingResults  (*trainRes)[MAX_CH][MAX_STROBE];
  CHIP_DDRIO_STRUCT       dataControl;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  trainRes = &Host->var.mem.trainRes;

  TxPiSampleChipInit (&dataControl);

  chBitmask = GetChBitmask (Host, socket, dimm, rank);
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ( !((1 << ch) & chBitmask)) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, socket, ch);
    if (rank >= (*dimmNvList)[dimm].numRanks) {
      continue;
    }
    GetChipLogicalRank (Host, socket, ch, dimm, rank, &dataControl);
    //
    // Clear training results
    //
    for (strobe = 0; strobe < MSVx4; strobe++) {
      for (dWord = 0; dWord < 4; dWord++) {
        (*trainRes)[ch][strobe].results[dWord] = 0;
      } // dWord loop
    } // strobe loop

    SetChipTxPiSampleTrainingMode (Host, socket, ch, &dataControl);
  } // ch loop

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                          "Write Leveling Pi Scanning...\n");

  GetChipTiPiSamplepiDelayStartandEnd (Host, socket, &piDelay, &piDelayEnd);

  for (; piDelay < piDelayEnd; piDelay += WR_LVL_STEP_SIZE) {

    //
    // Change the TX DQS PI Setting
    //
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ( !((1 << ch) & chBitmask)) {
        continue;
      }

      dimmNvList = GetDimmNvList (Host, socket, ch);
      if (rank >= (*dimmNvList)[dimm].numRanks) {
        continue;
      }

      //
      // Set training mode and rank to all channels
      //
      SetChipTxPiSampleTrainingTXDQSStrobesI (Host, socket, ch, dimm, rank, piDelay, &dataControl);

      FixedDelayMicroSecond (1);

      IO_Reset(Host, socket); // Need reset before enabling training mode.

      FixedDelayMicroSecond (1);

      SetChipTxPiSampleTrainingTXDQSStrobesII (Host, socket, ch, dimm, &dataControl);


      FixedDelayMicroSecond (1);


    } // ch loop

    //
    // Clear Errors and wait to collect results
    //
    FixedDelayMicroSecond (1);

    IO_Reset(Host, socket); // Need reset after enabling training mode.

    //
    // Clear Errors and wait to collect results
    //
    FixedDelayMicroSecond (1);
    GetResultsWL (Host, socket, dimm, rank, piDelay);

  } // piDelay inner loop

  return;
} // TxPiSample

/**

  Changes Rtt Nom to Rtt Wr

  @param Host  - Pointer to sysHost
  @param socket  - Socket number
  @param dimm  - DIMM number
  @param rank  - Rank number

**/
VOID
OverrideRttNom (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    dimm,
  UINT8    rank
  )
{
#ifndef DDR5_SUPPORT
  struct ddrRank  (*rankNvList)[MAX_RANK_DIMM];
  UINT8               ch;
  UINT16              MR1;
  UINT8               imcRank;
  UINT8               d;
  UINT8               r;
  UINT8               odtPin;
  UINT16              ReadAct;
  UINT16              WriteAct;
  UINT32              wrOdtTableData;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct rankDevice   (*rankStruct)[MAX_RANK_DIMM];
  BOOLEAN             OdtActFound;
#ifdef  LRDIMM_SUPPORT
  UINT8               cwData;
#endif  //  LRDIMM_SUPPORT
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);

  //
  // Enter WL mode and override Rtt_nom
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, socket, ch);
    rankStruct = GetRankStruct (Host, socket, ch, dimm);

    //
    // Skip if no rank
    //
    if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
      continue;
    }

#ifdef  LRDIMM_SUPPORT
    if ((ShouldChipEnableDDR4LRDIMMHostSideTraining (Host, socket, ch, dimm, rank)) &&
        ((*rankStruct)[rank].CurrentLrdimmTrainingMode != LRDIMM_DWL_TRAINING_MODE) &&
        (Host->var.mem.InPbaWaMode == 0)) {
      //
      // Set the current rank within the BC08 register.
      //
      WriteLrbuf (Host, socket, ch, dimm, 0, rank, LRDIMM_F0, LRDIMM_BC08);
      //
      // all other dimms in the channel must have their Host interface DQ/DQS drivers disabled
      //
      for (d = 0; d < MAX_DIMM; d++) {
        if ((*DimmNvList)[d].dimmPresent == 0) {
          continue;
        }

        if (!(IsLrdimmPresent (socket, ch, d))) {
          continue;
        }
        cwData = (*channelNvList)[ch].dimmList[d].lrBuf_BC03;
        if (d != dimm) {
          cwData |= LRDIMM_HOSTDQDQSDRIVERS_DISABLE;
        }
        WriteLrbuf (Host, socket, ch, d, 0, cwData, LRDIMM_F0, LRDIMM_BC03);
      } // d loop
      //
      // Enable LRDIMM Host side training mode
      //
      WriteLrbuf (Host, socket, ch, dimm, 0, LRDIMM_HWL_TRAINING_MODE, LRDIMM_F0, LRDIMM_BC0C);
      (*rankStruct)[rank].CurrentLrdimmTrainingMode = LRDIMM_HWL_TRAINING_MODE;

      if ((FeaturePcdGet (PcdCosimBuild) == FALSE) &&
          (FeaturePcdGet (PcdCteBuild) == TRUE)) {
        RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                        "CTE workaround: About to send MR1 with WRITE LEVEL TRAIN ENABLE\n");

        MR1 = (*rankStruct)[rank].MR1;
        //
        // Enable Write Leveling mode
        // Includes wait for tMOD before enabling ODT
        //
        MR1 = MR1 | MR1_WLE;
        MR1 |= 1;
        MR1 &= 0xfff9;
        WriteMRS (Host, socket, ch, dimm, rank, MR1, BANK1);
      }

      //
      // Copy RTT_WR to RTT_PARK
      //
      if ((*channelNvList)[ch].dimmList[dimm].lrBuf_BC01 != 0) {
        if (FeaturePcdGet (PcdCosimBuild) == TRUE) {
          RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                  "COSIM workaround: WRITE LEVEL training: Rtt_park to 0\n");
          cwData = 0;
        } else {
          cwData = (*channelNvList)[ch].dimmList[dimm].lrBuf_BC01;
        }
        WriteLrbuf (Host, socket, ch, dimm, 0, cwData, LRDIMM_F0, LRDIMM_BC02);
      }
    } else
#endif  //LRDIMM_SUPPORT
    {
      //
      // for DDR4, override RTT_PARK with RTT_WR value (unless RTT_WR is disabled)
      //
      SetChipRTTPark (Host, socket, ch, dimm, rank);
      // Get the current settings for MR1
      MR1 = (*rankStruct)[rank].MR1;

#ifdef  LRDIMM_SUPPORT
      if (Host->var.mem.InPbaWaMode == 0)
#endif
      {
        //
        // Enable Write Leveling mode
        // Includes wait for tMOD before enabling ODT
        //
        MR1 = MR1 | MR1_WLE;
        if (Host->nvram.mem.dramType == SPD_TYPE_DDR4) {
          MR1 |= 1;
        }
        MR1 &= 0xfff9;
        WriteMRS (Host, socket, ch, dimm, rank, MR1, BANK1);
      }
    }

    // Lookup ODT activations for this config
    OdtActFound = GetReadWriteActivationValuesOdt (socket, ch, dimm, rank, &ReadAct, &WriteAct);

    wrOdtTableData = 0;

    // Set encodings for ODT signals in IMC registers
    for (d = 0; d < MAX_DIMM; d++) {
      if ((*DimmNvList)[d].dimmPresent == 0) {
        continue;
      }

      rankNvList = GetRankNvList (Host, socket, ch, d);
      for (r = 0; r < Host->var.mem.socket[socket].maxRankDimm; r++) {

        // Skip if no rank
        if ((*rankNvList)[r].enabled == 0) {
          continue;
        }

        // Get ODT signal associated with this rank
        odtPin = (*rankNvList)[r].ODTIndex;
        imcRank = d * MAX_RANK_DIMM + r;

        // Set ODT signals for target write
        if ((OdtActFound) && (WriteAct & (1 << imcRank))) {
          wrOdtTableData |= (1 << odtPin);
        }

      } // matrix rank
    } // matrix dimm

    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "ODT Override: 0x%x\n", wrOdtTableData);

    AssertChipODTOveride (Host, socket, ch, wrOdtTableData);
  } // ch loop

  // Wait for tWLDQSEN to be met
  FixedDelayMicroSecond (1);
#endif // !DDR5_SUPPORT
} // OverrideRttNom

/*++

  Changes Rtt Nom to Rtt Wr

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param dimm    - DIMM number
  @param rank    - Rank number

  @retval N/A

--*/
VOID
RestoreRttNom (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    dimm,
  UINT8    rank
  )
{
#ifndef DDR5_SUPPORT
  UINT8               ch;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct rankDevice   (*rankStruct)[MAX_RANK_DIMM];
#ifdef  LRDIMM_SUPPORT
  UINT8               d;
  UINT8               cwData;
  UINT8               rankBitMask;
  UINT16              Mr1WlClr;
#endif  //  LRDIMM_SUPPORT
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);

  //
  // Restore Rtt Nom setting and exit WL mode
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, socket, ch);
    rankStruct = GetRankStruct (Host, socket, ch, dimm);

    //
    // Skip if no rank
    //
    if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
      continue;
    }

    //
    // Deassert ODT overrides
    //
    DeAssertChipODTOveride (Host, socket, ch);
    // Wait for ODTLoff (CWL-2)
    FixedDelayMicroSecond (1);


#ifdef  LRDIMM_SUPPORT
    if (ShouldChipDisableDDR4LRDIMMWriteLeveling (Host, socket, ch, dimm, rank)) {
      //
      // Disable LRDIMM Host side training mode
      //
      WriteLrbuf (Host, socket, ch, dimm, 0, LRDIMM_NORMAL_MODE, LRDIMM_F0, LRDIMM_BC0C);
      (*rankStruct)[rank].CurrentLrdimmTrainingMode = LRDIMM_NORMAL_MODE;
      //
      // Restore to RTT_PARK
      //
      cwData = (*channelNvList)[ch].dimmList[dimm].lrBuf_BC02;
      WriteLrbuf (Host, socket, ch, dimm, 0, cwData, LRDIMM_F0, LRDIMM_BC02);
      //
      // restore Host interface DQ/DQS driver settings for all dimms in the channel
      //
      for (d = 0; d < MAX_DIMM; d++) {
        if ((*DimmNvList)[d].dimmPresent == 0) {
          continue;
        }

        if (!(IsLrdimmPresent (socket, ch, d))) {
          continue;
        }
        cwData = (*channelNvList)[ch].dimmList[d].lrBuf_BC03;
        WriteLrbuf (Host, socket, ch, d, 0, cwData, LRDIMM_F0, LRDIMM_BC03);
      }
      //
      //Per IDT DDR4 Data Buffer A0 sightings report version 2013_01_21_1349
      //sighting number 100003
      //Program BC07 to select the proper rank again.
      //
      //rank present bit mask
      rankBitMask = 0;
      switch ((*channelNvList)[ch].dimmList[dimm].numDramRanks) {
      case 1:
        rankBitMask = 0xE;
        break;

      case 2:
        rankBitMask = 0xC;
        break;

      case 4:
        rankBitMask = 0x0;
        break;
        default:
        //Automated add of default case. Please review.
        break;
      }
      cwData = rankBitMask;
      WriteLrbuf (Host, socket, ch, dimm, 0, cwData, LRDIMM_F0, LRDIMM_BC07);

      if (FeaturePcdGet (PcdCteBuild) == TRUE) {
        RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                        "CTE workaround: About to send MR1 with WRITE LEVEL TRAIN MODE CLEAR\n");
        //
        // Restore MR1 to disable WL mode
        //
        Mr1WlClr = (*rankStruct)[rank].MR1 & ~(MR1_WLE);
        WriteMRS (Host, socket, ch, dimm, rank, Mr1WlClr, BANK1);
      }

    } else
#endif
    {

#ifdef  LRDIMM_SUPPORT
      if (Host->var.mem.InPbaWaMode == 0)
#endif
      {
        //
        // Restore Rtt_nom, keeping WL mode enabled
        // Use separate MR1 write for Native QR DIMMs because ODT is strapped high on Ranks 1 and 3
        // Assume wait for tMOD includes ODTLoff (CWL-2)
        //
        WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR1 | MR1_WLE, BANK1);

        //
        // Restore MR1 to disable WL mode
        //
        WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR1, BANK1);
      }

      DoChipCompatibleRTT_PARKRestore (Host, socket, ch, dimm, rank);
    }

  } // ch loop
#endif // !DDR5_SUPPORT
} // RestoreRttNom


/*++

  This function execute the write leveling Cleanup data evaluation for DDR4 and DDRT.

  @param Host                - Pointer to sysHost
  @param ByteOff             - Byte Offset
  @param ByteSum             - Byte Sum
  @param ByteCount           - Byte Count
  @param Done                - Done indication from centering flow
  @param CRAddDelay          - Additive Delay
  @param WLChipCleanUpStruct - WL Delays
  @param DdrTech             - DDR Technology 0 DDR4 1 DDRT

  @retval NA

--*/
VOID
WriteLevelingCleanUpDataEvalTech (
  IN PSYSHOST                Host,
  INT16                      ByteOff[MAX_CH][MAX_RANK_CH][MAX_STROBE],
  IN INT16                   ByteSum[2][MAX_CH],
  IN UINT16                  ByteCount[2][MAX_CH],
  IN UINT8                   Done,
  IN INT16                   CRAddDelay[MAX_CH],
  IN CHIP_WL_CLEANUP_STRUCT  WLChipCleanUpStruct,
  IN UINT8                   DdrTech
  )
{
  UINT8                   Socket;
  UINT8                   Ch;
  UINT8                   Dimm;
  UINT8                   Rank;
  UINT8                   Strobe;
  UINT8                   AllGood;
  UINT8                   AllGoodLoops;
  INT16                   TxDqs;
  INT16                   TxDq;
  INT16                   GlobalByteOff = 0;
  INT16                   LocalOffset = 0;
  INT16                   TargetOffset = 0;
  UINT8                   MaxStrobe;
  UINT32                  StrobeFail[MAX_CH]; // Bit mask indicating which ch/byte fails to pass
  UINT8                   FaultyPartsStatus;
  struct channelNvram     (*ChannelNvList)[MAX_CH];
  struct ddrRank          (*RankList)[MAX_RANK_DIMM];
  struct dimmNvram        (*DimmNvList)[MAX_DIMM];
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ZeroMem ((UINT8 *) StrobeFail, sizeof (StrobeFail));
  //
  // Return if this Socket is disabled
  //
  Socket = Host->var.mem.currentSocket;
  ChannelNvList = GetChannelNvList (Host, Socket);
  //
  // Evaluate the data for DDR4/DDRT DIMMs
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    GetChipTimingData (Host, Socket, Ch, &WLChipCleanUpStruct);
    //
    // Caclulate the average offset, rounding up. Apply that offset to the global MC CRAddDelay register
    //
    if (ByteCount[DdrTech][Ch]) {
      TargetOffset = (ByteSum[DdrTech][Ch] + (ByteCount[DdrTech][Ch] / 2)) / ByteCount[DdrTech][Ch];
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (!DoesDdrTechMatchThisDimm (DdrTech, (*ChannelNvList)[Ch].dimmList[Dimm].DcpmmPresent)) {
        continue; // Skip DDR4/DDRT when DdrTech does not match DIMMs
      }
      RankList = GetRankNvList (Host, Socket, Ch, Dimm);

      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_WR_FLY_BY, (UINT16) ((CHECKPOINT_DATA_WR_FLY_BY_EVALUATE << 12) | (Socket << 8) | (Dimm << 4) | Rank)));

        if (Done == 0) {
          MaxStrobe = GetMaxStrobe (Host, Socket, Ch, Dimm, WrLvlDelay, DdrLevel);
          for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {

            if (StrobeFail[Ch] & (1 << Strobe)) {
              Host->nvram.mem.socket[Socket].FaultyPartsFlag[Ch] |= (1 << Strobe);
              (*RankList)[Rank].faultyParts[Strobe] |= FPT_WR_FLY_BY_FAILED;

              FaultyPartsStatus = EvaluateFaultyParts (Host, Socket, Ch);

              switch (FaultyPartsStatus) {
              case FPT_NO_ERROR:
                break;
              case FPT_CORRECTABLE_ERROR:
                // if correctable  log the warning -> OutputWarning
                // careful for cases when warning gets promoted to error !!!  - log the Strobe
                RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT, "Failed Write Fly By\n");
                EwlOutputType2 (WARN_FPT_CORRECTABLE_ERROR, WARN_FPT_MINOR_WR_FLYBY, Socket, Ch, Dimm, Rank, EwlSeverityWarning, Strobe, NO_BIT, WrLvlDelay, DdrLevel, 0xFF);
                break;
              default:
                // does anything need to be done here?
                RcDebugPrint (SDBG_MINMAX,
                                        "Write Fly By training failure!!!\n");
                //
                // Add this error to the warning log for both correctable and uncorrectable errors.
                //
                DisableChannelSw (Host, Socket, Ch);
                EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_WR_FLYBY, Socket, Ch, Dimm, Rank, EwlSeverityWarning, Strobe, NO_BIT, WrLvlDelay, DdrLevel, 0xFF);
                break;
              } // switch FaultyPartsStatus
            } // if strobeFail
          } // for Strobe
        } // if Done

        AllGood = 0;
        AllGoodLoops = 0;
        while (AllGood == 0) {

          UpdateGlobalOffsets (Host, Ch, TargetOffset, &CRAddDelay[Ch], &GlobalByteOff);

          RcDebugPrintLine (SDBG_MAX, 80, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                                  "Current values: GlobalByteOff = %d DClks, CRAddDelay[%d] = %d DClks\n", GlobalByteOff, Ch, CRAddDelay[Ch]);

          // Refine TargetOffset to make sure it works for all byte lanes
          AllGood = 1;
          MaxStrobe = GetMaxStrobe (Host, Socket, Ch, Dimm, WrLvlDelay, DdrLevel);
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                                  "Refining TargetOffset for all byte lanes...\n");
          RcDebugPrint (SDBG_MAX,
                                  "\t\tTgtOffset\tByteOff\n");

          RcDebugPrintLine (SDBG_MAX, 80, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

          for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
            //
            // Skip if this is an ECC Strobe when ECC is disabled
            //
            if (IsStrobeNotValid (Host, Strobe)) {
              continue;
            }
            if ((*RankList)[Rank].faultyParts[Strobe]) {
              continue;
            }

            LocalOffset = GlobalByteOff + (128 * (ByteOff[Ch][ (*RankList)[Rank].rankIndex][Strobe] - TargetOffset));
            GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, (INT16 *)&TxDqs);
            // 447 on TxDqs should give enough room for DQ/DQS margins
            if ((TxDqs + LocalOffset) > 447) {
              AllGood = 0;
              AllGoodLoops += 1;
              TargetOffset += 1;
              break;
            }
            if ((TxDqs + LocalOffset) < 40) {
              AllGood = 0;
              AllGoodLoops += 1;
              TargetOffset -= 1;
              break;
            }
            RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
                                    "%d DClks\t%d DClks\n", TargetOffset, ByteOff[Ch][(*RankList)[Rank].rankIndex][Strobe]);
          } // Strobe loop

          // Avoid an infinite loop
          if (AllGoodLoops > 3) {
            RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                                    "TargetOffset refining failed \n");
            EwlOutputType2 (WARN_FPT_CORRECTABLE_ERROR, WARN_WR_FLYBY_UNCORR, Socket, Ch, Dimm, Rank, EwlSeverityWarning, Strobe, NO_BIT, WrLvlDelay, DdrLevel, 0xFF);

            //
            // This is a fatal error condition so map out the entire Channel.
            //
            DisableChannelSw (Host, Socket, Ch);
            AllGood = 1;
          }
        } // while AllGood
      } // Rank loop
    } // Dimm loop

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (!DoesDdrTechMatchThisDimm (DdrTech, (*ChannelNvList)[Ch].dimmList[Dimm].DcpmmPresent)) {
        continue; // Skip DDR4/DDRT when DdrTech does not match DIMMs
      }

      RankList = GetRankNvList (Host, Socket, Ch, Dimm);

      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        // Program the final settings to the DQ bytes and update MrcData
        MaxStrobe = GetMaxStrobe (Host, Socket, Ch, Dimm, WrLvlDelay, DdrLevel);
        RcDebugPrint (SDBG_MAX,
                                "\t\tTxDq (PI)\tTxDqs (PI)\tPI Offset\n");

        RcDebugPrintLine (SDBG_MAX, 80, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

        for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
          //
          // Skip if this is an ECC Strobe when ECC is disabled
          //
          if (IsStrobeNotValid (Host, Strobe)) {
            continue;
          }
          if ((*RankList)[Rank].faultyParts[Strobe]) {
            continue;
          }

          LocalOffset = GlobalByteOff + (128 * (ByteOff[Ch][ (*RankList)[Rank].rankIndex][Strobe] - TargetOffset));
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, (INT16 *)&TxDqs);
          TxDqs = TxDqs + LocalOffset;
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_UPDATE_CACHE | GSM_FORCE_WRITE, (INT16 *)&TxDqs);
          if (MaxStrobe == MSVx8) {
            GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe + MSVx8, 0, DdrLevel, TxDqsDelay, GSM_UPDATE_CACHE | GSM_FORCE_WRITE, (INT16 *)&TxDqs);
          }

          TxDq = TxDqs + TX_DQS_DQ_OFFSET;
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, TxDqDelay, GSM_UPDATE_CACHE | GSM_FORCE_WRITE, (INT16 *)&TxDq);
          if (MaxStrobe == MSVx8) {
            GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe + MSVx8, 0, DdrLevel, TxDqDelay, GSM_UPDATE_CACHE | GSM_FORCE_WRITE, (INT16 *)&TxDq);
          }
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT, "%3d\t\t%3d\t\t%3d\n", TxDq, TxDqs, LocalOffset);
        } // Strobe loop
      } // Rank loop

      // Update MC Delay
      UpdateChipMCDelay (Host, Socket, Ch, Dimm, ChannelNvList, CRAddDelay, &WLChipCleanUpStruct);
    } // Dimm loop
  } // Ch loop
}

/*++

  this function execute the write leveling Cleanup.
  Center TxDQS-CLK timing

  @param Host    - Pointer to sysHost
  @param socket  - Socket number

  @retval SUCCESS

--*/
UINT32
WriteLevelingCleanUp (
  PSYSHOST  Host
  )
{
  UINT8                   socket;
  UINT8                   ch;
  UINT8                   dimm;
  UINT8                   rank;
  UINT8                   logRank;
  UINT8                   rankPresent;
  UINT8                   strobe;
  UINT8                   maxStrobe;
  UINT8                   offset;
  UINT32                  chBitmask;
  UINT8                   DqOffset;
  const INT16             DqOffsets[11] = {0, -4, 4, -8, 8, -12, 12, -20, 20, -40, 40};
  INT16                   Offsets[CHIP_OFFSET_ARRAY_SIZE] = CHIP_OFFSET_ARRAY_VALUES;
  INT16                   ByteOff[MAX_CH][MAX_RANK_CH][MAX_STROBE]; // Passing offset for each ch/byte.
  INT16                   ByteSum[2][MAX_CH]; // Sum of passing offsets for a ch (DDR4/DDRT)
  UINT16                  ByteCount[2][MAX_CH]; // Sum of passing offsets for a ch (DDR4/DDRT)
  UINT32                  strobeMask[MAX_CH];
  UINT32                  Result;
  UINT32                  errResult;
  UINT32                  SkipMe;
  UINT32                  strobePass[MAX_CH]; // Bit mask indicating which ch/byte has passed
  UINT32                  strobeFail[MAX_CH]; // Bit mask indicating which ch/byte fails to pass
  INT16                   txDqs;
  INT16                   txDq;
  INT16                   CRAddDelay[MAX_CH];
  INT16                   GlobalByteOff = 0;
  UINT8                   chStatus;
  UINT32                  bwSerr[MAX_CH][3];
  UINT8                   bit;
  UINT8                   Done = 0;
  UINT8                   IncludeDeselects;
  struct channelNvram     (*channelNvList)[MAX_CH];
  struct ddrRank          (*rankList)[MAX_RANK_DIMM];
  struct dimmNvram        (*dimmNvList)[MAX_DIMM];
  CHIP_WL_CLEANUP_STRUCT  WLChipCleanUpStruct;
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;
  MRC_RT                  MrcRankList;
  UINT8                   MemSsType;

  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  WLChipCleanUpStruct.ddrtDimm0BasicTiming.Data = 0;
  WLChipCleanUpStruct.tCOTHP.Data = 0;
  WLChipCleanUpStruct.tCOTHP2.Data = 0;

  ZeroMem ((UINT8 *) strobeFail, sizeof (strobeFail));

  if (!IsMemFlowEnabled (WriteLevelingFlybyCleanup)) {
    return SUCCESS;
  }

  //
  // Return if this socket is disabled
  //
  socket = Host->var.mem.currentSocket;

  SetCurrentTestType (socket, WrLvlCoarseTest);

  if (IsSocketDimmPopulated (Host, socket) != TRUE) {
    return SUCCESS;
  }

  channelNvList = GetChannelNvList (Host, socket);

  ZeroMem ((UINT8 *) CRAddDelay, sizeof (CRAddDelay));
  //
  // Put the pattern into the WDB
  //
  IncludeDeselects = GetChipIncludeDeselectsWriteLevelingCleanUp ();
  WriteWDBDqLanePattern (Host, socket, WR_FLY_BY_PATTERN, FLY_BY_CACHELINES, IncludeDeselects);

  for (ch = 0; ch < MaxChDdr; ch++) {
    ByteSum[0][ch] = 0;
    ByteSum[1][ch] = 0;
    ByteCount[0][ch] = 0;
    ByteCount[1][ch] = 0;
    strobeMask[ch] = 0;
  } // ch loop

  //
  // Train each DIMM
  //
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    //
    // Train each rank
    //
    for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
      OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_WR_FLY_BY, (UINT16) ((CHECKPOINT_DATA_WR_FLY_BY_TRAINING << 12) | (socket << 8) | (dimm << 4) |rank)));

      //
      // Initialize to no ranks present
      //
      rankPresent = 0;

      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }

        dimmNvList = GetDimmNvList (Host, socket, ch);
        rankList = GetRankNvList (Host, socket, ch, dimm);

        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT) || (rank >= (*dimmNvList)[dimm].numRanks)) {
          continue;
        }
        //
        // Get the logical rank #
        //
        logRank = GetLogicalRank (Host, socket, ch, dimm, rank);

        //
        // Set strobe bit masks
        //
        strobeMask[ch] = GetStrobeMask (Host);
        if (!(*dimmNvList)[dimm].x4Present) {
#ifdef DDR5_SUPPORT
          if (IsDdr5Present (Host, socket)) {
            strobeMask[ch] &= 0x1F;
          } else
#endif
          {
            strobeMask[ch] &= 0x1FF;
          }
        }

        maxStrobe = GetMaxStrobe (Host, socket, ch, dimm, WrLvlDelay, DdrLevel);

        for (strobe = 0; strobe < maxStrobe; strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if (IsStrobeNotValid (Host, strobe)) {
            continue;
          }

          if ((*rankList)[rank].faultyParts[strobe]) {
            strobeMask[ch] &= ~(1 << strobe);
          }
        } // strobe loop

        //
        // Inidicate this rank is present on at least one channel
        //
        rankPresent = 1;

        SelectCPGCRanks (Host, socket, ch, 1 << logRank, 0, 0);

        strobePass[ch] = 0;
        strobeFail[ch] = strobeMask[ch];
      } // ch loop

      //
      // Go to the next rank if this rank is not present on any channels
      //
      if (!rankPresent) {
        continue;
      }

      // Setup IO test CmdPat = PatWrRd, NumCL = 4, LC = 4, REUTAddress, SOE = 3,
      // WDBPattern, EnCADB = 0, EnCKE = 0, SubSeqWait = 0 )
      chBitmask = GetChBitmask (Host, socket, dimm, rank);

      MemSsType = GetSysCpuCsrAccessVar()->MemSsType[socket];
      MrcRankList.Dimm = dimm;
      MrcRankList.Rank = rank;

      SetupTest (Host, socket, MemSsType, chBitmask, 1, &MrcRankList, DdrLevel, WrLvlCoarseTest, gsmCsnDelim, 0, FIRST_ITERATION_LOOP);

      //
      // Sweep through the cycle offsets until we find a value that passes
      //

      for (offset = 0; offset < (sizeof (Offsets)) / sizeof (INT16); offset++) {
        //
        // Program new delay offsets to DQ/DQS timing:
        //
        RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                                "Current DQS offset delay is %d DClks (DQS offset index=%d)\n", Offsets[offset], offset);

        chBitmask = GetChBitmask (Host, socket, dimm, rank);
        for (ch = 0; ch < MaxChDdr; ch++) {
          if ( !((1 << ch) & chBitmask)) {
            continue;
          }

          dimmNvList = GetDimmNvList (Host, socket, ch);
          if (rank >= (*dimmNvList)[dimm].numRanks) {
            continue;
          }

          SetChipDQDQSTiming (Host, socket, ch, Offsets[offset], &CRAddDelay[ch], &GlobalByteOff, &WLChipCleanUpStruct);

          //Write Tx DQ/DQS Flyby delays
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                                  "Write Tx DQ/DQS Flyby delay: GlobalByteOff = %d, CRAddDelay = %d\n", GlobalByteOff, CRAddDelay[ch]);
          maxStrobe = GetMaxStrobe (Host, socket, ch, dimm, WrLvlDelay, DdrLevel);

          for (strobe = 0; strobe < maxStrobe; strobe++) {
            //
            // Skip if this is an ECC strobe when ECC is disabled
            //
            if (IsStrobeNotValid (Host, strobe)) {
              continue;
            }

            //read cache
            GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, (INT16 *)&txDqs);

            txDqs = txDqs + GlobalByteOff;

            //write csr
            GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, TxDqsDelay, GSM_FORCE_WRITE, (INT16 *)&txDqs);
            if (maxStrobe == MSVx8) {
              //write csr
              GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe + 0, 0, DdrLevel, TxDqsDelay, GSM_FORCE_WRITE, (INT16 *)&txDqs);
            }
            txDq = txDqs + TX_DQS_DQ_OFFSET - TX_PER_BIT_SETTLE_TIME;
            //read csr, write csr
            GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, TxDqDelay, GSM_READ_CSR | GSM_FORCE_WRITE, (INT16 *)&txDq);
            if (maxStrobe == MSVx8) {
              //read cache, write csr
              GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe + MSVx8, 0, DdrLevel, TxDqDelay, GSM_READ_CSR | GSM_FORCE_WRITE, (INT16 *)&txDq);
            }
            //read csr
            GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY | GSM_READ_CSR, (INT16 *)&txDqs);

          } // strobe loop

          // Write Wr ADD Delays
          WriteChipWrADDDelays (Host, socket, ch, 0, Offsets[offset], CRAddDelay, &WLChipCleanUpStruct);
        } // ch loop

        if (chBitmask == 0) {
          Done = 0;
        } else {
          //
          // Run Test across all DqOffsets points
          //
          RcDebugPrint (SDBG_MAX,
                                  "\t\tDqs Index\tDqs Offset (DClks)\tDqOffset (PI)\tPrev Pass\tCurrent Error\tstrobePass\n");

          RcDebugPrintLine (SDBG_MAX, 115, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

          for (DqOffset = 0; DqOffset < (sizeof (DqOffsets)) / sizeof (INT16); DqOffset++) {
            // Update Offset
            for (ch = 0; ch < MaxChDdr; ch++) {
              if ( !((1 << ch) & chBitmask)) {
                continue;
              }
              dimmNvList = GetDimmNvList (Host, socket, ch);
              if (rank >= (*dimmNvList)[dimm].numRanks) {
                continue;
              }
              maxStrobe = GetMaxStrobe (Host, socket, ch, dimm, WrLvlDelay, DdrLevel);

              for (strobe = 0; strobe < maxStrobe; strobe++) {
                //
                // Skip if this is an ECC strobe when ECC is disabled
                //
                if (IsStrobeNotValid (Host, strobe)) {
                  continue;
                }

                //read csr
                GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY | GSM_READ_CSR, (INT16 *)&txDqs);

                txDq = txDqs + TX_DQS_DQ_OFFSET - TX_PER_BIT_SETTLE_TIME + (INT16) DqOffsets[DqOffset];
                //read csr, write csr
                GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, TxDqDelay, GSM_READ_CSR | GSM_FORCE_WRITE, &txDq);
                if (maxStrobe == MSVx8) {
                  //read csr, write csr
                  GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe + MSVx8, 0, DdrLevel, TxDqDelay, GSM_READ_CSR | GSM_FORCE_WRITE, (INT16 *)&txDq);
                }
              } // strobe loop
            } // ch loop

            //
            // Reset the IO
            //
            IO_Reset (Host, socket);

            TestControl (Host, socket, MemSsType, chBitmask, NO_SUBCH, WrLvlCoarseTest, 1, START_TEST, NOT_STOP_TEST);

            // Update results for all ch/bytes
            Done = 1;

            // Update results for all ch/bytes
            for (ch = 0; ch < MaxChDdr; ch++) {
              if (!((1 << ch) & chBitmask)) {
                continue;
              }

              dimmNvList = GetDimmNvList (Host, socket, ch);
              rankList = GetRankNvList (Host, socket, ch, dimm);

              chStatus = 1 << ch;
              bwSerr[ch][0] = 0;
              bwSerr[ch][1] = 0;
              bwSerr[ch][2] = 0;
              CollectTestResults (Host, socket, chStatus, bwSerr);
              // Read out per byte error results and check for any byte error
              Result = 0;
              maxStrobe = GetMaxStrobe (Host, socket, ch, dimm, WrLvlDelay, DdrLevel);
              for (strobe = 0; strobe < maxStrobe; strobe++) {

                bit = (8 * (strobe - MSVx8 * (strobe >= MSVx8))) + (4 * (strobe >= MSVx8));

                if (bit < 32) {
                  errResult = bwSerr[ch][0];
                } else if (bit < 64) {
                  errResult = bwSerr[ch][1];
                } else  {
                  errResult = bwSerr[ch][2];
                }

                if (errResult & (1 << (bit))) {
                  Result |= 1 << strobe;
                }
              } // strobe loop

              SkipMe = (Result & strobeMask[ch]) | strobePass[ch];

              for (strobe = 0; strobe < maxStrobe; strobe++) {
                //
                // Skip if this is an ECC strobe when ECC is disabled
                //
                if (IsStrobeNotValid (Host, strobe)) {
                  continue;
                }

                // If this byte has failed or previously passed, nothing to do
                if (SkipMe & (1 << strobe)) {
                  continue;
                }

                if (~(strobeMask[ch]) & (1 << strobe)) {
                  continue;
                }

                strobePass[ch] |= (1 << strobe);
                ByteOff[ch][ (*rankList)[rank].rankIndex][strobe] = Offsets[offset];

                if (FeaturePcdGet (PcdSeparateCwlAdj) == TRUE) {
                  ByteSum[CWLC_DDR4][ch] += Offsets[offset];
                  ByteCount[CWLC_DDR4][ch] += 1;
                } else {
                  ByteSum[ (*channelNvList)[ch].dimmList[dimm].DcpmmPresent][ch] += Offsets[offset];
                  ByteCount[ (*channelNvList)[ch].dimmList[dimm].DcpmmPresent][ch] += 1;
                }
              } // strobe loop

              if (SkipMe == strobePass[ch] ) {
                RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                                        "\t%2d\t\t%2d\t\t\t%2d\t\tYES\t\t0x%05x\t\t0x%05x\n", Offsets[offset], offset, DqOffsets[DqOffset], Result, strobePass[ch] );
              } else {
                RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                                        "\t%2d\t\t%2d\t\t\t%2d\t\tNO\t\t0x%05x\t\t0x%05x\n", Offsets[offset], offset, DqOffsets[DqOffset], Result, strobePass[ch] );
              }

              strobeFail[ch] &= ~strobePass[ch];

              if ((strobePass[ch] & strobeMask[ch]) != strobeMask[ch]) {
                Done = 0;
              }
            }  // ch loop
            // Jump out of the for DqOffset loop if everybody is passing
            if (Done == 1) {
              break;
            }
          } // DqOffset loop
        } // chBitmask
        // Jump out of the for offset loop if everybody is passing
        if (Done == 1) {
          break;
        }
      } // offset loop
    } // rank loop
  } // dimm loop

  //
  // Evaluate the data
  //
  WriteLevelingCleanUpDataEval (Host, ByteOff, ByteSum, ByteCount, Done, CRAddDelay, WLChipCleanUpStruct);

  DoFinalChipSpecificWLCleanUp (Host, socket);
  //
  // Reset the IO
  //
  IO_Reset (Host, socket);
  FifoTrainReset (Host, socket);
  Host->nvram.mem.socket[socket].TrainingStepDone.HostFlyBy = 1;

#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    DisplayResults (Host, socket, TxDqsDelay);
  }
#endif //DEBUG_CODE_BLOCK

  return SUCCESS;
} // WriteLevelingCleanUp
