/** @file
  API for the memory Pkg Delay library

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

#include <SysHostChip.h>
#include <Library/MemoryCoreLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemCmdControlTablesLib.h>

#include <Library/BaseLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/BaseMemoryLib.h>

#include "Include/MemCmdControl.h"
#include <Memory/MemCmdCtlClkCommon.h>
#include "Include/MemDdrioRegs.h"
#include <UncoreCommonIncludes.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/CheckpointLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/MemRcLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/PkgDelayLib.h>
#include <Memory/MemoryCheckpointCodes.h>

#include <Library/RcMemAllocLib.h>
#include <Chip/Include/MemHostChip.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/KtiApi.h>

#define MIN_CMD_VREF 85
#define MAX_CMD_VREF 170

#define DISABLE_OUTPUT_DATAPATH 0x0

// Table of # pico seconds per pi ticks for each supported frequency
// Formula is: ((1/(Frq/2))*1.0E6)/64 - ((1/(1066/2))*1000000)/64 = 29.29
//                                        800 1000 1066 1200 1333 1400 1600 1800 1866 2000 2133 2200 2400 2600 2666 2800 2933 3000 3200 3400 3467 3600 3733 3800 4000 4200 4266 4400
const UINT8 piPsFrqTable[MAX_SUP_FREQ] = { 20,  16,  15,  13,  12,  11,  10,   9,   8,   8,   7,   7,   7,   6,   6,   6,   5,   5,   5,   5,   5,   4,   4,   4,   4,   4,   4,   4};

//
// Local Prototypes
//
STATIC MRC_STATUS GetSetCmdVrefHostSide (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 mode, INT16 *value);

STATIC VOID EnableCCCOutputPath (PSYSHOST Host, UINT8 Socket, UINT8 Ch, BOOLEAN Enable, UINT32* RankEnCmdS, UINT32* RankEnCmdN, UINT32* RankEnCtl, UINT32* RankEnCke);

/*
  Function to obtain pico seconds per pi ticks for each supported frequency

  @param [in] RatioIndex - Frequency ratio index value

  @retval Pico seconds per pi tick
*/
UINT8
GetPicoSecondsPerPiTickForFreq (
  IN UINT8 RatioIndex
  )
{
  return piPsFrqTable[RatioIndex];
}

/**
  Multi-use function to either get or set signal delays based on the provided group number

  @param [in    ] Host   - Pointer to SYSHOST
  @param [in    ] socket - Socket number
  @param [in    ] ch     - Channel number
  @param [in    ] level  - IO level to access
  @param [in    ] sig    - Enumerated signal name
  @param [in    ] mode   - GSM_READ_CSR - Read the data from hardware and not cache
                           GSM_READ_ONLY - Do not write
                           GSM_WRITE_OFFSET - Write offset and not value
                           GSM_FORCE_WRITE - Force the write
  @param [   out] value  - Absolute value or offset selected by GSM_WRITE_OFFSET

  @retval MRC_STATUS

**/
MRC_STATUS
GetSetSignal (
  IN     PSYSHOST Host,
  IN     UINT8    socket,
  IN     UINT8    ch,
  IN     MRC_LT   level,
  IN     GSM_CSN  sig,
  IN     UINT8    mode,
     OUT INT16    *value
  )
{
  MRC_STATUS           status;
  struct signalIOGroup signalGroup;

  status = MRC_STATUS_SUCCESS;
  //
  // Get the silicon pi group this signal belongs to
  //
  status = SignalToPiGroup (Host, sig, &signalGroup);

  if (status == MRC_STATUS_SUCCESS) {
    status = GetSetCmdDelay (Host, socket, ch, signalGroup.group.num, signalGroup.group.side, mode, value);
  }

  return status;
} // GetSetSignal

/**
  Gets the silicon pi group from the signal name

  @param [in    ] Host        - Pointer to SYSHOST
  @param [in    ] sig         - Enumerated signal name
  @param [   out] signalGroup - Signal group pointer

  @retval MRC_STATUS
**/
MRC_STATUS
SignalToPiGroup (
  IN     PSYSHOST             Host,
  IN     GSM_CSN              sig,
     OUT struct signalIOGroup *signalGroup
  )
{
  UINT8                sigLoop;
  UINT8                maxSignals;
  struct signalIOGroup *signals;
  MRC_STATUS           status;
  UINT8                SocketId;

  SocketId = GetSysSocketId ();
  GetDdrSignalsChip (&signals, &maxSignals);

  for (sigLoop = 0; sigLoop < maxSignals; sigLoop++) {
    if (signals[sigLoop].sig == sig) {
      *signalGroup = signals[sigLoop];
      break;
    }
  } // sigLoop loop

  if (sigLoop == maxSignals) {
    RcDebugPrint (SDBG_MAX,
                   "Unable to find signal %a\n", GetSignalStr (sig));
    status = MRC_STATUS_SIGNAL_NOT_SUPPORTED;
  } else {
    status = MRC_STATUS_SUCCESS;
  }

  return status;
} // SignalToPiGroup

/**
  Gets pointer to CmdCtl group list and number of elements in the list

  @param [in    ] Host       - Pointer to SYSHOST
  @param [in    ] group      - CmdCtl group to get
  @param [   out] maxIoGroup - Ponter to number of elements in the list
  @param [   out] iogPtr     - Pointer to the list

  @retval MRC_STATUS_SUCCESS or MRC_STATUS_GROUP_NOT_SUPPORTED
**/
MRC_STATUS
GetCmdCtlGroupList (
  IN     PSYSHOST       Host,
  IN     MRC_GT         group,
     OUT UINT8          *maxIoGroup,
     OUT struct ioGroup **iogPtr
  )
{
  MRC_STATUS status = MRC_STATUS_SUCCESS;
  UINT8      SocketId;

  SocketId = GetSysSocketId ();

  switch (group) {
    case CmdAll:
      status = GetCmdGroupAllTable (iogPtr, maxIoGroup);
      break;
    case CtlGrp0:
      status = GetCtlGroup0Table (iogPtr, maxIoGroup);
      break;
    case CtlGrp1:
      status = GetCtlGroup1Table (iogPtr, maxIoGroup);
      break;
    case CtlGrp2:
      status = GetCtlGroup2Table (iogPtr, maxIoGroup);
      break;
    case CtlGrp3:
      status = GetCtlGroup3Table (iogPtr, maxIoGroup);
      break;
    case CtlAll:
      status = GetCtlGroupAllTable (iogPtr, maxIoGroup);
      break;
    case CmdCtlAll:
      status = GetCmdCtlGroupAllTable (iogPtr, maxIoGroup);
      break;
    default:
      status = MRC_STATUS_GROUP_NOT_SUPPORTED;
      break;
  }

  return status;
}
/*

  Return the number of knobs (signals) inside the specified group

  @param[in]      Host        Pointer to syshost structure
  @param[in]      Group       MRC_GT group under test
  @param[out]     MaxIoGroup  Pointer to return the Max number of IO groups

  @retval         MRC_STATUS_SUCCESS    If function successfully executed
  @retval         MRC_STATUS_GROUP_NOT_SUPPORTED    If group is not supported

*/
MRC_STATUS
GetNumberOfKnobsPerCmdCtlGroups (
  IN     PSYSHOST       Host,
  IN     MRC_GT         Group,
     OUT UINT8          *MaxIoGroup
)
{
  struct ioGroup  *IoGroupPtr;
  MRC_STATUS      Status;

  *MaxIoGroup = 0;

  Status = GetCmdCtlGroupList (Host, Group, MaxIoGroup, &IoGroupPtr);

  return Status;

}

/**
  Multi-use function to either get or set control delays based on the provided group number

  @param [in    ] Host    - Pointer to SYSHOST
  @param [in    ] socket  - Socket number
  @param [in    ] ch      - Channel number
  @param [in    ] group   - Group number
  @param [in    ] mode    - GSM_READ_CSR - Read the data from hardware and not cache
                            GSM_READ_ONLY    - Do not write
                            GSM_WRITE_OFFSET - Write offset and not value
                            GSM_FORCE_WRITE  - Force the write
  @param [   out] value   - Value to program
  @param [   out] *minVal - Current minimum control delay
  @param [   out] *maxVal - Current maximum control delay

  @retval minVal, maxVal and MRC_STATUS
**/
MRC_STATUS
GetSetCtlGroupDelay (
  IN     PSYSHOST Host,
  IN     UINT8    socket,
  IN     UINT8    ch,
  IN     MRC_GT   group,
  IN     UINT8    mode,
     OUT INT16    *value,
     OUT UINT16   *minVal,
     OUT UINT16   *maxVal
  )
{
  UINT8           maxPlatformGroup;
  UINT8           maxIoGroup;
  UINT8           iog;
  UINT8           iogNum;
  UINT8           iogSide;
  struct ioGroup  *iogPtr;
  MRC_STATUS      status = MRC_STATUS_SUCCESS;
  UINT32          RankEnCmdS;
  UINT32          RankEnCmdN;
  UINT32          RankEnCtl;
  UINT32          RankEnCke;

  maxPlatformGroup = (UINT8)group + 1;

  // Associate CTL groups for RDIMM
  if (Host->nvram.mem.dimmTypePresent == RDIMM) {    // or LRDIMM
    if ((group == CtlGrp0) || (group == CtlGrp2)) {
      maxPlatformGroup++;
    }
  }

  //
  // Turn off the output data path before moving the picode to prevent gitch
  //
  EnableCCCOutputPath (Host, socket, ch, FALSE, &RankEnCmdS, &RankEnCmdN, &RankEnCtl, &RankEnCke);

  for ( ; group < maxPlatformGroup; group++) {

    status = GetCmdCtlGroupList(Host, group, &maxIoGroup, &iogPtr);

    if (status == MRC_STATUS_SUCCESS) {
      for (iog = 0; iog < maxIoGroup; iog++) {

        // Get IO Group number and side
        iogNum = (iogPtr + iog)->num;
        iogSide = (iogPtr + iog)->side;

        // Program the IO delay offset
        status = GetSetCmdDelay (Host, socket, ch, iogNum, iogSide, mode, value);

        // Update min/max values
        UpdateMinMaxUint (*value, minVal, maxVal);
      } // iog loop
    } // status
  }

  EnableCCCOutputPath (Host, socket, ch, TRUE, &RankEnCmdS, &RankEnCmdN, &RankEnCtl, &RankEnCke);

  return status;
} // GetSetCtlGroupDelay

/**
  Multi-use function to either get or set command vref

  @param [in    ] Host   - Pointer to SYSHOST
  @param [in    ] socket - Socket number
  @param [in    ] ch     - Channel number
  @param [in    ] dimm   - Dimm slot
  @param [in    ] level  - DDR level
  @param [in    ] mode   - GSM_READ_CSR - Read the data from hardware and not cache
                           GSM_READ_ONLY - Do not write
                           GSM_WRITE_OFFSET - Write offset and not value
                           GSM_FORCE_WRITE - Force the write
  @param [   out] value  - Value to program

  @retval minVal and maxVal
**/
MRC_STATUS
GetSetCmdVref (
  IN     PSYSHOST Host,
  IN     UINT8  socket,
  IN     UINT8  ch,
  IN     UINT8  dimm,
  IN     MRC_LT level,
  IN     UINT8  mode,
     OUT INT16  *value
  )
{
  MRC_STATUS status;
  struct dimmNvram (*dimmNvList)[MAX_DIMM];

  dimmNvList = GetDimmNvList(Host, socket, ch);

  if ((*dimmNvList)[dimm].DcpmmPresent) {
#ifdef LRDIMM_SUPPORT
    if (level == LrbufLevel) {
      status = GetSetCmdVrefFnvBackside(Host, socket, ch, dimm, mode, value);
    } else
#endif //LRDIMM_SUPPORT
    {
      status = GetSetCmdVrefFnv(Host, socket, ch, dimm, mode, value);
    }
  } else {
    status = GetSetCmdVrefHostSide (Host, socket, ch, mode, value);
  }

  return status;
}

/**
  Multi-use function to either get or set command delays based on the provided group number

  @param [in    ] Host    - Pointer to SYSHOST
  @param [in    ] socket  - Socket number
  @param [in    ] ch      - Channel number
  @param [in    ] group   - Group number
  @param [in    ] mode    - GSM_READ_CSR - Read the data from hardware and not cache
                            GSM_READ_ONLY - Do not write
                            GSM_WRITE_OFFSET - Write offset and not value
                            GSM_FORCE_WRITE - Force the write
  @param [   out] value   - Value to program
  @param [   out] *minVal - Current minimum command delay
  @param [   out] *maxVal - Current maximum command delay

  @retval minVal and maxVal
**/
MRC_STATUS
GetSetCmdGroupDelay (
  IN     PSYSHOST Host,
  IN     UINT8    socket,
  IN     UINT8    ch,
  IN     MRC_GT   group,
  IN     UINT8    mode,
     OUT INT16    *value,
     OUT UINT16   *minVal,
     OUT UINT16   *maxVal
  )
{
  UINT8          maxIoGroup;
  UINT8          iog;
  UINT8          iogNum;
  UINT8          iogSide;
  MRC_STATUS     status = MRC_STATUS_SUCCESS;
  struct ioGroup *iogPtr;
  UINT32         RankEnCmdS;
  UINT32         RankEnCmdN;
  UINT32         RankEnCtl;
  UINT32         RankEnCke;

  if (group == CmdVref) {
    status = GetSetCmdVref (Host, socket, ch, 0, DdrLevel, mode, value);
  } else {
    status = GetCmdCtlGroupList(Host, group, &maxIoGroup, &iogPtr);

    if (status == MRC_STATUS_SUCCESS) {

      EnableCCCOutputPath (Host, socket, ch, FALSE, &RankEnCmdS, &RankEnCmdN, &RankEnCtl, &RankEnCke);

      for (iog = 0; iog < maxIoGroup; iog++) {

        // Get IO Group number and side
        iogNum = (iogPtr + iog)->num;
        iogSide = (iogPtr + iog)->side;

        //
        // Skip MA14, MA15, MA16 for Late Cmd Clk to avoid spurious PM Idle power down command
        //
        if (!((((iogNum == 4) && (iogSide == SIDE_B)) ||
               ((iogNum == 5) && (iogSide == SIDE_B)) ||
               ((iogNum == 7) && (iogSide == SIDE_B))) &&
               ((mode & GSM_SKIP_CMD_FUB) != 0) &&
               (mode & (GSM_WRITE_OFFSET | GSM_FORCE_WRITE)))) {
                 status = GetSetCmdDelay (Host, socket, ch, iogNum, iogSide, mode, value);
               }

        // Update min/max values
        UpdateMinMaxUint (*value, minVal, maxVal);
      }

      EnableCCCOutputPath (Host, socket, ch, TRUE, &RankEnCmdS, &RankEnCmdN, &RankEnCtl, &RankEnCke);

    } // status
  }
  return status;
} // GetSetCmdGroupDelay

/*

  Get or Set and specific Knob inside a Cmd or Ctl group delays

  @param[in]      Host          Pointer to syshost
  @param[in]      Socket        Current socket under test (0-based)
  @param[in]      ChannelIndex  Current channel under test (0-based)
  @param[in]      KnobIndex     Current knob under test (0-based)
  @param[in]      Group         Specifies the MRC_GT group that will be under test
  @param[in]      Mode          Mode to access the register
                                GSM_READ_CSR: Read register directly instead of cache value
                                GSM_READ_ONLY: Only read, skip write portion
                                GSM_WRITE_OFFSET: Use the value as an offset to original
                                GSM_UPDATE_CACHE: Update cache
  @param[in,out]  DelayPtr      Pointer to read/write the value from current socket/channel/knob


*/
MRC_STATUS
GetSetCmdCtlIoKnob (
  IN        PSYSHOST          Host,
  IN        UINT8             Socket,
  IN        UINT8             ChannelIndex,
  IN        UINT8             KnobIndex,
  IN        MRC_GT            Group,
  IN        UINT8             Mode,
  IN  OUT   INT16             *DelayPtr
)
{
  MRC_STATUS     Status;
  UINT8          IoGroupNum;
  UINT8          IoGroupSide;
  UINT8          MaxIoGroup;
  struct ioGroup *IoGroupPtr;
  UINT32         RankEnCmdS;
  UINT32         RankEnCmdN;
  UINT32         RankEnCtl;
  UINT32         RankEnCke;

  Status = GetCmdCtlGroupList (Host, Group, &MaxIoGroup, &IoGroupPtr);

  if (Status != MRC_STATUS_SUCCESS) {
    return Status;
  }

  if (KnobIndex >= MaxIoGroup) {
    return MRC_STATUS_SIGNAL_NOT_SUPPORTED;
  }

  EnableCCCOutputPath (
    Host,
    Socket,
    ChannelIndex,
    FALSE,
    &RankEnCmdS,
    &RankEnCmdN,
    &RankEnCtl,
    &RankEnCke
    );

  IoGroupNum = (IoGroupPtr + KnobIndex)->num;
  IoGroupSide = (IoGroupPtr + KnobIndex)->side;

  GetSetCmdDelay (Host, Socket, ChannelIndex, IoGroupNum, IoGroupSide, Mode, DelayPtr);

  EnableCCCOutputPath (
    Host,
    Socket,
    ChannelIndex,
    TRUE,
    &RankEnCmdS,
    &RankEnCmdN,
    &RankEnCtl,
    &RankEnCke
    );

  return MRC_STATUS_SUCCESS;

}

/**
  Gets the CSR address and cache index for given IoGroup and side

  @param [in    ] Host     - Pointer to SYSHOST
  @param [in    ] ioGroup  - Pi group number
  @param [in    ] side     - Pi group side
  @param [   out] cmdIndex - Cache index for the CSR

  @retval CSR address for given Pi group number and side
**/
UINT32
GetCmdGroupAddress10nm (
  IN     PSYSHOST Host,
  IN     UINT8    ioGroup,
  IN     UINT8    side,
     OUT UINT8    *cmdIndex
  )
{
  UINT32 address = 0;

  switch (ioGroup) {
    case 0:
    case 1:
    case 2:
      if (side == SIDE_A) {
        //DDRCRCMDPICODING_CMDN
        address = DDRCRCMDPICODING_CMDN_MCIO_DDRIOEXT_REG;
        *cmdIndex = 0;
      }
      else {
        //DDRCRCMDPICODING_CKE
        address = DDRCRCMDPICODING_CKE_MCIO_DDRIOEXT_REG;
        *cmdIndex = 4;
      }
      break;
    case 3:
    case 4:
    case 5:
      if (side == SIDE_A) {
        //DDRCRCMDPICODING2_CMDN
        address = DDRCRCMDPICODING2_CMDN_MCIO_DDRIOEXT_REG;
        *cmdIndex = 1;
      }
      else {
        //DDRCRCMDPICODING2_CKE
        address = DDRCRCMDPICODING2_CKE_MCIO_DDRIOEXT_REG;
        *cmdIndex = 5;
      }
      break;
    case 6:
    case 7:
    case 8:
      if (side == SIDE_A) {
        //DDRCRCMDPICODING3_CMDN
        address = DDRCRCMDPICODING3_CMDN_MCIO_DDRIOEXT_REG;
        *cmdIndex = 2;
      }
      else {
        //DDRCRCMDPICODING3_CKE
        address = DDRCRCMDPICODING3_CKE_MCIO_DDRIOEXT_REG;
        *cmdIndex = 6;
      }
      break;
    case 9:
    case 10:
    case 11:
      if (side == SIDE_A) {
        //DDRCRCMDPICODING4_CMDN
        address = DDRCRCMDPICODING4_CMDN_MCIO_DDRIOEXT_REG;
        *cmdIndex = 3;
      }
      else {
        //DDRCRCMDPICODING4_CKE
        address = DDRCRCMDPICODING4_CKE_MCIO_DDRIOEXT_REG;
        *cmdIndex = 7;
      }
      break;

    case 0 + CC_SOUTH:
    case 1 + CC_SOUTH:
    case 2 + CC_SOUTH:
      if (side == SIDE_B) {
        //DDRCRCMDPICODING_CTL
        address = DDRCRCMDPICODING_CTL_MCIO_DDRIOEXT_REG;
        *cmdIndex = 8;
      }
      else {
        //DDRCRCMDPICODING_CMDS
        address = DDRCRCMDPICODING_CMDS_MCIO_DDRIOEXT_REG;
        *cmdIndex = 12;
      }
      break;
    case 3 + CC_SOUTH:
    case 4 + CC_SOUTH:
    case 5 + CC_SOUTH:
      if (side == SIDE_B) {
        //DDRCRCMDPICODING2_CTL
        address = DDRCRCMDPICODING2_CTL_MCIO_DDRIOEXT_REG;
        *cmdIndex = 9;
      }
      else {
        //DDRCRCMDPICODING2_CMDS
        address = DDRCRCMDPICODING2_CMDS_MCIO_DDRIOEXT_REG;
        *cmdIndex = 13;
      }
      break;
    case 6 + CC_SOUTH:
    case 7 + CC_SOUTH:
    case 8 + CC_SOUTH:
      if (side == SIDE_B) {
        //DDRCRCMDPICODING3_CTL
        address = DDRCRCMDPICODING3_CTL_MCIO_DDRIOEXT_REG;
        *cmdIndex = 10;
      }
      else {
        //DDRCRCMDPICODING3_CMDS
        address = DDRCRCMDPICODING3_CMDS_MCIO_DDRIOEXT_REG;
        *cmdIndex = 14;
      }
      break;
    case 9 + CC_SOUTH:
    case 10 + CC_SOUTH:
    case 11 + CC_SOUTH:
      if (side == SIDE_B) {
        //DDRCRCMDPICODING4_CTL
        address = DDRCRCMDPICODING4_CTL_MCIO_DDRIOEXT_REG;
        *cmdIndex = 11;
      }
      else {
        //DDRCRCMDPICODING4_CMDS
        address = DDRCRCMDPICODING4_CMDS_MCIO_DDRIOEXT_REG;
        *cmdIndex = 15;
      }
      break;
    default:
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_95);
      break;
  }
  return address;

} //GetCmdGroupAddress10nm

/**
  Multi-use function to either get or set command delays based on the provided group number

  @param [in    ] Host    - Pointer to SYSHOST
  @param [in    ] socket  - Socket number
  @param [in    ] ch      - Channel number
  @param [in    ] ioGroup - Group number
  @param [in    ] side    - Side number
  @param [in    ] mode    - GSM_READ_CSR - Read the data from hardware and not cache
                            GSM_READ_ONLY - Do not write
                            GSM_WRITE_OFFSET - Write offset and not value
                            GSM_FORCE_WRITE - Force the write
  @param [   out] value   - Value to program or offset

  @retval Pi delay value
**/
MRC_STATUS
GetSetCmdDelay (
  IN     PSYSHOST Host,
  IN     UINT8    socket,
  IN     UINT8    ch,
  IN     UINT8    ioGroup,
  IN     UINT8    side,
  IN     UINT8    mode,
     OUT INT16    *value
  )
{
  UINT32                                      csrReg = 0;
  UINT32                                      address;
  UINT8                                       regIndex;
  UINT8                                       logicVal;
  UINT8                                       piVal;
  UINT16                                      curVal;
  UINT16                                      piDelay;
  UINT16                                      maxVal = 0;
  INT16                                       tmp;
  struct socketNvram                          *socketNvram;
  DDRCRCMDPICODING_CMDN_MCIO_DDRIOEXT_STRUCT  cmdCsr;
  DDRCRCMDPICODING2_CMDN_MCIO_DDRIOEXT_STRUCT cmdCsr2;
  DDRCRCMDPICODING3_CMDN_MCIO_DDRIOEXT_STRUCT cmdCsr3;
  DDRCRCMDPICODING4_CMDN_MCIO_DDRIOEXT_STRUCT cmdCsr4;

  //RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
  //               "GetSetCmdDelay ioGroup=%d, side=%d\n", ioGroup, side);

  socketNvram = &Host->nvram.mem.socket[socket];
  if (IsBrsPresent (Host, socket)) {
    mode = mode | GSM_READ_CSR;
  }
  //
  // Maximum CMD delay
  //
  maxVal = MAX_CMD_DELAY;

  //
  // Point to the correct group
  //
  address = GetCmdGroupAddress10nm(Host, ioGroup, side, &regIndex);

  cmdCsr.Data = 0;
  cmdCsr2.Data = 0;
  cmdCsr3.Data = 0;
  cmdCsr4.Data = 0;

  //
  // Get register data
  //
  if (mode & GSM_READ_CSR) {
    csrReg =  MemReadPciCfgEp (socket, ch, address);
    switch (ioGroup % CC_SOUTH) {
      case 0:
      case 1:
      case 2:
        cmdCsr.Data = csrReg;
        break;
      case 3:
      case 4:
      case 5:
        cmdCsr2.Data = csrReg;
        break;
      case 6:
      case 7:
      case 8:
        cmdCsr3.Data = csrReg;
        break;
      case 9:
      case 10:
      case 11:
        cmdCsr4.Data = csrReg;
        break;
      default:
        RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_41);
    }
  } else {
    switch (regIndex % 4) {
      case 0:
        cmdCsr.Data = socketNvram->channelList[ch].cmdCsr[regIndex];
        break;
      case 1:
        cmdCsr2.Data = socketNvram->channelList[ch].cmdCsr[regIndex];
        break;
      case 2:
        cmdCsr3.Data = socketNvram->channelList[ch].cmdCsr[regIndex];
        break;
      case 3:
        cmdCsr4.Data = socketNvram->channelList[ch].cmdCsr[regIndex];
        break;
      default:
        //Automated add of default case. Please review.
        break;
    }
  }

  switch (ioGroup % CC_SOUTH) {
    case 0:
      logicVal = (UINT8)cmdCsr.Bits.cmdpilogicdelay0;
      piVal = (UINT8)cmdCsr.Bits.cmdpicode0;
      break;

    case 1:
      logicVal = (UINT8)cmdCsr.Bits.cmdpilogicdelay1;
      piVal = (UINT8)cmdCsr.Bits.cmdpicode1;
      break;

    case 2:
      logicVal = (UINT8)cmdCsr.Bits.cmdpilogicdelay2;
      piVal = (UINT8)cmdCsr.Bits.cmdpicode2;
      break;

    case 3:
      logicVal = (UINT8)cmdCsr2.Bits.cmdpilogicdelay3;
      piVal = (UINT8)cmdCsr2.Bits.cmdpicode3;
      break;

    case 4:
      logicVal = (UINT8)cmdCsr2.Bits.cmdpilogicdelay4;
      piVal = (UINT8)cmdCsr2.Bits.cmdpicode4;
      break;

    case 5:
      logicVal = (UINT8)cmdCsr2.Bits.cmdpilogicdelay5;
      piVal = (UINT8)cmdCsr2.Bits.cmdpicode5;
      break;

    case 6:
      logicVal = (UINT8)cmdCsr3.Bits.cmdpilogicdelay6;
      piVal = (UINT8)cmdCsr3.Bits.cmdpicode6;
      break;

    case 7:
      logicVal = (UINT8)cmdCsr3.Bits.cmdpilogicdelay7;
      piVal = (UINT8)cmdCsr3.Bits.cmdpicode7;
      break;

    case 8:
      logicVal = (UINT8)cmdCsr3.Bits.cmdpilogicdelay8;
      piVal = (UINT8)cmdCsr3.Bits.cmdpicode8;
      break;

    case 9:
      logicVal = (UINT8)cmdCsr4.Bits.cmdpilogicdelay9;
      piVal = (UINT8)cmdCsr4.Bits.cmdpicode9;
      break;

    case 10:
      logicVal = (UINT8)cmdCsr4.Bits.cmdpilogicdelay10;
      piVal = (UINT8)cmdCsr4.Bits.cmdpicode10;
      break;

    case 11:
      logicVal = (UINT8)cmdCsr4.Bits.cmdpilogicdelay11;
      piVal = (UINT8)cmdCsr4.Bits.cmdpicode11;
      break;

    default:
      logicVal = 0;
      piVal = 0;
      // Assert error
  } // switch ioGroup

  // Combine into delay
  curVal = (logicVal * 64) + piVal;

  // Read only?
  if (mode & GSM_READ_ONLY) {
    *value = (INT16)curVal;
  }
  // Handle writes
  else {

    // Adjust the current CMD delay value by value
    if (mode & GSM_WRITE_OFFSET) {
      tmp = curVal + *value;
    } else {
      //
      // Absolute value
      //
      tmp = *value;
    }

    // Check for boundaries
    // value is INT16 and can be a negative value
    // Value and curVal are UINT8
    // Do not allow negative wraparound (curVal = 3 and offset = -4)
    if (tmp > 0) {
      piDelay = tmp;
    } else {
      piDelay = 0;
    }
    // Ensure we do not exceed maximums (caller should enforce this)
    if (piDelay > maxVal) {
      piDelay = maxVal;
    }

    // Program the IO delay
    logicVal = (UINT8) (piDelay / 64);
    piVal    = (UINT8) (piDelay % 64);

    switch (ioGroup % CC_SOUTH) {
      case 0:
        cmdCsr.Bits.cmdpilogicdelay0 = logicVal;
        cmdCsr.Bits.cmdpicode0 = piVal;
        break;

      case 1:
        cmdCsr.Bits.cmdpilogicdelay1 = logicVal;
        cmdCsr.Bits.cmdpicode1 = piVal;
        break;

      case 2:
        cmdCsr.Bits.cmdpilogicdelay2 = logicVal;
        cmdCsr.Bits.cmdpicode2 = piVal;
        break;

      case 3:
        cmdCsr2.Bits.cmdpilogicdelay3 = logicVal;
        cmdCsr2.Bits.cmdpicode3 = piVal;
        break;

      case 4:
        cmdCsr2.Bits.cmdpilogicdelay4 = logicVal;
        cmdCsr2.Bits.cmdpicode4 = piVal;
        break;

      case 5:
        cmdCsr2.Bits.cmdpilogicdelay5 = logicVal;
        cmdCsr2.Bits.cmdpicode5 = piVal;
        break;

      case 6:
        cmdCsr3.Bits.cmdpilogicdelay6 = logicVal;
        cmdCsr3.Bits.cmdpicode6 = piVal;
        break;

      case 7:
        cmdCsr3.Bits.cmdpilogicdelay7 = logicVal;
        cmdCsr3.Bits.cmdpicode7 = piVal;
        break;

      case 8:
        cmdCsr3.Bits.cmdpilogicdelay8 = logicVal;
        cmdCsr3.Bits.cmdpicode8 = piVal;
        break;

      case 9:
        cmdCsr4.Bits.cmdpilogicdelay9 = logicVal;
        cmdCsr4.Bits.cmdpicode9 = piVal;
        break;

      case 10:
        cmdCsr4.Bits.cmdpilogicdelay10 = logicVal;
        cmdCsr4.Bits.cmdpicode10 = piVal;
        break;

      case 11:
        cmdCsr4.Bits.cmdpilogicdelay11 = logicVal;
        cmdCsr4.Bits.cmdpicode11 = piVal;
        break;

      default:
        //Automated add of default case. Please review.
        break;
    } // switch ioGroup

    switch (regIndex % 4) {
      case 0:
        csrReg = cmdCsr.Data;
        break;
      case 1:
        csrReg = cmdCsr2.Data;
        break;
      case 2:
        csrReg = cmdCsr3.Data;
        break;
      case 3:
        csrReg = cmdCsr4.Data;
        break;
      default:
        //Automated add of default case. Please review.
        break;
    }
    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
    if ((socketNvram->channelList[ch].cmdCsr[regIndex] != csrReg) || (mode & GSM_FORCE_WRITE)) {
      MemWritePciCfgEp (socket, ch, address, csrReg);
    }

  }

  //
  // Save to cache
  //
  if (mode & GSM_UPDATE_CACHE) {
    switch (regIndex % 4) {
      case 0:
        socketNvram->channelList[ch].cmdCsr[regIndex] = cmdCsr.Data;
        break;
      case 1:
        socketNvram->channelList[ch].cmdCsr[regIndex] = cmdCsr2.Data;
        break;
      case 2:
        socketNvram->channelList[ch].cmdCsr[regIndex] = cmdCsr3.Data;
        break;
      case 3:
        socketNvram->channelList[ch].cmdCsr[regIndex] = cmdCsr4.Data;
        break;
      default:
        //Automated add of default case. Please review.
        break;
    }
  }

  return MRC_STATUS_SUCCESS;
} // GetSetCmdDelay



/**
  Multi-use function to either get or set command vref

  @param [in    ] Host   - Pointer to SYSHOST
  @param [in    ] socket - Socket number
  @param [in    ] ch     - Channel number
  @param [in    ] mode   - GSM_READ_CSR - Read the data from hardware and not cache
                           GSM_READ_ONLY - Do not write
                           GSM_WRITE_OFFSET - Write offset and not value
                           GSM_FORCE_WRITE - Force the write
  @param [   out] value  - Value to program or offset

  @retval Pi delay value

**/
STATIC MRC_STATUS
GetSetCmdVrefHostSide (
  IN     PSYSHOST Host,
  IN     UINT8    socket,
  IN     UINT8    ch,
  IN     UINT8    mode,
     OUT INT16    *value
  )
{
  INT16                                         curVal = 0;
  INT16                                         vref;
  UINT16                                        maxVal = 0;
  UINT16                                        minVal = 0;
  INT16                                         tmp;
  UINT8                                         NumChPerMc;
  struct socketNvram                            *socketNvram;
  DDRCRDIMMVREFCONTROL1_MCIO_DDRIOEXT_STRUCT    ddrCrDimmVrefControl1;

  socketNvram = &Host->nvram.mem.socket[socket];
  NumChPerMc  = GetNumChannelPerMc ();

  //
  // Maximum and Minimum CMD delay
  //
  maxVal = MAX_CMD_VREF;
  minVal = MIN_CMD_VREF;

  //
  // Get register data
  //
  if (mode & GSM_READ_CSR) {
    ddrCrDimmVrefControl1.Data = MemReadPciCfgEp (socket, (GetMCID(Host, socket, ch) * NumChPerMc), DDRCRDIMMVREFCONTROL1_MCIO_DDRIOEXT_REG);
  } else {
    ddrCrDimmVrefControl1.Data = socketNvram->imc[GetMCID(Host, socket, ch)].dimmVrefControl1;
  }

  //
  // Get the CA Vref current setting
  //
  switch (ch % NumChPerMc) {
#if (MEM_IP_VER >= MEM_IP_17ww37e)
    case 0:
      curVal = (INT16)(ddrCrDimmVrefControl1.Bits.ch0dimmvrefctl >> 1);
      break;
    case 1:
      curVal = (INT16)(ddrCrDimmVrefControl1.Bits.ch1dimmvrefctl >> 1);
      break;
    case 2:
      curVal = (INT16)(ddrCrDimmVrefControl1.Bits.ch2dimmvrefctl >> 1);
      break;
#else // (MEM_IP_VER >= MEM_IP_17ww37e)
    case 0:
      curVal = (INT16)(ddrCrDimmVrefControl1.Bits.ch0cavrefctl >> 1);
      break;
    case 1:
      curVal = (INT16)(ddrCrDimmVrefControl1.Bits.ch1cavrefctl >> 1);
      break;
    case 2:
      curVal = (INT16)(ddrCrDimmVrefControl1.Bits.ch2cavrefctl >> 1);
      break;
#endif // (MEM_IP_VER >= MEM_IP_17ww37e)
    default:
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                       "GetSetCmdVrefHostSide called for unknown channel: %d!\n", ch);
        RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_140);
      break;
  }

  // Read only?
  if (mode & GSM_READ_ONLY) {
    *value = (INT16)curVal;
  }
  // Handle writes
  else {

    // Adjust the current CMD delay value by value
    if (mode & GSM_WRITE_OFFSET) {
      tmp = curVal + *value;
    } else {
      //
      // Absolute value
      //
      tmp = *value;
    }

    // Check for boundaries
    // value is INT16 and can be a negative value
    // Value and curVal are UINT8
    // Do not allow negative wraparound (curVal = 3 and offset = -4)
    if (tmp > 0) {
      vref = tmp;
    } else {
      vref = 0;
    }
    // Ensure we do not exceed maximum/minimum
    if (vref > maxVal) {
      vref = maxVal;
    }
    if (vref < minVal) {
      vref = minVal;
    }

    //
    // Program the new value
    //
    switch (ch % NumChPerMc) {
#if (MEM_IP_VER >= MEM_IP_17ww37e)
      case 0:
        ddrCrDimmVrefControl1.Bits.ch0dimmvrefctl = vref << 1;
        break;
      case 1:
        ddrCrDimmVrefControl1.Bits.ch1dimmvrefctl = vref << 1;
        break;
      case 2:
        ddrCrDimmVrefControl1.Bits.ch2dimmvrefctl = vref << 1;
        break;
#else // (MEM_IP_VER >= MEM_IP_17ww37e)
      case 0:
        ddrCrDimmVrefControl1.Bits.ch0cavrefctl = vref << 1;
        break;
      case 1:
        ddrCrDimmVrefControl1.Bits.ch1cavrefctl = vref << 1;
        break;
      case 2:
        ddrCrDimmVrefControl1.Bits.ch2cavrefctl = vref << 1;
        break;
#endif // (MEM_IP_VER >= MEM_IP_17ww37e)
      default:
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                       "GetSetCmdVrefHostSide called for unknown channel: %d!\n", ch);
        RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_140);
        break;
    }

    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
    if ((socketNvram->imc[GetMCID(Host, socket, ch)].dimmVrefControl1 != ddrCrDimmVrefControl1.Data) || (mode & GSM_FORCE_WRITE)) {
      MemWritePciCfgMC (socket, GetMCID(Host, socket, ch), DDRCRDIMMVREFCONTROL1_MCIO_DDRIOEXT_REG, ddrCrDimmVrefControl1.Data);
    }
  }

  //
  // Save to cache
  //
  if (mode & GSM_UPDATE_CACHE) {
    socketNvram->imc[GetMCID(Host, socket, ch)].dimmVrefControl1 = ddrCrDimmVrefControl1.Data;
  }

  return MRC_STATUS_SUCCESS;
} // GetSetCmdVref

/**
  Multi-use function to either get or set ERID vref

  @param [in    ] Host   - Pointer to SYSHOST
  @param [in    ] socket - Socket number
  @param [in    ] ch     - Channel number
  @param [in    ] mode   - GSM_READ_CSR - Read the data from hardware and not cache
                           GSM_READ_ONLY - Do not write
                           GSM_WRITE_OFFSET - Write offset and not value
                           GSM_FORCE_WRITE - Force the write
  @param [   out] value  - Value to program or offset

  @retval Pi delay value
**/
MRC_STATUS
GetSetEridVref (
  IN     PSYSHOST Host,
  IN     UINT8    socket,
  IN     UINT8    ch,
  IN     UINT8    mode,
     OUT INT16    *value
  )
{
  INT16                                   curVal;
  INT16                                   vref;
  INT16                                   tmp;
  UINT16                                  maxLimit;
  UINT16                                  minLimit;
  UINT16                                  usDelay;
  struct channelNvram                     (*channelNvList)[MAX_CH];
  DDRCRCLKCONTROLS_MCIO_DDRIOEXT_STRUCT  ddrCRClkControls;

  channelNvList = GetChannelNvList(Host, socket);

  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits(Host, DdrLevel, EridVref, &minLimit, &maxLimit, &usDelay);

  //
  // Get register data
  //
  if (mode & GSM_READ_CSR) {
    ddrCRClkControls.Data = MemReadPciCfgEp (socket, ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG);
  } else {
    ddrCRClkControls.Data = (*channelNvList)[ch].ddrCRClkControls;
  }

  //
  // Get the CA Vref current setting
  //
  curVal = (INT16)ddrCRClkControls.Bits.rxvref;

  // Read only?
  if (mode & GSM_READ_ONLY) {
    *value = curVal;
  } else { // Handle writes
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
      if (tmp >= minLimit) {
        vref = tmp;
      } else {
        vref = minLimit;
      }
      // Ensure we do not exceed maximums
      if (vref > maxLimit) {
        vref = maxLimit;
      }
    } else {
      //
      // Write absolute value
      //
      vref = *value;
    }

    //
    // Program the new value
    //
    ddrCRClkControls.Bits.rxvref = vref;

    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
    if ((ddrCRClkControls.Data != (*channelNvList)[ch].ddrCRClkControls) || (mode & GSM_FORCE_WRITE)) {
      MemWritePciCfgEp (socket, ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG, ddrCRClkControls.Data);
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
    (*channelNvList)[ch].ddrCRClkControls = ddrCRClkControls.Data;
  }

  return MRC_STATUS_SUCCESS;
} // GetSetEridVref

/**
  Multi-use function to either get or set control delays based on the provided group number

  @param [in    ] Host   - Pointer to SYSHOST
  @param [in    ] socket - Socket number
  @param [in    ] ch     - Channel number
  @param [in    ] clk    - Clock number
  @param [in    ] mode   - GSM_READ_CSR - Read the data from hardware and not cache
                           GSM_READ_ONLY - Do not write
                           GSM_WRITE_OFFSET - Write offset and not value
                           GSM_FORCE_WRITE - Force the write
  @param [   out] value  - Value to program

  @retval Pi delay value
**/
MRC_STATUS
GetSetClkDelay (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    clk,
  UINT8    mode,
  INT16    *value
  )
{
  UINT8                                 logicVal;
  UINT8                                 piVal;
  UINT16                                curVal;
  INT16                                 tmp;
  UINT16                                piDelay;
  struct socketNvram                    *socketNvram;
  DDRCRCLKPICODE_MCIO_DDRIOEXT_STRUCT   clkCsr;

  socketNvram = &Host->nvram.mem.socket[socket];

  //
  // Get register data
  //
  if (mode & GSM_READ_CSR) {
    clkCsr.Data = MemReadPciCfgEp (socket, ch, DDRCRCLKPICODE_MCIO_DDRIOEXT_REG);
  } else {
    clkCsr.Data = socketNvram->channelList[ch].clkCsr;
  }

  switch (clk) {
    case 0:
      logicVal = (UINT8)clkCsr.Bits.pilogicdelayrank0;
      piVal = (UINT8)clkCsr.Bits.pisettingrank0;
      break;

    case 1:
      logicVal = (UINT8)clkCsr.Bits.pilogicdelayrank1;
      piVal = (UINT8)clkCsr.Bits.pisettingrank1;
      break;

    case 2:
      logicVal = (UINT8)clkCsr.Bits.pilogicdelayrank2;
      piVal = (UINT8)clkCsr.Bits.pisettingrank2;
      break;

    case 3:
      logicVal = (UINT8)clkCsr.Bits.pilogicdelayrank3;
      piVal = (UINT8)clkCsr.Bits.pisettingrank3;
      break;

    default:
      logicVal = 0;
      piVal = 0;
      // Assert error
  }

  // Combine into delay
  curVal = (logicVal * 64) + piVal;

  // Read only?
  if (mode & GSM_READ_ONLY) {
    *value = (INT16)curVal;
  }
  // Handle writes
  else {

    // Adjust the current CMD delay value by offset
    if (mode & GSM_WRITE_OFFSET) {
      // Get the new offset (note: can be negative)
      tmp = curVal + *value;
    } else {
      // FORCE_WRITE
      tmp = *value;
    }
    // Check if we are below 0
    if (tmp >= 0) {
      piDelay = (UINT8) tmp;
    } else {
      piDelay = 128 + tmp;
    }
    if (piDelay > MAX_CK_DELAY) {
      piDelay = tmp - MAX_CK_DELAY;
    }

    // Program the IO delay
    logicVal = (UINT8) (piDelay / 64);
    piVal    = (UINT8) (piDelay % 64);

    switch (clk) {
      case 0:
        clkCsr.Bits.pilogicdelayrank0 = logicVal;
        clkCsr.Bits.pisettingrank0 = piVal;
        break;

      case 1:
        clkCsr.Bits.pilogicdelayrank1 = logicVal;
        clkCsr.Bits.pisettingrank1 = piVal;
        break;

      case 2:
        clkCsr.Bits.pilogicdelayrank2 = logicVal;
        clkCsr.Bits.pisettingrank2 = piVal;
        break;

      case 3:
        clkCsr.Bits.pilogicdelayrank3 = logicVal;
        clkCsr.Bits.pisettingrank3 = piVal;
        break;
      default:
        //Automated add of default case. Please review.
        break;
    }

    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
    if (Host->var.mem.socket[socket].ioInitdone == 1) {
      Resync(Host, socket, ch, clk);
    }

    if ((socketNvram->channelList[ch].clkCsr != clkCsr.Data) || (mode & GSM_FORCE_WRITE)) {
      MemWritePciCfgEp (socket, ch, DDRCRCLKPICODE_MCIO_DDRIOEXT_REG, clkCsr.Data);
    }

    if (Host->var.mem.socket[socket].ioInitdone == 1) {
      RelockPll(Host, socket, ch, clk);
    }
  }

  //
  // Save to cache
  //
  if (mode & GSM_UPDATE_CACHE) {
    socketNvram->channelList[ch].clkCsr = clkCsr.Data;
  }

  return MRC_STATUS_SUCCESS;
} // GetSetClkDelay

/**
  Set starting/initial values for clock and control signals.

  Initial values come from analog design.

  @param[in] Host   - Pointer to sysHost
  @param[in] Socket - Socket number

  @retval N/A
**/
VOID
SetStartingCCC (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  UINT8               ai;
  INT16               baseDelay;
  UINT8               mode = GSM_FORCE_WRITE + GSM_UPDATE_CACHE; // Use the cached value and don't read
  UINT8               picoPerPi;
  INT16               delay;
  TYPE_CHOPTYPE_ENUM  CpuChopType;
  INT16               maxDelay;
  INT16               minDelay;
  UINT8               numClkEntries;
  UINT8               numCmdEntries;
  UINT8               numCtlEntries;
  struct channelNvram (*channelNvList)[MAX_CH];
  UINT8               ch;
  UINT8               Clk;
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  INT16               AepClk[MAX_CLK];
  UINT8               *CtlDelay;
  UINT8               *CmdDelay;
  UINT16              *ClkDelay;
  IoGroupStruct       *CtlTableStruct;
  IoGroupStruct       *CmdTableStruct;
  IoGroupClkStruct    *ClkTableStruct;
  UINT8               MaxChDdr;

  OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_PACKAGE_DELAY_CCC, Socket);

  if (UbiosGenerationOrHsleEnabled ()) {
    OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_POST_PACKAGE_DELAY_CCC, Socket);
    return;
  }

  channelNvList = GetChannelNvList (Host, Socket);
  picoPerPi = GetPicoSecondsPerPiTickForFreq (Host->nvram.mem.ratioIndex);
  MaxChDdr  = GetMaxChDdr ();

#ifdef DEBUG_CODE_BLOCK
  AcquirePrintControl ();
#endif //DEBUG_CODE_BLOCK

  CpuChopType = GetChopType (GetCurrentSocketId ());

  GetCmdDelay (&numCmdEntries, &CmdDelay, &CmdTableStruct);
  GetCtlDelay (&numCtlEntries, &CtlDelay, &CtlTableStruct);
  GetClkDelay (&numClkEntries, &ClkDelay, &ClkTableStruct);


  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "SetStartingCCC => CpuSku=%2d, CtlEntries=%3d\n", CpuChopType, numCtlEntries);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "SetStartingCCC => CpuSku=%2d, CmdEntries=%3d\n", CpuChopType, numCmdEntries);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "SetStartingCCC => CpuSku=%2d, ClkEntries=%3d\n", CpuChopType, numClkEntries);

  for (ch = 0; ch < MaxChDdr; ch++) {

    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    // Calculate max and min delays and use them to determine a shared base delay.

    // Initialize with extreme values in the opposite direction. (Allowable range is 0 - 255 PI ticks.)
    maxDelay = 0;
    minDelay = MAX_CMD_DELAY;

    for (ai = 0; ai < numCtlEntries; ai++) {
      if (CtlTableStruct[ai].ch != ch || CtlDelay[ai] == 0xFF) {
        continue;
      }
      delay = CtlDelay[ai] / picoPerPi;
      if (delay > maxDelay) {
        maxDelay = delay;
      }
      if (delay < minDelay) {
        minDelay = delay;
      }
    }

    for (ai = 0; ai < numCmdEntries; ai++) {
      if (CmdTableStruct[ai].ch != ch || CmdDelay[ai] == 0xFF) {
        continue;
      }
      delay = CmdDelay[ai] / picoPerPi + 64;  // Additional 64 PI tick delay is due to 3N timing.
      if (delay > maxDelay) {
        maxDelay = delay;
      }
      if (delay < minDelay) {
        minDelay = delay;
      }
    }

    baseDelay = (MAX_CMD_DELAY + 1 - (maxDelay + minDelay)) / 2;

    RcDebugPrintWithDevice (SDBG_MAX, Socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "SetStartingCCC => CpuSku=%2d, Max Delay=%3d\n", CpuChopType, maxDelay);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "SetStartingCCC => CpuSku=%2d, Min Delay=%3d\n", CpuChopType, minDelay);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "SetStartingCCC => CpuSku=%2d, Base Delay=%3d\n", CpuChopType, baseDelay);

    // Apply base delay (and other modifiers) to control, command, and clock.

    for (ai = 0; ai < numCtlEntries; ai++) {
      if (CtlTableStruct[ai].ch != ch || CtlDelay[ai] == 0xFF) {
        continue;
      }
      delay = CtlDelay[ai] / picoPerPi + baseDelay;
      if (delay > 191) {
        delay = 191;
      } else if (delay < 64) {
        delay = 64;
      }
      GetSetCmdDelay (Host, Socket, CtlTableStruct[ai].ch, CtlTableStruct[ai].iog, CtlTableStruct[ai].side, mode, &delay);
    }

    for (ai = 0; ai < numCmdEntries; ai++) {
      if (CmdTableStruct[ai].ch != ch || CmdDelay[ai] == 0xFF) {
        continue;
      }
      delay = CmdDelay[ai] / picoPerPi + baseDelay + 64;  // Additional 64 PI tick delay is due to 3N timing.
      if (delay > 191) {
        delay = 191;
      }  // Delay cannot be less than 64 due to 3N timing adjustment, so "else if (delay < 64)" is omitted.
      GetSetCmdDelay (Host, Socket, CmdTableStruct[ai].ch, CmdTableStruct[ai].iog, CmdTableStruct[ai].side, mode, &delay);
    }

    dimmNvList = GetDimmNvList (Host, Socket, ch);
    for (Clk = 0; Clk < MAX_CLK; Clk++) {
      AepClk[Clk] = 0;
      if ((Clk == 0) || (Clk == 2)) {
        if (((*dimmNvList)[Clk/2].DcpmmPresent == 1) && !FeaturePcdGet (PcdCteBuild)) {
          AepClk[Clk] = 64;
        }
      }
    }

    for (ai = 0; ai < numClkEntries; ai++) {
      // Maximum of piPsFrqTable is 4, clk delay range is 128 (MAX_CK_DELAY). 4*128 - 1 = 511 (0x1FF)
      if ((ClkTableStruct[ai].ch != ch) || (ClkDelay[ai] >= (4*MAX_CK_DELAY - 1))) {
        continue;
      }
      delay = ClkDelay[ai] / picoPerPi + baseDelay + AepClk[ClkTableStruct[ai].clk];
      GetSetClkDelay (Host, Socket, ClkTableStruct[ai].ch, ClkTableStruct[ai].clk, mode, &delay);
    }

  }  // ch loop

#ifdef DEBUG_CODE_BLOCK
  DisplayCCCResults (Host, Socket);
  ReleasePrintControl ();
#endif //DEBUG_CODE_BLOCK

  OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_POST_PACKAGE_DELAY_CCC, Socket);

} // SetStartingCCC

/**

  Get min/max limits for Command, Control and Clock signals

  @param[in    ] Socket   - Processor socket within the system (0-based)
  @param[in    ] Ch       - Channel number
  @param[in    ] Dimm     - DIMM number
  @param[in    ] Level    - IO level to access
  @param[in    ] Group    - Command, clock or control group to access
  @param[   out] MinLimit - Minimum delay value allowed pointer
  @param[   out] MaxLimit - Maximum delay value allowed pointer

  @retval EFI_SUCESS if signal max/mins are returned
          EFI_NOT_FOUND otherwise
**/
EFI_STATUS
GetCmdGroupLimitsCpu (
  IN    UINT8     Socket,
  IN    UINT8     Ch,
  IN    UINT8     Dimm,
  IN    MRC_LT    Level,
  IN    MRC_GT    Group,
  OUT   UINT16    *MinLimit,
  OUT   UINT16    *MaxLimit
  )
{
  EFI_STATUS      Status = EFI_SUCCESS;

  *MinLimit = 0;

  switch (Group) {
    case CkAll:
      *MaxLimit = MAX_CLK_DELAY;
      *MinLimit = 0;
      break;
    case CtlGrp0:
    case CtlGrp1:
    case CtlGrp2:
    case CtlGrp3:
    case CtlGrp4:
    case CtlGrp5:
    case CtlAll:
      *MaxLimit = MAX_CTL_DELAY;
      *MinLimit = 0;
      break;
    case CmdGrp0:
    case CmdGrp1:
    case CmdGrp2:
    case CmdGrp3:
    case CmdGrp4:
    case CmdAll:
      *MaxLimit = MAX_CMD_DELAY;
      *MinLimit = 0;
      break;
    case CmdTxEq:
      *MaxLimit =  MAX_CMD_TX_EQ_VAL;
      *MinLimit = 0;
      break;
    case CmdVref:
      *MinLimit =  MIN_CMD_VREF;
      *MaxLimit =  MAX_CMD_VREF;
      break;
    default:
      Status = EFI_NOT_FOUND;
      break;
    }

  return Status;
} // GetCmdGroupLimitsCpu

/**

  Get min/max limits for Command, Control and Clock signals

  @param[in    ] Socket   - Processor socket within the system (0-based)
  @param[in    ] Ch       - Channel number, use ALL_CH to ignore Ch/Dimm parameters
  @param[in    ] Dimm     - DIMM number
  @param[in    ] Level    - IO level to access
  @param[in    ] Group    - Command, clock or control group to access
  @param[   out] MinLimit - Minimum delay value allowed pointer
  @param[   out] MaxLimit - Maximum delay value allowed pointer

  @retval EFI_SUCCESS if MinLimit and MaxLimit are updated
          EFI_INVALID_PARAMTER if bad parameter passed in
          EFI_NOT_FOUND otherwise
**/
EFI_STATUS
GetCmdGroupLimits (
  IN     UINT8  Socket,
  IN     UINT8  Ch,
  IN     UINT8  Dimm,
  IN     MRC_LT Level,
  IN     MRC_GT Group,
     OUT UINT16 *MinLimit,
     OUT UINT16 *MaxLimit
  )
{
  EFI_STATUS              Status;
  PSYSHOST                Host;
  struct dimmNvram        (*DimmNvList)[MAX_DIMM];

  Host = GetSysHostPointer();
  DimmNvList = NULL;
  if (Ch != ALL_CH) {
    if (Dimm >= MAX_DIMM) {
      return EFI_INVALID_PARAMETER;
    }
    DimmNvList = GetDimmNvList(Host, Socket, Ch);
  }

  Status = EFI_NOT_FOUND;
  //
  // If channel and DIMM are specified, use DDRT limits if DDRT DIMM is installed in the slot
  //
  if ((DimmNvList != NULL) && ((*DimmNvList)[Dimm].DcpmmPresent == 1)) {
    Status = GetCmdGroupLimitsFmc (Socket, Ch, Dimm, Level, Group, MinLimit, MaxLimit);
  }
  //
  // If FMC library doesn't support the requested Group or the slot does not have a DDRT DIMM,
  // use the CPU limits
  //
  if (Status != EFI_SUCCESS) {
    Status = GetCmdGroupLimitsCpu (Socket, Ch, Dimm, Level, Group, MinLimit, MaxLimit);
  }

  return Status;
}

/**

  Given an array of command delays relative to current clock and control delays,
  this function will combine shared settings in the DDRIO design and normalize the
  lowest command, clock or control value to their minimum limits.

  @param [in    ] Host     - Pointer to Host structure
  @param [in    ] socket   - Processor socket within the system (0-based)
  @param [in    ] ch       - DDR channel number within the processor socket (0-based)
  @param [in    ] listSize - Number of entries in each list
  @param [   out] value    - Array of pointers to listType

  @retval MRC_STATUS

**/
MRC_STATUS
SetCombinedCtlGroup (
  IN     PSYSHOST Host,
  IN     UINT8    socket,
  IN     UINT8    ch,
  IN     UINT16   listSize,
     OUT VOID     *value
  )
{
  UINT8                 dimm;
  UINT8                 rank;
  UINT8                 listIndex;
  UINT8                 ctlPiGroup;
  UINT8                 *Allocation;
  UINT8                 MaxCmdCtlGroup;
  struct ioGroup        *CmdCtlGroupSignals;
  INT16                 *ctlRight;
  INT16                 *ctlLeft;
  GSM_CSN               *ctlSignal;
  INT16                 *ctlValue;
  MRC_STATUS            status;
  GSM_CSEDGE_CTL        (*signalListEdge)[MAX_RANK_CH][NUM_SIGNALS_TO_SWEEP_LB];
  struct signalIOGroup  signalGroup;
  struct ddrRank        (*rankList)[MAX_RANK_DIMM];
  struct dimmNvram      (*dimmNvList)[MAX_DIMM];
  UINT8 maxIoGroup;
  struct ioGroup  *iogPtr;
  status = MRC_STATUS_SUCCESS;

  GetCmdCtlGroupAllTable (&CmdCtlGroupSignals, &MaxCmdCtlGroup);

  Allocation = RcAllocatePool (MaxCmdCtlGroup * (sizeof (*ctlRight) + sizeof (*ctlLeft) + sizeof (*ctlSignal) + sizeof (*ctlValue)));
  if (Allocation == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return MRC_STATUS_FAILURE;
  }

  ctlRight = (INT16*)((UINTN)Allocation);
  ctlLeft = (INT16*)((UINTN)ctlRight + (MaxCmdCtlGroup * sizeof (*ctlRight)));
  ctlSignal = (GSM_CSN*)((UINTN)ctlLeft + (MaxCmdCtlGroup * sizeof (*ctlLeft)));
  ctlValue = (INT16*)((UINTN)ctlSignal + (MaxCmdCtlGroup * sizeof (*ctlSignal)));

  status = GetCmdCtlGroupList(Host, CmdCtlAll, &maxIoGroup, &iogPtr);

  ZeroMem ((UINT8 *)ctlSignal, sizeof (ctlSignal));

  //
  // Initialize the edges for each CTL pi group
  //
  for (ctlPiGroup = 0; ctlPiGroup < maxIoGroup; ctlPiGroup++) {
    ctlLeft[ctlPiGroup] = 0;
    ctlRight[ctlPiGroup] = MEM_CHIP_POLICY_VALUE(Host, ctlPiGrp);
    ctlValue[ctlPiGroup] = 0;
  } // ctlPiGroup loop

  signalListEdge = value;

  dimmNvList = GetDimmNvList(Host, socket, ch);
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {

    rankList = GetRankNvList(Host, socket, ch, dimm);
    for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
      if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
        continue;
      }

        //
        // Loop through the list to create the composite left and right edges for each CTL group
        //
        for (listIndex = 0; listIndex < listSize; listIndex++) {

          //
          // Get the silicon pi group this signal belongs to
          //
          if ((*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].toSample == 0) {
            continue;
          }
          status = SignalToPiGroup (Host, (*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].signal, &signalGroup);

          //
          // Get the index into the CTL silicon pi group
          //
          for (ctlPiGroup = 0; ctlPiGroup < maxIoGroup; ctlPiGroup++) {
            if ((signalGroup.group.num == iogPtr[ctlPiGroup].num) &&
              (signalGroup.group.side == iogPtr[ctlPiGroup].side)) {

              //
              // We found the CTL pi group so break out of the loop
              //
              break;
            }
          } // ctlPiGroup loop

          if (ctlPiGroup >= maxIoGroup) {
            RcFreePool (Allocation);
            RC_FATAL_ERROR (ctlPiGroup < maxIoGroup, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_42);
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "The CTL Pi Group was never found");
            return MRC_STATUS_GROUP_NOT_SUPPORTED;
          }

          //
          // Update the composite left and right edges for the current CTL pi group relative to the clock
          //
          UpdateEdges((*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].le, (*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].re,
                      &ctlLeft[ctlPiGroup], &ctlRight[ctlPiGroup]);
          ctlValue[ctlPiGroup] = ((ctlLeft[ctlPiGroup] + ctlRight[ctlPiGroup])/2)%256;
          ctlSignal[ctlPiGroup] = (*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].signal;

          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                       "%a:\t CTL Pi Group %d: le = %d re = %d, ctlLeft = %d ctlRight = %d ctlValue = %d\n",
                        GetSignalStr (signalGroup.sig), ctlPiGroup,
                        (*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].le, (*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].re,
                        ctlLeft[ctlPiGroup], ctlRight[ctlPiGroup], ctlValue[ctlPiGroup]);
      } // listIndex loop
    } // rank loop
  } // dimm loop

  for (ctlPiGroup = 0; ctlPiGroup < maxIoGroup; ctlPiGroup++) {
    if (ctlValue[ctlPiGroup] != 0) {
      status = GetSetSignal (Host, socket, ch, DdrLevel, ctlSignal[ctlPiGroup], GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &ctlValue[ctlPiGroup]);
    }
  }

  RcFreePool (Allocation);

  return status;
} // SetCombinedCtlGroup


/**

  Given an array of command delays relative to current clock and control delays,
  this function will combine shared settings in the DDRIO design and normalize the
  lowest command, clock or control value to their minimum limits.

  @param [in    ] Host     - Pointer to Host structure
  @param [in    ] socket   - Processor socket within the system (0-based)
  @param [in    ] ch       - DDR channel number within the processor socket (0-based)
  @param [in    ] level    - IO level to access
  @param [in    ] mode     - Bit-field of different modes
  @param [in    ] listType - Selects type of each list
  @param [in    ] listSize - Number of entries in each list
  @param [   out] value    - Array of pointers to listType

  @retval MRC_STATUS

**/
MRC_STATUS
SetCombinedCmdGroup (
  IN     PSYSHOST Host,
  IN     UINT8    socket,
  IN     UINT8    ch,
  IN     MRC_LT   level,
  IN     UINT8    mode,
  IN     UINT8    listType,
  IN     UINT16   listSize,
     OUT VOID     *value
  )
{
  UINT8                 dimm;
  UINT8                 rank;
  UINT8                 clk;
  UINT8                 listIndex;
  UINT8                 cmdPiGroup;
  UINT8                 ckEnabled[MAX_CLK];
  UINT8                 ctlIndex[MAX_CLK];
  UINT8                 *Allocation;
  UINT8                 MaxCmdCtlGroup;
  struct ioGroup        *CmdCtlGroupSignals;
  INT16                 (*cmdLeft)[MAX_CLK];
  INT16                 (*cmdRight)[MAX_CLK];
  MRC_GT                (*cmdGroup)[MAX_CLK];
  GSM_CSN               (*cmdSignal)[MAX_CLK];
  INT16                 *cmdOffset;
  UINT16                *cmdPiDelaySum;
  UINT8                 *cmdPiDelayCount;
  INT16                 *cmdPiDelay;
  UINT8                 maxIoGroup;
  UINT8                 iog;
  struct ioGroup        *iogPtr;
  UINT8                 maxCmdGroupAll;
  struct ioGroup        *platformCmdGroupAll;
  UINT8                 maxCmdCtlGroupAll;
  struct ioGroup        *platformCmdCtlGroupAll;
  MRC_STATUS            status;
  GSM_CSVAL             (*signalList)[MAX_RANK_CH][NUM_SIGNALS_TO_SWEEP];
  GSM_CSEDGE            (*signalListEdge)[MAX_RANK_CH][NUM_SIGNALS_TO_SWEEP];
  GSM_CGVAL             (*groupList)[MAX_RANK_CH][8];
  GSM_CGEDGE            (*groupListEdge)[MAX_RANK_CH][MAX_CMD];
  struct signalIOGroup  signalGroup;
  struct ddrRank        (*rankList)[MAX_RANK_DIMM];
  struct dimmNvram      (*dimmNvList)[MAX_DIMM];

  status = MRC_STATUS_SUCCESS;

  GetCmdCtlGroupAllTable (&CmdCtlGroupSignals, &MaxCmdCtlGroup);

  Allocation = RcAllocatePool (MaxCmdCtlGroup * (sizeof (*cmdLeft) + sizeof (*cmdRight) + sizeof (*cmdGroup) + sizeof (*cmdSignal) +
                               sizeof (*cmdOffset) + sizeof (*cmdPiDelaySum) + sizeof (*cmdPiDelayCount) + sizeof (*cmdPiDelay)));
  if (Allocation == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return MRC_STATUS_FAILURE;
  }

  cmdLeft = (INT16(*)[MAX_CLK])((UINTN)Allocation);
  cmdRight = (INT16(*)[MAX_CLK])((UINTN)cmdLeft + (MaxCmdCtlGroup * sizeof (*cmdLeft)));
  cmdGroup = (MRC_GT(*)[MAX_CLK])((UINTN)cmdRight + (MaxCmdCtlGroup * sizeof (*cmdRight)));
  cmdSignal = (GSM_CSN(*)[MAX_CLK])((UINTN)cmdGroup + (MaxCmdCtlGroup * sizeof (*cmdGroup)));
  cmdOffset = (INT16*)((UINTN)cmdSignal + (MaxCmdCtlGroup * sizeof (*cmdSignal)));
  cmdPiDelaySum = (UINT16*)((UINTN)cmdOffset + (MaxCmdCtlGroup * sizeof (*cmdOffset)));
  cmdPiDelayCount = (UINT8*)((UINTN)cmdPiDelaySum + (MaxCmdCtlGroup * sizeof (*cmdPiDelaySum)));
  cmdPiDelay = (INT16*)((UINTN)cmdPiDelayCount + (MaxCmdCtlGroup * sizeof (*cmdPiDelayCount)));

  status = GetCmdCtlGroupList(Host, CmdAll, &maxCmdGroupAll, &platformCmdGroupAll);
  status = GetCmdCtlGroupList(Host, CmdCtlAll, &maxCmdCtlGroupAll, &platformCmdCtlGroupAll);

  dimmNvList = GetDimmNvList(Host, socket, ch);

  for (clk = 0; clk < MAX_CLK; clk++) {
    ckEnabled[clk] = 0;
    ctlIndex[clk] = 0;
  } // clk loop

  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    rankList = GetRankNvList(Host, socket, ch, dimm);
    for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {

      // If rank enabled
      if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
        continue;
      }

      ctlIndex[(*rankList)[rank].ckIndex] = (*rankList)[rank].ctlIndex;
      ckEnabled[(*rankList)[rank].ckIndex] = 1;
    } // rank loop
  } // dimm loop

  //
  // Initialize the edges for each CMD pi group
  //
  for (cmdPiGroup = 0; cmdPiGroup < maxCmdCtlGroupAll; cmdPiGroup++) {
    // Init CK composite edges to 255
    for (clk = 0; clk < MAX_CLK; clk++) {
      cmdLeft[cmdPiGroup][clk] = -UNMARGINED_CMD_EDGE;
      cmdRight[cmdPiGroup][clk] = UNMARGINED_CMD_EDGE;
      //cmdOffsetClk[cmdPiGroup][clk] = 0;
    } // clk loop
    cmdOffset[cmdPiGroup] = 0;
  } // cmdPiGroup loop

  if (listType == CMD_GRP_DELAY) {
    //
    // Combine edges if necessary
    //
    if (mode == GSM_COMBINE_EDGES) {
      groupListEdge = value;


      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        rankList = GetRankNvList(Host, socket, ch, dimm);

        for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
          if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          clk = (*rankList)[rank].ckIndex;

          //
          // Loop through the list to create the composite left and right edges for each CMD group
          //
          for (listIndex = 0; listIndex < listSize; listIndex++) {

            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                           "Platform Group = %a\n", GetPlatformGroupStr ((*groupListEdge)[(*rankList)[rank].rankIndex][listIndex].group));

            status = GetCmdCtlGroupList(Host, (*groupListEdge)[(*rankList)[rank].rankIndex][listIndex].group, &maxIoGroup, &iogPtr);
            for (iog = 0; iog < maxIoGroup; iog++) {

              //
              // Get the index into the CMD silicon pi group
              //
              for (cmdPiGroup = 0; cmdPiGroup < maxCmdGroupAll; cmdPiGroup++) {
                if (((iogPtr + iog)->num == platformCmdGroupAll[cmdPiGroup].num) &&
                    ((iogPtr + iog)->side == platformCmdGroupAll[cmdPiGroup].side)) {
                  //
                  // We found the CMD pi group so break out of the loop
                  //
                  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                                 "Found CMD Pi group: %d side %d\n", platformCmdGroupAll[cmdPiGroup].num, platformCmdGroupAll[cmdPiGroup].side);
                  break;
                }
              } // cmdPiGroup loop

              cmdLeft[cmdPiGroup][clk] = (*groupListEdge)[(*rankList)[rank].rankIndex][listIndex].le;
              cmdRight[cmdPiGroup][clk] = (*groupListEdge)[(*rankList)[rank].rankIndex][listIndex].re;
              cmdGroup[cmdPiGroup][clk] = (*groupListEdge)[(*rankList)[rank].rankIndex][listIndex].group;
              cmdSignal[cmdPiGroup][clk] = gsmCsnDelim;
              cmdOffset[cmdPiGroup] = 0;
              RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                             "cmdLeft[%d][%d] = %d : cmdRight[%d][%d] = %d\n", cmdPiGroup, clk,
                             (*groupListEdge)[(*rankList)[rank].rankIndex][listIndex].le,cmdPiGroup, clk,
                             (*groupListEdge)[(*rankList)[rank].rankIndex][listIndex].re);
            } // iog loop
          } // listIndex loop
        } // rank loop
      } // dimm loop

      //
      // Evaluate CMD margins and offset CMD, CTL, and CLK appropriately
      //
      EvaluateCMDMargins(Host, socket, ch, ckEnabled, ctlIndex, cmdLeft, cmdRight, cmdGroup, cmdSignal, cmdOffset);


    } else {
      //
      // Average center points
      //
      groupList = value;

      //
      // Initialize for each CMD pi group
      //
      ZeroMem( cmdPiDelaySum, maxCmdGroupAll * sizeof (UINT16));
      ZeroMem( cmdPiDelayCount, maxCmdGroupAll);

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        rankList = GetRankNvList(Host, socket, ch, dimm);

        for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
          if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          //
          // Loop through the list to create the composite left and right edges for each CMD group
          //
          for (listIndex = 0; listIndex < listSize; listIndex++) {

            status = GetCmdCtlGroupList(Host, (*groupList)[(*rankList)[rank].rankIndex][listIndex].group, &maxIoGroup, &iogPtr);
            for (iog = 0; iog < maxIoGroup; iog++) {
              //
              // Get the index into the CMD silicon pi group
              //
              for (cmdPiGroup = 0; cmdPiGroup < maxCmdGroupAll; cmdPiGroup++) {
                if (((iogPtr + iog)->num == platformCmdGroupAll[cmdPiGroup].num) &&
                    ((iogPtr + iog)->side == platformCmdGroupAll[cmdPiGroup].side)) {

                  //
                  // Add this pi delay to the total and increment the count
                  //
                  cmdPiDelaySum[cmdPiGroup] += (*groupList)[(*rankList)[rank].rankIndex][listIndex].value;
                  cmdPiDelayCount[cmdPiGroup]++;

                  //
                  // We found the CMD pi group so break out of the loop
                  //
                  break;
                }
              } // cmdPiGroup loop
            } // iog group
          } // listIndex loop
        } // rank loop
      } // dimm loop

      //
      // Calculate the pi delay for each CMD si group
      //
      for (cmdPiGroup = 0; cmdPiGroup < maxCmdGroupAll; cmdPiGroup++) {
        if (cmdPiDelayCount[cmdPiGroup] == 0) {
          continue;
        }

        cmdPiDelay[cmdPiGroup] = (INT16)(cmdPiDelaySum[cmdPiGroup] / cmdPiDelayCount[cmdPiGroup]);
      } // cmdPiGroup loop

      //
      // Program the delay
      //
      for (cmdPiGroup = 0; cmdPiGroup < maxCmdGroupAll; cmdPiGroup++) {
        GetSetCmdDelay (Host, socket, ch, platformCmdGroupAll[cmdPiGroup].num, platformCmdGroupAll[cmdPiGroup].side,
                        GSM_WRITE_OFFSET + GSM_UPDATE_CACHE, &cmdPiDelay[cmdPiGroup]);
      } // cmdPiGroup loop
    }

  } else if (listType == CMD_SIGNAL_DELAY) {

    //
    // Combine edges if necessary
    //
    if (mode == GSM_COMBINE_EDGES) {

      signalListEdge = value;

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        rankList = GetRankNvList(Host, socket, ch, dimm);

        for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
          if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          //
          // Loop through the list to create the composite left and right edges for each CMD group
          //
          for (listIndex = 0; listIndex < listSize; listIndex++) {

            clk = (*rankList)[rank].ckIndex;

            //
            // Get the silicon pi group this signal belongs to
            //
            status = SignalToPiGroup (Host, (*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].signal, &signalGroup);

            //
            // Get the index into the CMD silicon pi group
            //
            for (cmdPiGroup = 0; cmdPiGroup < maxCmdGroupAll; cmdPiGroup++) {
              if ((signalGroup.group.num == platformCmdGroupAll[cmdPiGroup].num) &&
                  (signalGroup.group.side == platformCmdGroupAll[cmdPiGroup].side)) {
                //
                // We found the CMD pi group so break out of the loop
                //
                break;
              }
            } // cmdPiGroup loop

            RC_FATAL_ERROR (cmdPiGroup != maxCmdGroupAll, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_43);
            //
            // Update the composite left and right edges for the current CMD pi group relative to the clock
            //
            UpdateEdges((*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].le, (*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].re,
                        &cmdLeft[cmdPiGroup][clk], &cmdRight[cmdPiGroup][clk]);
            cmdGroup[cmdPiGroup][clk] = MrcGtDelim;
            cmdSignal[cmdPiGroup][clk] = (*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].signal;
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                    "%a:\t CMD Pi Group %d clk %d: le %d re = %d, cmdLeft = %d cmdRight = %d\n",
                                    GetSignalStr (signalGroup.sig), cmdPiGroup, clk,
                                    (*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].le, (*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].re,
                                    cmdLeft[cmdPiGroup][clk], cmdRight[cmdPiGroup][clk]);
          } // listIndex loop
        } // rank loop
      } // dimm loop

      //
      // Evaluate CMD margins and offset CMD, CTL, and CLK appropriately
      //
      EvaluateCMDMargins(Host, socket, ch, ckEnabled, ctlIndex, cmdLeft, cmdRight, cmdGroup, cmdSignal, cmdOffset);

    } else {
      //
      // Average center points
      //
      signalList = value;

      //
      // Initialize for each CMD pi group
      //
      ZeroMem (cmdPiDelaySum, maxCmdGroupAll * sizeof(UINT16));
      ZeroMem (cmdPiDelayCount, maxCmdGroupAll);

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        rankList = GetRankNvList(Host, socket, ch, dimm);

        for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
          if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          //
          // Loop through the list to create the composite left and right edges for each CMD group
          //
          for (listIndex = 0; listIndex < listSize; listIndex++) {

            //
            // Get the silicon pi group this signal belongs to
            //
            status = SignalToPiGroup (Host, (*signalList)[(*rankList)[rank].rankIndex][listIndex].signal, &signalGroup);

            //
            // Get the index into the CMD silicon pi group
            //
            for (cmdPiGroup = 0; cmdPiGroup < maxCmdGroupAll; cmdPiGroup++) {
              if ((signalGroup.group.num == platformCmdGroupAll[cmdPiGroup].num) &&
                  (signalGroup.group.side == platformCmdGroupAll[cmdPiGroup].side)) {

                //
                // Add this pi delay to the total and increment the count
                //
                cmdPiDelaySum[cmdPiGroup] += (*signalList)[(*rankList)[rank].rankIndex][listIndex].value;
                cmdPiDelayCount[cmdPiGroup]++;

                //
                // We found the CMD pi group so break out of the loop
                //
                break;
              }
            } // cmdPiGroup loop
          } // listIndex loop
        } // rank loop
      } // dimm loop

      //
      // Calculate the pi delay for each CMD pi group
      //
      for (cmdPiGroup = 0; cmdPiGroup < maxCmdGroupAll; cmdPiGroup++) {
        if (cmdPiDelayCount[cmdPiGroup] == 0) {
          continue;
        }

        cmdPiDelay[cmdPiGroup] = (INT16)(cmdPiDelaySum[cmdPiGroup] / cmdPiDelayCount[cmdPiGroup]);
      } // cmdPiGroup loop

      //
      // Program the delay
      //
      for (cmdPiGroup = 0; cmdPiGroup < maxCmdGroupAll; cmdPiGroup++) {
        GetSetCmdDelay (Host, socket, ch, platformCmdGroupAll[cmdPiGroup].num, platformCmdGroupAll[cmdPiGroup].side,
                        GSM_WRITE_OFFSET + GSM_UPDATE_CACHE, &cmdPiDelay[cmdPiGroup]);
      } // cmdPiGroup loop

    }
  } else if (listType == CMDCTL_SIGNAL_DELAY) {

    //
    // Combine edges if necessary
    //
    if (mode == GSM_COMBINE_EDGES) {
      RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                           "Calculate Composite edges per PI Group per DIMM\n");

      signalListEdge = value;

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        rankList = GetRankNvList(Host, socket, ch, dimm);

        for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
          if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          //skip ranks >0 if rdimm
          if ((Host->nvram.mem.dimmTypePresent != UDIMM) && (rank > 0)) {
            continue;
          }

          //
          // Loop through the list to create the composite left and right edges for each CMD group
          //
          for (listIndex = 0; listIndex < listSize; listIndex++) {

            clk = (*rankList)[rank].ckIndex;

            //
            // Get the silicon pi group this signal belongs to
            //
            status = SignalToPiGroup (Host, (*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].signal, &signalGroup);

            //
            // Get the index into the CMD silicon pi group
            //

            for (cmdPiGroup = 0; cmdPiGroup < maxCmdCtlGroupAll; cmdPiGroup++) {
              if ((signalGroup.group.num == platformCmdCtlGroupAll[cmdPiGroup].num) &&
                  (signalGroup.group.side == platformCmdCtlGroupAll[cmdPiGroup].side)) {
                //
                // We found the CMD pi group so break out of the loop
                //
                break;
              }
            } // cmdPiGroup loop

            RC_FATAL_ERROR (cmdPiGroup != maxCmdCtlGroupAll, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_44);

            //
            // Update the composite left and right edges for the current CMD pi group relative to the clock
            //
            UpdateEdges((*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].le, (*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].re,
                        &cmdLeft[cmdPiGroup][clk], &cmdRight[cmdPiGroup][clk]);
              cmdGroup[cmdPiGroup][clk] = MrcGtDelim;
              cmdSignal[cmdPiGroup][clk] = (*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].signal;
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                           "%8a: CMD/CTL PI Group %2d, Group Num %2d, Side %2d,  clk %d: le %3d re = %3d, Left = %3d Right = %3d\n",
                           GetSignalStr (signalGroup.sig), cmdPiGroup, signalGroup.group.num, signalGroup.group.side, clk,
                           (*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].le, (*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].re,
                           cmdLeft[cmdPiGroup][clk], cmdRight[cmdPiGroup][clk]);
          } // listIndex loop
        } // rank loop
      } // dimm loop

      //
      // Evaluate CMD margins and offset CMD, CTL, and CLK appropriately
      //
      EvaluateCMDMargins(Host, socket, ch, ckEnabled, ctlIndex, cmdLeft, cmdRight, cmdGroup, cmdSignal, cmdOffset);

    } else {
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                           "ERROR: This SetCombinedCmd mode not supported!\n");

    }
  } else {
    status = MRC_STATUS_GROUP_NOT_SUPPORTED;
  }

  RcFreePool (Allocation);

  return status;
} // SetCombinedCmdGroup

/**
  Given an array of RCD QxCA Training delays relative to clock and control delays,
  this function will combine shared settings in the DDRIO design and normalize the
  lowest command, clock or control value to their minimum limits.

  @param[in] Host           - Pointer to host structure
  @param[in] Socket         - Processor socket within the system (0-based)
  @param[in] Ch             - DDR channel number within the processor socket (0-based)
  @param[in] SignalRange    - Total number of signals swept
  @param[in] SignalListEdge - Array of RCD QxCA Training delays

  @retval None
**/
VOID
SetCombinedBacksideCmdGroup (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN UINT16     SignalRange,
  IN GSM_CSEDGE (*SignalListEdge)[MAX_CH][MAX_RANK_CH][BACKSIDE_SIGNAL_RANGE]
  )
{
  UINT8             Dimm;
  UINT8             Rank;
  INT16             CompositeLeftEdge;
  INT16             CompositeRightEdge;
  UINT8             Signal;
  INT16             ClkCtlpiDelay;
  INT16             CmdpiDelay;
  UINT32            PrevDebugLevel;
  struct ddrRank    (*RankList)[MAX_RANK_DIMM];
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];

  UINT16            MinVal = 0xFF;
  UINT16            MaxVal = 0;

  DimmNvList = GetDimmNvList(Host, Socket, Ch);

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    //
    // Excluded UDIMMs, AEP DIMMs, and Revision 1 DIMMS
    //
    if (((*DimmNvList)[Dimm].dimmPresent == 0) ||
        (Host->nvram.mem.dimmTypePresent == UDIMM) ||
        ((*DimmNvList)[Dimm].DcpmmPresent == 1) ||
        ((*DimmNvList)[Dimm].DimmRevType < RCD_REV_2)) {
      continue;
    }

    //
    // initialize composite right and left edge for each dimm
    //
    CompositeLeftEdge = 0;
    CompositeRightEdge = BACKSIDE_SWEEP_RANGE - 1;

    RankList = GetRankNvList (Host, Socket, Ch, Dimm);

    for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {

      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
        continue;
      }

      //
      // Loop through the signals to create a composite left and right edges for all signals per dimm
      //
      for (Signal = 0; Signal < SignalRange; Signal++) {
        //
        // Find the maximum left edge
        //
        if (CompositeLeftEdge < ((*SignalListEdge)[Ch][(*RankList)[Rank].rankIndex][Signal].le)) {
          CompositeLeftEdge = (*SignalListEdge)[Ch][(*RankList)[Rank].rankIndex][Signal].le;
        }

        //
        // Find the minimum right edge
        //
        if (CompositeRightEdge > ((*SignalListEdge)[Ch][(*RankList)[Rank].rankIndex][Signal].re)) {
          CompositeRightEdge = (*SignalListEdge)[Ch][(*RankList)[Rank].rankIndex][Signal].re;
        }
      } // Signal loop
    } // Rank loop

    //
    // Calculate RCD QxCA trained offset
    //
    (*DimmNvList)[Dimm].QxCAClkOffset = ((CompositeLeftEdge - BACKSIDE_SWEEP_END) + (CompositeRightEdge - BACKSIDE_SWEEP_END)) / 2;
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "RCD QxCA Final Trained Value = %d, CompositeLeftEdge = %d, CompositeRightEdge = %d\n",
                            (*DimmNvList)[Dimm].QxCAClkOffset, CompositeLeftEdge, CompositeRightEdge);

    //
    // Maintain the CLK/CTL and CMD relationship (CMD-CLK=trained offset) by programing the normalized settings
    //
    if ((*DimmNvList)[Dimm].QxCAClkOffset < 0){
      //
      // Negative Offset
      //
      ClkCtlpiDelay = -(*DimmNvList)[Dimm].QxCAClkOffset;
      CmdpiDelay = 0;
    } else {
      //
      // Positive Offset
      //
      ClkCtlpiDelay = 0;
      CmdpiDelay = (*DimmNvList)[Dimm].QxCAClkOffset;
    }

    //
    // Program final RCD QxCA trained delay for Clk, Ctl, and Cmd
    //
    PrevDebugLevel = GetDebugLevel ();
    if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
      SetDebugLevel (SDBG_ERROR);
    }
    GetSetClkDelayCore (Host, Socket, Ch, Dimm, LrbufLevel, 0, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &ClkCtlpiDelay);
    GetSetCtlGroupDelayCore (Host, Socket, Ch, Dimm, LrbufLevel, CtlAll, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &ClkCtlpiDelay, &MinVal, &MaxVal);
    GetSetCmdGroupDelayCore (Host, Socket, Ch, Dimm, LrbufLevel, CmdAll, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &CmdpiDelay, &MinVal, &MaxVal);
    SetDebugLevel (PrevDebugLevel);

  } // Dimm loop
} // SetCombinedBacksideCmdGroup

/**
  Checks of the eyes passed as parameters are under the acceptance criteria

  @param[in] Host       - Pointer to host structure
  @param[in] Socket     - Processor socket within the system (0-based)
  @param[in] Channel    - DDR channel number within the processor socket (0-based)
  @param[in] CmdPiGroup - Command pi group delay
  @param[in] Clk        - Clock delay
  @param[in] CmdLeft    - Array of command values on the left
  @param[in] CmdRight   - Array of command values on the right

  @retval None

  **/
VOID
CheckCmdEyeWidth (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    CmdPiGroup,
  IN UINT8    Clk,
  IN INT16    (*CmdLeft)[MAX_CLK],
  IN INT16    (*CmdRight)[MAX_CLK]
  )
{
  UINT8 Dimm;
  UINT8 Rank;

#ifdef DDR5_SUPPORT
  if (IsBrsPresent (Host, Socket)) {
    if ((CmdLeft[CmdPiGroup][Clk] != -UNMARGINED_CMD_EDGE) && (CmdRight[CmdPiGroup][Clk] != UNMARGINED_CMD_EDGE)) {
      if ((CmdRight[CmdPiGroup][Clk] - CmdLeft[CmdPiGroup][Clk]) < MinimumMargin(Host, CmdAll)) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                "Eye width is too small: %d\n",
                                CmdRight[CmdPiGroup][Clk] - CmdLeft[CmdPiGroup][Clk]);
        OutputWarning(WARN_CMD_CLK_TRAINING, 0, Socket, Channel, NO_DIMM, NO_RANK);
        DisableChannelSw (Host, Socket, Channel);
      }
    }
  } else
#endif
  {
    if ((CmdRight[CmdPiGroup][Clk] - CmdLeft[CmdPiGroup][Clk]) < MinimumMargin(Host, CmdAll)) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "Eye width is too small: %d\n", CmdRight[CmdPiGroup][Clk] - CmdLeft[CmdPiGroup][Clk]);

      //
      // DDR CLK mapping in ICX
      //    Dimm 0: clk 0(RDIMM)/1(UDIMM Rank 1)
      //    Dimm 1: clk 2(RDIMM)/3(UDIMM Rank 1)
      //
      if (Host->nvram.mem.dimmTypePresent == RDIMM) {
        Dimm = Clk / 2;
        Rank = NO_RANK;
      } else {
        Dimm = Clk / 2;
        Rank = Clk % 2;
      }

      OutputWarning (WARN_CMD_CLK_TRAINING, WARN_CMD_PI_GROUP_SMALL_EYE, Socket, Channel, Dimm, Rank);
      DisableChannelSw (Host, Socket, Channel);
    }
  }
}

/**
  Evaluates the CMD margins and determines the appropriate offset

  @param [in    ] Host       - Pointer to Host structure
  @param [in    ] socket     - Processor socket within the system (0-based)
  @param [in    ] ch         - Memory channel number
  @param [in    ] ckEnabled  - Array of clocks enabled
  @param [in    ] ctlEnabled - Array of control indexes
  @param [in out] cmdLeft    - Array of command values for the left side
  @param [in out] cmdRight   - Array of command values for the right side
  @param [in    ] group      - Command group
  @param [in    ] cmdSignal  - Command signal
  @param [   out] cmdOffset  - Command offset

  @retval MRC_STATUS
**/
VOID
EvaluateCMDMargins (
  IN     PSYSHOST Host,
  IN     UINT8    socket,
  IN     UINT8    ch,
  IN     UINT8    ckEnabled[MAX_CLK],
  IN     UINT8    ctlIndex[MAX_CLK],
  IN OUT INT16    (*cmdLeft)[MAX_CLK],
  IN OUT INT16    (*cmdRight)[MAX_CLK],
  IN     MRC_GT   (*group)[MAX_CLK],
  IN     GSM_CSN  (*cmdSignal)[MAX_CLK],
     OUT INT16    *cmdOffset
  )
{
  UINT8                 dimm;
  UINT8                 rank;
  UINT8                 clk;
  UINT8                 cmdPiGroup;
  UINT8                 MaxCmdCtlGroup;
  struct ioGroup        *CmdCtlGroupSignals;
  INT16                 minCmdOffset;
  INT16                 maxCmdOffset;
  INT16                 (*cmdOffsetClk)[MAX_CLK];
  INT16                 maxLeftOffset;
  UINT16                ctlMinVal;
  UINT16                ctlMaxVal;
  INT16                 minRightOffset;
  INT16                 ckOffset[MAX_CLK];
  UINT8                 earlyCmdClkDisable = 0;
  UINT8                 parityWorkaround = 0;
  struct ddrRank        (*rankList)[MAX_RANK_DIMM];
  struct dimmNvram      (*dimmNvList)[MAX_DIMM];
  UINT8                 maxCmdGroupAll;
  UINT8                 CmdSetupOffset;
  struct ioGroup        *platformCmdGroupAll;
  MRC_STATUS            status;
  struct baseMargin     Margins;
  SYS_SETUP             *Setup;

  Setup = GetSysSetupPointer ();

  GetCmdCtlGroupAllTable (&CmdCtlGroupSignals, &MaxCmdCtlGroup);
  cmdOffsetClk = (INT16(*)[MAX_CLK]) RcAllocatePool (MaxCmdCtlGroup * sizeof (*cmdOffsetClk));
  if (cmdOffsetClk == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return;
  }

  status = GetCmdCtlGroupList (Host, CmdCtlAll, &maxCmdGroupAll, &platformCmdGroupAll);
  CmdSetupOffset = Setup->mem.cmdSetupPercentOffset;

  //
  // Determine if early CMD/CLK training has been disabled
  //
  dimmNvList = GetDimmNvList (Host, socket, ch);

  //
  // Determine if early CMD/CLK training has been disabled
  //
  earlyCmdClkDisable = ((!IsMemFlowEnabled (EarlyCmdClkTraining)) || parityWorkaround);

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "Calculate offset per PI group per DIMM \n");

  for (clk = 0; clk < MAX_CLK; clk++) {
    if (ckEnabled[clk] == 0) {
      continue;
    }
    //
    // Initialize these values so the maximum CMD delay can be determined
    //
    minCmdOffset = 255;
    maxCmdOffset = -255;

    //
    // Determine the offset for each CMD pi group and the max offset
    //
    for (cmdPiGroup = 0; cmdPiGroup < maxCmdGroupAll; cmdPiGroup++) {

      //
      // Make sure the eye width is large enough
      //
      CheckCmdEyeWidth (Host, socket, ch, cmdPiGroup, clk, cmdLeft, cmdRight);

      if ((cmdLeft[cmdPiGroup][clk] != -UNMARGINED_CMD_EDGE) && (cmdRight[cmdPiGroup][clk] != UNMARGINED_CMD_EDGE)){
        cmdOffsetClk[cmdPiGroup][clk] = (cmdLeft[cmdPiGroup][clk] + cmdRight[cmdPiGroup][clk]) / 2;

        UpdateMinMaxInt(cmdOffsetClk[cmdPiGroup][clk], &minCmdOffset, &maxCmdOffset);

        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                       "CMD Pi Group %2d clk %d cmdOffset %d\n",
                       cmdPiGroup, clk, cmdOffsetClk[cmdPiGroup][clk]);
      }
    } // cmdPiGroup loop

  } // clk offset

  //
  // Calculate the final offset for each CMD pi group
  //
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                       "Calculate Final offset per PI group per CH \n");

  for (cmdPiGroup = 0; cmdPiGroup < maxCmdGroupAll; cmdPiGroup++) {
    // Track minCgOffset, maxCgOffset across all CK groups
    minCmdOffset = 255;
    maxCmdOffset = -255;

    for (clk = 0; clk < MAX_CLK; clk++) {
      if (ckEnabled[clk]) {
        if ((cmdLeft[cmdPiGroup][clk] != -UNMARGINED_CMD_EDGE) && (cmdRight[cmdPiGroup][clk] != UNMARGINED_CMD_EDGE)){
          //if(moveClk) {
          //  UpdateMinMaxInt(cmdPiShift[cmdPiGroup][clk], &minCmdOffset, &maxCmdOffset);
          //} else {
          UpdateMinMaxInt(cmdOffsetClk[cmdPiGroup][clk], &minCmdOffset, &maxCmdOffset);
        }
      }
    } // clk loop

    // Save common CMD offset
    cmdOffset[cmdPiGroup] = (maxCmdOffset + minCmdOffset) / 2;
    //
    // Setup/Hold parameter in late CMD CLK (early CMD CLK disabled OR early CMD CLK executed already)
    //
    if (earlyCmdClkDisable || (Host->var.mem.earlyCmdClkExecuted == 1)) {
      cmdOffset[cmdPiGroup] = maxCmdOffset - (((PERCENT - CmdSetupOffset)* (maxCmdOffset - minCmdOffset)) / PERCENT);
    }

    //
    // Write the CMD pi Group
    //
    if ((maxCmdOffset != -255) && (minCmdOffset != 255)){
      GetSetCmdDelay (Host, socket, ch, platformCmdGroupAll[cmdPiGroup].num, platformCmdGroupAll[cmdPiGroup].side,
                    GSM_WRITE_OFFSET + GSM_UPDATE_CACHE, &cmdOffset[cmdPiGroup]);

      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "CMD Pi Group %2d: maxOffset = %d, minOffset = %d, cmdOffset = %d\n",
                   cmdPiGroup, maxCmdOffset, minCmdOffset, cmdOffset[cmdPiGroup]);
    }
  } // cmdPiGroup loop


  //
  // Determine the clock offset
  //
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                       "Calculate Clk offset \n");
  for (cmdPiGroup = 0; cmdPiGroup < maxCmdGroupAll; cmdPiGroup++) {
    for (clk = 0; clk < MAX_CLK; clk++) {
      if (ckEnabled[clk] == 0) {
        continue;
      }
      //
      // Avoid DCPMM ERID clock movement: clk1-Dimm0/ERID, clk3-Dimm1/ERID
      //
      if ((clk == 1) || (clk == 3)) {
        if ((*dimmNvList)[clk/2].DcpmmPresent == 1) {
          continue;
        }
      }

      //
      // Avoid BWV-A0 DDRT clock movement clk0-Dimm0/DDRT clock, clk2-Dimm1/DDRT clock
      //
      if ((clk == 0) || (clk == 2)) {
        // clk0 and clk2 correspond to ddrt dimm0 and dimm1
        if (FmcAvoidClockMovementInDimm (Host, socket, ch, clk / 2)) {
          continue;
        }
      }
      if ((cmdLeft[cmdPiGroup][clk] != -UNMARGINED_CMD_EDGE) && (cmdRight[cmdPiGroup][clk] != UNMARGINED_CMD_EDGE)){
        cmdLeft[cmdPiGroup][clk] = cmdLeft[cmdPiGroup][clk] - cmdOffset[cmdPiGroup];
        cmdRight[cmdPiGroup][clk] = cmdRight[cmdPiGroup][clk] - cmdOffset[cmdPiGroup];
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                       "<--CMD Pi Group %2d clk %d: cmdLeft %d - cmdRight %d\n",
                       cmdPiGroup, clk, cmdLeft[cmdPiGroup][clk], cmdRight[cmdPiGroup][clk]);
      }
    } // clk offset
  } // cmdPiGroup loop

  for (clk = 0; clk < MAX_CLK; clk++) {
    if (ckEnabled[clk] == 0) {
      continue;
    }
    //
    // Avoid DCPMM ERID clock movement: clk1-Dimm0/ERID, clk3-Dimm1/ERID
    //
    if ((clk == 1) || (clk == 3)) {
      if ((*dimmNvList)[clk/2].DcpmmPresent == 1) {
        continue;
      }
    }

    //
    // Avoid BWV-A0 DDRT clock movement clk0-Dimm0/DDRT clock, clk2-Dimm1/DDRT clock
    //
    if ((clk == 0) || (clk == 2)) {
      // clk0 and clk2 correspond to ddrt dimm0 and dimm1
      if (FmcAvoidClockMovementInDimm (Host, socket, ch, clk / 2)) {
        continue;
      }
    }

    minRightOffset = 255;
    maxLeftOffset = -255;

    for (cmdPiGroup = 0; cmdPiGroup < maxCmdGroupAll; cmdPiGroup++) {
      if ((cmdLeft[cmdPiGroup][clk] != -UNMARGINED_CMD_EDGE) && (cmdRight[cmdPiGroup][clk] != UNMARGINED_CMD_EDGE)){
        UpdateMinMaxInt(cmdLeft[cmdPiGroup][clk], &minCmdOffset, &maxLeftOffset);
        UpdateMinMaxInt(cmdRight[cmdPiGroup][clk], &minRightOffset, &maxCmdOffset);
      }
    } // cmdPiGroup loop
    Margins.n = maxLeftOffset;
    Margins.p = minRightOffset;
    UpdateStartingOffsetCmd (socket, ch, clk, &Margins);

    ckOffset[clk] = -(minRightOffset + maxLeftOffset) / 2;

    //
    // Write the CTL delay
    //
    ctlMinVal = 255;
    ctlMaxVal = 0;
    GetSetCtlGroupDelay (Host, socket, ch, ctlIndex[clk] + CtlGrp0, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE, &ckOffset[clk], &ctlMinVal, &ctlMaxVal);

    //
    // Write the CLK delay
    //
    GetSetClkDelay (Host, socket, ch, clk, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE, &ckOffset[clk]);
    //
    // Only move IO Delays if in late CMD CLK (early CMD CLK disabled OR early CMD CLK executed already)
    //
    if (earlyCmdClkDisable || (Host->var.mem.earlyCmdClkExecuted == 1)) {
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        rankList = GetRankNvList(Host, socket, ch, dimm);
        for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
          if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          if ( clk == (*rankList)[rank].ckIndex) {
            GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, 0, DdrLevel, RecEnDelay, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE,
                             (INT16 *)&ckOffset[clk]);
            GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, 0, DdrLevel, TxDqDelay, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE,
                             (INT16 *)&ckOffset[clk]);
            GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, 0, DdrLevel, TxDqsDelay, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE,
                             (INT16 *)&ckOffset[clk]);
          }
        } // rank loop
      } // dimm loop
    }

    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "<----clk %d ckOffset %d: -(maxLeftOffset:%d + minRightOffset:%d) / 2\n",
                   clk, ckOffset[clk], maxLeftOffset, minRightOffset);

  } // clk loop

  RcFreePool (cmdOffsetClk);
} // EvaluateCMDMargins

/**
  Multi-use function to either get or set command tx eq

  @param[in]      Host   - Pointer to SYSHOST
  @param[in]      Socket - Socket number
  @param[in]      Ch     - Channel number
  @param[in]      Mode   - GSM_READ_CSR - Read the data from hardware and not cache
                           GSM_READ_ONLY - Do not write
                           GSM_WRITE_OFFSET - Write offset and not value
                           GSM_FORCE_WRITE - Force the write
  @param[in,out]  Value  - Value or offset to program. Actual value returned to caller.

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
GetSetCmdTxEq (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  )
{
  EFI_STATUS                                 Status;
  INT16                                      CurVal;
  INT16                                      CmdTapCoef;
  UINT16                                     MaxLimit = 0;
  UINT16                                     MinLimit = 0;
  INT16                                      Tmp;
  struct channelNvram                        (*ChannelNvList)[MAX_CH];
  DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_STRUCT DdrCrCmdNControls;
  DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_STRUCT DdrCrCmdSControls;
  DDRCRCMDCONTROLS_CKE_MCIO_DDRIOEXT_STRUCT  DdrCrCkeControls;
  DDRCRCMDCONTROLS_CTL_MCIO_DDRIOEXT_STRUCT  DdrCrCtlControls;

  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // CMD Tx Eq limits
  //
  Status = GetCmdGroupLimitsCpu (Socket, Ch, 0, DdrLevel, CmdTxEq, &MinLimit, &MaxLimit);

  //
  // Get register data
  //
  if (Mode & GSM_READ_CSR) {
    DdrCrCmdNControls.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG);
    DdrCrCmdSControls.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_REG);
    DdrCrCkeControls.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDCONTROLS_CKE_MCIO_DDRIOEXT_REG);
    DdrCrCtlControls.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDCONTROLS_CTL_MCIO_DDRIOEXT_REG);
  } else {
    DdrCrCmdNControls.Data = (*ChannelNvList)[Ch].DdrCrCmdNControls;
    DdrCrCmdSControls.Data = (*ChannelNvList)[Ch].DdrCrCmdSControls;
    DdrCrCkeControls.Data = (*ChannelNvList)[Ch].DdrCrCkeControls;
    DdrCrCtlControls.Data = (*ChannelNvList)[Ch].DdrCrCtlControls;
  }

  //
  // Get the current setting
  // There are 4 instances of the register per channel but they will all be set to the same value.
  //
  CurVal = (INT16)(DdrCrCmdNControls.Bits.cmdtapcoef);

  // Read only?
  if (Mode & GSM_READ_ONLY) {
    *Value = (INT16)CurVal;
  } else {
    // Handle writes
    // Adjust the current CMD delay value by value
    if (Mode & GSM_WRITE_OFFSET) {
      Tmp = CurVal + *Value;
    } else {
      //
      // Absolute value
      //
      Tmp = *Value;
    }

    // Check for boundaries
    // value is INT16 and can be a negative value
    // Value and curVal are UINT8
    // Do not allow negative wraparound (curVal = 3 and offset = -4)
    if (Tmp >= MinLimit) {
      CmdTapCoef = Tmp;
    } else {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "CMD TxEq Out of range!! tmp = 0x%x, MinLimit = 0x%x\n", Tmp, MinLimit);
      CmdTapCoef = MinLimit;
    }
    // Ensure we do not exceed maximums
    if (CmdTapCoef > MaxLimit) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "CMD TxEq Out of range!! tmp = 0x%x, MaxLimit = 0x%x\n", CmdTapCoef, MaxLimit);
      CmdTapCoef = MaxLimit;
    }

    //
    // Program the new value
    //
    DdrCrCmdNControls.Bits.cmdtapcoef = CmdTapCoef;
    DdrCrCmdSControls.Bits.cmdtapcoef = CmdTapCoef;
    DdrCrCkeControls.Bits.cmdtapcoef = CmdTapCoef;
    DdrCrCtlControls.Bits.cmdtapcoef = CmdTapCoef;

    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
    if (((*ChannelNvList)[Ch].DdrCrCmdNControls != DdrCrCmdNControls.Data) || (Mode & GSM_FORCE_WRITE)) {
      MemWritePciCfgEp (Socket, Ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG, DdrCrCmdNControls.Data);
      MemWritePciCfgEp (Socket, Ch, DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_REG, DdrCrCmdSControls.Data);
      MemWritePciCfgEp (Socket, Ch, DDRCRCMDCONTROLS_CKE_MCIO_DDRIOEXT_REG, DdrCrCkeControls.Data);
      MemWritePciCfgEp (Socket, Ch, DDRCRCMDCONTROLS_CTL_MCIO_DDRIOEXT_REG, DdrCrCtlControls.Data);
    }
  }

  //
  // Save to cache
  //
  if (Mode & GSM_UPDATE_CACHE) {
    (*ChannelNvList)[Ch].DdrCrCmdNControls = DdrCrCmdNControls.Data;
    (*ChannelNvList)[Ch].DdrCrCmdSControls = DdrCrCmdSControls.Data;
    (*ChannelNvList)[Ch].DdrCrCkeControls = DdrCrCkeControls.Data;
    (*ChannelNvList)[Ch].DdrCrCtlControls = DdrCrCtlControls.Data;
  }

  return MRC_STATUS_SUCCESS;
} // GetSetCmdTxEq

/**
  Indicates if internal RCD VrefCA source is enabled

  @retval TRUE  - Internal RCD VrefCa source used
          FALSE - External RCD VrefCa source used
**/
BOOLEAN
InternalRcdVrefCaEnabled (
  VOID
)
{
  return FALSE;
} // InternalRcdVrefCaEnabled

/**

  Displays the training results

  @param[in] Host   - Pointer to sysHost, the system Host (root) structure struct
  @param[in] socket - Socket Id

  @retval None

**/
VOID
EFIAPI
DisplayCCCResults (
  IN  PSYSHOST Host,
  IN  UINT8 Socket
  )
{
  UINT8               ch;
  UINT8               index;
  UINT8               ioGroup;
  INT16               delay;
  struct channelNvram (*channelNvList)[MAX_CH];
  UINT8               MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, Socket);

  RcDebugPrint (SDBG_MEM_TRAIN,
                "\nSTART_DATA_CMD_CTL\n");

  RcDebugPrint (SDBG_MEM_TRAIN,
                "                  ");
  for (index = 0; index < CC_SOUTH; index++) {
    RcDebugPrint (SDBG_MEM_TRAIN,
                  "    %-2d", index);
  } // index loop

  RcDebugPrint (SDBG_MEM_TRAIN,
                "\n");

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    // ddrcmdctlckech0_0
    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CmdCtlCke0 ");
    for (ioGroup = 0; ioGroup < CC_SOUTH; ioGroup++) {

      GetSetCmdDelay (Host, Socket, ch, ioGroup, SIDE_A, GSM_READ_ONLY, &delay); //side = 0
      RcDebugPrint (SDBG_MEM_TRAIN,
                    "  %3d ", delay);
    } // ioGroup loop
    RcDebugPrint (SDBG_MEM_TRAIN, "\n");

     // ddrcmdctlckech0_1
    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CmdCtlCke1 ");
    for (ioGroup = CC_SOUTH; ioGroup < (CC_SOUTH * 2); ioGroup++) {

      GetSetCmdDelay (Host, Socket, ch, ioGroup, SIDE_A, GSM_READ_ONLY, &delay); //side = 1
      RcDebugPrint (SDBG_MEM_TRAIN,
                    "  %3d ", delay);
    } // ioGroup loop
    RcDebugPrint (SDBG_MEM_TRAIN, "\n");

    // ddrcmdctlckech0_2
    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,"CmdCtlCke2 ");
    for (ioGroup = CC_SOUTH; ioGroup < (CC_SOUTH + CMD_CTL_B); ioGroup++) {

      GetSetCmdDelay (Host, Socket, ch, ioGroup, SIDE_B, GSM_READ_ONLY, &delay); //side = 0
      RcDebugPrint (SDBG_MEM_TRAIN,
                    "  %3d ", delay);
    } // ioGroup loop
    RcDebugPrint (SDBG_MEM_TRAIN, "\n");

    // ddrcmdctlckech0_3
    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,"CmdCtlCke3 ");
    for (ioGroup = 0; ioGroup < CMD_CTL_B; ioGroup++) {

      GetSetCmdDelay (Host, Socket, ch, ioGroup, SIDE_B, GSM_READ_ONLY, &delay); //side = 1
      RcDebugPrint (SDBG_MEM_TRAIN,
                    "  %3d ", delay);
    } // ioGroup loop
    RcDebugPrint (SDBG_MEM_TRAIN, "\n");

  } // ch loop

  RcDebugPrint (SDBG_MEM_TRAIN,
                "\nSTART_DATA_CLK\n");

  RcDebugPrint (SDBG_MEM_TRAIN,
                "       ");
  for (index = 0; index < 4; index++) {
    RcDebugPrint (SDBG_MEM_TRAIN,
                  "  %3d ", index);
  } // index loop

  RcDebugPrint (SDBG_MEM_TRAIN,
                "\n");

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "");
    for (ioGroup = 0; ioGroup < 4; ioGroup++) {

      GetSetClkDelay (Host, Socket, ch, ioGroup, GSM_READ_ONLY, &delay);
      RcDebugPrint (SDBG_MEM_TRAIN,
                    "  %3d ", delay);
    } // ioGroup loop

    RcDebugPrint (SDBG_MEM_TRAIN,
                  "\n");
  } // ch loop

  RcDebugPrint (SDBG_MEM_TRAIN,
                  "\n");
} // DisplayCCCResults

/**
  This function is used to determine if the given margin parameter requires
  incremental stepping.

  @param[in]       MarginGroup        Margin group.

  @retval  TRUE   Incremental stepping is required.
  @retval  FALSE  Incremental stepping is not required.
**/
BOOLEAN
IsIncrementalSteppingRequired (
  IN MRC_GT MarginGroup
  )
{
  switch (MarginGroup) {
    case CmdAll:
    case CmdGrp0:
    case CmdGrp1:
    case CmdGrp2:
    case CmdGrp3:
    case CmdGrp4:
    case CmdGrp5:
    case CtlAll:
    case CtlGrp0:
    case CtlGrp1:
    case CtlGrp2:
    case CtlGrp3:
    case CtlGrp4:
    case CtlGrp5:
    case CmdCtlAll:
    case CkAll:
    case CmdVref:
    case AlertVref:
    case CmdRon:
    case ReqVref:
      return TRUE;
      break;
    default:
      return FALSE;
  }
} // end function IsIncrementalSteppingRequired

/**
  This function is used to determine whether the given margin group is for
  command/address signals (as opposed to data signals).

  @param[in]  MarginGroup  Margin parameter group.

  @retval TRUE/FALSE
**/
BOOLEAN
IsCmdMarginGroup (
  IN MRC_GT MarginGroup
  )
{
  switch (MarginGroup) {
    case CmdAll:
    case CmdGrp0:
    case CmdGrp1:
    case CmdGrp2:
    case CmdGrp3:
    case CmdGrp4:
    case CmdGrp5:
    case CmdVref:
      return TRUE;
    default:
      return FALSE;
  }
} // end function IsCmdMarginGroup

/**
  This function is used to determine whether the given margin group is for
  control signals.

  @param[in]  MarginGroup  Margin parameter group.

  @retval TRUE/FALSE
**/
BOOLEAN
IsCtlMarginGroup (
  IN MRC_GT MarginGroup
  )
{
  switch (MarginGroup) {
    case CtlAll:
    case CtlGrp0:
    case CtlGrp1:
    case CtlGrp2:
    case CtlGrp3:
    case CtlGrp4:
    case CtlGrp5:
      return TRUE;
    default:
      return FALSE;
  }
} // end function IsCtlMarginGroup

/**
  This function is used to enable or disable the output data path for
  command, control and clock signals.

  When disable, it first saves the ranken field setting then overwrites the field to 0x0.
  When enable, it restores the setting.

  @param [in    ] Host   - Pointer to SYSHOST
  @param [in    ] Socket - Socket number
  @param [in    ] Ch     - Channel number
  @param [in    ] Enable - TRUE - restore/enable the output data path
                           FALSE - disable the output data path
  @param [in,out] RankEnCmdS - rankEn value of CmdS
  @param [in,out] RankEnCmdN - rankEn value of CmdN
  @param [in,out] RankEnCtl  - rankEn value of Ctl
  @param [in,out] RankEnCke  - rankEn value of Cke

  @retval None
**/
STATIC VOID
EnableCCCOutputPath (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN BOOLEAN Enable,
  IN OUT UINT32* RankEnCmdS,
  IN OUT UINT32* RankEnCmdN,
  IN OUT UINT32* RankEnCtl,
  IN OUT UINT32* RankEnCke
)
{
  DDRCRCTLRANKSUSED_CMDS_MCIO_DDRIOEXT_STRUCT CmdSCsr;
  DDRCRCTLRANKSUSED_CMDN_MCIO_DDRIOEXT_STRUCT CmdNCsr;
  DDRCRCTLRANKSUSED_CTL_MCIO_DDRIOEXT_STRUCT  CtlCsr;
  DDRCRCTLRANKSUSED_CKE_MCIO_DDRIOEXT_STRUCT  CkeCsr;

  if (!IsSiliconWorkaroundEnabled ("S1707041980")) {
    return;
  }

  //
  // Wait 40 QCLK for stable IO.
  //
  FixedQClkDelay (Host, 40);

  CmdSCsr.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCTLRANKSUSED_CMDS_MCIO_DDRIOEXT_REG);
  CmdNCsr.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCTLRANKSUSED_CMDN_MCIO_DDRIOEXT_REG);
  CtlCsr.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCTLRANKSUSED_CTL_MCIO_DDRIOEXT_REG);
  CkeCsr.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCTLRANKSUSED_CKE_MCIO_DDRIOEXT_REG);

  if (Enable) {
    CmdSCsr.Bits.ranken = *RankEnCmdS;
    CmdNCsr.Bits.ranken = *RankEnCmdN;
    CtlCsr.Bits.ranken = *RankEnCtl;
    CkeCsr.Bits.ranken = *RankEnCke;
  } else {
    *RankEnCmdS = CmdSCsr.Bits.ranken;
    *RankEnCmdN = CmdNCsr.Bits.ranken;
    *RankEnCtl = CtlCsr.Bits.ranken;
    *RankEnCke = CkeCsr.Bits.ranken;

    CmdSCsr.Bits.ranken = DISABLE_OUTPUT_DATAPATH;
    CmdNCsr.Bits.ranken = DISABLE_OUTPUT_DATAPATH;
    CtlCsr.Bits.ranken = DISABLE_OUTPUT_DATAPATH;
    CkeCsr.Bits.ranken = DISABLE_OUTPUT_DATAPATH;
  }

  MemWritePciCfgEp (Socket, Ch, DDRCRCTLRANKSUSED_CMDS_MCIO_DDRIOEXT_REG, CmdSCsr.Data);
  MemWritePciCfgEp (Socket, Ch, DDRCRCTLRANKSUSED_CMDN_MCIO_DDRIOEXT_REG, CmdNCsr.Data);
  MemWritePciCfgEp (Socket, Ch, DDRCRCTLRANKSUSED_CTL_MCIO_DDRIOEXT_REG, CtlCsr.Data);
  MemWritePciCfgEp (Socket, Ch, DDRCRCTLRANKSUSED_CKE_MCIO_DDRIOEXT_REG, CkeCsr.Data);

  //
  // Wait 40 QCLK for stable IO.
  //
  FixedQClkDelay (Host, 40);

}// end function EnableCmdCtlOutputPath

/**
  Get or Set Scomp Slew Rate Up or Down Adjuster.

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
GetSetCcScompUpDownAdj (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     SubCh,
  IN      UINT8     Mode,
  IN      BOOLEAN   UpDown,
  IN OUT  UINT8     *Value
  )
{
  return MRC_STATUS_SIGNAL_NOT_SUPPORTED;
}

/**
  Initialize Chip Specific Scomp.

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket number
  @param ch       - Channel number
  @param SubCh    - Sub channel number

  @retval         - EFI_SUCCESS

**/
MRC_STATUS
EFIAPI
InitializeCcScompChip (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     SubCh
  )
{
  return MRC_STATUS_SIGNAL_NOT_SUPPORTED;
}

/**

  Move the Clocks from it original point by the number of ticks specified as input parameter

  @param[in]  OffsetDelay   Offset to move the CLK

**/
VOID
EFIAPI
InitializeClkDelay (
  INT16   OffsetDelay
  )
{
  //
  // Not supported
  //

  return;

}
