/** @file
    MemTurnaroundTimingSoc.c - contains the Memory turnaround timing functions for 10nm.

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


#include <SysHost.h>
#include <MemHostChipCommon.h>
#include <Library/MemTurnaroundTimingLib.h>
#include "Include/MemTurnaroundTimingLibInternal.h"
#include <Library/MemoryCoreLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/RcDebugLib.h>
#include <Library/MemRcLib.h>

/**

  Program early turnaround times in accordance with Turnaround doc for this SOC.
  Display at the end of training for final results.

  @param[in] Socket              - Socket number
  @param[in] Channel             - Channel number
  @param[in] TurnaroundTable     - Pointer to table containing constants provided by EV
  @param[in] TurnaroundSettings  - Pointer to table containing settings to be programmed

  @retval none

**/
VOID
RelaxTurnaroundTimesSoc (
  IN UINT8                           Socket,
  IN UINT8                           Channel,
  IN CONST TURNAROUND_CONSTANTS      *TurnaroundTable,
  IN UINT32                          TurnaroundSettings[]
  )
{
  UINT32               tCL;
  UINT32               tWL;
  UINT32               TclMinusTcwl;
  UINT32               TcwlMinusTcl;
  UINT16               tWTR_L;
  UINT16               tWTR_S;
  UINT32               WriteBurstLen;
  UINT32               ReadBurstLen;
  PSYSHOST             Host;
  UINT8                LoopLatency;
  struct channelNvram  (*ChannelNvList)[MAX_CH];

  Host          = (PSYSHOST) GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  (*ChannelNvList)[Channel].common.tCCD      = tCCD_S_DEFINE;
  (*ChannelNvList)[Channel].common.tCCD_L    = GetTccd_L (Socket, Channel);
  (*ChannelNvList)[Channel].common.tCCD_WR   = GetTccdWr_S (Socket, Channel);
  (*ChannelNvList)[Channel].common.tCCD_WR_L = GetTccdWr_L (Socket, Channel);

  tCL           = GettCL (Host, Socket, Channel);
  tWL           = GettCWL (Host, Socket, Channel);
  tWTR_L        = (*ChannelNvList)[Channel].common.nWTR_L;
  tWTR_S        = (*ChannelNvList)[Channel].common.nWTR;
  WriteBurstLen = GetWriteBurstLength (Channel);
  ReadBurstLen  = GetReadBurstLength ();
  LoopLatency   = GetLoopLatency ();
  TclMinusTcwl  = (tCL > tWL ? (tCL - tWL) : 0);
  TcwlMinusTcl  = (tWL > tCL ? (tWL - tCL) : 0);

  //
  // fill in Turnaround settings table from calculated values
  //

  //
  // t_xxSR
  //
  TurnaroundSettings[t_RRSR] = (*ChannelNvList)[Channel].common.tCCD;
  TurnaroundSettings[t_WWSR] = (*ChannelNvList)[Channel].common.tCCD;

  if (!(IsDdr5MemSsPresent(Host, Socket)) && (TurnaroundSettings[t_RRSR] == 5) && (GetPreambleState (Socket, READ_PREAMBLE) == PREAMBLE_2TCLK)) {
    TurnaroundSettings[t_RRSR] += 1;
  }
  if (!(IsDdr5MemSsPresent(Host, Socket)) && (TurnaroundSettings[t_WWSR] == 5) && (GetPreambleState (Socket, WRITE_PREAMBLE) == PREAMBLE_2TCLK)) {
    TurnaroundSettings[t_WWSR] += 1;
  }

  TurnaroundSettings[t_RWSR] = TurnaroundTable->t_rwsr + (ReadBurstLen / QCLK_PER_DCLK) + TclMinusTcwl;
  // Note: For WRSR, tWTR_S increase is nor needed when WrCRC. Therefore, GetReadBurstLength
  //       is used instead of GetWriteBurstLength since it is not affected by WrCRC
  TurnaroundSettings[t_WRSR] = (ReadBurstLen / QCLK_PER_DCLK) + tWL + tWTR_S;

  //
  // t_xxSG
  //
  TurnaroundSettings[t_RRSG] = (*ChannelNvList)[Channel].common.tCCD_L;
  if (!(IsDdr5MemSsPresent (Host, Socket)) && (TurnaroundSettings[t_RRSG] == 5) && (GetPreambleState (Socket, READ_PREAMBLE) == PREAMBLE_2TCLK)) {
    TurnaroundSettings[t_RRSG] += 1;
  }
  TurnaroundSettings[t_WWSG] = (*ChannelNvList)[Channel].common.tCCD_WR_L;
  if (!(IsDdr5MemSsPresent (Host, Socket)) && (TurnaroundSettings[t_WWSG] == 5) && (GetPreambleState (Socket, WRITE_PREAMBLE) == PREAMBLE_2TCLK)) {
    TurnaroundSettings[t_WWSG] += 1;
  }
  TurnaroundSettings[t_RWSG] = TurnaroundTable->t_rwsg + (ReadBurstLen / QCLK_PER_DCLK) + TclMinusTcwl;
  // Note: For WRSG, tWTR_L increase is nor needed when WrCRC. Therefore, GetReadBurstLength
  //       is used instead of GetWriteBurstLength since it is not affected by WrCRC
  TurnaroundSettings[t_WRSG] = (ReadBurstLen / QCLK_PER_DCLK) + tWL + tWTR_L;

  //
  // t_xxDR
  //
  TurnaroundSettings[t_RRDR] = TurnaroundTable->t_rrdr + (ReadBurstLen / QCLK_PER_DCLK);
  TurnaroundSettings[t_WWDR] = TurnaroundTable->t_wwdr + (WriteBurstLen / QCLK_PER_DCLK);
  TurnaroundSettings[t_RWDR] = TurnaroundTable->t_rwdr + (ReadBurstLen / QCLK_PER_DCLK) + TclMinusTcwl;
  TurnaroundSettings[t_WRDR] = TurnaroundTable->t_wrdr + (WriteBurstLen / QCLK_PER_DCLK) + TcwlMinusTcl;
  // For 3DS DIMMs t_wrdr/t_wrds = CWL + WBL/2 + tWTR_S
  if ((*ChannelNvList)[Channel].encodedCSMode == 2) {
    TurnaroundSettings[t_WRDR] = tWL + (WriteBurstLen / QCLK_PER_DCLK) + tWTR_S;
  }

  //
  // t_xxDD
  //
  TurnaroundSettings[t_RRDD] = TurnaroundTable->t_rrdd + (ReadBurstLen / QCLK_PER_DCLK);
  TurnaroundSettings[t_WWDD] = TurnaroundTable->t_wwdd + (WriteBurstLen / QCLK_PER_DCLK);
  TurnaroundSettings[t_RWDD] = TurnaroundTable->t_rwdd + (ReadBurstLen / QCLK_PER_DCLK) + TclMinusTcwl;
  TurnaroundSettings[t_WRDD] = TurnaroundTable->t_wrdd + (WriteBurstLen / QCLK_PER_DCLK) + TcwlMinusTcl;

  //
  // t_xxDS
  //
  TurnaroundSettings[t_RRDS] = TurnaroundSettings[t_RRDD];
  TurnaroundSettings[t_WWDS] = TurnaroundSettings[t_WWDD];
  TurnaroundSettings[t_RWDS] = TurnaroundSettings[t_RWDD];
  TurnaroundSettings[t_WRDS] = TurnaroundSettings[t_WRDR];

  //
  // t_rrd_dlr same as t_rrds
  //
  TurnaroundSettings[t_RRDLR] = TurnaroundSettings[t_RRDS];

  if (IsBrsPresent (Host, Socket)) {
    //
    // Relax turnaround settings for BRS
    //
    TurnaroundSettings[t_WWSR] = 10 + 2;
    TurnaroundSettings[t_RWSR] = 37 + 2;
    TurnaroundSettings[t_WRSR] = 46 + 2;
    TurnaroundSettings[t_RWSG] = 37 + 2;
    TurnaroundSettings[t_RRSR] = 10 + 2;
    TurnaroundSettings[t_WRSG] = 58 + 2;
    TurnaroundSettings[t_WWSG] = 31 + 2;
    TurnaroundSettings[t_RRSG] = 10 + 2;
    TurnaroundSettings[t_RWDS] = 27 + 2;
  }
}

/**

  Programs safe (non-optimized) DDRT turnaround timings

  @param[in] Socket  - Socket number

  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
SetSafeTurnaroundTimingsDcpmm (
  UINT8    Socket
)
{
#ifdef DDRT_SUPPORT
  PSYSHOST         Host;
  UINT8            Channel;
  UINT32           DcpmmTurnaroundSettings[DcpmmTurnaroundTypeMax];
  UINT32           TurnaroundSettings[TurnaroundTypeMax];
  BUS_TIMING_DATA  BusTimingData;
  struct channelNvram  (*channelNvList)[MAX_CH];
  UINT8            LoopLatency;
  UINT8            MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = (PSYSHOST) GetSysHostPointer();
  LoopLatency = GetLoopLatency ();
  InitTurnaroundTable (DcpmmTurnaroundSettings, DcpmmTurnaroundTypeMax);
  InitTurnaroundTable (TurnaroundSettings, TurnaroundTypeMax);

  channelNvList = GetChannelNvList(Host, Socket);
  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if ((*channelNvList)[Channel].enabled == 0) {
      continue;
    }
    if ((*channelNvList)[Channel].ddrtEnabled == 0) {
      continue;
    }

    GetSetDcpmmTurnaroundSettings (Socket, Channel, GSM_READ_ONLY, DcpmmTurnaroundSettings);
    GetSetTurnaroundSettings (MemTechFmc, Socket, Channel, GSM_READ_CSR, TurnaroundSettings);

    InitBusTimingData (Socket, Channel, &BusTimingData, TRUE);

    DcpmmTurnaroundSettings[t_GNTGNT_DD] = CalctDdr4toDdrtRRDD10nm (Host, Socket, Channel, &BusTimingData);
    // Add Loop Latency into these turnaround values as DCPMM calculations do not account for loop latency and it is
    // subtracted in the GetSet function
    TurnaroundSettings[t_RRDD] = CalctDdrttoDdr4RRDD10nm (Host, Socket, Channel, &BusTimingData) + LoopLatency;

    GetSetTurnaroundSettings (MemTechFmc, Socket, Channel, GSM_FORCE_WRITE, TurnaroundSettings);
    GetSetDcpmmTurnaroundSettings (Socket, Channel, GSM_FORCE_WRITE, DcpmmTurnaroundSettings);
  }
#endif // DDRT_SUPPORT

  return EFI_SUCCESS;
} // SetSafeTurnaroundTimingsDcpmm

/**

  If a 3DS DIMM is present, this functions accounts for it by adjusting the necessary turnaround times

  @param[in]  Host                - Pointer to syshost
  @param[in]  Socket              - Socket number
  @param[in]  Channel             - Channel number
  @param[out] TurnaroundSettings  - Pointer to table containing settings to be programmed

  @retval none

**/
VOID
AdjustTimingsFor3DS (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT32    TurnaroundSettings[]
  )
{
  UINT16               WritePreamble;
  UINT32               TempValue;
  UINT32               WriteBurstLenDclk;
  UINT32               ReadBurstLenDclk;
  UINT32               TclMinusTcwl;
  struct channelNvram  (*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);
  WriteBurstLenDclk = GetWriteBurstLength (Channel) / QCLK_PER_DCLK;
  ReadBurstLenDclk = GetReadBurstLength () / QCLK_PER_DCLK;

  if ((*ChannelNvList)[Channel].encodedCSMode == 2) {
    WritePreamble = 0;
    if (GetPreambleState (Socket, WRITE_PREAMBLE) > PREAMBLE_1TCLK) {
      WritePreamble = 1;
    }
    // t_rwds = CL - CWL + RBL/2 + 1tCK + tWPRE
    TclMinusTcwl  = GettCL (Host, Socket, Channel) > GettCWL (Host, Socket, Channel) ? (GettCL (Host, Socket, Channel) - GettCWL (Host, Socket, Channel)) : 0;
    TurnaroundSettings[t_RWDS]  = TclMinusTcwl + ReadBurstLenDclk + 1 + WritePreamble;
    // t_wrds/t_wrdr/t_wrdd = CWL + WBL/2 + tWTR_S
    TempValue = (GettCWL (Host, Socket, Channel) + WriteBurstLenDclk + (*ChannelNvList)[Channel].common.nWTR);
    TurnaroundSettings[t_WRDD] = TempValue;
    TurnaroundSettings[t_WRDR] = TempValue;
    TurnaroundSettings[t_WRDS] = TempValue;


    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Update for 3DS t_rwds = %d, t_wrds = %d, t_wrdd = %d, t_wrdr = %d\n", TurnaroundSettings[t_RWDS], TurnaroundSettings[t_WRDS], TurnaroundSettings[t_WRDD], TurnaroundSettings[t_WRDR]);
  }
}

/**

  Returns a ODT stretch

  @param[in]  Socket           - Socket Number
  @param[in]  Channel          - Channel Number
  @param[in]  *BusTimingData   - Structure that holds turnaround data variables
  @param[out] Direction        - Direction for "rr", "rw", "wr", and "ww"

  @retval  OdtStretch

**/
UINT8
CalculateOdtStretch (
  IN  UINT8                     Socket,
  IN  UINT8                     Channel,
  IN  BUS_TIMING_DATA           *BusTimingData,
  IN  MRC_ODTSTRETCH_SELECTION  Direction
)
{
  PSYSHOST             Host;
  UINT8                OdtStretch = 0;
  struct channelNvram  (*ChannelNvList)[MAX_CH];

  Host = (PSYSHOST) GetSysHostPointer ();

  ChannelNvList = GetChannelNvList (Host, Socket);
  switch (Direction) {
    case RD_RD:
      if (((*BusTimingData).odtRdTrailing > 1) || ((*BusTimingData).odtRdLeading > 1)) {
        OdtStretch = MAX(OdtStretch, (*BusTimingData).odtRdTrailing - 1 + (*BusTimingData).odtRdLeading - 1);
      }
      break;
    case RD_WR:
      if (((*BusTimingData).odtRdTrailing > 1) || ((*BusTimingData).odtWrLeading > 1)) {
        OdtStretch = MAX (OdtStretch, (*BusTimingData).odtRdTrailing - 1 + (*BusTimingData).odtWrLeading - 1);
      }
      break;
    case WR_WR:
      if (((*BusTimingData).odtWrTrailing > 1) || ((*BusTimingData).odtWrLeading > 1)) {
        OdtStretch = MAX (OdtStretch, (*BusTimingData).odtWrTrailing - 1 + (*BusTimingData).odtWrLeading - 1);
      }
      break;
    case WR_RD:
      if (((*BusTimingData).odtWrTrailing > 1) || ((*BusTimingData).odtRdLeading > 1)) {
        OdtStretch = MAX (OdtStretch, (*BusTimingData).odtWrTrailing - 1 + (*BusTimingData).odtRdLeading - 1);
      }
      break;
    default:
      break;
  }

  return OdtStretch;
}

/**

  This function calculates the read to read turnaround time for different ranks on the same DIMM (10nm version)

  @param[in] Host            - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket          - Socket number
  @param[in] Channel         - Channel number
  @param[in] BusTimingData   - Structure that holds turnaround data variables

  @retval Trrdr channel value

**/
UINT8
CalculateRRDR10nm (
  IN  PSYSHOST         Host,                     // Pointer to sysHost, the system Host (root) structure
  IN  UINT8            Socket,                   // CPU Socket Node number (Socket ID)
  IN  UINT8            Channel,                  // Channel number (0-based)
  IN  BUS_TIMING_DATA  *BusTimingData            // Pointer to the structure for timing variables
  )
{
  UINT8   OdtStretch = 0;
  UINT32  Trrdr = 0;

  //
  // t_RRDR = 1(RTT Change Skew) + max(ODT Gap due to CK PI Difference (for UDIMM), ODT Stretch, 1 (ODT Gap due to CAS2RCVEN))
  //
  Trrdr = (*BusTimingData).cas2RecEnDR;

  //
  // Get ODT stretch
  //
  if (CheckOdtAsserted (Socket, Channel, DiffRank)) {
    OdtStretch = CalculateOdtStretch (Socket, Channel, BusTimingData, RD_RD);
    Trrdr += OdtStretch;
  }

  //
  // RTT Change Skew
  //
  Trrdr += 1;
  Trrdr += (*BusTimingData).clkDiffDR;


  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "cas2RecEnDR = %d, clkDiffDR = %d, OdtStretch = %d\n", (*BusTimingData).cas2RecEnDR, (*BusTimingData).clkDiffDR, OdtStretch);

  return (UINT8) Trrdr;
} // CalculateRRDR10nm

/**

  This function calculates the read to read turnaround time for different DIMMs (10nm version)

  @param[in] Socket          - Socket number
  @param[in] Channel         - Channel number
  @param[in] BusTimingData   - Structure that holds turnaround data variables

  @retval Trrdd channel value

**/
UINT8
CalculateRRDD10nm (
  IN  UINT8            Socket,
  IN  UINT8            Channel,
  IN  BUS_TIMING_DATA  *BusTimingData
  )
{
  UINT32  Trrdd = 0;
  UINT8   OdtStretch = 0;

  //
  // t_RRDR = 1(RTT Change Skew) + max(ODT Gap due to CK PI Difference (for UDIMM), ODT Stretch, 1 (ODT Gap due to CAS2RCVEN))
  //

  //
  // Get ODT stretch
  //
  if (CheckOdtAsserted (Socket, Channel, DiffDimm)) {
    OdtStretch = CalculateOdtStretch (Socket, Channel, BusTimingData, RD_RD);
    Trrdd += OdtStretch;
  }

  //
  // RTT Change Skew
  //
  Trrdd += 1;

  Trrdd += BusTimingData->cas2RecEnDD;
  Trrdd += BusTimingData->clkDiffDD;


  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "cas2RecEnDD = %d, clkDiffDD = %d, OdtStretch = %d\n", BusTimingData->cas2RecEnDD, BusTimingData->clkDiffDD, OdtStretch);

  //
  // +1 to WWDR, WWDS, WWDD, RRDD for now
  //
  Trrdd += 1;

  return (UINT8)Trrdd;
} // CalculateRRDD10nm

/**

  This function calculates the write to write turnaround time for different ranks on the same DIMM (10nm version)

  @param[in]  Host,           - Pointer to sysHost, the system Host (root) structure
  @param[in]  Socket          - CPU Socket Node number (Socket ID)
  @param[in]  Channel         - Channel number (0-based)
  @param[in]  BusTimingData   - Pointer to the structure for timing variables
  @param[out] Twwdr           - Pointer to the return value for Twwdr

  @retval None

**/
VOID
CalculateWWDR10nm (
  IN  PSYSHOST         Host,
  IN  UINT8            Socket,
  IN  UINT8            Channel,
  IN  BUS_TIMING_DATA  *BusTimingData,
  OUT UINT32           *Twwdr
  )
{
  UINT8               OdtStretch = 0;
  SYS_SETUP           *Setup;

  Setup = GetSysSetupPointer ();

  //
  // Rtt change skew
  //
  *Twwdr = 1;

  //
  // Get ODT stretch
  //
  if (CheckOdtAsserted (Socket, Channel, DiffRank)) {
    OdtStretch = CalculateOdtStretch (Socket, Channel, BusTimingData, WR_WR);
    *Twwdr += OdtStretch;
  }

  *Twwdr += (*BusTimingData).cas2DrvEnDR;
  *Twwdr += (*BusTimingData).clkDiffDR;
  *Twwdr += 1;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "cas2DrvEnDR = %d, clkDiffDR = %d, OdtStretch = %d\n", (*BusTimingData).cas2DrvEnDR, (*BusTimingData).clkDiffDR, OdtStretch);

  //
  // +1 to WWDR, WWDS, WWDD, RRDD for now
  //
  *Twwdr += 1;

  EnforceWWDR (Host, Channel, Twwdr);

} // CalculateWWDR10nm

/**

  This function calculates the write to write turnaround time for different DIMMs (10nm version)

  @param[in]  Host            - Pointer to sysHost, the system Host (root) structure
  @param[in]  Socket          - CPU Socket Node number (Socket ID)
  @param[in]  Channel         - Channel number (0-based)
  @param[in]  BusTimingData   - Pointer to the structure for timing variables
  @param[out] Twwdd           - Pointer to the return value for Twwdd

  @retval None

**/
VOID
CalculateWWDD10nm (
  IN  PSYSHOST         Host,
  IN  UINT8            Socket,
  IN  UINT8            Channel,
  IN  BUS_TIMING_DATA  *BusTimingData,
  OUT UINT32           *Twwdd
  )
{
  UINT8      OdtStretch = 0;
  SYS_SETUP  *Setup;
  struct channelNvram (*ChannelNvList)[MAX_CH];

  Setup = GetSysSetupPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  *Twwdd = (*BusTimingData).cas2DrvEnDD;

  //
  // Get ODT stretch
  //
  if (CheckOdtAsserted (Socket, Channel, DiffDimm)) {
    OdtStretch = CalculateOdtStretch (Socket, Channel, BusTimingData, WR_WR);
    *Twwdd += OdtStretch;
  }

  //
  // Rtt change skew
  //
  *Twwdd += 1;
  *Twwdd += (*BusTimingData).clkDiffDD;
  *Twwdd += 1;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "OdtStretch = %d, clkDiffDD = %d\n", OdtStretch, (*BusTimingData).clkDiffDD);

  //
  // +1 to WWDR, WWDS, WWDD, RRDD for now
  //
  *Twwdd += 1;

  EnforceWWDD (Host, Channel, Twwdd);

} // CalculateWWDD10nm

/**

  This function calculates the read to write turnaround time for same rank (10nm version)

  @param[in]  Host            - Pointer to sysHost, the system Host (root) structure
  @param[in]  Socket          - CPU Socket Node number (Socket ID)
  @param[in]  Channel         - Channel number (0-based)
  @param[in]  *BusTimingData  - Pointer to the structure for timing variables
  @param[out] *Trwsr          - Pointer to the return value for Trwsr

  @retval None

**/
VOID
CalculateRWSR10nm (
  IN  PSYSHOST         Host,
  IN  UINT8            Socket,
  IN  UINT8            Channel,
  IN  BUS_TIMING_DATA  *BusTimingData,
  OUT UINT32           *Trwsr
  )
{
  UINT8                OdtStretch = 0;
  *Trwsr = (*BusTimingData).effBusDelaySR;

  //
  // Check ODT asserted
  //
  if (CheckOdtAsserted (Socket, Channel, SameRank)) {
    OdtStretch = CalculateOdtStretch (Socket, Channel, BusTimingData, RD_WR);
    *Trwsr += OdtStretch;
  }

  //
  // RTT Change Skew
  //
  *Trwsr += 1;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "effBusDelaySR = %d\n", (*BusTimingData).effBusDelaySR);

} // CalculateRWSR10nm

/**

  This function calculates the read to write turnaround time for different ranks on the same DIMM (10nm version)

  @param[in]  Host            - Pointer to sysHost, the system Host(root)structure
  @param[in]  Socket          - CPU Socket Node number (Socket ID)
  @param[in]  Channel         - Channel number (0-based)
  @param[in]  *BusTimingData  - Pointer to the structure for timing variables
  @param[out] *Trwdr          - Pointer to the return value for Trwdr

  @retval None

**/
VOID
CalculateRWDR10nm (
  IN  PSYSHOST         Host,
  IN  UINT8            Socket,
  IN  UINT8            Channel,
  IN  BUS_TIMING_DATA  *BusTimingData,
  OUT UINT32           *Trwdr
  )
{
  UINT8                OdtStretch = 0;

  *Trwdr = (*BusTimingData).effBusDelayDR;

  //
  // Get ODT stretch
  //
  if (CheckOdtAsserted (Socket, Channel, DiffRank)) {
    OdtStretch = CalculateOdtStretch (Socket, Channel, BusTimingData, RD_WR);
    *Trwdr += OdtStretch;
  }

  //
  // RRT Change Skew
  //
  *Trwdr += 1;

  //
  // Get clk diff
  //
  if ((*BusTimingData).clkDiffDR) {
    *Trwdr += 1;
  }

  //
  // Add an extra clock for now
  //
  *Trwdr += 1;

} // CalculateRWDR10nm

/**

  This function calculates the read to write turnaround time for different ranks on the different DIMM (10nm version)

  @param[in]  Host            - Pointer to sysHost, the system Host (root) structure
  @param[in]  Socket          - CPU Socket Node number (Socket ID)
  @param[in]  Channel         - Channel number (0-based)
  @param[in]  *BusTimingData  - Pointer to the structure for timing variables
  @param[out] *Trwdd          - Pointer to the return value for tRWDD

  @retval None

**/
VOID
CalculateRWDD10nm (
  IN  PSYSHOST         Host,
  IN  UINT8            Socket,
  IN  UINT8            Channel,
  IN  BUS_TIMING_DATA  *BusTimingData,
  OUT UINT32           *tRWDD
  )
{
  UINT8  OdtStretch = 0;
  *tRWDD = (*BusTimingData).effBusDelayDD;
  //
  // RTT Change Skew
  //
  *tRWDD += 1;

  //
  // Get ODT stretch
  //
  if (CheckOdtAsserted (Socket, Channel, DiffDimm)) {
    OdtStretch = CalculateOdtStretch (Socket, Channel, BusTimingData, RD_WR);
    *tRWDD += OdtStretch;
  }

  //
  // Get clk diff
  //
  if ((*BusTimingData).clkDiffDD) {
    *tRWDD += 1;
  }

  *tRWDD += 1; // Add an extra clock for now per Grantley BIOS s5385877

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "effBusDelayDD = %d, OdtStretch = %d, clkDiffDD = %d\n", (*BusTimingData).effBusDelayDD, OdtStretch, (*BusTimingData).clkDiffDD);
} // CalculateRWDD10nm

/**

  This function calculates the write to read turnaround time for different ranks on the same DIMM (10nm version)

  @param[in]  Host            - Pointer to sysHost, the system Host (root) structure
  @param[in]  Socket          - CPU Socket Node number (Socket ID)
  @param[in]  Channel         - Channel number (0-based)
  @param[in]  BusTimingData   - Pointer to the structure for timing variables
  @param[out] Twrdr           - Pointer to the return value for Twrdr

  @retval None
**/
VOID
CalculateWRDR10nm (
  IN  PSYSHOST         Host,
  IN  UINT8            Socket,
  IN  UINT8            Channel,
  IN  BUS_TIMING_DATA  *BusTimingData,
  OUT UINT32           *Twrdr
  )
{
  UINT8  OdtStretch = 0;
  struct channelNvram (*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // Rtt change skew
  //
  *Twrdr = 1;

  //
  // Get ODT stretch
  //
  if (CheckOdtAsserted (Socket, Channel, DiffRank)) {
    OdtStretch = CalculateOdtStretch (Socket, Channel, BusTimingData, WR_RD);
    *Twrdr += OdtStretch;
  }

  *Twrdr += (*BusTimingData).clkDiffDR;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "OdtStretch = %d, clkDiffDR = %d\n", OdtStretch, (*BusTimingData).clkDiffDR);
} // CalculateWRDR10nm

/**

  This function calculates the write to read turnaround time for different DIMMs (10nm version)

  @param[in]  Host             - Pointer to sysHost, the system Host (root) structure
  @param[in]  Socket           - CPU Socket Node number (Socket ID)
  @param[in]  Channel          - Channel number (0-based)
  @param[in]  *BusTimingData   - Pointer to the structure for timing variables
  @param[out] *Twrdd           - Pointer to the return value for Twrdd

  @retval None

**/
VOID
CalculateWRDD10nm (
  IN  PSYSHOST         Host,
  IN  UINT8            Socket,
  IN  UINT8            Channel,
  IN  BUS_TIMING_DATA  *BusTimingData,
  OUT UINT8            *Twrdd
  )
{
  UINT8               OdtStretch = 0;
  struct channelNvram (*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // Rtt change skew
  //
  *Twrdd = 1;

  //
  // Get ODT stretch
  //
  if (CheckOdtAsserted (Socket, Channel, DiffDimm)) {
    OdtStretch = CalculateOdtStretch (Socket, Channel, BusTimingData, WR_RD);
    *Twrdd += OdtStretch;
  }

  *Twrdd += (*BusTimingData).clkDiffDD;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "OdtStretch = %d, clkDiffDD = %d\n", OdtStretch, (*BusTimingData).clkDiffDD);
} // CalculateWRDD10nm

/**

  This function calculates the DDR4 read to DDRT write turnaround time for different DIMMs (10nm version)

  @param[in] Host           - Point to sysHost
  @param[in] Socket         - Socket number
  @param[in] Channel        - Channel number
  @param[in] DDRTCWL        - DDRT CAS Write Latency
  @param[in] BusTimingData  - Structure that holds the variables

  @retval DDRTRDWR channel value

**/
UINT8
CalctDdr4toDdrtRWDD10nm (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Channel,
  IN UINT8            DDRTCWL,
  IN BUS_TIMING_DATA  *BusTimingData
  )
{
  UINT8  DDRTRDWR = 0;
  UINT8  Tcl;
  UINT32 Trwdd = 0;
  UINT8  ReadBurstLenDclk;
  UINT8  TclMinusTcwl;
  UINT8  PreambleCheck;

  //
  // Calculate RWDD turnaround timing
  //
  CalculateRWDD10nm (Host, Socket, Channel, BusTimingData, &Trwdd);

  // Apply 10nm offset to RWDD turnaround (TA)
  //
  // t_rwdd  |  ICX = TA + CL - CWL + BL + Preamble
  //
  Tcl = (UINT8) GettCL (Host, Socket, Channel);
  ReadBurstLenDclk = GetReadBurstLength () / QCLK_PER_DCLK;
  PreambleCheck = (GetPreambleState (Socket, WRITE_PREAMBLE) == PREAMBLE_2TCLK ? 2 : 1);
  TclMinusTcwl = (Tcl > DDRTCWL) ? (Tcl - DDRTCWL) : 0;

  DDRTRDWR = (UINT8) Trwdd + TclMinusTcwl + ReadBurstLenDclk + PreambleCheck;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "RWDD (DDR4 to DDRT)[%d] = MAX(tCLDDR4[%d] - tCWLDDRT[%d], 0) + Trwdd[%d] + BL[%d] + PreambleCheck[%d]\n",
                          DDRTRDWR, Tcl, DDRTCWL, Trwdd, ReadBurstLenDclk, PreambleCheck);

  return DDRTRDWR;
} // CalctDdr4toDdrtRWDD10nm

/**

  This function calculates the DDR4 write to DDRT read turnaround time for different DIMMs (10nm version)

  @param[in] Host           - Point to sysHost
  @param[in] Socket         - Socket number
  @param[in] Channel        - Channel number
  @param[in] DDRTCL         - DDRT CAS Latency
  @param[in] BusTimingData  - Structure that holds the variables

  @retval DDRTWRRD channel value

**/
UINT8
CalctDdr4toDdrtWRDD10nm (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Channel,
  IN UINT8            DDRTCL,
  IN BUS_TIMING_DATA  *BusTimingData
  )
{
  UINT8  DDRTWRRD;
  UINT8  Tcwl;
  UINT8  Twrdd = 0;
  UINT8  WriteBurstLenDclk;
  UINT8  TcwlMinusTcl;
  UINT8  PreambleCheck;

  //
  // Calculate WRDD turnaround timing
  //
  CalculateWRDD10nm (Host, Socket, Channel, BusTimingData, &Twrdd);

  // Apply 10nm offset to WRDD turnaround (TA)
  //
  // t_wrdd  |  ICX = TA + CWL - CL + BL + Preamble
  //
  Tcwl = (UINT8) GettCWL (Host, Socket, Channel);
  WriteBurstLenDclk = GetWriteBurstLength (Channel) / QCLK_PER_DCLK;
  PreambleCheck = (GetPreambleState (Socket, READ_PREAMBLE) == PREAMBLE_2TCLK ? 2 : 1);
  TcwlMinusTcl = (Tcwl > DDRTCL) ? (Tcwl - DDRTCL) : 0;

  DDRTWRRD = Twrdd + TcwlMinusTcl + WriteBurstLenDclk + PreambleCheck;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "WRDD (DDR4 to DDRT)[%d] = MAX(tCWLDDR4[%d] - tCLDDRT[%d], 0) + Twrdd[%d] + BL[%d] + PreambleCheck[%d]\n",
                          DDRTWRRD, Tcwl, DDRTCL, Twrdd, WriteBurstLenDclk, PreambleCheck);

  return DDRTWRRD;
} // CalctDdr4toDdrtWRDD10nm

/**

  DDR4-TO-DDRT
  This function calculates the read to read turnaround time for different DIMMs (10nm version)

  @param[in] Host           - Point to sysHost
  @param[in] Socket         - Socket number
  @param[in] Ch             - Channel number
  @param[in] BusTimingData  - Structure that holds the variables

  @retval t_DDRT_GNTGNT_DD channel value

**/
UINT16
CalctDdr4toDdrtRRDD10nm (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Ch,
  IN BUS_TIMING_DATA  *BusTimingData
  )
{
  UINT8   Cas2CmplLate;
  UINT8   Gnt2Cmpl;
  UINT8   MaxRoundTrip;
  UINT16  McDelay;
  UINT16  TddrtGntGntDd = 0;
  UINT16  Trrdd;
  UINT16  ReadBurstLenDclk;
  UINT16  PreambleCheck;
  UINT16  Offset;

  // Calculate the ceiling value.
  // Magic numbers:
  // -13: The number of internal cycles prior to Roundtrip Latency that RdValid is asserted
  // + 8: The delay to shift through programmable Rd Delay in RRD fub
  // + 1: The delay due to the flop from DP to 2LMCntl
  MaxRoundTrip = CalculateMaxRoundTrip (Host, Socket, Ch, BusTimingData);
  Cas2CmplLate = MaxRoundTrip - 13 + 8 + 1;
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Cas2CmplLate[%d] = MaxRT[%d] - RdValidAsserted[13] + RdDelay[8] + FlopCompletion[1]\n",
                  Cas2CmplLate, MaxRoundTrip);

  // Calculate the memory controller's internal data path constraint.
  // Magic numbers:
  // + 1: Ensures there is no conflict in switching over from DDR4 to DDRT (due to the scheduler)
  //  1: The minimum spec
  Gnt2Cmpl = CalculateGnt2Cmpl (Host, Socket, Ch, BusTimingData->Gnt2Erid);
  McDelay = (UINT32) MAX (Cas2CmplLate - Gnt2Cmpl + 1, 1);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "McDelay[%d] = MAX(Cas2CmplLate[%d] - Gnt2Cmpl[%d] + Ddr4ToDdrtScheduler[1], 1)\n",
                  McDelay, Cas2CmplLate, Gnt2Cmpl);

  // Apply 10nm offset to RRDD turnaround (TA)
  //
  // t_rrdd  |  ICX = TA + BL + Preamble
  //
  Trrdd = CalculateRRDD10nm (Socket, Ch, BusTimingData);
  ReadBurstLenDclk = GetReadBurstLength () / QCLK_PER_DCLK;
  PreambleCheck = (GetPreambleState (Socket, READ_PREAMBLE) == PREAMBLE_2TCLK ? 2 : 1);
  Offset = ReadBurstLenDclk + PreambleCheck;

  Trrdd += Offset;

  TddrtGntGntDd = MAX (Trrdd, McDelay);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "RRDD (DDR4 to DDRT)[%d] = MAX(tRRDD[%d], McDelay[%d])\n",
                  TddrtGntGntDd, Trrdd, McDelay);

  return TddrtGntGntDd;
} // CalctDdr4toDdrtRRDD10nm

/**

  DDRT-TO-DDR4
  This function calculates the read to read turnaround time for different DIMMs (10nm version)

  @param[in] Host           - Point to sysHost
  @param[in] Socket         - Socket number
  @param[in] Ch             - Channel number
  @param[in] BusTimingData  - Structure that holds the variables

  @retval DDRTRD_to_DDR4RD channel value

**/
UINT8
CalctDdrttoDdr4RRDD10nm (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Ch,
  IN BUS_TIMING_DATA  *BusTimingData
  )
{
  UINT8  Cas2CmplEarly;
  UINT8  DdrtRdToDdr4Rd = 0;
  UINT8  Gnt2Cmpl;
  UINT8  MaxRoundTrip;
  UINT8  McDelay;
  UINT8  Trrdd;
  UINT8  ReadBurstLenDclk;
  UINT8  PreambleCheck;
  UINT8  Offset;

  // Calculate the ceiling value.
  // Magic numbers:
  // -13: The number of internal cycles prior to Roundtrip Latency that RdValid is asserted
  // + 1: The delay to shift through programmable Rd Delay in RRD fub
  // + 1: The delay due to the flop from DP to 2LMCntl
  MaxRoundTrip = CalculateMaxRoundTrip (Host, Socket, Ch, BusTimingData);
  Cas2CmplEarly = MaxRoundTrip - 13 + 1 + 1;
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Cas2CmplEarly[%d] = MaxRT[%d] - RdValidAsserted[13] + RdDelay[1] + FlopCompletion[1]\n",
                  Cas2CmplEarly, MaxRoundTrip);

  // Calculate the memory controller's internal data path constraint.
  // Magic numbers:
  // + 1: Ensures there is no conflict in switching over from DDR4 to DDRT (due to the scheduler)
  //  1: The minimum spec
  Gnt2Cmpl = CalculateGnt2Cmpl (Host, Socket, Ch, BusTimingData->Gnt2Erid);
  McDelay = (UINT32) MAX (Gnt2Cmpl - Cas2CmplEarly + 1, 1);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "McDelay[%d] = MAX(Gnt2Cmpl[%d] - Cas2CmplEarly[%d] + Ddr4ToDdrtScheduler[1], 1)\n",
                  McDelay, Gnt2Cmpl, Cas2CmplEarly);


  // Apply 10nm offset to RRDD turnaround (TA)
  //
  // t_rrdd  |  ICX = TA + BL + Preamble
  //
  Trrdd = CalculateRRDD10nm (Socket, Ch, BusTimingData);
  ReadBurstLenDclk = GetReadBurstLength () / QCLK_PER_DCLK;
  PreambleCheck = (GetPreambleState (Socket, READ_PREAMBLE) == PREAMBLE_2TCLK ? 2 : 1);
  Offset = ReadBurstLenDclk + PreambleCheck;
  Trrdd += Offset;

  //
  // + 4: DDR4 safe logic blocking adder required for command blocking
  //
  DdrtRdToDdr4Rd = MAX (Trrdd, McDelay) + 4;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "RRDD (DDRT to DDR4)[%d] = MAX(tRRDD[%d], McDelay[%d]) + SafeBlockingLogic[4]\n",
                  DdrtRdToDdr4Rd, Trrdd, McDelay);

  return DdrtRdToDdr4Rd;
} // CalctDdrttoDdr4RRDD10nm

/**

  This function calculates the DDRT GNT to DDR4 write turnaround time for different DIMMs (10nm version)

  @param[in] Host           - Point to sysHost
  @param[in] Socket         - Socket number
  @param[in] Channel        - Channel number
  @param[in] DDRTCL         - DDRT CAS Latency
  @param[in] BusTimingData  - Structure that holds the variables

  @retval DDRTRDWR channel value

**/
UINT8
CalctDdrttoDdr4RWDD10nm (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Channel,
  IN UINT8            DDRTCL,
  IN BUS_TIMING_DATA  *BusTimingData
  )
{
  UINT8  DDRTRDWR = 0;
  UINT8  TCwlValue;
  UINT32 Trwdd = 0;
  UINT8  ReadBurstLenDclk;
  UINT8  PreambleCheck;
  UINT8  TclMinusTcwl;

  //
  // Calculate RWDD turnaround timing
  //
  CalculateRWDD10nm (Host, Socket, Channel, BusTimingData, &Trwdd);

  // Apply 10nm offset to RWDD turnaround (TA)
  //
  // t_rwdd  |  ICX = TA + CL - CWL + BL + Preamble
  //
  TCwlValue   = (UINT8) GettCWL (Host, Socket, Channel);
  ReadBurstLenDclk = GetReadBurstLength () / QCLK_PER_DCLK;
  PreambleCheck = (GetPreambleState (Socket, WRITE_PREAMBLE) == PREAMBLE_2TCLK ? 2 : 1);
  TclMinusTcwl = (DDRTCL > TCwlValue) ? (DDRTCL - TCwlValue) : 0;

  DDRTRDWR = TclMinusTcwl + (UINT8) Trwdd + ReadBurstLenDclk + PreambleCheck;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "RWDD (DDRT to DDR4)[%d] = MAX(tCLDDRT[%d] - tCWLDDR4[%d], 0) + Trwdd[%d] + BL[%d] + PreambleCheck[%d]\n",
                          DDRTRDWR, DDRTCL, TCwlValue, Trwdd, ReadBurstLenDclk, PreambleCheck);

  return DDRTRDWR;
} // CalctDdrttoDdr4RWDD10nm

/**

  This function calculates the DDRT write to DDR4 read turnaround time for different DIMMs (10nm version)

  @param[in] Host           - Point to sysHost
  @param[in] Socket         - Socket number
  @param[in] Channel        - Channel number
  @param[in] DDRTCWL        - DDRT CAS Write Latency
  @param[in] BusTimingData  - Structure that holds the variables

  @retval DDRTWRRD channel value

**/
UINT8
CalctDdrttoDdr4WRDD10nm (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Channel,
  IN UINT8            DDRTCWL,
  IN BUS_TIMING_DATA  *BusTimingData
  )
{
  UINT8  DDRTWRRD = 0;
  UINT8  Tcl;
  UINT8  Twrdd = 0;
  UINT8  WriteBurstLenDclk;
  UINT8  PreambleCheck;
  UINT8  TcwlMinusTcl;

  Host = (PSYSHOST) GetSysHostPointer ();

  //
  // Calculate WRDD turnaround timing
  //
  CalculateWRDD10nm (Host, Socket, Channel, BusTimingData, &Twrdd);

  // Apply 10nm offset to WRDD turnaround (TA)
  //
  // t_wrdd  |  ICX = TA + CWL - CL + BL + Preamble
  //
  Tcl = (UINT8) GettCL (Host, Socket, Channel);
  WriteBurstLenDclk = GetWriteBurstLength (Channel) / QCLK_PER_DCLK;
  PreambleCheck = (GetPreambleState (Socket, READ_PREAMBLE) == PREAMBLE_2TCLK ? 2 : 1);
  TcwlMinusTcl = (DDRTCWL > Tcl) ? (DDRTCWL - Tcl) : 0;

  DDRTWRRD = TcwlMinusTcl + Twrdd + WriteBurstLenDclk + PreambleCheck;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "WRDD (DDRT to DDR4)[%d] = MAX(tCWLDDRT[%d] - tCLDDR4[%d], 0) + Twrdd[%d] + BL[%d] + PreambleCheck[%d]\n",
                          DDRTWRRD, DDRTCWL, Tcl, Twrdd, WriteBurstLenDclk, PreambleCheck);

  return DDRTWRRD;
} // CalctDdrttoDdr4WRDD10nm

/**

  This function calculates the DDRT read to DDRT write turnaround time for same DIMM (10nm version)

  @param[in] Host           - Point to sysHost
  @param[in] Socket         - Socket number
  @param[in] Channel        - Channel number
  @param[in] DDRTCL         - DDRT CAS Latency
  @param[in] DDRTCWL        - DDRT CAS Write Latency
  @param[in] BusTimingData  - Structure that holds the variables

  @retval DdrtRWSR channel value

**/
UINT8
CalctDdrttoDdrtRWSR10nm (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Channel,
  IN UINT8            DDRTCL,
  IN UINT8            DDRTCWL,
  IN BUS_TIMING_DATA  *BusTimingData
  )
{
  UINT8  DDRTRDWR = 0;
  UINT32 Trwsr = 0;
  UINT8  TclMinusTcwl;
  UINT8  ReadBurstLenDclk;
  UINT8  PreambleCheck;

  //
  // Calculate RWSR turnaround timing
  //
  CalculateRWSR10nm (Host, Socket, Channel, BusTimingData, &Trwsr);

  //
  // t_rwsg/t_rwsr  |  ICX = TA + CL - CWL + BL + Preamble
  //
  ReadBurstLenDclk = GetReadBurstLength () / QCLK_PER_DCLK;
  PreambleCheck = (GetPreambleState (Socket, WRITE_PREAMBLE) == PREAMBLE_2TCLK ? 2 : 1);
  TclMinusTcwl = (DDRTCL > DDRTCWL) ? (DDRTCL - DDRTCWL) : 0;

  DDRTRDWR = (UINT8) Trwsr + TclMinusTcwl + ReadBurstLenDclk + PreambleCheck;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "RWSR (DDRT to DDRT)[%d] = MAX(tCLDDRT[%d] - tCWLDDRT[%d], 0) + tRWSR[%d] + BL[%d] + PreambleCheck[%d]\n",
                          DDRTRDWR, DDRTCL, DDRTCWL, Trwsr, ReadBurstLenDclk, PreambleCheck);

  return DDRTRDWR;
} // CalctDdrttoDdrtRWSR10nm

/**

  This function calculates the DDRT write to DDRT read turnaround time for same rank (10nm version)

  @param[in] Host           - Point to sysHost
  @param[in] Socket         - Socket number
  @param[in] Channel        - Channel number
  @param[in] DDRTCL         - DDRT CAS Latency
  @param[in] DDRTCWL        - DDRT CAS Write Latency

  @retval DdrtWRSR channel value

**/
UINT8
CalctDdrttoDdrtWRSR10nm (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Channel,
  IN UINT8            DdrtCl,
  IN UINT8            DdrtCwl
  )
{
  UINT8               DDRTWRSR = 0;
  UINT8               Dimm;
  UINT8               Strobe;
  UINT8               WriteBurstLenDclk;
  UINT8               MSVx8;
  INT16               TxCycle;
  INT16               RecEnCycle;
  INT16               Max_Trained_Delay = 0;
  INT16               Trained_Delay[MAX_STROBE];
  INT32               t_ddrt_wrgnt_s_temp;
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];

  DimmNvList = GetDimmNvList (Host, Socket, Channel);
  MSVx8 = GetMaxStrobeValid (Host) / 2;
  WriteBurstLenDclk = GetWriteBurstLength (Channel) / QCLK_PER_DCLK;

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*DimmNvList)[Dimm].dimmPresent == 1) {
      if ((*DimmNvList)[Dimm].DcpmmPresent == 1) {
        RankList = GetRankNvList(Host, Socket, Channel, Dimm);
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

  Max_Trained_Delay = MAX (Max_Trained_Delay, 0);

  //
  // Magic number: 500 is for round up
  //
  t_ddrt_wrgnt_s_temp = 500 + (WriteBurstLenDclk * 1000) + MAX ((DdrtCwl * 1000 - DdrtCl * 1000) , 0) + ((INT16)(Max_Trained_Delay) * 1000 / 64);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "t_ddrt_wrgnt_s_temp = %d, CWLTemp= %d, CASTemp =%d, Max_Trained_Delay = %d \n", t_ddrt_wrgnt_s_temp, DdrtCwl, DdrtCl, Max_Trained_Delay);

  t_ddrt_wrgnt_s_temp = MAX ((t_ddrt_wrgnt_s_temp / 1000 - 1), 0);
  DDRTWRSR = (UINT8) t_ddrt_wrgnt_s_temp;

  //
  // Compare with specmin: BL + preamble + RTT Change Skew
  //
  DDRTWRSR = MAX (DDRTWRSR, WriteBurstLenDclk + 1 + 1);

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "PMem t_WRGNT_S = %d \n", DDRTWRSR);

  return DDRTWRSR;
} // CalctDdrttoDdrtWRSR10nm

#ifdef DDR5_SUPPORT
/**

/**

  Check and do workaround if applicable, to resolve Rank Switch Fix issue

  @param[in] Socket  - Socket number

  @retval EFI_SUCCESS

**/
MRC_STATUS
EFIAPI
DcpmmTurnaroundEquationCheck (
  IN UINT8  Socket
  )
{
  MRC_STATUS Status = MRC_FUNCTION_NOT_IMPLEMENTED;

 return Status;

}

#endif // DDR5_SUPPORT
