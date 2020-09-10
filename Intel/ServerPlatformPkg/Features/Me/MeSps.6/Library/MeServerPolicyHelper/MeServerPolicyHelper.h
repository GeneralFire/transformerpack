/** @file
  Implements SPS Config Blocks initialization Helper for common data

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

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

#ifndef _ME_SERVER_POLICY_HELPER_H_
#define _ME_SERVER_POLICY_HELPER_H_

/**
  Function copies setup data in to SPS_PEI_CONFIG object.

  @param[in]  MeSetup           Input pointer to ME_RC_CONFIGURATION
  @param[out] SpsCommonConfig   Pointer to SPS_COMMON_CONFIG

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_INVALID_PARAMETER Wrong pointer passed to the function
**/
EFI_STATUS
CopyDataOfSpsCommonConfig (
  IN     ME_RC_CONFIGURATION  *MeSetup,
     OUT SPS_COMMON_CONFIG    *SpsCommonConfig
  );

#endif // _ME_SERVER_POLICY_HELPER_H_
