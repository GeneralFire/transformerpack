/** @file
  Public API header file for the MemoryPolicyLib library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

#ifndef _MEMORY_POLICY_LIB_H_
#define _MEMORY_POLICY_LIB_H_

#include <Ppi/MemoryPolicyPpi.h>
#include <Library/MemoryServicesLib.h>

/**
  Helper function to enable or disable memory flows in the policy.

  @param[in]  Flow          The flow to enable or disable.
  @param[in]  Enable        Enable or disabled.
  @param[out] PolicyPpi     A pointer to the Policy to fill in.

  @retval None

**/

VOID
EFIAPI
SetPolicyMemFlow (
  IN MEMFLOWS Flow,
  IN BOOLEAN Enable,
  OUT SYS_SETUP *Policy
  );

/**
  Set the memory policy default values.

  Set the default values for the MRC. UpdatePeiMemoryPolicy will update
  these default values via Setup variables. Default values come from MemDefaults.h

  @param[out]  PolicyPpi          A pointer to PPI to fill in.

  @retval EFI_SUCCESS             Success
  @retval EFI_INVALID_PARAMETER   Failure

**/

EFI_STATUS
EFIAPI
SetMemoryPolicyDefaults (
  OUT MEMORY_POLICY_PPI *PolicyPpi
  );

/**
  Install the memory policy.

  Allocates memory for the policy, installs the policy, and
  sets the static pointer table pointer to the policy.

  @param[in]  PolicyPpi          A pointer to PPI to fill in.

  @retval EFI_SUCCESS             Success
  @retval EFI_INVALID_PARAMETER   Failure

**/

EFI_STATUS
EFIAPI
InstallMemoryPolicyPpi (
  IN  MEMORY_POLICY_PPI     *PolicyPpi
  );

#endif // _MEMORY_POLICY_LIB_H_
