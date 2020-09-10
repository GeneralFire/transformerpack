/** @file
  MeSmmLib.c

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

#include <Uefi.h>
#include <Library/MeUtilsLib.h>
#include "Library/DebugLib.h"
#include "Library/BaseLib.h"
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/MeSmmLib.h>
#include "MeState.h"
#include <MkhiMsgs.h>
#include <CoreBiosMsg.h>
#include <Protocol/HeciProtocol.h>
#include <Protocol/HeciControlProtocol.h>

/**
  Send Get Firmware Version Request to ME

  @param[in, out] MsgGenGetFwVersionAckData   Return the message of FW version

  @exception EFI_UNSUPPORTED        Current ME mode doesn't support this function
  @retval    EFI_SUCCESS            Command succeeded
  @retval    EFI_DEVICE_ERROR       HECI Device error, command aborts abnormally
  @retval    EFI_TIMEOUT            HECI does not return the buffer before timeout
  @retval    EFI_BUFFER_TOO_SMALL   Message Buffer is too small for the Acknowledge
  @retval    EFI_INVALID_PARAMETER  Passed NULL pointer to function
**/
EFI_STATUS
GetMeFwVersionSmm (
  IN OUT GET_FW_VER_ACK_DATA  *MsgGenGetFwVersionAckData
  )
{
  EFI_STATUS                      Status;
  GET_FW_VER_ACK                  MsgGenGetFwVersionAck;
  GET_FW_VER                      MsgGenGetFwVersion;
  UINT32                          Length;
  UINT32                          RecvLength;
  HECI_PROTOCOL                   *HeciLegacy;
  HECI_CONTROL                    *HeciControl;
  UINT32                          MeMode;

  DEBUG ((DEBUG_INFO, "[ME] %a.\n", __FUNCTION__));

  if (MsgGenGetFwVersionAckData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  HeciLegacy = NULL;
  HeciControl = NULL;

  Status = gSmst->SmmLocateProtocol (
                    &gHeciControlSmmProtocolGuid,
                    NULL,
                    &HeciControl
                    );
  if (EFI_ERROR (Status) || (HeciControl == NULL)) {
    Status = gSmst->SmmLocateProtocol (
                      &gSmmHeciProtocolGuid,
                      NULL,
                      &HeciLegacy
                      );
  }
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[ME] Unable to Locate Heci Protocol.- %r\n", Status));
    return Status;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    DEBUG ((DEBUG_ERROR, "[ME] %a: unsupported! (status: %r - ME mode: %d)\n", __FUNCTION__, Status, MeMode));
    return EFI_UNSUPPORTED;
  }
  ///
  /// Initialize MsgGenGetFwVersion data structure
  ///
  MsgGenGetFwVersion.MkhiHeader.Data              = 0;
  MsgGenGetFwVersion.MkhiHeader.Fields.GroupId    = MKHI_GEN_GROUP_ID;
  MsgGenGetFwVersion.MkhiHeader.Fields.Command    = GEN_GET_FW_VERSION_CMD;
  MsgGenGetFwVersion.MkhiHeader.Fields.IsResponse = 0;
  Length                                          = sizeof (GET_FW_VER);
  RecvLength                                      = sizeof (GET_FW_VER_ACK);
  ///
  /// Send Get Firmware Version Request to ME
  ///
  if (HeciControl != NULL) {
    Status = HeciControl->HeciSendAndReceive (
                            HeciControl,
                            HECI_DEFAULT_PCH_ID,
                            NULL,
                            HECI_DEFAULT_RETRY_NUMBER,
                            BIOS_FIXED_HOST_ADDR,
                            HECI_MKHI_MESSAGE_ADDR  ,
                            (UINT32*) &MsgGenGetFwVersion,
                            Length,
                            (UINT32*) &MsgGenGetFwVersionAck,
                            &RecvLength,
                            NULL,
                            NULL
                            );
  } else {
    MsgGenGetFwVersionAck.MkhiHeader.Data              = MsgGenGetFwVersion.MkhiHeader.Data;
    MsgGenGetFwVersionAck.MkhiHeader.Fields.GroupId    = MsgGenGetFwVersion.MkhiHeader.Fields.GroupId;
    MsgGenGetFwVersionAck.MkhiHeader.Fields.Command    = MsgGenGetFwVersion.MkhiHeader.Fields.Command;
    MsgGenGetFwVersionAck.MkhiHeader.Fields.IsResponse = 0;
    Status = HeciLegacy->SendwAck (
                           HECI1_DEVICE,
                           (UINT32*) &MsgGenGetFwVersionAck,
                           Length,
                           &RecvLength,
                           BIOS_FIXED_HOST_ADDR,
                           HECI_MKHI_MESSAGE_ADDR
                           );
  }

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[ME] %a: message failed! - %r\n", __FUNCTION__, Status));
    return Status;
  }

  if ((MsgGenGetFwVersionAck.MkhiHeader.Fields.Command != GEN_GET_FW_VERSION_CMD) ||
      (MsgGenGetFwVersionAck.MkhiHeader.Fields.GroupId != MKHI_GEN_GROUP_ID) ||
      (MsgGenGetFwVersionAck.MkhiHeader.Fields.IsResponse != 1) ||
      (MsgGenGetFwVersionAck.MkhiHeader.Fields.Result != 0)) {
    DEBUG ((
      DEBUG_ERROR,
      "[ME] Invalid response received for GET_FW_VERSION (MKHI: %08X)\n",
      MsgGenGetFwVersionAck.MkhiHeader.Data));
    return EFI_DEVICE_ERROR;
  }

  *MsgGenGetFwVersionAckData = MsgGenGetFwVersionAck.Data;

  DEBUG ((
    DEBUG_INFO,
    "[ME] Firmware version is:\n"
    "\tOperational: %02X:%d.%d.%d.%d\n"
    "\tRecovery:    %02X:%d.%d.%d.%d\n",
    (MsgGenGetFwVersionAck.Data.CodeMajor >> 8),
    (MsgGenGetFwVersionAck.Data.CodeMajor & 0x00FF),
    MsgGenGetFwVersionAck.Data.CodeMinor,
    MsgGenGetFwVersionAck.Data.CodeHotFix,
    MsgGenGetFwVersionAck.Data.CodeBuildNo,
    (MsgGenGetFwVersionAck.Data.RcvyMajor >> 8),
    (MsgGenGetFwVersionAck.Data.RcvyMajor & 0x00FF),
    MsgGenGetFwVersionAck.Data.RcvyMinor,
    MsgGenGetFwVersionAck.Data.RcvyHotFix,
    MsgGenGetFwVersionAck.Data.RcvyBuildNo
    ));


  DEBUG ((DEBUG_INFO, "[ME] %a: exit (%r)\n", __FUNCTION__, Status));

  return Status;
}
