/** @file
  Common CPU Power Management

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


#include <Library/CpuPpmLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/KtiApi.h>
#include <Library/CpuIpLib.h>

VOID
EFIAPI
CommonPpmProgramMsrs (
  EFI_CPU_PM_STRUCT         *ppm,
  PPM_POLICY_CONFIGURATION  *PpmPolicy,
  UINTN                     ProcessorNumber
)
{
  InitializePowerControl (ppm, &PpmPolicy->PowerCtl, ProcessorNumber);
  InitializeVrCurrentConfig (&PpmPolicy->AdvPwrMgtCtl, &PpmPolicy->PpoCurrentCfg, ProcessorNumber);
  InitializeRaplPowerLimitMsr (ppm, &PpmPolicy->TurboPowerLimit, ProcessorNumber);
  InitializePlatformPowerLimitMsr (ppm, &PpmPolicy->PsysConfig, ProcessorNumber);
  InitializeTurboRatioLimitCores (ppm, &PpmPolicy->TurboRatioLimit, &PpmPolicy->PpmXe, ProcessorNumber);
  InitializeEnergyPerformanceBias (&PpmPolicy->PerfBiasConfig, ProcessorNumber);
  InitializeCStateLatency (ppm, &PpmPolicy->PpmCst, ProcessorNumber);
  InitialzeHwpFeatures (ppm, &PpmPolicy->Hwpm, ProcessorNumber);
  AvxDisConfig (ppm, PpmPolicy->AvxSupport, ProcessorNumber);
  InitialzeConfigurePbf (ppm, &PpmPolicy->Hwpm, ProcessorNumber);
}

VOID
EFIAPI
CommonPpmProgramCsrs (
  EFI_CPU_PM_STRUCT         *ppm,
  PPM_POLICY_CONFIGURATION  *PpmPolicy,
  UINT32                    SocketPresentBitMap
)
{
  UINT8  SocketNumber;

  for (SocketNumber = 0; SocketNumber < MAX_SOCKET; SocketNumber++) {
    if (SocketPresentBitMap & (1 << SocketNumber)) {

      InitializeRaplPowerLimitCsr (SocketNumber, ppm, &PpmPolicy->TurboPowerLimit, PpmPolicy->C2C3TT);
      InitializePlatformPowerLimitCsr (SocketNumber, ppm, &PpmPolicy->PsysConfig);
      InitializePkgCstEntryCriteriaMask (SocketNumber, PpmPolicy);
      //ProgramCsrPcuBiosSpare2((EFI_CPU_PM_STRUCT *)ppm);  //take care in MPM (MemThrot.c)
      InitializePerfPlimitControl (SocketNumber, &PpmPolicy->PerPLimitCtl);
      InitializeDynamicPerfPowerCtl (SocketNumber, &PpmPolicy->DynamicPerPowerCtl);
      InitializeSapmControl (SocketNumber, &PpmPolicy->SapmCtl[SocketNumber]);
      InitializeSwLtrOverride (SocketNumber, PpmPolicy);
      InitializePmaxConfiguration (SocketNumber, ppm, &PpmPolicy->PmaxConfig);
      InitializeThermalReport (SocketNumber, &PpmPolicy->ThermalReport, PpmPolicy->CpuThermalManagement);
      InitializeProchotResponseRatio (SocketNumber, PpmPolicy->ProchotRatio);
      InitializeFastRaplDutyCycle (SocketNumber, PpmPolicy->FastRaplDutyCycle);
      InitializeHWPMMiscConfig (SocketNumber, &PpmPolicy->PowerCtl, &PpmPolicy->Hwpm);
      InitializeSstCpConfig (SocketNumber, PpmPolicy->Hwpm.SstCpSystemStatus, PpmPolicy->Hwpm.SstCpSetting);
      InitializeDynamicL1 (SocketNumber, PpmPolicy);
      if (IsSiliconWorkaroundEnabled ("ForceUncoreAndMeshRatio")) {
        if ((GetChopType  (SocketNumber) == TypeXcc)) {  // execute on XCC only
          ForceUncoreAndMeshRatio (ppm);
        }
      }

      MiscWorkaroundEnable (SocketNumber, &PpmPolicy->AdvPwrMgtCtl);

      InitializePkgcSaPsCriteria (SocketNumber, &PpmPolicy->PkgcCriteria);
      InitializeAVXICCPPreGrantLevel (SocketNumber, PpmPolicy->AvxLicensePreGrant, PpmPolicy->AvxIccpLevel);
      InitializeGpssTimer (SocketNumber, PpmPolicy->GpssTimer);
      InitializeLimca (SocketNumber);
      InitializeDcstLuts(SocketNumber);
    }
  }
}

