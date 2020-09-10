/** @file
  Implementation of Register table End of DXE callback.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/CsrRegTableLib.h>

/**
  Dxe callback for programing registers according to register table.

  @param  Event                 The triggered event.
  @param  Context               Context for this event.

**/
VOID
EFIAPI
SaiCsrInBatchWriteDxeCallBack (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  SaiCsrInBatchWrite ();
}

/**
  Entry point for Csr Reg Table Dxe driver.

  This function Registers End of DXE event to program CSR according to RegTable.

  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval Status.

**/
EFI_STATUS
EFIAPI
CsrRegTableDxeEntry (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
  EFI_STATUS Status;
  EFI_EVENT  EndOfDxeEvent;

  //
  // Register End of DXE event to program CSR according to RegTable.
  //
  Status = gBS->CreateEventEx (
                EVT_NOTIFY_SIGNAL,
                TPL_NOTIFY,
                SaiCsrInBatchWriteDxeCallBack,
                NULL,
                &gEfiEndOfDxeEventGroupGuid,
                &EndOfDxeEvent
                );
  ASSERT_EFI_ERROR (Status);

  return Status;
}


