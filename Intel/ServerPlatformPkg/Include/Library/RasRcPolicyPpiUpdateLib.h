/** @file
  RAS RC policy PPI update library class header file.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#ifndef _RAS_RC_POLICY_PPI_UPDATE_LIB_H_
#define _RAS_RC_POLICY_PPI_UPDATE_LIB_H_

#include <Ppi/RasRcPolicy.h>
#include <Library/SetupLib.h>

/**
  Update the RAS RC policy PPI.

  Modify the RAS RC policy PPI to set platform related and customized settings.
  The silicon's safe defaults are expected to have been set prior to calling
  this function. This function must be called before installing the PPI.

  @param[in, out] RasRcPolicyPpi  A pointer to the RAS RC policy PPI to set.
  @param[in, out] SetupData       Pointer to SETUP_DATA structure.

  @retval EFI_SUCCESS             The policy has been updated as necessary.
  @retval EFI_INVALID_PARAMETER   The PPI pointer parameter is null.
**/
EFI_STATUS
EFIAPI
UpdateRasRcPolicyPpi (
  IN OUT    RAS_RC_POLICY_PPI   *RasRcPolicyPpi,
  // APTIOV_SERVER_OVERRIDE_RC_START
  IN OUT    INTEL_SETUP_DATA          *SetupData
  // APTIOV_SERVER_OVERRIDE_RC_END
  );

#endif // _RAS_RC_POLICY_PPI_UPDATE_LIB_H_
