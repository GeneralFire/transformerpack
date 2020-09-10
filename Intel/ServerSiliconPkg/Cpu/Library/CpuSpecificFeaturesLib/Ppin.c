/** @file
  Code for Protected Processor Inventory Number(PPIN) feature.

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
  Prepares for the data used by CPU feature detection and initialization.

  @param[in]  NumberOfProcessors  The number of CPUs in the platform.

  @return  Pointer to a buffer of CPU related configuration data.

  @note This service could be called by BSP only.
**/
VOID *
EFIAPI
SpecificPpinGetConfigData (
  IN UINTN               NumberOfProcessors
  )
{
  VOID          *ConfigData;

  ConfigData = AllocateZeroPool (sizeof (MSR_PPIN_CTL_REGISTER) * NumberOfProcessors);
  ASSERT (ConfigData != NULL);
  return ConfigData;
}

/**
  Detects if Protected Processor Inventory Number feature supported on current
  processor.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().

  @retval TRUE     Protected Processor Inventory Number feature is supported.
  @retval FALSE    Protected Processor Inventory Number feature is not supported.

  @note This service could be called by BSP/APs.
**/
BOOLEAN
EFIAPI
SpecificPpinSupport (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData  OPTIONAL
  )
{
  ICX_MSR_PLATFORM_INFO_REGISTER    PlatformInfo;
  MSR_PPIN_CTL_REGISTER             *MsrPpinCtrl;

  PlatformInfo.Uint64 = AsmReadMsr64 (ICX_MSR_PLATFORM_INFO);

  if (PlatformInfo.Bits.PpinCap != 0) {
    MsrPpinCtrl = (MSR_PPIN_CTL_REGISTER *) ConfigData;
    ASSERT (MsrPpinCtrl != NULL);
    if (MsrPpinCtrl == NULL) {
      return FALSE;
    }
    MsrPpinCtrl[ProcessorNumber].Uint64 = AsmReadMsr64 (MSR_PPIN_CTL);
    return TRUE;
  }
  return FALSE;
}

/**
  Initializes Protected Processor Inventory Number feature to specific state.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().
  @param[in]  State            If TRUE, then the Protected Processor Inventory
                               Number feature must be enabled.
                               If FALSE, then the Protected Processor Inventory
                               Number feature must be disabled.

  @retval RETURN_SUCCESS            Protected Processor Inventory Number feature is
                                    initialized.
  @retval RETURN_OUT_OF_RESOURCES   ConfigData is NULL.
  @retval RETURN_DEVICE_ERROR       Device can't change state because it has been
                                    locked.

  @note This service could be called by BSP only.
**/
RETURN_STATUS
EFIAPI
SpecificPpinInitialize (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData,  OPTIONAL
  IN BOOLEAN                           State
  )
{
  MSR_PPIN_CTL_REGISTER     *MsrPpinCtrl;

  MsrPpinCtrl = (MSR_PPIN_CTL_REGISTER *) ConfigData;
  ASSERT (MsrPpinCtrl != NULL);
  if (MsrPpinCtrl == NULL) {
    return RETURN_OUT_OF_RESOURCES;
  }

  //
  // If already locked, just based on the request state and
  // the current state to return the status.
  //
  if (MsrPpinCtrl[ProcessorNumber].Bits.Lock != 0) {
    return MsrPpinCtrl[ProcessorNumber].Bits.Enable == State ? RETURN_SUCCESS : RETURN_DEVICE_ERROR;
  }

  //
  // The scope of the MSR_PPIN_CTL is package level, only program MSR_PPIN_CTL once for each package.
  //
  if (!IsPackageFirstProcessor (ProcessorNumber, CpuInfo)) {
    return RETURN_SUCCESS;
  }

  if (State) {
    //
    // Enable and Unlock.
    //
    MsrPpinCtrl[ProcessorNumber].Bits.Enable = 1;
    MsrPpinCtrl[ProcessorNumber].Bits.Lock = 0;
  } else {
    //
    // Disable and Lock.
    //
    MsrPpinCtrl[ProcessorNumber].Bits.Enable = 0;
    MsrPpinCtrl[ProcessorNumber].Bits.Lock = 1;
  }

  CPU_REGISTER_TABLE_WRITE64 (
    ProcessorNumber,
    Msr,
    MSR_PPIN_CTL,
    MsrPpinCtrl[ProcessorNumber].Uint64
    );

  return RETURN_SUCCESS;
}
