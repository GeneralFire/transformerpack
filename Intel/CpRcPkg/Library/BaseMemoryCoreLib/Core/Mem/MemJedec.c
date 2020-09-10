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

#include <Memory/JedecDefinitions.h>
#include <Memory/MemCmdCtlClkCommon.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Library/MemoryCoreLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/PerformanceTrackerLib.h>
#include "Include/BaseMemoryCoreInternal.h"
#include <Library/SpdDecodeLib.h>

                                                    // 800   1000   1066   1200   1333   1400   1600   1800   1866   2000  2133  2200  2400  2600  2666  2800  2933  3000  3200  3400  3467  3600  3733  3800  4000  4200  4266  4400  4800  5200  5600  6000  6400  6800  7200  7600  8000  8400
CONST UINT8  tPARRECOVERY[MAX_SUP_FREQ]           = {    0,     0,     0,     0,   104,   105,   105,   122,   122,   138,  139,  156,  157,  157,  157,  158,  158,  158,  159,  159,  159,  159,  159,  159,  159,  159,  159,  159};
//
// TRR defines
//
#define PTRR_MODE   BIT0
#define TRR_MODE_A  BIT1
#define TRR_IMMUNE  BIT2

//
// Max range of the Output Delay Control (F1RC[1-9]x)
//
#define MAX_BACKSIDE_CTL_CMD_DELAY 31

//
// The PI tick ratio between backside and host.
// Backside PI tick unit is 1/64 tCK.
// Hostside PI tick unit is 1/128 tCK.
//
#define BACKSIDE_TO_HOST_TCK_DELAY_RATIO 2


extern const UINT32  preciseTCK[MAX_SUP_FREQ];

//
// Local Prototypes
//

VOID    DoRegisterInitDDR4_CKE (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank);
VOID    ProgramMRSDDR4 (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank);
VOID    ProgramMRSAEP (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank);
#ifdef LRDIMM_SUPPORT
VOID    DoLrbufInitDDR4 (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank);
#endif // LRDIMM_SUPPORT

VOID
IndirectRCWrite (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     controlWordData,
  UINT8     controlWordFunc,
  UINT8     controlWordAddr
  );

// -------------------------------------
// Code section
// -------------------------------------


/**

  Return the time required for the parity recovery for DDR4 in DCLKs

  @param[in]    Socket        Current socket under test (0-based)
  @param[in]    RatioIndex    Current ratio index (frequency)

  @return       return the parity recovery in DCLKs

**/
UINT32
EFIAPI
GetParityRecoveryDdr4 (
  IN        UINT8           Socket,
  IN        UINT8           RatioIndex
)
{
  return tPARRECOVERY[RatioIndex];
}

/**
  Muli-use function to either get or set control delays based on the provided group number

  @param Host:     Pointer to SYSHOST
  @param socket:   Socket number
  @param ch:       Channel number
  @param dimm:     Dimm number
  @param level:    Level of the bus
  @param group:    Group number
  @param mode:     GSM_READ_CSR - Read the data from hardware and not cache
                   GSM_READ_ONLY - Do not write
                   GSM_WRITE_OFFSET - Write offset and not value
                   GSM_FORCE_WRITE - Force the write
  @param value:    Value to program
  @param *minVal:  Current minimum control delay
  @param *maxVal:  Current maximum control delay

  @retval status
**/
MRC_STATUS
GetSetCtlGroupDelayCore (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value,
  UINT16    *minVal,
  UINT16    *maxVal
  )
{
  UINT8                   rank;
  UINT16                  curVal;
  UINT8                   rcVal;
  INT16                   tmp = 0;
  struct socketNvram      *socketNvram;
  struct dimmNvram        (*dimmNvList)[MAX_DIMM];
  MRC_STATUS              status = MRC_STATUS_SUCCESS;

  socketNvram = &Host->nvram.mem.socket[socket];

  if (level == LrbufLevel && Host->nvram.mem.dramType == SPD_TYPE_DDR5 && CheckCTLGroup (group)) {

    status = GetSetCsBacksideDdr5 (Host, socket, ch, dimm, 0, level, group, mode, value, minVal, maxVal);

  } else if (level == LrbufLevel) {
    dimmNvList = GetDimmNvList (Host, socket, ch);
    if ((*dimmNvList)[dimm].DcpmmPresent) {
      status = GetSetCtlDelayFnv (Host, socket, ch, dimm, mode, value, minVal, maxVal);
    } else {
      curVal = socketNvram->channelList[ch].dimmList[dimm].rcCtl;

      if (mode & GSM_READ_ONLY) {
        *value = curVal;
      } else {
        // Adjust the current CLK delay value by offset
        if (mode & GSM_WRITE_OFFSET) {
          // Get the new offset (note: can be negative)
          tmp = curVal + *value;
        } else {
          // FORCE_WRITE
          tmp = *value;
        }
        // Check range
        if (!((tmp >= 0) && (tmp <= 31))) {
          RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_71);
        }
        //
        // Write the new value
        //
        if (tmp) {
          rcVal = (UINT8) tmp | BIT7;
        } else {
          rcVal = (UINT8) tmp;
        }
        rank = 0;
        IndirectRCWrite (Host, socket, ch, dimm, rank, rcVal, 1, RDIMM_RC1x);
        IndirectRCWrite (Host, socket, ch, dimm, rank, rcVal, 1, RDIMM_RC2x);
        IndirectRCWrite (Host, socket, ch, dimm, rank, rcVal, 1, RDIMM_RC3x);
        IndirectRCWrite (Host, socket, ch, dimm, rank, rcVal, 1, RDIMM_RC4x);

        if (mode & GSM_UPDATE_CACHE) {
          socketNvram->channelList[ch].dimmList[dimm].rcCtl = (UINT8) tmp;
        }
      }
    }
    UpdateMinMaxUint (*value, minVal, maxVal);
  } else {

    status = GetSetCtlGroupDelay (Host, socket, ch, group, mode, value, minVal, maxVal);
  } // level

  return status;
} // GetSetCtlGroupDelayCore


/**
  Muli-use function to either get or set command delays based on the provided group number

  @param[in]      Host      Pointer to SYSHOST
  @param[in]      Socket    Socket number
  @param[in]      Ch        Channel number
  @param[in]      Dimm      Dimm number
  @param[in]      Level     Level of the bus
  @param[in]      Group     Group number
  @param[in]      Mode      GSM_READ_CSR - Read the data from hardware and not cache
                            GSM_READ_ONLY - Do not write
                            GSM_WRITE_OFFSET - Write offset and not value
                            GSM_FORCE_WRITE - Force the write
  @param[in, out] Value     Value to program
  @param[in, out] *MinVal   Current minimum command delay
  @param[in, out] *MaxVal   Current maximum command delay

  @retval MRC_STATUS_SUCCESS    All operations completed successfully
  @retval MRC_STATUS_FAILURE    Failure(s) occurred during operation
**/
MRC_STATUS
GetSetCmdGroupDelayCore (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Dimm,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value,
  IN OUT  UINT16    *MinVal,
  IN OUT  UINT16    *MaxVal
  )
{
  UINT8                   Rank;
  UINT16                  CurVal;
  UINT8                   RcVal;
  INT16                   Tmp = 0;
  struct socketNvram      *SocketNvram;
  struct dimmNvram        (*DimmNvList)[MAX_DIMM];
  struct channelNvram     (*ChannelNvList)[MAX_CH];
  MRC_STATUS              Status = MRC_STATUS_SUCCESS;

  SocketNvram = &Host->nvram.mem.socket[Socket];
  ChannelNvList = GetChannelNvList (Host, Socket);
  DimmNvList = GetDimmNvList(Host, Socket, Ch);

  if (Level == LrbufLevel && Host->nvram.mem.dramType == SPD_TYPE_DDR5 && CheckCMDGroup (Group)) {

    Status = GetSetCaBacksideDdr5 (Host, Socket, Ch, Dimm, 0, Level, Group, Mode, Value, MinVal, MaxVal);

  } else if (Group == CmdVref) {

    Status = GetSetCmdVrefCore (Host, Socket, Ch, Dimm, Level, Mode, Value);

  } else if (Level == LrbufLevel) {

    if ((*DimmNvList)[Dimm].DcpmmPresent) {

      Status = GetSetCmdGroupDelayFnv (Host, Socket, Ch, Dimm, Group, Mode, Value, MinVal, MaxVal);

    } else {

      CurVal = SocketNvram->channelList[Ch].dimmList[Dimm].rcCmd;

      if (Mode & GSM_READ_ONLY) {
        *Value = CurVal;
      } else {

        // Adjust the current CLK delay value by offset
        if (Mode & GSM_WRITE_OFFSET) {

          // Get the new offset (note: can be negative)
          Tmp = CurVal + *Value;

        } else {

          // FORCE_WRITE
          Tmp = *Value;

        }
        // Check range
        if (!((Tmp >= 0) && (Tmp <= 31))) {
          RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_72);
        }

        //
        // Write the new value
        //
        if (Tmp) {
          RcVal = (UINT8) (Tmp | BIT7);
        } else {
          RcVal = (UINT8) Tmp;
        }
        Rank = 0;
        IndirectRCWrite (Host, Socket, Ch, Dimm, Rank, RcVal, 1, RDIMM_RC5x);

        //
        // program Cbit delay only for 3DS DIMMs
        //
        if ((*ChannelNvList)[Ch].encodedCSMode == 2) {
          IndirectRCWrite (Host, Socket, Ch, Dimm, Rank, RcVal, 1, RDIMM_RC2x);
        }

        if (Mode & GSM_UPDATE_CACHE) {
          SocketNvram->channelList[Ch].dimmList[Dimm].rcCmd = (UINT8) Tmp;
        }
      }
      UpdateMinMaxUint (*Value, MinVal, MaxVal);
    }

  } else {
    //
    // Don't Skip MA14, MA15, MA16 if you are DDR4
    //
    if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
        Mode = Mode & (~GSM_SKIP_CMD_FUB);
    }
    Status = GetSetCmdGroupDelay (Host, Socket, Ch, Group, Mode, Value, MinVal, MaxVal);
  } // Level

  return Status;
} // GetSetCmdGroupDelayCore


/**
  Muli-use function to either get or set clock delays based on the provided clock number

  @param[in]      Host      Pointer to SYSHOST
  @param[in]      Socket    Socket number
  @param[in]      Ch        Channel number
  @param[in]      Dimm      Dimm number
  @param[in]      Level     Level of the bus
  @param[in]      Clk       Clock number
  @param[in]      Mode      GSM_READ_CSR - Read the data from hardware and not cache
                            GSM_READ_ONLY - Do not write
                            GSM_WRITE_OFFSET - Write offset and not value
                            GSM_FORCE_WRITE - Force the write
  @param[in, out] Value     Value to program

  @retval MRC_STATUS_SUCCESS    All operations completed successfully
  @retval MRC_STATUS_FAILURE    Failure(s) occurred during operation
**/
MRC_STATUS
GetSetClkDelayCore (
  IN     PSYSHOST Host,
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Dimm,
  IN     MRC_LT   Level,
  IN     UINT8    Clk,
  IN     UINT8    Mode,
  IN OUT INT16    *Value
  )
{
  UINT8                   Rank;
  UINT16                  CurVal;
  INT16                   Tmp;
  UINT8                   RcVal;
  struct socketNvram      *SocketNvram;
  struct dimmNvram        (*DimmNvList)[MAX_DIMM];
  MRC_STATUS              Status = MRC_STATUS_SUCCESS;

  SocketNvram = &Host->nvram.mem.socket[Socket];

  if (Level == LrbufLevel) {
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    if ((*DimmNvList)[Dimm].DcpmmPresent) {
      Status = GetSetClkDelayFnv (Host, Socket, Ch, Dimm, Mode, Value);
    } else {
      CurVal = SocketNvram->channelList[Ch].dimmList[Dimm].rcClk;

      if (Mode & GSM_READ_ONLY) {
        *Value = CurVal;
      } else {
        // Adjust the current CLK delay value by offset
        if (Mode & GSM_WRITE_OFFSET) {
          // Get the new offset (note: can be negative)
          Tmp = CurVal + *Value;
        } else {
          // FORCE_WRITE
          Tmp = *Value;
        }
        // Check range
        if (!((Tmp >= 0) && (Tmp <= 31))) {
          RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_73);
        }
        //
        // Write the new value
        //
        if (Tmp) {
          RcVal = (UINT8) Tmp | BIT7;
        } else {
          RcVal = (UINT8) Tmp;
        }
        Rank = 0;
        IndirectRCWrite (Host, Socket, Ch, Dimm, Rank, RcVal, 1, RDIMM_RC6x);
        IndirectRCWrite (Host, Socket, Ch, Dimm, Rank, RcVal, 1, RDIMM_RC7x);
        IndirectRCWrite (Host, Socket, Ch, Dimm, Rank, RcVal, 1, RDIMM_RC8x);
        IndirectRCWrite (Host, Socket, Ch, Dimm, Rank, RcVal, 1, RDIMM_RC9x);

        if (Mode & GSM_UPDATE_CACHE) {
          SocketNvram->channelList[Ch].dimmList[Dimm].rcClk = (UINT8) Tmp;
        }
      }
    }
  } else {

    Status = GetSetClkDelay (Host, Socket, Ch, Clk, Mode, Value);
  } // Level

  return Status;
} // GetSetClkDelayCore

/**
This function accesses the IO group setting as directed by the mode parameter. The access
is based on the MemSs, Ch, Dimm, Rank, Strobe and Bit parameters.

  @param Host:        Pointer to SYSHOST
  @param Socket:      Socket number
  @param MemSs:       MemSS number within the socket (0-based)
  @param Channel:     Channel number within the MemSS (0-based)
  @param Dimm:        DIMM number within a channel (0-based, farthest slot == 0)
  @param Rank:        Logical rank number within a DIMM (0-based)
  @param Strobe:      Dqs data group within the rank
  @param Bit:         Bit index within the data group
  @param FreqIndex:   Index to designate which operating frequency, for multi-freq support
  @param Level:       IO level to access
  @param Group:       IO group to access
  @param Mode:        Bit-field of different modes
  @param *Value:      Pointer to value or offset based on mode

@retval MRC_STATUS
**/
MRC_STATUS
GetSetIoGroup (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     MemSs,
  IN     UINT8     Channel,
  IN     UINT8     Dimm,
  IN     UINT8     Rank,
  IN     UINT8     Strobe,
  IN     UINT8     Bit,
  IN     UINT8     FreqIndex,
  IN     MRC_LT    Level,
  IN     MRC_GT    Group,
  IN     UINT8     Mode,
  IN OUT INT16     *Value
  )
{
  MRC_STATUS           Status = MRC_STATUS_SUCCESS;
  UINT16               cmdMinVal = MAX_CMD_MARGIN;
  UINT16               cmdMaxVal = 0;
  if (CheckCMDGroup (Group)) {
    Status = GetSetCmdGroupDelayCore (Host, Socket, Channel, Dimm, Level, Group, Mode, Value, &cmdMinVal, &cmdMaxVal);
  } else if (CheckCTLGroup (Group)) {
    Status = GetSetCtlGroupDelayCore (Host, Socket, Channel, Dimm, Level, Group, Mode, Value, &cmdMinVal, &cmdMaxVal);
  } else {
    Status = GetSetDataGroup (Host, Socket, Channel, NO_SUBCH, Dimm, Rank, Strobe, Bit, Level, Group, Mode, Value);
  }
  return Status;
}

/**
  Muli-use function to either get or set command vref

  @param Host:     Pointer to SYSHOST
  @param socket:   Socket number
  @param ch:       Channel number
  @param dimm:     Dimm number
  @param level:    Level of the bus
  @param mode:     GSM_READ_CSR - Read the data from hardware and not cache
                   GSM_READ_ONLY - Do not write
                   GSM_WRITE_OFFSET - Write offset and not value
                   GSM_FORCE_WRITE - Force the write
  @param value:    Value to program or offset

  @retval status
**/
MRC_STATUS
GetSetCmdVrefCore (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  MRC_LT    level,
  UINT8     mode,
  INT16     *value
  )
{
  INT16                   curVal;
  UINT8                   controlWordData;
  UINT8                   vrefOffset = 0;
  INT16                   tmp;
  struct socketNvram      *socketNvram;
  struct dimmNvram        (*dimmNvList)[MAX_DIMM];
  MRC_STATUS              status = MRC_STATUS_SUCCESS;
  BOOLEAN                 RcdInternalVrefCa;

  socketNvram = &Host->nvram.mem.socket[socket];
  dimmNvList = GetDimmNvList (Host, socket, ch);

  RcdInternalVrefCa = InternalRcdVrefCaEnabled ();

  if (!((*dimmNvList)[dimm].DcpmmPresent) && (level == LrbufLevel || (level == DdrLevel && RcdInternalVrefCa && (Host->nvram.mem.dimmTypePresent == RDIMM)))) {

    curVal = (socketNvram->channelList[ch].dimmList[dimm].rcVref >> (8 * level)) & 0xff;
    if (curVal & BIT5) {
      curVal = (curVal & 0x1f) - 32;
    }

    if (mode & GSM_READ_ONLY) {
      *value = curVal;
    } else {
      // Adjust the current CLK delay value by offset
      if (mode & GSM_WRITE_OFFSET) {
        // Get the new offset (note: can be negative)
        tmp = curVal + *value;
      } else {
        // FORCE_WRITE
        tmp = *value;
      }

      // Check range
      if ((tmp >= -20) && (tmp <= 20)) {
        vrefOffset = (UINT8) tmp & 0x3F;
      } else {
        RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_74);
      }

      //
      // Write the new value
      //
      controlWordData = 0;
      if (!RcdInternalVrefCa) {
        controlWordData = RC0B_INPUT_VREF_SOURCE_EXTERNAL;
      }

      if (level == LrbufLevel) {
        controlWordData &= (RC0B_VDD_MASK | RC0B_INPUT_VREF_SOURCE_MASK);
        controlWordData |= (RC0B_QVREFCA_BVREFCA_OPTION2); // Internally generated Vref connected to QVrefCa and Vdd/2 connected to BVrefCA
      }
      WriteRC (Host, socket, ch, dimm, 0, controlWordData, RDIMM_RC0B);
      WriteRC (Host, socket, ch, dimm, 0, vrefOffset, RDIMM_RC1x);

      if (mode & GSM_UPDATE_CACHE) {
        socketNvram->channelList[ch].dimmList[dimm].rcVref &= ~(0xff << (8 * level));
        socketNvram->channelList[ch].dimmList[dimm].rcVref |= (vrefOffset << (8 * level));
      }
    }
  } else {

    status = GetSetCmdVref (Host, socket, ch, dimm, level, mode, value);
  } // level

  return status;
} // GetSetCmdVrefCore

/**
  Function to shift backside Cmd relative to Clk & Ctl to maintain Cmd - Clk = trained offset
  and shift TxDq and TxDqs by +/- 62 depending on direction

  @param[in] Host       Pointer to SYSHOST
  @param[in] Socket     Socket number
  @param[in] Direction  +/- direction to move timings

  @retval SUCCESS
**/
UINT32
BacksideShift (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  INT8      Direction
  )
{
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               Strobe;
  INT16               ClkCtlPiDelay;
  INT16               CmdPiDelay;
  INT16               PiDelay2;
  UINT16              MinVal = 0xFF;
  UINT16              MaxVal = 0;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  UINT8               Ddr4Present = 0;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      if ((*DimmNvList)[Dimm].DcpmmPresent) {
        continue;
      }

      for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
      }

      Ddr4Present |= 1 << Ch;
      break;
    }
  }

  if (!Ddr4Present) {
    return SUCCESS;
  }

  RcDebugPrint (SDBG_MAX, "\n");
  if (Direction == DENORMALIZE) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "DENORMALIZE: Push out backside Cmd, Ctl, Clk by trained value, to maintain Cmd - Clk = trained value \n");
  } else {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "RENORMALIZE: Pull in backside Cmd, Ctl, Clk by trained value, to maintain Cmd - Clk = trained value\n");
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      if ((*DimmNvList)[Dimm].DcpmmPresent) {
        continue;
      }

      //
      // Algorithm used to maintain the CLK/CTL and CMD relationship (Cmd - Clk = trained offset)
      // When denormalizing, set the larger of ClkCtl or Cmd to the max limit of the backside delay.
      // Because the unit(1/128 tCk) of the host side delay "PiDelay2" is half of the backside (1/64 tCK),
      // thus we need to double the "PiDelay2" value.
      //
      if ((*DimmNvList)[Dimm].QxCAClkOffset < 0) {
        //
        // Trained Value = Negative
        //
        if (Direction == DENORMALIZE) {
          ClkCtlPiDelay = MAX_BACKSIDE_CTL_CMD_DELAY;
          CmdPiDelay = MAX_BACKSIDE_CTL_CMD_DELAY + (*DimmNvList)[Dimm].QxCAClkOffset;
          PiDelay2 = MAX_BACKSIDE_CTL_CMD_DELAY * BACKSIDE_TO_HOST_TCK_DELAY_RATIO;
        } else {
          ClkCtlPiDelay = - (*DimmNvList)[Dimm].QxCAClkOffset;
          CmdPiDelay = 0;
          PiDelay2 = -MAX_BACKSIDE_CTL_CMD_DELAY * BACKSIDE_TO_HOST_TCK_DELAY_RATIO;
        }
      } else {
        //
        // Trained Value = Positive
        //
        if (Direction == DENORMALIZE) {
          ClkCtlPiDelay = MAX_BACKSIDE_CTL_CMD_DELAY - (*DimmNvList)[Dimm].QxCAClkOffset;
          CmdPiDelay = MAX_BACKSIDE_CTL_CMD_DELAY;
          PiDelay2 = (MAX_BACKSIDE_CTL_CMD_DELAY - (*DimmNvList)[Dimm].QxCAClkOffset) * BACKSIDE_TO_HOST_TCK_DELAY_RATIO;
        } else {
          ClkCtlPiDelay = 0;
          CmdPiDelay = (*DimmNvList)[Dimm].QxCAClkOffset;
          PiDelay2 = -(MAX_BACKSIDE_CTL_CMD_DELAY - (*DimmNvList)[Dimm].QxCAClkOffset) * BACKSIDE_TO_HOST_TCK_DELAY_RATIO;
        }
      }

      //
      // Program calculated delays for Clk, Ctl, and Cmd
      //
      GetSetClkDelayCore (Host, Socket, Ch, Dimm, LrbufLevel, 0, GSM_FORCE_WRITE| GSM_UPDATE_CACHE, &ClkCtlPiDelay);
      GetSetCtlGroupDelayCore (Host, Socket, Ch, Dimm, LrbufLevel, CtlAll, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &ClkCtlPiDelay, &MinVal, &MaxVal);
      GetSetCmdGroupDelayCore (Host, Socket, Ch, Dimm, LrbufLevel, CmdAll, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &CmdPiDelay, &MinVal, &MaxVal);

      //
      // Since Clk, Ctl, and Cmd have been delayed, Data also needs to be delayed accordingly so it arrives at the correct cycle
      //
      for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        //
        // Shift data delays
        //
        for (Strobe = 0; Strobe < MSVx4; Strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if (IsStrobeNotValid (Host, Strobe)) {
            continue;
          }
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, (INT16 *)&PiDelay2);
          GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, Strobe, 0, DdrLevel, TxDqDelay, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, (INT16 *)&PiDelay2);
        } // Strobe loop

      } // Rank loop
    } // Dimm loop
  } // Ch loop

  IO_Reset (Host, Socket);
  JedecInitSequence (Host, Socket, Ddr4Present);

  //
  // Re-run Receive Enable training (messes up LRDIMMs when normalizing cmd/ctl)
  // Since have moved TxDq and TxDqs, must re-run receive enable
  //
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "Re-run Receive Enable training\n");
#ifdef DDR5_SUPPORT
  if (IsDdr5Present (Host, Socket)) {
    ReceiveEnableDdr5 (Host);
  } else
#endif //DDR5_SUPPORT
  {
    ReceiveEnable(Host);
  }
  RoundTripOptimize (Host);

  return SUCCESS;
} // BacksideShift

/**
  Function to shift backside Cmd, Ctl, Clk by +/- 31 PI ticks

  @param[in] Host       Pointer to SYSHOST
  @param[in] Socket     Socket number
  @param[in] Direction  +/- direction to move timings

  @retval SUCCESS
**/
UINT32
EarlyBacksideShift (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  INT8      Direction
  )
{
  UINT8               Ch;
  UINT8               Dimm;
  INT16               PiDelay;
  UINT16              MinVal = 0xFF;
  UINT16              MaxVal = 0;
  UINT32              PrevDebugLevel;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  UINT8               Ddr4Present = 0;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }
      if ((*DimmNvList)[Dimm].DcpmmPresent) {
        continue;
      }

      Ddr4Present |= 1 << Ch;
      break;
    }
  }

  if (!Ddr4Present) {
    return SUCCESS;
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    if (Direction == DENORMALIZE) {
      PiDelay = 31;
    } else {
      PiDelay = 0;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }
      if ((*DimmNvList)[Dimm].DcpmmPresent) {
        continue;
      }
      GetSetClkDelayCore (Host, Socket, Ch, Dimm, LrbufLevel, 0, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &PiDelay);
      GetSetCtlGroupDelayCore (Host, Socket, Ch, Dimm, LrbufLevel, CtlAll, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &PiDelay, &MinVal, &MaxVal);
    } // Dimm loop
  } // Ch loop

  PrevDebugLevel = GetDebugLevel ();
  if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
    SetDebugLevel (SDBG_ERROR);
  }
  JedecInitSequence (Host, Socket, Ddr4Present);
  SetDebugLevel (PrevDebugLevel);

  return SUCCESS;
} // EarlyBacksideShift

/**

  Turns on the MPR feature for read training

  @param Host  - Pointer to sysHost

  @retval N/A

**/
UINT32
EnableMPR (
  PSYSHOST Host
  )
{

  SetUbiosOutputMode (ASM_OUTPUT_DISABLE);

  //
  // Turn on MPR pattern for all ranks
  //
  SetAllRankMPR (Host, Host->var.mem.currentSocket, MR3_MPR);

  SetUbiosOutputMode (ASM_OUTPUT_ENABLE);

  return SUCCESS;
} // EnableMPR

/**

  Turns off the MPR feature for read training

  @param Host  - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
DisableMPR (
  PSYSHOST Host
  )
{
  //
  // Turn off MPR pattern for all ranks
  //
  SetAllRankMPR (Host, Host->var.mem.currentSocket, 0);

  return SUCCESS;
} // DisableMPR

/**

  Starts the JEDEC initialization sequence for DDR4

  @param Host    - Pointer to sysHost
  @param socket  - Socket to initizlize
  @param chBitMask - Bit Mask of Channels Present on the Socket

  @retval SUCCESS

**/
UINT32
JedecInitSequenceDdr4 (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    chBitMask
  )
{
  // PLL lock workaround
  UINT8               ch;
  UINT8               dimm;
  UINT32              lrbufData = 0;
  UINT8               errorcnt[MAX_CH][MAX_DIMM];
  UINT8               passFlag;
  UINT8               errorFlag;
  UINT32              PrevDebugLevel;
  UINT32              DaTrainingEnOrg[MAX_CH];
  UINT32              DTrainingEnOrg[MAX_CH];
  UINT32              HsioTrainingEnOrg[MAX_CH];
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  BOOLEAN             BpsIsFound = FALSE;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  PrevDebugLevel = GetDebugLevel ();

  // PLL lock workaround
  channelNvList = GetChannelNvList (Host, socket);

  //
  // Save Original DCPMM training mode for BWV
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    DaTrainingEnOrg[ch] = 0;
    DTrainingEnOrg[ch] = 0;
    HsioTrainingEnOrg[ch] = 0;

    if (GetFmcType (socket, ch) == FMC_BWV_TYPE) {
      GetRestoreAepTrainingModeSMB (Host, socket, ch, TRUE, &DaTrainingEnOrg[ch], &DTrainingEnOrg[ch], NULL);
      BpsIsFound = TRUE;
    } else if (GetFmcType (socket, ch) == FMC_CWV_TYPE) {
      GetRestoreAepTrainingModeSMB (Host, socket, ch, TRUE, &DaTrainingEnOrg[ch], &DTrainingEnOrg[ch], &HsioTrainingEnOrg[ch]);
      BpsIsFound = TRUE;
    }
  }

  //
  // Disale DCPMM training step for BWV
  //
  if (BpsIsFound) {
    SetAepTrainingModeSMB (Host,socket, DISABLE_TRAINING_STEP);
  }

  //
  // PLL lock workaround
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    ZeroMem (errorcnt[ch], MAX_DIMM);
  } // ch loop

  errorFlag = 0;
  passFlag = 1;

  while (passFlag) {
    passFlag = 0;

    //
    // Issue reset to the channels
    //
    ResetAllDdrChannels (Host, socket, chBitMask);

    if ((Host->nvram.mem.dimmTypePresent == RDIMM) && (Host->nvram.mem.dramType == SPD_TYPE_DDR4)) { //RDIMM and DDR4
      for (ch = 0; ch < MaxChDdr; ch++) {
        //Check if this channel has been masked off as each BSSA Call is per MC
        if ((chBitMask & (1 << ch)) == 0) {
          continue;
        }
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

          if (((*dimmNvList)[dimm].SPDRegVen == MFGID_IDT) &&
              ((*dimmNvList)[dimm].SPDRegRev == IDT_SPD_REGREV_B0)) {

            ReadLrbufSmb (Host, socket, ch, dimm, 0x1010, &lrbufData);
            if ((((lrbufData >> 9) & 0x1) != 1) || (((lrbufData >> 12) & 0x1) != 0)) {
              errorcnt[ch][dimm] += 1;
              if (errorcnt[ch][dimm] >= 5) {
                errorFlag = 1;
              }
              passFlag = 1;
            }
          } // IDT check
        } // dimm loop
      } // ch loop
    } // RDIMM/DDR4 check

    if (errorFlag) {
      passFlag = 0;
    } // if PLL lock errors
  } // while loop

  for (ch = 0; ch < MaxChDdr; ch++) {
    //Check if this channel has been masked off as each BSSA Call is per MC
    if ((chBitMask & (1 << ch)) == 0) {
      continue;
    }
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    dimmNvList = GetDimmNvList (Host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }

      if (errorcnt[ch][dimm]) {
        SetDebugLevel (PrevDebugLevel);
        RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "# PLL Lock errors=%d\n", errorcnt[ch][dimm]);
      }
    } // dimm loop
  } // ch loop

  if (errorFlag) {
    RcDebugPrint (SDBG_MINMAX, "PLL lock errors\n");
    OutputError (ERR_VENDOR_SPECIFIC, ERR_DIMM_PLL_LOCK_ERROR, socket, NO_CH, NO_DIMM, NO_RANK);
  } // if PLL lock errors

  // PLL lock workaround

  if (((GetSysBootMode () == NormalBoot) && (Host->var.mem.subBootMode != WarmBootFast))) {
    //
    // Initiate JEDEC init
    //
    JedecInitDdrAll (Host, socket, chBitMask);
  }

  // PPR_SUPPORT
  //
  // If soft PPR has already been applied, we need to re-run
  // it every time JedecInitSequence is executed
  //

  Host->var.mem.notRunningFromCT = 1; //Set when not calling from Call Table
  Host->var.mem.chBitMask = chBitMask;

  if (Host->var.mem.softPprDone[socket]) {
    PostPackageRepairMain (Host);
  }

  Host->var.mem.notRunningFromCT = 0; //Clear value

  // End PPR_SUPPORT

  //
  // Restore DCPMM Training mode setting for BWV
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    if (GetFmcType (socket, ch) == FMC_BWV_TYPE) {
      GetRestoreAepTrainingModeSMB (Host, socket, ch, FALSE, &DaTrainingEnOrg[ch], &DTrainingEnOrg[ch], NULL);
    } else if (GetFmcType (socket, ch) == FMC_CWV_TYPE) {
      GetRestoreAepTrainingModeSMB (Host, socket, ch, FALSE, &DaTrainingEnOrg[ch], &DTrainingEnOrg[ch], &HsioTrainingEnOrg[ch]);
    }
  }

  //
  // One RCOMP cycle after every JEDEC Init
  //
  DoCompDdrioPerSetupOption (socket, TRAINING_COMP_EVERY_JEDEC_INIT);

  return SUCCESS;
} // JedecInitSequenceDdr4

/**

  Executes the JEDEC initialization sequence for each channel on the provided socket number

  @param Host      - Pointer to sysHost
  @param socket    - Socket Id
  @param chBitMask - Bit Mask of Channels Present on the Socket

  @retval N/A

**/
VOID
JedecInitDdrAllDdr4 (
  PSYSHOST      Host,
  UINT8         socket,
  UINT32        chBitMask
  )
{
  UINT8               ch;
  struct channelNvram (*channelNvList)[MAX_CH];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  RcDebugPrint (SDBG_MAX,
                "JEDEC Init\n");
  channelNvList = GetChannelNvList (Host, socket);

  //
  // Update channels
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    //Check if this channel has been masked off as each BSSA Call is per MC
    if ((chBitMask & (1 << ch)) == 0) {
      continue;
    }
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    //
    // Reset and JEDEC init
    //
    JedecInitDdrChannel (Host, socket, ch);
  } // ch
} // JedecInitDdrAllDdr4

/**

  Executes the JEDEC initialization sequence for the given channel

  @param Host  - Pointer to sysHost
  @param socket  - Socket Id
  @param ch    - Channel number (0-based)

  @retval N/A

**/
VOID
JedecInitDdrChannelDdr4 (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  )
{
  UINT8   dimm;
  UINT8   rank;
  UINT8   ckeMask = 0;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  UINT8  cmd_stretch;
  BootMode SysBootMode;
  UINT16 dateCode;

  SysBootMode = GetSysBootMode ();
  channelNvList = GetChannelNvList (Host, socket);
  dimmNvList = GetDimmNvList (Host, socket, ch);

  //
  // Set 3N timing and save original setting
  //
  cmd_stretch = GetCurrentTimingMode (Host, socket, ch);
  {
    SetTimingMode (Host, socket, ch, (*channelNvList)[ch].trainTimingMode);
  }

  //
  // Initialize the register if this is an RDIMM
  //
  if (Host->nvram.mem.dimmTypePresent == RDIMM) {
    //
    // First pass with CKE low
    //
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      if (CheckRank (Host, socket, ch, dimm, 0, DONT_CHECK_MAPOUT)) {
        continue;
      }
      if (CheckS3Jedec (Host, socket, ch, dimm) == 0) {
        continue;
      }
      if ((*dimmNvList)[dimm].DcpmmPresent) {
        if (GetSysBootMode() != S3Resume) {
          DdrtBufferReset (Host, socket, ch, dimm);
        }
        if (dimm == 0) {
          ckeMask |= BIT0;
        }
        else {
          ckeMask |= BIT4;
        }
      } else {
        if (Host->nvram.mem.dramType == SPD_TYPE_DDR4) {
          DoRegisterInitDDR4 (Host, socket, ch, dimm, 0);
        }
      }
    } // dimm loop

    //
    // Take CKE high to all slots
    //
    FixedDelayMicroSecond (500);

    // Assert CKE
    if (SysBootMode == NormalBoot) {
      SetCkeOverride (Host, socket, ch, (*channelNvList)[ch].ckeMask);
    } else {
      OverrideCke (socket, ch, CKE_OVERRIDE_ALL_RANK, (UINT32)ckeMask);
    }
    FixedDelayMicroSecond (1);
    //
    // FMC CSR unlock sequence
    //
    FmcCsrUnlockCh (socket, ch);
    //
    // Second pass with CKE high
    //
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      if (CheckRank (Host, socket, ch, dimm, 0, DONT_CHECK_MAPOUT)) {
        continue;
      }
      if (CheckS3Jedec (Host, socket, ch, dimm) == 0) {
        continue;
      }
      if (!(*dimmNvList)[dimm].DcpmmPresent) {
        if (Host->nvram.mem.dramType == SPD_TYPE_DDR4) {
          DoRegisterInitDDR4_CKE (Host, socket, ch, dimm, 0);
        }
      } // NVMDIMM
    } // dimm loop

    SetEncodedCsMode (Host, socket, ch);
  } // if RDIMM

  if (!(UbiosGenerationOrHsleEnabled ())) {
    // Hynix caseID(RMA): R14D035 - Start
    // If Hynix DIMM is present, issue a PREA
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      if (CheckS3Jedec (Host, socket, ch, dimm) == 0) {
        continue;
      }
      dateCode = (((*dimmNvList)[dimm].SPDModDate >> 8) & 0xFF) | (((*dimmNvList)[dimm].SPDModDate << 8) & 0xFF00);
      if ((Host->nvram.mem.dramType == SPD_TYPE_DDR4) && ((*dimmNvList)[dimm].SPDDramMfgId == MFGID_HYNIX)
          && (dateCode < 0x1545) && ((*dimmNvList)[dimm].sdramCapacity == SPD_4Gb) && ((*dimmNvList)[dimm].SPDModPartDDR4[8] == 0x4D)) {

        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                       "Hynix W/A On\n");
        //
        // Loop for each rank
        //
        for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
          //
          // Skip if no rank
          //
          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          //
          // Setup the test for a precharge
          // 4987528: Reset noise sensitivity on RDIMM (post register) may cause boot failure
          //
          WriteCADBCmd (Host, socket, ch, dimm, rank, PRE_CMD, 0, 0x400, 0);
        } //rank
      } //Hynix DDR4
    } //dimm
    // Hynix caseID(RMA): R14D035 - End
  }
  //
  // Loop for each DIMM
  //
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    //
    // Skip if no DIMM present
    //
    if ((*dimmNvList)[dimm].dimmPresent == 0) {
      continue;
    }

    //
    // Loop for each rank
    //
    for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
      //
      // Skip if no rank
      //
      if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
        continue;
      }
      if (CheckS3Jedec (Host, socket, ch, dimm) == 0) {
        continue;
      }

#ifdef DDRT_SUPPORT
      if ((*dimmNvList)[dimm].DcpmmPresent && ((*dimmNvList)[dimm].fmcType == FMC_EKV_TYPE || (*dimmNvList)[dimm].fmcType == FMC_BWV_TYPE)) {
        ProgramMRSAEP (Host, socket, ch, dimm, rank);
        // FatalError
      }
#endif // DDRT_SUPPORT
      if ((Host->nvram.mem.dramType == SPD_TYPE_DDR4) && ((*dimmNvList)[dimm].DcpmmPresent == 0)) {
        ProgramMRSDDR4 (Host, socket, ch, dimm, rank);
      }
    } // rank loop
  } // dimm loop
#ifdef DDR5_SUPPORT
  if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
    ProgramCasLDdr5 (Host, socket, ch);
  }
#endif //DDR5_SUPPORT
  //
  // Issue ZQCAL. DDRT Cosim/Cte may execute in 1n causing
  // parity issue. ICX_TODO clean CABD ZQCal for 2 cycle.
  //
  if (FeaturePcdGet (PcdCosimBuild) == FALSE) {
    DoZQ (Host, socket, ch, ZQ_LONG);
  }

#ifdef  LRDIMM_SUPPORT
  //
  // LRDIMM buffer init
  //
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    if ((*dimmNvList)[dimm].dimmPresent == 0) {
      continue;
    }

    if (IsLrdimmPresent (socket, ch, dimm)) {
      if (CheckS3Jedec (Host, socket, ch, dimm) == 0) {
        continue;
      }
      DoLrbufInitDDR4 (Host, socket, ch, dimm, 0);
    }
  } //dimm loop

  //
  // Restore Data Buffer DFE Coef setting
  //
  RestoreDBDfeCoef (Host, ch);
#endif
  //
  // Restore original CMD timing
  //
  SetTimingMode (Host, socket, ch, cmd_stretch);

  if (Host->nvram.mem.socket[socket].TrainingStepDone.TxVref == 1) {

    FixedDelayMicroSecond (1);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      //
      // Skip if no DIMM present
      //
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      if (CheckS3Jedec (Host, socket, ch, dimm) == 0) {
        continue;
      }
      //
      // Loop for each rank
      //
      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        //
        // Skip if no rank
        //
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        if ((rank > 0) & (IsLrdimmPresent (socket, ch, dimm))) {
          continue;
        } // only do rank = 0 for lrdimm

        RestoreOffset (Host, socket, ch, dimm, rank, 0, DdrLevel, TxVref);
      } // rank loop
    } // dimm loop

  }

} // JedecInitDdrChannelDdr4

/**

  Starts the JEDEC initialization sequence

  @param Host    - Pointer to sysHost
  @param socket  - Socket to initizlize
  @param chBitMask - Bit Mask of Channels Present on the Socket

  @retval SUCCESS

**/
UINT32
JedecInitSequence (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT32    ChBitMask
  )
{
  UINT32    Status;

  if (IsDdr5Present(Host, Socket)) {
    Status = FAILURE;
    if (JedecInitSequenceDdr5 (Host, Socket, ChBitMask) == MRC_STATUS_SUCCESS) {
      Status = SUCCESS;
    }
  } else {
    Status = JedecInitSequenceDdr4 (Host, Socket, ChBitMask);
  }
  return Status;
} // JedecInitSequence

/**

  Executes the JEDEC initialization sequence for each channel on the provided socket number

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket Id
  @param[in] ChBitMask - Bit Mask of Channels Present on the Socket

  @retval N/A

**/
VOID
JedecInitDdrAll (
  IN  PSYSHOST      Host,
  IN  UINT8         Socket,
  IN  UINT32        ChBitMask
  )
{
  if (IsDdr5Present (Host, Socket)) {
    JedecInitDdrAllDdr5 (Host, Socket, ChBitMask);
  } else {
    JedecInitDdrAllDdr4 (Host, Socket, ChBitMask);
  }
}

/**

  Executes the JEDEC initialization sequence for the given channel

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket Id
  @param[in] Ch       - Channel number (0-based)

  @retval N/A

**/
VOID
JedecInitDdrChannel (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Ch
  )
{
  if (IsDdr5Present (Host, Socket)) {
    JedecInitDdrChannelDdr5 (Host, Socket, Ch);
  } else {
    JedecInitDdrChannelDdr4 (Host, Socket, Ch);
  }
}

//
// Table used to convert tRTP or tWR to MR0 field encoding
// Assume that tWR = 2 * tRTP
//
#define TWR_TRTP_ENTRY_SIZE   2

UINT16 Mr0TwrTrtp_LookupTable[][TWR_TRTP_ENTRY_SIZE] = {
  {5, 0x0000},  // 10 clocks for tWR, 5 for tRTP
  {6, 0x0200},  // 12 clocks for tWR, 6 for tRTP
  {7, 0x0400},  // 14 clocks for tWR, 7 for tRTP
  {8, 0x0600},  // 16 clocks for tWR, 8 for tRTP
  {9, 0x0800},  // 18 clocks for tWR, 9 for tRTP
  {10, 0x0A00},  // 20 clocks for tWR, 10 for tRTP
  {11, 0x0E00},  // 22 clocks for tWR, 11 for tRTP
  {12, 0x0C00},  // 24 clocks for tWR, 12 for tRTP
  {13, 0x2000}   // 26 clocks for tWR, 13 for tRTP
};

/**

  Encodes tRTP/tWR value to MR0 field

  @param Value - tRTP/tWR value in tCK units
  @param Twr   - FALSE = tRTP; TRUE = tWR
  @param *Mr0Mask  - MR0 bit mask of tRTP/tWR field
  @param *Mr0Value - MR0 encoded value of tRTP/tWR field

  @retval SUCCESS/FAILURE

**/
UINT32
EncodeMr0TrtpTwrDdr4 (
  UINT8     Value,
  BOOLEAN   Twr,
  UINT16    *Mr0Mask,
  UINT16    *Mr0Value
)
{
  UINT32 Index;
  UINT32 MaxIndex;
  UINT8  Temp = Value;

  if (Twr) {
    Temp = Value >> 1;
  }
  *Mr0Mask = (BIT13 | BIT11 | BIT10 | BIT9);
  MaxIndex = sizeof (Mr0TwrTrtp_LookupTable) / (sizeof (UINT16) * TWR_TRTP_ENTRY_SIZE);
  for (Index = 0; Index < MaxIndex; Index++) {
    if (Mr0TwrTrtp_LookupTable[Index][0] >= Temp) {
      *Mr0Value = Mr0TwrTrtp_LookupTable[Index][1];
      break;
    }
  }
  if (Index == MaxIndex) {
    return FAILURE;
  }
  return SUCCESS;
}  // EncodeMr0TrtpTwrDdr4

/**

  Decodes tRTP/tWR value from MR0 field

  @param Twr      - FALSE = tRTP; TRUE = tWR
  @param Mr0Value - MR0 encoded value of tRTP/tWR field
  @param *Value   - tRTP/tWR value in tCK units

  @retval SUCCESS/FAILURE

**/
UINT32
DecodeMr0TrtpTwrDdr4 (
  BOOLEAN   Twr,
  UINT16    Mr0Value,
  UINT8     *Value
  )
{
  UINT32 Index;
  UINT32 MaxIndex;
  UINT8  Temp = 0;

  Mr0Value &= (BIT13 | BIT11 | BIT10 | BIT9);
  MaxIndex = sizeof (Mr0TwrTrtp_LookupTable) / (sizeof (UINT16) * TWR_TRTP_ENTRY_SIZE);
  for (Index = 0; Index < MaxIndex; Index++) {
    if (Mr0TwrTrtp_LookupTable[Index][1] == Mr0Value) {
      Temp = (UINT8) Mr0TwrTrtp_LookupTable[Index][0];
      break;
    }
  }
  if (Twr) {
    *Value = Temp << 1;
  } else {
    *Value = Temp;
  }
  if (Index == MaxIndex) {
    return FAILURE;
  }
  return SUCCESS;
}  // DecodeMr0TrtpTwrDdr4


// DDR4 CAS Latency Table, CL =     9,  10,   11,   12,   13,   14,   15,   16,   17,   18,
UINT16 DDR4CASLatencyTable[24] = { 0x0, 0x4, 0x10, 0x14, 0x20, 0x24, 0x30, 0x34, 0x64, 0x40,
//  19,   20,   21,   22,   23,   24,     25,     26,     27,     28      29,     30,     31,     32
  0x70, 0x44, 0x74, 0x50, 0x60, 0x54, 0x1000, 0x1004, 0x1010, 0x1014, 0x1020, 0x1024, 0x1030, 0x1034 };

/**

  Programs mode registers for DDR4 devices

  @param Host  - Pointer to sysHost
  @param socket  - Socket Id
  @param ch    - Channel number (0-based)
  @param dimm  - DIMM number (0-based)
  @param rank  - Rank number (0-based)

  @retval N/A

**/
VOID
ProgramMRSDDR4 (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank
  )
{
#ifndef DDR5_SUPPORT
  UINT8               strobe;
  UINT16              MR6;
  UINT16              Mr0Mask;
  UINT16              Mr0Value;
  UINT16              TccdlFittedForMr6;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct rankDevice   (*rankStruct)[MAX_RANK_DIMM];
  UINT16              RxEqValue = 0;

  UINT8 MSVx4 = GetMaxStrobeValid (Host);

  channelNvList = GetChannelNvList (Host, socket);
  dimmNvList = GetDimmNvList (Host, socket, ch);
  rankList = GetRankNvList (Host, socket, ch, dimm);
  rankStruct = GetRankStruct (Host, socket, ch, dimm);

  //
  // Burst Length = 8, Read Burst Type = Interleave, Normal mode, DLL reset = yes, PPD Fast Exit
  //
  (*rankStruct)[rank].MR0 = MR0_RBT + MR0_DLL;

  //
  // Set Write Recovery and Read To Precharge field in MR0
  //
  if (EncodeMr0TrtpTwrDdr4 ((*channelNvList)[ch].common.nRTP, FALSE, &Mr0Mask, &Mr0Value) == SUCCESS) {
    (*rankStruct)[rank].MR0 |= (Mr0Mask & Mr0Value);
  } else {
    (*rankStruct)[rank].MR0 |= 0xA00;
  }

  //
  // Set CAS Read latency
  //
  (*rankStruct)[rank].MR0 |= DDR4CASLatencyTable[ ((*channelNvList)[ch].common.nCL - 9) ];

  //
  // Clear Output Drive Impedance Control
  //
  (*rankStruct)[rank].MR1 &= ~MR1_RTT_NOM;

  //
  // Clear RxEq bits
  //
  (*rankStruct)[rank].MR1 &= ~MR1_RX_EQ;

  //Use Strobe 0's data for RxEq.
  RxEqValue = (UINT16) GetCachedRxEq (Host, socket, ch, rank, dimm, 0);
  //
  // Set the Rx Eq bits from the Mr1RxEqSettings struct in the Host
  //
  RxEqValue = DistributeSparse16BitField (MR1_RX_EQ, RxEqValue);

  (*rankStruct)[rank].MR1 |= RxEqValue;

  //
  // DLL enable, Additive latency = 0, Write Leveling disabled, Output Buffer Enabled, Rtt_Nom,
  // Output Driver Impedance Control = RZQ/7
  // Set RttNom bits
  //
  (*rankStruct)[rank].MR1 |= (*rankList)[rank].RttNom; // For DDR5, RTT_NOM_XX values are first cached by BufferDramAndLrdimmBacksideOdtToHost()
                                                       // and later programmed by BacksideDramInitAfterQca()

  if (IsLrdimmPresent (socket, ch, dimm)) {
#ifdef LRDIMM_SUPPORT
    if ((*dimmNvList)[dimm].SPDSpecRev > 7) {
      // LRDIMM uses SPD data for Driver Impedance Control
      (*rankStruct)[rank].MR1 &= ~MR1_RTT_NOM;
      if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_1866)  {
        (*rankStruct)[rank].MR1 |= (((*dimmNvList)[dimm].SPDLrbufDramDs & (BIT1 + BIT0)) << 1);
      } else if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_2400) {
        (*rankStruct)[rank].MR1 |= (((*dimmNvList)[dimm].SPDLrbufDramDs & (BIT3 + BIT2)) >> 1);
      } else {
        (*rankStruct)[rank].MR1 |= (((*dimmNvList)[dimm].SPDLrbufDramDs & (BIT5 + BIT4)) >> 3);
      }
    }
#endif // LRDIMM_SUPPORT
  } else {
    //
    // Output Driver Impedance Control
    //
    (*rankStruct)[rank].MR1 |= (*rankList)[rank].OutDrvImpCtrl & MR1_RTT_NOM;
  }

  //
  // DLL Enable
  //
  (*rankStruct)[rank].MR1 |= BIT0;

  //
  // Additive Latency - MR1 BIT[4:3]
  // 00b -> AL = 0(Disable), 01b -> AL = CL-1, 10b -> AL = CL-2, 11b -> Reserved
  //
  if ((*channelNvList)[ch].common.nAL == ((*channelNvList)[ch].common.nCL - 1)) {
    (*rankStruct)[rank].MR1 |= BIT3;
  } else if ((*channelNvList)[ch].common.nAL == ((*channelNvList)[ch].common.nCL - 2)) {
    (*rankStruct)[rank].MR1 |= BIT4;
  } else if ((*channelNvList)[ch].common.nAL > 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "Additive Latency invalid!\n");
  }
  //
  // Enable tDQS if x4 and x8 DIMMs are mixed on this channel and this is a x8
  //
  if (((*channelNvList)[ch].features & X4_AND_X8_PRESENT) && !((*dimmNvList)[dimm].x4Present)) {
    (*rankStruct)[rank].MR1 |= MR1_TDQS;
  }

  //
  // CAS Write latency
  //
  if ((*channelNvList)[ch].common.nWL < 14) {
    (*rankStruct)[rank].MR2 = ((*channelNvList)[ch].common.nWL - 9) << 3;
  } else {
    (*rankStruct)[rank].MR2 = ((*channelNvList)[ch].common.nWL - (((*channelNvList)[ch].common.nWL - 14) / 2) - 10) << 3;
  }

  //
  // Rtt_WR
  //
  (*rankStruct)[rank].MR2 |= (*rankList)[rank].RttWr;

  //
  // All DDR4 DIMMs support ASR
  //
  (*rankStruct)[rank].MR2 |= MR2_ASR_DDR4;

  (*rankStruct)[rank].MR3 = 0;
  (*rankStruct)[rank].MR4 = 0;
  //
  // 2nCK preamble
  //
  if (GetPreambleState (socket, WRITE_PREAMBLE) == PREAMBLE_2TCLK) {
    (*rankStruct)[rank].MR4 |= BIT12;
  }
  if (GetPreambleState (socket, READ_PREAMBLE) == PREAMBLE_2TCLK) {
    (*rankStruct)[rank].MR4 |= BIT11;
  }

  (*rankStruct)[rank].MR5 = (*rankList)[rank].RttPrk;

  //
  // tCCD_L
  //
  TccdlFittedForMr6 = ((GetTccd_L (socket, ch) - 4) & 7) << 10;
  for (strobe = 0; strobe < MSVx4; strobe++) {
    (*rankStruct)[rank].MR6[strobe] |= TccdlFittedForMr6;
  } // strobe loop

  MR6 = GetVrefRange (Host, (*channelNvList)[ch].txVrefSafe[ (*rankList)[rank].rankIndex]);
#ifdef LRDIMM_SUPPORT
  if (IsLrdimmPresent (socket, ch, dimm)) {
    MR6 = (UINT8) (*rankList)[rank].lrbufTxVref[0];
  }
#endif
  MR6 |= TccdlFittedForMr6;

  MRSCmdStacking (Host, socket, ch, dimm, LOAD_MRS);
  //
  // Follow MRS programming order in the DDR4 SDRAM spec
  //
  WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR3, BANK3);
  //
  // The first write ensures VrefDQ Training Mode is disabled
  //
  WriteMRS (Host, socket, ch, dimm, rank, MR6, BANK6);
  //
  // The second write enters VrefDQ Training Mode and sets the range
  //
  WriteMRS (Host, socket, ch, dimm, rank, MR6 | BIT7, BANK6);
  //
  // The third write sets the VrefDQ value
  //
  WriteMRS (Host, socket, ch, dimm, rank, MR6 | BIT7, BANK6);
  //
  // The fourth write exits VrefDq Training Mode
  //
  WriteMRS (Host, socket, ch, dimm, rank, MR6, BANK6);
  WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR5, BANK5);
  WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR4, BANK4);
  WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR2, BANK2);
  WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR1, BANK1);
  WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR0, BANK0);
  MRSCmdStacking(Host, socket, ch, dimm, EXECUTE_MRS);   // execute stack of MRS commands and return to NORMAL_MRS

  if (Host->nvram.mem.socket[socket].TrainingStepDone.DramRxEq == 1) {
    RestoreOffset (Host, socket, ch, dimm, rank, 0, DdrLevel, DramRxEq);
  }

#endif // !DDR5_SUPPORT
} // ProgramMRSDDR4

/**

  Programs mode registers for NVMDIMM devices

  @param Host  - Pointer to sysHost
  @param socket  - Socket Id
  @param ch    - Channel number (0-based)
  @param dimm  - DIMM number (0-based)
  @param rank  - Rank number (0-based)

  @retval N/A

**/
VOID
ProgramMRSAEP (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank
  )
{
#if (defined(DDRT_SUPPORT)) && (!defined(DDR5_SUPPORT))
  UINT8                                 CASTemp;
  UINT8                                 CWLTemp;
  UINT8                                 tCLAdd;
  UINT8                                 tCWLAdd;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct ddrRank (*rankList)[MAX_RANK_DIMM];
  struct rankDevice (*rankStruct)[MAX_RANK_DIMM];
  INT16                                 zeroOffset;
  BOOLEAN                               ExecuteDelayAdjustment = FALSE;

  channelNvList = GetChannelNvList (Host, socket);
  rankList = GetRankNvList (Host, socket, ch, dimm);
  rankStruct = GetRankStruct (Host, socket, ch, dimm);

  zeroOffset = 0;
  GetSetCmdVrefCore (Host, socket, ch, dimm, DdrLevel, GSM_READ_ONLY, &zeroOffset);

  //
  // DLL reset = yes
  //
  (*rankStruct)[rank].MR0 = MR0_DLL;

  GetDdrtCasLatencies (socket, ch, dimm, &CASTemp, &tCLAdd, &CWLTemp, &tCWLAdd);

  if (FeaturePcdGet (PcdCosimBuild) == TRUE) {
    ExecuteDelayAdjustment = TRUE;
  } else {
    if (UbiosGenerationOrHsleEnabled () == FALSE) {
      ExecuteDelayAdjustment = TRUE;
    }
  }

  if (ExecuteDelayAdjustment) {
    if ((CASTemp + tCLAdd) < 12) {
      CASTemp = (12 - tCLAdd);
    }
  }

  CASTemp = CASTemp - 9;
  if (CWLTemp < 14) {
    CWLTemp = CWLTemp - 9;
  } else {
    CWLTemp = (CWLTemp - ((CWLTemp - 14) / 2) - 10);
  }
  (*rankStruct)[rank].MR0 |= DDR4CASLatencyTable[CASTemp];

  //
  // DLL enable, Additive latency = 0, Write Leveling disabled, Output Buffer Enabled, Rtt_Nom,
  // Output Driver Impedance Control = RZQ/7
  //
#ifndef DDR5_SUPPORT
  (*rankStruct)[rank].MR1 = (*rankList)[rank].RttNom; // For DDR5, RTT_NOM_XX values are first cached by BufferDramAndLrdimmBacksideOdtToHost()
                                                      // and later programmed by BacksideDramInitAfterQca()
#endif

  //
  // CAS Write latency A5:A3
  //
  if ((CWLTemp & ~0x7) != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                            "ERROR: CWL exceeds MR2 bitfield range: 0x%x\n", CWLTemp);
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_160);
  }
  (*rankStruct)[rank].MR2 = (0x38 & (CWLTemp << 3));

  //
  // Enable NVMDIMM 2N mode
  //
  if ((*channelNvList)[ch].timingMode == TIMING_2N) {
    (*rankStruct)[rank].MR2 |= BIT6;
  }
  //
  // Rtt_WR
  //
  (*rankStruct)[rank].MR2 |= (*rankList)[rank].RttWr;

  (*rankStruct)[rank].MR3 = 0;
  (*rankStruct)[rank].MR4 = 0;

  //
  // 2nCK preamble
  //
  if (GetPreambleState (socket, WRITE_PREAMBLE) == PREAMBLE_2TCLK) {
    (*rankStruct)[rank].MR4 |= BIT12;
  }
  if (GetPreambleState (socket, READ_PREAMBLE) == PREAMBLE_2TCLK) {
    (*rankStruct)[rank].MR4 |= BIT11;
  }
  (*rankStruct)[rank].MR5 = (*rankList)[rank].RttPrk;

  //
  // Follow MRS programming order in the DDR4 SDRAM spec
  //
  MRSCmdStacking (Host, socket, ch, dimm, LOAD_MRS);
  WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR3, BANK3);
  WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR5, BANK5);
  WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR4, BANK4);
  WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR2, BANK2);
  WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR1, BANK1);
  WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR0, BANK0);
  MRSCmdStacking (Host, socket, ch, dimm, EXECUTE_MRS);

  if (isCmdVrefTrainingDone (Host, socket)) {
    RestoreOffset (Host, socket, ch, dimm, rank, zeroOffset, DdrLevel, CmdVref);
  }
#endif // DDRT_SUPPORT && !DDR5_SUPPORT
} // ProgramMRSAEP

/**

  Writes DRAM mode registers

  @param Host  - Pointer to sysHost
  @param socket  - Socket Id
  @param ch    - Channel number (0-based)
  @param dimm  - DIMM number (0-based)
  @param rank  - Rank number (0-based)
  @param data  - Data for the MRS write
  @param bank  - Bank address for the MRS write

  @retval N/A

**/
VOID
WriteMRS (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    rank,
  UINT16   data,
  UINT8    bank
  )
{
  struct dimmNvram  (*dimmNvList)[MAX_DIMM];
#ifdef DDR5_SUPPORT
  if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
    return;
  }
#endif //DDR5_SUPPORT
  CountTrackingData (PTC_MRS_WRITE, 1);

  dimmNvList = GetDimmNvList (Host, socket, ch);

  SetMRSShadow (Host, socket, ch, dimm, data, bank);

  WriteMrsDdr4 (Host, socket, ch, dimm, rank, data, bank, A_SIDE);
  //
  // Write to the both sides of the DIMM if this is a buffered DIMM
  //
  if ((Host->nvram.mem.dimmTypePresent == RDIMM) && (bank != 7) &&
      !(*dimmNvList)[dimm].DcpmmPresent) {
    WriteMrsDdr4 (Host, socket, ch, dimm, rank, data, bank, B_SIDE);
  }
  return;

} // WriteMRS

/**

  Writes DRAM mode registers

  @param Host  - Pointer to sysHost
  @param socket- Socket Id
  @param ch    - Channel number (0-based)
  @param dimm  - DIMM number (0-based)
  @param rank  - Rank number (0-based)
  @param data  - Data for the MRS write
  @param bank  - Bank address for the MRS write

  @retval N/A

**/
VOID
WriteMrsDdr4 (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    rank,
  UINT32   data,
  UINT8    bank,
  UINT8    side
  )
{
  struct dimmNvram  (*dimmNvList)[MAX_DIMM];
  UINT32            backside = TRUE;
  EFI_STATUS        Status;

  if (Host->var.mem.socket[socket].hostRefreshStatus == 1) {
    FixedDelayMicroSecond (10);
  }

  dimmNvList = GetDimmNvList (Host, socket, ch);

#ifdef MRC_TRACE
  RcDebugPrintWithDevice (SDBG_TRACE, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                 "DDR4 MRS write: bank 0x%02X, data 0x%04X\n", bank, data);
#endif //MRC_TRACE
  //Trap any code error
#ifdef DDRT_SUPPORT
  if ((side == B_SIDE) && (*dimmNvList)[dimm].DcpmmPresent) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_75);
  }
#endif // DDRT_SUPPORT

  //
  // Are the address and bank mappings mirrored?
  //
  if ((rank & BIT0) && (*dimmNvList)[dimm].SPDAddrMapp) {
    data = MirrorAddrBits (Host, data);
    bank = MirrorBankBits (Host, ch, bank);
  }

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {
    // Ongoing DDR4 BFM compatibility issues
    Status = CteGetOptions ("backside", &backside);
    if (EFI_ERROR (Status)) {
      //
      // Assume backside is not valid since there was an error in the CteGetOptions call
      //
      backside = FALSE;
      RcDebugPrint (SDBG_DEFAULT, "WriteMrsDdr4: Failed CteGetOptions with return code %d.\n", Status);
    }
  }

  if (backside != FALSE) {
    //
    // Invert address/bank bits
    //
    if (side == B_SIDE) {
      data = InvertAddrBits (data);
      bank = InvertBankBits (bank);
    }
  }

#ifdef MRC_TRACE
  RcDebugPrintWithDevice (SDBG_TRACE, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                 "DDR4 MRS write after mirror and invert: bank 0x%02X, data 0x%04X\n", bank, data);
#endif //MRC_TRACE
  WriteCADBCmd (Host, socket, ch, dimm, rank, MRS_CMD, bank, data, 0);

  FixedDelayMicroSecond (1);

} // WriteMrsDdr4

/**

  Mirrors address bits

  @param Host  - Pointer to sysHost
  @param data  - Data for MRS write

  @retval dataNew - Mirrored data

**/
UINT32
MirrorAddrBits (
  PSYSHOST  Host,
  UINT32    data
  )
{
  UINT32  dataNew;

  dataNew = data &~0x1F8;
  //
  // Swap of bits (3,4), (5,6), (7,8)
  //
  dataNew |= ((data & BIT3) << 1) | ((data & BIT4) >> 1) | ((data & BIT5) << 1) | ((data & BIT6) >> 1) | ((data & BIT7) << 1) | ((data & BIT8) >> 1);

  //
  // Swap of address bits (11,13)
  //
  dataNew &= ~(BIT11 | BIT13);
  dataNew |= ((data & BIT11) << 2) | ((data & BIT13) >> 2);

  return dataNew;
} // MirrorAddrBits

/**
  Mirrors bank address bits

  @param [in] Host - Pointer to sysHost
  @param [in] Ch   - Channel number (0-based)
  @param [in] Bank - Bank address to be mirrored

  @retval BankNew - Mirrored bank address
**/
UINT8
MirrorBankBits (
  IN PSYSHOST Host,
  IN UINT8    Ch,
  IN UINT8    Bank
  )
{
         UINT8        Socket;
         UINT8        BankNew = 0;
  struct channelNvram (*ChannelNvList)[MAX_CH];

  Socket = Host->var.mem.currentSocket;
  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // Swap bits 0 (BA0) and 1 (BA1) of the bank address
  //
  BankNew |= ((Bank & BIT0) << 1) | ((Bank & BIT1) >> 1);

  if ((*ChannelNvList)[Ch].features & X16_PRESENT) {
    //
    // Don not swap bank groups for x16 devices since BG1 is not valid
    //
    BankNew |= (Bank & BIT2);
  } else {
    //
    // Swap bits 2 (BG0) and 3 (BG1) of the bank address
    //
    BankNew |= ((Bank & BIT2) << 1) | ((Bank & BIT3) >> 1);
  }

  return BankNew;
} // MirrorBankBits

/**

  Inverts address bits

  @param bank  - Address to be inverted

  @retval bankNew - Inverted address

**/
UINT32
InvertAddrBits (
  UINT32 data
  )
{
  return ((data ^ (BIT3 | BIT4 | BIT5 | BIT6 | BIT7 | BIT8 | BIT9 | BIT11 | BIT13 | BIT17)) & 0x3FFFF);

} // InvertAddrBits

/**

  Inverts bank address bits

  @param bank  - Bank address to be inverted

  @retval bankNew - Inverted bank address

**/
UINT8
InvertBankBits (
  UINT8 bank
  )
{
  return ((bank ^ (BIT0 | BIT1 | BIT2 | BIT3)) & 0xF);
} // InvertBankBits


/**

  Inverts bank address bits

  @param bank  - Bank address to be inverted

  @retval bankNew - Inverted bank address

**/
UINT8
InvertMprBankBits (
  UINT8 bank
  )
{
  return ((bank ^ (BIT3 | BIT2)) & 0xF);
} // InvertMprBankBits

/*
Temporary backside drive strength settings until SPD bytes are defined:

Card type DRAM      Setting       RC03  RC04  RC05
1Rx8      4 or 5    light         0     0     0
2Rx8/1Rx4 8 or 10   moderate      5     5     5
2Rx4      16 or 20  strong        A     A     A
4Rx4      32 or 40  very strong   F     F     F
*/

UINT8   RegisterBacksideDriveStrength[4][2] = {
//  x8,  x4
  {  0,   5}, // 1R
  {  5, 0xA}, // 2R
  {  5, 0xA}, // 3R
  {  5, 0xA}  // 4R
};


/**
  Return TRUE for (1) NGN DIMMs, (2) Channel with at least one x4 16Gb DIMM

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket Id
  @param[in] Ch         - Channel number (0-based)
  @param[in] TargetDimm - DIMM number (0-based)

  @retval   True      This DIMM has A17 Enabled
  @retval   False     This DIMM does not have A17 Enabled
**/
BOOLEAN
A17Enabled (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    TargetDimm
  )
{
  UINT8  Dimm;
  UINT8  Spd16GbPresent = 0;
  struct dimmNvram(*DimmNvList)[MAX_DIMM];
  struct channelNvram(*channelNvList)[MAX_CH];

  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  channelNvList = GetChannelNvList (Host, Socket);

  //
  // Determine if x4 16Gb DIMM is populated in the channel
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if (((*DimmNvList)[Dimm].sdramCapacity == SPD_16Gb) && ((*DimmNvList)[Dimm].x4Present)) {
      Spd16GbPresent = 1;
    }
  }

  //
  // Enable A17 if x4 16Gb DIMM is populated on the channel
  //
  if (Spd16GbPresent) {
    return TRUE;
  }


  //
  // NGN: Enable A17 for NGN DIMM
  //
  if (((*DimmNvList)[TargetDimm].DcpmmPresent == 1) && ((*DimmNvList)[TargetDimm].x4Present != 0)) {
    return TRUE;
  }

  return FALSE;
}

/**
  Return TRUE if the DIMM has the A17 signal enabled for the backside

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket Id
  @param[in] Ch       - Channel number (0-based)
  @param[in] Dimm     - DIMM number (0-based)

  @retval   True      This DIMM has A17 Enabled
  @retval   False     This DIMM does not have A17 Enabled
**/
BOOLEAN
A17EnabledBackside (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    Dimm
  )
{
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  //
  // Enable A17 if DIMM is x4 16Gb
  //
  if (((*DimmNvList)[Dimm].sdramCapacity == SPD_16Gb) && ((*DimmNvList)[Dimm].x4Present)) {
    return TRUE;
  }

  //
  // NGN: Enable A17 for NGN DIMM
  //
  if (((*DimmNvList)[Dimm].DcpmmPresent == 1) && ((*DimmNvList)[Dimm].x4Present != 0)) {
    return TRUE;
  }

  return FALSE;
}

/**

  Program register control words for RDIMMs

  @param Host  - Pointer to sysHost
  @param socket  - Socket Id
  @param ch    - Channel number (0-based)
  @param dimm  - DIMM number (0-based)
  @param rank  - Rank number (0-based)

  @retval N/A

**/
VOID
DoRegisterInitDDR4 (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    rank
  )
{
  UINT8 controlWordData;
  UINT8 tempData;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  UINT16 rdimmFrequency = 0;
  INT16               piDelay;
  UINT16              minVal = 0xFF;
  UINT16              maxVal = 0;

  channelNvList = GetChannelNvList (Host, socket);
  dimmNvList = GetDimmNvList (Host, socket, ch);

  MRSCmdStacking (Host, socket, ch, dimm, LOAD_MRS);
  //
  // Write to RC0
  //
  controlWordData = 0;
  WriteRC (Host, socket, ch, dimm, rank, controlWordData, RDIMM_RC00);

  //
  // Write to RC1
  //
  controlWordData = 0;
  WriteRC (Host, socket, ch, dimm, rank, controlWordData, RDIMM_RC01);

  //
  // Write to RC2
  //

  //
  // If configuration is 16Gbx4, we need to enable Input Bus Termination (IBT) clearing BIT 0 of RC2.
  //
  if (A17Enabled (Host, socket, ch, dimm)) {
    controlWordData = 0;
  } else {
    controlWordData = BIT0;
  }

  WriteRC (Host, socket, ch, dimm, rank, controlWordData, RDIMM_RC02);

  if ((*dimmNvList)[dimm].SPDSpecRev >= 7) {
    //
    // Get CA and CS Signals Driver Characteristics from SPD byte 137 upper nibble
    //
    controlWordData = (*dimmNvList)[dimm].SPDODCtl >> 4;
  } else {
    controlWordData = RegisterBacksideDriveStrength[ (*dimmNvList)[dimm].numDramRanks - 1][ (*dimmNvList)[dimm].x4Present];
  }
  //
  // Write to RC3
  //
  WriteRC (Host, socket, ch, dimm, rank, controlWordData, RDIMM_RC03);

  if ((*dimmNvList)[dimm].SPDSpecRev >= 7) {
    //
    // Get ODT and CKE Signals Driver Characteristics from SPD byte 137 lower nibble
    //
    controlWordData = (*dimmNvList)[dimm].SPDODCtl & 0xF;
    tempData = (controlWordData << 2) & 0xC;
    controlWordData = (controlWordData >> 2) & 0x3;
    controlWordData |= tempData;
  } else {
    controlWordData = RegisterBacksideDriveStrength[ (*dimmNvList)[dimm].numDramRanks - 1][ (*dimmNvList)[dimm].x4Present];
  }
  //
  // Write to RC4
  //
  WriteRC (Host, socket, ch, dimm, rank, controlWordData, RDIMM_RC04);

  //s4987926 WA starts
  if ((*dimmNvList)[dimm].SPDRegVen == MFGID_INPHI) {
    //Inphi test mode disable
    controlWordData = ((0x2) << 5) | (0x0 << 4) | 0x0;           //RC4x = Func upper addr F7
    WriteRC (Host, socket, ch, dimm, 0, controlWordData, 0x40); //RDIMM_RC4x);
    controlWordData = (UINT8)(0x02);                            //RC6x = Data
    WriteRC (Host, socket, ch, dimm, 0, controlWordData, 0x60); //RDIMM_RC6x);
    WriteRC (Host, socket, ch, dimm, 0, 0x5, 0x06);             //RDIMM_RCx6); //Issue CSR Write
  }
  //s4987926 WA ends

  if ((*dimmNvList)[dimm].SPDSpecRev >= 7) {
    //
    // Get Clock Driver Characteristics from SPD byte 138 lower nibble
    //
    controlWordData = (*dimmNvList)[dimm].SPDODCk & 0xF;
    tempData = (controlWordData << 2) & 0xC;
    controlWordData = (controlWordData >> 2) & 0x3;
    controlWordData |= tempData;
  } else {
    controlWordData = RegisterBacksideDriveStrength[ (*dimmNvList)[dimm].numDramRanks - 1][ (*dimmNvList)[dimm].x4Present];
  }
  //
  // Write to RC5
  //
  WriteRC (Host, socket, ch, dimm, rank, controlWordData, RDIMM_RC05);

  //
  // Write to RC4X(Set bits of reg to read (RC4X))
  //
  controlWordData = 0xf;
  WriteRC (Host, socket, ch, dimm, rank, controlWordData, RDIMM_RC4x);

  //
  // Write to RC8
  //

  controlWordData = 0x0;

  if ((*channelNvList)[ch].encodedCSMode == 2) {
    // For 3DS, use the cidBitMap variable to enable the proper CID bits in the RCD

    if ((*channelNvList)[ch].cidBitMap & BIT1) {
      controlWordData |= QxC_1_0_enabled_QxC_2_disabled;
    } else if ((*channelNvList)[ch].cidBitMap & BIT0) {
      controlWordData |= QxC_2_1_disabled_QxC_0_enabled;
    } else {
      controlWordData |= QxC_2_0_disabled;
    }
  } else if ((*dimmNvList)[dimm].numDramRanks == 4) {
    // Enable C[1:0] because they are used as chip selects.
    controlWordData |= 0x9;
  } else {
    // Disable C[2:0]
    controlWordData |= 0xB;
  }

  //
  // DA17 input buffer and QxA17 outputs enabled for 16Gb devices by clearing Bit 3
  //
  if (A17Enabled (Host, socket, ch, dimm)) {
    controlWordData &= ~BIT3;
  } else {
    controlWordData |= BIT3;
  }

  WriteRC (Host, socket, ch, dimm, rank, controlWordData, RDIMM_RC08);

  //
  // Write to RC0A
  //
  if (Host->var.mem.socket[socket].hostRefreshStatus == 0) {
    if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_1600) {
      controlWordData = 0;
    } else if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_1866) {
      controlWordData = 1;
    } else if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_2133) {
      controlWordData = 2;
    } else if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_2400) {
      controlWordData = 3;
    } else if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_2666) {
      controlWordData = 4;
    } else if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_2933) {
      controlWordData = 5;
    } else if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_3200) {
      controlWordData = 6;
    }
    WriteRC (Host, socket, ch, dimm, rank, controlWordData, RDIMM_RC0A);

    //
    // Write to RC0B
    //
    controlWordData = 0;
    if (!InternalRcdVrefCaEnabled ()) {
      controlWordData = RC0B_INPUT_VREF_SOURCE_EXTERNAL;
    }
    WriteRC (Host, socket, ch, dimm, rank, controlWordData, RDIMM_RC0B);
  }

  //
  // Write to RC0C
  //
  controlWordData = 0;
  WriteRC (Host, socket, ch, dimm, rank, controlWordData, RDIMM_RC0C);

  //
  // Write to RC0E
  //
  controlWordData = 0x0;
  WriteRC (Host, socket, ch, dimm, rank, controlWordData, RDIMM_RC0E);

  //
  // Write to RC0F
  //
  if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_2400 || ((*channelNvList)[ch].dimmRevType == RCD_REV_2)) {
    controlWordData = 0x0;
  } else {
    controlWordData = 0x1;
  }
  if (IsLrdimmPresent (socket, ch, dimm) && (*channelNvList)[ch].ddrtEnabled && Host->nvram.mem.socket[socket].ddrFreq <= DDR_1866) {
    controlWordData = 0x1;
  }
  WriteRC (Host, socket, ch, dimm, rank, controlWordData, RDIMM_RC0F);

  //
  // Write to RC1x
  //
  if (InternalRcdVrefCaEnabled ()) {
    controlWordData = (UINT8) ((*channelNvList)[ch].dimmList[dimm].rcVref >> (8 * DdrLevel)) & 0xff;
    WriteRC (Host, socket, ch, dimm, rank, controlWordData, RDIMM_RC1x);
  }

  //
  // Write to RC3x
  //
  if (Host->var.mem.socket[socket].hostRefreshStatus == 0) {

    rdimmFrequency = Host->nvram.mem.socket[socket].ddrFreqMHz * 2;
    if (rdimmFrequency < 1241) {
      rdimmFrequency = 1241;
    }
    if (rdimmFrequency > 3200) {
      rdimmFrequency = 3200;
    }
    controlWordData = (UINT8) ((rdimmFrequency - 1241) / 20);
    //
    // TI DDR4 Register Errata Sheet v1.8, CAB4A3 Errata #3
    //
    if (!(((*dimmNvList)[dimm].SPDRegVen == MFGID_TI) && ((*dimmNvList)[dimm].SPDRegRev == 0xA))) {
      WriteRC (Host, socket, ch, dimm, rank, controlWordData, RDIMM_RC3x);
    }
  }


  //If single rank dimm, then
  //  * bit 6 DCKE1 Input Buffer and QxCKE1 Output Driver Disable (1, disabled)
  //  * bit 5 DCKE1 Input Bus Termination Disable (0, not disabled)
  if ((*dimmNvList)[dimm].numDramRanks == 1) {
    controlWordData = BIT6;
    controlWordData &= (~BIT5);
    WriteRC (Host, socket, ch, dimm, rank, controlWordData, RDIMM_RCBx);
  }

  MRSCmdStacking (Host, socket, ch, dimm, EXECUTE_MRS);

  //
  // Restore backside Cmd, Ctl, Ck delays
  //
  if (IsDdr4RegisterRev2 (Host, socket, ch, dimm)) {
    piDelay = 0;
    GetSetClkDelayCore (Host, socket, ch, dimm, LrbufLevel, 0, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &piDelay);
    GetSetCtlGroupDelayCore (Host, socket, ch, dimm, LrbufLevel, CtlAll, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &piDelay, &minVal, &maxVal);
    GetSetCmdGroupDelayCore (Host, socket, ch, dimm, LrbufLevel, CmdAll, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &piDelay, &minVal, &maxVal);
  }
} // DoRegisterInitDDR4

VOID
DoRegisterInitDDR4_CKE (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    rank
  )
/*++

  Program register control words for RDIMMs

  @param Host    - Pointer to sysHost
  @param socket  - Socket Id
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)

  @retval N/A

--*/
{
  UINT8  controlWordData;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
#ifdef  LRDIMM_SUPPORT
  UINT8 i;
  UINT8 partNumberMatch;
  UINT16 dateCode;
  CHAR8 SKHynix4Rx4PartNum[SPD_MODULE_PART_DDR4 + 1] = "HMA84GL7MMR4N-TFT1  ";
#endif

  channelNvList = GetChannelNvList (Host, socket);
  dimmNvList = GetDimmNvList (Host, socket, ch);

  //
  // Write to RC9
  //
  if ((*channelNvList)[ch].maxDimm > 1) {
    controlWordData = 0x8;     // 10xx to disable IBT-off during power down
  } else {
    controlWordData = 0xC;     // 11xx to enable IBT-off during power down
  }
  if (PcdGetBool (PcdNvDimmEn)) {
    if (((*dimmNvList)[dimm].numRanks == 1) && ((*dimmNvList)[dimm].NvDimmNPresent))  {
      controlWordData |= 3;
    }
  }
  WriteRC (Host, socket, ch, dimm, rank, controlWordData, RDIMM_RC09);

  //
  // Write to RC0D
  //
  controlWordData = 0;
  if ((*dimmNvList)[dimm].SPDAddrMapp) {
    // Enable address mirroring
    controlWordData |= BIT3;
  }
  if (FeaturePcdGet (PcdCteBuild) == FALSE) {
    //
    // DDR BFM Compatibility issue
    //
    if ((*dimmNvList)[dimm].numRanks == 4 && ((*channelNvList)[ch].encodedCSMode != 2)) {
      controlWordData |= BIT0;
    }
  }
  //
  // IDT B0 sighting 200001: Use LRDIMM mode
  //
  if (!(((*dimmNvList)[dimm].SPDRegVen == MFGID_IDT) && ((*dimmNvList)[dimm].SPDRegRev == IDT_SPD_REGREV_B0))) {
    if (IsLrdimmPresent (socket, ch, dimm) == 0) {
      // Indicate this is an RDIMM
      controlWordData |= BIT2;
    }
  }
  WriteRC (Host, socket, ch, dimm, rank, controlWordData, RDIMM_RC0D);

#ifdef  LRDIMM_SUPPORT
  //M88DDR4 RCD/DB Sighting Report,
  // Montage MRC code w/a for SKHynix 4R x4 LRDIMM PCB issue
  //
/* Montage note: 07/03/2013
     The following two lines are used to pull-in the output clock Y of RCD by T/8 offset.
     This is to help fix the QCA/Y hold time margin issue reported by Hynix.*/
  if ((*dimmNvList)[dimm].SPDRegVen == MFGID_MONTAGE &&
      IsLrdimmPresent (socket, ch, dimm)) {

    partNumberMatch = 1;
    for (i = 0; i < SPD_MODULE_PART_DDR4; i++) {
      if ((*dimmNvList)[dimm].SPDModPartDDR4[i] != SKHynix4Rx4PartNum[i]) {
        partNumberMatch = 0;
        break;
      }
    }
    dateCode = (*dimmNvList)[dimm].SPDModDate >> 8;
    dateCode |= (*dimmNvList)[dimm].SPDModDate << 8;
    if ((partNumberMatch) && (dateCode < 0x1330)) {
      controlWordData = 0x07;
      WriteLrbufSmb (Host, socket, ch, dimm, sizeof (UINT8), (3 << 12) | 0x25, (UINT32 *)&controlWordData);
    }
  }

#endif //LRDIMM_SUPPORT
} // DoRegisterInitDDR4_CKE

/**

  Writes a nibble to RDIMM/LRDIMM register control words

  @param Host            - Pointer to sysHost
  @param socket            - Socket Id
  @param ch              - Channel number (0-based)
  @param rank            - Rank number (0-based)
  @param controlWordData - Control Word data (a nibble)
  @param controlWordAddr - Control Word Address

  @retval N/A

**/
VOID
WriteRC (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     controlWordData,
  UINT8     controlWordAddr
  )
{
  struct dimmNvram  (*dimmNvList)[MAX_DIMM];

#ifdef MRC_TRACE
  RcDebugPrintWithDevice (SDBG_RCWRITETAG, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                 "Write RC%02x = 0x%02x\n", controlWordAddr, controlWordData);
#endif //MRC_TRACE

  dimmNvList = GetDimmNvList (Host, socket, ch);

  if ((rank >= 2) && (((*dimmNvList)[dimm].dimmAttrib & 3) < 2)) {
    rank = rank & 0xFC;
  } else {
    rank = rank & 0xFE;
  }

  if (controlWordAddr >= 0x10) {
    WriteMRS (Host, socket, ch, dimm, rank, ((controlWordAddr & 0xF0) << 4) | controlWordData, BANK7);
    (*dimmNvList)[dimm].rcxCache[controlWordAddr >> 4] = controlWordData;
  } else {
    WriteMRS (Host, socket, ch, dimm, rank, (controlWordAddr << 4) | controlWordData, BANK7);
    (*dimmNvList)[dimm].rcCache[controlWordAddr] = controlWordData;
  }
} // WriteRC

/**

    MPR Write Pattern Rank DDR4

    @param Host    - Pointer to SysHost
    @param socket  - Processor Socket
    @param ch      - Channel on socket
    @param dimm    - DIMM on Channel
    @param rank    - Rank on DIMM
    @param side    - Side
    @param pattern - Pattern

    @retval N/A

**/
VOID
MprWritePatternRankDdr4 (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     side,
  UINT16    pattern
  )
{
  UINT32            addr;
  UINT8             bank;
  struct dimmNvram  (*dimmNvList)[MAX_DIMM];
  EFI_STATUS        Status;
  UINT32            backside = TRUE;

  dimmNvList = GetDimmNvList (Host, socket, ch);

  if ((*dimmNvList)[dimm].DcpmmPresent) {

    ProgramDFnvMprPattern (Host, socket, ch, dimm, pattern);

  } else {

    //
    // Issue write command with A[7:0] = desired MPR pattern
    // A7 = UI0, A6 = UI1, ..., A0 = UI7
    // BA[1:0] = MPR location 0
    //

    //
    // Create value to write to DDR3CMD
    //
    addr = pattern & 0xFF;
    bank = 0;

#ifdef MRC_TRACE
    RcDebugPrintWithDevice (SDBG_TRACE, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                  "DDR4 MPR write: bank 0x%02X, data 0x%04X\n", bank, addr);
#endif //MRC_TRACE


    if (FeaturePcdGet (PcdCteBuild) == TRUE) {
      //
      // Ongoing DDR4 BFM compatibility issues
      //
      Status = CteGetOptions ("backside", &backside);
      if (EFI_ERROR (Status)) {
        //
        // Assume backside is not valid since there was an error in the CteGetOptions call
        //
        backside = FALSE;
        RcDebugPrint (SDBG_DEFAULT, "MprWritePatternRankDdr4: Failed CteGetOptions with return code %d.\n", Status);
      }
    }

    if (backside != FALSE) {
      //
      // Are the address and bank mappings mirrored?
      //
      if ((rank & BIT0) && (*dimmNvList)[dimm].SPDAddrMapp) {
        addr = MirrorAddrBits (Host, addr);
        bank = MirrorBankBits (Host, ch, bank);
      }
      //
      // Invert address/bank bits
      //
      if (side == B_SIDE) {
        addr = InvertAddrBits (addr);
        bank = InvertMprBankBits (bank);
      }
    }

#ifdef MRC_TRACE
    RcDebugPrintWithDevice (SDBG_TRACE, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                  "DDR4 MPR write after mirror and invert: bank 0x%02X, data 0x%04X\n", bank, addr);
#endif //MRC_TRACE
    WriteCADBCmd (Host, socket, ch, dimm, rank, WR_CMD, bank, addr, 0);

    FixedDelayMicroSecond (1);
  }
} // MprWritePatternRankDdr4

#ifdef DEBUG_CODE_BLOCK
#define MPR_PAGE_0     0
#define MPR_LOCATION_3 3

/**
  Clear dimm_alert after MPR read completion.
  dimm_alert is set when doing a MPR read and the register bit 6 of MR_READ_RESULT_DQ12TO0_MCDDC_DP_REG is set
  MR_READ_RESULT_DQ12TO0_MCDDC_DP_REG cannot be directly cleared, so we need a MPR read that returns 0 to clear the register
  Reading Page 0 and Location 3 of Side A of Dimm will clear the register MR_READ_RESULT_DQ12TO0_MCDDC_DP_REG
  This does not applies to AEP dimms

  @param[in] Host    - Pointer to sysHost
  @param[in] socket  - Socket Id

  @retval N/A
**/
VOID
ClearDimmAlert (
  IN PSYSHOST Host,
  IN UINT8 socket
)
{
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  UINT8 MaxChDdr;
  BOOLEAN DimmAlertCleared = TRUE;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    dimmNvList = GetDimmNvList (Host, socket, ch);
    DimmAlertCleared = FALSE;
    for (dimm = 0; dimm < MAX_DIMM && DimmAlertCleared == FALSE; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0 || (*dimmNvList)[dimm].DcpmmPresent == 1) {
        continue;
      }
      for (rank = 0; rank < MAX_RANK_DIMM && DimmAlertCleared == FALSE; rank++) {
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        ReadMprCADBSeq (Host, socket, ch, dimm, rank, SIDE_A, MPR_PAGE_0, MPR_LOCATION_3);
        DimmAlertCleared = TRUE; // Do only once per Channel
      } //rank
    } //dimm
  }//channel
} //ClearDimmAlert


/**

  Prints all DRAM, Register and Buffer CSR data from all DIMMs on the socket.
  Note: this function assumes that the read training has been successfully
  completed in order to read DRAM and Buffer data.

  @param Host    - Pointer to sysHost
  @param socket  - Socket Id

  @retval N/A

**/
VOID
DumpDimmStateDdr4 (
  PSYSHOST  Host,
  UINT8     socket
  )
{
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               strobe;
  UINT8               maxStrobe;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  UINT8               mprPage, mprLoc;
  UINT8               MprData[MAX_STROBE];
  UINT32              lrbufData = 0;
  UINT16              i;
  UINT32              PrevDebugLevel;
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);
  maxStrobe = MSVx8;
  RcDebugPrint (SDBG_DEFAULT, "\n");
  PrevDebugLevel = GetDebugLevel ();

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, socket, ch);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {

      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }

      if (Host->nvram.mem.dimmTypePresent == RDIMM) {

        if (!(*dimmNvList)[dimm].DcpmmPresent) {

          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "DDR4 Reg CW data:\n");
          AcquirePrintControl ();
          RcDebugPrint (SDBG_DEFAULT, "SMbus Byte: 0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F\n");

          for (i = 0; i < 0x20; i += 4) {
            if (SUCCESS == ReadLrbufSmb (Host, socket, ch, dimm, i, &lrbufData)) {

              if (i % 16 == 0) {
                RcDebugPrint (SDBG_DEFAULT, "\n      0x%02x:", i);
              }
              RcDebugPrint (SDBG_DEFAULT, " 0x%02x 0x%02x 0x%02x 0x%02x",
                         lrbufData & 0xFF, (lrbufData >> 8) & 0xFF, (lrbufData >> 16) & 0xFF, (lrbufData >> 24) & 0xFF);
            } else {
              RcDebugPrint (SDBG_DEFAULT, "Failure: DDR4 Reg SMBus RD at off 0x%03x\n", i);
            }
          }
          RcDebugPrint (SDBG_DEFAULT, "\n\n");
          ReleasePrintControl ();
        }

        //
        // Dump the buffer BCWs
        //
        DumpStateBcwDdr4 (Host, socket, ch, dimm);
      }//RDIMM

      if (!(*dimmNvList)[dimm].DcpmmPresent) {

        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "DDR4 DRAM MPR data:\n");
        AcquirePrintControl ();
        if ((*dimmNvList)[dimm].x4Present) {
          RcDebugPrint (SDBG_DEFAULT, "DQ Index:       0    4    8    12   16   20   24   28   32   36   40   44   48   52   56   60   64   68\n");
        } else {
          RcDebugPrint (SDBG_DEFAULT, "DQ Index:       0         8         16        24        32        40        48        56        64\n");
        }

        for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          for (mprPage = 0; mprPage < 4; mprPage++) {
            for (mprLoc = 0; mprLoc < 4; mprLoc++) {
              if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
                SetDebugLevel (SDBG_ERROR);
              }
              ReadMprRankDdr4 (Host, socket, ch, dimm, rank, mprPage, mprLoc, MprData);
              SetDebugLevel (PrevDebugLevel);
              RcDebugPrint (SDBG_DEFAULT, "R%d MPR P%d, L%d: ", rank, mprPage, mprLoc);

              for (strobe = 0; strobe < maxStrobe; strobe++) {
                RcDebugPrint (SDBG_DEFAULT, " 0x%02x", MprData[strobe]);

                if ((*dimmNvList)[dimm].x4Present) {
                  RcDebugPrint (SDBG_DEFAULT, " 0x%02x", MprData[strobe + MSVx8]);
                } else {
                  RcDebugPrint (SDBG_DEFAULT, "     ");
                }
              }//strobe
              RcDebugPrint (SDBG_DEFAULT, "\n");
            }//mprLoc
          }//mprPage
        }//rank
        RcDebugPrint (SDBG_DEFAULT, "\n");
        ReleasePrintControl ();
      }
    }//dimm
  }//ch
  ClearDimmAlert (Host, socket);
} //DumpDimmStateDdr4
#endif // DEBUG_CODE_BLOCK

/**

  Prints all DRAM, Register and Buffer CSR data from all DIMMs on the socket.
  Note: this function assumes that the read training has been successfully
  completed in order to read DRAM and Buffer data.

  @param[in] Socket  - Socket Id

  @retval MRC_STATUS_FAILURE if the DRAM technology is not supported
          MRC_STATUS_SUCCESS otherwise

**/
MRC_STATUS
EFIAPI
DumpDimmState (
  IN    UINT8     Socket
  )
{
#if DEBUG_CODE_BLOCK

  PSYSHOST    Host;

  //
  // Only enabled for SDBG_MAX
  //
  if ((GetDebugLevel () & SDBG_MAX) == 0) {
    return MRC_STATUS_SUCCESS;
  }

  Host = GetSysHostPointer ();

  if (IsDdr5Present (Host, Socket)) {
    DumpDimmStateDdr5 (Socket);
  } else {
    DumpDimmStateDdr4 (Host, Socket);
  }
#endif // DEBUG_CODE_BLOCK

  return MRC_STATUS_SUCCESS;
}

/**

  Enables or Disables the MPR pattern in all DIMMs/Channels/Ranks

  @param Host    - Pointer to sysHost
  @param socket  - Socket Id
  @param mprMode - Either 0 for Disable or MR3_MPR to Enable

  @retval Supported DDR frequency

**/
VOID
SetRankMprDdr4 (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     side,
  UINT8     mprPage,
  UINT16    mprMode
  )
{
#ifndef DDR5_SUPPORT

  UINT16            addr;
  UINT8             bank;
  struct rankDevice   (*rankStruct)[MAX_RANK_DIMM];

  rankStruct = GetRankStruct (Host, socket, ch, dimm);

  if (mprMode) {
    addr = (*rankStruct)[rank].MR3 | MR3_MPR | (mprMode & BIT11) | mprPage;
    bank = BANK3;
  } else {
    addr = (*rankStruct)[rank].MR3;
    bank = BANK3;
  }

  WriteMrsDdr4 (Host, socket, ch, dimm, rank, addr, bank, side);
#endif // !DDR5_SUPPORT

} // SetRankMprDdr4

VOID
SetRankMPRPatternDA (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT16    mprMode,
  UINT16    pattern
  )
{
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  dimmNvList = GetDimmNvList (Host, socket, ch);

  if ((*dimmNvList)[dimm].DcpmmPresent) {
    ProgramDAFnvMprPattern (Host, socket, ch, dimm, pattern);
    SetRankDAMprFnv (Host, socket, ch, dimm, mprMode);
  }
}

/**

  Enables or Disables the MPR pattern in all DIMMs/Channels/Ranks

  @param Host    - Pointer to sysHost
  @param socket  - Socket Id
  @param mprMode - Either 0 for Disable or MR3_MPR to Enable

  @retval N/A

**/
VOID
SetRankMPRPattern (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT16    mprMode,
  UINT16    pattern
  )
{
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];

  dimmNvList = GetDimmNvList (Host, socket, ch);

  if (mprMode) {
    SetRankMprDdr4 (Host, socket, ch, dimm, rank, A_SIDE, 0, mprMode);
    MprWritePatternRankDdr4 (Host, socket, ch, dimm, rank, A_SIDE, pattern);
    if ((*dimmNvList)[dimm].DcpmmPresent) {
      SetRankDMprFnv (Host, socket, ch, dimm, mprMode);
    } else if (Host->nvram.mem.dimmTypePresent == RDIMM) {
      SetRankMprDdr4 (Host, socket, ch, dimm, rank, A_SIDE, 0, 0);
      SetRankMprDdr4 (Host, socket, ch, dimm, rank, B_SIDE, 0, mprMode);
      MprWritePatternRankDdr4 (Host, socket, ch, dimm, rank, B_SIDE, pattern);
      SetRankMprDdr4 (Host, socket, ch, dimm, rank, A_SIDE, 0, mprMode);
    }
  } else {
    if ((*dimmNvList)[dimm].DcpmmPresent) {
      SetRankDMprFnv (Host, socket, ch, dimm, mprMode);
    }
    SetRankMprDdr4 (Host, socket, ch, dimm, rank, A_SIDE, 0, mprMode);
    if ((Host->nvram.mem.dimmTypePresent == RDIMM) && !(*dimmNvList)[dimm].DcpmmPresent) {
      SetRankMprDdr4 (Host, socket, ch, dimm, rank, B_SIDE, 0, mprMode);
    }
  }

} // SetRankMPRPattern


/**

  Enables or Disables the MPR pattern in all DIMMs/Channels/Ranks

  @param Host    - Pointer to sysHost
  @param socket  - Socket Id
  @param mprMode - Either 0 for Disable or MR3_MPR to Enable

  @retval N/A

**/
VOID
SetRankMPR (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     mprMode
  )
{
#ifdef DDR5_SUPPORT
  if (IsDdr5Present (Host, socket)) {
    // BRS_TODO:
    // Use MRW to initialize DATA0 patttern
    return;
  }
#endif
  if (Host->nvram.mem.dramType == SPD_TYPE_DDR4) {
    SetRankMPRPattern (Host, socket, ch, dimm, rank, mprMode, 0x33a5);
  } else {
    SetRankMPRPattern (Host, socket, ch, dimm, rank, mprMode, 0x3355);
  }
} // SetRankMPR


/**

  Enables or Disables the MPR pattern in all DIMMs/Channels/Ranks

  @param Host    - Pointer to sysHost
  @param socket    - Socket Id
  @param mprMode - Either 0 for Disable or MR3_MPR to Enable

  @retval N/A

**/
VOID
SetAllRankMPR (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     mprMode
  )
{
  struct rankDevice  (*rankStruct)[MAX_RANK_DIMM];
  UINT8                               ch;
  UINT8                               dimm;
  UINT8                               rank;
  struct channelNvram                 (*channelNvList)[MAX_CH];
  UINT8                               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);
  //
  // Set MPR pattern for all ranks
  //
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {

      for (ch = 0; ch < MaxChDdr; ch++) {
        rankStruct = GetRankStruct (Host, socket, ch, dimm);
        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }

        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        SetRankMPR (Host, socket, ch, dimm, rank, mprMode);
#ifdef LRDIMM_SUPPORT
        (*rankStruct)[rank].CurrentDramMode = mprMode;
        if (mprMode) {
          (*rankStruct)[rank].CurrentMpr0Pattern = 0x55;
        }
#endif
      } // ch loop
    } // rank loop
  } // dimm loop
} // SetAllRankMPR


VOID
IndirectRCWrite (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     controlWordData,
  UINT8     controlWordFunc,
  UINT8     controlWordAddr
  )
/*++

  Writes a nibble or byte to DDR4 Register Control using the indirect method

  @param Host            - Pointer to sysHost
  @param socket          - Socket Id
  @param ch              - Channel number (0-based)
  @param rank            - Rank number (0-based)
  @param controlWordData - Control Word data (a nibble or byte)
  @param controlWordAddr - Control Word Address

  @retval N/A

--*/
{

  if (controlWordAddr >= 0x10) {
    //
    // Select the control word
    //
    WriteRC (Host, socket, ch, dimm, 0, (controlWordFunc << 5) | (controlWordAddr >> 4), RDIMM_RC4x);

    //
    // Write the data
    //
    WriteRC (Host, socket, ch, dimm, 0, controlWordData, RDIMM_RC6x);

  } else {
    //
    // Select the control word
    //
    WriteRC (Host, socket, ch, dimm, 0, controlWordFunc << 5, RDIMM_RC4x);

    //
    // Write the data
    //
    WriteRC (Host, socket, ch, dimm, 0, (controlWordAddr << 4) | controlWordData, RDIMM_RC6x);
  }

  //
  // Issue the write command
  //
  WriteRC (Host, socket, ch, dimm, 0, 5, RDIMM_RC06);

} // IndirectRCWrite

/**

  Determine if Backside CMD Margin is enabled for DDR4

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket number

  @retval TRUE  - Backside CMD Margin is enabled
  @retval FALSE - Backside CMD Margin is not enabled

**/
BOOLEAN
IsDdr4BacksideCmdMarginEnabled (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  UINT8 Ch;
  UINT8 Dimm;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  SYS_SETUP *Setup;
  UINT8     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  if ((Setup->mem.enableBacksideRMT) && (Setup->mem.enableBacksideCMDRMT)) {
    channelNvList = GetChannelNvList (Host, Socket);
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((*channelNvList)[Ch].enabled == 0) {
        continue;
      }
      dimmNvList = GetDimmNvList (Host, Socket, Ch);
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*dimmNvList)[Dimm].dimmPresent == 0) {
          continue;
        }

        if (IsDdr4RegisterRev2 (Host, Socket, Ch, Dimm) || ((*dimmNvList)[Dimm].DcpmmPresent && (Setup->mem.enableNgnBcomMargining))) {
          return TRUE;
        }
      } // dimm loop
    } // ch loop
    return FALSE; // if all DIMMs dont support it, return false
  }
  return FALSE;
} // IsDdr4BacksideCmdMarginEnabled

UINT8
IsDdr4RegisterRev2 (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm
  )
{
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  dimmNvList = GetDimmNvList (Host, socket, ch);
  if ((Host->nvram.mem.dimmTypePresent == RDIMM) && ((*dimmNvList)[dimm].dimmAttrib & BIT4)) {
    return 1;
  }
  return 0;
} // IsDdr4RegisterRev2

#ifdef  LRDIMM_SUPPORT

VOID
DoLrbufInitDDR4 (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    rank
  )
/*++

  Program buffer control words for LRDIMMs

  @param Host    - Pointer to sysHost
  @param socket  - Socket Id
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)

  @retval N/A

  --*/
{
#ifndef DDR5_SUPPORT
  UINT8 controlWordData;
  UINT8 controlWordAddr;
  UINT8 fn;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  struct ddrRank (*rankList)[MAX_RANK_DIMM];
  struct rankDevice (*rankStruct)[MAX_RANK_DIMM];
  UINT8 rankBitMask = 0xE;
  UINT8 backsideRank;
  UINT8 *controlWordDataPtr;
  UINT16 lrdimmFrequency = 0;
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;

  channelNvList = GetChannelNvList (Host, socket);
  dimmNvList = GetDimmNvList (Host, socket, ch);
  rankList = GetRankNvList (Host, socket, ch, dimm);
  rankStruct = GetRankStruct (Host, socket, ch, dimm);

  RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                "DDR4 LRDIMM Buffer Init\n");

  // Inphi A1 W/A
  if (((*dimmNvList)[dimm].SPDRegVen == MFGID_INPHI) &&
      ((*dimmNvList)[dimm].lrbufRid == LRBUF_INPHI_RID_A1)) {

    MRSCmdStacking (Host, socket, ch, dimm, LOAD_MRS);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0x01, LRDIMM_F7, LRDIMM_BC4x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0x01, 0x4, LRDIMM_BC4x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0x30, LRDIMM_F6, LRDIMM_BC6x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0x01, 0x0, LRDIMM_BC0A);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0x02, LRDIMM_F7, LRDIMM_BC4x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0x00, 0x2, LRDIMM_BCEx);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0x00, 0x4, LRDIMM_BC1x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0xB1, 0x1, LRDIMM_BCFx);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0x03, LRDIMM_F7, LRDIMM_BC4x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0x80, 0x1, LRDIMM_BC8x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0x1B, 0x1, LRDIMM_BC5x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0x00, LRDIMM_F7, LRDIMM_BC4x);
    MRSCmdStacking (Host, socket, ch, dimm, EXECUTE_MRS);

    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "INPHI A1\n");
  }

  // Montage Gen2 LRBUF W/A
  if (((*dimmNvList)[dimm].SPDRegVen == MFGID_MONTAGE) && ((*dimmNvList)[dimm].lrbufGen == 0x01) && ((*dimmNvList)[dimm].lrbufRid == LRBUF_MONTAGE_RID_GEN2_A0)) {

    MRSCmdStacking (Host, socket, ch, dimm, LOAD_MRS);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0xcc, LRDIMM_F7, LRDIMM_BC4x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0x23, LRDIMM_F7, LRDIMM_BC5x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0x6d, LRDIMM_F7, LRDIMM_BC6x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0xcd, LRDIMM_F7, LRDIMM_BC4x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0xcc, LRDIMM_F7, LRDIMM_BC4x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0x22, LRDIMM_F7, LRDIMM_BC5x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0x0d, LRDIMM_F7, LRDIMM_BC6x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0xcd, LRDIMM_F7, LRDIMM_BC4x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0xcc, LRDIMM_F7, LRDIMM_BC4x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0x21, LRDIMM_F7, LRDIMM_BC5x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0x05, LRDIMM_F7, LRDIMM_BC6x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0xcd, LRDIMM_F7, LRDIMM_BC4x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0xcc, LRDIMM_F7, LRDIMM_BC4x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0x20, LRDIMM_F7, LRDIMM_BC5x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0x05, LRDIMM_F7, LRDIMM_BC6x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0xcd, LRDIMM_F7, LRDIMM_BC4x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0xcc, LRDIMM_F7, LRDIMM_BC4x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0xcc, LRDIMM_F7, LRDIMM_BC5x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0x37, LRDIMM_F7, LRDIMM_BC6x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0xcd, LRDIMM_F7, LRDIMM_BC4x);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0xcc, LRDIMM_F7, LRDIMM_BC4x);
    MRSCmdStacking (Host, socket, ch, dimm, EXECUTE_MRS);

    FixedDelayMicroSecond (1);
    WriteLrbuf (Host, socket, ch, dimm, 0, 0x01, 0x2, LRDIMM_BCCx);
    FixedDelayMicroSecond (1);
  }

  // Montage Gen2 LRBUF W/A
  if (((*dimmNvList)[dimm].SPDRegVen == MFGID_MONTAGE) && ((*dimmNvList)[dimm].lrbufGen == 0x01) &&
      (((*dimmNvList)[dimm].lrbufRid == LRBUF_MONTAGE_RID_GEN2_A0) || ((*dimmNvList)[dimm].lrbufRid == LRBUF_MONTAGE_RID_GEN2_A1))) {

    for (fn = 4; fn < 8; fn++) {
      WriteLrbuf (Host, socket, ch, dimm, 0, 0x77, fn, LRDIMM_BCCx);
      WriteLrbuf (Host, socket, ch, dimm, 0, 0x77, fn, LRDIMM_BCDx);
      WriteLrbuf (Host, socket, ch, dimm, 0, 0x77, fn, LRDIMM_BCEx);
      WriteLrbuf (Host, socket, ch, dimm, 0, 0x77, fn, LRDIMM_BCFx);
    }
  }

  // Initialize the function control nibble to 0

  WriteBC (Host, socket, ch, dimm, 0, 0, LRDIMM_F0, LRDIMM_BC7x);
  (*dimmNvList)[dimm].rcLrFunc = 0;
  //
  // Write to BC00
  //
  if (FeaturePcdGet (PcdCosimBuild) == TRUE) {
    controlWordData = 0;
  } else {
    controlWordData = (*dimmNvList)[dimm].lrBuf_BC00;
  }

  WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, LRDIMM_F0, LRDIMM_BC00);
  //
  // Write to BC01
  //
  if (FeaturePcdGet (PcdCosimBuild) == TRUE) {
    controlWordData = 0;
  } else {
    controlWordData = (*dimmNvList)[dimm].lrBuf_BC01;
  }
  WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, LRDIMM_F0, LRDIMM_BC01);
  //
  // Write to BC02
  //
  if (FeaturePcdGet (PcdCosimBuild) == TRUE) {
    controlWordData = 0;
  } else {
    controlWordData = (*dimmNvList)[dimm].lrBuf_BC02;
  }
  WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, LRDIMM_F0, LRDIMM_BC02);
  //
  // Write to BC03
  //
  controlWordData = (*dimmNvList)[dimm].lrBuf_BC03;
  WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, LRDIMM_F0, LRDIMM_BC03);
  //
  // Write to BC04
  //
  if (FeaturePcdGet (PcdCosimBuild) == TRUE) {
    controlWordData = 0;
  } else {
    controlWordData = (*dimmNvList)[dimm].lrBuf_BC04;
  }
  WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, LRDIMM_F0, LRDIMM_BC04);
  //
  // Write to BC05
  //
  controlWordData = (*dimmNvList)[dimm].lrBuf_BC05;
  WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, LRDIMM_F0, LRDIMM_BC05);
  //
  // Write to BC07
  //
  //rank present bit mask
  switch ((*dimmNvList)[dimm].numDramRanks) {
  case 1:
    rankBitMask = 0xE;
    break;

  case 2:
    rankBitMask = 0xC;
    break;

  case 4:
    rankBitMask = 0x0;
    break;
    default:
    //Automated add of default case. Please review.
    break;
  }
  controlWordData = rankBitMask;
  WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, LRDIMM_F0, LRDIMM_BC07);
  //
  // Write to BC09
  //
  controlWordData = 8;  //CKE Power Down Mode Enable
  WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, LRDIMM_F0, LRDIMM_BC09);
  //
  // Write to BC0A
  //
  // LRDIMM Set Frequency
  controlWordData = LRDIMM_BC0A_DDR4_1600;
  if (Host->nvram.mem.socket[socket].ddrFreq > DDR_1600)  {
    controlWordData = LRDIMM_BC0A_DDR4_1866;
  }
  if (Host->nvram.mem.socket[socket].ddrFreq > DDR_1866)  {
    controlWordData = LRDIMM_BC0A_DDR4_2133;
  }
  if (Host->nvram.mem.socket[socket].ddrFreq > DDR_2133)  {
    controlWordData = LRDIMM_BC0A_DDR4_2400;
  }
  if (Host->nvram.mem.socket[socket].ddrFreq > DDR_2400)  {
    controlWordData = LRDIMM_BC0A_DDR4_2666;
  }
  if (Host->nvram.mem.socket[socket].ddrFreq > DDR_2666)  {
    controlWordData = LRDIMM_BC0A_DDR4_2933;
  }
  if (Host->nvram.mem.socket[socket].ddrFreq > DDR_2933)  {
    controlWordData = LRDIMM_BC0A_DDR4_3200;
  }
  WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, LRDIMM_F0, LRDIMM_BC0A);

  //if not IDT 0xA0, write F0 BC6x
  if (!(((*dimmNvList)[dimm].SPDRegVen == MFGID_IDT) &&
        ((*dimmNvList)[dimm].lrbufRid == LRBUF_IDT_DB_RID_A0))) {
    //
    // Write F0 BC6x
    //
    lrdimmFrequency = Host->nvram.mem.socket[socket].ddrFreqMHz * 2;
    if (lrdimmFrequency < 1241) {
      lrdimmFrequency = 1241;
    }
    if (lrdimmFrequency > 3200) {
      lrdimmFrequency = 3200;
    }
    controlWordData = (UINT8) ((lrdimmFrequency - 1241) / 20);
    WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, LRDIMM_F0, LRDIMM_BC6x);
  }
  //
  // Write to BC0B
  //
  controlWordData = 0;
  WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, LRDIMM_F0, LRDIMM_BC0B);
  //
  // Write to BC0C
  //
  WriteLrbuf (Host, socket, ch, dimm, 0, LRDIMM_NORMAL_MODE, LRDIMM_F0, LRDIMM_BC0C);
  (*rankStruct)[rank].CurrentLrdimmTrainingMode = LRDIMM_NORMAL_MODE;
  //
  // Write to BC0E
  //
  WriteLrbuf (Host, socket, ch, dimm, 0, LRDIMM_NORMAL_MODE, LRDIMM_F0, LRDIMM_BC0E);
  //
  // Write to F0, BC1x
  //
  controlWordData = (*dimmNvList)[dimm].lrBuf_BC1x;
  WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, LRDIMM_F0, LRDIMM_BC1x);
  //
  // program LRDIMM Buffer Training Control Word, Host Interface VrefDQ Training Range
  //
  controlWordData = GetVrefRange (Host, (*channelNvList)[ch].txVrefSafe[ (*rankList)[rank].rankIndex]);
  // (take vref range select bit 6 and put it in the LRDIMM buffer register bit 1)
  //NOTE: the backside DRAM range is written to range 1...ok for now, but may need to change later
  WriteLrbuf (Host, socket, ch, dimm, 0, (((controlWordData & BIT6) >> 5) | BIT0), LRDIMM_F6, LRDIMM_BC4x);
  //
  // program LRDIMM Host Interface VREF Control Word
  //
  WriteLrbuf (Host, socket, ch, dimm, 0, (controlWordData & 0x3F), LRDIMM_F5, LRDIMM_BC5x);
  //
  // program LRDIMM DRAM Interface VREF Control Word
  //
  WriteLrbuf (Host, socket, ch, dimm, 0, (*rankList)[rank].lrbufRxVref[0] - DB_DRAM_VREF_RANGE2_OFFSET, LRDIMM_F5, LRDIMM_BC6x);

  for (backsideRank = 0; backsideRank < (*dimmNvList)[dimm].numDramRanks; backsideRank++) {
    //
    // Write F[3:0]BC2x/F[3:0]BC3x
    //
    controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBC2x3x[0]);
    WriteLrbufPBA (Host, socket, ch, dimm, rank, controlWordDataPtr, backsideRank, LRDIMM_BC2x, ALL_DATABUFFERS);

    controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBC2x3x[MSVx8]);
    WriteLrbufPBA (Host, socket, ch, dimm, rank, controlWordDataPtr, backsideRank, LRDIMM_BC3x, ALL_DATABUFFERS);
    //
    // Write F[3:0]BC4x/F[3:0]BC5x
    //
    controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBC4x5x[0]);
    WriteLrbufPBA (Host, socket, ch, dimm, rank, controlWordDataPtr, backsideRank, LRDIMM_BC4x, ALL_DATABUFFERS);

    controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBC4x5x[MSVx8]);
    WriteLrbufPBA (Host, socket, ch, dimm, rank, controlWordDataPtr, backsideRank, LRDIMM_BC5x, ALL_DATABUFFERS);
    //
    // Write F[3:0]BC8x/F[3:0]BC9x
    //
    controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBC8x9x[0]);
    WriteLrbufPBA (Host, socket, ch, dimm, rank, controlWordDataPtr, backsideRank, LRDIMM_BC8x, ALL_DATABUFFERS);

    controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBC8x9x[MSVx8]);
    WriteLrbufPBA (Host, socket, ch, dimm, rank, controlWordDataPtr, backsideRank, LRDIMM_BC9x, ALL_DATABUFFERS);
    //
    // Write F[3:0]BCAx/F[3:0]BCBx
    //
    controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBCAxBx[0]);
    WriteLrbufPBA (Host, socket, ch, dimm, rank, controlWordDataPtr, backsideRank, LRDIMM_BCAx, ALL_DATABUFFERS);

    controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBCAxBx[MSVx8]);
    WriteLrbufPBA (Host, socket, ch, dimm, rank, controlWordDataPtr, backsideRank, LRDIMM_BCBx, ALL_DATABUFFERS);

    //
    // Inphi A1 WA
    //
    if (((*dimmNvList)[dimm].SPDRegVen == MFGID_INPHI) &&
        ((*dimmNvList)[dimm].lrbufRid == LRBUF_INPHI_RID_A1)) {
      InphiPIWA (Host, socket, ch, dimm);
    }

    //if not IDT 0xA0, write FxBCCx, FxBCDx, FxBCEx, and FxBCFx
    if (!(((*dimmNvList)[dimm].SPDRegVen == MFGID_IDT) &&
          ((*dimmNvList)[dimm].lrbufRid == LRBUF_IDT_DB_RID_A0))) {
      // F0BCCx for backside rank 0, F1BCCx for backside rank 1, F0BCEx for backside rank 2, and F1BCEx for backside rank 3
      controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBCCxEx[0]);
      controlWordAddr = LRDIMM_BCCx + ((backsideRank >> 1) * 0x20);
      WriteLrbufPBA (Host, socket, ch, dimm, rank, controlWordDataPtr, (backsideRank & BIT0), controlWordAddr, ALL_DATABUFFERS);

      // F0BCDx for backside rank 0, F1BCDx for backside rank 1, F0BCFx for backside rank 2, and F1BCFx for backside rank 3
      controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBCDxFx[0]);
      controlWordAddr = LRDIMM_BCDx + ((backsideRank >> 1) * 0x20);
      WriteLrbufPBA (Host, socket, ch, dimm, rank, controlWordDataPtr, (backsideRank & BIT0), controlWordAddr, ALL_DATABUFFERS);
    }
  }
  //
  // Write to BC07 Again Per IDT DDR4 Data Buffer A0 sightings report version 2013_01_21_1349
  //sighting number 100002
  //
  //rank present bit mask
  switch ((*dimmNvList)[dimm].numDramRanks) {
  case 1:
    rankBitMask = 0xE;
    break;

  case 2:
    rankBitMask = 0xC;
    break;

  case 4:
    rankBitMask = 0x0;
    break;
    default:
    //Automated add of default case. Please review.
    break;
  }
  controlWordData = rankBitMask;
  WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, LRDIMM_F0, LRDIMM_BC07);

  // IDT C0/C1 W/A
  if (((*dimmNvList)[dimm].SPDRegVen == MFGID_IDT) &&
      (((*dimmNvList)[dimm].lrbufRid == LRBUF_IDT_DB_RID_GPC0) || ((*dimmNvList)[dimm].lrbufRid == LRBUF_IDT_DB_RID_GPC1))) {
    //write password
    WriteLrbuf (Host, socket, ch, dimm, 0, 0xD2, LRDIMM_F6, LRDIMM_BC6x); // upper pass
    WriteLrbuf (Host, socket, ch, dimm, 0, 0x36, LRDIMM_F7, LRDIMM_BC6x); // Lower pass

    WriteLrbuf (Host, socket, ch, dimm, 0, 0x04, LRDIMM_F8, LRDIMM_BC0x);  // Set coninuously running internal calibration engine W/A
    WriteLrbuf (Host, socket, ch, dimm, 0, 0x19, LRDIMM_F12, LRDIMM_BC0x); // Increase internal timing margin for Write CRC W/A

    WriteLrbuf (Host, socket, ch, dimm, 0, 0x00, LRDIMM_F6, LRDIMM_BC6x); // cancel upper pass
    WriteLrbuf (Host, socket, ch, dimm, 0, 0x00, LRDIMM_F7, LRDIMM_BC6x); // cancel lower pass
  }
#endif // !DDR5_SUPPORT

} // DoLrbufInitDDR4
#endif //LRDIMM_SUPPORT

//---------------------------------------------------------------------------
VOID
WriteBC (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     controlWordData,
  UINT8     controlWordFunc,
  UINT8     controlWordAddr
  )
/*++

  Writes a nibble or byte to DDR4 Buffer Control

  @param Host            - Pointer to sysHost
  @param socket          - Socket Id
  @param ch              - Channel number (0-based)
  @param rank            - Rank number (0-based)
  @param controlWordData - Control Word data (a nibble or byte)
  @param controlWordAddr - Control Word Address

  @retval N/A

--*/
{

  if ((controlWordAddr >= 0x10) || (controlWordFunc != 0)) {
    WriteMRS (Host, socket, ch, dimm, rank, BIT12 | ((controlWordAddr & 0xF0) << 4) | controlWordData, BANK7);
  } else {
    WriteMRS (Host, socket, ch, dimm, rank, BIT12 | (controlWordAddr << 4) | controlWordData, BANK7);
  }

} // WriteBC

#ifdef LRDIMM_SUPPORT
VOID
WriteLrbuf (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    rank,
  UINT8    controlWordData,
  UINT8    controlWordFunc,
  UINT8    controlWordAddr
  )
/*++

  Writes a nibble to DDR4 LRDIMM buffer control words

  @param Host            - Pointer to sysHost
  @param socket          - Socket Id
  @param ch              - Channel number (0-based)
  @param dimm            - Dimm number (0-based)
  @param rank            - Rank number (0-based)
  @param controlWordData - Control Word data
  @param controlWordFunc - Function index in the LR buffer
  @param controlWordAddr - Control Word Address

  @retval N/A

--*/
{
  struct channelNvram (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList (Host, socket);

  if (IsLrdimmPresent (socket, ch, dimm) == 0) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT, "Command not supported \n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_65);
  }

  if (PcdGetBool (PcdMrcDebugLrdimmExtraMessages) == TRUE) {

    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                            "LRDIMM BCW WRITE Fn: 0x%X  Addr: 0x%X CS: 0x%x write data: 0x%X\n",
                            controlWordFunc, controlWordAddr, rank, controlWordData);
  }

  if (Host->nvram.mem.dramType == SPD_TYPE_DDR4) {
    // Write LR function index only if needed
    if (controlWordFunc != (*channelNvList)[ch].dimmList[dimm].rcLrFunc) {
      // Write function to addr 7x
      WriteBC (Host, socket, ch, dimm, rank, controlWordFunc, LRDIMM_F0, LRDIMM_BC7x);
      (*channelNvList)[ch].dimmList[dimm].rcLrFunc = controlWordFunc;
    }

    // Write LR target data to the addr
    WriteBC (Host, socket, ch, dimm, rank, controlWordData, controlWordFunc, controlWordAddr);
  }
}



VOID
WriteLrbufPBA (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    rank,
  UINT8    *controlWordDataPtr,
  UINT8    controlWordFunc,
  UINT8    controlWordAddr,
  UINT8    bufferAddr
  )
/*++

  Writes a nibble to LRDIMM register control words using PBA mode

  @param Host               - Pointer to sysHost
  @param socket             - Socket Id
  @param ch                 - Channel number (0-based)
  @param dimm               - Dimm number (0-based)
  @param rank               - Rank number (0-based)
  @param controlWordDataPtr - Pointer to a 9 byte array of control word data
  @param controlWordFunc    - Function index in the LR buffer
  @param controlWordAddr    - Control Word Address
  @param bufferAddr         - Address of the individual buffer to target...or 'FF' for all buffers

  @retval N/A

--*/
{
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  UINT8             ReEnableMprNeeded = 0;
  UINT8             controlWordData;
  struct rankDevice   (*rankStruct)[MAX_RANK_DIMM];
  UINT8             i;
  UINT8             allBuffersSame;

  rankStruct = GetRankStruct (Host, socket, ch, dimm);
  dimmNvList = GetDimmNvList (Host, socket, ch);
  if (IsLrdimmPresent (socket, ch, dimm) == 0) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,  "Command not supported \n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_65);
  }
  //
  // boot time optimization, if all buffers need to be written with the same value, just use a broadcast write
  //
  allBuffersSame = 1;
  for (i = 1; i < MAX_STROBE/2; i++) {
    if (controlWordDataPtr[0] != controlWordDataPtr[i]) {
      allBuffersSame = 0;
      break;
    }
  }
  if ((allBuffersSame) && (bufferAddr == ALL_DATABUFFERS)) {
    WriteLrbuf (Host, socket, ch, dimm, 0, controlWordDataPtr[0], controlWordFunc, controlWordAddr);
    return;
  }

  // ONLY DDR4 will call this routine  if (Host->nvram.mem.dramType == SPD_TYPE_DDR4)

  if (PcdGetBool (PcdMrcDebugLrdimmExtraMessages) == TRUE) {

    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                            "LRDIMM PBA Fn 0x%x, BCW 0x%x, BufAddr 0x%x\n",controlWordFunc, controlWordAddr, bufferAddr);

    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                            "Data = 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n",
                            controlWordDataPtr[0],controlWordDataPtr[1],controlWordDataPtr[2],controlWordDataPtr[3],controlWordDataPtr[4],
                            controlWordDataPtr[5],controlWordDataPtr[6],controlWordDataPtr[7],controlWordDataPtr[8]);
  }

  if ((*rankStruct)[rank].CurrentDramMode == MR3_MPR) {
    SetRankMPR (Host, socket, ch, dimm, rank, 0);
    ReEnableMprNeeded = 1;
  }
  //
  // force the buffer into normal mode for the duration of the PBA write
  //
  if ((*rankStruct)[rank].CurrentLrdimmTrainingMode != LRDIMM_NORMAL_MODE) {
    WriteLrbuf (Host, socket, ch, dimm, 0, LRDIMM_NORMAL_MODE, LRDIMM_F0, LRDIMM_BC0C);
  }

  if (((*dimmNvList)[dimm].SPDRegVen == MFGID_INPHI) &&
      ((*dimmNvList)[dimm].lrbufRid == LRBUF_INPHI_RID_A1)) {
    WriteLrbuf (Host, socket, ch, dimm, 0, 2, LRDIMM_F0, LRDIMM_BC0C);
  }
  WritePBA (Host, socket, ch, dimm, 0, controlWordDataPtr, controlWordFunc, controlWordAddr, bufferAddr);

  //
  // restore the buffer training mode
  //
  controlWordData = (*rankStruct)[rank].CurrentLrdimmTrainingMode;
  if ((controlWordData != LRDIMM_NORMAL_MODE) || (((*dimmNvList)[dimm].SPDRegVen == MFGID_INPHI) &&
                                                  ((*dimmNvList)[dimm].lrbufRid == LRBUF_INPHI_RID_A1))) {
    WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, LRDIMM_F0, LRDIMM_BC0C);
  }

  //
  //restore MPR mode setting
  //
  if (ReEnableMprNeeded) {
    SetRankMPRPattern (Host, socket, ch, dimm, rank, MR3_MPR, (*rankStruct)[rank].CurrentMpr0Pattern);
  }
}

#endif  // LRDIMM_SUPPORT

//-----------------------------------------------------------------------------
/**

  Read LR Buffer

  @param Host       - Pointer to sysHost
  @param socket     - Socket Id
  @param ch         - Channel number (0-based)
  @param dimm       - DIMM number (0-based)
  @param rank       - Rank number (0-based)
  @param lrbuffAddr - Address for the read
  @param lrbuffData - Pointer to the LR Buffer data

  @retval SUCCESS
  @retval FAILURE

**/
UINT32
ReadLrbufSmb (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT16   lrbufAddr,
  UINT32   *lrbufData
  )
{

  UINT8 dc = 0;
  UINT8 func;
  UINT8 extOffset;
  UINT8 offset;
  UINT32 temp = 0xFFFFFFFF;        // Emulate master abort
  UINT32 status = FAILURE;
  SMB_DEVICE_STRUCT dev;

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {
    CteReadRcdCfgSmb (ch, dimm, lrbufAddr, sizeof (UINT32), lrbufData);
    return SUCCESS;
  }

  dev.compId = SPD;
  dev.address.controller = PLATFORM_SMBUS_CONTROLLER_PROCESSOR;
  dev.address.deviceType = DTI_LRDIMM;

  if (EFI_ERROR (GetSmbAddress (socket, ch, dimm, &dev))) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_147);
    return FAILURE;
  }

  func = (UINT8) (lrbufAddr >> 12);
  extOffset = (UINT8) ((lrbufAddr >> 8) & 0xF);
  offset = (UINT8) (lrbufAddr & 0xFF);
  if ((WriteProcSmb (dev, 0x80, &dc) == EFI_SUCCESS) &&
      (WriteProcSmb (dev, 0x00, &func) == EFI_SUCCESS) &&
      (WriteProcSmb (dev, 0x00, &extOffset) == EFI_SUCCESS) &&
      (WriteProcSmb (dev, 0x40, &offset) == EFI_SUCCESS) &&
      (ReadProcSmb (dev, 0x80, &dc) == EFI_SUCCESS) &&
      (ReadProcSmb (dev, 0x00, (UINT8 *)&temp + 3) == EFI_SUCCESS) &&
      (ReadProcSmb (dev, 0x00, (UINT8 *)&temp + 2) == EFI_SUCCESS) &&
      (ReadProcSmb (dev, 0x00, (UINT8 *)&temp + 1) == EFI_SUCCESS) &&
      (ReadProcSmb (dev, 0x40, (UINT8 *)&temp) == EFI_SUCCESS)) {

    status = SUCCESS;
  }

  *lrbufData = temp;

  return status;
}

//-----------------------------------------------------------------------------
/**

  Write LR Buffer

  @param Host       - Pointer to sysHost
  @param socket     - Socket Id
  @param ch         - Channel number (0-based)
  @param dimm       - DIMM number (0-based)
  @param size       - number of bytes to be written
  @param lrbuffAddr - Address for the read
  @param lrbuffData - Pointer to the LR Buffer data

  @retval SUCCESS
  @retval FAILURE

**/
UINT32
WriteLrbufSmb (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    size,
  UINT16   lrbufAddr,
  UINT32   *lrbufData
  )
{
  UINT8 dc = 0;
  UINT8 func;
  UINT8 extOffset;
  UINT8 offset;
  UINT32 Status = FAILURE;
  EFI_STATUS CteStatus;
  SMB_DEVICE_STRUCT dev;
  UINT32 tempData = 0;

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {

    switch (size) {
    case sizeof (UINT32) :
      tempData = *(UINT32 *) lrbufData;
      break;
    case sizeof (UINT16) :
      tempData = *(UINT16 *) lrbufData;
      break;
    case sizeof (UINT8) :
      tempData = *(UINT8 *) lrbufData;
      break;
    default:
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_77);
    }

    CteStatus = CteWriteRcdCfgSmb (ch, dimm, lrbufAddr, size, tempData);
    if (EFI_ERROR (CteStatus)) {
      Status = FAILURE;
      RcDebugPrint (SDBG_DEFAULT, "WriteLrbufSmb: Failed CteWriteRcdCfgSmb with return code %d.\n", Status);
    } else {
      Status = SUCCESS;
    }

    return Status;
  }

  dev.compId = SPD;
  dev.address.controller = PLATFORM_SMBUS_CONTROLLER_PROCESSOR;
  dev.address.deviceType = DTI_LRDIMM;

  if (EFI_ERROR (GetSmbAddress (socket, ch, dimm, &dev))) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_147);
    return FAILURE;
  }

  func = (UINT8) (lrbufAddr >> 12);
  extOffset = (UINT8) ((lrbufAddr >> 8) & 0xF);
  offset = (UINT8) (lrbufAddr & 0xFF);
  switch (size) {
  case sizeof (UINT32) :
    if ((WriteProcSmb (dev, 0x8C, &dc) == EFI_SUCCESS) &&
        (WriteProcSmb (dev, 0x0C, &func) == EFI_SUCCESS) &&
        (WriteProcSmb (dev, 0x0C, &extOffset) == EFI_SUCCESS) &&
        (WriteProcSmb (dev, 0x0C, &offset) == EFI_SUCCESS) &&
        (WriteProcSmb (dev, 0x0C, (UINT8 *) lrbufData + 3) == EFI_SUCCESS) &&
        (WriteProcSmb (dev, 0x0C, (UINT8 *) lrbufData + 2) == EFI_SUCCESS) &&
        (WriteProcSmb (dev, 0x0C, (UINT8 *) lrbufData + 1) == EFI_SUCCESS) &&
        (WriteProcSmb (dev, 0x4C, (UINT8 *) lrbufData) == EFI_SUCCESS)) {

      Status = SUCCESS;
    }
    break;
  case sizeof (UINT16) :
    if ((WriteProcSmb (dev, 0x88, &dc) == EFI_SUCCESS) &&
        (WriteProcSmb (dev, 0x08, &func) == EFI_SUCCESS) &&
        (WriteProcSmb (dev, 0x08, &extOffset) == EFI_SUCCESS) &&
        (WriteProcSmb (dev, 0x08, &offset) == EFI_SUCCESS) &&
        (WriteProcSmb (dev, 0x08, (UINT8 *) lrbufData + 1) == EFI_SUCCESS) &&
        (WriteProcSmb (dev, 0x48, (UINT8 *) lrbufData) == EFI_SUCCESS)) {

      Status = SUCCESS;
    }
    break;
  case sizeof (UINT8) :
    if ((WriteProcSmb (dev, 0x84, &dc) == EFI_SUCCESS) &&
        (WriteProcSmb (dev, 0x04, &func) == EFI_SUCCESS) &&
        (WriteProcSmb (dev, 0x04, &extOffset) == EFI_SUCCESS) &&
        (WriteProcSmb (dev, 0x04, &offset) == EFI_SUCCESS) &&
        (WriteProcSmb (dev, 0x44, (UINT8 *) lrbufData) == EFI_SUCCESS)) {

      Status = SUCCESS;
    }
    break;
  default:
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_76);
  }
  return Status;
}

//---------------------------------------------------------------------------

UINT8
DetectTRRSupport (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch
  )
/*++

  Detect the TRR Mode(s) supported on this channel

  @param Host    - Pointer to sysHost
  @param socket  - Processor socket
  @param ch      - Memory channel

  @retval TRR mode(s) supported

--*/
{
  UINT8               mode;
  UINT8               dimm;
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  dimmNvList = GetDimmNvList (Host, socket, ch);

  //
  // Default both modes enabled
  //
  if (Host->nvram.mem.dramType == SPD_TYPE_DDR4) {
    mode = 0;

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }

      if ((*dimmNvList)[dimm].SPDOptionalFeature & SPD_TRR_IMMUNE) {
        mode |= TRR_IMMUNE;
      } else if ((*dimmNvList)[dimm].SPDOptionalFeature & SPD_MAC_MASK) {
        mode |= TRR_MODE_A;
      } else {
        //
        // This DIMM does not support TRR so disable support for the whole channel
        //
        mode = 0;
        break;
      }
    } // dimm loop

    if ((mode & TRR_IMMUNE) && (mode & TRR_MODE_A)) {
      //
      // Use mode A if mixed with an immune DIMM
      //
      mode &= ~TRR_IMMUNE;
    }
  } else {
    mode = 0;

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }

      if ((*dimmNvList)[dimm].SPDOptionalFeature & SPD_PTRR) {
        mode |= PTRR_MODE;
        break;
      } else if ((*dimmNvList)[dimm].SPDOptionalFeature & SPD_TRR_IMMUNE) {
        mode |= TRR_IMMUNE;
      } else {
        mode = 0;
        break;
      }
    } // dimm loop

    if ((mode & TRR_IMMUNE) && (mode & PTRR_MODE)) {
      //
      // Use pTRR mode if mixed with an immune DIMM
      //
      mode &= ~TRR_IMMUNE;
    }
  }

  return mode;
} // DetectTRRSupport

/**
  Perform a DLL reset

  @param Host            - Pointer to sysHost
  @param socket          - Socket Id

  @retval N/A

**/
VOID
IssueDllReset (
  PSYSHOST  Host,
  UINT8     socket
  )
{
#ifndef DDR5_SUPPORT
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT32              PrevDebugLevel;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct rankDevice   (*rankStruct)[MAX_RANK_DIMM];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);
  PrevDebugLevel = GetDebugLevel ();
  if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
    SetDebugLevel (SDBG_ERROR);
  }

  for (ch = 0; ch < MaxChDdr; ch++) {
    // skip if channel not enabled
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, socket, ch);

    // Loop for each DIMM
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {

      // Skip if no DIMM present
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }

      rankStruct = GetRankStruct (Host, socket, ch, dimm);

      // Loop for each rank
      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {

        // Skip if no rank
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        // MR0
        WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR0, BANK0);

      } // rank loop
    } // dimm loop
  } // ch loop

  FixedDelayMicroSecond (1);

  SetDebugLevel (PrevDebugLevel);
#endif // !DDR5_SUPPORT
}


UINT32
GettCK (
  PSYSHOST Host,
  UINT8    socket
  )
/*++

This function returns the value of tCK

@param Host    - Point to sysHost
@param socket  - Socket number

@retval UINT32 Value of tCK

--*/
{
  //UINT32 BCLK;
  //return ((10000000 + (Host->nvram.mem.socket[socket].ddrFreqMHz - 1)) / Host->nvram.mem.socket[socket].ddrFreqMHz);
  //if (BCLKs[Host->nvram.mem.ratioIndex] = 133){
  //  BCLK = 133333;
  //} else {
  //  BCLK = 100000;
  //}
  return preciseTCK[Host->nvram.mem.ratioIndex];
} // GettCK

/**

  This function returns the value of tREFI x 9

  @param[in] Socket - CPU Socket ID
  @param[in] Ch     - Channel number
  @param[in] tREFI  - Refresh rate

  @retval UINT32 tREFI x 9

**/
UINT32
GettREFIx9 (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT16   tREFI
  )
{
  UINT32  tREFIx9Local;

  tREFIx9Local = (9 * tREFI) / 1024;

  CheckBoundsChip (MemTechDdr, Socket, Ch, tREFIx9, &tREFIx9Local);

  return tREFIx9Local;
} // GettREFIx9

/**

  This function returns the value of tREFI

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tREFI

**/
UINT32
GettREFIDdr4 (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch
  )
{
  UINT32              tREFILocal;
  struct channelNvram (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList (Host, Socket);

  //
  // Calculate tREFI
  // -tREFI for DDR4 is 7.8us (7800ps)
  // -tCK is expressed in 0.1x ps units, requiring a 10000x (10000x10^(-1)ps = 1us) multiplier in the numerator to transform
  //  microseconds (us) to picoseconds (ps) units at the end
  // -In order to deal with the floating point calculation, the refresh rate (twoXRefreshSetPerCh) is expressed in 10x units
  //
  if ((*channelNvList)[Ch].twoXRefreshSetPerCh >= 20) {
    // Assume temp_lo = 0 (14nm) | temp_twoxref = 0 (10nm), so divide requested rate by 2
    tREFILocal = (UINT16)((7800 * 20 * 10000) / ((*channelNvList)[Ch].twoXRefreshSetPerCh * GettCK (Host, Socket)));
  } else {
    // Otherwise use 1x formula
    tREFILocal = (UINT16)((7800 * 10 * 10000) / ((*channelNvList)[Ch].twoXRefreshSetPerCh * GettCK (Host, Socket)));
  }

  CheckBoundsChip (MemTechDdr, Socket, Ch, tREFI, &tREFILocal);

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Refresh rate = %d\n", tREFILocal);

  return tREFILocal;
} // GettREFI

/**

  This function returns the value of tCKE

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - CPU Socket ID
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tCKE

**/
UINT32
GettCKE (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch
  )
{
  UINT32  tCKELocal;
  UINT32  tCKETime;

  tCKETime = 50000; // 5 ns

  tCKELocal = (UINT8) ((tCKETime + (GettCK (Host, Socket) - 100)) / GettCK (Host, Socket));

  //
  // Must be at least 3 clocks
  //
  if (tCKELocal < 3) {
    tCKELocal = 3;
  }

  CheckBoundsChip (MemTechDdr, Socket, Ch, tCKE, &tCKELocal);

  return tCKELocal;
} // GettCKE

/**

  This function returns the value of tXSDLL

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Channel - Channel number

  @retval tXSDLLLocal

**/
UINT32
GettXSDLL (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel
  )
{
  UINT32  tXSDLLLocal = 0;

  if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
    //
    // DDR5 Full Spec Draft Rev0.99
    //
    if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_3200) {
      tXSDLLLocal = 1024;
    } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_3600) {
      tXSDLLLocal = 1024;
    } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_4000) {
      tXSDLLLocal = 1280;
    } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_4400) {
      tXSDLLLocal = 1280;
    } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_4800) {
      tXSDLLLocal = 1536;
    } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_5200) {
      tXSDLLLocal = 1536;
    } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_5600) {
      tXSDLLLocal = 1792;
    } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_6000) {
      tXSDLLLocal = 1792;
    } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_6400) {
      tXSDLLLocal = 2048;
    }
  } else {
    //
    // DDR4 Full spec update(79-4A)
    //
    if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_1866) {
      tXSDLLLocal = 597;
    } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_2400){
      tXSDLLLocal = 768;
    } else {
      tXSDLLLocal = 1024;
    }
  }

  CheckBoundsChip (MemTechDdr, Socket, Channel, tXSDLL, &tXSDLLLocal);

  return tXSDLLLocal;
} // GettXSDLL

/**

  This function returns the value of tXSOFFSET

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tXSOFFSET

**/
UINT32
GettXSOFFSET (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch
  )
{
  UINT32  tXSOFFSETLocal;

  tXSOFFSETLocal = (100000 + (GettCK (Host, Socket) - 100)) / GettCK (Host, Socket);

  CheckBoundsChip (MemTechDdr, Socket, Ch, tXSOFFSET, &tXSOFFSETLocal);

  return tXSOFFSETLocal;
} // GettXSOFFSET

/**

  This function returns the value of tXP

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tXP

**/
UINT32
GettXP (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch
  )
{
  UINT32  tXPLocal;
  UINT32  tXPTime;

  tXPTime = 60000; // 6 ns
  tXPLocal = (tXPTime + (GettCK (Host, Socket) - 100)) / GettCK (Host, Socket);
  //
  // Must be at least 3 clocks
  //
  if (tXPLocal < 3) {
    tXPLocal = 3;
  }

  CheckBoundsChip (MemTechDdr, Socket, Ch, tXP, &tXPLocal);

  return tXPLocal;
} // GettXP

/**

This function returns the value of tCKSRX

@param[in] Host    - Point to sysHost
@param[in] Socket  - Socket Id

@retval UINT32 Value of tCKSRX

**/
UINT32
EFIAPI
GettCKSRX (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  UINT32 Tcksrx;

  if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
    //
    // DDR5 tCKSRKX should be max(3.5ns, 8nCK)
    //
    Tcksrx = MAX (PicosecondToClocks (Socket, tCKSRKX_MIN_PS), tCKSRKX_MIN_DCLK);
  } else {
    Tcksrx = (100000 + (GettCK (Host, Socket) - 100)) / GettCK (Host, Socket);
  }
  return Tcksrx;
} // GettCKSRX


//
// DDR5 tSTAB should be 3.5us for RDIMM or 4.14us for LRDIMM
//
#define DDR5_LRDIMM_TSTAB_NS 4140
#define DDR5_RDIMM_TSTAB_NS  3500

/**

  This function returns the value of tSTAB

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tSTAB

**/
UINT32
GettSTAB (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch
  )
{
  UINT32  tSTABLocal;
  if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
    //
    // DDR5 tSTAB should be 3.5us for RDIMM or 4.14us for LRDIMM
    //
    if (IsLrdimmPresentOnSocket (Socket)) {
      tSTABLocal = NanosecondToClocks (Socket, DDR5_LRDIMM_TSTAB_NS);
    } else {
      tSTABLocal = NanosecondToClocks (Socket, DDR5_RDIMM_TSTAB_NS);
    }

  } else {

    tSTABLocal = (50000000 + (GettCK (Host, Socket) - 100)) / GettCK (Host, Socket);
  }

  CheckBoundsChip (MemTechDdr, Socket, Ch, tSTAB, &tSTABLocal);

  return tSTABLocal;
} // GettSTAB

/**

  This function returns the value of tMOD

  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tMOD

**/
UINT32
GettMOD (
  IN UINT8    Socket,
  IN UINT8    Ch
  )
{
  UINT32 tMODLocal;

  tMODLocal = 24;

  if (tMODLocal < 12) {
    tMODLocal = 12;
  }

  CheckBoundsChip (MemTechDdr, Socket, Ch, tMOD, &tMODLocal);

  return tMODLocal;
} // GettMOD

/*

  Return tFAW in number of cycles for DDR4 memory technologies

  @param[in] Socket     Current socket under test (0-based)
  @param[in] Channel    Current channel within the socket under test (0-based)
  @param[in] PageSize   DDR4 valid options: PAGE_SIZE_1K, PAGE_SIZE_2K & PAGE_SIZE_HALFK

  @return     tFAW in number of cycles
*/
UINT32
GettFawDdr4 (
  IN    UINT8    Socket,
  IN    UINT8    Channel,
  IN    UINT8    PageSize
  )
{
  PSYSHOST  Host;
  UINT32    tFAWLocal;
  UINT32    tFAWTime;

  Host = GetSysHostPointer ();

  if (PageSize == PAGE_SIZE_HALFK) {
    if (Host->nvram.mem.socket[Socket].ddrFreqMHz <= 800) {
      tFAWTime = 200000;
    } else if (Host->nvram.mem.socket[Socket].ddrFreqMHz < 1066) {
      tFAWTime = 170000;
    } else if (Host->nvram.mem.socket[Socket].ddrFreqMHz < 1200) {
      tFAWTime = 150000;
    } else if (Host->nvram.mem.socket[Socket].ddrFreqMHz < 1333) {
      tFAWTime = 130000;
    } else if (Host->nvram.mem.socket[Socket].ddrFreqMHz < 1466) {
      tFAWTime = 120000;
    } else if (Host->nvram.mem.socket[Socket].ddrFreqMHz < 1600) {
      tFAWTime = 102400;
    } else {
      tFAWTime = 100000;
    }
  } else if (PageSize == PAGE_SIZE_1K) {
    if (Host->nvram.mem.socket[Socket].ddrFreqMHz <= 800) {
      tFAWTime = 250000;
    } else if (Host->nvram.mem.socket[Socket].ddrFreqMHz < 1066) {
      tFAWTime = 230000;
    } else {
      tFAWTime = 210000;
    }
  } else {
    if (Host->nvram.mem.socket[Socket].ddrFreqMHz <= 800) {
      tFAWTime = 350000;
    } else {
      tFAWTime = 300000;
    }
  }

  tFAWLocal = (UINT8) ((tFAWTime + (GettCK (Host, Socket) - 100)) / GettCK (Host, Socket));

  if (PageSize == PAGE_SIZE_HALFK) {
    if (tFAWLocal < 16) {
      tFAWLocal = 16;
    }
  } else if (PageSize == PAGE_SIZE_1K) {
    if (tFAWLocal < 20) {
      tFAWLocal = 20;
    }
  } else {
    if (tFAWLocal < 28) {
      tFAWLocal = 28;
    }
  }

  return tFAWLocal;

}

/*

  Return tFAW in number of cycles for DDR5 memory technologies

  @param[in] Socket     Current socket under test (0-based)
  @param[in] Channel    Current channel within the socket under test (0-based)
  @param[in] PageSize   DDR5 valid options: PAGE_SIZE_1K & PAGE_SIZE_2K

  @return     tFAW in number of cycles
*/
UINT32
GettFawDdr5 (
  IN    UINT8    Socket,
  IN    UINT8    Channel,
  IN    UINT8    PageSize
  )
{
  PSYSHOST  Host;
  UINT32    tFAWLocal;
  UINT32    tFAWTime;
  UINT32    FrequencyTest;
  UINT32    *TablePointer;
  UINT8     TableIndex;
  UINT8     TableSize;
  //
  // GetTck uses precise TCK in terms of 100fs units,
  // so to denote a timing of 25ns --> 25000ps / 0.1ps  = 250000
  // Implemented as an array in stack so it does not take unnecessary code space (as Global array)
  // Frequencies above o equal to 6800M are just place holders for 1K and 2K tables
  //                           3200M,  3600M,  4000M,  4400M,  4800M,  5200M,  5600M,  6000M,  6400M,  6800M,  7200M,  7600M,  8000M,  8400M
  UINT32  Ddr5tFawPage2K[] = {250000, 222200, 200000, 181800, 166600, 153800, 142800, 133300, 125000, 100000, 100000, 100000, 100000, 100000};
  UINT32  Ddr5tFawPage1K[] = {200000, 177700, 160000, 145400, 133300, 123000, 114200, 106600, 100000, 100000, 100000, 100000, 100000, 100000};

  Host = GetSysHostPointer ();

  if (PageSize == PAGE_SIZE_2K) {
    TablePointer  = Ddr5tFawPage2K;
    TableSize     = sizeof (Ddr5tFawPage2K) / sizeof (UINT32);
  } else {
    TablePointer  = Ddr5tFawPage1K;
    TableSize     = sizeof (Ddr5tFawPage1K) / sizeof (UINT32);
  }

  tFAWTime = 0;

  for (TableIndex = 0, FrequencyTest = 1600; TableIndex < TableSize; TableIndex++, FrequencyTest += 200) {
    if (Host->nvram.mem.socket[Socket].ddrFreqMHz <= FrequencyTest) {
      tFAWTime = TablePointer[TableIndex];
      break;
    }
  }

  tFAWLocal = (UINT8) ((tFAWTime + (GettCK (Host, Socket) - 100)) / GettCK (Host, Socket));

  if (PageSize == PAGE_SIZE_1K) {
    if (tFAWLocal < 32) {
      tFAWLocal = 32;
    }
  } else {
    if (tFAWLocal < 40) {
      tFAWLocal = 40;
    }
  }


  return tFAWLocal;
}

/**

  This function returns the value of tFAW

  @param[in] Host      - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket    - Socket Id
  @param[in] Ch        - Channel number
  @param[in] PageSize  - Page size

  @retval UINT32 Value of tFAW

**/
UINT32
GettFAW (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    PageSize
  )
{
  UINT32  tFawInCycles;

  if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
    tFawInCycles = GettFawDdr5 (Socket, Ch, PageSize);
  } else {
    tFawInCycles = GettFawDdr4 (Socket, Ch, PageSize);
  }

  CheckBoundsChip (MemTechDdr, Socket, Ch, tFAW, &tFawInCycles);

  return tFawInCycles;
} // GettFAW

/**

  This function returns the value of tRTP

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Channel - Channel number

  @retval UINT32 Value of tRTP

**/
UINT32
GettRTP (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel
  )
{
  UINT8  TimingMode;
  UINT32 tRTPLocal;
  struct channelNvram (*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);

  tRTPLocal = (75000 + (GettCK (Host, Socket) - 1)) / GettCK (Host, Socket);

  if (Host->nvram.mem.dramType != SPD_TYPE_DDR5) {
    if (tRTPLocal < 4) {
      tRTPLocal = 4;
    }
  } else {
    switch ((*ChannelNvList) [Channel].timingMode) {
      case TIMING_1N:
        TimingMode = 1;
        break;
      case TIMING_2N:
        TimingMode = 2;
        break;
      default:
        TimingMode = 0;
        break;
    }
    tRTPLocal += TimingMode;
  }

  CheckBoundsChip (MemTechDdr, Socket, Channel, tRTP, &tRTPLocal);

  return tRTPLocal;
} // GettRTP

/**

  This function returns the value of tWR

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tWR

**/
UINT32
GettWR (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch
  )
{
  UINT32 tWRLocal;

  tWRLocal = 15;
  if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
    tWRLocal = 30;
  }

  tWRLocal = NanosecondToClocks (Socket, tWRLocal);

  CheckBoundsChip (MemTechDdr, Socket, Ch, tWR, &tWRLocal);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "tWRLocal = %d\n", tWRLocal);

  return tWRLocal;
} // GettWR

/**

  This function returns the value of tRFC

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tRFC in units of DCLKs (nCK)

**/
UINT32
GettRFC (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  UINT32              tRFCLocal;
  struct channelNvram (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList (Host, Socket);
  tRFCLocal = (*channelNvList)[Ch].common.nRFC;

  CheckBoundsChip (MemTechDdr, Socket, Ch, tRFC, &tRFCLocal);

  return tRFCLocal;
} // GettRFC

/**

  This function returns the value of tRFC_dlr1 in ddr clock cycles for 3DS dimms

  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT16 Value of tRFC_dlr1 in units of DCLK

**/
UINT16
EFIAPI
GetTrfcDlr1 (
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  PSYSHOST            Host;
  UINT16              CommonTrfcDlrLocal = 0xFFFF;
  UINT16              Cycles;
  UINT8               Dimm;
  UINT8               Density;
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
                      //Note: the lookup table below assumes values are for
                      // 4Gb, 8Gb, and 16Gb densities in that order
                      //Values here come from JEDEC Standard Addendum No. 1 to JESD79-4
                      // 3D Stacked DRAM
                      // Table 44 - Refresh parameters by logical rank density
  UINT16              TrfcDlr1LookupTable [SPD_16Gb - SPD_4Gb + 1] = { 90, 120, 190};

  Host = (PSYSHOST) GetSysHostPointer ();
  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }
    if (((*DimmNvList)[Dimm].SPDDeviceType & SPD_SIGNAL_LOADING) != SPD_3DS_TYPE) {
      continue;
    }
    // Here we have a present 3DS dimm, so find the logical rank size
    Density = (*DimmNvList)[Dimm].sdramCapacity;
    // We only have values for 4Gb, 8Gb, and 16Gb densities
    if ((Density >= SPD_4Gb) && (Density <= SPD_16Gb)) {
      //Convert the time value to cycles rounded up
      Cycles = NanosecondToClocks (Socket, TrfcDlr1LookupTable[(Density - SPD_4Gb)]);
      //Find the smallest number of cycles for the channel
      if (Cycles < CommonTrfcDlrLocal) {
        CommonTrfcDlrLocal = Cycles;
      }
    }
  }

  return CommonTrfcDlrLocal;
} // GetTrfcDlr1

/**

  This function returns the value of tRAS

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tRAS

**/
UINT32
GettRAS (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel
  )
{
  UINT8               TimingMode;
  UINT32              tRASLocal;
  struct channelNvram (*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);
  tRASLocal = MAX (NanosecondToClocks (Socket, tRAS_MIN_NS), (*ChannelNvList)[Channel].common.nRAS);

  if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
    switch ((*ChannelNvList) [Channel].timingMode) {
      case TIMING_1N:
        TimingMode = 1;
        break;
      case TIMING_2N:
        TimingMode = 2;
        break;
      default:
        TimingMode = 0;
        break;
    }
    tRASLocal += TimingMode;
  }

  CheckBoundsChip (MemTechDdr, Socket, Channel, tRAS, &tRASLocal);

  return tRASLocal;
} // GettRAS

/**

  This function returns the value of tCWL

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tCWL

**/
UINT32
GettCWL (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  UINT32              tCWLLocal;
  struct channelNvram (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList (Host, Socket);
  tCWLLocal = (*channelNvList)[Ch].common.nWL;

  CheckBoundsChip (MemTechDdr, Socket, Ch, tCWL, &tCWLLocal);

  return tCWLLocal;
} // GettWL

/**

  This function returns the value of tCL

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tCL

**/
UINT32
GettCL (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  UINT32              tCLLocal;
  struct channelNvram (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList (Host, Socket);
  tCLLocal = (*channelNvList)[Ch].common.nCL;

  CheckBoundsChip (MemTechDdr, Socket, Ch, tCL, &tCLLocal);

  return tCLLocal;
} // GettCL

/**

  This function returns the value of tRP

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tRP

**/
UINT32
GettRP (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  UINT32              tRPLocal;
  struct channelNvram (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList (Host, Socket);
  tRPLocal = (*channelNvList)[Ch].common.nRP;

  CheckBoundsChip (MemTechDdr, Socket, Ch, tRP, &tRPLocal);

  return tRPLocal;
} // GettRP

/**

  This function returns the value of tRC

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tRC

**/
UINT32
GettRC (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  UINT32              tRCLocal;
  struct channelNvram (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList (Host, Socket);
  tRCLocal = (*channelNvList)[Ch].common.nRC;

  CheckBoundsChip (MemTechDdr, Socket, Ch, tRC, &tRCLocal);

  return tRCLocal;
} // GettRC

/**

  This function returns the value of tRP

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tRCD

**/
UINT32
GettRCD (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  UINT32              tRCDLocal = 0;
  struct channelNvram (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList (Host, Socket);
  tRCDLocal = (*channelNvList)[Ch].common.nRCD;

  CheckBoundsChip (MemTechDdr, Socket, Ch, tRCD, &tRCDLocal);

  return tRCDLocal;
} // GettRCD

/**

  This function returns the value of tRRD_L

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Sh      - Channel number

  @retval UINT32 Value of tRRD_L

**/
UINT32
GettRRD_L (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  UINT32              tRRD_LLocal;
  struct channelNvram (*channelNvList)[MAX_CH];

  if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
    //
    // GetTck uses precise TCK in terms of 100fs units,
    // so to denote a timing of 5ns --> 5000ps / 0.1ps  = 50000
    //
    tRRD_LLocal = (UINT8) ((50000 + (GettCK (Host, Socket) - 100)) / GettCK (Host, Socket));

    if (tRRD_LLocal < 8) {
      tRRD_LLocal = 8;
    }
  } else {
    channelNvList = GetChannelNvList (Host, Socket);
    tRRD_LLocal = (*channelNvList)[Ch].common.nRRD_L;
  }

  CheckBoundsChip (MemTechDdr, Socket, Ch, tRRD_L, &tRRD_LLocal);

  return tRRD_LLocal;
} // GettRRD_L

/**

  This function returns the value of tRRD

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tRRD

**/
UINT32
GettRRD (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  UINT32              tRRDLocal;
  struct channelNvram (*channelNvList)[MAX_CH];

  if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
    tRRDLocal = TRRD_S_DEFINE_DDR5;
  } else {
    channelNvList = GetChannelNvList (Host, Socket);
    tRRDLocal = (*channelNvList)[Ch].common.nRRD;
  }

  CheckBoundsChip (MemTechDdr, Socket, Ch, tRRD, &tRRDLocal);

  return tRRDLocal;
} // GettRRD

/**

  Calculates and returns tCCD_L
  tCCD_L is defined in the JEDEC specification as the minimum CAS to CAS Delay
  between back to back read commands to the same bank group.

  @param[in] Socket  - Socket number
  @param[in] Ch      - Channel number

  @retval tCCD (in units of DCLK cycles)

**/
UINT8
GetTccd_L (
  IN    UINT8     Socket,
  IN    UINT8     Ch
  )
{
  PSYSHOST  Host;
  UINT32    TccdLocal = 4;
  UINT16    TccdTime;
  struct    channelNvram (*ChannelNvList)[MAX_CH];

  Host = GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  // Enforce tCCD_L based on freq
  if (Host->nvram.mem.socket[Socket].ddrFreqMHz <= 800) {
    TccdTime = 62500; // 6.25 ns
  } else if (Host->nvram.mem.socket[Socket].ddrFreqMHz < 1200) {
    TccdTime = 53550; // 5.355 ns
  } else {
    TccdTime = 50000; // 5 ns
  }
  TccdLocal = (UINT8)((TccdTime + (GettCK (Host, Socket) - 100)) / GettCK (Host, Socket));
  TccdLocal = TccdLocal + (*ChannelNvList)[Ch].tCCDAdder;

  //
  // Jedec DDR4 Spec section 4.20 - With 2tck read/write preamble, tCCD of 5 is not allowed.
  //
  if (GetPreambleState (Socket, READ_OR_WRITE_PREAMBLE) == PREAMBLE_2TCLK) {
    if (TccdLocal == 5) {
      TccdLocal = 6;
    }
  }

  CheckBoundsChip (MemTechDdr, Socket, Ch, tCCD_L, &TccdLocal);

#ifdef DDR5_SUPPORT
  if (IsDdr5Present (Host, Socket)) {
    // Max (8nCK, 5ns)
    TccdTime = 50000; // 5 ns
    TccdLocal = (UINT8)((TccdTime + (GettCK (Host, Socket) - 100)) / GettCK (Host, Socket));
    TccdLocal = MAX (8, TccdLocal);
  }
#endif
  return (UINT8)TccdLocal;
} // GetTccd_L

/**

  Calculates and returns tCCD_S_WR
  tCCD_WR_S is defined in the JEDEC specification as WRITE CAS_n to WRITE CAS_n
  command delay for different bank group

  @param[in] Socket  - Socket number
  @param[in] Ch      - Channel number

  @retval tCCD_WR_S (in units of DCLK cycles)

**/
UINT8
EFIAPI
GetTccdWr_S (
  IN    UINT8     Socket,
  IN    UINT8     Ch
  )
{
  return GetWriteBurstLength (Ch) / 2;
}

/**
  Calculates and returns tCCD_L_WR
  tCCD_WR_L is defined in the JEDEC specification as WRITE CAS_n to WRITE CAS_n
  command delay for same bank group

  @param[in] Socket  - Socket number
  @param[in] Ch      - Channel number

  @retval tCCD_WR_L (in units of DCLK cycles)

**/
UINT8
EFIAPI
GetTccdWr_L (
  IN    UINT8     Socket,
  IN    UINT8     Ch
  )
{
  UINT32              TccdWrLLocal;
#ifdef DDR5_SUPPORT
  PSYSHOST            Host;
  UINT32              TccdWrLTime;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  SYS_SETUP           *Setup;

  Host = GetSysHostPointer ();
  Setup = GetSysSetupPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // JEDEC DDR5 Full Spec - tCCD_L = Max (32nCK, 20ns)
  //
  TccdWrLTime = 200000; // 20 ns in units of .1ps
  TccdWrLLocal = (UINT8)((TccdWrLTime + (GettCK (Host, Socket) - 100)) / GettCK (Host, Socket));
  TccdWrLLocal = MAX (32, TccdWrLLocal);
  if ((Setup->mem.optionsExt & WR_CRC) != 0) {
    //
    // WrCrc adds 1 DCLK to the write burst length
    //
    TccdWrLLocal += 1;
  }
#else // DDR5_SUPPORT
  TccdWrLLocal = GetTccd_L (Socket, Ch);
#endif // DDR5_SUPPORT

  return (UINT8)TccdWrLLocal;
}

/**

  This function returns the value of tWTR_S

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tWTR_S

**/
UINT32
GettWTR_S (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch
  )
{
  UINT32 tWTR_SLocal;

  tWTR_SLocal = (25000 + (GettCK(Host, Socket) - 100)) / GettCK(Host, Socket);

  CheckBoundsChip (MemTechDdr, Socket, Ch, tWTR_S, &tWTR_SLocal);

  return tWTR_SLocal;
} // GettWTR_S

/**

This function returns the value of tWTR_L

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tWTR_L

**/
UINT32
GettWTR_L (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  UINT32 tWTR_LLocal;

  tWTR_LLocal = 75000;
  if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
    tWTR_LLocal = 100000;
  }

  tWTR_LLocal = (tWTR_LLocal + (GettCK (Host, Socket) - 100)) / GettCK (Host, Socket);

  if ((Host->nvram.mem.dramType == SPD_TYPE_DDR4) &&
      (tWTR_LLocal < 4)) {
    tWTR_LLocal = 4;
  }

  CheckBoundsChip (MemTechDdr, Socket, Ch, tWTR, &tWTR_LLocal);

  return tWTR_LLocal;
} // GettWTR_L

/**
  This function returns the value of tPRPDEN

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tWTR_L

**/
UINT32
GettPRPDEN (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  UINT32              tPRPDEN_local;

  tPRPDEN_local = 1;

  if (Host->nvram.mem.socket[Socket].ddrFreq >= DDR_2133) {
    tPRPDEN_local = 2;
  }
  CheckBoundsChip (MemTechDdr, Socket, Ch, tPRPDEN, &tPRPDEN_local);

  return tPRPDEN_local;
} // GettPRPDEN

/**

  This function returns the value of tRDA (RdCAS_AP to ACT)

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tRDA

**/
UINT32
GettRDA (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  UINT32                tRDA_Local;
  struct channelNvram   (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(Host, Socket);

  tRDA_Local = (*channelNvList)[Ch].common.nRP + (*channelNvList)[Ch].common.nRTP;

  CheckBoundsChip (MemTechDdr, Socket, Ch, tRDA, &tRDA_Local);

  return tRDA_Local;
} // GettRDA

/**

  This function returns the value of tWRA (WrCAS_AP to ACT)

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tWRA

**/
UINT32
GettWRA (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  UINT32                tWRA_Local;
  UINT8                 WriteBL;
  struct channelNvram   (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList (Host, Socket);
  WriteBL = GetWriteBurstLength (Ch);

  tWRA_Local = (*channelNvList)[Ch].common.nWL + WriteBL / 2 + (*channelNvList)[Ch].common.nWR + (*channelNvList)[Ch].common.nRP;

  CheckBoundsChip (MemTechDdr, Socket, Ch, tWRA, &tWRA_Local);

  return tWRA_Local;
} // GettWRA

/**

  This function returns the value of tWRPRE (WrCAS to PRE)

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tWRPRE

**/
UINT32
GettWRPRE (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  UINT32                tWRPRE_Local;
  UINT8                 WriteBL;
  struct channelNvram   (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList (Host, Socket);
  WriteBL = GetWriteBurstLength (Ch);

  tWRPRE_Local = (*channelNvList)[Ch].common.nWL + WriteBL / 2 + (*channelNvList)[Ch].common.nWR;

  CheckBoundsChip (MemTechDdr, Socket, Ch, tWRPRE, &tWRPRE_Local);

  return tWRPRE_Local;
} // GettWRPRE

/**

  This function returns the value of tWRRDA
  (Write CAS to Read CAS with AutoPrecharge)

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tWRRDA

**/
UINT32
GettWRRDA (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  UINT32                tWRRDA_Local;
  UINT8                 WriteBL;
  struct channelNvram   (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList (Host, Socket);
  WriteBL = GetWriteBurstLength (Ch);

  tWRRDA_Local = (*channelNvList)[Ch].common.nWL + WriteBL / 2 +
       MAX ((*channelNvList)[Ch].common.nWR - (*channelNvList)[Ch].common.nRTP, (*channelNvList)[Ch].common.nWTR_L);

  CheckBoundsChip (MemTechDdr, Socket, Ch, tWRRDA, &tWRRDA_Local);

  return tWRRDA_Local;
} // GettWRRDA

/**

  This function returns the value of tRDPDEN (RdCAS to CKE low time)

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tRDPDEN

**/
UINT32
GettRDPDEN (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  UINT8                 ReadBL;
  UINT8                 timingMode;
  UINT32                tRDPDEN_Local;
  struct channelNvram   (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList (Host, Socket);
  ReadBL = GetReadBurstLength ();

  // Get 1-based CMD timing mode
  switch ((*channelNvList)[Ch].timingMode) {
  case TIMING_1N:
    timingMode = 1;
    break;
  case TIMING_2N:
    timingMode = 2;
    break;
  case TIMING_3N:
  default:
    timingMode = 3;
    break;
  }

  //RdCAS to CKE low time : tCL + BL/2 + N (1N = 1, 2N = 2, 3N = 3)
  tRDPDEN_Local = (*channelNvList)[Ch].common.nCL + ReadBL / 2 + timingMode;

  CheckBoundsChip (MemTechDdr, Socket, Ch, tRDPDEN, &tRDPDEN_Local);

  return tRDPDEN_Local;
} // GettRDPDEN

/**

  This function returns the value of tWRPDEN (WrCAS to CKE low time)

  @param[in] Host    - Point to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of tWRPDEN

**/
UINT32
GettWRPDEN (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  UINT32                tWRPDEN_Local;
  UINT8                 WriteBL;
  struct channelNvram   (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList (Host, Socket);
  WriteBL = GetWriteBurstLength (Ch);

  // WrCAS to CKE low time : tWL + BL/2 + tWR
  tWRPDEN_Local = (*channelNvList)[Ch].common.nWL + WriteBL / 2 + (*channelNvList)[Ch].common.nWR;

  CheckBoundsChip (MemTechDdr, Socket, Ch, tWRPDEN, &tWRPDEN_Local);

  return tWRPDEN_Local;
} // GettWRPDEN


/**
  Determine if the given memory subsystem is DDR type

  @param[in]      MsmListPtr          - Pointer to MemSs structure

  @retVal 1 for DDR type
  @retVal 0 for other type
**/
BOOLEAN
IsMemSsDdrType (
  IN      MRC_MSM       *MsmListPtr
  )
{
  if ((MsmListPtr->MemSsType == Ddr4MemSs)
#ifdef DDR5_SUPPORT
      || (MsmListPtr->MemSsType == Ddr5MemSs)
#endif  // DDR5_SUPPORT
      ) {
    return 1;
  }
  return 0;
} // IsMemSsDdrType

/**
  Determine the maximum number of channels of MemSsType in given socket

  @param[in]      Host          Pointer to sysHost structure
  @param[in]      Socket        Processor socket
  @param[in]      MemSsType     Number of entries in MemSsList

  @retval UINT32  Number of channels of MemSsType in given socket
**/
UINT32
GetMaxCh (
  IN      PSYSHOST      Host,
  IN      UINT8         Socket,
  IN      MRC_MST       MemSsType
  )
{
  UINT32 MaxCh = 0;
  UINT32 MemSs;
  MRC_MSM *MsmListPtr;
  UINT32 MsmListSize;

  MsmListPtr = &Host->var.mem.socket[Socket].MemSsList[0];
  MsmListSize = Host->var.mem.socket[Socket].NumMemSs;

  for (MemSs = 0; MemSs < MsmListSize; MemSs++) {
    if (MsmListPtr[MemSs].MemSsType == MemSsType) {
      MaxCh += MsmListPtr[MemSs].NumDataCh;
    }
  }
  return MaxCh;
} // GetMaxCh

/**
  Determine the maximum number of strobes per DDR channel in current socket

  @param[in]      Host          Pointer to sysHost structure

  @retval UINT32  Number of strobes per DDR channel
**/
UINT8
GetMaxStrobeValid (
  IN      PSYSHOST        Host
  )
{
  UINT32 Socket = Host->var.mem.currentSocket;
  UINT32 MemSs;
  MRC_MSM *MsmListPtr;
  UINT32 MsmListSize;

  MsmListPtr = &Host->var.mem.socket[Socket].MemSsList[0];
  MsmListSize = Host->var.mem.socket[Socket].NumMemSs;

  for (MemSs = 0; MemSs < MsmListSize; MemSs++) {
    if (IsMemSsDdrType (&MsmListPtr[MemSs])) {
      return (UINT8) (MsmListPtr[MemSs].NumDataIoFubsPerCh);
    }
  }
  RC_FATAL_ERROR (FALSE, ERR_MRC_STRUCT, RC_FATAL_ERROR_MINOR_CODE_0);
  return 0;
} // GetMaxStrobeValid

/**
  Determine the maximum number of strobes per DDR subchannel in current Socket for DDR5.

  @param[in]      Host          Pointer to sysHost structure

  @retval UINT32  Number of strobes per DDR subchannel
**/
UINT8
GetSubChMaxStrobeValid (
  IN PSYSHOST        Host
  )
{
  UINT32 Socket = Host->var.mem.currentSocket;
  UINT32 MemSs;
  MRC_MSM *MsmListPtr;
  UINT32 MsmListSize;

  MsmListPtr = &Host->var.mem.socket[Socket].MemSsList[0];
  MsmListSize = Host->var.mem.socket[Socket].NumMemSs;

  for (MemSs = 0; MemSs < MsmListSize; MemSs++) {
    if (IsMemSsDdrType (&MsmListPtr[MemSs])) {
      return (UINT8) (MsmListPtr[MemSs].NumDataIoFubsPerSubCh);
    }
  }
  RC_FATAL_ERROR (FALSE, ERR_MRC_STRUCT, RC_FATAL_ERROR_MINOR_CODE_0);
  return 0;
} // GetSubChMaxStrobeValid

/**
  Determine the maximum number of DQ bits supported per DDR channel in current socket

  @param[in]      Host          - Pointer to sysHost

  @retVal UINT8   Number of DQ bits per DDR channel
**/
UINT8
GetMaxBitsValid (
  IN      PSYSHOST          Host
  )
{
  UINT32 Socket;
  UINT32 MemSs;
  MRC_MSM *MsmListPtr;
  UINT32 MsmListSize;

  Socket = GetCurrentSocketId ();

  MsmListPtr = &Host->var.mem.socket[Socket].MemSsList[0];
  MsmListSize = Host->var.mem.socket[Socket].NumMemSs;

  for (MemSs = 0; MemSs < MsmListSize; MemSs++) {
    if (IsMemSsDdrType (&MsmListPtr[MemSs])) {
      return (UINT8) (MsmListPtr[MemSs].NumDqLanesPerCh);
    }
  }
  RC_FATAL_ERROR (FALSE, ERR_MRC_STRUCT, RC_FATAL_ERROR_MINOR_CODE_0);
  return 0;
} // GetMaxBitsValid

/**
  Determine if the given Strobe number is valid for ECC mode on current socket

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Strobe        - Strobe number to check

  @retVal   1     Strobe number is not valid
  @retVal   0     Strobe number is valid
**/
BOOLEAN
IsStrobeNotValid (
  IN      PSYSHOST        Host,
  IN      UINT8           Strobe
  )
{
  UINT8 MSVx4 = GetMaxStrobeValid (Host);

  if ((Strobe >= MSVx4) ||
      ((!Host->nvram.mem.eccEn) && ((Strobe == (MSVx4/2 - 1)) || (Strobe == (MSVx4 - 1))))) {
    return 1;
  }
  return 0;
} // IsStrobeNotValid

/**
  Determine if the given Strobe number is an ECC Strobe

  @param[in]                Host          Pointer to sysHost
  @param[in]                Strobe        Strobe number to check

  @retVal                   TRUE          Strobe is ECC
  @retVal                   FALSE         Strobe is not ECC

**/
BOOLEAN
IsEccStrobeDdr4 (
  IN        PSYSHOST        Host,
  IN        UINT8           Strobe
  )
{
  UINT8 MSVx4;

  MSVx4 = GetMaxStrobeValid (Host);

  if ((Strobe == (MSVx4 / 2 - 1) || Strobe == (MSVx4 - 1))) {
    return TRUE;
  }
  return FALSE;
} // IsEccStrobeDdr4

/**
  Determine if the given Strobe number is an ECC Strobe for Ddr5

  @param[in]                Host          Pointer to sysHost
  @param[in]                Strobe        Strobe number to check

  @retVal                   TRUE          Strobe is ECC
  @retVal                   FALSE         Strobe is not ECC

**/
BOOLEAN
IsEccStrobeDdr5 (
  IN        PSYSHOST        Host,
  IN        UINT8           Strobe
  )
{
  UINT8 MSVx4;

  MSVx4 = MAX_STROBE_DDR5;

  if (Strobe == (MSVx4 / 4 - 1) ||
      Strobe == (MSVx4 / 2 - 1) ||
      Strobe == ((MSVx4 / 4 - 1) + MSVx4 / 2) ||
      Strobe == ((MSVx4 / 2 - 1) + MSVx4 / 2)) {
    return TRUE;
  }

  return FALSE;
} // IsEccStrobeDdr5

/**
  Determine if the given Strobe number is an ECC Strobe for Ddr5 or Ddr4

  @param[in]                Host          Pointer to sysHost
  @param[in]                Strobe        Strobe number to check

  @retVal                   TRUE          Strobe is ECC
  @retVal                   FALSE         Strobe is not ECC

**/
BOOLEAN
IsEccStrobe (
  IN        PSYSHOST        Host,
  IN        UINT8           Strobe
)
{
  BOOLEAN EccStrobe;
  DDR_TECHNOLOGY_TYPE DdrType;

  DdrType = Ddr4Type;
#ifdef DDR5_SUPPORT
  DdrType = Ddr5Type;
#endif  // DDR5_SUPPORT

  EccStrobe = FALSE;

  if (DdrType == Ddr5Type) {
    return IsEccStrobeDdr5 (Host, Strobe);
  } else {
    return IsEccStrobeDdr4 (Host, Strobe);
  }
}

/**
  Determine if the given Dimm and Strobe number is valid for ECC mode on current socket for DDR5.

  @param[in]  DimmNvramStructPtr  Pointer to dimmNvram structure to check
  @param[in]  Strobe              Strobe number to check

  @retVal   TRUE    Strobe number is not valid
  @retVal   FALSE   Strobe number is valid
**/
BOOLEAN
EFIAPI
IsDimmStrobeNotValid (
  IN struct dimmNvram *DimmNvramStructPtr,
  IN UINT8            Strobe
  )
{
  SYSHOST   *Host;
  UINT8     MSVx4;
  UINT8     MSVx8;

  Host = GetSysHostPointer ();
  MSVx4 = GetMaxStrobeValid (Host);
  MSVx8 = MSVx4 / 2;

  if ((*DimmNvramStructPtr).x4Present) {
    if (Strobe >= MSVx4) {
      return TRUE;
    } else if ((!Host->nvram.mem.eccEn) && ((Strobe == (MSVx8 - 1)) || (Strobe == (MSVx4 - 1)))) {
      return TRUE;
    }
  } else {
    if (Strobe >= MSVx8) {
      return TRUE;
    } else if ((!Host->nvram.mem.eccEn) && (Strobe == (MSVx8 - 1))) {
      return TRUE;
    }
  }

  return FALSE;
} // IsDimmStrobeNotValid

/**
  Determine if the given DQ nibble number is valid for ECC mode on current socket

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Nibble        - DQ nibble number to check

  @retVal   1     DQ nibble number is not valid
  @retVal   0     DQ nibble number is valid
**/
BOOLEAN
IsNibbleNotValid (
  IN      PSYSHOST        Host,
  IN      UINT8           Nibble
  )
{
  UINT8 MaxNibble;

  if (!Host->nvram.mem.eccEn) {
    MaxNibble = GetMaxStrobeValid (Host) - 2;
  } else {
    MaxNibble = GetMaxStrobeValid (Host);
  }
  if (Nibble >= MaxNibble) {
    return 1;
  }
  return 0;
} // IsNibbleNotValid

/**
  Determine if the given DQ bit number is valid for ECC mode on current socket

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Bit           - DQ bit number to check

  @retVal   1     DQ bit number is not valid
  @retVal   0     DQ bit number is valid
**/
BOOLEAN
IsBitNotValid (
  IN      PSYSHOST        Host,
  IN      UINT8           Bit
  )
{
  UINT8 MaxBit;

  if (!Host->nvram.mem.eccEn) {
    MaxBit = (GetMaxStrobeValid (Host) - 2) * BITS_PER_NIBBLE;
  } else {
    MaxBit = GetMaxBitsValid (Host);
  }
  if (Bit >= MaxBit) {
    return 1;
  }
  return 0;
} // IsBitNotValid

/**
  Determines the DQ nibble number associated with the given strobe number

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Strobe        - Strobe number

  @retVal   UINT8   DQ nibble number
**/
UINT8
GetNibbleFromStrobe (
  IN      PSYSHOST        Host,
  IN      UINT8           Strobe
  )
{
  UINT8   Nibble;
  UINT8   LookupUpperStrobes [10] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
  UINT8   MSVx4 = GetMaxStrobeValid (Host);
  UINT8   MSVx8 = GetMaxStrobeValid (Host) / 2;
  //
  // DDR5 (BRS):
  // Strobe   0   5   1   6   2   7   3   8   4   9...
  // Nibble   0   1   2   3   4   5   6   7   8   9...
  //
  if (Strobe < MSVx8) {
    Nibble = Strobe * 2;
  } else if (Strobe < MSVx4) {
    Nibble = LookupUpperStrobes[Strobe - MSVx8];
  } else {
    Nibble = 0;
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_176);
  }
  return Nibble;
}
/**
  Determines the strobe number associated with the given DQ nibble number

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Nibble        - DQ nibble number

  @retVal   UINT8   strobe number
**/
UINT8
GetStrobeFromNibble (
  IN      PSYSHOST        Host,
  IN      UINT8           Nibble
  )
{
  UINT8 Strobe;
#ifdef DDR5_SUPPORT
  if (IsDdr5Present(Host, Host->var.mem.currentSocket)) {
    //
    // DDR5 (BRS):
    // Nibble   0   1   2   3   4   5   6   7   8   9
    // Strobe   0   5   1   6   2   7   3   8   4   9
    //
    if (Nibble % 2) {
      Strobe = Nibble / 2 + MAX_STROBE/4;
    } else {
      Strobe = Nibble / 2;
    }
  } else {
#endif
    if (Nibble % 2) {
      Strobe = Nibble / 2 + GetMaxStrobeValid(Host) / 2;
    } else {
      Strobe = Nibble / 2;
    }
#ifdef DDR5_SUPPORT
  }
#endif
  return Strobe;
} // GetStrobeFromNibble


/**
  Determines a bit mask of valid strobe numbers for ECC mode on current socket

  @param[in]      Host          - Pointer to sysHost

  @retVal   UINT32    Bit mask of valid Strobe numbers
**/
UINT32
GetStrobeMask (
  IN      PSYSHOST        Host
  )
{
  UINT32 StrobeMask = 0;
  UINT8 Strobe;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);

  for (Strobe = 0; Strobe < MSVx4; Strobe++) {
    if (IsStrobeNotValid (Host, Strobe)) {
      continue;
    }
    StrobeMask |= (1 << Strobe);
  }
  return StrobeMask;
} // GetStrobeMask

/**

  Get Strobe Mapping based on a SubChannel and current strobe within the SubChannel

  DDR5:
  Strobe 0 -  9 in SUCH 0 are logical strobes 00 - 09
  Strobe 0 -  9 in SUCH 1 are logical strobes 10 - 19

  DDR4:
  Strobe 0 - 17 in CH 0 are logical strobes 00 - 17 (SubChannel must be always '0')


  @param[in] Channel      Channel number
  @param[in] SubChannel   Sub Channel number
  @param[in] Strobe       Strobe within the SubChannel

  @return    Strobe Mapping

**/
UINT8
GetStrobeMapping (
  IN     UINT8   Channel,
  IN     UINT8   SubChannel,
  IN     UINT8   Strobe
  )
{
  UINT8     GlobalStrobe;
  PSYSHOST  Host = GetSysHostPointer ();

  if (Strobe >= GetSubChMaxStrobeValid (Host)) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_174);
    return 0;
  }

  GlobalStrobe = Strobe + (UINT8)(GetSubChMaxStrobeValid (Host) * SubChannel);

  return GlobalStrobe;
}

/**
  Compare a DDR4 DIMM's "Module Part Number" SPD data against a template.

  Additional considerations:
  * The character '*' in the template is used as a wildcard.
  * The first ASCII blank (0x20) in the SPD's "Module Part Number" is expected to be the termination byte, and the
    first NULL character ('\0') in the template is expected to be the termination character. Any bytes or characters
    after these terminations are ignored.

  @param[in]  Host          A pointer to the Host structure
  @param[in]  Socket        The socket containing the DIMM to compare against
  @param[in]  Channel       The channel containing the DIMM to compare against
  @param[in]  Dimm          The DIMM to compare against
  @param[in]  Template      The template to compare against the DIMM's SPD data
  @param[in]  TemplateSize  The number of characters in the template, including the termination character

  @retval FALSE   The DIMM's "Module Part Number" SPD data did not match the template.
  @retval TRUE    The DIMM's "Module Part Number" SPD data matched the template.
**/
BOOLEAN
CompareDimmPartNumberDdr4 (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  UINT8     Dimm,
  IN  CHAR8     Template[],
  IN  UINT8     TemplateSize
  )
{
  DIMM_NVRAM_STRUCT (*DimmNvList)[MAX_DIMM] = NULL;
  UINT8             Index = 0;
  BOOLEAN           Matching = TRUE;
  UINT8             SharedMaxIndex = 0;
  UINT8             SpdByte = SPD_MODULE_PART_UNUSED;
  CHAR8             TemplateCharacter = '\0';
  BOOLEAN           TerminationFound = FALSE;

  DimmNvList = GetDimmNvList (Host, Socket, Channel);
  SharedMaxIndex = MIN (SPD_MODULE_PART_DDR4, TemplateSize);

  // Compare the SPD part number info against the template.
  for (Index = 0; Index < SharedMaxIndex; Index++) {

    SpdByte = (*DimmNvList)[Dimm].SPDModPartDDR4[Index];
    TemplateCharacter = Template[Index];

    // If we've found either string termination character, we're done comparing.
    if ((TemplateCharacter == '\0') || (SpdByte == SPD_MODULE_PART_UNUSED)) {

      // However, if they're not both termination characters, then it's not a match.
      if ((TemplateCharacter != '\0') || (SpdByte != SPD_MODULE_PART_UNUSED)) {
        Matching = FALSE;
      }

      TerminationFound = TRUE;
      break;
    }

    // If this character is a "don't care," skip to the next one.
    if (TemplateCharacter == SPD_MODPART_WILDCARD) {
      continue;
    }

    // If an SPD byte doesn't match the template, then the part number isn't a match.
    if (((CHAR8)SpdByte) != TemplateCharacter) {
      Matching = FALSE;
      break;
    }
  }

  // If all the characters are matching, but we didn't find a string termination character in the template, then we
  // need to confirm that the termination character is the first uncomparable one. Otherwise the part number doesn't
  // actually match the template, and--more importantly--our template is too long for the number of SPD bytes.
  if (Matching && (TerminationFound == FALSE)) {

    // Confirm whether the first and only uncomparable character is a string termination.
    if (TemplateSize == (SPD_MODULE_PART_DDR4 + 1)) {
      if (Template[SPD_MODULE_PART_DDR4] == '\0') {
        TerminationFound = TRUE;
      }
    }

    // Raise an error if the template was too long (no termination character found), or if for some reason we never got
    // through all the SPD bytes. In either case, the comparison against the template was incomplete.
    if ((TerminationFound == FALSE) || (Index != SPD_MODULE_PART_DDR4)) {
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_195);
      Matching = FALSE; // This isn't necessary, but it's added here to be on the safe side.
    }
  }

  return Matching;
}

/**

  Determine if a given dimm supports programable CTLE / Rx Eq.  Data from Intel PMO

  @param[in] Host        - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket      - Socket Id
  @param[in] Ch          - Channel number
  @param[in] Dimm        - DIMM number (0-based)

  @retval:   TRUE if input Dimm supports programable CTLE / Rx Eq
             FALSE if input Dimm does NOT support programable CTLE / Rx Eq

**/
BOOLEAN
EFIAPI
DimmSupportForProgrammableCtle (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch,
  IN     UINT8     Dimm
  )
{
  BOOLEAN             SupportForProgrammableCtle = FALSE;
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  UINT16              DateCode = 0;
  BOOLEAN             IsSamsungDDie = FALSE;
  CHAR8               SamsungDDiePartNumber[] = SAMSUNG_SPD_MODPART_DDR4_D_DIE;

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  if ((Host->nvram.mem.dramType != SPD_TYPE_DDR4) ||
      (IsLrdimmPresent (Socket, Ch, Dimm) == 1) ||
      ((*DimmNvList)[Dimm].DcpmmPresent == 1)) {
    SupportForProgrammableCtle = FALSE;
  } else {
    if ((*DimmNvList)[Dimm].sdramCapacity == SPD_8Gb) {
      switch ((*DimmNvList)[Dimm].SPDDramMfgId) {
      case MFGID_HYNIX:
        //
        // Get the Date code of the dimm, swap the year and week bytes for easier comparison
        //
        DateCode = (*DimmNvList)[Dimm].SPDModDate >> 8;
        DateCode |= (*DimmNvList)[Dimm].SPDModDate << 8;
        //
        // The provided value to check is 2018, ww18, which is represented in BCD format as 0x1818
        //
        if (DateCode >= 0x1818) {
          SupportForProgrammableCtle = TRUE;
        } else {
          SupportForProgrammableCtle = FALSE;
        }
        break;

      case MFGID_MICRON:
        //
        // All micron dimms >= 2933 will support programmable Ctle
        //
        if ((*DimmNvList)[Dimm].minTCK <= DDR_2933_TCK_MIN) {
          SupportForProgrammableCtle = TRUE;
        } else {
          SupportForProgrammableCtle = FALSE;
        }
        break;

      case MFGID_SAMSUNG:
        //
        // Get the Date code of the dimm, swap the year and week bytes for easier comparison
        //
        DateCode = (*DimmNvList)[Dimm].SPDModDate >> 8;
        DateCode |= (*DimmNvList)[Dimm].SPDModDate << 8;

        IsSamsungDDie = CompareDimmPartNumberDdr4 (Host, Socket, Ch, Dimm, SamsungDDiePartNumber,
          ARRAY_SIZE (SamsungDDiePartNumber));

        if (IsSamsungDDie || (DateCode >= SAMSUNG_SPD_MODDATE_CTLE))
        {
          SupportForProgrammableCtle = TRUE;
        } else {
          SupportForProgrammableCtle = FALSE;
        }
        break;

      case MFGID_NANYA:
        //
        // Nanya DIMM:
        // Get the Data code of the dimm, swap the year and week bytes for easier comparison
        //
        DateCode = (*DimmNvList)[Dimm].SPDModDate >> 8;
        DateCode |= (*DimmNvList)[Dimm].SPDModDate << 8;
        //
        // The provided value to check is 2019, WW27 which is represented in BCD format as 1927
        //
        if (DateCode >= 0x1927) {
          SupportForProgrammableCtle = TRUE;
        } else {
          SupportForProgrammableCtle = FALSE;
        }
        break;

      default:
        SupportForProgrammableCtle = FALSE;
        break;
      }
    } else if ((*DimmNvList)[Dimm].sdramCapacity == SPD_16Gb) {
      SupportForProgrammableCtle = TRUE;
    } else {
      SupportForProgrammableCtle = FALSE;
    }
  }

  return SupportForProgrammableCtle;
}

/*
  Update the cached Rx Eq value in RankList Structure

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Socket Id (0-based)
  @param[in] Ch           - Channel number (0-based)
  @param[in] Dimm         - DIMM number (0-based)
  @param[in] Rank         - Rank number (0-based)
  @param[in] Strobe:      - Dqs data group within the rank
  @param[in] RxEqSetting  - Value to be updated in cache.

  @retval - N/A
*/
VOID
SetCachedRxEq (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Rank,
  IN UINT8    Dimm,
  IN UINT8    Strobe,
  IN INT16    RxEqSetting
  )
{
  UINT8               MSVx8;
  Mr1RxEqStruct       *CachedRxEq;
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];

  RankList = GetRankNvList (Host, Socket, Ch, Dimm);
  ASSERT (RankList != NULL);

  MSVx8 = GetMaxStrobeValid (Host) / 2;
    if (Strobe < MSVx8) {
      CachedRxEq = &((*RankList)[Rank].Mr1RxEqSettings[0]);
    } else {
      CachedRxEq = &((*RankList)[Rank].Mr1RxEqSettings[1]);
    }
    switch (Strobe % MSVx8) {
    case 0:
      CachedRxEq->Strobe0 = RxEqSetting;
      break;
    case 1:
      CachedRxEq->Strobe1 = RxEqSetting;
      break;
    case 2:
      CachedRxEq->Strobe2 = RxEqSetting;
      break;
    case 3:
      CachedRxEq->Strobe3 = RxEqSetting;
      break;
    case 4:
      CachedRxEq->Strobe4 = RxEqSetting;
      break;
    case 5:
      CachedRxEq->Strobe5 = RxEqSetting;
      break;
    case 6:
      CachedRxEq->Strobe6 = RxEqSetting;
      break;
    case 7:
      CachedRxEq->Strobe7 = RxEqSetting;
      break;
    case 8:
      CachedRxEq->Strobe8 = RxEqSetting;
      break;
    default:
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_77);
    }
}

/*
  Get the Cached Rx Eq value from RankList Structure

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket Id (0-based)
  @param[in] Ch       - Channel number (0-based)
  @param[in] Dimm     - DIMM number (0-based)
  @param[in] Rank     - Rank number (0-based)
  @param[in] Strobe:  - Dqs data group within the rank

  @retval - Cached DQ RX EQ value
*/
UINT32
GetCachedRxEq (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Rank,
  IN UINT8    Dimm,
  IN UINT8    Strobe
  )
{
  UINT8               MSVx8;
  Mr1RxEqStruct       *CachedRxEq;
  struct ddrRank      (*RankList)[MAX_RANK_DIMM];
  UINT32              RxEqSetting = 0;

  RankList = GetRankNvList (Host, Socket, Ch, Dimm);
  ASSERT (RankList != NULL);

  MSVx8 = GetMaxStrobeValid (Host) / 2;
    if (Strobe < MSVx8) {
      CachedRxEq = &((*RankList)[Rank].Mr1RxEqSettings[0]);
    } else {
      CachedRxEq = &((*RankList)[Rank].Mr1RxEqSettings[1]);
    }

    switch (Strobe % MSVx8) {
    case 0:
      RxEqSetting = CachedRxEq->Strobe0;
      break;
    case 1:
      RxEqSetting = CachedRxEq->Strobe1;
      break;
    case 2:
      RxEqSetting = CachedRxEq->Strobe2;
      break;
    case 3:
      RxEqSetting = CachedRxEq->Strobe3;
      break;
    case 4:
      RxEqSetting = CachedRxEq->Strobe4;
      break;
    case 5:
      RxEqSetting = CachedRxEq->Strobe5;
      break;
    case 6:
      RxEqSetting = CachedRxEq->Strobe6;
      break;
    case 7:
      RxEqSetting = CachedRxEq->Strobe7;
      break;
    case 8:
      RxEqSetting = CachedRxEq->Strobe8;
      break;
    default:
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_77);
    }
    return RxEqSetting;
}

/**

  Get/Set DDR4 DRAM Rx Eq (DRAM CTLE) per DDR4 standard

  @param[in]      Host     - Pointer to sysHost, the system Host (root) structure
  @param[in]      Socket   - Socket Id
  @param[in]      Ch       - Channel number
  @param[in]      Dimm     - DIMM number (0-based)
  @param[in]      Rank     - Rank number (0-based)
  @param[in]      Strobe   - Strobe number (0-based)
  @param[in]      Bit      - Bit number
  @param[in]      Level    - MRC_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]      Group    - MRC_GT - Parameter to program
  @param[in]      Mode     - GSM_READ_ONLY - skip write,
  @param[in, out] *Value   - Pointer to int16 to return read data or to use for write data

  @retval         MRC_STATUS_SUCCESS

**/
MRC_STATUS
EFIAPI
GetSetDdr4DramRxEq (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch,
  IN     UINT8     Dimm,
  IN     UINT8     Rank,
  IN     UINT8     Strobe,
  IN     UINT8     Bit,
  IN     MRC_LT    Level,
  IN     MRC_GT    Group,
  IN     UINT8     Mode,
  IN OUT INT16     *Value
  )
{
#ifndef DDR5_SUPPORT
  INT16               CurVal = 0;
  INT16               RxEqSetting = 0;
  UINT16              PerStrobeMr1Value = 0;
  UINT8               Dram;
  UINT8               MaxStrobe;
  struct rankDevice   (*RankStruct)[MAX_RANK_DIMM];
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  UINT8               MSVx4;
  UINT8               MSVx8;
  BOOLEAN             DoMrsPda = FALSE;

  MSVx4 = GetMaxStrobeValid (Host);
  MSVx8 = GetMaxStrobeValid (Host) / 2;
  ChannelNvList = GetChannelNvList (Host, Socket);
  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);

  if ((Strobe >= MAX_STROBE) &&
      (Strobe != ALL_STROBES)) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
                   "The strobe value is out of range!\n");
    RC_FATAL_ERROR ((Strobe < MAX_STROBE || Strobe == ALL_STROBES), ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  //
  // If Dimm does not support programmable CTLE, then skip
  //
  if (!DimmSupportForProgrammableCtle (Host, Socket, Ch, Dimm)) {
    return MRC_STATUS_SUCCESS;
  }

  if (Host->nvram.mem.socket[Socket].TrainingStepDone.DramRxEq == 1) {
    DoMrsPda = TRUE;
    if (Strobe == ALL_STROBES) {
      Strobe = 0;
      MaxStrobe = GetMaxStrobe (Host, Socket, Ch, Dimm, Group, Level);
    } else {
      MaxStrobe = Strobe + 1;
      if (MaxStrobe > MSVx4) {
        return MRC_STATUS_STROBE_NOT_PRESENT;
      }
    }
  } else {
    DoMrsPda = FALSE;
    //
    // Just use Strobe 0 as the "common" setting
    //
    Strobe = 0;
    MaxStrobe = Strobe + 1;
  }

  for (; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValid (Host, Strobe)) {
      continue;
    }

    //
    // Always Read from Cache, as this is an MRS and cannot be read from hardware
    //
    PerStrobeMr1Value = GetPerStrobeMrsValue (Socket, Ch, Dimm, Rank, Strobe, BANK1);
    CurVal = (INT16) RightJustifySparse16BitField (MR1_RX_EQ, PerStrobeMr1Value);

    //
    // Check if read only
    //
    if ((Mode & GSM_READ_ONLY) != 0) {
      //
      // Save the value read into the output parameter
      //
      *Value = CurVal;
    } else {
      //
      // Write
      //
      if ((Mode & GSM_WRITE_OFFSET) != 0) {
        //
        // Add offset to the current value
        //
        RxEqSetting = *Value + CurVal;
      } else {
        //
        // Write absolute value
        //
        RxEqSetting = *Value;
      }
      //
      // Only 3 valid bits, so check to make sure the limit is not exceeded
      //
      if (RxEqSetting < 0) {
        RxEqSetting = 0;
      }
      if (RxEqSetting > 7) {
        RxEqSetting = 7;
      }
      PerStrobeMr1Value &= ~MR1_RX_EQ;
      //
      //Write MR1 for DRAM Rx Eq setting
      //
      PerStrobeMr1Value |= DistributeSparse16BitField (MR1_RX_EQ, RxEqSetting);
      if ((*DimmNvList)[Dimm].x4Present) {
        if (Strobe >= MSVx8) {
          Dram = ((Strobe - MSVx8) * 2) + 1;
        } else {
          Dram = Strobe * 2;
        }
      } else {
        Dram = Strobe % MSVx8;
      }
      if (DoMrsPda) {
        WriteMRSPDA (Host, Socket, Ch, Dimm, Rank, Dram, PerStrobeMr1Value, BANK1);
      } else {
        WriteMRS (Host, Socket, Ch, Dimm, Rank, PerStrobeMr1Value, BANK1);
      }
      if (Mode & GSM_UPDATE_CACHE) {
        (*RankStruct)[Rank].MR1 = (PerStrobeMr1Value & ~MR1_RX_EQ);
        SetCachedRxEq (Host, Socket, Ch, Rank, Dimm, Strobe, RxEqSetting);
      }
    }
  } //Strobe loop

#endif // !DDR5_SUPPORT
  return MRC_STATUS_SUCCESS;
} // GetSetDramRxEq

/**

  Enables parity checking on the DIMM

  @param Host:         Pointer to sysHost
  @param Socket:       Processor socket to check

  @retval: None
**/
VOID
EFIAPI
EnableParityChecking (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  UINT8                       Ch;
  struct channelNvram         (*channelNvList)[MAX_CH];
  BootMode                    SysBootMode;
  UINT8                       MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  SysBootMode = GetSysBootMode ();

  channelNvList = GetChannelNvList (Host, Socket);

  if (!DoesChipSupportParityChecking (Host)) {
    return;
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*channelNvList)[Ch].enabled == 0) {
      continue;
    }

    OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_EARLY_CMD_CLK, (UINT16) ((CHECKPOINT_DATA_ENABLE_PARITY << 8) | Socket)));

    if ((SysBootMode == NormalBoot) &&
        ((Host->var.mem.subBootMode == ColdBoot) || (Host->var.mem.subBootMode == ColdBootFast) || (Host->var.mem.subBootMode == NvDimmResume)) &&
        (Host->nvram.mem.dramType == SPD_TYPE_DDR4)) {

      EnableParityCheckingDdr4 (Socket, Ch);

    } else if ((SysBootMode == NormalBoot) &&
      ((Host->var.mem.subBootMode == ColdBoot) || (Host->var.mem.subBootMode == ColdBootFast) || (Host->var.mem.subBootMode == NvDimmResume)) &&
      (Host->nvram.mem.dramType == SPD_TYPE_DDR5)) {
#ifdef DDR5_SUPPORT
      EnableParityCheckingDdr5 (Socket, Ch);
#endif
    } // DDR5
  } // Ch loop

  EnableCAParityRuntime (Host, Socket);
} // EnableParityChecking

/**

  Enables parity checking on the DDR4 RCD/DRAM

  @param[in]  Socket:  Processor socket to check
  @param[in]  Ch:      Channel number

  @retval: None
**/
VOID
EFIAPI
EnableParityCheckingDdr4 (
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
#ifndef DDR5_SUPPORT
  PSYSHOST                     Host;
  UINT8                        Dimm;
  UINT8                        Rank;
  struct dimmNvram             (*dimmNvList)[MAX_DIMM];
  struct rankDevice            (*rankStruct)[MAX_RANK_DIMM];
  CHIP_PARITY_CHECKING_STRUCT  chipEnableParityCheckingStruct;

  Host = GetSysHostPointer();

  //
  // Set 3N timing and save original setting
  //
  ChipSet3NTimingandSaveOrigSetting (Host, Socket, Ch, &chipEnableParityCheckingStruct);

  dimmNvList = GetDimmNvList (Host, Socket, Ch);

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*dimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }

    rankStruct = GetRankStruct (Host, Socket, Ch, Dimm);

    for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
        continue;
      }

      // Clear parity results so we can start with a clean, parity-enabled state
      ChipClearParityResultsAlt (Host, Socket, (1 << Ch), Rank, Dimm);

      if ((Host->nvram.mem.dimmTypePresent == UDIMM) || (Host->nvram.mem.dimmTypePresent == SODIMM)) {
        //
        // Enable parity checking in the DRAMs
        //
        if (Host->nvram.mem.socket[Socket].ddrFreq < DDR_2400) {
          (*rankStruct)[Rank].MR5 = (*rankStruct)[Rank].MR5 | BIT0 | BIT9;
        } else if (Host->nvram.mem.socket[Socket].ddrFreq >= DDR_2400 && Host->nvram.mem.socket[Socket].ddrFreq < DDR_2933) {
          (*rankStruct)[Rank].MR5 = (*rankStruct)[Rank].MR5 | BIT1 | BIT9;
        } else {
          (*rankStruct)[Rank].MR5 = (*rankStruct)[Rank].MR5 | BIT0 | BIT1 | BIT9;
        }

        WriteMRS (Host, Socket, Ch, Dimm, Rank, (*rankStruct)[Rank].MR5, BANK5);
      } else {
        if ((Rank == 0)) {
          //
          // Enable parity checking in the register
          //
          if (Host->nvram.mem.socket[Host->var.mem.currentSocket].cmdClkTrainingDone == 0) {
            if ((*dimmNvList)[Dimm].DcpmmPresent) {
              WriteRCFnv (Host, Socket, Ch, Dimm, BIT0, RDIMM_RC0E);
            } else {
              WriteRC (Host, Socket, Ch, Dimm, Rank, BIT0, RDIMM_RC0E);
            }
          } else {
            if ((*dimmNvList)[Dimm].DcpmmPresent) {
              WriteRCFnv (Host, Socket, Ch, Dimm, BIT0 | BIT2 | BIT3, RDIMM_RC0E);
            } else {
              WriteRC (Host, Socket, Ch, Dimm, Rank, BIT0 | BIT2 | BIT3, RDIMM_RC0E);
            }
          }

          if (! ((*dimmNvList)[Dimm].DcpmmPresent)) {
            // Set RC4x CW Selection control to F0RCFx, Error Log Register
            WriteRC (Host, Socket, Ch, Dimm, Rank, 0xF, RDIMM_RC4x);
          }
        }
      }
    } // Rank loop
  } // Dimm loop

  //
  // Restore original CMD timing
  //
  ChipRestoreOrigCMDtiming (Host, Socket, Ch, &chipEnableParityCheckingStruct);
#endif // !DDR5_SUPPORT
}

/**

  Disables parity checking on the DDR4 DIMM

  @param[in]  Socket:  Processor socket to check
  @param[in]  Ch:      Channel number

**/
VOID
DisableParityCheckingDdr4 (
  UINT8     Socket,
  UINT8     Ch
  )
{
#ifndef DDR5_SUPPORT
  PSYSHOST                          Host;
  UINT8                             Dimm;
  UINT8                             Rank;
  struct dimmNvram                  (*dimmNvList)[MAX_DIMM];
  struct rankDevice                 (*rankStruct)[MAX_RANK_DIMM];
  CHIP_PARITY_CHECKING_STRUCT       chipDisableParityCheckingStruct;

  Host = GetSysHostPointer();

  //
  // Set 3N timing and save original setting
  //
  ChipSet3NTimingandSaveOrigSetting (Host, Socket, Ch, &chipDisableParityCheckingStruct);

  dimmNvList = GetDimmNvList (Host, Socket, Ch);

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*dimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }

    rankStruct = GetRankStruct (Host, Socket, Ch, Dimm);

    for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
        continue;
      }

      if ((Host->nvram.mem.dimmTypePresent == UDIMM) || (Host->nvram.mem.dimmTypePresent == SODIMM)) {

        if (Host->nvram.mem.socket[Socket].ddrFreq < DDR_2400) {
          (*rankStruct)[Rank].MR5 = (*rankStruct)[Rank].MR5 & ~ (BIT0 | BIT9);
        } else if (Host->nvram.mem.socket[Socket].ddrFreq >= DDR_2400 && Host->nvram.mem.socket[Socket].ddrFreq < DDR_2933) {
          (*rankStruct)[Rank].MR5 = (*rankStruct)[Rank].MR5 & ~ (BIT1 | BIT9);
        } else {
          (*rankStruct)[Rank].MR5 = (*rankStruct)[Rank].MR5 & ~ (BIT0 | BIT1 | BIT9);
        }

        WriteMRS (Host, Socket, Ch, Dimm, Rank, (*rankStruct)[Rank].MR5, BANK5);
      } else {
        if (Rank == 0) {
          if ((*dimmNvList)[Dimm].DcpmmPresent) {
            WriteRCFnv (Host, Socket, Ch, Dimm, 0, RDIMM_RC0E);
          } else {
            WriteRC (Host, Socket, Ch, Dimm, Rank, 0, RDIMM_RC0E);
          }
        }
      }
    } // Rank loop
  } // Dimm loop

  //
  // Restore original CMD timing
  //
  ChipRestoreOrigCMDtiming (Host, Socket, Ch, &chipDisableParityCheckingStruct);
#endif // !DDR5_SUPPORT
} // DisableParityCheckingDdr4

/**

  Enables Write CRC in the DRAM

  @param[in] Socket   - Socket number
  @param[in] Ch       - Channel number within the socket
  @param[in] Dimm     - DIMM number within the channel
  @param[in] Rank     - Rank number within the DIMM

  @retval N/A

**/
VOID
EFIAPI
Ddr4DramEnableWrCrc (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT8     Rank
  )
{
#ifndef DDR5_SUPPORT
  PSYSHOST              Host;
  struct rankDevice     (*RankStruct)[MAX_RANK_DIMM];

  Host = GetSysHostPointer ();

  RankStruct = GetRankStruct(Host, Socket, Ch, Dimm);
  (*RankStruct)[Rank].MR2 |= BIT12;// WrCRC Bit

  WriteMRS (Host, Socket, Ch, Dimm, Rank, (*RankStruct)[Rank].MR2, BANK2);
#endif // !DDR5_SUPPORT
}

/**

  Enables Write CRC in the DRAM

  @param[in] Socket   - Socket number
  @param[in] Ch       - Channel number within the socket
  @param[in] Dimm     - DIMM number within the channel
  @param[in] Rank     - Rank number within the DIMM

  @retval N/A

**/
VOID
EFIAPI
DramEnableWrCrc (
  IN  UINT8    Socket,
  IN  UINT8    Ch,
  IN  UINT8    Dimm,
  IN  UINT8    Rank
  )
{
  PSYSHOST        Host;

  Host = GetSysHostPointer ();

  if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
    Ddr5DramEnableWrCrc (Socket, Ch, Dimm, Rank);
  } else {
    Ddr4DramEnableWrCrc (Socket, Ch, Dimm, Rank);
  }
}

/**

  Checks to see of supplied group is a member of the "Dimm" parameter group which is adjuested
  by the RCD, DB or DRAM.

  @param group - Group item to check

  @retval TRUE  - Is a member of DIMM Group
  @retval FALSE - Is not a member of DIMM Group

**/
BOOLEAN
CheckDimmParamGroupDdr4 (
  MRC_GT  Group
  )
{
  // Placeholder. return FALSE to not change current bios behavior.
  return FALSE;
}

/**

  Checks to see of supplied group is a member of the "Dimm" parameter group which is adjuested
  by the RCD, DB or DRAM.

  @param group - Group item to check

  @retval TRUE  - Is a member of DIMM Group
  @retval FALSE - Is not a member of DIMM Group

**/
BOOLEAN
CheckDimmParamGroup (
  MRC_GT  Group
  )
{
  PSYSHOST Host;

  Host = GetSysHostPointer ();
  if (Host->nvram.mem.dramType == SPD_TYPE_DDR4) {
    return CheckDimmParamGroupDdr4 (Group);
  } else {
    return CheckDimmParamGroupDdr5 (Group);
  }
}
