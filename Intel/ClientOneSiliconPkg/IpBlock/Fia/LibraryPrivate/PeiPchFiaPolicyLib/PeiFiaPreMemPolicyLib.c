/** @file
  This file provides services for FIA MUX PreMem policy function.

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
#include <Uefi.h>
#include <Ppi/SiPolicy.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PchInfoLib.h>
#include <ConfigBlock/Fia/FiaMuxConfig.h>
#include <Library/PchFiaLib.h>
#include <Library/MeFiaMuxLib.h>

VOID
FiaMuxPreMemLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  );

STATIC COMPONENT_BLOCK_ENTRY  mFiaMuxPreMemBlocks [] = {
  {
    &gFiaMuxPreMemConfigGuid,
    sizeof (PCH_FIA_MUX_PREMEM_CONFIG),
    PCH_FIA_MUX_PREMEM_CONFIG_REVISION,
    FiaMuxPreMemLoadConfigDefault
  },
};

/**
  Load FIA MUX config block default.

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
FiaMuxPreMemLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_FIA_MUX_PREMEM_CONFIG  *FiaMuxPreMemConfig;
  UINT8                      Lane;

  FiaMuxPreMemConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "FiaMuxPreMemConfig->Header.GuidHob.Name = %g\n", &FiaMuxPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "FiaMuxPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", FiaMuxPreMemConfig->Header.GuidHob.Header.HobLength));

  //
  // Settings
  //

  for (Lane = 0; Lane < PchFiaGetMaxLaneNum (); Lane++) {
    FiaMuxPreMemConfig->FiaMuxConfig.FiaMuxLaneOverride [Lane]                    = 0;
    FiaMuxPreMemConfig->FiaMuxConfig.FiaMux[Lane].FiaMuxSelection                 = FIA_MUX_LANE_DISABLED;
    FiaMuxPreMemConfig->FiaMuxConfig.FiaMux[Lane].FiaMuxHardwareIndex             = Lane;
    FiaMuxPreMemConfig->FiaMuxConfig.Sata[Lane].SataHardwareIndex                 = Lane;
    FiaMuxPreMemConfig->FiaMuxConfig.PcieRootPort[Lane].PcieRootPortHardwareIndex = Lane;
  }
}

/**
  Print PCH_FIA_MUX_PREMEM_CONFIG and serial out.

  @param[in] SiPreMemPolicyPpi            Pointer to a SI_PREMEM_POLICY_PPI
**/
VOID
FiaMuxPreMemPrintConfig (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                  Status;
  PCH_FIA_MUX_PREMEM_CONFIG   *FiaMuxPreMemConfig;
  UINT8                       Lane;                 // Hardware lane number
  UINT8                       MaxLanes;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gFiaMuxPreMemConfigGuid, (VOID *) &FiaMuxPreMemConfig);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  MaxLanes = PchFiaGetMaxLaneNum ();
  if (MaxLanes > PCH_MAX_HSIO_LANES) {
    MaxLanes = PCH_MAX_HSIO_LANES;
  }

  DEBUG ((DEBUG_INFO, "------------------ FIA Mux PreMem Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " FIA configuration of validation                = %x\n", FiaMuxPreMemConfig->FiaMuxConfig.FiaMuxCfgInvalidate));
  DEBUG ((DEBUG_INFO, " FiaMuxRecordsCount                             = %x\n", FiaMuxPreMemConfig->FiaMuxConfig.FiaMuxRecordsCount));
  DEBUG ((DEBUG_INFO, " SataRecordsCount                               = %x\n", FiaMuxPreMemConfig->FiaMuxConfig.SataRecordsCount));
  DEBUG ((DEBUG_INFO, " PcieRootPortRecordsCount                       = %x\n", FiaMuxPreMemConfig->FiaMuxConfig.PcieRootPortRecordsCount));

  for (Lane = 0; Lane < MaxLanes; Lane++) {
    DEBUG ((DEBUG_INFO, " Lane: %d\n", Lane));
    // FIA
    DEBUG ((DEBUG_INFO, " FiaMuxSelection                                = "));
    switch (FiaMuxPreMemConfig->FiaMuxConfig.FiaMux[Lane].FiaMuxSelection) {
      case FIA_MUX_LANE_DISABLED:
        DEBUG ((DEBUG_INFO, "(FIA_MUX_LANE_DISABLED)\n"));
        break;
      case FIA_MUX_LANE_PCIE_OWNER:
        DEBUG ((DEBUG_INFO, "(FIA_MUX_LANE_PCIE_OWNER)\n"));
        break;
      case FIA_MUX_LANE_SATA_OWNER:
        DEBUG ((DEBUG_INFO, "(FIA_MUX_LANE_SATA_OWNER)\n"));
        break;
      case FIA_MUX_LANE_USB_OWNER:
        DEBUG ((DEBUG_INFO, "(FIA_MUX_LANE_USB_OWNER)\n"));
        break;
      default:
        DEBUG ((DEBUG_ERROR, "(ERROR: FiaMuxSelection not valid)\n"));
    }
    DEBUG ((DEBUG_INFO, " FiaMuxHardwareIndex                            = %d\n", FiaMuxPreMemConfig->FiaMuxConfig.FiaMux[Lane].FiaMuxHardwareIndex));
    // SATA
    DEBUG ((DEBUG_INFO, " SataSelection                                  = "));
    switch (FiaMuxPreMemConfig->FiaMuxConfig.Sata[Lane].SataSelection) {
      case SATA_CONTROLLER_ASSIGNED:
        DEBUG ((DEBUG_INFO, "(SATA_CONTROLLER_ASSIGNED)\n"));
        break;
      case SATA_CONTROLLER_NOT_ASSIGNED:
        DEBUG ((DEBUG_INFO, "(SATA_CONTROLLER_NOT_ASSIGNED)\n"));
        break;
      default:
        DEBUG ((DEBUG_ERROR, "(ERROR: SataSelection not valid)\n"));
    }
    DEBUG ((DEBUG_INFO, " SataHardwareIndex                              = %d\n", FiaMuxPreMemConfig->FiaMuxConfig.Sata[Lane].SataHardwareIndex));
    // PCIe
    DEBUG ((DEBUG_INFO, " PcieRootPortEnable                             = "));
    switch (FiaMuxPreMemConfig->FiaMuxConfig.PcieRootPort[Lane].PcieRootPortEnable) {
      case PCIE_ROOT_PORT_IS_ENABLED:
        DEBUG ((DEBUG_INFO, "(PCIE_ROOT_PORT_IS_ENABLED)\n"));
        break;
      case PCIE_ROOT_PORT_IS_DISABLED:
        DEBUG ((DEBUG_INFO, "(PCIE_ROOT_PORT_IS_DISABLED)\n"));
        break;
    }
    DEBUG ((DEBUG_INFO, " PcieRootPortLinkWidth                          = ", FiaMuxPreMemConfig->FiaMuxConfig.PcieRootPort[Lane].PcieRootPortLinkWidth));
    switch (FiaMuxPreMemConfig->FiaMuxConfig.PcieRootPort[Lane].PcieRootPortLinkWidth) {
      case PCIE_ROOT_PORT_LINK_WIDTH_SET_BY_BICTRL:
        DEBUG ((DEBUG_INFO, "(PCIE_ROOT_PORT_LINK_WIDTH_SET_BY_BICTRL)\n"));
        break;
      case PCIE_ROOT_PORT_LINK_WIDTH_X1:
        DEBUG ((DEBUG_INFO, "(PCIE_ROOT_PORT_LINK_WIDTH_X1)\n"));
        break;
      default:
        DEBUG ((DEBUG_ERROR, "(ERROR: PcieRootPortLinkWidth not valid)\n"));
    }
    DEBUG ((DEBUG_INFO, " PcieRootPortHardwareIndex                      = %d\n", FiaMuxPreMemConfig->FiaMuxConfig.PcieRootPort[Lane].PcieRootPortHardwareIndex));
  }
}

/**
  Get FIA Mux config block table size.

  @retval      Size of config block
**/
UINT16
FiaMuxPreMemGetConfigBlockTotalSize (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mFiaMuxPreMemBlocks[0], sizeof (mFiaMuxPreMemBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  Add FIA Mux ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
FiaMuxPreMemAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mFiaMuxPreMemBlocks[0], sizeof (mFiaMuxPreMemBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}

