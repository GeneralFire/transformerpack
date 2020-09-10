/** @file
  This file is PeiMePolicy library.

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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/
#include "PeiMePolicyLibrary.h"
#include "PeiMePolicy.h"

STATIC COMPONENT_BLOCK_ENTRY  mMeCompontBlockPreMemBlocks [] = {
  {&gMePeiPreMemConfigGuid, sizeof (ME_PEI_PREMEM_CONFIG),  ME_PEI_PREMEM_CONFIG_REVISION,  LoadMePeiPreMemDefault}
};

STATIC COMPONENT_BLOCK_ENTRY  mMeCompontBlockBlocks [] = {
  {&gMePeiConfigGuid,       sizeof (ME_PEI_CONFIG),         ME_PEI_CONFIG_REVISION,         LoadMePeiDefault}
};

/**
  Load default settings for ME config block in PEI phase.

  @param[in] ConfigBlockPointer                 The pointer to the config block
**/
VOID
LoadMePeiDefault (
  IN VOID                           *ConfigBlockPointer
  )
{
  ME_PEI_CONFIG *MePeiConfig;
  MePeiConfig = ConfigBlockPointer;

  MePeiConfig->EndOfPostMessage     = EOP_SEND_IN_DXE;
  MePeiConfig->MeUnconfigOnRtcClear = 1;
}

/**
  Print values of ME config block in PEI phase.

  @param[in] MePeiConfig                    The pointer to the config block
**/
VOID
EFIAPI
PrintMePeiConfig (
  IN ME_PEI_CONFIG              *MePeiConfig
  )
{
  DEBUG_CODE_BEGIN ();
  DEBUG ((DEBUG_INFO, "------------------------ ME_PEI_CONFIG -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision                  : 0x%x\n", MePeiConfig->Header.Revision));
  ASSERT (MePeiConfig->Header.Revision == ME_PEI_CONFIG_REVISION);

  DEBUG ((DEBUG_INFO, " MctpBroadcastCycle        : 0x%x\n", MePeiConfig->MctpBroadcastCycle));
  DEBUG ((DEBUG_INFO, " EndOfPostMessage          : 0x%x\n", MePeiConfig->EndOfPostMessage));
  DEBUG ((DEBUG_INFO, " DisableD0I3SettingForHeci : 0x%x\n", MePeiConfig->DisableD0I3SettingForHeci));
  DEBUG ((DEBUG_INFO, " EnforceEDebugMode         : 0x%x\n", MePeiConfig->EnforceEDebugMode));
  DEBUG ((DEBUG_INFO, " MeUnconfigOnRtcClear      : 0x%x\n", MePeiConfig->MeUnconfigOnRtcClear));

  DEBUG_CODE_END ();
}

/**
  Print PEI ME config block

  @param[in] SiPolicyPpiPreMem The RC Policy PPI instance
**/
VOID
EFIAPI
MePrintPolicyPpiPreMem (
  IN  SI_PREMEM_POLICY_PPI *SiPolicyPpiPreMem
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS                        Status;
  ME_PEI_PREMEM_CONFIG              *MePeiPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpiPreMem, &gMePeiPreMemConfigGuid, (VOID *) &MePeiPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "\n---------------------- Me Config Block Pre-Mem Print Begin -----------------\n"));
  PrintMePeiPreMemConfig (MePeiPreMemConfig);
  DEBUG ((DEBUG_INFO, "\n---------------------- Me Config Block Pre-Mem Print End -------------------\n"));
  DEBUG_CODE_END ();
}

/**
  Print PEI ME config block

  @param[in] SiPolicyPpi The RC Policy PPI instance
**/
VOID
EFIAPI
MePrintPolicyPpi (
  IN  SI_POLICY_PPI *SiPolicyPpi
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS                        Status;
  ME_PEI_CONFIG                     *MePeiConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gMePeiConfigGuid, (VOID *) &MePeiConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "\n---------------------- Me Config Block Print Begin -----------------\n"));
  PrintMePeiConfig (MePeiConfig);
  DEBUG ((DEBUG_INFO, "\n---------------------- Me Config Block Print End -------------------\n"));
  DEBUG_CODE_END ();
}

/**
  Get ME config block table total size.

  @retval        Size of ME config block table
**/
UINT16
EFIAPI
MeGetConfigBlockTotalSizePreMem (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mMeCompontBlockPreMemBlocks[0], sizeof (mMeCompontBlockPreMemBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  Get ME config block table total size.

  @retval        Size of ME config block table
**/
UINT16
EFIAPI
MeGetConfigBlockTotalSize (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mMeCompontBlockBlocks[0], sizeof (mMeCompontBlockBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  MeAddConfigBlocksPreMem add all config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
MeAddConfigBlocksPreMem (
  IN VOID           *ConfigBlockTableAddress
  )
{
  DEBUG ((DEBUG_INFO, "Me AddConfigBlocks. TotalBlockCount = 0x%x\n",  sizeof (mMeCompontBlockPreMemBlocks) / sizeof (COMPONENT_BLOCK_ENTRY)));

  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mMeCompontBlockPreMemBlocks[0], sizeof (mMeCompontBlockPreMemBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  MeAddConfigBlocks add all config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
MeAddConfigBlocks (
  IN VOID           *ConfigBlockTableAddress
  )
{
  DEBUG ((DEBUG_INFO, "ME AddConfigBlocks. TotalBlockCount = 0x%x\n",  sizeof (mMeCompontBlockBlocks) / sizeof (COMPONENT_BLOCK_ENTRY)));

  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mMeCompontBlockBlocks[0], sizeof (mMeCompontBlockBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}
