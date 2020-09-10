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

#include "Ddr5Core.h"
#include <Library/SysHostPointerLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <MemCommon.h>
#include <Library/ProcMemInitChipLib.h>

//
// DDR5 signal name strings
//
#ifdef DEBUG_CODE_BLOCK
  SIGNAL_STRING   mDdr5SignalString[] = {{A0B, "A0_B"},
                                         {A1B, "A1_B"},
                                         {A2B, "A2_B"},
                                         {A3B, "A3_B"},
                                         {A4B, "A4_B"},
                                         {A5B, "A5_B"},
                                         {A6B, "A6_B"},
                                         {A7B, "A7_B"},
                                         {A8B, "A8_B"},
                                         {A9B, "A9_B"},
                                         {A10B, "A10_B"},
                                         {A11B, "A11_B"},
                                         {A12B, "A12_B"},
                                         {A13B, "A13_B"},
                                         {A0A, "A0_A"},
                                         {A1A, "A1_A"},
                                         {A2A, "A2_A"},
                                         {A3A, "A3_A"},
                                         {A4A, "A4_A"},
                                         {A5A, "A5_A"},
                                         {A6A, "A6_A"},
                                         {A7A, "A7_A"},
                                         {A8A, "A8_A"},
                                         {A9A, "A9_A"},
                                         {A10A, "A10_A"},
                                         {A11A, "A11_A"},
                                         {A12A, "A12_A"},
                                         {A13A, "A13_A"},
                                         {PARA, "PAR_A"},
                                         {PARB, "PAR_B"},
                                         {CS0_A_N, "CS0_A_N"},
                                         {CS1_A_N, "CS1_A_N"},
                                         {CS2_A_N, "CS2_A_N"},
                                         {CS3_A_N, "CS3_A_N"},
                                         {CS0_B_N, "CS0_B_N"},
                                         {CS1_B_N, "CS1_B_N"},
                                         {CS2_B_N, "CS2_B_N"},
                                         {CS3_B_N, "CS3_B_N"},
                                         {RSPA_SA, "RSPA_SA"},
                                         {RSPB_SA, "RSPB_SA"},
                                         {RSPA_SB, "RSPA_SB"},
                                         {RSPB_SB, "RSPB_SB"},
                                           //
                                           // Backside signals--sub channel 0.
                                           //
                                         {Q0ACA0, "Q0ACA0"},
                                         {Q0ACA1, "Q0ACA1"},
                                         {Q0ACA2, "Q0ACA2"},
                                         {Q0ACA3, "Q0ACA3"},
                                         {Q0ACA4, "Q0ACA4"},
                                         {Q0ACA5, "Q0ACA5"},
                                         {Q0ACA6, "Q0ACA6"},
                                         {Q0ACA7, "Q0ACA7"},
                                         {Q0ACA8, "Q0ACA8"},
                                         {Q0ACA9, "Q0ACA9"},
                                         {Q0ACA10, "Q0ACA10"},
                                         {Q0ACA11, "Q0ACA11"},
                                         {Q0ACA12, "Q0ACA12"},
                                         {Q0ACA13, "Q0ACA13"},
                                         {BCOM_A, "BCOM_A"},
                                             //
                                             // B Copy signals.
                                             //
                                         {Q0BCA0, "Q0BCA0"},
                                         {Q0BCA1, "Q0BCA1"},
                                         {Q0BCA2, "Q0BCA2"},
                                         {Q0BCA3, "Q0BCA3"},
                                         {Q0BCA4, "Q0BCA4"},
                                         {Q0BCA5, "Q0BCA5"},
                                         {Q0BCA6, "Q0BCA6"},
                                         {Q0BCA7, "Q0BCA7"},
                                         {Q0BCA8, "Q0BCA8"},
                                         {Q0BCA9, "Q0BCA9"},
                                         {Q0BCA10, "Q0BCA10"},
                                         {Q0BCA11, "Q0BCA11"},
                                         {Q0BCA12, "Q0BCA12"},
                                         {Q0BCA13, "Q0BCA13"},
                                             //
                                             // Backside signals--sub channel 1.
                                             //
                                         {Q1ACA0, "Q1ACA0"},
                                         {Q1ACA1, "Q1ACA1"},
                                         {Q1ACA2, "Q1ACA2"},
                                         {Q1ACA3, "Q1ACA3"},
                                         {Q1ACA4, "Q1ACA4"},
                                         {Q1ACA5, "Q1ACA5"},
                                         {Q1ACA6, "Q1ACA6"},
                                         {Q1ACA7, "Q1ACA7"},
                                         {Q1ACA8, "Q1ACA8"},
                                         {Q1ACA9, "Q1ACA9"},
                                         {Q1ACA10, "Q1ACA10"},
                                         {Q1ACA11, "Q1ACA11"},
                                         {Q1ACA12, "Q1ACA12"},
                                         {Q1ACA13, "Q1ACA13"},
                                         {BCOM_B, "BCOM_B"},
                                             //
                                             // B Copy Signals.
                                             //
                                         {Q1BCA0, "Q1BCA0"},
                                         {Q1BCA1, "Q1BCA1"},
                                         {Q1BCA2, "Q1BCA2"},
                                         {Q1BCA3, "Q1BCA3"},
                                         {Q1BCA4, "Q1BCA4"},
                                         {Q1BCA5, "Q1BCA5"},
                                         {Q1BCA6, "Q1BCA6"},
                                         {Q1BCA7, "Q1BCA7"},
                                         {Q1BCA8, "Q1BCA8"},
                                         {Q1BCA9, "Q1BCA9"},
                                         {Q1BCA10, "Q1BCA10"},
                                         {Q1BCA11, "Q1BCA11"},
                                         {Q1BCA12, "Q1BCA12"},
                                         {Q1BCA13, "Q1BCA13"},
                                         {QCS0_A_n, "QCS0_A_n"},
                                         {QCS1_A_n, "QCS1_A_n"},
                                         {QCS2_A_n, "QCS2_A_n"},
                                         {QCS3_A_n, "QCS3_A_n"},
                                         {QCS0_B_n, "QCS0_B_n"},
                                         {QCS1_B_n, "QCS1_B_n"},
                                         {QCS2_B_n, "QCS2_B_n"},
                                         {QCS3_B_n, "QCS3_B_n"},
                                         {CK0, "CK0"},
                                         {CK1, "CK1"},
                                         {CK2, "CK2"},
                                         {CK3, "CK3"},
                                         {CK4, "CK4"},
                                         {CK5, "CK5"},
                                         {C0, "C0"},
                                         {C1, "C1"},
                                         {C2, "C2"}
                                        };

BACKSIDE_QCS_SIGNAL_STRING  Ddr5QcsSignalString[] = {
                          {RANK0, COPY_A, "QACS0_n"},
                          {RANK0, COPY_B, "QBCS0_n"},
                          {RANK1, COPY_A, "QACS1_n"},
                          {RANK1, COPY_B, "QBCS1_n"},
                        };
#endif // DEBUG_CODE_BLOCK
CHAR8 *mUnknownDdr5Signal = "Unknown signal";

/**
  Displays Per Bit Results with no Subchannel
  Prints 80 bits per channel

  @param[in] Host:         Pointer to sysHost
  @param[in] Socket:       Processor Socket
  @param[in] Channel:      Memory Channel
  @param[in] Dimm:         Dimm Number
  @param[in] Rank:         Rank Number
  @param[in] Group:        MRC signal group
*/
VOID
PrintPerBitResultNoSubCh (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Dimm,
  IN UINT8 Rank,
  IN MRC_GT Group
  );

/**
  Displays Per Bit Results with Subchannel
  Prints 40 bits per sub-channel

  @param[in] Host:         Pointer to sysHost
  @param[in] Socket:       Processor Socket
  @param[in] Channel:      Memory Channel
  @param[in] SubCh:        Memory Sub Channel
  @param[in] Dimm:         Dimm Number
  @param[in] Rank:         Rank Number
  @param[in] Group:        MRC signal group
*/
VOID
PrintPerBitResultWithSubCh (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 SubCh,
  IN UINT8 Dimm,
  IN UINT8 Rank,
  IN MRC_GT Group
  );

/**
  Displays the bitwise error status

  @param Host:         Pointer to sysHost
  @param Socket:       Processor Socket
  @param Ch:           Memory Channel
  @param SubCh:        Memory Sub Channel
  @param Dimm:         Dimm Number
  @param Rank:         Rank Number
  @param BwSerr[3]     BitWise error status
  @param Margin:       Margin

  @retval N/A

**/
VOID
DisplayBwSerrDdr5 (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     SubCh,
  UINT8     Dimm,
  UINT8     Rank,
  UINT32    BwSerr[3],
  INT16     Margin
  )
{
  UINT8   Bit;
  UINT8   MaxBits;
  UINT8   BitIndex;
  UINT32  ErrResult = 0;

  AcquirePrintControl ();

  MaxBits = GetMaxBitsValid (Host);

  if ((SubCh % 2) == 0) {
    RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, "%3d", Margin);
  } else {
    RcDebugPrint (SDBG_DEFAULT, " ");
  }

  for (Bit = 0; Bit < MaxBits; Bit++) {

    if (IsBitNotValidSubChDdr5 (Host, Bit)) {
      continue;
    }
    if ((Bit % 8) == 0) {
      RcDebugPrint (SDBG_DEFAULT, " ");
    }

    switch (Bit) {
      case 0:
        ErrResult = BwSerr[0];
        break;

      case 32:
        if ((IsBrsPresent (Host, Socket))) {
          ErrResult = BwSerr[1];
        } else {
          ErrResult = BwSerr[2];
        }
        break;

      case 64:
        ErrResult = BwSerr[2];
        break;

      default:
        //Automated add of default case. Please review.
        break;
    } // switch

    BitIndex = Bit % 32;

    if (ErrResult & (1 << BitIndex)) {
      RcDebugPrint (SDBG_DEFAULT, "#");
    } else {
      RcDebugPrint (SDBG_DEFAULT, ".");
    }

  } // Bit loop

  if ((SubCh % 2) == 1) {
    RcDebugPrint (SDBG_DEFAULT, "\n");
  }

  ReleasePrintControl ();
} // DisplayBwSerrDdr5

/**

  Display the bitwise error status for DDR5.

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure struct
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number (0-based)
  @param[in] SubCh   - Current sub channel
  @param[in] Dimm    - Current dimm
  @param[in] Rank    - Rank number (0-based)
  @param[in] BwSerr  - BitWise error status
  @param[in] Margin  - Margin

  @retval None

**/
VOID
DisplayBwSerrDdr5Ch (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubCh,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT32   BwSerr[3],
  IN INT16    Margin
  )
{
  UINT8   Bit;
  UINT8   MaxBits;
  UINT8   BitIndex;
  UINT32  ErrResult = 0;

  AcquirePrintControl ();

  MaxBits = GetMaxBitsValid (Host);

  if (SubCh == 0) {
    RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, "%3d", Margin);
  }
  RcDebugPrint (SDBG_DEFAULT, " |");

  for (Bit = 0; Bit < MaxBits; Bit++) {

    if (IsBitNotValidSubChDdr5 (Host, Bit)) {
      continue;
    }
    if ((Bit % 8) == 0) {
      RcDebugPrint (SDBG_DEFAULT, " ");
    }

    switch (Bit) {
      case 0:
        ErrResult = BwSerr[0];
        break;

      case 32:
        if ((IsBrsPresent (Host, Socket))) {
          ErrResult = BwSerr[1];
        } else {
          ErrResult = BwSerr[2];
        }
        break;

      case 64:
        ErrResult = BwSerr[2];
        break;

      default:
        //Automated add of default case. Please review.
        break;
    } // switch

    BitIndex = Bit % 32;

    if (ErrResult & (1 << BitIndex)) {
      RcDebugPrint (SDBG_DEFAULT, "#");
    } else {
      RcDebugPrint (SDBG_DEFAULT, ".");
    }

  } // Bit loop

  if (SubCh == 1) {
    RcDebugPrint (SDBG_DEFAULT, " |\n");
  }

  ReleasePrintControl ();

} // DisplayBwSerrDdr5Ch

/**
Finds the first populated channel

@param[in] Host:                   Pointer to sysHost
@param[in] Socket:                 Processor Socket

@retval Channel

**/
UINT8
GetFirstPopCh (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  UINT8  MaxChDdr = 0;
  UINT8  Ch = 0;

  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == TRUE) {
      break;
    }
  }

  return Ch;
}//GetFirstPopCh

/**
  Displays the bitwise error status

  @param[in] Host:                   Pointer to sysHost
  @param[in] Socket:                 Processor Socket
  @param[in] Ch:                     Memory Channel
  @param[in] SubCh:                  Memory Sub Channel
  @param[in] Dimm:                   Dimm Number
  @param[in] Rank:                   Rank Number
  @param[in] BwSerr[2][3]            BitWise error status
  @param[in] Margin:                 Margin
  @param[in] AllStrobePassDqsOffset  Subchannel pass info

  @retval N/A

**/
VOID
DisplayBwSerrEvenOddDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     SubCh,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT32    BwSerr[2][3],
  IN INT16     Margin,
  IN UINT8     AllStrobePassDqsOffset [MAX_CH][SUB_CH]
  )
{
  UINT8   FirstPopCh;
  UINT8   Bit;
  UINT8   MaxBits;
  UINT8   BitIndex;
  UINT32  ErrResult = 0;
  UINT8   Evenodd = 0;

  AcquirePrintControl ();

  MaxBits = MAX_BITS_DDR5 / 2;

  //get the first populated channel
  FirstPopCh = GetFirstPopCh (Host, Socket);

  if ((Ch == FirstPopCh && SubCh == 0 && Margin == RXDQS_SWEEP_START) || ((AllStrobePassDqsOffset[Ch][0] == 1) && (Margin == RXDQS_SWEEP_START))) {
    RcDebugPrint (SDBG_DEFAULT,"                  SUBCH0                                         ");
    RcDebugPrint (SDBG_DEFAULT,"                                               SUBCH1\n\n");

    RcDebugPrint (SDBG_DEFAULT,"                  EVEN:                                          ");
    RcDebugPrint (SDBG_DEFAULT,"ODD:                                           EVEN:");
    RcDebugPrint (SDBG_DEFAULT,"                                          ODD:\n");

  }

  if (SubCh == 0) {
    RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, "%3d", Margin);
  } else if (AllStrobePassDqsOffset[Ch][0] == 1)  {
    RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, "%3d", Margin);
    RcDebugPrintEmptyBlock (SDBG_DEFAULT, 93);
  }

  for (Evenodd = 0; Evenodd < 2; Evenodd++) {
    for (Bit = 0; Bit < MaxBits; Bit++) {

      if (IsBitNotValid (Host, Bit)) {
        continue;
      }
      if ((Bit % 8) == 0) {
        RcDebugPrint (SDBG_DEFAULT, " ");
      }

      switch (Bit) {
        case 0:
          ErrResult = BwSerr[Evenodd][0];
          break;

        case 32:
          ErrResult = BwSerr[Evenodd][2];
          break;

        case 64:
          ErrResult = BwSerr[Evenodd][2];
          break;
        default:
          //Automated add of default case. Please review.
          break;
      } // switch

      BitIndex = Bit % 32;

      if (ErrResult & (1 << BitIndex)) {
        RcDebugPrint (SDBG_DEFAULT, "#");
      } else {
        RcDebugPrint (SDBG_DEFAULT, ".");
      }

    } // Bit loop

    RcDebugPrint (SDBG_DEFAULT, "  ");

  } //Evenodd loop

  if ((SubCh == 1) || (AllStrobePassDqsOffset[Ch][1] == 1)){
    RcDebugPrint (SDBG_DEFAULT, "\n");
  }
  ReleasePrintControl ();
} // DisplayBwSerrEvenOddDdr5

    /*++
    Displays the training results

    @param Host:         Pointer to sysHost
    @param Socket:       Processor Socket
    @param Group:        MRC_GT Group

    @retval N/A

    --*/
VOID
EFIAPI
DisplayResultsDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN MRC_GT    Group
  )
{
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               FirstDimm;
  UINT8               Rank;
  UINT8               Strobe;
  UINT8               Bit;
  UINT8               MaxBits;
  UINT8               RoundTrip;
  UINT8               IoLatency;
  UINT8               IoLatencyComp;
  INT16               Delay;
  UINT8               Subchannel = 0;
  UINT8               ChannelLocal = 0;
  struct channelNvram (*ChannelNvList) [MAX_CH];
  struct dimmNvram    (*DimmNvList) [MAX_DIMM];
  struct ddrRank      (*RankList) [MAX_RANK_DIMM];
  struct RankCh       (*RankPerCh) [MAX_RANK_CH];
  UINT8               SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8               MBV = GetMaxBitsValid (Host);
  UINT8               MaxChDdr;
  UINT8               SubChTemp = 0;
  MRC_TT              CurrentTestType;
  UINT8               Index;
  CHAR8               *GroupString;
  GROUP_STRING        GroupStrings[] = {{RxDqsDelay,        "RX_DQS"},
                                        {RxDqDelay,         "RX_DQ"},
                                        {RxVref,            "RX_VREF"},
                                        {RxSamplerEven,     "RX_SAMPLER_EVEN"},
                                        {RxSamplerOdd,      "RX_SAMPLER_ODD"},
                                        {RxSummer,          "RX_SUMMER_DQ"},
                                        {RxDqDqsDelay,      "RX_DQ_DQS"},
                                        {RxDqsPDelay,       "RX_DQS_P"},
                                        {RxDqsNDelay,       "RX_DQS_N"},
                                        {RecEnDelay,        "RECEIVE_ENABLE_DELAY"},
                                        {RxCoarseDelay,     "RX_COARSE_DELAY"},
                                        {TxDqsDelay,        "WR_LVL"},
                                        {TxDqDelay,         "TX_DQ"},
                                        {TxDqsPieOffset,    "TX_DQS_PIE_OFFSET"},
                                        {TxDqPieOffset,     "TX_DQ_PIE_OFFSET"},
                                        {TxVref,            "DRAM_VREFDQ"},
                                        {TxEq,              "TxEq"},
                                        {RxDqsBitDelay,     "RX_DQS_PER_BIT"},
                                        {RxDqsPBitDelay,    "RX_DQS_P_PER_BIT"},
                                        {RxDqsNBitDelay,    "RX_DQS_N_PER_BIT"},
                                        {TxDqBitDelay,      "TX_DQ_PER_BIT"},
                                        {TxEqTap2,          "TxEqTap2"},
                                        {RxDfeCoefficient,  "RX DFE Coefficient"},
                                        {RxStrobeInvert,    "RX Strobe Inversion"},
                                        {RxSampler,         "Rx Sampler"},
                                        {DcaTco,            "DCA_TCO"},
                                        {TxDqRiseSlewRate,  "TX_SCOMP_UP_ADJ"},
                                        {TxDqFallSlewRate,  "TX_SCOMP_DN_ADJ"},
                                        {TxDqScomp,         "TX_SCOMP"},
                                        {TcoDqDqs,          "TX_TCO"},
                                        {RxDfeCoefficientTap1, "RX_DFE_TAP1"},
                                        {RxDfeCoefficientTap2, "RX_DFE_TAP2"},
                                        {RxDfeCoefficientTap3, "RX_DFE_TAP3"},
                                        {RxDfeCoefficientTap4, "RX_DFE_TAP4"},
                                        {RxDfeCoefficientTap1PerBit, "RX_DFE_TAP1_PER_BIT"},
                                        {RxDfeCoefficientTap2PerBit, "RX_DFE_TAP2_PER_BIT"},
                                        {RxDfeCoefficientTap3PerBit, "RX_DFE_TAP3_PER_BIT"},
                                        {RxDfeCoefficientTap4PerBit, "RX_DFE_TAP4_PER_BIT"},
                                       };

  CurrentTestType = GetCurrentTestType (Socket);
  MaxChDdr = GetMaxChDdr ();
  if ((CurrentTestType == WrDqTest) || (CurrentTestType == DqSlewRateTest)) {
    MBV = MAX_BITS_DDR5 / 2;
  }
  if (!IsBrsPresent (Host, Socket)) {
    MBV = MAX_BITS_DDR5 / 2;
  }
  ChannelNvList = GetChannelNvList (Host, Socket);

  GroupString = "";

  for (Index = 0; Index < ARRAY_SIZE (GroupStrings); Index++) {
    if (Group == GroupStrings[Index].Group) {
      GroupString = GroupStrings[Index].String;
      break;
    }
  }
  RcDebugPrint (SDBG_MEM_TRAIN, "\nSTART_DATA_%a\n", GroupString);

  if (Host->nvram.mem.eccEn) {
    MaxBits = MBV;
  } else {
    MaxBits = MBV - 8;
  }

  if ((Group == RxDqsBitDelay) || (Group == TxDqBitDelay) || (Group == RxVref) ||
      (Group == RxSamplerEven) || (Group == RxSamplerOdd) || (Group == RxSummer) ||
      (Group == RxDqsPBitDelay) || (Group == RxDqsNBitDelay) || (Group == TxDqPieOffset) ||
      (Group == RxDfeCoefficientTap1PerBit) || (Group == RxDfeCoefficientTap2PerBit) ||
      (Group == RxDfeCoefficientTap3PerBit) || (Group == RxDfeCoefficientTap4PerBit)) {
    RcDebugPrint (SDBG_MAX, "BIT:              ");
    for (Bit = 0; Bit < MaxBits; Bit++) {
      RcDebugPrint (SDBG_MEM_TRAIN,
        "  %2d", Bit);
    } // bit loop
  } else if (Group == TxVref) {
    RcDebugPrint (SDBG_MAX, "DRAM:              ");
    for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }

        RcDebugPrint (SDBG_MEM_TRAIN,
          "  %2d ", Strobe);
    } // Strobe loop
  } else if (Group == DcaTco) {
    RcDebugPrint (SDBG_MAX, "PARAM:            TCO_COMP");

  } else if (Group == RxCoarseDelay) {
    RcDebugPrint (SDBG_MAX, "PARAM:            IO_LAT   ROUND_TRIP   IO_COMP");

  } else if (Group == TcoDqDqs) {
    RcDebugPrint (SDBG_MAX, "BYTE:              ");

    for (Strobe = 0; Strobe < (SubChMSVx4 / 2); Strobe++) {
      //
      // Skip if this is an ECC Strobe when ECC is disabled
      //
      if (IsStrobeNotValidDdr5 (Host, Strobe)) {
        continue;
      }

      RcDebugPrint (SDBG_MEM_TRAIN,
        "  %2d ", Strobe);
    } // Strobe loop

  } else {
    RcDebugPrint (SDBG_MAX, "NIBBLE:            ");
    for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
      //
      // Skip if this is an ECC Strobe when ECC is disabled
      //
      if (IsStrobeNotValidDdr5 (Host, Strobe)) {
        continue;
      }

      RcDebugPrint (SDBG_MEM_TRAIN,
        "  %2d ", Strobe);
    } // Strobe loop
  }
  RcDebugPrint (SDBG_MEM_TRAIN,
                  "\n");

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (Subchannel = 0; Subchannel < SUB_CH; Subchannel++) {
      if (IsBrsPresent (Host, Socket)) {
#ifdef DEBUG_CODE_BLOCK
        DisplayTcwlAdjDdr5 (Host, Socket, Ch, Subchannel, Group);
#endif //DEBUG_CODE_BLOCK
      }
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    RankPerCh = &(*ChannelNvList) [Ch].rankPerCh;

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList) [Dimm].dimmPresent == 0) {
        continue;
      }

      RankList = GetRankNvList (Host, Socket, Ch, Dimm);

      for (Subchannel = 0; Subchannel < SUB_CH; Subchannel++) {
        ChannelLocal = Ch;
        GetChannelMapping (Ch, Subchannel, &ChannelLocal);
        for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
            continue;
          }

          if (!FindFirstDimmInChannel (Socket, Ch, &FirstDimm)) {
            RC_FATAL_ERROR (FALSE, ERR_DIMM_COMPAT, RC_FATAL_ERROR_MINOR_CODE_146);
          }

          if (((Group == TxDqScomp) || (Group == TxDqRiseSlewRate) || (Group == TxDqFallSlewRate)) && ((Dimm != FirstDimm) || (Rank > 0))) {
            continue;
          }

          if ((Group == RxVref) && ((Dimm != FirstDimm) | (Rank > 0))) {
            continue;
          }

          if (((Group == DcaTco) || (Group == TcoDqDqs)) && ((Dimm != FirstDimm) || (Rank > 0))) {
            continue;
          }

          if ((Group == TxVref) && IsLrdimmPresent (Socket, Ch, Dimm) && (Rank > 0)) {
            continue;
          }

          if ((Group == TxDqScomp) || (Group == TxDqRiseSlewRate) || (Group == TxDqFallSlewRate)) {
            RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, NO_DIMM, Subchannel, NO_RANK, NO_STROBE, NO_BIT,
                                "");
          } else {
            RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, Subchannel, Rank, NO_STROBE, NO_BIT,
                                "");
          }

          if (Group == DcaTco) {
            GetSetDataGroup (Host, Socket, ChannelLocal, Subchannel, Dimm, Rank, 0, 0, DdrLevel, Group, GSM_READ_ONLY, &Delay);
            RcDebugPrint (SDBG_MEM_TRAIN,
                            "%5d\n", Delay);
            continue;
          }
          if (Group == TcoDqDqs) {

            for (Strobe = 0; Strobe < (SubChMSVx4 / 2); Strobe++) {
              GetSetDataGroup (Host, Socket, ChannelLocal, Subchannel, Dimm, Rank, Strobe, 0, DdrLevel, Group, GSM_READ_ONLY, &Delay);
              RcDebugPrint (SDBG_MEM_TRAIN,
                              "%5d", Delay);
            } // Strobe loop

            RcDebugPrint (SDBG_MEM_TRAIN,
                            "\n");
            continue;
          }
          if (Group == RxCoarseDelay) {
            GetRoundTripDdr5 (Socket, Ch, Subchannel, GetLogicalRank (Host, Socket, Ch, Dimm, Rank), GSM_READ_ONLY, &RoundTrip);
            GetIoLatencyDdr5 (Socket, Ch, Subchannel, GetLogicalRank (Host, Socket, Ch, Dimm, Rank), GSM_READ_ONLY, &IoLatency);
            IoLatencyComp = GetIoLatCompValue ();
            RcDebugPrint (SDBG_MEM_TRAIN,
                            "%5d        %5d      %5d\n", IoLatency, RoundTrip, IoLatencyComp);
            continue;
          }
          if ((Group == RxDqsBitDelay) || (Group == RxDqsPBitDelay) || (Group == RxDqsNBitDelay) || (Group == TxDqBitDelay) ||
              (Group == RxVref) || (Group == RxSamplerEven) || (Group == RxSamplerOdd) ||
              (Group == RxSummer) || (Group == TxDqPieOffset) ||
              (Group == RxDfeCoefficientTap1PerBit) || (Group == RxDfeCoefficientTap2PerBit) ||
              (Group == RxDfeCoefficientTap3PerBit) || (Group == RxDfeCoefficientTap4PerBit)) {
            if (IsBrsPresent (Host, Socket)) {
              PrintPerBitResultNoSubCh (Host, Socket, ChannelLocal, Dimm, Rank, Group);
            } else {
              PrintPerBitResultWithSubCh (Host, Socket, ChannelLocal, Subchannel, Dimm, Rank, Group);
            }
          } else {
            for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
              //
              // Skip if this is an ECC Strobe when ECC is disabled
              //
              if (IsStrobeNotValid (Host, Strobe)) {
                continue;
              }
              if ((IsBrsPresent (Host, Socket))) {
                SubChTemp = NO_SUBCH;
              } else {
                SubChTemp = Subchannel;
              }

              GetSetDataGroup (Host, Socket, ChannelLocal, SubChTemp, Dimm, Rank, Strobe, 0, DdrLevel, Group, GSM_READ_ONLY, &Delay);
              RcDebugPrint (SDBG_MEM_TRAIN,
                              "%5d", Delay);
            } // Strobe loop

            RcDebugPrint (SDBG_MEM_TRAIN,
                            "\n");

            if (Group == RecEnDelay) {
              GetRoundTripDdr5 (Socket, Ch, Subchannel, GetLogicalRank (Host, Socket, Ch, Dimm, Rank), GSM_READ_ONLY, &RoundTrip);
              GetIoLatencyDdr5 (Socket, Ch, Subchannel, GetLogicalRank (Host, Socket, Ch, Dimm, Rank), GSM_READ_ONLY, &IoLatency);
              if (IsBrsPresent (Host, Socket)) {
                RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                              "IO Latency = %d, Round Trip = %d\n", IoLatency, RoundTrip);
              }
            } // if per Bit
          }
        } // Rank loop
      } //Subchannel loop
    }// Dimm loop
  }// Ch loop
  RcDebugPrint (SDBG_MEM_TRAIN, "STOP_DATA_%a\n", GroupString);
} // DisplayResultsDdr5

/*
  Gets strobe number from given bit for a signal

  @param[in]   Bit    - Bit to be mapped to strobe
  @param[out] *Strobe - Calculated Strobe value

  @retval MRC_STATUS_SUCCESS               - Input Bit successfully mapped to strobe
  @retval MRC_STATUS_INVALID_PARAMETER     - Input Bit is invalid

*/
MRC_STATUS
GetStrobeFromBitDDR5 (
  IN UINT8 Bit,
  OUT UINT8 *Strobe
  )
{
  PSYSHOST   Host       = GetSysHostPointer ();
  UINT8      SubChMSVx8 = GetSubChMaxStrobeValid (Host) / 2;

  if (Bit > GetMaxBitsValid (Host) || Strobe == NULL) {
   return MRC_STATUS_INVALID_PARAMETER;
  }

  if ((Bit / 4) % 2) {
    *Strobe = Bit / 8 + SubChMSVx8;
  } else {
    *Strobe = Bit / 8;
  }
  return MRC_STATUS_SUCCESS;
}

/**
  Displays Per Bit Results with Subchannel
  Prints 40 bits per sub-channel

  @param[in] Host:         Pointer to sysHost
  @param[in] Socket:       Processor Socket
  @param[in] Channel:      Memory Channel
  @param[in] SubCh:        Memory Sub Channel
  @param[in] Dimm:         Dimm Number
  @param[in] Rank:         Rank Number
  @param[in] Group:        MRC signal group
*/
VOID
PrintPerBitResultWithSubCh (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 SubCh,
  IN UINT8 Dimm,
  IN UINT8 Rank,
  IN MRC_GT Group
  )
{
  UINT8           MBV = GetMaxBitsValid (Host);//SPR_TODO.
  UINT8           Strobe = 0;
  UINT8           Bit = 0;
  UINT8           MaxBits = 0;
  INT16           Delay = 0;

  MBV = MAX_BITS_DDR5 / 2;
  if (Host->nvram.mem.eccEn) {
    MaxBits = MBV;
  } else {
    MaxBits = MBV - 8;
  }

  for (Bit = 0; Bit < MaxBits; Bit++) {

    GetStrobeFromBitDDR5 (Bit, &Strobe);

    // call SetGDCR to update the appropriate register field

    GetSetDataGroup (Host, Socket, Channel, SubCh, Dimm, Rank, Strobe, Bit % 4, DdrLevel, Group, GSM_READ_ONLY,
      &Delay);
    RcDebugPrint (SDBG_MEM_TRAIN,
                    " %3d", Delay);

  } // Bit loop
  RcDebugPrint (SDBG_MEM_TRAIN, "\n");
}


/**
  Displays Per Bit Results with no Subchannel
  Prints 80 bits per channel

  @param[in] Host:         Pointer to sysHost
  @param[in] Socket:       Processor Socket
  @param[in] Channel:      Memory Channel
  @param[in] Dimm:         Dimm Number
  @param[in] Rank:         Rank Number
  @param[in] Group:        MRC signal group
*/
VOID
PrintPerBitResultNoSubCh (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Dimm,
  IN UINT8 Rank,
  IN MRC_GT Group
  )
{
  UINT8           MBV = GetMaxBitsValid (Host);//SPR_TODO.
  UINT8           Strobe = 0;
  UINT8           Bit = 0;
  UINT8           BitIndex = 0;
  UINT8           MaxBits = 0;
  INT16           Delay = 0;

  if (Host->nvram.mem.eccEn) {
    MaxBits = MBV;
  } else {
    MaxBits = MBV - 8;
  } //if (Host->nvram.mem.eccEn)

  for (Bit = 0; Bit < MaxBits; Bit++) {
    GetStrobeFromBitDDR5 (Bit, &Strobe);

    if ((Bit == 0) || (Bit == (MaxBits / 2))) {
    RcDebugPrint (SDBG_MEM_TRAIN,
                    "\n");
    if (Bit == (MaxBits / 2)) {
      RcDebugPrint (SDBG_MEM_TRAIN,
                      "\n");
    }
    for (BitIndex = Bit; BitIndex < (Bit + (MaxBits / 2)); BitIndex++) {
      RcDebugPrint (SDBG_MEM_TRAIN,
                      "  %2d", BitIndex);
    }
    RcDebugPrint (SDBG_MEM_TRAIN,
                    "\n");
    }

    GetSetDataGroup (Host, Socket, Channel, NO_SUBCH, Dimm, Rank, Strobe, (Bit % 4), DdrLevel, Group, GSM_READ_ONLY,
      &Delay);
    RcDebugPrint (SDBG_MEM_TRAIN,
                  " %3d", Delay);
  } // for (Bit = 0; Bit < MaxBits; Bit++)

  RcDebugPrint (SDBG_MEM_TRAIN, "\n");
}
//PrintPerBitResultNoSubCh ()


/**

Displays the DDR5 CWL_ADJ results

@param Host    - Pointer to sysHost, the system Host (root) structure struct
@param Socket  - Socket Id
@param Mode    - Program Mode.
@param Group   - MRC_GT Group

@retval None

**/
VOID
DisplayCwlAdjDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Mode,
  IN INT16     (*CwlAdj) [MAX_CH][SUB_CH][MAX_RANK_CH]
  )
{
  UINT8               Dimm;
  UINT8               Rank;
  UINT8                Ch = 0;
  UINT8               Subchannel = 0;
  UINT8               ChannelLocal = 0;
  INT16               CwlA = 0;
  struct channelNvram (*ChannelNvList) [MAX_CH];
  struct dimmNvram (*DimmNvList) [MAX_DIMM];
  struct ddrRank (*RankList) [MAX_RANK_DIMM];
  struct RankCh (*RankPerCh) [MAX_RANK_CH];

  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  RcDebugPrint (SDBG_MEM_TRAIN,
                  "              ");

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (Subchannel = 0; Subchannel < SUB_CH; Subchannel++) {
      ChannelLocal = Ch;
        GetChannelMapping (Ch, Subchannel, &ChannelLocal);

      RcDebugPrint (SDBG_MEM_TRAIN,
                      "\n");

      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      RankPerCh = &(*ChannelNvList) [Ch].rankPerCh;

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList) [Dimm].dimmPresent == 0) {
          continue;
        }

        RankList = GetRankNvList (Host, Socket, Ch, Dimm);


        //    MaxStrobe = GetMaxStrobe(Host, Socket, Ch, Dimm, Group, DdrLevel);

        for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
            continue;
          }
          if (Mode == DISPLAY_FROM_REGISTER) {
            GetCwlAdjDdr5 (Host, Socket, Ch, Subchannel, Dimm, &CwlA);
          } else if (Mode == DISPLAY_FROM_CACHE) {
            CwlA = (*CwlAdj) [Ch][Subchannel][ (*RankList) [Rank].rankIndex];
          }
          RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, Subchannel, Rank, NO_STROBE, NO_BIT, "   t_cwl_adj = %d\n", CwlA);

        } // Rank loop
      } // Dimm loop
    }//Subchannel loop
  } // Ch loop
} // DisplayCwlAdjDdr5

/**
  Displays tCWL and DDR_DQS adjust results.

  @param[in] Host      Pointer to sysHost, the system Host (root) structure struct
  @param[in] Socket    Socket Id

  @retval              None

**/
VOID
DisplaytCwlDdrDqsAdjResults (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               Ch = 0;
  UINT8               Subchannel = 0;
  UINT8               ChannelLocal = 0;
  UINT8               MaxChDdr;
  INT16               tCwlAdj = 0;
  INT16               DdrDqsAdj = 0;
  struct channelNvram (*ChannelNvList) [MAX_CH];
  struct dimmNvram (*DimmNvList) [MAX_DIMM];
  struct ddrRank (*RankList) [MAX_RANK_DIMM];
  struct RankCh (*RankPerCh) [MAX_RANK_CH];

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  if (IsBrsPresent (Host, Socket)) {
    return;
  }

  RcDebugPrint (SDBG_MEM_TRAIN,
                  "\nSTART_DATA_WRITE_LEVELING_COARSE_DELAY\n");
  RcDebugPrint (SDBG_MEM_TRAIN,
                  "PARAM:           t_CWL_ADJ   DDR_DQS_ADJ        \n");

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (Subchannel = 0; Subchannel < SUB_CH; Subchannel++) {
      ChannelLocal = Ch;
      GetChannelMapping (Ch, Subchannel, &ChannelLocal);
      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      RankPerCh = &(*ChannelNvList) [Ch].rankPerCh;

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList) [Dimm].dimmPresent == 0) {
          continue;
        }
        if (!(*DimmNvList) [Dimm].DcpmmPresent) {
          GetCwlDdrDqsAdjDdrio (Host, Socket, Ch, CWLADJ_DDR5, &tCwlAdj, &DdrDqsAdj);
        } else {
          GetCwlDdrDqsAdjDdrio (Host, Socket, Ch, CWLADJ_DDRT, &tCwlAdj, &DdrDqsAdj);
        }
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
            continue;
          }
          //
          // Print the tCWL and DDR_DQS adjustment
          //
          RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, Subchannel, Rank, NO_STROBE, NO_BIT, "       %d             %d\n", tCwlAdj, DdrDqsAdj);
        } // Rank loop
      } // Dimm loop
    } // Subchannel loop
  } // Ch loop
  RcDebugPrint (SDBG_MEM_TRAIN, "STOP_DATA_WRITE_LEVELING_COARSE_DELAY\n");
} // DisplaytCwlDdrDqsAdjResults

/**

  Displays the DDR5 training results for specific Rank.

  @param[in] Host           - Pointer to sysHost, the system Host (root) structure struct
  @param[in] Socket         - Socket Id
  @param[in] Ch             - Ch number
  @param[in] Dimm           - Dimm number
  @param[in] Rank           - Rank number
  @param[in] Mode           - program Mode
  @param[in] DisplayINTCAL  - display internal cycle alignment or not
  @param[in] CwlAdjInput    - cwl_adj value input
  @param[in] Group          - MRC_GT Group

  @retval None

**/
VOID
DisplaySpecificRankResultsDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     Mode,
  IN BOOLEAN   DisplayINTCAL,
  IN INT16     (*CwlAdj) [MAX_CH][SUB_CH][MAX_RANK_CH],
  IN MRC_GT    Group
  )
{
  UINT8               Strobe;
  UINT8               MaxStrobe;
  UINT8               Bit;
  UINT8               b;
  UINT8               MaxBits;
  UINT8               EncodedNibble;
  UINT8               RoundTrip;
  UINT8               IoLatency;
  INT16               Delay;
  UINT8               Subchannel = 0;
  UINT8               ChannelLocal = 0;
  UINT8               FirstDimm;
  INT16               CwlA = 0;
  struct channelNvram (*ChannelNvList) [MAX_CH];
  struct dimmNvram    (*DimmNvList) [MAX_DIMM];
  struct ddrRank      (*RankList) [MAX_RANK_DIMM];
  struct RankCh       (*RankPerCh) [MAX_RANK_CH];
  UINT8               SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8               SubChMSVx8 = GetSubChMaxStrobeValid (Host) / 2;
  UINT8               MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8               MBV = GetMaxBitsValid (Host);

  ChannelNvList = GetChannelNvList (Host, Socket);

  RcDebugPrint (SDBG_MEM_TRAIN,
                  "              ");

  if (Host->nvram.mem.eccEn) {
    MaxBits = MBV;
  } else {
    MaxBits = MBV - 8;
  }

  if (IsX4Dimm (Socket, Ch, Dimm)) {
    MaxStrobe = SubChMSVx4;
  } else {
    MaxStrobe = SubChMSVx8;
  }

  RcDebugPrint (SDBG_MEM_TRAIN,
                  "\n");

  if (IsChannelEnabled (Socket, Ch) == FALSE) {
    return;
  }
  for (Subchannel = 0; Subchannel < SUB_CH; Subchannel++) {
    ChannelLocal = Ch;
    GetChannelMapping (Ch, Subchannel, &ChannelLocal);
    RankList = GetRankNvList (Host, Socket, Ch, Dimm);
    if (Mode == DISPLAY_FROM_REGISTER) {
      GetCwlAdjDdr5 (Host, Socket, Ch, Subchannel, Dimm, &CwlA);
    } else if (Mode == DISPLAY_FROM_CACHE) {
      CwlA = (*CwlAdj) [Ch][Subchannel][ (*RankList) [Rank].rankIndex];
    }
    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, Subchannel, Rank, NO_STROBE, NO_BIT, "   t_cwl_adj = %d\n", CwlA);

    if ((Group != RxDqsBitDelay) && (Group != TxDqBitDelay) && (Group != RxVref) && (Group != RxDqsPBitDelay) && (Group != RxDqsNBitDelay)) {
      RcDebugPrint (SDBG_MEM_TRAIN,
                      "                  ");
      for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValid (Host, Strobe)) {
          continue;
        }

        RcDebugPrint (SDBG_MEM_TRAIN,
                        "  %2d ", Strobe);
      } // Strobe loop
    } // if not per Bit
    RcDebugPrint (SDBG_MEM_TRAIN,
                    "\n");

    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    RankPerCh = &(*ChannelNvList) [Ch].rankPerCh;

    if ((*DimmNvList) [Dimm].dimmPresent == 0) {
      return;
    }

    if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
      return;
    }

    if (!FindFirstDimmInChannel (Socket, Ch, &FirstDimm)) {
      RC_FATAL_ERROR (FALSE, ERR_DIMM_COMPAT, RC_FATAL_ERROR_MINOR_CODE_146);
    }

    if ((Group == RxVref) && ((Dimm != FirstDimm) | (Rank > 0))) {
      continue;
    }

    if ((Group == TxVref) && IsLrdimmPresent (Socket, Ch, Dimm) && (Rank > 0)) {
      continue;
    }

    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, Subchannel, Rank, NO_STROBE, NO_BIT,
                        "");

    if ((Group == RxDqsBitDelay) || (Group == RxDqsPBitDelay) || (Group == RxDqsNBitDelay) || (Group == TxDqBitDelay) || (Group == RxVref)) {
      for (Bit = 0; Bit < MaxBits; Bit++) {
        if ((Bit / 4) % 2) {
          EncodedNibble = Bit / 8 + MSVx8;
        } else {
          EncodedNibble = Bit / 8;
        }
        if ((Bit == 0) || (Bit == (MaxBits / 2))) {
          RcDebugPrint (SDBG_MEM_TRAIN,
                          "\n");
          if (Bit == (MaxBits / 2)) {
            RcDebugPrint (SDBG_MEM_TRAIN,
                            "\n");
          }
          for (b = Bit; b < (Bit + (MaxBits / 2)); b++) {
            RcDebugPrint (SDBG_MEM_TRAIN,
                            "  %2d", b);
          }
          RcDebugPrint (SDBG_MEM_TRAIN,
                          "\n");
        }

        GetSetDataGroup (Host, Socket, ChannelLocal, NO_SUBCH, Dimm, Rank, EncodedNibble, (Bit % 4), DdrLevel, Group, GSM_READ_ONLY,
          &Delay);
        RcDebugPrint (SDBG_MEM_TRAIN,
                        " %3d", Delay);

      } // Bit loop
      RcDebugPrint (SDBG_MEM_TRAIN,
                      "\n");

    } else {
      for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValid (Host, Strobe)) {
          continue;
        }

        GetSetDataGroup (Host, Socket, ChannelLocal, Subchannel, Dimm, Rank, Strobe, 0, DdrLevel, Group, GSM_READ_ONLY, &Delay);
        RcDebugPrint (SDBG_MEM_TRAIN,
                        "%5d", Delay);
      } // Strobe loop

      RcDebugPrint (SDBG_MEM_TRAIN,
                      "\n");

      if (Group == RecEnDelay) {
        GetRoundTripDdr5 (Socket, Ch, Subchannel, GetLogicalRank (Host, Socket, Ch, Dimm, Rank), GSM_READ_ONLY, &RoundTrip);
        GetIoLatencyDdr5 (Socket, Ch, Subchannel, GetLogicalRank (Host, Socket, Ch, Dimm, Rank), GSM_READ_ONLY, &IoLatency);
        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                        "IO Latency = %d, Round Trip = %d\n", IoLatency, RoundTrip);
      }
    } // if per Bit
  }//Subchannel loop
} // DisplaySpecificRankResultsDdr5

/**

Displays the DDR5 training results for specific Rank.

@param Host    - Pointer to sysHost, the system Host (root) structure struct
@param Socket  - Socket Id
@param Mode    - program Mode
@param Group   - MRC_GT Group

@retval None

**/
VOID
DisplayTxDqsDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Mode,
  IN MRC_GT    Group
  )
{
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               FirstDimm;
  UINT8               Rank;
  UINT8               Strobe;
  UINT8               MaxStrobe;
  INT16               Delay;
  UINT8               Subchannel = 0;
  UINT8               ChannelLocal = 0;
  struct channelNvram (*ChannelNvList) [MAX_CH];
  struct dimmNvram    (*DimmNvList) [MAX_DIMM];
  struct RankCh       (*RankPerCh) [MAX_RANK_CH];
  UINT8               MaxChDdr;
  UINT8               SubChTemp = 0;
  UINT8               SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8               SubChMSVx8 = GetSubChMaxStrobeValid (Host) / 2;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  RcDebugPrint (SDBG_MEM_TRAIN,
                  "              ");

  RcDebugPrint (SDBG_MEM_TRAIN,
                  "\n");

  if ((Group != RxDqsBitDelay) && (Group != TxDqBitDelay) && (Group != RxVref) && (Group != RxDqsPBitDelay) && (Group != RxDqsNBitDelay)) {
    RcDebugPrint (SDBG_MEM_TRAIN,
                    "                  ");
    for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
      //
      // Skip if this is an ECC Strobe when ECC is disabled
      //
      if (IsStrobeNotValid (Host, Strobe)) {
        continue;
      }

      RcDebugPrint (SDBG_MEM_TRAIN,
                      "  %2d ", Strobe);
    } // Strobe loop
  } // if not per Bit
  RcDebugPrint (SDBG_MEM_TRAIN,
                  "\n");
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (Subchannel = 0; Subchannel < SUB_CH; Subchannel++) {
      ChannelLocal = Ch;
      GetChannelMapping (Ch, Subchannel, &ChannelLocal);
      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      RankPerCh = &(*ChannelNvList) [Ch].rankPerCh;

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList) [Dimm].dimmPresent == 0) {
          continue;
        }

        if (IsX4Dimm (Socket, Ch, Dimm)) {
          MaxStrobe = SubChMSVx4;
        } else {
          MaxStrobe = SubChMSVx8;
        }

        for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
            continue;
          }

          if (!FindFirstDimmInChannel (Socket, Ch, &FirstDimm)) {
            RC_FATAL_ERROR (FALSE, ERR_DIMM_COMPAT, RC_FATAL_ERROR_MINOR_CODE_146);
          }

          if ((Group == RxVref) && ((Dimm != FirstDimm) | (Rank > 0))) {
            continue;
          }

          if ((Group == TxVref) && IsLrdimmPresent (Socket, Ch, Dimm) && (Rank > 0)) {
            continue;
          }

          RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, Subchannel, Rank, NO_STROBE, NO_BIT,
                              "");

          for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
            //
            // Skip if this is an ECC Strobe when ECC is disabled
            //
            if (IsStrobeNotValid (Host, Strobe)) {
              continue;
            }
            if ((IsBrsPresent (Host, Socket))) {
              SubChTemp = NO_SUBCH;
            } else {
              SubChTemp = Subchannel;
            }

            GetSetDataGroup (Host, Socket, ChannelLocal, SubChTemp, Dimm, Rank, Strobe, 0, DdrLevel, Group, GSM_READ_ONLY, &Delay);
            RcDebugPrint (SDBG_MEM_TRAIN,
                            "%5d", Delay);
          } // Strobe loop
          RcDebugPrint (SDBG_MEM_TRAIN,
                          "\n");
        } // Rank loop
      } // Dimm loop
      if (Subchannel == 1) {
        RcDebugPrint (SDBG_MEM_TRAIN,
                        "\n");
      }
    } // Subchannel loop
  }//Ch loop
} // DisplayTxDqsDdr5

/**

Displays the DDR5 Internal Cycle Alignment training results .

@param Host                         - Pointer to sysHost, the system Host (root) structure struct
@param Socket                       - Socket Id
@param Ch                           - Ch number
@param Dimm                         - Dimm number
@param Rank                         - Rank number
@param Mode                         - program Mode
@param InternalCycleAlignmentInput  - Internal cycle alingment value input
@param Group                        - MRC_GT Group

@retval None

**/
VOID
DisplayInternalCycleAlingmentDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     Mode,
  IN INT16     InternalCycleAlignment [MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE_DDR5 / 2],
  IN MRC_GT    Group
  )
{
  UINT8               FirstDimm;
  UINT8               Strobe;
  UINT8               MaxStrobe;
  UINT8               Subchannel = 0;
  UINT8               ChannelLocal = 0;
  UINT8               MaxChDdr;
  struct channelNvram (*ChannelNvList) [MAX_CH];
  struct dimmNvram    (*DimmNvList) [MAX_DIMM];
  struct ddrRank      (*RankList) [MAX_RANK_DIMM];
  struct RankCh       (*RankPerCh) [MAX_RANK_CH];
  UINT8               SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8               SubChMSVx8 = GetSubChMaxStrobeValid (Host) / 2;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  RcDebugPrint (SDBG_MEM_TRAIN,
                  "\nINTERNAL_CYCLE_ALIGNMENT:\n");
  RcDebugPrint (SDBG_MEM_TRAIN,
                  "              ");

  if ((Group != RxDqsBitDelay) && (Group != TxDqBitDelay) && (Group != RxVref) && (Group != RxDqsPBitDelay) && (Group != RxDqsNBitDelay)) {
    RcDebugPrint (SDBG_MEM_TRAIN,
                    "    ");
    for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
      //
      // Skip if this is an ECC Strobe when ECC is disabled
      //
      if (IsStrobeNotValid (Host, Strobe)) {
        continue;
      }

      RcDebugPrint (SDBG_MEM_TRAIN,
                      "  %2d ", Strobe);
    } // Strobe loop
  } // if not per Bit
  RcDebugPrint (SDBG_MEM_TRAIN,
                  "\n");

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (Subchannel = 0; Subchannel < SUB_CH; Subchannel++) {
      ChannelLocal = Ch;
      GetChannelMapping (Ch, Subchannel, &ChannelLocal);

      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      RankPerCh = &(*ChannelNvList) [Ch].rankPerCh;

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList) [Dimm].dimmPresent == 0) {
          continue;
        }
        if (IsLrdimmPresent (Socket, Ch, Dimm)) {
          continue;
        }
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        if (IsX4Dimm (Socket, Ch, Dimm)) {
          MaxStrobe = SubChMSVx4;
        } else {
          MaxStrobe = SubChMSVx8;
        }

        for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
            continue;
          }

          if (!FindFirstDimmInChannel (Socket, Ch, &FirstDimm)) {
            RC_FATAL_ERROR (FALSE, ERR_DIMM_COMPAT, RC_FATAL_ERROR_MINOR_CODE_146);
          }

          if ((Group == RxVref) && ((Dimm != FirstDimm) | (Rank > 0))) {
            continue;
          }

          if ((Group == TxVref) && IsLrdimmPresent (Socket, Ch, Dimm) && (Rank > 0)) {
            continue;
          }

          RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, Subchannel, Rank, NO_STROBE, NO_BIT,
                              "");

          if (Mode == DISPLAY_FROM_CACHE) {
            for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
              //
              // Skip if this is an ECC Strobe when ECC is disabled
              //
              if (IsStrobeNotValid (Host, Strobe)) {
                continue;
              }

              if (Strobe == 0) {
                RcDebugPrint (SDBG_MEM_TRAIN,
                                "% 4d", InternalCycleAlignment [Ch][Subchannel][ (*RankList) [Rank].rankIndex][Strobe]);
              } else {
                RcDebugPrint (SDBG_MEM_TRAIN,
                                "%5d", InternalCycleAlignment [Ch][Subchannel][ (*RankList) [Rank].rankIndex][Strobe]);
              }
            } // Strobe loop
          }

          RcDebugPrint (SDBG_MEM_TRAIN,
                          "\n");

        } // Rank loop
      } // Dimm loop
    } // Subchannel loop
  } // Ch loop
} // DisplayInternalCycleAlingmentDdr5

/**

  Return a pointer to a signal name string

  @param[in]  SignalName    - The signal name

  @retval Pointer to the string

**/
CHAR8*
GetSignalStrDdr5 (
  IN  GSM_CSN   SignalName
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8     Index;

  for (Index = 0; Index < ARRAY_SIZE (mDdr5SignalString); Index++) {
    if (SignalName == mDdr5SignalString[Index].Signal) {
      return mDdr5SignalString[Index].String;
    }
  }

#endif // DEBUG_CODE_BLOCK
  return mUnknownDdr5Signal;
} // GetSignalStrDdr5

/**

  Return a pointer to a signal name string

  @param[in]  SignalName    - The signal name

  @retval Pointer to the string

**/
CHAR8*
GetQcsSignalStrDdr5 (
  IN  GSM_CSN   Rank,
  IN  UINT8     Copy
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8     Index;

  for (Index = 0; Index < ARRAY_SIZE (Ddr5QcsSignalString); Index++) {
    if ((Rank == Ddr5QcsSignalString[Index].Rank) && (Copy == Ddr5QcsSignalString[Index].Copy)) {
      return Ddr5QcsSignalString[Index].String;
    }
  }

#endif // DEBUG_CODE_BLOCK
  return mUnknownDdr5Signal;
} // GetQCsSignalStrDdr5

/**

  Prints the edges found for DDR5 receive enable training.

  @param[in] Host          - Pointer to sysHost
  @param[in] Socket        - Current Socket
  @param[in] Ch            - Channel number
  @param[in] PulseWidth    - Pointer to save the pulse width
  @param[in] FallingEdge   - Pointer to save the falling edge
  @param[in] RisingEdge    - Pointer to save the rising edge
  @param[in] CenterPoint   - Pointer to save the center point
  @param[in] MaxStrobe     - Maximum number of strobes

  @retval N/A

**/
VOID
DisplayEdgesReceiveEnableDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT16    PulseWidth[MAX_STROBE],
  IN UINT16    FallingEdge[MAX_STROBE],
  IN UINT16    RisingEdge[MAX_STROBE],
  IN UINT16    CenterPoint[MAX_STROBE],
  IN UINT8     MaxStrobe
  )
{
  UINT8 Strobe;

  if (!(GetDebugLevel () & SDBG_MAX)) {
    return;
  }

  AcquirePrintControl ();

  RcDebugPrintLine (SDBG_MAX, (MaxStrobe * 4) + 4, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  //
  // Print rising edges
  //
  RcDebugPrint (SDBG_DEFAULT, "RE: ");
  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, " %3d", RisingEdge[Strobe]);
  } // strobe loop
  //
  // Print center points
  //
  RcDebugPrint (SDBG_DEFAULT, "\nCP: ");
  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, " %3d", CenterPoint[Strobe]);
  } // strobe loop
  //
  // Print falling edges
  //
  RcDebugPrint (SDBG_DEFAULT, "\nFE: ");
  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, " %3d", FallingEdge[Strobe]);
  } // strobe loop
  //
  // Print pulse width
  //
  RcDebugPrint (SDBG_DEFAULT, "\nPW: ");
  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_DEFAULT, " %3d", PulseWidth[Strobe]);
  } // strobe loop
  RcDebugPrint (SDBG_DEFAULT, "\n\n");

  ReleasePrintControl ();
} // DisplayEdgesReceiveEnableDdr5

/**
  Displays the training results for all parameters

  @param[in] Host:         Pointer to sysHost

  @retval N/A
**/
UINT32
DisplayTrainResultsDdr5 (
  IN PSYSHOST  Host
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8  socket;
  UINT32 SerialDebugMsgLvlsave = 0;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  if (Setup->common.debugJumper) {
    SerialDebugMsgLvlsave = GetDebugLevel ();
    SetDebugLevel (SerialDebugMsgLvlsave | SDBG_INFO | SDBG_MEM_TRAIN);
  }

  socket = Host->var.mem.currentSocket;

  //
  // Return if this socket is disabled
  //

  if (IsSocketDimmPopulated (Host, socket) != TRUE) {
    return SUCCESS;
  }

  AcquirePrintControl ();

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "START_TRAINING_REGISTER_DUMP\n");

  DisplayXoverResults (Host, socket);

  DisplaySenseAmpTrainingResults (socket);

  DisplayCCCResults (Host, socket);

  if (Setup->mem.enableNgnBcomMargining) {
    DisplayXoverResultsFnv (Host, socket);
  }

  DisplayResultsDdr5 (Host, socket, RecEnDelay);
  DisplayResultsDdr5 (Host, socket, RxCoarseDelay);
  if (GetTrainingStepEnableChip (socket, CHECKPOINT_MINOR_T_RX_DFE)) {
    DisplayResultsDdr5 (Host, socket, RxStrobeInvert);
  }

  if (Setup->mem.optionsExt & DUTY_CYCLE_EN) {
    DisplayResultsDdr5 (Host, socket, RxDqsNDelay);
    DisplayResultsDdr5 (Host, socket, RxDqsPDelay);
  } else {
    DisplayResultsDdr5 (Host, socket, RxDqsDelay);
  }
  if (DESKEW_DELAY == RxDqsBitDelay) {
    DisplayResultsDdr5 (Host, socket, RxDqsBitDelay);
  } else {
    DisplayResultsDdr5 (Host, socket, RxDqsNBitDelay);
    DisplayResultsDdr5 (Host, socket, RxDqsPBitDelay);
  }
  DisplaytCwlDdrDqsAdjResults (Host, socket);
  DisplayResultsDdr5 (Host, socket, TxDqsDelay);
  DisplayResultsDdr5 (Host, socket, TxDqDelay);
  DisplayResultsDdr5 (Host, socket, RxVref);
  DisplayResultsDdr5 (Host, socket, TxVref);
  DisplayResultsDdr5 (Host, socket, TxDqBitDelay);
  DisplayResultsDdr5 (Host, socket, TxDqsPieOffset);
  DisplayResultsDdr5 (Host, socket, TxDqPieOffset);

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "STOP_TRAINING_REGISTER_DUMP\n");

  ReleasePrintControl ();

  if (Setup->common.debugJumper) {
    SetDebugLevel (SerialDebugMsgLvlsave);
  }

  //
  // Dump DRAM registers to serial log
  //
  DumpDimmState (socket);
#endif // DEBUG_CODE_BLOCK
  return SUCCESS;
} // DisplayTrainResultsDdr5

/**

  Displays Advanced Centering data

  @param[in] Host:         Pointer to sysHost
  @param[in] Socket:       Processor socket
  @param[in] Ch            Channel number
  @param[in] Sub-Ch        SubChannel number
  @param[in] Dimm          DIMM number
  @param[in] Rank          Rank number
  @param[in] Group         Parameter to center
  @param[in] Results       Pointer to the structure to store the margin results
  @param[in] Delay         Pointer to the delay value
  @param[in] OldDelay      Pointer to the old delay value
  @param[in] MaxVref       Max vref
  @param[in] StepSize      Setp size
  @param[in] NumPoints     Num points

  @retval N/A

**/
VOID
DisplayAdvancedCenteringResultsDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Subchannel,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN MRC_GT    Group,
  IN VOID      *Results,
  IN INT16     *Delay,
  IN INT16     *OldDelay,
  IN UINT8     MaxVref,
  IN UINT8     StepSize,
  IN UINT8     NumPoints
)
{
  UINT8               Strobe;
  UINT8               MaxStrobe;
  UINT8               VIndex;
  INT16               Margin;
  struct strobeMargin (*DisplayResults)[MAX_VREF_POINTS];
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  UINT8               SubChannel;
  UINT8               SubChannelMaxStrobe;
  UINT8               StrobeLow;
  UINT8               StrobeHigh;

  RankList = GetRankNvList (Host, Socket, Channel, Dimm);
  DisplayResults = Results;


  MaxStrobe = GetMaxStrobe (Host, Socket, Channel, Dimm, Group, DdrLevel);
  SubChannelMaxStrobe = MaxStrobe / 2;

  if (GetDebugLevel () & SDBG_MAX) {

    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

      RcDebugPrint (SDBG_DEFAULT, "Results for ");
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT, "\n");
      RcDebugPrint (SDBG_DEFAULT, "       ");

      for (Strobe = 0; Strobe < SubChannelMaxStrobe; Strobe++) {

        if (IsStrobeNotValid (Host, Strobe)) {
          continue;
        }
        RcDebugPrint (SDBG_DEFAULT, " %3d", Strobe);
      } // strobe loop

      RcDebugPrint (SDBG_DEFAULT, "\n");
      RcDebugPrintLine (SDBG_MAX, 79, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

      StrobeLow = SubChannelMaxStrobe * SubChannel;
      StrobeHigh = SubChannelMaxStrobe * (SubChannel + 1);

      for (VIndex = NumPoints; VIndex > 0; VIndex--) {

        Margin = -MaxVref + ((VIndex - 1) * StepSize);
        RcDebugPrint (SDBG_DEFAULT, "Vref offset: %d\n", Margin);
        // Left Edge
        RcDebugPrint (SDBG_DEFAULT, "Left:  ");

        for (Strobe = StrobeLow; Strobe < StrobeHigh; Strobe++) {
          if (IsStrobeNotValid (Host, Strobe)) {
            continue;
          }
          RcDebugPrint (SDBG_DEFAULT, " %3d", (*DisplayResults)[ (VIndex - 1) ].strobe[Channel][ (*RankList)[Rank].rankIndex][Strobe].n);
        } // strobe loop

        // Right Edge
        RcDebugPrint (SDBG_DEFAULT, "\nRight: ");

        for (Strobe = StrobeLow; Strobe < StrobeHigh; Strobe++) {

          if (IsStrobeNotValid(Host, Strobe)) {
            continue;
          }
          RcDebugPrint(SDBG_DEFAULT, " %3d", (*DisplayResults)[(VIndex - 1)].strobe[Channel][(*RankList)[Rank].rankIndex][Strobe].p);
        } // strobe loop
        RcDebugPrint (SDBG_DEFAULT, "\n");
      } // vIndex loop

      RcDebugPrintLine (SDBG_MAX, 79, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

      // Previous delay
      RcDebugPrint (SDBG_DEFAULT, "Prev Pi:");

      for (Strobe = StrobeLow; Strobe < StrobeHigh; Strobe++) {

        if (IsStrobeNotValid (Host, Strobe)) {
          continue;
        }
        RcDebugPrint (SDBG_DEFAULT, " %3d", OldDelay[Strobe]);
      } // strobe loop

      // Print new delay
      RcDebugPrint (SDBG_DEFAULT, "\nNew Pi: ");

      for (Strobe = StrobeLow; Strobe < StrobeHigh; Strobe++) {

        if (IsStrobeNotValid (Host, Strobe)) {
          continue;
        }
        RcDebugPrint (SDBG_DEFAULT, " %3d", Delay[Strobe]);
      } // strobe loop

      // Print difference
      RcDebugPrint (SDBG_DEFAULT, "\nDiff:   ");

      for (Strobe = StrobeLow; Strobe < StrobeHigh; Strobe++) {

        if (IsStrobeNotValid (Host, Strobe)) {
          continue;
        }
        RcDebugPrint (SDBG_DEFAULT, " %3d", Delay[Strobe] - OldDelay[Strobe]);
      } // strobe loop

      RcDebugPrint (SDBG_DEFAULT, "\n\n");
    }
  }
}

/**

  Displays And Adjust Vref Centering data

  @param[in] Host            Pointer to sysHost
  @param[in] Socket          Processor socket
  @param[in] Group           Parameter to group
  @param[in] BitResults      Pointer to the bit results
  @param[in] StrobeResults   Pointer to the strobe results
  @param[in] PerBit          Vref training mode perbit

  @retval N/A

**/
VOID
DisplayAndAdjustVrefCenteringResultsDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN MRC_GT    Group,
  IN VOID      *BitResults,
  IN VOID      *StrobeResults,
  IN UINT8     PerBit
  )
{
  UINT8                         Ch;
  UINT8                         Dimm;
  UINT8                         Rank;
  UINT8                         RankIndex;
  UINT8                         Strobe;
  UINT8                         Bit;
  UINT8                         MaxChDdr;
  UINT8                         MaxStrobe;
  struct dimmNvram              (*DimmNvList)[MAX_DIMM];
  struct ddrRank                (*RankList)[MAX_RANK_DIMM];
  struct bitMargin              (*VrefMarginBit)[MAX_VREF_TIMING_NUMBER];
  struct strobeMargin           (*VrefMarginStrobe)[MAX_VREF_TIMING_NUMBER];
  UINT8                         TimeIndex;
  UINT8                         MaxBits = GetMaxBitsValid (Host);
  UINT8                         SubChannel;
  UINT8                         MSVx8;
  UINT8                         StrobeInMarginResult;

  MSVx8 = GetMaxStrobeValid (Host) / 2;

  MaxChDdr = GetMaxChDdr ();

  if ((BitResults == NULL) || (StrobeResults == NULL)) {
    RcDebugPrint (SDBG_MAX, "The results pointer is NULL!");
    ASSERT (FALSE);
    return;
  }

  VrefMarginBit = BitResults;
  VrefMarginStrobe = StrobeResults;

  if (GetDebugLevel () & SDBG_MAX) {
    AcquirePrintControl ();

    if (Group == RxVref) {
      RcDebugPrint (SDBG_MAX, "\nSTART_RX_VREF_CENTER\n");
    } else if (Group == RxSamplerEven) {
      RcDebugPrint (SDBG_MAX, "\nSTART_RX_SAMPLER_EVEN_CENTER\n");
    } else if (Group == RxSamplerOdd) {
      RcDebugPrint (SDBG_MAX, "\nSTART_RX_SAMPLER_ODD_CENTER\n");
    } else {
      RcDebugPrint (SDBG_MAX, "\nSTART_TX_VREF_CENTER\n");
    }

  for (Ch = 0;Ch < MaxChDdr; Ch++) {
    //
    // Skip if channel is disabled
    //
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Skip if no DIMM present
      //
      if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
        continue;
      }

      RankList = GetRankNvList (Host, Socket, Ch, Dimm);

      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        //
        // Skip if no rank or if it is mapped out
        //

        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (IsLrdimmPresent (Socket, Ch, Dimm)&& (Group == TxVref) && (Rank > 0)) {
          continue;
        }

        RankIndex = (*RankList)[Rank].rankIndex;

        //
        // print "vref Margins  -"
        //
        RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "vref Margins           -           0           +\n");

        if (PerBit == PER_BIT) {

          for (Bit = 0; Bit < MaxBits; Bit++) {
            //
            // Skip if this is an ECC strobe when ECC is disabled
            //
            if (IsBitNotValid (Host, Bit)) {
              continue;
            }

            if (Bit < (MaxBits / 2)) {
              SubChannel = SUB_CH_A;
            } else {
              SubChannel = SUB_CH_B;
            }

            for (TimeIndex = 0; TimeIndex < MAX_VREF_TIMING_NUMBER; TimeIndex++) {
              if ((*VrefMarginBit)[TimeIndex].bits[Ch][RankIndex][Bit].n >
                  (*VrefMarginBit)[TimeIndex].bits[Ch][RankIndex][Bit].p) {

                  (*VrefMarginBit)[TimeIndex].bits[Ch][RankIndex][Bit].p = 0;
                  (*VrefMarginBit)[TimeIndex].bits[Ch][RankIndex][Bit].n = 0;
              }
            } // timeIndex loop
            RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, Bit % (MaxBits / 2), " %3d : %3d,  %3d : %3d,  %3d : %3d\n",
                                    (*VrefMarginBit)[0].bits[Ch][RankIndex][Bit].n,
                                    (*VrefMarginBit)[0].bits[Ch][RankIndex][Bit].p,
                                    (*VrefMarginBit)[2].bits[Ch][RankIndex][Bit].n,
                                    (*VrefMarginBit)[2].bits[Ch][RankIndex][Bit].p,
                                    (*VrefMarginBit)[1].bits[Ch][RankIndex][Bit].n,
                                    (*VrefMarginBit)[1].bits[Ch][RankIndex][Bit].p
                                    );
          } // bit loop
        } else {
            MaxStrobe = GetMaxStrobe (Host, Socket, Ch, Dimm, Group, DdrLevel);
            for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
              //
              // Skip if this is an ECC strobe when ECC is disabled
              //
              if ((!Host->nvram.mem.eccEn) && IsEccStrobe (Host, Strobe)){
                continue;
              }

              if (Strobe < (MaxStrobe / 2)) {
                SubChannel = SUB_CH_A;
              } else {
                SubChannel = SUB_CH_B;
              }

              StrobeInMarginResult = Strobe;

              if (IsBrsPresent (Host, Socket) == FALSE) {
                //
                // x8 case, the per strobe margin results are stored in strobes 0-4 and 10-14.
                //
                if ((MaxStrobe == MSVx8) && (Strobe >= (MSVx8 / 2))) {
                  StrobeInMarginResult = Strobe + MSVx8 / 2;
                }
              }

              for (TimeIndex = 0; TimeIndex < 3; TimeIndex++) {
                if ((*VrefMarginStrobe)[TimeIndex].strobe[Ch][ (*RankList)[Rank].rankIndex][StrobeInMarginResult].n >
                    (*VrefMarginStrobe)[TimeIndex].strobe[Ch][ (*RankList)[Rank].rankIndex][StrobeInMarginResult].p) {

                    (*VrefMarginStrobe)[TimeIndex].strobe[Ch][ (*RankList)[Rank].rankIndex][StrobeInMarginResult].p = 0;
                    (*VrefMarginStrobe)[TimeIndex].strobe[Ch][ (*RankList)[Rank].rankIndex][StrobeInMarginResult].n = 0;
                }
              } // timeIndex loop
              RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, Strobe % (MaxStrobe / 2), NO_BIT, "  %3d : %3d,  %3d : %3d,  %3d : %3d\n",
                  (*VrefMarginStrobe)[0].strobe[Ch][ (*RankList)[Rank].rankIndex][StrobeInMarginResult].n,
                  (*VrefMarginStrobe)[0].strobe[Ch][ (*RankList)[Rank].rankIndex][StrobeInMarginResult].p,
                  (*VrefMarginStrobe)[2].strobe[Ch][ (*RankList)[Rank].rankIndex][StrobeInMarginResult].n,
                  (*VrefMarginStrobe)[2].strobe[Ch][ (*RankList)[Rank].rankIndex][StrobeInMarginResult].p,
                  (*VrefMarginStrobe)[1].strobe[Ch][ (*RankList)[Rank].rankIndex][StrobeInMarginResult].n,
                  (*VrefMarginStrobe)[1].strobe[Ch][ (*RankList)[Rank].rankIndex][StrobeInMarginResult].p
              );
            } // strobe loop
          }
        } // rank loop
      } // dimm loop
    } // ch loop

    if (Group == RxVref) {
      RcDebugPrint (SDBG_MAX, "STOP_RX_VREF_CENTER\n\n");
    } else if (Group == RxSamplerEven) {
      RcDebugPrint (SDBG_DEFAULT, "STOP_RX_SAMPLER_EVEN_CENTER\n\n");
    } else if (Group == RxSamplerOdd) {
      RcDebugPrint (SDBG_DEFAULT, "STOP_RX_SAMPLER_ODD_CENTER\n\n");
    } else {
      RcDebugPrint (SDBG_MAX, "STOP_TX_VREF_CENTER\n\n");
    }

    ReleasePrintControl ();
  }
}
