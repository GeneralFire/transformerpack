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
#include <Library/RcMemAllocLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/SimulationServicesLib.h>
#include "Include/BaseMemoryCoreInternal.h"
#include <Library/SysHostPointerLib.h>
#include <Library/MemTurnaroundTimingLib.h>

//
// Local function prototypes
//
STATIC
VOID
WrDqDqsInit (
  PSYSHOST        Host,
  UINT8           socket
  );

STATIC
UINT32
FindPerBitResults (
  PSYSHOST     Host,
  UINT8        socket,
  UINT8        dimm,
  UINT8        rank,
  BErrorResult *ErrorResult
  );

//
// Internal definitions
//
// Define the max number of sample in this algorithm
//
#define WRITE_DQ_DQS_MAX_SAMPLE (MAX_PHASE_IN_FINE_ADJUSTMENT * 2)


UINT32
WriteDqDqsPerBit (
  PSYSHOST Host
  )
{
  UINT8               socket;
  UINT8               ch;
  UINT32              chBitmask;
  UINT8               dimm;
  UINT8               rank;
  UINT8               logRank = 0;
  UINT8               nibble;
  UINT8               faultyStrobe;
  UINT8               bit, bitIdx, msl;
  INT16               BitSkew[BITS_PER_NIBBLE];
  INT16               bitNibbleMin;
  INT16               bitSkewMax;
  UINT16              bitCenter[8];
  UINT8               rankPresent;
  PerBitPi            errorStart[MAX_CH];
  PerBitPi            errorEnd[MAX_CH];
  PerBitPi            piPosition[MAX_CH];
  UINT32              status;
  UINT16              txDqs;
  UINT8               faultyPartsStatus;
  UINT8               problemEyeSize = 0;
  UINT8               problemStrobe = 0;
  struct bitMarginCh  resultsBit;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  BErrorResult        (*ErrorResult)[MAX_CH];
  TCPGCAddress        CPGCAddress = {{0,   0, 0,    0},  // Start
                                     {0, 0xF, 0, 0x1F},  // Stop
                                     {0,   0, 0,    0},  // Order
                                     {0,   0, 0,    0},  // IncRate
                                     {0,   4, 0,    1}}; // IncValue
  TWDBPattern         WDBPattern = {0,  0,  0,  BasicVA};
  UINT8               MSVx4 = GetMaxStrobeValid (Host);
  UINT8               MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8               MBV = GetMaxBitsValid (Host);
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  socket = Host->var.mem.currentSocket;
  channelNvList = GetChannelNvList (Host, socket);

  //
  // Enable WrCRC in the DDRIO if applicable
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    if (IsEarlyWrCrcRequired (ch) && !(Host->nvram.mem.RASmode & STAT_VIRT_LOCKSTEP)) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "Early WrCRC for the DDRIO enabled\n");
      EnableWrCrcDdrio (Host, socket, ch);
    }
  } // ch loop

  if (!IsMemFlowEnabled (WriteDqDqsTraining)) {
    return SUCCESS;
  }

  //
  // Return if this socket is disabled
  //

  if (IsSocketDimmPopulated (Host, socket) != TRUE) {
    return SUCCESS;
  }

  ErrorResult = RcAllocatePool (sizeof (*ErrorResult));
  if (ErrorResult == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return FAILURE;
  }
  status = SUCCESS;

  WrDqDqsInit (Host, socket);

  // Initialize LE and RE Margin results structure
  for (ch = 0; ch < MaxChDdr; ch++) {
    for (bit = 0; bit < MBV; bit++) {
      resultsBit.bits[ch][bit].n = 0;
      resultsBit.bits[ch][bit].p = 0;
    }
  } // Ch loop

  //
  // Train each DIMM
  //
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    //
    // Train each rank
    //
    for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
      //
      // Initialize to no ranks present
      //
      rankPresent = 0;

      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }

        rankList = GetRankNvList (Host, socket, ch, dimm);
        dimmNvList = GetDimmNvList (Host, socket, ch);
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT) || (rank >= (*dimmNvList)[dimm].numRanks)) {
          continue;
        }
        //
        // Inidicate this rank is present on at least one channel
        //
        rankPresent |= (*rankList)[rank].enabled;
        //
        // Get the logical rank #
        //
        logRank = GetLogicalRank (Host, socket, ch, dimm, rank);

        SelectCPGCRanks (Host, socket, ch, 1 << logRank, 0, 0);

      } // ch loop

      chBitmask = GetChBitmask (Host, socket, dimm, rank);

      SetupIOTest (Host, socket, chBitmask, PatWrRd, 32, 1, &CPGCAddress, NSOE, &WDBPattern, 0, 0, CPGC_SUBSEQ_WAIT_DDRT); //FNV restricted to 32
      SetCpgcPatControl (Host, socket, chBitmask, BTBUFFER, BTBUFFER, BTBUFFER, DDR_CPGC_PATBUF_MUX0, DDR_CPGC_PATBUF_MUX1, DDR_CPGC_PATBUF_MUX2);


      //
      // Go to the next rank if this rank is not present on any channels
      //
      if (!rankPresent) {
        continue;
      }

      //
      // Data collecting
      //
      status = FindPerBitResults (Host, socket, dimm, rank, (*ErrorResult));

      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }

        rankList = GetRankNvList (Host, socket, ch, dimm);
        dimmNvList = GetDimmNvList (Host, socket, ch);
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT) || (rank >= (*dimmNvList)[dimm].numRanks)) {
          continue;
        }

        status = SUCCESS;

        for (bit = 0; bit < MBV; bit++) {
          nibble = bit / 4;
          faultyStrobe = GetStrobeFromNibble (Host, nibble);

          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if (IsBitNotValid (Host, bit)) {
            continue;
          }
          if ((*rankList)[rank].faultyParts[faultyStrobe] != 0) {
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, faultyStrobe, NO_BIT,
                           "skipped Write DqDqs FindErrorGraphMin\n");
            continue;
          }

          status = FindErrorGraphMin (Host, socket, ch, dimm, rank, 0, &(*ErrorResult)[ch].BitErrorResult[bit][0],
                     WRITE_DQ_DQS_MAX_SAMPLE, &errorStart[ch].PiPosition[bit],
                     &errorEnd[ch].PiPosition[bit], &piPosition[ch].PiPosition[bit]);

          errorStart[ch].PiPosition[bit] = errorStart[ch].PiPosition[bit] + WR_DQ_PI_START;
          errorEnd[ch].PiPosition[bit] = errorEnd[ch].PiPosition[bit] + WR_DQ_PI_START;
          piPosition[ch].PiPosition[bit] = piPosition[ch].PiPosition[bit] + WR_DQ_PI_START;

          //
          // Add in Tx Dqs offset
          //
          if ((*dimmNvList)[dimm].x4Present) {
            GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, faultyStrobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, (INT16 *)&txDqs);
          } else {
            GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, faultyStrobe % MSVx8, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, (INT16 *)&txDqs);
          }

          errorStart[ch].PiPosition[bit] = (UINT16) (txDqs - TX_DQ_START_OFFSET - TX_PER_BIT_SETTLE_TIME) + errorStart[ch].PiPosition[bit];
          errorEnd[ch].PiPosition[bit] = (UINT16) (txDqs - TX_DQ_START_OFFSET - TX_PER_BIT_SETTLE_TIME) + errorEnd[ch].PiPosition[bit];
          piPosition[ch].PiPosition[bit] = (UINT16) (txDqs - TX_DQ_START_OFFSET - TX_PER_BIT_SETTLE_TIME) + piPosition[ch].PiPosition[bit];

          //LE and RE Margins
          resultsBit.bits[ch][bit].n = errorEnd[ch].PiPosition[bit];
          resultsBit.bits[ch][bit].p = errorStart[ch].PiPosition[bit];

          if (status == FAILURE) {
            //
            // Mark this part as faulty
            //
            Host->nvram.mem.socket[socket].FaultyPartsFlag[ch] |= (1 << faultyStrobe);

            (*rankList)[rank].faultyParts[faultyStrobe] |= FPT_WR_DQ_DQS_FAILED;

            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, faultyStrobe, bit,
                           "Failed Write DqDqsPerBit\n");

            faultyPartsStatus = EvaluateFaultyParts (Host, socket, ch);

            switch (faultyPartsStatus) {
            case FPT_NO_ERROR:
              status = SUCCESS;
              break;
            case FPT_CORRECTABLE_ERROR:
              // if correctable log the warning -> OutputWarning
              // careful for cases when warning gets promoted to error !!!  - log the strobe
              RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, rank, faultyStrobe, NO_BIT,
                             "WrDqDqs Correctable\n");
              EwlOutputType2 (WARN_FPT_CORRECTABLE_ERROR, WARN_FPT_MINOR_WR_DQ_DQS, socket, ch, dimm, rank, EwlSeverityWarning, faultyStrobe, bit, TxDqDelay, DdrLevel, (UINT8) (errorStart[ch].PiPosition[bit] - errorEnd[ch].PiPosition[bit]));
              status = SUCCESS;
              break;
            default:
              // no need to warning/disable channel - it is done out of the loop
              problemEyeSize = (UINT8) (errorStart[ch].PiPosition[bit] - errorEnd[ch].PiPosition[bit]);
              problemStrobe = faultyStrobe;
              status = FAILURE;
              break;
            }

            if (status == FAILURE) {
              break;  // failure confirmed, break the bit loop.
            }
          }
        } // bit loop

#ifdef DEBUG_CODE_BLOCK
        //Print LE and RE Per Bit Deskew Margins for current rank
        if (GetDebugLevel () & SDBG_MAX) {
          DisplayREandLEMargins (Host, socket, ch, dimm, rank, &resultsBit, TxDqDelay);
        }
#endif // DEBUG_CODE_BLOCK
        DetectLowMargin (Host, ch, dimm, rank, &resultsBit, TxDqDelay, NON_PER_BIT);

        if (status) {
          RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                         "WrDq/Dqs Training Failure! (status = %d)\n", status);
          EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_WR_DQ_DQS, socket, ch, dimm, rank, EwlSeverityWarning, problemStrobe, NO_BIT, TxDqsDelay, DdrLevel, problemEyeSize);

          //
          // This is a fatal error condition so map out the entire channel.
          //
          DisableChannelSw (Host, socket, ch);
        } // if status

        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "dimm = %d, rank = %d\n", dimm, rank);
        RcDebugPrint (SDBG_MAX,
                      "         Per Bit Margin Center       Per Bit Skew\n");
        RcDebugPrint (SDBG_MAX,
                      "N# = Nibble, BCx = Bit Center, BSx = Bit Skew , MSL = Most Skewed Lane\n\n");
        RcDebugPrint (SDBG_MAX,
                      "N# BC0 BC1 BC2 BC3   NPi   BS0 BS1 BS2 BS3  MSL\n");

        // Loop for each nibble/strobe.
        for (nibble = 0; nibble < MSVx4; nibble++) {
          //
          // Skip if this is an ECC nibble when ECC is disabled
          // Note: unlike usual in training code, per-bit skew register are mapped linerly, so ECC bits are on the end
          //
          if (IsNibbleNotValid (Host, nibble)) {
            continue;
          }
          faultyStrobe = GetStrobeFromNibble (Host, nibble);
          bitNibbleMin = 512;   // Set to largest possible
          RcDebugPrint (SDBG_MAX,
                      "%2d ", nibble);

          // Loop for all the bits in this nibble
          for (bit = 0; bit < BITS_PER_NIBBLE; bit++) {
            // Get the absolute bit index (0-72)
            bitIdx = (nibble * 4) + bit;

            // Find the center for this bit
            bitCenter[bit] = piPosition[ch].PiPosition[bitIdx];

            RcDebugPrint (SDBG_MAX,
                          "%3d ", bitCenter[bit]);

            // Check if the center of this bit is smaller than the smallest found so far
            if (bitCenter[bit] < bitNibbleMin) {
              bitNibbleMin = bitCenter[bit];
            }
          } // bit loop

          RcDebugPrint (SDBG_MAX,
                        "  %3d   ", bitNibbleMin);
          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, faultyStrobe, 0, DdrLevel, TxDqDelay, GSM_UPDATE_CACHE, &bitNibbleMin);
          // Loop for all the bits in this nibble
          bitSkewMax = 0;
          msl = 0;
          for (bit = 0; bit < BITS_PER_NIBBLE; bit++) {
            BitSkew[bit] = bitCenter[bit] - bitNibbleMin;
            if (BitSkew[bit] > bitSkewMax) {
              bitSkewMax = BitSkew[bit];
              msl = bit;
            }
            RcDebugPrint (SDBG_MAX,
                          "%3d ", BitSkew[bit]);

          } // bit loop
          RcDebugPrint (SDBG_MAX, " %3d\n", msl);
          //
          // Call SetGDCR to update the appropriate register field
          // Pass a pointer to all bits in the nibble - the Get/Set routine will need them all
          //
          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, faultyStrobe, ALL_BITS, DdrLevel, TxDqBitDelay, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET,
              BitSkew);
        } // nibble loop
      } //ch loop
    } // rank loop
  } // dimm loop
#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    DisplayResults (Host, socket, TxDqDelay);
    DisplayResults (Host, socket, TxDqBitDelay);
  }
#endif // DEBUG_CODE_BLOCK
  IO_Reset (Host, socket);
  FifoTrainReset (Host, socket);

  RcFreePool (ErrorResult);
  return status;

} //WriteDqDqsPerBit

/**

  Initialize Wr DQ/DQs parameters

  @param Host                - Pointer to sysHost
  @param socket                - Socket number

  @retval N/A

**/
STATIC
VOID
WrDqDqsInit (
  PSYSHOST        Host,
  UINT8           Socket
  )
{
  WriteWDBChunkPattern (Host, Socket, MRC_WDB_PATTERN_P, 1);
} // WrDqDqsInit



STATIC
UINT32
FindPerBitResults (
  PSYSHOST        Host,
  UINT8           socket,
  UINT8           dimm,
  UINT8           rank,
  BErrorResult    *ErrorResult
  )
/*++

  This function collects all the data from DqDqs algorithm

  @param Host                - Pointer to sysHost
  @param socket                - Socket number
  @param dimm                - DIMM number
  @param rank                - Rank number
  ErrorResult         - Error result return data

  @retval 0 - successful
  @retval Other - failure

--*/
{
  UINT8               ch;
  UINT32              chBitmask;
  UINT8               bit;
  INT16               piDelay;
  UINT8               piIndex;
  UINT8               strobe;
  UINT8               maxStrobe;
  UINT32              errResult;
  UINT32              status;
  UINT16              txDqs;
  UINT16              txDq;
  UINT8               chStatus;
  UINT32              bwSerr[MAX_CH][3];
  UINT8               DumArr[7] = {1,1,1,1,1,1,1};
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8 MBV = GetMaxBitsValid (Host);
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  status = SUCCESS;
  channelNvList = GetChannelNvList (Host, socket);

  chBitmask = GetChBitmask (Host, socket, dimm, rank);
  errResult = 0;
  chStatus = 0;

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, socket, ch);
    if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT) || (rank >= (*dimmNvList)[dimm].numRanks)) {
      continue;
    }

    //
    // Clear dimm error results
    //
    ZeroMem (ErrorResult[ch].BitErrorResult, (MAX_PHASE_IN_FINE_ADJUSTMENT * 2) * MBV);
  } // ch loop

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                "TxDqDqs Pi Scanning...\n");

  for (piDelay = WR_DQ_PI_START; piDelay < WR_DQ_PI_START + WR_DQ_PI_RANGE; piDelay += WR_DQ_STEP_SIZE) {

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      dimmNvList = GetDimmNvList (Host, socket, ch);
      if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT) || (rank >= (*dimmNvList)[dimm].numRanks)) {
        continue;
      }

      //
      // Set the new Pi.
      //
      maxStrobe = GetMaxStrobe (Host, socket, ch, dimm, WrLvlDelay, DdrLevel);

      for (strobe = 0; strobe < maxStrobe; strobe++) {
        if (IsStrobeNotValid (Host, strobe)) {
          continue;
        }
        GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, (INT16 *)&txDqs);
        txDq = txDqs - TX_DQ_START_OFFSET - TX_PER_BIT_SETTLE_TIME + piDelay;
        GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, TxDqDelay, GSM_UPDATE_CACHE, (INT16 *)&txDq);
        if (!(*dimmNvList)[dimm].x4Present) {
          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe + MSVx8, 0, DdrLevel, TxDqDelay, GSM_UPDATE_CACHE, (INT16 *)&txDq);
        }
      } // strobe loop
    } // ch loop

    //
    // Clear Errors and Run Test
    //
    RunIOTest (Host, socket, chBitmask, BasicVA, DumArr, 1, 0);

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      dimmNvList = GetDimmNvList (Host, socket, ch);
      if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT) || (rank >= (*dimmNvList)[dimm].numRanks)) {
        continue;
      }
      chStatus = 1 << ch;
      bwSerr[ch][0] = 0;
      bwSerr[ch][1] = 0;
      bwSerr[ch][2] = 0;
      // Collect test results
      status = CollectTestResults (Host, socket, chStatus, bwSerr);
#ifdef DEBUG_CODE_BLOCK
      if (GetDebugLevel () & SDBG_MAX) {
        DisplayBwSerr (Host,socket,ch, dimm, rank, bwSerr[ch], piDelay);
      }
#endif // DEBUG_CODE_BLOCK
      for (bit = 0; bit < MBV; bit++) {
        if (IsBitNotValid (Host, bit)) {
          continue;
        }
        if (bit < 32) {
          errResult = bwSerr[ch][0];
        } else if (bit < 64) {
          errResult = bwSerr[ch][1];
        } else  {
          errResult = bwSerr[ch][2];
        }


        for (piIndex = 0; piIndex < WR_DQ_STEP_SIZE; piIndex++) {
          // Any failure is a failure for the bit
          // Assumption for SHL operation: if shift bit above 32, operation will become << (Bit%32) by CPU instruction set
          if (errResult & (1<<bit)) {
            ErrorResult[ch].BitErrorResult[bit][ (piDelay % (MAX_PHASE_IN_FINE_ADJUSTMENT * 2)) + piIndex] = 1;
          } else {
            ErrorResult[ch].BitErrorResult[bit][ (piDelay % (MAX_PHASE_IN_FINE_ADJUSTMENT * 2)) + piIndex] = 0;
          }
        } // piIndex loop
      } // bit loop
    } // ch loop
  } // piDelay loop

  return status;
} // FindPerBitResults
