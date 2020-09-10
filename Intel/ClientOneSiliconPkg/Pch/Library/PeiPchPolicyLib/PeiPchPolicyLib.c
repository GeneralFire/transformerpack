/** @file
  This file is PeiPchPolicy library.

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
#include <Library/PchPcieRpLib.h>
#include <Library/CpuPlatformLib.h>
#include <Register/PchRegsLpcCnl.h>

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadPchGeneralConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_GENERAL_CONFIG  *PchGeneralConfig;
  PchGeneralConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "PchGeneralConfig->Header.GuidHob.Name = %g\n", &PchGeneralConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "PchGeneralConfig->Header.GuidHob.Header.HobLength = 0x%x\n", PchGeneralConfig->Header.GuidHob.Header.HobLength));

  /********************************
    PCH general configuration
  ********************************/
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadPcieRpConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  UINTN            Index;
  PCH_PCIE_CONFIG  *PchPcieConfig;

  PchPcieConfig = ConfigBlockPointer;

  /********************************
    PCI Express related settings
  ********************************/

  for (Index = 0; Index < GetPchMaxPciePortNum (); Index++) {
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.Aspm                   = PchPcieAspmAutoConfig;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PmSci                  = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.AcsEnabled             = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PtmEnabled             = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.DpcEnabled             = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.RpDpcExtensionsEnabled = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.MaxPayload             = PchPcieMaxPayload256;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.SlotImplemented        = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PhysicalSlotNumber     = (UINT8) Index;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.L1Substates            = PchPcieL1SubstatesL1_1_2;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.EnableCpm              = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.RelaxedOrder           = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.NoSnoop                = TRUE;
    if (IsTglPch () && IsPchLp () && (PchStepping () == PCH_Z0)) {
      PchPcieConfig->RootPort[Index].PcieRpCommonConfig.Aspm        = PchPcieAspmAutoConfig;
      PchPcieConfig->RootPort[Index].PcieRpCommonConfig.L1Substates = PchPcieL1SubstatesL1_1;
    } else if (IsAdlPch () && IsPchS ()) {
      PchPcieConfig->RootPort[Index].PcieRpCommonConfig.Aspm        = PchPcieAspmL1;
    }
    //
    // PCIe LTR Configuration.
    //
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.LtrEnable              = TRUE;
    if (IsPchLp () || IsPchN () || IsPchP ()) {
      PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxSnoopLatency              = 0x1003;
      PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxNoSnoopLatency            = 0x1003;
    } else if (IsPchH ()) {
      PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxSnoopLatency              = 0x0846;
      PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxNoSnoopLatency            = 0x0846;
    }
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideMode          = 2;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideMultiplier    = 2;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideValue         = 60;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideMode       = 2;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideMultiplier = 2;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideValue      = 60;
  }
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadPcieCommonConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_PCIE_CONFIG  *PchPcieConfig;

  PchPcieConfig = ConfigBlockPointer;

  PchPcieConfig->PcieCommonConfig.RpFunctionSwap = TRUE;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadPchPcieConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_PCIE_CONFIG  *PchPcieConfig;

  PchPcieConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "PchPcieConfig->Header.GuidHob.Name = %g\n", &PchPcieConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "PchPcieConfig->Header.GuidHob.Header.HobLength = 0x%x\n", PchPcieConfig->Header.GuidHob.Header.HobLength));

  LoadPcieCommonConfigDefault (PchPcieConfig);
  LoadPcieRpConfigDefault (PchPcieConfig);
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadRstConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  UINTN            Index;
  RST_CONFIG       *RstConfig;

  RstConfig = (RST_CONFIG *)ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "RstConfig->Header.GuidHob.Name = %g\n", &RstConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "RstConfig->Header.GuidHob.Header.HobLength = 0x%x\n", RstConfig->Header.GuidHob.Header.HobLength));

  RstConfig->Raid0              = TRUE;
  RstConfig->Raid1              = TRUE;
  RstConfig->Raid10             = TRUE;
  RstConfig->Raid5              = TRUE;
  RstConfig->Irrt               = TRUE;
  RstConfig->OromUiBanner       = TRUE;
  RstConfig->OromUiDelay        = SataOromDelay2sec;
  RstConfig->HddUnlock          = TRUE;
  RstConfig->LedLocate          = TRUE;
  RstConfig->IrrtOnly           = TRUE;
  RstConfig->SmartStorage       = TRUE;
  RstConfig->OptaneMemory       = TRUE;
  RstConfig->CpuAttachedStorage = TRUE;

  for (Index = 0; Index < PCH_MAX_RST_PCIE_STORAGE_CR; Index++) {
    RstConfig->HardwareRemappedStorageConfig[Index].DeviceResetDelay = 100;
  }
}

/**
  Get Sata Config Policy

  @param[in]  SiPolicy            The RC Policy PPI instance
  @param[in]  SataCtrlIndex       SATA controller index

  @retval     SataConfig          Pointer to Sata Config Policy
**/
SATA_CONFIG *
GetPchSataConfig (
  IN SI_POLICY_PPI      *SiPolicy,
  IN UINT32             SataCtrlIndex
  )
{
  SATA_CONFIG         *SataConfig;
  EFI_STATUS          Status;

  ASSERT (SataCtrlIndex < MaxSataControllerNum ());

  Status = GetConfigBlock ((VOID *) SiPolicy, &gSataConfigGuid, (VOID *) &SataConfig);
  ASSERT_EFI_ERROR (Status);

  SataConfig += SataCtrlIndex;

  return SataConfig;
}

/**
  Get Rst Config Policy

  @param[in]  SiPolicy            The RC Policy PPI instance
  @param[in]  SataCtrlIndex       SATA controller index

  @retval     RstConfig           Pointer to Rst Config Policy
**/
RST_CONFIG *
GetPchRstConfig (
  IN SI_POLICY_PPI      *SiPolicy,
  IN UINT32             SataCtrlIndex
  )
{
  RST_CONFIG          *RstConfig;
  EFI_STATUS          Status;

  ASSERT (SataCtrlIndex < MaxSataControllerNum ());

  Status = GetConfigBlock ((VOID *) SiPolicy, &gRstConfigGuid, (VOID *) &RstConfig);
  ASSERT_EFI_ERROR (Status);

  RstConfig += SataCtrlIndex;

  return RstConfig;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadFlashProtectionConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_FLASH_PROTECTION_CONFIG  *FlashProtectionConfig;
  FlashProtectionConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "FlashProtectionConfig->Header.GuidHob.Name = %g\n", &FlashProtectionConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "FlashProtectionConfig->Header.GuidHob.Header.HobLength = 0x%x\n", FlashProtectionConfig->Header.GuidHob.Header.HobLength));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadLockDownConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_LOCK_DOWN_CONFIG  *LockDownConfig;
  LockDownConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "LockDownConfig->Header.GuidHob.Name = %g\n", &LockDownConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "LockDownConfig->Header.GuidHob.Header.HobLength = 0x%x\n", LockDownConfig->Header.GuidHob.Header.HobLength));

  /********************************
    Lockdown configuration
  ********************************/
  LockDownConfig->GlobalSmi       = TRUE;
  LockDownConfig->BiosInterface   = TRUE;
  LockDownConfig->BiosLock        = TRUE;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadAdrConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  ADR_CONFIG  *AdrConfig;
  AdrConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "AdrConfig->Header.GuidHob.Name = %g\n", &AdrConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "AdrConfig->Header.GuidHob.Header.HobLength = 0x%x\n", AdrConfig->Header.GuidHob.Header.HobLength));

  /********************************
    Adr configuration.
  ********************************/
  AdrConfig->AdrEn = PLATFORM_POR;
  AdrConfig->AdrTimerEn = PLATFORM_POR;
  AdrConfig->AdrHostPartitionReset = PLATFORM_POR;
  AdrConfig->AdrSrcOverride = FALSE;
}

/**
  Load Config block default for FIVR

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadFivrConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_FIVR_CONFIG  *FivrConfig;
  FivrConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "FivrConfig->Header.GuidHob.Name = %g\n", &FivrConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "FivrConfig->Header.GuidHob.Header.HobLength = 0x%x\n", FivrConfig->Header.GuidHob.Header.HobLength));

  /********************************
    Fivr configuration.
  ********************************/
  FivrConfig->ExtV1p05Rail.EnabledStates = 0x0;
  FivrConfig->ExtV1p05Rail.SupportedVoltageStates = 0x0;
  FivrConfig->ExtV1p05Rail.Voltage = 0x01A4;
  FivrConfig->ExtV1p05Rail.IccMax = 0x64;
  FivrConfig->ExtVnnRail.EnabledStates = 0x0;
  FivrConfig->ExtVnnRail.Voltage = 0x1A4;
  FivrConfig->ExtVnnRail.SupportedVoltageStates = 0x0;
  FivrConfig->ExtVnnRail.IccMax = 0xC8;
  FivrConfig->ExtVnnRailSx.EnabledStates = 0x0;
  FivrConfig->ExtVnnRailSx.Voltage = 0x01A4;
  FivrConfig->ExtVnnRailSx.IccMax = 0xC8;

#ifdef PCH_TGL
  if (IsPchH () && (PchStepping () == PCH_A0)) {
    FivrConfig->VccinAux.LowToHighCurModeVolTranTime = 0x0;
    FivrConfig->VccinAux.RetToHighCurModeVolTranTime = 0x0;
  } else {
    FivrConfig->VccinAux.LowToHighCurModeVolTranTime = 0xC;
    FivrConfig->VccinAux.RetToHighCurModeVolTranTime = 0x36;
  }
  FivrConfig->VccinAux.RetToLowCurModeVolTranTime = 0x2B;
  FivrConfig->VccinAux.OffToHighCurModeVolTranTime = 0x96;
#else
  FivrConfig->VccinAux.LowToHighCurModeVolTranTime = 0x0;
  FivrConfig->VccinAux.RetToHighCurModeVolTranTime = 0x0;
  FivrConfig->VccinAux.RetToLowCurModeVolTranTime = 0x0;
  FivrConfig->VccinAux.OffToHighCurModeVolTranTime = 0x5A;
#endif
  FivrConfig->FivrDynPm = TRUE;
#ifdef PCH_TGL
  FivrConfig->ExtV1p05Rail.CtrlRampTmr  = 0x01; // Default after reset value
  FivrConfig->ExtVnnRail.CtrlRampTmr    = 0x01; // Default after reset value
#endif // PCH_TGL
  if (IsPchN() && IsJslPch()) {

    FivrConfig->ExtV1p05Rail.IccMax = 0xC8;
    FivrConfig->ExtV1p05Rail.EnabledStates = 0x1F;
    FivrConfig->ExtVnnRail.EnabledStates = 0x1F;
    FivrConfig->ExtVnnRailSx.EnabledStates = 0x1C;

    FivrConfig->ExtVnnRail.Voltage = 0x130;
    FivrConfig->VccinAux.LowToHighCurModeVolTranTime = 0xD;
    FivrConfig->VccinAux.RetToHighCurModeVolTranTime = 0x37;
    FivrConfig->VccinAux.RetToLowCurModeVolTranTime = 0x2C;
    FivrConfig->VccinAux.OffToHighCurModeVolTranTime = 0x96;
  }
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadHsioConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_HSIO_CONFIG  *HsioConfig;
  HsioConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "HsioConfig->Header.GuidHob.Name = %g\n", &HsioConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "HsioConfig->Header.GuidHob.Header.HobLength = 0x%x\n", HsioConfig->Header.GuidHob.Header.HobLength));
}

/**
  Config blocks associated with gRstConfigGuid have to be placed one after another due to our access method.
**/
GLOBAL_REMOVE_IF_UNREFERENCED COMPONENT_BLOCK_ENTRY  mPchIpBlocks [] = {
  {&gPchGeneralConfigGuid,       sizeof (PCH_GENERAL_CONFIG),           PCH_GENERAL_CONFIG_REVISION,       LoadPchGeneralConfigDefault},
  {&gPchPcieConfigGuid,          sizeof (PCH_PCIE_CONFIG),              PCIE_CONFIG_REVISION,              LoadPchPcieConfigDefault},
  {&gRstConfigGuid,              sizeof (RST_CONFIG),                   RST_CONFIG_REVISION,               LoadRstConfigDefault},
  {&gRstConfigGuid,              sizeof (RST_CONFIG),                   RST_CONFIG_REVISION,               LoadRstConfigDefault},
  {&gRstConfigGuid,              sizeof (RST_CONFIG),                   RST_CONFIG_REVISION,               LoadRstConfigDefault},
  {&gFlashProtectionConfigGuid,  sizeof (PCH_FLASH_PROTECTION_CONFIG),  FLASH_PROTECTION_CONFIG_REVISION,  LoadFlashProtectionConfigDefault},
  {&gLockDownConfigGuid,         sizeof (PCH_LOCK_DOWN_CONFIG),         LOCK_DOWN_CONFIG_REVISION,         LoadLockDownConfigDefault},
  {&gHsioConfigGuid,             sizeof (PCH_HSIO_CONFIG),              HSIO_CONFIG_REVISION,              LoadHsioConfigDefault},
  {&gAdrConfigGuid,              sizeof (ADR_CONFIG),                   ADR_CONFIG_REVISION,               LoadAdrConfigDefault}
  ,{&gFivrConfigGuid,            sizeof (PCH_FIVR_CONFIG),              FIVR_CONFIG_REVISION,              LoadFivrConfigDefault}
};

/**
  Get PCH config block table total size.

  @retval                               Size of PCH config block table
**/
UINT16
EFIAPI
PchGetConfigBlockTotalSize (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mPchIpBlocks[0], sizeof (mPchIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  PchAddConfigBlocks add all PCH config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add PCH config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
PchAddConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
  )
{
  DEBUG ((DEBUG_INFO, "PCH AddConfigBlocks\n"));

  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mPchIpBlocks[0], sizeof (mPchIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}
