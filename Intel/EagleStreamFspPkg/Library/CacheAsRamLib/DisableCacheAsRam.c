/** @file
  Disable Cache As Ram

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 Intel Corporation. <BR>

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
#include <Library/BaseLib.h>
#include <Library/CacheAsRamLib.h>

#define   EFI_MSR_NO_EVICT_MODE                  0x000002E0
#define   B_EFI_MSR_NO_EVICT_MODE_RUN            BIT1
#define   B_EFI_MSR_NO_EVICT_MODE_SETUP          BIT0
/**
  This function disable CAR.

  @param[in] DisableCar       TRUE means use INVD, FALSE means use WBINVD

**/
VOID
EFIAPI
DisableCacheAsRam (
  IN BOOLEAN                   DisableCar
  )
{
  //
  // Disable CAR
  //
  UINT64 CacheAsRamMsr;

  CacheAsRamMsr = AsmReadMsr64 (EFI_MSR_NO_EVICT_MODE);

  //
  // Disable No-Eviction Mode Run State by clearing
  // NO_EVICT_MODE MSR 2E0h bit [1] = 0
  CacheAsRamMsr &= ~B_EFI_MSR_NO_EVICT_MODE_RUN;
  AsmWriteMsr64 (EFI_MSR_NO_EVICT_MODE, CacheAsRamMsr);

  // Disable No-Eviction Mode Setup State by clearing
  // NO_EVICT_MODE MSR 2E0h bit [0] = 0
  CacheAsRamMsr &= ~B_EFI_MSR_NO_EVICT_MODE_SETUP;
  AsmWriteMsr64 (EFI_MSR_NO_EVICT_MODE, CacheAsRamMsr);

  if (DisableCar) {
    AsmInvd ();
  } else {
    AsmWbinvd ();
  }

  return ;
}
