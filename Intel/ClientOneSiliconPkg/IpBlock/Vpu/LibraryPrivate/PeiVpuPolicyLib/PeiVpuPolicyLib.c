/** @file
  This file provides services for VPU PEI policy default initialization

@copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

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

#include <Library/DebugLib.h>
#include <Library/PeiVpuPolicyLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/SiPolicy.h>
#include <SiPolicyStruct.h>
#include <Register/VpuRegs.h>

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadVpuPeiDefault (
  IN VOID    *ConfigBlockPointer
  )
{
  VPU_PEI_CONFIG              *VpuPeiConfig;

  VpuPeiConfig = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "VpuPeiConfig->Header.GuidHob.Name = %g\n", &VpuPeiConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "VpuPeiConfig->Header.GuidHob.Header.HobLength = 0x%x\n", VpuPeiConfig->Header.GuidHob.Header.HobLength));

  //
  // Initialize VPU default configuration
  //
  VpuPeiConfig->VpuEnable = 1;
}

static COMPONENT_BLOCK_ENTRY  mVpuIpBlocks = {
  &gVpuPeiConfigGuid, sizeof (VPU_PEI_CONFIG), VPU_PEI_CONFIG_REVISION, LoadVpuPeiDefault
};

/**
  Get VPU PEI config block table total size.

  @retval     Size of VPU PEI config block table
**/
UINT16
EFIAPI
VpuGetConfigBlockTotalSize (
  VOID
  )
{
  return mVpuIpBlocks.Size;
}

/**
  VpuAddConfigBlocks add all VPU PEI config block.

  @param[in] ConfigBlockTableAddress    The pointer to add SA config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
VpuAddConfigBlocks (
  IN VOID           *ConfigBlockTableAddress
  )
{
  EFI_STATUS  Status;
  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mVpuIpBlocks, 1);
  return Status;
}


/**
  This function prints the VPU PEI phase policy.

  @param[in] SiPolicyPpi - Instance of SI_POLICY_PPI
**/
VOID
EFIAPI
VpuPrintPolicyPpi (
  IN  SI_POLICY_PPI     *SiPolicyPpi
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS                   Status;
  VPU_PEI_CONFIG               *VpuPeiConfig;

  //
  // Get requisite IP Config Blocks which needs to be used here
  //
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gVpuPeiConfigGuid, (VOID *) &VpuPeiConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------------ VPU_PEI_CONFIG BEGIN-----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : %d\n", VpuPeiConfig->Header.Revision));
  DEBUG ((DEBUG_INFO, " VpuEnable : 0x%x\n", VpuPeiConfig->VpuEnable));
  DEBUG ((DEBUG_INFO, "\n------------------------ VPU_PEI_CONFIG END -----------------\n"));

  DEBUG_CODE_END ();
  return;
}
