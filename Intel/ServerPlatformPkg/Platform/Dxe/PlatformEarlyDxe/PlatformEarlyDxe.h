/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2019 Intel Corporation. <BR>

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

#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/S3BootScriptLib.h>
#include <Protocol/MpService.h>
#include <Guid/PlatformTxt.h>
#include <Cpu/CpuCoreRegs.h>
#include <Platform.h>
#include <PchAccess.h>
#include <Library/SetupLib.h>
#include <RcRegs.h>

extern EFI_GUID gEfiDprRegsProgrammedGuid;

//
// Chispet register
//
#define SA_DPR_REG                          0x290
#define   SA_DPR_LOCK                       0x1
#define   SA_DPR_PRM                        0x4

#define B_HOSTIA_POSTBOOT_SAI               BIT0
#define B_HOSTIA_UCODE_SAI                  BIT1
#define B_HOSTIA_SMM_SAI                    BIT2
#define B_HOSTIA_SUNPASS_SAI                BIT3
#define B_HOSTIA_BOOT_SAI                   BIT4
#define B_PM_PCS_SAI                        BIT9
#define B_DFX_INTEL_MANUFACTURING_SAI       BIT24
#define B_DFX_UNTRUSTED_SAI                 BIT25
#define B_DFX_INTEL_PRODUCTION_SAI          BIT42
#define B_OOB_MSM_SAI                       BIT61

///#define PcdPciExpressBaseAddress           0x80000000


//
// define PCI address macro for reg > 0xFF
//
#define EFI_PCI_ADDRESS_EXT(bus, dev, func, reg) \
    ((UINT64) ((((UINTN) reg) << 32) + (((UINTN) bus) << 24) + (((UINTN) dev) << 16) + (((UINTN) func) << 8)))

//
// LT Configuration Space Register Definitions
//
#define LT_PUBLIC_SPACE_BASE_ADDRESS        0xfed30000
#define LT_DMA_PROTECTED_RANGE_REG_OFF      0x330

EFI_STATUS
EFIAPI
PlatformEarlyDxeEntry (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
);

VOID
ProgramDPRregs (
  VOID
);
