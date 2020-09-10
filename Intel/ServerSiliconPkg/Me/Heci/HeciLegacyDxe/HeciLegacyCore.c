/** @file
  Heci legacy driver core. For Dxe Phase

 @copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2019 - 2020 Intel Corporation.

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

**/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/HeciControlProtocol.h>

#include <Library/MeEwlLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/MeTypeLib.h>

#include "MeAccess.h"
#include "HeciLegacyCore.h"
#include "MeState.h"
#include <Register/HeciRegs.h>


/**
  Determines if the HECI device is present and, if present, initializes it for
  use by the BIOS.

  @param [in] HeciDev             The HECI device to be accessed.

  @retval EFI_SUCCESS             The function completed successfully.
**/
EFI_STATUS
EFIAPI
HeciInitialize (
  IN HECI_DEVICE              HeciDev
  )
{
  ASSERT (HeciDev == HECI1_DEVICE);

  return EFI_SUCCESS;
}


/**
  Heci Re-initializes it for Host

  @param [in] HeciDev         The HECI device to be accessed.

  @retval EFI_UNSUPPORTED     Unsupported in new HECI driver model
**/
EFI_STATUS
EFIAPI
HeciReInitialize(
  IN HECI_DEVICE              HeciDev
  )
{
  return EFI_UNSUPPORTED;
}


/**
  Reads a message from the ME across HECI. This function can only be used after invoking HeciSend() first.

  @param [in]     HeciDev         The HECI device to be accessed.
  @param [in]     Blocking        Used to determine if the read is BLOCKING or NON_BLOCKING.
  @param[in, out] MessageBody     Pointer to a buffer used to receive a message.
  @param[in, out] Length          Pointer to the length of the buffer on input and the length
                                  of the message on return. (in bytes)

  @retval EFI_SUCCESS             One message packet read.
  @retval EFI_DEVICE_ERROR        Failed to initialize HECI or zero-length message packet read
  @retval EFI_TIMEOUT             HECI is not ready for communication
  @retval EFI_BUFFER_TOO_SMALL    The caller's buffer was not large enough
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this message through this HECI
**/
EFI_STATUS
EFIAPI
HeciReceive (
  IN      HECI_DEVICE  HeciDev,
  IN      UINT32       Blocking,
  IN OUT  UINT32       *MessageBody,
  IN OUT  UINT32       *Length
  )
{
  HECI_CONTROL             *HeciProtocol;
  EFI_STATUS               Status;

  ASSERT (HeciDev == HECI1_DEVICE);

  Status = gBS->LocateProtocol (
                  &gHeciControlProtocolGuid,
                  NULL,
                  &HeciProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "[HECI Legacy] ERROR: Cannot locate HECI protocol (%r)\n",
      Status
      ));
    return Status;
  }

  Status = HeciProtocol->HeciReceive (
                           HeciProtocol,
                           HECI_DEFAULT_PCH_ID,
                           NULL,
                           NULL,
                           NULL,
                           (UINT32*) MessageBody,
                           Length
                           );

  return Status;
}

/**
  Function sends one message (of any length) through the HECI circular buffer.

  @param [in] HeciDev             The HECI device to be accessed.
  @param [in] Message             Pointer to the message data to be sent.
  @param [in] Length              Length of the message in bytes.
  @param [in] HostAddress         The address of the Host processor.
  @param [in] MeAddress           Address of the ME subsystem the message is being sent to.

  @retval EFI_SUCCESS             One message packet sent.
  @retval EFI_DEVICE_ERROR        Failed to initialize HECI
  @retval EFI_TIMEOUT             HECI is not ready for communication
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support send this message through this HECI
**/
EFI_STATUS
EFIAPI
HeciSend (
  IN HECI_DEVICE  HeciDev,
  IN UINT32       *Message,
  IN UINT32       Length,
  IN UINT8        HostAddress,
  IN UINT8        MeAddress
  )
{
  HECI_CONTROL             *HeciProtocol;
  EFI_STATUS               Status;

  ASSERT (HeciDev == HECI1_DEVICE);

  Status = gBS->LocateProtocol (
                  &gHeciControlProtocolGuid,
                  NULL,
                  &HeciProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "[HECI Legacy] ERROR: Cannot locate HECI protocol (%r)\n",
      Status
      ));
    return Status;
  }

  Status = HeciProtocol->HeciSend (
                           HeciProtocol,
                           HECI_DEFAULT_PCH_ID,
                           NULL,
                           HostAddress,
                           MeAddress,
                           (UINT32*) Message,
                           Length
                           );

  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "[HECI] Send msg %02X -> %02X failed (%r)\n",
      HostAddress,
      MeAddress,
      Status
      ));
  }

  return Status;
}

/**
  Function sends one message through the HECI circular buffer and waits
  for the corresponding ACK message.

  @param [in]      HeciDev        The HECI device to be accessed.
  @param [in, out] Message        Pointer to the message buffer.
  @param [in]      Length         Length of the message in bytes.
  @param [in, out] RecLength      Length of the message response in bytes.
  @param [in]      HostAddress    Address of the sending entity.
  @param [in]      MeAddress      Address of the ME entity that should receive the message.

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support send this message through this HECI
**/
EFI_STATUS
EFIAPI
HeciSendwAck (
  IN      HECI_DEVICE  HeciDev,
  IN OUT  UINT32       *Message,
  IN      UINT32       Length,
  IN OUT  UINT32       *RecLength,
  IN      UINT8        HostAddress,
  IN      UINT8        MeAddress
  )
{
  HECI_CONTROL             *HeciProtocol;
  EFI_STATUS               Status;

  ASSERT (HeciDev == HECI1_DEVICE);

  Status = gBS->LocateProtocol (
                  &gHeciControlProtocolGuid,
                  NULL,
                  &HeciProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "[HECI Legacy] ERROR: Cannot locate HECI protocol (%r)\n",
      Status
      ));

    return Status;
  }

  Status = HeciProtocol->HeciSendAndReceive (
                           HeciProtocol,
                           HECI_DEFAULT_PCH_ID,
                           NULL,
                           HECI_DEFAULT_RETRY_NUMBER,
                           HostAddress,
                           MeAddress,
                           (UINT32*) Message,
                           Length,
                           (UINT32*) Message,
                           RecLength,
                           NULL,
                           NULL
                           );

  return Status;
}

/**
  Function forces a reinit of the heci interface by following the reset heci interface via Host algorithm

  @param [in] HeciDev             The HECI device to be accessed.

  @retval EFI_TIMEOUT             ME is not ready
  @retval EFI_SUCCESS             Interface reset
**/
EFI_STATUS
EFIAPI
HeciResetInterface(
  IN  HECI_DEVICE             HeciDev)
{
  HECI_CONTROL             *HeciProtocol;
  EFI_STATUS               Status;

  ASSERT (HeciDev == HECI1_DEVICE);

  Status = gBS->LocateProtocol (&gHeciControlProtocolGuid, NULL, &HeciProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "[HECI Legacy] ERROR: Cannot locate HECI protocol (%r)\n",
      Status
      ));
    return Status;
  }

  Status = HeciProtocol->HeciReset (
                           HeciProtocol,
                           HECI_DEFAULT_PCH_ID,
                           NULL
                           );

  return Status;
}

/**
  Return ME Status

  @param MeStatus pointer for status report

  @retval EFI_STATUS
**/
EFI_STATUS
EFIAPI
HeciGetMeStatus (
  IN UINT32                     *MeStatus
  )
{
  HECI_FWS_REGISTER          MeFirmwareStatus;
  HECI_CONTROL               *HeciProtocol;
  EFI_STATUS                 Status;

  if (MeStatus == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateProtocol (
                  &gHeciControlProtocolGuid,
                  NULL,
                  &HeciProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "[HECI Legacy] ERROR: Cannot locate HECI protocol (%r)\n",
      Status
      ));
    return Status;
  }

  Status = HeciProtocol->HeciFirmwareStatus (
                           HeciProtocol,
                           HECI_DEFAULT_PCH_ID,
                           1,
                           &(MeFirmwareStatus.ul)
                           );

  if (EFI_ERROR (Status) || MeTypeIsDfx ()) {
    *MeStatus = ME_NOT_READY;
  } else {
    switch (MeFirmwareStatus.r.CurrentState) {
    case ME_STATE_NORMAL:
      if (MeFirmwareStatus.r.ErrorCode == ME_ERROR_CODE_NO_ERROR) {
        *MeStatus = ME_READY;
      }
      break;
    case ME_STATE_RECOVERY:
      *MeStatus = ME_IN_RECOVERY_MODE;
      break;
    case ME_STATE_INIT:
      *MeStatus = ME_INITIALIZING;
      break;
    case ME_STATE_DISABLE_WAIT:
      *MeStatus = ME_DISABLE_WAIT;
      break;
    case ME_STATE_TRANSITION:
      *MeStatus = ME_TRANSITION;
      break;
    default:
      *MeStatus = ME_NOT_READY;
      break;
    }
  }

  if (MeFirmwareStatus.r.FwUpdateInprogress) {
    *MeStatus |= ME_FW_UPDATES_IN_PROGRESS;
  }

  if (MeFirmwareStatus.r.FwInitComplete == ME_FIRMWARE_COMPLETED) {
    *MeStatus |= ME_FW_INIT_COMPLETE;
  }

  if (MeFirmwareStatus.r.MeBootOptionsPresent == ME_BOOT_OPTIONS_PRESENT) {
    *MeStatus |= ME_FW_BOOT_OPTIONS_PRESENT;
  }

  DEBUG ((
    DEBUG_INFO,
    "[HECI] MEFS1 %08X -> MeStatus %X\n",
    MeFirmwareStatus.ul,
    *MeStatus
    ));

  return EFI_SUCCESS;
}


/**
  Return ME Mode

  @param [out] MeMode             Pointer for ME Mode report

  @retval EFI_SUCCESS             MeMode copied
  @retval EFI_INVALID_PARAMETER   Pointer of MeMode is invalid
**/
EFI_STATUS
EFIAPI
HeciGetMeMode (
  OUT UINT32                    *MeMode
  )
{
  UINT32                   MeFirmwareStatus;
  UINT32                   MeModeLocal;
  HECI_CONTROL             *HeciProtocol;
  EFI_STATUS               Status;

  if (MeMode == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateProtocol (
                  &gHeciControlProtocolGuid,
                  NULL,
                  &HeciProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "[HECI Legacy] ERROR: Cannot locate HECI protocol (%r)\n",
      Status
      ));
    return Status;
  }

  Status = HeciProtocol->HeciFirmwareStatus (
                            HeciProtocol,
                            HECI_DEFAULT_PCH_ID,
                            1,
                            &MeFirmwareStatus
                            );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "[HECI Legacy] ERROR: Cannot locate HECI protocol (%r)\n",
      Status
      ));
    return Status;
  }

  //
  // Check if it's DFX ME FW first
  //
  if (MeTypeIsDfx ()) {
    *MeMode = ME_NOT_READY;
  } else {
    Status = MeBiosGetMeMode (&MeModeLocal);
    if (!EFI_ERROR (Status)) {
      *MeMode = MeModeLocal;
    }
  }

  DEBUG ((
    DEBUG_INFO,
    "[HECI] MEFS1: %08X -> MeMode %d\n",
    MeFirmwareStatus,
    *MeMode
    ));

  return Status;
}

/**
  @param [in] Delay               The biggest waiting time

  @retval EFI_TIMEOUT             ME is not ready
  @retval EFI_SUCCESS             Me is ready
**/
EFI_STATUS
EFIAPI
HeciMeResetWait (
  IN  HECI_DEVICE                 HeciDev,
  IN  UINT32                      Delay
  )
{
  return EFI_UNSUPPORTED;
}