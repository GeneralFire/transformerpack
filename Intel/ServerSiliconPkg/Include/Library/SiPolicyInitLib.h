/** @file
  Public API header file for the SiPolicyInitLib library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 Intel Corporation. <BR>

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

#ifndef __SI_POLICY_INIT_LIB_H__
#define __SI_POLICY_INIT_LIB_H__

/**
  Create silicon policy defaults.

  Allocate and populate the silicon policy with Intel defaults.

  @param[out] Policy            A pointer to the allocated policy structure.

  @retval EFI_SUCCESS           The default policy was successfully created.
  @retval EFI_INVALID_PARAMETER The return pointer parameter was null on entry.
  @retval EFI_OUT_OF_RESOURCES  Memory could not be allocated for the policy.
--*/
EFI_STATUS
EFIAPI
CreateSiPolicyDefaults (
  OUT    VOID **Policy
  );

/**
  Install the silicon policy PPI.

  Install the silicon policy structure as a PPI. The field values should be finalized by
  this point.

  @param[in]  Policy            A pointer to the policy structure to install as a PPI.

  @retval EFI_SUCCESS           The policy PPI was successfully installed.
  @retval EFI_INVALID_PARAMETER The parameter was null.
  @retval EFI_OUT_OF_RESOURCES  The PPI descriptor could not be allocated for installing.
  @retval !EFI_SUCCESS          The policy PPI was not installed. See InstallPpi() errors.
--*/
EFI_STATUS
EFIAPI
InstallSiPolicyPpi (
  IN     VOID *Policy
  );

#endif // __SI_POLICY_INIT_LIB_H__
