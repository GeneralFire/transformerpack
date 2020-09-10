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
#include <Library/SystemInfoLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include "Include/BaseMemoryCoreInternal.h"
#include <Library/MemoryCoreTargetLib.h>
#include <Library/SimulationServicesLib.h>
#include <Memory/MemoryCheckpointCodes.h>

#define   RD_DQS_STEP_SIZE  1

#define RD_DQ_DQS_PATTERN  0xA5A5A5A5

//
// Local function prototypes
//
STATIC
UINT32
FindPerBitResults (
  PSYSHOST     Host,
  UINT8        socket,
  UINT8        dimm,
  UINT8        rank,
  BErrorResult *ErrorResult
  );

VOID
RdDqDqsInit (
  PSYSHOST Host,
  UINT8 socket
  );


/**

  @param Host              - Pointer to sysHost
  @param socket            - Socket number
  @param dimm              - DIMM number
  @param rank              - Rank number
  @param ErrorResult       - include the error rwsult for this channel

  @retval Status

**/
STATIC
UINT32
RunMprTest (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     dimm,
  UINT8     rank,
  UINT32    chBitmask,
  UINT32    bwSerr[MAX_CH][3]
  )
{
  UINT8   ch;
  UINT32  status;
  UINT8   DumArr[7] = {1,1,1,1,1,1,1};
  UINT8   MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  status = SUCCESS;

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((chBitmask & (1 << ch)) == 0) {
      continue;
    }

    SelectCPGCRanks (Host, socket, ch, 1 << GetLogicalRank (Host, socket, ch, dimm, rank), 0, 0);
  } // ch loop

  SetupIOTestMPR (Host, socket, chBitmask, 1, 0, 0, 0);
  SetCpgcPatControl (Host, socket, chBitmask, BTBUFFER, BTBUFFER, BTBUFFER, 0, 0, 0);

  IO_Reset (Host, socket);

  //
  // Run the test
  //
  RunIOTest (Host, socket, chBitmask, BasicVA, DumArr, 1, 0);

  // Collect test results
  status = CollectTestResults (Host, socket, chBitmask, bwSerr);

  IO_Reset (Host, socket);

  return status;
}

/**

  Detects DQ swizzling on board routing for NVMDIMM DIMMs

  @param Host  - Pointer to sysHost

  @retval Status

**/
UINT32
DqSwizzleDiscovery (
  PSYSHOST Host
  )
{
  struct rankDevice  (*rankStruct)[MAX_RANK_DIMM];
  UINT8               socket;
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT32              chBitmask;
  UINT32              bwSerr[MAX_CH][3];
  UINT8               lane;
  UINT8               ToggleBit = 0;
  UINT8               strobe;
  UINT8               bit;
  UINT8               b;
  UINT8               mask;
  UINT8               maxStrobe = GetMaxStrobeValid (Host) / 2;
  UINT8               result;
  UINT8               i;
  UINT8               foundSel;
  UINT8               swzSel = 0;
  UINT8               maxLanes = 8;
  UINT8               lanesPerBwSerr = 4;
  UINT16              mprPattern;
  UINT8               simFeedback = 0;
  UINT32              TargetStrobeMask = 0;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8 MBV = GetMaxBitsValid (Host);
  UINT8     MaxChDdr;

  //
  // Return if this socket is disabled
  //
  socket = Host->var.mem.currentSocket;
  MaxChDdr = GetMaxChDdr ();

  if (CheckDqSwizzleSupport (Host, socket) == 0) {
    return SUCCESS;
  }

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_DQ_SWIZZLE_DISCOVERY, (UINT16) ((socket << 8))));

  channelNvList = GetChannelNvList (Host, socket);

  //Clear WDB
  SetPatternBuffers (Host, socket, MEMINIT, 0);

  //
  // Train each DIMM
  //
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    for (rank = 0; rank < 1; rank++) {

      //
      // Initialize to no ranks present
      //
      chBitmask = 0;
      for (ch = 0; ch < MaxChDdr; ch++) {
        rankStruct = GetRankStruct (Host, socket, ch, dimm);

        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }
        dimmNvList = GetDimmNvList (Host, socket, ch);
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT) || (rank >= (*dimmNvList)[dimm].numRanks)) {
          continue;
        }

        // Inidicate this rank is present on at least one channel
        chBitmask |= (1 << ch);

        // Initialize data structure
        for (i = 0; i < 36; i++) {
          (*dimmNvList)[dimm].dqSwz[i] = 0;
        }

        SetMprModeFilterActPre (Host, socket, ch, !(*dimmNvList)[dimm].DcpmmPresent);
#ifdef LRDIMM_SUPPORT
        (*rankStruct)[rank].CurrentDramMode = MR3_MPR;
#endif
      }  // ch loop

      // Go to the next rank if this rank is not present on any channels
      if (!chBitmask) {
        continue;
      }

      //
      // Loop through bits 0-7 selecting one DQ to drive high
      //
      for (lane = 0; lane < 8; lane++) {

        for (ch = 0; ch < MaxChDdr; ch++) {
          if ((chBitmask & (1 << ch)) == 0) {
            continue;
          }

          // for x8 device bits[7:0] map to DQ[0:7]
          // for x4 device bits[7:4] map to DQ[0:3]
          mprPattern = BIT7 >> lane;

          // Use parallel MPR patterns for swizzle detection
          SetRankMPRPattern (Host, socket, ch, dimm, rank, BIT11 | MR3_MPR, mprPattern);
        } // ch loop

        //
        // Run test
        //
        RunMprTest (Host, socket, dimm, rank, chBitmask, bwSerr);

        for (ch = 0; ch < MaxChDdr; ch++) {
          if ((chBitmask & (1 << ch)) == 0) {
            continue;
          }
          rankList = GetRankNvList (Host, socket, ch, dimm);
          dimmNvList = GetDimmNvList (Host, socket, ch);

          //
          // For DCPMM FPT feature, using current test lane to override bwSerr result,
          // it can make system keep booting when link error being detected
          //
          if ((*dimmNvList)[dimm].DcpmmPresent == 1) {
            for(strobe = 0; strobe < MSVx4; strobe++) {
              if (((*rankList)[rank].faultyParts[strobe] & FPT_RD_DQ_DQS_FAILED) != 0) {
                ToggleBit = BIT0 << lane;

                if ((strobe % 9) < 4) {
                  // Override bwserr0 when error happen in strobe 0/1/2/3, 9/10/11/12
                  TargetStrobeMask = (0xFF << ((strobe % 9) * 8));
                  bwSerr[ch][0] &= ~(TargetStrobeMask);
                  bwSerr[ch][0] |= ToggleBit << ((strobe % 9) * 8);
                } else if (((strobe % 9) >= 4) && ((strobe % 9) < 8)) {
                  // Override bwserr1 when error happen in strobe 4/5/6/7, 13/14/15/16
                  TargetStrobeMask = (0xFF << (((strobe - 4) % 9) * 8));
                  bwSerr[ch][1] &= ~(TargetStrobeMask);
                  bwSerr[ch][1] |= ToggleBit << (((strobe - 4) % 9) * 8);
                } else {
                  // override bwserr2 when error happen in strobe 8/17
                  TargetStrobeMask = (0xFF << (((strobe - 8) % 9) * 8));
                  bwSerr[ch][2] &= ~(TargetStrobeMask);
                  bwSerr[ch][2] |= ToggleBit << (((strobe - 8) % 9) * 8);
                }
              }
            }
          }

          if ((*dimmNvList)[dimm].x4Present && ((*dimmNvList)[dimm].DcpmmPresent == 0)) {
            if (lane >= 4) {
              continue;
            } else{
              maxLanes = 4;
              maxStrobe = MSVx4;
              lanesPerBwSerr = 8;
              mask = 0xF;
            }
          } else {
            maxLanes = 8;
            maxStrobe = MSVx8;
            lanesPerBwSerr = 4;
            mask = 0xFF;
          }

          // Simics workaround starts
          if (GetEmulation () & SIMICS_FLAG) {
            simFeedback = 1;
          }
          if (simFeedback) {

            /*
            // HSX test results with SRx4
            Dq Swizzle Discovery -- Started
            Checkpoint: Socket 0, 0xB7, 0x1E, 0x0000
            N0.C0.R0:  0 ...##... ...##... ...##... ...##... ...##... ...##... ...##... ...##... ...##...
            N0.C0.R0:  1 #......# #......# #......# #......# #......# #......# #......# #......# #......#
            N0.C0.R0:  2 ..#..#.. ..#..#.. ..#..#.. ..#..#.. ..#..#.. ..#..#.. ..#..#.. ..#..#.. ..#..#..
            N0.C0.R0:  3 .#....#. .#....#. .#....#. .#....#. .#....#. .#....#. .#....#. .#....#. .#....#.
            DQ lanes:     000000000011111111112222222222333333333344444444445555555555666666666677
                          012345678901234567890123456789012345678901234567890123456789012345678901
            N0.C0.D0.R0:  302103123021031230210312302103123021031230210312302103123021031230210312
            Dq Swizzle Discovery - 69ms
            */
            if (maxLanes <= 4) {
              UINT32 pattern = 0;
              switch (lane) {
              case 0:
                pattern = 0x18181818;
                break;
              case 1:
                pattern = 0x81818181;
                break;
              case 2:
                pattern = 0x24242424;
                break;
              case 3:
                pattern = 0x42424242;
                break;
                default:
                //Automated add of default case. Please review.
                break;
              }
              bwSerr[ch][0] = pattern;
              bwSerr[ch][1] = pattern;
              bwSerr[ch][2] = (UINT8) pattern;
            } else {
              bwSerr[ch][0] = 0x01010101 << lane;
              bwSerr[ch][1] = 0x01010101 << lane;
              bwSerr[ch][2] = 0x01 << lane;
            }

            //Simics workaround ends
          }
#ifdef DEBUG_CODE_BLOCK
          if (GetDebugLevel () & SDBG_MAX) {
            DisplayBwSerr (Host, socket, ch, dimm, (*rankList)[rank].rankIndex, bwSerr[ch], lane);
          }
#endif //DEBUG_CODE_BLOCK
          // record passing lane - 3 bits per target lane
          // bits[1:0] are DQ swizzle within the nibble
          // bit[2] is nibble swap, if applicable
          for (strobe = 0; strobe < maxStrobe; strobe++) {


            result = (bwSerr[ch][strobe / lanesPerBwSerr] >> ((strobe % lanesPerBwSerr) * maxLanes)) & mask;
            foundSel = 0;
            for (i = 0; i < maxLanes; i++) {
              if (result & (1 << i)) {
                if (!foundSel) {
                  swzSel = i;
                  foundSel = 1;
                } else {
                  // Flag error if more than one DQ lane = 1 per strobe
                  Host->nvram.mem.socket[socket].FaultyPartsFlag[ch] |= ~0;
                  rankList = GetRankNvList (Host, socket, ch, dimm);
                  (*rankList)[rank].faultyParts[0] |= FPT_TRAINING_FAILED;
                  RcDebugPrint (SDBG_MAX,
                         "More than one DQ lane = 1: 0x%x!\n", result);
                  EwlOutputType10 (WARN_FPT_CORRECTABLE_ERROR, WARN_DQ_SWIZZLE_DISC_UNCORR, socket, ch, NO_DIMM, NO_RANK, EwlSeverityWarning, (UINT8) bwSerr[ch][strobe / lanesPerBwSerr], foundSel, maxLanes, strobe);
                  DisableChannelSw (Host, socket, ch);
                }
              }
            } // i loop

            if (!foundSel) {
              // Flag error if no DQ lane = 1 per byte
              Host->nvram.mem.socket[socket].FaultyPartsFlag[ch] |= ~0;
              rankList = GetRankNvList (Host, socket, ch, dimm);
              (*rankList)[rank].faultyParts[0] |= FPT_TRAINING_FAILED;
              RcDebugPrint (SDBG_MAX,
                     "No DQ lanes = 1!\n");
              EwlOutputType10 (WARN_FPT_CORRECTABLE_ERROR, WARN_DQ_SWIZZLE_DISC_UNCORR, socket, ch, NO_DIMM, NO_RANK, EwlSeverityWarning, (UINT8) bwSerr[ch][strobe / lanesPerBwSerr], foundSel, maxLanes, strobe);
              DisableChannelSw (Host, socket, ch);
            }

            // Save result for later use
            (*dimmNvList)[dimm].dqSwz[ (strobe * maxLanes + lane) / 2] |= (swzSel << ((lane % 2) * 4));

          } // strobe loop
        } // ch loop
      } // lane loop

      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((chBitmask & (1 << ch)) == 0) {
          continue;
        }
        // Disable MPR mode
        rankStruct = GetRankStruct (Host, socket, ch, dimm);
        dimmNvList = GetDimmNvList (Host, socket, ch);
        SetRankMPR (Host, socket, ch, dimm, rank, 0);

        if ((*dimmNvList)[dimm].DcpmmPresent == 1) {
          SetRankDAMprFnv (Host, socket, ch, dimm, 0);
          SetRankDMprFnv (Host, socket, ch, dimm, 0);
        }

        SetMprModeFilterActPre (Host, socket, ch, 0);
#ifdef LRDIMM_SUPPORT
        (*rankStruct)[rank].CurrentDramMode = 0;
#endif

      } // ch loop
    } // rank loop
  } // dimm loop
    RcDebugPrint (SDBG_MEM_TRAIN,
                  "\n\n");
    RcDebugPrint (SDBG_MEM_TRAIN,
                  "START_DQ_SWIZZLE \n");


  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    dimmNvList = GetDimmNvList (Host, socket, ch);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                       "");
        for (bit = 0; bit < MBV; bit++) {
          if ((bit == 0) || (bit == (MBV / 2))) {
            RcDebugPrint (SDBG_MEM_TRAIN,
                           "\n");
            if (bit == (MBV / 2)) {
              RcDebugPrint (SDBG_MEM_TRAIN,
                             "\n");
            }
            for (b = bit; b < (bit + (MBV / 2)); b++) {
              RcDebugPrint (SDBG_MEM_TRAIN,
                             "  %2d", b);
            }
            RcDebugPrint (SDBG_MEM_TRAIN,
                        "\n");
          }
          RcDebugPrint (SDBG_MEM_TRAIN,
                         " %3d", ((*dimmNvList)[dimm].dqSwz[bit / 2] >> ((bit % 2) * 4)) & 7);

        } // bit loop

        RcDebugPrint (SDBG_MEM_TRAIN,
                    "\n");
      } // rank loop
    } // dimm loop
  } // ch loop


  RcDebugPrint (SDBG_MAX,"\n\n");

  FifoTrainReset (Host, socket);

  return SUCCESS;
} // DqSwizzleDiscovery

/**

  Discover DQS layout swizzle for x16 DIMMs

  @param[in] Host - Pointer to sysHost, the system host (root) structure

  @retval SUCCESS

**/
UINT32
DqsSwizzleDiscoveryx16 (
  IN  PSYSHOST  Host
  )
{
#ifndef DDR5_SUPPORT
  UINT8               Socket;
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               Dram;
  UINT8               Strobe;
  UINT8               Index;
  UINT8               MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8               BwSerrSize = 4;
  UINT16              MprPattern;
  UINT32              ChBitmask;
  UINT32              BwSerr[MAX_CH][3];
  UINT32              TargetDramMask = 0;
  UINT32              TargetBwSerr[3];
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  struct rankDevice   (*RankStruct)[MAX_RANK_DIMM];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Socket = Host->var.mem.currentSocket;
  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // Return success if x16 DRAM is not present
  //
  if ((Host->nvram.mem.socket[Socket].x16DimmPresent == FALSE)) {
    return SUCCESS;
  }

  //
  // Exit if this training step should not be executed
  //
  if (!IsMemFlowEnabled (DqSwizzlingDiscoveryX16) ||
      (FeaturePcdGet (PcdCosimBuild) == TRUE) ||
      (FeaturePcdGet (PcdCteBuild) == TRUE)) {

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      for (Ch = 0; Ch < MaxChDdr; Ch++) {
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        //
        // Initial PairedLogic to 0xFF and IsLow variable to 0
        //
        for (Strobe = 0; Strobe < MSVx8; Strobe++) {
          (*DimmNvList)[Dimm].LogicalX16Mapping.PairLogical[Strobe] = 0xFF;
          (*DimmNvList)[Dimm].LogicalX16Mapping.IsLow[Strobe] = 0;
        }
      }  // Ch loop
    }
    return SUCCESS;
  }

  RcDebugPrint (SDBG_MINMAX,
                  "START_DQS_SWIZZLE_Discovery_x16 \n");

  //
  // Clear WDB
  //
  SetPatternBuffers (Host, Socket, MEMINIT, 0);

  //
  // Train each DIMM
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    ChBitmask = 0;
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }

      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      ChBitmask |= (1 << Ch);

      //
      // Initial PairedLogic and IsLow variable to 0xFF
      //
      for (Strobe = 0; Strobe < MSVx8; Strobe++) {
        (*DimmNvList)[Dimm].LogicalX16Mapping.PairLogical[Strobe] = 0xFF;
        (*DimmNvList)[Dimm].LogicalX16Mapping.IsLow[Strobe] = 0xFF;
      }
    }  // Ch loop

    for (Dram = 0; Dram < MSVx8; Dram += 1) {
      if (IsStrobeNotValid (Host, Dram)) {
        continue;
      }
      //
      // Initial MPR Pattern and fill same pattern into WDB
      //
      if ((Dram & BIT0) != BIT0) {
        MprPattern = 0xFF;
        SetPatternBuffersX16Dqs (Host, Socket, TRUE);
      } else {
        MprPattern = 0;
        SetPatternBuffersX16Dqs (Host, Socket, FALSE);
      }

      //
      // Enter MPR with parallel MPR patterns for x16 DQS Swizzle Discover
      // Invert MPR pattern to induce failures in all CPGC error registers, this should clear CPGC logic
      //
      for (Ch = 0; Ch < MaxChDdr; Ch++) {
        if ((ChBitmask & (1 << Ch)) == 0) {
          continue;
        }
        SetRankMPRPattern (Host, Socket, Ch, Dimm, 0, BIT11 | MR3_MPR, ~MprPattern);
      }

      //
      // Do a full test again to clear HW state in CPGC logic
      //
      RunMprTest (Host, Socket, Dimm, 0, ChBitmask, BwSerr);
      //
      // Exit MPR
      //
      for (Ch = 0; Ch < MaxChDdr; Ch++) {
        if ((ChBitmask & (1 << Ch)) == 0) {
          continue;
        }
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);

        SetRankMPR (Host, Socket, Ch, Dimm, 0, 0);
      }

      for (Ch = 0; Ch < MaxChDdr; Ch++) {
        if ((ChBitmask & (1 << Ch)) == 0) {
          continue;
        }

        RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);

        //
        // Disable Qoff on non-target Drams
        //
        for (Strobe = 0; Strobe < MSVx8; Strobe++) {
          if (IsStrobeNotValid (Host, Strobe)) {
            continue;
          }
          if (Strobe != Dram) {
            WriteMRSPDA (Host, Socket, Ch, Dimm, 0, Strobe, (*RankStruct)[0].MR1 | MR1_QOFF, BANK1);
          } else {
            WriteMRSPDA (Host, Socket, Ch, Dimm, 0, Strobe, (*RankStruct)[0].MR1 & ~MR1_QOFF, BANK1);
          }
        } // Strobe loop

        //
        // Enter MPR with parallel MPR patterns for x16 DQS Swizzle Discover
        //
        SetRankMPRPattern (Host, Socket, Ch, Dimm, 0, BIT11 | MR3_MPR, MprPattern);

        //
        // Clean error
        //
        BwSerr[Ch][0] = 0;
        BwSerr[Ch][1] = 0;
        BwSerr[Ch][2] = 0;
      } // Ch loop

      //
      // Run test
      //
      RunMprTest (Host, Socket, Dimm, 0, ChBitmask, BwSerr);
      //
      // Override the results depending on the target
      //
      OverrideX16SwizzleTarget (Dram, BwSerr);

      for (Ch = 0; Ch < MaxChDdr; Ch++) {
        if ((ChBitmask & (1 << Ch)) == 0) {
          continue;
        }

        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);

        DisplayBwSerr (Host, Socket, Ch, Dimm, (*RankList)[0].rankIndex, BwSerr[Ch], Dram);

        //
        // Parsing test result
        //
        TargetDramMask = 0xFF << ((Dram % 4) * 8);

        TargetBwSerr[0] = BwSerr[Ch][0];
        TargetBwSerr[1] = BwSerr[Ch][1];
        TargetBwSerr[2] = BwSerr[Ch][2] | 0xFFFFFF00; // Only 8 bit is valid for ECC

        if (Host->nvram.mem.eccEn == 0) {
          TargetBwSerr[2] = 0xFFFFFFFF;
        }

        if ((TargetBwSerr[0] != 0xFFFFFFFF) || (TargetBwSerr[1] != 0xFFFFFFFF) || (TargetBwSerr[2] != 0xFFFFFFFF)) {
          if (Dram < BwSerrSize) {
            TargetBwSerr[0] |= TargetDramMask;
          } else if (Dram < (BwSerrSize * 2)) {
            TargetBwSerr[1] |= TargetDramMask;
          } else {
            TargetBwSerr[2] |= TargetDramMask;
          }

          //
          // PairedLogic: Indicate paired logic strobe, 0xFF - no paired logic strobe
          // IsLow: Set to 1 means Effective DQS_L which connected to Low side
          //
          (*DimmNvList)[Dimm].LogicalX16Mapping.IsLow[Dram] = TRUE;
          for (Index = 0; Index < BwSerrSize; Index++) {
            //
            // Test BIT0/8/16/24 to gather effective DQS
            //
            if ((TargetBwSerr[0] & (BIT0 << ((Index % 4) * 8))) == 0) {
              //
              // Gather pass result from strobe [3:0]
              //
              (*DimmNvList)[Dimm].LogicalX16Mapping.PairLogical[Dram] = Index;
              (*DimmNvList)[Dimm].LogicalX16Mapping.PairLogical[Index] = Dram;
              (*DimmNvList)[Dimm].LogicalX16Mapping.IsLow[Index] = FALSE;
            } else if ((TargetBwSerr[1] & (BIT0 << ((Index % 4) * 8))) == 0) {
              //
              // Gather pass result from strobe [7:4]
              //
              (*DimmNvList)[Dimm].LogicalX16Mapping.PairLogical[Dram] = Index + 4;
              (*DimmNvList)[Dimm].LogicalX16Mapping.PairLogical[Index + 4] = Dram;
              (*DimmNvList)[Dimm].LogicalX16Mapping.IsLow[Index + 4] = FALSE;
            } else if ((TargetBwSerr[2] & (BIT0 << ((Index % 4) * 8))) == 0) {
              //
              // Gather pass result from strobe [8]
              //
              (*DimmNvList)[Dimm].LogicalX16Mapping.PairLogical[Dram] = Index + 8;
              (*DimmNvList)[Dimm].LogicalX16Mapping.PairLogical[Index + 8] = Dram;
              (*DimmNvList)[Dimm].LogicalX16Mapping.IsLow[Index + 8] = FALSE;
            }
          }
        }
      } // Ch loop

      //
      // Clear MR1 Qoff
      //
      for (Ch = 0; Ch < MaxChDdr; Ch++) {
        if ((ChBitmask & (1 << Ch)) == 0) {
          continue;
        }

        RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);
        WriteMRS (Host, Socket, Ch, Dimm, 0, (*RankStruct)[0].MR1 & ~MR1_QOFF, BANK1);
      }
      //
      // Exit MPR
      //
      for (Ch = 0; Ch < MaxChDdr; Ch++) {
        if ((ChBitmask & (1 << Ch)) == 0) {
          continue;
        }

        RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);
        SetRankMPR (Host, Socket, Ch, Dimm, 0, 0);
      }
    } // Dram loop

    //
    // Print result
    //
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((ChBitmask & (1 << Ch)) == 0) {
        continue;
      }

      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      for (Strobe = 0; Strobe < MSVx8; Strobe++) {
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "PairedLogic[%x] = %x \n", Strobe, (*DimmNvList)[Dimm].LogicalX16Mapping.PairLogical[Strobe]);
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "IsLow[%x] = %x \n", Strobe, (*DimmNvList)[Dimm].LogicalX16Mapping.IsLow[Strobe]);
      }
    }
  } //Dimm loop

  RcDebugPrint (SDBG_MINMAX,
                "STOP_DQS_SWIZZLE_Discovery_x16 \n");

  FifoTrainReset (Host, Socket);
#endif // !DDR5_SUPPORT

  return SUCCESS;
} // DqsSwizzleDiscoveryx16

UINT32
ReadDqDqsPerBit (
  PSYSHOST Host
  )
{
  struct rankDevice  (*rankStruct)[MAX_RANK_DIMM];
  UINT8               socket;
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               nibble;
  UINT8               faultyStrobe;
  UINT8               bit;
  UINT8               bitIdx;
  UINT8               msl;
  INT16               BitSkew[BITS_PER_NIBBLE];
  INT16               bitNibbleMin;
  INT16               bitSkewMax;
  UINT16              bitCenter[8];
  UINT8               rankPresent;
  PerBitPi            errorStart[MAX_CH];
  PerBitPi            errorEnd[MAX_CH];
  PerBitPi            piPosition[MAX_CH];
  UINT16              worstLeft;
  UINT16              worstRight;
  UINT32              status;
  UINT8               faultyPartsStatus;
  struct bitMarginCh  resultsBit;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  BErrorResult        (*ErrorResult)[MAX_CH];

  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MBV = GetMaxBitsValid (Host);
  UINT8 MaxChDdr;

  if (!IsMemFlowEnabled (ReadDqDqsTraining)) {
    return SUCCESS;
  }

  status = SUCCESS;
  MaxChDdr = GetMaxChDdr ();
  socket = Host->var.mem.currentSocket;

  ApplyRxDqsSafeSettings (socket);

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
  RdDqDqsInit (Host, socket);

  channelNvList = GetChannelNvList (Host, socket);

  // Initialize LE and RE Margin results structure
  for (ch = 0; ch < MaxChDdr; ch++) {
    for (bit = 0; bit < MBV; bit++) {
      resultsBit.bits[ch][bit].n = 0;
      resultsBit.bits[ch][bit].p = 0;
    }
  } // ch loop

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
        rankStruct = GetRankStruct (Host, socket, ch, dimm);
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

        SetRankMPR (Host, socket, ch, dimm, rank, MR3_MPR);
#ifdef LRDIMM_SUPPORT
        (*rankStruct)[rank].CurrentDramMode = MR3_MPR;
#endif

        //
        // Tell the iMC not to issue activates and precharges
        //
        SetMprModeFilterActPre (Host, socket, ch, !(*dimmNvList)[dimm].DcpmmPresent);
      } // ch loop

      //
      // Go to the next rank if this rank is not present on any channels
      //
      if (!rankPresent) {
        continue;
      }

      //
      // Data collecting
      //
      status = FindPerBitResults (Host, socket, dimm, rank, *ErrorResult);

      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }

        rankList = GetRankNvList (Host, socket, ch, dimm);
        rankStruct = GetRankStruct (Host, socket, ch, dimm);
        dimmNvList = GetDimmNvList (Host, socket, ch);
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT) || (rank >= (*dimmNvList)[dimm].numRanks)) {
          continue;
        }

        SetMprModeFilterActPre (Host, socket, ch, 0);
        SetRankMPR (Host, socket, ch, dimm, rank, 0);
#ifdef LRDIMM_SUPPORT
        (*rankStruct)[rank].CurrentDramMode = 0;
#endif

#ifdef DEBUG_CODE_BLOCK
        //Print Margins Here
#endif // DEBUG_CODE_BLOCK

        worstLeft = 0xFF;
        worstRight = 0xFF;

        for (bit = 0; bit < MBV; bit++) {

          nibble = bit / 4;
          if (nibble % 2) {
            faultyStrobe = (nibble + 17) / 2;
          } else {
            faultyStrobe = nibble / 2;
          }

          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if (IsBitNotValid (Host, bit)) {
            continue;
          }

          status = FindErrorGraphMin (Host, socket, ch, dimm, rank, 0, &(*ErrorResult)[ch].BitErrorResult[bit][0],
                     MEM_CHIP_POLICY_VALUE (Host, MaxPhaseInReadAdjustmentDq), &errorStart[ch].PiPosition[bit],
                     &errorEnd[ch].PiPosition[bit], &piPosition[ch].PiPosition[bit]);

          //LE and RE Margins
          resultsBit.bits[ch][bit].n = errorEnd[ch].PiPosition[bit];
          resultsBit.bits[ch][bit].p = errorStart[ch].PiPosition[bit];

          if (status == FAILURE) {

            //
            // Mark this part as faulty
            //
            Host->nvram.mem.socket[socket].FaultyPartsFlag[ch] |= (1 << faultyStrobe);

            (*rankList)[rank].faultyParts[faultyStrobe] |= FPT_RD_DQ_DQS_FAILED;

            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, faultyStrobe, bit,
                           "Failed Read DqDqsPerBit\n");

            faultyPartsStatus = EvaluateFaultyParts (Host, socket, ch);

            switch (faultyPartsStatus) {
            case FPT_NO_ERROR:
              break;
            case FPT_CORRECTABLE_ERROR:
              // if correctable  log the warning -> OutputWarning
              // careful for cases when warning gets promoted to error !!!  - log the strobe
              RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, rank, faultyStrobe, NO_BIT,
                             "RdDqDqs Correctable\n");
              EwlOutputType2 (WARN_FPT_CORRECTABLE_ERROR, WARN_FPT_MINOR_RD_DQ_DQS, socket, ch, dimm, rank, EwlSeverityWarning, 0xFF, bit, RxDqsBitDelay, DdrLevel, (UINT8) (resultsBit.bits[ch][bit].p - resultsBit.bits[ch][bit].n));
              status = SUCCESS;
              break;
            default:
              // does anything need to be done here?
              RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                             "RdDqDqs training failure!!!\n");
              //
              // Add this error to the warning log for both correctable and uncorrectable errors.
              //
              DisableChannelSw (Host, socket, ch);
              EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_RD_DQ_DQS, socket, ch, dimm, rank, EwlSeverityWarning, 0xFF, bit, RxDqsBitDelay, DdrLevel, (UINT8) (resultsBit.bits[ch][bit].p - resultsBit.bits[ch][bit].n));
              status = FAILURE;
              break;
            }
          } // if FAILURE


          if ((piPosition[ch].PiPosition[bit] - errorEnd[ch].PiPosition[bit]) < worstLeft) {
            worstLeft = piPosition[ch].PiPosition[bit] - errorEnd[ch].PiPosition[bit];
          }
          if ((errorStart[ch].PiPosition[bit] - piPosition[ch].PiPosition[bit]) < worstRight) {
            worstRight = errorStart[ch].PiPosition[bit] - piPosition[ch].PiPosition[bit];
          }

          // //
          // // BIOS writes the calculated values of PI setting
          // //
          // GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, rank, strobe, 0, DdrLevel, RxDqsDelay, GSM_UPDATE_CACHE, (INT16 *)&piPosition[ch][strobe]);
        } // bit loop

#ifdef DEBUG_CODE_BLOCK
        //Print LE and RE Per Bit Deskew Margins for current rank
        if (GetDebugLevel () & SDBG_MAX) {
          DisplayREandLEMargins (Host, socket, ch, dimm, rank, &resultsBit, RxDqsDelay);
        }
#endif // DEBUG_CODE_BLOCK
        DetectLowMargin (Host, ch, dimm, rank, &resultsBit, RxDqsDelay, NON_PER_BIT);

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
          //
          if (IsNibbleNotValid (Host, nibble)) {
            continue;
          }
          faultyStrobe = GetStrobeFromNibble (Host, nibble);

          bitNibbleMin = 127;   // Set to largest possible
          RcDebugPrint (SDBG_MAX,
                      "%2d ", nibble);

          // Loop for all the bits in this nibble
          for (bit = 0; bit < BITS_PER_NIBBLE; bit++) {
            // Get the absolute bit index (0-72)
            bitIdx = (nibble * BITS_PER_NIBBLE) + bit;

            // Find the center for this bit
            bitCenter[bit] = piPosition[ch].PiPosition[bitIdx];

            RcDebugPrint (SDBG_MAX,
                          "%3d ", bitCenter[bit]);

            // Check if the center of this bit is smaller than the smallest found so far
            if (bitCenter[bit] < bitNibbleMin) {
              bitNibbleMin = bitCenter[bit];
            }
          } // for (bit)

          RcDebugPrint (SDBG_MAX,
                        "  %3d   ", bitNibbleMin);
          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, faultyStrobe, 0, DdrLevel, RxDqDqsDelay, GSM_UPDATE_CACHE, &bitNibbleMin);
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

          } // for (bit)
          RcDebugPrint (SDBG_MAX, " %3d\n", msl);
          //
          // Update the register
          // Pass a pointer to all bits in the nibble - the Get/Set routine will need them all
          //
          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, faultyStrobe, ALL_BITS, DdrLevel, RxDqsBitDelay, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET,
                           BitSkew);
        } // for nibble
      } // ch loop
    } // rank loop
  } // dimm loop

  FifoTrainReset (Host, socket);
#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    DisplayResults (Host, socket, RxDqDqsDelay);
    DisplayResults (Host, socket, RxDqDelay);
    DisplayResults (Host, socket, RxDqsPDelay);
    DisplayResults (Host, socket, RxDqsNDelay);
    DisplayResults (Host, socket, RxDqsBitDelay);
  }
#endif // DEBUG_CODE_BLOCK
  RcDebugPrint (SDBG_MAX,"\n");

  ReadDqDqsCleanup (Host, socket);

  RcFreePool (ErrorResult);
  return status;

} // ReadDqDqsPerBit



STATIC
UINT32
FindPerBitResults (
  PSYSHOST     Host,
  UINT8        socket,
  UINT8        dimm,
  UINT8        rank,
  BErrorResult *ErrorResult
  )
{
  UINT8               ch;
  UINT32              chBitmask;
  UINT8               bit;
  UINT16               piDelay;
  UINT8               piIndex;
  UINT32              errResult;
  UINT8               chStatus;
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct channelNvram (*channelNvList)[MAX_CH];
  UINT8               DumArr[7] = {1,1,1,1,1,1,1};
  UINT32              bwSerr[MAX_CH][3];
  UINT8 MBV = GetMaxBitsValid (Host);
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
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

    SelectCPGCRanks (Host, socket, ch, 1 << GetLogicalRank (Host, socket, ch, dimm, rank), 0, 0);

    //
    // Clear dimm error results
    //
    for (bit = 0; bit < MBV; bit++) {
      for (piDelay = 0; piDelay < MEM_CHIP_POLICY_VALUE (Host, MaxPhaseInReadAdjustmentDq); piDelay++) {
        ErrorResult[ch].BitErrorResult[bit][piDelay] = 0;
      } //piDelay loop
    } // bit loop
  } // ch loop

  SetupIOTestMPR (Host, socket, chBitmask, 1, 0, 0, 0);
  SetCpgcPatControl (Host, socket, chBitmask, BTBUFFER, BTBUFFER, BTBUFFER, 0xAAAAAA, 0xCCCCCC, 0xF0F0F0);

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                "RxDqDqs Pi Scanning...\n");
  for (piDelay = 0; piDelay < MEM_CHIP_POLICY_VALUE (Host, MaxPhaseInReadAdjustmentDq); piDelay += RD_DQS_STEP_SIZE) {

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      dimmNvList = GetDimmNvList (Host, socket, ch);
      if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT) || (rank >= (*dimmNvList)[dimm].numRanks)) {
        continue;
      }

      //
      // Change the RX Dq Dqs PI Setting
      //
      GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, 0, DdrLevel, RxDqDqsDelay, GSM_UPDATE_CACHE, &piDelay);

    } // ch loop

    IO_Reset (Host, socket);

    //
    // Run the test
    //
    RunIOTest (Host, socket, chBitmask, BasicVA, DumArr, 1, 0);

    //
    // Check the results
    //
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
      CollectTestResults (Host, socket, chStatus, bwSerr);
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

        for (piIndex = 0; piIndex < RD_DQS_STEP_SIZE; piIndex++) {
          //Ensure the step size does not go beyond the range of the array
          if ((piDelay + piIndex) > MEM_CHIP_POLICY_VALUE (Host, MaxPhaseInReadAdjustmentDq)) {
            break;
          }
          // Any failure is a failure for the strobe
          // Assumption for SHL operation: if shift bit above 32, operation will become << (Bit%32) by CPU instruction set
          if (errResult & (1<<bit)) {
            ErrorResult[ch].BitErrorResult[bit][piDelay + piIndex] = 1;
          } else {
            ErrorResult[ch].BitErrorResult[bit][piDelay + piIndex] = 0;
          }
        } // piIndex loop
      } // bit loop
    } // ch loop

    IO_Reset (Host, socket);
  } // piDelay loop

  return SUCCESS;
}

/**

  The function gets number of error samples and finds the middle of zero error

  @param Host              - Pointer to sysHost
  @param socket              - Socket number
  @param ch                - Channel number
  @param dimm              - DIMM number
  @param rank              - Rank number
  @param strobe
  @param SdRamErrorSamples - Array with 64 sampes for each dqs pi setting
  @param ErrorSamplesSize  - Include how many sample we have in the array 72 or 64
  @param errorStart        - Pointer to location for error start
  @param errorEnd          - Pointer to location for error end

  @retval status

**/
UINT32
FindErrorGraphMin (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     *SdRamErrorSamples,
  UINT16    ErrorSamplesSize,
  UINT16    *errorStart,
  UINT16    *errorEnd,
  UINT16    *piPosition
  )
{
  UINT16  ZeroSize;
  UINT16  i;
  UINT16  TempErrorEnd;
  UINT16  TempErrorStart;
  UINT32  status;

  TempErrorEnd    = ErrorSamplesSize;
  TempErrorStart  = ErrorSamplesSize;
  status          = SUCCESS;

  *errorEnd       = ErrorSamplesSize;
  *errorStart     = ErrorSamplesSize;

  //
  // If the first sample passed and the last sample failed
  //
  if (SdRamErrorSamples[0] == 0) {
    TempErrorEnd = 0;
  }
  //
  // Loop through each sample
  //
  for (i = 1; i < ErrorSamplesSize; i++) {
    //
    // Find the place of error start
    //
    if (TempErrorEnd == ErrorSamplesSize) {
      //
      // If this sample passed and the previous sample failed
      //
      if ((SdRamErrorSamples[i] == 0) && (SdRamErrorSamples[i - 1])) {
        TempErrorEnd = i;

        //
        // Check if this is the last sample and it passed
        //
        if (i == (ErrorSamplesSize - 1)) {
          TempErrorStart = i;

          //
          // Update error start and error end
          //
          if ((*errorStart - *errorEnd) < (TempErrorStart - TempErrorEnd)) {
            *errorEnd   = TempErrorEnd;
            *errorStart = TempErrorStart;

            //
            // Clean the temp variable
            //
            TempErrorEnd   = ErrorSamplesSize;
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
      if (SdRamErrorSamples[i] && (SdRamErrorSamples[i - 1] == 0)) {
        //
        // We found the first error sample
        //
        TempErrorStart = (i - 1);

        //
        // Update error start and error end
        //
        if ((*errorStart - *errorEnd) <= (TempErrorStart - TempErrorEnd)) {
          *errorEnd   = TempErrorEnd;
          *errorStart = TempErrorStart;
        }

        //
        // Clean the temp variable
        //
        TempErrorEnd   = ErrorSamplesSize;
        TempErrorStart = ErrorSamplesSize;
      }
      //
      // Check if this is the last sample and it passed
      //
      if ((i == (ErrorSamplesSize - 1)) && (SdRamErrorSamples[i] == 0) && (SdRamErrorSamples[0])) {
        //
        // We found the first error sample
        //
        TempErrorStart = i;

        //
        // Update error start and error end
        //
        if ((*errorStart - *errorEnd) < (TempErrorStart - TempErrorEnd)) {
          *errorEnd   = TempErrorEnd;
          *errorStart = TempErrorStart;

          //
          // Clean the temp variable
          //
          TempErrorEnd   = ErrorSamplesSize;
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
    if (SdRamErrorSamples[0]) {
      TempErrorStart = (ErrorSamplesSize - 1);
      //
      // the case is like the next: EEEEEEEEEEEE##############
      //
    } else {
      for (i = 1; i < ErrorSamplesSize; i++) {
        //
        // If this sample failed and the previous sample passed
        //
        if ((SdRamErrorSamples[i]) && (SdRamErrorSamples[i - 1] == 0)) {
          TempErrorStart = (i - 1);
          //
          // Break because we found the start of the errors
          //
          break;
        }
      } // ErrorSamplesSize loop
    }

    if ((*errorStart - *errorEnd) < ((TempErrorStart + ErrorSamplesSize) - TempErrorEnd)) {
      *errorEnd   = TempErrorEnd;
      *errorStart = TempErrorStart;
    }
  }
  if (FAULTY_PARTS_TRACKING_INJECT_2ND_ERROR == 1) {
    if ((ch == 0) && (rank == 0) && (strobe == 7)) {
      *errorEnd = *errorStart;
    }
  } // FAULTY_PARTS_TRACKING_INJECT_2ND_ERROR

  //
  // Result correction checking and find the Pi position
  //
  if (*errorEnd == ErrorSamplesSize || *errorStart == ErrorSamplesSize || *errorEnd == *errorStart) {

    status = FAILURE;
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                   "Dq/Dqs Pi not found. Using default\n");
    *piPosition = 36;
    *errorStart = 42;
    *errorEnd   = 30;
  } else {
    if (*errorStart < *errorEnd) {
      ZeroSize    = ((ErrorSamplesSize - (UINT8)*errorEnd) + (UINT8)*errorStart);
      *piPosition = (*errorEnd + (ZeroSize / 2)) % ErrorSamplesSize;
    } else {
      ZeroSize    = (UINT8) (*errorStart - *errorEnd);
      *piPosition = *errorEnd + (ZeroSize / 2);
    }
  }

  return status;
}

/**

  Initialize the Rd DqDqs operation

  @param Host  - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/
VOID
RdDqDqsInit (
  PSYSHOST Host,
  UINT8    socket
  )
{
  //
  // Fill 1 cacheline of the WDB with a clock pattern
  //
  WriteWDBDqLanePattern (Host, socket, RD_DQ_DQS_PATTERN, 1, FALSE);
} // RdDqDqsInit

#ifdef DEBUG_CODE_BLOCK

/**

  Display a summary of the results

  @param Host            - Pointer to sysHost
  @param socket            - Socket number
  @param ch              - Channel number
  @param dimm            - DIMM number
  @param rank            - Rank number
  @param ErrorResult     - Array that include that data to display
  @param PiMaxNumber     - Array size. It can be 128 or 72 pi
  @param ReadMode        - if ReadMode is:
                             0 = "Read DQ/DQS"
                             1 = "Read Vref"
                             2 = "Write DQ/DQS"
                             3 = "Write Vref"
  @param AggressiveMode  - If true that caller is aggressive training

  @retval N/A

**/
VOID
DqDqsDebugDataSummarization (
  PSYSHOST     Host,
  UINT8        socket,
  UINT8        ch,
  UINT8        dimm,
  UINT8        rank,
  TErrorResult *ErrorResult,
  UINT8        PiMaxNumber,
  UINT8        ReadMode,
  UINT8        AggressiveMode
  )
{
  UINT8 i;
  UINT8 strobe;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);

  AcquirePrintControl ();

  RcDebugPrint (SDBG_DEFAULT, "\n");
  if (AggressiveMode) {
    RcDebugPrint (SDBG_DEFAULT, "Aggressive ");
  }
  if (ReadMode == 0) {
    RcDebugPrint (SDBG_DEFAULT, "Read DQ/DQS ");
  } else if (ReadMode == 1) {
    RcDebugPrint (SDBG_DEFAULT, "Read Vref ");
  } else if (ReadMode == 2) {
    RcDebugPrint (SDBG_DEFAULT, "Write DQ/DQS ");
  } else if (ReadMode == 3) {
    RcDebugPrint (SDBG_DEFAULT, "Write Vref ");
  }

  RcDebugPrint (SDBG_DEFAULT, "summary for socket:%d channel:%d dimm:%d rank:%d\n", socket, ch, dimm, rank);
  RcDebugPrintLine (SDBG_DEFAULT, 85, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  //BRS_TODO
  if (Host->nvram.mem.eccEn) {
    RcDebugPrint (SDBG_DEFAULT, "       0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17\n");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "       0   1   2   3   4   5   6   7   9  10  11  12  13  14  15  16\n");
  }
  for (i = 0; i < PiMaxNumber; i++) {
    RcDebugPrint (SDBG_DEFAULT, "%d ", i);
    if (i < 10) {
      RcDebugPrint (SDBG_DEFAULT, "  ");
    }

    if ((i < 100) && (i > 9)) {
      RcDebugPrint (SDBG_DEFAULT, " ");
    }

    for (strobe = 0; strobe < MSVx4; strobe++) {
      //
      // Skip if this is an ECC strobe when ECC is disabled
      //
      if (IsStrobeNotValid (Host, strobe)) {
        continue;
      }
      RcDebugPrint (SDBG_DEFAULT, "   ");
      if (ErrorResult->DimmErrorResult[strobe][i]) {
        RcDebugPrint (SDBG_DEFAULT, "#");
      } else {
        RcDebugPrint (SDBG_DEFAULT, ".");
      }
    } // strobe loop

    RcDebugPrint (SDBG_DEFAULT, "\n");
  } // i loop

  ReleasePrintControl ();
} // DqDqsDebugDataSummarization

/**

  Display a summary of the results

  @param Host            - Pointer to sysHost
  @param socket            - Socket number
  @param ch              - Channel number

  @retval N/A

**/
VOID
DisplayErrorStartEnd (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT16    piPosition[MAX_STROBE],
  UINT16    errorStart[MAX_STROBE],
  UINT16    errorEnd[MAX_STROBE]
  )
{
  UINT8 strobe;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);

  if (!(GetDebugLevel () & SDBG_MAX)) {
    return;
  }

  AcquirePrintControl ();

  RcDebugPrintLine (SDBG_MAX, 76, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

  // Print error end (passing starts)
  RcDebugPrint (SDBG_DEFAULT, "EE: ");
  for (strobe = 0; strobe < MSVx4; strobe++) {
    if (IsStrobeNotValid (Host, strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, " %3d", errorEnd[strobe]);
  } // strobe loop

  // Print Pi Position (center point)
  RcDebugPrint (SDBG_DEFAULT, "\nPP: ");
  for (strobe = 0; strobe < MSVx4; strobe++) {
    if (IsStrobeNotValid (Host, strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, " %3d", piPosition[strobe]);
  } // strobe loop

  // Print error start (passing stops)
  RcDebugPrint (SDBG_DEFAULT, "\nFE: ");
  for (strobe = 0; strobe < MSVx4; strobe++) {
    if (IsStrobeNotValid (Host, strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, " %3d", errorStart[strobe]);
  } // strobe loop

  RcDebugPrint (SDBG_DEFAULT, "\n\n");

  ReleasePrintControl ();
} // DisplayErrorStartEnd
#endif //   DEBUG_CODE_BLOCK

/**

  Get's the resutls from the current test

  @param Host      - Pointer to sysHost
  @param dimm      - DIMM number
  @param rank      - rank number of the DIMM

  @retval N/A

**/
VOID
GetResultsPassFail (
  PSYSHOST      Host,
  UINT8         socket,
  UINT8         dimm,
  UINT8         rank,
  TErrorResult  *ErrorResult,
  UINT16        piDelay
  )
{
  UINT8               ch;
  UINT8               strobe;
  UINT8               status;
  UINT8               step;
  UINT32              feedBack;
  struct channelNvram (*channelNvList)[MAX_CH];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MaxChDdr;

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

    //SKX change for upper strobes in N1 registers
    for (strobe = 0; strobe < MSVx4; strobe++) {
      feedBack = GetDataTrainFeedback (Host, socket, ch, strobe);
      if ((feedBack & 1) == 0) {
        status = 1;
      } else {
        status = 0;
      }

      for (step = 0; step < RD_DQS_STEP_SIZE; step++) {
        ErrorResult[ch].DimmErrorResult[strobe][piDelay + step] = status;
      }

    } // strobe loop
  } // ch loop
} // GetResultsPassFail

