/** @file
  MeConfigPei.c

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

#include <PiPei.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <PiDxe.h>
#ifdef AMT_SUPPORT
#include <Library/PeiServicesLib.h>
#include <Ppi/MemoryDiscovered.h>
#include <MkhiMsgs.h>
#include <Library/PeiMeLib.h>
#include <Library/MeTypeLib.h>
#include <Register/HeciRegs.h>
#include <MeAccess.h>
#endif // AMT_SUPPORT
#include <PchLimits.h>
#include <Library/MeConfigLib.h>
#include <Library/MeUtilsLib.h>
#include <ConfigBlock/Sps/SpsPeiConfig.h>
#include <ConfigBlock/MeGetConfigBlock.h>
#include <MeConfigPeiDxe.h>

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
  AMT_BIOS_SYNC_INFO      RuleData;
  VOID                    *Signal;
  EFI_STATUS              Status;
  HECI_FWS_REGISTER       MeFs1;
  SPS_PEI_CONFIG          *SpsPeiConfig;

  if (!MeTypeIsClient ()) {
    return FALSE;
  }

  Status = PeiServicesLocatePpi (&gEfiPeiMemoryDiscoveredPpiGuid, 0, NULL, &Signal);
  if (!EFI_ERROR (Status)) {
    MeFs1.ul = HeciPciRead32 (R_ME_HFS);
    if ((MeFs1.r.FwInitComplete == 1) && (MeFs1.ul != 0xFFFFFFFF)) {
      Status = PeiHeciGetAmtBiosSyncInfo (&RuleData);
      if (!EFI_ERROR (Status)) {
        if (RuleData.ProvisioningAvailable &&
            RuleData.ProvisioningState == AmtProvisioningStateIn) {
          return TRUE;
        }
      } else {
        // Can't get message, use last valid
        SpsPeiConfig = GetSpsPreMemPeiConfigBlock (NULL);
        if (SpsPeiConfig != NULL) {
          return (BOOLEAN) (SpsPeiConfig->SolProvisioningState);
        }
      }
    }
  }
#endif // AMT_SUPPORT

  return FALSE;
}

/**
  Function sets default values in SPS_PEI_CONFIG.

  @param[in] SpsPeiConfig      pointer to SPS_PEI_CONFIG
**/
VOID
SetSpsPeiPolicyDefaults (
  IN     SPS_PEI_CONFIG       *SpsPeiConfig
  )
{
#ifdef ME_SUPPORT_FLAG
  if (SpsPeiConfig == NULL) {
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
    return;
  }

  SpsPeiConfig->HsioMessaging = 1;
  SpsPeiConfig->IderDeviceEnable = MeConfigDeviceAutoResolve (
                                     ME_AUTO_CONFIG_IDER,
                                     ME_DEVICE_AUTO,
                                     MeClientIsCorporate ()
                                     );
  SpsPeiConfig->HeciCommunication4 = MeConfigDeviceAutoResolve (
                                       ME_AUTO_CONFIG_HEC4,
                                       ME_DEVICE_AUTO,
                                       MeClientIsCorporate ()
                                       );

#ifdef ME_SPS_SUPPORT
  SpsPeiConfig->PreDidMeResetEnabled = 0;
  SpsPeiConfig->MeTimeout = ME_SPS_INIT_TIMEOUT;
  SpsPeiConfig->SpsIccClkSscSetting = 1;
  SpsPeiConfig->FiaMuxMessageEnabled = 0;

  SetSpsCommonPolicyDefaults (&(SpsPeiConfig->Common));
#endif // ME_SPS_SUPPORT
  SpsPeiConfig->SolProvisioningState = 0;
#endif // ME_SUPPORT_FLAG
}
