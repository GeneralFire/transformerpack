/** @file
  This file is for Intel IIO PLATFORM HOOKS PPI initialization.

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

#include "IioPlatformHooksPeim.h"


//
// This PPI must be a static global variable in a memory mapped location
// This is because it may be called from AP.
//

static OEM_IIO_HOOKS_PPI                mOemIioHooksPpi = {
  OemGetIioPlatformInfo,
  OemIioUplinkPortDetails,
  OemIioEarlyWorkAround,
  OemOverrideIioRxRecipeSettings,
  OemDefaultIioPortBifurationInfo,
  OemInitIioPortBifurcationInfo,
  OemDmiPreInit,
  OemDmiDeviceInit,
  OemDmiResourceAssignedInit,
  OemSetIioDefaultValues,
  OemSetPlatformDataValues,
  OemPublishOpaSocketMapHob
};

static EFI_PEI_PPI_DESCRIPTOR       mInstallOemIioHooksPpi = {
  EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gPeiOemIioHooksPpiGuid,
  &mOemIioHooksPpi
};

/**
  IioPlatformHooksPeimEntry entry point

  This driver simply publishes a PPI that provides the OEM IIO hooks for IIO RC
  initialization code.

  @param FileHandle         Pointer to the PEIM FFS file header.
  @param PeiServices        General purpose services available to every PEIM.

  @retval EFI_SUCCESS       Operation completed successfully.
  @retval Otherwise         Oem Iio Hooks Ppi Init failed.
**/
EFI_STATUS
EFIAPI
IioPlatformHooksPeimEntry(
  IN EFI_PEI_FILE_HANDLE          FileHandle,
  IN CONST EFI_PEI_SERVICES       **PeiServices
)
{
  EFI_STATUS                   Status;
  //
  // Initialize Oem IIO hooks PPI
  //
  Status = PeiServicesInstallPpi(&mInstallOemIioHooksPpi);
  ASSERT_EFI_ERROR (Status);
  return Status;
}
