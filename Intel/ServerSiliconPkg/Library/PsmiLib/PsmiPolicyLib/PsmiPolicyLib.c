/** @file
  Implementation of PSMI Policy library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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
#include <Library/BaseLib.h>
#include <Uefi.h>
#include <SysHostChipCommon.h>
#include <Pi/PiBootMode.h>
#include <Pi/PiHob.h>
#include <Library/PsmiPolicyLib.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
/**
  Set PSMI policy info into system.

  @param   PsmiPolicy       Pointer to PSMI Policy Info.
  @param   DataSize         The size of PSMI Policy.

  @retval  EFI_SUCCESS  Set PSMI policy success.
  @retval EFI_OUT_OF_RESOURCES Out of resource;

**/

EFI_STATUS
CreateDefaultPsmiPolicy ()
{
  struct psmiSetup PsmiPolicy;
  //
  // Init PSMI feature related setup options to default safe values that ensure minimum boot.
  // A platform hook will be called later to override these values.
  //
  ZeroMem ((VOID*)&PsmiPolicy, sizeof (PsmiPolicy));
  return SetPsmiPolicy (&PsmiPolicy, sizeof (PsmiPolicy));

}

EFI_STATUS
EFIAPI
SetPsmiPolicy (
  IN VOID *PsmiPolicy,
  IN UINTN DataSize
  )
{
  VOID      *PsmiPolicyGuidHob;
  VOID      *GuidData;
  UINTN      GuidDataSize;
  EFI_STATUS Status = EFI_SUCCESS;

  PsmiPolicyGuidHob = GetFirstGuidHob (&gEfiPsmiPolicyDataHobGuid);
  if (PsmiPolicyGuidHob == NULL) {
    GuidData = BuildGuidDataHob (&gEfiPsmiPolicyDataHobGuid, PsmiPolicy, DataSize);
  }else {
    GuidData = GET_GUID_HOB_DATA (PsmiPolicyGuidHob);
    GuidDataSize = GET_GUID_HOB_DATA_SIZE (PsmiPolicyGuidHob);
    if (GuidDataSize >= DataSize) {
      CopyMem (GuidData, PsmiPolicy, DataSize);
    } else {
      Status = EFI_OUT_OF_RESOURCES;
    }
  }

  return Status;
}

/**
  Get PSMI policy info into system.

  @param   PsmiPolicy       Pointer to memory to get Psmi Policy Info.
  @param   BufferSize       The buffer's size of the Psmi Policy

  @retval  EFI_SUCCESS  Get Psmi policy success.
  @retval  EFI_NOT_FOUND Can't find the Psmi Policy.
  @retval  EFI_BUFFER_TOO_SMALL the Buffer's size is too small.


**/
struct psmiSetup *
EFIAPI
GetPsmiPolicy (
  )
{
  VOID *PsmiPolicyGuidHob;
  VOID *GuidData;
  EFI_STATUS Status;
  PsmiPolicyGuidHob = GetFirstGuidHob (&gEfiPsmiPolicyDataHobGuid);
  if (PsmiPolicyGuidHob == NULL) {
    Status = CreateDefaultPsmiPolicy ();
    if (!EFI_ERROR (Status)) {
      PsmiPolicyGuidHob = GetFirstGuidHob (&gEfiPsmiPolicyDataHobGuid);
    }
  }
  GuidData = GET_GUID_HOB_DATA (PsmiPolicyGuidHob);
  return GuidData;
}



