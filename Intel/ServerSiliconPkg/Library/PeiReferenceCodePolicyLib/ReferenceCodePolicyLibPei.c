/** @file
  Implementation of ReferenceCodePolicyLib

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 Intel Corporation. <BR>

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
#include <PiPei.h>
#include <Library/DebugLib.h>
#include <ReferenceCodePolicy.h>
#include <Library/ReferenceCodePolicyLib.h>
#include <Library/HobLib.h>

REFERENCE_CODE_POLICY *
GetReferenceCodePolicy (
  VOID
  )
{
  VOID *GuidHob;

  GuidHob = GetFirstGuidHob (&gReferenceCodePolicyHobGuid);
  if (GuidHob != NULL) {
    return (REFERENCE_CODE_POLICY *)GET_GUID_HOB_DATA(GuidHob);
  }
  return NULL;
}
/**
  This API gets NumaEn.

  @param  None

  @retval UINT8  The data for NumaEn returned.
**/
UINT8
EFIAPI
GetNumaEn (
  VOID
)
{
  REFERENCE_CODE_POLICY *ReferenceCodePolicy;
  ReferenceCodePolicy = GetReferenceCodePolicy();

  if (ReferenceCodePolicy == NULL) {
    DEBUG ((DEBUG_ERROR, "Unale to read NumaEn, GetReferenceCodePolicy Returned NULL pointer\n"));
    ASSERT (ReferenceCodePolicy != NULL);
    return 0;
  }

  return ReferenceCodePolicy->NumaEn;
}

/**
  This API sets the value for NumaEn.

  @param[in] UINT8  Value to set for NumaEn.

  @retval None
**/
VOID
EFIAPI
SetNumaEn (
  IN UINT8  NumaEn
)
{
  REFERENCE_CODE_POLICY *ReferenceCodePolicy;
  ReferenceCodePolicy = GetReferenceCodePolicy();

  if (ReferenceCodePolicy == NULL) {
    DEBUG ((DEBUG_ERROR, "Unale to set NumaEn=%d, GetReferenceCodePolicy Returned NULL pointer\n", NumaEn));
    ASSERT (ReferenceCodePolicy != NULL);
    return;
  }

  ReferenceCodePolicy->NumaEn = NumaEn;
  return;
}

/**
  This API gets UmaBasedClustering.

  @param  None

  @retval UINT8  The data for UmaBasedClustering returned.
**/
UINT8
EFIAPI
GetUmaBasedClustering (
  VOID
)
{
  REFERENCE_CODE_POLICY *ReferenceCodePolicy;
  ReferenceCodePolicy = GetReferenceCodePolicy();

  if (ReferenceCodePolicy == NULL) {
    DEBUG ((DEBUG_ERROR, "Unale to read UmaBasedClustering, GetReferenceCodePolicy Returned NULL pointer\n"));
    ASSERT (ReferenceCodePolicy != NULL);
    return 0;
  }

  return ReferenceCodePolicy->UmaBasedClustering;
}

/**
  This API sets the value for UmaBasedClustering.

  @param[in] UINT8  Value to set for UmaBasedClustering.

  @retval None
**/
VOID
EFIAPI
SetUmaBasedClustering (
  IN UINT8  UmaBasedClustering
)
{
  REFERENCE_CODE_POLICY *ReferenceCodePolicy;
  ReferenceCodePolicy = GetReferenceCodePolicy();

  if (ReferenceCodePolicy == NULL) {
    DEBUG ((DEBUG_ERROR, "Unale to set UmaBasedClustering=%d, GetReferenceCodePolicy Returned NULL pointer\n", UmaBasedClustering));
    ASSERT (ReferenceCodePolicy != NULL);
    return;
  }

  ReferenceCodePolicy->UmaBasedClustering = UmaBasedClustering;
}
