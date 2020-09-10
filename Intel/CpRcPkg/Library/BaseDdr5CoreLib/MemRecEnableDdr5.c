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
#include <Library/EnhancedWarningLogLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Memory/JedecDefinitions.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Memory/Ddr5RcwRegs.h>
#include <Library/SimulationServicesLib.h>

#define REC_EN_COUNT_WINDOW  8
#define MIN_IO_LATENCY 3


//
// Local function prototypes
//
STATIC
VOID
FindPiSettingDdr5 (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Dimm,
  UINT8     Rank,
  UINT16    RisingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  UINT16    CenterPoint[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  UINT16    FallingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  UINT16    PulseWidth[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  struct    TrainingResultsDdr5 (*TrainResInput) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

STATIC
VOID
PISettingAdjustmentDdr5 (
  PSYSHOST                    Host,
  UINT8                       Socket,
  UINT8                       Dimm,
  UINT8                       Rank,
  struct TrainingResultsDdr5  (*TrainResInput) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

/**

 For each data module, a 128 bit vector of zeros and ones holding the tests results is
 evaluated which is a concatenation of the two reads from the feedback CRs.
 BIOS locates the middle of the longest consecutive stream of ones (when the vector is evaluated as cyclic,
 this location in the vector indicates the PI setting and logic delay for that data module).

  @param[in]  Socket        - Current Socket
  @param[in]  Ch            - Channel number
  @param[in]  Subch         - Sub channel number
  @param[in]  Dimm          - Dimm number on Ch
  @param[in]  Rank          - Rank on Dimm
  @param[in]  Strobe        - Strobe number
  @param[in]  RisingEdge    - Pointer to save the rising edge
  @param[in]  CenterPoint   - Pointer to save the center point
  @param[in]  FallingEdge   - Pointer to save the falling edge
  @param[in]  PulseWidth    - Pointer to save the pulse width
  @param[in]  Step          - The PI Step to work on:
                                FPT_PI_READ_TYPE FPT_PI_WRITE_TYPE
                                FPT_PI_LRDIMM_READ_TYPE
                                FPT_PI_LRDIMM_WRITE_TYPE
                                FPT_PI_LRDIMM_RD_MRD_TYPE
                                FPT_PI_LRDIMM_WR_MRD_TYPE
  @param[out] TrainResInput - Training result

  @retval N/A

**/
VOID
EFIAPI
EvaluatePiSettingResultsDdr5 (
  IN UINT8                       Socket,
  IN UINT8                       Ch,
  IN UINT8                       Subch,
  IN UINT8                       Dimm,
  IN UINT8                       Rank,
  IN UINT8                       Strobe,
  IN UINT16                      RisingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN UINT16                      CenterPoint[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN UINT16                      FallingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN UINT16                      PulseWidth[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN UINT8                       Step,
  OUT struct TrainingResultsDdr5 (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

STATIC
VOID
FindRoundTripLatencyDdr5 (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Dimm,
  UINT8     Rank,
  UINT16    CenterPoint[MAX_CH][SUB_CH][MAX_STROBE_DDR5/2]
  );

VOID
GetResultsDdr5 (
  PSYSHOST Host,
  UINT8 Socket,
  UINT8 Dimm,
  UINT8 Rank,
  UINT16 PiDelay,
  struct TrainingResultsDdr5 (*TrainResInput) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );



/**

  Enable/disable read preamble training mode on the DRAM.

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket number
  @param[in] Ch         - Channel number
  @param[in] SubChMask  - Mask of subchannels to be programmed
  @param[in] Dimm       - DIMM number
  @param[in] Rank       - Rank number
  @param[in] Enable     - TRUE - Enable DRAM read preamble training mode
                          FALSE - Disable DRAM read preamble training mode

  @retval N/A

**/
VOID
SetDramReceiveEnableModeDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     SubChMask,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN BOOLEAN   Enable
  )
{
  DDR5_MODE_REGISTER_2_STRUCT Ddr5Mr2;

  // Do not enable Read Preamble Training Mode in DRAM for LRDIMM
  if (IsLrdimmPresent (Socket, Ch, Dimm) == FALSE) {
    ReadDramModeRegCachePerRank (Socket, Ch, SUB_CH_A, Dimm, Rank, DDR5_MODE_REGISTER_2_REG, &Ddr5Mr2.Data);
    //
    // Enable Read Preamble Training Mode in Dram.
    //
    if (Enable == TRUE) {
#if USE_LATEST_DRAM_SPEC
      Ddr5Mr2.Bits.read_preamble_training = 1;
#else
      Ddr5Mr2.Bits.read_preamble_enable = 1;
#endif //USE_LATEST_DRAM_SPEC
    } else {
#if USE_LATEST_DRAM_SPEC
      Ddr5Mr2.Bits.read_preamble_training = 0;
#else
      Ddr5Mr2.Bits.read_preamble_enable = 0;
#endif //USE_LATEST_DRAM_SPEC
    }
    DramModeRegWrite (Socket, Ch, SubChMask, Dimm, Rank, DDR5_MODE_REGISTER_2_REG, Ddr5Mr2.Data);
    //
    // SPR_TODO. whether need to check the preamble state then call the SetLongReadPreamble?
    //
  }
} // SetDdrReceiveEnableMode


  /**

  Get the receive enable read preamble final offset for DDR5.

  @param Host    - Pointer to sysHost
  @param Socket  - Socket number.

  @retval INT16

  **/

INT16
GetReadPreambleFinalOffsetDdr5 (
  PSYSHOST  Host,
  UINT8     Socket
  )
{
  UINT16 PreambleLen = 0;
  INT16  PreambleOffset = 0;

  PreambleLen = GetPreambleState (Socket, READ_PREAMBLE);
  switch (PreambleLen) {
    case PREAMBLE_1TCLK:
      PreambleOffset = 160;
      break;
    case PREAMBLE_2TCLK:
      PreambleOffset = 64;
      break;
    case PREAMBLE_3TCLK:
      PreambleOffset = 0;
      break;
    case PREAMBLE_4TCLK:
      PreambleOffset = -128;
      break;
    default:
      PreambleOffset = 160;
      break;
  }
  return PreambleOffset;
} // GetPreambleFinalOffsetDdr5

/**
  This function gets the test engine ready to run tests for Receive Enable Fine training

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket number
  @param[in] ChBitmask  - Bitmask of channels to test
  @param[in] Dimm       - Dimm to test
  @param[in] Rank       - Rank number
**/
VOID
SetupRecEnFineTestDdr5 (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT32   ChBitmask,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  UINT8               LoopCount;
  UINT8               NumCl;
  MRC_RT              MrcRankList;
  MRC_MST             MemSsType;

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  MrcRankList.Dimm = Dimm;
  MrcRankList.Rank = Rank;

  //
  // Only take the DdrioUltSupport path on CTE builds
  //
  if (Host->DdrioUltSupport) {

    LoopCount = 2;
    NumCl     = 32;

    CteSetupPattern ("rcven", (LoopCount) | (NumCl << 4), ChBitmask, 1, GetPhyRank (Dimm, Rank));
  } else {
    SetupTest (Host, Socket, MemSsType, ChBitmask, 1, &MrcRankList, DdrLevel, RcvenFineTest, gsmCsnDelim, 0, FIRST_ITERATION_LOOP);
  }

  return;

} // SetupRecEnFineTestDdr5

/**

  Pre/Post Receive Enable training configuration for the data buffer.

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket number
  @param[in] Channel     - Channel number
  @param[in] Dimm        - DIMM number in channel
  @param[in] Rank        - Rank number on DIMM
  @param[in] Enable      - TRUE to enable DB training for RecEn
                           FALSE to disable DB training for RecEn

  @retval N/A

**/
VOID
SetDbReceiveEnableModeDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN BOOLEAN   Enable
  )
{
#ifdef  LRDIMM_SUPPORT
  DDR5_DATA_BUFFER_RW83_STRUCT  DataBufferRw83;

  // If LRDIMM present, configure the data buffer for receive enable training
  if (IsLrdimmPresent (Socket, Channel, Dimm)) {

    if (Enable == TRUE) {
      // Set the DBs to Host Pre-Amble training mode
      DataBufferRw83.Data = 0;
      DataBufferRw83.Bits.hpa = 1;
      DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW83_REG, DataBufferRw83.Data);

    } else {
      // Disable Host Pre-Amble training mode
      DataBufferRw83.Data = 0;
      DataBufferRw83.Bits.hpa = 0;
      DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW83_REG, DataBufferRw83.Data);
    }
  }
#endif // LRDIMM_SUPPORT
} // SetDbReceiveEnableMode

/**

  Equalize the Roundtrip latency between subch A and subch B.

  @param[in] Socket  - CPU socket number
  @param[in] Ch      - Channel number
  @param[in] Dimm    - Dimm number
  @param[in] Rank    - Rank number
  @param[in] LogRank - Logical rank number

  @retval status

**/
VOID
EqualizeRoundTripLatencyScaScb (
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     LogRank
  )
{
  UINT8                             IoLatency;
  PSYSHOST                          Host;
  UINT8                             RoundTripSca = 0;
  UINT8                             RoundTripScb = 0;
  UINT8                             RoundTripSkew = 0;

  Host = GetSysHostPointer ();

  if (IsBrsPresent (Host, Socket)) {
    //
    // There is no requirement to program the two subchannels' roundtrip latency with the same value in BRS.
    //
    return;
  }
  //
  // Check the round trip value in two sub channels. Program it with same value in sub channel A and sub channel B.
  //
  GetRoundTripDdr5 (Socket, Ch, SUB_CH_A, LogRank, GSM_READ_ONLY, &RoundTripSca);
  GetRoundTripDdr5 (Socket, Ch, SUB_CH_B, LogRank, GSM_READ_ONLY, &RoundTripScb);
  if (RoundTripSca != RoundTripScb) {
    RoundTripSkew = ABS (RoundTripSca - RoundTripScb);
    if (RoundTripSca < RoundTripScb) {
      SetRoundTripAll (Socket, Ch, SUB_CH_A, RoundTripScb);
      GetIoLatencyDdr5 (Socket, Ch, SUB_CH_A, LogRank, GSM_READ_ONLY, &IoLatency);
      SetIoLatencyDdr5 (Socket, Ch, SUB_CH_A, LogRank, IoLatency + RoundTripSkew);
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SUB_CH_A, Rank, NO_STROBE, NO_BIT,
        "New Round Trip = %d, New IO Latency = %d\n", RoundTripScb, IoLatency + RoundTripSkew);
    } else {
      SetRoundTripAll (Socket, Ch, SUB_CH_B, RoundTripSca);
      GetIoLatencyDdr5 (Socket, Ch, SUB_CH_B, LogRank, GSM_READ_ONLY, &IoLatency);
      SetIoLatencyDdr5 (Socket, Ch, SUB_CH_B, LogRank, IoLatency + RoundTripSkew);
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SUB_CH_B, Rank, NO_STROBE, NO_BIT,
        "New Round Trip = %d, New IO Latency = %d\n", RoundTripSca, IoLatency + RoundTripSkew);
    }
  }
} // EqualizeRoundTripLatencyScaScb

/**
  Display Receive Enable summary.

  @param[in]  Socket    Socket number
  @param[in]  Channel   Channel number in socket
  @param[in]  Dimm      DIMM number in channel
  @param[in]  Rank      Rank number in DIMM
  @param[in]  RoundTripFound  Array that stores the individual Round Trip values for each
                              channel/subchannel/dimm/rank
**/
VOID
DisplayReceiveEnableSummary (
  IN  UINT8   Socket,
  IN  UINT8   Channel,
  IN  UINT8   Dimm,
  IN  UINT8   Rank,
  IN  UINT8   RoundTripFound[MAX_CH][SUB_CH][MAX_DIMM][MAX_RANK_DIMM]     OPTIONAL
  )
{
#ifdef DEBUG_CODE_BLOCK
  PSYSHOST  Host;
  UINT8     SubCh;
  UINT8     ChannelLocal;
  UINT8     LogRank;
  UINT8     SubChMSVx4;
  UINT8     Strobe;
  UINT8     StrobeLocal;
  UINT32    DbgLevel;
  INT16     PiDelay[SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT8     IoLatency[SUB_CH];
  UINT8     RoundTrip[SUB_CH];

  //
  // Every print in this function should use the same level.
  //
  DbgLevel = SDBG_MAX;

  //
  // If this function can't print, return immediately.
  //
  if ((GetDebugLevel () & DbgLevel) == 0) {
    return;
  }

  Host = GetSysHostPointer ();
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  LogRank = GetLogicalRank (Host, Socket, Channel, Dimm, Rank);

  //
  // Gather data up-front so any prints by these functions do not interrupt
  // the summary below.
  //
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    ChannelLocal = Channel;
    GetChannelMapping (Channel, SubCh, &ChannelLocal);

    for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
      if (IsStrobeNotValidDdr5 (Host, Strobe)) {
        continue;
      }

      StrobeLocal = Strobe;
      GetSetDataGroup (Host, Socket, ChannelLocal, SubCh, Dimm, Rank, StrobeLocal, 0, DdrLevel, RecEnDelay, GSM_READ_ONLY, &PiDelay[SubCh][Strobe]);

    } // Strobe read loop

    GetIoLatencyDdr5 (Socket, Channel, SubCh, LogRank, GSM_READ_ONLY, &IoLatency[SubCh]);
    if (RoundTripFound != NULL) {
      RoundTrip[SubCh] = RoundTripFound[Channel][SubCh][Dimm][Rank];
    } else {
      GetRoundTripDdr5 (Socket, Channel, SubCh, LogRank, GSM_READ_ONLY, &RoundTrip[SubCh]);
    }


  } // SubCh read loop

  AcquirePrintControl ();
  RcDebugPrint (DbgLevel, "\n Receive Enable Summary\n");
  RcDebugPrint (DbgLevel, "------------------------\n");

  //
  // Print a header row for strobe columns.
  //
  RcDebugPrint (DbgLevel, "                  ");
  for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    RcDebugPrint (DbgLevel, " %3d", Strobe);
  } // Strobe header loop
  RcDebugPrint (DbgLevel, "\n");

  //
  // Print the data.
  //
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

    RcDebugPrintWithDevice (DbgLevel, Socket, Channel, Dimm, SubCh, Rank, NO_STROBE, NO_BIT, "");
    for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
      if (IsStrobeNotValidDdr5 (Host, Strobe)) {
        continue;
      }
      RcDebugPrint (DbgLevel, " %3d", PiDelay[SubCh][Strobe]);
    } // Strobe print loop

    RcDebugPrint (DbgLevel, "\n");

    RcDebugPrintWithDevice (
      DbgLevel, Socket, Channel, Dimm, SubCh, Rank, NO_STROBE, NO_BIT,
      "IO Latency = %d, Round Trip = %d\n",
      IoLatency[SubCh],
      RoundTrip[SubCh]
      );

  } // SubCh print loop

  ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK
}

/**
  Save RxStrobeInvert for each strobe and temporarily set the register to 0
  for ReceiveEnable training

  @param[in]  Host                - Pointer to sysHost
  @param[in]  Socket              - Socket number
  @param[out] PreviousStrobeInv   - Storage for each strobe in the socket

  @retval N/A

**/
VOID
ReceiveEnabledRxStrobeInvInit (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket,
     OUT  UINT8       PreviousStrobeInv[MAX_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_STROBE]
  )
{
  UINT8                             Channel;
  UINT8                             Dimm;
  UINT8                             Rank;
  UINT8                             Strobe;
  UINT8                             MaxChDdr;
  UINT8                             NumRanks;
  INT16                             RxStrobeInvertValue;
  UINT8                             MSVx4;

  MaxChDdr = GetMaxChDdr ();
  MSVx4 = GetMaxStrobeValid (Host);

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
        continue;
      }

      NumRanks = Host->var.mem.socket[Socket].maxRankDimm;
      for (Rank = 0; Rank < NumRanks; Rank++) {

        if (!IsRankValid (Socket, Channel, Dimm, Rank, FALSE)) {
          continue;
        }

        for (Strobe = 0; Strobe < MSVx4; Strobe++) {

          if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
              continue;
          }
          //
          // Save the current value and write 0 if it isn't already 0
          //
          GetSetDataGroup (Host, Socket, Channel, NO_SUBCH, Dimm, Rank, Strobe, ALL_BITS, DdrLevel, RxStrobeInvert,
            GSM_READ_ONLY, &RxStrobeInvertValue);

          PreviousStrobeInv[Channel][Dimm][Rank][Strobe] = (UINT8)RxStrobeInvertValue;

          if (RxStrobeInvertValue != 0) {

            RxStrobeInvertValue = 0;
            GetSetDataGroup (Host, Socket, Channel, NO_SUBCH, Dimm, Rank, Strobe, ALL_BITS, DdrLevel, RxStrobeInvert,
              GSM_FORCE_WRITE, &RxStrobeInvertValue);

          }

        } // Strobe
      } // Rank
    } // Dimm
  } // Channel

}

/**
  Restore RxStrobeInvert for each strobe and adjust ReceiveEnableDelay if needed

  @param[in]  Host                - Pointer to sysHost
  @param[in]  Socket              - Socket number
  @param[in]  PreviousStrobeInv   - Values to restore for each strobe in the socket

  @retval N/A

**/
VOID
ReceiveEnabledRxStrobeInvRestore (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket,
  IN      UINT8       PreviousStrobeInv[MAX_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_STROBE]
  )
{
  UINT8                             Channel;
  UINT8                             Dimm;
  UINT8                             Rank;
  UINT8                             Strobe;
  UINT8                             MaxChDdr;
  UINT8                             NumRanks;
  INT16                             RxStrobeInvertValue;
  INT16                             ReceiveEnableOffset;
  UINT8                             MSVx4;

  MaxChDdr = GetMaxChDdr ();
  MSVx4 = GetMaxStrobeValid (Host);
  ReceiveEnableOffset = 0;

  for (Channel = 0; Channel < MaxChDdr; Channel += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
        continue;
      }

      NumRanks = Host->var.mem.socket[Socket].maxRankDimm;
      for (Rank = 0; Rank < NumRanks; Rank++) {

        if (!IsRankValid (Socket, Channel, Dimm, Rank, FALSE)) {
          continue;
        }

        for (Strobe = 0; Strobe < MSVx4; Strobe++) {

          if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
              continue;
          }
          //
          // Restore previous RxStrobeInvert value if needed
          //
          GetSetDataGroup (Host, Socket, Channel, NO_SUBCH, Dimm, Rank, Strobe, ALL_BITS, DdrLevel, RxStrobeInvert,
            GSM_READ_ONLY, &RxStrobeInvertValue);

          if (RxStrobeInvertValue != PreviousStrobeInv[Channel][Dimm][Rank][Strobe]) {

            RxStrobeInvertValue = PreviousStrobeInv[Channel][Dimm][Rank][Strobe];
            GetSetDataGroup (Host, Socket, Channel, NO_SUBCH, Dimm, Rank, Strobe, ALL_BITS, DdrLevel, RxStrobeInvert,
              GSM_FORCE_WRITE, &RxStrobeInvertValue);

          }
          if (RxStrobeInvertValue != 0) {
            //
            // ReceiveEnable always trains with RxStrobeInvert = 0. Previous RxStrobeInvert value was saved.
            // At the end of ReceiveEnable training, RxStrobeInvert is restored to the previous setting
            // and ReceiveEnableDelay is adjusted, if needed, based on this table.
            //
            // +-------------------------------------------------------------------------------------+
            // |              DDR5 ReceiveEnableDelay Adjustment Based on RxStrobeInvert             |
            // +-------------------------+-----------------------------+-----------------------------+
            // | Read Pre-amble (DDR5)   | RxStrobeInvert: from 0 to 1 | RxStrobeInvert: from 1 to 0 |
            // | 1 tCK (10)              |            -32              |            +32              |
            // | 2 tCK (0010)            |              0              |              0              |
            // | 2 tCK DDR4-like (1110)* |            -32              |            +32              |
            // | 3 tCK (000010)          |              0              |              0              |
            // | 4 tCK (00001010)        |              0              |              0              |
            // +-------------------------+-----------------------------+-----------------------------+
            //   * 2 tCK DDR4-like (1110) is not used
            //
            if (GetPreambleState (Socket, READ_PREAMBLE) == PREAMBLE_1TCLK) {
              ReceiveEnableOffset = -RECEIVE_ENABLE_OFFSET_32;
              GetSetDataGroup (Host, Socket, Channel, NO_SUBCH, Dimm, Rank, Strobe, ALL_BITS, DdrLevel, RecEnDelay,
                GSM_UPDATE_CACHE | GSM_WRITE_OFFSET, &ReceiveEnableOffset);
            }
          }

        } // Strobe
      } // Rank
    } // Dimm
  } // Channel

}

/**

  Perform Receive Enable Training

  @param Host  - Pointer to sysHost

  @retval status

  **/
UINT32
ReceiveEnableDdr5 (
  PSYSHOST  Host
  )
{
  UINT8                             Socket;
  UINT8                             Ch;
  UINT8                             Dimm;
  UINT8                             Rank;
  UINT8                             LogRank = 0;
  UINT8                             Strobe = 0;
  UINT8                             TempStrobe = 0;
  UINT8                             SubCh = 0;
  UINT8                             NumRanks;
  UINT8                             IoLatency;
  UINT8                             RoundTrip;
  UINT8                             RoundTripOriginal[MAX_CH][SUB_CH][MAX_DIMM][MAX_RANK_DIMM];
  UINT8                             RoundTripFound[MAX_CH][SUB_CH][MAX_DIMM][MAX_RANK_DIMM];
  UINT8                             MinRoundTripLatencyChannel;
  UINT8                             MaxRoundTripLatencyChannel;
  INT16                             PreambleOffset = 0;
  UINT16                            TempOffset = 0;
  UINT8                             RtError;
  UINT8                             RankPresent;
  UINT8                             SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8                             ChannelLocal = 0;
  UINT8                             StrobeLocal = 0;
  UINT16                            RisingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT16                            CenterPoint[MAX_DIMM][MAX_RANK_DIMM][MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT16                            FallingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT16                            PulseWidth[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  struct channelNvram               (*ChannelNvList) [MAX_CH];
  struct dimmNvram                  (*DimmNvList) [MAX_DIMM];
  struct ddrRank                    (*RankList) [MAX_RANK_DIMM];
  struct RankCh                     (*RankPerCh) [MAX_RANK_CH];
  struct rankDevice                 (*RankStruct) [MAX_RANK_DIMM];
  struct TrainingResultsDdr5        (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  ACT_TO_ACT_SAVE_STRUCT            ActToActSave[MAX_CH];
  UINT8                             MaxChDdr;
  UINT8                             RoundTripStepUnit;
  MRC_STATUS                        Status;
  UINT8                             PreviousStrobeInv[MAX_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_STROBE];

  MaxChDdr = GetMaxChDdr ();
  RoundTripStepUnit = GetRoundTripStepUnit ();

  Socket = Host->var.mem.currentSocket;
  TrainRes = RcAllocatePool (sizeof (*TrainRes));
  ZeroMem ((UINT8 *) RisingEdge, sizeof (RisingEdge));
  ZeroMem ((UINT8 *) CenterPoint, sizeof (CenterPoint));
  ZeroMem ((UINT8 *) FallingEdge, sizeof (FallingEdge));
  ZeroMem ((UINT8 *) PulseWidth, sizeof (PulseWidth));

  //
  // Clear the training result.
  //
  ZeroMem ((UINT8 *) TrainRes, sizeof (*TrainRes));
  SetCurrentTestType (Socket, RcvenFineTest);
  //
  // Return if Socket is disable or No DIMMs found or RECVEN training disabled
  //
  if ((!IsMemFlowEnabled (ReceiveEnableTraining)) || (Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
    return SUCCESS;
  }

  DoCompDdrioPerSetupOption (Socket, TRAINING_COMP_EVERY_TRAINING);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    ActToActSave[Ch].TRrdSOrg = 0;
    ActToActSave[Ch].TRrdLOrg = 0;
    ActToActSave[Ch].TRcdRdOrg = 0;
    ActToActSave[Ch].TRrsrOrg = 0;
    ActToActSave[Ch].TRrSgOrg = 0;
  }

  ReceiveEnableInitAllRanksDdr5 (Host, Socket);
  //
  // RxStrobeInv must be 0 during Receive Enable training
  // Save current setting for each strobe and temporarily set them all to 0
  //
  ReceiveEnabledRxStrobeInvInit (Host, Socket, PreviousStrobeInv);

  SetAepTrainingMode (Host, Socket, CHECKPOINT_MINOR_RX_BACKSIDE_PHASE_TRAINING);

  ChannelNvList = GetChannelNvList (Host, Socket);
  PreambleOffset = GetReadPreambleFinalOffsetDdr5 (Host, Socket);
  SetTrainingModeDdr5 (Socket, TmReceiveEnable);

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }

        LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);

        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          GetRoundTripDdr5 (Socket, Ch, SubCh, LogRank, GSM_READ_ONLY, &RoundTripOriginal[Ch][SubCh][Dimm][Rank]);
          RcDebugPrintWithDevice (
            SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT,
            "Save RoundTripOriginal = %3d\n",
            RoundTripOriginal[Ch][SubCh][Dimm][Rank]
            );
        }
      }
    }
  }

  //
  // Train each DIMM
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    //
    // Train each Rank
    //
    NumRanks = Host->var.mem.socket[Socket].maxRankDimm;
    for (Rank = 0; Rank < NumRanks; Rank++) {
      OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_RECEIVE_ENABLE_BASIC, (UINT16) ((Socket << 8) | (Dimm << 4) | Rank)));
      //
      // Initialize to no ranks present
      //
      RankPresent = 0;

      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }

        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);

        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }
        //
        // Init DRAM interface only for CTE.
        //
        if (FeaturePcdGet (PcdCteBuild) == TRUE) {
          InitDramInterface (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, Rank);
        }
        if (Host->DdrioUltSupport) {
          //
          // LoopCount = 2
          // NumCL = 32
          //
          CteSetupPattern ("rcven", (2) | (32 << 4), 1<<Ch, 3, GetPhyRank (Dimm, Rank));
        }

        //
        // Enable Preamble Training Mode in DRAM
        //
        if (IsDcpmmPresentDimm (Socket, Ch, Dimm)) {
          SetFMCReceiveEnableModeDdrt2 (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, TRUE);
        } else {
          SetDramReceiveEnableModeDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, TRUE);
        }

        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          ChannelLocal = Ch;
          GetChannelMapping (Ch, SubCh, &ChannelLocal);
          //
          // Get the logical Rank #
          //
          LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);

          SetActToActPeriod(Host, Socket, ChannelLocal, ActToActSave);//SPR_TODO.
          //
          // Indicate this Rank is present on at least one channel
          //
          RankPresent = 1;

          SelectCPGCRanks(Host, Socket, ChannelLocal, 1 << LogRank, 0, 0);//SPR_TODO.
        } //Subch loop

        SetDbReceiveEnableModeDdr5 (Host, Socket, Ch, Dimm, Rank, TRUE);

        SetMprModeFilterActPre (Host, Socket, Ch, 1);
        SetMprTrainModeDdr5 (Host, Socket, Ch, 1);
        ConvertRdToMrr (Host, Socket, Ch, 1);

      } // Ch loop

      //
      // Go to the next Rank if this Rank is not present on any channels
      //
      if (!RankPresent) {
        continue;
      }

      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }

        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

          SetRoundTripAll (Socket, Ch, SubCh, RoundTripOriginal[Ch][SubCh][Dimm][Rank]);

        }
      }

      SetDqCountingWindow (Host, Socket, Dimm, Rank);
      if (IsBrsPresent (Host, Socket)) {
        SetTrainingMode (Host, Socket, Dimm, Rank, CHECKPOINT_MINOR_RECEIVE_ENABLE_BASIC);
      }
      //
      // Find the DQS high phase
      //
      ZeroMem ((UINT8 *) TrainRes, sizeof (*TrainRes));
      FindPiSettingDdr5 (Host, Socket, Dimm, Rank, RisingEdge, CenterPoint[Dimm][Rank], FallingEdge, PulseWidth, TrainRes);
      DetectLowMargin (Host, NO_CH, Dimm, Rank, PulseWidth, RecEnDelay, NON_PER_BIT);

#ifdef DEBUG_CODE_BLOCK
      if (GetDebugLevel () & SDBG_MAX) {
        AcquirePrintControl ();
      }
#endif // DEBUG_CODE_BLOCK
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }

        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        DimmNvList = GetDimmNvList (Host, Socket, Ch);

        // Continue to the next Rank if this one is disabled
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }
        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          //
          // Preparation for back in time
          //
          RankPerCh = &(*ChannelNvList) [Ch].rankPerCh;

          //
          // Write the PI setting to MC
          //
          for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
            //
            // Skip if this is an ECC Strobe when ECC is disabled
            //
            if (IsStrobeNotValidDdr5 (Host, Strobe)) {
              continue;
            }
            ChannelLocal = Ch;
            GetChannelMapping (Ch, SubCh, &ChannelLocal);
            RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, Strobe, NO_BIT,
              "Rec En Delay trained value %d\n", CenterPoint[Dimm][Rank][Ch][SubCh][Strobe]);
            GetSetDataGroup (Host, Socket, ChannelLocal, SubCh, Dimm, Rank, Strobe, 0, DdrLevel, RecEnDelay, GSM_UPDATE_CACHE,
              (INT16 *)&CenterPoint[Dimm][Rank][Ch][SubCh][Strobe]);
            RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, Strobe, NO_BIT,
              "Rec En Delay %d\n", CenterPoint[Dimm][Rank][Ch][SubCh][Strobe]);
          } // Strobe loop
          //
          // Delay if this is a CTE build, otherwise do nothing.
          //
          CteDelayDclk (100); // To prevent BIOS programming the rcven dly too fast and causing an DDRIO assertion to fire.

          LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);

          GetIoLatencyDdr5 (Socket, Ch, SubCh, LogRank, GSM_READ_ONLY, &IoLatency);
          GetRoundTripDdr5 (Socket, Ch, SubCh, LogRank, GSM_READ_ONLY, &RoundTrip);

          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT,
            "Round trip latency = %d\nIO latency = %d\n",
            RoundTrip, IoLatency);
        } //subchannel loop
      } // Ch loop

      //
      // Find out the round trip latency
      //
      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "\nRound trip latency\n");
#ifdef DEBUG_CODE_BLOCK
      if (GetDebugLevel () & SDBG_MAX) {
        ReleasePrintControl ();
      }
#endif // DEBUG_CODE_BLOCK
      FindRoundTripLatencyDdr5 (Host, Socket, Dimm, Rank, CenterPoint[Dimm][Rank]);
      //
      // Save the found RoundTripFound Latency for Dimm/Rank
      //
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }

        LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);

        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          GetRoundTripDdr5 (Socket, Ch, SubCh, LogRank, GSM_READ_ONLY, &RoundTripFound[Ch][SubCh][Dimm][Rank]);
        }

        // Disable Receive Enable Mode in Data Buffer
        SetDbReceiveEnableModeDdr5 (Host, Socket, Ch, Dimm, Rank, FALSE);


      }

      if (IsBrsPresent (Host, Socket)) {
        //
        // Notify IO to move out of RCVEN training mode
        //
        SetTrainingMode (Host, Socket, Dimm, Rank, CLEAR_MODE);
      }
      //
      // Disable Receive Enable Mode in DRAM
      // BRS_TODO: Could this be moved earlier where DB disable occurs?
      //
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }

        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }
        //
        // Disable Read Preamble Training Mode in Dram
        //
        SetDramReceiveEnableModeDdr5 (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, FALSE);
      } // Ch loop
    } // Ranks loop
  } // Dimms Loop

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    //
    // Train each Rank
    //
    NumRanks = Host->var.mem.socket[Socket].maxRankDimm;
    for (Rank = 0; Rank < NumRanks; Rank++) {
      //
      // Initialize to no ranks present
      //
      RankPresent = 0;

      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }

        DimmNvList = GetDimmNvList (Host, Socket, Ch);

        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }
        //
        // Indicate this Rank is present on at least one channel
        //
        RankPresent = 1;
      } // Ch loop

      //
      // Go to the next Rank if this Rank is not present on any channels
      //
      if (!RankPresent) {
        continue;
      }

      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }

        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        DimmNvList = GetDimmNvList (Host, Socket, Ch);

        // Continue to the next Rank if this one is disabled
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }

        LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);
        RankPerCh = &(*ChannelNvList) [Ch].rankPerCh;
        if (FeaturePcdGet (PcdCteBuild) == FALSE) {
          SetMprModeFilterActPre (Host, Socket, Ch, 0);
        }
        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          ChannelLocal = Ch;
          GetChannelMapping (Ch, SubCh, &ChannelLocal);
          // Setup the new Pi setting
          //
          for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
            //
            // Skip if this is an ECC Strobe when ECC is disabled
            //
            if (IsStrobeNotValidDdr5 (Host, Strobe)) {
              continue;
            }

            if ((CenterPoint[Dimm][Rank][Ch][SubCh][Strobe] + PreambleOffset) < 0) {
              //
              // Need to adjust the center point down but there is not enough range, so subtract 1 or 2 dclk from roundtrip
              //  and add corresponding number pi ticks equal to a cycle to all strobes center values for this Rank
              //
              RoundTrip = RoundTripFound[Ch][SubCh][Dimm][Rank];
              if (PreambleOffset >= -128) {
                //
                // subtract RTL by 1 DCLK
                //
                RoundTripFound[Ch][SubCh][Dimm][Rank] = RoundTrip - RoundTripStepUnit;
                TempOffset = 128;
              } else if (PreambleOffset < -128) {
                //
                // subtract RTL by 2 DCLK
                //
                RoundTrip = RoundTrip - (2 * RoundTripStepUnit);
                TempOffset = 256;
              }
              RoundTripFound[Ch][SubCh][Dimm][Rank] = RoundTrip;
              for (TempStrobe = 0; TempStrobe < SubChMSVx4; TempStrobe++) {
                CenterPoint[Dimm][Rank][Ch][SubCh][TempStrobe] = CenterPoint[Dimm][Rank][Ch][SubCh][TempStrobe] + TempOffset;
              } //TempStrobe
            }
            CenterPoint[Dimm][Rank][Ch][SubCh][Strobe] = CenterPoint[Dimm][Rank][Ch][SubCh][Strobe] + PreambleOffset;
            StrobeLocal = Strobe;
            GetSetDataGroup (Host, Socket, ChannelLocal, SubCh, Dimm, Rank, StrobeLocal, 0, DdrLevel, RecEnDelay, GSM_UPDATE_CACHE,
              (INT16 *)&CenterPoint[Dimm][Rank][Ch][SubCh][Strobe]);
          } // Strobe loop
        } //subchannel loop.

        DisplayReceiveEnableSummary (Socket, Ch, Dimm, Rank, RoundTripFound);

        //
        // Equalize the roundtrip latency value in two sub channels.
        //
        EqualizeRoundTripLatencyScaScb (Socket, Ch, Dimm, Rank, LogRank);
      } // Ch loop


    } // Rank loop
  } // Dimm loop

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {

        DimmNvList = GetDimmNvList (Host, Socket, Ch);

        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }

        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          RcDebugPrintWithDevice (
            SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT,
            "RoundTripFound = %3d\n",
            RoundTripFound[Ch][SubCh][Dimm][Rank]
            );
        }
      }
    }

    MinRoundTripLatencyChannel = MAX_UINT8;
    MaxRoundTripLatencyChannel = 0;

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

      NumRanks = Host->var.mem.socket[Socket].maxRankDimm;
      for (Rank = 0; Rank < NumRanks; Rank++) {

        DimmNvList = GetDimmNvList (Host, Socket, Ch);

        // Continue to the next Rank if this one is disabled
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }

        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          if (MinRoundTripLatencyChannel > RoundTripFound[Ch][SubCh][Dimm][Rank]) {

            MinRoundTripLatencyChannel = RoundTripFound[Ch][SubCh][Dimm][Rank];

          }
          if (MaxRoundTripLatencyChannel < RoundTripFound[Ch][SubCh][Dimm][Rank]) {

            MaxRoundTripLatencyChannel = RoundTripFound[Ch][SubCh][Dimm][Rank];

          }
        } // SubCh
      } // Rank loop
    } // Dimm loop

    RcDebugPrintWithDevice (
      SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "MinRoundTripLatencyChannel = %3d MaxRoundTripLatencyChannel = %3d\n",
      MinRoundTripLatencyChannel,
      MaxRoundTripLatencyChannel
      );

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      SetRoundTripAll (Socket, Ch, SubCh, MaxRoundTripLatencyChannel);
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      NumRanks = Host->var.mem.socket[Socket].maxRankDimm;
      for (Rank = 0; Rank < NumRanks; Rank++) {

        DimmNvList = GetDimmNvList (Host, Socket, Ch);

        // Continue to the next Rank if this one is disabled
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }

        LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);

        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

          GetIoLatencyDdr5 (Socket, Ch, SubCh, LogRank, GSM_READ_ONLY, &IoLatency);
          RcDebugPrintWithDevice (
            SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT,
            "IoLatency = %3d ",
            IoLatency
            );

          IoLatency += MaxRoundTripLatencyChannel - RoundTripFound[Ch][SubCh][Dimm][Rank];

          if (IoLatency < MIN_IO_LATENCY) {
            RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, 0);
          }

          SetIoLatencyDdr5 (Socket, Ch, SubCh, LogRank, IoLatency);
          RcDebugPrint (
            SDBG_MAX,
            "NewIoLatency = %3d \n",
            IoLatency
            );

        }

        DisplayReceiveEnableSummary (Socket, Ch, Dimm, Rank, NULL);
      } // Rank loop
    } // Dimm loop
  } // Ch loop
  //
  // Restore RxStrobeInv and adjust final RcvEnDelay if needed
  //
  ReceiveEnabledRxStrobeInvRestore (Host, Socket, PreviousStrobeInv);

  //
  // Notify IO to move out of RCVEN training mode
  //
  SetTrainingModeDdr5 (Socket, TmReceiveEnableExit);
  //
  // Set IO compensation delay to the final value
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    SetMprTrainModeDdr5 (Host, Socket, Ch, 0);
    ConvertRdToMrr (Host, Socket, Ch, 0);
    RtError = 0;

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      SetIOLatencyComp (Host, Socket, Ch, SubCh, RtError, ActToActSave);
    } //SubCh loop
  } // Ch loop

  //
  // Set SenseAmpDelay and ODT Delay and durations
  //
  SetSenseAmpODTDelaysDdr5 (Socket);

  // Exit Training Mode
  SetAepTrainingMode (Host, Socket, DISABLE_TRAINING_STEP);

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    NumRanks = Host->var.mem.socket[Socket].maxRankDimm;
    for (Rank = 0; Rank < NumRanks; Rank++) {
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }

        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);

        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }

        //
        // Revert to functional read preamble
        //
        if (IsDcpmmPresentDimm (Socket, Ch, Dimm)) {
          SetFMCReceiveEnableModeDdrt2 (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, FALSE);
        }
      } //Ch Loop
    } // Rank Loop
  } // Dimm Loop
  //
  // Program the Rx Analog En Early
  //
  Status = ProgramRxAnalogEnEarly (Socket);
  if (Status != MRC_STATUS_SUCCESS) {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Program rx_analog_en_early fails\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_211);
  }

  //
  // Reset DDR IO
  //
  IO_Reset (Host, Socket);
  FifoTrainReset (Host, Socket);

  RcFreePool (TrainRes);

  DisplayResultsDdr5 (Host, Socket, RecEnDelay);
  DisplayResultsDdr5 (Host, Socket, RxCoarseDelay);

  return SUCCESS;
} // ReceiveEnableDdr5


/**

  Find the edges of the signal

  @param Host              - Pointer to sysHost
  @param Socket              - Socket number
  @param Dimm              - DIMM number
  @param Rank              - Rank number
  @param RisingEdge        - Pointer to save the rising edge
  @param CenterPoint       - Pointer to save the center point
  @param FallingEdge       - Pointer to save the falling edge
  @param PulseWidth        - Pointer to save the pulse width

  @retval N/A

**/
STATIC
VOID
FindPiSettingDdr5 (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Dimm,
  UINT8     Rank,
  UINT16    RisingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  UINT16    CenterPoint[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  UINT16    FallingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  UINT16    PulseWidth[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  struct    TrainingResultsDdr5 (*TrainResInput) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8               Ch;
  UINT8               Strobe;
  UINT8               SubCh = 0;
  struct dimmNvram    (*DimmNvList) [MAX_DIMM];
  UINT8               FaultyPartsStatus;
  UINT8 SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  FaultyPartsStatus = FPT_NO_ERROR;

  //
  // Starting by sampling all the SDRAM present
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    if ((*DimmNvList) [Dimm].dimmPresent == 0) {
      Host->var.mem.piSettingStopFlag[Ch] = 0;
      continue;
    }
    SetDqCountingWindow (Host, Socket, Dimm, Rank);
    if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
      Host->var.mem.piSettingStopFlag[Ch] = 0;
      continue;
    }

    Host->var.mem.piSettingStopFlag[Ch] = GetStrobeMask (Host);
  } // Ch loop

  PISettingAdjustmentDdr5 (Host, Socket, Dimm, Rank, TrainResInput);

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    // Continue to the next Rank if this one is disabled
    if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
      continue;
    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
#ifdef DEBUG_CODE_BLOCK
      if (GetDebugLevel () & SDBG_MAX) {
        AcquirePrintControl ();
        RcDebugPrint (SDBG_DEFAULT, "\nSummary: Receive Enable Pi\nS%d, Ch%d, DIMM%d, SC%d Rank%d\n", Socket, Ch, Dimm, SubCh, Rank);
        PrintSampleArrayDdr5 (Host, Socket, Ch, SubCh, Dimm, 1, TrainResInput);
        ReleasePrintControl ();
      }
#endif // DEBUG_CODE_BLOCK
      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }

        EvaluatePiSettingResultsDdr5 (Socket, Ch, SubCh, Dimm, Rank, Strobe, RisingEdge, CenterPoint, FallingEdge, PulseWidth,
          FPT_PI_READ_TYPE, TrainResInput);

        if (Host->nvram.mem.socket[Socket].FaultyPartsFlag[Ch] & (1 << Strobe)) {
          //
          // Add this error to the warning log for both correctable and uncorrectable errors.
          //
          FaultyPartsStatus = EvaluateFaultyParts (Host, Socket, Ch);

          switch (FaultyPartsStatus) {
            case FPT_NO_ERROR:
              break;
            case FPT_CORRECTABLE_ERROR:
              // if correctable  log the warning -> OutputWarning
              // careful for cases when warning gets promoted to error !!!
              EwlOutputType2 (WARN_FPT_CORRECTABLE_ERROR, WARN_FPT_MINOR_RD_RCVEN, Socket, Ch, Dimm, Rank, EwlSeverityWarning, Strobe, NO_BIT, RecEnDelay, DdrLevel, (UINT8) PulseWidth[Ch][SubCh][Strobe]);
              break;
            default:
              RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
                "RecEn training failure!!!\n");
              //
              // Add this error to the warning log for both correctable and uncorrectable errors.
              //
              Host->var.mem.piSettingStopFlag[Ch] |= (1 << Strobe);
              DisableChannelSw (Host, Socket, Ch);
              EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_RD_RCVEN, Socket, Ch, Dimm, Rank, EwlSeverityWarning, Strobe, NO_BIT, RecEnDelay, DdrLevel, (UINT8) PulseWidth[Ch][SubCh][Strobe]);
              break;
          }
        }
      } // Strobe loop

#ifdef DEBUG_CODE_BLOCK
    DisplayEdgesReceiveEnableDdr5 (Host, Socket, Ch, PulseWidth[Ch][SubCh], FallingEdge[Ch][SubCh], RisingEdge[Ch][SubCh], CenterPoint[Ch][SubCh], SubChMSVx4);
#endif // DEBUG_CODE_BLOCK
    } //subchannel loop
  } // Ch loop

  return;
} // FindPiSettingDdr5

/**

  This function change 128 times the pi setting for each Rank channel.

  @param Host              - Pointer to sysHost
  @param Socket              - Socket number
  @param Dimm              - DIMM number
  @param Rank              - Rank number

  @retval N/A

**/
STATIC
VOID
PISettingAdjustmentDdr5 (
  PSYSHOST                    Host,
  UINT8                       Socket,
  UINT8                       Dimm,
  UINT8                       Rank,
  struct TrainingResultsDdr5  (*TrainResInput) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8                       Ch;
  UINT8                       ChannelLocal;
  UINT8                       SubCh;
  UINT32                      ChBitmask;
  UINT16                      FineDelay;
  UINT16                      FineDelayEnd;
  struct dimmNvram            (*DimmNvList) [MAX_DIMM];
  struct TrainingResultsDdr5  (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  MRC_RT                      MrcRankList;
  MRC_MST                     MemSsType;
  UINT8                       MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  MrcRankList.Dimm = Dimm;
  MrcRankList.Rank = Rank;

  TrainRes = TrainResInput;

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    // Continue to the next Rank if this one is disabled
    if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
      continue;
    }
  } // Ch loop

  ChBitmask = GetChBitmask (Host, Socket, Dimm, Rank);
  //
  // DDR5_TODO. To update the column address to MR31. To set the bit to convert the read to mrr in mc.
  //
  SetupRecEnFineTestDdr5 (Host, Socket, ChBitmask, Dimm, Rank);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "RecEn Pi Scanning:\n");
  //
  // Set the count window
  //
  SetDataTrainCountWindow (Host, Socket, Dimm, Rank, REC_EN_COUNT_WINDOW);

  FineDelayEnd = REC_EN_PI_START + REC_EN_PI_RANGE;

  for (FineDelay = REC_EN_PI_START; FineDelay < FineDelayEnd; FineDelay += REC_EN_STEP_SIZE) {
    //
    // Change the Receive Enable PI Setting
    //
    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }

      DimmNvList = GetDimmNvList (Host, Socket, Ch);

      // Continue to the next Rank if this one is disabled
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
        continue;
      }
      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        ChannelLocal = Ch;
        GetChannelMapping (Ch, SubCh, &ChannelLocal);
        GetSetDataGroup (Host, Socket, ChannelLocal, NO_SUBCH, Dimm, Rank, ALL_STROBES, 0, DdrLevel, RecEnDelay, GSM_UPDATE_CACHE,
          (INT16 *)&FineDelay);
      } //SubCh
    } // Ch loop

    //
    // Send a burst of 7 read commands back to back (4 DCLK apart) using MC_SPECIAL_COMMAND CRs
    //

    if (Host->DdrioUltSupport) {

      CteSetOptions ("io_reset", ChBitmask, 0x1, 0x0, 1);
      CteDelayDclk (8);
      CteSetOptions ("io_reset", ChBitmask, 0x1, 0x0, 0);
      CteStartPattern (ChBitmask, 0x3, GetPhyRank (Dimm, Rank));
      CteWaitPatternDone ();

    } else {
      TestControl (Host, Socket, MemSsType, ChBitmask, NO_SUBCH, RcvenFineTest, 0, START_TEST, NOT_STOP_TEST);
    }


    //
    // Delay if this is a CTE build, otherwise do nothing.
    //

    CteDelayDclk (100); // To prevent IO_RESET in the middle of the data burst corrupting previous sequence.

    GetResultsDdr5 (Host, Socket, Dimm, Rank, FineDelay, TrainRes);

  } // FineDelay loop

  return;
} // PISettingAdjustmentDdr5

/**

 For each data module, a 128 bit vector of zeros and ones holding the tests results is
 evaluated which is a concatenation of the two reads from the feedback CRs.
 BIOS locates the middle of the longest consecutive stream of ones (when the vector is evaluated as cyclic,
 this location in the vector indicates the PI setting and logic delay for that data module).

  @param[in]  Socket        - Current Socket
  @param[in]  Ch            - Channel number
  @param[in]  Subch         - Sub channel number
  @param[in]  Dimm          - Dimm number on Ch
  @param[in]  Rank          - Rank on Dimm
  @param[in]  Strobe        - Strobe number
  @param[in]  RisingEdge    - Pointer to save the rising edge
  @param[in]  CenterPoint   - Pointer to save the center point
  @param[in]  FallingEdge   - Pointer to save the falling edge
  @param[in]  PulseWidth    - Pointer to save the pulse width
  @param[in]  Step          - The PI Step to work on:
                                FPT_PI_READ_TYPE FPT_PI_WRITE_TYPE
                                FPT_PI_LRDIMM_READ_TYPE
                                FPT_PI_LRDIMM_WRITE_TYPE
                                FPT_PI_LRDIMM_RD_MRD_TYPE
                                FPT_PI_LRDIMM_WR_MRD_TYPE
  @param[out] TrainResInput - Training result

  @retval N/A

**/
VOID
EFIAPI
EvaluatePiSettingResultsDdr5 (
  IN UINT8                       Socket,
  IN UINT8                       Ch,
  IN UINT8                       Subch,
  IN UINT8                       Dimm,
  IN UINT8                       Rank,
  IN UINT8                       Strobe,
  IN UINT16                      RisingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN UINT16                      CenterPoint[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN UINT16                      FallingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN UINT16                      PulseWidth[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN UINT8                       Step,
  OUT struct TrainingResultsDdr5 (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT16                     PiSetting;
  UINT16                     PiStart;
  UINT32                     Register;
  UINT32                     Bit;
  UINT8                      CyclicCase;
  UINT16                     OneCount;
  UINT16                     OneStart;
  UINT16                     NumberOfCycle;
  UINT16                     MaxOneCount;
  UINT8                      DWord;
  struct ddrRank             (*RankList) [MAX_RANK_DIMM];
  struct dimmNvram           (*DimmNvList) [MAX_DIMM];
  PSYSHOST                   Host;

  Host = GetSysHostPointer ();
  Register = 0;
  OneStart = 0;

  Bit = BIT0;
  OneCount = 0;
  MaxOneCount = 0;
  CyclicCase = 0;
  //TrainRes = &Host->var.mem.TrainResDdr5;

  RisingEdge[Ch][Subch][Strobe] = 0;
  FallingEdge[Ch][Subch][Strobe] = 0;
  PulseWidth[Ch][Subch][Strobe] = 0;

  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  RankList = GetRankNvList (Host, Socket, Ch, Dimm);

  //
  // Start at bit 0
  //
  SetupPiSettingsParamsDdr5 (Socket, Step, &PiSetting, &PiStart, &NumberOfCycle);
  Host->var.mem.piSettingStopFlag[Ch] = GetStrobeMask(Host); //to make sure all the Strobe will be handled.
  while (Host->var.mem.piSettingStopFlag[Ch] & (1 << Strobe)) {

    DWord = GetTrainingResultIndex (Step, PiSetting, PiStart);

    //
    // Get the proper training results
    //
    if ((PiSetting % 32) == 0) {
      Register = (*TrainRes) [Ch][Subch][Strobe].results[DWord];
      Bit = BIT0;
    }
    //
    // algo start
    //
    if (Register & Bit) {
      //
      // this index is 1
      //
      if (OneCount == 0) {
        //
        // This first one so save the starting point
        //
        OneStart = PiSetting;
      }

      OneCount++;
    } else {
      //
      // this index is 0
      //
      if (OneCount > 0) {
        //
        // We just moved from a one to a zero so check if that was the largest stream of one's
        //
        if (MaxOneCount < OneCount) {
          //
          // TBD  if i have two max value equals
          //
          MaxOneCount = OneCount;
          RisingEdge[Ch][Subch][Strobe] = OneStart;
          FallingEdge[Ch][Subch][Strobe] = PiSetting;
        }
        //
        // Start counting again at 0
        //
        OneCount = 0;
      }
    }

    //
    // Go to the next bit
    //
    PiSetting++;
    Bit = Bit << 1;

    //
    // Check if we're at the end of the cycle
    //
    if (PiSetting == NumberOfCycle) {
      if (OneCount > 0) {
        //
        // Cyclic case continue to scan
        // Start back over with bit 0 if the last bit is a 1
        //
        PiSetting = StartBackOver (Step, PiStart);
        CyclicCase = 1;
      } else {
        //
        // stop the scan
        // We've checked all bits so save the results
        //
        Host->var.mem.piSettingStopFlag[Ch] &= ~(1 << Strobe);
        PulseWidth[Ch][Subch][Strobe] = FallingEdge[Ch][Subch][Strobe] - RisingEdge[Ch][Subch][Strobe];
        CenterPoint[Ch][Subch][Strobe] = ((PulseWidth[Ch][Subch][Strobe] / 2) + RisingEdge[Ch][Subch][Strobe]) % (NumberOfCycle - PiStart);
      }
    }
    //
    // Check if this is a cyclic case and if we are at the end of the one's
    //
    if ((CyclicCase == 1) && (OneCount == 0)) {
      Host->var.mem.piSettingStopFlag[Ch] &= ~(1 << Strobe);
      if (RisingEdge[Ch][Subch][Strobe] > FallingEdge[Ch][Subch][Strobe]) {
        PulseWidth[Ch][Subch][Strobe] = ((NumberOfCycle - RisingEdge[Ch][Subch][Strobe]) + FallingEdge[Ch][Subch][Strobe]) - PiStart;
        CenterPoint[Ch][Subch][Strobe] = (((PulseWidth[Ch][Subch][Strobe] / 2) + RisingEdge[Ch][Subch][Strobe])) % (NumberOfCycle - PiStart);
      } else {
        //
        // last bit is 1 and first bit after cycle is 0 bit 127 = 1 and bit 0 = 0
        //
        PulseWidth[Ch][Subch][Strobe] = FallingEdge[Ch][Subch][Strobe] - RisingEdge[Ch][Subch][Strobe];
        CenterPoint[Ch][Subch][Strobe] = ((PulseWidth[Ch][Subch][Strobe] / 2) + RisingEdge[Ch][Subch][Strobe]) % (NumberOfCycle - PiStart);
      }
    }
  } // while loop

  if (FAULTY_PARTS_TRACKING_INJECT_ERROR == 1) {
    if ((Ch == 0) && (Dimm == 0) && (Rank == 1) && (Strobe == 5)) {
      //_asm jmp $;
      PulseWidth[Ch][Subch][Strobe] = 1;
      CenterPoint[Ch][Subch][Strobe] = ((PulseWidth[Ch][Subch][Strobe] / 2) + RisingEdge[Ch][Subch][Strobe]) % (NumberOfCycle - PiStart);
    }
  } // FAULTY_PARTS_TRACKING_INJECT_ERROR

  //
  // If the PulseWidth (ones stream) is smaller than threshold need to sample again
  //
  if ((Step == FPT_PI_READ_TYPE) || (Step == FPT_PI_WRITE_TYPE)) {
    if (PulseWidth[Ch][Subch][Strobe] <= RE_EDGE_SIZE_THRESHOLD) {

      //
      // This is a fatal error condition so map out the entire channel
      //
      Host->nvram.mem.socket[Socket].FaultyPartsFlag[Ch] |= (1 << Strobe);
      if (Step == FPT_PI_READ_TYPE) {
        (*RankList) [Rank].faultyParts[Strobe] |= FPT_REC_ENABLE_FAILED;

        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
          "Failed RecEn Pi\n");
      }
      if (Step == FPT_PI_WRITE_TYPE) {
        (*RankList) [Rank].faultyParts[Strobe] |= FPT_WR_LEVELING_FAILED;
        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
          "Failed WL Pi\n");
        // Evaluate here for correctable error  ... possibly disable channel
      }
    }
  }

#ifdef LRDIMM_SUPPORT
  //
  // HSD4929028: Error checking for LRDIMM
  //
  if ((Step == FPT_PI_LRDIMM_READ_TYPE) ||
      (Step == FPT_PI_LRDIMM_WRITE_TYPE)) {
    if (PulseWidth[Ch][Subch][Strobe] <= RE_LRDIMM_EDGE_SMALL_THRESHOLD_PHASE ||
        PulseWidth[Ch][Subch][Strobe] >= RE_LRDIMM_EDGE_LARGE_THRESHOLD_PHASE) {
      Host->nvram.mem.socket[Socket].FaultyPartsFlag[Ch] |= (1 << Strobe);
      (*RankList) [Rank].faultyParts[Strobe] |= FPT_LRDIMM_TRAINING_FAILED;
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
        "Failed LRDIMM Pi\n");
    }
  }
  if ((Step == FPT_PI_LRDIMM_RD_MRD_TYPE) ||
      (Step == FPT_PI_LRDIMM_WR_MRD_TYPE)) {
    if (PulseWidth[Ch][Subch][Strobe] <= RE_LRDIMM_EDGE_SIZE_THRESHOLD) {
      Host->nvram.mem.socket[Socket].FaultyPartsFlag[Ch] |= (1 << Strobe);
      (*RankList) [Rank].faultyParts[Strobe] |= FPT_LRDIMM_TRAINING_FAILED;
      RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
        "Failed LRDIMM Pi\n");
    }
  }
#endif //Lrdimm_support
}

/**

  Once the DQS high phase has been found (for each DRAM) the next stage is to find out the round trip latency,
  by locating the preamble cycle. This is achieved by trying smaller and smaller roundtrip
  values until the Strobe sampling is done on the preamble cycle.

  @param Host              - Pointer to sysHost
  @param Socket              - Socket number
  @param Dimm              - DIMM number
  @param Rank              - Rank number
  @param CenterPoint       - Pointer to center point

  @retval N/A

**/
STATIC
VOID
FindRoundTripLatencyDdr5 (
  PSYSHOST                            Host,
  UINT8                               Socket,
  UINT8                               Dimm,
  UINT8                               Rank,
  UINT16                              CenterPoint[MAX_CH][SUB_CH][MAX_STROBE_DDR5/2]
  )
{
  UINT8               Ch;
  UINT32              ChBitmask;
  UINT8               SubChBitmask[MAX_CH];
  UINT8               Strobe;
  UINT8               LogRank = 0;
  UINT8               StopFlag;
  UINT8               HaveZero;
  UINT8               RoundTrip;
  UINT8               IoLatency;
  UINT16              TmpPiDelay;
  UINT32              FeedBack = 0;
  UINT32              ZeroFlag[SUB_CH];
  UINT32              AllZeros[MAX_CH][SUB_CH];
  UINT32              ShiftIndex;
  UINT32              Register;
  UINT8               SubCh = 0;
  UINT8               SubChMSVx4;
  struct channelNvram (*ChannelNvList) [MAX_CH];
  struct dimmNvram    (*DimmNvList) [MAX_DIMM];
  struct ddrRank      (*RankList) [MAX_RANK_DIMM];
  struct RankCh       (*RankPerCh) [MAX_RANK_CH];
  UINT8               Aux_strobe;
  UINT16              MaxLimit = 0;
  UINT16              MinLimit = 0;
  UINT16              UsDelay = 0;
  UINT8               ChannelLocal;
  UINT8               MaxChDdr;
  UINT8               RtLoopNum = 0;
  UINT8               RoundTripStepUnit;
  UINT8               IoLatStepUnit;
  MRC_MST             MemSsType;

  RcDebugPrint (SDBG_MEM_TRAIN, "FindRoundTripLatencyDdr5 starts\n");

  SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  MaxChDdr = GetMaxChDdr ();
  RoundTripStepUnit = GetRoundTripStepUnit ();
  IoLatStepUnit = GetIoLatStepUnit ();
  MemSsType = (GetSysCpuCsrAccessVar ())->MemSsType[Socket];
  Register = 0;

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      AllZeros[Ch][SubCh] = 0;
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }

      DimmNvList = GetDimmNvList (Host, Socket, Ch);

      // Continue to the next Rank if this one is disabled
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
        continue;
      }
      AllZeros[Ch][SubCh] = GetSubChStrobeMaskDdr5 (Host);
    } //subchannel loop.
  } // Ch loop

  ChBitmask = GetChBitmask (Host, Socket, Dimm, Rank);
  SetMem (SubChBitmask, sizeof (SubChBitmask), TWO_SUBCH_MASK);
  GetDataGroupLimits(Host, DdrLevel, RecEnDelay, &MinLimit, &MaxLimit, &UsDelay); // move this out of loops

  StopFlag = 0;

  while (StopFlag == 0) {

    //
    // Send a burst of 16 read commands back to back (4 DCLK apart)
    //
    if (Host->DdrioUltSupport) {

      CteSetOptions ("io_reset", ChBitmask, 0x1, 0x0, 1);
      CteDelayDclk (8);
      CteSetOptions ("io_reset", ChBitmask, 0x1, 0x0, 0);
      CteStartPattern (ChBitmask, 0x3, GetPhyRank (Dimm, Rank));
      CteWaitPatternDone ();

    } else {
      CteDelayDclk (300);
      TestControl (Host, Socket, MemSsType, ChBitmask, NO_SUBCH, RcvenFineTest, 0, START_TEST, NOT_STOP_TEST);
    }
    if (FeaturePcdGet (PcdCosimBuild) == TRUE) {
      CteDelayDclk (100); // To prevent IO_RESET in the middle of the data burst corrupting previous sequence
    }

    StopFlag = 1;
    for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

      HaveZero = 0;

      ZeroMem ((UINT8 *) ZeroFlag, sizeof (ZeroFlag));

      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        ChannelLocal = Ch;
        GetChannelMapping (Ch, SubCh, &ChannelLocal);
        if ((ChBitmask & (1 << ChannelLocal)) == 0) {
          continue;
        }
        if (!IsBrsPresent (Host, Socket) &&
            (SubChBitmask[Ch] & (1 << SubCh)) == 0) {
          continue;
        }

        //
        // Continue if this Rank is not present
        //
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        DimmNvList = GetDimmNvList (Host, Socket, Ch);

        // Continue to the next Rank if this one is disabled
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }

        RankPerCh = &(*ChannelNvList) [Ch].rankPerCh;
        DimmNvList = GetDimmNvList (Host, Socket, Ch);


        //
        // BIOS reads result from IO CRs and extract the relevant bit for each data module.
        //
        for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
          //
          // Skip if this is an ECC Strobe when ECC is disabled
          //
          if (IsStrobeNotValidDdr5 (Host, Strobe)) {
            continue;
          }

          ShiftIndex = CenterPoint[Ch][SubCh][Strobe] % RE_TRAINING_RESULT_HIGH_IN_BITS;

          //
          // Read the correct register to work on
          //
          FeedBack = ReadDataTrainCounterFeedbackDdr5 (Host, Socket, Ch, SubCh, Strobe);
          if (FeedBack >= 1) {
            Register = 1 << ShiftIndex;
          } else {
            Register = 0;
          }
          if ((*RankList) [Rank].faultyParts[Strobe] != 0) {
            RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, Strobe, NO_BIT,
              "Skip RecEn HaveZero & ZeroFlag\n");
            // do nothing here we will fool the system that the faulty part is working later when we see other strobes to be working too
          } else {
            //
            // Test if DQS value is 0 or 1
            //
            if ((Register & (1 << ShiftIndex)) == 0) {
              //
              // SDRAM i index value is 0
              //
              HaveZero = 1;
              ZeroFlag[SubCh] |= 1 << Strobe;

              // if some other Strobe is ok lets append the faulty Strobe as working too - to advance the algorithm
              for (Aux_strobe = 0; Aux_strobe < SubChMSVx4; Aux_strobe++) {
                //
                // Skip if this is an ECC Strobe when ECC is disabled
                //
                if (IsStrobeNotValidDdr5 (Host, Aux_strobe)) {
                  continue;
                }
                if ((*RankList) [Rank].faultyParts[Aux_strobe] != 0) {
                  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, Aux_strobe, NO_BIT,
                    "Adding faulty Strobe RecEn\n");
                  ZeroFlag[SubCh] |= 1 << Aux_strobe;
                }
              } // Aux_strobe loop
            }
          }
        } // Strobe loop
      }//subchannel loop

      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

        ChannelLocal = Ch;
        GetChannelMapping (Ch, SubCh, &ChannelLocal);

        if ((ChBitmask & (1 << ChannelLocal)) == 0) {
          continue;
        }

        if (!IsBrsPresent (Host, Socket) &&
            (SubChBitmask[Ch] & (1 << SubCh)) == 0) {
          continue;
        }
        //
        // Get the logical Rank #
        //
        LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);

        if (HaveZero) {
          if (ZeroFlag[SubCh] != AllZeros[Ch][SubCh]) {
            StopFlag = 0;

            //
            // increase the Rank # IO latency by IoLatStepUnit and continue to next iteration.
            //
            GetIoLatencyDdr5 (Socket, Ch, SubCh, LogRank, GSM_READ_ONLY, &IoLatency);
            IoLatency = IoLatency + IoLatStepUnit;
            SetIoLatencyDdr5 (Socket, Ch, SubCh, LogRank, IoLatency);
            RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT,
              "IO Latency = %d\nzeroFlag = 0x%x, AllZeros[%d][%d] = 0x%x\n",
              IoLatency, ZeroFlag[SubCh], Ch, SubCh, AllZeros[Ch][SubCh]);

            //
            //  If some are zero, on next iteration increase their RX RcvEnable Logic Delay settings by 2 QCLK
            //
            for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
              //
              // Skip if this is an ECC Strobe when ECC is disabled
              //
              if (IsStrobeNotValidDdr5 (Host, Strobe)) {
                continue;
              }

              if (ZeroFlag[SubCh] & (1 << Strobe)) {
                //
                // Increase logic delay by 2QCLK
                //
                GetSetDataGroup (Host, Socket, ChannelLocal, SubCh, Dimm, Rank, Strobe, 0, DdrLevel, RecEnDelay, GSM_READ_ONLY,
                  (INT16 *)&TmpPiDelay);
                TmpPiDelay = TmpPiDelay + (2 * 64);

                //
                // TODO: Check against a max pi delay instead of of max logic delay
                //
                if (TmpPiDelay > MaxLimit) {
                  TmpPiDelay = MaxLimit;
                  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
                    "ERROR: Rec En Logic Delay is %d > %d *64!!!\n", TmpPiDelay, RE_MAX_LOGIC_DELAY);
                  EwlOutputType2 (WARN_RD_RCVEN, WARN_RCVEN_PI_DELAY_EXCEEDED, Socket, Ch, Dimm, Rank, EwlSeverityWarning, Strobe, NO_BIT, RecEnDelay, DdrLevel, 0xFF);
                  DisableChannelSw (Host, Socket, Ch);
                }

                CenterPoint[Ch][SubCh][Strobe] = TmpPiDelay;
                //DDR5_TODO. SPR_TODO.
                GetSetDataGroup (Host, Socket, ChannelLocal, SubCh, Dimm, Rank, Strobe, 0, DdrLevel, RecEnDelay, GSM_UPDATE_CACHE,
                  (INT16 *)&TmpPiDelay);
                RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, Strobe, NO_BIT,
                  "Rec En Delay = %d\n", TmpPiDelay);
              } // ZeroFlag
            } // Strobe loop
          } else {
            RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT,
              "Round trip latency: Found all zeros\n");
            if (IsBrsPresent (Host, Socket)) {
              ChBitmask &= ~(1 << ChannelLocal);    // don't test this channel anymore
            } else {
              SubChBitmask[Ch] &= ~(1 << SubCh);    // don't test this subchannel anymore
              if (SubChBitmask[Ch] == 0) {
                ChBitmask &= ~(1 << ChannelLocal);  // don't test this channel anymore
              }
            }
          }
        } else {
          //
          // If all bits are one, decrement roundtrip value by RoundTripStepUnit, and continue to next iteration.
          //
          StopFlag = 0;
          GetRoundTripDdr5 (Socket, Ch, SubCh, LogRank, GSM_READ_ONLY, &RoundTrip);
          if (RoundTrip < RoundTripStepUnit) {
            if (IsBrsPresent (Host, Socket)) {
              ChBitmask &= ~(1 << ChannelLocal);    // don't test this channel anymore
            } else {
              SubChBitmask[Ch] &= ~(1 << SubCh);    // don't test this subchannel anymore
              if (SubChBitmask[Ch] == 0) {
                ChBitmask &= ~(1 << ChannelLocal);  // don't test this channel anymore
              }
            }
            RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT,
              "WARNING: Unable to find round trip latency!!!\n");
            DisableChannelSw (Host, Socket, Ch);
            EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_RD_RCVEN, Socket, Ch, Dimm, Rank, EwlSeverityWarning, NO_STROBE, NO_BIT, RecEnDelay, DdrLevel, 0xFF);
          } else {
            RoundTrip -= RoundTripStepUnit;
          }
          SetRoundTripAll (Socket, Ch, SubCh, RoundTrip);
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT,
            "Round trip latency: No zeros found, change round trip latency to %d\n",
            RoundTrip);
        } //else
      }//subchannel loop
    } // Ch loop
    RtLoopNum++;
  } // while loop
  RcDebugPrint (SDBG_MAX, "FindRoundTripLatencyDdr5 ends\n");

  return;
} // FindRoundTripLatencyDdr5

#ifdef DEBUG_CODE_BLOCK
/**

  This function print the sample array

  @param Host      - Pointer to sysHost
  @param Socket      - Socket number
  @param Ch        - Channel number
  @param Dimm      - DIMM number
  @param ReadMode  - 5 = LRDIMM Write MWD Delay
                     4 = LRDIMM Read DQ DQS
                     3 = LRDIMM Read Recieve Enable Phase
                     2 = LRDIMM Write DWL
                     1 = Read DqDqs
                     0 = Write DqDqs

  @retval N/A

**/
VOID
PrintSampleArrayDdr5 (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Subch,
  UINT8     Dimm,
  UINT8     ReadMode,
  struct    TrainingResultsDdr5 (*TrainResInput) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8   Strobe;
  UINT8   MaxStrobe;
  UINT32  PiSetting;
  struct TrainingResultsDdr5 (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  struct dimmNvram (*DimmNvList) [MAX_DIMM];
  UINT8 SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8 SubChMSVx8 = GetSubChMaxStrobeValid (Host) /2;

  TrainRes = TrainResInput;
#ifdef LRDIMM_SUPPORT
#if 0
  if (ReadMode == PRINT_ARRAY_TYPE_LRDIMM_MDQ_WR_DELAY) {
    TrainRes = &Host->var.mem.lrMwdTrainRes;
  }
  if (ReadMode == PRINT_ARRAY_TYPE_LRDIMM_MDQ_RD_DELAY) {
    TrainRes = &Host->var.mem.lrMrdTrainRes;
  }
  if (ReadMode == PRINT_ARRAY_TYPE_LRDIMM_RCV_EN_PHASE) {
    TrainRes = &Host->var.mem.lrTrainRes;
  }
  if (ReadMode == PRINT_ARRAY_TYPE_LRDIMM_WL_PHASE) {
    TrainRes = &Host->var.mem.lrDwlTrainRes;
  }
#endif
#endif // LRDIMM_SUPPORT

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  if ((ReadMode == 0) && !(*DimmNvList) [Dimm].x4Present) {
    MaxStrobe = SubChMSVx8;
    RcDebugPrintLine (SDBG_DEFAULT, (MaxStrobe * 4) + 3, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  } else {
    MaxStrobe = SubChMSVx4;
    RcDebugPrintLine (SDBG_DEFAULT, (MaxStrobe * 4) + 13, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  }
  RcDebugPrint (SDBG_DEFAULT, "    ");
  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (!IsStrobeNotValidDdr5 (Host, Strobe)) {
      RcDebugPrint (SDBG_DEFAULT, "%4d", Strobe);
    }
  }
  RcDebugPrint (SDBG_DEFAULT, "\n");

  for (PiSetting = 0; PiSetting < (MAX_PHASE_IN_FINE_ADJUSTMENT * 2); PiSetting++) {
    if ((ReadMode > 1) && (PiSetting % 2)) {
      continue;
    }
    if (((ReadMode == PRINT_ARRAY_TYPE_LRDIMM_MDQ_RD_DELAY) || (ReadMode == PRINT_ARRAY_TYPE_LRDIMM_MDQ_WR_DELAY)) && (PiSetting % 4)) {
      continue;
    }
    if ((ReadMode == PRINT_ARRAY_TYPE_LRDIMM_MDQ_RD_DELAY) || (ReadMode == PRINT_ARRAY_TYPE_LRDIMM_MDQ_WR_DELAY)) {
      RcDebugPrint (SDBG_DEFAULT, "%d ", (((INT32) PiSetting / 4) - 15));

      if ((((INT32) PiSetting / 4 - 15) >= 0) && (((INT32) PiSetting / 4 - 15) < 10)) {
        RcDebugPrint (SDBG_DEFAULT, "  ");
      }

      if ((((INT32) PiSetting / 4 - 15) < 100) && (((INT32) PiSetting / 4 - 15) > 9)) {
        RcDebugPrint (SDBG_DEFAULT, " ");
      }

      if ((((INT32) PiSetting / 4 - 15) < 0) && (((INT32) PiSetting / 4 - 15) > -10)) {
        RcDebugPrint (SDBG_DEFAULT, " ");
      }

    } else {
      RcDebugPrint (SDBG_DEFAULT, "%d ", PiSetting);

      if (PiSetting < 10) {
        RcDebugPrint (SDBG_DEFAULT, "  ");
      }

      if ((PiSetting < 100) && (PiSetting > 9)) {
        RcDebugPrint (SDBG_DEFAULT, " ");
      }
    }
    for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
      if (IsStrobeNotValidDdr5 (Host, Strobe)) {
        continue;
      }
      if (((1 << PiSetting) & (*TrainRes) [Ch][Subch][Strobe].results[PiSetting / 32]) != 0) {
        RcDebugPrint (SDBG_DEFAULT, "   1");
      } else {
        RcDebugPrint (SDBG_DEFAULT, "   0");
      }
    } // Strobe loop

    RcDebugPrint (SDBG_DEFAULT, "\n");
  } // PiSetting loop
} // PrintSampleArrayDdr5


#endif // DEBUG_CODE_BLOCK


/**

  Get's the resutls from the current test

  @param Host      - Pointer to sysHost
  @param Socket    - Current Socket
  @param Dimm      - DIMM number
  @param Rank      - Rank number of the DIMM
  @param PiDelay   - CLK adjustment setting

  @retval N/A

**/
VOID
GetResultsDdr5 (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Dimm,
  UINT8     Rank,
  UINT16    PiDelay,
  struct    TrainingResultsDdr5 (*TrainResInput) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8               Ch;
  UINT8               Strobe;
  UINT8               SubCh = 0;
  UINT8               SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT32              CsrReg;
  struct dimmNvram (*DimmNvList) [MAX_DIMM];
  struct TrainingResultsDdr5  (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  UINT8  MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  TrainRes = TrainResInput;

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      DimmNvList = GetDimmNvList (Host, Socket, Ch);

      // Continue to the next Rank if this one is disabled
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
        continue;
      }

      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        //
        // Skip if this is an ECC Strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }
        CsrReg = ReadDataTrainCounterFeedbackDdr5 (Host, Socket, Ch, SubCh, Strobe);

        RecEnTrainFeedbackDdr5 (Host, Socket, Ch, SubCh, Strobe, CsrReg, PiDelay, TrainRes);
      } // Strobe loop
    } //subchannel loop
  } // Ch loop
} // GetResultsDdr5

