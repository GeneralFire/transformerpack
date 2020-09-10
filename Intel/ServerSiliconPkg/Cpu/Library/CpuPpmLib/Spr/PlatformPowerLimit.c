/** @file
  Code File for CPU Power Management

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

#include "CpuPpmIncludes.h"
#include <Cpu/CpuCoreRegs.h>
#include <RcRegs.h>
#include <Library/KtiApi.h>

/**
  Init Psys power limit MSR register.

  @param ppm              Pointer to policy protocol instance
  @param PsysPowerLimit   Pointer to PSYS_POWRER_LIMIT instance
  @param ProcessorNumber  Processor index that is executing

  @retval                 None

**/
VOID
EFIAPI
InitializePlatformPowerLimitMsr (
  EFI_CPU_PM_STRUCT     *ppm,
  PSYS_POWRER_LIMIT     *PsysPowerLimit,
  UINTN                 ProcessorNumber
  )
{
  UINT32                                  Ppl1PowerLimit;
  UINT32                                  Ppl2PowerLimit;

  UINT16                                  Mutliplier;
  UINT32                                  TempPowerLimit = 0;
  UINT16                                  PackageTdp;
  UINT8                                   ProcessorPowerUnit;
  UINT32                                  RegEdx;
  UINT32                                  CurrentSocket;
  SPR_MSR_PLATFORM_INFO_REGISTER          PlatformInfoMsr;
  SPR_MSR_PLATFORM_POWER_LIMIT_SRVR_REGISTER   PlatPowerLimitMsr;

  AsmCpuidEx(CPUID_EXTENDED_TOPOLOGY, 0, NULL, NULL, NULL, &RegEdx);
  CurrentSocket = (RegEdx >> ppm->Info->NumOfBitShift) ;
  if ((ppm->Info->PackageDieCount[CurrentSocket] > 1) && (ppm->Info->SlaveDieBitMap & (1 << CurrentSocket))) {
    // Slave Die is detected, skip program
    return;
  }

  Mutliplier = 120;
  PackageTdp          = ppm->Info->PackageTdp[CurrentSocket];
  ProcessorPowerUnit  = ppm->Info->ProcessorPowerUnit[CurrentSocket];

  PlatformInfoMsr.Uint64    = AsmReadMsr64 (SPR_MSR_PLATFORM_INFO);
  PlatPowerLimitMsr.Uint64  = AsmReadMsr64 (SPR_MSR_PLATFORM_POWER_LIMIT_SRVR);

  //
  // Check if TDP limits are programmable
  // Platform Info MSR (0xCE) [29]
  //
  if (PlatformInfoMsr.Bits.PrgTdpLimEn == 0) {
    return ;
  }

  //
  // Initialize the Power Limit 1 and Power Limit 1 enable bit
  // Power Limit 1: Platform Power Limit MSR [17:0]
  // Power Limit 1 Enable: Platform Power Limit MSR [18]
  PlatPowerLimitMsr.Bits.PowerLimit1En = PsysPowerLimit->PowerLimit1En;
  PlatPowerLimitMsr.Bits.PowerLimit2En = PsysPowerLimit->PowerLimit2En;
  if (PsysPowerLimit->PowerLimit1En != 0) {
    //
    //
    // By default, program Power Limit 1 to Package TDP limit
    //
    Ppl1PowerLimit = (UINT32) MultU64x32 (PackageTdp, ProcessorPowerUnit);
    TempPowerLimit = PsysPowerLimit->PowerLimit1Power;
    if (TempPowerLimit) {
      if (TempPowerLimit <= PackageTdp) {
        Ppl1PowerLimit = (UINT32) MultU64x32 (TempPowerLimit, ProcessorPowerUnit);
      }

      //
      // OverClocking unit is detected
      //
      if (ppm->Info->CapId4Csr.Bits.oc_enabled) {
        Ppl1PowerLimit = (UINT32) MultU64x32 (TempPowerLimit, ProcessorPowerUnit);
      }
    }

    PlatPowerLimitMsr.Bits.PowerLimit1 = Ppl1PowerLimit;
    PlatPowerLimitMsr.Bits.PowerLimit1En = 1;
    PlatPowerLimitMsr.Bits.CriticalPowerClamp1 = PsysPowerLimit->ClampingLim1;
    PlatPowerLimitMsr.Bits.PowerLimit1Time = PsysPowerLimit->PowerLimit1Time;
  }

  if (PsysPowerLimit->PowerLimit2En != 0) {
    Ppl2PowerLimit = (UINT32) EFI_IDIV_ROUND (MultU64x64(MultU64x32(Mutliplier,PackageTdp), ProcessorPowerUnit), 100);
    TempPowerLimit = PsysPowerLimit->PowerLimit2Power;
    if ((TempPowerLimit > 0) && (TempPowerLimit < (UINT32)EFI_IDIV_ROUND (MultU64x32 (Mutliplier, PackageTdp), 100))) {
      Ppl2PowerLimit = (UINT32) MultU64x32 (TempPowerLimit, ProcessorPowerUnit);
    }

    //
    // OverClocking unit is detected
    //
    if (ppm->Info->CapId4Csr.Bits.oc_enabled) {
      if(TempPowerLimit > 0) {
        Ppl2PowerLimit = (UINT32) MultU64x32 (TempPowerLimit, ProcessorPowerUnit);
      }
    }

    PlatPowerLimitMsr.Bits.PowerLimit2  = Ppl2PowerLimit;
    PlatPowerLimitMsr.Bits.PowerLimit2En = 1;
    PlatPowerLimitMsr.Bits.CriticalPowerClamp2 = PsysPowerLimit->ClampingLim2;
    PlatPowerLimitMsr.Bits.PowerLimit2Time = PsysPowerLimit->PowerLimit2Time;
  }

  AsmWriteMsr64 (SPR_MSR_PLATFORM_POWER_LIMIT_SRVR, PlatPowerLimitMsr.Uint64);
  //
  // Save for S3
  //
  WriteRegisterTable (ProcessorNumber, Msr, SPR_MSR_PLATFORM_POWER_LIMIT_SRVR, 0, 64, PlatPowerLimitMsr.Uint64);
}

/**
  Lock Psys power limit MSR registers.

  @param PsysPowerLimit    Pointer to PSYS_POWRER_LIMIT instance
  @param ProcessorNumber   Processor index that is executing

  @retval None

**/
VOID
LockPlatformPowerLimitMsr (
  PSYS_POWRER_LIMIT     *PsysPowerLimit,
  UINTN                 ProcessorNumber
  )
{
  SPR_MSR_PLATFORM_POWER_LIMIT_SRVR_REGISTER   PlatPowerLimitMsr;

  if (PsysPowerLimit->PowerLimitMsrLock) {
    PlatPowerLimitMsr.Uint64  = AsmReadMsr64 (SPR_MSR_PLATFORM_POWER_LIMIT_SRVR);
    PlatPowerLimitMsr.Bits.Lock = 1;
    AsmWriteMsr64 (SPR_MSR_PLATFORM_POWER_LIMIT_SRVR, PlatPowerLimitMsr.Uint64);

    //
    // Save for S3
    //
    WriteRegisterTable (ProcessorNumber, Msr, SPR_MSR_PLATFORM_POWER_LIMIT_SRVR, 0, 64, PlatPowerLimitMsr.Uint64);
  }
}

/**
  Init Psys Power Info register.
  Set MAX/MIN PPL1, MAX PPL2 and MAX time window.

  @param SocketNumber       Socket index that is executing
  @param ppm                Pointer to policy protocol instance
  @param PsysPowerLimit     Pointer to PSYS_POWRER_LIMIT instance

  @retval None

**/
VOID
InitializePlatformPowerInfoCsr (
  UINT8                 SocketNumber,
  EFI_CPU_PM_STRUCT     *ppm,
  PSYS_POWRER_LIMIT     *PsysPowerLimit
  )
{
  PLATFORM_POWER_INFO_N0_PCU_FUN6_STRUCT  PowerInfoN0Csr;
  PLATFORM_POWER_INFO_N1_PCU_FUN6_STRUCT  PowerInfoN1Csr;

  UINT32                                  Ppl1PowerLimit;
  UINT32                                  Ppl2PowerLimit;
  UINT32                                  TempPowerLimit;
  UINT64                                  RegAddr = 0;
  UINT16                                  PackageTdp;
  UINT8                                   ProcessorPowerUnit;

  if (!PsysPowerLimit->OverridePplMaxMin) {
    return ;
  }

  PackageTdp          = ppm->Info->PackageTdp[SocketNumber];
  ProcessorPowerUnit  = ppm->Info->ProcessorPowerUnit[SocketNumber];
  PowerInfoN0Csr.Data = UsraCsrRead (SocketNumber, 0, PLATFORM_POWER_INFO_N0_PCU_FUN6_REG);
  PowerInfoN1Csr.Data = UsraCsrRead (SocketNumber, 0, PLATFORM_POWER_INFO_N1_PCU_FUN6_REG);

  TempPowerLimit = PsysPowerLimit->MinPpl1Power;
  if (TempPowerLimit) {
    Ppl1PowerLimit = (UINT32) MultU64x32 (TempPowerLimit, ProcessorPowerUnit);
    PowerInfoN0Csr.Bits.min_ppl1_value = Ppl1PowerLimit;
  }

  TempPowerLimit = PsysPowerLimit->MaxPpl1Power;
  if (TempPowerLimit) {
    Ppl1PowerLimit = (UINT32) MultU64x32 (TempPowerLimit, ProcessorPowerUnit);
    PowerInfoN0Csr.Bits.max_ppl1_value = Ppl1PowerLimit;
  }

  TempPowerLimit = PsysPowerLimit->MaxPpl2Power;
  if (TempPowerLimit) {
    Ppl2PowerLimit = (UINT32) MultU64x32 (TempPowerLimit, ProcessorPowerUnit);
    PowerInfoN1Csr.Bits.max_ppl2_value = Ppl2PowerLimit;
  }

  PowerInfoN1Csr.Bits.max_time_window = PsysPowerLimit->MaxPplTimeWindow;

  UsraCsrWrite (SocketNumber, 0, PLATFORM_POWER_INFO_N0_PCU_FUN6_REG, PowerInfoN0Csr.Data);
  UsraCsrWrite (SocketNumber, 0, PLATFORM_POWER_INFO_N1_PCU_FUN6_REG, PowerInfoN1Csr.Data);

  RegAddr = UsraGetCsrRegisterAddress (SocketNumber, 0, PLATFORM_POWER_INFO_N0_PCU_FUN6_REG);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &PowerInfoN0Csr.Data);
  RegAddr = UsraGetCsrRegisterAddress (SocketNumber, 0, PLATFORM_POWER_INFO_N1_PCU_FUN6_REG);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &PowerInfoN1Csr.Data);
}

/**
  Lock Psys Power Info register.

  @param SocketNumber       Socket index that is executing
  @param PsysPowerLimit     Pointer to PSYS_POWRER_LIMIT instance

  @retval None

**/
VOID
LockPlatformPowerInfoCsr (
  UINT8                 SocketNumber,
  PSYS_POWRER_LIMIT     *PsysPowerLimit
  )
{
  PLATFORM_POWER_INFO_N1_PCU_FUN6_STRUCT  PowerInfoN1Csr;
  UINT64                                  RegAddr = 0;

  if (PsysPowerLimit->PowerLimitCsrLock) {
    PowerInfoN1Csr.Data = UsraCsrRead (SocketNumber, 0, PLATFORM_POWER_INFO_N1_PCU_FUN6_REG);
    PowerInfoN1Csr.Bits.plt_pwr_info_lock = 1;
    UsraCsrWrite (SocketNumber, 0, PLATFORM_POWER_INFO_N1_PCU_FUN6_REG, PowerInfoN1Csr.Data);

    RegAddr = UsraGetCsrRegisterAddress (SocketNumber, 0, PLATFORM_POWER_INFO_N1_PCU_FUN6_REG);
    S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &PowerInfoN1Csr.Data);
  }
}

/**
  Override the Psys master setting.
  By default, Psys sensor connect to socket 0, but user could choose
  different config to match board design.

  @param SocketNumber       Socket index that is executing
  @param PsysPowerLimit     Pointer to PSYS_POWRER_LIMIT instance

  @retval                   None

**/
VOID
OverridePsysMasterConfig (
  UINT8                 SocketNumber,
  PSYS_POWRER_LIMIT     *PsysPowerLimit
  )
{
  B2P_PSYS_CONFIG_DATA_STRUCT       PsysDataB2p;
  B2P_STANDARD_INTERFACE_STRUCT     PsysCmdB2p;
  UINT32                            Tempdata;

  UINT32                            Bitmask;
  UINT8                             Index;

  B2P_VR_INTERFACE_CMD_STRUCT       VrInterfaceCmdB2p;
  B2P_VR_INTERFACE_DATA_STRUCT      VrInterfaceDataB2p;
  UINT32                            SocketPresentBitmap;

  if (!PsysPowerLimit->OverridePsysMaster) {
    return ;
  }

  Bitmask           = 0;
  PsysCmdB2p.Data   = 0;
  PsysDataB2p.Data  = 0;
  SocketPresentBitmap = GetSocketPresentBitMap();

  //
  // Validate proper config value is given
  //
  if (SocketNumber == 0) {
    for (Index = 0; Index < MAX_SOCKET; Index++) {
      if (!SocketPresent(Index)) {
        PsysPowerLimit->PsysMaster[Index] = 0;
        break;
      }
      VrInterfaceCmdB2p.Data              = 0;
      VrInterfaceDataB2p.Data             = 0;

      VrInterfaceCmdB2p.Bits.Command      = MAILBOX_BIOS_CMD_VR_INTERFACE;
      VrInterfaceCmdB2p.Bits.VrId         = 0x0d;

      VrInterfaceDataB2p.Bits.VrCmd       = SVID_CMD_GET_REG;
      VrInterfaceDataB2p.Bits.VidRegData  = 0;
      VrInterfaceDataB2p.Bits.RegAddress  = 0x05;

      Tempdata = ReadModifyWritePcuMailbox (
                  Index,
                  VrInterfaceCmdB2p.Data,
                  MAILBOX_BIOS_CMD_ZERO,
                  &VrInterfaceDataB2p.Data,
                  0,
                  0
                  );
      if ((Tempdata != 0) || (VrInterfaceDataB2p.Data != 0x0c)) {
        DEBUG ((EFI_D_ERROR, "[Psys] failed to read S%d VR %d or Psys not present %d, remove from Psys master\n",
              Index,
              VrInterfaceCmdB2p.Bits.VrId,
              VrInterfaceDataB2p.Data));
        PsysPowerLimit->PsysMaster[Index] = 0;
        PsysPowerLimit->PsysSlaveBitmap[Index] = 0;
      }

      if (PsysPowerLimit->PsysMaster[Index]) {
        Bitmask |= 1 << Index;
      }
    }

    for (Index = 0; Index < MAX_SOCKET; Index++) {
      if (PsysPowerLimit->PsysMaster[Index]) {
        if (Bitmask & PsysPowerLimit->PsysSlaveBitmap[Index]) {
          DEBUG ((EFI_D_ERROR, "[Psys] Psys master[%d], found slave bitmap conflict, disable override.\n", Index));
          PsysPowerLimit->OverridePsysMaster = 0;
          return;
        }
        Bitmask |= PsysPowerLimit->PsysSlaveBitmap[Index] & SocketPresentBitmap;
      }
    }
  }

  //
  // Psys master and slave bitmap
  //
  PsysDataB2p.Bits.NodeIdMask = PsysPowerLimit->PsysSlaveBitmap[SocketNumber];
  PsysDataB2p.Bits.Master     = PsysPowerLimit->PsysMaster[SocketNumber];
  PsysCmdB2p.Bits.command     = MAILBOX_BIOS_CMD_PSYS_CONFIG;
  Tempdata = ReadModifyWritePcuMailbox (
              SocketNumber,
              MAILBOX_BIOS_CMD_ZERO,
              PsysCmdB2p.Data,
              &PsysDataB2p.Data,
              0,
              0
              );
  PpmS3BootScriptSaveB2PWrite (SocketNumber, PsysCmdB2p.Data, PsysDataB2p.Data);
  if (Tempdata != 0) {
    DEBUG ((EFI_D_ERROR, " \n\n :: !!! BIOS failed to program B2P Psys Config on socket (%d). Pcode returns (%d) !!!. \n\n", SocketNumber, Tempdata));
  } else {
    if (SocketNumber == 0) {
      DEBUG ((EFI_D_INFO, "[PSYS] MAILBOX_BIOS_CMD_PSYS_CONFIG write %08X\n", PsysDataB2p.Data));
    }
  }
}

/**
  Init Psys power limit CSR register.
  Set PPL1 and PPL2 power limit and time window.

  @param SocketNumber       Socket index that is executing
  @param ppm                Pointer to policy protocol instance
  @param PsysPowerLimit     Pointer to PSYS_POWRER_LIMIT instance

  @retval                   None

**/
VOID
EFIAPI
InitializePlatformPowerLimitCsr (
  UINT8                 SocketNumber,
  EFI_CPU_PM_STRUCT     *ppm,
  PSYS_POWRER_LIMIT     *PsysPowerLimit
  )
{
  SPR_MSR_PLATFORM_INFO_REGISTER          PlatformInfoMsr;

  UINT32                                  Ppl1PowerLimit;
  UINT32                                  Ppl2PowerLimit;
  UINT16                                  Mutliplier;
  UINT32                                  TempPowerLimit;
  UINT64                                  RegAddr = 0;
  UINT16                                  PackageTdp;
  UINT8                                   ProcessorPowerUnit;

  PLATFORM_RAPL_LIMIT_N0_PCU_FUN6_STRUCT  PowerLimitN0Csr;
  PLATFORM_RAPL_LIMIT_N1_PCU_FUN6_STRUCT  PowerLimitN1Csr;

  if ((ppm->Info->PackageDieCount[SocketNumber] > 1) && (ppm->Info->SlaveDieBitMap & (1 << SocketNumber))) {
    // Slave Die is detected, skip program
    return;
  }

  Mutliplier          = 120;
  PackageTdp          = ppm->Info->PackageTdp[SocketNumber];
  ProcessorPowerUnit  = ppm->Info->ProcessorPowerUnit[SocketNumber];
  PlatformInfoMsr.Uint64 = AsmReadMsr64 (SPR_MSR_PLATFORM_INFO);
  PowerLimitN0Csr.Data = UsraCsrRead (SocketNumber, 0, PLATFORM_RAPL_LIMIT_N0_PCU_FUN6_REG);
  PowerLimitN1Csr.Data = UsraCsrRead (SocketNumber, 0, PLATFORM_RAPL_LIMIT_N1_PCU_FUN6_REG);

  //
  // Check if TDP limits are programmable
  // Platform Info MSR (0xCE) [29]
  //
  if (PlatformInfoMsr.Bits.PrgTdpLimEn == 0) {
    return ;
  }

  //
  // Set Power Info CSR
  //
  InitializePlatformPowerInfoCsr (SocketNumber, ppm, PsysPowerLimit);

  PowerLimitN0Csr.Bits.ppl1_en = PsysPowerLimit->PowerLimit1En;
  PowerLimitN1Csr.Bits.ppl2_en = PsysPowerLimit->PowerLimit2En;
  DEBUG((EFI_D_ERROR, "ppl1_en:%d ppl2_en:%d\n", PsysPowerLimit->PowerLimit1En, PsysPowerLimit->PowerLimit2En));

  if (PsysPowerLimit->PowerLimit1En != 0) {
    //
    // By default, program Power Limit 1 to Package TDP limit
    //
    Ppl1PowerLimit = (UINT32) MultU64x32 (PackageTdp, ProcessorPowerUnit);
    TempPowerLimit = PsysPowerLimit->PowerLimit1Power;
    if (TempPowerLimit) {
      if (TempPowerLimit <= PackageTdp) {
        Ppl1PowerLimit = (UINT32) MultU64x32 (TempPowerLimit, ProcessorPowerUnit);
      }

      //
      // OverClocking unit is detected
      //
      if (ppm->Info->CapId4Csr.Bits.oc_enabled) {
        Ppl1PowerLimit = (UINT32) MultU64x32 (TempPowerLimit, ProcessorPowerUnit);
      }
    }

    PowerLimitN0Csr.Bits.ppl1 = Ppl1PowerLimit;
    PowerLimitN0Csr.Bits.plt_clmp_lim_1 = PsysPowerLimit->ClampingLim1;
    PowerLimitN0Csr.Bits.ptw1 = PsysPowerLimit->PowerLimit1Time;
  }

  if (PsysPowerLimit->PowerLimit2En != 0) {
    //
    // PowerLimit2Power value is in watts. We need to convert it to
    // CPU Power unit, specified in PACKAGE_POWER_SKU_UNIT_MSR[3:0].
    // Since we are converting from Watts to CPU power units, multiply by
    // PACKAGE_POWER_SKU_UNIT_MSR[3:0]
    //
    Ppl2PowerLimit = (UINT32) EFI_IDIV_ROUND (MultU64x64(MultU64x32(Mutliplier,PackageTdp), ProcessorPowerUnit), 100);
    TempPowerLimit = PsysPowerLimit->PowerLimit2Power;
    if ((TempPowerLimit > 0) && (TempPowerLimit < (UINT32)EFI_IDIV_ROUND (MultU64x32 (Mutliplier, PackageTdp), 100))) {
      Ppl2PowerLimit = (UINT32) MultU64x32 (TempPowerLimit, ProcessorPowerUnit);
    }

    //
    // OverClocking unit is detected
    //
    if (ppm->Info->CapId4Csr.Bits.oc_enabled) {
      if(TempPowerLimit > 0) {
        Ppl2PowerLimit = (UINT32) MultU64x32 (TempPowerLimit, ProcessorPowerUnit);
      }
    }

    PowerLimitN1Csr.Bits.ppl2 = Ppl2PowerLimit;
    PowerLimitN1Csr.Bits.plt_clmp_lim_2 = PsysPowerLimit->ClampingLim2;
    PowerLimitN1Csr.Bits.ptw2 = PsysPowerLimit->PowerLimit2Time;
  }

  UsraCsrWrite (SocketNumber, 0, PLATFORM_RAPL_LIMIT_N0_PCU_FUN6_REG, PowerLimitN0Csr.Data);
  UsraCsrWrite (SocketNumber, 0, PLATFORM_RAPL_LIMIT_N1_PCU_FUN6_REG, PowerLimitN1Csr.Data);

  if (SocketNumber == 0) {
    DEBUG ((EFI_D_INFO, "[PSYS] PLATFORM_POWER_LIMIT CSR=%08X %08X\n", PowerLimitN1Csr.Data, PowerLimitN0Csr.Data));
  }

  RegAddr = UsraGetCsrRegisterAddress (SocketNumber, 0, PLATFORM_RAPL_LIMIT_N0_PCU_FUN6_REG);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &PowerLimitN0Csr.Data);
  RegAddr = UsraGetCsrRegisterAddress (SocketNumber, 0, PLATFORM_RAPL_LIMIT_N1_PCU_FUN6_REG);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &PowerLimitN1Csr.Data);

  OverridePsysMasterConfig (SocketNumber, PsysPowerLimit);
}

/**
  Lock Psys power limit CSR register.

  @param SocketNumber       Socket index that is executing
  @param ppm                Pointer to policy protocol instance
  @param PsysPowerLimit     Pointer to PSYS_POWRER_LIMIT instance

  @retval None

**/
VOID
LockPlatformPowerLimitCsr (
  UINT8                 SocketNumber,
  EFI_CPU_PM_STRUCT     *ppm,
  PSYS_POWRER_LIMIT     *PsysPowerLimit
  )
{
  UINT64                                  RegAddr = 0;
  PLATFORM_RAPL_LIMIT_N1_PCU_FUN6_STRUCT  PowerLimitN1Csr;

  if (PsysPowerLimit->PowerLimitCsrLock) {
    if ((ppm->Info->PackageDieCount[SocketNumber] > 1) && (ppm->Info->SlaveDieBitMap & (1 << SocketNumber))) {
      // Slave Die is detected, skip program
      return;
    }

    LockPlatformPowerInfoCsr (SocketNumber, PsysPowerLimit);

    PowerLimitN1Csr.Data = UsraCsrRead (SocketNumber, 0, PLATFORM_RAPL_LIMIT_N1_PCU_FUN6_REG);
    PowerLimitN1Csr.Bits.plt_pwr_lim_lock = 1;
    UsraCsrWrite (SocketNumber, 0, PLATFORM_RAPL_LIMIT_N1_PCU_FUN6_REG, PowerLimitN1Csr.Data);

    RegAddr = UsraGetCsrRegisterAddress (SocketNumber, 0, PLATFORM_RAPL_LIMIT_N1_PCU_FUN6_REG);
    S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &PowerLimitN1Csr.Data);
  }
}
