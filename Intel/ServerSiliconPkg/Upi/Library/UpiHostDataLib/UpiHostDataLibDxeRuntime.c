/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation. <BR>

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
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/HobLib.h>
#include "UpiHostData.h"
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Guid/EventGroup.h>

//
// Global variables
//
GLOBAL_REMOVE_IF_UNREFERENCED KTI_HOST *mDxeRuntimeKtiProtocol        = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN  mUpiDataMemoryAttributeUpdated = FALSE;

EFI_EVENT                 mDxeRuntimeUpiHostDataLibVirtualAddressChangeEvent  = NULL;

/**
  Convert mDxeRuntimeKtiProtocol physical address to virtual address.

  @param[in] Event      Event whose notification function is being invoked.
  @param[in] Context    The pointer to the notification function's context, which
                        is implementation-dependent.
**/
VOID
EFIAPI
DxeHostDataLibVirtualAddressChangeEvent (
  IN  EFI_EVENT   Event,
  IN  VOID        *Context
  )
{
  gRT->ConvertPointer (0x00, (VOID**) &(mDxeRuntimeKtiProtocol));
}

VOID
EFIAPI
UpdateUpiHostDataMemoryAttribute (
  VOID
  )
{
  EFI_STATUS                      Status;
  UINT64                          Length;
  UINT64                          Attributes;
  EFI_GCD_MEMORY_SPACE_DESCRIPTOR MemorySpaceDescriptor;

  Status = gDS->GetMemorySpaceDescriptor ((EFI_PHYSICAL_ADDRESS)(UINTN)mDxeRuntimeKtiProtocol, &MemorySpaceDescriptor);
  ASSERT_EFI_ERROR (Status);

  Length = EFI_PAGES_TO_SIZE (EFI_SIZE_TO_PAGES (sizeof (KTI_HOST)));
  if ((MemorySpaceDescriptor.Attributes & EFI_MEMORY_RUNTIME) != EFI_MEMORY_RUNTIME) {
    Attributes = MemorySpaceDescriptor.Attributes | EFI_MEMORY_RUNTIME;
    Status = gDS->SetMemorySpaceAttributes (
                    (EFI_PHYSICAL_ADDRESS)(UINTN)mDxeRuntimeKtiProtocol,
                    Length,
                    Attributes
                    );
    ASSERT_EFI_ERROR (Status);
  }
}

/**
  The constructor function initialize global DXE KTI Host protocol data pointer
  get data from Hob and install protocol for DXE KTI Host protocol

  Standard EFI driver point.

  @param ImageHandle         -  A handle for the image that is initializing this driver.
  @param SystemTable         -  A pointer to the EFI system table.

  @retval EFI_SUCCESS   The constructor should returns EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
UpiHostDataLibDxeRuntimeConstructor (
  IN EFI_HANDLE              ImageHandle,
  IN EFI_SYSTEM_TABLE        *SystemTable
  )
{
  EFI_STATUS                 Status;
  KTI_HOST_IN                *KtiHostInDataPtr;
  KTI_HOST_OUT               *KtiHostOutDataPtr;
  KTI_HOST_NVRAM             *KtiHostNvramDataPtr;
  EFI_PHYSICAL_ADDRESS       KtiHostProtocol;
  EFI_HANDLE                 Handle;
  VOID                       *GuidHob;

  Status = gBS->LocateProtocol (&gDxeKtiProtocolGuid, NULL, &mDxeRuntimeKtiProtocol);

  if (!EFI_ERROR (Status)) {
    if (!mUpiDataMemoryAttributeUpdated) {
      UpdateUpiHostDataMemoryAttribute ();
      mUpiDataMemoryAttributeUpdated = TRUE;
    }

    return EFI_SUCCESS;
  }

  //
  // Get data from Hob and install protocol
  //
  GuidHob = GetFirstGuidHob (&gEfiKtiHostInDataHobGuid);
  ASSERT (GuidHob != NULL);
  KtiHostInDataPtr = (KTI_HOST_IN *)(GET_GUID_HOB_DATA (GuidHob));

  GuidHob = GetFirstGuidHob (&gEfiKtiHostOutDataHobGuid);
  ASSERT (GuidHob != NULL);
  KtiHostOutDataPtr = (KTI_HOST_OUT *)(GET_GUID_HOB_DATA (GuidHob));

  GuidHob = GetFirstGuidHob (&gEfiKtiHostNvramDataHobGuid);
  ASSERT (GuidHob != NULL);
  KtiHostNvramDataPtr = (KTI_HOST_NVRAM *)(GET_GUID_HOB_DATA (GuidHob));

  KtiHostProtocol = 0xffffffff;
  Status = gBS->AllocatePages (
                 AllocateMaxAddress,
                 EfiRuntimeServicesCode,
                 EFI_SIZE_TO_PAGES (sizeof (KTI_HOST)),
                 &KtiHostProtocol);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  CopyMem ((VOID *)&(((KTI_HOST *)(UINTN)KtiHostProtocol)->HostIn), (VOID *)KtiHostInDataPtr, sizeof (KTI_HOST_IN));
  CopyMem ((VOID *)&(((KTI_HOST *)(UINTN)KtiHostProtocol)->HostOut), (VOID *)KtiHostOutDataPtr, sizeof (KTI_HOST_OUT));
  CopyMem ((VOID *)&(((KTI_HOST *)(UINTN)KtiHostProtocol)->HostNvram), (VOID *)KtiHostNvramDataPtr, sizeof (KTI_HOST_NVRAM));

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gDxeKtiProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    (VOID *)(UINTN)KtiHostProtocol
                    );

  if (EFI_ERROR (Status)) {
    Status = gBS->FreePool ((VOID *)(UINTN)KtiHostProtocol);
    ASSERT_EFI_ERROR (Status);

    return Status;
  }

  gBS->LocateProtocol (&gDxeKtiProtocolGuid, NULL, &mDxeRuntimeKtiProtocol);

  mUpiDataMemoryAttributeUpdated = TRUE;

  //
  // Make sure we can handle virtual address changes.
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  DxeHostDataLibVirtualAddressChangeEvent,
                  NULL,
                  &gEfiEventVirtualAddressChangeGuid,
                  &mDxeRuntimeUpiHostDataLibVirtualAddressChangeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

/**
  The destructor function of Runtime DXE UPI HOST Data Lib.

  The destructor function frees memory allocated by constructor, and closes related events.
  It will ASSERT() if that related operation fails and it will always return EFI_SUCCESS.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
UpiHostDataLibDxeRuntimeDestructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;

  ASSERT (gBS != NULL);
  if (gBS != NULL) {
    Status = gBS->CloseEvent (mDxeRuntimeUpiHostDataLibVirtualAddressChangeEvent);
  }
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

/**
  Get the KTI Host data pointer.
  @param KtiHostType  -  The KTI host data type defined in KTI_HOST_TYPE
  @return pointer of the KTI Host data

 **/
VOID *
EFIAPI
GetKtiHostDataPtr (
  KTI_HOST_TYPE KtiHostType
  )
{
  ASSERT (KtiHostType < KtiHostMax);
  ASSERT (mDxeRuntimeKtiProtocol != NULL);

  if (KtiHostType == KtiHostIn) {
    return &(mDxeRuntimeKtiProtocol->HostIn);
  } else if (KtiHostType == KtiHostOut) {
    return &(mDxeRuntimeKtiProtocol->HostOut);
  } else if (KtiHostType == KtiHostNvram) {
    return &(mDxeRuntimeKtiProtocol->HostNvram);
  } else {
    ASSERT (0);
    return NULL;
  }
}
