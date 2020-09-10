/** @file
  SPS SMM driver - HMRFPO extended protocol.

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

//
// Standard header files.
//
#include <Uefi.h>

#include <Library/UefiLib.h>
#include <Library/IoLib.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Library/SmmServicesTableLib.h>
#include "Library/MeTypeLib.h"
#include <Library/HobLib.h>
#include <Library/PreSiliconEnvDetectLib.h>

//
// Protocols
//
#include <Protocol/HeciControlProtocol.h>
#include <Protocol/SpsSmmHmrfpoExtProtocol.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/SmmVariable.h>
#include <Protocol/HeciTransportProtocol.h>

#include <Guid/HobList.h>
#include <Guid/SpsInfoHobGuid.h>

//
// Driver specific headers.
//
#include <Register/HeciRegs.h>
#include <MeAccess.h>
#include <SpsSmm.h>
#include <SpsSmmHmrfpoExt.h>


// global to store Nonce value read by ME HMRFPO_LOCK request
extern UINT64         gNonceValue;
// global to store pointer of Smm Heci Protocol
extern HECI_CONTROL   *gSmmHeci;


/**
  Initialize HMRFPO Extended protocol.

  @param[in] pSpsSmmHmrfpo    Pointer to base HMRFPO protocol.

  @retval EFI_SUCCESS            Return if success.
  @retval EFI_INVALID_PARAMETER  Pointer to Smm Hmrfpo protocol is Null.
**/
EFI_STATUS
InitializeHmrfpoExt (
  IN SPS_SMM_HMRFPO_PROTOCOL      *pSpsSmmHmrfpo
  )
{
  EFI_STATUS                   Status;
  EFI_HANDLE                   Handle;
  SPS_SMM_HMRFPO_EXT_PROTOCOL  *pSpsSmmHmrfpoExt;

  DEBUG ((DEBUG_INFO, "[SPS_SMM] %a started.\n", __FUNCTION__));

  if (pSpsSmmHmrfpo == NULL) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] Smm Hmrfpo parameter pointer is NULL.\n"));
    ASSERT (pSpsSmmHmrfpo != NULL);
    return EFI_INVALID_PARAMETER;
  }

  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    sizeof (SPS_SMM_HMRFPO_EXT_PROTOCOL),
                    (VOID*) &pSpsSmmHmrfpoExt
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "[SPS_SMM] ERROR: Can't allocate memory for SpsSmmHmrfpoExt protocol! Status=%r\n",
      Status
      ));
    ASSERT_EFI_ERROR (Status);
  } else {
    // Active operational firmware
    pSpsSmmHmrfpoExt->ActiveFwVersion.Minor  = pSpsSmmHmrfpo->ActiveFwVersion.Minor;
    pSpsSmmHmrfpoExt->ActiveFwVersion.Major  = pSpsSmmHmrfpo->ActiveFwVersion.Major;
    pSpsSmmHmrfpoExt->ActiveFwVersion.Build  = pSpsSmmHmrfpo->ActiveFwVersion.Build;
    pSpsSmmHmrfpoExt->ActiveFwVersion.Patch  = pSpsSmmHmrfpo->ActiveFwVersion.Patch;

    // Recovery firmware
    pSpsSmmHmrfpoExt->RecoveryFwVersion.Minor  = pSpsSmmHmrfpo->RecoveryFwVersion.Minor;
    pSpsSmmHmrfpoExt->RecoveryFwVersion.Major  = pSpsSmmHmrfpo->RecoveryFwVersion.Major;
    pSpsSmmHmrfpoExt->RecoveryFwVersion.Build  = pSpsSmmHmrfpo->RecoveryFwVersion.Build;
    pSpsSmmHmrfpoExt->RecoveryFwVersion.Patch  = pSpsSmmHmrfpo->RecoveryFwVersion.Patch;

    //
    // ME in dual-image configuration provides the version of the backup image yet
    //
    // Backup operational firmware (optional)
    pSpsSmmHmrfpoExt->BackupFwVersion.Minor  = pSpsSmmHmrfpo->BackupFwVersion.Minor;
    pSpsSmmHmrfpoExt->BackupFwVersion.Major  = pSpsSmmHmrfpo->BackupFwVersion.Major;
    pSpsSmmHmrfpoExt->BackupFwVersion.Build  = pSpsSmmHmrfpo->BackupFwVersion.Build;
    pSpsSmmHmrfpoExt->BackupFwVersion.Patch  = pSpsSmmHmrfpo->BackupFwVersion.Patch;

    //install SpsSmmHmrfpoExt Protocol
    pSpsSmmHmrfpoExt->Lock            = HmrfpoExt_Lock;
    pSpsSmmHmrfpoExt->Enable          = HmrfpoExt_Enable;
    pSpsSmmHmrfpoExt->MeReset         = HmrfpoExt_MeReset;
    pSpsSmmHmrfpoExt->LockImmediately = HmrfpoExt_LockImmediately;
    pSpsSmmHmrfpoExt->GetStatus       = HmrfpoExt_GetStatus;

    Handle = NULL;
    Status = gSmst->SmmInstallProtocolInterface (
                      &Handle,
                      &gSpsSmmHmrfpoExtProtocolGuid,
                      EFI_NATIVE_INTERFACE,
                      pSpsSmmHmrfpoExt
                      );

    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}


/**
  Procedure to send HMRFPO_LOCK HECI Request

  @param[in,out] HeciMsgLock  struct for Heci message

  @retval EFI_SUCCESS            Return if success.
  @retval EFI_INVALID_PARAMETER  Pointer to Heci message is Null.
**/
EFI_STATUS
CallMsg_HmrfpoExtLock (
  IN OUT HECI_MSG_HMRFPO_EXT_LOCK  *HeciMsgLock
  )
{
  UINT32             RspLen;
  EFI_STATUS         Status;
  EFI_STATUS         StatusFw;
  HECI_FWS_REGISTER  MeFirmwareStatus;

  if (HeciMsgLock == NULL) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] Heci message parameter pointer is NULL.\n"));
    ASSERT (HeciMsgLock != NULL);
    return EFI_INVALID_PARAMETER;
  }

  RspLen = sizeof (HeciMsgLock->Rsp);
  Status = gSmmHeci->HeciSendAndReceive (
                       gSmmHeci,
                       HECI_DEFAULT_PCH_ID,
                       NULL,
                       HECI_DEFAULT_RETRY_NUMBER,
                       SPS_CLIENTID_BIOS,
                       SPS_CLIENTID_ME_MKHI,
                       (UINT32*) &(HeciMsgLock->Req),
                       sizeof (HeciMsgLock->Req),
                       (UINT32*) &(HeciMsgLock->Rsp),
                       &RspLen,
                       NULL,
                       NULL
                       );
  StatusFw = gSmmHeci->HeciFirmwareStatus (gSmmHeci, HECI_DEFAULT_PCH_ID, 1, &MeFirmwareStatus.ul);
  if (EFI_ERROR (StatusFw)) {
    DEBUG ((DEBUG_ERROR, "%a () - cannot get MeFirmwareStatus\n", __FUNCTION__));
    ASSERT_EFI_ERROR (StatusFw);
  }
  DEBUG ((DEBUG_INFO, "[SPS_SMM] MEFS1: 0x%08X\n", MeFirmwareStatus.ul));

  return Status;
}

/**
  Procedure to send HMRFPO_LOCK HECI Request

  @param[in,out] Nonce    buffer to store nonce value from HECI message response

  @retval EFI_SUCCESS            Return if success.
  @retval EFI_DEVICE_ERROR       Cannot get MeFirmwareStatus
                                 or send or receive message or status of response are failed.
  @retval EFI_INVALID_PARAMETER  Pointer to Heci message is Null (from CallMsg_HmrfpoExtLock).
**/
EFI_STATUS
HeciReq_HmrfpoExtLock (
  IN OUT UINT64        *Nonce
  )
{
  EFI_STATUS                Status;
  HECI_MSG_HMRFPO_EXT_LOCK  HeciMsgLock;
  HECI_FWS_REGISTER         MeFirmwareStatus;

  Status = gSmmHeci->HeciFirmwareStatus (gSmmHeci, HECI_DEFAULT_PCH_ID, 1, &MeFirmwareStatus.ul);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () - cannot get MeFirmwareStatus\n", __FUNCTION__));
    ASSERT_EFI_ERROR (Status);
    return EFI_DEVICE_ERROR;
  }
  DEBUG ((DEBUG_INFO, "[SPS_SMM] Sending HMRFPO_LOCK, MEFS1: 0x%08X\n", MeFirmwareStatus.ul));

  // Construct HECI HMRFPO_LOCK request
  ZeroMem (&HeciMsgLock, sizeof (HeciMsgLock));
  HeciMsgLock.Req.Mkhi.Fields.Command     = HMRFPO_CMD_LOCK;
  HeciMsgLock.Req.Mkhi.Fields.IsResponse  = 0;
  HeciMsgLock.Req.Mkhi.Fields.GroupId     = MKHI_GRP_HMRFPO;
  HeciMsgLock.Req.Mkhi.Fields.Reserved    = 0;
  HeciMsgLock.Req.Mkhi.Fields.Result      = 0;

  // Call HMRFPO message: lock
  Status = CallMsg_HmrfpoExtLock (&HeciMsgLock);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] HMRFPO_LOCK send failed (%r)\n", Status));
    return EFI_DEVICE_ERROR;
  }

  if ((HeciMsgLock.Rsp.Mkhi.Fields.IsResponse != 1) ||
      (HeciMsgLock.Rsp.Mkhi.Fields.GroupId != MKHI_GRP_HMRFPO) ||
      (HeciMsgLock.Rsp.Mkhi.Fields.Command != HMRFPO_CMD_LOCK) ||
      (HeciMsgLock.Rsp.Mkhi.Fields.Result != MkhiStatusSuccess)) {
    DEBUG ((
      DEBUG_ERROR,
      "[SPS_SMM] ERROR: Invalid HMRFPO_LOCK response (MKHI: 0x%08X)\n",
      HeciMsgLock.Rsp.Mkhi.Data
      ));

    return EFI_DEVICE_ERROR;
  }

  if (HeciMsgLock.Rsp.Status != HECI_RSP_STATUS_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: HMRFPO_LOCK failed (cause: %d)\n", HeciMsgLock.Rsp.Status));
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "[SPS_SMM] HMRFPO_LOCK reports success\n"));
  if (Nonce != NULL) {
    //save the nonce value
    *Nonce = HeciMsgLock.Rsp.Nonce;
  }

  return EFI_SUCCESS;
}

/**
  Procedure to send HMRFPO_ENABLE HECI Request

  @param[in,out] HeciMsgEnable struct for Heci message

  @retval EFI_SUCCESS            Return if success.
  @retval EFI_INVALID_PARAMETER  Pointer to Heci message is Null.
  @retval EFI_DEVICE_ERROR       Cannot get MeFirmwareStatus.
**/
EFI_STATUS
CallMsg_HmrfpoExtEnable (
  IN OUT HECI_MSG_HMRFPO_EXT_ENABLE *HeciMsgEnable
  )
{
  UINT32                      RspLen;
  EFI_STATUS                  Status;
  EFI_STATUS                  StatusFw;
  HECI_FWS_REGISTER           MeFirmwareStatus;
  UINT32                      MeRebootTimeout;
  UINT32                      Timeout;
  HECI_MSG_HMRFPO_EXT_ENABLE  HeciMsgEnableInternal;

  if (HeciMsgEnable == NULL) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] Heci message parameter pointer is NULL.\n"));
    ASSERT (HeciMsgEnable != NULL);
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&HeciMsgEnableInternal, sizeof (HECI_MSG_HMRFPO_EXT_ENABLE));
  CopyMem (&HeciMsgEnableInternal, HeciMsgEnable, sizeof (HECI_MSG_HMRFPO_EXT_ENABLE));

  Timeout = SPS_INITIALIZATION_TIMEOUT/2;
  MeRebootTimeout     = (HECI_ME_REINIT_TIMEOUT + HECI_TIMEOUT_TICK / 2) / HECI_TIMEOUT_TICK;

  StatusFw = gSmmHeci->HeciFirmwareStatus (gSmmHeci, HECI_DEFAULT_PCH_ID, 1, &MeFirmwareStatus.ul);
  if (EFI_ERROR (StatusFw)) {
    DEBUG ((DEBUG_ERROR, "%a () - cannot get MeFirmwareStatus\n", __FUNCTION__));
    ASSERT_EFI_ERROR (StatusFw);
    return EFI_DEVICE_ERROR;
  }

  if (MeFirmwareStatus.r.CurrentState == ME_STATE_NORMAL) {
    Status = gSmmHeci->HeciSend (
                         gSmmHeci,
                         HECI_DEFAULT_PCH_ID,
                         &Timeout,
                         SPS_CLIENTID_BIOS,
                         SPS_CLIENTID_ME_MKHI,
                         (UINT32*) &HeciMsgEnableInternal.Req,
                         sizeof (HeciMsgEnableInternal.Req)
                         );

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: HMRFPO_ENABLE sending failed (%r)\n", Status));
      return Status;
    }

    while (TRUE) {
      StatusFw = gSmmHeci->HeciFirmwareStatus (gSmmHeci, HECI_DEFAULT_PCH_ID, 1, &MeFirmwareStatus.ul);
      if (EFI_ERROR (StatusFw)) {
        DEBUG ((DEBUG_ERROR, "%a () - cannot get MeFirmwareStatus\n", __FUNCTION__));
        ASSERT_EFI_ERROR (StatusFw);
        return EFI_DEVICE_ERROR;
      }
      if (MeFirmwareStatus.r.CurrentState == ME_STATE_RECOVERY) {
        DEBUG ((DEBUG_INFO, "[SPS_SMM] ME is in Recovery state, MEFS1: 0x%08X\n", MeFirmwareStatus.ul));
        break;
      }
      if (MeRebootTimeout-- == 0) {
        DEBUG ((DEBUG_WARN, "[SPS_SMM] Timeout. ME still is in Normal state, MEFS1: 0x%08X\n", MeFirmwareStatus.ul));
        break;
      }
      MicroSecondDelay (HECI_TIMEOUT_TICK);
    }
  }

  RspLen = sizeof (HeciMsgEnable->Rsp);
  Status = gSmmHeci->HeciSendAndReceive (
                       gSmmHeci,
                       HECI_DEFAULT_PCH_ID,
                       NULL,
                       HECI_DEFAULT_RETRY_NUMBER,
                       SPS_CLIENTID_BIOS,
                       SPS_CLIENTID_ME_MKHI,
                       (UINT32*) &(HeciMsgEnable->Req),
                       sizeof (HeciMsgEnable->Req),
                       (UINT32*) &(HeciMsgEnable->Rsp),
                       &RspLen,
                       NULL,
                       NULL
                       );
  StatusFw = gSmmHeci->HeciFirmwareStatus (gSmmHeci, HECI_DEFAULT_PCH_ID, 1, &MeFirmwareStatus.ul);
  if (EFI_ERROR (StatusFw)) {
    DEBUG ((DEBUG_ERROR, "%a () - cannot get MeFirmwareStatus\n", __FUNCTION__));
    ASSERT_EFI_ERROR (StatusFw);
  }
  DEBUG ((DEBUG_INFO, "[SPS_SMM] MEFS1: 0x%08X\n", MeFirmwareStatus.ul));
  return Status;
}

/**
  Procedure to send HMRFPO_ENABLE HECI Request

  Please note: After sending HmrfpoEnable message ME needs to reset itself
  - this means that this is highly probable that this function fails at first few tries.
  This is recommended to call this function in a loop a few times until success is returned.
  Please refer to ME-Bios spec. for more details.

  @param[in]    Nonce         Nonce value for current session
  @param[out]   FactoryBase   Pointer to the base address of the Intel ME factory data area.
                              Passing NULL pointer is legal.
  @param[out]   FactoryLimit  Pointer to the length of the Intel ME factory data area.
                              Passing NULL pointer is legal.

  @retval EFI_SUCCESS            Return if success.
  @retval EFI_DEVICE_ERROR       Cannot get MeFirmwareStatus
                                 or send or receive message or status of response are failed.
  @retval EFI_INVALID_PARAMETER  Pointer to Heci message is Null (from CallMsg_HmrfpoExtEnable).
**/
EFI_STATUS
HeciReq_HmrfpoExtEnable (
  IN  UINT64   Nonce,
  OUT UINT32   *FactoryBase,
  OUT UINT32   *FactoryLimit
  )
{
  EFI_STATUS                  Status;
  HECI_MSG_HMRFPO_EXT_ENABLE  HeciMsgEnable;
  HECI_FWS_REGISTER           MeFirmwareStatus;

  Status = gSmmHeci->HeciFirmwareStatus (gSmmHeci, HECI_DEFAULT_PCH_ID, 1, &MeFirmwareStatus.ul);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () - cannot get MeFirmwareStatus\n", __FUNCTION__));
    ASSERT_EFI_ERROR (Status);
    return EFI_DEVICE_ERROR;
  }
  DEBUG ((DEBUG_INFO, "[SPS_SMM] Sending HMRFPO_ENABLE, MEFS1: 0x%08X\n", MeFirmwareStatus.ul));

  // Construct HMRFPO_ENABLE request message
  ZeroMem (&HeciMsgEnable, sizeof (HeciMsgEnable));

  HeciMsgEnable.Req.Mkhi.Fields.Command     = HMRFPO_CMD_ENABLE;
  HeciMsgEnable.Req.Mkhi.Fields.IsResponse  = 0;
  HeciMsgEnable.Req.Mkhi.Fields.GroupId     = MKHI_GRP_HMRFPO;
  HeciMsgEnable.Req.Mkhi.Fields.Reserved    = 0;
  HeciMsgEnable.Req.Mkhi.Fields.Result      = 0;
  HeciMsgEnable.Req.Nonce = Nonce;

  Status = CallMsg_HmrfpoExtEnable (&HeciMsgEnable);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: HMRFPO_ENABLE sending failed (%r)\n", Status));
    return EFI_DEVICE_ERROR;
  }

  if ((HeciMsgEnable.Rsp.Mkhi.Fields.IsResponse != 1) ||
      (HeciMsgEnable.Rsp.Mkhi.Fields.GroupId != MKHI_GRP_HMRFPO) ||
      (HeciMsgEnable.Rsp.Mkhi.Fields.Command != HMRFPO_CMD_ENABLE) ||
      (HeciMsgEnable.Rsp.Mkhi.Fields.Result != MkhiStatusSuccess)) {
    DEBUG ((
      DEBUG_ERROR,
      "[SPS_SMM] ERROR: Invalid HMRFPO_ENABLE response (MKHI: 0x%08X)\n",
      HeciMsgEnable.Rsp.Mkhi.Data
      ));
    return EFI_DEVICE_ERROR;
  }

  if (HeciMsgEnable.Rsp.Status != HECI_RSP_STATUS_SUCCESS) {
    DEBUG ((
      DEBUG_ERROR,
      "[SPS_SMM] ERROR: HMRFPO_ENABLE refused (cause: %d)\n",
      HeciMsgEnable.Rsp.Status
      ));
    return EFI_DEVICE_ERROR;
  }

  if (FactoryBase != NULL) {
    *FactoryBase = HeciMsgEnable.Rsp.FactoryBase;
  }
  if (FactoryLimit != NULL) {
    *FactoryLimit = HeciMsgEnable.Rsp.FactoryLimit;
  }

  DEBUG ((DEBUG_INFO, "[SPS_SMM] HMRFPO_ENABLE reports success.\n"));
  return EFI_SUCCESS;
}

/**
  Procedure to send HMRFPO_MERESET HECI Request

  @param[in,out] HeciMsgMeReset  struct for Heci message

  @retval EFI_SUCCESS            Return if success.
  @retval EFI_INVALID_PARAMETER  Pointer to Heci message is Null.
  @retval EFI_DEVICE_ERROR       Cannot get MeFirmwareStatus.
**/
EFI_STATUS
CallMsg_HmrfpoExtMeReset (
  IN OUT HECI_MSG_HMRFPO_EXT_MERESET *HeciMsgMeReset
  )
{
  UINT32             RspLen;
  EFI_STATUS         Status;
  EFI_STATUS         StatusFw;
  HECI_FWS_REGISTER  MeFirmwareStatus;

  if (HeciMsgMeReset == NULL) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] Heci message parameter pointer is NULL.\n"));
    ASSERT (HeciMsgMeReset != NULL);
    return EFI_INVALID_PARAMETER;
  }

  RspLen = sizeof (HeciMsgMeReset->Rsp);
  Status = gSmmHeci->HeciSendAndReceive (
                       gSmmHeci,
                       HECI_DEFAULT_PCH_ID,
                       NULL,
                       HECI_DEFAULT_RETRY_NUMBER,
                       SPS_CLIENTID_BIOS,
                       SPS_CLIENTID_ME_MKHI,
                       (UINT32*) &(HeciMsgMeReset->Req),
                       sizeof (HeciMsgMeReset->Req),
                       (UINT32*) &(HeciMsgMeReset->Rsp),
                       &RspLen,
                       NULL,
                       NULL
                       );
  StatusFw = gSmmHeci->HeciFirmwareStatus (gSmmHeci, HECI_DEFAULT_PCH_ID, 1, &MeFirmwareStatus.ul);
  if (EFI_ERROR (StatusFw)) {
    DEBUG ((DEBUG_ERROR, "%a () - cannot get MeFirmwareStatus\n", __FUNCTION__));
    ASSERT_EFI_ERROR (StatusFw);
  }
  DEBUG ((DEBUG_INFO, "[SPS_SMM] MEFS1: 0x%08X\n", MeFirmwareStatus.ul));
  return Status;
}

/**
  Procedure to send HMRFPO_RESET HECI Request

  Please note: HmrfpoReset message make ME to reset itself
  this means that this is highly probable that this function return fail (timeout occurs).

  Please refer to ME-Bios spec. for more details.

  @param[in]  Nonce      Nonce value for current session

  @retval EFI_SUCCESS            Return if success.
  @retval EFI_DEVICE_ERROR       Cannot get MeFirmwareStatus
                                 or send or receive message or status of response are failed.
  @retval EFI_INVALID_PARAMETER  Pointer to Heci message is Null (from CallMsg_HmrfpoExtMeReset).
**/
EFI_STATUS
HeciReq_HmrfpoExtMeReset (
  IN UINT64                Nonce
  )
{
  EFI_STATUS                   Status;
  HECI_MSG_HMRFPO_EXT_MERESET  HeciMsgMeReset;
  HECI_FWS_REGISTER            MeFirmwareStatus;

  Status = gSmmHeci->HeciFirmwareStatus (gSmmHeci, HECI_DEFAULT_PCH_ID, 1, &MeFirmwareStatus.ul);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () - cannot get MeFirmwareStatus\n", __FUNCTION__));
    ASSERT_EFI_ERROR (Status);
    return EFI_DEVICE_ERROR;
  }
  DEBUG ((DEBUG_INFO, "[SPS_SMM] Sending HMRFPO_MERESET, MEFS1: 0x%08X\n", MeFirmwareStatus.ul));

  // Construct HMRFPO_MERESET request message
  ZeroMem (&HeciMsgMeReset, sizeof (HeciMsgMeReset));

  HeciMsgMeReset.Req.Mkhi.Fields.Command     = HMRFPO_CMD_MERESET;
  HeciMsgMeReset.Req.Mkhi.Fields.IsResponse  = 0;
  HeciMsgMeReset.Req.Mkhi.Fields.GroupId     = MKHI_GRP_HMRFPO;
  HeciMsgMeReset.Req.Mkhi.Fields.Reserved    = 0;
  HeciMsgMeReset.Req.Mkhi.Fields.Result      = 0;
  HeciMsgMeReset.Req.Nonce = Nonce;

  Status = CallMsg_HmrfpoExtMeReset (&HeciMsgMeReset);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] HMRFPO_MERESET send failed (%r)\n", Status));
    return EFI_DEVICE_ERROR;
  }

  if ((HeciMsgMeReset.Rsp.Mkhi.Fields.IsResponse != 1) ||
      (HeciMsgMeReset.Rsp.Mkhi.Fields.GroupId != MKHI_GRP_HMRFPO) ||
      (HeciMsgMeReset.Rsp.Mkhi.Fields.Command != HMRFPO_CMD_MERESET) ||
      (HeciMsgMeReset.Rsp.Mkhi.Fields.Result != MkhiStatusSuccess)) {
    DEBUG ((
      DEBUG_ERROR,
      "[SPS_SMM] ERROR: Invalid HMRFPO_MERESET response (MKHI: 0x%08X)\n",
      HeciMsgMeReset.Rsp.Mkhi.Data
      ));
    return EFI_DEVICE_ERROR;
  }

  if (HeciMsgMeReset.Rsp.Status != HECI_RSP_STATUS_SUCCESS) {
    DEBUG ((
      DEBUG_ERROR,
      "[SPS_SMM] ERROR: HMRFPO_MERESET refused (cause: %d)\n",
      HeciMsgMeReset.Rsp.Status
      ));
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "[SPS_SMM] HMRFPO_MERESET reports success.\n"));
  return EFI_SUCCESS;
}


/**
  Procedure to send HMRFPO_LOCK_IMMEDIATELY HECI Request

  @param[in,out] HeciMsgLock  struct for Heci message

  @retval EFI_SUCCESS            Return if success.
  @retval EFI_INVALID_PARAMETER  Pointer to Heci message is Null.
  @retval EFI_DEVICE_ERROR       Cannot get MeFirmwareStatus.
**/
EFI_STATUS
CallMsg_HmrfpoExtLockImmediately (
  IN OUT HECI_MSG_HMRFPO_EXT_LOCK_IMMEDIATELY  *HeciMsgLockImmediately
  )
{
  UINT32             RspLen;
  EFI_STATUS         Status;
  EFI_STATUS         StatusFw;
  HECI_FWS_REGISTER  MeFirmwareStatus;

  if (HeciMsgLockImmediately == NULL) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] Heci message parameter pointer is NULL.\n"));
    ASSERT (HeciMsgLockImmediately != NULL);
    return EFI_INVALID_PARAMETER;
  }

  RspLen = sizeof (HeciMsgLockImmediately->Rsp);
  Status = gSmmHeci->HeciSendAndReceive (
                       gSmmHeci,
                       HECI_DEFAULT_PCH_ID,
                       NULL,
                       HECI_DEFAULT_RETRY_NUMBER,
                       SPS_CLIENTID_BIOS,
                       SPS_CLIENTID_ME_MKHI,
                       (UINT32*) &(HeciMsgLockImmediately->Req),
                       sizeof (HeciMsgLockImmediately->Req),
                       (UINT32*) &(HeciMsgLockImmediately->Rsp),
                       &RspLen,
                       NULL,
                       NULL
                       );
  StatusFw = gSmmHeci->HeciFirmwareStatus (gSmmHeci, HECI_DEFAULT_PCH_ID, 1, &MeFirmwareStatus.ul);
  if (EFI_ERROR (StatusFw)) {
    DEBUG ((DEBUG_ERROR, "%a () - cannot get MeFirmwareStatus\n", __FUNCTION__));
    ASSERT_EFI_ERROR (StatusFw);
  }
  DEBUG ((DEBUG_INFO, "[SPS_SMM] MEFS1: 0x%08X\n", MeFirmwareStatus.ul));
  return Status;
}

/**
  Procedure to send HMRFPO_LOCK_IMMEDIATELY HECI Request

  @param[in] Type               Type of lock.

  @retval EFI_SUCCESS            Return if success.
  @retval EFI_DEVICE_ERROR       Cannot get MeFirmwareStatus
                                 or send or receive message or status of response are failed.
  @retval EFI_INVALID_PARAMETER  Type parameter not supported
                                 or pointer to Heci message is Null (from CallMsg_HmrfpoExtLockImmediately).
**/
EFI_STATUS
HeciReq_HmrfpoExtLockImmediately (
  IN HMRFPO_EXT_LOCK_TYPE  Type
  )
{
  EFI_STATUS                            Status;
  HECI_MSG_HMRFPO_EXT_LOCK_IMMEDIATELY  HeciMsgLockImmediately;
  HECI_FWS_REGISTER                     MeFirmwareStatus;


  Status = gSmmHeci->HeciFirmwareStatus (gSmmHeci, HECI_DEFAULT_PCH_ID, 1, &MeFirmwareStatus.ul);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () - cannot get MeFirmwareStatus\n", __FUNCTION__));
    ASSERT_EFI_ERROR (Status);
    return EFI_DEVICE_ERROR;
  }
  DEBUG ((DEBUG_INFO, "[SPS_SMM] Sending HMRFPO_EXT_LOCK_IMMEDIATELY, MEFS1: 0x%08X\n", MeFirmwareStatus.ul));

  if ((Type != HmrfpoExtLockType0) && (Type != HmrfpoExtLockType1) && (Type != HmrfpoExtLockType2)) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] Type parameter not supported: %d.\n", Type));
    ASSERT ((Type == HmrfpoExtLockType0) || (Type == HmrfpoExtLockType1) || (Type == HmrfpoExtLockType2));
    return EFI_INVALID_PARAMETER;
  }

  // Construct HECI HMRFPO_EXT_LOCK_IMMEDIATELY request
  ZeroMem (&(HeciMsgLockImmediately), sizeof (HeciMsgLockImmediately));
  HeciMsgLockImmediately.Req.Mkhi.Fields.Command     = HMRFPO_CMD_LOCK_IMMEDIATELY;
  HeciMsgLockImmediately.Req.Mkhi.Fields.IsResponse  = 0;
  HeciMsgLockImmediately.Req.Mkhi.Fields.GroupId     = MKHI_GRP_HMRFPO;
  HeciMsgLockImmediately.Req.Mkhi.Fields.Reserved    = 0;
  HeciMsgLockImmediately.Req.Mkhi.Fields.Result      = 0;
  HeciMsgLockImmediately.Req.Type                    = Type;

  Status = CallMsg_HmrfpoExtLockImmediately (&HeciMsgLockImmediately);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] HMRFPO_EXT_LOCK_IMMEDIATELY send failed (%r)\n", Status));
    return EFI_DEVICE_ERROR;
  }

  Status = gSmmHeci->HeciFirmwareStatus (gSmmHeci, HECI_DEFAULT_PCH_ID, 1, &MeFirmwareStatus.ul);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () - cannot get MeFirmwareStatus\n", __FUNCTION__));
    ASSERT_EFI_ERROR (Status);
  }
  DEBUG ((DEBUG_INFO, "[SPS_SMM] MEFS1: 0x%08X\n", MeFirmwareStatus.ul));

  if ((HeciMsgLockImmediately.Rsp.Mkhi.Fields.IsResponse != 1) ||
      (HeciMsgLockImmediately.Rsp.Mkhi.Fields.GroupId != MKHI_GRP_HMRFPO) ||
      (HeciMsgLockImmediately.Rsp.Mkhi.Fields.Command != HMRFPO_CMD_LOCK_IMMEDIATELY) ||
      (HeciMsgLockImmediately.Rsp.Mkhi.Fields.Result != MkhiStatusSuccess)) {
    DEBUG ((
      DEBUG_ERROR,
      "[SPS_SMM] ERROR: Invalid HMRFPO_EXT_LOCK_IMMEDIATELY response (MKHI: 0x%08X)\n",
      HeciMsgLockImmediately.Rsp.Mkhi.Data
      ));
    return EFI_DEVICE_ERROR;
  }

  if (HeciMsgLockImmediately.Rsp.Status != HECI_RSP_STATUS_SUCCESS) {
    DEBUG ((
      DEBUG_ERROR,
      "[SPS_SMM] ERROR: HMRFPO_EXT_LOCK_IMMEDIATELY failed (cause: %d)\n",
      HeciMsgLockImmediately.Rsp.Status
      ));

    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "[SPS_SMM] HMRFPO_EXT_LOCK_IMMEDIATELY reports success\n"));

  return EFI_SUCCESS;
}


/**
  Procedure to send HMRFPO_GETSTATUS HECI Request

  @param[in,out] HeciMsgLock  struct for Heci message

  @retval EFI_SUCCESS            Return if success.
  @retval EFI_INVALID_PARAMETER  Pointer to Heci message is Null.
  @retval EFI_DEVICE_ERROR       Cannot get MeFirmwareStatus.
**/
EFI_STATUS
CallMsg_HmrfpoExtGetStatus (
  IN OUT HECI_MSG_HMRFPO_EXT_GETSTATE  *HeciMsgGetState
  )
{
  UINT32             RspLen;
  EFI_STATUS         Status;
  EFI_STATUS         StatusFw;
  HECI_FWS_REGISTER  MeFirmwareStatus;

  if (HeciMsgGetState == NULL) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] Heci message parameter pointer is NULL.\n"));
    ASSERT (HeciMsgGetState != NULL);
    return EFI_INVALID_PARAMETER;
  }

  RspLen = sizeof (HeciMsgGetState->Rsp);
  Status = gSmmHeci->HeciSendAndReceive (
                       gSmmHeci,
                       HECI_DEFAULT_PCH_ID,
                       NULL,
                       HECI_DEFAULT_RETRY_NUMBER,
                       SPS_CLIENTID_BIOS,
                       SPS_CLIENTID_ME_MKHI,
                       (UINT32*) &(HeciMsgGetState->Req),
                       sizeof (HeciMsgGetState->Req),
                       (UINT32*) &(HeciMsgGetState->Rsp),
                       &RspLen,
                       NULL,
                       NULL
                       );
  StatusFw = gSmmHeci->HeciFirmwareStatus (gSmmHeci, HECI_DEFAULT_PCH_ID, 1, &MeFirmwareStatus.ul);
  if (EFI_ERROR (StatusFw)) {
    DEBUG ((DEBUG_ERROR, "%a () - cannot get MeFirmwareStatus\n", __FUNCTION__));
    ASSERT_EFI_ERROR (StatusFw);
  }
  DEBUG ((DEBUG_INFO, "[SPS_SMM] MEFS1: 0x%08X\n", MeFirmwareStatus.ul));
  return Status;
}

/**
  Procedure to send HMRFPO_GETSTATUS HECI Request. Return status of flash protection override.

  @param[out]   State            Status of flash protection override.

  @retval EFI_SUCCESS            Return if success.
  @retval EFI_DEVICE_ERROR       Cannot get MeFirmwareStatus
                                 or send or receive message or status of response are failed.
  @retval EFI_INVALID_PARAMETER  State or pointer to Heci message is Null (from CallMsg_HmrfpoExtGetStatus).
**/
EFI_STATUS
HeciReq_HmrfpoExtGetStatus (
  OUT HMRFPO_EXT_STATUS  *State
  )
{
  EFI_STATUS                    Status;
  HECI_MSG_HMRFPO_EXT_GETSTATE  HeciMsgGetState;
  HECI_FWS_REGISTER             MeFirmwareStatus;

  Status = gSmmHeci->HeciFirmwareStatus (gSmmHeci, HECI_DEFAULT_PCH_ID, 1, &MeFirmwareStatus.ul);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () - cannot get MeFirmwareStatus\n", __FUNCTION__));
    ASSERT_EFI_ERROR (Status);
    return EFI_DEVICE_ERROR;
  }
  DEBUG ((DEBUG_INFO, "[SPS_SMM] Sending HMRFPO_GETSTATE, MEFS1: 0x%08X\n", MeFirmwareStatus.ul));

  if (State == NULL) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] State parameter pointer is NULL.\n"));
    ASSERT (State != NULL);
    return EFI_INVALID_PARAMETER;
  }

  // Construct HMRFPO_GETSTATE request message
  ZeroMem (&(HeciMsgGetState), sizeof (HeciMsgGetState));
  HeciMsgGetState.Req.Mkhi.Fields.Command     = HMRFPO_CMD_GETSTATUS;
  HeciMsgGetState.Req.Mkhi.Fields.IsResponse  = 0;
  HeciMsgGetState.Req.Mkhi.Fields.GroupId     = MKHI_GRP_HMRFPO;
  HeciMsgGetState.Req.Mkhi.Fields.Reserved    = 0;
  HeciMsgGetState.Req.Mkhi.Fields.Result      = 0;

  Status = CallMsg_HmrfpoExtGetStatus (&HeciMsgGetState);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] HMRFPO_GETSTATE send failed (%r)\n", Status));
    return EFI_DEVICE_ERROR;
  }

  Status = gSmmHeci->HeciFirmwareStatus (gSmmHeci, HECI_DEFAULT_PCH_ID, 1, &MeFirmwareStatus.ul);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () - cannot get MeFirmwareStatus\n", __FUNCTION__));
    ASSERT_EFI_ERROR (Status);
  }
  DEBUG ((DEBUG_INFO, "[SPS_SMM] MEFS1: 0x%08X\n", MeFirmwareStatus.ul));

  if ((HeciMsgGetState.Rsp.Mkhi.Fields.IsResponse != 1) ||
      (HeciMsgGetState.Rsp.Mkhi.Fields.GroupId != MKHI_GRP_HMRFPO) ||
      (HeciMsgGetState.Rsp.Mkhi.Fields.Command != HMRFPO_CMD_GETSTATUS) ||
      (HeciMsgGetState.Rsp.Mkhi.Fields.Result != MkhiStatusSuccess)) {
    DEBUG ((
      DEBUG_ERROR,
      "[SPS_SMM] ERROR: Invalid HMRFPO_GETSTATE response (MKHI: 0x%08X)\n",
      HeciMsgGetState.Rsp.Mkhi.Data
      ));
    return EFI_DEVICE_ERROR;
  }

  *State = HeciMsgGetState.Rsp.State;

  DEBUG ((DEBUG_INFO, "[SPS_SMM] HMRFPO_GETSTATE reports success.\n"));
  return EFI_SUCCESS;
}


/**
  Procedure to send HMRFPO_LOCK HECI Request

  @retval EFI_SUCCESS            Return if success.
  @retval EFI_DEVICE_ERROR       Send or receive message or status of response are failed
                                 (from HeciReq_HmrfpoExtLock).
  @retval EFI_INVALID_PARAMETER  Pointer to Heci message is Null (from HeciReq_HmrfpoExtLock).
**/
EFI_STATUS
HmrfpoExt_Lock (
  VOID
  )
{
  EFI_STATUS    Status;

  Status = HeciReq_HmrfpoExtLock (NULL);
  if (!IsSimicsEnvironment ()) {
    // Reset HECI to clear NONCE data
    gSmmHeci->HeciReset (gSmmHeci, HECI_DEFAULT_PCH_ID, NULL);
  }

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: %a function returns error (%r)\n", __FUNCTION__, Status));
  }
  return Status;
}

/**
  Procedure to send HMRFPO_LOCK_IMMEDIATELY HECI Request

  @param[in] Type               Type of lock.

  @retval EFI_SUCCESS            Return if success.
  @retval EFI_DEVICE_ERROR       Send or receive message or status of response are failed
                                 (from HeciReq_HmrfpoExtLockImmediately).
  @retval EFI_INVALID_PARAMETER  Type parameter not supported
                                 or pointer to Heci message is Null (from HeciReq_HmrfpoExtLockImmediately).
**/
EFI_STATUS
HmrfpoExt_LockImmediately (
  IN HMRFPO_EXT_LOCK_TYPE Type
  )
{
  EFI_STATUS    Status;

  Status = HeciReq_HmrfpoExtLockImmediately (Type);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: %a function returns error (%r)\n", __FUNCTION__, Status));
  }
  return Status;
}

/**
  Procedure to send HMRFPO_ENABLE HECI Request

  @param[in]    RegionSelect  Indicate region for enable.
                              Only HmrfpoExtEnableRegionIntelMe is supported.
  @param[out]   FactoryBase   Pointer to the base address of the Intel ME factory data area.
                              Passing NULL pointer is legal.
  @param[out]   FactoryLimit  Pointer to the length of the Intel ME factory data area.
                              Passing NULL pointer is legal.

  @retval EFI_SUCCESS            Return if success.
  @retval EFI_UNSUPPORTED        Variable gNonceValue is 0 or unsupported RegionSelect.
  @retval EFI_DEVICE_ERROR       Send or receive message or status of response are failed
                                 (from HeciReq_HmrfpoExtEnable).
  @retval EFI_INVALID_PARAMETER  Pointer to Heci message is Null (from HeciReq_HmrfpoExtEnable).
**/
EFI_STATUS
HmrfpoExt_Enable (
  IN  HMRFPO_EXT_REGION_SELECT  RegionSelect,
  OUT UINT32                    *FactoryBase,
  OUT UINT32                    *FactoryLimit
  )
{
  EFI_STATUS    Status;

  if ((FactoryBase == NULL) || (FactoryLimit == NULL)) {
    DEBUG ((DEBUG_WARN, "[SPS_SMM] WARNING: %a (): NULL pointers passed", __FUNCTION__));
  }

  if (gNonceValue == 0) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Nonce value unknown. Operation not supported\n"));
    return EFI_UNSUPPORTED;
  }

  if (RegionSelect != HmrfpoExtEnableRegionIntelMe) {
    DEBUG ((
      DEBUG_ERROR,
      "[SPS_SMM] ERROR: RegionSelect is not the Region IntelME. RegionSelect (%d) not supported\n",
      RegionSelect
      ));
    ASSERT_EFI_ERROR (EFI_UNSUPPORTED);
    return EFI_UNSUPPORTED;
  }

  Status = HeciReq_HmrfpoExtEnable (gNonceValue, FactoryBase, FactoryLimit);
  if (!IsSimicsEnvironment ()) {
    // Reset HECI to clear NONCE data
    gSmmHeci->HeciReset (gSmmHeci, HECI_DEFAULT_PCH_ID, NULL);
  }

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: %a function returns error (%r)\n", __FUNCTION__, Status));
  }
  return Status;
}

/**
  Procedure to send HMRFPO_MERESET HECI Request

  @param[in]   ResetType      Reset type.

  @retval EFI_SUCCESS            Return if success.
  @retval EFI_UNSUPPORTED        Variable gNonceValue is 0 or unsupported reset type.
  @retval EFI_DEVICE_ERROR       Send or receive message or status of response are failed
                                 (from HeciReq_HmrfpoExtMeReset).
  @retval EFI_INVALID_PARAMETER  Pointer to Heci message is Null (from HeciReq_HmrfpoExtMeReset).
**/
EFI_STATUS
HmrfpoExt_MeReset (
  IN HMRFPO_EXT_RESET_TYPE  ResetType
  )
{
  EFI_STATUS    Status;

  if (gNonceValue == 0) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Nonce value unknown. Operation not supported\n"));
    return EFI_UNSUPPORTED;
  }

  if ((ResetType != HmrfpoExtLegacyReset) && (ResetType != HmrfpoExtFactoryDefaultReset)) {
    DEBUG ((
      DEBUG_ERROR,
      "[SPS_SMM] ERROR: Reset type is not the Legacy ME Reset. Reset type (%d) not supported\n",
      ResetType
      ));
    ASSERT_EFI_ERROR (EFI_UNSUPPORTED);
    return EFI_UNSUPPORTED;
  }

  Status = HeciReq_HmrfpoExtMeReset (gNonceValue);
  if (!IsSimicsEnvironment ()) {
    // Reset HECI to clear NONCE data
    gSmmHeci->HeciReset (gSmmHeci, HECI_DEFAULT_PCH_ID, NULL);
  }

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: %a function returns error (%r)\n", __FUNCTION__, Status));
  }
  return Status;
}

/**
  Procedure to send HMRFPO_GETSTATUS HECI Request. Return status of flash protection override.

  @param[out]  ExtStatus         Status of flash protection override.

  @retval EFI_SUCCESS            Return if success.
  @retval EFI_DEVICE_ERROR       ExtStatus is unsupported
                                 or send or receive message are failed (from HeciReq_HmrfpoExtGetStatus).
  @retval EFI_INVALID_PARAMETER  ExtStatus or pointer to Heci message is Null (from HeciReq_HmrfpoExtGetStatus).
**/
EFI_STATUS
HmrfpoExt_GetStatus (
  OUT HMRFPO_EXT_STATUS *ExtStatus
  )
{
  EFI_STATUS    Status;

  if (ExtStatus == NULL) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] Pointer to status of flash protection override is null.\n"));
    ASSERT (ExtStatus != NULL);
    return EFI_INVALID_PARAMETER;
  }

  Status = HeciReq_HmrfpoExtGetStatus (ExtStatus);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: %a function returns error (%r)\n", __FUNCTION__, Status));
    return Status;
  }

  if ((*ExtStatus != HmrfpoExtStatusDisabled) &&
      (*ExtStatus != HmrfpoExtStatusLocked) &&
      (*ExtStatus != HmrfpoExtStatusEnabled) &&
      (*ExtStatus != HmrfpoExtStatusUpdateInProgressLock)) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: satus (%d) of flash protection override is unknown.\n", *ExtStatus));
    ASSERT_EFI_ERROR (EFI_DEVICE_ERROR);
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

