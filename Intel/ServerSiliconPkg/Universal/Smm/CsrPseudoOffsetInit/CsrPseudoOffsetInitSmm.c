/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2018 Intel Corporation. <BR>

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

#include "CsrPseudoOffsetInitSmmInternal.h"
/**
  Standard EFI driver point.

  @param ImageHandle         -  A handle for the image that is initializing this driver.
  @param SystemTable         -  A pointer to the EFI system table.

  @retval EFI_SUCCESS        -  Driver initialized successfully.
  @retval All other error conditions encountered result in an ASSERT.

**/
EFI_STATUS
EFIAPI
CsrPseudoOffsetInitSmm (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS              Status;
  EFI_HANDLE              Handle = NULL;

#if (TOTAL_PSEUDO_OFFSET_TABLES == 0)

  if (gSmst == NULL) {
    ASSERT (FALSE);
    return EFI_NOT_READY;
  }
  Status = gSmst->SmmInstallProtocolInterface (
                    &Handle,
                    &gSmmPseudoOffestInfoProtocol,
                    EFI_NATIVE_INTERFACE,
                    NULL
                    );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }
  return EFI_SUCCESS;
#else
  EFI_HOB_GUID_TYPE       *GuidHob;
  PSEUDO_OFFSET_TABLE_HEADER       *TableHeader;
  CSR_PSEUDO_OFFEST_INFO_PROTOCOL  *SmmCsrPseudoOffsetInfo;

  GuidHob = GetFirstGuidHob (&gCsrPseudoOffsetTableGuid);
  if (GuidHob == NULL) {
    ASSERT (GuidHob != NULL);
    return EFI_NOT_FOUND;
  }
  TableHeader  = GET_GUID_HOB_DATA(GuidHob);

  SmmCsrPseudoOffsetInfo = (CSR_PSEUDO_OFFEST_INFO_PROTOCOL *)AllocatePool (TableHeader->TableSize);

  CopyMem (
      (UINT8*)SmmCsrPseudoOffsetInfo,
      (UINT8*)TableHeader,
      TableHeader->TableSize
      );

  //Install Protocol
  if (gSmst == NULL) {
    ASSERT (FALSE);
    return EFI_NOT_READY;
  }
  Status = gSmst->SmmInstallProtocolInterface (
                    &Handle,
                    &gSmmPseudoOffestInfoProtocol,
                    EFI_NATIVE_INTERFACE,
                    SmmCsrPseudoOffsetInfo
                    );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }
  return EFI_SUCCESS;
#endif // #if (TOTAL_PSEUDO_OFFSET_TABLES == 0)
}
