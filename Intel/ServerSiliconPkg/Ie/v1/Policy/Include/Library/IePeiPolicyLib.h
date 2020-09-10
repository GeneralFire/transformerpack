/** @file
  IePeiPolicyLib.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#ifndef _PEI_IE_POLICY_PEI_LIB_H_
#define _PEI_IE_POLICY_PEI_LIB_H_

#include <PiPei.h>
#include <Ppi/IePlatformPolicyPei.h>

/**
  Install IE PEI Policy

  @param[in] IePolicyPpi          IE Policy PPI pointer

  @retval EFI_SUCCESS             The policy is installed.
  @retval EFI_OUT_OF_RESOURCES    Unable to allocate necessary data structures.
**/
EFI_STATUS
IeInstallPolicyPpi (
  IN PEI_IE_PLATFORM_POLICY_PPI  *IePolicyPpi
  );


/**
  Create the default setting of IE Policy. The function allocates and zero out buffer,
  and fills in the Intel default settings.

  @param[out] IePolicyPpi               The pointer to get IE Policy PPI instance

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
IeCreatePolicyDefaults (
  OUT PEI_IE_PLATFORM_POLICY_PPI  **IePolicyPpi
  );

#endif // _PEI_ME_SPS_POLICY_PEI_LIB_H_
