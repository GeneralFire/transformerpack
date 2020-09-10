/** @file
  This file is SampleCode for Intel PEI Platform Policy initialzation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2020 Intel Corporation. <BR>

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

#include "PeiPolicyInit.h"
#include <Library/SiPolicyLib.h>

/**
  Initialize Intel PEI Platform Policy

  @param[in]  FirmwareConfiguration  It is used to skip specific policy init that depends
                                     on the 'FirmwareConfiguration' variable.
  @param[in]  SystemConfiguration  It uses to deliver setup configuration.

**/
VOID
EFIAPI
PeiPolicyInitPreMem (
  IN CONST EFI_PEI_SERVICES    **PeiServices,
  IN UINT8                     FirmwareConfiguration
  )
{
#ifdef CDF_SC_FLAG
  EFI_STATUS                   Status;
  SI_PREMEM_POLICY_PPI         *SiPreMemPolicyPpi;

  DEBUG ((DEBUG_INFO, "Silicon PEI Policy Initialization Start in Pre-Memory...\n"));
  //
  // Call SiCreatePreMemConfigBlocks to initialize platform policy structure
  // and get all intel default policy settings.
  //
  Status = SiCreatePreMemConfigBlocks (&SiPreMemPolicyPpi);
  ASSERT_EFI_ERROR (Status);

  if (FirmwareConfiguration != FwConfigDefault) {
    //
    // Update and override all platform related and customized settings below.
    //

    UpdatePeiPchPolicyPreMem (SiPreMemPolicyPpi);
    if ((FirmwareConfiguration >= FwConfigTest) && (FirmwareConfiguration < FwConfigMax)) {
      // Update Debug Policies
      UpdatePeiPchPolicyDebugPreMem (SiPreMemPolicyPpi);
    }


    UpdatePeiMePolicyPreMem (SiPreMemPolicyPpi);
    UpdatePeiSpsPreMemPolicy (SiPreMemPolicyPpi);
  }
  //
  // Install SiPreMemPolicyPpi.
  // While installed, RC assumes the Policy is ready and finalized. So please
  // update and override any setting before calling this function.
  //
  Status = SiPreMemInstallPolicyPpi (SiPreMemPolicyPpi);
  ASSERT_EFI_ERROR (Status);

  Status = SiPreMemInstallPolicyReadyPpi ();
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "Silicon PEI Policy Initialization Done in Pre-Memory\n"));
#else
  //
  // PCH PEI Policy Initialization
  //
  PeiPolicyInit (PeiServices, FirmwareConfiguration);
  DEBUG ((DEBUG_INFO, "PCH PEI Policy Initialization Done \n"));
#endif //CDF_SC_FLAG
}
