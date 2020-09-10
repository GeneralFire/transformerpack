/** @file
  Implements SPS Config Blocks initialization Helper for PEI data

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

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
#include <Base.h>
#include <PiPei.h>
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SetupLib.h>
#include <ConfigBlock.h>
#include <Guid/MeRcVariable.h>
#include <ConfigBlock/SpsPeiConfig.h>
#include <Library/MeConfigLib.h>
#include <Library/MeServerPolicyHelperLib.h>
#include <MeServerPolicyHelper.h>

/**
  Function copies setup data in to SPS_PEI_CONFIG object.

  @param[in]  MeSetup           Input pointer to ME_RC_CONFIGURATION
  @param[out] SpsPeiConfig      Pointer to SPS_PEI_CONFIG

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_INVALID_PARAMETER Wrong pointer passed to the function
**/
EFI_STATUS
CopyDataOfSpsPreMemConfig (
  IN     ME_RC_CONFIGURATION  *MeSetup,
     OUT SPS_PEI_CONFIG       *SpsPeiConfig
   )
{
#if ME_SUPPORT_FLAG
  EFI_STATUS           Status;
  ME_RC_CONFIGURATION  MeRcSetup;

  if (SpsPeiConfig == NULL) {
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
    return EFI_INVALID_PARAMETER;
  }

  SetSpsPeiPolicyDefaults (SpsPeiConfig);

  if (MeSetup == NULL) {
    MeSetup = &MeRcSetup;
    Status = GetSpecificConfigGuid (&gEfiMeRcVariableGuid, (VOID*) MeSetup);
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return Status;
    }
  }
  SpsPeiConfig->SolProvisioningState = MeSetup->SolProvisioningState;

#if ME_TESTMENU_FLAG
  SpsPeiConfig->IderDeviceEnable = MeConfigDeviceAutoResolve (
    ME_AUTO_CONFIG_IDER,
    MeSetup->MeIderEnabled,
    FALSE
  );
  SpsPeiConfig->HsioMessaging = MeSetup->HsioMessagingEnabled;
  SpsPeiConfig->HeciCommunication4 = MeConfigDeviceAutoResolve (
    ME_AUTO_CONFIG_HEC4,
    MeSetup->HeciCommunication4,
    FALSE
  );
  SpsPeiConfig->FiaMuxMessageEnabled = MeSetup->FiaMuxMessageEnabled;
#endif // ME_TESTMENU_FLAG

#if ME_SPS_SUPPORT
  SpsPeiConfig->PreDidMeResetEnabled = MeSetup->PreDidMeResetEnabled;
#if ME_TESTMENU_FLAG
  SpsPeiConfig->MeTimeout = MeSetup->MeTimeout;
  SpsPeiConfig->SpsIccClkSscSetting = MeSetup->SpsIccClkSscSetting;
#endif // ME_TESTMENU_FLAG
#endif // ME_SPS_SUPPORT
#endif // ME_SUPPORT_FLAG

  CopyDataOfSpsCommonConfig (MeSetup, &(SpsPeiConfig->Common));

  return EFI_SUCCESS;
} // CopyDataOfSpsPreMemConfig
