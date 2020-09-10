/** @file
  Library for Dual Bios Watchdog for Smm modules.

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
*/

#include <PiDxe.h>
#include <PiSmm.h>
#include <Library/UefiLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/IoLib.h>

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#include <Protocol/SmmBase2.h>
#include <Protocol/HeciControlProtocol.h>

#include <MkhiMsgs.h>
#include <Sps.h>
#include <Guid/SpsInfoHobGuid.h>
#include <Protocol/SpsSmmHmrfpoProtocol.h>
#include <SpsSmm.h>
#include <SpsRasNotify.h>
#include <Protocol/SmmCommunication.h>
#include <Library/MeWatchdogControlLib.h>

/*****************************************************************************
 * Local definitions.
*****************************************************************************/
/*****************************************************************************
 * Variables.
 *****************************************************************************/
extern UINT64    gNonceValue;

/**
  Function sends Watcdog Control HECI message.

  @param[in]  SpsSmmWatchdogHeader  pointer to Watchdog structure
 **/
VOID
MeWatchdogControlInSmm (
  IN SPS_SMM_WATCHDOG_HEADER  *SpsSmmWatchdogHeader
  )
{
  EFI_STATUS                 Status;
  HECI_CONTROL               *pHeciPtr;
  HECI_MSG_WATCHDOG_CONTROL  SmmHeciMsg;
  UINT32                     ReqLen;
  UINT32                     RspLen;

  if (SpsSmmWatchdogHeader == NULL) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] Input parameter is invalid\n"));
    ASSERT (FALSE);
    return;
  }

  Status = gSmst->SmmLocateProtocol (&gHeciControlSmmProtocolGuid, NULL, &pHeciPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Cannot locate SMM HECI protocol (status: %r)\n", Status));
    SpsSmmWatchdogHeader->Status = EFI_DEVICE_ERROR;
    return;
  }

  //construct Heci request
  SmmHeciMsg.Req.Command    = SPS_CMD_BIOS_UPDATE_WATCHDOG_CONTROL_REQ;
  SmmHeciMsg.Req.Nonce      = gNonceValue;
  SmmHeciMsg.Req.ActionFlag = SpsSmmWatchdogHeader->ActionFlag;
  ReqLen = sizeof (SmmHeciMsg.Req);
  RspLen = sizeof (SmmHeciMsg.Rsp);


  Status = pHeciPtr->HeciSendAndReceive (
                       pHeciPtr,
                       HECI_DEFAULT_PCH_ID,
                       NULL,
                       HECI_DEFAULT_RETRY_NUMBER,
                       SPS_CLIENTID_BIOS,
                       SPS_CLIENTID_ME_WATCHDOG_SERVICES,
                       (UINT32*) &(SmmHeciMsg.Req),
                       ReqLen,
                       (UINT32*) &(SmmHeciMsg.Rsp),
                       &RspLen,
                       NULL,
                       NULL
                       );

  if (EFI_ERROR (Status)) {
    DEBUG (
      (DEBUG_ERROR,
      "[SPS_SMM] ERROR: Cannot send SPS_CMD_BIOS_UPDATE_WATCHDOG_CONTROL_REQ (%r)\n",
      Status
      ));
    SpsSmmWatchdogHeader->Status = Status;
  } else if (SmmHeciMsg.Rsp.Command != SPS_CMD_BIOS_UPDATE_WATCHDOG_CONTROL_RSP) {
    DEBUG (
      (DEBUG_ERROR,
      "[SPS_SMM] ERROR: SPS_CMD_BIOS_UPDATE_WATCHDOG_CONTROL response failed\n"
      ));
    SpsSmmWatchdogHeader->Status = EFI_UNSUPPORTED;
    SpsSmmWatchdogHeader->ReturnStatus = SmmHeciMsg.Rsp.Status;
  } else if (SmmHeciMsg.Rsp.Status != HECI_RSP_STATUS_SUCCESS) {
    DEBUG (
      (DEBUG_ERROR,
      "[SPS_SMM] ERROR: SPS_CMD_BIOS_UPDATE_WATCHDOG_CONTROL failed (status: %d)\n",
      SmmHeciMsg.Rsp.Status
      ));
    SpsSmmWatchdogHeader->Status = EFI_UNSUPPORTED;
    SpsSmmWatchdogHeader->ReturnStatus = SmmHeciMsg.Rsp.Status;
  } else {
    //no error, status OK
    DEBUG ((DEBUG_INFO, "[SPS_SMM] SPS_CMD_BIOS_UPDATE_WATCHDOG_CONTROL reports success\n"));
    SpsSmmWatchdogHeader->Status = EFI_SUCCESS;
    SpsSmmWatchdogHeader->ReturnStatus = SmmHeciMsg.Rsp.Status;
  }
}

 /**
  Communication service SMI Handler entry.

  This SMI handler provides services for the sending Watchdog Control messages
  via SMM to allow Nonce usage.

  Caution: This function may receive untrusted input.

  @param[in]     DispatchHandle  The unique handle assigned to this handler by SmiHandlerRegister().
  @param[in]     RegisterContext Points to an optional handler context which was specified when the
                                 handler was registered.
  @param[in, out] CommBuffer     A pointer to a collection of data in memory that will
                                 be conveyed from a non-SMM environment into an SMM environment.
  @param[in, out] CommBufferSize The size of the CommBuffer.

  @retval EFI_SUCCESS                         The interrupt was handled and quiesced. No other handlers
                                              should still be called.
**/
EFI_STATUS
EFIAPI
SmmWatchdogHandler (
  IN     EFI_HANDLE       DispatchHandle,
  IN     CONST VOID       *RegisterContext,
  IN OUT VOID             *CommBuffer,
  IN OUT UINTN            *CommBufferSize
  )
{
  EFI_STATUS               Status;
  SPS_SMM_WATCHDOG_HEADER  *SpsSmmWatchdogHeader;

  DEBUG ((DEBUG_INFO, "[SPS_SMM] %s has been hit\n", __FUNCTION__));
  //
  // If input is invalid, stop processing this SMI
  //
  if ((CommBuffer == NULL) || (CommBufferSize == NULL) || (*CommBufferSize != sizeof (SPS_SMM_WATCHDOG_HEADER))) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: %a: Invalid input\n", __FUNCTION__));
    ASSERT ((CommBuffer != NULL) && (CommBufferSize != NULL) && (*CommBufferSize == sizeof (SPS_SMM_WATCHDOG_HEADER)));
    return EFI_SUCCESS;
  }

  Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof (SPS_SMM_WATCHDOG_HEADER), (VOID**)&SpsSmmWatchdogHeader);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Cannot allocate memory (status: %r)\n", Status));
    ASSERT_EFI_ERROR (Status);
    return EFI_SUCCESS;
  }
  CopyMem (SpsSmmWatchdogHeader, CommBuffer, sizeof (SPS_SMM_WATCHDOG_HEADER));

//  SpsSmmWatchdogHeader = (SPS_SMM_WATCHDOG_HEADER*)CommBuffer;
  MeWatchdogControlInSmm (SpsSmmWatchdogHeader);

  gSmst->SmmFreePool (SpsSmmWatchdogHeader);

  DEBUG ((DEBUG_INFO, "[SPS_SMM] %a exit\n", __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  Initialize SMM Watchdog handler

  @retval EFI_SUCCESS            Variable service successfully initialized.
  @retval EFI_INVALID_PARAMETER  Handler or DispatchHandle is NULL.

**/
EFI_STATUS
EFIAPI
MeWatchdogLibInitialize (
  VOID
  )
{
  EFI_STATUS    Status;
  EFI_HANDLE    WatchdogHandle;

  //
  // Register Sps SMM Watchdog SMI handler
  //
  WatchdogHandle = NULL;
  Status = gSmst->SmiHandlerRegister (SmmWatchdogHandler, &gSpsSmmWatchdogGuid, &WatchdogHandle);
  DEBUG ((DEBUG_INFO, "[SPS_SMM] DualBios Watchdog initialize result %r.\n", Status));
  ASSERT_EFI_ERROR (Status);

  return Status;
}
