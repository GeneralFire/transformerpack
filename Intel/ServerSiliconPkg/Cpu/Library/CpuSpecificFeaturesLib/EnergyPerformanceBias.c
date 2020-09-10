/** @file
  Code for Energy Performance Bias features.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

/**
  Initializes Energy Performance Bias feature to specific state.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().
  @param[in]  State            If TRUE, then the Energy Performance Bias feature
                               must be enabled.
                               If FALSE, then the Energy Performance Bias feature
                               must be disabled.

  @retval RETURN_SUCCESS       Energy Performance Bias feature is initialized.

  @note This service could be called by BSP only.
**/
RETURN_STATUS
EFIAPI
EnergyPerformanceBiasInitialize (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData,  OPTIONAL
  IN BOOLEAN                           State
  )
{
  if (IsPackageFirstProcessor (ProcessorNumber, CpuInfo)) {
    //
    // ICX_MSR_POWER_CTL1 is package scope.
    //
    CPU_REGISTER_TABLE_WRITE_FIELD (
      ProcessorNumber,
      Msr,
      ICX_MSR_POWER_CTL1,
      ICX_MSR_POWER_CTL1_REGISTER,
      Bits.PwrPerfPltfrmOvr,
      (State) ? 1 : 0
      );
  }
  if (State) {
    //
    // Use PackageDepType semaphore to make sure package scope bit
    // PwrPerfPltfrmOvr programmed for all processors in same package
    // before programming thread scope bit PowerPolicyPreference.
    //
    CpuRegisterTableWrite (ProcessorNumber, Semaphore, 0, 0, PackageDepType);
    CPU_REGISTER_TABLE_WRITE_FIELD (
      ProcessorNumber,
      Msr,
      MSR_IA32_ENERGY_PERF_BIAS,
      MSR_IA32_ENERGY_PERF_BIAS_REGISTER,
      Bits.PowerPolicyPreference,
      PcdGet8 (PcdCpuEnergyPolicy)
      );
  }
  return RETURN_SUCCESS;
}
