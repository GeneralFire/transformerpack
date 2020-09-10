/** @file
  This file is for Intel UPI PLATFORM HOOKS PPI initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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
#include "UpiPlatformHooksPeim.h"
#include <Library/KtiApi.h>

VOID
PlatformPreProcInit (
  IN VOID
  );

VOID
PlatformPostProcInit (
  IN UINT32    ProcStatus
  );

//
// This PPI must be a static global variable in a memory mapped location
// This is because it may be called from AP.
//
static UPI_PLATFORM_HOOKS_PPI                mUpiPlatformHooksPpi = {
  UPI_PLATFORM_HOOKS_PPI_REVISION,
  PlatformPreProcInit,
  PlatformPostProcInit
};

static EFI_PEI_PPI_DESCRIPTOR       mInstallUpiPlatformHooksPpi = {
  EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gUpiPlatformHooksPpiGuid,
  &mUpiPlatformHooksPpi
};

//
// Function implementations
//

/**

  Perform platform specific pre processor init

  @param VOID

  @retval VOID

**/
VOID
PlatformPreProcInit (
  IN VOID
  )
{
}

/**

  Perform platform specific post processor init

  ProcStatus - Return status from Proc RC code execution

  @retval VOID

**/
VOID
PlatformPostProcInit (
  IN UINT32    ProcStatus
  )
{
  UINT8 BitsUsed;

  BitsUsed = GetLatestConsumePA ();
  //
  // The number of bits used is greater than 48 so 5 level paging is needed
  //
  if (BitsUsed > 48) {
    PcdSetBoolS (PcdUse5LevelPageTable, TRUE);
  }
}

/**
  UpiPlatformHooksPeim entry point

  This driver simply publishes a PPI that provides the Platform hooks for silicon
  initialization code.

  @param FileHandle         Pointer to the PEIM FFS file header.
  @param PeiServices        General purpose services available to every PEIM.

  @retval EFI_SUCCESS       Operation completed successfully.
  @retval Otherwise         Upi Platform Ppi Init failed.
**/
EFI_STATUS
EFIAPI
UpiPlatformHooksInitEntry (
  IN EFI_PEI_FILE_HANDLE          FileHandle,
  IN CONST EFI_PEI_SERVICES       **PeiServices
)
{
  EFI_STATUS                   Status;

  //
  //
  // Initialize platform PPIs
  //
  Status = PeiServicesInstallPpi (&mInstallUpiPlatformHooksPpi);
  ASSERT_EFI_ERROR (Status);

  return Status;
}
