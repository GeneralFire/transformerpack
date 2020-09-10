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
#include <Memory/CpgcDefinitions.h>
#include "Chip10nm/Include/MemHostChip10nm.h"
#include <Library/MemDdrioIpLib.h>
#include <Library/ProcMemInitChipLib.h>

#include "Include/MemIoControl.h"
#include "Include/MemDdrioRegs.h"
#include "Include/MemDdrioPrivate.h"
#include <UncoreCommonIncludes.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/RcMemAllocLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CheckpointLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/IoAccessLib.h>
#include <Library/CteNetLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemDdrIoIpTargetLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemRoutingLib.h>
#include <Library/MemoryServicesLib.h>
#include <Memory/Ddr5RcwRegs.h>
#include <Memory/Ddr5Cmds.h>
#include <Library/Ddr5CoreLib.h>
#include <Memory/JedecDefinitions.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Library/EmulationConfigurationLib.h>
#include <MemProjectSpecific.h>
#include <Library/MemRcLib.h>
#include "MemCmdControl.h"
#include <Include/MemDdrioSpecific.h>
#include <Library/TimerLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/SpdDecodeLib.h>
#include <Library/SwizzleLib.h>

#define SENSEAMP_DELAY_MAX       8
#define SENSEAMP_DELAY_CONVERT_NEGATIVE_SETTING 16

#define RSPA_D0_SCA_PAD 16
#define RSPA_D1_SCA_PAD 15
#define RSPB_D1_SCB_PAD 15
#define RSPB_D0_SCB_PAD 16

#define RX_PREAMBLE_1TCK       0
#define RX_PREAMBLE_2TCK_0010  1
#define RX_PREAMBLE_2TCK_1110  2
#define RX_PREAMBLE_3TCK       3
#define RX_PREAMBLE_4TCK       4

#define TX_PREAMBLE_LEN_2TCK 2
#define TX_PREAMBLE_LEN_3TCK 3
#define TX_PREAMBLE_LEN_4TCK 4

#define TX_PREAMBLE_PAT_2TCK_0111 7
#define TX_PREAMBLE_PAT_2TCK_0010 4
#define TX_PREAMBLE_PAT_3TCK_01_0000 0x10
#define TX_PREAMBLE_PAT_3TCK_01_0100 0x14
#define TX_PREAMBLE_PAT_4TCK 0x50

#define MRC_ROUND_TRIP_IO_COMP_START_DDR5        20  // Roundtrip - IO compensation starting point for DDR5
#define MRC_ROUND_TRIP_IO_COMP_START_DDR4        19  // Roundtrip - IO compensation starting point for DDR4
#define MRC_ROUND_TRIP_IO_COMPENSATION_DDR5      20  // Roundtrip - IO compensation value for DDR5
#define MRC_ROUND_TRIP_IO_COMPENSATION_DDR4      19  // Roundtrip - IO compensation value for DDR4

#define SAMPLER_FEEDBACK_DELAY 60
#define MAX_CWL_ADJ_BETWEEN_TWO_SUB_CHANEELS     8
#define DDR_TCWL_ADJ_SIGN_BIT                    BIT3

/**
  Obtain pointer to DDRIO NVRAM data channel list

  @param[in]  Host    - Pointer to SysHost
  @param[in]  Socket  - Socket number

  @retval Pointer to DdrioNvData.Socket[Socket].ChannelList
**/
DDRIO_CHANNEL_NVDATA
(*GetDdrioChannelNvList (
  PSYSHOST  Host,
  UINT8     Socket
  ))[MAX_CH]
{
  DDRIO_NVDATA    *DdrioNvdata;

  DdrioNvdata = (VOID *)Host->nvram.mem.DdrioNvdata;

  return (&(DdrioNvdata->Socket[Socket].ChannelList));
}

/**
  Obtain pointer to DDRIO NVRAM data memory controller list

  @param[in]  Host    - Pointer to SysHost
  @param[in]  Socket  - Socket number

  @retval Pointer to DdrioNvData.Socket[Socket].Imc
**/
DDRIO_IMC_NVDATA
(*GetDdrioImcNvList (
  PSYSHOST  Host,
  UINT8     Socket
  ))[MAX_IMC]
{
  DDRIO_NVDATA    *DdrioNvdata;

  DdrioNvdata = (VOID *)Host->nvram.mem.DdrioNvdata;

  return (&(DdrioNvdata->Socket[Socket].Imc));
}

/**
  Program x8_device

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket number

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
ProgramX8Device (
  IN PSYSHOST  Host,
  IN  UINT8    Socket
  );

/**
  Get Sense Amp ODT Delays Duration Constant

  @param[in]  Socket      Socket number

  @retval DDR Sense Amp ODT Delays Duration Constant value
**/
UINT8
GetDurationConstant (
  IN      UINT8       Socket
  )
{
  PSYSHOST        Host;
  UINT8           TempDurationData;

  Host = GetSysHostPointer ();
  //
  // Default Duration Data
  //
  TempDurationData = DDR_3200_DURATION_CONSTANT;

  if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_1866) {
    TempDurationData = DDR_1867_DURATION_CONSTANT;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_2133) {
    TempDurationData = DDR_2133_DURATION_CONSTANT;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_2400) {
    TempDurationData = DDR_2400_DURATION_CONSTANT;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_2666) {
    TempDurationData = DDR_2667_DURATION_CONSTANT;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_2933) {
    TempDurationData = DDR_2933_DURATION_CONSTANT;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_3200) {
    TempDurationData = DDR_3200_DURATION_CONSTANT;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_3600) {
    TempDurationData = DDR_3600_DURATION_CONSTANT;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_4000) {
    TempDurationData = DDR_4000_DURATION_CONSTANT;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_4400) {
    TempDurationData = DDR_4400_DURATION_CONSTANT;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_4800) {
    TempDurationData = DDR_4800_DURATION_CONSTANT;
  }

  return TempDurationData;

}

/**

  Set Sense Amp ODT Delays for DDR5

  @param[in]  Socket      Socket number

**/
VOID
SetSenseAmpODTDelaysDdr5 (
  IN      UINT8       Socket
  )
{
  PSYSHOST                             Host;
  UINT8                                Ch, Dimm, Rank;
  UINT8                                StrobeIndex;
  UINT8                                Strobe;
  INT16                                Delay;
  INT16                                DqsSenseAmpDelay;
  INT16                                DqSenseAmpDelay;
  UINT32                               DqsSenseAmpDuration;
  UINT32                               DqSenseAmpDuration;
  INT16                                MaxRcven;
  INT16                                MinRcven;
  UINT16                               RcvenDelta;
  UINT8                                SubCh = 0;
  UINT8                                SubChMSVx4;
  UINT8                                MaxChDdr;
  UINT8                                DurationConstant;
  UINT8                                ReadPreamble;
  UINT8                                ReadPreambleLen = 0;
  UINT8                                ReadPostambleLen = 0;
  struct channelNvram                  (*ChannelNvList)[MAX_CH];
  DDRIO_CHANNEL_NVDATA                 (*DdrioChannelNvList) [MAX_CH];
  struct dimmNvram                     (*DimmNvList)[MAX_DIMM];
  DDRD_N0_RX_CTL0_MCIO_DDRIO_STRUCT    DdrdN0RxCtl0;
  DDRD_N0_TRAIN_CTL0_MCIO_DDRIO_STRUCT DdrdN0TrainCtl0;
  DDRD_N0_TRAIN_CTL3_MCIO_DDRIO_STRUCT DdrdN0TrainCtl3;

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  ChannelNvList = GetChannelNvList (Host, Socket);
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  RcDebugPrint (SDBG_MAX, "\n Starting Senseamp and ODT delay and duration calculations \n");
  RcDebugPrint (SDBG_MAX, "------------------------------------------------------------ \n");

  ReadPreamble = GetPreambleState (Socket, READ_PREAMBLE);
  switch (ReadPreamble) {
  case PREAMBLE_1TCLK:
    ReadPreambleLen = 1;
    break;
  case PREAMBLE_2TCLK:
    ReadPreambleLen = 2;
    break;
  case PREAMBLE_3TCLK:
    ReadPreambleLen = 3;
    break;
  case PREAMBLE_4TCLK:
    ReadPreambleLen = 4;
    break;
  default:
    ReadPreambleLen = 1;
    break;
  }

  DurationConstant = GetDurationConstant (Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      for (StrobeIndex = 0; StrobeIndex < SubChMSVx4; StrobeIndex++) {
        //
        // Skip if this is an ECC strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, StrobeIndex)) {
          continue;
        }

        Strobe = StrobeIndex + SubCh * SubChMSVx4;
        //
        // Clear out min and max values
        //
        MaxRcven = 0;
        MinRcven = 0xfff;
        for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
          for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
            if (CheckRank(Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
              continue;
            }
            GetSetDataGroup (Host, Socket, Ch, SubCh, Dimm, Rank, Strobe, 0, DdrLevel, RecEnDelay, GSM_READ_ONLY, &Delay);
            if (Delay < MinRcven) {
              MinRcven = Delay;
            }
            if (Delay > MaxRcven) {
              MaxRcven = Delay;
            }
          } // Rank
        } // Dimm

        //
        // rx_postamble_len = 0 or 1 based on the selected mode for postamble
        //   0 or 1 for DDR5 (DDR5 has selectable post-amble of 0.5 and 1.5 tCK)
        //
        DdrdN0RxCtl0.Data = (*DdrioChannelNvList)[Ch].DdrDataRxCtl0[Strobe];
        if (DdrdN0RxCtl0.Bits.rx_postamble_len == 0) {
          ReadPostambleLen = 1; // rx_postamble_len = 0 means 0.5 tCK postamble. Round up 0.5 DCLK to 1 DCLK integer.
        } else {
          ReadPostambleLen = 2; // rx_postamble_len = 1 means 1.5 tCK postamble. Round up 1.5 DCLK to 2 DCLK integer.
        }

        //
        // calculate delays
        // Sense Amp Delay (in DCLK)
        // dqs_sa_dly[byte] = min(rcven_logic_dly)
        //
        DqsSenseAmpDelay = MinRcven / DCLKPITICKS;

        //
        // Sense Amp Duration (in DCLK)
        //   dqs_sa_dur[byte] = max(rcven_logic_dly) - min(rcven_logic_dly) + rx_preamble_len + rx_postamble_len + DurationConstant
        //   dq_sa_dur[byte] = max(rcven_logic_dly) - min(rcven_logic_dly) + DurationConstant
        //
        RcvenDelta = MaxRcven - MinRcven;
        RcvenDelta = ALIGN_VALUE (RcvenDelta, DCLKPITICKS); // Round up RcvEn delta to DCLKPITICKS alignment.
        DqsSenseAmpDuration = (RcvenDelta / DCLKPITICKS) + ReadPreambleLen+ ReadPostambleLen + DurationConstant;
        DqSenseAmpDuration = (RcvenDelta / DCLKPITICKS) + DurationConstant;

        if (DqsSenseAmpDelay < 0) {
          //
          // Convert negative number to 10nm senseampdelay bit field encoding of settings, this code assumes
          // the largest negative number that can result from the initial Senseampdelay formula is -3.
          //
          DqsSenseAmpDelay += SENSEAMP_DELAY_CONVERT_NEGATIVE_SETTING;
        } else if (DqsSenseAmpDelay > SENSEAMP_DELAY_MAX) {
          DqsSenseAmpDuration += ((UINT32)DqsSenseAmpDelay - SENSEAMP_DELAY_MAX);
          DqsSenseAmpDelay = SENSEAMP_DELAY_MAX;
        }

        //
        // dq_sa_dly[byte] = dqs_sa_dly[byte] + rx_preamble_len
        //
        DqSenseAmpDelay = DqsSenseAmpDelay + ReadPreambleLen;

        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubCh, NO_RANK, StrobeIndex, NO_BIT,
          "ReadPreambleLen = %d, ReadPostambleLen = %d, MinRcven=%3d, MaxRcven=%3d: DqsSenseAmpDelay=%d DqsSenseAmpDuration=%d\n",
          ReadPreambleLen, ReadPostambleLen, MinRcven, MaxRcven, DqsSenseAmpDelay, DqsSenseAmpDuration);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubCh, NO_RANK, StrobeIndex, NO_BIT,
          "ReadPreambleLen = %d, ReadPostambleLen = %d, MinRcven=%3d, MaxRcven=%3d: DqSenseAmpDelay=%d DqSenseAmpDuration=%d\n",
          ReadPreambleLen, ReadPostambleLen, MinRcven, MaxRcven, DqSenseAmpDelay, DqSenseAmpDuration);

        //
        // Read cached values
        //
        DdrdN0TrainCtl0.Data = (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl0[Strobe];
        DdrdN0TrainCtl3.Data = (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl3[Strobe];

        //
        //modify with new values
        //
        DdrdN0TrainCtl0.Bits.dqs_sa_dly        = (UINT32)DqsSenseAmpDelay;
        DdrdN0TrainCtl0.Bits.dqs_sa_dur        = (UINT32)DqsSenseAmpDuration;

        DdrdN0TrainCtl3.Bits.dq_sa_dly         = (UINT32)DqSenseAmpDelay;
        DdrdN0TrainCtl3.Bits.dq_sa_dur         = (UINT32)DqSenseAmpDuration;

        //
        // write new values
        //
        (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl0[Strobe] =  DdrdN0TrainCtl0.Data;
        (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl3[Strobe] =  DdrdN0TrainCtl3.Data;
        MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL0_MCIO_DDRIO_REG), DdrdN0TrainCtl0.Data);
        MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL3_MCIO_DDRIO_REG), DdrdN0TrainCtl3.Data);
      } // StrobeIndex loop
    } // Subch loop
  } // Ch loop
  RcDebugPrint (SDBG_MAX, "\n\n");
} //SetSenseAmpODTDelaysDdr5

/**

  This routine is run immediately after DDR training

  @param Host  - Pointer to sysHost

  @retval N/A

**/
VOID
PostTrainingInitDdrio (
  PSYSHOST Host
  )
{
  DDRIO_CHANNEL_NVDATA                  (*DdrioChannelNvList)[MAX_CH];
  UINT8                                 Ch = 0;
  UINT8                                 Socket = Host->var.mem.currentSocket;
  UINT8                                 MaxChDdr;
  BOOLEAN                               Ddr5IsPresent;

  MaxChDdr      = GetMaxChDdr ();
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);
  Ddr5IsPresent = IsDdr5Present (Host, Socket);

  SetTrainingModeDdr5 (Socket, TmPostTraining);
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    if (Ddr5IsPresent) {
      ProgramDqsIntervalControl (Host, Socket, Ch, DDR5_DQS_INTERVAL_TIMER_RUNTIME);
    }
  } // Ch
}

/** Get the DDRINTF_DATA_TIMING0*_CH* register offset for a particular channel and DIMM.

  This additionally provides bounds checking for the channel and DIMM values.

  @param[in]  Channel             The target channel number on the memory controller
  @param[in]  SubCh               The target subchannel number on the memory controller
  @param[in]  Dimm                The target DIMM number
  @param[out] RoundTripRegOffset  The DDRCRINTFROUNDTRIP*_CH* register offset for the target channel and DIMM

  @retval EFI_SUCCESS   Register offset was successfully determined
  @retval !EFI_SUCCESS  Register offset was not successfully determined

**/
EFI_STATUS
GetDataTimingRegOffset (
  IN      UINT8   MemoryControllerChannel,
  IN      UINT8   SubCh,
      OUT UINT32  *DataTimingRegOffset
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT8       Index;
  UINT32      DataTiming0Ch[] = { DDRINTF_DATA_TIMING0_CH_0_MCIO_DDRIO_REG, // CH0A
                                  DDRINTF_DATA_TIMING0_CH_1_MCIO_DDRIO_REG, // CH0B
                                  DDRINTF_DATA_TIMING0_CH_2_MCIO_DDRIO_REG, // CH1A
                                  DDRINTF_DATA_TIMING0_CH_3_MCIO_DDRIO_REG  // CH1B
                                };

  Index = MemoryControllerChannel * 2 + SubCh;

  if (Index >= ((sizeof DataTiming0Ch) / (sizeof DataTiming0Ch[0]))) {
    RcDebugPrint (SDBG_ERROR, "\nGetDataTimingRegOffset: Ch(%d) or SubCh(%d) out of bounds\n", MemoryControllerChannel, SubCh);
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_92);
    Status = EFI_UNSUPPORTED;
  } else {
    *DataTimingRegOffset = DataTiming0Ch[Index];
  }

  return Status;
}

/**

  Set the CAS write latency for DDRT

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Socket number
  @param[in] Ch           - Channel number
  @param[in] Cwl          - CAS Write Latency value to set

  @retval N/A

**/
VOID
SetDdrtCwlDdrio (
  IN  PSYSHOST                  Host,
  IN  UINT8                     Socket,
  IN  UINT8                     Ch,
  IN  UINT32                    Cwl
  )
{
  UINT32                                      DataTimingRegOffset;
  DDRINTF_DATA_TIMING0_CH_0_MCIO_DDRIO_STRUCT DataTiming0Reg;
  UINT8                                       MemoryControllerChannel;
  UINT8                                       SubCh;
  UINT8                                       NumChPerMc;

  NumChPerMc = GetNumChannelPerMc ();

  MemoryControllerChannel = Ch % NumChPerMc;

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (EFI_SUCCESS != GetDataTimingRegOffset (MemoryControllerChannel, SubCh, &DataTimingRegOffset)) {
      continue;
    }
    DataTiming0Reg.Data = MemReadPciCfgEp (Socket, Ch, DataTimingRegOffset);
    DataTiming0Reg.Bits.ddrt_tcwl = Cwl;
    MemWritePciCfgEp (Socket, Ch, DataTimingRegOffset, DataTiming0Reg.Data);
  } // SubCh
}

/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
VOID
SetRankEnDdrio (
  PSYSHOST                  Host,
  UINT8                     Socket,
  UINT8                     Ch,
  UINT8                     RankEn
  )
{
  UINT8                                            Clk;
  DDRCLK_CTL0_PAIR_0_MCIO_DDRIOEXT_STRUCT          ClkCsr;
  DDRIO_CHANNEL_NVDATA                             (*DdrioChannelNvList) [MAX_CH];

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  for (Clk = 0; Clk < MIN(MAX_CLK_VALID_REG, MAX_CLK); Clk++) {
    ClkCsr.Data = MemReadPciCfgEp (Socket, Ch, GetClkRegOffset (Clk));
    ClkCsr.Bits.clk_pair_en = (RankEn & (1 << Clk)) ? 1:0;
    MemWritePciCfgEp (Socket, Ch, GetClkRegOffset (Clk), ClkCsr.Data);
    //
    // When DDRCLK_CTL0_PAIR_0_MCIO_DDRIOEXT_REG is updated, clkCsr below needs to be updated accordingly
    // Otherwise, clkCsr may be out of sync when calling GetSetClkDelay ()
    //
    (*DdrioChannelNvList)[Ch].ClkCsr[Clk] = ClkCsr.Data;
  }
}


/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
VOID
SetRxOffsetDdrio (
  PSYSHOST                  Host,
  UINT8                     Socket,
  UINT8                     Ch,
  UINT8                     Dimm,
  UINT8                     Rank
  )
{
  //
  //SPR_TODO_DDRIO.HSD 1507190591.
  //
}

/**

  Perform DDRIO functionality necessary to support higher command-to-data completion delay values

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Socket number
  @param[in] Ch           - Channel number

  @retval N/A

**/
VOID
ExtendCmdToDataCompletionDdrio (
  IN  PSYSHOST                  Host,
  IN  UINT8                     Socket,
  IN  UINT8                     Ch
  )
{
  //
  // Wave3 AEP DIMMs do not have ERID;
  //
}

/**
  Perform any silicon specific initialization required before executing
  Read Early Vref Centering.

  @retval N/A
**/
VOID
EFIAPI
ChipInitBeforeRdEarlyVrefCentering (
  VOID
  )
{
}

/**
  Perform any silicon specific initialization required before executing
  Write Early Vref Centering.

  @retval N/A
**/
VOID
ChipInitBeforeWrEarlyVrefCentering (
  VOID
  )
{
}

/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
VOID
SetIodirectionValidDdrio (
  PSYSHOST                  Host,
  UINT8                     Socket
  )
{
  UINT8     Ch;
  DDRCLK_TX_CTL1_MCIO_DDRIOEXT_STRUCT          ddrCRCtlTx;
  struct channelNvram (*channelNvList) [MAX_CH];
  UINT8     MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*channelNvList) [Ch].enabled == 0) {
      continue;
    }
    //
    // SPR_TODO_DDRIO  To use DDRCLK_TX_CTL1_MCIO_DDRIOEXT_REG or DDRCLK_TX_CTL1_MCIO_DDRIO_REG?
    //
    ddrCRCtlTx.Data = MemReadPciCfgEp (Socket, Ch, DDRCLK_TX_CTL1_MCIO_DDRIOEXT_REG);
    ddrCRCtlTx.Bits.io_dir_valid = 1;
    MemWritePciCfgEp (Socket, Ch, DDRCLK_TX_CTL1_MCIO_DDRIOEXT_REG, ddrCRCtlTx.Data);
  }
}

/**

  Configure Rx Path Ungate.

  @param[in] Socket - Socket Id
  @param[in] Value  - Value to program

  @retval  None

**/
VOID
EFIAPI
ConfigureRxPathUngate (
  IN UINT8                     Socket,
  IN UINT8                     Value
  )
{

  UINT8                                        Ch;
  UINT8                                        SubCh;
  DDRCC_RX_CTL0_MCIO_DDRIOEXT_STRUCT           DdrccRxCtl0;
  DDRCLK_RX_CTL0_MCIO_DDRIOEXT_STRUCT          DdrclkRxCtl0;
  struct channelNvram                          (*channelNvList) [MAX_CH];
  UINT8                                        MaxChDdr;
  PSYSHOST                                     Host;

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*channelNvList) [Ch].enabled == 0) {
      continue;
    }
    //
    // Set the rx_path_ungate as 1.
    //
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      DdrccRxCtl0.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_RX_CTL0_MCIO_DDRIOEXT_REG);
      DdrccRxCtl0.Bits.rx_path_ungate = Value;
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_RX_CTL0_MCIO_DDRIOEXT_REG, DdrccRxCtl0.Data);
    }
    //
    // Set the rx_path_ungate as 1 for clk fub.
    //
    DdrclkRxCtl0.Data = MemReadPciCfgEp (Socket, Ch, DDRCLK_RX_CTL0_MCIO_DDRIOEXT_REG);
    DdrclkRxCtl0.Bits.rx_path_ungate = Value;
    MemWritePciCfgEp (Socket, Ch, DDRCLK_RX_CTL0_MCIO_DDRIOEXT_REG, DdrclkRxCtl0.Data);

  }
  return;
}

/**

  Set Qlbd and Qlbs Io direction.

  @param[in] Host              - Pointer to sysHost, the system Host (root) structure struct
  @param[in] Socket            - Socket Id
  @param[in] RxPathUngateValue - Rx Path Ungate Value to program

  @retval  None

**/
VOID
EFIAPI
SetQlbdQlbsIodirection (
  IN PSYSHOST                  Host,
  IN UINT8                     Socket,
  IN UINT8                     RxPathUngateValue
  )

{

  UINT8     Ch;
  DDRCC_CTL0_BIT_15_MCIO_DDRIOEXT_STRUCT       DdrccCtlBit15;
  DDRCC_CTL0_BIT_16_MCIO_DDRIOEXT_STRUCT       DdrccCtlBit16;
  UINT8                                        SubCh;
  DDRCC_RX_CTL0_MCIO_DDRIOEXT_STRUCT           DdrccRxCtl0;

  struct channelNvram (*channelNvList) [MAX_CH];
  UINT8                                        MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*channelNvList) [Ch].enabled == 0) {
      continue;
    }
    //
    // Set the IO direction as input.
    //
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      DdrccCtlBit15.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_CTL0_BIT_15_MCIO_DDRIOEXT_REG);
      DdrccCtlBit15.Bits.io_dir = 0;
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_CTL0_BIT_15_MCIO_DDRIOEXT_REG, DdrccCtlBit15.Data);

      DdrccCtlBit16.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_CTL0_BIT_16_MCIO_DDRIOEXT_REG);
      DdrccCtlBit16.Bits.io_dir = 0;
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_CTL0_BIT_16_MCIO_DDRIOEXT_REG, DdrccCtlBit16.Data);

      DdrccRxCtl0.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_RX_CTL0_MCIO_DDRIOEXT_REG);
      DdrccRxCtl0.Bits.rx_path_ungate = RxPathUngateValue;
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_RX_CTL0_MCIO_DDRIOEXT_REG, DdrccRxCtl0.Data);
    }
  }
}


/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/

/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
VOID
SwitchToNormalModeDdrio (
  PSYSHOST                  Host,
  UINT8                     Socket
  )
{
}

/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
VOID
SetSelectRefreshDdrio (
  PSYSHOST                  Host,
  UINT8                     Socket
  )
{
  //
  // selectrefresh bit is removed in SPR.
  //
}

/**

  Get LV Mode

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param socket - Processor socket

  @retval (UINT8)ddrCRCompCtl0.Bits.lvmode

**/
UINT8
GetLvmode (
  PSYSHOST  Host,
  UINT8     socket
  )
{
  //
  //lvmode is removed in SPR.
  //
  return 0;
}

/**

  Program registers for tCL and tWL\tCWL for ICX

  @param  Hhst        - Ptr to SYSHOST, system Host (root) structure
  @param  socket      - Socket number
  @param  ch          - Channel number
  @param  nCL         - tCL value in nclks
  @param  nCWL        - tCWL value in nclks

  @retval None

**/
UINT32
SetDataTxOnDdrio (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT32    TxOn
  )
{
  //
  //SPR_TODO_DDRIO. HSD 1507190589
  //
  return 0;
}

/**

  Program registers for tCL and tWL\tCWL for ICX

  @param  Hhst        - Ptr to SYSHOST, system Host (root) structure
  @param  socket      - Socket number
  @param  ch          - Channel number
  @param  nCL         - tCL value in nclks
  @param  nCWL        - tCWL value in nclks

  @retval None

**/
UINT32
GetCachedDataRxOffsetDdrio (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT8     Rank,
  UINT8     Strobe,
  UINT8     Bit
  )
{
  DDRIO_CHANNEL_NVDATA                        (*DdrioChannelNvList) [MAX_CH];
  struct ddrRank                              (*rankList) [MAX_RANK_DIMM];
  DDRD_N0_SA_OFFSET_RANK_0_MC_DDRIOMC_STRUCT  rxOffset;
  UINT32                                      RetVal = 0;

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);
  rankList = GetRankNvList (Host, Socket, Ch, Dimm);

  rxOffset.Data = (*DdrioChannelNvList) [Ch].rxOffset[ (*rankList) [Rank].rankIndex][Strobe];
  switch (Bit) {
    case 0:
      RetVal = rxOffset.Bits.sa_offset0;
      break;
    case 1:
      RetVal = rxOffset.Bits.sa_offset1;
      break;
    case 2:
      RetVal = rxOffset.Bits.sa_offset2;
      break;
    case 3:
      RetVal = rxOffset.Bits.sa_offset3;
      break;
    default:
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL,  0xFF);
  }
  return RetVal;
}

/**

  Program registers for tCL and tWL\tCWL for ICX

  @param  Hhst        - Ptr to SYSHOST, system Host (root) structure
  @param  socket      - Socket number
  @param  ch          - Channel number
  @param  nCL         - tCL value in nclks
  @param  nCWL        - tCWL value in nclks

  @retval None

**/
UINT32
GetDataTrainFeedback (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     strobe
  )
{
  //
  //SPR_TODO_DDRIO. To check what's the replacement register in SPR?
  //
  return 0;
}

/**
  Set IOLatency in HW and update the cache if requested

  @param[in]  Host          - Pointer to sysHost
  @param[in]  Socket        - Processor to initialize
  @param[in]  Ch            - Channel number
  @param[in]  SubCh         - Sub channel number
  @param[out] IoLatIoComp   - Pointer to caller's storage

  @retval N/A
**/
VOID
SetIoLatencyIoComp (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     SubCh,
  UINT32    IoLatIoComp
  )
{
  DDRINTF_IO_LAT1_CH_0_MCIO_DDRIO_STRUCT    IoLatency1;
  UINT32                                    IoLatency1Ch[] = {DDRINTF_IO_LAT1_CH_0_MCIO_DDRIO_REG,
                                                              DDRINTF_IO_LAT1_CH_1_MCIO_DDRIO_REG,
                                                              DDRINTF_IO_LAT1_CH_2_MCIO_DDRIO_REG,
                                                              DDRINTF_IO_LAT1_CH_3_MCIO_DDRIO_REG
                                                             };
  struct channelNvram                       (*channelNvList) [MAX_CH];
  UINT8                                     Index;
  UINT8                                     MaxChDdr;
  UINT8                                     SwappedSubCh;

  MaxChDdr = GetMaxChDdr ();
  if (Ch >= MaxChDdr) {
    // invalid ch specified
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_125);
    return;
  }

  channelNvList = GetChannelNvList (Host, Socket);

  //
  // Update Sub Ch just in case lane reversal is enabled on current Ch.
  //
  SwappedSubCh = UpdateDdrIoCcRegisterSubCh (Host, Socket, Ch, SubCh);

  Index = (Ch*SUB_CH + SwappedSubCh) % MAX_MC_SCH;

  IoLatency1.Data = MemReadPciCfgEp (Socket, Ch, IoLatency1Ch[Index]);
  IoLatency1.Bits.io_lat_comp = IoLatIoComp;
  MemWritePciCfgEp (Socket, Ch, IoLatency1Ch[Index], IoLatency1.Data);

#ifdef DDR5_SUPPORT
  (*channelNvList) [Ch].ioLatency1[SubCh] = IoLatency1.Data;
#else
  (*channelNvList) [Ch].ioLatency1 = IoLatency1.Data;
#endif // #ifdef DDR5_SUPPORT

}

/**
  Set IOLatency in HW and update the cache if requested

  @param Host           - Point to sysHost
  @param socket         - Socket number
  @param ch             - Channel number
  @param ioLateIoComp   - Pointer to IOLatency

  @retval N/A
**/
UINT32
ReadTrainFeedback (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    strobe
  )
{
  UINT32 feedback = 0;
  return feedback;
} // ReadTrainFeedback

/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
VOID
SetChipGetCtlClkResLowerNibbleStrobeFeedback (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 strobe,
  UINT32* csrReg,
  UINT32* strobeFeedback
  )
{
  //Let compiler optimize if possible.
  UINT32 _strobeFeedback = *strobeFeedback;
  //
  // Common accross HSX, SKX and KNL
  //
#if 0 //SPR_TODO_DDRIO. Find the according register bitfield in SPR.
  *csrReg = MemReadPciCfgEp (socket, ch, UpdateIoRegister10nm (Host, 0, strobe, DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG));
#endif
  //
  // Lower nibble
  //
  if ((*csrReg & 0x1FF) > 0) {
    _strobeFeedback |= 1 << strobe;
    *strobeFeedback = _strobeFeedback;
  }
}

/**

  Program Read Preamble setting for DDR5.

  @param socket  - Processor to initialize

  @retval N/A

**/
VOID
EFIAPI
ProgramReadPreamble (
  UINT8    Socket
  )
{
  PSYSHOST                              Host;
  UINT8                                 Strobe;
  UINT8                                 Ch = 0;
  UINT8                                 MSVx4 = MAX_STROBE_DDR5;
  UINT8                                 MaxChDdr;
  DDRD_N0_RX_CTL0_MCIO_DDRIO_STRUCT     DdrdN0RxCtl0;
  DDRIO_CHANNEL_NVDATA                 (*DdrioChannelNvList) [MAX_CH];
  UINT8 PreambleLen = 0;
  UINT8 RxPreambleType = 0;

  Host = GetSysHostPointer ();
  if (!IsDdr5Present (Host, Socket)) {
    return;
  }
  MaxChDdr = GetMaxChDdr ();
  PreambleLen = GetPreambleState (Socket, READ_PREAMBLE);
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  switch (PreambleLen) {
    case PREAMBLE_1TCLK:
      RxPreambleType = RX_PREAMBLE_1TCK;
      break;
    case PREAMBLE_2TCLK:
      //
      // No DDR4 style in SPR.
      //
      RxPreambleType = RX_PREAMBLE_2TCK_0010;
      break;
    case PREAMBLE_3TCLK:
      RxPreambleType = RX_PREAMBLE_3TCK;
      break;
    case PREAMBLE_4TCLK:
      RxPreambleType = RX_PREAMBLE_4TCK;
      break;
    default:
      RxPreambleType = RX_PREAMBLE_2TCK_0010;
      break;
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }
      DdrdN0RxCtl0.Data = (*DdrioChannelNvList)[Ch].DdrDataRxCtl0[Strobe];
      DdrdN0RxCtl0.Bits.rx_preamble_type = RxPreambleType;
      (*DdrioChannelNvList)[Ch].DdrDataRxCtl0[Strobe] = DdrdN0RxCtl0.Data;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_RX_CTL0_MCIO_DDRIO_REG), DdrdN0RxCtl0.Data);
    }

  }
  return;
}

/**

  Restore Write Preamble Setting after write leveling training.

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
VOID
EFIAPI
RestoreWritePreambleWriteLeveling (
  PSYSHOST Host,
  UINT8    Socket
  )
{
  UINT8                                 Strobe;
  struct channelNvram                   (*ChannelNvList)[MAX_CH];
  UINT8                                 Ch = 0;
  UINT8                                 MSVx4 = MAX_STROBE_DDR5;
  UINT8                                 MaxChDdr;
  DDRD_N0_TX_CTL1_MCIO_DDRIO_STRUCT     DdrdN0TxCtl1;
  UINT8 PreambleLen = 0;
  UINT8 TxPreambleLength = 2;
  UINT8 TxPreamblePat = 4;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  PreambleLen = GetPreambleState (Socket, WRITE_PREAMBLE);

  if (PreambleLen == PREAMBLE_2TCLK) {
    return;
  } else {
    switch (PreambleLen) {
      case PREAMBLE_1TCLK:
        RcDebugPrint (SDBG_MAX, "\n Warning: DDR5 Write Preamble Length is not valid \n");
        break;
      case PREAMBLE_3TCLK:
        TxPreambleLength = TX_PREAMBLE_LEN_3TCK ;
        //
        // 01_0100 is another option.
        //
        TxPreamblePat = TX_PREAMBLE_PAT_3TCK_01_0000;
        break;
      case PREAMBLE_4TCLK:
        TxPreambleLength = TX_PREAMBLE_LEN_4TCK;
        TxPreamblePat = TX_PREAMBLE_PAT_4TCK;
        break;
      default:
        TxPreambleLength = TX_PREAMBLE_LEN_2TCK;
        TxPreamblePat = TX_PREAMBLE_PAT_2TCK_0010;
        break;
    }

    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((*ChannelNvList) [Ch].enabled == 0) {
        continue;
      }
      for (Strobe = 0; Strobe < MSVx4; Strobe++) {
        if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
          continue;
        }
        DdrdN0TxCtl1.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TX_CTL1_MCIO_DDRIO_REG));
        DdrdN0TxCtl1.Bits.tx_preamble_len = TxPreambleLength;
        DdrdN0TxCtl1.Bits.tx_preamble_pat = TxPreamblePat;
        MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TX_CTL1_MCIO_DDRIO_REG), DdrdN0TxCtl1.Data);
      }
    }
  } //else
  return;
}

/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
UINT32
LrDimmReadTrainFeedback (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    strobe,
  UINT8    bit
  )
{
  UINT32 csrReg = 0;
  UINT32 feedback = 0;

  // SKX HSD 4928153 DQ swizzle, normally this would be a straight up read but on SKX the bits are swizzled
  // nibble0 bits 0,1 are located in nibble1 bits 0,1 and vise-versa. Bits 2,3 for nibble0 and nibble1 are where they are supposed to be.
  //csrReg = ((MemReadPciCfgEp (socket, ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG)) & 0xc) |
  //        (MemReadPciCfgEp (socket, ch, UpdateIoRegisterCh10nm (Host, (strobe + 9)%18, DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG)) & 0x3));
#if 0 //SPR_TODO_DDRIO. Find the according register bitfield in SPR.
  csrReg = MemReadPciCfgEp (socket, ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG));
#endif //if 0
  if (bit == 0xFF) {
    feedback = (csrReg & 0xF);
  } else {
    feedback = ((csrReg&0x01) << bit);
  }
  return feedback;
} // LrDimmReadTrainFeedback

/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
VOID
SetSenseAmpODTDelays (
  PSYSHOST  Host,
  UINT8     socket
  )
{
  UINT8                                Ch, Dimm, Rank, Strobe;
  INT16                                Delay;
  INT16                                DqsSenseAmpDelay;
  INT16                                DqSenseAmpDelay;
  UINT32                               DqsSenseAmpDuration;
  UINT32                               DqSenseAmpDuration;
  INT16                                MaxRcven;
  INT16                                MinRcven;
  INT16                                RcvenDelta;
  UINT8                                MSVx4;
  UINT8                                MaxChDdr;
  UINT8                                DurationConstant;
  UINT8                                ReadPreamble;
  UINT8                                ReadPreambleLen = 0;
  UINT8                                ReadPostambleLen = 0;
  struct channelNvram                  (*ChannelNvList)[MAX_CH];
  DDRIO_CHANNEL_NVDATA                 (*DdrioChannelNvList) [MAX_CH];
  struct dimmNvram                     (*DimmNvList)[MAX_DIMM];
  DDRD_N0_TRAIN_CTL0_MCIO_DDRIO_STRUCT DdrdN0TrainCtl0;
  DDRD_N0_TRAIN_CTL3_MCIO_DDRIO_STRUCT DdrdN0TrainCtl3;

  MaxChDdr = GetMaxChDdr ();
  MSVx4 = GetMaxStrobeValid (Host);
  ChannelNvList = GetChannelNvList (Host, socket);
  DdrioChannelNvList = GetDdrioChannelNvList (Host, socket);

  RcDebugPrint (SDBG_MAX, "\n Starting Senseamp and ODT delay and duration calculations \n");
  RcDebugPrint (SDBG_MAX, "------------------------------------------------------------ \n");

  ReadPreamble = GetPreambleState (socket, READ_PREAMBLE);
  switch (ReadPreamble) {
  case PREAMBLE_1TCLK:
    ReadPreambleLen = 1;
    break;
  case PREAMBLE_2TCLK:
    ReadPreambleLen = 2;
    break;
  default:
    ReadPreambleLen = 1;
    break;
  }

  DurationConstant = GetDurationConstant (socket);

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, socket, Ch);

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      //
      // Skip if this is an ECC strobe when ECC is disabled
      //
      if (IsStrobeNotValid (Host, Strobe)) {
        continue;
      }
      //
      // Clear out min and max values
      //
      MaxRcven = 0;
      MinRcven = 0xfff;
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
          if (CheckRank(Host, socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          GetSetDataGroup (Host, socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RecEnDelay, GSM_READ_ONLY, &Delay);
          if (Delay < MinRcven) {
            MinRcven = Delay;
          }
          if (Delay > MaxRcven) {
            MaxRcven = Delay;
          }
        } // Rank
      } // Dimm

      //
      // rx_postamble_len = 0 or 1 based on the selected mode for postamble
      //   0 for DDR4 (DDR4 has a fixed post-amble length of 0.5 tCK)
      //
      ReadPostambleLen = 1; // DDR4 default is fixed  0.5 tCK

      //
      // calculate delays
      // Sense Amp Delay (in DCLK)
      // dqs_sa_dly[byte] = min(rcven_logic_dly)
      //
      DqsSenseAmpDelay = MinRcven / DCLKPITICKS ;

      //
      // Sense Amp Duration (in DCLK)
      //   dqs_sa_dur[byte] = max(rcven_logic_dly) - min(rcven_logic_dly) + rx_preamble_len + rx_postamble_len + DurationConstant
      //   dq_sa_dur[byte] = max(rcven_logic_dly) - min(rcven_logic_dly) + DurationConstant
      //
      RcvenDelta = MaxRcven - MinRcven;
      RcvenDelta = ALIGN_VALUE (RcvenDelta, DCLKPITICKS); // Round up RcvEn delta to DCLKPITICKS alignment.
      DqsSenseAmpDuration = (RcvenDelta / DCLKPITICKS) + ReadPreambleLen + ReadPostambleLen + DurationConstant;
      DqSenseAmpDuration = (RcvenDelta / DCLKPITICKS) + DurationConstant;
      if (DqsSenseAmpDelay < 0) {
        //
        // Convert negative number to 10nm senseampdelay bit field encoding of settings, this code assumes
        // the largest negative number that can result from the initial Senseampdelay formula is -3.
        //
        DqsSenseAmpDelay += SENSEAMP_DELAY_CONVERT_NEGATIVE_SETTING;
      } else if (DqsSenseAmpDelay > SENSEAMP_DELAY_MAX) {
        DqsSenseAmpDuration += ((UINT32)DqsSenseAmpDelay - SENSEAMP_DELAY_MAX);
        DqsSenseAmpDelay = SENSEAMP_DELAY_MAX;
      }

      //
      // dq_sa_dly[byte] = dqs_sa_dly[byte] + rx_preamble_len
      //
      DqSenseAmpDelay = DqsSenseAmpDelay + ReadPreambleLen;

      RcDebugPrintWithDevice (SDBG_MAX, socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, Strobe, NO_BIT,
        "ReadPreambleLen = %d, ReadPostambleLen = %d, MinRcven=%3d, MaxRcven=%3d: DqsSeAmpDelay=%d DqsSenseAmpDuration=%d\n",
        ReadPreambleLen, ReadPostambleLen, MinRcven, MaxRcven, DqsSenseAmpDelay, DqsSenseAmpDuration);
      RcDebugPrintWithDevice (SDBG_MAX, socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, Strobe, NO_BIT,
        "ReadPreambleLen = %d, ReadPostambleLen = %d, MinRcven=%3d, MaxRcven=%3d: DqSeAmpDelay=%d DqSenseAmpDuration=%d\n",
        ReadPreambleLen, ReadPostambleLen, MinRcven, MaxRcven, DqSenseAmpDelay, DqSenseAmpDuration);

      //
      // Read cached values
      //
      DdrdN0TrainCtl0.Data = (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl0[Strobe];
      DdrdN0TrainCtl3.Data = (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl3[Strobe];

      //
      //modify with new values
      //
      DdrdN0TrainCtl0.Bits.dqs_odt_dly       = (UINT32)DqsSenseAmpDelay;
      DdrdN0TrainCtl0.Bits.dqs_sa_dly        = (UINT32)DqsSenseAmpDelay;
      DdrdN0TrainCtl0.Bits.dqs_odt_dur       = (UINT32)DqsSenseAmpDuration;
      DdrdN0TrainCtl0.Bits.dqs_sa_dur        = (UINT32)DqsSenseAmpDuration;

      DdrdN0TrainCtl3.Bits.dq_odt_dly        = (UINT32)DqSenseAmpDelay;
      DdrdN0TrainCtl3.Bits.dq_sa_dly         = (UINT32)DqSenseAmpDelay;
      DdrdN0TrainCtl3.Bits.dq_odt_dur        = (UINT32)DqSenseAmpDuration;
      DdrdN0TrainCtl3.Bits.dq_sa_dur         = (UINT32)DqSenseAmpDuration;
      //
      // write new values
      //
      (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl0[Strobe] =  DdrdN0TrainCtl0.Data;
      (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl3[Strobe] =  DdrdN0TrainCtl3.Data;
      MemWritePciCfgEp (socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL0_MCIO_DDRIO_REG), DdrdN0TrainCtl0.Data);
      MemWritePciCfgEp (socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL3_MCIO_DDRIO_REG), DdrdN0TrainCtl3.Data);
    } // Strobe loop
  } // Ch loop
  RcDebugPrint (SDBG_MAX, "\n\n");
}  // SetSenseAmpODTDelays

/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
STATIC
UINT16
GetDdrCrDdrt (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  )
{
  //
  //SPR_TODO_DDRIO. The register is removed in SPR.
  //
  return 0;
} // GetDdrCrDdrtResult

/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
UINT8
GetDdrtTrainResult (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm
  )
{
  //
  //SPR_TODO_DDRIO.
  //
  return 0;
} // GetDdrCrDdrtResult

/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
VOID
TxPiSampleChipInit (
  PCHIP_DDRIO_STRUCT dataControl
  )
{

}

/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
VOID
GetChipLogicalRank (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 rank,
  PCHIP_DDRIO_STRUCT dataControl
  )
{
#ifndef DDR5_SUPPORT
  DDRINTF_CTL0_CH_0_MCIO_DDRIO_STRUCT   DdrintfCtl0Ch;
  //
  // SPR_TODO_DDRIO. two rank_value_ovr. One is in MCIO_DDRIO.h and the other is in MC_DDRIOMC.h. Whic one to use?
  //
  DdrintfCtl0Ch.Data = dataControl->dataControl0.Data;
  DdrintfCtl0Ch.Bits.rank_value_ovr = GetLogicalRank (Host, socket, ch, dimm, rank);
  dataControl->dataControl0.Data = DdrintfCtl0Ch.Data;
#endif // !DDR5_SUPPORT
}

/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
VOID
SetChipTxPiSampleTrainingMode (
  PSYSHOST            Host,
  UINT8               socket,
  UINT8               ch,
  PCHIP_DDRIO_STRUCT  dataControl
  )
{
#ifndef DDR5_SUPPORT
  UINT8                                 strobe;
  DDRINTF_CTL0_CH_0_MCIO_DDRIO_STRUCT   DdrintfCtl0Ch;
  DDRD_N0_TRAIN_CTL1_MC_DDRIOMC_STRUCT  DdrdN0TrainCtl1;
  DDRIO_CHANNEL_NVDATA                  (*DdrioChannelNvList) [MAX_CH];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);

  DdrioChannelNvList = GetDdrioChannelNvList (Host, socket);

  DdrintfCtl0Ch.Data = dataControl->dataControl0.Data;
  DdrdN0TrainCtl1.Data = dataControl->dataControl2.Data;

  for (strobe = 0; strobe < MSVx4; strobe++) {
    if (IsStrobeNotValid (Host, strobe)) {
      continue;
    }
    //
    // Set training mode and rank to all channels
    //
    MemWritePciCfgEp (socket, ch, UpdateIoRegisterCh10nm (Host, ch, strobe, DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_REG), DdrdN0TrainCtl1.Data | (*DdrioChannelNvList) [ch].DdrdN0TrainCtl1[strobe]);

    //
    // Delay only if this is a CTE build, otherwise do nothing.
    //

    CteDelayQclk (1); // needed for delay before setting readrf* = 1

    //
    // DDRINTF_CTL0_CH_0_MCIO_DDRIO_REG register access method is wrong. Temporarily comment out the code to avoid fatal error.
    // HSD: https://hsdes.intel.com/appstore/article/#/1507343355
    //
    //MemWritePciCfgEp (socket, ch, UpdateIoRegisterCh10nm (Host, ch, strobe, DDRINTF_CTL0_CH_0_MCIO_DDRIO_REG), dataControl0.Data | (*DdrioChannelNvList) [ch].dataControl0);
  } // strobe loop
#endif // !DDR5_SUPPORT
}

/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
VOID
SetChipTxPiSampleTrainingTXDQSStrobesI (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 rank,
  INT16 piDelay,
  PCHIP_DDRIO_STRUCT dataControl
  )
{
#ifndef DDR5_SUPPORT
  UINT8                                 strobe;
  DDRINTF_CTL0_CH_0_MCIO_DDRIO_STRUCT   dataControl0;
  DDRIO_CHANNEL_NVDATA                  (*DdrioChannelNvList) [MAX_CH];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);

  DdrioChannelNvList = GetDdrioChannelNvList (Host, socket);

  dataControl0.Data = dataControl->dataControl0_1.Data;

  // SKX change for upper nibble
  for (strobe = 0; strobe < MSVx4; strobe++) {
    if (IsStrobeNotValid (Host, strobe)) {
      continue;
    }
    //
    // Set training mode and rank to all channels
    //
    // DDRINTF_CTL0_CH_0_MCIO_DDRIO_REG register access method is wrong. Temporarily comment out the code to avoid fatal error.
    // HSD: https://hsdes.intel.com/appstore/article/#/1507343355
    //
    // MemWritePciCfgEp (socket, ch, UpdateIoRegisterCh10nm (Host, ch, strobe, DDRINTF_CTL0_CH_0_MCIO_DDRIO_REG), dataControl0.Data | (*DdrioChannelNvList)[ch].dataControl0); //KV3
  } // strobe loop

  GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, 0, DdrLevel, TxDqsDelay, GSM_UPDATE_CACHE, (INT16 *)&piDelay);
#endif // !DDR5_SUPPORT
}


/**

  Chip specific setup for TxPiSample TXDQS Strobe training, part 2

  @param Host  - Pointer to the system Host (root) structure
  @param socket  - processor socket ID
  @param ch  - Processor socket Channel number
  @param dimm  - Processor socket channel dimm number
  @param dataControl0data - training mode data

  @retval N/A

**/
VOID
SetChipTxPiSampleTrainingTXDQSStrobesII (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  PCHIP_DDRIO_STRUCT dataControl
  )
{
#ifndef DDR5_SUPPORT
  UINT8                                 strobe;
  DDRINTF_CTL0_CH_0_MCIO_DDRIO_STRUCT   dataControl0;
  DDRIO_CHANNEL_NVDATA                  (*DdrioChannelNvList) [MAX_CH];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);

  DdrioChannelNvList = GetDdrioChannelNvList (Host, socket);
  dataControl0.Data = dataControl->dataControl0.Data;

  for (strobe = 0; strobe < MSVx4; strobe++) {
    if (IsStrobeNotValid (Host, strobe)) {
      continue;
    }
    //
    // Set training mode and rank to all channels
    //
    // DDRINTF_CTL0_CH_0_MCIO_DDRIO_REG register access method is wrong. Temporarily comment out the code to avoid fatal error.
    // HSD: https://hsdes.intel.com/appstore/article/#/1507343355
    //
    // MemWritePciCfgEp (socket, ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRINTF_CTL0_CH_0_MCIO_DDRIO_REG), dataControl0.Data | (*DdrioChannelNvList) [ch].dataControl0);
  } // strobe loop
#endif // !DDR5_SUPPORT
}

/**

  Caches the Tx Eq formula factors per strobe

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number


  @retval VOID

**/
STATIC
VOID
CacheTxEqFormulaFactors10nm (
  IN     PSYSHOST           Host,
  IN     UINT8              Socket
  )
{
  UINT8   RcompDrvUp;
  UINT8   DqDrvUpCompOffset = 0;
  UINT8   Strobe;
  UINT8   Ch;
  struct channelNvram                           (*ChannelNvList) [MAX_CH];
  DDRIO_CHANNEL_NVDATA                          (*DdrioChannelNvList) [MAX_CH];
  DDRD_N0_COMP_LA0_MC_DDRIOMC_STRUCT            CompData0;
  UINT8                                         MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);
  //
  // Cache RcompCode values for this socket
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList) [Ch].enabled == 0) {
      continue;
    }

    for (Strobe = 0; Strobe < MAX_STROBE; Strobe++) {
      if (IsStrobeNotValid (Host, Strobe)) {
        continue;
      }
      CompData0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_COMP_LA0_MCIO_DDRIO_REG));
      RcompDrvUp = (UINT8) CompData0.Bits.rcomp_drv_up;

      (*DdrioChannelNvList) [Ch].RcompCode[Strobe] = RcompDrvUp + DqDrvUpCompOffset;
      if ((*DdrioChannelNvList) [Ch].RcompCode[Strobe] > MAX_RCOMP_CODE) {
        (*DdrioChannelNvList) [Ch].RcompCode[Strobe] = MAX_RCOMP_CODE;
      }
    }
  }
}

/**

  Gets the Tx Eq settings list and cache the formula factors per
  strobe

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in,out]  Param         - Pointer to caller's POWER_TRAIN_PARAM

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
GetTxEqSettings (
  IN     PSYSHOST           Host,
  IN     UINT8              Socket,
  IN OUT POWER_TRAIN_PARAM  *Param
  )
{
  UINT8   Loop;
  UINT8   NumSettings;
  UINT8   SizeSettings;
  INT16   Settings[NUM_TXEQTAP2_SETTINGS] = {3, 2, 1, 0, 1, 2, 3};

  if (Param->ParamGsm == TxEq) {
    //
    // Cache the TxEq formula factors only on the first tap and reuse cached values on Tap2
    //
    CacheTxEqFormulaFactors10nm (Host, Socket);
    //
    // Tap 1
    //
    NumSettings = NUM_TXEQTAP1_SETTINGS;

    for (Loop = 0; Loop < NumSettings; Loop++) {
      Param->SettingList[Loop] = Loop;
    }
  } else {
    //
    // Tap 2 Setting   0  1  2  3  4  5  6
    // Tap 2 Code      3  2  1  0  1  2  3
    // Post2Sign       0  0  0  1  1  1  1
    //
    SizeSettings = MIN (sizeof (Settings), sizeof (Param->SettingList));
    NumSettings = NUM_TXEQTAP2_SETTINGS;
    CopyMem ((UINT8 *) Param->SettingList, (UINT8 *) Settings, SizeSettings);
  }
  Param->NumSettings = NumSettings;
  Param->ParamType = PerNibblePerRankPerCh;

  return MRC_STATUS_SUCCESS;
} // GetTxEqSettings

/**
  Determine max DDJC setting allowed
  DDJC training requires (DdjcDelta + PxcDelta) <= MAX_DDJC_PLUC_PXC
  This routine will update NumSettings inside the POWER_TRAIN_PARAM structure.

  @param [in]     Host          - Pointer to sysHost
  @param [in]     Socket        - Socket number
  @param[in,out]  Param         - Pointer to caller's POWER_TRAIN_PARAM

  @retval - MRC_STATUS_SUCCESS

**/
STATIC
MRC_STATUS
InitDdjcNumSettings (
  IN     PSYSHOST           Host,
  IN     UINT8              Socket,
  IN OUT POWER_TRAIN_PARAM  *Param
  )
{
  UINT8                                 Bit;
  UINT8                                 Strobe;
  UINT8                                 Rank;
  UINT8                                 LogRank;
  UINT8                                 Dimm;
  UINT8                                 Ch;
  UINT8                                 SettingIndex;
  UINT16                                MaxValDdjc;
  UINT16                                MinValPxc;
  UINT16                                CurValPxc[MAX_BITS_IN_BYTE];
  UINT8                                 MSVx8;
  struct dimmNvram                      (*DimmNvList) [MAX_DIMM];
  struct channelNvram                   (*ChannelNvList) [MAX_CH];
  DDRIO_CHANNEL_NVDATA                  (*DdrioChannelNvList) [MAX_CH];
  DDRD_N0_TX_CTL1_RANK_0_MCIO_DDRIO_STRUCT     XtalkDeltaN0;
  DDRD_N0_TX_CTL1_RANK_0_MCIO_DDRIO_STRUCT     XtalkDeltaN1;
  UINT8                                        MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  MaxValDdjc = Param->SettingList[Param->NumSettings - 1];
  MinValPxc = 0xFFFF;
  MSVx8 = GetMaxStrobeValid (Host) / 2;

  ChannelNvList = GetChannelNvList (Host, Socket);
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList) [Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

      if ((*DimmNvList) [Dimm].dimmPresent == 0) {
        continue;
      }

      for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {

        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }
        LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);

        for (Strobe = 0; Strobe < MSVx8; Strobe++) {
          if (IsStrobeNotValid (Host, Strobe)) {
            continue;
          }

          //
          // Get current PXC Delta programming
          //
          XtalkDeltaN0.Data = (UINT32) (*DdrioChannelNvList) [Ch].XtalkDeltaN0[LogRank][Strobe % MSVx8];
          XtalkDeltaN1.Data = (UINT32) (*DdrioChannelNvList) [Ch].XtalkDeltaN1[LogRank][ (Strobe % MSVx8) + MSVx8];

          CurValPxc[0] = (UINT16)XtalkDeltaN0.Bits.pxc_delta0; // Bit 0
          CurValPxc[1] = (UINT16)XtalkDeltaN0.Bits.pxc_delta1; // Bit 1
          CurValPxc[2] = (UINT16)XtalkDeltaN0.Bits.pxc_delta2; // Bit 2
          CurValPxc[3] = (UINT16)XtalkDeltaN0.Bits.pxc_delta3; // Bit 3
          CurValPxc[4] = (UINT16)XtalkDeltaN1.Bits.pxc_delta2; // Bit 6
          CurValPxc[5] = (UINT16)XtalkDeltaN1.Bits.pxc_delta3; // Bit 7
          CurValPxc[6] = (UINT16)XtalkDeltaN1.Bits.pxc_delta0; // Bit 4
          CurValPxc[7] = (UINT16)XtalkDeltaN1.Bits.pxc_delta1; // Bit 5

          //
          // ***     DDJC training requires (DdjcDelta + PxcDelta) <= MAX_DDJC_PLUC_PXC      ****
          //
          // Find the minimum PxcDelta value programmed in order to determine the maximum DdjcDelta value allowed.
          // This might save time during DDJC training.
          //
          for (Bit = 0; Bit < MAX_BITS_IN_BYTE; Bit++) {
            if (CurValPxc[Bit] < MinValPxc) {
              MinValPxc = CurValPxc[Bit];
            }
          } // Bit
        } // Strobe
      } // Rank
    } // Dimm
  } // Ch

  if (MinValPxc < MAX_DDJC_PLUS_PXC) {
    MaxValDdjc = MAX_DDJC_PLUS_PXC - MinValPxc;
  } else {
    MaxValDdjc = 0;
  }

  for (SettingIndex = 0; SettingIndex < Param->NumSettings; SettingIndex++) {
    if (MaxValDdjc <= Param->SettingList[SettingIndex]) {
      Param->NumSettings = SettingIndex + 1;
      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "Capping DdjcDelta at 0x%x and reducing number of DDJC settings to %d\n", MaxValDdjc, Param->NumSettings);
      break;
    }
  }

  return MRC_STATUS_SUCCESS;
}


/**
  This gets DDJC delta settings list

  @param [in]     Host          - Pointer to sysHost
  @param [in]     Socket        - Socket number
  @param[in,out]  Param         - Pointer to caller's POWER_TRAIN_PARAM

  @retval - MRC_STATUS_SUCCESS

**/
MRC_STATUS
GetDdjcDeltaSettings (
  IN     PSYSHOST           Host,
  IN     UINT8              Socket,
  IN OUT POWER_TRAIN_PARAM  *Param
  )
{
  UINT8   SizeSettings;
  UINT8   NumSettings;
  INT16   Settings[NUM_DDJC_SETTINGS] = {0, 1, 2, 3};

  SizeSettings = MIN (sizeof (Settings), sizeof (Param->SettingList));
  NumSettings = SizeSettings / sizeof (Settings[0]);
  CopyMem ((UINT8 *) Param->SettingList, (UINT8 *) Settings, SizeSettings);

  Param->NumSettings = NumSettings;
  Param->ParamType = PerBitPerRankPerCh;

  InitDdjcNumSettings (Host, Socket, Param);

  return MRC_STATUS_SUCCESS;
} // GetDdjcDeltaSettings


/*++
  Enables Write CRC in the DDRIO

  @param[in] Host   - Pointer to sysHost
  @param[in] Socket - Socket number
  @param[in] Ch     - Channel number

  @retval N/A
--*/
VOID
EnableWrCrcDdrio (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Ch
  )
{
  UINT8                                 SubCh;
  DDRINTF_CTL0_CH_0_MCIO_DDRIO_STRUCT   DdrintfCtl0;

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

    if (GetDdrintfCtl0 (Host, Socket, Ch, SubCh, &(DdrintfCtl0.Data)) != MRC_STATUS_SUCCESS) {
      RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
        "Enabling WrCrc in DDRIO failed\n");
      break;
    }
    DdrintfCtl0.Bits.wr_crc_en = 1;
    SetDdrintfCtl0 (Host, Socket, Ch, SubCh, DdrintfCtl0.Data);
  } // SubCh
}

/**

  Returns the DDRIO Long Preamble Enable setting

  @param[in] Socket      - Socket
  @param[in] Ch          - Ch

  @retval LONG_PREAMBLE_ENABLE if long preamble is enabled in the DDRIO
          LONG_PREAMBLE_DISABLE otherwise

**/
UINT8
EFIAPI
GetLongPreambleEnableDdrio (
  IN  UINT8     Socket,
  IN  UINT8     Ch
  )
{
  PSYSHOST  Host;
  UINT8     Preamble;

  Host = GetSysHostPointer ();
  GetLongPreambleEnableTarget (Host, Socket, Ch, &Preamble);

  return Preamble;

}

/**

  Sets the DDRIO Long Preamble Enable setting

  @param[in] Socket      - Socket
  @param[in] Ch          - Ch
  @param[in] Preamble    - LONG_PREAMBLE_ENABLE or LONG_PREAMBLE_DISABLE

  @retval   N/A
**/
VOID
EFIAPI
SetLongPreambleEnableDdrio (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Preamble
  )
{
  //
  // Empty function to satisfy the API
  // The DDRIO longpreamble register does not exist on SPR
  //
}

/**

  Read the train feedback register FOR DDR5.

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Socket number
  @param[in] Channel      - Channel number
  @param[in] SubChannel   - SubChannel number
  @param[in] InputStrobe  - Strobe number

  @retval (UINT32) Feedback - Training feedback

**/
UINT32
ReadDataTrainCounterFeedbackDdr5 (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    SubChannel,
  IN UINT8    InputStrobe
  )
{

  UINT32 Feedback;
  UINT8  Strobe = 0;
  UINT8  SubChMSVx4 = GetSubChMaxStrobeValid (Host);

  Strobe = InputStrobe + SubChannel * SubChMSVx4;
  Feedback = MemReadPciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, Strobe, DDRD_N0_TRAIN_CNT_RESULT_MCIO_DDRIO_REG));

  return Feedback;
} // ReadDataTrainCounterFeedbackDdr5

/**

  Chip specific code path to get training feedback from the correct register for DDR5

  @param[in] Host         - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket       - CPU Socket Node number (Socket ID)
  @param[in] Channel      - Channel number (0-based)
  @param[in] SubChannel   - Sub Channel number (0-based)
  @param[in] InputStrobe  - Strobe (0-based)

  @retval (UINT32) Feedback - Training feedback

**/
UINT32
ReadDataTrainResultFeedbackDdr5 (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT8       Channel,
  IN UINT8       SubChannel,
  IN UINT8       InputStrobe
  )
{
  UINT32                                   Feedback;
  UINT8                                    Strobe = 0;
  UINT8                                    SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  DDRD_N0_TRAIN_RESULT0_MCIO_DDRIO_STRUCT  TrainResult;

  Strobe = InputStrobe + SubChannel * SubChMSVx4;
  TrainResult.Data = MemReadPciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, Strobe, DDRD_N0_TRAIN_RESULT0_MCIO_DDRIO_REG));
  // Even Sampler is used to capture feedback on DDR5
  Feedback = TrainResult.Bits.sa_even_sampler;
  return Feedback;
}


/**

This routine gets memory ready to be written and read for DDR5.

@param Host         - Pointer to sysHost
@param socket       - Processor to initialize
@param ch           - channel number
@param dimm         - dimm number
@param subch        - sub channel number
@param rank         - rank number
@param piDelay      - pi delay
@param dataControl  - data control value
@param StrobePass   - strobe pass number
@param UpdateMode   - register program mode

@retval N/A

**/
VOID
SetDdrioWlModeDdr5 (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 subch,
  UINT8 rank
  )
{
  UINT8                                 Strobe;
  DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_STRUCT  DdrdN0TrainCtl2;
  DDRINTF_CTL0_CH_0_MCIO_DDRIO_STRUCT   DDrintfCtl0;
  DDRD_N0_TX_CTL1_MCIO_DDRIO_STRUCT     DdrdN0TxCtl1;
  DDRIO_CHANNEL_NVDATA                  (*DdrioChannelNvList) [MAX_CH];
  UINT8                                 SubChMSVx4 = GetMaxStrobeValid (Host) / 2;
  UINT8                                 ChannelLocal = 0;

  ChannelLocal = ch;
  DdrioChannelNvList = GetDdrioChannelNvList (Host, socket);

  GetChannelMapping (ch, subch, &ChannelLocal);

  for (Strobe = (subch*SubChMSVx4); Strobe < (SubChMSVx4 + subch*SubChMSVx4); Strobe++) {
    if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
      continue;
    }
    //
    // Set training mode and rank to all channels
    //
    DdrdN0TrainCtl2.Data = MemReadPciCfgEp (socket, ChannelLocal, UpdateIoRegisterCh10nm (Host, ch, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG));
    DdrdN0TxCtl1.Data = MemReadPciCfgEp (socket, ChannelLocal, UpdateIoRegisterCh10nm (Host, ch, Strobe, DDRD_N0_TX_CTL1_MCIO_DDRIO_REG));
    //
    // To drives DQS low, when there is no WRITE command sent on the CA bus
    //
    DdrdN0TrainCtl2.Bits.drv_dqs_diff_low = 1;
    //
    // Only enable the first strobe.
    //
    DdrdN0TrainCtl2.Bits.tx_strobe_mask = 0xFE;
    //
    // Set the DQ sample maks as 0.The reason is that there is no need to mask any DQ bit in the sampling. It's to avoid the DQ swizzling impact.
    //
    DdrdN0TrainCtl2.Bits.dq_sample_mask = 0;

    MemWritePciCfgEp (socket, ChannelLocal, UpdateIoRegisterCh10nm (Host, ch, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG), DdrdN0TrainCtl2.Data);
    //
    // 2TCK: 0100 preamble.
    //
    DdrdN0TxCtl1.Bits.tx_preamble_pat = TX_PREAMBLE_PAT_2TCK_0010;
    DdrdN0TxCtl1.Bits.tx_preamble_len = TX_PREAMBLE_LEN_2TCK;
    //
    // 0.5 TCK Postamble
    //
    DdrdN0TxCtl1.Bits.tx_postamble_len = 0;
    MemWritePciCfgEp (socket, ChannelLocal, UpdateIoRegisterCh10nm (Host, ch, Strobe, DDRD_N0_TX_CTL1_MCIO_DDRIO_REG), DdrdN0TxCtl1.Data);

  } // strobe loop

  if (GetDdrintfCtl0 (Host, socket, ChannelLocal, subch, &(DDrintfCtl0.Data)) == MRC_STATUS_SUCCESS) {
    DDrintfCtl0.Bits.wr_level_train_en = 1;
    SetDdrintfCtl0 (Host, socket, ChannelLocal, subch, DDrintfCtl0.Data);
  }

}


/**

This routine clears the wl settings and make it ready to be written and read for DDR5.

@param[in] Host         - Pointer to sysHost
@param[in] socket       - Processor to initialize

@retval N/A

**/
VOID
ClearDdrioWlModeDdr5 (
  IN PSYSHOST Host,
  IN UINT8 Socket
  )
{
  UINT8                                 Strobe;
  DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_STRUCT  DdrdN0TrainCtl2;
  DDRINTF_CTL0_CH_0_MCIO_DDRIO_STRUCT   DDrintfCtl0;
  DDRD_N0_TX_CTL1_MCIO_DDRIO_STRUCT     DdrdN0TxCtl1;
  UINT8                                 Dimm;
  UINT8                                 Rank;
  struct dimmNvram                      (*DimmNvList) [MAX_DIMM];
  DDRIO_CHANNEL_NVDATA                  (*DdrioChannelNvList) [MAX_CH];
  UINT8                                 SubChMSVx4 = GetMaxStrobeValid (Host) / 2;
  UINT8                                 ChannelLocal = 0;
  UINT8                                 Ch = 0;
  UINT8                                 Subch = 0;

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

      for (Ch = 0; Ch < MAX_CH; Ch++) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if (((*DimmNvList) [Dimm].dimmPresent == 0)) {
          continue;
        }
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (Rank >= (*DimmNvList) [Dimm].numRanks) {
          continue;
        }

        for (Subch = 0; Subch < SUB_CH; Subch++) {
          GetChannelMapping (Ch, Subch, &ChannelLocal);

          for (Strobe = (Subch*SubChMSVx4); Strobe < (SubChMSVx4 + Subch*SubChMSVx4); Strobe++) {
            if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
              continue;
            }
            //
            // Set training mode and rank to all channels
            //
            DdrdN0TrainCtl2.Data = MemReadPciCfgEp (Socket, ChannelLocal, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG));
            DdrdN0TxCtl1.Data = MemReadPciCfgEp (Socket, ChannelLocal, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TX_CTL1_MCIO_DDRIO_REG));
            //
            // To drives DQS low, when there is no WRITE command sent on the CA bus
            //
            DdrdN0TrainCtl2.Bits.drv_dqs_diff_low = 0;
            //
            // Only enable the first strobe.
            //
            DdrdN0TrainCtl2.Bits.tx_strobe_mask = 0;
            //
            // Set the DQ sample maks as 0.
            // The reason is that there is no need to mask any DQ bit in the sampling. It's to avoid the DQ swizzling impact.
            //
            DdrdN0TrainCtl2.Bits.dq_sample_mask = 0;
            //
            // Where is the disablepreamble function bit in SPR.
            //

            MemWritePciCfgEp (Socket, ChannelLocal, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG), DdrdN0TrainCtl2.Data);
            //
            // 2TCK: 0100 preamble.
            //
            DdrdN0TxCtl1.Bits.tx_preamble_pat = TX_PREAMBLE_PAT_2TCK_0010;
            DdrdN0TxCtl1.Bits.tx_preamble_len = TX_PREAMBLE_LEN_2TCK;
            //
            // 0.5 TCK Postamble
            //
            DdrdN0TxCtl1.Bits.tx_postamble_len = 0;
            MemWritePciCfgEp (Socket, ChannelLocal, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TX_CTL1_MCIO_DDRIO_REG), DdrdN0TxCtl1.Data);

          } // strobe loop
        } //Subch
      } //ch
    } //rank
  } //dimm

  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (Subch = 0; Subch < SUB_CH; Subch++) {
      GetChannelMapping (Ch, Subch, &ChannelLocal);
      if (GetDdrintfCtl0 (Host, Socket, ChannelLocal, Subch, &(DDrintfCtl0.Data)) == MRC_STATUS_SUCCESS) {
        DDrintfCtl0.Bits.wr_level_train_en = 0;
        SetDdrintfCtl0 (Host, Socket, ChannelLocal, Subch, DDrintfCtl0.Data);
      }
    } // SubCh
  } // Ch
}

/**

  This routine program the csr to drive the dqs low or high.

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Processor to initialize
  @param[in] Ch           - channel number
  @param[in] Subch        - Sub channel number
  @param[in] Value        - Value

  @retval N/A

**/
VOID
ProgramDriveDqsLowDdr5 (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Subch,
  IN UINT8    Value
  )
{
  UINT8                                 Strobe;
  DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_STRUCT  DdrdN0TrainCtl2;
  UINT8                                 SubChMSVx4 = GetMaxStrobeValid (Host) / 2;
  UINT8                                 ChannelLocal = 0;
  ChannelLocal = Ch;

  GetChannelMapping (Ch, Subch, &ChannelLocal);
  if (Value == 1) {
    //
    // Add the delay to allow the PI to take effective.
    // The logic is that, before programing the TXDQS delay,
    // BIOS will drive the drv_dqs_diff_low low, and then program the txdqs delay,
    // finally, drive the drv_dqs_diff_low high and add 10 DCLKs delay to make the TX DQS PI effective.
    //
    TrainingDelay (Host, 10, 1);
  }
  for (Strobe = (Subch*SubChMSVx4); Strobe < (SubChMSVx4 + Subch*SubChMSVx4); Strobe++) {
    if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
      continue;
    }
    DdrdN0TrainCtl2.Data = MemReadPciCfgEp (Socket, ChannelLocal, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG));
    //
    // To drives DQS low, when there is no WRITE command sent on the CA bus
    //
    DdrdN0TrainCtl2.Bits.drv_dqs_diff_low = Value;
    MemWritePciCfgEp (Socket, ChannelLocal, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG), DdrdN0TrainCtl2.Data);
  } // strobe loop

}

/**

Set Drive Dqs Low for all strobes in all ranks in the channel.

@param[in] Host         - Pointer to sysHost
@param[in] Socket       - Processor to initialize
@param[in] Ch           - channel number
@param[in] Subch        - sub channel number
@param[in] Value        - value to be programmed
@param[in] Drive        - Drive or not

@retval N/A

**/
VOID
EFIAPI
SetDriveDqsLowDdr5 (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN UINT8 Subch,
  IN UINT8 Value,
  IN BOOLEAN Drive
  )
{
  UINT8                                 Strobe;
  UINT8                                 SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8                                 ChannelLocal = 0;
  DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_STRUCT  DdrdN0TrainCtl2;

  ChannelLocal = Ch;
  GetChannelMapping (Ch, Subch, &ChannelLocal);

  for (Strobe = (Subch*SubChMSVx4); Strobe < (SubChMSVx4 + Subch*SubChMSVx4); Strobe++) {
    //
    // Skip if this is an ECC strobe when ECC is disabled
    //
    if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
      continue;
    }
    DdrdN0TrainCtl2.Data = MemReadPciCfgEp (Socket, ChannelLocal, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG));;
    //
    // To drives DQS low, when there is no WRITE command sent on the CA bus
    //
    if (Drive == TRUE) {
      if (Value > 0) {
        DdrdN0TrainCtl2.Bits.drv_dqs_diff_low = 1;
        DdrdN0TrainCtl2.Bits.drv_dqs_diff_high = 0;
      } else {
        DdrdN0TrainCtl2.Bits.drv_dqs_diff_low = 0;
        DdrdN0TrainCtl2.Bits.drv_dqs_diff_high = 1;
      }
    } else {
      DdrdN0TrainCtl2.Bits.drv_dqs_diff_low = 0;
      DdrdN0TrainCtl2.Bits.drv_dqs_diff_high = 0;
    }
    MemWritePciCfgEp (Socket, ChannelLocal, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG), DdrdN0TrainCtl2.Data);
  } // strobe loop
}

/**

  Override the DQ value.

  @param Host         - Pointer to sysHost
  @param Socket       - Processor to initialize
  @param Ch           - channel number
  @param SubChMask    - sub channel mask
  @param Strobe       - strobe number
  @param Override     - override or not
  @param Value        - value

  @retval N/A

**/
VOID
OverrideDqValueDdr5 (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Ch,
  UINT8    SubChMask,
  UINT8    Strobe,
  BOOLEAN  Override,
  UINT8    Value
  )
{

  UINT8                                 ChannelLocal = 0;
  DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_STRUCT  DdrdN0TrainCtl2;
  UINT8                                 SubCh = 0;

  ChannelLocal = Ch;
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if ((SubChMask & (1<<SubCh)) == 0) {
      continue;
    }
    GetChannelMapping (Ch, SubCh, &ChannelLocal);
    Strobe = STROBE_INDEX_SUBCH (Strobe, SubCh);
    DdrdN0TrainCtl2.Data = MemReadPciCfgEp (Socket, ChannelLocal, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG));;
    //
    // To drives DQS low, when there is no WRITE command sent on the CA bus
    //
    if (Override) {
      DdrdN0TrainCtl2.Bits.dq_ovr_en = 1;
      DdrdN0TrainCtl2.Bits.dq_value_ovr = Value;
    } else {
      DdrdN0TrainCtl2.Bits.dq_ovr_en = 0; //default Value
      DdrdN0TrainCtl2.Bits.dq_value_ovr = 0;//default Value
    }
    MemWritePciCfgEp (Socket, ChannelLocal, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG), DdrdN0TrainCtl2.Data);
  }//SubCh
}

/**

  Set Dq Counting Window for a channel

  @param Socket    - Current Socket
  @param Channel   - Channel number

  @retval N/A

**/
VOID
EFIAPI
SetDqCountingWindowByChannel (
  UINT8     Socket,
  UINT8     Channel
  )
{
  UINT8     Strobe;
  PSYSHOST  Host = GetSysHostPointer ();
  UINT8     MSVx4 = GetMaxStrobeValid (Host);

  for (Strobe = 0; Strobe < MSVx4; Strobe++) {

    //
    // This function does NOTHING in SPR.
    //
    WriteDdr5DqCountingWindowValueTarget (Host, Socket, Channel, Strobe);

  } // Strobe loop
}// SetDqCountingWindowByChannel

/**

  Set Dq Counting Window

  @param host      - Pointer to sysHost
  @param socket    - Current Socket
  @param dimm      - DIMM number
  @param rank      - rank number of the DIMM

  @retval N/A

**/
VOID
EFIAPI
SetDqCountingWindow (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     dimm,
  UINT8     rank
  )
{
  UINT8               ch;
  struct channelNvram (*channelNvList) [MAX_CH];
  struct dimmNvram    (*dimmNvList) [MAX_DIMM];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList) [ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, socket, ch);

    // Continue to the next rank if this one is disabled
    if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT) || (rank >= (*dimmNvList) [dimm].numRanks)) {
      continue;
    }

    SetDqCountingWindowByChannel (socket, ch);
  } // ch loop
} // SetDqCountingWindow
/**

Set Req Counting Window

@param host      - Pointer to sysHost
@param socket    - Current Socket
@param dimm      - DIMM number
@param rank      - rank number of the DIMM

@retval N/A

**/
VOID
EFIAPI
SetReqCountingWindow (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT32    ChBitMask,
  UINT8     Dimm
  )
{
  UINT8 Ch;
  UINT8 MaxChDdr;
  DDRCC_TRAIN_CNT_WINDOW0_MCIO_DDRIOEXT_STRUCT DdrccTrainCntWindow0;

  MaxChDdr = GetMaxChDdr ();
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (!((1 << Ch) & ChBitMask)) {
      continue;
    }

    //
    // SubCh 0 counts for Dimm slot 0
    // SubCh 1 counts for Dimm slot 1
    //
    DdrccTrainCntWindow0.Data = ReadDdrioCcRegister (Host, Socket, Ch,  Dimm, DDRCC_TRAIN_CNT_WINDOW0_MCIO_DDRIOEXT_REG);
    DdrccTrainCntWindow0.Bits.cnt_window = REQ_COUNTING_WINDOW_VALUE;
    WriteDdrioCcRegister (Host, Socket, Ch, Dimm, DDRCC_TRAIN_CNT_WINDOW0_MCIO_DDRIOEXT_REG, DdrccTrainCntWindow0.Data);
  } // Ch loop
} // SetReqCountingWindow

/**

  Set Counting Window per channel

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Current Socket
  @param[in] Channel      - Channel number
  @param[in] CountWindow  - Count Window Value to program

  @retval N/A

**/
VOID
EFIAPI
SetCountingWindowByChannel (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT32    CountWindow
  )
{
  UINT8 SubCh;
  DDRCC_TRAIN_CNT_WINDOW0_MCIO_DDRIOEXT_STRUCT DdrccTrainCntWindow0;

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    DdrccTrainCntWindow0.Data = ReadDdrioCcRegister (Host, Socket, Channel, SubCh, DDRCC_TRAIN_CNT_WINDOW0_MCIO_DDRIOEXT_REG);
    DdrccTrainCntWindow0.Bits.cnt_window = CountWindow;
    WriteDdrioCcRegister (Host, Socket, Channel, SubCh, DDRCC_TRAIN_CNT_WINDOW0_MCIO_DDRIOEXT_REG, DdrccTrainCntWindow0.Data);
  }
} // SetCountingWindowByChannel
/**

  Set Counting Window of all channels with the given Dimm and Rank.

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Current Socket
  @param[in] Dimm         - DIMM number
  @param[in] Rank         - Rank number of the DIMM
  @param[in] CountWindow  - Count Window Value to program

  @retval N/A

**/
VOID
EFIAPI
SetCountingWindow (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT32    CountWindow
  )
{
  UINT8 Ch;
  struct channelNvram (*channelNvList) [MAX_CH];
  struct dimmNvram (*dimmNvList) [MAX_DIMM];
  UINT8 MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*channelNvList) [Ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, Socket, Ch);
    //
    // Continue to the next rank if this one is disabled
    //
    if (CheckRankNotToTrain (Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT, dimmNvList)) {
      continue;
    }

    SetCountingWindowByChannel (Host, Socket, Ch, CountWindow);
  } // Ch loop
} // SetCountingWindow

/**

  Set Data Train Count Window for a channel

  @param[in] Socket    - Current Socket
  @param[in] Channel   - Channel number
  @param[in] Value     - Value to be programmed

  @retval N/A

**/
VOID
EFIAPI
SetDataTrainCountWindowByChannel (
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Value
  )
{
  PSYSHOST                                    Host;
  UINT8                                       Strobe;
  UINT8                                       MSVx4;
  DDRD_N0_TRAIN_CNT_WINDOW0_MCIO_DDRIO_STRUCT DdrdTrainCntWindow0;

  Host = GetSysHostPointer ();
  MSVx4 = GetMaxStrobeValid (Host);

  for (Strobe = 0; Strobe < MSVx4; Strobe++) {
    //
    // Skip if this is an ECC strobe when ECC is disabled
    //
    if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
      continue;
    }
    DdrdTrainCntWindow0.Data = MemReadPciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, Strobe, DDRD_N0_TRAIN_CNT_WINDOW0_MCIO_DDRIO_REG));
    DdrdTrainCntWindow0.Bits.cnt_window = Value;
    MemWritePciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, Strobe, DDRD_N0_TRAIN_CNT_WINDOW0_MCIO_DDRIO_REG), DdrdTrainCntWindow0.Data);
  } // Strobe loop
} //SetDataTrainCountWindowByChannel

/**

  Set Data Train Count Window of all channel for a given Dimm and Rank

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Current Socket
  @param[in] Dimm      - DIMM number
  @param[in] Rank      - Rank number of the DIMM
  @param[in] Value     - Value to be programmed

  @retval N/A

**/
VOID
EFIAPI
SetDataTrainCountWindow (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     Value
  )
{
  UINT8                                       Ch;
  struct channelNvram                         (*ChannelNvList) [MAX_CH];
  struct dimmNvram                            (*DimmNvList) [MAX_DIMM];
  UINT8                                       MaxChDdr;
  MaxChDdr      = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList) [Ch].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    // Continue to the next rank if this one is disabled
    if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
      continue;
    }

    SetDataTrainCountWindowByChannel (Socket, Ch, Value);
  } // ch loop
} // SetDataTrainCountWindow

/**
  Sets the DQ Sample Mask field to mask off any DQ bits not considered
  in the OR computation for the command training counting.

  @param[in] Socket     - Socket number
  @param[in] ChBitmask  - Channel bit mask
  @param[in] MaskLane   - Indicate the lane needs to mask

  @retval N/A
**/
VOID
EFIAPI
SetCounterDqSampleMask (
  IN UINT8   Socket,
  IN UINT32  ChBitmask,
  IN UINT8   MaskLane
  )
{
  PSYSHOST                              Host;
  DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_STRUCT  DdrdN0TrainCtl2;
  DDRIO_CHANNEL_NVDATA                  (*DdrioChannelNvList) [MAX_CH];
  UINT8                                 Channel;
  UINT8                                 Strobe;
  UINT8                                 MSVx4;
  UINT8                                 MaxChDdr;
  UINT8                                 NewMaskLane;
  UINT8                                 MaskValue;

  Host               = GetSysHostPointer ();
  MSVx4              = GetMaxStrobeValid (Host);
  MaxChDdr           = GetMaxChDdr ();
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (!((1 << Channel) & ChBitmask)) {
      continue;
    }

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      //
      // Skip if this is an ECC Strobe when ECC is disabled
      //
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }

      // Get the current state
      DdrdN0TrainCtl2.Data = (*DdrioChannelNvList) [Channel].DdrdN0TrainCtl2[Strobe];

      //
      // Wave 3 has lane reversal(already handled by UpdateIoRegisterCh10nm) and DQ swizzle in DDRIO
      //
      NewMaskLane = GetDqSwizzle (Channel, Strobe, MaskLane, DdrLevel, MrcGtMax);
      MaskValue = ~(1 << NewMaskLane);
      DdrdN0TrainCtl2.Bits.dq_sample_mask = MaskValue & 0x0F;

      // Only write to TrainCtl2 if necessary
      if (DdrdN0TrainCtl2.Data != (*DdrioChannelNvList) [Channel].DdrdN0TrainCtl2[Strobe]) {
        (*DdrioChannelNvList) [Channel].DdrdN0TrainCtl2[Strobe] = DdrdN0TrainCtl2.Data;
        MemWritePciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG), DdrdN0TrainCtl2.Data);
      }
    } // Strobe loop
  } // Channel loop
}

/**
  Reset and start the CMD training counter

  @param[in]  Host              Ptr to SysHost.
  @param[in]  Socket            Socket.
  @param[in]  Channel           Channel.
  @param[in]  Strobe            Strobe.

  @retval EFI_SUCCESS           Wrote successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
WriteDdr5CaTrainingMode (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Strobe
  )
{
  DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_STRUCT DdrdN0TrainCtl1;
  DDRIO_CHANNEL_NVDATA                 (*DdrioChannelNvList) [MAX_CH];

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  DdrdN0TrainCtl1.Data = (*DdrioChannelNvList)[Channel].DdrdN0TrainCtl1[Strobe];
  DdrdN0TrainCtl1.Bits.cmd_train_ctr_en = 1;
  //
  // Do not update the cache for cmd_train_ctr_en - this bit is Type=RW/V
  //
  MemWritePciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, Strobe, DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_REG), DdrdN0TrainCtl1.Data);

  return EFI_SUCCESS;

} // WriteDdr5CaTrainingMode

/**

  Set Dq Counting Mode per channel

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Current Socket
  @param[in] Channel   - Channel number

  @retval N/A

**/

VOID
EFIAPI
EnableDdrioTrainingCounterByChannel (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel
  )
{
  UINT8               Strobe = 0;
  UINT8               MSVx4;
  MRC_TT              CurrentTestType;

  MSVx4 = SUB_CH * GetSubChMaxStrobeValid (Host);
  CurrentTestType = GetCurrentTestType (Socket);

  if ((CurrentTestType == EarlyCsLoopbackTest) ||
      (CurrentTestType == EarlyCaLoopbackTest) ||
      (CurrentTestType == BcomTrainingTest) ||
      (CurrentTestType == MdqsCoarseReadDelayTraining) ||
      (CurrentTestType == BacksideDqSwizzleDiscovery) ||
      (CurrentTestType == FrontsideDqSwizzleDiscovery) ||
      (CurrentTestType == MdqsReadDelayDeskew) ||
      (CurrentTestType == MdqsWriteDelay) ||
      (CurrentTestType == EarlyCsBacksideLoopbackTest) ||
      (CurrentTestType == EarlyCaBacksideLoopbackTest)) {

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {

      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }

      WriteDdr5CaTrainingMode (Host, Socket, Channel, Strobe);

    } // strobe loop
  } else if ((CurrentTestType == EarlyRcdCsLoopbackTest) ||
              (CurrentTestType == EarlyRcdCaLoopbackTest) ||
              (CurrentTestType == DcaDckDutyCycleTest) ||
              (CurrentTestType == ReqClkTest)||
              ((CurrentTestType == DcaDfeDdr5Test))) {

    //
    // Below function takes the "Strobe" input but has nothing to do with it.
    //
    UpdateDdr5IntfCmdTrainCtrlTarget (Host, Socket, Channel, Strobe);

  }

} // EnableDdrioTrainingCounterByChannel

/**

  Set Dq Counting Mode for all channels with given Dimm and Rank

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Current Socket
  @param[in] Dimm      - DIMM number
  @param[in] Rank      - rank number of the DIMM

  @retval N/A

**/

VOID
EFIAPI
EnableDdrioTrainingCounter (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
  UINT8               Ch;
  struct channelNvram (*channelNvList) [MAX_CH];
  struct dimmNvram (*dimmNvList) [MAX_DIMM];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*channelNvList) [Ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, Socket, Ch);

    // Continue to the next rank if this one is disabled
    if (CheckRankNotToTrain (Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT, dimmNvList)) {
      continue;
    }

    EnableDdrioTrainingCounterByChannel (Host, Socket, Ch);
  } // Ch loop
} // EnableDdrioTrainingCounter

/**

  Enable CS assertion trigger or not per channel.

  @param Host      - Pointer to sysHost
  @param Socket    - Current Socket
  @param Channel   - Channel number
  @param Dimm      - Dimm number
  @param Rank      - Rank number
  @param Value     - 1: enable Csa Trigger. 0: disable Csa Trigger.

  @retval N/A

**/
VOID
EFIAPI
EnableCsaTriggerByChannel (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Value
  )
{
  UINT8 SubCh;
  DDRCC_TRAIN_CTL2_MCIO_DDRIOEXT_STRUCT  DdrccTrainCtl2;

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    DdrccTrainCtl2.Data = ReadDdrioCcRegister (Host, Socket, Channel, SubCh, DDRCC_TRAIN_CTL2_MCIO_DDRIOEXT_REG);
    DdrccTrainCtl2.Bits.csa_trigger_en = Value;
    WriteDdrioCcRegister (Host, Socket, Channel, SubCh, DDRCC_TRAIN_CTL2_MCIO_DDRIOEXT_REG, DdrccTrainCtl2.Data);
  }
}

/**

  Enable CS assertion trigger or not for all channels with the given Dimm and Rank.

  @param Host      - Pointer to sysHost
  @param Socket    - Current Socket
  @param Channel   - Channel number
  @param Value     - 1: enable Csa Trigger. 0: disable Csa Trigger.

  @retval EFI_STATUS

**/
EFI_STATUS
EFIAPI
EnableCsaTrigger (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Value
  )
{
  UINT8 Ch;
  struct channelNvram (*channelNvList) [MAX_CH];
  struct dimmNvram (*dimmNvList) [MAX_DIMM];
  UINT8 MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*channelNvList) [Ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, Socket, Ch);
    //
    // Continue to the next rank if this one is disabled
    //
    if (CheckRankNotToTrain (Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT, dimmNvList)) {
      continue;
    }

    EnableCsaTriggerByChannel (Host, Socket, Ch, Value);
  }

  return EFI_SUCCESS;

} // EnableCsaTrigger

/**
  Get DDR5 write leveling training feedback.

  @param[in]  Host              Ptr to SysHost.
  @param[in]  Socket            Socket.
  @param[in]  Channel           Channel.
  @param[in]  Strobe            Strobe.
  @param[out] Feedback          Feedback.

  @retval EFI_SUCCESS           Wrote successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
GetDdr5WlDataTrainFeedback (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    SubChannel,
  IN UINT8    Strobe,
  OUT UINT32  *Feedback
  )
{
  DDRD_N0_TRAIN_CNT_RESULT_MCIO_DDRIO_STRUCT   DdrddTrainCntResult;
  //
  // Read the training result.
  //
  DdrddTrainCntResult.Data = MemReadPciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, Strobe, DDRD_N0_TRAIN_CNT_RESULT_MCIO_DDRIO_REG));

  *Feedback = DdrddTrainCntResult.Bits.train_cnt;

  return EFI_SUCCESS;

} // GetDdr5WlDataTrainFeedback

  /**
  Read DDR5 CS/CA Train Feedback

  @param Host           - Point to sysHost
  @param socket         - Socket number
  @param ch             - Channel number
  @param ioLateIoComp   - Pointer to IOLatency

  @retval N/A
  **/
UINT32
ReadTrainFeedbackDdr5 (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    subch,
  UINT8    strobe,
  UINT8    dimm
  )
{
  UINT32  feedback = 0;
  UINT8   StrobeIndex = strobe;
  MRC_TT  CurrentTestType;

  CurrentTestType = GetCurrentTestType (socket);

  if ((CurrentTestType == EarlyCsLoopbackTest) ||
      (CurrentTestType == EarlyCaLoopbackTest) ||
      (CurrentTestType == BcomTrainingTest) ||
      (CurrentTestType == EarlyCsBacksideLoopbackTest) ||
      (CurrentTestType == EarlyCaBacksideLoopbackTest)) {
    StrobeIndex = STROBE_INDEX_SUBCH (strobe, subch);
    GetDdr5DataTrainFeedbackTarget (Host, socket, ch, subch, StrobeIndex, &feedback);

  } else if (CurrentTestType == WLDdr5) {

    StrobeIndex = STROBE_INDEX_SUBCH (strobe, subch);
    GetDdr5WlDataTrainFeedback (Host, socket, ch, subch, StrobeIndex, &feedback);

  } else if ((CurrentTestType == EarlyRcdCsLoopbackTest) ||
             (CurrentTestType == EarlyRcdCaLoopbackTest) ||
             (CurrentTestType == DcaDckDutyCycleTest) ||
             (CurrentTestType == DcaDfeDdr5Test)) {

    GetDdr5IntfCmdTrainCtrlFeedbackTarget (Host, socket, ch, subch, StrobeIndex, dimm, &feedback);
    if (CurrentTestType == DcaDfeDdr5Test) {
       //
       // Check the CADB status
       //
       if (GetCadbTestDone (Host, socket, ch, subch) == FALSE) {
         //
         // Poll test completion
         //
         CadbPollTestDone (Host, socket, 1 << ch, 1 << subch);
         RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_179);
       }
    }
    //
    // Disable the CS Assertion Trigger.
    //
    EnableCsaTrigger (Host, socket, dimm, 0, CSA_TRIGGER_DISABLE);
  }
  return feedback;

} // ReadTrainFeedbackDdr5

/**
  Program JEDEC timing registers

  @param[in] Host                 - Pointer to sysHost
  @param[in] socket               - Socket number
  @param[in] ch                   - Channel number
  @param[in] OriginalWriteTimings - Pointer to saved timing values
                                    Will be ignored if NULL

  @retval N/A
**/
VOID
EFIAPI
ProgramJedecTimingsDdrio (
  IN  PSYSHOST        Host,
  IN  UINT8           Socket,
  IN  UINT8           Ch,
  IN  WRITE_TIMINGS   *OriginalWriteTimings   OPTIONAL
  )
{
  UINT8                                       SubCh;
  UINT32                                      DataTimingRegOffset;
  DDRINTF_DATA_TIMING0_CH_0_MCIO_DDRIO_STRUCT DataTiming0Reg;
  UINT8                                       MemoryControllerChannel;
  UINT8                                       NumChPerMc;
  struct channelNvram                         (*ChannelNvList)[MAX_CH];
  UINT8                                       LocalCWL;

  NumChPerMc = GetNumChannelPerMc ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  MemoryControllerChannel = Ch % NumChPerMc;
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (EFI_SUCCESS != GetDataTimingRegOffset (MemoryControllerChannel, SubCh, &DataTimingRegOffset)) {
      continue;
    }
    if (OriginalWriteTimings != NULL) {
      //
      // Use original value is caller passed it in and size is correct
      //
      DataTiming0Reg.Data = OriginalWriteTimings->Timings[SubCh];
    } else {
      //
      // Else program initial settings
      //
      DataTiming0Reg.Data = MemReadPciCfgEp (Socket, Ch, DataTimingRegOffset);

      if (IsDcpmmPresentChannel (Socket, Ch) && ((*ChannelNvList)[Ch].maxDimm == 1)) {
        GetDdrtCasLatencies(Socket, Ch, 0, NULL, NULL, &LocalCWL, NULL);
      } else {
        LocalCWL = (UINT8) GettCWL (Host, Socket, Ch);
      }
      DataTiming0Reg.Bits.ddr_tcwl = LocalCWL;
    }

    MemWritePciCfgEp (Socket, Ch, DataTimingRegOffset, DataTiming0Reg.Data);
  } // SubCh
}


/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
VOID
SetCwlAdjDdrio (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     SetType,
  INT16     CwlAdj
  )
{
  DDRINTF_DATA_TIMING0_CH_0_MCIO_DDRIO_STRUCT DataTiming0Reg;
  INT16                                       DdrDqsAdj = 0;
  UINT32                                      DataTimingRegOffset;
  UINT8                                       MemoryControllerChannel;
  UINT8                                       SubCh;
  UINT8                                       NumChPerMc;
  UINT8                                       WritePreamble;
  UINT8                                       WritePreambleLen = 0;

  NumChPerMc = GetNumChannelPerMc ();

  MemoryControllerChannel = Ch % NumChPerMc;
  WritePreamble = GetPreambleState (Socket, WRITE_PREAMBLE);

  switch (WritePreamble) {
    case PREAMBLE_1TCLK:
      WritePreambleLen = 1;
      break;
    case PREAMBLE_2TCLK:
      WritePreambleLen = 2;
      break;
    case PREAMBLE_3TCLK:
      WritePreambleLen = 3;
      break;
    case PREAMBLE_4TCLK:
      WritePreambleLen = 4;
      break;
    default:
      break;
  }

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (EFI_SUCCESS != GetDataTimingRegOffset (MemoryControllerChannel, SubCh, &DataTimingRegOffset)) {
      continue;
    }

    DataTiming0Reg.Data = MemReadPciCfgEp (Socket, Ch, DataTimingRegOffset);

    DdrDqsAdj = CwlAdj - WritePreambleLen;

    if ((SetType == CWLADJ_ALL) || (SetType == CWLADJ_DDR4)) {
      DataTiming0Reg.Bits.ddr_tcwl_adj = CwlAdj;
      DataTiming0Reg.Bits.ddr_dqs_adj = DdrDqsAdj;
    }

    if ((SetType == CWLADJ_ALL) || (SetType == CWLADJ_DDRT)) {
      DataTiming0Reg.Bits.ddrt_tcwl_adj = CwlAdj;
      DataTiming0Reg.Bits.ddrt_dqs_adj = DdrDqsAdj;
    }

    MemWritePciCfgEp (Socket, Ch, DataTimingRegOffset, DataTiming0Reg.Data);
  } // SubCh
}

/**
  Get the CWL and DDR_DQS adjustment

  @param[in] Host       Pointer to sysHost, the system Host (root) structure
  @param[in] Socket     CPU Socket Node number (Socket ID)
  @param[in] Ch         Channel number (0-based)
  @param[in] SubCh      SubChannel number (0-based)
  @param[in] Dimm       Dimm number (0-based)
  @param[out]CwlAdj     Ptr to the CWL adjustment
  @param[out]DdrDqsAdj  Ptr to the DDR_DQS adjustment

  @retval               None

**/
VOID
GetCwlDdrDqsAdjDdrio (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SetType,
  OUT INT16   *CwlAdj,
  OUT INT16   *DdrDqsAdj
  )
{
  DDRINTF_DATA_TIMING0_CH_0_MCIO_DDRIO_STRUCT  DataTiming0Reg;
  UINT32                                       DataTimingRegOffset;
  UINT8                                        MemoryControllerChannel;
  UINT8                                        SubCh;
  UINT8                                        NumChPerMc;

  NumChPerMc = GetNumChannelPerMc ();
  MemoryControllerChannel = Ch % NumChPerMc;

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (EFI_SUCCESS != GetDataTimingRegOffset (MemoryControllerChannel, SubCh, &DataTimingRegOffset)) {
      continue;
    }
    DataTiming0Reg.Data = MemReadPciCfgEp (Socket, Ch, DataTimingRegOffset);

    if ((SetType == CWLADJ_ALL) || (SetType == CWLADJ_DDR5)) {
      *CwlAdj     = (INT16) DataTiming0Reg.Bits.ddr_tcwl_adj;
      *DdrDqsAdj  = (INT16) DataTiming0Reg.Bits.ddr_dqs_adj;
    }

    if ((SetType == CWLADJ_ALL) || (SetType == CWLADJ_DDRT)) {
      *CwlAdj     = (INT16) DataTiming0Reg.Bits.ddrt_tcwl_adj;
      *DdrDqsAdj  = (INT16) DataTiming0Reg.Bits.ddrt_dqs_adj;
    }
  }  // SubCh
}

/**

  Restore the MC CWL with the write leveling trained value for the write dqdqs training in ULT.

  @param[in] Host  - Pointer to sysHost
  @param[in] Socket  - Processor to initialize

  @retval N/A

**/
VOID
RestoreCwlForSpid (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  DDRINTF_DATA_TIMING0_CH_0_MCIO_DDRIO_STRUCT DataTiming0Reg;
  UINT32                                      DataTimingRegOffset;
  UINT8                                       MemoryControllerChannel;
  struct channelNvram                         (*ChannelNvList) [MAX_CH];
  UINT8                                       SubCh = 0;
  UINT8                                       NumChPerMc;

  UINT8 Ch;
  ChannelNvList = GetChannelNvList (Host, Socket);
  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if ((*ChannelNvList) [Ch].enabled == 0) {
      continue;
    }
    NumChPerMc = GetNumChannelPerMc ();
    MemoryControllerChannel = Ch % NumChPerMc;
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      if (EFI_SUCCESS != GetDataTimingRegOffset (MemoryControllerChannel, SubCh, &DataTimingRegOffset)) {
        continue;
      }

      DataTiming0Reg.Data = MemReadPciCfgEp (Socket, Ch, DataTimingRegOffset);
      if (Host->DdrioUltSupport) {
        //
        // Notify ULT to change tcl
        //
        CteSetOptions ("set_tcl", 1 << Ch, TWO_SUBCH_MASK, 0, (*ChannelNvList) [Ch].common.nCL);
        CteSetOptions ("set_tcl", 1 << Ch, TWO_SUBCH_MASK, 1, (*ChannelNvList) [Ch].common.nCL);
        CteSetOptions ("set_tcl", 1 << Ch, TWO_SUBCH_MASK, 2, (*ChannelNvList) [Ch].common.nCL);
        CteSetOptions ("set_tcl", 1 << Ch, TWO_SUBCH_MASK, 3, (*ChannelNvList) [Ch].common.nCL);
        //
        // Notify ULT to change tcwl //SPR_todo. Two sub ch with the same cwladj
        //
        CteSetOptions ("set_tcwl", 1 << Ch, TWO_SUBCH_MASK, 0, (*ChannelNvList) [Ch].common.nCL - 2 + DataTiming0Reg.Bits.ddr_tcwl_adj);
        CteSetOptions ("set_tcwl", 1 << Ch, TWO_SUBCH_MASK, 1, (*ChannelNvList) [Ch].common.nCL - 2 + DataTiming0Reg.Bits.ddr_tcwl_adj);
        CteSetOptions ("set_tcwl", 1 << Ch, TWO_SUBCH_MASK, 2, (*ChannelNvList) [Ch].common.nCL - 2 + DataTiming0Reg.Bits.ddr_tcwl_adj);
        CteSetOptions ("set_tcwl", 1 << Ch, TWO_SUBCH_MASK, 3, (*ChannelNvList) [Ch].common.nCL - 2 + DataTiming0Reg.Bits.ddr_tcwl_adj);
      }
    } // SubCh
  }//ch
}

/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
UINT32
GetLongPreambleDdrio (
  PSYSHOST                            Host,
  UINT8                               Socket,
  UINT8                               Ch
  )
{
  //
  // The longpreambleenable bit is removed in SPR.
  //
  return 0;
}

/**

  This routine gets memory ready to be written and read

  @param[in]  Host          - Pointer to sysHost
  @param[in]  Socket        - Processor to initialize
  @param[in]  Ch            - Channel number
  @param[in]  SubCh         - Sub channel number
  @param[out] IoLatIoComp   - Pointer to caller's storage
  @param[in]  Mode          - Bit field of access modes

  @retval N/A

**/
VOID
GetIoLatencyIoComp (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     SubCh,
  UINT32    *IoLatIoComp,
  UINT8     Mode
  )
{
  DDRINTF_IO_LAT1_CH_0_MCIO_DDRIO_STRUCT IoLatency1;
  UINT32                                    IoLatency1Ch[] = { DDRINTF_IO_LAT1_CH_0_MCIO_DDRIO_REG,
                                                               DDRINTF_IO_LAT1_CH_1_MCIO_DDRIO_REG,
                                                               DDRINTF_IO_LAT1_CH_2_MCIO_DDRIO_REG,
                                                               DDRINTF_IO_LAT1_CH_3_MCIO_DDRIO_REG
                                                             };
  struct channelNvram                       (*channelNvList) [MAX_CH];
  UINT8                                     Index;
  UINT8                                     MaxChDdr;
  UINT8                                     SwappedSubCh;

  MaxChDdr = GetMaxChDdr ();
  if (Ch >= MaxChDdr) {
    // invalid ch specified
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_123);
    return;
  }

  channelNvList = GetChannelNvList (Host, Socket);

  //
  // Update Sub Ch just in case lane reversal is enabled on current Ch.
  //
  SwappedSubCh = UpdateDdrIoCcRegisterSubCh (Host, Socket, Ch, SubCh);
  Index = (Ch*SUB_CH + SwappedSubCh) % MAX_MC_SCH;

  if (Mode & GSM_READ_CSR) {
    IoLatency1.Data = MemReadPciCfgEp (Socket, Ch, IoLatency1Ch[Index]);
  } else {
#ifdef DDR5_SUPPORT
    IoLatency1.Data = ((*channelNvList) [Ch].ioLatency1[SubCh]);
#else
    IoLatency1.Data = ((*channelNvList) [Ch].ioLatency1);
#endif // #ifdef DDR5_SUPPORT
  }

  *IoLatIoComp = IoLatency1.Bits.io_lat_comp;
}

/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
VOID
WriteFastBootTimings (
  PSYSHOST                                    Host,
  UINT8                                       socket,
  UINT8                                       ch,
  UINT8                                       dimm,
  UINT8                                       rank,
  UINT8                                       strobe
  )
{
  DDRIO_CHANNEL_NVDATA          (*DdrioChannelNvList) [MAX_CH];
  struct ddrRank                (*rankList) [MAX_RANK_DIMM];

  DdrioChannelNvList = GetDdrioChannelNvList (Host, socket);
  rankList = GetRankNvList (Host, socket, ch, dimm);
  MemWritePciCfgEp (socket, ch, UpdateIoRegister10nm (Host, ch, (*rankList) [rank].logicalRank, strobe, DDRD_N0_SA_OFFSET_RANK_0_MCIO_DDRIO_REG),
    (*DdrioChannelNvList) [ch].rxOffset[ (*rankList) [rank].rankIndex][strobe]);
}

/**

  Restore IO Latency timings for fast boot or resume paths

  @param[in]  Host    - Pointer to sysHost
  @param[in]  Socket  - Processor to initialize
  @param[in]  Ch      - Channel number

  @retval N/A

**/
VOID
RestoreIoLatency (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch
  )
{
  UINT8                                     SubCh;
  UINT8                                     RankIndex;
  UINT8                                     IoLatency[MAX_RANK_CH];
  DDRINTF_IO_LAT0_CH_0_MCIO_DDRIO_STRUCT    IoLatency0;
  UINT32                                    IoLatency0Ch[] = { DDRINTF_IO_LAT0_CH_0_MCIO_DDRIO_REG,
                                                               DDRINTF_IO_LAT0_CH_1_MCIO_DDRIO_REG,
                                                               DDRINTF_IO_LAT0_CH_2_MCIO_DDRIO_REG,
                                                               DDRINTF_IO_LAT0_CH_3_MCIO_DDRIO_REG
                                                             };
  UINT32                                    IoLatency1Ch[] = { DDRINTF_IO_LAT1_CH_0_MCIO_DDRIO_REG,
                                                              DDRINTF_IO_LAT1_CH_1_MCIO_DDRIO_REG,
                                                              DDRINTF_IO_LAT1_CH_2_MCIO_DDRIO_REG,
                                                              DDRINTF_IO_LAT1_CH_3_MCIO_DDRIO_REG
                                                             };
  struct channelNvram                       (*ChannelNvList) [MAX_CH];
  UINT8                                     Index;
  UINT8                                     MaxChDdr;
  UINT8                                     SwappedSubCh;

  MaxChDdr = GetMaxChDdr ();
  if (Ch >= MaxChDdr) {
    // invalid ch specified
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_142);
    return;
  }

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (SubCh = 0 ; SubCh < SUB_CH; SubCh++) {

    //
    // Update Sub Ch just in case lane reversal is enabled on current Ch.
    //
    SwappedSubCh = UpdateDdrIoCcRegisterSubCh (Host, Socket, Ch, SubCh);

    Index = (Ch*SUB_CH + SwappedSubCh) % MAX_MC_SCH;

    //
    // Read per-rank IoLatency from cache
    //
    for (RankIndex = 0; RankIndex < MAX_RANK_CH; RankIndex++) {
      GetIoLatency (Socket, Ch, RankIndex, GSM_READ_ONLY, &IoLatency[RankIndex]);
    }

    //
    // Write cachecd per-rank IoLatency to the register
    //
    IoLatency0.Data = MemReadPciCfgEp (Socket, Ch, IoLatency0Ch[Index]);
    IoLatency0.Bits.io_lat0 = IoLatency[0];
    IoLatency0.Bits.io_lat1 = IoLatency[1];
    IoLatency0.Bits.io_lat2 = IoLatency[2];
    IoLatency0.Bits.io_lat3 = IoLatency[3];
    MemWritePciCfgEp (Socket, Ch, IoLatency0Ch[Index], IoLatency0.Data);

    //
    // Write per-channel IoComp from cache to the register
    //
#ifdef DDR5_SUPPORT
    MemWritePciCfgEp (Socket, Ch, IoLatency1Ch[Index], (*ChannelNvList) [Ch].ioLatency1[SubCh]);
#else
    MemWritePciCfgEp (Socket, Ch, IoLatency1Ch[Index], (*ChannelNvList) [Ch].ioLatency1);
#endif // #ifdef DDR5_SUPPORT
  } // SubCh
}

/**

  Restore DDRIO timing parameters for fast boot or resume paths

  @param[in]  Host    - Pointer to sysHost
  @param[in]  Socket  - Processor to initialize
  @param[in]  Ch      - Channel number

  @retval N/A

**/
VOID
SetRestoreTimingsDdrio (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch
  )
{

  RestoreRoundTrip (Host, Socket, Ch);
  RestoreIoLatency (Host, Socket, Ch);

}

/**

  This function applies RfOn setting to given channel.

  @param Host    - Pointer to sysHost
  @param socket  - Socket
  @param ch      - Channel
  @param rfOn    - rfOn setting to apply

@retval UINT32 - previous value

**/
UINT32
SetRfOn (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT32    rfOn
  )
{
  return 0;
} // SetRfOn

/**

  This function sets the rank type as DDRT the default is DDR4.
  This function is called only when the dimm is DDRT, this
  configuration will allow the design to select which set of
  timing and delay registers the rank is affected by

  @param  Socket Socket number
  @param  Ch     Channel number
  @param  Dimm   Dimm number

  @retval MRC_STATUS

**/
MRC_STATUS
SetDdrioRankTypeDdrt (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm
  )
{
  PSYSHOST    Host;
  MRC_STATUS  Status;
  UINT8       SubCh;
  UINT8       Index;
  DDRINTF_DATA_TIMING1_CH_0_MCIO_DDRIO_STRUCT DdrCrCh;
  UINT32                                      Offset[] = { DDRINTF_DATA_TIMING1_CH_0_MCIO_DDRIO_REG,
                                                           DDRINTF_DATA_TIMING1_CH_1_MCIO_DDRIO_REG,
                                                           DDRINTF_DATA_TIMING1_CH_2_MCIO_DDRIO_REG,
                                                           DDRINTF_DATA_TIMING1_CH_3_MCIO_DDRIO_REG
                                                         };

  Host = GetSysHostPointer ();
  Status = MRC_STATUS_SUCCESS;

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    Index = (Ch*SUB_CH + SubCh) % MAX_MC_SCH;
    DdrCrCh.Data = MemReadPciCfgEp (Socket, Ch, Offset[Index]);
    //
    // The desing supports 2 Dimms and 4 ranks per dimm. DDRT dimms are single rank.
    //
    DdrCrCh.Bits.tcwl_para_sel = 1 << (Dimm * 4);
    MemWritePciCfgEp (Socket, Ch, Offset[Index], DdrCrCh.Data);
  }

  return Status;
}

/**

  This function Asserts or DeAsserts DramReset bit in DDRIO

  @param  Socket         Socket number
  @param  ChannelBitMask Bitmask of channels to reset
  @param  IsAssert       Assert\Deassert for Dram Reset Bit

  @retval MRC_STATUS

**/
MRC_STATUS
SetDramResetN (
  IN UINT8     Socket,
  IN UINT32    ChannelBitMask,
  IN BOOLEAN   IsAssert
  )
{
  PSYSHOST                                Host;
  MRC_STATUS                              Status;
  DDRCOMP_CTL0_MCIO_DDRIOEXT_STRUCT       DdrCrSpdCfg1;
  UINT8                                   Ch;
  UINT8                                   McId;
  UINT8                                   SocketChannel;
  UINT8                                   NumChPerMc;
  UINT8                                   MaxImc;
  struct channelNvram                     (*ChannelNvList) [MAX_CH];

  Host       = GetSysHostPointer ();
  NumChPerMc = GetNumChannelPerMc ();
  MaxImc     = GetMaxImc ();
  Status     = MRC_STATUS_SUCCESS;

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (McId = 0; McId < MaxImc; McId++) {
    if (Host->var.mem.socket[Socket].imcEnabled[McId] == 0) {
      continue;
    }
    for (Ch = 0; Ch < NumChPerMc; Ch++) {
      SocketChannel = (McId * NumChPerMc) + Ch;
      if ((*ChannelNvList) [SocketChannel].enabled == 0) {
        continue;
      }
      if ((ChannelBitMask & (1 << SocketChannel)) == 0) {
        continue;
      }

      DdrCrSpdCfg1.Data = MemReadPciCfgMC (Socket, McId, DDRCOMP_CTL0_MCIO_DDRIOEXT_REG);
      if (IsAssert) {
        DdrCrSpdCfg1.Bits.dram_rst_n = 0;
      } else {
        DdrCrSpdCfg1.Bits.dram_rst_n = 1;
      }
      MemWritePciCfgMC (Socket, McId, DDRCOMP_CTL0_MCIO_DDRIOEXT_REG, DdrCrSpdCfg1.Data);
      // BIOS must assert/de-assert BRST during ddrt_reset_n assertion
      if (IsAssert) {
        DdrtAssertBrst (Host, Socket, Ch);
      }
      break;
    } //Channel loop
  } //Memory Controller loop

  return Status;
}

/**
  Return Data Comp value for specified parameter

  @param[in]  Host            - Pointer to SysHost structure
  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number
  @param[in]  Strobe          - Strobe number
  @param[in]  DataCompParam   - The specific data comp parameter
  @param[in]  DataCompValue   - Pointer to caller's storage for the value to be read

  @retval EFI_SUCCESS if value for parameter is returned
          EFI_INVALID_PARAMETER otherwise

**/
EFI_STATUS
GetDataCompValue (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      UINT8             Ch,
  IN      UINT8             Strobe,
  IN      DATA_COMP_PARAM   DataCompParam,
  OUT     UINT32            *DataCompValue
  )
{

  DDRD_N0_COMP_LA0_MCIO_DDRIO_STRUCT        CompLa0Data;
  DDRD_N0_COMP_LA1_MCIO_DDRIO_STRUCT        CompLa1Data;

  SetMem (&CompLa0Data, sizeof (DDRD_N0_COMP_LA0_MCIO_DDRIO_STRUCT), 0);
  SetMem (&CompLa1Data, sizeof (DDRD_N0_COMP_LA1_MCIO_DDRIO_STRUCT), 0);

  if (DataCompValue == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  switch (DataCompParam) {
    case DataCompRCompDrvUp:
    case DataCompRCompDrvDn:
    case DataCompOdtUp:
    case DataCompOdtDn:
      CompLa0Data.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_COMP_LA0_MCIO_DDRIO_REG));
    case DataCompSComp:
    case DataCompTcoComp:
      CompLa1Data.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_COMP_LA1_MCIO_DDRIO_REG));
      break;
    default:
      return EFI_INVALID_PARAMETER;
  }

  switch (DataCompParam) {
    case DataCompRCompDrvUp:
      *DataCompValue = CompLa0Data.Bits.rcomp_drv_up;
      break;
    case DataCompRCompDrvDn:
      *DataCompValue = CompLa0Data.Bits.rcomp_drv_dn;
      break;
    case DataCompOdtUp:
      *DataCompValue = CompLa0Data.Bits.rcomp_odt_up;
       break;
    case DataCompOdtDn:
      *DataCompValue = CompLa0Data.Bits.rcomp_odt_dn;
      break;
    case DataCompSComp:
      *DataCompValue = CompLa1Data.Bits.scomp;
      break;
    case DataCompTcoComp:
      *DataCompValue = CompLa1Data.Bits.tco_comp;
      break;
    default:
      return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;

} // GetDataCompValue

/**
  Return Comp values for the specified CMD/CTL/CLK group

  @param[in]  Host            - Pointer to SysHost structure
  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number
  @param[in]  CompGroup       - The specific cmd/ctl/clk comp group
  @param[in]  CompValues      - Pointer to caller's CMD_CTL_CLK_COMP structure

  @retval EFI_SUCCESS if value for parameter is returned
          EFI_INVALID_PARAMETER otherwise

**/
EFI_STATUS
GetCmdCtlClkCompValue (
  IN      PSYSHOST                Host,
  IN      UINT8                   Socket,
  IN      UINT8                   Ch,
  IN      CMD_CTL_CLK_COMP_GROUP  CompGroup,
  OUT     CMD_CTL_CLK_COMP        *CompValues
  )
{
  DDRCC_COMP_LA0_MCIO_DDRIOEXT_STRUCT          CCCompLa0;
  DDRCC_COMP_LA1_MCIO_DDRIOEXT_STRUCT          CCCompLa1;
  DDRCLK_COMP_LA0_MCIO_DDRIOEXT_STRUCT         ClkCompLa0;
  DDRCLK_COMP_LA1_MCIO_DDRIOEXT_STRUCT         ClkCompLa1;

  if (CompValues == NULL) {
      return EFI_INVALID_PARAMETER;
  }

  switch (CompGroup) {
    case ClkCompGroup:
      ClkCompLa0.Data = MemReadPciCfgEp (Socket, Ch, DDRCLK_COMP_LA0_MCIO_DDRIOEXT_REG);
      ClkCompLa1.Data = MemReadPciCfgEp (Socket, Ch, DDRCLK_COMP_LA1_MCIO_DDRIOEXT_REG);
      CompValues->DrvUp   = ClkCompLa0.Bits.rcomp_drv_up;
      CompValues->DrvDn   = ClkCompLa0.Bits.rcomp_drv_dn;
      CompValues->SComp   = ClkCompLa1.Bits.scomp;
      CompValues->TcoComp = ClkCompLa1.Bits.tco_comp;
      break;
    case CmdNCompGroup:
    case CmdSCompGroup:
    case CmdCkeCompGroup:
    case CmdCtlCompGroup:
    case CtlNCompGroup:
    case CtlSCompGroup:
    case CtlCkeCompGroup:
    case CtlCtlCompGroup:
      CCCompLa0.Data = MemReadPciCfgEp (Socket, Ch, DDRCC_COMP_LA0_MCIO_DDRIOEXT_REG);
      CCCompLa1.Data = MemReadPciCfgEp (Socket, Ch, DDRCC_COMP_LA1_MCIO_DDRIOEXT_REG);
      CompValues->DrvUp   = CCCompLa0.Bits.rcomp_drv_up;
      CompValues->DrvDn   = CCCompLa0.Bits.rcomp_drv_dn;
      CompValues->SComp   = CCCompLa1.Bits.scomp;
      CompValues->TcoComp = CCCompLa1.Bits.tco_comp;
      break;
    default:
      return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
} // GetCmdCtlClkCompValue

/**

  Return the DDRT DDRIO CLK number value.
  This function is called only when the dimm is DDRT, Current DDRT
  ERID is either uses CLK2 or CLK3,  this configuration will design to return
  the corresponding DIMM using clock number by input Dimm number.

  @param[in]  Host            - Pointer to SysHost structure
  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number
  @param[in]  Dimm            -Dimm number

  @retval DDRIO Clock number

**/
UINT8
GetDdrioClkNumberDdrt (
  IN      PSYSHOST                Host,
  IN      UINT8                   Socket,
  IN      UINT8                   Ch,
  IN      UINT8                   Dimm
  )
{
  UINT8   ClkNumber = 0;

  switch (Dimm) {
    case 0:
      ClkNumber = 0;        // CLK 0 for DIMM0
      break;
    case 1:
      ClkNumber = 1;        //CLK 1 for DIMM1
      break;
    default:
      break;
  } // switch

  return ClkNumber;

} // GetDdrioClkNumberDdrt

/**

  Set the RCD Host Interface Training Feedback selection per channel

  @param[in]  Host            - Pointer to SysHost structure
  @param[in]  Socket          - Socket number
  @param[in]  Channel         - Channel number
  @param[in]  Dimm            - Dimm number
  @param[in]  Rank            - Rank number
  @param[in]  PatternLoop     - Pattern loop number

  @retval N/A

**/
VOID
EFIAPI
SetRcdTrainFeedbackSelectionByChannel (
  IN      PSYSHOST                Host,
  IN      UINT8                   Socket,
  IN      UINT8                   Channel,
  IN      UINT8                   Dimm,
  IN      UINT8                   Rank,
  IN      UINT8                   PatternLoop
  )
{
  DDRCC_TRAIN_CTL2_MCIO_DDRIOEXT_STRUCT       DdrccTrainCtl2;
  UINT8 Value = 1;

  if (PatternLoop == 1) {
    Value = 0;
  }

  //
  // count 0 DdrccTrainCtl2.Bits.sample_sel = 1.Sub CH A chooses RSP_A, Sub CH B chooses RSP_B.
  //
  if (Dimm == 0) {
    DdrccTrainCtl2.Data = ReadDdrioCcRegister (Host, Socket, Channel, SUB_CH_A, DDRCC_TRAIN_CTL2_MCIO_DDRIOEXT_REG);
    DdrccTrainCtl2.Bits.train_cnt_io_sel0 = RSPA_D0_SCA_PAD;
    DdrccTrainCtl2.Bits.sample_sel = Value;
    WriteDdrioCcRegister (Host, Socket, Channel, SUB_CH_A, DDRCC_TRAIN_CTL2_MCIO_DDRIOEXT_REG, DdrccTrainCtl2.Data);

    DdrccTrainCtl2.Data = ReadDdrioCcRegister (Host, Socket, Channel, SUB_CH_B, DDRCC_TRAIN_CTL2_MCIO_DDRIOEXT_REG);
    DdrccTrainCtl2.Bits.train_cnt_io_sel0 = RSPB_D0_SCB_PAD;
    DdrccTrainCtl2.Bits.sample_sel = Value;
    WriteDdrioCcRegister (Host, Socket, Channel, SUB_CH_B, DDRCC_TRAIN_CTL2_MCIO_DDRIOEXT_REG, DdrccTrainCtl2.Data);
  } else {
    DdrccTrainCtl2.Data = ReadDdrioCcRegister (Host, Socket, Channel, SUB_CH_A, DDRCC_TRAIN_CTL2_MCIO_DDRIOEXT_REG);
    DdrccTrainCtl2.Bits.train_cnt_io_sel1 = RSPA_D1_SCA_PAD;
    DdrccTrainCtl2.Bits.sample_sel = Value;
    WriteDdrioCcRegister (Host, Socket, Channel, SUB_CH_A, DDRCC_TRAIN_CTL2_MCIO_DDRIOEXT_REG, DdrccTrainCtl2.Data);

    DdrccTrainCtl2.Data = ReadDdrioCcRegister (Host, Socket, Channel, SUB_CH_B, DDRCC_TRAIN_CTL2_MCIO_DDRIOEXT_REG);
    DdrccTrainCtl2.Bits.train_cnt_io_sel1 = RSPB_D1_SCB_PAD;
    DdrccTrainCtl2.Bits.sample_sel = Value;
    WriteDdrioCcRegister (Host, Socket, Channel, SUB_CH_B, DDRCC_TRAIN_CTL2_MCIO_DDRIOEXT_REG, DdrccTrainCtl2.Data);
  }

  ChooseQLBAsFeedback (Socket, Channel, Dimm, Rank);
}

/**

  Set the RCD Host Interface Training Feedback selection.

  @param[in]  Host            - Pointer to SysHost structure
  @param[in]  Socket          - Socket number
  @param[in]  Dimm            - Dimm number
  @param[in]  Rank            - Rank number
  @param[in]  PatternLoop     - Pattern loop number

  @retval N/A

**/
VOID
EFIAPI
SetRcdTrainFeedbackSelection (
  IN      PSYSHOST                Host,
  IN      UINT8                   Socket,
  IN      UINT8                   Dimm,
  IN      UINT8                   Rank,
  IN      UINT8                   PatternLoop
  )
{
  UINT8                                       Ch;
  struct dimmNvram                            (*DimmNvList) [MAX_DIMM];
  struct channelNvram                         (*ChannelNvList) [MAX_CH];
  UINT8                                       MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList) [Ch].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    //
    // Continue to the next rank if this one is disabled
    //
    if (CheckRankNotToTrain (Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT, DimmNvList)) {
      continue;
    }

    SetRcdTrainFeedbackSelectionByChannel (Host, Socket, Ch, Dimm, Rank, PatternLoop);

  } //ch loop
}
/**

DDRIO Init for CS CLK Training.

@param host      - Pointer to sysHost
@param socket    - Current Socket

@retval N/A

**/
VOID
EFIAPI
CsClkDdrioInit (
  PSYSHOST  Host,
  UINT8     Socket
  )
{
  UINT8 Ch;
  UINT8 SubCh;
  struct channelNvram (*channelNvList) [MAX_CH];
  DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_STRUCT DdrccTrainCntResult1;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*channelNvList) [Ch].enabled == 0) {
      continue;
    }

    for (SubCh = 0 ; SubCh < SUB_CH; SubCh++) {
      DdrccTrainCntResult1.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_REG);
      DdrccTrainCntResult1.Bits.rx_vref_en = 3;
      DdrccTrainCntResult1.Bits.force_sa_on = 1;
      DdrccTrainCntResult1.Bits.force_rxbias_on = 1;
      DdrccTrainCntResult1.Bits.force_odt_on = 1;
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_REG, DdrccTrainCntResult1.Data);
    }//SubCh loop
  } //Ch loop
} // CsClkDdrioInit

/**

DDRIO Init for Req CLK Training.

@param host      - Pointer to sysHost
@param socket    - Current Socket

@retval N/A

**/
VOID
EFIAPI
ReqClkDdrioInit (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT32    ChBitMask,
  UINT8     Dimm
  )
{
  UINT8                                  Ch;
  UINT8                                  IoCounter0;
  UINT8                                  IoCounter1;
  UINT8                                  SubChannelCache = SUB_CH_A;
  DDRIO_CHANNEL_NVDATA                   (*DdrioChannelNvList) [MAX_CH];
  DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_STRUCT  DdrccTrainCntResult1;
  DDRCC_RX_CTL0_MCIO_DDRIOEXT_STRUCT     DdrccRxCtl0;
  DDRCC_CTL0_BIT_16_MCIO_DDRIOEXT_STRUCT DdrccCtlBit16;
  DDRCC_CTL0_BIT_15_MCIO_DDRIOEXT_STRUCT DdrccCtlBit15;
  DDRCLK_RX_CTL0_MCIO_DDRIOEXT_STRUCT    DdrclkRxCtl0;
  DDRCC_TRAIN_CTL2_MCIO_DDRIOEXT_STRUCT  DdrccTrainCtl2;
  UINT8                                  MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (!((1 << Ch) & ChBitMask)) {
      continue;
    }

    //
    // RSPA_D0_SCA_PAD (16) counts for Dimm slot 0
    // RSPA_D1_SCA_PAD (15) counts for Dimm slot 1
    //
    if (Dimm == 0) {
      IoCounter0 = RSPA_D0_SCA_PAD;
      IoCounter1 = RSPA_D0_SCA_PAD;
    } else {
      IoCounter0 = RSPA_D1_SCA_PAD;
      IoCounter1 = RSPA_D1_SCA_PAD;
    }

    DdrccTrainCntResult1.Data = ReadDdrioCcRegister (Host, Socket, Ch, Dimm, DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_REG);
    DdrccTrainCntResult1.Bits.rx_vref_en = 3;
    DdrccTrainCntResult1.Bits.force_sa_on = 1;
    DdrccTrainCntResult1.Bits.force_rxbias_on = 1;
    DdrccTrainCntResult1.Bits.force_odt_on = 1;
    DdrccTrainCntResult1.Bits.ddrt2_req_train_en = 1;
    DdrccTrainCntResult1.Bits.rx_path_fb_en = 1;
    DdrccTrainCntResult1.Bits.train_clk_gate_en = 0;
    DdrccTrainCntResult1.Bits.ddr4 = 0;
    WriteDdrioCcRegister (Host, Socket, Ch, Dimm, DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_REG, DdrccTrainCntResult1.Data);


    //
    // Regular Swizzling: Dimm Slot 0 RspA is Subch A CC 16 and Dimm Slot 1 Resp A is Subch A CC15
    // Lane Reversal: Dimm Slot 0 RspA is Subch B CC 16 and Dimm Slot 1 Resp A is Subch B CC15
    // ReadDdrioCcRegister will swap Subch in case of lane reversal
    //
    
    if (IsLaneReversalEnabled(Host, Socket, Ch) == TRUE) {

      SubChannelCache = SUB_CH_B;

    }

    if (Dimm == 0) {
      DdrccCtlBit16.Data = ReadDdrioCcRegister(Host, Socket, Ch, SUB_CH_A, DDRCC_CTL0_BIT_16_MCIO_DDRIOEXT_REG);
      DdrccCtlBit16.Bits.io_dir = 0;
      DdrccCtlBit16.Bits.io_dis = 0;
      DdrccCtlBit16.Bits.rx_unsample_path_sel = 0;
      DdrccCtlBit16.Bits.rxfifo_ptr_ctl = 4;
      DdrccCtlBit16.Bits.odt_en = 1;
      DdrccCtlBit16.Bits.sa_en = 1;
      WriteDdrioCcRegister(Host, Socket, Ch, SUB_CH_A, DDRCC_CTL0_BIT_16_MCIO_DDRIOEXT_REG, DdrccCtlBit16.Data);
      (*DdrioChannelNvList)[Ch].CmdCsr[16][SubChannelCache] = DdrccCtlBit16.Data;
    } else {
      DdrccCtlBit15.Data = ReadDdrioCcRegister(Host, Socket, Ch, SUB_CH_A, DDRCC_CTL0_BIT_15_MCIO_DDRIOEXT_REG);
      DdrccCtlBit15.Bits.io_dir = 0;
      DdrccCtlBit15.Bits.io_dis = 0;
      DdrccCtlBit15.Bits.rx_unsample_path_sel = 0;
      DdrccCtlBit15.Bits.rxfifo_ptr_ctl = 4;
      DdrccCtlBit15.Bits.odt_en = 1;
      DdrccCtlBit15.Bits.sa_en = 1;
      WriteDdrioCcRegister(Host, Socket, Ch, SUB_CH_A, DDRCC_CTL0_BIT_15_MCIO_DDRIOEXT_REG, DdrccCtlBit15.Data);
      (*DdrioChannelNvList)[Ch].CmdCsr[15][SubChannelCache] = DdrccCtlBit15.Data;
    }
    
    DdrccRxCtl0.Data = ReadDdrioCcRegister (Host, Socket, Ch, Dimm, DDRCC_RX_CTL0_MCIO_DDRIOEXT_REG);
    DdrccRxCtl0.Bits.rx_path_ungate = 0;
    WriteDdrioCcRegister (Host, Socket, Ch, Dimm, DDRCC_RX_CTL0_MCIO_DDRIOEXT_REG, DdrccRxCtl0.Data);

    DdrclkRxCtl0.Data = MemReadPciCfgEp (Socket, Ch, DDRCLK_RX_CTL0_MCIO_DDRIOEXT_REG);
    DdrclkRxCtl0.Bits.rx_path_ungate = 0;
    MemWritePciCfgEp (Socket, Ch, DDRCLK_RX_CTL0_MCIO_DDRIOEXT_REG, DdrclkRxCtl0.Data);

    DdrccTrainCtl2.Data = ReadDdrioCcRegister (Host, Socket, Ch, Dimm, DDRCC_TRAIN_CTL2_MCIO_DDRIOEXT_REG);
    DdrccTrainCtl2.Bits.train_cnt_io_sel0 = IoCounter0;
    DdrccTrainCtl2.Bits.train_cnt_io_sel1 = IoCounter1;
    DdrccTrainCtl2.Bits.sample_sel = 1;
    DdrccTrainCtl2.Bits.csa_trigger_en = 0;
    WriteDdrioCcRegister (Host, Socket, Ch, Dimm, DDRCC_TRAIN_CTL2_MCIO_DDRIOEXT_REG, DdrccTrainCtl2.Data);

    SetIodirectionValidDdrio(Host, Socket);
  } //Ch loop
} // ReqClkDdrioInit

  /**

  Programs rx_path_ungate = 1

  @param host      - Pointer to sysHost
  @param socket    - Current Socket

  @retval N/A

  **/
VOID
EFIAPI
CleanupReqClk (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm
  )
{
  DDRCC_RX_CTL0_MCIO_DDRIOEXT_STRUCT     DdrccRxCtl0;
  DDRCLK_RX_CTL0_MCIO_DDRIOEXT_STRUCT    DdrclkRxCtl0;
  DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_STRUCT  DdrccTrainCntResult1;

  DdrccRxCtl0.Data = ReadDdrioCcRegister (Host, Socket, Ch, Dimm, DDRCC_RX_CTL0_MCIO_DDRIOEXT_REG);
  DdrccRxCtl0.Bits.rx_path_ungate = 1;
  WriteDdrioCcRegister (Host, Socket, Ch, Dimm, DDRCC_RX_CTL0_MCIO_DDRIOEXT_REG, DdrccRxCtl0.Data);

  DdrclkRxCtl0.Data = MemReadPciCfgEp (Socket, Ch, DDRCLK_RX_CTL0_MCIO_DDRIOEXT_REG);
  DdrclkRxCtl0.Bits.rx_path_ungate = 1;
  MemWritePciCfgEp (Socket, Ch, DDRCLK_RX_CTL0_MCIO_DDRIOEXT_REG, DdrclkRxCtl0.Data);

  DdrccTrainCntResult1.Data = ReadDdrioCcRegister (Host, Socket, Ch, Dimm, DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_REG);
  DdrccTrainCntResult1.Bits.ddrt2_req_train_en = 0;
  WriteDdrioCcRegister (Host, Socket, Ch, Dimm, DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_REG, DdrccTrainCntResult1.Data);
}

/**

  Configure QCA training feedback sample selection based on training pattern mode

  @param[in] Socket  - Socket number
  @param[in] Ch      - Channel number (0-based)
  @param[in] Dimm    - DIMM number (0-based)
  @param[in] Rank    - Rank number (0-based)

  @retval N/A

**/
VOID
EFIAPI
ConfigQcaSampleSelection (
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN UINT8 Dimm,
  IN UINT8 Rank
)
{
  PSYSHOST                              Host;
  UINT8                                 Strobe;
  UINT8                                 MSVx4;
  DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_STRUCT  DdrdTrainCtl2;
  UINT8                                 TrainingPatternMode;

  Host = GetSysHostPointer ();
  MSVx4 = GetMaxStrobeValid (Host);
  TrainingPatternMode = Host->var.mem.socket[Socket].TrainingStatus.Bits.TrainingPatternMode;

  for (Strobe = 0; Strobe < MSVx4; Strobe++) {
    if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
      continue;
    }
    //
    // Program sample selection
    //
    DdrdTrainCtl2.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG));

    if ((TrainingPatternMode == QCA_TRAINING_PATTERN_SIMPLE) && Ca13Enabled (Host, Socket, Ch, Dimm, Rank)) {
      DdrdTrainCtl2.Bits.sample_sel = 1;
    } else {
      DdrdTrainCtl2.Bits.sample_sel = 0;
    }
    MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG), DdrdTrainCtl2.Data);

  } // strobe loop
}

/**

  DDRIO Init for QCS CLK Training for a channel

  @param[in] Socket    - Current Socket
  @param[in] Channel   - Channel
  @param[in] Dimm      - Dimm number

  @retval N/A

**/
VOID
EFIAPI
QcsQcaClkDdrioInitByChannel (
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm
  )
{
  PSYSHOST                              Host;
  UINT8                                 Strobe;
  UINT8                                 MSVx4;
  DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_STRUCT  DdrdTrainCtl2;
  struct dimmNvram                      (*DimmNvList)[MAX_DIMM];

  Host = GetSysHostPointer ();
  MSVx4 = GetMaxStrobeValid (Host);

  //
  // Only conduct DQ0
  //
  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  for (Strobe = 0; Strobe < MSVx4; Strobe++) {

    if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
      continue;
    }
    //
    // Program sample selection and DQ sample mask.
    //
    DdrdTrainCtl2.Data = MemReadPciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG));
    DdrdTrainCtl2.Bits.sample_sel = 1;

    if (((*DimmNvList) [Dimm].x4Present) == 1) {
      //
      // x4 DIMM, mask the rest 3 PINs.
      //
      DdrdTrainCtl2.Bits.dq_sample_mask = 0x0E;
    } else {
      //
      // x8 DIMM, mask the rest 7 PINs.
      //
      if (Strobe < MSVx4/2) {
        if (Strobe >= (MSVx4/4)) {
          DdrdTrainCtl2.Bits.dq_sample_mask = 0x0F;
        } else {
          DdrdTrainCtl2.Bits.dq_sample_mask = 0x0E;
        }
      } else {
        if (Strobe >= (MSVx4/4 + MSVx4/2)) {
          DdrdTrainCtl2.Bits.dq_sample_mask = 0x0F;
        } else {
          DdrdTrainCtl2.Bits.dq_sample_mask = 0x0E;
        }
      } //else
    } //else

    DdrdTrainCtl2.Bits.sampler_feedback_en = 0;
    //
    // when unsampled_path_en is 1, by pass even and odd sampler and put the result directly to the train cnt csr.
    // it's needed for both cte and post silicon.
    //
    DdrdTrainCtl2.Bits.unsampled_path_en = 1;
    MemWritePciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG), DdrdTrainCtl2.Data);

  } // Strobe loop
}

/**

  DDRIO Init for QCS CLK Training for all channels for a give Dimm.

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Current Socket
  @param[in] Dimm      - Dimm number

  @retval N/A

**/
VOID
EFIAPI
QcsQcaClkDdrioInit (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Dimm
  )
{
  UINT8 Ch;
  UINT8 Strobe;
  UINT8 MSVx4;
  DDRIO_CHANNEL_NVDATA                  (*DdrioChannelNvList) [MAX_CH];
  DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_STRUCT  DdrdTrainCtl2;
  UINT8                                 MaxChDdr;
  struct dimmNvram                      (*DimmNvList)[MAX_DIMM];

  MaxChDdr = GetMaxChDdr ();
  MSVx4 = GetMaxStrobeValid (Host);
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "QcsQcaClkDdrioInit starts\n");

  SetTrainingModeDdr5 (Socket, TmQcsQcaClk);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    //
    // Only conduct DQ0
    //
    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
      continue;
    }

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {

      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }
      //
      // Program sample selection and DQ sample mask.
      //
      DdrdTrainCtl2.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG));
      DdrdTrainCtl2.Bits.sample_sel = 1;

      if (((*DimmNvList) [Dimm].x4Present) == 1) {
        //
        // x4 DIMM, mask the rest 3 PINs.
        //
        DdrdTrainCtl2.Bits.dq_sample_mask = 0x0E;
      } else {
        //
        // x8 DIMM, mask the rest 7 PINs.
        //
        if (Strobe < MSVx4/2) {
          if (Strobe >= (MSVx4/4)) {
            DdrdTrainCtl2.Bits.dq_sample_mask = 0x0F;
          } else {
            DdrdTrainCtl2.Bits.dq_sample_mask = 0x0E;
          }
        } else {
          if (Strobe >= (MSVx4/4 + MSVx4/2)) {
            DdrdTrainCtl2.Bits.dq_sample_mask = 0x0F;
          } else {
            DdrdTrainCtl2.Bits.dq_sample_mask = 0x0E;
          }
        } //else
      } //else

      DdrdTrainCtl2.Bits.sampler_feedback_en = 0;
      //
      // when unsampled_path_en is 1, by pass even and odd sampler and put the result directly to the train cnt csr.
      // it's needed for both cte and post silicon.
      //
      DdrdTrainCtl2.Bits.unsampled_path_en = 1;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG), DdrdTrainCtl2.Data);

    } // strobe loop
  } //Ch loop
  //
  // Put a 60us delay here to allow the DDRIO HW to do synchronization after enabling sampler_feedback_en.
  //
  FixedDelayMicroSecond (SAMPLER_FEEDBACK_DELAY);
  //
  // Delay only if this is a CTE build, otherwise do nothing.
  //
  CteDelayQclk (6000); // needed for delay
} // QcsQcaClkDdrioInit

/**

Is strobe in nibble 0.

@param[in] Strobe    - Strobe number

@retval TRUE or FALSE

**/
BOOLEAN
IsStrobeinNibble0 (
  IN UINT8     Strobe
  )
{
  PSYSHOST  Host;
  Host = GetSysHostPointer ();
  UINT8 SubChMSVx4 = GetSubChMaxStrobeValid (Host);

  if ((Strobe < SubChMSVx4 /2) ||
      (((Strobe >= (SubChMSVx4 /2 + SubChMSVx4)) && (Strobe != 19)) || (Strobe == 14))) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/** Program idle_cnt_valid_early.


  @param[in]  Socket    Socket number
  @param[in]  Ch        The target channel number on the memory controller
  @retval EFI_SUCCESS   Register offset was successfully determined and programmed
  @retval !EFI_SUCCESS  Register offset was not successfully determined and not programmed

**/
EFI_STATUS
ProgramIdelCntValidEarly (
  IN      UINT8   Socket,
  IN      UINT8   Ch
  )
{
  PSYSHOST    Host;
  UINT8       Index;
  UINT8       SubCh = 0;
  EFI_STATUS  Status = EFI_SUCCESS;
  DDRINTF_DATA_TIMING1_CH_0_MCIO_DDRIO_STRUCT DdrintfDataTiming1;
  //
  // Per sub channel DDRINTF_DATA_TIMING1 registers
  //
  UINT32 DdrintfDataTiming1RegOffsets[] = {
    DDRINTF_DATA_TIMING1_CH_0_MCIO_DDRIO_REG,// Ch0A
    DDRINTF_DATA_TIMING1_CH_1_MCIO_DDRIO_REG,// Ch0B
    DDRINTF_DATA_TIMING1_CH_2_MCIO_DDRIO_REG,// Ch1A
    DDRINTF_DATA_TIMING1_CH_3_MCIO_DDRIO_REG // Ch1B
  };

  Host = GetSysHostPointer ();

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    Index = (Ch*SUB_CH + SubCh) % MAX_MC_SCH;
    if (Index < ((sizeof DdrintfDataTiming1RegOffsets) / (sizeof DdrintfDataTiming1RegOffsets[0]))) {
      DdrintfDataTiming1.Data = MemReadPciCfgEp (Socket, Ch, DdrintfDataTiming1RegOffsets[Index]);
      //
      // Set the idle_cnt_valid_early as 2.
      //
      DdrintfDataTiming1.Bits.idle_cnt_valid_early = 2;
      MemWritePciCfgEp (Socket, Ch, DdrintfDataTiming1RegOffsets[Index], DdrintfDataTiming1.Data);
      //
      // After programing the idle_cnt_valid_early, it require a train_rst to be issued to DDRIO.
      //
      IO_Reset (Host, Socket);
    } else {
      Status = EFI_UNSUPPORTED;
    }
  }

  return Status;
}

/**

DDRIO Init in pre training.

@param[in] Host      - Pointer to sysHost
@param[in] Socket    - Current Socket

@retval N/A

**/
VOID
EFIAPI
PretrainingDdrioInit (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  UINT8                                            Ch;
  UINT8                                            SubCh;
  UINT8                                            Strobe;
  UINT32                                           CosimSkhynix;
  DDRIO_CHANNEL_NVDATA                             (*DdrioChannelNvList) [MAX_CH];
  DDRINTF_CTL0_MCIO_DDRIO_STRUCT                   DdrintfCtl0;
  DDRCC_VSSHI_CTL_MCIO_DDRIOEXT_STRUCT             DdrccVsshiCtl;
  DDRD_N0_VSSHI_CTL_MCIO_DDRIO_STRUCT              DdrdN0VsshiCtl;
  DDRCC_RX_CTL0_MCIO_DDRIOEXT_STRUCT               DdrccRxCtl0;
  DDRD_N0_RX_CTL0_MCIO_DDRIO_STRUCT                DdrdN0RxCtl0;
  DDRCLK_RX_CTL0_MCIO_DDRIOEXT_STRUCT              DdrclkRxCtl0;
#if (DDRIO_IP_VER >= MEM_IP_18ww34e)
  DDRD_N0_DLL_CSR_MCIO_DDRIO_STRUCT                DdrdN0DllCsr;
  UINT8                                            DetrmLockStatus = 1;
  UINT32                                           InMcUlt = 0;
  BOOLEAN                                          BreakEarly = FALSE;
  EFI_STATUS                                       Status;
#endif //DDRIO_IP_VER >= MEM_IP_18ww34e
  struct dimmNvram                                 (*DimmNvList) [MAX_DIMM];
  UINT8                                            MSVx4 = GetMaxStrobeValid (Host);
  UINT8                                            MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  if (CteGetOptions ("cosim_skhynix", &CosimSkhynix) != EFI_SUCCESS) {
    CosimSkhynix = 0;
  }

  ConfigureRxPathUngate (Socket, RX_PATH_UNGATE_DISABLE);
  //
  // Program Read Preamble setting.
  //
  ProgramReadPreamble (Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    DdrintfCtl0.Data = MemReadPciCfgEp (Socket, Ch, DDRINTF_CTL0_MCIO_DDRIO_REG);
    //
    // Enable same_cycle_par_en for DDR5 RDIMM while disabling it for DDR4 RDIMM.
    //
    if (IsDdr5Present (Host, Socket)) {
      if (Host->nvram.mem.dimmTypePresent == RDIMM) {
        DdrintfCtl0.Bits.ca7_en = 1;
        DdrintfCtl0.Bits.ddr4 = 0;
        DdrintfCtl0.Bits.same_cycle_par_en = 1;
      }
    } else {
      DdrintfCtl0.Bits.ca7_en = 0;
      DdrintfCtl0.Bits.ddr4 = 1;
      if (Host->nvram.mem.dimmTypePresent == RDIMM) {
        DdrintfCtl0.Bits.same_cycle_par_en = 0;
      }
    }
    MemWritePciCfgEp (Socket, Ch, DDRINTF_CTL0_MCIO_DDRIO_REG, DdrintfCtl0.Data);
    //
    // Remove the below function call once the final MMRC is integrated
    //
    Status = ProgramIdelCntValidEarly (Socket, Ch);
    if (Status != EFI_SUCCESS) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Program idle_cnt_valid_early fails\n");
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_178);
    }
    //
    // Program VSSHI and delay for power good assertion.
    //
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      DdrccVsshiCtl.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_VSSHI_CTL_MCIO_DDRIOEXT_REG);
      DdrccVsshiCtl.Bits.vsshi_close_loop = 1;
      //
      // Choose the delay value--1K DCLKs.
      //
      DdrccVsshiCtl.Bits.vsshi_tmr_ext = 0;
      WriteDdrioCcRegister (Host, Socket, Ch,  SubCh, DDRCC_VSSHI_CTL_MCIO_DDRIOEXT_REG, DdrccVsshiCtl.Data);

      if (CosimSkhynix || UbiosGenerationOrHsleEnabled ()) {
        //
        // To allow skhynix to drive the data back.
        //
        DdrccRxCtl0.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_RX_CTL0_MCIO_DDRIOEXT_REG);
        DdrccRxCtl0.Bits.odt_seg_en = 0;
        WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_RX_CTL0_MCIO_DDRIOEXT_REG, DdrccRxCtl0.Data);
      }
    } //subch loop

    if (CosimSkhynix || UbiosGenerationOrHsleEnabled ()) {
      //
      // To allow skhynix to drive the data back.
      //
      DdrclkRxCtl0.Data = MemReadPciCfgEp (Socket, Ch, DDRCLK_RX_CTL0_MCIO_DDRIOEXT_REG);
      DdrclkRxCtl0.Bits.odt_seg_en = 0;
      MemWritePciCfgEp (Socket, Ch, DDRCLK_RX_CTL0_MCIO_DDRIOEXT_REG, DdrclkRxCtl0.Data);
    }
    //
    // Program VSSHI and delay for power good assertion.
    //
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      DdrdN0VsshiCtl.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_VSSHI_CTL_MCIO_DDRIO_REG));
      DdrdN0VsshiCtl.Bits.vsshi_close_loop = 1;
      DdrdN0VsshiCtl.Bits.ldo_close_loop = 1;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_VSSHI_CTL_MCIO_DDRIO_REG), DdrdN0VsshiCtl.Data);

      if (CosimSkhynix || UbiosGenerationOrHsleEnabled ()) {
        //
        // To allow skhynix to drive the data back.
        //
        DdrdN0RxCtl0.Data = (*DdrioChannelNvList)[Ch].DdrDataRxCtl0[Strobe];
        DdrdN0RxCtl0.Bits.odt_seg_en = 0;
        (*DdrioChannelNvList)[Ch].DdrDataRxCtl0[Strobe] = DdrdN0RxCtl0.Data;
        MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_RX_CTL0_MCIO_DDRIO_REG), DdrdN0RxCtl0.Data);
      }
    } //strobe loop
  } //Ch loop

  //
  // Polling dll lock in paralell for all channels for performance improvement
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    //
    // Program VSSHI and delay for power good assertion. Start polling the detrm_lock bit.
    //
#if (DDRIO_IP_VER >= MEM_IP_18ww34e)
    Status = CteGetOptions ("mc_ult", &(InMcUlt));
    if ((Status != EFI_SUCCESS) && (Status != EFI_UNSUPPORTED)) {
      RcDebugPrint (SDBG_DEFAULT, "PretrainingDdrioInit: Failed CteGetOptions for mc_ult\n");
    } else if ((Status == EFI_SUCCESS) && (InMcUlt == 1)) {
      BreakEarly = TRUE;
    }


    if (IsSimics ()) {
      BreakEarly = TRUE;
    }

    do {
      DetrmLockStatus = 1;
      for (Strobe = 0; Strobe < MSVx4; Strobe ++) {
        if (IsStrobeinNibble0 (Strobe)) {
          DdrdN0DllCsr.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_DLL_CSR_MCIO_DDRIO_REG));
          DetrmLockStatus = DetrmLockStatus & DdrdN0DllCsr.Bits.detrm_lock;
          if (BreakEarly) {
            DetrmLockStatus = 1;
          }
        } //if
      }//strobe loop

      // If this is a CTE build, delay between reads so we don't poll so often.
      CteDelayQclk (50);

    } while (!DetrmLockStatus);//SPR_TODO. Add timeout handler function.
#endif //DDRIO_IP_VER >= MEM_IP_18ww34e
    if (FeaturePcdGet (PcdCteBuild) == TRUE) {
      //
      // Delay if this is a CTE build.
      //
      CteDelayDclk (50);
    } else {
      //
      // 2US is the safe value here.
      //
      FixedDelayMicroSecond (2);
    }

  }
  //
  // Program x8_device bit. SPR_TODO. Will move it back to MMRC.
  //
  ProgramX8Device (Host, Socket);
} // PretrainingDdrioInit

/**
  Program x8_device

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket number

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
ProgramX8Device (
  IN PSYSHOST  Host,
  IN  UINT8    Socket
  )
{
  UINT8                                            Dimm;
  UINT8                                            Rank;
  UINT8                                            Ch;
  UINT8                                            Strobe;
  struct channelNvram                              (*ChannelNvList) [MAX_CH];
  struct dimmNvram                                 (*DimmNvList) [MAX_DIMM];
  DDRD_N0_TXDQ_PIE_OFFSET_RANK_0_MCIO_DDRIO_STRUCT DdrdN0TxdqPieOffset;
  UINT8                                            MSVx4 = GetMaxStrobeValid (Host);
  UINT8                                            MaxChDdr;
  UINT8                                            LogRank = 0;

  MaxChDdr = GetMaxChDdr ();
  //
  // SPR_CTE_WA_START. SPR_TODO. Move it to MMRC.
  //
  ChannelNvList = GetChannelNvList (Host, Socket);
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

      for (Ch = 0; Ch < MaxChDdr; Ch++) {
        if ((*ChannelNvList) [Ch].enabled == 0) {
          continue;
        }
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if (((*DimmNvList) [Dimm].dimmPresent == 0) || ((*DimmNvList) [Dimm].x4Present)) {
          continue;
        }
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (Rank >= (*DimmNvList) [Dimm].numRanks) {
          continue;
        }
        //
        // Get the logical rank #
        //
        LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);

        for (Strobe = 0; Strobe < MSVx4; Strobe++) {
          if (Strobe < MSVx4/2) {
            if (Strobe >= (MSVx4/4)) {
              DdrdN0TxdqPieOffset.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegister10nm (Host, Ch, LogRank, Strobe, DDRD_N0_TXDQ_PIE_OFFSET_RANK_0_MCIO_DDRIO_REG));
              DdrdN0TxdqPieOffset.Bits.x8_device = 1;
              MemWritePciCfgEp (Socket, Ch, UpdateIoRegister10nm(Host, Ch, LogRank, Strobe, DDRD_N0_TXDQ_PIE_OFFSET_RANK_0_MCIO_DDRIO_REG), DdrdN0TxdqPieOffset.Data);
            }
          } else {
            if (Strobe >= (MSVx4/4 +  MSVx4/2)) {
              DdrdN0TxdqPieOffset.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegister10nm (Host, Ch, LogRank, Strobe, DDRD_N0_TXDQ_PIE_OFFSET_RANK_0_MCIO_DDRIO_REG));
              DdrdN0TxdqPieOffset.Bits.x8_device = 1;
              MemWritePciCfgEp (Socket, Ch, UpdateIoRegister10nm (Host, Ch, LogRank, Strobe, DDRD_N0_TXDQ_PIE_OFFSET_RANK_0_MCIO_DDRIO_REG), DdrdN0TxdqPieOffset.Data);
            } //if
          } //else
        } //strobe loop
      }
    }
  }
  //
  // SPR_CTE_WA_END. Move it to MMRC.
  //
  return EFI_SUCCESS;
}

/**
  Apply safe but non-optimal Rx data settings

  @param[in] Socket   - Socket number

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
ApplyRxDqsSafeSettings (
  IN  UINT8     Socket
  )
{
  return EFI_SUCCESS;
}


/**

  Enable force_sa_on for soc emulation

  @param VOID

  @retval N/A

**/
VOID
EnableForceSaOn (
  IN VOID
  )
{
  UINT8 Strobe;
  PSYSHOST Host;
  DDRIO_CHANNEL_NVDATA                    (*DdrioChannelNvList) [MAX_CH];
  DDRD_N0_TRAIN_CTL1_MC_DDRIOMC_STRUCT    DdrdN0TrainCtl1;
  DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_STRUCT   DdrccTrainCntResult1;
  DDRCLK_TRAIN_CTL1_MCIO_DDRIOEXT_STRUCT  DdrclkTrainCtl1;
  UINT8 Ch = 0;
  UINT8 SubCh = 0;
  UINT8 MSVx4 = 0;
  UINT8 Socket = 0;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;
  MSVx4 = GetMaxStrobeValid (Host);
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    //
    // Enable force_sa_on for Data
    //
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      DdrdN0TrainCtl1.Data = (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl1[Strobe];
      DdrdN0TrainCtl1.Bits.force_sa_on = 1;
      (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl1[Strobe] = DdrdN0TrainCtl1.Data;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_REG), DdrdN0TrainCtl1.Data);
    }

    //
    // Enable force_sa_on for CMD/CTL
    //
    for (SubCh = 0 ; SubCh < SUB_CH; SubCh++) {
      DdrccTrainCntResult1.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_REG);
      DdrccTrainCntResult1.Bits.force_sa_on = 1;
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_REG, DdrccTrainCntResult1.Data);

    } // SubCh loop

    //
    // Enable force_sa_on for CLK
    //
    DdrclkTrainCtl1.Data = MemReadPciCfgEp (Socket, Ch, DDRCLK_TRAIN_CTL1_MCIO_DDRIOEXT_REG);
    DdrclkTrainCtl1.Bits.force_sa_on = 1;
    MemWritePciCfgEp (Socket, Ch, DDRCLK_TRAIN_CTL1_MCIO_DDRIOEXT_REG, DdrclkTrainCtl1.Data);
  }
}

/**
  Program JEDEC timing registers to meet the DDR5 PDA/PBA timing requirements.

  @param[in] Host                 - Pointer to sysHost
  @param[in] Socket               - Socket number
  @param[in] Channel              - Channel number
  @param[in] OriginalWriteTimings - Pointer to saved timing values
                                    Will be ignored if NULL

  @retval N / A
**/
VOID
ProgramPdaPbaTimingValues (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Channel,
  IN  WRITE_TIMINGS   *OriginalWriteTimings   OPTIONAL
  )
{
  UINT8                                       SubCh;
  UINT32                                      DataTimingRegOffset;
  DDRINTF_DATA_TIMING0_CH_0_MCIO_DDRIO_STRUCT DataTiming0Reg;
  UINT8                                       MemoryControllerChannel;
  UINT8                                       NumChPerMc;

  NumChPerMc = GetNumChannelPerMc ();

  MemoryControllerChannel = Channel % NumChPerMc;
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (EFI_SUCCESS != GetDataTimingRegOffset (MemoryControllerChannel, SubCh, &DataTimingRegOffset)) {
      continue;
    }

    // Program CWL to meet the DDR5 t_PDA/PBA_DQS_Delay timing requirement.
    DataTiming0Reg.Data = MemReadPciCfgEp (Socket, Channel, DataTimingRegOffset);
    if (OriginalWriteTimings != NULL) {
      //
      // Save original value if caller passed in a buffer
      //
      OriginalWriteTimings->Timings[SubCh] = DataTiming0Reg.Data;
    }
    DataTiming0Reg.Bits.ddr_tcwl = PDA_PBA_DQS_DELAY;
    DataTiming0Reg.Bits.ddrt_tcwl = PDA_PBA_DQS_DELAY;
    DataTiming0Reg.Bits.ddr_tcwl_adj = 0;
    DataTiming0Reg.Bits.ddrt_tcwl_adj = 0;
    MemWritePciCfgEp (Socket, Channel, DataTimingRegOffset, DataTiming0Reg.Data);
  } // SubCh
} //ProgramPdaPbaTimingValues

/**

  DDRIO Init for BCOM CLK Training.

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Current Socket

  @retval N/A

**/
VOID
EFIAPI
BcomDdrioInit (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  UINT8                                         Ch;
  UINT8                                         Strobe;
  UINT8                                         MSVx4;
  DDRD_N0_DFX_CTL0_MCIO_DDRIO_STRUCT            DdrdN0DfxCtl0;

  MSVx4 = GetMaxStrobeValid (Host);

  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }
      //
      // No Vref power saving in Self Refresh. All DQ Vref generators is enabled by dq_vref_en.
      //
      DdrdN0DfxCtl0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_DFX_CTL0_MCIO_DDRIO_REG));
      DdrdN0DfxCtl0.Bits.sr_vref_dis = 0;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_DFX_CTL0_MCIO_DDRIO_REG), DdrdN0DfxCtl0.Data);
      }
  }

  SetTrainingModeDdr5 (Socket, TmBcomTraining);
} // BcomDdrioInit

/**

  DDRIO programming for after BCOM CLK Training.

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Current Socket

  @retval N/A

**/
VOID
EFIAPI
PostBcomDdrioProgramming (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  SetTrainingModeDdr5 (Socket, TmBcomTrainingExit);
}

/**

  DDRIO initialization common to backside training steps.

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Current socket

  @retval N/A

**/
VOID
BacksideDdrioInit (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  UINT8                               Channel;
  UINT8                               MaxChDdr;
  UINT8                               Strobe;
  UINT8                               MSVx4;
  DDRD_N0_DFX_CTL0_MCIO_DDRIO_STRUCT  DdrdN0DfxCtl0;
  DDRD_N0_RX_CTL1_MCIO_DDRIO_STRUCT   DdrdN0RxCtl1;

  MaxChDdr = GetMaxChDdr ();
  MSVx4 = GetMaxStrobeValid (Host);

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }

      // No Vref power saving in Self Refresh.
      DdrdN0DfxCtl0.Data = MemReadPciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, Strobe, DDRD_N0_DFX_CTL0_MCIO_DDRIO_REG));
      DdrdN0DfxCtl0.Bits.sr_vref_dis = 0;
      MemWritePciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, Strobe, DDRD_N0_DFX_CTL0_MCIO_DDRIO_REG), DdrdN0DfxCtl0.Data);

      // Enable the offset control in DQ/DQS RxAmp and RxBIAS
      DdrdN0RxCtl1.Data = MemReadPciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, Strobe, DDRD_N0_RX_CTL1_MCIO_DDRIO_REG));
      DdrdN0RxCtl1.Bits.rx_offset_en = 1;
      MemWritePciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, Strobe, DDRD_N0_RX_CTL1_MCIO_DDRIO_REG), DdrdN0RxCtl1.Data);
    }
  }
}

/**

  DDRIO programming common to after backside training steps are complete.

  @param[in] Host               - Pointer to sysHost
  @param[in] Socket             - Current socket

  @retval N/A

**/
VOID
BacksideDdrioCleanup (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  UINT8                               Channel;
  UINT8                               MaxChDdr;
  UINT8                               Strobe;
  UINT8                               MSVx4;
  DDRD_N0_DFX_CTL0_MCIO_DDRIO_STRUCT  DdrdN0DfxCtl0;

  MaxChDdr = GetMaxChDdr ();
  MSVx4 = GetMaxStrobeValid (Host);

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }

      // Restore sr_vref_dis to 1 for power savings
      DdrdN0DfxCtl0.Data = MemReadPciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, Strobe, DDRD_N0_DFX_CTL0_MCIO_DDRIO_REG));
      DdrdN0DfxCtl0.Bits.sr_vref_dis = 1;
      MemWritePciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, Strobe, DDRD_N0_DFX_CTL0_MCIO_DDRIO_REG), DdrdN0DfxCtl0.Data);
    }
  }
}

/**

  DDRIO initialization for Backside Receive Enable (MRE) training.

  @param[in]  Socket  - Current socket

  @retval N/A

**/
VOID
EFIAPI
MreDdrioInit (
  IN  UINT8  Socket
  )
{
  PSYSHOST  Host;
  Host = GetSysHostPointer ();

  // Do init common to all backside training steps
  BacksideDdrioInit (Host, Socket);

  SetTrainingModeDdr5 (Socket, TmMre);
}

/**

  DDRIO programming after MRE training has been completed.

  @param[in] Socket  - Current socket

  @retval N/A

**/
VOID
EFIAPI
PostMreDdrioProgramming (
  IN UINT8  Socket
  )
{
  PSYSHOST  Host;
  Host = GetSysHostPointer ();

  // Do post DDRIO programming common to all backside training steps
  BacksideDdrioCleanup (Host, Socket);

  SetTrainingModeDdr5 (Socket, TmMreExit);
}

/**

  DDRIO initialization for Backside Read Delay (MRD) training.

  @param[in]  Socket  - Current socket

  @retval N/A

**/
VOID
EFIAPI
MrdDdrioInit (
  IN  UINT8  Socket
  )
{
  PSYSHOST  Host;
  Host = GetSysHostPointer ();

  // Do init common to all backside training steps
  BacksideDdrioInit (Host, Socket);

  SetTrainingModeDdr5 (Socket, TmMrd);
}

/**

  DDRIO programming after MRD training has been completed.

  @param[in] Socket  - Current socket

  @retval N/A

**/
VOID
EFIAPI
PostMrdDdrioProgramming (
  IN UINT8  Socket
  )
{
  PSYSHOST  Host;
  Host = GetSysHostPointer ();

  // Do post DDRIO programming common to all backside training steps
  BacksideDdrioCleanup (Host, Socket);

  SetTrainingModeDdr5 (Socket, TmMrdExit);
}

/**

  DDRIO initialization for Backside DWL training.

  @param[in]  Socket             - Current socket

  @retval N/A

**/
VOID
EFIAPI
DwlDdrioInit (
  IN UINT8  Socket
  )
{
  PSYSHOST                            Host;
  UINT8                               Channel;
  UINT8                               SubCh;
  UINT8                               MaxChDdr;
  UINT8                               Strobe;
  UINT8                               MSVx4;
  DDRD_N0_DFX_CTL0_MCIO_DDRIO_STRUCT  DdrdN0DfxCtl0;
  DDRD_N0_RX_CTL1_MCIO_DDRIO_STRUCT   DdrdN0RxCtl1;
  DDRINTF_CTL0_CH_0_MCIO_DDRIO_STRUCT DDrintfCtl0;

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();
  MSVx4 = GetMaxStrobeValid (Host);

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }

      // No Vref power saving in Self Refresh.
      DdrdN0DfxCtl0.Data = MemReadPciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, Strobe, DDRD_N0_DFX_CTL0_MCIO_DDRIO_REG));
      DdrdN0DfxCtl0.Bits.sr_vref_dis = 0;
      MemWritePciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, Strobe, DDRD_N0_DFX_CTL0_MCIO_DDRIO_REG), DdrdN0DfxCtl0.Data);

      // Enable the offset control in DQ/DQS RxAmp and RxBIAS
      DdrdN0RxCtl1.Data = MemReadPciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, Strobe, DDRD_N0_RX_CTL1_MCIO_DDRIO_REG));
      DdrdN0RxCtl1.Bits.rx_offset_en = 1;
      MemWritePciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, Strobe, DDRD_N0_RX_CTL1_MCIO_DDRIO_REG), DdrdN0RxCtl1.Data);
    }

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      if (GetDdrintfCtl0(Host, Socket, Channel, SubCh, &(DDrintfCtl0.Data)) == MRC_STATUS_SUCCESS) {
        DDrintfCtl0.Bits.wr_level_train_en = 1;
        SetDdrintfCtl0(Host, Socket, Channel, SubCh, DDrintfCtl0.Data);
      }
    }
  }

  SetTrainingModeDdr5 (Socket, TmDwl);
}

/**

  DDRIO programming after DWL training has been completed.

  @param[in] Socket             - Current socket

  @retval N/A

**/
VOID
EFIAPI
PostDwlDdrioProgramming (
  IN UINT8  Socket
  )
{
  PSYSHOST                            Host;
  UINT8                               Channel;
  UINT8                               MaxChDdr;
  UINT8                               Strobe;
  UINT8                               SubCh;
  UINT8                               MSVx4;
  DDRD_N0_DFX_CTL0_MCIO_DDRIO_STRUCT  DdrdN0DfxCtl0;
  DDRINTF_CTL0_CH_0_MCIO_DDRIO_STRUCT DDrintfCtl0;

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();
  MSVx4 = GetMaxStrobeValid (Host);

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }

      // Restore sr_vref_dis to 1 for power savings
      DdrdN0DfxCtl0.Data = MemReadPciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, Strobe, DDRD_N0_DFX_CTL0_MCIO_DDRIO_REG));
      DdrdN0DfxCtl0.Bits.sr_vref_dis = 1;
      MemWritePciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, Strobe, DDRD_N0_DFX_CTL0_MCIO_DDRIO_REG), DdrdN0DfxCtl0.Data);
    }

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      if (GetDdrintfCtl0(Host, Socket, Channel, SubCh, &(DDrintfCtl0.Data)) == MRC_STATUS_SUCCESS) {
        DDrintfCtl0.Bits.wr_level_train_en = 0;
        SetDdrintfCtl0(Host, Socket, Channel, SubCh, DDrintfCtl0.Data);
      }
    }
  }

  SetTrainingModeDdr5 (Socket, TmDwlExit);
}

/**

  DDRIO initialization for Backside Write Delay (MWD) training.

  @param[in]  Socket  - Current socket

  @retval N/A

**/
VOID
EFIAPI
MwdDdrioInit (
  IN  UINT8  Socket
  )
{
  PSYSHOST  Host;
  Host = GetSysHostPointer ();

  // Do init common to all backside training steps
  BacksideDdrioInit (Host, Socket);

  SetTrainingModeDdr5 (Socket, TmMwd);
}

/**

  DDRIO programming after MWD training has been completed.

  @param[in] Socket  - Current socket

  @retval N/A

**/
VOID
EFIAPI
PostMwdDdrioProgramming (
  IN UINT8  Socket
  )
{
  PSYSHOST  Host;
  Host = GetSysHostPointer ();

  // Do post DDRIO programming common to all backside training steps
  BacksideDdrioCleanup (Host, Socket);

  SetTrainingModeDdr5 (Socket, TmMwdExit);
}

/**
  Configure TX UI (Transmit Unit Interval).

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket number

  @retval EFI_SUCCESS
**/
VOID
ConfigureTxUi (
  IN PSYSHOST                  Host,
  IN UINT8                     Socket
  )
{
  MRC_STATUS                                Status;
  UINT8                                     Ch;
  DDRCC_CTL0_BIT_0_MCIO_DDRIOEXT_STRUCT     DdrccCtl0Bit0;
  UINT8                                     SubCh;
  GSM_CSN                                   *SignalList;
  UINT8                                     SignalListLength;
  UINT8                                     TxUiValue;
  UINT8                                     Signal;
  UINT8                                     RegisterIndex;
  UINT32                                    Address;
  UINT8                                     TimingMode;
  DDRIO_CHANNEL_NVDATA                      (*DdrioChannelNvList) [MAX_CH];

  //
  // For DDR5 RDIMM, program tx_ui for CA0-CA6 and PAR on both subchannels
  // For DDR5 UDIMM, program tx_ui for CA0-CA13 and PAR on both subchannels
  // For DDR4, program tx_ui for CMD signals on both subchannels
  //
  GSM_CSN                                   Ddr5RdimmSignalList[] = {A0A, A1A, A2A, A3A, A4A, A5A, A6A, PARA,
                                                                     A0B, A1B, A2B, A3B, A4B, A5B, A6B, PARB};
  GSM_CSN                                   Ddr5UdimmSignalList[] = {A0A, A1A, A2A, A3A, A4A, A5A, A6A, A7A, A8A, A9A, A10A, A11A, A12A, A13A, PARA,
                                                                     A0B, A1B, A2B, A3B, A4B, A5B, A6B, A7B, A8B, A9B, A10B, A11B, A12B, A13B, PARB};
  GSM_CSN                                   Ddr4SignalList[] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, ACT_N, BA0, BA1, BG0, BG1, PAR};
  UINT8                                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  //
  // Transmit Unit Interval
  //   Bit 1: Set to 1 if TX UI is 0.5 Dclk
  //   Bit 0: Set to 1 if TX UI is 2 DCLK
  //   - DDR5 RDIMM CMD bit (CA7) (1N mode) : 2'b10
  //   - DDR5 UDIMM CMD bit (CA14) (1N mode) : 2'b00
  //   - DDR5 RDIMM CMD bit (CA7) (2N mode) : 2'b00
  //   - DDR5 UDIMM CMD bit (CA14) (2N mode) : 2'b01
  //   - DDR5 CTL bit : 2'b00
  //   - DDR4 CMD/CTL bit (1N mode) : 2'b00
  //   - DDR4 CMD bit (2N mode) : 2'b01
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (!IsChannelEnabled (Socket, Ch)) {
      continue;
    }

    TimingMode = GetCurrentTimingMode (Host, Socket, Ch);
    if (IsDdr5Present (Host, Socket)) {
      if (Host->nvram.mem.dimmTypePresent == RDIMM) {
        SignalList = Ddr5RdimmSignalList;
        SignalListLength = sizeof (Ddr5RdimmSignalList) / sizeof (Ddr5RdimmSignalList[0]);

        if (TimingMode == TIMING_1N) {
          TxUiValue = 2;
        } else {
          TxUiValue = 0;
        }
      } else {
        SignalList = Ddr5UdimmSignalList;
        SignalListLength = sizeof (Ddr5UdimmSignalList) / sizeof (Ddr5UdimmSignalList[0]);

        if (TimingMode == TIMING_1N) {
          TxUiValue = 0;
        } else {
          TxUiValue = 1;
        }
      }
    } else {
      SignalList = Ddr4SignalList;
      SignalListLength = sizeof (Ddr4SignalList) / sizeof (Ddr4SignalList[0]);

      if (TimingMode == TIMING_2N) {
        TxUiValue = 1;
      } else {
        //
        // Use HW default value
        //
        return;
      }
    }

    for (Signal = 0; Signal < SignalListLength; Signal++) {

      //
      // Get register index for the signal. This can handle lane reversal.
      //
      Status = SignalToRegIndex (Host, Socket, Ch, SignalList[Signal], &SubCh, &RegisterIndex);
      if (Status != MRC_STATUS_SUCCESS) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "ConfigureTxUi - Unable to find RegIndex for signal: 0x%x\n", SignalList[Signal]);
        RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_126);
        continue;
      }

      Address = GetCmdGroupAddress (Host, RegisterIndex);
      if (Address == 0xFFFFFFFF) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "ConfigureTxUi - Unable to find address for RegIndex: %d\n", RegisterIndex);
        RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_129);
        continue;
      }

      DdrccCtl0Bit0.Data = (*DdrioChannelNvList)[Ch].CmdCsr[RegisterIndex][SubCh];
      DdrccCtl0Bit0.Bits.tx_ui = TxUiValue;
      (*DdrioChannelNvList)[Ch].CmdCsr[RegisterIndex][SubCh] = DdrccCtl0Bit0.Data;
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, Address, DdrccCtl0Bit0.Data);
    }
  }
}

/**

  This enforces a "fence" for posted CSR writes to DDRIO registers in given MEM SS

  @param Host    - Pointer to sysHost
  @param Socket  - Processor to initialize
  @param McId    - Memory controller

  @retval N/A

**/
VOID
FencePostedCsrWritesDdrio (
  PSYSHOST                  Host,
  UINT8                     Socket,
  UINT8                     McId
  )
{
  if (GetPostedMethodState (GetSysCpuCsrAccessVar ()) == POSTED_METHOD_ENABLED) {
    MemReadPciCfgMC (Socket, McId, DDRCOMP_CMD_COMP0_MCIO_DDRIOEXT_REG);
  }
}

/**

  Issue a DDRIO COMP cycle

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket number

  @retval MRC_STATUS_SUCCESS   COMP cycle successfully completed
  @retval !MRC_STATUS_SUCCESS  COMP cycle was not successful

**/
MRC_STATUS
EFIAPI
DoCompDdrio (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
  EFI_STATUS                        Status;
  UINT8                             McId;
  UINT32                            RunComp;
  DDRINTF_CTL0_MCIO_DDRIO_STRUCT    DdrintfCtl0;
  UINT8                             MaxImc;

  MaxImc     = GetMaxImc ();

  //
  // Always run this, except CTE builds should check to see if
  // it should be run.
  //

  RunComp = 1;

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {

    Status = CteGetOptions ("runcomp", &RunComp);
    if (EFI_ERROR (Status)) {
      RunComp = 1;
    }
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "RUNCOMP=%d\n", RunComp);

  }

  if (RunComp) {

#ifdef MRC_TRACE
    RcDebugPrintWithDevice (SDBG_TRACE, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "DoCompDdrio Starts\n");
#endif //MRC_TRACE

    for (McId = 0; McId < MaxImc; McId++) {
      if (Host->var.mem.socket[Socket].imcEnabled[McId] == 0) {
        continue;
      }

      DdrintfCtl0.Data = MemReadPciCfgMC (Socket, McId, DDRINTF_CTL0_MCIO_DDRIO_REG);

      //
      // Start RCOMP cycle
      //
      DdrintfCtl0.Bits.force_comp_begin = 1;
      MemWritePciCfgMC (Socket, McId, DDRINTF_CTL0_MCIO_DDRIO_REG, DdrintfCtl0.Data);

      //
      // Wait for the RCOMP cycle to complete
      //
      WaitForDdrioCompCycleTarget (Host, Socket, McId);

    } // McId loop
  }
  return MRC_STATUS_SUCCESS;
} // DoCompDdrio

/**

  Issue a DDRIO COMP cycle at specific points in training
  Do not need RCOMP cycle for QCS, QCA, BCOM (all these steps are RCD to DRAM), backside MRD, MRE, MWD, DWL, and PDA and PBA both

  @param[in] Socket   - Socket number
  @param[in] Option   - Options to specific specific points in training
                        TRAINING_COMP_MMRC_INIT
                        TRAINING_COMP_EVERY_JEDEC_INIT
                        TRAINING_COMP_EVERY_TRAINING

  @retval MRC_STATUS_SUCCESS   COMP cycle successfully completed
  @retval !MRC_STATUS_SUCCESS  COMP cycle was not successful

**/
MRC_STATUS
EFIAPI
DoCompDdrioPerSetupOption (
  IN  UINT8     Socket,
  IN  UINT8     Option
  )
{
  PSYSHOST        Host;
  SYS_SETUP       *Setup;

  Host = GetSysHostPointer ();
  Setup = GetSysSetupPointer ();

  //
  // If the input option matchs the SetupOption, then issue a Comp. cycle (RCOMP)
  //
  if (Setup->mem.TrainingCompOptions == Option) {
    return DoCompDdrio (Host, Socket);
  } else {
    return MRC_STATUS_SUCCESS;
  }
}

/**
  Do not allows Req and Err pin to propagate to MC IP.

  @param[in]  Host        - Pointer to sysHost, the system Host (root) structure struct
  @param[in]  Socket      - Processor socket within the system (0-based)
  @param[in]  Channel     - Channel within the Socket

  @retval N/A

**/
VOID
ProgramReqRxPathGate (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{
  UINT8                                        SubCh;
  struct channelNvram                          (*ChannelNvList)[MAX_CH];
  DDRCC_RX_CTL0_MCIO_DDRIOEXT_STRUCT           DdrccRxCtl0;
  DDRCLK_RX_CTL0_MCIO_DDRIOEXT_STRUCT          DdrclkRxCtl0;

  ChannelNvList = GetChannelNvList (Host, Socket);

  if ((*ChannelNvList)[Channel].ddrtEnabled == 0) {
    return;
  }

  //
  // ERR pin is on one subchannel. Req pin is on the other subchannel
  //
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    DdrccRxCtl0.Data = ReadDdrioCcRegister (Host, Socket, Channel, SubCh, DDRCC_RX_CTL0_MCIO_DDRIOEXT_REG);
    DdrccRxCtl0.Bits.rx_path_ungate = 0;
    WriteDdrioCcRegister (Host, Socket, Channel, SubCh, DDRCC_RX_CTL0_MCIO_DDRIOEXT_REG, DdrccRxCtl0.Data);
  }

  DdrclkRxCtl0.Data = MemReadPciCfgEp (Socket, Channel, DDRCLK_RX_CTL0_MCIO_DDRIOEXT_REG);
  DdrclkRxCtl0.Bits.rx_path_ungate = 0;
  MemWritePciCfgEp (Socket, Channel, DDRCLK_RX_CTL0_MCIO_DDRIOEXT_REG, DdrclkRxCtl0.Data);
}

/**
  Allows Req and Err pin to propagate to MC IP.

  @param[in]  Host        - Pointer to sysHost, the system Host (root) structure struct
  @param[in]  Socket      - Processor socket within the system (0-based)
  @param[in]  Channel     - Channel within the Socket

  @retval N/A

**/
VOID
ProgramReqRxPathUngate (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{
  UINT8                                        SubCh;
  struct channelNvram                          (*ChannelNvList)[MAX_CH];
  DDRCC_RX_CTL0_MCIO_DDRIOEXT_STRUCT           DdrccRxCtl0;
  DDRCLK_RX_CTL0_MCIO_DDRIOEXT_STRUCT          DdrclkRxCtl0;

  ChannelNvList = GetChannelNvList (Host, Socket);

  if ((*ChannelNvList)[Channel].ddrtEnabled == 0) {
    return;
  }

  //
  // ERR pin is on one subchannel. Req pin is on the other subchannel
  //
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    DdrccRxCtl0.Data = ReadDdrioCcRegister (Host, Socket, Channel, SubCh, DDRCC_RX_CTL0_MCIO_DDRIOEXT_REG);
    DdrccRxCtl0.Bits.rx_path_ungate = 1;
    WriteDdrioCcRegister (Host, Socket, Channel, SubCh, DDRCC_RX_CTL0_MCIO_DDRIOEXT_REG, DdrccRxCtl0.Data);
  }

  DdrclkRxCtl0.Data = MemReadPciCfgEp (Socket, Channel, DDRCLK_RX_CTL0_MCIO_DDRIOEXT_REG);
  DdrclkRxCtl0.Bits.rx_path_ungate = 1;
  MemWritePciCfgEp (Socket, Channel, DDRCLK_RX_CTL0_MCIO_DDRIOEXT_REG, DdrclkRxCtl0.Data);
}

/**

  Initialize the DDRIO NVDATA structure
  Called early in RC, before data is restored from NVRAM storage

  @retval EFI_BUFFER_TOO_SMALL if not enough memory allocated for the buffer
          EFI_UNSUPPORTED if the function is not implemented
          EFI_SUCCESS otherwise
**/
EFI_STATUS
EFIAPI
InitDdrioDataStructure (
  VOID
  )
{
  PSYSHOST              Host;
  UINT32                RequiredSize;
  UINT32                AllocatedSize;

  Host = GetSysHostPointer ();

  RequiredSize = sizeof (DDRIO_NVDATA);
  AllocatedSize = sizeof (Host->nvram.mem.DdrioNvdata);
  //
  // Verify sufficient memory has been allocated for the DDRIO_NVDATA structure
  //
  if (AllocatedSize < RequiredSize) {
    RcDebugPrint (SDBG_ERROR, "ERROR: Space allocated for DdrioNvdata(%d) < Space required by DDRIO_NVDATA(%d)\n", AllocatedSize, RequiredSize);
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_210);
    return EFI_BUFFER_TOO_SMALL;
  }

  //
  // If there is more space allocated for DDRIO_NVDATA than is needed, NEM and NVRAM storage space are being wasted
  //
  if (AllocatedSize > RequiredSize) {
    RcDebugPrint (SDBG_MAX, "Warning - wasted memory: Space allocated for DdrioNvdata(%d) > Space required by DDRIO_NVDATA(%d)\n", AllocatedSize, RequiredSize);
  }

  return EFI_SUCCESS;

}

/**

  Check if DDR5 sub-channel A and B require equal Round Trip Latency.

  In SPR, the Memory Controller requires roundtrip latency to be the same
  across the two subchannels. The reason is that there is a single datapath
  running at HCLK(= 2 DCLK) to handle data return from both subchannels. If
  data from 2 subchannels comes back too close to each other, there is data
  corruption in the datapath. To alleviate this, the MC scheduler has a
  restriction so that it spaces read CAS between the subchannels by 2 DCLK.
  If roundtrip latency is the same across subchannels, then the 2 DCLK spacing
  is preserved when the data gets back. If roundtrip latency was different,
  it could create a scenario where the spacing is less than 2 DCLKs causing
  corruption in the datapath.

  @param  VOID

  @retval TRUE  Sub-channel A and B require equal Round Trip Latency.
**/
BOOLEAN
EFIAPI
IsEqualSubChRtlRequired (
  VOID
  )
{
  return TRUE;
}

/**

  Get the round trip step unit.

  SPR rt_lat register is in DCLK, so RT latency step size is 1.

  @param VOID

  @retval round trip step unit

**/

UINT8
EFIAPI
GetRoundTripStepUnit (
  VOID
  )
{
  return 1;
}

/**

  Get the IO latency step unit.

  SPR io_lat register is in DCLK, so IO latency step size is 1.

  @param VOID

  @retval round trip step unit

**/

UINT8
EFIAPI
GetIoLatStepUnit (
  VOID
  )
{
  return 1;
}

/**

  Program registers to enable or disable TxOn for all strobes in a channel for DDR5

  @param[in]  Host        - Ptr to SYSHOST, system Host (root) structure
  @param[in]  Socket      - Socket number
  @param[in]  Ch          - Channel number
  @param[in]  SubCh       - Sub Channel number
  @param[in]  Strobe      - Strobe number
  @param[in]  TxOn        - TxOn value to program

  @retval None

**/
VOID
SetDataTxOnDdrioDdr5 (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT8     Ch,
  IN    UINT8     SubChMask,
  IN    UINT8     Strobe,
  IN    UINT32    TxOn
  )
{
  return;
} //SetDataTxOnDdrioDdr5

/**

  Gets the max DCA retrain loop count.

  @param VOID

  @retval MAX DCA Loop Count

**/
UINT8
GetMaxDcaRetrainLoopCount (
  VOID
  )
{
  return 3;
}

/**

  Check whether the rank needs to be trained or not

  @param[in] Socket - Socket Id
  @param[in] Ch     - Ch Id
  @param[in] Dimm   - Dimm number
  @param[in] Rank   - Rank number
  @param[in] Mode   - Mode: DONT_CHECK_MAPOUT or CHECK_MAPOUT
  @param[in] DimmNvList - DimmNvList pointer

  @retval  TRUE or FALSE

**/
BOOLEAN
EFIAPI
CheckRankNotToTrain (
  IN UINT8                     Socket,
  IN UINT8                     Ch,
  IN UINT8                     Dimm,
  IN UINT8                     Rank,
  IN UINT8                     Mode,
  IN struct                    dimmNvram (*DimmNvList) [MAX_DIMM]
  )
{
  PSYSHOST        Host;
  Host = GetSysHostPointer ();
  //
  // The function will be called during DCS training. Train DCS0 and DCS1 for DCS training.
  //
  if ((CheckRank (Host, Socket, Ch, Dimm, Rank, Mode) || (Rank >= (*DimmNvList) [Dimm].numRanks)) &&
      (GetCurrentTestType (Socket) != EarlyRcdCsLoopbackTest)) {
    return TRUE;
  } else {
    return FALSE;
  }
}


/**
  Enable Rx Dfe

  @param[in] Socket  - Socket number

  @retval None

**/
VOID
EFIAPI
EnableRxDfe (
  IN UINT8     Socket
  )
{
  PSYSHOST  Host;
  UINT8                                         Ch;
  UINT8                                         Strobe;
  DDRD_N0_RX_CTL1_MCIO_DDRIO_STRUCT             DdrdN0RxCTL1;
  Host = GetSysHostPointer ();
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }
      //
      // Enable sampler offset.
      //
      DdrdN0RxCTL1.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_RX_CTL1_MCIO_DDRIO_REG));
      DdrdN0RxCTL1.Bits.dfe_en = 1;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_RX_CTL1_MCIO_DDRIO_REG), DdrdN0RxCTL1.Data);
    }
  } //Ch loop
}

/**
  Program Rx Gain Bias

  @param[in] Socket           - Socket number
  @param[in] RxGainBiasValue  - Rx Gain Bias Value to be programmed

  @retval None

**/
VOID
EFIAPI
ProgramRxGainBias (
  IN UINT8     Socket,
  IN UINT8     RxGainBiasValue
  )
{
  PSYSHOST  Host;
  UINT8                                         Ch;
  UINT8                                         Strobe;
  DDRD_N0_RX_CTL0_MCIO_DDRIO_STRUCT             DdrdN0RxCTL0;
  Host = GetSysHostPointer ();
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }
      //
      // Enable sampler offset.
      //
      DdrdN0RxCTL0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_RX_CTL1_MCIO_DDRIO_REG));
      DdrdN0RxCTL0.Bits.rxbias_rxgain_ctl = RxGainBiasValue;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_RX_CTL1_MCIO_DDRIO_REG), DdrdN0RxCTL0.Data);
    }
  } //Ch loop
}

/**

  Check the timer expiration or not.

  @param StartTime   - The count start time
  @param MaxPollTime - The Max polling time

  @retval FALSE : Not expired.
  @retval TRUE  : Expired.

**/
BOOLEAN
CheckTimerExpiration (
  IN  UINT64 StartTime,
  IN  UINT64 MaxPollTime
  )
{
  UINT64 EndTsc = 0;

  EndTsc = GetPerformanceCounter ();
  if (TimeDiff (StartTime, EndTsc, TDIFF_UNIT_NS) <= MaxPollTime) {
    return FALSE;
  } else {
    return TRUE;
  }
} // CheckTimerExpiration

/**

  Poll the HW FSM Complete.

  @param[in]  Host                  - Pointer to sysHost, the system Host (root) structure struct
  @param[in]  Socket                - Current CPU socket
  @param[in]  ChannelMask           - Mask of channels to poll for completion
  @param[in]  MaxEstimatedFsmTime   - Polling time out value

  @retval EFI_SUCCESS   Successfully detected completion
  @retval EFI_TIMEOUT   Timed out while polling for completion

**/
EFI_STATUS
PollHwFsmComplete (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT32   ChannelMask,
  IN  UINT64   MaxEstimatedFsmTime
  )
{
  UINT8                                      Ch = 0;
  UINT8                                      Strobe;
  UINT8                                      FsmStillRunning = 0;
  BOOLEAN                                    TimerExpired = FALSE;
  DDRD_N0_HW_TRAIN_CTL0_MCIO_DDRIO_STRUCT    DdrdHwTrainCtl0;
  DDRCC_HW_TRAIN_CTL0_MCIO_DDRIOEXT_STRUCT   DdrccHwTrainCtl0;
  DDRCLK_HW_TRAIN_CTL0_MCIO_DDRIOEXT_STRUCT  DdrclkHwTrainCtl0;
  UINT32                                     InMcUlt = 0;
  BOOLEAN                                    BreakEarly = FALSE;
  EFI_STATUS                                 Status;
  UINT8                                      MSVx4 = GetMaxStrobeValid (Host);
  UINT8                                      MaxChDdr;
  UINT8                                      SubCh;
  UINT64                                     StartTsc = 0;

  MaxChDdr = GetMaxChDdr ();
  Status = CteGetOptions ("mc_ult", &(InMcUlt));
  if ((Status != EFI_SUCCESS) && (Status != EFI_UNSUPPORTED)) {
    RcDebugPrint (SDBG_DEFAULT, "PollHwFsmComplete: Failed CteGetOptions for mc_ult\n");
  } else if ((Status == EFI_SUCCESS) && (InMcUlt == 1)) {
    BreakEarly = TRUE;
  }
  if (IsSiliconWorkaroundEnabled ("22010041180")) {
    //
    // Simics currently does not clear hw_cal_en
    //
    BreakEarly = TRUE;
  }

  StartTsc = GetPerformanceCounter ();
  do {
    FsmStillRunning = 0;
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if (((ChannelMask & (1 << Ch)) == 0) || (IsChannelEnabled (Socket, Ch) == FALSE)) {
        continue;
      }
      //
      // Read the hw_cal_en for Data.
      //
      for (Strobe = 0; Strobe < MSVx4; Strobe++) {
        DdrdHwTrainCtl0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_HW_TRAIN_CTL0_MCIO_DDRIO_REG));
        FsmStillRunning |= DdrdHwTrainCtl0.Bits.hw_cal_en;
      }
      //
      // Read the hw_cal_en for CMD/CTL.
      //
      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        DdrccHwTrainCtl0.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_HW_TRAIN_CTL0_MCIO_DDRIOEXT_REG);
        FsmStillRunning |= DdrccHwTrainCtl0.Bits.hw_cal_en;
      }
      //
      // Read the hw_cal_en for CLK.
      //
      DdrclkHwTrainCtl0.Data = MemReadPciCfgEp (Socket, Ch, DDRCLK_HW_TRAIN_CTL0_MCIO_DDRIOEXT_REG);
      FsmStillRunning |= DdrclkHwTrainCtl0.Bits.hw_cal_en;
    } // Ch loop
    //
    // Poll for test completion
    if (BreakEarly) {
      TimerExpired = TRUE;
    } else {
      //
      // Check the polling time expired or not
      //
      if (FsmStillRunning != 0) {
        TimerExpired = CheckTimerExpiration (StartTsc, MaxEstimatedFsmTime);
      }
    }
    //
    // If this is a CTE build, delay between reads so we don't poll so often. For real silicon, the CSR access delay should be enough? : Yes. Confirmed with desgin.
    //
    CteDelayQclk (50);

  } while ((FsmStillRunning) && (!TimerExpired));

  if ((BreakEarly != TRUE) && (TimerExpired)) {
    RcDebugPrintWithDevice (
      SDBG_DEFAULT, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "HW FSM polling time expired\n"
      );
    return EFI_TIMEOUT;
  } else {
    return EFI_SUCCESS;
  }
} // PollHwFsmComplete

/**

  Get the IO Latency Comp Start Value

  @retval IO Lat Comp Start Value

**/
UINT8
EFIAPI
GetIoLatCompValueStart (
  VOID
  )
{
  return MRC_ROUND_TRIP_IO_COMP_START_DDR5;
} // GetIoLatCompValueStart

/**

  Get the IO Latency Comp Value

  @retval IO Lat Comp Value

**/
UINT8
EFIAPI
GetIoLatCompValue (
  VOID
  )
{
  return MRC_ROUND_TRIP_IO_COMPENSATION_DDR5;
} // GetIoLatCompValue

/**

DDRIO Init for Tx ReTraining.

@param[in] socket             - Current Socket
@param[in] TxReTrainDuration  - Tx ReTrain Duration Value
@param[in] TxReTrainInit      - Tx ReTrain Init Value

@retval N/A

**/
VOID
EFIAPI
ConfigureDdrioTxRetrain (
  IN UINT8     Socket,
  IN UINT8     TxReTrainDuration,
  IN UINT8     TxReTrainInit
  )
{
  UINT8 Ch;
  UINT8 Strobe;
  UINT8 MSVx4;
  struct channelNvram (*channelNvList) [MAX_CH];
  UINT8                                 MaxChDdr;
  UINT8                                 Dimm;
  DDRD_N0_TX_RETRAIN0_MCIO_DDRIO_STRUCT DdrdN0TxRetrain0;
  PSYSHOST        Host;

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();
  MSVx4 = GetMaxStrobeValid (Host);
  channelNvList = GetChannelNvList (Host, Socket);

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "ConfigureDdrioTxRetrain starts\n");

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*channelNvList) [Ch].enabled == 0) {
      continue;
    }
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      for (Strobe = 0; Strobe < MSVx4; Strobe++) {

        if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
          continue;
        }

        if (IsDcpmmPresentDimm (Socket, Ch, Dimm) == TRUE) {
          continue;
        }

      DdrdN0TxRetrain0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TX_RETRAIN0_MCIO_DDRIO_REG));
      DdrdN0TxRetrain0.Bits.txrt_dur = TxReTrainDuration;
      DdrdN0TxRetrain0.Bits.txrt_init = TxReTrainInit;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TX_RETRAIN0_MCIO_DDRIO_REG), DdrdN0TxRetrain0.Data);
      } // strobe loop
    } //Dimm
  } //Ch loop
} // ConfigureDdrioTxRetrain


/**

Configure the Re Train Enable bit in DDRIO.

@param[in] socket    - Socket Number
@param[in] Ch        - Ch number
@param[in] Dimm      - Dimm number
@param[in] Rank      - Rank number
@param[in] Value    -  Value to program

@retval N/A

**/
VOID
EFIAPI
ConfigureDdrioTxRetrainEnable (
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     Value
  )
{
  UINT8 Strobe;
  UINT8 MSVx4;
  DDRD_N0_TX_RETRAIN_RANK_0_MCIO_DDRIO_STRUCT DddrdN0TxReTrainRank0;
  UINT8                                 LogRank;
  PSYSHOST        Host;

  Host = GetSysHostPointer ();
  //
  // Get the logical rank #
  //
  LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);

  MSVx4 = GetMaxStrobeValid (Host);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "ConfigureDdrioTxRetrainEnable starts\n");
  for (Strobe = 0; Strobe < MSVx4; Strobe++) {
    if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
      continue;
    }
    DddrdN0TxReTrainRank0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegister10nm (Host, Ch, LogRank, Strobe, DDRD_N0_TX_RETRAIN_RANK_0_MCIO_DDRIO_REG));
    DddrdN0TxReTrainRank0.Bits.txrt_en = Value;
    MemWritePciCfgEp (Socket, Ch, UpdateIoRegister10nm (Host, Ch, LogRank, Strobe, DDRD_N0_TX_RETRAIN_RANK_0_MCIO_DDRIO_REG), DddrdN0TxReTrainRank0.Data);

  } // strobe loop
} // ConfigureDdrioTxRetrainEnable



/**

Apply the DDRIO Phy Update in Tx ReTraining.

@param[in] Socket    - Current Socket
@param[in] Ch        - Ch Number
@param[in] Dimm      - Dimm Number
@param[in] Rank      - Rank Number

@retval N/A

**/
VOID
EFIAPI
TxReTrainDdrioPhyUpdate (
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
  UINT8 Strobe;
  UINT8 MSVx4;
  struct channelNvram (*channelNvList) [MAX_CH];
  DDRD_N0_TX_RETRAIN0_MCIO_DDRIO_STRUCT DdrdN0TxRetrain0;
  PSYSHOST        Host;

  Host = GetSysHostPointer ();
  MSVx4 = GetMaxStrobeValid (Host);
  channelNvList = GetChannelNvList (Host, Socket);

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "TxReTrainDdrioPhyUpdate starts\n");

  for (Strobe = 0; Strobe < MSVx4; Strobe++) {

    if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
      continue;
    }

    DdrdN0TxRetrain0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TX_RETRAIN0_MCIO_DDRIO_REG));
    DdrdN0TxRetrain0.Bits.txrt_update = 1;
    MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TX_RETRAIN0_MCIO_DDRIO_REG), DdrdN0TxRetrain0.Data);
  } // strobe loop
  return;
} // TxReTrainDdrioPhyUpdate

/**

  Equalize CWL value between the two sub channels of one rank.

  @param[in] Host                 - Pointer to sysHost
  @param[in] Socket               - Current Socket
  @param[in] Ch                   - Channel number
  @param[in] Dimm                 - Dimm number
  @param[in] Rank                 - Rank number
  @param[in] MaxStrobe            - Maximum number of strobes
  @param[in] CwlAdj               - Cwl Adjust
  @param[in] WLChipCleanUpStruct  - WL Chip Clean Up Struct
  @param[in] TxDqsDelayValue      - The array is used to store the TxDqs delay value.

  @retval N/A

**/
VOID
EFIAPI
EqualizeCwl (
  IN  PSYSHOST                          Host,
  IN  UINT8                             Socket,
  IN  UINT8                             Ch,
  IN  UINT8                             Dimm,
  IN  UINT8                             Rank,
  IN  UINT8                             MaxStrobe,
  IN  INT16                             CwlAdj[MAX_CH][SUB_CH][MAX_RANK_CH],
  IN  CHIP_WL_CLEANUP_STRUCT            *WLChipCleanUpStruct,
  IN  INT16                             TxDqsDelayValue[MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE_DDR5 / 2]
  )
{
  INT16                             CwlAdjTargetValue;
  INT16                             CwlAdjDelta;
  UINT8                             SubChTarget;
  INT16                             PiLocal;
  UINT8                             Strobe;
  struct ddrRank                    (*RankList) [MAX_RANK_DIMM];
  UINT8                             SubChMSVx4;

  RankList = GetRankNvList (Host, Socket, Ch, Dimm);
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  //
  // Check the CWL ADJ is the same between two sub channels or not.
  //
  if (CwlAdj[Ch][SUB_CH_A][ (*RankList) [Rank].rankIndex] != CwlAdj[Ch][SUB_CH_B][ (*RankList) [Rank].rankIndex]) {
    //
    // Choose the minimum CWL ADJ as the target.
    //
    if (CwlAdj[Ch][SUB_CH_A][ (*RankList) [Rank].rankIndex] < CwlAdj[Ch][SUB_CH_B][ (*RankList) [Rank].rankIndex]) {
      CwlAdjTargetValue = CwlAdj[Ch][SUB_CH_A][ (*RankList) [Rank].rankIndex];
      CwlAdjDelta = CwlAdj[Ch][SUB_CH_B][ (*RankList) [Rank].rankIndex] - CwlAdj[Ch][SUB_CH_A][ (*RankList) [Rank].rankIndex];
      SubChTarget = SUB_CH_B;
    } else {
      CwlAdjTargetValue = CwlAdj[Ch][SUB_CH_B][ (*RankList) [Rank].rankIndex];
      CwlAdjDelta = CwlAdj[Ch][SUB_CH_A][ (*RankList) [Rank].rankIndex] - CwlAdj[Ch][SUB_CH_B][ (*RankList) [Rank].rankIndex];
      SubChTarget = SUB_CH_A;
    }

    CwlAdj[Ch][SubChTarget][ (*RankList) [Rank].rankIndex] = CwlAdjTargetValue;

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "Cwl Adj Target Value = %d, Cwl Adj Delta = %d, Sub Channel Target = %d\n", CwlAdjTargetValue, CwlAdjDelta, SubChTarget);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChTarget, Rank, NO_STROBE, NO_BIT, "Tx Dqs Delay after equalization:\n");

    if (CwlAdjDelta >= MAX_CWL_ADJ_BETWEEN_TWO_SUB_CHANEELS) {
      RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "Cwl Adj Delta %d between the two sub channels is too large\n", CwlAdjDelta);
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_104);
      return;
    }

    PiLocal = CwlAdjDelta * 128;
    ProgramDriveDqsLowDdr5 (Host, Socket, Ch, SubChTarget, 0);
    //
    // Program the CWL delta to TxDqs.
    //
    for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
      if (IsStrobeNotValidDdr5 (Host, Strobe)) {
        continue;
      }
      TxDqsDelayValue[Ch][SubChTarget][ (*RankList) [Rank].rankIndex][Strobe] += PiLocal;
      GetSetDataGroup (Host, Socket, Ch, SubChTarget, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, (INT16 *) &PiLocal);
    } // Strobe loop

    WriteChipWrADDDelaysDdr5 (Host, Socket, Ch, SubChTarget, CWLADJ_ALL, 0, CwlAdjTargetValue, WLChipCleanUpStruct);
    ProgramDriveDqsLowDdr5 (Host, Socket, Ch, SubChTarget, 1);
#ifdef DEBUG_CODE_BLOCK
    //
    // Display the TxDqs result after equalization.
    //
    DisplayWlCoarseResultsDdr5 (Host, Socket, Ch, Dimm, Rank, SubChTarget, MaxStrobe);
#endif // DEBUG_CODE_BLOCK
  }//if
} //EqualizeCwl


/**

  Equalize CWL value between the two sub channels for all the DIMMs and Ranks in this channel.

  @param[in] Host                 - Pointer to sysHost
  @param[in] Socket               - Current Socket
  @param[in] Ch                   - Channel number
  @param[in] WLChipCleanUpStruct  - WL Chip Clean Up Struct
  @param[in] CwlAdjTargetValueInput  - CWL target value

  @retval N/A

**/
VOID
EFIAPI
EqualizeCwlPerCh (
  IN  PSYSHOST                          Host,
  IN  UINT8                             Socket,
  IN  UINT8                             Ch,
  IN  CHIP_WL_CLEANUP_STRUCT            *WLChipCleanUpStruct,
  IN  INT16                             CwlAdj[SUB_CH]
  )
{
  INT16                             CwlAdjTargetValue;
  INT16                             CwlAdjDelta;
  UINT8                             SubChTarget;
  INT16                             PiLocal;
  UINT8                             Strobe;
  struct ddrRank                    (*RankList) [MAX_RANK_DIMM];
  UINT8                             SubChMSVx4;
  UINT8                             Dimm = 0;
  UINT8                             Rank = 0;
  UINT8                             MemoryControllerChannel = 0;
  UINT8                             NumChPerMc = 0;
  struct dimmNvram (*DimmNvList) [MAX_DIMM];

  UINT8 MaxStrobe = 0;

  RankList = GetRankNvList (Host, Socket, Ch, Dimm);
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);

  NumChPerMc = GetNumChannelPerMc ();

  MemoryControllerChannel = Ch % NumChPerMc;
  DimmNvList = GetDimmNvList (Host, Socket, Ch);


  //
  // Check the CWL ADJ is the same between two sub channels or not.
  //
  if (CwlAdj[SUB_CH_A] != CwlAdj[SUB_CH_B]) {
    //
    // Choose the minimum CWL ADJ as the target.
    //
    if (CwlAdj[SUB_CH_A] < CwlAdj[SUB_CH_B]) {
      CwlAdjTargetValue = CwlAdj[SUB_CH_A];
      CwlAdjDelta = CwlAdj[SUB_CH_B] - CwlAdj[SUB_CH_A];
      SubChTarget = SUB_CH_B;
    } else {
      CwlAdjTargetValue = CwlAdj[SUB_CH_B];
      CwlAdjDelta = CwlAdj[SUB_CH_A] - CwlAdj[SUB_CH_B];
      SubChTarget = SUB_CH_A;
    }

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "Cwl Adj Target Value = %d, Cwl Adj Delta = %d, Sub Channel Target = %d\n", CwlAdjTargetValue, CwlAdjDelta, SubChTarget);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChTarget, Rank, NO_STROBE, NO_BIT, "Tx Dqs Delay after equalization:\n");

    if (CwlAdjDelta >= MAX_CWL_ADJ_BETWEEN_TWO_SUB_CHANEELS) {
      RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "Cwl Adj Delta %d between the two sub channels is too large\n", CwlAdjDelta);
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_104);
      return;
    }
    PiLocal = CwlAdjDelta * 128;

    ProgramDriveDqsLowDdr5 (Host, Socket, Ch, SubChTarget, 0);

    //
    // Program the CWL delta to TxDqs.
    //
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList) [Dimm].dimmPresent == 0) {
        continue;
      }
      MaxStrobe = GetMaxStrobe (Host, Socket, Ch, Dimm, WrLvlDelay, DdrLevel) / SUB_CH;
      for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
          if (IsStrobeNotValidDdr5 (Host, Strobe)) {
            continue;
          }

          GetSetDataGroup (Host, Socket, Ch, SubChTarget, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, (INT16 *) &PiLocal);
        } // Strobe loop
#ifdef DEBUG_CODE_BLOCK
        //
        // Display the TxDqs result after equalization.
        //
        DisplayWlCoarseResultsDdr5 (Host, Socket, Ch, Dimm, Rank, SubChTarget, MaxStrobe);
#endif //DEBUG_CODE_BLOCK
      } //rank
    } //dimm

    WriteChipWrADDDelaysDdr5 (Host, Socket, Ch, SubChTarget, CWLADJ_ALL, 0, CwlAdjTargetValue, WLChipCleanUpStruct);
    ProgramDriveDqsLowDdr5 (Host, Socket, Ch, SubChTarget, 1);

  }//if
} //EqualizeCwlPerCh


  /**

  Get the rx_analog_en_early value for DDR5.

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number.

  @retval UINT8        Value for rx_analog_en_early.

  **/

UINT8
GetRxAnalogEnEarlyDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  UINT16 PreambleLen = 0;
  UINT8 Value = 0;

  PreambleLen = GetPreambleState (Socket, READ_PREAMBLE);
  switch (Host->nvram.mem.socket[Socket].ddrFreq) {
    case DDR_3200:
      if (PreambleLen == PREAMBLE_1TCLK) {
        Value = 0;
      } else {
        Value = 1;
      }
      break;
    case DDR_3600:
      Value = 1;
      break;
    case DDR_4000:
      if ((PreambleLen == PREAMBLE_1TCLK) || (PreambleLen == PREAMBLE_2TCLK)) {
        Value = 1;
      } else {
        Value = 2;
      }
      break;
    case DDR_4400:
      Value = 2;
      break;
    case DDR_4800:
      if ((PreambleLen == PREAMBLE_1TCLK) || (PreambleLen == PREAMBLE_2TCLK)) {
        Value = 2;
      } else {
        Value = 3;
      }
      break;
    default:
      RcDebugPrintWithDevice (SDBG_ERROR, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Warning: the DDR frequency %d is not supported. Choose 3200.\n", Host->nvram.mem.socket[Socket].ddrFreq);
      if (PreambleLen == PREAMBLE_1TCLK) {
        Value = 0;
      } else {
        Value = 1;
      }
      break;
  }
  return Value;
} // GetRxAnalogEnEarlyDdr5


/**
  Get the DDRINTF_DATA_TIMING1*_CH* register offset for a particular channel and DIMM.

  This additionally provides bounds checking for the channel and DIMM values.

  @param[in]     Ch                    The target channel number on the memory controller
  @param[in]     SubCh                 The target subchannel number on the memory controller
  @param[in out] DataTiming1RegOffset  The DDRINTF_DATA_TIMING1_*_CH* register offset for the target channel and DIMM

  @retval MRC_STATUS_SUCCESS   Register offset was successfully determined
  @retval !MRC_STATUS_SUCCESS  Register offset was not successfully determined

**/
MRC_STATUS
GetIntfDataTiming1RegOffset (
  IN      UINT8   Ch,
  IN      UINT8   SubCh,
  IN OUT  UINT32  *DataTiming1RegOffset
  )
{
  MRC_STATUS  Status = MRC_STATUS_SUCCESS;
  UINT8       Index;
  UINT32      DdrintfDataTiming1RegOffsets[] = { DDRINTF_DATA_TIMING1_CH_0_MCIO_DDRIO_REG,// Ch0A
                                                 DDRINTF_DATA_TIMING1_CH_1_MCIO_DDRIO_REG,// Ch0B
                                                 DDRINTF_DATA_TIMING1_CH_2_MCIO_DDRIO_REG,// Ch1A
                                                 DDRINTF_DATA_TIMING1_CH_3_MCIO_DDRIO_REG // Ch1B
                                               };

  Index = (Ch*SUB_CH + SubCh) % MAX_MC_SCH;

  if (Index >= ((sizeof DdrintfDataTiming1RegOffsets) / (sizeof DdrintfDataTiming1RegOffsets[0]))) {
    RcDebugPrint (SDBG_ERROR, "\nGetDataTiming1RegOffset: Ch(%d) or SubCh(%d) out of bounds\n", Ch, SubCh);
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_212);
    Status = MRC_STATUS_FAILURE;
  } else {
    *DataTiming1RegOffset = DdrintfDataTiming1RegOffsets[Index];
  }

  return Status;
}

/**
  Program Rx Analog En Early Value

  @param[in]  Socket    Socket number

  @retval MRC_STATUS_SUCCESS   Register offset was successfully determined and programmed
  @retval !MRC_STATUS_SUCCESS  Register offset was not successfully determined and not programmed

**/
MRC_STATUS
EFIAPI
ProgramRxAnalogEnEarly (
  IN      UINT8   Socket
  )
{
  PSYSHOST    Host;
  UINT8       MaxChDdr;
  UINT8       Ch = 0;
  UINT8       SubCh = 0;
  MRC_STATUS  Status = MRC_STATUS_SUCCESS;
  DDRINTF_DATA_TIMING1_CH_0_MCIO_DDRIO_STRUCT DdrintfDataTiming1;
  UINT8       MemoryControllerCh;
  UINT8       NumChPerMc;
  NumChPerMc = GetNumChannelPerMc ();
  UINT32     DataTiming1RegOffset;

  UINT8 Value = 0;
  MaxChDdr = GetMaxChDdr ();

  Host = GetSysHostPointer ();
  //
  // Get the value to program
  //
  Value = GetRxAnalogEnEarlyDdr5 (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    MemoryControllerCh = Ch % NumChPerMc;

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

      Status = GetIntfDataTiming1RegOffset (MemoryControllerCh, SubCh, &DataTiming1RegOffset);
      if (Status != MRC_STATUS_SUCCESS) {
        continue;
      }
      DdrintfDataTiming1.Data = MemReadPciCfgEp (Socket, Ch, DataTiming1RegOffset);
      //
      // Set the rx_analog_en_early value.
      //
      DdrintfDataTiming1.Bits.rx_analog_en_early = Value;
      MemWritePciCfgEp (Socket, Ch, DataTiming1RegOffset, DdrintfDataTiming1.Data);

    }
  } //Ch loop
  //
  // After programing the rx_analog_en_early, it requires a train_rst to be issued to DDRIO.
  //
  IO_Reset (Host, Socket);

  return Status;
}

/**

  Get the ANALOG_TUNING Value

  @retval analog_tuning Value

**/
UINT8
EFIAPI
GetAnalogTuningValue (
  IN      PSYSHOST                Host,
  IN      UINT8                   Socket,
  IN      UINT8                   Channel
  )
{
  UINT8 Value = 0;
  DDRD_N0_DLL_CSR_MCIO_DDRIO_STRUCT DdrdN0DllCsr;
  DdrdN0DllCsr.Data = MemReadPciCfgEp (Socket, Channel, UpdateIoRegisterCh10nm (Host, Channel, 0, DDRD_N0_DLL_CSR_MCIO_DDRIO_REG));
  Value = (UINT8) DdrdN0DllCsr.Bits.analog_tuning;
  return Value;
} // GetAnalogTuningValue
