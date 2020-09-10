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
#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <ReferenceCodePolicy.h>
#include <Library/ReferenceCodePolicyLib.h>
#include <Protocol/ReferenceCodePolicyProtocol.h>

REFERENCE_CODE_POLICY  *mReferenceCodePolicy;
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
  return mReferenceCodePolicy->NumaEn;
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
  mReferenceCodePolicy->NumaEn = NumaEn;
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
  return mReferenceCodePolicy->UmaBasedClustering;
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
  mReferenceCodePolicy->UmaBasedClustering = UmaBasedClustering;
}

/**
  The constructor function initialize Reference Code Policy.

  Standard EFI driver point.

  @param ImageHandle         -  A handle for the image that is initializing this driver.
  @param SystemTable         -  A pointer to the EFI system table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
DxeReferenceCodePolicyLibConstructor (
  IN EFI_HANDLE              ImageHandle,
  IN EFI_SYSTEM_TABLE        *SystemTable
  )
{
  EFI_STATUS                       Status;
  REFERENCE_CODE_POLICY_PROTOCOL   *DxeReferenceCodePolicyProtocol;

  Status = gBS->LocateProtocol (
                    &gDxeReferenceCodePolicyProtocol,
                    NULL,
                    &DxeReferenceCodePolicyProtocol
                    );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  mReferenceCodePolicy =  (REFERENCE_CODE_POLICY*)DxeReferenceCodePolicyProtocol;

  return EFI_SUCCESS;
}

