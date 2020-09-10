/** @file

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

#include "SiliconDataInitSmmInternal.h"
/**
  Standard EFI driver point.

  @param ImageHandle         -  A handle for the image that is initializing this driver.
  @param SystemTable         -  A pointer to the EFI system table.

  @retval EFI_SUCCESS        -  Driver initialized successfully.
  @retval All other error conditions encountered result in an ASSERT.

**/
EFI_STATUS
EFIAPI
SiliconDataInitSmm (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                      Status;
  EFI_HANDLE                      Handle;
  EFI_HOB_GUID_TYPE               *GuidHob;
  SYSTEM_INFO_VAR                 *SystemInfoVar;
  SYSTEM_INFO_VAR_PROTOCOL        *SmmSystemInfoVarProtocol;
  REFERENCE_CODE_POLICY           *ReferenceCodePolicy;
  REFERENCE_CODE_POLICY_PROTOCOL  *SmmReferenceCodePolicyProtocol;
  EMULATION_SETTING               *EmulationSetting;
  EMULATION_SETTING_PROTOCOL      *SmmEmulationSetting;

  Handle = NULL;
  GuidHob = GetFirstGuidHob (&gSystemInfoVarHobGuid);
  if (GuidHob == NULL) {
    ASSERT (GuidHob != NULL);
    return EFI_NOT_FOUND;
  }
  SystemInfoVar = GET_GUID_HOB_DATA(GuidHob);

  SmmSystemInfoVarProtocol = (SYSTEM_INFO_VAR_PROTOCOL *)AllocatePool (sizeof (SYSTEM_INFO_VAR));

  CopyMem (
      (UINT8*)SmmSystemInfoVarProtocol,
      (UINT8*)SystemInfoVar,
      sizeof (SYSTEM_INFO_VAR)
      );

  Status = gSmst->SmmInstallProtocolInterface (
                    &Handle,
                    &gSmmSystemInfoVarProtocol,
                    EFI_NATIVE_INTERFACE,
                    SmmSystemInfoVarProtocol
                    );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  GuidHob = GetFirstGuidHob (&gReferenceCodePolicyHobGuid);
  if (GuidHob == NULL) {
    ASSERT (GuidHob != NULL);
    return EFI_NOT_FOUND;
  }
  ReferenceCodePolicy = GET_GUID_HOB_DATA(GuidHob);

  SmmReferenceCodePolicyProtocol = (REFERENCE_CODE_POLICY_PROTOCOL *)AllocatePool (sizeof (REFERENCE_CODE_POLICY));

  CopyMem (
      (UINT8*)SmmReferenceCodePolicyProtocol,
      (UINT8*)ReferenceCodePolicy,
      sizeof (REFERENCE_CODE_POLICY)
      );

  Status = gSmst->SmmInstallProtocolInterface (
                    &Handle,
                    &gSmmReferenceCodePolicyProtocol,
                    EFI_NATIVE_INTERFACE,
                    SmmReferenceCodePolicyProtocol
                    );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Handle = NULL;
  GuidHob = GetFirstGuidHob (&gEmulationHobGuid);
  if (GuidHob == NULL) {
    ASSERT (GuidHob != NULL);
    return EFI_NOT_FOUND;
  }
  EmulationSetting  = GET_GUID_HOB_DATA (GuidHob);

  SmmEmulationSetting = (EMULATION_SETTING_PROTOCOL *)AllocatePool (sizeof (EMULATION_SETTING));

  CopyMem (
      (UINT8*)SmmEmulationSetting,
      (UINT8*)EmulationSetting,
      sizeof (EMULATION_SETTING)
      );

  Status = gSmst->SmmInstallProtocolInterface (
                    &Handle,
                    &gSmmEmulationSettingProtocol,
                    EFI_NATIVE_INTERFACE,
                    SmmEmulationSetting
                    );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return EFI_SUCCESS;
}
