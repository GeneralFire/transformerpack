/** @file
  Do Platform Stage SPS initialization.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation.

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

#include "PeiSpsPolicyUpdate.h"
#include <Library/ConfigBlockLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Guid/MeRcVariable.h>
#include <Library/SetupLib.h>
#include <ConfigBlock/SpsPeiConfig.h>
#include <Library/MeConfigLib.h>
#include <Library/MeTypeLib.h>
#include <ConfigBlock/MeGetConfigBlock.h>
#include <MeDefines.h>
#include <Library/MeServerPolicyHelperLib.h>

/**
  Update the SPS Policy Ppi (pre mem)

  @param[in, out] SiPreMemPolicyPpi   PEI Pre Meme Si Policy

  @retval EFI_SUCCESS           Initialization complete.
  @retval EFI_UNSUPPORTED       The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR      Device error, driver exits abnormally.
  @retval EFI_INVALID_PARAMETER Wrong pointer passed to the function
**/
EFI_STATUS
UpdatePeiSpsPreMemPolicy (
  IN OUT  SI_PREMEM_POLICY_PPI *SiPreMemPolicyPpi
  )
{
#if ME_SPS_SUPPORT
  EFI_STATUS                      Status;
  ME_RC_CONFIGURATION             MeSetup;
  SPS_PEI_CONFIG                  *SpsPeiConfig;

  DEBUG ((DEBUG_INFO, "UpdatePeiSpsPreMemPolicy\n"));

  if (SiPreMemPolicyPpi == NULL) {
    Status = EFI_INVALID_PARAMETER;
    DEBUG ((DEBUG_ERROR, "ERROR : SiPreMemPolicy unknown (%r)\n", Status));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  SpsPeiConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gSpsPeiConfigGuid, (VOID *) &SpsPeiConfig);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ERROR : SiPreMemPolicy cant get gSpsPeiConfigGuid (%r)\n", Status));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = GetSpecificConfigGuid (&gEfiMeRcVariableGuid, (VOID *)&MeSetup);
  if (!EFI_ERROR (Status) && (SpsPeiConfig != NULL)) {
    CopyDataOfSpsPreMemConfig (&MeSetup, SpsPeiConfig);
  }

  return Status;
#else // ME_SPS_SUPPORT
  return EFI_SUCCESS;
#endif // ME_SPS_SUPPORT
}

/**
  Update the SPS Policy Ppi (post mem)

  @param[in, out] SiPolicyPpi   PEI Si Policy

  @retval EFI_SUCCESS           Initialization complete.
  @retval EFI_UNSUPPORTED       The chipset is unsupported by this driver.
  @retval EFI_DEVICE_ERROR      Device error, driver exits abnormally.
  @retval EFI_INVALID_PARAMETER Wrong pointer passed to the function
**/
EFI_STATUS
UpdatePeiSpsPolicy (
  IN OUT  SI_POLICY_PPI *SiPolicyPpi
  )
{
#if ME_SPS_SUPPORT
  SI_PREMEM_POLICY_PPI *SiPreMemPolicyPpi = NULL;
  EFI_STATUS           Status;

  DEBUG ((DEBUG_INFO, "UpdatePeiSpsPolicy\n"));

  PeiServicesLocatePpi (&gSiPreMemPolicyPpiGuid, 0, NULL, (VOID **)&SiPreMemPolicyPpi);

  Status = UpdatePeiSpsPreMemPolicy (SiPreMemPolicyPpi);

  return Status;
#else // ME_SPS_SUPPORT
  return EFI_SUCCESS;
#endif // ME_SPS_SUPPORT
}
