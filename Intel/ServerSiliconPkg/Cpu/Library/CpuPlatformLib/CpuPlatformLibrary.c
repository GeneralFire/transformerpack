/** @file
  CPU Platform Lib implementation.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/

#include <Cpu/CpuCoreRegs.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Register/Intel/Cpuid.h>


#define MSR_SPCL_CHIPSET_USAGE_ADDR 0x000001FE

/**
  Is BIOS GUARD enabled.

  @retval TRUE   BIOS GUARD is supported and enabled.
  @retval FALSE  BIOS GUARD is disabled.
**/
BOOLEAN
IsBiosGuardEnabled (
  VOID
  )
{
  ICX_MSR_PLATFORM_INFO_REGISTER    PlatformInfoMsr;
  MSR_PLAT_FRMW_PROT_CTRL_REGISTER  PlatFirmCtrlMsr;

  PlatformInfoMsr.Uint64 = AsmReadMsr64 (ICX_MSR_PLATFORM_INFO);
  if (PlatformInfoMsr.Bits.PfatEnable != 0) {
    PlatFirmCtrlMsr.Uint64 = AsmReadMsr64 (MSR_PLAT_FRMW_PROT_CTRL);
    if (PlatFirmCtrlMsr.Bits.PfatEnable != 0) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Get processor generation

  @retval CPU_GENERATION  Returns the executing thread's processor generation.
**/
UINT32
GetCpuGeneration (
  VOID
  )
{
  return 255;
}

/**
  Return Cpu stepping type

  @retval UINT8                   Cpu stepping type
**/
UINT8
EFIAPI
GetCpuStepping (
  VOID
  )
{
  CPUID_VERSION_INFO_EAX  Eax;
  ///
  /// Read the CPUID information
  ///
  AsmCpuid (CPUID_VERSION_INFO, &Eax.Uint32, NULL, NULL, NULL);
  return ((UINT8)Eax.Bits.SteppingId);
}
