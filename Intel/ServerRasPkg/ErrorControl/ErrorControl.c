/** @file
  SMM driver for Error Control support.

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

#include <Library/BaseLib.h>
#include <Library/PlatPolicyLib.h>
#include <Library/RasDebugLib.h>
// APTIOV_SERVER_OVERRIDE_RC_START : Need to change file name from ErrorControlLIb.h to ErrorControlLib.h
#include <Library/ErrorControlLIb.h>
// APTIOV_SERVER_OVERRIDE_RC_END : Need to change file name from ErrorControlLIb.h to ErrorControlLib.h

/**
  Entry point of Error Control driver.

  @param[in]  ImageHandle   A handle for the image that is initializing this driver
  @param[in]  SystemTable   A pointer to the EFI system table

  @retval EFI_SUCCESS               Driver initialized successfully.
  @retval EFI_LOAD_ERROR            Failed to Initialize or to Load the driver.
  @retval EFI_OUT_OF_RESOURCES      Could not allocate needed resources.
**/
EFI_STATUS
EFIAPI
ErrorControlEntry (
  IN EFI_HANDLE               ImageHandle,
  IN EFI_SYSTEM_TABLE         *SystemTable
  )
{
  RAS_POLICY                  *RasPolicy;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "Error Control Entry Point\n"));

  RasPolicy = GetRasPolicy ();
  ASSERT (RasPolicy != NULL);
  if(RasPolicy != NULL) {
    PatrolErrorDowngradeSetting (RasPolicy->ErrorControlPolicy.PatrolErrorDowngradeEn);
    Ce2LmLoggingSetting (RasPolicy->ErrorControlPolicy.Ce2LmLoggingEn);
    KtiFirstCeLatchSetting (RasPolicy->ErrorControlPolicy.KtiFirstCeLatchEn);
    LmceDowngradeSetting (RasPolicy->ErrorControlPolicy.LmceDowngradeEn);
    LlcEwbErrorControlSetting (RasPolicy->ErrorControlPolicy.LlcEwbErrorControl);
  }
  ClearFnvViral ();

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "Error Control Exit\n"));
  return EFI_SUCCESS;
}
