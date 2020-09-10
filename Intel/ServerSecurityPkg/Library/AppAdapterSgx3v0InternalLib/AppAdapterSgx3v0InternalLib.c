/** @file

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

#include "AppAdapterSgx3v0InternalLib.h"

// Might need a static global to speed things up
FRU_CPU_FEATURE_SGX_3V0_PPI *mFruCpuFeatureSgx3v0 = NULL;

EFI_STATUS
EFIAPI
_RetrievePrmrr (
  IN APP_SGX_3V0_PPI                   *This,
  IN CONST EFI_PEI_SERVICES            **PeiServices
  )
{
  This->PrmrrData.PrmrrBase[0] = 0;
  This->PrmrrData.PrmrrMask    = 0;
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
_ProgramPrmrr (
  IN APP_SGX_3V0_PPI        *This,
  IN CONST EFI_PEI_SERVICES **PeiServices
  )
{
  EFI_STATUS Status = EFI_UNSUPPORTED;
  DEBUG ((EFI_D_INFO, "[APP_ADAPTER] %a BEGIN\n", __FUNCTION__));
  //
  // Locate Ppi
  //
  if (mFruCpuFeatureSgx3v0 == NULL) {
    Status = (*PeiServices)->LocatePpi (
      PeiServices,
      &gSecurityFruCpuFeatureSgx3v0PpiGuid,
      0,
      NULL,
      &mFruCpuFeatureSgx3v0
      );
  }
  if (EFI_ERROR(Status)) {
    goto Return_AppAdapterSgx3v0InternalLib_ProgramPrmrr;
  }
  // Initialize internal structures
  CopyMem (&mFruCpuFeatureSgx3v0->PrmrrData, &This->PrmrrData, sizeof(mFruCpuFeatureSgx3v0->PrmrrData));
  Status = mFruCpuFeatureSgx3v0->ProgramPrmrr (mFruCpuFeatureSgx3v0, PeiServices);

Return_AppAdapterSgx3v0InternalLib_ProgramPrmrr:
  DEBUG ((EFI_D_INFO, "[APP_ADAPTER] %a END Status: %r\n", __FUNCTION__, Status));
  return Status;
}

EFI_STATUS
EFIAPI
_LockPrmrr (
  IN APP_SGX_3V0_PPI *This,
  IN CONST EFI_PEI_SERVICES            **PeiServices
  )
{
  return EFI_UNSUPPORTED;
}


