/** @file

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

#include <Library/MemoryCoreLib.h>

/**
  HBM pre-training entry

  This function is the HBM pre-training entry where the HBM pre-training
  steps are executed sequentially.

  @param[in] Host      Pointer to sysHost.

  @retval SUCCESS      HBM pre-training steps completed successfully.
**/
UINT32
EFIAPI
HbmPreTraining (
  IN PSYSHOST  Host
  )
{
  return SUCCESS;
}

/**
  HBM training entry

  This function is the HBM training entry where the HBM training steps
  are executed sequentially.

  @param[in] Host      Pointer to sysHost.

  @retval SUCCESS      HBM training steps completed successfully.
**/
UINT32
EFIAPI
HbmTraining (
  IN PSYSHOST  Host
  )
{
  return SUCCESS;
}

/**
  HBM post-training entry

  This function is the HBM post-training entry where the HBM post-training
  steps are executed sequentially.

  @param[in] Host      Pointer to sysHost.

  @retval SUCCESS      HBM post-training steps completed successfully.
**/
UINT32
EFIAPI
HbmPostTraining (
  IN PSYSHOST  Host
  )
{
  return SUCCESS;
}

/**
  HBM switch to normal mode entry

  This function is the HBM switch to normal mode entry where the HBM CSRs
  required by switching HBM MC to normal mode are programmed sequentially.

  @param[in] Host      Pointer to sysHost.

  @retval SUCCESS      HBM SwitchToNormalMode steps completed successfully.
**/
UINT32
EFIAPI
HbmSwitchToNormalMode (
  IN PSYSHOST  Host
  )
{
  return SUCCESS;
}
