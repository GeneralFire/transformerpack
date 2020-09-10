/** @file
  This is a library for ME ConfigBlock functionality.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include <Base.h>
#include <Uefi.h>
#include <PiDxe.h>
#include <ConfigBlock/SpsGetDxeConfigBlock.h>
#include <Protocol/SpsPolicyProtocol.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/ConfigBlockLib.h>

/**
  @brief
  Get SPS Dxe Config Block


  @retval SPS_DXE_CONFIG *
**/
SPS_DXE_CONFIG *
GetSpsDxeConfigBlock (
  VOID
  )
{
  EFI_STATUS            Status;
  SPS_POLICY_PROTOCOL   *DxeSpsPolicy;
  SPS_DXE_CONFIG        *SpsDxeConfig = NULL;

  Status = gBS->LocateProtocol (&gDxeSpsPolicyProtocolGuid, NULL, (VOID **) &DxeSpsPolicy);
  ASSERT_EFI_ERROR (Status);
  if (!EFI_ERROR (Status)) {
    Status = GetConfigBlock ((VOID *) DxeSpsPolicy, &gSpsDxeConfigGuid, (VOID *) &SpsDxeConfig);
    if (EFI_ERROR (Status)) {
      SpsDxeConfig = NULL;
      ASSERT_EFI_ERROR (Status);
    }
  }

  return SpsDxeConfig;
} // GetSpsDxeConfigBlock