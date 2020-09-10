/** @file ProcSmbIpRasLibSmmSpr.c
  Ras library to access I3C

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

#include <Library/ProcSmbIpLib/Spr/Include/ProcSmbRegs.h>
#include <Library/ProcSmbIpRasLib.h>
#include <Library/CpuCsrAccessLib.h>
#include <Memory/ProcSmbChipCommon.h>

UINT32  I3cIntrStatusReg[MAX_SMB_INSTANCE] = { INTR_STATUS_SB_I3C0_REG, INTR_STATUS_SB_I3C1_REG };
UINT32  I3cStatusEnReg[MAX_SMB_INSTANCE] = { INTR_STATUS_ENABLE_SB_I3C0_REG, INTR_STATUS_ENABLE_SB_I3C1_REG };
UINT32  I3cSignalEnReg[MAX_SMB_INSTANCE] = { INTR_SIGNAL_ENABLE_SB_I3C0_REG, INTR_SIGNAL_ENABLE_SB_I3C1_REG };
UINT32  I3cResetReg[MAX_SMB_INSTANCE] = { RESET_CTRL_SB_I3C0_REG, RESET_CTRL_SB_I3C1_REG };
UINT32  TsodCmdReg[MAX_SMB_INSTANCE] = { PERIODIC_POLL_COMMAND_ENABLE_SB_I3C0_REG, PERIODIC_POLL_COMMAND_ENABLE_SB_I3C1_REG };

/**
  Convert I3C port id to instance id
  SbPortId 466,467 map to Instance id 0,1.

  @param [IN]   SbPortId    --  Sb port Id for I3C

  @retval  Instance Id
**/
UINT8
GetI3cInstFromSbPortId (
  IN  UINT32         SbPortId
  )
{
  return (UINT8) (SbPortId - 466);
}

/**
  Read INTR_STATUS_ENABLE_SB_I3C0_REG

  @param [IN]   Socket      --  Socket index
  @param [IN]   Inst    --  Instance Id for I3C

  @retval  Value of register
**/
UINT32
ReadIntrStatusEn (
  IN  UINT8         Socket,
  IN  UINT8         Inst
  )
{
  return ReadCpuCsr (Socket, Inst, I3cStatusEnReg[Inst]);
}

/**
  Read INTR_SIGNAL_ENABLE_SB_I3C0_REG

  @param [IN]   Socket      --  Socket index
  @param [IN]   Inst    --  Instance Id for I3C

  @retval  Value of register
**/
UINT32
ReadIntrSignalEn (
  IN  UINT8         Socket,
  IN  UINT8         Inst
  )
{
  return ReadCpuCsr (Socket, Inst, I3cSignalEnReg[Inst]);
}

/**
  Write INTR_STATUS_ENABLE_SB_I3C0_REG

  @param [IN]   Socket      --  Socket index
  @param [IN]   Inst    --  Instance Id for I3C
  @param [IN]   Inst    --  Data to write

  @retval  None
**/
VOID
WriteIntrStatusEn (
  IN  UINT8         Socket,
  IN  UINT8         Inst,
  IN  UINT32        Data
  )
{
  WriteCpuCsr (Socket, Inst, I3cStatusEnReg[Inst], Data);
}

/**
  Write INTR_SIGNAL_ENABLE_SB_I3C0_REG

  @param [IN]   Socket      --  Socket index
  @param [IN]   Inst    --  Instance Id for I3C
  @param [IN]   Inst    --  Data to write

  @retval  None
**/
VOID
WriteIntrSignalEn (
  IN  UINT8         Socket,
  IN  UINT8         Inst,
  IN  UINT32        Data
  )
{
  WriteCpuCsr (Socket, Inst, I3cSignalEnReg[Inst], Data);
}

/**
  Enable error for I3C status enable register.

  @param [IN]   Socket      --  Socket index
  @param [IN]   Inst    --  Instance Id for I3C

  @retval  None
**/
VOID
SetIntrStatusErrorEn (
  IN  UINT8         Socket,
  IN  UINT8         Inst
  )
{
  INTR_STATUS_ENABLE_SB_I3C0_STRUCT            IntrStatusEn;

  IntrStatusEn.Data = ReadIntrStatusEn (Socket, Inst);
  IntrStatusEn.Bits.transfer_abort_stat_en = 1;
  IntrStatusEn.Bits.transfer_err_stat_en = 1;
  IntrStatusEn.Bits.hc_internal_err_stat_en = 1;
  IntrStatusEn.Bits.ppoll_cmd_miss_stat_en = 1;

  WriteIntrStatusEn (Socket, Inst, IntrStatusEn.Data);
}

/**
  Enable error for I3C signal enable register.

  @param [IN]   Socket      --  Socket index
  @param [IN]   Inst    --  Instance Id for I3C

  @retval  None
**/
VOID
SetIntrSignalErrorEn (
  IN  UINT8         Socket,
  IN  UINT8         Inst
  )
{
  INTR_SIGNAL_ENABLE_SB_I3C0_STRUCT            IntrSignalEn;

  IntrSignalEn.Data = ReadIntrSignalEn (Socket, Inst);
  IntrSignalEn.Bits.transfer_abort_signal_en = 1;
  IntrSignalEn.Bits.transfer_err_signal_en = 1;
  IntrSignalEn.Bits.hc_internal_err_signal_en = 1;
  IntrSignalEn.Bits.ppoll_cmd_miss_signal_en = 1;

  WriteIntrSignalEn (Socket, Inst, IntrSignalEn.Data);
}

/**
  This function will check if the error happened on SMBUS.

  @param [IN]   Socket  --  socket index
  @param [IN]   Inst  -- Instance Id of I3C

  @retval FALSE: If no error happened
  @retval TRUE:  If there was an error
**/
BOOLEAN
IsSmbusError (
  IN  UINT8             Socket,
  IN  UINT8             Inst
  )
{
  INTR_STATUS_SB_I3C0_STRUCT            I3cIntrStatus;

  I3cIntrStatus.Data = ReadCpuCsr (Socket, Inst, I3cIntrStatusReg[Inst]);
  if ((I3cIntrStatus.Bits.transfer_abort_stat == 1) || (I3cIntrStatus.Bits.transfer_err_stat == 1) ||
    (I3cIntrStatus.Bits.hc_internal_err_stat == 1) || (I3cIntrStatus.Bits.ppoll_cmd_miss_stat == 1)) {
    return TRUE;
  }

  return FALSE;
}

/**
  This function set bus reset done status.

  @param [IN]   Socket  --  socket index
  @param [IN]   Inst  -- Instance Id of I3C

  @retval None
**/
VOID
SetI3cStatusResetDone (
  IN  UINT8             Socket,
  IN  UINT8             Inst
  )
{
  INTR_STATUS_ENABLE_SB_I3C0_STRUCT            IntrStatusEn;

  IntrStatusEn.Data = ReadIntrStatusEn(Socket, Inst);
  IntrStatusEn.Bits.bus_reset_done_stat_en = 1;
  WriteIntrStatusEn(Socket, Inst, IntrStatusEn.Data);
}

/**
  This function set bus reset done signal.

  @param [IN]   Socket  --  socket index
  @param [IN]   Inst  -- Instance Id of I3C

  @retval None
**/
VOID
SetI3cSignalResetDone (
  IN  UINT8             Socket,
  IN  UINT8             Inst
  )
{
  INTR_SIGNAL_ENABLE_SB_I3C0_STRUCT            IntrSignalEn;

  IntrSignalEn.Data = ReadIntrSignalEn (Socket, Inst);
  IntrSignalEn.Bits.bus_reset_done_signal_en = 1;
  WriteIntrSignalEn (Socket, Inst, IntrSignalEn.Data);
}

/**
  This function clear bus reset done status.

  @param [IN]   Socket  --  socket index
  @param [IN]   Inst  -- Instance Id of I3C

  @retval None
**/
VOID
ClearI3cStatusResetDone (
  IN  UINT8             Socket,
  IN  UINT8             Inst
  )
{
  INTR_STATUS_ENABLE_SB_I3C0_STRUCT            IntrStatusEn;

  IntrStatusEn.Data = ReadIntrStatusEn(Socket, Inst);
  IntrStatusEn.Bits.bus_reset_done_stat_en = 0;
  WriteIntrStatusEn(Socket, Inst, IntrStatusEn.Data);
}

/**
  This function clear bus reset done signal.

  @param [IN]   Socket  --  socket index
  @param [IN]   Inst  -- Instance Id of I3C

  @retval None
**/
VOID
ClearI3cSignalResetDone (
  IN  UINT8             Socket,
  IN  UINT8             Inst
  )
{
  INTR_SIGNAL_ENABLE_SB_I3C0_STRUCT            IntrSignalEn;

  IntrSignalEn.Data = ReadIntrSignalEn (Socket, Inst);
  IntrSignalEn.Bits.bus_reset_done_signal_en = 0;
  WriteIntrSignalEn (Socket, Inst, IntrSignalEn.Data);
}

/**
  Send bus reset.
  Set RESET_CTRL_SB.BUS_RESET_TYPE = 011b,
       AND RESET_CTRL_SB.BUS_RESET = 1.

  @param [IN]   Socket  --  socket index
  @param [IN]   Inst - Instance Id of I3C

  @retval  None
**/
VOID
I3cReset (
  IN  UINT8             Socket,
  IN  UINT8             Inst
  )
{
  RESET_CTRL_SB_I3C0_STRUCT            ResetCtrl;

  //reset the sts and fifo
  ResetCtrl.Data = ReadCpuCsr (Socket, Inst, I3cResetReg[Inst]);
  ResetCtrl.Bits.bus_reset = I3C_ENABLE;
  ResetCtrl.Bits.bus_reset_type |= (BIT30 + BIT29);

  WriteCpuCsr (Socket, Inst, I3cResetReg[Inst], ResetCtrl.Data);
}

/**
   Read register

  @param [IN]   Socket  --  socket index
  @param [IN]   Inst - Instance Id of I3C

  @retval  Data of periodic_poll_cmd_en
**/
UINT16
ReadTsodCmd (
  IN  UINT8             Socket,
  IN  UINT8             Inst
  )
{
  PERIODIC_POLL_COMMAND_ENABLE_SB_I3C0_STRUCT TsodCmd;

  TsodCmd.Data = ReadCpuCsr (Socket, Inst, TsodCmdReg[Inst]);
  return (UINT16) TsodCmd.Bits.periodic_poll_cmd_en;
}

/**
  Write Tsod

  @param [IN]   Socket  --  socket index
  @param [IN]   Inst - Instance Id of I3C
  @param [IN]   Data - Data to write to periodic_poll_cmd_en

  @retval  None
**/
VOID
WriteTsodCmd (
  IN  UINT8             Socket,
  IN  UINT8             Inst,
  IN  UINT16            Data
  )
{
  PERIODIC_POLL_COMMAND_ENABLE_SB_I3C0_STRUCT TsodCmd;

  TsodCmd.Data = 0;
  TsodCmd.Bits.periodic_poll_cmd_en = Data;
  WriteCpuCsr (Socket, Inst, TsodCmdReg[Inst], TsodCmd.Data);
}

/**
  Check Bus reset complete.

  @param [IN]   Socket  --  socket index
  @param [IN]   Inst - Instance Id of I3C

  @retval FALSE: If reset not done.
  @retval TRUE:  Bus reset is done.
**/
BOOLEAN
IsSmbusResetDone (
  IN  UINT8             Socket,
  IN  UINT8             Inst
  )
{
  INTR_STATUS_SB_I3C0_STRUCT            I3cIntrStatus;

  I3cIntrStatus.Data = ReadCpuCsr (Socket, Inst, I3cIntrStatusReg[Inst]);
  if ((I3cIntrStatus.Bits.bus_reset_done_stat == 1)) {
    return TRUE;
  }

  return FALSE;
}
