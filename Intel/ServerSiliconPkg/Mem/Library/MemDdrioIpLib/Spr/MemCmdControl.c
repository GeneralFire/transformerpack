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

#include <SysHostChip.h>
#include <Library/MemoryCoreLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemDdrioIpLib.h>

#include <Library/BaseLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/BaseMemoryLib.h>

#include "MemCmdControl.h"
#include <Memory/MemCmdCtlClkCommon.h>
#include "Include/MemDdrioRegs.h"
#include <UncoreCommonIncludes.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/CheckpointLib.h>
#include <Library/MemRcLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/PkgDelayLib.h>
#include <Include/MemIoControl.h>
#include <Chip/Include/MemHostChip.h>
#include <Library/MemRoutingLib.h>
#include <Library/KtiApi.h>

#include <Library/Ddr5CoreLib.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Include/MemDdrioSpecific.h>
#include "Include/MemDdrioPrivate.h"
#include <Library/RcMemAllocLib.h>

STATIC
VOID
EvaluateCaBacksideMargins (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     ckEnabled[MAX_CLK_BS_SUBCH_DDR5],
  UINT8     ctlIndex[MAX_CLK_BS_SUBCH_DDR5],
  INT16     cmdLeft[COPY_NUM][MAX_CLK_BS_SUBCH_DDR5],
  INT16     cmdRight[COPY_NUM][MAX_CLK_BS_SUBCH_DDR5],
  MRC_GT    group[COPY_NUM][MAX_CLK_BS_SUBCH_DDR5],
  GSM_CSN   cmdSignal[COPY_NUM][MAX_CLK_BS_SUBCH_DDR5],
  INT16     cmdOffset[COPY_NUM]
  );

/**
  Multi-use function to either get or set control delays based on the provided group number

  @param[in] Host     Pointer to SYSHOST
  @param[in] socket   Socket number
  @param[in] ch       Channel number
  @param[in] clk      Clock number
  @param[in] mode     GSM_READ_CSR - Read the data from hardware and not cache
                      GSM_READ_ONLY - Do not write
                      GSM_WRITE_OFFSET - Write offset and not value
                      GSM_FORCE_WRITE - Force the write
  @param[in] value    Value to program

  @retval MRC_STATUS

**/
MRC_STATUS
GetSetClkPieOffset (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    clk,
  UINT8    mode,
  INT16    *value
  );

#define CLK_MOVE_DELAY      0
#define CLK_MOVE_PIE_OFFSET 1
#define CLK_MOVE_MAX        2

#define MIN_CMD_VREF              85
#define MAX_CMD_VREF              170

#define Ch0 0
#define Ch1 1
#define Ch2 2
#define Ch3 3

#define MIN_CMD_OFFSET_INIT  255
#define MAX_CMD_OFFSET_INIT  -255

//
// SPR CMD CTL signal number.
//

#ifdef DDR5_SUPPORT
#define MAX_CMDCTL_ARRAY         22*2
#define MAX_CMDCTL_ARRAY_SPR     22*2
#else
#define MAX_CMDCTL_ARRAY         MAX_CMDCTL_ARRAY_SPR_DDR4
#define MAX_CMDCTL_ARRAY_SPR     MAX_CMDCTL_ARRAY_SPR_DDR4
#endif //DDR5_SUPPORT
//
// Define maximums for CMD, CTL and CK for all processors
//

#define MAX_CMD_DELAY_SPR (511)
#define MAX_CTL_DELAY_SPR (511)
#define MAX_CMD_PIE_OFFSET_SPR (127) //SPR_TODO. To confirm the maximum value. Will add the HSD here.

#define MAX_CLK_PIE_OFFSET 127
//
// For display CCC
//
PinRegister PinRegisterSprSbDisplay[] = {
  { NC,  0 },
  { A0B, 1 },
  { A1B, 2 },
  { A2B, 3 },
  { A3B, 4 },
  { A4B, 5 },
  { A5B, 6 },
  { A6B, 7 },
  { A7B, 8 },
  { A8B, 9 },
  { A9B, 10 },
  { A10B, 11 },
  { A11B, 12 },
  { A12B, 13 },
  { A13B, 14 },
  { RSPB_SB, 15 },
  { RSPA_SB, 16 },
  { PARB, 17 },
  { CS1_B_N, 18 },
  { CS0_B_N, 19 },
  { CS3_B_N, 20 },
  { CS2_B_N, 21 }
};

//-----------------------------------------------------------------------------------

//
// Local Prototypes
//
/**
  Evaluates the CMD margins and determines the appropriate offset

  @param [in    ] Host       - Pointer to Host structure
  @param [in    ] socket     - Processor socket within the system (0-based)
  @param [in    ] ch         - Memory channel number
  @param [in    ] ckEnabled  - Array of clocks enabled
  @param [in    ] ctlIndex - Array of control indexes
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
  );

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
  );

/**
  Evaluates the CMD margins and determines the appropriate offset

  @param [in    ] Host       - Pointer to Host structure
  @param [in    ] socket     - Processor socket within the system (0-based)
  @param [in    ] ch         - Memory channel number
  @param [in    ] ckEnabled  - Array of clocks enabled
  @param [in    ] ctlIndex - Array of control indexes
  @param [in out] cmdLeft    - Array of command values for the left side
  @param [in out] cmdRight   - Array of command values for the right side
  @param [in    ] group      - Command group
  @param [in    ] cmdSignal  - Command signal
  @param [   out] cmdOffset  - Command offset

  @retval None
**/
STATIC
VOID
EvaluateCMDMarginsDdr5 (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch,
  IN     UINT8     CkEnabled[MAX_CLK],
  IN     UINT8     CtlIndex[MAX_CLK],
  IN OUT INT16     CmdLeft[MAX_CMDCTL_ARRAY][MAX_CLK],
  IN OUT INT16     CmdRight[MAX_CMDCTL_ARRAY][MAX_CLK],
  IN     MRC_GT    Group[MAX_CMDCTL_ARRAY][MAX_CLK],
  IN     GSM_CSN   CmdSignal[MAX_CMDCTL_ARRAY][MAX_CLK],
  OUT    INT16     CmdOffset[MAX_CMDCTL_ARRAY]
  );

/**
  Evaluates the CMD margins and determines the appropriate offset for DCA DCK Duty Cycle Training.

  @param [in]      Host       - Pointer to Host structure
  @param [in]      Socket     - Processor Socket within the system (0-based)
  @param [in]      Ch         - Memory channel number
  @param [in]      CkEnabled  - Array of clocks enabled
  @param [in]      CtlIndex   - Array of control indexes
  @param [in][out] CmdLeft    - Array of command values for the left side
  @param [in][out] CmdRight   - Array of command values for the right side

  @retval None
**/
STATIC
VOID
EvaluateCMDMarginsDutyCycleDdr5 (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch,
  IN     UINT8     CkEnabled[MAX_CLK],
  IN     UINT8     CtlIndex[MAX_CLK],
  IN OUT INT16     CmdLeft[MAX_CMDCTL_ARRAY][MAX_CLK][MAX_SAMPLING_LOOP],
  IN OUT INT16     CmdRight[MAX_CMDCTL_ARRAY][MAX_CLK][MAX_SAMPLING_LOOP]
  );

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
  UINT8 Ch;
  UINT8 MaxChDdr;
  UINT8 Socket;
  PSYSHOST Host;

  Host = GetSysHostPointer ();

  Socket = Host->var.mem.currentSocket;

  MaxChDdr = GetMaxChDdr ();
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    GetSetClkDelay (Host, Socket, Ch, 0, GSM_READ_CSR | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &OffsetDelay);
    GetSetClkDelay (Host, Socket, Ch, 1, GSM_READ_CSR | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &OffsetDelay);
    GetSetClkDelay (Host, Socket, Ch, 2, GSM_READ_CSR | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &OffsetDelay);
    GetSetClkDelay (Host, Socket, Ch, 3, GSM_READ_CSR | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &OffsetDelay);
  }

}
/**
  Multi-use function to either get or set signal delays based on the provided group number

  @param Host:     Pointer to SYSHOST
  @param socket:   Socket number
  @param ch:       Channel number
  @param level:    IO level to access
  @param sig:      Enumerated signal name
  @param mode:     GSM_READ_CSR - Read the data from hardware and not cache
                   GSM_READ_ONLY - Do not write
                   GSM_WRITE_OFFSET - Write offset and not value
                   GSM_FORCE_WRITE - Force the write
  @param value:    Absolute value or offset selected by GSM_WRITE_OFFSET

  @retval MRC_STATUS

**/
MRC_STATUS
GetSetSignal (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  MRC_LT    level,
  GSM_CSN   sig,
  UINT8     mode,
  INT16     *value
  )
{
  MRC_STATUS            status;
  UINT8                 RegisterIndex;
  UINT8                 SubCh;

  //
  // Get pi register index
  //
  status = SignalToRegIndex (Host, socket, ch, sig, &SubCh, &RegisterIndex);

  if (status == MRC_STATUS_SUCCESS) {
    status = GetSetCmdDelay (Host, socket, ch, SubCh, RegisterIndex, mode, value);
  }
  return status;
} // GetSetSignal

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
  SIGNAL_DESCRIPTOR   *SignalGroup;
  MRC_STATUS          Status;

  *MaxIoGroup = 0;

  Status = GetCmdCtlGroupList (Host, Group, MaxIoGroup, &SignalGroup);

  return Status;
}

/**
  Multi-use function to either get or set control delays based on the provided group number

  @param[in] Host:     Pointer to SYSHOST
  @param[in] Socket:   Socket number
  @param[in] Ch:       Channel number
  @param[in] Group:    Group number
  @param[in] Mode:     GSM_READ_CSR - Read the data from hardware and not cache
                       GSM_READ_ONLY - Do not write
                       GSM_WRITE_OFFSET - Write offset and not value
                       GSM_FORCE_WRITE - Force the write
  @param[in] Value:    Value to program
  @param[in] MinVal:  Current minimum control delay
  @param[in] MaxVal:  Current maximum control delay


  @retval minVal, maxVal and MRC_STATUS

**/
MRC_STATUS
GetSetCtlGroupDelay (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN MRC_GT    Group,
  IN UINT8     Mode,
  IN INT16     *Value,
  IN UINT16    *MinVal,
  IN UINT16    *MaxVal
  )
{
  UINT8                 MaxPlatformGroup;
  UINT8                 MaxIoGroup;
  UINT8                 GroupIndex;
  SIGNAL_DESCRIPTOR     *SignalGroup;
  MRC_STATUS            Status;

  Status = MRC_STATUS_SUCCESS;
  MaxPlatformGroup = (UINT8) Group + 1;

  //
  // Associate CTL groups for RDIMM or LRDIMM
  //
  if (Host->nvram.mem.dimmTypePresent == RDIMM) {
    if ((Group == CtlGrp0) || (Group == CtlGrp2)) {
      MaxPlatformGroup++;
    }
  }

  for ( ; Group < MaxPlatformGroup; Group++) {
    Status = GetCmdCtlGroupList (Host, Group, &MaxIoGroup, &SignalGroup);
    if (Status == MRC_STATUS_SUCCESS) {
      for (GroupIndex = 0; GroupIndex < MaxIoGroup; GroupIndex++) {
        GetSetCmdDelay (Host, Socket, Ch, SignalGroup[GroupIndex].SubCh, SignalGroup[GroupIndex].RegIndex, Mode, Value);
        UpdateMinMaxUint (*Value, MinVal, MaxVal);
      } // for each signal in the group
    }
  } // for each group

  return Status;
} // GetSetCtlGroupDelay


/**
  Multi-use function to either get or set command vref

  @param Host:     Pointer to SYSHOST
  @param socket:   Socket number
  @param ch:       Channel number
  @param dimm:     Dimm slot
  @param group:    Group number
  @param mode:     GSM_READ_CSR - Read the data from hardware and not cache
                   GSM_READ_ONLY - Do not write
                   GSM_WRITE_OFFSET - Write offset and not value
                   GSM_FORCE_WRITE - Force the write
  @param value:    Value to program
  @param *minVal:  Current minimum command delay
  @param *maxVal:  Current maximum command delay


  @retval minVal and maxVal

**/
MRC_STATUS
GetSetCmdVref (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  MRC_LT   level,
  UINT8    mode,
  INT16    *value
  )
{
  MRC_STATUS status;
  struct dimmNvram (*dimmNvList) [MAX_DIMM];

  dimmNvList = GetDimmNvList (Host, socket, ch);

  if ((*dimmNvList) [dimm].DcpmmPresent) {
#ifdef LRDIMM_SUPPORT
    if (level == LrbufLevel) {
      status = GetSetCmdVrefFnvBackside (Host, socket, ch, dimm, mode, value);
    } else
#endif //LRDIMM_SUPPORT
    {
      status = GetSetCmdVrefFnv (Host, socket, ch, dimm, mode, value);
    }
  } else {
    status = GetSetCmdVrefHostSide (Host, socket, ch, mode, value);
  }

  return status;
}

/**
  Multi-use function to either get or set command delays based on the provided group number

  @param[in] Host:     Pointer to SYSHOST
  @param[in] Socket:   Socket number
  @param[in] Ch:       Channel number
  @param[in] Group:    Group number
  @param[in] Mode:     GSM_READ_CSR - Read the data from hardware and not cache
                       GSM_READ_ONLY - Do not write
                       GSM_WRITE_OFFSET - Write offset and not value
                       GSM_FORCE_WRITE - Force the write
  @param[in] Value:    Value to program
  @param[in] MinVal:   Current minimum command delay
  @param[in] MaxVal:   Current maximum command delay


  @retval minVal and maxVal

**/
MRC_STATUS
GetSetCmdGroupDelay (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN MRC_GT    Group,
  IN UINT8     Mode,
  IN INT16     *Value,
  IN UINT16    *MinVal,
  IN UINT16    *MaxVal
  )
{
  UINT8                 MaxIoGroup;
  UINT8                 GroupIndex;
  SIGNAL_DESCRIPTOR     *SignalGroup;
  MRC_STATUS            Status;

  Status = MRC_STATUS_SUCCESS;

  if (Group == CmdVref) {

    Status = GetSetCmdVref (Host, Socket, Ch, 0, DdrLevel, Mode, Value);

  } else {

    Status = GetCmdCtlGroupList (Host, Group, &MaxIoGroup, &SignalGroup);
    if (Status == MRC_STATUS_SUCCESS) {
      for (GroupIndex = 0; GroupIndex < MaxIoGroup; GroupIndex++) {
        GetSetCmdDelay (Host, Socket, Ch, SignalGroup[GroupIndex].SubCh, SignalGroup[GroupIndex].RegIndex, Mode, Value);
        UpdateMinMaxUint (*Value, MinVal, MaxVal);
      } // for each signal in the group
    }

  }

  return Status;
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
  MRC_STATUS        Status;
  UINT8             MaxIoGroup;
  SIGNAL_DESCRIPTOR *SignalGroup;

  Status = GetCmdCtlGroupList (Host, Group, &MaxIoGroup, &SignalGroup);

  if (Status != MRC_STATUS_SUCCESS) {
    return Status;
  }

  if (KnobIndex >= MaxIoGroup) {
    return MRC_STATUS_SIGNAL_NOT_SUPPORTED;
  }

  GetSetCmdDelay (
    Host,
    Socket,
    ChannelIndex,
    SignalGroup[KnobIndex].SubCh,
    SignalGroup[KnobIndex].RegIndex,
    Mode,
    DelayPtr
    );

  return MRC_STATUS_SUCCESS;

}


/**
  Gets the CSR address for a CMD/CTL/CLK CSR index

  @param[in] Host       - Pointer to SYSHOST
  @param[in] RegIndex   - DDRCC_CTRL0_BIT register index

  @retval CSR address for given register index if the index is valid
  @retval 0xFFFFFFFF if register not found

**/
UINT32
GetCmdGroupAddress (
  IN  PSYSHOST Host,
  IN  UINT8    RegIndex
  )
{
  UINT32 Address = 0;

  UINT32                                    CaPinPiRegs[] = { DDRCC_CTL0_BIT_0_MCIO_DDRIOEXT_REG,
                                                              DDRCC_CTL0_BIT_1_MCIO_DDRIOEXT_REG,
                                                              DDRCC_CTL0_BIT_2_MCIO_DDRIOEXT_REG,
                                                              DDRCC_CTL0_BIT_3_MCIO_DDRIOEXT_REG,
                                                              DDRCC_CTL0_BIT_4_MCIO_DDRIOEXT_REG,
                                                              DDRCC_CTL0_BIT_5_MCIO_DDRIOEXT_REG,
                                                              DDRCC_CTL0_BIT_6_MCIO_DDRIOEXT_REG,
                                                              DDRCC_CTL0_BIT_7_MCIO_DDRIOEXT_REG,
                                                              DDRCC_CTL0_BIT_8_MCIO_DDRIOEXT_REG,
                                                              DDRCC_CTL0_BIT_9_MCIO_DDRIOEXT_REG,
                                                              DDRCC_CTL0_BIT_10_MCIO_DDRIOEXT_REG,
                                                              DDRCC_CTL0_BIT_11_MCIO_DDRIOEXT_REG,
                                                              DDRCC_CTL0_BIT_12_MCIO_DDRIOEXT_REG,
                                                              DDRCC_CTL0_BIT_13_MCIO_DDRIOEXT_REG,
                                                              DDRCC_CTL0_BIT_14_MCIO_DDRIOEXT_REG,
                                                              DDRCC_CTL0_BIT_15_MCIO_DDRIOEXT_REG,
                                                              DDRCC_CTL0_BIT_16_MCIO_DDRIOEXT_REG,
                                                              DDRCC_CTL0_BIT_17_MCIO_DDRIOEXT_REG,
                                                              DDRCC_CTL0_BIT_18_MCIO_DDRIOEXT_REG,
                                                              DDRCC_CTL0_BIT_19_MCIO_DDRIOEXT_REG,
                                                              DDRCC_CTL0_BIT_20_MCIO_DDRIOEXT_REG,
                                                              DDRCC_CTL0_BIT_21_MCIO_DDRIOEXT_REG
                                                             };
  if (RegIndex < ARRAY_SIZE (CaPinPiRegs)) {
    Address = CaPinPiRegs[RegIndex];
  } else {
    Address = 0xFFFFFFFF;
  }
  return Address;
} //GetCmdGroupAddress


/**
  Gets the CSR address for a DDRCC_CTL1 index

  @param[in] Host       - Pointer to SYSHOST
  @param[in] DdrccBit   - DDRCC_CTRL0_BIT register index

  @retval CSR address for given register index if the index is valid
  @retval 0xFFFFFFFF if register not found

**/
UINT32
GetDdrCcCtl1RegisterAddress (
  IN  PSYSHOST Host,
  IN  UINT8    DdrccBit
  )
{
  UINT32 Address = 0;

  UINT32 DdrCcCtl1[] = {
           DDRCC_CTL1_BIT_0_MCIO_DDRIOEXT_REG,
           DDRCC_CTL1_BIT_1_MCIO_DDRIOEXT_REG,
           DDRCC_CTL1_BIT_2_MCIO_DDRIOEXT_REG,
           DDRCC_CTL1_BIT_3_MCIO_DDRIOEXT_REG,
           DDRCC_CTL1_BIT_4_MCIO_DDRIOEXT_REG,
           DDRCC_CTL1_BIT_5_MCIO_DDRIOEXT_REG,
           DDRCC_CTL1_BIT_6_MCIO_DDRIOEXT_REG,
           DDRCC_CTL1_BIT_7_MCIO_DDRIOEXT_REG,
           DDRCC_CTL1_BIT_8_MCIO_DDRIOEXT_REG,
           DDRCC_CTL1_BIT_9_MCIO_DDRIOEXT_REG,
           DDRCC_CTL1_BIT_10_MCIO_DDRIOEXT_REG,
           DDRCC_CTL1_BIT_11_MCIO_DDRIOEXT_REG,
           DDRCC_CTL1_BIT_12_MCIO_DDRIOEXT_REG,
           DDRCC_CTL1_BIT_13_MCIO_DDRIOEXT_REG,
           DDRCC_CTL1_BIT_14_MCIO_DDRIOEXT_REG,
           DDRCC_CTL1_BIT_15_MCIO_DDRIOEXT_REG,
           DDRCC_CTL1_BIT_16_MCIO_DDRIOEXT_REG,
           DDRCC_CTL1_BIT_17_MCIO_DDRIOEXT_REG,
           DDRCC_CTL1_BIT_18_MCIO_DDRIOEXT_REG,
           DDRCC_CTL1_BIT_19_MCIO_DDRIOEXT_REG,
           DDRCC_CTL1_BIT_20_MCIO_DDRIOEXT_REG,
           DDRCC_CTL1_BIT_21_MCIO_DDRIOEXT_REG
           };

  if (DdrccBit < ARRAY_SIZE (DdrCcCtl1)) {
    Address = DdrCcCtl1[DdrccBit];
  } else {
    Address = 0x00;
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_0);
  }
  return Address;
}

/**
  Multi-use function to either get or set command delays based on the provided group number

  @param Host:     Pointer to SYSHOST
  @param socket:   Socket number
  @param ch:       Channel number
  @param SubCh:    SubCh number
  @param RegIndex: Register Index
  @param mode:     GSM_READ_CSR - Read the data from hardware and not cache
                   GSM_READ_ONLY - Do not write
                   GSM_WRITE_OFFSET - Write offset and not value
                   GSM_FORCE_WRITE - Force the write
  @param value:    Value to program or offset

  @retval Pi delay value

**/
MRC_STATUS
GetSetCmdDelay (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    SubCh,
  UINT8    RegIndex,
  UINT8    mode,
  INT16    *value
  )
{
  UINT32                                      Address;
  UINT16                                      curVal;
  UINT16                                      piDelay;
  UINT16                                      maxVal = 0;
  INT16                                       tmp;
  DDRIO_CHANNEL_NVDATA                        (*DdrioChannelNvList) [MAX_CH];
  DDRCC_CTL0_BIT_0_MCIO_DDRIOEXT_STRUCT       DdrccCtlBit;

  if (SubCh >= SUB_CH) {
    return MRC_STATUS_FAILURE;
  }

  DdrioChannelNvList = GetDdrioChannelNvList (Host, socket);

  //
  // Maximum CMD delay
  //
  maxVal = MAX_CMD_DELAY_SPR;//SPR_TODO.

  //
  // Ensure RegIndex doesn't overrun the register cache array
  //
  ASSERT (RegIndex < ARRAY_SIZE ((*DdrioChannelNvList)[ch].CmdCsr));

  //
  // Point to the correct group
  //
  Address = GetCmdGroupAddress (Host, RegIndex);
  if (Address == 0xFFFFFFFF) {
    return MRC_STATUS_FAILURE;
  }

  DdrccCtlBit.Data = 0;
  //
  // Get register data
  //
  if (mode & GSM_READ_CSR) {
    DdrccCtlBit.Data = ReadDdrioCcRegister (Host, socket, ch, SubCh, Address);
  } else {
    DdrccCtlBit.Data = (*DdrioChannelNvList)[ch].CmdCsr[RegIndex][SubCh];
  }

  curVal = (DdrccCtlBit.Bits.cmd_dly) & 0x01FF;
  //
  // Read only
  //
  if (mode & GSM_READ_ONLY) {
    *value = (INT16) curVal;
  } else {
    //
    // Handle write
    // Adjust the current CMD delay value by value
    //
    if (mode & GSM_WRITE_OFFSET) {
      tmp = curVal + *value;
    } else {
      //
      // Absolute value
      //
      tmp = *value;
    }
    //
    // Check for boundaries
    // value is INT16 and can be a negative value
    // Value and curVal are UINT8
    // Do not allow negative wraparound (curVal = 3 and offset = -4)
    //
    if (tmp > 0) {
      piDelay = tmp;
    } else {
      piDelay = 0;
    }
    //
    // Ensure we do not exceed maximums (caller should enforce this)
    //
    if (piDelay > maxVal) {
      piDelay = maxVal;
    }
    DdrccCtlBit.Bits.cmd_dly = piDelay;

    RcDebugPrintWithDevice (SDBG_TRACE, socket, ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
                   "GetSetCmdDelay CMD Delay = %x\n", DdrccCtlBit.Bits.cmd_dly);

    //
    // Program the IO delay
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
    if (((*DdrioChannelNvList)[ch].CmdCsr[RegIndex][SubCh] != DdrccCtlBit.Data) || (mode & GSM_FORCE_WRITE)) {
      WriteDdrioCcRegister (Host, socket, ch, SubCh, Address, DdrccCtlBit.Data);
    }
  }
  //
  // Save to cache
  //
  if (mode & GSM_UPDATE_CACHE) {
    (*DdrioChannelNvList)[ch].CmdCsr[RegIndex][SubCh] = DdrccCtlBit.Data;
  }

  return MRC_STATUS_SUCCESS;
} // GetSetCmdDelay

/**
  Multi-use function to either get or set command Pi even offset based on the provided group number

  @param[in]      Host:     Pointer to SYSHOST
  @param[in]      socket:   Socket number
  @param[in]      ch:       Channel number
  @param[in]      SubCh:    SubCh number
  @param[in]      RegIndex: Register Index
  @param[in]      mode:     GSM_READ_CSR - Read the data from hardware and not cache
                            GSM_READ_ONLY - Do not write
                            GSM_WRITE_OFFSET - Write offset and not value
                            GSM_FORCE_WRITE - Force the write
  @param[in][out] value:    Value to program or offset

  @retval MRC_STATUS

**/
MRC_STATUS
GetSetCmdPieOffset (
  IN     PSYSHOST Host,
  IN     UINT8    socket,
  IN     UINT8    ch,
  IN     UINT8    SubCh,
  IN     UINT8    RegIndex,
  IN     UINT8    mode,
  IN OUT INT16    *value
  )
{
  UINT32                                      Address;
  UINT16                                      curVal;
  UINT16                                      piDelay;
  UINT16                                      maxVal = 0;
  INT16                                       tmp;
  DDRIO_CHANNEL_NVDATA                        (*DdrioChannelNvList) [MAX_CH];
  DDRCC_CTL0_BIT_0_MCIO_DDRIOEXT_STRUCT       DdrccCtlBit;

  if (SubCh >= SUB_CH) {
    return MRC_STATUS_FAILURE;
  }

  DdrioChannelNvList = GetDdrioChannelNvList (Host, socket);

  //
  // Maximum CMD delay
  //
  maxVal = MAX_CMD_PIE_OFFSET_SPR;

  //
  // Ensure RegIndex doesn't overrun the register cache array
  //
  ASSERT (RegIndex < ARRAY_SIZE ((*DdrioChannelNvList)[ch].CmdCsr));

  //
  // Point to the correct group
  //
  Address = GetCmdGroupAddress (Host, RegIndex);
  if (Address == 0xFFFFFFFF) {
    return MRC_STATUS_FAILURE;
  }

  DdrccCtlBit.Data = 0;

  //
  // Get register data
  //
  if (mode & GSM_READ_CSR) {
    DdrccCtlBit.Data = ReadDdrioCcRegister (Host, socket, ch, SubCh, Address);
  } else {
    DdrccCtlBit.Data = (*DdrioChannelNvList)[ch].CmdCsr[RegIndex][SubCh];
  }

  curVal = (DdrccCtlBit.Bits.cmd_pie_offset) & 0x01FF;
  //
  // Read only
  //
  if (mode & GSM_READ_ONLY) {
    *value = (INT16) curVal;
  } else {
    //
    // Handle write
    // Adjust the current CMD delay value by value
    //
    if (mode & GSM_WRITE_OFFSET) {
      tmp = curVal + *value;
    } else {
      //
      // Absolute value
      //
      tmp = *value;
    }
    //
    // Check for boundaries
    // value is INT16 and can be a negative value
    // Value and curVal are UINT8
    // Do not allow negative wraparound (curVal = 3 and offset = -4)
    //
    if (tmp > 0) {
      piDelay = tmp;
    } else {
      piDelay = 0;
    }
    //
    // Ensure we do not exceed maximums (caller should enforce this)
    //
    if (piDelay > maxVal) {
      piDelay = maxVal;
    }
    DdrccCtlBit.Bits.cmd_pie_offset = piDelay;

    RcDebugPrintWithDevice (SDBG_TRACE, socket, ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
      "GetSetCmdDelay CMD Delay = %x\n", DdrccCtlBit.Bits.cmd_pie_offset);

    //
    // Program the IO delay
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
    if (((*DdrioChannelNvList) [ch].CmdCsr[RegIndex][SubCh] != DdrccCtlBit.Data) || (mode & GSM_FORCE_WRITE)) {
      WriteDdrioCcRegister (Host, socket, ch, SubCh, Address, DdrccCtlBit.Data);
    }
  }
  //
  // Save to cache
  //
  if (mode & GSM_UPDATE_CACHE) {
    (*DdrioChannelNvList) [ch].CmdCsr[RegIndex][SubCh] = DdrccCtlBit.Data;
  }

  return MRC_STATUS_SUCCESS;
} // GetSetCmdPieOffset

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
  UINT8                                         Imc;
  DDRIO_IMC_NVDATA                              (*DdrioImcNvList)[MAX_IMC];
  DDRCOMP_DIMM_VREF_CTL0_MCIO_DDRIOEXT_STRUCT   DdrDimmVrefControl;

  NumChPerMc = GetNumChannelPerMc ();
  DdrioImcNvList = GetDdrioImcNvList (Host, socket);
  Imc = GetMCID (Host, socket, ch);

  //
  // Maximum and Minimum CMD delay
  //
  maxVal = MAX_CMD_VREF;
  minVal = MIN_CMD_VREF;

  //
  // Get register data
  //
  if (mode & GSM_READ_CSR) {
    DdrDimmVrefControl.Data = MemReadPciCfgEp (socket, (Imc * NumChPerMc), DDRCOMP_DIMM_VREF_CTL0_MCIO_DDRIOEXT_REG);
  } else {
    DdrDimmVrefControl.Data = (*DdrioImcNvList)[Imc].dimmVrefControl1;
  }

  //
  // Get the CA Vref current setting
  //
  switch (ch % NumChPerMc) {
  case 0:
    curVal = (INT16) (DdrDimmVrefControl.Bits.ch0_dimm_vref_ctl >> 1);
    break;

  case 1:
    curVal = (INT16) (DdrDimmVrefControl.Bits.ch1_dimm_vref_ctl >> 1);
    break;

  default:
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "GetSetCmdVrefHostSide called for unknown channel: %d!\n", ch);
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_140);
    break;
  }

  // Read only?
  if (mode & GSM_READ_ONLY) {
    *value = (INT16) curVal;
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
    case 0:
      DdrDimmVrefControl.Bits.ch0_dimm_vref_ctl = vref << 1;
      break;

    case 1:
      DdrDimmVrefControl.Bits.ch1_dimm_vref_ctl = vref << 1;
      break;

    default:
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "GetSetCmdVrefHostSide called for unknown channel: %d!\n", ch);
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_140);
      break;
    }

    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
    if (((*DdrioImcNvList)[Imc].dimmVrefControl1 != DdrDimmVrefControl.Data) || (mode & GSM_FORCE_WRITE)) {
      MemWritePciCfgMC (socket, Imc, DDRCOMP_DIMM_VREF_CTL0_MCIO_DDRIOEXT_REG, DdrDimmVrefControl.Data);
    }
  }

  //
  // Save to cache
  //
  if (mode & GSM_UPDATE_CACHE) {
    (*DdrioImcNvList)[Imc].dimmVrefControl1 = DdrDimmVrefControl.Data;
  }

  return MRC_STATUS_SUCCESS;
} // GetSetCmdVref

/**
  Multi-use function to either get or set ERID vref

  @param Host:     Pointer to SYSHOST
  @param socket:   Socket number
  @param ch:       Channel number
  @param mode:     GSM_READ_CSR - Read the data from hardware and not cache
                   GSM_READ_ONLY - Do not write
                   GSM_WRITE_OFFSET - Write offset and not value
                   GSM_FORCE_WRITE - Force the write
  @param value:    Value to program or offset

  @retval Pi delay value

**/
MRC_STATUS
GetSetEridVref (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     mode,
  INT16     *value
  )
{
  INT16                                   curVal;
  INT16                                   vref;
  INT16                                   tmp;
  UINT16                                  maxLimit;
  UINT16                                  minLimit;
  UINT16                                  usDelay;
  DDRIO_CHANNEL_NVDATA                    (*DdrioChannelNvList) [MAX_CH];
  DDRCLK_RXVREF_CTL_MCIO_DDRIOEXT_STRUCT  DdrClkRxVrefCtl;

  DdrioChannelNvList = GetDdrioChannelNvList (Host, socket);

  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits (Host, DdrLevel, EridVref, &minLimit, &maxLimit, &usDelay);

  //
  // Get register data
  //
  if (mode & GSM_READ_CSR) {
    DdrClkRxVrefCtl.Data = MemReadPciCfgEp (socket, ch, DDRCLK_RXVREF_CTL_MCIO_DDRIOEXT_REG);
  } else {
#if 0 // SPR_TODO BIOS HSD-ES 1504716216
    DdrClkRxVrefCtl.Data = (*DdrioChannelNvList) [ch].DdrClkRxVrefCtl;
#else // SPR_TODO BIOS HSD-ES 1504716216
    //
    // Temporarily use ddrCRClkControls but switch to DdrClkRxVrefCtl
    // when we can do that without growing the size of SysHost.
    //
    DdrClkRxVrefCtl.Data = (*DdrioChannelNvList) [ch].ddrCRClkControls;
#endif // SPR_TODO BIOS HSD-ES 1504716216
  }

  //
  // Get the CA Vref current setting
  //
  curVal = (INT16) DdrClkRxVrefCtl.Bits.rx_vref_ctl;

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
    DdrClkRxVrefCtl.Bits.rx_vref_ctl = vref;

    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
#if 0 // SPR_TODO BIOS HSD-ES 1504716216
    if ((DdrClkRxVrefCtl.Data != (*DdrioChannelNvList) [ch].DdrClkRxVrefCtl) || (mode & GSM_FORCE_WRITE)) {
#else // SPR_TODO BIOS HSD-ES 1504716216
    //
    // Temporarily use ddrCRClkControls but switch to DdrClkRxVrefCtl
    // when we can do that without growing the size of SysHost.
    //
    if ((DdrClkRxVrefCtl.Data != (*DdrioChannelNvList) [ch].ddrCRClkControls) || (mode & GSM_FORCE_WRITE)) {
#endif // SPR_TODO BIOS HSD-ES 1504716216
      MemWritePciCfgEp (socket, ch, DDRCLK_RXVREF_CTL_MCIO_DDRIOEXT_REG, DdrClkRxVrefCtl.Data);
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
#if 0 // SPR_TODO BIOS HSD-ES 1504716216
    (*DdrioChannelNvList) [ch].DdrClkRxVrefCtl = DdrClkRxVrefCtl.Data;
#else // SPR_TODO BIOS HSD-ES 1504716216
    //
    // Temporarily use ddrCRClkControls but switch to DdrClkRxVrefCtl
    // when we can do that without growing the size of SysHost.
    //
    (*DdrioChannelNvList) [ch].ddrCRClkControls = DdrClkRxVrefCtl.Data;
#endif // SPR_TODO BIOS HSD-ES 1504716216
  }

  return MRC_STATUS_SUCCESS;
} // GetSetEridVref

/**
  Returns the desired clk CSR

  @param [IN] Clk: Clock number

  @retval: Clk CSR
**/
UINT32
GetClkRegOffset (
  IN UINT8 Clk
  )
{
  UINT32 ClkCsr[] = { DDRCLK_CTL0_PAIR_0_MCIO_DDRIOEXT_REG, // Dimm0 RDIMM, LRDIMM, DDRT2, UDIMM Rank 0
                      DDRCLK_CTL0_PAIR_1_MCIO_DDRIOEXT_REG, // Dimm1 RDIMM, LRDIMM, DDRT2, UDIMM Rank 0
                      DDRCLK_CTL0_PAIR_2_MCIO_DDRIOEXT_REG, // Dimm0 UDIMM Rank1
                      DDRCLK_CTL0_PAIR_3_MCIO_DDRIOEXT_REG  // Dimm1 UDIMM Rank1
                    };

  if (Clk < (sizeof (ClkCsr) / sizeof (ClkCsr[0]))) {
    return ClkCsr[Clk];
  } else {
    RcDebugPrint (SDBG_ERROR, "\nGetClkRegOffset: Clk(%d) out of bounds\n", Clk);
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_177);
    return ClkCsr[Clk % (sizeof (ClkCsr) / sizeof (ClkCsr[0]))];
  }
}

/**
  Returns the DDRT dimm number per channel for this clk

  @param[in] Clk - Clock number

  @retval: Dimm number
**/
UINT8
DdrtClkToDimmIndex (
  IN UINT8 Clk
  )
{
  // For DDRT
  // Clk 0: Dimm 0
  // Clk 1: Dimm 1
  return Clk % MAX_DIMM;
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
  DDRCC_COMP_OFFSET_MCIO_DDRIOEXT_STRUCT DdrCcCompOffset;
  DDRCC_COMP_LA1_MCIO_DDRIOEXT_STRUCT    DdrCcCompLa1;
  DDRIO_CHANNEL_NVDATA                  (*DdrioChannelNvList) [MAX_CH];

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  DdrCcCompOffset.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_COMP_OFFSET_MCIO_DDRIOEXT_REG);
  DdrCcCompOffset.Bits.scomp_up_adj = 0;
  DdrCcCompOffset.Bits.scomp_dn_adj = 0;
  WriteDdrioCcRegister(Host, Socket, Ch, SubCh, DDRCC_COMP_OFFSET_MCIO_DDRIOEXT_REG, DdrCcCompOffset.Data);

  DdrCcCompLa1.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_COMP_LA1_MCIO_DDRIOEXT_REG);
  DdrCcCompLa1.Bits.scomp = 0;
  WriteDdrioCcRegister(Host, Socket, Ch, SubCh, DDRCC_COMP_LA1_MCIO_DDRIOEXT_REG, DdrCcCompLa1.Data);

  DoCompUpdate (Host, Socket, GetMCID (Host, Socket, Ch));

  //
  // Initialize cache.
  //
  ((*DdrioChannelNvList) [Ch].SubChannel[SubCh]).DdrCcCompOffset = DdrCcCompOffset.Data;
  ((*DdrioChannelNvList) [Ch].SubChannel[SubCh]).DdrCcCompLa1 = DdrCcCompLa1.Data;

  return MRC_STATUS_SUCCESS;
}

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
  DDRCC_COMP_OFFSET_MCIO_DDRIOEXT_STRUCT DdrCcCompOffset;
  DDRIO_CHANNEL_NVDATA                  (*DdrioChannelNvList) [MAX_CH];

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  if ((Mode & GSM_READ_CSR) != 0) {
    DdrCcCompOffset.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_COMP_OFFSET_MCIO_DDRIOEXT_REG);
  } else {
    //
    // Read from cache.
    //
    DdrCcCompOffset.Data = ((*DdrioChannelNvList) [Ch].SubChannel[SubCh]).DdrCcCompOffset;
  }

  if ((Mode & GSM_READ_ONLY) == GSM_READ_ONLY) {
    if (UpDown == TRUE) {
      *Value = (UINT8)DdrCcCompOffset.Bits.scomp_up_adj;
    } else {
      *Value = (UINT8)DdrCcCompOffset.Bits.scomp_dn_adj;
    }

    return MRC_STATUS_SUCCESS;
  }

  if ((Mode & GSM_FORCE_WRITE) == GSM_FORCE_WRITE) {
    if (UpDown == TRUE) {
      DdrCcCompOffset.Bits.scomp_up_adj = *Value;
    } else {
      DdrCcCompOffset.Bits.scomp_dn_adj = *Value;
    }

    WriteDdrioCcRegister(Host, Socket, Ch, SubCh, DDRCC_COMP_OFFSET_MCIO_DDRIOEXT_REG, DdrCcCompOffset.Data);

    DoCompUpdate (Host, Socket, GetMCID (Host, Socket, Ch));

    //
    // Update cache.
    //
    ((*DdrioChannelNvList)[Ch].SubChannel[SubCh]).DdrCcCompOffset = DdrCcCompOffset.Data;
  }

  return MRC_STATUS_SUCCESS;
}

/**
  Restore CA Slew Rate comp of channel.

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket number
  @param Channel  - Channel number

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
EFIAPI
RestoreCaSlewRate (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{
  DDRCC_COMP_OFFSET_MCIO_DDRIOEXT_STRUCT DdrCcCompOffset;
  DDRCC_COMP_LA1_MCIO_DDRIOEXT_STRUCT    DdrCcCompLa1;
  DDRIO_CHANNEL_NVDATA                   (*DdrioChannelNvList) [MAX_CH];
  UINT8                                  SubCh;

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

    DdrCcCompOffset.Data = ((*DdrioChannelNvList) [Channel].SubChannel[SubCh]).DdrCcCompOffset;
    WriteDdrioCcRegister (Host, Socket, Channel, SubCh, DDRCC_COMP_OFFSET_MCIO_DDRIOEXT_REG, DdrCcCompOffset.Data);

    DdrCcCompLa1.Data = ((*DdrioChannelNvList) [Channel].SubChannel[SubCh]).DdrCcCompLa1;
    WriteDdrioCcRegister (Host, Socket, Channel, SubCh, DDRCC_COMP_LA1_MCIO_DDRIOEXT_REG, DdrCcCompLa1.Data);
  }

  DoCompUpdate (Host, Socket, GetMCID (Host, Socket, Channel));

  return MRC_STATUS_SUCCESS;
}

/**
  Function to move the clock to desired offset

  @param[in] Host:         Pointer to SYSHOST
  @param[in] Socket:       Socket number
  @param[in] Ch:           Channel number
  @param[in] ClkCsrOffset: Clock number
  @param[in] Data:         Data for ClkCsrOffset
  @param[in] PiVal:        Original Pi delay
  @param[in] PiDelay:      Target Pi delay
  @param[in] ClkMoveMode:  Clock Movement Mode
                           - CLK_MOVE_DELAY
                           - CLK_MOVE_PIE_OFFSET
                           - CLK_MOVE_MAX

  @retval N/A

**/
VOID
DdrtClockMovementWorker (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT32   ClkCsrOffset,
  IN UINT32   Data,
  IN UINT8    PiVal,
  IN UINT16   PiDelay,
  IN UINT8    ClkMoveMode
  )
{
  DDRCLK_CTL0_PAIR_0_MCIO_DDRIOEXT_STRUCT  ClkCsr;
  UINT8                                    ClkMoveNum;
  UINT8                                    OrgPiDelay;
  UINT8                                    NewPiDelay;
  UINT8                                    TargetPiDelay;

  if (ClkMoveMode >= CLK_MOVE_MAX) {
    ASSERT (FALSE);
  }

  ClkCsr.Data = Data;
  ClkMoveNum = 0;
  OrgPiDelay = (UINT8) PiVal;
  TargetPiDelay = (UINT8) PiDelay;
  NewPiDelay = TargetPiDelay;

  do {
    //
    // DDRT 2.0: CWV clock movement
    //
    if (IsDdrtClockMovementAllowed (Socket, Ch, OrgPiDelay, TargetPiDelay, &NewPiDelay)) {

      if (ClkMoveMode == CLK_MOVE_DELAY) {
        ClkCsr.Bits.clk_dly = NewPiDelay;
      } else {
        ClkCsr.Bits.clk_pie_offset = NewPiDelay;
      }

      MemWritePciCfgEp (Socket, Ch, ClkCsrOffset, ClkCsr.Data);
      OrgPiDelay = NewPiDelay;
      //
      // Wait 6 usec
      //
      FixedDelayMicroSecond (6);
    }

    if (NewPiDelay == TargetPiDelay) {
      break;
    }

    ClkMoveNum++;
  } while (ClkMoveNum < MAX_CLOCK_MOVEMENT_NUM);
}

/**
  Multi-use function to either get or set control delays based on the provided group number

  @param Host:     Pointer to SYSHOST
  @param socket:   Socket number
  @param ch:       Channel number
  @param clk:      Clock number
  @param mode:     GSM_READ_CSR - Read the data from hardware and not cache
                   GSM_READ_ONLY - Do not write
                   GSM_WRITE_OFFSET - Write offset and not value
                   GSM_FORCE_WRITE - Force the write
  @param value:    Value to program

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
  UINT8                                 piVal;
  INT16                                 tmp;
  UINT16                                piDelay;
  UINT32                                ClkCsrOffset;
  DDRIO_CHANNEL_NVDATA  (*DdrioChannelNvList) [MAX_CH];

  DDRCLK_CTL0_PAIR_0_MCIO_DDRIOEXT_STRUCT   clkCsr;

  DdrioChannelNvList = GetDdrioChannelNvList (Host, socket);

  ClkCsrOffset = GetClkRegOffset (clk);
  //
  // Get register data
  //
  if (mode & GSM_READ_CSR) {
    clkCsr.Data = MemReadPciCfgEp (socket, ch, ClkCsrOffset);
  } else {
    clkCsr.Data = (*DdrioChannelNvList) [ch].ClkCsr[clk];
  }

  piVal = (UINT8) clkCsr.Bits.clk_dly;

  // Read only?
  if (mode & GSM_READ_ONLY) {
    *value = (INT16) piVal;
  }
  // Handle writes
  else {

    // Adjust the current CMD delay value by offset
    if (mode & GSM_WRITE_OFFSET) {
      // Get the new offset (note: can be negative)
      tmp = piVal + *value;
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

    clkCsr.Bits.clk_dly = piDelay;

    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
    if (((*DdrioChannelNvList) [ch].ClkCsr[clk] != clkCsr.Data) || (mode & GSM_FORCE_WRITE)) {

      if (IsDcpmmPresentDimm (socket, ch, DdrtClkToDimmIndex (clk))) {

        DdrtClockMovementWorker (Host, socket, ch, ClkCsrOffset, clkCsr.Data, piVal, piDelay, CLK_MOVE_DELAY);

      } else {

        MemWritePciCfgEp (socket, ch, ClkCsrOffset, clkCsr.Data);
      }
    }

  }

  //
  // Save to cache
  //
  if (mode & GSM_UPDATE_CACHE) {
    (*DdrioChannelNvList) [ch].ClkCsr[clk] = clkCsr.Data;
  }

  return MRC_STATUS_SUCCESS;
} // GetSetClkDelay

// Table of # pico seconds per pi ticks for each supported frequency
// Formula is: ((1/(Frq/2))*1.0E6)/64 - ((1/(1066/2))*1000000)/64 = 29.29
//                                        800 1000 1066 1200 1333 1400 1600 1800 1866 2000 2133 2200 2400 2600 2666 2800 2933 3000 3200
//                                       3400 3467 3600 3733 3800 4000 4200 4266 4400 4800 5200 5600 6000 6400 6800 7200 7600 8000 8400
const UINT8 piPsFrqTable[MAX_SUP_FREQ] = { 20,  16,  15,  13,  12,  11,  10,   9,   8,   8,   7,   7,   7,   6,   6,   6,   5,   5,   5,
                                            5,   5,   4,   4,   4,   4,   4,   4,   4,   3,   3,   3,   3,   2,   2,   2,   2,   2,   2};

/** Set starting/initial values for clock and control signals.

  Initial values come from analog design.

  @param[in]  Host    Pointer to sysHost
  @param[in]  Socket  Socket number

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
  UINT8               clk;
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  INT16               aepClk[MAX_CLK];
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
  picoPerPi = piPsFrqTable[Host->nvram.mem.ratioIndex];
  MaxChDdr = GetMaxChDdr ();
  AcquirePrintControl ();

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
    minDelay = MAX_CMD_DELAY_SPR;

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

    baseDelay = (MAX_CMD_DELAY_SPR + 1 - (maxDelay + minDelay)) / 2;

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
    for (clk = 0; clk < MAX_CLK; clk++) {
      // 1:1 association between clock and DIMM assumed, up to a point.
      if (clk < MAX_DIMM && (*dimmNvList)[clk].DcpmmPresent) {
        aepClk[clk] = 64;
      } else {
        aepClk[clk] = 0;
      }
    }

    for (ai = 0; ai < numClkEntries; ai++) {
      // Maximum of piPsFrqTable is 2, clk delay range is 128 (MAX_CK_DELAY). 2*128 - 1 = 255 (0xFF)
      if ((ClkTableStruct[ai].ch != ch) || (ClkDelay[ai] >= (2*MAX_CK_DELAY - 1))) {
        continue;
      }
      delay = ClkDelay[ai] / picoPerPi + baseDelay + aepClk[ClkTableStruct[ai].clk];
      GetSetClkDelay (Host, Socket, ClkTableStruct[ai].ch, ClkTableStruct[ai].clk, mode, &delay);
    }

  }  // ch loop

  DisplayCCCResults (Host, Socket);
  ReleasePrintControl ();

  OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_POST_PACKAGE_DELAY_CCC, Socket);

} // SetStartingCCC

/**

  Get min/max limits for Command, Control and Clock signals

  @param[in]  Socket      - Processor socket within the system (0-based)
  @param[in]  Ch          - Channel number
  @param[in]  Dimm        - DIMM number
  @param[in]  Level       - IO level to access
  @param[in]  Group       - Command, clock or control group to access
  @param[out] *MinLimit   - Minimum delay value allowed
  @param[out] *MaxLimit   - Maximum delay value allowed

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
      *MaxLimit = MAX_CTL_DELAY_SPR;
      *MinLimit = 0;
      break;
    case CmdGrp0:
    case CmdGrp1:
    case CmdGrp2:
    case CmdGrp3:
    case CmdGrp4:
    case CmdAll:
      *MaxLimit = MAX_CMD_DELAY_SPR;
      *MinLimit = 0;
      break;
    case CmdTxEq:
      *MaxLimit = MAX_CMD_TX_EQ_VAL;
      *MinLimit = 0;
      break;
    case CmdVref:
      *MinLimit = MIN_CMD_VREF;
      *MaxLimit = MAX_CMD_VREF;
      break;
    default:
      Status = EFI_NOT_FOUND;
      break;
  }

  return Status;
} // GetCmdGroupLimitsCpu

/**

  Get min/max limits for Command, Control and Clock signals

  @param[in]  Socket      - Processor socket within the system (0-based)
  @param[in]  Ch          - Channel number, use ALL_CH to ignore Ch/Dimm parameters
  @param[in]  Dimm        - DIMM number
  @param[in]  Level       - IO level to access
  @param[in]  Group       - Command, clock or control group to access
  @param[out] *MinLimit   - Minimum delay value allowed
  @param[out] *MaxLimit   - Maximum delay value allowed

  @retval EFI_SUCCESS if MinLimit and MaxLimit are updated
          EFI_INVALID_PARAMTER if bad parameter passed in
          EFI_NOT_FOUND otherwise

**/
EFI_STATUS
GetCmdGroupLimits (
  IN    UINT8     Socket,
  IN    UINT8     Ch,
  IN    UINT8     Dimm,
  IN    MRC_LT    Level,
  IN    MRC_GT    Group,
  OUT   UINT16    *MinLimit,
  OUT   UINT16    *MaxLimit
  )
{
  EFI_STATUS              Status;
  PSYSHOST                Host;
  struct dimmNvram        (*DimmNvList) [MAX_DIMM];

  Host = GetSysHostPointer ();
  DimmNvList = NULL;
  if (Ch != ALL_CH) {
    if (Dimm >= MAX_DIMM) {
      return EFI_INVALID_PARAMETER;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
  }

  Status = EFI_NOT_FOUND;
  //
  // If channel and DIMM are specified, use DDRT limits if DDRT DIMM is installed in the slot
  //
  if ((DimmNvList != NULL) && ((*DimmNvList) [Dimm].DcpmmPresent == 1)  && (Level != LrbufLevel)) {
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

  @param Host                 - Pointer to Host structure
  @param socket               - Processor socket within the system (0-based)
  @param ch                   - DDR channel number within the processor socket (0-based)
  @param listSize             - Number of entries in each list
  @param *value[MAX_RANK_CH]  - Array of pointers to listType

  @retval MRC_STATUS

**/
MRC_STATUS
SetCombinedCtlGroup (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT16    listSize,
  VOID      *value
  )
{
  UINT8                 dimm;
  UINT8                 rank;
  UINT8                 RankIndex;
  UINT8                 SubCh;
  UINT8                 RegIndex;
  UINT8                 listIndex;
  GSM_CSN               Signal;
  UINT8                 ctlPiGroup;
  INT16                 ctlRight[MAX_CMDCTL_ARRAY];
  INT16                 ctlLeft[MAX_CMDCTL_ARRAY];
  GSM_CSN               ctlSignal[MAX_CMDCTL_ARRAY];
  INT16                 ctlValue[MAX_CMDCTL_ARRAY];
  MRC_STATUS            status;
  GSM_CSEDGE_CTL        (*signalListEdge) [MAX_RANK_CH][NUM_SIGNALS_TO_SWEEP_LB];
  struct ddrRank        (*rankList) [MAX_RANK_DIMM];
  struct dimmNvram      (*dimmNvList) [MAX_DIMM];
  UINT8                 MaxIoGroup;
  SIGNAL_DESCRIPTOR     *SignalGroup;

  status = GetCmdCtlGroupList (Host, CmdCtlAll, &MaxIoGroup, &SignalGroup);

  ZeroMem ((UINT8 *) ctlSignal, sizeof (ctlSignal));

  //
  // Initialize the edges for each CTL pi group
  //
  for (ctlPiGroup = 0; ctlPiGroup < MaxIoGroup; ctlPiGroup++) {
    ctlLeft[ctlPiGroup] = 0;
    ctlRight[ctlPiGroup] = MEM_CHIP_POLICY_VALUE (Host, ctlPiGrp);
    ctlValue[ctlPiGroup] = 0;
  } // ctlPiGroup loop

  signalListEdge = value;

  dimmNvList = GetDimmNvList (Host, socket, ch);
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {

    rankList = GetRankNvList (Host, socket, ch, dimm);
    for (rank = 0; rank < (*dimmNvList) [dimm].numRanks; rank++) {
      if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
        continue;
      }

      RankIndex = (*rankList)[rank].rankIndex;

      //
      // Loop through the list to create the composite left and right edges for each CTL group
      //
      for (listIndex = 0; listIndex < listSize; listIndex++) {

        //
        // Get the silicon pi group this signal belongs to
        //
        if ((*signalListEdge)[RankIndex][listIndex].toSample == 0) {
          continue;
        }

        Signal = (*signalListEdge)[RankIndex][listIndex].signal;
        status = SignalToRegIndex (Host, socket, ch, Signal, &SubCh, &RegIndex);

        //
        // Get the index into the CTL silicon pi group
        //
        for (ctlPiGroup = 0; ctlPiGroup < MaxIoGroup; ctlPiGroup++) {
          if ((SubCh == SignalGroup[ctlPiGroup].SubCh) &&
              (RegIndex == SignalGroup[ctlPiGroup].RegIndex)) {

            //
            // We found the CTL pi group so break out of the loop
            //
            break;
          }
        } // ctlPiGroup loop

        if (ctlPiGroup >= MaxIoGroup) {
          RC_FATAL_ERROR (ctlPiGroup < MaxIoGroup, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_42);
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "The CTL Pi Group was never found");
          return MRC_STATUS_GROUP_NOT_SUPPORTED;
        }

        //
        // Update the composite left and right edges for the current CTL pi group relative to the clock
        //
        UpdateEdges ((*signalListEdge)[RankIndex][listIndex].le, (*signalListEdge)[RankIndex][listIndex].re,
          &ctlLeft[ctlPiGroup], &ctlRight[ctlPiGroup]);
        ctlValue[ctlPiGroup] = ((ctlLeft[ctlPiGroup] + ctlRight[ctlPiGroup]) / 2) % 256;
        ctlSignal[ctlPiGroup] = Signal;
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                        "%a:\t CTL Pi Group %d: le = %d re = %d, ctlLeft = %d ctlRight = %d ctlValue = %d\n",
                        GetSignalStr (Signal), ctlPiGroup,
                        (*signalListEdge)[RankIndex][listIndex].le, (*signalListEdge)[RankIndex][listIndex].re,
                        ctlLeft[ctlPiGroup], ctlRight[ctlPiGroup], ctlValue[ctlPiGroup]);
      } // listIndex loop
    } // rank loop
  } // dimm loop

  for (ctlPiGroup = 0; ctlPiGroup < MaxIoGroup; ctlPiGroup++) {
    if (ctlValue[ctlPiGroup] != 0) {
      status = GetSetSignal (Host, socket, ch, DdrLevel, ctlSignal[ctlPiGroup], GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &ctlValue[ctlPiGroup]);
    }
  }
  return status;
} // SetCombinedCtlGroup


/**

  Given an array of command delays relative to current clock and control delays,
  this function will combine shared settings in the DDRIO design and normalize the
  lowest command, clock or control value to their minimum limits.

  @param Host                 - Pointer to Host structure
  @param socket               - Processor socket within the system (0-based)
  @param ch                   - DDR channel number within the processor socket (0-based)
  @param level                - IO level to access
  @param mode                 - Bit-field of different modes
  @param listType             - Selects type of each list
  @param listSize             - Number of entries in each list
  @param *value[MAX_RANK_CH]  - Array of pointers to listType

  @retval MRC_STATUS

**/
MRC_STATUS
SetCombinedCmdGroup (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  MRC_LT    level,
  UINT8     mode,
  UINT8     listType,
  UINT16    listSize,
  VOID      *value
  )
{
  UINT8                 dimm;
  UINT8                 rank;
  UINT8                 RankIndex;
  UINT8                 clk;
  UINT8                 listIndex;
  GSM_CSN               Signal;
  UINT8                 SubCh;
  UINT8                 RegIndex;
  UINT8                 cmdPiGroup;
  UINT8                 ckEnabled[MAX_CLK];
  UINT8                 ctlIndex[MAX_CLK];
  INT16                 cmdLeft[MAX_CMDCTL_ARRAY][MAX_CLK];
  INT16                 cmdRight[MAX_CMDCTL_ARRAY][MAX_CLK];
  MRC_GT                cmdGroup[MAX_CMDCTL_ARRAY][MAX_CLK];
  GSM_CSN               cmdSignal[MAX_CMDCTL_ARRAY][MAX_CLK];
  INT16                 cmdOffset[MAX_CMDCTL_ARRAY];
  UINT16                cmdPiDelaySum[MAX_CMDCTL_ARRAY];
  UINT8                 cmdPiDelayCount[MAX_CMDCTL_ARRAY];
  INT16                 cmdPiDelay[MAX_CMDCTL_ARRAY];
  UINT8                 MaxIoGroup;
  UINT8                 GroupIndex;
  SIGNAL_DESCRIPTOR     *SignalGroup;
  UINT8                 MaxCmdGroupAll;
  SIGNAL_DESCRIPTOR     *CmdGroupAll;
  UINT8                 MaxCmdCtlGroupAll;
  SIGNAL_DESCRIPTOR     *CmdCtlGroupAll;
  MRC_STATUS            status;
  GSM_CSVAL             (*signalList) [MAX_RANK_CH][NUM_SIGNALS_TO_SWEEP];
  GSM_CSEDGE            (*signalListEdge) [MAX_RANK_CH][NUM_SIGNALS_TO_SWEEP];
  GSM_CGVAL             (*groupList) [MAX_RANK_CH][8];
  GSM_CGEDGE            (*groupListEdge) [MAX_RANK_CH][MAX_CMD];
  struct ddrRank        (*rankList) [MAX_RANK_DIMM];
  struct dimmNvram      (*dimmNvList) [MAX_DIMM];

  status = MRC_STATUS_SUCCESS;

  status = GetCmdCtlGroupList (Host, CmdAll, &MaxCmdGroupAll, &CmdGroupAll);
  status = GetCmdCtlGroupList (Host, CmdCtlAll, &MaxCmdCtlGroupAll, &CmdCtlGroupAll);

  dimmNvList = GetDimmNvList (Host, socket, ch);

  for (clk = 0; clk < MAX_CLK; clk++) {
    ckEnabled[clk] = 0;
    ctlIndex[clk] = 0;
  } // clk loop

  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    rankList = GetRankNvList (Host, socket, ch, dimm);
    for (rank = 0; rank < (*dimmNvList) [dimm].numRanks; rank++) {

      // If rank enabled
      if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
        continue;
      }

      clk = (*rankList) [rank].ckIndex;
      ctlIndex[clk] = (*rankList) [rank].ctlIndex;
      ckEnabled[clk] = 1;
    } // rank loop
  } // dimm loop

  //
  // Initialize the edges for each CMD pi group
  //
  for (cmdPiGroup = 0; cmdPiGroup < MaxCmdCtlGroupAll; cmdPiGroup++) {
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

        rankList = GetRankNvList (Host, socket, ch, dimm);

        for (rank = 0; rank < (*dimmNvList) [dimm].numRanks; rank++) {
          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          RankIndex = (*rankList)[rank].rankIndex;
          clk = (*rankList) [rank].ckIndex;

          //
          // Loop through the list to create the composite left and right edges for each CMD group
          //
          for (listIndex = 0; listIndex < listSize; listIndex++) {
            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                            "Platform Group = %a\n", GetPlatformGroupStr ((*groupListEdge)[RankIndex][listIndex].group));
            status = GetCmdCtlGroupList (Host, (*groupListEdge)[RankIndex][listIndex].group, &MaxIoGroup, &SignalGroup);
            for (GroupIndex = 0; GroupIndex < MaxIoGroup; GroupIndex++) {

              //
              // Get the index into the CMD silicon pi group
              //
              for (cmdPiGroup = 0; cmdPiGroup < MaxCmdGroupAll; cmdPiGroup++) {
                if ((SignalGroup[GroupIndex].RegIndex == CmdGroupAll[cmdPiGroup].RegIndex) &&
                    (SignalGroup[GroupIndex].SubCh == CmdGroupAll[cmdPiGroup].SubCh)) {
                  //
                  // We found the CMD pi group so break out of the loop
                  //
                  RcDebugPrintWithDevice (
                    SDBG_MAX, NO_SOCKET, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                    "Found CMD Pi group: register index %d, subchannel %d\n",
                    CmdGroupAll[cmdPiGroup].RegIndex,
                    CmdGroupAll[cmdPiGroup].SubCh
                    );
                  break;
                }
              } // cmdPiGroup loop

              cmdLeft[cmdPiGroup][clk] = (*groupListEdge)[RankIndex][listIndex].le;
              cmdRight[cmdPiGroup][clk] = (*groupListEdge)[RankIndex][listIndex].re;
              cmdGroup[cmdPiGroup][clk] = (*groupListEdge)[RankIndex][listIndex].group;
              cmdSignal[cmdPiGroup][clk] = gsmCsnDelim;
              cmdOffset[cmdPiGroup] = 0;
              RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                              "cmdLeft[%d][%d] = %d : cmdRight[%d][%d] = %d\n", cmdPiGroup, clk,
                              (*groupListEdge)[RankIndex][listIndex].le,cmdPiGroup, clk,
                              (*groupListEdge)[RankIndex][listIndex].re);
            } // Signal loop
          } // listIndex loop
        } // rank loop
      } // dimm loop

      //
      // Evaluate CMD margins and offset CMD, CTL, and CLK appropriately
      //
      EvaluateCMDMargins (Host, socket, ch, ckEnabled, ctlIndex, cmdLeft, cmdRight, cmdGroup, cmdSignal, cmdOffset);


    } else {
      //
      // Average center points
      //
      groupList = value;

      //
      // Initialize for each CMD pi group
      //
      ZeroMem (cmdPiDelaySum, MaxCmdGroupAll * sizeof (UINT16));
      ZeroMem (cmdPiDelayCount, MaxCmdGroupAll);

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        rankList = GetRankNvList (Host, socket, ch, dimm);

        for (rank = 0; rank < (*dimmNvList) [dimm].numRanks; rank++) {
          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          RankIndex = (*rankList)[rank].rankIndex;

          //
          // Loop through the list to create the composite left and right edges for each CMD group
          //
          for (listIndex = 0; listIndex < listSize; listIndex++) {

            status = GetCmdCtlGroupList (Host, (*groupList)[RankIndex][listIndex].group, &MaxIoGroup, &SignalGroup);
            for (GroupIndex = 0; GroupIndex < MaxIoGroup; GroupIndex++) {
              //
              // Get the index into the CMD silicon pi group
              //
              for (cmdPiGroup = 0; cmdPiGroup < MaxCmdGroupAll; cmdPiGroup++) {
                if ((SignalGroup[GroupIndex].RegIndex == CmdGroupAll[cmdPiGroup].RegIndex) &&
                    (SignalGroup[GroupIndex].SubCh == CmdGroupAll[cmdPiGroup].SubCh)) {

                  //
                  // Add this pi delay to the total and increment the count
                  //
                  cmdPiDelaySum[cmdPiGroup] += (*groupList)[RankIndex][listIndex].value;
                  cmdPiDelayCount[cmdPiGroup]++;

                  //
                  // We found the CMD pi group so break out of the loop
                  //
                  break;
                }
              } // cmdPiGroup loop
            } // GroupIndex loop
          } // listIndex loop
        } // rank loop
      } // dimm loop

      //
      // Calculate the pi delay for each CMD si group
      //
      for (cmdPiGroup = 0; cmdPiGroup < MaxCmdGroupAll; cmdPiGroup++) {
        if (cmdPiDelayCount[cmdPiGroup] == 0) {
          continue;
        }

        cmdPiDelay[cmdPiGroup] = (INT16) (cmdPiDelaySum[cmdPiGroup] / cmdPiDelayCount[cmdPiGroup]);
      } // cmdPiGroup loop

      //
      // Program the delay
      //
      for (cmdPiGroup = 0; cmdPiGroup < MaxCmdGroupAll; cmdPiGroup++) {
        GetSetCmdDelay (Host, socket, ch, CmdGroupAll[cmdPiGroup].SubCh, CmdGroupAll[cmdPiGroup].RegIndex, (GSM_WRITE_OFFSET | GSM_UPDATE_CACHE), &cmdPiDelay[cmdPiGroup]);
      } // cmdPiGroup loop
    }

  } else if (listType == CMD_SIGNAL_DELAY) {

    //
    // Combine edges if necessary
    //
    if (mode == GSM_COMBINE_EDGES) {

      signalListEdge = value;

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        rankList = GetRankNvList (Host, socket, ch, dimm);

        for (rank = 0; rank < (*dimmNvList) [dimm].numRanks; rank++) {
          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          RankIndex = (*rankList)[rank].rankIndex;

          //
          // Loop through the list to create the composite left and right edges for each CMD group
          //
          for (listIndex = 0; listIndex < listSize; listIndex++) {

            clk = (*rankList) [rank].ckIndex;

            //
            // Get the silicon pi group this signal belongs to
            //
            Signal = (*signalListEdge)[RankIndex][listIndex].signal;
            status = SignalToRegIndex (Host, socket, ch, Signal, &SubCh, &RegIndex);

            //
            // Get the index into the CMD silicon pi group
            //
            for (cmdPiGroup = 0; cmdPiGroup < MaxCmdGroupAll; cmdPiGroup++) {
              if ((SubCh == CmdGroupAll[cmdPiGroup].SubCh) &&
                  (RegIndex == CmdGroupAll[cmdPiGroup].RegIndex)) {
                //
                // We found the CMD pi group so break out of the loop
                //
                break;
              }
            } // cmdPiGroup loop

            RC_FATAL_ERROR (cmdPiGroup != MaxCmdGroupAll, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_43);
            //
            // Update the composite left and right edges for the current CMD pi group relative to the clock
            //
            UpdateEdges ((*signalListEdge)[RankIndex][listIndex].le, (*signalListEdge)[RankIndex][listIndex].re,
              &cmdLeft[cmdPiGroup][clk], &cmdRight[cmdPiGroup][clk]);
            cmdGroup[cmdPiGroup][clk] = MrcGtDelim;
            cmdSignal[cmdPiGroup][clk] = Signal;
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              "%a:\t CMD Pi Group %d clk %d: le %d re = %d, cmdLeft = %d cmdRight = %d\n",
              GetSignalStr (Signal),
              cmdPiGroup,
              clk,
              (*signalListEdge)[RankIndex][listIndex].le,
              (*signalListEdge)[RankIndex][listIndex].re,
              cmdLeft[cmdPiGroup][clk],
              cmdRight[cmdPiGroup][clk]);
          } // listIndex loop
        } // rank loop
      } // dimm loop

      //
      // Evaluate CMD margins and offset CMD, CTL, and CLK appropriately
      //
      EvaluateCMDMargins (Host, socket, ch, ckEnabled, ctlIndex, cmdLeft, cmdRight, cmdGroup, cmdSignal, cmdOffset);

    } else {
      //
      // Average center points
      //
      signalList = value;

      //
      // Initialize for each CMD pi group
      //
      ZeroMem (cmdPiDelaySum, MaxCmdGroupAll * sizeof (UINT16));
      ZeroMem (cmdPiDelayCount, MaxCmdGroupAll);

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        rankList = GetRankNvList (Host, socket, ch, dimm);

        for (rank = 0; rank < (*dimmNvList) [dimm].numRanks; rank++) {
          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          RankIndex = (*rankList)[rank].rankIndex;

          //
          // Loop through the list to create the composite left and right edges for each CMD group
          //
          for (listIndex = 0; listIndex < listSize; listIndex++) {

            //
            // Get the silicon pi group this signal belongs to
            //
            Signal = (*signalList)[RankIndex][listIndex].signal;
            status = SignalToRegIndex (Host, socket, ch, Signal, &SubCh, &RegIndex);

            //
            // Get the index into the CMD silicon pi group
            //
            for (cmdPiGroup = 0; cmdPiGroup < MaxCmdGroupAll; cmdPiGroup++) {
              if ((SubCh == CmdGroupAll[cmdPiGroup].SubCh) &&
                  (RegIndex == CmdGroupAll[cmdPiGroup].RegIndex)) {

                //
                // Add this pi delay to the total and increment the count
                //
                cmdPiDelaySum[cmdPiGroup] += (*signalList)[RankIndex][listIndex].value;
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
      for (cmdPiGroup = 0; cmdPiGroup < MaxCmdGroupAll; cmdPiGroup++) {
        if (cmdPiDelayCount[cmdPiGroup] == 0) {
          continue;
        }

        cmdPiDelay[cmdPiGroup] = (INT16) (cmdPiDelaySum[cmdPiGroup] / cmdPiDelayCount[cmdPiGroup]);
      } // cmdPiGroup loop

      //
      // Program the delay
      //
      for (cmdPiGroup = 0; cmdPiGroup < MaxCmdGroupAll; cmdPiGroup++) {
        GetSetCmdDelay (Host, socket, ch, CmdGroupAll[cmdPiGroup].SubCh, CmdGroupAll[cmdPiGroup].RegIndex, (GSM_WRITE_OFFSET | GSM_UPDATE_CACHE), &cmdPiDelay[cmdPiGroup]);
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

        rankList = GetRankNvList (Host, socket, ch, dimm);

        for (rank = 0; rank < (*dimmNvList) [dimm].numRanks; rank++) {
          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          //skip ranks >0 if rdimm
          if ((Host->nvram.mem.dimmTypePresent != UDIMM) && (rank > 0)) {
            continue;
          }

          RankIndex = (*rankList)[rank].rankIndex;

          //
          // Loop through the list to create the composite left and right edges for each CMD group
          //
          for (listIndex = 0; listIndex < listSize; listIndex++) {

            clk = (*rankList) [rank].ckIndex;

            //
            // Get the silicon pi group this signal belongs to
            //
            Signal = (*signalListEdge)[RankIndex][listIndex].signal;
            status = SignalToRegIndex (Host, socket, ch, Signal, &SubCh, &RegIndex);

            //
            // Get the index into the CMD silicon pi group
            //

            for (cmdPiGroup = 0; cmdPiGroup < MaxCmdCtlGroupAll; cmdPiGroup++) {
              if ((SubCh == CmdCtlGroupAll[cmdPiGroup].SubCh) &&
                  (RegIndex == CmdCtlGroupAll[cmdPiGroup].RegIndex)) {
                //
                // We found the CMD pi group so break out of the loop
                //
                break;
              }
            } // cmdPiGroup loop

            RC_FATAL_ERROR (cmdPiGroup != MaxCmdCtlGroupAll, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_44);

            //
            // Update the composite left and right edges for the current CMD pi group relative to the clock
            //
            UpdateEdges ((*signalListEdge)[RankIndex][listIndex].le, (*signalListEdge)[RankIndex][listIndex].re,
              &cmdLeft[cmdPiGroup][clk], &cmdRight[cmdPiGroup][clk]);
            cmdGroup[cmdPiGroup][clk] = MrcGtDelim;
            cmdSignal[cmdPiGroup][clk] = Signal;
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                            "%8a: CMD/CTL PI Group %2d, SubCh %d, RegIndex %2d,  clk %d: le %3d re = %3d, Left = %3d Right = %3d\n",
                            GetSignalStr (Signal), cmdPiGroup, SubCh, RegIndex, clk,
                            (*signalListEdge)[RankIndex][listIndex].le, (*signalListEdge)[RankIndex][listIndex].re,
                            cmdLeft[cmdPiGroup][clk], cmdRight[cmdPiGroup][clk]);
          } // listIndex loop
        } // rank loop
      } // dimm loop

      //
      // Evaluate CMD margins and offset CMD, CTL, and CLK appropriately
      //
      EvaluateCMDMargins (Host, socket, ch, ckEnabled, ctlIndex, cmdLeft, cmdRight, cmdGroup, cmdSignal, cmdOffset);

    } else {
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "ERROR: This SetCombinedCmd mode not supported!\n");

    }
  } else {
    status = MRC_STATUS_GROUP_NOT_SUPPORTED;
  }

  return status;
} // SetCombinedCmdGroup

/**

  Given an array of RCD QxCA Training delays relative to clock and control delays,
  this function will combine shared settings in the DDRIO design and normalize the
  lowest command, clock or control value to their minimum limits.

  @param[in] Host             Pointer to host structure
  @param[in] Socket           Processor socket within the system (0-based)
  @param[in] Ch               DDR channel number within the processor socket (0-based)
  @param[in] SignalRange      Total number of signals swept
  @param[in] *SignalListEdge  Array of RCD QxCA Training delays

  @retval None

**/
VOID
SetCombinedBacksideCmdGroup (
  IN  PSYSHOST    Host,
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  UINT16      SignalRange,
  IN  GSM_CSEDGE  (*SignalListEdge)[MAX_CH][MAX_RANK_CH][BACKSIDE_SIGNAL_RANGE]
  )
{
  UINT8             Dimm;
  UINT8             Rank;
  UINT8             RankIndex;
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

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

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

      RankIndex = (*RankList)[Rank].rankIndex;

      //
      // Loop through the signals to create a composite left and right edges for all signals per dimm
      //
      for (Signal = 0; Signal < SignalRange; Signal++) {
        //
        // Find the maximum left edge
        //
        if (CompositeLeftEdge < ((*SignalListEdge)[Ch][RankIndex][Signal].le)) {
          CompositeLeftEdge = (*SignalListEdge)[Ch][RankIndex][Signal].le;
        }

        //
        // Find the minimum right edge
        //
        if (CompositeRightEdge > ((*SignalListEdge)[Ch][RankIndex][Signal].re)) {
          CompositeRightEdge = (*SignalListEdge)[Ch][RankIndex][Signal].re;
        }
      } // Signal loop
    } // Rank loop

    //
    // Calculate RCD QxCA trained offset
    //
    (*DimmNvList)[Dimm].QxCAClkOffset = ((CompositeLeftEdge - BACKSIDE_SWEEP_END) + (CompositeRightEdge - BACKSIDE_SWEEP_END)) / 2;
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "RCD QxCA Final Trained Value = %d, CompositeLeftEdge = %d, CompositeRightEdge = %d\n", (*DimmNvList)[Dimm].QxCAClkOffset, CompositeLeftEdge, CompositeRightEdge);

    //
    // Maintain the CLK/CTL and CMD relationship (CMD-CLK=trained offset) by programing the normalized settings
    //
    if ((*DimmNvList)[Dimm].QxCAClkOffset < 0){
      // Negative Offset
      ClkCtlpiDelay = -(*DimmNvList)[Dimm].QxCAClkOffset;
      CmdpiDelay = 0;
    } else {
      // Positive Offset
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

Given an array of command delays relative to current clock and control delays,
this function will combine shared settings in the DDRIO design and normalize the
lowest command, clock or control value to their minimum limits. This is for BRS.

@param[in] Host                 - Pointer to Host structure
@param[in] Socket               - Processor socket within the system (0-based)
@param[in] Ch                   - DDR channel number within the processor socket (0-based)
@param[in] Mode                 - Bit-field of different modes
@param[in] ListType             - Selects type of each list
@param[in] ListSize             - Number of entries in each list
@param[in] Value                - Array of pointers to listType
@param[in] ValueSize            - List data size

@retval MRC_STATUS

**/
MRC_STATUS
SetCombinedCmdGroupDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Mode,
  IN UINT8     ListType,
  IN UINT16    ListSize,
  IN VOID      *Value,
  IN UINT32    ValueSize
  )
{
  UINT8                 Dimm;
  UINT8                 Rank;
  UINT8                 RankIndex;
  UINT8                 SubChLocal;
  UINT8                 RegIndex;
  UINT8                 Clk;
  UINT8                 ListIndex;
  GSM_CSN               Signal;
  UINT8                 CmdPiGroup;
  UINT8                 CkEnabled[MAX_CLK];
  UINT8                 CtlIndex[MAX_CLK];
  UINT8                 *Allocation;
  UINT8                 MaxCmdCtlGroupSubCh;
  INT16                 (*CmdLeft)[MAX_CLK];
  INT16                 (*CmdRight)[MAX_CLK];
  MRC_GT                (*CmdGroup)[MAX_CLK];
  GSM_CSN               (*CmdSignal)[MAX_CLK];
  INT16                 *CmdOffset;
  UINT8                 SubChannel = 0;
  MRC_STATUS            Status;
  GSM_CSEDGE_CA         (*RcdsignalListEdge) [SUB_CH][MAX_RANK_CH][RCD_CA_SIGNALS_TO_SWEEP];
  GSM_CSEDGE_CA         (*DramsignalListEdge) [SUB_CH][MAX_RANK_CH][DRAM_CA_SIGNALS_TO_SWEEP];
  struct ddrRank        (*RankList) [MAX_RANK_DIMM];
  struct dimmNvram      (*DimmNvList) [MAX_DIMM];

  Status = MRC_STATUS_SUCCESS;
  MaxCmdCtlGroupSubCh = MAX_CMDCTL_ARRAY_SPR / 2;
  Allocation = RcAllocatePool (MaxCmdCtlGroupSubCh * 2 * (sizeof (*CmdLeft) + sizeof (*CmdRight) + sizeof (*CmdGroup) + sizeof (*CmdSignal) +
                               sizeof (*CmdOffset)));
  if (Allocation == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return MRC_STATUS_FAILURE;
  }

  CmdLeft = (INT16 (*) [MAX_CLK]) ((UINTN) Allocation);
  CmdRight = (INT16 (*) [MAX_CLK]) ((UINTN) CmdLeft + (MaxCmdCtlGroupSubCh * 2 * sizeof (*CmdLeft)));
  CmdGroup = (MRC_GT (*) [MAX_CLK]) ((UINTN) CmdRight + (MaxCmdCtlGroupSubCh * 2 * sizeof (*CmdRight)));
  CmdSignal = (GSM_CSN (*) [MAX_CLK]) ((UINTN) CmdGroup + (MaxCmdCtlGroupSubCh * 2 * sizeof (*CmdGroup)));
  CmdOffset = (INT16*) ((UINTN) CmdSignal + (MaxCmdCtlGroupSubCh * 2 * sizeof (*CmdSignal)));
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  for (Clk = 0; Clk < MAX_CLK; Clk++) { //DDR5 8 clks
    CkEnabled[Clk] = 0;
    CtlIndex[Clk] = 0;
  } // clk loop

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    RankList = GetRankNvList (Host, Socket, Ch, Dimm);
    for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {

      // If Rank enabled
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
        continue;
      }

      Clk = (*RankList) [Rank].ckIndex;
      CtlIndex[Clk] = (*RankList) [Rank].ctlIndex;
      CkEnabled[Clk] = 1;
    } // Rank loop
  } // Dimm loop

  //
  // Initialize the edges for each CMD pi group
  //
  for (CmdPiGroup = 0; CmdPiGroup < MaxCmdCtlGroupSubCh * 2; CmdPiGroup++) {
    // Init CK composite edges to 255
    for (Clk = 0; Clk < MAX_CLK; Clk++) {
      CmdLeft[CmdPiGroup][Clk] = -UNMARGINED_CMD_EDGE;
      CmdRight[CmdPiGroup][Clk] = UNMARGINED_CMD_EDGE;
    } // clk loop
    CmdOffset[CmdPiGroup] = 0;
  } // cmdPiGroup loop
  if (ListType == CMDCTL_SIGNAL_DELAY) {
    //
    // Combine edges if necessary
    //
    if (Mode == GSM_COMBINE_EDGES) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "Calculate CA Composite edges per PI Group per Dimm\n");
      if (CheckRCDLoopback (Host)) {
        if (ValueSize == sizeof (*RcdsignalListEdge)) {
          RcdsignalListEdge = Value;
        } else {
          RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_225);
        }
      } else if (ValueSize == sizeof (*DramsignalListEdge)) {
        DramsignalListEdge = Value;
      } else {
        RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_225);
      }
      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
          RankList = GetRankNvList (Host, Socket, Ch, Dimm);

          for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
            if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
              continue;
            }

            //skip ranks >0 if rdimm
            if ((Host->nvram.mem.dimmTypePresent != UDIMM) && (Rank > 0)) {
              continue;
            }

            RankIndex = (*RankList)[Rank].rankIndex;

            //
            // Loop through the list to create the composite left and right edges for each CMD group
            //
            for (ListIndex = 0; ListIndex < ListSize; ListIndex++) {
              Clk = (*RankList) [Rank].ckIndex;
              //
              // Get the silicon pi register index this signal belongs to
              //
              if (CheckRCDLoopback (Host)) {
                if (ListIndex < RCD_CA_SIGNALS_TO_SWEEP) {
                  Signal = (*RcdsignalListEdge)[SubChannel][RankIndex][ListIndex].signal;
                }
                else {
                  RcDebugPrint (SDBG_ERROR, "Error: Array 'RcdsignalListEdge' out of range\n");
                }
              } else {
                Signal = (*DramsignalListEdge)[SubChannel][RankIndex][ListIndex].signal;
              }
              Status = SignalToRegIndex (Host, Socket, Ch, Signal, &SubChLocal, &RegIndex);

              //
              // Get the index into the CMD silicon pi group
              //
              CmdPiGroup = RegIndex + (MaxCmdCtlGroupSubCh * SubChannel);
              RC_FATAL_ERROR (CmdPiGroup != MaxCmdCtlGroupSubCh * 2, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_44);

              //
              // Update the composite left and right edges for the current CMD pi group relative to the clock
              //
              if (CheckRCDLoopback (Host)) {
                UpdateEdges ((*RcdsignalListEdge)[SubChannel][RankIndex][ListIndex].le, (*RcdsignalListEdge)[SubChannel][RankIndex][ListIndex].re,
                  &CmdLeft[CmdPiGroup][Clk], &CmdRight[CmdPiGroup][Clk]);
                CmdGroup[CmdPiGroup][Clk] = MrcGtDelim;
                CmdSignal[CmdPiGroup][Clk] = Signal;
                RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
                                    "%8a: CA PI Group %2d, Clk %d: le = %3d re = %3d, Left = %3d Right = %3d\n",
                                    GetSignalStrDdr5 (Signal), CmdPiGroup- MaxCmdCtlGroupSubCh * SubChannel, Clk,
                                    (*RcdsignalListEdge)[SubChannel][RankIndex][ListIndex].le, (*RcdsignalListEdge)[SubChannel][RankIndex][ListIndex].re,
                                    CmdLeft[CmdPiGroup][Clk], CmdRight[CmdPiGroup][Clk]);
              } else {
                UpdateEdges ((*DramsignalListEdge)[SubChannel][RankIndex][ListIndex].le, (*DramsignalListEdge)[SubChannel][RankIndex][ListIndex].re,
                  &CmdLeft[CmdPiGroup][Clk], &CmdRight[CmdPiGroup][Clk]);
                CmdGroup[CmdPiGroup][Clk] = MrcGtDelim;
                CmdSignal[CmdPiGroup][Clk] = Signal;
                RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
                                    "%8a: CA PI Group %2d, Clk %d: le = %3d re = %3d, Left = %3d Right = %3d\n",
                                    GetSignalStrDdr5 (Signal), CmdPiGroup, Clk,
                                    (*DramsignalListEdge)[SubChannel][RankIndex][ListIndex].le, (*DramsignalListEdge)[SubChannel][RankIndex][ListIndex].re,
                                    CmdLeft[CmdPiGroup][Clk], CmdRight[CmdPiGroup][Clk]);
              }

            } // ListIndex loop
            RcDebugPrint (SDBG_MAX, "\n");
          } // Rank loop
        }//subchannel loop
      } // Dimm loop
      //
      // Evaluate CMD margins and offset CMD, CTL, and Clk appropriately
      //
      EvaluateCMDMarginsDdr5 (Host, Socket, Ch, CkEnabled, CtlIndex, CmdLeft, CmdRight, CmdGroup, CmdSignal, CmdOffset);
    } else {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "ERROR: This SetCombinedCmd Mode not supported!\n");
    }
  } else {
    Status = MRC_STATUS_GROUP_NOT_SUPPORTED;
  }

  RcFreePool (Allocation);
  return Status;
} // SetCombinedCmdGroupDdr5

  /**

  For DCA DCK duty cycle training, given an array of command delays relative to current clock and control delays,
  this function will combine shared settings in the DDRIO design and normalize the
  lowest command, clock or control value to their minimum limits.

  @param[in] Host                 - Pointer to Host structure
  @param[in] Socket               - Processor socket within the system (0-based)
  @param[in] Ch                   - DDR channel number within the processor socket (0-based)
  @param[in] Subch                - DDR sub channel number
  @param[in] Level                - IO level to access
  @param[in] Mode                 - Bit-field of different modes
  @param[in] ListType             - Selects type of each list
  @param[in] ListSize             - Number of entries in each list
  @param[in] Value                - Array of pointers to listType

  @retval MRC_STATUS

  **/
MRC_STATUS
SetCombinedCmdGroupDutyCycleDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Subch,
  IN MRC_LT    Level,
  IN UINT8     Mode,
  IN UINT8     ListType,
  IN UINT16    ListSize,
  IN VOID      *Value
  )
{
  UINT8                 Dimm;
  UINT8                 Rank;
  UINT8                 RankIndex;
  UINT8                 SubChLocal;
  UINT8                 RegIndex;
  UINT8                 Clk;
  UINT8                 ListIndex;
  GSM_CSN               Signal;
  UINT8                 CmdPiGroup;
  UINT8                 CkEnabled[MAX_CLK];
  UINT8                 CtlIndex[MAX_CLK];
  UINT8                 *Allocation;
  UINT8                 MaxCmdCtlGroupSubCh;
  INT16                 (*CmdLeft)[MAX_CLK][MAX_SAMPLING_LOOP];
  INT16                 (*CmdRight)[MAX_CLK][MAX_SAMPLING_LOOP];
  MRC_GT                (*CmdGroup)[MAX_CLK][MAX_SAMPLING_LOOP];
  GSM_CSN               (*CmdSignal)[MAX_CLK];
  UINT8                 SubChannel = 0;
  UINT8                 MaxCmdGroupAll;
  SIGNAL_DESCRIPTOR     *CmdGroupAll;
  UINT8                 EvenOddLoop = 0;
  MRC_STATUS            Status;
  GSM_CSEDGE_CA         (*DutyCycleSignalEdgeData) [SUB_CH][MAX_SAMPLING_LOOP][MAX_RANK_CH][RCD_CA_SIGNALS_TO_SWEEP];
  struct ddrRank        (*RankList) [MAX_RANK_DIMM];
  struct dimmNvram      (*DimmNvList) [MAX_DIMM];

  Status = MRC_STATUS_SUCCESS;

  MaxCmdCtlGroupSubCh = MAX_CMDCTL_ARRAY_SPR / 2;

  Allocation = RcAllocatePool (MaxCmdCtlGroupSubCh * 2 * (sizeof (*CmdLeft) + sizeof (*CmdRight) + sizeof (*CmdGroup) + sizeof (*CmdSignal)));

  if (Allocation == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return MRC_STATUS_FAILURE;
  }

  CmdLeft = (INT16 (*) [MAX_CLK][MAX_SAMPLING_LOOP]) ((UINTN) Allocation);
  CmdRight = (INT16 (*) [MAX_CLK][MAX_SAMPLING_LOOP]) ((UINTN) CmdLeft + (MaxCmdCtlGroupSubCh * 2 * sizeof (*CmdLeft)));
  CmdGroup = (MRC_GT (*) [MAX_CLK][MAX_SAMPLING_LOOP]) ((UINTN) CmdRight + (MaxCmdCtlGroupSubCh * 2 * sizeof (*CmdRight)));
  CmdSignal = (GSM_CSN (*) [MAX_CLK]) ((UINTN) CmdGroup + (MaxCmdCtlGroupSubCh * 2 * sizeof (*CmdGroup)));

  DutyCycleSignalEdgeData = Value;
  Status = GetCmdCtlGroupList (Host, CmdAll, &MaxCmdGroupAll, &CmdGroupAll);
  ASSERT (MaxCmdGroupAll <= (MaxCmdCtlGroupSubCh * 2));

  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  for (Clk = 0; Clk < MAX_CLK; Clk++) {
    CkEnabled[Clk] = 0;
    CtlIndex[Clk] = 0;
  } // clk loop

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    RankList = GetRankNvList (Host, Socket, Ch, Dimm);
    for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
      // If rank enabled
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
        continue;
      }
      Clk = (*RankList) [Rank].ckIndex;
      CtlIndex[Clk] = (*RankList) [Rank].ctlIndex;
      CkEnabled[Clk] = 1;
    } // rank loop
  } // dimm loop

  //
  // Initialize the edges for each CMD pi group
  //
  for (CmdPiGroup = 0; CmdPiGroup < MaxCmdCtlGroupSubCh * 2; CmdPiGroup++) {
    // Init CK composite edges to 255
    for (Clk = 0; Clk < MAX_CLK; Clk++) {
      for (EvenOddLoop = 0; EvenOddLoop < MAX_SAMPLING_LOOP; EvenOddLoop++) {
        CmdLeft[CmdPiGroup][Clk][EvenOddLoop] = -UNMARGINED_CMD_EDGE;
        CmdRight[CmdPiGroup][Clk][EvenOddLoop] = UNMARGINED_CMD_EDGE;
      }
    } // clk loop
  } // cmdPiGroup loop
  //
  // Combine edges if necessary
  //
  if (Mode == GSM_COMBINE_EDGES) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Calculate CA Composite edges per PI Group per DIMM\n");

    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      for (EvenOddLoop = 0; EvenOddLoop < MAX_SAMPLING_LOOP; EvenOddLoop++) {

        if (EvenOddLoop == 0) {
          RcDebugPrint (SDBG_MAX, "Rising Edge\n");
        } else {
          RcDebugPrint (SDBG_MAX, "Falling Edge\n");
        }

        for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
          RankList = GetRankNvList (Host, Socket, Ch, Dimm);

          for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
            if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
              continue;
            }

            //skip ranks >0 if rdimm
            if ((Host->nvram.mem.dimmTypePresent != UDIMM) && (Rank > 0)) {
              continue;
            }

            RankIndex = (*RankList) [Rank].rankIndex;

            //
            // Loop through the list to create the composite left and right edges for each CMD group
            //
            for (ListIndex = 0; ListIndex < ListSize; ListIndex++) {

              Clk = (*RankList) [Rank].ckIndex;

              //
              // Get the silicon pi register index this signal belongs to
              //
              if (ListIndex < RCD_CA_SIGNALS_TO_SWEEP) {
                Signal = (*DutyCycleSignalEdgeData) [SubChannel][EvenOddLoop][RankIndex][ListIndex].signal;
              }
              else
              {
                RcDebugPrint (SDBG_ERROR, "Error: Array 'DutyCycleSignalEdgeData' out of range\n");
              }
              Status = SignalToRegIndex (Host, Socket, Ch, Signal, &SubChLocal, &RegIndex);

              //
              // Get the index into the CMD silicon pi group
              //
              CmdPiGroup = RegIndex + (MaxCmdCtlGroupSubCh * SubChannel);
              RC_FATAL_ERROR (CmdPiGroup != MaxCmdCtlGroupSubCh * 2, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_44);

              //
              // Update the composite left and right edges for the current CMD pi group relative to the clock
              //
              UpdateEdges ((*DutyCycleSignalEdgeData) [SubChannel][EvenOddLoop][RankIndex][ListIndex].le, (*DutyCycleSignalEdgeData) [SubChannel][EvenOddLoop][RankIndex][ListIndex].re,
                &CmdLeft[CmdPiGroup][Clk][EvenOddLoop], &CmdRight[CmdPiGroup][Clk][EvenOddLoop]);
              CmdSignal[CmdPiGroup][Clk] = Signal;
              RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
                "%8a: CA PI Group %2d, clk %d: le = %3d re = %3d, Left = %3d Right = %3d\n",
                GetSignalStrDdr5 (Signal), CmdPiGroup- MaxCmdCtlGroupSubCh * SubChannel, Clk,
                (*DutyCycleSignalEdgeData) [SubChannel][EvenOddLoop][RankIndex][ListIndex].le, (*DutyCycleSignalEdgeData) [SubChannel][EvenOddLoop][RankIndex][ListIndex].re,
                CmdLeft[CmdPiGroup][Clk][EvenOddLoop], CmdRight[CmdPiGroup][Clk][EvenOddLoop]);

            } // listIndex loop
            RcDebugPrint (SDBG_MAX, "\n");
          } // rank loop
        }// dimm loop
      } //EvenOdd Loop
    } //subchannel loop

    //
    // Evaluate CMD margins and offset CMD PIE OFFSET, CTL, and CLK appropriately
    //
    EvaluateCMDMarginsDutyCycleDdr5 (Host, Socket, Ch, CkEnabled, CtlIndex, CmdLeft, CmdRight);
  } else {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "ERROR: This SetCombinedCmd Mode not supported!\n");
  }
  RcFreePool (Allocation);
  return Status;
} // SetCombinedCmdGroupDutyCycleDdr5

/**

Evaluates the CMD margins and determines the appropriate offset

@param Host                 - Pointer to Host structure
@param socket               - Processor socket within the system (0-based)

@retval MRC_STATUS

**/
STATIC VOID
EvaluateCaBacksideMargins (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     ckEnabled[MAX_CLK_BS_SUBCH_DDR5],
  UINT8     ctlIndex[MAX_CLK_BS_SUBCH_DDR5],
  INT16     cmdLeft[COPY_NUM][MAX_CLK_BS_SUBCH_DDR5],
  INT16     cmdRight[COPY_NUM][MAX_CLK_BS_SUBCH_DDR5],
  MRC_GT    group[COPY_NUM][MAX_CLK_BS_SUBCH_DDR5],
  GSM_CSN   cmdSignal[COPY_NUM][MAX_CLK_BS_SUBCH_DDR5],
  INT16     cmdOffset[COPY_NUM]
  )
{
  UINT8                 dimm;
  UINT8                 rank;
  UINT8 clk;
  UINT8 cmdPiGroup;
  INT16                 minCmdOffset;
  INT16                 maxCmdOffset;
  INT16                 cmdOffsetClk[COPY_NUM][MAX_CLK_BS_SUBCH_DDR5];
  INT16                 maxLeftOffset;
  UINT16                ctlMinVal;
  UINT16                ctlMaxVal;
  INT16                 minRightOffset;
  INT16                 ckOffset[MAX_CLK];
  UINT8                 earlyCmdClkDisable = 0;
  UINT8                 parityWorkaround = 0;
  struct ddrRank (*rankList) [MAX_RANK_DIMM];
  struct dimmNvram (*dimmNvList) [MAX_DIMM];

  //
  // Determine if early CMD/CLK training has been disabled
  //
  dimmNvList = GetDimmNvList (Host, socket, ch);

  //
  // Determine if early CMD/CLK training has been disabled
  //
  earlyCmdClkDisable = ((!IsMemFlowEnabled (EarlyCmdClkTraining)) || parityWorkaround);//DDR5_TODO.

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Calculate offset per PI group per sub channel per DIMM \n");

  for (clk = 0; clk < MAX_CLK_BS_SUBCH_DDR5; clk++) {
    if (ckEnabled[clk] == 0) {
      continue;
    }
    //
    // Initialize these values so the maximum CMD delay can be determined
    //
    minCmdOffset = 128;
    maxCmdOffset = -128;

    //
    // Determine the offset for each CMD pi group and the max offset
    //
    for (cmdPiGroup = 0; cmdPiGroup < COPY_NUM; cmdPiGroup++) {

      //
      // Make sure the eye width is large enough
      //
      if ((cmdRight[cmdPiGroup][clk] - cmdLeft[cmdPiGroup][clk]) < 20) { //DDR5_TODO.
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                        "Eye width is too small: %d\n",
                        cmdRight[cmdPiGroup][clk] - cmdLeft[cmdPiGroup][clk]);
        EwlOutputType3 (WARN_FPT_CORRECTABLE_ERROR, WARN_CMD_PI_GROUP_SMALL_EYE, socket, ch, NO_DIMM, NO_RANK, EwlSeverityWarning, group[cmdPiGroup][clk], DdrLevel, cmdSignal[cmdPiGroup][clk], (UINT8) (cmdRight[cmdPiGroup][clk] - cmdLeft[cmdPiGroup][clk]));
        DisableChannelSw (Host, socket, ch);
      }
      if ((cmdLeft[cmdPiGroup][clk] != -UNMARGINED_CMD_EDGE) && (cmdRight[cmdPiGroup][clk] != UNMARGINED_CMD_EDGE)) {
        cmdOffsetClk[cmdPiGroup][clk] = (cmdLeft[cmdPiGroup][clk] + cmdRight[cmdPiGroup][clk]) / 2;

        UpdateMinMaxInt (cmdOffsetClk[cmdPiGroup][clk], &minCmdOffset, &maxCmdOffset);

        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                        "Backside CA PI Group %2d clk %d cmdOffset %d\n",
                        cmdPiGroup, clk, cmdOffsetClk[cmdPiGroup][clk]);
      }
    } // cmdPiGroup loop

  } // clk offset

  //
  // Calculate the final offset for each CMD pi group
  //
  RcDebugPrint (SDBG_MAX, "\n");
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Calculate Final offset per PI group per CH \n");

  for (cmdPiGroup = 0; cmdPiGroup < COPY_NUM; cmdPiGroup++) {
    minCmdOffset = 255;
    maxCmdOffset = -255;

    for (clk = 0; clk < MAX_CLK_BS_SUBCH_DDR5; clk++) {
      if (ckEnabled[clk]) {
        if ((cmdLeft[cmdPiGroup][clk] != -UNMARGINED_CMD_EDGE) && (cmdRight[cmdPiGroup][clk] != UNMARGINED_CMD_EDGE)) {
          UpdateMinMaxInt (cmdOffsetClk[cmdPiGroup][clk], &minCmdOffset, &maxCmdOffset);
        }
      }
    } // clk loop

    // Save common CMD offset
    cmdOffset[cmdPiGroup] = (maxCmdOffset + minCmdOffset) / 2;
    //
    // Write the CMD pi Group
    //
    if ((maxCmdOffset != -255) && (minCmdOffset != 255)) {
      //DDR5_TODO. Use the MRW to write the delay.
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "CMD Pi Group %2d: maxOffset = %d, minOffset = %d, cmdOffset = %d\n",
                      cmdPiGroup, maxCmdOffset, minCmdOffset, cmdOffset[cmdPiGroup]);
    }
  } // cmdPiGroup loop
  //
  // Determine the clock offset
  //
  RcDebugPrint (SDBG_MAX, "\n");
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Calculate Clk offset \n");
  for (cmdPiGroup = 0; cmdPiGroup < COPY_NUM; cmdPiGroup++) {
    for (clk = 0; clk < MAX_CLK_BS_SUBCH_DDR5; clk++) {
      if (ckEnabled[clk] == 0) {
        continue;
      }
      if ((cmdLeft[cmdPiGroup][clk] != -UNMARGINED_CMD_EDGE) && (cmdRight[cmdPiGroup][clk] != UNMARGINED_CMD_EDGE)) {
        cmdLeft[cmdPiGroup][clk] = cmdLeft[cmdPiGroup][clk] - cmdOffset[cmdPiGroup];
        cmdRight[cmdPiGroup][clk] = cmdRight[cmdPiGroup][clk] - cmdOffset[cmdPiGroup];

        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                        "<--CMD Pi Group %2d clk %d: cmdLeft %d - cmdRight %d\n",
                        cmdPiGroup, clk, cmdLeft[cmdPiGroup][clk], cmdRight[cmdPiGroup][clk]);
      }
    } // clk offset
  } // cmdPiGroup loop

  for (clk = 0; clk < MAX_CLK_BS_SUBCH_DDR5; clk++) {
    if (ckEnabled[clk] == 0) {
      continue;
    }

    minRightOffset = 255;
    maxLeftOffset = -255;

    for (cmdPiGroup = 0; cmdPiGroup < COPY_NUM; cmdPiGroup++) {
      if ((cmdLeft[cmdPiGroup][clk] != -UNMARGINED_CMD_EDGE) && (cmdRight[cmdPiGroup][clk] != UNMARGINED_CMD_EDGE)) {
        UpdateMinMaxInt (cmdLeft[cmdPiGroup][clk], &minCmdOffset, &maxLeftOffset);
        UpdateMinMaxInt (cmdRight[cmdPiGroup][clk], &minRightOffset, &maxCmdOffset);
      }
    } // cmdPiGroup loop

    ckOffset[clk] = - (minRightOffset + maxLeftOffset) / 2;

    //
    // Write the CTL delay--shift the CS Delay--DDR5_TODO.
    //
    ctlMinVal = 255;
    ctlMaxVal = 0;
    //
    // DDR5_TODO. Based on CLK--A/B CS mapping to shift the QCS delay.
    //
    GetSetCtlGroupDelay (Host, socket, ch, ctlIndex[clk] + CtlGrp0, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE, &ckOffset[clk], &ctlMinVal, &ctlMaxVal);
    //
    // Write the CLK delay
    //
    GetSetClkDelay (Host, socket, ch, clk, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE, &ckOffset[clk]);//DDR5_TODO.
    //
    // Only move IO Delays if in late CMD CLK (early CMD CLK disabled OR early CMD CLK executed already)
    //
    //DDR5_TODO. To update the below code.
    if (earlyCmdClkDisable || (Host->var.mem.earlyCmdClkExecuted == 1)) {
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        rankList = GetRankNvList (Host, socket, ch, dimm);
        for (rank = 0; rank < (*dimmNvList) [dimm].numRanks; rank++) {
          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          if (clk == (*rankList) [rank].ckIndex) {
            /*
            GetSetDataGroup(Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, 0, DdrLevel, RecEnDelay, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE,
              (INT16 *)&ckOffset[clk]);
            GetSetDataGroup(Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, 0, DdrLevel, TxDqDelay, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE,
              (INT16 *)&ckOffset[clk]);
            GetSetDataGroup(Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, 0, DdrLevel, TxDqsDelay, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE,
              (INT16 *)&ckOffset[clk]);
            */
          }
        } // rank loop
      } // dimm loop
    }

    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "<----clk %d ckOffset %d: -(maxLeftOffset:%d + minRightOffset:%d) / 2\n",
                    clk, ckOffset[clk], maxLeftOffset, minRightOffset);

  } // clk loop
} // EvaluateCaBacksideMargins

/**

Given an array of command delays relative to current clock and control delays,
this function will combine shared settings in the DDRIO design and normalize the
lowest command, clock or control value to their minimum limits.

@param[in] Host                 - Pointer to Host structure
@param[in] Socket               - Processor socket within the system (0-based)
@param[in] Ch                   - DDR channel number within the processor socket (0-based)
@param[in] SubChannel           - DDR sub channel number
@param[in] Level                - IO level to access
@param[in] Mode                 - Bit-field of different modes
@param[in] ListType             - Selects type of each list
@param[in] ListSize             - Number of entries in each list
@param[in] Value                - Array of pointers to listType
@param[in] ValueSize            - List data size

@retval MRC_STATUS

**/
MRC_STATUS
SetCombinedCaGroupDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     SubChannel,
  IN MRC_LT    Level,
  IN UINT8     Mode,
  IN UINT8     ListType,
  IN UINT16    ListSize,
  IN VOID      *Value,
  IN UINT32    ValueSize
  )
{
  MRC_STATUS    Status;
  GSM_CSEDGE_CA (*RcdsignalListEdgeCh) [MAX_CH][SUB_CH][MAX_RANK_CH][RCD_CA_SIGNALS_TO_SWEEP];
  GSM_CSEDGE_CA (*DramsignalListEdgeCh) [MAX_CH][SUB_CH][MAX_RANK_CH][DRAM_CA_SIGNALS_TO_SWEEP];
  GSM_CSEDGE_CA (*RcdBacksidesignalListEdgeCh) [MAX_CH][SUB_CH][MAX_RANK_CH][COPY_NUM][BACKSIDE_CA_SIGNALS_TO_SWEEP];
  GSM_CSEDGE_CA (*DutyCycleSignalEdgeData) [MAX_CH][SUB_CH][MAX_SAMPLING_LOOP][MAX_RANK_CH][RCD_CA_SIGNALS_TO_SWEEP];
  UINT8         MaxChDdr;

  if (GetCurrentTestType (Socket) == EarlyCaBacksideLoopbackTest) {
    if (ValueSize == sizeof (*RcdBacksidesignalListEdgeCh)) {
      RcdBacksidesignalListEdgeCh = Value;
    } else {
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_224);
    }
  } else if (GetCurrentTestType (Socket) == DcaDckDutyCycleTest) {
    if (ValueSize == sizeof (*DutyCycleSignalEdgeData)) {
      DutyCycleSignalEdgeData = Value;
    } else {
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_224);
    }
  } else if (CheckRCDLoopback (Host)) {
    if (ValueSize == sizeof (*RcdsignalListEdgeCh)) {
      RcdsignalListEdgeCh = Value;
    } else {
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_224);
    }
  } else if (ValueSize == sizeof (*DramsignalListEdgeCh)) {
    DramsignalListEdgeCh = Value;
  } else {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_224);
  }

  Status   = MRC_STATUS_SUCCESS;
  MaxChDdr = GetMaxChDdr ();
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (!IsChannelEnabled (Socket, Ch)) {
      continue;
    }
    //
    // Evaluate results and write them
    //
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Summary:\n");
    if (GetCurrentTestType (Socket) == EarlyCaBacksideLoopbackTest) {
      Status = SetCombinedCaClkBacksideDdr5 (Host, Socket, Ch, SubChannel, Level, Mode, ListType, ListSize, &(*RcdBacksidesignalListEdgeCh) [Ch]);
    } else if (GetCurrentTestType (Socket) == DcaDckDutyCycleTest) {
      Status = SetCombinedCmdGroupDutyCycleDdr5 (Host, Socket, Ch, SubChannel, Level, Mode, ListType, ListSize, (*DutyCycleSignalEdgeData) [Ch]);
    } else if (CheckRCDLoopback (Host)) {
      Status = SetCombinedCmdGroupDdr5 (Host, Socket, Ch, Mode, ListType, ListSize, (*RcdsignalListEdgeCh) [Ch], sizeof ((*RcdsignalListEdgeCh) [Ch]));
    } else {
      Status = SetCombinedCmdGroupDdr5 (Host, Socket, Ch, Mode, ListType, ListSize, (*DramsignalListEdgeCh) [Ch], sizeof ((*DramsignalListEdgeCh) [Ch]));
    }
  } // Ch loop

  return Status;
} //SetCombinedCaGroupDdr5


/**
  Evaluates the CMD margins and determines the appropriate offset

  @param [in    ] Host       - Pointer to Host structure
  @param [in    ] Socket     - Processor Socket within the system (0-based)
  @param [in    ] Ch         - Memory channel number
  @param [in    ] CkEnabled  - Array of clocks enabled
  @param [in    ] CtlIndex   - Array of control indexes
  @param [in out] CmdLeft    - Array of command values for the left side
  @param [in out] CmdRight   - Array of command values for the right side
  @param [in    ] Group      - Command Group
  @param [in    ] CmdSignal  - Command signal
  @param [   out] CmdOffset  - Command offset

  @retval None
**/
STATIC
VOID
EvaluateCMDMarginsDdr5 (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch,
  IN     UINT8     CkEnabled[MAX_CLK],
  IN     UINT8     CtlIndex[MAX_CLK],
  IN OUT INT16     CmdLeft[MAX_CMDCTL_ARRAY][MAX_CLK],
  IN OUT INT16     CmdRight[MAX_CMDCTL_ARRAY][MAX_CLK],
  IN     MRC_GT    Group[MAX_CMDCTL_ARRAY][MAX_CLK],
  IN     GSM_CSN   CmdSignal[MAX_CMDCTL_ARRAY][MAX_CLK],
  OUT    INT16     CmdOffset[MAX_CMDCTL_ARRAY]
  )
{
  UINT8                 Dimm;
  UINT8                 Rank;
  UINT8                 Clk;
  UINT8                 CmdPiGroup;
  UINT8                 MaxCmdCtlGroupSubCh;
  INT16                 MinCmdOffset;
  INT16                 MaxCmdOffset;
  INT16                 (*CmdOffsetClk) [MAX_CLK];
  INT16                 MaxLeftOffset;
  UINT16                CtlMinVal;
  UINT16                CtlMaxVal;
  UINT8                 SubChannel;
  INT16                 MinRightOffset;
  INT16                 CkOffset[MAX_CLK];
  UINT32                EarlyCmdClkDisable = 0;
  struct ddrRank        (*RankList) [MAX_RANK_DIMM];
  struct dimmNvram      (*DimmNvList) [MAX_DIMM];
  UINT8                 CmdSetupOffset;
  INT16                 TempValue;
  SYS_SETUP             *Setup;

  Setup = GetSysSetupPointer ();

  MaxCmdCtlGroupSubCh = MAX_CMDCTL_ARRAY_SPR / 2;

  CmdOffsetClk = (INT16 (*) [MAX_CLK]) RcAllocatePool (MaxCmdCtlGroupSubCh * 2 * sizeof (*CmdOffsetClk));
  if (CmdOffsetClk == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return;
  }

  CmdSetupOffset = Setup->mem.cmdSetupPercentOffset;

  //
  // Determine if early CMD/CLK training has been disabled
  //
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  //
  // Determine if early CMD/CLK training has been disabled
  //
  EarlyCmdClkDisable = !IsMemFlowEnabled (CaClkTraining);

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Calculate offset per PI Group per DIMM \n");

  for (Clk = 0; Clk < MAX_CLK; Clk++) {
    if (CkEnabled[Clk] == 0) {
      continue;
    }
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      //
      // Initialize these values so the maximum CMD delay can be determined
      //
      MinCmdOffset = 255;
      MaxCmdOffset = -255;

      //
      // Determine the offset for each CMD pi Group and the max offset
      //
      for (CmdPiGroup = 0; CmdPiGroup < MaxCmdCtlGroupSubCh; CmdPiGroup++) {

        CmdPiGroup = CmdPiGroup + MaxCmdCtlGroupSubCh * SubChannel;
        //
        // Make sure the eye width is large enough
        //
        if ((CmdLeft[CmdPiGroup][Clk] != -UNMARGINED_CMD_EDGE) && (CmdRight[CmdPiGroup][Clk] != UNMARGINED_CMD_EDGE)) {
          if ((CmdRight[CmdPiGroup][Clk] - CmdLeft[CmdPiGroup][Clk]) < MinimumMargin (Host, CA)) {
            RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "Eye width is too small: %d\n",
                            CmdRight[CmdPiGroup][Clk] - CmdLeft[CmdPiGroup][Clk]);
            OutputWarning (WARN_CMD_CLK_TRAINING, 0, Socket, Ch, NO_DIMM, NO_RANK);
            DisableChannelSw (Host, Socket, Ch);
          }
        }
        if ((CmdLeft[CmdPiGroup][Clk] != -UNMARGINED_CMD_EDGE) && (CmdRight[CmdPiGroup][Clk] != UNMARGINED_CMD_EDGE)) {
          CmdOffsetClk[CmdPiGroup][Clk] = (CmdLeft[CmdPiGroup][Clk] + CmdRight[CmdPiGroup][Clk]) / 2;

          UpdateMinMaxInt (CmdOffsetClk[CmdPiGroup][Clk], &MinCmdOffset, &MaxCmdOffset);
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubChannel, NO_RANK, NO_STROBE, NO_BIT,
                          "CMD Pi Group %2d Clk %d CmdOffset %d\n",
                          CmdPiGroup - MaxCmdCtlGroupSubCh * SubChannel, Clk, CmdOffsetClk[CmdPiGroup][Clk]);
        }
        CmdPiGroup = CmdPiGroup - MaxCmdCtlGroupSubCh * SubChannel;
      } // CmdPiGroup loop
    } //subchannel loop
  } // Clk offset

  //
  // Calculate the final offset for each CMD pi Group
  //
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Calculate Final offset per PI Group per CH \n");
  for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
    for (CmdPiGroup = 0; CmdPiGroup < MaxCmdCtlGroupSubCh; CmdPiGroup++) {
      //
      // Track MinCmdOffset, MaxCmdOffset across all CK groups
      //
      MinCmdOffset = MAX_CMD_DELAY_DDR5;
      MaxCmdOffset = MIN_CMD_DELAY_DDR5;
      CmdPiGroup = CmdPiGroup + MaxCmdCtlGroupSubCh * SubChannel;
      for (Clk = 0; Clk < MAX_CLK; Clk++) {
        if (CkEnabled[Clk]) {
          if ((CmdLeft[CmdPiGroup][Clk] != -UNMARGINED_CMD_EDGE) && (CmdRight[CmdPiGroup][Clk] != UNMARGINED_CMD_EDGE)) {
            UpdateMinMaxInt (CmdOffsetClk[CmdPiGroup][Clk], &MinCmdOffset, &MaxCmdOffset);
          }
        }
      } // Clk loop

      // Save common CMD offset
      CmdOffset[CmdPiGroup] = (MaxCmdOffset + MinCmdOffset) / 2;
      //
      // Setup/Hold parameter in late CMD CLK (early CMD CLK disabled OR early CMD CLK executed already)
      //
      if (EarlyCmdClkDisable || (Host->var.mem.earlyCmdClkExecuted == 1)) {
        CmdOffset[CmdPiGroup] = MaxCmdOffset - (((PERCENT - CmdSetupOffset)* (MaxCmdOffset - MinCmdOffset)) / PERCENT);
      }

      //
      // Write the CMD pi Group
      //
      TempValue = CmdOffset[CmdPiGroup];
      CmdPiGroup = CmdPiGroup - MaxCmdCtlGroupSubCh * SubChannel;
      if ((MaxCmdOffset != MIN_CMD_DELAY_DDR5) && (MinCmdOffset != MAX_CMD_DELAY_DDR5)) {
        GetSetCmdDelay (Host, Socket, Ch, SubChannel, CmdPiGroup, (GSM_WRITE_OFFSET | GSM_UPDATE_CACHE), &TempValue);

        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubChannel, NO_RANK, NO_STROBE, NO_BIT,
          "CMD Pi Group %2d: maxOffset = %d, minOffset = %d, CmdOffset = %d\n",
          CmdPiGroup, MaxCmdOffset, MinCmdOffset, TempValue);
      }
    } // CmdPiGroup loop
  }//sub channel loop

  //
  // Determine the clock offset
  //
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Calculate Clk offset \n");
  for (Clk = 0; Clk < MAX_CLK; Clk++) {
    if (CkEnabled[Clk] == 0) {
      continue;
    }
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      for (CmdPiGroup = 0; CmdPiGroup < MaxCmdCtlGroupSubCh; CmdPiGroup++) {
        CmdPiGroup = CmdPiGroup + MaxCmdCtlGroupSubCh * SubChannel;
        if ((CmdLeft[CmdPiGroup][Clk] != -UNMARGINED_CMD_EDGE) && (CmdRight[CmdPiGroup][Clk] != UNMARGINED_CMD_EDGE)) {
          CmdLeft[CmdPiGroup][Clk] = CmdLeft[CmdPiGroup][Clk] - CmdOffset[CmdPiGroup];
          CmdRight[CmdPiGroup][Clk] = CmdRight[CmdPiGroup][Clk] - CmdOffset[CmdPiGroup];

          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubChannel, NO_RANK, NO_STROBE, NO_BIT,
                          "<--CMD Pi Group %2d Clk %d: CmdLeft %d - CmdRight %d\n",
                          CmdPiGroup - MaxCmdCtlGroupSubCh * SubChannel, Clk, CmdLeft[CmdPiGroup][Clk], CmdRight[CmdPiGroup][Clk]);
        }
        CmdPiGroup = CmdPiGroup - MaxCmdCtlGroupSubCh * SubChannel;
      } // Clk offset
    } // CmdPiGroup loop
  }//subchannel loop

  for (Clk = 0; Clk < MAX_CLK; Clk++) {
    if (CkEnabled[Clk] == 0) {
      continue;
    }

    MinRightOffset = 255;
    MaxLeftOffset = -255;
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      for (CmdPiGroup = 0; CmdPiGroup < MaxCmdCtlGroupSubCh; CmdPiGroup++) {
        CmdPiGroup = CmdPiGroup + MaxCmdCtlGroupSubCh * SubChannel;
        if ((CmdLeft[CmdPiGroup][Clk] != -UNMARGINED_CMD_EDGE) && (CmdRight[CmdPiGroup][Clk] != UNMARGINED_CMD_EDGE)) {
          UpdateMinMaxInt (CmdLeft[CmdPiGroup][Clk], &MinCmdOffset, &MaxLeftOffset);
          UpdateMinMaxInt (CmdRight[CmdPiGroup][Clk], &MinRightOffset, &MaxCmdOffset);
        }
        CmdPiGroup = CmdPiGroup - MaxCmdCtlGroupSubCh * SubChannel;
      } // CmdPiGroup loop
    }//subchannel loop

    CkOffset[Clk] = - (MinRightOffset + MaxLeftOffset) / 2;

    //
    // Write the CTL delay
    //
    CtlMinVal = 255;
    CtlMaxVal = 0;
    GetSetCtlGroupDelay (Host, Socket, Ch, CtlIndex[Clk] + CtlGrp0, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE, &CkOffset[Clk], &CtlMinVal, &CtlMaxVal);

    //
    // Write the CLK delay
    //
    GetSetClkDelay (Host, Socket, Ch, Clk, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE, &CkOffset[Clk]);
    //
    // Only move IO Delays if in late CMD CLK (early CMD CLK disabled OR early CMD CLK executed already)
    //
    if (EarlyCmdClkDisable || (Host->var.mem.earlyCmdClkExecuted == 1)) {
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          if (Clk == (*RankList) [Rank].ckIndex) {
            GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, ALL_STROBES, 0, DdrLevel, RecEnDelay, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE,
              (INT16 *)&CkOffset[Clk]);
            GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, ALL_STROBES, 0, DdrLevel, TxDqDelay, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE,
              (INT16 *)&CkOffset[Clk]);
            GetSetDataGroup (Host, Socket, Ch, NO_SUBCH, Dimm, Rank, ALL_STROBES, 0, DdrLevel, TxDqsDelay, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE,
              (INT16 *)&CkOffset[Clk]);
          }
        } // Rank loop
      } // Dimm loop
    }

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "<----Clk %d CkOffset %d: -(MaxLeftOffset:%d + MinRightOffset:%d) / 2\n",
                    Clk, CkOffset[Clk], MaxLeftOffset, MinRightOffset);

  } // Clk loop
  RcFreePool (CmdOffsetClk);
} // EvaluateCMDMarginsDdr5

/**
  Evaluates the CMD margins and determines the appropriate offset for DCA DCK Duty Cycle Training.

  @param [in]      Host       - Pointer to Host structure
  @param [in]      Socket     - Processor Socket within the system (0-based)
  @param [in]      Ch         - Memory channel number
  @param [in]      CkEnabled  - Array of clocks enabled
  @param [in]      CtlIndex   - Array of control indexes
  @param [in][out] CmdLeft    - Array of command values for the left side
  @param [in][out] CmdRight   - Array of command values for the right side

  @retval None
**/
STATIC
VOID
EvaluateCMDMarginsDutyCycleDdr5 (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch,
  IN     UINT8     CkEnabled[MAX_CLK],
  IN     UINT8     CtlIndex[MAX_CLK],
  IN OUT INT16     CmdLeft[MAX_CMDCTL_ARRAY][MAX_CLK][MAX_SAMPLING_LOOP],
  IN OUT INT16     CmdRight[MAX_CMDCTL_ARRAY][MAX_CLK][MAX_SAMPLING_LOOP]
  )
{
  UINT8                 Clk;
  UINT8                 CmdPiGroup;
  UINT8                 PerChannelCmdPiGroup;
  UINT8                 MaxCmdCtlGroupSubCh;
  INT16                 MinCmdOffset;
  INT16                 MaxCmdOffset;
  INT16                 MaxLeftOffset[MAX_SAMPLING_LOOP];
  UINT8                 SubChannel;
  INT16                 MinRightOffset[MAX_SAMPLING_LOOP];
  INT16                 ClkDelay[MAX_CLK];
  INT16                 ClkDelayPrev[MAX_CLK];
  INT16                 Value;
  struct dimmNvram      (*DimmNvList) [MAX_DIMM];
  INT16                 CompCmdLeft[MAX_CMDCTL_ARRAY][MAX_SAMPLING_LOOP];
  INT16                 CompCmdRight[MAX_CMDCTL_ARRAY][MAX_SAMPLING_LOOP];
  INT16                 EyeWidth[MAX_CMDCTL_ARRAY][MAX_SAMPLING_LOOP];
  UINT8                 EvenOddLoop = 0;
  INT16                 CmdPieOffset = 0;
  INT16                 CmdPieOffsetPrev = 0;
  INT16                 ClkPieOffset[MAX_CLK];
  INT16                 ClkPieOffsetPrev[MAX_CLK];
  UINT16                CtlMinVal = 0;
  UINT16                CtlMaxVal = 0;
  INT16                 CompositeEyeEven = 0;
  INT16                 CompositeEyeOdd = 0;

  MaxCmdCtlGroupSubCh = MAX_CMDCTL_ARRAY / 2;

  ZeroMem ((UINT8 *) EyeWidth, sizeof (EyeWidth));
  ZeroMem ((UINT8 *) ClkPieOffset, sizeof (ClkPieOffset));
  ZeroMem ((UINT8 *) ClkDelay, sizeof (ClkDelay));

  //Initialize Composite eye values
  for (EvenOddLoop = EVEN_SAMPLING; EvenOddLoop < MAX_SAMPLING_LOOP; EvenOddLoop++) {
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      for (CmdPiGroup = 0; CmdPiGroup < MaxCmdCtlGroupSubCh; CmdPiGroup++) {
        PerChannelCmdPiGroup = CmdPiGroup + MaxCmdCtlGroupSubCh * SubChannel;
        CompCmdLeft[PerChannelCmdPiGroup][EvenOddLoop] = -UNMARGINED_CMD_EDGE;
        CompCmdRight[PerChannelCmdPiGroup][EvenOddLoop] = UNMARGINED_CMD_EDGE;
      } // CmdPiGroup loop
    } //subchannel loop
  } //EvenOddLoop

  //
  // Determine if early CMD/CLK training has been disabled
  //
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Check the eye per PI Group per DIMM \n");
  for (EvenOddLoop = EVEN_SAMPLING; EvenOddLoop < MAX_SAMPLING_LOOP; EvenOddLoop++) {
    for (Clk = 0; Clk < MAX_CLK; Clk++) {
      if (CkEnabled[Clk] == 0) {
        continue;
      }
      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        //
        // Determine the offset for each CMD pi Group and the max offset
        //
        for (CmdPiGroup = 0; CmdPiGroup < MaxCmdCtlGroupSubCh; CmdPiGroup++) {

          PerChannelCmdPiGroup = CmdPiGroup + MaxCmdCtlGroupSubCh * SubChannel;
          //
          // Make sure the eye width is large enough
          //
          if ((CmdLeft[PerChannelCmdPiGroup][Clk][EvenOddLoop] != -UNMARGINED_CMD_EDGE) && (CmdRight[PerChannelCmdPiGroup][Clk][EvenOddLoop] != UNMARGINED_CMD_EDGE)) {
            if ((ABS (CmdRight[PerChannelCmdPiGroup][Clk][EvenOddLoop] - CmdLeft[PerChannelCmdPiGroup][Clk][EvenOddLoop])) < MinimumMargin (Host, CA)) {
              RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "Eye width is too small: %d\n",
                ABS (CmdRight[PerChannelCmdPiGroup][Clk][EvenOddLoop] - CmdLeft[PerChannelCmdPiGroup][Clk][EvenOddLoop]));
              OutputWarning (WARN_CMD_CLK_TRAINING, 0, Socket, Ch, NO_DIMM, NO_RANK);
              DisableChannelSw (Host, Socket, Ch);
            }
          }
        } // CmdPiGroup loop
      } //subchannel loop
    } // Clk offset
  } //EvenOddLoop

  //
  // Calculate Cmd Pie Offset  per PI Group per CH
  //
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Calculate Cmd Pie Offset  per PI Group per CH \n");
  for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
    for (CmdPiGroup = 0; CmdPiGroup < MaxCmdCtlGroupSubCh; CmdPiGroup++) {
      for (EvenOddLoop = EVEN_SAMPLING; EvenOddLoop < MAX_SAMPLING_LOOP; EvenOddLoop++) {
        //
        // Track MinCmdOffset, MaxCmdOffset across all CK groups
        //
        MinCmdOffset = MAX_CMD_DELAY_DDR5;
        MaxCmdOffset = MIN_CMD_DELAY_DDR5;
        PerChannelCmdPiGroup = CmdPiGroup + MaxCmdCtlGroupSubCh * SubChannel;
        for (Clk = 0; Clk < MAX_CLK; Clk++) {
          if (CkEnabled[Clk]) {
            if ((CmdLeft[PerChannelCmdPiGroup][Clk][EvenOddLoop] != -UNMARGINED_CMD_EDGE) && (CmdRight[PerChannelCmdPiGroup][Clk][EvenOddLoop] != UNMARGINED_CMD_EDGE)) {
              UpdateEdges (CmdLeft[PerChannelCmdPiGroup][Clk][EvenOddLoop], CmdRight[PerChannelCmdPiGroup][Clk][EvenOddLoop],
                &CompCmdLeft[PerChannelCmdPiGroup][EvenOddLoop], &CompCmdRight[PerChannelCmdPiGroup][EvenOddLoop]);
              UpdateMinMaxInt (CmdLeft[PerChannelCmdPiGroup][Clk][EvenOddLoop], &MinCmdOffset, &MaxCmdOffset);
              UpdateMinMaxInt (CmdRight[PerChannelCmdPiGroup][Clk][EvenOddLoop], &MinCmdOffset, &MaxCmdOffset);

              RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubChannel, NO_RANK, NO_STROBE, NO_BIT,
                "CA PI Group %2d, clk %d: le = %3d re = %3d, Left = %3d Right = %3d\n",
                CmdPiGroup, Clk,
                CmdLeft[PerChannelCmdPiGroup][Clk][EvenOddLoop], CmdRight[PerChannelCmdPiGroup][Clk][EvenOddLoop],
                CompCmdLeft[PerChannelCmdPiGroup][EvenOddLoop], CompCmdRight[PerChannelCmdPiGroup][EvenOddLoop]);
            }
          }
        } // Clk loop
      } //EvenOddLoop

      EyeWidth[PerChannelCmdPiGroup][EVEN_SAMPLING] = ABS (CompCmdRight[PerChannelCmdPiGroup][EVEN_SAMPLING] - CompCmdLeft[PerChannelCmdPiGroup][EVEN_SAMPLING]);
      EyeWidth[PerChannelCmdPiGroup][ODD_SAMPLING] = ABS (CompCmdRight[PerChannelCmdPiGroup][ODD_SAMPLING] - CompCmdLeft[PerChannelCmdPiGroup][ODD_SAMPLING]);
      //
      // 1. CA duty cycle. Determine the required offset for CA PI (even cycle):
      // cmd_pie_offset = cmd_pie_offset_prev + (EWeven – EWodd)/2
      // Where:
      // cmd_pie_offset = new setting for CA even PI offset (cmd_pie_offset register)
      // cmd_pie_offset_prev = previous (default) setting for cmd_pie_offset register
      // EWeven = Composite total eye width across ranks for the even UI (ticks)
      // EWodd = Composite total eye width across ranks for the odd UI (ticks)
      //
      GetSetCmdPieOffset (Host, Socket, Ch, SubChannel, CmdPiGroup, (GSM_READ_ONLY | GSM_READ_CSR), &CmdPieOffsetPrev);
      CmdPieOffset = CmdPieOffsetPrev + (EyeWidth[PerChannelCmdPiGroup][EVEN_SAMPLING] - EyeWidth[PerChannelCmdPiGroup][ODD_SAMPLING]) / 2;
      //
      // Write the CMD Pie Offset
      //
      if ((MaxCmdOffset != MIN_CMD_DELAY_DDR5) && (MinCmdOffset != MAX_CMD_DELAY_DDR5)) {

        GetSetCmdPieOffset (Host, Socket, Ch, SubChannel, CmdPiGroup, (GSM_FORCE_WRITE | GSM_UPDATE_CACHE), &CmdPieOffset);

        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubChannel, NO_RANK, NO_STROBE, NO_BIT,
          "CMD Pi Group %2d: maxOffset = %d, minOffset = %d, CmdOffset = %d CmdOffsetPrev = %d EWEven = %d, EWOdd = %d\n",
          CmdPiGroup, MaxCmdOffset, MinCmdOffset, CmdPieOffset, CmdPieOffsetPrev,
          EyeWidth[PerChannelCmdPiGroup][EVEN_SAMPLING],
          EyeWidth[PerChannelCmdPiGroup][ODD_SAMPLING]
          );
      }
    } // CmdPiGroup loop
  }//sub channel loop

  //
  // 2.  CLK centering adjust. Determine the required offset for the clock based on the CA duty cycle adjustment.
  // This is to adjust the CLK delay (odd cycle, falling edge):
  // clk_dly = clk_dly_prev + (odd_right + odd_left) / 2 + (EWeven – EWodd)/4
  // Where:
  // clk_dly = new setting for clock delay (clk_dly register)
  // clk_dly_prev = previous (default) setting for clk_dly register
  // odd_right = absolute margin to the right edge (current rank) for the odd UI (ticks)
  // odd_left = absolute margin to the left edge (current rank) for the odd UI (ticks)
  // EWeven = Composite total eye width across ranks for the even UI (ticks)
  // EWodd = Composite total eye width across ranks for the odd UI (ticks)
  // Note: If expressing left margin as a negative number and right margin as a positive number, the formula needs to be adjusted by changing the sign of the asymmetry term:
  // clk_dly = clk_dly_prev + (odd_right + odd_left) / 2 + (EWeven – EWodd)/4
  //
  for (Clk = 0; Clk < MAX_CLK; Clk++) {
    if (CkEnabled[Clk] == 0) {
      continue;
    }

    GetSetClkDelay (Host, Socket, Ch, Clk, GSM_READ_CSR | GSM_READ_ONLY, &ClkDelayPrev[Clk]);

    for (EvenOddLoop = EVEN_SAMPLING; EvenOddLoop < MAX_SAMPLING_LOOP; EvenOddLoop++) {
      MinRightOffset[EvenOddLoop] = MIN_CMD_OFFSET_INIT;
      MaxLeftOffset[EvenOddLoop] = MAX_CMD_OFFSET_INIT;
      MinCmdOffset = MAX_CMD_DELAY_DDR5;
      MaxCmdOffset = MIN_CMD_DELAY_DDR5;

      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        for (CmdPiGroup = 0; CmdPiGroup < MaxCmdCtlGroupSubCh; CmdPiGroup++) {
          PerChannelCmdPiGroup = CmdPiGroup + MaxCmdCtlGroupSubCh * SubChannel;
          if ((CmdLeft[PerChannelCmdPiGroup][Clk][EvenOddLoop] != -UNMARGINED_CMD_EDGE) && (CmdRight[PerChannelCmdPiGroup][Clk][EvenOddLoop] != UNMARGINED_CMD_EDGE)) {
            UpdateMinMaxInt (CmdLeft[PerChannelCmdPiGroup][Clk][EvenOddLoop], &MinCmdOffset, &MaxLeftOffset[EvenOddLoop]);
            UpdateMinMaxInt (CmdRight[PerChannelCmdPiGroup][Clk][EvenOddLoop], &MinRightOffset[EvenOddLoop], &MaxCmdOffset);
          }
        } // CmdPiGroup loop
      }//subchannel loop
    } //EvenOddLoop

    CompositeEyeEven = MinRightOffset[EVEN_SAMPLING] - MaxLeftOffset[EVEN_SAMPLING];
    CompositeEyeOdd = MinRightOffset[ODD_SAMPLING] - MaxLeftOffset[ODD_SAMPLING];

    ClkDelay[Clk] = ClkDelayPrev[Clk];
    ClkDelay[Clk] += (MinRightOffset[ODD_SAMPLING] + MaxLeftOffset[ODD_SAMPLING]) / 2;
    ClkDelay[Clk] += (CompositeEyeEven - CompositeEyeOdd) / 4;

    //
    // Write the CLK delay.
    //
    GetSetClkDelay (Host, Socket, Ch, Clk, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &ClkDelay[Clk]);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "<----Clk %d ClkDelay %d: ClkDelayPrev:%d MinRightOffset[ODD]:%d MaxLeftOffset[ODD]:%d CompositeEyeEven=%d CompositeEyeOdd=%d\n",
      Clk, ClkDelay[Clk], ClkDelayPrev[Clk], MinRightOffset[ODD_SAMPLING], MaxLeftOffset[ODD_SAMPLING],
      CompositeEyeEven, CompositeEyeOdd
    );
    //
    // Write the CS delay
    //
    CtlMinVal = 255;
    CtlMaxVal = 0;
    Value = ClkDelay[Clk] - ClkDelayPrev[Clk];
    GetSetCtlGroupDelay (Host, Socket, Ch, CtlIndex[Clk] + CtlGrp0, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE, &Value, &CtlMinVal, &CtlMaxVal);
    //
    // 3.  CLK duty cycle. Determine the required offset for the CLK PI (even cycle):
    // clk_pie_offset = clk_pie_offset_prev + (even_right – even_left)/2 +
    //                         (EWeven – EWodd)/4 + (clk_delay_prev – clk_delay)
    // Where:
    // clk_pie_offset = new setting for clock even PI offset (clk_pie_offset register)
    // clk_pie_offset_prev = previous (default) setting for clk_pie_offset register
    // even_right = absolute margin to the right edge (current rank) for the even UI (ticks)
    // even_left = absolute margin to the left edge (current rank) for the even UI (ticks)
    // Note: If expressing left margin as a negative number and right margin as a positive number, the formula needs to be adjusted by changing the sign of the asymmetry term:
    // clk_pie_offset = clk_pie_offset_prev + (even_right + even_left)/2 +
    //                         (EWeven – EWodd)/4 + (clk_delay_prev – clk_delay)
    //
    GetSetClkPieOffset (Host, Socket, Ch, Clk, GSM_READ_ONLY | GSM_READ_CSR, &ClkPieOffsetPrev[Clk]);

    ClkPieOffset[Clk] = ClkPieOffsetPrev[Clk];
    //
    // Asymmetry for the even UI
    //
    ClkPieOffset[Clk] += ((MinRightOffset[EVEN_SAMPLING] + MaxLeftOffset[EVEN_SAMPLING]) / 2);
    //
    // Duty cycle between even and odd
    //
    ClkPieOffset[Clk] += ((CompositeEyeEven - CompositeEyeOdd) / 4);
    ClkPieOffset[Clk] += ClkDelayPrev[Clk] - ClkDelay[Clk];

    //
    // Write the CLK PIE Offset
    //
    GetSetClkPieOffset (Host, Socket, Ch, Clk, GSM_FORCE_WRITE + GSM_UPDATE_CACHE, &ClkPieOffset[Clk]);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "<----Clk %d ClkPieOffset %d: ClkPieOffsetPrev:%d MinRightOffset[EVEN]:%d MaxLeftOffset[EVEN]:%d CompositeEyeEven=%d CompositeEyeOdd=%d\n",
      Clk, ClkPieOffset[Clk], ClkPieOffsetPrev[Clk], MinRightOffset[EVEN_SAMPLING], MaxLeftOffset[EVEN_SAMPLING],
      CompositeEyeEven, CompositeEyeOdd
    );

  } // Clk loop
} // EvaluateCMDMarginsDutyCyleDdr5

/**
  Multi-use function to either get or set signal delays based on the provided group number

  @param Host:     Pointer to SYSHOST
  @param socket:   Socket number
  @param ch:       Channel number
  @param level:    IO level to access
  @param sig:      Enumerated signal name
  @param mode:     GSM_READ_CSR - Read the data from hardware and not cache
                   GSM_READ_ONLY - Do not write
                   GSM_WRITE_OFFSET - Write offset and not value
                   GSM_FORCE_WRITE - Force the write
  @param value:    Absolute value or offset selected by GSM_WRITE_OFFSET

  @retval MRC_STATUS

**/
MRC_STATUS
GetSetSignalDdr5 (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  MRC_LT    level,
  GSM_CSN   sig,
  UINT8     mode,
  INT16     *value
  )
{
  MRC_STATUS            status;
  UINT8                 RegisterIndex;
  UINT8                 SubCh;
  //
  // Get pi register index
  //
  status = SignalToRegIndex (Host, socket, ch, sig, &SubCh, &RegisterIndex);

  if (status == MRC_STATUS_SUCCESS) {
    status = GetSetCmdDelay (Host, socket, ch, SubCh, RegisterIndex, mode, value);
  }
  return status;
} // GetSetSignalDdr5

/**
  Evaluates the CMD margins and determines the appropriate offset

  @param [in    ] Host       - Pointer to Host structure
  @param [in    ] socket     - Processor socket within the system (0-based)
  @param [in    ] ch         - Memory channel number
  @param [in    ] ckEnabled  - Array of clocks enabled
  @param [in    ] ctlIndex - Array of control indexes
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
  UINT8 clk;
  UINT8 cmdPiGroup;
  INT16                 minCmdOffset;
  INT16                 maxCmdOffset;
  INT16                 cmdOffsetClk[MAX_CMDCTL_ARRAY_SPR_DDR4][MAX_CLK];
  INT16                 maxLeftOffset;
  UINT16                ctlMinVal;
  UINT16                ctlMaxVal;
  INT16                 minRightOffset;
  INT16                 ckOffset[MAX_CLK];
  UINT8                 earlyCmdClkDisable = 0;
  UINT8                 parityWorkaround = 0;
  struct ddrRank        (*rankList) [MAX_RANK_DIMM];
  struct dimmNvram      (*dimmNvList) [MAX_DIMM];
  UINT8                 MaxCmdGroupAll;
  UINT8                 CmdSetupOffset;
  SIGNAL_DESCRIPTOR     *CmdGroupAll;
  MRC_STATUS            status;
  SYS_SETUP             *Setup;

  Setup = GetSysSetupPointer ();

  status = GetCmdCtlGroupList (Host, CmdCtlAll, &MaxCmdGroupAll, &CmdGroupAll);
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
    for (cmdPiGroup = 0; cmdPiGroup < MaxCmdGroupAll; cmdPiGroup++) {

      //
      // Make sure the eye width is large enough
      //
#ifdef DDR5_SUPPORT
      if (IsDdr5Present (Host, socket)) {
        if ((cmdLeft[cmdPiGroup][clk] != -UNMARGINED_CMD_EDGE) && (cmdRight[cmdPiGroup][clk] != UNMARGINED_CMD_EDGE)) {
          if ((cmdRight[cmdPiGroup][clk] - cmdLeft[cmdPiGroup][clk]) < MinimumMargin (Host, CA)) {
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "Eye width is too small: %d\n",
                            cmdRight[cmdPiGroup][clk] - cmdLeft[cmdPiGroup][clk]);
            OutputWarning (WARN_CMD_CLK_TRAINING, 0, socket, ch, NO_DIMM, NO_RANK);
            DisableChannelSw (Host, socket, ch);
          }
        }
      } else {
#endif
        if ((cmdRight[cmdPiGroup][clk] - cmdLeft[cmdPiGroup][clk]) < MinimumMargin (Host, CA)) {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "Eye width is too small: %d\n", cmdRight[cmdPiGroup][clk] - cmdLeft[cmdPiGroup][clk]);

          // This is an inversion (or as close as possible) of "Get CK Index" in InitMem.c:SetRankIndexes.
          if (Host->nvram.mem.dimmTypePresent == RDIMM) {
            dimm = clk;
            rank = NO_RANK;
          } else {
            dimm = clk & 1;
            rank = clk >> 1;
          }

          OutputWarning (WARN_CMD_CLK_TRAINING, WARN_CMD_PI_GROUP_SMALL_EYE, socket, ch, dimm, rank);
          DisableChannelSw (Host, socket, ch);

        }
#ifdef DDR5_SUPPORT
      }
#endif
      if ((cmdLeft[cmdPiGroup][clk] != -UNMARGINED_CMD_EDGE) && (cmdRight[cmdPiGroup][clk] != UNMARGINED_CMD_EDGE)) {
        cmdOffsetClk[cmdPiGroup][clk] = (cmdLeft[cmdPiGroup][clk] + cmdRight[cmdPiGroup][clk]) / 2;

        UpdateMinMaxInt (cmdOffsetClk[cmdPiGroup][clk], &minCmdOffset, &maxCmdOffset);

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

  for (cmdPiGroup = 0; cmdPiGroup < MaxCmdGroupAll; cmdPiGroup++) {
    // Track minCgOffset, maxCgOffset across all CK groups
    minCmdOffset = 255;
    maxCmdOffset = -255;

    for (clk = 0; clk < MAX_CLK; clk++) {
      if (ckEnabled[clk]) {
        if ((cmdLeft[cmdPiGroup][clk] != -UNMARGINED_CMD_EDGE) && (cmdRight[cmdPiGroup][clk] != UNMARGINED_CMD_EDGE)) {
          //if(moveClk) {
          //  UpdateMinMaxInt(cmdPiShift[cmdPiGroup][clk], &minCmdOffset, &maxCmdOffset);
          //} else {
          UpdateMinMaxInt (cmdOffsetClk[cmdPiGroup][clk], &minCmdOffset, &maxCmdOffset);
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
    if ((maxCmdOffset != -255) && (minCmdOffset != 255)) {
      GetSetCmdDelay (Host, socket, ch, CmdGroupAll[cmdPiGroup].SubCh, CmdGroupAll[cmdPiGroup].RegIndex, (GSM_WRITE_OFFSET | GSM_UPDATE_CACHE), &cmdOffset[cmdPiGroup]);

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
  for (cmdPiGroup = 0; cmdPiGroup < MaxCmdGroupAll; cmdPiGroup++) {
    for (clk = 0; clk < MAX_CLK; clk++) {
      if (ckEnabled[clk] == 0) {
        continue;
      }
      if ((cmdLeft[cmdPiGroup][clk] != -UNMARGINED_CMD_EDGE) && (cmdRight[cmdPiGroup][clk] != UNMARGINED_CMD_EDGE)) {
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

    minRightOffset = 255;
    maxLeftOffset = -255;

    for (cmdPiGroup = 0; cmdPiGroup < MaxCmdGroupAll; cmdPiGroup++) {
      if ((cmdLeft[cmdPiGroup][clk] != -UNMARGINED_CMD_EDGE) && (cmdRight[cmdPiGroup][clk] != UNMARGINED_CMD_EDGE)) {
        UpdateMinMaxInt (cmdLeft[cmdPiGroup][clk], &minCmdOffset, &maxLeftOffset);
        UpdateMinMaxInt (cmdRight[cmdPiGroup][clk], &minRightOffset, &maxCmdOffset);
      }
    } // cmdPiGroup loop

    ckOffset[clk] = - (minRightOffset + maxLeftOffset) / 2;

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
        rankList = GetRankNvList (Host, socket, ch, dimm);
        for (rank = 0; rank < (*dimmNvList) [dimm].numRanks; rank++) {
          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          if (clk == (*rankList) [rank].ckIndex) {
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
} // EvaluateCMDMargins

/**
  Multi-use function to either get or set command tx eq

  @param[in]      Host    - Pointer to SYSHOST
  @param[in]      Socket  - Socket number
  @param[in]      Ch      - Channel number
  @param[in]      Mode    - GSM_READ_CSR - Read the data from hardware and not cache
                            GSM_READ_ONLY - Do not write
                            GSM_WRITE_OFFSET - Write offset and not value
                            GSM_FORCE_WRITE - Force the write
  @param[in,out]  Value   - Value or offset to program. Actual value returned to caller.

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
#if 0 //SPR_TODO_DDRIO. Don't find ICX DdrCrCmdNControls in SPR?
  EFI_STATUS                                            Status;
  INT16                                                 CurVal;
  INT16                                                 CmdTapCoef;
  UINT16                                                MaxLimit = 0;
  UINT16                                                MinLimit = 0;
  INT16                                                 Tmp;
  DDRIO_CHANNEL_NVDATA                                  (*DdrioChannelNvList) [MAX_CH];
  DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_STRUCT            DdrCrCmdNControls;
  DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_STRUCT            DdrCrCmdSControls;
  DDRCRCMDCONTROLS_CKE_MCIO_DDRIOEXT_STRUCT             DdrCrCkeControls;
  DDRCRCMDCONTROLS_CTL_MCIO_DDRIOEXT_STRUCT             DdrCrCtlControls;

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

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
    DdrCrCmdNControls.Data = (*DdrioChannelNvList) [Ch].DdrCrCmdNControls;
    DdrCrCmdSControls.Data = (*DdrioChannelNvList) [Ch].DdrCrCmdSControls;
    DdrCrCkeControls.Data = (*DdrioChannelNvList) [Ch].DdrCrCkeControls;
    DdrCrCtlControls.Data = (*DdrioChannelNvList) [Ch].DdrCrCtlControls;
  }

  //
  // Get the current setting
  // There are 4 instances of the register per channel but they will all be set to the same value.
  //
  CurVal = (INT16) (DdrCrCmdNControls.Bits.cmdtapcoef);

  // Read only?
  if (Mode & GSM_READ_ONLY) {
    *Value = (INT16) CurVal;
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
    if (((*DdrioChannelNvList) [Ch].DdrCrCmdNControls != DdrCrCmdNControls.Data) || (Mode & GSM_FORCE_WRITE)) {
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
    (*DdrioChannelNvList) [Ch].DdrCrCmdNControls = DdrCrCmdNControls.Data;
    (*DdrioChannelNvList) [Ch].DdrCrCmdSControls = DdrCrCmdSControls.Data;
    (*DdrioChannelNvList) [Ch].DdrCrCkeControls = DdrCrCkeControls.Data;
    (*DdrioChannelNvList) [Ch].DdrCrCtlControls = DdrCrCtlControls.Data;
  }
#endif //if 0
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
  UINT8               MaxSignals;
  UINT8               MaxSignalA;
  UINT8               MaxSignalB;
  PinRegister         *Signals;
  PinRegister         *PinRegisterSprSaP = NULL;
  PinRegister         *PinRegisterSprSbP = NULL;
  struct channelNvram (*channelNvList) [MAX_CH];
  UINT8 MaxChDdr;

  channelNvList = GetChannelNvList (Host, Socket);
  MaxChDdr = GetMaxChDdr ();

  RcDebugPrint (SDBG_MEM_TRAIN, "\nSTART_DATA_DCA_DCS\n");

  //
  // Get the Max signals
  //

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList) [ch].enabled == 0) {
      continue;
    }

    GetPinRegisterValue (Host, Socket, ch, SUB_CH_A, &PinRegisterSprSaP, &MaxSignalA);
    if (IsDdr5Present (Host, Socket)) {
      PinRegisterSprSbP = PinRegisterSprSbDisplay;
      MaxSignalB = sizeof (PinRegisterSprSbDisplay) / sizeof (PinRegister);
    } else {
      GetPinRegisterValue (Host, Socket, ch, SUB_CH_B, &PinRegisterSprSbP, &MaxSignalB);
    }

    MaxSignals = MaxSignalA > MaxSignalB ? MaxSignalA : MaxSignalB;

  }



  //
  // Now print the table
  //

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList) [ch].enabled == 0) {
      continue;
    }

    //
    // Print the table header based on the max signals
    //
    if (ch == 0) {
      RcDebugPrint (SDBG_MEM_TRAIN,
        "SIGNAL:     ");
      for (index = 0; index < MaxSignals; index++) {
        RcDebugPrint (SDBG_MEM_TRAIN,
          "  %3d ", index);
      } // index loop

      RcDebugPrint (SDBG_MEM_TRAIN, "\n");
    }

    // Cmd ctl SPR SIDEA
    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, ch, NO_DIMM, SUB_CH_A, NO_RANK, NO_STROBE, NO_BIT, "");
    Signals = PinRegisterSprSaP;
    for (index = 0; index < MaxSignals; index++) {
      delay = 0;
      GetSetSignalDdr5 (Host, Socket, ch, DdrLevel, Signals[index].Signal, GSM_READ_ONLY, &delay);
      RcDebugPrint (SDBG_MEM_TRAIN, "  %3d ", delay);
    } // CmdCtlSideA loop

    //
    //  Display Command control delay of SPR Sub Channel B.
    //
    RcDebugPrint (SDBG_MEM_TRAIN, "\n");
    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, ch, NO_DIMM, SUB_CH_B, NO_RANK, NO_STROBE, NO_BIT, "");
    Signals = PinRegisterSprSbP;
    for (index = 0; index < MaxSignals; index++) {
      delay = 0;
      if (Signals[index].Signal == NC) {
        RcDebugPrint (SDBG_MEM_TRAIN, "   %a ", "NC");
      } else {
        GetSetSignalDdr5 (Host, Socket, ch, DdrLevel, Signals[index].Signal, GSM_READ_ONLY, &delay);
        RcDebugPrint (SDBG_MEM_TRAIN, "  %3d ", delay);
      }
    } // CmdCtlSideB loop

    RcDebugPrint (SDBG_MEM_TRAIN, "\n");

  } // ch loop

  RcDebugPrint (SDBG_MEM_TRAIN,
    "STOP_DATA_DCA_DCS\n");

  RcDebugPrint (SDBG_MEM_TRAIN, "\nSTART_DATA_DCK\n");

  RcDebugPrint (SDBG_MEM_TRAIN, "SIGNAL: ");
  for (index = 0; index < 4; index++) {
    RcDebugPrint (SDBG_MEM_TRAIN, "  %3d ", index);
  } // index loop

  RcDebugPrint (SDBG_MEM_TRAIN, "\n");

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList) [ch].enabled == 0) {
      continue;
    }

    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "");
    for (ioGroup = 0; ioGroup < 4; ioGroup++) {

      GetSetClkDelay (Host, Socket, ch, ioGroup, GSM_READ_ONLY, &delay);
      RcDebugPrint (SDBG_MEM_TRAIN, "  %3d ", delay);
    } // ioGroup loop

    RcDebugPrint (SDBG_MEM_TRAIN, "\n");
  } // ch loop

  RcDebugPrint (SDBG_MEM_TRAIN, "STOP_DATA_DCK\n");
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
  Multi-use function to either get or set control delays based on the provided group number

  @param[in] Host     Pointer to SYSHOST
  @param[in] Socket   Socket number
  @param[in] Ch       Channel number
  @param[in] Clk      Clock number
  @param[in] Mode     GSM_READ_CSR - Read the data from hardware and not cache
                      GSM_READ_ONLY - Do not write
                      GSM_WRITE_OFFSET - Write offset and not value
                      GSM_FORCE_WRITE - Force the write
  @param[in] Value    Value to program

  @retval MRC_STATUS

**/
MRC_STATUS
GetSetClkPieOffset (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Clk,
  IN UINT8    Mode,
  IN INT16    *Value
  )
{
  UINT8                                 PiVal;
  INT16                                 Tmp;
  UINT16                                PiDelay;
  UINT32                                ClkCsrOffset;
  DDRIO_CHANNEL_NVDATA                  (*DdrioChannelNvList) [MAX_CH];

  DDRCLK_CTL0_PAIR_0_MCIO_DDRIOEXT_STRUCT   ClkCsr;

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  ClkCsrOffset = GetClkRegOffset (Clk);
  //
  // Get register data
  //
  if (Mode & GSM_READ_CSR) {
    ClkCsr.Data = MemReadPciCfgEp (Socket, Ch, ClkCsrOffset);
  } else {
    ClkCsr.Data = (*DdrioChannelNvList) [Ch].ClkCsr[Clk];
  }

  PiVal = (UINT8) ClkCsr.Bits.clk_pie_offset;

  // Read only?
  if (Mode & GSM_READ_ONLY) {
    *Value = (INT16) PiVal;
  }
  // Handle writes
  else {
    //
    // Adjust the current CLK PIE offset value by offset
    //
    if (Mode & GSM_WRITE_OFFSET) {
      // Get the new offset (note: can be negative)
      Tmp = PiVal + *Value;
    } else {
      // FORCE_WRITE
      Tmp = *Value;
    }
    // Check if we are below 0
    if (Tmp >= 0) {
      PiDelay = (UINT8) Tmp;
    } else {
      PiDelay = 128 + Tmp;
    }
    if (PiDelay > MAX_CLK_PIE_OFFSET) {
      PiDelay = Tmp - MAX_CLK_PIE_OFFSET;
    }

    ClkCsr.Bits.clk_pie_offset = PiDelay;

    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
    if (((*DdrioChannelNvList) [Ch].ClkCsr[Clk] != ClkCsr.Data) || (Mode & GSM_FORCE_WRITE)) {

      if (IsDcpmmPresentDimm (Socket, Ch, DdrtClkToDimmIndex (Clk))) {

        DdrtClockMovementWorker (Host, Socket, Ch, ClkCsrOffset, ClkCsr.Data, PiVal, PiDelay, CLK_MOVE_PIE_OFFSET);

      } else {

        MemWritePciCfgEp (Socket, Ch, ClkCsrOffset, ClkCsr.Data);
      }
    }
  }

  //
  // Save to cache
  //
  if (Mode & GSM_UPDATE_CACHE) {
    (*DdrioChannelNvList) [Ch].ClkCsr[Clk] = ClkCsr.Data;
  }

  return MRC_STATUS_SUCCESS;
} // GetSetClkPieOffset
