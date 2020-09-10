/** @file
  Public API header file for the PeiUpiPolicyLib library class.

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

#ifndef _PEI_UPI_POLICY_LIB_H_
#define _PEI_UPI_POLICY_LIB_H_

#include <Ppi/UpiPolicyPpi.h>

/**
  UpiCreatePolicyDefaults generates the default Upi Pei Policy.

  @param[in, out] PchPolicyPpi          The pointer to UPI Policy PPI instance

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
UpiCreatePolicyDefaults (
  IN OUT  UPI_POLICY_PPI     *UpiPolicyPpi
  );

/**
  UpiInstallPolicyPpi installs UpiPolicyPpi.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] UpiPolicyPpi               The pointer to UPI Policy PPI instance

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
UpiInstallPolicyPpi (
  IN  UPI_POLICY_PPI     *UpiPolicyPpi
  );
#endif // _PEI_UPI_POLICY_LIB_H_
