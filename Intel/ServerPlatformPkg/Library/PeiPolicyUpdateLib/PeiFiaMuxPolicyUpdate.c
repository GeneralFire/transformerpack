/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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
**/

#include <PiPei.h>
#include <Library/DebugLib.h>
#include "PeiFiaMuxPolicyUpdate.h"
#include <Library/ConfigBlockLib.h>
#include <FiaMuxConfig.h>
#include <Library/MeFiaMuxLib.h>


/**
  This function performs FIA Mux PEI Platform PreMem Policy initialization.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] PchSetup       Pointer to PCH_SETUP
**/
VOID
EFIAPI
UpdateFiaMuxPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicy,
  IN  PCH_SETUP                *PchSetup
  )
{
  PCH_FIA_MUX_PREMEM_CONFIG       *FiaMuxPreMemConfig;
  EFI_STATUS                      Status;
  UINT8                           LaneMax;             // Max lanes to be configured
  UINT8                           Lane;                // Hardware lane number
  UINT8                           LowLane;             // Low lane to be skipped
  UINT8                           HiLane;              // Hi lane to be skipped
  UINT8                           AvailableLanes;      // Lanes available for configuration

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gFiaMuxPreMemConfigGuid, (VOID *) &FiaMuxPreMemConfig);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  FiaMuxPreMemConfig->FiaMuxOverrideRequired                = PchSetup->FiaMuxOverride;
  FiaMuxPreMemConfig->FiaMuxConfig.FiaMuxRecordsCount       = 0;
  FiaMuxPreMemConfig->FiaMuxConfig.SataRecordsCount         = 0;
  FiaMuxPreMemConfig->FiaMuxConfig.PcieRootPortRecordsCount = 0;

  LaneMax = GetHsioLanes (&LowLane, &HiLane);
  AvailableLanes = LaneMax;
  if (HiLane > LowLane) {
    AvailableLanes -= HiLane;
    AvailableLanes += LowLane;
    // LaneMax base index is 1 and for HiLane and LowLane base indexes are 0.
    // Need "+1" to get the correct number of available lanes.
    AvailableLanes += 1;
  }

  FiaMuxPreMemConfig->FiaMuxConfig.FiaMuxCfgInvalidate = PchSetup->FiaMuxCfgInvalidate;

  for (Lane = 0; Lane < LaneMax; Lane++) {
    FiaMuxPreMemConfig->FiaMuxConfig.FiaMuxLaneOverride[Lane] = 0;
    if ((Lane > LowLane) && (Lane < HiLane)) {
      continue;
    }

    if (PchSetup->FiaLaneConfig[Lane] == 0xFF) {
      // no configuration override for this lane
      continue;
    }

    FiaMuxPreMemConfig->FiaMuxConfig.FiaMuxLaneOverride[Lane]                     = 1;
    FiaMuxPreMemConfig->FiaMuxConfig.FiaMux[Lane].FiaMuxHardwareIndex             = Lane;
    FiaMuxPreMemConfig->FiaMuxConfig.FiaMux[Lane].FiaMuxSelection                 = FIA_MUX_LANE_DISABLED;
    FiaMuxPreMemConfig->FiaMuxConfig.Sata[Lane].SataHardwareIndex                 = Lane;
    FiaMuxPreMemConfig->FiaMuxConfig.Sata[Lane].SataSelection                     = SATA_CONTROLLER_NOT_ASSIGNED;
    FiaMuxPreMemConfig->FiaMuxConfig.PcieRootPort[Lane].PcieRootPortHardwareIndex = Lane;
    FiaMuxPreMemConfig->FiaMuxConfig.PcieRootPort[Lane].PcieRootPortEnable        = PCIE_ROOT_PORT_IS_DISABLED;

    switch (PchSetup->FiaLaneConfig[Lane]) {
      case 0: // disabled
        FiaMuxPreMemConfig->FiaMuxConfig.FiaMuxRecordsCount++;
        FiaMuxPreMemConfig->FiaMuxConfig.PcieRootPortRecordsCount++;
      break;
      case 1: // PCIe
        FiaMuxPreMemConfig->FiaMuxConfig.FiaMux[Lane].FiaMuxSelection = FIA_MUX_LANE_PCIE_OWNER;
        FiaMuxPreMemConfig->FiaMuxConfig.FiaMuxRecordsCount++;
        FiaMuxPreMemConfig->FiaMuxConfig.PcieRootPortRecordsCount++;
        switch (PchSetup->FiaLaneLinkWidth[Lane]) {
          case 0:
            FiaMuxPreMemConfig->FiaMuxConfig.PcieRootPort[Lane].PcieRootPortLinkWidth = PCIE_ROOT_PORT_LINK_WIDTH_SET_BY_BICTRL;
            break;
          case 1:
            FiaMuxPreMemConfig->FiaMuxConfig.PcieRootPort[Lane].PcieRootPortLinkWidth = PCIE_ROOT_PORT_LINK_WIDTH_X1;
            break;
          default:
            DEBUG ((
              DEBUG_ERROR,
              "ERROR: Unknown  PCIE_ROOT_PORT_LINK_WIDTH = %d for lane %d= %d\n",
              PchSetup->FiaLaneLinkWidth[Lane],
              Lane
              ));
            ASSERT (FALSE);
        }
        FiaMuxPreMemConfig->FiaMuxConfig.PcieRootPort[Lane].PcieRootPortEnable = PCIE_ROOT_PORT_IS_ENABLED;
    // Mark lane as not SATA
    FiaMuxPreMemConfig->FiaMuxConfig.SataRecordsCount++;
      break;
      case 2: // SATA
        FiaMuxPreMemConfig->FiaMuxConfig.FiaMuxRecordsCount++;
        FiaMuxPreMemConfig->FiaMuxConfig.FiaMux[Lane].FiaMuxSelection = FIA_MUX_LANE_SATA_OWNER;
        FiaMuxPreMemConfig->FiaMuxConfig.Sata[Lane].SataSelection     = SATA_CONTROLLER_ASSIGNED;
        FiaMuxPreMemConfig->FiaMuxConfig.SataRecordsCount++;
      break;
      case 3: // XHCI
        FiaMuxPreMemConfig->FiaMuxConfig.FiaMuxRecordsCount++;
        FiaMuxPreMemConfig->FiaMuxConfig.FiaMux[Lane].FiaMuxSelection = FIA_MUX_LANE_USB_OWNER;
      break;
      default:
        ASSERT (FALSE);
    }

    if ((FiaMuxPreMemConfig->FiaMuxConfig.FiaMuxRecordsCount > AvailableLanes) ||
        (FiaMuxPreMemConfig->FiaMuxConfig.SataRecordsCount > AvailableLanes) ||
        (FiaMuxPreMemConfig->FiaMuxConfig.PcieRootPortRecordsCount > AvailableLanes)) {
      DEBUG ((DEBUG_ERROR, "ERROR: UpdateFiaMuxPreMemPolicy: Too many lanes configured (more than %d or %d)\n", LaneMax/2, AvailableLanes));
      ASSERT (FALSE);
      break;
    }
  }

  DEBUG ((
    DEBUG_ERROR,
    "\t\tFiaMuxRecordsCount = %d, SataRecordsCount = %d, PcieRootPortRecordsCount = %d\n",
    FiaMuxPreMemConfig->FiaMuxConfig.FiaMuxRecordsCount,
    FiaMuxPreMemConfig->FiaMuxConfig.SataRecordsCount,
    FiaMuxPreMemConfig->FiaMuxConfig.PcieRootPortRecordsCount
    ));
}