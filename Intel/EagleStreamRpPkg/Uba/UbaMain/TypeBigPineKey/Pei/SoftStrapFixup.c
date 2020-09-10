/** @file
  Soft Strap update.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include "PeiBoardInit.h"
#include <Library/UbaSoftStrapUpdateLib.h>
#include <GpioConfig.h>
#include <Library/HobLib.h>
#include <Guid/PlatformInfo.h>

PLATFORM_PCH_SOFTSTRAP_FIXUP_ENTRY  TypeBigPineKeySoftStrapTable[] =
{
// SoftStrapNumber, LowBit, BitLength, Value

//
// END OF LIST
//
  {0, 0, 0, 0}
};

UINT32
TypeBigPineKeySystemBoardRevIdValue (VOID)
{
  EFI_HOB_GUID_TYPE       *GuidHob;
  EFI_PLATFORM_INFO       *PlatformInfo;

  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  ASSERT(GuidHob != NULL);
  if (GuidHob == NULL) {
    return 0xFF;
  }
  PlatformInfo  = GET_GUID_HOB_DATA(GuidHob);
  return PlatformInfo->TypeRevisionId;
}

VOID
TypeBigPineKeyPlatformSpecificUpdate (
  IN OUT  UINT8                 *FlashDescriptorCopy
  )
{
}

PLATFORM_PCH_SOFTSTRAP_UPDATE  TypeBigPineKeySoftStrapUpdate =
{
  PLATFORM_SOFT_STRAP_UPDATE_SIGNATURE,
  PLATFORM_SOFT_STRAP_UPDATE_VERSION,
  TypeBigPineKeySoftStrapTable,
  TypeBigPineKeyPlatformSpecificUpdate
};

EFI_STATUS
TypeBigPineKeyInstallSoftStrapData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
  )
{
  EFI_STATUS                            Status;

  Status = UbaConfigPpi->AddData (
                                 UbaConfigPpi,
                                 &gPlatformPchSoftStrapConfigDataGuid,
                                 &TypeBigPineKeySoftStrapUpdate,
                                 sizeof(TypeBigPineKeySoftStrapUpdate)
                                 );

  return Status;
}

