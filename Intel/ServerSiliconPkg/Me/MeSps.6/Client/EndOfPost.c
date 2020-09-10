/** @file
  ME End Of Post message and process implementation prior to boot OS

@copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2019 Intel Corporation.

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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Private/Library/PmcPrivateLib.h>
#include <Protocol/HeciControlProtocol.h>
#include <MkhiMsgs.h>
#include <MeState.h>
#include <CoreBiosMsg.h>
#include <Register/HeciRegs.h>
#include <MeiBusMsg.h>
#include <Library/MeUtilsLib.h>

/**
  Send End of Post Request Message through HECI.

  @param[out] RequestedActions    Action request returned by EOP ACK
                                    0x00 (HECI_EOP_STATUS_SUCCESS) - Continue to boot
                                    0x01 (HECI_EOP_PERFORM_GLOBAL_RESET) - Global reset

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
HeciSendEndOfPostMessage (
  OUT UINT32                      *RequestedActions
  )
{
  EFI_STATUS         Status;
  UINT32             Length;
  UINT32             RecvLength;
  END_OF_POST_BUFFER EndOfPost;
  HECI_CONTROL       *Heci;

  ZeroMem (&EndOfPost, sizeof (END_OF_POST_BUFFER));

  EndOfPost.Request.MkhiHeader.Fields.Command = GEN_END_OF_POST_CMD;
  EndOfPost.Request.MkhiHeader.Fields.GroupId = MKHI_GEN_GROUP_ID;

  Length                                  = sizeof (END_OF_POST);
  RecvLength                              = sizeof (END_OF_POST_ACK);

  Status = gBS->LocateProtocol (&gHeciControlProtocolGuid, NULL, (VOID **) &Heci);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () - cannot locate gHeciControlProtocolGuid\n", __FUNCTION__));
    return Status;
  }

  Status = Heci->HeciSendAndReceive (
                   Heci,
                   HECI_DEFAULT_PCH_ID,
                   NULL,
                   HECI_DEFAULT_RETRY_NUMBER,
                   BIOS_FIXED_HOST_ADDR,
                   HECI_MKHI_MESSAGE_ADDR,
                   (UINT32*) &(EndOfPost.Request),
                   Length,
                   (UINT32*) &(EndOfPost.Response),
                   &RecvLength,
                   NULL,
                   NULL
                   );

  if (!EFI_ERROR (Status)) {
    *RequestedActions = EndOfPost.Response.Data.RequestedActions;
    if (EndOfPost.Response.Data.RequestedActions == HeciEopPerformGlobalReset) {
      DEBUG ((DEBUG_INFO, "HeciSendEndOfPostMessage(): Global Reset requested by FW EOP ACK\n"));
    }
  }

  return Status;
}

/**
  This message is sent by the BIOS if EOP-ACK not received to force ME to disable
  HECI interfaces.

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             HECI interfaces disabled by ME
**/
EFI_STATUS
HeciDisableHeciBusMsg (
  VOID
  )
{
  EFI_STATUS                      Status;
  UINT32                          Length;
  UINT32                          RespLength;
  HECI_BUS_DISABLE_CMD            MsgHeciBusDisableRequest;
  HECI_BUS_DISABLE_CMD_ACK        MsgHeciBusDisableResponse;
  HECI_CONTROL                    *Heci;

  ZeroMem (&MsgHeciBusDisableRequest, sizeof (HECI_BUS_DISABLE_CMD));
  ZeroMem (&MsgHeciBusDisableResponse, sizeof (HECI_BUS_DISABLE_CMD_ACK));

  MsgHeciBusDisableRequest.Command.Data = HECI_BUS_DISABLE_OPCODE;
  Length                                = sizeof (HECI_BUS_DISABLE_CMD);
  RespLength                            = sizeof (HECI_BUS_DISABLE_CMD_ACK);

  Status = gBS->LocateProtocol (&gHeciControlProtocolGuid, NULL, (VOID **) &Heci);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () - cannot locate gHeciControlProtocolGuid\n", __FUNCTION__));
    return Status;
  }

  Status = Heci->HeciSendAndReceive (
                   Heci,
                   HECI_DEFAULT_PCH_ID,
                   NULL,
                   HECI_DEFAULT_RETRY_NUMBER,
                   BIOS_FIXED_HOST_ADDR,
                   HECI_HBM_MESSAGE_ADDR,
                   (UINT32*) &(MsgHeciBusDisableRequest),
                   Length,
                   (UINT32*) &(MsgHeciBusDisableResponse),
                   &RespLength,
                   NULL,
                   NULL
                   );

  if (!EFI_ERROR (Status) &&
      ((MsgHeciBusDisableResponse.Command.Fields.Command != HECI_BUS_DISABLE_OPCODE) ||
       (MsgHeciBusDisableResponse.Command.Fields.IsResponse == 0) ||
       (MsgHeciBusDisableResponse.Status != 0))) {
    Status = EFI_ABORTED;
  }

  return Status;
}

/**
  Send ME the BIOS end of Post message.

  @param[out] RequestedActions    Action request returned by EOP ACK
                                    0x00 (HECI_EOP_STATUS_SUCCESS) - Continue to boot
                                    0x01 (HECI_EOP_PERFORM_GLOBAL_RESET) - Global reset

  @retval EFI_SUCCESS             Platform reached End of Post successfully
  @retval EFI_DEVICE_ERROR        An error has occured by EoP message
**/
EFI_STATUS
MeEndOfPostEvent (
  OUT UINT32                          *RequestedActions
  )
{
  EFI_STATUS                          Status;
  UINT8                               EopSendRetries;

  DEBUG ((DEBUG_INFO, "ME-BIOS: EOP Entry.\n"));

  for (EopSendRetries = 0; EopSendRetries < MAX_EOP_SEND_RETRIES; EopSendRetries++) {
    Status = HeciSendEndOfPostMessage (RequestedActions);
    if (!EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "ME-BIOS: EOP Exit - Success.\n"));
      return EFI_SUCCESS;
    }
  }

  HeciDisableHeciBusMsg ();
  DEBUG ((DEBUG_ERROR, "ME-BIOS: EOP Exit - Error by sending EOP message. Forcing HECI interface closure\n"));

  return EFI_DEVICE_ERROR;
}

/**
  Perform Cf9Gr Lock Config
      - PCH BIOS Spec Rev 0.9 Section 18.4  Additional Power Management Programming
        Set "Power Management Initialization Register (PMIR) Field 1", D31:F0:ACh[31] = 1b
        for production machine according to "RS - PCH Intel Management Engine
        (Intel(r) ME) BIOS Writer's Guide".
**/
VOID
LockConfig (
  VOID
  )
{
  EFI_STATUS                      Status;
  UINT32                          MeMode;
  HECI_FWS_REGISTER               MeFirmwareStatus;
  HECI_CONTROL                    *Heci;

  DEBUG ((DEBUG_INFO, "%a () - start\n", __FUNCTION__));

  Status = gBS->LocateProtocol (&gHeciControlProtocolGuid, NULL, (VOID **) &Heci);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () - cannot locate gHeciControlProtocolGuid\n", __FUNCTION__));
    goto LockConfigExit;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () - cannot get MeMode\n", __FUNCTION__));
    ASSERT_EFI_ERROR (Status);
    goto LockConfigExit;
  }
  Status = Heci->HeciFirmwareStatus (Heci, HECI_DEFAULT_PCH_ID, 1, &MeFirmwareStatus.ul);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () - cannot get MeFirmwareStatus\n", __FUNCTION__));
    ASSERT_EFI_ERROR (Status);
    goto LockConfigExit;
  }

  ///
  /// PCH BIOS Spec Rev 0.9 Section 18.4  Additional Power Management Programming
  /// Step 2
  ///   Set "Power Management Initialization Register (PMIR) Field 1", D31:F0:ACh[31] = 1b
  ///   for production machine according to "RS - PCH Intel Management Engine
  ///  (Intel(r) ME) BIOS Writer's Guide".
  ///
  /// PCH ME BWG section 3.6.2
  /// The IntelR FPT tool /GRST option uses CF9GR bit to trigger global reset.
  /// Based on above reason, the BIOS should not lock down CF9GR bit during Manufacturing and
  /// Re-manufacturing environment if HFSTS1 [4] is set.
  ///
  if (((MeMode == ME_MODE_NORMAL) || (MeMode == ME_MODE_TEMP_DISABLED)) && !(MeFirmwareStatus.r.SpiProtectionMode)) {
    ///
    /// PCH ME BWG section 4.4.1
    /// BIOS must also ensure that CF9GR is cleared and locked (via bit31 of the same register) before
    /// handing control to the OS in order to prevent the host from issuing global resets and reseting
    /// Intel Management Engine.
    ///
    PmcDisableCf9GlobalResetWithLock ();
  } else {
    PmcDisableCf9GlobalReset ();
  }

LockConfigExit:
  DEBUG ((DEBUG_INFO, "%a () - end, status: %r\n", __FUNCTION__, Status));
}

