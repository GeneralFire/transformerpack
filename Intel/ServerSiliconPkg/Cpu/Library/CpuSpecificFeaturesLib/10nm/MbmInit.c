/** @file
  Code for MbmInit feature (updates MBM package MSR).

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
#include <RcRegs.h>
#include <Library/UsraCsrLib.h>
#include <Library/KtiApi.h>
#include <Guid/SocketCommonRcVariable.h>
#include <KtiSetupDefinitions.h>

typedef struct {
  UINT8  ChasPerCluster;
  UINT8  ChaThresholdWithinCluster;
  UINT8  CorrectionFactorHi;
  UINT8  CorrectionFactorLo;
} MBM_CORRECTION;

MBM_CORRECTION mMbmCorrectionFactorTBL[] = {
// ChasPerCluster  ChaThresholdWithinCluster  CorrectionFactorHi CorrectionFactorLo
  { 1,                         0,                       100,               100 },
  { 2,                         0,                       100,               100 },
  { 3,                         2,                       97,                102 },
  { 4,                         0,                       100,               100 },
  { 5,                         4,                       107,               98  },
  { 6,                         4,                       97,                102 },
  { 7,                         6,                       114,               98  },
  { 8,                         0,                       100,               100 },
  { 9,                         8,                       119,               98  },
  { 10,                        8,                       107,               98  },
  { 11,                        10,                      145,               97  },
  { 12,                        0,                       100,               100 },
  { 13,                        12,                      123,               98  },
  { 14,                        12,                      114,               98  },
  { 15,                        12,                      107,               98  },
  { 16,                        0,                       100,               100 },
  { 17,                        16,                      125,               99  },
  { 18,                        16,                      119,               98  },
  { 19,                        0,                       100,               100 },
  { 20,                        16,                      107,               98  },
  { 21,                        0,                       100,               100 },
  { 22,                        20,                      145,               97  },
  { 23,                        0,                       100,               100 },
  { 24,                        16,                      97,                102 },
  { 25,                        24,                      128,               99  },
  { 26,                        24,                      123,               98  },
  { 27,                        0,                       100,               100 },
  { 28,                        24,                      114,               98  },
  { 29,                        0,                       100,               100 },
  { 30,                        24,                      107,               98  },
  { 31,                        0,                       100,               100 },
  { 32,                        0,                       100,               100 },
  { 33,                        0,                       100,               100 },
  { 34,                        32,                      125,               99  },
  { 35,                        0,                       100,               100 },
  { 36,                        32,                      119,               98  },
  { 37,                        0,                       100,               100 },
  { 38,                        32,                      112,               98  },
  { 39,                        0,                       100,               100 },
  { 40,                        32,                      107,               98  },
  { 41,                        40,                      156,               99  },
  { 42,                        40,                      152,               98  },
  { 43,                        40,                      149,               98  },
  { 44,                        40,                      145,               97  },
  { 45,                        40,                      142,               96  },
  { 46,                        40,                      139,               96  },
  { 47,                        40,                      136,               96  },
  { 48,                        32,                      97,                102 },
  { 49,                        48,                      131,               100 },
  { 50,                        48,                      128,               99  },
  { 51,                        48,                      126,               99  },
  { 52,                        48,                      123,               98  },
  { 53,                        48,                      121,               98  },
  { 54,                        48,                      118,               98  },
  { 55,                        48,                      116,               98  },
  { 56,                        48,                      114,               98  },
  { 57,                        48,                      112,               98  },
  { 58,                        48,                      110,               98  },
  { 59,                        48,                      108,               98  },
  { 60,                        48,                      107,               98  },
  { 61,                        48,                      105,               99  },
  { 62,                        48,                      103,               99  },
  { 63,                        48,                      102,               100 },
  { 64,                        0,                       100,               100 }
};

/**
  Get MBM correction factor entry from mMbmCorrectionFactorTBL.

  @param[in] ChasPerCluster     CHAs per cluster.

  @retval MBM correction factor entry pointer, NULL if not found.

**/
MBM_CORRECTION *
GetMbmCorrectionFactor (
  IN UINT8                ChasPerCluster
  )
{
  UINT8                   Index;
  MBM_CORRECTION          *MbmTblPtr;

  for (Index = 0; Index < ARRAY_SIZE(mMbmCorrectionFactorTBL); Index++) {
    MbmTblPtr = &mMbmCorrectionFactorTBL[Index];
    if (ChasPerCluster == MbmTblPtr->ChasPerCluster) {
      return MbmTblPtr;
    }
  }

  return NULL;
}

/**
  Detects if MbmInit feature supported on current processor.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().

  @retval TRUE     MbmInit feature is supported.
  @retval FALSE    MbmInit feature is not supported.

  @note This service could be called by BSP/APs.
**/
BOOLEAN
EFIAPI
MbmSupport (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData  OPTIONAL
  )
{
  CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS_EBX   RegEbx;

  if ((FeaturePcdGet (PcdCpuIcelakeFamilyFlag) && IS_ICELAKE_SERVER_PROC (CpuInfo->DisplayFamily, CpuInfo->DisplayModel)) ||
      (FeaturePcdGet (PcdCpuIcelakeFamilyFlag) && IS_ICELAKE_D_SERVER_PROC (CpuInfo->DisplayFamily, CpuInfo->DisplayModel)) ||
      (FeaturePcdGet (PcdCpuSapphireRapidsFamilyFlag) && IS_SAPPHIRERAPIDS_SERVER_PROC (CpuInfo->DisplayFamily, CpuInfo->DisplayModel))) {
    AsmCpuidEx (
      CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS,
      CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS_SUB_LEAF_INFO,
      NULL,
      &RegEbx.Uint32,
      NULL,
      NULL
      );

    if (RegEbx.Bits.RDT_M) {
      //
      // RDT_M capability is set.
      //
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Initializes MbmInit feature to specific state.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().
  @param[in]  State            If TRUE, then the MbmInit feature
                               must be enabled.
                               If FALSE, then the MbmInit feature
                               must be disabled.

  @retval RETURN_SUCCESS       MbmInit feature is initialized.
  @retval RETURN_DEVICE_ERROR  Unknown configuration.

  @note This service could be called by BSP only.
**/
RETURN_STATUS
EFIAPI
MbmInitialize (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData,  OPTIONAL
  IN BOOLEAN                           State
  )
{
  UINT8                                         SocketId;
  CAPID4_PCU_FUN3_STRUCT                        Capid4Csr;
  UINT8                                         UmaBasedClustering;
  UINT8                                         SncEn;
  UINT8                                         ChasPerSocket;
  UINT8                                         ChasPerCluster;
  UINT8                                         NumOfClusters;
  MBM_CORRECTION                                *MbmCorrection;
  OVERRIDE_ICX_MSR_QMC_CNTR_CORRECTION_REGISTER MsrQmcCntrValue;

  //
  // The scope of the ICX_MSR_QMC_CNTR_CORRECTION is package level, only program ICX_MSR_QMC_CNTR_CORRECTION once for each package.
  //
  if (IsPackageFirstProcessor (ProcessorNumber, CpuInfo)) {
    SocketId = (UINT8) CpuInfo->ProcessorInfo.Location.Package;

    Capid4Csr.Data = UsraCsrRead (SocketId, 0, CAPID4_PCU_FUN3_REG);

    DEBUG ((DEBUG_INFO, ":MBM: Socket = %d, Processor = %d\n", SocketId, ProcessorNumber));
    DEBUG ((DEBUG_INFO, "  Capid4Csr = 0x%x qos_mon (Bit[10]) = %d\n", Capid4Csr.Data, Capid4Csr.Bits.llc_qos_mon_en));

    if (Capid4Csr.Bits.llc_qos_mon_en) {
      //
      // QOS_MON fuse is set.
      //
      UmaBasedClustering = GetKtiOutputUmaClusterSetting ();
      SncEn = GetNumOfClusterPerSystem ();
      if ((UmaBasedClustering == UMABASEDCLUSTERING_DISABLED) && (SncEn == KTI_DISABLE)) {
        NumOfClusters = 1;
      } else if ((UmaBasedClustering == UMABASEDCLUSTERING_HEMISPHERE) || (SncEn == KTI_SNC2)) {
        NumOfClusters = 2;
      } else if ((UmaBasedClustering == UMABASEDCLUSTERING_QUADRANT) || (SncEn == KTI_SNC4)) {
        NumOfClusters = 4;
      } else {
        DEBUG ((DEBUG_ERROR, "  Unknown UmaBasedClustering = %d, SncEn = %d\n", UmaBasedClustering, SncEn));
        return RETURN_DEVICE_ERROR;
      }

      ChasPerSocket = GetTotChaCount (SocketId);
      ChasPerCluster = ChasPerSocket / NumOfClusters;

      MbmCorrection = GetMbmCorrectionFactor (ChasPerCluster);
      if (MbmCorrection != NULL) {
        //
        // Found correction factor
        //
        DEBUG ((
          DEBUG_INFO,
          "  ChasPerCluster = %d, ChaThresholdWithinCluster = %d, CorrectionFactorHi = %d CorrectionFactorLo = %d\n",
          MbmCorrection->ChasPerCluster,
          MbmCorrection->ChaThresholdWithinCluster,
          MbmCorrection->CorrectionFactorHi,
          MbmCorrection->CorrectionFactorLo
          ));

        MsrQmcCntrValue.Uint64 = 0;
        MsrQmcCntrValue.Bits.ChasPerCluster = MbmCorrection->ChasPerCluster;
        MsrQmcCntrValue.Bits.ChaThresholdWithinCluster = MbmCorrection->ChaThresholdWithinCluster;
        MsrQmcCntrValue.Bits.CorrectionFactorHi = MbmCorrection->CorrectionFactorHi;
        MsrQmcCntrValue.Bits.CorrectionFactorLo = MbmCorrection->CorrectionFactorLo;

        CPU_REGISTER_TABLE_WRITE32 (
          ProcessorNumber,
          Msr,
          ICX_MSR_QMC_CNTR_CORRECTION,
          MsrQmcCntrValue.Uint32
          );
      } else {
        DEBUG ((DEBUG_ERROR, "  Unknown ChasPerCluster = %d\n", ChasPerCluster));
        return RETURN_DEVICE_ERROR;
      }
    }
  }

  return RETURN_SUCCESS;
}
