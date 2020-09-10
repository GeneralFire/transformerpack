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
#include <Library/SystemInfoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemRoutingLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/TimerLib.h>
#include <Library/Ddr5CoreLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>

#ifndef INT16_MIN
#define INT16_MIN -32768
#endif

#ifndef INT16_MAX
#define INT16_MAX  32767
#endif
/**

  Display Right Edge and Left Edge margin results for DDR5.

  @param Host          - Pointer to sysHost, the system Host (root) structure
  @param socket        - current socket
  @param ch            - channle number
  @param Subch         - Sub Channel number (0-based)
  @param dimm          - dimm number
  @param rank          - rank number (0-based)
  @param resultsBit    - Per Bit margin results
  @param group         - Command group

  @retval None

**/
VOID
DisplayREandLEMarginsDdr5 (
  IN PSYSHOST            Host,
  IN UINT8               Socket,
  IN UINT8               Ch,
  IN UINT8               SubCh,
  IN UINT8               Dimm,
  IN UINT8               Rank,
  IN struct bitMarginCh  *ResultsBit,
  IN MRC_GT              Group
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8               Bit;
  UINT8               b;
  UINT8               MaxBits;
  struct dimmNvram (*DimmNvList) [MAX_DIMM];
  UINT8 MBV = GetMaxBitsValid (Host);

  if (SubCh != NO_SUBCH) {
    MBV = MAX_BITS_DDR5 / 2;
  }
  //
  // Check the debug message level
  //
  if (!(GetDebugLevel () & SDBG_MEM_TRAIN)) {
    return;
  }

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  //
  // Skip if no DIMM present
  //
  if ((*DimmNvList) [Dimm].dimmPresent) {

    RcDebugPrint (SDBG_MEM_TRAIN,
                    "\n");
    if (Group == RxDqsDelay) {
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT,
                          "RxDqs");
    } else if (Group == RxDqsPDelay) {
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT,
                          "RxDqsP");
    } else if (Group == RxDqsNDelay) {
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT,
                          "RxDqsN");
    } else if (Group == TxDqDelay) {
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT,
                          "TxDq");
    } else if (Group == RxVref) {
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT,
                          "RxVref");
    } else if (Group == TxVref) {
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT,
                          "TxVref");
    }
    RcDebugPrint (SDBG_DEFAULT, " - Per Bit margins\n");

    if (Host->nvram.mem.eccEn == 0) {
      MaxBits = MBV - 8;
    } else {
      MaxBits = MBV;
    }
    for (Bit = 0; Bit < MaxBits; Bit++) {
      if ((Bit == 0) || (Bit == (MaxBits / 2))) {
        RcDebugPrint (SDBG_MEM_TRAIN,
                        "Bits:");
        for (b = Bit; b < (Bit + (MaxBits / 2)); b++) {
          RcDebugPrint (SDBG_MEM_TRAIN,
                          "  %2d", b);
        }
        RcDebugPrint (SDBG_MEM_TRAIN,
                        "\n");
        RcDebugPrint (SDBG_MEM_TRAIN,
                       "RE:  ");
      }

      RcDebugPrint (SDBG_MEM_TRAIN,
                      " %3d", ResultsBit->bits[Ch][Bit].p);

      if ((Bit == ((MaxBits / 2) - 1)) || (Bit == (MaxBits - 1))) {
        RcDebugPrint (SDBG_MEM_TRAIN,
                        "\n");
        RcDebugPrint (SDBG_MEM_TRAIN,
                       "LE:  ");
        for (b = Bit - ((MaxBits / 2) - 1); b <= Bit; b++) {
          RcDebugPrint (SDBG_MEM_TRAIN,
                          " %3d", ResultsBit->bits[Ch][b].n);
        }
        RcDebugPrint (SDBG_MEM_TRAIN,
                        "\n\n");
      }
    } // Bit loop
    RcDebugPrint (SDBG_MEM_TRAIN,
                    "\n");
  }
#endif // DEBUG_CODE_BLOCK
} // DisplayREandLEMarginsDdr5

/**

  This is a DDR5 wrapper for GetMargins.  GetMargins is only called when DDR5 is installed.

  @param[in] Host           - Pointer to sysHost
  @param[in] Socket         - Processor socket to check
  @param[in] Level          - DDR or LRDIMM Host side, LRDIMM Backside, etc...
  @param[in] Group          - Parameter to be margined
  @param[in] Mode           - Test mode to use
  @param[in] Scope          - Margin granularity
  @param[in] Mask           - Mask of bits to exclude from testing
  @param[in out] MarginData - Pointer to the structure to store the margin results. The structure type varies based on scope.
  @param[in] PatternLength  - RankMarginTest Pattern length
  @param[in] Update         - Update starting margins so the next pass will run faster or not
  @param[in] ChMask         - Bit Mask of channels to not be used
  @param[in] RankMask       - Bit Mask of ranks to not be used
  @param[in] BurstLength    - Number of cachelines to test for each write/read operation
  @param[in] TestType       - Type of test to be run

  @retval None

**/
VOID
GetLegacyRmtMarginsDdr5 (
  IN PSYSHOST        Host,
  IN UINT8           Socket,
  IN MRC_LT          Level,
  IN MRC_GT          Group,
  IN UINT16          Mode,
  IN UINT8           Scope,
  IN struct bitMask  *Mask,
  IN OUT VOID        *MarginData,
  IN UINT32          PatternLength,
  IN UINT8           Update,
  IN UINT8           ChMask,
  IN UINT8           RankMask,
  IN UINT8           BurstLength,
  IN MRC_TT          TestType
  )
{
  UINT8  SubChannel;
  UINT8  MaxSubChannels;
  UINT8  ScopeSelection;
  UINT8  Channel;
  UINT8  Dimm;
  UINT8  Rank;
  UINT8  Strobe;
  UINT8  LogicalRank;
  INT16  Left;
  INT16  Right;
  INT16  MinLeft;
  INT16  MinRight;
  VOID   *MarginDataPtr;
  CHAR8  *StringPtr;
  struct strobeMargin Results;
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  UINT8  MaxChDdr;

  //
  // Initialize local data
  //
  MaxChDdr = GetMaxChDdr ();
  MarginDataPtr   = MarginData;
  SubChannel      = NO_SUBCH;
  ScopeSelection  = Scope;
  SetMem ((VOID *)&Results, sizeof (Results), 0);
  MaxSubChannels = GetMaxStrobeValid (Host) / GetSubChMaxStrobeValid (Host);
  GetGroupString (Group, &StringPtr);

  // Fill out debug print info
  switch (Group) {
    case Cmd0All:
    case Ctl0Grp:
      SubChannel = 0;
      break;
    case Cmd1All:
    case Ctl1Grp:
      SubChannel = 1;
      break;
    case RxDqsDelay:
    case TxDqDelay:
    case RxVref:
    case TxVref:
    case CmdAll:
    case CtlAll:
      //
      // Override:
      // Scope to get results per strobe
      //
      //
      ScopeSelection  = SCOPE_STROBE;
      MarginDataPtr   = (VOID *)&Results;
      break;
    default:
      break;
  }

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, SubChannel, NO_RANK, NO_STROBE, NO_BIT, "%a\n", StringPtr);

  GetMargins (
    Host, Socket, Level, Group, Mode,
    ScopeSelection, Mask, MarginDataPtr, PatternLength,
    Update, ChMask, RankMask, BurstLength, TestType, NULL
    );

  if (Scope == SCOPE_SUBCH) {
    switch (Group) {
    case RxDqsDelay:
    case TxDqDelay:
    case RxVref:
    case TxVref:
    case CmdAll:
    case CtlAll:
      for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
        //
        // Skip if the channel is disabled or masked out
        //
        if ((IsChannelEnabled (Socket, Channel) == FALSE) ||
            (ChMask & (1 << Channel))) {
          continue;
        }

        DimmNvList = GetDimmNvList (Host, Socket, Channel);

        for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

          RankList = GetRankNvList (Host, Socket, Channel, Dimm);

          for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {

            //
            // Get Logical rank
            //
            LogicalRank = GetLogicalRank (Host, Socket, Channel, Dimm, Rank);

            //
            // Skip if the rank is disabled or not suitable for training or masked out
            //
            if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT) ||
                CheckSkipRankTrain (Host, Socket, Channel, Dimm, Rank, Level, Group) ||
                (RankMask & (1 << LogicalRank))) {
              continue;
            }
            for (SubChannel = 0; SubChannel < MaxSubChannels; SubChannel++) {

              MinLeft  = INT16_MIN;
              MinRight = INT16_MAX;

              for (Strobe = 0; Strobe < GetSubChMaxStrobeValid (Host); Strobe++) {

                Left  = Results.strobe[Channel][Rank][Strobe].n;
                Right = Results.strobe[Channel][Rank][Strobe].p;

                if (Left < Right) {
                  if (MinLeft < Left) {
                    MinLeft = Left;
                  }
                  if (Right < MinRight) {
                    MinRight = Right;
                  }
                }
              } // Strobe loop ...
              if (MinLeft == INT16_MIN) {
                ((struct rankMargin *)MarginData)[SubChannel].rank[Channel][LogicalRank].n = 0;
              } else {
                ((struct rankMargin *)MarginData)[SubChannel].rank[Channel][LogicalRank].n = MinLeft;
              }
              if (MinRight == INT16_MAX) {
                ((struct rankMargin *)MarginData)[SubChannel].rank[Channel][LogicalRank].p = 0;
              } else {
                ((struct rankMargin *)MarginData)[SubChannel].rank[Channel][LogicalRank].p = MinRight;
              }

            } // SubChannel loop ...
          } // Rank loop ...
        } // Dimm loop ...
      } // Channel loop ...
      break;
    } // Switch (Group)
  }
}

/**

  Writes GetMargin results to Debug log.  Results are only displayed when DDR5 is installed.

  @param[in] Host           - Pointer to sysHost
  @param[in] Socket         - Processor socket
  @param[in] ResultsQcs     - Pointer to array which contains QCS test results
  @param[in] ResultsQca     - Pointer to array which contains QCA test results
  @param[in] ResultsRxDqs   - Pointer to array which contains RxDqs test results
  @param[in] ResultsTxDq    - Pointer to array which contains TxDq test results
  @param[in] ResultsRxVref  - Pointer to array which contains RxVref test results
  @param[in] ResultsTxVref  - Pointer to array which contains TxVref test results
  @param[in] ResultsCmd     - Pointer to array which contains Cmd test results
  @param[in] ResultsCtl     - Pointer to array which contains Ctl test results
  @param[in] NumSubCh       - Number of elements in ResultsQcs and ResultsQca

**/
VOID
DisplayLegacyRmtMarginsDdr5 (
  IN PSYSHOST           Host,
  IN UINT8              Socket,
  IN struct rankMargin  *ResultsQcs,
  IN struct rankMargin  *ResultsQca,
  IN struct rankMargin  *ResultsRxDqs,
  IN struct rankMargin  *ResultsTxDq,
  IN struct rankMargin  *ResultsRxVref,
  IN struct rankMargin  *ResultsTxVref,
  IN struct rankMargin  *ResultsCmd,
  IN struct rankMargin  *ResultsCtl,
  IN UINT8              NumSubCh
  )
{
  UINT8                Channel;
  UINT8                Dimm;
  UINT8                Rank;
  UINT8                SubChannel;
  struct dimmNvram     (*DimmNvList)[MAX_DIMM];
  struct ddrRank       (*RankList)[MAX_RANK_DIMM];
  UINT8 MaxChDdr;

  // Only display results if test is enabled

  RcDebugPrint (SDBG_DEFAULT, "\n                    RxDqs-   RxDqs+   TxDq-   TxDq+    RxV-    RxV+    TxV-    TxV+    Cmd-    Cmd+    Ctl-    Ctl+");
  if (IsDdr5BacksideCmdMarginEnabled ()) {
    RcDebugPrint (SDBG_DEFAULT, "    QxCA-    QxCA+    QxCS-     QxCS+");
  }
  RcDebugPrintLine (SDBG_DEFAULT, 118, SINGLE_LINE, CRLF_FLAG, CRLF_FLAG);

  MaxChDdr = GetMaxChDdr ();

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Channel);

    //
    // Loop for each Dimm and each Rank
    //
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      RankList = GetRankNvList (Host, Socket, Channel, Dimm);

      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        //
        // Skip if no Rank
        //
        if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        for (SubChannel = 0; SubChannel < NumSubCh; SubChannel++) {
          RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, Channel, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
            "   %3d     %3d     %3d      %3d      %3d     %3d     %3d     %3d     %3d     %3d     %3d     %3d",
            ResultsRxDqs[SubChannel].rank[Channel][(*RankList)[Rank].rankIndex].n, ResultsRxDqs[SubChannel].rank[Channel][(*RankList)[Rank].rankIndex].p,
            ResultsTxDq[SubChannel].rank[Channel][(*RankList)[Rank].rankIndex].n,  ResultsTxDq[SubChannel].rank[Channel][(*RankList)[Rank].rankIndex].p,
            ResultsRxVref[SubChannel].rank[Channel][(*RankList)[Rank].rankIndex].n, ResultsRxVref[SubChannel].rank[Channel][(*RankList)[Rank].rankIndex].p,
            ResultsTxVref[SubChannel].rank[Channel][(*RankList)[Rank].rankIndex].n, ResultsTxVref[SubChannel].rank[Channel][(*RankList)[Rank].rankIndex].p,
            ResultsCmd[SubChannel].rank[Channel][(*RankList)[Rank].rankIndex].n, ResultsCmd[SubChannel].rank[Channel][(*RankList)[Rank].rankIndex].p,
            ResultsCtl[SubChannel].rank[Channel][(*RankList)[Rank].rankIndex].n, ResultsCtl[SubChannel].rank[Channel][(*RankList)[Rank].rankIndex].p
            );

          if (IsDdr5BacksideCmdMarginEnabled ()) {
            RcDebugPrint (
              SDBG_DEFAULT,
              "   %3d      %3d      %3d      %3d",
              ResultsQca[SubChannel].rank[Channel][(*RankList)[Rank].rankIndex].n, ResultsQca[SubChannel].rank[Channel][(*RankList)[Rank].rankIndex].p,
              ResultsQcs[SubChannel].rank[Channel][(*RankList)[Rank].rankIndex].n, ResultsQcs[SubChannel].rank[Channel][(*RankList)[Rank].rankIndex].p
              );
          }
          RcDebugPrint (SDBG_DEFAULT, "\n");
        }
      } // Rank loop
    } // Dimm loop
  } // Channel loop

}

