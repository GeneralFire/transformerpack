/** @file
  Code File for CPU Power Management

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2020 Intel Corporation. <BR>

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
#include <Library/PcuMailBoxLib.h>
#include <Library/CpuAndRevisionLib.h>

/**
  Checks FUSE_HWP_ENABLE support for HWPM feature for specified processor.

  This function retuns of HWPM Interface supported if FUSE_HWP_ENABLE is enabled

  @param SocketNumber          Socket index that is executing.

  @retval                      HWPM feature support status

**/
UINT8
GetHwpmSupport (
   UINT8     SocketNumber
  )
{
  UINT8                       HwpSupport;
  CAPID5_PCU_FUN3_STRUCT      CapId5;

  CapId5.Data = UsraCsrRead (SocketNumber, 0, CAPID5_PCU_FUN3_REG);
  HwpSupport = (UINT8)CapId5.Bits.hwp_en;

  return HwpSupport;
}

/**
  Enables HWPM Interface feature  for specified processor.

  This function enables of HWPM Interface for specified processor.

  @param Hwpm             Pointer to PPM_HWPM_STRUCT instance
  @param ProcessorNumber  Processor index that is executing

  @retval                 None

**/
VOID
InitializeHWPMInterface (
  PPM_HWPM_STRUCT     *Hwpm,
  UINTN               ProcessorNumber
  )
{
  MSR_IA32_PM_ENABLE_REGISTER     PmEnableMsr;
  MSR_MISC_PWR_MGMT_REGISTER      MiscPwrMsr;

  //
  // Read MSR_MISC_PWR_MGMT
  //
  MiscPwrMsr.Uint64 = AsmReadMsr64 (MSR_MISC_PWR_MGMT);
  MiscPwrMsr.Bits.EnableHwp = 0;
  MiscPwrMsr.Bits.EnableHwpInterrupt = 0;
  MiscPwrMsr.Bits.EnableOutOfBandAutonomous = 0;
  MiscPwrMsr.Bits.EnableHwpEpp = 0;

  if(Hwpm->HWPMNative) { //if setup has choosen Native or Native w/o Legacy mode
    MiscPwrMsr.Bits.EnableHwp = 1;
  } else if (Hwpm->HWPMOOB) {
    MiscPwrMsr.Bits.EnableOutOfBandAutonomous = 1;
  }

  if(Hwpm->HWPMInterrupt) {
    MiscPwrMsr.Bits.EnableHwpInterrupt = 1;
  }

  if (Hwpm->EPPEnable) {
    MiscPwrMsr.Bits.EnableHwpEpp = 1;
  }

  //
  // HWPM Ineterface setup
  //
  AsmWriteMsr64 (MSR_MISC_PWR_MGMT, MiscPwrMsr.Uint64);
  WriteRegisterTable (ProcessorNumber, Msr, MSR_MISC_PWR_MGMT, 0, 64, MiscPwrMsr.Uint64);

  if (Hwpm->HWPMEnable == HWP_MODE_NATIVE_NO_LEGACY) {
    PmEnableMsr.Uint64 = AsmReadMsr64 (MSR_IA32_PM_ENABLE);
    PmEnableMsr.Bits.HWP_ENABLE = 1;
    AsmWriteMsr64 (MSR_IA32_PM_ENABLE, PmEnableMsr.Uint64);
    WriteRegisterTable(ProcessorNumber, Msr, MSR_IA32_PM_ENABLE, 0, 64, PmEnableMsr.Uint64);
  }

  return;
}

/**
  Enables HWP interrupts in MSR IA32_HWP_INTERRUPT (773h)

  @param ProcessorNumber  Processor index that is executing
  @retval                 None

**/
VOID
EnableHwpLvtThermalInterrupt(
  UINTN               ProcessorNumber
  )
{
  MSR_IA32_HWP_INTERRUPT_REGISTER   HwpInterruptMsr;

  ///
  /// Set bits 0 and 1 of MSR IA32_HWP_INTERRUPT (773h) to enable HWP interrupts
  ///
  HwpInterruptMsr.Uint64 = AsmReadMsr64 (MSR_IA32_HWP_INTERRUPT);
  HwpInterruptMsr.Bits.EN_Guaranteed_Performance_Change = 1;
  HwpInterruptMsr.Bits.EN_Excursion_Minimum = 1;
  AsmWriteMsr64 (MSR_IA32_HWP_INTERRUPT, HwpInterruptMsr.Uint64);
  //S3 save
  WriteRegisterTable (ProcessorNumber, Msr, MSR_IA32_HWP_INTERRUPT, 0, 64, HwpInterruptMsr.Uint64);

  return;
}

/**
  This function check and enable the HWP features.

  @param ppm              Pointer to EFI_CPU_PM_STRUCT instance.
  @param Hwpm             Pointer to PPM_HWPM_STRUCT instance
  @param ProcessorNumber  Processor index that is executing

  @retval                 None

**/
VOID
EFIAPI
InitialzeHwpFeatures (
  EFI_CPU_PM_STRUCT   *ppm,
  PPM_HWPM_STRUCT     *Hwpm,
  UINTN               ProcessorNumber
  )
{
  if (ppm->Info->HwpmSupport) {
    InitializeHWPMInterface (Hwpm, ProcessorNumber);  //enable HWP Native or OOB, Interrupt
    if(Hwpm->HWPMInterrupt) {
      EnableHwpLvtThermalInterrupt(ProcessorNumber); //enable Lvt
    }
  }
  return;
}

/**

  This routine is called to program PCU_MISC_CONFIG.
  It will be call multiple time passing any the SocketNumber to be programmed.

  @param SocketNumber          Socket index that is executing.
  @param PowerCtrl             Pointer to POWER_CTL instance.
  @param Hwpm                  Pointer to PPM_HWPM_STRUCT instance.

  @retval                      None

**/
VOID
EFIAPI
InitializeHWPMMiscConfig (
  UINT8             SocketNumber,
  POWER_CTL         *PowerCtrl,
  PPM_HWPM_STRUCT   *Hwpm
  )
{
  UINT32            Tempdata = 0;
  B2P_PCU_MISC_CONFIG_DATA_STRUCT                  PcuMiscConfigB2pOrData;
  B2P_OOB_INIT_EPP_DATA_STRUCT                     OobInitEppB2p;
  B2P_STANDARD_INTERFACE_STRUCT                    OobInitEppCmdB2p;
  B2P_EPB_PECI_OVERRIDE_CONTROL_INTERFACE_STRUCT   EpbPeciCtlCmdB2p;
  B2P_EPB_PECI_OVERRIDE_CONTROL_DATA_STRUCT        EpbPeciCtlData2p;

  PcuMiscConfigB2pOrData.Data = 0;
  OobInitEppB2p.Data    = 0;
  OobInitEppCmdB2p.Data = 0;
  EpbPeciCtlCmdB2p.Data = 0;
  EpbPeciCtlData2p.Data = 0;

  if (Hwpm->HWPMEnable) {

    if (!Hwpm->APSrocketing) {
      PcuMiscConfigB2pOrData.Bits.DisAPSrocketing = 1;
    }

    if (!Hwpm->Scalability) {
      PcuMiscConfigB2pOrData.Bits.DisScalabilityCore = 1;
    }
  }

  ReadModifyWritePcuMailbox (
    SocketNumber,
    MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG,
    MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG,
    &Tempdata,
    (UINT32)-1,
    PcuMiscConfigB2pOrData.Data
    );

  if ((IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) && (PowerCtrl->PwrPerfTuning != PWR_PERF_TUNING_BIOS_CONTROL)) {
    //BIOS knob for 'Out of band alternate EPB enable'
    if (Hwpm->OutofBandAlternateEPB) {
      SetPcuBiosSpare2Csr (SocketNumber, PCU_OUT_OF_BAND_ALTERNATE_EPB);
    }
  }

  if (!(IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) && (PowerCtrl->PwrPerfTuning == PWR_PERF_TUNING_PECI_CONTROL)) {
    //PECI Controls EPB
    EpbPeciCtlCmdB2p.Bits.command = (UINT32) MAILBOX_BIOS_CMD_WRITE_EPB_PECI_OVERRIDE_CONTROL;
    EpbPeciCtlData2p.Bits.EPB = 1;
    Tempdata = ReadModifyWritePcuMailbox (
                 SocketNumber,
                 0,
                 EpbPeciCtlCmdB2p.Data,
                 &EpbPeciCtlData2p.Data,
                 0,
                 0
                 );
    PpmS3BootScriptSaveB2PWrite (SocketNumber, EpbPeciCtlCmdB2p.Data, EpbPeciCtlData2p.Data);
    if (Tempdata > 0) {
      DEBUG ( (EFI_D_ERROR, " \n\n :: !!! BIOS failed to program B2P WRITE_EPB_PECI_OVERRIDE_CONTROL on socket (%d). Pcode returns (%d) !!!. \n\n", SocketNumber, Tempdata) );
    }
  }

  //HWP Out of band BIOS EPP initialization through B2P mailbox
  if (Hwpm->HWPMOOB && Hwpm->EPPEnable) {
    OobInitEppB2p.Bits.Default_Epp = (UINT32) Hwpm->EPPProfile;
    OobInitEppCmdB2p.Bits.command = (UINT32) MAILBOX_BIOS_CMD_OOB_INIT_EPP;
    OobInitEppCmdB2p.Bits.read_write = 1;
    //DEBUG ( (EFI_D_ERROR, " \n :: BIOS programs MAILBOX_BIOS_CMD_OOB_INIT_EPP for ALT_EPB on socket (%d). Value = 0x%x \n", SocketNumber, OobInitEppB2p.Data) );
    Tempdata = ReadModifyWritePcuMailbox (
                 SocketNumber,
                 0,
                 OobInitEppCmdB2p.Data,
                 &OobInitEppB2p.Data,
                 0,
                 0
                 );
    PpmS3BootScriptSaveB2PWrite (SocketNumber, OobInitEppCmdB2p.Data, OobInitEppB2p.Data);
    if (Tempdata > 0) {
      DEBUG ( (EFI_D_ERROR, " \n\n :: !!! BIOS failed to program ALT_EPB on socket (%d). Pcode returns (%d) !!!. \n\n", SocketNumber, Tempdata) );
    }
  }
}

/**

  This routine is called to program MSR_IA32_HWP_REQUEST not programed
  elsewhere. It will be call multiple time passing any the node to be programmed.

  @param ppm              Pointer to EFI_CPU_PM_STRUCT instance.
  @param Hwpm             Pointer to PPM_HWPM_STRUCT instance.
  @param ProcessorNumber  Processor index that is executing

  @retval                 None

**/
VOID
EFIAPI
InitialzeConfigurePbf (
  EFI_CPU_PM_STRUCT   *ppm,
  PPM_HWPM_STRUCT     *Hwpm,
  UINTN               ProcessorNumber
  )
{
  MSR_IA32_HWP_REQUEST_REGISTER   HwpRequestMsr;
  MSR_THD_LOGICAL_ID_REGISTER     ThreadLogicalIdMsr;
  UINT32                          CurrentSocket;
  UINT32                          LogicalCoreId;
  UINT8                           MinPerfRatio;

  if (!Hwpm->ConfigurePbf) {
    return;
  }

  CurrentSocket = PpmGetSocketFromProcessNumber (ProcessorNumber);

  ThreadLogicalIdMsr.Uint64 = AsmReadMsr32 (MSR_THD_LOGICAL_ID);
  LogicalCoreId = (ThreadLogicalIdMsr.Bits.LogicalId >> 1);

  //
  // Find matching bit index in PbfHighPriCoreMap by logical core ID.
  //
  if (RShiftU64 (Hwpm->PbfHighPriCoreMap[CurrentSocket], LogicalCoreId) & 1) {
    MinPerfRatio = Hwpm->PbfP1HighRatio[CurrentSocket];
  } else {
    return;
  }

  //
  // Update MSR_IA32_HWP_REQUEST_REGISTER.Minimum_Performance Bit[7:0].
  //
  HwpRequestMsr.Uint64 = AsmReadMsr64 (MSR_IA32_HWP_REQUEST);
  HwpRequestMsr.Bits.Minimum_Performance = MinPerfRatio;
  AsmWriteMsr64 (MSR_IA32_HWP_REQUEST, HwpRequestMsr.Uint64);

  //
  // S3 save
  //
  WriteRegisterTable (ProcessorNumber, Msr, MSR_IA32_HWP_REQUEST, 0, 64, HwpRequestMsr.Uint64);

  return;
}

/**

  This function is to detect HwpmSupport.

  @param[in]          SocketPresentBitMap   Bitmap of socket Id with CPUs present detected by KTI RC.

  @retval TRUE        If Hwpm Supported
  @retval FALSE       If Hwpm doesn't Supported

**/
BOOLEAN
EFIAPI
IsHwpmSupported (
  IN    UINT32   SocketPresentBitMap
  )
{
  UINT8           Socket;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (SocketPresentBitMap & (1 << Socket)) {
      if (GetHwpmSupport (Socket) != 1) {
        return FALSE;
      }
    }
  }
  return TRUE;
}
