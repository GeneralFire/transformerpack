/** @file
  RAS RC policy PPI library class header file.

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

#ifndef _RAS_RC_POLICY_PPI_LIB_H_
#define _RAS_RC_POLICY_PPI_LIB_H_

#include <Ppi/RasRcPolicy.h>

/**
  Set RAS RC policy PPI defaults.

  Set all fields in the RAS RC policy PPI to safe defaults. Since all fields
  are set by this function, the PPI does not need to be zeroed before calling.

  @param[out] RasRcPolicyPpi    A pointer to the RAS RC policy PPI to set.

  @retval EFI_SUCCESS             The policy defaults are set.
  @retval EFI_INVALID_PARAMETER   The PPI pointer parameter is null.
**/
EFI_STATUS
EFIAPI
SetRasRcPolicyPpiDefaults (
  OUT       RAS_RC_POLICY_PPI   *RasRcPolicyPpi
  );

/**
  Install the RAS RC policy PPI.

  Once the PPI is installed, RC assumes its values are final. Any platform
  customizations must be done before calling this function.

  @param[in]  RasRcPolicyPpi    A pointer to the RAS RC policy PPI to install.

  @retval EFI_SUCCESS             The policy is installed.
  @retval EFI_INVALID_PARAMETER   The PPI pointer parameter is null.
  @retval EFI_OUT_OF_RESOURCES    Unable to allocate a PPI descriptor.
  @retval EFI_OUT_OF_RESOURCES    There is no more space in the PPI database.
**/
EFI_STATUS
EFIAPI
InstallRasRcPolicyPpi (
  IN        RAS_RC_POLICY_PPI   *RasRcPolicyPpi
  );

#endif // _RAS_RC_POLICY_PPI_LIB_H_
