/** @file
  Implement north bridge PCI express port initialization

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary    and
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

#include <Base.h>
#include <Uefi.h>
#include <Library/DebugLib.h>

#include <RcRegs.h>
#include <PcieRegs.h>
#include <IioSetupDefinitions.h>

#include <Library/IioDataHubLib.h>
#include <Library/IioSysInfoLib.h>
#include <Library/IioAccessLib.h>

#include <LibraryPrivate/IioPwrConf.h>
#include <LibraryPrivate/IioDebug.h>
#include <LibraryPrivate/IioPcieLib.h>

#include "IioPortInit.h"

/**
  Enable Slot power: Reset power_control_bit on Slot control register.

  @param[in] IioIndex       Socket being initialized.
  @param[in] PortIndex      Port number being initialized.
**/
VOID
EnableSlotPower (
  IN  UINT8        IioIndex,
  IN  UINT8        PortIndex
  )
{
  UINT8                            PCIeCapOffset;
  PCI_REG_PCIE_SLOT_CONTROL        SlotControl;
  PCI_ROOT_BRIDGE_PCI_ADDRESS      PciAddress;

  GetPciePortBusDeviceFunction (IioIndex, PortIndex, &PciAddress);

  //
  // Enable Power to Slot: PWRCTRL on SLOTCTRL
  //
  // HSD 4166825: HSX: Polarity for PCIe Slot Power is Wrong When HotPlug is Enabled
  //
  IIO_D_PCIDBG ("[%d p%d] %a\n", IioIndex, PortIndex, __FUNCTION__);

  PCIeCapOffset = IioPCIeGetCapOffset (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
    EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (PCIeCapOffset == 0) {
    ASSERT (PCIeCapOffset != 0);
    return;
  }

  SlotControl.Uint16 = IioPciExpressRead16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
    PCIeCapOffset + R_PCIE_SLCTL_OFFSET);

  SlotControl.Bits.PowerController = 0;

  IioPciExpressWrite16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
    PCIeCapOffset + R_PCIE_SLCTL_OFFSET, SlotControl.Uint16);

  IioStall (LINK_INIT_POLL_TIMEOUT); //10ms delay
}

/**
  PCIe slot init: Slot initialization (Power parameters, clock, etc)

  @param[in] IioIndex       Socket number
  @param[in] PortIndex      Port number
**/
VOID
PcieSlotInit (
  IN  UINT8                                          IioIndex,
  IN  UINT8                                          PortIndex
  )
{
  PCI_REG_PCIE_SLOT_CAPABILITY  SltCap;
  UINT8                         PCIeCapOffset;
  PCI_ROOT_BRIDGE_PCI_ADDRESS   PciAddress;

  IIO_D_PCIDBG ("[%d p%d] %a\n", IioIndex, PortIndex, __FUNCTION__);


  GetPciePortBusDeviceFunction (IioIndex, PortIndex, &PciAddress);

  PCIeCapOffset = IioPCIeGetCapOffset (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                    EFI_PCI_CAPABILITY_ID_PCIEXP);

  if (PCIeCapOffset == 0) {
    ASSERT (PCIeCapOffset != 0);
    return;
  }

  //
  // Previously we were not calling the SlotPower Control routine for HP init so added the call if
  // the slot is HP capable.  This CALL MUST occur after programming HC Capability and MRL present capable bits
  //
  SltCap.Uint32 = IioPciExpressRead32 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
      PCIeCapOffset + R_PCIE_SLCAP_OFFSET);

  if ((SltCap.Bits.HotPlugCapable == 1) || (SltCap.Bits.MrlSensor == 1)) {
    ConfigureSlotPowerControl (IioIndex, PortIndex);
  } else {
    EnableSlotPower (IioIndex, PortIndex);
  }

}

/**
  PCIe slot pre init: Slot initialization

  @param[in] IioIndex       Socket number
  @param[in] PortIndex      Port number
**/
VOID
PcieSlotPreInit (
  IN  UINT8                     IioIndex,
  IN  UINT8                     PortIndex
  )
{
  PCI_REG_PCIE_CAPABILITY       PxpCap;
  PCI_REG_PCIE_SLOT_CAPABILITY  SltCap;
  UINT8                         MaxPortNumberPerSocket;
  UINT8                         PCIeCapOffset;
  IIO_CONFIG                    *SetupData;
  PCI_ROOT_BRIDGE_PCI_ADDRESS   PciAddress;
  UINT16                        TotalPortIndex;

  BOOLEAN                       FormFactorPresent;
  UINT8                         FormFactorValue;

  IIO_D_PCIDBG ("[%d p%d] %a\n", IioIndex, PortIndex, __FUNCTION__);

  SetupData = GetIioConfig ();

  GetPciePortBusDeviceFunction (IioIndex, PortIndex, &PciAddress);
  PCIeCapOffset = IioPCIeGetCapOffset (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                    EFI_PCI_CAPABILITY_ID_PCIEXP);

  if (PCIeCapOffset == 0) {
    ASSERT (PCIeCapOffset != 0);
    return;
  }

  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);
  TotalPortIndex = IioIndex * MaxPortNumberPerSocket + PortIndex;

  //
  // Set slot implemented bit in PCIE XCAP register
  //
  PxpCap.Uint16 = IioPciExpressRead16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
    PCIeCapOffset + R_PCIE_XCAP_OFFSET);

  PxpCap.Bits.SlotImplemented = (SetupData->SLOTIMP[TotalPortIndex] ? 1:0);

  IioPciExpressWrite16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
    PCIeCapOffset + R_PCIE_XCAP_OFFSET, PxpCap.Uint16);


  IioVppHotPlugInit (IioIndex, PortIndex);

  //
  // Update PCIE Slot Capabilities Register
  //
  if (!SetupData->PcieLinkDis[TotalPortIndex]) {             // Port not disabled in setup
    if (SetupData->SLOTIMP[TotalPortIndex]) {                // This port has slot assigned

      SltCap.Uint32 = IioPciExpressRead32 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                        PCIeCapOffset + R_PCIE_SLCAP_OFFSET);

      //
      // set physical slot number
      //
      // HSD: 4168942: PCIe physical slot number register is 13 bit,
      // but reference code interface can only program 8 bit
      //
      SltCap.Bits.PhysicalSlotNumber = SetupData->SLOTPSP[TotalPortIndex] & 0x1FFF;


      //
      // Read FormFactor value - this is present only for Gen3 controllers
      //
      GetMiscCtlStat1FormFactor (IioIndex, PortIndex, &FormFactorPresent, &FormFactorValue);

      //
      // set Electromechanical Interlock Present
      //
      if (SetupData->SLOTEIP[TotalPortIndex]) {
        // Set "electromechanical_interlock_present" based on "formfactor" for SIOM topologies.
        if (!FormFactorPresent || FormFactorValue == 1) {
          SltCap.Bits.ElectromechanicalInterlock = 1;
        }
      }

      //
      // set Attention Indicator Present
      //
      if (SetupData->SLOTAIP[TotalPortIndex]) {
        // Set "MrlSensor" based on "formfactor" for SIOM topologies.
        if (!FormFactorPresent || FormFactorValue == 0) {
          SltCap.Bits.AttentionIndicator = 1;
        }
      }

      //
      // set Slot Power Limit Scale
      //
      SltCap.Bits.SlotPowerLimitScale = SetupData->SLOTSPLS[TotalPortIndex];

      //
      // set slot power limit value
      //
      SltCap.Bits.SlotPowerLimitValue = SetupData->SLOTSPLV[TotalPortIndex];

      //
      // set Hot Plug Capable
      // If slot supports HP then enable set the correct bit in
      // in the SLOTCAp register & call slot power control
      //
      if (IioIsHpEnabledOnPort (IioIndex, PortIndex)) {
        SltCap.Bits.HotPlugCapable = 1;
        //
        // MGPHY recipe will modify no_compliance bit so set it if HP is enabled.
        // HotPlug ports should be set to block compliance entry due to the possibility
        // of unwanted compliance entry when inserting and removing cards.
        //
        IioSetNoCompliance (IioIndex, PortIndex, TRUE);
      }

      //
      // set Hot Plug Surprise if HP enabled globally and for port
      //
      if (SetupData->IioPcieConfig.PcieHotPlugEnable && SetupData->SLOTHPSUP[TotalPortIndex]) {
        SltCap.Bits.HotPlugSurprise = 1;
      }

      //
      // set Power Indicator Present
      //
      if (SetupData->SLOTPIP[TotalPortIndex]) {
        SltCap.Bits.PowerIndicator = 1;
      }

      //
      // set MRL Sensor Present
      //
      if (SetupData->SLOTMRLSP[TotalPortIndex]) {
        SltCap.Bits.MrlSensor = 1;
      }

      //
      // set Power Controller Present
      //
      if (SetupData->SLOTPCP[TotalPortIndex]) {
        SltCap.Bits.PowerController = 1;
      }

      //
      // set Attention Button Present
      //
      if (SetupData->SLOTABP[TotalPortIndex]) {
        SltCap.Bits.AttentionButton = 1;
      }

      IioPciExpressWrite32 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
        PCIeCapOffset + R_PCIE_SLCAP_OFFSET, SltCap.Uint32);

      //
      // 5385597 : Pcie Hot Plug doesn't work on LR - RP or ECB board with Winged out risers
      //
      PcieSlotInit (IioIndex, PortIndex);

    } // if: SetupData->SLOTIMP[TotalPortIndex]
  } // if: !SetupData->PcieLinkDis[TotalPortIndex]
}

/**
   Per port Early initialization before link training is done.

  @param[in] IioGlobalData  Pointer to IIO global data/methods
  @param[in] IioIndex       Current socket.
  @param[in] PortIndex      Port number being initialized.
**/
VOID
PciePortEarlyInit (
  IN IIO_GLOBALS              *IioGlobalData,
  IN UINT8                     IioIndex,
  IN UINT8                     PortIndex
  )
{
  UINT8                        MaxPortNumberPerSocket;
  PCI_ROOT_BRIDGE_PCI_ADDRESS  PciAddress;
  UINT16                       VendorID;
  UINT16                       DeviceID;
  IIO_CONFIG                  *SetupData;
  IIO_OUT_DATA                *IioOutData;

  SetupData = GetIioConfig ();
  IioOutData = GetIioOutData ();

  GetPciePortBusDeviceFunction (IioIndex, PortIndex, &PciAddress);

  VendorID = (UINT16) IioPciExpressRead16 (
      IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function, PCI_VENDOR_ID_OFFSET);
  DeviceID = (UINT16) IioPciExpressRead16 (
      IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function, PCI_DEVICE_ID_OFFSET);

  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);

  //
  // If the port designated for initialization is hidden, do not attempt to initialize
  //
  if (SetupData->PEXPHIDE[IioIndex * MaxPortNumberPerSocket + PortIndex]) {
    return;
  }

  if ((VendorID == 0xFFFF) || (DeviceID == 0xFFFF)) {
    IIO_D_PCIERR ("[%d p%d] Port init skipped - can't read Vid.\n", IioIndex, PortIndex);
    return;
  }

  //
  // Enable support for PCIE Dynamic Link Width
  //
  IioPcieEnableDlw (IioIndex, PortIndex);

  if (IioOutData->CurrentPXPMap[IioIndex * MaxPortNumberPerSocket + PortIndex]) {
    //
    // Only initialize the present port per the port map
    //
    PcieLinkInit (IioGlobalData, IioIndex, PortIndex);
    PcieSlotPreInit (IioIndex, PortIndex);

  } else {
    //
    // Put links into low power mode which are not connected to slot.
    //
    IIO_D_PCIDBG ("[%d p%d] <-- spare port\n", IioIndex, PortIndex);
  }
}
