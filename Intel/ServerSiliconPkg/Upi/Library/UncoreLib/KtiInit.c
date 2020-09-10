/** @file
  Reference Code Module.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2019 Intel Corporation. <BR>

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

#include <Library/UncoreLib.h>
#include <Library/MultiSocketLib.h>
#include <Uefi.h>
#include <Library/PeiServicesLib.h>
#include "Ppi/UpiPlatformHooksPpi.h"
#include <KtiMisc.h>
#include <Library/KtiApi.h>

/**
  Entry point for KTI initialization

  @param None

  @retval N/A

**/
VOID
KtiInit (
  VOID
  )
{
  UINT32  KtiStatus;
  UPI_PLATFORM_HOOKS_PPI*   HooksPpiPointer;
  EFI_STATUS                Status;

  Status = PeiServicesLocatePpi (
             &gUpiPlatformHooksPpiGuid,// GUID
             0,                        // INSTANCE
             NULL,                     // EFI_PEI_PPI_DESCRIPTOR
             (VOID **)&HooksPpiPointer // PPI
             );

  if (Status != EFI_NOT_FOUND && HooksPpiPointer != NULL) {
    HooksPpiPointer->PlatformPreProcInit ();
  }

  //
  // Execute KTI initialization
  //
  KtiStatus = KtiMain ();
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "KTI Init completed! Reset Requested: %x\n", GetSysResetRequired()));

  if (Status != EFI_NOT_FOUND && HooksPpiPointer != NULL) {
    HooksPpiPointer->PlatformPostProcInit (KtiStatus);
  }
}
