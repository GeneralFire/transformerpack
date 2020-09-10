/**@file
  MeSmbios driver

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

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

  This file contains a 'Sample Driver' and is licensed as such under the terms
  of your license agreement with Intel or your vendor. This file may be modified
  by the user, subject to the additional terms of the license agreement.

@par Specification Reference:
**/

#include "MeSmbiosInternal.h"
#include <Guid/EventGroup.h>
#include <Protocol/MeSmbiosUpdateProtocol.h>
#include <Library/MeTypeLib.h>

/**
  Add an SMBIOS record.

  @param[in]  SmbiosTable       The pointer to the SMBIOS record.
  @param[in]  SmbiosTableSize   The size of the SMBIOS record.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_OUT_OF_RESOURCES  Record was not added due to lack of system resources.

**/
EFI_STATUS
AddSmbiosRecord (
  IN VOID   *SmbiosTable,
  IN UINT32 SmbiosTableSize
  )
{
  EFI_STATUS              Status;
  EFI_SMBIOS_PROTOCOL     *Smbios;
  EFI_SMBIOS_TABLE_HEADER *SmbiosRecord;
  EFI_SMBIOS_HANDLE       SmbiosHandle;

  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **) &Smbios);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  //
  // Add two zeros at the tail. SmbiosAdd will check the last two zeros as terminator.
  //
  SmbiosRecord = AllocateZeroPool (SmbiosTableSize + 2);
  if (SmbiosRecord == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (SmbiosRecord, SmbiosTable, SmbiosTableSize);

  Status = Smbios->Add (Smbios, NULL, &SmbiosHandle, SmbiosRecord);

  FreePool (SmbiosRecord);

  return Status;
}

/**
  Add and update tables 130 (if AMT is enabled) and 131 to SMBIOS.

  @param[in]  Event         The Event this notify function registered to.
  @param[in]  Context       Pointer to the context data registered to the Event.
**/
VOID
EFIAPI
UpdateMeSmbiosTable (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                       Status;
  ME_SMBIOS_TABLES_UPDATE_PROTOCOL *MeSmbiosTablesUpdateProtocol;

#if FixedPcdGetBool(PcdAmtEnable) == 1
  CreateSmbiosTableType130 ();
#endif
  CreateSmbiosTableType131 ();

  MeSmbiosTablesUpdateProtocol = NULL;
  Status = gBS->LocateProtocol (&gMeSmbiosTablesUpdateProtocolGuid, NULL, (VOID**)&MeSmbiosTablesUpdateProtocol);
  if (EFI_ERROR (Status)) {
    gBS->CloseEvent (Event);
    return;
  }

#if FixedPcdGetBool(PcdAmtEnable) == 1
  MeSmbiosTablesUpdateProtocol->UpdateSmbios130 ();
#endif
  MeSmbiosTablesUpdateProtocol->UpdateSmbios131 ();

  gBS->CloseEvent (Event);
}

/**
  Initialize ME SMBIOS driver.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The driver initializes correctly.
  @retval EFI_UNSUPPORTED   The ME doesn't run the client FW.
**/
EFI_STATUS
EFIAPI
MeSmbiosDriverEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   Event;

  if (!MeTypeIsClient ()) {
    return EFI_UNSUPPORTED;
  }

  //
  // Register End of DXE event
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  UpdateMeSmbiosTable,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &Event
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
