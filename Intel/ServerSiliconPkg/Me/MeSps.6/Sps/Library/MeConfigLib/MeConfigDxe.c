/** @file
  MeConfigDxe.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <PiDxe.h>
#ifdef AMT_SUPPORT
#include <MkhiMsgs.h>
#include <Library/DxeMeLib.h>
#include <Library/MeTypeLib.h>
#endif // AMT_SUPPORT
#include <MeConfigPeiDxe.h>
#include <MeDefines.h>

/**
  Function asks Client's ME if provisioning is active.
  Should be called for Client's ME only

  @retval TRUE    Provisioning is active.
  @retval FALSE   Provisioning is not active.
**/
BOOLEAN
ClientGetProvisioningState (
  VOID
  )
{
#ifdef AMT_SUPPORT
  AMT_BIOS_SYNC_INFO    RuleData;
  EFI_STATUS            Status;

  if (MeTypeIsClient ()) {
    Status = HeciGetAmtBiosSyncInfo (&RuleData);
    if (!EFI_ERROR (Status) &&
        RuleData.ProvisioningAvailable &&
        RuleData.ProvisioningState == AmtProvisioningStateIn) {
      return TRUE;
    }
  }
#endif // AMT_SUPPORT

  return FALSE;
}

/**
  Function sets default values in SPS_DXE_CONFIG.

  @param[in] SpsPeiConfig      pointer to SPS_DXE_CONFIG
**/
VOID
SetSpsDxePolicyDefaults (
  IN     SPS_DXE_CONFIG       *SpsDxeConfig
  )
{
  if (SpsDxeConfig == NULL) {
    ASSERT (FALSE);
    return;
  }
  SpsDxeConfig->DelayedAuthenticationMode = 0;
  SpsDxeConfig->DelayedAuthenticationModeOverride = 0;
  SpsDxeConfig->MeHmrfpoViaHeci3 = 0;
  SpsDxeConfig->MeGrLockEnabled = 1;
  SpsDxeConfig->MeGrPromotionEnabled = 1;
  SpsDxeConfig->BreakRtcEnabled = 0;
  SpsDxeConfig->NmPowerMsmtOverride = 0;
  SpsDxeConfig->NmHwChangeOverride = 0;
  SpsDxeConfig->NmPtuLoadOverride = 0;
  SpsDxeConfig->SpsAltitude = ME_SPS_ALTITUDE_NONE;
  SpsDxeConfig->MeHmrfpoEnableRegion = 0;

  SetSpsCommonPolicyDefaults (&(SpsDxeConfig->Common));
}