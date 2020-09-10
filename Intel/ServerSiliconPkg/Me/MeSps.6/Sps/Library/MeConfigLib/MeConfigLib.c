/** @file
  MeConfigLib.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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
#include <PiDxe.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <MeDefines.h>
#include <Library/MeTypeLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/MeConfigLib.h>
#include <Register/HeciRegs.h>
#include <MeAccess.h>
#include <Protocol/SpsSmmHmrfpoProtocol.h>
#include <MeConfigPeiDxe.h>

/**

  @brief
  Function resolves AutoSettings for particular configuration entry

  @param[in] Device             Device to resolve auto-configuration
  @param[in] ConfigToResolve    Auto-configuration to be resolved
  @param[in] MeMode             ME Mode
  @param[in] MeIsCorporateWs    Is Corporate AMT is present at the platform?

  @retval resolved auto-configuration

**/
UINT32
MeConfigDeviceAutoResolve(
  IN     ME_AUTO_CONFIG Device,
  IN     UINT32         ConfigToResolve,
  IN     BOOLEAN        MeIsCorporateWs
  )
{
  UINT32                retval;
#ifdef ME_WS_SUPPORT
  HECI_FWS_REGISTER     Mefs1;
#endif

  retval = ME_DEVICE_DISABLED;

  if (ConfigToResolve != ME_DEVICE_AUTO) {
    DEBUG ((DEBUG_INFO, "[ME Policy] Not Auto-configuration (%d) passed for device %d\n", ConfigToResolve, Device));
    return ConfigToResolve;
  }
  //
  // If DFX all ME devices should be enabled
  //
  if (MeTypeIsDfx ()) {
    retval = ME_DEVICE_ENABLED;
    goto MeConfigDeviceAutoResolveExit;
  }
  //
  // If ME is disabled all ME devices should be disabled
  //
  if (MeTypeIsDisabled ()) {
    if (MeTypeIsSpsIgnition () && (Device == ME_AUTO_CONFIG_HEC1)) {
      // Enable HECI 1 for Ignition SPS
      retval = ME_DEVICE_ENABLED;
    }
    goto MeConfigDeviceAutoResolveExit;
  }

  switch (Device) {
    case ME_AUTO_CONFIG_HEC1:
#ifdef ME_SPS_SUPPORT
      if (MeTypeIsSps ()) {
        retval = ME_DEVICE_ENABLED;
        goto MeConfigDeviceAutoResolveExit;
      }
#endif // ME_SPS_SUPPORT
#ifdef ME_WS_SUPPORT
      if (MeTypeIsClient ()) {
        Mefs1.ul = HeciPciRead32 (R_ME_HFS);
        if ((Mefs1.r.MeOperationMode == ME_OPERATION_MODE_SECOVR_JMPR) ||
            (Mefs1.r.MeOperationMode == ME_OPERATION_MODE_SOFT_TEMP_DISABLE)) {
          goto MeConfigDeviceAutoResolveExit;
        }
        retval = ME_DEVICE_ENABLED;
        goto MeConfigDeviceAutoResolveExit;
      }
#endif // ME_WS_SUPPORT
      break;

    case ME_AUTO_CONFIG_HEC2:
#ifdef ME_SPS_SUPPORT
      if (MeTypeIsSps ()) {
        if (MeSpsIsNm ()) {
          retval = ME_DEVICE_ENABLED;
          goto MeConfigDeviceAutoResolveExit;
        }
        goto MeConfigDeviceAutoResolveExit;
      }
#endif // ME_SPS_SUPPORT
#ifdef ME_WS_SUPPORT
      if (MeTypeIsClient ()) {
        goto MeConfigDeviceAutoResolveExit;
      }
#endif // ME_WS_SUPPORT
      break;

    case ME_AUTO_CONFIG_HEC3:
#ifdef ME_SPS_SUPPORT
      if (MeTypeIsSps ()) {
        retval = ME_DEVICE_ENABLED;
        goto MeConfigDeviceAutoResolveExit;
      }
#endif // ME_SPS_SUPPORT
#ifdef ME_WS_SUPPORT
      if (MeTypeIsClient ()) {
        goto MeConfigDeviceAutoResolveExit;
      }
#endif // ME_WS_SUPPORT
      break;

    case ME_AUTO_CONFIG_HEC4:
        goto MeConfigDeviceAutoResolveExit;

    case ME_AUTO_CONFIG_SOL:
#ifdef ME_SPS_SUPPORT
      if (MeTypeIsSps ()) {
        goto MeConfigDeviceAutoResolveExit;
      }
#endif // ME_SPS_SUPPORT
#ifdef ME_WS_SUPPORT
      if (MeTypeIsClient ()) {
        if (MeIsCorporateWs) {
          Mefs1.ul = HeciPciRead32 (R_ME_HFS);
          if ((Mefs1.r.MeOperationMode == ME_OPERATION_MODE_SECOVR_JMPR) ||
              (Mefs1.r.MeOperationMode == ME_OPERATION_MODE_SOFT_TEMP_DISABLE)) {
            goto MeConfigDeviceAutoResolveExit;
          }
          if (ClientGetProvisioningState ()) {
            retval = ME_DEVICE_ENABLED;
          }
          goto MeConfigDeviceAutoResolveExit;
        }
        goto MeConfigDeviceAutoResolveExit;
      }
#endif // ME_WS_SUPPORT
      break;

    case ME_AUTO_CONFIG_IDER:
      goto MeConfigDeviceAutoResolveExit;

    default:
      DEBUG ((DEBUG_WARN, "[ME Policy] WARNING: Auto-configuration passed for unrecognised device %d\n", Device));
  }

  DEBUG ((DEBUG_WARN, "[ME Policy] WARNING: Unexpected ME type (MEFS1: %08X)\n", HeciPciRead32 (R_ME_HFS)));

MeConfigDeviceAutoResolveExit:
  DEBUG ((
    DEBUG_INFO,
    "[ME Policy] MeConfigDeviceAutoResolve (device = %d, ConfigToResolve = %d, MeIsCorporateWs = %d\n",
    Device,
    ConfigToResolve,
    MeIsCorporateWs
    ));
  DEBUG ((
    DEBUG_INFO,
    "[ME Policy] MeConfigDeviceAutoResolve returns %s\n",
    retval == ME_DEVICE_DISABLED ? L"ME_DEVICE_DISABLED" : L"ME_DEVICE_ENABLED"
    ));

  return retval;
}

/**
  Function sets default values in SPS_COMMON_CONFIG.

  @param[in] SpsCommonConfig      pointer to SPS_COMMON_CONFIG
**/
VOID
SetSpsCommonPolicyDefaults (
  IN     SPS_COMMON_CONFIG       *SpsCommonConfig
  )
{
#ifdef ME_SPS_SUPPORT
  SpsCommonConfig->SpsMctpBusOwner = ME_SPS_MCTP_BUS_OWNER_NONE;
  SpsCommonConfig->CoreBiosDoneEnabled = 1;
  SpsCommonConfig->Heci1HideInMe = ME_SPS_HECI_IN_ME_OFF;
  SpsCommonConfig->Heci2HideInMe = ME_SPS_HECI_IN_ME_OFF;
  SpsCommonConfig->Heci3HideInMe = ME_SPS_HECI_IN_ME_OFF;
  SpsCommonConfig->Heci4HideInMe = ME_SPS_HECI_IN_ME_OFF;
  SpsCommonConfig->MeHmrfpoLockEnabled = 1;
  SpsCommonConfig->MeHmrfpoEnableEnabled = 0;
  SpsCommonConfig->NmCores2DisableOverride = 0;
  SpsCommonConfig->NmCores2Disable = 0;
  SpsCommonConfig->NmPwrOptBootOverride = 0;
  SpsCommonConfig->NmPwrOptBoot = 0;
  SpsCommonConfig->NmPowerMsmtSupport = 0;
  SpsCommonConfig->NmHwChangeStatus = 0;
  SpsCommonConfig->MeWatchdogControlEnabled = 1;
  SpsCommonConfig->MeWatchdogDisable = 0;
  SpsCommonConfig->MeRegionSelectEnabled = 0;
#endif // ME_SPS_SUPPORT
}