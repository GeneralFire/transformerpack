/** @file
  CPU Specific Features Library code for both PEI and DXE phases.

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

CPU_POLICY_COMMON *mCpuSpecificFeaturesCpuPolicy = NULL;

/**
  Select a processor as package first processor for package scope programming.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION

  @retval TRUE     The processor is selected as package first processor.
  @retval FALSE    The processor is not selected as package first processor.

**/
BOOLEAN
IsPackageFirstProcessor (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo
  )
{
  if ((CpuInfo->First.Die == 1) &&
      (CpuInfo->First.Tile == 1) &&
      (CpuInfo->First.Module == 1) &&
      (CpuInfo->First.Core == 1) &&
      (CpuInfo->First.Thread == 1)) {
    //
    // Select first thread of first core of first module of first tile of first die
    // as package first processor.
    //
    return TRUE;
  }
  return FALSE;
}

/**
  CPU generation support check function can be used for some features.
  Other features may need have their own support check function.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().

  @retval TRUE     The feature is supported.
  @retval FALSE    The feature is not supported.

  @note This service could be called by BSP/APs.
**/
BOOLEAN
EFIAPI
CpuGenerationSupportCheck (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData  OPTIONAL
  )
{
  //
  // Skylake/Icelake/SapphireRapids/Snowridge processor.
  //
  if ((FeaturePcdGet (PcdCpuSkylakeFamilyFlag) && IS_SKYLAKE_SERVER_PROC (CpuInfo->DisplayFamily, CpuInfo->DisplayModel)) ||
      (FeaturePcdGet (PcdCpuIcelakeFamilyFlag) && IS_ICELAKE_SERVER_PROC (CpuInfo->DisplayFamily, CpuInfo->DisplayModel)) ||
      (FeaturePcdGet (PcdCpuSapphireRapidsFamilyFlag) && IS_SAPPHIRERAPIDS_SERVER_PROC (CpuInfo->DisplayFamily, CpuInfo->DisplayModel)) ||
      (FeaturePcdGet (PcdCpuSnowridgeFamilyFlag) && IS_SNOWRIDGE_SERVER_PROC (CpuInfo->DisplayFamily, CpuInfo->DisplayModel)) ||
      (FeaturePcdGet (PcdCpuIcelakeFamilyFlag) && IS_ICELAKE_D_SERVER_PROC(CpuInfo->DisplayFamily, CpuInfo->DisplayModel))) {
    return TRUE;
  }
  return FALSE;
}

/**
  Common function to return FALSE (unsupported) for some features.

  @param[in]  ProcessorNumber  The index of the CPU executing this function.
  @param[in]  CpuInfo          A pointer to the REGISTER_CPU_FEATURE_INFORMATION
                               structure for the CPU executing this function.
  @param[in]  ConfigData       A pointer to the configuration buffer returned
                               by CPU_FEATURE_GET_CONFIG_DATA.  NULL if
                               CPU_FEATURE_GET_CONFIG_DATA was not provided in
                               RegisterCpuFeature().

  @retval TRUE     The feature is supported.
  @retval FALSE    The feature is not supported.

  @note This service could be called by BSP/APs.
**/
BOOLEAN
EFIAPI
CpuFeatureUnsupported (
  IN UINTN                             ProcessorNumber,
  IN REGISTER_CPU_FEATURE_INFORMATION  *CpuInfo,
  IN VOID                              *ConfigData  OPTIONAL
  )
{
  return FALSE;
}

/**
  NULL implementation of GetConfigData.

  @param[in]  NumberOfProcessors  The number of CPUs in the platform.

  @return  Pointer to a buffer of CPU related configuration data.

  @note This service could be called by BSP only.
**/
VOID *
EFIAPI
CpuFeatureGetConfigDataNull (
  IN UINTN  NumberOfProcessors
  )
{
  return NULL;
}

/**
  Override some functions for some common features
  which have original implementation in UefiCpuPkg CpuCommonFeaturesLib.

**/
VOID
OverrideCommonFeatures (
  VOID
  )
{
  EFI_STATUS         Status;

  //
  // Override some functions for some common features
  // which have original implementation in UefiCpuPkg CpuCommonFeaturesLib.
  //
  if (IsCpuFeatureSupported (CPU_FEATURE_EIST)) {
    Status = RegisterCpuFeature (
               "EIST",
               SpecificEistGetConfigData, // Use own function
               SpecificEistSupport,       // Override core function EistSupport() in Eist.c of CpuCommonFeaturesLib
               SpecificEistInitialize,    // Override core function EistInitialize() in Eist.c of CpuCommonFeaturesLib
               CPU_FEATURE_EIST,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  if (IsCpuFeatureSupported (CPU_FEATURE_FASTSTRINGS)) {
    Status = RegisterCpuFeature (
               "FastStrings",
               NULL,
               CpuGenerationSupportCheck, // Use own function
               NULL,
               CPU_FEATURE_FASTSTRINGS,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  if (IsCpuFeatureSupported (CPU_FEATURE_LOCK_FEATURE_CONTROL_REGISTER)) {
    //
    // Other codes have been handling MSR_IA32_FEATURE_CONTROL_REGISTER.Bits.Lock,
    // see LockFeatureControl(for normal boot) and RestoreCpuMsrs(for S3).
    // So here just use CpuFeatureUnsupported() to return FALSE (unsupported).
    //
    Status = RegisterCpuFeature (
               "Lock Feature Control Register",
               NULL,
               CpuFeatureUnsupported,        // Override core function LockFeatureControlRegisterSupport() in FeatureControl.c of CpuCommonFeaturesLib
               NULL,
               CPU_FEATURE_LOCK_FEATURE_CONTROL_REGISTER,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  if (IsCpuFeatureSupported (CPU_FEATURE_PENDING_BREAK)) {
    Status = RegisterCpuFeature (
               "Pending Break",
               NULL,
               SpecificPendingBreakSupport, // Override core function PendingBreakSupport() in PendingBreak.c of CpuCommonFeaturesLib
               NULL,
               CPU_FEATURE_PENDING_BREAK,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  if (IsCpuFeatureSupported (CPU_FEATURE_C1E)) {
    //
    // Since C1E programming has been in early PEI phase per ACP feature's request,
    // see SetEarlyC1e().
    // So here just use CpuFeatureUnsupported() to return FALSE (unsupported).
    //
    Status = RegisterCpuFeature (
               "C1E",
               NULL,
               CpuFeatureUnsupported, // Override core function C1eSupport() in C1e.c of CpuCommonFeaturesLib
               NULL,
               CPU_FEATURE_C1E,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  if (IsCpuFeatureSupported (CPU_FEATURE_PPIN)) {
    Status = RegisterCpuFeature (
               "PPIN",
               SpecificPpinGetConfigData, // Override core function PpinGetConfigData() in Ppin.c of CpuCommonFeaturesLib
               SpecificPpinSupport,       // Override core function PpinSupport() in Ppin.c of CpuCommonFeaturesLib
               SpecificPpinInitialize,    // Override core function PpinInitialize() in Ppin.c of CpuCommonFeaturesLib
               CPU_FEATURE_PPIN,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  if (IsCpuFeatureSupported (CPU_FEATURE_LMCE)) {
    //
    // Other code has been handling MSR_IA32_FEATURE_CONTROL_REGISTER.Bits.LmceOn,
    // see ApEnableLmce().
    // So here just use CpuFeatureUnsupported() to return FALSE (unsupported).
    //
    Status = RegisterCpuFeature (
               "LMCE",
               NULL,
               CpuFeatureUnsupported,       // Override core function LmceSupport() in MachineCheck.c of CpuCommonFeaturesLib
               NULL,
               CPU_FEATURE_LMCE,
               CPU_FEATURE_LOCK_FEATURE_CONTROL_REGISTER | CPU_FEATURE_THREAD_BEFORE,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  if (IsCpuFeatureSupported (CPU_FEATURE_PROC_TRACE)) {
    //
    // Not required yet.
    // So here just use CpuFeatureUnsupported() to return FALSE (unsupported).
    //
    Status = RegisterCpuFeature (
               "Proc Trace",
               CpuFeatureGetConfigDataNull, // Override core function ProcTraceGetConfigData() in ProcTrace.c of CpuCommonFeaturesLib
               CpuFeatureUnsupported,       // Override core function ProcTraceSupport() in ProcTrace.c of CpuCommonFeaturesLib
               NULL,
               CPU_FEATURE_PROC_TRACE,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }

  //
  // Reuse the common code completely from UefiCpuPkg CpuCommonFeaturesLib
  // for the features below.
  //   CPU_FEATURE_ACPI                : ClockModulation.c
  //   CPU_FEATURE_AESNI               : Aesni.c
  //   CPU_FEATURE_LIMIT_CPUID_MAX_VAL : LimitCpuIdMaxval.c
  //   CPU_FEATURE_MCE                 : MachineCheck.c
  //   CPU_FEATURE_MCA                 : MachineCheck.c
  //   CPU_FEATURE_MCG_CTL             : MachineCheck.c
  //   CPU_FEATURE_MWAIT               : MonitorMwait.c
  //   CPU_FEATURE_SMX                 : FeatureControl.c
  //   CPU_FEATURE_VMX                 : FeatureControl.c
  //   CPU_FEATURE_X2APIC              : X2Apic.c
  //
}

/**
  Register CPU features.

  @retval  RETURN_SUCCESS            Register successfully
**/
RETURN_STATUS
EFIAPI
CpuSpecificFeaturesLibConstructor (
  VOID
  )
{
  EFI_STATUS                Status;

  //
  // Override some functions for some common features
  // which have original implementation in UefiCpuPkg CpuCommonFeaturesLib.
  //
  OverrideCommonFeatures ();

  //
  // The features below are not implemented in UefiCpuPkg CpuCommonFeaturesLib at all.
  //
  if (IsCpuFeatureSupported (CPU_FEATURE_PECI_DOWN_STREAM)) {
    Status = RegisterCpuFeature (
               "PECI Down Stream",
               NULL,
               NULL,
               PeciDownstreamInitialize,
               CPU_FEATURE_PECI_DOWN_STREAM,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  if (IsCpuFeatureSupported (CPU_FEATURE_L1_NEXT_PAGE_PREFETCHER)) {
    Status = RegisterCpuFeature (
               "L1 Next Page Prefetcher",
               NULL,
               L1NextPagePrefetcherSupport,
               L1NextPagePrefetcherInitialize,
               CPU_FEATURE_L1_NEXT_PAGE_PREFETCHER,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  if (IsCpuFeatureSupported (CPU_FEATURE_DCU_PREFETCHER)) {
    Status = RegisterCpuFeature (
               "DCU Streamer Prefetcher",
               NULL,
               CpuGenerationSupportCheck,
               DcuPrefetcherInitialize,
               CPU_FEATURE_DCU_PREFETCHER,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  if (IsCpuFeatureSupported (CPU_FEATURE_IP_PREFETCHER)) {
    Status = RegisterCpuFeature (
               "DCU IP Prefetcher",
               NULL,
               CpuGenerationSupportCheck,
               IpPrefetcherInitialize,
               CPU_FEATURE_IP_PREFETCHER,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  if (IsCpuFeatureSupported (CPU_FEATURE_MLC_STREAMER_PREFETCHER)) {
    Status = RegisterCpuFeature (
               "Mlc Streamer Prefetcher",
               NULL,
               CpuGenerationSupportCheck,
               MlcStreamerPrefetcherInitialize,
               CPU_FEATURE_MLC_STREAMER_PREFETCHER,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  if (IsCpuFeatureSupported (CPU_FEATURE_MLC_SPATIAL_PREFETCHER)) {
    Status = RegisterCpuFeature (
               "Mlc Spatial Prefetcher",
               NULL,
               CpuGenerationSupportCheck,
               MlcSpatialPrefetcherInitialize,
               CPU_FEATURE_MLC_SPATIAL_PREFETCHER,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }

  if (IsCpuFeatureSupported (CPU_FEATURE_AMP_PREFETCH)) {
    Status = RegisterCpuFeature (
               "AMP Prefetcher",
               NULL,
               AmpPrefetchSupport,
               AmpPrefetchInitialize,
               CPU_FEATURE_AMP_PREFETCH,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }

  if (IsCpuFeatureSupported (CPU_FEATURE_THREE_STRIKE_COUNTER)) {
    Status = RegisterCpuFeature (
               "Three Strike Counter",
               NULL,
               CpuGenerationSupportCheck,
               ThreeStrikeCounterInitialize,
               CPU_FEATURE_THREE_STRIKE_COUNTER,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  if (IsCpuFeatureSupported (CPU_FEATURE_DBP_F)) {
    Status = RegisterCpuFeature (
               "DBP-F",
               NULL,
               DbpSupport,
               DbpInitialize,
               CPU_FEATURE_DBP_F,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  if (IsCpuFeatureSupported (CPU_FEATURE_ENERGY_PERFORMANCE_BIAS)) {
    Status = RegisterCpuFeature (
               "Energy Performance Bias",
               NULL,
               CpuGenerationSupportCheck,
               EnergyPerformanceBiasInitialize,
               CPU_FEATURE_ENERGY_PERFORMANCE_BIAS,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  if (IsCpuFeatureSupported (CPU_FEATURE_C_STATE)) {
    Status = RegisterCpuFeature (
               "C State",
               CStateGetConfigData,
               CStateSupport,
               CStateInitialize,
               CPU_FEATURE_C_STATE,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  if (IsCpuFeatureSupported (CPU_FEATURE_TM)) {
    Status = RegisterCpuFeature (
               "Thermal management",
               ThermalMonitorGetConfigData,
               ThermalMonitorSupport,
               ThermalMonitorInitialize,
               CPU_FEATURE_TM,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  if (IsCpuFeatureSupported (CPU_FEATURE_SNC_INIT)) {
    Status = RegisterCpuFeature (
               "SncInit",
               SncGetConfigData,
               SncSupport,
               SncInitialize,
               CPU_FEATURE_SNC_INIT,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  if (IsCpuFeatureSupported (CPU_FEATURE_MBM_INIT)) {
    Status = RegisterCpuFeature (
               "MbmInit",
               NULL,
               MbmSupport,
               MbmInitialize,
               CPU_FEATURE_MBM_INIT,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }
  if (IsCpuFeatureSupported (CPU_FEATURE_IIO_LLC_WAYS)) {
    Status = RegisterCpuFeature (
               "IioLlcWays",
               IioLlcWaysGetConfigData,
               IioLlcWaysSupport,
               IioLlcWaysInitialize,
               CPU_FEATURE_IIO_LLC_WAYS,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }

  if (IsCpuFeatureSupported (CPU_FEATURE_MTOI_WA)) {
    Status = RegisterCpuFeature (
               "MtoIWa",
               NULL,
               CpuMtoIWaSupport,
               CpuMtoIWaInitialize,
               CPU_FEATURE_MTOI_WA,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }

  if (IsCpuFeatureSupported (CPU_FEATURE_AC_SPLIT_LOCK)) {
    Status = RegisterCpuFeature (
               "AcSplitLock",
               NULL,
               AcExceptionOnSplitLockSupport,
               AcExceptionOnSplitLockInitialize,
               CPU_FEATURE_AC_SPLIT_LOCK,
               CPU_FEATURE_END
               );
    ASSERT_EFI_ERROR (Status);
  }

  return RETURN_SUCCESS;
}

