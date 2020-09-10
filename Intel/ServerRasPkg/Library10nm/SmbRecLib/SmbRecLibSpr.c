/** @file SmbRecLib.c
  Smbus Recovery support

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2020 Intel Corporation. <BR>

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

#include <Library/RasDebugLib.h>
#include <Library/SmbusRecLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ProcSmbIpRasLib.h>
#include <Memory/ProcSmbChipCommon.h>

SMBUS_REC_DATA_STRUCT                    mSmbusRecData[MAX_SOCKET][MAX_SMB_INSTANCE];

/**
   Save register state

  @param [IN]   Socket  --  socket index
  @param [IN]   SbPortId - Sideband Port Id of I3C

  @retval  None
**/
VOID
SaveTSODPolling (
  IN  UINT8             Socket,
  IN  UINT32            SbPortId
  )
{
  UINT16  TsodCmd;
  UINT8   Inst = GetI3cInstFromSbPortId (SbPortId);

  if (mSmbusRecData[Socket][Inst].Bits.SbPortId == 0) {
    // Save register state and disable TSOD polling
    TsodCmd = ReadTsodCmd (Socket, Inst);
    mSmbusRecData[Socket][Inst].Bits.periodic_poll_cmd_en = TsodCmd;
    mSmbusRecData[Socket][Inst].Bits.SbPortId = SbPortId;
  }
}

/**
  Restore the state of the SMBUS that was saved before doing the recovery.

  @param [IN]   Socket  --  socket index
  @param [IN]   SbPortId - Sideband Port Id of I3C

  @retval  None
**/
VOID
RestoreTsodPolling (
  IN  UINT8             Socket,
  IN  UINT32            SbPortId
  )
{
  UINT16  TsodCmd;
  UINT8   Inst = GetI3cInstFromSbPortId (SbPortId);

  //
  // Restore the state of the SMBUS that was saved before doing the recovery
  //
  if (mSmbusRecData[Socket][Inst].Bits.SbPortId == SbPortId) {
    TsodCmd = (UINT16) mSmbusRecData[Socket][Inst].Bits.periodic_poll_cmd_en;
    // mSmbusRecData[i][j].Data = 0;
    WriteTsodCmd (Socket, Inst, TsodCmd);
  }
}

/**
   Clear register state

  @retval  None
**/
VOID
ClearTsodPolling (
    VOID
  )
{
  ZeroMem (&mSmbusRecData, sizeof(mSmbusRecData));
}
/**
  Initialize reset completion status trigger by setting following register.
       INTR_STATUS_ENABLE.BUS_RESET_DONE_STATE_EN (BIT15)
       INTR_SIGNAL_ENABLE. BUS_RESET_DONE_STATE_EN (BIT15)

  @param [IN]   Socket  --  socket index
  @param [IN]   SbPortId - Sideband Port Id of I3C

  @retval  None
**/
VOID
InitializeResetCompletionStatus (
  IN  UINT8             Socket,
  IN  UINT32            SbPortId
  )
{
  SetI3cStatusResetDone(Socket, GetI3cInstFromSbPortId (SbPortId));
  SetI3cSignalResetDone(Socket, GetI3cInstFromSbPortId (SbPortId));
}

/**
  Clearing reset trigger register.
       INTR_STATUS_ENABLE.BUS_RESET_DONE_STATE_EN (BIT15)
       INTR_SIGNAL_ENABLE. BUS_RESET_DONE_STATE_EN (BIT15)

  @param [IN]   Socket  --  socket index
  @param [IN]   SbPortId - Sideband Port Id of I3C

  @retval  None
**/
VOID
ClearResetDoneState (
  IN  UINT8             Socket,
  IN  UINT32            SbPortId
  )
{
  ClearI3cStatusResetDone(Socket, GetI3cInstFromSbPortId (SbPortId));
  ClearI3cSignalResetDone(Socket, GetI3cInstFromSbPortId (SbPortId));
}

/**
  This function is for Smbus Recovery initialize flow.

  @param [IN]   Socket      --  Socket index
  @param [IN]   SbPortId    --  Side Band Id for I3C

  @retval  None
**/
VOID
EFIAPI
SmbusRecoveryConfig (
  IN  UINT8         Socket,
  IN  UINT32        SbPortId
  )
{
  SetIntrStatusErrorEn(Socket, GetI3cInstFromSbPortId (SbPortId));
  SetIntrSignalErrorEn(Socket, GetI3cInstFromSbPortId (SbPortId));
}

/**
  This function is the entry point of the Smbus recovery SMM handler

  @param [IN]   Socket      --  Socket index
  @param [IN]   SbPortId    --  Side Band Id for I3C

  @retval  None

**/
VOID
EFIAPI
SmbusRecoverySmiHandler (
  IN  UINT8         Socket,
  IN  UINT32        SbPortId
  )
{
  UINT8   Inst = GetI3cInstFromSbPortId (SbPortId);

  //
  // Check for Smbus Error and if any prepare for its recovery
  //
  if (IsSmbusError (Socket, Inst)) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "Smbus Error is present on SktId:0x%x, SegmentId:0x%x\n", Socket, SbPortId));
    // Handle error.
    SaveTSODPolling (Socket, SbPortId);
    InitializeResetCompletionStatus (Socket, SbPortId);
    // send bus reset.
    I3cReset (Socket, Inst);
  }
  // When bus reset complete, it will trigger interrupt again.
  if (IsSmbusResetDone (Socket, Inst)) {
    ClearResetDoneState (Socket, SbPortId);
    // Restore TSOD polling configuration.
    RestoreTsodPolling (Socket, SbPortId);
    ClearTsodPolling ();
  }
}

/**

  The constructor function initialize SmbRec library.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
InitSmbRecLib (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
{
  ZeroMem (&mSmbusRecData, sizeof(mSmbusRecData));
  return EFI_SUCCESS;
}
