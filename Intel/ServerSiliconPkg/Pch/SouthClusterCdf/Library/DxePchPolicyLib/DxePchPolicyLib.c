/** @file
  This file provide services for DXE phase policy default initialization

@copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2018 Intel Corporation.

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

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <ConfigBlock.h>
#include <Library/ConfigBlockLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Protocol/PchPolicy.h>
#include <ConfigBlock/GpioDevConfig.h>
#include <ConfigBlock/ScsConfig.h>
#include <ConfigBlock/HdAudioConfig.h>

extern PCH_PCIE_DEVICE_OVERRIDE                          mPcieDeviceTable[];

/**
  Load DXE Config block default for PCIE RP

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadPcieDxeConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCIE_RP_DXE_CONFIG  *PcieRpDxeConfig;

  PcieRpDxeConfig = ConfigBlockPointer;
  PcieRpDxeConfig->PcieDeviceOverrideTablePtr = mPcieDeviceTable;
}

/**
  Load DXE Config block default for GPIO

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadGpioDxeConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_GPIO_DXE_CONFIG  *GpioDxeConfig;
  GpioDxeConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "GpioDxeConfig->Header.GuidHob.Name = %g\n", &GpioDxeConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "GpioDxeConfig->Header.GuidHob.Header.HobLength = 0x%x\n", GpioDxeConfig->Header.GuidHob.Header.HobLength));

  GpioDxeConfig->HideGpioAcpiDevice = 0;
}

/**
  Load DXE Config block default for eMMC

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadEmmcDxeConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  SCS_EMMC_DXE_CONFIG  *EmmcDxeConfig;
  EmmcDxeConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "EmmcDxeConfig->Header.GuidHob.Name = %g\n", &EmmcDxeConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "EmmcDxeConfig->Header.GuidHob.Header.HobLength = 0x%x\n", EmmcDxeConfig->Header.GuidHob.Header.HobLength));

  EmmcDxeConfig->DriverStrength = DriverStrength40Ohm;
}

/**
  Load DXE Config block default for HD Audio

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadHdAudioDxeConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_HDAUDIO_DXE_CONFIG  *HdAudioDxeConfig;
  HdAudioDxeConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "HdaDxeConfig->Header.GuidHob.Name = %g\n", &HdAudioDxeConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "HdaDxeConfig->Header.GuidHob.Header.HobLength = 0x%x\n", HdAudioDxeConfig->Header.GuidHob.Header.HobLength));
}

GLOBAL_REMOVE_IF_UNREFERENCED COMPONENT_BLOCK_ENTRY  mPchDxeIpBlocks [] = {
  {&gGpioDxeConfigGuid, sizeof (PCH_GPIO_DXE_CONFIG), GPIO_DXE_CONFIG_REVISION, LoadGpioDxeConfigDefault},
  {&gEmmcDxeConfigGuid, sizeof (SCS_EMMC_DXE_CONFIG), SCS_EMMC_DXE_CONFIG_REVISION, LoadEmmcDxeConfigDefault},
  {&gHdAudioDxeConfigGuid, sizeof (PCH_HDAUDIO_DXE_CONFIG), HDAUDIO_DXE_CONFIG_REVISION, LoadHdAudioDxeConfigDefault},
  {&gPcieRpDxeConfigGuid, sizeof (PCIE_RP_DXE_CONFIG), PCIE_RP_DXE_CONFIG_REVISION, LoadPcieDxeConfigDefault}
};

/**
  Print PCH_GPIO_DXE_CONFIG.

  @param[in] GpioDxeConfig         Pointer to a PCH_GPIO_DXE_CONFIG that provides the GPIO settings
**/
VOID
PchPrintGpioDxeConfig (
  IN CONST PCH_GPIO_DXE_CONFIG   *GpioDxeConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH GPIO DXE Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " HideGpioAcpiDevice : %d\n", GpioDxeConfig->HideGpioAcpiDevice));
}

/**
  Print SCS_EMMC_DXE_CONFIG.

  @param[in] EmmcDxeConfig         Pointer to a SCS_EMMC_DXE_CONFIG that provides the eMMC settings
**/
VOID
PchPrintEmmcDxeConfig (
  IN CONST SCS_EMMC_DXE_CONFIG  *EmmcDxeConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH eMMC DXE Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " DriverStrength : %d\n", EmmcDxeConfig->DriverStrength));
  DEBUG ((DEBUG_INFO, " EnableSoftwareHs400Tuning: %d\n", EmmcDxeConfig->EnableSoftwareHs400Tuning));
  DEBUG ((DEBUG_INFO, " TuningLba : %X\n", EmmcDxeConfig->TuningLba));
  DEBUG ((DEBUG_INFO, " Previous tuning success : %d\n", EmmcDxeConfig->PreviousTuningResults.TuningSuccessful));
  if (EmmcDxeConfig->PreviousTuningResults.TuningSuccessful) {
    DEBUG ((DEBUG_INFO, "  Hs400 Rx DLL value : %X\n", EmmcDxeConfig->PreviousTuningResults.Hs400RxValue));
    DEBUG ((DEBUG_INFO, "  Hs400 Tx DLL value : %X\n", EmmcDxeConfig->PreviousTuningResults.Hs400TxValue));
  }
}

/**
  Print PCH_HDAUDIO_DXE_CONFIG.

  @param[in] HdaDxeConfig         Pointer to a PCH_HDAUDIO_DXE_CONFIG that provides the HD Audio settings
**/
VOID
PchPrintHdAudioDxeConfig (
  IN CONST PCH_HDAUDIO_DXE_CONFIG   *HdaDxeConfig
  )
{
  UINT32 Index;

  DEBUG ((DEBUG_INFO, "------------------ PCH HD-Audio DXE Config ------------------\n"));
  for (Index = 0; Index < PCH_MAX_HDA_SNDW_LINK_NUM; Index++) {
    DEBUG ((DEBUG_INFO, " SNDW%d AutonomousClockStop  : %d\n", Index, HdaDxeConfig->SndwConfig[Index].AutonomousClockStop));
    DEBUG ((DEBUG_INFO, " SNDW%d DODS                 : %d\n", Index, HdaDxeConfig->SndwConfig[Index].DataOnDelaySelect));
    DEBUG ((DEBUG_INFO, " SNDW%d DOAS                 : %d\n", Index, HdaDxeConfig->SndwConfig[Index].DataOnActiveIntervalSelect));
  }
  DEBUG ((DEBUG_INFO, " DSP Feature Mask              : 0x%x\n", HdaDxeConfig->DspFeatureMask));
}

/**
  Print PCIE_RP_DXE_CONFIG.

  @param[in] PcieRpDxeConfig          Pointer to a PCIE_RP_DXE_CONFIG
**/
VOID
PchPrintPcieDxeConfig (
  IN CONST PCIE_RP_DXE_CONFIG   *PcieRpDxeConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH PCIE DXE CONFIG ------------------\n"));
  DEBUG ((DEBUG_INFO, " PcieDeviceOverrideTablePtr : 0x%x\n",PcieRpDxeConfig->PcieDeviceOverrideTablePtr));
}


/**
  This function prints the PCH DXE phase policy.

  @param[in] PchPolicy - PCH DXE Policy protocol
**/
VOID
PchPrintPolicyProtocol (
  IN  PCH_POLICY_PROTOCOL      *PchPolicy
  )
{
  DEBUG_CODE_BEGIN();
  EFI_STATUS                 Status;
  PCH_GPIO_DXE_CONFIG        *GpioDxeConfig;
  SCS_EMMC_DXE_CONFIG        *EmmcDxeConfig;
  PCH_HDAUDIO_DXE_CONFIG     *HdaDxeConfig;
  PCIE_RP_DXE_CONFIG         *PcieRpDxeConfig;

  //
  // Get requisite IP Config Blocks which needs to be used here
  //
  Status = GetConfigBlock ((VOID *) PchPolicy, &gGpioDxeConfigGuid, (VOID *)&GpioDxeConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) PchPolicy, &gEmmcDxeConfigGuid, (VOID *)&EmmcDxeConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) PchPolicy, &gHdAudioDxeConfigGuid, (VOID *)&HdaDxeConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) PchPolicy, &gPcieRpDxeConfigGuid, (VOID *)&PcieRpDxeConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------------ PCH Policy (DXE) Print Start ------------------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : %x\n", PchPolicy->TableHeader.Header.Revision));

  PchPrintGpioDxeConfig (GpioDxeConfig);
  PchPrintEmmcDxeConfig (EmmcDxeConfig);
  PchPrintHdAudioDxeConfig (HdaDxeConfig);
  PchPrintPcieDxeConfig (PcieRpDxeConfig);

  DEBUG ((DEBUG_INFO, "------------------------ PCH Policy (DXE) Print End --------------------------\n"));
  DEBUG_CODE_END();
}

/**
  CreatePchDxeConfigBlocks generates the config blocksg of PCH DXE Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[out] PchPolicy                 The pointer to get PCH DXE Protocol instance

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
CreatePchDxeConfigBlocks (
  IN OUT  PCH_POLICY_PROTOCOL      **DxePchPolicy
  )
{
  UINT16              TotalBlockSize;
  EFI_STATUS          Status;
  PCH_POLICY_PROTOCOL *PchPolicyInit;
  UINT16              RequiredSize;


  DEBUG ((DEBUG_INFO, "PCH Create Dxe Config Blocks\n"));

  PchPolicyInit = NULL;

  TotalBlockSize = GetComponentConfigBlockTotalSize (&mPchDxeIpBlocks[0], sizeof (mPchDxeIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));

  DEBUG ((DEBUG_INFO, "TotalBlockSize = 0x%x\n", TotalBlockSize));

  RequiredSize = sizeof (CONFIG_BLOCK_TABLE_HEADER) + TotalBlockSize;

  Status = CreateConfigBlockTable (RequiredSize, (VOID *) &PchPolicyInit);
  ASSERT_EFI_ERROR (Status);

  //
  // General initialization
  //
  PchPolicyInit->TableHeader.Header.Revision = PCH_POLICY_PROTOCOL_REVISION;
  //
  // Add config blocks.
  //
  Status =  AddComponentConfigBlocks ((VOID *) PchPolicyInit, &mPchDxeIpBlocks[0], sizeof (mPchDxeIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
  ASSERT_EFI_ERROR (Status);

  //
  // Assignment for returning SaInitPolicy config block base address
  //
  *DxePchPolicy = PchPolicyInit;
  return Status;
}

/**
  PchInstallPolicyProtocol installs PCH Policy.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] ImageHandle                Image handle of this driver.
  @param[in] SaPolicy                   The pointer to SA Policy Protocol instance

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
PchInstallPolicyProtocol (
  IN  EFI_HANDLE                  ImageHandle,
  IN  PCH_POLICY_PROTOCOL         *PchPolicy
  )
{

  EFI_STATUS            Status;

  ///
  /// Print PCH DXE Policy
  ///
  PchPrintPolicyProtocol (PchPolicy);

  ///
  /// Install protocol to to allow access to this Policy.
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gPchPolicyProtocolGuid,
                  PchPolicy,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
