/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#include <PiPei.h>

#include <SysHost.h>

#include <Library/IoLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/BasicIpBlockServicesLib.h>
#include <Library/RcDebugLib.h>

#include <Guid/MrcPlatformHooksGuid.h>

/**

  Platform Hook for resetting the platform.

  @param[in] ResetType - Warm == 0x02, Cold == 0x04

  @retval None

**/

VOID
EFIAPI
IssueReset (
  IN UINT8 ResetType
  )
{

  EFI_STATUS Status;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;
  MRC_STATUS HookStatus;
  EFI_PEI_SERVICES **PeiServices;

  //
  // Check to see if the hook is present
  //

  HookStatus = MRC_FUNCTION_NOT_IMPLEMENTED;

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (MrcPlatformHooksInterface != NULL) {
    HookStatus = MrcPlatformHooksInterface->PlatformIssueReset (ResetType);
  }

  //
  // If the hook was present, and it handled reset, 
  // skip doing it ourselves.
  //

  if (HookStatus != MRC_FUNCTION_NOT_IMPLEMENTED) {
    return;
  }

  PeiServices = (EFI_PEI_SERVICES **) GetPeiServicesTablePointer ();
  if (PeiServices != NULL) {

    if (ResetType == POST_RESET_POWERGOOD) {

      (*PeiServices)->ResetSystem2 (EfiResetCold, EFI_SUCCESS, 0, NULL);

    } else if (ResetType == POST_RESET_WARM) {

      (*PeiServices)->ResetSystem2 (EfiResetWarm, EFI_SUCCESS, 0, NULL);

    }
  }

  //
  // Reset PPI failed to initialize, issue reset via direct IO
  //

  if (ResetType == POST_RESET_POWERGOOD) {
    IoWrite8 (0xcf9, 0x0e);
  } else if (ResetType == POST_RESET_WARM) {
    IoWrite8 (0xcf9, 0x06);
  }

}
