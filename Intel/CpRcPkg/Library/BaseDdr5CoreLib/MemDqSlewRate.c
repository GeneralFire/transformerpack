/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include "Ddr5Core.h"
#include <Library/Ddr5CoreLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Memory/JedecDefinitions.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <MemHostChipCommon.h>
#include <Library/MemSweepLib.h>
#include <Base.h>

#define MAX_DQ_SLEW_RATE_DELTA    2 // This is a TBD value which needs to be readjusted on real hardware.

#define INVALID_SLEW_RATE         0x5F
#define MAX_DQ_SLEW_RATE_SAMPLES  31

#define STROBE_INDEX_SUBCH(STROBE, SUBCH) (STROBE + SUBCH * (MAX_STROBE_DDR5 / 2))

//
// Scomp to be swept
//
// rise-fall                                         -15 -14 -13 -12 -11 -10  -9  -8  -7  -6  -5  -4  -3  -2  -1   0    1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
//
UINT8  mDataScomp[MAX_DQ_SLEW_RATE_SAMPLES]        = {8,   8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 8, 8, 8, 8, 8};
UINT8  mDataScompUpAdj[MAX_DQ_SLEW_RATE_SAMPLES]   = {8,   8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  0,  0,  0,  0,  0,  1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7};
UINT8  mDataScompDownAdj[MAX_DQ_SLEW_RATE_SAMPLES] = {7,   6,  5,  4,  3,  2,  1,  0,  7,  6,  5,  4,  3,  2,  1,  0,  0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 8, 8, 8, 8, 8};

typedef struct ScompSetting SCOMP_SETTING;

struct ScompSetting {
  UINT8  Scomp;
  UINT8  ScompUpAdj;
  UINT8  ScompDownAdj;
};


/**

  Set Tx Vref per DRAM device.

  @param  Socket   Socket number.
  @param  Mode     Read/Write mode.
  @parm   Vref     The Tx vref value to be set.

  @retval VOID

**/
VOID
SetTxVref (
  IN UINT8  Socket,
  IN UINT8  Mode,
  IN INT16  (*Vref)[MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE/2]
  )
{
  UINT8                           Ch;
  UINT8                           SubCh;
  UINT8                           Dimm;
  UINT8                           Rank;
  UINT8                           RankIndex;
  UINT8                           Strobe;
  INT16                           DramVref;
  UINT8                           MaxStrobe;
  PSYSHOST                        Host;
  UINT8                           MaxChDdr;

  Host = (PSYSHOST) GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr;Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm ++) {

      if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
        continue;
      }

      for (Rank = 0; Rank < GetNumberOfRanksOnDimm (Socket, Ch, Dimm); Rank ++) {

        if (IsRankValid (Socket, Ch, Dimm, Rank, TRUE) == FALSE) {
          continue;
        }

        RankIndex = GetRankIndex(Socket, Ch, Dimm, Rank);

        MaxStrobe = GetMaxStrobe(Host, Socket, Ch, Dimm, TxVref, DdrLevel);

        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          for (Strobe = 0; Strobe < MaxStrobe; Strobe ++) {

            if (IsStrobeNotValidDdr5 (Host, Strobe) == TRUE) {
              continue;
            }

            DramVref = (*Vref) [Ch][SubCh][RankIndex][Strobe];
            GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, Strobe, ALL_BITS, DdrLevel, TxVref, Mode, &DramVref);
          }
        }
      }
    }
  }
}


/**
  Get Tx DQ Vref margin. This function returns nominal Vref, 70% of Max Vref and 70% of Min Vref of each strobe.

  @param  Host          Pointer to sysHost.
  @param  VrefNominal   Nominal Vref of each DQ strobe.
  @param  VrefMax0p7    70% of Max Vref.
  @param  VrefMin0p7    70% of Min Vref.

  @retval FAILURE       Fails to get Vref margin.
  @retval SUCESS        Succeed to get Vref margin.
**/
UINT32
GetDqVrefMargin (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN OUT  INT16     (*VrefNominal)[MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE/2],
  IN OUT  INT16     (*VrefMax0p7)[MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE/2],
  IN OUT  INT16     (*VrefMin0p7)[MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE/2]
  )
{
  UINT8   Ch;
  UINT8   SubCh;
  UINT8   Dimm;
  UINT8   Rank;
  UINT8   MaxChDdr;
  UINT8   VrefStrobe;
  UINT8   VrefStrobeInCh;
  UINT8   RankIndex;
  UINT8   VrefMaxStrobe;
  INT16   Vref;
  struct  strobeMargin *VrefMargin;

  MaxChDdr = GetMaxChDdr ();

  VrefMargin = RcAllocatePool (sizeof (struct strobeMargin));
  ZeroMem ((UINT8 *) (VrefMin0p7), sizeof (*VrefMin0p7));

  //
  // Get Vref Margin (Vref Max and Vref Min).
  GetAdvDataMargins (
    Socket, DdrLevel, TxVref, ResultsRelative,
    VrefMargin, NULL, NULL,
    NULL, NULL, NULL
    );

  RcDebugPrint(SDBG_MEM_TRAIN, "                         VrefMax0p7    VrefNominal    VrefMin0p7\n");

  //
  // Get Vref Nominal, 70% Vref Max and 70% Vref Min of DQ.
  //
  for (Ch = 0; Ch < MaxChDdr;Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm ++) {

      //
      // TxVref is x4 or x8 strobe, depending on DRAM DQ width.
      // TxDqRiseSlewRate is always x4 strobe.
      //
      VrefMaxStrobe = GetMaxStrobeWithinSubCh (Host, Socket, Ch, Dimm, TxVref, DdrLevel);

      if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
        continue;
      }

      for (Rank = 0; Rank < GetNumberOfRanksOnDimm (Socket, Ch, Dimm); Rank ++) {

        if (IsRankValid (Socket, Ch, Dimm, Rank, TRUE) == FALSE) {
          continue;
        }

        RankIndex = GetRankIndex(Socket, Ch, Dimm, Rank);

        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          for (VrefStrobe = 0; VrefStrobe < VrefMaxStrobe; VrefStrobe ++) {

            VrefStrobeInCh = STROBE_INDEX_SUBCH(VrefStrobe, SubCh);

            if (IsStrobeNotValidDdr5 (Host, VrefStrobe) == TRUE) {
              continue;
            }

            //
            // Get Vref nominal. (This is the absolute Vref linear value)
            //
            GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, VrefStrobe, ALL_BITS, DdrLevel, TxVref, GSM_READ_ONLY, &Vref);
            (*VrefNominal) [Ch][SubCh][RankIndex][VrefStrobe] = Vref;

            //
            // 70% Vref Max. (This is the absolute Vref linear value)
            //
            (*VrefMax0p7) [Ch][SubCh][RankIndex][VrefStrobe] = (*VrefNominal) [Ch][SubCh][RankIndex][VrefStrobe] + (VrefMargin->strobe[Ch][RankIndex][VrefStrobeInCh]).p * 7 / 10;

            //
            // 70% Vref Min. (This is the absolute Vref linear value)
            //
            (*VrefMin0p7) [Ch][SubCh][RankIndex][VrefStrobe] = (*VrefNominal) [Ch][SubCh][RankIndex][VrefStrobe] + (VrefMargin->strobe [Ch][RankIndex][VrefStrobeInCh]).n * 7 / 10;

            RcDebugPrintWithDevice (
              SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, VrefStrobe, NO_BIT, "       %3d            %3d            %3d \n",
              (*VrefMax0p7) [Ch][SubCh][RankIndex][VrefStrobe],
              (*VrefNominal) [Ch][SubCh][RankIndex][VrefStrobe],
              (*VrefMin0p7) [Ch][SubCh][RankIndex][VrefStrobe]
              );
          }
        }
      }
    }
  }

  RcFreePool (VrefMargin);

  return MRC_STATUS_SUCCESS;
}

/**
  Prints warning messages if an error was detected on Timing values 2, 3, or 5.

  @param[in]  Timing2Error  - True, if there was an error detected with timing value 2
  @param[in]  Timing3Error  - True, if there was an error detected with timing value 3
  @param[in]  Timing5Error  - True, if there was an error detected with timing value 5

  @retval N/A
**/
VOID
PrintTimingWarnings (
  IN BOOLEAN  Timing2Error,
  IN BOOLEAN  Timing3Error,
  IN BOOLEAN  Timing5Error
  )
{
  RcDebugPrint (SDBG_MAX,  "     WARNING: ");

  if (Timing2Error) {
    RcDebugPrint (SDBG_MAX,  "  T2 <= T6 ");
  }

  if (Timing3Error) {
    RcDebugPrint (SDBG_MAX,  "  T3 <= T6 ");
  }

  if (Timing5Error) {
    RcDebugPrint (SDBG_MAX,  "  T5 <= T1 or T4");
  }

  RcDebugPrint (SDBG_MAX,  "  Values adjusted");
}

/*
  Get per-nibble max slew rate delta.

  @param[in]      Host                       System Host..
  @param[in]      Socket                     Current socket ID.
  @param[in]      VrefNominal                Vref norminal per strobe.
  @param[in]      VrefMax0p7                 70% Vref Max per strobe.
  @param[in]      VrefMin0p7                 70% Vref Min per strobe.
  @param[in]      TimingMarginNorminal       Per bit timing margin under Vref nominal.
  @param[in]      TimingMarginVrefMax0p7     Per bit timing margin under 70% Vref Max.
  @param[in]      TimingMarginVrefMin0p7     Per bit timing margin under 70% Vref Min.
  @param[out]     MaxSlewRateDeltaPerStrobe  Per strobe max slew rate delta.
  @param[in out]  SlewRateTrainingDone       Per strobe indication if slew rate training is done.
  @param[in]      LoopCount                  Slew rate training loop count.

  @retval FAILURE Fails to get max slew rate delta.
  @retval SUCESS  Succeed to get max slew rate delta.

*/
MRC_STATUS
CalculateMaxSlewRateDeltaPerNibble (
  IN     PSYSHOST          Host,
  IN     UINT8             Socket,
  IN     INT16             (*VrefNominal)[MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE/2],
  IN     INT16             (*VrefMax0p7)[MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE/2],
  IN     INT16             (*VrefMin0p7)[MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE/2],
  IN     struct bitMargin  *TimingMarginNorminal,
  IN     struct bitMargin  *TimingMarginVrefMax0p7,
  IN     struct bitMargin  *TimingMarginVrefMin0p7,
  OUT    INT16             (*MaxSlewRateDeltaPerStrobe)[MAX_CH][SUB_CH][MAX_STROBE/2],
  IN OUT BOOLEAN           (*SlewRateTrainingDone)[MAX_CH][SUB_CH][MAX_STROBE/2],
  IN     UINT8             LoopCount
  )
{
  UINT8     Ch;
  UINT8     SubCh;
  UINT8     Dimm;
  UINT8     Rank;
  UINT8     RankIndex;
  UINT8     ScompStrobe;
  UINT8     ScompMaxStrobe;
  UINT8     VrefStrobe;
  UINT8     DqNibble;
  UINT8     DqBitIndexInCh;
  UINT8     MaxChDdr;
  INT16     SlewRateRisingEdge;
  INT16     SlewRateFallingEdge;
  INT16     UpperHalfVrefDelta;
  INT16     LowerHalfVrefDelta;
  UINT8     Bit;
  INT16     T1[BITS_PER_NIBBLE];
  INT16     T2[BITS_PER_NIBBLE];
  INT16     T3[BITS_PER_NIBBLE];
  INT16     T4[BITS_PER_NIBBLE];
  INT16     T5[BITS_PER_NIBBLE];
  INT16     T6[BITS_PER_NIBBLE];
  INT16     (*RisingEdgeSlewRate)[MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE/2][BITS_PER_NIBBLE];
  INT16     (*FallingEdgeSlewRate)[MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE/2][BITS_PER_NIBBLE];
  INT16     (*SlewRateDelta)[MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE/2][BITS_PER_NIBBLE]; // (Rising Edge - Falling Edge)
  BOOLEAN   T2Error;
  BOOLEAN   T3Error;
  BOOLEAN   T5Error;

  MaxChDdr = GetMaxChDdr ();

  RisingEdgeSlewRate = RcAllocatePool (sizeof (*RisingEdgeSlewRate));
  FallingEdgeSlewRate = RcAllocatePool (sizeof (*FallingEdgeSlewRate));
  SlewRateDelta = RcAllocatePool (sizeof (*SlewRateDelta));

  if ((RisingEdgeSlewRate == NULL) || (FallingEdgeSlewRate == NULL) || (SlewRateDelta == NULL)) {
    if (SlewRateDelta != NULL) {
      RcFreePool (SlewRateDelta);
    }

    if (FallingEdgeSlewRate != NULL) {
      RcFreePool (FallingEdgeSlewRate);
    }

    if (RisingEdgeSlewRate != NULL) {
      RcFreePool (RisingEdgeSlewRate);
    }

    RcDebugPrint (SDBG_MAX, "WARNING: Out of temporary memory space. DQ Slew Rate Training return without doing anything.\n");
    return MRC_STATUS_FAILURE;
  }

  ZeroMem ((UINT8 *) (RisingEdgeSlewRate), sizeof (*RisingEdgeSlewRate));
  ZeroMem ((UINT8 *) (FallingEdgeSlewRate), sizeof (*FallingEdgeSlewRate));
  ZeroMem ((UINT8 *) (SlewRateDelta), sizeof (*SlewRateDelta));

  RcDebugPrint (SDBG_MAX, "                         0.7VrefMax  T2(Le)  T1(Re)  |   VrefNominal  T6(Le)  T5(Re)   |   0.7VrefMin  T3(Le)  T4(Re)  |  Rising  Falling  Delta(Rising-Falling) \n");
  RcDebugPrintLine (SDBG_DEFAULT, 118, SINGLE_LINE, CRLF_FLAG, CRLF_FLAG);

  for (Ch = 0; Ch < MaxChDdr;Ch += GetChInc (Host)) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

      ScompMaxStrobe = GetMaxStrobeWithinSubCh(Host, Socket, Ch, Dimm, TxDqRiseSlewRate, DdrLevel);

      if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
        continue;
      }

      for (Rank = 0; Rank < GetNumberOfRanksOnDimm (Socket, Ch, Dimm); Rank++) {

        if (IsRankValid (Socket, Ch, Dimm, Rank, TRUE) == FALSE) {
          continue;
        }

        RankIndex = GetRankIndex (Socket, Ch, Dimm, Rank);
        for (SubCh = 0; SubCh < SUB_CH; SubCh ++) {

          for (DqNibble = 0; DqNibble < ScompMaxStrobe; DqNibble ++) {

            //
            // Map nibble to Scomp strobe (x4) number.
            //
            ScompStrobe = GetStrobeFromNibble (Host, DqNibble);

            //
            // Find the Tx Vref strobe number. (Vref strobe could be x4 or x8).
            //
            if (IsX4Dimm (Socket, Ch, Dimm)) {
              VrefStrobe = ScompStrobe;
            } else {
              //
              // x8 DIMM.
              //    x8 Vref Strobe      x4 Scomp Strobe
              //       0                   0, 5
              //       1                   1, 6
              //       2                   2, 7
              //       3                   3, 8
              //       4                   4, 9
              //
              VrefStrobe = ScompStrobe % (MAX_STROBE / 4);
            }

            if (IsStrobeNotValidDdr5 (Host, ScompStrobe) == TRUE) {
              continue;
            }

            //
            // Skip strobe that has been trained.
            //
            if ((*SlewRateTrainingDone)[Ch][SubCh][ScompStrobe] == TRUE) {
              continue;
            }

            for (Bit = 0; Bit < BITS_PER_NIBBLE; Bit++) {
              T2Error = FALSE;
              T3Error = FALSE;
              T5Error = FALSE;

              //
              //  Get timing margin under different Vref levels.
              //
              //              T2----------------T1                (Timing margin under 70% Vref Max)
              //         T6----------------------------T5         (Timing margin under Vref Nominal)
              //              T3----------------T4                (Timing margin under 70% Vref Min)
              //

              DqBitIndexInCh = (SubCh * (MAX_STROBE / 2) * BITS_PER_NIBBLE) + DqNibble * BITS_PER_NIBBLE + Bit;

              T5[Bit] = TimingMarginNorminal->bits[Ch][RankIndex][DqBitIndexInCh].p; // right edge
              T6[Bit] = TimingMarginNorminal->bits[Ch][RankIndex][DqBitIndexInCh].n; // left edge

              T1[Bit] = TimingMarginVrefMax0p7->bits[Ch][RankIndex][DqBitIndexInCh].p; // right edge
              T2[Bit] = TimingMarginVrefMax0p7->bits[Ch][RankIndex][DqBitIndexInCh].n; // left edge

              T4[Bit] = TimingMarginVrefMin0p7->bits[Ch][RankIndex][DqBitIndexInCh].p; // right edge
              T3[Bit] = TimingMarginVrefMin0p7->bits[Ch][RankIndex][DqBitIndexInCh].n; // left edge

              RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, ScompStrobe, Bit, "");
              RcDebugPrint (SDBG_MAX, " %3d         %3d    %3d  ", (*VrefMax0p7) [Ch][SubCh][RankIndex][VrefStrobe], T2[Bit], T1[Bit]); // Vref Max
              RcDebugPrint (SDBG_MAX, "           %3d      %3d    %3d  ", (*VrefNominal) [Ch][SubCh][RankIndex][VrefStrobe], T6[Bit], T5[Bit]); // Vref Nominal
              RcDebugPrint (SDBG_MAX, "            %3d       %3d    %3d  ", (*VrefMin0p7) [Ch][SubCh][RankIndex][VrefStrobe], T3[Bit], T4[Bit]); // Vref Min

              // Based on the equations for calculating SlewRateRising/Falling Edges below, ensure the following conditions
              // are not true. If any timing condition is violated, indicate an error for that value and adjust the value
              // such that the failing condition no longer holds.
              if ((T2[Bit] <= T6[Bit]) || (T3[Bit] <= T6[Bit]) || (T5[Bit] <= T1[Bit]) || (T5[Bit] <= T4[Bit])) {

                if (T2[Bit] <= T6[Bit]) {  // T2 should be > T6
                  T2[Bit] = T6[Bit] + 1;   // Adjust T2 to be > T6
                  T2Error = TRUE;
                }
                if (T3[Bit] <= T6[Bit]) {  // T3 should be > T6
                  T3[Bit] = T6[Bit] + 1;   // Adjust T3 to be > T6
                  T3Error = TRUE;
                }
                if (T5[Bit] <= T1[Bit]) {  // T5 should be > T1
                  T5[Bit] = T1[Bit] + 1;   // Adjust T5 to be > T1
                  T5Error = TRUE;
                }
                if (T5[Bit] <= T4[Bit]) {  // T5 should be > T4
                  T5[Bit] = T4[Bit] + 1;   // Adjust T5 to be > T4
                  T5Error = TRUE;
                }

                //
                // If an error has happened, stop training this bit.
                //
                (*SlewRateDelta)[Ch][SubCh][RankIndex][ScompStrobe][Bit] = 0x00;
              }

              //
              // Get the slew rate delta of each bit.
              // Note: TxVref could be x4 or x8 strobe, TxSlewRate is always x4.
              //
              UpperHalfVrefDelta = (*VrefMax0p7) [Ch][SubCh][RankIndex][VrefStrobe] - (*VrefNominal) [Ch][SubCh][RankIndex][VrefStrobe];
              LowerHalfVrefDelta = (*VrefNominal) [Ch][SubCh][RankIndex][VrefStrobe] - (*VrefMin0p7) [Ch][SubCh][RankIndex][VrefStrobe];
              SlewRateRisingEdge = UpperHalfVrefDelta/(T2[Bit] - T6[Bit]) + LowerHalfVrefDelta / (T5[Bit] - T4[Bit]);
              SlewRateFallingEdge = UpperHalfVrefDelta/(T5[Bit] - T1[Bit]) + LowerHalfVrefDelta / (T3[Bit] - T6[Bit]);

              (*RisingEdgeSlewRate)[Ch][SubCh][RankIndex][ScompStrobe][Bit] = SlewRateRisingEdge;
              (*FallingEdgeSlewRate)[Ch][SubCh][RankIndex][ScompStrobe][Bit] = SlewRateFallingEdge;

              (*SlewRateDelta)[Ch][SubCh][RankIndex][ScompStrobe][Bit] = ABS(SlewRateRisingEdge) - ABS(SlewRateFallingEdge);

              RcDebugPrint (SDBG_MAX,  "       %3d    %3d     %3d  ",
                (*RisingEdgeSlewRate)[Ch][SubCh][RankIndex][ScompStrobe][Bit],
                (*FallingEdgeSlewRate)[Ch][SubCh][RankIndex][ScompStrobe][Bit],
                (*SlewRateDelta)[Ch][SubCh][RankIndex][ScompStrobe][Bit]
                );

              // Display warning message, if necessary
              if (T2Error || T3Error || T5Error) {
                PrintTimingWarnings (T2Error, T3Error, T5Error);
              }

              RcDebugPrint (SDBG_MAX, "\n");

              //
              // Record the max slew rate delta that has been found per nibble.
              //
              if (ABS((*MaxSlewRateDeltaPerStrobe)[Ch][SubCh][ScompStrobe]) < ABS ((*SlewRateDelta)[Ch][SubCh][RankIndex][ScompStrobe][Bit])) {
                (*MaxSlewRateDeltaPerStrobe)[Ch][SubCh][ScompStrobe] = (*SlewRateDelta)[Ch][SubCh][RankIndex][ScompStrobe][Bit];
              }

            } // Bit
          } // DqNibble
        } // SubCh
      } // Rank
    } // Dimm
  } // Ch

  RcFreePool (SlewRateDelta);
  RcFreePool (FallingEdgeSlewRate);
  RcFreePool (RisingEdgeSlewRate);

  return MRC_STATUS_SUCCESS;
}


/**

  Trains Tx DQ Slew Rate.

  @param  Host     Pointer to sysHost

  @retval SUCCESS  DQ slew rate training succeeds.
  @retval FAILURE  DQ slew rate training fails.

**/
UINT32
TxDqSlewRateTraining (
  IN PSYSHOST  Host
  )
{
  UINT8                           Ch;
  UINT8                           SubCh;
  UINT8                           Socket;
  UINT8                           ScompStrobe;
  UINT8                           MaxChDdr;
  struct bitMargin                *TimingMarginNorminal;
  struct bitMargin                *TimingMarginVrefMax0p7;
  struct bitMargin                *TimingMarginVrefMin0p7;
  INT16                           (*VrefNominal)[MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE/2];
  INT16                           (*VrefMax0p7)[MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE/2];
  INT16                           (*VrefMin0p7)[MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE/2];
  INT16                           MaxSlewRateDeltaPerStrobe[MAX_CH][SUB_CH][MAX_STROBE/2];
  BOOLEAN                         AllChannelDone;
  BOOLEAN                         SlewRateTrainingDone[MAX_CH][SUB_CH][MAX_STROBE/2];
  UINT8                           LoopCount;
  INT16                           OptimalSlewRateDelta[MAX_CH][SUB_CH][MAX_STROBE/2];
  SCOMP_SETTING                   OptimalScomp[MAX_CH][SUB_CH][MAX_STROBE/2];
  INT16                           Data16;
  MRC_STATUS                      Status;

  //
  // Local parameters initialization.
  //

  MaxChDdr = GetMaxChDdr ();
  Socket = GetCurrentSocketId ();
  AllChannelDone = FALSE;
  LoopCount = 0;

  VrefNominal = RcAllocatePool (sizeof (*VrefNominal));
  VrefMax0p7 = RcAllocatePool (sizeof (*VrefMax0p7));
  VrefMin0p7 = RcAllocatePool (sizeof (*VrefMin0p7));
  TimingMarginNorminal = RcAllocatePool (sizeof (struct bitMargin));
  TimingMarginVrefMax0p7 = RcAllocatePool (sizeof (struct bitMargin));
  TimingMarginVrefMin0p7 = RcAllocatePool (sizeof (struct bitMargin));


  if ((VrefMin0p7 == NULL) || (VrefMax0p7 == NULL) || (VrefNominal == NULL)
    || (TimingMarginNorminal == NULL) || (TimingMarginVrefMax0p7 == NULL) || (TimingMarginVrefMin0p7 == NULL)) {

    if (TimingMarginVrefMin0p7 != NULL) {
      RcFreePool (TimingMarginVrefMin0p7);
    }

    if (TimingMarginVrefMax0p7 != NULL) {
      RcFreePool (TimingMarginVrefMax0p7);
    }

    if (TimingMarginNorminal != NULL) {
      RcFreePool (TimingMarginNorminal);
    }

    if (VrefMin0p7 != NULL) {
      RcFreePool (VrefMin0p7);
    }

    if (VrefMax0p7 != NULL) {
      RcFreePool (VrefMax0p7);
    }

    if (VrefNominal != NULL) {
      RcFreePool (VrefNominal);
    }

    RcDebugPrint (SDBG_MAX, "WARNING: Out of temporary memory space. DQ Slew Rate Training return without doing anything.\n");
    return MRC_STATUS_FAILURE;
  }

  ZeroMem ((UINT8 *) (VrefNominal), sizeof (*VrefNominal));
  ZeroMem ((UINT8 *) (VrefMax0p7), sizeof (*VrefMax0p7));
  ZeroMem ((UINT8 *) (VrefMin0p7), sizeof (*VrefMin0p7));
  ZeroMem ((UINT8 *) (TimingMarginNorminal), sizeof (*TimingMarginNorminal));
  ZeroMem ((UINT8 *) (TimingMarginVrefMax0p7), sizeof (*TimingMarginVrefMax0p7));
  ZeroMem ((UINT8 *) (TimingMarginVrefMin0p7), sizeof (*TimingMarginVrefMin0p7));
  ZeroMem ((UINT8 *) SlewRateTrainingDone, sizeof (SlewRateTrainingDone));
  ZeroMem ((UINT8 *) MaxSlewRateDeltaPerStrobe, sizeof(MaxSlewRateDeltaPerStrobe));
  ZeroMem ((UINT8 *) OptimalSlewRateDelta, sizeof(OptimalSlewRateDelta));
  ZeroMem ((UINT8 *) OptimalScomp, sizeof(OptimalScomp));

  //
  // Initialize chip specific Scomp. Scomp strobe is x4 strobe.
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      for (ScompStrobe = 0; ScompStrobe < (MAX_STROBE / 2); ScompStrobe++) {
        OptimalSlewRateDelta[Ch][SubCh][ScompStrobe] = INVALID_SLEW_RATE;
        InitializeDataScompChip (Host, Socket, Ch, SubCh, ScompStrobe);
      }
    }
  }

  //
  // Get Vref Nominal, 70% Vref Max and 70% Vref Min of DQ.
  //
  GetDqVrefMargin (Host, Socket, VrefNominal, VrefMax0p7, VrefMin0p7);

  //
  // Per nibble DQ slew rate traning loop.
  //
  for (LoopCount = 0; LoopCount < MAX_DQ_SLEW_RATE_SAMPLES; LoopCount++) {

    RcDebugPrint (SDBG_MEM_TRAIN, "\n\nDQ Slew Rate Training Loop %d \n", LoopCount);

    ZeroMem ((UINT8 *) MaxSlewRateDeltaPerStrobe, sizeof (MaxSlewRateDeltaPerStrobe));

    //
    // Set new Scomp for all unfinished nibbles.
    //
    for (Ch = 0; Ch < MaxChDdr;Ch += GetChInc (Host)) {

      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }

      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        for (ScompStrobe = 0; ScompStrobe < (MAX_STROBE / 2); ScompStrobe++) {

          if (SlewRateTrainingDone[Ch][SubCh][ScompStrobe] == FALSE) {
            Data16 = mDataScompUpAdj[LoopCount];
            GetSetDataGroup (Host, Socket, Ch, SubCh, NO_DIMM, NO_RANK, ScompStrobe, NO_BIT, DdrLevel, TxDqRiseSlewRate, GSM_FORCE_WRITE, &Data16);
            Data16 = mDataScompDownAdj[LoopCount];
            GetSetDataGroup (Host, Socket, Ch, SubCh, NO_DIMM, NO_RANK, ScompStrobe, NO_BIT, DdrLevel, TxDqFallSlewRate, GSM_FORCE_WRITE, &Data16);
            Data16 = mDataScomp[LoopCount];
            GetSetDataGroup (Host, Socket, Ch, SubCh, NO_DIMM, NO_RANK, ScompStrobe, NO_BIT, DdrLevel, TxDqScomp, GSM_FORCE_WRITE, &Data16);
          }
        }
      }
    }

    DisplayResultsDdr5 (Host, Socket, TxDqScomp);
    DisplayResultsDdr5 (Host, Socket, TxDqRiseSlewRate);
    DisplayResultsDdr5 (Host, Socket, TxDqFallSlewRate);

    //
    // Tx DQ timing margin under 70% Vref Max.
    //
    SetTxVref (Socket, (GSM_FORCE_WRITE | GSM_UPDATE_CACHE), VrefMax0p7);
    GetAdvDataMargins (
      Socket, DdrLevel, TxDqDelay, DefaultResults,
      NULL, TimingMarginVrefMax0p7, NULL,
      NULL, NULL, NULL
      );

    //
    // Tx DQ timing margin under 70% Vref Min.
    //
    SetTxVref (Socket, (GSM_FORCE_WRITE | GSM_UPDATE_CACHE), VrefMin0p7);
    GetAdvDataMargins (
      Socket, DdrLevel, TxDqDelay, DefaultResults,
      NULL, TimingMarginVrefMin0p7, NULL,
      NULL, NULL, NULL
      );

    //
    // Tx DQ timing margin under Vref Norminal.
    //
    SetTxVref (Socket, (GSM_FORCE_WRITE | GSM_UPDATE_CACHE), VrefNominal);
    GetAdvDataMargins (
      Socket, DdrLevel, TxDqDelay, DefaultResults,
      NULL, TimingMarginNorminal, NULL,
      NULL, NULL, NULL
      );

    //
    // Calculate max slew rate of each strobe.
    //
    Status = CalculateMaxSlewRateDeltaPerNibble (
      Host,
      Socket,
      VrefNominal,
      VrefMax0p7,
      VrefMin0p7,
      TimingMarginNorminal,
      TimingMarginVrefMax0p7,
      TimingMarginVrefMin0p7,
      &MaxSlewRateDeltaPerStrobe,
      &SlewRateTrainingDone,
      LoopCount
      );

    if (Status != MRC_STATUS_SUCCESS) {
      return Status;
    }

    //
    // Evaluate max slew rate delta per nibble.
    // If max slew rate delta of a nibble is less than MAX_DQ_SLEW_RATE_DELTA, stop training that nibble.
    // Otherwise update OptimalSlewRateDelta of the nibble and continue.
    //

    RcDebugPrint (SDBG_MAX, "\nSummary: Max Delta  \n");
    RcDebugPrintLine (SDBG_DEFAULT, 54, SINGLE_LINE, CRLF_FLAG, CRLF_FLAG);
    RcDebugPrint (SDBG_MAX, "Nibble         ");
    for (ScompStrobe = 0; ScompStrobe < (MAX_STROBE / 2); ScompStrobe++) {
      if (IsStrobeNotValidDdr5 (Host, ScompStrobe) == TRUE) {
        continue;
      }
      RcDebugPrint (SDBG_MAX, "%3d  ", ScompStrobe);
    }
    RcDebugPrint (SDBG_MAX, "\n");

    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
          //
          // Skip the disabled channel.
          //
      if (IsChannelEnabled (Socket, Ch) == FALSE) {

        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {


          for (ScompStrobe = 0; ScompStrobe < (MAX_STROBE / 2); ScompStrobe++) {
            SlewRateTrainingDone[Ch][SubCh][ScompStrobe] = TRUE;
          }
        }

        continue;
      }


      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT, " ");
        for (ScompStrobe = 0; ScompStrobe < (MAX_STROBE / 2); ScompStrobe++) {

          if (IsStrobeNotValidDdr5 (Host, ScompStrobe) == TRUE) {
            continue;
          }

          if (SlewRateTrainingDone[Ch][SubCh][ScompStrobe] == FALSE ) {
            RcDebugPrint (SDBG_MAX, "  %3d", MaxSlewRateDeltaPerStrobe[Ch][SubCh][ScompStrobe]);
          } else {
            //
            // For strobes which has finished training in previous loops.
            //
            RcDebugPrint (SDBG_MAX, "  %3d", OptimalSlewRateDelta[Ch][SubCh][ScompStrobe]);
          }

          //
          // Skip strobes that has been trained.
          //
          if (SlewRateTrainingDone[Ch][SubCh][ScompStrobe] == TRUE) {
            continue;
          }

          //
          // Record that optimal slew rate delat and corresponding scomp that has been found.
          //
          if (ScompStrobe < (MAX_STROBE_DDR5 /4)) {
            if ((ABS (MaxSlewRateDeltaPerStrobe[Ch][SubCh][ScompStrobe]) < ABS (OptimalSlewRateDelta[Ch][SubCh][ScompStrobe])) &&
                ((ABS (MaxSlewRateDeltaPerStrobe[Ch][SubCh][ScompStrobe + (MAX_STROBE_DDR5 /4) ])) < (ABS (OptimalSlewRateDelta[Ch][SubCh][ScompStrobe + (MAX_STROBE_DDR5 /4) ])))) {

              OptimalScomp[Ch][SubCh][ScompStrobe].Scomp = mDataScomp[LoopCount];
              OptimalScomp[Ch][SubCh][ScompStrobe].ScompUpAdj = mDataScompUpAdj[LoopCount];
              OptimalScomp[Ch][SubCh][ScompStrobe].ScompDownAdj = mDataScompDownAdj[LoopCount];

              OptimalScomp[Ch][SubCh][ScompStrobe + (MAX_STROBE_DDR5 /4)].Scomp = mDataScomp[LoopCount];
              OptimalScomp[Ch][SubCh][ScompStrobe + (MAX_STROBE_DDR5 /4)].ScompUpAdj = mDataScompUpAdj[LoopCount];
              OptimalScomp[Ch][SubCh][ScompStrobe + (MAX_STROBE_DDR5 /4)].ScompDownAdj = mDataScompDownAdj[LoopCount];

              OptimalSlewRateDelta[Ch][SubCh][ScompStrobe] = MaxSlewRateDeltaPerStrobe[Ch][SubCh][ScompStrobe];
              OptimalSlewRateDelta[Ch][SubCh][ScompStrobe + (MAX_STROBE_DDR5 /4)] = MaxSlewRateDeltaPerStrobe[Ch][SubCh][ScompStrobe + (MAX_STROBE_DDR5 /4)];
            }

            if ((ABS (MaxSlewRateDeltaPerStrobe[Ch][SubCh][ScompStrobe]) <= MAX_DQ_SLEW_RATE_DELTA) &&
                (ABS (MaxSlewRateDeltaPerStrobe[Ch][SubCh][ScompStrobe + (MAX_STROBE_DDR5 /4) ]) <= MAX_DQ_SLEW_RATE_DELTA)) {
              //
              // This strobe is done.
              //
              SlewRateTrainingDone[Ch][SubCh][ScompStrobe] = TRUE;
              SlewRateTrainingDone[Ch][SubCh][ScompStrobe + (MAX_STROBE_DDR5 /4) ] = TRUE;
            }
          }
        }
        RcDebugPrint (SDBG_MAX, "  \n");
      }
      RcDebugPrint (SDBG_MAX, "  \n");
    } // End of step 2.

    //
    // Check if all channels are done.
    //

    RcDebugPrint (SDBG_MAX, "\nSummary: DQ Slew Rate Training Result");
    RcDebugPrintLine (SDBG_DEFAULT, 118, SINGLE_LINE, CRLF_FLAG, CRLF_FLAG);
    AllChannelDone = TRUE;
    for (Ch = 0; Ch < MaxChDdr;Ch += GetChInc (Host)) {
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }

      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

        for (ScompStrobe = 0; ScompStrobe < (MAX_STROBE / 2); ScompStrobe++) {

          if (IsStrobeNotValidDdr5 (Host, ScompStrobe) == TRUE) {
            continue;
          }

          if (SlewRateTrainingDone[Ch][SubCh][ScompStrobe] == FALSE) {
            AllChannelDone = FALSE;
            RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubCh, NO_RANK, ScompStrobe, NO_BIT, "     Continue\n");
          } else {
            RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubCh, NO_RANK, ScompStrobe, NO_BIT, "     Finished\n");
          }
        }
      }
    }

    //
    // Stop slew rate training if all channles are done.
    //
    if (AllChannelDone == TRUE) {
      break;
    }
  } // End for.

  //
  // For those strobes which has failed to find slew rate delat which is less than MAX_DQ_SLEW_RATE_DELTA, set them to the known optimal Scomp.
  // For those strobes which has found slew rate which is less than MAX_DQ_SLEW_RATE_DELTA, they have already been set to the optimal scomp
  // during the training loop.
  //
  RcDebugPrint (SDBG_MAX, "\n\nMax Slew Rate Delat Summary: \n");
  RcDebugPrint (SDBG_MAX, "Nibble       ");
  for (ScompStrobe = 0; ScompStrobe < (MAX_STROBE / 2); ScompStrobe++) {
    if (IsStrobeNotValidDdr5 (Host, ScompStrobe) == TRUE) {
      continue;
    }
    RcDebugPrint (SDBG_MAX, "%3d  ", ScompStrobe);
  }
  RcDebugPrint (SDBG_MAX, "\n");

  for (Ch = 0; Ch < MaxChDdr;Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT, " ");
      for (ScompStrobe = 0; ScompStrobe < (MAX_STROBE / 2); ScompStrobe++) {

        if (IsStrobeNotValidDdr5 (Host, ScompStrobe) == TRUE) {
          continue;
        }

        RcDebugPrint (SDBG_MAX, "%3d  ", OptimalSlewRateDelta[Ch][SubCh][ScompStrobe]);

        if (SlewRateTrainingDone[Ch][SubCh][ScompStrobe] == FALSE) {
          Data16 = OptimalScomp[Ch][SubCh][ScompStrobe].ScompUpAdj;
          GetSetDataGroup (Host, Socket, Ch, SubCh, NO_DIMM, NO_RANK, ScompStrobe, NO_BIT, DdrLevel, TxDqRiseSlewRate, GSM_FORCE_WRITE, &Data16);
          Data16 = OptimalScomp[Ch][SubCh][ScompStrobe].ScompDownAdj;
          GetSetDataGroup (Host, Socket, Ch, SubCh, NO_DIMM, NO_RANK, ScompStrobe, NO_BIT, DdrLevel, TxDqFallSlewRate, GSM_FORCE_WRITE, &Data16);
          Data16 = OptimalScomp[Ch][SubCh][ScompStrobe].Scomp;
          GetSetDataGroup (Host, Socket, Ch, SubCh, NO_DIMM, NO_RANK, ScompStrobe, NO_BIT, DdrLevel, TxDqScomp, GSM_FORCE_WRITE, &Data16);
        }
      }
      RcDebugPrint (SDBG_MAX, "\n");
    }
  }

  RcDebugPrint (SDBG_MAX, "\nDQ slew rate training finished.\n\n");

  DisplayResultsDdr5 (Host, Socket, TxDqScomp);
  DisplayResultsDdr5 (Host, Socket, TxDqRiseSlewRate);
  DisplayResultsDdr5 (Host, Socket, TxDqFallSlewRate);


  RcFreePool (TimingMarginVrefMin0p7);
  RcFreePool (TimingMarginVrefMax0p7);
  RcFreePool (TimingMarginNorminal);
  RcFreePool (VrefMin0p7);
  RcFreePool (VrefMax0p7);
  RcFreePool (VrefNominal);

  return MRC_STATUS_SUCCESS;
}


/**

  Trains Tx DQ Slew Rate.

  @param  Host     Pointer to sysHost

  @retval SUCCESS  DQ slew rate training succeeds.
  @retval FAILURE  DQ slew rate training fails.

**/
UINT32
TxDqSlewRate (
  IN PSYSHOST  Host
  )
{
  UINT8           Socket;
  MRC_STATUS      Status;

  if (!IsMemFlowEnabled (DqSlewRateTraining)) {
    return SUCCESS;
  }

  Socket = GetCurrentSocketId ();

  RcDebugPrint (SDBG_MAX, "Socket%x DqSlewRate Starts\n", Socket);

  if (!IsDdr5Present (Host, Socket)) {
    return FAILURE;
  }

  //
  // Return if this Socket is disabled
  //
  if (IsSocketDimmPopulated (Host, Socket) != TRUE) {
    return SUCCESS;
  }

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_DQ_SLEW_RATE, Socket));

  SetCurrentTestType (Socket, WritePreDfe2DCenteringTest);

  Status = TxDqSlewRateTraining (Host);

  SetCurrentTestType (Socket, 0);

  if (Status != MRC_STATUS_SUCCESS) {
    return SUCCESS;
  } else {
    return FAILURE;
  }
}
