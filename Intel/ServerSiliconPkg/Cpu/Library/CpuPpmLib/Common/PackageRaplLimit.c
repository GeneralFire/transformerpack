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
#include <Library/PcuMailBoxLib.h>

//
// Table to convert Seconds into equivalent MSR values
// This table is used for TDP Time Window programming
//
const UINT16        mSecondsToMsrValueMapTable[][2] = {
   // Seconds       MSR Value
   {   1        ,     0xA },
   {   2        ,     0xB },
   {   3        ,    0x4B },
   {   4        ,     0xC },
   {   5        ,    0x2C },
   {   6        ,    0x4C },
   {   7        ,    0x6C },
   {   8        ,     0xD },
   {  10        ,    0x2D },
   {  12        ,    0x4D },
   {  14        ,    0x6D },
   {  16        ,     0xE },
   {  20        ,    0x2E },
   {  24        ,    0x4E },
   {  28        ,    0x6E },
   {  32        ,     0xF },
   {  40        ,    0x2F },
   {  48        ,    0x4F },
   {  56        ,    0x6F },
   {  64        ,    0x10 },
   {  80        ,    0x30 },
   {  96        ,    0x50 },
   {  112       ,    0x70 },
   {  128       ,    0x11 },
   {  160       ,    0x31 },
   {  192       ,    0x51 },
   {  224       ,    0x71 },
   {  256       ,    0x12 },
   {  320       ,    0x32 },
   {  384       ,    0x52 },
   {  448       ,    0x72 },
   {END_OF_TABLE,END_OF_TABLE}
};

/**

  This routine is called to program CSR_TURBO_POWER_LIMIT. It will be call multiple time passing any the SocketNumber to be programmed.

  @param SocketNumber          Socket index that is executing.
  @param ppm                   Pointer to EFI_CPU_PM_STRUCT instance.
  @param TurboPowerLimit       Pointer to TURBO_POWRER_LIMIT instance
  @param C2C3tt                Platform setting for C2C3tt

  @retval                      None

**/
VOID
EFIAPI
InitializeRaplPowerLimitCsr (
  UINT8                 SocketNumber,
  EFI_CPU_PM_STRUCT     *ppm,
  TURBO_POWRER_LIMIT    *TurboPowerLimit,
  UINT32                C2C3tt
  )
{
  ICX_MSR_PLATFORM_INFO_REGISTER   PlatformInfoMsr;

  UINT16   ConvertedPowerLimit1;
  UINT16   ConvertedPowerLimit1Time;
  UINT16   ConvertedPowerLimit2Time;
  UINT16   ConvertedShortDurationPowerLimit;
  UINT16   Index;
  UINT16   Mutliplier;
  UINT16   GetPcdTemp;
  UINT64   RegAddr = 0;

  MSR_PACKAGE_POWER_LIMIT_REGISTER      PkgPowerLimitMsr;
  PACKAGE_RAPL_LIMIT_N0_PCU_FUN0_STRUCT RaplLimitN0Csr;
  PACKAGE_RAPL_LIMIT_N1_PCU_FUN0_STRUCT RaplLimitN1Csr;
  C2C3TT_PCU_FUN1_STRUCT                C2C3TtCsr;


  ConvertedPowerLimit1Time = 0;
  Mutliplier = 0;

  // Read (CSR)
  C2C3TtCsr.Data = UsraCsrRead (SocketNumber, 0, C2C3TT_PCU_FUN1_REG);

  // Program CSR_C2C3TT
  C2C3TtCsr.Bits.ppdn_init = C2C3tt;
  UsraCsrWrite (SocketNumber, 0, C2C3TT_PCU_FUN1_REG, C2C3TtCsr.Data);
  RegAddr = UsraGetCsrRegisterAddress (SocketNumber, 0, C2C3TT_PCU_FUN1_REG);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &C2C3TtCsr.Data );

  if ((ppm->Info->PackageDieCount[SocketNumber] > 1) && (ppm->Info->SlaveDieBitMap & (1 << SocketNumber))) {
    // Slave Die is detected, skip CSR PACKAGE_RAPL_LIMIT program
    return;
  }

  // Read (MSR)
  PlatformInfoMsr.Uint64 = AsmReadMsr64 (ICX_MSR_PLATFORM_INFO);

  PkgPowerLimitMsr.Bits.PkgPwrLim1      = TurboPowerLimit->PowerLimit1Power;
  PkgPowerLimitMsr.Bits.PkgPwrLim1En    = TurboPowerLimit->PowerLimit1En;
  PkgPowerLimitMsr.Bits.PkgClmpLim1     = TurboPowerLimit->PkgClmpLim1;
  PkgPowerLimitMsr.Bits.PkgPwrLim2      = TurboPowerLimit->PowerLimit2Power;
  PkgPowerLimitMsr.Bits.PkgPwrLim2En    = TurboPowerLimit->PowerLimit2En;
  PkgPowerLimitMsr.Bits.PkgClmpLim2     = TurboPowerLimit->PkgClmpLim2;

  // PACKAGE_RAPL_LIMIT_N0_PCU_FUN0_REG
  RaplLimitN0Csr.Data = UsraCsrRead (SocketNumber, 0, PACKAGE_RAPL_LIMIT_N0_PCU_FUN0_REG);
  // PACKAGE_RAPL_LIMIT_N1_PCU_FUN0_REG
  RaplLimitN1Csr.Data = UsraCsrRead (SocketNumber, 0, PACKAGE_RAPL_LIMIT_N1_PCU_FUN0_REG);

  // Program CSR_TURBO_POWER_LIMIT
  //
  // Check if TDP limits are programmable
  // Platform Info MSR (0xCE) [29]
  //
  if (PlatformInfoMsr.Bits.PrgTdpLimEn != 0) {
    //
    // Initialize the Power Limit 1 and Power Limit 1 enable bit
    // Power Limit 1: Turbo Power Limit MSR [14:0]
    // Power Limit 1 Enable: Turbo Power Limit MSR [15]
    //
    //
    // By default, program Power Limit 1 to Package TDP limit
    //
    ConvertedPowerLimit1 = (ppm->Info->PackageTdp[SocketNumber] * ppm->Info->ProcessorPowerUnit[SocketNumber]);
    GetPcdTemp = (UINT16)PkgPowerLimitMsr.Bits.PkgPwrLim1;
    if (GetPcdTemp) {
      //
      // PPMPolicy->pTurboSettings->PowerLimit1Power is in watts. We need to convert it to
      // CPU Power unit, specified in PACKAGE_POWER_SKU_UNIT_MSR[3:0].
      // Since we are converting from Watts to CPU power units, multiply by
      // PACKAGE_POWER_SKU_UNIT_MSR[3:0]. New TDP limit cannot be higher than
      // the fused TDP limit.
      //
      if (GetPcdTemp <= ppm->Info->PackageTdp[SocketNumber]) {
        ConvertedPowerLimit1 = (GetPcdTemp * ppm->Info->ProcessorPowerUnit[SocketNumber]);
      }

      //
      // OverClocking unit is detected
      //
      if (ppm->Info->CapId4Csr.Bits.oc_enabled) {
        ConvertedPowerLimit1 = (GetPcdTemp * ppm->Info->ProcessorPowerUnit[SocketNumber]);
      }
    }
    RaplLimitN0Csr.Bits.pkg_pwr_lim_1 = ConvertedPowerLimit1;

    //
    // Force Power Limit 1 override to be enabled
    //
    RaplLimitN0Csr.Bits.pkg_pwr_lim_1_en = 1;
    RaplLimitN0Csr.Bits.pkg_clmp_lim_1 = (UINT32)PkgPowerLimitMsr.Bits.PkgClmpLim1;

    //
    // Convert seconds to MSR value. Since not all values are programmable, we'll select
    // the entry from mapping table which is either equal to the user selected value. OR to a value in the mapping table
    // which is closest (but less than) to the user-selected value.
    //
    ConvertedPowerLimit1Time = mSecondsToMsrValueMapTable[0][1];
    ConvertedPowerLimit2Time = mSecondsToMsrValueMapTable[0][1];
    for(Index = 0; mSecondsToMsrValueMapTable[Index][0] != END_OF_TABLE; Index++) {
      if (TurboPowerLimit->PowerLimit1Time >= mSecondsToMsrValueMapTable[Index][0]) {
        ConvertedPowerLimit1Time = mSecondsToMsrValueMapTable[Index][1];
      }

      if (TurboPowerLimit->PowerLimit2Time >= mSecondsToMsrValueMapTable[Index][0]) {
        ConvertedPowerLimit2Time = mSecondsToMsrValueMapTable[Index][1];
      }

      if ((TurboPowerLimit->PowerLimit1Time < mSecondsToMsrValueMapTable[Index][0]) &&
          (TurboPowerLimit->PowerLimit2Time < mSecondsToMsrValueMapTable[Index][0])) {
        break;
      }
    }

    //
    // Configure Power Limit 1 (Long Duration Turbo) time windows: Turbo Power Limit MSR [23:17]
    //
    RaplLimitN0Csr.Bits.pkg_pwr_lim_1_time = ConvertedPowerLimit1Time;
    RaplLimitN1Csr.Bits.pkg_pwr_lim_2_time = ConvertedPowerLimit2Time;

    //
    // Initialize Short Duration Power limit and enable bit
    // Short duration Power Limit: Turbo Power Limit MSR (0x450h) [46:32]
    // Short duration Power Limit Enable:Turbo Power Limit MSR (0x450h) [47]
    //
    //
    // PPMPolicy->pTurboSettings->PowerLimit2Power value is in watts. We need to convert it to
    // CPU Power unit, specified in PACKAGE_POWER_SKU_UNIT_MSR[3:0].
    // Since we are converting from Watts to CPU power units, multiply by
    // PACKAGE_POWER_SKU_UNIT_MSR[3:0]
    //
    Mutliplier = 120;
    ConvertedShortDurationPowerLimit = (UINT16) EFI_IDIV_ROUND (MultU64x32(MultU64x32(Mutliplier, ppm->Info->PackageTdp[SocketNumber]), ppm->Info->ProcessorPowerUnit[SocketNumber]), 100);
    GetPcdTemp = (UINT16)PkgPowerLimitMsr.Bits.PkgPwrLim2;
    if ((GetPcdTemp > 0) && (GetPcdTemp < EFI_IDIV_ROUND (MultU64x32(Mutliplier, ppm->Info->PackageTdp[SocketNumber]), 100))) {
      ConvertedShortDurationPowerLimit = (UINT16) EFI_IDIV_ROUND (MultU64x32(MultU64x32 (Mutliplier, GetPcdTemp), ppm->Info->ProcessorPowerUnit[SocketNumber]), 100);
    }

    //
    // OverClocking unit is detected
    //
    if (ppm->Info->CapId4Csr.Bits.oc_enabled) {
      if(GetPcdTemp > 0) {
        ConvertedShortDurationPowerLimit = (UINT16) EFI_IDIV_ROUND (MultU64x32 (MultU64x32 (Mutliplier, GetPcdTemp), ppm->Info->ProcessorPowerUnit[SocketNumber]), 100);
      }
    }
    RaplLimitN1Csr.Bits.pkg_pwr_lim_2 = ConvertedShortDurationPowerLimit;
    if (PkgPowerLimitMsr.Bits.PkgPwrLim2En) {
      RaplLimitN1Csr.Bits.pkg_pwr_lim_2_en = 1;
    }
    RaplLimitN1Csr.Bits.pkg_clmp_lim_2 = (UINT32)PkgPowerLimitMsr.Bits.PkgClmpLim2;
  }

  // Program CSR_TURBO_POWER_LIMIT
  UsraCsrWrite (SocketNumber, 0, PACKAGE_RAPL_LIMIT_N0_PCU_FUN0_REG, RaplLimitN0Csr.Data);
  UsraCsrWrite (SocketNumber, 0, PACKAGE_RAPL_LIMIT_N1_PCU_FUN0_REG, RaplLimitN1Csr.Data);

  RegAddr = UsraGetCsrRegisterAddress (SocketNumber, 0, PACKAGE_RAPL_LIMIT_N0_PCU_FUN0_REG);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &RaplLimitN0Csr.Data);
  RegAddr = UsraGetCsrRegisterAddress (SocketNumber, 0, PACKAGE_RAPL_LIMIT_N1_PCU_FUN0_REG);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &RaplLimitN1Csr.Data);

  return;
}

/**

    - Enables Turbo Mode at package level
    - Based on user configuration
    - Configures Turbo Mode TDP power level and TDP power level time window
    - Configures Dynamic Turbo Power level and Dynamic Turbo Power time window
    - Enables Critical Power Clamp 1 and Critical Power Clamp 2

  @param ppm               Pointer to EFI_CPU_PM_STRUCT instance.
  @param TurboPowerLimit   Pointer to TURBO_POWRER_LIMIT instance.
  @param ProcessorNumber   Processor index that is executing.

  @retval None

**/
VOID
EFIAPI
InitializeRaplPowerLimitMsr (
  EFI_CPU_PM_STRUCT     *ppm,
  TURBO_POWRER_LIMIT    *TurboPowerLimit,
  UINTN                 ProcessorNumber
  )
{
  UINT16                           ConvertedPowerLimit1;
  UINT16                           ConvertedPowerLimit1Time;
  UINT16                           ConvertedPowerLimit2Time;
  UINT16                           ConvertedShortDurationPowerLimit;
  UINT16                           Index;
  UINT16                           Mutliplier;
  UINT16                           GetPcdTemp = 0;
  UINT16                           PackageTdp;
  UINT8                            ProcessorPowerUnit;
  UINT32                           RegEdx;
  UINT32                           CurrentSocket;
  ICX_MSR_PLATFORM_INFO_REGISTER   PlatformInfoMsr;
  MSR_PACKAGE_POWER_LIMIT_REGISTER PkgPowerLimitMsr;

  AsmCpuidEx(CPUID_EXTENDED_TOPOLOGY, 0, NULL, NULL, NULL, &RegEdx);
  CurrentSocket = (RegEdx >> ppm->Info->NumOfBitShift) ;

  if ((ppm->Info->PackageDieCount[CurrentSocket] > 1) && (ppm->Info->SlaveDieBitMap & (1 << CurrentSocket))) {
    // Slave Die is detected, skip MSR_PACKAGE_POWER_LIMIT program
    return;
  }

  ConvertedPowerLimit1Time = 0;
  Mutliplier = 0;
  PackageTdp          = ppm->Info->PackageTdp[CurrentSocket];
  ProcessorPowerUnit  = ppm->Info->ProcessorPowerUnit[CurrentSocket];

  PkgPowerLimitMsr.Uint64 = AsmReadMsr64 (MSR_PACKAGE_POWER_LIMIT);
  PlatformInfoMsr.Uint64 = AsmReadMsr64 (ICX_MSR_PLATFORM_INFO);
  //
  // Check if TDP limits are programmable
  // Platform Info MSR (0xCE) [29]
  //
  if (PlatformInfoMsr.Bits.PrgTdpLimEn != 0) {
    //
    // Initialize the Power Limit 1 and Power Limit 1 enable bit
    // Power Limit 1: Turbo Power Limit MSR [14:0]
    // Power Limit 1 Enable: Turbo Power Limit MSR [15]
    //
    //
    // By default, program Power Limit 1 to Package TDP limit
    //
    ConvertedPowerLimit1 = (UINT16) MultU64x32 (PackageTdp, ProcessorPowerUnit);
    GetPcdTemp = TurboPowerLimit->PowerLimit1Power;
    if (GetPcdTemp) {
      //
      // PPMPolicy->pTurboSettings->PowerLimit1Power is in watts. We need to convert it to
      // CPU Power unit, specified in PACKAGE_POWER_SKU_UNIT_MSR[3:0].
      // Since we are converting from Watts to CPU power units, multiply by
      // PACKAGE_POWER_SKU_UNIT_MSR[3:0]. New TDP limit cannot be higher than
      // the fused TDP limit.
      //
      if (GetPcdTemp <= PackageTdp) {
        ConvertedPowerLimit1 = (UINT16) MultU64x32 (GetPcdTemp, ProcessorPowerUnit);
      }

      //
      // OverClocking unit is detected
      //
      if (ppm->Info->CapId4Csr.Bits.oc_enabled) {
        ConvertedPowerLimit1 = (UINT16) MultU64x32 (GetPcdTemp, ProcessorPowerUnit);
      }
    }

    PkgPowerLimitMsr.Bits.PkgPwrLim1 = (UINT32) (ConvertedPowerLimit1);
    //
    // Force Power Limit 1 override to be enabled
    //
    PkgPowerLimitMsr.Bits.PkgPwrLim1En = 1;
    PkgPowerLimitMsr.Bits.PkgClmpLim1 = TurboPowerLimit->PkgClmpLim1;

    //
    // Convert seconds to MSR value. Since not all values are programmable, we'll select
    // the entry from mapping table which is either equal to the user selected value. OR to a value in the mapping table
    // which is closest (but less than) to the user-selected value.
    //
    ConvertedPowerLimit1Time = mSecondsToMsrValueMapTable[0][1];
    ConvertedPowerLimit2Time = mSecondsToMsrValueMapTable[0][1];
    for(Index = 0; mSecondsToMsrValueMapTable[Index][0] != END_OF_TABLE; Index++) {
      if (TurboPowerLimit->PowerLimit1Time >= mSecondsToMsrValueMapTable[Index][0]) {
        ConvertedPowerLimit1Time = mSecondsToMsrValueMapTable[Index][1];
      }

      if (TurboPowerLimit->PowerLimit2Time >= mSecondsToMsrValueMapTable[Index][0]) {
        ConvertedPowerLimit2Time = mSecondsToMsrValueMapTable[Index][1];
      }

      if ((TurboPowerLimit->PowerLimit1Time < mSecondsToMsrValueMapTable[Index][0]) &&
          (TurboPowerLimit->PowerLimit2Time < mSecondsToMsrValueMapTable[Index][0])) {
        break;
      }
    }

    //
    // Configure Power Limit 1 (Long Duration Turbo) time windows: Turbo Power Limit MSR [23:17]
    //
    PkgPowerLimitMsr.Bits.PkgPwrLim1Time = ConvertedPowerLimit1Time;
    PkgPowerLimitMsr.Bits.PkgPwrLim2Time = ConvertedPowerLimit2Time;

    //
    // PPMPolicy->pTurboSettings->PowerLimit2Power value is in watts. We need to convert it to
    // CPU Power unit, specified in PACKAGE_POWER_SKU_UNIT_MSR[3:0].
    // Since we are converting from Watts to CPU power units, multiply by
    // PACKAGE_POWER_SKU_UNIT_MSR[3:0]
    //
    Mutliplier = 120;
    ConvertedShortDurationPowerLimit = (UINT16) EFI_IDIV_ROUND (MultU64x64(MultU64x32(Mutliplier,PackageTdp), ProcessorPowerUnit), 100);

    GetPcdTemp = TurboPowerLimit->PowerLimit2Power;
    if ((GetPcdTemp > 0) && (GetPcdTemp < EFI_IDIV_ROUND (MultU64x32 (Mutliplier, PackageTdp), 100))) {
      ConvertedShortDurationPowerLimit = (UINT16) MultU64x32 (GetPcdTemp, ProcessorPowerUnit);
    }

    //
    // OverClocking unit is detected
    //
    if (ppm->Info->CapId4Csr.Bits.oc_enabled) {
      if(GetPcdTemp > 0) {
        ConvertedShortDurationPowerLimit = (UINT16) MultU64x32 (GetPcdTemp, ProcessorPowerUnit);
      }
    }
    PkgPowerLimitMsr.Bits.PkgPwrLim2 = (UINT32)ConvertedShortDurationPowerLimit;

    if (TurboPowerLimit->PowerLimit2En != 0) {
      PkgPowerLimitMsr.Bits.PkgPwrLim2En = 1;
    }
    PkgPowerLimitMsr.Bits.PkgClmpLim2 = TurboPowerLimit->PkgClmpLim2;
  }

  AsmWriteMsr64 (MSR_PACKAGE_POWER_LIMIT, PkgPowerLimitMsr.Uint64);

  //
  // Save for S3 if we are not locking MSR, otherwise we capture S3 save when we lock
  //
  if (TurboPowerLimit->TurboPowerLimitLock == 0) {
    WriteRegisterTable (ProcessorNumber, Msr, MSR_PACKAGE_POWER_LIMIT, 0, 64, PkgPowerLimitMsr.Uint64);
  }

  return;
}

/**
  This routine is called to program FAST_RAPL_NSTRIKE_PL2_DUTY_CYCLE. It will be call multiple time passing any the SocketNumber to be programmed.

  @param SocketNumber          Socket index that is executing.
  @param FastRaplDutyCycle     Fast Rapl Duty Cycle value.

  @retval                      None
**/
VOID
EFIAPI
InitializeFastRaplDutyCycle (
  UINT8             SocketNumber,
  UINT8             FastRaplDutyCycle
  )
{
  UINT32 PcuMailboxCommand = 0;
  UINT32 PcuMailboxData = 0;
  UINT32 Tempdata = 0;

  if (FastRaplDutyCycle > 0) {
    PcuMailboxData = FastRaplDutyCycle;
    PcuMailboxCommand = MAILBOX_BIOS_CMD_FAST_RAPL_NSTRIKE_PL2_DUTY_CYCLE;
    Tempdata = ReadModifyWritePcuMailbox (
                 SocketNumber,
                 0,
                 PcuMailboxCommand,
                 &PcuMailboxData,
                 0,
                 0
                 );
    PpmS3BootScriptSaveB2PWrite (SocketNumber, PcuMailboxCommand, PcuMailboxData);
  }

  return;
}
