/** @file
  Implementation of SystemInfoLib in DXE runtime

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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
#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SystemInfoLib.h>
#include <Protocol/SystemInfoVarProtocol.h>
#include <Library/UefiRuntimeLib.h>

extern EFI_GUID  gDxeSystemInfoVarProtocol;
SYSTEM_INFO_VAR  *mSystemInfoVar;
EFI_EVENT        mSystemInfoLibRuntimeVirtualAddressChangeEvent;

/**
  This function returns the pointer to SYSTEM_INFO_VAR.

  @param VOID

  @retval SYSTEM_INFO_VAR*   The pointer to SYSTEM_INFO_VAR.
**/
SYSTEM_INFO_VAR *
GetSystemInfoVar (
  VOID
  )
{
  return mSystemInfoVar;
}

/**
  Enable the posted CSR access method
  Subsequent CSR accesses will be sent using Posted semantics.

  A call to this function must always be paired with a call
  to PostedMethodDisable to avoid unintentionally sending all
  future CSR accesses using posted semantics.

  @retval EFI_UNSUPPORTED if posted accesses are not supported
  @retval EFI_SUCCESS otherwise
**/
EFI_STATUS
EFIAPI
PostedMethodEnable (
  VOID
  )
{
  //
  // Posted accesses currently not enabled in DXE
  //
  return EFI_UNSUPPORTED;
}

/**
  Disable the posted CSR access method
  Subsequent CSR writes will be sent using non-posted semantics

  @retval EFI_UNSUPPORTED if posted accesses are not supported
  @retval EFI_SUCCESS otherwise
**/
EFI_STATUS
EFIAPI
PostedMethodDisable (
  VOID
  )
{
  //
  // Posted accesses currently not enabled in DXE
  //
  return EFI_UNSUPPORTED;
}

/**
  This is a notification function registered on EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE event.
  It convers pointer to new virtual address.

  @param  Event        Event whose notification function is being invoked.
  @param  Context      Pointer to the notification function's context.

  @retval VOID

**/
VOID
EFIAPI
SystemInfoLibRuntimeVirtualAddressChangeEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  EfiConvertPointer (0, (VOID **) &mSystemInfoVar);
}

/**
  The constructor function initialize system info var.

  Standard EFI driver point.

  @param ImageHandle         -  A handle for the image that is initializing this driver.
  @param SystemTable         -  A pointer to the EFI system table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
RuntimeSystemInfoLibConstructor (
  IN EFI_HANDLE              ImageHandle,
  IN EFI_SYSTEM_TABLE        *SystemTable
  )
{
  EFI_STATUS                 Status;
  SYSTEM_INFO_VAR_PROTOCOL   *DxeSystemInfoVarProtocol;

  Status = gBS->LocateProtocol (
                    &gDxeSystemInfoVarProtocol,
                    NULL,
                    &DxeSystemInfoVarProtocol
                    );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  mSystemInfoVar =  (SYSTEM_INFO_VAR *) DxeSystemInfoVarProtocol;

  //
  // Make sure we can handle virtual address changes.
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  SystemInfoLibRuntimeVirtualAddressChangeEvent,
                  NULL,
                  &gEfiEventVirtualAddressChangeGuid,
                  &mSystemInfoLibRuntimeVirtualAddressChangeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

/**
  The destructor function of Runtime System Info Lib.

  The destructor function frees memory allocated by constructor, and closes related events.
  It will ASSERT() if that related operation fails and it will always return EFI_SUCCESS.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
RuntimeSystemInfoLibDestructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;

  ASSERT (gBS != NULL);
  if (gBS != NULL) {
    Status = gBS->CloseEvent (mSystemInfoLibRuntimeVirtualAddressChangeEvent);
  }
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

