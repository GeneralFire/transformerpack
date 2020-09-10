/** @file
  This file is a wrapper for Platform Policy driver. Get Setup
  Value to initialize Intel DXE Platform Policy.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2018 Intel Corporation.

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

#include "PolicyInitDxe.h"

GLOBAL_REMOVE_IF_UNREFERENCED UINT8            mFirmwareConfiguration = 0;


/**
  Initialize  DXE Platform Policy

  @param[in] ImageHandle       Image handle of this driver.
  @param[in] SystemTable       Global system service table.

  @retval EFI_SUCCESS           Initialization complete.
  @exception EFI_UNSUPPORTED    The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR      Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
PolicyInitDxeEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS           Status;
  PCH_SETUP            PchSetup;

  Status = GetSpecificConfigGuid (&gPchSetupVariableGuid, (VOID *)&PchSetup);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ERROR: Fail to get Pch Setup to identify the firmware configuration!\n"));
  } else {
    mFirmwareConfiguration = PchSetup.FirmwareConfiguration;
  }

  //
  // PCH Dxe Platform Policy Initialization
  //
  Status = PchPolicyInitDxe (ImageHandle);
  DEBUG ((DEBUG_INFO, "PCH Dxe Platform Policy Initialization done\n"));
  ASSERT_EFI_ERROR (Status);

  //
  // ME DXE Policy Initialization
  //
  Status = MePolicyInitDxe (ImageHandle);
  DEBUG ((DEBUG_INFO, "ME DXE Policy Initialization done\n"));
  ASSERT_EFI_ERROR (Status);

#if AMT_SUPPORT
  //
  // AMT DXE Policy Initialization
  //
  Status = AmtPolicyInitDxe (ImageHandle);
  DEBUG ((DEBUG_INFO, "AMT DXE Policy Initialization done\n"));
  ASSERT_EFI_ERROR (Status);
#endif // AMT_SUPPORT

  //
  // SPS DXE Policy Initialization
  //
  Status = SpsPolicyInitDxe (ImageHandle);
  DEBUG ((DEBUG_INFO, "SPS DXE Policy Initialization done\n"));
  ASSERT_EFI_ERROR (Status);

  return Status;

}
