/** @file
  HECI Legacy Initialization DXE Driver

 @copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2019 Intel Corporation.

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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

**/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/MeUefiFwHealthStatusLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/HeciProtocol.h>
#include <Register/HeciRegs.h>
#include "HeciLegacyCore.h"
#include <Library/MeUtilsLib.h>
#include <Library/MeTypeLib.h>

//
// Global driver data
//
HECI_PROTOCOL   HeciProtocolInstance;

/**
  DXE HECI driver entry point

  @param [in] ImageHandle  EFI image handle
  @param [in] SystemTable  EFI system table

  @retval Status           Driver initialization status
**/
EFI_STATUS
EFIAPI
HeciDxeEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS              Status;

  DEBUG ((DEBUG_INFO, "[HECI Legacy] %a () - Start\n", __FUNCTION__));
  if (MeTypeIsSpsIgnition ()) {
    SetMeUefiFwHealthEfiStatus (Heci1DxeInstalledStatus, EFI_UNSUPPORTED);
    return EFI_UNSUPPORTED;
  }

  //
  // Initialize HECI protocol pointers
  //
  HeciProtocolInstance.SendwAck     = HeciSendwAck;
  HeciProtocolInstance.ReadMsg      = HeciReceive;
  HeciProtocolInstance.SendMsg      = HeciSend;
  HeciProtocolInstance.ResetHeci    = HeciResetInterface;
  HeciProtocolInstance.InitHeci     = HeciInitialize;
  HeciProtocolInstance.MeResetWait  = HeciMeResetWait;
  HeciProtocolInstance.ReInitHeci   = HeciReInitialize;
  HeciProtocolInstance.GetMeStatus  = HeciGetMeStatus;
  HeciProtocolInstance.GetMeMode    = HeciGetMeMode;

  //
  // Install the HECI interface
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gHeciProtocolGuid,
                  &HeciProtocolInstance,
                  NULL
                  );
  SetMeUefiFwHealthEfiStatus (Heci1DxeInstalledStatus, Status);

  DEBUG ((DEBUG_INFO, "[HECI Legacy] %a () - Exit (%r)\n", __FUNCTION__, Status));

  return Status;
}
