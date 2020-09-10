/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#ifndef _SGX_EARLY_INIT_H_
#define _SGX_EARLY_INIT_H_

#include <Register/Cpuid.h>
#include <Register/ArchitecturalMsr.h>

#include <Base.h>
#include <PiPei.h>
#include <Uefi.h>
#include <Uefi/UefiBaseType.h>
#include <Ppi/MpServices.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PcdLib.h>
#include <Cpu/CpuCoreRegs.h>
#include <RcRegs.h>
#include <SysHost.h>
#include <Chip/Include/CpuPciAccess.h>
#include <MemHostChipCommon.h>

#include <Guid/SgxInitDataSpr.h>

#define B_MSR_PRMRR_CONFIG_BIT                  BIT3
#define B_MSR_PRMRR_PHYS_MASK_ASSIST            BIT9
#define B_MSR_PRMRR_PHYS_MASK_LOCK              BIT10
#define B_MSR_PRMRR_PHYS_MASK_VALID             BIT11
#define B_TME_ACTIVATE_KEYIDBITS_MASK           (BIT0 | BIT1 | BIT2 | BIT3)
#define B_BIOS_PKG_CONFIG_ENABLE                BIT1
#define B_BIOS_PKG_CONFIG_LOCK                  BIT0
#define B_SPR_MSR_SGX_RAS_OPTIN                 BIT0
#define B_HOSTIA_POSTBOOT_SAI                   BIT0
#define B_HOSTIA_UCODE_SAI                      BIT1
#define B_HOSTIA_SMM_SAI                        BIT2
#define B_HOSTIA_SUNPASS_SAI                    BIT3
#define B_HOSTIA_BOOT_SAI                       BIT4
#define B_PM_PCS_SAI                            BIT9
#define B_DFX_INTEL_MANUFACTURING_SAI           BIT24
#define B_DFX_INTEL_PRODUCTION_SAI              BIT42
#define B_OOB_MSM_SAI                           BIT61
#define B_MSR_MCG_CONTAIN_POISON_ENABLE         BIT0
#define B_MSR_MCG_CAP_SER_P                     BIT24

#define CSR_MULTICAST                           0xFF

#define MCHECK_SAI_POLICY_LOW_MASK  (B_HOSTIA_UCODE_SAI | B_HOSTIA_SUNPASS_SAI | B_PM_PCS_SAI | B_DFX_INTEL_MANUFACTURING_SAI) // Permitted agents by MCHECK for CHA's and UBOX's SAIs, lower 32 bits
#define MCHECK_SAI_POLICY_HIGH_MASK (B_DFX_INTEL_PRODUCTION_SAI >> 32) // Permitted agents by MCHECK for CHA's and UBOX's SAIs, higher 32 bits

#define FULL_64BIT_MASK 0xFFFFFFFFFFFFFFFF

#endif // _SGX_EARLY_INIT_H_
