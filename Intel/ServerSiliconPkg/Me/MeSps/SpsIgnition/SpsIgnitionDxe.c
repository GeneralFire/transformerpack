/** @file
  This driver implements the DXE phase of SPS Ignition support.

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

#include <Base.h>
#include <PiDxe.h>
#include <Uefi.h>

#include <Library/UefiBootServicesTableLib.h>

#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/MeTypeLib.h>

#include "SpsIgnition.h"


/**
  This function gets registered as a EndOfDxe callback to indicate CORE_BIOS_DONE.

  @param[in] Event                The event that triggered this callback
  @param[in] Context              Pointer to the callback context
**/
VOID
EFIAPI
IgnitionEndOfDxeCallback (
  IN EFI_EVENT                    Event,
  IN VOID                         *Context
  )
{
  DEBUG ((DEBUG_INFO, "[%a]\n", __FUNCTION__));

  IgnitionIndicateCoreBiosDone ();

  gBS->CloseEvent (Event);
}

/**
  This function gets registered as a ReadyToBoot callback to indicate END_OF_POST.

  @param[in] Event                The event that triggered this callback
  @param[in] Context              Pointer to the callback context
**/
VOID
EFIAPI
IgnitionReadyToBootCallback (
  IN  EFI_EVENT                   Event,
  IN  VOID                        *Context
  )
{
  DEBUG ((DEBUG_INFO, "[%a]\n", __FUNCTION__));

  IgnitionIndicateEndOfPost ();

  gBS->CloseEvent (Event);
}

/**
  SPS Ignition DXE entry point.

  @param[in] ImageHandle          DXE image file handle
  @param[in] pSystemTable         Pointer to system table

  @retval EFI_SUCCESS             The function completed successfully.
**/
EFI_STATUS
EFIAPI
IgnitionDxeEntryPoint (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *pSystemTable
  )
{
  EFI_STATUS                      Status;
  EFI_EVENT                       EndOfDxeEvent;
  EFI_EVENT                       ReadyToBootEvent;

  DEBUG ((DEBUG_INFO, "[%a] ", __FUNCTION__));

  if (MeTypeIsSpsIgnition ()) {
    DEBUG ((DEBUG_INFO, "SPS Ignition\n"));
  } else {
    DEBUG ((DEBUG_INFO, "not SPS Ignition\n"));
    return EFI_SUCCESS;
  }

  IgnitionClearDxeIndicators (); // In case they have not been cleared in PEI

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  IgnitionEndOfDxeCallback,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "[%a] Cannot install EndOfDxe event callback\n",
      __FUNCTION__
      ));
    return Status;
  }

  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             IgnitionReadyToBootCallback,
             NULL,
             &ReadyToBootEvent
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "[%a] Cannot install ReadyToBoot event callback\n",
      __FUNCTION__
      ));
    return Status;
  }

  return EFI_SUCCESS;
}
