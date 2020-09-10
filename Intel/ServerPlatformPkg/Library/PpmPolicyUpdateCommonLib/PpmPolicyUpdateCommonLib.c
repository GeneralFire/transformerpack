/** @file
  Common library for PEI PPM Policy Update Library and
  PlatformCpuPolicy DXE driver

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


#include <PpmPolicyPeiDxeCommon.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SetupLib.h>
#include <Library/DebugLib.h>
#include <Library/CpuConfigLib.h>
#include <Library/CpuPpmLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/KtiApi.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/CpuEarlyDataLib.h>
#include <Library/PpmPolicyUpdateCommonLib.h>
#include <Cpu/CpuIds.h>
#include <Library/MemRcLib.h>
#include <ProcessorPpmSetup.h>


BOOLEAN GetAepPresentStatus (
  VOID
)
{
  UINT8   Socket;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (SocketPresent(Socket) && CheckSocketDcpmmPresent(Socket)) {
      DEBUG ((EFI_D_ERROR, "Common PPM policy update, PEI PMem: 1\n"));
      return TRUE;
    }
  }

  return FALSE;
}

UINT8 UpdatePkgCStatePlatform (
  UINT8                   SetupPkgCValue
  )
{
  UINT8                    PackageCStateSetting = 0;
  BOOLEAN                  AepPresent;

  if (SetupPkgCValue == PPM_AUTO) {
    PackageCStateSetting = C0_ENABLE;

    //
    // Override PkgC for ICXSP to be C6 non retention for RX, unless AEP DIMM is
    // present, then disable Pkg C. Enable C6 with retention for other revisions.
    //
    if (IsCpuAndRevision (CPU_ICXSP, REV_RX)) {
      AepPresent = GetAepPresentStatus ();
      if (AepPresent == FALSE) {
        PackageCStateSetting = C3_ENABLE;
        DEBUG ((EFI_D_ERROR, "ICXSP R0 (w/o PMem) detected, PkgC6 non-ret enabled.\n"));
      } else {
        DEBUG ((EFI_D_ERROR, "ICXSP R0 (with PMem) detected, PkgC disabled.\n"));
      }
    } else if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
      PackageCStateSetting = C3_ENABLE;
      DEBUG ((EFI_D_ERROR, "ICXSP L0 or later detected, PkgC6 non-ret enabled.\n"));
    }

    //
    // Override Setup Package C State setting to C6 for any system with CPX installed.
    //
    if (IsCpuAndRevision (CPU_CPX, REV_ALL)) {
      PackageCStateSetting = C6_ENABLE;
      DEBUG ((EFI_D_INFO, "CPX is detected, PkgC is enabled as 'C6(Retention) state'.\n"));
    }

    if (PackageCStateSetting == C0_ENABLE) {
      DEBUG ((EFI_D_INFO, "PkgC is set as 'C0/C1 state' for AUTO selection.\n"));
    }
  } else {
    PackageCStateSetting = SetupPkgCValue;
  }

  return PackageCStateSetting;
}


/**
  Initialize PBF policy.

  @param SetupData     Pointer to setup data.
  @param PpmPolicy     Pointer to PPM policy.
  @param CpuVarData    Pointer to CpuVarData HOB.

**/
VOID
InitializePbfPolicy (
  // APTIOV_SERVER_OVERRIDE_RC_START: To solve structure redefinition error in AMI.
  IN  INTEL_SETUP_DATA                *SetupData,
  // APTIOV_SERVER_OVERRIDE_RC_END: To solve structure redefinition error in AMI.          
  OUT PPM_POLICY_CONFIGURATION  *PpmPolicy,
  IN  CPU_VAR_DATA              *CpuVarData
  )
{
  UINT8         Socket;
  UINT8         PbfActived;
  UINT8         Level;

  PbfActived = (SetupData->SocketConfig.PowerManagementConfig.ProcessorActivePbf & CpuVarData->PbfCapableSystem);

  if (PbfActived) {
    PpmPolicy->Hwpm.ConfigurePbf = SetupData->SocketConfig.PowerManagementConfig.ProcessorConfigurePbf;

    //
    // [NOTE]: Current PBF only supports level 0.
    // This is for future support.
    // Level = CpuVarData->IssConfigTdpCurrentLevel;
    //
    Level = 0;

    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if (!SocketPresent (Socket)) {
        continue;
      }
      PpmPolicy->Hwpm.PbfHighPriCoreMap[Socket] = CpuVarData->PbfP1HiCoreMap[Socket][Level];
      PpmPolicy->Hwpm.PbfP1HighRatio[Socket] = CpuVarData->PbfP1HighRatio[Socket][Level];
      PpmPolicy->Hwpm.PbfP1LowRatio[Socket] = CpuVarData->PbfP1LowRatio[Socket][Level];
    }

    //
    // HWP Native with no legacy
    //
    PpmPolicy->Hwpm.HWPMEnable = HWP_MODE_NATIVE_NO_LEGACY;
    PpmPolicy->Hwpm.HWPMNative = 1;
    PpmPolicy->Hwpm.HWPMOOB = 0;
    PpmPolicy->Hwpm.HWPMInterrupt = SetupData->SocketConfig.PowerManagementConfig.ProcessorHWPMInterrupt;
    PpmPolicy->Hwpm.EPPEnable = SetupData->SocketConfig.PowerManagementConfig.ProcessorEPPEnable;
    //
    // Enable (1) SST-CP when PBF is enabled
    //
    PpmPolicy->Hwpm.SstCpSetting = 1;
    //
    // Ensure "Scalability" option under HWP is disabled (0)
    //
    PpmPolicy->Hwpm.Scalability = 0;

    DEBUG ((DEBUG_INFO, "\nPBF is actived:\n"));
    DEBUG ((DEBUG_INFO, "  PbfActived = %d\n  ConfigurePbf = %d\n",
            PbfActived,
            PpmPolicy->Hwpm.ConfigurePbf
            ));
    DEBUG ((DEBUG_INFO, "  HWPMEnable = %d\n  HWPMNative = %d\n  HWPMOOB = %d\n  HWPMInterrupt = %d\n  EPPEnable = %d\n  SstCpSetting = %d\n  Scalability = %d\n",
            PpmPolicy->Hwpm.HWPMEnable,
            PpmPolicy->Hwpm.HWPMNative,
            PpmPolicy->Hwpm.HWPMOOB,
            PpmPolicy->Hwpm.HWPMInterrupt,
            PpmPolicy->Hwpm.EPPEnable,
            PpmPolicy->Hwpm.SstCpSetting,
            PpmPolicy->Hwpm.Scalability
            ));
  }
}

/**

Routine Description:

  Common function to update PPM policy for PEI and DXE library or module

Returns:

**/
EFI_STATUS
EFIAPI
UpdatePpmPolicyFromSetup (
  OUT PPM_POLICY_CONFIGURATION *PpmPolicy
  )
{
  /*

    !!! Note: This function shall be common for both PEI and DXE module !!!

  */

  EFI_STATUS      Status;
  // APTIOV_SERVER_OVERRIDE_RC_START: To solve redefinition error
  INTEL_SETUP_DATA                SetupData;
  // APTIOV_SERVER_OVERRIDE_RC_END: To solve redefinition error
  UINT8           socket;
  CPU_VAR_DATA    *CpuVarData;
  UINT8           PackageCStateSetting;
  UINTN           i;

  // APTIOV_SERVER_OVERRIDE_RC_START: To solve redefinition error
  ZeroMem (&SetupData, sizeof (INTEL_SETUP_DATA));
  // APTIOV_SERVER_OVERRIDE_RC_END: To solve redefinition error
  Status = GetEntireConfig (&SetupData);
  if (EFI_ERROR (Status)) {
    ASSERT(FALSE);
    return Status;
  }

  //-------------------------------------------------------
  //
  //  Prepare some system info
  //
  //-------------------------------------------------------
  CpuVarData = GetCpuVarData ();
  if (CpuVarData == NULL) {
    ASSERT(FALSE);
    return EFI_NOT_FOUND;
  }

  PackageCStateSetting = UpdatePkgCStatePlatform (SetupData.SocketConfig.PowerManagementConfig.PackageCState);
  DEBUG ((EFI_D_ERROR, "Common PPM policy update, PackageCState:%x\n", PackageCStateSetting));

  PpmPolicy->RunCpuPpmInPei = SetupData.SocketConfig.PowerManagementConfig.RunCpuPpmInPei;
  DEBUG ((EFI_D_ERROR, "Run PPM in PEI: %x\n", PpmPolicy->RunCpuPpmInPei));

  PpmPolicy->CpuThermalManagement = SetupData.SocketConfig.PowerManagementConfig.EnableThermalMonitor ? 1 : 0;

  //-------------------------------------------------------
  //
  //  Update PPM policy from Setup or system info
  //
  //-------------------------------------------------------

  PpmPolicy->OverclockingLock  = SetupData.SocketConfig.PowerManagementConfig.OverclockingLock;
  PpmPolicy->AvxSupport        = SetupData.SocketConfig.PowerManagementConfig.AvxSupport;
  PpmPolicy->AvxLicensePreGrant = SetupData.SocketConfig.PowerManagementConfig.AvxLicensePreGrant;
  PpmPolicy->AvxIccpLevel      = SetupData.SocketConfig.PowerManagementConfig.AvxIccpLevel;
  PpmPolicy->GpssTimer         = SetupData.SocketConfig.PowerManagementConfig.GpssTimer;

  PpmPolicy->ThermalReport.ThermalMonitorStatusFilter           = SetupData.SocketConfig.PowerManagementConfig.ThermalMonitorStatusFilter;
  PpmPolicy->ThermalReport.ThermalMonitorStatusFilterTimeWindow = SetupData.SocketConfig.PowerManagementConfig.ThermalMonitorStatusFilterTimeWindow;

  PpmPolicy->FastRaplDutyCycle = SetupData.SocketConfig.PowerManagementConfig.FastRaplDutyCycle;

  //CSR SAPM CTL
  for( socket = 0; socket < MAX_SOCKET; socket++) {
    PpmPolicy->SapmCtl[socket].Iio0PkgcClkGateDis  = SetupData.SocketConfig.PowerManagementConfig.Iio0PkgcClkGateDis[socket];
    PpmPolicy->SapmCtl[socket].Iio1PkgcClkGateDis  = SetupData.SocketConfig.PowerManagementConfig.Iio1PkgcClkGateDis[socket];
    PpmPolicy->SapmCtl[socket].Iio2PkgcClkGateDis  = SetupData.SocketConfig.PowerManagementConfig.Iio2PkgcClkGateDis[socket];
    PpmPolicy->SapmCtl[socket].Kti01PkgcClkGateDis = SetupData.SocketConfig.PowerManagementConfig.Kti01PkgcClkGateDis[socket];
    PpmPolicy->SapmCtl[socket].Kti23PkgcClkGateDis = SetupData.SocketConfig.PowerManagementConfig.Kti23PkgcClkGateDis[socket];
    PpmPolicy->SapmCtl[socket].Kti45PkgcClkGateDis = SetupData.SocketConfig.PowerManagementConfig.Kti45PkgcClkGateDis[socket];
    PpmPolicy->SapmCtl[socket].Mc0PkgcClkGateDis   = SetupData.SocketConfig.PowerManagementConfig.Mc0PkgcClkGateDis[socket];
    PpmPolicy->SapmCtl[socket].Mc1PkgcClkGateDis   = SetupData.SocketConfig.PowerManagementConfig.Mc1PkgcClkGateDis[socket];
    PpmPolicy->SapmCtl[socket].P0pllOffEna         = SetupData.SocketConfig.PowerManagementConfig.P0pllOffEna[socket];
    PpmPolicy->SapmCtl[socket].P1pllOffEna         = SetupData.SocketConfig.PowerManagementConfig.P1pllOffEna[socket];
    PpmPolicy->SapmCtl[socket].P2pllOffEna         = SetupData.SocketConfig.PowerManagementConfig.P2pllOffEna[socket];
    PpmPolicy->SapmCtl[socket].Kti01pllOffEna      = SetupData.SocketConfig.PowerManagementConfig.Kti01pllOffEna[socket];
    PpmPolicy->SapmCtl[socket].Kti23pllOffEna      = SetupData.SocketConfig.PowerManagementConfig.Kti23pllOffEna[socket];
    PpmPolicy->SapmCtl[socket].Kti45pllOffEna      = SetupData.SocketConfig.PowerManagementConfig.Kti45pllOffEna[socket];
    PpmPolicy->SapmCtl[socket].Mc0pllOffEna        = SetupData.SocketConfig.PowerManagementConfig.Mc0pllOffEna[socket];
    PpmPolicy->SapmCtl[socket].Mc1pllOffEna        = SetupData.SocketConfig.PowerManagementConfig.Mc1pllOffEna[socket];
    PpmPolicy->SapmCtl[socket].SetvidDecayDisable  = SetupData.SocketConfig.PowerManagementConfig.SetvidDecayDisable[socket];
    PpmPolicy->SapmCtl[socket].SapmCtlLock         = SetupData.SocketConfig.PowerManagementConfig.SapmCtlLock[socket];

    if (SetupData.SocketConfig.MemoryConfig.OppSrefEn == 1) {
      PpmPolicy->SapmCtl[socket].Mc0PkgcIoVolRedDis  = 1;
      PpmPolicy->SapmCtl[socket].Mc1PkgcIoVolRedDis  = 1;
      PpmPolicy->SapmCtl[socket].Mc0PkgcDigVolRedDis = 1;
      PpmPolicy->SapmCtl[socket].Mc1PkgcDigVolRedDis = 1;
    }
  }

  //
  // PMAX Detector Config
  //
  PpmPolicy->PmaxConfig.PmaxDetector         = SetupData.SocketConfig.PowerManagementConfig.PmaxDetector;
  PpmPolicy->PmaxConfig.EnhancedPmaxDetector = SetupData.SocketConfig.PowerManagementConfig.EnhancedPmaxDetector;
  PpmPolicy->PmaxConfig.PmaxAutoAdjustment   = SetupData.SocketConfig.PowerManagementConfig.PmaxAutoAdjustment;
  PpmPolicy->PmaxConfig.PmaxSign             = SetupData.SocketConfig.PowerManagementConfig.PmaxSign;
  PpmPolicy->PmaxConfig.PmaxOffset           = SetupData.SocketConfig.PowerManagementConfig.PmaxOffset;
  PpmPolicy->PmaxConfig.PmaxOffsetNegative   = SetupData.SocketConfig.PowerManagementConfig.PmaxOffsetNegative;
  PpmPolicy->PmaxConfig.PmaxTriggerSetup     = SetupData.SocketConfig.PowerManagementConfig.PmaxTriggerSetup;

  //PmaxLoadLine (SPR):
  //   0      Read over SVID
  //   1      0.50mohm platform LL
  //   2      0.85mohm platform LL
  PpmPolicy->PmaxConfig.PmaxLoadLine         = SetupData.SocketConfig.PowerManagementConfig.PmaxLoadLine;

  //
  // PERF_P_LIMIT_CONTROL (CSR 1:30:2:0xE4)
  //
  PpmPolicy->PerPLimitCtl.PerfPLmtThshld         = SetupData.SocketConfig.PowerManagementConfig.PerfPLmtThshld;
  PpmPolicy->PerPLimitCtl.PerfPLimitClipC        = SetupData.SocketConfig.PowerManagementConfig.PerfPLimitClipC;
  PpmPolicy->PerPLimitCtl.PerfPlimitDifferential = SetupData.SocketConfig.PowerManagementConfig.PerfPlimitDifferential;
  PpmPolicy->PerPLimitCtl.PerfPLimitEn           = SetupData.SocketConfig.PowerManagementConfig.PerfPLimitEn;

  PpmPolicy->DynamicIss = SetupData.SocketConfig.PowerManagementConfig.DynamicIss;
  PpmPolicy->IssCapableSystem = CpuVarData->IssCapableSystem;
  PpmPolicy->ConfigTDPLevel = CpuVarData->IssConfigTdpCurrentLevel;
  PpmPolicy->ConfigTDPLock = SetupData.SocketConfig.PowerManagementConfig.ConfigTdpLock;

  PpmPolicy->PkgcCriteria.EnablePkgcCriteria     = SetupData.SocketConfig.PowerManagementConfig.EnablePkgcCriteria;

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    PpmPolicy->PmaxConfig.BasePackageTdp[socket] = CpuVarData->IssConfigTdpPower[socket][0];
    PpmPolicy->CurrentPackageTdp[socket] = CpuVarData->IssConfigTdpPower[socket][PpmPolicy->ConfigTDPLevel];

    // NOTE: EnablePkgCCriteria knobs only for 10nm products
    if ((IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) == 0) {
      PpmPolicy->PkgcCriteria.EnablePkgCCriteriaKti[socket]          = SetupData.SocketConfig.PowerManagementConfig.EnablePkgCCriteriaKti[socket];
      PpmPolicy->PkgcCriteria.EnablePkgCCriteriaRlink[socket]        = SetupData.SocketConfig.PowerManagementConfig.EnablePkgCCriteriaRlink[socket];
      PpmPolicy->PkgcCriteria.EnablePkgCCriteriaFxr[socket]          = SetupData.SocketConfig.PowerManagementConfig.EnablePkgCCriteriaFxr[socket];
      PpmPolicy->PkgcCriteria.EnablePkgCCriteriaMcddr[socket]        = SetupData.SocketConfig.PowerManagementConfig.EnablePkgCCriteriaMcddr[socket];
      PpmPolicy->PkgcCriteria.EnablePkgCCriteriaHbm[socket]          = SetupData.SocketConfig.PowerManagementConfig.EnablePkgCCriteriaHbm[socket];

      PpmPolicy->PkgcCriteria.EnablePkgCCriteriaIio[socket]          = SetupData.SocketConfig.PowerManagementConfig.EnablePkgCCriteriaIio[socket];
      PpmPolicy->PkgcCriteria.EnablePkgCCriteriaHqm[socket]          = SetupData.SocketConfig.PowerManagementConfig.EnablePkgCCriteriaHqm[socket];
      PpmPolicy->PkgcCriteria.EnablePkgCCriteriaNac[socket]          = SetupData.SocketConfig.PowerManagementConfig.EnablePkgCCriteriaNac[socket];
      PpmPolicy->PkgcCriteria.EnablePkgCCriteriaTip[socket]          = SetupData.SocketConfig.PowerManagementConfig.EnablePkgCCriteriaTip[socket];
      PpmPolicy->PkgcCriteria.EnablePkgCCriteriaMdfs[socket]         = SetupData.SocketConfig.PowerManagementConfig.EnablePkgCCriteriaMdfs[socket];
      PpmPolicy->PkgcCriteria.EnablePkgCCriteriaHcx[socket]          = SetupData.SocketConfig.PowerManagementConfig.EnablePkgCCriteriaHcx[socket];
      PpmPolicy->PkgcCriteria.EnablePkgCCriteriaDino[socket]         = SetupData.SocketConfig.PowerManagementConfig.EnablePkgCCriteriaDino[socket];
      PpmPolicy->PkgcCriteria.PkgCCriteriaLogicalIpType[socket]      = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaLogicalIpType[socket];
      PpmPolicy->PkgcCriteria.PkgCCriteriaLogicalIpTypeMcddr[socket] = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaLogicalIpTypeMcddr[socket];
      PpmPolicy->PkgcCriteria.PkgCCriteriaLogicalIpTypeHbm[socket]   = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaLogicalIpTypeHbm[socket];
      PpmPolicy->PkgcCriteria.PkgCCriteriaLogicalIpTypeIio[socket]   = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaLogicalIpTypeIio[socket];
      PpmPolicy->PkgcCriteria.PkgCCriteriaInstanceNoKti[socket]      = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaInstanceNoKti[socket];
      PpmPolicy->PkgcCriteria.EnableLinkInL1Kti[socket]              = SetupData.SocketConfig.PowerManagementConfig.EnableLinkInL1Kti[socket];

      PpmPolicy->PkgcCriteria.PkgCCriteriaInstanceNoRlink[socket]    = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaInstanceNoRlink[socket];
      PpmPolicy->PkgcCriteria.EnableLinkInL1Rlink[socket]            = SetupData.SocketConfig.PowerManagementConfig.EnableLinkInL1Rlink[socket];
      PpmPolicy->PkgcCriteria.PkgCCriteriaInstanceNoFxr[socket]      = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaInstanceNoFxr[socket];
      PpmPolicy->PkgcCriteria.PkgcCriteraPsMaskFxr[socket]           = SetupData.SocketConfig.PowerManagementConfig.PkgcCriteraPsMaskFxr[socket];
      PpmPolicy->PkgcCriteria.PkgCCriteriaAllowedPsMaskFxr[socket]   = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaAllowedPsMaskFxr[socket];
      PpmPolicy->PkgcCriteria.PkgCCriteriaInstanceNoMcddr[socket]    = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaInstanceNoMcddr[socket];
      PpmPolicy->PkgcCriteria.PkgcCriteriaPsOptionMcddr[socket]      = SetupData.SocketConfig.PowerManagementConfig.PkgcCriteriaPsOptionMcddr[socket];
      PpmPolicy->PkgcCriteria.PkgCCriteriaInstanceNoHbm[socket]      = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaInstanceNoHbm[socket];
      PpmPolicy->PkgcCriteria.PkgcCriteriaPsOptionHbm[socket]        = SetupData.SocketConfig.PowerManagementConfig.PkgcCriteriaPsOptionHbm[socket];

      PpmPolicy->PkgcCriteria.PkgCCriteriaInstanceNoIio[socket]      = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaInstanceNoIio[socket];
      PpmPolicy->PkgcCriteria.EnableLinkInL1Iio[socket]              = SetupData.SocketConfig.PowerManagementConfig.EnableLinkInL1Iio[socket];
      PpmPolicy->PkgcCriteria.PkgCCriteriaInstanceNoHqm[socket]      = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaInstanceNoHqm[socket];
      PpmPolicy->PkgcCriteria.PkgcCriteraPsMaskHqm[socket]           = SetupData.SocketConfig.PowerManagementConfig.PkgcCriteraPsMaskHqm[socket];
      PpmPolicy->PkgcCriteria.PkgCCriteriaAllowedPsMaskHqm[socket]   = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaAllowedPsMaskHqm[socket];
      PpmPolicy->PkgcCriteria.PkgCCriteriaInstanceNoNac[socket]      = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaInstanceNoNac[socket];
      PpmPolicy->PkgcCriteria.PkgcCriteraPsMaskNac[socket]           = SetupData.SocketConfig.PowerManagementConfig.PkgcCriteraPsMaskNac[socket];
      PpmPolicy->PkgcCriteria.PkgCCriteriaAllowedPsMaskNac[socket]   = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaAllowedPsMaskNac[socket];
      PpmPolicy->PkgcCriteria.PkgCCriteriaInstanceNoTip[socket]      = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaInstanceNoTip[socket];
      PpmPolicy->PkgcCriteria.PkgcCriteraPsMaskTip[socket]           = SetupData.SocketConfig.PowerManagementConfig.PkgcCriteraPsMaskTip[socket];
      PpmPolicy->PkgcCriteria.PkgCCriteriaAllowedPsMaskTip[socket]   = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaAllowedPsMaskTip[socket];
      PpmPolicy->PkgcCriteria.PkgCCriteriaInstanceNoMdfs[socket]     = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaInstanceNoMdfs[socket];
      PpmPolicy->PkgcCriteria.AllowLpStateMdfs[socket]               = SetupData.SocketConfig.PowerManagementConfig.AllowLpStateMdfs[socket];
      PpmPolicy->PkgcCriteria.PkgCCriteriaInstanceNoHcx[socket]      = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaInstanceNoHcx[socket];
      PpmPolicy->PkgcCriteria.PkgcCriteraPsMaskHcx[socket]           = SetupData.SocketConfig.PowerManagementConfig.PkgcCriteraPsMaskHcx[socket];
      PpmPolicy->PkgcCriteria.PkgCCriteriaAllowedPsMaskHcx[socket]   = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaAllowedPsMaskHcx[socket];
      PpmPolicy->PkgcCriteria.PkgCCriteriaInstanceNoDino[socket]     = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaInstanceNoDino[socket];
      PpmPolicy->PkgcCriteria.PkgcCriteraPsMaskDino[socket]          = SetupData.SocketConfig.PowerManagementConfig.PkgcCriteraPsMaskDino[socket];
      PpmPolicy->PkgcCriteria.PkgCCriteriaAllowedPsMaskDino[socket]  = SetupData.SocketConfig.PowerManagementConfig.PkgCCriteriaAllowedPsMaskDino[socket];
    }

    if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
      PpmPolicy->PpmCst.PkgCstEntryCriteriaMaskKti[socket] = (SetupData.SocketConfig.PowerManagementConfig.Kti0In[socket] |
                               (SetupData.SocketConfig.PowerManagementConfig.Kti1In[socket] << 1) |
                               (SetupData.SocketConfig.PowerManagementConfig.Kti2In[socket] << 2) );

      if (SetupData.SocketConfig.PowerManagementConfig.PcieIio0In[socket]) {
        PpmPolicy->PpmCst.PkgCstEntryCriteriaMaskPcie[socket] |= SET_PCIEx_MASK;
      }
      if (SetupData.SocketConfig.PowerManagementConfig.PcieIio1In[socket]) {
        PpmPolicy->PpmCst.PkgCstEntryCriteriaMaskPcie[socket] |= (SET_PCIEx_MASK << 4);
      }
      if (SetupData.SocketConfig.PowerManagementConfig.PcieIio2In[socket]) {
        PpmPolicy->PpmCst.PkgCstEntryCriteriaMaskPcie[socket] |= (SET_PCIEx_MASK << 8);
      }
      if (SetupData.SocketConfig.PowerManagementConfig.PcieIio3In[socket]) {
        PpmPolicy->PpmCst.PkgCstEntryCriteriaMaskPcie[socket] |= (SET_PCIEx_MASK << 12);
      }
      if (SetupData.SocketConfig.PowerManagementConfig.PcieIio4In[socket]) {
        PpmPolicy->PpmCst.PkgCstEntryCriteriaMaskPcie[socket] |= (SET_PCIEx_MASK << 16);
      }
      if (SetupData.SocketConfig.PowerManagementConfig.PcieIio5In[socket]) {
        PpmPolicy->PpmCst.PkgCstEntryCriteriaMaskPcie[socket] |= (SET_PCIEx_MASK << 20);
      }
    }
  }

  PpmPolicy->AdvPwrMgtCtl.SapmctlValCtl = SetupData.SocketConfig.PowerManagementConfig.SapmctlValCtl;
  PpmPolicy->AdvPwrMgtCtl.MsrLock       = 1;
  PpmPolicy->AdvPwrMgtCtl.PkgCstEntryValCtl    = 1;
  PpmPolicy->AdvPwrMgtCtl.CurrentConfig        = SetupData.SocketConfig.PowerManagementConfig.CurrentConfig;
  PpmPolicy->AdvPwrMgtCtl.PcodeWdogTimerEn     = SetupData.SocketConfig.PowerManagementConfig.PcodeWdogTimerEn;
  PpmPolicy->AdvPwrMgtCtl.DramRaplPwrLimitLockCsr     = SetupData.SocketConfig.MemoryConfig.DramRaplPwrLimitLockCsr;
  PpmPolicy->TurboPowerLimit.TurboLimitCsrLock = SetupData.SocketConfig.PowerManagementConfig.TurboPowerLimitCsrLock;



  PpmPolicy->AdvPwrMgtCtl.MsrPkgCstConfigControlLock = SetupData.SocketConfig.SocketProcessorCoreConfiguration.ProcessorMsrPkgCstConfigControlLock ? 1 : 0;

  if ((PackageCStateSetting > 0) && SetupData.SocketConfig.PowerManagementConfig.DynamicL1) {
    PpmPolicy->AdvPwrMgtCtl.DynamicL1Disable = 1;
  }

  if (SetupData.SocketConfig.PowerManagementConfig.VccSAandVccIOdisable) {
    PpmPolicy->AdvPwrMgtCtl.VccsaVccioDisable = 1;
  }

  PpmPolicy->AdvPwrMgtCtl.SwLtrOvrdCtl = SetupData.SocketConfig.PowerManagementConfig.SwLtrOvrdCtl;

  if (SetupData.SocketConfig.PowerManagementConfig.ProcessorEistEnable == 0) {
    PpmPolicy->PowerCtl.EeTurboDisable = 1;
  }

  PpmPolicy->PowerCtl.PkgCLatNeg     = SetupData.SocketConfig.PowerManagementConfig.PkgCLatNeg;
  PpmPolicy->PowerCtl.LtrSwInput     = SetupData.SocketConfig.PowerManagementConfig.LTRSwInput;
  PpmPolicy->PowerCtl.PwrPerfSwitch  = SetupData.SocketConfig.PowerManagementConfig.PwrPerfSwitch;
  PpmPolicy->PowerCtl.SapmControl    = SetupData.SocketConfig.PowerManagementConfig.SAPMControl;
  PpmPolicy->PowerCtl.EeTurboDisable = SetupData.SocketConfig.PowerManagementConfig.EETurboDisable;
  PpmPolicy->PowerCtl.ProchotLock    = 1;


  PpmPolicy->PowerCtl.C1eEnable      = SetupData.SocketConfig.PowerManagementConfig.ProcessorC1eEnable;
  PpmPolicy->PowerCtl.SetvidDecayDisable = SetupData.SocketConfig.PowerManagementConfig.SetvidDecayDisable[0];

  PpmPolicy->PowerCtl.PwrPerfTuning  = SetupData.SocketConfig.PowerManagementConfig.PwrPerfTuning;

  // Sanity check for PwrPerfTuning. Correct them to be default if needed
  switch (SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMEnable) {
    case HWP_MODE_DISABLED: // fall through
    case HWP_MODE_NATIVE:
      // HWP Disabled and Native mode: OS, BIOS and PECI control are all available
      break;

    case HWP_MODE_OOB:
      // Only BIOS and PECI control available, PECI control is default
      if ( (PpmPolicy->PowerCtl.PwrPerfTuning != PWR_PERF_TUNING_BIOS_CONTROL)
           && (PpmPolicy->PowerCtl.PwrPerfTuning != PWR_PERF_TUNING_PECI_CONTROL) ) {
        if (IsCpuAndRevision (CPU_CPX, REV_ALL)) {
          // PECI control isn't available on CPX
          PpmPolicy->PowerCtl.PwrPerfTuning = PWR_PERF_TUNING_BIOS_CONTROL;
        } else {
          PpmPolicy->PowerCtl.PwrPerfTuning = PWR_PERF_TUNING_PECI_CONTROL;
        }
      }
      break;

    case HWP_MODE_NATIVE_NO_LEGACY:
      // Only BIOS and PECI control available, BIOS control is default
      if ( (PpmPolicy->PowerCtl.PwrPerfTuning != PWR_PERF_TUNING_BIOS_CONTROL)
           && (PpmPolicy->PowerCtl.PwrPerfTuning != PWR_PERF_TUNING_PECI_CONTROL) ) {
        PpmPolicy->PowerCtl.PwrPerfTuning = PWR_PERF_TUNING_BIOS_CONTROL;
      }

      //
      // On CPX, for HWPM Native mode with no Legacy support, set the Power Performance
      // tuning bit as it is set by the corresponding Setup question.
      //
      if (IsCpuAndRevision (CPU_CPX, REV_ALL)) {
        PpmPolicy->PowerCtl.PwrPerfTuning = SetupData.SocketConfig.PowerManagementConfig.PwrPerfTuning;
      }

      break;

    default:
      // something broken
      SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMEnable = HWP_MODE_DISABLED;
      PpmPolicy->PowerCtl.PwrPerfTuning = PWR_PERF_TUNING_OS_CONTROL;

      DEBUG((EFI_D_ERROR, "HWP mode illegal\n"));
      ASSERT(FALSE);
  }

  //  From Setup:
  //    0: Output only
  //    1: Disable
  //    2: Bi-Directional
  //    3: Input Only
  //
  //  SPR supports only 1 (Disable) and 3 (Input Only)
  //
  if ((SetupData.SocketConfig.PowerManagementConfig.EnableProcHot & 1) == 0) {
    PpmPolicy->PowerCtl.ProchotOutputDisable = 0;
  } else {
    PpmPolicy->PowerCtl.ProchotOutputDisable = 1;
  }

  if ((SetupData.SocketConfig.PowerManagementConfig.EnableProcHot & 2) == 0) {
    PpmPolicy->PowerCtl.BidirProchotEnable = 0;
  } else {
    PpmPolicy->PowerCtl.BidirProchotEnable = 1;
  }

  //
  // PROCHOT_RESPONSE_RATIO (CSR 1:30:2:0xB0)
  //
  PpmPolicy->ProchotRatio = SetupData.SocketConfig.PowerManagementConfig.ProchotResponseRatio;

  PpmPolicy->PpoCurrentCfg.PpcccLock = SetupData.SocketConfig.PowerManagementConfig.PpcccLock;
  PpmPolicy->PpoCurrentCfg.CurrentLimit = SetupData.SocketConfig.PowerManagementConfig.CurrentLimit;

  // MSR_PACKAGE_POWER_LIMIT 0x610
  // CSR_TURBO_POWER_LIMIT 1:30:0:0xe8
  PpmPolicy->TurboPowerLimit.TurboPowerLimitLock  = SetupData.SocketConfig.PowerManagementConfig.TurboPowerLimitLock;
  PpmPolicy->TurboPowerLimit.PowerLimit1Power     = SetupData.SocketConfig.PowerManagementConfig.PowerLimit1Power;
  PpmPolicy->TurboPowerLimit.PowerLimit1Time      = SetupData.SocketConfig.PowerManagementConfig.PowerLimit1Time;
  PpmPolicy->TurboPowerLimit.PowerLimit1En        = SetupData.SocketConfig.PowerManagementConfig.PowerLimit1En;
  PpmPolicy->TurboPowerLimit.PowerLimit2Power     = SetupData.SocketConfig.PowerManagementConfig.PowerLimit2Power;
  PpmPolicy->TurboPowerLimit.PowerLimit2Time      = SetupData.SocketConfig.PowerManagementConfig.PowerLimit2Time;
  PpmPolicy->TurboPowerLimit.PowerLimit2En        = SetupData.SocketConfig.PowerManagementConfig.PowerLimit2En;
  if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    PpmPolicy->TurboPowerLimit.PkgClmpLim1          = 0;
    PpmPolicy->TurboPowerLimit.PkgClmpLim2          = 0;
  } else {
    PpmPolicy->TurboPowerLimit.PkgClmpLim1          = 1;
    PpmPolicy->TurboPowerLimit.PkgClmpLim2          = 1;
  }

  //
  // Psys - Platform power limit config
  //
  PpmPolicy->PsysConfig.PowerLimitMsrLock     = SetupData.SocketConfig.PowerManagementConfig.PsysMsrLock;
  PpmPolicy->PsysConfig.PowerLimitCsrLock     = SetupData.SocketConfig.PowerManagementConfig.PsysCsrLock;
  PpmPolicy->PsysConfig.PowerLimit1Power      = SetupData.SocketConfig.PowerManagementConfig.PsysPowerLimit1Power;
  PpmPolicy->PsysConfig.PowerLimit1Time       = SetupData.SocketConfig.PowerManagementConfig.PsysPowerLimit1Time;
  PpmPolicy->PsysConfig.PowerLimit1En         = SetupData.SocketConfig.PowerManagementConfig.PsysPowerLimit1En;
  PpmPolicy->PsysConfig.PowerLimit2Power      = SetupData.SocketConfig.PowerManagementConfig.PsysPowerLimit2Power;
  PpmPolicy->PsysConfig.PowerLimit2Time       = SetupData.SocketConfig.PowerManagementConfig.PsysPowerLimit2Time;
  PpmPolicy->PsysConfig.PowerLimit2En         = SetupData.SocketConfig.PowerManagementConfig.PsysPowerLimit2En;
  PpmPolicy->PsysConfig.ClampingLim1          = 1;
  PpmPolicy->PsysConfig.ClampingLim2          = 1;
  PpmPolicy->PsysConfig.OverridePplMaxMin     = SetupData.SocketConfig.PowerManagementConfig.OverridePplMaxMin;
  PpmPolicy->PsysConfig.MinPpl1Power          = SetupData.SocketConfig.PowerManagementConfig.MinPpl1Power;
  PpmPolicy->PsysConfig.MaxPpl1Power          = SetupData.SocketConfig.PowerManagementConfig.MaxPpl1Power;
  PpmPolicy->PsysConfig.MaxPpl2Power          = SetupData.SocketConfig.PowerManagementConfig.MaxPpl2Power;
  PpmPolicy->PsysConfig.MaxPplTimeWindow      = SetupData.SocketConfig.PowerManagementConfig.MaxPplTimeWindow;

  PpmPolicy->PsysConfig.OverridePsysMaster    = SetupData.SocketConfig.PowerManagementConfig.OverridePsysMaster;
  if (PpmPolicy->PsysConfig.OverridePsysMaster) {
    CopyMem (
        PpmPolicy->PsysConfig.PsysMaster,
        SetupData.SocketConfig.PowerManagementConfig.PsysMaster,
        sizeof(PpmPolicy->PsysConfig.PsysMaster));
    CopyMem (
        PpmPolicy->PsysConfig.PsysSlaveBitmap,
        SetupData.SocketConfig.PowerManagementConfig.PsysSlaveBitmap,
        sizeof(PpmPolicy->PsysConfig.PsysSlaveBitmap));
  }

  // DYNAMIC_PERF_POWER_CTL (CSR 1:30:2:0x64)
  PpmPolicy->DynamicPerPowerCtl.UncrPerfPlmtOvrdEn   = SetupData.SocketConfig.PowerManagementConfig.UncrPerfPlmtOvrdEn;
  PpmPolicy->DynamicPerPowerCtl.EetOverrideEn        = SetupData.SocketConfig.PowerManagementConfig.EetOverrideEn;
  PpmPolicy->DynamicPerPowerCtl.IoBwPlmtOvrdEn       = SetupData.SocketConfig.PowerManagementConfig.IoBwPlmtOvrdEn;
  PpmPolicy->DynamicPerPowerCtl.IomApmOvrdEn         = SetupData.SocketConfig.PowerManagementConfig.IomApmOvrdEn;
  PpmPolicy->DynamicPerPowerCtl.KtiApmOvrdEn         = SetupData.SocketConfig.PowerManagementConfig.KtiApmOvrdEn;

  //
  // CSR_PCIE_ILTR_OVRD (CSR 1:30:1:0xFC)
  // SW_LTR_OVRD (MSR 0xa02) -- not used
  //
  PpmPolicy->PcieIltrOvrd.SnpLatVld     = SetupData.SocketConfig.PowerManagementConfig.SnpLatVld;
  PpmPolicy->PcieIltrOvrd.SnpLatOvrd    = SetupData.SocketConfig.PowerManagementConfig.SnpLatOvrd;
  PpmPolicy->PcieIltrOvrd.SnpLatMult    = SetupData.SocketConfig.PowerManagementConfig.SnpLatMult;
  PpmPolicy->PcieIltrOvrd.SnpLatVal     = SetupData.SocketConfig.PowerManagementConfig.SnpLatVal;
  PpmPolicy->PcieIltrOvrd.NonSnpLatVld  = SetupData.SocketConfig.PowerManagementConfig.NonSnpLatVld;
  PpmPolicy->PcieIltrOvrd.NonSnpLatOvrd = SetupData.SocketConfig.PowerManagementConfig.NonSnpLatOvrd;
  PpmPolicy->PcieIltrOvrd.NonSnpLatMult = SetupData.SocketConfig.PowerManagementConfig.NonSnpLatMult;
  PpmPolicy->PcieIltrOvrd.NonSnpLatVal  = SetupData.SocketConfig.PowerManagementConfig.NonSnpLatVal;

  for(i = 0; i < NUM_TURBO_RATIO_GROUP; i++) {
    PpmPolicy->TurboRatioLimit.RatioLimitRatio[i] = SetupData.SocketConfig.PowerManagementConfig.TurboRatioLimitRatio[i];

    PpmPolicy->TurboRatioLimit.RatioLimitRatioMask[i] = 0xFF;
    if (SetupData.SocketConfig.PowerManagementConfig.TurboRatioLimitRatio[i] > 0) {
      PpmPolicy->TurboRatioLimit.RatioLimitRatioMask[i] = 0;
    }

    PpmPolicy->TurboRatioLimit.RatioLimitCoresMask[i] = 0xFF;
    PpmPolicy->TurboRatioLimit.RatioLimitCores[i] = 0;
    if (SetupData.SocketConfig.PowerManagementConfig.TurboRatioLimitCores[i] != 0xFF) {
      PpmPolicy->TurboRatioLimit.RatioLimitCoresMask[i] = 0;
      PpmPolicy->TurboRatioLimit.RatioLimitCores[i] = SetupData.SocketConfig.PowerManagementConfig.TurboRatioLimitCores[i];
    }
  }

  //
  // ENERGY_PERF_BIAS_CONFIG (MSR 0xA01)
  //
  PpmPolicy->PerfBiasConfig.EngAvgTimeWdw1 = SetupData.SocketConfig.PowerManagementConfig.EngAvgTimeWdw1;
  PpmPolicy->PerfBiasConfig.P0TtlTimeLow1  = SetupData.SocketConfig.PowerManagementConfig.P0TtlTimeLow1;
  PpmPolicy->PerfBiasConfig.P0TtlTimeHigh1 = SetupData.SocketConfig.PowerManagementConfig.P0TtlTimeHigh1;
  PpmPolicy->PerfBiasConfig.AltEngPerfBIAS = SetupData.SocketConfig.PowerManagementConfig.AltEngPerfBIAS;
  PpmPolicy->PerfBiasConfig.WorkLdConfig   = SetupData.SocketConfig.PowerManagementConfig.WorkLdConfig;

  //
  //ProcessorHWPM-init as disabled.
  //
  PpmPolicy->Hwpm.HWPMNative    = 0;
  PpmPolicy->Hwpm.HWPMOOB       = 0;
  PpmPolicy->Hwpm.HWPMEnable    = SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMEnable;
  PpmPolicy->Hwpm.HWPMInterrupt = SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMInterrupt;
  PpmPolicy->Hwpm.EPPEnable     = SetupData.SocketConfig.PowerManagementConfig.ProcessorEPPEnable;
  PpmPolicy->Hwpm.EPPProfile    = SetupData.SocketConfig.PowerManagementConfig.ProcessorEppProfile;

  PpmPolicy->Hwpm.APSrocketing          = SetupData.SocketConfig.PowerManagementConfig.ProcessorAPSrocketing;
  PpmPolicy->Hwpm.Scalability           = SetupData.SocketConfig.PowerManagementConfig.ProcessorScalability;
  PpmPolicy->Hwpm.OutofBandAlternateEPB = SetupData.SocketConfig.PowerManagementConfig.ProcessorOutofBandAlternateEPB;

  if ((IsCpuAndRevision (CPU_SPRSP, REV_ALL)) || (IsCpuAndRevision (CPU_GNRSP, REV_ALL)) || (IsCpuAndRevision (CPU_SRFSP, REV_ALL))) {
    PpmPolicy->Hwpm.SstCpSetting  = SetupData.SocketConfig.PowerManagementConfig.ProcessorSstCpSetting;
  } else {
    PpmPolicy->Hwpm.SstCpSetting  = SetupData.SocketConfig.PowerManagementConfig.ProcessorRaplPrioritization;
  }

  PpmPolicy->Hwpm.SstCpSystemStatus = CpuVarData->SstCpSystemStatus;

  if ((SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMEnable == HWP_MODE_NATIVE) ||
     (SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMEnable == HWP_MODE_NATIVE_NO_LEGACY)) {
    PpmPolicy->Hwpm.HWPMNative  = SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMEnable;
  } else if (SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMEnable == HWP_MODE_OOB){
    PpmPolicy->Hwpm.HWPMOOB     = SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMEnable;
    PpmPolicy->Hwpm.HWPMInterrupt = 0;
  } else if (SetupData.SocketConfig.PowerManagementConfig.ProcessorHWPMEnable == HWP_MODE_DISABLED ){
    PpmPolicy->Hwpm.HWPMNative    = 0;
    PpmPolicy->Hwpm.HWPMOOB       = 0;
    PpmPolicy->Hwpm.HWPMInterrupt = 0;
    PpmPolicy->Hwpm.EPPEnable     = 0;
    PpmPolicy->Hwpm.SstCpSetting = 0;   //Disable
  }

  //
  // Initialize PBF policy after HWP policy is initialized above.
  //
  InitializePbfPolicy (&SetupData, PpmPolicy, CpuVarData);

  for (i = 0; i < NUM_CST_LAT_MSR; i++) {    // CStateLatencyCtrl CSRs
    PpmPolicy->PpmCst.LatencyCtrl[i].Valid      = SetupData.SocketConfig.PowerManagementConfig.CStateLatencyCtrlValid[i];
    PpmPolicy->PpmCst.LatencyCtrl[i].Multiplier = SetupData.SocketConfig.PowerManagementConfig.CStateLatencyCtrlMultiplier[i];
    PpmPolicy->PpmCst.LatencyCtrl[i].Value      = SetupData.SocketConfig.PowerManagementConfig.CStateLatencyCtrlValue[i];
  }

  if (SetupData.SocketConfig.PowerManagementConfig.C2C3TT) { //if option is not AUTO
    PpmPolicy->C2C3TT = SetupData.SocketConfig.PowerManagementConfig.C2C3TT;
  } else {
    PpmPolicy->C2C3TT = 0x10;
  }

  return EFI_SUCCESS;
}

