/** @file
  HECI Control protocol driver filtering functionality for Sps ME (Smm)

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

#include <PiDxe.h>
#include <Base.h>
#include <Uefi.h>
#include <Sps.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <ConfigBlock/SpsGetDxeConfigBlock.h>

#define HECI_CONTROL_DEBUG   "[HECI Control %s]"

STATIC BOOLEAN mMeHmrfpoViaHeci3 = FALSE;

/**
  Get from configuration if HMRFPO messages has been send via HECI3


  @retval TRUE      Use HECI 3 for HMRFPO messages
  @retval FALSE     Use HECI 1 for HMRFPO messages
**/
BOOLEAN
GetMeHmrfpoViaHeci3 (
  VOID
  )
{
  return mMeHmrfpoViaHeci3;
}

/**
  Callback fuction upon DxeSpsPolicyProtocol to read if HMRFPO messages
  should be sent via HECI-1 or HECI-3.

  @param[in]  Event     Event whose notification function is being invoked
  @param[in]  Context   Pointer to the notification function's context
**/
VOID
EFIAPI
EventDxeSpsPolicyProtocolNotify (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS      Status;
  VOID            *Interface;
  SPS_DXE_CONFIG  *SpsDxeConfig;

  Status = gBS->LocateProtocol (
                  &gDxeSpsPolicyProtocolGuid,
                  NULL,
                  (VOID**) &Interface
                  );

  if (!EFI_ERROR (Status)) {
    SpsDxeConfig = GetSpsDxeConfigBlock ();
    if (SpsDxeConfig != NULL) {
      mMeHmrfpoViaHeci3 = (UINT8) SpsDxeConfig->MeHmrfpoViaHeci3;
    }
    gBS->CloseEvent (Event);
  }
}

/**
  The constructor function for HECI Control Filter SPS SMM Library.

  @retval EFI_SUCCESS   The function always return EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
HeciControlFilterLibConstructor (
  VOID
  )
{
// APTIOV_SERVER_OVERRIDE_RC_START : Power on override for Exception
// EIP547597
/*  VOID *Registration;

  EfiCreateProtocolNotifyEvent (
    &gDxeSpsPolicyProtocolGuid,
    TPL_CALLBACK,
    EventDxeSpsPolicyProtocolNotify,
    NULL,
    &Registration
    );
*/
// APTIOV_SERVER_OVERRIDE_RC_END : Power on override for Exception

  return EFI_SUCCESS;
}