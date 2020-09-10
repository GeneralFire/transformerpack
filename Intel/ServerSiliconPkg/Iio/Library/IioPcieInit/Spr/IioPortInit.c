/** @file
  Implement north bridge PCI express port initialization

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#include <IioSetupDefinitions.h>
#include <Library/IioDataHubLib.h>
#include <Library/IioAccessLib.h>
#include <RcRegs.h>
#include <PcieRegs.h>
#include <Library/IioDataHubLib.h>
#include <LibraryPrivate/IioPwrConf.h>
#include <Library/IioSysInfoLib.h>

#include <LibraryPrivate/IioDebug.h>
#include <LibraryPrivate/IioPcieLib.h>

//
// Interrupt Pin register values telling which pin the function uses.
//
#define INTP_INTA                   0x1

/**
    Configure SLOT power control register.

    @param IioIndex      - Socket number of the port being configured.
    @param PortIndex     - Port number being configured.
**/
VOID
ConfigureSlotPowerControl (
  IN  UINT8                                          IioIndex,
  IN  UINT8                                          PortIndex
  )
{
  UINT16                          LoopCount;
  UINT8                           PCIeCapOffset;
  PCI_REG_PCIE_SLOT_CONTROL       SlotControl;
  PCI_REG_PCIE_SLOT_STATUS        SlotStatus;
  PCI_ROOT_BRIDGE_PCI_ADDRESS     PciAddress;

  GetPciePortBusDeviceFunction (IioIndex, PortIndex, &PciAddress);
  IIO_D_PCIDBG ("%s:%d Port:%d Bus:%x Dev:%d Func:%d Slot Power...\n",
    __FUNCTION__, IioIndex, PortIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function);

  //
  // An initial write to slot control register is required for VPP bus to initialize.
  // Any read before this will result in stall data.
  //
  SlotControl.Uint16 = IioReadCpuCsr16 (IioIndex, PortIndex, SLOTCTL_IIO_PCIE_G5_REG);
  IioWriteCpuCsr16 (IioIndex, PortIndex, SLOTCTL_IIO_PCIE_G5_REG, SlotControl.Uint16);

  //
  // wait on slot control command to complete (max. 100ms)
  //
  LoopCount = 0;
  while (LoopCount < 100) {
    SlotStatus.Uint16 = IioReadCpuCsr16 (IioIndex, PortIndex, SLOTSTS_IIO_PCIE_G5_REG);
    if (SlotStatus.Bits.CommandCompleted) {
      break;
    }
    IioStall (1000); //1ms delay
    LoopCount++;
  }

  //
  // HSD 4166825: HSX: Polarity for PCIe Slot Power is Wrong When HotPlug is Enabled
  //
  // Check PCIE slot status
  //
  PCIeCapOffset = IioPCIeGetCapOffset (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                       EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (PCIeCapOffset == 0) {

    ASSERT (PCIeCapOffset != 0);

  } else {

    LoopCount = 0;
    SlotStatus.Uint16 = 0;
    while (LoopCount < 50) {
      SlotStatus.Uint16 = IioPciExpressRead16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
        PCIeCapOffset + R_PCIE_SLSTS_OFFSET);
      if (SlotStatus.Bits.PresenceDetectChanged) {
        break;
      }
      IioStall (SLOT_POWER_DELAY); //1ms delay
      LoopCount++;
    }

    if (SlotStatus.Bits.PresenceDetectChanged == 1) {
      if (SlotStatus.Bits.MrlSensor == 0) {
        //
        // Enable Power to Slot: PWRCTRL on SLOTCTRL, turn PWD indicator ON
        //
        IIO_D_PCIDBG ("Enabling PCIE Skt:%d Port:%d Bus:%x Dev:%d Func:%d Slot Power...\n",
          IioIndex, PortIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function);

        SlotControl.Uint16 = IioPciExpressRead16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
          PCIeCapOffset + R_PCIE_SLCTL_OFFSET);
        if (IsVMDEnabledForPort (IioIndex, PortIndex)) {
          SlotControl.Bits.PowerIndicator = 3;           // Indicator OFF (2 bit field)
          SlotControl.Bits.AttentionIndicator = 3;       // Attention Indicator OFF (2 bit field)
        } else {
          SlotControl.Bits.PowerIndicator = 1;           // Indicator ON (2 bit field)
        }
        SlotControl.Bits.PowerController = 0;          // Power ON
        IioPciExpressWrite16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
          PCIeCapOffset + R_PCIE_SLCTL_OFFSET, SlotControl.Uint16);
      } else {
        //
        // Card Present and MRL Open - Error Condition
        // Just Blink Attn Indicator without powering card
        //
        IIO_D_PCIDBG ("Disabling PCIE Skt:%d Port:%d Bus:%x Dev:%d Func:%d Slot Power...\n",
          IioIndex, PortIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function);

        if (IsVMDEnabledForPort (IioIndex, PortIndex)) {
          SlotControl.Bits.PowerIndicator = 3;           // Indicator OFF (2 bit field)
          SlotControl.Bits.AttentionIndicator = 3;       // Attention Indicator OFF (2 bit field)
        } else {
          SlotControl.Bits.PowerIndicator = 3;           // Indicator OFF (2 bit field)
          SlotControl.Bits.AttentionIndicator = 2;       // Attention Indicator BLINK (2 bit field)
        }
        SlotControl.Bits.PowerController = 1;          // Power OFF
        IioPciExpressWrite16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
          PCIeCapOffset + R_PCIE_SLCTL_OFFSET, SlotControl.Uint16);
      }
    } else {
      //
      // Empty slot
      // Remove Power to Slot: PWRCTRL on SLOTCTRL
      //
      IIO_D_PCIDBG ("Disabling PCIE Skt:%d Port:%d Bus:%x Dev:%d Func:%d Slot Power...\n",
        IioIndex, PortIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function);
      SlotControl.Uint16 = IioPciExpressRead16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
        PCIeCapOffset + R_PCIE_SLCTL_OFFSET);

      SlotControl.Bits.PowerIndicator = 3;           // Indicator OFF (2 bit field)
      SlotControl.Bits.PowerController = 1;          // Power OFF
      if (SlotStatus.Bits.MrlSensor == 0){
          SlotControl.Bits.AttentionIndicator = 3;     // Attention Indicator OFF (2 bit field)
      } else if (IsVMDEnabledForPort (IioIndex, PortIndex)){
          SlotControl.Bits.AttentionIndicator = 3;       // Attention Indicator OFF (2 bit field)
      }
      IioPciExpressWrite16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
        PCIeCapOffset + R_PCIE_SLCTL_OFFSET, SlotControl.Uint16);
    }

    //
    // wait on slot control command to complete (max. 100ms)
    //
    LoopCount = 0;
    while (LoopCount < 100) {
      SlotStatus.Uint16 = IioPciExpressRead16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
        PCIeCapOffset + R_PCIE_SLSTS_OFFSET);
      if (SlotStatus.Bits.CommandCompleted) {
        break;
      }
      IioStall (1000); //1ms delay
      LoopCount++;
    }

    //
    // Clear all the pending status
    //
    SlotStatus.Uint16 = IioPciExpressRead16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
      PCIeCapOffset + R_PCIE_SLSTS_OFFSET);
    SlotStatus.Bits.CommandCompleted = 1;
    SlotStatus.Bits.PresenceDetectChanged = 1;
    SlotStatus.Bits.MrlSensorChanged = 1;
    SlotStatus.Bits.PowerFaultDetected = 1;
    SlotStatus.Bits.AttentionButtonPressed = 1;
    IioPciExpressWrite16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
      PCIeCapOffset + R_PCIE_SLSTS_OFFSET, SlotStatus.Uint16);
    //
    // Enable all Hot Plug Interrupts regardless of card presence
    // except Hot Plug Interrupt HPINTEN bit-5 reg 84
    //
    SlotControl.Uint16 = IioPciExpressRead16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
      PCIeCapOffset + R_PCIE_SLCTL_OFFSET);
    SlotControl.Bits.HotPlugInterrupt = 0;
    SlotControl.Bits.CommandCompletedInterrupt = 0;
    SlotControl.Bits.PresenceDetectChanged = 1;
    SlotControl.Bits.MrlSensorChanged = 1;
    SlotControl.Bits.PowerFaultDetected = 1;
    SlotControl.Bits.AttentionButtonPressed = 1;
    IioPciExpressWrite16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
      PCIeCapOffset + R_PCIE_SLCTL_OFFSET, SlotControl.Uint16);
  }
}


/**
    PCIe Interrupt pin init.

    @param IioGlobalData - Pointer to IIO globals.
    @param IioIndex      - Socket number of the PCIe device being initialized.
    @param PortIndex     - Port number of the PCIe device being initialized.
**/
VOID
IntPinInit (
  IN  IIO_GLOBALS   *IioGlobalData,
  IN  UINT8         IioIndex,
  IN  UINT8         PortIndex
  )
{
  UINT8                           Bus;
  UINT8                           Device;
  UINT8                           Function;
  UINT16                          PciCmd;
  UINT8                           IntPin;

  Bus = GetSocketPortBusNumber (IioIndex, PortIndex);
  Device =  GetPciePortDevice (PortIndex);
  Function = GetPciePortFunction (PortIndex);
  // Disable interrupt
  PciCmd= IioPciExpressRead16 (IioIndex, Bus, Device, Function, PCI_COMMAND_OFFSET);
  IioPciExpressWrite16 (IioIndex, Bus, Device, Function, PCI_COMMAND_OFFSET, (PciCmd | BIT10));
  // IAFW must not program the INTP (Interrupt Pin) Cfg Space registers to any value other than INTA.
  IntPin = INTP_INTA;
  IioPciExpressWrite8 (IioIndex, Bus, Device, Function, PCI_INT_PIN_OFFSET, IntPin);

}

/**
  PCIe device initialization.

  @param[in] IioGlobalData  Pointer to IIO globals.
  @param[in] IioIndex       Socket number of the PCIe device being initialized.
  @param[in] PortIndex      Port number of the PCIe device being initialized.
**/
VOID
PcieDeviceInit (
  IN  IIO_GLOBALS   *IioGlobalData,
  IN  UINT8         IioIndex,
  IN  UINT8         PortIndex
  )
{
  UINT8                           *Array;
  UINT8                           MaxPortNumberPerSocket;
  PCI_REG_PCIE_DEVICE_CONTROL     DevCtrl;
  UINT8                           PCIeCapOffset;
  IIO_CONFIG                      *SetupData;
  PCI_ROOT_BRIDGE_PCI_ADDRESS     PciAddress;
  PCI_REG_PCIE_DEVICE_CAPABILITY  DevCap;
  BOOLEAN                         VmdEnabledPort;

  SetupData = &IioGlobalData->SetupData;
  GetPciePortBusDeviceFunction (IioIndex, PortIndex, &PciAddress);
  IIO_D_PCIDBG ("PcieDeviceInit:%d Port:%d Bus:%x Dev:%d Func:%d Slot Power...\n",
    IioIndex, PortIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function);

  PCIeCapOffset = IioPCIeGetCapOffset (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                       EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (PCIeCapOffset == 0) {

    ASSERT (PCIeCapOffset != 0);

  } else {
    MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);
    VmdEnabledPort         = IsVMDEnabledForPort (IioIndex, PortIndex);
    //
    // Skip MPS settings for VMD ports because this needs to be also set for endpoints in DXE,
    // but then the endpoints are hidden behind VMD device
    //
    if (!VmdEnabledPort) {
      //
      // Change Max Payload Size Supported field in root port Device Capabilities,
      // leave HW default if set to AUTO, otherwise, set MPSS to user-specified size.
      //
      if (SetupData->IioPcieConfig.PcieMaxPayload[(IioIndex * MaxPortNumberPerSocket) + PortIndex] != PCIE_MAX_PAYLOAD_AUTO) {
        DevCap.Uint32 = IioPciExpressRead32 (
                          IioIndex,
                          PciAddress.Bus,
                          PciAddress.Device,
                          PciAddress.Function,
                          PCIeCapOffset + R_PCIE_DCAP_OFFSET
                          );
        DevCap.Bits.MaxPayloadSize = SetupData->IioPcieConfig.PcieMaxPayload[(IioIndex * MaxPortNumberPerSocket) + PortIndex];
        IioPciExpressWrite32 (
          IioIndex,
          PciAddress.Bus,
          PciAddress.Device,
          PciAddress.Function,
          PCIeCapOffset + R_PCIE_DCAP_OFFSET,
          DevCap.Uint32
          );
      }
    }

    DevCtrl.Uint16 = IioPciExpressRead16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                          PCIeCapOffset + R_PCIE_DCTL_OFFSET);
    Array = &(SetupData->IioPcieConfig.PcieUnsupportedRequests[0]);
    if (Array[((IioIndex)*MaxPortNumberPerSocket)+PortIndex]) {
      DevCtrl.Bits.UnsupportedRequest = 1;
    } else {
      DevCtrl.Bits.UnsupportedRequest = 0;
    }

    //
    // Skip MRRS settings for VMD ports because this needs to be also set for endpoints in DXE,
    // but then the endpoints are hidden behind VMD device
    //
    if (!VmdEnabledPort) {
      if (SetupData->PcieMaxReadRequestSize != PCIE_LIB_MRRS_VALUE_AUTO) {
        DevCtrl.Bits.MaxReadRequestSize = SetupData->PcieMaxReadRequestSize;
      } else {
        DevCtrl.Bits.MaxReadRequestSize = PCIE_LIB_MRRS_4096B;
      }
    }

    //s3427199: register name below was incorrect - start
    IioPciExpressWrite16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
      PCIeCapOffset + R_PCIE_DCTL_OFFSET, DevCtrl.Uint16);
    //s3427199 - end
    IntPinInit(IioGlobalData, IioIndex, PortIndex);
  }
}

/**
  Match SupportedSpeedsVector with setup configuration and return max speed.

  @param[in]  IioGlobalData - Pointer to IIO globals.
  @param[in]  IioIndex      - Socket number of the port being initialized.
  @param[in]  PortIndex     - Port number being initialized.

  @return Link speed is returned, 1 for gen1, 2 for gen2, and so on.
**/
UINT8
GetPciePortLinkSpeed (
  IN  IIO_GLOBALS *IioGlobalData,
  IN  UINT8        IioIndex,
  IN  UINT8        PortIndex,
  IN  UINT8        SupportedSpeedsVector
  )
{
  UINT8            MaxPortNumberPerSocket;
  UINT8            Speed;

  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);
  switch (IioGlobalData->SetupData.IioPcieConfig.PciePortLinkSpeed[IioIndex * MaxPortNumberPerSocket + PortIndex]) {
    case PCIE_LINK_SPEED_GEN1:
      SupportedSpeedsVector &= BIT0;
    case PCIE_LINK_SPEED_GEN2:
      SupportedSpeedsVector &= BIT0 | BIT1;
    case PCIE_LINK_SPEED_GEN3:
      SupportedSpeedsVector &= BIT0 | BIT1 | BIT2;
    case PCIE_LINK_SPEED_GEN4:
      SupportedSpeedsVector &= BIT0 | BIT1 | BIT2 | BIT3;
    case PCIE_LINK_SPEED_GEN5:
      SupportedSpeedsVector &= BIT0 | BIT1 | BIT2 | BIT3 | BIT4;
    case PCIE_LINK_SPEED_AUTO: // For AUTO we leave HW defaults
      break;
    default:
      IIO_D_PCIERR ("[%d p%d] Unsupported PCIe link speed gen%d in setup knob - ignore\n", IioIndex, PortIndex,
                    IioGlobalData->SetupData.IioPcieConfig.PciePortLinkSpeed[IioIndex * MaxPortNumberPerSocket + PortIndex]);
      break;
  }
  for (Speed = 0; SupportedSpeedsVector != 0; Speed++) {
    SupportedSpeedsVector >>= 1;
  }
  return Speed;
}


/**
    Configure Link Capability register.

    @param[in] IioGlobalData - Pointer to IIO globals.
    @param[in] IioIndex      - Socket number of the port being initialized.
    @param[in] PortIndex     - Port number being initialized.
    @param[in] PCIeCapOffset - Capabilitiy offset in PCI device config space.
**/
VOID
LinkCapInit (
  IN  IIO_GLOBALS  *IioGlobalData,
  IN  UINT8         IioIndex,
  IN  UINT8         PortIndex,
  IN  UINT8         PCIeCapOffset
  )
{
  PCI_REG_PCIE_LINK_CAPABILITY    LnkCap;
  IIO_CONFIG                      *SetupData;
  IIO_OUT_DATA                    *IioOutData;
  PCI_ROOT_BRIDGE_PCI_ADDRESS     PciAddress;
  UINT32                          RegBkp1;
  UINT32                          RegBkp2;
  UINT8                           MaxPortNumberPerSocket;
  UINT8                           Aspm;
  EFI_STATUS                      Status;

  SetupData = &IioGlobalData->SetupData;
  IioOutData = &IioGlobalData->IioVar.IioOutData;

  GetPciePortBusDeviceFunction (IioIndex, PortIndex, &PciAddress);
  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);

  Status = GetPcieCapReg32 (IioIndex, PortIndex, R_PCIE_LCAP_OFFSET, &LnkCap.Uint32);
    if (EFI_ERROR (Status)) {
      IIO_D_PCIERR ("[%d p%d] %a: Can't read R_PCIE_LCAP_OFFSET register!\n",
        IioIndex, PortIndex, __FUNCTION__);
      return;
    }
  RegBkp1 = LnkCap.Uint32;

  //
  // Port# (31:24)
  //
  LnkCap.Bits.PortNumber = 0;                                               // clear bits 31:24
  LnkCap.Bits.PortNumber = PortIndex;

  //
  // HSD: 5332673 [SKX_P] FPGA does not support L1 for PCIe. Only L0 supported.
  //
  if (CheckFPGAPort (IioGlobalData, IioIndex, PortIndex) == FALSE) {
    //
    // L1 exit Latecy (17:15)
    //
    LnkCap.Bits.L1ExitLatency = SetupData->PcieL1Latency[((IioIndex)*MaxPortNumberPerSocket)+PortIndex];
  }
  //
  // Set ASPM on value requested in setup
  //
  Aspm =  GetAspmSettingForPort (IioGlobalData, IioIndex, PortIndex);
  if (Aspm != PCIE_ASPM_AUTO) {
    LnkCap.Bits.Aspm = Aspm;
  }

  // If Override Max Link Width was not specified in MaxPXPMap, then copy CurrentPXPMap (bifurcated max width) to MaxPXPMap (override max width)
  if (IioOutData->MaxPXPMap[(IioIndex*MaxPortNumberPerSocket)+PortIndex] == 0) {
    IioOutData->MaxPXPMap[(IioIndex*MaxPortNumberPerSocket)+PortIndex] = IioOutData->CurrentPXPMap[(IioIndex*MaxPortNumberPerSocket)+PortIndex];
  }
  RegBkp2 = LnkCap.Uint32;

  Status = SetPcieCapReg32 (IioGlobalData, IioIndex, PortIndex, R_PCIE_LCAP_OFFSET, LnkCap.Uint32);
  if (EFI_ERROR (Status)) {
    IIO_D_PCIERR ("[%d p%d] %a: Can't write R_PCIE_LCAP_OFFSET register!\n",
      IioIndex, PortIndex, __FUNCTION__);
    return;
  }

  Status = GetPcieCapReg32 (IioIndex, PortIndex, R_PCIE_LCAP_OFFSET, &LnkCap.Uint32);
  IIO_D_PCIDBG ("[%d p%d] R_PCIE_LCAP %04X -> %04X -> %04X\n", IioIndex, PortIndex,
                RegBkp1, RegBkp2, LnkCap.Uint32);
   }


/**
  Configure Link Capability 2 register.

  Since gen4 & gen5 controller supported speeds vector is RO this
  function is empty for system that use gen4/gen5 controller.

  @param[in] IioGlobalData - Pointer to IIO globals.
  @param[in] IioIndex      - Socket number of the port being initialized.
  @param[in] PortIndex     - Port number being initialized.
  @param[in] PCIeCapOffset - Capabilitiy offset in PCI device config space.
**/
VOID
LinkCap2Init (
  IN  IIO_GLOBALS                *IioGlobalData,
  IN  UINT8                       IioIndex,
  IN  UINT8                       PortIndex,
  IN  UINT8                       PCIeCapOffset
  )
{
}


/**
  Configure Link Control 2 register.

  @param[in] IioGlobalData - Pointer to IIO globals.
  @param[in] IioIndex      - Socket number of the port being initialized.
  @param[in] PortIndex     - Port number being initialized.
  @param[in] PCIeCapOffset - Capabilitiy offset in PCI device config space.
**/
VOID
LinkCtl2Init (
  IN  IIO_GLOBALS                *IioGlobalData,
  IN  UINT8                       IioIndex,
  IN  UINT8                       PortIndex,
  IN  UINT8                       PCIeCapOffset
  )
{
  PCI_REG_PCIE_LINK_CAPABILITY2   LnkCap2;
  PCI_REG_PCIE_LINK_CONTROL2      LnkCtl2;
  PCI_ROOT_BRIDGE_PCI_ADDRESS     PciAddress;
  UINT16                          RegBkp1;
  UINT16                          RegBkp2;
  UINT8                           MaxPortNumberPerSocket;
  EFI_STATUS                      Status;

  GetPciePortBusDeviceFunction (IioIndex, PortIndex, &PciAddress);
  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);

  Status = GetPcieCapReg32 (IioIndex, PortIndex, R_PCIE_LCAP2_OFFSET, &LnkCap2.Uint32);
  if (EFI_ERROR (Status)) {
    IIO_D_PCIERR ("[%d p%d] %a: Can't read R_PCIE_LCAP2_OFFSET register!\n",
      IioIndex, PortIndex, __FUNCTION__);
    return;
  }

  Status = GetPcieCapReg16 (IioIndex, PortIndex, R_PCIE_LCTL2_OFFSET, &LnkCtl2.Uint16);
if (EFI_ERROR (Status)) {
  IIO_D_PCIERR ("[%d p%d] %a: Can't read R_PCIE_LCTL2_OFFSET register!\n",
    IioIndex, PortIndex, __FUNCTION__);
  return;
}

  RegBkp1 = LnkCtl2.Uint16;
  LnkCtl2.Bits.EnterCompliance = 0;
  LnkCtl2.Bits.SelectableDeemphasis = 0;
  LnkCtl2.Bits.TargetLinkSpeed = GetPciePortLinkSpeed (IioGlobalData, IioIndex, PortIndex,
                                                       (UINT8)LnkCap2.Bits.LinkSpeedsVector);
  // Set Compliance Mode
  if (IioGlobalData->SetupData.IioPcieConfig.ComplianceMode[((IioIndex)*MaxPortNumberPerSocket) + PortIndex]) {
    LnkCtl2.Bits.EnterCompliance = 1;
  }
  //
  // Program De-Emphasis select (bit 6)
  //
  if (IioGlobalData->SetupData.DeEmphasis[((IioIndex)*MaxPortNumberPerSocket)+PortIndex]) {
    LnkCtl2.Bits.SelectableDeemphasis = 1;
  }
  //
  // Update PCI Express Link Control Register 2
  //
  RegBkp2 = LnkCtl2.Uint16;
  Status = SetPcieCapReg16 (IioGlobalData, IioIndex, PortIndex, R_PCIE_LCTL2_OFFSET, LnkCtl2.Uint16);
  if (EFI_ERROR (Status)) {
    IIO_D_PCIERR ("[%d p%d] %a: Can't write R_PCIE_LCTL2_OFFSET register!\n",
      IioIndex, PortIndex, __FUNCTION__);
    return;
  }

  Status = GetPcieCapReg16 (IioIndex, PortIndex, R_PCIE_LCTL2_OFFSET, &LnkCtl2.Uint16);
  IIO_D_PCIDBG ("[%d p%d] R_PCIE_LCAP2 %04X   R_PCIE_LCTL2 %02X -> %02X -> %02X\n", IioIndex, PortIndex,
                LnkCap2.Uint32, RegBkp1, RegBkp2, LnkCtl2.Uint16);
}


/**
  Configure Link Control 2 register.

  @param[in] IioGlobalData - Pointer to IIO globals.
  @param[in] IioIndex      - Socket number of the port being initialized.
  @param[in] PortIndex     - Port number being initialized.
  @param[in] PCIeCapOffset - Capabilitiy offset in PCI device config space.
**/
VOID
LinkCtlInit (
  IN  IIO_GLOBALS   *IioGlobalData,
  IN  UINT8         IioIndex,
  IN  UINT8         PortIndex,
  IN  UINT8         PCIeCapOffset
  )
{
  PCI_REG_PCIE_LINK_CONTROL       LnkCtl;
  BOOLEAN                         DisableLink;
  UINT8                           MaxPortNumberPerSocket;
  IIO_CONFIG                      *SetupData;
  PCI_ROOT_BRIDGE_PCI_ADDRESS     PciAddress;
  UINT32                          DWord;

  SetupData = &IioGlobalData->SetupData;
  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);

  GetPciePortBusDeviceFunction (IioIndex, PortIndex, &PciAddress);
  if (CheckVarPortEnable (IioGlobalData, IioIndex, PortIndex) == PCIE_PORT_DISABLE) {
    DisableLink = TRUE;
  } else {
    DisableLink = FALSE;
  }
  //
  // User need to disable link
  //
  if (SetupData->PcieLinkDis[((IioIndex)*MaxPortNumberPerSocket)+PortIndex]) {
    DisableLink = TRUE;
  }
  LnkCtl.Uint16 = IioPciExpressRead16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                       PCIeCapOffset + R_PCIE_LCTL_OFFSET);
  DWord = LnkCtl.Uint16;
  if (DisableLink) {
    LnkCtl.Bits.LinkDisable = 1;
  }
  if (SetupData->IioPcieConfig.ExtendedSync[((IioIndex)*MaxPortNumberPerSocket)+PortIndex]) {
    LnkCtl.Bits.ExtendedSynch = 1;
  }
  IioPciExpressWrite16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                        PCIeCapOffset + R_PCIE_LCTL_OFFSET, LnkCtl.Uint16);
  IIO_D_PCIDBG ("[%d p%d] R_PCIE_LCTL %X -> %X -> %X\n", IioIndex, PortIndex, DWord, LnkCtl.Uint16,
                IioPciExpressRead16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                     PCIeCapOffset + R_PCIE_LCTL_OFFSET));
}


/**
    PCIe Link initialization.

    @param IioGlobalData - Pointer to IIO globals.:
    @param IioIndex      - Socket number of the port being initialized.
    @param PortIndex     - Port number being initialized.
**/
VOID
PcieLinkInit (
  IN  IIO_GLOBALS                 *IioGlobalData,
  IN  UINT8                       IioIndex,
  IN  UINT8                       PortIndex
  )
{
  UINT8                           PCIeCapOffset;
  UINT8                           StackIndex = 0;
  PCI_ROOT_BRIDGE_PCI_ADDRESS     PciAddress;
  EFI_STATUS                      Status;

  GetPciePortBusDeviceFunction (IioIndex, PortIndex, &PciAddress);

  Status = GetPcieStackPerPort (IioIndex, PortIndex, &StackIndex);
  if (Status != EFI_SUCCESS) {
    return;
  }

  IIO_D_PCIDBG ("[%d.%d p%d] %02X:%02X:%02X.%X: Initialize PCIe port...\n",
                IioIndex, StackIndex, PortIndex,
                IioGlobalData->IioVar.IioVData.SegmentSocket[IioIndex],
                PciAddress.Bus, PciAddress.Device, PciAddress.Function);

  IioSrisEnable (IioGlobalData, IioIndex, PortIndex);

  PCIeCapOffset = IioPCIeGetCapOffset (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function, EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (PCIeCapOffset == 0) {
    ASSERT (PCIeCapOffset != 0);
  } else {

    LinkCapInit (IioGlobalData, IioIndex, PortIndex, PCIeCapOffset);
    LinkCap2Init (IioGlobalData, IioIndex, PortIndex, PCIeCapOffset);
    LinkCtlInit (IioGlobalData, IioIndex, PortIndex, PCIeCapOffset);
    LinkCtl2Init (IioGlobalData, IioIndex, PortIndex, PCIeCapOffset);
  }
}

/**

    PCIe PerfCtrlSts1 register initialization

    @param IioGlobalData - Pointer to IIO globals.
    @param IioIndex      - Socket number
    @param PortIndex     - Port number
**/
VOID
PciePerfCtrlStsInit (
  IN  IIO_GLOBALS    *IioGlobalData,
  IN  UINT8          IioIndex,
  IN  UINT8          PortIndex
  )
{
  return;
}

/**
    PCIe miscelaneous initializations.

    @param IioGlobalData - Pointer to global Data
    @param IioIndex      - Socket number
    @param PortIndex     - Port number.
**/
VOID
PcieMiscInit (
  IN  IIO_GLOBALS   *IioGlobalData,
  IN  UINT8         IioIndex,
  IN  UINT8         PortIndex
  )
{
  XPTDEF6_IIO_PCIE_G5_STRUCT               XptDef6;
  XPTDEF2_IIO_PCIE_G5_STRUCT               XptDef2;
  UINT8                                    MaxPortNumberPerSocket;
  IIO_CONFIG                               *SetupData;
  MCSTCAP_IIO_PCIE_G5_STRUCT               McsCapGen5;
  CAPSR_IIO_PCIE_G5_STRUCT                 CAPSRG5;

  SetupData = &IioGlobalData->SetupData;
  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);

  ProgramMsiMsgCtrl (IioIndex, PortIndex, SetupData->MsiEn[IioIndex * MaxPortNumberPerSocket + PortIndex]);

  PciePerfCtrlStsInit (IioGlobalData, IioIndex, PortIndex);
  //
  // Program Root Port Error Escalation enable/disable
  //
  RpErrEscalationProg (
    IioIndex, PortIndex,
    SetupData->IioPcieConfig.RpCorrectableErrorEsc[IioIndex],
    SetupData->IioPcieConfig.RpUncorrectableNonFatalErrorEsc[IioIndex],
    SetupData->IioPcieConfig.RpUncorrectableFatalErrorEsc[IioIndex]
    );

  //
  //1507929166: Program CAPSR to enable support for PCIE ECRC
  //
  CAPSRG5.Data = IioReadCpuCsr32 (IioIndex, PortIndex, CAPSR_IIO_PCIE_G5_REG);
  CAPSRG5.Bits.aercapctl_egc = 1;
  CAPSRG5.Bits.aercapctl_ecc = 1;
  IioWriteCpuCsr32 (IioIndex, PortIndex, CAPSR_IIO_PCIE_G5_REG, CAPSRG5.Data);

  //
  //1306935767: Set capabilities to match HW support : AER ECRC and Multicast Capability Register
  //
  McsCapGen5.Data = IioReadCpuCsr16 (IioIndex, PortIndex, MCSTCAP_IIO_PCIE_G5_REG);
  McsCapGen5.Bits.mcers = 1;
  IioWriteCpuCsr16 (IioIndex, PortIndex, MCSTCAP_IIO_PCIE_G5_REG, McsCapGen5.Data);

  // Enable/Disable support for Enqueue requests on PCIe
  XptDef6.Data = IioReadCpuCsr16 (IioIndex, PortIndex, XPTDEF6_IIO_PCIE_G5_REG);
  if (SetupData->PcieEnqCmdSupport) {
    XptDef6.Bits.enqd = 0;
  } else {
    XptDef6.Bits.enqd = 1;
  }
  IioWriteCpuCsr16 (IioIndex, PortIndex, XPTDEF6_IIO_PCIE_G5_REG, XptDef6.Data);

  // Enable the command/data parity check
  XptDef2.Data = IioReadCpuCsr32 (IioIndex, PortIndex, XPTDEF2_IIO_PCIE_G5_REG);
  XptDef2.Bits.tcpar_chk_dis = 0;
  XptDef2.Bits.tdpar_chk_dis = 0;
  IioWriteCpuCsr32 (IioIndex, PortIndex, XPTDEF2_IIO_PCIE_G5_REG, XptDef2.Data);
}

/**
  PCI Pre initialization procedures.

  @param[in] IioGlobalData - Pointer to global data structure
  @param[in] IioIndex      - Socket number being initialized
  @param[in] PortIndex     - Port number being initialized
**/
VOID
PciePreInit (
  IN  IIO_GLOBALS                      *IioGlobalData,
  IN  UINT8                             IioIndex,
  IN  UINT8                             PortIndex
  )
{
  //
  // Set/reset any RWO features of the PCIe port capability register here, which
  // can be used to initialize any features on the PCIe port control registers later
  //
  PCI_REG_PCIE_DEVICE_CAPABILITY        DevCap;
  PCI_REG_PCIE_DEVICE_CAPABILITY2       DevCap2;
  PCI_REG_PCIE_DEVICE_CONTROL2          DevCtl2;
  UINT8                                 PCIeCapOffset;
  PCI_ROOT_BRIDGE_PCI_ADDRESS           PciAddress;

  GetPciePortBusDeviceFunction (IioIndex, PortIndex, &PciAddress);
  IIO_D_PCIDBG ("PciePreInit:%d Port:%d Bus:%x Dev:%d Func:%d \n",
    IioIndex, PortIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function);

  PCIeCapOffset = IioPCIeGetCapOffset (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
    EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (PCIeCapOffset == 0) {

    ASSERT (PCIeCapOffset != 0);

  } else {
    //
    // SKX eco[300732] Incorrect default values are loaded by hardware for devcap::extended_tag_field_supported and rootcap7::crs_software_visibility
    // 4928293: BIOS workaround to write the value 1b1 to devcap dev0 fun0 bit 5 extended tag field supported
    // only if IIO ports in P-stacks that are in PCIE mode
    //
    if (!CheckNtbPortConfSetting (IioIndex, PortIndex)) {
      DevCap.Uint32 = IioPciExpressRead32 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
        PCIeCapOffset + R_PCIE_DCAP_OFFSET);
      DevCap.Bits.ExtendedTagField = 1;
      IioPciExpressWrite32 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
        PCIeCapOffset + R_PCIE_DCAP_OFFSET, DevCap.Uint32);
    }
    DevCap2.Uint32 = IioPciExpressRead32 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
      PCIeCapOffset + R_PCIE_DCAP2_OFFSET);
    DevCtl2.Uint16 = IioPciExpressRead16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
      PCIeCapOffset + R_PCIE_DCTL2_OFFSET);
    //
    // Set LtrMechanism capability and control2 bits when PCIe Latency Tolerance Reporting is enabled
    //
    if (IioGlobalData->SetupData.IioPcieConfig.PCIe_LTR == PCIE_LTR_ENABLE_YES) {
      DevCap2.Bits.LtrMechanism = 1;
      DevCtl2.Bits.LtrMechanism = 1;

      IioPciExpressWrite16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
        PCIeCapOffset + R_PCIE_DCTL2_OFFSET, DevCtl2.Uint16);
    }
    //
    // Set TenBitTagRequesterSupported and TenBitTagCompleterSupported bit if Setup knob PCIe 10-bit Tag Enable is enable.
    //

      if (IioGlobalData->SetupData.Pcie10bitTag) {
        DevCap2.Bits.TenBitTagCompleterSupported = 1;
        DevCap2.Bits.TenBitTagRequesterSupported = 1;
      }
    //
    // Set the TPH completer supported capability
    //
    DevCap2.Bits.TphCompleter = 1;
    //
    // Set AtomicOp support capability
    //
    DevCap2.Bits.AtomicOp32Completer = 1;
    DevCap2.Bits.AtomicOp64Completer = 1;
    DevCap2.Bits.Cas128Completer = 1;
    //
    //  AtomicOpRouting Supported by GEN4 and GEN5 controller. For GEN3 its RO.
    //
    DevCap2.Bits.AtomicOpRouting = 1;

    IioPciExpressWrite32 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
      PCIeCapOffset + R_PCIE_DCAP2_OFFSET, DevCap2.Uint32);
  }
}

/**

    Post initialization methods for PCIe

    @param IioGlobalData - Pointer to IIO Globals.
    @param IioIndex      - Socket of the initialized port.
    @param PortIndex     - Number of the initialized port.
**/
VOID
PciePostInit (
  IN  IIO_GLOBALS                 *IioGlobalData,
  IN  UINT8                       IioIndex,
  IN  UINT8                       PortIndex
  )
{
  UINT16                          PortWidth;
  BOOLEAN                         PortLinked;
  UINT8                           PortSpeed;
  UINT8                           Stack = 0;
  UINT8                           NtbPortIndex;
  UINT8                           MaxPortNumberPerSocket;
  PCICMD_IIO_PCIE_G5_STRUCT       PciCmd;
  LINKSTS_IIO_PCIE_G5_STRUCT      LnkSts;
  EXPCAP_IIO_PCIE_G5_STRUCT       Pxpcap;
  SVID_IIO_PCIENTB_STRUCT         Svid_pcientb;
  PCI_REG_PCIE_SLOT_CAPABILITY    SltCap;
  PCI_REG_PCIE_LINK_CAPABILITY    LnkCap;
  PCI_REG_PCIE_DEVICE_CONTROL     DevCtrl;
  UINT8                           PCIeCapOffset;
  IIO_CONFIG                      *SetupData;
  IIO_OUT_DATA                    *IioOutData;
  PCI_ROOT_BRIDGE_PCI_ADDRESS     PciAddress;
  UINT8                           PcieRevId;
  EFI_STATUS                      Status;

  SetupData = &IioGlobalData->SetupData;
  IioOutData = &IioGlobalData->IioVar.IioOutData;

  GetPciePortBusDeviceFunction (IioIndex, PortIndex, &PciAddress);
  IIO_D_PCIDBG ("PciePostInit: %d Port:%d Bus:%x Dev:%d Func:%d Slot Power...\n",
    IioIndex, PortIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function);

  PCIeCapOffset = IioPCIeGetCapOffset (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
    EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (PCIeCapOffset == 0) {
    ASSERT (PCIeCapOffset != 0);
    return;
  }

  //
  // skip DMI port
  //
  if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPortActive)) {
    return;
  }

  Status = GetPcieStackPerPort (IioIndex, PortIndex, &Stack);
  if (Status != EFI_SUCCESS) {
    return;
  }

  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);
  if (((IioIndex * MaxPortNumberPerSocket) + PortIndex) >= MAX_TOTAL_PORTS) {
    return;
  }

  //
  // Command register has to be set after PCI bus enumeration
  // PCI bus driver will change the command register during the enumeration
  //
  PciCmd.Data = IioReadCpuCsr16 (IioIndex, PortIndex, PCICMD_IIO_PCIE_G5_REG);

  if (SetupData->Serr) {
    PciCmd.Bits.see = 1;
  }
  if (SetupData->Perr) {
    PciCmd.Bits.pere = 1;
  }

  if (SetupData->Perr) {
    PciCmd.Bits.pere = 1;
  }
  //
  // Program PCI SERR/PERR
  //
  DevCtrl.Uint16 = IioPciExpressRead16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
    PCIeCapOffset + R_PCIE_DCTL_OFFSET);

  IioPciExpressWrite16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
    PCIeCapOffset + R_PCIE_DCTL_OFFSET, DevCtrl.Uint16);
  IioWriteCpuCsr16 (IioIndex, PortIndex, PCICMD_IIO_PCIE_G5_REG, PciCmd.Data);

  IioPciExpressWrite16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
    PCIeCapOffset + R_PCIE_DCTL_OFFSET, DevCtrl.Uint16);
  IioWriteCpuCsr16 (IioIndex, PortIndex, PCICMD_IIO_PCIE_G5_REG, PciCmd.Data);

  //
  // Update for PCI Express Port 1-10 (PciAddress.Device)
  //
  LnkSts.Data = IioPciExpressRead16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
    PCIeCapOffset + R_PCIE_LSTS_OFFSET);

  SltCap.Uint32 = IioPciExpressRead32 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
    PCIeCapOffset + R_PCIE_SLCAP_OFFSET);
  IioPciExpressWrite32 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
    PCIeCapOffset + R_PCIE_SLCAP_OFFSET, SltCap.Uint32);
  LnkCap.Uint32 = IioPciExpressRead32 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
    PCIeCapOffset + R_PCIE_LCAP_OFFSET);
  IioPciExpressWrite32 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
    PCIeCapOffset + R_PCIE_LCAP_OFFSET, LnkCap.Uint32);
  Pxpcap.Data = IioPciExpressRead16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
    PCIeCapOffset + R_PCIE_XCAP_OFFSET);
  IioPciExpressWrite16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
    PCIeCapOffset + R_PCIE_XCAP_OFFSET, Pxpcap.Data);

  // Gather and store data about linked ports.
  PortWidth = LnkSts.Bits.nlw;
  PortSpeed = (UINT8)LnkSts.Bits.cls;
  PortLinked = (LnkSts.Bits.dllla == 1);
  DEBUG ((DEBUG_INFO, "PCIe Port link: Width = %x, Speed = %x, Link active = %x\n",
    PortWidth, PortSpeed, PortLinked));

  //
  // Handle programming of RWO and RWOG registers for Iio Port functions
  //

  //
  // Lock Revision ID
  //
  PcieRevId = IioPciExpressRead8 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
      PCI_REVISION_ID_OFFSET);
  IioPciExpressWrite8 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function,
      PCI_REVISION_ID_OFFSET, PcieRevId);


    if (FeaturePcdGet (PcdLockCsrSsidSvidRegister)) {
      // check if this is NTB port
      if (CheckNtbPortConfSetting (IioIndex, PortIndex)) {

        Status = GetNtbPortPerPortIndex (PortIndex, &NtbPortIndex);
        if (Status != EFI_SUCCESS) {
          return;
        }
        Svid_pcientb.Data = IioReadCpuCsr16 (IioIndex, NtbPortIndex, SVID_IIO_PCIENTB_REG);
        IioWriteCpuCsr16 (IioIndex, NtbPortIndex, SVID_IIO_PCIENTB_REG, Svid_pcientb.Data);
      } else {
        // PCIe mode
        IioLockSvid (IioIndex, PortIndex);
      }
      IioLockSdid (IioIndex, PortIndex);
  }
  //
  // Lock PXPNXTPTR register
  //
  LockExpCapPtr (IioIndex, PortIndex);


  //
  // Clear Expected PCIe Errors (3612591, 3875328)
  // [permanent programming consideration for all steppings]
  PcieErrProg (IioIndex, PortIndex);

  LnkSts.Data = IioReadCpuCsr16 (IioIndex, PortIndex, LINKSTS_IIO_PCIE_G5_REG);
  LnkSts.Bits.labs = 1;
  LnkSts.Bits.lbms = 1;
  IioWriteCpuCsr16 (IioIndex, PortIndex, LINKSTS_IIO_PCIE_G5_REG, LnkSts.Data);

}

/**
    Per port initialization.

    @param Phase         - Initialization phase (pre, init, post).
    @param IioGlobalData - Pointer to IIO global data/methods
    @param IioIndex      - Current socket.
    @param PortIndex     - Port number being initialized.
**/
VOID
PciePortInit (
  IN  IIO_INIT_PHASE                 Phase,
  IN  IIO_GLOBALS                    *IioGlobalData,
  IN  UINT8                          IioIndex,
  IN  UINT8                          PortIndex
  )
{
  UINT8                              Var8=0;
  UINT8                              *Array;
  PCI_ROOT_BRIDGE_PCI_ADDRESS        PciAddress;
  UINT8                              MaxPortNumberPerSocket;
  IIO_CONFIG                         *SetupData;
  IIO_OUT_DATA                       *IioOutData;
  UINT16                             VendorID;
  SetupData = &IioGlobalData->SetupData;
  IioOutData = &IioGlobalData->IioVar.IioOutData;

  //
  // If the port designated for initialization is hidden, do not attempt to initialize
  //
  Array = &(SetupData->PEXPHIDE[0]);
  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);
  if (Array[(IioIndex*MaxPortNumberPerSocket)+PortIndex]) {
    return;
  }
  Var8 = IioOutData->CurrentPXPMap[(IioIndex*MaxPortNumberPerSocket)+PortIndex];

  if (Var8 == 0) {
    //
    // Put links into low power mode which are not connected to slot.
    //
    if ( Phase == IIOInitPhase1 ) {
      IIO_D_PCIDBG ("  Spare port --> [%s(%d)]", IIO_PORT_LABEL(PortIndex), PortIndex);
      // Forcing of unused ports to L1 (in case Hot-plug is feasible).
      // This may not be applicable if we consider the HP functionality.
      // For HP slots even if the card is connected when the system is powered up will show the link status bit as down until in phase 3
      // where the driver will power up the slots and retrains, until that point the endpoint device will not be visible in the slots
      // which are HP capable, and if we try to pull down the power level in phase 1, the cards connected to the HP slots will fail to train,
      // so this is not applicable as per the current flow
      //ForceUnusedPortsToL1 (PciAddress, IioGlobalData,0,Port);
    }
    return;
  } else {
    //
    // Only initialize the present port per the port map
    //
    PciAddress.Bus = GetSocketPortBusNumber (IioIndex, PortIndex);
    PciAddress.Device   =  GetPciePortDevice (PortIndex);
    PciAddress.Function = GetPciePortFunction (PortIndex);
    VendorID = (UINT16)IioPciExpressRead16 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function, PCI_VENDOR_ID_OFFSET);
    if ((VendorID == 0xFFFF) || (VendorID == 0)) {
      IIO_D_PCIWARN ("Device not present or accessible IIO[%x] PortIndex:%x\n", IioIndex, PortIndex);
      return;
    }
    switch (Phase) {
      case IIOInitPhase1:
        IIO_D_PCIDBG ("Initialize IIO PCIE Bus = %X Dev:%X Func:%X...\n", PciAddress.Bus, PciAddress.Device,PciAddress.Function);
        PciePreInit    (IioGlobalData, IioIndex, PortIndex);
        PcieDeviceInit (IioGlobalData, IioIndex, PortIndex);
        break;

      case IIOInitPhase2:
        PcieMiscInit   (IioGlobalData, IioIndex, PortIndex);
        break;

      case IIOInitPhase3:
        IIO_D_PCIDBG ("PciPostInit Bus:%X Dev:%X Func%X...\n", PciAddress.Bus, PciAddress.Device,PciAddress.Function);
        PciePostInit   (IioGlobalData, IioIndex, PortIndex);
        break;

      default:
        break;
    }
  }
}


/**

  IIO Port post initialization settings after the other IIO Port resources are assigned.

  @param IioGlobalData        Pointer to the Internal Data Storage (UDS) for this driver
  @param IioIndex             Socket Index
**/
VOID
PciePortPostInit (
  IN  IIO_GLOBALS                 *IioGlobalData,
  IN  UINT8                       IioIndex
  )
{
  UINT8                           PortIndex;
  UINT8                           NtbPortIndex;
  UINT8                           PowerState;
  UINT8                           *Array;
  PMCSR_IIO_PCIENTB_STRUCT        PmcsrNtb;
  UINT8                           MaxPortNumberPerSocket;
  IIO_CONFIG                      *SetupData;
  PCICMD_IIO_PCIEDMI_STRUCT       PciCmd;
  EFI_STATUS                      Status;


  SetupData = &IioGlobalData->SetupData;
  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);
  for (PortIndex = 0; PortIndex < MaxPortNumberPerSocket; PortIndex++) {
      if (!IioVData_ConfigurePciePort(IioGlobalData, IioIndex, PortIndex)) {
        continue;
      }

      IIO_D_PCIDBG ("PciePortPostInit IIO[%x], Port = %x\n", IioIndex, PortIndex);
      //
      // Set D0 or D3Hot state based on setup option
      //
      Array = &(SetupData->PcieDState[0]);
      if (Array[((IioIndex)*MaxPortNumberPerSocket)+PortIndex] == 1) {
        PowerState = V_PCIE_PORT_PXPPMCSR_PWR_STATE_D3HOT;
      } else {
        PowerState = V_PCIE_PORT_PXPPMCSR_PWR_STATE_D0;
      }

      //
      // 4927900: IIO: NTB setting change for acsctrl and pmcsr.power_state
      //
      // 2. if NTB, setup pmcsr.power_state after NTB or move pmcsr.power_state setting after NTB setup.
      // 4. Msinxtptr moved to after NTB
      // 5. PMCSR_IIOCB, PMCSR_IIO_IOAPIC, PMCSR_IIO_PCIE, all 3 should be programmed after NTB.

      // Check if this is NTB port
      if (CheckNtbPortConfSetting (IioIndex, PortIndex)){
         //
         // Update PMCSR and lock the register
         //
         Status = GetNtbPortPerPortIndex (PortIndex, &NtbPortIndex);
         if (Status != EFI_SUCCESS) {
           continue;
         }

         PmcsrNtb.Data = IioReadCpuCsr16 (IioIndex, NtbPortIndex, PMCSR_IIO_PCIENTB_REG);
         PmcsrNtb.Bits.ps = PowerState;
         IioWriteCpuCsr16 (IioIndex, NtbPortIndex, PMCSR_IIO_PCIENTB_REG, PmcsrNtb.Data);

      } else {
        // if PCIE Mode
        //
        // Update PMCSR and lock the register
        //
        SetPowerState (IioIndex, PortIndex, PowerState);

        //
        // Lock MSINXTPTR register
        //
        LockMsiCapPtr (IioIndex, PortIndex);
      }
  } // End PortIndex
  //
  // Disabling SAI policy
  //
  IioRestrictAccessPerSocket(IioGlobalData, IioIndex);
  //
  // DMI is in RCRBBAR allocated by KTI-RC.
  // Clear memory space enable bit for after DFX1 access is disabled.
  //
  PciCmd.Data = IioReadCpuCsr16 (IioIndex, DMI_PORT_INDEX, PCICMD_IIO_PCIEDMI_REG);
  PciCmd.Bits.mse = 0;
  IioWriteCpuCsr16 (IioIndex, DMI_PORT_INDEX, PCICMD_IIO_PCIEDMI_REG, PciCmd.Data);
}

