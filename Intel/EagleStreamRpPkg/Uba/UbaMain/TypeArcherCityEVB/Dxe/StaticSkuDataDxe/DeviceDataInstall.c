/** @file
  Device data installation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2018 Intel Corporation. <BR>

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

#include "StaticSkuDataDxe.h"

#include <Library/UbaPirqUpdateLib.h>
#include <Library/UbaMpTableUpdateLib.h>

#include <PlatPirqData.h>
#include <PlatDevData.h>

extern PLATFORM_PIRQ_DATA    mPlatformPirqDataPlatformSRP;

PLATFORM_PIRQ_UPDATE_TABLE  PirqUpdate =
{
  PLATFORM_PIRQ_UPDATE_SIGNATURE,
  PLATFORM_PIRQ_UPDATE_VERSION,
  &mPlatformPirqDataPlatformSRP
};

EFI_STATUS
InstallPirqData (
  IN UBA_CONFIG_DATABASE_PROTOCOL    *UbaConfigProtocol
)
{
  EFI_STATUS                            Status = EFI_SUCCESS;
DEBUG((DEBUG_INFO,"UBA DXE: %a\n",__FUNCTION__));
  Status = UbaConfigProtocol->AddData (
                                     UbaConfigProtocol, 
                                     &gPlatformPirqConfigDataGuid, 
                                     &PirqUpdate, 
                                     sizeof(PirqUpdate)
                                     );

  return Status;
}

extern DEVICE_DATA           mDeviceDataPlatformSRP;

PLATFORM_MP_UPDATE_TABLE  MpTableUpdate =
{
  PLATFORM_MP_TABLE_UPDATE_SIGNATURE,
  PLATFORM_MP_TABLE_UPDATE_VERSION,
  &mDeviceDataPlatformSRP
};

EFI_STATUS
InstallMpTableData (
  IN UBA_CONFIG_DATABASE_PROTOCOL    *UbaConfigProtocol
)
{
  EFI_STATUS                            Status = EFI_SUCCESS;

DEBUG((DEBUG_INFO,"UBA DXE: %a\n",__FUNCTION__));
  Status = UbaConfigProtocol->AddData (
                                     UbaConfigProtocol,
                                     &gPlatformMpTableConfigDataGuid, 
                                     &MpTableUpdate, 
                                     sizeof(MpTableUpdate)
                                     );

  return Status;
}
