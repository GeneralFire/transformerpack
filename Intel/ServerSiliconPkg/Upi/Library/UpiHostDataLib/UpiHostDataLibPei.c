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

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/HobLib.h>

/**
  Get the KTI Host data pointer.
  @param KtiHostType  -  The KTI host data type defined in KTI_HOST_TYPE
  @return pointer of the KTI Host data

 **/
VOID *
EFIAPI
GetKtiHostDataPtr (
  KTI_HOST_TYPE KtiHostType
  )
{
  VOID           *GuidHob;
  EFI_GUID       *HobGuid;

  ASSERT (KtiHostType < KtiHostMax);

  if (KtiHostType == KtiHostIn) {
    HobGuid = &gEfiKtiHostInDataHobGuid;
  } else if (KtiHostType == KtiHostOut) {
    HobGuid = &gEfiKtiHostOutDataHobGuid;
  } else if (KtiHostType == KtiHostNvram) {
    HobGuid = &gEfiKtiHostNvramDataHobGuid;
  } else {
    ASSERT (0);
    return NULL;
  }

  GuidHob = GetFirstGuidHob (HobGuid);
  ASSERT (GuidHob != NULL);

  return (VOID *)(GET_GUID_HOB_DATA (GuidHob));
}
