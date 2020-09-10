/** @file
  Header file for PpmPolicyUpdateCommonLib Library.

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

#ifndef __PPM_POLICY_UPDATE_COMMON_LIB_H__
#define __PPM_POLICY_UPDATE_COMMON_LIB_H__

#include <PpmPolicyPeiDxeCommon.h>

UINT8 UpdatePkgCStatePlatform (
  UINT8                   SetupPkgCValue
  );

/**
  This function provides common code for both PEI and DXE code to
  update PPM policy from setup

  @param[out] PpmPolicy           The policy to be updated from setup

  @retval EFI_SUCCESS             The PPI is installed and initialized.
  @retval EFI ERRORS              The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
UpdatePpmPolicyFromSetup (
  OUT PPM_POLICY_CONFIGURATION *PpmPolicy
);

#endif
