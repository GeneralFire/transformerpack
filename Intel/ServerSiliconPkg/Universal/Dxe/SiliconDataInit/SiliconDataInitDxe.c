/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 -2019 Intel Corporation. <BR>

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

#include "SiliconDataInitDxeInternal.h"
#include <Library/DxeServicesTableLib.h>
/**
  Standard EFI driver point.

  @param ImageHandle         -  A handle for the image that is initializing this driver.
  @param SystemTable         -  A pointer to the EFI system table.

  @retval EFI_SUCCESS        -  Driver initialized successfully.
  @retval All other error conditions encountered result in an ASSERT.

**/
EFI_STATUS
EFIAPI
SiliconDataInitDxe (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                        Status;
  EFI_HANDLE                        Handle;
  EFI_HOB_GUID_TYPE                 *GuidHob;
  EFI_PHYSICAL_ADDRESS              BaseAddressMem;
  SYSTEM_INFO_VAR                   *SystemInfoVar;
  SYSTEM_INFO_VAR_PROTOCOL          *DxeSystemInfoVarProtocol;

  REFERENCE_CODE_POLICY             *ReferenceCodePolicy;
  REFERENCE_CODE_POLICY_PROTOCOL    *DxeReferenceCodePolicyProtocol;

  EFI_GCD_MEMORY_SPACE_DESCRIPTOR   MemorySpaceDescriptor;
  UINT64                            Attributes;

  Handle = NULL;

  /*
    Produce DXE protocol for system info var
  */
  GuidHob = GetFirstGuidHob (&gSystemInfoVarHobGuid);
  if (GuidHob == NULL) {
    ASSERT (GuidHob != NULL);
    return  EFI_NOT_FOUND;
  }
  SystemInfoVar = GET_GUID_HOB_DATA(GuidHob);

  BaseAddressMem = 0xFFFFFFFF;
  Status = gBS->AllocatePages (
                AllocateMaxAddress,
                EfiReservedMemoryType,
                EFI_SIZE_TO_PAGES (sizeof (SYSTEM_INFO_VAR)),
                &BaseAddressMem
                );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }
  DxeSystemInfoVarProtocol = (SYSTEM_INFO_VAR_PROTOCOL*) BaseAddressMem;
  CopyMem (
      (UINT8*)DxeSystemInfoVarProtocol,
      (UINT8*)SystemInfoVar,
      sizeof (SYSTEM_INFO_VAR)
      );

 //Install Protocol
  Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gDxeSystemInfoVarProtocol,
                    EFI_NATIVE_INTERFACE,
                    DxeSystemInfoVarProtocol
                    );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  /*
    Produce DXE protocol for Reference Code Policy
  */

  GuidHob = GetFirstGuidHob (&gReferenceCodePolicyHobGuid);
  if (GuidHob == NULL) {
    ASSERT (GuidHob != NULL);
    return  EFI_NOT_FOUND;
  }
  ReferenceCodePolicy = GET_GUID_HOB_DATA(GuidHob);

  BaseAddressMem = 0xFFFFFFFF;
  Status = gBS->AllocatePages (
                AllocateMaxAddress,
                EfiReservedMemoryType,
                EFI_SIZE_TO_PAGES (sizeof (REFERENCE_CODE_POLICY)),
                &BaseAddressMem
                );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = gDS->GetMemorySpaceDescriptor ((EFI_PHYSICAL_ADDRESS)BaseAddressMem, &MemorySpaceDescriptor);
  ASSERT_EFI_ERROR (Status);

  Attributes = MemorySpaceDescriptor.Attributes | EFI_MEMORY_RUNTIME;

  Status = gDS->SetMemorySpaceAttributes (
                  (EFI_PHYSICAL_ADDRESS)BaseAddressMem,
                  (sizeof (REFERENCE_CODE_POLICY) + EFI_PAGE_SIZE - 1) & (~EFI_PAGE_MASK),
                  Attributes
                  );
  ASSERT_EFI_ERROR (Status);

  DxeReferenceCodePolicyProtocol = (REFERENCE_CODE_POLICY_PROTOCOL*) BaseAddressMem;
  CopyMem (
      (UINT8*)DxeReferenceCodePolicyProtocol,
      (UINT8*)ReferenceCodePolicy,
      sizeof(REFERENCE_CODE_POLICY)
      );

  Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gDxeReferenceCodePolicyProtocol,
                    EFI_NATIVE_INTERFACE,
                    DxeReferenceCodePolicyProtocol
                    );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return EFI_SUCCESS;
}
