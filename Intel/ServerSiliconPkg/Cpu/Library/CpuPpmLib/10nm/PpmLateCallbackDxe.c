/** @file
  10nm DXE Code File for CPU Power Management

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

#include "Common/CpuPpmIncludes.h"
#include <Library/EmulationConfigurationLib.h>
#include <Library/MemRcLib.h>
#include <Library/MemoryServicesLib.h>

extern EFI_MP_SERVICES_PROTOCOL     *mMpService;

/**
  Send fADR config and core/mesh ratio to pcode.

  @param[in] SktId    Socket index.

  @retval VOID
**/
VOID
SendFadrRatio (
  IN UINT8   SktId
  )
{
  SPR_MSR_UNCORE_PERF_STATUS_REGISTER UncorePerfStatusMsr;
  UINT8                               CoreRatio = 0;
  UINT8                               MeshRatio = 0;
  BOOLEAN                             IsFadrEn;
  FLEX_RATIO_N0_PCU_FUN3_STRUCT       FlexRatioCsr;

  IsFadrEn = IsFadrEnabled ();

  //
  // Send fADR ratio when system is in cold boot and fADR is enabled.
  //
  if (IsSystemColdBoot () && IsFadrEn) {
    UncorePerfStatusMsr.Uint64 = AsmReadMsr64 (SPR_MSR_UNCORE_PERF_STATUS);
    FlexRatioCsr.Data = UsraCsrRead (SktId, 0, FLEX_RATIO_N0_PCU_FUN3_REG);

    CoreRatio = (UINT8) FlexRatioCsr.Bits.flex_ratio;
    MeshRatio = (UINT8) UncorePerfStatusMsr.Bits.CurrentClrRatio;
    PcuSetEadrRatio (SktId, IsFadrEn, CoreRatio, MeshRatio);
  }
}

/**

    Executed by System BSP only.
    Common software programming steps needed before warm reset and CPU-only reset.

    @param SocketNumber          Socket index that is executing.
    @param PpmWrapPtr            Pointer to PPM_WRAP_STRUCT instance

    @retval                      VOID

**/
VOID
EFIAPI
SetBiosInitDone (
  UINT8             SocketNumber,
  PPM_WRAP_STRUCT   *PpmWrapPtr
  )
{
  B2P_PCU_MISC_CONFIG_DATA_STRUCT PcuMiscConfigB2p;
  UINT32                          Tempdata = 0;
  EFI_CPU_PM_STRUCT               *ppm;

  ppm       = PpmWrapPtr->Ppm;

  PcuMiscConfigB2p.Data = 0;

  Tempdata = ReadModifyWritePcuMailbox (
               SocketNumber,
               MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG,
               0,
               &PcuMiscConfigB2p.Data,
               0,
               0
               );
  if ((Tempdata & 0x000000ff) != MAILBOX_BIOS_CC_INVALID_COMMAND) {
    PpmS3BootScriptSaveB2PWrite (SocketNumber, MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG, PcuMiscConfigB2p.Data);
  }

  //BIOS_RESET_CPL_PCU_FUN1_REG
  ppm->Info->Bios_Reset_Cpl_Phase= 3; //set BIOS_RESET_CPL3
  SetPpmBiosResetCpl (SocketNumber, ppm);

  SendFadrRatio (SocketNumber);

  //if (/*PPMPolicy->HybridMode == PPM_DISABLE*/ 1) {
    // if DRAM RAPL Hybrid mode is disabled then lock Mailbox interface, else leave it clear to be be set later
    ppm->Info->Bios_Reset_Cpl_Phase= 4; //set BIOS_RESET_CPL4
    SetPpmBiosResetCpl (SocketNumber, ppm);
    //Note: This is for DRAM RAPL, may move tie this to a
    //setup option later which is why this is on a seperate line
  //}

  return;
}

/**

    Executed by System BSP only.
    Sets lock bit in CSR_DESIRED_CORES after CPL3/4

    @param SocketNumber          Socket index that is executing.
    @param PpmWrapPtr            Pointer to PPM_WRAP_STRUCT instance

    @retval                      VOID

**/
VOID
EFIAPI
SetLockBitAfterBiosInitDone (
  UINT8             SocketNumber,
  PPM_WRAP_STRUCT   *PpmWrapPtr
  )
{
  UINT64                            RegAddr = 0;
  CSR_DESIRED_CORES_PCU_FUN1_STRUCT CsrDesiredCores;
  EFI_CPU_PM_STRUCT                 *ppm;
  PPM_POLICY_CONFIGURATION          *PpmPolicy;

  ppm       = PpmWrapPtr->Ppm;
  PpmPolicy = PpmWrapPtr->PpmPolicy;


  if (!PpmPolicy->IssCapableSystem || (PpmPolicy->DynamicIss == 0)) {
    DEBUG ((EFI_D_ERROR, " :PPM: SST-PP is incapable or Dynamic SST-PP is disabled, setting CSR_DESIRED_CORES_PCU_FUN1 lock bit on S%d\n", SocketNumber));
    CsrDesiredCores.Data = UsraCsrRead (SocketNumber, 0, CSR_DESIRED_CORES_PCU_FUN1_REG);
    CsrDesiredCores.Bits.lock = 1;
    UsraCsrWrite (SocketNumber, 0, CSR_DESIRED_CORES_PCU_FUN1_REG, CsrDesiredCores.Data);
    RegAddr = UsraGetCsrRegisterAddress (SocketNumber, 0, CSR_DESIRED_CORES_PCU_FUN1_REG);
    S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &CsrDesiredCores.Data);
  }

  return;
}

/**

    Set lock bit for each package. Executed by System BSP only.
    Common software programming steps needed before warm reset and CPU-only reset.

    @param SocketNumber          Socket index that is executing.
    @param ppm                   Pointer to EFI_CPU_PM_STRUCT instance.
    @param PpmPolicy             Pointer to PPM Policy protocol instance

    @retval                      None

**/
VOID
EFIAPI
SetPackageLockBit (
  UINT8                     SocketNumber,
  EFI_CPU_PM_STRUCT         *ppm,
  PPM_POLICY_CONFIGURATION  *PpmPolicy
  )
{
  UINT64    RegAddr = 0;
  PACKAGE_RAPL_LIMIT_N1_PCU_FUN0_STRUCT         RaplLimitN1Csr;
  P_STATE_LIMITS_PCU_FUN0_STRUCT                PStateLimitsCsr;
  CONFIG_TDP_CONTROL_PCU_FUN3_STRUCT            ConfigTdpCsr;
  PMAX_PCU_FUN0_STRUCT                          PmaxConfigCsr;
  SAPMCTL_PCU_FUN1_STRUCTURE                    SapmCtlCsr;
  TURBO_ACTIVATION_RATIO_PCU_FUN0_STRUCT        TurboActivationRatioLockCsr;
  DRAM_PLANE_POWER_LIMIT_N1_PCU_FUN2_STRUCT     DramPlaneLimitCsr;


  //
  // Keep PMAX CSR locked, even it no function in 10nm
  //
  PmaxConfigCsr.Data = UsraCsrRead (SocketNumber, 0, PMAX_PCU_FUN0_REG);
  PmaxConfigCsr.Bits.lock = 1;
  UsraCsrWrite (SocketNumber, 0, PMAX_PCU_FUN0_REG, PmaxConfigCsr.Data);
  RegAddr = UsraGetCsrRegisterAddress (SocketNumber, 0, PMAX_PCU_FUN0_REG);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &PmaxConfigCsr.Data );

  if (PpmPolicy->SapmCtl[SocketNumber].SapmCtlLock != 0) {
    SapmCtlCsr.Data = UsraCsrRead (SocketNumber, 0, SAPMCTL_PCU_FUN1_REG_10NM);
    SapmCtlCsr.Bits.lock = 1;
    UsraCsrWrite (SocketNumber, 0, SAPMCTL_PCU_FUN1_REG_10NM, SapmCtlCsr.Data);
    RegAddr = UsraGetCsrRegisterAddress (SocketNumber, 0, SAPMCTL_PCU_FUN1_REG_10NM);
    S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &SapmCtlCsr.Data );
  }

  PStateLimitsCsr.Data = UsraCsrRead (SocketNumber, 0, P_STATE_LIMITS_PCU_FUN0_REG);
  PStateLimitsCsr.Bits.lock = 1;
  UsraCsrWrite (SocketNumber, 0, P_STATE_LIMITS_PCU_FUN0_REG, PStateLimitsCsr.Data);
  RegAddr = UsraGetCsrRegisterAddress (SocketNumber, 0, P_STATE_LIMITS_PCU_FUN0_REG);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &PStateLimitsCsr.Data );

  // PACKAGE_RAPL_LIMIT_N1_PCU_FUN0_REG
  if (PpmPolicy->TurboPowerLimit.TurboLimitCsrLock != 0) {
    RaplLimitN1Csr.Data = UsraCsrRead (SocketNumber, 0, PACKAGE_RAPL_LIMIT_N1_PCU_FUN0_REG);
    RaplLimitN1Csr.Bits.pkg_pwr_lim_lock = 1;
    UsraCsrWrite (SocketNumber, 0, PACKAGE_RAPL_LIMIT_N1_PCU_FUN0_REG, RaplLimitN1Csr.Data);
    RegAddr = UsraGetCsrRegisterAddress (SocketNumber, 0, PACKAGE_RAPL_LIMIT_N1_PCU_FUN0_REG);
    S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &RaplLimitN1Csr.Data);
  }

  if (IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL)) {
    TurboActivationRatioLockCsr.Data = UsraCsrRead (SocketNumber, 0, TURBO_ACTIVATION_RATIO_PCU_FUN0_REG);
    TurboActivationRatioLockCsr.Bits.turbo_activation_ratio_lock = 1;
    UsraCsrWrite (SocketNumber, 0, TURBO_ACTIVATION_RATIO_PCU_FUN0_REG, TurboActivationRatioLockCsr.Data);
    RegAddr = UsraGetCsrRegisterAddress (SocketNumber, 0, TURBO_ACTIVATION_RATIO_PCU_FUN0_REG);
    S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &TurboActivationRatioLockCsr.Data);
    DEBUG ((EFI_D_ERROR, "SNR/TNR: Locking CSR turbo activation ratio\n"));
  }

  // CONFIG_TDP_CONTROL_PCU_FUN3_REG
  if ((PpmPolicy->IssCapableSystem && (PpmPolicy->DynamicIss == 0)) ||
      (!PpmPolicy->IssCapableSystem && (PpmPolicy->ConfigTDPLock != 0))) {
    ConfigTdpCsr.Data = UsraCsrRead (SocketNumber, 0, CONFIG_TDP_CONTROL_PCU_FUN3_REG);
    ConfigTdpCsr.Bits.config_tdp_lock = 1;
    UsraCsrWrite (SocketNumber, 0, CONFIG_TDP_CONTROL_PCU_FUN3_REG, ConfigTdpCsr.Data);
    RegAddr = UsraGetCsrRegisterAddress (SocketNumber, 0, CONFIG_TDP_CONTROL_PCU_FUN3_REG);
    S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &ConfigTdpCsr.Data);
  }

  LockPlatformPowerLimitCsr (SocketNumber, ppm, &PpmPolicy->PsysConfig);

  //DRAM_PLANE_POWER_LIMIT_N1_PCU_FUN2_REG
  if (PpmPolicy->AdvPwrMgtCtl.DramRaplPwrLimitLockCsr) {
    DramPlaneLimitCsr.Data = UsraCsrRead (SocketNumber, 0, DRAM_PLANE_POWER_LIMIT_N1_PCU_FUN2_REG);
    DramPlaneLimitCsr.Bits.pp_pwr_lim_lock = 1;
    UsraCsrWrite (SocketNumber, 0, DRAM_PLANE_POWER_LIMIT_N1_PCU_FUN2_REG, DramPlaneLimitCsr.Data);
    RegAddr = UsraGetCsrRegisterAddress (SocketNumber, 0, DRAM_PLANE_POWER_LIMIT_N1_PCU_FUN2_REG);
    S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &DramPlaneLimitCsr.Data );
  }

  return;
}

/**

    Set lock bit for each thread.

    NOTE: This routine should be specifically used to program MSR. There has been a separate routine created for programming CSR's.

    @param PpmWrapPtr            Pointer to PPM_WRAP_STRUCT instance

    @retval                      None

**/
VOID
EFIAPI
SetThreadLockBit (
  PPM_WRAP_STRUCT   *PpmWrapPtr
  )
{
  UINTN                              ProcessorNumber = 0;
  ICX_MSR_VR_CURRENT_CONFIG_REGISTER VrCurrentConfigMsr;
  MSR_PACKAGE_POWER_LIMIT_REGISTER   PkgPowerLimitMsr;
  MSR_MISC_PWR_MGMT_REGISTER         MiscPwrMsr;
  MSR_FLEX_RATIO_REGISTER            FlexRatioMsr;
  EFI_CPU_PM_STRUCT                  *ppm;
  PPM_POLICY_CONFIGURATION           *PpmPolicy;
  ICX_MSR_POWER_CTL1_REGISTER        PowerCtrlMsr;
  UINT32                             RegEdx;
  UINT32                             CurrentSocket;
  SPR_MSR_PLATFORM_POWER_LIMIT_SRVR_REGISTER  PlatPowerLimitMsr;
  MSR_TURBO_ACTIVATION_RATIO_REGISTER         TurboActivationRatioMsr;

  ppm       = PpmWrapPtr->Ppm;
  PpmPolicy = PpmWrapPtr->PpmPolicy;
  mMpService->WhoAmI(mMpService, &ProcessorNumber);

  AsmCpuidEx(CPUID_EXTENDED_TOPOLOGY, 0, NULL, NULL, NULL, &RegEdx);
  CurrentSocket = (RegEdx >> ppm->Info->NumOfBitShift) ;

  // Program MSR_PACKAGE_POWER_LIMIT LOCK bit
  if ((ppm->Info->SlaveDieBitMap & (1 << CurrentSocket)) == 0) {

    PkgPowerLimitMsr.Uint64 = AsmReadMsr64 (MSR_PACKAGE_POWER_LIMIT);
    if (PpmPolicy->TurboPowerLimit.TurboPowerLimitLock != 0) {
      PkgPowerLimitMsr.Bits.PkgPwrLimLock = 1;
      AsmWriteMsr64 (MSR_PACKAGE_POWER_LIMIT, PkgPowerLimitMsr.Uint64);
      WriteRegisterTable (ProcessorNumber, Msr, MSR_PACKAGE_POWER_LIMIT, 0, 64, PkgPowerLimitMsr.Uint64);
    }

    if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
      PlatPowerLimitMsr.Uint64 = AsmReadMsr64 (SPR_MSR_PLATFORM_POWER_LIMIT_SRVR);
      if (PpmPolicy->PsysConfig.PowerLimitMsrLock != 0) {
        PlatPowerLimitMsr.Bits.Lock = 1;
        AsmWriteMsr64 (SPR_MSR_PLATFORM_POWER_LIMIT_SRVR, PlatPowerLimitMsr.Uint64);
        WriteRegisterTable (ProcessorNumber, Msr, SPR_MSR_PLATFORM_POWER_LIMIT_SRVR, 0, 64, PlatPowerLimitMsr.Uint64);
      }
    }
  }

  if ((PpmPolicy->OverclockingLock) &&
      (!ppm->Info->CapId4Csr.Bits.oc_enabled && ppm->Info->CapId4Csr.Bits.ocvolt_ovrd_dis)) {
    FlexRatioMsr.Uint64 = AsmReadMsr64 (MSR_FLEX_RATIO);
    FlexRatioMsr.Bits.OcLock = 1;
    AsmWriteMsr64 (MSR_FLEX_RATIO, FlexRatioMsr.Uint64);
    WriteRegisterTable (ProcessorNumber, Msr, MSR_FLEX_RATIO, 0, 64, FlexRatioMsr.Uint64);
  }

  // Program MSR_POWER_CTL LOCK bit
  // BIT[27] for SKX
  // BIT[23] for ICX
  //
  PowerCtrlMsr.Uint64 = AsmReadMsr64 (ICX_MSR_POWER_CTL1);
  if (PpmPolicy->PowerCtl.ProchotLock != 0) {
    PowerCtrlMsr.Bits.ProchotLock = 1;
    AsmWriteMsr64 (ICX_MSR_POWER_CTL1, PowerCtrlMsr.Uint64);
    WriteRegisterTable (ProcessorNumber, Msr, ICX_MSR_POWER_CTL1, 0, 64, PowerCtrlMsr.Uint64);
  }


  // Program ICX_MSR_VR_CURRENT_CONFIG LOCK bit
  VrCurrentConfigMsr.Uint64 = AsmReadMsr64 (ICX_MSR_VR_CURRENT_CONFIG);
  if (PpmPolicy->PpoCurrentCfg.PpcccLock != 0) {
    VrCurrentConfigMsr.Bits.Lock = 1;
    AsmWriteMsr64 (ICX_MSR_VR_CURRENT_CONFIG, VrCurrentConfigMsr.Uint64);

    // Save Lock Bit for S3
    WriteRegisterTable (ProcessorNumber, Msr, ICX_MSR_VR_CURRENT_CONFIG, 0, 64, VrCurrentConfigMsr.Uint64);
  }

  // Program MSR_MISC_PWR_MGMT LOCK bit
  MiscPwrMsr.Uint64 = AsmReadMsr64 (MSR_MISC_PWR_MGMT);
  if (MiscPwrMsr.Bits.Lock == 0) {
    MiscPwrMsr.Bits.Lock = 1;
    AsmWriteMsr64 (MSR_MISC_PWR_MGMT, MiscPwrMsr.Uint64);  // MSR 1AA:Bit13 write
  }

  LockPlatformPowerLimitMsr (&PpmPolicy->PsysConfig, ProcessorNumber);

  // Program MSR_TURBO_ACTIVATION_RATIO Lock bit
  if (IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL)) {
    TurboActivationRatioMsr.Uint64 = AsmReadMsr64 (MSR_TURBO_ACTIVATION_RATIO);
    TurboActivationRatioMsr.Bits.TurboActivationRatioLock = 1;
    AsmWriteMsr64 (MSR_TURBO_ACTIVATION_RATIO, TurboActivationRatioMsr.Uint64);
    WriteRegisterTable (ProcessorNumber, Msr, MSR_TURBO_ACTIVATION_RATIO, 0, 64, TurboActivationRatioMsr.Uint64);
    DEBUG ((EFI_D_ERROR, "SNR/TNR: Locking MSR turbo activation ratio\n"));
  }

  return;
}

/**

    Set LOCK bit for MSRs

    NOTE: This routine should be specifically used to program MSR.

    @param PpmPolicy             Pointer to PPM Policy protocol instance

    @retval                      None

**/
VOID
EFIAPI
SetConfigControlLockBit (
  PPM_POLICY_CONFIGURATION  *PpmPolicy
  )
{
  MSR_CLOCK_CST_CONFIG_CONTROL_REGISTER   CstConfigMsr;

  if (PpmPolicy->AdvPwrMgtCtl.MsrLock) {
    // placeholder
  }

  if (PpmPolicy->AdvPwrMgtCtl.MsrPkgCstConfigControlLock) {
    //
    // Program MSR_PKG_CST_CONFIG_CONTROL LOCK bit if not set
    //
    CstConfigMsr.Uint32 = (UINT32)AsmReadMsr32 (MSR_CLOCK_CST_CONFIG_CONTROL);
    if (CstConfigMsr.Bits.Lock == 0) {
      CstConfigMsr.Bits.Lock = 1;
      AsmWriteMsr32 (MSR_CLOCK_CST_CONFIG_CONTROL, CstConfigMsr.Uint32);
    }
  }
}

/**

    This routine will program BIOS_REST_CPL CSR bits per phase input.

    @param SocketNumber          Socket index that is executing.
    @param ppm                   Pointer to EFI_CPU_PM_STRUCT instance.

    @retval                      None

**/
VOID
SetPpmBiosResetCpl (
  UINT8             SocketNumber,
  EFI_CPU_PM_STRUCT *ppm
  )
{
  UINT64                         RegAddr = 0;
  UINT8                          phase;
  BIOS_RESET_CPL_PCU_FUN1_STRUCT BiosResetCpl;
  UINT8                          TimeoutCounter;
  UINT8                          PcodeInitDone = 0;
  UINTN                          Delay;

  phase = ppm->Info->Bios_Reset_Cpl_Phase;

  if ((ppm->Info->DfxRstCplBitsEn & (1 << (phase - 1))) == 0) {
    return;
  }

  //BIOS_RESET_CPL_PCU_FUN1_REG
  BiosResetCpl.Data = UsraCsrRead (SocketNumber, 0, BIOS_RESET_CPL_PCU_FUN1_REG);

  switch (phase) {
    case SET_CPL_1:
      //
      // Set CPL1: NID MAP, package C-state and S-state BIOS configurations are completed.
      //
      BiosResetCpl.Bits.rst_cpl1 = 1;
      break;
    case SET_CPL_2:
      //
      // Set CPL2: Ready for Periodic RCOMP Starts
      //
      BiosResetCpl.Bits.rst_cpl2 = 1;
      break;
    case SET_CPL_3:
      //
      // Set CPL3: BIOS Power and Thermal Management Configure Completed
      //
      BiosResetCpl.Bits.rst_cpl3 = 1;
      break;
    case SET_CPL_4:
      //
      // Set CPL4: Memory Calibration Completed
      //
      BiosResetCpl.Bits.rst_cpl4 = 1;
      break;
    default:
      break;
  }

  if (UbiosGenerationEnabled ()) {
    DEBUG ((EFI_D_ERROR, " \n:INIT - BIOS_RESET_CPL: Set CPL%d", phase));
  }

  DEBUG ((EFI_D_ERROR, " \n:INIT - BIOS_RESET_CPL: Set CPL%d on #S%d into BIOS_RESET_CPL_PCU_FUN1_REG\n", phase, SocketNumber) );
  UsraCsrWrite (SocketNumber, 0, BIOS_RESET_CPL_PCU_FUN1_REG, BiosResetCpl.Data);
  RegAddr = UsraGetCsrRegisterAddress (SocketNumber, 0, BIOS_RESET_CPL_PCU_FUN1_REG);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &BiosResetCpl.Data );

  if (HybridSystemLevelEmulationEnabled()) {
    Delay = 20000000;//20000000us
  } else {
    Delay = 20;//20us
  }

  if (UbiosGenerationEnabled ()) {
    DEBUG ((EFI_D_ERROR, " \n:Done. INIT - BIOS_RESET_CPL: Set CPL%d on #S%d", phase, SocketNumber));
  }

  //
  // wait for PCU acknowledgement for 2ms (20us * 100)
  //
  for (TimeoutCounter = 100; TimeoutCounter > 0; TimeoutCounter--) {
    BiosResetCpl.Data = UsraCsrRead (SocketNumber, 0, BIOS_RESET_CPL_PCU_FUN1_REG);
    switch(phase) {
      case SET_CPL_1:
        PcodeInitDone = (UINT8) BiosResetCpl.Bits.pcode_init_done1;
        break;
      case SET_CPL_2:
        PcodeInitDone = (UINT8) BiosResetCpl.Bits.pcode_init_done2;
        break;
      case SET_CPL_3:
        PcodeInitDone = (UINT8) BiosResetCpl.Bits.pcode_init_done3;
        break;
      case SET_CPL_4:
        PcodeInitDone = (UINT8) BiosResetCpl.Bits.pcode_init_done4;
        break;
      default:
        break;
    }

    if (PcodeInitDone == 1) {
      break;
    }

    MicroSecondDelay (Delay);
  }

  //If timeout, BIOS should be in wait loop until pcode has reeponse for CPL3/CPL4.
  if ((TimeoutCounter == 0) || (PcodeInitDone == 0)) {
    DEBUG ((EFI_D_ERROR, " \n:PM - Pcode fails to ACK CPL%d on #S%d\n\n", phase, SocketNumber) );

    if ((phase == SET_CPL_3) || (phase == SET_CPL_4)) {
      DEBUG ((EFI_D_ERROR, " \n:PM - Waiting...... to ACK CPL%d on #S%d\n\n", phase, SocketNumber) );

      while(PcodeInitDone == 0) {
        BiosResetCpl.Data = UsraCsrRead (SocketNumber, 0, BIOS_RESET_CPL_PCU_FUN1_REG);
        if (phase == SET_CPL_3) {
          PcodeInitDone = (UINT8) BiosResetCpl.Bits.pcode_init_done3;
        }
        if (phase == SET_CPL_4) {
          PcodeInitDone = (UINT8) BiosResetCpl.Bits.pcode_init_done4;
        }
      }

      DEBUG ((EFI_D_ERROR, " \n:PM - Successfully got ACK CPL%d on #S%d\n\n", phase, SocketNumber) );
    }
  }
}
