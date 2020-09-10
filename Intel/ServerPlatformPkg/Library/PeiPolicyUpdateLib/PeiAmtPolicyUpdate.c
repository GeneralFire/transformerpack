/** @file
  Do Platform Stage AMT initialization.

@copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2019 Intel Corporation.

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

#include "PeiAmtPolicyUpdate.h"
#if AMT_SUPPORT
#include <AmtForcePushPetPolicy.h>
#include <MebxDataHob.h>
#include <AlertStandardFormat.h>
#endif // AMT_SUPPORT
#include <Library/ConfigBlockLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Guid/MeRcVariable.h>
#include <ConfigBlock/Amt/AmtConfig.h>
#include <Library/SetupLib.h>
#include <Library/MeTypeLib.h>

#if AMT_SUPPORT
GLOBAL_REMOVE_IF_UNREFERENCED ASF_FRAMEWORK_MESSAGE_TYPE mAmtForcePushPetPolicy[] = {
  AsfMessageChassisIntrusion,
  AsfMessageUserAuthenticationInvalid,
  AsfMessageHddAuthenticationInvalid,
};
#endif // AMT_SUPPORT

/**
  Install the Active Management push PET policy
**/
VOID
InstallAmtForcePushPetPolicy (
  VOID
  )
{
#if AMT_SUPPORT
  VOID        *Hob;

  //
  // Build HOB for information needed to push to PET
  //
  Hob = BuildGuidDataHob (
          &gAmtForcePushPetPolicyGuid,
          &mAmtForcePushPetPolicy,
          sizeof (mAmtForcePushPetPolicy)
          );

  //
  // Cannot Build HOB
  //
  ASSERT (Hob != NULL);

  return;
#endif // AMT_SUPPORT
}

/**
  Install the Active Management Policy Ppi Library

  @param[in, out] SiPolicyPpi   PEI Si Policy

  @retval EFI_SUCCESS           Initialization complete.
  @retval EFI_UNSUPPORTED       The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR      Device error, driver exits abnormally.
**/
EFI_STATUS
UpdatePeiAmtPolicy (
  IN OUT  SI_POLICY_PPI *SiPolicyPpi
  )
{
#if AMT_SUPPORT
  EFI_STATUS                      Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;
  UINTN                           VariableSize;
  ME_RC_CONFIGURATION             MeSetup;
  AMT_PEI_CONFIG                  *AmtPeiConfig;
  VOID                            *MebxSavedData;

  if (!MeTypeIsClient ()) {
    return EFI_SUCCESS;
  }
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gAmtPeiConfigGuid, (VOID *) &AmtPeiConfig);
  ASSERT_EFI_ERROR (Status);

  //
  // Locate Variable Ppi
  //
  Status = PeiServicesLocatePpi (&gEfiPeiReadOnlyVariable2PpiGuid, 0, NULL, (VOID **) &VariableServices);

  //
  // Make sure we have a PPI, if not, just return.
  //
  if (EFI_ERROR (Status)) {
    return EFI_SUCCESS;
  }

  Status = GetSpecificConfigGuid (&gEfiMeRcVariableGuid, (VOID *)&MeSetup);

  if (!EFI_ERROR (Status)) {
    AmtPeiConfig->AmtEnabled         = MeSetup.Amt;
    AmtPeiConfig->WatchDogEnabled    = MeSetup.WatchDog;
    AmtPeiConfig->WatchDogTimerBios  = MeSetup.WatchDogTimerBios;
    AmtPeiConfig->WatchDogTimerOs    = MeSetup.WatchDogTimerOs;
    AmtPeiConfig->FwProgress         = MeSetup.FWProgress;
  }

  VariableSize                 = 0;
  MebxSavedData                = NULL;
  AmtPeiConfig->ForcMebxSyncUp = 1;
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"MeBiosExtensionSetup",
                               &gMeBiosExtensionSetupGuid,
                               NULL,
                               &VariableSize,
                               MebxSavedData
                               );
  if (Status == EFI_BUFFER_TOO_SMALL) {
    MebxSavedData = AllocateZeroPool (VariableSize);
    ASSERT (MebxSavedData != NULL);
    if (MebxSavedData == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    DEBUG ((DEBUG_INFO, "VariableSize is 0x%x\n", VariableSize));
    Status = VariableServices->GetVariable (
                                 VariableServices,
                                 L"MeBiosExtensionSetup",
                                 &gMeBiosExtensionSetupGuid,
                                 NULL,
                                 &VariableSize,
                                 MebxSavedData
                                 );
    if (!EFI_ERROR (Status)) {
      AmtPeiConfig->ManageabilityMode = ((MEBX_DATA_HOB *)MebxSavedData)->PlatformMngSel;
      AmtPeiConfig->AmtSolEnabled     = ((MEBX_DATA_HOB *)MebxSavedData)->AmtSol;
      AmtPeiConfig->RemoteAssistance  = ((MEBX_DATA_HOB *)MebxSavedData)->RemoteAssistanceTriggerAvailablilty;
      AmtPeiConfig->AmtKvmEnabled     = ((MEBX_DATA_HOB *)MebxSavedData)->KvmEnable;
      AmtPeiConfig->ForcMebxSyncUp    = 0;
    } else {
      DEBUG ((DEBUG_WARN, "Failed to retrieve Variable:\"MeBiosExtensionSetup\", Status = %r\n", Status));
    }
  }

  InstallAmtForcePushPetPolicy ();

  return Status;
#else // AMT_SUPPORT
  return EFI_SUCCESS;
#endif // AMT_SUPPORT
}

