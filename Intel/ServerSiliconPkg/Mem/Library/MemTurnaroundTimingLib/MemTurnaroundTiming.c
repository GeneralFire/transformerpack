/** @file
  Implementation of the MemTurnaroundTimingLib.

  This library exposes the Memory Turnaround Timing functions for use.

  This implementation exposes functions used in setting up and calculating turnaround
  times for the memory controller.

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
#include <Library/MemoryCoreLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Memory/JedecDefinitions.h>
#include <Library/MemTurnaroundTimingLib.h>
#include "Include/MemTurnaroundTimingLibInternal.h"
#include <Library/RcDebugLib.h>
#include <Library/MemRcLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/HbmCoreLib.h>
#include <Library/MemTurnaroundTimingLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/SpdDecodeLib.h>

/**

  Initializes a turn around table by setting all values to 0 and setting the timing
  type to the appropriate value.

  @param[in] TurnaroundTable  - Table to be initialized
  @param[in] ArraySize        - Size of array to initialize

  @retval none

**/
VOID
InitTurnaroundTable (
  IN UINT32  TurnaroundTable[],
  IN UINT32  ArraySize
  )
{
  UINT8  TableIndex;

  // Initialize all entries to 0
  for (TableIndex = 0; TableIndex < ArraySize; TableIndex++) {
    TurnaroundTable[TableIndex] = 0;
  }
}

/**

  Display turnaround times

  @param[in] MemTechType   - Memory Technology Type
  @param[in] Socket        - Socket number
  @param[in] Ch            - Channel number

  @retval None

**/
VOID
EFIAPI
DisplayTurnaroundTimes (
  IN     MEM_TECH_TYPE  MemTechType,
  IN     UINT8          Socket,
  IN     UINT8          Ch
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT32  TurnaroundTimes[TurnaroundTypeMax];

  InitTurnaroundTable (TurnaroundTimes, TurnaroundTypeMax);

  GetSetTurnaroundSettings (MemTechType, Socket, Ch, GSM_READ_CSR, TurnaroundTimes);

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "DisplayTurnaroundTimes - turnaround timings\n");

  //
  // Same Bank Group, Same Rank, Same DIMM
  //

  RcDebugPrint (SDBG_MAX,
                 "       %a = %d\n       %a = %d\n       %a = %d\n       %a = %d\n",
                 "t_rrsg", TurnaroundTimes[t_RRSG],
                 "t_wwsg", TurnaroundTimes[t_WWSG],
                 "t_rwsg", TurnaroundTimes[t_RWSG],
                 "t_wrsg", TurnaroundTimes[t_WRSG]);

  //
  // Same Rank, Different Bank Group, Same DIMM
  //
  RcDebugPrint (SDBG_MAX,
                 "       %a = %d\n       %a = %d\n       %a = %d\n       %a = %d\n",
                 "t_rrsr", TurnaroundTimes[t_RRSR],
                 "t_wwsr", TurnaroundTimes[t_WWSR],
                 "t_rwsr", TurnaroundTimes[t_RWSR],
                 "t_wrsr", TurnaroundTimes[t_WRSR]);

  //
  // Different Rank, Same DIMM
  //
  RcDebugPrint (SDBG_MAX,
                 "       %a = %d\n       %a = %d\n       %a = %d\n       %a = %d\n",
                 "t_rrdr", TurnaroundTimes[t_RRDR],
                 "t_wwdr", TurnaroundTimes[t_WWDR],
                 "t_rwdr", TurnaroundTimes[t_RWDR],
                 "t_wrdr", TurnaroundTimes[t_WRDR]);

  //
  // Different Subrank, Same DIMM
  //
  RcDebugPrint (SDBG_MAX,
                 "       %a = %d\n       %a = %d\n       %a = %d\n       %a = %d\n",
                 "t_rrds", TurnaroundTimes[t_RRDS],
                 "t_wwds", TurnaroundTimes[t_WWDS],
                 "t_rwds", TurnaroundTimes[t_RWDS],
                 "t_wrds", TurnaroundTimes[t_WRDS]);

  //
  // Different DIMM
  //
  RcDebugPrint (SDBG_MAX,
                 "       %a = %d\n       %a = %d\n       %a = %d\n       %a = %d\n",
                 "t_rrdd", TurnaroundTimes[t_RRDD],
                 "t_wwdd", TurnaroundTimes[t_WWDD],
                 "t_rwdd", TurnaroundTimes[t_RWDD],
                 "t_wrdd", TurnaroundTimes[t_WRDD]);

#endif // DEBUG_CODE_BLOCK
} // DisplayTurnaroundTimes

/**

  Display turnaround times for DCPMM

  @param[in] MemTechType   - Memory Technology Type
  @param[in] Socket        - Socket number
  @param[in] Channel       - Channel number

  @retval None

**/
VOID
DisplayTurnaroundTimesDcpmm (
  IN     MEM_TECH_TYPE  MemTechType,
  IN     UINT8          Socket,
  IN     UINT8          Channel
  )
{
#if defined(DEBUG_CODE_BLOCK) && defined(DDRT_SUPPORT)
  UINT32  DcpmmTurnaroundTimes[DcpmmTurnaroundTypeMax];
  UINT32  TurnaroundTimes[TurnaroundTypeMax];

  InitTurnaroundTable (DcpmmTurnaroundTimes, DcpmmTurnaroundTypeMax);
  InitTurnaroundTable (TurnaroundTimes, TurnaroundTypeMax);

  GetSetDcpmmTurnaroundSettings (Socket, Channel, GSM_READ_ONLY, DcpmmTurnaroundTimes);
  GetSetTurnaroundSettings (MemTechType, Socket, Channel, GSM_READ_CSR, TurnaroundTimes);

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "DisplayTurnaroundTimesPMem - turnaround timings\n");


  RcDebugPrint (SDBG_MAX,
                 "PMem RD\n       %a = %d\n       %a = %d\n       %a = %d\n       %a = %d\n",
                 "t_ddrt_rdrd_s ", DcpmmTurnaroundTimes[t_RDRD_S],
                 "t_ddrt_rdrd_dd", DcpmmTurnaroundTimes[t_RDRD_DD],
                 "t_ddrt_wrrd_s ", DcpmmTurnaroundTimes[t_WRRD_S],
                 "t_ddrt_wrrd_dd", DcpmmTurnaroundTimes[t_WRRD_DD]
                 );

  RcDebugPrint (SDBG_MAX,
                 "PMem WR\n       %a = %d\n       %a = %d\n       %a = %d\n       %a = %d\n",
                 "t_ddrt_rdwr_s ", DcpmmTurnaroundTimes[t_RDWR_S],
                 "t_ddrt_rdwr_dd", DcpmmTurnaroundTimes[t_RDWR_DD],
                 "t_ddrt_wrwr_s ", DcpmmTurnaroundTimes[t_WRWR_S],
                 "t_ddrt_wrwr_dd", DcpmmTurnaroundTimes[t_WRWR_DD]
                 );

  RcDebugPrint (SDBG_MAX,
                 "PMem GNT2RW\n       %a = %d\n       %a = %d\n       %a = %d\n       %a = %d\n",
                 "t_ddrt_gntrd_s ", DcpmmTurnaroundTimes[t_GNTRD_S],
                 "t_ddrt_gntrd_dd", DcpmmTurnaroundTimes[t_GNTRD_DD],
                 "t_ddrt_gntwr_s ", DcpmmTurnaroundTimes[t_GNTWR_S],
                 "t_ddrt_gntwr_dd", DcpmmTurnaroundTimes[t_GNTWR_DD]
                 );

  DisplayRWtoGNT (DcpmmTurnaroundTimes);

  RcDebugPrint (SDBG_MAX,
                "PMem GNT2GNT\n       %a = %d\n       %a = %d\n",
                "t_ddrt_gntgnt_s ", DcpmmTurnaroundTimes[t_GNTGNT_S],
                "t_ddrt_gntgnt_dd", DcpmmTurnaroundTimes[t_GNTGNT_DD]
                );

  RcDebugPrint (SDBG_MAX,
                "MEMORY TIMINGS CAS2CAS DD\n       %a = %d\n       %a = %d\n       %a = %d\n       %a = %d\n",
                "t_rrdd", TurnaroundTimes[t_RRDD],
                "t_wwdd", TurnaroundTimes[t_WWDD],
                "t_rwdd", TurnaroundTimes[t_RWDD],
                "t_wrdd", TurnaroundTimes[t_WRDD]
                );
#endif // DEBUG_CODE_BLOCK && DDRT_SUPPORT
}

/**

  Gets the timing variables needed to calculate turnaround times

  @param[in]  Socket              - Socket number
  @param[in]  Channel             - Channel number
  @param[out] BusTimingData       - Structure that holds the variables
  @param[out] DdrtTurnaroundFlag  - DDRT turnaround flag

  @retval N/A

**/
VOID
EFIAPI
InitBusTimingData (
  IN UINT8             Socket,
  IN UINT8             Channel,
  OUT BUS_TIMING_DATA  *BusTimingData,
  IN BOOLEAN           DdrtTurnaroundFlag
  )
{
  PSYSHOST    Host;
  INT32       EffectiveBusDelaySr;
  INT32       EffectiveBusDelayDr;
  INT32       EffectiveBusDelayDd;
  ODT_TIMING  OdtTiming;

  Host = (PSYSHOST) GetSysHostPointer ();

  //
  // Get the effective bus delays for same rank (SR), different rank same DIMM (DR) and different DIMM (DD)
  //
  EffectiveBusDelay (Host, Socket, Channel, &EffectiveBusDelaySr, &EffectiveBusDelayDr, &EffectiveBusDelayDd, DdrtTurnaroundFlag);
  (*BusTimingData).effBusDelaySR = EffectiveBusDelaySr;
  (*BusTimingData).effBusDelayDR = EffectiveBusDelayDr;
  (*BusTimingData).effBusDelayDD = EffectiveBusDelayDd;

  (*BusTimingData).cas2RecEnDR = Cas2RecEnGap (Host, Socket, Channel, DR_MODE, FALSE);
  (*BusTimingData).cas2RecEnDD = Cas2RecEnGap (Host, Socket, Channel, DD_MODE, TRUE);

  (*BusTimingData).cas2DrvEnDR = Cas2DrvEnGap (Host, Socket, Channel, DR_MODE, FALSE);
  (*BusTimingData).cas2DrvEnDD = Cas2DrvEnGap (Host, Socket, Channel, DD_MODE, TRUE);

  GetOdtTiming (Host, Socket, Channel, &OdtTiming);
  (*BusTimingData).odtRdLeading = OdtTiming.odtRdLeading;
  (*BusTimingData).odtRdTrailing = OdtTiming.odtRdTrailing;
  (*BusTimingData).odtWrLeading = OdtTiming.odtWrLeading;
  (*BusTimingData).odtWrTrailing = OdtTiming.odtWrTrailing;

  (*BusTimingData).clkDiffDR = CkDiffGap (Host, Socket, Channel, DR_MODE);
  (*BusTimingData).clkDiffDD = CkDiffGap (Host, Socket, Channel, DD_MODE);
}

/**

  Early turnaround times programed before training according to relax values given by TA doc.
  display at the end of training for final results.

  @param[in] Socket   - Socket number
  @param[in] Channel  - Channel number

  @retval EFI_SUCCESS   Turnaround CSRs read/write successful

**/
EFI_STATUS
EFIAPI
InitRelaxTurnaroundTimes (
  IN UINT8  Socket,
  IN UINT8  Channel
  )
{
  EFI_STATUS                       Status;
  UINT32                           TurnaroundSettings[TurnaroundTypeMax];
  CONST TURNAROUND_CONSTANTS   *TurnaroundTable = NULL;

  InitTurnaroundTable (TurnaroundSettings, TurnaroundTypeMax);
  GetRelaxedTurnaroundConstants (Socket, &TurnaroundTable);

  RelaxTurnaroundTimesSoc (Socket, Channel, TurnaroundTable, TurnaroundSettings);

  Status = GetSetTurnaroundSettings (MemTechDdr, Socket, Channel, GSM_FORCE_WRITE, TurnaroundSettings);

  DisplayTurnaroundTimes (MemTechDdr, Socket, Channel);

  return Status;
}

/**

  Early turnaround times for DCPMM programed before training according to relax values given by TA doc.
  Display at the end of training for final results.

  @param[in] Socket   - Socket number
  @param[in] Channel  - Channel number

  @retval None

**/
VOID
InitRelaxTurnAroundTimesDcpmm (
  IN UINT8 Socket,
  IN UINT8 Channel
  )
{
#ifdef DDRT_SUPPORT
  EFI_STATUS                        ReturnStatus = EFI_SUCCESS;
  UINT32                            DcpmmTurnaroundSettings[DcpmmTurnaroundTypeMax];
  CONST DCPMM_TURNAROUND_CONSTANTS  *DcpmmTurnaroundConstants = NULL;

  InitTurnaroundTable (DcpmmTurnaroundSettings, DcpmmTurnaroundTypeMax);
  GetDcpmmRelaxedTurnaroundConstants (&DcpmmTurnaroundConstants);
  if (DcpmmTurnaroundConstants == NULL) {
    ReturnStatus = EFI_INVALID_PARAMETER;
  }

  if (ReturnStatus == EFI_SUCCESS) {
    GetSetDcpmmTurnaroundSettings (Socket, Channel, GSM_READ_ONLY, DcpmmTurnaroundSettings);
    DcpmmTurnaroundSettings[t_RDRD_DD] = DcpmmTurnaroundConstants->RdRdDDConst;
    DcpmmTurnaroundSettings[t_WRRD_DD] = DcpmmTurnaroundConstants->WrRdDDConst;
    DcpmmTurnaroundSettings[t_RDRD_S] = DcpmmTurnaroundConstants->RdRdSCosnt;
    DcpmmTurnaroundSettings[t_WRRD_S] = DcpmmTurnaroundConstants->WrRdSConst;

    DcpmmTurnaroundSettings[t_RDWR_DD] = DcpmmTurnaroundConstants->RdWrDDConst;
    DcpmmTurnaroundSettings[t_RDWR_S] = DcpmmTurnaroundConstants->RdWrSConst;
    DcpmmTurnaroundSettings[t_WRWR_DD] = DcpmmTurnaroundConstants->WrWrDDConst;
    DcpmmTurnaroundSettings[t_WRWR_S] = DcpmmTurnaroundConstants->WrWrSConst;

    DcpmmTurnaroundSettings[t_GNTRD_DD] = DcpmmTurnaroundConstants->GntRdDDConst;
    DcpmmTurnaroundSettings[t_GNTRD_S] = DcpmmTurnaroundConstants->GntRdSConst;
    DcpmmTurnaroundSettings[t_GNTWR_DD] = DcpmmTurnaroundConstants->GntWrDDConst;
    DcpmmTurnaroundSettings[t_GNTWR_S] = DcpmmTurnaroundConstants->GntWrSConst;

    DcpmmTurnaroundSettings[t_RDGNT_DD] = DcpmmTurnaroundConstants->RdGntDDConst;
    DcpmmTurnaroundSettings[t_RDGNT_S] = DcpmmTurnaroundConstants->RdGntSConst;
    DcpmmTurnaroundSettings[t_WRGNT_DD] = DcpmmTurnaroundConstants->WrGntDDConst;
    DcpmmTurnaroundSettings[t_WRGNT_S] = DcpmmTurnaroundConstants->WrGntSConst;

    DcpmmTurnaroundSettings[t_GNTGNT_DD] = DcpmmTurnaroundConstants->GntGntDDConst;
    DcpmmTurnaroundSettings[t_GNTGNT_S] = DcpmmTurnaroundConstants->GntGntSConst;

    GetSetDcpmmTurnaroundSettings (Socket, Channel, GSM_FORCE_WRITE, DcpmmTurnaroundSettings);
  }

  DisplayTurnaroundTimesDcpmm (MemTechFmc, Socket, Channel);
#endif DDRT_SUPPORT
}

/**

  Programs turnaround times after training.

  @param[in] Socket  - Socket number

  @retval EFI_SUCCESS   Turnaround CSRs read/write successful

**/
EFI_STATUS
EFIAPI
OptimizeTurnaroundTimings (
  IN UINT8  Socket
  )
{
  EFI_STATUS       Status;
  UINT8            Ch;
  UINT32           TurnaroundSettings[TurnaroundTypeMax];
  UINT8            MaxChDdr;

  Status = EFI_SUCCESS;
  MaxChDdr = GetMaxChDdr ();

  InitTurnaroundTable (TurnaroundSettings, TurnaroundTypeMax);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    Status = GetSetTurnaroundSettings (MemTechDdr, Socket, Ch, GSM_READ_CSR, TurnaroundSettings);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    OptimizeTurnaroundTimingsSoc (Socket, Ch, TurnaroundSettings);

    Status = GetSetTurnaroundSettings (MemTechDdr, Socket, Ch, GSM_FORCE_WRITE, TurnaroundSettings);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    DisplayTurnaroundTimes (MemTechDdr, Socket, Ch);

  } // Ch loop

  return Status;
}

/**

  Override turnaround times for select environments

  @param[in] Socket - Socket number

  @retval EFI_SUCCESS - Turnaround CSRs read/write successful

**/
EFI_STATUS
EFIAPI
OverrideTurnaroundTimes (
  IN UINT8 Socket
  )
{
  struct channelNvram       (*ChannelNvList)[MAX_CH];
  PSYSHOST                  Host;
  UINT8                     Ch;
  UINT32                    PerfTableSettings[TurnaroundTypeMax];
  UINT8                     MaxChDdr;

  //
  // Override calculated values with perf table values if FC emulation is enabled
  //
  if (!UbiosGenerationEnabled ()) {
    return EFI_SUCCESS;
  }

  RcDebugPrint (SDBG_MAX, "     Override cached values with Perf Table values - Socket %d\n", Socket);

  Host = (PSYSHOST) GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  InitTurnaroundTable (PerfTableSettings, TurnaroundTypeMax);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

#ifdef DDRT_SUPPORT
    //
    // Don't need to do comparsion for DDRT only channel since the timing are not applicable
    //
    if (((*ChannelNvList)[Ch].ddrtEnabled) && ((*ChannelNvList)[Ch].maxDimm == 1)) {
      continue;
    }
#endif

    GetSetTurnaroundSettings (MemTechDdr, Socket, Ch, GSM_READ_CSR, PerfTableSettings);

    if (EFI_SUCCESS != GetPerfTableTaValuesPerCh (Socket, Ch, PerfTableSettings)) {
      RcDebugPrint (SDBG_DEFAULT, "Unable to obtain DDR performance timimgs\n");
      return EFI_UNSUPPORTED;
    }

    GetSetTurnaroundSettings (MemTechDdr, Socket, Ch, GSM_FORCE_WRITE, PerfTableSettings);

    DisplayTurnaroundTimes (MemTechDdr, Socket, Ch);
  }

  return EFI_SUCCESS;
}

/**

  Retrieve timing string to be displayed.

  @param[in]  TurnaroundType  - turnaround type to be labeled
  @param[in]  TimingString    - pointer to string

  @retval non

**/
EFI_STATUS
GetTimingString (
  IN TURNAROUND_TYPE  TurnaroundType,
  IN CHAR8            **TimingString
  )
{
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;

  switch (TurnaroundType) {
    case t_RRSG:
      *TimingString = "t_rrsg";
      break;
    case t_WWSG:
      *TimingString = "t_wwsg";
      break;
    case t_RWSG:
      *TimingString = "t_rwsg";
      break;
    case t_WRSG:
      *TimingString = "t_wrsg";
      break;
    case t_RRSR:
      *TimingString = "t_rrsr";
      break;
    case t_WWSR:
      *TimingString = "t_wwsr";
      break;
    case t_RWSR:
      *TimingString = "t_rwsr";
      break;
    case t_WRSR:
      *TimingString = "t_wrsr";
      break;
    case t_RRDR:
      *TimingString = "t_rrdr";
      break;
    case t_WWDR:
      *TimingString = "t_wwdr";
      break;
    case t_RWDR:
      *TimingString = "t_rwdr";
      break;
    case t_WRDR:
      *TimingString = "t_wrdr";
      break;
    case t_RRDD:
      *TimingString = "t_rrdd";
      break;
    case t_WWDD:
      *TimingString = "t_wwdd";
      break;
    case t_RWDD:
      *TimingString = "t_rwdd";
      break;
    case t_WRDD:
      *TimingString = "t_wrdd";
      break;
    case t_RRDS:
      *TimingString = "t_rrds";
      break;
    case t_WWDS:
      *TimingString = "t_wwds";
      break;
    case t_RWDS:
      *TimingString = "t_rwds";
      break;
    case t_WRDS:
      *TimingString = "t_wrds";
      break;
    case t_RRDLR:
      *TimingString = "t_rrdlr";
      break;
    default:
      Status = EFI_INVALID_PARAMETER;
  }

  return Status;
}

/**

  Compare the calculated value that is stored in cache with the Perf table turnaround values.
  If a mismatch is detected, the turnaround mismatch is displayed.

  @param[in] Socket                - Socket number
  @param[in] Ch                    - Channel number
  @param[in] CalculatedSettings    - Turnaround Table values read from cache
  @param[in] PerfTableSettings     - Turnaround Table values calculated from from perf table

  @retval TRUE  - returns true if there are no differences found between tables
          FALSE - returns false if any differences are found between tables

**/
VOID
DetectPerfTableMismatch (
  IN UINT8   Socket,
  IN UINT8   Ch,
  IN UINT32  CalculatedSettings[],
  IN UINT32  PerfTableSettings[]
  )
{
  UINT8    TableIndex;
  CHAR8    *TimingString = "";
  UINT8    NumChPerMc;

  NumChPerMc = GetNumChannelPerMc ();

  // Loop through tables and display any mismatches -- ignore cases where the PerfTable setting is 0 as this means there is no
  // performance setting for this timing
  for (TableIndex = 0; TableIndex < TurnaroundTypeMax; TableIndex++) {
    if ((CalculatedSettings[TableIndex] != PerfTableSettings[TableIndex]) && (PerfTableSettings[TableIndex] != 0)) {
      //
      // Only report turnaround times that do not match
      //
      GetTimingString (TableIndex, &TimingString);
      RcDebugPrint (SDBG_MAX,
          "<====== PerfTable Turnaround Timing Compare - Socket %d - MC %d - Ch %d - Calculated values NOT equal to perf table! \n Turnaround Type: %a  - Calculated Value = %d  != Performance Table = %d \n\n",
          Socket,
          Ch / NumChPerMc,
          Ch % NumChPerMc,
          TimingString,
          CalculatedSettings[TableIndex],
          PerfTableSettings[TableIndex]
          );
    }
  }
}

/**

  Check turnaround times programmed after training against optimized values
  and display at the end of training for final results.

  @param[in] Socket  - Socket number

  @retval None

**/
VOID
EFIAPI
ComparePerformanceTurnaroundTimes (
  IN UINT8  Socket
  )
{
  struct channelNvram       (*ChannelNvList)[MAX_CH];
  PSYSHOST                  Host;
  UINT8                     Ch;
  UINT32                    PerfTableSettings[TurnaroundTypeMax];
  UINT32                    CalculatedSettings[TurnaroundTypeMax];
  UINT8                     MaxChDdr;

  Host = (PSYSHOST) GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  InitTurnaroundTable (PerfTableSettings, TurnaroundTypeMax);
  InitTurnaroundTable (CalculatedSettings, TurnaroundTypeMax);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

#ifdef DDRT_SUPPORT
    //
    // Don't need to do comparsion for DDRT only channel since the timing are not applicable
    //
    if (((*ChannelNvList)[Ch].ddrtEnabled) && ((*ChannelNvList)[Ch].maxDimm == 1)) {
      continue;
    }
#endif

    if (EFI_SUCCESS != GetPerfTableTaValuesPerCh (Socket, Ch, PerfTableSettings)) {
      RcDebugPrint (SDBG_DEFAULT, "Unable to obtain DDR performance timimgs\n");
      return;
    }

    GetSetTurnaroundSettings (MemTechDdr, Socket, Ch, GSM_READ_CSR, CalculatedSettings);

    //
    // Check if the calculated values that reside in cache are the same as PerfTable values.
    //
    DetectPerfTableMismatch (Socket, Ch, CalculatedSettings, PerfTableSettings);

    DisplayTurnaroundTimes (MemTechDdr, Socket, Ch);
  }
}

#ifdef LRDIMM_SUPPORT
/**

  This function calculates the minimum Rank to Rank timing requirement for DDR4DB02 in RDWR transactions

  @param[in] Host        - Point to sysHost
  @param[in] Socket      - Socket number
  @param[in] Channel     - Channel number

  @retval Minimum RWDR based on the spec.

**/
UINT8
DataBufferSpecMinRWDR (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel
  )
{
  UINT32               tPDM_RD;
  UINT32               tPDM_WR;
  UINT32               BurstLength;
  UINT32               tCK;
  UINT32               Tcl;
  UINT32               Tcwl;
  UINT32               TclMinusTcwl;
  UINT32               WrPreamble;
  UINT32               RdPreamble;
  UINT8                SpecMinRWDR;
  SYS_SETUP            *Setup;
  struct channelNvram  (*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);
  Setup = GetSysSetupPointer ();

  tCK = GettCK (Host, Socket);

  // tPDM_RD is the delay between first rising edge of MDQS and the first rising edge of DQS for a RD command
  tPDM_RD = (167000000 / tCK) + 2500;   // unit is in tCKs

  // tPDM_WR the delay between first rising edge of DQS and the first rising edge of MDQS for a WR command
  tPDM_WR = (167000000 / tCK) + 2500;   // unit is in tCKs

  // tWRPRE is minimum pulse width of write preamble
  WrPreamble = (GetPreambleState (Socket, WRITE_PREAMBLE) == PREAMBLE_2TCLK ? 2 : 1) * 10000;

  // tRPRE is the minimum pulse width of read preamble
  RdPreamble = (GetPreambleState (Socket, READ_PREAMBLE) == PREAMBLE_2TCLK ? 2 : 1) * 10000;

  BurstLength = (CalculateWrCrcAdder (Channel) * 20000) + 80000; // burst length is 8 or 10 (x10)

  Tcl = GettCL (Host, Socket, Channel);
  Tcwl = GettCWL (Host, Socket, Channel);
  TclMinusTcwl = (Tcl > Tcwl) ? (Tcl - Tcwl) : 0;

  // Rank to Rank timing requirement for DDR4DB02 in RDWR transactions
  // tPMD_RD + tPMD_WR + tWRPRE + tRPRE/2 + BL/2 + 2.7 + MAX((CL - CWL), 0)
  // tPMD_RD + tPMD_WR + tWRPRE + tRPRE/2 + BL/2 + 1.3 + MAX((CL - CWL), 0) for one rank timing mode
  // Note: 10000 is used to avoid floating point operations
  //        5000 is used for round up
  if ((*ChannelNvList)[Channel].chOneRankTimingModeEnable == ONE_RANK_TIMING_MODE_ENABLED) {
    SpecMinRWDR = (UINT8) ((5000 + tPDM_RD + tPDM_WR + WrPreamble + RdPreamble / 2 + (BurstLength / 2) + 13000 + TclMinusTcwl * 10000) / 10000);
  } else {
    SpecMinRWDR = (UINT8) ((5000 + tPDM_RD + tPDM_WR + WrPreamble + RdPreamble / 2 + (BurstLength / 2) + 27000 + TclMinusTcwl * 10000) / 10000);
  }

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "(x10000) tck = %d, tPDM_RD=%d, tPDM_WR=%d, WrPreamble=%d, RdPreamble=%d, BL=%d, TclMinusTcwl=%d, DB RWDR specMin=%d\n",
                  tCK, tPDM_RD, tPDM_WR, WrPreamble, RdPreamble, BurstLength, TclMinusTcwl, SpecMinRWDR);

  return SpecMinRWDR;
} //DataBufferSpecMinRWDR

/**

  This function calculates the minimum Rank to Rank timing requirement for DDR4DB02 in WRRD transactions

  @param[in] Host        - Point to sysHost
  @param[in] Socket      - Socket number
  @param[in] Channel     - Channel number

  @retval Minimum WRRD based on the spec.

**/
UINT8
DataBufferSpecMinWRDR (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel
  )
{
  UINT32               WriteBurstLenDclk;
  UINT32               Tcl;
  UINT32               Tcwl;
  UINT32               TcwlMinusTcl;
  UINT32               RdPreamble;
  UINT8                SpecMinWRDR;
  struct channelNvram  (*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);

  // tRPRE is the minimum pulse width of read preamble
  RdPreamble = (GetPreambleState (Socket, READ_PREAMBLE) == PREAMBLE_2TCLK ? 2 : 1) * 10;

  WriteBurstLenDclk = (GetWriteBurstLength (Channel) / QCLK_PER_DCLK) * 10;

  Tcl = GettCL (Host, Socket, Channel);
  Tcwl = GettCWL (Host, Socket, Channel);
  TcwlMinusTcl = (Tcwl > Tcl) ? (Tcwl - Tcl) : 0;

  // Rank to Rank timing requirement for DDR4DB02 in WRRD transactions
  // tRPRE/2 + BL/2 + 2.7 + MAX((CWL - CL), 0)
  // tRPRE/2 + BL/2 + 1.3 + MAX((CWL - CL), 0) for one rank timing mode
  // Note: 10 is used to avoid floating point operations
  //        5 is used for round up
  if ((*ChannelNvList)[Channel].chOneRankTimingModeEnable == ONE_RANK_TIMING_MODE_ENABLED) {
    SpecMinWRDR = (UINT8) ((5 + RdPreamble / 2 + WriteBurstLenDclk + 27 + TcwlMinusTcl * 10) / 10);
  } else {
    SpecMinWRDR = (UINT8) ((5 + RdPreamble / 2 + WriteBurstLenDclk + 13 + TcwlMinusTcl * 10) / 10);
  }

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "(x10) RdPreamble = %d, BurstLenDclk=%d, TcwlMinusTcl=%d, DB WRDR specMin=%d\n",
                  RdPreamble, WriteBurstLenDclk, TcwlMinusTcl, SpecMinWRDR);

  return SpecMinWRDR;
} //DataBufferSpecMinWRDR
#endif

/**

  Enforces WWDR settings

  @param[in]  Host  - Pointer to sysHost, the system Host (root) structure
  @param[in]  Ch    - Channel number (0-based)
  @param[out] tWWDR - Pointer to the resulting enforcement decision

  @retval None

**/
VOID
EnforceWWDR (
  IN  PSYSHOST  Host,
  IN  UINT8     Ch,
  OUT UINT32    *tWWDR
  )
{
  if (Host->var.mem.pxcEnabled & (1 << Ch)) {
    if (*tWWDR < ENFORCE_WWDR_PXC_EN) {
      *tWWDR = ENFORCE_WWDR_PXC_EN;
    }
  } else {
    if (*tWWDR < ENFORCE_WWDR_PXC_DIS) {
      *tWWDR = ENFORCE_WWDR_PXC_DIS;
    }
  }
}

/**

  Enforces WWDD settings

  @param[in]  Host  - Pointer to sysHost, the system Host (root) structure
  @param[in]  Ch    - Channel number (0-based)
  @param[out] tWWDD - Pointer to the resulting enforcement decision

  @retval None

**/
VOID
EnforceWWDD (
  IN  PSYSHOST  Host,
  IN  UINT8     Ch,
  OUT UINT32    *tWWDD
  )
{
  if (Host->var.mem.pxcEnabled & (1 << Ch)) {
    if (*tWWDD < ENFORCE_WWDD_PXC_EN) {
      *tWWDD = ENFORCE_WWDD_PXC_EN;
    }
  } else {
    if (*tWWDD < ENFORCE_WWDD_PXC_DIS) {
      *tWWDD = ENFORCE_WWDD_PXC_DIS;
    }
  }
}

/**
  Evaluates if the Early WrCRC feature is applicable to the current configuration

  @param[in] Channel - Channel number (0-based)

  @retval FALSE - Not applicable
          TRUE  - Applicable
**/
BOOLEAN
IsEarlyWrCrcRequired (
  IN UINT8 Channel
  )
{
  UINT8               Dimm;
  UINT8               Socket;
  SYS_SETUP           *Setup;
  PSYSHOST            Host;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];

  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;
  Setup = GetSysSetupPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // Search if the Early WrCRC is aplicable
  //
  if (IsSiliconWorkaroundEnabled ("S14011037991")) {
    if (((Setup->mem.optionsExt & EARLY_WR_CRC_DDRIO) != 0) || ((Setup->mem.optionsExt & WR_CRC) != 0)) {
      DimmNvList = GetDimmNvList (Host, Socket, Channel);

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList)[Dimm].dimmPresent == 0) {
          continue;
        }

        //
        // If Micron 8Gb E-die and no LRDIMM
        //
        if (((*DimmNvList)[Dimm].SPDDramMfgId == MFGID_MICRON) &&
            ((*DimmNvList)[Dimm].sdramCapacity == SPD_8Gb) &&
            ((*DimmNvList)[Dimm].SPDDramRev == MICRON_SPD_DRAMREV_E) &&
            (IsLrdimmPresent (Socket, Channel, Dimm) == 0)) {
          return TRUE;
        }
      } // Dimm loop
    }
  }

  return FALSE;
}

/**
  Calculate any additional delay due to WR_CRC settings (in Dclks).

  This is an adder to the adjusted burst length, as WR_CRC increases the default
  burst length from 8 to 10 Qclks (4 to 5 Dclks).

  @param[in] Channel - DDR channel number (0-based)

  @retval Any additional delay due to WR_CRC settings (in Dclks).
**/
UINT8
CalculateWrCrcAdder (
  IN UINT8 Channel
  )
{
  SYS_SETUP  *Setup;

  Setup = GetSysSetupPointer ();
  if (!IsEarlyWrCrcRequired (Channel) && ((Setup->mem.optionsExt & WR_CRC) == 0)) {
    return 0;
  } else {
    return 1;
  }
}

/**

  Calculate the maximum round trip latency (in Dclks).

  @param[in] Host           - Pointer to the SysHost structure
  @param[in] Socket         - Socket to calculate the delay for
  @param[in] Channel        - Channel to calculate the delay for
  @param[in] BusTimingData  - Structure that holds the turnaround timing variables

  @retval The maximum round trip latency (in Dclks).

**/
UINT8
CalculateMaxRoundTrip (
  IN  PSYSHOST         Host,
  IN  UINT8            Socket,
  IN  UINT8            Channel,
  IN  BUS_TIMING_DATA  *BusTimingData
  )
{
  UINT8             CurrentRoundTripLatency[MAX_CH];
  UINT8             Dimm;
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];
  UINT8             LogicalRank;
  UINT8             MaxRoundTripLatency[MAX_CH];
  UINT8             Rank;

  // Initialize channel-based array elements to 0.
  ZeroMem (CurrentRoundTripLatency, sizeof (CurrentRoundTripLatency));
  ZeroMem (MaxRoundTripLatency, sizeof (MaxRoundTripLatency));

  // Find the maximum roundtrip latency.
  DimmNvList = GetDimmNvList (Host, Socket, Channel);
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      if (CheckRank (Host, Socket, Channel, Dimm, Rank, CHECK_MAPOUT)) {
        continue;
      }
      LogicalRank = GetLogicalRank (Host, Socket, Channel, Dimm, Rank);
      GetRoundTrip (Socket, Channel, LogicalRank, GSM_READ_CSR, &CurrentRoundTripLatency[Channel]);
      if (MaxRoundTripLatency[Channel] < CurrentRoundTripLatency[Channel]) {
        MaxRoundTripLatency[Channel] = CurrentRoundTripLatency[Channel];
      }
    }
    if ((*DimmNvList)[Dimm].DcpmmPresent == 1) {
      GetSetSxpGnt2Erid (Host, Socket, Channel, Dimm, GSM_READ_ONLY, (INT16 *)&BusTimingData->Gnt2Erid);
    }
  }

  // Convert from Qclks to Dclks, rounding up.
  return (MaxRoundTripLatency[Channel] / QCLK_PER_DCLK) + (MaxRoundTripLatency[Channel] % QCLK_PER_DCLK);
}

/**

  Calculate the grant-to-completion delay

  @param[in] Host      - Pointer to the SysHost structure
  @param[in] Socket    - Socket to calculate the delay for
  @param[in] Channel   - Channel to calculate the delay for
  @param[in] Gnt2Erid  - Grant-to-early read ID delay (in Dclks)

  @retval The grant-to-completion delay (in Dclks)

**/
UINT8
CalculateGnt2Cmpl (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Gnt2Erid
  )
{
  UINT8 Gnt2Cmpl;

  // Magic numbers:
  // + 4: The number of cycles to accumulate the ERID values
  // + 1: The number of cycles to look up DDRTQ
  // + 1: The delay for DCLK to flop completion
  Gnt2Cmpl = Gnt2Erid + 4 + 1 + 1;
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Gnt2Cmpl[%d] = Gnt2Erid[%d] + ERID[4] + DDRTQ[1] + FlopCompletion[1]\n", Gnt2Cmpl, Gnt2Erid);

  return Gnt2Cmpl;
}
