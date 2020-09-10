/** @file
  The library provides functions relevant to SPS NM.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation.

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

#include <Base.h>
#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/SetupLib.h>
#include <Library/PmcLib.h>
#include <Library/MeTypeLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/NmLib.h>
#include <Guid/MeRcVariable.h>
#include <Ppi/Stall.h>
#include <Sps.h>
#include <MeAccess.h>
#include <HeciRegs.h>

/**
  Return information if the SPS NM SPS NM requires Power Optimized Mode or Performance Optimized Mode.

  @param[out] BiosBootingMode     In case the function returns EFI_SUCCESS, then this variable
                                  stores information if the SPS NM requires Power Optimized Mode
                                  or Performance Optimized Mode

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_INVALID_PARAMETER   Invalid pointer to BiosBootingMode
  @retval EFI_UNSUPPORTED         The ME doesn't run SPS firmware or SPS NM is disabled
  @retval EFI_TIMEOUT             Timeout while waiting for the ME
  @retval EFI_ABORTED             In case of the boot after dirty warm reset,
                                  the execution of the function is aborted
  @retval EFI_NOT_FOUND           Cannot locate MeRcVariable config
**/
EFI_STATUS
GetBootingModeRequestedBySpsNm (
  OUT BIOS_BOOTING_MODE  *BiosBootingMode
  )
{
#if ME_SPS_SUPPORT
  HECI_FWS_REGISTER        MeFs1;
  SPS_NMFS                 NmFs;
  UINT32                   Timeout;
  EFI_PEI_STALL_PPI        *pStallPpi;
  BOOLEAN                  PwrOptBoot;
  BOOLEAN                  NmPwrOptBootOverride;
  EFI_STATUS               Status;
  EFI_BOOT_MODE            BootMode;
  ME_RC_CONFIGURATION      MeSetup;
  CONST EFI_PEI_SERVICES   **PeiServices;

  DEBUG ((DEBUG_INFO, "[SPS] %a called.\n", __FUNCTION__));

  PwrOptBoot           = FALSE;
  NmPwrOptBootOverride = FALSE;
  PeiServices          = GetPeiServicesTablePointer ();

  if (BiosBootingMode == NULL) {
    Status = EFI_INVALID_PARAMETER;
    goto GetBootingModeRequestedBySpsNmExit;
  }

  if (PmcIsDwrBootMode ()) {
    Status = EFI_ABORTED;
    goto GetBootingModeRequestedBySpsNmExit;
  }

  MeFs1.ul    = HeciPciRead32 (R_ME_HFS);
  NmFs.UInt32 = Heci2PciRead32 (SPS_REG_NMFS);

  if (!MeTypeIsSps ()) {
    // do not proceed for non-SPS firmware
    DEBUG ((DEBUG_WARN, "[SPS] Non SPS firmware in ME (MEFS1: 0x%08X, NMFS: 0x%08X)\n", MeFs1.ul, NmFs.UInt32));
    Status = EFI_UNSUPPORTED;
    goto GetBootingModeRequestedBySpsNmExit;
  }

  if (HybridSystemLevelEmulationEnabled ()) {
    Timeout = 2;
  } else {
    Timeout = 500;
  }

  //
  // Determine boot mode
  //
  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  if (EFI_ERROR (Status)) {
    goto GetBootingModeRequestedBySpsNmExit;
  }

  Status = GetSpecificConfigGuid (&gEfiMeRcVariableGuid, (VOID*) &MeSetup);
  ASSERT_EFI_ERROR (Status);

  if (!EFI_ERROR (Status)) {
    PwrOptBoot = MeSetup.NmPwrOptBoot;
#if ME_TESTMENU_FLAG
    NmPwrOptBootOverride = MeSetup.NmPwrOptBootOverride;
#endif // ME_TESTMENU_FLAG
  }

  if (BootMode == BOOT_ON_S4_RESUME) {
    if (EFI_ERROR (Status)) {
      Status = EFI_NOT_FOUND;
    }
    goto GetBootingModeRequestedBySpsNmExit;
  }

  //
  // If it is SPS firmware running in ME and Node Manager is enabled
  // apply performance vs power optimized boot path.
  // Note that default for PwrOptBoot is neutral so
  // for non SPS firmware or ME error BIOS default is not changed.
  //
  if (BootMode != BOOT_ON_S3_RESUME) {
    PeiServicesLocatePpi (&gEfiPeiStallPpiGuid, 0, NULL, &pStallPpi);
    while (TRUE) {
      MeFs1.ul    = HeciPciRead32 (R_ME_HFS);
      NmFs.UInt32 = Heci2PciRead32 (SPS_REG_NMFS);

      if (!MeTypeIsSps ()) {
        // do not proceed for non-SPS firmware
        DEBUG ((DEBUG_WARN, "[SPS] Non SPS firmware in ME (MEFS1: 0x%08X, NMFS: 0x%08X)\n", MeFs1.ul, NmFs.UInt32));
        Status = EFI_UNSUPPORTED;
        break;
      }

      if (MeFs1.r.ErrorCode != 0) {
        DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Invalid ME state (MEFS1: 0x%08X)\n", MeFs1.ul));
        Status = EFI_UNSUPPORTED;
        break;
      }

      if (MeSpsIsNm ()) {
        if (!NmPwrOptBootOverride) {
          PwrOptBoot = !NmFs.Bits.PerfOptBoot;
        }
        DEBUG ((
          DEBUG_INFO,
          "[SPS] NM firmware detected in ME, boot mode: %d, (NMFS: 0x%08X)\n",
          !PwrOptBoot,
          NmFs.UInt32
          ));

        Status = EFI_SUCCESS;
        break;
      }
      if (MeFs1.r.CurrentState >= ME_STATE_RECOVERY) {
        DEBUG ((DEBUG_ERROR, "[SPS] NM not enabled in ME (MEFS1: 0x%08X, NMFS: 0x%08X)\n", MeFs1.ul, NmFs.UInt32));
        Status = EFI_UNSUPPORTED;
        break;
      }
      if (Timeout-- == 0) {
        DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Timeout waiting for ME (MEFS1: 0x%08X)\n", MeFs1.ul));
        Status = EFI_TIMEOUT;
        break;
      }
      if (pStallPpi != NULL) {
        if (!(HybridSystemLevelEmulationEnabled ())) {
          pStallPpi->Stall (PeiServices, pStallPpi, 1000);
        }
      }
    }
  }

GetBootingModeRequestedBySpsNmExit:
  if (!EFI_ERROR (Status)) {
    if (PwrOptBoot) {
      *BiosBootingMode = PowerOptimizedMode;
    } else {
      *BiosBootingMode = PerformanceOptimizedMode;
    }
  }

  DEBUG ((DEBUG_INFO, "[SPS] %a exit, return = %r.\n", __FUNCTION__, Status));
  return Status;

#else // ME_SPS_SUPPORT
  return EFI_UNSUPPORTED;
#endif // ME_SPS_SUPPORT
}