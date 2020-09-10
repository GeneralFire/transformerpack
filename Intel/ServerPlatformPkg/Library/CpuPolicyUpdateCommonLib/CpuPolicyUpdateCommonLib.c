/** @file
  Common library for PEI CPU Late Policy Update Library and
  PlatformCpuPolicy DXE driver.

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

#include <Library/CpuPolicyUpdateCommonLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/SetupLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/KtiApi.h>
#include <Library/X2ApicIdDetectLib.h>
#include <Library/PpmPolicyUpdateCommonLib.h>
#include <Platform.h>
#include <ProcessorPpmSetup.h>

// CPU C State Settings
#define C0_ENABLE                           0x00
#define C3_ENABLE                           0x02
#define C6_ENABLE                           0x03
#define C7_ENABLE                           0x04
#define C8_ENABLE                           0x05
#define C9_ENABLE                           0x06
#define C10_ENABLE                          0x07

/**
  This function provides common code for both PEI and DXE code to
  update CPU policy from setup

  @param[out] CpuPolicy         The policy to be updated from setup

  @retval EFI_SUCCESS           The policies are updated.

**/
EFI_STATUS
EFIAPI
UpdateCpuPolicyFromSetup (
  OUT CPU_POLICY_COMMON     *CpuPolicy
  )
{
  EFI_STATUS                Status;
  // APTIOV_SERVER_OVERRIDE_RC_START
  INTEL_SETUP_DATA                SetupData;
  
  ZeroMem (&SetupData, sizeof (INTEL_SETUP_DATA));
  // APTIOV_SERVER_OVERRIDE_RC_END
  Status = GetEntireConfig (&SetupData);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (SetupData.SocketConfig.PowerManagementConfig.TStateEnable && (SetupData.SocketConfig.PowerManagementConfig.OnDieThermalThrottling > 0)) {
    if (IsCpuAndRevision (CPU_SPRSP, REV_ALL) ||
        IsCpuAndRevision (CPU_GNRSP, REV_ALL) || IsCpuAndRevision (CPU_SRFSP, REV_ALL)) {
      CpuPolicy->CpuTStateEnable = FALSE;
    } else {
      CpuPolicy->CpuTStateEnable = (SetupData.SocketConfig.PowerManagementConfig.TStateEnable > 0) ? TRUE : FALSE;
      CpuPolicy->CpuClockModulationDutyCycle = SetupData.SocketConfig.PowerManagementConfig.OnDieThermalThrottling;
    }
  }
  CpuPolicy->CpuAesEnable = (SetupData.SocketConfig.SocketProcessorCoreConfiguration.AesEnable > 0) ? TRUE : FALSE;
  CpuPolicy->CpuFastStringEnable = (SetupData.SocketConfig.SocketProcessorCoreConfiguration.FastStringEnable > 0) ? TRUE : FALSE;
  CpuPolicy->CpuMaxCpuidValueLimitEnable = (SetupData.SocketConfig.SocketProcessorCoreConfiguration.CpuidMaxValue > 0) ? TRUE : FALSE;
  CpuPolicy->CpuMachineCheckEnable = (SetupData.SocketConfig.SocketProcessorCoreConfiguration.MachineCheckEnable > 0) ? TRUE : FALSE;
  CpuPolicy->CpuMonitorMwaitEnable = (SetupData.SocketConfig.PowerManagementConfig.MonitorMWait > 0) ? TRUE : FALSE;
  CpuPolicy->CpuVtEnable = (SetupData.SocketConfig.SocketProcessorCoreConfiguration.ProcessorVmxEnable > 0) ? TRUE : FALSE;
  CpuPolicy->CpuLtEnable = (SetupData.SocketConfig.SocketProcessorCoreConfiguration.ProcessorSmxEnable > 0) ? TRUE : FALSE;
  if (PcdGet8 (PcdEvMode) != 0) {
    CpuPolicy->CpuLtEnable = FALSE;
  }
  CpuPolicy->CpuX2ApicEnable = ((SetupData.SocketConfig.SocketProcessorCoreConfiguration.ProcessorX2apic > 0) || X2ApicIdDetect (NULL)) ? TRUE : FALSE;
  CpuPolicy->CpuEistEnable = (SetupData.SocketConfig.PowerManagementConfig.ProcessorEistEnable > 0) ? TRUE : FALSE;
  if (CpuPolicy->CpuEistEnable) {
    CpuPolicy->CpuTurboModeEnable = (SetupData.SocketConfig.PowerManagementConfig.TurboMode > 0) ? TRUE : FALSE;
    CpuPolicy->CpuHwCoordinationEnable = (SetupData.SocketConfig.PowerManagementConfig.ProcessorEistPsdFunc == 0) ? TRUE : FALSE;
    CpuPolicy->CpuBootPState = SetupData.SocketConfig.PowerManagementConfig.BootPState;
  }
  CpuPolicy->CpuPpinControlEnable = (SetupData.SocketConfig.SocketProcessorCoreConfiguration.PpinControl > 0) ? TRUE : FALSE;
  CpuPolicy->CpuPeciDownstreamWriteEnable = (SetupData.SocketConfig.SocketProcessorCoreConfiguration.PCIeDownStreamPECIWrite > 0) ? TRUE : FALSE;
  CpuPolicy->CpuL1NextPagePrefetcherDisable  = (SetupData.SocketConfig.SocketProcessorCoreConfiguration.CpuL1NextPagePrefetcherDisable > 0) ? TRUE : FALSE;
  CpuPolicy->CpuDcuPrefetcherEnable = (SetupData.SocketConfig.SocketProcessorCoreConfiguration.DCUStreamerPrefetcherEnable > 0) ? TRUE : FALSE;
  CpuPolicy->CpuIpPrefetcherEnable = (SetupData.SocketConfig.SocketProcessorCoreConfiguration.DCUIPPrefetcherEnable > 0) ? TRUE : FALSE;
  CpuPolicy->CpuMlcStreamerPrefetecherEnable = (SetupData.SocketConfig.SocketProcessorCoreConfiguration.MlcStreamerPrefetcherEnable > 0) ? TRUE : FALSE;
  CpuPolicy->CpuMlcSpatialPrefetcherEnable = (SetupData.SocketConfig.SocketProcessorCoreConfiguration.MlcSpatialPrefetcherEnable > 0) ? TRUE : FALSE;
  CpuPolicy->CpuAmpPrefetchEnable = (SetupData.SocketConfig.SocketProcessorCoreConfiguration.AmpPrefetchEnable > 0) ? TRUE : FALSE;
  CpuPolicy->CpuThreeStrikeCounterEnable = (SetupData.SocketConfig.SocketProcessorCoreConfiguration.ThreeStrikeTimer > 0) ? TRUE : FALSE;

  CpuPolicy->CpuCStateEnable = TRUE;

  CpuPolicy->CpuPackageCStateLimit = UpdatePkgCStatePlatform (SetupData.SocketConfig.PowerManagementConfig.PackageCState);
  if ((IsCpuAndRevision (CPU_SKX, REV_ALL)) || (IsCpuAndRevision (CPU_CLX, REV_ALL)) || (IsCpuAndRevision (CPU_CPX, REV_ALL))) {
    CpuPolicy->CpuC1AutoDemotionEnable = FALSE;
    CpuPolicy->CpuC1AutoUndemotionEnable = FALSE;
  } else {
    CpuPolicy->CpuC1AutoDemotionEnable = (SetupData.SocketConfig.PowerManagementConfig.C1AutoDemotion > 0) ? TRUE : FALSE;
    CpuPolicy->CpuC1AutoUndemotionEnable = (SetupData.SocketConfig.PowerManagementConfig.C1AutoUnDemotion > 0) ? TRUE : FALSE;
  }
  if (SetupData.SocketConfig.PowerManagementConfig.C6Enable == PPM_AUTO) {
    CpuPolicy->CpuCoreCStateValue = C6_ENABLE; //POR Default = Enabled
  } else {
    CpuPolicy->CpuCoreCStateValue = (SetupData.SocketConfig.PowerManagementConfig.C6Enable * C6_ENABLE);
  }
  CpuPolicy->CpuAcpiLvl2Addr = PCH_ACPI_BASE_ADDRESS + R_ACPI_LV2;

  CpuPolicy->CpuThermalManagementEnable = (SetupData.SocketConfig.PowerManagementConfig.EnableThermalMonitor > 0) ? TRUE : FALSE;
  if (CpuPolicy->CpuThermalManagementEnable) {
    CpuPolicy->CpuTccActivationOffset = SetupData.SocketConfig.PowerManagementConfig.TCCActivationOffset;
  }
  CpuPolicy->CpuDbpfEnable = (SetupData.SocketConfig.SocketProcessorCoreConfiguration.CpuDbpEnable > 0) ? TRUE : FALSE;
  CpuPolicy->CpuEnergyPerformanceBiasEnable = TRUE;
  CpuPolicy->CpuIioLlcWaysBitMask = SetupData.SocketConfig.SocketProcessorCoreConfiguration.IioLlcWaysMask;
  CpuPolicy->CpuExpandedIioLlcWaysBitMask = SetupData.SocketConfig.SocketProcessorCoreConfiguration.ExpandedIioLlcWaysMask;
  CpuPolicy->CpuRemoteWaysBitMask = SetupData.SocketConfig.SocketProcessorCoreConfiguration.RemoteWaysMask;
  CpuPolicy->CpuRrqCountThreshold = GetRequestThreshold ();
  if (IsCpuAndRevision (CPU_ICXSP, REV_R0) || IsCpuAndRevision (CPU_ICXSP, REV_L0) || IsCpuAndRevision (CPU_ICXSP, REV_C0)) {
    CpuPolicy->CpuMtoIWa = SetupData.SocketConfig.SocketProcessorCoreConfiguration.CpuMtoIWa;
  }
  CpuPolicy->RunCpuPpmInPei = (SetupData.SocketConfig.PowerManagementConfig.RunCpuPpmInPei > 0) ? TRUE : FALSE;
  CpuPolicy->AcExceptionOnSplitLockEnable = (SetupData.SocketConfig.SocketProcessorCoreConfiguration.AcExceptionOnSplitLockEnable > 0) ? TRUE : FALSE;

  return EFI_SUCCESS;
}
