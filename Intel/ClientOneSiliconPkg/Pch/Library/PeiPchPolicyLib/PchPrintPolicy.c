/** @file
  Print whole PCH_POLICY_PPI

@copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2020 Intel Corporation.

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
#include "PeiPchPolicyLibrary.h"
#include <Library/GpioPrivateLib.h>
#include <Library/ChipsetInitLib.h>

/**
  Print PCH_HSIO_CONFIG.

  @param[in] HsioConfig         Pointer to a PCH_HSIO_CONFIG that provides the eSPI setting

**/
VOID
PchPrintHsioConfig (
  IN CONST PCH_HSIO_CONFIG   *HsioConfig
  )
{
  PCH_HSIO_VER_INFO             *BiosChipsetInitVerInfoPtr;
  DEBUG ((DEBUG_INFO, "------------------ PCH HSIO Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " ChipsetInit Binary Pointer            = %x\n", HsioConfig->ChipsetInitBinPtr));
  DEBUG ((DEBUG_INFO, " ChipsetInit Binary Length             = %x\n", HsioConfig->ChipsetInitBinLen));
  BiosChipsetInitVerInfoPtr = (PCH_HSIO_VER_INFO *) HsioConfig->ChipsetInitBinPtr;
  if (HsioConfig->ChipsetInitBinPtr && HsioConfig->ChipsetInitBinLen) {
    DEBUG ((DEBUG_INFO, " ChipsetInit Binary Base CRC           = %x\n", BiosChipsetInitVerInfoPtr->BaseCrc));
    DEBUG ((DEBUG_INFO, " ChipsetInit Binary OEM CRC            = %x\n", BiosChipsetInitVerInfoPtr->OemCrc));
    DEBUG ((DEBUG_INFO, " ChipsetInit Binary SUS CRC            = %x\n", BiosChipsetInitVerInfoPtr->SusCrc));
    DEBUG ((DEBUG_INFO, " ChipsetInit Binary Version            = %x\n", BiosChipsetInitVerInfoPtr->Version));
    DEBUG ((DEBUG_INFO, " ChipsetInit Binary Product            = %x\n", BiosChipsetInitVerInfoPtr->Product));
    DEBUG ((DEBUG_INFO, " ChipsetInit Binary Metal Layer        = %x\n", BiosChipsetInitVerInfoPtr->MetalLayer));
    DEBUG ((DEBUG_INFO, " ChipsetInit Binary Base Layer         = %x\n", BiosChipsetInitVerInfoPtr->BaseLayer));
    DEBUG ((DEBUG_INFO, " ChipsetInit Binary OEM Version        = %x\n", BiosChipsetInitVerInfoPtr->OemVersion));
    DEBUG ((DEBUG_INFO, " ChipsetInit Binary Debug Mode         = %x\n", BiosChipsetInitVerInfoPtr->DebugMode));
    DEBUG ((DEBUG_INFO, " ChipsetInit Binary OEM CRC Valid      = %x\n", BiosChipsetInitVerInfoPtr->OemCrcValid));
    DEBUG ((DEBUG_INFO, " ChipsetInit Binary SUS CRC Valid      = %x\n", BiosChipsetInitVerInfoPtr->SusCrcValid));
    DEBUG ((DEBUG_INFO, " ChipsetInit Binary Base CRC Valid     = %x\n", BiosChipsetInitVerInfoPtr->BaseCrcValid));
    DEBUG ((DEBUG_INFO, " ChipsetInit Binary Use CSME ChipsetInit = %x\n", BiosChipsetInitVerInfoPtr->Reserved & BIT0));
  }
}

/**
  Print PCH_PCIE_CONFIG and serial out.

  @param[in] PchPcieConfig      Pointer to a PCH_PCIE_CONFIG that provides the platform settings
**/
VOID
PchPrintPcieConfig (
  IN CONST PCH_PCIE_CONFIG   *PchPcieConfig
  )
{
  UINT32 Index;

  DEBUG ((DEBUG_INFO, "------------------ PCH PCIE Config ------------------\n"));
  for (Index = 0; Index < GetPchMaxPciePortNum (); Index++) {
    DEBUG ((DEBUG_INFO, " RootPort[%d] HotPlug= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.HotPlug));
    DEBUG ((DEBUG_INFO, " RootPort[%d] PmSci= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PmSci));
    DEBUG ((DEBUG_INFO, " RootPort[%d] ClkReqDetect= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.ClkReqDetect));
    DEBUG ((DEBUG_INFO, " RootPort[%d] MrlSensorPresent = %d\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.MrlSensorPresent));
    DEBUG ((DEBUG_INFO, " RootPort[%d] UnsupportedRequestReport= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.UnsupportedRequestReport));
    DEBUG ((DEBUG_INFO, " RootPort[%d] FatalErrorReport= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.FatalErrorReport));
    DEBUG ((DEBUG_INFO, " RootPort[%d] NoFatalErrorReport= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.NoFatalErrorReport));
    DEBUG ((DEBUG_INFO, " RootPort[%d] CorrectableErrorReport= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.CorrectableErrorReport));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SystemErrorOnFatalError= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.SystemErrorOnFatalError));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SystemErrorOnNonFatalError= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.SystemErrorOnNonFatalError));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SystemErrorOnCorrectableError= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.SystemErrorOnCorrectableError));
    DEBUG ((DEBUG_INFO, " RootPort[%d] MaxPayload= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.MaxPayload));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SlotImplemented= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.SlotImplemented));
    DEBUG ((DEBUG_INFO, " RootPort[%d] AcsEnabled= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.AcsEnabled));
    DEBUG ((DEBUG_INFO, " RootPort[%d] PtmEnabled= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PtmEnabled));
    DEBUG ((DEBUG_INFO, " RootPort[%d] AdvancedErrorReporting= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.AdvancedErrorReporting));
    DEBUG ((DEBUG_INFO, " RootPort[%d] TransmitterHalfSwing= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.TransmitterHalfSwing));
    DEBUG ((DEBUG_INFO, " RootPort[%d] PcieSpeed= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieSpeed));
    DEBUG ((DEBUG_INFO, " RootPort[%d] PhysicalSlotNumber= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PhysicalSlotNumber));
    DEBUG ((DEBUG_INFO, " RootPort[%d] CompletionTimeout= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.CompletionTimeout));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Aspm= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.Aspm));
    DEBUG ((DEBUG_INFO, " RootPort[%d] L1Substates= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.L1Substates));
    DEBUG ((DEBUG_INFO, " RootPort[%d] LtrEnable= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.LtrEnable));
    DEBUG ((DEBUG_INFO, " RootPort[%d] LtrConfigLock= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.LtrConfigLock));
    DEBUG ((DEBUG_INFO, " RootPort[%d] LtrMaxSnoopLatency= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxSnoopLatency));
    DEBUG ((DEBUG_INFO, " RootPort[%d] LtrMaxNoSnoopLatency= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxNoSnoopLatency));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SnoopLatencyOverrideMode= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideMode));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SnoopLatencyOverrideMultiplier= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideMultiplier));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SnoopLatencyOverrideValue= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideValue));
    DEBUG ((DEBUG_INFO, " RootPort[%d] NonSnoopLatencyOverrideMode= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideMode));
    DEBUG ((DEBUG_INFO, " RootPort[%d] NonSnoopLatencyOverrideMultiplier= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideMultiplier));
    DEBUG ((DEBUG_INFO, " RootPort[%d] NonSnoopLatencyOverrideValue= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideValue));
    DEBUG ((DEBUG_INFO, " RootPort[%d] ForceLtrOverride= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.ForceLtrOverride));
    DEBUG ((DEBUG_INFO, " RootPort[%d] DetectTimeoutMs= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.DetectTimeoutMs));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SlotPowerLimitScale= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.SlotPowerLimitScale));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SlotPowerLimitValue= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.SlotPowerLimitValue));
    DEBUG ((DEBUG_INFO, " RootPort[%d] EnableCpm= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.EnableCpm));
    DEBUG ((DEBUG_INFO, " RootPort[%d] FormFactor = %d\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.FormFactor));
  }

  DEBUG ((DEBUG_INFO, " OverrideEqualizationDefaults: %d\n", PchPcieConfig->OverrideEqualizationDefaults));
  DEBUG ((DEBUG_INFO, " PcieLinkEqMethod: %d\n", PchPcieConfig->PcieLinkEqPlatformSettings.PcieLinkEqMethod));
  DEBUG ((DEBUG_INFO, " PcieLinkEqMode: %d\n", PchPcieConfig->PcieLinkEqPlatformSettings.PcieLinkEqMode));
  DEBUG ((DEBUG_INFO, " Ph1DownstreamPortTransmitterPreset: %d\n", PchPcieConfig->PcieLinkEqPlatformSettings.Ph1DownstreamPortTransmitterPreset));
  DEBUG ((DEBUG_INFO, " Ph1UpstreamPortTransmitterPreset: %d\n", PchPcieConfig->PcieLinkEqPlatformSettings.Ph1UpstreamPortTransmitterPreset));
  DEBUG ((DEBUG_INFO, " Ph3NumberOfPresetsOrCoefficients: %d\n", PchPcieConfig->PcieLinkEqPlatformSettings.Ph3NumberOfPresetsOrCoefficients));
  for (Index = 0; Index < PCIE_LINK_EQ_COEFFICIENTS_MAX; Index++) {
    DEBUG ((DEBUG_INFO,
            " Phase 3 Coefficient %d, pre-cursor = %d, post-cursor = %d\n",
            Index,
            PchPcieConfig->PcieLinkEqPlatformSettings.Ph3CoefficientsList[Index].PreCursor,
            PchPcieConfig->PcieLinkEqPlatformSettings.Ph3CoefficientsList[Index].PostCursor));
  }
  for (Index = 0; Index < PCIE_LINK_EQ_PRESETS_MAX; Index++) {
    DEBUG ((DEBUG_INFO, " Phase 3 Preset %d = %d\n", Index, PchPcieConfig->PcieLinkEqPlatformSettings.Ph3PresetList[Index]));
  }
  DEBUG ((DEBUG_INFO, " LocalTransmitterOverrideEnable = %d\n", PchPcieConfig->PcieLinkEqPlatformSettings.LocalTransmitterOverrideEnable));
  DEBUG ((DEBUG_INFO, " Ph2LocalTransmitterOverridePreset = %d\n", PchPcieConfig->PcieLinkEqPlatformSettings.Ph2LocalTransmitterOverridePreset));
  DEBUG ((DEBUG_INFO, " EnablePort8xhDecode= %x\n", PchPcieConfig->EnablePort8xhDecode));
  DEBUG ((DEBUG_INFO, " PchPciePort8xhDecodePortIndex= %x\n", PchPcieConfig->PchPciePort8xhDecodePortIndex));
  DEBUG ((DEBUG_INFO, " EnablePeerMemoryWrite= %x\n", PchPcieConfig->PcieCommonConfig.EnablePeerMemoryWrite));
  DEBUG ((DEBUG_INFO, " ComplianceTestMode= %x\n", PchPcieConfig->PcieCommonConfig.ComplianceTestMode));
  DEBUG ((DEBUG_INFO, " RpFunctionSwap= %x\n", PchPcieConfig->PcieCommonConfig.RpFunctionSwap));
}

/**
  Print RST_CONFIG and serial out.

  @param[in]  RstConfig         Pointer to a RST_CONFIG that provides the platform setting
**/
VOID
PchPrintRstConfig (
  IN CONST RST_CONFIG           *RstConfig
  )
{
  UINT32 Index;
  DEBUG ((DEBUG_INFO, "--------------------- RST Config ------------------------\n"));

  DEBUG ((DEBUG_INFO, " Raid0= %x\n", RstConfig->Raid0));
  DEBUG ((DEBUG_INFO, " Raid1= %x\n", RstConfig->Raid1));
  DEBUG ((DEBUG_INFO, " Raid10= %x\n", RstConfig->Raid10));
  DEBUG ((DEBUG_INFO, " Raid5= %x\n", RstConfig->Raid5));
  DEBUG ((DEBUG_INFO, " Irrt= %x\n", RstConfig->Irrt));
  DEBUG ((DEBUG_INFO, " OromUiBanner= %x\n", RstConfig->OromUiBanner));
  DEBUG ((DEBUG_INFO, " OromUiDelay= %x\n", RstConfig->OromUiDelay));
  DEBUG ((DEBUG_INFO, " HddUnlock= %x\n", RstConfig->HddUnlock));
  DEBUG ((DEBUG_INFO, " LedLocate= %x\n", RstConfig->LedLocate));
  DEBUG ((DEBUG_INFO, " IrrtOnly= %x\n", RstConfig->IrrtOnly));
  DEBUG ((DEBUG_INFO, " SmartStorage= %x\n", RstConfig->SmartStorage));
  DEBUG ((DEBUG_INFO, " LegacyOrom= %x\n", RstConfig->LegacyOrom));
  DEBUG ((DEBUG_INFO, " OptaneMemory= %x\n", RstConfig->OptaneMemory));
  DEBUG ((DEBUG_INFO, " CpuAttachedStorage= %x\n", RstConfig->CpuAttachedStorage));

  for (Index = 0; Index < PCH_MAX_RST_PCIE_STORAGE_CR; Index++) {
    DEBUG ((DEBUG_INFO, " RstPcieStorageRemap[%d].Enable                  = %x\n", Index, RstConfig->HardwareRemappedStorageConfig[Index].Enable));
    DEBUG ((DEBUG_INFO, " RstPcieStorageRemap[%d].RstPcieStoragePort      = %x\n", Index, RstConfig->HardwareRemappedStorageConfig[Index].RstPcieStoragePort));
    DEBUG ((DEBUG_INFO, " RstPcieStorageRemap[%d].DeviceResetDelay        = %x\n", Index, RstConfig->HardwareRemappedStorageConfig[Index].DeviceResetDelay));
  }
}

/**
  Print PCH_LOCK_DOWN_CONFIG and serial out.

  @param[in] LockDownConfig         Pointer to a PCH_LOCK_DOWN_CONFIG that provides the platform setting
**/
VOID
PchPrintLockDownConfig (
  IN CONST PCH_LOCK_DOWN_CONFIG   *LockDownConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH Lock Down Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " GlobalSmi= %x\n", LockDownConfig->GlobalSmi));
  DEBUG ((DEBUG_INFO, " BiosInterface= %x\n", LockDownConfig->BiosInterface));
  DEBUG ((DEBUG_INFO, " BiosLock= %x\n", LockDownConfig->BiosLock));
  DEBUG ((DEBUG_INFO, " UnlockGpioPads= %x\n", LockDownConfig->UnlockGpioPads));
}

/**
  Print PCH_GENERAL_CONFIG and serial out.

  @param[in] PchGeneralConfig   Pointer to a PCH_GENERAL_CONFIG that provides the platform setting
**/
VOID
PchPrintGeneralConfig (
  IN CONST PCH_GENERAL_CONFIG   *PchGeneralConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH General Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " Crid= %x\n", PchGeneralConfig->Crid));
  DEBUG ((DEBUG_INFO, " LegacyIoLowLatency = %x\n", PchGeneralConfig->LegacyIoLowLatency));
}

/**
  Print PCH_FLASH_PROTECTION_CONFIG and serial out.

  @param[in] FlashProtectConfig  Pointer to a PCH_FLASH_PROTECTION_CONFIG that provides the platform setting
**/
VOID
PchPrintFlashProtectionConfig (
  IN CONST PCH_FLASH_PROTECTION_CONFIG   *FlashProtectConfig
  )
{
  UINT32 Index;

  DEBUG ((DEBUG_INFO, "------------------ PCH Flash Protection Config ------------------\n"));
  for (Index = 0; Index < PCH_FLASH_PROTECTED_RANGES; ++Index) {
    DEBUG ((DEBUG_INFO, " WriteProtectionEnable[%d] = %x\n", Index, FlashProtectConfig->ProtectRange[Index].WriteProtectionEnable));
    DEBUG ((DEBUG_INFO, " ReadProtectionEnable[%d]  = %x\n", Index, FlashProtectConfig->ProtectRange[Index].ReadProtectionEnable));
    DEBUG ((DEBUG_INFO, " ProtectedRangeLimit[%d]   = %x\n", Index, FlashProtectConfig->ProtectRange[Index].ProtectedRangeLimit));
    DEBUG ((DEBUG_INFO, " ProtectedRangeBase[%d]    = %x\n", Index, FlashProtectConfig->ProtectRange[Index].ProtectedRangeBase));
  }
}

/**
  Print PCH_FIVR_CONFIG.

  @param[in] FivrConfig         Pointer to a PCH_FIVR_CONFIG that provides the FIVR settings

**/
VOID
PchPrintFivrConfig (
  IN CONST PCH_FIVR_CONFIG   *FivrConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH FIVR Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " ExtV1p05Rail:\n"));
  DEBUG ((DEBUG_INFO, "  EnabledStates = %x\n", FivrConfig->ExtV1p05Rail.EnabledStates));
  DEBUG ((DEBUG_INFO, "  SupportedVoltageStates = %x\n", FivrConfig->ExtV1p05Rail.SupportedVoltageStates));
  DEBUG ((DEBUG_INFO, "  Voltage = %x\n", FivrConfig->ExtV1p05Rail.Voltage));
  DEBUG ((DEBUG_INFO, "  IccMax = %x\n", FivrConfig->ExtV1p05Rail.IccMax));
  DEBUG ((DEBUG_INFO, " ExtVnnRail:\n"));
  DEBUG ((DEBUG_INFO, "  EnabledStates = %x\n", FivrConfig->ExtVnnRail.EnabledStates));
  DEBUG ((DEBUG_INFO, "  SupportedVoltageStates = %x\n", FivrConfig->ExtVnnRail.SupportedVoltageStates));
  DEBUG ((DEBUG_INFO, "  Voltage = %x\n", FivrConfig->ExtVnnRail.Voltage));
  DEBUG ((DEBUG_INFO, "  IccMax = %x\n", FivrConfig->ExtVnnRail.IccMax));
  DEBUG ((DEBUG_INFO, " ExtVnnRailSx:\n"));
  DEBUG ((DEBUG_INFO, "  EnabledStates = %x\n", FivrConfig->ExtVnnRailSx.EnabledStates));
  DEBUG ((DEBUG_INFO, "  Voltage = %x\n", FivrConfig->ExtVnnRailSx.Voltage));
  DEBUG ((DEBUG_INFO, "  IccMax = %x\n", FivrConfig->ExtVnnRailSx.IccMax));
  DEBUG ((DEBUG_INFO, " VccinAux:\n"));
  DEBUG ((DEBUG_INFO, "  LowToHighCurModeVolTranTime = %x\n", FivrConfig->VccinAux.LowToHighCurModeVolTranTime));
  DEBUG ((DEBUG_INFO, "  RetToHighCurModeVolTranTime = %x\n", FivrConfig->VccinAux.RetToHighCurModeVolTranTime));
  DEBUG ((DEBUG_INFO, "  RetToLowCurModeVolTranTime = %x\n", FivrConfig->VccinAux.RetToLowCurModeVolTranTime));
  DEBUG ((DEBUG_INFO, "  OffToHighCurModeVolTranTime = %x\n", FivrConfig->VccinAux.OffToHighCurModeVolTranTime));
  DEBUG ((DEBUG_INFO, " FivrDynPm = %x\n", FivrConfig->FivrDynPm));
}

/**
  Print whole PCH config blocks and serial out.

  @param[in] SiPolicyPpi    The RC Policy PPI instance
**/
VOID
EFIAPI
PchPrintPolicyPpi (
  IN  SI_POLICY_PPI     *SiPolicyPpi
  )
{
DEBUG_CODE_BEGIN();
  UINT32                          SataCtrlIndex;
  EFI_STATUS                      Status;
  PCH_GENERAL_CONFIG              *PchGeneralConfig;
  PCH_PCIE_CONFIG                 *PchPcieConfig;
  RST_CONFIG                      *RstConfig;
  PCH_FLASH_PROTECTION_CONFIG     *FlashProtectionConfig;
  PCH_LOCK_DOWN_CONFIG            *LockDownConfig;
  PCH_HSIO_CONFIG                 *HsioConfig;
  PCH_FIVR_CONFIG                 *FivrConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPchGeneralConfigGuid, (VOID *) &PchGeneralConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPchPcieConfigGuid, (VOID *) &PchPcieConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gFlashProtectionConfigGuid, (VOID *) &FlashProtectionConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gLockDownConfigGuid, (VOID *) &LockDownConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gFivrConfigGuid, (VOID *) &FivrConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gHsioConfigGuid, (VOID *) &HsioConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------------ PCH Print Policy Start ------------------------\n"));
  DEBUG ((DEBUG_INFO, " Revision= %x\n", SiPolicyPpi->TableHeader.Header.Revision));

  PchPrintGeneralConfig (PchGeneralConfig);
  PchPrintPcieConfig (PchPcieConfig);

  for (SataCtrlIndex = 0; SataCtrlIndex < MaxSataControllerNum(); SataCtrlIndex++) {
    RstConfig = GetPchRstConfig(SiPolicyPpi, SataCtrlIndex);
    PchPrintRstConfig (RstConfig);
  }
  PchPrintLockDownConfig (LockDownConfig);
  PchPrintFlashProtectionConfig (FlashProtectionConfig);
  PchPrintHsioConfig (HsioConfig);
  PchPrintFivrConfig (FivrConfig);

  DEBUG ((DEBUG_INFO, "------------------------ PCH Print Platform Protocol End --------------------------\n"));
DEBUG_CODE_END();
}
