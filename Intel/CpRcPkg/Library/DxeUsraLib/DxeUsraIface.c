/** @file
  Implementation of UsarLib class library for DXE phase.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2020 Intel Corporation. <BR>

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

#include <PiDxe.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Protocol/SiliconRegAccess.h>
#include <Library/DebugLib.h>
#include <Guid/DxeServices.h>
#include <Library/UefiLib.h>

USRA_PROTOCOL     *mUsra = NULL;

/**
  Retrieves the USRA protocol from the handle database.

  @param - None

  @retval - Pointer to the USRA_PROTOCOL

**/

USRA_PROTOCOL *
EFIAPI
GetUsraProtocol (
  VOID
  )
{
  EFI_STATUS  Status;

  //
  // USRA protocol need to be installed before the module access USRA.
  //
  Status = gBS->LocateProtocol (&gDxeUsraProtocolGuid, NULL, (VOID **)&mUsra);
  ASSERT_EFI_ERROR (Status);
  if (Status != EFI_SUCCESS) {
    return NULL;
  }
  ASSERT (mUsra != NULL);
  if (mUsra == NULL) {
    return NULL;
  }

  return mUsra;
}

