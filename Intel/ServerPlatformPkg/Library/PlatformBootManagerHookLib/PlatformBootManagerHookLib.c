/** @file
  Platform Boot Manager Hook Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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
#include <Uefi.h>

#include <Library/DebugLib.h>
#include <Library/PlatformBootManagerHookLib.h>
#include <Library/UefiBootServicesTableLib.h>

/**
  An empty function to pass error checking of CreateEventEx ().

  This empty function ensures that EVT_NOTIFY_SIGNAL_ALL is error
  checked correctly since it is now mapped into CreateEventEx() in UEFI 2.0.

  @param  Event                 Event whose notification function is being invoked.
  @param  Context               The pointer to the notification function's context,
                                which is implementation-dependent.

**/
VOID
EFIAPI
InternalNullFunction (
  IN EFI_EVENT                Event,
  IN VOID                     *Context
  )
{
  return;
}

/**
  Create an EFI event for boot manager pause.

  @param  BootManagerPauseEvent  Returns the EFI event returned from gBS->CreateEvent(Ex).

  @retval EFI_SUCCESS       Event was created.
  @retval Other             Event was not created.

**/
EFI_STATUS
EFIAPI
CreateEventBootManagerPause (
  OUT EFI_EVENT  *BootManagerPauseEvent
  )
{
  return CreateEventBootManagerPauseEx (
           TPL_CALLBACK,
           InternalNullFunction,
           NULL,
           BootManagerPauseEvent
           );
}

/**
  Create an EFI event for boot manager pause and allows
  the caller to specify a notification function.

  @param  NotifyTpl         The task priority level of the event.
  @param  NotifyFunction    The notification function to call when the event is signaled.
  @param  NotifyContext     The content to pass to NotifyFunction when the event is signaled.
  @param  BootManagerPauseEvent  Returns the EFI event returned from gBS->CreateEvent(Ex).

  @retval EFI_SUCCESS       Event was created.
  @retval Other             Event was not created.

**/
EFI_STATUS
EFIAPI
CreateEventBootManagerPauseEx (
  IN  EFI_TPL           NotifyTpl,
  IN  EFI_EVENT_NOTIFY  NotifyFunction,  OPTIONAL
  IN  VOID              *NotifyContext,  OPTIONAL
  OUT EFI_EVENT         *BootManagerPauseEvent
  )
{
  EFI_STATUS    Status;

  if (BootManagerPauseEvent == NULL) {
    ASSERT (BootManagerPauseEvent != NULL);
    return EFI_INVALID_PARAMETER;
  }

  if (gST->Hdr.Revision < EFI_2_00_SYSTEM_TABLE_REVISION) {
    DEBUG ((EFI_D_ERROR, "EFI1.1 can't support ReadyToBootEvent!"));
    ASSERT (FALSE);

    return EFI_UNSUPPORTED;
  } else {
    //
    // For UEFI 2.0 and the future use an Event Group
    //
    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    NotifyTpl,
                    NotifyFunction,
                    NotifyContext,
                    &gEfiEventBootManagerPauseGuid,
                    BootManagerPauseEvent
                    );
  }

  return Status;
}

/**
  Signal the boot manager pause event by create a new boot manager pause event
  with gEfiEventBootManagerPauseGuid and then signal the event just created.
  After event signaled close this event.
**/
VOID
EFIAPI
SignalEventBootManagerPause (
  VOID
  )
{
  EFI_STATUS    Status;
  EFI_EVENT     BootManagerPauseEvent;

  Status = CreateEventBootManagerPause (&BootManagerPauseEvent);
  if (!EFI_ERROR (Status)) {
    gBS->SignalEvent (BootManagerPauseEvent);
    gBS->CloseEvent (BootManagerPauseEvent);
  }

}

/**
  Create an EFI event for boot manager resume.

  @param  BootManagerResumeEvent  Returns the EFI event returned from gBS->CreateEvent(Ex).

  @retval EFI_SUCCESS       Event was created.
  @retval Other             Event was not created.

**/
EFI_STATUS
EFIAPI
CreateEventBootManagerResume (
  OUT EFI_EVENT  *BootManagerResumeEvent
  )
{
  return CreateEventBootManagerResumeEx (
           TPL_CALLBACK,
           InternalNullFunction,
           NULL,
           BootManagerResumeEvent
           );
}

/**
  Create an EFI event for boot manager resume and allows
  the caller to specify a notification function.

  @param  NotifyTpl         The task priority level of the event.
  @param  NotifyFunction    The notification function to call when the event is signaled.
  @param  NotifyContext     The content to pass to NotifyFunction when the event is signaled.
  @param  BootManagerResumeEvent  Returns the EFI event returned from gBS->CreateEvent(Ex).

  @retval EFI_SUCCESS       Event was created.
  @retval Other             Event was not created.

**/
EFI_STATUS
EFIAPI
CreateEventBootManagerResumeEx (
  IN  EFI_TPL           NotifyTpl,
  IN  EFI_EVENT_NOTIFY  NotifyFunction,  OPTIONAL
  IN  VOID              *NotifyContext,  OPTIONAL
  OUT EFI_EVENT         *BootManagerResumeEvent
  )
{
  EFI_STATUS    Status;

  if (BootManagerResumeEvent == NULL) {
    ASSERT (BootManagerResumeEvent != NULL);
    return EFI_INVALID_PARAMETER;
  }

  if (gST->Hdr.Revision < EFI_2_00_SYSTEM_TABLE_REVISION) {
    DEBUG ((EFI_D_ERROR, "EFI1.1 not support!"));
    ASSERT (FALSE);

    return EFI_UNSUPPORTED;
  } else {
    //
    // For UEFI 2.0 and the future use an Event Group
    //
    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    NotifyTpl,
                    NotifyFunction,
                    NotifyContext,
                    &gEfiEventBootManagerResumeGuid,
                    BootManagerResumeEvent
                    );
  }

  return Status;
}

/**
  Signal the boot manager resume event by create a new boot manager resume event
  with gEfiEventBootManagerResumeGuid  and then signal the event just created.
  After event signaled close this event.
**/
VOID
EFIAPI
SignalEventBootManagerResume (
  VOID
  )
{
  EFI_STATUS    Status;
  EFI_EVENT     BootManagerResumeEvent;

  Status = CreateEventBootManagerResume (&BootManagerResumeEvent);
  if (!EFI_ERROR (Status)) {
    gBS->SignalEvent (BootManagerResumeEvent);
    gBS->CloseEvent (BootManagerResumeEvent);
  }

}

/**
  Create an EFI event for platform boot manager after console.

  @param  AfterConsoleEvent  Returns the EFI event returned from gBS->CreateEvent(Ex).

  @retval EFI_SUCCESS       Event was created.
  @retval Other             Event was not created.

**/
EFI_STATUS
EFIAPI
CreateEventAfterConsole (
  OUT EFI_EVENT  *AfterConsoleEvent
  )
{
  return CreateEventAfterConsoleEx (
           TPL_CALLBACK,
           InternalNullFunction,
           NULL,
           AfterConsoleEvent
           );
}

/**
  Create an EFI event for platform boot manager after console and allows
  the caller to specify a notification function.

  @param  NotifyTpl         The task priority level of the event.
  @param  NotifyFunction    The notification function to call when the event is signaled.
  @param  NotifyContext     The content to pass to NotifyFunction when the event is signaled.
  @param  AfterConsoleEvent  Returns the EFI event returned from gBS->CreateEvent(Ex).

  @retval EFI_SUCCESS       Event was created.
  @retval Other             Event was not created.

**/
EFI_STATUS
EFIAPI
CreateEventAfterConsoleEx (
  IN  EFI_TPL           NotifyTpl,
  IN  EFI_EVENT_NOTIFY  NotifyFunction,  OPTIONAL
  IN  VOID              *NotifyContext,  OPTIONAL
  OUT EFI_EVENT         *AfterConsoleEvent
  )
{
  EFI_STATUS    Status;

  if (AfterConsoleEvent == NULL) {
    ASSERT (AfterConsoleEvent != NULL);
    return EFI_INVALID_PARAMETER;
  }

  if (gST->Hdr.Revision < EFI_2_00_SYSTEM_TABLE_REVISION) {
    DEBUG ((EFI_D_ERROR, "EFI1.1 not support!"));
    ASSERT (FALSE);

    return EFI_UNSUPPORTED;
  } else {
    //
    // For UEFI 2.0 and the future use an Event Group
    //
    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    NotifyTpl,
                    NotifyFunction,
                    NotifyContext,
                    &gEfiEventPlatformBootManagerAfterConsoleGuid,
                    AfterConsoleEvent
                    );
  }

  return Status;
}

/**
  Signal the after console event by create a new after console event
  with gEfiEventPlatformBootManagerAfterConsoleGuid and then signal the event just created.
  After event signaled close this event.
**/
VOID
EFIAPI
SignalEventAfterConsole (
  VOID
  )
{
  EFI_STATUS    Status;
  EFI_EVENT     AfterConsoleEvent;

  Status = CreateEventAfterConsole (&AfterConsoleEvent);
  if (!EFI_ERROR (Status)) {
    gBS->SignalEvent (AfterConsoleEvent);
    gBS->CloseEvent (AfterConsoleEvent);
  }

}

/**
  Signal AfterTrustConsoleEvent event after trust console
**/
VOID
EFIAPI
SignalEventAfterTrustConsole (
  VOID
  )
{
  EFI_STATUS    Status;
  EFI_EVENT     AfterTrustConsoleEvent;

  Status = gBS->CreateEventEx(
      EVT_NOTIFY_SIGNAL,
      TPL_CALLBACK,
      InternalNullFunction,
      NULL,
      &gEfiAfterTrustConsoleEventGroupGuid,
      &AfterTrustConsoleEvent
      );
  ASSERT_EFI_ERROR(Status);
  gBS->SignalEvent(AfterTrustConsoleEvent);
  gBS->CloseEvent(AfterTrustConsoleEvent);
  DEBUG((EFI_D_INFO,"All AfterTrustConsoleEvent callbacks have returned successfully\n"));

}

