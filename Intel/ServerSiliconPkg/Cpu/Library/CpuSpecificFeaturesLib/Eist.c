/** @file
  Code for Enhanced Intel Speedstep Technology feature.

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
#include <Library/CpuEarlyDataLib.h>

typedef struct {
  CPUID_THERMAL_POWER_MANAGEMENT_EAX    ThermalPowerManagementEax;
  MSR_MISC_ENABLES_REGISTER             MiscEnablesMsr;
  MSR_PERF_CTL_REGISTER                 PerfCtlMsr;
} EIST_CONFIG_DATA;

/**
  Set boot PState.

  @param[in] ProcessorNumber    The index of the CPU executing this function.
  @param[in] CpuInfo            A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                                structure for the CPU executing this function.
  @param[in] State              If TRUE, then the EIST feature is to be enabled.
                                If FALSE, then the EIST feature is to be disabled.
  @param[in] PerfCtlMsr         Pointer to PerfCtl MSR value.

  @note This service could be called by BSP only.
**/
VOID
SetBootPState (
  IN UINTN                            ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION *CpuInfo,
  IN BOOLEAN                          State,
  IN MSR_PERF_CTL_REGISTER            *PerfCtlMsr
  )
{
  CPU_VAR_DATA      *CpuVarDataPtr;
  UINT8             SocketId;
  UINT8             MinRatio;
  UINT8             MaxRatio;
  BOOLEAN           EfficientBoot;
  UINTN             PReq;

  CpuVarDataPtr = GetCpuVarData ();
  SocketId = (UINT8) CpuInfo->ProcessorInfo.Location.Package;
  MinRatio = CpuVarDataPtr->MinCoreToBusRatio[SocketId];
  MaxRatio = CpuVarDataPtr->IssConfigTdpRatio[SocketId][CpuVarDataPtr->IssConfigTdpCurrentLevel];

  //
  // Before setting BootPState, enable EIST if it is at disabled state.
  //
  if (!State) {
    //
    // The bits of MSR_MISC_ENABLES have different scopes.
    // Use PackageDepType semaphore to make sure package scope bit
    // programmed synchronously for all processors in same package.
    //
    CpuRegisterTableWrite (ProcessorNumber, Semaphore, 0, 0, PackageDepType);
    if (IsPackageFirstProcessor (ProcessorNumber, CpuInfo)) {
      //
      // MSR_MISC_ENABLES EnableGv3 bit is package scope.
      //
      CPU_REGISTER_TABLE_WRITE_FIELD (
        ProcessorNumber,
        Msr,
        MSR_MISC_ENABLES,
        MSR_MISC_ENABLES_REGISTER,
        Bits.EnableGv3,
        1
        );
    }
    //
    // The bits of MSR_MISC_ENABLES have different scopes.
    // Use PackageDepType semaphore to make sure package scope bit
    // programmed synchronously for all processors in same package.
    //
    CpuRegisterTableWrite (ProcessorNumber, Semaphore, 0, 0, PackageDepType);
  }

  //
  // Set BootPState based on policy.
  //

  EfficientBoot = FALSE;

  if (State) {
    //
    // The processor is equipped with EIST feature, and the user wish to enable it.
    //
    if (mCpuSpecificFeaturesCpuPolicy->CpuBootPState == 0) {
      //
      // Max performance
      //
      EfficientBoot = FALSE;
    } else if (mCpuSpecificFeaturesCpuPolicy->CpuBootPState == 1) {
      //
      // Max efficient
      //
      EfficientBoot = TRUE;
    }

#if ME_SPS_SUPPORT
  if (mCpuSpecificFeaturesCpuPolicy->CpuBootPState == 2) {
    //
    // Set by Intel Node Manager
    //
    if (PerfCtlMsr->Bits.PReq == MinRatio) {
      EfficientBoot = TRUE;
      DEBUG ((DEBUG_INFO, "NM FW has configured Efficient Boot frequency.\n"));
    } else {
      EfficientBoot = FALSE;
      DEBUG ((DEBUG_INFO, "NM FW has configured Performance Boot frequency.\n"));
    }
  }
#endif
  } else {
    //
    // The processor is equipped with EIST feature, but the user wish to disable it.
    // In this situation BIOS should make the processors run at max none turbo freq.
    //
    EfficientBoot = FALSE;
  }

  if (EfficientBoot) {
    //
    // Max efficient (min Ratio)
    //
    PReq = MinRatio;
  } else {
    //
    // Max performance (max ratio)
    //
    PReq = MaxRatio;
  }

  //
  // Set P-State Request
  //
  CPU_REGISTER_TABLE_WRITE_FIELD (
    ProcessorNumber,
    Msr,
    MSR_PERF_CTL,
    MSR_PERF_CTL_REGISTER,
    Bits.PReq,
    PReq
    );

  //
  // After setting BootPState, disable EIST if it was enabled above.
  //
  if (!State) {
    //
    // The bits of MSR_MISC_ENABLES have different scopes.
    // Use PackageDepType semaphore to make sure package scope bit
    // programmed synchronously for all processors in same package.
    //
    CpuRegisterTableWrite (ProcessorNumber, Semaphore, 0, 0, PackageDepType);
    if (IsPackageFirstProcessor (ProcessorNumber, CpuInfo)) {
      //
      // MSR_MISC_ENABLES EnableGv3 bit is package scope.
      //
      CPU_REGISTER_TABLE_WRITE_FIELD (
        ProcessorNumber,
        Msr,
        MSR_MISC_ENABLES,
        MSR_MISC_ENABLES_REGISTER,
        Bits.EnableGv3,
        0
        );
    }
    //
    // The bits of MSR_MISC_ENABLES have different scopes.
    // Use PackageDepType semaphore to make sure package scope bit
    // programmed synchronously for all processors in same package.
    //
    CpuRegisterTableWrite (ProcessorNumber, Semaphore, 0, 0, PackageDepType);
  }
}

/**
  Prepares for the data used by CPU feature detection and initialization.

  @param[in]  NumberOfProcessors  The number of CPUs in the platform.

  @return  Pointer to a buffer of CPU related configuration data.

  @note This service could be called by BSP only.
**/
VOID *
EFIAPI
SpecificEistGetConfigData (
  IN UINTN               NumberOfProcessors
  )
{
  EIST_CONFIG_DATA  *EistConfigData;

  EistConfigData = AllocateZeroPool (sizeof (EIST_CONFIG_DATA) * NumberOfProcessors);
  ASSERT (EistConfigData != NULL);

  return EistConfigData;
}

/**
  Detects if EIST feature supported on current processor.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().

  @retval TRUE     EIST feature is supported.
  @retval FALSE    EIST feature is not supported.

  @note This service could be called by BSP/APs.
**/
BOOLEAN
EFIAPI
SpecificEistSupport (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData  OPTIONAL
  )
{
  EIST_CONFIG_DATA    *EistConfigData;

  if (CpuInfo->CpuIdVersionInfoEcx.Bits.EIST == 1) {
    EistConfigData = (EIST_CONFIG_DATA *) ConfigData;
    ASSERT (EistConfigData != NULL);
    if (EistConfigData == NULL) {
      return FALSE;
    }

    AsmCpuid (
      CPUID_THERMAL_POWER_MANAGEMENT,
      &EistConfigData[ProcessorNumber].ThermalPowerManagementEax.Uint32,
      NULL,
      NULL,
      NULL
      );
    EistConfigData[ProcessorNumber].MiscEnablesMsr.Uint64 = AsmReadMsr64 (MSR_MISC_ENABLES);
    EistConfigData[ProcessorNumber].PerfCtlMsr.Uint64 = AsmReadMsr64 (MSR_PERF_CTL);
    return TRUE;
  }

  return FALSE;
}

/**
  Initializes EIST feature to specific state.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().
  @param[in]  State            If TRUE, then the EIST feature
                               must be enabled.
                               If FALSE, then the EIST feature
                               must be disabled.

  @retval RETURN_SUCCESS            EIST feature is initialized.
  @retval RETURN_OUT_OF_RESOURCES   ConfigData is NULL.

  @note This service could be called by BSP only.
**/
RETURN_STATUS
EFIAPI
SpecificEistInitialize (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData,  OPTIONAL
  IN BOOLEAN                           State
  )
{
  EIST_CONFIG_DATA                      *EistConfigData;
  CPUID_THERMAL_POWER_MANAGEMENT_EAX    *ThermalPowerManagementEax;
  MSR_MISC_ENABLES_REGISTER             *MiscEnablesMsr;
  MSR_PERF_CTL_REGISTER                 *PerfCtlMsr;
  BOOLEAN                               TurboSupported;

  EistConfigData = (EIST_CONFIG_DATA *) ConfigData;
  ASSERT (EistConfigData != NULL);
  if (EistConfigData == NULL) {
    return RETURN_OUT_OF_RESOURCES;
  }
  ThermalPowerManagementEax = &EistConfigData[ProcessorNumber].ThermalPowerManagementEax;
  MiscEnablesMsr = &EistConfigData[ProcessorNumber].MiscEnablesMsr;
  PerfCtlMsr = &EistConfigData[ProcessorNumber].PerfCtlMsr;

  //
  // The bits of MSR_MISC_ENABLES have different scopes.
  // Use PackageDepType semaphore to make sure package scope bit
  // programmed synchronously for all processors in same package.
  //
  CpuRegisterTableWrite (ProcessorNumber, Semaphore, 0, 0, PackageDepType);
  if (IsPackageFirstProcessor (ProcessorNumber, CpuInfo)) {
    //
    // MSR_MISC_ENABLES EnableGv3 bit is package scope.
    //
    CPU_REGISTER_TABLE_WRITE_FIELD (
      ProcessorNumber,
      Msr,
      MSR_MISC_ENABLES,
      MSR_MISC_ENABLES_REGISTER,
      Bits.EnableGv3,
      (State) ? 1 : 0
      );
  }
  //
  // The bits of MSR_MISC_ENABLES have different scopes.
  // Use PackageDepType semaphore to make sure package scope bit
  // programmed synchronously for all processors in same package.
  //
  CpuRegisterTableWrite (ProcessorNumber, Semaphore, 0, 0, PackageDepType);

  //
  //  +----------------------------------------------------------------------------+
  //  | CPUID.(EAX=06):EAX[1] | MSR_MISC_ENABLES.Bits[38]    | Turbo Mode          |
  //  +-----------------------|------------------------------|---------------------+
  //  |          0            |       0                      |Not Available        |
  //  +-----------------------|------------------------------|---------------------+
  //  |          0            |       1                      |Available but hidden |
  //  +-----------------------|------------------------------|---------------------+
  //  |          1            |       0                      |Available and visible|
  //  +-----------------------|------------------------------|---------------------+
  //
  TurboSupported = (BOOLEAN)((ThermalPowerManagementEax->Bits.TurboBoostTechnology != 0) ||
                             (MiscEnablesMsr->Bits.TurboModeDisable != 0));
  if (TurboSupported) {
    //
    // The bits of MSR_MISC_ENABLES have different scopes.
    // Use PackageDepType semaphore to make sure package scope bit
    // programmed synchronously for all processors in same package.
    //
    CpuRegisterTableWrite (ProcessorNumber, Semaphore, 0, 0, PackageDepType);
    if (IsPackageFirstProcessor (ProcessorNumber, CpuInfo)) {
      //
      // MSR_MISC_ENABLES TurboModeDisable bit is package scope.
      //
      CPU_REGISTER_TABLE_WRITE_FIELD (
        ProcessorNumber,
        Msr,
        MSR_MISC_ENABLES,
        MSR_MISC_ENABLES_REGISTER,
        Bits.TurboModeDisable,
        mCpuSpecificFeaturesCpuPolicy->CpuTurboModeEnable ? 0 : 1
        );
    }
    //
    // The bits of MSR_MISC_ENABLES have different scopes.
    // Use PackageDepType semaphore to make sure package scope bit
    // programmed synchronously for all processors in same package.
    //
    CpuRegisterTableWrite (ProcessorNumber, Semaphore, 0, 0, PackageDepType);
  }

  //
  // HW Coordination programming
  //
  if (FeaturePcdGet (PcdCpuSkylakeFamilyFlag) && IS_SKYLAKE_SERVER_PROC (CpuInfo->DisplayFamily, CpuInfo->DisplayModel)) {
    if (IsPackageFirstProcessor (ProcessorNumber, CpuInfo)) {
      CpuRegisterTableWrite (
        ProcessorNumber,
        Msr,
        MSR_MISC_PWR_MGMT,
        B_MSR_EIST_HW_COORDINATION_DISABLE,
        mCpuSpecificFeaturesCpuPolicy->CpuHwCoordinationEnable ? 0 : 1
        );
    }
  }

  //
  // Set BootPState based on policy.
  //
  SetBootPState (
    ProcessorNumber,
    CpuInfo,
    State,
    PerfCtlMsr
    );

  return RETURN_SUCCESS;
}
