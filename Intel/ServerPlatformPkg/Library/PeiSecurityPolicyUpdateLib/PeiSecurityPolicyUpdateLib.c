/** @file
  PEI Security policy update library implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <Library/BaseMemoryLib.h>
#include <Library/SecurityPolicyLib.h>
#include <Library/SetupLib.h>
#include <Library/DebugLib.h>

/**
  Update the Common RC policy from Setup.

  @param[in]  SetupData         A pointer to the setup data.

  @retval None

**/

VOID
GetPlatformSetupValueforSecurity (
  // APTIOV_SERVER_OVERRIDE_RC_START: To solve structure redefinition error in AMI.
  IN OUT INTEL_SETUP_DATA  *SetupData
  // APTIOV_SERVER_OVERRIDE_RC_END: To solve structure redefinition error in AMI.
  )
{
  SECURITY_POLICY *SecurityPolicy;

  SecurityPolicy = GetSecurityPolicy ();
  if (SecurityPolicy == NULL) {
    ASSERT (SecurityPolicy != NULL);
    DEBUG ((DEBUG_WARN, "Couldn't get policy\n"));
    return;
  }

  // TME
  SecurityPolicy->EnableTme                  = SetupData->SocketConfig.SocketProcessorCoreConfiguration.EnableTme;
  SecurityPolicy->EnableTmeCR                = SetupData->SocketConfig.SocketProcessorCoreConfiguration.EnableTmeCR;
  // MK-TME
  SecurityPolicy->EnableMktme                = SetupData->SocketConfig.SocketProcessorCoreConfiguration.EnableMktme;
  SecurityPolicy->MktmeIntegrity             = SetupData->SocketConfig.SocketProcessorCoreConfiguration.MktmeIntegrity;
  // TDX
  SecurityPolicy->EnableTdx                  = SetupData->SocketConfig.SocketProcessorCoreConfiguration.EnableTdx;
  SecurityPolicy->KeySplit                   = SetupData->SocketConfig.SocketProcessorCoreConfiguration.KeySplit;

  // SGX
  SecurityPolicy->EnableSgx                  = SetupData->SocketConfig.SocketProcessorCoreConfiguration.EnableSgx;
  SecurityPolicy->PrmrrSize                  = SetupData->SocketConfig.SocketProcessorCoreConfiguration.PrmrrSize;
  SecurityPolicy->SprspOrLaterPrmSize        = SetupData->SocketConfig.SocketProcessorCoreConfiguration.SprspOrLaterPrmSize;
  SecurityPolicy->SgxQoS                     = SetupData->SocketConfig.SocketProcessorCoreConfiguration.SgxQoS;
  SecurityPolicy->SgxAutoRegistrationAgent   = SetupData->SocketConfig.SocketProcessorCoreConfiguration.SgxAutoRegistrationAgent;
  SecurityPolicy->SgxPackageInfoInBandAccess = SetupData->SocketConfig.SocketProcessorCoreConfiguration.SgxPackageInfoInBandAccess;
  SecurityPolicy->EpochUpdate                = SetupData->SocketConfig.SocketProcessorCoreConfiguration.EpochUpdate;
  SecurityPolicy->SgxEpoch0                  = SetupData->SocketConfig.SocketProcessorCoreConfiguration.SgxEpoch0;
  SecurityPolicy->SgxEpoch1                  = SetupData->SocketConfig.SocketProcessorCoreConfiguration.SgxEpoch1;
  SecurityPolicy->SgxLeWr                    = SetupData->SocketConfig.SocketProcessorCoreConfiguration.SgxLeWr;
  SecurityPolicy->SgxLePubKeyHash0           = SetupData->SocketConfig.SocketProcessorCoreConfiguration.SgxLePubKeyHash0;
  SecurityPolicy->SgxLePubKeyHash1           = SetupData->SocketConfig.SocketProcessorCoreConfiguration.SgxLePubKeyHash1;
  SecurityPolicy->SgxLePubKeyHash2           = SetupData->SocketConfig.SocketProcessorCoreConfiguration.SgxLePubKeyHash2;
  SecurityPolicy->SgxLePubKeyHash3           = SetupData->SocketConfig.SocketProcessorCoreConfiguration.SgxLePubKeyHash3;
  SecurityPolicy->SgxDebugMode               = SetupData->SocketConfig.SocketProcessorCoreConfiguration.SgxDebugMode;
  SecurityPolicy->SgxFactoryReset            = SetupData->SocketConfig.SocketProcessorCoreConfiguration.SgxFactoryReset;


  if (FixedPcdGetBool (PcdBiosDfxKnobEnabled)) {
    //
    // MK-TME
    //
    SecurityPolicy->DfxEnableTmePOC            = SetupData->SocketConfig.SocketProcessorCoreConfiguration.DfxEnableTmePOC;
    SecurityPolicy->DfxTmeKeyRestore           = SetupData->SocketConfig.SocketProcessorCoreConfiguration.DfxTmeKeyRestore;
    SecurityPolicy->DfxTmeExclusionBase        = SetupData->SocketConfig.SocketProcessorCoreConfiguration.DfxTmeExclusionBase;
    SecurityPolicy->DfxTmeExclusionLength      = SetupData->SocketConfig.SocketProcessorCoreConfiguration.DfxTmeExclusionLength;
    //
    // SGX Dfx
    //
    SecurityPolicy->DfxSgxDebugPrint           = SetupData->SocketConfig.SocketProcessorCoreConfiguration.DfxSgxDebugPrint;
    SecurityPolicy->DfxMcheckMsr72             = SetupData->SocketConfig.SocketProcessorCoreConfiguration.DfxMcheckMsr72;
    SecurityPolicy->DfxMockSgxIsNotCapable     = SetupData->SocketConfig.SocketProcessorCoreConfiguration.DfxMockSgxIsNotCapable;
    SecurityPolicy->DfxSgxRegistrationServerSelect = SetupData->SocketConfig.SocketProcessorCoreConfiguration.DfxSgxRegistrationServerSelect;
    SecurityPolicy->DfxSgxAddPackageSupport    = SetupData->SocketConfig.SocketProcessorCoreConfiguration.DfxSgxAddPackageSupport;
    //
    // TDX Dfx
    //
    SecurityPolicy->DfxAllowTdxWithoutSgx      = SetupData->SocketConfig.SocketProcessorCoreConfiguration.DfxAllowTdxWithoutSgx;

  }
} // GetPlatformSetupValueforSecurity

/**
  Update PEI Security policy.

  @param[in, out] SetupData       Pointer to SETUP_DATA structure.

  @return Update status.

**/

EFI_STATUS
EFIAPI
UpdatePeiSecurityPolicy (
  // APTIOV_SERVER_OVERRIDE_RC_START
  IN OUT    INTEL_SETUP_DATA          *SetupData
  // APTIOV_SERVER_OVERRIDE_RC_END
  )
{
  GetPlatformSetupValueforSecurity (SetupData);

  return EFI_SUCCESS;

} // UpdatePeiSecurityPolicy
