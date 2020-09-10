/** @file
  USRA DXE driver manage all USRA entry initialized in PEI phase and DXE phase, and
  produce the implementation of native USRA protocol defined

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2018 Intel Corporation. <BR>

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

#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Protocol/SmmBase2.h>

#include <Protocol/SiliconRegAccess.h>
#include <Library/SiliconAccessLib.h>
#include <Library/DebugLib.h>

EFI_HANDLE                gUsraHandle = NULL;

EFI_SMM_BASE2_PROTOCOL    *mSmmBase = NULL;
EFI_SMM_SYSTEM_TABLE2     *mSmst = NULL;

///
/// USRA_PROTOCOL the EDKII native implementation which support Unified
/// Silicon Register (CSR, PCIE) Access
///
USRA_PROTOCOL mUsraInstance = {
  SmmRegRead,
  SmmRegWrite,
  SmmRegModify,
  SmmGetRegAddr
};


/**
  Main entry for USRA SMM driver.
  This routine initialize the USRA driver and install USRA_PROTOCOL.

  @param[in] ImageHandle          Image handle for USRA SMM driver
  @param[in] SystemTable          Pointer to SystemTable

  @retval Status                  Status of mSmst->SmmInstallProtocolInterface
**/
EFI_STATUS
EFIAPI
UsraSmmInit (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS              Status;
  //
  // Retrieve SMM Base Protocol
  //
  Status = gBS->LocateProtocol (
                  &gEfiSmmBase2ProtocolGuid,
                  NULL,
                  (VOID**) &mSmmBase
                  );
  mSmmBase->GetSmstLocation (mSmmBase, &mSmst);
  //
  // Install USRA_Smm to handle Unified Reg access in Dxe
  //

  Status = mSmst->SmmInstallProtocolInterface (
                    &gUsraHandle,
                    &gSmmUsraProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mUsraInstance
                    );

  ASSERT_EFI_ERROR (Status);
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  return Status;

}
