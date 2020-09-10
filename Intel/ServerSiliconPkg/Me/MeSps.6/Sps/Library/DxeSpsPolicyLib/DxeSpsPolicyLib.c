/** @file
  This file is DxeSpsPolicyLib library.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation.

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
#include "DxeSpsPolicyLibrary.h"
#include <ConfigBlock/Sps/SpsDxeConfig.h>
#include <Protocol/SpsPolicyProtocol.h>
#include <Library/MeTypeLib.h>

/**
  Load default settings for SPS config block in DXE phase.

  @param[in] ConfigBlockPointer                 The pointer to the config block
**/
VOID
LoadSpsDxeDefault (
  IN VOID                           *ConfigBlockPointer
  );

STATIC COMPONENT_BLOCK_ENTRY  mSpsIpBlocks [] = {
  {&gSpsDxeConfigGuid,        sizeof (SPS_DXE_CONFIG),            SPS_DXE_CONFIG_REVISION,        LoadSpsDxeDefault},
};

/**
  Get total size of IP list for the config block.

  @param[in] IpBlocks                   The pointer to the IP list
  @param[in] TotalIpSize                The size of IP list

  @retval                               Size of config block table
**/
STATIC
UINT16
EFIAPI
GetConfigBlockTotalSize (
  IN COMPONENT_BLOCK_ENTRY  IpBlocks [],
  IN UINTN                  TotalIpSize
  )
{
  UINT16            TotalBlockCount;
  UINT16            TotalBlockSize;
  UINT16            ConfigBlockHdrSize;
  UINT16            BlockCount;

  TotalBlockCount = (UINT16) (TotalIpSize / sizeof (COMPONENT_BLOCK_ENTRY));
  TotalBlockSize = 0;
  for (BlockCount = 0 ; BlockCount < TotalBlockCount; BlockCount++) {
    TotalBlockSize += (UINT32) IpBlocks[BlockCount].Size;
    DEBUG ((DEBUG_INFO, "TotalBlockSize after adding Block[0x%x]= 0x%x\n", BlockCount, TotalBlockSize));
  }
  ConfigBlockHdrSize = sizeof (CONFIG_BLOCK_TABLE_HEADER);

  return ConfigBlockHdrSize + TotalBlockSize;
}

/**
  CreateConfigBlocksByIpBlocks generates the config blocks for IP list.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[in] IpBlocks                     The pointer to the IP list
  @param[in] TotalIpSize                  The size of IP list
  @param[in, out] ConfigBlockTableHeader  The pointer to the config block

  @retval EFI_SUCCESS                     The config block for IP list is created and initialized.
  @retval EFI_OUT_OF_RESOURCES            Insufficient resources to create buffer
**/
STATIC
EFI_STATUS
EFIAPI
CreateConfigBlocksByIpBlocks (
  IN COMPONENT_BLOCK_ENTRY          IpBlocks [],
  IN UINTN                          TotalIpSize,
  IN OUT CONFIG_BLOCK_TABLE_HEADER  **ConfigBlockTableHeader
  )
{
  UINT16            TotalBlockCount;
  UINT16            BlockCount;
  VOID              *ConfigBlockPointer;
  CONFIG_BLOCK      ConfigBlockBuf;
  EFI_STATUS        Status;
  UINT16            RequiredSize;
  CONFIG_BLOCK_TABLE_HEADER      *TableHeader;

  DEBUG ((DEBUG_INFO, "CreateConfigBlocksByIpBlocks\n"));

  TotalBlockCount = (UINT16) (TotalIpSize / sizeof (COMPONENT_BLOCK_ENTRY));
  DEBUG ((DEBUG_INFO, "TotalBlockCount = 0x%x\n", TotalBlockCount));

  RequiredSize = GetConfigBlockTotalSize (IpBlocks, TotalIpSize);

  Status = CreateConfigBlockTable (RequiredSize, (VOID *)&TableHeader);
  ASSERT_EFI_ERROR (Status);

  if (!EFI_ERROR (Status)) {
    //
    // Initialize ConfigBlockPointer to NULL
    //
    ConfigBlockPointer = NULL;
    //
    // Loop to identify each config block from IpBlocks[] Table and add each of them
    //
    for (BlockCount = 0 ; BlockCount < TotalBlockCount; BlockCount++) {
      CopyMem (&(ConfigBlockBuf.Header.GuidHob.Name), IpBlocks[BlockCount].Guid, sizeof (EFI_GUID));
      ConfigBlockBuf.Header.GuidHob.Header.HobLength = IpBlocks[BlockCount].Size;
      ConfigBlockBuf.Header.Revision        = IpBlocks[BlockCount].Revision;
      ConfigBlockPointer = (VOID *)&ConfigBlockBuf;
      Status = AddConfigBlock ((VOID *) TableHeader, (VOID *)&ConfigBlockPointer);
      ASSERT_EFI_ERROR (Status);
      if (IpBlocks[BlockCount].LoadDefault != NULL) {
        IpBlocks[BlockCount].LoadDefault (ConfigBlockPointer);
      }
    }
    //
    // Assignment for returning config block base address
    //
    *ConfigBlockTableHeader = TableHeader;
    return EFI_SUCCESS;
  }

  return Status;
}

/**
  Load default settings for SPS config block in DXE phase.

  @param[in] ConfigBlockPointer                 The pointer to the config block
**/
VOID
LoadSpsDxeDefault (
  IN VOID                           *ConfigBlockPointer
  )
{
  SPS_DXE_CONFIG         *SpsDxeConfig;

  SpsDxeConfig = ConfigBlockPointer;
}

/**
  Dump values of SPS config block in DXE phase.

  @param[in] SpsDxeConfig                        The pointer to the config block
**/
VOID
EFIAPI
PrintSpsDxeConfig (
  IN SPS_DXE_CONFIG                  *SpsDxeConfig
  )
{
  DEBUG_CODE_BEGIN ();

  PrintMeType (L"DxeSpsPolicy");

  DEBUG ((DEBUG_INFO, "------------------------ SPS_DXE_CONFIG -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision                           : 0x%x\n", SpsDxeConfig->Header.Revision));
  ASSERT (SpsDxeConfig->Header.Revision == SPS_DXE_CONFIG_REVISION);
  DEBUG ((DEBUG_INFO, "---------------------------------------------------------\n"));
  DEBUG ((DEBUG_INFO, " DelayedAuthenticationMode          : 0x%x\n", SpsDxeConfig->DelayedAuthenticationMode));
  DEBUG ((DEBUG_INFO, " DelayedAuthenticationModeOverride  : 0x%x\n", SpsDxeConfig->DelayedAuthenticationModeOverride));
  DEBUG ((DEBUG_INFO, " MeHmrfpoViaHeci3                   : 0x%x\n", SpsDxeConfig->MeHmrfpoViaHeci3));
  DEBUG ((DEBUG_INFO, " MeGrLockEnabled                    : 0x%x\n", SpsDxeConfig->MeGrLockEnabled));
  DEBUG ((DEBUG_INFO, " MeGrPromotionEnabled               : 0x%x\n", SpsDxeConfig->MeGrPromotionEnabled));
  DEBUG ((DEBUG_INFO, " BreakRtcEnabled                    : 0x%x\n", SpsDxeConfig->BreakRtcEnabled));
  DEBUG ((DEBUG_INFO, " NmPowerMsmtOverride                : 0x%x\n", SpsDxeConfig->NmPowerMsmtOverride));
  DEBUG ((DEBUG_INFO, " NmHwChangeOverride                 : 0x%x\n", SpsDxeConfig->NmHwChangeOverride));
  DEBUG ((DEBUG_INFO, " NmPtuLoadOverride                  : 0x%x\n", SpsDxeConfig->NmPtuLoadOverride));
  DEBUG ((DEBUG_INFO, " SpsAltitude                        : 0x%x\n", SpsDxeConfig->SpsAltitude));
  DEBUG ((DEBUG_INFO, " MeHmrfpoEnableRegion               : 0x%x\n", SpsDxeConfig->MeHmrfpoEnableRegion));
  DEBUG ((DEBUG_INFO, "---------------------------------------------------------\n"));
  DEBUG ((DEBUG_INFO, " SpsCommonConfig                    : 0x%p\n", &(SpsDxeConfig->Common)));
  DEBUG ((DEBUG_INFO, "---------------------------------------------------------\n"));
  DEBUG ((DEBUG_INFO, " SpsMctpBusOwner                    : 0x%x\n", SpsDxeConfig->Common.SpsMctpBusOwner));
  DEBUG ((DEBUG_INFO, " NmCores2Disable                    : 0x%x\n", SpsDxeConfig->Common.NmCores2Disable));
  DEBUG ((DEBUG_INFO, " NmPwrOptBootOverride               : 0x%x\n", SpsDxeConfig->Common.NmPwrOptBootOverride));
  DEBUG ((DEBUG_INFO, " NmPwrOptBoot                       : 0x%x\n", SpsDxeConfig->Common.NmPwrOptBoot));
  DEBUG ((DEBUG_INFO, " NmCores2DisableOverride            : 0x%x\n", SpsDxeConfig->Common.NmCores2DisableOverride));
  DEBUG ((DEBUG_INFO, " NmPowerMsmtSupport                 : 0x%x\n", SpsDxeConfig->Common.NmPowerMsmtSupport));
  DEBUG ((DEBUG_INFO, " NmHwChangeStatus                   : 0x%x\n", SpsDxeConfig->Common.NmHwChangeStatus));
  DEBUG ((DEBUG_INFO, " MeWatchdogControlEnabled           : 0x%x\n", SpsDxeConfig->Common.MeWatchdogControlEnabled));
  DEBUG ((DEBUG_INFO, " MeWatchdogDisable                  : 0x%x\n", SpsDxeConfig->Common.MeWatchdogDisable));
  DEBUG ((DEBUG_INFO, " MeRegionSelectEnabled              : 0x%x\n", SpsDxeConfig->Common.MeRegionSelectEnabled));
  DEBUG ((DEBUG_INFO, " CoreBiosDoneEnabled                : 0x%x\n", SpsDxeConfig->Common.CoreBiosDoneEnabled));
  DEBUG ((DEBUG_INFO, " Heci1HideInMe                      : 0x%x\n", SpsDxeConfig->Common.Heci1HideInMe));
  DEBUG ((DEBUG_INFO, " Heci2HideInMe                      : 0x%x\n", SpsDxeConfig->Common.Heci2HideInMe));
  DEBUG ((DEBUG_INFO, " Heci3HideInMe                      : 0x%x\n", SpsDxeConfig->Common.Heci3HideInMe));
  DEBUG ((DEBUG_INFO, " Heci4HideInMe                      : 0x%x\n", SpsDxeConfig->Common.Heci4HideInMe));
  DEBUG ((DEBUG_INFO, " MeHmrfpoLockEnabled                : 0x%x\n", SpsDxeConfig->Common.MeHmrfpoLockEnabled));
  DEBUG ((DEBUG_INFO, " MeHmrfpoEnableEnabled              : 0x%x\n", SpsDxeConfig->Common.MeHmrfpoEnableEnabled));

  DEBUG_CODE_END ();
}

/**
  This function prints the SPS DXE phase policy.

  @param[in] DxeSpsPolicy - SPS DXE Policy protocol
**/
VOID
EFIAPI
SpsPrintPolicyProtocol (
  IN  SPS_POLICY_PROTOCOL            *DxeSpsPolicy
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS                        Status;
  SPS_DXE_CONFIG                    *SpsDxeConfig;

  Status = GetConfigBlock ((VOID *) DxeSpsPolicy, &gSpsDxeConfigGuid, (VOID *) &SpsDxeConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "\n---------------------- DxeSpsPolicy Print Begin -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : 0x%x\n", DxeSpsPolicy->TableHeader.Header.Revision));
  ASSERT (DxeSpsPolicy->TableHeader.Header.Revision == SPS_POLICY_PROTOCOL_REVISION);

  PrintSpsDxeConfig (SpsDxeConfig);

  DEBUG ((DEBUG_INFO, "\n---------------------- DxeSpsPolicy Print End -------------------\n"));
  DEBUG_CODE_END ();
}

/**
  CreateConfigBlocks generates the config blocksg of SPS Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[in, out] DxeSpsPolicy           The pointer to get SPS Policy protocol instance

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
SpsDxeCreateConfigBlocks (
  IN OUT  SPS_POLICY_PROTOCOL            **DxeSpsPolicy
  )
{
  EFI_STATUS        Status;

  DEBUG ((DEBUG_INFO, "SpsDxeCreateConfigBlocks\n"));
  Status = EFI_NOT_FOUND;

  Status = CreateConfigBlocksByIpBlocks (mSpsIpBlocks, sizeof (mSpsIpBlocks), (CONFIG_BLOCK_TABLE_HEADER **) DxeSpsPolicy);
  if (!EFI_ERROR (Status)) {
    //
    // Initialize Policy Revision
    //
    (*DxeSpsPolicy)->TableHeader.Header.Revision = SPS_POLICY_PROTOCOL_REVISION;
  }

  DEBUG ((DEBUG_INFO, "SpsDxeCreateConfigBlocks done, Status: %r\n", Status));

  return Status;
}

/**
  SpsCreatePolicyDefaults creates the default setting of SPS Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[in, out] DxeSpsPolicy           The pointer to get SPS Policy protocol instance

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
SpsCreatePolicyDefaults (
  IN OUT  SPS_POLICY_PROTOCOL            **DxeSpsPolicy
  )
{
  return SpsDxeCreateConfigBlocks (DxeSpsPolicy);
}

/**
  SpsInstallPolicyProtocol installs SPS Policy.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] ImageHandle                Image handle of this driver.
  @param[in] DxeSpsPolicy                The pointer to SPS Policy Protocol instance

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
SpsInstallPolicyProtocol (
  IN  EFI_HANDLE                    ImageHandle,
  IN  SPS_POLICY_PROTOCOL           *DxeSpsPolicy
  )
{
  EFI_STATUS            Status;

  ///
  /// Print SPS DXE Policy
  ///
  SpsPrintPolicyProtocol (DxeSpsPolicy);

  ///
  /// Install protocol to allow access to this Policy.
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gDxeSpsPolicyProtocolGuid,
                  DxeSpsPolicy,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

