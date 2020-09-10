/** @file
  This file is SampleCode of the library for Intel PCH PEI Policy initialization.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains a 'Sample Driver' and is licensed as such under the terms
  of your license agreement with Intel or your vendor. This file may be modified
  by the user, subject to the additional terms of the license agreement.

@par Specification Reference:
**/

#include <Ppi/ReadOnlyVariable2.h>
#include <PchSetupVariableCdfEbg.h>
#include <Guid/SetupVariable.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PchPolicyLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPolicyLib.h>
#include <PchSetupVariableCdfEbg.h>
#include <Library/GbeLib.h>
#include <Library/SataSocLib.h>
#include <Library/RstCrLib.h>
#include <Ppi/Spi.h>
#include <Library/UbaUsbOcUpdateLib.h>
#include <Library/UbaPcieBifurcationUpdateLib.h>

/**
  Update Pm config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdatePmConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  PCH_PM_CONFIG                *PmConfig;
  EFI_STATUS                   Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gPmConfigGuid, (VOID *) &PmConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  PmConfig->PchDeepSxPol                  = PchSetup->DeepSxMode;
  PmConfig->WakeConfig.WolEnableOverride  = PchSetup->PchWakeOnLan;
  PmConfig->WakeConfig.WoWlanEnable       = PchSetup->PchWakeOnWlan;
  PmConfig->WakeConfig.WoWlanDeepSxEnable = PchSetup->PchWakeOnWlanDeepSx;
  PmConfig->WakeConfig.LanWakeFromDeepSx  = PchSetup->LanWakeFromDeepSx;
  PmConfig->SlpLanLowDc                   = PchSetup->SlpLanLowDc;

  // Power Button Override Period (PBOP): refer to EDS for detail
  // Encoding:
  // 000b - 4 seconds
  // 011b - 10 seconds
  // power button behavior will depend on this setting and other features
  // such as power button SMI which will shutdown system immediately when it is enabled in pre-OS.
  PmConfig->PwrBtnOverridePeriod = 0;

  PmConfig->DisableDsxAcPresentPulldown = PchSetup->DisableDsxAcPresentPulldown;
  PmConfig->PciePllSsc                  = PchSetup->PciePllSsc;

  //
  // TCO Timer Enable required for Simics
  //
  if (IsSimicsEnvironment ()) {
    PmConfig->EnableTcoTimer = TRUE;
  } else {
    PmConfig->EnableTcoTimer = PchSetup->EnableTcoTimer;
  }

  PmConfig->ModPhySusPgEnable   = FALSE;
  PmConfig->PmcDbgMsgEn         = PchSetup->PmcDbgMsgEn;
  PmConfig->OsIdleEnable        = FALSE;
  PmConfig->EnableTimedGpio0    = PchSetup->EnableTimedGpio0;
  PmConfig->EnableTimedGpio1    = PchSetup->EnableTimedGpio1;

  PmConfig->GlobalResetMasksOverride     = PchSetup->GlobalResetMasksOverride;
  if (PmConfig->GlobalResetMasksOverride) {
    PmConfig->GlobalResetTriggerMask.Value = PchSetup->GlobalResetEventMask;
    PmConfig->GlobalResetEventMask.Value   = PchSetup->GlobalResetTriggerMask;
  }
}

/**
  Update Espi config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateEspiConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  PCH_ESPI_CONFIG  *EspiConfig;
  EFI_STATUS       Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gEspiConfigGuid, (VOID *) &EspiConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  EspiConfig->LgmrEnable          = PchSetup->PchEspiLgmrEnable;
  EspiConfig->HostC10ReportEnable = PchSetup->PchEspiHostC10ReportEnable;
  EspiConfig->EspiPmHAE           = FALSE;
}


/**
  Add verb table helper function.
  This function calculates verbtable number and shows verb table information.

  @param[in,out] VerbTableEntryNum      Input current VerbTable number and output the number after adding new table
  @param[in,out] VerbTableArray         Pointer to array of VerbTable
  @param[in]     VerbTable              VerbTable which is going to add into array
**/
STATIC
VOID
InternalAddVerbTable (
  IN OUT  UINT8                   *VerbTableEntryNum,
  IN OUT  UINT32                  *VerbTableArray,
  IN      HDAUDIO_VERB_TABLE      *VerbTable
  )
{
  if (VerbTable == NULL) {
    DEBUG ((DEBUG_INFO, "InternalAddVerbTable wrong input: VerbTable == NULL\n"));
    return;
  }

  VerbTableArray[*VerbTableEntryNum] = (UINT32) VerbTable;
  *VerbTableEntryNum += 1;

  DEBUG ((DEBUG_INFO,
    "HDA: Add verb table for vendor = 0x%04X devId = 0x%04X (size = %d DWords)\n",
    VerbTable->Header.VendorId,
    VerbTable->Header.DeviceId,
    VerbTable->Header.DataDwords)
    );
}

/**
  Add verb table function.
  This function update the verb table number and verb table ptr of policy.

  @param[in] HdAudioConfig            HDAudio config block
  @param[in] HdAudioPreMemConfig      HDAudio PreMem config block
**/
STATIC
VOID
InternalAddPlatformVerbTables (
  IN  HDAUDIO_CONFIG                  *HdAudioConfig,
  IN  HDAUDIO_PREMEM_CONFIG           *HdAudioPreMemConfig
  )
{
  UINT8                           VerbTableEntryNum;
  UINT32                          VerbTableArray[6];
  UINT32                          *VerbTablePtr;

  VerbTableEntryNum = 0;

  if (!!HdAudioPreMemConfig->AudioLinkHda.Enable) {
    InternalAddVerbTable (&VerbTableEntryNum, VerbTableArray, (VOID *) (UINTN) PcdGet32 (PcdHdaVerbTable));
    InternalAddVerbTable (&VerbTableEntryNum, VerbTableArray, (VOID *) (UINTN) PcdGet32 (PcdHdaVerbTable2));
  }

  HdAudioConfig->VerbTableEntryNum = VerbTableEntryNum;

  VerbTablePtr = (UINT32 *) AllocateZeroPool (sizeof (UINT32) * VerbTableEntryNum);
  CopyMem (VerbTablePtr, VerbTableArray, sizeof (UINT32) * VerbTableEntryNum);
  HdAudioConfig->VerbTablePtr = (UINT32) VerbTablePtr;
}

/**
  Update HD Audio configuration.

  @param[in] SiPolicy       Pointer to SI_POLICY_PPI
  @param[in] SiPreMemPolicy Pointer to SI_POLICY_PREMEM_PPI
  @param[in] PchSetup  Pointer to PCH_SETUP
**/
STATIC
VOID
UpdateHdAudioConfig (
  IN SI_POLICY_PPI        *SiPolicy,
  IN SI_PREMEM_POLICY_PPI *SiPreMemPolicy,
  IN PCH_SETUP            *PchSetup
  )
{
  HDAUDIO_CONFIG         *HdAudioConfig;
  HDAUDIO_PREMEM_CONFIG  *HdAudioPreMemConfig;
  EFI_STATUS             Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gHdAudioConfigGuid, (VOID *) &HdAudioConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gHdAudioPreMemConfigGuid, (VOID *) &HdAudioPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  HdAudioConfig->CodecSxWakeCapability = PchSetup->PchHdAudioCodecSxWakeCapability;
  HdAudioConfig->Pme                   = PchSetup->PchHdAudioPme;
  HdAudioConfig->HdAudioLinkFrequency  = PchSetup->PchHdaHdAudioLinkFreq;

  InternalAddPlatformVerbTables (HdAudioConfig, HdAudioPreMemConfig);
}

/**
  This function performs PCH Serial IO Platform Policy initialization

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
VOID
UpdateSerialIoConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  UINT8                 Index;
  SERIAL_IO_CONFIG      *SerialIoConfig;
  EFI_STATUS            Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gSerialIoConfigGuid, (VOID *) &SerialIoConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // UART
  //
  // Translate the EBG UART PCH Setup values to the IP block policy values
  for (Index = 0; Index < GetPchMaxSerialIoUartControllersNum (); Index++) {
    SerialIoConfig->UartDeviceConfig[Index].Mode                = (UINT8)(SERIAL_IO_UART_MODE)PchSetup->PchSerialIoUart[Index];
    SerialIoConfig->UartDeviceConfig[Index].Attributes.AutoFlow = PchSetup->PchUartHwFlowCtrl[Index];
    SerialIoConfig->UartDeviceConfig[Index].PowerGating         = PchSetup->PchUartPowerGating[Index];
    SerialIoConfig->UartDeviceConfig[Index].DmaEnable           = PchSetup->PchUartDmaEnable[Index];
  }
}

/**
  Update SATA Controllers configuration.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] PchSetup  Pointer to PCH_SETUP
**/
STATIC
VOID
UpdateSataConfig (
  IN SI_POLICY_PPI  *SiPolicy,
  IN PCH_SETUP      *PchSetup
  )
{
  SATA_CONFIG         *SataConfig;
  UINT32              SataCtrlIndex;
  UINT8               Index;
  UINT8               MaxSataPorts;

  SataConfig = NULL;

  for (SataCtrlIndex = 0; SataCtrlIndex < MaxSataControllerNum (); SataCtrlIndex++) {

    SataConfig = GetPchSataConfig (SiPolicy, SataCtrlIndex);

    SataConfig->Enable   = PchSetup->PchSata[SataCtrlIndex];
    SataConfig->SataMode = PchSetup->SataInterfaceMode[SataCtrlIndex];

    SataConfig->RaidDeviceId     = PchSetup->RaidDeviceId[SataCtrlIndex];
    SataConfig->SataRstInterrupt = PchSetup->SataRstInterrupt[SataCtrlIndex];

    SataConfig->SalpSupport      = PchSetup->SataSalp[SataCtrlIndex];

    SataConfig->SpeedLimit       = PchSetup->SataSpeed[SataCtrlIndex];

    SataConfig->LedEnable        = FALSE;
    SataConfig->SgpioSupport     = PchSetup->SataSgpioSupport[SataCtrlIndex];

    SataConfig->ThermalThrottling.SuggestedSetting = PchSetup->PchSataTsSuggestSet[SataCtrlIndex];
    SataConfig->ThermalThrottling.P1TDispFinit     = PchSetup->PchP1TDispFinit[SataCtrlIndex];
    SataConfig->ThermalThrottling.P1Tinact         = PchSetup->PchP1Tinact[SataCtrlIndex];
    SataConfig->ThermalThrottling.P1TDisp          = PchSetup->PchP1TDisp[SataCtrlIndex];
    SataConfig->ThermalThrottling.P1T3M            = PchSetup->PchP1T3M[SataCtrlIndex];
    SataConfig->ThermalThrottling.P1T2M            = PchSetup->PchP1T2M[SataCtrlIndex];
    SataConfig->ThermalThrottling.P1T1M            = PchSetup->PchP1T1M[SataCtrlIndex];
    SataConfig->ThermalThrottling.P0TDispFinit     = PchSetup->PchP0TDispFinit[SataCtrlIndex];
    SataConfig->ThermalThrottling.P0Tinact         = PchSetup->PchP0Tinact[SataCtrlIndex];
    SataConfig->ThermalThrottling.P0TDisp          = PchSetup->PchP0TDisp[SataCtrlIndex];
    SataConfig->ThermalThrottling.P0T3M            = PchSetup->PchP0T3M[SataCtrlIndex];
    SataConfig->ThermalThrottling.P0T2M            = PchSetup->PchP0T2M[SataCtrlIndex];
    SataConfig->ThermalThrottling.P0T1M            = PchSetup->PchP0T1M[SataCtrlIndex];

    MaxSataPorts = MaxSataPortNum (SataCtrlIndex);

    // If SataDevSlpPort[SataCtrlIndex] is equal to 0xFF, DevSlp is not enabled for any port on the controller.
    if ((PchSetup->SataDevSlpPort[SataCtrlIndex] < MaxSataPorts) || PchSetup->SataSgpioSupport[SataCtrlIndex]) {
      SataConfig->PortSettings[PchSetup->SataDevSlpPort[SataCtrlIndex]].DevSlp = TRUE;
    }

    switch (SataCtrlIndex) {
      case SATA_1_CONTROLLER_INDEX:
        for (Index = 0; Index < MaxSataPorts; Index++) {
          if (PchSetup->SataTestMode[SataCtrlIndex] == TRUE) {
            SataConfig->PortSettings[Index].Enable = TRUE;
          } else {
            SataConfig->PortSettings[Index].Enable = PchSetup->SataPortController0[Index];
          }
          SataConfig->PortSettings[Index].HotPlug          = PchSetup->SataHotPlugController0[Index];
          if (SataConfig->SgpioSupport == TRUE) {
            SataConfig->PortSettings[Index].InterlockSw      = FALSE;
          }
          else {
            SataConfig->PortSettings[Index].InterlockSw      = PchSetup->SataMechanicalSwController0[Index];
          }
          SataConfig->PortSettings[Index].SpinUp           = PchSetup->SataSpinUpController0[Index];
          SataConfig->PortSettings[Index].External         = PchSetup->SataExternalController0[Index];
          SataConfig->PortSettings[Index].EnableDitoConfig = PchSetup->EnableDitoConfigController0[Index];
          SataConfig->PortSettings[Index].DmVal            = PchSetup->DmValController0[Index];
          SataConfig->PortSettings[Index].DitoVal          = PchSetup->DitoValController0[Index];
          SataConfig->PortSettings[Index].SolidStateDrive  = PchSetup->SataTypeController0[Index];
          SataConfig->EnclosureSupport                     = TRUE;
        }
        break;

      case SATA_2_CONTROLLER_INDEX:
        for (Index = 0; Index < MaxSataPorts; Index++) {
          if (PchSetup->SataTestMode[SataCtrlIndex] == TRUE) {
            SataConfig->PortSettings[Index].Enable = TRUE;
          } else {
            SataConfig->PortSettings[Index].Enable = PchSetup->SataPortController1[Index];
          }
          SataConfig->PortSettings[Index].HotPlug          = PchSetup->SataHotPlugController1[Index];
          if (SataConfig->SgpioSupport == TRUE) {
            SataConfig->PortSettings[Index].InterlockSw      = FALSE;
          }
          else {
            SataConfig->PortSettings[Index].InterlockSw      = PchSetup->SataMechanicalSwController1[Index];
          }
          SataConfig->PortSettings[Index].SpinUp           = PchSetup->SataSpinUpController1[Index];
          SataConfig->PortSettings[Index].External         = PchSetup->SataExternalController1[Index];
          SataConfig->PortSettings[Index].EnableDitoConfig = PchSetup->EnableDitoConfigController1[Index];
          SataConfig->PortSettings[Index].DmVal            = PchSetup->DmValController1[Index];
          SataConfig->PortSettings[Index].DitoVal          = PchSetup->DitoValController1[Index];
          SataConfig->PortSettings[Index].SolidStateDrive  = PchSetup->SataTypeController1[Index];
          SataConfig->EnclosureSupport                     = TRUE;
        }
        break;

      case SATA_3_CONTROLLER_INDEX:
        for (Index = 0; Index < MaxSataPorts; Index++) {
          if (PchSetup->SataTestMode[SataCtrlIndex] == TRUE) {
            SataConfig->PortSettings[Index].Enable = TRUE;
          } else {
            SataConfig->PortSettings[Index].Enable = PchSetup->SataPortController2[Index];
          }
          SataConfig->PortSettings[Index].HotPlug          = PchSetup->SataHotPlugController2[Index];
          if (SataConfig->SgpioSupport == TRUE) {
            SataConfig->PortSettings[Index].InterlockSw    = FALSE;
          }
          else {
            SataConfig->PortSettings[Index].InterlockSw    = PchSetup->SataMechanicalSwController2[Index];
          }
          SataConfig->PortSettings[Index].SpinUp           = PchSetup->SataSpinUpController2[Index];
          SataConfig->PortSettings[Index].External         = PchSetup->SataExternalController2[Index];
          SataConfig->PortSettings[Index].EnableDitoConfig = PchSetup->EnableDitoConfigController2[Index];
          SataConfig->PortSettings[Index].DmVal            = PchSetup->DmValController2[Index];
          SataConfig->PortSettings[Index].DitoVal          = PchSetup->DitoValController2[Index];
          SataConfig->PortSettings[Index].SolidStateDrive  = PchSetup->SataTypeController2[Index];
          SataConfig->EnclosureSupport                     = FALSE;
        }
        break;

      default:
        break;
    }
  }
}

/**
  Update GBE config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateGbeConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  GBE_CONFIG                   *GbeConfig;
  EFI_STATUS                   Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gGbeConfigGuid, (VOID *) &GbeConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
  //
  // LAN Config
  // Check Gbe Region to determine if hide LAN controller setup option
  //
  GbeConfig->Enable = IsGbePresent () ? PchSetup->PchLan : FALSE;
}

/**
  Update DMI config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateDmiConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  PCH_DMI_CONFIG                  *DmiConfig;
  EFI_STATUS                      Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gPchDmiConfigGuid, (VOID *) &DmiConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  DmiConfig->DmiAspmCtrl       = PchSetup->PchDmiAspm;
  DmiConfig->L1RpCtl           = FALSE;
  DmiConfig->DmiPowerReduction = FALSE;
  DmiConfig->ClockGating       = PchSetup->PchRlinkClockGating;
}

/**
  Update Psf config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdatePsfConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{

}

/**
  Update PCH General config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdatePchGeneralConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  PCH_GENERAL_CONFIG           *PchGeneralConfig;
  EFI_STATUS            Status;
  Status = GetConfigBlock ((VOID *) SiPolicy, &gPchGeneralConfigGuid, (VOID *) &PchGeneralConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  PchGeneralConfig->Crid = PchSetup->PchCrid;
}

/**
  Update RST configuration.

  @param[in] SiPolicy     Pointer to SI_POLICY_PPI
  @param[in] PchSetup     Pointer to PCH_SETUP
**/
STATIC
VOID
UpdateRstConfig (
  IN SI_POLICY_PPI  *SiPolicy,
  IN PCH_SETUP      *PchSetup
  )
{
  RST_CONFIG          *RstConfig;
  UINT32              SataCtrlIndex;

  for (SataCtrlIndex = 0; SataCtrlIndex < MaxSataControllerNum(); SataCtrlIndex++){
    RstConfig = GetPchRstConfig(SiPolicy, SataCtrlIndex);

    RstConfig->LegacyOrom   = PchSetup->SataLegacyOrom[SataCtrlIndex];
    RstConfig->Raid0        = PchSetup->SataRaidR0[SataCtrlIndex];
    RstConfig->Raid1        = PchSetup->SataRaidR1[SataCtrlIndex];
    RstConfig->Raid10       = PchSetup->SataRaidR10[SataCtrlIndex];
    RstConfig->Raid5        = PchSetup->SataRaidR5[SataCtrlIndex];
    RstConfig->Irrt         = PchSetup->SataRaidIrrt[SataCtrlIndex];
    RstConfig->OromUiBanner = PchSetup->SataRaidOub[SataCtrlIndex];
    RstConfig->HddUnlock    = PchSetup->SataHddlk[SataCtrlIndex];
    RstConfig->LedLocate    = PchSetup->SataLedl[SataCtrlIndex];
    RstConfig->IrrtOnly     = PchSetup->SataRaidIooe[SataCtrlIndex];
    RstConfig->SmartStorage = PchSetup->SataRaidSrt[SataCtrlIndex];
    RstConfig->OromUiDelay  = PchSetup->SataRaidOromDelay[SataCtrlIndex];

    RstConfig->OptaneMemory       = PchSetup->SataRstOptaneMemory;
    RstConfig->CpuAttachedStorage = PchSetup->SataRstCpuAttachedStorage;
  }
}


/**
  Update PCIe Root Port Configuration

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
VOID
UpdatePcieRpConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  PCH_PCIE_CONFIG                *PcieRpConfig;
  EFI_STATUS                     Status;
  UINT8                          MaxPciePorts;
  UINT8                          Index;
  PCIE_BIFURCATION_CONFIG        *PcieUbaBifurcation;
  PCH_SLOT_CONFIG_DATA_ENTRY_EX  *SlotUbaConfig;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gPchPcieConfigGuid, (VOID *) &PcieRpConfig);
  ASSERT_EFI_ERROR (Status);

  Status = PlatformGetPchPcieBifurcationConfig (&PcieUbaBifurcation,&SlotUbaConfig);
  ASSERT_EFI_ERROR (Status);

  MaxPciePorts = GetPchMaxPciePortNum();

  for (Index = 0; Index < MaxPciePorts; Index++) {
    PcieRpConfig->RootPort[Index].PcieRpCommonConfig.PhysicalSlotNumber             = (UINT8) Index;
    PcieRpConfig->RootPort[Index].PcieRpCommonConfig.ClkReqDetect                   = FALSE;
    PcieRpConfig->RootPort[Index].PcieRpCommonConfig.Aspm                           = PchSetup->PcieRootPortAspm[Index];
    PcieRpConfig->RootPort[Index].PcieRpCommonConfig.L1Substates                    = PchSetup->PcieRootPortL1SubStates[Index];
    PcieRpConfig->RootPort[Index].PcieRpCommonConfig.PcieSpeed                      = PchSetup->PcieRootPortSpeed[Index];
    PcieRpConfig->RootPort[Index].PcieRpCommonConfig.AcsEnabled                     = PchSetup->PcieRootPortACS[Index];
    PcieRpConfig->RootPort[Index].PcieRpCommonConfig.PtmEnabled                     = PchSetup->PcieRootPortPTM[Index];
    PcieRpConfig->RootPort[Index].MvcEnabled                                        = PchSetup->PcieRootPortMVC[Index];
    PcieRpConfig->RootPort[Index].PcieRpCommonConfig.DpcEnabled                     = PchSetup->PcieRootPortDPC[Index];
    PcieRpConfig->RootPort[Index].PcieRpCommonConfig.RpDpcExtensionsEnabled         = PchSetup->PcieRootPortEDPC[Index];
    PcieRpConfig->RootPort[Index].PcieRpCommonConfig.SlotImplemented                = PchSetup->PcieRootPortSI[Index];
    PcieRpConfig->RootPort[Index].PcieRpCommonConfig.MaxPayload                     = PchSetup->PcieRootPortMPS[Index];
    PcieRpConfig->RootPort[Index].PcieRpCommonConfig.RelaxedOrder                   = PchSetup->PcieRootPortRO[Index];
    PcieRpConfig->RootPort[Index].PcieRpCommonConfig.NoSnoop                        = PchSetup->PcieRootPortNS[Index];

    if (PchSetup->PcieRootPortHPE[Index]) {
      PcieRpConfig->RootPort[Index].PcieRpCommonConfig.SlotImplemented = TRUE;
    }
    PcieRpConfig->RootPort[Index].PcieRpCommonConfig.PmSci                          = PchSetup->PcieRootPortPMCE[Index];
    PcieRpConfig->RootPort[Index].PcieRpCommonConfig.HotPlug                        = PchSetup->PcieRootPortHPE[Index];
    PcieRpConfig->RootPort[Index].PcieRpCommonConfig.AdvancedErrorReporting         = PchSetup->PcieRootPortAER[Index];
    PcieRpConfig->RootPort[Index].PcieRpCommonConfig.DetectTimeoutMs                = PchSetup->PcieDetectTimeoutMs[Index];
    PcieRpConfig->RootPort[Index].PcieRpCommonConfig.CompletionTimeout              = PchSetup->PcieRootPortCompletionTimeout[Index];
    PcieRpConfig->RootPort[Index].PcieRpCommonConfig.LtrEnable                      = PchSetup->PchPcieLtrEnable[Index];
    PcieRpConfig->RootPort[Index].VppPort                                           = SlotUbaConfig[Index].VppPort;
    PcieRpConfig->RootPort[Index].VppAddress                                        = SlotUbaConfig[Index].VppAddress;
    PcieRpConfig->RootPort[Index].PcieRpCommonConfig.MrlSensorPresent               = PchSetup->PcieRootPortMrls[Index];
  }
  PcieRpConfig->PcieCommonConfig.ComplianceTestMode                               = PchSetup->PcieComplianceTestMode;

  // [SPR A0 PO]ASPM issue cause the BMC link was getting dropped for root port 7,  No BMC Video
  PcieRpConfig->RootPort[7].PcieRpCommonConfig.Aspm = 0x0;
  PcieRpConfig->RootPort[7].PcieRpCommonConfig.L1Substates  = 0x0;
}

/**
  Check if BIOS Update is going to happen during this boot.

  @retval TRUE       BIOS Update is going to happen during this boot.
  @retval FALSE      No request of BIOS update is detected.

**/
STATIC
BOOLEAN
IsBiosUpdateRequired (
  VOID
  )
{
// EBG_TODO check if boot mode is BOOT_ON_FLASH_UPDATE
  return FALSE;
}

/**
  Get Bios Guard variable

  @param[in] CpuSecurityPreMemConfig   Pointer to CPU Security PreMem Config
**/
STATIC
UINT32
GetBiosGuardEnable (
  VOID
  )
{
  // EBG_TODO add detection if BIOS Guard is enabled
  return FALSE;
}


/**
  Update LockDown config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
  @param[in] BiosGuardEnable      BIOS Guard enable
**/
STATIC
VOID
UpdateLockDownConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup,
  IN UINT32                    BiosGuardEnable
  )
{
  PCH_LOCK_DOWN_CONFIG            *LockDownConfig;
  EFI_STATUS                      Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gLockDownConfigGuid, (VOID *) &LockDownConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // BiosGuard is capable of overriding PCH BIOS protection.
  // So only need to consider the case when BiosGuard is disabled here
  //
  if (IsBiosUpdateRequired () && !BiosGuardEnable) {
    DEBUG ((DEBUG_INFO, "Disabling BIOS lock for BIOS Update Process\n"));
    PchSetup->PchBiosLock = 0;

    DEBUG ((DEBUG_INFO, "Disabling BIOS interface lockdown for supporting Fault tolerant BIOS Update\n"));
    LockDownConfig->BiosInterface = FALSE;
  }

  if (BiosGuardEnable) {
    DEBUG ((DEBUG_INFO, "Enable InSMM.STS (EISS) and BIOS Lock Enable (BLE) since BIOS Guard is enabled.\n"));
    LockDownConfig->BiosLock = TRUE;
  } else {
    LockDownConfig->BiosLock = PchSetup->PchBiosLock;
  }
}

/**
  Update Rtc config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateRtcConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  RTC_CONFIG                   *RtcConfig;
  EFI_STATUS                   Status;
  Status = GetConfigBlock ((VOID *) SiPolicy, &gRtcConfigGuid, (VOID *) &RtcConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  if (PchSetup->PchAllUnLock == TRUE) {
    RtcConfig->BiosInterfaceLock = FALSE;
    RtcConfig->MemoryLock = FALSE;
  }
  else {
    RtcConfig->BiosInterfaceLock = TRUE;
    RtcConfig->MemoryLock = PchSetup->PchRtcMemoryLock;
  }
}

/**
  Update FlashProtection config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
  @param[in] BiosGuardEnable      BIOS Guard enable
**/
STATIC
VOID
UpdateFlashProtectionConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup,
  IN UINT32                    BiosGuardEnable
  )
{
  UINT32                          BaseAddr;
  UINT32                          RegionSize;
  UINT32                          EfiNvVariableStorageBase;
  UINT32                          EfiNvVariableStorageSize;
  BOOLEAN                         FlashProtectionEnabled;
  EFI_STATUS                      Status;
  PCH_SPI_PPI                     *SpiPpi;
  UINT16                          ProtectedRangeBase;
  UINT16                          ProtectedRangeLimit;
  PCH_FLASH_PROTECTION_CONFIG     *FlashProtectionConfig;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gFlashProtectionConfigGuid, (VOID *) &FlashProtectionConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  FlashProtectionEnabled = PchSetup->FprrEnable;

  //
  // BiosGuard is capable of overriding PCH BIOS protection.
  // So only need to consider the case when BiosGuard is disabled here
  //
  if (IsBiosUpdateRequired () && !BiosGuardEnable) {
    DEBUG ((DEBUG_INFO, "Disable Flash Protection Range Registers for BIOS Update Process\n"));
    FlashProtectionEnabled = FALSE;
  }

  //
  // Flash Protection Range Register initialization
  //
  if (FlashProtectionEnabled) {
    Status = PeiServicesLocatePpi (
               &gPchSpiPpiGuid,
               0,
               NULL,
               (VOID **)&SpiPpi
               );

    DEBUG ((DEBUG_INFO, "Enabling Flash Protection Range: LocatePpi gPchSpiPpiGuid = %r\n", Status));
    if (EFI_ERROR (Status)) {
      return;
    }
    // Find the base address for BIOS region
    //
    Status = SpiPpi->GetRegionAddress (SpiPpi, FlashRegionBios, &BaseAddr, &RegionSize);
    DEBUG ((DEBUG_INFO, "BIOS Region: %r - Start Address: 0x%x - Size: 0x%x\n", Status, BaseAddr, RegionSize));
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      return;
    }
    //
    // Initialize flash protection
    //
    FlashProtectionConfig->ProtectRange[0].WriteProtectionEnable = TRUE;
    FlashProtectionConfig->ProtectRange[0].ReadProtectionEnable  = FALSE;
    //  APTIOV_SERVER_OVERRIDE_RC_START: AMI Porting 
    // EIP 555091
    //FlashProtectionConfig->ProtectRange[1].WriteProtectionEnable = TRUE;
    //FlashProtectionConfig->ProtectRange[1].ReadProtectionEnable  = FALSE;
    //
    // Assign FPRR ranges
    //
    // Enable BIOS region protection on Flash device
    // with exclusion of EFI Variable Storage area
    //

    //EfiNvVariableStorageBase = FixedPcdGet32 (PcdFlashFvWheaBase) - FixedPcdGet32 (PcdFlashFdMainBase);
    //EfiNvVariableStorageSize = FixedPcdGet32 (PcdFlashFvWheaSize) + FixedPcdGet32 (PcdFlashNvStorageVariableSize) + FixedPcdGet32 (PcdFlashFvNvStorageEventLogSize)
    //                 + FixedPcdGet32 (PcdFlashNvStorageFtwWorkingSize) + FixedPcdGet32 (PcdFlashNvStorageFtwSpareSize);
    EfiNvVariableStorageBase = NVRAM_OFFSET;
#if defined(FAULT_TOLERANT_NVRAM_UPDATE) && (FAULT_TOLERANT_NVRAM_UPDATE == 1)
    EfiNvVariableStorageSize = NVRAM_SIZE*2 + FV_WHEA_SIZE;
#else
    EfiNvVariableStorageSize = NVRAM_SIZE + FV_WHEA_SIZE;
#endif
     

    //FlashProtectionConfig->ProtectRange[0].ProtectedRangeBase  = (UINT16)(BaseAddr >> 12);
    //FlashProtectionConfig->ProtectRange[0].ProtectedRangeLimit = (UINT16)((BaseAddr + EfiNvVariableStorageBase - 1) >> 12);
    //FlashProtectionConfig->ProtectRange[1].ProtectedRangeBase  = (UINT16)((BaseAddr + EfiNvVariableStorageBase + EfiNvVariableStorageSize) >> 12);
    //FlashProtectionConfig->ProtectRange[1].ProtectedRangeLimit = (UINT16)(((BaseAddr + RegionSize) - 1) >> 12);
    FlashProtectionConfig->ProtectRange[0].ProtectedRangeBase  = (UINT16)((BaseAddr + EfiNvVariableStorageBase + EfiNvVariableStorageSize) >> 12);
    FlashProtectionConfig->ProtectRange[0].ProtectedRangeLimit = (UINT16)(((BaseAddr + RegionSize) - 1) >> 12);

    ProtectedRangeBase  = FlashProtectionConfig->ProtectRange[0].ProtectedRangeBase;
    ProtectedRangeLimit = FlashProtectionConfig->ProtectRange[0].ProtectedRangeLimit;
    DEBUG ((DEBUG_INFO, "FlashProtectionConfig->ProtectRange[0].ProtectedRangeBase  %x\n", ProtectedRangeBase));
    DEBUG ((DEBUG_INFO, "FlashProtectionConfig->ProtectRange[0].ProtectedRangeLimit %x\n", ProtectedRangeLimit));

    //ProtectedRangeBase  = FlashProtectionConfig->ProtectRange[1].ProtectedRangeBase;
    //ProtectedRangeLimit = FlashProtectionConfig->ProtectRange[1].ProtectedRangeLimit;
    //DEBUG ((DEBUG_INFO, "FlashProtectionConfig->ProtectRange[1].ProtectedRangeBase  %x\n", ProtectedRangeBase));
    //DEBUG ((DEBUG_INFO, "FlashProtectionConfig->ProtectRange[1].ProtectedRangeLimit %x\n", ProtectedRangeLimit));
    //  APTIOV_SERVER_OVERRIDE_RC_END: AMI Porting
  }
}

/**
  Update Ieh config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateIehConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  IEH_CONFIG                  *IehConfig;
  EFI_STATUS                  Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gIehConfigGuid, (VOID *) &IehConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  IehConfig->Mode = PchSetup->IehMode;
}

/**
  Update IO APIC Configuration

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
VOID
UpdateIoApicConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  PCH_IOAPIC_CONFIG   *IoApicConfig;
  EFI_STATUS          Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gIoApicConfigGuid, (VOID *) &IoApicConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  IoApicConfig->IoApicEntry24_119 = PchSetup->PchIoApic24119Entries;
  //
  // To support SLP_S0, it's required to disable 8254 timer.
  // Note that CSM may require this option to be disabled for correct operation.
  // Once 8254 timer disabled, some legacy OPROM and legacy OS will fail while using 8254 timer.
  // For some OS environment that it needs to set 8254CGE in late state it should
  // This is also required during S3 resume.
  //
  // The Enable8254ClockGatingOnS3 is only applicable when Enable8254ClockGating is disabled.
  // If Enable8254ClockGating is enabled, RC will do 8254 CGE programming on S3 as well.
  // else, RC will do the programming on S3 when Enable8254ClockGatingOnS3 is enabled.
  // This avoids the SMI requirement for the programming.
  //
  if (PchSetup->Enable8254ClockGating == 1) {
    IoApicConfig->Enable8254ClockGating = TRUE;
  } else if (PchSetup->Enable8254ClockGating == 2) {
    IoApicConfig->Enable8254ClockGating     = FALSE;
    IoApicConfig->Enable8254ClockGatingOnS3 = TRUE;
  } else {
    IoApicConfig->Enable8254ClockGating     = FALSE;
    IoApicConfig->Enable8254ClockGatingOnS3 = FALSE;
  }
}


/**
  Update Interrupt config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateInterruptConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  PCH_INTERRUPT_CONFIG         *InterruptConfig;
  EFI_STATUS                   Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gInterruptConfigGuid, (VOID *) &InterruptConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  InterruptConfig->GpioIrqRoute = PchSetup->PchGpioIrqRoute;
}

/**
  Update Thermal config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateThermalConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  THERMAL_CONFIG                  *ThermalConfig;
  EFI_STATUS                      Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gThermalConfigGuid, (VOID *) &ThermalConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  ThermalConfig->PchHotEnable = PcdGetBool (PcdPchThermalHotEnable);

  //
  // Program Thermal Throttling Level
  //
  ThermalConfig->TTLevels.SuggestedSetting   = PchSetup->PchTtLevelSuggestSet;
  ThermalConfig->TTLevels.PchCrossThrottling = PchSetup->PchCrossThrottling;
  ThermalConfig->TTLevels.TTLock             = PchSetup->PchThrmTtLock;
  ThermalConfig->TTLevels.TTState13Enable    = PchSetup->PchThrmTtState13Enable;
  ThermalConfig->TTLevels.TTEnable           = PchSetup->PchThrmTtEnable;
  ThermalConfig->TTLevels.T2Level            = PchSetup->PchThrmT2Level;
  ThermalConfig->TTLevels.T1Level            = PchSetup->PchThrmT1Level;
  ThermalConfig->TTLevels.T0Level            = PchSetup->PchThrmT0Level;

  //
  // Program DMI Thermal Throttling
  //
  ThermalConfig->DmiHaAWC.SuggestedSetting = PchSetup->PchDmiTsSuggestSet;
  ThermalConfig->DmiHaAWC.TS3TW            = PchSetup->PchTs3Width;
  ThermalConfig->DmiHaAWC.TS2TW            = PchSetup->PchTs2Width;
  ThermalConfig->DmiHaAWC.TS1TW            = PchSetup->PchTs1Width;
  ThermalConfig->DmiHaAWC.TS0TW            = PchSetup->PchTs0Width;
  ThermalConfig->DmiHaAWC.DmiTsawEn        = PchSetup->PchDmiTsawEn;
}

/**
  This function updates USB OverCurrent policy mapping based on board design

  @param[in]  UsbConfig        Pointer to USB_CONFIG data buffer
**/
VOID
UpdateUsbOverCurrentPolicy (
  IN USB_CONFIG                 *UsbConfig
  )
{
  EFI_STATUS            Status;
  USB_OVERCURRENT_PIN   *Usb20OverCurrentMappings;
  USB_OVERCURRENT_PIN   *Usb30OverCurrentMappings;
  USB2_PHY_PARAMETERS   *Usb20AfeParams;
  UINT8                 PortIndex;

  Usb20OverCurrentMappings = NULL;
  Usb30OverCurrentMappings = NULL;
  Usb20AfeParams = NULL;

  Status = PlatformGetUsbOcMappings (
             (USB_OVERCURRENT_PIN **) &Usb20OverCurrentMappings,
             (USB_OVERCURRENT_PIN **) &Usb30OverCurrentMappings,
             (USB2_PHY_PARAMETERS **) &Usb20AfeParams
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error while getting USB OC mapping tables.\n"));
    return;
  }

  //
  // Updating USB2 OC pin mapping
  //
  if (Usb20OverCurrentMappings == NULL) {
    DEBUG ((DEBUG_WARN, "%a: No data for USB2 OC pin mapping provided.\n", __FUNCTION__));
  } else {
    for (PortIndex = 0; PortIndex < GetPchUsb2MaxPhysicalPortNum (); PortIndex++) {
      if ((Usb20OverCurrentMappings[PortIndex] < USB_OC_MAX_PINS) ||
          (Usb20OverCurrentMappings[PortIndex] == USB_OC_SKIP)) {
        UsbConfig->PortUsb20[PortIndex].OverCurrentPin = Usb20OverCurrentMappings[PortIndex];
      } else {
        DEBUG ((DEBUG_ERROR, "%a: Invalid OverCurrent pin specified USB2 port %d\n", __FUNCTION__, PortIndex));
      }
    }
  }

  //
  // Updating USB3 OC pin mapping
  //
  if (Usb30OverCurrentMappings == NULL) {
    DEBUG ((DEBUG_WARN, "%a: No data for USB3 OC pin mapping provided.\n", __FUNCTION__));
  } else {
    for (PortIndex = 0; PortIndex < GetPchXhciMaxUsb3PortNum (); PortIndex++) {
      if ((Usb30OverCurrentMappings[PortIndex] < USB_OC_MAX_PINS) ||
          (Usb30OverCurrentMappings[PortIndex] == USB_OC_SKIP)) {
        UsbConfig->PortUsb30[PortIndex].OverCurrentPin = Usb30OverCurrentMappings[PortIndex];
      } else {
        DEBUG ((DEBUG_ERROR, "%a: Invalid OverCurrent pin specified USB3 port %d\n", __FUNCTION__, PortIndex));
      }
    }
  }
}

/**
  Update Usb config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateUsbConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  UINTN           PortIndex;
  USB_CONFIG      *UsbConfig;
  EFI_STATUS      Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gUsbConfigGuid, (VOID *) &UsbConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  UsbConfig->PdoProgramming    = PchSetup->PchUsbPdoProgramming;
  // EBG A0 PO WA: temporary disable overcurrent until we get proper OC configutation form SV
  // UsbConfig->OverCurrentEnable = !!PchSetup->PchEnableDbcObs ? FALSE : PchSetup->PchUsbOverCurrentEnable;
  UsbConfig->OverCurrentEnable = FALSE;

  for (PortIndex = 0; PortIndex < GetPchUsb2MaxPhysicalPortNum (); PortIndex++) {
    UsbConfig->PortUsb20[PortIndex].Enable = !!PchSetup->PchUsbHsPort[PortIndex];
  }
  for (PortIndex = 0; PortIndex < GetPchXhciMaxUsb3PortNum (); PortIndex++) {
    UsbConfig->PortUsb30[PortIndex].Enable = !!PchSetup->PchUsbSsPort[PortIndex];
  }

  //
  // xDCI (USB device) related settings from setup variable
  //
  UsbConfig->XdciConfig.Enable = FALSE;

  UpdateUsbOverCurrentPolicy (UsbConfig);
}


/**
  Update USB2 PHY config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdatePchUsb2PhyConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  EFI_STATUS            Status;
  USB_OVERCURRENT_PIN   *Usb20OverCurrentMappings;
  USB_OVERCURRENT_PIN   *Usb30OverCurrentMappings;
  USB2_PHY_PARAMETERS   *Usb20AfeParams;
  UINT8                 PortIndex;
  USB2_PHY_CONFIG       *Usb2PhyConfig;

  Usb20OverCurrentMappings = NULL;
  Usb30OverCurrentMappings = NULL;
  Usb20AfeParams = NULL;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gUsb2PhyConfigGuid, (VOID *) &Usb2PhyConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  Status = PlatformGetUsbOcMappings (
             (USB_OVERCURRENT_PIN **) &Usb20OverCurrentMappings,
             (USB_OVERCURRENT_PIN **) &Usb30OverCurrentMappings,
             (USB2_PHY_PARAMETERS **) &Usb20AfeParams
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error while getting USB2 PHY tuning table.\n"));
    return;
  }

  if (Usb20AfeParams == NULL) {
    DEBUG ((DEBUG_WARN, "%a: No data for USB2 PHY tuning provided.\n", __FUNCTION__));
  } else {
    for (PortIndex = 0; PortIndex < GetPchUsb2MaxPhysicalPortNum (); PortIndex++) {
      Usb2PhyConfig->Port[PortIndex].Petxiset  = Usb20AfeParams[PortIndex].Petxiset;
      Usb2PhyConfig->Port[PortIndex].Txiset    = Usb20AfeParams[PortIndex].Txiset;
      Usb2PhyConfig->Port[PortIndex].Predeemp  = Usb20AfeParams[PortIndex].Predeemp;
      Usb2PhyConfig->Port[PortIndex].Pehalfbit = Usb20AfeParams[PortIndex].Pehalfbit;
    }
  }
}

/**
  Update ADR config

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateAdrConfig (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  EFI_STATUS        Status;
  ADR_CONFIG    *AdrConfig;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gAdrConfigGuid, (VOID *) &AdrConfig);
  ASSERT_EFI_ERROR (Status);

  if (PchSetup->PchAdrEn != PLATFORM_POR) {
    AdrConfig->AdrEn = PchSetup->PchAdrEn;
  }

  if (PchSetup->PchAdrTimerEn != PLATFORM_POR) {
    AdrConfig->AdrTimerEn = PchSetup->PchAdrTimerEn;
  }
  AdrConfig->AdrTimer1Val = PchSetup->PchAdrTimer1Val;
  AdrConfig->AdrMultiplier1Val = PchSetup->PchAdrMultiplier1Val;
  AdrConfig->AdrTimer2Val = PchSetup->PchAdrTimer2Val;
  AdrConfig->AdrMultiplier2Val = PchSetup->PchAdrMultiplier2Val;
  AdrConfig->AdrHostPartitionReset = PchSetup->AdrHostPartitionReset;
}

/**
  This function performs PCH PEI Policy initialization.

  @param[in, out] SiPolicy        The SI Policy PPI instance

  @retval EFI_SUCCESS             The PPI is installed and initialized.
  @retval EFI ERRORS              The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
UpdatePeiPchPolicy (
  IN OUT      SI_POLICY_PPI     *SiPolicy
  )
{
  EFI_STATUS                      Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;
  UINTN                           VariableSize;
  SYSTEM_CONFIGURATION            SystemConfiguration;
  PCH_SETUP                       PchSetup;
  UINT32                          BiosGuardEnable;
  SI_PREMEM_POLICY_PPI            *SiPreMemPolicy;

  DEBUG ((DEBUG_INFO, "Update UpdatePeiPchPolicy Post-Mem start\n"));

  BiosGuardEnable = GetBiosGuardEnable ();

  //
  // Retrieve Setup variable
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid, // GUID
             0,                                // INSTANCE
             NULL,                             // EFI_PEI_PPI_DESCRIPTOR
             (VOID **) &VariableServices       // PPI
             );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (SYSTEM_CONFIGURATION);
  Status = VariableServices->GetVariable (
    VariableServices,
    L"Setup",
    &gEfiSetupVariableGuid,
    NULL,
    &VariableSize,
    &SystemConfiguration
    );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (PCH_SETUP);
  Status = VariableServices->GetVariable (
    VariableServices,
    L"PchSetup",
    &gPchSetupVariableGuid,
    NULL,
    &VariableSize,
    &PchSetup
    );
  ASSERT_EFI_ERROR (Status);

  Status = PeiServicesLocatePpi (&gSiPreMemPolicyPpiGuid, 0, NULL, (VOID **) &SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  UpdatePmConfig (SiPolicy, &PchSetup);
  UpdateEspiConfig (SiPolicy, &PchSetup);
  UpdateSerialIoConfig (SiPolicy, &PchSetup);
  UpdateSataConfig (SiPolicy, &PchSetup);
  UpdateHdAudioConfig (SiPolicy, SiPreMemPolicy, &PchSetup);
  UpdateGbeConfig (SiPolicy, &PchSetup);
  UpdateDmiConfig (SiPolicy, &PchSetup);
  UpdatePsfConfig (SiPolicy, &PchSetup);
  UpdatePchGeneralConfig (SiPolicy, &PchSetup);
  UpdateLockDownConfig (SiPolicy, &PchSetup, BiosGuardEnable);
  UpdateRtcConfig (SiPolicy, &PchSetup);
  UpdateFlashProtectionConfig (SiPolicy, &PchSetup, BiosGuardEnable);
  UpdateIehConfig (SiPolicy, &PchSetup);
  UpdateIoApicConfig (SiPolicy, &PchSetup);
  UpdateInterruptConfig (SiPolicy, &PchSetup);
  UpdateThermalConfig (SiPolicy, &PchSetup);
  UpdateUsbConfig (SiPolicy, &PchSetup);
  UpdatePchUsb2PhyConfig (SiPolicy, &PchSetup);
  UpdateRstConfig(SiPolicy, &PchSetup);
  UpdatePcieRpConfig(SiPolicy, &PchSetup);
  UpdateAdrConfig(SiPolicy, &PchSetup);
  DEBUG ((DEBUG_INFO, "Update PeiPchPolicyUpdate Post-Mem end\n"));

  return Status;
}
