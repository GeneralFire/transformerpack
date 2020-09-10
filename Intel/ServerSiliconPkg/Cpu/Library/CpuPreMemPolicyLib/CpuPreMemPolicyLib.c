/** @file
  Implementation of CPU Pre Mem Policy library

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

#include <Base.h>
#include <Library/BaseLib.h>
#include <Uefi.h>
#include <SysHostChipCommon.h>
#include <Pi/PiBootMode.h>
#include <Pi/PiHob.h>
#include <Library/CpuPreMemPolicyLib.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SimulationServicesLib.h>

EFI_GUID mCpuPolicyDataHobGuid = {0x8d1faf2d, 0x8d6, 0x4c05, {0x8b, 0xd3, 0x3b, 0x6c, 0xf, 0xf, 0xff, 0x1a}};

/**
  Create default CPU Pre Mem policy.

  @retval EFI_SUCCESS           Create default CPU Pre Mem policy successfully.
  @retval EFI_OUT_OF_RESOURCES  Out of resource.

**/

EFI_STATUS
CreateDefaultCpuPreMemPolicy (
  VOID
  )
{
  CPU_PRE_MEM_POLICY CpuPolicy;
  //
  // Init CPU feature related setup options to default safe values that ensure minimum boot.
  // A platform hook will be called later to override these values.
  //
  ZeroMem ((VOID*)&CpuPolicy, sizeof(CpuPolicy));

  CpuPolicy.dcuModeSelect                  = 0;                     // CPU DCU Mode select
  CpuPolicy.flexRatioEn                    = 0;                     // 0 = keep HW default ratio
  CpuPolicy.flexRatioNext                  = 0;                     // 0 = keep HW default ratio
  CpuPolicy.smtEnable                      = 1;                     // 0/1 Disable/Enable SMT(HT)
  CpuPolicy.EnableGv                       = 1;                     // 0/1 Disable/Enable GV3
  CpuPolicy.ConfigTdpLevel                 = 0;                     // 0 - 2: 0 = Normal; 1 = Level 1; 2 = Level 2
  CpuPolicy.IssTdpLevel                    = 0;                     // 0, 3 - 4: 0 = Base; 3 = Level 3; 4 = Level 4
  CpuPolicy.DynamicIss                     = 0;                     // 0/1 Disable/Enable Dynamic ISS
  CpuPolicy.CheckCpuBist                   = 1;                     // 0 - ignore BIST result, 1 - will disable BIST fail core
  CpuPolicy.CoreFailover                   = 1;                     // 1 = Enable spare core(s) in place of core(s) that fail BIST
  CpuPolicy.DfxBistFailureEmulation        = 0;                     // 0 = Disable emulation of failed cores to test core failover
  CpuPolicy.AllowMixedPowerOnCpuRatio      = 0;                     // 0 = Force common ratio among CPU sockets 1 = Allow mixed ration
  CpuPolicy.WFRWAEnable                    = 0;
  CpuPolicy.PmaxDisable                    = 0;                     // 0 default for Pmax detector disable
  CpuPolicy.vtEnable                       = 0;                     // 0/1 Disable/Enable VT

  CpuPolicy.debugInterfaceEn               = 0;                     // CPU Debug Interface Enable
  CpuPolicy.CpuPaLimit                     = 0;                     // 0 = keep CPU PA limit disable
  CpuPolicy.EarlyC1eEnable                 = 1;                     // 1 = Enable C1E by default
  CpuPolicy.LlcPrefetchEnable              = 0;
  CpuPolicy.ProcessorMsrLockControl        = 0;
  CpuPolicy.ProcessorMsrPkgCstConfigControlLock = 1;
  CpuPolicy.FadrSupport                    = 0;
  CpuPolicy.TscResetEnable                 = 0;

  return SetCpuPreMemPolicy (&CpuPolicy, sizeof(CpuPolicy));
}

/**
  Set CPU Pre Mem policy.

  @param  CpuPolicy             Pointer to Cpu Pre Mem Policy Info.
  @param  DataSize              The size of Cpu Pre Mem Policy.

  @retval EFI_SUCCESS           Set CPU Pre Mem policy successfully.
  @retval EFI_OUT_OF_RESOURCES  Out of resource.

**/
EFI_STATUS
EFIAPI
SetCpuPreMemPolicy (
  IN VOID   *CpuPolicy,
  IN UINTN  DataSize
  )
{
  VOID *CpuPolicyGuidHob;
  VOID *GuidData;
  UINTN GuidDataSize;
  CpuPolicyGuidHob = GetFirstGuidHob (&mCpuPolicyDataHobGuid);
  if (CpuPolicyGuidHob == NULL) {
    GuidData = BuildGuidDataHob (&mCpuPolicyDataHobGuid, CpuPolicy, DataSize);
  } else {
    GuidData = GET_GUID_HOB_DATA (CpuPolicyGuidHob);
    GuidDataSize = GET_GUID_HOB_DATA_SIZE (CpuPolicyGuidHob);
    if (GuidDataSize >= DataSize) {
      CopyMem(GuidData, CpuPolicy, DataSize);
    } else {
      return EFI_OUT_OF_RESOURCES;
    }
  }
  return EFI_SUCCESS;
}

/**
  Get CPU Pre Mem policy.

  @retval Pointer to CPU_PRE_MEM_POLICY Get CPU Pre Mem policy successfully.
  @retval NULL                  Can't find the Cpu Pre Mem Policy.

**/
CPU_PRE_MEM_POLICY *
EFIAPI
GetCpuPreMemPolicy (
  )
{
  VOID *CpuPolicyGuidHob;
  VOID *GuidData;
  EFI_STATUS Status;
  CpuPolicyGuidHob = GetFirstGuidHob(&mCpuPolicyDataHobGuid);
  while (CpuPolicyGuidHob == NULL) {
    Status = CreateDefaultCpuPreMemPolicy();
    if (!EFI_ERROR(Status)) {
      CpuPolicyGuidHob = GetFirstGuidHob(&mCpuPolicyDataHobGuid);
    }
  }
  GuidData = GET_GUID_HOB_DATA (CpuPolicyGuidHob);
  return GuidData;
}
