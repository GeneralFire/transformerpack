/** @file

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

#include "BzmDxe.h"
#include <Cpu/CpuCoreRegs.h>
#include <Library/BaseLib.h>
#include <Protocol/MpService.h>
#include <Library/UefiBootServicesTableLib.h>

/**
  Set Fast Zero Memory MSR.

  This function programs the Features Control MSR to enable the Boot-time
  Fast Zero Memory support.

**/
VOID
EFIAPI
SetFastZeroMemMsr (
  VOID
  )
{
  SPR_MSR_OPTIN_FEATURE_CONTROL_REGISTER  MsrReg;

  MsrReg.Uint64 = AsmReadMsr64 (SPR_MSR_OPTIN_FEATURE_CONTROL);
  MsrReg.Bits.FzmEnable = 1;
  AsmWriteMsr64 (SPR_MSR_OPTIN_FEATURE_CONTROL, MsrReg.Uint64);
}

/**
  This function programs MSR to enable the Boot-time fast Zero Memory
  support on all processors.

  @retval EFI_SUCCESS   This function programmed the MSR successfully.
  @retval Others        This function failed to program the MSR.

**/
EFI_STATUS
EnableBzmSupport (
  VOID
  )
{
  EFI_STATUS                Status;
  EFI_MP_SERVICES_PROTOCOL  *MpService;

  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, (VOID **) &MpService);
  ASSERT_EFI_ERROR (Status);

  //
  // Enable Boot-time fast Zero Memory on BSP.
  //
  SetFastZeroMemMsr ();

  //
  // Enable Boot-time fast Zero Memory on all APs.
  //
  Status = MpService->StartupAllAPs (MpService, (EFI_AP_PROCEDURE) SetFastZeroMemMsr, FALSE, NULL, 0, NULL, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[BZM] Failed to program MSR on APs, Status: %r\n", Status));
    return Status;
  }
  DEBUG ((DEBUG_INFO, "[BZM] The MSR has been programmed on all processors to enable BZM support\n"));

  return EFI_SUCCESS;
}
