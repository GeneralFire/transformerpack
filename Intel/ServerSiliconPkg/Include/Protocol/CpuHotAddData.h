/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2010 Intel Corporation. <BR>

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

#ifndef _CPU_HOT_ADD_DATA_H_
#define _CPU_HOT_ADD_DATA_H_

//
// Includes
//
#include <Library/PcdLib.h>
#include <Library/CpuPpmLib.h>

//
// Cpu hot add data Protocol GUID
//
#define EFI_CPU_HOT_ADD_DATA_PROTOCOL_GUID \
{0x330be755, 0xfbed, 0x4f18, 0xb9, 0xa8, 0x49, 0x58, 0x56, 0xd3, 0xd7, 0xa1}

//
// Extern the GUID for protocol users.
//
extern EFI_GUID gEfiCpuHotAddDataProtocolGuid;

#define SBSP_MSR_NUM    4

typedef struct {
  //
  // Placeholder for MP intialization data
  //

  //
  // Data required for SMBASE relocation
  //
  UINT64    ApicId[FixedPcdGet32 (PcdCpuMaxLogicalProcessorNumber)];
  UINTN     SmBase[FixedPcdGet32 (PcdCpuMaxLogicalProcessorNumber)];
  UINT32    IEDBase;
  UINT32    SmrrBase;
  UINT32    SmrrSize;

} CPU_SMM_INIT_DATA;



#pragma pack(1)

typedef struct {
  UINT32                    Number;
  UINT32                    BIST;
} BIST_INFO;

typedef struct {
  UINT32                    MsrOffset;
  UINT64                    MsrValue;
} MSR_INFO;

typedef struct {
  UINTN             Lock;
  VOID              *StackStart;
  UINTN             StackSize;
  VOID              *ApFunction;
  UINT32            InitFlag;
  UINT32            SignalFlag;
  UINT32            PModeSignalFlag;
  UINT32            NumCpuCheckedIn;
  BIST_INFO         BistBuffer[FixedPcdGet32 (PcdCpuMaxLogicalProcessorNumber)];    // We'll be handling only one CPU at a time during hot add. So 256 should be enough to cover all the APIC IDs
  CPU_SMM_INIT_DATA CpuSmmInitData;
  MSR_INFO          SbspMsrInfo[SBSP_MSR_NUM];
} HOT_ADD_CPU_EXCHANGE_INFO;

typedef
VOID
(EFIAPI *EFI_CPU_SMM_RELOCATE) (
);

typedef struct {
  EFI_PHYSICAL_ADDRESS        StartupVector;
  HOT_ADD_CPU_EXCHANGE_INFO   *MpExchangeInfoPtr;
  EFI_CPU_SMM_RELOCATE        PackageSmmRelocate;
  EFI_CPU_PM_STRUCT           *CpuPmStruct;
  PPM_POLICY_CONFIGURATION    *CpuPmPolicy;
  EFI_PHYSICAL_ADDRESS        SmmStartupCodeArea;
} CPU_HOT_ADD_DATA;

#pragma pack()

#endif //_CPU_HOT_ADD_DATA_H_
