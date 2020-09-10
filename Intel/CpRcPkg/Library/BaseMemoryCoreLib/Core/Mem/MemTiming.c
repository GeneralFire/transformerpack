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
#include <Library/SysHostPointerLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/PcdLib.h>
#include "Include/BaseMemoryCoreInternal.h"
#include <Library/BaseMemoryLib.h>
#include <Library/Ddr5CoreLib.h>
#include <Library/MemTurnaroundTimingLib.h>

                                          // 800   1000   1066   1200   1333   1400   1600   1800   1866   2000  2133  2200  2400  2600  2666  2800  2933  3000  3200  3400  3467  3600  3733  3800  4000  4200  4266  4400  4800  5200  5600  6000  6400  6800  7200  7600  8000  8400
CONST UINT32 preciseTCK[MAX_SUP_FREQ]   = {25000, 20000, 18750, 16667, 15000, 14286, 12500, 11111, 10714, 10000, 9375, 9091, 8333, 7692, 7500, 7143, 6818, 6667, 6250, 5883, 5769, 5556, 5358, 5264, 5000, 4762, 4689, 4546, 4166, 3846, 3571, 3333, 3125, 2941, 2777, 2631, 2500, 2380};
//Based on 1tCK 2nd set CWL (DRAM Spec.)
CONST UINT8  tWL_DDR4[MAX_SUP_FREQ]     = {    9,     9,     9,     9,     9,    11,    11,    11,    12,    12,   14,   14,   16,   16,   18,   18,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20};
CONST UINT8  tCL_DDRT                   = 23;
//Based on 1tCK 1st set CWL (DRAM Spec.)
CONST UINT8  tWL_DDRT[MAX_SUP_FREQ]     = {    9,     9,     9,     9,     9,     9,     9,     9,    10,    10,   11,   11,   12,   12,   14,   14,   16,   16,   16,   16,   16,   16,   16,   16,   16,   16,   16,   16};
CONST UINT8  tMRD_DDR4 = 4;
CONST UINT8  PiTicks = 64;

//
// Local Prototypes
//

UINT16
GetMaxClkDiffDD (
  PSYSHOST Host,
  UINT8 Socket,
  UINT8 Channel,
  UINT8 Mode
  );

/*++

  Restores trained timing values register

  @param[in]  Host    - Pointer to sysHost
  @param[in]  Socket  - Socket number
  @param[in]  Channel - Channel number

  @retval N/A

--*/

VOID
RestoreTimings (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel
  )
{
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               Strobe;
  UINT8               CtlIndex;
  UINT8               Clk;
  INT16               ZeroValue = 0;
  INT16               ZeroValueNibble[BITS_PER_NIBBLE];
  UINT16              CmdMinVal;
  UINT16              CmdMaxVal;
  UINT16              CtlMinVal;
  UINT16              CtlMaxVal;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];

#ifdef LRDIMM_SUPPORT
  struct rankDevice   (*RankStruct)[MAX_RANK_DIMM];
#endif // #ifdef LRDIMM_SUPPORT

  UINT8 MSVx4 = GetMaxStrobeValid (Host);

  ChannelNvList = GetChannelNvList (Host, Socket);
  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "Restore Timings\n");

  SetRestoreTimings (Host, Socket, Channel);

  SetMem ((UINT8 *)ZeroValueNibble, sizeof (ZeroValueNibble), 0);
  //
  // Restore CMD timings
  //
  GetSetCmdGroupDelay (Host, Socket, Channel, CmdAll, GSM_FORCE_WRITE + GSM_WRITE_OFFSET, &ZeroValue, &CmdMinVal, &CmdMaxVal);

  //
  // Restore CTL timings
  //
  for (CtlIndex = 0; CtlIndex < NUM_CTL_PLATFORM_GROUPS; CtlIndex++) {
    GetSetCtlGroupDelay (Host, Socket, Channel, CtlIndex + CtlGrp0, GSM_FORCE_WRITE + GSM_WRITE_OFFSET, &ZeroValue, &CtlMinVal, &CtlMaxVal);
  }

  //
  // Restore CLK timings
  //
  for (Clk = 0; Clk < 4; Clk++) {
    GetSetClkDelay (Host, Socket, Channel, Clk, GSM_FORCE_WRITE + GSM_WRITE_OFFSET, &ZeroValue);
  }

  //
  // Restore Tx Rise/Fall slew rate
  //
  GetSetTxRiseFallSlewRate (Host, Socket, Channel, 0, 0, ALL_STROBES, 0, DdrLevel, 0xFF, GSM_FORCE_WRITE | GSM_WRITE_OFFSET, &ZeroValue);

  //
  // Restore all timings on fast boot.
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }

#ifdef LRDIMM_SUPPORT
    RankStruct = GetRankStruct (Host, Socket, Channel, Dimm);
#endif // #ifdef LRDIMM_SUPPORT

    for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {

      if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
        continue;
      }

      for (Strobe = 0; Strobe < MSVx4; Strobe++) {

        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValid (Host, Strobe)) {
          continue;
        }
        GetSetTxDelay (Host, Socket, Channel, Dimm, Rank, Strobe, 0, DdrLevel, TxDqDelay, GSM_FORCE_WRITE | GSM_WRITE_OFFSET,
          &ZeroValue);
        GetSetRxDelay (Host, Socket, Channel, Dimm, Rank, Strobe, 0, DdrLevel, RxDqsDelay, GSM_FORCE_WRITE | GSM_WRITE_OFFSET,
          &ZeroValue);
        GetSetRxVref (Host, Socket, Channel, Dimm, Rank, Strobe, 0, DdrLevel, RxVref, GSM_FORCE_WRITE | GSM_WRITE_OFFSET,
          &ZeroValue);

        if (DESKEW_DELAY == RxDqsBitDelay) {
          GetSetRxDelayBit (Host, Socket, Channel, Dimm, Rank, Strobe, ALL_BITS, DdrLevel, RxDqsBitDelay, GSM_FORCE_WRITE | GSM_WRITE_OFFSET,
            ZeroValueNibble);
        } else {
          GetSetRxDelayBit (Host, Socket, Channel, Dimm, Rank, Strobe, ALL_BITS, DdrLevel, RxDqsPBitDelay, GSM_FORCE_WRITE | GSM_WRITE_OFFSET,
            ZeroValueNibble);
          GetSetRxDelayBit (Host, Socket, Channel, Dimm, Rank, Strobe, ALL_BITS, DdrLevel, RxDqsNBitDelay, GSM_FORCE_WRITE | GSM_WRITE_OFFSET,
            ZeroValueNibble);
        }

        WriteFastBootTimings (Host, Socket, Channel, Dimm, Rank, Strobe);

#if defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)
        // restore LRDIMM cache
        if (IsLrdimmPresent (Socket, Channel, Dimm)) {

          (*RankStruct)[Rank].cachedLrBuf_FxBC2x3x[Strobe] = (*ChannelNvList)[Channel].dimmList[Dimm].rankList[Rank].lrBuf_FxBC2x3x[Strobe];
          (*RankStruct)[Rank].cachedLrBuf_FxBC4x5x[Strobe] = (*ChannelNvList)[Channel].dimmList[Dimm].rankList[Rank].lrBuf_FxBC4x5x[Strobe];
          (*RankStruct)[Rank].cachedLrBuf_FxBC8x9x[Strobe] = (*ChannelNvList)[Channel].dimmList[Dimm].rankList[Rank].lrBuf_FxBC8x9x[Strobe];
          (*RankStruct)[Rank].cachedLrBuf_FxBCAxBx[Strobe] = (*ChannelNvList)[Channel].dimmList[Dimm].rankList[Rank].lrBuf_FxBCAxBx[Strobe];
          (*RankStruct)[Rank].lrbufTxVrefCache[Strobe] = (*ChannelNvList)[Channel].dimmList[Dimm].rankList[Rank].lrbufTxVref[Strobe];

          if (Strobe < MSVx4/2) {
            (*RankStruct)[Rank].cachedLrBuf_FxBCCxEx[Strobe] = (*ChannelNvList)[Channel].dimmList[Dimm].rankList[Rank].lrBuf_FxBCCxEx[Strobe];
            (*RankStruct)[Rank].cachedLrBuf_FxBCDxFx[Strobe] = (*ChannelNvList)[Channel].dimmList[Dimm].rankList[Rank].lrBuf_FxBCDxFx[Strobe];
            (*RankStruct)[Rank].lrbufRxVrefCache[Strobe] = (*ChannelNvList)[Channel].dimmList[Dimm].rankList[Rank].lrbufRxVref[Strobe];
          }
        }

#endif // defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)

      } // Strobe loop
    } // Rank loop
  } // Dimm loop

  //
  // Other Timing Parameters
  //
  RestoreTimingsChip (Host, Socket, Channel);

} // RestoreTimings

/*++


/**

  CAS2RCVEN: The time from sending CAS command @ IO till RCV_EN is asserted per this command.
  It should be calculated per byte as
  CAS2RCVEN(rank, byte) = 1/2 * (round_trip_latency(rank) + RxRcvEnLogicDelay(rank, byte)
        - IO_latency(rank) + RcvPIsetting(rank, byte) / 64) - CONST
  The CONST is the latency from pads to MC. It doesn't affect the further calculations because these will be the differences
  of CAS2RCVEN of different ranks same byte.
  The gap between two data transfers is determined by two limits - drive period and ODT period. The limiting factor is
  different for different rank and for different DIMM.
  R2RDR_GAP: On different ranks same DIMM the ODT value doesn't change in the turnaround, so the limiters are derived from
  need to keep non-overlap between the two drivers. For this purpose 0.6 DCLK cycle is enough. The final equation is
            tRRDR-D0 = max(Byte = 0 to 7, ECC) |CAS2RCVEN(R0, byte) - CAS2RCVEN(R1, byte)| + 0.25
            tRRDR-D2 = max(Byte = 0 to 7, ECC) |CAS2RCVEN(R2, byte) - CAS2RCVEN(R3, byte)| + 0.25
            tRRDR = round-up (max (tRRDR-D0, tRRDR-D2)) -1

  @param[in]  Host    - Point to sysHost
  @param[in]  Socket  - Socket number
  @param[in]  Channel - Channel number
  @param[in]  Mode    - Timing Mode: SR_MODE, DR_MODE, or DD_MODE

  @retval  gap

**/
UINT8
EFIAPI
Cas2RecEnGap (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Mode,
  IN BOOLEAN   DdrtTurnaroundFlag
  )
{
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               logRank;
  UINT8               Strobe;
  UINT8               RoundTrip;
  UINT8               IoLatency;
  INT16               recEnPi;
  UINT16              cas2Rcven;
  UINT16              maxGap = 0;
  UINT16              strobeMin[MAX_STROBE];
  UINT16              strobeMax[MAX_STROBE];
  UINT16              gap;
  UINT16              gapDimm = 0;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);

  ChannelNvList = GetChannelNvList (Host, Socket);
  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  gap = 0;

  if (Mode == DD_MODE) {
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      strobeMin[Strobe] = 0xFFFF;
      strobeMax[Strobe] = 0;
    } // Strobe loop
  }


  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }
    if (((*DimmNvList)[Dimm].DcpmmPresent == 1) && (DdrtTurnaroundFlag == 0)) {
      continue;
    }

    if (Mode == DR_MODE) {
      gapDimm = 0;

      for (Strobe = 0; Strobe < MSVx4; Strobe++) {
        strobeMin[Strobe] = 0xFFFF;
        strobeMax[Strobe] = 0;
      } // Strobe loop
    }

    if (PcdGetBool (PcdMrcDebugTurnarounds) == TRUE) {

      RcDebugPrint (SDBG_MAX,
                     "\nCAS To Receive Enable Data %d DIMM %d\n", Channel, Dimm);
      if (Mode == DD_MODE) {
        RcDebugPrint (SDBG_MAX,
                      "Different DIMM\n");
      } else {
        RcDebugPrint (SDBG_MAX,
                      "Different Rank\n");
      }
      RcDebugPrint (SDBG_MAX,
                     "           | rank 0 | rank 1 | rank 2 | rank 3\n", Dimm);
      RcDebugPrint (SDBG_MAX,
                     "-----------------------------------------------\n");
    }

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      //
      // Skip if this is an ECC Strobe when ECC is disabled
      //
      if (IsStrobeNotValid (Host, Strobe)) {
        continue;
      }


      if (PcdGetBool (PcdMrcDebugTurnarounds) == TRUE) {

        RcDebugPrint (SDBG_MAX,
          "Strobe %2d:", Strobe);

      }

      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        //
        // Get the logical rank #
        //
        logRank = GetLogicalRank (Host, Socket, Channel, Dimm, Rank);
        if (FeaturePcdGet (PcdCteBuild) == TRUE) {
          GetSetDataGroup (Host, Socket, Channel, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RecEnDelay, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, &recEnPi);
        } else {
          GetSetDataGroup (Host, Socket, Channel, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RecEnDelay, GSM_READ_ONLY, &recEnPi);
        }
        GetRoundTrip (Socket, Channel, logRank, GSM_READ_CSR, &RoundTrip);
        GetIoLatency (Socket, Channel, logRank, GSM_READ_CSR, &IoLatency);
        cas2Rcven = (RoundTrip * MAX_PHASE_IN_FINE_ADJUSTMENT) -
          (IoLatency * MAX_PHASE_IN_FINE_ADJUSTMENT) + recEnPi;

        if (PcdGetBool (PcdMrcDebugTurnarounds) == TRUE) {

          RcDebugPrint (SDBG_MAX,
            " |  %3d ", cas2Rcven);
        }

        UpdateMinMaxUint (cas2Rcven, &strobeMin[Strobe], &strobeMax[Strobe]);
      } // Rank loop

      if (PcdGetBool (PcdMrcDebugTurnarounds) == TRUE) {
        RcDebugPrint (SDBG_MAX, "\n");
      }

    } // Strobe loop

    if (Mode == DR_MODE) {

      maxGap = 0;
      for (Strobe = 0; Strobe < MSVx4; Strobe++) {
        if (maxGap < (strobeMax[Strobe] - strobeMin[Strobe])) {
          maxGap = strobeMax[Strobe] - strobeMin[Strobe];
        }
      } // Strobe loop

      //
      // An extra DCLK is required if the diff is > 39 ticks
      //
      if (maxGap >= 39) {
        gapDimm = maxGap - 39;
      } else {
        gapDimm = 0;
      }

      //
      // Account for separation between commands (convert pi ticks to DCLK)
      //
      if ((gapDimm % (MAX_PHASE_IN_FINE_ADJUSTMENT * 2)) > 0) {
        gapDimm = (gapDimm / (MAX_PHASE_IN_FINE_ADJUSTMENT * 2)) + 1;
      } else {
        gapDimm = gapDimm / (MAX_PHASE_IN_FINE_ADJUSTMENT * 2);
      }
    }


    if (PcdGetBool (PcdMrcDebugTurnarounds) == TRUE) {
      RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "gapDimm = %d\n", gapDimm);
    }

    if (gap < gapDimm) {
      gap = gapDimm;
    }
  } // Dimm loop

  if (Mode == DD_MODE) {
    //
    // Account for separation between commands (convert pi ticks to DCLK)
    //
    maxGap = 0;
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (maxGap < (strobeMax[Strobe] - strobeMin[Strobe])) {
        maxGap = strobeMax[Strobe] - strobeMin[Strobe];
      }
    } // Strobe loop

    CalcFinalGap (&gap, maxGap, gapDimm);
  }


  if (PcdGetBool (PcdMrcDebugTurnarounds) == TRUE) {
    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "cas2RecEnGap = %d\n", gap);
  }

  return (UINT8) gap;
} // Cas2RecEnGap

/**

  Get Cas2DrvEnGap

  @param[in]  Host                - Point to sysHost
  @param[in]  Socket              - Socket number
  @param[in]  Channel             - Channel number
  @param[in]  Mode                - Mode
  @param[in]  DdrtTurnaroundFlag  - DDRT turnaround flag

  @retval N/A

**/
UINT8
EFIAPI
Cas2DrvEnGap (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Mode,
  IN BOOLEAN   DdrtTurnaroundFlag
  )
{
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               Strobe;
  INT16               piDelay;
  UINT16              gap = 0;
  UINT16              gapDimm = 0;
  UINT16              cas2DrvEn;
  UINT16              maxGap = 0;
  UINT16              strobeMin[MAX_STROBE];
  UINT16              strobeMax[MAX_STROBE];
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);

  ChannelNvList = GetChannelNvList (Host, Socket);
  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  if (Mode == DD_MODE) {
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      strobeMin[Strobe] = 0xFFFF;
      strobeMax[Strobe] = 0;
    } // Strobe loop
  }

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }
    if (((*DimmNvList)[Dimm].DcpmmPresent == 1) && (DdrtTurnaroundFlag == 0)) {
      continue;
    }

    if (PcdGetBool (PcdMrcDebugTurnarounds) == TRUE) {

      RcDebugPrint (SDBG_MAX,
        "\nCAS To Drive Enable Data %d DIMM %d\n", Channel, Dimm);

      if (Mode == DD_MODE) {
        RcDebugPrint (SDBG_MAX,
          "Different DIMM\n");

      } else {
        RcDebugPrint (SDBG_MAX,
          "Different Rank\n");
      }

      RcDebugPrint (SDBG_MAX,
        "           | rank 0 | rank 1 | rank 2 | rank 3\n", Dimm);
      RcDebugPrint (SDBG_MAX,
        "-----------------------------------------------\n");

    }

    if (Mode == DR_MODE) {
      gapDimm = 0;

      for (Strobe = 0; Strobe < MSVx4; Strobe++) {
        strobeMin[Strobe] = 0xFFFF;
        strobeMax[Strobe] = 0;
      } // Strobe loop
    }


    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      //
      // Skip if this is an ECC Strobe when ECC is disabled
      //
      if (IsStrobeNotValid (Host, Strobe)) {
        continue;
      }

      if (PcdGetBool (PcdMrcDebugTurnarounds) == TRUE) {
        RcDebugPrint (SDBG_MAX, "Strobe %2d:", Strobe);
      }

      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (FeaturePcdGet (PcdCteBuild) == TRUE) {
          GetSetDataGroup (Host, Socket, Channel, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, &piDelay);
        } else {
          GetSetDataGroup (Host, Socket, Channel, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, &piDelay);
        }
        cas2DrvEn = piDelay;

        if (PcdGetBool (PcdMrcDebugTurnarounds) == TRUE) {
          RcDebugPrint (SDBG_MAX,
            " |  %3d ", cas2DrvEn);
        }

        UpdateMinMaxUint (cas2DrvEn, &strobeMin[Strobe], &strobeMax[Strobe]);
      } // Rank loop

      if (PcdGetBool (PcdMrcDebugTurnarounds) == TRUE) {
        RcDebugPrint (SDBG_MAX, "\n");
      }

    } // Strobe loop
    if (Mode == DR_MODE) {

      maxGap = 0;
      for (Strobe = 0; Strobe < MSVx4; Strobe++) {
        if (maxGap < (strobeMax[Strobe] - strobeMin[Strobe])) {
          maxGap = strobeMax[Strobe] - strobeMin[Strobe];
        }
      } // Strobe loop

      if (maxGap >= MEM_CHIP_POLICY_VALUE (Host, cas2DrvenMaxGap)) {
        gapDimm = maxGap - MEM_CHIP_POLICY_VALUE (Host, cas2DrvenMaxGap);
      } else {
        gapDimm = 0;
      }

      //
      // Account for separation between commands (convert pi ticks to DCLK)
      //
      if ((gapDimm % (MAX_PHASE_IN_FINE_ADJUSTMENT * 2)) > 0) {
        gapDimm = (gapDimm / (MAX_PHASE_IN_FINE_ADJUSTMENT * 2)) + 1;
      } else {
        gapDimm = gapDimm / (MAX_PHASE_IN_FINE_ADJUSTMENT * 2);
      }
    }

    if (PcdGetBool (PcdMrcDebugTurnarounds) == TRUE) {
      RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "gapDimm = %d\n", gapDimm);
    }

    if (gap < gapDimm) {
      gap = gapDimm;
    }

  } // Dimm loop

  if (Mode == DD_MODE) {
    maxGap = 0;
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (maxGap < (strobeMax[Strobe] - strobeMin[Strobe])) {
        maxGap = strobeMax[Strobe] - strobeMin[Strobe];
      }
    } // Strobe loop

    if (maxGap >= MEM_CHIP_POLICY_VALUE (Host, cas2DrvenMaxGap)) {
      maxGap = maxGap - MEM_CHIP_POLICY_VALUE (Host, cas2DrvenMaxGap);
    } else {
      maxGap = 0;
    }

    if ((maxGap % (MAX_PHASE_IN_FINE_ADJUSTMENT * 2)) > 0) {
      gap = (maxGap / (MAX_PHASE_IN_FINE_ADJUSTMENT * 2)) + 1;
    } else {
      gap = maxGap / (MAX_PHASE_IN_FINE_ADJUSTMENT * 2);
    }

  }

  if (PcdGetBool (PcdMrcDebugTurnarounds) == TRUE) {
    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "cas2DrvEnGap = %d\n", gap);
  }

  return (UINT8) gap;
} // Cas2DrvEnGap

/*++

  Effective Bus Delay

  @param[in]  Host                 - Point to sysHost
  @param[in]  Socket               - Socket number
  @param[in]  Channel              - Channel number
  @param[out] EffectiveBusDelaySr  - effective bus delay for same rank (SR)
  @param[out] EffectiveBusDelayDr  - effective bus delay for different rank same DIMM (DR)
  @param[out  EffectiveBusDelayDd  - effective bus delay for different DIMM (DD)
  @param[in]  DdrtTurnaroundFlag   - DDRT Turnaround flag

  @retval N/A

--*/

VOID
EFIAPI
EffectiveBusDelay (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  OUT INT32   *EffectiveBusDelaySr,
  OUT INT32   *EffectiveBusDelayDr,
  OUT INT32   *EffectiveBusDelayDd,
  IN BOOLEAN   DdrtTurnaroundFlag
  )
{
  UINT8                   Dimm;
  UINT8                   Rank;
  UINT8                   logRank;
  UINT8                   Strobe;
  UINT8                   Nmode;
  UINT8                   RoundTrip;
  INT16                   RcvEnPi;
  INT16                   TxDqsPi;
  UINT8                   preamble;
  INT16                   CwlAdj;
  UINT16                  IntDelta;
  UINT16                  RTL;
  UINT16                  CL;
  UINT16                  DDRTCL = 0;
  UINT32                  EffectiveBusRoundTrip[MAX_DIMM][MAX_RANK_CH][MAX_STROBE];
  UINT32                  EffBusRT = 0;
  UINT32                  TxDelay[MAX_DIMM][MAX_RANK_CH][MAX_STROBE];
  UINT32                  TxDelayTemp = 0;
  UINT32                  IOComp;
  UINT8                   IoLatency;
  UINT32                  longPreambleEnable;
  struct channelNvram     (*ChannelNvList)[MAX_CH];
  struct dimmNvram        (*DimmNvList)[MAX_DIMM];
  struct ddrRank          (*rankList)[MAX_RANK_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);

  ChannelNvList = GetChannelNvList (Host, Socket);
  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  *EffectiveBusDelaySr = 0;
  *EffectiveBusDelayDr = 0;
  *EffectiveBusDelayDd = 0;

  EffectiveBusDelayInit (Host, Socket, Channel, 0, &CwlAdj, &IOComp, &IntDelta, &longPreambleEnable);

  preamble  = (UINT8) (PiTicks * (longPreambleEnable + 1));
  CL        = ((*ChannelNvList)[Channel].common.nCL) * PiTicks * 2;
  Nmode     = ((*ChannelNvList)[Channel].timingMode);

  if (Nmode > 0) {
    Nmode = Nmode - 1;
  }
  Nmode = Nmode * 2 * PiTicks;

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }

    EffectiveBusDelayInit (Host, Socket, Channel, Dimm, &CwlAdj, &IOComp, &IntDelta, &longPreambleEnable);

    rankList = GetRankNvList (Host, Socket, Channel, Dimm);

#ifdef DDRT_SUPPORT
    UINT8 DDRTCLtemp = 0;
    UINT8 DDRTCWLtemp = 0;

    if (((*DimmNvList)[Dimm].DcpmmPresent == 1) && (DdrtTurnaroundFlag == 1)) {
      GetDdrtCasLatencies (Socket, Channel, Dimm, &DDRTCLtemp, NULL, &DDRTCWLtemp, NULL);
      DDRTCL = DDRTCLtemp * 2 * PiTicks;

      if (PcdGetBool (PcdMrcDebugTurnarounds) == TRUE) {

        RcDebugPrint (SDBG_MAX,
          "PMem Present CL = %d\n", DDRTCLtemp);
        RcDebugPrint (SDBG_MAX,
          "emEM Present CL(piticks) = %d\n", DDRTCL);
        RcDebugPrint (SDBG_MAX,
          "PMem Present CWL = %d\n", DDRTCWLtemp);
        RcDebugPrint (SDBG_MAX,
          "PMem Present CWL(piticks) = %d\n", DDRTCWLtemp * 2 * PiTicks);

      }
    }
#endif // DDRT_SUPPORT

    for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {

      if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
        continue;
      }
      //
      // Get the logical rank #
      //
      logRank = GetLogicalRank (Host, Socket, Channel, Dimm, Rank);

      GetIoLatency (Socket, Channel, logRank, GSM_READ_ONLY, &IoLatency);
      for (Strobe = 0; Strobe < MSVx4; Strobe++) {
        EffectiveBusRoundTrip[Dimm][ (*rankList)[Rank].rankIndex][Strobe] = 0;
        TxDelay[Dimm][ (*rankList)[Rank].rankIndex][Strobe] = 0;
      } // Strobe loop

      //
      // Get round trip latency for each channel
      //
      GetRoundTrip (Socket, Channel, logRank, GSM_READ_CSR, &RoundTrip);
      RTL = RoundTrip * PiTicks;

      for (Strobe = 0; Strobe < MSVx4; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValid (Host, Strobe)) {
          continue;
        }

        if (FeaturePcdGet (PcdCteBuild) == TRUE) {
          GetSetDataGroup (Host, Socket, Channel, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RecEnDelay, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, &RcvEnPi);
          GetSetDataGroup (Host, Socket, Channel, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, &TxDqsPi);
        } else {
          GetSetDataGroup (Host, Socket, Channel, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RecEnDelay, GSM_READ_ONLY, &RcvEnPi);
          GetSetDataGroup (Host, Socket, Channel, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, &TxDqsPi);
        }

        if (((*DimmNvList)[Dimm].DcpmmPresent == 1) && (DdrtTurnaroundFlag == 1)) {
          EffectiveBusRoundTrip[Dimm][ (*rankList)[Rank].rankIndex][Strobe] = RTL - DDRTCL - (UINT16) IOComp - ((UINT16) PiTicks * IoLatency) - \
                                                                               Nmode + RcvEnPi + IntDelta + preamble;
        } else {
          EffectiveBusRoundTrip[Dimm][ (*rankList)[Rank].rankIndex][Strobe] = RTL - CL - (UINT16) IOComp - ((UINT16) PiTicks * IoLatency) - \
                                                                               Nmode + RcvEnPi + IntDelta + preamble;
        }

        if (PcdGetBool (PcdMrcDebugTurnarounds) == TRUE) {

          RcDebugPrint (SDBG_MAX,
                         "**************************************************************\n", RTL);
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,"\n");
          RcDebugPrint (SDBG_MAX,
            " RTL - CL - IOComp - IoLatency - Nmode + RcvEnPi + IntDelta + preamble\n");

#ifdef DDRT_SUPPORT
          if (((*DimmNvList)[Dimm].DcpmmPresent == 1) && (DdrtTurnaroundFlag == 1)) {

            RcDebugPrint (SDBG_MAX,
              "    0x%X - 0x%X - 0x%X", RTL, DDRTCL, IOComp);
          }
#endif // DDRT_SUPPORT
          if (((*DimmNvList)[Dimm].DcpmmPresent == 0) && (DdrtTurnaroundFlag != 1)) {
            RcDebugPrint (SDBG_MAX,
              "    0x%X - 0x%X - 0x%X", RTL, CL, IOComp);
          }

          RcDebugPrint (SDBG_MAX,
            " - 0x%X",  ((UINT16)PiTicks * IoLatency) );
          RcDebugPrint (SDBG_MAX,
            " - 0x%X + 0x%X", Nmode, RcvEnPi);
          RcDebugPrint (SDBG_MAX,
            " + 0x%X + 0x%X", IntDelta, preamble);
          RcDebugPrint (SDBG_MAX,
            " = 0x%X\n", EffectiveBusRoundTrip[Dimm][(*rankList)[Rank].rankIndex][Strobe]);

#ifdef DDRT_SUPPORT
          if (((*DimmNvList)[Dimm].DcpmmPresent == 1) && (DdrtTurnaroundFlag == 1)) {

            RcDebugPrint (SDBG_MAX,
              "    %d - %d - %d", RTL/128, DDRTCL/128, IOComp/128);
          }
#endif // DDRT_SUPPORT
          if (((*DimmNvList)[Dimm].DcpmmPresent == 0) && (DdrtTurnaroundFlag != 1)) {
            RcDebugPrint (SDBG_MAX,
              "    %d - %d - %d", RTL/128, CL/128, IOComp/128);
          }

          RcDebugPrint (SDBG_MAX,
            " - %d",  ((UINT16)PiTicks * IoLatency) / 128 );
          RcDebugPrint (SDBG_MAX,
            " - %d + %d", Nmode/128, RcvEnPi/128);
          RcDebugPrint (SDBG_MAX,
            " + %d + %d", IntDelta/128, preamble/128);
          RcDebugPrint (SDBG_MAX,
            " = %d\n", (EffectiveBusRoundTrip[Dimm][(*rankList)[Rank].rankIndex][Strobe])/128);
        }

        if ((TxDqsPi + CwlAdj) >0 ) {
          TxDelay[Dimm][(*rankList)[Rank].rankIndex][Strobe] = (UINT16)(TxDqsPi) + CwlAdj;// - (PiTicks / 2);
        }

        if (PcdGetBool (PcdMrcDebugTurnarounds) == TRUE) {

          RcDebugPrint (SDBG_MAX,
            " TxDqsPi + CwlAdj = TxDelay\n");
          RcDebugPrint (SDBG_MAX,
            "  0x%X", TxDqsPi);
          RcDebugPrint (SDBG_MAX,
            " + 0x%X", CwlAdj);
          RcDebugPrint (SDBG_MAX,
            " = 0x%X\n", TxDelay[Dimm][(*rankList)[Rank].rankIndex][Strobe]);
          RcDebugPrint (SDBG_MAX,
            " TxDqsPi + CwlAdj = TxDelay\n");
          RcDebugPrint (SDBG_MAX,
            "  %d", TxDqsPi/128);
          RcDebugPrint (SDBG_MAX,
            " + %d", CwlAdj/128);
          RcDebugPrint (SDBG_MAX,
            " = %d\n", (TxDelay[Dimm][(*rankList)[Rank].rankIndex][Strobe])/128);

        }

      } // Strobe loop
    } // Rank loop
  } // Dimm loop

  //
  // Calculate effective bus delay for same rank (SR)
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }
    if (((*DimmNvList)[Dimm].DcpmmPresent == 1) && (DdrtTurnaroundFlag == 0)) {
      continue;
    }

    rankList = GetRankNvList (Host, Socket, Channel, Dimm);

    for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {

      if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
        continue;
      }

      for (Strobe = 0; Strobe < MSVx4; Strobe++) {
        if (IsStrobeNotValid (Host, Strobe)) {
          continue;
        }

        if ((INT32)(EffectiveBusRoundTrip[Dimm][ (*rankList)[Rank].rankIndex][Strobe] -
             TxDelay[Dimm][ (*rankList)[Rank].rankIndex][Strobe]) >= *EffectiveBusDelaySr) {

          *EffectiveBusDelaySr = EffectiveBusRoundTrip[Dimm][ (*rankList)[Rank].rankIndex][Strobe] -
            TxDelay[Dimm][ (*rankList)[Rank].rankIndex][Strobe];

          if (PcdGetBool (PcdMrcDebugTurnarounds) == TRUE) {

          RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
                                  "EffBusRT %d, TxDelayTemp %d, EffectiveBusDelaySr = %d\n",
                                  EffectiveBusRoundTrip[Dimm][ (*rankList)[Rank].rankIndex][Strobe],
                                  TxDelay[Dimm][(*rankList)[Rank].rankIndex][Strobe], *EffectiveBusDelaySr);
          }

        }
      } // Strobe loop
    } // Rank loop
  } // Dimm loop

  //
  // Calculate effective bus delay for different rank same DIMM (DR)
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }
    if (((*DimmNvList)[Dimm].DcpmmPresent == 1) && (DdrtTurnaroundFlag == 0)) {
      continue;
    }

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValid (Host, Strobe)) {
        continue;
      }

      rankList = GetRankNvList (Host, Socket, Channel, Dimm);

      EffBusRT = 0;
      TxDelayTemp = 0xFFF;

      for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {
        if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        if (EffBusRT < EffectiveBusRoundTrip[Dimm][ (*rankList)[Rank].rankIndex][Strobe]) {
          EffBusRT = EffectiveBusRoundTrip[Dimm][ (*rankList)[Rank].rankIndex][Strobe];
        }
        if (TxDelayTemp > TxDelay[Dimm][ (*rankList)[Rank].rankIndex][Strobe]) {
          TxDelayTemp = TxDelay[Dimm][ (*rankList)[Rank].rankIndex][Strobe];
        }

      } // Rank loop

      if ((INT32)(EffBusRT - TxDelayTemp) >= *EffectiveBusDelayDr) {
        *EffectiveBusDelayDr = EffBusRT - TxDelayTemp;

        if (PcdGetBool (PcdMrcDebugTurnarounds) == TRUE) {

          RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, Strobe, NO_BIT,
                                  "EffBusRT %d, TxDelayTemp %d, EffectiveBusDelayDr = %d\n",
                                  EffBusRT, TxDelayTemp, *EffectiveBusDelayDr);
        }

      }
    } // Strobe loop
  } // Dimm loop

  //
  // Calculate effective bus delay for different DIMM (DD)
  //
  for (Strobe = 0; Strobe < MSVx4; Strobe++) {
    if (IsStrobeNotValid (Host, Strobe)) {
      continue;
    }

    EffBusRT = 0;
    TxDelayTemp = 0xFFF;

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      rankList = GetRankNvList (Host, Socket, Channel, Dimm);

      for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {
        if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        if (EffBusRT < EffectiveBusRoundTrip[Dimm][ (*rankList)[Rank].rankIndex][Strobe]) {
          EffBusRT = EffectiveBusRoundTrip[Dimm][ (*rankList)[Rank].rankIndex][Strobe];
        }
        if (TxDelayTemp > TxDelay[Dimm][ (*rankList)[Rank].rankIndex][Strobe]) {
          TxDelayTemp = TxDelay[Dimm][ (*rankList)[Rank].rankIndex][Strobe];
        }
      } // Rank loop
    } // Dimm loop

    if ((INT32)(EffBusRT - TxDelayTemp) >= *EffectiveBusDelayDd) {
      *EffectiveBusDelayDd = EffBusRT - TxDelayTemp;

      if (PcdGetBool (PcdMrcDebugTurnarounds) == TRUE) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, Strobe, NO_BIT,
                                "EffBusRT %d, TxDelayTemp %d, EffectiveBusDelayDd = %d\n",
                                EffBusRT, TxDelayTemp, *EffectiveBusDelayDd);
      }

    }
  } // Strobe loop


  if (PcdGetBool (PcdMrcDebugTurnarounds) == TRUE) {

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "EffBusRT %d, TxDelayTemp %d, EffectiveBusDelayDd = %d\n",
                            EffBusRT, TxDelayTemp, *EffectiveBusDelayDd);
    RcDebugPrint (SDBG_MAX,
                            "**************************************************************\n");

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "EffectiveBusDelaySr = %d -> %d\n", *EffectiveBusDelaySr, (*EffectiveBusDelaySr + (2 * PiTicks) - 1) / (2 * PiTicks));
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "EffectiveBusDelayDr = %d -> %d\n", *EffectiveBusDelayDr, (*EffectiveBusDelayDr + (2 * PiTicks) - 1) / (2 * PiTicks));
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "EffectiveBusDelayDd = %d -> %d\n", *EffectiveBusDelayDd, (*EffectiveBusDelayDd + (2 * PiTicks) - 1) / (2 * PiTicks));
  }

  *EffectiveBusDelaySr = (*EffectiveBusDelaySr + (2 * PiTicks) - 1) / (2 * PiTicks);
  *EffectiveBusDelayDr = (*EffectiveBusDelayDr + (2 * PiTicks) - 1) / (2 * PiTicks);
  *EffectiveBusDelayDd = (*EffectiveBusDelayDd + (2 * PiTicks) - 1) / (2 * PiTicks);

  return;

} // EffectiveBusDelay

/**

  @param[in]  Host    - Pointer to sysHost
  @param[in]  Socket  - Socket number
  @param[in]  Channel - Channel number
  @param[in]  Mode    - Timing Mode: SR_MODE, DR_MODE, or DD_MODE

  @retval Maximum delay between clocks across DIMMs in Pi ticks

**/
UINT8
EFIAPI
CkDiffGap (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Mode
  )
{

  if (GetMaxClkDiffDD (Host, Socket, Channel, Mode) > 39) {
    return 1;
  } else {
    return 0;
  }

} // CkDiffGap

/*++

  Finds the maximum clock difference between DIMMs in a channel

  @param[in]  Host        - Pointer to sysHost
  @param[in]  Socket      - Socket number
  @param[in]  Channel     - Channel number
  @param[in]  Mode        - Timing Mode: SR_MODE, DR_MODE, or DD_MODE

  @retval Maximum delay between clocks across DIMMs in Pi ticks

--*/

UINT16
GetMaxClkDiffDD (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Mode
  )
{
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               clocksFound = 0;
  UINT8               clkIndex;
  INT16               clkDelay[MAX_RANK_CH];
  UINT16              diff = 0;
  UINT16              diffTemp = 0;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];

  ChannelNvList = GetChannelNvList (Host, Socket);
  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  //
  // No need to ckeck for differences between DIMMs if there are not multiple DIMMs
  //
  if ((*ChannelNvList)[Channel].maxDimm < 2) {
    return 0;
  }

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }

    if (Mode == DR_MODE) {
      clocksFound = 0;
    }

    rankList = GetRankNvList (Host, Socket, Channel, Dimm);
    for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
      if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
        continue;
      }
      if (FeaturePcdGet (PcdCteBuild) == TRUE) {
        GetSetClkDelay (Host, Socket, Channel, (*rankList)[Rank].ckIndex, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, &clkDelay[clocksFound]);
      } else {
        GetSetClkDelay (Host, Socket, Channel, (*rankList)[Rank].ckIndex, GSM_READ_ONLY, &clkDelay[clocksFound]);
      }
      for (clkIndex = 0; clkIndex < clocksFound; clkIndex++) {
        if (clkDelay[clocksFound] > clkDelay[clkIndex]) {
          diffTemp = clkDelay[clocksFound] - clkDelay[clkIndex];
        } else {
          diffTemp = clkDelay[clkIndex] - clkDelay[clocksFound];
        }

        if (diffTemp > (128 - diffTemp)) {
          diffTemp = 128 - diffTemp;
        }

        if (diffTemp > diff) {
          diff = diffTemp;
        }
      } // clkIndex Loop

      if (PcdGetBool (PcdMrcDebugTurnarounds) == TRUE) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                                "CLK %d Pi Delay = %d\n", (*rankList)[Rank].ckIndex, clkDelay[clocksFound]);
      }

      if (Mode == DR_MODE) {

        if (PcdGetBool (PcdMrcDebugTurnarounds) == TRUE) {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, "diff = %d\n", diff);
        }

      }
      clocksFound++;
    } // Rank loop
  } // Dimm loop


  if (Mode == DD_MODE) {

    if (PcdGetBool (PcdMrcDebugTurnarounds) == TRUE) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Diff = %d\n", diff);
    }

  }

  return diff;
} // GetMaxClkDiffDD

/**
  Indicate if long (2TCLK) preamble is allowed by the configuration

  @param[in] Socket - Socket number

  @retval   TRUE if long preamble allowed
            FALSE otherwise
**/

BOOLEAN
LongPreambleAllowed (
  IN      UINT8   Socket
  )
{
  PSYSHOST        Host;
  BOOLEAN         AllowLongPreamble = FALSE;

  Host = GetSysHostPointer ();

  //
  // The 2 tCK preamble is valid for DDR4-2400 and higher speed bin tables
  //
  if (Host->nvram.mem.socket[Socket].ddrFreq >= DDR_2400) {
    AllowLongPreamble = TRUE;
  }
  return AllowLongPreamble;

} // LongPreambleAllowed

/**
  Return the preamble timing setting

  @param[in] Socket         - Socket number
  @param[in] PreambleType   - READ_PREAMBLE, WRITE_PREAMBLE or READ_OR_WRITE_PREAMBLE

  @retval   PREAMBLE_1TCLK, PREAMBLE_2TCLK
**/
UINT8
GetPreambleState (
  IN      UINT8   Socket,
  IN      UINT8   PreambleType
  )
{
  PSYSHOST        Host;
  UINT8           PreambleState = PREAMBLE_1TCLK;
  SYS_SETUP       *Setup;

  Setup = GetSysSetupPointer ();

  Host = GetSysHostPointer ();

  if (IsDdr5Present (Host, Socket)) {
    return GetPreambleStateDdr5 (Socket, PreambleType);
  }

  if (LongPreambleAllowed (Socket)) {

    //
    // 2TCLK Write preamble is enabled if:
    // - long preamble is allowed
    //   AND
    // - writePreamble Setup option is set to PREAMBLE_2TCLK
    //
    if ((PreambleType & WRITE_PREAMBLE) != 0) {
      if (Setup->mem.writePreamble == PREAMBLE_2TCLK) {
        PreambleState = PREAMBLE_2TCLK;
      }
    }

    //
    // 2TCLK Read preamble is enabled if:
    // - long preamble is allowed
    //   AND
    // - Setup option is not set to PREAMBLE_1TCLK (this forces 1TCLK)
    //   AND
    //    - readPreamble Setup option is set to PREAMBLE_2TCLK
    //      OR
    //    - RxDfe training is enabled
    //
    if (((PreambleType & READ_PREAMBLE) != 0) && (Setup->mem.readPreamble != PREAMBLE_1TCLK)) {
      if ((Setup->mem.readPreamble == PREAMBLE_2TCLK) ||
          (GetRxDfeEnableChip (Host, Socket) == TRUE)) {
        PreambleState = PREAMBLE_2TCLK;
      }
    }
  }

  return PreambleState;

}

/**

  Return the Postamble timing setting

  @param[in] Socket         - Socket number
  @param[in] PostambleType  - READ_POSTAMBLE, WRITE_PREAMBLE or READ_OR_WRITE_PREAMBLE

  @retval POSTAMBLE_0P5TCLK, POSTAMBLE_1P5TCLK

**/
UINT8
GetPostambleState (
  IN      UINT8   Socket,
  IN      UINT8   PostambleType
  )
{
  UINT8           PostambleState;
  //
  // Default setting is POSTAMBLE_0P5TCLK
  //
  PostambleState = POSTAMBLE_0P5TCLK;

  return PostambleState;
}

/**
  Return the preamble timing in tCK units

  @param[in] Socket         - Socket number
  @param[in] PreambleType   - READ_PREAMBLE, WRITE_PREAMBLE or READ_OR_WRITE_PREAMBLE

  @retval   tWPRE in tCK units
**/
UINT8
GetTWpre (
  IN      UINT8   Socket,
  IN      UINT8   PreambleType
  )
{
  UINT8 TWpre;

  switch (GetPreambleState (Socket, PreambleType)) {
    case PREAMBLE_4TCLK:
      TWpre = 4;
      break;
    case PREAMBLE_3TCLK:
      TWpre = 3;
      break;
    case PREAMBLE_2TCLK:
      TWpre = 2;
      break;
    case PREAMBLE_1TCLK:
    default:
      TWpre = 1;
      break;
  }
  return TWpre;
}

/**

  Get Tx Flyby Delay

  @param[in]  Host    - Pointer to sysHost
  @param[in]  Socket  - Socket number
  @param[in]  Ch      - Channel number (0-based)
  @param[in]  Dimm    - DIMM number (0-based)
  @param[in]  Rank    - Rank number (0-based)
  @param[in]  Strobe  - Strobe number (0-based)
  @param[in]  Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]  Mode    - Bit-field of different modes
  @param[out] Value   - Pointer to delay value or offset based on Mode

  @retval MRC_STATUS_SUCCESS if value found
          MRC_STATUS_LEVEL_NOT_SUPPORTED if Level != DdrLevel
          MRC_STATUS_STROBE_NOT_PRESENT if Strobe number is invalid
          MRC_STATUS_FAILURE otherwise

**/

MRC_STATUS
EFIAPI
GetTxFlyByCore (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT8     Ch,
  IN    UINT8     Dimm,
  IN    UINT8     Rank,
  IN    UINT8     Strobe,
  IN    MRC_LT    Level,
  IN    UINT8     Mode,
  OUT   INT16     *Value
  )
{
  MRC_STATUS      Status;
  INT16           CwlAdj;
  INT16           WrLvlDelayValue = 0;
  UINT8           MSVx4 = GetMaxStrobeValid (Host);

  if ((Mode != GSM_READ_ONLY) || (Value == NULL)) {
    return MRC_STATUS_FAILURE;
  }
  if (Level != DdrLevel) {
    return MRC_STATUS_LEVEL_NOT_SUPPORTED;
  }
  if (Strobe >= MSVx4) {
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  GetCwlAdj (Host, Socket, Ch, Dimm, &CwlAdj);
  Status = GetSetTxDelay (Host, Socket, Ch, Dimm, Rank, Strobe, 0, Level, WrLvlDelay, Mode, &WrLvlDelayValue);
  if (Status == MRC_STATUS_SUCCESS) {
    *Value = CwlAdj * DCLKPITICKS + WrLvlDelayValue;
  }
  return Status;
}

/**

  Get Rx Flyby Delay

  @param[in]  Host    - Pointer to sysHost
  @param[in]  Socket  - Socket number
  @param[in]  Ch      - Channel number (0-based)
  @param[in]  SubCh   - Sub Channel number (0-based)
  @param[in]  Dimm    - DIMM number (0-based)
  @param[in]  Rank    - Rank number (0-based)
  @param[in]  Strobe  - Strobe number (0-based)
  @param[in]  Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]  Mode    - Bit-field of different modes
  @param[out] Value   - Pointer to delay value or offset based on Mode

  @retval MRC_STATUS_SUCCESS if value found
          MRC_STATUS_LEVEL_NOT_SUPPORTED if Level != DdrLevel
          MRC_STATUS_STROBE_NOT_PRESENT if Strobe number is invalid
          MRC_STATUS_FAILURE otherwise

**/

MRC_STATUS
EFIAPI
GetRxFlyByCore (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT8     Ch,
  IN    UINT8     SubCh,
  IN    UINT8     Dimm,
  IN    UINT8     Rank,
  IN    UINT8     Strobe,
  IN    MRC_LT    Level,
  IN    UINT8     Mode,
  OUT   INT16     *Value
  )
{
  MRC_STATUS      Status;
  UINT8           LogicalRank;
  UINT8           RoundTrip;
  UINT8           IoLatency;
  UINT32          IoComp = 0;
  INT16           RecEnDelayValue = 0;
  UINT8           MSVx4 = GetMaxStrobeValid (Host);

  if ((Mode != GSM_READ_ONLY) || (Value == NULL)) {
    return MRC_STATUS_FAILURE;
  }
  if (Level != DdrLevel) {
    return MRC_STATUS_LEVEL_NOT_SUPPORTED;
  }
  if (Strobe >= MSVx4) {
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  Status = GetSetRxDelay (Host, Socket, Ch, Dimm, Rank, Strobe, 0, Level, RecEnDelay, Mode, &RecEnDelayValue);

  if (Status == MRC_STATUS_SUCCESS) {

    LogicalRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);

    if ((EFI_SUCCESS != GetRoundTrip (Socket, Ch, LogicalRank, GSM_READ_ONLY, &RoundTrip)) ||
        (EFI_SUCCESS != GetIoLatency (Socket, Ch, LogicalRank, GSM_READ_ONLY, &IoLatency))) {

      Status = MRC_STATUS_FAILURE;

    } else {

      GetIoLatencyIoComp (Host, Socket, Ch, SubCh, &IoComp, GSM_READ_ONLY);
      *Value = (INT16) (RecEnDelayValue + (RoundTrip - IoLatency - IoComp) * QCLKPITICKS);

    }
  }
  return Status;

} // GetRxFlyByCore

/**
  Convert a timing parameter in picosecond (ps) units to number of clocks (nCK) units

  @param[in] Socket           - Socket number
  @param[in] TimePicosecond   - Time in ps

  @retval Time in clock units
**/
UINT16
EFIAPI
PicosecondToClocks (
  IN UINT8     Socket,
  IN UINT32    TimePicosecond
  )
{
  PSYSHOST  Host;
  UINT32    nCK;
  UINT32    tCK;

  Host = GetSysHostPointer ();

  //
  // Convert a timing parameter in picosecond (ps) units to number of clocks (nCK) units
  // This algorithm comes from JEDEC document JC45 - DDR5 SPD Content - using integer math to convert ps to nCK
  // The algorithm adds 99.8% of a clock and truncates down to the next lower integer value
  //
  //              nCK = truncate ( (((parameter_in_ps * 1000) / tCK_in_ps) + 998) / 1000 )
  //
  // GettCK returns tCK (clock period) in 0.1ps units so first convert to ps, rounding to the nearest ps
  //
  tCK = GettCK (Host, Socket);          // tCK in 0.1ps
  tCK = (tCK + 5) / 10;                 // Round to nearest ps
  nCK = (TimePicosecond * 1000) / tCK;  // Preliminary nCK calculation, scaled by 1000
  nCK = nCK + 998;                      // Apply inverse of 1% correction factor
  nCK = nCK / 1000;                     // Truncate to next lower integer

  return (UINT16)nCK;
}

/**
  Convert a timing parameter in nanosecond (ns) units to number of clocks (nCK) units

  @param[in] Socket   - Socket number
  @param[in] Time     - Time in ns

  @retval Time in clock units
**/
UINT16
EFIAPI
NanosecondToClocks (
  IN UINT8     Socket,
  IN UINT32    TimeNanosecond
  )
{
  UINT32  nCK;
  UINT32  TimePicosecond;

  TimePicosecond = TimeNanosecond * 1000;
  nCK = PicosecondToClocks (Socket, TimePicosecond);

  return (UINT16)nCK;
}

/**
  Get the system DDR read burst length, that ignores WrCRC padding

  @param n/a

  @retval Burst Length in UINT32
**/
UINT8
EFIAPI
GetReadBurstLength (
  VOID
  )
{
  UINT8 ReadBurstLength;

  //
  // DDR4 burst length is 8 without WrCRC
  //
  ReadBurstLength = 8;
#if DDR5_SUPPORT
  //
  // DDR5 burst length is 16 without WrCRC
  //
  ReadBurstLength = 16;
#endif // DDR5_SUPPORT

  return ReadBurstLength;
}

/**
  Get the system DDR write burst length, that considers WrCRC padding

  @param[in] Channel - DDR channel number (0-based) in the current socket

  @retval Burst Length in UINT32
**/
UINT8
EFIAPI
GetWriteBurstLength (
  IN UINT8 Channel
  )
{
  SYS_SETUP *Setup;
  UINT8     WriteBurstLength;

  Setup = GetSysSetupPointer ();

  WriteBurstLength = GetReadBurstLength ();

  //
  // For DDR4 and DDR5, WrCRC adds 2 to the burstlength when enabled
  //
  if (IsEarlyWrCrcRequired (Channel) || ((Setup->mem.optionsExt & WR_CRC) != 0)) {
    WriteBurstLength += 2;
  }
  return WriteBurstLength;
}

