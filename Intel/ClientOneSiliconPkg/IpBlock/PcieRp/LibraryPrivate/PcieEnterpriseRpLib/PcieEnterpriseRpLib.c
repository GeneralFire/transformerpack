/** @file
  This file contains routines that support PCI Express initialization

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
#include <Uefi/UefiBaseType.h>
#include <IndustryStandard/Pci.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcieHelperLib.h>
#include <Library/PchSbiAccessLib.h>
#include <Library/PcieRpLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PsfLib.h>
#include <PchPcieRpInfo.h>
#include <PchBdfAssignment.h>
#include <Register/Generated/EnterprisePcieCfgRegs.h>

/**
  Get PCIe port number for enabled port.
  @param[in] RpBase    Root Port pci segment base address

  @retval Root Port number (1 based)
**/
UINT32
PciePortNum (
  IN     UINT64  RpBase
  )
{
  UINT32       RpDev;
  UINTN        RpNumber;
  EFI_STATUS   Status;

  RpDev = (PciSegmentRead32 (RpBase + R_PCH_PCIE_CFG_LINKCAP) & B_PCH_PCIE_CFG_LINKCAP_PN) >> N_PCH_PCIE_CFG_LINKCAP_PN;
  Status = GetPchPcieRpNumber (RpDev, 0, &RpNumber);
  ASSERT_EFI_ERROR (Status);

  return (UINT32)(RpNumber + 1);
}

/**
  Get PCIe root port index
  @param[in] RpBase    Root Port pci segment base address
  @retval Root Port index (0 based)
**/
UINT32
PciePortIndex (
  IN     UINT64  RpBase
  )
{
  return PciePortNum (RpBase) - 1;
}

/**
  This function checks whether PHY lane power gating is enabled on the port.

  @param[in] RpBase                 Root Port base address

  @retval TRUE                      PHY power gating is enabled
  @retval FALSE                     PHY power gating disabled
**/
BOOLEAN
PcieIsPhyLanePgEnabled (
  IN     UINT64  RpBase
  )
{
  return FALSE;
}

/**
  Configures rootport packet split.

  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device
  @param[in] Mps                            maximum packet size
**/
VOID
ConfigureRpPacketSplit (
  UINT64 RpBase,
  UINT8  Mps
  )
{
  // dummy function for compatibility of interface
}

/**
  Configures LTR override in Root Port's proprietary registers.

  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device
  @param[in] DevNum                         currently visited device number (compatibility)
  @param[in] LtrConfig                      Root Port LTR configuration
  @param[in] AspmOverride                   combination of LTR override values from all devices under this Root Port
**/
VOID
ConfigureRpLtrOverride (
  UINT64           RpBase,
  UINT32           DevNum,
  LTR_OVERRIDE     *TreeLtr,
  PCIE_LTR_CONFIG  *LtrConfig
  )
{
  UINT32       OvrEn;
  UINT32       OvrVal;

  OvrEn = 0;
  OvrVal = 0;
  //
  // LTR settings from LTRCSR register only get acknowledged on rising edge of LTRCSR[1:0]
  // If those bits were already set (that can happen on a plug-hotUnplug-hotPlug scenario),
  // they need to be toggled
  //
  if (PciSegmentRead32 (RpBase + R_PCH_PCIE_CFG_LTRCSR) != 0) {
    PciSegmentWrite32 (RpBase + R_PCH_PCIE_CFG_LTRCSR, 0);
  }
  //
  // (*)LatencyOverrideMode = 0 -> no override
  //                          1 -> override with RP policy values
  //                          2 -> override with endpoint's override values
  //

  if (LtrConfig->ForceLtrOverride || TreeLtr->ForceOverride) {
    OvrEn |= B_PCH_PCIE_CFG_LTRCSR_LTRAMD;
  }
  if (LtrConfig->LtrConfigLock == TRUE) {
    OvrEn |= B_PCH_PCIE_CFG_LTRCSR_LTROVRPLCY;
  }

  if (LtrConfig->SnoopLatencyOverrideMode == 1) {
    OvrEn |= B_PCH_PCIE_CFG_LTRCSR_LTRSOVREN;
    OvrVal |= (UINT32)LtrConfig->SnoopLatencyOverrideValue;
    OvrVal |= LtrConfig->SnoopLatencyOverrideMultiplier << 10;
    OvrVal |= B_PCH_PCIE_CFG_LTROVR_LTRSROVR;
  } else if (LtrConfig->SnoopLatencyOverrideMode == 2) {
    if (TreeLtr->MaxSnoopLatencyRequirement) {
      OvrEn |= B_PCH_PCIE_CFG_LTRCSR_LTRSOVREN;
      OvrVal |= TreeLtr->MaxSnoopLatencyValue;
      OvrVal |= TreeLtr->MaxSnoopLatencyScale << 10;
      OvrVal |= B_PCH_PCIE_CFG_LTROVR_LTRSROVR;
    }
  }
  if (LtrConfig->NonSnoopLatencyOverrideMode == 1) {
    OvrEn |= B_PCH_PCIE_CFG_LTRCSR_LTRNSOVREN;
    OvrVal |= LtrConfig->NonSnoopLatencyOverrideValue << 16;
    OvrVal |= LtrConfig->NonSnoopLatencyOverrideMultiplier << 26;
    OvrVal |= B_PCH_PCIE_CFG_LTROVR_LTRNSROVR;
  } else if (LtrConfig->NonSnoopLatencyOverrideMode == 2) {
    if (TreeLtr->MaxNoSnoopLatencyRequirement) {
      OvrEn |= B_PCH_PCIE_CFG_LTRCSR_LTRNSOVREN;
      OvrVal |= TreeLtr->MaxNoSnoopLatencyValue << 16;
      OvrVal |= TreeLtr->MaxNoSnoopLatencyScale << 26;
      OvrVal |= B_PCH_PCIE_CFG_LTROVR_LTRNSROVR;
    }
  }
  PciSegmentWrite32 (RpBase + R_PCH_PCIE_CFG_LTROVR, OvrVal);
  PciSegmentWrite32 (RpBase + R_PCH_PCIE_CFG_LTRCSR, OvrEn);
  DEBUG ((DEBUG_INFO, "ConfigureRpLtrOverride %x Val %x En %x\n", RpBase, OvrVal, OvrEn));
}

/**
  This function configures EOI message forwarding for PCIe port.
  If there's an IoAPIC behind this port, forwarding will be enabled
  Otherwise it will be disabled to minimize bus traffic

  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device
  @param[in] IoApicPresent  TRUE if there's IoAPIC behind this Root Port
**/
VOID
ConfigureEoiForwarding (
  UINT64   RpBase,
  BOOLEAN  IoApicPresent
  )
{
  UINT32 RpIndex;

  RpIndex = PciePortIndex (RpBase);

  if (IoApicPresent == FALSE) {
    PciSegmentOr32 (RpBase + R_PCH_PCIE_CFG_PBTXNCTL, B_PCH_PCIE_CFG_PBTXNCTL_DEOIM);
  } else {
    ///
    /// If there is an IOAPIC discovered behind Root Port, program PSF Multicast registers
    /// accordingly to CNL PCH BWG  PSF EOI Multicast Configuration
    ///
    PciSegmentAnd32 (RpBase + R_PCH_PCIE_CFG_PBTXNCTL, (UINT32)~B_PCH_PCIE_CFG_PBTXNCTL_DEOIM);
    PsfConfigurEoiForPciePort (RpIndex);
  }
}

/**
  Configures proprietary parts of L1 substates configuration in Root Port

  @param[in] RpSbdf       segment:bus:device:function coordinates of Root Port
  @param[in] LtrCapable   TRUE if Root Port is LTR capable
**/
VOID
L1ssProprietaryConfiguration (
  UINT64  RpBase,
  BOOLEAN LtrCapable
  )
{
  // dummy function for interface compatibility
}
