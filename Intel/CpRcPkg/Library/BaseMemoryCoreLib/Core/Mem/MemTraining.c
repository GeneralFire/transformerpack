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
#include <Library/BaseMemoryLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/RcMemAllocLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/PcdLib.h>
#include <Library/BasicIpBlockServicesLib.h>

#include <Guid/MrcPlatformHooksGuid.h>

#include <Memory/MemoryCheckpointCodes.h>

#include "Include/BaseMemoryCoreInternal.h"
#include <Library/MemSweepLib.h>
#include <Library/Ddr5CoreLib.h>

#include <Memory/MemPorDefinitions.h>

//
// Local Prototypes
//
VOID   AdvancedCentering (PSYSHOST Host, UINT8 socket, MRC_GT group);
UINT32 VrefCentering (PSYSHOST Host, UINT8 socket, UINT8 perBit, MRC_GT group);
UINT8  VrefQuick (PSYSHOST Host, UINT8 socket, MRC_GT group, INT16 *low, INT16 *high);
UINT32 EarlyVrefCentering (PSYSHOST Host, UINT8 socket, MRC_GT group);
UINT32 CombineVrefResults (PSYSHOST Host, UINT8 socket, MRC_GT group, UINT8 perBit, VOID *value);
VOID CalculateBitCenters (PSYSHOST Host, UINT8 socket, UINT8 ch, INT8 rankBitCenter[MAX_DIMM][MAX_RANK_DIMM][MAX_BITS], struct bitMargin *resultsBit);


/**

  This function does the conversion between linear and logical strobe.
  A linear strobe is only a increasing counter that determines the number of strobes to be tested
  The logical strobe is the actual one to be trained or processed.

  DDR4:
  Linear strobe could be 0->17 (nibble) or 0->8 (byte) --> the conversion is 1:1 mapping to logical strobe

  DDR5:
  Linear strobe to Logical strobe is based on ALL or Independent Sub-Channel operation,

  0->9  (nibbles)for Sub-Channel 0 --> the conversion is 1:1 mapping to logical strobe
  0->9  (nibbles)for Sub-Channel 1 --> the conversion needs to be adjusted to logical strobes 10-19

  0->4  (bytes)for Sub-Channel 0 --> the conversion is 1:1 mapping to logical strobe
  0->4  (bytes)for Sub-Channel 1 --> the conversion needs to be adjusted to logical strobes 10-14


  @param[in]      MaxStrobeCh   - Maximum strobe number for the channel
  @param[in]      StrobeLinear  - Current linear strobe under test (0-based)

  @return   The logical strobe based on the linear strobe

**/
UINT8
StrobeLinearToLogical (
  IN  UINT8     MaxStrobeCh,
  IN  UINT8     StrobeLinear
  )
{
  UINT8   StrobeLogical;

  StrobeLogical = StrobeLinear;
#ifdef DDR5_SUPPORT

  if (StrobeLinear >= MaxStrobeCh) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_111);
    return StrobeLogical;
  }
  StrobeLogical = StrobeLinear % (MaxStrobeCh / 2) + (StrobeLinear / (MaxStrobeCh / 2)) * (MAX_STROBE_DDR5 / 2);
#endif // DDR5_SUPPORT

  return StrobeLogical;
}

/**

  Evaluate if the Rank List work is complete

  @param Host       - Pointer to sysHost
  @param socket     - Socket number
  @param ch         - Current channel
  @param dimm       - Dimm on current channel
  @param rank       - Rank of current Dimm
  @param scope      - Margin granularity
  @param ranksDone  - Array of bits showing if ranks are done. If bitX = 1, the rank is done.

  @retval N/A

**/
VOID
EvalResSetRanksDone (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     scope,
  UINT32    *ranksDone
  )
{
  UINT8           imc;
  struct  ddrRank (*rankList)[MAX_RANK_DIMM];


  imc = GetMCID (Host, socket, ch);
  rankList = GetRankNvList (Host, socket, ch, dimm);

  if ((scope == SCOPE_SOCKET) || (scope == SCOPE_CH)) {
    ranksDone[imc] = 0xFFFFFFFF;
  } else {
    ranksDone[imc] |= (1 << ((*rankList)[rank].rankIndex + (GetMCCh (imc, ch) * 8)));
  }
} // EvalResSetRanksDone


VOID
EFIAPI
AdvancedCentering (
  PSYSHOST  Host,
  UINT8     socket,
  MRC_GT    group
  )
/*++

  Re-centers rx or tx dq timing using a more robust test

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param group   - Parameter to center
                   RxDqsDelay    1: Center Rx DqDqs
                   RxDqsPDelay   2: Center Rx DqDqsp
                   RxDqsNDelay   3: Center Rx DqDqsn
                   TxDqDelay     12: Center Tx DqDqs

  @retval N/A

--*/
{
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               strobe;
  UINT8               maxStrobe;
  UINT8               maxVref;
  UINT8               numPoints = 0;
  UINT8               stepSize = 0;
  INT16               offset;
  INT16               low = 0;
  INT16               high = 0;
  INT32               numerator;
  INT32               denominator;
  INT16               initalOffset;
  INT16               worstEyeWidth = 0;
  INT16               timingCenter = 0;
  INT16               offset1;
  INT16               offset2;
  INT16               temp;
  UINT8               Multiplier = 0;
  INT16               eyeWidth;
  MRC_GT              vrefGroup = 0;
  INT16               maxOffsetFromTimingCenter = 0;
  UINT16              vrefRatio = 0;
  UINT16              vrefRatioSpec = 0;
  UINT8               mode = 0;
  UINT8               vIndex;
  UINT8               vIndexCenter;
  UINT32              patternLength = 1;
  INT16               eyeHeight;
#ifdef DEBUG_CODE_BLOCK
  INT16               delay[MAX_STROBE];
  INT16               oldDelay[MAX_STROBE];
#endif  //DEBUG_CODE_BLOCK
  struct bitMask      filter;
  struct strobeMargin (*results)[MAX_VREF_POINTS];
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MaxChDdr;
  UINT8 SubChannel = NO_SUBCH;

  MaxChDdr = GetMaxChDdr ();

  //
  // Return if this socket is disabled
  //

  if (IsSocketDimmPopulated (Host, socket) != TRUE) {
    return;
  }

  results = RcAllocatePool (sizeof (*results));
  if (results == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return;
  }
  channelNvList = GetChannelNvList (Host, socket);

  // Get pattern length
  if ((group == RxDqsDelay) || (group == RxDqsPDelay) || (group == RxDqsNDelay)) {
    mode = MODE_VIC_AGG;
    vrefGroup = RxVref;
    if (GetCurrentTestType (socket) == ReadDfeTest) {
      vrefGroup = RxSamplerEvenOdd;
    }
    patternLength = TRAIN_RD_DQS_PL;
    numPoints = RX_MULTI_VREF_POINTS;
    stepSize = RX_MULTI_VREF_STEP;

  } else if (group == TxDqDelay) {
    mode = MODE_VIC_AGG;
    vrefGroup = TxVref;
    patternLength = TRAIN_WR_DQS_PL;
    numPoints = TX_MULTI_VREF_POINTS;
    stepSize = TX_MULTI_VREF_STEP;
  }

  vIndexCenter = numPoints / 2;

  //
  // Clear the filter bits to enable error checking on every bit
  //
  ClearFilter (Host, socket, &filter);

  // Setup bitMask
  for (ch = 0; ch < MaxChDdr; ch++) {
    for (rank = 0; rank < MAX_RANK_CH; rank++) {
      for (strobe = 0; strobe < MSVx4; strobe++) {
#ifdef DEBUG_CODE_BLOCK
        oldDelay[strobe] = 0;
        delay[strobe] = 0;
#endif //DEBUG_CODE_BLOCK
        for (vIndex = 0; vIndex < numPoints; vIndex++) {
          (*results)[vIndex].strobe[ch][rank][strobe].n = 0;
          (*results)[vIndex].strobe[ch][rank][strobe].p = 0;
        }
      } // strobe loop
    } // rank loop
  } // ch loop

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "Get eye width\n");
  VrefQuick (Host, socket, group, &low, &high);
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "Low: %3d High: %3d\n", low, high);
  initalOffset = ((low + high))/2; // Doesn't matter if we round
  worstEyeWidth = -low + high;

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "Offset = %d\n", initalOffset);
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "Eye width = %d\n", worstEyeWidth);

  // Center timing for all strobes and ranks if required to get a better vref value for risk analysis
  for (ch = 0; ch < MaxChDdr; ch++) {
    //
    // Skip if channel is disabled
    //
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    dimmNvList = GetDimmNvList (Host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        //
        // Skip if no rank
        //
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        maxStrobe = GetMaxStrobe (Host, socket, ch, dimm, group, DdrLevel);

        // Get Vref Margins at timing center
        for (strobe = 0; strobe < maxStrobe; strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if (IsStrobeNotValid (Host, strobe)) {
            continue;
          }
          // Apply new center
          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, group, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &initalOffset);
        } // strobe loop
      } // rank loop
    } // dimm loop
  } // ch loop
  //
  // Get eye height (Vref margins)
  //
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "Get eye height\n");
  VrefQuick (Host, socket, vrefGroup, &low, &high);
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "Low: %3d High: %3d\n", low, high);
  eyeHeight = (-low) + high;

  if ((vrefGroup == RxVref) || (vrefGroup == RxSamplerEvenOdd)) {
    eyeHeight = (5 * eyeHeight) / 8;
  }

  if (eyeHeight < 8) {
    eyeHeight = 8;
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "New Eye height = %d\n", eyeHeight);
  }
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "Eye height = %d\n", eyeHeight);

  // Now find the ratio between the worst-case timing and voltage margins across all rank and figure out it we are
  // timing or voltage margin limited.
  vrefRatio = (worstEyeWidth * 1000 / eyeHeight); // 1000x and RxVref adjusted

  // All ratios are 1000x and RxVref adjusted
  if (Host->nvram.mem.socket[socket].ddrFreq >= DDR_2400) {
    if ((group == RxDqsDelay) || (group == RxDqsPDelay) || (group == RxDqsNDelay)) {
      vrefRatioSpec = 1467; // * 1000
    } else if (group == TxDqDelay) {
      vrefRatioSpec = 1300; // * 1000
    }
  } else if (Host->nvram.mem.socket[socket].ddrFreq >= DDR_2133) {
    if ((group == RxDqsDelay) || (group == RxDqsPDelay) || (group == RxDqsNDelay)) {
      vrefRatioSpec = 1333; // * 1000
    } else if (group == TxDqDelay) {
      vrefRatioSpec = 1200; // * 1000
    }
  } else if (Host->nvram.mem.socket[socket].ddrFreq >= DDR_1866) {
    if ((group == RxDqsDelay) || (group == RxDqsPDelay) || (group == RxDqsNDelay)) {
      vrefRatioSpec = 1333; // * 1000
    } else if (group == TxDqDelay) {
      vrefRatioSpec = 1200; // * 1000
    }
  } else if (Host->nvram.mem.socket[socket].ddrFreq >= DDR_1600) {
    if ((group == RxDqsDelay) || (group == RxDqsPDelay) || (group == RxDqsNDelay)) {
      vrefRatioSpec = 1200; // * 1000
    } else if (group == TxDqDelay) {
      vrefRatioSpec = 1100; // * 1000
    }
  } else { // Anything lower than 1600
    if ((group == RxDqsDelay) || (group == RxDqsPDelay) || (group == RxDqsNDelay)) {
      vrefRatioSpec = 1200; // * 1000
    } else if (group == TxDqDelay) {
      vrefRatioSpec = 1100; // * 1000
    }
  }

  // Find the minimum timing offset from 1D timing center (given a worst-case triangular eye) that must be set to end up
  // timing margin limited. Make sure if we are doing a 2D centering, we do not run over this value.
  numerator = (INT32)(1000 * worstEyeWidth * worstEyeWidth - vrefRatioSpec * eyeHeight * worstEyeWidth); // *1000 to account for VrefMultiplier * 1000. Numerator is 1000x what it really is
  if (numerator < 0) {
    numerator = 0;
  }
  denominator = (INT32)(worstEyeWidth * 2 * 1000 + 2 * vrefRatioSpec * eyeHeight); // 1000 to account for VrefMultiplier * 1000. Denominator is 1000x what it really is
  if (denominator < 0) {
    denominator = -denominator;
  }
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "numerator: %9d\n", numerator);
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "denominator: %9d\n", denominator);

  maxOffsetFromTimingCenter = (INT16)(numerator / denominator); // No need to round, since we want minimum
  if (maxOffsetFromTimingCenter < 0) {
    maxOffsetFromTimingCenter = -maxOffsetFromTimingCenter;
  }
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "vrefRatio: %5d, vrefRatioSpec: %5d\n", vrefRatio, vrefRatioSpec);
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Max offset from timing center: %3d\n", maxOffsetFromTimingCenter);
  //
  // 1D timing centering
  //
  if (vrefRatio <= vrefRatioSpec) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Timing Limited\n");
  } else {
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Voltage Limited\n");
  }

  vIndexCenter = numPoints / 2;

  //
  // Set eyeHeight
  //
  //eyeHeight = vrefResults.p - vrefResults.n;

  //
  // Calculate Multi-Vref step size
  //
  stepSize = (UINT8) (eyeHeight / 8);
  maxVref = (numPoints / 2) * stepSize;

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "GetMultiVref: patternLength = %d, stepSize = %d, numPoints = %d\n",
                patternLength, stepSize, numPoints);

  // Get margins per strobe group
  GetMultiVref (Host, socket, DdrLevel, group, mode, SCOPE_STROBE, &filter, (VOID *) (*results), patternLength, 0, 0, stepSize, numPoints);
#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    AcquirePrintControl ();

    if (group == RxDqsDelay) {
      RcDebugPrint (SDBG_DEFAULT, "START_RX_DQS_ADV\n");
    } else if (group == RxDqsPDelay) {
      RcDebugPrint (SDBG_DEFAULT, "START_RX_DQSP_ADV\n");
    } else if (group == RxDqsNDelay) {
      RcDebugPrint (SDBG_DEFAULT, "START_RX_DQSN_ADV\n");
    } else if (group == TxDqDelay) {
      RcDebugPrint (SDBG_DEFAULT, "START_TX_DQ_ADV\n");
    }
  }
#endif  // DEBUG_CODE_BLOCK

  for (ch = 0; ch < MaxChDdr; ch++) {
    //
    // Skip if channel is disabled
    //
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, socket, ch);

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

        maxStrobe = GetMaxStrobe (Host, socket, ch, dimm, group, DdrLevel);

        for (strobe = 0; strobe < maxStrobe; strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if (IsStrobeNotValid (Host, strobe)) {
            continue;
          }

          if (IsDdr5Present (Host, socket)) {
            if (strobe < maxStrobe / 2) {
              SubChannel = SUB_CH_A;
            } else {
              SubChannel = SUB_CH_B;
            }
          }
          for (vIndex = 0; vIndex < numPoints; vIndex++) {
            if ((*results)[vIndex].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n >
                (*results)[vIndex].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p) {

              (*results)[vIndex].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n = 0;
              (*results)[vIndex].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p = 0;
            }
          } // vIndex loop

          //
          // Get the total eye width
          //
          eyeWidth = (*results)[vIndexCenter].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p -
            (*results)[vIndexCenter].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n;
          timingCenter = ((*results)[vIndexCenter].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p +
                          (*results)[vIndexCenter].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n) /2;
          // Round to counter pattern asymmetry issues
          temp = ((*results)[vIndexCenter].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p + (*results)[vIndexCenter].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n);
          if (temp < 0) {
            temp = -temp;
          }
          if ((temp % 2) == 1) {
            if ((*results)[vIndexCenter].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p + (*results)[vIndexCenter].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n < 0) {
              timingCenter -= 1;
            } else {
              timingCenter += 0;
            }
          }
          Multiplier = 1; //BDX Multiplier

          //
          // If RX centering and speed > 1866, then do 1D centering calculation
          //
          if (numPoints == 1) {
            offset = ((*results)[vIndexCenter].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p + (*results)[vIndexCenter].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n) / 2;
          } else {
            //RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
            //              "EW = R0 + L0 = %d\n", eyeWidth);
            offset = eyeHeight * Multiplier * ((2 * (*results)[vIndexCenter].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p) +
                                               (2 * (*results)[vIndexCenter].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n));
            //RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
            //              "Center = EH * (2*R0 + 2*L0) = %d\n", offset);
            offset += ((((10 * eyeWidth) / 3) + ((10 * 2 * eyeHeight) / 3)) / 10) *
            ((*results)[vIndexCenter + 1].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p +
             (*results)[vIndexCenter - 1].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p +
             (*results)[vIndexCenter + 1].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n +
             (*results)[vIndexCenter - 1].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n);
            //RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
            //                        "Center += (1/3EW + 2/3EH) * (R + 1 + R-1 + L + 1 + L-1) = %d\n", offset);
            offset += ((((10 * 2 * eyeWidth) / 3) + ((10 * eyeHeight) / 3)) / 10)*
            ((*results)[vIndexCenter + 2].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p +
             (*results)[vIndexCenter - 2].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p +
             (*results)[vIndexCenter + 2].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n +
             (*results)[vIndexCenter - 2].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n);
            //RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
            //                        "Center += (2/3EW + 1/3EH) * (R + 2 + R-2 + L + 2 + L-2) = %d\n", offset);
            offset += eyeWidth *
            ((*results)[vIndexCenter + 3].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p +
             (*results)[vIndexCenter - 3].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p +
             (*results)[vIndexCenter + 3].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n +
             (*results)[vIndexCenter - 3].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n);
            //RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
            //                        "Center += EW * (R + 3 + R-3 + L + 3 + L-3) = %d\n", offset);
            offset1 = 100 * ((offset / ((4 * eyeWidth) + (4 * eyeHeight))) / 2);
            offset2 = ((100 * offset) / ((4* eyeWidth) + (4 * eyeHeight))) / 2;
            if ((offset2 - offset1) >= 50) {
              offset = (offset1 / 100) + 1;
            } else {
              offset = offset1 / 100;
            }
            //RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
            //              "Center = Center / (4*EW + 4*EH) / 2 = %d\n", offset);
          }

          temp = offset - timingCenter;
          if (temp < 0) {
            temp = -temp;
          }
          if (temp > maxOffsetFromTimingCenter) {
            if (offset < 0) {
              if (IsDdr5Present (Host, socket)) {
                RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, SubChannel, rank, strobe % (maxStrobe / 2), NO_BIT, "Truncated: %2d -> %2d\n", offset, timingCenter - maxOffsetFromTimingCenter);
              } else {
                RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT, "Truncated: %2d -> %2d\n", offset, timingCenter - maxOffsetFromTimingCenter);
              }
              offset = timingCenter - maxOffsetFromTimingCenter;
            } else {
              if (IsDdr5Present (Host, socket)) {
                RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, SubChannel, rank, strobe % (maxStrobe / 2), NO_BIT, "Truncated: %2d -> %2d\n", offset, timingCenter + maxOffsetFromTimingCenter);
              } else {
                RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT, "Truncated: %2d -> %2d\n", offset, timingCenter + maxOffsetFromTimingCenter);
              }
              offset = timingCenter + maxOffsetFromTimingCenter;
            }
          }


#ifdef DEBUG_CODE_BLOCK
          if (GetDebugLevel () & SDBG_MAX) {
            GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, group, GSM_READ_CSR | GSM_READ_ONLY, &oldDelay[strobe]);
          }
#endif //DEBUG_CODE_BLOCK
          // Apply new center
          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, group, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &offset);
#ifdef DEBUG_CODE_BLOCK
          if (GetDebugLevel () & SDBG_MAX) {
            GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, group, GSM_READ_ONLY, &delay[strobe]);
          }
#endif //DEBUG_CODE_BLOCK
        } // strobe loop
#ifdef DEBUG_CODE_BLOCK

        if (group == TxDqDelay) {
          TxDqWaAdv (socket, ch, NO_SUBCH, dimm, rank, TRUE);
        }

        if (IsDdr5Present (Host, socket)) {
          DisplayAdvancedCenteringResultsDdr5 (Host, socket, ch, SubChannel, dimm, rank, group, results, delay, oldDelay, maxVref, stepSize, numPoints);
        } else {
          DisplayAdvancedCenteringResults (Host, socket, ch, NO_SUBCH, dimm, rank, group, results, delay, oldDelay, maxVref, stepSize, numPoints);
        }
#endif  // DEBUG_CODE_BLOCK
        DetectLowMargin (Host, ch, dimm, rank, (*results), group, LM_ADVANCED);
      } // rank loop
    } // dimm loop
  } // ch loop
#ifndef SPR_PO_FLAG
  TxDqDqsDutyCycleCorrection (Host, socket);
#endif //SPR_PO_FLAG
#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    if (group == RxDqsDelay) {
      RcDebugPrint (SDBG_DEFAULT, "STOP_RX_DQS_ADV\n");
    } else if (group == RxDqsPDelay) {
      RcDebugPrint (SDBG_DEFAULT, "STOP_RX_DQSP_ADV\n");
    } else if (group == RxDqsNDelay) {
      RcDebugPrint (SDBG_DEFAULT, "STOP_RX_DQSN_ADV\n");
    } else if (group == TxDqDelay) {
      RcDebugPrint (SDBG_DEFAULT, "STOP_TX_DQ_ADV\n");
    }

    ReleasePrintControl ();
  }
#endif  // DEBUG_CODE_BLOCK
  RcFreePool (results);
} // AdvancedCentering

#define DELAY_TEST_OFFSET       10

#ifdef DEBUG_CODE_BLOCK
/*

  This function moves the current delay on Bit/Nibble by 'DELAY_TEST_OFFSET' ticks,
  report the margins caused by this movement and then restores the original delay

  @param[in]         Host        Pointer to structure
  @param[in]         Socket      Current socket under test (0-based)
  @param[in]         Channel     Current channel under test (0-based)
  @param[in]         Dimm        Current dimm under test (0-based)
  @param[in]         Rank        Current rank under test (0-based)
  @param[in]         Strobe      Current strobe under test (0-based)
  @param[in]         Bit         Current bit under test (0-based) or ALL_BITS
  @param[in]         Level       DDR Level, LRDIMM Host side or backside
  @param[in]         Group       Parameter to be margined
  @param[in]         ChannelMask Channels to be masked (Bitwise), 0: Run, 1: Skip (Masked)
  @param[in]         RankMask    Ranks to be masked (bitwise), 0: Run, 1: Skip (Masked)

*/
VOID
NibbleBitTest (
  IN        PSYSHOST    Host,
  IN        UINT8       Socket,
  IN        UINT8       Channel,
  IN        UINT8       Dimm,
  IN        UINT8       Rank,
  IN        UINT8       Strobe,
  IN        UINT8       Bit,
  IN        MRC_LT      Level,
  IN        MRC_GT      Group,
  IN        UINT8       ChannelMask,
  IN        UINT8       RankMask
  )
{
  UINT8   BitIndex;
  MRC_GT  MarginGroup;
  INT16   Offset[BITS_PER_NIBBLE];
  struct bitMask      Filter;
  struct bitMargin    ResultsBit;

  ZeroMem (&Offset, sizeof (Offset));

  ClearFilter (Host, Socket, &Filter);

  MarginGroup = Group;
  BitIndex    = ALL_BITS;

  RcDebugPrintWithDevice (
    SDBG_MARGINS, Socket, Channel, Dimm, NO_SUBCH, Rank, Strobe, Bit,
    "\n");

  if (Bit == ALL_BITS){
    Offset[0]   = DELAY_TEST_OFFSET;
    Offset[1]   = DELAY_TEST_OFFSET;
    Offset[2]   = DELAY_TEST_OFFSET;
    Offset[3]   = DELAY_TEST_OFFSET;

  } else {
    Offset[0]   = 0;
    Offset[1]   = 0;
    Offset[2]   = 0;
    Offset[3]   = 0;

    if (Bit >= BITS_PER_NIBBLE) {
      RcDebugPrintWithDevice (
        SDBG_MARGINS, Socket, Channel, Dimm, NO_SUBCH, Rank, Strobe, Bit,
        "Invalid Bit input parameter\n"
        );
      return;
    }
    Offset[Bit] = DELAY_TEST_OFFSET;

    //
    // TxDqBitDelay and RxDqsBitDelay GetSets function always
    // except to update all 4 bits within the nibble, but
    // RxVref only updates at a bit level and this is the
    // reason bit index 0 need to also be updated
    //
    if (Group == RxVref) {
      Offset[0] = DELAY_TEST_OFFSET;
      BitIndex = Bit;
    } else {
      BitIndex = ALL_BITS;
      if (Group == TxDqBitDelay) {
        MarginGroup = TxDqDelay;
      } else {
        MarginGroup = RxDqsDelay;
      }
    }
  }

  if (Group == RxVref) {
    DumpRxVrefBitRegs (Socket, Channel, Dimm, Rank, Strobe, Bit, Group, "RxVRef - Before");
  } else if (Group == TxDqDelay || Group == TxDqBitDelay) {
    DumpTxDelayBitRegs (Socket, Channel, Dimm, Rank, Strobe, Bit, Group, "TxDq - Before");
  } else {
    DumpRxDelayBitRegs (Socket, Channel, Dimm, Rank, Strobe, Bit, Group, "RxDqs - Before");
  }

  GetSetDataGroup (
    Host, Socket, Channel, NO_SUBCH, Dimm, Rank,
    Strobe, BitIndex, Level, Group,
    GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &Offset[0]
    );

  if (Group == RxVref) {
    DumpRxVrefBitRegs (Socket, Channel, Dimm, Rank, Strobe, Bit, Group, "RxVRef - After");
  } else if (Group == TxDqDelay || Group == TxDqBitDelay) {
    DumpTxDelayBitRegs (Socket, Channel, Dimm, Rank, Strobe, Bit, Group, "TxDq - After");
  } else {
    DumpRxDelayBitRegs (Socket, Channel, Dimm, Rank, Strobe, Bit, Group, "RxDqs - After");
  }

  GetMargins (
    Host, Socket, Level, MarginGroup, MODE_VIC_AGG | MODE_POS_ONLY, SCOPE_BIT,
    &Filter, (VOID *)&ResultsBit, 64, 0, ChannelMask, RankMask,
    WDB_BURST_LENGTH, RmtDqTest, NULL
    );

  Offset[0] = -Offset[0];
  Offset[1] = -Offset[1];
  Offset[2] = -Offset[2];
  Offset[3] = -Offset[3];

  GetSetDataGroup (
    Host, Socket, Channel, NO_SUBCH, Dimm, Rank,
    Strobe, BitIndex, Level, Group,
    GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &Offset[0]
    );

}

/*

  This function prepares the initial setup for the input signal

  @param[in]         Host        Pointer to structure
  @param[in]         Socket      Current socket under test (0-based)
  @param[in]         Channel     Current channel under test (0-based)
  @param[in]         Dimm        Current dimm under test (0-based)
  @param[in]         Rank        Current rank under test (0-based)
  @param[in]         Strobe      Current strobe under test (0-based)
  @param[in]         Bit         Only valid parameter is ALL_BITS (Nibble/Byte signals) or EACH_BIT (per bit signals)
  @param[in]         Level       DDR Level, LRDIMM Host side or backside
  @param[in]         Group       Parameter to be margined
  @param[in]         ChannelMask Channels to be masked (Bitwise), 0: Run, 1: Skip (Masked)
  @param[in]         StrName     String for the current signal getting inspect
  @param[in]         SkipOriginalMarginCheck  TRUE:   will skip initial margin check (Must be used for xxBitDelay)
                                              FALSE:  will execute initial margin check

*/
VOID
InspectSwizzleForSignal (
  IN        PSYSHOST    Host,
  IN        UINT8       Socket,
  IN        UINT8       Channel,
  IN        UINT8       Dimm,
  IN        UINT8       Rank,
  IN        UINT8       Bit,
  IN        MRC_LT      Level,
  IN        MRC_GT      Group,
  IN        UINT8       ChannelMask,
  IN        UINT8       RankMask,
  IN        CHAR8       *StrName,
  IN        BOOLEAN     SkipOriginalMarginCheck
  )
{
  UINT8     Strobe;
  UINT8     BitIndex;
  UINT8     BitEnd;
  UINT8     BitLevel;
  UINT8     MSVx4;
  struct bitMargin    ResultsBit;
  struct bitMask      Filter;

  ClearFilter (Host, Socket, &Filter);

  MSVx4       = GetMaxStrobeValid (Host);

  if (Bit == ALL_BITS) {
    BitEnd    = 1;
  } else {
    BitEnd    = BITS_PER_NIBBLE;
  }

  if (!SkipOriginalMarginCheck) {
    RcDebugPrintWithDevice (
      SDBG_MARGINS, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "Original Margin for %a\n", StrName
      );

    GetMargins (
      Host, Socket, Level, Group, MODE_VIC_AGG | MODE_POS_ONLY, SCOPE_BIT,
      &Filter, (VOID *)&ResultsBit, 64, 0, ChannelMask, RankMask,
      WDB_BURST_LENGTH, RmtDqTest, NULL
      );
  }

  if (Bit == ALL_BITS) {
    RcDebugPrintWithDevice (
      SDBG_MARGINS, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "Nibble Margining for %a\n", StrName
      );
  } else {
    RcDebugPrintWithDevice (
      SDBG_MARGINS, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "Bit Margining for %a\n", StrName
      );
  }

  for (Strobe = 0; Strobe < MSVx4; Strobe++) {
    for (BitIndex = 0; BitIndex < BitEnd; BitIndex++) {
      if (Bit == ALL_BITS) {
        BitLevel = ALL_BITS;
      } else {
        BitLevel = BitIndex;
      }
      NibbleBitTest (Host, Socket, Channel, Dimm, Rank, Strobe, BitLevel, Level, Group, ChannelMask, RankMask);
    }
  }
}
#endif // #ifdef DEBUG_CODE_BLOCK

/*

 This function is used to verify if the bit swizzling used for Rx and Tx is correct.
 This function does not do the check automatically, but it shows data margin on per bits
 basis in the serial to inspect if the swizzling look correct.

 @param[in]         Host        Pointer to structure

 @retval            SUCCESS     Always return success execution

*/
UINT32
BitSwizzlingInspection (
  IN        PSYSHOST    Host
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8   Socket;
  UINT8   Channel;
  UINT8   ChannelMask;
  UINT8   Dimm;
  UINT8   Rank;
  UINT8   LogicalRank;
  UINT8   RankMask;
  UINT32  BackupMsg;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  UINT8  MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Socket    = Host->var.mem.currentSocket;
  BackupMsg = GetDebugLevel ();

  if (PcdGetBool (PcdMrcDebugSwizzleInspection) == FALSE) {
    return SUCCESS;
  }

  if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
    return SUCCESS;
  }

  SetDebugLevel (SDBG_FORCE_ALL | SDBG_MARGINS | SDBG_DEFAULT);

  ChannelNvList = GetChannelNvList (Host, Socket);
  Channel = 0;
  Dimm    = 0;
  Rank    = 0;

  //
  // Determine the first usable channel, dimm, rank
  // Everything else should be masked out
  //
  RankMask    = 0xFF;
  ChannelMask = 0xFF;
  for (Channel = 0; Channel < MaxChDdr; Channel++) {

    if ((*ChannelNvList)[Channel].enabled == 0) {
     continue;
    }

    ChannelMask &= ~(1 << Channel);

    DimmNvList = GetDimmNvList (Host, Socket, Channel);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
         continue;
      }

      for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {

        if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        LogicalRank = GetLogicalRank (Host, Socket, Channel, Dimm, Rank);

        RankMask &= ~(1 << LogicalRank);

        RcDebugPrintWithDevice (
          SDBG_MARGINS, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "Logical Rank = %d ChannelMask = 0x%02x RankMask = 0x%02x\n",
          LogicalRank,
          ChannelMask,
          RankMask
          );

        break;

      } // Rank loop

      break;

    } // Dimm loop

    break;

  } // Channel loop

  InspectSwizzleForSignal (Host, Socket, Channel, Dimm, Rank, ALL_BITS, DdrLevel, RxDqsDelay, ChannelMask, RankMask, "RxDqsDelay", FALSE);
  InspectSwizzleForSignal (Host, Socket, Channel, Dimm, Rank, EACH_BIT, DdrLevel, RxDqsPBitDelay, ChannelMask, RankMask, "RxDqsPBitDelay", TRUE);
  InspectSwizzleForSignal (Host, Socket, Channel, Dimm, Rank, EACH_BIT, DdrLevel, RxDqsNBitDelay, ChannelMask, RankMask, "RxDqsNBitDelay", TRUE);

  InspectSwizzleForSignal (Host, Socket, Channel, Dimm, Rank, ALL_BITS, DdrLevel, TxDqDelay, ChannelMask, RankMask, "TxDqDelay", FALSE);
  InspectSwizzleForSignal (Host, Socket, Channel, Dimm, Rank, EACH_BIT, DdrLevel, TxDqBitDelay, ChannelMask, RankMask, "TxDqBitDelay", TRUE);

  InspectSwizzleForSignal (Host, Socket, Channel, Dimm, Rank, EACH_BIT, DdrLevel, RxVref, ChannelMask, RankMask, "RxVref", FALSE);

  SetDebugLevel (BackupMsg);
#endif // #ifdef DEBUG_CODE_BLOCK
  return SUCCESS;
}


UINT32
RdAdvancedCentering (
  PSYSHOST  Host
  )
/*++

  Re-centers read dq timing using a more robust test

  @param Host - Pointer to sysHost

  @retval N/A

--*/
{
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  if (!IsMemFlowEnabled (ReadAdvancedCenteringTraining)) {
    return SUCCESS;
  }

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_TRAIN_RD_DQS, Host->var.mem.currentSocket));
  if (Setup->mem.optionsExt & DUTY_CYCLE_EN) {
    AdvancedCentering (Host, Host->var.mem.currentSocket, RxDqsNDelay);
    AdvancedCentering (Host, Host->var.mem.currentSocket, RxDqsPDelay);
  } else {
    AdvancedCentering (Host, Host->var.mem.currentSocket, RxDqsDelay);
  }

  return SUCCESS;
} // RdAdvancedCentering

UINT32
WrAdvancedCentering (
  PSYSHOST  Host
  )
/*++

  Re-centers tx dq timing using a more robust test

  @param Host - Pointer to sysHost

  @retval SUCCESS

--*/
{
  if (!IsMemFlowEnabled (WriteAdvancedCenteringTraining)) {
    return SUCCESS;
  }

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_TRAIN_WR_DQS, Host->var.mem.currentSocket));
  AdvancedCentering (Host, Host->var.mem.currentSocket, TxDqDelay);

  return SUCCESS;
} // WrAdvancedCentering

UINT8
VrefQuick (
  PSYSHOST  Host,
  UINT8     socket,
  MRC_GT    group,
  INT16     *low,
  INT16     *high
  )
/*++

  Quick centering of Vref

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param group   - Parameter to center
                   RxDqsDelay   0: Center Rx DqDqs
                   TxDqDelay    1: Center Tx DqDqs
  @param low     - Pointer to worst low
  @param high    - Pointer to worst high

  @retval UINT8   - Vref eye height

--*/
{
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT16              mode;
  UINT32              patternLength;
  INT16               compLow = 0;
  INT16               compHigh = 0;
  INT16               worstLow = 0;
  INT16               worstHigh = 0;
  //INT16               offset[MAX_RANK_CH];
  struct bitMask      filter;
  struct rankMargin   resultsRank;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  UINT8               rankPerChannel;
  UINT8               MaxChDdr;
#ifdef DDR5_SUPPORT
  UINT8               BitIndex;
  UINT8               MaxBitIndex;

  if (GetCurrentTestType (socket) == ReadDfeTest) {
    MaxBitIndex = BITS_PER_NIBBLE;
  } else {
    MaxBitIndex = 1;
  }
#endif // #ifdef DDR5_SUPPORT
  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);

  patternLength = 64;
  mode = MODE_VIC_AGG;

  // Setup bitMask
  for (ch = 0; ch < MaxChDdr; ch++) {
    for (rankPerChannel = 0; rankPerChannel < MAX_RANK_CH; rankPerChannel++) {
      resultsRank.rank[ch][rankPerChannel].n = 0;
      resultsRank.rank[ch][rankPerChannel].p = 0;
      //offset[rankPerChannel] = 0;
    }
  } // ch loop

  //
  // Clear the filter bits to enable error checking on every bit
  //
  ClearFilter (Host, socket, &filter);

  if (IsDdr5Present (Host, socket) && (IsBrsPresent (Host, socket) == FALSE)) {
#ifdef DDR5_SUPPORT

    for (BitIndex = 0; BitIndex < MaxBitIndex; BitIndex++) {
      Host->var.mem.socket[socket].TrainingStatus.Bits.BitIndex = BitIndex;
      UpdateDramSeed (socket);
      GetDfeDataMargins (socket, DdrLevel, group, NULL, NULL, &resultsRank);
    }

#endif //DDR5_SUPPOER
  } else {
    GetMargins (Host, socket, DdrLevel, group, mode, SCOPE_RANK, &filter, (VOID *)&resultsRank, patternLength, 0, 0, 0,
      WDB_BURST_LENGTH, RmtDqTest, NULL);
  }
  //if (group == TxVref) {
  //  compLow = -0xFF;
  //  compHigh = 0xFF;
  // }

  worstLow = -0xFF;
  worstHigh = 0xFF;

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    //if (group == RxVref) {
    compLow = -0xFF;
    compHigh = 0xFF;
    //}
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      rankList = GetRankNvList (Host, socket, ch, dimm);
      for (rank = 0; rank < (*channelNvList)[ch].dimmList[dimm].numRanks; rank++) {
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        if (resultsRank.rank[ch][ (*rankList)[rank].rankIndex].n > resultsRank.rank[ch][ (*rankList)[rank].rankIndex].p) {
          resultsRank.rank[ch][ (*rankList)[rank].rankIndex].n = 0;
          resultsRank.rank[ch][ (*rankList)[rank].rankIndex].p = 0;
        }
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                                "High = %d - Low = %d\n",
                                resultsRank.rank[ch][(*rankList)[rank].rankIndex].p, resultsRank.rank[ch][(*rankList)[rank].rankIndex].n);

        if (compLow < resultsRank.rank[ch][ (*rankList)[rank].rankIndex].n) {
          compLow = resultsRank.rank[ch][ (*rankList)[rank].rankIndex].n;
        }
        if (compHigh > resultsRank.rank[ch][ (*rankList)[rank].rankIndex].p) {
          compHigh = resultsRank.rank[ch][ (*rankList)[rank].rankIndex].p;
        }
        //offset[(*rankList)[rank].rankIndex] = (resultsRank.rank[ch][(*rankList)[rank].rankIndex].n +
        //      resultsRank.rank[ch][(*rankList)[rank].rankIndex].p) / 2;
      } // rank loop
    } // dimm loop

    if (worstLow < compLow) {
      worstLow = compLow;
    }
    if (worstHigh > compHigh) {
      worstHigh = compHigh;
    }
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "Composite High = %d - Composite Low = %d\n", compHigh, compLow);
    //}
  } // ch loop

  if (group == TxVref) {
    UpdateSafeTxVref (Host, socket, DdrLevel);
  }
  //
  // Reset the IO
  //
  IO_Reset (Host, socket);

  *low = worstLow;
  *high = worstHigh;
  if (worstLow > worstHigh) {
    return 0;
  } else {
    return (UINT8) (worstHigh - worstLow);
  }
} // VrefQuick

/*++

  Early Vref centering

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param group   - Parameter to center
                   RxVref    0: Center Rx Vref
                   TxVref    1: Center Tx Vref

  @retval SUCCESS

--*/
UINT32
EarlyVrefCentering (
  PSYSHOST  Host,
  UINT8     socket,
  MRC_GT    group
  )
{
  UINT8               ch;
  UINT16              eyesize = 0;
  UINT8               maxEyeHeight = 0;
  UINT8               MaxStrobe;
  UINT8               dimm;
  UINT8               rank;
  UINT8               rank2;
  UINT8               dimm2 = 0;
  UINT8               strobe;
  UINT8               done = 0;
  UINT8               chDoneMask = 0;
  UINT8               dimmLoopBreak = 0;
  UINT8               dimmDone[MAX_CH][MAX_DIMM];
  UINT8               rankDone[MAX_CH][MAX_DIMM][MAX_RANK_DIMM];
  UINT8               counter[MAX_CH][MAX_DIMM][MAX_RANK_DIMM];
  INT16               offset[MAX_CH][MAX_DIMM][MAX_RANK_DIMM];
  INT16               inital[MAX_CH][MAX_DIMM][MAX_RANK_DIMM];
  INT16               setting[MAX_CH][MAX_DIMM][MAX_RANK_DIMM];
  INT16               tOffset;
  INT16               counterSign = 1;
  INT16               lowMargin;
  INT16               highMargin;
  UINT32              status;
  MRC_GT              tGroup = 0;
  struct bitMask      filter;
  struct rankMargin   rankResults;
  struct strobeMargin results;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;
  channelNvList = GetChannelNvList (Host, socket);
  UINT8 MaxChDdr;

  if (Host->nvram.mem.socket[socket].maxDimmPop == 0) {
    return SUCCESS;
  }

  MaxChDdr = GetMaxChDdr ();
#ifdef DEBUG_CODE_BLOCK
  RcDebugPrint (SDBG_MEM_TRAIN,
               "\nPrevious Settings ");

  if (group == RxVref) {
    DisplayResults (Host, socket, RxVref);
    DisplayResults (Host, socket, RxDqsDelay);
  } else if (group == TxVref) {
    DisplayResults (Host, socket, TxVref);
    DisplayResults (Host, socket, TxDqDelay);
    DisplayResults (Host, socket, TxDqBitDelay);
    DisplayResults (Host, socket, TxDqsDelay);
  } else {
    // Error
  }
#endif //DEBUG_CODE_BLOCK

  if (group == RxVref) {
    tGroup = RxDqsDelay;
  } else if (group == TxVref) {
    tGroup = TxDqDelay;
  } else {
    // Error
  }

  ClearFilter (Host, socket, &filter);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    dimmNvList = GetDimmNvList (Host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      dimmDone[ch][dimm] = 0;
      rankList = GetRankNvList (Host, socket, ch, dimm);
      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        counter[ch][dimm][rank] = 0;
        offset[ch][dimm][rank] = 0;
        setting[ch][dimm][rank] = 0;
        GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, ALL_BITS, DdrLevel, group, GSM_READ_ONLY, &inital[ch][dimm][rank]);
        rankDone[ch][dimm][rank] = 0;
      } // rank loop
    } // dimm loop
  } // ch loop

  RcDebugPrint (SDBG_MAX,
                  "\n");


  //
  // Get per rank eye width
  // W/A for unusually low or high margins s4986776. Try multiple vref offsets until a timing eye is found.
  //
  while (!done) {
    GetMargins (Host, socket, DdrLevel, tGroup, MODE_VIC_AGG, SCOPE_STROBE, &filter, &results, 64, 0, 0, 0,
      WDB_BURST_LENGTH, RmtDqTest, NULL);

    done = 1;
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Cycling through ranks to check eye width for 2DPC margining W/A!!\n");

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      if (chDoneMask & (1 << ch)) {
        continue;
      }
      chDoneMask |= (1 << ch);

      maxEyeHeight = 0;

      dimmNvList = GetDimmNvList (Host, socket, ch);
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if (dimmDone[ch][dimm]) {
          continue;
        }
        if (group == TxVref) {
          dimmDone[ch][dimm] = 1;
          maxEyeHeight = 0;
        }
        rankList = GetRankNvList (Host, socket, ch, dimm);
        for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          //
          // Skip if this rank is done
          //
          if (rankDone[ch][dimm][rank]) {
            continue;
          }

          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                        "Checking for eye widths!! Current VREF offset is %d\n", offset[ch][dimm][rank]);
          status = SUCCESS;
          if (group == TxVref) {
            if ((*dimmNvList)[dimm].x4Present == 0) {
              MaxStrobe = MSVx8;
            } else {
              MaxStrobe = MSVx4;
            }
          } else { // RxVref
            MaxStrobe = MSVx4;
          }

          for (strobe = 0; strobe < MaxStrobe; strobe++) {
            if (IsStrobeNotValid (Host, strobe)) {
              continue;
            }
            if ((*rankList)[rank].faultyParts[strobe] != 0) {
              RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                            "faultyParts set\n");
              continue;
            }

            //
            // For x8 device, check the result from paired nibble as well since iMC side always use nibble
            //
            if ((*dimmNvList)[dimm].x4Present == 0) {
              if ((*rankList)[rank].faultyParts[strobe + MSVx8] != 0) {
                RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                            "faultyParts set\n");
                continue;
              }
            }

            eyesize = (results.strobe[ch][ (*rankList)[rank].rankIndex][strobe].p - results.strobe[ch][ (*rankList)[rank].rankIndex][strobe].n);

            if (eyesize < MIN_EARLYVREF_EYE) {
              status = FAILURE;
              break;
            }
          } //strobe loop

          if (status == FAILURE) {
            if (eyesize > maxEyeHeight) {
              maxEyeHeight = (UINT8) eyesize;
            }
            chDoneMask &= ~(1 << ch);
            dimmDone[ch][dimm] = 0;
            if (counter[ch][dimm][rank] & BIT0) {
              counterSign = -1;
            } else {
              counterSign = 1;
            }
            offset[ch][dimm][rank] = offset[ch][dimm][rank] + counterSign * MEM_CHIP_POLICY_VALUE (Host, earlyVrefStepSize) * (counter[ch][dimm][rank] + 1);

            setting[ch][dimm][rank] = inital[ch][dimm][rank] + offset[ch][dimm][rank];
            GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, ALL_BITS, DdrLevel, group, GSM_UPDATE_CACHE, &setting[ch][dimm][rank]);

            counter[ch][dimm][rank]++;
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                            "Eye width %2d detected!! Setting VREF offset to %d using counter %d...\n",
                          eyesize, offset[ch][dimm][rank],counter[ch][dimm][rank]);
            done = 0;
            if (counter[ch][dimm][rank] > MEM_CHIP_POLICY_VALUE (Host, maxVrefSettings)) {
              if (group == TxVref) {
                RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                            "Unable to find the TxVref for the eye!\n");
              } else if (group == RxVref) {
                RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                            "Unable to find the RxVref for the eye!\n");
              }
              //
              // Map out channel
              //
              DisableChannelSw (Host, socket, ch);
              //
              // Add this error to the warning log for both correctable and uncorrectable errors.
              //
              EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_VREF_TRAINING, socket, ch, dimm, rank, EwlSeverityWarning, NO_STROBE, NO_BIT, group, DdrLevel, maxEyeHeight);
            }
            // If this is Tx (and not LRDIMM), then continue to the next rank. If Rx, then we need to set the vref offset
            // and break to the channel loop. Otherwise, we will get multiple per-channel RxVref offsets
            // if multiple ranks per channel are failing RxVref.
            if (group == RxVref) {
              // Offsets and counters are shared with the entire channel for Rx
              for (dimm2 = 0; dimm2 < MAX_DIMM; dimm2++) {
                rankList = GetRankNvList (Host, socket, ch, dimm2);
                for (rank2 = 0; rank2 < (*dimmNvList)[dimm2].numRanks; rank2++) {

                  // Do not test if this rank is disabled
                  if (CheckRank (Host, socket, ch, dimm2, rank2, DONT_CHECK_MAPOUT)) {
                    continue;
                  }

                  offset[ch][dimm2][rank2] = offset[ch][dimm][rank];
                  counter[ch][dimm2][rank2] = counter[ch][dimm][rank];
                } // rank2 loop
              } // dimm2 loop
              dimmLoopBreak = 1;
              break; // breaks out of rank loop
            } else if (group == TxVref && IsLrdimmPresent (socket, ch, dimm)) {
              // If Tx LRDIMM, then we need to set the vref offset
              // and break to the dimm loop.
              // Offsets and counters are shared with the entire DIMM
              rankList = GetRankNvList (Host, socket, ch, dimm);
              for (rank2 = 0; rank2 < (*dimmNvList)[dimm].numRanks; rank2++) {
                if (CheckRank (Host, socket, ch, dimm, rank2, DONT_CHECK_MAPOUT)) {
                  continue;
                }

                // Do not test if this rank is mapped out
                if (rank2 == rank) {
                  continue;
                } // Don't re-update what was just done

                offset[ch][dimm][rank2] = offset[ch][dimm][rank];
                counter[ch][dimm][rank2] = counter[ch][dimm][rank];
                setting[ch][dimm][rank2] = inital[ch][dimm][rank2] + offset[ch][dimm][rank2];
                GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank2, ALL_STROBES, ALL_BITS, DdrLevel, group, GSM_UPDATE_CACHE, &setting[ch][dimm][rank2]); // Set the cache values for all ranks in the LRDIMM
              } // rank2 loop
              break;
            }
          } else {

            //
            // Tx Vref is centering per rank
            //
            if (group == TxVref) {
              if (IsLrdimmPresent (socket, ch, dimm)) {
                rankDone[ch][dimm][rank] = 0;
              } else {
                rankDone[ch][dimm][rank] = 1;
              }
            }
          }
        } // rank loop
        if (dimmLoopBreak == 1)  { // Allow Rx to break the DIMM/rank loops
          dimmLoopBreak = 0;
          break;
        }
      } // dimm loop

    } // ch loop
  } // while loop

  //
  // Recenter timing based on the new Vref offsets
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      rankList = GetRankNvList (Host, socket, ch, dimm);
      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        if (group == TxVref) {
          if ((*dimmNvList)[dimm].x4Present == 0) {
            MaxStrobe = MSVx8;
          } else {
            MaxStrobe = MSVx4;
          }
        } else { // RxVref
          MaxStrobe = MSVx4;
        }

        for (strobe = 0; strobe < MaxStrobe; strobe++) {
          if (IsStrobeNotValid (Host, strobe)) {
            continue;
          }
          if ((*rankList)[rank].faultyParts[strobe] != 0) {
            continue;
          }

          //
          // For x8 device, check the result from paired nibble as well since iMC side always use nibble
          //
          if ((*dimmNvList)[dimm].x4Present == 0) {
            if ((*rankList)[rank].faultyParts[strobe + MSVx8] != 0) {
              continue;
            }
          }

          tOffset = (results.strobe[ch][ (*rankList)[rank].rankIndex][strobe].p + results.strobe[ch][ (*rankList)[rank].rankIndex][strobe].n) / 2;
          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, ALL_BITS, DdrLevel, tGroup, GSM_UPDATE_CACHE + GSM_WRITE_OFFSET,
            &tOffset);
        } //strobe loop
      } // rank loop
    } // dimbecm loop
  } // ch loop
  //
  // Update starting Tx/Rx timing offsets for later use by GetMargins
  //
  UpdateStartingOffset (Host, socket, tGroup, DdrLevel, SCOPE_STROBE, (VOID *)&results);

  //
  // Recenter Vref at the current timing
  //
  GetMargins (Host, socket, DdrLevel, group, MODE_VIC_AGG, SCOPE_RANK, &filter, &rankResults, 64, 0, 0, 0,
    WDB_BURST_LENGTH, RmtDqTest, NULL);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    lowMargin = -0x7F;
    highMargin = 0x7F;

    dimmNvList = GetDimmNvList (Host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      rankList = GetRankNvList (Host, socket, ch, dimm);
      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (IsLrdimmPresent (socket, ch, dimm)&& (rank > 0) && (group == TxVref)) {
          continue;
        }

        if (group == TxVref) {
          tOffset = (rankResults.rank[ch][ (*rankList)[rank].rankIndex].p + rankResults.rank[ch][ (*rankList)[rank].rankIndex].n) / 2;
          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, ALL_BITS, DdrLevel, group, GSM_UPDATE_CACHE + GSM_WRITE_OFFSET,
            &tOffset);

          TxDqWaAdv (socket, ch, NO_SUBCH, dimm, rank, TRUE);

        } else { // calculate composite eye for RX VREF
          if (lowMargin < rankResults.rank[ch][ (*rankList)[rank].rankIndex].n) {
            lowMargin = rankResults.rank[ch][ (*rankList)[rank].rankIndex].n;
          }

          if (highMargin > rankResults.rank[ch][ (*rankList)[rank].rankIndex].p) {
            highMargin = rankResults.rank[ch][ (*rankList)[rank].rankIndex].p;
          }
        }
      } // rank loop
    } // dimm loop
    if (group == RxVref) {
      tOffset = (highMargin + lowMargin) / 2;
      GetSetDataGroup (Host, socket, ch, NO_SUBCH, 0, 0, ALL_STROBES, ALL_BITS, DdrLevel, group, GSM_UPDATE_CACHE + GSM_WRITE_OFFSET,
        &tOffset);
    }
  } // ch loop

  //
  // Update starting Tx/Rx Vref offsets for later use by GetMargins
  //
  UpdateStartingOffset (Host, socket, group, DdrLevel, SCOPE_RANK, (VOID *)&rankResults);
  if (group == TxVref) {
    UpdateSafeTxVref (Host, socket, DdrLevel);
  }
#ifdef DEBUG_CODE_BLOCK
  RcDebugPrint (SDBG_MEM_TRAIN,
                         "\nNew Settings");

  if (group == RxVref) {
    DisplayResults (Host, socket, RxVref);
    DisplayResults (Host, socket, RxDqsDelay);
  } else if (group == TxVref) {
    DisplayResults (Host, socket, TxVref);
    DisplayResults (Host, socket, TxDqDelay);
    DisplayResults (Host, socket, TxDqBitDelay);
    DisplayResults (Host, socket, TxDqsDelay);
  } else {
    RcDebugPrint (SDBG_MAX,
                         "\nERROR - Invalid group\n ");
  }
#endif //DEBUG_CODE_BLOCK
  return SUCCESS;
} // EarlyVrefCentering

UINT32
RdEarlyVrefCentering (
  PSYSHOST  Host
  )
/*++

  Early Vref centering

  @param Host    - Pointer to sysHost

  @retval SUCCESS

--*/
{
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  if (!IsMemFlowEnabled (EarlyReadVrefCenteringTraining)) {
    return SUCCESS;
  }
  //
  // Perform any silicon specific initialization required before executing Read Early Vref Centering
  //
  ChipInitBeforeRdEarlyVrefCentering ();
  return EarlyVrefCentering (Host, Host->var.mem.currentSocket, RxVref);
} // RcEarlyVrefCentering

UINT32
WrEarlyVrefCentering (
  PSYSHOST  Host
  )
/*++

  Early Vref centering

  @param Host    - Pointer to sysHost

  @retval SUCCESS

--*/
{
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  if (!IsMemFlowEnabled (EarlyWriteVrefCenteringTraining)) {
    return SUCCESS;
  }
  ChipInitBeforeWrEarlyVrefCentering ();
  return EarlyVrefCentering (Host, Host->var.mem.currentSocket, TxVref);
} // WrEarlyVrefCentering

UINT32
CmdVrefCentering (
  PSYSHOST  Host
  )
/*++

  Re-centers using a more robust test

  @param Host    - Pointer to sysHost

  @retval SUCCESS

--*/
{
  UINT8 socket;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();
  socket = Host->var.mem.currentSocket;

  if (!IsMemFlowEnabled (CmdVrefCenteringTraining)) {
    return SUCCESS;
  }

  SetNormalCMDTiming (Host, Host->var.mem.currentSocket);

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_CMD_VREF_CENTERING, Host->var.mem.currentSocket));
  return CmdVrefQuick (Host, socket, CmdVref);

} // CMDVrefCentering

UINT32
RdVrefCentering (
  PSYSHOST  Host
  )
/*++

  Re-centers using a more robust test

  @param Host    - Pointer to sysHost

  @retval N/A

--*/
{
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  if (!IsMemFlowEnabled (ReadVrefTraining)) {
    return SUCCESS;
  }

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_TRAIN_RD_VREF, Host->var.mem.currentSocket));
  return VrefCentering (Host, Host->var.mem.currentSocket, MEM_CHIP_POLICY_VALUE (Host, RxVrefTrainingMode), RxVref);
} // RdVrefCentering

UINT32
WrVrefCentering (
  PSYSHOST  Host
  )
/*++

  Re-centers using a more robust test

  @param Host    - Pointer to sysHost

  @retval SUCCESS

--*/
{
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  if (!IsMemFlowEnabled (WriteVrefTraining)) {
    return SUCCESS;
  }

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_TRAIN_WR_VREF, Host->var.mem.currentSocket));
  return VrefCentering (Host, Host->var.mem.currentSocket, MEM_CHIP_POLICY_VALUE (Host, TxVrefTrainingMode), TxVref);
} // WrVrefCentering

#ifdef LRDIMM_SUPPORT
UINT32
WrLRDIMMVrefCentering (
  PSYSHOST  Host
  )
/*++

  Re-centers using a more robust test

  @param Host    - Pointer to sysHost

  @retval SUCCESS

--*/
{

  if (!IsMemFlowEnabled (LrdimmWriteVrefCenteringTraining)) {
    return SUCCESS;
  }
  return  LRDIMMAdvCentering (Host, Host->var.mem.currentSocket, TxVref, LrbufLevel, 1);

} // WrVrefCentering

UINT32
RdLRDIMMVrefCentering (
  PSYSHOST  Host
  )
/*++

  Re-centers using a more robust test

  @param Host    - Pointer to sysHost

  @retval SUCCESS

--*/
{
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  if (!IsMemFlowEnabled (LrdimmReadVrefCenteringTraining)) {
    return SUCCESS;
  }

  return  LRDIMMAdvCentering (Host, Host->var.mem.currentSocket, RxVref, LrbufLevel, 1);

} // RdVrefCentering

UINT32
TxLRDIMMDqCentering (
  PSYSHOST  Host
  )
/*++

  Re-centers using a more robust test

  @param Host    - Pointer to sysHost

  @retval SUCCESS

--*/
{
  SYS_SETUP *Setup;
  UINT8     Socket = Host->var.mem.currentSocket;

  Setup = GetSysSetupPointer ();

  if ((!IsMemFlowEnabled (LrdimmBacksideDqTraining)) ||
      (Host->nvram.mem.socket[Socket].ddrFreq < DDR_2933)) {
    return SUCCESS;
  }

  return  LRDIMMAdvCentering (Host, Socket, TxDqDelay, LrbufLevel, 0);

} // TxDqCentering

UINT32
RxLRDIMMDqCentering (
  PSYSHOST  Host
  )
/*++

  Re-centers using a more robust test

  @param Host    - Pointer to sysHost

  @retval SUCCESS

--*/
{
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  if (!IsMemFlowEnabled (LrdimmBacksideDqTraining)) {
    return SUCCESS;
  }

  return  LRDIMMAdvCentering (Host, Host->var.mem.currentSocket, RxDqsDelay, LrbufLevel, 0);

} // RxDqCentering

UINT32
LRDIMMAdvCentering (
  PSYSHOST  Host,
  UINT8     socket,
  MRC_GT    group,
  MRC_LT    level,
  UINT8     perRankCentering
  )
/*++

  VRef Centering for LRDIMM (DDR4)

  @param Host                - Pointer to sysHost
  @param socket              - Processor socket to check
  @param group               - Parameter to be margined
  @param level               - DDR or LRDIMM Host side, LRDIMM Backside, etc...
  @param perRankCentering    - 0 = false, 1 = true (if false, PDA training is done)

  @retval SUCCESS

--*/
{
#ifndef DDR5_SUPPORT
  UINT8               ch;
  UINT8               MaxSubRankCh = 0;
  UINT8               dimm;
  UINT8               rank;
  UINT8               subRank;
  UINT8               maxSubRank[MAX_CH];
  UINT8               strobe;
  UINT8               maxStrobe;
  INT16               vlo;
  INT16               vhi;
  INT16               vlo_rank;
  INT16               vhi_rank;
  INT16               vlo_strobe[MAX_STROBE];
  INT16               vhi_strobe[MAX_STROBE];
  UINT16              mode;
  UINT32              patternLength = 1;
  INT16               offset = 0;
  struct bitMask      filter;
  struct rankMargin   marginRank[MAX_SUBRANK];
  struct strobeMargin marginStrobe[MAX_SUBRANK];
  struct channelNvram (*channelNvList)[MAX_CH];
  struct ddrChannel   (*channelList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;
  SYS_SETUP           *Setup;
  UINT8               MaxChDdr;
  UINT8               SwizzleStrobe;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  ZeroMem (maxSubRank, sizeof (maxSubRank));

  //
  // Return under any of these conditions.
  //
  if ((Host->nvram.mem.socket[socket].enabled == 0) ||
      (Host->nvram.mem.socket[socket].maxDimmPop == 0) ||
      (Host->nvram.mem.socket[Host->var.mem.currentSocket].lrDimmPresent == 0)) {
    return SUCCESS;
  }

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "Starting LRDIMM backside Training. \n");

  channelNvList = GetChannelNvList (Host, socket);
  channelList = &Host->var.mem.socket[socket].channelList;

  // Clearout margin data structure
  for (ch = 0; ch < MaxChDdr; ch++) {
    maxSubRank[ch] = 1;
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      rankList = GetRankNvList (Host, socket, ch, dimm);
      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
        for (subRank = 0; subRank < maxSubRank[ch]; subRank++) {
          for (strobe = 0; strobe < MSVx4; strobe++) {
            marginStrobe[subRank].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n = 0;
            marginStrobe[subRank].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p = 0;
          } // strobe
          marginRank[subRank].rank[ch][rank].n = 0;
          marginRank[subRank].rank[ch][rank].p = 0;
        } // subRank
      } // rank loop
    } // dimm loop
  } // ch loop

  //
  // Clear the filter bits to enable error checking on every bit
  //
  ClearFilter (Host, socket, &filter);

  // Get pattern length
  mode = MODE_VIC_AGG;
  if (group == RxVref) {
    patternLength = TRAIN_RD_DQS_PL;
    maxStrobe = MSVx8;   // Buffers are x8 level
  } else if (group == RxDqsDelay) {
    patternLength = TRAIN_RD_DQS_DL;
    maxStrobe = MSVx4;     // ALL LRDIMM are x4 DRAMS
  } else {
    patternLength = TRAIN_WR_DQS_PL;
    maxStrobe = MSVx4;     // ALL LRDIMM are x4 DRAMS
  }

  for (ch = 1; ch < MaxChDdr; ch++) {
    if (maxSubRank[ch] > maxSubRank[MaxSubRankCh]) {
      MaxSubRankCh = ch;
    }
  }

  for (subRank = 0; subRank < maxSubRank[MaxSubRankCh]; subRank++) {
    Host->var.mem.currentSubRank = subRank;

    if ((perRankCentering == 1) || ((group == TxVref) && ((Setup->mem.optionsExt & PDA_EN) == 0))) { // Per rank
      GetMargins (Host, socket, level, group, mode, SCOPE_RANK, &filter, &marginRank[subRank], patternLength, 0, 0, 0, WDB_BURST_LENGTH, RmtDqTest, NULL);
    } else { // Per strobe
      GetMargins (Host, socket, level, group, mode, SCOPE_STROBE, &filter, &marginStrobe[subRank], patternLength, 0, 0, 0, WDB_BURST_LENGTH, RmtDqTest, NULL);
    }
  } // subRank loop
  Host->var.mem.currentSubRank = 0;

  if (group == RxVref) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "START_BACKSIDE_RX_VREF_CENTER\n");
  } else if (group == TxVref) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "START_BACKSIDE_TX_VREF_CENTER\n");
  } else if (group == TxDqDelay) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "START_BACKSIDE_TX_DQ_CENTER\n");
  } else if (group == RxDqsDelay) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "START_BACKSIDE_RX_DQ_CENTER\n");
  }

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    dimmNvList = GetDimmNvList (Host, socket, ch);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if (((*dimmNvList)[dimm].dimmPresent == 0)) {
        continue;
      }
      if (IsLrdimmPresent (socket, ch, dimm) == 0) {
        continue;
      }
      rankList = GetRankNvList (Host, socket, ch, dimm);

      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        for (subRank = 0; subRank < maxSubRank[ch]; subRank++) {
          if ((perRankCentering == 1) || ((group == TxVref) && ((Setup->mem.optionsExt & PDA_EN) == 0))) { // Per rank
            if (marginRank[subRank].rank[ch][ (*rankList)[rank].rankIndex].n > marginRank[subRank].rank[ch][ (*rankList)[rank].rankIndex].p) {
              marginRank[subRank].rank[ch][ (*rankList)[rank].rankIndex].p = 0;
              marginRank[subRank].rank[ch][ (*rankList)[rank].rankIndex].n = 0;
            }
          } else { // Per strobe
            for (strobe = 0; strobe < maxStrobe; strobe++) {
              if (IsStrobeNotValid (Host, strobe)) {
                continue;
              }

              if (marginStrobe[subRank].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n >
                  marginStrobe[subRank].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p) {

                marginStrobe[subRank].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p = 0;
                marginStrobe[subRank].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n = 0;
              }
              RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, (rank * maxSubRank[ch]) + subRank, strobe, NO_BIT, " %3d : %3d\n", marginStrobe[subRank].strobe[ch][(*rankList)[rank].rankIndex][strobe].n, marginStrobe[subRank].strobe[ch][(*rankList)[rank].rankIndex][strobe].p);
            } // strobe loop
          } // if per strobe
        } // subRank
      } // rank loop
    } // dimm loop
  } // ch loop

  if (group == RxVref) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "STOP_BACKSIDE_RX_VREF_CENTER\n");
  } else if (group == TxVref) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "STOP_BACKSIDE_TX_VREF_CENTER\n");
  } else if (group == TxDqDelay) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "STOP_BACKSIDE_TX_DQ_CENTER\n");
  } else if (group == RxDqsDelay) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "STOP_BACKSIDE_RX_DQ_CENTER\n");
  }

  //
  //  program final values
  //
  if (perRankCentering == 1) {
    RcDebugPrint (SDBG_MAX, "Per Rank Centering\n");
  } else {
    RcDebugPrint (SDBG_MAX, "Per Strobe Centering\n");
  }

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    dimmNvList = GetDimmNvList (Host, socket, ch);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      // reset the composite eye for per dimm results
      vlo_rank = -0xff;
      vhi_rank = 0xff;

      for (strobe = 0; strobe < maxStrobe; strobe++) {
        vlo_strobe[strobe] = -0xFF;
        vhi_strobe[strobe] = 0xFF;
      }

      if (((*dimmNvList)[dimm].dimmPresent == 0)) {
        continue;
      }
      if (IsLrdimmPresent (socket, ch, dimm) == 0) {
        continue;
      }
      rankList = GetRankNvList (Host, socket, ch, dimm);

      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        if ((group == RxDqsDelay) || (group == TxDqDelay)) {
          RcDebugPrint (SDBG_MAX,
                                  "                 DqDqs-   DqDqs + offset\n");
        } else {
          RcDebugPrint (SDBG_MAX,
                                  "                 vLo   vHi   offset\n");
        }

        for (subRank = 0; subRank < maxSubRank[ch]; subRank++) {
          // reset the composite eye for per rank results
          if ((perRankCentering == 1) || ((group == TxVref) && ((Setup->mem.optionsExt & PDA_EN) == 0))) { // Per rank
            vhi = marginRank[subRank].rank[ch][ (*rankList)[rank].rankIndex].p;
            vlo = marginRank[subRank].rank[ch][ (*rankList)[rank].rankIndex].n;
            offset = (vhi + vlo) / 2;

            if ((*channelNvList)[ch].chOneRankTimingModeEnable != ONE_RANK_TIMING_MODE_ENABLED) {
              RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, (rank * maxSubRank[ch]) + subRank, NO_STROBE, NO_BIT, " %3d     %3d      %3d\n", vlo, vhi, offset);
              GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, (rank * maxSubRank[ch]) + subRank, ALL_STROBES, ALL_BITS, level, group,
                GSM_UPDATE_CACHE | GSM_WRITE_OFFSET | GSM_FORCE_WRITE, &offset);
            } else {
              //
              // One Rank Timing mode, catch max_n and min_p across ranks on same Dimm
              //
              if (vhi_rank > marginRank[subRank].rank[ch][ (*rankList)[rank].rankIndex].p) {
                vhi_rank = marginRank[subRank].rank[ch][ (*rankList)[rank].rankIndex].p;
              }
              if (vlo_rank < marginRank[subRank].rank[ch][ (*rankList)[rank].rankIndex].n) {
                vlo_rank = marginRank[subRank].rank[ch][ (*rankList)[rank].rankIndex].n;
              }
              RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, (rank * maxSubRank[ch]) + subRank, NO_STROBE, NO_BIT, " %3d     %3d\n", vlo_rank, vhi_rank);
            }
          } else { // Per strobe
            for (strobe = 0; strobe < maxStrobe; strobe++) {
              if (IsStrobeNotValid (Host, strobe)) {
                continue;
              }
              vhi = marginStrobe[subRank].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p;
              vlo = marginStrobe[subRank].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n;
              offset = (vhi + vlo) / 2;

              SwizzleStrobe = strobe;
              if ((*dimmNvList)[dimm].DcpmmPresent) {
                SwizzleStrobe = CheckBacksideSwizzle (Host, socket, ch, dimm, strobe);
              }

              if ((*channelNvList)[ch].chOneRankTimingModeEnable != ONE_RANK_TIMING_MODE_ENABLED) {

                RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, (rank * maxSubRank[ch]) + subRank, strobe, NO_BIT, " %3d     %3d      %3d\n", vlo, vhi, offset);
                GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, (rank * maxSubRank[ch]) + subRank, SwizzleStrobe, ALL_BITS, level, group,
                  GSM_UPDATE_CACHE | GSM_WRITE_OFFSET | GSM_FORCE_WRITE, &offset);
              } else {
                if (vhi_strobe[strobe] > marginStrobe[subRank].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p) {
                  vhi_strobe[strobe] = marginStrobe[subRank].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p;
                }
                if (vlo_strobe[strobe] < marginStrobe[subRank].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n) {
                  vlo_strobe[strobe] = marginStrobe[subRank].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n;
                }
                RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, (rank * maxSubRank[ch]) + subRank, strobe, NO_BIT, " %3d     %3d\n", vlo_strobe[strobe], vhi_strobe[strobe]);
              }
            } // strobe loop
          } // if per rank / per strobe
        } // subRank loop
      } // rank loop

      if ((*channelNvList)[ch].chOneRankTimingModeEnable == ONE_RANK_TIMING_MODE_ENABLED) {
        RcDebugPrint (SDBG_MAX, "One Rank Timing Mode Enable\n");
        if ((perRankCentering == 1) || ((group == TxVref) && ((Setup->mem.optionsExt & PDA_EN) == 0))) {
          offset = (vhi_rank + vlo_rank) / 2;
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, 0, NO_STROBE, NO_BIT, " %3d     %3d      %3d\n", vlo_rank, vhi_rank, offset);
          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, 0, ALL_STROBES, ALL_BITS, level, group,
            GSM_UPDATE_CACHE | GSM_WRITE_OFFSET | GSM_FORCE_WRITE, &offset);
        } else {
          for (strobe = 0; strobe < maxStrobe; strobe++) {
            if (IsStrobeNotValid (Host, strobe)) {
              continue;
            }

            offset = (vhi_strobe[strobe] + vlo_strobe[strobe]) / 2;

            SwizzleStrobe = strobe;
            if ((*dimmNvList)[dimm].DcpmmPresent) {
              SwizzleStrobe = CheckBacksideSwizzle (Host, socket, ch, dimm, strobe);
            }

            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, 0, strobe, NO_BIT, " %3d     %3d      %3d\n", vlo_strobe[strobe], vhi_strobe[strobe], offset);
            GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, 0, SwizzleStrobe, ALL_BITS, level, group,
              GSM_UPDATE_CACHE | GSM_WRITE_OFFSET | GSM_FORCE_WRITE, &offset);
          }
        }
      }
    } // dimm loop
  } // ch loop
#ifdef DEBUG_CODE_BLOCK
  DisplayDdr4LrdimmTrainingResults (Host, socket, group);
#endif //DEBUG_CODE_BLOCK

  IO_Reset (Host, socket);

  if ((group == TxVref) && (Setup->mem.optionsExt & PDA_EN)) {
    UpdateSafeTxVref (Host, socket, level);
  }

  return SUCCESS;
#else // DDR5_SUPPORT
  return FAILURE;
#endif // DDR5_SUPPORT
} // LRDIMMAdvCentering
#endif // LRDIMM_SUPPORT

UINT32
VrefCentering (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     perBit,
  MRC_GT    group
  )
/*++

  Re-centers using a more robust test

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param group   - Parameter to center
                   RxDqsDelay   0: Center Rx DqDqs
                   TxDqDelay    1: Center Tx DqDqs

  @retval SUCCESS

--*/
{
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               strobe;
  UINT8               bit;
  INT16               tOffset[MAX_CH][MAX_RANK_CH][3];
  MRC_GT              tGroup = 0;
  UINT16              mode;
  UINT8               timeIndex;
  UINT32              patternLength = 1;
  struct bitMask      filter;
  struct bitMargin    (*vrefMarginBit)[MAX_VREF_TIMING_NUMBER];
  struct strobeMargin vrefMarginStrobe[MAX_VREF_TIMING_NUMBER];
  struct rankMargin   eyeWidth;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  UINT8               MSVx4 = GetMaxStrobeValid (Host);
  UINT8               MBV = GetMaxBitsValid (Host);
  SYS_SETUP           *Setup;
  UINT8               MaxChDdr;
#ifdef DDR5_SUPPORT
  UINT8               BitIndex;
  UINT8               MaxBitIndex;

  if (GetCurrentTestType (socket) == ReadDfeTest) {
    MaxBitIndex = BITS_PER_NIBBLE;
  } else {
    MaxBitIndex = 1;
  }
#endif // #ifdef DDR5_SUPPORT

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  //
  // Return if this socket is disabled
  //

  if (IsSocketDimmPopulated (Host, socket) != TRUE) {
    return SUCCESS;
  }

  vrefMarginBit = RcAllocatePool (sizeof (*vrefMarginBit));
  if (vrefMarginBit == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return FAILURE;
  }
  channelNvList = GetChannelNvList (Host, socket);

#ifndef DDR5_SUPPORT
  //
  // For DDR4, make sure this is cleared before training
  // DDR5 has already trained TxVref at this point
  //
  if ((group == TxVref)) {
    Host->nvram.mem.socket[socket].TrainingStepDone.TxVref = 0;
  }
#endif // DDR5_SUPPORT

  if ((group == RxVref) || (group == RxSamplerEvenOdd)) {
    tGroup = RxDqsDelay;
  } else if (group == TxVref) {
    tGroup = TxDqDelay;
  } else {
    // Error
  }

  // Setup bitMask
  for (ch = 0; ch < MaxChDdr; ch++) {
    for (rank = 0; rank < MAX_RANK_CH; rank++) {
      eyeWidth.rank[ch][rank].n = 0;
      eyeWidth.rank[ch][rank].p = 0;
      tOffset[ch][rank][0] = 0;
      tOffset[ch][rank][1] = 0;
      tOffset[ch][rank][2] = 0;

      for (bit = 0; bit < MBV; bit++) {
        for (timeIndex = 0; timeIndex < MAX_VREF_TIMING_NUMBER; timeIndex++) {
          (*vrefMarginBit)[timeIndex].bits[ch][rank][bit].n = 0;
          (*vrefMarginBit)[timeIndex].bits[ch][rank][bit].p = 0;
        }
      } // bit loop

      for (strobe = 0; strobe < MSVx4; strobe++) {
        for (timeIndex = 0; timeIndex < MAX_VREF_TIMING_NUMBER; timeIndex++) {
          vrefMarginStrobe[timeIndex].strobe[ch][rank][strobe].n = 0;
          vrefMarginStrobe[timeIndex].strobe[ch][rank][strobe].p = 0;
        }
      } // strobe loop
    } // rank loop
  } // ch loop
  //
  // Clear the filter bits to enable error checking on every bit
  //
  ClearFilter (Host, socket, &filter);

  //
  // Get per rank eye width
  //
  if (IsDdr5Present (Host, socket)  && (IsBrsPresent (Host, socket) == FALSE)) {
#ifdef DDR5_SUPPORT
    for (BitIndex = 0; BitIndex < MaxBitIndex; BitIndex++) {
      Host->var.mem.socket[socket].TrainingStatus.Bits.BitIndex = BitIndex;
      UpdateDramSeed (socket);
      GetDfeDataMargins (socket, DdrLevel, tGroup, NULL, NULL, &eyeWidth);
    }
#endif //DDR5_SUPPOER
  } else {
    GetMargins (Host, socket, DdrLevel, tGroup, MODE_VIC_AGG, SCOPE_RANK, &filter, &eyeWidth, 64, 0, 0, 0,
      WDB_BURST_LENGTH, RmtDqTest, NULL);
  }

  //
  // Determine the timing offsets per rank
  //
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "Timing offsets to test\n");
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "tOffset         0     1     2\n");
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, socket, ch);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {

      rankList = GetRankNvList (Host, socket, ch, dimm);

      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (IsLrdimmPresent (socket, ch, dimm)&& (group == TxVref) && (rank > 0)) {
          continue;
        }

        if (eyeWidth.rank[ch][ (*rankList)[rank].rankIndex].n > eyeWidth.rank[ch][ (*rankList)[rank].rankIndex].p) {
          eyeWidth.rank[ch][ (*rankList)[rank].rankIndex].n = 0;
          eyeWidth.rank[ch][ (*rankList)[rank].rankIndex].p = 0;
        }

        // T = min(12, EW/2)/3
        if (12 <= ((eyeWidth.rank[ch][ (*rankList)[rank].rankIndex].p - eyeWidth.rank[ch][ (*rankList)[rank].rankIndex].n) / 2)) {
          tOffset[ch][ (*rankList)[rank].rankIndex][0] = -12/3;
          tOffset[ch][ (*rankList)[rank].rankIndex][1] = 12/3;
        } else {
          tOffset[ch][ (*rankList)[rank].rankIndex][0] = - ((eyeWidth.rank[ch][ (*rankList)[rank].rankIndex].p - eyeWidth.rank[ch][ (*rankList)[rank].rankIndex].n) / 6);
          tOffset[ch][ (*rankList)[rank].rankIndex][1] = (eyeWidth.rank[ch][ (*rankList)[rank].rankIndex].p - eyeWidth.rank[ch][ (*rankList)[rank].rankIndex].n) / 6;
        }
        tOffset[ch][ (*rankList)[rank].rankIndex][2] = 0;
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                      "     %3d   %3d   %3d\n", tOffset[ch][(*rankList)[rank].rankIndex][0], tOffset[ch][(*rankList)[rank].rankIndex][1], tOffset[ch][(*rankList)[rank].rankIndex][2]);
      } // rank loop
    } // dimm loop
  } // ch loop

  //
  // Get per strobe Vref margins at 3 timing settings
  //
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "Get vref margins at 3 timing points\n");
  // Get pattern length
  mode = MODE_VIC_AGG;
  patternLength = TRAIN_RD_DQS_PL;

  // Set timing offset per rank
  for (timeIndex = 0; timeIndex < MAX_VREF_TIMING_NUMBER; timeIndex++) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      dimmNvList = GetDimmNvList (Host, socket, ch);

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        rankList = GetRankNvList (Host, socket, ch, dimm);

        for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          if (IsLrdimmPresent (socket, ch, dimm)&& (group == TxVref) && (rank > 0)) {
            continue;
          }
          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, 0, DdrLevel, tGroup, GSM_WRITE_OFFSET | GSM_FORCE_WRITE,
            &tOffset[ch][ (*rankList)[rank].rankIndex][timeIndex]);

        } // rank loop
      } // dimm loop
    } // ch loop

    //
    // Get Vref margin at this timing offset
    //

    if (perBit == PER_BIT) {
      if (IsDdr5Present (Host, socket) && (IsBrsPresent (Host, socket) == FALSE)) {
#ifdef DDR5_SUPPORT
        for (BitIndex = 0; BitIndex < MaxBitIndex; BitIndex++) {
          Host->var.mem.socket[socket].TrainingStatus.Bits.BitIndex = BitIndex;
          UpdateDramSeed (socket);
          GetDfeDataMargins (socket, DdrLevel, group, NULL, &(*vrefMarginBit)[timeIndex], NULL);
        }
#endif //DDR5_SUPPOER
      } else {
        GetMargins (Host, socket, DdrLevel, group, mode, SCOPE_BIT, &filter, &(*vrefMarginBit)[timeIndex],
          patternLength, 0, 0, 0, WDB_BURST_LENGTH, RmtDqTest, NULL);
      }
    } else {
      if (IsDdr5Present (Host, socket) && (IsBrsPresent (Host, socket) == FALSE)) {
#ifdef DDR5_SUPPORT
        for (BitIndex = 0; BitIndex < MaxBitIndex; BitIndex++) {
          Host->var.mem.socket[socket].TrainingStatus.Bits.BitIndex = BitIndex;
          UpdateDramSeed (socket);
          GetDfeDataMargins (socket, DdrLevel, group, &vrefMarginStrobe[timeIndex], NULL, NULL);
        }
#endif //DDR5_SUPPOER
      } else {
        GetMargins (Host, socket, DdrLevel, group, mode, SCOPE_STROBE, &filter, &vrefMarginStrobe[timeIndex],
          patternLength, 0, 0, 0, WDB_BURST_LENGTH, RmtDqTest, NULL);
      }
    }
  } // timeIndex loop

#ifdef DEBUG_CODE_BLOCK
  if (IsDdr5Present (Host, socket)) {
    if (group == RxSamplerEvenOdd) {
      DisplayAndAdjustVrefCenteringResultsDdr5 (Host, socket, RxSamplerEven, (*vrefMarginBit), vrefMarginStrobe, perBit);
      DisplayAndAdjustVrefCenteringResultsDdr5 (Host, socket, RxSamplerOdd, (*vrefMarginBit), vrefMarginStrobe, perBit);
    } else {
      DisplayAndAdjustVrefCenteringResultsDdr5 (Host, socket, group, (*vrefMarginBit), vrefMarginStrobe, perBit);
    }
  } else {
    DisplayAndAdjustVrefCenteringResults (Host, socket, group, (*vrefMarginBit), vrefMarginStrobe, perBit);
  }
#endif  // DEBUG_CODE_BLOCK

  if ((group == TxVref) && (Setup->mem.optionsExt & PDA_EN) && (CheckDimmType (Host) == SUCCESS)) {
    Host->nvram.mem.socket[socket].TrainingStepDone.TxVref = 1;
  }
  //
  // Evaluate and program the results
  //
  if (perBit == PER_BIT) {
    CombineVrefResults (Host, socket, group, perBit, (*vrefMarginBit));
    DetectLowMargin (Host, NO_CH, NO_DIMM, NO_RANK, (*vrefMarginBit), group, perBit);
  } else {
    CombineVrefResults (Host, socket, group, perBit, vrefMarginStrobe);
    DetectLowMargin (Host, NO_CH, NO_DIMM, NO_RANK, vrefMarginStrobe, group, perBit);
  }

  if ((group == TxVref) && (Setup->mem.optionsExt & PDA_EN)) {
    UpdateSafeTxVref (Host, socket, DdrLevel);
  }
  RcFreePool (vrefMarginBit);

#ifdef DEBUG_CODE_BLOCK
  if (((group == RxVref) || (group == RxSamplerEvenOdd)) &&
      ((GetDebugLevel () & SDBG_MAX) != 0)) {
    AcquirePrintControl ();
    if (IsDdr5Present (Host, socket)) {
      if (group == RxSamplerEvenOdd) {
        DisplayResultsDdr5 (Host, socket, RxSamplerEven);
        DisplayResultsDdr5 (Host, socket, RxSamplerOdd);
      } else {
        DisplayResultsDdr5 (Host, socket, group);
      }
    } else {
      DisplayResults (Host, socket, RxVref);
    }
    RcDebugPrint (SDBG_MEM_TRAIN, "\n");
    ReleasePrintControl ();
  }
#endif  // DEBUG_CODE_BLOCK

  return SUCCESS;
} // VrefCentering

UINT32
RxPerBitDeskew (
  PSYSHOST  Host
  )
/*++

  Receive Per Bit Deskew

  @param Host   - Pointer to sysHost

  @retval SUCCESS

--*/
{
  volatile UINT8 DefinedDeskewDelay = DESKEW_DELAY;

  if (!IsMemFlowEnabled (RxTxPerBitDeskewTraining)) {
    return SUCCESS;
  }

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_PER_BIT_DESKEW_RX, Host->var.mem.currentSocket));
  if (DefinedDeskewDelay == RxDqsBitDelay) {
    PerBitDeskew (Host, Host->var.mem.currentSocket, RxDqsBitDelay);
  } else {
    PerBitDeskew (Host, Host->var.mem.currentSocket, RxDqsPBitDelay);
    PerBitDeskew (Host, Host->var.mem.currentSocket, RxDqsNBitDelay);
  }

  return SUCCESS;

} // RxPerBitDeskew

UINT32
TxPerBitDeskew (
  PSYSHOST  Host
  )
/*++

    Tx Per Bit Deskew

    @param Host  - Pointer to sysHost

    @retval SUCCESS

--*/
{
  if (!IsMemFlowEnabled (RxTxPerBitDeskewTraining)) {
    return SUCCESS;
  }

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_PER_BIT_DESKEW_TX, Host->var.mem.currentSocket));
  PerBitDeskew (Host, Host->var.mem.currentSocket, TxDqBitDelay);

  return SUCCESS;
} // TxPerBitDeskew


/**

Arguments:

  Host    - Pointer to sysHost

**/
VOID
PerBitDeskew (
  PSYSHOST  Host,
  UINT8     socket,
  MRC_GT    group
  )
{
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               nibble;
  UINT8               strobe;
  MRC_GT              gmGroup;
  UINT8               bit, bitIdx;
  INT16               BitSkew[BITS_PER_NIBBLE];
  INT8                bitCenter[8];
  INT16               bitNibbleMin;
  UINT32              patternLength;
  INT8                rankBitCenter[MAX_DIMM][MAX_RANK_DIMM][MAX_BITS];
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  struct bitMargin    resultsBit;
  struct bitMask      filter;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MBV = GetMaxBitsValid (Host);
  SYS_SETUP           *Setup;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  //
  // Return if this socket is disabled
  //

  if (IsSocketDimmPopulated (Host, socket) != TRUE) {
    return;
  }

  channelNvList = GetChannelNvList (Host, socket);
  patternLength = 64;

  if (Setup->mem.perBitDeSkew == BIT_DESKEW_DISABLE) {
    RcDebugPrint (SDBG_MAX, "PER_BIT_DESKEW - Disabled\n");
    return;
  }
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "START_PER_BIT_DESKEW\n");


  // Fill Filter Structure and initialize margin results structure
  for (ch = 0; ch < MaxChDdr; ch++) {
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      rankList = GetRankNvList (Host, socket, ch, dimm);
      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
        for (bit = 0; bit < MBV; bit++) {
          resultsBit.bits[ch][ (*rankList)[rank].rankIndex][bit].n = 0;
          resultsBit.bits[ch][ (*rankList)[rank].rankIndex][bit].p = 0;
        }
      } // Rank loop
    } // Dimm loop
  } // Ch loop

#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    AcquirePrintControl ();
    RcDebugPrintLine (SDBG_MAX, 78, DOUBLE_LINE, CRLF_FLAG, CRLF_FLAG);
  }
#endif //DEBUG_CODE_BLOCK

  // Setup for either Rx and Tx
  if (group == RxDqsPBitDelay) {
    gmGroup = RxDqsPDelay;
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Per Bit Deskew Rx P\n");
  } else if (group == RxDqsNBitDelay) {
    gmGroup = RxDqsNDelay;
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Per Bit Deskew Rx N\n");
  } else if (group == RxDqsBitDelay) {
    gmGroup = RxDqsDelay;
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Per Bit Deskew Rx\n");
  } else {
    gmGroup = TxDqDelay;
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "Per Bit Deskew Tx\n");
  }

  RcDebugPrintLine (SDBG_MAX, 78, DOUBLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

  //
  // Clear the filter bits to enable error checking on every bit
  //
  ClearFilter (Host, socket, &filter);

  // Get per bit margins
  RcDebugPrint (SDBG_MAX, "PatternLength: %d\n", patternLength);
  GetMargins (Host, socket, DdrLevel, gmGroup, MODE_VIC_AGG, SCOPE_BIT, &filter, (VOID *)&resultsBit, patternLength,
    0, 0, 0, WDB_BURST_LENGTH, RmtDqTest, NULL);

#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    DisplayPerBitMargins (Host, socket, &resultsBit, gmGroup, DdrLevel);
  }
#endif //DEBUG_CODE_BLOCK
  DetectLowMargin (Host, NO_CH, NO_DIMM, NO_RANK, &resultsBit, group, NON_PER_BIT);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, socket, ch);

    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Calculating Bit Centers\n");

    CalculateBitCenters (Host, socket, ch, rankBitCenter, &resultsBit);

    dimm = 0;
    rank = 0;
    dimmNvList = GetDimmNvList (Host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }

      rankList = GetRankNvList (Host, socket, ch, dimm);
      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        // Get the Logical rank #
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "dimm = %d, rank = %d\n", dimm, rank);
        RcDebugPrint (SDBG_MAX,
          "         Per Bit Margin Center       Per Bit Skew\n");
        RcDebugPrint (SDBG_MAX,
          "N# = Nibble, BCx = Bit Center, BSx = Bit Skew , MSL = Most Skewed Lane\n");
        if ((group == RxDqsPBitDelay) || (group == RxDqsNBitDelay)) {
          RcDebugPrint (SDBG_MAX,
            "N# BC0-BC1-BC2-BC3   BS0-BS1-BS2-BS3  MSL\n");
        } else {
          RcDebugPrint (SDBG_MAX,
                        "N# BC0-BC1-BC2-BC3   BS0-BS1-BS2-BS3\n");
        }

        RcDebugPrintLine (SDBG_MAX, 78, DOUBLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

        // Loop for each nibble/strobe.
        for (nibble = 0; nibble < MSVx4; nibble++) {
          //
          // Skip if this is an ECC nibble when ECC is disabled
          // Note: unlike usual in training code, per-bit skew register are mapped linerly, so ECC bits are on the end
          //
          if (IsNibbleNotValid (Host, nibble)) {
            continue;
          }
          strobe = GetStrobeFromNibble (Host, nibble);

          // Faulty part tracking: need to add Array checking here and if we find a faulty part.
          // need to add minimum margin (advanced strobe centering) and/or just no margin at all
          // If more than one failing nibble then that is a hard failure. So need to keep track of FPT and if more than
          // One nibble is failing, we have a real failure.
          // A sincle bit margin failure within a nibble can indicate a FPT, so flag it, if we can't rehabilitate the nibble via the deskew algo.

          bitNibbleMin = 127;   // Set to largest possible
          RcDebugPrint (SDBG_MAX,
                      "%2d ", nibble);

          // Loop for all the bits in this nibble
          for (bit = 0; bit < BITS_PER_NIBBLE; bit++) {
            // Get the absolute bit index (0-72)
            bitIdx = (nibble * 4) + bit;

            // Find the center for this bit
            bitCenter[bit] = rankBitCenter[dimm][rank][bitIdx];

            RcDebugPrint (SDBG_MAX,
                           "%3d ", bitCenter[bit]);

            // Check if the center of this bit is smaller than the smallest found so far
            if (bitCenter[bit] < bitNibbleMin) {
              bitNibbleMin = bitCenter[bit];
            }
          } // for (bit)

          RcDebugPrint (SDBG_MAX,
                         "  ");

          // This bit loop is only for calculating the per-bit skew values
          for (bit = 0; bit < BITS_PER_NIBBLE; bit++) {
            // Get the Skew
            BitSkew[bit] = bitCenter[bit] - bitNibbleMin;
            RcDebugPrint (SDBG_MAX, "%3d ", BitSkew[bit]);
          } // for bit
          //
          // Call SetGDCR to update the appropriate register fields - do the entire nibble at once
          // Pass a pointer to all bits in the nibble - the Get/Set routine might need them all
          //
          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, ALL_BITS, DdrLevel, group, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET, BitSkew);
          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, gmGroup, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET, &bitNibbleMin);
          RcDebugPrint (SDBG_MAX, "\n");

        } // nibble loop
        if (group == TxDqBitDelay) {
          TxDqWaAdv (socket, ch, NO_SUBCH, dimm, rank, TRUE);
        }
      } // rank loop
    } // dimm loop
  } // ch loop

#ifdef DEBUG_CODE_BLOCK

  if (GetDebugLevel () & SDBG_MAX) {
    DisplayResults (Host, socket, group);
  }

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "STOP_PER_BIT_DESKEW\n");

  if (GetDebugLevel () & SDBG_MAX) {
    ReleasePrintControl ();
  }
#endif //DEBUG_CODE_BLOCK

} // PerBitDeskew


VOID
CalculateBitCenters (
  PSYSHOST          Host,
  UINT8             socket,
  UINT8             ch,
  INT8              rankBitCenter[MAX_DIMM][MAX_RANK_DIMM][MAX_BITS],
  struct bitMargin  *resultsBit
  )
{
  UINT8   dimm, rank, bit;
  struct  ddrRank (*rankList)[MAX_RANK_DIMM];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  UINT8 MBV = GetMaxBitsValid (Host);

  dimmNvList = GetDimmNvList (Host, socket, ch);

  // Clear the rankBitCenter Array first
  for (bit = 0; bit < MBV; bit++) {
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
        rankBitCenter[dimm][rank][bit] = 0;
      }
    }
  }

  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    if ((*dimmNvList)[dimm].dimmPresent == 0) {
      continue;
    }
    rankList = GetRankNvList (Host, socket, ch, dimm);
    for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
      if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
        continue;
      }

      for (bit = 0; bit < MBV; bit++) {
        if (IsBitNotValid (Host, bit)) {
          continue;
        }
        rankBitCenter[dimm][rank][bit] = (((INT8) (resultsBit->bits[ch][ (*rankList)[rank].rankIndex][bit].n +
                                                   resultsBit->bits[ch][ (*rankList)[rank].rankIndex][bit].p))) / 2;
      } // for bit
    } // rank
  } // dimm
} // CalculateBitCenters


VOID
InitStartingOffset (
  PSYSHOST  Host,
  UINT8     socket
  )
/*++
  Initializes the starting margins

  @param Host        - Pointer to sysHost
  @param socket      - Processor socket to check

  @retval N/A

--*/
{

  UINT8                   ch;
  UINT8                   dimm;
  UINT8                   rank;
  struct channelNvram     (*channelNvList)[MAX_CH];
  struct dimmNvram        (*dimmNvList)[MAX_DIMM];
  struct ddrRank          (*rankList)[MAX_RANK_DIMM];
  struct TrainingVariable *TrainingVar;
  UINT8                   MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  TrainingVar = MemTrainingVar ();
  channelNvList = GetChannelNvList (Host, socket);

  TrainingVar->rdVrefLo  = RD_WR_CMD_INIT_MARG;
  TrainingVar->rdVrefHi  = RD_WR_CMD_INIT_MARG;
  TrainingVar->wrVrefLo  = RD_WR_CMD_INIT_MARG;
  TrainingVar->wrVrefHi  = RD_WR_CMD_INIT_MARG;
  TrainingVar->cmdVrefLo = RD_WR_CMD_INIT_MARG;
  TrainingVar->cmdVrefHi = RD_WR_CMD_INIT_MARG;
#ifdef SPR_HOST
  TrainingVar->RxSamplerLo  = RD_WR_CMD_INIT_MARG; //SPR_todo. HSD 16010478187 To confirm the value?
  TrainingVar->RxSamplerHi  = RD_WR_CMD_INIT_MARG; //SPR_todo. HSD 16010478187 To confirm the value?
#endif //SPR_HOST
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
        (*rankList)[rank].cmdLow      = CMD_VREF_INIT_MARG;
        (*rankList)[rank].cmdHigh     = CMD_VREF_INIT_MARG;
        (*rankList)[rank].cmdLeft     = CMD_INIT_MARG;
        (*rankList)[rank].cmdRight    = CMD_INIT_MARG;
        (*rankList)[rank].rxDQLeftSt  = RX_TX_DQ_INIT_MARG;
        (*rankList)[rank].rxDQRightSt = RX_TX_DQ_INIT_MARG;
        (*rankList)[rank].txDQLeftSt  = RX_TX_DQ_INIT_MARG;
        (*rankList)[rank].txDQRightSt = RX_TX_DQ_INIT_MARG;
      } // rank loop
    } // dimm loop
  } // ch loop
} // InitStartingOffset

UINT32
CombineVrefResults (
  PSYSHOST  Host,
  UINT8     socket,
  MRC_GT    group,
  UINT8     perBit,
  VOID      *value
  )
/*++

  Evaluates Vref results and programs optimal settings

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param group   - Parameter to center
                   RxVref 0: Center Rx Vref
                   TxVref 1: Center Tx Vref
  @param value   - Pointer to the vrefMargin

  @retval SUCCESS

--*/
{
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               strobe = 0;
  UINT8               StrobeIndex;
  UINT8               bit;
  UINT8               bitPerNibble = 0;
  UINT8               maxStrobe;
  INT16               vrefHi = 0x5F;
  INT16               vrefLo = -0x5F;
  INT16               tmpEdge;
  INT16               offset;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  struct bitMargin    (*vrefMarginBit)[MAX_VREF_TIMING_NUMBER];
  struct strobeMargin (*vrefMarginStrobe)[MAX_VREF_TIMING_NUMBER];
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8 MBV = GetMaxBitsValid (Host);
  SYS_SETUP           *Setup;
  UINT8               MaxChDdr;
#ifdef DDR5_SUPPORT
  UINT8               SubChannelMaxBit;
  UINT8               SubChannelBitIndex;
  UINT8               SubChannel;
  UINT8               Nibble = 0;
  SubChannelMaxBit = MBV / 2;
#endif //DDR5_SUPPORT
  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  vrefMarginBit = value;
  vrefMarginStrobe = value;

  channelNvList = GetChannelNvList (Host, socket);

  if ((group == RxVref) || (group == RxSamplerEvenOdd)) {
    if (perBit == PER_BIT) {
      if (IsDdr5Present (Host, socket)) {
#ifdef DDR5_SUPPORT
        RcDebugPrint (SDBG_MAX,
                      "                vrefLo  vrefHi    offset\n");
#endif // DDR5_SUPPORT
      } else {
        RcDebugPrint (SDBG_MAX,
                      "            vrefLo  vrefHi    offset\n");
      }
      for (bit = 0; bit < MBV; bit++) {
        //
        // Skip if this is an ECC strobe when ECC is disabled
        //
        if (IsBitNotValid (Host, bit)) {
          continue;
        }
        for (ch = 0; ch < MaxChDdr; ch++) {
          //
          // Skip if channel is disabled
          //
          if ((*channelNvList)[ch].enabled == 0) {
            continue;
          }

          vrefHi = 0x5F;
          vrefLo = -0x5F;

          dimmNvList = GetDimmNvList (Host, socket, ch);

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
              //
              // Find the composite eye for this strobe across ranks
              //
              tmpEdge = ((*vrefMarginBit)[0].bits[ch][ (*rankList)[rank].rankIndex][bit].p +
                         (*vrefMarginBit)[1].bits[ch][ (*rankList)[rank].rankIndex][bit].p +
                         (*vrefMarginBit)[2].bits[ch][ (*rankList)[rank].rankIndex][bit].p) / 3;
              if (vrefHi > tmpEdge) {
                vrefHi = tmpEdge;
              }

              tmpEdge = ((*vrefMarginBit)[0].bits[ch][ (*rankList)[rank].rankIndex][bit].n +
                         (*vrefMarginBit)[1].bits[ch][ (*rankList)[rank].rankIndex][bit].n +
                         (*vrefMarginBit)[2].bits[ch][ (*rankList)[rank].rankIndex][bit].n) / 3;
              if (vrefLo < tmpEdge) {
                vrefLo = tmpEdge;
              }
            } // rank loop
          } // dimm loop

          offset = (vrefHi + vrefLo) / 2;

          if (IsDdr5Present (Host, socket)) {
#ifdef DDR5_SUPPORT
            if (bit < SubChannelMaxBit) {
              SubChannel  = SUB_CH_A;
            } else {
              SubChannel  = SUB_CH_B;
            }
            SubChannelBitIndex = bit - SubChannel * SubChannelMaxBit;
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, SubChannel, NO_RANK, NO_STROBE, SubChannelBitIndex, "  %2d      %2d       %2d\n", vrefLo, vrefHi, offset);
#endif // DDR5_SUPPORT
          } else {
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, bit, "  %2d      %2d       %2d\n", vrefLo, vrefHi, offset);
          }

          if (IsDdr5Present (Host, socket)) {
#ifdef DDR5_SUPPORT
            Nibble = bit / BITS_PER_NIBBLE;
            if (Nibble < (MAX_STROBE_DDR5 / 2)) {
              strobe = GetStrobeFromNibble (Host, Nibble);
            } else {
              Nibble = Nibble - (MAX_STROBE_DDR5 / 2);
              strobe = GetStrobeFromNibble (Host, Nibble) + (MAX_STROBE_DDR5 / 2);
            }
            bitPerNibble = bit % BITS_PER_NIBBLE;
#endif //DDR5_SUPPORT
          } else {
            maxStrobe = GetMaxStrobe (Host, socket, ch, 0, RxVref, DdrLevel);
            if (maxStrobe != 0) {
              strobe = (((bit % MAX_BITS_IN_BYTE) / (MBV / maxStrobe) * MSVx8) + (bit / MAX_BITS_IN_BYTE));
              bitPerNibble = bit % (MBV / maxStrobe);
            }
          }

          GetSetDataGroup (Host, socket, ch, NO_SUBCH, 0, 0, strobe, bitPerNibble, DdrLevel, group, GSM_UPDATE_CACHE + GSM_WRITE_OFFSET,
            &offset);
        } // ch loop
      } // bit loop
    } else {
      RcDebugPrint (SDBG_MAX,
                    "            vrefLo  vrefHi    offset\n");
      maxStrobe = GetMaxStrobe (Host, socket, 0, 0, group, DdrLevel);
      for (strobe = 0; strobe < maxStrobe; strobe++) {
        //
        // Skip if this is an ECC strobe when ECC is disabled
        //
        if (IsStrobeNotValid (Host, strobe)) {
          continue;
        }

        for (ch = 0; ch < MaxChDdr; ch++) {
          //
          // Skip if channel is disabled
          //
          if ((*channelNvList)[ch].enabled == 0) {
            continue;
          }

          vrefHi = 0x5F;
          vrefLo = -0x5F;

          dimmNvList = GetDimmNvList (Host, socket, ch);

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

              //
              // Find the composite eye for this strobe across ranks
              //
              tmpEdge = ((*vrefMarginStrobe)[0].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p +
                         (*vrefMarginStrobe)[1].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p +
                         (*vrefMarginStrobe)[2].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p) / 3;
              if (vrefHi > tmpEdge) {
                vrefHi = tmpEdge;
              }

              tmpEdge = ((*vrefMarginStrobe)[0].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n +
                         (*vrefMarginStrobe)[1].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n +
                         (*vrefMarginStrobe)[2].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n) / 3;
              if (vrefLo < tmpEdge) {
                vrefLo = tmpEdge;
              }

            } // rank loop
          } // dimm loop

          offset = (vrefHi + vrefLo) / 2;
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, strobe, NO_BIT,
                         "  %2d      %2d       %2d\n", vrefLo, vrefHi, offset);

          GetSetDataGroup (Host, socket, ch, NO_SUBCH, 0, 0, strobe, ALL_BITS, DdrLevel, group, GSM_UPDATE_CACHE + GSM_WRITE_OFFSET,
            &offset);
        } // ch loop
      } // strobe loop
    }
  } else if (group == TxVref) {
    //
    // Do not center Tx Vref if PDA is disabled
    //
    if (Setup->mem.optionsExt & PDA_EN) {

      for (ch = 0; ch < MaxChDdr; ch++) {
        //
        // Skip if channel is disabled
        //
        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }

        dimmNvList = GetDimmNvList (Host, socket, ch);

        for (dimm = 0; dimm < MAX_DIMM; dimm++) {
          //
          // Skip if no DIMM present
          //
          if ((*dimmNvList)[dimm].dimmPresent == 0) {
            continue;
          }

          maxStrobe = GetMaxStrobe (Host, socket, ch, dimm, group, DdrLevel);

          rankList = GetRankNvList (Host, socket, ch, dimm);

          for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
            //
            // Skip if no rank
            //
            if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
              continue;
            }
            if (IsLrdimmPresent (socket, ch, dimm)&& (group == TxVref) && (rank > 0)) {
              continue;
            }

            RcDebugPrint (SDBG_MAX,
                           "                vrefLo  vrefHi    offset\n");
            for (StrobeIndex = 0; StrobeIndex < maxStrobe; StrobeIndex++) {
              //
              // Skip if this is an ECC strobe when ECC is disabled
              //
              if ((!Host->nvram.mem.eccEn) && IsEccStrobe (Host, StrobeIndex)){
                continue;
              }

              //
              // In the DDR5 x8 case, the "linear" strobes 5-9 results are stored at the 10-14
              // In the rest of cases, they are 1:1 mapped.
              //
              strobe = StrobeLinearToLogical (maxStrobe, StrobeIndex);

              //
              // Find the composite eye for this strobe across ranks
              //
              vrefHi = ((*vrefMarginStrobe)[0].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p +
                        (*vrefMarginStrobe)[1].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p +
                        (*vrefMarginStrobe)[2].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p) / 3;

              vrefLo = ((*vrefMarginStrobe)[0].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n +
                        (*vrefMarginStrobe)[1].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n +
                        (*vrefMarginStrobe)[2].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n) / 3;

              offset = (vrefHi + vrefLo) / 2;

              RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                             "  %2d      %2d       %2d\n", vrefLo, vrefHi, offset);
              GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, ALL_BITS, DdrLevel, group,
                GSM_UPDATE_CACHE | GSM_WRITE_OFFSET | GSM_FORCE_WRITE, &offset);
            } // strobe loop
          } // rank loop
        } // dimm loop
      } // ch loop

    } else {

      RcDebugPrint (SDBG_MAX,
                     "                vrefLo  vrefHi    offset\n");
      for (ch = 0; ch < MaxChDdr; ch++) {
        //
        // Skip if channel is disabled
        //
        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }

        dimmNvList = GetDimmNvList (Host, socket, ch);

        for (dimm = 0; dimm < MAX_DIMM; dimm++) {
          //
          // Skip if no DIMM present
          //
          if ((*dimmNvList)[dimm].dimmPresent == 0) {
            continue;
          }

          maxStrobe = GetMaxStrobe (Host, socket, ch, dimm, group, DdrLevel);

          rankList = GetRankNvList (Host, socket, ch, dimm);

          for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
            //
            // Skip if no rank
            //
            if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
              continue;
            }
            if (IsLrdimmPresent (socket, ch, dimm)&& (group == TxVref) && (rank > 0)) {
              continue;
            }
            vrefHi = 0x5F;
            vrefLo = -0x5F;

            for (StrobeIndex = 0; StrobeIndex < maxStrobe; StrobeIndex++) {
              //
              // Skip if this is an ECC strobe when ECC is disabled
              //
              if ((!Host->nvram.mem.eccEn) && IsEccStrobe (Host, StrobeIndex)) {
                continue;
              }

              if (IsBrsPresent (Host, socket) == FALSE) {
                //
                // In the DDR5 x8 case, the "linear" strobes 5-9 results are stored at the 10-14
                // In the rest of cases, they are 1:1 mapped.
                //
                strobe = StrobeLinearToLogical (maxStrobe, StrobeIndex);
              } else {
                strobe = StrobeIndex;
              }

              //
              // Find the composite eye for this strobe across ranks
              //
              tmpEdge = ((*vrefMarginStrobe)[0].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p +
                         (*vrefMarginStrobe)[1].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p +
                         (*vrefMarginStrobe)[2].strobe[ch][ (*rankList)[rank].rankIndex][strobe].p) / 3;
              if (vrefHi > tmpEdge) {
                vrefHi = tmpEdge;
              }

              tmpEdge = ((*vrefMarginStrobe)[0].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n +
                         (*vrefMarginStrobe)[1].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n +
                         (*vrefMarginStrobe)[2].strobe[ch][ (*rankList)[rank].rankIndex][strobe].n) / 3;
              if (vrefLo < tmpEdge) {
                vrefLo = tmpEdge;
              }

            } // strobe loop

            offset = (vrefHi + vrefLo) / 2;
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                           "  %2d      %2d       %2d\n", vrefLo, vrefHi, offset);

            //
            // Set Tx Vref per channel
            //
            GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, ALL_BITS, DdrLevel, group, GSM_UPDATE_CACHE + GSM_WRITE_OFFSET,
              &offset);

          } // rank loop
        } // dimm loop
      } // ch loop
    } // PDA check
  } else {
    // Error
  }

  IO_Reset (Host, socket);

  return SUCCESS;
} // CombineVrefResults

VOID
GetMultiVref (
  PSYSHOST        Host,
  UINT8           socket,
  MRC_LT          level,
  MRC_GT          group,
  UINT16          mode,
  UINT8           scope,
  struct bitMask  *filter,
  VOID            *results,
  UINT32          patternLength,
  UINT8           chMask,
  UINT8           rankMask,
  UINT8           stepSize,
  UINT8           numPoints
  )
/*++
  Calculates margins at multiple Vref points

  @param Host         - Pointer to sysHost
  @param socket       - Processor socket to check
  @param level        - DDR or LRDIMM Host side, LRDIMM Backside, etc...
  @param group        - Parameter to be margined
                         RxDqsDelay    : Margin Rx DqDqs
                         TxDqDelay     : Margin Tx DqDqs
                         RxVref         : Margin Rx Vref
                         TxVref         : Margin Tx Vref
                         RecEnDelay    : Margin Receive Enable
                         WrLvlDelay    : Margin Write Level
                         PcodeMailboxStatus = 0;
                         CmdGrp0        : Margin CMD group 0
                         CmdGrp1        : Margin CMD group 1
                         CmdGrp2        : Margin CMD group 2
                         CmdAll         : Margin all CMD groups
  @param mode         - Test mode to use
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
  @param scope        - Margin granularity
                         SCOPE_SOCKET      0: Margin per processor socket
                         SCOPE_CH        1: Margin per channel
                         SCOPE_DIMM      2: Margin per DIMM
                         SCOPE_RANK      3: Margin per rank
                         SCOPE_STROBE    4: Margin per strobe group
                         SCOPE_BIT       5: Margin per bit
  @param filter        - Mask of bits to exclude from testing
  @param results       - Pointer to the structure to store the margin results. The structure type varies based on scope
  @param patternLength - RankMarginTest Pattern length
  @param chMask        - Bit Mask of channels to not be used
  @param rankMask      - Bit Mask of ranks to not be used
  @param stepSize      - Step size
  @param numPoints     - Number of points to test

  @retval *results:  Pointer to the structure to store the margin results. The structure type varies based on scope.
--*/
{
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  MRC_GT              vGroup = 0;
  UINT8               maxVref;
  UINT8               errChMask = 0;
  INT32               vIndex;
  INT16               margin;
  INT16               restoreOffset[MAX_CH];
  UINT32              status = 0;
  UINT32              PrevDebugLevel;
  struct channelNvram (*channelNvList)[MAX_CH];
  VOID                *tmpResults = 0;
  UINT8               MaxChDdr;
  struct strobeMargin *StrobeResults;
  struct bitMargin    *BitResults;
  struct rankMargin   *RankResults;
#ifdef DDR5_SUPPORT
  UINT8               BitIndex;
  UINT8               MaxBitIndex;

  if (GetCurrentTestType (socket) == ReadDfeTest) {
    MaxBitIndex = BITS_PER_NIBBLE;
  } else {
    MaxBitIndex = 1;
  }
#endif // #ifdef DDR5_SUPPORT
  MaxChDdr = GetMaxChDdr ();
  ZeroMem ((UINT8 *) restoreOffset, sizeof (restoreOffset));
  channelNvList = GetChannelNvList (Host, socket);

  PrevDebugLevel = GetDebugLevel ();
  if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
    SetDebugLevel (SDBG_ERROR);
  }

  if ((group == RxDqsDelay) || (group == RxDqsPDelay) || (group == RxDqsNDelay)) {
    vGroup = RxVref;
    if (GetCurrentTestType (socket) == ReadDfeTest) {
      vGroup = RxSamplerEvenOdd;
    }
  } else if (group == TxDqDelay) {
    vGroup = TxVref;
  } else if (CheckCMDGroup (group)) {
    vGroup = CmdVref;
  } else {
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "group %d not supported in GetMultiVref\n", group);
  }

  maxVref = (numPoints / 2) * stepSize;

  //
  // Loop for all Vref's
  //
  vIndex = 0;

  for (margin = -maxVref; margin < maxVref + 1; margin = margin + (INT8) stepSize) {

    // For debug
    RcDebugPrint (SDBG_MAX,
                   "margin = %d, vIndex = %d, maxVref = %d\n", margin, vIndex, maxVref);

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      // Check if this channel is masked off
      if (chMask & (1 << ch)) {
        continue;
      }

      if (CheckCMDGroup (group) || CheckCTLGroup (group)) {
        GetSetCmdVrefCore (Host, socket, ch, 0, level, GSM_READ_ONLY, &restoreOffset[ch]);
        status = SetOffset (Host, socket, ch, 0, 0, margin, level, vGroup, 0);
        if (status != SUCCESS) {
          errChMask |= (1 << ch);
        }
      } else {

        for (dimm = 0; dimm < MAX_DIMM; dimm++) {

          for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
            if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
              continue;
            }

            if (rankMask & (1 << GetLogicalRank (Host, socket, ch, dimm, rank))) {
              continue;
            }

            // Skip setting TXVREF multiple times on LRDIMMs
            if (IsLrdimmPresent (socket, ch, dimm) && (vGroup == TxVref) && (level == DdrLevel) && (rank > 0)) {
              continue;
            }
            if (!IsLrdimmPresent (socket, ch, dimm)&& (level == LrbufLevel)) {
              continue;
            }

            // Set the new margin to test
            if (((vGroup != RxVref) && (vGroup != RxSamplerEvenOdd)) || ((dimm == 0) && (rank == 0) && (level == DdrLevel)) || ((level == LrbufLevel) && (rank == 0))) {
              status = SetOffset (Host, socket, ch, dimm, rank, margin, level, vGroup, 0);
            }
            if (status != SUCCESS) {
              errChMask |= (1 << ch);
            }
          } // rank loop
        } // dimm loop
      }
    } // ch loop

    StrobeResults = NULL;
    BitResults    = NULL;
    RankResults   = NULL;

    if (scope == SCOPE_RANK) {
      tmpResults    = (UINT8 *) results + (sizeof (struct rankMargin) * vIndex);
      RankResults   = tmpResults;
    } else if (scope == SCOPE_STROBE) {
      tmpResults    = (UINT8 *) results + (sizeof (struct strobeMargin) * vIndex);
      StrobeResults = tmpResults;
    } else if (scope == SCOPE_BIT) {
      tmpResults    = (UINT8 *) results + (sizeof (struct bitMargin) * vIndex);
      BitResults    = tmpResults;
    }

    //RcDebugPrint (SDBG_MAX,
    //              "Calling GetMargins\n");
    //
    // Get the margins at the current Vref setting
    //
    if (tmpResults == NULL) {
      continue;
    }
    if (margin == 0) {
      SetDebugLevel (PrevDebugLevel);
       RcDebugPrint (SDBG_MAX,
                  "margin = %d, vIndex = %d, Running 7 times pattern length\n", margin, vIndex);
      if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
        SetDebugLevel (SDBG_ERROR);
      }

      if (IsDdr5Present (Host, socket) && (IsBrsPresent (Host, socket) == FALSE)) {
#ifdef DDR5_SUPPORT
        for (BitIndex = 0; BitIndex < MaxBitIndex; BitIndex++) {
          Host->var.mem.socket[socket].TrainingStatus.Bits.BitIndex = BitIndex;
          UpdateDramSeed (socket);
          GetDfeDataMargins (socket, DdrLevel, group, StrobeResults, BitResults, RankResults);
        }
#endif //DDR5_SUPPOER
      } else {
        GetMargins (Host, socket, level, group, mode, scope, filter, tmpResults, patternLength * 7, 1, chMask | errChMask, rankMask,
          WDB_BURST_LENGTH, RmtDqTest, NULL);
      }
    } else {
      if (IsDdr5Present (Host, socket) && (IsBrsPresent (Host, socket) == FALSE)) {
#ifdef DDR5_SUPPORT

        for (BitIndex = 0; BitIndex < MaxBitIndex; BitIndex++) {
          Host->var.mem.socket[socket].TrainingStatus.Bits.BitIndex = BitIndex;
          UpdateDramSeed (socket);
          GetDfeDataMargins (socket, DdrLevel, group, StrobeResults, BitResults, RankResults);
        }
#endif //DDR5_SUPPOER
      } else {
        GetMargins (Host, socket, level, group, mode, scope, filter, tmpResults, patternLength, 1, chMask | errChMask, rankMask,
          WDB_BURST_LENGTH, RmtDqTest, NULL);
      }
    }

    if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
      SetDebugLevel (SDBG_ERROR);
    }

    if (CheckCMDGroup (group) || CheckCTLGroup (group)) {
      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }
        // Check if this channel is masked off
        if (chMask & (1 << ch)) {
          continue;
        }

        //
        // Restore the new margin to test
        //
        RestoreOffset (Host, socket, ch, 0, 0, restoreOffset[ch], level, vGroup);
      } // ch loop
    }

    //
    // Reset the DIMMs incase Tx Vref got set to a failing voltage
    //
    if (vGroup == TxVref) {
      IO_Reset (Host, socket);
      if (IsDdr5Present (Host, socket) == FALSE) {
        // Execute JEDEC reset/init sequence
        JedecInitSequence (Host, socket, CH_BITMASK);
      }
    }

    //
    // Increment the index into the results array
    //
    vIndex++;

  } // margin loop

  if (!(CheckCMDGroup (group) || CheckCTLGroup (group))) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      // Check if this channel is masked off
      if (chMask & (1 << ch)) {
        continue;
      }

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          if (rankMask & (1 << GetLogicalRank (Host, socket, ch, dimm, rank))) {
            continue;
          }

          // Skip setting TXVREF multiple times on LRDIMMs
          if (IsLrdimmPresent (socket, ch, dimm)&& (vGroup == TxVref) && (level == DdrLevel) && (rank > 0)) {
            continue;
          }
          if (!IsLrdimmPresent (socket, ch, dimm)&& (level == LrbufLevel)) {
            continue;
          }
          //
          // Restore Vref
          //
          if (((vGroup != RxVref) && (vGroup != RxSamplerEvenOdd)) || ((dimm == 0) && (rank == 0) && (level == DdrLevel)) || ((level == LrbufLevel) && (rank == 0))) {
            RestoreOffset (Host, socket, ch, dimm, rank, 0, level, vGroup);
          }
        } // rank loop
      } // dimm loop
    } // ch loop
  }

  SetDebugLevel (PrevDebugLevel);
} // GetMultiVref

UINT32
CollectTestResults (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    chToCheck,
  UINT32    bwSerr[MAX_CH][3]
  )
/*++
  Collects the results of the previous test

  @param Host         - Pointer to sysHost
  @param socket       - Processor socket to check
  @param chToCheck    - Channel to check
  @param bwSerr       - Bit wise error status

  @retval status      - Non-zero if any failure was detected

--*/
{
  UINT8                 ch;
  UINT32                status = SUCCESS;
  CPGC_ERROR_STATUS     cpgcErrorStatus[MAX_CH];
  struct  channelNvram  (*channelNvList)[MAX_CH];
  UINT8                 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);

  // Collect test results
  CpgcAdvTrainingErrorStatus (Host, socket, chToCheck, cpgcErrorStatus);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    // No need to check this channel if it's bit isn't set
    if ((chToCheck & (1 << ch))) {

      bwSerr[ch][0] = cpgcErrorStatus[ch].cpgcErrDat0S | cpgcErrorStatus[ch].cpgcErrDat2S;
      bwSerr[ch][1] = cpgcErrorStatus[ch].cpgcErrDat1S | cpgcErrorStatus[ch].cpgcErrDat3S;
      if (Host->nvram.mem.eccEn) {
        bwSerr[ch][2] = (cpgcErrorStatus[ch].cpgcErrEccS & 0xFF) | ((cpgcErrorStatus[ch].cpgcErrEccS >> 8) & 0xFF);
      } else {
        bwSerr[ch][2] = 0;
      }

      status |= bwSerr[ch][0];
      status |= bwSerr[ch][1];
      status |= bwSerr[ch][2];
    }
  } // ch loop

  return  status;
} // CollectTestResults

/**

  Get training result status. This function will use the last Level
  and TestType parameters given by SetupTest for the channel.

  @param[in] Host              - Pointer to sysHost
  @param[in] Socket            - Socket number
  @param[in] MemSs             - MemSS number within the socket (0-based)
  @param[in] ChannelMask       - Channel within MemSS (0-based)
  @param[in] SubChMask         - Sub Channel Mask within MemSS (0-based)
  @param[in] StatusArraySize   - Input giving status array size
  @param[in] *Status           - Pointer to caller buffer to be filled in with results
  @param[in] *ValidMask        - Each bit represents valid entry in Status array
  @param[in] ChunkMask         - A set bit indicates the chunk of the cache line will be included

  @retval MRC_STATUS

**/
MRC_STATUS
GetTrainResultStatus (
  IN      PSYSHOST     Host,
  IN      UINT8        Socket,
  IN      UINT32       MemSs,
  IN      UINT32       ChannelMask,
  IN      UINT8        SubChMask,
  IN      UINT32       StatusArraySize,
  IN OUT  UINT32       *Status,
  IN OUT  UINT32       *ValidMask,
  IN      UINT8        ChunkMask,
  IN      UINT8        Dimm
  )
{
  UINT8                 ch;
  UINT8                 i = 0;
  UINT32                returnstatus = SUCCESS;
  CPGC_ERROR_STATUS     cpgcErrorStatus[MAX_CH];
  struct                channelNvram (*channelNvList)[MAX_CH];
  UINT32                bwSerr[MAX_CH][3];
#ifdef DDR5_SUPPORT
  UINT32                CsrValue = 0;
  UINT8                 Strobe;
  UINT8                 SubChannel;
  UINT32                (*DqSampleResultStrobe) [SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT8                 SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  struct dimmNvram (*dimmNvList) [MAX_DIMM];
#endif
  UINT8                 MaxChDdr;
  MRC_TT                CurrentTestType;

  CurrentTestType = GetCurrentTestType (Socket);
  MaxChDdr        = GetMaxChDdr ();
  channelNvList   = GetChannelNvList (Host, Socket);
  ZeroMem (cpgcErrorStatus, sizeof (cpgcErrorStatus));
#ifdef DDR5_SUPPORT
  DqSampleResultStrobe = (VOID*) Status;
  //
  // EarlyCsLoopbackTest and EarlyCaLoopbackTest
  //
  if ((CurrentTestType == EarlyCsLoopbackTest) ||
      (CurrentTestType == EarlyCaLoopbackTest) ||
      (CurrentTestType == BcomTrainingTest) ||
      (CurrentTestType == EarlyCsBacksideLoopbackTest) ||
      (CurrentTestType == EarlyCaBacksideLoopbackTest)) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((ChannelMask & (1 << ch)) == 0) {
        continue;
      }
      if (StatusArraySize != sizeof ((*DqSampleResultStrobe))) {
        RcDebugPrint (SDBG_MINMAX, "Array Size is not enough\n");
        FatalError (ERR_MRC_STRUCT, ERR_ARRAY_OUT_OF_BOUNDS);
      }
      dimmNvList = GetDimmNvList (Host, Socket, ch);
      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
          if (IsDimmStrobeNotValidDdr5 (&(*dimmNvList) [Dimm], Strobe)) {
            continue;
          }
          CsrValue = ReadTrainFeedbackDdr5 (Host, Socket, ch, SubChannel, Strobe, Dimm);
          (*DqSampleResultStrobe) [SubChannel][Strobe] = CsrValue;
          if (Host->nvram.mem.eccEn) { //DDR5_TODO BRS
            *ValidMask = 0xFFFFF;
          } else {
            *ValidMask = 0xFFFF;//DDR5_TODO
          }
        } // Strobe loop
      }//subchannel loop
    } //ch loop
    return MRC_STATUS_SUCCESS;
  } else if ((CurrentTestType == EarlyRcdCsLoopbackTest) ||
             (CurrentTestType == EarlyRcdCaLoopbackTest) ||
             (CurrentTestType == DcaDckDutyCycleTest) ||
             (CurrentTestType == DcaDfeDdr5Test) ||
             (CurrentTestType == CaSlewRateTest)) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((ChannelMask & (1 << ch)) == 0) {
        continue;
      }
      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        if ((SubChMask & (1 << SubChannel)) == 0) {
          continue;
        }
        CsrValue = ReadTrainFeedbackDdr5(Host, Socket, ch, SubChannel, 0, Dimm); //SPR_TODO. HSD 1507170812 Update it to support in parallel.
      }
    }
    *Status = CsrValue;
    return MRC_STATUS_SUCCESS;
  }
#endif //DDR5_SUPPORT

  // Collect test results
  CpgcAdvTrainingErrorStatus (Host, Socket, ChannelMask, cpgcErrorStatus);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    // No need to check this channel if it's bit isn't set
    if ((ChannelMask & (1 << ch))) {
      bwSerr[ch][0] = 0;
      bwSerr[ch][1] = 0;
      bwSerr[ch][2] = 0;

      if (DdrtReadPendingQueueTimedOut (Host, Socket, ch)) {
        // Indicate a failure on all available bits if the RPQ had a timeout on this channel,
        // since individual lanes didn't fail.
        bwSerr[ch][0] = (UINT32) -1;
        bwSerr[ch][1] = (UINT32) -1;
        bwSerr[ch][2] = (UINT32) -1;
      } else {
        if ((ChunkMask & 0x55) != 0) {
          bwSerr[ch][0] |= cpgcErrorStatus[ch].cpgcErrDat0S;
          bwSerr[ch][1] |= cpgcErrorStatus[ch].cpgcErrDat1S;
        }
        if ((ChunkMask & 0xAA) != 0) {
          bwSerr[ch][0] |= cpgcErrorStatus[ch].cpgcErrDat2S;
          bwSerr[ch][1] |= cpgcErrorStatus[ch].cpgcErrDat3S;
        }
        bwSerr[ch][2] = 0;
        if (Host->nvram.mem.eccEn) {
          if ((ChunkMask & 0x55) != 0) {
            bwSerr[ch][2] |= (cpgcErrorStatus[ch].cpgcErrEccS & 0xFF);
          }
          if ((ChunkMask & 0xAA) != 0) {
            bwSerr[ch][2] |= ((cpgcErrorStatus[ch].cpgcErrEccS >> 8) & 0xFF);
          }
        }
      } // if (DdrtReadPendingQueueTimedOut (Host, Socket, ch))

      returnstatus |= bwSerr[ch][0];
      returnstatus |= bwSerr[ch][1];
      returnstatus |= bwSerr[ch][2];
    }
  } // ch loop
  for (ch = 0; ch < MaxChDdr; ch++) {
    for (i = 0; i < 3; i++) {
      *(Status + ch*3 + i) = bwSerr[ch][i];
    }
  }

  //
  // Indicate valid status
  //
  SetMem32 (ValidMask, StatusArraySize, 0x1);

  return  returnstatus;

}

UINT8
CountBitsEqOne (
  UINT32 Input
  )
/*++

  This function calculates the number of bits set to 1 in a 32-bit value

  @param Input           - the value to work on

  @retval the number of bits set to 1 in Input

--*/
{
  UINT8 NumOnes;

  NumOnes = 0;
  while (Input > 0) {
    NumOnes++;
    Input &= (Input - 1);
  }

  return NumOnes;
} // CountBitsEqOne

UINT32
XOR (
  UINT32 a,
  UINT32 b
  )
{
  if (a == b) {
    return 0;
  } else{
    return 1;
  }
} // XOR

/*++

  Calculate the next LFSR based on the inputs

  @param lfsr         - Current lfsr
  @param lfsrPoly     - Polynomial taps
  @param lfsrOrder    - order

  @retval returns updated lfsr

--*/
UINT32
NextLFSROut (
  UINT32 lfsr,
  UINT32 lfsrPoly,
  UINT32 lfsrOrder
  )
{
  UINT32  order;
  UINT32  NorOut;
  UINT32  NorCount;
  UINT32  SelectBit = 0x01;
  UINT32  feedback;
  UINT32  next_lfsr;
  UINT32  temp_lfsr;

  NorOut = 0;
  NorCount = 0;
  temp_lfsr = lfsr;

  for (order = 0; order < lfsrOrder - 1; order++) {
    if ((temp_lfsr & SelectBit) == 0) {
      NorCount++;
    }

    temp_lfsr = temp_lfsr >> 1;
  } // order loop

  if (NorCount == (lfsrOrder - 1)) {
    NorOut = 1;
  }

  feedback = XOR (NorOut, (SelectBit & (lfsr >> (lfsrOrder - 1))));
  next_lfsr = 0x0;

  for (order = 1; order < lfsrOrder; order++) {
    next_lfsr = next_lfsr + XOR (((lfsr >> (lfsrOrder - order - 1)) & SelectBit),
                              (((lfsrPoly >> (lfsrOrder - order)) & SelectBit) * feedback));
    next_lfsr = next_lfsr << 1;
  } // order loop

  next_lfsr = next_lfsr + feedback;

  return next_lfsr;
} // NextLFSROut

/*++
  Evaluates (based on ODT table) if there are any detected issues
  with strobes and if so whether the set of issues form correctable
  or non-correctable error

  @param Host    - Pointer to sysHost
  @param socket  - Socket
  @param ch      - Channel

  @retval returns status -> FPT_NO_ERROR, FPT_CORRECTABLE_ERROR, FPT_NO_CORRECTABLE_ERROR

--*/
UINT8
EvaluateFaultyParts (
  PSYSHOST Host,
  UINT8     socket,
  UINT8     ch
  )
{
  UINT8             found_cnt = 0;
  UINT8             strobe;
  UINT8             aux_strobe;
  UINT8             rank;
  UINT8             dimm;
  UINT8             previous_faulty_strobe[MAX_STROBE];
  UINT8             status;
  UINT8             non_correctable_get_out = 0;
  struct dimmNvram  (*dimmNvList)[MAX_DIMM];
  struct ddrRank    (*rankList)[MAX_RANK_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  SYS_SETUP         *Setup;

  Setup = GetSysSetupPointer ();

  status = FPT_NO_ERROR;
  if (GetEmulation () & SIMICS_FLAG) {
    return status;
  }
  /*
for (dimm1 = 0; dimm1 < MSVx4; dimm1++)  {
    previous_faulty_dimm[dimm] = 0xFF;
  }
for (rank1 = 0; rank1 < MSVx4; rank1++)  {
    previous_faulty_rank[rank] = 0xFF;
  }
   */
  SetMem (previous_faulty_strobe, MSVx4, 0xFF);

  dimmNvList = GetDimmNvList (Host, socket, ch);

  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    if (non_correctable_get_out) {
      break;
    }

    if ((*dimmNvList)[dimm].dimmPresent == 0) {
      continue;
    }

    rankList = GetRankNvList (Host, socket, ch, dimm);
    for (rank = 0; rank< MAX_RANK_DIMM; rank++) {
      if (non_correctable_get_out) {
        break;
      }

      // Continue to the next rank if this one is disabled
      if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
        continue;
      }

      // let's first check all the strobes
      for (strobe = 0; strobe < MSVx4; strobe++) {
        if (non_correctable_get_out) {
          break;
        }
        // is there strobe with marked failing part
        if ((*rankList)[rank].faultyParts[strobe] > 0) {
          // there is failing part and if there is no ECC it is non-correctable error right away
          if (Host->nvram.mem.eccEn) {
            if (found_cnt > 0) {
              // this is already second strobe failing - can still be ok only if we are lock step mode and nibbles are next to each other
              if (ChipLockstepEn (Host)) {
                RcDebugPrint (SDBG_MAX,
                               "EFP LOCKSTEP \n");
                //do some more analysis later - fail for now
                status = FPT_NO_CORRECTABLE_ERROR;
                non_correctable_get_out = 1;
                break;
              } else {
                // this is already second strobe failing and since we are NOT in lock step mode on x8
                // we need to check for the ranks vs strobe failures
                // 2 different strobes on the same rank - non correctable
                // 2 same strobes on different ranks - correctable
                // 2 different strobes on two different ranks - possibly correctable evaluated based on ODT matrix
                //    if both strobe-rank are terminating - non correctable
                //    if only one strobe-rank is terminating - correctable
                RcDebugPrint (SDBG_MAX,
                               "EFP 2 strobes failing \n");

                for (aux_strobe = 0; aux_strobe < found_cnt; aux_strobe++) {
                  if (previous_faulty_strobe[aux_strobe] != 0xFF) {
                    if (strobe == previous_faulty_strobe[aux_strobe]) {
                      status = FPT_CORRECTABLE_ERROR;
                      if ( 0 == (Setup->mem.optionsExt & ALLOW_CORRECTABLE_ERROR)) {// Disable the channel on SBE during the training.
                        status = FPT_NO_CORRECTABLE_ERROR;
                        non_correctable_get_out = 1;
                        break;
                      } // if ALLOW_CORRECTABLE_ERROR
                    } else {
                      status = FPT_NO_CORRECTABLE_ERROR;
                      non_correctable_get_out = 1;
                      break;
                    } //  if previous_faulty_strobe
                  } //previous_faulty_strobe[aux_strobe]
                } // aux_strobe loop
              } // if LOCKSTEP
            } else {
              // this is the first strobe failing and since there is ECC it is correctable
              RcDebugPrint (SDBG_MAX,
                             "EFP ODT based eval expected\n");

              status = FPT_CORRECTABLE_ERROR;
              if ( 0 == (Setup->mem.optionsExt & ALLOW_CORRECTABLE_ERROR)) {// Disable the channel on SBE during the training.
                status = FPT_NO_CORRECTABLE_ERROR;
                non_correctable_get_out = 1;
                break;
              } // if ALLOW_CORRECTABLE_ERROR

              //
              // Disable the channel since DCPMM is not support FPT
              //
              if ((*dimmNvList)[dimm].DcpmmPresent) {
                status = FPT_NO_CORRECTABLE_ERROR;
                non_correctable_get_out = 1;
                break;
              } // if DCPMM is present
            } // found_cnt
          } else {
            // no ECC it is non-correctable error right away
            RcDebugPrint (SDBG_MAX,
                           "EFP NO ECC\n");
            status = FPT_NO_CORRECTABLE_ERROR;
            non_correctable_get_out = 1;
            break;
          }
          //previous_faulty_dimm[found_cnt] = dimm;
          //previous_faulty_rank[found_cnt] = rank;
          previous_faulty_strobe[found_cnt] = strobe;
          found_cnt++;
          if (MSVx4 == found_cnt) {
            status = FPT_NO_CORRECTABLE_ERROR;
            non_correctable_get_out = 1;
            break;
          }
        } // if faulty part
      } // strobe loop
    } // rank loop
  } // dimm loop

  // only print if non zero
  if (status != FPT_NO_ERROR) {
    RcDebugPrint (SDBG_MAX,
                  "EFP status: %d\n", status);
  }
  return (status);
} // EvaluateFaultyParts

#if 0
/*++

  Centers Tx Vref then Tx timing

  @param Host    - Pointer to sysHost

  @retval N/A

--*/
VOID
CenterTx (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     flag
  )
{
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               strobe;
  INT16               offset;
  UINT32              patternLength;
  UINT16              mode;
  UINT32              PrevDebugLevel;
  struct bitMask      filter;
  struct rankMargin   vrefMargin;
  struct strobeMargin results;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);

  channelNvList = GetChannelNvList (Host, socket);
  mode = MODE_VIC_AGG;
  patternLength = 64;

  //
  // Clear the filter bits to enable error checking on every bit
  //
  ClearFilter (Host, socket, &filter);

  if (flag & FIRST_TIMING) {
    //
    // Center Tx Timing
    //
    for (ch = 0; ch < MaxChDdr; ch++) {
      for (rank = 0; rank < MAX_RANK_CH; rank++) {
        for (strobe = 0; strobe < MSVx4; strobe++) {
          results.strobe[ch][rank][strobe].n = 0;
          results.strobe[ch][rank][strobe].p = 0;
        } // strobe loop
      } // rank loop
    } // ch loop

    GetMargins (Host, socket, DdrLevel, TxDqDelay, mode, SCOPE_STROBE, &filter, &results, patternLength, 0, 0, 0,
      WDB_BURST_LENGTH);

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      dimmNvList = GetDimmNvList (Host, socket, ch);

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        //
        // Skip if no DIMM present
        //
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }

        rankList = GetRankNvList (Host, socket, ch, dimm);

        for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
          //
          // Skip if no rank
          //
          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          for (strobe = 0; strobe < MSVx4; strobe++) {
            offset = (results.strobe[ch][ (*rankList)[rank].rankIndex][strobe].p + results.strobe[ch][ (*rankList)[rank].rankIndex][strobe].n) / 2;
            GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, ALL_BITS, DdrLevel, TxDqDelay, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET, &offset);
          } // strobe loop
        } // rank loop
      } // dimm loop
    } // ch loop
  }

  if (flag & SECOND_VREF) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      for (rank = 0; rank < MAX_RANK_CH; rank++) {
        vrefMargin.rank[ch][rank].n = 0;
        vrefMargin.rank[ch][rank].p = 0;
      } // rank loop
    } // ch loop

    //
    // Center Tx Vref
    //
    GetMargins (Host, socket, DdrLevel, TxVref, mode, SCOPE_RANK, &filter, &vrefMargin, patternLength, 0, 0, 0,
      WDB_BURST_LENGTH);

    PrevDebugLevel = GetDebugLevel ();
    if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
      SetDebugLevel (SDBG_ERROR);
    }

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      dimmNvList = GetDimmNvList (Host, socket, ch);

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        //
        // Skip if no DIMM present
        //
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }

        rankList = GetRankNvList (Host, socket, ch, dimm);

        for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
          //
          // Skip if no rank
          //
          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          offset = (vrefMargin.rank[ch][ (*rankList)[rank].rankIndex].p + vrefMargin.rank[ch][ (*rankList)[rank].rankIndex].n) / 2;
          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, ALL_BITS, DdrLevel, TxVref, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET, &offset);
        } // rank loop
      } // dimm loop
    } // ch loop
  }

  if (flag & SECOND_TIMING) {
    //
    // Center Tx Timing
    //
    for (ch = 0; ch < MaxChDdr; ch++) {
      for (rank = 0; rank < MAX_RANK_CH; rank++) {
        for (strobe = 0; strobe < MSVx4; strobe++) {
          results.strobe[ch][rank][strobe].n = 0;
          results.strobe[ch][rank][strobe].p = 0;
        } // strobe loop
      } // rank loop
    } // ch loop

    GetMargins (Host, socket, DdrLevel, TxDqDelay, mode, SCOPE_STROBE, &filter, &results, patternLength, 0, 0, 0,
      WDB_BURST_LENGTH);

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      dimmNvList = GetDimmNvList (Host, socket, ch);

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        //
        // Skip if no DIMM present
        //
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }

        rankList = GetRankNvList (Host, socket, ch, dimm);

        for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
          //
          // Skip if no rank
          //
          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          for (strobe = 0; strobe < MSVx4; strobe++) {
            offset = (results.strobe[ch][ (*rankList)[rank].rankIndex][strobe].p + results.strobe[ch][ (*rankList)[rank].rankIndex][strobe].n) / 2;
            GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, ALL_BITS, DdrLevel, TxDqDelay, GSM_UPDATE_CACHE | GSM_WRITE_OFFSET, &offset);
          } // strobe loop
        } // rank loop
      } // dimm loop
    } // ch loop
  }

  SetDebugLevel (PrevDebugLevel);
} // CenterTx
#endif // if 0

/*++

  CoreSetVdd - Call an OEM hook to change the socket Vddq

  @param[in] Host  - Pointer to sysHost
  @param[in] socket  - processor number

  @retval SUCCESS if the Vddq change was made
  @retval FAILURE if the Vddq change was not made

--*/

UINT8
CoreSetVdd (
  IN PSYSHOST Host,
  IN UINT8    socket
  )
{
  EFI_STATUS Status;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    return MrcPlatformHooksInterface->PlatformSetVdd (socket, &Host->nvram.mem.socket[socket].ddrVoltage);
  }

  return SUCCESS;
}

VOID
CoreReleaseADRClamps (
  PSYSHOST Host,
  UINT8 socket
  )
/*++

  Core to OEM platform hook call to release any platform clamps affecting CKE
  and/or DDR Reset

  @param Host      - pointer to sysHost structure

--*/
{
  EFI_STATUS Status;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    MrcPlatformHooksInterface->PlatformReleaseADRClamps (socket);
  }

}

/*++

  CoreCheckPORCompat - Core to platform call of an OEM hook to restrict supported configurations if required.

  @param[in] Host  - Pointer to sysHost

  @retval N/A

--*/

VOID
CoreCheckPORCompat (
  IN PSYSHOST Host
  )
{
  EFI_STATUS Status;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    MrcPlatformHooksInterface->PlatformCheckPORCompat (Host);
  }
}

/*++

  CoreLookupFreqTable - core call to OEM hook for overriding the memory POR frequency table

  @param[in] Host            - Pointer to sysHost
  @param[in] freqTablePtr    - Pointer to Intel POR memory frequency table
  @param[in] freqTableLength - Pointer to number of DimmPOREntryStruct entries in POR Frequency Table

  @retval N/A

--*/

VOID
CoreLookupFreqTable (
  IN PSYSHOST Host,
  OUT struct DimmPOREntryStruct **freqTablePtr,
  OUT UINT16 *freqTableLength
  )
{
  EFI_STATUS Status;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    MrcPlatformHooksInterface->PlatformLookupFreqTable (freqTablePtr, freqTableLength);
  }

}

/**
  Calculate the encode strobe value according to DQS and DQ FUBs

  @param[in]  Socket    The socket containing the strobe of interest
  @param[in]  Channel   The channel containing the strobe of interest
  @param[in]  Dimm      The DIMM containing the strobe of interest
  @param[in]  Strobe    The strobe of interest

  @retval   The encoded strobe number, if it could be calculated
  @retval   NO_STROBE, if the encoded value could not be calculated
**/
UINT8
EFIAPI
StrobeEncoding (
  IN  UINT8 Socket,
  IN  UINT8 Channel,
  IN  UINT8 Dimm,
  IN  UINT8 Strobe
  )
{
  PSYSHOST          Host;
  UINT8             EncodedStrobe;
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];
  UINT8             MSVx4;

  if ((Socket == NO_SOCKET) || (Channel == NO_CH) || (Dimm == NO_DIMM) || (Strobe == NO_STROBE)) {

    EncodedStrobe = NO_STROBE;

  } else {

    Host = (PSYSHOST) GetSysHostPointer ();
    MSVx4 = GetMaxStrobeValid (Host);
    DimmNvList = GetDimmNvList (Host, Socket, Channel);

    if ((*DimmNvList)[Dimm].x4Present) {
      if (Strobe % 2) {
        EncodedStrobe = (Strobe + (MSVx4 - 1)) / 2;
      } else {
        EncodedStrobe = Strobe / 2;
      }
    } else {
      EncodedStrobe = Strobe;
    }
  }

  return EncodedStrobe;
}
