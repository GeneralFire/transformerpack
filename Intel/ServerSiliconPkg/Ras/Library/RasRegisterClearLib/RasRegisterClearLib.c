/** @file
  Implementation of RAS Register clear library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include <Library/RasGlobalDataLib.h>
#include <Library/MemMcIpRasLib.h>
#include <UncoreCommonIncludes.h>
#include <RcRegs.h>
#include <Library/UsraCsrLib.h>
#include <Library/KtiApi.h>

typedef struct{
  UINT8 PositionRecord[MAX_SOCKET][MAX_CH];
} POSITION_RECORD_STRUCT;

POSITION_RECORD_STRUCT *mPosionRecordData;

/**

  Record the position skt/ch on which need to clear the retry register.

  @param[in]  Skt             Socket index.
  @param[in]  ChOnSkt         Channel on socket.

**/
VOID
EFIAPI
RecordPositionToClearRetryRegister (
  IN    UINT8      Skt,
  IN    UINT8      ChOnSkt
  )
{
  mPosionRecordData -> PositionRecord[Skt][ChOnSkt] = 1;
}

/**
  Clear the Error logged Retry register

  @param  None

  @retval None
**/
VOID
EFIAPI
ClearRecordedRetryRegister ()
{
  UINT8                                     Skt;
  UINT8                                     ChOnSkt;

  for (Skt = 0; Skt < MAX_SOCKET; Skt++) {
    for (ChOnSkt = 0; ChOnSkt < MAX_CH; ChOnSkt++) {
      if (mPosionRecordData -> PositionRecord[Skt][ChOnSkt] == 1) {
        ClearRetryRegister (Skt, ChOnSkt);
        mPosionRecordData -> PositionRecord[Skt][ChOnSkt] = 0;
      }
    }
  }
}

/**
  Clear UBOX SMI source sideBand Portid.

  @param  none

  @retval none
**/
VOID
EFIAPI
ClearUboxSmiSrcSideBandPortId (
  )
{
#if !defined(SKX_HOST)
  UINT8                                  Socket;
  NCEVENTS_CR_SMISRCCLR_UBOX_CFG_STRUCT  UboxSmiSrcClrReg;
  NCEVENTS_CR_SMISRCLOG_UBOX_CFG_STRUCT  UboxSmiSrcLogReg;

  for (Socket = 0; Socket < MAX_SOCKET; Socket ++) {
    if (!SocketPresent(Socket)) {
      continue;
    }
    UboxSmiSrcLogReg.Data = UsraCsrRead (Socket, 0, NCEVENTS_CR_SMISRCLOG_UBOX_CFG_REG);
    if (UboxSmiSrcLogReg.Bits.sidebandvalid == 1) {
      UboxSmiSrcClrReg.Data   = UsraCsrRead (Socket, 0, NCEVENTS_CR_SMISRCCLR_UBOX_CFG_REG);
      UboxSmiSrcClrReg.Bits.sidebandvalidclr = 1;
      UsraCsrWrite (Socket, 0, NCEVENTS_CR_SMISRCCLR_UBOX_CFG_REG, UboxSmiSrcClrReg.Data);
    }
  }
#endif // #if !defined(SKX_HOST)
}

/**
  Clear ras register

  @param  None

  @retval None
**/
VOID
EFIAPI
ClearRasRegister ()
{
  ClearRecordedRetryRegister ();
  ClearUboxSmiSrcSideBandPortId ();
}

/**
  The constructor function for the library

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
InitializeRasRegisterClearLib (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
{
  BOOLEAN                NewAllocation;

  mPosionRecordData = GetRasGlobalData (L"SocketAndChannelPositionRecord", sizeof (POSITION_RECORD_STRUCT), &NewAllocation);
  ASSERT (mPosionRecordData != NULL);

  return EFI_SUCCESS;
}
