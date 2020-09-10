/** @file
  Implements SPS Config Blocks initialization Helper for common data

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation.

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
#include <Library/MeConfigLib.h>
#include <ConfigBlock.h>
#include <Guid/MeRcVariable.h>
#include <ConfigBlock/SpsCommonConfig.h>
#include <Library/MeConfigLib.h>
#include <Library/MeServerPolicyHelperLib.h>
#include <MeServerPolicyHelper.h>

/**
  Function copies setup data in to SPS_PEI_CONFIG object.

  @param[in]  MeSetup           Input pointer to ME_RC_CONFIGURATION
  @param[out] SpsCommonConfig   Pointer to SPS_COMMON_CONFIG

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_INVALID_PARAMETER Wrong pointer passed to the function
**/
EFI_STATUS
CopyDataOfSpsCommonConfig (
  IN     ME_RC_CONFIGURATION  *MeSetup,
     OUT SPS_COMMON_CONFIG    *SpsCommonConfig
  )
{
#if ME_SUPPORT_FLAG
  EFI_STATUS           Status;
  ME_RC_CONFIGURATION  MeRcSetup;

  if (SpsCommonConfig == NULL) {
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
    return EFI_INVALID_PARAMETER;
  }

  if (MeSetup == NULL) {
    MeSetup = &MeRcSetup;
    Status = GetSpecificConfigGuid (&gEfiMeRcVariableGuid, (VOID *)MeSetup);
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return Status;
    }
  }

#if ME_SPS_SUPPORT
  SpsCommonConfig->SpsMctpBusOwner = MeSetup->SpsMctpBusOwner;
  SpsCommonConfig->Heci1HideInMe = MeSetup->MeHeci1HideInMe;
  SpsCommonConfig->Heci2HideInMe = MeSetup->MeHeci2HideInMe;
  SpsCommonConfig->Heci3HideInMe = MeSetup->MeHeci3HideInMe;
  SpsCommonConfig->Heci4HideInMe = MeSetup->MeHeci4HideInMe;
  SpsCommonConfig->NmPwrOptBoot = MeSetup->NmPwrOptBoot;
#if ME_TESTMENU_FLAG
  SpsCommonConfig->MeHmrfpoLockEnabled = MeSetup->MeHmrfpoLockEnabled;
  SpsCommonConfig->MeHmrfpoEnableEnabled = MeSetup->MeHmrfpoEnableEnabled;
  SpsCommonConfig->NmCores2Disable = MeSetup->NmCores2Disable;
  SpsCommonConfig->NmPwrOptBootOverride = MeSetup->NmPwrOptBootOverride;
  SpsCommonConfig->NmCores2DisableOverride = MeSetup->NmCores2DisableOverride;
  SpsCommonConfig->NmPowerMsmtSupport = MeSetup->NmPowerMsmtSupport;
  SpsCommonConfig->NmHwChangeStatus = MeSetup->NmHwChangeStatus;
  SpsCommonConfig->NmPwrOptBootOverride = 0;
  SpsCommonConfig->MeWatchdogControlEnabled = MeSetup->MeWatchdogControlEnabled;
  if (SpsCommonConfig->MeWatchdogControlEnabled == 0) {
    SpsCommonConfig->MeWatchdogDisable = MeSetup->MeWatchdogDisable;
  } else {
    SpsCommonConfig->MeWatchdogDisable = 0;
  }
  SpsCommonConfig->MeRegionSelectEnabled = MeSetup->MeRegionSelectEnabled;
#endif // ME_TESTMENU_FLAG
#endif // ME_SPS_SUPPORT
  SpsCommonConfig->CoreBiosDoneEnabled = MeSetup->CoreBiosDoneEnabled;
#endif // ME_SUPPORT_FLAG

  return EFI_SUCCESS;
} // CopyDataOfSpsCommonConfig
