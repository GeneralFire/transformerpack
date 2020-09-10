/** @file
  The CPU specific programming for PiSmmCpuDxeSmm module.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2010 - 2020 Intel Corporation. <BR>

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

#ifndef __SMM_FEATURES_H__
#define __SMM_FEATURES_H__

#include "CpuHotPlugData.h" // Copy from UefiCpuPkg and add some close source definitions
#include "SmramSaveStateMap.h" // Copy from UefiCpuPkg and add some close source definitions
#include <PiSmm.h>

#include <Protocol/SmmAccess2.h>

#include "SmmCpuFeaturesLib.h"
#include <Library/SmmCpuPlatformHookLib.h>

#include <Library/PcdLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PciLib.h>
#include <Library/LocalApicLib.h>
#include <Library/SynchronizationLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MtrrLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/HobLib.h>
#include <Register/ArchitecturalMsr.h>
#include <Cpu/CpuCoreRegs.h>
#include <Register/Intel/Cpuid.h>

#include <UncoreCommonIncludes.h>

#define CACHE_WRITE_PROTECT                    5
#define CACHE_WRITE_BACK                       6
#define SMM_DEFAULT_SMBASE                     0x30000
#define CR0_WP                                 BIT16

#define SMM_IEDBASE_OFFSET                     0xFEEC
#define IED_STRING                             "INTEL RSVD"

#define CHA_SMM_REG_EN                         BIT11

#define SKYLAKE_NON_STICKY_SCRATCHPAD3(Bus)    PCI_LIB_ADDRESS (Bus, 8, 2, 0xE8)
#define TEN_NM_NON_STICKY_SCRATCHPAD3(Bus)     PCI_LIB_ADDRESS (Bus, 0, 2, 0xE8)

//
// Server register defines
//
#define SKYLAKE_SMM_DELAYED0_REG(Bus)          PCI_LIB_ADDRESS (Bus, 8, 1, 0x40)
#define SKYLAKE_SMM_DELAYED1_REG(Bus)          PCI_LIB_ADDRESS (Bus, 8, 1, 0x44)
#define SKYLAKE_SMM_BLOCKED0_REG(Bus)          PCI_LIB_ADDRESS (Bus, 8, 1, 0x48)
#define SKYLAKE_SMM_BLOCKED1_REG(Bus)          PCI_LIB_ADDRESS (Bus, 8, 1, 0x4C)
#define SKYLAKE_SMM_FEATURE_CONTROL_REG(Bus)   PCI_LIB_ADDRESS (Bus, 8, 1, 0x7C)

#define ICXSP_R_SMM_DELAYED_REG(Bus)                    PCI_LIB_ADDRESS (Bus, 0, 1, 0x40) // SMM_ENABLE_DELAYED_COUNT_UBOX_DFX_REG
#define ICXSP_R_SMM_BLOCKED_REG(Bus)                    PCI_LIB_ADDRESS (Bus, 0, 1, 0x44) // SMM_ENABLE_BLOCKED_COUNT_UBOX_DFX_REG

#define SNR_A_SMM_DELAYED_REG(Bus)                      PCI_LIB_ADDRESS (Bus, 0, 1, 0x40) // SMM_ENABLE_DELAYED_COUNT_UBOX_DFX_REG
#define SNR_A_SMM_BLOCKED_REG(Bus)                      PCI_LIB_ADDRESS (Bus, 0, 1, 0x44) // SMM_ENABLE_BLOCKED_COUNT_UBOX_DFX_REG

#define TEN_NM_SMM_DELAYED_REG(Bus)                     PCI_LIB_ADDRESS (Bus, 0, 1, 0x150) // SMM_ENABLE_DELAYED_COUNT_UBOX_DFX_REG
#define TEN_NM_SMM_BLOCKED_REG(Bus)                     PCI_LIB_ADDRESS (Bus, 0, 1, 0x154) // SMM_ENABLE_BLOCKED_COUNT_UBOX_DFX_REG
#define TEN_NM_SMM_FEATURE_CONTROL_REG(Bus)             PCI_LIB_ADDRESS (Bus, 0, 1, 0x8C)
#define TEN_NM_SMMSEG_BASE_N0_CHABC_SAD_REG(Bus)        PCI_LIB_ADDRESS (Bus, 29, 0, 0xA0)
#define TEN_NM_SMMSEG_BASE_N1_CHABC_SAD_REG(Bus)        PCI_LIB_ADDRESS (Bus, 29, 0, 0xA4)
#define TEN_NM_SMMSEG_LIMIT_N0_CHABC_SAD_REG(Bus)       PCI_LIB_ADDRESS (Bus, 29, 0, 0xA8)
#define TEN_NM_SMMSEG_LIMIT_N1_CHABC_SAD_REG(Bus)       PCI_LIB_ADDRESS (Bus, 29, 0, 0xAC)

//
// Define for Protected Mode SMM Entry Template
//
#define SMMSEG_PROTECT_MODE_CODE_SEGMENT   0x10

//
// Below section is definition for CPU SMM Feature context
//

//
// Structure to describe CPU identification mapping
// if ((CPUID_EAX(1) & Mask) == (Signature & Mask)), it means matched.
//
typedef struct {
  UINT32  Signature;
  UINT32  Mask;
} CPUID_MAPPING;

//
// CPU SMM familiy
//
typedef enum {
  CpuSkylake,
  CpuIcelake,
  CpuSpr,
  CpuSnowridge,
  CpuSmmFamilyMax
} CPU_SMM_FAMILY;

//
// Structure to describe CPU SMM class
//
typedef struct {
  CPU_SMM_FAMILY    Family;
  UINT32            MappingCount;
  CPUID_MAPPING     *MappingTable;
} CPU_SMM_CLASS;

//
// Structure to describe CPU_SMM_FEATURE_CONTEXT
//
typedef struct {
  BOOLEAN          SmrrEnabled;
  BOOLEAN          EmrrSupported;
  BOOLEAN          Smrr2Enabled;
} CPU_SMM_FEATURE_CONTEXT;

//
// SMMSEG_FEATURE_ENABLES bitmap
//
#define SMMSEG_FEATURE_ENABLE                  BIT0
#define SMMSEG_FEATURE_CR4_MCE_CTL_ENABLE      BIT1

//
// Structure to describe CPU SMM Protected Mode
//
typedef struct {
  UINT32  Reserved1;
  UINT32  SmmSegFeatureEnables;   // SMM_SEG_FEATURE_ENABLES
  UINT32  GDTRLimit;
  UINT32  GDTRBaseOffset;
  UINT32  CSSelector;
  UINT32  Reserved3;
  UINT32  ESPOffset;
  UINT32  Reserved4;
  UINT32  IDTRLimit;
  UINT32  IDTRBaseOffset;
} SMMSEG;

#define  SMM_PROT_MODE_GDT_ENTRY_COUNT    27

//
// SMM PROT MODE CONTEXT (total 0x100 bytes)
//
typedef struct {
  SMMSEG                    SmmProtectedModeSMMSEG;                       // 40 bytes
  IA32_SEGMENT_DESCRIPTOR   SmmProtectedModeGdt[SMM_PROT_MODE_GDT_ENTRY_COUNT];  // 27 * 8 = 216 bytes
} SMM_PROT_MODE_CONTEXT;

//
// CPUID masks
//
#define CPUID_MASK_NO_STEPPING                  0x0FFF0FF0
#define CPUID_MASK_NO_STEPPING_MODEL            0x0FFF0F00

//
// SMM CPU synchronization features available on a processor
//
typedef struct {
  BOOLEAN          DelayIndicationSupported;
  BOOLEAN          BlockIndicationSupported;
  //
  // This processor's LOG_PROC_EN bit used in SMM_ENABLE, SMM_DELAYED, and SMM_BLOCKED MSRs
  // (introduced in Haswell processor).
  // Value of "-1" indicates this field is invalid (i.e. LOG_PROC_EN bit is not
  // supported)
  //
  UINT64           HaswellLogProcEnBit;
} SMM_CPU_SYNC_FEATURE;

extern BOOLEAN    mSaveStateInMsr;
extern BOOLEAN    mSmmInitSaveStateInMsr;
extern UINT8      mSmmFeatureSaveStateRegisterLma;
extern SPIN_LOCK  *mMsrSpinLock;

/**
  Read MSR or CSR based on the CPU type Register to read.

  NOTE: Since platform may uses I/O ports 0xCF8 and 0xCFC to access
        CSR, we need to use SPIN_LOCK to avoid collision on MP System.

  @param[in]  CpuIndex  The processor index.
  @param[in]  RegName   Register name.

  @return 64-bit value read from register.

**/
UINT64
SmmReadReg64 (
  IN  UINTN           CpuIndex,
  IN  SMM_REG_NAME    RegName
  );

/**
  Write MSR or CSR based on the CPU type Register to write.

  NOTE: Since platform may uses I/O ports 0xCF8 and 0xCFC to access
        CSR, we need to use SPIN_LOCK to avoid collision on MP System.

  @param[in]  CpuIndex  The processor index.
  @param[in]  RegName   Register name.
  @param[in]  RegValue  64-bit Register value.

**/
VOID
SmmWriteReg64 (
  IN  UINTN           CpuIndex,
  IN  SMM_REG_NAME    RegName,
  IN  UINT64          RegValue
  );

/**
  Initialize SMM Protected Mode IDT table.

  @param[in]  Cr3      CR3 value.
**/
VOID
InitProtectedModeIdt (
  IN UINT32  Cr3
  );

/**
  Allocate pages for code.

  @param  Pages Number of pages to be allocated.

  @return Allocated memory.
**/
VOID *
SmmFeatureAllocateCodePages (
  IN UINTN           Pages
  );

/**
  This function sets SmmSupovrStateLock MSR.

  NOTE: This function does not take effect before
  SmmCpuFeaturesCompleteSmmReadyToLock().
**/
VOID
EFIAPI
SmmSupovrStateLock (
  VOID
  );

/**
  This function sets and locks SMM_FEATURE_CONTROL_UBOX_DFX_REG CSR.
**/
VOID
SmmCodeCheckEnableAndLock (
  VOID
  );

#endif
