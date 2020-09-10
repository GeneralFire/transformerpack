/** @file
  This file is PeiSpsPolicy library.

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
#include "PeiSpsPreMemPolicyLibrary.h"
#include <ConfigBlock/Sps/SpsPeiConfig.h>
#include <Library/MeTypeLib.h>
#include <MeDefines.h>
#include <Library/MeConfigLib.h>

/**
  Load default settings for SPS config block in PEI phase.

  @param[in] ConfigBlockPointer                 The pointer to the config block
**/
VOID
LoadSpsPeiDefault (
  IN VOID  *ConfigBlockPointer
  );

STATIC COMPONENT_BLOCK_ENTRY  mSpsIpBlocks [] = {
  {&gSpsPeiConfigGuid,      sizeof (SPS_PEI_CONFIG),          SPS_PEI_CONFIG_REVISION,      LoadSpsPeiDefault},
};

/**
  Get SPS config block table total size.

  @retval        Size of SPS config block table
**/
UINT16
EFIAPI
SpsPreMemGetConfigBlockTotalSize (
  VOID
  )
{
#ifdef ME_SPS_SUPPORT
  return GetComponentConfigBlockTotalSize (&mSpsIpBlocks[0], sizeof (mSpsIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
#else // ME_SPS_SUPPORT
  return 0;
#endif // ME_SPS_SUPPORT
}

/**
  SpsAddConfigBlocks add all SPS config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add SPS config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
SpsPreMemAddConfigBlocks (
  IN VOID      *ConfigBlockTableAddress
  )
{
#ifdef ME_SPS_SUPPORT
  DEBUG ((DEBUG_INFO, "SPS AddConfigBlocks\n"));

  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mSpsIpBlocks[0], sizeof (mSpsIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
#else // ME_SPS_SUPPORT
  return EFI_SUCCESS;
#endif // ME_SPS_SUPPORT
}

/**
  Load default settings for SPS config block in PEI phase.

  @param[in] ConfigBlockPointer                 The pointer to the config block
**/
VOID
LoadSpsPeiDefault (
  IN VOID                           *ConfigBlockPointer
  )
{
#ifdef ME_SPS_SUPPORT
  SPS_PEI_CONFIG        *SpsPeiConfig;

  DEBUG ((DEBUG_INFO, "SPS LoadSpsPeiDefault\n"));

  SpsPeiConfig = ConfigBlockPointer;

  SetSpsPeiPolicyDefaults (SpsPeiConfig);
#endif // ME_SPS_SUPPORT
}

/**
  Dump values of SPS config block in PEI phase.

  @param[in] SpsPeiConfig                   The pointer to the config block
**/
VOID
EFIAPI
PrintSpsPeiConfig (
  IN  SPS_PEI_CONFIG            *SpsPeiConfig
  )
{
#ifdef ME_SPS_SUPPORT
  if (SpsPeiConfig == NULL) {
    DEBUG ((DEBUG_ERROR, "ERROR : SPS Config Block Print - no config block\n"));
    return;
  }

  PrintMeType (L"PeiSpsPolicy");

  DEBUG ((DEBUG_INFO, "------------------------ SPS_PEI_CONFIG -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision                           : 0x%x\n", SpsPeiConfig->Header.Revision));
  ASSERT (SpsPeiConfig->Header.Revision == SPS_PEI_CONFIG_REVISION);
  DEBUG ((DEBUG_INFO, "---------------------------------------------------------\n"));
  DEBUG ((DEBUG_INFO, " IderDeviceEnable                   : 0x%x\n", SpsPeiConfig->IderDeviceEnable));
  DEBUG ((DEBUG_INFO, " HsioMessaging                      : 0x%x\n", SpsPeiConfig->HsioMessaging));
  DEBUG ((DEBUG_INFO, " HeciCommunication4                 : 0x%x\n", SpsPeiConfig->HeciCommunication4));
  DEBUG ((DEBUG_INFO, " PreDidMeResetEnabled               : 0x%x\n", SpsPeiConfig->PreDidMeResetEnabled));
  DEBUG ((DEBUG_INFO, " MeTimeout                          : 0x%x\n", SpsPeiConfig->MeTimeout));
  DEBUG ((DEBUG_INFO, " SpsIccClkSscSetting                : 0x%x\n", SpsPeiConfig->SpsIccClkSscSetting));
  DEBUG ((DEBUG_INFO, " FiaMuxMessageEnabled               : 0x%x\n", SpsPeiConfig->FiaMuxMessageEnabled));
  DEBUG ((DEBUG_INFO, " SolProvisioningState               : 0x%x\n", SpsPeiConfig->SolProvisioningState));
  DEBUG ((DEBUG_INFO, "---------------------------------------------------------\n"));
  DEBUG ((DEBUG_INFO, " SpsCommonConfig                    : 0x%p\n", &(SpsPeiConfig->Common)));
  DEBUG ((DEBUG_INFO, "---------------------------------------------------------\n"));
  DEBUG ((DEBUG_INFO, " SpsMctpBusOwner                    : 0x%x\n", SpsPeiConfig->Common.SpsMctpBusOwner));
  DEBUG ((DEBUG_INFO, " NmCores2Disable                    : 0x%x\n", SpsPeiConfig->Common.NmCores2Disable));
  DEBUG ((DEBUG_INFO, " NmPwrOptBootOverride               : 0x%x\n", SpsPeiConfig->Common.NmPwrOptBootOverride));
  DEBUG ((DEBUG_INFO, " NmPwrOptBoot                       : 0x%x\n", SpsPeiConfig->Common.NmPwrOptBoot));
  DEBUG ((DEBUG_INFO, " NmCores2DisableOverride            : 0x%x\n", SpsPeiConfig->Common.NmCores2DisableOverride));
  DEBUG ((DEBUG_INFO, " NmPowerMsmtSupport                 : 0x%x\n", SpsPeiConfig->Common.NmPowerMsmtSupport));
  DEBUG ((DEBUG_INFO, " NmHwChangeStatus                   : 0x%x\n", SpsPeiConfig->Common.NmHwChangeStatus));
  DEBUG ((DEBUG_INFO, " MeWatchdogControlEnabled           : 0x%x\n", SpsPeiConfig->Common.MeWatchdogControlEnabled));
  DEBUG ((DEBUG_INFO, " MeWatchdogDisable                  : 0x%x\n", SpsPeiConfig->Common.MeWatchdogDisable));
  DEBUG ((DEBUG_INFO, " MeRegionSelectEnabled              : 0x%x\n", SpsPeiConfig->Common.MeRegionSelectEnabled));
  DEBUG ((DEBUG_INFO, " CoreBiosDoneEnabled                : 0x%x\n", SpsPeiConfig->Common.CoreBiosDoneEnabled));
  DEBUG ((DEBUG_INFO, " Heci1HideInMe                      : 0x%x\n", SpsPeiConfig->Common.Heci1HideInMe));
  DEBUG ((DEBUG_INFO, " Heci2HideInMe                      : 0x%x\n", SpsPeiConfig->Common.Heci2HideInMe));
  DEBUG ((DEBUG_INFO, " Heci3HideInMe                      : 0x%x\n", SpsPeiConfig->Common.Heci3HideInMe));
  DEBUG ((DEBUG_INFO, " Heci4HideInMe                      : 0x%x\n", SpsPeiConfig->Common.Heci4HideInMe));
  DEBUG ((DEBUG_INFO, " MeHmrfpoLockEnabled                : 0x%x\n", SpsPeiConfig->Common.MeHmrfpoLockEnabled));
  DEBUG ((DEBUG_INFO, " MeHmrfpoEnableEnabled              : 0x%x\n", SpsPeiConfig->Common.MeHmrfpoEnableEnabled));

#endif // ME_SPS_SUPPORT
}

/**
  Print PEI SPS Config Block (pre mem)

  @param[in] SiPolicyPreMemPpi The RC Policy PPI instance
**/
VOID
EFIAPI
SpsPreMemPrintPolicyPpi (
  IN  SI_PREMEM_POLICY_PPI *SiPolicyPreMemPpi
  )
{
#ifdef ME_SPS_SUPPORT
  DEBUG_CODE_BEGIN ();
  EFI_STATUS                        Status;
  SPS_PEI_CONFIG                    *SpsPeiConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPreMemPpi, &gSpsPeiConfigGuid, (VOID *) &SpsPeiConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "\n---------------------- SPS Config Block Print (pre mem) Begin -----------------\n"));
  PrintSpsPeiConfig (SpsPeiConfig);
  DEBUG ((DEBUG_INFO, "\n---------------------- SPS Config Block Print (pre mem) End -------------------\n"));

  DEBUG_CODE_END ();
#endif // ME_SPS_SUPPORT
}

/**
  Print PEI SPS Config Block (post mem)

  @param[in] SiPolicyPreMemPpi The RC Policy PPI instance
**/
VOID
EFIAPI
SpsPrintPolicyPpi (
  IN  SI_POLICY_PPI *SiPolicyPpi
  )
{
#ifdef ME_SPS_SUPPORT
  DEBUG_CODE_BEGIN ();
  EFI_STATUS                        Status;
  SPS_PEI_CONFIG                    *SpsPeiConfig;
  SI_PREMEM_POLICY_PPI              *SiPolicyPreMemPpi;

  Status = PeiServicesLocatePpi (&gSiPreMemPolicyPpiGuid, 0, NULL, (VOID **)&SiPolicyPreMemPpi);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "ERROR : Can't get SiPreMemPolicy\n"));
    ASSERT_EFI_ERROR (Status);
    return;
  }

  Status = GetConfigBlock ((VOID *) SiPolicyPreMemPpi, &gSpsPeiConfigGuid, (VOID *) &SpsPeiConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "\n---------------------- SPS Config Block Print (post mem) Begin -----------------\n"));
  PrintSpsPeiConfig (SpsPeiConfig);
  DEBUG ((DEBUG_INFO, "\n---------------------- SPS Config Block Print (post mem) End -------------------\n"));

  DEBUG_CODE_END ();
#endif // ME_SPS_SUPPORT
}