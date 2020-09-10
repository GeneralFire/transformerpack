/** @file

@copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation.

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

#include "PeiMePolicyUpdate.h"
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <SetupVariable.h>
#include <Library/ConfigBlockLib.h>

#include <Guid/MeRcVariable.h>
#include <ConfigBlock/Me/MePeiConfig.h>
#include <Library/SetupLib.h>
#include <Library/MeConfigLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/HeciConfigureLib.h>

/**
  Update the ME Policy Library

  @param[in] SiPreMemPolicyPpi  The pointer to SiPreMemPolicyPpi

  @retval EFI_SUCCESS           Update complete.
**/
EFI_STATUS
UpdatePeiMePolicyPreMem (
  IN OUT SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                      Status;
#if ME_SUPPORT_FLAG
  ME_RC_CONFIGURATION             MeSetup;
  BOOLEAN                         MeIsCorporateWs = FALSE;
  ME_PEI_PREMEM_CONFIG            *MePeiPreMemConfig;

  DEBUG ((DEBUG_INFO, "UpdatePeiMePolicyPreMem\n"));

  Status = GetSpecificConfigGuid (&gEfiMeRcVariableGuid, (VOID *)&MeSetup);
  if (!EFI_ERROR (Status)) {
    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMePeiPreMemConfigGuid, (VOID *) &MePeiPreMemConfig);
    ASSERT_EFI_ERROR (Status);
#if AMT_SUPPORT
    if (GetBootModeHob () == BOOT_ON_S3_RESUME) {
      MeIsCorporateWs = MeSetup.MeIsCorporateWs;
    } else {
      MeIsCorporateWs = MeClientIsCorporate ();
    }
#endif // AMT_SUPPORT
    MePeiPreMemConfig->HeciTimeouts              = 1;
#if SILENT_MODE
    MePeiPreMemConfig->HeciFullTrace             = 0;
#else // SILENT_MODE
    MePeiPreMemConfig->HeciFullTrace             = 1;
#endif // SILENT_MODE
#if ME_TESTMENU_FLAG
    MePeiPreMemConfig->HeciFullTrace             = MeSetup.MeEnableHeciTraces;
    MePeiPreMemConfig->DidInitStat               = MeSetup.DidInitStat;
    MePeiPreMemConfig->SendDidMsg                = MeSetup.SendDidMsg;
#else // ME_TESTMENU_FLAG
    MePeiPreMemConfig->DidInitStat               = 0;
    MePeiPreMemConfig->SendDidMsg                = 1;
#endif // ME_TESTMENU_FLAG
    MePeiPreMemConfig->DisableCpuReplacedPolling = 0;
    MePeiPreMemConfig->SkipMbpHob                = 0;

    MePeiPreMemConfig->HeciCommunication2        = MeConfigDeviceAutoResolve (
      ME_AUTO_CONFIG_HEC2,
#if ME_TESTMENU_FLAG
      MeSetup.HeciCommunication2,
#else // ME_TESTMENU_FLAG
      ME_DEVICE_AUTO,
#endif // ME_TESTMENU_FLAG
      MeIsCorporateWs
    );
    MePeiPreMemConfig->KtDeviceEnable            = MeConfigDeviceAutoResolve (
      ME_AUTO_CONFIG_SOL,
#if ME_TESTMENU_FLAG
      MeSetup.KtDeviceEnable,
#else // ME_TESTMENU_FLAG
      ME_DEVICE_AUTO,
#endif // ME_TESTMENU_FLAG
      MeIsCorporateWs
    );
  }
#else // ME_SUPPORT_FLAG
  Status = EFI_SUCCESS;
#endif // ME_SUPPORT_FLAG

  return Status;
}
