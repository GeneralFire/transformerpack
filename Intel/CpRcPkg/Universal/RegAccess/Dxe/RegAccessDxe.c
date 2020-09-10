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

#include <Protocol/SiliconRegAccess.h>
#include <Library/SiliconAccessLib.h>
#include <Library/DebugLib.h>

// #include <Library/BaseMemoryLib/MemLibInternals.h>
// #pragma optimize ("",off)

EFI_HANDLE                gUsraHandle = NULL;

///
/// USRA_PROTOCOL the EDKII native implementation which support Unified
/// Silicon Register (CSR, PCIE) Access
///
USRA_PROTOCOL mUsraInstance = {
  DxeRegRead,
  DxeRegWrite,
  DxeRegModify,
  DxeGetRegAddr
};


/**
  Main entry for USRA DXE driver.
  This routine initialize the USRA driver and install USRA_PROTOCOL.

  @param[in] ImageHandle          Image handle for USRA DXE driver
  @param[in] SystemTable          Pointer to SystemTable

  @retval Status                  Status of gBS->InstallProtocolInterface()
**/
EFI_STATUS
EFIAPI
UsraDxeInit (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS              Status;
  //
  // Install USRA_PROTOCOL to handle Unified Reg access in Dxe
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &gUsraHandle,
                  &gDxeUsraProtocolGuid,
                  &mUsraInstance,
                  NULL
                  );

  ASSERT_EFI_ERROR (Status);
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  return Status;

}

