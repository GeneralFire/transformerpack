/** @file
  LegacyRmtLib.c

  Library implementation of API to support Legacy RMT function.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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
#include <Library/MemoryServicesLib.h>
#include <Library/CheckpointLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/TimerLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Library/MemFmcIpLib.h>
#include <Ppi/MemoryPolicyPpi.h>
#include <Library/Ddr5CoreLib.h>

#ifdef DDR5_SUPPORT
#include "Include/Memory/Ddr5MrRegs.h"
#endif // #ifdef DDR5_SUPPORT

#define IS_NEG(a) ((a < 0) ? 1 : 0)

/**

  Clears Bit Margin results structure

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket number
  @param[in] ResultsBit  - Bits to clear

  @retval N/A

**/
VOID
ClearBitResults (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN struct bitMargin  *ResultsBit
  )
{
  UINT8               Channel;
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               Bit;
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  //
  // Clear Margin data
  //
  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      RankList = GetRankNvList(Host, Socket, Channel, Dimm);
      for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {
        // Do not look at more than two ranks for the third DIMM
        if ((Dimm > 1) && (Rank > 1)) {
          continue;
        }

        // Initialize to 0
        for (Bit = 0; Bit < 72; Bit++) {
          ResultsBit->bits[Channel][(*RankList)[Rank].rankIndex][Bit].n = 0;
          ResultsBit->bits[Channel][(*RankList)[Rank].rankIndex][Bit].p = 0;
        }
      } // Rank loop
    } // Dimm loop
  } // Channel loop
} //ClearBitResults


/**

  Finds margins for various parameters per Rank

  @param[in] Host    - Pointer to sysHost,
                       the system Host (root) structure struct

  @retval SUCCESS       Function executed successfully
  @retVal FAILURE       Function has errors

**/
UINT32
EFIAPI
RankMarginTool (
  IN PSYSHOST Host
  )
{
  UINT8               Socket;
  UINT8               Channel;
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               DllTest;
  UINT16              MaxDllLoops;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  UINT64              StartTsc  = 0;
  UINT64              EndTsc    = 0;
  UINT32              MrcLatency = 0;
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  struct bitMask      Filter;
  struct bitMargin    ResultsBit;
#ifdef DEBUG_CODE_BLOCK
  UINT8 DataMask[MAX_STROBE/2];
  UINT8               Strobe;
  UINT32              OriginalSerialDebugMsgLvl;
#endif // DEBUG_CODE_BLOCK
  struct rankMargin   ResultsRxDq;
  struct rankMargin   ResultsTxDq;
  struct rankMargin   ResultsRxVref;
  struct rankMargin   ResultsTxVref;
  struct rankMargin   ResultsCmdVref;
  struct rankMargin   ResultsCmd;

  struct rankMargin   ResultsQcs[SUB_CH];
  struct rankMargin   ResultsQca[SUB_CH];

  UINT32              ChBitmask;
  struct rankMargin   ResultsCtl;
#ifdef LRDIMM_SUPPORT
  struct subRankMargin ResultsBacksideSubRankRxDq;
  struct subRankMargin ResultsBacksideSubRankTxDq;
  struct subRankMargin ResultsBacksideSubRankRxVref;
  struct subRankMargin ResultsBacksideSubRankTxVref;
#endif
  struct subRankMargin ResultsBacksideSubRankCmd;
  struct subRankMargin ResultsBacksideSubRankCtl;
  struct subRankMargin ResultsBacksideSubRankCaVref;
  UINT8                SubRank;
  SYS_SETUP            *Setup;
  UINT8                MSVx4;
  UINT8                MSVx8;
  UINT8                MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();
  Host = GetSysHostPointer ();

  MSVx4 = GetMaxStrobeValid (Host);
  MSVx8 = GetMaxStrobeValid (Host) / 2;

  ChBitmask = 0;

  SetMem ((VOID *)ResultsQca, sizeof (ResultsQca), 0);
  SetMem ((VOID *)ResultsQcs, sizeof (ResultsQcs), 0);

  if (GetSysBootMode () == S3Resume || Host->var.mem.subBootMode == WarmBootFast ||
      (Host->var.mem.subBootMode == ColdBootFast && !(Setup->mem.options & RMT_COLD_FAST_BOOT))) {
    RcDebugPrint (SDBG_ERROR, "LegacyRmt Not supported in current Boot mode\n");
    return SUCCESS;
  }

  if (!IsMemFlowEnabled (LegacyRankMarginTool)) {
    RcDebugPrint (SDBG_ERROR, "LegacyRmt Not enabled in MemFlow\n");
    return SUCCESS;
  }
  if((Setup->mem.options & MARGIN_RANKS_EN) == 0) {
    RcDebugPrint (SDBG_ERROR, "LegacyRmt Not enabled in Bios Setup\n");
    return SUCCESS;
  }

  //
  // Return if this Socket is disabled
  //
  Socket = Host->var.mem.currentSocket;
  if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
    RcDebugPrint (SDBG_ERROR, "Cannot run LegacyRmt Socket %d disabled\n", Socket);
    return SUCCESS;
  }

  if (GetEmulation() & SIMICS_FLAG) {
    RcDebugPrint (SDBG_ERROR, "Cannot run LegacyRmt in simics\n");
    return SUCCESS;
  }

  Host->var.mem.runningRmt = 1;
#ifdef DEBUG_CODE_BLOCK
  OriginalSerialDebugMsgLvl = GetDebugLevel ();

  if (Setup->common.DfxPpvEnabled == DFX_PPV_ENABLED) {
    SetDebugLevel (SDBG_MINMAX);
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "DBG:BEGIN %s()\n", __FUNCTION__);
  }
#endif //DEBUG_CODE_BLOCK

  // Issue a minor check point
  OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_RMT, 0);

  SetConfigBeforeRmt (Host, Socket);

  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // Clear the Filter bits to enable error checking on every Bit
  //
  ClearFilter (Host, Socket, &Filter);

  if (Setup->mem.dllResetTestLoops) {
    MaxDllLoops = Setup->mem.dllResetTestLoops * 2;
  } else {
    MaxDllLoops = 1;
  }

  for (DllTest = 0; DllTest < MaxDllLoops; DllTest++) {

    Host->var.mem.currentSubRank = 0;
    Host->var.mem.maxSubRank = 1;

    for (Channel = 0; Channel < MaxChDdr; Channel++) {
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        RankList = GetRankNvList (Host, Socket, Channel, Dimm);

        for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {
          // Do not look at more than two ranks for the third DIMM
          if ((Dimm > 1) && (Rank > 1)) {
            continue;
          }

          // Initialize to 0
          ResultsRxDq.rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
          ResultsRxDq.rank[Channel][(*RankList)[Rank].rankIndex].p = 0;
          ResultsRxVref.rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
          ResultsRxVref.rank[Channel][(*RankList)[Rank].rankIndex].p = 0;
          ResultsTxDq.rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
          ResultsTxDq.rank[Channel][(*RankList)[Rank].rankIndex].p = 0;
          ResultsTxVref.rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
          ResultsTxVref.rank[Channel][(*RankList)[Rank].rankIndex].p = 0;
          ResultsCmd.rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
          ResultsCmd.rank[Channel][(*RankList)[Rank].rankIndex].p = 0;
          ResultsCmdVref.rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
          ResultsCmdVref.rank[Channel][(*RankList)[Rank].rankIndex].p = 0;
          ResultsCtl.rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
          ResultsCtl.rank[Channel][(*RankList)[Rank].rankIndex].p = 0;

          if (IsDdr4BacksideCmdMarginEnabled (Host, Socket)) {
            for (SubRank = 0; SubRank < Host->var.mem.maxSubRank; SubRank++) {
              ResultsBacksideSubRankCmd.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
              ResultsBacksideSubRankCmd.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p = 0;
              ResultsBacksideSubRankCtl.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
              ResultsBacksideSubRankCtl.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p = 0;
              ResultsBacksideSubRankCaVref.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
              ResultsBacksideSubRankCaVref.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p = 0;
            }
          }
#ifdef LRDIMM_SUPPORT
          if ((IsLrdimmPresent (Socket, Channel, Dimm)) && (Setup->mem.enableBacksideRMT)) {
            for (SubRank = 0; SubRank < Host->var.mem.maxSubRank; SubRank++) {
              ResultsBacksideSubRankRxDq.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
              ResultsBacksideSubRankRxDq.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p = 0;
              ResultsBacksideSubRankTxDq.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
              ResultsBacksideSubRankTxDq.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p = 0;
              ResultsBacksideSubRankRxVref.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
              ResultsBacksideSubRankRxVref.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p = 0;
              ResultsBacksideSubRankTxVref.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
              ResultsBacksideSubRankTxVref.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p = 0;
            }
          }
#endif // LRDIMM_SUPPORT
        } // Rank loop
      } // Dimm loop
    } // Channel loop

    StartTsc = GetPerformanceCounter ();

    if( (Setup->mem.options & MARGIN_RANKS_EN) && (Setup->mem.rmtPatternLength > 4000 )){
        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "RMT Pattern length %d\n",
                      Setup->mem.rmtPatternLength);
    }

    if ((Setup->mem.options & MARGIN_RANKS_EN) && (Setup->mem.rmtPatternLengthExt > 4000 )) {
        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "CMD Pattern length %d\n",
                      Setup->mem.rmtPatternLengthExt);
    }

    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Getting margins:\n");

#ifndef DDR5_SUPPORT
    //
    // Avoid clock movement for BPS A0
    //
    if (FmcAvoidClockMovementInSocket (Host, Socket) == FALSE) {
      DeNormalizeCCC (Host, Socket, DENORMALIZE);
    }

    //
    // Setup CADB for ColdFastBoot Margin tests
    //
    if (Host->var.mem.subBootMode == ColdBootFast){
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        DimmNvList = GetDimmNvList(Host, Socket, Channel);
        for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
          ChBitmask = GetChBitmask (Host, Socket, Dimm, Rank);
          SetupIOTestBasicVA (Host, Socket, ChBitmask, 1, NSOE, 0, 0, 10);
        }
      }
    }

    // Setting the CPGC DDRT RPQ credit limit on certain projects will cause the related timer to be enabled.
    SetDefaultCpgcRpqCreditLimitIfNeeded (Host, Socket);

    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CMD\n");
    GetMargins (Host, Socket, DdrLevel, CmdAll, MODE_VA_DESELECT, SCOPE_RANK, &Filter, (VOID *)&ResultsCmd,
      Setup->mem.rmtPatternLengthExt, 1, 0, 0, WDB_BURST_LENGTH, RmtCmdTest, NULL);

    if (IsMemFlowEnabled (CmdVrefCenteringTraining) == TRUE) {
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CMDVref\n");
       GetMargins (Host, Socket, DdrLevel, CmdVref, MODE_VA_DESELECT, SCOPE_RANK, &Filter, (VOID *)&ResultsCmdVref,
       Setup->mem.rmtPatternLengthExt, 1, 0, 0, WDB_BURST_LENGTH, RmtCmdTest, NULL);
    }

    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CTL\n");
    GetMargins (Host, Socket, DdrLevel, CtlAll, MODE_VA_DESELECT, SCOPE_RANK, &Filter, (VOID *)&ResultsCtl,
      Setup->mem.rmtPatternLengthExt, 1, 0, 0, WDB_BURST_LENGTH, RmtCmdTest, NULL);

    // Clearing the CPGC DDRT RPQ credit limit on certain projects will cause the related timer to be disabled.
    ClearCpgcRpqCreditLimitIfNeeded (Host, Socket);

    //
    // Avoid clock movement for BPS A0
    //
    if (FmcAvoidClockMovementInSocket (Host, Socket) == FALSE) {
      DeNormalizeCCC (Host, Socket, RENORMALIZE);
    }
#endif // #ifndef DDR5_SUPPORT

    if (!Host->RmtFlags.Bits.SkipRmtTxDq) {
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "TxDq\n");
      GetMargins (Host, Socket, DdrLevel, TxDqDelay, MODE_VIC_AGG, SCOPE_RANK, &Filter, (VOID *)&ResultsTxDq,
        Setup->mem.rmtPatternLength, 1, 0, 0, WDB_BURST_LENGTH, RmtDqTest, NULL);
    }

    if (!Host->RmtFlags.Bits.SkipRmtTxVref) {
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "TxVref\n");
      GetMargins (Host, Socket, DdrLevel, TxVref, MODE_VIC_AGG, SCOPE_RANK, &Filter, (VOID *)&ResultsTxVref,
        Setup->mem.rmtPatternLength, 1, 0, 0, WDB_BURST_LENGTH, RmtDqTest, NULL);
    }
    //
    // Do not recenter if we are testing DLL reset
    //
    Host->var.mem.runningRmt = 0;
    if (Setup->mem.dllResetTestLoops != 0) {
      //
      // Issue a DLL reset after the first half of testing
      //
      if(DllTest >= Setup->mem.dllResetTestLoops) {
        IssueDllReset (Host, Socket);
      }
    }
    Host->var.mem.runningRmt = 1;

    if (!Host->RmtFlags.Bits.SkipRmtRxDqs) {
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "RxDqs\n");
      GetMargins (Host, Socket, DdrLevel, RxDqsDelay, MODE_VIC_AGG, SCOPE_RANK, &Filter, (VOID *)&ResultsRxDq,
        Setup->mem.rmtPatternLength, 1, 0, 0, WDB_BURST_LENGTH, RmtDqTest, NULL);
    }

    if (!Host->RmtFlags.Bits.SkipRmtRxVref) {
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "RxVref\n");
      GetMargins (Host, Socket, DdrLevel, RxVref, MODE_VIC_AGG, SCOPE_RANK, &Filter, (VOID *)&ResultsRxVref,
        Setup->mem.rmtPatternLength, 1, 0, 0, WDB_BURST_LENGTH, RmtDqTest, NULL);
    }



    if (IsDdr5BacksideCmdMarginEnabled ()) {
      GetLegacyRmtMarginsDdr5 (
        Host, Socket, LrbufLevel, Cmd0All, MODE_VA_DESELECT,
        SCOPE_RANK, &Filter, (VOID *)&ResultsQca[0],
        Setup->mem.rmtPatternLengthExt, 1, 0, 0, WDB_BURST_LENGTH, RmtCmdTest
        );
      GetLegacyRmtMarginsDdr5 (
        Host, Socket, LrbufLevel, Cmd1All, MODE_VA_DESELECT,
        SCOPE_RANK, &Filter, (VOID *)&ResultsQca[1],
        Setup->mem.rmtPatternLengthExt, 1, 0, 0, WDB_BURST_LENGTH, RmtCmdTest
        );
      GetLegacyRmtMarginsDdr5 (
        Host, Socket, LrbufLevel, Ctl0Grp, MODE_VA_DESELECT,
        SCOPE_RANK, &Filter, (VOID *)&ResultsQcs[0],
        Setup->mem.rmtPatternLengthExt, 1, 0, 0, WDB_BURST_LENGTH, RmtCmdTest
        );
      GetLegacyRmtMarginsDdr5 (
        Host, Socket, LrbufLevel, Ctl1Grp, MODE_VA_DESELECT,
        SCOPE_RANK, &Filter, (VOID *)&ResultsQcs[1],
        Setup->mem.rmtPatternLengthExt, 1, 0, 0, WDB_BURST_LENGTH, RmtCmdTest
        );
    }

    // DDR4 RDIMM RCD Rev 2 backside Cmd, Ctl margins
    if (IsDdr4BacksideCmdMarginEnabled (Host, Socket)) {

      for (SubRank = 0; SubRank < Host->var.mem.maxSubRank; SubRank++) {
        Host->var.mem.currentSubRank = SubRank;
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Get Backside Command margins:\n");
        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "BACKSIDE_CMD\n");
        GetMargins (Host, Socket, LrbufLevel, CmdAll, MODE_VA_DESELECT, SCOPE_RANK, &Filter, (VOID *)&(ResultsBacksideSubRankCmd.subRank[SubRank]),
          Setup->mem.rmtPatternLengthExt, 1, 0, 0, WDB_BURST_LENGTH, RmtCmdTest, NULL);

        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "BACKSIDE_CTL\n");
        GetMargins (Host, Socket, LrbufLevel, CtlAll, MODE_VA_DESELECT, SCOPE_RANK, &Filter, (VOID *)&(ResultsBacksideSubRankCtl.subRank[SubRank]),
          Setup->mem.rmtPatternLengthExt, 1, 0, 0, WDB_BURST_LENGTH, RmtCmdTest, NULL);

        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "BACKSIDE_CMD_VREF\n");
        GetMargins (Host, Socket, LrbufLevel, CmdVref, MODE_VA_DESELECT, SCOPE_RANK, &Filter, (VOID *)&(ResultsBacksideSubRankCaVref.subRank[SubRank]),
          Setup->mem.rmtPatternLengthExt, 1, 0, 0, WDB_BURST_LENGTH, RmtCmdTest, NULL);
      }
      Host->var.mem.currentSubRank = 0;
    }
#ifdef LRDIMM_SUPPORT
    if ((Host->nvram.mem.socket[Socket].lrDimmPresent) && (Setup->mem.enableBacksideRMT)) {
      for (SubRank = 0; SubRank < Host->var.mem.maxSubRank; SubRank++) {
        Host->var.mem.currentSubRank = SubRank;
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Get Backside Data margins:\n");
        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "RxDqs\n");
        GetMargins (Host, Socket, LrbufLevel, RxDqsDelay, MODE_VIC_AGG, SCOPE_RANK, &Filter, (VOID *)&(ResultsBacksideSubRankRxDq.subRank[SubRank]),
          Setup->mem.rmtPatternLength, 1, 0, 0, WDB_BURST_LENGTH, RmtDqTest, NULL);

        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "RxVref\n");
        GetMargins (Host, Socket, LrbufLevel, RxVref, MODE_VIC_AGG, SCOPE_RANK, &Filter, (VOID *)&(ResultsBacksideSubRankRxVref.subRank[SubRank]),
          Setup->mem.rmtPatternLength, 1, 0, 0, WDB_BURST_LENGTH, RmtDqTest, NULL);

        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "TxVref\n");
        GetMargins (Host, Socket, LrbufLevel, TxVref, MODE_VIC_AGG, SCOPE_RANK, &Filter, (VOID *)&(ResultsBacksideSubRankTxVref.subRank[SubRank]),
          Setup->mem.rmtPatternLength, 1, 0, 0, WDB_BURST_LENGTH, RmtDqTest, NULL);

        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "TxDqs\n");
        GetMargins (Host, Socket, LrbufLevel, TxDqDelay, MODE_VIC_AGG, SCOPE_RANK, &Filter, (VOID *)&(ResultsBacksideSubRankTxDq.subRank[SubRank]),
          Setup->mem.rmtPatternLength, 1, 0, 0, WDB_BURST_LENGTH, RmtDqTest, NULL);
      }
      Host->var.mem.currentSubRank = 0;
    }
#endif // LRDIMM_SUPPORT
    EndTsc = GetPerformanceCounter ();
    MrcLatency = TimeDiff (StartTsc, EndTsc, TDIFF_UNIT_S);
    RcDebugPrint (SDBG_MEM_TRAIN,
                        "\nRMT time            %d ms\n", MrcLatency);
#ifdef DEBUG_CODE_BLOCK
    if (GetDebugLevel () & SDBG_MEM_TRAIN) {
      AcquirePrintControl ();
    }

    if (Setup->mem.dllResetTestLoops) {
      if (DllTest == 0) {
        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "START_DLL_RESET_TEST_NO_DLL_RESET\n");
      } else if (DllTest == Setup->mem.dllResetTestLoops) {
        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "START_DLL_RESET_TEST_DLL_RESET\n");
      }
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "DLL test loop = %d\n", DllTest % Setup->mem.dllResetTestLoops);
    }

    //
    // Display units for each parameter
    //
    if ((DllTest == 0) || (DllTest == Setup->mem.dllResetTestLoops)) {
      RcDebugPrint (SDBG_MINMAX,
                    "\nRxDqs = 1/64th QCLK\nRxVref = 4.68 mV\nTxDq = 1/64th QCLK\nTxVref = 7.8 mV\nCmd = 1/64th QCLK\n");
      if (IsMemFlowEnabled (CmdVrefCenteringTraining) == TRUE) {
        if (InternalRcdVrefCaEnabled ()) {
          RcDebugPrint (SDBG_MINMAX, "CmdVref = 10 mV\nCmdVref FMC = 6.25 mV\n");
        } else {
          RcDebugPrint (SDBG_MINMAX, "CmdVref = 6.25 mV\nCmdVref FMC = 6.25 mV\n");
        }
      }
      RcDebugPrint (SDBG_MINMAX, "Ctl = 1/64th QCLK\n\n");
    }

    if (GetDebugLevel () & SDBG_MINMAX) {
      RcDebugPrint (SDBG_DEFAULT, "START_RMT_N%d\n", Socket);

      if (Host->RmtFlags.Bits.EnableShortFormat) {
        RcDebugPrint (SDBG_DEFAULT, "\n             ");
        if (!Host->RmtFlags.Bits.SkipRmtRxDqs) {
          RcDebugPrint (SDBG_DEFAULT, "RxDqs-   RxDqs+   ");
        }
        if (!Host->RmtFlags.Bits.SkipRmtRxVref) {
          RcDebugPrint (SDBG_DEFAULT, "RxV-     RxV+     ");
        }
        if (!Host->RmtFlags.Bits.SkipRmtTxDq) {
          RcDebugPrint (SDBG_DEFAULT, "TxDq-    TxDq+    ");
        }
        if (!Host->RmtFlags.Bits.SkipRmtTxVref) {
          RcDebugPrint (SDBG_DEFAULT, "TxV-     TxV+     ");
        }

        RcDebugPrintLine (SDBG_DEFAULT, 118, SINGLE_LINE, CRLF_FLAG, CRLF_FLAG);

      } else if (IsMemFlowEnabled (CmdVrefCenteringTraining) == TRUE) {
        RcDebugPrint (SDBG_DEFAULT, "\n             RxDqs-   RxDqs+     RxV-     RxV+    TxDq-    TxDq+     TxV-     TxV+     Cmd-     Cmd+    CmdV-    CmdV+     Ctl-     Ctl+");
        RcDebugPrintLine (SDBG_DEFAULT, 138, SINGLE_LINE, CRLF_FLAG, CRLF_FLAG);
      } else {
        RcDebugPrint (SDBG_DEFAULT, "\n             RxDqs-   RxDqs+     RxV-     RxV+    TxDq-    TxDq+     TxV-     TxV+     Cmd-     Cmd+     Ctl-     Ctl+");
        RcDebugPrintLine (SDBG_DEFAULT, 118, SINGLE_LINE, CRLF_FLAG, CRLF_FLAG);
      }

    }
#endif // DEBUG_CODE_BLOCK
    ChannelNvList = GetChannelNvList(Host, Socket);

    //
    // Loop for each channel
    //
    for (Channel = 0; Channel < MaxChDdr; Channel++) {
      if ((*ChannelNvList)[Channel].enabled == 0) {
        continue;
      }

      DimmNvList = GetDimmNvList(Host, Socket, Channel);

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

        RankList = GetRankNvList(Host, Socket, Channel, Dimm);

        for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
          //
          // Skip if no Rank
          //
          if (CheckRank(Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          if (ResultsRxDq.rank[Channel][(*RankList)[Rank].rankIndex].n >
              ResultsRxDq.rank[Channel][(*RankList)[Rank].rankIndex].p) {
            ResultsRxDq.rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
            ResultsRxDq.rank[Channel][(*RankList)[Rank].rankIndex].p = 0;
          }
          if (ResultsTxDq.rank[Channel][(*RankList)[Rank].rankIndex].n >
              ResultsTxDq.rank[Channel][(*RankList)[Rank].rankIndex].p) {
            ResultsTxDq.rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
            ResultsTxDq.rank[Channel][(*RankList)[Rank].rankIndex].p = 0;
          }
          if (ResultsRxVref.rank[Channel][(*RankList)[Rank].rankIndex].n >
              ResultsRxVref.rank[Channel][(*RankList)[Rank].rankIndex].p) {
            ResultsRxVref.rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
            ResultsRxVref.rank[Channel][(*RankList)[Rank].rankIndex].p = 0;
          }
          if (ResultsTxVref.rank[Channel][(*RankList)[Rank].rankIndex].n >
              ResultsTxVref.rank[Channel][(*RankList)[Rank].rankIndex].p) {
            ResultsTxVref.rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
            ResultsTxVref.rank[Channel][(*RankList)[Rank].rankIndex].p = 0;
          }
          if (ResultsCmd.rank[Channel][(*RankList)[Rank].rankIndex].n >
              ResultsCmd.rank[Channel][(*RankList)[Rank].rankIndex].p) {
            ResultsCmd.rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
            ResultsCmd.rank[Channel][(*RankList)[Rank].rankIndex].p = 0;
          }
          if (IsMemFlowEnabled (CmdVrefCenteringTraining) == TRUE) {
            if (ResultsCmdVref.rank[Channel][(*RankList)[Rank].rankIndex].n >
                ResultsCmdVref.rank[Channel][(*RankList)[Rank].rankIndex].p) {
              ResultsCmdVref.rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
              ResultsCmdVref.rank[Channel][(*RankList)[Rank].rankIndex].p = 0;
            }
          }

          if (ResultsCtl.rank[Channel][(*RankList)[Rank].rankIndex].n >
              ResultsCtl.rank[Channel][(*RankList)[Rank].rankIndex].p) {
            ResultsCtl.rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
            ResultsCtl.rank[Channel][(*RankList)[Rank].rankIndex].p = 0;
          }

          (*RankList)[Rank].cmdLeft = (UINT8)ABS (ResultsCmd.rank[Channel][(*RankList)[Rank].rankIndex].n);
          (*RankList)[Rank].cmdRight = (UINT8)ResultsCmd.rank[Channel][(*RankList)[Rank].rankIndex].p;
          if (IsMemFlowEnabled (CmdVrefCenteringTraining) == TRUE) {
            (*RankList)[Rank].cmdLow = (UINT8)ABS (ResultsCmdVref.rank[Channel][(*RankList)[Rank].rankIndex].n);
            (*RankList)[Rank].cmdHigh = (UINT8)ResultsCmdVref.rank[Channel][(*RankList)[Rank].rankIndex].p;
          }

          if (Host->RmtFlags.Bits.EnableShortFormat) {
            RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, "   ");

            if (!Host->RmtFlags.Bits.SkipRmtRxDqs) {
              RcDebugPrint (SDBG_DEFAULT, "%3d      %3d      ",
                ResultsRxDq.rank[Channel][(*RankList)[Rank].rankIndex].n,
                ResultsRxDq.rank[Channel][(*RankList)[Rank].rankIndex].p
                );
            }
            if (!Host->RmtFlags.Bits.SkipRmtRxVref) {
              RcDebugPrint (SDBG_DEFAULT, "%3d      %3d      ",
                ResultsRxVref.rank[Channel][(*RankList)[Rank].rankIndex].n,
                ResultsRxVref.rank[Channel][(*RankList)[Rank].rankIndex].p
                );
            }
            if (!Host->RmtFlags.Bits.SkipRmtTxDq) {
              RcDebugPrint (SDBG_DEFAULT, "%3d      %3d      ",
                ResultsTxDq.rank[Channel][(*RankList)[Rank].rankIndex].n,
                ResultsTxDq.rank[Channel][(*RankList)[Rank].rankIndex].p
                );
            }
            if (!Host->RmtFlags.Bits.SkipRmtTxVref) {
              RcDebugPrint (SDBG_DEFAULT, "%3d      %3d      ",
                ResultsTxVref.rank[Channel][(*RankList)[Rank].rankIndex].n,
                ResultsTxVref.rank[Channel][(*RankList)[Rank].rankIndex].p
                );
            }

            RcDebugPrint (SDBG_DEFAULT, "\n");

          } else if (IsMemFlowEnabled (CmdVrefCenteringTraining) == TRUE) {
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                       "   %3d      %3d      %3d      %3d      %3d      %3d      %3d      %3d      %3d      %3d      %3d      %3d      %3d      %3d\n",
              ResultsRxDq.rank[Channel][(*RankList)[Rank].rankIndex].n, ResultsRxDq.rank[Channel][(*RankList)[Rank].rankIndex].p,
              ResultsRxVref.rank[Channel][(*RankList)[Rank].rankIndex].n, ResultsRxVref.rank[Channel][(*RankList)[Rank].rankIndex].p,
              ResultsTxDq.rank[Channel][(*RankList)[Rank].rankIndex].n, ResultsTxDq.rank[Channel][(*RankList)[Rank].rankIndex].p,
              ResultsTxVref.rank[Channel][(*RankList)[Rank].rankIndex].n, ResultsTxVref.rank[Channel][(*RankList)[Rank].rankIndex].p,
              ResultsCmd.rank[Channel][(*RankList)[Rank].rankIndex].n, ResultsCmd.rank[Channel][(*RankList)[Rank].rankIndex].p,
              ResultsCmdVref.rank[Channel][(*RankList)[Rank].rankIndex].n, ResultsCmdVref.rank[Channel][(*RankList)[Rank].rankIndex].p,
              ResultsCtl.rank[Channel][(*RankList)[Rank].rankIndex].n, ResultsCtl.rank[Channel][(*RankList)[Rank].rankIndex].p);
          } else {
           RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                       "   %3d      %3d      %3d      %3d      %3d      %3d      %3d      %3d      %3d      %3d      %3d      %3d\n",
              ResultsRxDq.rank[Channel][(*RankList)[Rank].rankIndex].n, ResultsRxDq.rank[Channel][(*RankList)[Rank].rankIndex].p,
              ResultsRxVref.rank[Channel][(*RankList)[Rank].rankIndex].n, ResultsRxVref.rank[Channel][(*RankList)[Rank].rankIndex].p,
              ResultsTxDq.rank[Channel][(*RankList)[Rank].rankIndex].n, ResultsTxDq.rank[Channel][(*RankList)[Rank].rankIndex].p,
              ResultsTxVref.rank[Channel][(*RankList)[Rank].rankIndex].n, ResultsTxVref.rank[Channel][(*RankList)[Rank].rankIndex].p,
              ResultsCmd.rank[Channel][(*RankList)[Rank].rankIndex].n, ResultsCmd.rank[Channel][(*RankList)[Rank].rankIndex].p,
              ResultsCtl.rank[Channel][(*RankList)[Rank].rankIndex].n, ResultsCtl.rank[Channel][(*RankList)[Rank].rankIndex].p);
          }
        } // Rank loop
      } // Dimm loop
    } // Channel loop



    if (Setup->mem.optionsExt & PER_BIT_MARGINS && !Host->RmtFlags.Bits.SkipPerBitMargin) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Get per Bit margins:\n");
      if (!Host->RmtFlags.Bits.SkipRmtRxDqs) {
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "RxDqs\n");
        ClearBitResults(Host, Socket, &ResultsBit);
        GetMargins (Host, Socket, DdrLevel, RxDqsDelay, MODE_VIC_AGG, SCOPE_BIT, &Filter, (VOID *)&ResultsBit,
          Setup->mem.rmtPatternLength, 0, 0, 0, WDB_BURST_LENGTH, RmtDqTest, NULL);
        DisplayPerBitMargins (Host, Socket, &ResultsBit, RxDqsDelay, DdrLevel);
      }
      if (!Host->RmtFlags.Bits.SkipRmtTxDq) {
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "TxDq\n");
        ClearBitResults(Host, Socket, &ResultsBit);
        GetMargins (Host, Socket, DdrLevel, TxDqDelay, MODE_VIC_AGG, SCOPE_BIT, &Filter, (VOID *)&ResultsBit,
          Setup->mem.rmtPatternLength, 0, 0, 0, WDB_BURST_LENGTH, RmtDqTest, NULL);
        DisplayPerBitMargins (Host, Socket, &ResultsBit, TxDqDelay, DdrLevel);
      }

      if (!Host->RmtFlags.Bits.SkipRmtRxVref) {
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "RxVref\n");
        ClearBitResults(Host, Socket, &ResultsBit);
        GetMargins (Host, Socket, DdrLevel, RxVref, MODE_VIC_AGG, SCOPE_BIT, &Filter, (VOID *)&ResultsBit,
          Setup->mem.rmtPatternLength, 0, 0, 0, WDB_BURST_LENGTH, RmtDqTest, NULL);
        DisplayPerBitMargins (Host, Socket, &ResultsBit, RxVref, DdrLevel);
      }

      if (!Host->RmtFlags.Bits.SkipRmtTxVref) {
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "TxVref\n");
        ClearBitResults(Host, Socket, &ResultsBit);
        GetMargins (Host, Socket, DdrLevel, TxVref, MODE_VIC_AGG, SCOPE_BIT, &Filter, (VOID *)&ResultsBit,
          Setup->mem.rmtPatternLength, 0, 0, 0, WDB_BURST_LENGTH, RmtDqTest, NULL);
        DisplayPerBitMargins (Host, Socket, &ResultsBit, TxVref, DdrLevel);
      }
    } // if per Bit margins

    //------------------------------------------
    // DDR4 RDIMM RCD Rev 2 backside Cmd, Ctl margins
    if (IsDdr4BacksideCmdMarginEnabled (Host, Socket)) {
      //
      // Display units for each parameter
      //
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "\n\nCmd = 1/32nd QCLK\nCtl = 1/32nd QCLK\nCmdVref = 10 mV\nCmdVref FMC = 6.25mV\n\n");
#ifdef DEBUG_CODE_BLOCK
      if (GetDebugLevel () & SDBG_MINMAX) {
        RcDebugPrint (SDBG_DEFAULT, "\nRCD Rev2 BACKSIDE         Cmd-   Cmd+   Ctl-   Ctl+  CmdV-  CmdV+");
        RcDebugPrintLine (SDBG_DEFAULT, 70, SINGLE_LINE, CRLF_FLAG, CRLF_FLAG);
      }
#endif // DEBUG_CODE_BLOCK
      //
      // Loop for each channel
      //
      for (Channel = 0; Channel < MaxChDdr; Channel++) {
        if ((*ChannelNvList)[Channel].enabled == 0) {
          continue;
        }
        DimmNvList = GetDimmNvList(Host, Socket, Channel);
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
          //
          // Skip Aep dimm but NgnBcomMargining is disabled
          //
          if (((*DimmNvList)[Dimm].DcpmmPresent == 1) && (Setup->mem.enableNgnBcomMargining == 0)) {
            continue;
          }

          RankList = GetRankNvList(Host, Socket, Channel, Dimm);
          for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
            //
            // Skip if no Rank
            //
            if (CheckRank(Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
              continue;
            }

            for (SubRank = 0; SubRank < Host->var.mem.maxSubRank; SubRank++) {
              if (((*ChannelNvList)[Channel].encodedCSMode == 0) && (SubRank > 0)) {
                continue;
              }

              if (ResultsBacksideSubRankCmd.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n >
                  ResultsBacksideSubRankCmd.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p) {
                ResultsBacksideSubRankCmd.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
                ResultsBacksideSubRankCmd.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p = 0;
            }

              if (ResultsBacksideSubRankCtl.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n >
                  ResultsBacksideSubRankCtl.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p) {
                ResultsBacksideSubRankCtl.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
                ResultsBacksideSubRankCtl.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p = 0;
              }

              if (ResultsBacksideSubRankCaVref.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n >
                  ResultsBacksideSubRankCaVref.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p) {
                ResultsBacksideSubRankCaVref.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
                ResultsBacksideSubRankCaVref.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p = 0;
              }

              RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                            "             %3d    %3d    %3d    %3d    %3d    %3d\n",
              ResultsBacksideSubRankCmd.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n,
              ResultsBacksideSubRankCmd.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p,
              ResultsBacksideSubRankCtl.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n,
              ResultsBacksideSubRankCtl.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p,
              ResultsBacksideSubRankCaVref.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n,
              ResultsBacksideSubRankCaVref.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p);
            } //subrank
          } //Rank
        } //Dimm
      } //Channel

      if (Setup->mem.optionsExt & PER_BIT_MARGINS) {

        for (SubRank = 0; SubRank < Host->var.mem.maxSubRank; SubRank++) {
          Host->var.mem.currentSubRank = SubRank;
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\nRCD Rev2 BS CMD per Bit\n");
          ClearBitResults(Host, Socket, &ResultsBit);
          GetMargins (Host, Socket, LrbufLevel, CmdAll, MODE_VA_DESELECT | MODE_START_ZERO, SCOPE_BIT, &Filter, (VOID *)&ResultsBit,
            Setup->mem.rmtPatternLengthExt, 1, 0, 0, WDB_BURST_LENGTH, RmtCmdTest, NULL);
          DisplayPerBitMargins (Host, Socket, &ResultsBit, CmdAll, LrbufLevel);

          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\nRCD Rev2 BS CTL per Bit\n");
          ClearBitResults(Host, Socket, &ResultsBit);
          GetMargins (Host, Socket, LrbufLevel, CtlAll, MODE_VA_DESELECT | MODE_START_ZERO, SCOPE_BIT, &Filter, (VOID *)&ResultsBit,
            Setup->mem.rmtPatternLengthExt, 1, 0, 0, WDB_BURST_LENGTH, RmtCmdTest, NULL);
          DisplayPerBitMargins (Host, Socket, &ResultsBit, CtlAll, LrbufLevel);
        }
        Host->var.mem.currentSubRank = 0;
      } //per Bit
    } // DDR4 RDIMM RCD rev 2 backside
    //------------------------------------------

#ifdef LRDIMM_SUPPORT
    if ((Host->nvram.mem.socket[Socket].lrDimmPresent) && (Setup->mem.enableBacksideRMT)) {
      //
      // Display units for each parameter
      //
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "\n\nRxDqs = 1/32nd QCLK\nRxVref = 7.8 mV\nTxDq = 1/32nd QCLK\nTxVref = 7.8 mV\n");
#ifdef DEBUG_CODE_BLOCK
      if (GetDebugLevel () & SDBG_MINMAX) {
        RcDebugPrint (SDBG_DEFAULT, "\nLRDIMM BACKSIDE      RxDqs-   RxDqs+     RxV-     RxV+    TxDq-    TxDq+     TxV-     TxV+");
        RcDebugPrintLine (SDBG_DEFAULT, 91, SINGLE_LINE, CRLF_FLAG, CRLF_FLAG);
      }
#endif // DEBUG_CODE_BLOCK
      //
      // Loop for each channel
      //
      for (Channel = 0; Channel < MaxChDdr; Channel++) {
        if ((*ChannelNvList)[Channel].enabled == 0) {
          continue;
        }
        DimmNvList = GetDimmNvList(Host, Socket, Channel);
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
          if (!IsLrdimmPresent (Socket, Channel, Dimm)) {
            continue;
          }
          RankList = GetRankNvList(Host, Socket, Channel, Dimm);
          for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
            //
            // Skip if no Rank
            //
            if (CheckRank(Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
              continue;
            }

            for (SubRank = 0; SubRank < Host->var.mem.maxSubRank; SubRank++) {
              if (((*ChannelNvList)[Channel].encodedCSMode == 0) && (SubRank > 0)) {
                continue;
              }
              if ((((*DimmNvList)[Dimm].SPDDeviceType & SPD_SIGNAL_LOADING) == SPD_3DS_TYPE) && (SubRank > 0)) {
                continue;
              }
              if (ResultsBacksideSubRankRxDq.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n >
                  ResultsBacksideSubRankRxDq.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p) {
                ResultsBacksideSubRankRxDq.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
                ResultsBacksideSubRankRxDq.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p = 0;
              }

              if (ResultsBacksideSubRankRxVref.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n >
                  ResultsBacksideSubRankRxVref.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p) {
                ResultsBacksideSubRankRxVref.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
                ResultsBacksideSubRankRxVref.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p = 0;
              }

              if (ResultsBacksideSubRankTxDq.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n >
                  ResultsBacksideSubRankTxDq.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p) {
                ResultsBacksideSubRankTxDq.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
                ResultsBacksideSubRankTxDq.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p = 0;
              }

              if (ResultsBacksideSubRankTxVref.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n >
                  ResultsBacksideSubRankTxVref.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p) {
                ResultsBacksideSubRankTxVref.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n = 0;
                ResultsBacksideSubRankTxVref.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p = 0;
              }

              RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                             "           %3d      %3d      %3d      %3d      %3d      %3d      %3d      %3d\n",
              ResultsBacksideSubRankRxDq.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n,
              ResultsBacksideSubRankRxDq.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p,
              ResultsBacksideSubRankRxVref.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n,
              ResultsBacksideSubRankRxVref.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p,
              ResultsBacksideSubRankTxDq.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n,
              ResultsBacksideSubRankTxDq.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p,
              ResultsBacksideSubRankTxVref.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].n,
              ResultsBacksideSubRankTxVref.subRank[SubRank].rank[Channel][(*RankList)[Rank].rankIndex].p);
            } //subrank
          } //Rank
        } //Dimm
      } //Channel

      if (Setup->mem.optionsExt & PER_BIT_MARGINS) {
        for (SubRank = 0; SubRank < Host->var.mem.maxSubRank; SubRank++) {
          Host->var.mem.currentSubRank = SubRank;
          RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "RxDqs\n");
          ClearBitResults(Host, Socket, &ResultsBit);
          GetMargins (Host, Socket, LrbufLevel, RxDqsDelay, MODE_VIC_AGG, SCOPE_BIT, &Filter, (VOID *)&ResultsBit,
            Setup->mem.rmtPatternLength, 0, 0, 0, WDB_BURST_LENGTH, RmtDqTest, NULL);
          DisplayPerBitMargins (Host, Socket, &ResultsBit, RxDqsDelay, LrbufLevel);

          RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "TxDq\n");
          ClearBitResults(Host, Socket, &ResultsBit);
          GetMargins (Host, Socket, LrbufLevel, TxDqDelay, MODE_VIC_AGG, SCOPE_BIT, &Filter, (VOID *)&ResultsBit,
            Setup->mem.rmtPatternLength, 0, 0, 0, WDB_BURST_LENGTH, RmtDqTest, NULL);
          DisplayPerBitMargins (Host, Socket, &ResultsBit, TxDqDelay, LrbufLevel);

          RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "RxVref\n");
          ClearBitResults(Host, Socket, &ResultsBit);
          GetMargins (Host, Socket, LrbufLevel, RxVref, MODE_VIC_AGG, SCOPE_BIT, &Filter, (VOID *)&ResultsBit,
            Setup->mem.rmtPatternLength, 0, 0, 0, WDB_BURST_LENGTH, RmtDqTest, NULL);
          DisplayPerBitMargins (Host, Socket, &ResultsBit, RxVref, LrbufLevel);

          RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "TxVref\n");
          ClearBitResults(Host, Socket, &ResultsBit);
          GetMargins (Host, Socket, LrbufLevel, TxVref, MODE_VIC_AGG, SCOPE_BIT, &Filter, (VOID *)&ResultsBit,
            Setup->mem.rmtPatternLength, 0, 0, 0, WDB_BURST_LENGTH, RmtDqTest, NULL);
          DisplayPerBitMargins (Host, Socket, &ResultsBit, TxVref, LrbufLevel);
        }
        Host->var.mem.currentSubRank = 0;
      } // if per Bit margins
    } //ddr4 lrdimm
#endif // LRDIMM_SUPPORT

#ifdef DEBUG_CODE_BLOCK
    if (GetDebugLevel () & SDBG_MEM_TRAIN) {
      RcDebugPrint (SDBG_DEFAULT, "STOP_RMT_N%d\n", Socket);
      ReleasePrintControl ();
    }
#endif // DEBUG_CODE_BLOCK
  } // DllTest loop

#ifdef DEBUG_CODE_BLOCK
  if ((GetDebugLevel () & SDBG_MINMAX) && (Setup->mem.options & DISPLAY_EYE_EN)) {

    if ((GetDebugLevel () & SDBG_MINMAX) && (Setup->mem.options & PER_NIBBLE_EYE_EN)) {

      for (Strobe = 0; Strobe < MSVx4; Strobe++) {
        // Clear the DataMask
        ZeroMem (DataMask, MSVx8);
        // Select the nibble
        // Strobe 0  - dq[3:0]
        // Strobe 9  - dq[7:4]
        // Strobe 1  - dq[11:8]
        // Strobe 10 - dq[15:12]
        // etc...
        if (Strobe >= MSVx8) {
          DataMask[Strobe - MSVx8] = 0x0F;
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                        "** Eyes for DQ[%d:%d] **\n", (Strobe-MSVx8)*8+7, (Strobe-MSVx8)*8+4);
        } else {
          DataMask[Strobe] = 0xF0;
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                        "** Eyes for DQ[%d:%d] **\n", Strobe*8+3, Strobe*8);
        }

        // Run the test
        EyeDiagram(Host, Socket, DdrLevel, RxDqsDelay, MODE_VIC_AGG, SCOPE_RANK, PcdGet32 (RmtPatternLengthCmd), 0, 0, DataMask);
        EyeDiagram(Host, Socket, DdrLevel, TxDqDelay, MODE_VIC_AGG, SCOPE_RANK, PcdGet32 (RmtPatternLengthCmd), 0, 0, DataMask);

#ifdef LRDIMM_SUPPORT
        if ((Host->nvram.mem.socket[Socket].lrDimmPresent) && (Setup->mem.enableBacksideRMT)) {
          for (SubRank = 0; SubRank < Host->var.mem.maxSubRank; SubRank++) {
            Host->var.mem.currentSubRank = SubRank;
            EyeDiagram(Host, Socket, LrbufLevel, RxDqsDelay, MODE_VIC_AGG, SCOPE_RANK, PcdGet32 (RmtPatternLengthCmd), 0, 0, DataMask);
            EyeDiagram(Host, Socket, LrbufLevel, TxDqDelay, MODE_VIC_AGG, SCOPE_RANK, PcdGet32 (RmtPatternLengthCmd), 0, 0, DataMask);
          }
          Host->var.mem.currentSubRank = 0;
        }
#endif // LRDIMM_SUPPORT

        //------------------------------------------
        // DDR4 RDIMM RCD Rev 2 backside Cmd, Ctl margins
        if (IsDdr4BacksideCmdMarginEnabled (Host, Socket)) {
          for (SubRank = 0; SubRank < Host->var.mem.maxSubRank; SubRank++) {
            Host->var.mem.currentSubRank = SubRank;
            EyeDiagram(Host, Socket, LrbufLevel, CmdAll, MODE_VIC_AGG, SCOPE_RANK, PcdGet32 (RmtPatternLengthCmd), 0, 0, DataMask);
          }
          Host->var.mem.currentSubRank = 0;
        }
      } // Strobe loop

    } else { //not PER_NIBBLE_EYE_EN

      // Clear the DataMask
      ZeroMem (DataMask, MSVx8);

      EyeDiagram(Host, Socket, DdrLevel, RxDqsDelay, MODE_VIC_AGG, SCOPE_RANK, PcdGet32 (RmtPatternLengthCmd), 0, 0, DataMask);
      //EyeDiagram(Host, Socket, DdrLevel, RxDqsPDelay, MODE_VIC_AGG, SCOPE_RANK, PcdGet32 (RmtPatternLengthCmd), 0, 0, DataMask);
      //EyeDiagram(Host, Socket, DdrLevel, RxDqsNDelay, MODE_VIC_AGG, SCOPE_RANK, PcdGet32 (RmtPatternLengthCmd), 0, 0, DataMask);
      EyeDiagram(Host, Socket, DdrLevel, TxDqDelay, MODE_VIC_AGG, SCOPE_RANK, PcdGet32 (RmtPatternLengthCmd), 0, 0, DataMask);

#ifdef LRDIMM_SUPPORT
      if ((Host->nvram.mem.socket[Socket].lrDimmPresent) && (Setup->mem.enableBacksideRMT)) {
        for (SubRank = 0; SubRank < Host->var.mem.maxSubRank; SubRank++) {
          Host->var.mem.currentSubRank = SubRank;
          EyeDiagram(Host, Socket, LrbufLevel, RxDqsDelay, MODE_VIC_AGG, SCOPE_RANK, PcdGet32 (RmtPatternLengthCmd), 0, 0, DataMask);
          EyeDiagram(Host, Socket, LrbufLevel, TxDqDelay, MODE_VIC_AGG, SCOPE_RANK, PcdGet32 (RmtPatternLengthCmd), 0, 0, DataMask);
        }
        Host->var.mem.currentSubRank = 0;
      }
#endif // LRDIMM_SUPPORT

      //------------------------------------------
      // DDR4 RDIMM RCD Rev 2 backside Cmd, Ctl margins
      if (IsDdr4BacksideCmdMarginEnabled (Host, Socket)) {
        for (SubRank = 0; SubRank < Host->var.mem.maxSubRank; SubRank++) {
          Host->var.mem.currentSubRank = SubRank;
          EyeDiagram(Host, Socket, LrbufLevel, CmdAll, MODE_VIC_AGG, SCOPE_RANK, PcdGet32 (RmtPatternLengthCmd), 0, 0, DataMask);
        }
        Host->var.mem.currentSubRank = 0;
      }
    }
  }
#endif // DEBUG_CODE_BLOCK
  Host->var.mem.runningRmt = 0;

#ifdef DEBUG_CODE_BLOCK
  if (Setup->common.DfxPpvEnabled == DFX_PPV_ENABLED) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "DBG:END %s()\n", __FUNCTION__);
    SetDebugLevel (OriginalSerialDebugMsgLvl);
  }
#endif // DEBUG_CODE_BLOCK

  return SUCCESS;
} // RankMarginTool


#ifdef DDR5_SUPPORT

/*

  Print setting related with RMT flags

  @param[in]   Host    SysHost structure pointer

*/
VOID
PrintRmtFlags (
  IN  PSYSHOST  Host
  )
{

  RcDebugPrint (SDBG_MINMAX, "EnableShortFormat        : %d\n", Host->RmtFlags.Bits.EnableShortFormat);
  RcDebugPrint (SDBG_MINMAX, "SkipDisplayPerBitEyes    : %d\n", Host->RmtFlags.Bits.SkipDisplayPerBitEyes);
  RcDebugPrint (SDBG_MINMAX, "SkipDisplayPerBitMargins : %d\n", Host->RmtFlags.Bits.SkipDisplayPerBitMargins);
  RcDebugPrint (SDBG_MINMAX, "SkipPerBitMargin         : %d\n", Host->RmtFlags.Bits.SkipPerBitMargin);
  RcDebugPrint (SDBG_MINMAX, "SkipRmtRxDqs             : %d\n", Host->RmtFlags.Bits.SkipRmtRxDqs);
  RcDebugPrint (SDBG_MINMAX, "SkipRmtRxVref            : %d\n", Host->RmtFlags.Bits.SkipRmtRxVref);
  RcDebugPrint (SDBG_MINMAX, "SkipRmtTxDq              : %d\n", Host->RmtFlags.Bits.SkipRmtTxDq);
  RcDebugPrint (SDBG_MINMAX, "SkipRmtTxVref            : %d\n", Host->RmtFlags.Bits.SkipRmtTxVref);

}

/*

  Configures all Ranks, and Dimms  across all channels with the TAP settings specified as input parameters

  @param[in]   Host         SysHost structure pointer
  @param[in]   Socket       Socket under test (0-based)
  @param[in]   Iteration    Current Iteration number
  @param[in]   DfeGainBias  DFE Gain Bias
  @param[in]   Tap1         DFE Tap1 settings
  @param[in]   Tap2         DFE Tap2 settings
  @param[in]   Tap3         DFE Tap3 settings
  @param[in]   Tap4         DFE Tap4 settings

*/
VOID
SetTapSettings (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT32    Iteration,
  IN  INT8      DfeGainBias,
  IN  INT8      Tap1,
  IN  INT8      Tap2,
  IN  INT8      Tap3,
  IN  INT8      Tap4
)
{
  UINT8  Dq;
  UINT8  DqOffset;
  UINT8  Dimm;
  UINT8  Rank;
  UINT8  Ch;
  struct channelNvram (*ChannelNvList) [MAX_CH];
  struct dimmNvram (*DimmNvList) [MAX_DIMM];

  DDR5_MODE_REGISTER_128_STRUCT DfeGainBiasGenericRegister;
  DDR5_MODE_REGISTER_129_STRUCT TapGenericRegister;

  RcDebugPrint (
    SDBG_MINMAX,
    "Iteration #%4d Tap1: %02d Tap2: %02d Tap3: %02d Tap4: %02d\n",
    Iteration,
    Tap1,
    Tap2,
    Tap3,
    Tap4
    );

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      for (Ch = 0; Ch < MAX_CH; Ch += 2) {

        if ((*ChannelNvList) [Ch].enabled == 0) {
          continue;
        }

        DimmNvList = GetDimmNvList (Host, Socket, Ch);

        if ((*DimmNvList) [Dimm].dimmPresent == 0) {
          continue;
        }

        if ((*DimmNvList) [Dimm].DcpmmPresent == 1) {
          continue;
        }

        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }

        for (Dq = 0; Dq < 8; Dq++) {

          DqOffset = 0x8*Dq;

          //
          // DFE Gain Bias Mode Register
          //
          DfeGainBiasGenericRegister.Data = 0;
          DfeGainBiasGenericRegister.Bits.dfe_gain_bias = ABS (DfeGainBias);
          DfeGainBiasGenericRegister.Bits.sign_bit_gain_bias = IS_NEG (DfeGainBias);
          DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_128_REG + DqOffset, DfeGainBiasGenericRegister.Data);
          //
          // Tap 1
          //
          TapGenericRegister.Data = 0;
          TapGenericRegister.Bits.enable_disable_dfe_tap_1 = 1;
          TapGenericRegister.Bits.dfe_tap_1_bias = ABS (Tap1);
          TapGenericRegister.Bits.sign_bit_dfe_tap_1_bias = IS_NEG (Tap1);
          DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_129_REG + DqOffset, TapGenericRegister.Data);
          //
          // Tap 2
          //
          TapGenericRegister.Data = 0;
          TapGenericRegister.Bits.enable_disable_dfe_tap_1 = 1;
          TapGenericRegister.Bits.dfe_tap_1_bias = ABS (Tap2);
          TapGenericRegister.Bits.sign_bit_dfe_tap_1_bias = IS_NEG (Tap2);
          DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_130_REG + DqOffset, TapGenericRegister.Data);
          //
          // Tap 3
          //
          TapGenericRegister.Data = 0;
          TapGenericRegister.Bits.enable_disable_dfe_tap_1 = 1;
          TapGenericRegister.Bits.dfe_tap_1_bias = ABS (Tap3);
          TapGenericRegister.Bits.sign_bit_dfe_tap_1_bias = IS_NEG (Tap3);
          DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_131_REG + DqOffset, TapGenericRegister.Data);
          //
          // Tap 4
          //
          TapGenericRegister.Data = 0;
          TapGenericRegister.Bits.enable_disable_dfe_tap_1 = 1;
          TapGenericRegister.Bits.dfe_tap_1_bias = ABS (Tap4);
          TapGenericRegister.Bits.sign_bit_dfe_tap_1_bias = IS_NEG (Tap4);
          DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_132_REG + DqOffset, TapGenericRegister.Data);
        }

      } //Ch
    } //Rank loop
  }//Dimm loop

}

#endif // #ifdef DDR5_SUPPORT

/*

 DFE path finding algorithm to analyze the behavior of each TAP setting

 @param[in]   Host    SysHost structure pointer

 @retval      SUCCESS Indicate proper execution

*/
UINT32
EFIAPI
DfePathFinding (
  IN PSYSHOST Host
  )
{
#ifdef DDR5_SUPPORT
  UINT8   Socket;
  INT8    Tap1Index;
  INT8    Tap2Index;
  INT8    Tap3Index;
  INT8    Tap4Index;
  UINT32  Iteration;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  Socket = Host->var.mem.currentSocket;

  RcDebugPrint (SDBG_MINMAX, "  EnableTapSweep = %d\n", Setup->mem.EnableTapSweep);
  RcDebugPrint (SDBG_MINMAX, "  DfeGainBias = %d\n", Setup->mem.DfeGainBias);

  RcDebugPrint (SDBG_MINMAX, "  Tap1Start  = %d\n", Setup->mem.Tap1Start);
  RcDebugPrint (SDBG_MINMAX, "  Tap1End    = %d\n", Setup->mem.Tap1End  );
  RcDebugPrint (SDBG_MINMAX, "  Tap1Size   = %d\n", Setup->mem.Tap1Size );

  RcDebugPrint (SDBG_MINMAX, "  Tap2Start  = %d\n", Setup->mem.Tap2Start);
  RcDebugPrint (SDBG_MINMAX, "  Tap2End    = %d\n", Setup->mem.Tap2End  );
  RcDebugPrint (SDBG_MINMAX, "  Tap2Size   = %d\n", Setup->mem.Tap2Size );

  RcDebugPrint (SDBG_MINMAX, "  Tap3Start  = %d\n", Setup->mem.Tap3Start);
  RcDebugPrint (SDBG_MINMAX, "  Tap3End    = %d\n", Setup->mem.Tap3End  );
  RcDebugPrint (SDBG_MINMAX, "  Tap3Size   = %d\n", Setup->mem.Tap3Size );

  RcDebugPrint (SDBG_MINMAX, "  Tap4Start  = %d\n", Setup->mem.Tap4Start);
  RcDebugPrint (SDBG_MINMAX, "  Tap4End    = %d\n", Setup->mem.Tap4End  );
  RcDebugPrint (SDBG_MINMAX, "  Tap4Size   = %d\n", Setup->mem.Tap4Size );


  if (!IsSocketEnabled (Socket)) {
    return SUCCESS;
  }

  if (!Setup->mem.EnableTapSweep) {
    return SUCCESS;
  }

  //
  // Default settings for RMT flags
  //
  Host->RmtFlags.Data = 0;
  Host->RmtFlags.Bits.EnableShortFormat         = 1;
  Host->RmtFlags.Bits.SkipDisplayPerBitEyes     = 1;
  Host->RmtFlags.Bits.SkipDisplayPerBitMargins  = 0;
  Host->RmtFlags.Bits.SkipPerBitMargin          = 0;
  Host->RmtFlags.Bits.SkipRmtRxDqs              = 1;
  Host->RmtFlags.Bits.SkipRmtRxVref             = 1;
  Host->RmtFlags.Bits.SkipRmtTxDq               = 0;
  Host->RmtFlags.Bits.SkipRmtTxVref             = 0;

  PrintRmtFlags (Host);

  Iteration = 0;
  for ( Tap1Index = Setup->mem.Tap1Start;
        Tap1Index <= Setup->mem.Tap1End;
        Tap1Index += Setup->mem.Tap1Size) {
    for ( Tap2Index = Setup->mem.Tap2Start;
          Tap2Index <= Setup->mem.Tap2End;
          Tap2Index += Setup->mem.Tap2Size) {
      for ( Tap3Index = Setup->mem.Tap3Start;
            Tap3Index <= Setup->mem.Tap3End;
            Tap3Index += Setup->mem.Tap3Size) {
        for ( Tap4Index = Setup->mem.Tap4Start;
              Tap4Index <= Setup->mem.Tap4End;
              Tap4Index += Setup->mem.Tap4Size) {

          SetTapSettings (
            Host,
            Socket,
            Iteration,
            Setup->mem.DfeGainBias,
            Tap1Index,
            Tap2Index,
            Tap3Index,
            Tap4Index
            );

          RankMarginTool (Host);

          Iteration++;
        } // Tap4Index loop ...
      } // Tap3Index loop ...
    } // Tap2Index loop ...
  } // Tap1Index loop ...

#endif // #ifdef DDR5_SUPPORT
  return SUCCESS;
}

