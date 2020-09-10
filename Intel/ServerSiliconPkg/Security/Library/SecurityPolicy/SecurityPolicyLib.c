/** @file
  Interface for Security Policy library. These functions are used to
  Initialize, store and retrive the security policy data. It has three
  functions namely CreateDefaultSecurityPolicy, SetSecurityPolicy and
  GetSecurityPolicy, they are used to create, Set and Get the Security
  policies repectively.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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
#include <Library/HobLib.h>
#include <SysHostChipCommon.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SecurityPolicyLib.h>
#include <Library/SimulationServicesLib.h>

/**
  Set Security policy info into system. Does initialization
  of Security Policy Structure.

  @retval  EFI_SUCCESS          Set Security policy success.
  @retval  EFI_OUT_OF_RESOURCES Out of resource;
**/
EFI_STATUS
EFIAPI
InitializeDefaultSecurityConfigPolicy (
  VOID
  )
{
  SECURITY_POLICY SecurityPolicy;

  //
  // Init Security feature related setup options to default safe values that ensure minimum boot.
  // A platform hook will be called later to override these values.
  //
  ZeroMem ((VOID*) &SecurityPolicy, sizeof (SecurityPolicy));

  //
  // TME
  //

  SecurityPolicy.EnableTme             = SECURITY_POLICY_DISABLE; // for those values we can use defines
  SecurityPolicy.TmeCapability         = SECURITY_POLICY_UNSUPPORTED;
  SecurityPolicy.EnableTmeCR           = SECURITY_POLICY_DISABLE;
  SecurityPolicy.EnableMktme           = SECURITY_POLICY_DISABLE;
  SecurityPolicy.MktmeIntegrity        = SECURITY_POLICY_ENABLE;
  SecurityPolicy.KeySplit              = 3;
  // DFX
  SecurityPolicy.DfxTmeKeyRestore      = SECURITY_POLICY_DFX_DISABLE;
  SecurityPolicy.DfxTmeExclusionBase   = 0;
  SecurityPolicy.DfxTmeExclusionLength = 0;

  //
  // TDX
  //

  SecurityPolicy.EnableTdx             = SECURITY_POLICY_DISABLE;
  SecurityPolicy.TdxCapability         = SECURITY_POLICY_UNSUPPORTED;
  // DFX
  SecurityPolicy.DfxAllowTdxWithoutSgx = SECURITY_POLICY_DFX_AUTO;

  //
  // SGX
  //

  // Outputs: Helper variables
  SecurityPolicy.CrDimmsPresent             = 0;
  SecurityPolicy.IsSgxCapable               = SECURITY_POLICY_UNSUPPORTED; // Prevent from showing SGX settings in BIOS menu when CPUs  don't support SGX.
  SecurityPolicy.IsHwCongifSupportedBySgx   = SECURITY_POLICY_SUPPORTED;
  SecurityPolicy.SkipSignalPpmDone    = 0;

  SecurityPolicy.ValidPrmrrBitMap           = SECURITY_POLICY_PRMRR_SIZE_2GB; // 2GB
  SecurityPolicy.SprspOrLaterPrmSizeBitmap  = SECURITY_POLICY_PRMRR_SIZE_1GB; // 1GB (default for SPR+)

  SecurityPolicy.SprspOrLaterAreHardwarePreconditionsMet = SECURITY_POLICY_UNSUPPORTED;
  SecurityPolicy.SprspOrLaterAreMemoryPreconditionsMet   = SECURITY_POLICY_UNSUPPORTED;
  SecurityPolicy.SprspOrLaterAreSetupPreconditionsMet    = SECURITY_POLICY_UNSUPPORTED;

  // Inputs: Regular
  SecurityPolicy.EnableSgx                  = SECURITY_POLICY_DISABLE;               // Disable
  SecurityPolicy.SgxFactoryReset            = SECURITY_POLICY_DISABLE;
  SecurityPolicy.PrmrrSize                  = SECURITY_POLICY_PRMRR_SIZE_2GB; // 2GB
  SecurityPolicy.SprspOrLaterPrmSize        = SECURITY_POLICY_PRMRR_SIZE_1GB; // 1GB (default for SPR+)
  SecurityPolicy.SgxPackageInfoInBandAccess = SECURITY_POLICY_DISABLE;               // Disable
  SecurityPolicy.EpochUpdate                = SECURITY_POLICY_SGX_USER_MANUAL_EPOCH; // Manual
  SecurityPolicy.SgxEpoch0                  = 0;
  SecurityPolicy.SgxEpoch1                  = 0;
  SecurityPolicy.SgxQoS                     = SECURITY_POLICY_ENABLE;  // Enable
  SecurityPolicy.SgxLePubKeyHash0           = 0;
  SecurityPolicy.SgxLePubKeyHash1           = 0;
  SecurityPolicy.SgxLePubKeyHash2           = 0;
  SecurityPolicy.SgxLePubKeyHash3           = 0;
  SecurityPolicy.SgxLeWr                    = SECURITY_POLICY_ENABLE;  // 1 = SGXLEPUBKEYHASHx Write Enable
  SecurityPolicy.SgxDebugMode               = SECURITY_POLICY_DISABLE;

  // DFX
  SecurityPolicy.DfxSgxDebugPrint           = SECURITY_POLICY_DFX_AUTO;    // Auto
  SecurityPolicy.DfxMcheckMsr72             = SECURITY_POLICY_DFX_AUTO;    // Auto
  SecurityPolicy.DfxMockSgxIsNotCapable     = SECURITY_POLICY_DFX_AUTO;    // Auto
  SecurityPolicy.DfxSgxRegistrationServerSelect = SECURITY_POLICY_DFX_PRX_SERVER;

  BuildGuidDataHob (&gSecurityPolicyDataGuid, &SecurityPolicy, sizeof (SecurityPolicy));

  return SetSecurityPolicy (&SecurityPolicy, sizeof (SecurityPolicy));
}

/**
  Set Security policy info into system.

  @param   SecurityPolicy       Pointer to Security Policy Info.
  @param   DataSize             The size of Security Policy.

  @retval  EFI_SUCCESS          Set Security policy success.
  @retval  EFI_OUT_OF_RESOURCES Out of resource;
**/
EFI_STATUS
EFIAPI
SetSecurityPolicy (
  IN SECURITY_POLICY *SecurityPolicy,
  IN UINTN DataSize
  )
{
  VOID *SecurityPolicyGuidHob;
  VOID *GuidData;
  UINTN GuidDataSize;

  SecurityPolicyGuidHob = GetFirstGuidHob (&gSecurityPolicyDataGuid);
  if (SecurityPolicyGuidHob != NULL) {
    GuidData = GET_GUID_HOB_DATA (SecurityPolicyGuidHob);
    GuidDataSize = GET_GUID_HOB_DATA_SIZE (SecurityPolicyGuidHob);
    if (GuidDataSize >= DataSize) {
      CopyMem (GuidData, SecurityPolicy, DataSize);
    } else {
      return EFI_OUT_OF_RESOURCES;
    }
  }
  return EFI_SUCCESS;
}

/**
  Get Security policy info into system.

  @retval  EFI_SUCCESS          Get Security policy success.
  @retval  EFI_NOT_FOUND        Can't find the Security Policy.
  @retval  EFI_BUFFER_TOO_SMALL The Buffer's size is too small.
**/
SECURITY_POLICY *
EFIAPI
GetSecurityPolicy (
  VOID
  )
{
  VOID *GuidHob = NULL;
  VOID *GuidData = NULL;
  EFI_STATUS Status;

  GuidHob = GetFirstGuidHob (&gSecurityPolicyDataGuid);
  while (GuidHob == NULL) {
    Status = InitializeDefaultSecurityConfigPolicy ();
    if (!EFI_ERROR (Status)) {
      GuidHob = GetFirstGuidHob (&gSecurityPolicyDataGuid);
    }
  }
  GuidData = GET_GUID_HOB_DATA (GuidHob);

  return GuidData;
}

/**
  Get Security Policy directly from HOB

  @retval  SECURITY_POLICY *  Get Security policy pointer, NULL on failure.
**/
SECURITY_POLICY *
EFIAPI
GetSecurityPolicyStalagmite (
  VOID
  )
{
  VOID *GuidHob  = NULL;
  VOID *GuidData = NULL;

  GuidHob = GetFirstGuidHob (&gSecurityPolicyDataGuid);
  if (GuidHob != NULL) {
    GuidData = GET_GUID_HOB_DATA (GuidHob);
  }

  return GuidData;
}
