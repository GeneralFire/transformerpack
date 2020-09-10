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
#include <Register/ArchitecturalMsr.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>

#include <Memory/MemoryCheckpointCodes.h>

UINT8
CmdVrefQuick (
  PSYSHOST  Host,
  UINT8     socket,
  MRC_GT    group
  )
/*++

Routine Description:

  Quick (1D) centering of Cmd Vref

Arguments:

  Host    - Pointer to sysHost
  socket  - Socket number
  group   - CmdVref

Returns:

  Success or failure

--*/
{
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT16              mode;
  UINT32              patternLength;
  INT16               compLow,compHigh;
  struct bitMask      filter;
  struct rankMargin   resultsRank;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  UINT8               rankPerChannel;
  INT16               Ch_Offset[MAX_CH];
  INT16               Aep_Offset[MAX_CH];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    // since we print all dimms at the end of the training step, we need to initialize the arrays here
    Ch_Offset[ch] = 0;
    Aep_Offset[ch] = 0;
  } // ch loop

  patternLength = 64;
  // mode = MODE_VIC_AGG;
  mode = MODE_VA_DESELECT;

  // Setup bitMask
  for (ch = 0; ch < MaxChDdr; ch++) {
    for (rankPerChannel = 0; rankPerChannel < MAX_RANK_CH; rankPerChannel++ ) {
      resultsRank.rank[ch][rankPerChannel].n = 0;
      resultsRank.rank[ch][rankPerChannel].p = 0;
    }
  } // ch loop

  //
  // Clear the filter bits to enable error checking on every bit
  //
  ClearFilter(Host, socket, &filter);

  SetAepTrainingMode (Host, socket, CHECKPOINT_MINOR_CMD_VREF_CENTERING);

  GetMargins (Host, socket, DdrLevel, group, mode, SCOPE_RANK, &filter, (VOID *)&resultsRank, patternLength, 0, 0, 0,
    WDB_BURST_LENGTH, LateCmdTest, NULL);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    compLow = -UNMARGINED_CMD_EDGE;
    compHigh = UNMARGINED_CMD_EDGE;

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      rankList = GetRankNvList(Host, socket, ch, dimm);
      for (rank = 0; rank < (*channelNvList)[ch].dimmList[dimm].numRanks; rank++) {
        if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        if (resultsRank.rank[ch][(*rankList)[rank].rankIndex].n > resultsRank.rank[ch][(*rankList)[rank].rankIndex].p) {
          resultsRank.rank[ch][(*rankList)[rank].rankIndex].n = 0;
          resultsRank.rank[ch][(*rankList)[rank].rankIndex].p = 0;
        }
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                      "Margin: %4d to %4d\n",
                      resultsRank.rank[ch][(*rankList)[rank].rankIndex].n, resultsRank.rank[ch][(*rankList)[rank].rankIndex].p);

#ifdef DDRT_SUPPORT
        //if NVMDIMM, set VREF in DIMM
        if ((*channelNvList)[ch].dimmList[dimm].DcpmmPresent) { // AEP is treated as single-rank so it's OK for this to be inside rank loop
          Aep_Offset[ch] = (resultsRank.rank[ch][(*rankList)[rank].rankIndex].n + resultsRank.rank[ch][(*rankList)[rank].rankIndex].p) / 2;
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "PMEM Offset = %d\n", Aep_Offset[ch]);
          // Set new VREF per NVMDIMM
          GetSetCmdVref (Host, socket, ch, dimm, DdrLevel, GSM_WRITE_OFFSET | GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &Aep_Offset[ch]);
        }
#endif // DDRT_SUPPORT
        // if any other kind of dimm store the results for composite channel
        if ((*channelNvList)[ch].dimmList[dimm].DcpmmPresent == 0) {
          if (resultsRank.rank[ch][(*rankList)[rank].rankIndex].n > compLow) {
            compLow = resultsRank.rank[ch][(*rankList)[rank].rankIndex].n;
          }
          if (resultsRank.rank[ch][(*rankList)[rank].rankIndex].p < compHigh) {
            compHigh = resultsRank.rank[ch][(*rankList)[rank].rankIndex].p;
          }
        }
      } // rank loop
    } // dimm loop

    //
    // Set new VREF per channel
    //
    if ((compLow != -UNMARGINED_CMD_EDGE) && (compHigh != UNMARGINED_CMD_EDGE)){

      Ch_Offset[ch] = (compLow + compHigh) / 2;
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "DDR4 Composite Low = %d, Composite High = %d, Offset = %d\n", compLow, compHigh, Ch_Offset[ch] );

      GetSetCmdVref (Host, socket, ch, 0, DdrLevel, GSM_WRITE_OFFSET | GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &Ch_Offset[ch]);
    }
  } // ch loop
  UpdateStartingOffset (Host, socket, group, DdrLevel, SCOPE_RANK, &resultsRank);

  SetAepTrainingMode (Host,socket, DISABLE_TRAINING_STEP);
  Host->nvram.mem.socket[socket].cmdVrefTrainingDone = 1;
  //
  // Reset the IO
  //
  IO_Reset (Host, socket);
  JedecInitSequence (Host, socket, CH_BITMASK);

#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    AcquirePrintControl ();
    RcDebugPrint (SDBG_DEFAULT, "START_CMD_VREF_OFFSETS\n");

    // Apply new center  SKX may be different
    for (ch = 0; ch < MaxChDdr; ch++) {

      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
#ifdef DDRT_SUPPORT
        if ((*channelNvList)[ch].dimmList[dimm].DcpmmPresent) {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            " %d\n", Aep_Offset[ch]);
        }
#endif // DDRT_SUPPORT
        if ((*channelNvList)[ch].dimmList[dimm].DcpmmPresent == 0) {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            " %d\n", Ch_Offset[ch]);
        }
      } // dimm loop
    } // ch loop

    RcDebugPrint (SDBG_DEFAULT, "STOP_CMD_VREF_OFFSETS\n");
    ReleasePrintControl ();
  }
#endif  // DEBUG_CODE_BLOCK

  return SUCCESS;
} // CmdVrefQuick

UINT8
FindIndexResData (
                  UINT8 strobe
                 )
{
  UINT8 strobeIndex;
  strobeIndex = strobe;
  while (strobeIndex > 3) {
    strobeIndex -= 4;
  }
  return strobeIndex;
} //FindIndexResData


#define GRP_MUL_NONE  0   // Map one rank to each group
#define GRP_MUL_2_1   1   // Map two ranks to each group (0,2 and 1,3 to each group on QR)
#define GRP_MUL_FULL  2   // Map all DIMM ranks to one group (Not supported)
#define PPDS_GROUP_X4 BIT15 // Indicate that this group is x4 vs. x8

/**

  This function applies RfOn setting to given channel.

  @param Host    - Pointer to sysHost
  @param socket  - Socket
  @param ch      - Channel

@retval none

**/

UINT32 CheckDimmType(
    PSYSHOST Host
    )
{
  return SUCCESS;
}

/**

 Determine if we train a given rank for a given group

  @param Host    - Pointer to sysHost
  @param socket  - Socket
  @param ch      - Channel
  @param dimm    - Dimm
  @param rank    - Rank
  @param level   - DdrLevel or LrbufLevel
  @group         - Tx/RxVref, Tx/RxDq/Dqs, etc
  @retval        - 1 = skip, 0 = train

**/
UINT8
CheckSkipRankTrain(
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    rank,
  MRC_LT   level,
  MRC_GT   group
  )
{
  UINT8               status = 0;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  SYS_SETUP           *Setup;

  Setup = GetSysSetupPointer ();

  channelNvList = GetChannelNvList(Host, socket);
  dimmNvList = GetDimmNvList(Host, socket, ch);

  if (rank > 0) {
#ifdef  LRDIMM_SUPPORT
    // only train rank0 for frontside TXVREF on LRDIMMs
    if (Host->var.mem.runningRmt == 0){
      if (IsLrdimmPresent (socket, ch, dimm) && (level == DdrLevel) && ((group == TxVref) || (group == RxVref) ||
      (group == TxDqDelay) || (group == RxDqsPDelay) || (group == RxDqsNDelay) || (group == RxDqsDelay))) {
        status = 1;
      }
    }
#endif
    // 3DS, skip logical ranks
    if (((*channelNvList)[ch].encodedCSMode == 2) && (rank > 1)) {
      status = 1;
    }

    // only train rank0 for CMD training on RDIMMs
    if (Host->nvram.mem.dimmTypePresent == RDIMM && CheckCMDGroup(group)
                && (!Host->var.mem.runningRmt) && (level == DdrLevel)) {
      status = 1;
    }

  }

  //
  // Only perform backside Cmd margining on RCD Rev2 non-DDRT dimms
  //
  if ((CheckCMDGroup (group) || CheckCTLGroup (group) || (group == CmdVref)) &&
      (level == LrbufLevel) && (Host->nvram.mem.dramType != SPD_TYPE_DDR5) &&
      (((!IsDdr4RegisterRev2 (Host, socket, ch, dimm)) && !((*dimmNvList)[dimm].DcpmmPresent)) || (((*dimmNvList)[dimm].DcpmmPresent) && !(Setup->mem.enableNgnBcomMargining)))) {
    status = 1;
  }

  if (!IsLrdimmPresent (socket, ch, dimm) && (level == LrbufLevel) && (!CheckCMDGroup(group)) && (!CheckCTLGroup(group))) {
    status = 1;
  }

  return status;
}

/**

  Checks to see if the TxDqDqs is too close to the zero encoding
  Adjusts TxDqDqs if this is the case for both directions

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - current socket
  @param[in] Direction  - DENORMALIZE (-1) or RENORMALIZE (1)
  @param[in] ExtraRange - Range to pull in

  @retval SUCCESS - Function executed successfully.

**/
UINT32
EFIAPI
WLPushOut (
  IN PSYSHOST  Host,
  IN UINT8 socket,
  IN INT8 Direction,
  IN UINT8 ExtraRange
  )
{
  UINT8               ch;
  UINT8               Dimm = 0;
  UINT8               Dimm2 = 0;
  UINT8               MaxDimms = 0;
  UINT8               rank;
  UINT8               strobe;
  BOOLEAN             Adjust[MAX_DIMM];
  INT16               PiDelay[MAX_DIMM];
  UINT16              MinDelay[MAX_DIMM];
  UINT16              MaxDelay[MAX_DIMM];
  INT16               CurCWL[MAX_DIMM];
  UINT16              MinLimit;
  UINT16              MaxLimit;
  UINT16              UsDelay;
  UINT32              PrevDebugLevel;

  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid(Host);
  UINT8 MaxChDdr;

#ifdef DEBUG_CODE_BLOCK
  if (Direction == DENORMALIZE) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "WriteLeveling Pushout\n BEFORE WL pushout values\n");
  } else {
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "WriteLeveling Pullin\n BEFORE WL pullin values\n");
  }
  DisplayResults(Host, socket, TxDqsDelay);
  DisplayResults(Host, socket, TxDqDelay);
#endif //DEBUG_CODE_BLOCK
  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, socket);

  GetDataGroupLimits (Host, DdrLevel, TxDqDelay, &MinLimit, &MaxLimit, &UsDelay);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      MinDelay[Dimm] = MaxLimit;
      MaxDelay[Dimm] = MinLimit;
      Adjust[Dimm] = TRUE;
      Host->var.mem.WrLvlDeNormalizeStatus[socket][ch][Dimm] = FALSE;
      //
      // Get current CWL
      //
      GetCwlAdj(Host, socket, ch, Dimm, &CurCWL[Dimm]);
    }

    // Read results from TxDqDqs, TxDq
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      MaxDimms = MAX_DIMM;
      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
        // If rank enabled
        if (CheckRank (Host, socket, ch, Dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        for(strobe = 0; strobe < MSVx4; strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if (IsStrobeNotValid(Host, strobe)) {
            continue;
          }
          // There are two slots per channel, which means that we could end up with multiple DIMMs of the same type in a channel.
          // The min/max for DDRT must be the same for all DDRT DIMMs, and the same goes for DDR4. So the min/max for all DIMMs
          // of the same type must be updated if any DIMM of that type is updated.
          GetSetDataGroup (Host, socket, ch, NO_SUBCH, Dimm, rank, strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, (INT16 *)&PiDelay[Dimm]);

          for (Dimm2 = 0; Dimm2 < MaxDimms; Dimm2++) {
            // If this DIMM is of some other type than the current dimm, skip it.
            if ((*channelNvList)[ch].dimmList[Dimm].DcpmmPresent != (*channelNvList)[ch].dimmList[Dimm2].DcpmmPresent){
              continue;
            }
            UpdateMinMaxInt (PiDelay[Dimm], (INT16 *)&MinDelay[Dimm2], (INT16 *)&MaxDelay[Dimm2]);
          }

          GetSetDataGroup (Host, socket, ch, NO_SUBCH, Dimm, rank, strobe, 0, DdrLevel, TxDqDelay, GSM_READ_ONLY, (INT16 *)&PiDelay[Dimm]);
          for (Dimm2 = 0; Dimm2 < MaxDimms; Dimm2++) {
            // If this DIMM is of some other type than the current dimm, skip it.
            if ((*channelNvList)[ch].dimmList[Dimm].DcpmmPresent != (*channelNvList)[ch].dimmList[Dimm2].DcpmmPresent) {
              continue;
            }
            UpdateMinMaxInt (PiDelay[Dimm], (INT16 *)&MinDelay[Dimm2], (INT16 *)&MaxDelay[Dimm2]);
          }
        } // strobe loop
      } // rank loop
      PiDelay[Dimm] = 0;
    } // dimm loop

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (Direction == DENORMALIZE) {
        //CWL by 1 or 2 clock and TX PI's by 128 or 256
        if ((CurCWL[Dimm] > -2) && (MaxDelay[Dimm] <= (MaxLimit - (2 * DCLKPITICKS) - (EXTRAPIMARGIN / 2)))) {
          CurCWL[Dimm] = CurCWL[Dimm] - 2;
          PiDelay[Dimm] = 2 * DCLKPITICKS;
          Host->var.mem.WrLvlDeNormalizeStatus[socket][ch][Dimm] = TRUE;
        } else if ((CurCWL[Dimm] > -3) && (MaxDelay[Dimm] <= (MaxLimit - DCLKPITICKS - (EXTRAPIMARGIN / 2)))) {
          CurCWL[Dimm] = CurCWL[Dimm] - 1;
          PiDelay[Dimm] = DCLKPITICKS;
          Host->var.mem.WrLvlDeNormalizeStatus[socket][ch][Dimm] = TRUE;
        } else {
          Adjust[Dimm] = FALSE;
          Host->var.mem.WrLvlDeNormalizeStatus[socket][ch][Dimm] = FALSE;
        }
        if (Adjust[Dimm]) {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Pushout to CWL_ADJ=%d and TXDq/Dqs by +%d\n", CurCWL[Dimm], PiDelay[Dimm]);
        }
      } else { // direction == RENORMALIZE
        if (((CurCWL[Dimm] <= 5) && (MinDelay[Dimm] >= 2 * DCLKPITICKS + EXTRAPIMARGIN / 2 + ExtraRange))) { // 256+16 = 272
          CurCWL[Dimm] += 2;
          PiDelay[Dimm] = -2 * DCLKPITICKS;
        } else if (((CurCWL[Dimm] <= 6) && (MinDelay[Dimm] >= DCLKPITICKS + EXTRAPIMARGIN / 2 + ExtraRange))) { // 128+16 = 144
          CurCWL[Dimm] += 1;
          PiDelay[Dimm] = -DCLKPITICKS;
        } else {
          Adjust[Dimm] = FALSE;
        }
      }
    }

    MaxDimms = MAX_DIMM;
    DimmNvList = GetDimmNvList(Host, socket, ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }
      if (Adjust[Dimm] && Host->var.mem.WrLvlDeNormalizeStatus[socket][ch][Dimm]) {
        // move CWL_ADJ
        if ((*DimmNvList)[Dimm].DcpmmPresent) {
          SetCwlAdj (Host, socket, ch, CWLADJ_DDRT, CurCWL[Dimm]);
        } else {
          SetCwlAdj (Host, socket, ch, CWLADJ_DDR4, CurCWL[Dimm]);
        }
        //
        // Explicitly write TxDqDqs and TxDq per strobe. Not use WrLvlDelay.
        //
        for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
          // If rank enabled
          if (CheckRank(Host, socket, ch, Dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          for (strobe = 0; strobe < MSVx4; strobe++) {
            //
            // Skip if this is an ECC strobe when ECC is disabled
            //
            if (IsStrobeNotValid(Host, strobe)) {
              continue;
            }
            GetSetDataGroup (Host, socket, ch, NO_SUBCH, Dimm, rank, strobe, 0, DdrLevel, TxDqsDelay, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE,(INT16 *)&PiDelay[Dimm]);
            GetSetDataGroup (Host, socket, ch, NO_SUBCH, Dimm, rank, strobe, 0, DdrLevel, TxDqDelay, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, (INT16 *)&PiDelay[Dimm]);
          } // strobe loop
        } // rank loop
      } // Adjust True
    } // dimm loop
  } // Ch loop

  PrevDebugLevel = GetDebugLevel ();
  if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
    SetDebugLevel (SDBG_ERROR);
  }
  IO_Reset (Host, socket);
  SetDebugLevel (PrevDebugLevel);
  return SUCCESS;
} // WLPushOut


VOID
DisableDDRTEridParityErrorLogging10nm (
  PSYSHOST Host,
  UINT8    socket
  )
{
  UINT64_STRUCT msrReg;
  msrReg = ReadMsrPipe (Host, socket, MSR_IA32_MC13_CTL);
  msrReg.lo |= BIT23;
  WriteMsrPipe (Host, socket, MSR_IA32_MC13_CTL, msrReg);
}
