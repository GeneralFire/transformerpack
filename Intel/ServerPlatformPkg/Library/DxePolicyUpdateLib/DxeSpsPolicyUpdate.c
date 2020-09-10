/** @file

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

#include "DxeSpsPolicyUpdate.h"
#include <ConfigBlock/SpsDxeConfig.h>
#include <Protocol/MePolicy.h>
#include <Library/MeConfigLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MeServerPolicyHelperLib.h>
#include <Library/MeUtilsLib.h>
#include <MeBiosPayloadHob.h>

VOID
EFIAPI
ShowMeReportError (
  IN ME_ERROR_MSG_ID              MsgId
  );

//
// Function implementations executed during policy initialization phase
//
EFI_STATUS
SaveSpsPolicyToSetup (
   SPS_DXE_CONFIG *SpsPolicyInstance
  )
{
  EFI_STATUS            Status;
  ME_RC_CONFIGURATION   MeRcConfiguration;
  ME_RC_CONFIGURATION   MeRcConfigurationOrg;
  UINT8                 MeType;

  DEBUG ((DEBUG_INFO, "[SPS Policy] SaveSpsPolicyToSetup\n"));

  Status = GetSpecificConfigGuid (&gEfiMeRcVariableGuid, (VOID*)&MeRcConfiguration);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "[SPS Policy] ERROR: Failed to read ME_RC_CONFIGURATION!  Status = %r\n", Status));
    return EFI_NOT_READY;
  }

  CopyMem (&MeRcConfigurationOrg, &MeRcConfiguration, sizeof(ME_RC_CONFIGURATION));
  MeType = (UINT8) MeTypeGet ();
  if ((MeRcConfiguration.MeType == METYPE_UNDEF || MeRcConfiguration.MeType == METYPE_DISABLED)
        && MeType != MeRcConfiguration.MeType) {
    MeRcConfiguration.MeType = MeType;
  }
#ifdef ME_WS_SUPPORT
  if (MeTypeIsClient ()) {
    ME_BIOS_PAYLOAD_HOB *MbpHob = GetFirstGuidHob(&gMeBiosPayloadHobGuid);

    if (MbpHob) {
      MeRcConfiguration.MeImageType = (UINT8) MbpHob->MeBiosPayload.FwPlatType.RuleData.Fields.IntelMeFwImageType;
    }
    MeRcConfiguration.SolProvisioningState = (UINT8) ClientGetProvisioningState ();
  }
#endif // ME_WS_SUPPORT

#ifdef ME_SPS_SUPPORT
  MeRcConfiguration.SpsAltitude = SpsPolicyInstance->SpsAltitude;
  MeRcConfiguration.SpsMctpBusOwner = SpsPolicyInstance->Common.SpsMctpBusOwner;
  MeRcConfiguration.MeHeci1HideInMe = (UINT8) SpsPolicyInstance->Common.Heci1HideInMe;
  MeRcConfiguration.MeHeci2HideInMe = (UINT8) SpsPolicyInstance->Common.Heci2HideInMe;
  MeRcConfiguration.MeHeci3HideInMe = (UINT8) SpsPolicyInstance->Common.Heci3HideInMe;
  MeRcConfiguration.MeHeci4HideInMe = (UINT8) SpsPolicyInstance->Common.Heci4HideInMe;
  MeRcConfiguration.NmCores2Disable = (UINT8) SpsPolicyInstance->Common.NmCores2Disable;
#if ME_TESTMENU_FLAG
  MeRcConfiguration.MeHmrfpoViaHeci3 = (UINT8) SpsPolicyInstance->MeHmrfpoViaHeci3;
  MeRcConfiguration.MeHmrfpoLockEnabled = (UINT8) SpsPolicyInstance->Common.MeHmrfpoLockEnabled;
  MeRcConfiguration.MeHmrfpoEnableEnabled = (UINT8) SpsPolicyInstance->Common.MeHmrfpoEnableEnabled;
  MeRcConfiguration.MeHmrfpoEnableRegion = (UINT8) SpsPolicyInstance->MeHmrfpoEnableRegion;
  MeRcConfiguration.NmPwrOptBootOverride = (UINT8) SpsPolicyInstance->Common.NmPwrOptBootOverride;
  MeRcConfiguration.NmCores2DisableOverride = (UINT8) SpsPolicyInstance->Common.NmCores2DisableOverride;
  MeRcConfiguration.NmPowerMsmtOverride = (UINT8) SpsPolicyInstance->NmPowerMsmtOverride;
  MeRcConfiguration.NmPowerMsmtSupport = (UINT8) SpsPolicyInstance->Common.NmPowerMsmtSupport;
  MeRcConfiguration.NmHwChangeOverride = (UINT8) SpsPolicyInstance->NmHwChangeOverride;
  MeRcConfiguration.NmHwChangeStatus = (UINT8) SpsPolicyInstance->Common.NmHwChangeStatus;
  MeRcConfiguration.NmPtuLoadOverride = (UINT8) SpsPolicyInstance->NmPtuLoadOverride;
  MeRcConfiguration.MeGrLockEnabled = (UINT8) SpsPolicyInstance->MeGrLockEnabled;
  MeRcConfiguration.MeGrPromotionEnabled = (UINT8) SpsPolicyInstance->MeGrPromotionEnabled;
  MeRcConfiguration.BreakRtcEnabled = (UINT8) SpsPolicyInstance->BreakRtcEnabled;
#endif // ME_TESTMENU_FLAG
#endif // SPS_SUPORT
  if (CompareMem (&MeRcConfigurationOrg, &MeRcConfiguration, sizeof (ME_RC_CONFIGURATION)) != 0) {
    Status = SetSpecificConfigGuid (&gEfiMeRcVariableGuid, (VOID *) &MeRcConfiguration);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[SPS Policy] ERROR: Failed to save ME policy to ME RC Variable (%r)\n", Status));
    }
  }

  return Status;
}

/**
  Update the SPS Policy Library

  @param[in, out] DxeSpsPolicy           The pointer to get SPS Policy protocol instance

  @retval EFI_SUCCESS                   Initialization complete.
  @retval EFI_UNSUPPORTED               The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES          Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR              Device error, driver exits abnormally.

**/
EFI_STATUS
EFIAPI
UpdateDxeSpsPolicy (
  IN OUT  SPS_POLICY_PROTOCOL            *DxeSpsPolicy
  )
{
  EFI_STATUS              Status;
  EFI_EVENT               EndOfDxeEvent;
  SPS_DXE_CONFIG          *SpsDxeConfig;

  DEBUG ((DEBUG_INFO, "UpdateDxeSpsPolicy\n"));
  UpdateSpsPolicyFromSetup (DxeSpsPolicy);

  Status = GetConfigBlock ((VOID *) DxeSpsPolicy, &gSpsDxeConfigGuid, (VOID *) &SpsDxeConfig);
#if ME_SPS_SUPPORT
  ASSERT (SpsDxeConfig != NULL);
#endif // ME_SPS_SUPPORT
  if ((!EFI_ERROR (Status)) && (SpsDxeConfig != NULL)) {
#ifdef ME_SUPPORT_FLAG
    Status = CopyDataOfSpsDxeConfig (NULL, SpsDxeConfig);
#if ME_SPS_SUPPORT
    ASSERT_EFI_ERROR (Status);
#endif // ME_SPS_SUPPORT
    SpsDxeConfig->MeReportError = ShowMeReportError;
    SpsDxeConfig->SpsSavePolicy = SaveSpsPolicyToSetup;
#endif // ME_SUPPORT_FLAG
  }

  //
  // Register End of DXE event
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  UpdateSpsSetupCallback,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Update SPS Policy while SpsPlatformProtocol is installed.

  @param[in] SpsPolicyInstance     Instance of SPS Policy Protocol

**/
VOID
UpdateSpsPolicyFromSetup (
  IN SPS_POLICY_PROTOCOL           *SpsPolicyInstance
  )
{
#if ME_TESTMENU_FLAG
  EFI_STATUS              Status;
  ME_RC_CONFIGURATION     MeSetup;
  ME_RC_CONFIGURATION     *MeSetupPtr;
  SPS_DXE_CONFIG          *SpsDxeConfig = NULL;

  DEBUG ((DEBUG_INFO, "UpdateSpsPolicyFromSetup\n"));


  //
  // Get SPS configuration from Setup Data
  //
  Status = GetSpecificConfigGuid (&gEfiMeRcVariableGuid, (VOID *)&MeSetup);
  if (EFI_ERROR (Status)) {
    MeSetupPtr = NULL;
  } else {
    MeSetupPtr = &MeSetup;
  }

  Status = GetConfigBlock ((VOID *) SpsPolicyInstance, &gSpsDxeConfigGuid, (VOID *) &SpsDxeConfig);
#if ME_SPS_SUPPORT
  ASSERT_EFI_ERROR (Status);
  if (!EFI_ERROR (Status)) {
    Status = CopyDataOfSpsDxeConfig (&MeSetup, SpsDxeConfig);
    ASSERT_EFI_ERROR (Status);
  }
#endif // ME_SPS_SUPPORT

  DEBUG ((DEBUG_INFO, "UpdateSpsPolicyFromSetup done, Status: %r\n", Status));
#endif // ME_TESTMENU_FLAG
}

/**
  Functions performs HECI exchange with FW to update SpsPolicy settings.

  @param[in] Event         A pointer to the Event that triggered the callback.
  @param[in] Context       A pointer to private data registered with the callback function.

**/
VOID
EFIAPI
UpdateSpsSetupCallback (
  IN  EFI_EVENT                   Event,
  IN  VOID                        *Context
  )
{
  DEBUG ((DEBUG_INFO, "UpdateSpsSetup event start\n"));


  gBS->CloseEvent (Event);

  DEBUG ((DEBUG_INFO, "UpdateSpsSetup event end\n"));

  return;
}

