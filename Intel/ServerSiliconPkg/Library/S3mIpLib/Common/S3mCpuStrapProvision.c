/** @file
  S3M IP library.

  This file implements S3M CPU strap provision flow.

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
#include <Library/S3mIpLib.h>
#include <Library/DebugLib.h>
#include "Include/S3mMailBox.h"
#include <Library/UsraCsrLib.h>
#include "S3mIpRegisters.h"

/**
  Interpret S3M MailBox Flow completion status and return the corresponding EFI_STATUS to caller.

  @param[in]      FlowStsEncoding   S3M MB Flow Status encoding.

  @retval EFI_SUCCESS               S3M MailBox flow completed successfully
  @retval !EFI_SUCCESS              S3M MailBox flow failed
**/
EFI_STATUS
EFIAPI
InterpretS3mMbFlowCompletionStatus (
  IN        UINT8    FlowStsEncoding
  )
{
  DEBUG ((EFI_D_ERROR, "  S3M MailBox Flow Status Encoding = 0x%x: ", FlowStsEncoding));

  if (FlowStsEncoding == S3M_MAILBOX_FLOW_SUCCESS) {
    DEBUG ((EFI_D_ERROR, "Success\n"));
    return EFI_SUCCESS;
  }

  if ((FlowStsEncoding == S3M_MAILBOX_DATA_AUTH_FAILURE) || (FlowStsEncoding == S3M_MAILBOX_FLOW_FAILURE)) {
    DEBUG ((EFI_D_ERROR, "Failed\n"));
    return EFI_PROTOCOL_ERROR;
  }

  DEBUG ((EFI_D_ERROR, "Unexpected\n"));
  ASSERT (FALSE);
  return EFI_ABORTED;
}

/**
  Check the completion of S3M MailBox Flow, which is necessary for S3M to release the MailBox.

  @param[in]      SocketId          The ID of the Socket from which to read CPU Strap Content.

  @retval EFI_SUCCESS               S3M MailBox flow completed successfully
  @retval !EFI_SUCCESS              S3M MailBox flow failed
**/
EFI_STATUS
EFIAPI
CheckS3mFlowCompletionStatus (
  IN        UINT8    SocketId
  )
{
  UINT8         FlowStsEncoding;

  FlowStsEncoding = PollS3mMailboxFlowStatus (SocketId, S3M_MAILBOX_FLOW_SUCCESS);

  return InterpretS3mMbFlowCompletionStatus (FlowStsEncoding);
}

/**
  Read CPU strap from S3M.

  @param[in]      SocketId          The ID of the Socket from which to read CPU Strap Content.
  @param[in out]  Buffer            Pointer of the buffer. If return EFI_SUCCESS, it is the CPU Strap Content.
  @param[in out]  BufSize           Pointer of the buffer size(Unit: Number of DWs). In return, it is the real number of DWs returned.

  @retval EFI_SUCCESS               Read S3M CPU Strap content successfully
  @retval !EFI_SUCCESS              Failed to read S3M CPU Strap content
**/
EFI_STATUS
EFIAPI
ReadS3mCpuStrap (
  IN        UINT8    SocketId,
  IN OUT    UINT32   *Buffer,
  IN OUT    UINT32   *BufSize
  )
{
  EFI_STATUS         Status;
  UINT8              FlowStsEncoding;
  BOOLEAN            ProceedFlow;

  if (SocketId >= MAX_SOCKET) {
    DEBUG ((EFI_D_ERROR, "\n%a (): Invalid parameter - SocketId = 0x%02X.\n", __FUNCTION__, SocketId));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if ((Buffer == NULL) || (BufSize == NULL)) {
    DEBUG ((EFI_D_ERROR, "\n%a (): Invalid parameter - Input NULL pointer.\n", __FUNCTION__));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((EFI_D_ERROR, "\nStart the S3M flow to read CPU_STRAP_CONTENT from Socket[%X]\n", SocketId));

  ProceedFlow = TRUE;

  //
  // Step 1: BIOS polls S3M_MAILBOX_FLOW_STATUS register to get a lock on the mailbox
  //
  Status = AcquireS3mMailbox (SocketId);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "  Abort: Failed to acquire the MailBox\n"));
    return EFI_ABORTED;
  }

  //
  // Step 2: BIOS issues a mailbox command (CPU_STRAP_CONTENT_READ)
  //
  if (ProceedFlow) {
    Status = IssueS3mMailBoxCommand (SocketId, CPU_STRAP_CONTENT_READ);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "  Abort: Failed to send MailBox command\n"));
      ProceedFlow = FALSE;
    }
  }

  //
  // Step 3: Polling the S3M MailBox Flow Status(TREG) to see if the data is ready
  //
  if (ProceedFlow) {
    FlowStsEncoding = PollS3mMailboxFlowStatus (SocketId, S3M_MAILBOX_DATA_READY);
    if (FlowStsEncoding != S3M_MAILBOX_DATA_READY) {
      DEBUG ((EFI_D_ERROR, "  Abort - S3M MailBox Data is not ready\n"));
      ProceedFlow = FALSE;
    }
  }

  //
  // Step 4: Polling the S3M_BIOS_W_DATA_OUTPUT_REG_CONFIG(VREG) to see if the data is valid and data type is expected
  //
  if (ProceedFlow) {
    Status = PollS3mOutputDataReadiness (SocketId, CPU_STRAP_CONTENT_READ);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "  Abort - The output content is not valid\n"));
      ProceedFlow = FALSE;
    }
  }

  //
  // Step 5: Start to receive S3M output contents through streaming registers
  //
  if (ProceedFlow) {
    Status = ReceiveS3mMailBoxOutputData (SocketId, Buffer, BufSize);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "  Abort - Failed to receive S3M output contents\n"));
    }
  }

  //
  // Step 6: BIOS polls S3M_MAILBOX_FLOW_STATUS register to see if S3M finished the flow successfully.
  //
  Status = CheckS3mFlowCompletionStatus (SocketId);

  return Status;
}

/**
  Override CPU Strap into S3M.

  @param[in]  SocketId          The ID of the Socket from which to read CPU Strap Content.
  @param[in]  Buffer            Pointer of the buffer with the CPU strap provisioning data.
  @param[in]  BufSize           Buffer size(Unit: Number of DWs).

  @retval EFI_SUCCESS           Override S3M CPU Strap content successfully
  @retval !EFI_SUCCESS          Failed to override S3M CPU Strap content
**/
EFI_STATUS
EFIAPI
OverrideS3mCpuStrap (
  IN    UINT8    SocketId,
  IN    UINT32   *Buffer,
  IN    UINT32   BufSize
  )
{
  EFI_STATUS     Status;
  UINT8          FlowStsEncoding;
  BOOLEAN        ProceedFlow;

  if (SocketId >= MAX_SOCKET) {
    DEBUG ((EFI_D_ERROR, "\n%a (): Invalid parameter - SocketId = 0x%02X.\n", __FUNCTION__, SocketId));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (Buffer == NULL) {
    DEBUG ((EFI_D_ERROR, "\n%a (): Invalid parameter - Input NULL pointer.\n", __FUNCTION__));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((EFI_D_ERROR, "\n\nStart the S3M flow to override CPU straps for Socket[%X]\n", SocketId));

  ProceedFlow = TRUE;

  //
  // Step 1: BIOS polls S3M_MAILBOX_FLOW_STATUS register to get a lock on the mailbox
  //
  Status = AcquireS3mMailbox (SocketId);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "  Abort: Failed to acquire the MailBox\n"));
    return EFI_ABORTED;
  }

  //
  // Step 2: BIOS writes to the S3M_MAILBOX_DATA_BUFFER with the CPU strap override data
  //
  if (ProceedFlow) {
    Status = FillS3mMailBoxDataBuffer (SocketId, Buffer, BufSize, FALSE);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "  Abort: Failed to fill data\n"));
      ProceedFlow = FALSE;
    }
  }

  //
  // Step 3: BIOS issues a mailbox command (CPU_STRAP_PROVISION)
  //
  if (ProceedFlow) {
    Status = IssueS3mMailBoxCommand (SocketId, CPU_STRAP_PROVISION);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "  Abort: Failed to send MailBox command\n"));
      ProceedFlow = FALSE;
    }
  }

  //
  // Step 4: Check if S3M already starts to execute the flow
  //
  if (ProceedFlow) {
    FlowStsEncoding = PollS3mMailboxFlowStatus (SocketId, S3M_MAILBOX_UPDATE_IN_PROGRESS);

    if (IS_S3M_MB_FLOW_COMPLETED (FlowStsEncoding)) {
      return InterpretS3mMbFlowCompletionStatus (FlowStsEncoding);
    }

    if (FlowStsEncoding != S3M_MAILBOX_UPDATE_IN_PROGRESS) {
      DEBUG ((EFI_D_ERROR, "  Abort - S3M MailBox flow not started\n"));
    }
  }

  //
  // Step 5: BIOS check the completion status by polling on the S3M_MAILBOX_FLOW_STATUS
  //
  Status = CheckS3mFlowCompletionStatus (SocketId);

  return Status;
}

/**
  Provision S3M CFR.

  @param[in]  SocketId          The ID of the Socket.
  @param[in]  Buffer            Pointer of the buffer with the CFR provisioning data.
  @param[in]  BufSize           Buffer size(Unit: Number of DWs).
  @param[in]  Commit            Commit the Provision result.

  @retval EFI_SUCCESS           Provision S3M CFR content successfully
  @retval !EFI_SUCCESS          Failed to Provision S3M CFR content content
**/
EFI_STATUS
EFIAPI
S3mProvision (
  IN    UINT8     SocketId,
  IN    UINT32   *Buffer,
  IN    UINT32    BufSize,
  IN    UINT32    ProvisionCmd
)
{
  EFI_STATUS      Status;
  UINT8           FlowStsEncoding;
  BOOLEAN         ProceedFlow;

  ProceedFlow = TRUE;

  //
  // Step 1: BIOS polls S3M_MAILBOX_FLOW_STATUS register to get a lock on the mailbox
  //
  Status = AcquireS3mMailbox (SocketId);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "  Abort: Failed to acquire the MailBox\n"));
    return EFI_ABORTED;
  }

  //
  // Step 2: BIOS writes to the S3M_MAILBOX_DATA_BUFFER with the CFR provision data
  //
  if (ProceedFlow) {
    Status = FillS3mMailBoxDataBuffer (SocketId, Buffer, BufSize, TRUE);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "  Abort: Failed to fill data\n"));
      ProceedFlow = FALSE;
    }
  }

  //
  // Step 3: BIOS issues a mailbox command
  //
  if (ProceedFlow) {
    Status = IssueS3mMailBoxCommand (SocketId, ProvisionCmd);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "  Abort: Failed to send MailBox command\n"));
      ProceedFlow = FALSE;
    }
  }

  //
  // Step 4: Check if S3M already starts to execute the flow
  //
  if (ProceedFlow) {
    FlowStsEncoding = PollS3mMailboxFlowStatus (SocketId, S3M_MAILBOX_UPDATE_IN_PROGRESS);

    if (IS_S3M_MB_FLOW_COMPLETED (FlowStsEncoding)) {
      return InterpretS3mMbFlowCompletionStatus (FlowStsEncoding);
    }

    if (FlowStsEncoding != S3M_MAILBOX_UPDATE_IN_PROGRESS) {
      DEBUG ((EFI_D_ERROR, "  Abort - S3M MailBox flow not started\n"));
    }
  }

  //
  // Step 5: BIOS check the completion status by polling on the S3M_MAILBOX_FLOW_STATUS
  //
  Status = CheckS3mFlowCompletionStatus (SocketId);

  return Status;
}

/**
  Get S3m Fw Uncommitted Version Info.

  @param[in]         SocketId  - The SocketId need to read
  @param[out]        SVN    - A pointer to the SVN
  @param[out]        RevID  - A pointer to the Valid
  @param[out]        Valid  - A pointer to the Valid

  @retval           KTI_SUCCESS
  @retval           KTI_IGNORE   Feature not supported
**/
EFI_STATUS
GetS3mFwUncommittedVersionInfo (
  UINT8          SocketId,
  UINT32        *SVN,
  UINT32        *RevID,
  UINT32        *Valid
)
{
  S3M_S3MFW_CFR_SVN_N0_S3M_TREG_STRUCT        S3mFwCfrN0;
  S3M_S3MFW_REVID_N0_S3M_TREG_STRUCT          S3mFwRevIDN0;

  S3mFwCfrN0.Data = UsraCsrRead (SocketId, 0, S3M_S3MFW_CFR_SVN_N0_S3M_TREG_REG);
  DEBUG((EFI_D_ERROR, "S%x S3M_S3MFW_CFR_SVN_N0_S3M_TREG_REG : %08x\n", SocketId, S3mFwCfrN0.Data));
  if (SVN != NULL) {
    *SVN = S3mFwCfrN0.Bits.s3m_s3mfw_uncommited_svn;
  }
  if (Valid != NULL) {
    *Valid = S3mFwCfrN0.Bits.s3m_s3mfw_uncommited_svn_valid;
  }
  S3mFwRevIDN0.Data = UsraCsrRead (SocketId, 0, S3M_S3MFW_REVID_N0_S3M_TREG_REG);
  DEBUG((EFI_D_ERROR, "S%x S3M_S3MFW_REVID_N0_S3M_TREG_REG   : %08x\n", SocketId, S3mFwRevIDN0.Data));
  if (RevID != NULL) {
    *RevID = S3mFwRevIDN0.Bits.s3m_s3mfw_uncommited_revid;
  }
  return EFI_SUCCESS;
}


/**
  Get S3m Fw Committed Version Info.

  @param[in]         SocketId  - The SocketId need to read
  @param[out]        SVN    - A pointer to the SVN
  @param[out]        RevID  - A pointer to the Valid
  @param[out]        Valid  - A pointer to the Valid

  @retval           EFI_SUCCESS
**/
EFI_STATUS
GetS3mFwCommittedVersionInfo (
  UINT8          SocketId,
  UINT32        *SVN,
  UINT32        *RevID,
  UINT32        *Valid
)
{
  S3M_S3MFW_CFR_SVN_N1_S3M_TREG_STRUCT        S3mFwCfrN1;
  S3M_S3MFW_REVID_N1_S3M_TREG_STRUCT          S3mFwRevIDN1;

  S3mFwCfrN1.Data = UsraCsrRead (SocketId, 0, S3M_S3MFW_CFR_SVN_N1_S3M_TREG_REG);
  DEBUG((EFI_D_ERROR, "S%x S3M_S3MFW_CFR_SVN_N1_S3M_TREG_REG : %08x\n", SocketId, S3mFwCfrN1.Data));
  if (SVN != NULL) {
    *SVN = S3mFwCfrN1.Bits.s3m_s3mfw_commited_svn;
  }
  if (Valid != NULL) {
    *Valid = S3mFwCfrN1.Bits.s3m_s3mfw_commited_svn_valid;
  }
  S3mFwRevIDN1.Data = UsraCsrRead (SocketId, 0, S3M_S3MFW_REVID_N1_S3M_TREG_REG);
  DEBUG((EFI_D_ERROR, "S%x S3M_S3MFW_REVID_N1_S3M_TREG_REG   : %08x\n", SocketId, S3mFwRevIDN1.Data));
  if (RevID != NULL) {
    *RevID = S3mFwRevIDN1.Bits.s3m_s3mfw_commited_revid;
  }
  return EFI_SUCCESS;
}

/**
  Get S3m Fw Cfr Valid Version Info.

  @param[in]         SocketId  - The SocketId need to read
  @param[out]        SVN    - A pointer to the SVN
  @param[out]        RevID  - A pointer to the Valid
  @param[out]        IsCommitted  - A pointer to the IsCommitted

  @retval           EFI_SUCCESS
  @retval           EFI_DEVICE_ERROR   S3M device error.
**/
EFI_STATUS
EFIAPI
GetS3mCfrValidVersionInfo (
  IN  UINT8         SocketId,
  OUT UINT32        *SVN,
  OUT UINT32        *RevID,
  OUT BOOLEAN       *IsCommitted
)
{
  UINT32        CfrSVN;
  UINT32        CfrRevID;
  UINT32        Valid;

  GetS3mFwUncommittedVersionInfo (SocketId, &CfrSVN, &CfrRevID, &Valid);
  if (Valid == BIT0) {
    *SVN = CfrSVN;
    *RevID = CfrRevID;
    *IsCommitted = FALSE;
    return EFI_SUCCESS;
  }

  GetS3mFwCommittedVersionInfo (SocketId, &CfrSVN, &CfrRevID, &Valid);
  if (Valid == BIT0) {
    *SVN = CfrSVN;
    *RevID = CfrRevID;
    *IsCommitted = TRUE;
    return EFI_SUCCESS;
  }
  DEBUG((EFI_D_ERROR, "No Valid S3mFw Rigister. Error!!!\n"));
  return EFI_DEVICE_ERROR;
}

/**
  Get Pcode/Ucode Uncommitted Version Info.

  @param[in]         SocketId  - The SocketId need to read
  @param[out]        SVN    - A pointer to the SVN
  @param[out]        RevID  - A pointer to the Valid
  @param[out]        Valid  - A pointer to the Valid

  @retval           KTI_SUCCESS
  @retval           KTI_IGNORE   Feature not supported
**/
EFI_STATUS
GetPucodeUncommittedVersionInfo (
  UINT8          SocketId,
  UINT32        *SVN,
  UINT32        *RevID,
  UINT32        *Valid
)
{
  S3M_PUCODE_CFR_SVN_N0_S3M_TREG_STRUCT        PucodeCfrN0;
  S3M_PUCODE_REVID_N0_S3M_TREG_STRUCT          PucodeRevIDN0;

  PucodeCfrN0.Data = UsraCsrRead (SocketId, 0, S3M_PUCODE_CFR_SVN_N0_S3M_TREG_REG);
  DEBUG((EFI_D_ERROR, "S%x S3M_PUCODE_CFR_SVN_N0_S3M_TREG_REG: %08x\n", SocketId, PucodeCfrN0.Data));
  if (SVN != NULL) {
    *SVN = PucodeCfrN0.Bits.s3m_pucode_uncommited_svn;
  }
  if (Valid != NULL) {
    *Valid = PucodeCfrN0.Bits.s3m_pucode_uncommited_svn_valid;
  }
  PucodeRevIDN0.Data = UsraCsrRead (SocketId, 0, S3M_PUCODE_REVID_N0_S3M_TREG_REG);
  DEBUG((EFI_D_ERROR, "S%x S3M_PUCODE_REVID_N0_S3M_TREG_REG  : %08x\n", SocketId, PucodeRevIDN0.Data));
  if (RevID != NULL) {
    *RevID = PucodeRevIDN0.Bits.s3m_pucode_uncommited_revid;
  }
  return EFI_SUCCESS;
}


/**
  Get Pucode Committed Version Info.

  @param[in]         SocketId  - The SocketId need to read
  @param[out]        SVN    - A pointer to the SVN
  @param[out]        RevID  - A pointer to the Valid
  @param[out]        Valid  - A pointer to the Valid

  @retval           EFI_SUCCESS
**/
EFI_STATUS
GetPucodeCommittedVersionInfo (
  UINT8          SocketId,
  UINT32        *SVN,
  UINT32        *RevID,
  UINT32        *Valid
)
{
  S3M_PUCODE_CFR_SVN_N1_S3M_TREG_STRUCT        PucodeCfrN1;
  S3M_PUCODE_REVID_N1_S3M_TREG_STRUCT          PucodeRevIDN1;

  PucodeCfrN1.Data = UsraCsrRead (SocketId, 0, S3M_PUCODE_CFR_SVN_N1_S3M_TREG_REG);
  DEBUG((EFI_D_ERROR, "S%x S3M_PUCODE_CFR_SVN_N1_S3M_TREG_REG: %08x\n", SocketId, PucodeCfrN1.Data));
  if (SVN != NULL) {
    *SVN = PucodeCfrN1.Bits.s3m_pucode_commited_svn;
  }
  if (Valid != NULL) {
    *Valid = PucodeCfrN1.Bits.s3m_pucode_commited_svn_valid;
  }
  PucodeRevIDN1.Data = UsraCsrRead (SocketId, 0, S3M_PUCODE_REVID_N1_S3M_TREG_REG);
  DEBUG((EFI_D_ERROR, "S%x S3M_PUCODE_REVID_N1_S3M_TREG_REG  : %08x\n", SocketId, PucodeRevIDN1.Data));
  if (RevID != NULL) {
    *RevID = PucodeRevIDN1.Bits.s3m_pucode_commited_revid;
  }
  return EFI_SUCCESS;
}

/**
  Get Pcode/Ucode Cfr Valid Version Info.

  @param[in]         SocketId  - The SocketId need to read
  @param[out]        SVN    - A pointer to the SVN
  @param[out]        RevID  - A pointer to the Valid
  @param[out]        IsCommitted  - A pointer to the IsCommitted

  @retval           EFI_SUCCESS
  @retval           EFI_DEVICE_ERROR   S3M device error.
**/
EFI_STATUS
EFIAPI
GetPucodeCfrValidVersionInfo (
  IN  UINT8         SocketId,
  OUT UINT32        *SVN,
  OUT UINT32        *RevID,
  OUT BOOLEAN       *IsCommitted
)
{
  UINT32        CfrSVN;
  UINT32        CfrRevID;
  UINT32        Valid;

  GetPucodeUncommittedVersionInfo (SocketId, &CfrSVN, &CfrRevID, &Valid);
  if (Valid == BIT0) {
    *SVN = CfrSVN;
    *RevID = CfrRevID;
    *IsCommitted = FALSE;
    return EFI_SUCCESS;
  }

  GetPucodeCommittedVersionInfo (SocketId, &CfrSVN, &CfrRevID, &Valid);
  if (Valid == BIT0) {
    *SVN = CfrSVN;
    *RevID = CfrRevID;
    *IsCommitted = TRUE;
    return EFI_SUCCESS;
  }
  DEBUG((EFI_D_ERROR, "No Valid Pucode Rigister. Error!!!\n"));
  return EFI_DEVICE_ERROR;
}
