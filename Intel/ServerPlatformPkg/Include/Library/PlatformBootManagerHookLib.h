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

#ifndef _PLATFORM_BOOT_MANAGER_HOOK_LIB_
#define _PLATFORM_BOOT_MANAGER_HOOK_LIB_

#include <Base.h>
#include <Uefi.h>

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
  );

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
  );

/**
  Signal the boot manager pause event by create a new boot manager pause event
  with gEfiEventBootManagerPauseGuid and then signal the event just created.
  After event signaled close this event.
**/
VOID
EFIAPI
SignalEventBootManagerPause (
  VOID
  );

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
  );

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
  );

/**
  Signal the boot manager resume event by create a new boot manager resume event
  with gEfiEventBootManagerResumeGuid  and then signal the event just created.
  After event signaled close this event.
**/
VOID
EFIAPI
SignalEventBootManagerResume (
  VOID
  );

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
  );

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
  );

/**
  Signal the after console event by create a new after console event
  with gEfiEventPlatformBootManagerAfterConsoleGuid and then signal the event just created.
  After event signaled close this event.
**/
VOID
EFIAPI
SignalEventAfterConsole (
  VOID
  );

/**
  Signal AfterTrustConsoleEvent event after trust console
**/
VOID
EFIAPI
SignalEventAfterTrustConsole (
  VOID
  );

#endif //_PLATFORM_BOOT_MANAGER_HOOK_LIB_
