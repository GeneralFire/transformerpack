/** @file
  Code for Prefetcher features.

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
  Detects if L1 Next Page Prefetcher feature supported on current processor.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().

  @retval TRUE     L1 Next Page Prefetcher feature is supported.
  @retval FALSE    L1 Next Page Prefetcher feature is not supported.

  @note This service could be called by BSP/APs.
**/
BOOLEAN
EFIAPI
L1NextPagePrefetcherSupport (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData  OPTIONAL
  )
{
  //
  // Snowridge processor.
  //
  if (FeaturePcdGet (PcdCpuSnowridgeFamilyFlag) && IS_SNOWRIDGE_SERVER_PROC (CpuInfo->DisplayFamily, CpuInfo->DisplayModel)) {
    return TRUE;
  }
  return FALSE;
}

/**
  Initializes L1 Next Page Prefetcher feature to specific state.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().
  @param[in]  State            If TRUE, then the L1 Next Page Prefetcher feature
                               must be enabled.
                               If FALSE, then the L1 Next Page Prefetcher feature
                               must be disabled.

  @retval RETURN_SUCCESS       L1 Next Page Prefetcher feature is initialized.

  @note This service could be called by BSP only.
**/
RETURN_STATUS
EFIAPI
L1NextPagePrefetcherInitialize (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData,  OPTIONAL
  IN BOOLEAN                           State
  )
{
  //
  // Core scope.
  //
  if (CpuInfo->ProcessorInfo.Location.Thread == 0) {
    if (!State) {
      CpuRegisterTableWrite (ProcessorNumber, CacheControl, 0, BIT0, 0); 
      CPU_REGISTER_TABLE_WRITE_FIELD (
        ProcessorNumber,
        Msr,
        SNR_MSR_UCODE_CR_MISC_FEATURE_CONTROL,
        SNR_MSR_UCODE_CR_MISC_FEATURE_CONTROL_REGISTER,
        Bits.DcuNextPagePrefetchDisable,
        1
        );
      CpuRegisterTableWrite (ProcessorNumber, CacheControl, 0, BIT0, 1);
    }
  }
  return RETURN_SUCCESS;
}

/**
  Detects if AMP Prefetch feature supported on current processor.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().

  @retval TRUE     AMP Prefetch feature is supported.
  @retval FALSE    AMP Prefetch feature is not supported.

  @note This service could be called by BSP/APs.
**/
BOOLEAN
EFIAPI
AmpPrefetchSupport (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData  OPTIONAL
  )
{
  //
  // SapphireRapids processor.
  //
  if (FeaturePcdGet (PcdCpuSapphireRapidsFamilyFlag) && IS_SAPPHIRERAPIDS_SERVER_PROC (CpuInfo->DisplayFamily, CpuInfo->DisplayModel)) {
    return TRUE;
  }
  return FALSE;
}

/**
  Initializes AMP Prefetch feature to specific state.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().
  @param[in]  State            If TRUE, then the AMP Prefetch feature
                               must be enabled.
                               If FALSE, then the AMP Prefetch feature
                               must be disabled.

  @retval RETURN_SUCCESS       AMP Prefetch feature is initialized.

  @note This service could be called by BSP only.
**/
RETURN_STATUS
EFIAPI
AmpPrefetchInitialize (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData,  OPTIONAL
  IN BOOLEAN                           State
  )
{
  //
  // Core scope.
  //
  if (CpuInfo->ProcessorInfo.Location.Thread == 0) {
    if (!State) {
      CpuRegisterTableWrite (ProcessorNumber, CacheControl, 0, BIT0, 0); 
      CPU_REGISTER_TABLE_WRITE_FIELD (
        ProcessorNumber,
        Msr,
        SPR_MSR_MISC_FEATURE_CONTROL,
        SPR_MSR_MISC_FEATURE_CONTROL_REGISTER,
        Bits.MlcAmpPrefetchDisable,
        1
        );
      CpuRegisterTableWrite (ProcessorNumber, CacheControl, 0, BIT0, 1);
    }
  }
  return RETURN_SUCCESS;
}

/**
  Initializes Dcu Prefetcher feature to specific state.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().
  @param[in]  State            If TRUE, then the Dcu Prefetcher feature
                               must be enabled.
                               If FALSE, then the Dcu Prefetcher feature
                               must be disabled.

  @retval RETURN_SUCCESS       Dcu Prefetcher feature is initialized.

  @note This service could be called by BSP only.
**/
RETURN_STATUS
EFIAPI
DcuPrefetcherInitialize (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData,  OPTIONAL
  IN BOOLEAN                           State
  )
{
  //
  // Core scope.
  //
  if (CpuInfo->ProcessorInfo.Location.Thread == 0) {
    if (!State) {
      CpuRegisterTableWrite (ProcessorNumber, CacheControl, 0, BIT0, 0); 
      CPU_REGISTER_TABLE_WRITE_FIELD (
        ProcessorNumber,
        Msr,
        ICX_MSR_MISC_FEATURE_CONTROL,
        ICX_MSR_MISC_FEATURE_CONTROL_REGISTER,
        Bits.DcuStreamerPrefetchDisable,
        1
        );
      CpuRegisterTableWrite (ProcessorNumber, CacheControl, 0, BIT0, 1);
    }
  }
  return RETURN_SUCCESS;
}

/**
  Initializes Ip Prefetcher feature to specific state.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().
  @param[in]  State            If TRUE, then the Ip Prefetcher feature
                               must be enabled.
                               If FALSE, then the Ip Prefetcher feature
                               must be disabled.

  @retval RETURN_SUCCESS       Ip Prefetcher feature is initialized.

  @note This service could be called by BSP only.
**/
RETURN_STATUS
EFIAPI
IpPrefetcherInitialize (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData,  OPTIONAL
  IN BOOLEAN                           State
  )
{
  //
  // Core scope.
  //
  if (CpuInfo->ProcessorInfo.Location.Thread == 0) {
    if (!State) {
      CpuRegisterTableWrite (ProcessorNumber, CacheControl, 0, BIT0, 0);
      CPU_REGISTER_TABLE_WRITE_FIELD (
        ProcessorNumber,
        Msr,
        ICX_MSR_MISC_FEATURE_CONTROL,
        ICX_MSR_MISC_FEATURE_CONTROL_REGISTER,
        Bits.DcuIpPrefetchDisable,
        1
        );
      CpuRegisterTableWrite (ProcessorNumber, CacheControl, 0, BIT0, 1);
    }
  }
  return RETURN_SUCCESS;
}

/**
  Initializes Mlc Streamer Prefetcher feature to specific state.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().
  @param[in]  State            If TRUE, then the Mlc Streamer Prefetcher feature
                               must be enabled.
                               If FALSE, then the Mlc Streamer Prefetcher feature
                               must be disabled.

  @retval RETURN_SUCCESS       Mlc Streamer Prefetcher feature is initialized.

  @note This service could be called by BSP only.
**/
RETURN_STATUS
EFIAPI
MlcStreamerPrefetcherInitialize (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData,  OPTIONAL
  IN BOOLEAN                           State
  )
{
  //
  // Core scope.
  //
  if (CpuInfo->ProcessorInfo.Location.Thread == 0) {
    if (!State) {
      CpuRegisterTableWrite (ProcessorNumber, CacheControl, 0, BIT0, 0);
      CPU_REGISTER_TABLE_WRITE_FIELD (
        ProcessorNumber,
        Msr,
        ICX_MSR_MISC_FEATURE_CONTROL,
        ICX_MSR_MISC_FEATURE_CONTROL_REGISTER,
        Bits.MlcStreamerPrefetchDisable,
        1
        );
      CpuRegisterTableWrite (ProcessorNumber, CacheControl, 0, BIT0, 1);
    }
  }
  return RETURN_SUCCESS;
}

/**
  Initializes Mlc Spatial Prefetcher feature to specific state.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().
  @param[in]  State            If TRUE, then the Mlc Spatial Prefetcher feature
                               must be enabled.
                               If FALSE, then the Mlc Spatial Prefetcher feature
                               must be disabled.

  @retval RETURN_SUCCESS       Mlc Spatial Prefetcher feature is initialized.

  @note This service could be called by BSP only.
**/
RETURN_STATUS
EFIAPI
MlcSpatialPrefetcherInitialize (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData,  OPTIONAL
  IN BOOLEAN                           State
  )
{
  //
  // Core scope.
  //
  if (CpuInfo->ProcessorInfo.Location.Thread == 0) {
    if (!State) {
      CpuRegisterTableWrite (ProcessorNumber, CacheControl, 0, BIT0, 0);
      CPU_REGISTER_TABLE_WRITE_FIELD (
        ProcessorNumber,
        Msr,
        ICX_MSR_MISC_FEATURE_CONTROL,
        ICX_MSR_MISC_FEATURE_CONTROL_REGISTER,
        Bits.MlcSpatialPrefetchDisable,
        1
        );
      CpuRegisterTableWrite (ProcessorNumber, CacheControl, 0, BIT0, 1);
    }
  }
  return RETURN_SUCCESS;
}
