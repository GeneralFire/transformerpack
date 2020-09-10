/** @file
  S3M IP library.

  This file implements common S3M MailBox functions.

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
#include <Library/UsraCsrLib.h>
#include <Library/BaseLib.h>

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
  )
{
  S3M_MAILBOX_COMMAND_N0_S3M_VREG_STRUCT    MailBoxCmdN0;

  MailBoxCmdN0.Data = 0;
  MailBoxCmdN0.Bits.command_encoding = MbCmd;
  UsraCsrWrite (SocketId, 0, S3M_MAILBOX_COMMAND_N0_S3M_VREG_REG, MailBoxCmdN0.Data);

  return EFI_SUCCESS;
}

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
  )
{
  UINT8                                                   Index;
  S3M_BIOS_W_DATA_OUTPUT_REG_CONFIG_N0_S3M_VREG_STRUCT    DataOutputRegConfigN0;
  S3M_BIOS_W_DATA_OUTPUT_REG_CONFIG_N1_S3M_VREG_STRUCT    DataOutputRegConfigN1;

  UbiosGenerateForReadS3mOutputData (SocketId);

  DataOutputRegConfigN0.Data = UsraCsrRead (SocketId, 0, S3M_BIOS_W_DATA_OUTPUT_REG_CONFIG_N0_S3M_VREG_REG);
  if (DataOutputRegConfigN0.Bits.s3m_generic_data_output_size > *BufSize) {  // Size in num of DWs
    *BufSize = DataOutputRegConfigN0.Bits.s3m_generic_data_output_size;
    DEBUG ((EFI_D_ERROR, "  Buffer too small. Actual number of DWs = 0x%X\n", *BufSize));
    return EFI_BUFFER_TOO_SMALL;
  }

  Index = 0;
  while (TRUE) {
    *(Buffer + Index) = UsraCsrRead (SocketId, 0, S3M_BIOS_W_GENERIC_DATA_OUT_REG_N0_S3M_VREG_REG);
    Index++;
    if (Index >= DataOutputRegConfigN0.Bits.s3m_generic_data_output_size) {
      break;
    }

    *(Buffer + Index) = UsraCsrRead (SocketId, 0, S3M_BIOS_W_GENERIC_DATA_OUT_REG_N1_S3M_VREG_REG);
    Index++;
    if (Index >= DataOutputRegConfigN0.Bits.s3m_generic_data_output_size) {
      break;
    }
  }

  //
  // Optionally check the s3m_data_output_valid to be cleared
  //
  DataOutputRegConfigN1.Data = UsraCsrRead (SocketId, 0, S3M_BIOS_W_DATA_OUTPUT_REG_CONFIG_N1_S3M_VREG_REG);
  if (DataOutputRegConfigN1.Bits.s3m_data_output_valid != 0) {
    DEBUG ((EFI_D_ERROR, "  Device status is not as expected, so the received data in Buffer is undetermined!\n"));
    return EFI_DEVICE_ERROR;
  }

  *BufSize = DataOutputRegConfigN0.Bits.s3m_generic_data_output_size;

  return EFI_SUCCESS;
}

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
  )
{
  UINT32    Index;
  UINT32    Data32;

  Index = 0;
  while (Index < BufSize) {
    Data32 = *(Buffer + Index);
    if (NeedSwap) {
      Data32 = SwapBytes32 (Data32);
    }
    UsraCsrWrite (SocketId, 0, S3M_MAILBOX_DATA_BUFFER_N0_S3M_VREG_REG, Data32);
    Index++;
    if (Index >= BufSize) {
      break;
    }

    Data32 = *(Buffer + Index);
    if (NeedSwap) {
      Data32 = SwapBytes32 (Data32);
    }
    UsraCsrWrite (SocketId, 0, S3M_MAILBOX_DATA_BUFFER_N1_S3M_VREG_REG, Data32);
    Index++;
  }

  return EFI_SUCCESS;
}
