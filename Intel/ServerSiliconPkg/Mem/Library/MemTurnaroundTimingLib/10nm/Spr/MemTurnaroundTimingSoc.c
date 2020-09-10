/** @file
    MemTurnaroundTimingSoc.c - contains the Memory turnaround timing functions for a
     specific processor.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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


#include <SysHost.h>
#include <MemHostChipCommon.h>
#include <Library/MemTurnaroundTimingLib.h>
#include "Include/MemTurnaroundTimingLibInternal.h"
#include <Library/MemoryCoreLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/RcDebugLib.h>
#include <Library/MemRcLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Memory/JedecDefinitions.h>

// Note that this Loop Latency value represents the CAS to CAS turnaround delay
//  through the iMC.  The value is already considered by the CAS to CAS iMC CSRs
//  such that if a value of 2 cycles of delay is desired, the CSR bit field would
//  need to be set to 0 (2 - LOOP_LATENCY).  This value is common for 10nm DDR4
//  designs but is different for 14nm designs.
#define LOOP_LATENCY 2

//
// Relaxed turnaround constants
//
//                                                  DdrFreq         t_rwsg  t_rwsr  t_rrdr  t_wwdr  t_rwdr  t_wrdr  t_rrdd  t_wwdd  t_rwdd  t_wrdd
CONST TURNAROUND_CONSTANTS  mRelaxedTurnaround = {DDR_FREQ_ANY,       19,     19,      7,      8,     19,      9,      7,      8,     19,      9};

//
// Relaxed DCPMM turnaround constants
//                                                            DdrFreq       RdRdDD  WrRdDD  RdRdS  WrRdS  RdWrDD  RdWrS  WrWrDD  WrWrS  GntRdDD  GntRdS  GntWrDD  GntWrS  RdGntDD  RdGntS  WrGntDD  WrGntS  GntGntDD  GntGntS
CONST DCPMM_TURNAROUND_CONSTANTS  mRelaxedDcpmmTurnaround = {DDR_FREQ_ANY,    6,      6,      6,     6,     6,     31,     10,     1,     15,      15,     15,      15,      0,      31,      7,      7,        7,      2};

//
// Electrical turnaround constants
//                                                      DdrFreq   t_rwsg  t_rwsr  t_rrdr  t_wwdr  t_rwdr  t_wrdr  t_rrdd  t_wwdd  t_rwdd  t_wrdd
CONST TURNAROUND_CONSTANTS  mTurnaroundDdr5Rdimm[] =  {{DDR_3200,   13,     13,      4,      5,     14,      5,      4,      5,     14,      5},
                                                       {DDR_3600,   14,     14,      4,      5,     15,      5,      4,      5,     15,      5},
                                                       {DDR_4000,   15,     15,      4,      5,     16,      5,      4,      5,     16,      5},
                                                       {DDR_4400,   16,     16,      4,      5,     17,      5,      4,      5,     17,      5},
                                                       {DDR_4800,   17,     17,      4,      5,     18,      5,      4,      5,     18,      5},
                                                       {DDR_5200,   18,     18,      4,      5,     19,      5,      4,      5,     19,      5},
                                                       {DDR_5600,   19,     19,      4,      5,     20,      5,      4,      5,     20,      5}
                                                      };

//                                                      DdrFreq   t_rwsg  t_rwsr  t_rrdr  t_wwdr  t_rwdr  t_wrdr  t_rrdd  t_wwdd  t_rwdd  t_wrdd
CONST TURNAROUND_CONSTANTS  mTurnaroundDdr4Rdimm[] =  {{DDR_1600,    5,      5,      2,      3,      6,      3,      2,      3,      6,      3},
                                                       {DDR_1866,    6,      6,      2,      3,      7,      3,      2,      3,      7,      3},
                                                       {DDR_2133,    7,      7,      2,      3,      8,      3,      2,      3,      8,      3},
                                                       {DDR_2400,    8,      8,      2,      3,      9,      3,      2,      3,      9,      3},
                                                       {DDR_2666,    9,      9,      2,      3,     10,      3,      2,      3,     10,      3},
                                                       {DDR_2933,   10,     10,      2,      3,     11,      3,      2,      3,     11,      3},
                                                       {DDR_3200,   11,     11,      2,      3,     12,      3,      2,      3,     12,      3}
                                                      };

#ifdef LRDIMM_SUPPORT
//                                                          DdrFreq   t_rwsg  t_rwsr  t_rrdr  t_wwdr  t_rwdr  t_wrdr  t_rrdd  t_wwdd  t_rwdd  t_wrdd
CONST TURNAROUND_CONSTANTS  mTurnaroundDdr4Lrdimm[] = {{DDR_1600,    9,      9,      3,      3,     10,      4,      2,      3,     10,      3},
                                                       {DDR_1866,   10,     10,      3,      3,     11,      4,      2,      3,     11,      3},
                                                       {DDR_2133,   11,     11,      3,      3,     12,      4,      2,      3,     12,      3},
                                                       {DDR_2400,   12,     12,      3,      3,     13,      4,      2,      3,     13,      3},
                                                       {DDR_2666,   13,     13,      3,      3,     14,      4,      2,      3,     14,      3},
                                                       {DDR_2933,   14,     14,      3,      3,     15,      4,      2,      3,     15,      3},
                                                       {DDR_3200,   15,     15,      3,      3,     16,      4,      2,      3,     16,      3}
                                                      };
#endif // LRDIMM_SUPPORT

//
// DCPMM RD                           DCPMM WR                            DCPMM GNT2RW
// Name      2400 2667  2933  3200    Name      2400 2667  2933  3200     Name       2400 2667  2933  3200
// t_RDRD_S   0x0  0x0   0x0   0x0    t_RDWR_S   0x0  0x0   0x0   0x0     t_GNTRD_S   0x0  0x0   0x0   0x0
// t_RDRD_DD  0x0  0x0   0x0   0x0    t_RDWR_DD  0x13 0x13  0x13  0x13    t_GNTRD_DD  0x0  0x0   0x0   0x0
// t_WRRD_S   0xF  0xF   0xF   0xF    t_WRWR_S   0x1  0x1   0x1   0x1     t_GNTWR_S   0x14 0x14  0x14  0x14
// t_WRRD_DD  0x2  0x2   0x2   0x2    t_WRWR_DD  0x5  0x5   0x5   0x5     t_GNTWR_DD  0x11 0x11  0x11  0x11
//
//  NOTE: SPR doesn't currently have t_RDGNT_DD defined - leaving value at 0
// DCPMM RW2GNT                        DCPMM GNT2GNT
// Name       2400 2667  2933  3200    Name        2400 2667  2933  3200
// t_RDGNT_S   0x0  0x0   0x0   0x0    t_GNTGNT_S   0x2  0x2   0x2   0x2
//                                     t_GNTGNT_DD  0x6  0x6   0x6   0x6
// t_WRGNT_S   0x0  0x0   0x0   0x0
// t_WRGNT_DD  0x1  0x1   0x1   0x1
//                                                     DdrFreq       RdRdDD  WrRdDD  RdRdS  WrRdS  RdWrDD  RdWrS  WrWrDD  WrWrS  GntRdDD  GntRdS  GntWrDD  GntWrS  RdGntDD  RdGntS  WrGntDD  WrGntS  GntGntDD  GntGntS
CONST DCPMM_TURNAROUND_CONSTANTS  mDcpmmTurnarounds = {DDR_FREQ_ANY,   0,      2,      0,     15,     19,     0,     5,      1,      0,      0,      17,      20,      0,      0,       1,      8,        6,      2};


/**

  Returns the loop latency for this SOC.  Loop Latency represents the CAS to CAS
  turnaround delay through the iMC.

  @param None

  @retval the loop latency for this SOC

**/
UINT8
EFIAPI
GetLoopLatency (
  VOID
  )
{
  return LOOP_LATENCY;
}

/**

  Return pointer to table of relaxed turnaorund constants

  @param[in]  Socket             - Socket number
  @param[out] TurnAroundTable    - Pointer to caller's table pointer to be
                                   populated by this function.

  @retval  EFI_SUCCESS

**/
EFI_STATUS
GetRelaxedTurnaroundConstants (
  IN UINT8                            Socket,
  OUT CONST TURNAROUND_CONSTANTS      **TurnaroundTable
  )
{
  *TurnaroundTable = &mRelaxedTurnaround;
  return EFI_SUCCESS;
}

/**

  Returns a pointer to a table of relaxed DCPMM turnaorund constants

  @param[out] TurnaroundTable    - Pointer to caller's table pointer to be
                                   populated by this function.

  @retval  EFI_SUCCESS

**/
EFI_STATUS
GetDcpmmRelaxedTurnaroundConstants (
  OUT CONST DCPMM_TURNAROUND_CONSTANTS  **DcpmmTurnaroundConstants
  )
{
  *DcpmmTurnaroundConstants = &mRelaxedDcpmmTurnaround;
  return EFI_SUCCESS;
}

/**

  Find and return a pointer to the turnaround contants structures
  for the current DDR frequency.

  @param[in]  Host                  - Pointer to SysHost
  @param[in]  Socket                - Socket number
  @param[in]  Channel               - Channel number
  @param[out] TurnaroundConstants   - On return, points to a pointer to the turnaround constants

  @retval EFI_SUCCESS if table is found
          !EFI_SUCCESS otherwise

**/
EFI_STATUS
GetTurnaroundConstants (
  IN  PSYSHOST                        Host,
  IN  UINT8                           Socket,
  IN  UINT8                           Channel,
  OUT CONST TURNAROUND_CONSTANTS      **TurnaroundConstants
  )
{
  EFI_STATUS                      Status = EFI_UNSUPPORTED;
  UINT8                           TableIndex;
  UINT8                           DdrFrequency;
  UINT16                          TableLength;
  struct channelNvram             (*ChannelNvList)[MAX_CH];
  CONST TURNAROUND_CONSTANTS      *TurnaroundTable;

  ChannelNvList = GetChannelNvList (Host, Socket);
  //
  // The function currently supports a table for LRDIMM and another for all others
  //
  if (IsDdr5MemSsPresent (Host, Socket)) {
    TurnaroundTable = mTurnaroundDdr5Rdimm;
    TableLength = sizeof (mTurnaroundDdr5Rdimm) / sizeof (mTurnaroundDdr5Rdimm[0]);
  } else {
    TurnaroundTable = mTurnaroundDdr4Rdimm;
    TableLength = sizeof (mTurnaroundDdr4Rdimm) / sizeof (mTurnaroundDdr4Rdimm[0]);
#ifdef LRDIMM_SUPPORT
    if ((*ChannelNvList)[Channel].lrDimmPresent) {
      TurnaroundTable = mTurnaroundDdr4Lrdimm;
      TableLength = sizeof (mTurnaroundDdr4Lrdimm) / sizeof (mTurnaroundDdr4Lrdimm[0]);
    }
#endif // LRDIMM_SUPPORT
  }

  DdrFrequency = Host->nvram.mem.socket[Socket].ddrFreq;

  //
  // Find the table entry based on frequency
  //
  for (TableIndex = 0; TableIndex < TableLength; TableIndex++) {
    if (TurnaroundTable[TableIndex].Frequency == DdrFrequency) {
      *TurnaroundConstants = &(TurnaroundTable[TableIndex]);
      Status = EFI_SUCCESS;
      break;
    }
  }
  return Status;
}

/**

  Find and return a pointer to the DCPMM turnaround contants structure.

  @param[out] DcpmmTurnaroundConstants  - On return, points to a pointer to the turnaround constants

  @retval EFI_SUCCESS if table is found
          !EFI_SUCCESS otherwise

**/
EFI_STATUS
GetDcpmmTurnaroundConstants (
  OUT CONST DCPMM_TURNAROUND_CONSTANTS  **DcpmmTurnaroundConstants
  )
{
  *DcpmmTurnaroundConstants = &mDcpmmTurnarounds;
  return EFI_SUCCESS;
}

/**

  Displays the read/write to grant turnaround times for this SOC.

  @param[in]  DcpmmTurnaroundSettings  - Table which contains DCPMM turnaround settings

  @retval none

**/
VOID
DisplayRWtoGNT (
  IN UINT32  DcpmmTurnaroundTimes[]
  )
{
  // SPR does not program t_RDGNT_DD
  RcDebugPrint (SDBG_MAX,
                "PMem RW2GNT\n       %a = %d\n       %a = %d\n       %a = %d\n",
                "t_ddrt_rdgnt_s ", DcpmmTurnaroundTimes[t_RDGNT_S],
                "t_ddrt_wrgnt_s ", DcpmmTurnaroundTimes[t_WRGNT_S],
                "t_ddrt_wrgnt_dd", DcpmmTurnaroundTimes[t_WRGNT_DD]
                );
}

/**

  Programs DCPMM turnaround times after training.

  @param[in] Socket  - Socket number

  @retval EFI_SUCCESS   Turnaround CSRs read/write successful

**/
EFI_STATUS
EFIAPI
OptimizeTurnaroundTimingsDcpmm (
  IN UINT8  Socket
  )
{
  EFI_STATUS                                 Status = EFI_SUCCESS;
#ifdef DDRT_SUPPORT
  UINT8                                      FreqIndex;
  UINT32                                     DdrCwl; // DDR Write latency
  CONST DCPMM_TURNAROUND_CONSTANTS           *DcpmmTurnaroundConstants = NULL;
  UINT32                                     DcpmmTurnaroundSettings[DcpmmTurnaroundTypeMax];
  UINT32                                     TurnaroundSettings[TurnaroundTypeMax];
  PSYSHOST                                   Host;
  struct channelNvram                        (*ChannelNvList)[MAX_CH];
  struct dimmNvram                           (*DimmNvList)[MAX_DIMM];
  UINT8                                      Strobe;
  UINT8                                      Channel;
  UINT8                                      Dimm;
  UINT8                                      DdrtCl = 0;  // DDRT CAS latency
  UINT8                                      DdrtCwl = 0; // DDRT Write latency
  INT16                                      Trained_Delay[MAX_STROBE];
  INT16                                      Max_Trained_Delay = 0;
  INT32                                      t_ddrt_wrgnt_s_temp;
  UINT8                                      PreambleLen;
  UINT8                                      TwlEffective;
  INT16                                      TxCycle;
  INT16                                      RecEnCycle;
  BUS_TIMING_DATA                            BusTimingData;
  struct ddrRank                             (*RankList)[MAX_RANK_DIMM];
  UINT8                                      MSVx8;
  SYS_SETUP                                  *Setup;
  UINT8                                      MaxChDdr;

  //
  // SPR Note: t_RDGNT_DD is not currently defined an any register
  //

  Setup = GetSysSetupPointer ();

  MaxChDdr = GetMaxChDdr ();
  Host = (PSYSHOST) GetSysHostPointer ();
  MSVx8 = GetMaxStrobeValid (Host) / 2;

  FreqIndex = Host->nvram.mem.socket[Socket].ddrFreq;
  InitTurnaroundTable (DcpmmTurnaroundSettings, DcpmmTurnaroundTypeMax);
  InitTurnaroundTable (TurnaroundSettings, TurnaroundTypeMax);

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (((*ChannelNvList)[Channel].enabled == 0) || ((*ChannelNvList)[Channel].ddrtEnabled == 0)) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Channel);
    InitBusTimingData (Socket, Channel, &BusTimingData, TRUE);
    DdrCwl = GettCWL (Host, Socket, Channel);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 1) {
        if ((*DimmNvList)[Dimm].DcpmmPresent == 1) {
          RankList = GetRankNvList(Host, Socket, Channel, Dimm);

          GetDdrtCasLatencies (Socket, Channel, Dimm, &DdrtCl, NULL, &DdrtCwl, NULL);

          Max_Trained_Delay = -0x8000;

          for (Strobe = 0; Strobe < MAX_STROBE; Strobe++) {
            Trained_Delay[Strobe] = 0;

            TxCycle = (INT16)((((*RankList)[0].lrBuf_FxBCDxFx[Strobe % MSVx8]) >> (4 * (Strobe / MSVx8))) & 0x0F);
            if (TxCycle > 2) {
              TxCycle = -1 * (TxCycle & 0x3);
            }

            RecEnCycle = (INT16)((((*RankList)[0].lrBuf_FxBCCxEx[Strobe % MSVx8]) >> (4 * (Strobe / MSVx8))) & 0x0F);
            if (RecEnCycle > 2) {
              RecEnCycle = -1 * (RecEnCycle & 0x3);
            }

            RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, 0, Strobe, NO_BIT,
                      "FxBCDxFx = %d, TxCycle=%d FxBCAxBx= %d \n", (*RankList)[0].lrBuf_FxBCDxFx[Strobe % MSVx8], TxCycle, (*RankList)[0].lrBuf_FxBCAxBx[Strobe]);
            RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, 0, Strobe, NO_BIT,
                      "FxBCCxEx = %d, RecEnCycle=%d, FxBC2x3x= %d \n", (*RankList)[0].lrBuf_FxBCCxEx[Strobe % MSVx8], RecEnCycle, (*RankList)[0].lrBuf_FxBC2x3x[Strobe]);

            Trained_Delay[Strobe] = (TxCycle * 64 + (*RankList)[0].lrBuf_FxBCAxBx[Strobe]) - (RecEnCycle * 64 + (*RankList)[0].lrBuf_FxBC2x3x[Strobe]);


            if (Max_Trained_Delay < Trained_Delay[Strobe]) {
              Max_Trained_Delay = Trained_Delay[Strobe];
            }
            RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, 0, Strobe, NO_BIT,
                      "Max_Trained_Delay = %d, Trained_Delay= %d \n", Max_Trained_Delay, Trained_Delay[Strobe]);
          }
        }
      }
    }

    GetDcpmmTurnaroundConstants (&DcpmmTurnaroundConstants);
    GetSetDcpmmTurnaroundSettings (Socket, Channel, GSM_READ_ONLY, DcpmmTurnaroundSettings);
    GetSetTurnaroundSettings (MemTechFmc, Socket, Channel, GSM_READ_CSR, TurnaroundSettings);

    // Read/Write to Read/Write
    DcpmmTurnaroundSettings[t_RDRD_S]  = DcpmmTurnaroundConstants->RdRdSCosnt;
    DcpmmTurnaroundSettings[t_RDRD_DD] = DcpmmTurnaroundConstants->RdRdDDConst;
    DcpmmTurnaroundSettings[t_WRRD_S]  = DcpmmTurnaroundConstants->WrRdSConst;
    DcpmmTurnaroundSettings[t_WRRD_DD] = DcpmmTurnaroundConstants->WrRdDDConst;

    DcpmmTurnaroundSettings[t_RDWR_S]  = DcpmmTurnaroundConstants->RdWrSConst;
    DcpmmTurnaroundSettings[t_RDWR_DD] = DcpmmTurnaroundConstants->RdWrDDConst;

    if (IsSiliconWorkaroundEnabled ("S14011156829") && IsChIn2lmCrossChannelSystem (Host, Socket, Channel, DdrtIn2lmCrossCh)) {
      // B0 Silicon Workaround: Increase write to write delay for DDRT2: t_ddrt_wrwr_s=6 for the channel which is sending cross-channel write completions.
      DcpmmTurnaroundSettings[t_WRWR_S]  = DDRT_T_WRWR_S_B0_SI_WA_VALUE;
    } else {
      DcpmmTurnaroundSettings[t_WRWR_S]  = DDRT_T_WRWR_S_DEFAULT_VALUE; // Default value in RDL
    }

    DcpmmTurnaroundSettings[t_WRWR_DD] = (DdrCwl - DdrtCwl) > 0 ? (DdrCwl - DdrtCwl) : 0;
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "WWSR (DDRT to DDRT)[%d]\n",
                    DcpmmTurnaroundSettings[t_WRWR_S]);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "WWDD (DDR to DDRT)[%d] = tCWLDDR[%d] - tCLDDRT[%d]\n", // tWMR: "Write Mode Return" Timing
                    DcpmmTurnaroundSettings[t_WRWR_DD], DdrCwl, DdrtCwl);


    // Grant to Read/Write
    DcpmmTurnaroundSettings[t_GNTRD_S] = DcpmmTurnaroundConstants->GntRdSConst;
    DcpmmTurnaroundSettings[t_GNTRD_DD] = DcpmmTurnaroundConstants->GntRdDDConst;
    if (UbiosGenerationOrHsleEnabled ()) {
      DcpmmTurnaroundSettings[t_GNTWR_S] = 0xB;
    } else {
      DcpmmTurnaroundSettings[t_GNTWR_S] = CalctDdrttoDdrtRWSR10nm (Host, Socket, Channel, DdrtCl, DdrtCwl, &BusTimingData);
    }
    DcpmmTurnaroundSettings[t_GNTWR_DD] = CalctDdr4toDdrtRWDD10nm (Host, Socket, Channel, DdrtCwl, &BusTimingData);

    // Read/Write to Grant
    DcpmmTurnaroundSettings[t_RDGNT_S] = DcpmmTurnaroundConstants->RdGntSConst;
    DcpmmTurnaroundSettings[t_WRGNT_S] = DcpmmTurnaroundConstants->WrGntSConst;
    DcpmmTurnaroundSettings[t_WRGNT_DD] = CalctDdr4toDdrtWRDD10nm (Host, Socket, Channel, DdrtCl, &BusTimingData);

    TwlEffective = GetDdrtWriteLatency(Socket, Channel);
    PreambleLen = GetPreambleState(Socket, READ_PREAMBLE);

    t_ddrt_wrgnt_s_temp = TwlEffective - DdrtCl + PreambleLen + 1;

    if (Setup->mem.optionsExt & WR_CRC) {
      t_ddrt_wrgnt_s_temp += 1;
    }
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "t_ddrt_wrgnt_s_temp = %d, TwlEffective= %d, CASTemp =%d, PreambleLen = %d \n", t_ddrt_wrgnt_s_temp, TwlEffective, DdrtCl, PreambleLen + 1);

    if (t_ddrt_wrgnt_s_temp < 0) {
      DcpmmTurnaroundSettings[t_WRGNT_S] = 0x8; // BL/2 + Pre-amble + Post-amble
    } else {
      DcpmmTurnaroundSettings[t_WRGNT_S] = DcpmmTurnaroundConstants->WrGntSConst + (UINT32)(t_ddrt_wrgnt_s_temp);
    }

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "PMem t_WRGNT_S = %d \n", DcpmmTurnaroundSettings[t_WRGNT_S]);

    // Grant to Grant
    DcpmmTurnaroundSettings[t_GNTGNT_S] = DcpmmTurnaroundConstants->GntGntSConst;
    DcpmmTurnaroundSettings[t_GNTGNT_DD] = CalctDdr4toDdrtRRDD10nm (Host, Socket, Channel, &BusTimingData);

    // Different DIMM
    TurnaroundSettings[t_WWDD] = (DdrtCwl - DdrCwl) > 0 ? (DdrtCwl - DdrCwl) : 0;
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "WWDD (DDRT to DDR)[%d] = tCWLDDRT[%d] - tCLDDR[%d]\n", // tWMR: "Write Mode Return" Timing
                    TurnaroundSettings[t_WWDD], DdrtCwl, DdrCwl);

    //DDRT GNT to DDR4 Read
    TurnaroundSettings[t_RRDD] = CalctDdrttoDdr4RRDD10nm (Host, Socket, Channel, &BusTimingData);
    //DDRT GNT to DDR4 Write
    TurnaroundSettings[t_RWDD] = CalctDdrttoDdr4RWDD10nm (Host, Socket, Channel, DdrtCl, &BusTimingData);
    // DDRT Write to DDR4 Read
    TurnaroundSettings[t_WRDD] = CalctDdrttoDdr4WRDD10nm (Host, Socket, Channel, DdrtCwl, &BusTimingData);

    GetSetTurnaroundSettings (MemTechFmc, Socket, Channel, GSM_FORCE_WRITE, TurnaroundSettings);
    GetSetDcpmmTurnaroundSettings (Socket, Channel, GSM_FORCE_WRITE, DcpmmTurnaroundSettings);

    DisplayTurnaroundTimesDcpmm (MemTechFmc, Socket, Channel);
  }// CH loop
#endif // DDRT_SUPPORT

  return Status;
}

/**

  Get turnaround times from Perf table and properly caculated out

  @param Socket         - Socket number
  @param Channel        - Channel number
  @param PerfTableRegs  - Perf table register matrix, will be updated after this function.

  @retval EFI_SUCCESS if turnaround times are available
          !EFI_SUCCESS otherwise

**/
EFI_STATUS
GetPerfTableTaValuesPerCh (
  IN UINT8                         Socket,
  IN UINT8                         Channel,
  IN OUT UINT32                    PerfTableSettings[]
  )
{
  PSYSHOST                        Host;
  struct channelNvram             (*ChannelNvList)[MAX_CH];
  UINT32                          WriteBurstLen;
  UINT32                          ReadBurstLen;
  UINT16                          tCL;
  UINT16                          tWL;
  UINT16                          tCCD_L;
  UINT16                          tCCD_S;
  UINT16                          tCCD_WR_L;
  UINT16                          tWTR_S;
  UINT16                          tWTR_L;
  UINT8                           tWPRE;
  UINT8                           t_rwds_offset;
  CONST TURNAROUND_CONSTANTS      *TurnaroundConstants;
  INT16                           TrwdxAdder;
  INT16                           TwrdxAdder;

  Host = (PSYSHOST) GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  WriteBurstLen = GetWriteBurstLength (Channel);
  ReadBurstLen = GetReadBurstLength ();

  //
  // Get pointer to the turnaround constants table for this configuration
  //
  if (EFI_SUCCESS != GetTurnaroundConstants (Host, Socket, Channel, &TurnaroundConstants)) {
    RcDebugPrint (SDBG_DEFAULT, "GetPerfTableTaValuesPerCh - Unsupported frequency\n");
    return EFI_UNSUPPORTED;
  }

  tCL    = (*ChannelNvList)[Channel].common.nCL;
  tWL    = (*ChannelNvList)[Channel].common.nWL;

  tWTR_S = (*ChannelNvList)[Channel].common.nWTR;
  tWTR_L = (*ChannelNvList)[Channel].common.nWTR_L;

  tCCD_S    = (*ChannelNvList)[Channel].common.tCCD;
  tCCD_L    = (*ChannelNvList)[Channel].common.tCCD_L;
  tCCD_WR_L = (*ChannelNvList)[Channel].common.tCCD_WR_L;

  tWPRE = GetTWpre (Socket, READ_OR_WRITE_PREAMBLE);
  TrwdxAdder = (tCL > tWL ? (tCL - tWL) : 0);
  TwrdxAdder = (tWL > tCL ? (tWL - tCL) : 0);

  //
  // Same bank group, different bank
  //
  PerfTableSettings[t_RRSG] = tCCD_L;
  PerfTableSettings[t_WWSG] = tCCD_WR_L;
  PerfTableSettings[t_RWSG] = (ReadBurstLen / QCLK_PER_DCLK) + TurnaroundConstants->t_rwsg + TrwdxAdder;
  // Note: For WRSG, tWTR_L increase is not needed when WrCRC. Therefore, GetReadBurstLength
  //       is used instead of GetWriteBurstLength since it is not affected by WrCRC
  PerfTableSettings[t_WRSG] = (ReadBurstLen / QCLK_PER_DCLK) + tWL + tWTR_L;

  //
  // Different bank group
  //
  PerfTableSettings[t_RRSR] = ((tCCD_S < 2) ? 0 : tCCD_S);
  PerfTableSettings[t_WWSR] = ((tCCD_S < 2) ? 0 : tCCD_S);
  PerfTableSettings[t_RWSR] = (ReadBurstLen / QCLK_PER_DCLK) + TurnaroundConstants->t_rwsr + TrwdxAdder;
  // Note: For WRSR, tWTR_S increase is not needed when WrCRC. Therefore, GetReadBurstLength
  //       is used instead of GetWriteBurstLength since it is not affected by WrCRC
  PerfTableSettings[t_WRSR] = (ReadBurstLen / QCLK_PER_DCLK) + tWL + tWTR_S;

  //
  // Different rank
  //
  PerfTableSettings[t_RRDR] = (ReadBurstLen / QCLK_PER_DCLK) + TurnaroundConstants->t_rrdr;
  PerfTableSettings[t_WWDR] = (WriteBurstLen / QCLK_PER_DCLK) + TurnaroundConstants->t_wwdr;
  PerfTableSettings[t_RWDR] = (ReadBurstLen / QCLK_PER_DCLK) + TurnaroundConstants->t_rwdr + TrwdxAdder;
  PerfTableSettings[t_WRDR] = (WriteBurstLen / QCLK_PER_DCLK) + TurnaroundConstants->t_wrdr + TwrdxAdder;

  //
  // Different DIMM
  //
  PerfTableSettings[t_RRDD] = (ReadBurstLen / QCLK_PER_DCLK) + TurnaroundConstants->t_rrdd;
  PerfTableSettings[t_WWDD] = (WriteBurstLen / QCLK_PER_DCLK) + TurnaroundConstants->t_wwdd;
  PerfTableSettings[t_RWDD] = (ReadBurstLen / QCLK_PER_DCLK) + TurnaroundConstants->t_rwdd + TrwdxAdder;
  PerfTableSettings[t_WRDD] = (WriteBurstLen / QCLK_PER_DCLK) + TurnaroundConstants->t_wrdd + TwrdxAdder;

  // Different subrank
  PerfTableSettings[t_RRDS] = PerfTableSettings[t_RRDR];
  PerfTableSettings[t_WWDS] = PerfTableSettings[t_WWDR];
  PerfTableSettings[t_RWDS] = PerfTableSettings[t_RWDR];
  PerfTableSettings[t_WRDS] = PerfTableSettings[t_WRDR];

  if ((*ChannelNvList)[Channel].encodedCSMode == 2) { // 3DS
    //
    // For t_rwds, DDR4 uses the offset for t_rwdr and DDR5 uses the offset for t_rwsr
    //
    t_rwds_offset = TurnaroundConstants->t_rwdr;
    if (IsDdr5MemSsPresent (Host, Socket)) {
      t_rwds_offset = TurnaroundConstants->t_rwsr;
    }
    PerfTableSettings[t_RWDS] = (ReadBurstLen / QCLK_PER_DCLK) + tCL - tWL + t_rwds_offset;
    PerfTableSettings[t_WRDS] = (WriteBurstLen / QCLK_PER_DCLK) + tWL + tWTR_S;
  }
  return EFI_SUCCESS;
}

/**

  Returns a Minimum turnaround setting for desired DDR4 turnaround timing

  @param Socket                    - Socket number
  @param Channel                   - Channel number
  @param[in]  Selection            - Desired turnaround timing
  @param[out] MinTurnaroundSetting - Minimum turnaround setting for desired turnaround timing

  @retval  EFI_SUCCESS   - Found it
           EFI_NOT_FOUND - Not found

**/
EFI_STATUS
GetDdr4TurnaroundTimingMinLimits (
  IN  UINT8                            Socket,
  IN  UINT8                            Channel,
  IN  TURNAROUND_TYPE                  Selection,
  OUT UINT32                           *MinTurnaroundSetting
  )
{
  // Not used for SPR yet
  return EFI_NOT_FOUND;
}

/**

  Returns a Minimum turnaround setting for desired DDRT turnaround timing

  @param Socket                    - Socket number
  @param Channel                   - Channel number
  @param[in]  Selection            - Desired turnaround timing
  @param[out] MinTurnaroundSetting - Minimum turnaround setting for desired turnaround timing

  @retval  EFI_SUCCESS   - Found it
           EFI_NOT_FOUND - Not found

**/
EFI_STATUS
GetDdrtTurnaroundTimingMinLimits (
  IN  UINT8                            Socket,
  IN  UINT8                            Channel,
  IN  DCPMM_TURNAROUND_TYPE            Selection,
  OUT UINT32                           *MinTurnaroundSetting
  )
{
  // Not used for SPR yet
  return EFI_NOT_FOUND;
}

/**

  This function calculates the read to read turnaround time for same group on DIMM
  for this SOC

  @param[in] Host            - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket          - Socket number
  @param[in] Channel         - Channel number

  @retval Trrsg channel value

**/
UINT8
CalculateRRSGSoc (
  IN  PSYSHOST         Host,                     // Pointer to sysHost, the system Host (root) structure
  IN  UINT8            Socket,                   // CPU Socket Node number (Socket ID)
  IN  UINT8            Channel                   // Channel number (0-based)
  )
{
  UINT32               Trrsg = 0;
  struct channelNvram  (*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);
  Trrsg = (*ChannelNvList) [Channel].common.tCCD_L;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Trrsg =%d\n", Trrsg);

  return (UINT8) Trrsg;
} // CalculateRRSGSoc

/**

  This function calculates the read to read turnaround time for same rank on the same DIMM
  for this SOC

  @param[in] Host            - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket          - Socket number
  @param[in] Channel         - Channel number

  @retval Trrsr channel value

**/
UINT8
CalculateRRSRSoc (
  IN  PSYSHOST         Host,                     // Pointer to sysHost, the system Host (root) structure
  IN  UINT8            Socket,                   // CPU Socket Node number (Socket ID)
  IN  UINT8            Channel                   // Channel number (0-based)
  )
{
  UINT32               Trrsr = 0;
  struct channelNvram  (*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);
  Trrsr = (*ChannelNvList) [Channel].common.tCCD;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Trrsr =%d\n", Trrsr);

  return (UINT8) Trrsr;
} // CalculateRRSRSoc


/**

  This function calculates the write to write turnaround time for same group on the same DIMM
  for this SOC

  @param[in] Host            - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket          - Socket number
  @param[in] Channel         - Channel number

  @retval Twwsg channel value

**/
UINT8
CalculateWWSGSoc (
  IN  PSYSHOST         Host,                     // Pointer to sysHost, the system Host (root) structure
  IN  UINT8            Socket,                   // CPU Socket Node number (Socket ID)
  IN  UINT8            Channel                   // Channel number (0-based)
  )
{
  UINT32               Twwsg = 0;
  struct channelNvram  (*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);
  Twwsg = (*ChannelNvList) [Channel].common.tCCD_WR_L;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Twwsg =%d\n", Twwsg);

  return (UINT8) Twwsg;
} // CalculateWWSGSoc

/**

  This function calculates the write to write turnaround time for same rank on the same DIMM
  for this SOC

  @param[in] Host            - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket          - Socket number
  @param[in] Channel         - Channel number

  @retval Twwsr channel value

**/
UINT8
CalculateWWSRSoc (
  IN  PSYSHOST         Host,                     // Pointer to sysHost, the system Host (root) structure
  IN  UINT8            Socket,                   // CPU Socket Node number (Socket ID)
  IN  UINT8            Channel                   // Channel number (0-based)
  )
{
  UINT32               Twwsr = 0;
  struct channelNvram  (*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);
  Twwsr = (*ChannelNvList) [Channel].common.tCCD_WR;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Twwsr =%d\n", Twwsr);

  return (UINT8) Twwsr;
} // CalculateWWSRSoc

/**

  This function calculates the write to read turnaround time for same group on the same DIMM
  for this SOC

  @param[in] Host            - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket          - Socket number
  @param[in] Channel         - Channel number

  @retval Twrsg channel value

**/
UINT8
CalculateWRSGSoc (
  IN  PSYSHOST         Host,                     // Pointer to sysHost, the system Host (root) structure
  IN  UINT8            Socket,                   // CPU Socket Node number (Socket ID)
  IN  UINT8            Channel                   // Channel number (0-based)
  )
{
  UINT32               Twrsg = 0;
  struct channelNvram  (*ChannelNvList)[MAX_CH];
  UINT32               WriteBurstLenDclk = 0;

  //
  // tCCD_L_WTR_slr = CWL + WBL/2 + tWTR_L
  //
  ChannelNvList = GetChannelNvList (Host, Socket);
  WriteBurstLenDclk = GetWriteBurstLength (Channel) / QCLK_PER_DCLK;
  Twrsg = (*ChannelNvList) [Channel].common.nWL + (*ChannelNvList) [Channel].common.nWTR_L + WriteBurstLenDclk;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Twrsg =%d\n", Twrsg);

  return (UINT8) Twrsg;
} // CalculateWRSGSoc

/**

  This function calculates the write to read turnaround time for the same ranks on the same DIMM
  for this SOC.

  @param[in]  Host            - Point to sysHost
  @param[in]  Socket          - Socket number
  @param[in]  Channel         - Channel number

  @retval tWRSR

**/
UINT8
CalculateWRSRSoc (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Channel
  )
{
  UINT32  Twrsr = 0;
  UINT32  WtrValue = 0;
  UINT32  WriteBurstLenDclk = 0;
  UINT32  TCwlValue = 0;

  //
  // tCCD_L_WTR_slr = CWL + WBL/2 + tWTR_S
  //
  WriteBurstLenDclk = GetWriteBurstLength (Channel) / QCLK_PER_DCLK;
  TCwlValue = GettCWL (Host, Socket, Channel);
  WtrValue = GettWTR_S (Host, Socket, Channel);
  Twrsr = TCwlValue + WriteBurstLenDclk + WtrValue;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Twrsr =%d\n", Twrsr);

  return (UINT8) Twrsr;
} // CalculateWRSRSoc

/**

  Calculate and program optimal TA times based on formulas and training results

  @param[in] Socket              - Socket number
  @param[in] Ch                  - Channel number
  @param[in] TurnaroundSettings  - Pointer to table containing settings to be programmed

  @retval none

**/
VOID
OptimizeTurnaroundTimingsSoc (
  IN UINT8                           Socket,
  IN UINT8                           Ch,
  IN UINT32                          TurnaroundSettings[]
  )
{
#ifdef LRDIMM_SUPPORT
  UINT8               Dimm;
#endif // LRDIMM_SUPPORT
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  PSYSHOST            Host;

  //
  // Calculate 6 turnaround timings which are not covered by turnaround training
  //
  if (!IsMemFlowEnabled (TurnaroundTraining)) {
    return;
  }

  Host = (PSYSHOST) GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  //
  // The following code is used to optimize turnaround times based on training results
  //
  TurnaroundSettings[t_RRSG] = CalculateRRSGSoc (Host, Socket, Ch);
  TurnaroundSettings[t_RRSR] = CalculateRRSRSoc (Host, Socket, Ch);

  TurnaroundSettings[t_WWSG] = CalculateWWSGSoc (Host, Socket, Ch);
  TurnaroundSettings[t_WWSR] = CalculateWWSRSoc (Host, Socket, Ch);

  TurnaroundSettings[t_WRSG] = CalculateWRSGSoc (Host, Socket, Ch);
  TurnaroundSettings[t_WRSR] = CalculateWRSRSoc (Host, Socket, Ch);

#ifdef LRDIMM_SUPPORT
  // Montage WA
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }
    if (((*DimmNvList)[Dimm].SPDRegVen == MFGID_MONTAGE) && ((*DimmNvList)[Dimm].lrbufGen == 0x01) && ((*DimmNvList)[Dimm].lrbufRid == LRBUF_MONTAGE_RID_GEN2_A0) && ((*ChannelNvList)[Ch].lrDimmPresent)) {
      if (TurnaroundSettings[t_WWSR] < 2) {
        TurnaroundSettings[t_WWSR] = 2;
      }
    } // if WA Dimm type
  } // Dimm loop
#endif // LRDIMM_SUPPORT
}
