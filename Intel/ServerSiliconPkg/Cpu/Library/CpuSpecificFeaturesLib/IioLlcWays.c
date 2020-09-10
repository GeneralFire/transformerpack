/** @file
  Code for IioLlcWays feature.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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
#include <RcRegs.h>
#include <Library/KtiApi.h>
#include <Library/UsraCsrLib.h>
#include <Library/SiliconWorkaroundLib.h>

typedef struct {
  ICX_MSR_IIO_LLC_WAYS_REGISTER         IioLlcWays;
  SKX_MSR_LLC_WAYS_UPDATE_REGISTER      LlcWaysUpdate;
  SKX_MSR_RRQ_THRESHOLD_TUNING_REGISTER RrqThresholdTuning;
} IIO_LLC_WYAS_MSR_CONFIG_DATA;

typedef struct {
  CAPID5_PCU_FUN3_STRUCT                Capid5Csr[MAX_SOCKET];
  //IIO_LLC_WYAS_MSR_CONFIG_DATA          MsrConfigData[NumberOfProcessors];
} IIO_LLC_WYAS_CONFIG_DATA;

/**
  Prepares for the data used by CPU feature detection and initialization.

  @param[in]  NumberOfProcessors  The number of CPUs in the platform.

  @return  Pointer to a buffer of CPU related configuration data.

  @note This service could be called by BSP only.
**/
VOID *
EFIAPI
IioLlcWaysGetConfigData (
  IN UINTN               NumberOfProcessors
  )
{
  IIO_LLC_WYAS_CONFIG_DATA  *IioLlcWaysConfigData;
  UINT8                     SocketId;

  IioLlcWaysConfigData = AllocateZeroPool (sizeof (IIO_LLC_WYAS_CONFIG_DATA) + sizeof (IIO_LLC_WYAS_MSR_CONFIG_DATA) * NumberOfProcessors);
  ASSERT (IioLlcWaysConfigData != NULL);
  if (IioLlcWaysConfigData == NULL) {
    return NULL;
  }

  for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
    if (SocketPresent (SocketId)) {
      IioLlcWaysConfigData->Capid5Csr[SocketId].Data = UsraCsrRead (SocketId, 0, CAPID5_PCU_FUN3_REG);
      DEBUG ((DEBUG_INFO, ":IioLlcWays: Socket = %d\n", SocketId));
      DEBUG ((DEBUG_INFO, "  Capid5Csr = 0x%x iio_llcconfig_en (Bit[1]) = %d\n", IioLlcWaysConfigData->Capid5Csr[SocketId].Data, IioLlcWaysConfigData->Capid5Csr[SocketId].Bits.iio_llcconfig_en));
    }
  }

  return IioLlcWaysConfigData;
}

/**
  Detects if IioLlcWays feature supported on current processor.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().

  @retval TRUE     IioLlcWays feature is supported.
  @retval FALSE    IioLlcWays feature is not supported.

  @note This service could be called by BSP/APs.
**/
BOOLEAN
EFIAPI
IioLlcWaysSupport (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData  OPTIONAL
  )
{
  IIO_LLC_WYAS_CONFIG_DATA      *IioLlcWaysConfigData;
  CAPID5_PCU_FUN3_STRUCT        *Capid5Csr;
  IIO_LLC_WYAS_MSR_CONFIG_DATA  *MsrConfigData;
  UINT8                         SocketId;

  IioLlcWaysConfigData = (IIO_LLC_WYAS_CONFIG_DATA *) ConfigData;
  ASSERT (IioLlcWaysConfigData != NULL);
  if (IioLlcWaysConfigData == NULL) {
    return FALSE;
  }
  Capid5Csr = IioLlcWaysConfigData->Capid5Csr;
  MsrConfigData = (IIO_LLC_WYAS_MSR_CONFIG_DATA *) (IioLlcWaysConfigData + 1);

  SocketId = (UINT8) CpuInfo->ProcessorInfo.Location.Package;
  if (Capid5Csr[SocketId].Bits.iio_llcconfig_en != 0) {
    MsrConfigData[ProcessorNumber].IioLlcWays.Uint64 = AsmReadMsr64 (ICX_MSR_IIO_LLC_WAYS);
    if (FeaturePcdGet (PcdCpuSkylakeFamilyFlag) && IS_SKYLAKE_SERVER_PROC (CpuInfo->DisplayFamily, CpuInfo->DisplayModel)) {
      MsrConfigData[ProcessorNumber].LlcWaysUpdate.Uint64 = AsmReadMsr64 (SKX_MSR_LLC_WAYS_UPDATE);
      MsrConfigData[ProcessorNumber].RrqThresholdTuning.Uint64 = AsmReadMsr64 (SKX_MSR_RRQ_THRESHOLD_TUNING);
    }
    return TRUE;
  }

  return FALSE;
}

/**
  Initializes IioLlcWays feature to specific state.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().
  @param[in]  State            If TRUE, then the IioLlcWays feature
                               must be enabled.
                               If FALSE, then the IioLlcWays feature
                               must be disabled.

  @retval RETURN_SUCCESS            IioLlcWays feature is initialized.
  @retval RETURN_OUT_OF_RESOURCES   ConfigData is NULL.

  @note This service could be called by BSP only.
**/
RETURN_STATUS
EFIAPI
IioLlcWaysInitialize (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData,  OPTIONAL
  IN BOOLEAN                           State
  )
{
  IIO_LLC_WYAS_CONFIG_DATA              *IioLlcWaysConfigData;
  IIO_LLC_WYAS_MSR_CONFIG_DATA          *MsrConfigData;
  UINT8                                 SocketId;
  ICX_MSR_IIO_LLC_WAYS_REGISTER         *IioLlcWaysMsr;
  SKX_MSR_IIO_LLC_WAYS_REGISTER         *SkxIioLlcWaysMsr;
  SNR_MSR_VIRT_CR_IIO_LLC_WAYS_REGISTER *SnrIioLlcWaysMsr;
  SKX_MSR_LLC_WAYS_UPDATE_REGISTER      *LlcWaysUpdateMsr;
  BOOLEAN                               LlcWaysUpdateMsrChanged;
  SKX_MSR_RRQ_THRESHOLD_TUNING_REGISTER *RrqThresholdTuningMsr;

  //
  // The MSRs to program are Package scope.
  //
  if (State && IsPackageFirstProcessor (ProcessorNumber, CpuInfo)) {
    IioLlcWaysConfigData = (IIO_LLC_WYAS_CONFIG_DATA *) ConfigData;
    ASSERT (IioLlcWaysConfigData != NULL);
    if (IioLlcWaysConfigData == NULL) {
      return RETURN_OUT_OF_RESOURCES;
    }
    MsrConfigData = (IIO_LLC_WYAS_MSR_CONFIG_DATA *) (IioLlcWaysConfigData + 1);

    SocketId = (UINT8) CpuInfo->ProcessorInfo.Location.Package;

    DEBUG ((DEBUG_INFO, ":IioLlcWays: Socket = %d, Processor = %d\n", SocketId, ProcessorNumber));

    //
    // Proggram MSR MSR_IIO_LLC_WAYS
    //
    if (mCpuSpecificFeaturesCpuPolicy->CpuIioLlcWaysBitMask != 0) {
      IioLlcWaysMsr = &MsrConfigData[ProcessorNumber].IioLlcWays;
      if (FeaturePcdGet (PcdCpuSkylakeFamilyFlag) && IS_SKYLAKE_SERVER_PROC (CpuInfo->DisplayFamily, CpuInfo->DisplayModel)) {
        //
        // The Iiocosways field bitwidth for SKX is different with others.
        //
        SkxIioLlcWaysMsr = (SKX_MSR_IIO_LLC_WAYS_REGISTER *) IioLlcWaysMsr;
        SkxIioLlcWaysMsr->Bits.Iiocosways = mCpuSpecificFeaturesCpuPolicy->CpuIioLlcWaysBitMask;
      } else if (FeaturePcdGet (PcdCpuSnowridgeFamilyFlag) && IS_SNOWRIDGE_SERVER_PROC (CpuInfo->DisplayFamily, CpuInfo->DisplayModel)) {
        //
        // The WayMask field bitwidth for SNR is different with others.
        //
        SnrIioLlcWaysMsr = (SNR_MSR_VIRT_CR_IIO_LLC_WAYS_REGISTER *) IioLlcWaysMsr;
        SnrIioLlcWaysMsr->Bits.WayMask = mCpuSpecificFeaturesCpuPolicy->CpuIioLlcWaysBitMask;
      } else {
        IioLlcWaysMsr->Bits.Iiocosways = mCpuSpecificFeaturesCpuPolicy->CpuIioLlcWaysBitMask;
      }
      DEBUG ((DEBUG_INFO, "  Set MSR_IIO_LLC_WAYS = 0x%lx\n", IioLlcWaysMsr->Uint64));
      CPU_REGISTER_TABLE_WRITE64 (
        ProcessorNumber,
        Msr,
        ICX_MSR_IIO_LLC_WAYS,
        IioLlcWaysMsr->Uint64
        );
    }

    //
    // Proggram other MSRs for SKX
    //
    if (FeaturePcdGet (PcdCpuSkylakeFamilyFlag) && IS_SKYLAKE_SERVER_PROC (CpuInfo->DisplayFamily, CpuInfo->DisplayModel)) {
      //
      // Proggram MSR MSR_LLC_WAYS_UPDATE
      //
      LlcWaysUpdateMsr = &MsrConfigData[ProcessorNumber].LlcWaysUpdate;
      LlcWaysUpdateMsrChanged = FALSE;
      if (mCpuSpecificFeaturesCpuPolicy->CpuIioLlcWaysBitMask != 0) {
        //
        // 4929403: BIOS programming and knob for expanded-IO ways
        // Need to program the upper 11 bits of IIO_LLC_WAYS into Ingress_Spare MSR
        //
        LlcWaysUpdateMsr->Bits.DdioIioLlcCoswaysMask = (UINT32) (SkxIioLlcWaysMsr->Bits.Iiocosways >> 12);
        LlcWaysUpdateMsrChanged = TRUE;
      }
      if (mCpuSpecificFeaturesCpuPolicy->CpuExpandedIioLlcWaysBitMask != 0) {
        LlcWaysUpdateMsr->Bits.DdioIioLlcCoswaysMask = mCpuSpecificFeaturesCpuPolicy->CpuExpandedIioLlcWaysBitMask;
        LlcWaysUpdateMsrChanged = TRUE;
      }
      //
      // 4930178: BIOS programming and knob for remote_ways
      //
      if (mCpuSpecificFeaturesCpuPolicy->CpuRemoteWaysBitMask != 0) {
        //
        // For 10nm, REMOTE_WAYS are moved to CSR, don't program it here
        //
        LlcWaysUpdateMsr->Bits.RemoteWays = mCpuSpecificFeaturesCpuPolicy->CpuRemoteWaysBitMask;
        LlcWaysUpdateMsrChanged = TRUE;
      }
      if (LlcWaysUpdateMsrChanged) {
        DEBUG ((DEBUG_INFO, "  Set SKX_MSR_LLC_WAYS_UPDATE = 0x%lx\n", LlcWaysUpdateMsr->Uint64));
        CPU_REGISTER_TABLE_WRITE64 (
          ProcessorNumber,
          Msr,
          SKX_MSR_LLC_WAYS_UPDATE,
          LlcWaysUpdateMsr->Uint64
          );
      }

      //
      // Proggram MSR MSR_RRQ_THRESHOLD_TUNING
      //
      //
      // 4930188: Ucode Virtual MSR needed to configure for huge multisocket perf
      //          imbalance caused by remote reads filling TOR.
      //          This is for SKX B0+
      //
      if (IsSiliconWorkaroundEnabled ("S4930188")) {
        if (mCpuSpecificFeaturesCpuPolicy->CpuRrqCountThreshold != 0) {
          RrqThresholdTuningMsr = &MsrConfigData[ProcessorNumber].RrqThresholdTuning;
          RrqThresholdTuningMsr->Bits.RrqThresholdValue = mCpuSpecificFeaturesCpuPolicy->CpuRrqCountThreshold;
          RrqThresholdTuningMsr->Bits.Lock = 1;
          DEBUG ((DEBUG_INFO, "  Set SKX_MSR_RRQ_THRESHOLD_TUNING = 0x%lx\n", RrqThresholdTuningMsr->Uint64));
          CPU_REGISTER_TABLE_WRITE64 (
            ProcessorNumber,
            Msr,
            SKX_MSR_RRQ_THRESHOLD_TUNING,
            RrqThresholdTuningMsr->Uint64
            );
        }
      }
    }
  }

  return RETURN_SUCCESS;
}
