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
#include <Library/MemMcIpLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/TimerLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/LegacyRmtLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/SimulationServicesLib.h>
#include <Memory/JedecDefinitions.h>
#include "Include/BaseMemoryCoreInternal.h"
#include <Memory/MemoryCheckpointCodes.h>
#include <Library/MemRcLib.h>
#include <Library/MemRoutingLib.h>
#include <Library/SsaBuiltInRmtLib.h>

#define SET_ALL   0xFF

//
// Local Prototypes
//
VOID   EvaluateResults (PSYSHOST Host, UINT8 socket, UINT8 dimm, UINT8 rank, UINT8 sign, INT16 offset[MAX_CH][MAX_RANK_CH],
         UINT32 chStatus, UINT8 scope, MRC_GT group, MRC_LT level, UINT16 mode, UINT8 searchUp[MAX_CH][MAX_RANK_CH], UINT32 bwSerr[][3], struct bitMask *foundPass,
         struct bitMask *foundFail, VOID *marginData, UINT32 *ranksDone, UINT8 maxSteps);
UINT8  CheckAllPassFail (PSYSHOST Host, UINT8 found[MAX_STROBE/2]);
VOID   GetStartingOffset (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, MRC_LT level, MRC_GT group, UINT8 sign, INT16 offset[MAX_CH][MAX_RANK_CH], UINT8 *maxSteps);
UINT32 *CheckRange (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, INT16 offset, UINT8 sign, MRC_LT level, MRC_GT group,
          UINT8 foundFail[MAX_STROBE/2], UINT8 foundPass[MAX_STROBE/2], UINT8 scope, INT16 previousOffset, VOID *marginData, UINT32 beyondRange[3]);
VOID   SaveThisMargin (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, UINT8 strobe, UINT8 maxStrobe, INT16 offset, UINT8 sign,
         UINT8 scope, UINT8 foundFail[MAX_STROBE/2], UINT8 foundPass[MAX_STROBE/2], VOID *marginData);
VOID   SetAllFound (PSYSHOST Host, UINT8 ch, UINT8 rank, UINT8 strobe, UINT8 bitMask, UINT8 found[MAX_CH][MAX_RANK_CH][MAX_STROBE/2]);
VOID   ClearAllFound (PSYSHOST Host, UINT8 ch, UINT8 rank, UINT8 strobe, UINT8 found[MAX_CH][MAX_RANK_CH][MAX_STROBE/2]);

VOID
EvaluateStrobeStatus (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bitMask,
  UINT32    status,
  INT16     offset,
  UINT8     sign,
  UINT8     searchUp,
  UINT8     foundPass[MAX_CH][MAX_RANK_CH][MAX_STROBE/2],
  UINT8     foundFail[MAX_CH][MAX_RANK_CH][MAX_STROBE/2],
  VOID      *marginData,
  UINT8     scope,
  UINT16    mode
  );

#define MAX_MARGIN_LIST  20
#define VREF_HI          0x5F
#define VREF_LO          -0x5F
//
// Signal to minimum margin in ticks
//
UINT8 MinimumMarginData[MAX_MARGIN_LIST][2] = {
  { RecEnDelay       , 25 },     // Receive enable pulse width minimun is 40% of the duty cycle 64 ticks ideal
  { RxDqsDelay       , 12 },
  { CtlAll           , 40 },     // Ideal is 64 ticks for CMD/CTL signals Jedec spec
  { CmdAll           , 40 },
  { TxDqsDelay       , 25 },     // 0.45 * 128 = 57.6 ticks rounded to 50 but for high pulse only would be 25 could be 28 since 45%
  { TxDqDelay        , 12 },     // 0.2*64 = 12.8 ticks rounded to 12
  { RxDqsBitDelay    , 16 },
  { RxDqsPBitDelay   , 16 },
  { RxDqsNBitDelay   , 16 },
  { TxDqBitDelay     , 16 },
  { RxVref           , 29 },
  { RxSamplerEvenOdd , 29 },
  { TxVref           , 17 },
  { RxDqsNDelay      , 12 },
  { RxDqsPDelay      , 12 },
  { CS               , 40 },
  { CA               , 12 },
  { QCS              , 20 },
  { QCA              , 20 },
  { Req              , 80 }
};

/**

  Clears GetMargins filter

  @param Host  - Pointer to sysHost
  @param socket  - Socket Id
  @param filter  - pointer to filter buffer

  @retval N/A

**/
VOID
ClearFilter (
  PSYSHOST        Host,
  UINT8           socket,
  struct bitMask  *filter
  )
{

  ZeroMem ((VOID *)filter, sizeof (struct bitMask));

} // ClearFilter

/**

  Displays per bit margin results

  @param Host  - Pointer to sysHost
  @param socket      - current socket
  @param resultsBit  - Data to compare against
  @param group       - Command group

  @retval N/A

**/
VOID
DisplayPerBitMargins (
  PSYSHOST          Host,
  UINT8             socket,
  struct bitMargin  *resultsBit,
  MRC_GT            group,
  MRC_LT            level
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8               ch;
  UINT8               bit;
  UINT8               b;
  UINT8               maxBits;
  UINT8               dimm;
  UINT8               rank;
  UINT8               i;
  UINT8               skipRow;
  INT16               margin;
  INT16               marginStart = 0;
  INT16               marginStop = 0;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8 MBV = GetMaxBitsValid (Host);
  UINT8               MaxChDdr;

  //
  // Check the debug message level
  //
  if (!(GetDebugLevel () & SDBG_MINMAX)) {
    return;
  }

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);

  //
  // Loop for each channel
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, socket, ch);

    //
    // Loop for each dimm and each rank
    //
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }

      rankList = GetRankNvList (Host, socket, ch, dimm);

      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        //
        // Skip if no rank
        //
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (CheckSkipRankTrain (Host, socket, ch, dimm, rank, level, group)) {
          continue;
        }

        RcDebugPrint (SDBG_MINMAX,
                              "Per bit margins: ");
        if (group == RxDqsDelay) {
          RcDebugPrint (SDBG_MINMAX,
                              "RxDqs\n");
          marginStart = MAX_TIMING_OFFSET - 1;
          marginStop = - (MAX_TIMING_OFFSET - 1);
        } else if (group == RxDqsPDelay) {
          RcDebugPrint (SDBG_MINMAX,
                         "Rx DqsP\n");
          marginStart = MAX_TIMING_OFFSET - 1;
          marginStop = - (MAX_TIMING_OFFSET - 1);
        } else if (group == RxDqsNDelay) {
          RcDebugPrint (SDBG_MINMAX,
                         "Rx DqsN\n");
          marginStart = MAX_TIMING_OFFSET - 1;
          marginStop = - (MAX_TIMING_OFFSET - 1);

        } else if (group == CmdAll) {
          RcDebugPrint (SDBG_MINMAX,
                         "Cmd\n");
          marginStart = MAX_TIMING_OFFSET - 1;
          marginStop = - (MAX_TIMING_OFFSET - 1);

        } else if (group == CtlAll) {
          RcDebugPrint (SDBG_MINMAX,
                         "Ctl\n");
          marginStart = MAX_TIMING_OFFSET - 1;
          marginStop = - (MAX_TIMING_OFFSET - 1);

        } else if (group == TxDqDelay) {
          RcDebugPrint (SDBG_MINMAX,
                              "TxDq\n");
          marginStart = MAX_TIMING_OFFSET - 1;
          marginStop = - (MAX_TIMING_OFFSET - 1);
        } else if (group == RxVref) {
          RcDebugPrint (SDBG_MINMAX,
                              "RxVref\n");
          marginStart = MAX_RD_VREF_OFFSET - 1;
          marginStop = - (MAX_RD_VREF_OFFSET - 1);
        } else if (group == TxVref) {
          RcDebugPrint (SDBG_MINMAX,
                              "TxVref\n");
          if (!IsMemFlowEnabled (LrdimmWriteVrefCenteringTraining)) {
            if ((*dimmNvList)[dimm].DcpmmPresent == 1) {
              marginStart = MAX_WR_VREF_OFFSET_DDRT - 1;
              marginStop = - (MAX_WR_VREF_OFFSET_DDRT - 1);
            } else {
              marginStart = MAX_WR_VREF_OFFSET_BACKSIDE - 1;
              marginStop = - (MAX_WR_VREF_OFFSET_BACKSIDE - 1);
            }
          } else {
            marginStart = MAX_WR_VREF_OFFSET - 1;
            marginStop = - (MAX_WR_VREF_OFFSET - 1);
          }
        }

        if (!Host->RmtFlags.Bits.SkipDisplayPerBitEyes) {
          RcDebugPrint (SDBG_DEFAULT, "    ");
          for (i = 0; i < MSVx8; i++) {
            if (IsStrobeNotValid (Host, i)) {
              continue;
            }
            RcDebugPrint (SDBG_DEFAULT, " %02d----%02d", i * 8, (i * 8) + 7);
          }
          RcDebugPrint (SDBG_DEFAULT, "\n");

          for (margin = marginStart; margin >= marginStop; margin--) {
            //
            // Skip row if no failures
            //
            skipRow = 1;
            for (bit = 0; bit < MBV; bit++) {
              if (IsBitNotValid (Host, bit)) {
                continue;
              }
              if (margin >= 0) {
                if (margin >= resultsBit->bits[ch][ (*rankList)[rank].rankIndex][bit].p) {
                  skipRow = 0;
                  break;
                }
              } else {
                if (margin <= resultsBit->bits[ch][ (*rankList)[rank].rankIndex][bit].n) {
                  skipRow = 0;
                  break;
                }
              }

            } // bit loop

            if (skipRow) {
              continue;
            }

            RcDebugPrint (SDBG_MINMAX,
                                "%3d ", margin);

            for (bit = 0; bit < MBV; bit++) {
              // Only display ECC bits if ECC is enabled
              if (IsBitNotValid (Host, bit)) {
                continue;
              }
              // Insert a space after every 8 bits
              if ((bit % 8) == 0) {
                RcDebugPrint (SDBG_DEFAULT, " ");
              }

              // Check for pass or fail for each bit at this margin
              if (margin >= 0) {
                if (margin >= resultsBit->bits[ch][ (*rankList)[rank].rankIndex][bit].p) {
                  RcDebugPrint (SDBG_MINMAX,
                                "*");
                } else {
                  RcDebugPrint (SDBG_MINMAX,
                                " ");
                }
              } else {
                if (margin <= resultsBit->bits[ch][ (*rankList)[rank].rankIndex][bit].n) {
                  RcDebugPrint (SDBG_MINMAX,
                                "*");
                } else {
                  RcDebugPrint (SDBG_MINMAX,
                                " ");
                }
              }
            } // bit loop

            RcDebugPrint (SDBG_MINMAX,
                          "\n");
          } // margin loop
        } // !Host->RmtFlags.Bits.SkipDisplayPerBitEyes

        if (!Host->RmtFlags.Bits.SkipDisplayPerBitMargins) {

          if (group == RxDqsDelay) {
            RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                              "RxDqs");
          } else if (group == RxDqsPDelay) {
            RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                              "RxDqsP");
          } else if (group == RxDqsNDelay) {
            RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                              "RxDqsN");
          } else if (group == TxDqDelay) {
            RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                              "TxDq");
          } else if (group == RxVref) {
            RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                              "RxVref");
          } else if (group == TxVref) {
            RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                              "TxVref");
          } else if (group == CmdAll) {
            RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                              "Cmd");
          } else if (group == CtlAll) {
            RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                              "Ctl");
          }
          RcDebugPrint (SDBG_DEFAULT, " - Per bit margins\n");

          if (Host->nvram.mem.eccEn == 0) {
            maxBits = MBV - 8;
          } else {
            maxBits = MBV;
          }
          for (bit = 0; bit < maxBits; bit++) {
            if ((bit == 0) || (bit == (maxBits / 2))) {
              for (b = bit; b < (bit + (maxBits / 2)); b++) {
                RcDebugPrint (SDBG_MINMAX,
                              "  %2d", b);
              }
              RcDebugPrint (SDBG_MINMAX,
                            "\n");
            }

            RcDebugPrint (SDBG_MINMAX,
                          " %3d", resultsBit->bits[ch][(*rankList)[rank].rankIndex][bit].p);

            if ((bit == ((maxBits / 2) - 1)) || (bit == (maxBits - 1))) {
              RcDebugPrint (SDBG_MINMAX,
                            "\n");
              for (b = bit - ((maxBits / 2) - 1); b <= bit; b++) {
                RcDebugPrint (SDBG_MINMAX,
                              " %3d", resultsBit->bits[ch][(*rankList)[rank].rankIndex][b].n);
              }
              RcDebugPrint (SDBG_MINMAX,
                            "\n\n");
            }
          } // bit loop
          RcDebugPrint (SDBG_MINMAX,
                              "\n");
        } // !Host->RmtFlags.Bits.SkipDisplayPerBitMargins
      } // rank loop
    } // dimm loop
  } // ch loop
#endif // DEBUG_CODE_BLOCK
} // DisplayPerBitMargins

VOID
DisplayREandLEMargins (
  PSYSHOST            Host,
  UINT8               socket,
  UINT8               ch,
  UINT8               dimm,
  UINT8               rank,
  struct bitMarginCh  *resultsBit,
  MRC_GT              group
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8               bit;
  UINT8               b;
  UINT8               maxBits;
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  UINT8 MBV = GetMaxBitsValid (Host);

  //
  // Check the debug message level
  //
  if (!(GetDebugLevel () & SDBG_MEM_TRAIN)) {
    return;
  }

  dimmNvList = GetDimmNvList (Host, socket, ch);

  //
  // Skip if no DIMM present
  //
  if ((*dimmNvList)[dimm].dimmPresent) {

    RcDebugPrint (SDBG_MEM_TRAIN,
                   "\n");
    if (group == RxDqsDelay) {
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                        "RxDqs");
    } else if (group == RxDqsPDelay) {
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                        "RxDqsP");
    } else if (group == RxDqsNDelay) {
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                        "RxDqsN");
    } else if (group == TxDqDelay) {
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                        "TxDq");
    } else if (group == RxVref) {
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                        "RxVref");
    } else if (group == TxVref) {
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                        "TxVref");
    }
    RcDebugPrint (SDBG_DEFAULT, " - Per bit margins\n");

    if (Host->nvram.mem.eccEn == 0) {
      maxBits = MBV - 8;
    } else {
      maxBits = MBV;
    }
    for (bit = 0; bit < maxBits; bit++) {
      if ((bit == 0) || (bit == (maxBits / 2))) {
        for (b = bit; b < (bit + (maxBits / 2)); b++) {
          RcDebugPrint (SDBG_MEM_TRAIN,
                        "  %2d", b);
        }
        RcDebugPrint (SDBG_MEM_TRAIN,
                      "\n");
      }

      RcDebugPrint (SDBG_MEM_TRAIN,
                    " %3d", resultsBit->bits[ch][bit].p);

      if ((bit == ((maxBits / 2) - 1)) || (bit == (maxBits - 1))) {
        RcDebugPrint (SDBG_MEM_TRAIN,
                      "\n");
        for (b = bit - ((maxBits / 2) - 1); b <= bit; b++) {
          RcDebugPrint (SDBG_MEM_TRAIN,
                        " %3d", resultsBit->bits[ch][b].n);
        }
        RcDebugPrint (SDBG_MEM_TRAIN,
                      "\n\n");
      }
    } // bit loop
    RcDebugPrint (SDBG_MEM_TRAIN,
                        "\n");
  }

#endif // DEBUG_CODE_BLOCK
} // DisplayREandLEMargins

/**

  Tests to see if all ranks across all memory controllers are done.

  @param RanksDone - Array of bits showing if ranks are done. If bitX = 1, the rank is done.

  @retval TRUE is ranks are all done
  @retval FALSE if any rank is not done.

**/
BOOLEAN
AreRanksDone (
  UINT32  RanksDone[MAX_IMC]
  )
{
  UINT8 i;
  UINT8 MaxImc;

  MaxImc = GetMaxImc ();
  for (i = 0; i < MaxImc; i++) {
    if (RanksDone[i] != 0xFFFFFFFF) {
      return FALSE;
    }
  }
  return TRUE;
}

/**
  This function returns TRUE when we are sweeping TxVref at a DdrLevel
  for x16 DRAM devices, if PDA mode is enabled and only in SCOPE_STROBE


  @param[in]                Host          Pointer to sysHost
  @param[in]                Socket        Socket under test ('0' based)
  @param[in]                Level         DDR, LRDIMM, Backside, etc
  @param[in]                Group         Parameter to be margined
  @param[in]                Scope         Margin Granularity

  @retval                   TRUE          When we are sweeping TxVref at a DdrLevel for x16 DRAM
                                          devices, if PDA mode is enabled and only in SCOPE_STROBE

  @retval                   FALSE         Otherwise

**/
BOOLEAN
IsTxVrefPdaX16 (
  IN        PSYSHOST        Host,
  IN        UINT8           Socket,
  IN        MRC_LT          Level,
  IN        MRC_GT          Group,
  IN        UINT8           Scope
  )
{
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  if (Host->nvram.mem.socket[Socket].x16DimmPresent &&
      Group == TxVref &&
      Level == DdrLevel &&
      (Setup->mem.optionsExt & PDA_EN) != 0 &&
      Scope == SCOPE_STROBE
      ) {
    return TRUE;
  }
  return FALSE;
}

/**
  Verifies the same TxVref for strobes within the same
  x16 DRAM device at a DdrLevel.


  @param[in]                Host          Pointer to sysHost
  @param[in]                Socket        Socket under test ('0' based)
  @param[in]                ChannelMask   Bit Mask of channels to not be used
  @param[in]                RankMask      Bit Mask of ranks to not be used
  @param[in]                Level         DDR, LRDIMM, Backside, etc
  @param[in]                Group         Parameter to be margined
  @param[in]                Scope         Margin Granularity

**/
VOID
VerifyInitialTxVrefOffsets (
  IN        PSYSHOST        Host,
  IN        UINT8           Socket,
  IN        UINT8           ChannelMask,
  IN        UINT8           RankMask,
  IN        MRC_LT          Level,
  IN        MRC_GT          Group,
  IN        UINT8           Scope
  )
{
  UINT8   Channel;
  UINT8   Dimm;
  UINT8   Rank;
  UINT8   Strobe;
  UINT8   MSVx8;
  INT16   Offset[3];
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  UINT8               MaxChDdr;

  if (!IsTxVrefPdaX16 (Host, Socket, Level, Group, Scope) ||
      !IsMemFlowEnabled (DqSwizzlingDiscoveryX16) ||
      FeaturePcdGet (PcdCosimBuild) == TRUE ||
      FeaturePcdGet (PcdCteBuild) == TRUE) {
    return;
  }

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  MSVx8 = GetMaxStrobeValid (Host) / 2;

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if ((*ChannelNvList)[Channel].enabled == 0) {
      continue;
    }
    //
    // Check if this channel is masked off
    //
    if (ChannelMask & (1 << Channel)) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Channel);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

      RankList = GetRankNvList (Host, Socket, Channel, Dimm);

      for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {
        //
        // Skip if this Rank is disabled
        //
        if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        //
        // Skip Rank training if we don't need to do it. example cmd training on RDIMM where Rank>0
        //
        if (CheckSkipRankTrain (Host, Socket, Channel, Dimm, Rank, Level, Group)) {
          continue;
        }

        //
        // Do not test this Rank if it is masked out
        //
        if (RankMask & (1 << GetLogicalRank (Host, Socket, Channel, Dimm, Rank))) {
          continue;
        }
        if (IsX16Dimm (Socket, Channel, Dimm)) {

          for (Strobe = 0; Strobe < MSVx8; Strobe++) {
            //
            // Get current delay for logical Strobe
            //
            GetSetDataGroup (
              Host, Socket, Channel, NO_SUBCH, Dimm, Rank, Strobe, ALL_BITS, Level,
              Group, GSM_READ_CSR | GSM_READ_ONLY, &Offset[0]
              );
            if ((*DimmNvList)[Dimm].LogicalX16Mapping.PairLogical[Strobe] != 0xFF) {
              //
              // Get current delay for logical PAIR Strobe
              //
              GetSetDataGroup (
                Host, Socket, Channel, NO_SUBCH, Dimm, Rank, (*DimmNvList)[Dimm].LogicalX16Mapping.PairLogical[Strobe],
                ALL_BITS, Level, Group, GSM_READ_CSR | GSM_READ_ONLY, &Offset[1]
                );
            } else {
              Offset[1] = Offset[0];
            }

            if (Offset[0] != Offset[1]) {
              RcDebugPrintWithDevice (
                                      SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                                      "\nTxVref for [%d]S=%02d, [%d]LS=%02d\n",
                                      Strobe,
                                      Offset[0],
                                      (*DimmNvList)[Dimm].LogicalX16Mapping.PairLogical[Strobe],
                                      Offset[1]
                                      );
              Offset[2] = (Offset[0] + Offset[1]) / 2;

              //
              // set current average delay for both logical Strobe and PAIR logical Strobe
              //
              GetSetDataGroup (
                Host, Socket, Channel, NO_SUBCH, Dimm, Rank, Strobe, ALL_BITS, Level,
                Group, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &Offset[2]
                );
              if ((*DimmNvList)[Dimm].LogicalX16Mapping.PairLogical[Strobe] != 0xFF) {
                //
                // Set current delay for logical PAIR Strobe
                //
                GetSetDataGroup (
                  Host, Socket, Channel, NO_SUBCH, Dimm, Rank, (*DimmNvList)[Dimm].LogicalX16Mapping.PairLogical[Strobe],
                  ALL_BITS, Level, Group, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &Offset[2]
                  );
              }
            }
          } // Strobe loop
        } // Is x16 Dimm
      } //  Rank loop
    } // Dimm loop
  } // Channel loop
}

/**
  Updates results for an specific strobe
  This function is intended only to be used for x8 and x16 devices,
  For x4 devices it is required to extend its functionality

  @param[in]                Host          Pointer to sysHost
  @param[in]                Channel       Channel number
  @param[in]                TargetStrobe  Strobe number
  @param[in]                BwSerr        Error feedback
  @param[in]                CompositeResults Results to append


**/
VOID
UpdateStrobeBwError (
  IN        PSYSHOST        Host,
  IN        UINT8           Channel,
  IN        UINT8           TargetStrobe,
  IN        UINT32          BwSerr[MAX_CH][3],
  IN        UINT8           CompositeResults
  )
{
  UINT8   MSVx8;

  MSVx8 = GetMaxStrobeValid (Host) / 2;

  if (TargetStrobe == 0xFF || TargetStrobe >= MSVx8) {
    return;
  }

  if (IsEccStrobe (Host, TargetStrobe) && Host->nvram.mem.eccEn == 1) {
    BwSerr[Channel][2] |= CompositeResults & 0xFF;
    return;
  } else if (IsEccStrobe (Host, TargetStrobe) && Host->nvram.mem.eccEn == 0) {
    return;
  }

  if (TargetStrobe < (MSVx8 / 2)) {
    BwSerr[Channel][0] |= (CompositeResults << (8 * TargetStrobe));
  } else {
    BwSerr[Channel][1] |= (CompositeResults << (8 * (TargetStrobe - (MSVx8 / 2))));
  }
}


/**
  Return the errors detected on an specific strobe
  This function is intended only to be used for x8 and x16 devices,
  For x4 devices it is required to extend its functionality

  @param[in]                Host          Pointer to sysHost
  @param[in]                Channel       Channel number
  @param[in]                TargetStrobe  Strobe number
  @param[in]                BwSerr        Error feedback

  @return                   ByteError     Errors for an specified strobe

**/
UINT8
GetStrobeBwError (
  IN        PSYSHOST        Host,
  IN        UINT8           Channel,
  IN        UINT8           TargetStrobe,
  IN        UINT32          BwSerr[MAX_CH][3]
  )
{
  UINT8   Error;
  UINT8   MSVx8;

  MSVx8 = GetMaxStrobeValid (Host) / 2;

  if (TargetStrobe == 0xFF || TargetStrobe >= MSVx8) {
    return 0x00;
  }

  if (IsEccStrobe (Host, TargetStrobe) && Host->nvram.mem.eccEn == 1) {
    return (BwSerr[Channel][2] & 0xFF);
  } else if (IsEccStrobe (Host, TargetStrobe) && Host->nvram.mem.eccEn == 0) {
    return 0x00;
  }

  if (TargetStrobe < (MSVx8 / 2)) {
    Error = (BwSerr[Channel][0] >> (8 * TargetStrobe)) & 0xFF;
  } else {
    Error = (BwSerr[Channel][1] >> (8 * (TargetStrobe - (MSVx8 / 2)))) & 0xFF;
  }
  return Error;
}

/**
  Calculates positive and negative margin for the group provided

  @param Host      - Pointer to sysHost
  @param Socket    - Processor socket to check
  @param TestType  - Indicates the test being executed
  @param SkipRanks - Array that indicates the ranks to skip based on the CTLE capabilities

  @retval N/A
**/
VOID
EvaluateRanksToSkipCtle (
  IN     PSYSHOST Host,
  IN     UINT8    Socket,
  IN     MRC_TT   TestType,
     OUT BOOLEAN  SkipRanks[MAX_CH][MAX_DIMM]
)
{
  UINT8               Ch;
  UINT8               MaxChDdr;
  UINT8               Dimm;
  struct channelNvram (*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);
  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }

      if ((TestType == DramRxEqTest) && !DimmSupportForProgrammableCtle (Host, Socket, Ch, Dimm)) {
        SkipRanks[Ch][Dimm] = TRUE;
      }
    }
  }
}

/**
  Calculates positive and negative margin for the group provided

  @param Host                 Pointer to sysHost
  @param socket               Processor socket to check
  @param level                DDR or LRDIMM Host side, LRDIMM Backside, etc...
  @param group                Parameter to be margined
                                RxDqsDelay  : Margin Rx DqDqs
                                TxDqDelay   : Margin Tx DqDqs
                                RxVref      : Margin Rx Vref
                                TxVref      : Margin Tx Vref
                                RecEnDelay  : Margin Receive Enable
                                WrLvlDelay  : Margin Write Level
                                CmdGrp0     : Margin CMD group 0
                                CmdGrp1     : Margin CMD group 1
                                CmdGrp2     : Margin CMD group 2
                                CmdAll      : Margin all CMD groups
  @param mode                 Test mode to use
                                MODE_XTALK            BIT0: Enable crosstalk (placeholder)
                                MODE_VIC_AGG          BIT1: Enable victim/aggressor
                                MODE_START_ZERO       BIT2: Start at margin 0
                                MODE_POS_ONLY         BIT3: Margin only the positive side
                                MODE_NEG_ONLY         BIT4: Margin only the negative side
                                MODE_DATA_MPR         BIT5: Enable MPR Data pattern
                                MODE_DATA_FIXED       BIT6: Enable Fixed Data pattern
                                MODE_DATA_LFSR        BIT7: Enable Data LFSR
                                MODE_ADDR_LFSR        BIT8: Enable Address LFSR
                                MODE_ADDR_CMD0_SETUP  BIT9
                                MODE_CHECK_PARITY     BIT10: Enable parity checking
                                MODE_DESELECT         BIT11: Enable deselect patterns
                                MODE_VA_DESELECT      BIT12: Enable Victim - Aggressor deselect patterns
  @param scope                Margin granularity
                                SCOPE_SOCKET  0: Margin per processor socket
                                SCOPE_CH      1: Margin per channel
                                SCOPE_DIMM    2: Margin per DIMM
                                SCOPE_RANK    3: Margin per rank
                                SCOPE_STROBE  4: Margin per strobe group
                                SCOPE_BIT     5: Margin per bit
  @param mask                 Mask of bits to exclude from testing
  @param marginData           Pointer to the structure to store the margin results. The structure type varies based on scope.
  @param patternLength        RankMarginTest Pattern length
  @param update               Update starting margins so the next pass will run faster or not
  @param chMask               Bit Mask of channels to not be used
  @param rankMask             Bit Mask of ranks to not be used
  @param burstLength          Number of cachelines to test for each write/read operation. This is the repeat count for
                              the WR/RD subsequence.
  @param TestType             Test type
  @param SafeMarginsArbiter   Pointer to a structure containing the context and callbacks used to set safe values for
                              over-margined signals and restore the trained values afterward.

  @returns N/A
**/
VOID
GetMargins (
  PSYSHOST              Host,
  UINT8                 socket,
  MRC_LT                level,
  MRC_GT                group,
  UINT16                mode,
  UINT8                 scope,
  struct bitMask        *mask,
  VOID                  *marginData,
  UINT32                patternLength,
  UINT8                 update,
  UINT8                 chMask,
  UINT8                 rankMask,
  UINT8                 burstLength,
  MRC_TT                TestType,
  SAFE_MARGINS_ARBITER  *SafeMarginsArbiter
  )
{

#ifndef DDR5_SUPPORT

  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               r;
  UINT8               strobe;
  UINT8               bit;
  UINT8               sign;
  UINT8               byteIndex;
  UINT8               maxSign;
  UINT8               maxSteps = 0;
  UINT8               faultyBits;
  UINT32              beyondRangeMask[MAX_CH][3];
  INT16               offset[MAX_CH][MAX_RANK_CH];
  INT16               previousOffset[MAX_CH][MAX_RANK_CH];
  INT16               restoreOffset[MAX_CH][MAX_RANK_CH];
  UINT32              chAllFail = 0;
  UINT32              chStatus;
  UINT8               searchUp[MAX_CH][MAX_RANK_CH];
  UINT32              ranksDone[MAX_IMC];
  UINT8               imc;
  UINT32              status;
  MRC_CSN             TargetSignal = MrcCsnDelim;
  MRC_RT              MrcRankList;
  UINT32              bwSerr[MAX_CH][3];
  UINT32              ValidMask[MAX_CH*3];
  UINT8               ChunkMask = 0xFF;
  UINT32              ResultStatus[MAX_CH*3];
  struct bitMask      foundPass;
  struct bitMask      foundFail;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  struct              baseMargin  *socketMarginCopy;
  struct              chMargin    *channelMarginCopy;
  struct rankMargin   *rankMarginCopy;
  struct strobeMargin *strobeMarginCopy;
  struct bitMargin    *bitMarginCopy;
  //UINT16              cmdMinVal = MAX_CMD_MARGIN;
  //UINT16              cmdMaxVal = 0;
  UINT8               SeqCnt = 0;
  UINT32              MemSs = 0;
  UINT32              StatusArraySize = 0;
  UINT8               i = 0;
  UINT8               MSVx4 = GetMaxStrobeValid (Host);
  UINT8               MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8               MBV = GetMaxBitsValid (Host);
  UINT8               CompositeResults;
  UINT8               CurrentLogicalStrobeErr;
  UINT8               CurrentLogicalStrobePairErr;
  UINT32              PrevDebugLevel;
  SYS_SETUP           *Setup;
  MRC_GT              MarginGroup;
  UINT8               MaxChDdr;
  UINT8               MaxImc;
  BOOLEAN             SkipRanks[MAX_CH][MAX_DIMM] = {FALSE};

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_GET_MARGINS, group, socket));

  MaxImc = GetMaxImc ();
  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  channelNvList = GetChannelNvList (Host, socket);

  //
  // Evaluate if DIMM ranks should be skipped from the margining based on
  // the test type and CTLE capabilities
  //
  EvaluateRanksToSkipCtle (Host, socket, TestType, SkipRanks);

  if (mode & MODE_POS_ONLY) {
    // Only check the positive side
    sign = 0;
    maxSign = 1;
  } else if (mode & MODE_NEG_ONLY) {
    // Only check the negative side
    sign = 1;
    maxSign = 2;
  } else {
    // Check both positive and negative
    sign = 0;
    maxSign = 2;
  }

  //
  //MarginGroup is a local parameter used to allow the input group to be updated for the purposes
  //of margining only.  If the group result is modified by a ChunkMask (for example) the MarginGroup
  //may be desired to be different than the input group.
  MarginGroup = group;
  ChunkMask = 0xFF;
  //
  // RxDqsPDelay is even chunks, only include even chunks in this test, and margin the whole range
  // by updating the MarginGroup to use the RxDqsDelay
  //
  if (group == RxDqsPDelay) {
    ChunkMask = 0x55;
    MarginGroup = RxDqsDelay;
  }
  if (group == TxDqDelayEven) {
    ChunkMask = 0x55;
    MarginGroup = TxDqDelay;
  }
  //
  // RxDqsNDelay odd chunks, only include odd chunks in this test, and margin the whole range
  // by updating the MarginGroup to use the RxDqsDelay
  //
  if (group == RxDqsNDelay) {
    ChunkMask = 0xAA;
    MarginGroup = RxDqsDelay;
  }

  if (group == TxDqDelayOdd) {
    ChunkMask = 0xAA;
    MarginGroup = TxDqDelay;
  }
  VerifyInitialTxVrefOffsets (Host, socket, chMask, rankMask, level, MarginGroup, scope);

  PrevDebugLevel = GetDebugLevel ();
  if (((PrevDebugLevel & SDBG_MARGINS) == 0) && ((PrevDebugLevel & SDBG_FORCE_ALL) == 0)) {
    SetDebugLevel (SDBG_ERROR);
  }

  if (TestType == LateCmdTest) {
    TargetSignal = MrcCsnDelim;
  }
  if (CheckCMDGroup (MarginGroup) || CheckCTLGroup (MarginGroup)) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      // Check if this channel is masked off
      if (chMask & (1 << ch)) {
        continue;
      }
      GetMarginsHook (Host, socket, ch, 1);
    } // ch loop

    // Protect the FMC states from being changed as command or control signals are margined. MRS, extended MRS, and
    // memory-mapped config access are ignored. The SMBus version is used because this function may be called before
    // the command bus is fully trained.
    SetAepTrainingModeSMB (Host, socket, CHECKPOINT_MINOR_LATE_CMD_CLK);

    // Skip draining the DDRT RPQ while polling for the CPGC to be done. The drain operation uses MRS mode, which
    // requires a functional command bus. However, it is possible for the bus to be margined out of an operable range
    // by the time the CPGC done polling function is called, and using MRS mode in this state could result in a hang.
    // Instead, the draining will be done along with JedecInit during the error recovery portion, later in this
    // function.
    Host->var.mem.DdrtSkipRpqDrainInCpgcPolling[socket] = TRUE;
  }

  // Margin both sides
  for (; sign < maxSign; sign++) {

    //
    // Make room for Backside Cmd hold margining
    //
    if (((MarginGroup == CmdAll) || (MarginGroup == CtlAll)) && (level == LrbufLevel) && sign) {
      //
      // Reset FMC to the general training mode because BacksideShift needs to re-train RecEn.
      // During RecEn training, dimm needs to accept MRS command to send out the right pre-amble
      // pattern. If FMC is in CHECKPOINT_MINOR_LATE_CMD_CLK mode, the MRS is ignored and causes
      // RecEn training to hang.
      //
      SetAepTrainingMode (Host, socket, DISABLE_TRAINING_STEP);
      BacksideShift (Host, socket, DENORMALIZE);
      //
      // Restore DDRT training mode to CHECKPOINT_MINOR_LATE_CMD_CLK
      //
      SetAepTrainingMode (Host, socket, CHECKPOINT_MINOR_LATE_CMD_CLK);
    }

    // Clear pass/fail status
    ClearAllFound (Host, SET_ALL, SET_ALL, SET_ALL, foundPass.bits);
    ClearAllFound (Host, SET_ALL, SET_ALL, SET_ALL, foundFail.bits);

    // Initialize to no failures
    for (imc = 0; imc < MaxImc; imc++) {
      ranksDone[imc] = 0xFFFFFFFF;
    }

    // Get the offset to start with
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      // Check if this channel is masked off
      if (chMask & (1 << ch)) {
        continue;
      }

      imc = GetMCID (Host, socket, ch);

      dimmNvList = GetDimmNvList (Host, socket, ch);
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        rankList = GetRankNvList (Host, socket, ch, dimm);

        for (rank = 0; rank < MAX_RANK_DIMM; rank++) {

          // skip if this rank is disabled
          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          // skip rank training if we don't need to do it. example cmd training on RDIMM where rank>0
          if (CheckSkipRankTrain (Host, socket, ch, dimm, rank, level, MarginGroup) || SkipRanks[ch][dimm]) {
            continue;
          }

          // Do not test this rank if it is masked out
          if (rankMask & (1 << GetLogicalRank (Host, socket, ch, dimm, rank))) {
            continue;
          }

          GetStartingOffset (Host, socket, ch, dimm, rank, level, MarginGroup, sign, offset, &maxSteps);

          //
          // Save the nominal(original, before margin test) so that we can restore them after margin test
          // or during error recovery.
          //
          GetSetIoGroup (Host, socket, 0, ch, dimm, rank, ALL_STROBES, ALL_BITS, 0, level, MarginGroup, GSM_READ_ONLY, &restoreOffset[ch][ (*rankList)[rank].rankIndex]);
          if (mode & MODE_START_ZERO) {
            offset[ch][ (*rankList)[rank].rankIndex] = 0;
          }
          //RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
          //        "Starting Offset = %d, maxSteps = %d\n", offset[ch][(*rankList)[rank].rankIndex], maxSteps);

          // Indicate this rank is not done
          ranksDone[imc] &= ~(1 << ((*rankList)[rank].rankIndex + (GetMCCh (imc, ch) * 8)));

          previousOffset[ch][ (*rankList)[rank].rankIndex] = 0;
          searchUp[ch][ (*rankList)[rank].rankIndex] = 1;

          //
          // Ignore nibbles/bytes that have previously failed
          //
          byteIndex = 0;
          for (strobe = 0; strobe < MSVx4; strobe++) {
            if ((*rankList)[rank].faultyParts[strobe]) {
              if ((*dimmNvList)[dimm].x4Present) {
                if (strobe < MSVx8) {
                  faultyBits = 0xF;
                } else {
                  faultyBits = 0xF0;
                }
              } else {
                faultyBits = 0xFF;
              }

              if (strobe < MSVx8) {
                byteIndex = strobe;
              } else {
                byteIndex = strobe - MSVx8;
              }

              SetAllFound (Host, ch, (*rankList)[rank].rankIndex, byteIndex, faultyBits, foundPass.bits);
              SetAllFound (Host, ch, (*rankList)[rank].rankIndex, byteIndex, faultyBits, foundFail.bits);
              mask->bits[ch][ (*rankList)[rank].rankIndex][byteIndex] |= faultyBits;
            }
          } // strobe loop
        } // rank loop
      } // dimm loop
    } // ch loop

    // Loop through the read Vref points
    chStatus = 0;

#ifdef DEBUG_CODE_BLOCK
    if (GetDebugLevel () & SDBG_DEFAULT) {
      AcquirePrintControl ();
      //
      // BRS_TODO: Make data bits column header dynamic based on MaxBitsValid
      //
      RcDebugPrint (SDBG_DEFAULT, "\nsign = %d\n", sign);
      if (IsDdr5Present (Host, socket)) {
        RcDebugPrint (SDBG_DEFAULT, "Data bits -> 0------7 8-----15 16----23 24----31");
        if (Host->nvram.mem.eccEn) {
          RcDebugPrint (SDBG_DEFAULT, " 32----39\n");
        }
        else {
          RcDebugPrint (SDBG_DEFAULT, "\n");
        }
      }
      else {
        RcDebugPrint (SDBG_DEFAULT, "Data bits -> 0------7 8-----15 16----23 24----31 32----39 40----47 48----55 56----63");
        if (Host->nvram.mem.eccEn) {
          RcDebugPrint (SDBG_DEFAULT, " 64----71\n");
        }
        else {
          RcDebugPrint (SDBG_DEFAULT, "\n");
        }
      }
      ReleasePrintControl ();
    }
#endif  // DEBUG_CODE_BLOCK
    // Loop until we either find an edge or reach the end of our range
    while (!AreRanksDone (ranksDone)) {

      if ((scope == SCOPE_SOCKET) || (scope == SCOPE_CH)) {
        for (ch = 0; ch < MaxChDdr; ch++) {
          if ((*channelNvList)[ch].enabled == 0) {
            continue;
          }
          // Check if this channel is masked off
          if (chMask & (1 << ch)) {
            continue;
          }

          //
          // Clear the per bit mask registers
          //
          ClearBWErrorStatus (Host, socket, ch);

          // Initialize error status to 0
          bwSerr[ch][0] = 0;
          bwSerr[ch][1] = 0;
          bwSerr[ch][2] = 0;
        } // ch loop
      }

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        if (scope == SCOPE_DIMM) {
          for (ch = 0; ch < MaxChDdr; ch++) {
            if ((*channelNvList)[ch].enabled == 0) {
              continue;
            }
            // Check if this channel is masked off
            if (chMask & (1 << ch)) {
              continue;
            }

            //
            // Clear the per bit mask registers
            //
            ClearBWErrorStatus (Host, socket, ch);

            // Initialize error status to 0
            bwSerr[ch][0] = 0;
            bwSerr[ch][1] = 0;
            bwSerr[ch][2] = 0;
          } // ch loop
        }

        // Loop for each rank
        for (rank = 0; rank < MAX_RANK_DIMM; rank++) {

          chStatus = 0xFF;

          chAllFail = chStatus;

          for (ch = 0; ch < MaxChDdr; ch++) {
            if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
              continue;
            }
            imc = GetMCID (Host, socket, ch);
            // Check if this channel is masked off
            if (chMask & (1 << ch)) {
              continue;
            }

            // Continue if this rank is not enabled
            if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
              continue;
            }

            if (CheckSkipRankTrain (Host, socket, ch, dimm, rank, level, MarginGroup) || SkipRanks[ch][dimm]) {
              continue;
            }

            // Do not test this rank if it is masked out
            if (rankMask & (1 << GetLogicalRank (Host, socket, ch, dimm, rank))) {
              continue;
            }

            rankList = GetRankNvList (Host, socket, ch, dimm);

            // Continue if this rank is done
            if (ranksDone[imc] & (1 << ((*rankList)[rank].rankIndex + (GetMCCh (imc, ch) * 8)))) {
              continue;
            }

            dimmNvList = GetDimmNvList (Host, socket, ch);

            // Clear the bit for this channel if this rank has not failed yet or has not yet passed
            if (CheckAllPassFail (Host, foundFail.bits[ch][ (*rankList)[rank].rankIndex]) ||
                CheckAllPassFail (Host, foundPass.bits[ch][ (*rankList)[rank].rankIndex])) {
              chStatus &= ~(1 << ch);
            } else {
              ranksDone[imc] |= (1 << ((*rankList)[rank].rankIndex + (GetMCCh (imc, ch) * 8)));
            }

          } // ch loop

          // If there are no channels to test reset channel status and go to the next rank
          if (chStatus == chAllFail) {

            //
            // The following ch loop reset the margin signals to their orignal setting
            // Similar code repeat multiple times inside the GetMargins and may be better to conslidate
            // them in one helper function. Need to be careful the chMask, chStatus and skipping rank
            //

            for (ch = 0; ch < MaxChDdr; ch++) {
              if ((*channelNvList)[ch].enabled == 0) {
                continue;
              }
              // Check if this channel is masked off
              if (chMask & (1 << ch)) {
                continue;
              }

              // skip if this rank is disabled
              if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
                continue;
              }

              // skip rank training if we don't need to do it. example cmd training on RDIMM where rank>0
              if (CheckSkipRankTrain (Host, socket, ch, dimm, rank, level, MarginGroup) || SkipRanks[ch][dimm]) {
                continue;
              }

              rankList = GetRankNvList (Host, socket, ch, dimm);

              //
              // Restore the new offset to test
              //
              if (previousOffset[ch][ (*rankList)[rank].rankIndex]) {
                RestoreOffset (Host, socket, ch, dimm, rank, restoreOffset[ch][ (*rankList)[rank].rankIndex], level, MarginGroup);

                dimmNvList = GetDimmNvList (Host, socket, ch);

                if (((CheckCMDGroup (MarginGroup) || CheckCTLGroup (MarginGroup)) && !(((*dimmNvList)[dimm].DcpmmPresent) && (MarginGroup == CmdVref)))) {
                  for (r = 0; r < MAX_RANK_CH; r++) {
                    previousOffset[ch][r] = 0;
                  }
                } else {
                  previousOffset[ch][ (*rankList)[rank].rankIndex] = 0;
                }
              }
            } // ch loop

            chStatus = 0;
            continue;
          } // end of (chStatus == chAllFail)

          // Set mask bits
          SetBwErrorMask (Host, socket, dimm, rank, mask);

          for (ch = 0; ch < MaxChDdr; ch++) {
            if ((*channelNvList)[ch].enabled == 0) {
              continue;
            }
            // Check if this channel is masked off
            if (chMask & (1 << ch)) {
              continue;
            }
            if ((~chStatus & (1 << ch)) == 0) {
              continue;
            }
            // skip if this rank is disabled
            if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
              continue;
            }

            // skip rank training if we don't need to do it. example cmd training on RDIMM where rank>0
            if (CheckSkipRankTrain (Host, socket, ch, dimm, rank, level, MarginGroup) || SkipRanks[ch][dimm]) {
              continue;
            }

            rankList = GetRankNvList (Host, socket, ch, dimm);


            if ((scope == SCOPE_RANK) || (scope == SCOPE_STROBE) || (scope == SCOPE_BIT)) {
              // Initialize error status to all fail
              bwSerr[ch][0] = (UINT32) -1;
              bwSerr[ch][1] = (UINT32) -1;
              bwSerr[ch][2] = (UINT32) -1;
            }

            // Set the new offset to test
            status = SetOffset (Host, socket, ch, dimm, rank, offset[ch][ (*rankList)[rank].rankIndex], level, MarginGroup,
                       previousOffset[ch][ (*rankList)[rank].rankIndex]);

            dimmNvList = GetDimmNvList (Host, socket, ch);

            if (((CheckCMDGroup (MarginGroup) || CheckCTLGroup (MarginGroup)) && !(((*dimmNvList)[dimm].DcpmmPresent) && (MarginGroup == CmdVref)))) {
              for (r = 0; r < MAX_RANK_CH; r++) {
                previousOffset[ch][r] = offset[ch][ (*rankList)[rank].rankIndex];
              }
            } else {
              previousOffset[ch][ (*rankList)[rank].rankIndex] = offset[ch][ (*rankList)[rank].rankIndex];
            }

            // Check if we've reached the maximum or minimum in the allowable range
            CheckRange (Host, socket, ch, dimm, rank, offset[ch][ (*rankList)[rank].rankIndex], sign, level, MarginGroup,
              foundFail.bits[ch][ (*rankList)[rank].rankIndex], foundPass.bits[ch][ (*rankList)[rank].rankIndex],
              scope, previousOffset[ch][ (*rankList)[rank].rankIndex], marginData, beyondRangeMask[ch]);

            //
            // Skipping this check for groups that need to be restored to original value between margin steps.
            // This corrects an issue where these groups are not properly restored when CheckRange reaches a limit.
            //
            if (!(CheckCMDGroup (MarginGroup) || CheckCTLGroup (MarginGroup))) {
              if (!(CheckAllPassFail (Host, foundFail.bits[ch][ (*rankList)[rank].rankIndex]) ||
                    CheckAllPassFail (Host, foundPass.bits[ch][ (*rankList)[rank].rankIndex]))) {
                chStatus |= (1 << ch);          // disable channel
              }
            }
            if ((scope == SCOPE_RANK) || (scope == SCOPE_STROBE) || (scope == SCOPE_BIT)) {
              //
              // Clear the per bit mask registers
              //
              ClearBWErrorStatus (Host, socket, ch);
            }
          } // ch loop

          MemSs = 0;
          MrcRankList.Dimm = dimm;
          MrcRankList.Rank = rank;
          SeqCnt = PatternLengthToCpgcLoopCnt (patternLength);
          FifoTrainReset (Host, socket);
          /*do {
            // Run tests
            if (rank == 0) {
              status = SetupTest (Host, socket, MemSs, ~chStatus, 1, &MrcRankList, level, TestType, TargetSignal, SeqCnt);
            } else {
              status = ModifyTest (Host, socket, MemSs, ~chStatus, 1, &MrcRankList, TestType);
            }
            status = TestControl (Host, socket, MemSs, ~chStatus, 1, 1, 0);
          } while (status == FAILURE);
          */
          if (ExecuteTest (Host, socket, ~chStatus, dimm, rank, MarginGroup, mode, patternLength, burstLength) == FAILURE) {
            SetDebugLevel (PrevDebugLevel);
            RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT, "ExecuteTest failed\n");
            if (((PrevDebugLevel & SDBG_MARGINS) == 0) && ((PrevDebugLevel & SDBG_FORCE_ALL) == 0)) {
              SetDebugLevel (SDBG_ERROR);
            }
          }

          // Collect test results
          StatusArraySize = MAX_CH * 3;
          status = GetTrainResultStatus (Host, socket, 0, ~chStatus, NO_SUBCH, StatusArraySize * 4, ResultStatus, ValidMask, ChunkMask, dimm);

          for (ch = 0; ch < MaxChDdr; ch++) {
            for (i = 0; i < 3; i++) {
              bwSerr[ch][i] = *(ResultStatus + ch * 3 + i);
            }
          }

          for (ch = 0; ch < MaxChDdr; ch++) {
            if ((*channelNvList)[ch].enabled == 0) {
              continue;
            }

            dimmNvList = GetDimmNvList (Host, socket, ch);

            bwSerr[ch][0] |= beyondRangeMask[ch][0];
            bwSerr[ch][1] |= beyondRangeMask[ch][1];
            bwSerr[ch][2] |= beyondRangeMask[ch][2];

            //
            // Merge results for TxVref when working with x16 devices, if DdrLevel and PDA Enabled
            //
            if (IsX16Dimm (socket, ch, dimm) &&
                IsTxVrefPdaX16 (Host, socket, level, MarginGroup, scope) &&
                IsMemFlowEnabled (DqSwizzlingDiscoveryX16) &&
                FeaturePcdGet (PcdCosimBuild) == FALSE &&
                FeaturePcdGet (PcdCteBuild) == FALSE) {

              for (strobe = 0; strobe < MSVx8; strobe++) {
                //
                // Get errors for current logical strobe
                //
                CurrentLogicalStrobeErr = GetStrobeBwError (Host, ch, strobe, bwSerr);
                //
                // Get errors for current logical strobe pair
                //
                CurrentLogicalStrobePairErr = GetStrobeBwError (Host, ch, (*dimmNvList)[dimm].LogicalX16Mapping.PairLogical[strobe], bwSerr);
                //
                // Composite results from both strobes
                //
                CompositeResults = CurrentLogicalStrobeErr | CurrentLogicalStrobePairErr;
                //
                // Put Composite results back in the BwSerr for current logical strobe
                //
                UpdateStrobeBwError (Host, ch, strobe, bwSerr, CompositeResults);
                //
                // Put Composite results back in the BwSerr for current logical pair strobe
                //
                UpdateStrobeBwError (Host, ch, (*dimmNvList)[dimm].LogicalX16Mapping.PairLogical[strobe], bwSerr, CompositeResults);

              } // Strobe loop
            } // TxVref && DdrLevel && x16Dimm
          } // ch loop

          if (CheckCMDGroup (MarginGroup) || CheckCTLGroup (MarginGroup)) {
            for (ch = 0; ch < MaxChDdr; ch++) {
              if ((*channelNvList)[ch].enabled == 0) {
                continue;
              }
              // Check if this channel is masked off
              if (chMask & (1 << ch)) {
                continue;
              }
              if ((~chStatus & (1 << ch)) == 0) {
                continue;
              }
              rankList = GetRankNvList (Host, socket, ch, dimm);

              //
              // Restore the new offset to test
              //
              if (previousOffset[ch][ (*rankList)[rank].rankIndex]) {
                RestoreOffset (Host, socket, ch, dimm, rank, restoreOffset[ch][ (*rankList)[rank].rankIndex], level, MarginGroup);

                dimmNvList = GetDimmNvList (Host, socket, ch);
                if (((CheckCMDGroup (MarginGroup) || CheckCTLGroup (MarginGroup)) && !(((*dimmNvList)[dimm].DcpmmPresent) && (MarginGroup == CmdVref)))) {
                  for (r = 0; r < MAX_RANK_CH; r++) {
                    previousOffset[ch][r] = 0;
                  } // r loop
                } else {
                  previousOffset[ch][ (*rankList)[rank].rankIndex] = 0;
                }
              }
            } // ch loop

            //
            // Error recovery flow for CMD/CTL margin tests
            //
            if (status) {

              // Use safe settings for the command and/or control signals.
              if (SafeMarginsArbiter != NULL) {
                SafeMarginsArbiter->SetSafeMargins (SafeMarginsArbiter);
              }

              for (ch = 0; ch < MaxChDdr; ch++) {
                if ((*channelNvList)[ch].enabled == 0) {
                  continue;
                }
                RPQDrain (Host, socket, ch, dimm, 0); // DDRT only has rank 0.
              }

              IO_Reset (Host, socket);
              IODdrtReset (Host, socket, dimm);
              FifoTrainReset (Host, socket);
              // Because the FMC training mode settings are already saved and restored inside the JedecInitSequence(),
              // no need to explicitly set the training mode back to CHECKPOINT_MINOR_LATE_CMD_CLK after the function call.
              JedecInitSequence (Host, socket, CH_BITMASK);

              ClrPcheMiss (Host, socket);

              // Restore command and/or control signals to trained values.
              if (SafeMarginsArbiter != NULL) {
                SafeMarginsArbiter->RestoreTrainedMargins (SafeMarginsArbiter);
              }

            } // end of status
          } // end of (CheckCMDGroup (MarginGroup) || CheckCTLGroup (MarginGroup))

          //
          // Error recovery flow for TxVref margin test
          //
          if (status &&
              (MarginGroup == TxVref) &&
              (Host->nvram.mem.socket[socket].TrainingStepDone.TxVref == 1) &&
              (level == DdrLevel))
          {
            IO_Reset (Host, socket);
            IODdrtReset (Host, socket, dimm);
            FifoTrainReset (Host, socket);
            if (IsDdr5Present (Host, socket) == FALSE) {
              JedecInitSequence (Host, socket, CH_BITMASK);
            }
          }

          if ((scope == SCOPE_RANK) || (scope == SCOPE_STROBE) || (scope == SCOPE_BIT)) {
            // Evaluate Test Results
            EvaluateResults (Host, socket, dimm, rank, sign, offset, ~chStatus, scope, MarginGroup, level, mode, searchUp, bwSerr, &foundPass,
              &foundFail, marginData, ranksDone, maxSteps);
          }
        } // rank loop

        if (scope == SCOPE_DIMM) {
          // Evaluate Test Results
          EvaluateResults (Host, socket, dimm, 0, sign, offset, ~chStatus, scope, MarginGroup, level, mode, searchUp, bwSerr, &foundPass,
            &foundFail, marginData, ranksDone, maxSteps);
        }
      } // dimm loop

      if ((scope == SCOPE_SOCKET) || (scope == SCOPE_CH)) {
        // Evaluate Test Results
        EvaluateResults (Host, socket, 0, 0, sign, offset, ~chStatus, scope, MarginGroup, level, mode, searchUp, bwSerr, &foundPass,
          &foundFail, marginData, ranksDone, maxSteps);
      }

      if (AreRanksDone (ranksDone)) {
        if (CheckCMDGroup (MarginGroup) || CheckCTLGroup (MarginGroup)) {
          for (ch = 0; ch < MaxChDdr; ch++) {
            if ((*channelNvList)[ch].enabled == 0) {
              continue;
            }
            if (chMask & (1 << ch)) {
              continue;
            }

            dimmNvList = GetDimmNvList (Host, socket, ch);

            for (dimm = 0; dimm < MAX_DIMM; dimm++) {

              rankList = GetRankNvList (Host, socket, ch, dimm);

              for (rank = 0; rank < MAX_RANK_DIMM; rank++) {

                // skip if this rank is disabled
                if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
                  continue;
                }

                // skip rank training if we don't need to do it. example cmd training on RDIMM where rank>0
                if (CheckSkipRankTrain (Host, socket, ch, dimm, rank, level, MarginGroup) || SkipRanks[ch][dimm]) {
                  continue;
                }

                // Do not test this rank if it is masked out
                if (rankMask & (1 << GetLogicalRank (Host, socket, ch, dimm, rank))) {
                  continue;
                }

                //
                // Restore the new offset to test
                //
                if (previousOffset[ch][ (*rankList)[rank].rankIndex]) {
                  RestoreOffset (Host, socket, ch, dimm, rank, restoreOffset[ch][ (*rankList)[rank].rankIndex], level, MarginGroup);

                  dimmNvList = GetDimmNvList (Host, socket, ch);
                  if (((CheckCMDGroup (MarginGroup) || CheckCTLGroup (MarginGroup)) && !(((*dimmNvList)[dimm].DcpmmPresent) && (MarginGroup == CmdVref)))) {
                    for (r = 0; r < MAX_RANK_CH; r++) {
                      previousOffset[ch][r] = 0;
                    } // r loop
                  } else {
                    previousOffset[ch][ (*rankList)[rank].rankIndex] = 0;
                  }
                }
              } // rank loop
            } // dimm loop
          } // ch loop

          IO_Reset (Host, socket);
          IODdrtResetAll (Host, socket);
          FifoTrainReset (Host, socket);
          JedecInitSequence (Host, socket, CH_BITMASK);
        } // end of (CheckCMDGroup (MarginGroup) || CheckCTLGroup (MarginGroup))

        if ((MarginGroup == TxVref) &&
            (Host->nvram.mem.socket[socket].TrainingStepDone.TxVref == 1) &&
            (level == DdrLevel))
        {
          IO_Reset (Host, socket);
          IODdrtResetAll (Host, socket);
          FifoTrainReset (Host, socket);
          if (IsDdr5Present (Host, socket) == FALSE) {
            JedecInitSequence (Host, socket, CH_BITMASK);
          }
        }
      }
    } // offset loop
  } // sign loop

  //
  // Restore Backside Cmd delays to 0
  //
  if (((MarginGroup == CmdAll) || (MarginGroup == CtlAll)) && (level == LrbufLevel) && (sign == 2)) {
    //
    // Reset FMC to the general training mode because BacksideShift needs to re-train RecEn.
    // During RecEn training, dimm needs to accept MRS command to send out the right pre-amble
    // pattern. If FMC is in CHECKPOINT_MINOR_LATE_CMD_CLK mode, the MRS is ignored and causes
    // RecEn training to hang.
    //
    SetAepTrainingMode (Host, socket, DISABLE_TRAINING_STEP);
    BacksideShift (Host, socket, RENORMALIZE);
    //
    // Restore DDRT training mode to CHECKPOINT_MINOR_LATE_CMD_CLK
    //
    SetAepTrainingMode (Host, socket, CHECKPOINT_MINOR_LATE_CMD_CLK);
  }

  //
  // clear bitwise error mask
  //
  ClearBwErrorMask (Host, socket);

  if (CheckCMDGroup (MarginGroup) || CheckCTLGroup (MarginGroup)) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      // Check if this channel is masked off
      if (chMask & (1 << ch)) {
        continue;
      }
      GetMarginsHook (Host, socket, ch, 0);
    } // ch loop

    // Reset FMC to the general training mode
    SetAepTrainingModeSMB (Host, socket, DISABLE_TRAINING_STEP);
  }

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    dimmNvList = GetDimmNvList (Host, socket, ch);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {

      rankList = GetRankNvList (Host, socket, ch, dimm);

      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {

        // skip if this rank is disabled
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        // skip rank training if we don't need to do it. example cmd training on RDIMM where rank>0
        if (CheckSkipRankTrain (Host, socket, ch, dimm, rank, level, MarginGroup) || SkipRanks[ch][dimm]) {
          continue;
        }

        // Do not test this rank if it is masked out
        if (rankMask & (1 << GetLogicalRank (Host, socket, ch, dimm, rank))) {
          continue;
        }
        //
        // Restore the new offset to test
        //
        if (previousOffset[ch][ (*rankList)[rank].rankIndex]) {
          RestoreOffset (Host, socket, ch, dimm, rank, restoreOffset[ch][ (*rankList)[rank].rankIndex], level, MarginGroup);

          dimmNvList = GetDimmNvList (Host, socket, ch);
          if (((CheckCMDGroup (MarginGroup) || CheckCTLGroup (MarginGroup)) && !(((*dimmNvList)[dimm].DcpmmPresent) && (MarginGroup == CmdVref)))) {
            for (r = 0; r < MAX_RANK_CH; r++) {
              previousOffset[ch][r] = 0;
            } // r loop
          } else {
            previousOffset[ch][ (*rankList)[rank].rankIndex] = 0;
          }
        }
      } // rank loop
    } // dimm loop
  } // ch loop
  r = 1;
  //
  // Update starting offsets so the next pass will run faster
  //
  if (level == DdrLevel) {
    if (update) {
      UpdateStartingOffset (Host, socket, MarginGroup, level, scope, marginData);
    }

    //
    // For LRDIMM, just treat all Host side ranks like rank 0, so copy rank 0 results to other ranks
    //

    if (Host->nvram.mem.socket[socket].lrDimmPresent && (MarginGroup == TxVref)) {
      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }
        dimmNvList = GetDimmNvList (Host, socket, ch);
        for (dimm = 0; dimm < MAX_DIMM; dimm++) {
          if ((*dimmNvList)[dimm].dimmPresent == 0) {
            continue;
          }
          rankList = GetRankNvList (Host, socket, ch, dimm);
          rank = 0;
          // skip if this rank is disabled
          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          if (scope == SCOPE_STROBE) {
            for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
              if (CheckSkipRankTrain (Host, socket, ch, dimm, rank, level, MarginGroup) || SkipRanks[ch][dimm]) {
                continue;
              }
              strobeMarginCopy = marginData;
              for (strobe = MSVx8; strobe < MSVx4; strobe++) {
                (*strobeMarginCopy).strobe[ch][ (*rankList)[rank].rankIndex][strobe].n = (*strobeMarginCopy).strobe[ch][ (*rankList)[rank].rankIndex][strobe - MSVx8].n;
                (*strobeMarginCopy).strobe[ch][ (*rankList)[rank].rankIndex][strobe].p = (*strobeMarginCopy).strobe[ch][ (*rankList)[rank].rankIndex][strobe - MSVx8].p;
              } // strobe
            } // rank
          } // scope
        } // dimm loop
      } // ch loop
    }
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      dimmNvList = GetDimmNvList (Host, socket, ch);
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }
        rankList = GetRankNvList (Host, socket, ch, dimm);
        for (rank = 1; rank < MAX_RANK_DIMM; rank++) {
          // skip if this rank is disabled
          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          // skip rank training if we don't need to do it. example cmd training on RDIMM where rank>0
          if ((CheckSkipRankTrain (Host, socket, ch, dimm, rank, level, MarginGroup) == 0) || SkipRanks[ch][dimm]){
            continue;
          }

          if (scope == SCOPE_RANK) {
            rankMarginCopy = marginData;
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,"Copy Rank from %d to %d, p = %d, n = %d\n",
                                    (*rankList)[0].rankIndex,
                                    (*rankList)[rank].rankIndex,
                                    (*rankMarginCopy).rank[ch][ (*rankList)[0].rankIndex].p,
                                    (*rankMarginCopy).rank[ch][(*rankList)[0].rankIndex].n
                                    );
            (*rankMarginCopy).rank[ch][ (*rankList)[rank].rankIndex].n = (*rankMarginCopy).rank[ch][ (*rankList)[0].rankIndex].n;
            (*rankMarginCopy).rank[ch][ (*rankList)[rank].rankIndex].p = (*rankMarginCopy).rank[ch][ (*rankList)[0].rankIndex].p;
          } else if (scope == SCOPE_STROBE) {
            strobeMarginCopy = marginData;
            for (strobe = 0; strobe < MSVx4; strobe++) {
              (*strobeMarginCopy).strobe[ch][ (*rankList)[rank].rankIndex][strobe].n = (*strobeMarginCopy).strobe[ch][ (*rankList)[0].rankIndex][strobe].n;
              (*strobeMarginCopy).strobe[ch][ (*rankList)[rank].rankIndex][strobe].p = (*strobeMarginCopy).strobe[ch][ (*rankList)[0].rankIndex][strobe].p;
              if (strobe >= MSVx8 && Host->nvram.mem.socket[socket].lrDimmPresent && (MarginGroup == TxVref)) {// duplicate the strobe values for power training for the Tx Vref case for each rank
                (*strobeMarginCopy).strobe[ch][ (*rankList)[rank].rankIndex][strobe].n = (*strobeMarginCopy).strobe[ch][ (*rankList)[0].rankIndex][strobe-MSVx8].n;
                (*strobeMarginCopy).strobe[ch][ (*rankList)[rank].rankIndex][strobe].p = (*strobeMarginCopy).strobe[ch][ (*rankList)[0].rankIndex][strobe-MSVx8].p;
              }
              RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,"Left %d, Right %d \n",
                                      (*strobeMarginCopy).strobe[ch][ (*rankList)[rank].rankIndex][strobe].p,
                                      (*strobeMarginCopy).strobe[ch][(*rankList)[rank].rankIndex][strobe].n
                                      );
            }
          } else if (scope == SCOPE_BIT) {
            bitMarginCopy = marginData;
            for (bit = 0; bit < MBV; bit++) {
              (*bitMarginCopy).bits[ch][ (*rankList)[rank].rankIndex][bit].n = (*bitMarginCopy).bits[ch][ (*rankList)[0].rankIndex][bit].n;
              (*bitMarginCopy).bits[ch][ (*rankList)[rank].rankIndex][bit].p = (*bitMarginCopy).bits[ch][ (*rankList)[0].rankIndex][bit].p;
              RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, bit, "Left %d, Right %d \n",
                                      (*bitMarginCopy).bits[ch][ (*rankList)[rank].rankIndex][bit].p,
                                      (*bitMarginCopy).bits[ch][(*rankList)[rank].rankIndex][bit].n
                                      );
            } // bit loop
          } // scope
        } // rank loop
      } // dimm loop
    } // ch loop
  }

  // Make sure if the margins are flipped, they are set to 0
  if (scope == SCOPE_SOCKET) {
    socketMarginCopy = marginData;
    if ((*socketMarginCopy).n >= 0 && (*socketMarginCopy).p <= 0) {
      (*socketMarginCopy).n = 0;
      (*socketMarginCopy).p = 0;
    }
  } else {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      if (scope == SCOPE_CH) {
        channelMarginCopy = marginData;
        if ((*channelMarginCopy).channel[ch].n >= 0 && (*channelMarginCopy).channel[ch].p <= 0) {
          (*channelMarginCopy).channel[ch].n = 0;
          (*channelMarginCopy).channel[ch].p = 0;
        }
      } else {
        dimmNvList = GetDimmNvList (Host, socket, ch);
        for (dimm = 0; dimm < MAX_DIMM; dimm++) {
          if ((*dimmNvList)[dimm].dimmPresent == 0) {
            continue;
          }
          rankList = GetRankNvList (Host, socket, ch, dimm);
          for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
            // skip if this rank is disabled
            if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
              continue;
            }

            //
            // skip rank training if we don't need to do it. For example, CMD training on RDIMM where rank>0
            //
            if (CheckSkipRankTrain (Host, socket, ch, dimm, rank, level, MarginGroup) || SkipRanks[ch][dimm]) {
              continue;
            }

            if (scope == SCOPE_RANK || scope == SCOPE_DIMM) {
              rankMarginCopy = marginData;
              if ((*rankMarginCopy).rank[ch][ (*rankList)[rank].rankIndex].n >= 0 && (*rankMarginCopy).rank[ch][ (*rankList)[rank].rankIndex].p <= 0) {
                (*rankMarginCopy).rank[ch][ (*rankList)[rank].rankIndex].n = 0;
                (*rankMarginCopy).rank[ch][ (*rankList)[rank].rankIndex].p = 0;
              }
            } else if (scope == SCOPE_STROBE) {
              strobeMarginCopy = marginData;
              for (strobe = 0; strobe < MSVx4; strobe++) {
                if ((*strobeMarginCopy).strobe[ch][ (*rankList)[rank].rankIndex][strobe].n >= 0 && (*strobeMarginCopy).strobe[ch][ (*rankList)[rank].rankIndex][strobe].p <= 0) {
                  (*strobeMarginCopy).strobe[ch][ (*rankList)[rank].rankIndex][strobe].n = 0;
                  (*strobeMarginCopy).strobe[ch][ (*rankList)[rank].rankIndex][strobe].p = 0;
                }
              }
            } else if (scope == SCOPE_BIT) {
              bitMarginCopy = marginData;
              for (bit = 0; bit < MBV; bit++) {
                if ((*bitMarginCopy).bits[ch][ (*rankList)[rank].rankIndex][bit].n >= 0 && (*bitMarginCopy).bits[ch][ (*rankList)[rank].rankIndex][bit].p <= 0) {
                  (*bitMarginCopy).bits[ch][ (*rankList)[rank].rankIndex][bit].n = 0;
                  (*bitMarginCopy).bits[ch][ (*rankList)[rank].rankIndex][bit].p = 0;
                }
              }
            } //scope
          } //rank
        } //dimm
      } // if/else scope = ch
    } //ch
  } // if/else scope = Socket

  if (CheckCMDGroup (MarginGroup) || CheckCTLGroup (MarginGroup)) {
    IO_Reset (Host, socket);
    IODdrtResetAll (Host, socket);
    FifoTrainReset (Host, socket);
    JedecInitSequence (Host, socket, CH_BITMASK);
    Host->var.mem.DdrtSkipRpqDrainInCpgcPolling[socket] = FALSE;
  }

  //
  // Reset the IO
  //
  IO_Reset (Host, socket);

  SetDebugLevel (PrevDebugLevel);

#endif // #ifndef DDR5_SUPPORT

} // GetMargins


/**

  Returns the starting offsets for the given test group

  @param Host          - Pointer to sysHost
  @param socket        - Processor socket to check
  @param ch            - Current Channel
  @param dimm          - Dimm number (0-based)
  @param rank          - Rank number (0-based)
  @param level         - DDR or LRDIMM Host side, LRDIMM Backside, etc...
  @param group         - Test group
  @param sign          - The direction we are currently going (up, down, left or right)
  @param offset        - Pointer to offsets
  @param maxSteps      - Pointer to maximum number of steps

  @retval N/A

**/
VOID
GetStartingOffset (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     sign,
  INT16     offset[MAX_CH][MAX_RANK_CH],
  UINT8     *maxSteps
  )
{
  UINT16                    MinLimit;
  UINT16                    MaxLimit;
  struct  ddrRank           (*rankList)[MAX_RANK_DIMM];
  struct  dimmNvram         (*dimmNvList)[MAX_DIMM];
  struct  TrainingVariable  *TrainingVar;

  TrainingVar = MemTrainingVar ();
  rankList = GetRankNvList (Host, socket, ch, dimm);
  dimmNvList = GetDimmNvList (Host, socket, ch);
  // Get the offset to start with
#ifdef LRDIMM_SUPPORT
  if (level == LrbufLevel) {
    switch (group) {
    case RxVref:
      if (sign) {
        offset[ch][ (*rankList)[rank].rankIndex] = -7;
      } else {
        offset[ch][ (*rankList)[rank].rankIndex] = 7;
      }
      *maxSteps = 45;
      break;
    case TxVref:
      if (sign) {
        offset[ch][ (*rankList)[rank].rankIndex] = -7;
      } else {
        offset[ch][ (*rankList)[rank].rankIndex] = 7;
      }
      //*maxSteps = MAX_LRBUF_RD_VREF_OFFSET;
      if ((*dimmNvList)[dimm].DcpmmPresent == 1) {
        *maxSteps = MAX_WR_VREF_OFFSET_BACKSIDE_DDRT;
      } else {
        *maxSteps = MAX_WR_VREF_OFFSET_BACKSIDE;
      }

      break;

    case RxDqsDelay:
    case RxDqsNDelay:
    case RxDqsPDelay:
    case TxDqDelay:
      if (sign) {
        offset[ch][ (*rankList)[rank].rankIndex] = -5;
      } else {
        offset[ch][ (*rankList)[rank].rankIndex] = 5;
      }
      *maxSteps = MAX_TIMING_OFFSET;
      break;
    case CmdAll:
    case Cmd0All:
    case Cmd1All:
    case CtlAll:
    case Ctl0Grp:
    case Ctl1Grp:
    case CmdVref:
      if (sign) {
        offset[ch][ (*rankList)[rank].rankIndex] = -1;
      } else {
        offset[ch][ (*rankList)[rank].rankIndex] = 1;
      }
      if ((*dimmNvList)[dimm].DcpmmPresent) {
        *maxSteps = 127;
      } else {
        *maxSteps = MAX_CMD_MARGIN;
      }
      if (group == CmdVref) {
        GetCmdGroupLimits (socket, ch, dimm, level, CmdVref, &MinLimit, &MaxLimit);
        *maxSteps = (UINT8) MIN (MAX_UINT8, (MaxLimit - MinLimit));
      }
    default:
      break;
    } //switch group
  } else
#endif //LRDIMM
  {
    if (group == RxVref) {
      if (sign) {
        if ((TrainingVar->rdVrefLo == 0) || (TrainingVar->rdVrefLo >= MAX_RD_VREF_OFFSET)) {
          offset[ch][ (*rankList)[rank].rankIndex] = -7;
        } else {
          offset[ch][ (*rankList)[rank].rankIndex] = 0 - (INT8) TrainingVar->rdVrefLo;
        }
      } else {
        if ((TrainingVar->rdVrefHi == 0) || (TrainingVar->rdVrefHi >= MAX_RD_VREF_OFFSET)) {
          offset[ch][ (*rankList)[rank].rankIndex] = 7;
        } else {
          offset[ch][ (*rankList)[rank].rankIndex] = (INT8) TrainingVar->rdVrefHi;
        }
      }
      *maxSteps = MAX_RD_VREF_OFFSET;
    } else if (group == RxSampler) {
#ifdef SPR_HOST
      if (sign) {
        if ((TrainingVar->RxSamplerLo == 0) || (TrainingVar->RxSamplerLo >= MAX_RX_SAMPLER_OFFSET)) {
          offset[ch][ (*rankList)[rank].rankIndex] = -7; //SPR_todo. HSD 16010478187 To confirm the value with EV?
        } else {
          offset[ch][ (*rankList)[rank].rankIndex] = 0 - (INT8) TrainingVar->RxSamplerLo;
        }
      } else {
        if ((TrainingVar->RxSamplerHi == 0) || (TrainingVar->RxSamplerHi >= MAX_RX_SAMPLER_OFFSET)) {
          offset[ch][ (*rankList)[rank].rankIndex] = 7; //SPR_todo. HSD HSD 16010478187 To confirm the value with EV?
        } else {
          offset[ch][ (*rankList)[rank].rankIndex] = (INT8) TrainingVar->RxSamplerHi;
        }
      }
      *maxSteps = MAX_RX_SAMPLER_OFFSET;
#endif //SPR_HOST
    } else if (group == TxVref) {
      if (sign) {
        if ((TrainingVar->wrVrefLo == 0) || (TrainingVar->wrVrefLo >= MAX_RD_VREF_OFFSET)) {
          offset[ch][ (*rankList)[rank].rankIndex] = -7;
        } else {
          offset[ch][ (*rankList)[rank].rankIndex] = 0 - (INT8) TrainingVar->wrVrefLo;
        }
      } else {
        if ((TrainingVar->wrVrefHi == 0) || (TrainingVar->wrVrefHi >= MAX_RD_VREF_OFFSET)) {
          offset[ch][ (*rankList)[rank].rankIndex] = 7;
        } else {
          offset[ch][ (*rankList)[rank].rankIndex] = (INT8) TrainingVar->wrVrefHi;
        }
      }
      if (IsDdr5Present (Host, socket)) {
        *maxSteps = MAX_WR_VREF_OFFSET_DDR5;
      }
      else if (IsMemFlowEnabled (LrdimmWriteVrefCenteringTraining)) {
        *maxSteps = MAX_WR_VREF_OFFSET_BACKSIDE;
      } else {
        *maxSteps = MAX_WR_VREF_OFFSET;
      }
    } else if ((group == RxDqsDelay) || (group == RxDqsPDelay) || (group == RxDqsNDelay)) {
      if (sign) {
        if ((*rankList)[rank].rxDQLeftSt >= MAX_TIMING_OFFSET) {
          (*rankList)[rank].rxDQLeftSt = MAX_TIMING_OFFSET - 5;
        }
        offset[ch][ (*rankList)[rank].rankIndex] = (INT8) (0 - (INT8) (*rankList)[rank].rxDQLeftSt);
      } else {
        if ((*rankList)[rank].rxDQRightSt >= MAX_TIMING_OFFSET) {
          (*rankList)[rank].rxDQRightSt = MAX_TIMING_OFFSET - 5;
        }
        offset[ch][ (*rankList)[rank].rankIndex] = (INT8) (*rankList)[rank].rxDQRightSt;
      }
      *maxSteps = MAX_TIMING_OFFSET;
    } else if (group == TxDqDelay) {
      if (sign) {
        if ((*rankList)[rank].txDQLeftSt >= MAX_TIMING_OFFSET) {
          (*rankList)[rank].txDQLeftSt = MAX_TIMING_OFFSET - 5;
        }
        offset[ch][ (*rankList)[rank].rankIndex] = (INT8) (0 - (INT8) (*rankList)[rank].txDQLeftSt);
      } else {
        if ((*rankList)[rank].txDQRightSt >= MAX_TIMING_OFFSET) {
          (*rankList)[rank].txDQRightSt = MAX_TIMING_OFFSET - 5;
        }
        offset[ch][ (*rankList)[rank].rankIndex] = (INT8) (*rankList)[rank].txDQRightSt;
      }
      *maxSteps = MAX_TIMING_OFFSET;
    } else if ((CheckCMDGroup (group) == 1) && (group != CmdVref)) {
      if (sign) {
        if ((*rankList)[rank].cmdLeft >= MAX_CMD_MARGIN) {
          (*rankList)[rank].cmdLeft = CMD_INIT_MARG;
        }
        offset[ch][ (*rankList)[rank].rankIndex] = (INT8) (0 - (INT8) (*rankList)[rank].cmdLeft);
      } else {
        if ((*rankList)[rank].cmdRight >= MAX_CMD_MARGIN) {
          (*rankList)[rank].cmdRight = CMD_INIT_MARG;
        }
        offset[ch][ (*rankList)[rank].rankIndex] = (INT8) (*rankList)[rank].cmdRight;
      }
      *maxSteps = MAX_CMD_MARGIN;
    } else if (group == CmdVref) {
      if (InternalRcdVrefCaEnabled () && (Host->nvram.mem.dimmTypePresent == RDIMM) && !((*dimmNvList)[dimm].DcpmmPresent)) {
        if (sign) {
          if ((*rankList)[rank].cmdLow >= MAX_CMD_MARGIN) {
            (*rankList)[rank].cmdLow = CMD_VREF_INIT_MARG;
          }
          offset[ch][ (*rankList)[rank].rankIndex] = (INT8) (0 - (INT8) (*rankList)[rank].cmdLow);
        } else {
          if ((*rankList)[rank].cmdHigh >= MAX_CMD_MARGIN) {
            (*rankList)[rank].cmdHigh = CMD_VREF_INIT_MARG;
          }
          offset[ch][ (*rankList)[rank].rankIndex] = (INT8) (*rankList)[rank].cmdHigh;
        }
        GetCmdGroupLimits (socket, ch, dimm, level, CmdVref, &MinLimit, &MaxLimit);
        *maxSteps = (UINT8) MIN (MAX_UINT8, (MaxLimit - MinLimit));
      } else {
        GetCmdGroupLimits (socket, ch, dimm, level, CmdVref, &MinLimit, &MaxLimit);
        if (sign) {
          if ((*rankList)[rank].cmdLow >= MAX_CMD_MARGIN) {
            (*rankList)[rank].cmdLow = CMD_INIT_MARG;
          }
          offset[ch][ (*rankList)[rank].rankIndex] = (INT8) (0 - (INT8) (*rankList)[rank].cmdLow);
        } else {
          if ((*rankList)[rank].cmdHigh >= MAX_CMD_MARGIN) {
            (*rankList)[rank].cmdHigh = CMD_INIT_MARG;
          }
          offset[ch][ (*rankList)[rank].rankIndex] = (INT8) (*rankList)[rank].cmdHigh;
        }
        *maxSteps = (UINT8) MIN (MAX_UINT8, (MaxLimit - MinLimit));
      }
    } else if (CheckCTLGroup (group)) {
      if (sign) {
        offset[ch][ (*rankList)[rank].rankIndex] = -15;
      } else {
        offset[ch][ (*rankList)[rank].rankIndex] = 15;
      }
      //*maxSteps = MAX_CMD_MARGIN;
      *maxSteps = 127;
    }
  }

} // GetStartingOffset

/**
  Returns the starting offset for the given test group

  @param Host            - Pointer to sysHost
  @param socket          - Processor socket to check
  @param group           - Test group
  @param level           - DDR or LRDIMM Host side, LRDIMM Backside, etc...
  @param scope           - Margin granularity
  @param marginData      - Pointer to the structure to store the margin results. The structure type varies based on scope.

  @retval N/A
**/
VOID
EFIAPI
UpdateStartingOffset (
  PSYSHOST  Host,
  UINT8     socket,
  MRC_GT    group,
  MRC_LT    level,
  UINT8     scope,
  VOID      *marginData
  )
{
  UINT8                   ch;
  UINT8                   dimm;
  UINT8                   rank;
  UINT8                   strobe;
  UINT8                   maxStrobe;
  UINT8                   bit;
  INT16                   pos;
  INT16                   neg;
  struct rankMargin       *rankM;
  struct strobeMargin     *strobeM;
  struct bitMargin        *bitM;
  struct channelNvram     (*channelNvList)[MAX_CH];
  struct dimmNvram        (*dimmNvList)[MAX_DIMM];
  struct ddrRank          (*rankList)[MAX_RANK_DIMM];
  struct TrainingVariable *TrainingVar;

  UINT8 MBV = GetMaxBitsValid (Host);
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  TrainingVar = MemTrainingVar ();
  channelNvList = GetChannelNvList (Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, socket, ch);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }

      rankList = GetRankNvList (Host, socket, ch, dimm);

      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {

        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        pos = 0x7F;
        neg = -0x7F;

        if (scope == SCOPE_RANK) {
          rankM = marginData;

          if ((*rankM).rank[ch][ (*rankList)[rank].rankIndex].p > 13) {
            pos = (*rankM).rank[ch][ (*rankList)[rank].rankIndex].p - 3;
          } else {
            pos = 10;
          }
          if ((*rankM).rank[ch][ (*rankList)[rank].rankIndex].n < -13) {
            neg = (*rankM).rank[ch][ (*rankList)[rank].rankIndex].n + 3;
          } else {
            neg = -10;
          }
        } else if (scope == SCOPE_STROBE) {
          strobeM = marginData;

          //
          // Setup variables based on x4 or x8 DIMM
          //
          maxStrobe = GetMaxStrobe (Host, socket, ch, dimm, group, level);

          for (strobe = 0; strobe < maxStrobe; strobe++) {
            //
            // Skip if this is an ECC strobe when ECC is disabled
            //
            if (IsStrobeNotValid (Host, strobe)) {
              continue;
            }

            if ((*strobeM).strobe[ch][ (*rankList)[rank].rankIndex][strobe].n > (*strobeM).strobe[ch][ (*rankList)[rank].rankIndex][strobe].p) {
              (*strobeM).strobe[ch][ (*rankList)[rank].rankIndex][strobe].n = 0;
            }
            if ((*strobeM).strobe[ch][ (*rankList)[rank].rankIndex][strobe].p > 13) {
              if (((*strobeM).strobe[ch][ (*rankList)[rank].rankIndex][strobe].p - 3) < pos) {
                pos = (*strobeM).strobe[ch][ (*rankList)[rank].rankIndex][strobe].p - 3;
              }
            } else {
              pos = 10;
            }
            if ((*strobeM).strobe[ch][ (*rankList)[rank].rankIndex][strobe].n < -13) {
              if (((*strobeM).strobe[ch][ (*rankList)[rank].rankIndex][strobe].n + 3) > neg) {
                neg = (*strobeM).strobe[ch][ (*rankList)[rank].rankIndex][strobe].n + 3;
              }
            } else {
              neg = -10;
            }
          } // strobe loop
        } else if (scope == SCOPE_BIT) {
          bitM = marginData;
          rankList = GetRankNvList (Host, socket, ch, dimm);

          for (bit = 0; bit < MBV; bit++) {
            if (IsBitNotValid (Host, bit)) {
              continue;
            }
            if ((*bitM).bits[ch][ (*rankList)[rank].rankIndex][bit].p > 13) {
              if (((*bitM).bits[ch][ (*rankList)[rank].rankIndex][bit].p - 3) < pos) {
                pos = (*bitM).bits[ch][ (*rankList)[rank].rankIndex][bit].p - 3;
              }
            } else {
              pos = 10;
            }
            if ((*bitM).bits[ch][ (*rankList)[rank].rankIndex][bit].n < -13) {
              if (((*bitM).bits[ch][ (*rankList)[rank].rankIndex][bit].n + 3) > neg) {
                neg = (*bitM).bits[ch][ (*rankList)[rank].rankIndex][bit].n + 3;
              }
            } else {
              neg = -10;
            }
          } // bit loop
        } // scope

        if ((pos != 0x7F) && (neg != -0x7F)) {
          if (group == RxVref) {
            TrainingVar->rdVrefLo = (UINT8) (0 - neg);
            TrainingVar->rdVrefHi = (UINT8) pos;
          } else if (group == TxVref) {
            TrainingVar->wrVrefLo = (UINT8) (0 - neg);
            TrainingVar->wrVrefHi = (UINT8) pos;
          } else if ((group == RxDqsDelay) || (group == RxDqsPDelay) || (group == RxDqsNDelay)) {
            (*rankList)[rank].rxDQLeftSt = (UINT8) (0 - neg);
            (*rankList)[rank].rxDQRightSt = (UINT8) pos;
          } else if (group == TxDqDelay) {
            (*rankList)[rank].txDQLeftSt = (UINT8) (0 - neg);
            (*rankList)[rank].txDQRightSt = (UINT8) pos;
          } else if (group == CmdVref) {
            (*rankList)[rank].cmdLow = (UINT8) (0 - neg);
            (*rankList)[rank].cmdHigh = (UINT8) pos;
          } else if (group == RxSampler) {
#ifdef SPR_HOST
            TrainingVar->RxSamplerLo = (UINT8) (0 - neg);
            TrainingVar->RxSamplerHi = (UINT8) pos;
#endif //SPR_HOST
          }
        }
      } // rank loop
    } // dimm loop
  } // ch loop
} // UpdateStartingOffset

/**
  Set the starting offsets for CMD timging margin sweeps to be close
  to the failing edges found during training. The purpose is to
  reduce time spent margining CMD timing.
  This routine will be called after Early and Late CMD Training.

  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number
  @param[in]  Clk       - Clock number
  @param[in]  Margins   - Pointer to structure with left and right edges

  @retval   EFI_SUCCESS if starting offsets are updated
            EFI_INVALID_PARAMETER otherwise

**/
EFI_STATUS
EFIAPI
UpdateStartingOffsetCmd (
  IN    UINT8               Socket,
  IN    UINT8               Ch,
  IN    UINT8               Clk,
  IN    struct baseMargin   *Margins
  )
{
  PSYSHOST            Host;
  UINT8               Dimm;
  UINT8               Rank;
  INT16               RightEdge;
  INT16               LeftEdge;
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct channelNvram (*ChannelNvList)[MAX_CH];

  if (Margins == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Margins->p > CMD_MARG_OFFSET) {
    RightEdge = Margins->p - CMD_MARG_OFFSET;
  } else {
    RightEdge = CMD_MARG_OFFSET;
  }
  if (Margins->n < -CMD_MARG_OFFSET) {
    LeftEdge = Margins->n + CMD_MARG_OFFSET;
  } else {
    LeftEdge = -CMD_MARG_OFFSET;
  }

  Host = GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }

    RankList = GetRankNvList (Host, Socket, Ch, Dimm);

    for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {

      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
        continue;
      }

      if (Clk == (*RankList)[Rank].ckIndex) {
        (*RankList)[Rank].cmdLeft = (UINT8) (ABS (LeftEdge));
        (*RankList)[Rank].cmdRight = (UINT8) RightEdge;
      }
    } // Rank
  } // Dimm

  return EFI_SUCCESS;
} // UpdateStartingOffsetCmd

UINT8
CheckAllPassFail (
  PSYSHOST  Host,
  UINT8     found[MAX_STROBE/2]
  )
/**

  Checks to see if all the bits are set

  @param Host        - Pointer to sysHost
  @param found       - Array of status bits

  @retval 0 - All set
  @retval 1 - Not all set

**/
{
  UINT8 strobe;
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;

  for (strobe = 0; strobe < MSVx8; strobe++) {
    if (IsStrobeNotValid (Host, strobe)) {
      continue;
    }
    if (found[strobe] != 0xFF) {
      return 1;
    }
  } // strobe loop

  // Return 0 if all bits are set
  return 0;
} // CheckAllPassFail

VOID
SetAllFound (
  PSYSHOST  Host,
  UINT8     ch,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bitMask,
  UINT8     found[MAX_CH][MAX_RANK_CH][MAX_STROBE/2]
  )
/**
  Sets all the status bits

  @param Host        - Pointer to sysHost
  @param ch          - Current Channel
  @param rank        - Current Rank
  @param strobe      - Current Strobe
  @param bitMask     - Value to save into the found array
  @param found       - Array of status bits

  @retval N/A

**/
{
  UINT8 Socket;
  UINT8 maxCh;
  UINT8 maxRank;
  UINT8 startRank;
  UINT8 maxStrobe;
  UINT8 startStrobe;
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;

  Socket = Host->var.mem.currentSocket;

  if (ch == SET_ALL) {
    ch = 0;
    maxCh = MAX_CH;
  } else {
    maxCh = ch + 1;
  }
  if (maxCh > MAX_CH) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, ch, NO_DIMM, NO_SUBCH, rank, strobe, NO_BIT, "\nInvalid channel number, ch=%d\n", ch);
    ASSERT (FALSE);
    return;
  }
  if (rank == SET_ALL) {
    startRank = 0;
    maxRank = MAX_RANK_CH;
  } else {
    startRank = rank;
    maxRank = rank + 1;
  }
  if (maxRank > MAX_RANK_CH) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, ch, NO_DIMM, NO_SUBCH, rank, strobe, NO_BIT, "\nInvalid rank number, rank=%d\n", rank);
    ASSERT (FALSE);
    return;
  }
  if (strobe == SET_ALL) {
    startStrobe = 0;
    maxStrobe = MSVx8;
  } else {
    startStrobe = strobe;
    maxStrobe = strobe + 1;
  }
  if (maxStrobe > (MAX_STROBE/2)) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, ch, NO_DIMM, NO_SUBCH, rank, strobe, NO_BIT, "\nInvalid strobe number, strobe=%d\n", strobe);
    ASSERT (FALSE);
    return;
  }

  for (; ch < maxCh; ch++) {
    for (rank = startRank; rank < maxRank; rank++) {
      for (strobe = startStrobe; strobe < maxStrobe; strobe++) {
        found[ch][rank][strobe] |= bitMask;
      } // strobe loop
    } // rank loop
  } // ch loop
} // SetAllFound

VOID
ClearAllFound (
  PSYSHOST  Host,
  UINT8     ch,
  UINT8     rank,
  UINT8     strobe,
  UINT8     found[MAX_CH][MAX_RANK_CH][MAX_STROBE/2]
  )
/**

  Clears all the status bits

  @param Host        - Pointer to sysHost
  @param ch          - Current Channel
  @param rank        - Current Rank
  @param strobe      - Current Strobe
  @param found       - Array of status bits

  @retval N/A

**/
{
  UINT8 Socket;
  UINT8 maxCh;
  UINT8 maxRank;
  UINT8 startRank;
  UINT8 maxStrobe;
  UINT8 startStrobe;
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;

  Socket = Host->var.mem.currentSocket;

  if (ch == SET_ALL) {
    ch = 0;
    maxCh = MAX_CH;
  } else {
    maxCh = ch + 1;
  }
  if (maxCh > MAX_CH) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, ch, NO_DIMM, NO_SUBCH, rank, strobe, NO_BIT, "\nInvalid channel number, ch=%d\n", ch);
    ASSERT (FALSE);
    return;
  }
  if (rank == SET_ALL) {
    startRank = 0;
    maxRank = MAX_RANK_CH;
  } else {
    startRank = rank;
    maxRank = rank + 1;
  }
  if (maxRank > MAX_RANK_CH) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, ch, NO_DIMM, NO_SUBCH, rank, strobe, NO_BIT, "\nInvalid rank number, rank=%d\n", rank);
    ASSERT (FALSE);
    return;
  }
  if (strobe == SET_ALL) {
    startStrobe = 0;
    maxStrobe = MSVx8;
  } else {
    startStrobe = strobe;
    maxStrobe = strobe + 1;
  }
#ifdef DDR5_SUPPORT
  if (maxStrobe > (MAX_STROBE_DDR5 / 2)) {
    RcDebugPrintWithDevice(SDBG_MINMAX, Socket, ch, NO_DIMM, NO_SUBCH, rank, strobe, NO_BIT, "\nInvalid strobe number, strobe=%d\n", strobe);
    ASSERT(FALSE);
    return;
  }
#else
  if (maxStrobe > (MAX_STROBE/2)) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, ch, NO_DIMM, NO_SUBCH, rank, strobe, NO_BIT, "\nInvalid strobe number, strobe=%d\n", strobe);
    ASSERT (FALSE);
    return;
  }
#endif //DDR5_SUPPORT

  for (; ch < maxCh; ch++) {
    for (rank = startRank; rank < maxRank; rank++) {

      ZeroMem (&found[ch][rank][startStrobe], (maxStrobe - startStrobe));
    } // rank loop
  } // ch loop
} // ClearAllFound

/**

  @param Host            - Pointer to sysHost
  @param socket          - Processor socket to check
  @param ch              - Current Channel
  @param dimm            - Dimm number (0-based)
  @param rank            - Rank number (0-based)
  @param offset          - Pointer to offset
  @param level           - DDR or LRDIMM Host side, LRDIMM Backside, etc...
  @param group           - Test group
  @param previousOffset  - Previous offset

  @retval SUCCESS
  @retval FAILURE

**/
UINT32
SetOffset (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  INT16     offset,
  MRC_LT    level,
  MRC_GT    group,
  INT16     previousOffset
  )
{
  UINT16  cmdMinVal = MAX_CMD_MARGIN;
  UINT16  cmdMaxVal = 0;
  UINT32  status = SUCCESS;
  INT16   piOffset;

  RcDebugPrint (SDBG_NONE,
                 "SetOffset\n");
  switch (group) {
  case RxVref:
  case RxSampler:
  case RxSamplerEvenOdd:
  case TxVref:
  case RxDqsDelay:
  case RxDqsPDelay:
  case RxDqsNDelay:
  case TxDqDelay:
  case RecEnDelay:
  case WrLvlDelay:
    status = GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, ALL_BITS, level, group, GSM_FORCE_WRITE | GSM_WRITE_OFFSET,
               &offset);
    break;
  case CmdGrp0:
  case CmdGrp1:
  case CmdGrp2:
  case CmdGrp3:
  case CmdGrp4:
  case CmdGrp5:
  case CmdAll:
  case Cmd0All:
  case Cmd1All:
    piOffset = offset - previousOffset;
    status = GetSetCmdGroupDelayCore (Host, socket, ch, dimm, level, group, GSM_SKIP_CMD_FUB | GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &piOffset, &cmdMinVal, &cmdMaxVal);
    break;
  case CmdVref:
    piOffset = offset - previousOffset;
    status = GetSetCmdVrefCore (Host, socket, ch, dimm, level, GSM_SKIP_CMD_FUB | GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &piOffset);
    break;
  case CtlGrp0:
  case CtlGrp1:
  case CtlGrp2:
  case CtlGrp3:
  case CtlGrp4:
  case CtlGrp5:
  case CtlAll:
  case Ctl0Grp:
  case Ctl1Grp:
    piOffset = offset - previousOffset;
    status = GetSetCtlGroupDelayCore (Host, socket, ch, dimm, level, group, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &piOffset, &cmdMinVal, &cmdMaxVal);
    break;
  default:
    break;
  }

  //
  // Reset the IO
  //
  IO_Reset (Host, socket);

  return status;
} // SetOffset

/**

  Function to offset the final training results to facilitate customer debug.
  After offsetting training results, RMT will be re-run if it is enabled. Final training result
  will be displayed as well.

  @param[in] Host  - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
EFIAPI
OffsetTrainingResult (
  IN PSYSHOST Host
  )
{
  UINT8               Socket;
  UINT8               OriginalSocket;
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               Rank;
  SYS_SETUP           *Setup;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  if (GetSysBootMode () == S3Resume || Host->var.mem.subBootMode == WarmBootFast || Host->var.mem.subBootMode == ColdBootFast ) {
    return SUCCESS;
  }

  if (Setup->mem.trainingResultOffsetFunctionEnable == FALSE) {
    return SUCCESS;
  }

  //
  // Change offsets to their actual values (-ve or + ve). Default = 0.
  // + ve starts at 0 --> for an offset of + 7, enter 7; -ve starts at 100 --> for an offset of -5, enter 105
  //
  if (Setup->mem.offsetTxDq >= 100) {
    Setup->mem.offsetTxDq = 100 - Setup->mem.offsetTxDq; //-ve
  }

  if (Setup->mem.OffsetRecEn >= 100) {
    Setup->mem.OffsetRecEn = 100 - Setup->mem.OffsetRecEn;
  }

  if (Setup->mem.offsetTxVref >= 100) {
    Setup->mem.offsetTxVref = 100 - Setup->mem.offsetTxVref;
  }

  if (Setup->mem.offsetRxDq >= 100) {
    Setup->mem.offsetRxDq = 100 - Setup->mem.offsetRxDq;
  }

  if (Setup->mem.offsetRxVref >= 100) {
    Setup->mem.offsetRxVref = 100 - Setup->mem.offsetRxVref;
  }

  if (Setup->mem.offsetCmdAll >= 100) {
    Setup->mem.offsetCmdAll = 100 - Setup->mem.offsetCmdAll;
  }

  if (Setup->mem.offsetCmdVref >= 100) {
    Setup->mem.offsetCmdVref = 100 - Setup->mem.offsetCmdVref;
  }

  if (Setup->mem.offsetCtlAll >= 100) {
    Setup->mem.offsetCtlAll = 100 - Setup->mem.offsetCtlAll;
  }


  RcDebugPrint (SDBG_DEFAULT,"\nPrinting Offsets that will be applied to given signal\n");

  RcDebugPrint (SDBG_DEFAULT,"START_OFFSET_TRAIN_RESULT_LIMITS_TABLE\n");
  RcDebugPrint (SDBG_DEFAULT, "offsetRecEn = %d\n", Setup->mem.OffsetRecEn);
  RcDebugPrint (SDBG_DEFAULT, "offsetTxDq = %d\n", Setup->mem.offsetTxDq);
  RcDebugPrint (SDBG_DEFAULT, "offsetRxDq = %d\n", Setup->mem.offsetRxDq);
  RcDebugPrint (SDBG_DEFAULT, "offsetTxVref = %d\n", Setup->mem.offsetTxVref);
  RcDebugPrint (SDBG_DEFAULT, "offsetRxVref = %d\n", Setup->mem.offsetRxVref);
  RcDebugPrint (SDBG_DEFAULT, "offsetCmdAll = %d\n", Setup->mem.offsetCmdAll);
  RcDebugPrint (SDBG_DEFAULT, "offsetCmdVref = %d\n", Setup->mem.offsetCmdVref);
  RcDebugPrint (SDBG_DEFAULT, "offsetCtlAll = %d\n", Setup->mem.offsetCtlAll);
  RcDebugPrint (SDBG_DEFAULT,"STOP_OFFSET_TRAIN_RESULT_LIMITS_TABLE\n");

  //
  // All offsets will be global and affect all ranks on all channels on all cpu's.
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    //
    // Skip disabled sockets or If Socket does not have Dimms
    //
    if (!IsSocketDimmPopulated (Host, Socket)) {
      continue;
    }

    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      //
      // Skip disabled channels
      //
      if (!IsChannelEnabled (Socket, Ch)) {
        continue;
      }

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        //
        // Skip if DIMM is not present
        //
        if (!IsDimmPresent (Socket, Ch, Dimm)) {
          continue;
        }

        for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          //
          // Change all parameters w.r.t. offsets across all ranks
          //
          TrainingResultOffsetFunction (Host, Socket, Ch, Dimm, Rank, Setup->mem.offsetTxDq, DdrLevel, TxDqDelay);
          TrainingResultOffsetFunction (Host, Socket, Ch, Dimm, Rank, Setup->mem.offsetTxVref, DdrLevel, TxVref);
          TrainingResultOffsetFunction (Host, Socket, Ch, Dimm, Rank, Setup->mem.offsetRxDq, DdrLevel, RxDqsDelay);
          TrainingResultOffsetFunction (Host, Socket, Ch, Dimm, Rank, Setup->mem.OffsetRecEn, DdrLevel, RecEnDelay);
        }//Rank
      }//Dimm
    }//Ch

    for (Ch = 0; Ch < MaxChDdr; Ch++) {

      if (!IsChannelEnabled (Socket, Ch)) {
        continue;
      }
      //
      // CMD/CTL offset functions called per Ch
      //
      TrainingResultOffsetFunction (Host, Socket, Ch, 0, 0, Setup->mem.offsetCmdAll, DdrLevel, CmdAll);
      TrainingResultOffsetFunction (Host, Socket, Ch, 0, 0, Setup->mem.offsetCmdVref, DdrLevel, CmdVref);
      TrainingResultOffsetFunction (Host, Socket, Ch, 0, 0, Setup->mem.offsetCtlAll, DdrLevel, CtlAll);
      TrainingResultOffsetFunction (Host, Socket, Ch, 0, 0, Setup->mem.offsetRxVref, DdrLevel, RxVref);
    } //Ch
  } // Socket

  OriginalSocket = Host->var.mem.currentSocket;

  RcDebugPrint (SDBG_DEFAULT,"\nRunning Rank Margin Tool, if it is enabled\n");

  //
  // Call BSSA RMT
  //
  RunBuiltInBssaRmt ();

  //
  // Call legacy RMT if it is enabled
  //
  if (((Setup->mem.options & MARGIN_RANKS_EN) != 0) && (IsMemFlowEnabled (LegacyRankMarginTool)))  {
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {

      if (!IsSocketDimmPopulated (Host, Socket)) {
        continue;
      }

      Host->var.mem.currentSocket = Socket;
      RankMarginTool (Host);
    } // Socket
  }

  RcDebugPrint (SDBG_DEFAULT,"\nStopping Rank Margin Tools, if it is enabled\n");

#ifdef DEBUG_CODE_BLOCK
  RcDebugPrint (SDBG_DEFAULT,"\nDisplay Signal Values after applying Offset\n");

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {

    if (!IsSocketDimmPopulated (Host, Socket)) {
      continue;
    }

    Host->var.mem.currentSocket = Socket;
    DisplayTrainResults (Host);
  } // Socket
#endif  // DEBUG_CODE_BLOCK

  Host->var.mem.currentSocket = OriginalSocket;

  return SUCCESS;
}//OffsetTrainingResult

/*
  Function to offset the final RecEn training results before entering the OS that would facilitate customer debug.

  @param[in] Host - Pointer to sysHost, the system host (root) structure struct
  @param[in] Socket - 0 based socket number

  @retval SUCCESS

**/
UINT32
EFIAPI
TrainingResultRecEnOffsetCfg (
  IN PSYSHOST Host,
  IN UINT8    Socket
)
{

  UINT8               Ch;
  UINT8               Dimm;
  UINT8               Rank;
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct channelNvram (*ChannelNvList)[MAX_CH];
  SYS_SETUP           *Setup;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  if (!IsSocketDimmPopulated (Host, Socket)) {
    return SUCCESS;
  }

  ChannelNvList = GetChannelNvList(Host, Socket);

  // Change offsets to their actual values (-ve or +ve). Default = 0.
  // +ve starts at 0 --> for an offset of +7, enter 7; -ve starts at 100 --> for an offset of -5, enter 105
  if (Setup->mem.OffsetRecEn >= 100) {
    Setup->mem.OffsetRecEn = 100 - Setup->mem.OffsetRecEn; //-ve
  }
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    //
    // Skip disabled channels
    //
    if (!IsChannelEnabled (Socket, Ch)) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (!IsDimmPresent (Socket, Ch, Dimm)) {
        continue;
     }

      for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        TrainingResultOffsetFunction (Host, Socket, Ch, Dimm, Rank, Setup->mem.OffsetRecEn, DdrLevel, RecEnDelay);
      }//rank
    }//dimm
  }//ch

  RcDebugPrint (SDBG_DEFAULT, "OffsetRecEn = %d\n", Setup->mem.OffsetRecEn);
  return SUCCESS;
}
/**

  Function to offset the final training results before entering the OS that would facilitate customer debug.

  @param Host            - Pointer to sysHost
  @param socket          - Processor socket to check
  @param ch              - Current Channel
  @param dimm            - Dimm number (0-based)
  @param rank            - Rank number (0-based)
  @param offset          - Pointer to offset
  @param level           - DDR or LRDIMM Host side, LRDIMM Backside, etc...
  @param group           - Test group

  @retval NULL
**/

VOID
TrainingResultOffsetFunction (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  INT16     offset,
  MRC_LT    level,
  MRC_GT    group
  )
{
  UINT16  cmdMinVal = MAX_CMD_MARGIN;
  UINT16  cmdMaxVal = 0;

  //Offsets can be programmed by calling the respective functions for each group
  switch (group) {
  case RxVref:
  case RxSampler:
  case TxVref:
  case RxDqsDelay:
  case TxDqDelay:
  case RecEnDelay:
    GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, ALL_BITS, level, group, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &offset);
    break;
  case CmdAll:
    GetSetCmdGroupDelayCore (Host, socket, ch, dimm, level, group, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &offset, &cmdMinVal, &cmdMaxVal);
    break;
  case CmdVref: //CmdVref is used for both CMD/CTL Vref
    GetSetCmdVrefCore (Host, socket, ch, dimm, level, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &offset);
    break;
  case CtlAll:
    GetSetCtlGroupDelayCore (Host, socket, ch, dimm, level, group,  GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &offset, &cmdMinVal, &cmdMaxVal);
    break;
  default:
    break;
  }//case

  //
  // Reset the IO
  //
  IO_Reset (Host, socket);
} //TrainingResultOffsetFunction


UINT32 *
CheckRange (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  INT16     offset,
  UINT8     sign,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     foundFail[MAX_STROBE/2],
  UINT8     foundPass[MAX_STROBE/2],
  UINT8     scope,
  INT16     previousOffset,
  VOID      *marginData,
  UINT32    beyondRange[3]
  )
/**
  Check if we've reached the max or min for the current margin offset

  @param Host           - Pointer to sysHost
  @param socket         - Processor socket to check
  @param ch             - Current Channel
  @param dimm           - Dimm number (0-based)
  @param rank           - Rank number (0-based)
  @param offset         - Pointer to offset
  @param sign           - The direction we are currently going (up, down, left or right)
  @param level          - DDR or LRDIMM Host side, LRDIMM Backside, etc...
  @param group          - Test group
  @param foundFail      - Bit mask marking passes
  @param foundPass      - Bit mask marking fails
  @param scope          - Margin granularity
  @param previousOffset - Previous offset
  @param marginData     - pointer to Margin data
  @param beyondRange    - pointer to the beyondRange array

  @retval Pointer to the beyondRange array

**/
{
  UINT8   strobe = 0;
  UINT8   maxStrobe;
  UINT8   update;
  UINT8   loop;
  INT16   maxDelay = 0;
  INT16   minDelay = 0;
  UINT16  maxCmdDelay = 0;
  UINT16  minCmdDelay = 0;
  UINT16  usDelay = 0;
  INT16   delay = 0;
  INT16   piOffset;
  MRC_GT  group2;
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;

  if ((MSVx8 > (MAX_STROBE/2)) || (MSVx4 > MAX_STROBE)) {
    RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT, "\nInvalid max strobe number, strobe=%d\n", MSVx8);
    ASSERT (FALSE);
    return 0;
  }

  beyondRange[0] = 0;
  beyondRange[1] = 0;
  beyondRange[2] = 0;

  if ((CheckCMDGroup (group) || CheckCTLGroup (group)) && (group != CmdVref)) {
    //
    // Calculate the new setting to be tested
    //
    minCmdDelay = MAX_CMD_MARGIN;
    maxCmdDelay = 0;
    if (CheckCMDGroup (group)) {
      GetSetCmdGroupDelayCore (Host, socket, ch, dimm, level, group, GSM_READ_ONLY, &delay, &minCmdDelay, &maxCmdDelay);
    } else {
      GetSetCtlGroupDelayCore (Host, socket, ch, dimm, level, group, GSM_READ_ONLY, &delay, &minCmdDelay, &maxCmdDelay);
    }

    //
    // Get min/max delay
    //
    if (level == LrbufLevel) {
      dimmNvList = GetDimmNvList (Host, socket, ch);
      if ((*dimmNvList)[dimm].DcpmmPresent) {
        minDelay = 0;
        maxDelay = 127;
      } else {
        if (sign) {
          minDelay = 0;
          maxDelay = 32;
        } else {
          minDelay = -1;
          maxDelay = 31;
        }
      }
    } else {
      minDelay = 0;
      maxDelay = MAX_CMD_MARGIN;
    }
    //
    // Flag to indicate if we need to update margins or not
    //
    update = 0;

    piOffset = offset - previousOffset;
    //
    // Check if the new setting is out of range
    //
    if (((INT16) (minCmdDelay + piOffset) <= minDelay) || ((maxCmdDelay + piOffset) >= maxDelay)) {
      for (strobe = 0; strobe < MSVx4; strobe++) {
        beyondRange[ (strobe % MSVx8) / 4] |= 0xF << (0x8 * ((strobe % MSVx8) % 4) + 0x4 * (strobe / MSVx8));
      } // strobe loop
      //
      // Do not update if it has already failed
      //
      if (((foundFail[0] & 0xFF) != 0xFF) && ((foundPass[0] & 0xFF) == 0xFF)) {
        update = 1;

        SetMem (foundFail, MSVx8, 0xFF);
      }

      //
      // Update the margin if this is the first time it failed
      //
      if (update) {
        for (strobe = 0; strobe < MSVx8; strobe++) {
          SaveThisMargin (Host, socket, ch, dimm, rank, strobe, MSVx4, offset, sign, scope, foundFail, foundPass, marginData);
        }
      }
    }

  } else if (CheckCMDGroup (group) && (group == CmdVref)) {
    dimmNvList = GetDimmNvList (Host, socket, ch);
    if (((level == LrbufLevel) && !((*dimmNvList)[dimm].DcpmmPresent)) ||
        ((level == DdrLevel) && InternalRcdVrefCaEnabled () && (Host->nvram.mem.dimmTypePresent == RDIMM))) {
      if (sign) {
        minDelay = -20;
        maxDelay = 1;
      } else {
        minDelay = -1;
        maxDelay = 20;
      }
    } else {
      GetCmdGroupLimits (socket, ch, dimm, level, CmdVref, &minDelay, &maxDelay);
    }

    //
    // Calculate the new setting to be tested
    //
    GetSetCmdVrefCore (Host, socket, ch, dimm, level, GSM_READ_ONLY, &delay);
    //
    // Flag to indicate if we need to update margins or not
    //
    update = 0;
    piOffset = offset - previousOffset;
    //
    // Check if the new setting is out of range
    //
    if (((INT16) (delay + piOffset) <= minDelay) || ((delay + piOffset) >= maxDelay)) {
      for (strobe = 0; strobe < MSVx4; strobe++) {
        beyondRange[ (strobe % MSVx8) / 4] |= 0xF << (0x8 * ((strobe % MSVx8) % 4) + 0x4 * (strobe / MSVx8));
      } // strobe loop

      //
      // Do not update if it has already failed
      //
      if (((foundFail[0] & 0xFF) != 0xFF) && ((foundPass[0] & 0xFF) == 0xFF)) {
        update = 1;
        SetMem (foundFail, MSVx8, 0xFF);
      }
      //
      // Update the margin if this is the first time it failed
      //
      if (update) {
        for (strobe = 0; strobe < MSVx8; strobe++) {
          SaveThisMargin (Host, socket, ch, dimm, rank, strobe, MSVx4, offset, sign, scope, foundFail, foundPass, marginData);
        }
      }
    }
  } else {
    if ((group == RxDqsDelay) || (group == TxDqDelay) || (group == RecEnDelay) || (group == WrLvlDelay) ||
        (group == RxVref) || (group == TxVref) || (group == RxDqsPDelay) || (group == RxDqsNDelay) || (group == RxSampler)) {

      maxStrobe = GetMaxStrobe (Host, socket, ch, dimm, group, level);

      //
      // Check each data group
      //
      for (strobe = 0; strobe < maxStrobe; strobe++) {
        if (group == RxDqsDelay && level == DdrLevel) {
          loop = 0;
          group2 = RxDqsPDelay;
        } else {
          loop = 1;
          group2 = group;
        }
        for (; loop < 2; loop++) {
          if (group == RxDqsDelay && level == DdrLevel && loop == 1) {
            group2 = RxDqsNDelay;
          }
          //
          // Flag to indicate if we need to update margins or not
          //
          update = 0;

          //
          // Calculate the new setting to be tested
          //
          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, ALL_BITS, level, group2, GSM_READ_ONLY, &delay);
          delay = delay + offset;

          //
          // Get min/max delay
          //
          GetDataGroupLimits (Host, level, group2, (UINT16 *)&minDelay, (UINT16 *)&maxDelay, &usDelay);
          UpdateDdrtGroupLimits (Host, socket, ch, dimm, level, group2, (UINT16 *)&minDelay, (UINT16 *)&maxDelay);

          //
          // Check if the new setting is out of range
          //
          if ((delay <= minDelay) || (delay >= maxDelay)) {
            if (maxStrobe <= MSVx8) {
              beyondRange[ (strobe % MSVx8) / 4] |= 0xFF << 0x8 * (strobe % 4);
            } else {
              beyondRange[ (strobe % MSVx8) / 4] |= 0xF << (0x8 * ((strobe % MSVx8) % 4) + 0x4 * (strobe / MSVx8));
            }
            if (strobe < MSVx8) {
              if(maxStrobe <= MSVx8) {  // Per byte
                if (((foundFail[strobe] & 0xFF) != 0xFF) && ((foundPass[strobe] & 0xFF) == 0xFF)) {
                  update = 1;
                  foundFail[strobe] = foundFail[strobe] | 0xFF;
                }
              } else { // Per nibble
                if (((foundFail[strobe] & 0x0F) != 0x0F) && ((foundPass[strobe] & 0x0F) == 0x0F)) {
                  update = 1;
                  foundFail[strobe] = foundFail[strobe] | 0x0F;
                }
              }
            } else {
              if ((strobe - MSVx8) >= (MAX_STROBE/2)) {
                RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT, "\nInvalid strobe index =%d\n", (strobe - MSVx8));
                ASSERT (FALSE);
                return 0;
              }
              if (((foundFail[strobe - MSVx8] & 0xF0) != 0xF0) && ((foundPass[strobe - MSVx8] & 0xF0) == 0xF0)) {
                update = 1;
                foundFail[strobe - MSVx8] = foundFail[strobe - MSVx8] | 0xF0;
              }
            }
            //
            // Update the margin if this is the first time it failed
            //
            if (update) {
              SaveThisMargin (Host, socket, ch, dimm, rank, strobe, maxStrobe, offset, sign, scope, foundFail, foundPass, marginData);
            }
          }
        }
      } // strobe loop
    }
  } // if CMD

  return &beyondRange[0];
} // CheckRange

/**
  Check if we've reached the max or min for the current offset

  @param Host          - Pointer to sysHost
  @param socket        - Processor socket to check
  @param ch            - Current Channel
  @param dimm          - Dimm number (0-based)
  @param rank          - Rank number (0-based)
  @param strobe        - Current Strobe
  @param maxStrobe     - Maximum number of strobes
  @param offset        - Pointer to offset
  @param sign          - The direction we are currently going (up, down, left or right)
  @param scope         - Margin granularity
  @param marginData    - pointer to Margin data

**/
VOID
SaveThisMargin (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     maxStrobe,
  INT16     offset,
  UINT8     sign,
  UINT8     scope,
  UINT8     foundFail[MAX_STROBE/2],
  UINT8     foundPass[MAX_STROBE/2],
  VOID      *marginData
  )
{
  UINT8               bit;
  UINT8               Loop;
  struct baseMargin   *socketM;
  struct chMargin     *channelM;
  struct rankMargin   *rankM;
  struct strobeMargin *strobeM;
  struct bitMargin    *bitM;
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;

  if (scope == SCOPE_SOCKET) {
    socketM = marginData;

    // Save new offset at the failing point
    if (sign) {
      (*socketM).n = offset;
    } else (*socketM).p = offset;
  } else if (scope == SCOPE_CH) {
    channelM = marginData;

    // Save new offset at the failing point
    if (sign) {
      (*channelM).channel[ch].n = offset;
    } else (*channelM).channel[ch].p = offset;
  } else if (scope == SCOPE_DIMM) {
  } else if (scope == SCOPE_RANK) {
    rankM = marginData;
    rankList = GetRankNvList (Host, socket, ch, dimm);

    // Save new offset at the failing point
    if (sign) {
      (*rankM).rank[ch][ (*rankList)[rank].rankIndex].n = offset;
    } else (*rankM).rank[ch][ (*rankList)[rank].rankIndex].p = offset;
  } else if (scope == SCOPE_STROBE) {
    strobeM = marginData;
    rankList = GetRankNvList (Host, socket, ch, dimm);

    // Save new offset at the failing point
    if (sign) {
      (*strobeM).strobe[ch][ (*rankList)[rank].rankIndex][strobe].n = offset;
      if (maxStrobe <= MSVx8 && strobe < MSVx8) {
        (*strobeM).strobe[ch][ (*rankList)[rank].rankIndex][strobe + MSVx8].n = offset;
      }
    } else {
      (*strobeM).strobe[ch][ (*rankList)[rank].rankIndex][strobe].p = offset;
      if (maxStrobe <= MSVx8 && strobe < MSVx8) {
        (*strobeM).strobe[ch][ (*rankList)[rank].rankIndex][strobe + MSVx8].p = offset;
      }
    }
  } else if (scope == SCOPE_BIT) {
    bitM = marginData;
    rankList = GetRankNvList (Host, socket, ch, dimm);

    bit = 0x8 * (strobe % MSVx8) + 0x4 * (strobe / MSVx8);
    Loop = 0x8 * (strobe % MSVx8) + 0x4 * (strobe / MSVx8) + 8 / (maxStrobe / MSVx8);
    for (; bit < Loop; bit++) {
      // Save new margin at the failing point if there wasn't already a failure
      if ((foundFail[strobe%MSVx8] & (1 << (bit % 8))) == 0) {
        if (sign) {
          (*bitM).bits[ch][ (*rankList)[rank].rankIndex][bit].n = offset;
        } else  {
          (*bitM).bits[ch][ (*rankList)[rank].rankIndex][bit].p = offset;
        }
      }
    } // bit loop
  }
} // SaveThisMargin

/**
  Restore Offset

  @param Host          - Pointer to sysHost
  @param socket        - Processor socket to check
  @param ch            - Current Channel
  @param dimm          - Dimm number (0-based)
  @param rank          - Rank number (0-based)
  @param offset        - Offset to restore
  @param level         - DDR or LRDIMM Host side, LRDIMM Backside, etc...
  @param group         - Test group

  @retval N/A
**/
VOID
RestoreOffset (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  INT16     restoreOffset,
  MRC_LT    level,
  MRC_GT    group
  )
{
  INT16   zeroOffset = 0;
  UINT16  cmdMinVal = MAX_CMD_MARGIN;
  UINT16  cmdMaxVal = 0;

  RcDebugPrint (SDBG_NONE,
                 "RestoreOffset\n");

  switch (group) {
  case RxVref:
  case RxSampler:
  case RxSamplerEvenOdd:
  case TxVref:
  case RxDqsDelay:
  case RxDqsPDelay:
  case RxDqsNDelay:
  case TxDqDelay:
  case RecEnDelay:
  case WrLvlDelay:
  case DramRxEq:
    GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, ALL_BITS, level, group, GSM_WRITE_OFFSET | GSM_FORCE_WRITE,
      &zeroOffset);
    break;
  case CmdGrp0:
  case CmdGrp1:
  case CmdGrp2:
  case CmdGrp3:
  case CmdGrp4:
  case CmdGrp5:
  case CmdAll:
  case Cmd0All:
  case Cmd1All:
    GetSetCmdGroupDelayCore (Host, socket, ch, dimm, level, group, GSM_READ_ONLY, &zeroOffset, &cmdMinVal, &cmdMaxVal); // Get Current value
    zeroOffset = restoreOffset - zeroOffset;
    GetSetCmdGroupDelayCore (Host, socket, ch, dimm, level, group, GSM_SKIP_CMD_FUB | GSM_WRITE_OFFSET | GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &zeroOffset, &cmdMinVal, &cmdMaxVal);
    break;
  case CmdVref:
    GetSetCmdVrefCore (Host, socket, ch, dimm, level, GSM_READ_ONLY, &zeroOffset);
    zeroOffset = restoreOffset - zeroOffset;
    GetSetCmdVrefCore (Host, socket, ch, dimm, level, GSM_SKIP_CMD_FUB | GSM_WRITE_OFFSET | GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &zeroOffset);
    break;
  case CtlGrp0:
  case CtlGrp1:
  case CtlGrp2:
  case CtlGrp3:
  case CtlGrp4:
  case CtlGrp5:
  case CtlAll:
  case Ctl0Grp:
  case Ctl1Grp:
    GetSetCtlGroupDelayCore (Host, socket, ch, dimm, level, group, GSM_READ_ONLY, &zeroOffset, &cmdMinVal, &cmdMaxVal); // Get Current value
    zeroOffset = restoreOffset - zeroOffset;
    GetSetCtlGroupDelayCore (Host, socket, ch, dimm, level, group, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &zeroOffset, &cmdMinVal, &cmdMaxVal);
    break;
  default:
    break;
  }

  if (Host->var.mem.socket[socket].hostRefreshStatus == 0) {
    IO_Reset (Host, socket);
  }
} // RestoreOffset

/**
  Evaluates the results of the previous test

  @param Host          - Pointer to sysHost
  @param socket        - Processor socket to check
  @param dimm          - Dimm number (0-based)
  @param rank          - Rank number (0-based)
  @param sign          - The direction we are currently going (up, down, left or right)
  @param offset        - Pointer to offsets
  @param chStatus      - Channel Status
  @param scope         - Margin granularity
  @param group         - Parameter to be margined
  @param level         - DDR or LRDIMM Host side, LRDIMM Backside, etc...
  @param mode          - Test mode to use
  @param searchUp      - Direction to search
  @param bwSerr        - Bit wise error status
  @param foundPass     - Bit mask marking passes
  @param foundFail     - Bit mask marking fails
  @param marginData    - pointer to Margin data
  @param ranksDone     - Array of bits showing if ranks are done. If bitX = 1, the rank is done.
  @param maxSteps      - Maximum Steps

  @retval N/A

**/
VOID
EvaluateResults (
  PSYSHOST        Host,
  UINT8           socket,
  UINT8           dimm,
  UINT8           rank,
  UINT8           sign,
  INT16           offset[MAX_CH][MAX_RANK_CH],
  UINT32          chStatus,
  UINT8           scope,
  MRC_GT          group,
  MRC_LT          level,
  UINT16          mode,
  UINT8           searchUp[MAX_CH][MAX_RANK_CH],
  UINT32          bwSerr[MAX_CH][3],
  struct bitMask  *foundPass,
  struct bitMask  *foundFail,
  VOID            *marginData,
  UINT32          *ranksDone,
  UINT8           maxSteps
  )
{
  UINT8               ch;
  UINT8               d = 0;
  UINT8               maxDimm = MAX_DIMM;
  UINT8               startDimm = 0;
  UINT8               r = 0;
  UINT8               maxRank = MAX_RANK_DIMM;
  UINT8               startRank = 0;
  UINT8               strobe;
  UINT8               strobeIndex;
  UINT8               bit;
  UINT8               bitIndex;
  UINT8               bitMask;
  UINT32              status = SUCCESS;
  UINT32              errResult = 0;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8 MBV = GetMaxBitsValid (Host);
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);

  if (scope == SCOPE_SOCKET) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      // No need to check this channel if it is done
      if ((chStatus & (1 << ch)) == 0) {
        continue;
      }

      status |= bwSerr[ch][0] + bwSerr[ch][1];

      if (Host->nvram.mem.eccEn) {
        status |= bwSerr[ch][2];
      }
    } // ch loop

    EvaluateStrobeStatus (Host, socket, SET_ALL, SET_ALL, SET_ALL, SET_ALL, SET_ALL, status, offset[0][0], sign,
      searchUp[0][0], foundPass->bits, foundFail->bits, marginData, scope, mode);
  } else if (scope == SCOPE_CH) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      // No need to check this channel if it is done
      if ((chStatus & (1 << ch)) == 0) {
        continue;
      }

      status = bwSerr[ch][0] | bwSerr[ch][1];

      if (Host->nvram.mem.eccEn) {
        status = status | bwSerr[ch][2];
      }

      EvaluateStrobeStatus (Host, socket, ch, SET_ALL, SET_ALL, SET_ALL, SET_ALL, status, offset[ch][0], sign,
        searchUp[ch][0], foundPass->bits, foundFail->bits, marginData, scope, mode);
    } // ch loop
  } else if (scope == SCOPE_DIMM) {
  } else if (scope == SCOPE_RANK) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      // No need to check this channel if it is done
      if ((chStatus & (1 << ch)) == 0) {
        continue;
      }

      rankList = GetRankNvList (Host, socket, ch, dimm);

#ifdef DEBUG_CODE_BLOCK
      if (GetDebugLevel () & SDBG_MAX) {
        DisplayBwSerr (Host, socket, ch, dimm, rank, bwSerr[ch], offset[ch][ (*rankList)[rank].rankIndex]);
      }
#endif  // DEBUG_CODE_BLOCK
      status = bwSerr[ch][0] | bwSerr[ch][1];

      if (Host->nvram.mem.eccEn) {
        status = status | bwSerr[ch][2];
      }

      EvaluateStrobeStatus (Host, socket, ch, dimm, rank, SET_ALL, SET_ALL, status,
        offset[ch][ (*rankList)[rank].rankIndex], sign, searchUp[ch][ (*rankList)[rank].rankIndex],
        foundPass->bits, foundFail->bits, marginData, scope, mode);
    } // ch loop
  } else if (scope == SCOPE_STROBE) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      // No need to check this channel if it is done
      if ((chStatus & (1 << ch)) == 0) {
        continue;
      }

      rankList = GetRankNvList (Host, socket, ch, dimm);

#ifdef DEBUG_CODE_BLOCK
      if (GetDebugLevel () & SDBG_MAX) {
        DisplayBwSerr (Host, socket, ch, dimm, (*rankList)[rank].rankIndex, bwSerr[ch], offset[ch][ (*rankList)[rank].rankIndex]);
      }
#endif  // DEBUG_CODE_BLOCK
      for (strobe = 0; strobe < MSVx8; strobe++) {
        if (IsStrobeNotValid (Host, strobe)) {
          continue;
        }

        switch (strobe) {
        case 0:
          errResult = bwSerr[ch][0];
          break;

        case 4:
          errResult = bwSerr[ch][1];
          break;

        case 8:
          errResult = bwSerr[ch][2];
          break;
          default:
          //Automated add of default case. Please review.
          break;
        }

        //
        // Find the index into the results data
        //
        strobeIndex = FindIndexResData (strobe);

        if (GetMaxStrobe (Host, socket, ch, dimm, group, level) == MSVx4) {

          //
          // x4 Lower nibble
          //
          status = (errResult >> (strobeIndex * 8)) & 0xF;
          EvaluateStrobeStatus (Host, socket, ch, dimm, rank, strobe, 0xF, status, offset[ch][ (*rankList)[rank].rankIndex],
            sign, searchUp[ch][ (*rankList)[rank].rankIndex], foundPass->bits, foundFail->bits, marginData,
            scope, mode);

          //
          // x4 Upper nibble
          //
          status = (errResult >> (strobeIndex * 8)) & 0xF0;
          EvaluateStrobeStatus (Host, socket, ch, dimm, rank, strobe + MSVx8, 0xF0, status,
            offset[ch][ (*rankList)[rank].rankIndex], sign, searchUp[ch][ (*rankList)[rank].rankIndex],
            foundPass->bits, foundFail->bits, marginData, scope, mode);
        } else {
          // x8 Byte
          status = (errResult >> (strobeIndex * 8)) & 0xFF;
          EvaluateStrobeStatus (Host, socket, ch, dimm, rank, strobe, 0xFF, status, offset[ch][ (*rankList)[rank].rankIndex], sign, searchUp[ch][ (*rankList)[rank].rankIndex],
            foundPass->bits, foundFail->bits, marginData, scope, mode);
        }
      } // strobe loop
    } // ch loop
  } // scope == strobe
  else if (scope == SCOPE_BIT) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      // No need to check this channel if it is done
      if ((chStatus & (1 << ch)) == 0) {
        continue;
      }

      rankList = GetRankNvList (Host, socket, ch, dimm);

#ifdef DEBUG_CODE_BLOCK
      if (GetDebugLevel () & SDBG_DEFAULT) {
        DisplayBwSerr (Host, socket, ch, dimm, (*rankList)[rank].rankIndex, bwSerr[ch], offset[ch][ (*rankList)[rank].rankIndex]);
      }
#endif  // DEBUG_CODE_BLOCK
      for (bit = 0; bit < MBV; bit++) {
        if (IsBitNotValid (Host, bit)) {
          continue;
        }
        switch (bit) {
        case 0:
          errResult = bwSerr[ch][0];
          break;

        case 32:
          errResult = bwSerr[ch][1];
          break;

        case 64:
          errResult = bwSerr[ch][2];
          break;
          default:
          //Automated add of default case. Please review.
          break;
        } // switch

        //
        // Get strobe group
        //
        strobe = bit / 8;

        //
        // Find the index into the results data
        //
        strobeIndex = FindIndexResData (strobe);

        //
        // Index into error results
        //
        bitIndex = bit % 8;

        //
        // Mask for error results
        //
        bitMask = 1 << bitIndex;

        //
        // Get the status for the current bit
        //
        status = (errResult >> (strobeIndex * 8)) & bitMask;

        EvaluateStrobeStatus (Host, socket, ch, dimm, rank, strobe, bitMask, status, offset[ch][ (*rankList)[rank].rankIndex], sign, searchUp[ch][ (*rankList)[rank].rankIndex],
          foundPass->bits, foundFail->bits, marginData, scope, mode);
      } // bit loop
    } // ch loop
  } // scope == bit


  if ((scope == SCOPE_SOCKET) || (scope == SCOPE_CH)) {
    startDimm = 0;
    maxDimm = MAX_DIMM;
    startRank = 0;
    maxRank = MAX_RANK_DIMM;
  }

  if (scope == SCOPE_DIMM) {
    startDimm = dimm;
    maxDimm = dimm + 1;
    startRank = 0;
    maxRank = MAX_RANK_DIMM;
  }

  if ((scope == SCOPE_RANK) || (scope == SCOPE_STROBE) || (scope == SCOPE_BIT)) {
    startDimm = dimm;
    maxDimm = dimm + 1;
    startRank = rank;
    maxRank = rank + 1;
  }

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    // No need to check this channel if it is done
    if ((chStatus & (1 << ch)) == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, socket, ch);

    for (d = startDimm; d < maxDimm; d++) {

      if ((*dimmNvList)[d].dimmPresent == 0) {
        continue;
      }

      rankList = GetRankNvList (Host, socket, ch, d);

      // Loop for each rank
      for (r = startRank; r < maxRank; r++) {

        // Continue to the next rank if this one is disabled
        if (CheckRank (Host, socket, ch, d, r, DONT_CHECK_MAPOUT)) {
          continue;
        }

        if (CheckAllPassFail (Host, foundPass->bits[ch][ (*rankList)[r].rankIndex])) {
          searchUp[ch][ (*rankList)[r].rankIndex] = 0;
        } else {
          searchUp[ch][ (*rankList)[r].rankIndex] = 1;
        }
      } // r loop
    } // d loop
  } // ch loop

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    // No need to check this channel if it is done
    if ((chStatus & (1 << ch)) == 0) {
      continue;
    }

    rankList = GetRankNvList (Host, socket, ch, dimm);

    // Continue to the next rank if this one is disabled
    if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
      continue;
    }

    // Increment offset if searching away from 0
    if (searchUp[ch][ (*rankList)[rank].rankIndex]) {
      if (sign) {
        offset[ch][ (*rankList)[rank].rankIndex] = offset[ch][ (*rankList)[rank].rankIndex] - 1;
      } else {
        offset[ch][ (*rankList)[rank].rankIndex] = offset[ch][ (*rankList)[rank].rankIndex] + 1;
      }
    }
    // Decrement offset is searching toward 0
    else {
      if (sign) {
        offset[ch][ (*rankList)[rank].rankIndex] = offset[ch][ (*rankList)[rank].rankIndex] + 1;
      } else {
        offset[ch][ (*rankList)[rank].rankIndex] = offset[ch][ (*rankList)[rank].rankIndex] - 1;
      }
    }

    // Set global fail if we have exceeded the maximum number of steps
    if ((offset[ch][ (*rankList)[rank].rankIndex] >= maxSteps) || (0 - offset[ch][ (*rankList)[rank].rankIndex] >= maxSteps)) {
      EvalResSetRanksDone (Host, socket, ch, dimm, rank, scope, ranksDone);
    }

    // Set global fail if we have searched all the way to 0
    //if (offset[ch][(*rankList)[rank].rankIndex] == 0) {
    //  *ranksDone |= (1 << ((*rankList)[rank].rankIndex + (ch * 8)));
    //}
  } // ch loop
} // EvaluateResults

VOID
EvaluateStrobeStatus (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bitMask,
  UINT32    status,
  INT16     offset,
  UINT8     sign,
  UINT8     searchUp,
  UINT8     foundPass[MAX_CH][MAX_RANK_CH][MAX_STROBE/2],
  UINT8     foundFail[MAX_CH][MAX_RANK_CH][MAX_STROBE/2],
  VOID      *marginData,
  UINT8     scope,
  UINT16    mode
  )
/**
  Evaluate test results for one strobe group

  @param Host          - Pointer to sysHost
  @param socket        - Processor socket to check
  @param ch            - Current Channel
  @param dimm          - Dimm number (0-based)
  @param rank          - Rank number (0-based)
  @param strobe        - Current Strobe
  @param bitMask       - Bit Mask of currently found passes
  @param status        - Status
  @param offset        - Pointer to offsets
  @param sign          - The direction we are currently going (up, down, left or right)
  @param searchUp      - Direction to search
  @param foundPass     - Bit mask marking passes
  @param foundFail     - Bit mask marking fails
  @param marginData    - pointer to Margin data
  @param scope         - Margin granularity
  @param mode          - Test mode to use

  @retval N/A

**/
{
  UINT8                 maxCh;
  UINT8                 maxDimm;
  UINT8                 maxRank;
  UINT8                 setAllStrobe;
  UINT8                 strobeByte = 0;
  UINT8                 i;
  UINT8                 bit;
  UINT8                 tmpBitMask;
  struct  ddrRank       (*rankList)[MAX_RANK_DIMM];
  struct  baseMargin    *socketM;
  struct  chMargin      *channelM;
  struct  rankMargin    *rankM;
  struct  strobeMargin  *strobeM;
  struct  bitMargin     *bitM;
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;

  if (MSVx8 > (MAX_STROBE/2) ) {
    RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT, "\nInvalid max strobe number, strobe=%d\n", MSVx8);
    ASSERT (FALSE);
    return;
  }

  if (ch == SET_ALL) {
    ch = 0;
    maxCh = MAX_CH;
  } else {
    maxCh = ch + 1;
  }
  if (maxCh > MAX_CH) {
    RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT, "\nInvalid channel number, ch=%d\n", ch);
    ASSERT (FALSE);
    return;
  }
  if (dimm == SET_ALL) {
    dimm = 0;
    maxDimm = MAX_DIMM;
  } else {
    maxDimm = dimm + 1;
  }
  if (maxDimm > MAX_DIMM) {
    RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT, "\nInvalid dimm number, dimm=%d\n", dimm);
    ASSERT (FALSE);
    return;
  }
  if (rank == SET_ALL) {
    rank = 0;
    maxRank = MAX_RANK_DIMM;
  } else {
    maxRank = rank + 1;
  }
  if (maxRank > MAX_RANK_DIMM) {
    RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT, "\nInvalid rank number, rank=%d\n", rank);
    ASSERT (FALSE);
    return;
  }
  if (strobe == SET_ALL) {
    strobe = 0;
    setAllStrobe = SET_ALL;
  } else if ((strobe > (MAX_BITS - 8) / 8) && (strobe < MSVx8)) {
    return;
  } else {
    if (strobe >= MAX_STROBE) {
      RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT, "\nInvalid strobe number, strobe=%d\n", strobe);
      ASSERT (FALSE);
      return;
    }
    strobeByte = strobe;
    if (strobe >= MSVx8) {
      if ((strobe - MSVx8) >= (MAX_STROBE / 2)) {
        RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT, "\nInvalid strobe index =%d\n", (strobe - MSVx8));
        ASSERT (FALSE);
        return;
      }
      strobe = strobe - MSVx8;
    }
    setAllStrobe = strobe;
  }

  // If there was a failure
  if (status) {
    for (; ch < maxCh; ch++) {
      for (; dimm < maxDimm; dimm++) {
        rankList = GetRankNvList (Host, socket, ch, dimm);
        for (; rank < maxRank; rank++) {
          // If a pass has previously been found
          if (foundPass[ch][ (*rankList)[rank].rankIndex][strobe] & bitMask) {
            // If we are searching up
            if (searchUp) {
              // Set all foundFail bits
              SetAllFound (Host, ch, (*rankList)[rank].rankIndex, setAllStrobe, bitMask, foundFail);
            }
          } // If a previous pass has been found
        } // rank loop
      } // dimm loop
    } // ch loop
  } else {
    if (scope == SCOPE_SOCKET) {
      socketM = marginData;

      // Save new offset at the failing point
      if (sign) {
        (*socketM).n = offset - 1;
      } else (*socketM).p = offset + 1;
    } else if (scope == SCOPE_CH) {
      channelM = marginData;

      // Save new offset at the failing point
      if (sign) {
        (*channelM).channel[ch].n = offset - 1;
      } else (*channelM).channel[ch].p = offset + 1;
    } else if (scope == SCOPE_DIMM) {
    } else if (scope == SCOPE_RANK) {
      rankM = marginData;
      rankList = GetRankNvList (Host, socket, ch, dimm);

      // Save new offset at the failing point
      if (sign) {
        (*rankM).rank[ch][ (*rankList)[rank].rankIndex].n = offset - 1;
      } else (*rankM).rank[ch][ (*rankList)[rank].rankIndex].p = offset + 1;
    } else if (scope == SCOPE_STROBE) {
      strobeM = marginData;
      rankList = GetRankNvList (Host, socket, ch, dimm);

      if ((foundFail[ch][ (*rankList)[rank].rankIndex][strobe] & bitMask) == 0) {
        if (strobeByte >= MAX_STROBE) {
          RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT, "\nInvalid strobeByte number, strobeByte=%d\n", strobeByte);
          ASSERT (FALSE);
          return;
        }
        // Save new offset at the failing point
        if (sign) {
          (*strobeM).strobe[ch][ (*rankList)[rank].rankIndex][strobeByte].n = offset - 1;
        } else {
          (*strobeM).strobe[ch][ (*rankList)[rank].rankIndex][strobeByte].p = offset + 1;
        }

      }
    } else if (scope == SCOPE_BIT) {
      bitM = marginData;
      rankList = GetRankNvList (Host, socket, ch, dimm);

      i = 0;
      tmpBitMask = bitMask;
      while (tmpBitMask != 1) {
        tmpBitMask >>= 1;
        i++;
      }

     if (strobe >= MSVx8) {
        bit = (strobe - MSVx8) * 8 + i;
      } else {
        bit = (strobe * 8) + i;
      }

      if ((foundFail[ch][ (*rankList)[rank].rankIndex][strobe] & (1 << i)) == 0) {
        // Save new offset at the failing point
        if (sign) {
          (*bitM).bits[ch][ (*rankList)[rank].rankIndex][bit].n = offset - 1;
        } else       {
          (*bitM).bits[ch][ (*rankList)[rank].rankIndex][bit].p = offset + 1;
        }
      }
    }

    for (; ch < maxCh; ch++) {
      for (; dimm < maxDimm; dimm++) {
        rankList = GetRankNvList (Host, socket, ch, dimm);
        for (; rank < maxRank; rank++) {
          // Set all foundPass bits
          SetAllFound (Host, ch, (*rankList)[rank].rankIndex, setAllStrobe, bitMask, foundPass);
        } // rank loop
      } // dimm loop
    } // ch loop
  }
} // EvaluateStrobeStatus

/**

  Returns the minimum margin sugested by spec or EV
  recomendation
  @param Host            - Pointer to sysHost
  @param TrainingSignal  - Training signal to be evaluated

  @retval UINT8

**/
UINT8
MinimumMargin (
  PSYSHOST                          Host,
  UINT8                             TrainingSignal
  )
{
  UINT8                             i;
  UINT8                             socket;

  socket = Host->var.mem.currentSocket;
  //
  // Search the minimum margin for a specific training signal
  //
  for (i = 0; i < MAX_MARGIN_LIST; i++) {
    if (MinimumMarginData[i][0] == TrainingSignal) {
      return MinimumMarginData[i][1];
    }
  }
  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "Warning MinimumMargin call Margin Index not found %d\n",
                          TrainingSignal
                          );
  return 0xFF;
}


UINT8
LowMarginMsg (
  PSYSHOST                          Host,
  UINT8                             skt,
  UINT8                             ch,
  UINT8                             dm,
  UINT8                             rk,
  UINT8                             st,
  UINT8                             bt,
  INT16                             Width,
  UINT8                             MinMargin
  )
{
  UINT8    Status;
  Status = SUCCESS;
  if (Width < MinMargin) {
    //
    // Warning about low margin
    //
    RcDebugPrintWithDevice (SDBG_MAX, skt, ch, dm, NO_SUBCH, rk, st, bt,
                            "Warning! eye width is too small, current margin: %d minimum: %d\n",
                            Width, MinMargin);
    Status = FAILURE;
  } // if Width
  return Status;
}

/**

  Detect Low Margins per training step signal

  @param Host            - Pointer to sysHost
  @param Channel         - Channel
  @param Dimm            - Dimm
  @param Rank            - Rank
  @param *Data           - Margin Data to review
  @param TrainingSignal  - Training signal to review low margin
                         on
  @param Flags           - Differentiate same margin signal different training

  @retval UINT8

**/
UINT8
DetectLowMargin (
  PSYSHOST                          Host,
  UINT8                             Channel,
  UINT8                             Dimm,
  UINT8                             Rank,
  VOID                              *Data,
  UINT8                             TrainingSignal,
  UINT8                             Flags
  )
{
  struct channelNvram               (*channelNvList)[MAX_CH];
  struct dimmNvram                  (*dimmNvList)[MAX_DIMM];
  struct ddrRank                    (*rankList)[MAX_RANK_DIMM];
  UINT8                             skt;
  UINT8                             ch;
  UINT8                             rk;
  UINT8                             st;
  UINT8                             bit;
  UINT8                             dimm;
  UINT8                             MinMargin;
  INT16                             Width;
  UINT8                             group;
  INT16                             vrefHi = VREF_HI;
  INT16                             vrefLo = VREF_LO;
  INT16                             tmpEdge;
  UINT8                             maxStrobe;
  UINT8                             numPoints;
  UINT8                             vIndexCenter;
  UINT8                             Status;
  UINT16                            (*pulseWidth)[MAX_CH][MAX_STROBE];
  struct bitMarginCh                *bitMarginChWidth;
  struct bitMargin                  *bitMarginRkWidth;
  struct bitMargin                  (*vrefMarginBit)[MAX_VREF_TIMING_NUMBER];
  struct strobeMargin               (*vrefMarginStrobe)[MAX_VREF_TIMING_NUMBER];
  struct strobeMargin               (*results)[MAX_VREF_POINTS];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MBV = GetMaxBitsValid (Host);
  UINT8                             MaxChDdr;
  UINT8                             StrobeIndex;

  //
  // Cast the VOID *
  //
  bitMarginRkWidth = Data;
  bitMarginChWidth = Data;
  pulseWidth = Data;
  vrefMarginBit = Data;
  vrefMarginStrobe = Data;
  results = Data;
  Status = SUCCESS;
  MaxChDdr = GetMaxChDdr ();
  skt = Host->var.mem.currentSocket;
  channelNvList = GetChannelNvList (Host, skt);

  //
  // MinMargin is unique for all SK/CH/RK/ST/BIT per signal
  //
  MinMargin = MinimumMargin (Host, TrainingSignal);

  switch (TrainingSignal) {
  case TxDqsDelay:
  case RecEnDelay:

    if (Dimm >= MAX_DIMM) {
      RcDebugPrintWithDevice (SDBG_MINMAX, skt, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, "\nInvalid Dimm number, Dimm=%d\n", Dimm);
      ASSERT (FALSE);
      return FAILURE;
    }

    for (ch = 0; ch < MaxChDdr; ch += GetChInc (Host)) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      dimmNvList = GetDimmNvList (Host, skt, ch);
      if ((Flags & LM_LRDIMM) && !((*dimmNvList)[Dimm].lrDimmPresent)) {
        continue;
      }
      //
      // Verify if any margin or pulse width is below recommended value
      //
      for (st = 0; st < MSVx4; st++) {
        //
        // Skip if this is an ECC strobe when ECC is disabled
        //
        if (IsDimmStrobeNotValid (&(*dimmNvList)[Dimm], st)) {
          continue;
        }
        Status = LowMarginMsg (Host, skt, ch, NO_DIMM, NO_RANK, st, NO_BIT, (*pulseWidth)[ch][st], MinMargin);
      } // for st
    } // for ch
    break;
  case RxDqsDelay:
  case TxDqDelay:
  case RxDqsNDelay:
  case RxDqsPDelay:
    //
    // Different process if this is for basic vs advanced trainining
    //
    if (!(Flags & LM_ADVANCED)) {
      if (!(Flags & LM_LRDIMM)) {

        if (Channel >= MAX_CH) {
          RcDebugPrintWithDevice (SDBG_MINMAX, skt, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, "\nInvalid Channel number, Channel=%d\n", Channel);
          ASSERT (FALSE);
          return FAILURE;
        }
        //
        // The validation if this is enabled channel/dimm/rank already happen for this signal
        //
        for (bit = 0; bit < MBV; bit++) {
          //
          //Detect if ECC is enabled and skip approapiate bits
          //
#ifdef DDR5_SUPPORT
          if (IsDdr5Present (Host, skt)) {
            if (IsBitNotValidSubChDdr5 (Host, bit)) {
              continue;
            }
          } else {
#endif //if DDR5_SUPPORT
            if (IsBitNotValid (Host, bit)) {
              continue;
            }
#ifdef DDR5_SUPPORT
          }
#endif //if DDR5_SUPPORT

          Width = bitMarginChWidth->bits[Channel][bit].p - bitMarginChWidth->bits[Channel][bit].n;
          Status = LowMarginMsg (Host, skt, Channel, Dimm, Rank, NO_STROBE, bit, Width, MinMargin);
        } // for bit
      } else {

        if (Dimm >= MAX_DIMM) {
          RcDebugPrintWithDevice (SDBG_MINMAX, skt, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, "\nInvalid Dimm number, Dimm=%d\n", Dimm);
          ASSERT (FALSE);
          return FAILURE;
        }
        //
        // LRDIMM Rd/Wr MDQS Delay
        //
        for (ch = 0; ch < MaxChDdr; ch += GetChInc (Host)) {
          if ((*channelNvList)[ch].enabled == 0) {
            continue;
          }
          dimmNvList = GetDimmNvList (Host, skt, ch);
          //
          // Verify this DIMM is LRDIMM since there can be assymetric population for type
          //
          if (!((*dimmNvList)[Dimm].lrDimmPresent)) {
            continue;
          }
          //
          // Verify if any margin or pulse width is below recommended value
          //
          for (st = 0; st < MSVx4; st++) {
            //
            // Skip if this is an ECC strobe when ECC is disabled
            //
            if (IsDimmStrobeNotValid (&(*dimmNvList)[Dimm], st)) {
              continue;
            }
            Status = LowMarginMsg (Host, skt, ch, NO_DIMM, NO_RANK, st, NO_BIT, (*pulseWidth)[ch][st], MinMargin);
          } // for st
        } // for ch
      }
    } else {
      //
      // The result for the 2D sweep for different Vref point offsets test only the center point
      //
      if (TrainingSignal == TxDqDelay) {
        numPoints = TX_MULTI_VREF_POINTS;
      } else {
        numPoints = RX_MULTI_VREF_POINTS;
      }
      vIndexCenter = numPoints / 2;
      for (ch = 0; ch < MaxChDdr; ch += GetChInc (Host)) {
        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }
        dimmNvList = GetDimmNvList (Host, skt, ch);

        for (dimm = 0; dimm < MAX_DIMM; dimm++) {
          //
          // Skip if no DIMM present
          //
          if ((*dimmNvList)[dimm].dimmPresent == 0) {
            continue;
          }

          rankList = GetRankNvList (Host, skt, ch, dimm);

          for (rk = 0; rk < (*dimmNvList)[dimm].numRanks; rk++) {
            //
            // Skip if no rank
            //
            if (CheckRank (Host, skt, ch, dimm, rk, DONT_CHECK_MAPOUT)) {
              continue;
            }

            maxStrobe = GetMaxStrobe (Host, skt, ch, dimm, TrainingSignal, DdrLevel);

            for (st = 0; st < maxStrobe; st++) {
              //
              // Skip if this is an ECC strobe when ECC is disabled
              //
              if (IsStrobeNotValid (Host, st)) {
                continue;
              }

              Width = ((*results)[vIndexCenter].strobe[ch][ (*rankList)[rk].rankIndex][st].p) +
                (- ((*results)[vIndexCenter].strobe[ch][ (*rankList)[rk].rankIndex][st].n));
              Status = LowMarginMsg (Host, skt, ch, dimm, rk, st, NO_BIT, Width, MinMargin);
            } // for strobe
          } // for rank
        } // for dimm
      } // for ch
    } // Adv
    break;
  case RxDqsBitDelay:
  case RxDqsPBitDelay:
  case RxDqsNBitDelay:
  case TxDqBitDelay:

    switch (TrainingSignal) {
    case RxDqsBitDelay:
      group = RxDqsPDelay;
      break;
    case RxDqsPBitDelay:
      group = RxDqsPDelay;
      break;
    case RxDqsNBitDelay:
      group = RxDqsNDelay;
      break;
    case TxDqBitDelay:
      group = TxDqDelay;
      break;
    default:
      group = TxDqDelay;
      break;
    }

    for (ch = 0; ch < MaxChDdr; ch += GetChInc (Host)) {
      dimmNvList = GetDimmNvList (Host, skt, ch);
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }
        rankList = GetRankNvList (Host, skt, ch, dimm);

        for (rk = 0; rk < (*dimmNvList)[dimm].numRanks; rk++) {
          if (CheckRank (Host, skt, ch, dimm, rk, DONT_CHECK_MAPOUT)) {
            continue;
          }
          if (CheckSkipRankTrain (Host, skt, ch, dimm, rk, DdrLevel, group)) {
            continue;
          }
          for (bit = 0; bit < MBV; bit++) {
            //
            //Detect if ECC is enabled and skip approapiate bits
            //
            if (IsBitNotValid (Host, bit)) {
              continue;
            }
            Width = bitMarginRkWidth->bits[ch][ (*rankList)[rk].rankIndex][bit].p + (- (bitMarginRkWidth->bits[ch][ (*rankList)[rk].rankIndex][bit].n));
            Status = LowMarginMsg (Host, skt, ch, dimm, rk, NO_STROBE, bit, Width, MinMargin);
          } // for bit
        } // for rank
      } // for dimm
    } // for ch
    break;
  case RxVref:
  case RxSampler:
  case RxSamplerEvenOdd:
  case TxVref:

    if ((Flags & PER_BIT)) {
      for (bit = 0; bit < MBV; bit++) {
        //
        // Skip if this is an ECC strobe when ECC is disabled
        //
        if (IsBitNotValid (Host, bit)) {
          continue;
        }
        for (ch = 0; ch < MaxChDdr; ch += GetChInc (Host)) {
          //
          // Skip if channel is disabled
          //
          if ((*channelNvList)[ch].enabled == 0) {
            continue;
          }
          vrefHi = 0x5F;
          vrefLo = -0x5F;
          dimmNvList = GetDimmNvList (Host, skt, ch);

          for (dimm = 0; dimm < MAX_DIMM; dimm++) {
            //
            // Skip if no DIMM present
            //
            if ((*dimmNvList)[dimm].dimmPresent == 0) {
              continue;
            }

            rankList = GetRankNvList (Host, skt, ch, dimm);

            for (rk = 0; rk < (*dimmNvList)[dimm].numRanks; rk++) {
              //
              // Skip if no rank
              //
              if (CheckRank (Host, skt, ch, dimm, rk, DONT_CHECK_MAPOUT)) {
                continue;
              }
              //
              // Find the composite eye for this strobe across ranks
              //
              tmpEdge = ((*vrefMarginBit)[0].bits[ch][ (*rankList)[rk].rankIndex][bit].p +
                         (*vrefMarginBit)[1].bits[ch][ (*rankList)[rk].rankIndex][bit].p +
                         (*vrefMarginBit)[2].bits[ch][ (*rankList)[rk].rankIndex][bit].p) / 3;
              if (vrefHi > tmpEdge) {
                vrefHi = tmpEdge;
              }

              tmpEdge = ((*vrefMarginBit)[0].bits[ch][ (*rankList)[rk].rankIndex][bit].n +
                         (*vrefMarginBit)[1].bits[ch][ (*rankList)[rk].rankIndex][bit].n +
                         (*vrefMarginBit)[2].bits[ch][ (*rankList)[rk].rankIndex][bit].n) / 3;
              if (vrefLo < tmpEdge) {
                vrefLo = tmpEdge;
              }
            } // for rank
          } // for dimm
          Width = vrefHi + (-vrefLo);
          Status = LowMarginMsg (Host, skt, ch, NO_DIMM, NO_RANK, NO_STROBE, bit, Width, MinMargin);
        }  // for ch
      }  // for bit

    } else {

      for (ch = 0; ch < MaxChDdr; ch += GetChInc (Host)) {
        //
        // Skip if channel is disabled
        //
        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }

        dimmNvList = GetDimmNvList (Host, skt, ch);

        for (dimm = 0; dimm < MAX_DIMM; dimm++) {
          //
          // Skip if no DIMM present
          //
          if ((*dimmNvList)[dimm].dimmPresent == 0) {
            continue;
          }

          maxStrobe = GetMaxStrobe (Host, skt, ch, dimm, TrainingSignal, DdrLevel);

          for (StrobeIndex = 0; StrobeIndex < maxStrobe; StrobeIndex++) {
            //
            // Skip if this is an ECC strobe when ECC is disabled
            //
            if ((!Host->nvram.mem.eccEn) && IsEccStrobe (Host, StrobeIndex)){
              continue;
            }

            if (IsDdr5Present (Host, skt)  && (IsBrsPresent (Host, skt) == FALSE)) {
              //
              // In DDR5 x8 case, the per strobe margin results are stored in strobes 0-4 and 10-14.
              //
              st = StrobeIndex % (maxStrobe / 2) + (StrobeIndex / (maxStrobe / 2)) * (MSVx4 / 2);
            } else {
              st = StrobeIndex;
            }

            rankList = GetRankNvList (Host, skt, ch, dimm);

            vrefHi = VREF_HI;
            vrefLo = VREF_LO;

            for (rk = 0; rk < (*dimmNvList)[dimm].numRanks; rk++) {
              //
              // Skip if no rank
              //
              if (CheckRank (Host, skt, ch, dimm, rk, DONT_CHECK_MAPOUT)) {
                continue;
              }

              //
              // Find the composite eye for this strobe across ranks
              //
              tmpEdge = ((*vrefMarginStrobe)[0].strobe[ch][(*rankList)[rk].rankIndex][st].p +
                (*vrefMarginStrobe)[1].strobe[ch][(*rankList)[rk].rankIndex][st].p +
                (*vrefMarginStrobe)[2].strobe[ch][(*rankList)[rk].rankIndex][st].p) / 3;
              if (vrefHi > tmpEdge) {
                vrefHi = tmpEdge;
              }

              tmpEdge = ((*vrefMarginStrobe)[0].strobe[ch][(*rankList)[rk].rankIndex][st].n +
                (*vrefMarginStrobe)[1].strobe[ch][(*rankList)[rk].rankIndex][st].n +
                (*vrefMarginStrobe)[2].strobe[ch][(*rankList)[rk].rankIndex][st].n) / 3;
              if (vrefLo < tmpEdge) {
                vrefLo = tmpEdge;
              }

            } // rank loop
            Width = vrefHi + (-vrefLo);
            Status = LowMarginMsg (Host, skt, ch, dimm, NO_RANK, st, NO_BIT, Width, MinMargin);
          } // StrobeIndex loop
        } // dimm loop
      } // ch loop
    }

    break;
    default:
    //
    // Invalid signal
    //
    break;
  }
  return Status;
} // DetectLowMargin

/** Return whether a Get/Set Mode indicates to read the CSR.

  @param[in]  Mode  The Get/Set Mode to evaluate.

  @retval FALSE The mode indicates to read the cache.
  @retval TRUE  The mode indicates to read the CSR.

**/
BOOLEAN
EFIAPI
GsmReadCsr (
  IN  UINT8 Mode
  )
{
  return ((Mode & GSM_READ_CSR) != 0);
}

/** Return whether a Get/Set Mode indicates read-only.

  @param[in]  Mode  The Get/Set Mode to evaluate.

  @retval FALSE The mode indicates read/write.
  @retval TRUE  The mode indicates read-only.

**/
BOOLEAN
EFIAPI
GsmReadOnly (
  IN  UINT8 Mode
  )
{
  return ((Mode & GSM_READ_ONLY) != 0);
}

/** Return whether a Get/Set Mode indicates to write an offset.

  @param[in]  Mode  The Get/Set Mode to evaluate.

  @retval FALSE The mode indicates the associated value is absolute.
  @retval TRUE  The mode indicates the associated values is an offset.

**/
BOOLEAN
EFIAPI
GsmWriteOffset (
  IN  UINT8 Mode
  )
{
  return ((Mode & GSM_WRITE_OFFSET) != 0);
}

/** Return whether a Get/Set Mode indicates to force a write.

  @param[in]  Mode  The Get/Set Mode to evaluate.

  @retval FALSE The mode indicates to write a CSR only if a value has changed.
  @retval TRUE  The mode indicates to write a CSR regardless of the value.

**/
BOOLEAN
EFIAPI
GsmForceWrite (
  IN  UINT8 Mode
  )
{
  return ((Mode & GSM_FORCE_WRITE) != 0);
}

/** Return whether a Get/Set Mode indicates the cache is to be updated.

  @param[in]  Mode  The Get/Set Mode to evaluate.

  @retval FALSE The mode indicates to leave the cache untouched.
  @retval TRUE  The mode indicates to update the cache.

**/
BOOLEAN
EFIAPI
GsmUpdateCache (
  IN  UINT8 Mode
  )
{
  return ((Mode & GSM_UPDATE_CACHE) != 0);
}

/** Return whether a Get/Set Mode indicates to combine edges.

  @param[in]  Mode  The Get/Set Mode to evaluate.

  @retval FALSE The mode indicates to use a single values as center.
  @retval TRUE  The mode indicates to use two values as opposite edges.

**/
BOOLEAN
EFIAPI
GsmCombineEdges (
  IN  UINT8 Mode
  )
{
  return ((Mode & GSM_COMBINE_EDGES) != 0);
}
