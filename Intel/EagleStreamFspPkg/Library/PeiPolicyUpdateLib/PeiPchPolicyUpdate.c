/** @file
  This file is SampleCode of the library for Intel PCH PEI Policy initialization.

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2020 Intel Corporation.

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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/FspCommonLib.h>
#include <FspsUpd.h>
#include <Library/PchInfoLib.h>
#include <Library/PchSerialIoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/SataSocLib.h>
#include <SerialIoDevices.h>

/**
  Update UFS policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateUfsPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  SCS_UFS_CONFIG  *UfsConfig;
  UINT8           UfsIndex;
  EFI_STATUS      Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gUfsConfigGuid, (VOID *) &UfsConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // FSPS UPD has hard allocated configuration space for only 2 UFS controllers. Be sure to change this loop termination
  // condition after adding more space in FSPS UPD.
  //
  for (UfsIndex = 0; (UfsIndex < PchGetMaxUfsNum ()) && (UfsIndex < 2); UfsIndex++) {
    UfsConfig->UfsControllerConfig[UfsIndex].Enable = FspsUpd->FspsConfig.UfsEnable[UfsIndex];
  }
}

/**
  Update SD policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateSdCardPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  SCS_SDCARD_CONFIG  *SdCardConfig;
  EFI_STATUS         Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gSdCardConfigGuid, (VOID *) &SdCardConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  SdCardConfig->Enable = FspsUpd->FspsConfig.ScsSdCardEnabled;
  SdCardConfig->PowerEnableActiveHigh = FspsUpd->FspsConfig.SdCardPowerEnableActiveHigh;
  SdCardConfig->UseCustomDlls = FspsUpd->FspsConfig.SdCardUseCustomDlls;
  SdCardConfig->CustomDllValues.TxCmdDelayControl = FspsUpd->FspsConfig.SdCardTxCmdDelayRegValue;
  SdCardConfig->CustomDllValues.TxDataDelayControl1 = FspsUpd->FspsConfig.SdCardTxDataDelay1RegValue;
  SdCardConfig->CustomDllValues.TxDataDelayControl2 = FspsUpd->FspsConfig.SdCardTxDataDelay2RegValue;
  SdCardConfig->CustomDllValues.RxCmdDataDelayControl1 = FspsUpd->FspsConfig.SdCardRxCmdDataDelay1RegValue;
  SdCardConfig->CustomDllValues.RxCmdDataDelayControl2 = FspsUpd->FspsConfig.SdCardRxCmdDataDelay2RegValue;
}

/**
  Update eMMC policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateEmmcPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  SCS_EMMC_CONFIG  *EmmcConfig;
  EFI_STATUS       Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gEmmcConfigGuid, (VOID *) &EmmcConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  EmmcConfig->Enable = FspsUpd->FspsConfig.ScsEmmcEnabled;
  EmmcConfig->Hs400Supported = FspsUpd->FspsConfig.ScsEmmcHs400Enabled;
  EmmcConfig->UseCustomDlls = FspsUpd->FspsConfig.EmmcUseCustomDlls;
  EmmcConfig->CustomDllValues.TxCmdDelayControl = FspsUpd->FspsConfig.EmmcTxCmdDelayRegValue;
  EmmcConfig->CustomDllValues.TxDataDelayControl1 = FspsUpd->FspsConfig.EmmcTxDataDelay1RegValue;
  EmmcConfig->CustomDllValues.TxDataDelayControl2 = FspsUpd->FspsConfig.EmmcTxDataDelay2RegValue;
  EmmcConfig->CustomDllValues.RxCmdDataDelayControl1 = FspsUpd->FspsConfig.EmmcRxCmdDataDelay1RegValue;
  EmmcConfig->CustomDllValues.RxCmdDataDelayControl2 = FspsUpd->FspsConfig.EmmcRxCmdDataDelay2RegValue;
  EmmcConfig->CustomDllValues.RxStrobeDelayControl = FspsUpd->FspsConfig.EmmcRxStrobeDelayRegValue;
}

/**
  Update HD Audio policies.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] FspsUpd   Pointer to FSPS_UPD
**/
STATIC
VOID
FspUpdateHdAudioPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN FSPS_UPD       *FspsUpd
  )
{
  PCH_HDAUDIO_CONFIG  *HdAudioConfig;
  EFI_STATUS          Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gHdAudioConfigGuid, (VOID *) &HdAudioConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  HdAudioConfig->DspEnable             = FspsUpd->FspsConfig.PchHdaDspEnable;
  HdAudioConfig->Pme                   = FspsUpd->FspsConfig.PchHdaPme;
  HdAudioConfig->VcType                = FspsUpd->FspsConfig.PchHdaVcType;
  HdAudioConfig->HdAudioLinkFrequency  = FspsUpd->FspsConfig.PchHdaLinkFrequency;
  HdAudioConfig->IDispLinkFrequency    = FspsUpd->FspsConfig.PchHdaIDispLinkFrequency;
  HdAudioConfig->IDispLinkTmode        = FspsUpd->FspsConfig.PchHdaIDispLinkTmode;
  HdAudioConfig->DspUaaCompliance      = FspsUpd->FspsConfig.PchHdaDspUaaCompliance;
  HdAudioConfig->IDispCodecDisconnect  = FspsUpd->FspsConfig.PchHdaIDispCodecDisconnect;
  HdAudioConfig->CodecSxWakeCapability = FspsUpd->FspsConfig.PchHdaCodecSxWakeCapability;
  HdAudioConfig->ResetWaitTimer        = FspsUpd->FspsConfig.PchHdaResetWaitTimer;
  HdAudioConfig->VerbTableEntryNum     = FspsUpd->FspsConfig.PchHdaVerbTableEntryNum;
  HdAudioConfig->VerbTablePtr          = FspsUpd->FspsConfig.PchHdaVerbTablePtr;
  HdAudioConfig->AudioLinkHda          = FspsUpd->FspsConfig.PchHdaAudioLinkHda;
  HdAudioConfig->AudioLinkDmic0        = FspsUpd->FspsConfig.PchHdaAudioLinkDmic0;
  HdAudioConfig->AudioLinkDmic1        = FspsUpd->FspsConfig.PchHdaAudioLinkDmic1;
  HdAudioConfig->AudioLinkSsp0         = FspsUpd->FspsConfig.PchHdaAudioLinkSsp0;
  HdAudioConfig->AudioLinkSsp1         = FspsUpd->FspsConfig.PchHdaAudioLinkSsp1;
  HdAudioConfig->AudioLinkSsp2         = FspsUpd->FspsConfig.PchHdaAudioLinkSsp2;
  HdAudioConfig->AudioLinkSsp3            = FspsUpd->FspsConfig.PchHdaAudioLinkSsp3;
  HdAudioConfig->AudioLinkSsp4            = FspsUpd->FspsConfig.PchHdaAudioLinkSsp4;
  HdAudioConfig->AudioLinkSsp5            = FspsUpd->FspsConfig.PchHdaAudioLinkSsp5;
  HdAudioConfig->AudioLinkSndw1           = FspsUpd->FspsConfig.PchHdaAudioLinkSndw1;
  HdAudioConfig->AudioLinkSndw2           = FspsUpd->FspsConfig.PchHdaAudioLinkSndw2;
  HdAudioConfig->AudioLinkSndw3           = FspsUpd->FspsConfig.PchHdaAudioLinkSndw3;
  HdAudioConfig->AudioLinkSndw4           = FspsUpd->FspsConfig.PchHdaAudioLinkSndw4;
}

/**
  This function performs PCH PEI Policy update.

  @param[in, out] SiPolicy       The SI Policy PPI instance
  @param[in]      FspsUpd        The pointer of FspsUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiPchPolicy (
  IN OUT SI_POLICY_PPI     *SiPolicy,
  IN     FSPS_UPD          *FspsUpd
  )
{
  UINTN                        Index;
  UINT8                        NumOfDevIntConfig;
  PCH_DEVICE_INTERRUPT_CONFIG  *DevIntConfg;
  EFI_STATUS                   Status;
  UINTN                        MaxPcieRootPorts;
  PCH_CNVI_CONFIG              *CnviConfig;
  PCH_HSIO_CONFIG              *HsioConfig;
  PCH_ESPI_CONFIG              *EspiConfig;
  PCH_LAN_CONFIG               *LanConfig;
  PCH_ISH_CONFIG               *IshConfig;
  PCH_SATA_CONFIG              *SataConfig;
  USB_CONFIG                   *UsbConfig;
  PCH_SERIAL_IO_CONFIG         *SerialIoConfig;
  PCH_INTERRUPT_CONFIG         *InterruptConfig;
  PCH_LOCK_DOWN_CONFIG         *LockDownConfig;
  PCH_PCIE_CONFIG              *PcieRpConfig;
  PCH_DMI_CONFIG               *DmiConfig;
  PCH_FLASH_PROTECTION_CONFIG  *FlashProtectionConfig;
  PCH_IOAPIC_CONFIG            *IoApicConfig;
  PCH_P2SB_CONFIG              *P2sbConfig;
  PCH_GENERAL_CONFIG           *PchGeneralConfig;
  PCH_PM_CONFIG                *PmConfig;
  PCH_THERMAL_CONFIG           *PchThermalConfig;
  PCH_FIVR_CONFIG              *FivrConfig;
  USB2_PHY_CONFIG              *Usb2PhyConfig;
  USB3_HSIO_CONFIG             *Usb3HsioConfig;

  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FSP UpdatePeiPchPolicy\n"));

  Status = GetConfigBlock ((VOID *) SiPolicy, &gLanConfigGuid, (VOID *) &LanConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gIshConfigGuid, (VOID *) &IshConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gSataConfigGuid, (VOID *) &SataConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gUsbConfigGuid, (VOID *) &UsbConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gSerialIoConfigGuid, (VOID *) &SerialIoConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gInterruptConfigGuid, (VOID *) &InterruptConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gLockDownConfigGuid, (VOID *) &LockDownConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gCnviConfigGuid, (VOID *) &CnviConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gHsioConfigGuid, (VOID *) &HsioConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gEspiConfigGuid, (VOID *) &EspiConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gPcieRpConfigGuid, (VOID *) &PcieRpConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gDmiConfigGuid, (VOID *) &DmiConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gFlashProtectionConfigGuid, (VOID *) &FlashProtectionConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gIoApicConfigGuid, (VOID *) &IoApicConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gP2sbConfigGuid, (VOID *) &P2sbConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gPchGeneralConfigGuid, (VOID *) &PchGeneralConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gPmConfigGuid, (VOID *) &PmConfig);
  ASSERT_EFI_ERROR (Status);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gThermalConfigGuid, (VOID *) &PchThermalConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gFivrConfigGuid, (VOID *) &FivrConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gUsb2PhyConfigGuid, (VOID *) &Usb2PhyConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gUsb3HsioConfigGuid, (VOID *) &Usb3HsioConfig);
  ASSERT_EFI_ERROR (Status);

  //
  // Update LAN policies
  //
  LanConfig->Enable          = FspsUpd->FspsConfig.PchLanEnable;
  LanConfig->LtrEnable       = FspsUpd->FspsConfig.PchLanLtrEnable;

  //
  // Update HDA policies
  //
  FspUpdateHdAudioPolicy (SiPolicy, FspsUpd);

  //
  // Update CNVi policies
  //
  CnviConfig->Mode           = FspsUpd->FspsConfig.CnviMode;
  CnviConfig->BtCore         = FspsUpd->FspsConfig.CnviBtCore;
  CnviConfig->BtAudioOffload = FspsUpd->FspsConfig.CnviBtAudioOffload;
  CnviConfig->PinMux.RfReset = FspsUpd->FspsConfig.CnviRfResetPinMux;
  CnviConfig->PinMux.Clkreq  = FspsUpd->FspsConfig.CnviClkreqPinMux;

  //
  // Update Pch HSIO policies
  //
  HsioConfig->ChipsetInitBinPtr = FspsUpd->FspsConfig.ChipsetInitBinPtr;
  HsioConfig->ChipsetInitBinLen = FspsUpd->FspsConfig.ChipsetInitBinLen;

  //
  // Update Pch Espi policies
  //
  EspiConfig->LgmrEnable = FspsUpd->FspsConfig.PchEspiLgmrEnable;
  EspiConfig->BmeMasterSlaveEnabled = FspsUpd->FspsConfig.PchEspiBmeMasterSlaveEnabled;
  EspiConfig->HostC10ReportEnable = FspsUpd->FspsConfig.PchEspiHostC10ReportEnable;

  //
  // Update SCS policies
  //
  FspUpdateSdCardPolicy (SiPolicy, FspsUpd);
  FspUpdateEmmcPolicy (SiPolicy, FspsUpd);
  FspUpdateUfsPolicy (SiPolicy, FspsUpd);

  //
  // Update ISH policies
  //
  IshConfig->SpiGpioAssign   = FspsUpd->FspsConfig.PchIshSpiGpioAssign;
  IshConfig->Uart0GpioAssign = FspsUpd->FspsConfig.PchIshUart0GpioAssign;
  IshConfig->Uart1GpioAssign = FspsUpd->FspsConfig.PchIshUart1GpioAssign;
  IshConfig->I2c0GpioAssign  = FspsUpd->FspsConfig.PchIshI2c0GpioAssign;
  IshConfig->I2c1GpioAssign  = FspsUpd->FspsConfig.PchIshI2c1GpioAssign;
  IshConfig->I2c2GpioAssign  = FspsUpd->FspsConfig.PchIshI2c2GpioAssign;
  IshConfig->Gp0GpioAssign   = FspsUpd->FspsConfig.PchIshGp0GpioAssign;
  IshConfig->Gp1GpioAssign   = FspsUpd->FspsConfig.PchIshGp1GpioAssign;
  IshConfig->Gp2GpioAssign   = FspsUpd->FspsConfig.PchIshGp2GpioAssign;
  IshConfig->Gp3GpioAssign   = FspsUpd->FspsConfig.PchIshGp3GpioAssign;
  IshConfig->Gp4GpioAssign   = FspsUpd->FspsConfig.PchIshGp4GpioAssign;
  IshConfig->Gp5GpioAssign   = FspsUpd->FspsConfig.PchIshGp5GpioAssign;
  IshConfig->Gp6GpioAssign   = FspsUpd->FspsConfig.PchIshGp6GpioAssign;
  IshConfig->Gp7GpioAssign   = FspsUpd->FspsConfig.PchIshGp7GpioAssign;
  IshConfig->PdtUnlock       = FspsUpd->FspsConfig.PchIshPdtUnlock;

  //
  // Update Sata policies
  //
  SataConfig->Enable          = FspsUpd->FspsConfig.SataEnable;
  SataConfig->TestMode        = FspsUpd->FspsConfig.SataTestMode;
  SataConfig->SalpSupport     = FspsUpd->FspsConfig.SataSalpSupport;
  SataConfig->PwrOptEnable    = FspsUpd->FspsConfig.SataPwrOptEnable;
  SataConfig->EsataSpeedLimit = FspsUpd->FspsConfig.EsataSpeedLimit;
  SataConfig->LedEnable       = FspsUpd->FspsConfig.SataLedEnable;
  SataConfig->SataMode        = FspsUpd->FspsConfig.SataMode;
  SataConfig->SpeedLimit      = FspsUpd->FspsConfig.SataSpeedLimit;

  for (Index = 0; Index < GetPchMaxSataPortNum (0); Index++) {
    SataConfig->PortSettings[Index].Enable           = FspsUpd->FspsConfig.SataPortsEnable[Index];
    SataConfig->PortSettings[Index].HotPlug          = FspsUpd->FspsConfig.SataPortsHotPlug[Index];
    SataConfig->PortSettings[Index].InterlockSw      = FspsUpd->FspsConfig.SataPortsInterlockSw[Index];
    SataConfig->PortSettings[Index].External         = FspsUpd->FspsConfig.SataPortsExternal[Index];
    SataConfig->PortSettings[Index].SpinUp           = FspsUpd->FspsConfig.SataPortsSpinUp[Index];
    SataConfig->PortSettings[Index].SolidStateDrive  = FspsUpd->FspsConfig.SataPortsSolidStateDrive[Index];
    SataConfig->PortSettings[Index].DevSlp           = FspsUpd->FspsConfig.SataPortsDevSlp[Index];
    SataConfig->PortSettings[Index].EnableDitoConfig = FspsUpd->FspsConfig.SataPortsEnableDitoConfig[Index];
    SataConfig->PortSettings[Index].DmVal            = FspsUpd->FspsConfig.SataPortsDmVal[Index];
    SataConfig->PortSettings[Index].DitoVal          = FspsUpd->FspsConfig.SataPortsDitoVal[Index];
    SataConfig->PortSettings[Index].ZpOdd            = FspsUpd->FspsConfig.SataPortsZpOdd[Index];
  }

  SataConfig->Rst.RaidDeviceId       = FspsUpd->FspsConfig.SataRstRaidDeviceId;
  SataConfig->Rst.Raid0              = FspsUpd->FspsConfig.SataRstRaid0;
  SataConfig->Rst.Raid1              = FspsUpd->FspsConfig.SataRstRaid1;
  SataConfig->Rst.Raid10             = FspsUpd->FspsConfig.SataRstRaid10;
  SataConfig->Rst.Raid5              = FspsUpd->FspsConfig.SataRstRaid5;
  SataConfig->Rst.Irrt               = FspsUpd->FspsConfig.SataRstIrrt;
  SataConfig->Rst.OromUiBanner       = FspsUpd->FspsConfig.SataRstOromUiBanner;
  SataConfig->Rst.OromUiDelay        = FspsUpd->FspsConfig.SataRstOromUiDelay;
  SataConfig->Rst.HddUnlock          = FspsUpd->FspsConfig.SataRstHddUnlock;
  SataConfig->Rst.LedLocate          = FspsUpd->FspsConfig.SataRstLedLocate;
  SataConfig->Rst.IrrtOnly           = FspsUpd->FspsConfig.SataRstIrrtOnly;
  SataConfig->Rst.SmartStorage       = FspsUpd->FspsConfig.SataRstSmartStorage;
  SataConfig->Rst.SataRstInterrupt   = FspsUpd->FspsConfig.SataRstInterrupt;
  SataConfig->Rst.OptaneMemory       = FspsUpd->FspsConfig.SataRstOptaneMemory;
  SataConfig->Rst.LegacyOrom         = FspsUpd->FspsConfig.SataRstLegacyOrom;
  SataConfig->Rst.CpuAttachedStorage = FspsUpd->FspsConfig.SataRstCpuAttachedStorage;

  for (Index = 0; Index < PCH_MAX_RST_PCIE_STORAGE_CR; Index++) {
    SataConfig->RstPcieStorageRemap[Index].Enable             = FspsUpd->FspsConfig.SataRstPcieEnable[Index];
    SataConfig->RstPcieStorageRemap[Index].RstPcieStoragePort = FspsUpd->FspsConfig.SataRstPcieStoragePort[Index];
    SataConfig->RstPcieStorageRemap[Index].DeviceResetDelay   = FspsUpd->FspsConfig.SataRstPcieDeviceResetDelay[Index];
  }

  SataConfig->ThermalThrottling.P0T1M            = FspsUpd->FspsConfig.SataP0T1M;
  SataConfig->ThermalThrottling.P0T2M            = FspsUpd->FspsConfig.SataP0T2M;
  SataConfig->ThermalThrottling.P0T3M            = FspsUpd->FspsConfig.SataP0T3M;
  SataConfig->ThermalThrottling.P0TDisp          = FspsUpd->FspsConfig.SataP0TDisp;
  SataConfig->ThermalThrottling.P1T1M            = FspsUpd->FspsConfig.SataP1T1M;
  SataConfig->ThermalThrottling.P1T2M            = FspsUpd->FspsConfig.SataP1T2M;
  SataConfig->ThermalThrottling.P1T3M            = FspsUpd->FspsConfig.SataP1T3M;
  SataConfig->ThermalThrottling.P1TDisp          = FspsUpd->FspsConfig.SataP1TDisp;
  SataConfig->ThermalThrottling.P0Tinact         = FspsUpd->FspsConfig.SataP0Tinact;
  SataConfig->ThermalThrottling.P0TDispFinit     = FspsUpd->FspsConfig.SataP0TDispFinit;
  SataConfig->ThermalThrottling.P1Tinact         = FspsUpd->FspsConfig.SataP1Tinact;
  SataConfig->ThermalThrottling.P1TDispFinit     = FspsUpd->FspsConfig.SataP1TDispFinit;
  SataConfig->ThermalThrottling.SuggestedSetting = FspsUpd->FspsConfig.SataThermalSuggestedSetting;

  //
  // Update USB policies
  //
  UsbConfig->EnableComplianceMode   = FspsUpd->FspsConfig.PchEnableComplianceMode;
  UsbConfig->PdoProgramming         = FspsUpd->FspsConfig.UsbPdoProgramming;
  UsbConfig->OverCurrentEnable      = FspsUpd->FspsConfig.PchUsbOverCurrentEnable;
  if (FspsUpd->FspsConfig.PchEnableDbcObs) {
    UsbConfig->OverCurrentEnable    = FALSE;
  }
  UsbConfig->XhciOcLock             = FspsUpd->FspsConfig.PchXhciOcLock;
  for (Index = 0; Index < GetPchUsb2MaxPhysicalPortNum (); Index++) {
    UsbConfig->PortUsb20[Index].Enable         = FspsUpd->FspsConfig.PortUsb20Enable[Index];
    UsbConfig->PortUsb20[Index].OverCurrentPin = FspsUpd->FspsConfig.Usb2OverCurrentPin[Index];
  }
  for (Index = 0; Index < GetPchXhciMaxUsb3PortNum (); Index++) {
    UsbConfig->PortUsb30[Index].Enable                    = FspsUpd->FspsConfig.PortUsb30Enable[Index];
    UsbConfig->PortUsb30[Index].OverCurrentPin            = FspsUpd->FspsConfig.Usb3OverCurrentPin[Index];
  }
//@todo BP1330 sync
//  UsbConfig->SsicConfig.SsicPort[0].Enable = FspsUpd->FspsConfig.SsicPortEnable;
  UsbConfig->XdciConfig.Enable  = FspsUpd->FspsConfig.XdciEnable;

  //
  // Update USB2 PHY policies
  //
  for (Index = 0; Index < GetPchUsb2MaxPhysicalPortNum (); Index++) {
    Usb2PhyConfig->Port[Index].Petxiset   = FspsUpd->FspsConfig.Usb2PhyPetxiset[Index];
    Usb2PhyConfig->Port[Index].Txiset     = FspsUpd->FspsConfig.Usb2PhyTxiset[Index];
    Usb2PhyConfig->Port[Index].Predeemp   = FspsUpd->FspsConfig.Usb2PhyPredeemp[Index];
    Usb2PhyConfig->Port[Index].Pehalfbit  = FspsUpd->FspsConfig.Usb2PhyPehalfbit[Index];
  }

  //
  // Update USB3 HSIO policies
  //
  for (Index = 0; Index < GetPchXhciMaxUsb3PortNum (); Index++) {
    Usb3HsioConfig->Port[Index].HsioTxDeEmphEnable             = FspsUpd->FspsConfig.Usb3HsioTxDeEmphEnable[Index];
    Usb3HsioConfig->Port[Index].HsioTxDeEmph                   = FspsUpd->FspsConfig.Usb3HsioTxDeEmph[Index];

    Usb3HsioConfig->Port[Index].HsioTxDownscaleAmpEnable       = FspsUpd->FspsConfig.Usb3HsioTxDownscaleAmpEnable[Index];
    Usb3HsioConfig->Port[Index].HsioTxDownscaleAmp             = FspsUpd->FspsConfig.Usb3HsioTxDownscaleAmp[Index];

    Usb3HsioConfig->Port[Index].HsioCtrlAdaptOffsetCfgEnable   = FspsUpd->FspsConfig.PchUsb3HsioCtrlAdaptOffsetCfgEnable[Index];
    Usb3HsioConfig->Port[Index].HsioCtrlAdaptOffsetCfg         = FspsUpd->FspsConfig.PchUsb3HsioCtrlAdaptOffsetCfg[Index];

    Usb3HsioConfig->Port[Index].HsioFilterSelPEnable           = FspsUpd->FspsConfig.PchUsb3HsioFilterSelPEnable[Index];
    Usb3HsioConfig->Port[Index].HsioFilterSelP                 = FspsUpd->FspsConfig.PchUsb3HsioFilterSelP[Index];

    Usb3HsioConfig->Port[Index].HsioFilterSelNEnable           = FspsUpd->FspsConfig.PchUsb3HsioFilterSelNEnable[Index];
    Usb3HsioConfig->Port[Index].HsioFilterSelN                 = FspsUpd->FspsConfig.PchUsb3HsioFilterSelN[Index];

    Usb3HsioConfig->Port[Index].HsioOlfpsCfgPullUpDwnResEnable = FspsUpd->FspsConfig.PchUsb3HsioOlfpsCfgPullUpDwnResEnable[Index];
    Usb3HsioConfig->Port[Index].HsioOlfpsCfgPullUpDwnRes       = FspsUpd->FspsConfig.PchUsb3HsioOlfpsCfgPullUpDwnRes[Index];
  }

  //
  // Update PCIE RP RootPort policies
  //
  MaxPcieRootPorts = GetPchMaxPciePortNum ();
  for (Index = 0; Index < MaxPcieRootPorts; Index ++) {
    PcieRpConfig->RootPort[Index].HotPlug                  = FspsUpd->FspsConfig.PcieRpHotPlug[Index];
    PcieRpConfig->RootPort[Index].PmSci                    = FspsUpd->FspsConfig.PcieRpPmSci[Index];
    PcieRpConfig->RootPort[Index].TransmitterHalfSwing     = FspsUpd->FspsConfig.PcieRpTransmitterHalfSwing[Index];
    PcieRpConfig->RootPort[Index].ClkReqDetect             = FspsUpd->FspsConfig.PcieRpClkReqDetect[Index];
    PcieRpConfig->RootPort[Index].AdvancedErrorReporting   = FspsUpd->FspsConfig.PcieRpAdvancedErrorReporting[Index];
    PcieRpConfig->RootPort[Index].UnsupportedRequestReport = FspsUpd->FspsConfig.PcieRpUnsupportedRequestReport[Index];
    PcieRpConfig->RootPort[Index].FatalErrorReport         = FspsUpd->FspsConfig.PcieRpFatalErrorReport[Index];
    PcieRpConfig->RootPort[Index].NoFatalErrorReport       = FspsUpd->FspsConfig.PcieRpNoFatalErrorReport[Index];
    PcieRpConfig->RootPort[Index].CorrectableErrorReport   = FspsUpd->FspsConfig.PcieRpCorrectableErrorReport[Index];
    PcieRpConfig->RootPort[Index].SystemErrorOnFatalError  = FspsUpd->FspsConfig.PcieRpSystemErrorOnFatalError[Index];
    PcieRpConfig->RootPort[Index].SystemErrorOnNonFatalError    = FspsUpd->FspsConfig.PcieRpSystemErrorOnNonFatalError[Index];
    PcieRpConfig->RootPort[Index].SystemErrorOnCorrectableError = FspsUpd->FspsConfig.PcieRpSystemErrorOnCorrectableError[Index];
    PcieRpConfig->RootPort[Index].MaxPayload               = FspsUpd->FspsConfig.PcieRpMaxPayload[Index];
    PcieRpConfig->RootPort[Index].DpcEnabled               = !!(FspsUpd->FspsConfig.PcieRpDpcMask & (BIT0<<Index));
    PcieRpConfig->RootPort[Index].RpDpcExtensionsEnabled   = !!(FspsUpd->FspsConfig.PcieRpDpcExtensionsMask & (BIT0<<Index));
    PcieRpConfig->RootPort[Index].PtmEnabled               = !!(FspsUpd->FspsConfig.PcieRpPtmMask & (BIT0<<Index));
    PcieRpConfig->RootPort[Index].PcieSpeed                = FspsUpd->FspsConfig.PcieRpPcieSpeed[Index];
    PcieRpConfig->RootPort[Index].Gen3EqPh3Method          = FspsUpd->FspsConfig.PcieRpGen3EqPh3Method[Index];
    PcieRpConfig->RootPort[Index].PhysicalSlotNumber       = FspsUpd->FspsConfig.PcieRpPhysicalSlotNumber[Index];
    PcieRpConfig->RootPort[Index].SlotImplemented          = FspsUpd->FspsConfig.PcieRpSlotImplemented[Index];
    PcieRpConfig->RootPort[Index].CompletionTimeout        = FspsUpd->FspsConfig.PcieRpCompletionTimeout[Index];
    PcieRpConfig->RootPort[Index].Aspm                     = FspsUpd->FspsConfig.PcieRpAspm[Index];
    PcieRpConfig->RootPort[Index].L1Substates              = FspsUpd->FspsConfig.PcieRpL1Substates[Index];
    PcieRpConfig->RootPort[Index].LtrEnable                = FspsUpd->FspsConfig.PcieRpLtrEnable[Index];
    PcieRpConfig->RootPort[Index].LtrConfigLock            = FspsUpd->FspsConfig.PcieRpLtrConfigLock[Index];
    PcieRpConfig->RootPort[Index].AcsEnabled               = FspsUpd->FspsConfig.PcieRpAcsEnabled[Index];
    PcieRpConfig->RootPort[Index].EnableCpm                = FspsUpd->FspsConfig.PcieRpEnableCpm[Index];
    PcieRpConfig->RootPort[Index].DetectTimeoutMs          = FspsUpd->FspsConfig.PcieRpDetectTimeoutMs[Index];

    PcieRpConfig->RootPort[Index].LtrMaxSnoopLatency       = FspsUpd->FspsConfig.PcieRpLtrMaxSnoopLatency[Index];
    PcieRpConfig->RootPort[Index].LtrMaxNoSnoopLatency     = FspsUpd->FspsConfig.PcieRpLtrMaxNoSnoopLatency[Index];

    PcieRpConfig->RootPort[Index].SnoopLatencyOverrideMode       = FspsUpd->FspsConfig.PcieRpSnoopLatencyOverrideMode[Index];
    PcieRpConfig->RootPort[Index].SnoopLatencyOverrideMultiplier = FspsUpd->FspsConfig.PcieRpSnoopLatencyOverrideMultiplier[Index];
    PcieRpConfig->RootPort[Index].SnoopLatencyOverrideValue      = FspsUpd->FspsConfig.PcieRpSnoopLatencyOverrideValue[Index];

    PcieRpConfig->RootPort[Index].NonSnoopLatencyOverrideMode       = FspsUpd->FspsConfig.PcieRpNonSnoopLatencyOverrideMode[Index];
    PcieRpConfig->RootPort[Index].NonSnoopLatencyOverrideMultiplier = FspsUpd->FspsConfig.PcieRpNonSnoopLatencyOverrideMultiplier[Index];
    PcieRpConfig->RootPort[Index].NonSnoopLatencyOverrideValue      = FspsUpd->FspsConfig.PcieRpNonSnoopLatencyOverrideValue[Index];

    PcieRpConfig->RootPort[Index].SlotPowerLimitScale      = FspsUpd->FspsConfig.PcieRpSlotPowerLimitScale[Index];
    PcieRpConfig->RootPort[Index].SlotPowerLimitValue      = FspsUpd->FspsConfig.PcieRpSlotPowerLimitValue[Index];
    PcieRpConfig->RootPort[Index].Uptp                     = FspsUpd->FspsConfig.PcieRpUptp[Index];
    PcieRpConfig->RootPort[Index].Dptp                     = FspsUpd->FspsConfig.PcieRpDptp[Index];
  }

  for (Index = 0; Index < GetPchMaxPcieClockNum (); Index ++) {
    PcieRpConfig->PcieClock[Index].Usage    = FspsUpd->FspsConfig.PcieClkSrcUsage[Index];
    PcieRpConfig->PcieClock[Index].ClkReq   = FspsUpd->FspsConfig.PcieClkSrcClkReq[Index];
  }
  //
  // Update PCIE RP EqPh3LaneParam policies
  //
  for (Index = 0; Index < MaxPcieRootPorts; Index ++) {
    PcieRpConfig->EqPh3LaneParam[Index].Cm = FspsUpd->FspsConfig.PcieEqPh3LaneParamCm[Index];
    PcieRpConfig->EqPh3LaneParam[Index].Cp = FspsUpd->FspsConfig.PcieEqPh3LaneParamCp[Index];
  }

  //
  // Update PCIE RP SwEqCoeffList policies
  //
  for (Index = 0; Index < PCH_PCIE_SWEQ_COEFFS_MAX; Index++) {
    PcieRpConfig->SwEqCoeffList[Index].Cm = FspsUpd->FspsConfig.PcieSwEqCoeffListCm[Index];
    PcieRpConfig->SwEqCoeffList[Index].Cp = FspsUpd->FspsConfig.PcieSwEqCoeffListCp[Index];
  }

  //
  // Update PCIE RP policies
  //
  PcieRpConfig->EnablePort8xhDecode           = FspsUpd->FspsConfig.PcieEnablePort8xhDecode;
  PcieRpConfig->PchPciePort8xhDecodePortIndex = FspsUpd->FspsConfig.PchPciePort8xhDecodePortIndex;
  PcieRpConfig->EnablePeerMemoryWrite         = FspsUpd->FspsConfig.PcieEnablePeerMemoryWrite;
  PcieRpConfig->ComplianceTestMode            = FspsUpd->FspsConfig.PcieComplianceTestMode;
  PcieRpConfig->RpFunctionSwap                = FspsUpd->FspsConfig.PcieRpFunctionSwap;

  //
  // Update SerialIo policies
  //
  for (Index = 0; Index < GetPchMaxSerialIoSpiControllersNum (); Index++) {
    SerialIoConfig->SpiDeviceConfig[Index].Mode            = FspsUpd->FspsConfig.SerialIoSpiMode[Index];
    SerialIoConfig->SpiDeviceConfig[Index].DefaultCsOutput = FspsUpd->FspsConfig.SerialIoSpiDefaultCsOutput[Index];
    SerialIoConfig->SpiDeviceConfig[Index].CsMode          = FspsUpd->FspsConfig.SerialIoSpiCsMode[Index];
    SerialIoConfig->SpiDeviceConfig[Index].CsState         = FspsUpd->FspsConfig.SerialIoSpiCsState[Index];
  }
  SerialIoConfig->SpiDeviceConfig[0].CsPolarity[0] = FspsUpd->FspsConfig.SerialIoSpi0CsPolarity[0];
  SerialIoConfig->SpiDeviceConfig[0].CsPolarity[1] = FspsUpd->FspsConfig.SerialIoSpi0CsPolarity[1];
  SerialIoConfig->SpiDeviceConfig[1].CsPolarity[0] = FspsUpd->FspsConfig.SerialIoSpi1CsPolarity[0];
  SerialIoConfig->SpiDeviceConfig[1].CsPolarity[1] = FspsUpd->FspsConfig.SerialIoSpi1CsPolarity[1];
  SerialIoConfig->SpiDeviceConfig[2].CsPolarity[0] = FspsUpd->FspsConfig.SerialIoSpi2CsPolarity[0];
  SerialIoConfig->SpiDeviceConfig[2].CsPolarity[1] = FspsUpd->FspsConfig.SerialIoSpi2CsPolarity[1];
  SerialIoConfig->SpiDeviceConfig[0].CsEnable[0]   = FspsUpd->FspsConfig.SerialIoSpi0CsEnable[0];
  SerialIoConfig->SpiDeviceConfig[0].CsEnable[1]   = FspsUpd->FspsConfig.SerialIoSpi0CsEnable[1];
  SerialIoConfig->SpiDeviceConfig[1].CsEnable[0]   = FspsUpd->FspsConfig.SerialIoSpi1CsEnable[0];
  SerialIoConfig->SpiDeviceConfig[1].CsEnable[1]   = FspsUpd->FspsConfig.SerialIoSpi1CsEnable[1];
  SerialIoConfig->SpiDeviceConfig[2].CsEnable[0]   = FspsUpd->FspsConfig.SerialIoSpi2CsEnable[0];
  SerialIoConfig->SpiDeviceConfig[2].CsEnable[1]   = FspsUpd->FspsConfig.SerialIoSpi2CsEnable[1];

  for (Index = 0; Index < GetPchMaxSerialIoUartControllersNum (); Index++) {
    SerialIoConfig->UartDeviceConfig[Index].Mode                = FspsUpd->FspsConfig.SerialIoUartMode[Index];
    SerialIoConfig->UartDeviceConfig[Index].Attributes.BaudRate = FspsUpd->FspsConfig.SerialIoUartBaudRate[Index];
    SerialIoConfig->UartDeviceConfig[Index].Attributes.Parity   = FspsUpd->FspsConfig.SerialIoUartParity[Index];
    SerialIoConfig->UartDeviceConfig[Index].Attributes.DataBits = FspsUpd->FspsConfig.SerialIoUartDataBits[Index];
    SerialIoConfig->UartDeviceConfig[Index].Attributes.StopBits = FspsUpd->FspsConfig.SerialIoUartStopBits[Index];
    SerialIoConfig->UartDeviceConfig[Index].Attributes.AutoFlow = FspsUpd->FspsConfig.SerialIoUartAutoFlow[Index];
    SerialIoConfig->UartDeviceConfig[Index].PinMux.Rx           = FspsUpd->FspsConfig.SerialIoUartRxPinMux[Index];
    SerialIoConfig->UartDeviceConfig[Index].PinMux.Tx           = FspsUpd->FspsConfig.SerialIoUartTxPinMux[Index];
    SerialIoConfig->UartDeviceConfig[Index].PinMux.Rts          = FspsUpd->FspsConfig.SerialIoUartRtsPinMux[Index];
    SerialIoConfig->UartDeviceConfig[Index].PinMux.Cts          = FspsUpd->FspsConfig.SerialIoUartCtsPinMux[Index];
    SerialIoConfig->UartDeviceConfig[Index].PowerGating         = FspsUpd->FspsConfig.SerialIoUartPowerGating[Index];
    SerialIoConfig->UartDeviceConfig[Index].DmaEnable           = FspsUpd->FspsConfig.SerialIoUartDmaEnable[Index];
    SerialIoConfig->UartDeviceConfig[Index].DBG2                = FspsUpd->FspsConfig.SerialIoUartDbg2[Index];
  }

  for (Index = 0; Index < GetPchMaxSerialIoI2cControllersNum (); Index++) {
    SerialIoConfig->I2cDeviceConfig[Index].Mode           = FspsUpd->FspsConfig.SerialIoI2cMode[Index];
    SerialIoConfig->I2cDeviceConfig[Index].PadTermination = FspsUpd->FspsConfig.PchSerialIoI2cPadsTermination[Index];
  }

  //
  // Update Interrupt policies
  //
  if (FspsUpd->FspsConfig.NumOfDevIntConfig != 0) {
    DevIntConfg = (PCH_DEVICE_INTERRUPT_CONFIG *)FspsUpd->FspsConfig.DevIntConfigPtr;
    NumOfDevIntConfig = FspsUpd->FspsConfig.NumOfDevIntConfig;
    ASSERT (NumOfDevIntConfig <= PCH_MAX_DEVICE_INTERRUPT_CONFIG);

    InterruptConfig->NumOfDevIntConfig = NumOfDevIntConfig;
    ZeroMem (
      InterruptConfig->DevIntConfig,
      PCH_MAX_DEVICE_INTERRUPT_CONFIG * sizeof (PCH_DEVICE_INTERRUPT_CONFIG)
      );
    CopyMem (
      InterruptConfig->DevIntConfig,
      DevIntConfg,
      NumOfDevIntConfig * sizeof (PCH_DEVICE_INTERRUPT_CONFIG)
      );
  }
  for (Index = 0; Index < PCH_MAX_PXRC_CONFIG; Index ++) {
    if (FspsUpd->FspsConfig.PxRcConfig[Index] != 0) {
      InterruptConfig->PxRcConfig[Index] = FspsUpd->FspsConfig.PxRcConfig[Index];
    }
  }
  InterruptConfig->GpioIrqRoute = FspsUpd->FspsConfig.GpioIrqRoute;
  InterruptConfig->SciIrqSelect = FspsUpd->FspsConfig.SciIrqSelect;
  InterruptConfig->TcoIrqSelect = FspsUpd->FspsConfig.TcoIrqSelect;
  InterruptConfig->TcoIrqEnable = FspsUpd->FspsConfig.TcoIrqEnable;
  for (Index = 0; Index < PCH_MAX_ITSS_IPC_REGS; Index ++) {
    InterruptConfig->IrqPolarity[Index] = FspsUpd->FspsConfig.ItssIrqPolarity[Index];
  }

  //
  // Update LockDown policies
  //
//@todo BP1330, un-comment after policy sync
//  LockDownConfig->RtcLock       = FspsUpd->FspsConfig.PchLockDownRtcLock;
  LockDownConfig->GlobalSmi       = FspsUpd->FspsConfig.PchLockDownGlobalSmi;
  LockDownConfig->BiosInterface   = FspsUpd->FspsConfig.PchLockDownBiosInterface;
  LockDownConfig->BiosLock        = FspsUpd->FspsConfig.PchLockDownBiosLock;
//@todo BP1330, should be removed?
  LockDownConfig->RtcMemoryLock   = FspsUpd->FspsConfig.PchLockDownRtcMemoryLock;
  LockDownConfig->UnlockGpioPads  = FspsUpd->FspsConfig.PchUnlockGpioPads;

  //
  // Update Dmi policies
  //
  DmiConfig->PwrOptEnable = FspsUpd->FspsConfig.PchPwrOptEnable;
  DmiConfig->DmiAspmCtrl  = FspsUpd->FspsConfig.PchDmiAspmCtrl;
  DmiConfig->CwbEnable    = FspsUpd->FspsConfig.PchDmiCwbEnable;

  //
  // Update Flash Protection policies
  //
  for (Index = 0; Index < PCH_FLASH_PROTECTED_RANGES; Index ++) {
    FlashProtectionConfig->ProtectRange[Index].WriteProtectionEnable = FspsUpd->FspsConfig.PchWriteProtectionEnable[Index];
    FlashProtectionConfig->ProtectRange[Index].ReadProtectionEnable  = FspsUpd->FspsConfig.PchReadProtectionEnable[Index];
    FlashProtectionConfig->ProtectRange[Index].ProtectedRangeLimit   = FspsUpd->FspsConfig.PchProtectedRangeLimit[Index];
    FlashProtectionConfig->ProtectRange[Index].ProtectedRangeBase    = FspsUpd->FspsConfig.PchProtectedRangeBase[Index];
  }

  //
  // Update IO Apic policies
  //
  IoApicConfig->IoApicEntry24_119          = FspsUpd->FspsConfig.PchIoApicEntry24_119;
  IoApicConfig->Enable8254ClockGating      = FspsUpd->FspsConfig.Enable8254ClockGating;
  IoApicConfig->Enable8254ClockGatingOnS3  = FspsUpd->FspsConfig.Enable8254ClockGatingOnS3;
  IoApicConfig->IoApicId                   = FspsUpd->FspsConfig.PchIoApicId;

  //
  // Update P2sb policies
  //
  P2sbConfig->SbAccessUnlock  = FspsUpd->FspsConfig.PchSbAccessUnlock;

  //
  // Update Pch General policies
  //
  PchGeneralConfig->Crid              = FspsUpd->FspsConfig.PchCrid;

  //
  // Update Pm policies
  //
  PmConfig->WakeConfig.PmeB0S5Dis         = FspsUpd->FspsConfig.PchPmPmeB0S5Dis;
  PmConfig->WakeConfig.WolEnableOverride  = FspsUpd->FspsConfig.PchPmWolEnableOverride;
  PmConfig->WakeConfig.PcieWakeFromDeepSx = FspsUpd->FspsConfig.PchPmPcieWakeFromDeepSx;
  PmConfig->WakeConfig.WoWlanEnable       = FspsUpd->FspsConfig.PchPmWoWlanEnable;
  PmConfig->WakeConfig.WoWlanDeepSxEnable = FspsUpd->FspsConfig.PchPmWoWlanDeepSxEnable;
  PmConfig->WakeConfig.LanWakeFromDeepSx  = FspsUpd->FspsConfig.PchPmLanWakeFromDeepSx;

  PmConfig->PchDeepSxPol       = FspsUpd->FspsConfig.PchPmDeepSxPol;
  PmConfig->PchSlpS3MinAssert  = FspsUpd->FspsConfig.PchPmSlpS3MinAssert;
  PmConfig->PchSlpS4MinAssert  = FspsUpd->FspsConfig.PchPmSlpS4MinAssert;
  PmConfig->PchSlpSusMinAssert = FspsUpd->FspsConfig.PchPmSlpSusMinAssert;
  PmConfig->PchSlpAMinAssert   = FspsUpd->FspsConfig.PchPmSlpAMinAssert;
  PmConfig->SlpStrchSusUp        = FspsUpd->FspsConfig.PchPmSlpStrchSusUp;
  PmConfig->SlpLanLowDc          = FspsUpd->FspsConfig.PchPmSlpLanLowDc;
  PmConfig->PwrBtnOverridePeriod = FspsUpd->FspsConfig.PchPmPwrBtnOverridePeriod;
  PmConfig->DisableEnergyReport  = FspsUpd->FspsConfig.PchPmDisableEnergyReport;
  PmConfig->EnergyReportDebugModeEnable = FspsUpd->FspsConfig.PchPmEnergyReportDebugMode;
  PmConfig->DisableDsxAcPresentPulldown = FspsUpd->FspsConfig.PchPmDisableDsxAcPresentPulldown;
  PmConfig->DisableNativePowerButton    = FspsUpd->FspsConfig.PchPmDisableNativePowerButton;
  PmConfig->PowerButtonDebounceTime     = FspsUpd->FspsConfig.PmcPowerButtonDebounceTime;
  PmConfig->DisablePowerButtonDebounce  = FspsUpd->FspsConfig.PmcDisablePowerButtonDebounce;
  PmConfig->SlpS0Enable          = FspsUpd->FspsConfig.PchPmSlpS0Enable;
  PmConfig->MeWakeSts            = FspsUpd->FspsConfig.PchPmMeWakeSts;
  PmConfig->WolOvrWkSts          = FspsUpd->FspsConfig.PchPmWolOvrWkSts;
  PmConfig->EnableTcoTimer       = FspsUpd->FspsConfig.EnableTcoTimer;
  PmConfig->VrAlert              = FspsUpd->FspsConfig.PchPmVrAlert;
  PmConfig->PchPwrCycDur         = FspsUpd->FspsConfig.PchPmPwrCycDur;
  PmConfig->PciePllSsc           = FspsUpd->FspsConfig.PchPmPciePllSsc;
  PmConfig->S0i3Support          = FspsUpd->FspsConfig.PchPmS0i3Support;
  PmConfig->PmcDbgMsgEn           = FspsUpd->FspsConfig.PmcDbgMsgEn;
  PmConfig->PsOnEnable            = FspsUpd->FspsConfig.PsOnEnable;
  PmConfig->OsIdleEnable          = FspsUpd->FspsConfig.PmcOsIdleEnable;
  PmConfig->CrashLogEnable        = FspsUpd->FspsConfig.PmcCrashLogEnable;
  PmConfig->ForcedAlignmentEnable = FspsUpd->FspsConfig.ForcedAlignmentEnable;

  //
  // Update Pch Thermal policies
  //
//@todo BP1330, un-comment after policy created
//  PchThermalConfig->ThermalDeviceEnable = FspsUpd->FspsConfig.PchThermalDeviceEnable;
  PchThermalConfig->PchHotEnable = FspsUpd->FspsConfig.PchHotEnable;

  PchThermalConfig->TTLevels.T0Level            = FspsUpd->FspsConfig.PchT0Level;
  PchThermalConfig->TTLevels.T1Level            = FspsUpd->FspsConfig.PchT1Level;
  PchThermalConfig->TTLevels.T2Level            = FspsUpd->FspsConfig.PchT2Level;
  PchThermalConfig->TTLevels.TTEnable           = FspsUpd->FspsConfig.PchTTEnable;
  PchThermalConfig->TTLevels.TTState13Enable    = FspsUpd->FspsConfig.PchTTState13Enable;
  PchThermalConfig->TTLevels.TTLock             = FspsUpd->FspsConfig.PchTTLock;
  PchThermalConfig->TTLevels.SuggestedSetting   = FspsUpd->FspsConfig.TTSuggestedSetting;
  PchThermalConfig->TTLevels.PchCrossThrottling = FspsUpd->FspsConfig.TTCrossThrottling;

  PchThermalConfig->DmiHaAWC.DmiTsawEn        = FspsUpd->FspsConfig.PchDmiTsawEn;
  PchThermalConfig->DmiHaAWC.SuggestedSetting = FspsUpd->FspsConfig.DmiSuggestedSetting;
  PchThermalConfig->DmiHaAWC.TS0TW            = FspsUpd->FspsConfig.DmiTS0TW;
  PchThermalConfig->DmiHaAWC.TS1TW            = FspsUpd->FspsConfig.DmiTS1TW;
  PchThermalConfig->DmiHaAWC.TS2TW            = FspsUpd->FspsConfig.DmiTS2TW;
  PchThermalConfig->DmiHaAWC.TS3TW            = FspsUpd->FspsConfig.DmiTS3TW;

  PchThermalConfig->MemoryThrottling.Enable = FspsUpd->FspsConfig.PchMemoryThrottlingEnable;
  PchThermalConfig->MemoryThrottling.TsGpioPinSetting[0].PmsyncEnable     = FspsUpd->FspsConfig.PchMemoryPmsyncEnable[0];
  PchThermalConfig->MemoryThrottling.TsGpioPinSetting[1].PmsyncEnable     = FspsUpd->FspsConfig.PchMemoryPmsyncEnable[1];
  PchThermalConfig->MemoryThrottling.TsGpioPinSetting[0].C0TransmitEnable = FspsUpd->FspsConfig.PchMemoryC0TransmitEnable[0];
  PchThermalConfig->MemoryThrottling.TsGpioPinSetting[1].C0TransmitEnable = FspsUpd->FspsConfig.PchMemoryC0TransmitEnable[1];
  PchThermalConfig->MemoryThrottling.TsGpioPinSetting[0].PinSelection     = FspsUpd->FspsConfig.PchMemoryPinSelection[0];
  PchThermalConfig->MemoryThrottling.TsGpioPinSetting[1].PinSelection     = FspsUpd->FspsConfig.PchMemoryPinSelection[1];

  PchThermalConfig->PchHotLevel = FspsUpd->FspsConfig.PchTemperatureHotLevel;

  FivrConfig->ExtV1p05Rail.EnabledStates = FspsUpd->FspsConfig.PchFivrExtV1p05RailEnabledStates;
  FivrConfig->ExtV1p05Rail.Voltage       = FspsUpd->FspsConfig.PchFivrExtV1p05RailVoltage;
  FivrConfig->ExtV1p05Rail.IccMax        = FspsUpd->FspsConfig.PchFivrExtV1p05RailIccMax;
  FivrConfig->ExtVnnRail.EnabledStates   = FspsUpd->FspsConfig.PchFivrExtVnnRailEnabledStates;
  FivrConfig->ExtVnnRail.Voltage         = FspsUpd->FspsConfig.PchFivrExtVnnRailVoltage;
  FivrConfig->ExtVnnRail.IccMax          = FspsUpd->FspsConfig.PchFivrExtVnnRailIccMax;
  FivrConfig->ExtVnnRailSx.EnabledStates = FspsUpd->FspsConfig.PchFivrExtVnnRailSxEnabledStates;
  FivrConfig->ExtVnnRailSx.Voltage       = FspsUpd->FspsConfig.PchFivrExtVnnRailSxVoltage;
  FivrConfig->ExtVnnRailSx.IccMax        = FspsUpd->FspsConfig.PchFivrExtVnnRailSxIccMax;
  FivrConfig->VccinAux.LowToHighCurModeVolTranTime = FspsUpd->FspsConfig.PchFivrVccinAuxLowToHighCurModeVolTranTime;
  FivrConfig->VccinAux.RetToHighCurModeVolTranTime = FspsUpd->FspsConfig.PchFivrVccinAuxRetToHighCurModeVolTranTime;
  FivrConfig->VccinAux.RetToLowCurModeVolTranTime  = FspsUpd->FspsConfig.PchFivrVccinAuxRetToLowCurModeVolTranTime;
  FivrConfig->VccinAux.OffToHighCurModeVolTranTime = FspsUpd->FspsConfig.PchFivrVccinAuxOffToHighCurModeVolTranTime;
  FivrConfig->FivrDynPm                            = FspsUpd->FspsConfig.PchFivrDynPm;

  return EFI_SUCCESS;
}
