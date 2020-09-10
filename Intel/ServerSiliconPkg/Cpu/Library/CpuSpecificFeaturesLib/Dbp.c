/** @file
  Code for DBP-F feature.

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
#include <Library/PreSiliconEnvDetectLib.h>

/**
  Detects if DBP-F feature supported on current processor.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().

  @retval TRUE     DBP-F feature is supported.
  @retval FALSE    DBP-F feature is not supported.

  @note This service could be called by BSP/APs.
**/
BOOLEAN
EFIAPI
DbpSupport (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData  OPTIONAL
  )
{
  if (FeaturePcdGet (PcdCpuSkylakeFamilyFlag) && IS_SKYLAKE_SERVER_PROC (CpuInfo->DisplayFamily, CpuInfo->DisplayModel) &&
      (CpuInfo->SteppingId >= CLX_B0_CPU_STEP))  {
    return TRUE;
  } else if (!IsSimicsEnvironment() &&
             ((FeaturePcdGet (PcdCpuIcelakeFamilyFlag) && IS_ICELAKE_SERVER_PROC (CpuInfo->DisplayFamily, CpuInfo->DisplayModel)) ||
              (FeaturePcdGet (PcdCpuSapphireRapidsFamilyFlag) && IS_SAPPHIRERAPIDS_SERVER_PROC (CpuInfo->DisplayFamily, CpuInfo->DisplayModel)))) {
    return TRUE;
  }

  return FALSE;
}

/**
  Initializes DBP-F feature to specific state.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().
  @param[in]  State            If TRUE, then the DBP-F feature must be enabled.
                               If FALSE, then the DBP-F feature must be disabled.

  @retval RETURN_SUCCESS       DBP-F feature is initialized.

  @note This service could be called by BSP only.
**/
RETURN_STATUS
EFIAPI
DbpInitialize (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData,  OPTIONAL
  IN BOOLEAN                           State
  )
{
  UINT64                            ValueMask;
  ICX_MSR_FEATURE_TUNING_1_REGISTER IcxFeatureTuningValueMask;

  if (!State) {
    if (FeaturePcdGet (PcdCpuSkylakeFamilyFlag) && IS_SKYLAKE_SERVER_PROC (CpuInfo->DisplayFamily, CpuInfo->DisplayModel)) {
      //
      // Disable
      // Bit 5: ML4_CR_DBP_CONFIG.ENABLE_DBP_FOR_F = 0
      // Bit 6: ML2_CR_ML_CACHE_CONTROL.BI_2IFU_4_F_VICTIMS_EN = 0
      //
      if (CpuInfo->SteppingId >= CPX_A0_CPU_STEP) {
        //
        // Read-modify-write the MSR to preserve other bits values
        //
        ValueMask = (B_ENABLE_DBP_FOR_F | B_BI_2IFU_4_F_VICTIMS_EN);
      } else {
        //
        // Override the whole MSR value to prevent GP Fault
        //
        ValueMask = MAX_UINT64;
      }

      CpuRegisterTableWrite (
        ProcessorNumber,
        Msr,
        MSR_VIRTUAL_MSR_LLC_PREFETCH,
        ValueMask,
        0
        );
    } else if ((FeaturePcdGet (PcdCpuIcelakeFamilyFlag) && IS_ICELAKE_SERVER_PROC (CpuInfo->DisplayFamily, CpuInfo->DisplayModel)) ||
               (FeaturePcdGet (PcdCpuSapphireRapidsFamilyFlag) && IS_SAPPHIRERAPIDS_SERVER_PROC (CpuInfo->DisplayFamily, CpuInfo->DisplayModel))) {
      //
      // Disable
      // Bit 2: Bi2ifu4FVictimsEn = 0
      // Bit 3: Enabledbpforf = 0
      //
      IcxFeatureTuningValueMask.Uint64 = 0;
      IcxFeatureTuningValueMask.Bits.Bi2ifu4FVictimsEn = 1;
      IcxFeatureTuningValueMask.Bits.Enabledbpforf = 1;
      CpuRegisterTableWrite (
        ProcessorNumber,
        Msr,
        ICX_MSR_FEATURE_TUNING_1,
        IcxFeatureTuningValueMask.Uint64,
        0
        );
    }
  }

  return RETURN_SUCCESS;
}
