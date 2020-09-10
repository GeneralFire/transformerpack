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
#include <Library/MemMcIpLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/RcMemAllocLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CheckpointLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/IoAccessLib.h>
#include <Library/CteNetLib.h>
#include <Library/MemDdrIoIpTargetLib.h>
#include <Library/TimerLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemRoutingLib.h>
#include <Library/SwizzleLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Memory/JedecDefinitions.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Library/MemRcLib.h>

#include <MemProjectSpecific.h>
#include <PlatformInfoTypes.h>

#ifdef DDR5_SUPPORT
#include <Library/Ddr5CoreLib.h>
#endif
#define SENSEAMP_DELAY_MAX       8
#define SENSEAMP_DELAY_CONVERT_NEGATIVE_SETTING 16
#define DDRCRINTFDATATIMING0_CH_MAX 3
#define RDDATA_VALID_EARLY_CYCLES    4
#define DQ_LOW_MASK    0x3
#define DQ_HIGH_MASK   0xC

VOID
ProgramJedecTimingsDdrioBrs(
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Ch,
  IN WRITE_TIMINGS    *OriginalWriteTimings
  );

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
  UINT8                                     ch;
  UINT8                                     strobe;
  struct channelNvram(*channelNvList)[MAX_CH];
  DATACONTROL0N0_0_MCIO_DDRIO_STRUCT            dataControl0;
  DATACONTROL2N0_0_MCIO_DDRIO_STRUCT            dataControl2;

  UINT8 socket = Host->var.mem.currentSocket;
  UINT8 MSVx4 = GetMaxStrobeValid(Host);
  UINT8 MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, socket);

  // HSD 4928504
  //
  // TXPION, RXPION, FORCERXON (moved from memLate)
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    dataControl0.Data = (*channelNvList)[ch].dataControl0;
    dataControl0.Bits.internalclockson = FixedPcdGet8 (PcdDdrInternalClocksOn);
    dataControl0.Bits.txpion = 0;
    dataControl0.Bits.rxpion = 0;

    for (strobe = 0; strobe < MSVx4; strobe++) {
      if (IsStrobeNotValid(Host, strobe)) {
        continue;
      }
      WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, dataControl0.Data);
      dataControl2.Data = (*channelNvList)[ch].dataControl2[strobe];
      if (IsSiliconWorkaroundEnabled ("S1408263639")) {
        dataControl2.Bits.forcerxon = 1;
      } else {
        dataControl2.Bits.forcerxon = 0;
      }
      WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL2N0_0_MCIO_DDRIO_REG, dataControl2.Data);
      (*channelNvList)[ch].dataControl2[strobe] = dataControl2.Data;
    } // strobe loop
    (*channelNvList)[ch].dataControl0 = dataControl0.Data;
  } // ch loop
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
  DDRCRINTFDATATIMING0_CH0_MCIO_DDRIO_STRUCT IntfDataTiming0;
  UINT32                                     IntfDataTiming0Ch[] = { DDRCRINTFDATATIMING0_CH0_MCIO_DDRIO_REG,
                                                                     DDRCRINTFDATATIMING0_CH1_MCIO_DDRIO_REG,
                                                                     DDRCRINTFDATATIMING0_CH2_MCIO_DDRIO_REG
                                                                   };
  struct channelNvram                        (*ChannelNvList)[MAX_CH];
  UINT8                                      Index;
  UINT8                                      NumChPerMc;

  ChannelNvList = GetChannelNvList (Host, Socket);
  NumChPerMc    = GetNumChannelPerMc ();

  Index = Ch % NumChPerMc;
  IntfDataTiming0.Data = MemReadPciCfgEp (Socket, Ch, IntfDataTiming0Ch[Index]);
  IntfDataTiming0.Bits.t_cwl_ddrt = Cwl;
  MemWritePciCfgEp (Socket, Ch, IntfDataTiming0Ch[Index], IntfDataTiming0.Data);
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
#ifdef DDRT_SUPPORT
  DDRCRINTFDATATIMING0_CH0_MCIO_DDRIO_STRUCT IntfDataTiming0;
  UINT32                                     IntfDataTiming0Ch[] = { DDRCRINTFDATATIMING0_CH0_MCIO_DDRIO_REG,
                                                                     DDRCRINTFDATATIMING0_CH1_MCIO_DDRIO_REG,
                                                                     DDRCRINTFDATATIMING0_CH2_MCIO_DDRIO_REG
                                                                   };
  UINT8                                      Index;
  UINT8                                      NumChPerMc;

  NumChPerMc    = GetNumChannelPerMc ();

  Index = Ch % NumChPerMc;
  ASSERT (Index < DDRCRINTFDATATIMING0_CH_MAX);


  IntfDataTiming0.Data = MemReadPciCfgEp (Socket, Ch, IntfDataTiming0Ch[Index]);
  IntfDataTiming0.Bits.rddata_valid_early = RDDATA_VALID_EARLY_CYCLES;
  MemWritePciCfgEp (Socket, Ch, IntfDataTiming0Ch[Index], IntfDataTiming0.Data);
#endif // DDRT_SUPPORT
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
  DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_STRUCT          ddrCRClkRanksEnabled;

  ddrCRClkRanksEnabled.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG);
  ddrCRClkRanksEnabled.Bits.ranken = RankEn;
  MemWritePciCfgEp (Socket, Ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG, ddrCRClkRanksEnabled.Data);
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
  UINT8                                           Strobe;
  INT16                                           RxDqsPSelValue;
  struct dimmNvram (*DimmNvList)[MAX_DIMM];
  struct channelNvram (*ChannelNvList)[MAX_CH];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;

  ChannelNvList = GetChannelNvList (Host, Socket);
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  for (Strobe = 0; Strobe < MSVx4; Strobe++) {

    // RxDqsPsel = 1 means OTHER amplifier output.
    // All Bytes, Nibble 0 = always 0
    // All Bytes, Nibble 1 = 1 if x8
    // All Bytes, Nibble 1 = 0 if x4
    if ((*DimmNvList)[Dimm].x4Present || (Strobe < MSVx8)) {
      RxDqsPSelValue = 0;
    } else {
      RxDqsPSelValue = 1;
    }
    GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, NO_BIT, DdrLevel, RxFifoDqsPBitSkew,
                           GSM_UPDATE_CACHE, &RxDqsPSelValue);
  } // Strobe loop
}

/**
  Reads or Writes the Sense Amp or Rx skewed DQSN/P offset parameter

  @param[in]  Host           - Pointer to sysHost
  @param[in]  Socket         - Socket number
  @param[in]  Ch             - Channel number (0-based)
  @param[in]  Dimm           - Dimm number (0-based)
  @param[in]  Rank           - Rank number (0-based)
  @param[in]  Strobe         - Strobe number (0-based)
  @param[in]  Bit            - Bit number (0-based)
  @param[in]  Level          - IO level to access
  @param[in]  Group          - Group number (RxSenseAmpOffset, RxFifoDqsPBitSkew, or RxFifoDqsNBitSkew)
  @param[in]  Mode           - GSM_READ_CSR - Read the data from hardware and not cache
                             - GSM_READ_ONLY - Do not write
                             - GSM_WRITE_OFFSET - Write offset and not value
                             - GSM_FORCE_WRITE - Force the write
  @param[in/out] Value       - pointer to a value

  @retval MRC_STATUS

**/
MRC_STATUS
EFIAPI
GetSetSenseAmpRxOffsetDdrio (
  IN PSYSHOST               Host,
  IN UINT8                  Socket,
  IN UINT8                  Ch,
  IN UINT8                  Dimm,
  IN UINT8                  Rank,
  IN UINT8                  Strobe,
  IN UINT8                  Bit,
  IN MRC_LT                 Level,
  IN MRC_GT                 Group,
  IN UINT8                  Mode,
  IN OUT INT16              *Value
  )
{
  EFI_MEMORY_TOPOLOGY_TYPE              MemoryTopology;
  UINT8                                 MaxStrobe;
  UINT8                                 CurrentBit;
  UINT8                                 MaxBit;
  UINT8                                 SwizzleBit;
  UINT8                                 NibbleIndex;
  UINT8                                 NibbleOffset;
  UINT8                                 NibbleStartIndex;
  UINT8                                 StrobeN[MAX_NIBBLES_PER_BYTE];
  UINT16                                CurrentPerBitValue[MAX_BITS_IN_BYTE];
  UINT16                                UsDelay;
  UINT8                                 LogRank;
  UINT16                                MaxLimit = 0;
  UINT16                                MinLimit = 0;
  UINT16                                TempValue;
  RXOFFSETN0RANK0_0_MCIO_DDRIO_STRUCT   RxOffset[MAX_NIBBLES_PER_BYTE];
  UINT8                                 MSVx4;
  UINT8                                 MSVx8;

  struct channelNvram(*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);
  MSVx4 = GetMaxStrobeValid (Host);
  MSVx8 = GetMaxStrobeValid (Host) / 2;
  //
  // Initialization to 0
  //
  ZeroMem ((UINT8 *)CurrentPerBitValue, sizeof(CurrentPerBitValue));

  //
  // Get the logical rank #
  //
  LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);

  MemoryTopology = GetMemoryTopologyConfiguration (Host, Socket, Ch);

  //
  // DATATRAINFEEDBACK assumes rank 0 is always populated for SenseAmp, when 1DPC and DIMM is not 0
  // (TTopology or Inverted Daisy chain), the logical rank is different to 0 and breaks the DATATRAINFEEDBACK assumption
  //
  if ((Group == RxSenseAmpOffset) && (LogRank != 0) && ((*ChannelNvList)[Ch].maxDimm == 1) && (MemoryTopology != DaisyChainTopology)) {
    LogRank = Rank;
  }

  //
  // Get minimum and maximum value for the overall SenseAmpOffset
  //
  GetDataGroupLimits (Host, DdrLevel, Group, &MinLimit, &MaxLimit, &UsDelay);

  if (Strobe >= MAX_STROBE && Strobe != ALL_STROBES) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
                   "The strobe value is out of range!\n");
    RC_FATAL_ERROR ((Strobe < MAX_STROBE || Strobe == ALL_STROBES), ERR_ARRAY_OUT_OF_BOUNDS, RC_FATAL_ERROR_MINOR_CODE_2);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  if (Strobe == ALL_STROBES) {
    Strobe    = 0;
    MaxStrobe = MSVx4;
  } else {
    MaxStrobe = Strobe + 1;
  }

  for (; Strobe < MaxStrobe; Strobe++) {

    StrobeN[INDEX_NIBBLE0] = Strobe % MSVx8;
    StrobeN[INDEX_NIBBLE1] = (Strobe % MSVx8) + MSVx8;

    NibbleStartIndex = Strobe / MSVx8;

    if (Bit >= BITS_PER_NIBBLE) {
      CurrentBit = 0;
      MaxBit = BITS_PER_NIBBLE;
    } else {
      CurrentBit = Bit;
      MaxBit    = Bit + 1;
    }
    //
    // Special case RxFifoDqsPBitSkew and RxFifoDqsNBitSkew group as the bit input is not meaningful
    //
    if ((Group == RxFifoDqsPBitSkew) || (Group == RxFifoDqsNBitSkew)) {
      CurrentBit = 0;
      MaxBit    = 1;
    }

    if ((Mode & GSM_READ_CSR) != 0) {
      //
      // Read from PCI config space
      //
      RxOffset[INDEX_NIBBLE0].Data  = ReadDdrioDataReg (Host, Socket, Ch, LogRank, StrobeN[INDEX_NIBBLE0], RXOFFSETN0RANK0_0_MCIO_DDRIO_REG);
      RxOffset[INDEX_NIBBLE1].Data  = ReadDdrioDataReg (Host, Socket, Ch, LogRank, StrobeN[INDEX_NIBBLE1], RXOFFSETN0RANK0_0_MCIO_DDRIO_REG);

    } else {
      //
      // Read from cache
      //
      RxOffset[INDEX_NIBBLE0].Data = (*ChannelNvList)[Ch].rxOffset[LogRank][ StrobeN[INDEX_NIBBLE0] ];
      RxOffset[INDEX_NIBBLE1].Data = (*ChannelNvList)[Ch].rxOffset[LogRank][ StrobeN[INDEX_NIBBLE1] ];
    }
    switch (Group){
      case RxSenseAmpOffset:
        for (NibbleIndex = 0; NibbleIndex < MAX_NIBBLES_PER_BYTE; NibbleIndex++) {
          NibbleOffset = NibbleIndex * BITS_PER_NIBBLE;

          CurrentPerBitValue[0 + NibbleOffset] = (UINT16)RxOffset[NibbleIndex].Bits.saoffset0;
          CurrentPerBitValue[1 + NibbleOffset] = (UINT16)RxOffset[NibbleIndex].Bits.saoffset1;
          CurrentPerBitValue[2 + NibbleOffset] = (UINT16)RxOffset[NibbleIndex].Bits.saoffset2;
          CurrentPerBitValue[3 + NibbleOffset] = (UINT16)RxOffset[NibbleIndex].Bits.saoffset3;
        }

        break;
      case RxFifoDqsPBitSkew:
        CurrentPerBitValue[0] = (UINT16)RxOffset[NibbleStartIndex].Bits.rxdqspsel;
        break;
      case RxFifoDqsNBitSkew:
        CurrentPerBitValue[0] = (UINT16)RxOffset[NibbleStartIndex].Bits.rxdqsnsel;
        break;
      default:
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
                       "%a group not supported in this function!\n", GetGroupStr (Group));
        RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_53);
        break;
    }
    if (Group == RxSenseAmpOffset) {
      SwizzleBit = GetDqSwizzle (Ch, Strobe, CurrentBit, DdrLevel, RxSenseAmpOffset);
    } else {
      SwizzleBit = CurrentBit;
    }
    //
    // Check if read only
    //
    if ((Mode & GSM_READ_ONLY) != 0) {
      //
      // Save the value read into the output parameter
      // note in the case of ALL_BITS, bit 0 will be read
      *Value = CurrentPerBitValue[SwizzleBit];
    } else {
      //
      // Write
      //
      for (; CurrentBit < MaxBit; CurrentBit++) {
        if (Group == RxSenseAmpOffset) {
          SwizzleBit = GetDqSwizzle (Ch, Strobe, CurrentBit, DdrLevel, RxSenseAmpOffset);
        } else {
          SwizzleBit = CurrentBit;
        }
        // Adjust the current value by offset
        if (Mode & GSM_WRITE_OFFSET) {
          //
          // Add the offset to the current value
          //
          TempValue = CurrentPerBitValue[SwizzleBit] + *Value;
        } else {
          //
          // Write absolute value
          //
          TempValue = *Value;
        }
        //
        // Make sure we do not exeed the limits
        //
        if (TempValue < MinLimit) {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, CurrentBit,
            "%a Out of range!! Value = 0x%x, MinLimit = 0x%x\n", GetGroupStr (Group), TempValue, MinLimit);
          TempValue = MinLimit;
        }
        if (TempValue > MaxLimit) {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, CurrentBit,
            "%a Out of range!! Value = 0x%x, MaxLimit = 0x%x\n", GetGroupStr (Group), TempValue, MaxLimit);
          TempValue = MaxLimit;
        }

        if (Group == RxSenseAmpOffset) {
          CurrentPerBitValue[SwizzleBit] = TempValue;

          for (NibbleIndex = 0; NibbleIndex < MAX_NIBBLES_PER_BYTE; NibbleIndex++) {
            NibbleOffset = NibbleIndex * BITS_PER_NIBBLE;

            RxOffset[NibbleIndex].Bits.saoffset0 = CurrentPerBitValue[0 + NibbleOffset];
            RxOffset[NibbleIndex].Bits.saoffset1 = CurrentPerBitValue[1 + NibbleOffset];
            RxOffset[NibbleIndex].Bits.saoffset2 = CurrentPerBitValue[2 + NibbleOffset];
            RxOffset[NibbleIndex].Bits.saoffset3 = CurrentPerBitValue[3 + NibbleOffset];
          }
        } else if (Group == RxFifoDqsPBitSkew) {
          RxOffset[NibbleStartIndex].Bits.rxdqspsel = TempValue;
        } else {
          RxOffset[NibbleStartIndex].Bits.rxdqsnsel = TempValue;
        }
      } //Bit Loop
      //
      // Write it back if the current data does not equal the cache value or if in force write mode
      //
      if ((RxOffset[INDEX_NIBBLE0].Data != (*ChannelNvList)[Ch].rxOffset[LogRank][ StrobeN[INDEX_NIBBLE0] ]) || (Mode & GSM_FORCE_WRITE)) {
        WriteDdrioDataReg (Host, Socket, Ch, LogRank,  StrobeN[INDEX_NIBBLE0] , RXOFFSETN0RANK0_0_MCIO_DDRIO_REG, RxOffset[INDEX_NIBBLE0].Data);
      }
      if ((RxOffset[INDEX_NIBBLE1].Data != (*ChannelNvList)[Ch].rxOffset[LogRank][ StrobeN[INDEX_NIBBLE1] ]) || (Mode & GSM_FORCE_WRITE)) {
        WriteDdrioDataReg (Host, Socket, Ch, LogRank,  StrobeN[INDEX_NIBBLE1] , RXOFFSETN0RANK0_0_MCIO_DDRIO_REG, RxOffset[INDEX_NIBBLE1].Data);
      }
      //
      // Wait for the new value to settle
      //
      FixedDelayMicroSecond (UsDelay);

    } // if read only

    //
    // Save to cache
    //
    if ((Mode & GSM_UPDATE_CACHE) != 0) {
      (*ChannelNvList)[Ch].rxOffset[LogRank][ StrobeN[INDEX_NIBBLE0] ] = RxOffset[INDEX_NIBBLE0].Data;
      (*ChannelNvList)[Ch].rxOffset[LogRank][ StrobeN[INDEX_NIBBLE1] ] = RxOffset[INDEX_NIBBLE1].Data;
    }
  } // strobe loop
  return MRC_STATUS_SUCCESS;
}

/**
  Change the receive sense amp setting to a centered value, effectively removing any offset.

  @retval N/A
**/
VOID
EFIAPI
CenterRxSenseAmp (
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
  INT16                               MiddleValue = 0xF;
  UINT8                               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;
  ChannelNvList = GetChannelNvList (Host, Socket);
  //
  // Set all bit offset values for SaOffset bits to the middle (15d / 0xF)
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }
      for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {
        if (CheckRank(Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList)[Dimm].numRanks)) {
          continue;
        }
        GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, ALL_STROBES, ALL_BITS, DdrLevel, RxSenseAmpOffset, GSM_UPDATE_CACHE, &MiddleValue);
      } //Rank
    } //Dimm
  } //Ch
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
  CenterRxSenseAmp ();
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
  CenterRxSenseAmp ();
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
  DDRCRCTLCOMPOFFSET_CMDN_MCIO_DDRIOEXT_STRUCT ddrCRCtlCompOffsetCmdN;
  DDRCRCTLCOMPOFFSET_CMDS_MCIO_DDRIOEXT_STRUCT ddrCRCtlCompOffsetCmdS;
  DDRCRCTLCOMPOFFSET_CKE_MCIO_DDRIOEXT_STRUCT  ddrCRCtlCompOffsetCke;
  DDRCRCTLCOMPOFFSET_CTL_MCIO_DDRIOEXT_STRUCT  ddrCRCtlCompOffsetCtl;
  struct channelNvram(*channelNvList)[MAX_CH];
  UINT8     MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*channelNvList)[Ch].enabled == 0) {
      continue;
    }

    ddrCRCtlCompOffsetCmdN.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCTLCOMPOFFSET_CMDN_MCIO_DDRIOEXT_REG);
    ddrCRCtlCompOffsetCmdS.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCTLCOMPOFFSET_CMDS_MCIO_DDRIOEXT_REG);
    ddrCRCtlCompOffsetCke.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCTLCOMPOFFSET_CKE_MCIO_DDRIOEXT_REG);
    ddrCRCtlCompOffsetCtl.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCTLCOMPOFFSET_CTL_MCIO_DDRIOEXT_REG);
    ddrCRCtlCompOffsetCmdN.Bits.iodirectionvalid = 1;
    ddrCRCtlCompOffsetCmdS.Bits.iodirectionvalid = 1;
    ddrCRCtlCompOffsetCke.Bits.iodirectionvalid = 1;
    ddrCRCtlCompOffsetCtl.Bits.iodirectionvalid = 1;

    MemWritePciCfgEp (Socket, Ch, DDRCRCTLCOMPOFFSET_CMDN_MCIO_DDRIOEXT_REG, ddrCRCtlCompOffsetCmdN.Data);
    MemWritePciCfgEp (Socket, Ch, DDRCRCTLCOMPOFFSET_CMDS_MCIO_DDRIOEXT_REG, ddrCRCtlCompOffsetCmdS.Data);
    MemWritePciCfgEp (Socket, Ch, DDRCRCTLCOMPOFFSET_CKE_MCIO_DDRIOEXT_REG, ddrCRCtlCompOffsetCke.Data);
    MemWritePciCfgEp (Socket, Ch, DDRCRCTLCOMPOFFSET_CTL_MCIO_DDRIOEXT_REG, ddrCRCtlCompOffsetCtl.Data);
  }
}


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
  UINT8                                         McId;
  UINT8                                         Ch;
  UINT8                                         Strobe;
  DATACONTROL2N0_0_MCIO_DDRIO_STRUCT            dataControl2;        //data
  DDRCRCOMPCTL2_MCIO_DDRIOEXT_STRUCT            ddrCompCtl2;
  struct channelNvram(*channelNvList)[MAX_CH];
  UINT8 MSVx4 = GetMaxStrobeValid(Host);
  UINT8 MaxChDdr;
  UINT8 MaxImc;

  MaxChDdr      = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, Socket);
  MaxImc        = GetMaxImc ();

  //
  // TODO: remove this loop after CSR trace correlation
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*channelNvList)[Ch].enabled == 0) {
      continue;
    }
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValid(Host, Strobe)) {
        continue;
      }
      dataControl2.Data = ReadDdrioDataRegCh (Host, Socket, Ch, Strobe, DATACONTROL2N0_0_MCIO_DDRIO_REG);
#if (MEM_IP_VER == MEM_IP_TODO)
      // ICX_TODO
      dataControl2.Bits.enearlywriterankqnnnh = 0;
#endif // (MEM_IP_VER == MEM_IP_TODO)
      WriteDdrioDataRegCh (Host, Socket, Ch, Strobe, DATACONTROL2N0_0_MCIO_DDRIO_REG, dataControl2.Data);
    } // strobe loop
  } // ch loop

  // 4930051: switching to normal mode, please set lvlshft_holden=1 for all data,cmd,ctl,clk fubs for SKX and NVMCTLR
  for (McId = 0; McId < MaxImc; McId++) {
    if (Host->var.mem.socket[Socket].imcEnabled[McId] == 0) {
      continue;
    }
    //comp
    ddrCompCtl2.Data = MemReadPciCfgMC (Socket, McId, DDRCRCOMPCTL2_MCIO_DDRIOEXT_REG);
    ddrCompCtl2.Bits.lvlshft_holden = 0;
    MemWritePciCfgMC (Socket, McId, DDRCRCOMPCTL2_MCIO_DDRIOEXT_REG, ddrCompCtl2.Data);
  }
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
  DDRCRCMDCONTROLS1_CMDS_MCIO_DDRIOEXT_STRUCT ddrCRCmdControls1CmdS;
  UINT8       Ch;
  UINT8       MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    ddrCRCmdControls1CmdS.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDCONTROLS1_CMDS_MCIO_DDRIOEXT_REG);
    ddrCRCmdControls1CmdS.Bits.selectrefresh = 1;
    MemWritePciCfgEp (Socket, Ch, DDRCRCMDCONTROLS1_CMDS_MCIO_DDRIOEXT_REG, ddrCRCmdControls1CmdS.Data);
  } // ch loop
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
  DDRCRCOMPCTL0_MCIO_DDRIOEXT_STRUCT ddrCRCompCtl0;
  // ICX_TODO: lvmodevalid is never checked but should be.

  ddrCRCompCtl0.Data = MemReadPciCfgEp (socket, 0, DDRCRCOMPCTL0_MCIO_DDRIOEXT_REG);
  return (UINT8)ddrCRCompCtl0.Bits.lvmode;
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
  DATACONTROL0N0_0_MCIO_DDRIO_STRUCT      dataControl0;
  UINT8                                         Strobe;
  struct channelNvram(*channelNvList)[MAX_CH];
  UINT8 MSVx4 = GetMaxStrobeValid(Host);

  channelNvList = GetChannelNvList(Host, Socket);

  dataControl0.Data = (*channelNvList)[Ch].dataControl0;
  dataControl0.Bits.txon = TxOn;
  for (Strobe = 0; Strobe < MSVx4; Strobe++) {
    if (IsStrobeNotValid(Host, Strobe)) {
      continue;
    }
    WriteDdrioDataReg (Host, Socket, Ch, 0, Strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, dataControl0.Data);
  } // strobe loop

  dataControl0.Data = (*channelNvList)[Ch].dataControl0;
  return dataControl0.Bits.txon;
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
  UINT8 ChannelLocal;
  DATACONTROL0N0_0_MCIO_DDRIO_STRUCT  DataControl0;
  struct channelNvram                 (*ChannelNvList)[MAX_CH];
  UINT8                               SubCh = 0;
  UINT8                               MaxStrobePairs;

  // Due to BRS DDRIO swizzling, txon must be set for both strobe pairs since DQs will be driven for different
  // DDRIO nibbles
  MaxStrobePairs = GetSubChMaxStrobeValid (Host) / 2;
  ChannelLocal = Ch;
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if ((SubChMask & (1 << SubCh)) == 0) {
      continue;
    }
    GetChannelMapping (Ch, SubCh, &ChannelLocal);

    ChannelNvList = GetChannelNvList(Host, Socket);

    DataControl0.Data = (*ChannelNvList)[ChannelLocal].dataControl0;
    DataControl0.Bits.txon = TxOn;
    WriteDdrioDataReg (Host, Socket, ChannelLocal, 0, Strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, DataControl0.Data);
    if (Strobe < MaxStrobePairs) {
      Strobe = Strobe + MaxStrobePairs;
    } else {
      Strobe = Strobe - MaxStrobePairs;
    }
    // Now set txon for its pair
    WriteDdrioDataReg (Host, Socket, ChannelLocal, 0, Strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, DataControl0.Data);
  }//SubCh
  return;
} //SetDataTxOnDdrioDdr5

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
GetCachedRcvenXoverOffsetDdrio (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Strobe
  )
{
  DATAOFFSETTRAINN0_0_MCIO_DDRIO_STRUCT           dataOffsetTrain;
  struct channelNvram(*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(Host, Socket);
  dataOffsetTrain.Data = (*channelNvList)[Ch].dataOffsetTrain[Strobe];

  return dataOffsetTrain.Bits.rcvenoffset;
}

/**

  Program registers for tCL and tWL\tCWL for ICX

  @param  Host        - Ptr to SYSHOST, system Host (root) structure
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
  INT16                                     Value = 0;
  UINT32                                    RetVal = 0;
  //
  //Implement with GetSetFunc
  //
  GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, Bit, DdrLevel, RxSenseAmpOffset, GSM_READ_ONLY, &Value);

  RetVal = (UINT32) Value;
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
  return ReadDdrioDataRegCh (Host, socket, ch, strobe, DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG);
}

/**
  Set IOLatency in HW and update the cache if requested

  @param Host           - Point to sysHost
  @param socket         - Socket number
  @param ch             - Channel number
  @param ioLateIoComp   - Pointer to IOLatency

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
  DDRCRINTFIOLATENCY1_CH0_MCIO_DDRIO_STRUCT IoLatency1;
  UINT32                                    IoLatency1Ch[] = {DDRCRINTFIOLATENCY1_CH0_MCIO_DDRIO_REG,
                                                              DDRCRINTFIOLATENCY1_CH1_MCIO_DDRIO_REG,
                                                              DDRCRINTFIOLATENCY1_CH2_MCIO_DDRIO_REG
                                                             };
  struct channelNvram                       (*channelNvList)[MAX_CH];
  UINT8                                     Index;
  UINT8                                     ChannelLocal;
  UINT8                                     NumChPerMc;
  UINT8                                     MaxChDdr;

  MaxChDdr   = GetMaxChDdr ();
  NumChPerMc = GetNumChannelPerMc ();

  ChannelLocal = Ch;
  if ((IsBrsPresent(Host, Socket)) && (SubCh != NO_SUBCH)) {
    GetChannelMapping(Ch, SubCh, &ChannelLocal);
  }
  if (ChannelLocal >= MaxChDdr) {
    // invalid ch specified
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_125);
    return;
  }

  channelNvList = GetChannelNvList(Host, Socket);

  Index = ChannelLocal % NumChPerMc;

  IoLatency1.Data = MemReadPciCfgEp (Socket, ChannelLocal, IoLatency1Ch[Index]);
  IoLatency1.Bits.io_lat_io_comp = IoLatIoComp;
  MemWritePciCfgEp (Socket, ChannelLocal, IoLatency1Ch[Index], IoLatency1.Data);

#ifndef DDR5_SUPPORT
  (*channelNvList)[ChannelLocal].ioLatency1 = IoLatency1.Data;
#endif // DDR5_SUPPORT
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
  UINT32 feedback;
  // SKX change
  feedback =  ReadDdrioDataRegCh (Host, socket, ch, strobe, DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG) & 0x1FF;
  return feedback;

} // ReadTrainFeedback

/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
VOID
SetChipGetCtlClkResLowerNibbleStrobeFeedback(
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
  *csrReg = ReadDdrioDataReg (Host, socket, ch, 0, strobe, DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG);
  //
  // Lower nibble
  //
  if ((*csrReg & 0x1FF) > 0) {
    _strobeFeedback |= 1 << strobe;
    *strobeFeedback = _strobeFeedback;
  }
}

/**


  Read the training feedback

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
UINT32
LrDimmReadTrainFeedbackBrs(
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    strobe,
  UINT8    bit
)
{
  UINT32 csrReg;
  UINT32 feedback;

  csrReg = ReadDdrioDataRegCh (Host, socket, ch, strobe, DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG);

  feedback = csrReg & 0x1FF;
  return feedback;
} // LrDimmReadTrainFeedbackBrs

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
  UINT32 csrReg;
  UINT32 feedback;


  if (IsBrsPresent(Host, socket)) {
    return LrDimmReadTrainFeedbackBrs (Host, socket, ch, strobe, bit);
  }

  // SKX HSD 4928153 DQ swizzle, normally this would be a straight up read but on SKX the bits are swizzled
  // nibble0 bits 0,1 are located in nibble1 bits 0,1 and vise-versa. Bits 2,3 for nibble0 and nibble1 are where they are supposed to be.
  //csrReg   = ((ReadDdrioDataRegCh (Host, socket, ch, strobe, DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG) & 0xc) |
  //        (ReadDdrioDataRegCh (Host, socket, ch, (strobe+9)%18, DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG) & 0x3));
  csrReg = ReadDdrioDataRegCh (Host, socket, ch, strobe, DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG);

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
  UINT8           ch,dimm,rank,strobe;
  UINT8           LongPreamble;
  INT16           delay;
  INT16           Senseampdelay;
  UINT32          SenseAmpDuration;
  INT16           MaxRcven;
  INT16           MinRcven;
  struct channelNvram                   (*channelNvList)[MAX_CH];
  struct dimmNvram                      (*dimmNvList)[MAX_DIMM];
  DATACONTROL1N0_0_MCIO_DDRIO_STRUCT    dataControl1;
  DATACONTROL4N0_0_MCIO_DDRIO_STRUCT    dataControl4;
  UINT8 MSVx4 = GetMaxStrobeValid(Host);
  UINT8 MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, socket);
  RcDebugPrint (SDBG_MAX, "\n Starting Senseamp and ODT delay and duration calculations \n");
  RcDebugPrint (SDBG_MAX, "------------------------------------------------------------ \n");
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    LongPreamble = (GetPreambleState (socket, READ_PREAMBLE) == PREAMBLE_2TCLK ? 1 : 0);
    dimmNvList = GetDimmNvList(Host, socket, ch);

    for (strobe = 0; strobe < MSVx4; strobe++) {
      //
      // Skip if this is an ECC strobe when ECC is disabled
      //
      if (IsStrobeNotValid(Host, strobe)) {
        continue;
      }

      // clear out min and max values
      MaxRcven=0;
      MinRcven=0xfff;
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
          if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, 0, DdrLevel, RecEnDelay, GSM_READ_ONLY, &delay);
          //RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT, "##Debug %d \n",delay);
          if (delay < MinRcven) {
            MinRcven = delay;
          }
          if (delay > MaxRcven) {
            MaxRcven = delay;
          }
        } //rank

      } //dimm

      // calculate delays
      // Receive Enable -long preamble - 1 (extra hold time req)
      Senseampdelay = MinRcven/64 - (2*LongPreamble) -1;
      // Receive Enable +1(Extend pulse 1 early) +2(for pre/postamble) +long preamble +1(roundup)
      SenseAmpDuration = MaxRcven/64 - MinRcven/64 + 1 + 2 + (2*LongPreamble) + 1;
      if (Senseampdelay < 0) {
        //
        // Convert negative number to 10nm senseampdelay bit field encoding of settings, this code assumes
        // the largest negative number that can result from the initial Senseampdelay formula is -3.
        //
        Senseampdelay += SENSEAMP_DELAY_CONVERT_NEGATIVE_SETTING;
      } else if (Senseampdelay > SENSEAMP_DELAY_MAX) {
        SenseAmpDuration += ((UINT32)Senseampdelay - SENSEAMP_DELAY_MAX);
        Senseampdelay = SENSEAMP_DELAY_MAX;
      }
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, strobe, NO_BIT,
      "LongPreamble = %d, MinRcven=%3d, MaxRcven=%3d: Senseampdelay=%d SenseAmpDuration=%d\n",LongPreamble,MinRcven,MaxRcven,Senseampdelay,SenseAmpDuration);

      //
      //program new values
      //

      //read cached values
      dataControl1.Data = (*channelNvList)[ch].dataControl1[strobe];
      dataControl4.Data = (*channelNvList)[ch].dataControl4[strobe];

      //modify with new values
      dataControl1.Bits.odtdelay          = (UINT32)Senseampdelay;
      dataControl1.Bits.senseampdelay     = (UINT32)Senseampdelay;
      dataControl1.Bits.odtduration         = (SenseAmpDuration & 7);
      dataControl1.Bits.senseampduration    = (SenseAmpDuration & 7);
      dataControl4.Bits.odtdurationmsb      = (SenseAmpDuration & 0x8) >> 0x3;
      dataControl4.Bits.senseampdurationmsb = (SenseAmpDuration & 0x8) >> 0x3;
      //write new values
      (*channelNvList)[ch].dataControl1[strobe] =  dataControl1.Data;
      (*channelNvList)[ch].dataControl4[strobe] =  dataControl4.Data;
      WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL1N0_0_MCIO_DDRIO_REG, dataControl1.Data);
      WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL4N0_0_MCIO_DDRIO_REG, dataControl4.Data);

    } //strobe
  } //ch
  RcDebugPrint (SDBG_MAX, "\n\n");
}  // SetSenseAmpODTDelays

/**

  Set Sense Amp ODT Delays for DDR5

  @param[in]  Socket      Socket number

**/
VOID
SetSenseAmpODTDelaysDdr5 (
  IN      UINT8       Socket
  )
{
#ifdef DDR5_SUPPORT
  PSYSHOST        Host;
  UINT8           Ch;
  UINT8           Dimm;
  UINT8           Rank;
  UINT8           Strobe;
  UINT8           LongPreamble;
  INT16           Delay;
  INT16           SenseAmpDelay;
  UINT32          SenseAmpDuration;
  INT16           MaxRcven;
  INT16           MinRcven;
  struct channelNvram                   (*ChannelNvList)[MAX_CH];
  struct dimmNvram                      (*DimmNvList)[MAX_DIMM];
  DATACONTROL1N0_0_MCIO_DDRIO_STRUCT    DataControl1;
  DATACONTROL4N0_0_MCIO_DDRIO_STRUCT    DataControl4;
  UINT8           SubCh = 0;
  UINT8           ChannelLocal = 0;
  UINT8           SubChMSVx4;

  Host = GetSysHostPointer ();
  SubChMSVx4 = GetSubChMaxStrobeValid(Host);
  ChannelNvList = GetChannelNvList(Host, Socket);

  for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    LongPreamble = (GetPreambleState (Socket, READ_PREAMBLE) == PREAMBLE_2TCLK ? 1 : 0);
    DimmNvList = GetDimmNvList(Host, Socket, Ch);

    for(SubCh = 0; SubCh < SUB_CH; SubCh++) {
      ChannelLocal = Ch;

      if (IsBrsPresent (Host, Socket)) {
        GetChannelMapping (Ch, SubCh, &ChannelLocal);
      }

      for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
        //
        // Skip if this is an ECC strobe when ECC is disabled
        //
        if (IsStrobeNotValidDdr5 (Host, Strobe)) {
          continue;
        }
        //
        // Clear out min and max values
        //
        MaxRcven=0;
        MinRcven = 0xfff;
        for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

          for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
            if (CheckRank(Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
              continue;
            }

            GetSetDataGroup (Host, Socket, ChannelLocal, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, RecEnDelay, GSM_READ_ONLY, &Delay);

            if (Delay < MinRcven) {
              MinRcven = Delay;
            }
            if (Delay > MaxRcven) {
              MaxRcven = Delay;
            }
          } // Rank
        } // Dimm

        //
        // calculate delays
        // Receive Enable -long preamble - 1 (extra hold time req)
        //
        SenseAmpDelay = MinRcven/64 - (2*LongPreamble) -1;
        //
        // Receive Enable +1(Extend pulse 1 early) +2(for pre/postamble) +long preamble +1(roundup)
        //
        SenseAmpDuration = MaxRcven/64 - MinRcven/64 + 1 + 2 + (2*LongPreamble) + 1;
        if (SenseAmpDelay < 0) {
          //
          // Convert negative number to 10nm senseampdelay bit field encoding of settings, this code assumes
          // the largest negative number that can result from the initial Senseampdelay formula is -3.
          //
          SenseAmpDelay += SENSEAMP_DELAY_CONVERT_NEGATIVE_SETTING;
        } else if (SenseAmpDelay > SENSEAMP_DELAY_MAX) {
          SenseAmpDuration += ((UINT32)SenseAmpDelay - SENSEAMP_DELAY_MAX);
          SenseAmpDelay = SENSEAMP_DELAY_MAX;
        }
        //
        // Program new values
        //
        (*ChannelNvList)[Ch].dataControl1[Strobe] = ReadDdrioDataRegCh (Host, Socket, ChannelLocal, Strobe, DATACONTROL1N0_0_MCIO_DDRIO_REG);
        (*ChannelNvList)[Ch].dataControl4[Strobe] = ReadDdrioDataRegCh (Host, Socket, ChannelLocal, Strobe, DATACONTROL4N0_0_MCIO_DDRIO_REG);
        //
        // read cached values
        //
        DataControl1.Data = (*ChannelNvList)[Ch].dataControl1[Strobe];
        DataControl4.Data = (*ChannelNvList)[Ch].dataControl4[Strobe];

        //modify with new values
        DataControl1.Bits.odtdelay          = (UINT32)SenseAmpDelay;
        DataControl1.Bits.senseampdelay     = (UINT32)SenseAmpDelay;
        DataControl1.Bits.odtduration         = (SenseAmpDuration & 7);
        DataControl1.Bits.senseampduration    = (SenseAmpDuration & 7);
        DataControl4.Bits.odtdurationmsb      = (SenseAmpDuration & 0x8) >> 0x3;
        DataControl4.Bits.senseampdurationmsb = (SenseAmpDuration & 0x8) >> 0x3;
        //write new values
        (*ChannelNvList)[Ch].dataControl1[Strobe] =  DataControl1.Data;
        (*ChannelNvList)[Ch].dataControl4[Strobe] =  DataControl4.Data;
        WriteDdrioDataRegCh (Host, Socket, ChannelLocal, Strobe, DATACONTROL1N0_0_MCIO_DDRIO_REG, DataControl1.Data);
        WriteDdrioDataRegCh (Host, Socket, ChannelLocal, Strobe, DATACONTROL4N0_0_MCIO_DDRIO_REG, DataControl4.Data);

      } // Strobe loop
    } // Subch loop
  } // Ch loop
#endif // #ifdef DDR5_SUPPORT
}  // SetSenseAmpODTDelaysDdr5

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
}

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
  return (UINT16)MemReadPciCfgEp (socket, ch, DDRCRDDRTTRAINRESULT_MCIO_DDRIOEXT_REG);
} // GetDdrCrDdrtResult

/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
UINT8
GetDdrtTrainResult(
                    PSYSHOST Host,
                    UINT8    socket,
                    UINT8    ch,
                    UINT8    dimm
)
{
  DDRCRDDRTTRAINRESULT_MCIO_DDRIOEXT_STRUCT ddrtTrainResult;

  ddrtTrainResult.Data = GetDdrCrDdrt ( Host, socket, ch);

  return (UINT8)(ddrtTrainResult.Bits.evenoddsamples >> (dimm * 4));
} // GetDdrCrDdrtResult

/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
VOID
TxPiSampleChipInit(
  PCHIP_DDRIO_STRUCT dataControl
  )
{
  DATACONTROL0N0_0_MCIO_DDRIO_STRUCT dataControl0;
  DATACONTROL0N0_0_MCIO_DDRIO_STRUCT dataControl0_1;
  DATACONTROL2N0_0_MCIO_DDRIO_STRUCT dataControl2;

  dataControl0.Data = 0;
  dataControl2.Data = 0;
  dataControl0.Bits.wltrainingmode  = 1;
  // ICX_TODO
  dataControl0.Bits.readrfwr        = 1;
  dataControl0.Bits.txpion          = 1;
  dataControl2.Bits.forcebiason     = 1;
  dataControl0_1.Data = 0; //KV3
  dataControl0_1.Bits.wltrainingmode  = 1;

  dataControl->dataControl0.Data   = dataControl0.Data;
  dataControl->dataControl2.Data   = dataControl2.Data;
  dataControl->dataControl0_1.Data = dataControl0_1.Data;
}

/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
VOID
GetChipLogicalRank(
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 rank,
  PCHIP_DDRIO_STRUCT dataControl
  )
{
  DATACONTROL0N0_0_MCIO_DDRIO_STRUCT   dataControl0;

  dataControl0.Data = dataControl->dataControl0.Data;
  dataControl0.Bits.readrfrank = GetLogicalRank (Host, socket, ch, dimm, rank);
  dataControl->dataControl0.Data = dataControl0.Data;
}

/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
VOID
SetChipTxPiSampleTrainingMode(
  PSYSHOST            Host,
  UINT8               socket,
  UINT8               ch,
  PCHIP_DDRIO_STRUCT  dataControl
  )
{
  UINT8                               strobe;
  DATACONTROL0N0_0_MCIO_DDRIO_STRUCT  dataControl0;
  DATACONTROL2N0_0_MCIO_DDRIO_STRUCT  dataControl2;
  struct channelNvram                 (*channelNvList)[MAX_CH];
  UINT8 MSVx4 = GetMaxStrobeValid(Host);

  channelNvList = GetChannelNvList(Host, socket);

  dataControl0.Data = dataControl->dataControl0.Data;
  dataControl2.Data = dataControl->dataControl2.Data;

  // SKX change for upper nibble
  for (strobe = 0; strobe < MSVx4; strobe++) {
    if (IsStrobeNotValid(Host, strobe)) {
      continue;
    }
    //
    // Set training mode and rank to all channels
    //
    WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL2N0_0_MCIO_DDRIO_REG, dataControl2.Data | (*channelNvList)[ch].dataControl2[strobe]);

    //
    // Delay only if this is a CTE build, otherwise do nothing.
    //

    CteDelayQclk (1); // needed for delay before setting readrf* =1

    WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, dataControl0.Data | (*channelNvList)[ch].dataControl0);
  } // strobe loop
}

/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
VOID
SetChipTxPiSampleTrainingTXDQSStrobesI(
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 rank,
  INT16 piDelay,
  PCHIP_DDRIO_STRUCT dataControl
  )
{
  UINT8                               strobe;
  DATACONTROL0N0_0_MCIO_DDRIO_STRUCT  dataControl0;
  struct channelNvram                 (*channelNvList)[MAX_CH];
  UINT8 MSVx4 = GetMaxStrobeValid(Host);

  channelNvList = GetChannelNvList(Host, socket);

  dataControl0.Data = dataControl->dataControl0_1.Data;

  // SKX change for upper nibble
  for (strobe = 0; strobe < MSVx4; strobe++) {
    if (IsStrobeNotValid(Host, strobe)) {
      continue;
    }
    //
    // Set training mode and rank to all channels
    //
    WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, dataControl0.Data | (*channelNvList)[ch].dataControl0); //KV3
  } // strobe loop

  GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, 0, DdrLevel, TxDqsDelay, GSM_UPDATE_CACHE, (INT16 *)&piDelay);
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
  UINT8                               strobe;
  DATACONTROL0N0_0_MCIO_DDRIO_STRUCT  dataControl0;
  struct channelNvram                 (*channelNvList)[MAX_CH];
  UINT8 MSVx4 = GetMaxStrobeValid(Host);

  channelNvList = GetChannelNvList(Host, socket);
  dataControl0.Data = dataControl->dataControl0.Data;

  for (strobe = 0; strobe < MSVx4; strobe++) {
    if (IsStrobeNotValid(Host, strobe)) {
      continue;
    }
    //
    // Set training mode and rank to all channels
    //
    WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, dataControl0.Data | (*channelNvList)[ch].dataControl0);
  } // strobe loop


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
  UINT8   DqDrvUpCompOffset;
  UINT8   Strobe;
  UINT8   Ch;
  struct channelNvram                           (*ChannelNvList)[MAX_CH];
  DATACONTROL3N0_0_MCIO_DDRIO_STRUCT            DataControl3;
  DATAOFFSETCOMP_MCIO_DDRIO_STRUCT              DataOffsetComp;
  COMPDATA0N0_0_MCIO_DDRIO_STRUCT               CompData0;
  UINT8                                         MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList(Host, Socket);
  //
  // Cache DrvStaticLegCfg, RcompCode and DataSegmentEnable values for this socket
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    for (Strobe = 0; Strobe < MAX_STROBE; Strobe++) {
      if (IsStrobeNotValid (Host, Strobe)) {
        continue;
      }
      DataOffsetComp.Data = ReadDdrioDataRegCh (Host, Socket, Ch, Strobe, DATAOFFSETCOMP_MCIO_DDRIO_REG);
      DqDrvUpCompOffset   = (UINT8) DataOffsetComp.Bits.dqdrvupcompoffset;
      CompData0.Data      = ReadDdrioDataRegCh (Host, Socket, Ch, Strobe, COMPDATA0N0_0_MCIO_DDRIO_REG);
      RcompDrvUp          = (UINT8) CompData0.Bits.rcompdrvup;
      DataControl3.Data   = ReadDdrioDataRegCh (Host, Socket, Ch, Strobe, DATACONTROL3N0_0_MCIO_DDRIO_REG);
      (*ChannelNvList)[Ch].DataSegmentEnable[Strobe] = (UINT8) DataControl3.Bits.datasegmentenable;
      (*ChannelNvList)[Ch].DrvStaticLegCfg[Strobe]   = (UINT8) DataControl3.Bits.drvstaticlegcfg;

      (*ChannelNvList)[Ch].RcompCode[Strobe] = RcompDrvUp + DqDrvUpCompOffset;
      if ((*ChannelNvList)[Ch].RcompCode[Strobe] > MAX_RCOMP_CODE) {
        (*ChannelNvList)[Ch].RcompCode[Strobe] = MAX_RCOMP_CODE;
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
    CopyMem ((UINT8 *)Param->SettingList, (UINT8 *)Settings, SizeSettings);
      }
  Param->NumSettings = NumSettings;

  //
  // TxEq and TxEqTap2 are per nibble register, but it has DQ swizzling in strobe pair,
  // need to set same value for strobe pair, so ParamType is PerBytePerRankPerCh
  //
  Param->ParamType = PerBytePerRankPerCh;

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
  struct dimmNvram                      (*DimmNvList)[MAX_DIMM];
  struct channelNvram                   (*ChannelNvList)[MAX_CH];
  XTALKDELTARANK0_MCIO_DDRIO_STRUCT     XtalkDeltaN0;
  XTALKDELTARANK0_MCIO_DDRIO_STRUCT     XtalkDeltaN1;
  UINT8                                 MaxChDdr;

  MaxChDdr   = GetMaxChDdr ();
  MaxValDdjc = Param->SettingList[Param->NumSettings - 1];
  MinValPxc = 0xFFFF;
  MSVx8 = GetMaxStrobeValid (Host) / 2;

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList(Host, Socket, Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {

        if (CheckRank(Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList)[Dimm].numRanks)) {
          continue;
        }
        LogRank = GetLogicalRank(Host, Socket, Ch, Dimm, Rank);

        for (Strobe = 0; Strobe < MSVx8; Strobe++) {
          if (IsStrobeNotValid(Host, Strobe)) {
            continue;
          }

          //
          // Get current PXC Delta programming
          //
          XtalkDeltaN0.Data = (UINT32)(*ChannelNvList)[Ch].XtalkDeltaN0[LogRank][Strobe % MSVx8];
          XtalkDeltaN1.Data = (UINT32)(*ChannelNvList)[Ch].XtalkDeltaN1[LogRank][(Strobe % MSVx8) + MSVx8];

          CurValPxc[0] = (UINT16)XtalkDeltaN0.Bits.xtalkdeltabit0; // Bit 0
          CurValPxc[1] = (UINT16)XtalkDeltaN0.Bits.xtalkdeltabit1; // Bit 1
          CurValPxc[2] = (UINT16)XtalkDeltaN0.Bits.xtalkdeltabit2; // Bit 2
          CurValPxc[3] = (UINT16)XtalkDeltaN0.Bits.xtalkdeltabit3; // Bit 3
          CurValPxc[4] = (UINT16)XtalkDeltaN1.Bits.xtalkdeltabit2; // Bit 6
          CurValPxc[5] = (UINT16)XtalkDeltaN1.Bits.xtalkdeltabit3; // Bit 7
          CurValPxc[6] = (UINT16)XtalkDeltaN1.Bits.xtalkdeltabit0; // Bit 4
          CurValPxc[7] = (UINT16)XtalkDeltaN1.Bits.xtalkdeltabit1; // Bit 5

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
  CopyMem ((UINT8 *)Param->SettingList, (UINT8 *)Settings, SizeSettings);

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
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Ch
  )
{
  UINT8                                  Strobe;
  DATACONTROL4N0_0_MCIO_DDRIO_STRUCT     dataControl4;
  struct channelNvram                    (*channelNvList)[MAX_CH];
  UINT8 MSVx4 = GetMaxStrobeValid(Host);

  channelNvList = GetChannelNvList(Host, Socket);

  if ((*channelNvList)[Ch].enabled == 0) {
    return;
  }

  //per strobe
  for (Strobe = 0; Strobe < MSVx4; Strobe++) {
    dataControl4.Data = ReadDdrioDataRegCh (Host, Socket, Ch, Strobe, DATACONTROL4N0_0_MCIO_DDRIO_REG);
    dataControl4.Bits.writecrcenable = 1;
    WriteDdrioDataRegCh (Host, Socket, Ch, Strobe, DATACONTROL4N0_0_MCIO_DDRIO_REG, dataControl4.Data);
    (*channelNvList)[Ch].dataControl4[Strobe] = dataControl4.Data;
  } // strobe loop
}

/**

  Returns the DDRIO Long Preamble Enable setting

  @param[in] Socket      - Socket
  @param[in] Ch          - Ch

  @retval 1 if long preamble is enabled in the DDRIO
          0 otherwise

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
  PSYSHOST                            Host;
  UINT8                               Strobe;
  struct channelNvram                 (*ChannelNvList) [MAX_CH];
  DATACONTROL0N0_0_MCIO_DDRIO_STRUCT  DataControl0;
  UINT8                               MSVx4;

  Host = GetSysHostPointer ();
  MSVx4 = GetMaxStrobeValid (Host);
  ChannelNvList = GetChannelNvList (Host, Socket);

  DataControl0.Data = (*ChannelNvList)[Ch].dataControl0;
  DataControl0.Bits.longpreambleenable = Preamble;

  for (Strobe = 0; Strobe < MSVx4; Strobe++) {
    //
    // Skip if this is an ECC strobe when ECC is disabled
    //
    if (IsStrobeNotValid (Host, Strobe)) {
      continue;
    }

    WriteDdrioDataRegCh (Host, Socket, Ch, Strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, DataControl0.Data);
  } // strobe loop

}

#ifdef DDR5_SUPPORT
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
  UINT8  ChannelLocal = Channel;
  GetChannelMapping (Channel, SubChannel, &ChannelLocal);
  Feedback = ReadDdrioDataRegCh (Host, Socket, ChannelLocal, InputStrobe, DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG) & 0x1FF;
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
  UINT32  Feedback;
  // For BRS, the feedback is returned in the same register/field as the counter feedback.
  Feedback = ReadDataTrainCounterFeedbackDdr5 (Host, Socket, Channel, SubChannel, InputStrobe);
  return Feedback;
}

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
  UINT8 Ch;
  UINT8 Strobe;
  UINT8 MSVx4;
  UINT8 SubChannel = 0;
  UINT8 ChannelLocal = 0;
  struct channelNvram (*ChannelNvList) [MAX_CH];
  DDR5DATATRAINCTRL_MCIO_DDRIO_STRUCT Ddr5DataTrainCtrl;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  MSVx4 = 18;
  ChannelNvList = GetChannelNvList (Host, Socket);

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "BcomDdrioInit starts \n");

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList) [Ch].enabled == 0) {
      continue;
    }
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {

      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        ChannelLocal = Ch;
        GetChannelMapping (Ch, SubChannel, &ChannelLocal);
        Ddr5DataTrainCtrl.Data = ReadDdrioDataRegCh (Host, Socket, ChannelLocal, Strobe, DDR5DATATRAINCTRL_MCIO_DDRIO_REG);
        Ddr5DataTrainCtrl.Bits.sample0 = 1;
        WriteDdrioDataRegCh (Host, Socket, ChannelLocal, Strobe, DDR5DATATRAINCTRL_MCIO_DDRIO_REG, Ddr5DataTrainCtrl.Data);
      } //SubChannel

    } // strobe loop
  } //Ch loop
  //
  // Put a 2us delay here to allow the DDRIO HW to do synchronization after enabling sampler_feedback_en.
  //
  FixedDelayMicroSecond (2);
  //
  // Delay only if this is a CTE build, otherwise do nothing.
  //
  CteDelayQclk(2); // needed for delay
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
  UINT8 Channel;
  UINT8 Strobe;
  UINT8 MSVx4;
  UINT8 SubChannel = 0;
  UINT8 ChannelLocal = 0;
  struct channelNvram (*ChannelNvList) [MAX_CH];
  DDR5DATATRAINCTRL_MCIO_DDRIO_STRUCT Ddr5DataTrainCtrl;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  MSVx4 = GetMaxStrobeValid (Host);
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if ((*ChannelNvList) [Channel].enabled == 0) {
      continue;
    }
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {

      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        ChannelLocal = Channel;
        GetChannelMapping (Channel, SubChannel, &ChannelLocal);
        Ddr5DataTrainCtrl.Data = ReadDdrioDataRegCh (Host, Socket, ChannelLocal, Strobe, DDR5DATATRAINCTRL_MCIO_DDRIO_REG);
        Ddr5DataTrainCtrl.Bits.sample0 = 0;
        WriteDdrioDataRegCh (Host, Socket, ChannelLocal, Strobe, DDR5DATATRAINCTRL_MCIO_DDRIO_REG, Ddr5DataTrainCtrl.Data);
      } // SubChannel loop
    } // Strobe loop
  } // Channel loop
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
  SetTrainingModeLatchResult (Host, Socket, CHECKPOINT_MINOR_SENSE_AMP);
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
  SetTrainingModeLatchResult (Host, Socket, CHECKPOINT_MINOR_DISABLE_SENSE_AMP);
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
  UINT8     Dimm;
  UINT8     Rank;

  Host = GetSysHostPointer ();

  // Enable Senseamp training mode
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      SetTrainingMode (Host, Socket, Dimm, Rank, CHECKPOINT_MINOR_SENSE_AMP);
    }
  }
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
  UINT8     Dimm;
  UINT8     Rank;

  Host = GetSysHostPointer ();

  // Disable Senseamp training mode
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      SetTrainingMode (Host, Socket, Dimm, Rank, CHECKPOINT_MINOR_DISABLE_SENSE_AMP);
    }
  }
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
  PSYSHOST  Host;

  Host = GetSysHostPointer ();
  SetTrainingModeLatchResult (Host, Socket, CHECKPOINT_MINOR_SENSE_AMP);
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
  PSYSHOST  Host;

  Host = GetSysHostPointer ();
  SetTrainingModeLatchResult (Host, Socket, CHECKPOINT_MINOR_DISABLE_SENSE_AMP);
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
  UINT8     Dimm;
  UINT8     Rank;

  Host = GetSysHostPointer ();

  // Enable Senseamp training mode
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {  // DIMM and rank present check done within SetTrainingMode
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      SetTrainingMode (Host, Socket, Dimm, Rank, CHECKPOINT_MINOR_SENSE_AMP);
    }
  }
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
  UINT8     Dimm;
  UINT8     Rank;

  Host = GetSysHostPointer ();

  // Disable Senseamp training mode
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {  // DIMM and rank present check done within SetTrainingMode
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      SetTrainingMode (Host, Socket, Dimm, Rank, CHECKPOINT_MINOR_DISABLE_SENSE_AMP);
    }
  }
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
  UINT8                               strobe;
  DDR5DATATRAINCTRL_MC_DDRIOMC_STRUCT  Ddr5DataTrainCtrl;
  DATACONTROL0N0_0_MCIO_DDRIO_STRUCT       DataControl0;
  struct channelNvram (*channelNvList) [MAX_CH];
  UINT8                               SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8                               ChannelLocal = 0;

  ChannelLocal = ch;
  channelNvList = GetChannelNvList (Host, socket);

  //
  // For BRS, take ch mapping into account.
  //
  GetChannelMapping (ch, subch, &ChannelLocal);

  for (strobe = 0; strobe < SubChMSVx4; strobe++) {
    if (IsStrobeNotValid (Host, strobe)) {
      continue;
    }
    Ddr5DataTrainCtrl.Data = ReadDdrioDataRegCh (Host, socket, ChannelLocal, strobe, DDR5DATATRAINCTRL_MCIO_DDRIO_REG);
    DataControl0.Data = ReadDdrioDataRegCh (Host, socket, ChannelLocal, strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG);
    //
    // Set training mode and rank to all channels
    //
    //
    //Enable DDR5 WL mode.
    //
    Ddr5DataTrainCtrl.Bits.wltrainingmode = 1;
    //
    // To drives DQS low, when there is no WRITE command sent on the CA bus
    //
    Ddr5DataTrainCtrl.Bits.drivedqslow = 1;
    //
    // Only enable the first strobe.
    //
    Ddr5DataTrainCtrl.Bits.strobemask = 0xFE;
    //
    // Don't disable the preamble
    //
    Ddr5DataTrainCtrl.Bits.disablepreamble = 0;
    //
    // Set it as 1, but actually, the bit doesn't function in DDRIO HW. By default, the DQS postamble is disabled in DDRIO WL mode.
    //
    Ddr5DataTrainCtrl.Bits.disablepostamble = 1;
    WriteDdrioDataRegCh (Host, socket, ChannelLocal, strobe, DDR5DATATRAINCTRL_MCIO_DDRIO_REG, Ddr5DataTrainCtrl.Data);
    //
    // Program DDR4 wltrainingmode bit.
    //
    DataControl0.Bits.wltrainingmode = 1;
    WriteDdrioDataRegCh (Host, socket, ChannelLocal, strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, DataControl0.Data);
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
  IN UINT8 socket,
  IN UINT8 ch,
  IN UINT8 subch,
  IN UINT8 value,
  IN BOOLEAN Drive
  )
{
  UINT8                               strobe;
  DDR5DATATRAINCTRL_MC_DDRIOMC_STRUCT  dataControl0;
  struct channelNvram (*channelNvList) [MAX_CH];
  UINT8                               SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8                               ChannelLocal = 0;

  ChannelLocal = ch;
  channelNvList = GetChannelNvList (Host, socket);

  GetChannelMapping (ch, subch, &ChannelLocal);

  for (strobe = 0; strobe < SubChMSVx4; strobe++) {
    //
    // Skip if this is an ECC strobe when ECC is disabled
    //
    if (IsStrobeNotValidDdr5 (Host, strobe)) {
      continue;
    }

    dataControl0.Data = ReadDdrioDataRegCh (Host, socket, ChannelLocal, strobe, DDR5DATATRAINCTRL_MCIO_DDRIO_REG);
    //
    // To drives DQS low, when there is no WRITE command sent on the CA bus
    //
    dataControl0.Bits.drivedqslow = value;
    WriteDdrioDataRegCh (Host, socket, ChannelLocal, strobe, DDR5DATATRAINCTRL_MCIO_DDRIO_REG, dataControl0.Data);
  } // strobe loop
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
    return;
}

/**


Set Drive Dqs Low for all strobes in all ranks in the channel.

@param Host[in]         - Pointer to sysHost
@param socket[in]       - Processor to initialize
@param ch[in]           - channel number
@param subch[in]        - sub channel number
@param value[in]        - value to be programmed

@retval N/A

**/
VOID
ProgramDriveDqsLowDdr5 (
  IN PSYSHOST Host,
  IN UINT8 socket,
  IN UINT8 ch,
  IN UINT8 subch,
  IN UINT8 value
  )
{
  return;
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

  DDR5DATATRAINCTRL_MC_DDRIOMC_STRUCT  DataControl0Low;
  DDR5DATATRAINCTRL_MC_DDRIOMC_STRUCT  DataControl0High;
  UINT8                                ChannelLocal = 0;
  UINT8                                SubCh = 0;
  UINT8                                MaxStrobePairs;
  UINT8                                LowStrobe;
  UINT8                                HighStrobe;

  MaxStrobePairs = GetSubChMaxStrobeValid (Host) / 2;
  if (Strobe < MaxStrobePairs) {
    LowStrobe = Strobe;
    HighStrobe = Strobe + MaxStrobePairs;
  } else {
    LowStrobe = Strobe - MaxStrobePairs;
    HighStrobe = Strobe;
  }
  ChannelLocal = Ch;
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if ((SubChMask & (1 << SubCh)) == 0) {
      continue;
    }
    GetChannelMapping (Ch, SubCh, &ChannelLocal);


    DataControl0Low.Data = ReadDdrioDataRegCh (Host, Socket, ChannelLocal, LowStrobe, DDR5DATATRAINCTRL_MCIO_DDRIO_REG);
    DataControl0High.Data = ReadDdrioDataRegCh (Host, Socket, ChannelLocal, HighStrobe, DDR5DATATRAINCTRL_MCIO_DDRIO_REG);
    //
    // To drives DQS low, when there is no WRITE command sent on the CA bus
    //
    if (Override) {
      // Take into account BRS DDRIO nibble swizzling:
      // To target DQ override on low nibbles (DQS 0-4): Override DQ on DDRIO DATA0 hi nibble bits [0,1] and lo nibble bits [2,3]
      if (Strobe < MaxStrobePairs) {
        DataControl0Low.Bits.dqvalueovr &= DQ_LOW_MASK; // Preserve lower bits
        DataControl0Low.Bits.dqvalueovr |= (Value & DQ_HIGH_MASK);  // set upper bits to value
        DataControl0High.Bits.dqvalueovr &= DQ_HIGH_MASK; // Preserve upper bits
        DataControl0High.Bits.dqvalueovr |= (Value & DQ_LOW_MASK); // set lower bits to value
      } else {  // To target DQ override on high nibbles (DQS 5-9): Override DQ on DDRIO DATA0 lo nibble bits [0,1] and hi nibble bits [2,3]
        DataControl0Low.Bits.dqvalueovr &= DQ_HIGH_MASK; // Preserve upper bits
        DataControl0Low.Bits.dqvalueovr |= (Value & DQ_LOW_MASK); // set lower bits to value
        DataControl0High.Bits.dqvalueovr &= DQ_LOW_MASK; // Preserver lower bits
        DataControl0High.Bits.dqvalueovr |= (Value & DQ_HIGH_MASK); // set upper bits to value
      }
      DataControl0Low.Bits.dqovren = 1;
      DataControl0High.Bits.dqovren = 1;
    } else {
      if (Strobe < MaxStrobePairs) {
        DataControl0Low.Bits.dqvalueovr &= DQ_LOW_MASK;   // Clear upper bits to default
        DataControl0High.Bits.dqvalueovr &= DQ_HIGH_MASK; // Clear lower bits to default
      } else {
        DataControl0Low.Bits.dqvalueovr &= DQ_HIGH_MASK;  // Clear lower bits to default
        DataControl0High.Bits.dqvalueovr &= DQ_LOW_MASK;  // Clear upper bits to default
      }
      DataControl0Low.Bits.dqovren = 0;
      DataControl0High.Bits.dqovren = 0;
    }
    WriteDdrioDataRegCh (Host, Socket, ChannelLocal, LowStrobe, DDR5DATATRAINCTRL_MCIO_DDRIO_REG, DataControl0Low.Data);
    WriteDdrioDataRegCh (Host, Socket, ChannelLocal, HighStrobe, DDR5DATATRAINCTRL_MCIO_DDRIO_REG, DataControl0High.Data);
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
  UINT8     MSVx4;
  PSYSHOST  Host = GetSysHostPointer ();

  MSVx4 = GetMaxStrobeValid (Host);

  for (Strobe = 0; Strobe < MSVx4; Strobe++) { //BRS_TODO. To update the MSVx4 for BRS.

    WriteDdr5DqCountingWindowValueTarget (Host, Socket, Channel, Strobe);

  } // Strobe loop
} // SetDqCountingWindowByChannel

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
  struct dimmNvram (*dimmNvList) [MAX_DIMM];
  UINT8               MaxChDdr;

  MaxChDdr    = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);

  for (ch = 0; ch < MaxChDdr; ch += GetChInc (Host)) {
    if ((*channelNvList) [ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, socket, ch);

    // Continue to the next rank if this one is disabled
    if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT) || (rank >= (*dimmNvList) [dimm].numRanks)) {
      continue;
    }

    SetDqCountingWindowByChannel (socket, ch);

    //
    //Set Invert Counting Sample as 1 //DDR5_TODO
    //
    //
    //Set the DQ MASK? //DDR5_TODO
    //
  } // ch loop
} // SetDqCountingWindow

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
  DDR5INTFCMDTRAINCNTR_CH0_MCIO_DDRIO_STRUCT Ddr5IntfCmdTrainCntr;

  if (IsBrsPresent (Host, Socket)) {
    Ddr5IntfCmdTrainCntr.Data = MemReadPciCfgEp (Socket, Channel, DDR5INTFCMDTRAINCNTR_CH1_MCIO_DDRIO_REG);
    Ddr5IntfCmdTrainCntr.Bits.trainingcounter = CSCA_TF_COUNTING_WINDOW_VALUE;
    MemWritePciCfgEp (Socket, Channel, DDR5INTFCMDTRAINCNTR_CH1_MCIO_DDRIO_REG, Ddr5IntfCmdTrainCntr.Data);
  }
} // SetCountingWindowByChannel

/**

  Set Counting Window of all channels with the given Dimm and Rank.

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Current Socket
  @param[in] Dimm         - DIMM number
  @param[in] Rank         - rank number of the DIMM
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
  UINT8                                      Ch;
  struct channelNvram                        (*channelNvList) [MAX_CH];
  struct dimmNvram                           (*dimmNvList) [MAX_DIMM];
  UINT8                                      MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
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
  } // ch loop
} // SetCountingWindow

/**

  Set Data Train Count Window

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
  return;
}

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
  return;
}

/**

  Sets the DQ Sample Mask field to mask off any DQ bits not considered
  in the OR computation for the command training counting.

  @param[in] Socket     - Socket number
  @param[in] ChBitmask  - Channel bit mask
  @param[in] MaskValue  - Mask value to be programmed

  @retval N/A

**/
VOID
EFIAPI
SetCounterDqSampleMask (
  IN UINT8   Socket,
  IN UINT32  ChBitmask,
  IN UINT8   MaskValue
  )
{
  return;
}

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
  MRC_TT              CurrentTestType;

  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  CurrentTestType = GetCurrentTestType (Socket);


  if ((CurrentTestType == EarlyCsLoopbackTest) ||
      (CurrentTestType == EarlyCaLoopbackTest) ||
      (CurrentTestType == BcomTrainingTest) ||
      (CurrentTestType == EarlyCsBacksideLoopbackTest)||
      (CurrentTestType == EarlyCaBacksideLoopbackTest)||
      (CurrentTestType == MdqsReceiveEnablePhaseTraining) ||
      (CurrentTestType == MdqsReceiveEnableCycleTraining) ||
      (CurrentTestType == MdqsCoarseReadDelayTraining) ||
      (CurrentTestType == BacksideDqSwizzleDiscovery) ||
      (CurrentTestType == MdqsExternalWriteLevelingCoarseTraining) ||
      (CurrentTestType == MdqsExternalWriteLevelingFineTraining) ||
      (CurrentTestType == MdqsInternalWriteLevelingCoarseTraining) ||
      (CurrentTestType == MdqsInternalWriteLevelingFineTraining) ||
      (CurrentTestType == WLDdr5)) {

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {

      if (IsNibbleNotValidDdr5 (Host, Strobe)) {
        continue; // DDR5_TODO
      }

      //DDR5_TODO. For BRS, take ch mapping into account.

      WriteDdr5CaTrainingModeTarget (Host, Socket, Channel, Strobe);

    } // strobe loop
  } else if ((CurrentTestType == EarlyRcdCsLoopbackTest) ||
              (CurrentTestType == EarlyRcdCaLoopbackTest)) {

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
  struct dimmNvram    (*dimmNvList) [MAX_DIMM];
  MRC_TT              CurrentTestType;
  UINT8               MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, Socket);
  CurrentTestType = GetCurrentTestType (Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
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

  @retval EFI_STATUS

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
    return;
}

/**

  Enable CS assertion trigger or not for all channels with the given Dimm and Rank.

  @param Host      - Pointer to sysHost
  @param Socket    - Current Socket
  @param Dimm      - Dimm number
  @param Rank      - Rank number
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
  return EFI_SUCCESS;
} // EnableCsaTrigger

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
  UINT32 feedback = 0;
  UINT8   StrobeIndex = strobe;
  UINT8   ChannelIndex = ch;
  MRC_TT  CurrentTestType;

  CurrentTestType = GetCurrentTestType (socket);
  GetChannelMapping (ch, subch, &ChannelIndex);

  if ((CurrentTestType == EarlyCsLoopbackTest) ||
      (CurrentTestType == EarlyCaLoopbackTest) ||
      (CurrentTestType == BcomTrainingTest) ||
      (CurrentTestType == EarlyCsBacksideLoopbackTest)||
      (CurrentTestType == WLDdr5)||
      (CurrentTestType == EarlyCaBacksideLoopbackTest)) {

    PollBacksideTrainDoneTarget (Host, socket, ch, subch, strobe);
    GetDdr5DataTrainFeedbackTarget (Host, socket, ch, subch, strobe, &feedback);

  } else if ((CurrentTestType == EarlyRcdCsLoopbackTest) || (CurrentTestType == EarlyRcdCaLoopbackTest)) {
    //
    // Polling the counting to be done. Use ch here as only routing channel 1 alert_n in BRS.
    //
    GetDdr5IntfCmdTrainCtrlFeedbackTarget (Host, socket, ch, subch, StrobeIndex, dimm, &feedback);
  }
  return feedback;

} // ReadTrainFeedbackDdr5

/**

  Gets the max DCA retrain loop count.

  @param VOID

  @retval MAX DCA Loop Count

**/
UINT8
EFIAPI
GetMaxDcaRetrainLoopCount (
  VOID
  )
{
  return 1;
}

/**

  Check if DDR5 sub-channel A and B require equal Round Trip Latency.

  @param  VOID

  @retval FALSE  Sub-channel A and B do not require equal Round Trip Latency.
**/
BOOLEAN
EFIAPI
IsEqualSubChRtlRequired (
  VOID
  )
{
  return FALSE;
}

#endif //DDR5_SUPPORT

/**

  Get the round trip step unit.

  10nm wave 1 rt_lat register is in QCLK, so RT latency step size is 2.

  @param VOID

  @retval round trip step unit

**/

UINT8
EFIAPI
GetRoundTripStepUnit (
  VOID
  )
{
  return 2;
}

/**

  Get the IO latency step unit.

  10nm wave 1 io_lat register is in QCLK, so IO latency step size is 2.

  @param VOID

  @retval round trip step unit

**/

UINT8
EFIAPI
GetIoLatStepUnit (
  VOID
  )
{
  return 2;
}

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
  DDRCRINTFDATATIMING0_CH0_MCIO_DDRIO_STRUCT            IntfDataTiming0;
  UINT32                                                IntfDataTiming0Ch[] = {DDRCRINTFDATATIMING0_CH0_MCIO_DDRIO_REG,
                                                                               DDRCRINTFDATATIMING0_CH1_MCIO_DDRIO_REG,
                                                                               DDRCRINTFDATATIMING0_CH2_MCIO_DDRIO_REG
                                                                              };
  UINT8                                                 Index;
  UINT8                                                 NumChPerMc;

  NumChPerMc = GetNumChannelPerMc ();
  if (IsBrsPresent (Host, Socket)) {
    ProgramJedecTimingsDdrioBrs (Host, Socket, Ch, OriginalWriteTimings);
    return;
  }
  Index = Ch % NumChPerMc;
  IntfDataTiming0.Data = MemReadPciCfgEp (Socket, Ch, IntfDataTiming0Ch[Index]);
  IntfDataTiming0.Bits.t_cwl_ddr4 = GettCWL (Host, Socket, Ch);
  MemWritePciCfgEp (Socket, Ch, IntfDataTiming0Ch[Index], IntfDataTiming0.Data);
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
  DDRCRINTFDATATIMING0_CH0_MCIO_DDRIO_STRUCT  IntfDataTiming0;
  UINT32                                      IntfDataTiming0Ch[] = {DDRCRINTFDATATIMING0_CH0_MCIO_DDRIO_REG,
                                                                     DDRCRINTFDATATIMING0_CH1_MCIO_DDRIO_REG,
                                                                     DDRCRINTFDATATIMING0_CH2_MCIO_DDRIO_REG
                                                                    };
  struct channelNvram                         (*ChannelNvList)[MAX_CH];
  UINT8                                       Index;
  INT16                                       Positive;
  INT16                                       Negative;
  UINT8                                       NumChPerMc;

  NumChPerMc    = GetNumChannelPerMc ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  Index = Ch % NumChPerMc;
  IntfDataTiming0.Data = MemReadPciCfgEp (Socket, Ch, IntfDataTiming0Ch[Index]);

  if (CwlAdj <= 0) {
    Positive = 0;
    Negative = ABS (CwlAdj);
  } else {
    Positive = ABS (CwlAdj);
    Negative = 0;
  }

  if ((SetType == CWLADJ_ALL) || (SetType == CWLADJ_DDR4)) {
    IntfDataTiming0.Bits.t_cwl_ddr4_adj = Positive;
    IntfDataTiming0.Bits.t_cwl_ddr4_adj_neg = Negative;
  }

  if ((SetType == CWLADJ_ALL) || (SetType == CWLADJ_DDRT)) {
    IntfDataTiming0.Bits.t_cwl_ddrt_adj = Positive;
    IntfDataTiming0.Bits.t_cwl_ddrt_adj_neg = Negative;
  }

  MemWritePciCfgEp (Socket, Ch, IntfDataTiming0Ch[Index], IntfDataTiming0.Data);
  (*ChannelNvList)[Ch].DdrCrintfDataTiming0 = IntfDataTiming0.Data;
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
  DATACONTROL0N0_0_MCIO_DDRIO_STRUCT  dataControl0;

  dataControl0.Data = ReadDdrioDataRegCh (Host, Socket, Ch, 0, DATACONTROL0N0_0_MCIO_DDRIO_REG); //strobe 0
  return dataControl0.Bits.longpreambleenable;
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
  DDRCRINTFIOLATENCY1_CH0_MCIO_DDRIO_STRUCT IoLatency1;
  UINT32                                    IoLatency1Ch[] = {DDRCRINTFIOLATENCY1_CH0_MCIO_DDRIO_REG,
                                                              DDRCRINTFIOLATENCY1_CH1_MCIO_DDRIO_REG,
                                                              DDRCRINTFIOLATENCY1_CH2_MCIO_DDRIO_REG
                                                             };
  struct channelNvram                       (*channelNvList)[MAX_CH];
  UINT8                                     Index;
  UINT8 ChannelLocal;
  UINT8                                     NumChPerMc;
  UINT8                                     MaxChDdr;

  MaxChDdr   = GetMaxChDdr ();
  NumChPerMc = GetNumChannelPerMc ();

  if (Ch >= MaxChDdr) {
    // invalid ch specified
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_123);
    return;
  }
  ChannelLocal = Ch;
  if (IsBrsPresent(Host, Socket)) {
    Mode = Mode | GSM_READ_CSR;
    if (SubCh != NO_SUBCH) {
      GetChannelMapping(Ch, SubCh, &ChannelLocal);
    }
  }
  channelNvList = GetChannelNvList(Host, Socket);

  Index = ChannelLocal % NumChPerMc;

  if (Mode & GSM_READ_CSR) {
    IoLatency1.Data = MemReadPciCfgEp (Socket, ChannelLocal, IoLatency1Ch[Index]);
  } else {
#ifdef DDR5_SUPPORT
    IoLatency1.Data =((*channelNvList)[Ch].ioLatency1[SUB_CH_A]);
#else
    IoLatency1.Data =((*channelNvList)[Ch].ioLatency1);
#endif // #ifdef DDR5_SUPPORT
  }

  *IoLatIoComp = IoLatency1.Bits.io_lat_io_comp;
}

/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
VOID
WriteFastBootTimings(
             PSYSHOST                                    Host,
             UINT8                                       socket,
             UINT8                                       ch,
             UINT8                                       dimm,
             UINT8                                       rank,
             UINT8                                       strobe
)
{
  struct channelNvram           (*channelNvList)[MAX_CH];
  struct ddrRank                (*rankList)[MAX_RANK_DIMM];
  UINT8                         Bit;
  INT16                         ZeroOffset = 0;

  channelNvList = GetChannelNvList(Host, socket);
  rankList = GetRankNvList(Host, socket, ch, dimm);

  for (Bit = 0; Bit < BITS_PER_NIBBLE; Bit++) {
    GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, Bit, DdrLevel, RxSenseAmpOffset, GSM_WRITE_OFFSET | GSM_FORCE_WRITE, &ZeroOffset);
  }
  GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, NO_BIT, DdrLevel, RxFifoDqsPBitSkew, GSM_WRITE_OFFSET | GSM_FORCE_WRITE, &ZeroOffset);
  GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, NO_BIT, DdrLevel, RxFifoDqsNBitSkew, GSM_WRITE_OFFSET | GSM_FORCE_WRITE, &ZeroOffset);
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
  UINT8                                     RankIndex;
  UINT8                                     IoLatency[MAX_RANK_CH];
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
  UINT8                                     Index;
  UINT8                                     NumChPerMc;
  UINT8                                     MaxChDdr;

  MaxChDdr   = GetMaxChDdr ();
  NumChPerMc = GetNumChannelPerMc ();

  if (Ch >= MaxChDdr) {
    // invalid ch specified
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_142);
    return;
  }

  ChannelNvList = GetChannelNvList (Host, Socket);

  Index = Ch % NumChPerMc;

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
  IoLatency0.Bits.io_lat_rank0 = IoLatency[0];
  IoLatency0.Bits.io_lat_rank1 = IoLatency[1];
  IoLatency0.Bits.io_lat_rank2 = IoLatency[2];
  IoLatency0.Bits.io_lat_rank3 = IoLatency[3];
  IoLatency0.Bits.io_lat_rank4 = IoLatency[4];
  IoLatency0.Bits.io_lat_rank5 = IoLatency[5];
  IoLatency0.Bits.io_lat_rank6 = IoLatency[6];
  IoLatency0.Bits.io_lat_rank7 = IoLatency[7];
  MemWritePciCfgEp (Socket, Ch, IoLatency0Ch[Index], IoLatency0.Data);

  //
  // Write per-channel IoComp from cache to the register
  //
#ifdef DDR5_SUPPORT
  MemWritePciCfgEp (Socket, Ch, IoLatency1Ch[Index], (*ChannelNvList)[Ch].ioLatency1[SUB_CH_A]);
#else
  MemWritePciCfgEp (Socket, Ch, IoLatency1Ch[Index], (*ChannelNvList)[Ch].ioLatency1);
#endif // #ifdef DDR5_SUPPORT
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
  UINT8                                       Index;
  UINT8                                       NumChPerMc;
  UINT32                                      Offset[] = {DDRCRINTFDATATIMING0_CH0_MCIO_DDRIO_REG,
                                                          DDRCRINTFDATATIMING0_CH1_MCIO_DDRIO_REG,
                                                          DDRCRINTFDATATIMING0_CH2_MCIO_DDRIO_REG
                                                         };
  struct channelNvram                         (*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);
  NumChPerMc    = GetNumChannelPerMc ();
  Index = Ch % NumChPerMc;

  RestoreRoundTrip (Host, Socket, Ch);
  RestoreIoLatency (Host, Socket, Ch);

  MemWritePciCfgEp (Socket, Ch, Offset[Index], (*ChannelNvList)[Ch].DdrCrintfDataTiming0);
}

/**

  This function implements Sense Amp Offset training.
  SenseAmp offset cancellation

  @param Host    - Pointer to sysHost

  @retval SUCCESS  - Sense Amp Offset training succeded
  @retval !SUCCESS - Sense Amp Offset training failed

**/
UINT32
EFIAPI
SenseAmpOffset (
  PSYSHOST  Host
  )
{
  UINT8                                 socket;
  UINT8                                 ch;
  UINT8                                 dimm;
  UINT8                                 rank;
  UINT8                                 strobe;
  UINT8                                 strobeWA;
  UINT8                                 bit;
  MRC_STATUS                            Status = MRC_STATUS_SUCCESS;
  INT16                                 (*firstZero)[MAX_CH][MAX_STROBE][MAX_BITS];
  INT16                                 lastOne[MAX_CH][MAX_STROBE][MAX_BITS];
  INT16                                 sampOffset;
  INT16                                 vref;
  struct channelNvram                   (*channelNvList)[MAX_CH];
  struct dimmNvram                      (*dimmNvList)[MAX_DIMM];
  struct ddrRank                        (*rankList)[MAX_RANK_DIMM];
  struct rankDevice                     (*rankStruct)[MAX_RANK_DIMM];
  DATATRAINFEEDBACKN0_0_MCIO_DDRIO_STRUCT dataTrainFeedback;
  DATACONTROL0N0_0_MCIO_DDRIO_STRUCT    dataControl0;
  DATACONTROL2N0_0_MCIO_DDRIO_STRUCT    dataControl2;
  DATACONTROL4N0_0_MCIO_DDRIO_STRUCT    dataControl4;
  UINT8                                 LoopCount;
  UINT8                                 MaxLoopCount;
  UINT8 MSVx4 = GetMaxStrobeValid(Host);
  UINT8                                 MaxChDdr;

#ifdef DEBUG_CODE_BLOCK
  UINT8             MSVx8 = GetMaxStrobeValid(Host) / 2;
  UINT64            startTsc = 0;
  UINT64            endTsc = 0;
#endif  // DEBUG_CODE_BLOCK

  if (!IsMemFlowEnabled (SenseAmpOffsetTraining)) {
    return Status;
  }

  firstZero = RcAllocatePool (sizeof (*firstZero));
  if (firstZero == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return MRC_STATUS_FAILURE;
  }
#ifdef DEBUG_CODE_BLOCK
  //
  // Start time
  //
  startTsc = GetPerformanceCounter ();
#endif // DEBUG_CODE_BLOCK

  socket = Host->var.mem.currentSocket;
  OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_SENSE_AMP, socket);
  channelNvList = GetChannelNvList(Host, socket);
  MaxChDdr = GetMaxChDdr ();

  MaxLoopCount = 1;
  if (PcdGetBool (PcdMrcDebugSenseAmp) == TRUE) {
    MaxLoopCount = 100;
  }

  for (LoopCount = 0; LoopCount < MaxLoopCount; LoopCount++) {

    OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, LoopCount + 1, socket);

    for (ch = 0; ch < MaxChDdr; ch++) {

      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      dimmNvList = GetDimmNvList(Host, socket, ch);

      //
      // Set Rtt Park to disabled
      //
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        //
        // Skip if no DIMM present
        //
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }

        rankStruct  = GetRankStruct(Host, socket, ch, dimm);

        for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
          //
          // Skip if no rank
          //
          if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

#ifdef LRDIMM_SUPPORT
          if (IsLrdimmPresent (socket, ch, dimm)) {
            WriteLrbuf(Host, socket, ch, dimm, 0, DDR4_RTT_PARK_DIS, LRDIMM_F0, LRDIMM_BC02);
          } else
#endif // #ifdef LRDIMM_SUPPORT
          {
#ifndef DDR5_SUPPORT
            WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR5 & ~DDR4_RTT_PRK_ODT_MASK, BANK5);
#endif // !DDR5_SUPPORT
          }
        } // rank loop
      } // dimm loop

      // enable senseamp training mode
      // SKX HSD 4927892
      for (strobe = 0; strobe < MSVx4; strobe++) {
        if (IsStrobeNotValid(Host, strobe)) {
          continue;
        }

        // datacontrol2
        dataControl2.Data                 = (*channelNvList)[ch].dataControl2[strobe];
        dataControl2.Bits.rxvocselqnnnh   = 0x1f;
        dataControl2.Bits.forcerxon       = 1;
        WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL2N0_0_MCIO_DDRIO_REG, dataControl2.Data);

        // datacontrol4
        dataControl4.Data               = (*channelNvList)[ch].dataControl4[strobe];
        dataControl4.Bits.rxvrefsel     = 0; //HSD 4927283 should be 8 initially
        WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL4N0_0_MCIO_DDRIO_REG, dataControl4.Data);

        // datacontrol0
        dataControl0.Data                       = (*channelNvList)[ch].dataControl0;  // going to add strobe soon
        dataControl0.Bits.senseamptrainingmode  = 0;
        dataControl0.Bits.forceodton            = 1;
        dataControl0.Bits.saoffsettraintxon     = 1;
        WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, dataControl0.Data);

        for (bit = 0; bit < BITS_PER_NIBBLE; bit++) {
          (*firstZero)[ch][strobe][bit] = 99;
          lastOne[ch][strobe][bit] = 99;
        } // bit loop
      } // nibble loop


#ifdef DEBUG_CODE_BLOCK
      RcDebugPrint (SDBG_MAX,
                     "\nSampOffset Training\n");
      RcDebugPrint (SDBG_MAX,
                     "               ");
      for (strobe = 0; strobe < MSVx4 ; strobe++) {
        if (IsStrobeNotValid(Host, strobe)) {
          continue;
        }
        RcDebugPrint (SDBG_MAX,
                       "%2d %2d ", (strobe+9)%18,strobe);
      } // strobe loop

      RcDebugPrint (SDBG_MAX,
                     "\n            SA ");
      for (strobe = 0; strobe < MSVx8; strobe++) {
        if (IsStrobeNotValid(Host, strobe)) {
          continue;
        }
        RcDebugPrint (SDBG_MAX,
                       "01 23 01 23 ");
      } // strobe loop
      RcDebugPrint (SDBG_MAX, "\n");

#endif // DEBUG_CODE_BLOCK

      for (sampOffset = 0; sampOffset <= 31; sampOffset++) {
        if (!FindFirstDimmInChannel (socket, ch, &dimm)) {
          RC_FATAL_ERROR (FALSE, ERR_DIMM_COMPAT, RC_FATAL_ERROR_MINOR_CODE_146);
        }
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }

        rankList = GetRankNvList(Host, socket, ch, dimm);
        //for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
        //
        // This only needs to get set for rank 0
        //
        rank = 0;
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, ALL_BITS, DdrLevel, RxSenseAmpOffset, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &sampOffset);

        // Propagate delay values (without a read command)
        dataControl0.Bits.senseamptrainingmode  = 1;

        // change for upper nibble
        for (strobe = 0; strobe < MSVx4 ; strobe++) {
          if (IsStrobeNotValid(Host, strobe)) {
            continue;
          }

          //
          // Delay only if this is a CTE build, otherwise do nothing.
          //
          CteDelayQclk (1); // needed for delay before setting readrf* =1

          WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, dataControl0.Data);
        } // strobe loop

        //
        // Delay if this is a CTE build, otherwise do nothing.
        //

        CteDelayDclk (4);

        IO_Reset(Host, socket);
        // change for upper nibble
        for (strobe = 0; strobe < MSVx4 ; strobe++) {
          if (IsStrobeNotValid(Host, strobe)) {
            continue;
          }
          WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, dataControl0.Data);
        } // strobe loop

        SenseAmpOffsetDelayTarget (Host);

        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                       "VOC %2d ", sampOffset);

        for (strobe = 0; strobe < MSVx4; strobe++) {
          if (IsStrobeNotValid(Host, strobe)) {
            continue;
          }
          dataTrainFeedback.Data = ReadDdrioDataRegCh (Host, socket, ch, strobe, DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG);

          AdjustSenseAmpFeedbackTarget (sampOffset, strobe, ch, (INT16 *)&dataTrainFeedback.Data);

          for (bit = 0; bit < BITS_PER_NIBBLE; bit++) {
            //SKX note: CTE model requires a change to match HW polarity
            //SKX HSD 4928076 swizzle SA feedback, this will recorded the feedback in the appropriate nibble
            if (bit < 2){
              strobeWA = (strobe+9)%18;
            } else {
              strobeWA = strobe;
            }
#ifdef DDR5_SUPPORT
            if (IsDdr5Present(Host, socket)) {
              strobeWA = strobe;
            }
#endif //DDR5_SUPPORT
            // HSD
            if (dataTrainFeedback.Bits.datatrainfeedback & (1 << bit)) {
              lastOne[ch][strobeWA][bit] = sampOffset;
            } else {
              if ((*firstZero)[ch][strobeWA][bit] == 99) {
                (*firstZero)[ch][strobeWA][bit] = sampOffset;
              }
            }

            //Display in bits
            if ((1 << bit) & dataTrainFeedback.Bits.datatrainfeedback) {
              RcDebugPrint (SDBG_MAX,
                             "1");
            } else {
              RcDebugPrint (SDBG_MAX,
                             "0");
            }
            if (bit & BIT0) {
              RcDebugPrint (SDBG_MAX," ");
            }
          } // bit
        } // strobe loop

        SenseAmpOffsetDelayTarget (Host);

        dataControl0.Bits.senseamptrainingmode = 0;
        // SKX change for upper nibble
        for (strobe = 0; strobe < MSVx4 ; strobe++) {
          if (IsStrobeNotValid(Host, strobe)) {
            continue;
          }
          WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, dataControl0.Data);
        } // strobe loop

        RcDebugPrint (SDBG_MAX,
                       "\n");
      } // sampOffset loop
      RcDebugPrint (SDBG_MAX,
          "\nBitSAmp\n              bit:  0  1  2  3\n");

      // Calculate and Program Offsets and display per bit SenseAmp Offset
      for (strobe = 0; strobe < MSVx4; strobe++) {
        if (IsStrobeNotValid(Host, strobe)) {
          continue;
        }

        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                       "Nibble %2d:", strobe);

        for (bit = 0; bit < BITS_PER_NIBBLE; bit++) {
          // Find vref center, add 1 for Round Up
          vref = ((*firstZero)[ch][strobe][bit] + lastOne[ch][strobe][bit]) / 2;

          // Check for saturation conditions
          // to make sure we are as close as possible to vdd/2 (750mv)
          if ((*firstZero)[ch][strobe][bit] == 99) {   // never saw a 0
            vref = 31;
          }
          if (lastOne[ch][strobe][bit] == 99) {     // never saw a 1
            vref = 0;
          }

          RcDebugPrint (SDBG_MAX,
                         "%3d", vref);
          //
          // Set value for this strobe in all dimms/ranks
          //
          for (dimm = 0; dimm < MAX_DIMM; dimm++) {
            //
            // Skip if no DIMM present
            //
            if ((*dimmNvList)[dimm].dimmPresent == 0) {
              continue;
            }
            for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
              //
              // Skip if no rank
              //
              if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
                continue;
              }
              GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, strobe, bit, DdrLevel, RxSenseAmpOffset, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &vref);
            } //rank
          } //dimm

        } // bit loop

        RcDebugPrint (SDBG_MAX,
                       "\n");
      } // strobe loop

      // Propagate delay values (without a read command)
      dataControl0.Data            = (*channelNvList)[ch].dataControl0;
      // change for upper nibble
      for (strobe = 0; strobe < MSVx4 ; strobe++) {
        if (IsStrobeNotValid(Host, strobe)) {
          continue;
        }

        //
        // Delay only if this is a CTE build, otherwise do nothing.
        //
        CteDelayQclk (1); // needed for delay before setting readrf* =1

        WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, dataControl0.Data);
      } // strobe loop

      RcDebugPrint (SDBG_MAX,
                     "\n");

      for (strobe = 0; strobe < MSVx4; strobe++) { // set both nibbles
        if (IsStrobeNotValid(Host, strobe)) {
          continue;
        }
        // Clean up after test
        // Disable force Sense amp
        // restore original datacontrol values
        WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL4N0_0_MCIO_DDRIO_REG, (*channelNvList)[ch].dataControl4[strobe]);
        WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, (*channelNvList)[ch].dataControl0);
        WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL2N0_0_MCIO_DDRIO_REG, (*channelNvList)[ch].dataControl2[strobe]);
      } // strobe loop

      RcDebugPrint (SDBG_MAX,
                   "\n");

      //
      // Restore Rtt Park
      //
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        //
        // Skip if no DIMM present
        //
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }

        rankList    = GetRankNvList(Host, socket, ch, dimm);
        rankStruct  = GetRankStruct(Host, socket, ch, dimm);

        for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
          //
          // Skip if no rank
          //
          if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

#ifndef DDR5_SUPPORT
          WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR5, BANK5);
#endif // !DDR5_SUPPORT
        } // rank loop
      } // dimm loop
    } // ch loop

    IO_Reset(Host, socket);

  } // LoopCount

#ifdef DEBUG_CODE_BLOCK
  //
  // End time
  //
  endTsc = GetPerformanceCounter ();
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "SenseAmpOffset - %dms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
#endif // DEBUG_CODE_BLOCK
  RcFreePool (firstZero);
  return Status;
}

/**
  Displays the Sense Amp register settings

  @param socket:       Processor socket

  @retval N/A
**/
VOID
DisplaySenseAmpTrainingResults(
  UINT8         socket
)
{
  UINT8                                     ch;
  UINT8                                     dimm;
  UINT8                                     rank;
  UINT8                                     Strobe;
  UINT8                                     Bit;
  INT16                                     SenseAmpOffsetValue;
  struct channelNvram                       (*channelNvList)[MAX_CH];
  struct dimmNvram                          (*dimmNvList)[MAX_DIMM];
  struct ddrRank                            (*rankList)[MAX_RANK_DIMM];
  PSYSHOST  Host;
  UINT8  MSVx4;
  UINT8  MaxChDdr;

  Host = GetSysHostPointer ();
  MSVx4 = GetMaxStrobeValid(Host);

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, socket);

  RcDebugPrint (SDBG_MEM_TRAIN,
                    "\nSTART_SENSE_AMP_TRAINING_OFFSETS\n");
  RcDebugPrint (SDBG_MAX,
        "Senseamp           bit:   0  1  2  3\n");
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
      //
      // This value available only for rank 0
      //
      rank = 0;
      if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
        continue;
      }

      for (Strobe = 0; Strobe < MSVx4; Strobe++) {
        if (IsStrobeNotValid(Host, Strobe)) {
          continue;
        }

        RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "Nibble %2d: ", Strobe);
        for (Bit = 0; Bit < BITS_PER_NIBBLE; Bit++) {
          GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, Strobe, Bit, DdrLevel, RxSenseAmpOffset, GSM_READ_ONLY, &SenseAmpOffsetValue);
          RcDebugPrint (SDBG_MEM_TRAIN,
                        " %2d", SenseAmpOffsetValue);
        }
        RcDebugPrint (SDBG_MEM_TRAIN,
                      "\n");
      } // Strobe loop

      break;
    }
  }
  RcDebugPrint (SDBG_MEM_TRAIN,
                    "\n");
} //DisplaySenseAmpTrainingResults

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
  UINT8       Index;
  UINT8       NumChPerMc;
  DDRCRINTFDATATIMING0_CH0_MCIO_DDRIO_STRUCT DdrCrCh;
  UINT32                                     Offset[] = {DDRCRINTFDATATIMING0_CH0_MCIO_DDRIO_REG,
                                                         DDRCRINTFDATATIMING0_CH1_MCIO_DDRIO_REG,
                                                         DDRCRINTFDATATIMING0_CH2_MCIO_DDRIO_REG
                                                        };
  struct channelNvram                        (*ChannelNvList)[MAX_CH];

  Host = GetSysHostPointer ();
  NumChPerMc = GetNumChannelPerMc ();
  Status = MRC_STATUS_SUCCESS;

  ChannelNvList = GetChannelNvList (Host, Socket);
  Index = Ch % NumChPerMc;
  DdrCrCh.Data = MemReadPciCfgEp (Socket, Ch, Offset[Index]);
  //
  // The desing supports 2 Dimms and 4 ranks per dimm. DDRT dimms are single rank.
  //
  DdrCrCh.Bits.rank_ddr4_ddrt = 1 << (Dimm * 4);
  MemWritePciCfgEp (Socket, Ch, Offset[Index], DdrCrCh.Data);

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
  PSYSHOST Host;
  MRC_STATUS  Status;
  DDRCRSPDCFG1_MCIO_DDRIOEXT_STRUCT DdrCrSpdCfg1;
  UINT8 Ch;
  UINT8 McId;
  UINT8 SocketChannel;
  UINT8 NumChPerMc;
  UINT8 MaxImc;
  struct channelNvram(*ChannelNvList)[MAX_CH];

  NumChPerMc = GetNumChannelPerMc ();
  Host       = GetSysHostPointer();
  MaxImc     = GetMaxImc ();
  Status = MRC_STATUS_SUCCESS;

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (McId = 0; McId < MaxImc; McId++) {
    if (Host->var.mem.socket[Socket].imcEnabled[McId] == 0) {
      continue;
    }
    for (Ch = 0; Ch < NumChPerMc; Ch++) {
      SocketChannel = (McId * NumChPerMc) + Ch;
      if ((*ChannelNvList)[SocketChannel].enabled == 0) {
        continue;
      }
      if ((ChannelBitMask & (1 << SocketChannel)) == 0) {
        continue;
      }
      DdrCrSpdCfg1.Data = MemReadPciCfgMC (Socket, McId, DDRCRSPDCFG1_MCIO_DDRIOEXT_REG);
      if (IsAssert) {
        DdrCrSpdCfg1.Bits.dramresetn = 0;
      } else {
        DdrCrSpdCfg1.Bits.dramresetn = 1;
      }
      MemWritePciCfgMC (Socket, McId, DDRCRSPDCFG1_MCIO_DDRIOEXT_REG, DdrCrSpdCfg1.Data);
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
  COMPDATA0N0_0_MCIO_DDRIO_STRUCT   CompData0;
  COMPDATA1N0_0_MCIO_DDRIO_STRUCT   CompData1;

  SetMem(&CompData0, sizeof (COMPDATA0N0_0_MCIO_DDRIO_STRUCT), 0);
  SetMem(&CompData1, sizeof (COMPDATA0N0_0_MCIO_DDRIO_STRUCT), 0);

  if (DataCompValue == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  switch (DataCompParam) {
    case DataCompRCompDrvUp:
    case DataCompRCompDrvDn:
    case DataCompSComp:
    case DataCompTcoComp:
      CompData0.Data = ReadDdrioDataRegCh (Host, Socket, Ch, Strobe, COMPDATA0N0_0_MCIO_DDRIO_REG);
      break;
    case DataCompOdtUp:
    case DataCompOdtDn:
      CompData0.Data = ReadDdrioDataRegCh (Host, Socket, Ch, Strobe, COMPDATA1N0_0_MCIO_DDRIO_REG);
      break;
    default:
      return EFI_INVALID_PARAMETER;
  }

  switch (DataCompParam) {
    case DataCompRCompDrvUp:
      *DataCompValue = CompData0.Bits.rcompdrvup;
      break;
    case DataCompRCompDrvDn:
      *DataCompValue = CompData0.Bits.rcompdrvdown;
      break;
    case DataCompSComp:
      *DataCompValue = CompData0.Bits.slewratecomp;
      break;
    case DataCompTcoComp:
      *DataCompValue = CompData0.Bits.tcocomp;
      break;
    case DataCompOdtUp:
      *DataCompValue = CompData1.Bits.rcompodtup;
      break;
    case DataCompOdtDn:
      *DataCompValue = CompData1.Bits.rcompodtdown;
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
  UINT8                                     McId;
  DDRCRCLKCOMP_CLK_MCIO_DDRIOEXT_STRUCT     DdrCRClkComp;
  DDRCRCMDCOMP_CMDN_MCIO_DDRIOEXT_STRUCT    DdrCRCmdCompCmdn;
  DDRCRCMDCOMP_CMDS_MCIO_DDRIOEXT_STRUCT    DdrCRCmdCompCmds;
  DDRCRCMDCOMP_CKE_MCIO_DDRIOEXT_STRUCT     DdrCRCmdCompCke;
  DDRCRCMDCOMP_CTL_MCIO_DDRIOEXT_STRUCT     DdrCRCmdCompCtl;
  DDRCRCTLCOMP_CMDN_MCIO_DDRIOEXT_STRUCT    DdrCRCtlCompCmdn;
  DDRCRCTLCOMP_CMDS_MCIO_DDRIOEXT_STRUCT    DdrCRCtlCompCmds;
  DDRCRCTLCOMP_CKE_MCIO_DDRIOEXT_STRUCT     DdrCRCtlCompCke;
  DDRCRCTLCOMP_CTL_MCIO_DDRIOEXT_STRUCT     DdrCRCtlCompCtl;

  if (CompValues == NULL) {
      return EFI_INVALID_PARAMETER;
  }

  McId = GetMCID (Host, Socket, Ch);

  switch (CompGroup) {
    case ClkCompGroup:
      DdrCRClkComp.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCLKCOMP_CLK_MCIO_DDRIOEXT_REG);
      CompValues->DrvUp   = DdrCRClkComp.Bits.rcompdrvup;
      CompValues->DrvDn   = DdrCRClkComp.Bits.rcompdrvdown;
      CompValues->SComp   = DdrCRClkComp.Bits.scomp;
      CompValues->TcoComp = DdrCRClkComp.Bits.tcocomp;
      break;
    case CmdNCompGroup:
      DdrCRCmdCompCmdn.Data = MemReadPciCfgMC (Socket, McId, DDRCRCMDCOMP_CMDN_MCIO_DDRIOEXT_REG);
      CompValues->DrvUp   = DdrCRCmdCompCmdn.Bits.rcompdrvup;
      CompValues->DrvDn   = DdrCRCmdCompCmdn.Bits.rcompdrvdown;
      CompValues->SComp   = DdrCRCmdCompCmdn.Bits.scomp;
      CompValues->TcoComp = DdrCRCmdCompCmdn.Bits.tcocomp;
      break;
    case CmdSCompGroup:
      DdrCRCmdCompCmds.Data = MemReadPciCfgMC (Socket, McId, DDRCRCMDCOMP_CMDS_MCIO_DDRIOEXT_REG);
      CompValues->DrvUp   = DdrCRCmdCompCmds.Bits.rcompdrvup;
      CompValues->DrvDn   = DdrCRCmdCompCmds.Bits.rcompdrvdown;
      CompValues->SComp   = DdrCRCmdCompCmds.Bits.scomp;
      CompValues->TcoComp = DdrCRCmdCompCmds.Bits.tcocomp;
      break;
    case CmdCkeCompGroup:
      DdrCRCmdCompCke.Data = MemReadPciCfgMC (Socket, McId, DDRCRCMDCOMP_CKE_MCIO_DDRIOEXT_REG);
      CompValues->DrvUp   = DdrCRCmdCompCke.Bits.rcompdrvup;
      CompValues->DrvDn   = DdrCRCmdCompCke.Bits.rcompdrvdown;
      CompValues->SComp   = DdrCRCmdCompCke.Bits.scomp;
      CompValues->TcoComp = DdrCRCmdCompCke.Bits.tcocomp;
      break;
    case CmdCtlCompGroup:
      DdrCRCmdCompCtl.Data = MemReadPciCfgMC (Socket, McId, DDRCRCMDCOMP_CTL_MCIO_DDRIOEXT_REG);
      CompValues->DrvUp   = DdrCRCmdCompCtl.Bits.rcompdrvup;
      CompValues->DrvDn   = DdrCRCmdCompCtl.Bits.rcompdrvdown;
      CompValues->SComp   = DdrCRCmdCompCtl.Bits.scomp;
      CompValues->TcoComp = DdrCRCmdCompCtl.Bits.tcocomp;
      break;
    case CtlNCompGroup:
      DdrCRCtlCompCmdn.Data = MemReadPciCfgMC (Socket, McId, DDRCRCTLCOMP_CMDN_MCIO_DDRIOEXT_REG);
      CompValues->DrvUp   = DdrCRCtlCompCmdn.Bits.rcompdrvup;
      CompValues->DrvDn   = DdrCRCtlCompCmdn.Bits.rcompdrvdown;
      CompValues->SComp   = DdrCRCtlCompCmdn.Bits.scomp;
      CompValues->TcoComp = DdrCRCtlCompCmdn.Bits.tcocomp;
      break;
    case CtlSCompGroup:
      DdrCRCtlCompCmds.Data = MemReadPciCfgMC (Socket, McId, DDRCRCTLCOMP_CMDS_MCIO_DDRIOEXT_REG);
      CompValues->DrvUp   = DdrCRCtlCompCmds.Bits.rcompdrvup;
      CompValues->DrvDn   = DdrCRCtlCompCmds.Bits.rcompdrvdown;
      CompValues->SComp   = DdrCRCtlCompCmds.Bits.scomp;
      CompValues->TcoComp = DdrCRCtlCompCmds.Bits.tcocomp;
      break;
    case CtlCkeCompGroup:
      DdrCRCtlCompCke.Data = MemReadPciCfgMC (Socket, McId, DDRCRCTLCOMP_CKE_MCIO_DDRIOEXT_REG);
      CompValues->DrvUp   = DdrCRCtlCompCke.Bits.rcompdrvup;
      CompValues->DrvDn   = DdrCRCtlCompCke.Bits.rcompdrvdown;
      CompValues->SComp   = DdrCRCtlCompCke.Bits.scomp;
      CompValues->TcoComp = DdrCRCtlCompCke.Bits.tcocomp;
      break;
    case CtlCtlCompGroup:
      DdrCRCtlCompCtl.Data = MemReadPciCfgMC (Socket, McId, DDRCRCTLCOMP_CTL_MCIO_DDRIOEXT_REG);
      CompValues->DrvUp   = DdrCRCtlCompCtl.Bits.rcompdrvup;
      CompValues->DrvDn   = DdrCRCtlCompCtl.Bits.rcompdrvdown;
      CompValues->SComp   = DdrCRCtlCompCtl.Bits.scomp;
      CompValues->TcoComp = DdrCRCtlCompCtl.Bits.tcocomp;
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
      ClkNumber = 1;        // CLK 1 for ICX/DIMM0
      break;
    case 1:
      ClkNumber = 3;        // CLK 3 for ICX/DIMM1
      break;
    default:
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL,  RC_FATAL_ERROR_MINOR_CODE_148);
      break;
  } // switch

 return ClkNumber;

} // GetDdrioClkNumberDdrt

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
  //
  // The register default value is 0 that chooses the Alert_N as the feedback.
  //
  return;
}

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
  return;
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
  UINT8 ch = 0;
  for (ch = 0; ch < MAX_CH; ch++) {
    //
    // Enable CK for all ranks
    //
    SetRankEnDdrio (Host, Socket, ch, 0xF);
  }
  return;
}

/**

  DDRIO Init for QCS CLK Training.

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Current Socket
  @param[in] Dimm      - Current Dimm

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
  return;
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
  return;
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
  //
  // Unsupported
  //
  return;
}

/**

DDRIO Init in Pre Training.

@param host      - Pointer to sysHost
@param socket    - Current Socket

@retval N/A

**/
VOID
EFIAPI
PretrainingDdrioInit (
  PSYSHOST  Host,
  UINT8     Socket
  )
{
  return;
} // PretrainingDdrioInit


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
  PSYSHOST                              Host;
  UINT8                                 Strobe;
  UINT8                                 Channel;
  UINT8                                 MSVx4;
  struct channelNvram                   (*ChannelNvList)[MAX_CH];
  DATACONTROL0N0_0_MCIO_DDRIO_STRUCT    DataControl0;
  DATACONTROL2N0_0_MCIO_DDRIO_STRUCT    DataControl2;
  UINT8                                 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  MSVx4 = GetMaxStrobeValid (Host);
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if ((*ChannelNvList)[Channel].enabled == 0) {
      continue;
    }

    //
    // For DataControl0, all strobes in a channel share the same cached value
    //
    DataControl0.Data = (*ChannelNvList)[Channel].dataControl0;
    DataControl0.Bits.internalclockson = FixedPcdGet8 (PcdDdrInternalClocksOn);

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (IsStrobeNotValid (Host, Strobe)) {
        continue;
      }
      if (DataControl0.Data != (*ChannelNvList)[Channel].dataControl0) {
        //
        // Update the per-strobe register here but don't update the per-channel cached value until
        // after the strobe loop exits. Otherwise the comparison above will always be FALSE after strobe 0.
        //
        WriteDdrioDataRegCh (Host, Socket, Channel, Strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, DataControl0.Data);
      }

      //
      // For DataControl2, each strobe has its own cache
      //
      DataControl2.Data = (*ChannelNvList)[Channel].dataControl2[Strobe];
      if (IsSiliconWorkaroundEnabled ("S1408263639")) {
        DataControl2.Bits.forcerxon = 1;
      }
      DataControl2.Bits.forcebiason = FixedPcdGet8 (PcdDdrForceBiasOn);
      if ((*ChannelNvList)[Channel].dataControl2[Strobe] != DataControl2.Data) {
        WriteDdrioDataRegCh (Host, Socket, Channel, Strobe, DATACONTROL2N0_0_MCIO_DDRIO_REG, DataControl2.Data);
        (*ChannelNvList)[Channel].dataControl2[Strobe] = DataControl2.Data;
      }
    } // Strobe
    (*ChannelNvList)[Channel].dataControl0 = DataControl0.Data;
  } // Channel
  return EFI_SUCCESS;
}

/**

  Set Qlbd and Qlbs Io direction.

  @param[in] Host              - Pointer to sysHost, the system Host (root) structure struct
  @param[in] Socket            - Socket Id
  @param[in] RxPathUngateValue - Rx Path Ungate Value to program

  @retval  None

**/
VOID
SetQlbdQlbsIodirection (
  IN PSYSHOST                  Host,
  IN UINT8                     Socket,
  IN UINT8                     RxPathUngateValue
  )

{
  return;
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
  DDRCRINTFDATATIMING0_CH0_MCIO_DDRIO_STRUCT            IntfDataTiming0;
  UINT32                                                IntfDataTiming0Ch[] = { DDRCRINTFDATATIMING0_CH0_MCIO_DDRIO_REG,
                                                                                DDRCRINTFDATATIMING0_CH1_MCIO_DDRIO_REG,
                                                                                DDRCRINTFDATATIMING0_CH2_MCIO_DDRIO_REG
                                                                              };
  struct channelNvram (*ChannelNvList) [MAX_CH];
  UINT8                                                 Index;
  UINT8                                                 SubCh = 0;
  UINT8                                                 ChannelLocal = 0;
  UINT8                                                 NumChPerMc;

  NumChPerMc = GetNumChannelPerMc ();

  RcDebugPrint (SDBG_MAX,
    "ProgramPdaPbaTimingValues Start \n");

  ChannelNvList = GetChannelNvList (Host, Socket);
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    GetChannelMapping (Channel, SubCh, &ChannelLocal);
    Index = ChannelLocal % NumChPerMc;

    // Program CWL to meet the DDR5 t_PDA/PBA_DQS_Delay timing requirement.
    IntfDataTiming0.Data = MemReadPciCfgEp (Socket, ChannelLocal, IntfDataTiming0Ch[Index]);

    if (OriginalWriteTimings != NULL) {
      //
      // Save original value if caller passed in a buffer
      //
      OriginalWriteTimings->Timings[SubCh] = IntfDataTiming0.Data;
    }

    IntfDataTiming0.Bits.t_cwl_ddr4 = PDA_PBA_DQS_DELAY;
    IntfDataTiming0.Bits.t_cwl_ddr4_adj = 0;
    IntfDataTiming0.Bits.t_cwl_ddr4_adj_neg = 0;
    MemWritePciCfgEp (Socket, ChannelLocal, IntfDataTiming0Ch[Index], IntfDataTiming0.Data);
  } //subch loop
} //ProgramPdaPbaTimingValues

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
ProgramJedecTimingsDdrioBrs (
  IN  PSYSHOST        Host,
  IN  UINT8           Socket,
  IN  UINT8           Ch,
  IN  WRITE_TIMINGS   *OriginalWriteTimings
  )
{
  DDRCRINTFDATATIMING0_CH0_MCIO_DDRIO_STRUCT            IntfDataTiming0;
  UINT8 SubCh = 0;
  UINT8 ChannelLocal = 0;
  UINT8 NumChPerMc;
  UINT32                                                IntfDataTiming0Ch[] = { DDRCRINTFDATATIMING0_CH0_MCIO_DDRIO_REG,
                                                                                DDRCRINTFDATATIMING0_CH1_MCIO_DDRIO_REG,
                                                                                DDRCRINTFDATATIMING0_CH2_MCIO_DDRIO_REG
                                                                              };
  struct channelNvram (*ChannelNvList) [MAX_CH];
  UINT8                                                 Index;
  RcDebugPrint (SDBG_MAX,
    "ProgramJedecTimingsDdrioBrs Start \n");
  ChannelNvList = GetChannelNvList (Host, Socket);
  NumChPerMc    = GetNumChannelPerMc ();

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    GetChannelMapping (Ch, SubCh, &ChannelLocal);
    Index = ChannelLocal % NumChPerMc;

    if (OriginalWriteTimings != NULL) {
      //
      // Use original value is caller passed it in and size is correct
      //
      IntfDataTiming0.Data = OriginalWriteTimings->Timings[SubCh];
    } else {
      //
      // Else program initial settings
      //
      IntfDataTiming0.Data = MemReadPciCfgEp (Socket, ChannelLocal, IntfDataTiming0Ch[Index]);
      IntfDataTiming0.Bits.t_cwl_ddr4 = GettCWL (Host, Socket, Ch);
    }
    MemWritePciCfgEp (Socket, ChannelLocal, IntfDataTiming0Ch[Index], IntfDataTiming0.Data);
  } //subch loop
}

/**
  Configure TX UI (Transmit Unit Interval).

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket number

  @retval EFI_SUCCESS
**/
VOID
ConfigureTxUi(
  IN PSYSHOST                  Host,
  IN UINT8                     Socket
)
{
  return;
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
    MemReadPciCfgMC (Socket, McId, DDRCRCOMPCTL2_MCIO_DDRIOEXT_REG);
  }
}


/**

  Issue a DDRIO COMP cycle at specific points in training

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
  return MRC_FUNCTION_NOT_IMPLEMENTED;
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
 // Wave 3 only
}

/**
Allows Req and Err pin to propagate to MC IP.

@param[in]  Host        - Pointer to sysHost, the system Host (root) structure struct
@param[in]  Socket      - Processor socket within the system (0-based)
@param[in]  Channel     - Channel within the Socket

@retval N/A

**/
VOID
ProgramReqRxPathUngate(
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{
 // Wave 3 only
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
  //
  // Empty function to satsify API
  //
  return EFI_UNSUPPORTED;
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
  return;
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
  return;
}

/**

  Program Read Preamble setting.

  @param socket  - Processor to initialize

  @retval N/A

**/
VOID
EFIAPI
ProgramReadPreamble (
  UINT8    Socket
  )
{
  return;
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
  return;
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
  return;
}

/**

  Get the IO Latency Comp Value

  @retval IO Lat Comp Start Value

**/
UINT8
EFIAPI
GetIoLatCompValueStart (
  VOID
  )
{
  return MRC_ROUND_TRIP_IO_COMP_START;
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
  return MRC_ROUND_TRIP_IO_COMPENSATION;
} // GetIoLatCompValue

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
  return;
} // TxReTrainDdrioPhyUpdate

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
  return;
} //ConfigureDdrioTxRetrain

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
  return;
}

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
  return;
}

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
  IN  INT16                             CwlAdjTargetValueInput[SUB_CH]
  )
{
  return;
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
  return MRC_STATUS_SUCCESS;
}
