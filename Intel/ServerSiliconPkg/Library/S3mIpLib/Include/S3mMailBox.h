/** @file
  S3M MailBox functions declarations.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary    and
  confidential information of Intel Corporation and its suppliers and licensors,
  and is protected by worlDWORDide copyright and trade secret laws and treaty
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

#ifndef __S3M_MAILBOX_H__
#define __S3M_MAILBOX_H__

#include <Uefi.h>

#define  POLLING_INTERVAL_TIME            10   //Unit: us(Micro Seconds)
#define  MAX_POLLING_TIMES                1000000

//
// S3M_MAILBOX_FLOW_STATUS_ENCODING
//
#define S3M_MAILBOX_IDLE                  0
#define S3M_MAILBOX_DATA_AUTH_FAILURE     1
#define S3M_MAILBOX_FLOW_FAILURE          2
#define S3M_MAILBOX_FLOW_SUCCESS          3
#define S3M_MAILBOX_DATA_READY            4
#define S3M_MAILBOX_UPDATE_IN_PROGRESS    7

/**
  Check if the S3M MailBox Flow is already completed.
  There are three status encodings that indicate the flow is done.
  1) S3M_MAILBOX_DATA_AUTH_FAILURE
  2) S3M_MAILBOX_FLOW_FAILURE
  3) S3M_MAILBOX_FLOW_SUCCESS
**/
#define IS_S3M_MB_FLOW_COMPLETED(StatusEncoding) \
  ((StatusEncoding == S3M_MAILBOX_DATA_AUTH_FAILURE) || \
  (StatusEncoding == S3M_MAILBOX_FLOW_FAILURE) || \
  (StatusEncoding == S3M_MAILBOX_FLOW_SUCCESS))

//
// S3M_MAILBOX_COMMAND_ENCODING
//
#define NO_FLOW                            0x0
#define SOCKET_ONLINING                    0x1
#define CPU_STRAP_PROVISION                0x2
#define SSKU_LICENSE_KEY_PROVISION         0x4
#define SSKU_LICENSE_BLOB_PROVISION        0x8
#define VABD_OEM_KEY_PROVISION             0x10
#define CPU_STRAP_CONTENT_READ             0x20
#define S3M_FW_CFR_PROVISION_NO_SVN_COMMIT 0x40
#define S3M_FW_CFR_PROVISION_SVN_COMMIT    0x41
#define PUCODE_CFR_PROVISION_NO_SVN_COMMIT 0x80
#define PUCODE_CFR_PROVISION_SVN_COMMIT    0x81
#define VAD_TOKEN_PROCESS                  0x200
#define SOCKET_OFFLINED_INDICATION         0x800

/**
  Generate assembly code for emulation to read S3M Output data.

  @param[in]  SocketId          Socket ID.

  @retval     VOID
**/
VOID
EFIAPI
UbiosGenerateForReadS3mOutputData (
  IN    UINT8    SocketId
  );

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
  );

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
  );

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
  );

/**
  Issue a MailBox command to S3M.

  @param[in]  SocketId          Socket ID.
  @param[in]  MbCmd             S3M MailBox Command.

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
IssueS3mMailBoxCommand (
  IN    UINT8    SocketId,
  IN    UINT32   MbCmd
  );

/**
  Receive S3M MailBox output content through streaming registers.

  @param[in]      SocketId          Socket ID.
  @param[in out]  Buffer            Pointer of the buffer. If return EFI_SUCCESS, it is the output content from S3M.
  @param[in out]  BufSize           Pointer of the buffer size(Unit: Number of DWs). In return, it is the real number of DWs of the content to be read out.

  @retval EFI_SUCCESS               Read out S3M output contents successfully.
  @retval !EFI_SUCCESS              Failed to read out S3M output contents.
**/
EFI_STATUS
EFIAPI
ReceiveS3mMailBoxOutputData (
  IN        UINT8    SocketId,
  IN OUT    UINT32   *Buffer,
  IN OUT    UINT32   *BufSize
  );

/**
  Fill S3M MailBox data buffer with input data.

  @param[in]  SocketId          Socket ID.
  @param[in]  Buffer            Pointer of the buffer with the input data.
  @param[in]  BufSize           Buffer size(Unit: Number of DWs).
  @param[in]  NeedSwap          Indicate need swap the data.

  @retval EFI_SUCCESS           Fill S3M MailBox data buffer successfully.
**/
EFI_STATUS
EFIAPI
FillS3mMailBoxDataBuffer (
  IN    UINT8    SocketId,
  IN    UINT32   *Buffer,
  IN    UINT32   BufSize,
  IN    BOOLEAN  NeedSwap
  );

#endif // __S3M_MAILBOX_H__
