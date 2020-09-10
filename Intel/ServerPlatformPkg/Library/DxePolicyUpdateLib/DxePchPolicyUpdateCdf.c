/** @file
  This file is the library for PCH DXE Policy initialization.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2018 Intel Corporation.

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

#include <PiDxe.h>
#include <SetupVariable.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/PchPolicy.h>
#include <PchSetupVariableCdfEbg.h>


GLOBAL_REMOVE_IF_UNREFERENCED PCH_PCIE_DEVICE_OVERRIDE mPcieDeviceTable[] = {

  //
  // End of Table
  //
  { 0 }
};

/**
  Get data for PCH policy from setup options.

  @param[in] PchPolicy                 The pointer to get PCH Policy protocol instance

  @retval EFI_SUCCESS                  Operation success.

**/
EFI_STATUS
UpdateDxePchPolicy (
  IN OUT  PCH_POLICY_PROTOCOL    *PchPolicy
  )
{
  EFI_STATUS              Status;
  UINTN                   VariableSize;
  PCH_SETUP               PchSetup;

  VariableSize = sizeof (PCH_SETUP);
  Status = gRT->GetVariable (
                   L"PchSetup",
                   &gPchSetupVariableGuid,
                   NULL,
                   &VariableSize,
                   &PchSetup
                   );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
