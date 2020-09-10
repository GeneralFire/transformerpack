/** @file
  Code for AcExceptionOnSplitLock feature.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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
  Detects if AcExceptionOnSplitLock feature supported on current processor.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().

  @retval TRUE     AcExceptionOnSplitLock feature is supported.
  @retval FALSE    AcExceptionOnSplitLock feature is not supported.

  @note This service could be called by BSP/APs.
**/
BOOLEAN
EFIAPI
AcExceptionOnSplitLockSupport (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData  OPTIONAL
  )
{
  if (FeaturePcdGet (PcdCpuIcelakeFamilyFlag) && IS_ICELAKE_D_SERVER_PROC (CpuInfo->DisplayFamily, CpuInfo->DisplayModel)) {
    //
    // Only support on ICX-D.
    //
    return TRUE;
  }

  return FALSE;
}

/**
  Initializes AcExceptionOnSplitLock feature to specific state.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().
  @param[in]  State            If TRUE, then the AcExceptionOnSplitLock feature
                               must be enabled.
                               If FALSE, then the AcExceptionOnSplitLock feature
                               must be disabled.

  @retval RETURN_SUCCESS       AcExceptionOnSplitLock feature is initialized.

  @note This service could be called by BSP/APs.
**/
RETURN_STATUS
EFIAPI
AcExceptionOnSplitLockInitialize (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData,  OPTIONAL
  IN BOOLEAN                           State
  )
{
  //
  // Core scope
  //
  if (CpuInfo->ProcessorInfo.Location.Thread == 0) {
    if (State) {
      CPU_REGISTER_TABLE_WRITE_FIELD (
        ProcessorNumber,
        Msr,
        ICX_MSR_MEMORY_CONTROL,
        ICX_MSR_MEMORY_CONTROL_REGISTER,
        Bits.SplitLockDisable,
        1
        );
    }
  }

  return RETURN_SUCCESS;
}
