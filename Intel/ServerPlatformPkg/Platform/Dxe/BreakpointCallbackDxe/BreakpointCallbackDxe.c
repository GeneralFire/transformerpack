/** @file
  Provides callbacks for preset breakpoints during DXE

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2017 Intel Corporation. <BR>

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

#include "Guid/SetupBreakpointGuid.h"
#include "BreakpointCallbackDxe.h"
#include <Guid/SetupVariable.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/SetupLib.h>

EFI_GUID  gEfiAfterResourceAllocGuid  = EFI_AFTER_RESOURCE_ALLOC_GUID;
EFI_GUID  gEfiAfterPostGuid           = EFI_AFTER_POST_GUID;
EFI_GUID  mSystemConfigurationGuid    = SYSTEM_CONFIGURATION_GUID;


/**

  Checks the setup option for breakpoint type and starts
  a deadloop if the After Resource Allocation was selected

  @param Event - The instance of the event triggering this function
  @param *Context - Not used

  @retval EFI_SUCCESS  -  Function has completed successfully.

**/
EFI_STATUS
EFIAPI
AfterResourceAllocBreakpoint (
  IN EFI_EVENT    Event,
  IN VOID             *Context
  )
{
    EFI_STATUS              Status = EFI_SUCCESS;
    UINT8                   ValidationBreakpointType;

    Status = GetOptionData (&gEfiSetupVariableGuid,  OFFSET_OF(SYSTEM_CONFIGURATION, ValidationBreakpointType), &ValidationBreakpointType, sizeof(UINT8));
  //
  // If setup variables can't be accessed, treat it as if no breakpoint is set
  //
    if(EFI_ERROR(Status) ) {
      ValidationBreakpointType= BP_NONE;
    }

    if (ValidationBreakpointType == BP_AFTER_RESOURCE_ALLOC) {
        DEBUG((EFI_D_INFO, "== Validation Breakpoint After Resource Allocation is done! ==\n"));
        EFI_DEADLOOP();
    }

    return Status;
}


/**

  Checks the setup option for breakpoint type and starts
  a deadloop if the After Post was selected

  @param Event - The instance of the event triggering this function
  @param *Context - Not used

  @retval EFI_SUCCESS  -  Function has completed successfully.

**/
EFI_STATUS
EFIAPI
AfterPostBreakpoint (
  IN EFI_EVENT    Event,
  IN VOID             *Context
  )
{
    EFI_STATUS              Status;
    UINT8                   ValidationBreakpointType;

    Status = GetOptionData (&gEfiSetupVariableGuid,  OFFSET_OF(SYSTEM_CONFIGURATION, ValidationBreakpointType), &ValidationBreakpointType, sizeof(UINT8));
  //
  // If setup variables can't be accessed, treat it as if no breakpoint is set
  //
    if(EFI_ERROR(Status) ) {
      ValidationBreakpointType = BP_NONE;
    }

    if (ValidationBreakpointType == BP_AFTER_POST) {
        DEBUG((EFI_D_INFO, "== Validation Breakpoint After Post is done! ==\n"));
        EFI_DEADLOOP();
    }

    return Status;
}


/**

  This routine creates events to trigger breakpoints
  after certain GUID's are installed

  @param ImageHandle - The image's handle
  @param *SystemTable - Pointer to the system table

  @retval EFI_SUCCESS  -  Function has completed successfully.

**/
EFI_STATUS
EFIAPI
BreakpointCallbackInit (
  IN EFI_HANDLE               ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
)
{
  EFI_STATUS       Status;
  EFI_EVENT        AfterResourceAllocEvent;
  EFI_EVENT        ReadyToBootEvent;
  VOID             *Registration;

  Status = gBS->CreateEvent(
                EVT_NOTIFY_SIGNAL,
                TPL_CALLBACK,
                AfterResourceAllocBreakpoint,
                NULL,
                &AfterResourceAllocEvent
                );
  ASSERT_EFI_ERROR(Status);

  Status = gBS->RegisterProtocolNotify(
                &gEfiAfterResourceAllocGuid,
                AfterResourceAllocEvent,
                &Registration
                );
  ASSERT_EFI_ERROR(Status);

  Status = EfiCreateEventReadyToBootEx(
                TPL_CALLBACK,
                AfterPostBreakpoint,
                NULL,
                &ReadyToBootEvent
                );
  ASSERT_EFI_ERROR(Status);

  return EFI_SUCCESS;
}


