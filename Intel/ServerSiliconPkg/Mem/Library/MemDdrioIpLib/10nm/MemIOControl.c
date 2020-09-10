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
#include <SysHostChip.h>
#include <Chip/Include/CpuPciAccess.h>
#include "Chip10nm/Include/MemHostChip10nm.h"
#include <Chip/Include/FnvAccessCommon.h>
#include <UncoreCommonIncludes.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/CheckpointLib.h>
#include <Library/BaseLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/SysHostPointerLib.h>

#include "Include/MemCmdControl.h"
#include "Include/MemIoControl.h"
#include "Include/MemPxc.h"
#include "Include/MemXoverCalib.h"
#include "Include/MemDdrioRegs.h"
#include "Include/MemDdrioPrivate.h"
#include <Library/EmulationConfigurationLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/CteNetLib.h>
#include <Library/MspChipLib.h>
#include <Library/MemDdrIoIpTargetLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/OdtTableLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemRoutingLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/SwizzleLib.h>
#include <Memory/JedecDefinitions.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Library/RcMemAllocLib.h>
#include <Library/MemRcLib.h>
#include <Library/PerformanceTrackerLib.h>

#include "Common/Include/MemDdrioIpLibDefinitions.h"
#ifdef DDR5_SUPPORT
#include <Library/Ddr5CoreLib.h>
#include <Memory/Ddr5MrRegs.h>
#include <Memory/Ddr5Cmds.h>
#endif // #ifdef DDR5_SUPPORT
#include <MemProjectSpecific.h>

#define ODT_SEGMENTS_EN_1 1
#define ODT_SEGMENTS_EN_2 2
#define MAX_DQ_GAP        17
//
// Internal data types
//

// Per-DIMM round-trip register offsets. Multiple per-DIMM arrays are used rather than a two-dimensional array or a
// single, flattened array in order to make bounds checking more deliberate and less error-prone.
CONST UINT32 mDimm0RoundTripRegOffsets[] = {
  DDRCRINTFROUNDTRIP0_CH0_MCIO_DDRIO_REG,
  DDRCRINTFROUNDTRIP0_CH1_MCIO_DDRIO_REG,
  DDRCRINTFROUNDTRIP0_CH2_MCIO_DDRIO_REG
};
CONST UINT32 mDimm1RoundTripRegOffsets[] = {
  DDRCRINTFROUNDTRIP1_CH0_MCIO_DDRIO_REG,
  DDRCRINTFROUNDTRIP1_CH1_MCIO_DDRIO_REG,
  DDRCRINTFROUNDTRIP1_CH2_MCIO_DDRIO_REG
};
#define DIMM_ROUND_TRIP_RANKS_PER_REG 4
#define DIMM_ROUND_TRIP_REGS          2
#define DIMM_ROUND_TRIP_RANKS         (DIMM_ROUND_TRIP_RANKS_PER_REG * DIMM_ROUND_TRIP_REGS)

//
// upper and lower desired range resistor value limits for rcomp static leg training
//
#define UPPER_RCOMP_RESISTOR_VALUE 53
#define LOWER_RCOMP_RESISTOR_VALUE 10

extern UINT32 ClkAddressConvertTable[MAX_CH];

//
// Local Prototypes
//
STATIC UINT32 ProgramIOCompValues(PSYSHOST Host, UINT8 socket);
STATIC MRC_STATUS GetSetTxDelayBit(PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, UINT8 strobe, UINT8 bit, MRC_LT level,
             MRC_GT group, UINT8 mode, INT16 *value);
STATIC MRC_STATUS GetSetTxVref (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, UINT8 strobe, UINT8 bit, MRC_LT level,
             MRC_GT group, UINT8 mode, INT16 *value);
STATIC MRC_STATUS GetSetImode (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 strobe, UINT8 mode, INT16 *value);
STATIC MRC_STATUS GetSetCTLE (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 strobe, UINT8 bit, MRC_LT level,
             MRC_GT group, UINT8 mode, INT16 *value);
STATIC MRC_STATUS GetSetCPUODT (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, UINT8 strobe, UINT8 bit, MRC_LT level,
             MRC_GT group, UINT8 mode, INT16 *value);
STATIC MRC_STATUS GetSetDIMMODT (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, UINT8 strobe, UINT8 bit, MRC_LT level,
             MRC_GT group, UINT8 mode, INT16 *value);
STATIC UINT32 MemWriteDimmVref(PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, UINT8 strobe, MRC_LT level, UINT8 vref);

STATIC MRC_STATUS GetSetPxcDdjcDelta (IN PSYSHOST Host, IN UINT8 Socket, IN UINT8 Ch, IN UINT8 Dimm, IN UINT8 Rank,
                               IN UINT8 Strobe, IN UINT8 Bit, IN MRC_GT Group, IN UINT8 Mode, IN OUT INT16 *Value);
STATIC UINT32 RcompStaticLegTraining (PSYSHOST Host);
VOID RxDfeCsrInit (IN PSYSHOST Host, IN UINT8 Socket, IN UINT8 Ch);
VOID DoCompUpdate (IN PSYSHOST Host, IN UINT8 Socket, IN UINT8 McId);
VOID CompDqOdtClkWorkAround (IN PSYSHOST Host, IN UINT8 Socket);
MRC_STATUS GetSetDelayMulticastOffset (IN PSYSHOST Host, IN UINT8 Socket, IN UINT8 Ch, IN UINT8 Dimm, IN UINT8 Rank, IN UINT8 Strobe,
                                       IN UINT8 Bit, IN MRC_LT Level, IN MRC_GT Group, IN UINT8 Mode, IN OUT INT16 *Value);
VOID ConvertPiDelayToRxDfeEnabledCsrValue (IN UINT16 PiDelay, IN UINT16 PiDelay2, OUT UINT16 *CsrValue, OUT UINT16 *CsrValue2);

/*
  Returns true if GetSet (..,ALL_STROBES,..) modifies
  all the strobes across all subchannels

  @param[in]      Host              Pointer to SysHost structure
  @param[in]      Socket            Current socket under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test

  @retval   TRUE    GetSet modifies all strobes across subchannels when ALL_STROBES is used
  @retval   FALSE   GetSet modifies all strobes within each subchannel when ALL_STROBES is used

*/
BOOLEAN
IsAllStrobesAcrossSubCh (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket,
  IN      MRC_LT      Level,
  IN      MRC_GT      Group
  )
{
  return FALSE;
}

/*
  Returns true if a margin group acts as per strobe control but underline is per be bit control which has
  this behavior: When it is read, only the maximum value of the strobe is returned.
  The MemSweepLib need to adjust the individual bit sweep results by read back the per bit value
  with a per bit version group.

  @param[in]      Host              Pointer to SysHost structure
  @param[in]      Socket            Current socket under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test

  @retval   TRUE    Group acts as per strobe control but underline is per be bit control
  @retval   FALSE   Group does not act as per strobe control but underline is per be bit control

*/
BOOLEAN
IsPerBitControlWhileActPerStrobe (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket,
  IN      MRC_LT      Level,
  IN      MRC_GT      Group
  )
{
  return FALSE;
}

/*
  Returns a per bit control margin group that corresponds to the input group.

  NOTE: it should be called after IsPerBitControlWhileActPerStrobe() to get the corresponding group.

  @param[in]      Host              Pointer to SysHost structure
  @param[in]      Socket            Current socket under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test

  @retval   Corresponding per be bit control group

*/
MRC_GT
GetPerBitGroup (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket,
  IN      MRC_LT      Level,
  IN      MRC_GT      Group
  )
{
  return Group;
}

MRC_STATUS
GetSetTxTco (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      UINT8     Strobe,
  IN      UINT8     Bit,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  );

#define MAX_TX_PER_BIT_SKEW   17

/**

Initialize the DDRIO interface Wrapper

@param Host  - Pointer to sysHost

@retval SUCCESS

**/
UINT32
InitInterface (
  PSYSHOST  Host                              // Pointer to sysHost, the system Host (root) structure
  )
{
  UINT32  Status = SUCCESS;

  Status = RunMmrc (Host);
  //
  // Exit if HVM Mode is enabled
  //
  HvmExitTarget (Host, Host->var.mem.currentSocket);
  //
  // Delay if this is a CTE build, otherwise do nothing.
  //
  CteDelayDclk (350); // Delay MMRC after vsshi is set for it to settle



  return Status;
}

/** Get the DDRCRINTFROUNDTRIP*_CH* register offset for a particular channel and DIMM.

  This additionally provides bounds checking for the channel and DIMM values.

  @param[in]  Channel             The target channel number on the memory controller
  @param[in]  Dimm                The target DIMM number
  @param[out] RoundTripRegOffset  The DDRCRINTFROUNDTRIP*_CH* register offset for the target channel and DIMM

  @retval EFI_SUCCESS   Register offset was successfully determined
  @retval !EFI_SUCCESS  Register offset was not successfully determined

**/
EFI_STATUS
GetRoundTripRegOffset (
  IN      UINT8   Channel,
  IN      UINT8   Dimm,
      OUT UINT32  *RoundTripRegOffset
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;

  *RoundTripRegOffset = 0;

  switch (Dimm) {
    case 0:
      if (Channel >= ((sizeof mDimm0RoundTripRegOffsets) / (sizeof mDimm0RoundTripRegOffsets[0]))) {
        RcDebugPrint (SDBG_ERROR, "\nChannel out of bounds: %d\n", Channel);
        RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_165);
        Status = EFI_UNSUPPORTED;
      } else {
        *RoundTripRegOffset = mDimm0RoundTripRegOffsets[Channel];
      }
      break;
    case 1:
      if (Channel >= ((sizeof mDimm1RoundTripRegOffsets) / (sizeof mDimm1RoundTripRegOffsets[0]))) {
        RcDebugPrint (SDBG_ERROR, "\nChannel out of bounds: %d\n", Channel);
        RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_166);
        Status = EFI_UNSUPPORTED;
      } else {
        *RoundTripRegOffset = mDimm1RoundTripRegOffsets[Channel];
      }
      break;
    default:
      RcDebugPrint (SDBG_ERROR, "\nDimm out of bounds: %d\n", Dimm);
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_167);
      Status = EFI_UNSUPPORTED;
      break;
  }

  return Status;
}

STATIC
VOID
RestoreDDRIO (
  PSYSHOST  Host,
  UINT8     socket
  )
{
  UINT8               ch;
  UINT8               imc;
  UINT8               strobe;
  struct imcNvram     (*imcNvList)[MAX_IMC];
  struct channelNvram (*channelNvList)[MAX_CH];
  UINT8 MSVx4 = GetMaxStrobeValid(Host);
  UINT8 MaxChDdr;
  UINT8 MaxImc;

  MaxImc        = GetMaxImc ();
  MaxChDdr      = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, socket);
  imcNvList     = GetImcNvList(Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    MemWritePciCfgEp (socket, ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRClkControls);

    for (strobe = 0; strobe < MSVx4; strobe++) {
      WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, (*channelNvList)[ch].dataControl0);
      WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL1N0_0_MCIO_DDRIO_REG, (*channelNvList)[ch].dataControl1[strobe]);
      WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL2N0_0_MCIO_DDRIO_REG, (*channelNvList)[ch].dataControl2[strobe]);
      WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL4N0_0_MCIO_DDRIO_REG, (*channelNvList)[ch].dataControl4[strobe]);
      WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL3N0_0_MCIO_DDRIO_REG, (*channelNvList)[ch].dataControl3[strobe]);
    }
    //
    // Restore roundtrip, IO latencies, etc, for fast boot.
    //
    RestoreTimings(Host, socket, ch);

  } // Ch

  for (imc = 0; imc < MaxImc; imc++) {
    if (Host->var.mem.socket[socket].imcEnabled[imc] == 0) {
      continue;
    }
    MemWritePciCfgMC (socket, imc, DDRCRDIMMVREFCONTROL1_MCIO_DDRIOEXT_REG, (*imcNvList)[imc].dimmVrefControl1);
    MemWritePciCfgMC (socket, imc, DDRCRCOMPCTL0_MCIO_DDRIOEXT_REG, (*imcNvList)[imc].ddrCRCompCtl0);
  }

}

/**
  Override Timing Parameters based on input data

  @param Host  - Pointer to sysHost

**/
STATIC
UINT32
DfxTimingOverride10nm (
  PSYSHOST  Host
  )
{
  UINT16                                      MinLimit;
  UINT16                                      MaxLimit;
  UINT16                                      UsDelay;
  UINT8                                       Socket;
  UINT8                                       Bit;
  UINT8                                       Ch;
  UINT8                                       Dimm;
  UINT8                                       Rank;
  UINT8                                       LogRank;
  UINT8                                       RoundTripValue;
  UINT8                                       IoLatencyValue;
  INT16                                       InputValue;
  INT16                                       NewDelays[BITS_PER_NIBBLE];
  struct dimmNvram                            (*DimmNvList)[MAX_DIMM];
  struct channelNvram                         (*channelNvList)[MAX_CH];
  SYS_SETUP                                   *Setup;
  UINT8                                       MaxChDdr;

  Setup = GetSysSetupPointer ();

  Socket = Host->var.mem.currentSocket;

  RcDebugPrint (SDBG_DEFAULT, "\n DfxTimingOverride10nm Starts \n");

  if (!(UbiosGenerationEnabled ())) {
    if (Setup->mem.dfxMemSetup.DfxTimingOverrideCtrl == 0) {
      return SUCCESS;
    }
  }
  //
  // Return if this socket is disabled
  //
  if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
    return SUCCESS;
  }

  channelNvList = GetChannelNvList (Host, Socket);
  MaxChDdr      = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*channelNvList)[Ch].enabled == 0) {
      continue;
    }
    DimmNvList  = GetDimmNvList (Host, Socket, Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }
      for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        //
        // Get the logical rank #
        //
        LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);

        //
        //Program Round Trip via DFx knobs for uBIOS
        //

        //
        // Get the round trip value for DDRT from setup and ensure it is an even value.
        // The choice to do it in this code section is that this is SOC specific code and
        // setup is generic code.  This is an SOC specific requirement.
        //
        if ((*DimmNvList)[Dimm].DcpmmPresent == 1) {
          RoundTripValue = (UINT8) Setup->mem.dfxMemSetup.DfxDdrtRt;
        } else {
          RoundTripValue = (UINT8) Setup->mem.dfxMemSetup.DfxInputRt;
        }
        //
        // DfxDdrtRt writes data across 7 bit fields; so an I/P > MRC_ROUND_TRIP_MAX_VALUE means skip programming
        // Check the original setup value to determine if this value should be programmed.
        //
        if (RoundTripValue <= MRC_ROUND_TRIP_MAX_VALUE) {
          if (RoundTripValue % 2 == 1) {
            // This is an ODD value so use -1 to get to a safe even value
            RoundTripValue -= 1;
          }
          SetRoundTrip (Socket, Ch, LogRank, RoundTripValue);
        }

        //
        // Get the IO Latency value from setup and ensure it is an odd value.
        // The choice to do it in this code section is that this is SOC specific code and
        // setup is generic code.  This is an SOC specific requirement.
        //
        IoLatencyValue = Setup->mem.dfxMemSetup.DfxInputIoLatency;
        if (IoLatencyValue != 0 && IoLatencyValue % 2 == 0) {
          // This is an even value so use +1 to get to a safe odd value
          IoLatencyValue += 1;
        }
        //
        // DfxInputIoLatency is a UINT8, > 0xF means skip programming
        // Check the original setup value to determine if this value should be programmed.
        //
        if (Setup->mem.dfxMemSetup.DfxInputIoLatency <= 0xF) {
          SetIOLatency (Socket, Ch, LogRank, IoLatencyValue);
        }

        //
        // DfxInputTxDqDelay is a UINT16, outside of silicon limits means skip programming
        //
        GetDataGroupLimits (Host, DdrLevel, TxDqDelay, &MinLimit, &MaxLimit, &UsDelay);
        if ((Setup->mem.dfxMemSetup.DfxInputTxDqDelay > MinLimit) && (Setup->mem.dfxMemSetup.DfxInputTxDqDelay < MaxLimit)) {
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, ALL_STROBES, 0, DdrLevel, TxDqDelay, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, (INT16 *)&Setup->mem.dfxMemSetup.DfxInputTxDqDelay);
        }
        //
        // DfxInputTxDqsDelay is a UINT16, outside of silicon limits means skip programming (Silicon max 703)
        //
        GetDataGroupLimits (Host, DdrLevel, TxDqsDelay, &MinLimit, &MaxLimit, &UsDelay);
        if ((Setup->mem.dfxMemSetup.DfxInputTxDqsDelay > MinLimit) && (Setup->mem.dfxMemSetup.DfxInputTxDqsDelay < MaxLimit)) {
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, ALL_STROBES, 0, DdrLevel, TxDqsDelay, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, (INT16 *)&Setup->mem.dfxMemSetup.DfxInputTxDqsDelay);
        }

        //
        // DfxInputTxDqBitDelay is a UINT16, outside of silicon limits means skip programming (Silicon max 703)
        //
        GetDataGroupLimits (Host, DdrLevel, TxDqBitDelay, &MinLimit, &MaxLimit, &UsDelay);
        if ((Setup->mem.dfxMemSetup.DfxInputTxDqBitDelay > MinLimit) && (Setup->mem.dfxMemSetup.DfxInputTxDqBitDelay < MaxLimit)) {
          for (Bit = 0; Bit < BITS_PER_NIBBLE; Bit++) {
            NewDelays[Bit] = (INT16)Setup->mem.dfxMemSetup.DfxInputTxDqBitDelay;
          }
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, ALL_STROBES, ALL_BITS, DdrLevel, TxDqBitDelay, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, NewDelays);
        }

        //
        // DfxInputRxDqDelay is a UINT8, outside of silicon limits means skip programming (Silcon max 0x7)
        //
        GetDataGroupLimits (Host, DdrLevel, RxDqDelay, &MinLimit, &MaxLimit, &UsDelay);
        if ((Setup->mem.dfxMemSetup.DfxInputRxDqDelay > MinLimit) && (Setup->mem.dfxMemSetup.DfxInputRxDqDelay < MaxLimit)) {
          InputValue = (INT16) Setup->mem.dfxMemSetup.DfxInputRxDqDelay;
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, ALL_STROBES, 0, DdrLevel, RxDqDelay, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &InputValue);
        }

        //
        // DfxInputRxDqsDelay is a UINT16, outside of silicon limits means skip programming (silicon max 127)
        //
        GetDataGroupLimits (Host, DdrLevel, RxDqsDelay, &MinLimit, &MaxLimit, &UsDelay);
        if ((Setup->mem.dfxMemSetup.DfxInputRxDqsDelay > MinLimit) && (Setup->mem.dfxMemSetup.DfxInputRxDqsDelay < MaxLimit)) {
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, ALL_STROBES, 0, DdrLevel, RxDqsDelay, GSM_UPDATE_CACHE, (INT16 *)&Setup->mem.dfxMemSetup.DfxInputRxDqsDelay);
        }

        //
        // DfxInputRxDqsBitDelay is a UINT8, outside of silicon limits means skip programming
        //
        GetDataGroupLimits (Host, DdrLevel, RxDqsBitDelay, &MinLimit, &MaxLimit, &UsDelay);
        if ((Setup->mem.dfxMemSetup.DfxInputRxDqBitDelay > MinLimit) && (Setup->mem.dfxMemSetup.DfxInputRxDqBitDelay < MaxLimit)) {
          for (Bit = 0; Bit < BITS_PER_NIBBLE; Bit++) {
            NewDelays[Bit] = (INT16)Setup->mem.dfxMemSetup.DfxInputRxDqBitDelay;
          }
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, ALL_STROBES, ALL_BITS, DdrLevel, RxDqsBitDelay, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, NewDelays);
        }

        //
        // DfxRecEnDelay is a UINT16, outside of silicon limits means skip programming (silicon max 639)
        //
        GetDataGroupLimits (Host, DdrLevel, RecEnDelay, &MinLimit, &MaxLimit, &UsDelay);
        if ((Setup->mem.dfxMemSetup.DfxRcvEnDelay > MinLimit) && (Setup->mem.dfxMemSetup.DfxRcvEnDelay < MaxLimit)) {
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, ALL_STROBES, 0, DdrLevel, RecEnDelay, GSM_UPDATE_CACHE, (INT16 *)&Setup->mem.dfxMemSetup.DfxRcvEnDelay);
        }
      } //rank loop

      //
      // DfxInputGnt2Erid is an UINT16, > DDRT_GNT2ERID_MAX means skip programming
      //
      if ((Setup->mem.dfxMemSetup.DfxInputGnt2Erid <= DDRT_GNT2ERID_MAX)  && ((*DimmNvList)[Dimm].DcpmmPresent)) {
        GetSetSxpGnt2Erid (Host, Socket, Ch, Dimm, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, (INT16 *)&Setup->mem.dfxMemSetup.DfxInputGnt2Erid);
      }

      //
      // DfxInputIoLatencyIoComp is a UINT8, > 0x3F means skip programming
      //
      if (Setup->mem.dfxMemSetup.DfxInputIoLatencyIoComp <= 0x3F) {
        SetIoLatencyIoComp (Host, Socket, Ch, NO_SUBCH, Setup->mem.dfxMemSetup.DfxInputIoLatencyIoComp);
      }

      //
      // DfxCwlAdjPos is a UINT8, > MAX_CWL_ADJ skip programming
      //
      if (Setup->mem.dfxMemSetup.DfxCwlAdjPos <= MAX_CWL_ADJ) {
        if ((*DimmNvList)[Dimm].DcpmmPresent) {
          SetCwlAdj (Host, Socket, Ch, CWLADJ_DDRT, (INT16) Setup->mem.dfxMemSetup.DfxCwlAdjPos);
        } else {
          SetCwlAdj (Host, Socket, Ch, CWLADJ_DDR4, (INT16) Setup->mem.dfxMemSetup.DfxCwlAdjPos);
        }
      }

      //
      // DfxCwlAdjNeg is a UINT8, > ABS (MIN_CWL_ADJ) skip programming
      //
      if (Setup->mem.dfxMemSetup.DfxCwlAdjNeg <= ABS (MIN_CWL_ADJ)) {
        if ((*DimmNvList)[Dimm].DcpmmPresent) {
          SetCwlAdj (Host, Socket, Ch, CWLADJ_DDRT, (INT16) Setup->mem.dfxMemSetup.DfxCwlAdjNeg);
        } else {
          SetCwlAdj (Host, Socket, Ch, CWLADJ_DDR4, (INT16) Setup->mem.dfxMemSetup.DfxCwlAdjNeg);
        }
      }
    } //dimm loop
  } // ch loop

  RcDebugPrint (SDBG_DEFAULT, " \n DfxTimingOverride10nm Ends \n");
  return SUCCESS;
}

/**

  Initialize DDRIO read and write preamble state

  @param[in]  Host    - Pointer to sysHost
  @param[in]  Socket  - Socket number
  @param[in]  Ch      - Channel number

  @retval n/a

**/
VOID
PreambleInitDdrio (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch
  )
{
  struct channelNvram                   (*ChannelNvList)[MAX_CH];
  UINT8                                 WritePreambleState;
  DATACONTROL0N0_0_MCIO_DDRIO_STRUCT    DataControl0;

  ChannelNvList = GetChannelNvList (Host, Socket);

  WritePreambleState = GetPreambleState (Socket, WRITE_PREAMBLE);

  //
  // 1 or 2 TCLK write preamble setting
  //
  DataControl0.Data = (*ChannelNvList)[Ch].dataControl0;
  DataControl0.Bits.longpreambleenable = WritePreambleState > PREAMBLE_1TCLK ? 1 : 0;
  (*ChannelNvList)[Ch].dataControl0 = DataControl0.Data; // update cached value
  //
  // Assume cached value will be written to the registers
  //
}

/**

  Initialize the Rx DFE DDRIO interface

  @param[in]  Host    - Pointer to sysHost
  @param[in]  Socket  - Socket number
  @param[in]  Ch      - Channel number

  @retval n/a

**/
VOID
RxDfeCsrInit (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  UINT8                                       Strobe;
  struct channelNvram                         (*ChannelNvList)[MAX_CH];
  DDRCRDATARXDFECONTROL_MCIO_DDRIO_STRUCT     RxDfeControl;
  DDRCRDATARXDFECOEFF_MCIO_DDRIO_STRUCT       RxDfeCoeff;
  UINT8                                       MSVx4 = GetMaxStrobeValid (Host);

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Strobe = 0; Strobe < MSVx4; Strobe++) {
    //
    // Initialize RxDfeControl and RxDfeCoeff
    // This code assumes RxDfeControl and RxDfeCoeff cache is initialized
    //
    RxDfeControl.Data = (*ChannelNvList)[Ch].RxDfeControl[Strobe];
    RxDfeCoeff.Data = (*ChannelNvList)[Ch].RxDfeCoeff[Strobe];
    if (GetRxDfeEnableChip (Host, Socket)) {
      RxDfeControl.Bits.dfe_en = 1;
      RxDfeControl.Bits.set_vcdl_fdbk_path = 1;
      RxDfeControl.Bits.byp_vcdl_fdbk_path = 1;
      RxDfeControl.Bits.byp_vcdl_fwd_path = 1;
      RxDfeCoeff.Bits.dfecoeff_rank0 = 7;
      RxDfeCoeff.Bits.dfecoeff_rank1 = 7;
      RxDfeCoeff.Bits.dfecoeff_rank2 = 7;
      RxDfeCoeff.Bits.dfecoeff_rank3 = 7;
      RxDfeCoeff.Bits.dfecoeff_rank4 = 7;
      RxDfeCoeff.Bits.dfecoeff_rank5 = 7;
      RxDfeCoeff.Bits.dfecoeff_rank6 = 7;
      RxDfeCoeff.Bits.dfecoeff_rank7 = 7;
    } else {
      RxDfeControl.Bits.dfe_en = 0;
      RxDfeControl.Bits.set_vcdl_fdbk_path = 0;
      RxDfeControl.Bits.byp_vcdl_fdbk_path = 0;
    }
    (*ChannelNvList)[Ch].RxDfeControl[Strobe] = RxDfeControl.Data; // update cache value
    (*ChannelNvList)[Ch].RxDfeCoeff[Strobe] = RxDfeCoeff.Data; // update cache value
    WriteDdrioDataRegCh (Host, Socket, Ch, Strobe, DDRCRDATARXDFECONTROL_MCIO_DDRIO_REG, RxDfeControl.Data);
    WriteDdrioDataRegCh (Host, Socket, Ch, Strobe, DDRCRDATARXDFECOEFF_MCIO_DDRIO_REG, RxDfeCoeff.Data);
  } //Strobe
  //
  // If RxDfe is enabled, fix up MaxPhaseInReadAdjustmentDq value
  //
  if (GetRxDfeEnableChip (Host, Socket)) {
    MEM_CHIP_POLICY_DEF(MaxPhaseInReadAdjustmentDq)            = MAX_PHASE_IN_READ_ADJ_DQ_RX_DFE;
  }
}

//
// Default value of VssHiOrVrefControl.vsshiorvrefctl
//  1) BIT[13]  Open Loop Mode        Set to 1 (also ungates CaVref controls in other channels if CH0 instance)
//  2) BIT[6:0] Target VssHi Voltage  Set to 0x38, safe default value
//  3) All other bits remain at 0
//
#define VSSHI_VREF_CTL_DEFAULT 0x2038

CONST UINT32  mVssHiOrVrefControlAddr[] = {
  VSSHIORVREFCONTROL_CMDS_MCIO_DDRIOEXT_REG,
  VSSHIORVREFCONTROL_CTL_MCIO_DDRIOEXT_REG,
  VSSHIORVREFCONTROL_CMDN_MCIO_DDRIOEXT_REG,
  VSSHIORVREFCONTROL_CKE_MCIO_DDRIOEXT_REG
};

/**
  Initializes all instances of VssHiOrVrefControl.vsshiorvrefctl

  The default programming of VssHiOrVrefControl.vsshiorvrefctl is required in all CH0 FUB instances of VsshiOrVrefControl
  because BIT13 gates CaVref controls on other channels. This programming cannot always be done in MMRC because MMRC does
  not program unpopulated channels. This routine programs the default into all instances of this register, some of which
  may be overwritten later as necessary by MMRC or training.

  @param[in]  Host    Pointer to sysHost
  @param[in]  Socket  Current socket
**/
VOID
InitVrefControls (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
  UINT8   Channel;
  UINT8   FubIndex;
  UINT8   MaxChannels = GetMaxChDdr ();

  VSSHIORVREFCONTROLN0_0_MCIO_DDRIO_STRUCT  VrefCtlStruct;

  for (Channel = 0; Channel < MaxChannels; Channel++) {
    for (FubIndex = 0; FubIndex < sizeof (mVssHiOrVrefControlAddr) / sizeof (mVssHiOrVrefControlAddr[0]); FubIndex++) {

      VrefCtlStruct.Data = MemReadPciCfgEp (Socket, Channel, mVssHiOrVrefControlAddr[FubIndex]);
      VrefCtlStruct.Bits.vsshiorvrefctl = VSSHI_VREF_CTL_DEFAULT;
      MemWritePciCfgEp (Socket, Channel, mVssHiOrVrefControlAddr[FubIndex], VrefCtlStruct.Data);
    }
  }

  return;
}

/**

  Initialize the DDRIO interface

  @param Host  - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
InitDdrioInterfaceLate (
  PSYSHOST  Host
  )
{
  UINT8                                       socket;
  UINT8                                       ch;
  UINT8                                       dimm;
  UINT8                                       rank;
  UINT8                                       strobe;
  UINT8                                       txVref = 0;
  UINT8                                       TxVrefSafe;
  UINT8                                       imc;
  INT16                                       tempVal;
  UINT16                                      minVal = 0xFF;
  UINT16                                      maxVal = 0x0;
  INT16                                       Linear;
  UINT16                                      VrefValue;
#ifdef DDR5_SUPPORT
  UINT16                                      DcaVrefValue;
  INT8                                        DcaVrefValueReg;
  UINT16                                      DcsVrefValue;
  INT8                                        DcsVrefValueReg;
  UINT8                                       IbtCaValue;
  UINT8                                       IbtCaValueReg;
  UINT8                                       IbtCsValue;
  UINT8                                       IbtCsValueReg;
  UINT8                                       IbtClkValue;
  UINT8                                       IbtClkValueReg;
#endif // DDR5_SUPPORT
  struct channelNvram                         (*channelNvList)[MAX_CH];
  struct dimmNvram                            (*dimmNvList)[MAX_DIMM];
  struct ddrRank                              (*rankList)[MAX_RANK_DIMM];
  struct rankDevice                           (*rankStruct)[MAX_RANK_DIMM];
  DATACONTROL0N0_0_MCIO_DDRIO_STRUCT          dataControl0;
  DATACONTROL0N0_0_MCIO_DDRIO_STRUCT          dataControl0_nonecc;
  DDRCRCLKCONTROLS_MCIO_DDRIOEXT_STRUCT       ddrCRClkControls;
  DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_STRUCT      ddrCRClkRanksEnabled;
  UINT8                                       MSVx4 = GetMaxStrobeValid (Host);
  BootMode                                    SysBootMode;
  CHIP_ERLY_CMD_CK_STRUCT                     ChipErlyCmdCKStruct;
  UINT32                                      Status = SUCCESS;
  UINT8                                       MaxChDdr;
  UINT8                                       MaxImc;
  SYS_SETUP                                   *Setup;

  Setup = GetSysSetupPointer ();
  ZeroMem ((UINT8 *) &ChipErlyCmdCKStruct, sizeof (ChipErlyCmdCKStruct));

  MaxImc      = GetMaxImc ();
  SysBootMode = GetSysBootMode ();
  Linear = 0;
  socket = Host->var.mem.currentSocket;

  SetMcDdrtCfg (Host);

  //
  // Return if this socket is disabled
  //

  if (IsSocketDimmPopulated (Host, socket) != TRUE) {
    return SUCCESS;
  }

  MaxChDdr = GetMaxChDdr ();

  SetFeedbackCmdVrefTarget (Host, socket, SysBootMode);

  if(Host->DdrioUltSupport) {
    CteSetOptions ("cmd_stretch", 0x7, 0x3, 0, 3);
    CteSetOptions ("cmd_stretch", 0x7, 0x3, 4, 3);
  }

  channelNvList = GetChannelNvList (Host, socket);

  if (IsSiliconWorkaroundEnabled ("S1909269691")) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      TrnOnChipCMDADDOpEn (Host, socket, ch, &ChipErlyCmdCKStruct);
    }
  }

  if ((SysBootMode == NormalBoot) && (Host->var.mem.subBootMode == ColdBoot)) {

    //
    // Initialize DDRIO cached values
    //
    CacheDDRIO (Host, socket);

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      if (!IsBrsPresent (Host, socket)) {
        //
        // The initial cmd value is set assuming 1n cmd timing, in the case where the configuration is not 1n then push cmd 1Qclk
        //
        if ((*channelNvList)[ch].timingMode != TIMING_1N) {
          tempVal = 64;
          GetSetCmdGroupDelay (Host, socket, ch, CmdAll, GSM_WRITE_OFFSET | GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &tempVal, &minVal, &maxVal);
        }
      }

      //
      // Initialize read/write preamble DDRIO registers
      //
      PreambleInitDdrio (Host, socket, ch);

      //
      // Initialize RX DFE related CSRs
      //
      RxDfeCsrInit (Host, socket, ch);

      dataControl0.Data = (*channelNvList)[ch].dataControl0;

      if (IsSiliconWorkaroundEnabled ("S1707129215") && (Setup->mem.dfxMemSetup.DfxForceOdtOn == FORCE_ODT_ON_ENABLE)) {
        dataControl0.Bits.forceodton = 1;
        (*channelNvList)[ch].dataControl0 = dataControl0.Data;
      }

      // setup no ecc version
      dataControl0_nonecc.Data = dataControl0.Data;
      dataControl0_nonecc.Bits.rxdisable = 1;
      dataControl0_nonecc.Bits.txdisable = 1;

      for (strobe = 0; strobe < MSVx4; strobe++) {
        if (IsStrobeNotValid (Host, strobe)){
          WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, dataControl0_nonecc.Data);
        } else {
          WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, dataControl0.Data);
        }
      }
    }

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      //
      // Construct CKE mask
      //
      dimmNvList  = GetDimmNvList (Host, socket, ch);
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }

        //
        // Continue if the Vref value is not found
        //
        if (!GetDramVrefValueOdt (socket, ch, dimm, &VrefValue)) {
          continue;
        }
        RcDebugPrintWithDevice (
          SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "DRAM Vref   from ODT table: %d.%d%%\n",
          VrefValue / ODT_TABLE_LIB_VREF_PERCENT_FACTOR,
          VrefValue % ODT_TABLE_LIB_VREF_PERCENT_FACTOR
          );

#ifdef DDR5_SUPPORT
        GetDcaVrefValueOdt (socket, ch, dimm, &DcaVrefValue);
        RcDebugPrintWithDevice (
          SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "CA Vref     from ODT table: %d.%d%%\n",
          DcaVrefValue / ODT_TABLE_LIB_VREF_PERCENT_FACTOR,
          DcaVrefValue % ODT_TABLE_LIB_VREF_PERCENT_FACTOR
          );

        GetDcsVrefValueOdt (socket, ch, dimm, &DcsVrefValue);
        RcDebugPrintWithDevice (
          SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "CS Vref     from ODT table: %d.%d%%\n",
          DcsVrefValue / ODT_TABLE_LIB_VREF_PERCENT_FACTOR,
          DcsVrefValue % ODT_TABLE_LIB_VREF_PERCENT_FACTOR
          );

        GetIbtCaValueOdt (socket, ch, dimm, &IbtCaValue);
        RcDebugPrintWithDevice (
          SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "IbtCaValue  from ODT table: %d ohms\n",
          IbtCaValue
          );

        GetIbtCsValueOdt (socket, ch, dimm, &IbtCsValue);
        RcDebugPrintWithDevice (
          SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "IbtCsValue  from ODT table: %d ohms\n",
          IbtCsValue
          );

        GetIbtClkValueOdt (socket, ch, dimm, &IbtClkValue);
        RcDebugPrintWithDevice (
          SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "IbtClkValue from ODT table: %d ohms\n",
          IbtClkValue
          );
#endif // New DDR5 parameters

        rankList = GetRankNvList (Host, socket, ch, dimm);
        rankStruct  = GetRankStruct (Host, socket, ch, dimm);

        // Calculate Vref % based on Jedec spec
#ifdef DDR5_SUPPORT
        if (IsDdr5Present (Host, socket)) {

          Linear = ConvertPercentageToLinear (VrefValue);
          TxVrefSafe = (UINT8) ConvertLinearToPhysical (Linear);

        } else
#endif // #ifdef DDR5_SUPPORT
        {
          // Assume DRAM Vref range 1 (60% - 92.5%)
          VrefValue = ODT_TABLE_LIB_ROUND_VREF_PERCENT (VrefValue);
          txVref = ((((VrefValue - 60) * 100) / 65) & 0x3F);
          TxVrefSafe = txVref + DDR4_VREF_RANGE1_OFFSET;
        }

        for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          //
          // Store ODT values into channelNvList
          //
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Storing [DRAM Vref %%:    0x%x] (txVrefSafe)\n", TxVrefSafe);
          (*channelNvList)[ch].txVrefSafe[(*rankList)[rank].rankIndex] = TxVrefSafe;

#ifdef DDR5_SUPPORT
          Linear = ConvertPercentageToLinear (DcaVrefValue);
          DcaVrefValueReg = (INT8) ConvertLinearToPhysical (Linear);
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Storing [DRAM CA Vref %%: 0x%x]\n", DcaVrefValueReg);
          (*channelNvList)[ch].DcaVrefRegValue[(*rankList)[rank].rankIndex] = DcaVrefValueReg; // Value from ODT table, converted to MR value (per JEDEC spec)

          Linear = ConvertPercentageToLinear (DcsVrefValue);
          DcsVrefValueReg = (INT8) ConvertLinearToPhysical (Linear);
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Storing [DRAM CS Vref %%: 0x%x]\n", DcsVrefValueReg);
          (*channelNvList)[ch].DcsVrefRegValue[(*rankList)[rank].rankIndex] = DcsVrefValueReg; // Value from ODT table, converted to MR value (per JEDEC spec)

          if (EncodeIbtValueToRegister (IbtCaValue, &IbtCaValueReg) == MRC_STATUS_SUCCESS) {
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Storing [IbtCaValueReg:  0x%x]\n", IbtCaValueReg);
          } else {
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Storing *DEFAULT* value [IbtCaValueReg:  0x%x]\n", IbtCaValueReg);
          }
          (*channelNvList)[ch].IbtCaRegValue[(*rankList)[rank].rankIndex] = IbtCaValueReg; // CA value from ODT table (or a default), converted to MR value (per JEDEC spec)

          if (EncodeIbtValueToRegister (IbtCsValue, &IbtCsValueReg) == MRC_STATUS_SUCCESS) {
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Storing [IbtCsValueReg:  0x%x]\n", IbtCsValueReg);
          } else {
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Storing *DEFAULT* value [IbtCsValueReg:  0x%x]\n", IbtCsValueReg);
          }
          (*channelNvList)[ch].IbtCsRegValue[(*rankList)[rank].rankIndex] = IbtCsValueReg; // CS value from ODT table (or a default), converted to MR value (per JEDEC spec)

          if (EncodeIbtValueToRegister (IbtClkValue, &IbtClkValueReg) == MRC_STATUS_SUCCESS) {
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Storing [IbtClkValueReg: 0x%x]\n", IbtClkValueReg);
          } else {
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Storing *DEFAULT* value [IbtClkValueReg:  0x%x]\n", IbtClkValueReg);
          }
          (*channelNvList)[ch].IbtClkRegValue[(*rankList)[rank].rankIndex] = IbtClkValueReg; // CLK value from ODT table (or a default), converted to MR value (per JEDEC spec)
#endif // DDR5_SUPPORT

          //
          // Save Tx Vref
          //
          for (strobe = 0; strobe < MSVx4; strobe++) {

#if defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)
            if (IsLrdimmPresent (socket, ch, dimm)) {
              if ((*dimmNvList)[dimm].DcpmmPresent == 1) {
                // save backside rxVref here
                (*rankList)[rank].lrbufRxVref[strobe] = (*dimmNvList)[dimm].SPDLrbufDbVrefdq;
                (*rankStruct)[rank].lrbufRxVrefCache[strobe] = (*rankList)[rank].lrbufRxVref[strobe];
                //RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT, "DB RxVref to DRAM = %d\n", (*rankList)[rank].lrbufRxVref[strobe]);
              } else { //DDR4 path
                if ((*dimmNvList)[dimm].SPDSpecRev > 7) {
                  // For SPD rev > 7, get the TxVref value from SPD
                  switch (rank) {
                    case 0:
                      (*rankList)[rank].lrbufTxVref[strobe] = (*dimmNvList)[dimm].SPDLrbufDramVrefdqR0;
                      break;

                    case 1:
                      (*rankList)[rank].lrbufTxVref[strobe] = (*dimmNvList)[dimm].SPDLrbufDramVrefdqR1;
                      break;

                    case 2:
                      (*rankList)[rank].lrbufTxVref[strobe] = (*dimmNvList)[dimm].SPDLrbufDramVrefdqR2;
                      break;

                    case 3:
                      (*rankList)[rank].lrbufTxVref[strobe] = (*dimmNvList)[dimm].SPDLrbufDramVrefdqR3;
                      break;
                    default:
                      //Automated add of default case. Please review.
                      break;
                  }

                  (*rankStruct)[rank].lrbufTxVrefCache[strobe] = (*rankList)[rank].lrbufTxVref[strobe];
                  // Set MR6 value with training mode enabled
                  (*rankStruct)[rank].MR6[strobe] = (UINT8) ((*rankList)[rank].lrbufTxVref[strobe] | BIT7);
                  //also save backside rxVref here
                  (*rankList)[rank].lrbufRxVref[strobe] = (*dimmNvList)[dimm].SPDLrbufDbVrefdq;
                  (*rankStruct)[rank].lrbufRxVrefCache[strobe] = (*rankList)[rank].lrbufRxVref[strobe];
                } else {
                  if ((*dimmNvList)[dimm].numDramRanks == 4) {
                    //Backside DRAM txVref, hard code MR6 value for 78.2% = 0x1C with training mode enabled
                    (*rankStruct)[rank].MR6[strobe] = (0x1C | BIT7);
                    (*rankList)[rank].lrbufTxVref[strobe] = 0x1C;
                    (*rankStruct)[rank].lrbufTxVrefCache[strobe] = 0x1C;
                    //also save backside rxVref here
                    //quad rank buffer backside vref: 82.1% = 0x22 (range 1)
                    (*rankList)[rank].lrbufRxVref[strobe] = 0x22+DB_DRAM_VREF_RANGE2_OFFSET;
                    (*rankStruct)[rank].lrbufRxVrefCache[strobe] = 0x22+DB_DRAM_VREF_RANGE2_OFFSET;
                  } else {
                    //Backside DRAM txVref, hard code MR6 value for 74% = 0x16 with training mode enabled
                    (*rankStruct)[rank].MR6[strobe] = (0x16 | BIT7);
                    (*rankList)[rank].lrbufTxVref[strobe] = 0x16;
                    (*rankStruct)[rank].lrbufTxVrefCache[strobe] = 0x16;
                    //also save backside rxVref here
                    //dual rank buffer backside vref: 76.9% = 0x1A (range 1)
                    (*rankList)[rank].lrbufRxVref[strobe] = 0x1A+DB_DRAM_VREF_RANGE2_OFFSET;
                    (*rankStruct)[rank].lrbufRxVrefCache[strobe] = 0x1A+DB_DRAM_VREF_RANGE2_OFFSET;
                  }
                } //SPD rev
              } // DDR4 present
            } else // LRDIMM
#endif // defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)
            {
              (*rankStruct)[rank].MR6[strobe] = (txVref | BIT7);
            }
            (*channelNvList)[ch].txVref[(*rankList)[rank].rankIndex][strobe] = (*channelNvList)[ch].txVrefSafe[(*rankList)[rank].rankIndex];
            (*channelNvList)[ch].txVrefCache[(*rankList)[rank].rankIndex][strobe] = (*channelNvList)[ch].txVrefSafe[(*rankList)[rank].rankIndex];
          } // strobe loop

          if (!IsBrsPresent (Host, socket)) {
            (*channelNvList)[ch].ckeMask |= 1 << (*rankList)[rank].CKEIndex;
          }
        } // rank loop
      } // dimm loop
    } // ch loop

#ifdef DDR5_SUPPORT
    if (IsBrsPresent(Host, socket)) {
      SetCSMask(Host, socket);
    }
#endif

#ifdef LRDIMM_SUPPORT
    // Update safe TXVREF for LRDIMMS on backside
    if (Host->nvram.mem.socket[socket].lrDimmPresent) {
      UpdateSafeTxVref (Host, socket, LrbufLevel);
    }
#endif // #ifdef LRDIMM_SUPPORT

  } //boot mode

  //
  // Restore DDRIO values
  //
  if ((SysBootMode == S3Resume) || (Host->var.mem.subBootMode == WarmBootFast) || (Host->var.mem.subBootMode == ColdBootFast)){
    RestoreDDRIO (Host, socket);
  }

  // Intialize PXC and DDJC related registers if PXC or DDJC training is enabled
  PxcDdjcInitialize (Host, socket);

  // HSD 5331856 disable closed page mode during training
  for (imc = 0; imc < MaxImc; imc++) {
    if (Host->var.mem.socket[socket].imcEnabled[imc] == 0) {
      continue;
    }

    SetPageModeMc (Host, socket, imc, CLOSED_PAGE_DIS);
  } // imc loop

  //
  // Hard coded COMP settings
  //
  ProgramIOCompValues (Host, socket);

  RcompStaticLegTraining (Host);

  if ((SysBootMode == NormalBoot) && ((Host->var.mem.subBootMode == ColdBoot) || (Host->var.mem.subBootMode == ColdBootFast))){

    // HSD 5331110  Disable Periodic Rcomp during memory training
    for (imc = 0; imc < MaxImc; imc++) {
      if (Host->var.mem.socket[socket].imcEnabled[imc] == 0) {
        continue;
      }
      SetRcompDisableMc (Host, socket, imc);
    }

    FnvDdrtIoInitPllTarget (Host, socket, SysBootMode);
  }

  if (Host->var.mem.subBootMode == WarmBootFast) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      MemWritePciCfgEp (socket, ch, DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCmdTrainingCmdN);
      MemWritePciCfgEp (socket, ch, DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCmdTrainingCmdS);
      MemWritePciCfgEp (socket, ch, DDRCRCMDTRAINING_CTL_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCtlTraining);
      MemWritePciCfgEp (socket, ch, DDRCRCMDTRAINING_CKE_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCkeTraining);
      MemWritePciCfgEp (socket, ch, DDRCRCLKTRAINING_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRClkTraining);

      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CMDN_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCmdControls3CmdN);
      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CMDS_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCmdControls3CmdS);
      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CTL_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCmdControls3Ctl);
      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CKE_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCmdControls3Cke);

      //
      // Restore value of DdrCrClkRanksUsed
      //
      // When programming DdrCrClkRanksUsed.refpiclk, there is a required settling time before the clocks can be safely
      // turned on. Programming DdrCrClkRanksUsed.ranken turns on clocks for enabled ranks, so these two fields must be
      // programmed separately.
      //
      // CSR access time (~300ns) is enough to statisfy the settling time, but posted writes may be enabled, so a 1us
      // fixed delay is inserted to ensure the minimum delay is met.
      //
      ddrCRClkRanksEnabled.Data = (*channelNvList)[ch].ddrCRClkRanksUsed;
      ddrCRClkRanksEnabled.Bits.ranken = 0;
      MemWritePciCfgEp (socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG, ddrCRClkRanksEnabled.Data);
      FixedDelayMicroSecond (1);
      MemWritePciCfgEp (socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRClkRanksUsed);

      for (strobe = 0; strobe < MAX_STROBE; strobe++) {
        WriteDdrioDataRegCh (Host, socket, ch, strobe, DATAOFFSETTRAINN0_0_MCIO_DDRIO_REG, (*channelNvList)[ch].dataOffsetTrain[strobe]);
        WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL4N0_0_MCIO_DDRIO_REG, (*channelNvList)[ch].dataControl4[strobe]);
      } // strobe loop
    }
  }

  OutputCheckpoint (CHECKPOINT_MAJOR_DDRIO_INIT, CHECKPOINT_MINOR_CLOCK_EN, 0);
  if ((SysBootMode != S3Resume)) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      //
      // Enable CK for all ranks
      //
      SetRankEnDdrio (Host, socket, ch, 0xF);
    }
    IO_Reset (Host, socket);
  }

  if ((SysBootMode == S3Resume)) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      MemWritePciCfgEp (socket, ch, DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCmdTrainingCmdN);
      MemWritePciCfgEp (socket, ch, DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCmdTrainingCmdS);
      MemWritePciCfgEp (socket, ch, DDRCRCMDTRAINING_CTL_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCtlTraining);
      MemWritePciCfgEp (socket, ch, DDRCRCMDTRAINING_CKE_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCkeTraining);
      MemWritePciCfgEp (socket, ch, DDRCRCLKTRAINING_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRClkTraining);

      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CMDN_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCmdControls3CmdN);
      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CMDS_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCmdControls3CmdS);
      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CTL_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCmdControls3Ctl);
      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CKE_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCmdControls3Cke);
      MemWritePciCfgEp (socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRClkRanksUsed);

      for (strobe = 0; strobe < MAX_STROBE; strobe++) {

        WriteDdrioDataRegCh (Host, socket, ch, strobe, DATAOFFSETTRAINN0_0_MCIO_DDRIO_REG, (*channelNvList)[ch].dataOffsetTrain[strobe]);
        WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL4N0_0_MCIO_DDRIO_REG, (*channelNvList)[ch].dataControl4[strobe]);
      } // strobe loop
    }

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      OutputCheckpoint (CHECKPOINT_MAJOR_DDRIO_INIT, CHECKPOINT_MINOR_CLOCK_EN_S3, 0);

      //
      // Enable CK for all ranks
      //
      SetRankEnDdrio (Host, socket, ch, 0xF);
    }
    IO_Reset (Host, socket);

    SetDclkEnableMc (MemTechDdr, socket);

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      ddrCRClkControls.Data = MemReadPciCfgEp (socket, ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG);
      ddrCRClkControls.Bits.intclkon = 1;
      MemWritePciCfgEp (socket, ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG, ddrCRClkControls.Data);
    }

    FnvPollingBootStatusRegister (Host, socket, FNV_DT_DONE);

    //
    // Send DDRT IO INIT with S3 complete to FW
    //
    FnvDdrtIoInit (Host, socket, DDRT_TRAINING_S3_COMPLETE);
  }

  CrossoverCalib10nm (Host);

  if (Host->var.mem.subBootMode == ColdBoot) {
    StartingCCCTarget (Host, socket);
  }

  //
  // Execute NVMDIMM IO init in NVMCTLR
  //
  OutputCheckpoint (CHECKPOINT_MAJOR_DDRIO_INIT, CHECKPOINT_MINOR_JEDEC_RESET_INIT, 0);
  ResetAllDdrChannels (Host, socket, CH_BITMASK);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if (((*channelNvList)[ch].enabled == 0) || ((*channelNvList)[ch].ddrtEnabled == 0)) {
      continue;
    }
    //
    // 1408838583: Disable DDRT Thermal Event setting prior to sending pll lock ddrtioinit command
    //
    SetMcaDdrtThermalEvent (socket, ch, TRUE);

    //
    // 1909299769: Disable MCA Unexpected Pkg CMI Idle during CPGC mode
    //
    SetMcaCmiIdleEvent (socket, ch, 1);
  }

  FnvDdrtIoInitDdrtFreqTarget (Host, socket, SysBootMode);

  FnvEnableResyncFsm (socket);

  Host->var.mem.socket[socket].ioInitdone = 1;

  OutputCheckpoint (CHECKPOINT_MAJOR_DDRIO_INIT, CHECKPOINT_MINOR_DDRT_IO_INIT, 0);

#ifdef DDR5_SUPPORT
  if (IsBrsPresent(Host, socket)) {
    if (Ddr5RambusD0Errata (Host, socket)) {
      //
      // Reset DRAM if Errata is applied
      //
      ResetAllDdrChannels (Host, socket, CH_BITMASK);
    }
  }
#endif //DDR5_SUPPORT

  if (((SysBootMode == NormalBoot) && ((Host->var.mem.subBootMode == ColdBoot) || (Host->var.mem.subBootMode == ColdBootFast)))){

    ScadExitTarget (Host, socket);

    //
    // CsrUnlock variable is to control only unlock DCPMM CSR once in cold boot path
    //
    for (ch = 0; ch < MaxChDdr; ch++) {
      if (((*channelNvList)[ch].enabled == 0) || ((*channelNvList)[ch].ddrtEnabled == 0)) {
        continue;
      }
      Host->var.mem.socket[socket].channelList[ch].CsrUnlock = 0;
    }
    //
    // Initiate JEDEC init
    //
    if (IsDdr5Present (Host, socket) == FALSE) {
      JedecInitDdrAll (Host, socket, CH_BITMASK);
    }

    if (Host->var.mem.subBootMode == ColdBoot){
      OutputCheckpoint (CHECKPOINT_MAJOR_DDRIO_INIT, CHECKPOINT_MINOR_SENSE_AMP_CAL, 0);
      SenseAmpOffset (Host);
      SetAepTrainingMode (Host,socket, ENABLE_TRAINING_MODE);
    }

    InitDdrioInterfaceLateFmc (Host, socket);
  }

  OutputCheckpoint (CHECKPOINT_MAJOR_DDRIO_INIT, CHECKPOINT_MINOR_TXVREF_CACHE, 0);

  //
  //
  // Update backside txVref cached values
  //

  CacheTxVrefFnvTarget (Host, socket, SysBootMode);

  //
  // Disable unused output clocks for ddr4 and NVMDIMM dimms
  //
  //
  //        DIMM1              DIMM0             Ranken -- binary(slot0 - bit 0 and bit 1; slot1 - bit 2 and bit 3)
  //  (L)RDIMM (SR/DR)   (L)RDIMM (SR/DR)          0101
  //       UDIMM (SR)        UDIMM (SR)            0101
  //       UDIMM (DR)        UDIMM (DR)            1111
  //       UDIMM (DR)        UDIMM (SR)            1101
  //       UDIMM (SR)        UDIMM (DR)            0111
  //       DDRT          (L)RDIMM (SR/DR)          1101

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    if (!(UbiosGenerationOrHsleEnabled ())) {
      dimmNvList = GetDimmNvList (Host, socket, ch);
      ddrCRClkRanksEnabled.Data = MemReadPciCfgEp (socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG);

      ddrCRClkRanksEnabled.Bits.ranken = 0;
      for (dimm = 0; dimm < Host->var.mem.socket[socket].channelList[ch].numDimmSlots; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }

        if (((*dimmNvList)[dimm].DcpmmPresent) || (((*dimmNvList)[dimm].numRanks == 2) && ((Host->nvram.mem.dimmTypePresent == UDIMM) || (Host->nvram.mem.dimmTypePresent == SODIMM)))) {
          ddrCRClkRanksEnabled.Bits.ranken |= (0x3 << dimm*2);
        } else {
          ddrCRClkRanksEnabled.Bits.ranken |= (0x1 << dimm*2);
        }
      }
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "Clocks enabled(ranken): 0x%x\n", ddrCRClkRanksEnabled.Bits.ranken);
      MemWritePciCfgEp (socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG, ddrCRClkRanksEnabled.Data);
    }

    SetIdleCmdToCkeLowMc (Host, socket, ch);
    CheckLpmodeEntryLatencyConstraints (Host, socket, ch);
    CheckTDdrtCkeConstraints (Host, socket, ch);

    if ((SysBootMode == NormalBoot) && (Host->var.mem.subBootMode == ColdBoot)) {
      SetTxPiOnFmc (Host, socket, ch, 1);
    }

  } // ch loop

  //
  // DFX hook to set timing values for FC Emulation at the end of InitDdrioInterfaceLate
  //
  DfxTimingOverride10nm (Host);

  return Status;
} // InitDdrioInterfaceLate

/**

  Initialize/Train the DDRIO Static Leg values

  @param[in]      Host  - Pointer to sysHost structure

  @retval SUCCESS

**/
STATIC
UINT32
RcompStaticLegTraining (
  IN      PSYSHOST Host
  )
{
  UINT8                                          Socket;
  UINT8                                          McId;
  UINT8                                          Ch;
  UINT8                                          Strobe;
  PT_ZONE                                        MemPhaseSave;
  struct channelNvram                            (*channelNvList)[MAX_CH];
  DDRCRCOMPSTATLEGTRAINCTRL_MCIO_DDRIOEXT_STRUCT DdrCrCompStatLegTrainCtrl;
  DDRCRCOMPOVR_MCIO_DDRIOEXT_STRUCT              DdrCrCompOvr;
  DDRCRCOMPCTL2_MCIO_DDRIOEXT_STRUCT             DdrCrCompCtl2;
  DDRCRDATACOMP1_MCIO_DDRIOEXT_STRUCT            DdrCrDataComp1;
  DDRCRDATACOMP0_MCIO_DDRIOEXT_STRUCT            DdrCrDataComp0;
  DDRCRCMDCOMP_CMDN_MCIO_DDRIOEXT_STRUCT         DdrCrCmdComp;
  DDRCRCTLCOMP_CMDN_MCIO_DDRIOEXT_STRUCT         DdrCrCtlComp;
  DDRCRCLKCOMP_CLK_MCIO_DDRIOEXT_STRUCT          DdrCrClkComp;
  BOOLEAN                                        ReRunComp;
  BOOLEAN                                        UpdateNeededForThisImc;
  DATACONTROL3N0_0_MCIO_DDRIO_STRUCT             DataControl3;
  DATACONTROL2N0_0_MCIO_DDRIO_STRUCT             DataControl2;
  DDRCRCLKCONTROLS_MCIO_DDRIOEXT_STRUCT          DdrCRClkControls;
  DDRCRCLKTRAINING_MCIO_DDRIOEXT_STRUCT          DdrCrClkTraining;
  DDRCRCMDCONTROLS1_CMDS_MCIO_DDRIOEXT_STRUCT    DdrCrCmdControls1Cmds;
  DDRCRCMDCONTROLS1_CTL_MCIO_DDRIOEXT_STRUCT     DdrCrCmdControls1Ctl;
  DDRCRCMDCONTROLS1_CMDN_MCIO_DDRIOEXT_STRUCT    DdrCrCmdControls1Cmdn;
  DDRCRCMDCONTROLS1_CKE_MCIO_DDRIOEXT_STRUCT     DdrCrCmdControls1Cke;
  UINT8                                          MaxChDdr;
  UINT8                                          MaxImc;


  UINT8 MSVx4 = GetMaxStrobeValid(Host);
  MaxImc      = GetMaxImc ();

  Socket = Host->var.mem.currentSocket;

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "RcompStaticLegTraining Starts\n");
  OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_RCOMP_STAT_LEG, Socket);

  channelNvList = GetChannelNvList (Host, Socket);

  //
  // Return if this Socket is disabled
  //
  if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
    return SUCCESS;
  }

  if (!IsMemFlowEnabled (RcompStaticLeg)) {
    return SUCCESS;
  }

  MaxChDdr = GetMaxChDdr ();
  GetMemPhase (&MemPhaseSave);
  SetMemPhaseCP (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_RCOMP_STAT_LEG);

  //
  //Set Number of Static Legs to 4 - set 4 bits - 11110b
  //
  for (McId = 0; McId < MaxImc; McId++) {
    if (Host->var.mem.socket[Socket].imcEnabled[McId] == 0) {
      continue;
    }
    DdrCrCompOvr.Data = MemReadPciCfgMC (Socket, McId, DDRCRCOMPOVR_MCIO_DDRIOEXT_REG);
    DdrCrCompCtl2.Data = MemReadPciCfgMC (Socket, McId, DDRCRCOMPCTL2_MCIO_DDRIOEXT_REG);
    DdrCrCompStatLegTrainCtrl.Data = MemReadPciCfgMC (Socket, McId, DDRCRCOMPSTATLEGTRAINCTRL_MCIO_DDRIOEXT_REG);
    DdrCrCompOvr.Bits.odtstatlegen = 0x1E;
    DdrCrCompCtl2.Bits.txdqstatlegen = 0x1E;
    DdrCrCompStatLegTrainCtrl.Bits.clkstatlegctrl = 0x1E;
    DdrCrCompStatLegTrainCtrl.Bits.cmdstatlegctrl = 0x1E;
    DdrCrCompStatLegTrainCtrl.Bits.ctlstatlegctrl = 0x1E;
    MemWritePciCfgMC (Socket, McId, DDRCRCOMPOVR_MCIO_DDRIOEXT_REG, DdrCrCompOvr.Data);
    MemWritePciCfgMC (Socket, McId, DDRCRCOMPCTL2_MCIO_DDRIOEXT_REG, DdrCrCompCtl2.Data);
    MemWritePciCfgMC (Socket, McId, DDRCRCOMPSTATLEGTRAINCTRL_MCIO_DDRIOEXT_REG, DdrCrCompStatLegTrainCtrl.Data);
  } //McId loop

  //
  // Run the comp
  //
  if (IsSiliconWorkaroundEnabled ("S1707054554")) {
    CompDqOdtClkWorkAround (Host, Socket);
  } else {
    DoComp (Host, Socket);
  }

  //
  // Read the rcomp code
  //   if rcomp code < 10, set the number of static legs to 3 (set 3 bits - 11100b) and rerun comp
  //   if rcomp code > 53, set the number of static legs to 5 (set 5 bits - 11111b)and rerun comp
  //
#ifdef DEBUG_CODE_BLOCK
  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "Comparing static leg results with resistor values Upper: 0x%x, Lower: 0x%x\n", UPPER_RCOMP_RESISTOR_VALUE, LOWER_RCOMP_RESISTOR_VALUE);
#endif //DEBUG_CODE_BLOCK
  ReRunComp = FALSE;
  for (McId = 0; McId < MaxImc; McId++) {
    if (Host->var.mem.socket[Socket].imcEnabled[McId] == 0) {
      continue;
    }
    UpdateNeededForThisImc = FALSE;
    DdrCrCompOvr.Data = MemReadPciCfgMC (Socket, McId, DDRCRCOMPOVR_MCIO_DDRIOEXT_REG);
    DdrCrCompCtl2.Data = MemReadPciCfgMC (Socket, McId, DDRCRCOMPCTL2_MCIO_DDRIOEXT_REG);
    DdrCrCompStatLegTrainCtrl.Data = MemReadPciCfgMC (Socket, McId, DDRCRCOMPSTATLEGTRAINCTRL_MCIO_DDRIOEXT_REG);
    DdrCrDataComp1.Data = MemReadPciCfgMC (Socket, McId, DDRCRDATACOMP1_MCIO_DDRIOEXT_REG);
    DdrCrDataComp0.Data = MemReadPciCfgMC (Socket, McId, DDRCRDATACOMP0_MCIO_DDRIOEXT_REG);
    DdrCrCmdComp.Data = MemReadPciCfgMC (Socket, McId, DDRCRCMDCOMP_CMDN_MCIO_DDRIOEXT_REG);
    DdrCrCtlComp.Data = MemReadPciCfgMC (Socket, McId, DDRCRCTLCOMP_CMDN_MCIO_DDRIOEXT_REG);
    DdrCrClkComp.Data = MemReadPciCfgMC (Socket, McId, DDRCRCLKCOMP_CLK_MCIO_DDRIOEXT_REG);

#ifdef DEBUG_CODE_BLOCK
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "MC%d RcompOdtUp = 0x%x  RcompOdtDown = 0x%x\n", McId, DdrCrDataComp1.Bits.rcompodtup, DdrCrDataComp1.Bits.rcompodtdown);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "MC%d DataRcompDrvUp = 0x%x  DataRcompDrvDown = 0x%x\n", McId, DdrCrDataComp0.Bits.rcompdrvup, DdrCrDataComp0.Bits.rcompdrvdown);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "MC%d CmdRcompDrvUp = 0x%x  CmdRcompDrvDown = 0x%x\n", McId, DdrCrCmdComp.Bits.rcompdrvup, DdrCrCmdComp.Bits.rcompdrvdown);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "MC%d CtlRcompDrvUp = 0x%x  CltRcompDrvDown = 0x%x\n", McId, DdrCrCtlComp.Bits.rcompdrvup, DdrCrCtlComp.Bits.rcompdrvdown);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "MC%d ClkRcompDrvUp = 0x%x  ClkRcompDrvDown = 0x%x\n", McId, DdrCrClkComp.Bits.rcompdrvup, DdrCrClkComp.Bits.rcompdrvdown);
#endif //DEBUG_CODE_BLOCK
    if ((DdrCrDataComp1.Bits.rcompodtup < LOWER_RCOMP_RESISTOR_VALUE) || (DdrCrDataComp1.Bits.rcompodtdown < LOWER_RCOMP_RESISTOR_VALUE)) {
      DdrCrCompOvr.Bits.odtstatlegen = 0x1C;
      UpdateNeededForThisImc = TRUE;
      ReRunComp = TRUE;
    } else if ((DdrCrDataComp1.Bits.rcompodtup > UPPER_RCOMP_RESISTOR_VALUE) || (DdrCrDataComp1.Bits.rcompodtdown > UPPER_RCOMP_RESISTOR_VALUE)) {
      DdrCrCompOvr.Bits.odtstatlegen = 0x1F;
      UpdateNeededForThisImc = TRUE;
      ReRunComp = TRUE;
    }
    if ((DdrCrDataComp0.Bits.rcompdrvup < LOWER_RCOMP_RESISTOR_VALUE) || (DdrCrDataComp0.Bits.rcompdrvdown < LOWER_RCOMP_RESISTOR_VALUE)) {
      DdrCrCompCtl2.Bits.txdqstatlegen = 0x1C;
      UpdateNeededForThisImc = TRUE;
      ReRunComp = TRUE;
    } else if ((DdrCrDataComp0.Bits.rcompdrvup > UPPER_RCOMP_RESISTOR_VALUE) || (DdrCrDataComp0.Bits.rcompdrvdown > UPPER_RCOMP_RESISTOR_VALUE)) {
      DdrCrCompCtl2.Bits.txdqstatlegen = 0x1F;
      UpdateNeededForThisImc = TRUE;
      ReRunComp = TRUE;
    }
    if ((DdrCrCmdComp.Bits.rcompdrvup < LOWER_RCOMP_RESISTOR_VALUE) || (DdrCrCmdComp.Bits.rcompdrvdown < LOWER_RCOMP_RESISTOR_VALUE)) {
      DdrCrCompStatLegTrainCtrl.Bits.cmdstatlegctrl = 0x1C;
      UpdateNeededForThisImc = TRUE;
      ReRunComp = TRUE;
    } else if ((DdrCrCmdComp.Bits.rcompdrvup > UPPER_RCOMP_RESISTOR_VALUE) || (DdrCrCmdComp.Bits.rcompdrvdown > UPPER_RCOMP_RESISTOR_VALUE)) {
      DdrCrCompStatLegTrainCtrl.Bits.cmdstatlegctrl = 0x1F;
      UpdateNeededForThisImc = TRUE;
      ReRunComp = TRUE;
    }
    if ((DdrCrCtlComp.Bits.rcompdrvup < LOWER_RCOMP_RESISTOR_VALUE) || (DdrCrCtlComp.Bits.rcompdrvdown < LOWER_RCOMP_RESISTOR_VALUE)) {
      DdrCrCompStatLegTrainCtrl.Bits.ctlstatlegctrl = 0x1C;
      UpdateNeededForThisImc = TRUE;
      ReRunComp = TRUE;
    } else if ((DdrCrCtlComp.Bits.rcompdrvup > UPPER_RCOMP_RESISTOR_VALUE) || (DdrCrCtlComp.Bits.rcompdrvdown > UPPER_RCOMP_RESISTOR_VALUE)) {
      DdrCrCompStatLegTrainCtrl.Bits.ctlstatlegctrl = 0x1F;
      UpdateNeededForThisImc = TRUE;
      ReRunComp = TRUE;
    }
    if ((DdrCrClkComp.Bits.rcompdrvup < LOWER_RCOMP_RESISTOR_VALUE) || (DdrCrClkComp.Bits.rcompdrvdown < LOWER_RCOMP_RESISTOR_VALUE)) {
      DdrCrCompStatLegTrainCtrl.Bits.clkstatlegctrl = 0x1C;
      UpdateNeededForThisImc = TRUE;
      ReRunComp = TRUE;
    } else if ((DdrCrClkComp.Bits.rcompdrvup > UPPER_RCOMP_RESISTOR_VALUE) || (DdrCrClkComp.Bits.rcompdrvdown > UPPER_RCOMP_RESISTOR_VALUE)) {
      DdrCrCompStatLegTrainCtrl.Bits.clkstatlegctrl = 0x1F;
      UpdateNeededForThisImc = TRUE;
      ReRunComp = TRUE;
    }
    if (UpdateNeededForThisImc) {
      MemWritePciCfgMC (Socket, McId, DDRCRCOMPOVR_MCIO_DDRIOEXT_REG, DdrCrCompOvr.Data);
      MemWritePciCfgMC (Socket, McId, DDRCRCOMPCTL2_MCIO_DDRIOEXT_REG, DdrCrCompCtl2.Data);
      MemWritePciCfgMC (Socket, McId, DDRCRCOMPSTATLEGTRAINCTRL_MCIO_DDRIOEXT_REG, DdrCrCompStatLegTrainCtrl.Data);
    }
  } //McId loop

  if (ReRunComp) {
    if (IsSiliconWorkaroundEnabled ("S1707054554")) {
      CompDqOdtClkWorkAround (Host, Socket);
    } else {
      DoComp (Host, Socket);
    }
  }
  //
  //Now program static training results to shadow/endpoint registers
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    McId = GetMCID (Host, Socket, Ch);
    if (Host->var.mem.socket[Socket].imcEnabled[McId] == 0) {
      continue;
    }
    //
    //DATA FUB: pins ddrdqstatdflt<4:0> & ddrodtstatdflt<4:0> of dqtransmitter & dqstransmitter
    //
    DdrCrCompCtl2.Data = MemReadPciCfgMC (Socket, McId, DDRCRCOMPCTL2_MCIO_DDRIOEXT_REG);
    DdrCrCompOvr.Data = MemReadPciCfgMC (Socket, McId, DDRCRCOMPOVR_MCIO_DDRIOEXT_REG);
    //
    //CMDCTLA/B FUBS: pins ddrcmdstatdflt<4:0> of cmdtransmitter -- AND --
    //CLK FUB: pins ddrdqstatdflt<4:0> & ddrodtstatdflt<4:0> of clktransmitter
    //
    DdrCrCompStatLegTrainCtrl.Data = MemReadPciCfgMC (Socket, McId, DDRCRCOMPSTATLEGTRAINCTRL_MCIO_DDRIOEXT_REG);

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValid(Host, Strobe)) {
        continue;
      }
      //
      //DATA FUB
      // Assumes CacheDDRIO has been run, so it makes use of the cached values for reads
      //
      DataControl3.Data = (*channelNvList)[Ch].dataControl3[Strobe];
      DataControl2.Data = (*channelNvList)[Ch].dataControl2[Strobe];
      DataControl3.Bits.drvstaticlegcfg = DdrCrCompCtl2.Bits.txdqstatlegen;
      DataControl3.Bits.odtstaticlegcfg = DdrCrCompOvr.Bits.odtstatlegen & (BIT1 + BIT0);
      DataControl2.Bits.odtstaticlegcfg = (DdrCrCompOvr.Bits.odtstatlegen & (BIT4 + BIT3 + BIT2)) >> 2;
      WriteDdrioDataRegCh (Host, Socket, Ch, Strobe, DATACONTROL3N0_0_MCIO_DDRIO_REG, DataControl3.Data);
      WriteDdrioDataRegCh (Host, Socket, Ch, Strobe, DATACONTROL2N0_0_MCIO_DDRIO_REG, DataControl2.Data);
      //
      //update cache
      //
      (*channelNvList)[Ch].dataControl3[Strobe] = DataControl3.Data;
      (*channelNvList)[Ch].dataControl2[Strobe] = DataControl2.Data;
    } //Strobe loop
    //
    //CMDCTLA/B FUBS: pins ddrcmdstatdflt<4:0> of cmdtransmitter
    //
    DdrCrCmdControls1Cmds.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDCONTROLS1_CMDS_MCIO_DDRIOEXT_REG);
    DdrCrCmdControls1Cmds.Bits.dqstatdflt = DdrCrCompStatLegTrainCtrl.Bits.cmdstatlegctrl;
    MemWritePciCfgEp (Socket, Ch, DDRCRCMDCONTROLS1_CMDS_MCIO_DDRIOEXT_REG, DdrCrCmdControls1Cmds.Data);

    DdrCrCmdControls1Ctl.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDCONTROLS1_CTL_MCIO_DDRIOEXT_REG);
    DdrCrCmdControls1Ctl.Bits.dqstatdflt = DdrCrCompStatLegTrainCtrl.Bits.ctlstatlegctrl;
    MemWritePciCfgEp (Socket, Ch, DDRCRCMDCONTROLS1_CTL_MCIO_DDRIOEXT_REG, DdrCrCmdControls1Ctl.Data);

    DdrCrCmdControls1Cmdn.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDCONTROLS1_CMDN_MCIO_DDRIOEXT_REG);
    DdrCrCmdControls1Cmdn.Bits.dqstatdflt = DdrCrCompStatLegTrainCtrl.Bits.cmdstatlegctrl;
    MemWritePciCfgEp (Socket, Ch, DDRCRCMDCONTROLS1_CMDN_MCIO_DDRIOEXT_REG, DdrCrCmdControls1Cmdn.Data);

    DdrCrCmdControls1Cke.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDCONTROLS1_CKE_MCIO_DDRIOEXT_REG);
    DdrCrCmdControls1Cke.Bits.dqstatdflt = DdrCrCompStatLegTrainCtrl.Bits.clkstatlegctrl;
    MemWritePciCfgEp (Socket, Ch, DDRCRCMDCONTROLS1_CKE_MCIO_DDRIOEXT_REG, DdrCrCmdControls1Cke.Data);
    //
    //CLK FUB: pins ddrdqstatdflt<4:0> & ddrodtstatdflt<4:0> of clktransmitter
    // note ddrCrClkControls.statlegen is the 2 LSB, and DdrCrClkTraining.extstatlegen is the 3 MSB of the field
    //
    DdrCRClkControls.Data = (*channelNvList)[Ch].ddrCRClkControls;
    DdrCrClkTraining.Data = (*channelNvList)[Ch].ddrCRClkTraining;
    DdrCRClkControls.Bits.statlegen = DdrCrCompStatLegTrainCtrl.Bits.clkstatlegctrl & (BIT1 + BIT0);
    DdrCrClkTraining.Bits.extstatlegen = (DdrCrCompStatLegTrainCtrl.Bits.clkstatlegctrl & (BIT4 + BIT3 + BIT2)) >> 2;
    MemWritePciCfgEp (Socket, Ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG, DdrCRClkControls.Data);
    MemWritePciCfgEp (Socket, Ch, DDRCRCLKTRAINING_MCIO_DDRIOEXT_REG, DdrCrClkTraining.Data);
    //
    //update cache
    //
    (*channelNvList)[Ch].ddrCRClkControls = DdrCRClkControls.Data;
    (*channelNvList)[Ch].ddrCRClkTraining = DdrCrClkTraining.Data;

#ifdef DEBUG_CODE_BLOCK
    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "DdrCrCompCtl2.txdqstatlegen: 0x%x \n", DdrCrCompCtl2.Bits.txdqstatlegen);
    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "DdrCrCompOvr.odtstatlegen: 0x%x \n", DdrCrCompOvr.Bits.odtstatlegen);
    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "DdrCrCompStatLegTrainCtrl.cmdstatlegctrl: 0x%x \n", DdrCrCompStatLegTrainCtrl.Bits.cmdstatlegctrl);
    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "DdrCrCompStatLegTrainCtrl.ctlstatlegctrl: 0x%x \n", DdrCrCompStatLegTrainCtrl.Bits.ctlstatlegctrl);
    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "DdrCrCompStatLegTrainCtrl.clkstatlegctrl: 0x%x \n", DdrCrCompStatLegTrainCtrl.Bits.clkstatlegctrl);
#endif //DEBUG_CODE_BLOCK
  } //Ch loop

  SetMemPhase (MemPhaseSave);
  return SUCCESS;
} //RcompStaticLegTraining

/**

  Single API to access, modify, increase or set –one or more registers– pertaining to a single MC, DDRIO or DRAM "knob"

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer, HBM)
  @param group   - CSM_GT - Parameter to program
  @param mode    - Bit-field of different modes
  @param value   - Pointer to delay value or offset based on mode

  @retval MRC_STATUS

**/
MRC_STATUS
GetSetDataGroup (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     SubCh,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value
  )
{
  MRC_STATUS  status = MRC_STATUS_SUCCESS;
  struct dimmNvram (*dimmNvList)[MAX_DIMM];

  //
  // During GetMargins it requires cached values
  //
  if (IsBrsPresent (Host, socket) && Host->ChannelModeOperation == ModeDdr5FullChannel) {
    mode = mode | GSM_READ_CSR;
    if (mode & GSM_UPDATE_CACHE) {
      mode = mode | GSM_FORCE_WRITE;
    }
  }

  if (IsBrsPresent (Host, socket) &&
      Host->ChannelModeOperation == ModeDdr5FullChannelPost40b &&
      strobe != ALL_STROBES
      ) {
    strobe = strobe % (MAX_STROBE_DDR5 / 2);
  }

  dimmNvList = GetDimmNvList (Host, socket, ch);
  switch (group) {
  case TxDqDelay:
  case TxDqsDelay:
  case TxDqsDelayCycle:
  case TxEq:
  case TxEqTap2:
  case WrLvlDelay:
    status = GetSetTxDelay (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
    break;

  case TxDqBitDelay:
    status = GetSetTxDelayBit (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
    break;

  case RxDqsBitDelay:
  case RxDqsPBitDelay:
  case RxDqsNBitDelay:
    status = GetSetRxDelayBit (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
    break;

  case RecEnDelay:
  case RecEnDelayCycle:
  case RxDqsPDelay:
  case RxDqsNDelay:
  case RxDqsDelay:
  case RxDqDelay:
  case RxDqDqsDelay:
  case RxDfeCoefficient:
  case RxStrobeInvert:
    status = GetSetRxDelay (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
    break;
  case RxVref:
    status = GetSetRxVref (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
    break;

  case PxcDelta:
  case DdjcDelta:
    status = GetSetPxcDdjcDelta (Host, socket, ch, dimm, rank, strobe, bit, group, mode, value);
    break;

  case TxVref:
    // Check for NVMDIMM Present and LrbufLevel, then call GetSetTxVrefFnv
    if ((*dimmNvList)[dimm].DcpmmPresent && level == LrbufLevel) {
      status = GetSetTxVrefFnv (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
    } else {
      status = GetSetTxVref (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
    }
    break;

  case TxImode:
    status = GetSetImode (Host, socket, ch, strobe, mode, value);
    break;

  case RxEq:
    status = GetSetRxDelay (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
    break;
  case RxCtleC:
  case RxCtleR:
    status = GetSetCTLE (Host, socket, ch, dimm, strobe, bit, level, group, mode, value);
    break;

  case RxOdt:
  case TxRon:
  case TxDsPd:
  case TxDsPu:
    status = GetSetCPUODT (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
    break;
#if defined (ICX_HOST) && defined (DDR5_SUPPORT)
  case RttNomWr:
  case RttNomRd:
  case RttWr:
  case RttPark:
  case DqsRttPark:
  case DramRonPullUp:
  case DramRonPullDown:
  case RttCkGroupA:
  case RttCkGroupB:
  case RttCsGroupA:
  case RttCsGroupB:
  case RttCaGroupA:
  case RttCaGroupB:
    status = GetSetDimmRttRonDdr5 (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
    break;
#else
  case DramDrvStr:
  case ParkOdt:
  case NomOdt:
  case WrOdt:
    status = GetSetDimmOdtDdr4 (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
    break;
#endif
  case TxRiseFallSlewRate:
  case TxFallSlewRate:
    status = GetSetTxRiseFallSlewRate (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
    break;
  case TxTco:
    status = GetSetTxTco (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
    break;
  case CmdTxEq:
    status = GetSetCmdTxEq (Host, socket, ch, mode, value);
    break;
  case PxcControl:
    status = GetSetPxcControl (Host, socket, ch, mode, value);
    break;
  case RxFlyBy:
    status = GetRxFlyByCore (Host, socket, ch, SubCh, dimm, rank, strobe, level, mode, value);
    break;
  case TxFlyBy:
    status = GetTxFlyByCore (Host, socket, ch, dimm, rank, strobe, level, mode, value);
    break;
  case RxSenseAmpOffset:
  case RxFifoDqsPBitSkew:
  case RxFifoDqsNBitSkew:
    status = GetSetSenseAmpRxOffsetDdrio (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
    break;
  case RxDqsPerChDelayOffset:
  case TxDqPerChDelayOffset:
    status = GetSetDelayMulticastOffset (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
    break;
  case DramRxEq:
    status = GetSetDdr4DramRxEq (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
    break;
#ifdef LRDIMM_SUPPORT
  case DbDfeTap1:
  case DbDfeTap2:
  case DbDfeTap3:
  case DbDfeTap4:
    status = GetSetDataBufferDfeTapPerBit (Host, socket, ch, dimm, rank, strobe, bit, level, group, mode, value);
    break;
#endif // LRDIMM_SUPPORT
  default:
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                   "GetSetDataGroup called unknown group!\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_53);
    break;
  }

  return status;
} // GetSetDataGroup


/**
  Read/write PXC DQ attackers for a strobe
  Each DQ pin has 0, 1 or 2 attacker DQs within the same byte.

  @param [in    ] Host       - Pointer to sysHost
  @param [in    ] Socket     - Socket number
  @param [in    ] Ch         - Channel number
  @param [in    ] Strobe     - Strobe number
  @param [in    ] Mode       - Bit-field of different access modes
  @param [in,out] AttackerN0 - PXC_ATTACKER_SEL structure for Nibble 0
  @param [in,out] AttackerN1 - PXC_ATTACKER_SEL structure for Nibble 1

  @retval SUCCESS
**/
UINT32
GetSetPxcAttackSel (
  IN     PSYSHOST         Host,
  IN     UINT8            Socket,
  IN     UINT8            Ch,
  IN     UINT8            Strobe,
  IN     UINT8            Mode,
  IN OUT PXC_ATTACKER_SEL AttackerN0,
  IN OUT PXC_ATTACKER_SEL AttackerN1
  )
{
  ATTACK0SEL_MCIO_DDRIO_STRUCT        Attacker0SelectReg[MAX_NIBBLES_PER_BYTE];
  UINT32                              BitValue[MAX_BITS_IN_BYTE];
  UINT8                               SwizzleBit[MAX_BITS_IN_BYTE];
  UINT8                               Index;
  UINT8                               StrobeN[MAX_NIBBLES_PER_BYTE];
  UINT8                               MSVx8 = GetMaxStrobeValid (Host) / 2;

  StrobeN[INDEX_NIBBLE0] = Strobe % MSVx8;
  StrobeN[INDEX_NIBBLE1] = (Strobe % MSVx8) + MSVx8;

  Attacker0SelectReg[INDEX_NIBBLE0].Data = ReadDdrioDataRegCh (Host, Socket, Ch, StrobeN[INDEX_NIBBLE0], ATTACK0SEL_MCIO_DDRIO_REG);
  Attacker0SelectReg[INDEX_NIBBLE1].Data = ReadDdrioDataRegCh (Host, Socket, Ch, StrobeN[INDEX_NIBBLE1], ATTACK0SEL_MCIO_DDRIO_REG);

  for (Index = 0; Index < MAX_BITS_IN_BYTE; Index++) {
    SwizzleBit[Index] = GetDqSwizzle (Ch, StrobeN[INDEX_NIBBLE0], Index, DdrLevel, PxcDelta);
  }
  //
  // Check if read only
  //
  if (Mode & (GSM_READ_ONLY)) {
    // Nibble 0
    BitValue[0] = Attacker0SelectReg[INDEX_NIBBLE0].Bits.attackr0d0;
    BitValue[1] = Attacker0SelectReg[INDEX_NIBBLE0].Bits.attackr0d1;
    BitValue[2] = Attacker0SelectReg[INDEX_NIBBLE0].Bits.attackr0d2;
    BitValue[3] = Attacker0SelectReg[INDEX_NIBBLE0].Bits.attackr0d3;
    // Nibble 1
    BitValue[4] = Attacker0SelectReg[INDEX_NIBBLE1].Bits.attackr0d0;
    BitValue[5] = Attacker0SelectReg[INDEX_NIBBLE1].Bits.attackr0d1;
    BitValue[6] = Attacker0SelectReg[INDEX_NIBBLE1].Bits.attackr0d2;
    BitValue[7] = Attacker0SelectReg[INDEX_NIBBLE1].Bits.attackr0d3;

    //
    // Nibble 0
    //
    AttackerN0.Bits.AttackerBit0 = BitValue [SwizzleBit[0]];
    AttackerN0.Bits.AttackerBit1 = BitValue [SwizzleBit[1]];
    AttackerN0.Bits.AttackerBit2 = BitValue [SwizzleBit[2]];
    AttackerN0.Bits.AttackerBit3 = BitValue [SwizzleBit[3]];
    //
    // Nibble 1
    //
    AttackerN1.Bits.AttackerBit0 = BitValue [SwizzleBit[4]];
    AttackerN1.Bits.AttackerBit1 = BitValue [SwizzleBit[5]];
    AttackerN1.Bits.AttackerBit2 = BitValue [SwizzleBit[6]];
    AttackerN1.Bits.AttackerBit3 = BitValue [SwizzleBit[7]];
  }

  //
  // Check for write
  //
  if (Mode & (GSM_FORCE_WRITE)) {
    //
    // Nibble 0
    //
    BitValue[0] = AttackerN0.Bits.AttackerBit0;
    BitValue[1] = AttackerN0.Bits.AttackerBit1;
    BitValue[2] = AttackerN0.Bits.AttackerBit2;
    BitValue[3] = AttackerN0.Bits.AttackerBit3;
    //
    // Nibble 1
    //
    BitValue[4] = AttackerN1.Bits.AttackerBit0;
    BitValue[5] = AttackerN1.Bits.AttackerBit1;
    BitValue[6] = AttackerN1.Bits.AttackerBit2;
    BitValue[7] = AttackerN1.Bits.AttackerBit3;

    //
    // Nibble 0
    //
    Attacker0SelectReg[INDEX_NIBBLE0].Bits.attackr0d0 = BitValue [SwizzleBit[0]];
    Attacker0SelectReg[INDEX_NIBBLE0].Bits.attackr0d1 = BitValue [SwizzleBit[1]];
    Attacker0SelectReg[INDEX_NIBBLE0].Bits.attackr0d2 = BitValue [SwizzleBit[2]];
    Attacker0SelectReg[INDEX_NIBBLE0].Bits.attackr0d3 = BitValue [SwizzleBit[3]];
    //
    // Nibble 1
    //
    Attacker0SelectReg[INDEX_NIBBLE1].Bits.attackr0d0 = BitValue [SwizzleBit[4]];
    Attacker0SelectReg[INDEX_NIBBLE1].Bits.attackr0d1 = BitValue [SwizzleBit[5]];
    Attacker0SelectReg[INDEX_NIBBLE1].Bits.attackr0d2 = BitValue [SwizzleBit[6]];
    Attacker0SelectReg[INDEX_NIBBLE1].Bits.attackr0d3 = BitValue [SwizzleBit[7]];

    WriteDdrioDataRegCh (Host, Socket, Ch, StrobeN[INDEX_NIBBLE0], ATTACK0SEL_MCIO_DDRIO_REG, Attacker0SelectReg[INDEX_NIBBLE0].Data);
    WriteDdrioDataRegCh (Host, Socket, Ch, StrobeN[INDEX_NIBBLE1], ATTACK0SEL_MCIO_DDRIO_REG, Attacker0SelectReg[INDEX_NIBBLE1].Data);
  }

  return SUCCESS;
}

/**
  Get and Set PXC Delta

  @param[in]      Host    - Pointer to sysHost
  @param[in]      Socket  - Socket number
  @param[in]      Ch      - Channel number (0-based)
  @param[in]      Dimm    - DIMM number (0-based)
  @param[in]      Rank    - Rank number (0-based)
  @param[in]      Strobe  - Strobe number (0-based)
  @param[in]      Bit     - Bit number (byte-scope)
  @param[in]      Group   - CSM_GT - Parameter to program
  @param[in]      Mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param[in,out]  Value   - Data to program

  @retval IO delay

**/
STATIC
MRC_STATUS
GetSetPxcDdjcDelta (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch,
  IN     UINT8     Dimm,
  IN     UINT8     Rank,
  IN     UINT8     Strobe,
  IN     UINT8     Bit,
  IN     MRC_GT    Group,
  IN     UINT8     Mode,
  IN OUT INT16     *Value
  )
{
  MRC_STATUS                            Status = MRC_STATUS_SUCCESS;
  UINT8                                 MaxStrobe;
  UINT8                                 TargetStrobe;
  UINT8                                 LogicalBit = 0;
  UINT8                                 IndexBit = 0;
  UINT8                                 BitStart = 0;
  UINT8                                 BitStop  = 0;
  UINT8                                 SwizzleBit = 0;
  UINT8                                 LogRank;
  UINT16                                CurValPxc[MAX_BITS_IN_BYTE];
  UINT16                                CurValDdjc[MAX_BITS_IN_BYTE];
  UINT16                                PiDelay[MAX_BITS_IN_BYTE];
  INT16                                 Tmp[MAX_BITS_IN_BYTE];
  UINT16                                MaxLimit = 127;
  UINT16                                MinLimit = 0;
  UINT16                                UsDelay = 1;
  BOOLEAN                               ValueOutOfRange;
  struct channelNvram                   (*ChannelNvList)[MAX_CH];
  XTALKDELTARANK0_MCIO_DDRIO_STRUCT     XtalkDeltaN0;
  XTALKDELTARANK0_MCIO_DDRIO_STRUCT     XtalkDeltaN1;
  struct rankDevice                     (*RankStruct)[MAX_RANK_DIMM];
  struct ddrRank                        (*RankList)[MAX_RANK_DIMM];
  struct dimmNvram                      (*DimmNvList)[MAX_DIMM];
  UINT8                                 MSVx8 = GetMaxStrobeValid (Host) / 2;

  ChannelNvList = GetChannelNvList (Host, Socket);
  RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);
  RankList = GetRankNvList (Host, Socket, Ch, Dimm);
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  //
  // Get the logical rank #
  //
  LogRank = GetLogicalRank(Host, Socket, Ch, Dimm, Rank);

  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits (Host, DdrLevel, Group, &MinLimit, &MaxLimit, &UsDelay);

  if (Bit == ALL_BITS) {
    BitStart = 0;
    BitStop  = MAX_BITS_IN_BYTE;
  } else {
    BitStart = 0;
    BitStop  = 1;
  }

  //
  // Clear all variables
  //
  for (IndexBit = 0; IndexBit < MAX_BITS_IN_BYTE; IndexBit++) {
    CurValPxc[IndexBit] = 0;
    CurValDdjc[IndexBit] = 0;
    PiDelay[IndexBit] = 0;
    Tmp[IndexBit] = 0;
  }

  if (Strobe == ALL_STROBES) {
    Strobe = 0;
    MaxStrobe = MAX_STROBE / 2;
  } else {
    Strobe = Strobe % (MAX_STROBE / 2);
    MemCheckIndex (Strobe, MAX_STROBE / 2);
    MaxStrobe = Strobe + 1;
  }

  for (; Strobe < MaxStrobe; Strobe++) {

    TargetStrobe = Strobe;
    if (Bit != ALL_BITS) {
      //
      // Determine swizzled bit position within the nibble
      //
      LogicalBit = Bit % BITS_PER_NIBBLE;
      //
      // If the bit is >= 4, then it's a bit in the upper nibble
      //
      if (Bit >= BITS_PER_NIBBLE) {
        TargetStrobe += 9;
      }
      //
      // Get DQ swizzle bit
      //
      SwizzleBit = GetDqSwizzle (Ch, TargetStrobe, LogicalBit, DdrLevel, Group);
      //
      // bit != ALL_BIT
      // Loop should only target the SwizzleBit
      //
      BitStart = SwizzleBit;
      BitStop  = SwizzleBit + 1;
    }

    if (Mode & GSM_READ_CSR) {
      //
      // Read from PCI config space
      //
      XtalkDeltaN0.Data = ReadDdrioDataReg (Host, Socket, Ch, LogRank, (Strobe % MSVx8), XTALKDELTARANK0_MCIO_DDRIO_REG);
      XtalkDeltaN1.Data = ReadDdrioDataReg (Host, Socket, Ch, LogRank, (Strobe % MSVx8) + MSVx8, XTALKDELTARANK0_MCIO_DDRIO_REG);
    } else {
      //
      // Read from cache
      //
      XtalkDeltaN0.Data = (UINT32)(*ChannelNvList)[Ch].XtalkDeltaN0[LogRank][Strobe % MSVx8];
      XtalkDeltaN1.Data = (UINT32)(*ChannelNvList)[Ch].XtalkDeltaN1[LogRank][(Strobe % MSVx8) + MSVx8];
    }

    CurValPxc[0] = (UINT16)XtalkDeltaN0.Bits.xtalkdeltabit0; // Bit 0
    CurValPxc[1] = (UINT16)XtalkDeltaN0.Bits.xtalkdeltabit1; // Bit 1
    CurValPxc[2] = (UINT16)XtalkDeltaN0.Bits.xtalkdeltabit2; // Bit 2
    CurValPxc[3] = (UINT16)XtalkDeltaN0.Bits.xtalkdeltabit3; // Bit 3
    CurValPxc[4] = (UINT16)XtalkDeltaN1.Bits.xtalkdeltabit0; // Bit 4
    CurValPxc[5] = (UINT16)XtalkDeltaN1.Bits.xtalkdeltabit1; // Bit 5
    CurValPxc[6] = (UINT16)XtalkDeltaN1.Bits.xtalkdeltabit2; // Bit 6
    CurValPxc[7] = (UINT16)XtalkDeltaN1.Bits.xtalkdeltabit3; // Bit 7

    CurValDdjc[0] = (UINT16)XtalkDeltaN0.Bits.ddjcdeltabit0; // Bit 0
    CurValDdjc[1] = (UINT16)XtalkDeltaN0.Bits.ddjcdeltabit1; // Bit 1
    CurValDdjc[2] = (UINT16)XtalkDeltaN0.Bits.ddjcdeltabit2; // Bit 2
    CurValDdjc[3] = (UINT16)XtalkDeltaN0.Bits.ddjcdeltabit3; // Bit 3
    CurValDdjc[4] = (UINT16)XtalkDeltaN1.Bits.ddjcdeltabit0; // Bit 4
    CurValDdjc[5] = (UINT16)XtalkDeltaN1.Bits.ddjcdeltabit1; // Bit 5
    CurValDdjc[6] = (UINT16)XtalkDeltaN1.Bits.ddjcdeltabit2; // Bit 6
    CurValDdjc[7] = (UINT16)XtalkDeltaN1.Bits.ddjcdeltabit3; // Bit 7

    if (Group == DdjcDelta) {
      PiDelay[0] = CurValDdjc[0];
      PiDelay[1] = CurValDdjc[1];
      PiDelay[2] = CurValDdjc[2];
      PiDelay[3] = CurValDdjc[3];
      PiDelay[4] = CurValDdjc[4];
      PiDelay[5] = CurValDdjc[5];
      PiDelay[6] = CurValDdjc[6];
      PiDelay[7] = CurValDdjc[7];
    } else {
      PiDelay[0] = CurValPxc[0];
      PiDelay[1] = CurValPxc[1];
      PiDelay[2] = CurValPxc[2];
      PiDelay[3] = CurValPxc[3];
      PiDelay[4] = CurValPxc[4];
      PiDelay[5] = CurValPxc[5];
      PiDelay[6] = CurValPxc[6];
      PiDelay[7] = CurValPxc[7];
    }

    //
    // Check if read only
    //
    if (Mode & GSM_READ_ONLY) {
      //
      // Save the value read into the output parameter
      //
      if (Group == DdjcDelta) {
        *Value = CurValDdjc[SwizzleBit];
      } else {
        *Value = CurValPxc[SwizzleBit];
      }
    } else {
      //
      // Write
      // Assume failure by setting ValutOutOfRange to TRUE. If ANY bit is in range, set ValutOutOfRange to FALSE.
      //
      ValueOutOfRange = TRUE;
      for (IndexBit = BitStart; IndexBit < BitStop; IndexBit++) {
        // Adjust the current value by offset
        if (Mode & GSM_WRITE_OFFSET) {
          //
          // Add the offset to the current value
          //
          if (Group == DdjcDelta) {
            Tmp[IndexBit] = CurValDdjc[IndexBit] + *Value;
          } else {
            Tmp[IndexBit] = CurValPxc[IndexBit] + *Value;
          }
        } else {
          Tmp[IndexBit] = *Value;
        }
        //
        // Make sure we do not exeed the limits
        //
        if (Tmp[IndexBit] >= MinLimit) {
          PiDelay[IndexBit] = Tmp[IndexBit];
        } else {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, Bit,
                          "%a Out of range!! tmp = 0x%x, minLimit = 0x%x\n", GetGroupStr (Group), Tmp[IndexBit], MinLimit);
          PiDelay[IndexBit] = MinLimit;
        }

        //
        // Ensure we do not exceed maximums
        //
        if (PiDelay[IndexBit] > MaxLimit) {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, Bit,
                          "%a Out of range!! piDelay = 0x%x, maxLimit = 0x%x\n", GetGroupStr (Group), PiDelay[IndexBit], MaxLimit);
          PiDelay[IndexBit] = MaxLimit;
        }

        //
        // When setting DdjcDelta, validate that DdjcDelta + XtalkDelta does not exceed MAX_DDJC_PLUS_PXC.
        //
        if (Group != DdjcDelta) {
          //
          // "ValueOutOfRange==TRUE" only valid for DdjcDelta. Always set it to FALSE for PxcDelta.
          //
          ValueOutOfRange = FALSE;
        } else {
          if ((PiDelay[IndexBit] + CurValPxc[IndexBit]) > MAX_DDJC_PLUS_PXC) {
            //
            // This debug message is disabled because it is expected to appear frequently under typical circumstances. It is not an error.
            //
            RcDebugPrintWithDevice (SDBG_NONE, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, Bit == ALL_BITS ? IndexBit : Bit,
                            "DdjcDelta(0x%x) + PxcDelta(0x%x) out of range!! Capping DdjcDelta at 0x%x\n", PiDelay[IndexBit], CurValPxc[IndexBit], MAX_DDJC_PLUS_PXC - CurValPxc[IndexBit]);
            PiDelay[IndexBit] = MAX_DDJC_PLUS_PXC - CurValPxc[IndexBit];
          } else {
            //
            // If any of the bits are in range, even if some are not, indicate success
            //
            ValueOutOfRange = FALSE;
          }
        }
      } // IndexBit
      if (Bit != ALL_BITS) {
        //
        // Update the return value in case it is different from the value passed in
        // This is useful if the caller prints out the return value after calling with a value that is
        // found to be out of range.
        //
        *Value = PiDelay[SwizzleBit];
      }
      if (ValueOutOfRange) {
        Status = MRC_STATUS_FAILURE;
      }

      if (Group == DdjcDelta) {
        XtalkDeltaN0.Bits.ddjcdeltabit0 = PiDelay[0]; // Bit 0
        XtalkDeltaN0.Bits.ddjcdeltabit1 = PiDelay[1]; // Bit 1
        XtalkDeltaN0.Bits.ddjcdeltabit2 = PiDelay[2]; // Bit 2
        XtalkDeltaN0.Bits.ddjcdeltabit3 = PiDelay[3]; // Bit 3
        XtalkDeltaN1.Bits.ddjcdeltabit0 = PiDelay[4]; // Bit 4
        XtalkDeltaN1.Bits.ddjcdeltabit1 = PiDelay[5]; // Bit 5
        XtalkDeltaN1.Bits.ddjcdeltabit2 = PiDelay[6]; // Bit 6
        XtalkDeltaN1.Bits.ddjcdeltabit3 = PiDelay[7]; // Bit 7
      } else {
        XtalkDeltaN0.Bits.xtalkdeltabit0 = PiDelay[0]; // Bit 0
        XtalkDeltaN0.Bits.xtalkdeltabit1 = PiDelay[1]; // Bit 1
        XtalkDeltaN0.Bits.xtalkdeltabit2 = PiDelay[2]; // Bit 2
        XtalkDeltaN0.Bits.xtalkdeltabit3 = PiDelay[3]; // Bit 3
        XtalkDeltaN1.Bits.xtalkdeltabit0 = PiDelay[4]; // Bit 4
        XtalkDeltaN1.Bits.xtalkdeltabit1 = PiDelay[5]; // Bit 5
        XtalkDeltaN1.Bits.xtalkdeltabit2 = PiDelay[6]; // Bit 6
        XtalkDeltaN1.Bits.xtalkdeltabit3 = PiDelay[7]; // Bit 7
      }

      //
      // Write it back if the current data does not equal the cache value or if in force write mode
      //
      if ((XtalkDeltaN0.Data != (UINT32)(*ChannelNvList)[Ch].XtalkDeltaN0[LogRank][Strobe % MSVx8]) ||
        (XtalkDeltaN1.Data != (UINT32)(*ChannelNvList)[Ch].XtalkDeltaN1[LogRank][(Strobe % MSVx8) + MSVx8]) || (Mode & GSM_FORCE_WRITE)) {

        WriteDdrioDataReg (Host, Socket, Ch, LogRank, (Strobe % MSVx8), XTALKDELTARANK0_MCIO_DDRIO_REG, XtalkDeltaN0.Data);
        WriteDdrioDataReg (Host, Socket, Ch, LogRank, (Strobe % MSVx8) + MSVx8, XTALKDELTARANK0_MCIO_DDRIO_REG, XtalkDeltaN1.Data);
      }
    } // if read only

    //
    // Save to cache
    //
    if (Mode & GSM_UPDATE_CACHE) {
      (*ChannelNvList)[Ch].XtalkDeltaN0[LogRank][Strobe % MSVx8]        = (UINT16)(XtalkDeltaN0.Data & 0xFFFF);
      (*ChannelNvList)[Ch].XtalkDeltaN1[LogRank][(Strobe % MSVx8) + MSVx8]  = (UINT16)(XtalkDeltaN1.Data & 0xFFFF);
    }
  } // Strobe loop

  return Status;
} // GetSetPxcDdjcDelta

/**

  Translate the register value to TxEQCode acording to: EqCode1=
  (TxEQ * (( DrvStaticLegCfg + (rcompcode/3))*
  DataSegmentEnable)) / 128
  Translate the TxEQCode to register value acording to: TxEQ   =
  (128*EqCode1)/(( DrvStaticLegCfg + (rcompcode/3))*
  DataSegmentEnable)

  @param Host      - Pointer to sysHost
  @param Socket    - Socket
  @param Ch        - Channel
  @param Strobe    - Strobe
  @param RegVal    - RegVal to translate to EqCode
  @param Direction - Direction 1: RegVal->EqCode 0:
                   EqCode->RegVal

  @retval UINT8

**/
STATIC
UINT8
RegVal2EqCode (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Strobe,
  IN UINT8     Value,
  IN BOOLEAN   Direction
)
{
  UINT8                EqCode;
  UINT8                RegVal;
  UINT8                DrvStaticLegCfg;
  UINT8                DataSegmentEnable;
  UINT8                RcompCode;
  struct channelNvram  (*ChannelNvList)[MAX_CH];
  UINT8                NumDataSegmentEnable = 0;
  UINT8                NumDrvStaticLegCfg = 0;

  ChannelNvList = GetChannelNvList(Host, Socket);
  DataSegmentEnable = (*ChannelNvList)[Ch].DataSegmentEnable[Strobe];
  RcompCode         = (*ChannelNvList)[Ch].RcompCode[Strobe];
  DrvStaticLegCfg   = (*ChannelNvList)[Ch].DrvStaticLegCfg[Strobe];

  while (DataSegmentEnable != 0) {
    DataSegmentEnable = DataSegmentEnable & (DataSegmentEnable - 1);
    NumDataSegmentEnable += 1;
  }
  while (DrvStaticLegCfg != 0) {
    DrvStaticLegCfg = DrvStaticLegCfg & (DrvStaticLegCfg - 1);
    NumDrvStaticLegCfg += 1;
  }

  if (Direction == REG_TO_TXEQ) {
    RegVal = Value;
    EqCode = (UINT8)(((RegVal * ((NumDrvStaticLegCfg + (RcompCode / TXEQ_FACTOR2)) * NumDataSegmentEnable))) / TXEQ_FACTOR1);
    Value  = EqCode;
  } else {
    EqCode = Value;
    //
    // Apply the formula and avoid division by 0
    //
    if (((NumDrvStaticLegCfg + (RcompCode / TXEQ_FACTOR2)) * NumDataSegmentEnable) == 0) {
      RegVal = 0;
    } else {
      RegVal = (UINT8)((TXEQ_FACTOR1 * EqCode) / ((NumDrvStaticLegCfg + (RcompCode / TXEQ_FACTOR2)) * NumDataSegmentEnable));
    }
    Value  = RegVal;
  }

  return Value;
}

/**

  Determine if it is appropriate to use the multicast offset register

  @param[in]     Strobe  - Strobe number (0-based)
  @param[in]     Bit     - Bit number
  @param[in]     Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]     Group   - CSM_GT - Parameter to program
  @param[in]     Mode    - Bit-field of different modes

  @retval TRUE  - OK to use multicast registers
          FALSE - Cannot use multicast register
**/
BOOLEAN
UseDelayMulticastOffset (
  IN      UINT8     Strobe,
  IN      UINT8     Bit,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode
  )
{
  //
  // Don't use multicast registers if the feature is disabled in the build
  //
  if (!FeaturePcdGet (PcdUseRxTxMultiCastRegisters)) {
    return FALSE;
  }

  //
  // Can use multicast registers if these are all true:
  // - Group is TxDqDelay or RxDqsDelay
  // - All strobes and all bits are being set to the same value
  // - Targeting front-side (host) registers
  // - Writing an offset
  //
  if (((Group != TxDqDelay) && (Group != RxDqsDelay)) ||
      (Strobe != ALL_STROBES)                         ||
      (Bit    != ALL_BITS)                            ||
      (Level  != DdrLevel)                            ||
      ((Mode & GSM_WRITE_OFFSET) == 0)) {
    //
    // Can't use multicast register
    //
    return FALSE;
  }
  return TRUE;
}

/**

  Get/Set TxDq and RxDqs Delay using multicast offset registers.
  The value written to the multicast registers used in this routine is added to
  the unicast register values within the channel. The value does not replace the
  value programmed in the unicast registers.

  @param[in]     Host    - Pointer to sysHost
  @param[in]     Socket  - Socket number
  @param[in]     Ch      - Channel number (0-based)
  @param[in]     Dimm    - DIMM number (0-based)
  @param[in]     Rank    - Rank number (0-based)
  @param[in]     Strobe  - Strobe number (0-based)
  @param[in]     Bit     - Bit number
  @param[in]     Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]     Group   - CSM_GT - Parameter to program
  @param[in]     Mode    - Bit-field of different modes
  @param[in,out] Value   - Pointer to delay value or offset based on mode

  @retval SUCCESS

**/
MRC_STATUS
GetSetDelayMulticastOffset (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      UINT8     Strobe,
  IN      UINT8     Bit,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  )
{
  UINT16                                    MaxLimit = 0;
  UINT16                                    MinLimit = 0;
  UINT16                                    UsDelay = 0;
  UINT16                                    PiDelay;
  UINT16                                    CurrentValue = 0;
  UINT16                                    NewValue;
  struct channelNvram                       (*ChannelNvList)[MAX_CH];
  DATATRAINFEEDBACK_MC_DDRIOMC_STRUCT       DataTrainFeedback;

  //
  // Don't use multicast registers if:
  // - The feature is disabled in the build
  // - Signal group is not supported
  // - Not targeting all strobes and bits
  // - Level is not DdrLevel (frontside)
  //
  if ((!FeaturePcdGet (PcdUseRxTxMultiCastRegisters)) ||
      ((Group != RxDqsPerChDelayOffset) && (Group != TxDqPerChDelayOffset)) ||
      (Strobe != ALL_STROBES)                         ||
      (Bit    != ALL_BITS)                            ||
      (Level  != DdrLevel)) {
    return MRC_STATUS_FAILURE;
  }

  ChannelNvList = GetChannelNvList (Host, Socket);
  DataTrainFeedback.Data = 0;
  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits (Host, DdrLevel, Group, &MinLimit, &MaxLimit, &UsDelay);

  if (Mode & GSM_READ_CSR) {
    //
    // Read from PCI config space
    //
    DataTrainFeedback.Data = MemReadPciCfgEp (Socket, Ch, DATATRAINFEEDBACK_MC_DDRIOMC_REG);
  } else {
    //
    // Read from cache
    //
    DataTrainFeedback.Data = (*ChannelNvList)[Ch].DataTrainFeedbackMultiCast;
  }

  switch (Group) {
    case TxDqPerChDelayOffset:
      CurrentValue = (UINT16)DataTrainFeedback.Bits.txdqpdoffset;
      break;
    case RxDqsPerChDelayOffset:
      CurrentValue = (UINT16)DataTrainFeedback.Bits.rxdqspdoffset;
      break;
    default:
      break;
  } // switch

  //
  // Check if read only
  //
  if (Mode & GSM_READ_ONLY) {
    *Value = CurrentValue;
  } else {
    //
    // Write
    //

    // Adjust the current CMD delay value by offset
    if (Mode & GSM_WRITE_OFFSET) {
      NewValue = CurrentValue + *Value;
    } else {
      NewValue = *Value;
    }

    if (NewValue >= MinLimit) {
      PiDelay = NewValue;
    } else {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "%a Multicast Out of range!! NewValue = 0x%x, MinLimit = 0x%x\n", GetGroupStr (Group), NewValue, MinLimit);
      PiDelay = MinLimit;
    }

    // Ensure we do not exceed maximums
    if (PiDelay > MaxLimit) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "%a Multicast Out of range!! piDelay = 0x%x, MaxLimit = 0x%x\n", GetGroupStr (Group), PiDelay, MaxLimit);
      PiDelay = MaxLimit;
    }

    switch (Group) {
      case TxDqPerChDelayOffset:
        DataTrainFeedback.Bits.txdqpdoffset = PiDelay;
        break;
      case RxDqsPerChDelayOffset:
        DataTrainFeedback.Bits.rxdqspdoffset = PiDelay;
        break;
      default:
        break;
    } // switch

    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
    if ((DataTrainFeedback.Data != (*ChannelNvList)[Ch].DataTrainFeedbackMultiCast) || (Mode & GSM_FORCE_WRITE)) {
      MemWritePciCfgEp (Socket, Ch, DATATRAINFEEDBACK_MC_DDRIOMC_REG, DataTrainFeedback.Data);
    }

    //
    // Wait for the new value to settle
    //
    FixedDelayMicroSecond (UsDelay);
  } // if read only

  //
  // Save to cache
  //
  if (Mode & GSM_UPDATE_CACHE) {
    (*ChannelNvList)[Ch].DataTrainFeedbackMultiCast = DataTrainFeedback.Data;
  }

  return MRC_STATUS_SUCCESS;
} // GetSetDelayMulticastOffset

/**

  Get TxDelay using unicast (single point) register

  @param[in]     Host    - Pointer to sysHost
  @param[in]     Socket  - Socket number
  @param[in]     Ch      - Channel number (0-based)
  @param[in]     Dimm    - DIMM number (0-based)
  @param[in]     Rank    - Rank number (0-based)
  @param[in]     Strobe  - Strobe number (0-based)
  @param[in]     Bit     - Bit number
  @param[in]     Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]     Group   - CSM_GT - Parameter to program
  @param[in]     Mode    - Bit-field of different modes
  @param[in,out] Value   - Pointer to delay value or offset based on mode

  @retval IO delay

**/
MRC_STATUS
GetSetTxDelayUnicast (
  IN      PSYSHOST  Host,
  IN      UINT8     socket,
  IN      UINT8     ch,
  IN      UINT8     dimm,
  IN      UINT8     rank,
  IN      UINT8     strobe,
  IN      UINT8     bit,
  IN      MRC_LT    level,
  IN      MRC_GT    group,
  IN      UINT8     mode,
  IN OUT  INT16     *value
  )
{
  MRC_STATUS                                Status;
  UINT8                                     logRank;
  UINT8                                     maxStrobe;
  UINT16                                    curVal = 0;
  UINT16                                    curVal2 = 0;
  UINT16                                    piDelay;
  INT16                                     tmp;
  UINT16                                    piDelay2 = 0;
  INT16                                     tmp2;
  UINT16                                    maxLimit = 0;
  UINT16                                    minLimit = 0;
  UINT16                                    usDelay = 0;
  struct channelNvram                       (*channelNvList)[MAX_CH];
  TXGROUP0N0RANK0_0_MCIO_DDRIO_STRUCT       txGroup0;
  DATATXEQCOEF_MCIO_DDRIO_STRUCT            TxEqCoefTap2;
  BOOLEAN                                   CompNeeded;
  INT16                                     ValueArray[BITS_PER_NIBBLE];
  UINT8                                     Index;
#ifdef LRDIMM_SUPPORT
  UINT8                                     updateNeeded;
  UINT8                                     tmpStrobe;
  UINT8                                     *controlWordDataPtr;
  UINT8                                     controlWordAddr;
  struct dimmNvram                          (*dimmNvList)[MAX_DIMM];
  struct rankDevice                         (*rankStruct)[MAX_RANK_DIMM];
  UINT8 MSVx8 = GetMaxStrobeValid(Host) / 2;
#endif //LRDIMM_SUPPORT
  UINT8 MSVx4 = GetMaxStrobeValid(Host);

  channelNvList = GetChannelNvList(Host, socket);
  CompNeeded = FALSE;

#ifdef LRDIMM_SUPPORT
  rankStruct  = GetRankStruct(Host, socket, ch, dimm);
  dimmNvList = GetDimmNvList(Host, socket, ch);
#endif //LRDIMM_SUPPORT

  txGroup0.Data = 0;
  TxEqCoefTap2.Data = 0;

  if (level == DdrLevel && group == TxDqDelay) {
    for (Index = 0; Index < sizeof (ValueArray) / sizeof (ValueArray[0]); Index++) {
      ValueArray[Index] = *value;
    }
    Status = GetSetTxDelayBit (Host, socket, ch, dimm, rank, strobe, ALL_BITS, DdrLevel, TxDqBitDelay, mode, ValueArray);
    *value = ValueArray[0];
    return Status;
  }
  //
  // Get the logical rank #
  //
  logRank = GetLogicalRank(Host, socket, ch, dimm, rank);

  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits(Host, level, group, &minLimit, &maxLimit, &usDelay);

  if (strobe >= MAX_STROBE && strobe != ALL_STROBES) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                   "The strobe value is out of range!\n");
    RC_FATAL_ERROR ((strobe < MAX_STROBE || strobe == ALL_STROBES), ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  if (strobe == ALL_STROBES) {
    strobe      = 0;
    maxStrobe   = MSVx4;
  } else {
    maxStrobe = strobe + 1;
  }

#if defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)
  updateNeeded = 0;
  if (((*dimmNvList)[dimm].lrBuf_BC1x & LRDIMM_ONE_RANK_TIMING_MODE) && (level == LrbufLevel)) {
    rank = 0;
    logRank = GetLogicalRank(Host, socket, ch, dimm, 0);
  }
#endif // defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)

  for ( ; strobe < maxStrobe; strobe++) {

#ifdef LRDIMM_SUPPORT
    if (level == LrbufLevel) {
      if (mode & GSM_READ_CSR) {
        //
        // Read from "actual setting" cache
        //
        switch (group) {
        case TxDqDelay:
          if ((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[strobe] < 16) {
            curVal = (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[strobe] + 15;
          } else {
            curVal = (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[strobe] ^ 31;
          }
          break;
        case TxDqsDelay:
          curVal = (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCAxBx[strobe];
          break;
        case TxDqsDelayCycle:
          curVal = (((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCDxFx[strobe % MSVx8] >> (4 * (strobe / MSVx8))) & 0xF);
          break;
        default:
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                         "GetSetTxDelay called unsupported group!\n");
          RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_54);
          break;
        } //switch group
      } else {
        //
        // Read from setting cache
        //
        switch (group) {
        case TxDqDelay:
          if ((*rankStruct)[rank].cachedLrBuf_FxBC8x9x[strobe] < 16) {
            curVal = (*rankStruct)[rank].cachedLrBuf_FxBC8x9x[strobe] + 15;
          } else {
            curVal = (*rankStruct)[rank].cachedLrBuf_FxBC8x9x[strobe] ^ 31;
          }
          break;
        case TxDqsDelay:
          curVal = (*rankStruct)[rank].cachedLrBuf_FxBCAxBx[strobe];
          break;
        case TxDqsDelayCycle:
          curVal = (((*rankStruct)[rank].cachedLrBuf_FxBCDxFx[strobe % MSVx8] >> (4 * (strobe / MSVx8))) & 0xF);
          break;
        default:
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                         "GetSetTxDelay called unsupported group!\n");
          RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL,  RC_FATAL_ERROR_MINOR_CODE_54);
          break;
        } //switch group
      }
    } else
#endif //LRDIMM_SUPPORT
    {

      if (mode & GSM_READ_CSR) {
        //
        // Read from PCI config space
        //
        if ((group == TxDqsDelay) || (group == WrLvlDelay) || (group == TxEq)) {
          txGroup0.Data = ReadDdrioDataReg (Host, socket, ch, logRank, strobe, TXGROUP0N0RANK0_0_MCIO_DDRIO_REG);
        }

        if (group == TxEqTap2) {
          TxEqCoefTap2.Data = ReadDdrioDataRegCh (Host, socket, ch, strobe, DATATXEQCOEF_MCIO_DDRIO_REG);
        }

      } else {
        //
        // Read from cache
        //
        if ((group == TxDqsDelay) || (group == WrLvlDelay) || (group == TxEq)) {
          txGroup0.Data = (*channelNvList)[ch].txGroup0[logRank][strobe];
        }

        if (group == TxEqTap2) {
          TxEqCoefTap2.Bits.rank0code = (*channelNvList)[ch].TxEqCoefTap2[0][strobe];
          TxEqCoefTap2.Bits.rank1code = (*channelNvList)[ch].TxEqCoefTap2[1][strobe];
          TxEqCoefTap2.Bits.rank2code = (*channelNvList)[ch].TxEqCoefTap2[2][strobe];
          TxEqCoefTap2.Bits.rank3code = (*channelNvList)[ch].TxEqCoefTap2[3][strobe];
          TxEqCoefTap2.Bits.rank4code = (*channelNvList)[ch].TxEqCoefTap2[4][strobe];
          TxEqCoefTap2.Bits.rank5code = (*channelNvList)[ch].TxEqCoefTap2[5][strobe];
          TxEqCoefTap2.Bits.rank6code = (*channelNvList)[ch].TxEqCoefTap2[6][strobe];
          TxEqCoefTap2.Bits.rank7code = (*channelNvList)[ch].TxEqCoefTap2[7][strobe];
        }

      }

      switch (group) {
        case WrLvlDelay:
          // get the strobe PI
          if (group == WrLvlDelay){
            curVal2 = (UINT16)txGroup0.Bits.txdqsdelay;
          }
          break;

        case TxDqsDelay:
          curVal = (UINT16)txGroup0.Bits.txdqsdelay;
          break;

        case TxEq:
          curVal = RegVal2EqCode (Host, socket, ch, strobe, (UINT8) txGroup0.Bits.txeq, REG_TO_TXEQ);
          break;

        case TxEqTap2:
          switch (logRank) {
          case 0:
            curVal = RegVal2EqCode (Host, socket, ch, strobe, (UINT8) TxEqCoefTap2.Bits.rank0code, REG_TO_TXEQ);
            break;
          case 1:
            curVal = RegVal2EqCode (Host, socket, ch, strobe, (UINT8) TxEqCoefTap2.Bits.rank1code, REG_TO_TXEQ);
            break;
          case 2:
            curVal = RegVal2EqCode (Host, socket, ch, strobe, (UINT8) TxEqCoefTap2.Bits.rank2code, REG_TO_TXEQ);
            break;
          case 3:
            curVal = RegVal2EqCode (Host, socket, ch, strobe, (UINT8) TxEqCoefTap2.Bits.rank3code, REG_TO_TXEQ);
            break;
          case 4:
            curVal = RegVal2EqCode (Host, socket, ch, strobe, (UINT8) TxEqCoefTap2.Bits.rank4code, REG_TO_TXEQ);
            break;
          case 5:
            curVal = RegVal2EqCode (Host, socket, ch, strobe, (UINT8) TxEqCoefTap2.Bits.rank5code, REG_TO_TXEQ);
            break;
          case 6:
            curVal = RegVal2EqCode (Host, socket, ch, strobe, (UINT8) TxEqCoefTap2.Bits.rank6code, REG_TO_TXEQ);
            break;
          case 7:
            curVal = RegVal2EqCode (Host, socket, ch, strobe, (UINT8) TxEqCoefTap2.Bits.rank7code, REG_TO_TXEQ);
            break;
          default:
            // return error on parameter
            break;
          }
          break;

        default:
          break;
      } // switch
    }

    //
    // Check if read only
    //
    if (mode & GSM_READ_ONLY) {
      if (group == WrLvlDelay){
        *value = curVal2; // return DQS value for wrlvlDelay
      } else {
        *value = curVal;
      }
    } else {
      //
      // Write
      //

      // Adjust the current CMD delay value by offset
      if (mode & GSM_WRITE_OFFSET) {
        tmp = curVal + *value;
      } else {
        tmp = *value;
      }

      if (tmp >= minLimit) {
        piDelay = tmp;
      } else {
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                        "%a Out of range!! tmp = 0x%x, minLimit = 0x%x\n", GetGroupStr (group), tmp, minLimit);
        piDelay = minLimit;
      }

      // Ensure we do not exceed maximums
      if (piDelay > maxLimit) {
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                        "%a Out of range!! piDelay = 0x%x, maxLimit = 0x%x\n", GetGroupStr (group), piDelay, maxLimit);
        piDelay = maxLimit;
      }

      if (group == WrLvlDelay){
        // Adjust the current CMD delay value by offset
        if (mode & GSM_WRITE_OFFSET) {
          tmp2 = curVal2 + *value;
        } else {
          tmp2 = *value;
        }

        if (tmp2 >= minLimit) {
          piDelay2 = tmp2;
        } else {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                          "%a Out of range!! tmp2 = 0x%x, minLimit = 0x%x\n", GetGroupStr (group), tmp2, minLimit);
          piDelay2 = minLimit;
        }
        // Ensure we do not exceed maximums
        if (piDelay2 > maxLimit) {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                          "%a Out of range!! piDelay2 = 0x%x, maxLimit = 0x%x\n", GetGroupStr (group), piDelay2, maxLimit);
          piDelay2 = maxLimit;
        }
      }

#ifdef LRDIMM_SUPPORT
      if (level == LrbufLevel) {
        curVal = piDelay;
        switch (group) {
          case TxDqDelay:
            if (curVal < 16) {
              if ((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[strobe] != (UINT8) (curVal ^ 31)) {
                updateNeeded = 1;
                (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[strobe] = (UINT8) (curVal ^ 31);
              }
            } else {
              if ((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[strobe] != (UINT8) (curVal - 15)) {
                updateNeeded = 1;
                (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[strobe] = (UINT8) (curVal - 15);
              }
            }
            break;
          case TxDqsDelay:
            if ((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCAxBx[strobe] != (UINT8) curVal) {
              updateNeeded = 1;
              (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCAxBx[strobe] = (UINT8) curVal;
            }
            break;
          case TxDqsDelayCycle:
            if ((((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCDxFx[strobe % MSVx8]>>(4 * (strobe / MSVx8))) & 0xF) != (UINT8) curVal) {
              updateNeeded = 1;
              (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCDxFx[strobe % MSVx8] &= 0xF0 >> (4 * (strobe / MSVx8));
              (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCDxFx[strobe % MSVx8] |= (UINT8) curVal<<(4 * (strobe / MSVx8));
            }
            break;
          default:
            break;
        }
      } else
#endif  //LRDIMM_SUPPORT
      {
        switch (group) {
          case WrLvlDelay:
            // update txdqs also
            if (group == WrLvlDelay){
              txGroup0.Bits.txdqsdelay = piDelay2;
            }
            break;

          case TxDqsDelay:
            txGroup0.Bits.txdqsdelay = piDelay;
            break;

          case TxEq:
            txGroup0.Bits.txeq = RegVal2EqCode (Host, socket, ch, strobe, (UINT8) piDelay, TXEQ_TO_REG);
            break;

          case TxEqTap2:
            piDelay = RegVal2EqCode (Host, socket, ch, strobe, (UINT8) piDelay, TXEQ_TO_REG);
            switch (logRank) {
            case 0:
              TxEqCoefTap2.Bits.rank0code = piDelay;
              break;
            case 1:
              TxEqCoefTap2.Bits.rank1code = piDelay;
              break;
            case 2:
              TxEqCoefTap2.Bits.rank2code = piDelay;
              break;
            case 3:
              TxEqCoefTap2.Bits.rank3code = piDelay;
              break;
            case 4:
              TxEqCoefTap2.Bits.rank4code = piDelay;
              break;
            case 5:
              TxEqCoefTap2.Bits.rank5code = piDelay;
              break;
            case 6:
              TxEqCoefTap2.Bits.rank6code = piDelay;
              break;
            case 7:
              TxEqCoefTap2.Bits.rank7code = piDelay;
              break;
            default:
              //return error invalid rank parameter
              break;
            }
           break;

          default:
            break;
        } // switch

        //
        // Write it back if the current data does not equal the cache value or if in force write mode
        //
        if ((txGroup0.Data != (*channelNvList)[ch].txGroup0[logRank][strobe]) || (mode & GSM_FORCE_WRITE)) {
          if ((group == TxDqsDelay) || (group == WrLvlDelay) || (group == TxDqDelay) || (group == TxEq)) {
            WriteDdrioDataReg (Host, socket, ch, logRank, strobe, TXGROUP0N0RANK0_0_MCIO_DDRIO_REG, txGroup0.Data);
          }
        }

        if ((TxEqCoefTap2.Data != (*channelNvList)[ch].TxEqCoefTap2[logRank][strobe]) || (mode & GSM_FORCE_WRITE)) {
          if (group == TxEqTap2) {
            WriteDdrioDataRegCh (Host, socket, ch, strobe, DATATXEQCOEF_MCIO_DDRIO_REG, TxEqCoefTap2.Data);
          }
        }

        //
        // Wait for the new value to settle
        //
        FixedDelayMicroSecond (usDelay);
      }
    } // if read only

    //
    // Save to cache
    //
    if (mode & GSM_UPDATE_CACHE) {
      if (level == DdrLevel) {
        if ((group == TxDqsDelay) || (group == WrLvlDelay) || (group == TxEq)) {
          (*channelNvList)[ch].txGroup0[logRank][strobe] = txGroup0.Data;
        }

        if (group == TxEqTap2) {
          (*channelNvList)[ch].TxEqCoefTap2[0][strobe] = TxEqCoefTap2.Bits.rank0code;
          (*channelNvList)[ch].TxEqCoefTap2[1][strobe] = TxEqCoefTap2.Bits.rank1code;
          (*channelNvList)[ch].TxEqCoefTap2[2][strobe] = TxEqCoefTap2.Bits.rank2code;
          (*channelNvList)[ch].TxEqCoefTap2[3][strobe] = TxEqCoefTap2.Bits.rank3code;
          (*channelNvList)[ch].TxEqCoefTap2[4][strobe] = TxEqCoefTap2.Bits.rank4code;
          (*channelNvList)[ch].TxEqCoefTap2[5][strobe] = TxEqCoefTap2.Bits.rank5code;
          (*channelNvList)[ch].TxEqCoefTap2[6][strobe] = TxEqCoefTap2.Bits.rank6code;
          (*channelNvList)[ch].TxEqCoefTap2[7][strobe] = TxEqCoefTap2.Bits.rank7code;
        }

      } //level
    }
  } // strobe loop

  if (CompNeeded) {
    if (IsSiliconWorkaroundEnabled ("S1707054554")) {
      CompDqOdtClkWorkAround (Host, socket);
    } else {
      DoComp (Host, socket);
    }
  }

#ifdef LRDIMM_SUPPORT
  if (mode & GSM_UPDATE_CACHE) {
    if (level == LrbufLevel) {
      switch (group) {
        case TxDqDelay:
          for (tmpStrobe = 0; tmpStrobe < MSVx4; tmpStrobe++) {
            (*rankStruct)[rank].cachedLrBuf_FxBC8x9x[tmpStrobe] =  (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[tmpStrobe];
          }
          break;

        case TxDqsDelay:
          for (tmpStrobe = 0; tmpStrobe < MSVx4; tmpStrobe++) {
            (*rankStruct)[rank].cachedLrBuf_FxBCAxBx[tmpStrobe] =  (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCAxBx[tmpStrobe];
          }
          break;

        case TxDqsDelayCycle:
          for (tmpStrobe = 0; tmpStrobe < MSVx8; tmpStrobe++) {
            (*rankStruct)[rank].cachedLrBuf_FxBCDxFx[tmpStrobe] =  (*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCDxFx[tmpStrobe];
          }
          break;

        default:
          break;
      } // switch
    } //level
  }

  if ((level == LrbufLevel) && !(mode & GSM_READ_ONLY) && ((updateNeeded) || (mode & GSM_FORCE_WRITE))) {
    switch (group) {
      case TxDqDelay:
        controlWordDataPtr = &((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[0]);
        WriteLrbufPBA(Host, socket, ch, dimm, rank, controlWordDataPtr, rank, LRDIMM_BC8x, ALL_DATABUFFERS);
        controlWordDataPtr = &((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBC8x9x[MSVx8]);
        WriteLrbufPBA(Host, socket, ch, dimm, rank, controlWordDataPtr, rank, LRDIMM_BC9x, ALL_DATABUFFERS);
        break;

      case TxDqsDelay:
        controlWordDataPtr = &((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCAxBx[0]);
        WriteLrbufPBA(Host, socket, ch, dimm, rank, controlWordDataPtr, rank, LRDIMM_BCAx, ALL_DATABUFFERS);
        controlWordDataPtr = &((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCAxBx[MSVx8]);
        WriteLrbufPBA(Host, socket, ch, dimm, rank, controlWordDataPtr, rank, LRDIMM_BCBx, ALL_DATABUFFERS);
        break;

      case TxDqsDelayCycle:
        controlWordDataPtr = &((*dimmNvList)[dimm].rankList[rank].lrBuf_FxBCDxFx[0]);
        controlWordAddr = LRDIMM_BCDx + ((rank >> 1) * 0x20);
        WriteLrbufPBA(Host, socket, ch, dimm, 0, controlWordDataPtr, (rank & BIT0) , controlWordAddr, ALL_DATABUFFERS);
        break;

      default:
        break;
    } // switch
  }
#endif  //LRDIMM

  return MRC_STATUS_SUCCESS;
} // GetSetTxDelayUnicast

/*

  Dumps each value across each bit in the byte (2 Nibbles)

  @param[in]         Socket      Current socket under test (0-based)
  @param[in]         Channel     Current channel under test (0-based)
  @param[in]         Dimm        Current dimm under test (0-based)
  @param[in]         Rank        Current rank under test (0-based)
  @param[in]         Strobe      Current strobe under test (0-based)
  @param[in]         Bit         Current bit under test (0-based) or ALL_BITS
  @param[in]         Group       Parameter to be margined
  @param[in]         Caller      String that identifies the purpose to be calling this function

*/
VOID
DumpRxDelayBitRegs (
  IN        UINT8     Socket,
  IN        UINT8     Channel,
  IN        UINT8     Dimm,
  IN        UINT8     Rank,
  IN        UINT8     Strobe,
  IN        UINT8     Bit,
  IN        MRC_GT    Group,
  IN        CHAR8     *Caller
  )
{
#ifdef DEBUG_CODE_BLOCK
  PSYSHOST                            Host;
  UINT8                               MSVx8;
  UINT8                               LogicalRank;
  UINT8                               StrobeSwizzled;
  UINT8                               NibbleIndex;
  BOOLEAN                             PreviousPrintCsr;
  UINT8                               StrobeN[MAX_NIBBLES_PER_BYTE];
  struct channelNvram                 (*ChannelNvList)[MAX_CH];
  RXGROUP0N0RANK0_0_MCIO_DDRIO_STRUCT RxGroup0[MAX_NIBBLES_PER_BYTE];
  RXGROUP1NRANK0_MCIO_DDRIO_STRUCT    RxNGroup1[MAX_NIBBLES_PER_BYTE];
  RXGROUP1PRANK0_MCIO_DDRIO_STRUCT    RxPGroup1[MAX_NIBBLES_PER_BYTE];
  RXOFFSETN0RANK0_0_MCIO_DDRIO_STRUCT RxOffset[MAX_NIBBLES_PER_BYTE];

  Host              = GetSysHostPointer ();
  MSVx8             = GetMaxStrobeValid (Host) / 2;
  ChannelNvList     = GetChannelNvList(Host, Socket);
  LogicalRank       = GetLogicalRank(Host, Socket, Channel, Dimm, Rank);
  PreviousPrintCsr  = Host->PrintCsr;
  StrobeN[INDEX_NIBBLE0] = Strobe % MSVx8;
  StrobeN[INDEX_NIBBLE1] = (Strobe % MSVx8) + MSVx8;

  RcDebugPrintWithDevice (SDBG_MARGINS, Socket, Channel, Dimm, NO_SUBCH, Rank, Strobe, Bit, "%a\n", Caller);

  for (NibbleIndex = 0; NibbleIndex < MAX_NIBBLES_PER_BYTE; NibbleIndex++) {

    StrobeSwizzled = GetStrobeSwizzle (Host, Channel, StrobeN[NibbleIndex]);

    RxGroup0[NibbleIndex].Data = ReadDdrioDataReg (Host, Socket, Channel, LogicalRank, StrobeN[NibbleIndex], RXGROUP0N0RANK0_0_MCIO_DDRIO_REG);
    RcDebugPrint (SDBG_MARGINS, "RXGROUP0N0RANK%d_%d = 0x%08x\n", LogicalRank, StrobeSwizzled, RxGroup0[NibbleIndex].Data);

    RxNGroup1[NibbleIndex].Data = ReadDdrioDataReg (Host, Socket, Channel, LogicalRank, StrobeN[NibbleIndex], RXGROUP1NRANK0_MCIO_DDRIO_REG);
    RcDebugPrint (SDBG_MARGINS, "RXGROUP1NRANK%d_%d = 0x%08x\n", LogicalRank, StrobeSwizzled, RxNGroup1[NibbleIndex].Data);

    RxPGroup1[NibbleIndex].Data = ReadDdrioDataReg (Host, Socket, Channel, LogicalRank, StrobeN[NibbleIndex], RXGROUP1PRANK0_MCIO_DDRIO_REG);
    RcDebugPrint (SDBG_MARGINS, "RXGROUP1PRANK%d_%d = 0x%08x\n", LogicalRank, StrobeSwizzled, RxPGroup1[NibbleIndex].Data);

    RxOffset[NibbleIndex].Data  = ReadDdrioDataReg (Host, Socket, Channel, LogicalRank, StrobeN[NibbleIndex], RXOFFSETN0RANK0_0_MCIO_DDRIO_REG);
    RcDebugPrint (SDBG_MARGINS, "RXOFFSETN0RANK%d_%d = 0x%08x\n", LogicalRank, StrobeSwizzled, RxOffset[NibbleIndex].Data);
  }

  RcDebugPrint (SDBG_MARGINS, "Nibble    0   1   2   3   ClkSel  RxDqDelay  RxDqsDelay [N Side]\n");
  for (NibbleIndex = 0; NibbleIndex < MAX_NIBBLES_PER_BYTE; NibbleIndex++) {
    RcDebugPrint (SDBG_MARGINS, "%d         %02d" , NibbleIndex, RxNGroup1[NibbleIndex].Bits.rxdqsndelay0);
    RcDebugPrint (SDBG_MARGINS, "  %02d", RxNGroup1[NibbleIndex].Bits.rxdqsndelay1);
    RcDebugPrint (SDBG_MARGINS, "  %02d", RxNGroup1[NibbleIndex].Bits.rxdqsndelay2);
    RcDebugPrint (SDBG_MARGINS, "  %02d", RxNGroup1[NibbleIndex].Bits.rxdqsndelay3);
    RcDebugPrint (SDBG_MARGINS, "    %d", RxOffset[NibbleIndex].Bits.rxdqsnsel);
    RcDebugPrint (SDBG_MARGINS, "       %03d", RxGroup0[NibbleIndex].Bits.rxdqdelay);
    RcDebugPrint (SDBG_MARGINS, "       %03d\n", RxGroup0[NibbleIndex].Bits.rxdqsndelay);
  }
  RcDebugPrint (SDBG_MARGINS, "Nibble    0   1   2   3   ClkSel  RxDqDelay  RxDqsDelay [P Side]\n");
  for (NibbleIndex = 0; NibbleIndex < MAX_NIBBLES_PER_BYTE; NibbleIndex++) {
    RcDebugPrint (SDBG_MARGINS, "%d         %02d" , NibbleIndex, RxPGroup1[NibbleIndex].Bits.rxdqspdelay0);
    RcDebugPrint (SDBG_MARGINS, "  %02d", RxPGroup1[NibbleIndex].Bits.rxdqspdelay1);
    RcDebugPrint (SDBG_MARGINS, "  %02d", RxPGroup1[NibbleIndex].Bits.rxdqspdelay2);
    RcDebugPrint (SDBG_MARGINS, "  %02d", RxPGroup1[NibbleIndex].Bits.rxdqspdelay3);
    RcDebugPrint (SDBG_MARGINS, "    %d", RxOffset[NibbleIndex].Bits.rxdqspsel);
    RcDebugPrint (SDBG_MARGINS, "       %03d", RxGroup0[NibbleIndex].Bits.rxdqdelay);
    RcDebugPrint (SDBG_MARGINS, "       %03d\n", RxGroup0[NibbleIndex].Bits.rxdqspdelay);
  }
  Host->PrintCsr = PreviousPrintCsr;
#endif // #ifdef DEBUG_CODE_BLOCK
}


/*

  Dumps each value across each bit in the byte (2 Nibbles)

  @param[in]         Socket      Current socket under test (0-based)
  @param[in]         Channel     Current channel under test (0-based)
  @param[in]         Dimm        Current dimm under test (0-based)
  @param[in]         Rank        Current rank under test (0-based)
  @param[in]         Strobe      Current strobe under test (0-based)
  @param[in]         Bit         Current bit under test (0-based) or ALL_BITS
  @param[in]         Group       Parameter to be margined
  @param[in]         Caller      String that identifies the purpose to be calling this function

*/
VOID
DumpTxDelayBitRegs (
  IN        UINT8     Socket,
  IN        UINT8     Channel,
  IN        UINT8     Dimm,
  IN        UINT8     Rank,
  IN        UINT8     Strobe,
  IN        UINT8     Bit,
  IN        MRC_GT    Group,
  IN        CHAR8     *Caller
  )
{
#ifdef DEBUG_CODE_BLOCK
  PSYSHOST                            Host;
  UINT8                               MSVx8;
  UINT8                               LogicalRank;
  UINT8                               StrobeSwizzled;
  UINT8                               NibbleIndex;
  BOOLEAN                             PreviousPrintCsr;
  UINT8                               StrobeN[MAX_NIBBLES_PER_BYTE];
  struct channelNvram                 (*ChannelNvList)[MAX_CH];
  TXGROUP0N0RANK0_0_MCIO_DDRIO_STRUCT TxGroup0[MAX_NIBBLES_PER_BYTE];
  TXGROUP1N0RANK0_0_MCIO_DDRIO_STRUCT TxGroup1[MAX_NIBBLES_PER_BYTE];


  Host              = GetSysHostPointer ();
  MSVx8             = GetMaxStrobeValid (Host) / 2;
  ChannelNvList     = GetChannelNvList(Host, Socket);
  LogicalRank       = GetLogicalRank(Host, Socket, Channel, Dimm, Rank);
  PreviousPrintCsr  = Host->PrintCsr;
  StrobeN[INDEX_NIBBLE0] = Strobe % MSVx8;
  StrobeN[INDEX_NIBBLE1] = (Strobe % MSVx8) + MSVx8;

  RcDebugPrintWithDevice (SDBG_MARGINS, Socket, Channel, Dimm, NO_SUBCH, Rank, Strobe, Bit, "%a\n", Caller);

  for (NibbleIndex = 0; NibbleIndex < MAX_NIBBLES_PER_BYTE; NibbleIndex++) {
    StrobeSwizzled = GetStrobeSwizzle (Host, Channel, StrobeN[NibbleIndex]);

    TxGroup0[NibbleIndex].Data = ReadDdrioDataReg (Host, Socket, Channel, LogicalRank, StrobeN[NibbleIndex], TXGROUP0N0RANK0_0_MCIO_DDRIO_REG);
    RcDebugPrint (SDBG_MARGINS, "TXGROUP0N0RANK%d_%d = 0x%08x\n", LogicalRank, StrobeSwizzled, TxGroup0[NibbleIndex].Data);

    TxGroup1[NibbleIndex].Data = ReadDdrioDataReg (Host, Socket, Channel, LogicalRank, StrobeN[NibbleIndex], TXGROUP1N0RANK0_0_MCIO_DDRIO_REG);
    RcDebugPrint (SDBG_MARGINS, "TXGROUP1N0RANK%d_%d = 0x%08x\n", LogicalRank, StrobeSwizzled, TxGroup1[NibbleIndex].Data);
  }


  RcDebugPrint (SDBG_MARGINS, "Nibble    0   1   2   3   ClkSel  TxDqDelay  TxDqsDelay\n");
  for (NibbleIndex = 0; NibbleIndex < MAX_NIBBLES_PER_BYTE; NibbleIndex++) {
    RcDebugPrint (SDBG_MARGINS, "%d         %02d" , NibbleIndex, TxGroup1[NibbleIndex].Bits.txdqpicode0);
    RcDebugPrint (SDBG_MARGINS, "  %02d", TxGroup1[NibbleIndex].Bits.txdqpicode1);
    RcDebugPrint (SDBG_MARGINS, "  %02d", TxGroup1[NibbleIndex].Bits.txdqpicode2);
    RcDebugPrint (SDBG_MARGINS, "  %02d", TxGroup1[NibbleIndex].Bits.txdqpicode3);
    RcDebugPrint (SDBG_MARGINS, "    %d", TxGroup0[NibbleIndex].Bits.txdqpiclksel);
    RcDebugPrint (SDBG_MARGINS, "       %03d", TxGroup0[NibbleIndex].Bits.txdqdelay);
    RcDebugPrint (SDBG_MARGINS, "       %03d\n", TxGroup0[NibbleIndex].Bits.txdqsdelay);
  }
  Host->PrintCsr = PreviousPrintCsr;
#endif // #ifdef DEBUG_CODE_BLOCK
}

/*

  Dumps each value across each bit in the byte (2 Nibbles)

  @param[in]         Socket      Current socket under test (0-based)
  @param[in]         Channel     Current channel under test (0-based)
  @param[in]         Dimm        Current dimm under test (0-based)
  @param[in]         Rank        Current rank under test (0-based)
  @param[in]         Strobe      Current strobe under test (0-based)
  @param[in]         Bit         Current bit under test (0-based) or ALL_BITS
  @param[in]         Group       Parameter to be margined
  @param[in]         Caller      String that identifies the purpose to be calling this function

*/
VOID
DumpRxVrefBitRegs (
  IN        UINT8     Socket,
  IN        UINT8     Channel,
  IN        UINT8     Dimm,
  IN        UINT8     Rank,
  IN        UINT8     Strobe,
  IN        UINT8     Bit,
  IN        MRC_GT    Group,
  IN        CHAR8     *Caller
  )
{
#ifdef DEBUG_CODE_BLOCK
  PSYSHOST                            Host;
  UINT8                               MSVx8;
  UINT8                               LogicalRank;
  UINT8                               StrobeSwizzled;
  UINT8                               NibbleIndex;
  BOOLEAN                             PreviousPrintCsr;
  UINT8                               StrobeN[MAX_NIBBLES_PER_BYTE];
  struct channelNvram                 (*ChannelNvList)[MAX_CH];
  RXVREFCTRLN0_0_MCIO_DDRIO_STRUCT    rxVrefCtrl[2];

  Host              = GetSysHostPointer ();
  MSVx8             = GetMaxStrobeValid (Host) / 2;
  ChannelNvList     = GetChannelNvList(Host, Socket);
  LogicalRank       = GetLogicalRank(Host, Socket, Channel, Dimm, Rank);
  PreviousPrintCsr  = Host->PrintCsr;
  StrobeN[INDEX_NIBBLE0]  = Strobe % MSVx8;
  StrobeN[INDEX_NIBBLE1]  = (Strobe % MSVx8) + MSVx8;

  RcDebugPrintWithDevice (SDBG_MARGINS, Socket, Channel, Dimm, NO_SUBCH, Rank, Strobe, Bit, "%a\n", Caller);

  for (NibbleIndex = 0; NibbleIndex < MAX_NIBBLES_PER_BYTE; NibbleIndex++) {
    StrobeSwizzled = GetStrobeSwizzle (Host, Channel, StrobeN[NibbleIndex]);

    rxVrefCtrl[NibbleIndex].Data   = ReadDdrioDataRegCh (Host, Socket, Channel, StrobeN[NibbleIndex], RXVREFCTRLN0_0_MCIO_DDRIO_REG);
    RcDebugPrint (SDBG_MARGINS, "RXVREFCTRLN%d_%d = 0x%08x\n", Channel, StrobeSwizzled, rxVrefCtrl[NibbleIndex].Data);
  }

  RcDebugPrint (SDBG_MARGINS, "Nibble    0   1   2   3\n");
  for (NibbleIndex = 0; NibbleIndex < MAX_NIBBLES_PER_BYTE; NibbleIndex++) {
    RcDebugPrint (SDBG_MARGINS, "%d         %02d" , NibbleIndex, rxVrefCtrl[NibbleIndex].Bits.vrefperbit0);
    RcDebugPrint (SDBG_MARGINS, "  %02d", rxVrefCtrl[NibbleIndex].Bits.vrefperbit1);
    RcDebugPrint (SDBG_MARGINS, "  %02d", rxVrefCtrl[NibbleIndex].Bits.vrefperbit2);
    RcDebugPrint (SDBG_MARGINS, "  %02d\n", rxVrefCtrl[NibbleIndex].Bits.vrefperbit3);
  }
  Host->PrintCsr = PreviousPrintCsr;
#endif // #ifdef DEBUG_CODE_BLOCK
}

/**

  Get TxDelay
  Wrapper for multicast and unicast versions of GetSetTxDelay

  @param[in]     Host    - Pointer to sysHost
  @param[in]     Socket  - Socket number
  @param[in]     Ch      - Channel number (0-based)
  @param[in]     Dimm    - DIMM number (0-based)
  @param[in]     Rank    - Rank number (0-based)
  @param[in]     Strobe  - Strobe number (0-based)
  @param[in]     Bit     - Bit number
  @param[in]     Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]     Group   - CSM_GT - Parameter to program
  @param[in]     Mode    - Bit-field of different modes
  @param[in,out] Value   - Pointer to delay value or offset based on mode

  @retval IO delay

**/
MRC_STATUS
GetSetTxDelay (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      UINT8     Strobe,
  IN      UINT8     Bit,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  )
{
  MRC_STATUS      Status = MRC_STATUS_FAILURE;

  //
  // Use multicast version if possible
  // Multicast register value is added, in HW, to all strobe values in the channel
  //
  if ((*Value > 0) && UseDelayMulticastOffset (Strobe, Bit, Level, Group, Mode)) {
    Status = GetSetDelayMulticastOffset (Host, Socket, Ch, Dimm, Rank, Strobe, Bit, Level, TxDqPerChDelayOffset, Mode, Value);
  }
  if (Status != MRC_STATUS_SUCCESS) {
    //
    // Multicast not available
    // For values of 0, force multicast register to 0 (in case it isn't already 0) then call the unicast routine
    //
    if ((*Value == 0) && UseDelayMulticastOffset (Strobe, Bit, Level, Group, Mode)) {
      //
      // This should not result in a register access if the Multicast register is already 0.
      // GetSetxxx will return without accessing HW if the cached value already matches the value passed in.
      //
      GetSetDelayMulticastOffset (Host, Socket, Ch, Dimm, Rank, Strobe, Bit, Level, TxDqPerChDelayOffset, Mode, Value);
    }
    Status = GetSetTxDelayUnicast (Host, Socket, Ch, Dimm, Rank, Strobe, Bit, Level, Group, Mode, Value);
  }
  return Status;
}

/**

  Get RxDelay

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param value   - Pointer data to read or write. For writes, must point to an array of values for every bit in the nibble

  @retval IO delay

**/
STATIC
MRC_STATUS
GetSetTxDelayBit (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value
  )
{
  UINT8                               logRank;
  UINT8                               maxStrobe;
  UINT8                               i;
  UINT8                               BitStart;
  UINT8                               BitEnd;
  UINT8                               SwizzleBit[MAX_BITS_IN_BYTE];
  UINT8                               NibbleIndex;
  UINT8                               NibbleOffset;
  UINT8                               BitIndex;
  UINT8                               BitOffset;
  UINT32                              UIDelta;
  UINT8                               clkSel[MAX_NIBBLES_PER_BYTE];
  UINT16                              logicVal[MAX_NIBBLES_PER_BYTE];
  UINT16                              curVal = 0;
  UINT16                              curValBit[MAX_BITS_IN_BYTE];
  UINT16                              MaxValBit;
  UINT16                              piVal;
  UINT16                              piValBit[MAX_BITS_IN_BYTE];
  UINT16                              piDelay;
  UINT16                              MaxDelayAcrossRegisterNibble[MAX_NIBBLES_PER_BYTE];
  INT16                               tmp;
  BOOLEAN                             StrobeCheck;
  BOOLEAN                             AllStrobeFlag;
  BOOLEAN                             AllBitFlag;
  BOOLEAN                             GsmWriteOffsetFlag;
  BOOLEAN                             AllStrobeBitWriteOffsetFlag;
  INT16                               bitNibbleMin;
  UINT16                              maxLimit = 0;
  UINT16                              minLimit = 0;
  UINT16                              usDelay = 0;
  struct channelNvram                 (*channelNvList)[MAX_CH];
  TXGROUP0N0RANK0_0_MCIO_DDRIO_STRUCT TxGroup0[MAX_NIBBLES_PER_BYTE];
  TXGROUP1N0RANK0_0_MCIO_DDRIO_STRUCT TxGroup1[MAX_NIBBLES_PER_BYTE];
  UINT8                               StrobeN[MAX_NIBBLES_PER_BYTE];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;


  channelNvList = GetChannelNvList(Host, socket);
  //
  // UIDelta is always '0' for bit swizzling within the Register Nibble
  // Although for bit swizzling across Register Nibble it may be different than '0'
  //
  UIDelta = 0;

  AllStrobeFlag = FALSE;
  AllBitFlag    = FALSE;
  AllStrobeBitWriteOffsetFlag = FALSE;
  GsmWriteOffsetFlag = FALSE;

  TxGroup0[INDEX_NIBBLE0].Data = 0;
  TxGroup1[INDEX_NIBBLE0].Data = 0;
  TxGroup0[INDEX_NIBBLE1].Data = 0;
  TxGroup1[INDEX_NIBBLE1].Data = 0;

  //
  // Get the logical rank #
  //
  logRank = GetLogicalRank(Host, socket, ch, dimm, rank);

  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits(Host, level, group, &minLimit, &maxLimit, &usDelay);

  if (strobe >= MAX_STROBE && strobe != ALL_STROBES) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                   "The strobe value is out of range!\n");
    RC_FATAL_ERROR ((strobe < MAX_STROBE || strobe == ALL_STROBES), ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  if (strobe == ALL_STROBES) {
    strobe    = 0;
    maxStrobe = MSVx4;
    AllStrobeFlag = TRUE;
  } else {
    maxStrobe = strobe + 1;
  }

  //Assert on error; should never get here!
  RC_FATAL_ERROR (bit < BITS_PER_NIBBLE || bit == ALL_BITS, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_56);
  if ((mode & GSM_READ_ONLY) != GSM_READ_ONLY) {
    //
    // Writes must pass in values for all bits in the nibble
    //
    RC_FATAL_ERROR (bit == ALL_BITS, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_164);
  }

  if (bit == ALL_BITS) {
    BitStart  = 0;
    BitEnd    = BITS_PER_NIBBLE;
    AllBitFlag = TRUE;
  } else {
    BitStart  = bit;
    BitEnd    = BitStart + 1;
  }

  if (((mode & GSM_READ_ONLY) == 0) && ((mode & GSM_UPDATE_CACHE) == 0)) {
    //
    // Only applicable for writes
    // Only applicable when operating cache in non-coherent mode
    //
    GsmWriteOffsetFlag = TRUE;
  }
  if (IsBrsPresent (Host, socket)) {
    //
    // This fix is required explicitly to BRS
    // To avoid validation overhead do not apply for any other program
    //
    AllStrobeBitWriteOffsetFlag = AllStrobeFlag & AllBitFlag & GsmWriteOffsetFlag;
  }

  if (AllStrobeBitWriteOffsetFlag) {
    maxStrobe = maxStrobe / 2;
    BitEnd    = MAX_BITS_IN_BYTE;
  }

  for ( ; strobe < maxStrobe; strobe++) {

    StrobeN[INDEX_NIBBLE0] = strobe % MSVx8;
    StrobeN[INDEX_NIBBLE1] = (strobe % MSVx8) + MSVx8;

    if (mode & GSM_READ_CSR) {
      //
      // Read from PCI config space
      //
      TxGroup0[INDEX_NIBBLE0].Data = ReadDdrioDataReg (Host, socket, ch, logRank, StrobeN[INDEX_NIBBLE0], TXGROUP0N0RANK0_0_MCIO_DDRIO_REG);
      TxGroup1[INDEX_NIBBLE0].Data = ReadDdrioDataReg (Host, socket, ch, logRank, StrobeN[INDEX_NIBBLE0], TXGROUP1N0RANK0_0_MCIO_DDRIO_REG);
      TxGroup0[INDEX_NIBBLE1].Data = ReadDdrioDataReg (Host, socket, ch, logRank, StrobeN[INDEX_NIBBLE1], TXGROUP0N0RANK0_0_MCIO_DDRIO_REG);
      TxGroup1[INDEX_NIBBLE1].Data = ReadDdrioDataReg (Host, socket, ch, logRank, StrobeN[INDEX_NIBBLE1], TXGROUP1N0RANK0_0_MCIO_DDRIO_REG);
    } else {
      //
      // Read from cache
      //
      TxGroup0[INDEX_NIBBLE0].Data = (*channelNvList)[ch].txGroup0[logRank][StrobeN[INDEX_NIBBLE0]];
      TxGroup1[INDEX_NIBBLE0].Data = (*channelNvList)[ch].txGroup1[logRank][StrobeN[INDEX_NIBBLE0]];
      TxGroup0[INDEX_NIBBLE1].Data = (*channelNvList)[ch].txGroup0[logRank][StrobeN[INDEX_NIBBLE1]];
      TxGroup1[INDEX_NIBBLE1].Data = (*channelNvList)[ch].txGroup1[logRank][StrobeN[INDEX_NIBBLE1]];
    }

    for (i = 0; i < MAX_BITS_IN_BYTE; i++) {
      //
      // Get Swizzle Bit
      //
      SwizzleBit[i]   = GetDqSwizzle (ch, StrobeN[INDEX_NIBBLE0], i, level, group);
    }

    for (NibbleIndex = 0; NibbleIndex < MAX_NIBBLES_PER_BYTE; NibbleIndex++) {

      NibbleOffset = (NibbleIndex * BITS_PER_NIBBLE);

      logicVal[NibbleIndex] = (UINT16)(TxGroup0[NibbleIndex].Bits.txdqdelay & TX_DQ_LOGIC_DELAY_MASK);

      piValBit[0 + NibbleOffset] = (UINT16)TxGroup1[NibbleIndex].Bits.txdqpicode0;
      piValBit[1 + NibbleOffset] = (UINT16)TxGroup1[NibbleIndex].Bits.txdqpicode1;
      piValBit[2 + NibbleOffset] = (UINT16)TxGroup1[NibbleIndex].Bits.txdqpicode2;
      piValBit[3 + NibbleOffset] = (UINT16)TxGroup1[NibbleIndex].Bits.txdqpicode3;

    }

    for (NibbleIndex = 0; NibbleIndex < MAX_NIBBLES_PER_BYTE; NibbleIndex++) {

      NibbleOffset = (NibbleIndex * BITS_PER_NIBBLE);
      //
      // N[x].clksel will be a value between 0->3
      // No matter the swizzling N0.picode0,1,2,3 will be always related with N0.clksel
      //
      piVal = piValBit[ TxGroup0[NibbleIndex].Bits.txdqpiclksel + NibbleOffset ];

      //
      // Calculate total delay values
      //
      for (i = 0; i < BITS_PER_NIBBLE; i++) {

        BitOffset = i + NibbleOffset;

        // Handle wrap
        if (piValBit[BitOffset] > piVal) {

          //Assert that sufficient range exists
          RC_FATAL_ERROR (logicVal[NibbleIndex], ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_57);

          curValBit[BitOffset] = logicVal[NibbleIndex] + piValBit[BitOffset] - 64;
        } else {
          curValBit[BitOffset] = logicVal[NibbleIndex] + piValBit[BitOffset];
        }

        //
        // UIDelta is always '0' for bit swizzling within the Register Nibble
        // Although for bit swizzling across Register Nibble it may be different than '0'
        //
        UIDelta = 0;

#ifdef DDR5_SUPPORT
        //
        //
        // BRS is the only CPU that has to apply this WA, because
        // so far it is the only 10nm product that uses swizzling
        // across Register Nibbles
        //
        // [7:0]   -> UIs for Bit 0
        // [15:8]  -> UIs for Bit 1
        // [23:16] -> UIs for Bit 2
        // [31:24] -> UIs for Bit 3
        // 1 UI = 64 ticks
        //
        UIDelta = (*channelNvList)[ch].TxDqBitClockDelta[logRank][ StrobeN[NibbleIndex] ];
        UIDelta = UIDelta >> (8*i);
        UIDelta = UIDelta & 0xFF;
        UIDelta = UIDelta * 64;
#endif // #ifdef DDR5_SUPPORT
        //
        // UIDelta is always '0' for bit swizzling within the Register Nibble
        // Although for bit swizzling across Register Nibble it may be different than '0'
        //
        curValBit[BitOffset] -= (UINT16)UIDelta;

      }

    } // NibbleIndex loop

    //
    // Check if read only
    //
    if (mode & GSM_READ_ONLY) {
      MaxValBit = 0;
      for (bit = BitStart; bit < BitEnd; bit++) {

        BitIndex = bit;
        if (strobe >= MSVx8) {
          BitIndex += BITS_PER_NIBBLE;
        }
        if (BitIndex >= MAX_BITS_IN_BYTE ) {
          break;
        }
        //
        // Get current delay value
        //
        curVal = curValBit[ SwizzleBit[BitIndex] ];
        if (curVal > MaxValBit) {
          MaxValBit = curVal;
        }
      }
      //
      // Save the value read into the output parameter
      //
      *value = MaxValBit;

    } else { // GSM_READ_ONLY
      //
      // Write
      //
      for (bit = BitStart; bit < BitEnd; bit++) {

        BitIndex = bit;
        if (strobe >= MSVx8) {
          BitIndex += BITS_PER_NIBBLE;
        }
        if (BitIndex >= MAX_BITS_IN_BYTE ) {
          break;
        }

        // Adjust the current delay value by offset
        if (mode & GSM_WRITE_OFFSET) {
          //
          // Add the offset to the current value
          //
          tmp = curValBit[ SwizzleBit[BitIndex] ] + value[bit % BITS_PER_NIBBLE];

        } else {
          //
          // Write absolute value
          //
          tmp = value[bit];
        }

        //
        // Make sure we do not exeed the limits
        //
        if (tmp >= minLimit) {
          piDelay = tmp;
        } else {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, BitIndex,
                         "%a Out of range!! tmp = 0x%x, minLimit = 0x%x\n", GetGroupStr (group), tmp, minLimit);
          piDelay = minLimit;
        }
        //
        // Ensure we do not exceed maximums
        //
        if (piDelay > maxLimit) {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, BitIndex,
                         "%a Out of range!! piDelay = 0x%x, maxLimit = 0x%x\n", GetGroupStr (group), piDelay, maxLimit);
          piDelay = maxLimit;
        }

        //
        // Update current delay value
        //
        curValBit[ SwizzleBit[BitIndex] ] = piDelay;
      } // bit

      for (NibbleIndex = 0; NibbleIndex < MAX_NIBBLES_PER_BYTE; NibbleIndex++) {

        NibbleOffset = (NibbleIndex * BITS_PER_NIBBLE);

        //
        // Determine minimum delay
        // Set to largest possible initially
        //
        bitNibbleMin = 1024;

        for (i = 0; i < BITS_PER_NIBBLE; i++) {

          BitOffset = i + NibbleOffset;

          if (curValBit[BitOffset] < bitNibbleMin) {
            bitNibbleMin = curValBit[BitOffset];
          }
        } // i loop

        clkSel[NibbleIndex] = 0;
        MaxDelayAcrossRegisterNibble[NibbleIndex] = 0;
        for (i = 0; i < BITS_PER_NIBBLE; i++) {

          BitOffset = i + NibbleOffset;

          //
          // Enforce maximum per-bit skew
          //
          StrobeCheck = TRUE;
#ifdef DDR5_SUPPORT
          //
          // Verify only when both nibbles in a byte have been updated
          //
          StrobeCheck = (strobe >= MSVx8);
#endif // #ifdef DDR5_SUPPORT
          if (((curValBit[BitOffset] - bitNibbleMin) > MAX_TX_PER_BIT_SKEW) && (StrobeCheck || AllStrobeBitWriteOffsetFlag)) {
            curValBit[BitOffset] = bitNibbleMin + MAX_TX_PER_BIT_SKEW;
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, BitOffset,
                         "Skew exceeds %d ticks. Limiting skew to %d ticks\n", MAX_TX_PER_BIT_SKEW, MAX_TX_PER_BIT_SKEW);
          }
          //
          // Determine largest per-bit delay across
          // N[x].picode0,1,2,3, so de-swizzle offset in the array
          // and find the greater value
          //
            if (curValBit[ BitOffset ] > MaxDelayAcrossRegisterNibble[NibbleIndex]) {
            clkSel[NibbleIndex]  = i;
              MaxDelayAcrossRegisterNibble[NibbleIndex] = curValBit[ BitOffset ];
          }
        } // i loop

        //
        // Update TxDq CSR settings
        //
          TxGroup0[NibbleIndex].Bits.txdqdelay = MaxDelayAcrossRegisterNibble[NibbleIndex] & TX_DQ_LOGIC_DELAY_MASK;
      } // NibbleIndex loop

      for (NibbleIndex = 0; NibbleIndex < MAX_NIBBLES_PER_BYTE; NibbleIndex++) {

        NibbleOffset = (NibbleIndex * BITS_PER_NIBBLE);

#ifdef DDR5_SUPPORT
          //
          // BRS is the only CPU that has to apply this WA, because
          // so far it is the only 10nm product that uses swizzling
          // across Register Nibbles
          //
          for (i = 0; i < BITS_PER_NIBBLE; i++) {
            UIDelta = (UINT8)((MaxDelayAcrossRegisterNibble[NibbleIndex] - curValBit[ i + NibbleOffset ]) / 64);
            //
            // Clear UI for corresponding bit
            // [7:0]   -> UIs for Bit 0
            // [15:8]  -> UIs for Bit 1
            // [23:16] -> UIs for Bit 2
            // [31:24] -> UIs for Bit 3
            // 1 UI = 64 ticks
            //
            (*channelNvList)[ch].TxDqBitClockDelta[logRank][ StrobeN[NibbleIndex] ] &= ~(0xFF << (8*i));
            (*channelNvList)[ch].TxDqBitClockDelta[logRank][ StrobeN[NibbleIndex] ] |= (UIDelta << (8*i));
          }
#endif // #ifdef DDR5_SUPPORT
        //
        // 'clkSel' variable has the Index that has the largest piDelay,
        // across its own nibble, no matter the swizzling
        //
        TxGroup0[NibbleIndex].Bits.txdqpiclksel = clkSel[NibbleIndex];

        TxGroup1[NibbleIndex].Bits.txdqpicode0  = curValBit[ 0 + NibbleOffset ] % 64;
        TxGroup1[NibbleIndex].Bits.txdqpicode1  = curValBit[ 1 + NibbleOffset ] % 64;
        TxGroup1[NibbleIndex].Bits.txdqpicode2  = curValBit[ 2 + NibbleOffset ] % 64;
        TxGroup1[NibbleIndex].Bits.txdqpicode3  = curValBit[ 3 + NibbleOffset ] % 64;

      } // NibbleIndex loop

    } // GSM_READ_ONLY

    for (NibbleIndex = 0; NibbleIndex < MAX_NIBBLES_PER_BYTE; NibbleIndex++) {

      NibbleOffset = (NibbleIndex * BITS_PER_NIBBLE);

      for (i = 0; i < BITS_PER_NIBBLE; i++) {

        BitOffset = i + NibbleOffset;

        //
        // When the upper nibble is getting updated there
        // should be NO UI delta within the Register Nibble
        //
        // Exception to this rule is when:
        // + Updating bit by bit, instead of ALL_BITS
        //
        UIDelta = (*channelNvList)[ch].TxDqBitClockDelta[logRank][ StrobeN[NibbleIndex] ];
        UIDelta = UIDelta >> (8*i);
        UIDelta = UIDelta & 0xFF;

        if (UIDelta > 0 && (strobe / MSVx8) == 1 && (BitEnd - BitStart) > 1) {
          RcDebugPrintWithDevice (
            SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, i,
            "UIDelta = %d\n", UIDelta
            );

          RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_161);
        }
      }
    }

    for (NibbleIndex = 0; NibbleIndex < MAX_NIBBLES_PER_BYTE; NibbleIndex++) {
      if ((mode & GSM_READ_ONLY) == 0) {
        if (IsBrsPresent (Host, socket)) {
          //
          // Write it back if the current data does not equal the cache value or
          // if in force write mode only for current nibble (optimization)
          // for BRS it is always required to access for NIBBLE registers
          //
          if ((TxGroup0[NibbleIndex].Data != (*channelNvList)[ch].txGroup0[logRank][ StrobeN[NibbleIndex] ]) || (mode & GSM_FORCE_WRITE)) {
            WriteDdrioDataReg (Host, socket, ch, logRank, StrobeN[NibbleIndex], TXGROUP0N0RANK0_0_MCIO_DDRIO_REG, TxGroup0[NibbleIndex].Data);
          }
          if ((TxGroup1[NibbleIndex].Data != (*channelNvList)[ch].txGroup1[logRank][ StrobeN[NibbleIndex] ]) || (mode & GSM_FORCE_WRITE)) {
            WriteDdrioDataReg (Host, socket, ch, logRank, StrobeN[NibbleIndex], TXGROUP1N0RANK0_0_MCIO_DDRIO_REG, TxGroup1[NibbleIndex].Data);
          }
        } else {
          //
          // Write it back if the current data does not equal the cache value or
          // if in force write mode only for current nibble (optimization)
          // The register only should be accessed when all the bits within the nibble were updated
          //
          if ((TxGroup0[NibbleIndex].Data != (*channelNvList)[ch].txGroup0[logRank][ StrobeN[NibbleIndex] ]) || ((mode & GSM_FORCE_WRITE) != 0 && (NibbleIndex == strobe / MSVx8))) {
            WriteDdrioDataReg (Host, socket, ch, logRank, StrobeN[NibbleIndex], TXGROUP0N0RANK0_0_MCIO_DDRIO_REG, TxGroup0[NibbleIndex].Data);
          }
          if ((TxGroup1[NibbleIndex].Data != (*channelNvList)[ch].txGroup1[logRank][ StrobeN[NibbleIndex] ]) || ((mode & GSM_FORCE_WRITE) != 0 && (NibbleIndex == strobe / MSVx8))) {
            WriteDdrioDataReg (Host, socket, ch, logRank, StrobeN[NibbleIndex], TXGROUP1N0RANK0_0_MCIO_DDRIO_REG, TxGroup1[NibbleIndex].Data);
          }
        }
        //
        // Wait for the new value to settle
        //
        FixedDelayMicroSecond (usDelay);
      }
      //
      // Save to cache
      //
      if (mode & GSM_UPDATE_CACHE) {
        (*channelNvList)[ch].txGroup0[logRank][ StrobeN[ NibbleIndex ] ] = TxGroup0[NibbleIndex].Data;
        (*channelNvList)[ch].txGroup1[logRank][ StrobeN[ NibbleIndex ] ] = TxGroup1[NibbleIndex].Data;
      }
    } // NibbleIndex loop
  } // strobe loop

  return MRC_STATUS_SUCCESS;
} // GetSetTxDelayBit

/**

  Get TxTco

  @param[in]     Host    - Pointer to sysHost
  @param[in]     Socket  - Socket number
  @param[in]     Ch      - Channel number (0-based)
  @param[in]     Dimm    - DIMM number (0-based)
  @param[in]     Rank    - Rank number (0-based)
  @param[in]     Strobe  - Strobe number (0-based)
  @param[in]     Bit     - Bit number
  @param[in]     Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]     Group   - CSM_GT - Parameter to program
  @param[in]     Mode    - Bit-field of different modes
  @param[in,out] Value   - Pointer to delay Value or offset based on mode

  @retval IO delay

**/
MRC_STATUS
GetSetTxTco (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      UINT8     Strobe,
  IN      UINT8     Bit,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  )
{
  UINT8                                     LogRank;
  UINT8                                     MaxStrobe;
  UINT16                                    CurVal = 0;
  UINT16                                    PiDelay;
  INT16                                     Tmp;
  UINT16                                    MaxLimit = 0;
  UINT16                                    MinLimit = 0;
  UINT16                                    UsDelay = 0;
  struct channelNvram                       (*ChannelNvList)[MAX_CH];
  COMPDATA0N0_0_MCIO_DDRIO_STRUCT           TxTcoN0;
  COMPDATA0N0_0_MCIO_DDRIO_STRUCT           TxTcoN1;
  BOOLEAN                                   CompNeeded;
  UINT8                                     MSVx8 = GetMaxStrobeValid(Host) / 2;

  if (Level != DdrLevel) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                                    "GetSetTxTco - Level not supported: %d\n", Level);
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_159);
    return MRC_STATUS_LEVEL_NOT_SUPPORTED;
  }

  ChannelNvList = GetChannelNvList(Host, Socket);
  CompNeeded = FALSE;

  TxTcoN0.Data    = 0;
  TxTcoN1.Data    = 0;

  //
  // Get the logical Rank #
  //
  LogRank = GetLogicalRank(Host, Socket, Ch, Dimm, Rank);

  //
  // Get minimum and maximum Value
  //
  GetDataGroupLimits(Host, Level, Group, &MinLimit, &MaxLimit, &UsDelay);

  if ((MSVx8 > (MAX_STROBE / 2)) || ((Strobe >= MSVx8) && (Strobe != ALL_STROBES))) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
                                    "The strobe Value is out of range!\n");
    RC_FATAL_ERROR (FALSE, ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  if (Strobe == ALL_STROBES) {
    Strobe      = 0;
    MaxStrobe   = MSVx8;
  } else {
    MaxStrobe = Strobe + 1;
  }

  //
  // Array bounds check
  //
  MemCheckIndex (Strobe, MSVx8);

  for ( ; Strobe < MaxStrobe; Strobe++) {

    if (Mode & GSM_READ_CSR) {
      //
      // Read from PCI config space
      //
      TxTcoN0.Data = ReadDdrioDataRegCh (Host, Socket, Ch, Strobe,           COMPDATA0N0_0_MCIO_DDRIO_REG);
      TxTcoN1.Data = ReadDdrioDataRegCh (Host, Socket, Ch, (Strobe + MSVx8), COMPDATA0N0_0_MCIO_DDRIO_REG);
    } else {
      //
      // Read from cache
      //
      TxTcoN0.Data = (*ChannelNvList)[Ch].txTco[LogRank][Strobe];
      TxTcoN1.Data = (*ChannelNvList)[Ch].txTco[LogRank][Strobe + MSVx8];
    }

    CurVal = (UINT16)TxTcoN0.Bits.tcocomp;

    //
    // Check if read only
    //
    if (Mode & GSM_READ_ONLY) {
      *Value = CurVal;
    } else {
      //
      // Write
      //

      // Adjust the current CMD delay Value by offset
      if (Mode & GSM_WRITE_OFFSET) {
        Tmp = CurVal + *Value;
      } else {
        Tmp = *Value;
      }

      if (Tmp >= MinLimit) {
        PiDelay = Tmp;
      } else {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
                                "%a Out of range!! Tmp = 0x%x, MinLimit = 0x%x\n", GetGroupStr (Group), Tmp, MinLimit);
        PiDelay = MinLimit;
      }

      // Ensure we do not exceed maximums
      if (PiDelay > MaxLimit) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
                                "%a Out of range!! PiDelay = 0x%x, MaxLimit = 0x%x\n", GetGroupStr (Group), PiDelay, MaxLimit);
        PiDelay = MaxLimit;
      }

      TxTcoN0.Bits.tcocomp = PiDelay;
      TxTcoN1.Bits.tcocomp = PiDelay;

      //
      // Write it back if the current data does not equal the cache Value or if in force write mode
      //

      if ((TxTcoN0.Data != (*ChannelNvList)[Ch].txTco[LogRank][Strobe]) || (Mode & GSM_FORCE_WRITE)) {
        WriteDdrioDataRegCh (Host, Socket, Ch, Strobe, COMPDATA0N0_0_MCIO_DDRIO_REG, TxTcoN0.Data);
        CompNeeded = TRUE;
      }
      if ((TxTcoN1.Data != (*ChannelNvList)[Ch].txTco[LogRank][Strobe + MSVx8]) || (Mode & GSM_FORCE_WRITE)) {
        WriteDdrioDataRegCh (Host, Socket, Ch, (Strobe + MSVx8), COMPDATA0N0_0_MCIO_DDRIO_REG, TxTcoN1.Data);
        CompNeeded = TRUE;
      }

      //
      // Wait for the new Value to settle
      //
      FixedDelayMicroSecond (UsDelay);
    } // if read only

    //
    // Save to cache
    //
    if (Mode & GSM_UPDATE_CACHE) {
      (*ChannelNvList)[Ch].txTco[LogRank][Strobe]           = TxTcoN0.Data;
      (*ChannelNvList)[Ch].txTco[LogRank][(Strobe + MSVx8)] = TxTcoN1.Data;
    }
  } // Strobe loop

  if (CompNeeded) {
    if (IsSiliconWorkaroundEnabled ("S1707054554")) {
      CompDqOdtClkWorkAround (Host, Socket);
    } else {
      DoComp (Host, Socket);
    }
  }

  return MRC_STATUS_SUCCESS;
} // GetSetTxTco

/***
  GetSetRxStrobeInvBit

  @param LogRank - Logical Rank to be operated on
  @param *RxDfeControl - pointer to the Data Rx DFE Control CSR structure to use
  @param Mode    - If GSM_READ_ONLY bit set read the bit, else write the bit
  @param value   - bit value read from the structure or to write to the structure

  */

MRC_STATUS
GetSetRxStrobeInvBit (
  UINT8 LogRank,
  DDRCRDATARXDFECONTROL_MCIO_DDRIO_STRUCT *RxDfeControl,
  UINT8 Mode,
  INT16 *Value
  )
{
  UINT16     CurVal = 0;

  if (LogRank >= MAX_RANK_CH) {
    return MRC_STATUS_RANK_NOT_PRESENT;
  }

  if (Mode & GSM_READ_ONLY) {
    switch (LogRank) {
    case 0:
      CurVal = (UINT16) (RxDfeControl->Bits.rxstrobeinv_rank0);
      break;
    case 1:
      CurVal = (UINT16) (RxDfeControl->Bits.rxstrobeinv_rank1);
      break;
    case 2:
      CurVal = (UINT16) (RxDfeControl->Bits.rxstrobeinv_rank2);
      break;
    case 3:
      CurVal = (UINT16) (RxDfeControl->Bits.rxstrobeinv_rank3);
      break;
    case 4:
      CurVal = (UINT16) (RxDfeControl->Bits.rxstrobeinv_rank4);
      break;
    case 5:
      CurVal = (UINT16) (RxDfeControl->Bits.rxstrobeinv_rank5);
      break;
    case 6:
      CurVal = (UINT16) (RxDfeControl->Bits.rxstrobeinv_rank6);
      break;
    case 7:
      CurVal = (UINT16) (RxDfeControl->Bits.rxstrobeinv_rank7);
      break;
    default:
      break;
    }
    //
    // Save the value read into the output parameter
    //
    *Value = CurVal;
  } else {
    //TODO
    switch (LogRank) {
    case 0:
      RxDfeControl->Bits.rxstrobeinv_rank0 = *Value;
      break;
    case 1:
      RxDfeControl->Bits.rxstrobeinv_rank1 = *Value;
      break;
    case 2:
      RxDfeControl->Bits.rxstrobeinv_rank2 = *Value;
      break;
    case 3:
      RxDfeControl->Bits.rxstrobeinv_rank3 = *Value;
      break;
    case 4:
      RxDfeControl->Bits.rxstrobeinv_rank4 = *Value;
      break;
    case 5:
      RxDfeControl->Bits.rxstrobeinv_rank5 = *Value;
      break;
    case 6:
      RxDfeControl->Bits.rxstrobeinv_rank6 = *Value;
      break;
    case 7:
      RxDfeControl->Bits.rxstrobeinv_rank7 = *Value;
      break;
    default:
      break;
    }
  }
  return MRC_STATUS_SUCCESS;
}

/**

  Convert a Pi delay setting (i.e. 0 to 151) to the corresponding CSR offset
  for Rx Dq/Dqs delays when DFE is enabled.  When DFE is enabled, the P and N
  CSR values share the RxStrobeInv setting.  The RxStrobeInv setting offsets the
  Pi Delay values by 64, so if both Pi Delay settings are greater than 64 then
  the CSR value is just the Pi Delay minus 64.  However, if either is less than
  or equal to 64, then this routine will ensure that the value does not exceed
  the CSR maximum MAX_PHASE_IN_READ_DQS_ADJUSTMENT (currently 87).

  @param[in]  PiDelay   - First Pi Delay setting
  @param[in]  PiDelay2  - Second Pi Delay setting
  @param[out] CsrValue  - Pointer to the CSR value corresponding to the first Pi Delay input
  @param[out] CsrValue2 - Pointer to the CSR value corresponding to the second Pi Delay input

  @retval N/A

**/
VOID
ConvertPiDelayToRxDfeEnabledCsrValue (
  IN     UINT16  PiDelay,
  IN     UINT16  PiDelay2,
     OUT UINT16  *CsrValue,
     OUT UINT16  *CsrValue2
  )
{
  if ((PiDelay < 64) || (PiDelay2 < 64)) {
    if (PiDelay > MAX_PHASE_IN_READ_DQS_ADJUSTMENT) {
      *CsrValue = MAX_PHASE_IN_READ_DQS_ADJUSTMENT;
    } else {
      *CsrValue = PiDelay;
    }
    if (PiDelay2 > MAX_PHASE_IN_READ_DQS_ADJUSTMENT) {
      *CsrValue2 = MAX_PHASE_IN_READ_DQS_ADJUSTMENT;
    } else {
      *CsrValue2 = PiDelay2;
    }
  } else {
    *CsrValue = PiDelay - 64;
    *CsrValue2 = PiDelay2 - 64;
  }
} //ConvertPiDelayToRxDfeEnabledCsrValue

/**

  Get RxDelay using unicast (single point) registers

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param value   - Data to program

  @retval IO delay

**/
MRC_STATUS
GetSetRxDelayUnicast (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value
  )
{
  UINT8                               logRank;
  UINT8                               maxStrobe;
  UINT8                               RoundTrip;
  UINT16                              curVal  = 0;
  UINT16                              curVal2 = 0;
  UINT16                              piDelay;
  UINT16                              piDelay2 = 0;
  INT16                               tmp;
  INT16                               tmp2 = 0;
  UINT16                              maxLimit = 0;
  UINT16                              minLimit = 0;
  UINT16                              usDelay = 0;
  struct channelNvram                 (*channelNvList)[MAX_CH];
  struct RankCh                       (*RankPerCh)[MAX_RANK_CH];
  struct ddrRank                      (*RankList)[MAX_RANK_DIMM];
  RXGROUP0N0RANK0_0_MCIO_DDRIO_STRUCT rxGroup0;
  DDRCRDATARXDFECONTROL_MCIO_DDRIO_STRUCT RxDfeControl;
  DDRCRDATARXDFECOEFF_MCIO_DDRIO_STRUCT RxDfeCoeff;
  INT16                               RxStrobeInvCurrentRankOnEntry = 0;
  INT16                               RxStrobeInvNew = 0;
  UINT16                              RxStrobeInvPiDelay = 0;
  INT16                               RcvEnOffset = 0;
#if defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)
  UINT8                               tmpStrobe;
  UINT8                               *controlWordDataPtr;
  UINT8                               controlWordAddr;
  struct rankDevice                   (*rankStruct)[MAX_RANK_DIMM];
  UINT8                               updateNeeded = 0;
  struct dimmNvram                    (*dimmNvList)[MAX_DIMM];
  UINT8                               MSVx8 = GetMaxStrobeValid(Host) / 2;
#endif  //defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)
  UINT8                               MSVx4 = GetMaxStrobeValid(Host);

  channelNvList = GetChannelNvList(Host, socket);
  RankPerCh = &(*channelNvList)[ch].rankPerCh;
  RankList = GetRankNvList(Host, socket, ch, dimm);
#if defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)
  rankStruct  = GetRankStruct(Host, socket, ch, dimm);
  dimmNvList = GetDimmNvList(Host, socket, ch);
#endif  //defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)

  rxGroup0.Data = 0;
  RxDfeCoeff.Data = 0;
  RxDfeControl.Data = 0;

  //
  // Get the logical rank #
  //
  logRank = GetLogicalRank(Host, socket, ch, dimm, rank);
  if (logRank >= MAX_RANK_CH) {
    RC_FATAL_ERROR (FALSE, ERR_VALUE_OUT_OF_RANGE, ERR_VALUE_ABOVE_MAX);
    return MRC_STATUS_RANK_NOT_PRESENT;
  }

  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits(Host, level, group, &minLimit, &maxLimit, &usDelay);

  if (strobe == ALL_STROBES) {
    strobe    = 0;
    maxStrobe = MSVx4;
  } else {
    maxStrobe = strobe + 1;

    if (maxStrobe > MAX_STROBE) {
      return MRC_STATUS_STROBE_NOT_PRESENT;
    }
  }

#if defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)
  if (((*dimmNvList)[dimm].lrBuf_BC1x & LRDIMM_ONE_RANK_TIMING_MODE) && (level == LrbufLevel)) {
    rank = 0;
    logRank = GetLogicalRank(Host, socket, ch, dimm, 0);
    if (logRank >= MAX_RANK_CH) {
      RC_FATAL_ERROR (FALSE, ERR_VALUE_OUT_OF_RANGE, ERR_VALUE_ABOVE_MAX);
      return MRC_STATUS_RANK_NOT_PRESENT;
    }
  }
#endif // defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)
  for (; strobe < maxStrobe; strobe++) {
#if defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)
    if (level == LrbufLevel) {

      if (mode & GSM_READ_CSR) {
        //
        // Read from actual setting cache
        //
        switch (group) {
          case RxDqsDelay:
            if ((*RankList)[rank].lrBuf_FxBC4x5x[strobe] < 16) {
              curVal = (*RankList)[rank].lrBuf_FxBC4x5x[strobe] + 15;
            } else {
              curVal = (*RankList)[rank].lrBuf_FxBC4x5x[strobe] ^ 31;
            }
            break;
          case RecEnDelay:
              curVal = (*RankList)[rank].lrBuf_FxBC2x3x[strobe];
            break;
          case RecEnDelayCycle:
              curVal = (((*RankList)[rank].lrBuf_FxBCCxEx[strobe % MSVx8] >> (4 * (strobe / MSVx8))) & 0xF);
            break;
          default:
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                           "GetSetRxDelay called unsupported group!\n");
            RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_58);
            break;
        } //switch group
      } else {
        //
        // Read from setting cache
        //
        switch (group) {
        case RxDqsDelay:
          if ((*rankStruct)[rank].cachedLrBuf_FxBC4x5x[strobe] < 16) {
            curVal = (*rankStruct)[rank].cachedLrBuf_FxBC4x5x[strobe] + 15;
          } else {
            curVal = (*rankStruct)[rank].cachedLrBuf_FxBC4x5x[strobe] ^ 31;
          }
          break;
        case RecEnDelay:
          curVal = (*rankStruct)[rank].cachedLrBuf_FxBC2x3x[strobe];
          break;
        case RecEnDelayCycle:
          curVal = (((*rankStruct)[rank].cachedLrBuf_FxBCCxEx[strobe % MSVx8]>>(4 * (strobe / MSVx8))) & 0xF);
          break;
        default:
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                         "GetSetRxDelay called unsupported group!\n");
          RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_59);
          break;
        } //switch group
      }
    } else
#endif // defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)
    {
      if (mode & GSM_READ_CSR) {
        //
        // Read from PCI config space
        //
        rxGroup0.Data = ReadDdrioDataReg (Host, socket, ch, logRank, strobe, RXGROUP0N0RANK0_0_MCIO_DDRIO_REG);
        RxDfeCoeff.Data = ReadDdrioDataRegCh (Host, socket, ch, strobe, DDRCRDATARXDFECOEFF_MCIO_DDRIO_REG);
        RxDfeControl.Data = ReadDdrioDataRegCh (Host, socket, ch, strobe, DDRCRDATARXDFECONTROL_MCIO_DDRIO_REG);

      } else {
        //
        // Read from cache
        //
        rxGroup0.Data = (*channelNvList)[ch].rxGroup0[logRank][strobe];
        RxDfeCoeff.Data = (*channelNvList)[ch].RxDfeCoeff[strobe];
        RxDfeControl.Data = (*channelNvList)[ch].RxDfeControl[strobe];

      }

      GetSetRxStrobeInvBit (logRank, &RxDfeControl, GSM_READ_ONLY, &RxStrobeInvCurrentRankOnEntry);
      if ((RxDfeControl.Bits.dfe_en == 1) && (RxStrobeInvCurrentRankOnEntry == 0)) {
        //
        // If DFE is enabled, the current per rank strobe inversion setting impacts the Delay
        //
        RxStrobeInvPiDelay = QCLKPITICKS;
      } else {
        RxStrobeInvPiDelay = 0;
      }

      switch (group) {
      case RxStrobeInvert:
        if (RxDfeControl.Bits.dfe_en == 1) {
          if (RxStrobeInvPiDelay == QCLKPITICKS) {
            curVal = 0;
          } else {
            curVal = 1;
          }
        } else {
          curVal = (UINT16)RxStrobeInvCurrentRankOnEntry;
        }
        break;

      case RecEnDelay:
        curVal = (UINT16)rxGroup0.Bits.rcvendelay;
        break;

      case RxDqsPDelay:
        curVal = (UINT16)rxGroup0.Bits.rxdqspdelay + RxStrobeInvPiDelay;
        break;

      case RxDqsNDelay:
        curVal = (UINT16)rxGroup0.Bits.rxdqsndelay + RxStrobeInvPiDelay;
        break;

      case RxDqsDelay:
        curVal  = (UINT16)rxGroup0.Bits.rxdqspdelay + RxStrobeInvPiDelay;
        curVal2 = (UINT16)rxGroup0.Bits.rxdqsndelay + RxStrobeInvPiDelay;
        break;

      case RxDqDelay:
        curVal = (UINT16)rxGroup0.Bits.rxdqdelay;
        break;

      case RxDqDqsDelay:
        if (RxDfeControl.Bits.dfe_en == 1) {
          curVal  = (UINT16)rxGroup0.Bits.rxdqspdelay + RxStrobeInvPiDelay;
          curVal2 = (UINT16)rxGroup0.Bits.rxdqsndelay + RxStrobeInvPiDelay;
        } else {
          curVal = (UINT16)rxGroup0.Bits.rxdqspdelay + (5-(UINT16)rxGroup0.Bits.rxdqdelay)*8;
        }
        break;

      case RxEq:
        curVal = (UINT16)rxGroup0.Bits.rxeq;
        break;

      case RxDfeCoefficient:
        switch (logRank) {
        case 0:
          curVal = (UINT16)RxDfeCoeff.Bits.dfecoeff_rank0;
          break;
        case 1:
          curVal = (UINT16)RxDfeCoeff.Bits.dfecoeff_rank1;
          break;
        case 2:
          curVal = (UINT16)RxDfeCoeff.Bits.dfecoeff_rank2;
          break;
        case 3:
          curVal = (UINT16)RxDfeCoeff.Bits.dfecoeff_rank3;
          break;
        case 4:
          curVal = (UINT16)RxDfeCoeff.Bits.dfecoeff_rank4;
          break;
        case 5:
          curVal = (UINT16)RxDfeCoeff.Bits.dfecoeff_rank5;
          break;
        case 6:
          curVal = (UINT16)RxDfeCoeff.Bits.dfecoeff_rank6;
          break;
        case 7:
          curVal = (UINT16)RxDfeCoeff.Bits.dfecoeff_rank7;
          break;
        default:
          break;
        } //switch logrank
        break;

      default:
        break;
      } // switch
    }

    //
    // Check if read only
    //
    if (mode & GSM_READ_ONLY) {
      //
      // Save the value read into the output parameter
      //
      *value = curVal;
    } else {
      //
      // Write
      //
      // Adjust the current CMD delay value by offset
      if (mode & GSM_WRITE_OFFSET) {
        //
        // Add the offset to the current value
        //
        tmp = curVal + *value;
        if ((group == RxDqsDelay) || ((group == RxDqDqsDelay) && (RxDfeControl.Bits.dfe_en == 1))) {
          tmp2 = curVal2 + *value;
        }

      } else {
        //
        // Write absolute value
        //
        tmp = *value;
        if ((group == RxDqsDelay) || ((group == RxDqDqsDelay) && (RxDfeControl.Bits.dfe_en == 1))) {
          tmp2 = *value;
        }
      }

      //
      // Make sure we do not exeed the limits
      //
      if (tmp >= minLimit) {
        piDelay = tmp;
      } else {
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                       "%a Out of range!! tmp = 0x%x, minLimit = 0x%x\n", GetGroupStr (group), tmp, minLimit);
        piDelay = minLimit;
      }
      // Ensure we do not exceed maximums
      if (piDelay > maxLimit) {
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                       "%a Out of range!! piDelay = 0x%x, maxLimit = 0x%x\n", GetGroupStr (group), piDelay, maxLimit);
        piDelay = maxLimit;
      }

      if (((group == RxDqsDelay) || ((group == RxDqDqsDelay) && (RxDfeControl.Bits.dfe_en == 1))) && (level == DdrLevel)) {
        if (tmp2 >= minLimit) {
          piDelay2 = tmp2;
        } else {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                       "%a Out of range!! tmp2 = 0x%x, minLimit = 0x%x\n", GetGroupStr (group), tmp2, minLimit);
          piDelay2 = minLimit;
        }
        // Ensure we do not exceed maximums
        if (piDelay2 > maxLimit) {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                       "%a Out of range!! piDelay2 = 0x%x, maxLimit = 0x%x\n", GetGroupStr (group), piDelay2, maxLimit);
          piDelay2 = maxLimit;
        }
      }

      if (level == DdrLevel) {
        switch (group) {
        case RecEnDelay:
          rxGroup0.Bits.rcvendelay = piDelay;
          break;

        case RxDqsPDelay:
          if (RxDfeControl.Bits.dfe_en == 1) {
            piDelay2 = ((UINT16)rxGroup0.Bits.rxdqsndelay + RxStrobeInvPiDelay);
            ConvertPiDelayToRxDfeEnabledCsrValue (piDelay, piDelay2, &tmp, &tmp2);
            rxGroup0.Bits.rxdqspdelay = tmp;
            rxGroup0.Bits.rxdqsndelay = tmp2;
          } else {
            rxGroup0.Bits.rxdqspdelay = piDelay;
          }
          break;

        case RxDqsNDelay:
          if (RxDfeControl.Bits.dfe_en == 1) {
            piDelay2 = ((UINT16)rxGroup0.Bits.rxdqspdelay + RxStrobeInvPiDelay);
            ConvertPiDelayToRxDfeEnabledCsrValue (piDelay, piDelay2, &tmp, &tmp2);
            rxGroup0.Bits.rxdqsndelay = tmp;
            rxGroup0.Bits.rxdqspdelay = tmp2;
          } else {
            rxGroup0.Bits.rxdqsndelay = piDelay;
          }
          break;

        case RxDqsDelay:
          if (RxDfeControl.Bits.dfe_en == 1) {
            ConvertPiDelayToRxDfeEnabledCsrValue (piDelay, piDelay2, &tmp, &tmp2);
            rxGroup0.Bits.rxdqspdelay = tmp;
            rxGroup0.Bits.rxdqsndelay = tmp2;
          } else {
            rxGroup0.Bits.rxdqspdelay = piDelay;
            rxGroup0.Bits.rxdqsndelay = piDelay2;
          }
          break;

        case RxDqDelay:
          rxGroup0.Bits.rxdqdelay = piDelay;
          break;

        case RxDqDqsDelay:
          if (RxDfeControl.Bits.dfe_en == 1) {
            ConvertPiDelayToRxDfeEnabledCsrValue (piDelay, piDelay2, &tmp, &tmp2);
            rxGroup0.Bits.rxdqspdelay = tmp;
            rxGroup0.Bits.rxdqsndelay = tmp2;
          } else {
            if ( piDelay>71 ) {
              rxGroup0.Bits.rxdqdelay = 0;
              rxGroup0.Bits.rxdqspdelay = piDelay-40;
              rxGroup0.Bits.rxdqsndelay = piDelay-40;
            } else if (piDelay>63) {
              rxGroup0.Bits.rxdqdelay = 1;
              rxGroup0.Bits.rxdqspdelay = piDelay-32;
              rxGroup0.Bits.rxdqsndelay = piDelay-32;
            } else if (piDelay>55) {
              rxGroup0.Bits.rxdqdelay = 2;
              rxGroup0.Bits.rxdqspdelay = piDelay-24;
              rxGroup0.Bits.rxdqsndelay = piDelay-24;
            } else if (piDelay>47) {
              rxGroup0.Bits.rxdqdelay = 3;
              rxGroup0.Bits.rxdqspdelay = piDelay-16;
              rxGroup0.Bits.rxdqsndelay = piDelay-16;
            } else if (piDelay>39) {
              rxGroup0.Bits.rxdqdelay = 4;
              rxGroup0.Bits.rxdqspdelay = piDelay-8;
              rxGroup0.Bits.rxdqsndelay = piDelay-8;
            } else {
              rxGroup0.Bits.rxdqdelay = 5;
              rxGroup0.Bits.rxdqspdelay = piDelay;
              rxGroup0.Bits.rxdqsndelay = piDelay;
            }
          }
          break;

        case RxEq:
          rxGroup0.Bits.rxeq = piDelay;
          break;

        case RxStrobeInvert:
          //
          //Unlike other parameter types that modify the register value with the piDelay setting in this part of the switch statement,
          // the RxStrobeInvert value will be updated in the register after the switch statement because of the interdependencies
          // on this value and other values.
          //
          if ((RxDfeControl.Bits.dfe_en == 1) && (piDelay == 0)) {
            //
            // If DFE is enabled, the current per rank strobe inversion setting impacts the Delay
            //
            piDelay = QCLKPITICKS;
            piDelay2 = QCLKPITICKS;
          } else {
            piDelay = 0;
            piDelay2 = 0;
          }
          break;

        case RxDfeCoefficient:
          switch (logRank) {
          case 0:
            RxDfeCoeff.Bits.dfecoeff_rank0 = piDelay;
            break;
          case 1:
            RxDfeCoeff.Bits.dfecoeff_rank1 = piDelay;
            break;
          case 2:
            RxDfeCoeff.Bits.dfecoeff_rank2 = piDelay;
            break;
          case 3:
            RxDfeCoeff.Bits.dfecoeff_rank3 = piDelay;
            break;
          case 4:
            RxDfeCoeff.Bits.dfecoeff_rank4 = piDelay;
            break;
          case 5:
            RxDfeCoeff.Bits.dfecoeff_rank5 = piDelay;
            break;
          case 6:
            RxDfeCoeff.Bits.dfecoeff_rank6 = piDelay;
            break;
          case 7:
            RxDfeCoeff.Bits.dfecoeff_rank7 = piDelay;
            break;
          default:
            break;
          } //switch (logrank)
          break;

        default:
          break;
        } // switch
        if ((RxDfeControl.Bits.dfe_en == 1) && ((group == RxDqsPDelay) || (group == RxDqsNDelay) || (group == RxDqsDelay) || (group == RxDqDqsDelay) || (group == RxStrobeInvert))) {
          if ((piDelay < QCLKPITICKS) || (piDelay2 < QCLKPITICKS)) {
            //
            // Set Strobe Inv
            //
            RxStrobeInvNew = 1;
            GetSetRxStrobeInvBit (logRank, &RxDfeControl, 0, &RxStrobeInvNew);
            if (RxStrobeInvCurrentRankOnEntry == 0) {
              //
              // if strobes rcven and pi are < 32, subtract 1 QCLK from RTL for the given rank and
              // increase the receive enable logic delay and pi value by 2 x 64 ticks for all strobes
              //
              if (rxGroup0.Bits.rcvendelay < (QCLKPITICKS / 2)) {

                GetRoundTrip (socket, ch, GetLogicalRank (Host, socket, ch, dimm, rank), GSM_READ_ONLY, &RoundTrip);
                RoundTrip = RoundTrip - 2;
                SetRoundTrip (socket, ch, logRank, RoundTrip);
                RcvEnOffset = QCLKPITICKS * 2;
                GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, NO_BIT, level, RecEnDelay,
                                     GSM_WRITE_OFFSET + GSM_FORCE_WRITE + GSM_UPDATE_CACHE, &RcvEnOffset);

                rxGroup0.Bits.rcvendelay += QCLKPITICKS * 2;
              }
              //
              //Decrease this strobes rcven logic delay and pi values by 32 pi ticks
              //
              rxGroup0.Bits.rcvendelay -= (QCLKPITICKS / 2);
            } //RxStrobeInvCurrentRankOnEntry

          } else {
            //
            // Clear Strobe Inv
            //
            RxStrobeInvNew = 0;
            GetSetRxStrobeInvBit (logRank, &RxDfeControl, 0, &RxStrobeInvNew);
            if (RxStrobeInvCurrentRankOnEntry == 1) {
              //
              //Increase the strobe's RCVEN logic delay and pi values by 32 pi ticks
              //
              rxGroup0.Bits.rcvendelay += (QCLKPITICKS / 2);
            } //RxStrobeInvCurrentRankOnEntry
          }
        }

      } //DdrLevel

#if defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)
      if (level == LrbufLevel) {
        //
        // first translate encoded value to register value
        //
        //curVal = piDelay >> 1;
        curVal = piDelay;

        switch (group) {
          case RxDqsDelay:
            if (curVal < 16) {
              if ((*RankList)[rank].lrBuf_FxBC4x5x[strobe] != (UINT8) (curVal ^ 31)) {
                updateNeeded = 1;
                (*RankList)[rank].lrBuf_FxBC4x5x[strobe] = (UINT8) (curVal ^ 31);
              }
            } else {
              if ((*RankList)[rank].lrBuf_FxBC4x5x[strobe] != (UINT8) (curVal - 15)) {
                updateNeeded = 1;
                (*RankList)[rank].lrBuf_FxBC4x5x[strobe] = (UINT8) (curVal - 15);
              }
            }
            break;
          case RecEnDelay:
            if ((*RankList)[rank].lrBuf_FxBC2x3x[strobe] != (UINT8) curVal) {
              updateNeeded = 1;
              (*RankList)[rank].lrBuf_FxBC2x3x[strobe] = (UINT8) curVal;
            }
            break;
          case RecEnDelayCycle:
            if ((((*RankList)[rank].lrBuf_FxBCCxEx[strobe % MSVx8]>>(4 * (strobe / MSVx8))) & 0xF) != (UINT8) curVal) {
              updateNeeded = 1;
              (*RankList)[rank].lrBuf_FxBCCxEx[strobe % MSVx8] &= 0xF0 >> (4 * (strobe / MSVx8));
              (*RankList)[rank].lrBuf_FxBCCxEx[strobe % MSVx8] |= (UINT8) curVal<<(4 * (strobe / MSVx8));
            }
            break;
          default:
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                           "GetSetRxDelay called unsupported group!\n");
            RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_60);
            break;
        } //switch group
      } else
#endif // defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)
      {
        //
        // Write it back if the current data does not equal the cache value or if in force write mode
        //
        if ((rxGroup0.Data != (*channelNvList)[ch].rxGroup0[logRank][strobe]) || (mode & GSM_FORCE_WRITE)) {
          WriteDdrioDataReg (Host, socket, ch, logRank, strobe, RXGROUP0N0RANK0_0_MCIO_DDRIO_REG, rxGroup0.Data);
        }
        if ((RxDfeCoeff.Data != (*channelNvList)[ch].RxDfeCoeff[strobe]) || (mode & GSM_FORCE_WRITE)) {
          WriteDdrioDataRegCh (Host, socket, ch, strobe, DDRCRDATARXDFECOEFF_MCIO_DDRIO_REG, RxDfeCoeff.Data);
        }
        if ((RxDfeControl.Data != (*channelNvList)[ch].RxDfeControl[strobe]) || (mode & GSM_FORCE_WRITE)) {
          WriteDdrioDataRegCh (Host, socket, ch, strobe, DDRCRDATARXDFECONTROL_MCIO_DDRIO_REG, RxDfeControl.Data);
        }

        if ((rxGroup0.Data != (*channelNvList)[ch].rxGroup0[logRank][strobe]) ||
            (RxDfeCoeff.Data != (*channelNvList)[ch].RxDfeCoeff[strobe]) ||
            (RxDfeControl.Data != (*channelNvList)[ch].RxDfeControl[strobe]) ||
            (mode & GSM_FORCE_WRITE)) {
          //
          // Wait for the new value to settle
          //
          FixedDelayMicroSecond (usDelay);
        }
      }
    } // if read only

    //
    // Save to cache
    //
    if (mode & GSM_UPDATE_CACHE) {
      if (level == DdrLevel) {
        (*channelNvList)[ch].rxGroup0[logRank][strobe] = rxGroup0.Data;
        (*channelNvList)[ch].RxDfeCoeff[strobe] = RxDfeCoeff.Data;
        (*channelNvList)[ch].RxDfeControl[strobe] = RxDfeControl.Data;
      }
    }

  } // strobe loop

#if defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)
  if ((level == LrbufLevel) && !(mode & GSM_READ_ONLY) && ((updateNeeded) || (mode & GSM_FORCE_WRITE))) {
    switch (group) {
      case RxDqsDelay:
        controlWordDataPtr = &((*RankList)[rank].lrBuf_FxBC4x5x[0]);
        WriteLrbufPBA(Host, socket, ch, dimm, rank, controlWordDataPtr, rank, LRDIMM_BC4x, ALL_DATABUFFERS);
        controlWordDataPtr = &((*RankList)[rank].lrBuf_FxBC4x5x[MSVx8]);
        WriteLrbufPBA(Host, socket, ch, dimm, rank, controlWordDataPtr, rank, LRDIMM_BC5x, ALL_DATABUFFERS);
        break;
      case RecEnDelay:
        controlWordDataPtr = &((*RankList)[rank].lrBuf_FxBC2x3x[0]);
        WriteLrbufPBA(Host, socket, ch, dimm, rank, controlWordDataPtr, rank, LRDIMM_BC2x, ALL_DATABUFFERS);
        controlWordDataPtr = &((*RankList)[rank].lrBuf_FxBC2x3x[MSVx8]);
        WriteLrbufPBA(Host, socket, ch, dimm, rank, controlWordDataPtr, rank, LRDIMM_BC3x, ALL_DATABUFFERS);
        break;
      case RecEnDelayCycle:
        controlWordDataPtr = &((*RankList)[rank].lrBuf_FxBCCxEx[0]);
        controlWordAddr = LRDIMM_BCCx + ((rank >> 1) * 0x20);
        WriteLrbufPBA(Host, socket, ch, dimm, 0, controlWordDataPtr, (rank & BIT0) , controlWordAddr, ALL_DATABUFFERS);
        break;
      default:
        break;
    } // end switch
    //
    // Wait for the new value to settle
    //
    FixedDelayMicroSecond (usDelay);
  }

  if (mode & GSM_UPDATE_CACHE) {
    if (level == LrbufLevel) {
      switch (group) {
        case RxDqsDelay:
          for (tmpStrobe = 0; tmpStrobe < MSVx4; tmpStrobe++) {
            (*rankStruct)[rank].cachedLrBuf_FxBC4x5x[tmpStrobe] =  (*RankList)[rank].lrBuf_FxBC4x5x[tmpStrobe];
          }
          break;
        case RecEnDelay:
          for (tmpStrobe = 0; tmpStrobe < MSVx4; tmpStrobe++) {
            (*rankStruct)[rank].cachedLrBuf_FxBC2x3x[tmpStrobe] =  (*RankList)[rank].lrBuf_FxBC2x3x[tmpStrobe];
          }
          break;
        case RecEnDelayCycle:
          for (tmpStrobe = 0; tmpStrobe < MSVx8; tmpStrobe++) {
            (*rankStruct)[rank].cachedLrBuf_FxBCCxEx[tmpStrobe] =  (*RankList)[rank].lrBuf_FxBCCxEx[tmpStrobe];
          }
        break;
        default:
          break;
      }
    }
  }
#endif // defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)

  return MRC_STATUS_SUCCESS;
} // GetSetRxDelayUnicast

/**

  Get RxDelay
  Wrapper for multicast and unicast versions of GetSetRxDelay

  @param[in]      Host    - Pointer to sysHost
  @param[in]      Socket  - Socket number
  @param[in]      Ch      - Channel number (0-based)
  @param[in]      Dimm    - DIMM number (0-based)
  @param[in]      Rank    - Rank number (0-based)
  @param[in]      Strobe  - Strobe number (0-based)
  @param[in]      Bit     - Bit number
  @param[in]      Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]      Group   - CSM_GT - Parameter to program
  @param[in]      Mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param[in,out]  Value   - Data to program

  @retval IO delay

**/
MRC_STATUS
GetSetRxDelay (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      UINT8     Strobe,
  IN      UINT8     Bit,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  )
{
  MRC_STATUS      Status = MRC_STATUS_FAILURE;
  //
  // Use multicast version if possible
  // Multicast register value is added, in HW, to all strobe values in all channels on the MC
  //
  if ((*Value > 0) && UseDelayMulticastOffset (Strobe, Bit, Level, Group, Mode)) {
    Status = GetSetDelayMulticastOffset (Host, Socket, Ch, Dimm, Rank, Strobe, Bit, Level, RxDqsPerChDelayOffset, Mode, Value);
  }
  if (Status != MRC_STATUS_SUCCESS) {
    //
    // Multicast not available
    // Force multicast register to 0 (in case it isn't already 0) then call the unicast routine
    //
    if ((*Value == 0) && UseDelayMulticastOffset (Strobe, Bit, Level, Group, Mode)) {
      //
      // This should not result in a register access if the Multicast register is already 0.
      // GetSetxxx will return without accessing HW if the cached value already matches the value passed in.
      //
      GetSetDelayMulticastOffset (Host, Socket, Ch, Dimm, Rank, Strobe, Bit, Level, RxDqsPerChDelayOffset, Mode, Value);
    }
    Status = GetSetRxDelayUnicast (Host, Socket, Ch, Dimm, Rank, Strobe, Bit, Level, Group, Mode, Value);
  }
  return Status;
}


/**

  Get RxDelay per-bit value

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param value   - Pointer data to read or write. For writes, must point to an array of values for every bit in the nibble

  @retval IO delay

**/
MRC_STATUS
GetSetRxDelayBit (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value
  )
{
  UINT8                               logRank;
  UINT8                               maxStrobe;
  UINT8                               i;
  UINT8                               RoundTrip;
  UINT8                               StrobeN[MAX_NIBBLES_PER_BYTE];
  UINT8                               SwizzleBit[MAX_BITS_IN_BYTE];
  UINT8                               NibbleIndex;
  UINT8                               NibbleOffset;
  UINT8                               BitOffset;
  UINT8                               BitIndex = 0;
  UINT16                              curVal[MAX_BITS_IN_BYTE];
  UINT16                              piDelayNibble[MAX_NIBBLES_PER_BYTE];
  UINT16                              piDelayNibble2;
  UINT16                              piDelay;
  UINT16                              piDelayMin;
  UINT16                              piDelayMax;
  UINT16                              delta;
  UINT16                              piDelayBit[MAX_BITS_IN_BYTE];
  INT16                               clkSel[MAX_NIBBLES_PER_BYTE];
  UINT16                              maxLimit = 0;
  UINT16                              minLimit = 0;
  UINT16                              maxPerBitLimit = 0;
  UINT16                              minPerBitLimit = 0;
  UINT16                              usDelay = 0;
  struct channelNvram                 (*channelNvList)[MAX_CH];
  struct RankCh                       (*RankPerCh)[MAX_RANK_CH];
  struct ddrRank                      (*RankList)[MAX_RANK_DIMM];
  RXGROUP0N0RANK0_0_MCIO_DDRIO_STRUCT RxGroup0[MAX_NIBBLES_PER_BYTE];
  RXGROUP1NRANK0_MCIO_DDRIO_STRUCT    RxGroup1n[MAX_NIBBLES_PER_BYTE];
  RXGROUP1PRANK0_MCIO_DDRIO_STRUCT    RxGroup1p[MAX_NIBBLES_PER_BYTE];
  DDRCRDATARXDFECONTROL_MCIO_DDRIO_STRUCT RxDfeControl[MAX_NIBBLES_PER_BYTE];
  INT16                               RxStrobeInvCurrentRankOnEntry[MAX_NIBBLES_PER_BYTE];
  INT16                               RxStrobeInvNew = 0;
  UINT16                              RxStrobeInvPiDelay[MAX_NIBBLES_PER_BYTE];
  INT16                               RcvEnOffset = 0;
  UINT16                              TempValue;
  UINT16                              TempValue2;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;


  channelNvList = GetChannelNvList (Host, socket);
  RankPerCh = &(*channelNvList)[ch].rankPerCh;
  RankList = GetRankNvList (Host, socket, ch, dimm);

  RxGroup1n[INDEX_NIBBLE0].Data = 0;
  RxGroup1p[INDEX_NIBBLE0].Data = 0;
  RxGroup1n[INDEX_NIBBLE1].Data = 0;
  RxGroup1p[INDEX_NIBBLE1].Data = 0;

  RxGroup0[INDEX_NIBBLE0].Data  = 0;
  RxGroup0[INDEX_NIBBLE1].Data  = 0;

  RxDfeControl[INDEX_NIBBLE0].Data  = 0;
  RxDfeControl[INDEX_NIBBLE1].Data  = 0;

  piDelayNibble[INDEX_NIBBLE0]  = 0;
  piDelayNibble[INDEX_NIBBLE1]  = 0;

  RxStrobeInvPiDelay[INDEX_NIBBLE0] = 0;
  RxStrobeInvPiDelay[INDEX_NIBBLE1] = 0;

  RxStrobeInvCurrentRankOnEntry[INDEX_NIBBLE0] = 0;
  RxStrobeInvCurrentRankOnEntry[INDEX_NIBBLE1] = 0;

  piDelayNibble2 = 0;
  //
  // Get the logical rank #
  //
  logRank = GetLogicalRank (Host, socket, ch, dimm, rank);

  //
  // Get minimum and maximum value for the overall RxDqDqsDelay, and the per bit Delays
  //
  GetDataGroupLimits (Host, level, RxDqsDelay, &minLimit, &maxLimit, &usDelay);
  GetDataGroupLimits (Host, level, group, &minPerBitLimit, &maxPerBitLimit, &usDelay);

  if (strobe >= MAX_STROBE && strobe != ALL_STROBES) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                   "The strobe value is out of range!\n");
    RC_FATAL_ERROR ((strobe < MAX_STROBE || strobe == ALL_STROBES), ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  if (strobe == ALL_STROBES) {
    strobe    = 0;
    maxStrobe = MSVx4;
  } else {
    maxStrobe = strobe + 1;
  }

  if ((mode & GSM_READ_ONLY) == GSM_READ_ONLY) {
    //
    // For reads, specific bit must be specified
    //
    if (bit >= BITS_PER_NIBBLE) {
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_61);
      return MRC_STATUS_FAILURE;
    }
  } else {
    //
    // For writes, must specify the entire nibble (ALL_BITS)
    //
    RC_FATAL_ERROR (bit == ALL_BITS, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_163);
  }

  for (; strobe < maxStrobe; strobe++) {

    StrobeN[INDEX_NIBBLE0] = strobe % MSVx8;
    StrobeN[INDEX_NIBBLE1] = (strobe % MSVx8) + MSVx8;

    if (mode & GSM_READ_CSR) {
      //
      // Read from PCI config space
      //
      RxGroup0[INDEX_NIBBLE0].Data      = ReadDdrioDataReg (Host, socket, ch, logRank, StrobeN[INDEX_NIBBLE0], RXGROUP0N0RANK0_0_MCIO_DDRIO_REG);
      RxGroup1n[INDEX_NIBBLE0].Data     = ReadDdrioDataReg (Host, socket, ch, logRank, StrobeN[INDEX_NIBBLE0], RXGROUP1NRANK0_MCIO_DDRIO_REG);
      RxGroup1p[INDEX_NIBBLE0].Data     = ReadDdrioDataReg (Host, socket, ch, logRank, StrobeN[INDEX_NIBBLE0], RXGROUP1PRANK0_MCIO_DDRIO_REG);
      RxDfeControl[INDEX_NIBBLE0].Data  = ReadDdrioDataRegCh (Host, socket, ch, StrobeN[INDEX_NIBBLE0], DDRCRDATARXDFECONTROL_MCIO_DDRIO_REG);

      RxGroup0[INDEX_NIBBLE1].Data      = ReadDdrioDataReg (Host, socket, ch, logRank, StrobeN[INDEX_NIBBLE1], RXGROUP0N0RANK0_0_MCIO_DDRIO_REG);
      RxGroup1n[INDEX_NIBBLE1].Data     = ReadDdrioDataReg (Host, socket, ch, logRank, StrobeN[INDEX_NIBBLE1], RXGROUP1NRANK0_MCIO_DDRIO_REG);
      RxGroup1p[INDEX_NIBBLE1].Data     = ReadDdrioDataReg (Host, socket, ch, logRank, StrobeN[INDEX_NIBBLE1], RXGROUP1PRANK0_MCIO_DDRIO_REG);
      RxDfeControl[INDEX_NIBBLE1].Data  = ReadDdrioDataRegCh (Host, socket, ch, StrobeN[INDEX_NIBBLE1], DDRCRDATARXDFECONTROL_MCIO_DDRIO_REG);

    } else {
      //
      // Read from cache
      //
      RxGroup0[INDEX_NIBBLE0].Data      = (*channelNvList)[ch].rxGroup0[logRank][StrobeN[INDEX_NIBBLE0]];
      RxGroup1n[INDEX_NIBBLE0].Data     = (*channelNvList)[ch].rxGroup1n[logRank][StrobeN[INDEX_NIBBLE0]];
      RxGroup1p[INDEX_NIBBLE0].Data     = (*channelNvList)[ch].rxGroup1p[logRank][StrobeN[INDEX_NIBBLE0]];
      RxDfeControl[INDEX_NIBBLE0].Data  = (*channelNvList)[ch].RxDfeControl[StrobeN[INDEX_NIBBLE0]];

      RxGroup0[INDEX_NIBBLE1].Data      = (*channelNvList)[ch].rxGroup0[logRank][StrobeN[INDEX_NIBBLE1]];
      RxGroup1n[INDEX_NIBBLE1].Data     = (*channelNvList)[ch].rxGroup1n[logRank][StrobeN[INDEX_NIBBLE1]];
      RxGroup1p[INDEX_NIBBLE1].Data     = (*channelNvList)[ch].rxGroup1p[logRank][StrobeN[INDEX_NIBBLE1]];
      RxDfeControl[INDEX_NIBBLE1].Data  = (*channelNvList)[ch].RxDfeControl[StrobeN[INDEX_NIBBLE1]];
    }

    for (i = 0; i < MAX_BITS_IN_BYTE; i++) {
      //
      // Get Swizzle Bit
      //
      SwizzleBit[i]   = GetDqSwizzle (ch, StrobeN[INDEX_NIBBLE0], i, level, group);
    }

    for (NibbleIndex = 0; NibbleIndex < MAX_NIBBLES_PER_BYTE; NibbleIndex++) {

      NibbleOffset = (NibbleIndex * BITS_PER_NIBBLE);

      RxStrobeInvPiDelay[NibbleIndex] = 0;
      //
      // If DFE is enabled, get the current per rank strobe inversion setting
      //
      if (RxDfeControl[NibbleIndex].Bits.dfe_en == 1) {
        GetSetRxStrobeInvBit (logRank, &RxDfeControl[NibbleIndex], GSM_READ_ONLY, &RxStrobeInvCurrentRankOnEntry[NibbleIndex]);
        if (RxStrobeInvCurrentRankOnEntry[NibbleIndex] == 0) {
          RxStrobeInvPiDelay[NibbleIndex] = 64;
        } else {
          RxStrobeInvPiDelay[NibbleIndex] = 0;
        }
      } //dfe_en

      switch (group) {
      case RxDqsBitDelay:
      case RxDqsPBitDelay:
        piDelayNibble[NibbleIndex] = (UINT16)RxGroup0[NibbleIndex].Bits.rxdqspdelay + RxStrobeInvPiDelay[NibbleIndex];
        piDelayBit[ 0 + NibbleOffset ] = (UINT16)RxGroup1p[NibbleIndex].Bits.rxdqspdelay0;
        piDelayBit[ 1 + NibbleOffset ] = (UINT16)RxGroup1p[NibbleIndex].Bits.rxdqspdelay1;
        piDelayBit[ 2 + NibbleOffset ] = (UINT16)RxGroup1p[NibbleIndex].Bits.rxdqspdelay2;
        piDelayBit[ 3 + NibbleOffset ] = (UINT16)RxGroup1p[NibbleIndex].Bits.rxdqspdelay3;
        break;
      case RxDqsNBitDelay:
        piDelayNibble[NibbleIndex] = (UINT16)RxGroup0[NibbleIndex].Bits.rxdqsndelay + RxStrobeInvPiDelay[NibbleIndex];
        piDelayBit[ 0 + NibbleOffset ] = (UINT16)RxGroup1n[NibbleIndex].Bits.rxdqsndelay0;
        piDelayBit[ 1 + NibbleOffset ] = (UINT16)RxGroup1n[NibbleIndex].Bits.rxdqsndelay1;
        piDelayBit[ 2 + NibbleOffset ] = (UINT16)RxGroup1n[NibbleIndex].Bits.rxdqsndelay2;
        piDelayBit[ 3 + NibbleOffset ] = (UINT16)RxGroup1n[NibbleIndex].Bits.rxdqsndelay3;
        break;
      default:
        piDelayNibble[NibbleIndex] = 0;
        piDelayBit[ 0 + NibbleOffset ] = 0;
        piDelayBit[ 1 + NibbleOffset ] = 0;
        piDelayBit[ 2 + NibbleOffset ] = 0;
        piDelayBit[ 3 + NibbleOffset ] = 0;
        break;
      }

      for (i = 0; i < BITS_PER_NIBBLE; i++) {

        BitOffset = i + NibbleOffset;

        piDelayBit[ BitOffset ] += piDelayNibble[NibbleIndex];
      }
    } // NibbleIndex loop

    for (i = 0; i < MAX_BITS_IN_BYTE; i++) {
      curVal[i] = piDelayBit[ SwizzleBit[i] ];
    }

    //
    // Check if read only
    //
    if ((mode & GSM_READ_ONLY) == GSM_READ_ONLY) {
      //
      // Save the value read into the output parameter
      //
      BitIndex = bit;
      if (strobe >= MSVx8) {
        BitIndex += BITS_PER_NIBBLE;
      }

      *value = curVal[BitIndex];
    } else {
      //
      // Write
      //
      NibbleIndex = 0;
      if (strobe >= MSVx8) {
        NibbleIndex = 1;
      }
      NibbleOffset = NibbleIndex * BITS_PER_NIBBLE;
      for (i = 0; i < BITS_PER_NIBBLE; i++) {
        if (mode & GSM_WRITE_OFFSET) {
          //
          // Add the offset to the current value
          //
          piDelay = curVal[i + NibbleOffset] + value[i];

        } else {
          //
          // Write absolute value
          //
          piDelay = value[i];
        }
        //
        // Adjust target bit delay
        //
        piDelayBit[ SwizzleBit[i + NibbleOffset] ] = piDelay;
      } // i

      for (NibbleIndex = 0; NibbleIndex < MAX_NIBBLES_PER_BYTE; NibbleIndex++) {

        NibbleOffset = (NibbleIndex * BITS_PER_NIBBLE);
        //
        // Normalize smallest per bit delay to 0
        //
        piDelayMax = 0;
        piDelayMin = 0xFF;
        clkSel[NibbleIndex] = 0;
        for (i = 0; i < BITS_PER_NIBBLE; i++) {

          BitOffset = i + NibbleOffset;

          //
          // Determine largest per-bit delay across
          // N[x].picode0,1,2,3, so de-swizzle offset in the array
          // and find the greater value
          //
          if (piDelayBit[BitOffset] > piDelayMax) {
            piDelayMax = piDelayBit[BitOffset];
            clkSel[NibbleIndex] = i;
          }
          if (piDelayBit[BitOffset] < piDelayMin) {
            piDelayMin = piDelayBit[BitOffset];
          }
        }

        delta = piDelayMin - piDelayNibble[NibbleIndex];
        piDelayNibble[NibbleIndex] += delta;
        //
        // Make sure we do not exeed the limits
        //
        if (piDelayNibble[NibbleIndex] < minLimit) {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, bit,
            "%a Out of range!! piDelayNibble = 0x%x, minLimit = 0x%x\n", GetGroupStr (RxDqsDelay), piDelayNibble[NibbleIndex], minLimit);
          piDelayNibble[NibbleIndex] = minLimit;
        }
        if (piDelayNibble[NibbleIndex] > maxLimit) {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, bit,
            "%a Out of range!! piDelayNibble = 0x%x, maxLimit = 0x%x\n", GetGroupStr (RxDqsDelay), piDelayNibble[NibbleIndex], maxLimit);
          piDelayNibble[NibbleIndex] = maxLimit;
        }

        for (i = 0; i < BITS_PER_NIBBLE; i++) {

          BitOffset = i + NibbleOffset;

          piDelayBit[BitOffset] = piDelayBit[BitOffset] - piDelayNibble[NibbleIndex];
          //
          // Check per bit limits
          //
          if (piDelayBit[BitOffset] > maxPerBitLimit) {
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, bit,
              "%a Out of range!! piDelayBit[%d] = 0x%x, maxPerBitLimit = 0x%x\n", GetGroupStr (group), i, piDelayBit[BitOffset], maxPerBitLimit);
            piDelayBit[BitOffset] = maxPerBitLimit;
          }
          if (piDelayBit[BitOffset] < minPerBitLimit){
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, bit,
              "%a Out of range!! piDelayBit[%d] = 0x%x, minPerBitLimit = 0x%x\n", GetGroupStr (group), i, piDelayBit[BitOffset], minPerBitLimit);
            piDelayBit[BitOffset] = minPerBitLimit;
          }
        }
      } // NibbleIndex loop

      for (NibbleIndex = 0; NibbleIndex < MAX_NIBBLES_PER_BYTE; NibbleIndex++) {

        NibbleOffset = (NibbleIndex * BITS_PER_NIBBLE);

        switch (group) {
        case RxDqsBitDelay:
          RxGroup0[NibbleIndex].Bits.rxdqspdelay = piDelayNibble[NibbleIndex];
          RxGroup0[NibbleIndex].Bits.rxdqsndelay = piDelayNibble[NibbleIndex];
          if (RxDfeControl[NibbleIndex].Bits.dfe_en == 1) {
            piDelayNibble2 = piDelayNibble[NibbleIndex];
            ConvertPiDelayToRxDfeEnabledCsrValue (piDelayNibble[NibbleIndex], piDelayNibble2, &TempValue, &TempValue2);
            RxGroup0[NibbleIndex].Bits.rxdqspdelay = TempValue;
            RxGroup0[NibbleIndex].Bits.rxdqsndelay = TempValue2;
          }
          break;

        case RxDqsPBitDelay:
          RxGroup0[NibbleIndex].Bits.rxdqspdelay = piDelayNibble[NibbleIndex];
          if (RxDfeControl[NibbleIndex].Bits.dfe_en == 1) {
            piDelayNibble2 = (UINT16)RxGroup0[NibbleIndex].Bits.rxdqsndelay + RxStrobeInvPiDelay[NibbleIndex];
            ConvertPiDelayToRxDfeEnabledCsrValue (piDelayNibble[NibbleIndex], piDelayNibble2, &TempValue, &TempValue2);
            RxGroup0[NibbleIndex].Bits.rxdqspdelay = TempValue;
            RxGroup0[NibbleIndex].Bits.rxdqsndelay = TempValue2;
          }
          break;

        case RxDqsNBitDelay:
          RxGroup0[NibbleIndex].Bits.rxdqsndelay = piDelayNibble[NibbleIndex];
          if (RxDfeControl[NibbleIndex].Bits.dfe_en == 1) {
            piDelayNibble2 = (UINT16)RxGroup0[NibbleIndex].Bits.rxdqspdelay + RxStrobeInvPiDelay[NibbleIndex];
            ConvertPiDelayToRxDfeEnabledCsrValue (piDelayNibble[NibbleIndex], piDelayNibble2, &TempValue, &TempValue2);
            RxGroup0[NibbleIndex].Bits.rxdqsndelay = TempValue;
            RxGroup0[NibbleIndex].Bits.rxdqspdelay = TempValue2;
          }
          break;
        default:
          break;
        } //switch group

        if (group == RxDqsBitDelay || group == RxDqsPBitDelay) {
          RxGroup1p[NibbleIndex].Bits.rxdqspdelay0 = piDelayBit[ 0 + NibbleOffset ];
          RxGroup1p[NibbleIndex].Bits.rxdqspdelay1 = piDelayBit[ 1 + NibbleOffset ];
          RxGroup1p[NibbleIndex].Bits.rxdqspdelay2 = piDelayBit[ 2 + NibbleOffset ];
          RxGroup1p[NibbleIndex].Bits.rxdqspdelay3 = piDelayBit[ 3 + NibbleOffset ];
        }

        if (group == RxDqsBitDelay || group == RxDqsNBitDelay) {
          RxGroup1n[NibbleIndex].Bits.rxdqsndelay0 = piDelayBit[ 0 + NibbleOffset ];
          RxGroup1n[NibbleIndex].Bits.rxdqsndelay1 = piDelayBit[ 1 + NibbleOffset ];
          RxGroup1n[NibbleIndex].Bits.rxdqsndelay2 = piDelayBit[ 2 + NibbleOffset ];
          RxGroup1n[NibbleIndex].Bits.rxdqsndelay3 = piDelayBit[ 3 + NibbleOffset ];
        }

        if (RxDfeControl[NibbleIndex].Bits.dfe_en == 1) {
          if ((piDelayNibble[NibbleIndex] < 64) || (piDelayNibble2 < 64)) {
            //
            // Set Strobe Inv
            //
            RxStrobeInvNew = 1;
            GetSetRxStrobeInvBit (logRank, &RxDfeControl[NibbleIndex], 0, &RxStrobeInvNew);
            if (RxStrobeInvCurrentRankOnEntry[NibbleIndex] == 0) {
              //
              // if strobes rcven and pi are < 32, subtract 1 QCLK from RTL for the given rank and
              // increase the receive enable logic delay and pi value by 64 ticks for all strobes
              //
              if (RxGroup0[NibbleIndex].Bits.rcvendelay < 32) {

                GetRoundTrip (socket, ch, logRank, GSM_READ_ONLY, &RoundTrip);
                RoundTrip -= 2;
                SetRoundTrip (socket, ch, logRank, RoundTrip);
                RcvEnOffset = 128;
                GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, NO_BIT, level, RecEnDelay,
                                     GSM_WRITE_OFFSET + GSM_FORCE_WRITE + GSM_UPDATE_CACHE, &RcvEnOffset);

                RxGroup0[NibbleIndex].Bits.rcvendelay += 128;
              }
              //
              //Decrease this strobes rcven logic delay and pi values by 32 pi ticks
              //
              RxGroup0[NibbleIndex].Bits.rcvendelay -= 32;
            } //RxStrobeInvCurrentRankOnEntry
          } else {
            //
            // Clear Strobe Inv
            //
            RxStrobeInvNew = 0;
            GetSetRxStrobeInvBit (logRank, &RxDfeControl[NibbleIndex], 0, &RxStrobeInvNew);
            if (RxStrobeInvCurrentRankOnEntry[NibbleIndex] == 1) {
              //
              //Increase the strobe's RCVEN logic delay and pi values by 32 pi ticks
              //
              RxGroup0[NibbleIndex].Bits.rcvendelay += 32;
            } //RxStrobeInvCurrentRankOnEntry
          } //((piDelayNibble < 64) || (piDelayNibble2 < 64))
        } //dfe_en

        //
        // 'clkSel' variable has the Index that has the largest piDelay,
        // across its own nibble, no matter the swizzling
        //
        switch (group) {
        case RxDqsBitDelay:
          GetSetSenseAmpRxOffsetDdrio (Host, socket, ch, dimm, rank, StrobeN[NibbleIndex], NO_BIT, DdrLevel, RxFifoDqsPBitSkew, (mode & GSM_UPDATE_CACHE), &clkSel[NibbleIndex]);
          GetSetSenseAmpRxOffsetDdrio (Host, socket, ch, dimm, rank, StrobeN[NibbleIndex], NO_BIT, DdrLevel, RxFifoDqsNBitSkew, (mode & GSM_UPDATE_CACHE), &clkSel[NibbleIndex]);
          break;
        case RxDqsPBitDelay:
          GetSetSenseAmpRxOffsetDdrio (Host, socket, ch, dimm, rank, StrobeN[NibbleIndex], NO_BIT, DdrLevel, RxFifoDqsPBitSkew, (mode & GSM_UPDATE_CACHE), &clkSel[NibbleIndex]);
          break;
        case RxDqsNBitDelay:
          GetSetSenseAmpRxOffsetDdrio (Host, socket, ch, dimm, rank, StrobeN[NibbleIndex], NO_BIT, DdrLevel, RxFifoDqsNBitSkew, (mode & GSM_UPDATE_CACHE), &clkSel[NibbleIndex]);
          break;
        default:
          break;
        }

        //
        // Write it back if the current data does not equal the cache value or if in force write mode
        //
        if ((RxGroup0[NibbleIndex].Data != (*channelNvList)[ch].rxGroup0[logRank][ StrobeN[NibbleIndex] ]) || (mode & GSM_FORCE_WRITE)) {
          WriteDdrioDataReg (Host, socket, ch, logRank, StrobeN[NibbleIndex], RXGROUP0N0RANK0_0_MCIO_DDRIO_REG, RxGroup0[NibbleIndex].Data);
        }
        if ((RxGroup1n[NibbleIndex].Data != (*channelNvList)[ch].rxGroup1n[logRank][ StrobeN[NibbleIndex] ]) || (mode & GSM_FORCE_WRITE)) {
          WriteDdrioDataReg (Host, socket, ch, logRank, StrobeN[NibbleIndex], RXGROUP1NRANK0_MCIO_DDRIO_REG, RxGroup1n[NibbleIndex].Data);
        }
        if ((RxGroup1p[NibbleIndex].Data != (*channelNvList)[ch].rxGroup1p[logRank][ StrobeN[NibbleIndex] ]) || (mode & GSM_FORCE_WRITE)) {
          WriteDdrioDataReg (Host, socket, ch, logRank, StrobeN[NibbleIndex], RXGROUP1PRANK0_MCIO_DDRIO_REG, RxGroup1p[NibbleIndex].Data);
        }
        if ((RxDfeControl[NibbleIndex].Data != (*channelNvList)[ch].RxDfeControl[ StrobeN[NibbleIndex] ]) || (mode & GSM_FORCE_WRITE)) {
          WriteDdrioDataRegCh (Host, socket, ch, StrobeN[NibbleIndex], DDRCRDATARXDFECONTROL_MCIO_DDRIO_REG, RxDfeControl[NibbleIndex].Data);
        }

        //
        // Wait for the new value to settle
        //
        FixedDelayMicroSecond (usDelay);

      } // NibbleIndex loop

    } // if read only

    for (NibbleIndex = 0; NibbleIndex < MAX_NIBBLES_PER_BYTE; NibbleIndex++) {

      //
      // Save to cache
      //
      if (mode & GSM_UPDATE_CACHE) {
        (*channelNvList)[ch].rxGroup0[logRank][ StrobeN[NibbleIndex] ] = RxGroup0[NibbleIndex].Data;
        (*channelNvList)[ch].rxGroup1n[logRank][ StrobeN[NibbleIndex] ] = RxGroup1n[NibbleIndex].Data;
        (*channelNvList)[ch].rxGroup1p[logRank][ StrobeN[NibbleIndex] ] = RxGroup1p[NibbleIndex].Data;
        (*channelNvList)[ch].RxDfeControl[ StrobeN[NibbleIndex] ] = RxDfeControl[NibbleIndex].Data;
      }
    }
  } // strobe loop

  return MRC_STATUS_SUCCESS;
} // GetSetRxDelayBit

/**

  Get RxDelay

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param value   - Data to program

  @retval IO delay

**/
MRC_STATUS
GetSetRxVref (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value
  )
{
  UINT8                             maxStrobe;
  UINT8                             targetStrobe;
  UINT8                             logicalBit;
  UINT8                             indexBit = 0;
  UINT8                             bitStart = 0;
  UINT8                             BitStop  = 0;
  UINT8                             SwizzleBit = 1;
  UINT16                            curVal[MAX_BITS_IN_BYTE];
  UINT16                            piDelay[MAX_BITS_IN_BYTE];
  INT16                             tmp[MAX_BITS_IN_BYTE];
  UINT16                            maxLimit = 127;
  UINT16                            minLimit = 0;
  UINT16                            usDelay = 1;
  struct channelNvram               (*channelNvList)[MAX_CH];
  RXVREFCTRLN0_0_MCIO_DDRIO_STRUCT  rxVrefCtrl;
  RXVREFCTRLN0_0_MCIO_DDRIO_STRUCT  rxVrefCtrln1;
  BOOLEAN                           RegisterAccessed;
#ifdef LRDIMM_SUPPORT
  UINT8                             vrefValue[MAX_STROBE/2];
  UINT8                             vrefSelect[MAX_STROBE/2];
  UINT8                             *controlWordDataPtr;
  struct rankDevice                 (*rankStruct)[MAX_RANK_DIMM];
  UINT8                             updateNeeded = 0;
  struct ddrRank                    (*rankList)[MAX_RANK_DIMM];
#endif //LRDIMM
  UINT8                             StrobeN[MAX_NIBBLES_PER_BYTE];
  UINT8 MSVx4 = GetMaxStrobeValid(Host);
  UINT8 MSVx8 = GetMaxStrobeValid(Host) / 2;

  CountTrackingData (PTC_VREF_MOVE, 1);

  channelNvList = GetChannelNvList(Host, socket);
#ifdef LRDIMM_SUPPORT
  rankStruct  = GetRankStruct(Host, socket, ch, dimm);
  rankList = GetRankNvList(Host, socket, ch, dimm);
#endif //LRDIMM

  rxVrefCtrl.Data = 0;
  rxVrefCtrln1.Data = 0;

  //
  // Get the logical rank #
  //

  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits(Host, level, group, &minLimit, &maxLimit, &usDelay);

  if (bit == ALL_BITS && level == DdrLevel) {
    bitStart = 0;
    BitStop  = MAX_BITS_IN_BYTE;
  } else {
    bitStart = 0;
    BitStop  = 1;
    SwizzleBit = 0;
  }
  //
  // Clear all variables
  //
  for (indexBit = 0; indexBit < MAX_BITS_IN_BYTE; indexBit++) {
    curVal[indexBit]  = 0;
    piDelay[indexBit] = 0;
    tmp[indexBit]     = 0;
  }

  if (strobe >= MAX_STROBE && strobe != ALL_STROBES) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                   "The strobe value is out of range!\n");
    RC_FATAL_ERROR ((strobe < MAX_STROBE || strobe == ALL_STROBES), ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  if (strobe == ALL_STROBES) {
    strobe = 0;
    if (bit == ALL_BITS) {
      maxStrobe = MSVx8;
    } else {
      maxStrobe = MSVx4;
    }
  } else {
    maxStrobe = strobe + 1;
    if (bit == ALL_BITS) {
      MemCheckIndex (strobe, MSVx8);
    } else {
      MemCheckIndex (strobe, MSVx4);
    }
  }

  for (; strobe < maxStrobe; strobe++) {

    StrobeN[INDEX_NIBBLE0] = strobe % MSVx8;
    StrobeN[INDEX_NIBBLE1] = (strobe % MSVx8) + MSVx8;

    // HSD 5330942
    logicalBit    = bit % BITS_PER_NIBBLE; // Find bit per nibble
    targetStrobe  = strobe;
    if (bit != ALL_BITS) {
      if (bit >= BITS_PER_NIBBLE) { // If the bit is >= 4, then it's a bit in the upper nibble
        targetStrobe += MSVx8;
      }
      //
      // Get DQ swizzle bit
      //
      SwizzleBit = GetDqSwizzle (ch, targetStrobe, logicalBit, level, group);
      //
      // bit != ALL_BIT
      // Loop should only target the SwizzleBit
      //
      bitStart = SwizzleBit;
      BitStop  = SwizzleBit + 1;
    }

    if (mode & GSM_READ_CSR) {
#ifdef LRDIMM_SUPPORT
      if (level == LrbufLevel) {
        //TODO: RxVref value is shared by the ranks, and should be in a shared location for the dimm and not per rank, but for now, just use the rank 0 value
        curVal[SwizzleBit] = (*rankList)[0].lrbufRxVref[strobe];
      } else
#endif //LRDIMM
      {
        //
        // Read from PCI config space
        //
        rxVrefCtrl.Data   = ReadDdrioDataRegCh (Host, socket, ch, StrobeN[INDEX_NIBBLE0], RXVREFCTRLN0_0_MCIO_DDRIO_REG);
        rxVrefCtrln1.Data = ReadDdrioDataRegCh (Host, socket, ch, StrobeN[INDEX_NIBBLE1], RXVREFCTRLN0_0_MCIO_DDRIO_REG);
      }
    } else {
      //
      // Read from cache
      //
#ifdef LRDIMM_SUPPORT
      if (level == LrbufLevel) {
        //TODO: RxVref value is shared by the ranks, and should be in a shared location for the dimm and not per rank, but for now, just use the rank 0 value
        curVal[SwizzleBit] = (*rankStruct)[0].lrbufRxVrefCache[strobe];
      } else
#endif //LRDIMM
      {
        rxVrefCtrl.Data   = (*channelNvList)[ch].rxVrefCtrl[StrobeN[INDEX_NIBBLE0]];
        rxVrefCtrln1.Data = (*channelNvList)[ch].rxVrefCtrl[StrobeN[INDEX_NIBBLE1]];
     //   RcDebugPrint (SDBG_MIN,
     //   "rxVrefCtrl: = %2d, rxVrefCtrln1= %2d \n", rxVrefCtrl.Data, rxVrefCtrln1.Data);
      }
    }

    if (level == DdrLevel) {
      //
      // for curVal and piDelay there is no need to consider the Bit Swizzling
      // Swizzling will be considered only when doing the operation to calculate the final value
      // SwizzleBit variable will track which will be the array index to modify
      //
      curVal[0] = (UINT16)rxVrefCtrl.Bits.vrefperbit0;
      curVal[1] = (UINT16)rxVrefCtrl.Bits.vrefperbit1;
      curVal[2] = (UINT16)rxVrefCtrl.Bits.vrefperbit2;
      curVal[3] = (UINT16)rxVrefCtrl.Bits.vrefperbit3;
      curVal[4] = (UINT16)rxVrefCtrln1.Bits.vrefperbit0;
      curVal[5] = (UINT16)rxVrefCtrln1.Bits.vrefperbit1;
      curVal[6] = (UINT16)rxVrefCtrln1.Bits.vrefperbit2;
      curVal[7] = (UINT16)rxVrefCtrln1.Bits.vrefperbit3;

      piDelay[0]  = curVal[0];
      piDelay[1]  = curVal[1];
      piDelay[2]  = curVal[2];
      piDelay[3]  = curVal[3];
      piDelay[4]  = curVal[4];
      piDelay[5]  = curVal[5];
      piDelay[6]  = curVal[6];
      piDelay[7]  = curVal[7];
    } // DdrLevel

    //
    // Check if read only
    //
    if (mode & GSM_READ_ONLY) {
      //
      // Save the value read into the output parameter
      //
      *value = curVal[SwizzleBit];
    } else { //read only -q1

      //
      // Write
      //
      // Adjust the current CMD delay value by offset
      if (mode & GSM_WRITE_OFFSET) {
        for (indexBit = bitStart; indexBit < BitStop; indexBit++) {
          //
          // Add the offset to the current value
          //
          tmp[indexBit] = curVal[indexBit] + *value;

          //
          // Make sure we do not exeed the limits
          //
          if (tmp[indexBit] > minLimit) {
            piDelay[indexBit] = tmp[indexBit];
          } else {
            piDelay[indexBit] = minLimit;
          }

          // Ensure we do not exceed maximums
          if (piDelay[indexBit] > maxLimit) {
            piDelay[indexBit] = maxLimit;
          }
        }
      } else {
        //
        // Write absolute value
        //
        for (indexBit = bitStart; indexBit < BitStop; indexBit++) {
          piDelay[indexBit] = *value;
        }
      }

      rxVrefCtrl.Bits.vrefperbit0   = piDelay[0];
      rxVrefCtrl.Bits.vrefperbit1   = piDelay[1];
      rxVrefCtrl.Bits.vrefperbit2   = piDelay[2];
      rxVrefCtrl.Bits.vrefperbit3   = piDelay[3];
      rxVrefCtrln1.Bits.vrefperbit0 = piDelay[4];
      rxVrefCtrln1.Bits.vrefperbit1 = piDelay[5];
      rxVrefCtrln1.Bits.vrefperbit2 = piDelay[6];
      rxVrefCtrln1.Bits.vrefperbit3 = piDelay[7];

#ifdef LRDIMM_SUPPORT
      if (level == LrbufLevel) {
        //
        // Write it back if the current data does not equal the cache value or if in force write mode
        //
        //TODO: RxVref value is shared by the ranks, and should be in a shared location for the dimm and not per rank, but for now, just use the rank 0 value
        if ((piDelay[SwizzleBit] != (*rankList)[0].lrbufRxVref[strobe]) || (mode & GSM_FORCE_WRITE)) {
          (*rankList)[0].lrbufRxVref[strobe] = (UINT8) piDelay[SwizzleBit];
          updateNeeded = 1;
        }
      } else
#endif //LRDIMM
      {
        RegisterAccessed = FALSE;
        //
        // Write it back if the current data does not equal the cache value or if in force write mode
        //
        if ((rxVrefCtrl.Data != (*channelNvList)[ch].rxVrefCtrl[StrobeN[INDEX_NIBBLE0]]) || (mode & GSM_FORCE_WRITE)) {
          RegisterAccessed = TRUE;
          WriteDdrioDataRegCh (Host, socket, ch, StrobeN[INDEX_NIBBLE0], RXVREFCTRLN0_0_MCIO_DDRIO_REG, rxVrefCtrl.Data);
        }
        if ((rxVrefCtrln1.Data != (*channelNvList)[ch].rxVrefCtrl[StrobeN[INDEX_NIBBLE1]]) || (mode & GSM_FORCE_WRITE)) {
          RegisterAccessed = TRUE;
          WriteDdrioDataRegCh (Host, socket, ch, StrobeN[INDEX_NIBBLE1], RXVREFCTRLN0_0_MCIO_DDRIO_REG, rxVrefCtrln1.Data);
        }
        if (RegisterAccessed) {
          //
          // Wait for the new value to settle
          //
          FixedDelayMicroSecond (usDelay);
        }
      }

    } // if read only //read only -q1

    //
    // Save to cache
    //
    if (mode & GSM_UPDATE_CACHE) {
#ifdef LRDIMM_SUPPORT
      if (level == LrbufLevel) {
        //TODO: RxVref value is shared by the ranks, and should be in a shared location for the dimm and not per rank, but for now, just use the rank 0 value
        (*rankStruct)[0].lrbufRxVrefCache[strobe] = (*rankList)[0].lrbufRxVref[strobe];
      } else
#endif //LRDIMM
      {
        (*channelNvList)[ch].rxVrefCtrl[StrobeN[INDEX_NIBBLE0]] = rxVrefCtrl.Data;
        (*channelNvList)[ch].rxVrefCtrl[StrobeN[INDEX_NIBBLE1]] = rxVrefCtrln1.Data;
      }
    }
  } // strobe loop

#ifdef LRDIMM_SUPPORT
  if ((level == LrbufLevel) && !(mode & GSM_READ_ONLY) && ((updateNeeded) || (mode & GSM_FORCE_WRITE))) {
    //
    //Note: because Rx vref is shared between upper and lower nibbles on the backside,
    // we have to either pick 1 or do some average.  For now, just picking the lower nibble
    // TODO: check this
    //
    //TODO: RxVref value is shared by the ranks, and should be in a shared location for the dimm and not per rank, but for now, just use the rank 0 value
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT, "DB RxVref to DRAM = %d\n", (*rankList)[rank].lrbufRxVref[0]);
    for (strobe = 0; strobe < MSVx8; strobe++) {
      if ((*rankList)[0].lrbufRxVref[strobe] < DB_DRAM_VREF_RANGE2_OFFSET) {
        //Select range 2
        vrefSelect[strobe] = 0x5;
        vrefValue[strobe] = (*rankList)[0].lrbufRxVref[strobe];
      }
      else {
        //Select range 1 and adjust encoding
        vrefSelect[strobe] = 0x1;
        vrefValue[strobe] = (*rankList)[0].lrbufRxVref[strobe] - DB_DRAM_VREF_RANGE2_OFFSET;
      }
    }
    controlWordDataPtr = &vrefSelect[0];
    WriteLrbufPBA(Host, socket, ch, dimm, 0, controlWordDataPtr, LRDIMM_F6, LRDIMM_BC4x, ALL_DATABUFFERS);
    controlWordDataPtr = &vrefValue[0];
    WriteLrbufPBA(Host, socket, ch, dimm, 0, controlWordDataPtr, LRDIMM_F5, LRDIMM_BC6x, ALL_DATABUFFERS);

    //
    // Wait for the new value to settle
    //
    FixedDelayMicroSecond (usDelay);
  }
#endif //LRDIMM

  return MRC_STATUS_SUCCESS;
} // GetSetRxVref


/**

  Get Tx Vref

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param value   - Data to program

  @retval IO delay

**/
STATIC
MRC_STATUS
GetSetTxVref (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value
  )
{
  UINT8               maxStrobe;
  UINT16              curVal = 0;
  INT16               newVref;
  INT16               tmp;
  UINT16              maxLimit = 0;
  UINT16              minLimit = 0;
  UINT16              usDelay = 0;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
#ifdef LRDIMM_SUPPORT
  struct rankDevice   (*rankStruct)[MAX_RANK_DIMM];
  UINT8               tmpStrobe;
#endif //LRDIMM
  UINT8 MSVx4 = GetMaxStrobeValid(Host);
  SYS_SETUP           *Setup;

  Setup = GetSysSetupPointer ();

  channelNvList = GetChannelNvList(Host, socket);
  rankList = GetRankNvList(Host, socket, ch, dimm);

#ifdef LRDIMM_SUPPORT
  rankStruct  = GetRankStruct(Host, socket, ch, dimm);
#endif //LRDIMM

  CountTrackingData (PTC_VREF_MOVE, 1);

#ifdef LRDIMM_SUPPORT
  if (IsLrdimmPresent (socket, ch, dimm) && (rank > 0) && (level == DdrLevel)){
    rank=0;
  }
#endif //LRDIMM_SUPPORT

  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits(Host, level, group, &minLimit, &maxLimit, &usDelay);

  if (strobe >= MAX_STROBE && strobe != ALL_STROBES) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                   "The strobe value is out of range!\n");
    RC_FATAL_ERROR ((strobe < MAX_STROBE || strobe == ALL_STROBES), ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  if ((Setup->mem.optionsExt & PDA_EN) &&
      ((Host->nvram.mem.socket[socket].TrainingStepDone.TxVref == 1) ||
       (level == LrbufLevel)))
  {

    if (strobe == ALL_STROBES) {
      strobe    = 0;
      maxStrobe = GetMaxStrobe(Host, socket, ch, dimm, group, level);
    } else {
      maxStrobe = strobe + 1;
      if (maxStrobe > MSVx4) {
        return MRC_STATUS_STROBE_NOT_PRESENT;
      }
    }

    for (; strobe < maxStrobe; strobe++) {
      if (IsStrobeNotValid(Host, strobe)) {
        continue;
      }

      if (mode & GSM_READ_CSR) {
#ifdef LRDIMM_SUPPORT
        if (level == LrbufLevel) {
          curVal = (UINT8) (*rankList)[rank].lrbufTxVref[strobe];
        } else
#endif  //LRDIMM
        {
          curVal = (*channelNvList)[ch].txVref[(*rankList)[rank].rankIndex][strobe];
        }
      } else {
#ifdef LRDIMM_SUPPORT
        if (level == LrbufLevel) {
          curVal = (UINT8) (*rankStruct)[rank].lrbufTxVrefCache[strobe];
        } else
#endif  //LRDIMM
        {
          curVal = (*channelNvList)[ch].txVrefCache[(*rankList)[rank].rankIndex][strobe];
        }
      } // if cached
#ifdef DDR5_SUPPORT
      curVal = ConvertPhysicalToLinear (curVal);
#endif // #ifdef DDR5_SUPPORT
      //
      // Check if read only
      //
      if (mode & GSM_READ_ONLY) {
        //
        // Save the value read into the output parameter
        //
        *value = curVal;
      } else {
        //
        // Write
        //
        if (mode & GSM_WRITE_OFFSET) {
          //
          // Add the offset to the current value
          //
          tmp = curVal + *value;

        } else {
          //
          // Write absolute value
          //
          tmp = *value;
        }

        //
        // Make sure we do not exeed the limits
        //
        if (tmp > minLimit) {
          newVref = tmp;
        } else {
          newVref = minLimit;
        }
        // Ensure we do not exceed maximums
        if (newVref > maxLimit) {
          newVref = maxLimit;
        }

#ifdef DDR5_SUPPORT
        newVref = ConvertLinearToPhysical (newVref);
#endif // #ifdef DDR5_SUPPORT
#ifdef LRDIMM_SUPPORT
        if (level == LrbufLevel) {
          //
          // Write it back if the current data does not equal the current value or if in force write mode
          //
          if ((newVref != (UINT8) (*rankList)[rank].lrbufTxVref[strobe]) || (mode & GSM_FORCE_WRITE)) {

            (*rankList)[rank].lrbufTxVref[strobe] = (UINT8)newVref;

            //
            // Write the new vref
            //
            MemWriteDimmVref(Host, socket, ch, dimm, rank, strobe, level, (UINT8)newVref);

          }
        } else
#endif  //LRDIMM
        {
          //
          // Write it back if the current data does not equal the current value or if in force write mode
          //
          if ((newVref != (*channelNvList)[ch].txVref[(*rankList)[rank].rankIndex][strobe]) || (mode & GSM_FORCE_WRITE)) {

            (*channelNvList)[ch].txVref[(*rankList)[rank].rankIndex][strobe] = (UINT8)newVref;

            //
            // Write the new vref
            //
            MemWriteDimmVref(Host, socket, ch, dimm, rank, strobe, level, (UINT8)newVref);
          }
        }
      } // if read only

      //
      // Save to cache
      //
      if (mode & GSM_UPDATE_CACHE) {
#ifdef LRDIMM_SUPPORT
        if (level == LrbufLevel) {
          (*rankStruct)[rank].lrbufTxVrefCache[strobe] = (*rankList)[rank].lrbufTxVref[strobe];
        } else
#endif //LRDIMM
        {
          (*channelNvList)[ch].txVrefCache[(*rankList)[rank].rankIndex][strobe] = (*channelNvList)[ch].txVref[(*rankList)[rank].rankIndex][strobe];
        }
      }

      // Wait for Wr Vref circuit to settle
      FixedDelayMicroSecond (usDelay);
    } // strobe loop

    //
    // Make sure Vref settles
    //
    FixedDelayMicroSecond (1);

  } else {
    if (mode & GSM_READ_CSR) {
#ifdef LRDIMM_SUPPORT
      if (level == LrbufLevel) {
        curVal = (UINT8) (*rankList)[rank].lrbufTxVref[0];
      } else
#endif //LRDIMM
      {
        curVal = (*channelNvList)[ch].txVref[(*rankList)[rank].rankIndex][0];
      }
    } else {
#ifdef LRDIMM_SUPPORT
      if (level == LrbufLevel) {
        curVal = (UINT8) (*rankStruct)[rank].lrbufTxVrefCache[0];
      } else
#endif //LRDIMM
      {
        curVal = (*channelNvList)[ch].txVrefCache[(*rankList)[rank].rankIndex][0];
      }
    } // if cached
#ifdef DDR5_SUPPORT
    curVal = ConvertPhysicalToLinear (curVal);
#endif // #ifdef DDR5_SUPPORT
    //
    // Check if read only
    //
    if (mode & GSM_READ_ONLY) {
      //
      // Save the value read into the output parameter
      //
      *value = curVal;
    } else {
      //
      // Write
      //
      if (mode & GSM_WRITE_OFFSET) {
        //
        // Add the offset to the current value
        //
        tmp = curVal + *value;

      } else {
        //
        // Write absolute value
        //
        tmp = *value;
      }

      //
      // Make sure we do not exeed the limits
      //
      if (tmp > minLimit) {
        newVref = tmp;
      } else {
        newVref = minLimit;
      }
      // Ensure we do not exceed maximums
      if (newVref > maxLimit) {
        newVref = maxLimit;
      }

#ifdef DDR5_SUPPORT
      newVref = ConvertLinearToPhysical (newVref);
#endif // #ifdef DDR5_SUPPORT

#ifdef LRDIMM_SUPPORT
      if (level == LrbufLevel) {
        //
        // Write it back if the current data does not equal the current value or if in force write mode
        //
        if ((newVref != (UINT8) (*rankList)[rank].lrbufTxVref[0]) || (mode & GSM_FORCE_WRITE)) {

          for (tmpStrobe = 0; tmpStrobe < MSVx4; tmpStrobe++) {
            (*rankList)[rank].lrbufTxVref[tmpStrobe] = (UINT8)newVref;
          } // strobe loop
          //
          // Write the new vref
          //
          MemWriteDimmVref(Host, socket, ch, dimm, rank, 0, level, (UINT8)newVref);
        }
      } else
#endif //LRDIMM
      {
        //
        // Write it back if the current data does not equal the current value or if in force write mode
        //
        if ((newVref != (*channelNvList)[ch].txVref[(*rankList)[rank].rankIndex][0]) || (mode & GSM_FORCE_WRITE)) {

          for (strobe = 0; strobe < MSVx4; strobe++) {
            (*channelNvList)[ch].txVref[(*rankList)[rank].rankIndex][strobe] = (UINT8)newVref;
          } // strobe loop

          //
          // Write the new vref
          //
          MemWriteDimmVref(Host, socket, ch, dimm, rank, 0, level, (UINT8)newVref);

        }
      }
    } // if read only

    //
    // Save to cache
    //
    if (mode & GSM_UPDATE_CACHE) {
      for (strobe = 0; strobe < MSVx4; strobe++) {
#ifdef LRDIMM_SUPPORT
        if (level == LrbufLevel) {
          (*rankStruct)[rank].lrbufTxVrefCache[strobe] = (*rankList)[rank].lrbufTxVref[strobe];
        } else
#endif //LRDIMM
        {
          (*channelNvList)[ch].txVrefCache[(*rankList)[rank].rankIndex][strobe] = (*channelNvList)[ch].txVref[(*rankList)[rank].rankIndex][strobe];
        }
      } // strobe loop
    }

    // Wait for Wr Vref circuit to settle
    FixedDelayMicroSecond (usDelay);
  }

  return MRC_STATUS_SUCCESS;
} // GetSetTxVref

/**

  Get Imode

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param strobe  - Strobe number (0-based)
  @param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param value   - Data to program

  @retval IO delay

**/
STATIC
MRC_STATUS
GetSetImode (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     strobe,
  UINT8     mode,
  INT16     *value
  )
{
  UINT8                               maxStrobe;
  UINT16                              curVal = 0;
  UINT16                              piDelay;
  INT16                               tmp;
  UINT16                              maxLimit = 15;
  UINT16                              minLimit = 0;
  struct channelNvram                 (*channelNvList)[MAX_CH];
  DATACONTROL2N0_0_MCIO_DDRIO_STRUCT  DataControl2N0;
  DATACONTROL2N0_0_MCIO_DDRIO_STRUCT  DataControl2N1;
  UINT8                               MSVx8 = GetMaxStrobeValid(Host) / 2;

  channelNvList = GetChannelNvList(Host, socket);

  if (strobe >= MAX_STROBE && strobe != ALL_STROBES) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, strobe, NO_BIT,
                   "The strobe value is out of range!\n");
    RC_FATAL_ERROR ((strobe < MAX_STROBE || strobe == ALL_STROBES), ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  if (strobe == ALL_STROBES) {
    strobe    = 0;
    maxStrobe = MSVx8;
  } else {
    maxStrobe = strobe + 1;
  }

  MemCheckIndex (strobe, MSVx8);

  for (; strobe < maxStrobe; strobe++) {
    if (IsStrobeNotValid(Host, strobe)) {
      continue;
    }

    if(mode & GSM_READ_CSR) {
      //
      // Read from PCI config space
      //
      DataControl2N0.Data = ReadDdrioDataReg (Host, socket, ch, 0, strobe,         DATACONTROL2N0_0_MCIO_DDRIO_REG);
      DataControl2N1.Data = ReadDdrioDataReg (Host, socket, ch, 0, strobe + MSVx8, DATACONTROL2N0_0_MCIO_DDRIO_REG);
    } else {
      //
      // Read from cache
      //
      DataControl2N0.Data = (*channelNvList)[ch].dataControl2[strobe];
      DataControl2N1.Data = (*channelNvList)[ch].dataControl2[strobe + MSVx8];
    }

    curVal = (UINT16)DataControl2N0.Bits.imodeeqcode;
    //
    // Check if read only
    //
    if (mode & GSM_READ_ONLY) {
      //
      // Save the value read into the output parameter
      //
      *value = curVal;
    } else {
      //
      // Write
      //
      // Adjust the current CMD delay value by offset
      if (mode & GSM_WRITE_OFFSET) {
        //
        // Add the offset to the current value
        //
        tmp = curVal + *value;

        //
        // Make sure we do not exeed the limits
        //
        if (tmp > minLimit) {
          piDelay = tmp;
        } else {
          piDelay = minLimit;
        }
        // Ensure we do not exceed maximums
        if (piDelay > maxLimit) {
          piDelay = maxLimit;
        }
      } else {
        //
        // Write absolute value
        //
        piDelay = *value;
      }

      DataControl2N0.Bits.imodeeqcode = piDelay;
      DataControl2N1.Bits.imodeeqcode = piDelay;

      if (piDelay != 0) {
        //
        // Enable Imode registers per byte
        //
        DataControl2N0.Bits.imodeenable = 1;
        DataControl2N1.Bits.imodeenable = 1;
      } else {
        DataControl2N0.Bits.imodeenable = 0;
        DataControl2N1.Bits.imodeenable = 0;
      }

      //
      // Write it back if the current data does not equal the cache value or if in force write mode
      //
      if ((DataControl2N0.Data != (*channelNvList)[ch].dataControl2[strobe]) || (mode & GSM_FORCE_WRITE)) {
        WriteDdrioDataReg (Host, socket, ch, 0, strobe, DATACONTROL2N0_0_MCIO_DDRIO_REG, DataControl2N0.Data);
      }
      if ((DataControl2N1.Data != (*channelNvList)[ch].dataControl2[strobe + MSVx8]) || (mode & GSM_FORCE_WRITE)) {
        WriteDdrioDataReg (Host, socket, ch, 0, strobe + MSVx8, DATACONTROL2N0_0_MCIO_DDRIO_REG, DataControl2N1.Data);
      }
    } // if read only

    //
    // Save to cache
    //
    if (mode & GSM_UPDATE_CACHE) {
      (*channelNvList)[ch].dataControl2[strobe]         = DataControl2N0.Data;
      (*channelNvList)[ch].dataControl2[strobe + MSVx8] = DataControl2N1.Data;
    }
  } // strobe loop

  return MRC_STATUS_SUCCESS;
} // GetSetImode

/**

  Get/Set Tx Rise Fall Slew Rate

  @param[in]     Host    - Pointer to sysHost
  @param[in]     Socket  - Socket number
  @param[in]     Ch      - Channel number (0-based)
  @param[in]     Dimm    - DIMM number (0-based)
  @param[in]     Rank    - Rank number (0-based)
  @param[in]     Strobe  - Strobe number (0-based)
  @param[in]     Bit     - Bit number
  @param[in]     Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]     Group   - CSM_GT - Parameter to program
  @param[in]     Mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param[in,out] Value   - Data to program

  @retval MRC_STATUS_STROBE_NOT_PRESENT if Strobe is out of range
  @retval MRC_STATUS_SUCCESS otherwise

**/
MRC_STATUS
GetSetTxRiseFallSlewRate (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      UINT8     Strobe,
  IN      UINT8     Bit,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  )
{
  UINT8                             MaxStrobe;
  UINT16                            CurVal = 0;
  UINT16                            PiDelay;
  INT16                             Tmp;
  UINT16                            MaxLimit;
  UINT16                            MinLimit;
  UINT16                            UsDelay;
  BOOLEAN                           CompUpdateNeeded;
  struct channelNvram               (*ChannelNvList)[MAX_CH];
  DATAOFFSETCOMP_MCIO_DDRIO_STRUCT  DataOffsetCompN0;
  DATAOFFSETCOMP_MCIO_DDRIO_STRUCT  DataOffsetCompN1;
  UINT8                             MSVx8 = GetMaxStrobeValid (Host) / 2;

  ChannelNvList = GetChannelNvList (Host, Socket);
  CompUpdateNeeded = FALSE;

  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits (Host, Level, Group, &MinLimit, &MaxLimit, &UsDelay);

  if (Strobe >= MAX_STROBE && Strobe != ALL_STROBES) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
                   "The Strobe value is out of range!\n");
    RC_FATAL_ERROR ((Strobe < MAX_STROBE || Strobe == ALL_STROBES), ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  //verify if there is one per Strobe
  if (Strobe == ALL_STROBES) {
    Strobe    = 0;
    MaxStrobe = MSVx8;
  } else {
    MaxStrobe = Strobe + 1;
  }

  //
  // Array bounds check
  //
  MemCheckIndex (Strobe, MSVx8);

  for (; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValid (Host, Strobe)) {
      continue;
    }

    if (Mode & GSM_READ_CSR) {
      //
      // Read from PCI config space
      //
      DataOffsetCompN0.Data = ReadDdrioDataRegCh (Host, Socket, Ch, Strobe,         DATAOFFSETCOMP_MCIO_DDRIO_REG);
      DataOffsetCompN1.Data = ReadDdrioDataRegCh (Host, Socket, Ch, Strobe + MSVx8, DATAOFFSETCOMP_MCIO_DDRIO_REG);
    } else {
      //
      // Read from cache
      //
      DataOffsetCompN0.Data = (*ChannelNvList)[Ch].dataOffsetComp[Strobe];
      DataOffsetCompN1.Data = (*ChannelNvList)[Ch].dataOffsetComp[Strobe + MSVx8];
    } // DDR_LEVEL

    switch (Group) {
      case TxRiseFallSlewRate:
        CurVal = (UINT16)DataOffsetCompN0.Bits.dqslewratecompoffset;
        break;
      case TxFallSlewRate:
        CurVal = (UINT16)DataOffsetCompN0.Bits.dqslewratedownoffset;
        break;
      default:
        break;
    }

    //
    // Check if read only
    //
    if (Mode & GSM_READ_ONLY) {
      //
      // Save the value read into the output parameter
      //
      *Value = CurVal;
    } else {
      //
      // Write
      //
      // Adjust the current CMD delay value by offset
      if (Mode & GSM_WRITE_OFFSET) {
        //
        // Add the offset to the current value
        //
        Tmp = CurVal + *Value;
      } else {
        //
        // Write absolute value
        //
        Tmp = *Value;
      }
      //
      // Make sure we do not exeed the limits
      //
      if (Tmp < MinLimit) {
        Tmp = MinLimit;
      }
      if (Tmp > MaxLimit) {
        Tmp = MaxLimit;
      }
      //
      // Write value
      //
      PiDelay = Tmp;

      switch (Group) {
        case TxRiseFallSlewRate:
          DataOffsetCompN0.Bits.dqslewratecompoffset = PiDelay;
          DataOffsetCompN1.Bits.dqslewratecompoffset = PiDelay;
          break;
        case TxFallSlewRate:
          DataOffsetCompN0.Bits.dqslewratedownoffset = PiDelay;
          DataOffsetCompN1.Bits.dqslewratedownoffset = PiDelay;
          break;
        default:
          break;
      }

      //
      // Write it back if the current data does not equal the cache value or if in force write mode
      //
      if ((DataOffsetCompN0.Data != (*ChannelNvList)[Ch].dataOffsetComp[Strobe]) || (Mode & GSM_FORCE_WRITE)) {
        WriteDdrioDataRegCh (Host, Socket, Ch, Strobe,         DATAOFFSETCOMP_MCIO_DDRIO_REG, DataOffsetCompN0.Data);
        CompUpdateNeeded = TRUE;
      }
      if ((DataOffsetCompN1.Data != (*ChannelNvList)[Ch].dataOffsetComp[Strobe + MSVx8]) || (Mode & GSM_FORCE_WRITE)) {
        WriteDdrioDataRegCh (Host, Socket, Ch, Strobe + MSVx8, DATAOFFSETCOMP_MCIO_DDRIO_REG, DataOffsetCompN1.Data);
        CompUpdateNeeded = TRUE;
      }
    } // if read only

    //
    // Save to cache
    //
    if (Mode & GSM_UPDATE_CACHE) {
      (*ChannelNvList)[Ch].dataOffsetComp[Strobe]         = DataOffsetCompN0.Data;
      (*ChannelNvList)[Ch].dataOffsetComp[Strobe + MSVx8] = DataOffsetCompN1.Data;
    }
  } // Strobe

  if (CompUpdateNeeded) {
    DoCompUpdate (Host, Socket, GetMCID (Host, Socket, Ch));
  }

  return MRC_STATUS_SUCCESS;
} // GetSetTxRiseFallSlewRate


STATIC
MRC_STATUS
GetSetCTLE (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     strobe,
  UINT8     bit,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value
  )
/*++

Routine Description:

  Get CTLE

Arguments:

  Host    - Pointer to sysHost
  socket  - Socket number
  ch      - Channel number (0-based)
  dimm    - DIMM number (0-based)
  strobe  - Strobe number (0-based)
  bit     - Bit number
  level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  group   - CSM_GT - Parameter to program
  mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  value   - Data to program

Returns:

  IO delay

--*/
{
  UINT8                             maxStrobe;
  UINT16                            curVal = 0;
  UINT16                            piDelay;
  INT16                             tmp;
  UINT16                            maxLimit = 3;
  UINT16                            minLimit = 0;
  struct channelNvram               (*channelNvList)[MAX_CH];
  DATACONTROL3N0_0_MCIO_DDRIO_STRUCT dataControl3;
  DATACONTROL3N0_0_MCIO_DDRIO_STRUCT dataControl3n1;
  UINT8 MSVx8 = GetMaxStrobeValid(Host) / 2;

  channelNvList = GetChannelNvList(Host, socket);

  if (strobe == ALL_STROBES) {
    strobe    = 0;
    maxStrobe = MSVx8;
  } else {
    maxStrobe = strobe + 1;
    if (maxStrobe > MSVx8) {
      return MRC_STATUS_STROBE_NOT_PRESENT;
    }
  }

  //
  // Array bounds check
  //
  MemCheckIndex (strobe, MSVx8);

  for (; strobe < maxStrobe; strobe++) {
    if (IsStrobeNotValid(Host, strobe)) {
      continue;
    }

    if(mode & GSM_READ_CSR) {
      //
      // Read from PCI config space
      //
      dataControl3.Data = ReadDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL3N0_0_MCIO_DDRIO_REG);
      dataControl3n1.Data = ReadDdrioDataRegCh (Host, socket, ch, (strobe + MSVx8), DATACONTROL3N0_0_MCIO_DDRIO_REG);
    } else {
      //
      // Read from cache
      //
      dataControl3.Data = (*channelNvList)[ch].dataControl3[strobe];
      dataControl3n1.Data = (*channelNvList)[ch].dataControl3[(strobe + MSVx8)];
    } // DDR_LEVEL

    switch (group) {

    case RxCtleC:
      curVal = (UINT16)dataControl3.Bits.ddrcrctlecapen;
      break;

    case RxCtleR:
      curVal = (UINT16)dataControl3.Bits.ddrcrctleresen;
      break;

    default:
      break;
    };

    //
    // Check if read only
    //
    if (mode & GSM_READ_ONLY) {
      //
      // Save the value read into the output parameter
      //
      *value = curVal;
    } else {
      //
      // Write
      //
      // Adjust the current CMD delay value by offset
      if (mode & GSM_WRITE_OFFSET) {
        //
        // Add the offset to the current value
        //
        tmp = curVal + *value;

        //
        // Make sure we do not exeed the limits
        //
        if (tmp > minLimit) {
          piDelay = tmp;
        } else {
          piDelay = minLimit;
        }
        // Ensure we do not exceed maximums
        if (piDelay > maxLimit) {
          piDelay = maxLimit;
        }
      } else {
        //
        // Write absolute value
        //
        piDelay = *value;
      }

      switch (group) {

      case RxCtleC:
        dataControl3.Bits.ddrcrctlecapen = piDelay;
        dataControl3n1.Bits.ddrcrctlecapen = piDelay;
        break;

      case RxCtleR:
        dataControl3.Bits.ddrcrctleresen = piDelay;
        dataControl3n1.Bits.ddrcrctleresen = piDelay;
        break;

      default:
        break;
      };

      {
        //
        // Write it back if the current data does not equal the cache value or if in force write mode
        //
        if ((dataControl3.Data != (*channelNvList)[ch].dataControl3[strobe]) || (mode & GSM_FORCE_WRITE)) {
          WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL3N0_0_MCIO_DDRIO_REG, dataControl3.Data);
          WriteDdrioDataRegCh (Host, socket, ch, (strobe + MSVx8), DATACONTROL3N0_0_MCIO_DDRIO_REG, dataControl3n1.Data);
        }
      }
    } // if read only

    //
    // Save to cache
    //
    if (mode & GSM_UPDATE_CACHE) {
      (*channelNvList)[ch].dataControl3[strobe] = dataControl3.Data;
      (*channelNvList)[ch].dataControl3[(strobe + MSVx8)] = dataControl3n1.Data;
    }
  } // strobe loop

  return MRC_STATUS_SUCCESS;
} // GetSetCTLE

/**
  Converts DDRCRCOMPCTL0.DqOdtPupVref value to ODT in ohms (or vice versa)

  @param[in] Value            Value to convert.
  @param[in] Code             Type of operation CODE_ODT or DECODE_ODT.

  @return                     Encoded or decoded ODT value.
**/
UINT16
CodeDeCodeRxOdt (
  IN  UINT16 Value,
  IN  UINT8  Code
  )
{
  UINT8               Ch;
  UINT8               Socket;
  UINT16              ConvertOdt         = 0;
  UINT16              TargetDq           = 0;
  UINT16              TotalSegments      = ODT_SEGMENTS_EN_2;
  UINT16              ExternalResistor   = 100;
  UINT16              VrefSteps          = 256;
  UINT16              ScalingFactor      = 10000; // Because GroundOffset is 0.014, ScalingFactor must be >= 1000  (also, mind the magnitude of ScalingFactor to avoid integer overflow)
  UINT16              ScaledGroundOffset = 140;   // 140 is (0.014 * ScalingFactor) and assumes ScalingFactor == 10,000  (if ScalingFactor changes then ScaledGroundOffset must change accordingly)
#ifdef DDR5_SUPPORT // DDR5 voltage
  UINT16              ScaledVcc          = 11000; // 11,000 is (1.1 * ScalingFactor) and assumes ScalingFactor == 10,000  (if ScalingFactor changes then ScaledVcc must change accordingly)
#else // DDR4 voltage
  UINT16              ScaledVcc          = 12000; // 12,000 is (1.2 * ScalingFactor) and assumes ScalingFactor == 10,000  (if ScalingFactor changes then ScaledVcc must change accordingly)
#endif // DDR voltage
  UINT16              SegmentsScaling    = 10;
  UINT16              RoundUp            = 0;
  UINT32              Numerator          = 0;
  UINT32              Denominator        = 0;
  UINT32              ScaledVrefCode     = 0;
  UINT32              PartialResult      = 0;
  PSYSHOST            Host;
  struct channelNvram (*ChannelNvList)[MAX_CH];

  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;
  ChannelNvList = GetChannelNvList (Host, Socket);
  if (FindFirstChannel (Socket, &Ch)) {
    // Set the total segments from the cached ODT segments enabled
    if ((*ChannelNvList)[Ch].OdtSegmentEnable[0] != 0) {
      TotalSegments = (*ChannelNvList)[Ch].OdtSegmentEnable[0];
    }
  }

  if (CODE_ODT == Code) {
    // get DqOdtPupVref value and convert it to ODT value
    ScaledVrefCode = ScalingFactor * Value;
    PartialResult  = (UINT32)(ScaledVrefCode / VrefSteps);
    Numerator      = (UINT32)(((PartialResult * (ScaledVcc - (2 * ScaledGroundOffset))) / ScalingFactor) + ScaledGroundOffset);
    Denominator    = (UINT32)(2 * ((ScalingFactor * Numerator) / ScaledVcc));
    PartialResult  = (UINT32)((ScalingFactor * ExternalResistor) / Denominator);
    TargetDq       = (UINT16)((PartialResult - ExternalResistor));
    RoundUp        = (UINT16)(SegmentsScaling*(TargetDq % TotalSegments) / ((SegmentsScaling*TotalSegments) / 2)); // RoundUp compensates for integer division truncation  (we don't use ScalingFactor here because 2/2 == 1)
    ConvertOdt     = (UINT16)((TargetDq / TotalSegments) + RoundUp);
  } else {
    // get ODT value and convert it to DqOdtPupVref value
    TargetDq       = TotalSegments * Value; // Target ohm per segment is (3 * X)
    Numerator      = (UINT32)(((ScaledVcc * ExternalResistor) / (2 * (TargetDq + ExternalResistor))) - ScaledGroundOffset);
    Denominator    = (UINT32)(ScaledVcc - (2 * ScaledGroundOffset));
    ScaledVrefCode = (UINT32)(VrefSteps * ((ScalingFactor * Numerator) / Denominator));
    RoundUp        = (UINT16)((ScaledVrefCode % ScalingFactor ) / (ScalingFactor / 2)); // Modulo idiom, to compensate for integer division "round down" (truncation)
    ConvertOdt     = (UINT16)((ScaledVrefCode / ScalingFactor) + RoundUp);
  } // if Code

  return ConvertOdt;
} // CodeDeCodeRxOdt

/**
  Converts DDRCRCOMPCTL0.DqDrvPupVref value to MC RON in ohms (or vice versa)

  @param[in] Value            Value to convert.
  @param[in] Code             Type of operation CODE_ODT or DECODE_ODT.

  @return                     Encoded or decoded MC RON value.
**/
UINT16
CodeDeCodeTxRon (
  IN  UINT16 Value,
  IN  UINT8  Code
  )
{
  UINT16 ConvertRon         = 0;
  UINT16 TargetDq           = 0;
  UINT16 TotalSegments      = 3;
  UINT16 ExternalResistor   = 90;
  UINT16 VrefSteps          = 256;
  UINT16 ScalingFactor      = 10000; // Because GroundOffset is 0.014, ScalingFactor must be >= 1000  (also, mind the magnitude of ScalingFactor to avoid integer overflow)
  UINT16 ScaledGroundOffset = 140;   // 140 is (0.014 * ScalingFactor) and assumes ScalingFactor == 10,000  (if ScalingFactor changes then ScaledGroundOffset must change accordingly)
#ifdef DDR5_SUPPORT // DDR5 voltage
  UINT16 ScaledVcc          = 11000; // 11,000 is (1.1 * ScalingFactor) and assumes ScalingFactor == 10,000  (if ScalingFactor changes then ScaledVcc must change accordingly)
#else // DDR4 voltage
  UINT16 ScaledVcc          = 12000; // 12,000 is (1.2 * ScalingFactor) and assumes ScalingFactor == 10,000  (if ScalingFactor changes then ScaledVcc must change accordingly)
#endif // DDR voltage
  UINT16 RoundUp            = 0;
  UINT32 Numerator          = 0;
  UINT32 Denominator        = 0;
  UINT32 ScaledVrefCode     = 0;
  UINT32 PartialResult      = 0;

  if (CODE_ODT == Code) {
    // get DqDrvPupVref value and convert it to TxRon value
    ScaledVrefCode = ScalingFactor * Value;
    PartialResult = (UINT32)(ScaledVrefCode / VrefSteps);
    Numerator = (UINT32)(((PartialResult * (ScaledVcc - (2 * ScaledGroundOffset))) / ScalingFactor) + ScaledGroundOffset);
    Denominator = (UINT32)(2 * ((ScalingFactor * Numerator) / ScaledVcc));
    PartialResult = (UINT32)((ScalingFactor * ExternalResistor) / Denominator);
    TargetDq = (UINT16)((PartialResult - ExternalResistor));
    RoundUp = (UINT16)((ScalingFactor * (TargetDq % TotalSegments)) / ((ScalingFactor * TotalSegments) / 2)); // RoundUp compensates for integer division truncation  (further, we use ScalingFactor here because 3/2 truncates to 1)
    ConvertRon = (UINT16)((TargetDq / TotalSegments) + RoundUp);
  } else {
    // get TxRon value and convert it to DqDrvPupVref value
    TargetDq = TotalSegments * Value; // Target ohm per segment is (3 * X)
    Numerator = (UINT32)(((ScaledVcc * ExternalResistor) / (2 * (TargetDq + ExternalResistor))) - ScaledGroundOffset);
    Denominator = (UINT32)(ScaledVcc - (2 * ScaledGroundOffset));
    ScaledVrefCode = (UINT32)(VrefSteps * ((ScalingFactor * Numerator) / Denominator));
    RoundUp = (UINT16)((ScaledVrefCode % ScalingFactor ) / (ScalingFactor / 2)); // Modulo idiom, to compensate for integer division "round down" (truncation)
    ConvertRon  = (UINT16)((ScaledVrefCode / ScalingFactor) + RoundUp);
  } // if Code

  return ConvertRon;
} // CodeDeCodeTxRon


STATIC
MRC_STATUS
GetSetCPUODT (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value
  )
/*++

Routine Description:

  Get ODT

Arguments:

  Host    - Pointer to sysHost
  socket  - Socket number
  ch      - Channel number (0-based)
  dimm    - DIMM number (0-based)
  rank    - Rank number (0-based)
  strobe  - Strobe number (0-based)
  bit     - Bit number
  level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  group   - CSM_GT - Parameter to program
  mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  value   - Data to program

Returns:

  IO delay

--*/
{
  UINT8                                   Strobes;
  UINT8                                   MSVx4;
  UINT16                                  curVal = 0;
  UINT16                                  tempVal = 0;
  UINT16                                  piDelay;
  INT16                                   tmp;
  UINT16                                  maxLimit = 3;
  UINT16                                  minLimit = 0;
  UINT8                                   imc;
  DDRCRCOMPCTL0_MCIO_DDRIOEXT_STRUCT      ddrCRCompCtl0;
  DDRCRCOMPCTL3_MCIO_DDRIOEXT_STRUCT      ddrCRCompCtl3;
  DDRCRDATACOMP1_MCIO_DDRIOEXT_STRUCT     DdrCrDataComp1;
  DDRCRCTLCOMP_CMDN_MCIO_DDRIOEXT_STRUCT  DdrCrCtlComp;
  DATACONTROL3N0_0_MCIO_DDRIO_STRUCT      DataControl3;
  struct channelNvram                     (*ChannelNvList)[MAX_CH];
  struct imcNvram                         (*imcNvList)[MAX_IMC];

  imcNvList     = GetImcNvList(Host, socket);
  imc = GetMCID(Host, socket, ch);

  ChannelNvList = GetChannelNvList (Host, socket);
  MSVx4 = GetMaxStrobeValid (Host);

  //if (ch & 1) return SUCCESS;

  //
  // Get minimum and maximum value
  //
  // GetDataGroupLimits(Host, level, group, &minLimit, &maxLimit, &usDelay);

  if(mode & GSM_READ_CSR) {

    //
    // Read from PCI config space
    //
    ddrCRCompCtl0.Data = MemReadPciCfgMC (socket, imc, DDRCRCOMPCTL0_MCIO_DDRIOEXT_REG);

    if ((group == RxOdt) && IsSiliconWorkaroundEnabled ("S1409370801")) {
      ddrCRCompCtl3.Data = MemReadPciCfgMC (socket, imc, DDRCRCOMPCTL3_MCIO_DDRIOEXT_REG);
    }

  } else {
    // Read from cache
    //
    ddrCRCompCtl0.Data = (*imcNvList)[imc].ddrCRCompCtl0;

    if ((group == RxOdt) && IsSiliconWorkaroundEnabled ("S1409370801")) {
      ddrCRCompCtl3.Data = (*imcNvList)[imc].ddrCRCompCtl3;
    }

  } // DDR_LEVEL

  switch (group) {
    case RxOdt:
      // Set the cached ODT segments based on the ODT value
      for (Strobes = 0; Strobes < MSVx4; Strobes++) {
        if (IsStrobeNotValid(Host, Strobes)) {
          continue;
        }

        if (*value < 100) {
          (*ChannelNvList)[ch].OdtSegmentEnable[Strobes] = ODT_SEGMENTS_EN_2;
        } else {
          (*ChannelNvList)[ch].OdtSegmentEnable[Strobes] = ODT_SEGMENTS_EN_1;
        }
      }

      if (IsSiliconWorkaroundEnabled ("S1409370801")) {
        tempVal = (UINT16)ddrCRCompCtl3.Bits.ctldrvpupvref;
      } else {
        tempVal = (UINT16)ddrCRCompCtl0.Bits.dqodtpupvref;
      }
      curVal  = ConvertOdtValue (Host, group, tempVal, CODE_ODT, Host->nvram.mem.dimmTypePresent);
      break;
    case TxRon:
      tempVal = (UINT16)ddrCRCompCtl0.Bits.dqdrvpupvref;
      curVal  = ConvertOdtValue (Host, group, tempVal, CODE_ODT, Host->nvram.mem.dimmTypePresent);
      break;
    default:
      break;
  }

  //
  // Check if read only
  //
  if (mode & GSM_READ_ONLY) {
    //
    // Save the value read into the output parameter
    //
    *value = curVal;
  } else {
    //
    // Write
    //
    // Adjust the current CMD delay value by offset
    if (mode & GSM_WRITE_OFFSET) {
      //
      // Add the offset to the current value
      //
      tmp = tempVal + *value;

      //
      // Make sure we do not exeed the limits
      //
      if (tmp > minLimit) {
        piDelay = tmp;
      } else {
        piDelay = minLimit;
      }
      // Ensure we do not exceed maximums
      if (piDelay > maxLimit) {
        piDelay = maxLimit;
      }
    } else {
      //
      // Write absolute value
      //
      tempVal = *value;
      piDelay = ConvertOdtValue(Host, group, tempVal, DECODE_ODT, Host->nvram.mem.dimmTypePresent);
    }

    switch (group) {
      case RxOdt:
        if ((group == RxOdt) && IsSiliconWorkaroundEnabled ("S1409370801")) {
          ddrCRCompCtl3.Bits.ctldrvpupvref = piDelay;
        } else {
          ddrCRCompCtl0.Bits.dqodtpupvref = piDelay;
        }

        for (Strobes = 0; Strobes < MSVx4; Strobes++) {
          if (IsStrobeNotValid (Host, Strobes)) {
            continue;
          }

          DataControl3.Data = ReadDdrioDataRegCh (Host, socket, ch, Strobes, DATACONTROL3N0_0_MCIO_DDRIO_REG);
          DataControl3.Bits.odtsegmentenable = (1 << (*ChannelNvList)[ch].OdtSegmentEnable[Strobes]) - 1;
          if (DataControl3.Data != (*ChannelNvList)[ch].dataControl3[Strobes]) {
            WriteDdrioDataRegCh (Host, socket, ch, Strobes, DATACONTROL3N0_0_MCIO_DDRIO_REG, DataControl3.Data);
          }
        }

        break;
      case TxRon:
        ddrCRCompCtl0.Bits.dqdrvpupvref = piDelay;
        break;
      default:
        break;
    }

    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
    // no cache
    if ((group == RxOdt) && IsSiliconWorkaroundEnabled ("S1409370801")) {
      if ((ddrCRCompCtl3.Data != (*imcNvList)[imc].ddrCRCompCtl3) || (mode & GSM_FORCE_WRITE)) {
        MemWritePciCfgMC (socket, imc, DDRCRCOMPCTL3_MCIO_DDRIOEXT_REG, ddrCRCompCtl3.Data);
        DoComp (Host, socket);
        DdrCrDataComp1.Data = MemReadPciCfgMC (socket, imc, DDRCRDATACOMP1_MCIO_DDRIOEXT_REG);
        DdrCrCtlComp.Data = MemReadPciCfgMC (socket, imc, DDRCRCTLCOMP_CMDN_MCIO_DDRIOEXT_REG);
        DdrCrDataComp1.Bits.rcompodtup = DdrCrCtlComp.Bits.rcompdrvup;
        MemWritePciCfgMC (socket, imc, DDRCRDATACOMP1_MCIO_DDRIOEXT_REG, DdrCrDataComp1.Data);
        DoComp (Host, socket);
      }
    } else {
      if ((ddrCRCompCtl0.Data != (*imcNvList)[imc].ddrCRCompCtl0) || (mode & GSM_FORCE_WRITE)) {
        MemWritePciCfgMC (socket, imc, DDRCRCOMPCTL0_MCIO_DDRIOEXT_REG, ddrCRCompCtl0.Data);
        DoComp (Host,socket);
      }
    }
  } // if read only

  //
  // Save to cache
  //
  if (mode & GSM_UPDATE_CACHE) {
    // no cache
    if ((group == RxOdt) && IsSiliconWorkaroundEnabled ("S1409370801")) {
      (*imcNvList)[imc].ddrCRCompCtl3 = ddrCRCompCtl3.Data;
    } else {
      (*imcNvList)[imc].ddrCRCompCtl0 = ddrCRCompCtl0.Data;
    }

    if (group == RxOdt) {
      for (Strobes = 0; Strobes < MSVx4; Strobes++) {
        if (IsStrobeNotValid(Host, Strobes)) {
          continue;
        }

        DataControl3.Data = ReadDdrioDataRegCh (Host, socket, ch, Strobes, DATACONTROL3N0_0_MCIO_DDRIO_REG);
        (*ChannelNvList)[ch].dataControl3[Strobes] = DataControl3.Data;
      }
    }
  }

  return MRC_STATUS_SUCCESS;
} // GetSetCPUODT

#if !(defined (ICX_HOST) && defined (DDR5_SUPPORT))
/*++

Routine Description:

  May read (e.g. to print them) or update multiple ODT values from/to the MRC cache

Arguments:

  Host    - Pointer to sysHost
  socket  - Socket number
  ch      - Channel number (0-based)
  dimm    - DIMM number (0-based)
  rank    - Rank number (0-based)
  strobe  - Strobe number (0-based)
  bit     - Bit number
  level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  group   - CSM_GT - Parameter to program
  mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  value   - Data to program

Returns:

  The call itself always returns MRC_STATUS_SUCCESS but the IN/OUT parameter "value" may pass-back different ODT parameter values to the caller

--*/
MRC_STATUS
EFIAPI
GetSetDimmOdtDdr4(
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value
  )
{
#ifndef DDR5_SUPPORT
  UINT16              curVal = 0;
  UINT16              tempVal = 0;
  UINT16              piDelay = 0;
  INT16               tmp;
  UINT16              maxLimit = 15;
  UINT16              minLimit = 0;
  UINT8               TempStrobe = 0;
  UINT16              TempMr1Value = 0;
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  struct rankDevice   (*rankStruct)[MAX_RANK_DIMM];
#ifdef  LRDIMM_SUPPORT
  struct channelNvram (*channelNvList)[MAX_CH];
#endif // LRDIMM_SUPPORT
  SYS_SETUP           *Setup;

  Setup = GetSysSetupPointer ();

  rankList = GetRankNvList(Host, socket, ch, dimm);
  rankStruct = GetRankStruct(Host, socket, ch, dimm);
#ifdef  LRDIMM_SUPPORT
  channelNvList = GetChannelNvList(Host, socket);
#endif

  //
  // Get minimum and maximum value
  //
  //GetDataGroupLimits(Host, level, group, &minLimit, &maxLimit, &usDelay);

  //
  // Read from cache
  //
#ifdef  LRDIMM_SUPPORT
  if (IsLrdimmPresent (socket, ch, dimm)) {
    switch (group) {
    case DramDrvStr:
      tempVal = (*channelNvList)[ch].dimmList[dimm].lrBuf_BC03;
      curVal  = ConvertOdtValue (Host, group, tempVal, CODE_ODT, LRDIMM);
      break;
    case WrOdt:
      tempVal = (*channelNvList)[ch].dimmList[dimm].lrBuf_BC01;
      curVal  = ConvertOdtValue (Host, group, tempVal, CODE_ODT, LRDIMM);
      break;
    case ParkOdt:
      tempVal = (*channelNvList)[ch].dimmList[dimm].lrBuf_BC02;
      curVal  = ConvertOdtValue (Host, group, tempVal, CODE_ODT, LRDIMM);
      break;
    case NomOdt:
      tempVal = (*channelNvList)[ch].dimmList[dimm].lrBuf_BC00;
      curVal  = ConvertOdtValue (Host, group, tempVal, CODE_ODT, LRDIMM);
      break;
    default:
      break;
    }
  } else
#endif
  {
    switch (group) {
    case DramDrvStr:
      tempVal = ((*rankStruct)[rank].MR1 & BIT1) >> 1;
      curVal  = ConvertOdtValue (Host, group, tempVal, CODE_ODT, Host->nvram.mem.dimmTypePresent);
      break;
    case WrOdt:
      tempVal = (*rankList)[rank].RttWr >> DDR4_RTT_WR_ODT_SHIFT;
      curVal  = ConvertOdtValue (Host, group, tempVal, CODE_ODT, Host->nvram.mem.dimmTypePresent);
      break;
    case ParkOdt:
      tempVal = (*rankList)[rank].RttPrk >> DDR4_RTT_PRK_ODT_SHIFT;
      curVal  = ConvertOdtValue (Host, group, tempVal, CODE_ODT, Host->nvram.mem.dimmTypePresent);
      break;
    case NomOdt:
      tempVal = (*rankList)[rank].RttNom >> DDR4_RTT_NOM_ODT_SHIFT;
      curVal  = ConvertOdtValue (Host, group, tempVal, CODE_ODT, Host->nvram.mem.dimmTypePresent);
      break;
    default:
      break;
    }
  }

  //
  // Check if read only
  //
  if (mode & GSM_READ_ONLY) {
    //
    // Save the value read into the output parameter
    //
    *value = curVal;
  } else {
    //
    // Write
    //
    // Adjust the current CMD delay value by offset
    if (mode & GSM_WRITE_OFFSET) {
      //
      // Add the offset to the current value
      //
      tmp = tempVal + *value;

      //
      // Make sure we do not exceed the limits
      //
      if (tmp > minLimit) {
        piDelay = tmp;
      } else {
        piDelay = minLimit;
      }
      // Ensure we do not exceed maximums
      if (piDelay > maxLimit) {
        piDelay = maxLimit;
      }
    } else {
      //
      // Write absolute value
      //
      tempVal = *value;
#ifdef  LRDIMM_SUPPORT
      if (IsLrdimmPresent (socket, ch, dimm)) {
        piDelay = ConvertOdtValue (Host, group, tempVal, DECODE_ODT, LRDIMM);
      } else
#endif
      {
        if ((tempVal == 60) && (group == WrOdt)){ // Invalid RTT_WRT setting for
          RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_62);
        }
        piDelay = ConvertOdtValue (Host, group, tempVal, DECODE_ODT, RDIMM);
      }
    }

    {
      //
      // Write it back if the current data does not equal the cache value or if in force write mode
      //
#ifdef  LRDIMM_SUPPORT
      if (IsLrdimmPresent (socket, ch, dimm)) {
        switch (group) {
        case DramDrvStr:
          (*channelNvList)[ch].dimmList[dimm].lrBuf_BC03 = (UINT8) piDelay;
          WriteLrbuf(Host, socket, ch, dimm, 0, (UINT8) piDelay, LRDIMM_F0, LRDIMM_BC03);
          break;
        case WrOdt:
          (*channelNvList)[ch].dimmList[dimm].lrBuf_BC01 = (UINT8) piDelay;
          WriteLrbuf(Host, socket, ch, dimm, 0, (UINT8) piDelay, LRDIMM_F0, LRDIMM_BC01);
          break;
        case ParkOdt:
          (*channelNvList)[ch].dimmList[dimm].lrBuf_BC02 = (UINT8) piDelay;
          WriteLrbuf(Host, socket, ch, dimm, 0, (UINT8) piDelay, LRDIMM_F0, LRDIMM_BC02);
          break;
        case NomOdt:
          (*channelNvList)[ch].dimmList[dimm].lrBuf_BC00 = (UINT8) piDelay;
          WriteLrbuf(Host, socket, ch, dimm, 0, (UINT8) piDelay, LRDIMM_F0, LRDIMM_BC00);
          break;
        default:
          break;
        }
      } else
#endif
      {
        switch (group) {
        case DramDrvStr:
          (*rankStruct)[rank].MR1 &= ~BIT1;
          (*rankStruct)[rank].MR1 |= (piDelay << 1);
          if (Host->nvram.mem.socket[socket].TrainingStepDone.DramRxEq == 1) {
            for (TempStrobe = 0; TempStrobe < MAX_STROBE; TempStrobe++) {
              TempMr1Value = GetPerStrobeMrsValue (socket, ch, dimm, rank, TempStrobe, BANK1);
              WriteMRSPDA (Host, socket, ch, dimm, rank, strobe, TempMr1Value, BANK1);
            } //TempStrobe loop
          } else {
            WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR1, BANK1);
          }
          break;
        case WrOdt:
          (*rankList)[rank].RttWr = piDelay << DDR4_RTT_WR_ODT_SHIFT;
          (*rankStruct)[rank].MR2 &= ~(0x3 << DDR4_RTT_WR_ODT_SHIFT); // clear RTTWR from MR2
          (*rankStruct)[rank].MR2 |= (*rankList)[rank].RttWr;
          WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR2, BANK2);
          break;
        case ParkOdt:
          (*rankList)[rank].RttPrk = piDelay << DDR4_RTT_PRK_ODT_SHIFT;
          (*rankStruct)[rank].MR5 &= ~(0x7 << DDR4_RTT_PRK_ODT_SHIFT);
          (*rankStruct)[rank].MR5 |= (*rankList)[rank].RttPrk;
          WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR5, BANK5);
          break;
        case NomOdt:
          (*rankList)[rank].RttNom = piDelay << DDR4_RTT_NOM_ODT_SHIFT ;
          (*rankStruct)[rank].MR1 &= ~(0x7 << DDR4_RTT_NOM_ODT_SHIFT); // clear RTTNOM from MR1
          (*rankStruct)[rank].MR1 |= (*rankList)[rank].RttNom;
          if (Host->nvram.mem.socket[socket].TrainingStepDone.DramRxEq == 1) {
            for (TempStrobe = 0; TempStrobe < MAX_STROBE; TempStrobe++) {
              TempMr1Value = GetPerStrobeMrsValue (socket, ch, dimm, rank, TempStrobe, BANK1);
              WriteMRSPDA (Host, socket, ch, dimm, rank, strobe, TempMr1Value, BANK1);
            } //TempStrobe loop
          } else {
            WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR1, BANK1);
          }
          break;
        default:
          break;
        }
      }
    }
  } // if read only

  //
  // Save to cache
  //
  if (mode & GSM_UPDATE_CACHE) {
#ifdef  LRDIMM_SUPPORT
    if ((*channelNvList)[ch].lrDimmPresent) {
    } else
#endif
    {
      switch (group) {
      case DramDrvStr:
        (*rankStruct)[rank].MR1 &= ~BIT1;
        (*rankStruct)[rank].MR1 |= (piDelay << 1);
        (*rankList)[rank].OutDrvImpCtrl = (piDelay << 1);
        break;
      case WrOdt:
        break;
      case ParkOdt:
        break;
      case NomOdt:
        break;
      default:
        break;
      }
    }
  }

  return MRC_STATUS_SUCCESS;
#else // !DDR5_SUPPORT
  return MRC_STATUS_FAILURE;
#endif // !DDR5_SUPPORT
}
#endif

/**

  PlatformWriteDimmVref: Procedure to write the DIMM Vref setting to the given target

  @param Host - Pointer to sysHost struct
  @param socket - processor number
  @param vref - vref value to write

  @retval SUCCESS if the Vref circuit is present and can be written
  @retval FAILURE if the Vref circuit is not present or cannot be written

**/
STATIC
UINT32
MemWriteDimmVref (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  MRC_LT    level,
  UINT8     vref
  )
{
  UINT8                 vrefRange;
  UINT8                 dram;
  struct channelNvram   (*channelNvList)[MAX_CH];
  struct rankDevice     (*rankStruct)[MAX_RANK_DIMM];
  struct dimmNvram      (*dimmNvList)[MAX_DIMM];
#ifdef LRDIMM_SUPPORT
  struct ddrRank        (*rankList)[MAX_RANK_DIMM];
  UINT8                 HostSideBufferTxVref[MAX_STROBE / 2];
  UINT8                 BufferIndex;
#endif
#ifdef DDR5_SUPPORT
  DDR5_MODE_REGISTER_10_STRUCT  Mr10;
#endif // #ifdef DDR5_SUPPORT
  UINT8 MSVx4 = GetMaxStrobeValid(Host);
  UINT8 MSVx8 = GetMaxStrobeValid(Host) / 2;
  SYS_SETUP             *Setup;

  Setup = GetSysSetupPointer ();

  channelNvList = GetChannelNvList(Host, socket);
  rankStruct    = GetRankStruct(Host, socket, ch, dimm);

  dimmNvList = GetDimmNvList(Host, socket, ch);

  //check to make sure that strobe is within the range of strobes (used as array index in function)
  if (strobe >= MSVx4) {
    return FAILURE;
  }

#ifdef LRDIMM_SUPPORT
  if (level == LrbufLevel) {
    // for LR buffer side, only support RANGE1 (TODO: see if RANGE 2 is needed)
    vrefRange = vref;
    // Enforce upper limit per JEDEC spec
    if (vrefRange > 50) {
      vrefRange = 50;
    }
  } else
#endif //LRDIMM
  {
    //
    // Select the Vref range
    //
    vrefRange = GetVrefRange (Host, vref);
  }
#ifdef LRDIMM_SUPPORT
    if (IsLrdimmPresent (socket, ch, dimm)) {

      if (level == DdrLevel) {

      rankList = GetRankNvList(Host, socket, ch, dimm);
      //
      // Get per strobe range settings into an array
      //
      // (take vref range select bit 6 and put it in the LRDIMM buffer register bit 1)
      //NOTE: the backside DRAM range is written to range 1...ok for now, but may need to change later
      for (BufferIndex = 0; BufferIndex < MSVx8; BufferIndex++) {
        HostSideBufferTxVref[BufferIndex] = ((((GetVrefRange(Host, (*channelNvList)[ch].txVref[(*rankList)[rank].rankIndex][BufferIndex])) & BIT6) >> 5) | BIT0);
      }
      HostSideBufferTxVref[strobe % MSVx8] = ((vrefRange & BIT6) >> 5) | BIT0;
      if ((Setup->mem.optionsExt & PDA_EN) &&
          (Host->nvram.mem.socket[socket].TrainingStepDone.TxVref == 1))
      {
      //NOTE: the backside DRAM range is written to range 1...ok for now, but may need to change later
        WriteLrbufPBA(Host, socket, ch, dimm, rank, HostSideBufferTxVref, LRDIMM_F6, LRDIMM_BC4x, (strobe % MSVx8));
      } else {
        WriteLrbuf(Host, socket, ch, dimm, 0, HostSideBufferTxVref[0], LRDIMM_F6, LRDIMM_BC4x);
      }
      //
      // Get per strobe txVref settings into an array
      //
      for (BufferIndex = 0; BufferIndex < MSVx8; BufferIndex++) {
        HostSideBufferTxVref[BufferIndex] = ((GetVrefRange(Host, (*channelNvList)[ch].txVref[(*rankList)[rank].rankIndex][BufferIndex])) & 0x3F);
      }
      HostSideBufferTxVref[strobe % MSVx8] = vrefRange & 0x3F;
      //
      // program LRDIMM Host Interface VREF Control Word
      //
      if ((Setup->mem.optionsExt & PDA_EN) &&
          (Host->nvram.mem.socket[socket].TrainingStepDone.TxVref == 1))
      {
        WriteLrbufPBA(Host, socket, ch, dimm, rank, HostSideBufferTxVref, LRDIMM_F5, LRDIMM_BC5x, (strobe % MSVx8));
      } else {
        WriteLrbuf(Host, socket, ch, dimm, 0, HostSideBufferTxVref[0], LRDIMM_F5, LRDIMM_BC5x);
      }
    } else {
      //LEVEL = LrbufLevel

      // Update MR6 value with new vref setting mode value
      (*rankStruct)[rank].MR6[strobe] &= ~0x7F;
      (*rankStruct)[rank].MR6[strobe] |= (vrefRange & 0x7F);

      MRSCmdStacking (Host, socket, ch, dimm, LOAD_MRS);
      // Write the new DRAM Vref value and enable VrefDQ training mode
      WriteMRS(Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR6[strobe] | BIT7, BANK6);
      WriteMRS(Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR6[strobe] | BIT7, BANK6);
      WriteMRS(Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR6[strobe], BANK6);
      MRSCmdStacking (Host, socket, ch, dimm, EXECUTE_MRS);

    }
    WriteBC(Host, socket, ch, dimm, 0, 0, LRDIMM_F0, LRDIMM_BC7x);
    (*channelNvList)[ch].dimmList[dimm].rcLrFunc = 0;

    } else
#endif
    {
      // Update MR6 value with new vref setting mode value
      (*rankStruct)[rank].MR6[strobe] &= ~0x7F;
      (*rankStruct)[rank].MR6[strobe] |= (vrefRange & 0x7F);

      // Write the new DRAM Vref value and enable VrefDQ training mode
      if ((Setup->mem.optionsExt & PDA_EN) &&
          (Host->nvram.mem.socket[socket].TrainingStepDone.TxVref == 1))
      {
        if ((*dimmNvList)[dimm].x4Present) {
          if (strobe >= MSVx8) {
            dram = ((strobe - MSVx8) * 2) + 1;
          } else {
            dram = strobe * 2;
          }
        } else {
          dram = strobe % MSVx8;
        }
#ifdef DDR5_SUPPORT
        if (IsDdr5Present (Host, socket)) {
          WriteMPC (Host, socket, ch, dimm, TWO_SUBCH_MASK, rank, PDA_SELECT_ID | dram, TIMING_4N, TRUE);
          Mr10.Data = 0;
          Mr10.Bits.vrefdq_cal_value = vrefRange;
          DramModeRegWrite (socket, ch, 1 << ((ch) % 2), dimm, rank, DDR5_MODE_REGISTER_10_REG, Mr10.Data);
          WriteMPC (Host, socket, ch, dimm, TWO_SUBCH_MASK, rank, PDA_SELECT_ALL, TIMING_4N, TRUE);
        } else
#endif // #ifdef DDR5_SUPPORT
        {
          WriteMRSPDA (Host, socket, ch, dimm, rank, dram, (*rankStruct)[rank].MR6[strobe], BANK6);
        }
      } else {
#ifdef DDR5_SUPPORT
        if (IsDdr5Present (Host, socket)) {
          Mr10.Data = 0;
          Mr10.Bits.vrefdq_cal_value = vrefRange;
          DramModeRegWrite (socket, ch, 1 << ((ch) % 2), dimm, rank, DDR5_MODE_REGISTER_10_REG, Mr10.Data);
        } else
#endif // #ifdef DDR5_SUPPORT
        {
          WriteMR6 (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR6[strobe], BANK6);
        }
      }
    }
  return SUCCESS;
} // MemWriteDimmVref

/**

  Converts the Vref setting to be written into MR6

  @param Host - Pointer to sysHost struct
  @param vref - vref value to write

**/
UINT8
GetVrefRange (
  PSYSHOST  Host,
  UINT8     vref
  )
{
  UINT8 vrefRange;
#ifdef DDR5_SUPPORT
  if (IsDdr5Present (Host, Host->var.mem.currentSocket)) {
    vrefRange = vref;
    // Enforce lower limit per JEDEC spec
    if (vrefRange > DDR5_MAX_VREF_RANGE) {
      vrefRange = DDR5_MAX_VREF_RANGE;
    }
  } else
#endif // #ifdef DDR5_SUPPORT
  {
    //
    // Select the Vref range
    //
    if (vref < DDR4_VREF_RANGE1_OFFSET) {

      // Use range 2 and enforce lower limit
      if (vref > DDR4_VREF_RANGE2_OFFSET) {
        vrefRange = (vref - DDR4_VREF_RANGE2_OFFSET) | BIT6;
      } else {
        vrefRange = BIT6;
      }

    } else {

      // Use range 1
      vrefRange = vref - DDR4_VREF_RANGE1_OFFSET;

      // Enforce upper limit per JEDEC spec
      if (vrefRange > 50) {
        vrefRange = 50;
      }
    }
  }
  return vrefRange;
} // GetVrefRange

/**

  Converts the Vref setting to be written into MR6

  @param Host    - Pointer to sysHost struct
  @param socket  - processor number

**/
VOID
UpdateSafeTxVref (
  PSYSHOST  Host,
  UINT8     socket,
  MRC_LT    level
  )
{
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               strobe;
  UINT8               maxStrobe;
  UINT8               hiVref;
  UINT8               loVref;
  UINT8               safelevel;
  INT16               vref;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  UINT8               MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList(Host, socket, ch);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      // skip NVMDIMM backside
      if (((*dimmNvList)[dimm].DcpmmPresent) && (level == LrbufLevel)) {
        continue;
      }
      // skip if lrdimm level but no LRDIMM found
      if ((level == LrbufLevel) && (IsLrdimmPresent (socket, ch, dimm) == 0)) {
        continue;
      }
      maxStrobe = GetMaxStrobe(Host, socket, ch, dimm, TxVref, level);

      rankList = GetRankNvList(Host, socket, ch, dimm);

      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (IsLrdimmPresent (socket, ch, dimm) && (rank > 0)) {
          continue;
        }
        loVref = 0xFF;
        hiVref = 0;

        for (strobe = 0; strobe < maxStrobe; strobe++) {
          if (IsStrobeNotValid(Host, strobe)) {
            continue;
          }

          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, ALL_BITS, level, TxVref, GSM_READ_ONLY, &vref);

          if (loVref > vref) {
            loVref = (UINT8)vref;
          }
          if (hiVref < vref) {
            hiVref = (UINT8)vref;
          }
        } // strobe loop

        safelevel = (UINT8)((loVref + hiVref) / 2);

        if (level ==  DdrLevel) {
          (*channelNvList)[ch].txVrefSafe[(*rankList)[rank].rankIndex] = safelevel;
        }
#ifdef DDR5_SUPPORT
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
              "txVrefSafe = 0x%x(Physical) %02d (Linear)\n", ConvertLinearToPhysical (safelevel), safelevel);
#endif // #ifdef DDR5_SUPPORT
      } // rank loop
    } // dimm loop
  } // ch loop
} // UpdateSafeTxVref

/**

  Get the limits for the group or level requested

  @param Host      - Pointer to sysHost
  @param level     - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group     - CSM_GT - Parameter to program
  @param minLimit  - Minimum delay value allowed
  @param maxLimit  - Maximum absolute value allowed
  @param usdelay   - Time in usec required for new setting to take effect

  @retval MRC_STATUS

**/
MRC_STATUS
GetDataGroupLimits (
  PSYSHOST  Host,
  MRC_LT    level,
  MRC_GT    group,
  UINT16    *minLimit,
  UINT16    *maxLimit,
  UINT16    *usDelay
  )
{
  MRC_STATUS  status = MRC_STATUS_SUCCESS;
  UINT8       Socket;

  Socket = Host->var.mem.currentSocket;

  if (level == DdrLevel) {
    switch (group) {
    case RxEq:
      *minLimit = 0;
      *maxLimit = 7;
      *usDelay  = 0;
      break;
    case TxEq:
      *minLimit = 0;
      *maxLimit = 9;
      *usDelay  = 0;
      break;
    case TxEqTap2:
      *minLimit = 0;
      *maxLimit = 6;
      *usDelay  = 0;
      break;
    case RecEnDelay:
      *minLimit = 0;
      *maxLimit = 639;
      *usDelay  = 0;
      break;
    case WrLvlDelay:
    case TxDqsDelay:
    case TxDqDelay:
    case TxDqBitDelay:
      *minLimit = 0;
      *maxLimit = 639;
      *usDelay  = 0;
      break;
    case RxDqsDelay:
    case RxDqsPDelay:
    case RxDqsNDelay:
      *minLimit = 0;
      if (GetRxDfeEnableChip (Host, Socket) == TRUE) {
        *maxLimit = 151;
      } else {
        *maxLimit = 127; // Register is only 7 bit width
      }
      *usDelay  = 0;
      break;
    case TxTco:
      *minLimit = 0;
      *maxLimit = 127;
      *usDelay  = 0;
      break;
    case RxDqsBitDelay:
    case RxDqsPBitDelay:
    case RxDqsNBitDelay:
      *minLimit = 0;
      *maxLimit = 20;
      *usDelay  = 0;
      break;
      //case RxDqDelay:
    case RxVref:
      *minLimit = 0;
      *maxLimit = 127;
      *usDelay  = 1;
      break;
    case TxVref:
#ifdef DDR5_SUPPORT
      if (IsDdr5Present (Host, Socket)) {
        *maxLimit = DDR5_MAX_VREF_RANGE;
        *minLimit = 0;
      } else
#endif // #ifdef DDR5_SUPPORT
      {
        *maxLimit = DDR4_VREF_RANGE1_OFFSET + 50;
        *minLimit = DDR4_VREF_RANGE2_OFFSET;
      }
      *usDelay  = 1;
      break;
    case RxDqDelay:
      *minLimit = 0;
      *maxLimit = 7;
      *usDelay  = 0;
      break;
    case RxDqDqsDelay:
      *minLimit = 0;
      if (GetRxDfeEnableChip (Host, Socket) == TRUE) {
        *maxLimit = 151;
      } else {
        *maxLimit = 127;
      }
      *usDelay  = 0;
      break;
    case TxRiseFallSlewRate:
    case TxFallSlewRate:
      *minLimit = 0;
      *maxLimit = 31;
      *usDelay  = 0;
      break;
    case DcaDelay:
      *minLimit = 0;
      *maxLimit = 255;
      break;
    case PxcDelta:
    case DdjcDelta:
      *minLimit = 0;
      *maxLimit = 3;
      *usDelay = 0;
      break;
    case RxDfeCoefficient:
      *minLimit = 0;
      *maxLimit = 14;
      *usDelay = 0;
      break;
    case RxSenseAmpOffset:
      *minLimit = 0;
      *maxLimit = 63;
      *usDelay = 0;
      break;
    case RxFifoDqsPBitSkew:
    case RxFifoDqsNBitSkew:
      *minLimit = 0;
      *maxLimit = 3;
      *usDelay = 0;
      break;
    case TxDqPerChDelayOffset:
    case RxDqsPerChDelayOffset:
      *minLimit = 0;
      *maxLimit = 63;
      *usDelay = 0;
      break;
    case RxStrobeInvert:
      *minLimit = 0;
      *maxLimit = 1;
      *usDelay = 0;
      break;
    case EridVref:
      *minLimit = 0;
      *maxLimit = 255;
      *usDelay = 1;
      break;
    default:
      status = MRC_STATUS_GROUP_NOT_SUPPORTED;
      break;
    } // switch group
#ifdef LRDIMM_SUPPORT
  } else if (level == LrbufLevel) {
    switch (group) {
    case TxDqDelay:
    case RxDqsPDelay:
    case RxDqsNDelay:
    case RxDqsDelay:
      *minLimit = 0;
      *maxLimit = 30;
      *usDelay  = 0;
      break;
    case RxVref:
      *maxLimit = 73;
      *minLimit = 0;
      *usDelay  = 1;
      break;

    case TxVref:
      *maxLimit = 50;
      *minLimit = 0;
      *usDelay  = 1;
      break;

    case DbDfeTap1:
    case DbDfeTap2:
    case DbDfeTap3:
    case DbDfeTap4:
      *maxLimit = 30;
      *minLimit = 0;
      *usDelay = 1;
      break;

    default:
      status = MRC_STATUS_GROUP_NOT_SUPPORTED;
      break;
    } // switch group
#endif
  } else {
    status = MRC_STATUS_LEVEL_NOT_SUPPORTED;
  }

  return status;
} // GetDataGroupLimits

/**
  Cache all RoundTrip registers for a channel

  @param  Host      - Pointer to sysHost structure
  @param  socket    - Socket number
  @param  channel   - Channel number

  @retval EFI_SUCCESS   Round trip registers were successfully cached
  @retval !EFI_SUCCESS  Round trip registers were not successfully cached

**/
EFI_STATUS
CacheRoundTrip (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch
  )
{
  EFI_STATUS                                Status;
  EFI_STATUS                                TemporaryStatus;
  UINT8                                     MemoryController;
  UINT8                                     MemoryControllerChannel;
  UINT8                                     NumChPerMc;
  DDRCRINTFROUNDTRIP0_CH0_MCIO_DDRIO_STRUCT RoundTrip0;
  DDRCRINTFROUNDTRIP1_CH0_MCIO_DDRIO_STRUCT RoundTrip1;
  UINT32                                    RoundTripRegOffset;
  struct channelNvram                       (*channelNvList)[MAX_CH];
  struct RankCh                             (*RankPerCh)[MAX_RANK_CH];
  UINT8                                     MaxChDdr;

  MaxChDdr   = GetMaxChDdr ();
  NumChPerMc = GetNumChannelPerMc ();

  if ((Ch >= MaxChDdr) ||
      (MAX_RANK_CH < DIMM_ROUND_TRIP_RANKS))
  {
    // An invalid preconditions for this function was observed:
    // 1. The channel index is too high.
    // 2. The number of channel ranks is too low.
    // Either would lead to out-of-bounds accesses.
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_87);
    return FAILURE;
  }

  channelNvList = GetChannelNvList (Host, Socket);
  RankPerCh = &(*channelNvList)[Ch].rankPerCh;
  MemoryController = Ch / NumChPerMc;
  MemoryControllerChannel = Ch % NumChPerMc;

  Status = GetRoundTripRegOffset (MemoryControllerChannel, 0, &RoundTripRegOffset);
  RoundTrip0.Data = MemReadPciCfgMC (Socket, MemoryController, RoundTripRegOffset);
  (*RankPerCh)[0].Roundtrip = (UINT8)RoundTrip0.Bits.rt_rank0;
  (*RankPerCh)[1].Roundtrip = (UINT8)RoundTrip0.Bits.rt_rank1;
  (*RankPerCh)[2].Roundtrip = (UINT8)RoundTrip0.Bits.rt_rank2;
  (*RankPerCh)[3].Roundtrip = (UINT8)RoundTrip0.Bits.rt_rank3;

  TemporaryStatus = GetRoundTripRegOffset (MemoryControllerChannel, 1, &RoundTripRegOffset);
  if (!EFI_ERROR (Status)) {
    Status = TemporaryStatus;
  }
  RoundTrip1.Data = MemReadPciCfgMC (Socket, MemoryController, RoundTripRegOffset);
  (*RankPerCh)[4].Roundtrip = (UINT8)RoundTrip1.Bits.rt_rank4;
  (*RankPerCh)[5].Roundtrip = (UINT8)RoundTrip1.Bits.rt_rank5;
  (*RankPerCh)[6].Roundtrip = (UINT8)RoundTrip1.Bits.rt_rank6;
  (*RankPerCh)[7].Roundtrip = (UINT8)RoundTrip1.Bits.rt_rank7;

  return Status;
}

/**
  Cache IoLatency registers for a channel

  @param  Host      - Pointer to sysHost structure
  @param  socket    - Socket number
  @param  channel   - Channel number

  @retval SUCCESS if IoLatency cached
          FAILURE otherwise
**/
UINT32
CacheIoLatency (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch
  )
{
  UINT8                                     RankIndex;
  UINT8                                     NumChPerMc;
  DDRCRINTFIOLATENCY0_CH0_MCIO_DDRIO_STRUCT IoLatency0;
  UINT32                                    IoLatency0Ch[] = {DDRCRINTFIOLATENCY0_CH0_MCIO_DDRIO_REG,
                                                              DDRCRINTFIOLATENCY0_CH1_MCIO_DDRIO_REG,
                                                              DDRCRINTFIOLATENCY0_CH2_MCIO_DDRIO_REG
                                                             };
  UINT32                                    IoLatency1Ch[] = {DDRCRINTFIOLATENCY1_CH0_MCIO_DDRIO_REG,
                                                              DDRCRINTFIOLATENCY1_CH1_MCIO_DDRIO_REG,
                                                              DDRCRINTFIOLATENCY1_CH2_MCIO_DDRIO_REG
                                                             };
  struct channelNvram                       (*ChannelNvList)[MAX_CH];
  struct RankCh                             (*RankPerCh)[MAX_RANK_CH];
  UINT8                                     Index;
  UINT8                                     MaxChDdr;

  MaxChDdr   = GetMaxChDdr ();
  NumChPerMc = GetNumChannelPerMc ();

  if (Ch >= MaxChDdr) {
    // invalid ch specified
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_86);
    return FAILURE;
  }

  ChannelNvList = GetChannelNvList (Host, Socket);
  RankPerCh = &(*ChannelNvList)[Ch].rankPerCh;

  Index = Ch % NumChPerMc;

  //
  // IoLatency0 is stored in NVRAM in per-rank structures
  //
  IoLatency0.Data = MemReadPciCfgEp (Socket, Ch, IoLatency0Ch[Index]);
  for (RankIndex = 0; RankIndex < MAX_RANK_CH; RankIndex++) {
#ifdef DDR5_SUPPORT
    (*RankPerCh)[RankIndex].IOLatency[SUB_CH_A] = (UINT8)((IoLatency0.Data >> (RankIndex * 4)) & 0xF);
#else
    (*RankPerCh)[RankIndex].IOLatency = (UINT8)((IoLatency0.Data >> (RankIndex * 4)) & 0xF);
#endif // #ifdef DDR5_SUPPORT
  }
  //
  // IoLatency1 is stored in NVRAM in per-channel structures
  //
#ifdef DDR5_SUPPORT
  (*ChannelNvList)[Ch].ioLatency1[SUB_CH_A] = MemReadPciCfgEp (Socket, Ch, IoLatency1Ch[Index]);
#else
  (*ChannelNvList)[Ch].ioLatency1 = MemReadPciCfgEp (Socket, Ch, IoLatency1Ch[Index]);
#endif // #ifdef DDR5_SUPPORT

  return SUCCESS;
}

/**

  Cache all DDRIO registers

  @param Host    - Pointer to sysHost
  @param socket  - Socket number

**/
UINT32
CacheDDRIO (
  PSYSHOST  Host,
  UINT8     socket
  )
{
  UINT8               imc;
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               strobe;
  UINT8               Bit;
  UINT8               MaxImc;
  UINT16              minVal = 0xFF;
  UINT16              maxVal = 0x0;
  UINT16              zeroValue = 0;
  INT16               tempVal = 0;
  struct imcNvram     (*imcNvList)[MAX_IMC];
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid(Host);
  UINT8 MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, socket);
  imcNvList     = GetImcNvList(Host, socket);
  MaxImc        = GetMaxImc ();

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    (*channelNvList)[ch].dataControl0 = ReadDdrioDataRegCh (Host, socket, ch, 0, DATACONTROL0N0_0_MCIO_DDRIO_REG);
    (*channelNvList)[ch].ddrCRClkControls = MemReadPciCfgEp (socket, ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG);
    (*channelNvList)[ch].ddrCRClkTraining = MemReadPciCfgEp (socket, ch, DDRCRCLKTRAINING_MCIO_DDRIOEXT_REG);
    (*channelNvList)[ch].ddrCRCmdTrainingCmdN = MemReadPciCfgEp (socket, ch, DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_REG);
    (*channelNvList)[ch].ddrCRCmdTrainingCmdS = MemReadPciCfgEp (socket, ch, DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXT_REG);
    (*channelNvList)[ch].ddrCRCtlTraining = MemReadPciCfgEp (socket, ch, DDRCRCMDTRAINING_CTL_MCIO_DDRIOEXT_REG);
    (*channelNvList)[ch].ddrCRCkeTraining = MemReadPciCfgEp (socket, ch, DDRCRCMDTRAINING_CKE_MCIO_DDRIOEXT_REG);
    (*channelNvList)[ch].ddrCRCmdControls3CmdN = MemReadPciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CMDN_MCIO_DDRIOEXT_REG);
    (*channelNvList)[ch].ddrCRCmdControls3Cke = MemReadPciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CKE_MCIO_DDRIOEXT_REG);
    (*channelNvList)[ch].ddrCRCmdControls3CmdS = MemReadPciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CMDS_MCIO_DDRIOEXT_REG);
    (*channelNvList)[ch].ddrCRCmdControls3Ctl = MemReadPciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CTL_MCIO_DDRIOEXT_REG);

    for (strobe = 0; strobe < MSVx4; strobe++) {
      (*channelNvList)[ch].dataControl1[strobe] = ReadDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL1N0_0_MCIO_DDRIO_REG);
      (*channelNvList)[ch].dataControl2[strobe] = ReadDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL2N0_0_MCIO_DDRIO_REG);
      (*channelNvList)[ch].dataControl4[strobe] = ReadDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL4N0_0_MCIO_DDRIO_REG);
      (*channelNvList)[ch].dataControl3[strobe] = ReadDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL3N0_0_MCIO_DDRIO_REG);
      (*channelNvList)[ch].RxDfeControl[strobe] = ReadDdrioDataRegCh (Host, socket, ch, strobe, DDRCRDATARXDFECONTROL_MCIO_DDRIO_REG);
      (*channelNvList)[ch].RxDfeCoeff[strobe]   = ReadDdrioDataRegCh (Host, socket, ch, strobe, DDRCRDATARXDFECOEFF_MCIO_DDRIO_REG);
      (*channelNvList)[ch].dataOffsetTrain[strobe] = ReadDdrioDataRegCh (Host, socket, ch, strobe, DATAOFFSETTRAINN0_0_MCIO_DDRIO_REG);
    } // strobe loop

    GetSetCmdGroupDelay (Host, socket, ch, CmdAll, GSM_READ_CSR | GSM_READ_ONLY | GSM_UPDATE_CACHE, &tempVal, &minVal, &maxVal);

    //
    // Cache registers that are not common between support chips
    //
    CacheRoundTrip (Host, socket, ch);
    CacheIoLatency (Host, socket, ch);

    //
    // Loop for each dimm and each rank
    //
    dimmNvList = GetDimmNvList(Host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }

      rankList = GetRankNvList(Host, socket, ch, dimm);
      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        //
        // Skip if no rank
        //
        if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        GetSetCtlGroupDelay (Host, socket, ch, (*rankList)[rank].ctlIndex + CtlGrp0, GSM_READ_CSR | GSM_READ_ONLY | GSM_UPDATE_CACHE, &tempVal, &minVal, &maxVal);
        GetSetClkDelay (Host, socket, ch, (*rankList)[rank].ckIndex, GSM_READ_CSR | GSM_READ_ONLY | GSM_UPDATE_CACHE, &tempVal);

        for (strobe = 0; strobe < MSVx4; strobe++) {
          if (IsStrobeNotValid(Host, strobe)) {
            continue;
          }

          for (Bit = 0; Bit < BITS_PER_NIBBLE; Bit++) {
            GetSetSenseAmpRxOffsetDdrio (Host, socket, ch, dimm, rank, strobe, Bit, DdrLevel, RxSenseAmpOffset, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, (INT16 *)&zeroValue);
          }
          GetSetSenseAmpRxOffsetDdrio (Host, socket, ch, dimm, rank, strobe, 0, DdrLevel, RxFifoDqsPBitSkew, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, (INT16 *)&zeroValue);
          GetSetSenseAmpRxOffsetDdrio (Host, socket, ch, dimm, rank, strobe, 0, DdrLevel, RxFifoDqsNBitSkew, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, (INT16 *)&zeroValue);

          (*channelNvList)[ch].dataOffsetTrain[strobe] = ReadDdrioDataRegCh (Host, socket, ch, strobe, DATAOFFSETTRAINN0_0_MCIO_DDRIO_REG);
          GetSetTxDelay (Host, socket, ch, dimm, rank, strobe, 0, DdrLevel, TxDqDelay, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, (INT16 *)&zeroValue);
          GetSetRxDelay (Host, socket, ch, dimm, rank, strobe, 0, DdrLevel, RxDqDelay, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, (INT16 *)&zeroValue);
          GetSetRxVref (Host, socket, ch, dimm, rank, strobe, 0, DdrLevel, RxVref, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, (INT16 *)&zeroValue);

        } // strobe loop
      } // rank loop
    } // dimm loop
  } // ch loop

  for (imc = 0; imc < MaxImc; imc++) {
    if (Host->var.mem.socket[socket].imcEnabled[imc] == 0) {
      continue;
    }
    (*imcNvList)[imc].dimmVrefControl1 = MemReadPciCfgMC (socket, imc, DDRCRDIMMVREFCONTROL1_MCIO_DDRIOEXT_REG);
    (*imcNvList)[imc].ddrCRCompCtl0 = MemReadPciCfgMC (socket, imc, DDRCRCOMPCTL0_MCIO_DDRIOEXT_REG);
  }

  return SUCCESS;
} // CacheDDRIO


/**
  Find register address for DDRIO strobe-based register.
  Take into account logical-to-physical strobe swizzling.

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Ch          - Channel number
  @param [in]       Strobe      - Strobe number
  @param [in]       AddressBase - Base address for ch0, rank0, strobe0

  @retval Register address for the swizzled strobe
**/
STATIC
UINT32
GetDdrIoSwizzledAddr (
  IN     PSYSHOST  Host,
  IN     UINT8     Ch,
  IN     UINT8     Strobe,
  IN     UINT32    AddressBase
  )
{
  UINT8   StrobeSwizzled;
  UINT32  SwizzledAddress;
  UINT32  AddressStrobe0;
  UINT32  AddressStrobe8;

  // Step 1: Swizzle logical strobe to physical strobe
  StrobeSwizzled = GetStrobeSwizzle (Host, Ch, Strobe);

  // Step 2: Adjust for register addresses not increasing monotonically with strobe#
  //         AddressBase is assumed to be the address for strobe 0.
  //         Address bits [15:8] increase with the strobe number from 0 to 7 but
  //         reset at strobe 8 to a value that is 10 less than that of strobe 0.
  //
  //         Determine addresses for Strobe 0 and Strobe 8. These will be used as
  //         the base addresses for the rest of the strobes.
  AddressStrobe0 = AddressBase;
  AddressStrobe8 = AddressBase & (0xffff00ff);
  AddressStrobe8 |= (AddressBase & 0x0000ff00) - (10 << 8);

  if (StrobeSwizzled < 8) {
    // Address bits[15:8] increase with strobe # from strobe 0 through 7
    SwizzledAddress = AddressStrobe0 + (StrobeSwizzled << 8);
  } else {
    // Address bits[15:8] increase with strobe # from strobe 8 through 17
    SwizzledAddress = AddressStrobe8 + ((StrobeSwizzled - 8) << 8);
  }

  return SwizzledAddress;
}

/**

  This function updates the register function block number and offset

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Ch          - Channel number
  @param [in]       LogRank     - Logical rank number in the DDRIO
  @param [in]       Strobe      - Strobe number
  @param [in]       RegOffset   - Register define

  @retval IO Address

**/
UINT32
UpdateIoRegister10nm (
  IN      PSYSHOST    Host,
  IN      UINT8       Ch,
  IN      UINT8       LogRank,
  IN      UINT8       Strobe,
  IN      UINT32      RegOffset
  )
{
  CSR_OFFSET  CsrReg;
  UINT32      RealOffset;

  //
  // Convert pseudo offset to real offset
  //
  RealOffset = MemPciCfgOffset (0, 0, RegOffset);

  CsrReg.Data = RealOffset;

  //
  // Add in offset for data group. Swizzle
  //
  CsrReg.Bits.offset = GetDdrIoSwizzledAddr (Host, Ch, Strobe, CsrReg.Data);

  //
  // Add in offset for rank
  //
  CsrReg.Bits.offset = CsrReg.Bits.offset + (LogRank * 4);

  return CsrReg.Data;
} // UpdateIoRegister10nm

/**

  This function updates the register function block number and offset

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Ch          - Channel number
  @param [in]       Strobe      - Strobe number
  @param [in]       RegOffset   - Register define

  @retval IO Address

**/
UINT32
UpdateIoRegisterCh10nm (
  IN      PSYSHOST    Host,
  IN      UINT8       Ch,
  IN      UINT8       Strobe,
  IN      UINT32      RegOffset
  )
{
  CSR_OFFSET      CsrReg;
  UINT32          RealOffset;

  //
  // Convert pseudo offset to real offset
  //
  RealOffset = MemPciCfgOffset (0, 0, RegOffset);

  CsrReg.Data = RealOffset;

  //
  // Add in offset for data group
  //
  CsrReg.Bits.offset = GetDdrIoSwizzledAddr (Host, Ch, Strobe, CsrReg.Data);

  return CsrReg.Data;
} // UpdateIoRegisterCh10nm


/**

  Reads per-rank register from DDRIO data FUB instance selected by strobe number

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Socket      - Socket number
  @param [in]       Ch          - Channel number
  @param [in]       LogRank     - Logical rank number in the DDRIO
  @param [in]       Strobe      - Strobe number
  @param [in]       Offset      - Register define

  @retval Register value

**/
UINT32
ReadDdrioDataReg (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    LogRank,
  IN      UINT8    Strobe,
  IN      UINT32   Offset
  )
{
  return MemReadPciCfgEp (Socket, Ch, UpdateIoRegister10nm (Host, Ch, LogRank, Strobe, Offset));
}

/**

  Reads per-channel register from DDRIO data FUB instance selected by strobe number

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Socket      - Socket number
  @param [in]       Ch          - Channel number
  @param [in]       Strobe      - Strobe number
  @param [in]       Offset      - Register define

  @retval Register value

**/
UINT32
ReadDdrioDataRegCh (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    Strobe,
  IN      UINT32   Offset
  )
{
  return MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm(Host, Ch, Strobe, Offset));
}

/**

  Writes per-rank register to DDRIO data FUB instance selected by strobe number

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Socket      - Socket number
  @param [in]       Ch          - Channel number
  @param [in]       LogRank     - Logical rank number in the DDRIO
  @param [in]       Strobe      - Strobe number
  @param [in]       Offset      - Register define
  @param [in]       Data        - Register value

  @retval N/A

**/
VOID
WriteDdrioDataReg (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    LogRank,
  IN      UINT8    Strobe,
  IN      UINT32   Offset,
  IN      UINT32   Data
  )
{
  MemWritePciCfgEp (Socket, Ch, UpdateIoRegister10nm(Host, Ch, LogRank, Strobe, Offset), Data);
}


/**

  Writes per-channel register to DDRIO data FUB instance selected by strobe number

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Socket      - Socket number
  @param [in]       Ch          - Channel number
  @param [in]       Strobe      - Strobe number
  @param [in]       Offset      - Register define
  @param [in]       Data        - Register value

  @retval N/A

**/
VOID
WriteDdrioDataRegCh (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    Strobe,
  IN      UINT32   Offset,
  IN      UINT32   Data
  )
{
  MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm(Host, Ch, Strobe, Offset), Data);
}


/**

  Reads DDRIO CC registers.

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Socket      - Socket number
  @param [in]       Ch          - Channel number
  @param [in]       Strobe      - Strobe number
  @param [in]       Offset      - Register define

  @retval Register value

**/
UINT32
ReadDdrioCcRegister (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    SubCh,
  IN      UINT32   Offset
  )
{

  return 0;
}


/**

  Writes DDRIO CC registers.

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Socket      - Socket number
  @param [in]       Ch          - Channel number
  @param [in]       SubCh       - Strobe number
  @param [in]       Offset      - Register define
  @param [in]       Data        - Register value

  @retval N/A

**/
VOID
WriteDdrioDataRegister (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    SubCh,
  IN      UINT32   Offset,
  IN      UINT32   Data
  )
{
  return;
}

/**

  Set the training mode

  @param[in] Host  - Pointer to sysHost
  @param[in] socket  - Socket number
  @param[in] mode  - Training mode

  @retval N/A

**/
VOID
SetTrainingModeBrs (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Dimm,
  UINT8    Rank,
  UINT8    Mode
)
{
#ifdef DDR5_SUPPORT
  UINT8                                 Ch;
  UINT8                                 SubCh;
  UINT8                                 Strobe;
  UINT8                                 ChannelLocal;
  struct channelNvram                   (*ChannelNvList) [MAX_CH];
  DATACONTROL0N0_0_MCIO_DDRIO_STRUCT    DataControl0;
  DATACONTROL2N0_0_MCIO_DDRIO_STRUCT    DataControl2;
  DDR5DATATRAINCTRL_MCIO_DDRIO_STRUCT   TempData;
  DDR5DATATRAINCTRL_MCIO_DDRIO_STRUCT   Ddr5dataControl0;
  UINT8                                 MaxStrobe;
  UINT8                                 MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  MaxStrobe = MAX_STROBE_DDR5 / 2;

  DataControl0.Data = 0;
  DataControl2.Data = 0;
  Ddr5dataControl0.Data = 0;

  switch (Mode) {
  case CHECKPOINT_MINOR_RECEIVE_ENABLE_BASIC:
    DataControl0.Bits.rltrainingmode = 1;
    DataControl0.Bits.forceodton = 1;
    DataControl2.Bits.forcerxon = 1;
    break;

  case CHECKPOINT_MINOR_RX_DQ_DQS_BASIC:
    DataControl0.Bits.rxtrainingmode = 1;
    break;

  case CHECKPOINT_MINOR_WRITE_LEVELING_BASIC:
    DataControl0.Bits.wltrainingmode = 1;
    DataControl0.Bits.txpion = 1;
    DataControl2.Bits.forcebiason = 1;
    break;

  case CHECKPOINT_MINOR_SENSE_AMP:
    DataControl0.Bits.wltrainingmode = 1;
    DataControl0.Bits.forceodton = 1;
    DataControl0.Bits.senseamptrainingmode = 1;
    DataControl2.Bits.forcebiason = 1;
    DataControl2.Bits.forcerxon = 1;
    break;

  case CHECKPOINT_MINOR_DISABLE_SENSE_AMP:
    DataControl0.Bits.wltrainingmode = 1;
    break;

  default:
    break;
  } // switch mode

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
      continue;
    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      GetChannelMapping (Ch, SubCh, &ChannelLocal);
      for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
        //
        // Set training mode and rank to all channels
        //
        // If enabling, program datacontrol2 first, if disabling program 0 first.
        if ((DataControl0.Bits.rltrainingmode == 1) ||
            (DataControl0.Bits.rxtrainingmode == 1) ||
            (DataControl0.Bits.wltrainingmode == 1) ||
            (DataControl0.Bits.senseamptrainingmode == 1)) {
          WriteDdrioDataRegCh (Host, Socket, ChannelLocal, Strobe, DATACONTROL2N0_0_MCIO_DDRIO_REG, DataControl2.Data | (*ChannelNvList)[Ch].dataControl2[Strobe]);
          WriteDdrioDataRegCh (Host, Socket, ChannelLocal, Strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, DataControl0.Data | (*ChannelNvList)[Ch].dataControl0);
        }
        else if ((GetCurrentTestType (Socket) == RcvenFineTest) && (Mode == CLEAR_MODE)) {

          DataControl0.Data = ReadDdrioDataRegCh (Host, Socket, ChannelLocal, Strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG);
          DataControl0.Bits.rltrainingmode = 0;
          WriteDdrioDataRegCh (Host, Socket, ChannelLocal, Strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, DataControl0.Data);
        }
        else {

          if (Mode == WR_LEVELING_CLEAR_MODE) {
            HandleWrLevelingClearModeTarget (Host, Socket, ChannelLocal, Strobe, (VOID *)&DataControl0);
          }

          WriteDdrioDataRegCh (Host, Socket, ChannelLocal, Strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, DataControl0.Data | (*ChannelNvList)[Ch].dataControl0);
          WriteDdrioDataRegCh (Host, Socket, ChannelLocal, Strobe, DATACONTROL2N0_0_MCIO_DDRIO_REG, DataControl2.Data | (*ChannelNvList)[Ch].dataControl2[Strobe]);

          if (Mode == WR_LEVELING_CLEAR_MODE) {
            TempData.Data = ReadDdrioDataRegCh (Host, Socket, ChannelLocal, Strobe, DDR5DATATRAINCTRL_MCIO_DDRIO_REG);
            Ddr5dataControl0.Bits.preamblepattern = TempData.Bits.preamblepattern;
            Ddr5dataControl0.Bits.wrpreamblepatsel = TempData.Bits.wrpreamblepatsel;
            WriteDdrioDataRegCh (Host, Socket, ChannelLocal, Strobe, DDR5DATATRAINCTRL_MCIO_DDRIO_REG, Ddr5dataControl0.Data);
          }
        }
      } // strobe loop
    } //subch loop
  } // ch loop
#endif // DDR5_SUPPORT
} // SetTrainingModeBrs

/**

  Set the training Mode

  @param Host  - Pointer to sysHost
  @param Socket  - Socket number
  @param Dimm  - DIMM number (0-based)
  @param Rank  - Rank number (0-based)
  @param Mode  - Training Mode

  @retval N/A

**/
VOID
SetTrainingMode(
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Dimm,
  UINT8    Rank,
  UINT8    Mode
)
{
  UINT8                                 Ch;
  UINT8                                 Strobe;
  struct channelNvram                   (*channelNvList)[MAX_CH];
  DATACONTROL0N0_0_MCIO_DDRIO_STRUCT    DataControl0;
  DATACONTROL2N0_0_MCIO_DDRIO_STRUCT    DataControl2;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, Socket);

  DataControl0.Data = 0;
  DataControl2.Data = 0;
  if (IsBrsPresent (Host, Socket)) {
    SetTrainingModeBrs (Host, Socket, Dimm, Rank, Mode);
    return;
  }
  switch (Mode) {
  case CHECKPOINT_MINOR_RECEIVE_ENABLE_BASIC:
    DataControl0.Bits.rltrainingmode = 1;
    DataControl0.Bits.forceodton = 1;
    DataControl2.Bits.forcerxon = 1;
    break;

 case CHECKPOINT_MINOR_RX_DQ_DQS_BASIC:
    DataControl0.Bits.rxtrainingmode = 1;
    break;

  case CHECKPOINT_MINOR_WRITE_LEVELING_BASIC:
    DataControl0.Bits.wltrainingmode = 1;
    DataControl0.Bits.txpion = 1;
    DataControl2.Bits.forcebiason = 1;
    break;

  case CHECKPOINT_MINOR_SENSE_AMP:
    DataControl0.Bits.forceodton = 1;
    DataControl0.Bits.senseamptrainingmode = 1;
    DataControl2.Bits.forcebiason = 1;
    DataControl2.Bits.forcerxon = 1;
    break;

  case CHECKPOINT_MINOR_DISABLE_SENSE_AMP:
    //set back to default...
    //DataControl2.Bits.forcebiason     = 1;
    //DataControl2.Bits.forcerxon       = 1;
    break;

  default:
    break;
  } // switch Mode

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*channelNvList)[Ch].enabled == 0) {
      continue;
    }

    if (CheckRank(Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
      continue;
    }

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      //
      // Skip if this is an ECC Strobe when ECC is disabled
      //
      if (IsStrobeNotValid(Host, Strobe)) {
        continue;
      }

      //
      // Set training Mode and Rank to all channels
      //
      // If enabling, program datacontrol2 first, if disabling program 0 first.
      if ((DataControl0.Bits.rltrainingmode == 1) ||
        (DataControl0.Bits.rxtrainingmode == 1) ||
        (DataControl0.Bits.wltrainingmode == 1) ||
        (DataControl0.Bits.senseamptrainingmode == 1)) {
        WriteDdrioDataRegCh(Host, Socket, Ch, Strobe, DATACONTROL2N0_0_MCIO_DDRIO_REG, DataControl2.Data | (*channelNvList)[Ch].dataControl2[Strobe]);
        WriteDdrioDataRegCh(Host, Socket, Ch, Strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, DataControl0.Data | (*channelNvList)[Ch].dataControl0);
      } else {

      if (Mode == WR_LEVELING_CLEAR_MODE) {
        HandleWrLevelingClearModeTarget(Host, Socket, Ch, Strobe, (VOID *)&DataControl0);
      }

      WriteDdrioDataRegCh(Host, Socket, Ch, Strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, DataControl0.Data | (*channelNvList)[Ch].dataControl0);
      WriteDdrioDataRegCh(Host, Socket, Ch, Strobe, DATACONTROL2N0_0_MCIO_DDRIO_REG, DataControl2.Data | (*channelNvList)[Ch].dataControl2[Strobe]);
      }
    } // Strobe loop
  } // Ch loop
} // SetTrainingMode

/**

  Initialize IO Comp settings and issue a comp cycle

  @param Host    - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/
STATIC
UINT32
ProgramIOCompValues (
  PSYSHOST  Host,
  UINT8     socket
  )
{
  UINT8                             ch;
  UINT8                             dimm;
  UINT8                             rank;
  UINT8                             logRank;
  UINT8                             rtDelay;
  UINT8                             IoLatency;
  struct ddrRank                    (*rankList)[MAX_RANK_DIMM];
  struct channelNvram               (*channelNvList)[MAX_CH];
  struct dimmNvram                  (*dimmNvList)[MAX_DIMM];
  struct RankCh                     (*rankPerCh)[MAX_RANK_CH];
  UINT8                             MaxChDdr;

  if (!(UbiosGenerationOrHsleEnabled ())) {
    return SUCCESS;
  }

  channelNvList = GetChannelNvList(Host, socket);
  MaxChDdr      = GetMaxChDdr ();

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList(Host, socket, ch);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }

      rankList = GetRankNvList(Host, socket, ch, dimm);

      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        rankPerCh = &(*channelNvList)[ch].rankPerCh;

        //
        // Get the logical rank #
        //
        logRank = GetLogicalRank(Host, socket, ch, dimm, rank);

        rtDelay = DEFAULT_EMULATION_ROUND_TRIP;
        SetRoundTrip (socket, ch, logRank, rtDelay);

        IoLatency = 0;
        SetIOLatency (socket, ch, logRank, IoLatency);

      } // rank loop
    } // dimm loop

    //
    // per Celeste Brown, for RDIMM +1 to t_cwl_adj.
    //
    if (Host->nvram.mem.dimmTypePresent == RDIMM) {
      SetCwlAdj (Host, socket, ch, 0, 1);
    }

    SetIoLatencyIoComp (Host, socket, ch, NO_SUBCH, MRC_ROUND_TRIP_IO_COMPENSATION_EMU);
  } // ch loop

  return SUCCESS;
} // ProgramIOCompValues

/**

  Issue a comp update

  @param[in] Host   - Pointer to sysHost
  @param[in] Socket - Socket number
  @param[in] McId   - iMC number

  @retval N/A

**/
VOID
DoCompUpdate (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    McId
  )
{
  DDRCRINTFPMCONFIG0_MCIO_DDRIO_STRUCT PmConfig0;
  //
  // Read DDRCRINTFPMCONFIG0
  //
  PmConfig0.Data = MemReadPciCfgMC (Socket, McId, DDRCRINTFPMCONFIG0_MCIO_DDRIO_REG);
  //
  // Set comp update bit
  //
  PmConfig0.Bits.ddrcompupdate = 1;
  //
  // Write DDRCRINTFPMCONFIG0
  //
  MemWritePciCfgMC (Socket, McId, DDRCRINTFPMCONFIG0_MCIO_DDRIO_REG, PmConfig0.Data);
} // DoCompUpdate

  /**

  Apply DQ ODT and CLK Comp workaround to Comp Engine

  @param Host   - Pointer to sysHost
  @param Socket - Socket number

  @retval N/A

  **/
VOID
CompDqOdtClkWorkAround (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  UINT8                                   McId;
  UINT8                                   MaxImc;
  DDRCRCLKCOMP_CLK_MCIO_DDRIOEXT_STRUCT   DdrCrClkComp;
  DDRCRCTLCOMP_CMDN_MCIO_DDRIOEXT_STRUCT  DdrCrCtlComp;
  DDRCRDATACOMP0_MCIO_DDRIOEXT_STRUCT     DdrCrDataComp0;
  DDRCRDATACOMP1_MCIO_DDRIOEXT_STRUCT     DdrCrDataComp1;

  MaxImc = GetMaxImc();

  //
  // Run the comp
  //
  DoComp (Host, Socket);

  for (McId = 0; McId < MaxImc; McId++) {
    if (Host->var.mem.socket[Socket].imcEnabled[McId] == 0) {
      continue;
    }

    //
    // Override DQ ODT Up with Ctl Comp values
    //
    DdrCrDataComp1.Data = MemReadPciCfgMC (Socket, McId, DDRCRDATACOMP1_MCIO_DDRIOEXT_REG);
    DdrCrCtlComp.Data = MemReadPciCfgMC (Socket, McId, DDRCRCTLCOMP_CMDN_MCIO_DDRIOEXT_REG);
    DdrCrDataComp1.Bits.rcompodtup = DdrCrCtlComp.Bits.rcompdrvup;
    MemWritePciCfgMC (Socket, McId, DDRCRDATACOMP1_MCIO_DDRIOEXT_REG, DdrCrDataComp1.Data);

    //
    // Override CLK Up Comp with Dq Ron Up values
    //
    DdrCrClkComp.Data = MemReadPciCfgMC (Socket, McId, DDRCRCLKCOMP_CLK_MCIO_DDRIOEXT_REG);
    DdrCrDataComp0.Data = MemReadPciCfgMC (Socket, McId, DDRCRDATACOMP0_MCIO_DDRIOEXT_REG);
    DdrCrClkComp.Bits.rcompdrvup = DdrCrDataComp0.Bits.rcompdrvup;
    MemWritePciCfgMC (Socket, McId, DDRCRCLKCOMP_CLK_MCIO_DDRIOEXT_REG, DdrCrClkComp.Data);
  } // McId

  //
  // Run the comp
  //
  DoComp (Host, Socket);

} // CompDqOdtClkWorkAround

/**

  Issue a ZQ cycle

  @param Host            - sysHost data structure
  @param socket          - Processor socket
  @param ch              - Channel to issue the ZQ to
  @param zqType          - Type of ZQ Calibration: Long or Short

  @retval N/A

**/
VOID
DoZQ (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     zqType
  )
{
  CpgcIssueZQ (Host, socket, ch, zqType);
} // DoZQ

VOID
GetOriginalRtlChip (
  PSYSHOST    Host,                             // Pointer to the system Host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank,                             // Rank number (0-based)
  UINT8       RtlInitializationMode,            // RTL Initialization Mode: RTL_INITIALIZE_ONCE or RTL_RE_INITIALIZE
  UINT8       *roundTripLatency                 // Pointer to the round trip latency
  )
{
  //
  // Get original RTL
  //
  if (RtlInitializationMode == RTL_INITIALIZE_ONCE) {
    GetRoundTrip (socket, ch, GetLogicalRank (Host, socket, ch, dimm, rank), GSM_READ_CSR, roundTripLatency);
  }
} // GetOriginalRtlChip

/**

  Set the IO latency register value in the register and cache

  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number (0 >= Ch < MaxChDdr)
  @param[in]  Rank            - Rank number (0 >= Rank < MAX_RANK_CH)
  @param[in]  IoLatencyValue  - IO Latency value to set

  @retval EFI_SUCCESS if value found
          EFI_INVALID_PARAMETER otherwise
          EFI_OUT_OF_RESOURCES - indicates to the caller that the IoLatencyValue is lower than the min IO Latency allowed.

**/
EFI_STATUS
SetIOLatency (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    Rank,
  IN    UINT8    IOLatencyValue
  )
{
  EFI_STATUS                                Status = EFI_SUCCESS;
  PSYSHOST                                  Host;
  DDRCRINTFIOLATENCY0_CH0_MCIO_DDRIO_STRUCT IoLatency;
  UINT32                                    IoLatency0Ch[] = {DDRCRINTFIOLATENCY0_CH0_MCIO_DDRIO_REG,
                                                              DDRCRINTFIOLATENCY0_CH1_MCIO_DDRIO_REG,
                                                              DDRCRINTFIOLATENCY0_CH2_MCIO_DDRIO_REG
                                                             };
  struct channelNvram                       (*ChannelNvList)[MAX_CH];
  struct RankCh                             (*RankPerCh)[MAX_RANK_CH];
  UINT8                                     Index;
  UINT8                                     NumChPerMc;
  UINT8                                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  NumChPerMc = GetNumChannelPerMc ();

  if ((Ch >= MaxChDdr) || (Rank >= MAX_RANK_CH)) {
    //
    // Invalid Ch or Rank specified
    //
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_88);
    return EFI_INVALID_PARAMETER;
  }


  //
  // Check input: IOLatencyValue must be within range and must be an odd value
  //
  if (IOLatencyValue < MEM_CHIP_POLICY_VALUE (Host, minIoLatency)) {
    // Min value exceeded so set IoLatencyValue to its minimum value and set status to inform caller.
    // This is not necessarily a failure so the caller must decide what action to take.
    IOLatencyValue = MEM_CHIP_POLICY_VALUE (Host, minIoLatency);
    if (IOLatencyValue % 2 == 0) {
      // This is an even value so use MIN + 1 to get the minimun odd value
      IOLatencyValue += 1;
    }
    Status = EFI_OUT_OF_RESOURCES;
  }
  if (IOLatencyValue % 2 == 0) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_144);
  }
  ChannelNvList = GetChannelNvList (Host, Socket);
  RankPerCh = &(*ChannelNvList)[Ch].rankPerCh;
  Index = Ch % NumChPerMc;

  IoLatency.Data = MemReadPciCfgEp (Socket, Ch, IoLatency0Ch[Index]);
  IoLatency.Data &= ~(0xF << (Rank * 4));
  IoLatency.Data |= IOLatencyValue << (Rank * 4);
  MemWritePciCfgEp (Socket, Ch, IoLatency0Ch[Index], IoLatency.Data);

#ifdef DDR5_SUPPORT
  (*RankPerCh)[Rank].IOLatency[SUB_CH_A] = IOLatencyValue;
#else
  (*RankPerCh)[Rank].IOLatency = IOLatencyValue;
#endif // #ifdef DDR5_SUPPORT

  return Status;
} // SetIOLatency

/**
  Get the IO latency value for a single rank

  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number (0 >= Ch < MaxChDdr)
  @param[in]  Rank            - Rank number (0 >= Rank < MAX_RANK_CH)
  @param[in]  Mode            - Bit field of access modes
  @param[out] IoLatencyValue  - Pointer to caller's IO Latency value

  @retval EFI_SUCCESS if value found
          EFI_INVALID_PARAMETER otherwise

**/
EFI_STATUS
GetIoLatency (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    Rank,
  IN    UINT8    Mode,
  OUT   UINT8    *IoLatencyValue
  )
{
  PSYSHOST                                  Host;
  UINT8                                     Index;
  UINT8                                     NumChPerMc;
  DDRCRINTFIOLATENCY0_CH0_MCIO_DDRIO_STRUCT IoLatency0;
  UINT32                                    IoLatency0Ch[] = {DDRCRINTFIOLATENCY0_CH0_MCIO_DDRIO_REG,
                                                              DDRCRINTFIOLATENCY0_CH1_MCIO_DDRIO_REG,
                                                              DDRCRINTFIOLATENCY0_CH2_MCIO_DDRIO_REG
                                                             };
  struct channelNvram                       (*ChannelNvList)[MAX_CH];
  struct RankCh                             (*RankPerCh)[MAX_RANK_CH];
  UINT8                                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  NumChPerMc = GetNumChannelPerMc ();

  if ((Ch >= MaxChDdr) || (Rank >= MAX_RANK_CH)) {
    //
    // Invalid Ch or Rank specified
    //
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_89);
    return EFI_INVALID_PARAMETER;
  }

  if ((Mode & GSM_READ_CSR) != 0) {
    //
    // Read from the register
    //
    Index = Ch % NumChPerMc;

    IoLatency0.Data  = MemReadPciCfgEp (Socket, Ch, IoLatency0Ch[Index]);
    *IoLatencyValue = (UINT8)((IoLatency0.Data >> (Rank * 4)) & 0xF);

  } else {
    //
    // Read from the cache
    //
    ChannelNvList = GetChannelNvList (Host, Socket);
    RankPerCh = &(*ChannelNvList)[Ch].rankPerCh;

#ifdef DDR5_SUPPORT
    *IoLatencyValue = (*RankPerCh)[Rank].IOLatency[SUB_CH_A];
#else
    *IoLatencyValue = (*RankPerCh)[Rank].IOLatency;
#endif // #ifdef DDR5_SUPPORT
  }

  return EFI_SUCCESS;
} // GetIoLatency

/**

  Set the round trip register value in the register and cache

  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number (0-based)
  @param[in]  Rank            - Rank number (0-based)
  @param[in]  RoundTripValue  - New Round Trip Latency value

  @retval EFI_SUCCESS if value found
          EFI_INVALID_PARAMETER if channel or rank is out of range
          EFI_OUT_OF_RESOURCES - indicates to the caller that the input RoundTripValue exceeds the max Round Trip Latency.
          !EFI_SUCCESS if other errors were encountered

**/
EFI_STATUS
SetRoundTrip (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    Rank,
  IN    UINT8    RoundTripValue
  )
{
  EFI_STATUS                                TemporaryStatus;
  EFI_STATUS                                Status = EFI_SUCCESS;
  PSYSHOST                                  Host;
  UINT8                                     MemoryController;
  UINT8                                     MemoryControllerChannel;
  UINT8                                     NumChPerMc;
  DDRCRINTFROUNDTRIP0_CH0_MCIO_DDRIO_STRUCT RoundTrip0;
  DDRCRINTFROUNDTRIP1_CH0_MCIO_DDRIO_STRUCT RoundTrip1;
  UINT8                                     RoundTripLocal[DIMM_ROUND_TRIP_RANKS_PER_REG];
  UINT32                                    RoundTripRegOffset;
  struct channelNvram                       (*ChannelNvList)[MAX_CH];
  struct RankCh                             (*RankPerCh)[MAX_RANK_CH];
  UINT8                                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  NumChPerMc = GetNumChannelPerMc ();

  if ((Ch >= MaxChDdr) || (Rank >= MAX_RANK_CH)) {
    // Invalid Ch or Rank specified
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_90);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check input: RoundTripValue must be within range and must be an even value
  //
  if (RoundTripValue > MRC_ROUND_TRIP_MAX_VALUE) {
    // Max value exceeded so set RoundTripValue to its maximum value and set status to inform caller.
    // This is not necessarily a failure so the caller must decide what action to take.
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "Round Trip delay %d exceeds limit %d\n", RoundTripValue, MRC_ROUND_TRIP_MAX_VALUE);
    RoundTripValue = MRC_ROUND_TRIP_MAX_VALUE;
    if (RoundTripValue % 2 == 1) {
      // This is an ODD value so use MAX - 1 to get the maximum even value
      RoundTripValue -= 1;
    }
    Status = EFI_OUT_OF_RESOURCES;
  }
  if (RoundTripValue % 2 == 1) {
    // This is a critical failure as the caller has sent in an odd value.  Round Trip must be even.
    RC_FATAL_ERROR (RoundTripValue % 2 == 0, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_142);
  }

  ChannelNvList = GetChannelNvList (Host, Socket);
  RankPerCh = &(*ChannelNvList)[Ch].rankPerCh;
  MemoryController = Ch / NumChPerMc;
  MemoryControllerChannel = Ch % NumChPerMc;

  TemporaryStatus = GetRoundTripRegOffset (MemoryControllerChannel, Rank / MAX_RANK_DIMM, &RoundTripRegOffset);
  if (!EFI_ERROR (Status)) {
    Status = TemporaryStatus;
  }

  if (Rank < DIMM_ROUND_TRIP_RANKS_PER_REG) {
    RoundTrip0.Data  = MemReadPciCfgMC (Socket, MemoryController, RoundTripRegOffset);
    RoundTripLocal[0] = (UINT8)RoundTrip0.Bits.rt_rank0;
    RoundTripLocal[1] = (UINT8)RoundTrip0.Bits.rt_rank1;
    RoundTripLocal[2] = (UINT8)RoundTrip0.Bits.rt_rank2;
    RoundTripLocal[3] = (UINT8)RoundTrip0.Bits.rt_rank3;
  } else {
    RoundTrip1.Data  = MemReadPciCfgMC (Socket, MemoryController, RoundTripRegOffset);
    RoundTripLocal[0] = (UINT8)RoundTrip1.Bits.rt_rank4;
    RoundTripLocal[1] = (UINT8)RoundTrip1.Bits.rt_rank5;
    RoundTripLocal[2] = (UINT8)RoundTrip1.Bits.rt_rank6;
    RoundTripLocal[3] = (UINT8)RoundTrip1.Bits.rt_rank7;
  }

  //
  // Set the new value for the target rank and update the cache
  //
  RoundTripLocal[Rank % DIMM_ROUND_TRIP_RANKS_PER_REG] = RoundTripValue;
  (*RankPerCh)[Rank].Roundtrip = RoundTripValue;

  if (Rank < DIMM_ROUND_TRIP_RANKS_PER_REG) {
    RoundTrip0.Bits.rt_rank0 = RoundTripLocal[0];
    RoundTrip0.Bits.rt_rank1 = RoundTripLocal[1];
    RoundTrip0.Bits.rt_rank2 = RoundTripLocal[2];
    RoundTrip0.Bits.rt_rank3 = RoundTripLocal[3];
    MemWritePciCfgMC (Socket, MemoryController, RoundTripRegOffset, RoundTrip0.Data);
  } else {
    RoundTrip1.Bits.rt_rank4 = RoundTripLocal[0];
    RoundTrip1.Bits.rt_rank5 = RoundTripLocal[1];
    RoundTrip1.Bits.rt_rank6 = RoundTripLocal[2];
    RoundTrip1.Bits.rt_rank7 = RoundTripLocal[3];
    MemWritePciCfgMC (Socket, MemoryController, RoundTripRegOffset, RoundTrip1.Data);
  }
  //
  // Set any additional round trip settings in the MC
  //
  SetRoundTripMc (Socket, Ch, Rank, RoundTripValue);

  return Status;
} // SetRoundTrip

/**
  Get the round trip value

  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number (0-based)
  @param[in]  Rank            - Rank number (0-based)
  @param[in]  Mode            - Bit field of access modes
  @param[out] RoundTripValue  - Pointer to caller's Round Trip Latency value

  @retval EFI_SUCCESS if value found
          EFI_INVALID_PARAMETER if channel or rank is out of range
          !EFI_SUCCESS if other errors were encountered

**/
EFI_STATUS
GetRoundTrip (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    Rank,
  IN    UINT8    Mode,
  OUT   UINT8    *RoundTripValue
  )
{
  PSYSHOST                                  Host;
  UINT8                                     MemoryController;
  UINT8                                     MemoryControllerChannel;
  UINT8                                     NumChPerMc;
  DDRCRINTFROUNDTRIP0_CH0_MCIO_DDRIO_STRUCT RoundTrip0;
  DDRCRINTFROUNDTRIP1_CH0_MCIO_DDRIO_STRUCT RoundTrip1;
  UINT8                                     RoundTripLocal[DIMM_ROUND_TRIP_RANKS_PER_REG];
  UINT32                                    RoundTripRegOffset;
  EFI_STATUS                                Status = EFI_SUCCESS;
  struct channelNvram                       (*ChannelNvList)[MAX_CH];
  struct RankCh                             (*RankPerCh)[MAX_RANK_CH];
  UINT8                                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  NumChPerMc = GetNumChannelPerMc ();

  if ((Ch >= MaxChDdr) || (Rank >= MAX_RANK_CH)) {
    //
    // Invalid Ch or Rank specified
    //
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_91);
    return EFI_INVALID_PARAMETER;
  }

  if ((Mode & GSM_READ_CSR) != 0) {
    //
    // Read from the register
    //
    MemoryController = Ch / NumChPerMc;
    MemoryControllerChannel = Ch % NumChPerMc;

    Status = GetRoundTripRegOffset (MemoryControllerChannel, Rank / MAX_RANK_DIMM, &RoundTripRegOffset);

    if (Rank < DIMM_ROUND_TRIP_RANKS_PER_REG) {
      RoundTrip0.Data  = MemReadPciCfgMC (Socket, MemoryController, RoundTripRegOffset);
      RoundTripLocal[0] = (UINT8)RoundTrip0.Bits.rt_rank0;
      RoundTripLocal[1] = (UINT8)RoundTrip0.Bits.rt_rank1;
      RoundTripLocal[2] = (UINT8)RoundTrip0.Bits.rt_rank2;
      RoundTripLocal[3] = (UINT8)RoundTrip0.Bits.rt_rank3;
    } else {
      RoundTrip1.Data  = MemReadPciCfgMC (Socket, MemoryController, RoundTripRegOffset);
      RoundTripLocal[0] = (UINT8)RoundTrip1.Bits.rt_rank4;
      RoundTripLocal[1] = (UINT8)RoundTrip1.Bits.rt_rank5;
      RoundTripLocal[2] = (UINT8)RoundTrip1.Bits.rt_rank6;
      RoundTripLocal[3] = (UINT8)RoundTrip1.Bits.rt_rank7;
    }

    *RoundTripValue = RoundTripLocal[Rank % DIMM_ROUND_TRIP_RANKS_PER_REG];

  } else {
    //
    // Read from the cache
    //
    ChannelNvList = GetChannelNvList (Host, Socket);
    RankPerCh = &(*ChannelNvList)[Ch].rankPerCh;
    *RoundTripValue = (*RankPerCh)[Rank].Roundtrip;
  }

  return Status;
} // GetRoundTrip

/**

  Restore roundtrip timings for fast boot or resume paths

  @param[in]  Host    - Pointer to sysHost
  @param[in]  Socket  - Processor to initialize
  @param[in]  Ch      - Channel number

  @retval EFI_SUCCESS   Round trip value was successfully restored
  @retval !EFI_SUCCESS  Round trip value was not successfully restored

**/
EFI_STATUS
RestoreRoundTrip (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch
  )
{
  UINT8                                     MemoryController;
  UINT8                                     MemoryControllerChannel;
  UINT8                                     RankIndex;
  UINT8                                     RoundTrip[DIMM_ROUND_TRIP_RANKS];
  UINT8                                     NumChPerMc;
  DDRCRINTFROUNDTRIP0_CH0_MCIO_DDRIO_STRUCT RoundTrip0;
  DDRCRINTFROUNDTRIP1_CH0_MCIO_DDRIO_STRUCT RoundTrip1;
  UINT32                                    RoundTripRegOffset;
  EFI_STATUS                                Status;
  EFI_STATUS                                TemporaryStatus;
  struct channelNvram                       (*ChannelNvList)[MAX_CH];
  UINT8                                     MaxChDdr;

  MaxChDdr   = GetMaxChDdr ();
  NumChPerMc = GetNumChannelPerMc ();

  if (Ch >= MaxChDdr) {
    // invalid ch specified
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_124);
    return EFI_INVALID_PARAMETER;
  }

  ChannelNvList = GetChannelNvList (Host, Socket);
  MemoryController = Ch / NumChPerMc;
  MemoryControllerChannel = Ch % NumChPerMc;

  for (RankIndex = 0; RankIndex < DIMM_ROUND_TRIP_RANKS; RankIndex++) {
    GetRoundTrip (Socket, Ch, RankIndex, GSM_READ_ONLY, &RoundTrip[RankIndex]);
  }

  Status = GetRoundTripRegOffset (MemoryControllerChannel, 0, &RoundTripRegOffset);
  RoundTrip0.Data = MemReadPciCfgMC (Socket, MemoryController, RoundTripRegOffset);
  RoundTrip0.Bits.rt_rank0 = RoundTrip[0];
  RoundTrip0.Bits.rt_rank1 = RoundTrip[1];
  RoundTrip0.Bits.rt_rank2 = RoundTrip[2];
  RoundTrip0.Bits.rt_rank3 = RoundTrip[3];
  MemWritePciCfgMC (Socket, MemoryController, RoundTripRegOffset, RoundTrip0.Data);

  TemporaryStatus = GetRoundTripRegOffset (MemoryControllerChannel, 1, &RoundTripRegOffset);
  if (!EFI_ERROR (Status)) {
    Status = TemporaryStatus;
  }
  RoundTrip1.Data = MemReadPciCfgMC (Socket, MemoryController, RoundTripRegOffset);
  RoundTrip1.Bits.rt_rank4 = RoundTrip[4];
  RoundTrip1.Bits.rt_rank5 = RoundTrip[5];
  RoundTrip1.Bits.rt_rank6 = RoundTrip[6];
  RoundTrip1.Bits.rt_rank7 = RoundTrip[7];
  MemWritePciCfgMC (Socket, MemoryController, RoundTripRegOffset, RoundTrip1.Data);

  return Status;
}

#ifdef DDR5_SUPPORT
/**

  Get the round trip register value for DDR5

  @param[in]  socket          - Socket number
  @param[in]  ch              - Channel number (0-based)
  @param[in]  subch           - Sub Channel number (0-based)
  @param[in]  rank            - Rank number (0-based)
  @param[in]  Mode            - Bit field specifying access mode
  @param[out] RoundTripValue  - Pointer to caller's Round Trip Latency value

  @retval EFI_SUCCESS if value found
          EFI_INVALID_PARAMETER if channel or rank is out of range
          !EFI_SUCCESS if other errors were encountered

**/
EFI_STATUS
GetRoundTripDdr5 (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    SubCh,
  IN    UINT8    Rank,
  IN    UINT8    Mode,
  OUT   UINT8    *RoundTripValue
  )
{
  PSYSHOST                                  Host;
  UINT8                                     MemoryController;
  UINT8                                     MemoryControllerChannel;
  UINT8                                     NumChPerMc;
  DDRCRINTFROUNDTRIP0_CH0_MCIO_DDRIO_STRUCT RoundTrip0;
  DDRCRINTFROUNDTRIP1_CH0_MCIO_DDRIO_STRUCT RoundTrip1;
  UINT8                                     RoundTripLocal[DIMM_ROUND_TRIP_RANKS_PER_REG];
  UINT32                                    RoundTripRegOffset;
  EFI_STATUS                                Status = EFI_SUCCESS;
  UINT8                                     ChannelLocal = 0;
  struct channelNvram                       (*ChannelNvList)[MAX_CH];
  struct RankCh                             (*RankPerCh)[MAX_RANK_CH];
  UINT8                                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  NumChPerMc = GetNumChannelPerMc ();

  ChannelLocal = Ch;

  GetChannelMapping (Ch, SubCh, &ChannelLocal);

  if ((ChannelLocal >= MaxChDdr) || (Rank >= MAX_RANK_CH)) {
    // Invalid Ch or Rank specified
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_91);
    return EFI_INVALID_PARAMETER;
  }
  Mode = Mode | GSM_READ_CSR;
  if ((Mode & GSM_READ_CSR) != 0) {
    //
    // Read from the register
    //
    MemoryController = ChannelLocal / NumChPerMc;
    MemoryControllerChannel = ChannelLocal % NumChPerMc;

    Status = GetRoundTripRegOffset (MemoryControllerChannel, Rank / MAX_RANK_DIMM, &RoundTripRegOffset);

    if (Rank < DIMM_ROUND_TRIP_RANKS_PER_REG) {
      RoundTrip0.Data  = MemReadPciCfgMC (Socket, MemoryController, RoundTripRegOffset);
      RoundTripLocal[0] = (UINT8)RoundTrip0.Bits.rt_rank0;
      RoundTripLocal[1] = (UINT8)RoundTrip0.Bits.rt_rank1;
      RoundTripLocal[2] = (UINT8)RoundTrip0.Bits.rt_rank2;
      RoundTripLocal[3] = (UINT8)RoundTrip0.Bits.rt_rank3;
    } else {
      RoundTrip1.Data  = MemReadPciCfgMC (Socket, MemoryController, RoundTripRegOffset);
      RoundTripLocal[0] = (UINT8)RoundTrip1.Bits.rt_rank4;
      RoundTripLocal[1] = (UINT8)RoundTrip1.Bits.rt_rank5;
      RoundTripLocal[2] = (UINT8)RoundTrip1.Bits.rt_rank6;
      RoundTripLocal[3] = (UINT8)RoundTrip1.Bits.rt_rank7;
    }

    *RoundTripValue = RoundTripLocal[Rank % DIMM_ROUND_TRIP_RANKS_PER_REG];

  } else {
    //
    // Read from the cache
    //
    ChannelNvList = GetChannelNvList (Host, Socket);
    RankPerCh = &(*ChannelNvList)[ChannelLocal].rankPerCh;
    *RoundTripValue = (*RankPerCh)[Rank].Roundtrip;
  }

  return Status;
}

/**

  Set the round trip register value for DDR5

  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number (0-based)
  @param[in]  Subch           - Sub Channel number (0-based)
  @param[in]  Rank            - Rank number (0-based)
  @param[in]  RoundTripValue  - New Round Trip Latency value

  @retval EFI_SUCCESS if value found
          EFI_INVALID_PARAMETER if channel or rank is out of range
          !EFI_SUCCESS if other errors were encountered

**/
EFI_STATUS
SetRoundTripDdr5 (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    SubCh,
  IN    UINT8    Rank,
  IN    UINT8    RoundTripValue
  )
{
  EFI_STATUS                                Status;
  PSYSHOST                                  Host;
  UINT8                                     MemoryController;
  UINT8                                     MemoryControllerChannel;
  UINT8                                     NumChPerMc;
  DDRCRINTFROUNDTRIP0_CH0_MCIO_DDRIO_STRUCT RoundTrip0;
  DDRCRINTFROUNDTRIP1_CH0_MCIO_DDRIO_STRUCT RoundTrip1;
  UINT8                                     RoundTripLocal[DIMM_ROUND_TRIP_RANKS_PER_REG];
  UINT32                                    RoundTripRegOffset;
  UINT8                                     ChannelLocal = 0;
  struct channelNvram                       (*ChannelNvList)[MAX_CH];
  struct RankCh                             (*RankPerCh)[MAX_RANK_CH];
  UINT8                                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  NumChPerMc = GetNumChannelPerMc ();

  ChannelNvList = GetChannelNvList (Host, Socket);
  ChannelLocal = Ch;

  GetChannelMapping (Ch, SubCh, &ChannelLocal);

  if ((ChannelLocal >= MaxChDdr) || (Rank >= MAX_RANK_CH)) {
    // Invalid Ch or Rank specified
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_90);
    return EFI_INVALID_PARAMETER;
  }

  RankPerCh = &(*ChannelNvList)[ChannelLocal].rankPerCh;
  MemoryController = ChannelLocal / NumChPerMc;
  MemoryControllerChannel = ChannelLocal % NumChPerMc;
  Status = GetRoundTripRegOffset (MemoryControllerChannel, Rank / MAX_RANK_DIMM, &RoundTripRegOffset);

  if (Rank < DIMM_ROUND_TRIP_RANKS_PER_REG) {
    RoundTrip0.Data  = MemReadPciCfgMC (Socket, MemoryController, RoundTripRegOffset);
    RoundTripLocal[0] = (UINT8)RoundTrip0.Bits.rt_rank0;
    RoundTripLocal[1] = (UINT8)RoundTrip0.Bits.rt_rank1;
    RoundTripLocal[2] = (UINT8)RoundTrip0.Bits.rt_rank2;
    RoundTripLocal[3] = (UINT8)RoundTrip0.Bits.rt_rank3;
  } else {
    RoundTrip1.Data  = MemReadPciCfgMC (Socket, MemoryController, RoundTripRegOffset);
    RoundTripLocal[0] = (UINT8)RoundTrip1.Bits.rt_rank4;
    RoundTripLocal[1] = (UINT8)RoundTrip1.Bits.rt_rank5;
    RoundTripLocal[2] = (UINT8)RoundTrip1.Bits.rt_rank6;
    RoundTripLocal[3] = (UINT8)RoundTrip1.Bits.rt_rank7;
  }

  //
  // Set the new value for the target rank and update the cache
  //
  RoundTripLocal[Rank % DIMM_ROUND_TRIP_RANKS_PER_REG] = RoundTripValue;
  (*RankPerCh)[Rank].Roundtrip = RoundTripValue;

  if (Rank < DIMM_ROUND_TRIP_RANKS_PER_REG) {
    RoundTrip0.Bits.rt_rank0 = RoundTripLocal[0];
    RoundTrip0.Bits.rt_rank1 = RoundTripLocal[1];
    RoundTrip0.Bits.rt_rank2 = RoundTripLocal[2];
    RoundTrip0.Bits.rt_rank3 = RoundTripLocal[3];
    MemWritePciCfgMC (Socket, MemoryController, RoundTripRegOffset, RoundTrip0.Data);
  } else {
    RoundTrip1.Bits.rt_rank4 = RoundTripLocal[0];
    RoundTrip1.Bits.rt_rank5 = RoundTripLocal[1];
    RoundTrip1.Bits.rt_rank6 = RoundTripLocal[2];
    RoundTrip1.Bits.rt_rank7 = RoundTripLocal[3];
    MemWritePciCfgMC (Socket, MemoryController, RoundTripRegOffset, RoundTrip1.Data);
  }

  return Status;
} // SetRoundTripDdr5

/**

  Set the round trip register value for DDR5 for all ranks

  @param[in]  Socket          Socket number
  @param[in]  Ch              Channel number (0-based)
  @param[in]  Subch           Sub Channel number (0-based)
  @param[in]  RoundTripValue  New Round Trip Latency value

  @retval MRC_STATUS_SUCCESS  When this function completes execution

**/
MRC_STATUS
SetRoundTripAll (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    SubCh,
  IN    UINT8    RoundTripValue
  )
{
  UINT8   Rank;

  for (Rank = 0; Rank < MAX_RANK_CH; Rank++) {
    SetRoundTripDdr5 (Socket, Ch, SubCh, Rank, RoundTripValue);
  }


  return MRC_STATUS_SUCCESS;
} // SetRoundTripAll

/**

  Set the IO latency register value in the register and cache

  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number (0 >= Ch < MaxChDdr)
  @param[in]  Subch           - Sub Channel number (0-based)
  @param[in]  Rank            - Rank number (0 >= Rank < MAX_RANK_CH)
  @param[in]  IoLatencyValue  - IO Latency value to set

  @retval EFI_SUCCESS if value found
          EFI_INVALID_PARAMETER otherwise

**/
EFI_STATUS
SetIoLatencyDdr5 (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    SubCh,
  IN    UINT8    Rank,
  IN    UINT8    IOLatencyValue
  )
{
  PSYSHOST                                  Host;
  DDRCRINTFIOLATENCY0_CH0_MCIO_DDRIO_STRUCT IoLatency;
  UINT8                                     ChannelLocal = 0;
  UINT32                                    IoLatency0Ch[] = {DDRCRINTFIOLATENCY0_CH0_MCIO_DDRIO_REG,
                                                              DDRCRINTFIOLATENCY0_CH1_MCIO_DDRIO_REG,
                                                              DDRCRINTFIOLATENCY0_CH2_MCIO_DDRIO_REG
                                                             };
  struct channelNvram                       (*ChannelNvList)[MAX_CH];
  struct RankCh                             (*RankPerCh)[MAX_RANK_CH];
  UINT8                                     Index;
  UINT8                                     NumChPerMc;
  UINT8                                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  NumChPerMc = GetNumChannelPerMc ();

  ChannelLocal = Ch;

  GetChannelMapping (Ch, SubCh, &ChannelLocal);

  if ((ChannelLocal >= MaxChDdr) || (Rank >= MAX_RANK_CH)) {
    //
    // Invalid Ch or Rank specified
    //
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_88);
    return EFI_INVALID_PARAMETER;
  }

  ChannelNvList = GetChannelNvList (Host, Socket);
  RankPerCh     = &(*ChannelNvList)[ChannelLocal].rankPerCh;
  Index         = ChannelLocal % NumChPerMc;

  IoLatency.Data = MemReadPciCfgEp (Socket, ChannelLocal, IoLatency0Ch[Index]);
  IoLatency.Data &= ~(0xF << (Rank * 4));
  IoLatency.Data |= IOLatencyValue << (Rank * 4);
  MemWritePciCfgEp (Socket, ChannelLocal, IoLatency0Ch[Index], IoLatency.Data);

  return EFI_SUCCESS;
} // SetIoLatencyDdr5

/**
  Get the IO latency value for a single rank

  @param[in]  Socket          - Socket numer
  @param[in]  Ch              - Channel number (0-based)
  @param[in]  Subch           - Sub Channel number (0-based)
  @param[in]  Rank            - Rank number (0-based)
  @param[in]  Mode            - Bit field of access modes
  @param[out] IoLatencyValue  - Pointer to caller's IO Latency value

  @retval EFI_SUCCESS if value found
          EFI_INVALID_PARAMETER otherwise

**/
EFI_STATUS
GetIoLatencyDdr5 (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    SubCh,
  IN    UINT8    Rank,
  IN    UINT8    Mode,
  OUT   UINT8    *IoLatencyValue
  )
{
  PSYSHOST                                  Host;
  UINT8                                     Index;
  UINT8                                     ChannelLocal = 0;
  UINT8                                     NumChPerMc;
  DDRCRINTFIOLATENCY0_CH0_MCIO_DDRIO_STRUCT IoLatency0;
  UINT32                                    IoLatency0Ch[] = {DDRCRINTFIOLATENCY0_CH0_MCIO_DDRIO_REG,
                                                              DDRCRINTFIOLATENCY0_CH1_MCIO_DDRIO_REG,
                                                              DDRCRINTFIOLATENCY0_CH2_MCIO_DDRIO_REG
                                                             };
  struct channelNvram                       (*ChannelNvList)[MAX_CH];
  struct RankCh                             (*RankPerCh)[MAX_RANK_CH];
  UINT8                                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  NumChPerMc = GetNumChannelPerMc ();

  ChannelLocal = Ch;
  Mode = Mode | GSM_READ_CSR;
  GetChannelMapping(Ch, SubCh, &ChannelLocal);

  if ((ChannelLocal >= MaxChDdr) || (Rank >= MAX_RANK_CH)) {
    //
    // Invalid Ch or Rank specified
    //
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_89);
    return EFI_INVALID_PARAMETER;
  }

  if ((Mode & GSM_READ_CSR) != 0) {
    //
    // Read from the register
    //
    Index = ChannelLocal % NumChPerMc;

    IoLatency0.Data  = MemReadPciCfgEp (Socket, ChannelLocal, IoLatency0Ch[Index]);
    *IoLatencyValue = (UINT8)((IoLatency0.Data >> (Rank * 4)) & 0xF);

  } else {
    //
    // Read from the cache
    //
    ChannelNvList = GetChannelNvList (Host, Socket);
    RankPerCh = &(*ChannelNvList)[ChannelLocal].rankPerCh;

#ifdef DDR5_SUPPORT
    *IoLatencyValue = (*RankPerCh)[Rank].IOLatency[SUB_CH_A];
#else
    *IoLatencyValue = (*RankPerCh)[Rank].IOLatency;
#endif // #ifdef DDR5_SUPPORT
  }

  return EFI_SUCCESS;
} // GetIoLatencyDdr5

/**
  Find the maximal round trip latency of all ranks in a channel, adjust each rank's
  round trip latency to this common channel level value.

  @param[in    ]  Socket            Socket number
  @param[in out]  RoundTripLatency  Input value is per-rank RTL, output value is the adjusted per-channel RTL value.
  @param[in out]  IoLatency         Input value is per-rank IOL, output value is the adjusted per-channel IOL value.

  @retval None
*/
VOID
EFIAPI
AdjustChannelRoundTrip (
 IN     UINT8   Socket,
 IN OUT UINT8   (*RoundTripLatency)[MAX_CH][SUB_CH][MAX_RANK_CH],
 IN OUT UINT8   (*IoLatency)[MAX_CH][SUB_CH][MAX_RANK_CH]
 )
{
  return;
}


/**

  Set the training mode for DDR5
  BRS uses SetTrainingModeBrs

  @param socket  - Socket number
  @param mode   - Training mode

  @retval N/A

**/
VOID
EFIAPI
SetTrainingModeDdr5 (
  IN      UINT8           Socket,
  IN      TRAINING_MODE   Mode
  )
{
  //
  // BRS uses SetTrainingModeBrs
  //
} // SetTrainingModeDdr5

/**

  Enable/Disable Write Level training mode in the DDRIO.

  @param[in] Socket  - Socket number
  @param[in] Enable  - TRUE to enable, FALSE to disable

  retval N/A

**/
VOID
EFIAPI
SetWlTrainingMode (
  IN UINT8    Socket,
  IN BOOLEAN  Enable
  )
{
  return;
}

/**

Training Mode Selection. It supports CS/CA CLK training.

@param[in]  MEMFLOWS       - Training Step
@param[out] TrainingMode   - Training Mode. 1: In parallel. 0: In serial.

@retval MRC_STATUS_SUCCESS  - Select training mode Successfully
        MRC_STATUS_FAILURE  - Fail to set training mode

**/
MRC_STATUS
EFIAPI
TrainingModeSelect (
  IN     MEMFLOWS   Trainingstep,
     OUT UINT8      *TrainingMode
  )
{
  if ((Trainingstep == EarlyCsClkTraining) || (Trainingstep == CaClkTraining)) {
    *TrainingMode = RUN_IN_SERIAL;
    return MRC_STATUS_SUCCESS;
  } else {
    *TrainingMode = TRAINING_MODE_NOT_SUPPORT;
    return MRC_STATUS_FAILURE;
  }
}

/**

Set the training mode to latch the training result.

@param[in] Host    - Pointer to sysHost
@param[in] Socket  - Socket number
@param[in] Mode    - Training mode

@retval N/A

**/
VOID
SetTrainingModeLatchResult (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Mode
  )
{

  UINT8                                 Strobe;
  UINT8                                 ChannelLocal;
  UINT8                                 Dimm = 0;
  UINT8                                 Rank = 0;
  UINT8                                 RankEnabled = 0;
  UINT8                                 SubChannel = 0;
  UINT8                                 Ch = 0;
  struct channelNvram                   (*ChannelNvList)[MAX_CH];
  struct dimmNvram                      (*DimmNvList)[MAX_DIMM];
  DATACONTROL0N0_0_MCIO_DDRIO_STRUCT    DataControl0;
  DATACONTROL2N0_0_MCIO_DDRIO_STRUCT    DataControl2;
  UINT8                                 SubChMSVx4 = GetSubChMaxStrobeValid(Host);
  UINT8                                 MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList(Host, Socket);

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

      RankEnabled = 0;

      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if ((*ChannelNvList)[Ch].enabled == 0) {
          continue;
        }
        if (CheckRank(Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        DimmNvList = GetDimmNvList(Host, Socket, Ch);
        if (Rank >= (*DimmNvList)[Dimm].numRanks) {
          continue;
        }
        RankEnabled = 1;
        break;
      } // ch loop
        //
        // Continue to the next rank if this one is not populated
        //
      if (RankEnabled == 0) {
        continue;
      }
      if (IsBrsPresent(Host, Socket)) {
        SubChMSVx4 = 18;
      }

      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        GetChannelMapping(Ch, SubChannel, &ChannelLocal);
        for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
          if (Mode == CHECKPOINT_MINOR_SENSE_AMP) {
            DataControl0.Data = ReadDdrioDataRegCh(Host, Socket, ChannelLocal, Strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG);
            DataControl0.Bits.forceodton = 1;
            DataControl0.Bits.senseamptrainingmode = 0;
            DataControl0.Bits.internalclockson = 1;
            WriteDdrioDataRegCh(Host, Socket, ChannelLocal, Strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, DataControl0.Data);

            DataControl2.Data = ReadDdrioDataRegCh(Host, Socket, ChannelLocal, Strobe, DATACONTROL2N0_0_MCIO_DDRIO_REG);
            DataControl2.Bits.forcebiason = 1;
            DataControl2.Bits.forcerxon = 1;
            WriteDdrioDataRegCh(Host, Socket, ChannelLocal, Strobe, DATACONTROL2N0_0_MCIO_DDRIO_REG, DataControl2.Data);

          }
          else if (Mode == CHECKPOINT_MINOR_DISABLE_SENSE_AMP) {
            DataControl0.Data = ReadDdrioDataRegCh(Host, Socket, ChannelLocal, Strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG);
            DataControl0.Bits.forceodton = 0;
            DataControl0.Bits.senseamptrainingmode = 0;
            DataControl0.Bits.internalclockson = 1;
            WriteDdrioDataRegCh(Host, Socket, ChannelLocal, Strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, DataControl0.Data);

            DataControl2.Data = ReadDdrioDataRegCh(Host, Socket, ChannelLocal, Strobe, DATACONTROL2N0_0_MCIO_DDRIO_REG);
            DataControl2.Bits.forcebiason = 0;
            DataControl2.Bits.forcerxon = 0;
            WriteDdrioDataRegCh(Host, Socket, ChannelLocal, Strobe, DATACONTROL2N0_0_MCIO_DDRIO_REG, DataControl2.Data);

          }
          else {
            return;
          }
        } //Strobe
      } //Ch Loop
    } //Rank
  } //Dimm
} // SetTrainingModeLatchResult
#endif // DDR5_SUPPORT

/**

  Clears Bit-wise error status registers per channel

  @param Host  - Pointer to sysHost
  @param socket  - Socket number
  @param ch    - Channel number (0-based)

  @retval N/A

**/
VOID
ClearBWErrorStatus (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  )
{
} // ClearBWErrorStatus

/**

  Toggle EqPost2Sign while TxEq Tunning for tap2

  @param Host    - Pointer to sysHost
  @param Socket  - Socket number
  @param Enable  - Enable / Disable

  @retval N/A

**/
MRC_STATUS
ToggleEqPost2Sign (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Strobe,
  IN UINT8    InvertData
) {
  UINT8                               Ch;
  UINT8                               MaxStrobe;
  DATACONTROL2N0_0_MCIO_DDRIO_STRUCT  DataControl2;
  struct channelNvram                 (*ChannelNvList)[MAX_CH];
  UINT8                               MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  //
  // Define the limits
  //
  if (Strobe == ALL_DRAMS) {
    Strobe    = 0;
    MaxStrobe = MAX_STROBE;
  } else {
    MaxStrobe = Strobe + 1;
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    for (Strobe; Strobe < MaxStrobe; Strobe++) {
      if (IsStrobeNotValid (Host, Strobe)) {
        continue;
      }
      DataControl2.Data = ReadDdrioDataRegCh (Host, Socket, Ch, Strobe, DATACONTROL2N0_0_MCIO_DDRIO_REG);
      if (InvertData != INVERT_TRUE) {
        InvertData = INVERT_FALSE;
      }
      DataControl2.Bits.eqpost2sign = InvertData;
      WriteDdrioDataRegCh (Host, Socket, Ch, Strobe, DATACONTROL2N0_0_MCIO_DDRIO_REG, DataControl2.Data);
    }
  }

  return MRC_STATUS_SUCCESS;
}

/**

  Control flow between 10nm and other legacy products

  @param Host    - Pointer to sysHost
  @param Param   - Parameter power training

  @retval N/A

**/
MRC_STATUS
ExecuteTheNextOptPhaseOnThisProduct (
  IN PSYSHOST          Host,
  IN POWER_TRAIN_PARAM Param
)
{
  return MRC_STATUS_SUCCESS;
}
/**

  Set the initial configuration for the TxEq training

  @param Host     - Pointer to sysHost
  @param Socket   - Socket

  @retval MRC_STATUS

**/
MRC_STATUS
InitTxEqTunning (
  IN PSYSHOST          Host,
  IN UINT8             Socket
  )
{
  UINT8                               Ch;
  UINT8                               Strobe;
  DATACONTROL4N0_0_MCIO_DDRIO_STRUCT  DataControl4;
  struct channelNvram                 (*ChannelNvList)[MAX_CH];
  UINT8                               MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // Enable TxEq Tunning
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    for (Strobe = 0; Strobe < MAX_STROBE; Strobe++) {
      if (IsStrobeNotValid (Host, Strobe)) {
        continue;
      }
      DataControl4.Data = ReadDdrioDataRegCh (Host, Socket, Ch, Strobe, DATACONTROL4N0_0_MCIO_DDRIO_REG);
      DataControl4.Bits.dqtxeqsel = 1;
      WriteDdrioDataRegCh (Host, Socket, Ch, Strobe, DATACONTROL4N0_0_MCIO_DDRIO_REG, DataControl4.Data);
    }
  }

  ToggleEqPost2Sign (Host, Socket, ALL_DRAMS, INVERT_FALSE);

  return MRC_STATUS_SUCCESS;
}

/**

  Initialize VSSHIORVREFCONTROL

  @param[in] Host  - Pointer to sysHost
  @param[in] socket  - Processor to initialize

  @retval N/A

**/
VOID
InitVssHiControlDdrio (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket
  )
{
  //
  // Stub function to satisfy API.
  //

  RC_FATAL_ERROR (FALSE, 0, 0);

  return;

} // InitVssHiControlDdrio

/**

  DDRIO setup for C/A Parity error flow

  @param[in] Host     - Pointer to sysHost
  @param[in] socket   - Processor to initialize
  @param[in] Ch       - Channel number

  @retval N/A

**/
VOID
SetupCapWrCrcErrorFlowDdrio (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket,
  IN      UINT8       Ch
  )
{
  //
  // Stub function to satisfy API.
  //

  RC_FATAL_ERROR (FALSE, 0, 0);

  return;

} // SetupCapWrCrcErrorFlowDdrio

/**
  Set/Restore Long Read Preamble in DDRIO

  @param[in] Host         - Point to sysHost
  @param[in] Socket       - Socket number
  @param[in] Ch           - Channel number
  @param[in] Set          - if TRUE: Set Long Read Preamble
                            else:    Restore original Read Preamble state

  @retval N/A
**/
VOID
SetLongReadPreambleDdrio (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      BOOLEAN  Set
  )
{
  //
  // Stub function to satisfy API.
  //

  RC_FATAL_ERROR (FALSE, 0, 0);

  return;

} // SetupCapWrCrcErrorFlowDdrio

CONST UINT32  mCtlRanksUsedAddr[] = {
  DDRCRCTLRANKSUSED_CMDS_MCIO_DDRIOEXT_REG,
  DDRCRCTLRANKSUSED_CTL_MCIO_DDRIOEXT_REG,
  DDRCRCTLRANKSUSED_CMDN_MCIO_DDRIOEXT_REG,
  DDRCRCTLRANKSUSED_CKE_MCIO_DDRIOEXT_REG
};

CONST UINT32  mCmdControls1Addr[] = {
  DDRCRCMDCONTROLS1_CMDS_MCIO_DDRIOEXT_REG,
  DDRCRCMDCONTROLS1_CTL_MCIO_DDRIOEXT_REG,
  DDRCRCMDCONTROLS1_CMDN_MCIO_DDRIOEXT_REG,
  DDRCRCMDCONTROLS1_CKE_MCIO_DDRIOEXT_REG
};

CONST UINT32  mCmdControls2Addr[] = {
  DDRCRCMDCONTROLS2_CMDS_MCIO_DDRIOEXT_REG,
  DDRCRCMDCONTROLS2_CTL_MCIO_DDRIOEXT_REG,
  DDRCRCMDCONTROLS2_CMDN_MCIO_DDRIOEXT_REG,
  DDRCRCMDCONTROLS2_CKE_MCIO_DDRIOEXT_REG
};

CONST UINT32  mCmdControls3Addr[] = {
  DDRCRCMDCONTROLS3_CMDS_MCIO_DDRIOEXT_REG,
  DDRCRCMDCONTROLS3_CTL_MCIO_DDRIOEXT_REG,
  DDRCRCMDCONTROLS3_CMDN_MCIO_DDRIOEXT_REG,
  DDRCRCMDCONTROLS3_CKE_MCIO_DDRIOEXT_REG
};

/**
  Disables channel-specific registers for the specified channels in the DDRIO

  @param[in]  Socket        Socket number
  @param[in]  McId          Memory controller ID
  @param[in]  Channel       Socket-scope channel ID (This channel number is *not* limited by MAX_CH, and must be the number of
                              a channel that is supported by the DDRIO but not the platform definition)

  @retval   EFI_SUCCESS   Operation was successful
**/
EFI_STATUS
EFIAPI
DisableUnsupportedChannelRegisters (
  IN  UINT8     Socket,
  IN  UINT8     McId,
  IN  UINT8     Channel
  )
{

  PSYSHOST  Host = NULL;
  UINT8     FubIndex;
  UINT8     Strobe;
  UINT32    RegAddr;

  DDRCRCLKCOMPOFFSET_CLK_MCIO_DDRIOEXT_STRUCT CompOffset;
  DDRCRCLKCONTROLS_MCIO_DDRIOEXT_STRUCT       ClkControls;
  DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_STRUCT      ClkRanksUsed;
  DDRCRCLKTRAINING_MCIO_DDRIOEXT_STRUCT       ClkTraining;
  DATACONTROL1N0_0_MCIO_DDRIO_STRUCT          DataControl1;
  DATACONTROL3N0_0_MCIO_DDRIO_STRUCT          DataControl3;
  DATACONTROL4N0_0_MCIO_DDRIO_STRUCT          DataControl4;
  DDRCRCTLRANKSUSED_CMDS_MCIO_DDRIOEXT_STRUCT CtlRanksUsed;
  DDRCRCMDCONTROLS1_CMDS_MCIO_DDRIOEXT_STRUCT CmdControls1;
  DDRCRCMDCONTROLS2_CMDS_MCIO_DDRIOEXT_STRUCT CmdControls2;
  DDRCRCMDCONTROLS3_CMDS_MCIO_DDRIOEXT_STRUCT CmdControls3;

  if (IsBrsPresent (Host, Socket)) {
    return EFI_SUCCESS;
  }

  Host = GetSysHostPointer ();

  //
  // Handle registers that have one instance per FUB by iterating over and programming each FUB individually
  //

  for (FubIndex = 0; FubIndex < sizeof (mCtlRanksUsedAddr) / sizeof (mCtlRanksUsedAddr[0]); FubIndex++) {
    CtlRanksUsed.Data = MemReadPciCfgEp (Socket, Channel, SET_UNSPTD_FUNCBLOCK_DDRIO(mCtlRanksUsedAddr[FubIndex]));
    CtlRanksUsed.Bits.ranken = 0;
    CtlRanksUsed.Bits.ddrcmdrxbiasctl = 0;
    CtlRanksUsed.Bits.ddrcmdrxbiasfoldedlegtrim = 0;
    MemWritePciCfgEp (Socket, Channel, SET_UNSPTD_FUNCBLOCK_DDRIO(mCtlRanksUsedAddr[FubIndex]), CtlRanksUsed.Data);
  }

  for (FubIndex = 0; FubIndex < sizeof (mCmdControls1Addr) / sizeof (mCmdControls1Addr[0]); FubIndex++) {
    CmdControls1.Data = MemReadPciCfgEp (Socket, Channel, SET_UNSPTD_FUNCBLOCK_DDRIO(mCmdControls1Addr[FubIndex]));
    CmdControls1.Bits.dqstatdflt = 0;
    MemWritePciCfgEp (Socket, Channel, SET_UNSPTD_FUNCBLOCK_DDRIO(mCmdControls1Addr[FubIndex]), CmdControls1.Data);
  }

  for (FubIndex = 0; FubIndex < sizeof (mCmdControls2Addr) / sizeof (mCmdControls2Addr[0]); FubIndex++) {
    CmdControls2.Data = MemReadPciCfgEp (Socket, Channel, SET_UNSPTD_FUNCBLOCK_DDRIO(mCmdControls2Addr[FubIndex]));
    CmdControls2.Bits.forcecmdbiason = 0;
    MemWritePciCfgEp (Socket, Channel, SET_UNSPTD_FUNCBLOCK_DDRIO(mCmdControls2Addr[FubIndex]), CmdControls2.Data);
  }

  for (FubIndex = 0; FubIndex < sizeof (mCmdControls3Addr) / sizeof (mCmdControls3Addr[0]); FubIndex++) {
    CmdControls3.Data = MemReadPciCfgEp (Socket, Channel, SET_UNSPTD_FUNCBLOCK_DDRIO(mCmdControls3Addr[FubIndex]));
    CmdControls3.Bits.bufdrvsegen = 0;
    CmdControls3.Bits.odtstrength = 0;
    MemWritePciCfgEp (Socket, Channel, SET_UNSPTD_FUNCBLOCK_DDRIO(mCmdControls3Addr[FubIndex]), CmdControls3.Data);
  }

  //
  // These registers have one instance per strobe
  //
  for (Strobe = 0; Strobe < MAX_STROBE; Strobe++) {

    RegAddr = SET_UNSPTD_FUNCBLOCK_DDRIO(UpdateIoRegisterCh10nm (Host, Channel, Strobe, DATACONTROL1N0_0_MCIO_DDRIO_REG));
    DataControl1.Data = MemReadPciCfgEp (Socket, Channel, RegAddr);
    DataControl1.Bits.usepxccbb = 0;
    MemWritePciCfgEp (Socket, Channel, RegAddr, DataControl1.Data);

    RegAddr = SET_UNSPTD_FUNCBLOCK_DDRIO(UpdateIoRegisterCh10nm (Host, Channel, Strobe, DATACONTROL3N0_0_MCIO_DDRIO_REG));
    DataControl3.Data = MemReadPciCfgEp (Socket, Channel, RegAddr);
    DataControl3.Bits.drvstaticlegcfg = 0;
    DataControl3.Bits.odtstaticlegcfg = 0;
    DataControl3.Bits.odtsegmentenable = 0;
    DataControl3.Bits.datasegmentenable = 0;
    DataControl3.Bits.imodebiasvrefen = 0;
    DataControl3.Bits.imodebiasrxbiastrim = 0;
    DataControl3.Bits.pcasbiasclosedloopen = 0;
    DataControl3.Bits.rxbiasfoldedlegtrim = 0;
    MemWritePciCfgEp (Socket, Channel, RegAddr, DataControl3.Data);

    RegAddr = SET_UNSPTD_FUNCBLOCK_DDRIO(UpdateIoRegisterCh10nm (Host, Channel, Strobe, DATACONTROL4N0_0_MCIO_DDRIO_REG));
    DataControl4.Data = MemReadPciCfgEp (Socket, Channel, RegAddr);
    DataControl4.Bits.rxvrefsel = 0;
    MemWritePciCfgEp (Socket, Channel, RegAddr, DataControl4.Data);
  }

  //
  // The remaining registers have only one instance per channel
  //

  CompOffset.Data = MemReadPciCfgEp (Socket, Channel, SET_UNSPTD_FUNCBLOCK_DDRIO(DDRCRCLKCOMPOFFSET_CLK_MCIO_DDRIOEXT_REG));
  CompOffset.Bits.forceclkbiason = 0;
  CompOffset.Bits.clkpcasbiasclosedloopen = 0;
  CompOffset.Bits.ddrclkrxbiasctl = 0;
  CompOffset.Bits.ddrclkrxbiasfoldedlegtrim = 0;
  MemWritePciCfgEp (Socket, Channel, SET_UNSPTD_FUNCBLOCK_DDRIO(DDRCRCLKCOMPOFFSET_CLK_MCIO_DDRIOEXT_REG), CompOffset.Data);

  ClkControls.Data = MemReadPciCfgEp (Socket, Channel, SET_UNSPTD_FUNCBLOCK_DDRIO(DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG));
  ClkControls.Bits.statlegen = 0;

#ifndef DDR5_SUPPORT
  ClkControls.Bits.odtpupsegen = 0;
#endif // ifndef DDR5_SUPPORT

  MemWritePciCfgEp (Socket, Channel, SET_UNSPTD_FUNCBLOCK_DDRIO(DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG), ClkControls.Data);

  ClkRanksUsed.Data = MemReadPciCfgEp (Socket, Channel, SET_UNSPTD_FUNCBLOCK_DDRIO(DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG));
  ClkRanksUsed.Bits.ranken = 0;
  ClkRanksUsed.Bits.drvsegen = 0;
  MemWritePciCfgEp (Socket, Channel, SET_UNSPTD_FUNCBLOCK_DDRIO(DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG), ClkRanksUsed.Data);

  ClkTraining.Data = MemReadPciCfgEp (Socket, Channel, SET_UNSPTD_FUNCBLOCK_DDRIO(DDRCRCLKTRAINING_MCIO_DDRIOEXT_REG));
  ClkTraining.Bits.extstatlegen = 0;
  MemWritePciCfgEp (Socket, Channel, SET_UNSPTD_FUNCBLOCK_DDRIO(DDRCRCLKTRAINING_MCIO_DDRIOEXT_REG), ClkTraining.Data);

  return EFI_SUCCESS;
}

/**
  Disables channels in DDRIO hardware that are unsupported by the platform

  @param[in]  Socket        Socket number to disable unused channels for

  @retval EFI_SUCCESS   Unsupported channels successfully disabled
  @retval !EFI_SUCCESS  An error occurred during the disabling of unsupported channels
**/
EFI_STATUS
EFIAPI
DisableUnsupportedChannelsDdrio (
  IN  UINT8   Socket
  )
{
  UINT8       NumChPerMc = GetNumChannelPerMc ();
  UINT8                                      NumChPerMcFused[MAX_IMC];
  UINT32                                     SupportedChMaskPlatform[MAX_IMC];
  UINT32                                     UnsupportedChMask[MAX_IMC];
  UINT8       McId;
  UINT8       ChannelIndex;
  UINT8       Channel;
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT8       MaxDdrioChannels = FixedPcdGet8 (PcdMaxDdrioIpChannels);
  UINT8       MaxDdrioChannelsMask = ((1 << MaxDdrioChannels) - 1);
  UINT8                                      ChDisabledMask = 0;
  UINT8       MaxImc = GetMaxImc ();
  UINT8                                      FusedChannelsCount;
  UINT8                                      FusedChannelsBitMap;
  DDRCRDIMMVREFCONTROL1_MCIO_DDRIOEXT_STRUCT DimmVrefControl1;
  PSYSHOST    Host = NULL;

  Host = GetSysHostPointer ();
  MaxImc = GetMaxImc ();
  SetMem32 (SupportedChMaskPlatform, sizeof (SupportedChMaskPlatform), (1 << NumChPerMc) - 1);
  SetMem (NumChPerMcFused, sizeof (NumChPerMcFused), MaxDdrioChannels);
  SetMem32 (UnsupportedChMask, sizeof (UnsupportedChMask), SupportedChMaskPlatform[0] ^ MaxDdrioChannelsMask);

  if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    //
    // Read the number of channels enabled in the fuse
    //
    FusedChannelsCount = (Host->var.common.procCom[Socket].capid5 >> 21) & 0xF;
    // 1 MC, 2 channels per MC, and 2 MC, 2 channels per MC cases
    FusedChannelsBitMap = 0x4;
    // 1 MC, 3 channels per MC case
    if (FusedChannelsCount == 3) {
      FusedChannelsBitMap = 0;
    }


    //
    // Loop through the channels in one MC to discover what is disabled by fuse
    //
    for (McId = 0; McId < MaxImc; McId++) {
      ChDisabledMask = 0;
      for (ChannelIndex = 0; ChannelIndex < MaxDdrioChannels; ChannelIndex++) {
        if (FusedChannelsBitMap & (1 << ((McId*MaxDdrioChannels) + ChannelIndex))) {
          ChDisabledMask |= 1 << ChannelIndex;
          NumChPerMcFused[McId]--;
        }
      }

      if (NumChPerMc != NumChPerMcFused[McId]) {
        NumChPerMc = NumChPerMcFused[McId];
        UnsupportedChMask[McId] = ChDisabledMask;
      }

      RcDebugPrint (SDBG_MAX, "MC%d: Channels disabled in fuse mask = 0x%X, Calculated channel disable mask DDRIO = 0x%X\n", McId, FusedChannelsBitMap, UnsupportedChMask[McId]);
    }
  }

  //
  // Nothing to do if the platform and DDRIO support the same number of channels
  //
  if (NumChPerMc == MaxDdrioChannels) {
    return EFI_SUCCESS;
  }

  //
  // Disable unsupported channels in all channel-specific registers first
  //
  for (McId = 0; McId < MaxImc; McId++) {
    for (ChannelIndex = 0; ChannelIndex < MaxDdrioChannels; ++ChannelIndex) {
      if (UnsupportedChMask[McId] & (1 << ChannelIndex)) {
      Channel = (McId * MaxDdrioChannels) + ChannelIndex;
      if (EFI_ERROR (DisableUnsupportedChannelRegisters (Socket, McId, Channel))) {
        Status = EFI_DEVICE_ERROR;
      }
    }
  }
  }

  //
  // Now disable unsupported channels in registers that have only one instance per MC
  //
  for (McId = 0; McId < MaxImc; McId++) {
    if (EFI_ERROR (DisableChannelsDdrio (Socket, McId, UnsupportedChMask[McId]))) {
      Status = EFI_DEVICE_ERROR;
    }

    if (IsBrsPresent (Host, Socket)) {
      continue;
    }

    //
    // There is only one instance of this register per DDRIO IP, but it is not touched when disabling supported but
    // unpopulated channels, so DisableChannelsDdrio () doesn't clear this bit. This must be cleared for unsupported
    // channels.
    //
    DimmVrefControl1.Data = MemReadPciCfgMC (Socket, McId, DDRCRDIMMVREFCONTROL1_MCIO_DDRIOEXT_REG);
    DimmVrefControl1.Bits.ch2dimmvrefen = 0;
    MemWritePciCfgMC (Socket, McId, DDRCRDIMMVREFCONTROL1_MCIO_DDRIOEXT_REG, DimmVrefControl1.Data);
  }

  return Status;
} // DisableUnsupportedChannelsDdrio

/**
  Disables the specified channels in the DDRIO

  @param[in]  Socket        Socket number
  @param[in]  McId          Memory controller ID
  @param[in]  ChDisableMask Bit mask indicating which channels on memory controller McId to disable
                            Bit 0 = McId CH0, Bit 1 = McId CH1, etc

  @retval   EFI_SUCCESS   Operation was successful
**/
EFI_STATUS
EFIAPI
DisableChannelsDdrio (
  IN  UINT8     Socket,
  IN  UINT8     McId,
  IN  UINT32    ChDisableMask
  )
{
  PSYSHOST                              Host;
  DDRCRINTFPMCONFIG0_MCIO_DDRIO_STRUCT  DdrCrIntFpmConfig0;
  UINT32                                MaxDdrioChannels = (UINT32) FixedPcdGet8 (PcdMaxDdrioIpChannels);
  UINT32                                MaxDdrioChannelsMask = ((1 << MaxDdrioChannels) - 1);

  Host = GetSysHostPointer ();

  //
  // Emit warning if disable mask is wider than the DDRIO IP supports
  //
  if (ChDisableMask > MaxDdrioChannelsMask) {
    RcDebugPrint (SDBG_WARN,
      "Error disabling channels in DDRIO. Invalid channel mask. McId: %d Channel Disable Mask: 0x%x\n" \
      "Mask will be truncated to maximum number of channels supported by the DDRIO IP\n",
      McId, ChDisableMask);
      ChDisableMask &= MaxDdrioChannelsMask;
  }

  //
  // Some channels may have been previously disabled due to being supported by the DDRIO IP but
  // not the platform, so we need a RMW cycle to avoid accidentally re-enabling them
  //
  DdrCrIntFpmConfig0.Data = MemReadPciCfgMC (Socket, McId, DDRCRINTFPMCONFIG0_MCIO_DDRIO_REG);
  DdrCrIntFpmConfig0.Bits.chdisable |= ChDisableMask;
  MemWritePciCfgMC (Socket, McId, DDRCRINTFPMCONFIG0_MCIO_DDRIO_REG, DdrCrIntFpmConfig0.Data);

  return EFI_SUCCESS;
}

/**

  Get/Set FIFO Point Control bit

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Socket      - Socket number
  @param [in]       Ch          - Channel number
  @param [in]       Mode        - GSM_READ_ONLY - Do not write
                                - GSM_FORCE_WRITE - Force the write
  @param [in,out]   Value       - IN this value is used to set/clear fifoptctl bit
                                - OUT contains the current value of the fifoptctl bit

  @retval EFI_SUCESS

**/
EFI_STATUS
GetSetFifoPointControl (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    Mode,
  IN OUT  UINT8    *Value
)
{
  EFI_STATUS                                  Status = EFI_SUCCESS;
  DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_STRUCT      DdrCRClkRanksEnabled;
  struct channelNvram                         (*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);

  if (Mode == GSM_FORCE_WRITE) {
    DdrCRClkRanksEnabled.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG);
    DdrCRClkRanksEnabled.Bits.fifoptctlqnnnh = *Value;
    MemWritePciCfgEp (Socket, Ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG, DdrCRClkRanksEnabled.Data);
    //
    // Write to NVRAM for Fast path restored
    //
    (*ChannelNvList)[Ch].ddrCRClkRanksUsed = DdrCRClkRanksEnabled.Data;
  } else if (Mode == GSM_READ_ONLY) {
    DdrCRClkRanksEnabled.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG);
    *Value = (UINT8)DdrCRClkRanksEnabled.Bits.fifoptctlqnnnh;
  } else {
    Status = EFI_INVALID_PARAMETER;
  }

  return Status;
}

//
// Bitmasks for programming Cmd Output Enable
//
#define ALL_CMDCTL_SIGNALS    0xFFF
#define ALL_CMD_SIGNALS_CMDN  0xD7D //Bypass CS0, CS1(GNT# for slot0), CS5(GNT# for slot1)
#define ALL_CMD_SIGNALS_CMDS  0x9FF //Bypass CKE0/2
#define ALL_CMD_SIGNALS_CTL   0x3FF
#define ALL_CMD_SIGNALS_CKE   0x1FF //Bypass CS4

//
// Calculations for working with Cmd Output buffer passing
//
#define CMD_OUTPUT_EN_SEL_MASK    0xFFF
#define CMD_OUTPUT_EN_SEL_WIDTH   12  // width of the cmdoutputensel field programmed from this buffer
#define SHIFT_CMDN                0
#define SHIFT_CMDS                1 * CMD_OUTPUT_EN_SEL_WIDTH
#define SHIFT_CTL                 2 * CMD_OUTPUT_EN_SEL_WIDTH
#define SHIFT_CKE                 3 * CMD_OUTPUT_EN_SEL_WIDTH

/**
  Program Cmd Ctl Output Enable

  This function is used to enable or disable (tristate) output pins in the DDRIO on the CMD/CTL busses. It does this
  by programming the cmdoutputensel field of the DdrCrCmdControls1 registers. Each register corresponds to a different
  functional unit block (FUB) of the DDRIO. These fubs are referred to by different names in different areas of the
  code and platform documentation. The following is a map of the different FUB names that might be encountered:

    Command North     Instance 0    CMDN    CMDCTLA North
    Command South     Instance 1    CMDS    CMDCTLA South
    Control           Instance 2    CTL     CMDCTLB South
    CKE               Instance 3    CKE     CMDCTLB North

  @param[in]              Host    Pointer to sysHost
  @param[in]              Socket  Current socket
  @param[in]              Channel Channel to program registers for
  @param[in]              Command Indicates whether to get, enable, or disable output and on which signals
  @param[in, out] Buffer  Buffer to store current enable settings to restore later

  @retval N/A
**/
VOID
CmdCtlOutputEnable (
  IN      PSYSHOST                Host,
  IN      UINT8                   Socket,
  IN      UINT8                   Channel,
  IN      CMD_CTL_OUT_EN          Command,
  IN OUT  CMD_CTL_OUTPUT_BUFFER   *Buffer
  )
{
  DDRCRCMDCONTROLS1_CMDN_MCIO_DDRIOEXT_STRUCT    DdrCrCmdControls1Cmdn;
  DDRCRCMDCONTROLS1_CMDS_MCIO_DDRIOEXT_STRUCT    DdrCrCmdControls1Cmds;
  DDRCRCMDCONTROLS1_CTL_MCIO_DDRIOEXT_STRUCT     DdrCrCmdControls1Ctl;
  DDRCRCMDCONTROLS1_CKE_MCIO_DDRIOEXT_STRUCT     DdrCrCmdControls1Cke;

  DdrCrCmdControls1Cmdn.Data = MemReadPciCfgEp (Socket, Channel, DDRCRCMDCONTROLS1_CMDN_MCIO_DDRIOEXT_REG);
  DdrCrCmdControls1Cmds.Data = MemReadPciCfgEp (Socket, Channel, DDRCRCMDCONTROLS1_CMDS_MCIO_DDRIOEXT_REG);
  DdrCrCmdControls1Ctl.Data = MemReadPciCfgEp (Socket, Channel, DDRCRCMDCONTROLS1_CTL_MCIO_DDRIOEXT_REG);
  DdrCrCmdControls1Cke.Data = MemReadPciCfgEp (Socket, Channel, DDRCRCMDCONTROLS1_CKE_MCIO_DDRIOEXT_REG);

  switch (Command) {

  case CmdCtlOutputEnableAll:
    DdrCrCmdControls1Cmdn.Bits.cmdoutputensel |= ALL_CMDCTL_SIGNALS;
    DdrCrCmdControls1Cmds.Bits.cmdoutputensel |= ALL_CMDCTL_SIGNALS;
    DdrCrCmdControls1Ctl.Bits.cmdoutputensel |= ALL_CMDCTL_SIGNALS;
    DdrCrCmdControls1Cke.Bits.cmdoutputensel |= ALL_CMDCTL_SIGNALS;
    break;

  case CmdCtlOutputDisableCmd:
    DdrCrCmdControls1Cmdn.Bits.cmdoutputensel &= (~ALL_CMD_SIGNALS_CMDN);
    DdrCrCmdControls1Cmds.Bits.cmdoutputensel &= (~ALL_CMD_SIGNALS_CMDS);
    DdrCrCmdControls1Ctl.Bits.cmdoutputensel &= (~ALL_CMD_SIGNALS_CTL);
    DdrCrCmdControls1Cke.Bits.cmdoutputensel &= (~ALL_CMD_SIGNALS_CKE);
    break;

  case CmdCtlOutputRestore:
    if (Buffer == NULL) {
      RcDebugPrint (SDBG_ERROR, "\nNULL buffer given to CmdCtlOutputEnable during Restore request on Socket %d, Channel %d: %d\n", Socket, Channel);
      ASSERT (FALSE);
      return;
    }

    DdrCrCmdControls1Cmdn.Bits.cmdoutputensel = RShiftU64 (*Buffer, SHIFT_CMDN) & CMD_OUTPUT_EN_SEL_MASK;
    DdrCrCmdControls1Cmds.Bits.cmdoutputensel = RShiftU64 (*Buffer, SHIFT_CMDS) & CMD_OUTPUT_EN_SEL_MASK;
    DdrCrCmdControls1Ctl.Bits.cmdoutputensel = RShiftU64 (*Buffer, SHIFT_CTL) & CMD_OUTPUT_EN_SEL_MASK;
    DdrCrCmdControls1Cke.Bits.cmdoutputensel = RShiftU64 (*Buffer, SHIFT_CKE) & CMD_OUTPUT_EN_SEL_MASK;
    break;

  case CmdCtlOutputGet:
    if (Buffer == NULL) {
      RcDebugPrint (SDBG_ERROR, "\nNULL buffer given to CmdCtlOutputEnable during Get request on Socket %d, Channel %d: %d\n", Socket, Channel);
      ASSERT (FALSE);
      return;
    }

    *Buffer = LShiftU64 (DdrCrCmdControls1Cmdn.Bits.cmdoutputensel, SHIFT_CMDN);
    *Buffer |= LShiftU64 (DdrCrCmdControls1Cmds.Bits.cmdoutputensel, SHIFT_CMDS);
    *Buffer |= LShiftU64 (DdrCrCmdControls1Ctl.Bits.cmdoutputensel, SHIFT_CTL);
    *Buffer |= LShiftU64 (DdrCrCmdControls1Cke.Bits.cmdoutputensel, SHIFT_CKE);
    return;

  default:
    RcDebugPrint (SDBG_ERROR, "\nInvalid command given to CmdCtlOutputEnable on Socket %d, Channel %d: %d\n", Socket, Channel);
    ASSERT (FALSE);
    return;
  }

  MemWritePciCfgEp (Socket, Channel, DDRCRCMDCONTROLS1_CMDN_MCIO_DDRIOEXT_REG, DdrCrCmdControls1Cmdn.Data);
  MemWritePciCfgEp (Socket, Channel, DDRCRCMDCONTROLS1_CMDS_MCIO_DDRIOEXT_REG, DdrCrCmdControls1Cmds.Data);
  MemWritePciCfgEp (Socket, Channel, DDRCRCMDCONTROLS1_CTL_MCIO_DDRIOEXT_REG, DdrCrCmdControls1Ctl.Data);
  MemWritePciCfgEp (Socket, Channel, DDRCRCMDCONTROLS1_CKE_MCIO_DDRIOEXT_REG, DdrCrCmdControls1Cke.Data);

  return;
}


/**

  Gets the maximum latency of DDRIO write plus read per channel. This value is used
  by the MC as a delay between write and read commands that hit a PCLS line.

  @param[in]  Socket         - Socket index at system level
  @param[in]  Ch             - Channel index at socket level
  @param[out] *Delay         - DDRIO write plus read latency in DCLK units

  @retval EFI_SUCCESS        - Delay value was updated
  @reval  EFI_UNSUPPORTED    - Delay value not updated

**/
EFI_STATUS
EFIAPI
GetPclsCaptureDelayDddrio (
  UINT8   Socket,
  UINT8   Ch,
  UINT32  *Delay
)
{
  PSYSHOST    Host;
  UINT8       Dimm;
  UINT8       Rank;
  UINT8       Strobe;
  UINT8       MSVx4;
  INT16       PiDelay = 0;
  UINT16      MinDelay;
  UINT16      MaxTxDqDelay;
  UINT16      MaxRecEnDelay;
  UINT16      DdrioWriteLatency;
  UINT16      DdrioReadLatency;

  Host = GetSysHostPointer ();
  MSVx4 = GetMaxStrobeValid (Host);

  MinDelay = 0xFFFF;
  MaxTxDqDelay = 0;
  MaxRecEnDelay = 0;

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {

      // If rank enabled
      if (CheckRank(Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
        continue;
      }
      for (Strobe = 0; Strobe < MSVx4; Strobe++) {
       //
       // Skip if this is an ECC strobe when ECC is disabled
       //
       if (IsStrobeNotValid (Host, Strobe)) {
         continue;
       }

       GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, TxDqDelay, GSM_READ_ONLY, (INT16 *)&PiDelay);
       UpdateMinMaxInt (PiDelay, (INT16 *)&MinDelay, (INT16 *)&MaxTxDqDelay);

       GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RecEnDelay, GSM_READ_ONLY, (INT16 *)&PiDelay);
       UpdateMinMaxInt (PiDelay, (INT16 *)&MinDelay, (INT16 *)&MaxRecEnDelay);

     } // strobe loop
   } // rank loop
  } // dimm loop

  //
  // Get maximum DDRIO write latency.
  // Note: Assume that TxDqDelay is always larger than TxDqsDelay
  //
  // WriteIO_Delay = 3Qclk(repeaters) + 1Qclk(txfifo write) + [0 to 10Qclk](dq delay) + 1.5Qclk(txfifo read) + 1Qclk(piL flop) + 1Qclk(piN flop) +
  //                    [0 or 2](PXC enabled?) + [0 or 2](2tck write preamble enabled?) + ~1UI(analog / phys delay)
  //               = 8.5Qclk + (PXC enabled ? 0 : 2) + (2tck Write Preamble Enabled ? 0 : 2) + DQ_Delay
  //
  // Where DQ_Delay = Max[(DqsFlyby + PiCode), (DqFlyby + BitPicode)]
  // �DqFlyby?: TXGROUP0RANKx.TxDqDelay[9:0]
  // �BitPicode?: TXGROUP1RANKx.TxDqPiCode[0 - 3][5:0], this is per DQ PiCode, total 4 PiCode per nibble.
  //
  DdrioWriteLatency = 85;
  if (Host->var.mem.pxcEnabled & (1 << Ch)) {
    DdrioWriteLatency += 20;
  }
  if (GetPreambleState (Socket, WRITE_PREAMBLE) == PREAMBLE_2TCLK) {
    DdrioWriteLatency += 20;
  }
  DdrioWriteLatency = DdrioWriteLatency + ((MaxTxDqDelay * 10) / 64);   // Units of QCLK*10

  //
  // Get maximum DDRIO read latency.
  //
  // ReadIO_Delay = 3Qclk(repeaters) + [0 - 10Qclk](rcven delay) + [0 - 1.375]Qclk(SDL) + 1UI(rxfifo write) + 2UI(rxfifo read) + ~1UI(analog / phys delay)
  //              = 8 Qclk + rcven_delay,
  // Where Rcven_delay = Max(RXGROUP0RANK[0 - 7].RcvEnDelay)
  //
  DdrioReadLatency = 80 + ((MaxRecEnDelay * 10) / 64);   // Units of QCLK*10

  //
  // Add DDRIO write + read latency and convert to DCLK, rounding up
  //
  *Delay = (DdrioWriteLatency + DdrioReadLatency + 19) / 20;

  return EFI_SUCCESS;

} // GetPclsCaptureDelayDddrio



/**
  Ensure TX DQS delays in a byte are within MAX_DQ_GAP ticks of each other

  @param[in] Socket     - Socket number

  @reval    N/A
**/
VOID
TxDqsWA (
  UINT8     Socket
  )
{
  PSYSHOST            Host;
  UINT8               Ch;
  UINT8               SubCh;
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               Strobe;
  UINT8               LowStrobe = 0;
  UINT8               HighStrobe = 0;
  INT16               DelayStrobe0;
  INT16               DelayStrobe1;
  INT16               DelayMin;
  INT16               DelayMax;
  INT16               Offset;
  INT16               DelayMinOffset;
  INT16               DelayMaxOffset;
  INT16               Difference;
  UINT8               ChannelLocal;
  UINT8               MSVx4;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];

  Host = GetSysHostPointer ();
  if (IsBrsPresent (Host, Socket) == FALSE) {
    return;
  }

  ChannelNvList = GetChannelNvList (Host, Socket);
  MSVx4 = GetMaxStrobeValid (Host);

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

      for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
        if ((*ChannelNvList) [Ch].enabled == 0) {
          continue;
        }
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        //
        // This WA is only needed for x4 DIMMs
        //
        if ((*DimmNvList)[Dimm].x4Present == 0) {
          continue;
        }

        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }

        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          GetChannelMapping (Ch, SubCh, &ChannelLocal);

          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT,
            "Applying TxDqsWA\n");

          //
          // Loop for each byte
          //
          for (Strobe = 0; Strobe < (MSVx4 / 2); Strobe++) {
            //
            // Skip if this is an ECC nibble when ECC is disabled
            //
            if (IsStrobeNotValidDdr5 (Host, Strobe)) {
              continue;
            }

            GetSetDataGroup (Host, Socket, ChannelLocal, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, &DelayStrobe0);
            GetSetDataGroup (Host, Socket, ChannelLocal, NO_SUBCH, Dimm, Rank, Strobe + MSVx4 / 2, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, &DelayStrobe1);
            if (DelayStrobe0 > DelayStrobe1) {
              DelayMax = DelayStrobe0;
              DelayMin = DelayStrobe1;
              HighStrobe = Strobe;
              LowStrobe = Strobe + MSVx4 / 2;
            } else {
              DelayMax = DelayStrobe1;
              DelayMin = DelayStrobe0;
              HighStrobe = Strobe + MSVx4 / 2;
              LowStrobe = Strobe;
            }

            //
            // Check if the max dq delay skew is > MAX_DQ_GAP
            //
            if ((DelayMax - DelayMin) > MAX_DQ_GAP) {
              RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT,
                "TxDqs skew is over limit in Byte %d: DelayMax(%d) - DelayMin(%d) = %d \n", Strobe, DelayMax, DelayMin, DelayMax - DelayMin);

              //
              // Make sure delays are within MAX_DQ_GAP pi ticks
              //
              Difference = (DelayMax - DelayMin) - MAX_DQ_GAP;

              //
              // Make Difference an even number
              //
              if (Difference % 2) {
                Difference++;
              }

              //
              // Get the amount to adjust by
              //
              Offset = Difference / 2;
              DelayMinOffset =  Offset;
              DelayMaxOffset = - Offset;

              GetSetDataGroup (Host, Socket, ChannelLocal, NO_SUBCH, Dimm, Rank, LowStrobe, 0, DdrLevel, TxDqsDelay, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &DelayMinOffset);
              GetSetDataGroup (Host, Socket, ChannelLocal, NO_SUBCH, Dimm, Rank, HighStrobe, 0, DdrLevel, TxDqsDelay, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &DelayMaxOffset);
            }
          } // Strobe
        } // SubCh
      } // Ch
    } // Rank
  } // Dimm
} // TxDqsWA

/*

  Update the register value with the cached value from the original TxDqsDelay

  @param[in]  Host      Pointer to Host structure
  @param[in]  Socket    Current socket under test (0-based)
  @param[in]  Ch        Current channel under test (0-based)
  @param[in]  SubCh     Current subchannel under test (0-based)
  @param[in]  Dimm      Current dimm under test (0-based)
  @param[in]  Rank      Current rank under test (0-based)
  @param[in]  Nibble    Current nibble under test (0-based).
                        This is not the strobe number (use function GetNibbleFromStrobe to do the conversion)

  @retval     EFI_INVALID_PARAMETER   If not a valid Nibble
  @retval     EFI_INVALID_PARAMETER   Nibble is an odd number
  @retval     EFI_SUCCESS             Function executed properly

*/
EFI_STATUS
EFIAPI
RestoreTxDqsOriginal (
  IN  OUT   PSYSHOST    Host,
  IN        UINT8       Socket,
  IN        UINT8       Ch,
  IN        UINT8       SubCh,
  IN        UINT8       Dimm,
  IN        UINT8       Rank,
  IN        UINT8       Nibble
  )
{
  UINT8   ChannelLocal;
  UINT8   Strobe;
  UINT8   LogRank;
  UINT8   NibbleIndex;
  INT16   TxDqsOffset;
  UINT32  DbgLevel = SDBG_MAX;
  UINT8   MSVx4;
  struct channelNvram (*ChannelNvList)[MAX_CH];

  MSVx4 = GetMaxStrobeValid (Host);

  if (IsNibbleNotValid (Host, Nibble)) {
    return EFI_INVALID_PARAMETER;
  }

  if (Nibble % 2) {
    return EFI_INVALID_PARAMETER;
  }

  GetChannelMapping (Ch, SubCh, &ChannelLocal);

  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // Get the logical rank #
  //
  LogRank = GetLogicalRank (Host, Socket, ChannelLocal, Dimm, Rank);

  for (NibbleIndex = 0; NibbleIndex < MAX_NIBBLES_PER_BYTE; NibbleIndex++) {

    Strobe = GetStrobeFromNibble (Host, Nibble + NibbleIndex);

    TxDqsOffset = (*ChannelNvList)[ChannelLocal].TxDqsDelay[LogRank][Strobe];

    RcDebugPrintWithDevice (
      DbgLevel, Socket, Ch, Dimm, SubCh, Rank, Strobe, NO_BIT,
      "TxDqsOffset %03d \n",
      TxDqsOffset
      );

    GetSetDataGroup (
      Host, Socket, ChannelLocal, NO_SUBCH, Dimm, Rank, Strobe, ALL_BITS,
      DdrLevel, TxDqsDelay, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &TxDqsOffset
      );
  }

  return EFI_SUCCESS;

}

/*

  Update the cached value for the original TxDqsDelay

  @param[in]  Host      Pointer to Host structure
  @param[in]  Socket    Current socket under test (0-based)
  @param[in]  Ch        Current channel under test (0-based)
  @param[in]  SubCh     Current subchannel under test (0-based)
  @param[in]  Dimm      Current dimm under test (0-based)
  @param[in]  Rank      Current rank under test (0-based)
  @param[in]  Nibble    Current nibble under test (0-based).
                        This is not the strobe number (use function GetNibbleFromStrobe to do the conversion)

  @retval     EFI_INVALID_PARAMETER   If not a valid Nibble
  @retval     EFI_INVALID_PARAMETER   Nibble is an odd number
  @retval     EFI_SUCCESS             Function executed properly

*/
EFI_STATUS
EFIAPI
UpdateTxDqsOriginal (
  IN  OUT   PSYSHOST    Host,
  IN        UINT8       Socket,
  IN        UINT8       Ch,
  IN        UINT8       SubCh,
  IN        UINT8       Dimm,
  IN        UINT8       Rank,
  IN        UINT8       Nibble
  )
{
  UINT8   ChannelLocal;
  UINT8   Strobe;
  UINT8   LogRank;
  UINT8   NibbleIndex;
  UINT8   MSVx4;
  INT16   TxDqsOffset;
  struct channelNvram (*ChannelNvList)[MAX_CH];

  MSVx4 = GetMaxStrobeValid (Host);

  if (IsNibbleNotValid (Host, Nibble)) {
    return EFI_INVALID_PARAMETER;
  }

  if (Nibble % 2) {
    return EFI_INVALID_PARAMETER;
  }

  GetChannelMapping (Ch, SubCh, &ChannelLocal);

  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // Get the logical rank #
  //
  LogRank = GetLogicalRank (Host, Socket, ChannelLocal, Dimm, Rank);

  for (NibbleIndex = 0; NibbleIndex < MAX_NIBBLES_PER_BYTE; NibbleIndex++) {

    Strobe = GetStrobeFromNibble (Host, Nibble + NibbleIndex);

    GetSetDataGroup (
      Host, Socket, ChannelLocal, NO_SUBCH, Dimm, Rank, Strobe, ALL_BITS,
      DdrLevel, TxDqsDelay, GSM_READ_ONLY | GSM_READ_CSR, &TxDqsOffset
      );

    RcDebugPrintWithDevice (
      SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, Strobe, NO_BIT,
      "TxDqsOffset %03d (Update)\n",
      TxDqsOffset
      );

    (*ChannelNvList)[ChannelLocal].TxDqsDelay[LogRank][Strobe] = TxDqsOffset;

  }

  return EFI_SUCCESS;
}

/*

  Enforces the maximum gap between bits within the BYTE,

  @param[in]  Host      Pointer to Host structure
  @param[in]  Nibble    Current nibble under test (0-based).
                        This is not the strobe number (use function GetNibbleFromStrobe to do the conversion)
  @param[in,out]  PiPosition  Pointer with the information of each center position within a BYTE
  @param[in,out]  BitMin      Returns the min value across bits in the BYTE
  @param[in,out]  BitMax      Returns the max value across bits in the BYTE

*/
VOID
GetMinMaxLowHighStrobe (
  IN  OUT   PSYSHOST    Host,
  IN        UINT8       Nibble,
  IN        UINT16      PiPosition[MAX_BITS_IN_BYTE],
      OUT   INT16       *BitMin,
      OUT   INT16       *BitMax,
      OUT   UINT8       *LowStrobe,   OPTIONAL
      OUT   UINT8       *HighStrobe   OPTIONAL
  )
{
  UINT8   Bit;
  UINT8   Strobe;
  UINT16  BitCenter[MAX_BITS_IN_BYTE];

  *BitMin = 1024;  // Set to largest possible
  *BitMax = 0;     // Set to smallest possible

  //
  // Loop for all the bits in this byte
  //
  for (Bit = 0; Bit < MAX_BITS_IN_BYTE; Bit++) {

    Strobe = GetStrobeFromNibble (Host, Nibble + (Bit / BITS_PER_NIBBLE));

    BitCenter[Bit] = PiPosition[Bit];
    //
    // Check if the center of this bit is smaller than the smallest found so far
    //
    if (BitCenter[Bit] < *BitMin) {
      *BitMin = BitCenter[Bit];
      if (LowStrobe != NULL) {
        *LowStrobe = Strobe;
      }
    }
    //
    // Check if the center of this bit is larger than the largest found so far
    //
    if (BitCenter[Bit] > *BitMax) {
      *BitMax = BitCenter[Bit];
      if (HighStrobe != NULL) {
        *HighStrobe = Strobe;
      }
    }
  } // bit loop
}

/*

  Enforces the maximum gap between bits within the BYTE,

  @param[in]  Host      Pointer to Host structure
  @param[in]  Socket    Current socket under test (0-based)
  @param[in]  Ch        Current channel under test (0-based)
  @param[in]  SubCh     Current subchannel under test (0-based)
  @param[in]  Dimm      Current dimm under test (0-based)
  @param[in]  Rank      Current rank under test (0-based)
  @param[in]  Nibble    Current nibble under test (0-based).
                        This is not the strobe number (use function GetNibbleFromStrobe to do the conversion)
  @param[in,out]  PiPosition  Pointer with the information of each center position within a BYTE
  @param[in]      ResultsBit  Pointer with the margining information of each bit within the BYTE
  @param[in,out]  BitMin      Returns the min value across bits in the BYTE
  @param[in,out]  BitMax      Returns the max value across bits in the BYTE

  @retval     EFI_INVALID_PARAMETER   If not a valid Nibble
  @retval     EFI_INVALID_PARAMETER   Nibble is an odd number
  @retval     EFI_SUCCESS             Function executed properly

*/
EFI_STATUS
EFIAPI
TxDqMaxSkewGapEnforcement (
  IN  OUT   PSYSHOST    Host,
  IN        UINT8       Socket,
  IN        UINT8       Ch,
  IN        UINT8       SubCh,
  IN        UINT8       Dimm,
  IN        UINT8       Rank,
  IN        UINT8       Nibble,
  IN        UINT16      PiPosition[MAX_BITS_IN_BYTE],
  IN struct baseMargin  ResultsBit[MAX_BITS_IN_BYTE],  OPTIONAL
      OUT   INT16       *BitMin,
      OUT   INT16       *BitMax
)
{
  UINT8               ChannelLocal;
  UINT8               Strobe;
  UINT8               LowStrobe = 0;
  UINT8               HighStrobe = 0;
  UINT8               Bit;
  INT16               TxDqsOffset;
  INT16               Difference;
  INT16               Offset;
  UINT8               MSVx4;

  MSVx4 = GetMaxStrobeValid (Host);

  if (IsNibbleNotValid (Host, Nibble)) {
    return EFI_INVALID_PARAMETER;
  }

  if (Nibble % 2) {
    return EFI_INVALID_PARAMETER;
  }

  GetChannelMapping (Ch, SubCh, &ChannelLocal);

  GetMinMaxLowHighStrobe (Host, Nibble, PiPosition, BitMin, BitMax, &LowStrobe, &HighStrobe);
  //
  // Check if the max dq delay skew is > MAX_DQ_GAP
  //
  if ((*BitMax - *BitMin) > MAX_TX_PER_BIT_SKEW) {
    RcDebugPrintWithDevice (
      SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT,
      "TxDq skew is over limit in Byte %d: BitMax(%d) - BitMin(%d) = %d LowStrobe = %d HighStrobe = %d\n",
      Nibble / 2,
      *BitMax,
      *BitMin,
      *BitMax - *BitMin,
      LowStrobe,
      HighStrobe
      );

    //
    // Make sure delays are within 17 pi ticks
    //
    Difference = (*BitMax - *BitMin) - MAX_TX_PER_BIT_SKEW;

    //
    // Make Difference an even number
    //
    if (Difference % 2) {
      Difference++;
    }

    //
    // Get the amount to adjust by
    //
    Offset = Difference / 2;
    *BitMin = *BitMin + Offset;
    *BitMax = *BitMax - Offset;

    //
    // Adjust the strobe groups equally
    //
    for (Bit = 0; Bit < MAX_BITS_IN_BYTE; Bit++) {

      Strobe = GetStrobeFromNibble (Host, Nibble + (Bit / BITS_PER_NIBBLE));

      if (Strobe == LowStrobe) {
        //
        // Increase the lower TxDqs delay
        //
        TxDqsOffset = Offset;
      } else {
        //
        // Decrease the higher TxDqs delay
        //
        TxDqsOffset = -Offset;
      }

      PiPosition[Bit] += TxDqsOffset;
      if (ResultsBit != NULL) {
        ResultsBit[Bit].n += TxDqsOffset;
        ResultsBit[Bit].p += TxDqsOffset;
      }
      if (Bit % BITS_PER_NIBBLE == 0) {

        GetSetDataGroup (Host, Socket, ChannelLocal, NO_SUBCH, Dimm, Rank, Strobe, ALL_BITS, DdrLevel, TxDqsDelay, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &TxDqsOffset);

        RcDebugPrintWithDevice (
          SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, Strobe, NO_BIT,
          "Offset strobe %d by %d\n",
          Strobe,
          TxDqsOffset
          );

      }
      RcDebugPrintWithDevice (
        SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, Strobe, Bit,
        "New TxDq PiPosition %d\n",
        PiPosition[Bit]
        );
    } // Bit loop
  } // if ((BitMax - BitMin) > MAX_DQ_GAP)

  return EFI_SUCCESS;
}

/*

  Enforces the maximum gap between bits within the BYTE, and if enabled also enforces the nibbles to be in the same TxDq UI
  Warning: This function must be called during 80b mode or Basic training execution

  @param[in]  Socket    Current socket under test (0-based)
  @param[in]  Ch        Current channel under test (0-based)
  @param[in]  SubCh     Current subchannel under test (0-based)
  @param[in]  Dimm      Current dimm under test (0-based)
  @param[in]  Rank      Current rank under test (0-based)
  @param[in]  PiPosition  Pointer with the information of each center position across ALL bits
  @param[in]  ResultsBit  Pointer with the margining information of each of bit
  @param[in]  UpdateTxDqsOriginalValue  TRUE: After executing the WA (*ChannelNvList)[ChannelLocal].TxDqsDelay is updated with the new value
                                        FALSE: (*ChannelNvList)[ChannelLocal].TxDqsDelay is not modified

*/
VOID
TxDqWA (
  UINT8     Socket,
  UINT8     Ch,
  UINT8     SubCh,
  UINT8     Dimm,
  UINT8     Rank,
  PerBitPi  *PiPosition,
  IN struct bitMarginCh  *ResultsBit,   OPTIONAL
  BOOLEAN   UpdateTxDqsOriginalValue
  )
{
#ifdef DDR5_SUPPORT
  PSYSHOST            Host;
  UINT8               Bit;
  UINT8               BitIdx;
  UINT8               Nibble;
  UINT8               MaxStrobe;
  UINT8               MSVx4;
  UINT8               MBV;
  INT16               BitMin;
  INT16               BitMax;
  UINT16              PiPositionByte[MAX_BITS_IN_BYTE];
  struct baseMargin   ResultsByte[MAX_BITS_IN_BYTE];
  struct baseMargin   *ResultsBytePtr;


  Host = GetSysHostPointer ();

  MSVx4 = GetMaxStrobeValid (Host);
  MBV   = GetMaxBitsValid (Host);

  if (IsBrsPresent (Host, Socket)) {

#ifdef DEBUG_CODE_BLOCK
    //Print LE and RE Per Bit Deskew Margins for current rank
    if ((GetDebugLevel () & SDBG_MAX) && ResultsBit != NULL) {
      DisplayREandLEMarginsDdr5 (Host, Socket, Ch, SubCh, Dimm, Rank, ResultsBit, TxDqDelay);
    }
#endif // DEBUG_CODE_BLOCK

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT,
      "Applying TxDqWA\n");

    MaxStrobe = GetMaxStrobe (Host, Socket, Ch, Dimm, WrLvlDelay, DdrLevel);

    //
    // Loop for each byte
    //
    for (Nibble = 0; Nibble < MSVx4; Nibble += 2) {
      //
      // Skip if this is an ECC nibble when ECC is disabled
      // Note: unlike usual in training code, per-bit skew register are mapped linerly, so ECC bits are on the end
      //
      if (IsNibbleNotValid (Host, Nibble)) {
        continue;
      }
      for (Bit = 0; Bit < MAX_BITS_IN_BYTE; Bit++) {
        //
        // Get the absolute bit index (0-79)
        //
        BitIdx = (Nibble * BITS_PER_NIBBLE) + Bit;

        PiPositionByte[Bit] = (*PiPosition).PiPosition[BitIdx];
        if (ResultsBit != NULL) {
          ResultsByte[Bit] = ResultsBit->bits[Ch][BitIdx];
          ResultsBytePtr   = ResultsByte;
        } else {
          ResultsBytePtr   = NULL;
        }

      }
      //
      // Enforce all bits within a BYTE do not exceed the maximum allowed gap
      //
      TxDqMaxSkewGapEnforcement (Host, Socket, Ch, SubCh, Dimm, Rank, Nibble, PiPositionByte, ResultsBytePtr, &BitMin, &BitMax);

      if (UpdateTxDqsOriginalValue) {
        //
        // Update original TxDqs delay to minimize impact on margin
        //
        UpdateTxDqsOriginal (Host, Socket, Ch, SubCh, Dimm, Rank, Nibble);
      }

      for (Bit = 0; Bit < MAX_BITS_IN_BYTE; Bit++) {
        //
        // Get the absolute bit index (0-79)
        //
        BitIdx = (Nibble * BITS_PER_NIBBLE) + Bit;

        (*PiPosition).PiPosition[BitIdx]  = PiPositionByte[Bit];
        if (ResultsBit != NULL) {
          ResultsBit->bits[Ch][BitIdx]      = ResultsBytePtr[Bit];
        }
      }
    } // nibble loop
  } // IsBrsPresent
#endif // #ifdef DDR5_SUPPORT
} // TxDqWA

/*

  Enforces the maximum gap between bits within the BYTE, and if enabled also enforces the nibbles to be in the same TxDq UI
  Warning: This function must be called during 40b mode or Adv training execution

  @param[in]  Socket    Current socket under test (0-based)
  @param[in]  Ch        Current channel under test (0-based)
  @param[in]  SubCh     Current subchannel under test (0-based)
  @param[in]  Dimm      Current dimm under test (0-based)
  @param[in]  Rank      Current rank under test (0-based)
  @param[in]  UpdateTxDqsOriginalValue  TRUE: After executing the WA (*ChannelNvList)[ChannelLocal].TxDqsDelay is updated with the new value
                                        FALSE: (*ChannelNvList)[ChannelLocal].TxDqsDelay is not modified

*/
VOID
TxDqWaAdv (
  IN        UINT8     Socket,
  IN        UINT8     Ch,
  IN        UINT8     SubCh,
  IN        UINT8     Dimm,
  IN        UINT8     Rank,
  IN        BOOLEAN   UpdateTxDqsOriginalValue
  )
{
#ifdef DDR5_SUPPORT
  PSYSHOST  Host;
  PerBitPi  PiPosition[MAX_CH];
  UINT8     Strobe;
  UINT8     Bit;
  UINT8     MBV;
  INT16     *NibbleBit;

  Host = GetSysHostPointer ();

  MBV = GetMaxBitsValid (Host);

  if (!IsBrsPresent (Host, Socket)) {
    return;
  }

  for (Bit = 0; Bit < MBV; Bit++) {

    if ((Bit % 4) == 0) {
      RestoreTxDqsOriginal (Host, Socket, Ch, SubCh, Dimm, Rank, Bit / 4);
    }

    Strobe = GetStrobeFromNibble (Host, Bit / 4);

    NibbleBit = &PiPosition[Ch].PiPosition[Bit];

    GetSetDataGroup (
      Host, Socket, Ch, SubCh, Dimm, Rank, Strobe, Bit % 4,
      DdrLevel, TxDqBitDelay, GSM_READ_ONLY,
      NibbleBit
      );
  }

  TxDqWA (Socket, Ch, 0, Dimm, Rank, &PiPosition[Ch], NULL, UpdateTxDqsOriginalValue);
#endif // #ifdef DDR5_SUPPORT
}
/**
  Save all the RxStrobeInv data to a buffer that is allocated by this routine
  and return a pointer to the buffer

  @Retval ChipDataPtr - A pointer that used to pass per strobe RxStrobeInv settings
**/
VOID *
EFIAPI
SaveRxStrobeInvIntoANewBuffer (
  VOID
  )
{
  PSYSHOST                            Host;
  UINT8                               Socket;
  UINT8                               Ch;
  UINT8                               Dimm;
  UINT8                               Rank;
  struct channelNvram                 (*ChannelNvList)[MAX_CH];
  struct dimmNvram                    (*DimmNvList)[MAX_DIMM];
  INT16                               (*RxStrobeInvSaveArea)[MAX_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_STROBE];
  INT16                               RxStrobeInvData = 0;
  UINT8                               MaxStrobe;
  UINT8                               Strobe;
  UINT8                               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;
  ChannelNvList = GetChannelNvList (Host, Socket);

  //Save off RxStrobeInvert values using GetSet functions, then
  //set RxStrobeInvert to 0 using GetSet all strobes/ranks
  RxStrobeInvSaveArea = RcAllocatePool ( sizeof (*RxStrobeInvSaveArea));
  if (RxStrobeInvSaveArea == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return NULL; // Not reachable code, just KW workaround
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        // Continue to the next rank if this one is disabled
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        MaxStrobe = GetMaxStrobe (Host, Socket, Ch, Dimm, DdrLevel, RxStrobeInvert);
        for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if (IsStrobeNotValid (Host, Strobe)) {
            continue;
          }
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RxStrobeInvert, GSM_READ_CSR + GSM_READ_ONLY,
            &RxStrobeInvData);
          (*RxStrobeInvSaveArea)[Ch][Dimm][Rank][Strobe] = RxStrobeInvData;
        } //Strobe
      } //Rank
    } //Dimm
  } //Ch
  return (VOID *)RxStrobeInvSaveArea;
} //SaveRxStrobeInvIntoANewBuffer


/**
  Clear all the RxStrobeInv bits for the current socket
**/
VOID
EFIAPI
ClearAllRxStrobeInvForCurrentSocket (
  VOID
  )
{
  PSYSHOST                            Host;
  UINT8                               Socket;
  UINT8                               Ch;
  UINT8                               Dimm;
  UINT8                               Rank;
  struct channelNvram                 (*ChannelNvList)[MAX_CH];
  struct dimmNvram                    (*DimmNvList)[MAX_DIMM];
  INT16                               RxStrobeInvInitVal = 0;
  UINT8                               MaxStrobe;
  UINT8                               Strobe;
  UINT8                               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        // Continue to the next rank if this one is disabled
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        MaxStrobe = GetMaxStrobe (Host, Socket, Ch, Dimm, DdrLevel, RxStrobeInvert);
        for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if (IsStrobeNotValid (Host, Strobe)) {
            continue;
          }
          RxStrobeInvInitVal = 0;
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RxStrobeInvert, GSM_UPDATE_CACHE,
            &RxStrobeInvInitVal);
        } //Strobe
      } //Rank
    } //Dimm
  } //Ch
} //ClearAllRxStrobeInvForCurrentSocket

/**
  Restore all the RxStrobeInv data from a buffer and then free the buffer space

  @param[in, out] * ChipDataPtr - A pointer that used to pass per strobe RxStrobeInv settings
**/
VOID
EFIAPI
RestoreRxStrobeInvAndFreeBuffer (
  IN OUT VOID          *ChipDataPtr
  )
{
  PSYSHOST                            Host;
  UINT8                               Socket;
  UINT8                               Ch;
  UINT8                               Dimm;
  UINT8                               Rank;
  struct channelNvram                 (*ChannelNvList)[MAX_CH];
  struct dimmNvram                    (*DimmNvList)[MAX_DIMM];
  INT16                               (*RxStrobeInvSaveArea)[MAX_CH][MAX_DIMM][MAX_RANK_DIMM][MAX_STROBE];
  INT16                               RxStrobeInvData;
  UINT8                               MaxStrobe;
  UINT8                               Strobe;
  UINT8                               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;
  ChannelNvList = GetChannelNvList (Host, Socket);

  RxStrobeInvSaveArea = (VOID *)ChipDataPtr;

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
        // Continue to the next rank if this one is disabled
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        MaxStrobe = GetMaxStrobe (Host, Socket, Ch, Dimm, DdrLevel, RxStrobeInvert);
        for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if (IsStrobeNotValid (Host, Strobe)) {
            continue;
          }
          RxStrobeInvData = (*RxStrobeInvSaveArea)[Ch][Dimm][Rank][Strobe];
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RxStrobeInvert, GSM_UPDATE_CACHE,
            &RxStrobeInvData);
        } //Strobe
      } //Rank
    } //Dimm
  } //Ch
  RcFreePool (ChipDataPtr);
} //SaveRxStrobeInvIntoANewBuffer

/**
  Perform any silicon specific initialization required before Recieve Enable Training

  @param[in, out] ** ChipDataPtr - A pointer to a pointer can be used to pass data

  @retval EFI_SUCCESS if init done
          !EFI_SUCCESS if other errors were encountered
**/

EFI_STATUS
EFIAPI
SaveAndInitChipRxEnableParams (
  IN OUT VOID                               **ChipDataPtr
  )
{
  PSYSHOST                            Host;
  UINT8                               Socket;

  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;

  *ChipDataPtr = NULL;
  //IF DFE is enabled, save restore RxStrobeInv
  if (GetRxDfeEnableChip (Host, Socket)) {
    //Save off RxStrobeInv bit values using GetSet functions
    *ChipDataPtr = SaveRxStrobeInvIntoANewBuffer ();
    //set all RxStrobeInv to "0"
    ClearAllRxStrobeInvForCurrentSocket ();
  } // RxDfe Enable Check
  return EFI_SUCCESS;
}

/**
  Perform any silicon specific initialization required after Recieve Enable Training

  @param[in, out] ** ChipDataPtr - A pointer to a pointer can be used to pass data

  @retval EFI_SUCCESS if init done
          !EFI_SUCCESS if other errors were encountered
**/
EFI_STATUS
EFIAPI
RestoreChipRxEnableParams (
  IN OUT VOID                               **ChipDataPtr
  )
{
  PSYSHOST                            Host;
  UINT8                               Socket;

  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;

  //IF DFE is enabled, save restore RxStrobeInv
  if (GetRxDfeEnableChip (Host, Socket)) {
    if (*ChipDataPtr == NULL) {
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_169);
      return EFI_INVALID_PARAMETER; // Not reachable code, just KW workaround
    }
    //Restore RxStrobeInv bit values
    RestoreRxStrobeInvAndFreeBuffer (*ChipDataPtr);
  } // RxDfe Enable Check
  return EFI_SUCCESS;
}

/**
  Initialize Tx Echo Cancellation feature.

  @param[in] Socket   - Socket number

  @retval  N/A
**/
VOID
EFIAPI
InitTxEchoCancellation (
  IN UINT8 Socket
 )
{
  //NULL Implementation
}

/**
  Initialize Legacy RxDfe feature.

  @param[in] Socket   - Socket number

  @retval  N/A
**/
VOID
EFIAPI
InitLegacyRxDfe (
  IN UINT8 Socket
 )
{
  //NULL Implementation
}

#ifdef LRDIMM_SUPPORT
/**

  Get/Set Data Buffer DFE Coefficient

  @param[in] Host          - Pointer to sysHost
  @param[in] Socket        - Socket number (0-based)
  @param[in] Channel       - Channel number (0-based)
  @param[in] Dimm          - DIMM number (0-based)
  @param[in] Rank          - Rank number (0-based)
  @param[in] Strobe        - Strobe number (0-based)
  @param[in] Bit           - Bit number (0-based)
  @param[in] Level         - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in] Group         - CSM_GT - Parameter to program
  @param[in] Mode          - Bit-field of different modes
  @param[in][out] *Value   - Pointer to Tap value

  @retval SUCCESS

**/
MRC_STATUS
GetSetDataBufferDfeTapPerBit (
  IN     PSYSHOST    Host,
  IN     UINT8       Socket,
  IN     UINT8       Channel,
  IN     UINT8       Dimm,
  IN     UINT8       Rank,
  IN     UINT8       Strobe,
  IN     UINT8       Bit,
  IN     MRC_LT      Level,
  IN     MRC_GT      Group,
  IN     UINT8       Mode,
  IN OUT INT16       *Value
  )
{
#ifndef DDR5_SUPPORT
  UINT8                                     UpdateNeeded;
  UINT8                                     *ControlWordDataPtr;
  UINT8                                     ControlWordAddr;
  INT16                                     Temp;
  UINT16                                    CurVal = 0;
  UINT16                                    PiDelay;
  UINT16                                    MaxLimit = 0;
  UINT16                                    MinLimit = 0;
  UINT16                                    UsDelay = 0;
  struct dimmNvram                          (*DimmNvList)[MAX_DIMM];
  struct dimmDevice                         (*ChdimmList)[MAX_DIMM];

  if ((Group != DbDfeTap1) && (Group != DbDfeTap2) && (Group != DbDfeTap3) && (Group != DbDfeTap4)) {
   RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
                        "Warning!!! Invalid Group = %d is given to GetSetDataBufferDfeTapPerBit\n", Group);
    return MRC_STATUS_GROUP_NOT_SUPPORTED;
  }

  DimmNvList = GetDimmNvList (Host, Socket, Channel);
  ChdimmList = &Host->var.mem.socket[Socket].channelList[Channel].dimmList;

  //
  // Enable DB DFE for all DBs
  //
  (*DimmNvList)[Dimm].Dfe_F2BCEX &= 0xF8;
  (*DimmNvList)[Dimm].Dfe_F2BCEX |= DFE_ENABLE_GLOBAL | DFE_WRITE_PER_DQ | Bit;
  WriteLrbuf (Host, Socket, Channel, Dimm, Rank, (*DimmNvList)[Dimm].Dfe_F2BCEX, LRDIMM_F2, LRDIMM_BCEx);

  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits (Host, Level, Group, &MinLimit, &MaxLimit, &UsDelay);

  /*
    DFE Tap Coefficient Setting and Decode:
    Encode - DFE Coeff setting
      0    - DA[7] = 1 and DA[3:0]= 1111b, -45 mV
      ...
      14   - DA[7] = 1 and DA[3:0]= 0001b,  -3 mV
      15   - DA[7] = 0 and DA[3:0]= 0000b,   0 mV
      16   - DA[7] = 0 and DA[3:0]= 0001b,   3 mV
      ...
      30   - DA[7] = 0 and DA[3:0]= 1111b,  45 mV
  */
  UpdateNeeded = 0;
  if (Mode & GSM_READ_CSR) {
    //
    // Read from "actual setting" cache
    //
    //Read F3BC[C:F][7] (0 is positive, 1 is negative)
    switch (Group) {
      case DbDfeTap1:
        //F3BCCx[0:3]
        CurVal = (*DimmNvList)[Dimm].LrBuf_DFECoef[Bit][0][Strobe];
        break;
      case DbDfeTap2:
        //F3BCDx[0:3]
        CurVal = (*DimmNvList)[Dimm].LrBuf_DFECoef[Bit][1][Strobe];
        break;
      case DbDfeTap3:
        //F3BCEx[0:3]
        CurVal = (*DimmNvList)[Dimm].LrBuf_DFECoef[Bit][2][Strobe];
        break;
      case DbDfeTap4:
        //F3BCFx[0:3]
        CurVal = (*DimmNvList)[Dimm].LrBuf_DFECoef[Bit][3][Strobe];
        break;
      default:
        break;
    } //switch group

    //
    // Encoding DFE setting for training use
    //
    if (CurVal & DFE_TAP_NEG){
      CurVal = 0xF - (CurVal & 0xF);
    } else {
      CurVal = (CurVal & 0xF) + 0xF;
    }
  } else {
    //
    // Read from setting cache
    //
    switch (Group) {
      case DbDfeTap1:
        //F3BCCx[0:3]
        CurVal = (*ChdimmList)[Dimm].CachedLrBuf_DFECoef[Bit][0][Strobe];
        break;
      case DbDfeTap2:
        //F3BCDx[0:3]
        CurVal = (*ChdimmList)[Dimm].CachedLrBuf_DFECoef[Bit][1][Strobe];
        break;
      case DbDfeTap3:
        //F3BCEx[0:3]
        CurVal = (*ChdimmList)[Dimm].CachedLrBuf_DFECoef[Bit][2][Strobe];
        break;
      case DbDfeTap4:
        //F3BCFx[0:3]
        CurVal = (*ChdimmList)[Dimm].CachedLrBuf_DFECoef[Bit][3][Strobe];
        break;
      default:
        break;
    } //switch group

    //
    // Encoding DFE setting for training use
    //
    if (CurVal & DFE_TAP_NEG){
      CurVal = 0xF - (CurVal & 0xF);
    } else {
      CurVal = (CurVal & 0xF) + 0xF;
    }
  }

  //
  // Check if read only
  //
  if (Mode & GSM_READ_ONLY) {
    *Value = CurVal;
  } else {
    if (Mode & GSM_WRITE_OFFSET) {
      Temp = CurVal + *Value;
    } else {
      Temp = *Value;
    }

    if (Temp >= MinLimit) {
      PiDelay = Temp;
    } else {
      PiDelay = MinLimit;
    }

    if (PiDelay > MaxLimit) {
      PiDelay = MaxLimit;
    }

    CurVal = PiDelay;
    if (CurVal < 0xF){
      CurVal = (0xF - CurVal) | DFE_TAP_NEG | DFE_TAP_ENABLE;
    } else {
      CurVal = (CurVal - 0xF)| DFE_TAP_ENABLE;
    }

    switch (Group) {
      case DbDfeTap1:
        if ((*DimmNvList)[Dimm].LrBuf_DFECoef[Bit][0][Strobe] != CurVal){
          UpdateNeeded = 1;
          (*DimmNvList)[Dimm].LrBuf_DFECoef[Bit][0][Strobe] = (UINT8) CurVal;
        }
        break;
      case DbDfeTap2:
        if ((*DimmNvList)[Dimm].LrBuf_DFECoef[Bit][1][Strobe] != CurVal){
          UpdateNeeded = 1;
          (*DimmNvList)[Dimm].LrBuf_DFECoef[Bit][1][Strobe] = (UINT8) CurVal;
        }
        break;
      case DbDfeTap3:
        if ((*DimmNvList)[Dimm].LrBuf_DFECoef[Bit][2][Strobe] != CurVal){
          UpdateNeeded = 1;
          (*DimmNvList)[Dimm].LrBuf_DFECoef[Bit][2][Strobe] = (UINT8) CurVal;
        }
        break;
      case DbDfeTap4:
        if ((*DimmNvList)[Dimm].LrBuf_DFECoef[Bit][3][Strobe] != CurVal){
          UpdateNeeded = 1;
          (*DimmNvList)[Dimm].LrBuf_DFECoef[Bit][3][Strobe] = (UINT8) CurVal;
        }
        break;
      default:
        break;
    }
  } // if read only

  //
  // Save to cache
  //
  if (Mode & GSM_UPDATE_CACHE) {
    switch (Group) {
      case DbDfeTap1:
        (*ChdimmList)[Dimm].CachedLrBuf_DFECoef[Bit][0][Strobe] = (*DimmNvList)[Dimm].LrBuf_DFECoef[Bit][0][Strobe];
        break;
      case DbDfeTap2:
        (*ChdimmList)[Dimm].CachedLrBuf_DFECoef[Bit][1][Strobe] = (*DimmNvList)[Dimm].LrBuf_DFECoef[Bit][1][Strobe];
        break;
      case DbDfeTap3:
        (*ChdimmList)[Dimm].CachedLrBuf_DFECoef[Bit][2][Strobe] = (*DimmNvList)[Dimm].LrBuf_DFECoef[Bit][2][Strobe];
        break;
      case DbDfeTap4:
        (*ChdimmList)[Dimm].CachedLrBuf_DFECoef[Bit][3][Strobe] = (*DimmNvList)[Dimm].LrBuf_DFECoef[Bit][3][Strobe];
        break;
      default:
        break;
    }
  }

  if (!(Mode & GSM_READ_ONLY) && ((UpdateNeeded) || (Mode & GSM_FORCE_WRITE))) {
    switch (Group) {
      case DbDfeTap1:
        ControlWordDataPtr = &((*DimmNvList)[Dimm].LrBuf_DFECoef[Bit][0][0]);
        ControlWordAddr = LRDIMM_BCCx;
        break;
      case DbDfeTap2:
        ControlWordDataPtr = &((*DimmNvList)[Dimm].LrBuf_DFECoef[Bit][1][0]);
        ControlWordAddr = LRDIMM_BCDx;
        break;
      case DbDfeTap3:
        ControlWordDataPtr = &((*DimmNvList)[Dimm].LrBuf_DFECoef[Bit][2][0]);
        ControlWordAddr = LRDIMM_BCEx;
        break;
      case DbDfeTap4:
        ControlWordDataPtr = &((*DimmNvList)[Dimm].LrBuf_DFECoef[Bit][3][0]);
        ControlWordAddr = LRDIMM_BCFx;
        break;
      default:
        break;
    }
    WriteLrbufPBA (Host, Socket, Channel, Dimm, 0, ControlWordDataPtr, LRDIMM_F3, ControlWordAddr, Strobe);
  }

  (*DimmNvList)[Dimm].Dfe_F2BCEX &= 0xF0;
  WriteLrbuf (Host, Socket, Channel, Dimm, Rank, (*DimmNvList)[Dimm].Dfe_F2BCEX, LRDIMM_F2, LRDIMM_BCEx);

  return MRC_STATUS_SUCCESS;
#else // !DDR5_SUPPORT
  return MRC_STATUS_FAILURE;
#endif // !DDR5_SUPPORT
} // GetSetDataBufferDfeTapPerBit
#endif  //LRDIMM


/**
  GetIpTcoSetting
    TCO settings to monotonically increase duty cycle adjustments are [95 - 65][0 - 31]
    This function undoes what GetLinearCntlTcoValue does.  It takes a normalized
    monotonically increasing value and encodes it based on the IP specific value.

    For example:
    input values    0,  1,  2,..., 30, 31, 32, 33,..., 63
    correspond to
    output values  95, 94, 93,..., 65,  0,  1,  2,..., 31

  @param[in]  LinearValue - Normalized Linear Control value

  @retval Correponding Setting based on the CSR definition

**/
UINT32
EFIAPI
GetIpTcoSetting (
  IN INT16  LinearValue
)
{
  return 0;
}

/**

  GetSet GetSetDcaTco

  @param[in]     Host    - Pointer to sysHost
  @param[in]     Socket  - Socket number
  @param[in]     Ch      - Channel number (0-based)
  @param[in]     SubCh   - Sub-Channel number (0-based)
  @param[in]     Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]     Group   - CSM_GT - Parameter to program
  @param[in]     Mode    - Bit-field of different modes
  @param[in,out] Value   - Pointer to delay Value or offset based on mode

  @retval MRC_STATUS_SUCCESS if no problem

**/
MRC_STATUS
GetSetDcaTco (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     SubCh,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  )
{
  return MRC_STATUS_NOT_FOUND;
}

/**

  Save the dataOffsetTrain value for subsequent boots.

  @param[in]  Host        - Pointer to sysHost

  @retval None

**/
VOID
EFIAPI
DataOffsetTrainSave (
  IN PSYSHOST          Host
  )
{

}

/**
  Initialize Chip Specific Tx DQ Scomp.

  @param[in] Host     - Pointer to sysHost, the system Host (root) structure
  @param[in] socket   - Socket number
  @param[in] ch       - Channel number
  @param[in] SubCh    - Sub channel number
  @param[in] Strobe   - Strobe number

  @retval         - EFI_SUCCESS

**/
MRC_STATUS
EFIAPI
InitializeDataScompChip (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     SubCh,
  IN  UINT8     Strobe
  )
{

  return MRC_STATUS_SUCCESS;
}

/**
  Get or Set Tx DQ Scomp Slew Rate Up or Down Adjuster.

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket number
  @param ch       - Channel number
  @param SubCh    - Sub channel number
  @param mode     - GSM_READ_CSR - Read the data from hardware and not cache
                  -  GSM_READ_ONLY - Do not write
                  -  GSM_WRITE_OFFSET - Write offset and not value
                  -  GSM_FORCE_WRITE - Force the write
  @param UpDown   - TRUE  - Set Scomp Up Adj.
                  - FALSE - Set Scomp Down Adj.
  @param value    - Value to program

  @retval Pi delay value

**/
MRC_STATUS
EFIAPI
GetSetDataScompUpDownAdj (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Strobe,
  IN      UINT8     Mode,
  IN      BOOLEAN   UpDown,
  IN OUT  UINT8     *Value
  )
{

  return MRC_STATUS_SUCCESS;
}

/**
  Restore Tx DQ Slew Rate comp of channel.

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket number
  @param Channel  - Channel number

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
EFIAPI
RestoreDqSlewRate (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{

  return MRC_STATUS_SUCCESS;
}
