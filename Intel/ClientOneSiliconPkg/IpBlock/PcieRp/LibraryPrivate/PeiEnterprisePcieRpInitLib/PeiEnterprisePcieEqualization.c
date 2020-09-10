/** @file
  Implementation of the library for enterprise PCIe root ports initialization

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

#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/PchSbiAccessLib.h>
#include <Library/PeiPcieRpLib.h>
#include <Library/DebugLib.h>
#include <Register/PchRegs.h>
#include <Library/PchPcieRpLib.h>
#include <PchPcieRpInfo.h>
#include <Register/PchPcrRegs.h>
#include <PcieRegs.h>
#include <Library/PciExpressHelpersLib.h>
#include <Library/PcieHelperLib.h>
#include <Register/EnterprisePcieRpRegs.h>
#include <PchPcieRpConfig.h>
#include <EnterprisePcieConfig.h>
#include <Library/PchPciBdfLib.h>

#include "PeiEnterprisePcieRpInitInternal.h"

/**
  Sets required transmitter presets for the root port.
  Those presets will be used during phase 1 of the equalization process
  to establish initial communication at higher speed.

  @param[in] RpDevice            Pointer to PCIE RP device
  @param[in] Ph1Settings         PCIe link EQ settings
**/
STATIC
VOID
PcieSetTransmitterPh1Presets (
  IN PCH_RP_DEVICE                             *RpDevice,
  IN ENTERPRISE_PCIE_LINK_TRANSMITTER_PRESETS  *Ph1Settings
  )
{
  UINT8  LaneIndex;
  UINT8  MaxLinkWidth;

  //
  // Enterprise PCIe root port implements only as many phase 1 preset
  // register as there are possible lanes in the link on this root port.
  // Because of that number of supported registers will be different on
  // first root port in the controller and second root port in the controller.
  // Care needs to be taken to avoid programing undefined registers.
  //
  MaxLinkWidth = GetMaxLinkWidth (RpDevice->PciCfgBaseAddr);
  for (LaneIndex = 0; LaneIndex < MaxLinkWidth; LaneIndex++) {
    //
    // Phase 1 presets are kept in a continous set of registers in PCI config
    // space. Each register holds upstream and downstream trabsmitter preset
    // for single lane.
    //
    PciSegmentAndThenOr16 (
      RpDevice->PciCfgBaseAddr + R_PCH_PCIE_CFG_LANEEQCTL_0 + (LaneIndex * S_PCH_PCIE_CFG_LANEEQCTL_0),
      (UINT16) ~(B_PCH_PCIE_CFG_LANEEQCTL_0_UPTP | B_PCH_PCIE_CFG_LANEEQCTL_0_DPTP),
      (UINT16)((Ph1Settings->UpstreamPortTxPreset << N_PCH_PCIE_CFG_LANEEQCTL_0_UPTP) |
               (Ph1Settings->DownstreamPortTxPreset << N_PCH_PCIE_CFG_LANEEQCTL_0_DPTP))
      );
  }
}

/**
  If local transmitter override is enabled this function will
  program all of the required coefficients overrides.

  @param[in] RpDevice       Pointer to PCIE RP device
  @param[in] Ph2Settings    PCIe link EQ settings and transmitter coefficient preset override settings
**/
STATIC
VOID
PcieSetTransmitterPh2Presets (
  IN PCH_RP_DEVICE                  *RpDevice,
  IN ENTERPRISE_PCIE_LINK_TX_COEFF  *Ph2Settings
  )
{
  UINT64 RpBase;
  UINT32 Data32And;
  UINT32 Data32Or;
  UINT8  LaneIndex;
  UINT8  MaxLinkWidth;

  if (Ph2Settings->LocalTxOverrideEnable) {
    MaxLinkWidth = GetMaxLinkWidth (RpDevice->PciCfgBaseAddr);
    for (LaneIndex = 0; LaneIndex < MaxLinkWidth; LaneIndex++) {
      RpBase = RpDevice->PciCfgBaseAddr + R_PCH_PCIE_MSG_GEN3TXCOEFF0 + (LaneIndex * S_PCH_PCIE_MSG_GEN3TXCOEFF0);
      Data32And   = 0;
      Data32Or    = (UINT32)(B_PCH_PCIE_MSG_GEN3TXCOEFF0_LNOVRD                                         |
                    (Ph2Settings->TxCoeffPreCursor  << N_PCH_PCIE_MSG_GEN3TXCOEFF0_G3NEGTXPRECF)  |
                    (Ph2Settings->TxCoeffCursor     << N_PCH_PCIE_MSG_GEN3TXCOEFF0_G3NEGTXMAINCF) |
                    (Ph2Settings->TxCoeffPostCursor << N_PCH_PCIE_MSG_GEN3TXCOEFF0_G3NEGTXPOSTCF));

      PciSegmentAndThenOr32 (RpBase, Data32And, Data32Or);
    }
  }
}


/**
  Sets single equalization list entry.

  @param[in] RpHandle        Pointer to PCIE RP handle
  @param[in] ListEntryIndex  Index of the ph3 equalization entry
  @param[in] ListEntry       Pointer ot entry settings
**/
STATIC
VOID
PcieSetEqEntry (
  IN PCH_RP_HANDLE                             *RpHandle,
  IN UINT32                                    ListEntryIndex,
  IN ENTERPRISE_PCIE_LINK_EQ_FOMHINTTBL_ENTRY  *ListEntry
  )
{
  EFI_STATUS  Status;
  UINT64      Offset;
  UINT32      Data32Or;


  Offset = R_PCH_PCIE_MEM_FOMHINTTBL0 + (ListEntryIndex * 4);
  //
  // For each valid list entry we set evaluation time multiplier to 0x1 and enable
  // the CTLE.
  //
  Data32Or = (B_PCH_PCIE_MEM_FOMHINTTBL0_ENABLE |
              (ListEntry->EntryFormat << N_PCH_PCIE_MEM_FOMHINTTBL0_FTYPE) |
              B_PCH_PCIE_MEM_FOMHINTTBL0_CTLE |
              (0x1 << N_PCH_PCIE_MEM_FOMHINTTBL0_ETM) |
              (ListEntry->D2 << N_PCH_PCIE_MEM_FOMHINTTBL0_D2) |
              (ListEntry->D1 << N_PCH_PCIE_MEM_FOMHINTTBL0_D1));

  Status = PchSbiRpPciMemAndThenOr32 (
             RpHandle,
             Offset,
             0,
             Data32Or
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set list entry %d on RP%d\n", ListEntryIndex, RpHandle->Device->RootPortIndex));
  }
}

/**
  Sets up ph3 list entries according to equalization settings.
  Each entry represents a data point that will be evaluated during equalization
  process with the exception of entries in the next stage format which represent
  another evaluation stage around best point from previous stage.

  @param[in] RpHandle            Pointer to PCIE RP handle
  @param[in] PcieLinkEqSettings  Structure containing equalization settings
**/
STATIC
VOID
PcieSetPh3EntryList (
  IN PCH_RP_HANDLE                       *RpHandle,
  IN ENTERPRISE_PCIE_LINK_EQ_FOMHINTTBL  *PcieLinkEqSettings
  )
{
  UINT32 ListIndex;

  for (ListIndex = 0; ListIndex < ARRAY_SIZE(PcieLinkEqSettings->Ph3Entries); ListIndex++) {
    if (PcieLinkEqSettings->Ph3Entries[ListIndex].EntryFormat != EnterprisePcieEqFormatDisabled) {
      PcieSetEqEntry (RpHandle, ListIndex, &PcieLinkEqSettings->Ph3Entries[ListIndex]);
    }
  }
}

/**
  Sets up PCIe root port registers to perform HW equalization
  flow during link training.

  @param[in] RpHandle            Pointer to PCIE RP handle
  @param[in] PcieLinkEqSettigns  Structure containing equalization settings
**/
STATIC
VOID
PcieSetupHardwareEq (
  IN PCH_RP_HANDLE                       *RpHandle,
  IN ENTERPRISE_PCIE_LINK_EQ_FOMHINTTBL  *PcieLinkEqFomHintTbl
  )
{
  EFI_STATUS  Status;

  PcieSetPh3EntryList (RpHandle, PcieLinkEqFomHintTbl);

  //
  // Set evaluation time per coefficient and max number of coefficients to evaluate
  // according to IP recommendation.
  //
  Status = PchSbiRpPciMemAndThenOr32 (
             RpHandle,
             R_PCH_PCIE_MSG_EQEVALCTL2,
             0,
             0x00000ba0
             );
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Set block error count limits according to IP recommendation.
  //
  Status = PchSbiRpPciMemAndThenOr32 (
             RpHandle,
             R_PCH_PCIE_MSG_EQEVALCTL3,
             0,
             0x00000802
             );
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Enable FOM search and set FOM error threshold. Set final evaluation loop time
  // according to IP recommendation.
  //
  Status = PchSbiRpPciMemAndThenOr32 (
             RpHandle,
             R_PCH_PCIE_MSG_EQEVALCTL4,
             0,
             0x04820004
             );
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Set master and slave phase timeout, set block lock timeouts according to
  // IP recommendation.
  //
  Status = PchSbiRpPciMemAndThenOr32 (
             RpHandle,
             R_PCH_PCIE_MSG_EQEVALCTL5,
             0,
             0x3a801013
             );
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Configure CTLE adaption as per IP recommendation.
  //
  Status = PchSbiRpPciMemAndThenOr32 (
             RpHandle,
             R_PCH_PCIE_MSG_EQEVALCTL6,
             0,
             0x04904623
             );
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Set max FOM value as per IP recommendation.
  //
  Status = PchSbiRpPciMemAndThenOr32 (
             RpHandle,
             R_PCH_PCIE_MSG_EQEVALCTL7,
             0,
             0x00000020
             );
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Set Fast and slow time count values as per IP recommendation.
  //
  Status = PchSbiRpPciMemAndThenOr32 (
             RpHandle,
             R_PCH_PCIE_MSG_FOMINCCTL,
             0,
             0x0004ffff
             );
  if (EFI_ERROR (Status)) {
    return;
  }
}

/**
  Transition root port link to link active state. This function
  is also responsible for setting up equalization for link speeds
  that require it.

  @param[in] RpHandle            Pointer to PCIE RP handle
  @param[in] PcieLinkEqSettings  Structure containing equalization settings for root port
**/
VOID
PcieRpTrainToLinkActive (
  IN PCH_RP_HANDLE                   *RpHandle,
  IN ENTERPRISE_PCIE_LINK_EQ_CONFIG  *PcieLinkEqSettings
  )
{
  UINT64 RpBase;

  RpBase = RpHandle->Device->PciCfgBaseAddr;

  PcieSetTransmitterPh1Presets (RpHandle->Device, &PcieLinkEqSettings->Ph1Settings);
  PcieSetTransmitterPh2Presets (RpHandle->Device, &PcieLinkEqSettings->Ph2Settings);
  PcieSetupHardwareEq (RpHandle, &PcieLinkEqSettings->Ph3Settings);

  //program pipectl registers to avoid hang on BIOS when VGA is in EBG PCIE slot
  PciSegmentAndThenOr32 (RpBase + R_PCH_PCIE_CFG_PIPECTL, (UINT32) ~0, (UINT32) B_PCH_PCIE_CFG_PIPECTL_PWRDNPERPORT);
  PciSegmentAndThenOr32 (RpBase + R_PCH_PCIE_CFG_PIPECTL2, (UINT32) ~(B_PCH_PCIE_CFG_PIPECTL2_L1P0_PSMAP), (UINT32) (V_PCH_PCIE_CFG_PIPECTL2_L1P0_PSMAP << N_PCH_PCIE_CFG_PIPECTL2_L1P0_PSMAP));

  PciSegmentAnd32 (RpBase + R_PCH_PCIE_CFG_LTSSMTRNCTL2, (UINT32) ~(B_PCH_PCIE_CFG_LTSSMTRNCTL2_TXRXDETPOL));

  //
  // Setting this bit starts link transition to link active. This will also start the
  // equalization process if gen3 device is attached.
  //
  PciSegmentOr8 (RpBase + R_PCH_PCIE_CFG_PPD0, B_PCH_PCIE_CFG_PPD0_ILINKTRN0);
}
