/** @file
  This file is Si PeiPpmPolicyLib library creates default settings of PPM RC
  Policy and installs PPM RC Policy PPI.

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


#include <Ppi/PpmPolicyPpi.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>


/**
  PpmCreatePolicyDefaults generates the default PPM Pei Policy.

  @param[in, out] PpmPolicyPpi          The pointer to PPM Policy PPI instance

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
PpmCreatePolicyDefaults (
  IN OUT PPM_POLICY_PPI   *PpmPolicyPpi
  )
{

  //
  // Policy not listed here are set to 0/FALSE as default.
  //

  PpmPolicyPpi->Revision = PPM_POLICY_PPI_REVISION;


  return EFI_SUCCESS;
}

/**
  PpmInstallPolicyPpi installs PpmPolicyPpi.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] PpmPolicyPpi               The pointer to PPM Policy PPI instance

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
PpmInstallPolicyPpi (
  IN  PPM_POLICY_PPI    *PpmPolicyPpi
  )
{
  EFI_STATUS              Status;
  EFI_PEI_PPI_DESCRIPTOR  *PpiDesc = NULL;

  Status = EFI_SUCCESS;

  PpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (PpiDesc == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  PpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  PpiDesc->Guid  = &gPpmPolicyPpiGuid;
  PpiDesc->Ppi   = PpmPolicyPpi;

  DEBUG ((DEBUG_INFO, "Installing PpmPolicyPpi\n"));
  Status = PeiServicesInstallPpi (PpiDesc);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Installing PpmPolicyPpi Error = %r\n", Status));
    ASSERT(FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  return Status;
}
