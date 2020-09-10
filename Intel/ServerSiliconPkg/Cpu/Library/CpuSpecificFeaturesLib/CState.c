/** @file
  Code for C-State feature.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include "CpuSpecificFeatures.h"

#define CST_C0          00
#define CST_C1          01
#define CST_C3          02
#define CST_C6          03
#define CST_C7          04
#define CST_NOLIMIT     07

typedef struct  {
  MSR_CLOCK_CST_CONFIG_CONTROL_REGISTER CstConfigControl;
  MSR_PMG_IO_CAPTURE_BASE_REGISTER      PmgIoCaptureBase;
  UINT8                                 MaxCState;
} CSTATE_CONFIG_DATA;

/**
  Return maximum C-State.

  @return Maximum C-State.
**/
UINT8
GetMaxCState (
  VOID
  )
{
  CPUID_MONITOR_MWAIT_EDX  Edx;
  UINT8                    CStateValue;
  UINT8                    Index;
  UINT32                   Params;

  CStateValue = 0;
  AsmCpuid (CPUID_MONITOR_MWAIT, NULL, NULL, NULL, &Edx.Uint32);
  //
  // Calculate MAX C state.
  //
  CStateValue = 0;
  for (Index = 7; Index > 0; Index--) {
    Params = Edx.Uint32 >> (Index * 4);
    if((Params & 0xF) != 0) {
      CStateValue = Index;
      break;
    }
  }

  return CStateValue;
}

/**
  Prepares for the data used by CPU feature detection and initialization.

  @param[in]  NumberOfProcessors  The number of CPUs in the platform.

  @return  Pointer to a buffer of CPU related configuration data.

  @note This service could be called by BSP only.
**/
VOID *
EFIAPI
CStateGetConfigData (
  IN UINTN  NumberOfProcessors
  )
{
  VOID      *CStateConfigData;

  CStateConfigData = AllocateZeroPool (sizeof (CSTATE_CONFIG_DATA) * NumberOfProcessors);
  ASSERT (CStateConfigData != NULL);
  return CStateConfigData;
}

/**
  Detects if C-State feature supported on current processor.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().

  @retval TRUE     C-State feature is supported.
  @retval FALSE    C-State feature is not supported.

  @note This service could be called by BSP/APs.
**/
BOOLEAN
EFIAPI
CStateSupport (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData  OPTIONAL
  )
{
  CSTATE_CONFIG_DATA  *CStateConfigData;
  UINT8               MaxCState;

  //
  // C-State feature has dependency on Monitor/Mwait capability.
  //
  if ((CpuInfo->CpuIdVersionInfoEcx.Bits.MONITOR == 0) ||
      !mCpuSpecificFeaturesCpuPolicy->CpuMonitorMwaitEnable) {
    return FALSE;
  }

  MaxCState = GetMaxCState ();
  if (MaxCState > 1) {
    CStateConfigData = (CSTATE_CONFIG_DATA *) ConfigData;
    ASSERT (CStateConfigData != NULL);
    if (CStateConfigData == NULL) {
      return FALSE;
    }
    if (MaxCState < mCpuSpecificFeaturesCpuPolicy->CpuCoreCStateValue) {
      CStateConfigData[ProcessorNumber].MaxCState = MaxCState;
    } else {
      CStateConfigData[ProcessorNumber].MaxCState = mCpuSpecificFeaturesCpuPolicy->CpuCoreCStateValue;
    }
    CStateConfigData[ProcessorNumber].CstConfigControl.Uint64 = AsmReadMsr64 (MSR_CLOCK_CST_CONFIG_CONTROL);
    CStateConfigData[ProcessorNumber].PmgIoCaptureBase.Uint64 = AsmReadMsr64 (MSR_PMG_IO_CAPTURE_BASE);
    return TRUE;
  }
  return FALSE;
}

/**
  Initializes C-State feature to specific state.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().
  @param[in]  State            If TRUE, then the C-State feature
                               must be enabled.
                               If FALSE, then the C-State feature
                               must be disabled.

  @retval RETURN_SUCCESS            C-State feature is initialized.
  @retval RETURN_OUT_OF_RESOURCES   ConfigData is NULL.

  @note This service could be called by BSP only.
**/
RETURN_STATUS
EFIAPI
CStateInitialize (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData,  OPTIONAL
  IN BOOLEAN                           State
  )
{
  CSTATE_CONFIG_DATA                            *CStateConfigData;
  MSR_CLOCK_CST_CONFIG_CONTROL_REGISTER         *CstConfigControl;
  MSR_PMG_IO_CAPTURE_BASE_REGISTER              *PmgIoCaptureBase;
  UINT8                                         MaxCState;

  //
  // The scope of the 0xE2 and 0xE4 MSRs is core level, only program them for thread 0 in each core.
  //
  if (CpuInfo->ProcessorInfo.Location.Thread != 0) {
    return RETURN_SUCCESS;
  }

  if (State) {
    CStateConfigData = (CSTATE_CONFIG_DATA *) ConfigData;
    ASSERT (CStateConfigData != NULL);
    if (CStateConfigData == NULL) {
      return RETURN_OUT_OF_RESOURCES;
    }
    CstConfigControl = &CStateConfigData[ProcessorNumber].CstConfigControl;
    MaxCState = CStateConfigData[ProcessorNumber].MaxCState;
    PmgIoCaptureBase = &CStateConfigData[ProcessorNumber].PmgIoCaptureBase;

    //
    // For Native MWAIT and IO Redirection Support
    // Enable IO MWAIT translation for processor core C-state control.
    // Also limit the package to the max supported C-state.
    //
    CstConfigControl->Bits.MaxPkgCState = mCpuSpecificFeaturesCpuPolicy->CpuPackageCStateLimit;
    CstConfigControl->Bits.IoMwaitRedirection = 1;

    if (mCpuSpecificFeaturesCpuPolicy->CpuC1AutoDemotionEnable) {
      CstConfigControl->Bits.C1StateAutoDemotionEnable = 1;
    }
    if (mCpuSpecificFeaturesCpuPolicy->CpuC1AutoUndemotionEnable) {
      CstConfigControl->Bits.Enc1undemotion = 1;
    }

    CPU_REGISTER_TABLE_WRITE64 (
      ProcessorNumber,
      Msr,
      MSR_CLOCK_CST_CONFIG_CONTROL,
      CstConfigControl->Uint64
      );

    //
    // For IO Redirection Support
    // Tell processor that 2 C-states (besides C1) are supported (C2 and C3).
    // These correspond to LVL_2 and LVL_3.  Setup LVL_2 base address.
    //
    // Enable the MAX C-State as we can, use _CST to report the correct setup in processor setup page
    //
    if((MaxCState == CST_C3)|| (MaxCState == CST_C6) ||(MaxCState == CST_C7)) {
      MaxCState -= 2;
    }
    PmgIoCaptureBase->Bits.CstRange = MaxCState;
    PmgIoCaptureBase->Bits.Lvl2b = mCpuSpecificFeaturesCpuPolicy->CpuAcpiLvl2Addr;
    CPU_REGISTER_TABLE_WRITE64 (
      ProcessorNumber,
      Msr,
      MSR_PMG_IO_CAPTURE_BASE,
      PmgIoCaptureBase->Uint64
      );
  }
  return RETURN_SUCCESS;
}
