/** @file
  S3M IP library.

  This file implements S3M MailBox functions related to status polling.

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

//
// Statements that include other files
//
#include "Include/S3mMailBox.h"
#include "S3mIpRegisters.h"
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/EmulationConfigurationLib.h>

CONST CHAR8 LABEL_C_S3M_MAILBOX[] = "%aLABEL_c_S3mMailbox%d%a";
CONST CHAR8 LABEL_W_S3M_MAILBOX[] = "%aLABEL_w_S3mMailbox%d%a";

/**
  Generate assembly code for emulation to poll S3M Mailbox Flow Status.

  @param[in]  SocketId          Socket ID.
  @param[in]  StatusEncoding    The encoding of the MailBox flow status to polling

  @retval     VOID
**/
VOID
EFIAPI
UbiosGenerateForPollS3mMailboxFlowStatus (
  IN    UINT8    SocketId,
  IN    UINT8    StatusEncoding
  )
{
  UINT32        LabelNumber;
  UINT32        RegAddr;
  KTI_HOST_OUT  *KtiHostOutPtr;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;

  if (UbiosGenerationEnabled()) {
    if (GetUbiosLoopbackLabelNumber (&LabelNumber) != EFI_SUCCESS) {
      return;
    }

    RegAddr = (UINT32) UsraGetCsrRegisterAddress (SocketId, 0, S3M_MAILBOX_FLOW_STATUS_N0_S3M_TREG_REG);

    if (KtiHostOutPtr->CurrentReset == POST_RESET_POWERGOOD) {
      DEBUG ((EFI_D_ERROR, LABEL_C_S3M_MAILBOX, "\n", LabelNumber, ":\n"));
    } else {
      DEBUG ((EFI_D_ERROR, LABEL_W_S3M_MAILBOX, "\n", LabelNumber, ":\n"));
    }

    DEBUG ((EFI_D_ERROR, "  mov eax, DWORD PTR ds:[0%08xh]\n", RegAddr));
    DEBUG ((EFI_D_ERROR, "  shr eax, 1\n"));
    DEBUG ((EFI_D_ERROR, "  and eax, 00000007h\n"));
    DEBUG ((EFI_D_ERROR, "  cmp eax, 0%08xh\n", StatusEncoding));

    if (KtiHostOutPtr->CurrentReset == POST_RESET_POWERGOOD) {
      DEBUG ((EFI_D_ERROR, LABEL_C_S3M_MAILBOX, "  jnz ", LabelNumber, "\n"));
    } else {
      DEBUG ((EFI_D_ERROR, LABEL_W_S3M_MAILBOX, "  jnz ", LabelNumber, "\n"));
    }

    SetUbiosLoopbackLabelNumber (LabelNumber + 1);
  }
}

/**
  Generate assembly code for emulation to poll S3M Output Data readiness.

  @param[in]  SocketId          Socket ID.
  @param[in]  DataType          S3M output data type.

  @retval     VOID
**/
VOID
EFIAPI
UbiosGenerateForPollS3mOutputDataReadiness (
  IN    UINT8    SocketId,
  IN    UINT32   DataType
  )
{
  UINT32        LabelNumber;
  UINT32        RegAddr;
  KTI_HOST_OUT  *KtiHostOutPtr;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;

  if (UbiosGenerationEnabled()) {
    if (GetUbiosLoopbackLabelNumber (&LabelNumber) != EFI_SUCCESS) {
      return;
    }

    RegAddr = (UINT32) UsraGetCsrRegisterAddress (SocketId, 0, S3M_BIOS_W_DATA_OUTPUT_REG_CONFIG_N1_S3M_VREG_REG);

    if (KtiHostOutPtr->CurrentReset == POST_RESET_POWERGOOD) {
      DEBUG ((EFI_D_ERROR, LABEL_C_S3M_MAILBOX, "\n", LabelNumber, ":\n"));
    } else {
      DEBUG ((EFI_D_ERROR, LABEL_W_S3M_MAILBOX, "\n", LabelNumber, ":\n"));
    }

    DEBUG ((EFI_D_ERROR, "  mov eax, DWORD PTR ds:[0%08xh]\n", RegAddr));
    DEBUG ((EFI_D_ERROR, "  bt  eax, 31\n"));  //Test BIT[31]: s3m_data_output_valid

    if (KtiHostOutPtr->CurrentReset == POST_RESET_POWERGOOD) {
      DEBUG ((EFI_D_ERROR, LABEL_C_S3M_MAILBOX, "  jnc ", LabelNumber, "\n"));
    } else {
      DEBUG ((EFI_D_ERROR, LABEL_W_S3M_MAILBOX, "  jnc ", LabelNumber, "\n"));
    }

    DEBUG ((EFI_D_ERROR, "  and eax, 000000FFh\n"));  //Test BIT[7:0]: s3m_generic_data_output_type
    DEBUG ((EFI_D_ERROR, "  cmp eax, 0%08xh\n", DataType));

    if (KtiHostOutPtr->CurrentReset == POST_RESET_POWERGOOD) {
      DEBUG ((EFI_D_ERROR, LABEL_C_S3M_MAILBOX, "  jnz ", LabelNumber, "\n"));
    } else {
      DEBUG ((EFI_D_ERROR, LABEL_W_S3M_MAILBOX, "  jnz ", LabelNumber, "\n"));
    }

    SetUbiosLoopbackLabelNumber (LabelNumber + 1);
  }
}

/**
  Generate assembly code for emulation to read S3M Output data.

  @param[in]  SocketId          Socket ID.

  @retval     VOID
**/
VOID
EFIAPI
UbiosGenerateForReadS3mOutputData (
  IN    UINT8    SocketId
  )
{
  UINT32         LabelNumber;
  UINT32         CounterRegAddr;
  UINT32         DataOutRegLoAddr;
  UINT32         DataOutRegHiAddr;
  KTI_HOST_OUT   *KtiHostOutPtr;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;

  if (UbiosGenerationEnabled()) {
    if (GetUbiosLoopbackLabelNumber (&LabelNumber) != EFI_SUCCESS) {
      return;
    }

    CounterRegAddr = (UINT32) UsraGetCsrRegisterAddress (SocketId, 0, S3M_BIOS_W_DATA_OUTPUT_REG_CONFIG_N0_S3M_VREG_REG);
    DataOutRegLoAddr = (UINT32) UsraGetCsrRegisterAddress (SocketId, 0, S3M_BIOS_W_GENERIC_DATA_OUT_REG_N0_S3M_VREG_REG);
    DataOutRegHiAddr = (UINT32) UsraGetCsrRegisterAddress (SocketId, 0, S3M_BIOS_W_GENERIC_DATA_OUT_REG_N1_S3M_VREG_REG);

    if (KtiHostOutPtr->CurrentReset == POST_RESET_POWERGOOD) {
      DEBUG ((EFI_D_ERROR, LABEL_C_S3M_MAILBOX, "\n", LabelNumber, ":\n"));
    } else {
      DEBUG ((EFI_D_ERROR, LABEL_W_S3M_MAILBOX, "\n", LabelNumber, ":\n"));
    }

    DEBUG ((EFI_D_ERROR, "  mov eax, DWORD PTR ds:[0%08xh]\n", CounterRegAddr));   //Get the data size(in DW) to be read out
    DEBUG ((EFI_D_ERROR, "  cmp eax, 00000000h\n"));

    if (KtiHostOutPtr->CurrentReset == POST_RESET_POWERGOOD) {
      DEBUG ((EFI_D_ERROR, LABEL_C_S3M_MAILBOX, "  jz ", LabelNumber + 1, "\n"));  //No more data to be read out
    } else {
      DEBUG ((EFI_D_ERROR, LABEL_W_S3M_MAILBOX, "  jz ", LabelNumber + 1, "\n"));  //No more data to be read out
    }

    //
    // Read data from a 64bit register
    //
    DEBUG ((EFI_D_ERROR, "  mov ebx, DWORD PTR ds:[0%08xh]\n", DataOutRegLoAddr)); //Store the low 32bit data in ebx
    DEBUG ((EFI_D_ERROR, "  mov ecx, DWORD PTR ds:[0%08xh]\n", DataOutRegHiAddr)); //Store the high 32bit data in ecx

    if (KtiHostOutPtr->CurrentReset == POST_RESET_POWERGOOD) {
      DEBUG ((EFI_D_ERROR, LABEL_C_S3M_MAILBOX, "  jmp ", LabelNumber, "\n"));     //Go back to check if there are any remained date to read
    } else {
      DEBUG ((EFI_D_ERROR, LABEL_W_S3M_MAILBOX, "  jmp ", LabelNumber, "\n"));     //Go back to check if there are any remained date to read
    }

    if (KtiHostOutPtr->CurrentReset == POST_RESET_POWERGOOD) {
      DEBUG ((EFI_D_ERROR, LABEL_C_S3M_MAILBOX, "\n", LabelNumber + 1, ":\n"));
    } else {
      DEBUG ((EFI_D_ERROR, LABEL_W_S3M_MAILBOX, "\n", LabelNumber + 1, ":\n"));
    }

    SetUbiosLoopbackLabelNumber (LabelNumber + 2);  //Two label number are used in this routine
  }
}

/**
  Try to acquire S3M MailBox.

  @param[in]  SocketId          Socket ID.

  @retval EFI_SUCCESS           The S3M MailBox is acquired.
  @retval EFI_TIMEOUT           The S3M MailBox is not acquired.
**/
EFI_STATUS
EFIAPI
AcquireS3mMailbox (
  IN    UINT8    SocketId
  )
{
  UINT32                                        LoopCnt;
  S3M_MAILBOX_FLOW_STATUS_N0_S3M_TREG_STRUCT    MailBoxFlowStatusN0;
  UINT8                                         *RegAddr;
  BOOLEAN                                       PowerGood;

  DEBUG ((EFI_D_ERROR, "\nPolling mailbox_busy to acquire S3M MailBox\n"));

  //
  // Polling the Bit0 to be 0
  //
  if (UbiosGenerationEnabled()) {
    RegAddr = (UINT8 *) UsraGetCsrRegisterAddress (SocketId, 0, S3M_MAILBOX_FLOW_STATUS_N0_S3M_TREG_REG);
    PowerGood = (KTI_HOST_OUT_PTR->CurrentReset == POST_RESET_POWERGOOD) ? TRUE : FALSE;
    EmulationPolling (RegAddr, BIT0, 0, PowerGood);
  }

  //
  // Define the maximum times to pulling the S3M MailBox Flow Status Register
  //
  LoopCnt = MAX_POLLING_TIMES;

  while (LoopCnt > 0) {
    MailBoxFlowStatusN0.Data = UsraCsrRead (SocketId, 0, S3M_MAILBOX_FLOW_STATUS_N0_S3M_TREG_REG);
    if (MailBoxFlowStatusN0.Bits.mailbox_busy == 0) {
      return EFI_SUCCESS;
    }

    MicroSecondDelay (POLLING_INTERVAL_TIME);
    LoopCnt--;
  }

  DEBUG ((EFI_D_ERROR, "  Acquire S3M MailBox failed: Polling times exhausted!\n"));

  return EFI_TIMEOUT;
}

/**
  Poll the S3M MailBox flow status.

  @param[in]  SocketId          Socket ID.
  @param[in]  StatusEncoding    The encoding of the MailBox flow status to polling

  @retval     S3M MailBox flow status encoding.
**/
UINT8
EFIAPI
PollS3mMailboxFlowStatus (
  IN    UINT8                   SocketId,
  IN    UINT8                   StatusEncoding
  )
{
  UINT32                                        LoopCnt;
  S3M_MAILBOX_FLOW_STATUS_N0_S3M_TREG_STRUCT    MailBoxFlowStatusN0;

  DEBUG ((EFI_D_ERROR, "\nPolling S3M MailBox Flow status - Encoding: 0x%x\n", StatusEncoding));

  if (StatusEncoding != S3M_MAILBOX_UPDATE_IN_PROGRESS) {
    //
    // UBIOS does not need to polling the UPDATE_IN_PROGRESS status as this will not 100% show up.
    // Otherwise it will cause emulation hang up.
    //
    UbiosGenerateForPollS3mMailboxFlowStatus (SocketId, StatusEncoding);
  }

  //
  // Define the maximum times to pulling the S3M MailBox Flow Status Register
  //
  LoopCnt = MAX_POLLING_TIMES;

  while (LoopCnt > 0) {
    MailBoxFlowStatusN0.Data = UsraCsrRead (SocketId, 0, S3M_MAILBOX_FLOW_STATUS_N0_S3M_TREG_REG);

    if (IS_S3M_MB_FLOW_COMPLETED (MailBoxFlowStatusN0.Bits.mailbox_flow_status_encoding)) {
      DEBUG ((EFI_D_INFO, "  S3M MB flow is already completed!\n"));
      return (UINT8) MailBoxFlowStatusN0.Bits.mailbox_flow_status_encoding;
    }

    if (MailBoxFlowStatusN0.Bits.mailbox_flow_status_encoding == StatusEncoding) {
      DEBUG ((EFI_D_INFO, "  S3M MB is already in expected status!\n"));
      return StatusEncoding;
    }

    MicroSecondDelay (POLLING_INTERVAL_TIME);
    LoopCnt--;
  }

  DEBUG ((EFI_D_ERROR, "  Fail: S3M MB is not in expected status before %d(us) timeout!\n", POLLING_INTERVAL_TIME * MAX_POLLING_TIMES));

  return (UINT8) MailBoxFlowStatusN0.Bits.mailbox_flow_status_encoding;
}

/**
  Poll the S3M output data readiness.

  @param[in]  SocketId          Socket ID.
  @param[in]  DataType          S3M output data type.

  @retval EFI_SUCCESS           The S3M output data is valid and ready for read.
  @retval EFI_TIMEOUT           The S3M output data is not ready.
**/
EFI_STATUS
EFIAPI
PollS3mOutputDataReadiness (
  IN    UINT8    SocketId,
  IN    UINT32   DataType
  )
{
  UINT32                                                  LoopCnt;
  S3M_BIOS_W_DATA_OUTPUT_REG_CONFIG_N1_S3M_VREG_STRUCT    DataOutputRegConfigN1;

  LoopCnt = MAX_POLLING_TIMES;

  DEBUG ((EFI_D_ERROR, "\nPolling S3M MailBox Output Data readiness\n"));

  UbiosGenerateForPollS3mOutputDataReadiness (SocketId, DataType);

  while (LoopCnt > 0) {
    DataOutputRegConfigN1.Data = UsraCsrRead (SocketId, 0, S3M_BIOS_W_DATA_OUTPUT_REG_CONFIG_N1_S3M_VREG_REG);

    if ((DataOutputRegConfigN1.Bits.s3m_data_output_valid == 1) && (DataOutputRegConfigN1.Bits.s3m_generic_data_output_type == DataType)) {
      return EFI_SUCCESS;
    }

    MicroSecondDelay (POLLING_INTERVAL_TIME);
    LoopCnt--;
  }

  return EFI_TIMEOUT;
}
