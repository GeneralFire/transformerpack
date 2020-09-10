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

#include "Common/CpuPpmIncludes.h"
#include <Library/EmulationConfigurationLib.h>
#include <Library/PcuIpLib.h>
#include <Library/CpuAndRevisionLib.h>


/**
  Adds a record for a B2P write operation into a specified boot script table.

  @param SocketNumber    Socket index that is executing.
  @param B2pCommand      The B2P Command.
  @param B2pData         The B2P Data.

  @note  A known Limitations in the implementation which is 64bits operations are not supported.

**/
VOID
PpmS3BootScriptSaveB2PWrite (
  IN  UINT8   SocketNumber,
  IN  UINT32  B2pCommand,
  IN  UINT32  B2pData
  )
{
  UINT32    Value = 0;
  UINT32    BitOffset = BIT31;
  UINT32    PM_MBX_CMD = ((UINT32) B2pCommand | (UINT32) BIT31) ;
  UINT32    PM_MBX_DATA = (UINT32) B2pData;
  UINT64    InterfaceAddr;
  UINT64    DataAddr;


  InterfaceAddr = UsraGetCsrRegisterAddress (SocketNumber, 0, BIOS_MAILBOX_INTERFACE);
  DataAddr = UsraGetCsrRegisterAddress (SocketNumber, 0, BIOS_MAILBOX_DATA);

  S3BootScriptSaveMemPoll(S3BootScriptWidthUint32, InterfaceAddr, &BitOffset, &Value, MAILBOX_WAIT_STALL, MAILBOX_WAIT_TIMEOUT);
  S3BootScriptSaveMemWrite(S3BootScriptWidthUint32, DataAddr, 1, &PM_MBX_DATA);
  S3BootScriptSaveMemWrite(S3BootScriptWidthUint32, InterfaceAddr, 1, &PM_MBX_CMD);

  return;
}

/**
    Initializes the platform power management library. This must be called
    prior to any of the library functions being used.

    @param ppm            Pointer to EFI_CPU_PM_STRUCT.
    @param PpmPolicy      Pointer to PPM Policy protocol instance.

    @retval               None
**/
VOID
EFIAPI
InitializeCpuPPMLib (
  EFI_CPU_PM_STRUCT   *ppm,
  PPM_POLICY_CONFIGURATION *PpmPolicy
  )
{
  PACKAGE_POWER_SKU_UNIT_PCU_FUN0_STRUCT  PackagePowerSKUUnit; //pwr_unit, time_unit
  PACKAGE_POWER_SKU_N0_PCU_FUN0_STRUCT    PackagePowerSKU;     //pkg_tdp
  UINT8                                   ProcessorPowerUnit = 0;
  UINT32                                  ProcessorTimeUnit = 0;
  UINT8                                   SocketNumber;

  ppm->Version.Major = (UINT8)PPM_MAJOR;
  ppm->Version.Minor = (UINT8)PPM_MINOR;
  ppm->Version.Rev   = (UINT8)PPM_REV;
  ppm->Version.Build = (UINT8)PPM_BUILD;

  for (SocketNumber = 0; SocketNumber < MAX_SOCKET; SocketNumber++) {
    if ((ppm->Info->SocketPresentBitMap & (1 << SocketNumber)) == 0) {
      continue;
    }

    ppm->Info->PackageDieCount[SocketNumber] = PcuGetDieCount(SocketNumber);
    DEBUG ((EFI_D_INFO, ":PPM: [%d] PackageDieCount = %d,  SlaveDieBitMap = 0x%x\n",
            SocketNumber, ppm->Info->PackageDieCount[SocketNumber], ppm->Info->SlaveDieBitMap));
    DEBUG ((EFI_D_INFO, ":PPM: [%d] ConfigTDPLevel = %d,  CurrentPackageTdp = %d\n",
            SocketNumber, PpmPolicy->ConfigTDPLevel, PpmPolicy->CurrentPackageTdp));

    //
    // Get Processor TDP
    // Get Maximum Power from Turbo Power Limit CSR Bits[14:0]
    // and convert it to units specified by Package Power SKU
    // Unit CSR [3:0]
    //
    PackagePowerSKUUnit.Data = UsraCsrRead (SocketNumber, 0, PACKAGE_POWER_SKU_UNIT_PCU_FUN0_REG);
    PackagePowerSKU.Data = UsraCsrRead (SocketNumber, 0, PACKAGE_POWER_SKU_N0_PCU_FUN0_REG);

    ProcessorPowerUnit = (UINT8) PackagePowerSKUUnit.Bits.pwr_unit;
    ProcessorPowerUnit = (UINT8) LShiftU64 (1, ProcessorPowerUnit);

    ProcessorTimeUnit = (UINT8) PackagePowerSKUUnit.Bits.time_unit;
    ProcessorTimeUnit = (UINT32) LShiftU64 (1, ProcessorTimeUnit);

    ppm->Info->ProcessorPowerUnit[SocketNumber] = ProcessorPowerUnit;
    ppm->Info->ProcessorTimeUnit[SocketNumber]  = ProcessorTimeUnit;
    ppm->Info->PackageTdp[SocketNumber] = (UINT16) PpmPolicy->CurrentPackageTdp[SocketNumber];

    DEBUG ((EFI_D_INFO, "PPM - [%d] PackageTdp: %d;  ProcessorPowerUnit: %d, ProcessorTimeUnit: %d\n",
       SocketNumber, ppm->Info->PackageTdp[SocketNumber], ppm->Info->ProcessorPowerUnit[SocketNumber], ppm->Info->ProcessorTimeUnit[SocketNumber]));
  }

  return;
}


/**

  This routine is called to program MAILBOX_BIOS_CMD_EADR. It will be call multiple time passing any the SocketNumber to be programmed.

  @param ppm   Pointer to PPM Policy protocol instance

  @retval      None

**/
VOID
EFIAPI
ForceUncoreAndMeshRatio (
  EFI_CPU_PM_STRUCT *ppm
  )
{

}

/**

  This routine is called to program MAILBOX_BIOS_CMD_MISC_WORKAROUND_ENABLE;. It will be call multiple time passing any the SocketNumber to be programmed.

  @param SocketNumber    Socket index that is executing.
  @param AdvPwrMgtCtl    Pointer to ADV_PWR_CTL instance.

  @retval                None

**/
VOID
EFIAPI
MiscWorkaroundEnable (
  UINT8                  SocketNumber,
  ADV_PWR_CTL            *AdvPwrMgtCtl
  )
{
  UINT8                         WdogTimerEn;
  EFI_STATUS                    PcodeMailboxStatus;

  if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    WdogTimerEn = AdvPwrMgtCtl->PcodeWdogTimerEn;
    if (WdogTimerEn == 2) {
      if (IsCpuAndRevision (CPU_ICXSP, REV_RX)) {
        WdogTimerEn = 0;
      }
      else {
        WdogTimerEn = 1;
      }
    }

    if (WdogTimerEn == 0) {
      PcodeMailboxStatus = PcuSetMiscWorkaroundEnable (SocketNumber, MAILBOX_BIOS_CMD_MISC_WORKAROUND_ENABLE_WATCHDOG_TIMER_DISABLE);   // send command to disable watchdog timer
      if ((PcodeMailboxStatus & 0x000000ff) != MAILBOX_BIOS_CC_INVALID_COMMAND) {
        PpmS3BootScriptSaveB2PWrite (SocketNumber, MAILBOX_BIOS_CMD_MISC_WORKAROUND_ENABLE, MAILBOX_BIOS_CMD_MISC_WORKAROUND_ENABLE_WATCHDOG_TIMER_DISABLE);
      } else {
        DEBUG ((EFI_D_ERROR, "Failed to disable watchdog timer in current socket (%d) !  Error code: %d \n\n", SocketNumber, PcodeMailboxStatus));
      }
    }
  }
}

/**

  This routine is called to program MSR BIOS_AVX_DIS. It will be call multiple time passing any the Processor to be programmed.

  @param ppm              Pointer to EFI_CPU_PM_STRUCT.
  @param AvxSupport       Platform setting for AvxSupport.
  @param ProcessorNumber  Processor index that is executing.

  @retval EFI_SUCCESS

**/
VOID
EFIAPI
AvxDisConfig (
  EFI_CPU_PM_STRUCT     *ppm,
  UINT8                 AvxSupport,
  UINTN                 ProcessorNumber
  )
{
  BOOLEAN                             AvxInst;
  BOOLEAN                             Avx3Inst;
  CPUID_VERSION_INFO_ECX              VersionInfoEcx;
  CPUID_EXTENDED_STATE_MAIN_LEAF_EAX  XStateMainLeafEax;
  ICX_MSR_BIOS_AVX_DIS_REGISTER       BiosAvxDisMsr;

  AvxInst  = FALSE;
  Avx3Inst = FALSE;

  //
  // AVX support setting disabled and overclocking supported
  //
  if ((AvxSupport == 0) && (ppm->Info->CapId4Csr.Bits.oc_enabled)) {
    //
    // Get CPUID(1).ECX[28] to check if processor supports the AVX instruction extensions
    //
    AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, &VersionInfoEcx.Uint32, NULL);
    if (VersionInfoEcx.Bits.AVX == 1) {
      AvxInst = TRUE;
    }

    //
    // CPUID.(EAX=0Dh, ECX=00h):EAX[7:5] - AVX-512 state.
    //
    AsmCpuidEx (
      CPUID_EXTENDED_STATE, CPUID_EXTENDED_STATE_MAIN_LEAF,
      &XStateMainLeafEax.Uint32, NULL, NULL, NULL
      );
    if (XStateMainLeafEax.Bits.AVX_512 > 0) {
      Avx3Inst = TRUE;
    }

    if (AvxInst && Avx3Inst) {
      BiosAvxDisMsr.Uint64 = AsmReadMsr64 (ICX_MSR_BIOS_AVX_DIS);
      BiosAvxDisMsr.Bits.AvxDis = 1;

      AsmWriteMsr64 (ICX_MSR_BIOS_AVX_DIS, BiosAvxDisMsr.Uint64);
      //save for S3
      WriteRegisterTable (ProcessorNumber, Msr, ICX_MSR_BIOS_AVX_DIS, 0, 64, BiosAvxDisMsr.Uint64);
    }
  }
}

/**

  This routine is called to program MAILBOX_BIOS_CMD_MISC_WORKAROUND_ENABLE;. It will be call multiple time passing any the SocketNumber to be programmed.

  @param SocketNumber          Socket index that is executing.
  @param AvxLicensePreGrant    AVX License PreGrant value.
  @param AvxIccpLevel          AVX ICCP Level value.

  @retval                      None

**/
VOID
EFIAPI
InitializeAVXICCPPreGrantLevel (
  UINT8                 SocketNumber,
  UINT8                 AvxLicensePreGrant,
  UINT8                 AvxIccpLevel
  )
{
  B2P_READ_WRITE_PM_CONFIG_INTERFACE_STRUCT    WritePmCfgCmdB2p;
  UINT32                                       MailBoxData = 0;
  UINT32                                       Tempdata = 0;

  if (AvxLicensePreGrant == 0) {
    return;
  }

  WritePmCfgCmdB2p.Data = 0;

  WritePmCfgCmdB2p.Bits.command = (UINT32) MAILBOX_BIOS_CMD_WRITE_PM_CONFIG;
  WritePmCfgCmdB2p.Bits.SubCommand = (UINT32) MAILBOX_BIOS_CMD_WRITE_PM_CONFIG_INTERFACE_SUB_COMMAND_MIN_ICCP_LEVEL;

  MailBoxData = AvxIccpLevel;

  DEBUG((EFI_D_ERROR, " \n: PPM: Set B2P WRITE_PM_CONFIG(0x%x) for Avx ICCP Level, Value = %d in S#%d\n", WritePmCfgCmdB2p.Data, MailBoxData, SocketNumber));
  Tempdata = ReadModifyWritePcuMailbox (
                 SocketNumber,
                 0,
                 WritePmCfgCmdB2p.Data,
                 &MailBoxData,
                 0,
                 0
                 );
  if ((Tempdata & 0x000000ff) != MAILBOX_BIOS_CC_INVALID_COMMAND) {
    PpmS3BootScriptSaveB2PWrite (SocketNumber, WritePmCfgCmdB2p.Data, MailBoxData);
  }
}

/**

  This routine is called to program MAILBOX_BIOS_CMD_WRITE_LIMCA_CONFIG:Limca Disable. It will be call multiple time passing any the SocketNumber to be programmed.

  @param SocketNumber          Socket index that is executing.

  @retval                      None
**/
VOID
EFIAPI
InitializeLimca (
  UINT8         SocketNumber
  )
{
  UINT32                                  PcodeMailboxStatus = 0;
  B2P_WRITE_LIMCA_CONFIG_DATA_STRUCT      WritePmConfigB2p;
  B2P_READ_WRITE_PM_CONFIG_INTERFACE_STRUCT    WritePmCfgCmdB2p;

  if (IsCpuAndRevision (CPU_ICXSP, REV_RX) || IsCpuAndRevision (CPU_ICXSP, REV_L0)) {
    WritePmConfigB2p.Data = 0;
    WritePmCfgCmdB2p.Data = 0;
    WritePmCfgCmdB2p.Bits.command = (UINT32) MAILBOX_BIOS_CMD_WRITE_LIMCA_CONFIG;
    WritePmConfigB2p.Bits.LimcaDisable = 1;
    DEBUG((EFI_D_INFO, " \n: PPM: ReadModifyWrite B2P MAILBOX_BIOS_CMD_WRITE_LIMCA_CONFIG(0x%x), Setting = 0x%x in S#%d\n", MAILBOX_BIOS_CMD_WRITE_LIMCA_CONFIG, WritePmConfigB2p.Data, SocketNumber));
    PcodeMailboxStatus = ReadModifyWritePcuMailbox (SocketNumber, MAILBOX_BIOS_CMD_READ_LIMCA_CONFIG, MAILBOX_BIOS_CMD_WRITE_LIMCA_CONFIG, &WritePmConfigB2p.Data, (UINT32)-1, WritePmConfigB2p.Data);
    DEBUG((EFI_D_INFO, " \n: PPM: Read B2P MAILBOX_BIOS_CMD_READ_LIMCA_CONFIG(0x%x), Value = 0x%x in S#%d\n", MAILBOX_BIOS_CMD_READ_LIMCA_CONFIG, WritePmConfigB2p.Data, SocketNumber));
    if ((PcodeMailboxStatus & 0x000000ff) != MAILBOX_BIOS_CC_INVALID_COMMAND) {
      PpmS3BootScriptSaveB2PWrite (SocketNumber, WritePmCfgCmdB2p.Data, WritePmConfigB2p.Data);
    } else {
      DEBUG ((EFI_D_ERROR, " \n :PPM: Failed to program B2P MAILBOX_BIOS_CMD_WRITE_LIMCA_CONFIG in current socket (%d) !!!  Error code: %d \n\n", SocketNumber, PcodeMailboxStatus));
    }
  }
}

/**

  This routine is called to program MAILBOX_BIOS_CMD_WRITE_PM_CONFIG:GPSS Timer Value. It will be call multiple time passing any the SocketNumber to be programmed.

  @param SocketNumber          Socket index that is executing.
  @param GpssTimer             GPSS timer value.

  @retval                      None

**/
VOID
EFIAPI
InitializeGpssTimer (
  UINT8                 SocketNumber,
  UINT8                 GpssTimer
  )
{
  B2P_READ_WRITE_PM_CONFIG_INTERFACE_STRUCT    WritePmCfgCmdB2p;
  UINT32                                       MailBoxData = 0;
  UINT32                                       Tempdata = 0;

  WritePmCfgCmdB2p.Data = 0;

  WritePmCfgCmdB2p.Bits.command = (UINT32) MAILBOX_BIOS_CMD_WRITE_PM_CONFIG;
  WritePmCfgCmdB2p.Bits.SubCommand = (UINT32) MAILBOX_BIOS_CMD_WRITE_PM_CONFIG_INTERFACE_SUB_COMMAND_GPSS_TIMER;

  MailBoxData = (UINT32) (GpssTimer * 1000);

  DEBUG((EFI_D_INFO, " \n: PPM: Set B2P WRITE_PM_CONFIG(0x%x) for GPSS Timer Value, Value = %d in S#%d\n", WritePmCfgCmdB2p.Data, MailBoxData, SocketNumber));
  Tempdata = ReadModifyWritePcuMailbox (
                 SocketNumber,
                 0,
                 WritePmCfgCmdB2p.Data,
                 &MailBoxData,
                 0,
                 0
                 );
  if ((Tempdata & 0x000000ff) != MAILBOX_BIOS_CC_INVALID_COMMAND) {
    PpmS3BootScriptSaveB2PWrite (SocketNumber, WritePmCfgCmdB2p.Data, MailBoxData);
  }
}

/**

  This routine is a placeholder for 10nm processors for which this initialization isn't needed

  @param SocketNumber          Socket index that is executing.

  @retval                      None

**/
VOID
EFIAPI
InitializeDcstLuts (
  UINT8       SocketNumber
  )
{
  return;
}
