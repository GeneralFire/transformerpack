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

#include <SysHost.h>
#include <MemDecodeCommonIncludes.h>
#include <Library/HobLib.h>
#include <Library/MemMap2SgxInterface.h>

/**
  Get current memory mode for a given socket.

  @param[in]  SocketId           The socket index id

  @param[out] MemMode            The output bitmap for the memory mode attribute.


  @retval EFI_SUCCESS            Successfully get the memory mode attribute in the output bitmap in MemMode.

  @retval EFI_NOT_READY          This function is invoked before memory map flow.

  @retval EFI_INVALID_PARAMETER  MemMode is NULL as input parameter.

  @retval EFI_INVALID_PARAMETER  Socket ID is invalid.
**/
EFI_STATUS
EFIAPI
GetMemMode (
  IN  UINT8                   SocketId,
  OUT MEMORY_MODE_ATTRIBUTE   *MemMode
  )
{
  SECURITY_MEMMAP_HOB         *SecurityMemMapHob;
  SGX_MCHECK_DATA             *SgxMcheckData;
  EFI_HOB_GUID_TYPE           *GuidHob;

  if ((MemMode == NULL) || (SocketId >= MAX_SOCKET)) {
    return EFI_INVALID_PARAMETER;
  }

  GuidHob = GetFirstGuidHob (&gSecurityMemMapHobGuid);
  if (GuidHob == NULL) {
    return EFI_NOT_READY;
  }

  SecurityMemMapHob = GET_GUID_HOB_DATA (GuidHob);
  SgxMcheckData = &SecurityMemMapHob->SgxMcheckData;

  *MemMode = 0;
  switch (SgxMcheckData->VolMemMode) {
  case VOL_MEM_MODE_1LM:
    *MemMode |= MEMORY_MODE_ATTRIBUTE_1LM;
    break;
  case VOL_MEM_MODE_MIX_1LM2LM:
    *MemMode |= MEMORY_MODE_ATTRIBUTE_1LM;
    //
    // break skipped on purpose
    //
  case VOL_MEM_MODE_2LM:
    *MemMode |= MEMORY_MODE_ATTRIBUTE_2LM;
    if (SgxMcheckData->XTile2lm[SocketId]) {
      *MemMode |= MEMORY_MODE_ATTRIBUTE_2LM_XTILE;
    }
  }
  return EFI_SUCCESS;
}
