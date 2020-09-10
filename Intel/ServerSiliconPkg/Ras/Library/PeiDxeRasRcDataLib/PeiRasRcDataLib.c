/** @file
  DXE RAS RC data library class implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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
**/

#include <PiPei.h>
#include <Library/RasRcDataLib.h> // *this* library class
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/RasRcPolicy.h>
#include "PeiDxeRasRcDataLibInternal.h"

EFI_STATUS
CopyPolicyPpiToHob (
  OUT RAS_RC_POLICY *Policy
  );

/**
  The library constructor function.

  @param[in]  FileHandle    Handle of the file being invoked.
  @param[in]  PeiServices   A pointer to the PEI Services table.

  @retval EFI_SUCCESS           The constructor was executed successfully.
  @retval EFI_OUT_OF_RESOURCES  A required HOB could not be allocated.
**/
EFI_STATUS
EFIAPI
PeiRasRcDataLibConstructor (
  IN EFI_PEI_FILE_HANDLE    FileHandle,
  IN CONST EFI_PEI_SERVICES **PeiServices
  )
{
  EFI_STATUS    Status;
  RAS_RC_POLICY *Policy;
  RAS_RC_CONFIG *Config;

  //
  // Initialize the input policy HOB if it does not already exist.
  //
  Policy = (RAS_RC_POLICY *) GetRasRcHobInternal (&gRasRcPolicyHobGuid);
  if (Policy == NULL) {
    DEBUG ((DEBUG_INFO, "Creating RAS RC policy HOB"));
    Policy = (RAS_RC_POLICY *) BuildGuidHob (
      &gRasRcPolicyHobGuid,
      sizeof (RAS_RC_POLICY)
      );
    ASSERT (Policy != NULL);
    if (Policy == NULL) {
      DEBUG ((DEBUG_ERROR, "ERROR: Could not allocate RAS RC policy HOB!"));
      Status = EFI_OUT_OF_RESOURCES;
    } else {
      //
      // First time data initialization.
      //
      Status = CopyPolicyPpiToHob (Policy);
      if (EFI_ERROR (Status)) {
        return Status;
      }
    }
  }

  //
  // Initialize the output config HOB if it does not already exist.
  //
  Config = (RAS_RC_CONFIG *) GetRasRcHobInternal (&gRasRcConfigHobGuid);
  if (Config == NULL) {
    DEBUG ((DEBUG_INFO, "Creating RAS RC config HOB"));
    Config = (RAS_RC_CONFIG *) BuildGuidHob (
      &gRasRcConfigHobGuid,
      sizeof (RAS_RC_CONFIG)
      );
    ASSERT (Config != NULL);
    if (Config == NULL) {
      DEBUG ((DEBUG_ERROR, "ERROR: Could not allocate RAS RC config HOB!"));
      return EFI_OUT_OF_RESOURCES;
    } else {
      //
      // First time data initialization.
      //
      ZeroMem ((VOID *) Config, sizeof (RAS_RC_CONFIG));
    }
  }

  return EFI_SUCCESS;
} // PeiRasRcDataLibConstructor

/**
  Copy policy PPI to HOB.

  PEI-only helper function to copy the policy PPI to a HOB for use in the
  DXE phase. For convenience, the HOB is used in the PEI phase as well.

  @param[out] Policy        A pointer to the RAS RC policy data destination.

  @retval EFI_SUCCESS       The data was successfully copied.
  @retval EFI_NOT_FOUND     The PPI was not found.
**/
EFI_STATUS
CopyPolicyPpiToHob (
  OUT RAS_RC_POLICY *Policy
  )
{
  EFI_STATUS        Status;
  RAS_RC_POLICY_PPI *Ppi;

  Status = PeiServicesLocatePpi (
    &gRasRcPolicyPpiGuid,
    0,
    NULL,
    (VOID **) &Ppi
    );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "ERROR: Could not locate RAS RC policy PPI: %r!",
      Status
      ));
    return Status;
  }

  ASSERT (sizeof (RAS_RC_POLICY_PPI) == sizeof (RAS_RC_POLICY));
  CopyMem (Policy, Ppi, sizeof (RAS_RC_POLICY));

  return EFI_SUCCESS;
} // CopyPolicyPpiToHob
