/** @file
  Library for SPS RAS Notification support.
  This library is used by RAS.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#include <PiDxe.h>
#include <PiSmm.h>
#include <Library/UefiLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#include <Protocol/SmmBase2.h>
#include <Protocol/HeciControlProtocol.h>
#include <Library/MeUtilsLib.h>

#include <MkhiMsgs.h>
#include <Sps.h>
#include <Guid/SpsInfoHobGuid.h>
#include <Protocol/SpsSmmHmrfpoProtocol.h>
#include <SpsSmm.h>
#include <SpsRasNotify.h>


/*****************************************************************************
 * Local definitions.
 *****************************************************************************/

/*****************************************************************************
 * Variables.
 *****************************************************************************/
extern UINT64 gNonceValue;

/*****************************************************************************
 * Local function prototypes.
 *****************************************************************************/

/**
 * Send RAS notification, wait for ACK.
 *
 * param[in] RasNotificationType    ME UMA error injection mode
 * param[in] Nonce                  Nonce that was  received in previous
 *                                  HMRFPO_LOCK response message
 *
 * return EFI_STATUS is returned.
 *
 *  EFI_PROTOCOL_ERROR    - invalid Nonce or response length
 *
 *  EFI_DEVICE_ERROR      - a persistent ME FW error preventing
 *                          the ME FW from fulfilling the request
 *  EFI_ABORTED           - general error
 *
 */
EFI_STATUS
EFIAPI
MeRasNotifyAck(
  IN RAS_NOTIFICATION_TYPE   RasNotificationType)
{
  EFI_STATUS                  Status;
  UINT32                      BufLen;
  HECI_CONTROL                *pMeHeci = NULL;
  HECI_API_VERSION            HeciApiVersion;
  union {
    HECI_RAS_NOTIFY_REQ Req;
    HECI_RAS_NOTIFY_RSP Rsp;
  } HeciMsg;

  DEBUG ((DEBUG_INFO, "[SPS_SMM] Sending RAS Notification (%d)\n", RasNotificationType));

  pMeHeci = GetHeci (&HeciApiVersion);
  if (pMeHeci == NULL) {
    return EFI_DEVICE_ERROR;
  }
  if (HeciApiVersion != HeciApiHeciControl) {
    ASSERT (FALSE);
    return EFI_DEVICE_ERROR;
  }

  HeciMsg.Req.Cmd = RAS_NOTIFY_REQ;
  HeciMsg.Req.Nonce = gNonceValue;
  HeciMsg.Req.NotificationType = (UINT8) RasNotificationType;
  BufLen = sizeof (HeciMsg.Rsp);
  Status = pMeHeci->HeciSendAndReceive (
                      pMeHeci,
                      HECI_DEFAULT_PCH_ID,
                      NULL,
                      HECI_DEFAULT_RETRY_NUMBER,
                      SPS_CLIENTID_BIOS,
                      SPS_CLIENTID_ME_RAS,
                      (UINT32*) &HeciMsg.Req,
                      sizeof (HeciMsg.Req),
                      (UINT32*) &HeciMsg.Rsp,
                      &BufLen,
                      NULL,
                      NULL
                      );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Cannot send RAS_NOTIFY request (%r)\n", Status));
  } else if (BufLen < sizeof (HeciMsg.Rsp)) {
    DEBUG ((
      DEBUG_ERROR,
      "[SPS_SMM] ERROR: Invalid RAS_NOTIFY response length %d (expect %d)\n",
      BufLen,
      sizeof (HeciMsg.Rsp)
      ));
    Status = EFI_PROTOCOL_ERROR;
  } else if (HeciMsg.Rsp.Status != 0) {
    Status = EFI_ABORTED;
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: RAS_NOTIFY failed (status: %d) - ", HeciMsg.Rsp.Status));
    if (HeciMsg.Rsp.Status == 1) {
      DEBUG ((DEBUG_ERROR, "invalid NONCE\n"));
      Status = EFI_PROTOCOL_ERROR;
    } else if (HeciMsg.Rsp.Status == 2) {
      DEBUG ((DEBUG_ERROR, "request rejected\n"));
      Status = EFI_DEVICE_ERROR;
    } else {
      DEBUG ((DEBUG_ERROR, "unknown error\n"));
    }
  } else if (HeciMsg.Rsp.Nonce != gNonceValue) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: RAS_NOTIFY response has wrong NONCE (HeciMsg.Rsp.Nonce: %d) - ", HeciMsg.Rsp.Nonce));
    Status = EFI_PROTOCOL_ERROR;
  }

  return Status;
} // MeRasNotifyAck ()


/*****************************************************************************
 * Public functions.
 *****************************************************************************/

/**
 * Install gSpsRasNotifyProtocolGuid
 *
 * param none
 *
 * return EFI_STATUS is returned.
 */
EFI_STATUS
EFIAPI
InstallSpsRasNotifyProtocol(
  VOID
  )
{
  EFI_STATUS                  Status;
  HECI_CONTROL                *pMeHeci = NULL;
  SPS_RAS_NOTIFY_PROTOCOL     *pSpsRasNotifyProtocol = NULL;
  EFI_HANDLE                  Handle;
  HECI_API_VERSION            HeciApiVersion;

  DEBUG ((DEBUG_INFO, "[SPS_SMM] Install gSpsRasNotifyProtocolGuid\n"));

  // Check if we can install gSpsRasNotifyProtocolGuid
  pMeHeci = GetHeci (&HeciApiVersion);
  if (pMeHeci == NULL) {
    DEBUG((DEBUG_ERROR, "[SPS_SMM] ERROR: gSpsRasNotifyProtocolGuid can't be installed\n"));
    return EFI_DEVICE_ERROR;
  }
  if (HeciApiVersion != HeciApiHeciControl) {
    ASSERT (FALSE);
    return EFI_DEVICE_ERROR;
  }

  Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof (SPS_RAS_NOTIFY_PROTOCOL), (VOID *) &pSpsRasNotifyProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Can't allocate memory for pSpsRasNotifyProtocol protocol!. Status=%r\n", Status));
    return Status;
  }

  if (gNonceValue == 0) {
    DEBUG ((DEBUG_ERROR, "[SPS SMM] ERROR: Nonce == 0, Don't install gSpsRasNotifyProtocolGuid\n"));
    Status = EFI_PROTOCOL_ERROR;
  } else {
    pSpsRasNotifyProtocol->RasNotify = MeRasNotifyAck;

    Handle = NULL;
    Status = gSmst->SmmInstallProtocolInterface (
                      &Handle,
                      &gSpsRasNotifyProtocolGuid,
                      EFI_NATIVE_INTERFACE,
                      pSpsRasNotifyProtocol
                      );

    DEBUG ((DEBUG_INFO, "[SPS_SMM] gSpsRasNotifyProtocolGuid installed with status %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  if (EFI_ERROR (Status) && (pSpsRasNotifyProtocol != NULL))  {
    gSmst->SmmFreePool (pSpsRasNotifyProtocol);
  }

  return Status;
}
