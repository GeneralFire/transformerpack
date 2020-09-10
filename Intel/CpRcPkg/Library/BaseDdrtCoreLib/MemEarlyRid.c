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
#include <Library/EmulationConfigurationLib.h>
#include <Library/DdrtCoreLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Memory/MemoryCheckpointCodes.h>

//
// Local function prototypes
//
#define   GNT2ERID_RANGE              8

#define   ERID_PI_RANGE               256
#define   ERID_RESULTS                (ERID_PI_RANGE / 32)
#define   ERID_STEP_SIZE  1
#define   ERID_COARSE_STEP_SIZE  8


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

  @retval status

**/
STATIC
UINT32
FindEarlyRidPiSetting (
              PSYSHOST  Host,
              UINT8     socket,
              UINT32    chBitMask,
              UINT8     dimm,
              UINT8     mode,
              UINT16    risingEdge[MAX_CH],
              UINT16    centerPoint[MAX_CH],
              UINT16    fallingEdge[MAX_CH],
              UINT16    pulseWidth[MAX_CH]
              )
{
  UINT8               ch;
  UINT8               rank = 0;
  UINT32              status = SUCCESS;
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  UINT16 gnt2erid;
  UINT16 eridDelay;
  UINT16 eridOffset;

  //locals
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
  UINT32                  trainRes[MAX_CH][ERID_RESULTS];
  UINT8                   piSettingStopFlag;
  UINT8                   eridResult;
  UINT8                   eridCombinedResult;
  UINT8                   i;
  UINT8                   clk;
  UINT8  DumArr[7]    = {1,1,1,1,1,1,1};
  INT16                   Gnt2EridDimmBaseline[MAX_CH];
  UINT8                   MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  if (mode == 0) {
    RcDebugPrint (SDBG_MAX,
      "Early RID Fine sweep for socket:%d dimm:%d\nPi Offset", socket, dimm);
  } else {
    RcDebugPrint (SDBG_MAX,
      "Early RID Final sweep for socket:%d dimm:%d\nPi Offset", socket, dimm);
  }

  for (ch = 0; ch < MaxChDdr; ch++) {
    for (dWord = 0; dWord < ERID_RESULTS; dWord++) {
      trainRes[ch][dWord] = 0;
    }
    GetSetSxpGnt2Erid (Host, socket, ch, dimm, GSM_READ_ONLY, &Gnt2EridDimmBaseline[ch]);
    RcDebugPrint (SDBG_MAX, "   Ch%d", ch);
  }
  RcDebugPrint (SDBG_MAX,"\n");

  for (eridOffset = 0; eridOffset < ERID_PI_RANGE; eridOffset += ERID_STEP_SIZE) {

    for (ch = 0; ch < MaxChDdr; ch++) {
      // Clear Errors before running test each time
      ClearEridLfsrErr (Host, socket, ch);

      if ((chBitMask & (1 << ch)) == 0) {
        continue;
      }

      gnt2erid = Gnt2EridDimmBaseline[ch] + (eridOffset / 128) - 1;
      eridDelay = eridOffset % 128;

      //
      // Set cycle delay in MC and logic + Pi delay in IO
      //

      if (mode) {
        GetSetSxpGnt2Erid (Host, socket, ch, dimm, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, (INT16 *)&gnt2erid);
      } // if mode
      clk = GetDdrioClkNumberDdrt(Host, socket, ch, dimm);
      GetSetClkDelay (Host, socket, ch, clk, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, (INT16 *)&eridDelay);


    } //ch loop

    if (mode != 0) {
      //
      // Run the test
      //
      RunIOTest(Host, socket, chBitMask, BasicVA, DumArr, 1, 0);
    }

    RcDebugPrint (SDBG_MAX, "%3d      ",  eridOffset);

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((chBitMask & (1 << ch)) == 0) {
        RcDebugPrint (SDBG_MAX, "      ");
        continue;
      }
/*
    ii. 2 1-bit register fields per signal to store samples (one for even "UI's" and one for odd "UI's" per CK and CK# (ERID[1:0] signal).
        Total of 4 bits per slot, 8 bits for both slots.
    iii.  BIOS will invert the ERID1 result and OR with the ERID0 result for even UI's.
    iv. Training register will be constantly updated. Reading the training register while the test is running is not an issue.
    v.  Changing the PI code, BIOS will need to wait for settling but BIOS delays should suffice - latency of message channel access
        is INT32 enough. There is no issue with the test running while the PI code is changed.

        During training, BIOS will look at the "even" samples. The additional samples are included for debug.
        Bit 7: CKP3 early RID (even)
        Bit 6: CKP3 early RID (odd)
        Bit 5: CKN3 early RID (even)
        Bit 4: CKN3 early RID (odd)
        Bit 3: CKP1 early RID (even)
        Bit 2: CKP1 early RID (odd)
        Bit 1: CKN1 early RID (even)
        Bit 0: CKN1 early RID (odd)
        CK2 is slot 0 ERID bits
        CK3 is slot 1 ERID bits
        CKP corresponds to ERID[0]
        CKN corresponds to ERID[1]
*/

      if (mode == 0) {
        //
        // Sample results
        //
        eridResult = GetDdrtTrainResult( Host, socket, ch, dimm);
        if (eridOffset < ERID_PI_RANGE/2) {
          eridCombinedResult = ((eridResult >> 3) & (~(eridResult >> 1))) & 1;
        } else {
          eridCombinedResult = ((eridResult >> 2) & (~(eridResult >> 0))) & 1;
        }
      } else {
        // Collect test results
        eridCombinedResult = 0;
        GetEridCombineResult (Host, socket, ch, &eridCombinedResult);
      } // if mode

      dWord = (UINT8)(eridOffset/32);

      trainRes[ch][dWord] |= eridCombinedResult << (eridOffset % 32);

      for (i = 1; i < ERID_STEP_SIZE; i++) {
        trainRes[ch][dWord] |= (trainRes[ch][dWord] & (1 << (eridOffset % 32))) << i;
      }
      RcDebugPrint (SDBG_MAX, "     %d", (trainRes[ch][dWord] >> (eridOffset % 32)) & 1);

    } // ch loop
    RcDebugPrint (SDBG_MAX, "\n");
  } //eridOffset


  //
  // Now evaluate results... move to separate function TBD
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((chBitMask & (1 << ch)) == 0) {
      continue;
    }

    Register = 0;
    OneStart = 0;

    Bit = BIT0;
    OneCount = 0;
    MaxOneCount = 0;
    CyclicCase = 0;
    piSettingStopFlag = 1;

    risingEdge[ch]  = 0;
    fallingEdge[ch] = 0;
    pulseWidth[ch] = 0;

    //
    // Start at bit 0
    //
    piSetting     = 0;
    piStart       = 0;
    NumberOfCycle = 256;

    while (piSettingStopFlag) {

      dWord = (UINT8)(piSetting / 32);
      //
      // Get the proper training results
      //
      if ((piSetting % 32) == 0) {
        Register  = trainRes[ch][dWord];
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
            MaxOneCount = OneCount;
            risingEdge[ch]  = OneStart;
            fallingEdge[ch] = piSetting;
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
          piSetting = 0;
          CyclicCase  = 1;
        } else {
          //
          // finsh stop the scan
          // We've checked all bits so save the results
          //
          piSettingStopFlag = 0;
          pulseWidth[ch]  = fallingEdge[ch] - risingEdge[ch];
          centerPoint[ch] = ((pulseWidth[ch] / 2) + risingEdge[ch]) % (NumberOfCycle - piStart);
        }
      }
      //
      // Check if this is a cyclic case and if we are at the end of the one's
      //
      if ((CyclicCase == 1) && (OneCount == 0)) {
        piSettingStopFlag = 0;
        if (risingEdge[ch] > fallingEdge[ch]) {
          pulseWidth[ch] = ((NumberOfCycle - risingEdge[ch]) + fallingEdge[ch]) - piStart;
          centerPoint[ch] = (((pulseWidth[ch] / 2) + risingEdge[ch])) % (NumberOfCycle - piStart);
        } else {
          pulseWidth[ch] = fallingEdge[ch] - risingEdge[ch];
          centerPoint[ch] = ((pulseWidth[ch] / 2) + risingEdge[ch]) % (NumberOfCycle - piStart);
        }
      }
    } // while loop

    //
    // If the pulseWidth (ones stream) is smaller than threshold need to sample again
    //
    if (pulseWidth[ch] <= RE_EDGE_SIZE_THRESHOLD) {

      //
      // This is a fatal error condition so map out the entire channel
      //
      Host->nvram.mem.socket[socket].FaultyPartsFlag[ch] |= ~0;
      rankList = GetRankNvList(Host, socket, ch, dimm);
      (*rankList)[rank].faultyParts[0] |= FPT_TRAINING_FAILED;

      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "Failed ERID Pi\n");
      DisableChannelSw (Host, socket, ch);
      EwlOutputType2 (WARN_EARLY_RID, WARN_EARLY_RID_UNCORR, socket, ch, NO_DIMM, NO_RANK, EwlSeverityWarning, NO_STROBE, NO_BIT, MrcGtDelim, MrcGtDelim, (UINT8)pulseWidth[ch]);
    } //if pulseWidth
  } // ch loop

  return status;
} //FindEarlyRidPiSetting


/**

  Basic Fine ERID training uses a HW generated pattern from NVMCTLR

  @param[in] Host - Pointer to SysHost

  @retval status

**/
UINT32
EarlyReadID (
  IN  PSYSHOST  Host
  )
{
  UINT8                 socket;
  UINT8                 ch = 0;
  UINT32                status = SUCCESS;
  UINT8                 dimm;
  UINT8                 rank = 0;
  UINT8                 rankPresent;
  UINT16                risingEdge[MAX_CH];
  UINT16                centerPoint[MAX_CH];
  UINT16                fallingEdge[MAX_CH];
  UINT16                pulseWidth[MAX_CH];
  UINT16                eridOffset;
  struct channelNvram  (*channelNvList)[MAX_CH];
  struct dimmNvram     (*dimmNvList)[MAX_DIMM];
  UINT16                eridDelay;
  INT16                 Gnt2Erid;
  INT16                 Gnt2eridOffset[GNT2ERID_RANGE] = {0, -1, -2, -3, -4, -5 ,-6, -7};
  UINT8                 gnt2eridIndex;
  UINT8                 gnt2eridPassed[MAX_CH];
  UINT32                chBitMask = 0;
  UINT32                chDone;
  UINT8                 DumArr[7]    = {1,1,1,1,1,1,1};
  UINT8                 clk;
  INT16                 Gnt2EridDimmBaseline[MAX_CH];
  UINT8                 FifoPtCtlQnnnH;
  UINT8                 MaxChDdr;

/*
Test Pattern Setup:

Basic Fine ERID training uses a HW generated pattern from NVMCTLR

The Test pattern for Coarse ERID training and Final Fine ERID training should be a write/read loopback pattern,
similar to the checkerboard pattern used for Basic WR DQ-DQS training,
with the addition of enabling LFSR patterns for the Read ID, with separate seeds for ERID[1] and ERID[0].


Read ID Training Algorithm:

The Early Read ID training step is unique to NVM DIMM's. There are two sub-steps for this training step, as described below:


2.  Coarse ERID Training
  a.  General ERID Training Test (LFSR patterns)
    i.  NVMCTLR will return to the general training mode (i.e. RD's from the NVMCTLR Prefetch Buffer.) This flow must include ERID returns,
        in order according to Read order.
    ii.  The Host CPGC engine will be set to support ERID training (CPGC_ERID regs listed above. This enables the generation of
        LFSR pattern RID that are inserted into the RD command, and the comparison of the expected LFSR pattern returned on the ERID signals.
        The Host DDRIO is no longer in the ERID sampling (NVMDIMM Training) training mode.
    iii.  The Host will send RD commands, with the random (LFSR) RID values. The RD com-mands do not necessarily need to be scheduled with
          minimum spacing, but it is ex-pected that the GNT's will be sent with 4 DCLK spacing so that the ERID signal will be sent back to back.
          When in the general training mode, NVMCTLR will return any RID value that is sent during the RD command (i.e. no filtering/interpretation of these values.)
    iv.  NVMCTLR will return data with these ERID values. The data may be the MPR pattern data, since there were no writes to initialize the
        prefetch buffer. The DQ data is irrelevant to this training step.
    v.  The Host will compare the ERID values to the expected generated values.
  b.  This basic test will be run while testing different settings for the GNT2ERID setting in the Host MC (one per slot) and a coarse increment
      of the PI setting. When the pattern comparison passes, the GNT2ERID is correct and coarse ERID training is complete.

3.  Final Fine RID Training
  a.  The same pattern generation and checking (used in Coarse Training) can then be used during a Host sampling delay PI sweep
      to determine the margins, and re-center the PI setting with the more aggressive/realistic patterns
*/

  if (!IsMemFlowEnabled (EarlyReadIdTraining)) {
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

  status = SUCCESS;

#ifdef DEBUG_CODE_BLOCK
  AcquirePrintControl ();
#endif //DEBUG_CODE_BLOCK
  channelNvList = GetChannelNvList(Host, socket);

  SetAepTrainingMode (Host,socket, CHECKPOINT_MINOR_EARLY_RID_COARSE);
  //
  // Reset DDR IO
  //
  IO_Reset (Host, socket);

  //
  //  Basic Coarse ERID Training
  //
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_EARLY_RID_COARSE, (UINT16)((socket << 8)|(dimm << 4)|rank)));

    rankPresent = 0;
    chBitMask = 0;
    chDone = 0;
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      dimmNvList = GetDimmNvList(Host, socket, ch);

      if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT) || (rank >= (*dimmNvList)[dimm].numRanks)) {
        continue;
      }
      if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
        continue;
      }

      rankPresent = 1;
      chBitMask |= 1 << ch;
      chDone |= 1 << ch;
      gnt2eridPassed[ch] = 0;

      SetupCoarseEridTraining (Host, socket, ch, dimm);
      SelectCPGCRanks(Host, socket, ch, 1 << GetLogicalRank(Host, socket, ch, dimm, rank), 0, 0);
    }

    //
    // Go to the next rank if this rank is not present on any channels
    //
    if (!rankPresent) {
      continue;
    }

    RcDebugPrint (SDBG_MAX,
      "Early RID Coarse search for socket:%d dimm:%d\nCycleOff   PI", socket, dimm);

    //
    // Burst of read commands back to back (4 DCLK apart)
    //
    SetupIOTestMPR(Host, socket, chBitMask, 1, 0, 0, 0);

    for (ch = 0; ch < MaxChDdr; ch++) {
       RcDebugPrint (SDBG_MAX, "   Ch%d", ch);

      if ((chBitMask & (1 << ch)) == 0) {
        continue;
      }
      SetupCpgcWdbBuffErid (Host, socket, ch);
      FifoPtCtlQnnnH = 1;
      GetSetFifoPointControl (Host, socket, ch, GSM_FORCE_WRITE, &FifoPtCtlQnnnH);
      GetSetSxpGnt2Erid (Host, socket, ch, dimm, GSM_READ_ONLY, &Gnt2EridDimmBaseline[ch]);
    } // ch loop

    RcDebugPrint (SDBG_MAX, "\n");

    for (gnt2eridIndex = 0; gnt2eridIndex < GNT2ERID_RANGE; gnt2eridIndex++) {

      for (eridOffset = 0; eridOffset < 128; eridOffset += ERID_COARSE_STEP_SIZE) {

        RcDebugPrint (SDBG_MAX,
          "  %3d     %3d", Gnt2eridOffset[gnt2eridIndex], eridOffset);

        //
        // Set cycle delay in MC
        //
        for (ch = 0; ch < MaxChDdr; ch++) {

          // Clear Errors before running test each time
          ClearEridLfsrErr (Host, socket, ch);

          if ((chBitMask & (1 << ch)) == 0) {
            continue;
          }
          if (gnt2eridPassed[ch]) {
            continue;
          }

          Gnt2Erid = Gnt2EridDimmBaseline[ch] + Gnt2eridOffset[gnt2eridIndex];
          GetSetSxpGnt2Erid (Host, socket, ch, dimm, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &Gnt2Erid);
          clk = GetDdrioClkNumberDdrt(Host, socket, ch, dimm);
          GetSetClkDelay (Host, socket, ch, clk, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, (INT16 *)&eridOffset);
        } //ch loop

        //
        // Run the test
        //
        RunIOTest(Host, socket, chBitMask, BasicVA, DumArr, 1, 0);

        //
        // Evaluate results
        //
        for (ch = 0; ch < MaxChDdr; ch++) {
          if (((chBitMask & (1 << ch)) == 0) || gnt2eridPassed[ch]) {
             RcDebugPrint (SDBG_MAX, "      ");
            continue;
          }

          // Collect test results
          CollectTestResultsErid (Host, socket, ch, dimm, Gnt2eridOffset[gnt2eridIndex], gnt2eridIndex, eridOffset, &gnt2eridPassed[ch]);

          if (gnt2eridPassed[ch] == 1) {
            chDone &= ~(1<<ch);
          }

          RcDebugPrint (SDBG_MAX,
            "     %d", gnt2eridPassed[ch]);
        } //ch loop
        RcDebugPrint (SDBG_MAX,"\n");

        if (chDone == 0) {
          break;
        }
      } //eridOffset
      if (chDone == 0) {
        break;
      }
    } //gnt2eridOffset
    for (ch = 0; ch < MaxChDdr; ch++) {
      // skip non NVMDIMM or unpopulated channels
      if ((chBitMask & (1 << ch)) == 0) {
        continue;
      }

      // error message for failing channel
      if (gnt2eridPassed[ch] == 0) {
        DisableChannelSw (Host, socket, ch);
        EwlOutputType2 (WARN_EARLY_RID, WARN_EARLY_RID_CYCLE_FAIL, socket, ch, dimm, rank, EwlSeverityWarning, NO_STROBE, NO_BIT, MrcGtDelim, MrcGtDelim, 0xFF);
      }
    } // ch loop
  } //dimm loop

  //
  //  Final Fine RID Training
  //
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_EARLY_RID_FINAL, (UINT16)((socket << 8)|(dimm << 4)|rank)));

    rankPresent = 0;
    chBitMask = 0;
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      dimmNvList = GetDimmNvList(Host, socket, ch);

      if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT) || (rank >= (*dimmNvList)[dimm].numRanks)) {
        continue;
      }
      if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
        continue;
      }

      //
      // Indicate this rank is present on at least one channel
      //
      rankPresent = 1;
      chBitMask |= 1 << ch;
    } // ch loop

    //
    // Go to the next rank if this rank is not present on any channels
    //
    if (!rankPresent) {
      continue;
    }


    //
    // Burst of read commands back to back (4 DCLK apart)
    //
    SetupIOTestMPR(Host, socket, chBitMask, 1, 0, 0, 0);

    for (ch = 0; ch < MaxChDdr; ch++) {
      //RcDebugPrint (SDBG_MAX, "   Ch%d", ch);

      if ((chBitMask & (1 << ch)) == 0) {
        continue;
      }
      SetupCpgcWdbBuffErid (Host, socket, ch);
    } // ch loop


    //
    // Find the ERID high phase
    //
    for (ch = 0; ch < MaxChDdr; ch++) {
      pulseWidth[ch] = 0;
    }
    status = FindEarlyRidPiSetting (Host, socket, chBitMask, dimm, 1, risingEdge, centerPoint, fallingEdge, pulseWidth);

    //
    // Apply final center results
    //
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((chBitMask & (1 << ch)) == 0) {
        continue;
      }

      //
      // Determine center results
      //
      GetSetSxpGnt2Erid (Host, socket, ch, dimm, GSM_READ_ONLY, &Gnt2Erid);
      Gnt2Erid += (centerPoint[ch] / 128) - 1;
      eridDelay = centerPoint[ch] % 128;

      //
      // Set cycle delay in MC and logic + Pi delay in IO
      //

      GetSetSxpGnt2Erid (Host, socket, ch, dimm, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &Gnt2Erid);
      clk = GetDdrioClkNumberDdrt(Host, socket, ch, dimm);
      GetSetClkDelay (Host, socket, ch, clk, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, (INT16 *)&eridDelay);

      //
      // Disable ERID LFSR training mode
      //
      DisableEridLfsrMode (Host, socket, ch);

    } // ch loop
  } // dimm loop

  SetAepTrainingMode (Host,socket, DISABLE_TRAINING_STEP);
  DisableEridReturn (Host, socket);
  //
  // Reset DDR IO
  //
  IO_Reset (Host, socket);
  FifoTrainReset(Host, socket);
#ifdef DEBUG_CODE_BLOCK
  ReleasePrintControl ();
  //
  // Summarization
  //
  if (GetDebugLevel () & SDBG_MAX) {
    AcquirePrintControl ();
    RcDebugPrint (SDBG_DEFAULT, "\n Early Read ID Training Results\n");
    RcDebugPrint (SDBG_DEFAULT, "---------------------------------\n");
    RcDebugPrint (SDBG_DEFAULT, "START_EARLY_RID\n");

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      rankPresent = 0;
      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }
        dimmNvList = GetDimmNvList(Host, socket, ch);
        if (CheckRank(Host, socket, ch, dimm, rank, CHECK_MAPOUT) || (rank >= (*dimmNvList)[dimm].numRanks)) {
          continue;
        }
        if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
          continue;
        }
        rankPresent = 1;
      } // ch loop

      if (!rankPresent) {
        continue;
      }

      for (ch = 0; ch < MaxChDdr; ch++) {
        clk = GetDdrioClkNumberDdrt(Host, socket, ch, dimm);
        GetSetClkDelay (Host, socket, ch, clk, GSM_READ_ONLY, (INT16 *)&eridDelay);
        GetSetSxpGnt2Erid (Host, socket, ch, dimm, GSM_READ_ONLY, &Gnt2Erid);
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                       "PW = %d, GNT2ERID = %d, ERID Pi delay = %d\n", pulseWidth[ch], Gnt2Erid, eridDelay);
      } // ch loop
    } // dimm loop
    RcDebugPrint (SDBG_DEFAULT, "STOP_EARLY_RID\n");
    ReleasePrintControl ();
  }
#endif // DEBUG_CODE_BLOCK

  return status;
} //EarlyReadID
