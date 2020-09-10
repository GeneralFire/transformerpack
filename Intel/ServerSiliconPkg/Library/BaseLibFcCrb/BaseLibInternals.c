/** @file
  BaseLibInternals functions of BaseLib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2017 Intel Corporation. <BR>

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

#include "BaseLibInternals.h"
#include <Pi/PiMultiPhase.h>
#include <Cpu/CpuCoreRegs.h>
#include <Register/ArchitecturalMsr.h>

#ifndef B_MSR_IA32_DEBUG_INTERFACE_LOCK
#define B_MSR_IA32_DEBUG_INTERFACE_LOCK         BIT30
#endif

#ifndef B_MSR_PRMRR_CONFIG_BIT
#define B_MSR_PRMRR_CONFIG_BIT                  BIT3
#endif

#ifndef B_MSR_PRMRR_PHYS_MASK_LOCK
#define B_MSR_PRMRR_PHYS_MASK_LOCK              BIT10
#endif

#ifndef B_BIOS_PKG_CONFIG_LOCK
#define B_BIOS_PKG_CONFIG_LOCK                  BIT0
#endif

typedef struct {
  UINT32 Index;
  UINT64 Mask;
} MSR_LOCK_GUARD_DESC;

GLOBAL_REMOVE_IF_UNREFERENCED MSR_LOCK_GUARD_DESC gMsrLockGuard [] = {
  {MSR_IA32_DEBUG_INTERFACE, B_MSR_IA32_DEBUG_INTERFACE_LOCK},
  {MSR_FEATURE_CONFIG, B_MSR_FEATURE_CONFIG_LOCK},
  {ICX_MSR_PRMRR_MASK, B_MSR_PRMRR_PHYS_MASK_LOCK},
  {ICX_MSR_PRMRR_BASE_0, B_MSR_PRMRR_CONFIG_BIT},
  {ICX_MSR_PRMRR_BASE_1, B_MSR_PRMRR_CONFIG_BIT},
  {ICX_MSR_PRMRR_BASE_2, B_MSR_PRMRR_CONFIG_BIT},
  {ICX_MSR_PRMRR_BASE_3, B_MSR_PRMRR_CONFIG_BIT},
  {ICX_MSR_PRMRR_BASE_4, B_MSR_PRMRR_CONFIG_BIT},
  {ICX_MSR_PRMRR_BASE_5, B_MSR_PRMRR_CONFIG_BIT},
  {ICX_MSR_PRMRR_BASE_6, B_MSR_PRMRR_CONFIG_BIT},
  {ICX_MSR_PRMRR_BASE_7, B_MSR_PRMRR_CONFIG_BIT},
  {ICX_MSR_BIOS_PKG_CONFIG, B_BIOS_PKG_CONFIG_LOCK},
  {MSR_PACKAGE_POWER_LIMIT, BIT63},
  {MSR_FLEX_RATIO, BIT20 },
#ifdef SKX_HOST
  {SKX_MSR_POWER_CTL, BIT27},
#else
  {ICX_MSR_POWER_CTL1, BIT23},
#endif
  {ICX_MSR_VR_CURRENT_CONFIG, BIT31},
  {MSR_MISC_PWR_MGMT,BIT13}
};

UINTN gMsrLockGuardEntries = sizeof(gMsrLockGuard)/sizeof(MSR_LOCK_GUARD_DESC);

/**

**/
UINT64
EFIAPI
MsrGuardMask (
  IN UINT32  Index
  )
{
  UINTN                 i;
  for (i=0; i < gMsrLockGuardEntries; i++) {
    if(Index ==  gMsrLockGuard[i].Index) {
      return gMsrLockGuard[i].Mask;
    }
  }

  return 0;
}

