/** @file
  Code for ThermalManagement feature.

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

typedef struct  {
  ICX_MSR_PLATFORM_INFO_REGISTER        PlatformInfo;
  ICX_MSR_TEMPERATURE_TARGET_REGISTER   TemperatureTarget;
} THERMAL_MONITOR_CONFIG_DATA;

/**
  Prepares for the data used by CPU feature detection and initialization.

  @param[in]  NumberOfProcessors  The number of CPUs in the platform.

  @return  Pointer to a buffer of CPU related configuration data.

  @note This service could be called by BSP only.
**/
VOID *
EFIAPI
ThermalMonitorGetConfigData (
  IN UINTN               NumberOfProcessors
  )
{
  VOID          *ConfigData;

  ConfigData = AllocateZeroPool (sizeof (THERMAL_MONITOR_CONFIG_DATA) * NumberOfProcessors);
  ASSERT (ConfigData != NULL);
  return ConfigData;
}

/**
  Detects if ThermalMonitor feature supported on current processor.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().

  @retval TRUE     ThermalMonitor feature is supported.
  @retval FALSE    ThermalMonitor feature is not supported.

  @note This service could be called by BSP/APs.
**/
BOOLEAN
EFIAPI
ThermalMonitorSupport (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData  OPTIONAL
  )
{
  THERMAL_MONITOR_CONFIG_DATA   *ThermalMonitorConfigData;

  if ((CpuInfo->CpuIdVersionInfoEdx.Bits.TM == 1) || (CpuInfo->CpuIdVersionInfoEcx.Bits.TM2 == 1)) {
    if (CpuGenerationSupportCheck (ProcessorNumber, CpuInfo, ConfigData)) {
      ThermalMonitorConfigData = (THERMAL_MONITOR_CONFIG_DATA *) ConfigData;
      ASSERT (ThermalMonitorConfigData != NULL);
      if (ThermalMonitorConfigData == NULL) {
        return FALSE;
      }
      ThermalMonitorConfigData[ProcessorNumber].PlatformInfo.Uint64 = AsmReadMsr64 (ICX_MSR_PLATFORM_INFO);
      ThermalMonitorConfigData[ProcessorNumber].TemperatureTarget.Uint64 = AsmReadMsr64 (ICX_MSR_TEMPERATURE_TARGET);
    }
    return TRUE;
  }

  return FALSE;
}

/**
  Initializes ThermalMonitor feature to specific state.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().
  @param[in]  State            If TRUE, then the ThermalMonitor feature
                               must be enabled.
                               If FALSE, then the ThermalMonitor feature
                               must be disabled.

  @retval RETURN_SUCCESS            ThermalMonitor feature is initialized.
  @retval RETURN_OUT_OF_RESOURCES   ConfigData is NULL.

  @note This service could be called by BSP only.
**/
RETURN_STATUS
EFIAPI
ThermalMonitorInitialize (
  IN UINTN            ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID             *ConfigData,  OPTIONAL
  IN BOOLEAN          State
  )
{
  THERMAL_MONITOR_CONFIG_DATA           *ThermalMonitorConfigData;
  ICX_MSR_TEMPERATURE_TARGET_REGISTER   *TemperatureTarget;
  SKX_MSR_TEMPERATURE_TARGET_REGISTER   *SkxTemperatureTarget;
  ICX_MSR_PLATFORM_INFO_REGISTER        *PlatformInfo;
  UINT8                                 TjMaxTccOffset;

  if (State) {
    if (CpuGenerationSupportCheck (ProcessorNumber, CpuInfo, ConfigData)) {
      ThermalMonitorConfigData = (THERMAL_MONITOR_CONFIG_DATA *) ConfigData;
      ASSERT (ThermalMonitorConfigData != NULL);
      if (ThermalMonitorConfigData == NULL) {
        return RETURN_OUT_OF_RESOURCES;
      }

      //
      // Enable Automatic Thermal Control Circuit.
      //
      if (FeaturePcdGet (PcdCpuSnowridgeFamilyFlag) && IS_SNOWRIDGE_SERVER_PROC (CpuInfo->DisplayFamily, CpuInfo->DisplayModel)) {
        //
        // The bits of MSR_IA32_MISC_ENABLE have different scopes.
        // Use PackageDepType semaphore to make sure package scope bit
        // programmed synchronously for all processors in same package.
        //
        CpuRegisterTableWrite (ProcessorNumber, Semaphore, 0, 0, PackageDepType);
        if (IsPackageFirstProcessor (ProcessorNumber, CpuInfo)) {
          //
          // MSR_IA32_MISC_ENABLE AutomaticThermalControlCircuit bit is package scope for SNR.
          //
          CPU_REGISTER_TABLE_WRITE_FIELD (
            ProcessorNumber,
            Msr,
            MSR_IA32_MISC_ENABLE,
            MSR_IA32_MISC_ENABLE_REGISTER,
            Bits.AutomaticThermalControlCircuit,
            1
            );
        }
        //
        // The bits of MSR_IA32_MISC_ENABLE have different scopes.
        // Use PackageDepType semaphore to make sure package scope bit
        // programmed synchronously for all processors in same package.
        //
        CpuRegisterTableWrite (ProcessorNumber, Semaphore, 0, 0, PackageDepType);
      } else {
        CPU_REGISTER_TABLE_WRITE_FIELD (
          ProcessorNumber,
          Msr,
          MSR_IA32_MISC_ENABLE,
          MSR_IA32_MISC_ENABLE_REGISTER,
          Bits.AutomaticThermalControlCircuit,
          1
          );
      }

      //
      // The scope of the 0x1FC, 0x1AA and 0x1A2 MSRs is package level, only program them once for each package.
      //
      if (IsPackageFirstProcessor (ProcessorNumber, CpuInfo)) {
        //
        // Enable Bi-Directional PROCHOT#.
        //
        CPU_REGISTER_TABLE_WRITE_FIELD (
          ProcessorNumber,
          Msr,
          ICX_MSR_POWER_CTL1,
          ICX_MSR_POWER_CTL1_REGISTER,
          Bits.EnableBidirProchot,
          1
          );
        //
        // Lock Thermal interrupts.
        //
        CPU_REGISTER_TABLE_WRITE_FIELD (
          ProcessorNumber,
          Msr,
          MSR_MISC_PWR_MGMT,
          MSR_MISC_PWR_MGMT_REGISTER,
          Bits.LockThermInt,
          1
          );

        PlatformInfo = &ThermalMonitorConfigData[ProcessorNumber].PlatformInfo;
        TemperatureTarget = &ThermalMonitorConfigData[ProcessorNumber].TemperatureTarget;

        //
        // Program TjMaxTccOffset.
        //
        if (PlatformInfo->Bits.PrgTjOffsetEn) {
          //
          // Compare with the RefTemp field (FUSE_TJ_MAX_OFFSET).
          //
          if (mCpuSpecificFeaturesCpuPolicy->CpuTccActivationOffset > TemperatureTarget->Bits.RefTemp) {
            TjMaxTccOffset = (UINT8) TemperatureTarget->Bits.RefTemp;
          } else {
            TjMaxTccOffset = mCpuSpecificFeaturesCpuPolicy->CpuTccActivationOffset;
          }

          if (FeaturePcdGet (PcdCpuSkylakeFamilyFlag) && IS_SKYLAKE_SERVER_PROC (CpuInfo->DisplayFamily, CpuInfo->DisplayModel)) {
            //
            // The TjMaxTccOffset field bitwidth for SKX is different with others.
            //
            SkxTemperatureTarget = (SKX_MSR_TEMPERATURE_TARGET_REGISTER *) TemperatureTarget;
            SkxTemperatureTarget->Bits.TjMaxTccOffset = TjMaxTccOffset;
          } else {
            TemperatureTarget->Bits.TjMaxTccOffset = TjMaxTccOffset;
          }

          CPU_REGISTER_TABLE_WRITE64 (
            ProcessorNumber,
            Msr,
            ICX_MSR_TEMPERATURE_TARGET,
            TemperatureTarget->Uint64
            );
        }
      }
    }
  }

  return RETURN_SUCCESS;
}
