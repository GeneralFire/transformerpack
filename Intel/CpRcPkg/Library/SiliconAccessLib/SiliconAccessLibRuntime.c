/** @file
  Unified Silicon Register Access (USRA) Lib

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
**/

#include "SiliconAccessLibInternal.h"
#include <Library/UefiRuntimeLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>


extern GET_ALIGNED_ACCESS_ADDRESS mAccessAddrPtr[AddrTypeMaximum];
extern REGISTER_READ mRegisterReadPtr[AddrTypeMaximum];
extern REGISTER_WRITE mRegisterWritePtr[AddrTypeMaximum];

EFI_EVENT     mSiliconAccessLibRuntimeVirtualAddressChangeEvent = NULL;

/**
  Notification function of EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE.

  This is a notification function registered on EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE event.
  It convers pointer to new virtual address.

  @param  Event        Event whose notification function is being invoked.
  @param  Context      Pointer to the notification function's context.

  @retval VOID

**/
VOID
EFIAPI
SiliconAccessLibRuntimeVirtualAddressChangeEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  UINT32                Count;
  UINT32                Index;

  Count = sizeof(mAccessAddrPtr) / sizeof(GET_ALIGNED_ACCESS_ADDRESS);
  for (Index = 0; Index < Count; Index++) {
    EfiConvertPointer (0, (VOID **) &(mAccessAddrPtr[Index]));
  }

  Count = sizeof(mRegisterReadPtr) / sizeof(REGISTER_READ);
  for (Index = 0; Index < Count; Index++) {
    EfiConvertPointer (0, (VOID **) &(mRegisterReadPtr[Index]));
  }

  Count = sizeof(mRegisterWritePtr) / sizeof(REGISTER_WRITE);
  for (Index = 0; Index < Count; Index++) {
    EfiConvertPointer (0, (VOID **) &((mRegisterWritePtr)[Index]));
  }

  EfiConvertPointer (0, (VOID **) &mAccessAddrPtr);
  EfiConvertPointer (0, (VOID **) &mRegisterReadPtr);
  EfiConvertPointer (0, (VOID **) &mRegisterWritePtr);

}

/**
  The constructor function of Silicon Access Library Runtime

  Standard EFI driver point.

  @param ImageHandle         -  A handle for the image that is initializing this driver.
  @param SystemTable         -  A pointer to the EFI system table.

  @retval EFI_SUCCESS   The constructor should returns EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
SiliconAccessLibRuntimeConstructor (
  IN EFI_HANDLE              ImageHandle,
  IN EFI_SYSTEM_TABLE        *SystemTable
  )
{
  EFI_STATUS      Status;

  //
  // Make sure we can handle virtual address changes.
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  SiliconAccessLibRuntimeVirtualAddressChangeEvent,
                  NULL,
                  &gEfiEventVirtualAddressChangeGuid,
                  &mSiliconAccessLibRuntimeVirtualAddressChangeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

/**
  The destructor function of Runtime DXE Silicon Access Lib.

  The destructor function frees memory allocated by constructor, and closes related events.
  It will ASSERT() if that related operation fails and it will always return EFI_SUCCESS.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
SiliconAccessLibRuntimeDestructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;

  ASSERT (gBS != NULL);
  if (gBS != NULL) {
    Status = gBS->CloseEvent (mSiliconAccessLibRuntimeVirtualAddressChangeEvent);
  }
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

