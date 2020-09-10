/**@file
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
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/FusaSafPolicyLib.h>
#include <Library/SecurityPolicyDefinitions.h>


/**

  Set FuSa (SAF) policy info into system.

  @param   FusaSafPolicy        Pointer to FuSa (SAF) policy.
  @param   DataSize             The size of FuSa (SAF) policy.

  @retval  EFI_SUCCESS          Set FuSa (SAF) policy success.
  @retval  EFI_OUT_OF_RESOURCES Out of resource.
  @retval  EFI_NOT_FOUND        Not found FuSa (SAF) HOB.

**/
EFI_STATUS
EFIAPI
SetFusaSafPolicy (
  IN FUSA_SAF_POLICY *FusaSafPolicy,
  IN UINTN           DataSize
  )
{
  VOID       *FusaSafPolicyGuidHob = NULL;
  VOID       *GuidData             = NULL;
  EFI_STATUS Status                = EFI_SUCCESS;
  UINTN GuidDataSize;

  FusaSafPolicyGuidHob = GetFirstGuidHob (&gFusaSafDataHobGuid);
  if (FusaSafPolicyGuidHob != NULL) {
    GuidData = GET_GUID_HOB_DATA (FusaSafPolicyGuidHob);
    GuidDataSize = GET_GUID_HOB_DATA_SIZE (FusaSafPolicyGuidHob);
    if (GuidDataSize >= DataSize) {
      CopyMem (GuidData, FusaSafPolicy, DataSize);
    } else {
      Status = EFI_OUT_OF_RESOURCES;
    }
  } else {
    Status = EFI_NOT_FOUND;
  }

  return Status;
}

/**

  Set FuSa (SAF) policy info into system. Does initialization
  of FuSa (SAF) Policy Structure.

  @retval  NOT NULL             Return the pointer to allocated FuSa (SAF) policy.
  @retval  NULL                 Cannot allocate FuSa (SAF) HOB and return NULL.

**/
VOID *
EFIAPI
InitializeDefaultFusaSafConfigPolicy (
  VOID
  )
{
  FUSA_SAF_POLICY FusaSafPolicy;

  //
  // Init Security feature related setup options to default safe values that ensure minimum boot.
  // A platform hook will be called later to override these values.
  //
  ZeroMem ((VOID*) &FusaSafPolicy, sizeof (FusaSafPolicy));
  //
  // default values of policy
  //
  FusaSafPolicy.EnableSaf  = SECURITY_POLICY_DISABLE;
  FusaSafPolicy.SafSupport = SECURITY_POLICY_DISABLE;
  // zeros or null
  FusaSafPolicy.SafImgLoadAddr = 0x0;

  //
  // end:default values of policy
  //
  return (VOID *)(BuildGuidDataHob (&gFusaSafDataHobGuid, &FusaSafPolicy, sizeof (FusaSafPolicy)));
}

/**

  Get FuSa (SAF) policy info from system.

  @retval  FUSA_SAF_POLICY      Return pointer to FuSa (SAF) policy data.
  @retval  NULL                 Cannot return data from FuSa (SAF) policy data.

**/
FUSA_SAF_POLICY *
EFIAPI
GetFusaSafPolicy (
  VOID
  )
{
  VOID       *GuidHob  = NULL;
  VOID       *GuidData = NULL;

  GuidHob = GetFirstGuidHob (&gFusaSafDataHobGuid);
  if (GuidHob == NULL) {
    GuidData = InitializeDefaultFusaSafConfigPolicy ();
    if (GuidData == NULL) {
      return NULL;
    }
  } else {
    GuidData = GET_GUID_HOB_DATA (GuidHob);
  }

  return (FUSA_SAF_POLICY *)(GuidData);
}