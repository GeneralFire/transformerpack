/** @file
  Code for PECI Feature.

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
#include <Library/OobIpLib.h>

/**
  Initializes Peci Downstream feature to specific state.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().
  @param[in]  State            If TRUE, then the Peci Downstream feature
                               must be enabled.
                               If FALSE, then the Peci Downstream feature
                               must be disabled.

  @retval RETURN_SUCCESS       Peci Downstream feature is initialized.

  @note This service could be called by BSP only.
**/
RETURN_STATUS
EFIAPI
PeciDownstreamInitialize (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData,  OPTIONAL
  IN BOOLEAN                           State
  )
{
  //
  // The scope of the SKX_MSR_PECI_DNSTRM_WRITE_EN is package level, only program
  // SKX_MSR_PECI_DNSTRM_WRITE_EN once for each package.
  //
  if (!IsPackageFirstProcessor (ProcessorNumber, CpuInfo)) {
    return RETURN_SUCCESS;
  }

  if (FeaturePcdGet (PcdCpuSkylakeFamilyFlag) && IS_SKYLAKE_SERVER_PROC (CpuInfo->DisplayFamily, CpuInfo->DisplayModel)) {
    CPU_REGISTER_TABLE_WRITE_FIELD (
      ProcessorNumber,
      Msr,
      SKX_MSR_PECI_DNSTRM_WRITE_EN,
      SKX_MSR_PECI_DNSTRM_WRITE_EN_REGISTER,
      Bits.Pdcwe,
      (State) ? 1 : 0
      );
  } else {
    OobSetPeciDownstreamEnabled ((UINT8) CpuInfo->ProcessorInfo.Location.Package, (State) ? 1 : 0);
  }

  return RETURN_SUCCESS;
}
