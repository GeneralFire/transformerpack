/** @file
  Configures PCH Sata Controller

@copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2020 Intel Corporation.

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

#include <Ppi/SiPolicy.h>
#include <Library/DebugLib.h>
#include <Library/PchPolicyLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/TimerLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PeiSataLib.h>
#include <Register/SataRegs.h>
#include <Library/IoLib.h>
#include <PcieRegs.h>

/**
  Write value to SATA SIR register

  @param[in] SataPciBase  SATA PCI config
  @param[in] SirIndex     Index of the SIR
  @param[in] Value        Value to write to register
**/
VOID
SataSirWrite32 (
  IN UINT64  SataPciBase,
  IN UINT8   SirIndex,
  IN UINT32  Value
  )
{
  PciSegmentWrite8 (SataPciBase + R_SATA_CFG_SIRI, SirIndex);
  PciSegmentWrite32 (SataPciBase + R_SATA_CFG_SIRD, Value);
}

/**
  AND then OR SATA SIR register with specified values

  @param[in] SataPciBase  SATA PCI config
  @param[in] SirIndex     Index of the SIR
  @param[in] AndValue     Value to AND the register with
  @param[in] OrValue      Value to OR the register with
**/
VOID
SataSirAndThenOr32 (
  IN UINT64  SataPciBase,
  IN UINT8   SirIndex,
  IN UINT32  AndValue,
  IN UINT32  OrValue
  )
{
  PciSegmentWrite8 (SataPciBase + R_SATA_CFG_SIRI, SirIndex);
  PciSegmentAndThenOr32 (SataPciBase + R_SATA_CFG_SIRD, AndValue, OrValue);
}

/**
  OR SATA SIR register with specified value

  @param[in] SataPciBase  SATA PCI config
  @param[in] SirIndex     Index of the SIR
  @param[in] OrValue      Value to OR the register with
**/
VOID
SataSirOr32 (
  IN UINT64  SataPciBase,
  IN UINT8   SirIndex,
  IN UINT32  OrValue
  )
{
  PciSegmentWrite8 (SataPciBase + R_SATA_CFG_SIRI, SirIndex);
  PciSegmentOr32 (SataPciBase + R_SATA_CFG_SIRD, OrValue);
}

/**
  SATA throttling actions

  @param[in]  SataController    Pointer to SATA device structure
  @param[in]  SataConfig        Pointer to SATA policy
**/
VOID
SataThrottlingConfiguration (
  IN  SATA_CONTROLLER *SataController,
  IN  SATA_CONFIG     *SataConfig
  )
{
  UINT8                                 Index;
  UINT32                                Data32And;
  UINT32                                Data32Or;
  UINT32                                Pcs;

  ///
  /// SATA throttling action
  /// Program SATA Indexed Register Index and Data:
  /// a. Set SATA PCI offset A0h = 0Ch for port 0 and 1 if avaiable,
  ///    If Port 0 has a HDD, set SATA PCI offset A4h[15:00] = 0039h
  ///    If Port 0 has a SSD, set SATA PCI offset A4h[15:00] = 0F39h
  ///    If Port 1 has a HDD, set SATA PCI offset A4h[31:16] = 0039h
  ///    If Port 1 has a SSD, set SATA PCI offset A4h[31:16] = 0F39h
  /// b. Set SATA PCI offset A0h = 1Ch for port 2 and 3 if avaiable,
  ///    If Port 2 has a HDD, set SATA PCI offset A4h[15:00] = 0039h
  ///    If Port 2 has a SSD, set SATA PCI offset A4h[15:00] = 0F39h
  ///    If Port 3 has a HDD, set SATA PCI offset A4h[31:16] = 0039h
  ///    If Port 3 has a SSD, set SATA PCI offset A4h[31:16] = 0F39h
  ///
  /// c. Set SATA PCI offset A0h = 2Ch for port 4 and 5 if available,
  ///    If Port 4 has a HDD, set SATA PCI offset A4h[15:00] = 0039h
  ///    If Port 4 has a SSD, set SATA PCI offset A4h[15:00] = 0F39h
  ///    If Port 5 has a HDD, set SATA PCI offset A4h[31:16] = 0039h
  ///    If Port 5 has a SSD, set SATA PCI offset A4h[31:16] = 0F39h
  /// d. Set SATA PCI offset A0h = 3Ch for port 6 and 7 if avaiable,
  ///    If Port 6 has a HDD, set SATA PCI offset A4h[15:00] = 0039h
  ///    If Port 6 has a SSD, set SATA PCI offset A4h[15:00] = 0F39h
  ///    If Port 7 has a HDD, set SATA PCI offset A4h[31:16] = 0039h
  ///    If Port 7 has a SSD, set SATA PCI offset A4h[31:16] = 0F39h
  ///

  //
  // Check PxE bit if port is enabled.
  //
  Pcs = PciSegmentRead32 (SataController->PciCfgBaseAddr + R_SATA_CFG_PCS);

  for (Index = 0; Index < ((SataController->TotalCtrlPortNum + 1) / 2); Index++) {
    Data32And     = 0;
    Data32Or      = 0;

    if (SataConfig->ThermalThrottling.SuggestedSetting == TRUE) {
      if (Pcs & (BIT0 << (Index * 2))) {
        Data32Or |= 0x00000039;
        if (SataConfig->PortSettings[0 + (Index * 2) ].SolidStateDrive == TRUE) {
          Data32Or |= 0x00000F00;
        }
      }
      //
      // If the max SATA port is in odd number, do not overflow the policy buffer.
      //
      if (SataController->TotalCtrlPortNum  > (UINTN)(1 + (Index * 2))) {
        if (Pcs & (BIT1 << (Index * 2))) {
          Data32Or |= 0x00390000;
          if (SataConfig->PortSettings[1 + (Index * 2) ].SolidStateDrive == TRUE) {
            Data32Or |= 0x0F000000;
          }
        }
      }
    } else {
      Data32Or  = (UINT32) (SataConfig->ThermalThrottling.P1TDispFinit << 31) |
        (UINT32) (SataConfig->ThermalThrottling.P1Tinact << 26) |
        (UINT32) (SataConfig->ThermalThrottling.P1TDisp << 24) |
        (UINT32) (SataConfig->ThermalThrottling.P1T3M << 20) |
        (UINT32) (SataConfig->ThermalThrottling.P1T2M << 18) |
        (UINT32) (SataConfig->ThermalThrottling.P1T1M << 16) |
        (UINT32) (SataConfig->ThermalThrottling.P0TDispFinit << 15) |
        (UINT32) (SataConfig->ThermalThrottling.P0Tinact << 10) |
        (UINT32) (SataConfig->ThermalThrottling.P0TDisp << 8) |
        (UINT32) (SataConfig->ThermalThrottling.P0T3M << 4) |
        (UINT32) (SataConfig->ThermalThrottling.P0T2M << 2) |
        (UINT32)  SataConfig->ThermalThrottling.P0T1M;
    }

    SataSirAndThenOr32 (SataController->PciCfgBaseAddr, (R_SATA_CFG_SIR_0C + (Index * 0x10)), Data32And, Data32Or);
  }
}

/**
  Check if any lanes are assigned to SATA

  @param[in]  Sata                Pointer to SATA controller configuration structure

  @retval TRUE                    Sata lanes exist
  @retval FALSE                   Sata lanes does not exist
**/
BOOLEAN
SataLaneExist (
  IN SATA_HANDLE   *Sata
  )
{
  UINT8                 Index;
  SATA_CONTROLLER       *SataController;
  SATA_CALLBACK         *SataCallback;

  SataController    = Sata->Controller;
  SataCallback      = Sata->Callback;

  for (Index = 0; Index < SataController->TotalCtrlPortNum; Index++) {
    if (SataCallback->SataIsPhyConnected != NULL) {
      if (SataCallback->SataIsPhyConnected (SataController, Index)) {
      return TRUE;
    }
    } else {
      DEBUG ((DEBUG_ERROR, "Error: SataLaneExist() NOT defined!\n"));
    }
  }
  return FALSE;
}

/**
  Configures which interrupts capabilities will be linked to SATA controller CAP list
  By default first capability in capability list is MSI cap structure

  @param[in] Sata        Pointer to SATA controller configuration structure

**/
VOID
ConfigureSataInterrupts (
  IN  SATA_HANDLE *Sata
  )
{
  SATA_CONFIG      *SataConfig;
  SATA_CONTROLLER  *SataController;

  SataConfig      = Sata->SataConfig;
  SataController  = Sata->Controller;

  //
  // Configure SATA interrupt
  //
  PciSegmentWrite8 (SataController->PciCfgBaseAddr + PCI_INT_PIN_OFFSET, Sata->PrivateConfig->InterruptPin);

  //
  // Set MSI-X Table offset and PBAO offset to 0.
  // We only do this because the register is write once and
  // we don't want to leave it unlocked.
  //
  PciSegmentAnd32 (SataController->PciCfgBaseAddr + R_SATA_CFG_MXT, 0x7);
  PciSegmentAnd32 (SataController->PciCfgBaseAddr + R_SATA_CFG_MXP, 0x7);

  //
  // Configures which interrupts capabilities will be linked to SATA controller CAP list
  // By default first capability in capability list is MSI cap structure
  // This option is only used for RAID mode since AHCI specification does not define MSI-X support
  //
  if (SataConfig->SataMode == SataModeRaid) {
    if (SataConfig->SataRstInterrupt == SataRstMsix) {
      PciSegmentWrite8 (Sata->Controller->PciCfgBaseAddr + PCI_CAPBILITY_POINTER_OFFSET, R_SATA_CFG_MXID);
      PciSegmentOr16 (Sata->Controller->PciCfgBaseAddr + R_SATA_CFG_MXID, R_SATA_CFG_PID << N_SATA_CFG_MXID_NEXT);
    } else if (SataConfig->SataRstInterrupt == SataRstLegacy) {
      //
      // Remove MSI cap structure from list by linking Power management capabilities in place of MSI cap
      //
      PciSegmentWrite8 (Sata->Controller->PciCfgBaseAddr + PCI_CAPBILITY_POINTER_OFFSET, R_SATA_CFG_PID);
    }
  }
}

/**
  Configures SATA controller device ID by programming SATAGC register

  @param[in]      Sata        Pointer to SATA controller configuration structure
  @param[in, out] SataGcReg   A value of SATAGC register to be programmed

**/
VOID
ConfigureSataControllerDeviceId  (
  IN CONST SATA_HANDLE  *Sata,
  IN OUT   UINT8        *SataGcReg
  )
{
  if ((Sata->SataConfig->SataMode == SataModeRaid) && (Sata->PrivateConfig->SataSupportedRstMode != RstUnsupported)) {
    if (Sata->SataConfig->RaidDeviceId == SataRaidAlternate) {
      (*SataGcReg) |= B_SATA_CFG_SATAGC_AIE;
      (*SataGcReg) &= ~B_SATA_CFG_SATAGC_AIES;
    } else if (Sata->SataConfig->RaidDeviceId == SataRaidServer) {
      (*SataGcReg) &= ~B_SATA_CFG_SATAGC_AIE;
      (*SataGcReg) |= B_SATA_CFG_SATAGC_AIES;
    }
  }
}

/**
  Configures SATA controller mode

  @param[in] Sata     Pointer to SATA controller configuration structure
**/
VOID
ConfigureSataControllerMode (
  IN SATA_HANDLE  *Sata
  )
{
  SATA_CONTROLLER    *SataController;

  SataController   = Sata->Controller;
  ///
  /// Initialize the SATA mode to be in AHCI, then check the SATA mode from the policy
  /// If RAID mode is selected and the SKU supports RAID feature, set the SATA Mode Select to 1b (RAID)
  ///
  PciSegmentAnd8 (SataController->PciCfgBaseAddr + R_SATA_CFG_SATAGC + 2,(UINT8) ~(BIT0));

  if (Sata->SataConfig->SataMode == SataModeRaid) {
    if (Sata->PrivateConfig->SataSupportedRstMode != RstUnsupported) {
      PciSegmentAndThenOr8 (
        SataController->PciCfgBaseAddr + R_SATA_CFG_SATAGC + 2,
        (UINT8) ~(BIT0),
        (UINT8) (V_SATA_CFG_SATAGC_SMS_RAID)
        );
    } else {
      DEBUG ((DEBUG_ERROR, "This SKU doesn't support RAID feature. Set to AHCI mode.\n"));
    }
  }

}

/**
  Performs DevSleep related programming for given SATA port.
  Caller of this function is responsible for making sure that AHCI BAR has been set.

  @param[in] Sata           Pointer to SATA controller configuration structure
  @param[in] Port           Port number (0 based) for Dev Slp initialization
  @param[in] AhciBar        AHCI BAR

**/
VOID
SataPortInitializeDevSleep (
  IN  SATA_HANDLE  *Sata,
  IN  UINT8        Port,
  IN  UINT32       AhciBar
  )
{
  SATA_CONFIG     *SataConfig;
  SATA_CONTROLLER *SataController;
  SATA_CALLBACK   *SataCallback;
  UINT32  Data32And;
  UINT32  Data32Or;

  SataConfig      = Sata->SataConfig;
  SataController  = Sata->Controller;
  SataCallback    = Sata->Callback;
  //
  // DevSleep programming
  // Set ABAR + 144h[1], ABAR + 1C4h[1], ABAR + 244h[1], ABAR + 2C4[1] to 0b as default
  // POR settings are ABAR + 144h[1], ABAR + 1C4h[1], ABAR + 244h[1] = 1b, ABAR + 2C4[1] to 0b
  //
  if (SataConfig->PortSettings[Port].DevSlp == TRUE) {
    Data32And = (UINT32) ~(B_SATA_MEM_AHCI_PxDEVSLP_DITO_MASK | B_SATA_MEM_AHCI_PxDEVSLP_DM_MASK);
    Data32Or  = (B_SATA_MEM_AHCI_PxDEVSLP_DSP | V_SATA_MEM_AHCI_PxDEVSLP_DM_16 | V_SATA_MEM_AHCI_PxDEVSLP_DITO_625);

    if (SataConfig->PortSettings[Port].EnableDitoConfig == TRUE) {
      Data32Or &= Data32And;
      Data32Or |= ((SataConfig->PortSettings[Port].DitoVal << 15) | (SataConfig->PortSettings[Port].DmVal << 25));
    }

    MmioAndThenOr32 (
      AhciBar + (R_SATA_MEM_AHCI_P0DEVSLP + (0x80 * Port)),
      Data32And,
      Data32Or
      );

    if (SataCallback->SataDevSleepEnable  != NULL) {
      SataCallback->SataDevSleepEnable (SataController, Port, SataConfig->PortSettings[Port].DevSlpResetConfig);
    } else {
      DEBUG ((DEBUG_WARN, "SataDevSleepEnable() NOT defined!\n"));
    }

  } else {
    MmioAnd32 (
      AhciBar + (R_SATA_MEM_AHCI_P0DEVSLP + (0x80 * Port)),
      (UINT32) ~(B_SATA_MEM_AHCI_PxDEVSLP_DSP)
      );
  }
}

/**
  Program AHCI PI register for Enabled ports
  Handle hotplug, and interlock switch setting,
  and config related GPIOs.

  @param[in] Sata         Pointer to SATA controller configuration structure

**/
VOID
ConfigureSataAhci (
  IN  SATA_HANDLE     *Sata
  )
{
  BOOLEAN               RemapEnable;
  UINT32                AhciBar;
  SATA_CONFIG           *SataConfig;
  SATA_CONTROLLER       *SataController;
  SATA_CALLBACK         *SataCallback;
  UINT32                Data32Or;
  UINT32                PxCMDRegister;
  UINT16                SataPortsEnabled;
  UINT8                 Index;
  UINT8                 PortSataControl;
  UINT32                CapRegister;

  SataConfig      = Sata->SataConfig;
  SataController  = Sata->Controller;
  SataCallback    = Sata->Callback;
  DEBUG ((DEBUG_INFO, "ConfigureSataAhci() Started (SATA controler index: %d)\n", SataController->SataCtrlIndex ));

  AhciBar         = (UINT32) Sata->TempBar;
  RemapEnable     = (Sata->PrivateConfig->RemappingSupported && Sata->PrivateConfig->StorageRemappingEnabled);

  ///
  /// Set the AHCI BAR
  ///
  PciSegmentWrite32 (SataController->PciCfgBaseAddr + R_SATA_CFG_AHCI_BAR, AhciBar);

  ///
  /// Enable command register memory space decoding
  ///
  PciSegmentOr16 (SataController->PciCfgBaseAddr + PCI_COMMAND_OFFSET, (UINT16) EFI_PCI_COMMAND_MEMORY_SPACE);

  ///
  /// Get Port Settings
  ///
  SataPortsEnabled = PciSegmentRead16 (SataController->PciCfgBaseAddr + R_SATA_CFG_PCS) & B_SATA_CFG_PCS_PXE_MASK;

  //
  // Read the default value of the Host Capabilites Register
  // NOTE: many of the bits in this register are R/WO (Read/Write Once)
  //
  CapRegister = MmioRead32 (AhciBar + R_SATA_MEM_AHCI_CAP);
  CapRegister &= (UINT32) ~(B_SATA_MEM_AHCI_CAP_SMPS | B_SATA_MEM_AHCI_CAP_SSS | B_SATA_MEM_AHCI_CAP_SALP |
                            B_SATA_MEM_AHCI_CAP_SPM  | B_SATA_MEM_AHCI_CAP_SSC | B_SATA_MEM_AHCI_CAP_PSC |
                            B_SATA_MEM_AHCI_CAP_SXS);

  for (Index = 0; Index < SataController->TotalCtrlPortNum; Index++) {
    //
    // Check PCS.PxE to know if the SATA Port x is disabled.
    //
    if ((SataPortsEnabled & (BIT0 << Index)) == 0) {
      continue;
    }

    if (SataConfig->PortSettings[Index].InterlockSw == TRUE) {
      //
      // Mechanical Presence Switch is Enabled for at least one of the ports
      //
      CapRegister |= B_SATA_MEM_AHCI_CAP_SMPS;
    }

    if ((SataConfig->PortSettings[Index].SpinUp == TRUE) ||
        (SataConfig->PortSettings[Index].External == TRUE)) {
      //
      // PCH BIOS Spec Section 14.1.4 Initialize Registers in AHCI Memory-Mapped Space
      // Step 1.4
      // Set SSS (ABAR + 00h[27]) to enable SATA controller supports staggered
      // spin-up on its ports, for use in balancing power spikes
      // SATA Port Spin up is supported at least one of the ports
      // If this is an extra eSATA port. It needs to be hotpluggable but it's usually empty anyway
      // so LPM is not available but Listen Mode is available, so it will have good power management.
      // If Sata Test Mode enabled, then uncoditonally clear SSS (ABAR + 00h[27])
      // to resolve Spin-donw issue with the test equiepment
      //
      if (SataConfig->TestMode == FALSE ) {
        CapRegister |= B_SATA_MEM_AHCI_CAP_SSS;
      }
    }

    if (SataConfig->PortSettings[Index].External == TRUE) {
      //
      // External SATA is supported at least one of the ports
      //
      CapRegister |= B_SATA_MEM_AHCI_CAP_SXS;
    }
  }

  //
  // PCH BIOS Spec Section 14.1.4 Initialize Registers in AHCI Memory-Mapped Space
  // Step 1
  // Set PSC (ABAR + 00h[13]). This bit informs the Windows software driver that the AHCI
  // Controller supports the partial low-power state.
  // Set SSC (ABAR + 00h[14]). This bit informs the Windows software driver that the AHCI
  // Controller supports the slumber low-power state.
  // Set SALP (ABAR + 00h[26]) to enable Aggressive Link Power Management (LPM) support.
  //
  CapRegister |= (B_SATA_MEM_AHCI_CAP_SSC | B_SATA_MEM_AHCI_CAP_PSC);

  if (SataConfig->SalpSupport == TRUE) {
    CapRegister |= B_SATA_MEM_AHCI_CAP_SALP;
  }
  //
  // Support Command List Override & PIO Multiple DRQ Block
  //
  CapRegister |= (B_SATA_MEM_AHCI_CAP_SCLO | B_SATA_MEM_AHCI_CAP_PMD);

  //
  // Configure for the max speed support 1.5Gb/s, 3.0Gb/s and 6.0Gb/s.
  //
  CapRegister &= ~B_SATA_MEM_AHCI_CAP_ISS_MASK;

  switch (SataConfig->SpeedLimit) {
    case SataSpeedGen1:
      CapRegister |= (V_SATA_MEM_AHCI_CAP_ISS_1_5_G << N_SATA_MEM_AHCI_CAP_ISS);
      break;

    case SataSpeedGen2:
      CapRegister |= (V_SATA_MEM_AHCI_CAP_ISS_3_0_G << N_SATA_MEM_AHCI_CAP_ISS);
      break;

    case SataSpeedGen3:
    case SataSpeedDefault:
      CapRegister |= (V_SATA_MEM_AHCI_CAP_ISS_6_0_G << N_SATA_MEM_AHCI_CAP_ISS);
      break;
  }

  if (SataConfig->EnclosureSupport == TRUE) {
    CapRegister |= B_SATA_MEM_AHCI_CAP_EMS;
  }

  //
  // If remapping is enabled, disable Enclosure Management capability
  //
  if (RemapEnable) {
    CapRegister &= ~B_SATA_MEM_AHCI_CAP_EMS;
  }

  if (SataConfig->LedEnable == TRUE && SataConfig->SgpioSupport == FALSE) {
    if (SataCallback->SataLedEnable) {
      SataCallback->SataLedEnable (SataController);
    } else {
      DEBUG ((DEBUG_WARN, "SataLedEnable() NOT defined!\n"));
    }
    CapRegister |= B_SATA_MEM_AHCI_CAP_SAL;
  } else {
    CapRegister &= ~B_SATA_MEM_AHCI_CAP_SAL;
  }

  //
  // Update the Host Capabilites Register
  // NOTE: Many of the bits in this register are R/WO (Read/Write Once)
  //
  MmioWrite32 (AhciBar + R_SATA_MEM_AHCI_CAP, CapRegister);

  ///
  /// Enable implemented SATA ports by setting respective PI bits to 1b
  /// If BIOS accesses any of the port specific AHCI address range before setting PI bit,
  /// BIOS is required to read the PI register before the initial write to the PI register.
  /// NOTE: The read before initial write to PI register is done by the MmioOr32 routine.
  /// NOTE: many of the bits in this register are R/WO (Read/Write Once)
  ///
  Data32Or = 0;
  for (Index = 0; Index < SataController->TotalCtrlPortNum; Index++) {
    Data32Or |= SataConfig->PortSettings[Index].Enable << Index;
  }
  MmioOr32 (AhciBar + R_SATA_MEM_AHCI_PI, Data32Or);

  ///
  /// After BIOS issues initial write to this register, BIOS is requested to issue two
  /// reads to this register.
  ///
  Data32Or = MmioRead32 (AhciBar + R_SATA_MEM_AHCI_PI);
  Data32Or = MmioRead32 (AhciBar + R_SATA_MEM_AHCI_PI);

  ///
  /// Set ABAR + 24h[5] to 1b
  /// Set ABAR + 24h[4:2] to 110b
  ///
  Data32Or = B_SATA_MEM_AHCI_CAP2_DESO;
  Data32Or |= B_SATA_MEM_AHCI_CAP2_SDS | B_SATA_MEM_AHCI_CAP2_SADM;
  MmioAndThenOr32 (AhciBar + R_SATA_MEM_AHCI_CAP2, ((UINT32)~B_SATA_MEM_AHCI_CAP2_APST), Data32Or);

  ///
  /// Port[Max:0] Command Register update
  /// NOTE: this register must be updated after Port Implemented and Capabilities register,
  /// Many of the bits in this register are R/WO (Read/Write Once)
  ///
  for (Index = 0; Index < SataController->TotalCtrlPortNum; Index++) {
    ///
    /// Check PCS.PxE to know if the SATA Port x is disabled.
    ///
    if ((SataPortsEnabled & (BIT0 << Index)) == 0) {
      continue;
    }
    ///
    /// Initial to zero first
    ///
    PxCMDRegister = 0;

    if (SataConfig->PortSettings[Index].HotPlug == TRUE) {
      if (SataConfig->PortSettings[Index].External == FALSE) {
        ///
        /// Hot Plug of this port is enabled
        ///
        PxCMDRegister |= B_SATA_MEM_AHCI_PxCMD_HPCP;
        if (SataConfig->PortSettings[Index].InterlockSw == TRUE && SataConfig->SgpioSupport == FALSE) {
          ///
          /// Mechanical Switch of this port is Attached
          ///
          PxCMDRegister |= B_SATA_MEM_AHCI_PxCMD_MPSP;

          if (SataCallback->SataHotPlugEnable != NULL) {
            SataCallback->SataHotPlugEnable (SataController, Index);
          } else {
            DEBUG ((DEBUG_WARN, "SataHotPlugEnable() NOT defined!\n"));
          }
        }
      }
    } else {
      ///
      /// When "Mechanical Switch Attached to Port" (PxCMD[19]) is set, it is expected that HPCP (PxCMD[18]) is also set.
      ///
      if (SataConfig->PortSettings[Index].InterlockSw == TRUE) {
        DEBUG ((DEBUG_ERROR, "Hot-Plug function of Port%d should be enabled while the Inter Lock Switch of it is enabled!\n",
                Index));
      }
    }

    if (SataConfig->PortSettings[Index].External == TRUE) {
      PxCMDRegister |= B_SATA_MEM_AHCI_PxCMD_ESP;
    }

    ///
    /// Set ALPE
    ///
    PxCMDRegister |= (B_SATA_MEM_AHCI_PxCMD_ALPE);

    //
    // If the SATA controller supports staggerred spin-up (SSS in AHCI_CAP is set to 1b),
    // then set the SUD bit in Port[Max:0] Command Register to 1b.
    // This should trigger COMRESET on all ports.
    //
    if ((CapRegister & B_SATA_MEM_AHCI_CAP_SSS) != 0) {
      PxCMDRegister |= B_SATA_MEM_AHCI_PxCMD_SUD;
    }

    MmioAndThenOr32 (
      AhciBar + (R_SATA_MEM_AHCI_P0CMD + (0x80 * Index)),
      (UINT32) ~(B_SATA_MEM_AHCI_PxCMD_MASK),
      (UINT32) PxCMDRegister
      );

    SataPortInitializeDevSleep (Sata, Index, AhciBar);

    //
    // eSATA port support only up to Gen2.
    // When enabled, BIOS will configure the PxSCTL.SPD to 2 to limit the eSATA port speed.
    // Please be noted, this setting could be cleared by HBA reset, which might be issued
    // by EFI AHCI driver when POST time, or by SATA inbox driver/RST driver after POST.
    // To support the Speed Limitation when POST, the EFI AHCI driver should preserve the
    // setting before and after initialization. For support it after POST, it's dependent on
    // driver's behavior.
    //
    if ((SataConfig->EsataSpeedLimit == TRUE) &&
        (SataConfig->PortSettings[Index].External == TRUE)) {
      PortSataControl = MmioRead8 (AhciBar + (R_SATA_MEM_AHCI_P0SCTL + (0x80 * Index)));
      PortSataControl &= (UINT8) ~(B_SATA_MEM_AHCI_PXSCTL_SPD);
      PortSataControl |= (UINT8) V_SATA_MEM_AHCI_PXSCTL_SPD_2;
      MmioWrite8 (AhciBar + (R_SATA_MEM_AHCI_P0SCTL + (0x80 * Index)), PortSataControl);
    }
  }

  ///
  /// Disable command register memory space decoding
  ///
  PciSegmentAnd16 (SataController->PciCfgBaseAddr + PCI_COMMAND_OFFSET, (UINT16) ~EFI_PCI_COMMAND_MEMORY_SPACE);

  ///
  /// Set Ahci Bar to zero
  ///
  AhciBar = 0;
  PciSegmentWrite32 (SataController->PciCfgBaseAddr + R_SATA_CFG_AHCI_BAR, AhciBar);

  ///
  /// if in test mode enable, perform misc programming for test mode
  ///
  if (SataConfig->TestMode == TRUE) {
    if (SataCallback->SataTestModeEnable != NULL) {
      SataCallback->SataTestModeEnable (SataController);
    } else {
      DEBUG ((DEBUG_WARN, "SataTestModeEnable() NOT defined!\n"));
    }
  }

  if(SataConfig->SgpioSupport == TRUE) {
    if (SataCallback->SataSgpioEnable != NULL) {
      SataCallback->SataSgpioEnable (SataController);
    } else {
      DEBUG ((DEBUG_WARN, "SataSgpioEnable() NOT defined!\n"));
    }
  }

  DEBUG ((DEBUG_INFO, "ConfigureSataAhci() Ended (SATA controler index: %d)\n", SataController->SataCtrlIndex ));
}

/**
  Disable Sata Controller

  @param[in] Sata         Pointer to SATA controller configuration structure

**/
VOID
DisablePchSataController (
  IN  SATA_HANDLE   *Sata
  )
{
  SATA_CONTROLLER *SataController;
  SATA_CALLBACK   *SataCallback;
  UINT32                                AhciBar;
  UINT32                                Data32And;
  UINT32                                Data32Or;

  SataController  = Sata->Controller;
  SataCallback    = Sata->Callback;

  DEBUG ((DEBUG_INFO, "DisablePchSataController() Started (SATA controler index: %d)\n", SataController->SataCtrlIndex));

  ///
  /// Set the AHCI BAR
  ///
  AhciBar = (UINT32) Sata->TempBar;
  PciSegmentWrite32 (SataController->PciCfgBaseAddr + R_SATA_CFG_AHCI_BAR, AhciBar);

  ///
  /// Enable command register memory space decoding
  ///
  PciSegmentOr16 (SataController->PciCfgBaseAddr + PCI_COMMAND_OFFSET, (UINT16) EFI_PCI_COMMAND_MEMORY_SPACE);

  ///
  /// PCH BIOS Spec Section 14.3 SATA Controller Disabling
  /// Step 1 - Disable all ports
  /// Set SATA PCI offset 94h[7:0] to all 0b
  ///
  PciSegmentAnd16 (SataController->PciCfgBaseAddr + R_SATA_CFG_PCS, (UINT16) ~(B_SATA_CFG_PCS_PXE_MASK));

  //
  // After clearing PCS.PxE register BIOS is required to delay for 1.4us before setting PCD register
  //
  MicroSecondDelay (2);

  ///
  /// Step 2 - Disable all ports
  /// Clear PI register, ABAR + 0Ch
  ///
  Data32And = (UINT32) (~B_SATA_MEM_AHCI_PI_PORT_MASK);
  MmioAnd32 (AhciBar + R_SATA_MEM_AHCI_PI, Data32And);

  ///
  /// After BIOS issues initial write to this register, BIOS is requested to
  /// issue two reads to this register.
  ///
  Data32Or = MmioRead32 (AhciBar + R_SATA_MEM_AHCI_PI);
  Data32Or = MmioRead32 (AhciBar + R_SATA_MEM_AHCI_PI);

  ///
  /// Step 3
  /// Clear MSE and IOE, SATA PCI offset 4h[1:0] = 00b
  ///
  PciSegmentAnd16 (SataController->PciCfgBaseAddr + PCI_COMMAND_OFFSET, (UINT16) ~(EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_IO_SPACE));

  ///
  /// Step 4
  /// Set Sata Port Clock Disable bits, SATA PCI offset 90h[7:0] to all 1b
  ///
  if (!Sata->PrivateConfig->SataSkipPortClockDisable) {
    PciSegmentOr32 (SataController->PciCfgBaseAddr + R_SATA_CFG_MAP, (UINT32) B_SATA_CFG_MAP_PCD);
  }

  ///
  /// Step 5 Enable all SATA dynamic clock gating and dynamic power gating features:
  ///  a. SATA SIR offset 9Ch bit[29, 23] = [1, 1]
  ///  b. SATA SIR offset 8Ch bit[23:16, 7:0] to all 1b
  ///  c. SATA SIR offset A0h bit[15] to [1]
  ///  d. SATA SIR offset 84h bit[23:16] to all 1b
  ///  e. SATA SIR offset A4h bit[26, 25, 24, 14, 5] = [0, 0, 0, 1, 1]
  ///  f. SATA SIR offset A8h bit[17, 16] = [1, 1]
  ///
  /// Step 5a
  /// SATA SIR offset 9Ch bit[29, 23] = [1, 1]
  ///
  SataSirOr32 (SataController->PciCfgBaseAddr, R_SATA_CFG_SIR_9C, (UINT32) (BIT29 | BIT23));

  ///
  /// Step 5b
  /// SATA SIR offset 8Ch bit[23:16, 7:0] to all 1b
  ///
  SataSirOr32 (SataController->PciCfgBaseAddr, R_SATA_CFG_SIR_8C, 0x00FF00FF);

  ///
  /// Step 5c
  /// SATA SIR offset A0h bit[15] to [1]
  ///
  SataSirOr32 (SataController->PciCfgBaseAddr, R_SATA_CFG_SIR_A0, (UINT32) (BIT15));

  ///
  /// Step 5d
  /// SATA SIR offset 84h bit[23:16] to all 1b
  ///
  SataSirOr32 (SataController->PciCfgBaseAddr, R_SATA_CFG_SIR_84, 0x00FF0000);

  ///
  /// Step 5e
  /// SATA SIR offset A4h bit[26, 25, 24, 14, 5] = [0, 0, 0, 1, 1]
  ///
  Data32And = (UINT32) ~(BIT26 | BIT25 | BIT24 | BIT14 | BIT5);
  Data32Or  = (UINT32) (BIT14 | BIT5);
  SataSirAndThenOr32 (SataController->PciCfgBaseAddr, R_SATA_CFG_SIR_A4, Data32And, Data32Or);

  ///
  /// Step 5f
  /// SATA SIR offset A8h bit[17, 16] = [1, 1]
  ///
  Data32And = (UINT32) ~(BIT17 | BIT16);
  Data32Or  = (UINT32) (BIT17 | BIT16);
  SataSirAndThenOr32 (SataController->PciCfgBaseAddr, R_SATA_CFG_SIR_A8, Data32And, Data32Or);

  ///
  /// Step 5h
  /// To support modPHY Sus Well power gating, put SATA PCI Function to enter D3 by programming SATA PCI Offset 74h [1:0] to 3.
  /// PMC require the _in_d3 signal from SATA Controller to assert in order to allow modPHY Sus Well power
  ///
  PciSegmentOr32 (SataController->PciCfgBaseAddr + R_SATA_CFG_PMCS, (UINT32) (BIT1 | BIT0));

  ///
  /// Step 6
  /// Disabling SATA Device by programming SATA SCFD, SATA PCI offset 9Ch[10] = 1
  ///
  PciSegmentOr32 (SataController->PciCfgBaseAddr + R_SATA_CFG_SATAGC, (UINT32) (BIT10));

  ///
  /// Set Ahci Bar to zero (Note: MSE and IO has been disabled)
  ///
  AhciBar = 0;
  PciSegmentWrite32 (SataController->PciCfgBaseAddr + R_SATA_CFG_AHCI_BAR, AhciBar);

  ///
  /// Step 7
  /// Disable SATA in PSF and in PMC
  ///
  if (SataCallback->SataControllerDisable) {
    SataCallback->SataControllerDisable (SataController);
  } else {
    DEBUG ((DEBUG_ERROR, "Error: SataControllerDisable() NOT defined!\n"));
  }

  DEBUG ((DEBUG_INFO, "DisablePchSataController() Ended (SATA controler index: %d)\n", SataController->SataCtrlIndex));
}

/**
  Configures Sata Controller

  @param[in] Sata         Pointer to SATA controller configuration structure

**/
VOID
EarlyConfigureSata (
  IN  SATA_HANDLE   *Sata
  )
{
  SATA_CONTROLLER         *SataController;
  BOOLEAN                 RemapEnable;
  UINT8                   SataGcReg;
  UINT32                  Data32And;
  UINT32                  Data32Or;

  SataController  = Sata->Controller;
  DEBUG ((DEBUG_INFO, "EarlyConfigureSata() Started (SATA controller reg base: 0x%08x)\n", SataController->PciCfgBaseAddr));

  RemapEnable     = (Sata->PrivateConfig->RemappingSupported && Sata->PrivateConfig->StorageRemappingEnabled);

  ///
  /// PCH BIOS Spec section 14.1.6 Additional Programming Requirements during
  /// SATA Initialization
  /// System BIOS must set SATA SIR Index 98h[8:0] = 183h
  /// System BIOS must set SATA SIR Index 98h[15:15] = 1h
  /// as part of the chipset initialization prior to SATA configuration.
  /// These bits should be restored while resuming from a S3 sleep state.
  ///
  Data32And = (UINT32) ~(BIT21 | BIT8 | BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0);
  Data32Or  = 0x200183;
  SataSirAndThenOr32 (SataController->PciCfgBaseAddr, R_SATA_CFG_SIR_98, Data32And, Data32Or);

  ///
  /// Set SATA SIR Index A4h[6] = 1b
  /// These bits should be restored while resuming from a S3 sleep state
  ///
  SataSirOr32 (SataController->PciCfgBaseAddr, R_SATA_CFG_SIR_A4, BIT6);

  ///
  /// PCH BIOS Spec section 14.1.7 Additional Programming Requirements during
  /// SATA Initialization
  /// Step 4
  /// Set SATA SIR Index 9Ch[22] = 1b for desktop and mobile platform only
  /// (Server may leave as default)
  ///
  if (Sata->PrivateConfig->SataSramParityCheckDisable) {
    SataSirOr32 (SataController->PciCfgBaseAddr, R_SATA_CFG_SIR_9C, BIT22);
  }

  ///
  /// Program SATA SIR Index 9Ch[29,23,20,19,18] to all 1b
  /// Program SATA SIR Index 9Ch[12:7] = 04h
  /// Program SATA SIR Index 9Ch[6:5] to 01b
  /// Program SATA SIR Index 9Ch[3:2] to 01b
  /// Note: If [3:2] value recommendation get changed, please refer to definition if Bit 1:0 and program per table accordingly
  ///
  Data32And = (UINT32) (~(BIT12 | BIT11 | BIT10 | BIT9 | BIT8 | BIT7 | BIT6 | BIT5 | BIT3 | BIT2));
  Data32Or  = (UINT32) (BIT29 | BIT23 | BIT20 | BIT19 | BIT18 | BIT9 | BIT5 | BIT2);
  SataSirAndThenOr32 (SataController->PciCfgBaseAddr, R_SATA_CFG_SIR_9C, Data32And, Data32Or);

  ///
  /// For PCH before B0 stepping:
  ///      Set SATA SIR Index C4h[27:26] = 01b
  ///
  if(Sata->PrivateConfig->SirC4Programming) {
    SataSirAndThenOr32 (SataController->PciCfgBaseAddr, R_SATA_CFG_SIR_C4, (UINT32) ~B_SATA_CFG_SIR_C4_TCLKSNACK_CP, (UINT32) (BIT26));
  }

  ///
  /// Program SATA PCI offset 9Ch[5] to 1b
  /// Note: Bits 7:0 are RWO, perform byte write here, program the 9Ch[31] to 1b later in PchOnEndOfDxe ()
  ///
  SataGcReg = PciSegmentRead8 (SataController->PciCfgBaseAddr + R_SATA_CFG_SATAGC);
  SataGcReg |= BIT5;

  ConfigureSataControllerDeviceId (Sata, &SataGcReg);

  ///
  /// If RstPcieStorageRemapEnabled is TRUE, program Sata PCI offset 9Ch, bit[4:3] and bit[2:0] to [00'b] and [110'b]
  /// else, program Sata PCI offset 9Ch, bit[4:3] and bit[2:0] to [10'b] and [000'b], and also disable Rst remap decoding
  ///
  if (RemapEnable) {
    SataGcReg |= V_SATA_CFG_SATAGC_ASSEL_512K;
    SataGcReg &= ~(B_SATA_CFG_SATAGC_MSS);
  } else {
    SataGcReg &= ~B_SATA_CFG_SATAGC_ASSEL;
    SataGcReg |= V_SATA_CFG_SATAGC_MSS_8K << N_SATA_CFG_SATAGC_MSS;
  }

  ///
  /// Unconditional write is necessary to lock the register
  ///
  PciSegmentWrite8 (SataController->PciCfgBaseAddr + R_SATA_CFG_SATAGC, SataGcReg);

  //
  // Program PCS "Port X Enabled", SATA PCI offset 94h[7:0] to all 1b.
  // Programming this register now won't trigger COMRESET since staggered spin-up is enabled by default
  //
  PciSegmentOr16 (SataController->PciCfgBaseAddr + R_SATA_CFG_PCS, (UINT16) B_SATA_CFG_PCS_PXE_MASK);

  DEBUG ((DEBUG_INFO, "EarlyConfigureSata() Ended (SATA controller reg base: 0x%08x)\n", SataController->PciCfgBaseAddr));
}

/**
  Get if SATA controller should be disabled

  @param[in]  Sata              Pointer to SATA controller configuration structure

  @retval     TRUE              SATA Controller should be disabled
  @retval     FALSE             SATA Controller should not be disabled
**/
STATIC
BOOLEAN
IsSataDisableNeeded (
  IN  SATA_HANDLE   *Sata
  )
{
  if ((Sata->SataConfig->Enable == FALSE) || ((Sata->SataConfig->SataMode == SataModeAhci) && (!SataLaneExist (Sata)))) {
    return TRUE;
  }
  return FALSE;
}
/**
  Disable SATA Controller if needed after RST Remapping code

  @param[in]  Sata              Pointer to SATA controller configuration structure

  @retval None
**/
VOID
ConfigureSataAfterRst (
  IN  SATA_HANDLE   *Sata
  )
{
  if (Sata->PrivateConfig->RemappingSupported && IsSataDisableNeeded (Sata)) {
    DisablePchSataController (Sata);
  }
}

typedef struct {
  UINT8  BurstRejectLowerMax;
  UINT8  BurstRejectUpperMin;
  UINT8  CominitGapDetectionMax;
  UINT8  CominitGapDetectionMin;
  UINT8  ComwakeGapDetectionMax;
  UINT8  ComwakeGapDetectionMin;
} SATA_COMINIT_COMWAKE_PARAMETERS;

GLOBAL_REMOVE_IF_UNREFERENCED  SATA_COMINIT_COMWAKE_PARAMETERS  mSataCominitComwakeParameters[] = {
  {8, 23, 46, 32, 18, 8}, // SataSoscClk125MHz
  {8, 22, 44, 30, 17, 8}, // SataSoscClk120MHz
  {7, 19, 36, 25, 14, 7}, // SataSoscClk100MHz
  {3, 10, 19, 12, 6, 3},  // SataSoscClk25MHz
  {3, 8, 15, 9, 5, 3}     // SataSoscClk19p2MHz
  };

/**
  Configure sosc_clk frequency related registers
  that configure COMINIT and COMWAKE sampling.

  @param[in] Sata     Pointer to SATA controller configuration structure
**/
STATIC
VOID
SataConfigureCominitComwake (
  IN SATA_HANDLE   *Sata
  )
{
  SATA_CONTROLLER                  *SataController;
  SATA_COMINIT_COMWAKE_PARAMETERS  *Parameters;

  SataController = Sata->Controller;

  if (Sata->PrivateConfig->SataOscClkFreq >= ARRAY_SIZE (mSataCominitComwakeParameters)) {
    DEBUG ((DEBUG_ERROR, "Incorrect SATA sosc_clk frequency(%d)\n", Sata->PrivateConfig->SataOscClkFreq));
    return;
  }

  Parameters = &mSataCominitComwakeParameters[Sata->PrivateConfig->SataOscClkFreq];

  SataSirAndThenOr32 (
    SataController->PciCfgBaseAddr,
    R_SATA_CFG_SIR_A0,
    (UINT32) ~((B_SATA_CFG_SIR_A0_CWICBRLMAX << N_SATA_CFG_SIR_A0_CWICBRLMAX) |
               (B_SATA_CFG_SIR_A0_CWCIBRUMIN << N_SATA_CFG_SIR_A0_CWCIBRUMIN)),
    (UINT32) (((Parameters->BurstRejectLowerMax & B_SATA_CFG_SIR_A0_CWICBRLMAX) << N_SATA_CFG_SIR_A0_CWICBRLMAX) |
              ((Parameters->BurstRejectUpperMin & B_SATA_CFG_SIR_A0_CWCIBRUMIN) << N_SATA_CFG_SIR_A0_CWCIBRUMIN))
    );

  SataSirAndThenOr32 (
    SataController->PciCfgBaseAddr,
    R_SATA_CFG_SIR_D4,
    (UINT32) ~((B_SATA_CFG_SIR_D4_CIGMAX << N_SATA_CFG_SIR_D4_CIGMAX) |
              (B_SATA_CFG_SIR_D4_CIGMIN << N_SATA_CFG_SIR_D4_CIGMIN) |
              (B_SATA_CFG_SIR_D4_CWGMAX << N_SATA_CFG_SIR_D4_CWGMAX) |
              (B_SATA_CFG_SIR_D4_CWGMIN << N_SATA_CFG_SIR_D4_CWGMIN)),
    (UINT32) (((Parameters->CominitGapDetectionMax & B_SATA_CFG_SIR_D4_CIGMAX) << N_SATA_CFG_SIR_D4_CIGMAX) |
             ((Parameters->CominitGapDetectionMin & B_SATA_CFG_SIR_D4_CIGMIN) << N_SATA_CFG_SIR_D4_CIGMIN) |
             ((Parameters->ComwakeGapDetectionMax & B_SATA_CFG_SIR_D4_CWGMAX) << N_SATA_CFG_SIR_D4_CWGMAX) |
             ((Parameters->ComwakeGapDetectionMin & B_SATA_CFG_SIR_D4_CWGMIN) << N_SATA_CFG_SIR_D4_CWGMIN))
    );
}

/**
  Additional Sata configuration

  @param[in]  Sata        Pointer to SATA controller configuration structure

  @retval None
**/
VOID
ConfigureSata (
  IN  SATA_HANDLE   *Sata
  )
{
  UINT32                  DisableDynamicPowerGateBitMap;
  UINT8                   Index;
  UINT32                  Data32And;
  UINT32                  Data32Or;
  SATA_CONFIG             *SataConfig;
  SATA_CONTROLLER         *SataController;
  UINT8                   RxPolarityBitMap;

  SataConfig      = Sata->SataConfig;
  SataController  = Sata->Controller;
  DEBUG ((DEBUG_INFO, "ConfigureSata() Started (SATA controler index: %d)\n", SataController->SataCtrlIndex ));

  //
  // To enable remapping, PCIe disk must be enumerated by RST code to be visible in HII,
  // even if SATA controller must be disabled. PCIe enumeration by RST is possible only in case,
  // when SATA controller is not disabled. However, in some cases SATA Controller must be disabled,
  // and BIOS is doing that, but after PCIe drives enumeration in RST code.
  // When SATA Controller has no Cycle Routers connected to itself, then hardware remapping is impossible,
  // so BIOS could disable SATA Controller immediately
  //
  if (IsSataDisableNeeded (Sata)) {
    if (Sata->PrivateConfig->RemappingSupported) {
      //
      // Controller will be disabled, but after PCIe storage detection
      return;
    } else {
      DisablePchSataController (Sata);
      return;
    }
  }

  ///
  /// Early SATA controller contiguration
  ///
  EarlyConfigureSata (Sata);

  ///
  /// Configure AHCI
  ///
  ConfigureSataAhci (Sata);

  ///
  /// PCH BIOS Spec Section 14.1.6 Power Optimizer Configuration
  /// System BIOS must execute the following steps as part of System BIOS initialization
  /// of the PCH SATA controller on both cold boot (G3/S5) and S3/S4 resume path. Please
  /// refer to the PCH EDS, section 14.1.35.1 for the SATA initialization settings and
  /// the actual register indexes/values to be programmed.
  ///
  if (SataConfig->PwrOptEnable == TRUE) {

    //
    // When platform design has one of below condition,
    // BIOS has to set SATA PHYDPGE=0 and SQOFFIDLED=1 for specific the SATA port.
    //   - SATA hot-plug enabled port (PxCMD.HPCP = 1)
    //   - SATA external port (PxCMD.ESP = 1)
    //   - SATA slimline port with zero-power ODD (ZPODD) attached (or other AN capable ODD)
    //
    // Set PHYDPGE=0 and SQOFFIDLED=1 for selected ports.
    //
    DisableDynamicPowerGateBitMap = 0;
    RxPolarityBitMap = 0;
    for (Index = 0; Index < SataController->TotalCtrlPortNum; Index++) {
      if (Sata->PrivateConfig->SataPowerGatingSupported) {
        if (SataConfig->PortSettings[Index].Enable) {
          if (SataConfig->PortSettings[Index].External ||
            SataConfig->PortSettings[Index].HotPlug  ||
            SataConfig->PortSettings[Index].ZpOdd) {
           DisableDynamicPowerGateBitMap |= 1u << Index;
          }
        }
      } else {
        DisableDynamicPowerGateBitMap |= 1u << Index;
      }
      //
      // RxPolarity Bit Map bit0=port0... bit7=port7
      //
      if (SataConfig->PortSettings[Index].RxPolarity) {
        RxPolarityBitMap |= 1u << Index;
      }
    }

    //
    // [RxPolarity] SIR offset 80h[0:7] . bit0=port0... bit7=port7
    //
    SataSirOr32(SataController->PciCfgBaseAddr, R_SATA_CFG_SIR_80, RxPolarityBitMap);

    ///
    /// Set SATA SIR Index 90h[7:0] = 1'S
    ///
    //
    //  [MPHY Dynamic Power gating] SIR offset 90h[7:0] . bit0=port0...bit7=port7
    //
    SataSirOr32 (SataController->PciCfgBaseAddr, R_SATA_CFG_SIR_90, B_SATA_CFG_PCS_PXE_MASK & ~DisableDynamicPowerGateBitMap);

    //
    //  [Slumber Squelch off] SIR offset 80h[16:23] . bit16=port0... bit23=port7
    //
    SataSirOr32 (SataController->PciCfgBaseAddr, R_SATA_CFG_SIR_80, (DisableDynamicPowerGateBitMap << 16));

    ///
    /// Set SATA SIR Index 8Ch[23:16, 7:0] = 1'S
    ///
    SataSirOr32 (SataController->PciCfgBaseAddr, R_SATA_CFG_SIR_8C, 0xFF00FF);

    ///
    /// Set SATA SIR Index A0h
    /// SVRDCGE   [15:15] = (01) 0x01      1
    ///
    SataSirAndThenOr32 (SataController->PciCfgBaseAddr, R_SATA_CFG_SIR_A0, (UINT32) ~0, BIT15);

    ///
    /// Set SATA SIR Index 84h[23:16, 7:0] = 1'S
    ///
    SataSirOr32 (SataController->PciCfgBaseAddr, R_SATA_CFG_SIR_84, 0xFF00FF);

    ///
    /// Set SATA SIR Index A4h[14] = 1b
    ///
    SataSirOr32 (SataController->PciCfgBaseAddr, R_SATA_CFG_SIR_A4, BIT4);

    ///
    /// For PCH-H and PS_ON Enabled apply below configuration:
    ///  Set SATA SIR Index CCh[31:0] = 883C1001h
    /// Else
    ///  Set SATA SIR Index CCh[31:0] = 883C9001h
    ///
    if (Sata->PrivateConfig->SataPsOnSupported) {
      SataSirWrite32 (SataController->PciCfgBaseAddr, R_SATA_CFG_SIR_CC, 0x883C1001);
    } else {
      SataSirWrite32 (SataController->PciCfgBaseAddr, R_SATA_CFG_SIR_CC, 0x883C9001);
    }

    ///
    /// Set SATA SIR Index D0h[15:0] = 880Ah
    ///
    SataSirAndThenOr32 (SataController->PciCfgBaseAddr, R_SATA_CFG_SIR_D0, 0xFFFF0000, 0x0000880A);

    ///
    /// Set SATA SIR Index C8h[3] = 1b
    ///
    SataSirAndThenOr32 (SataController->PciCfgBaseAddr, R_SATA_CFG_SIR_C8, 0xFFFFFFF7, 0x00000008);

    ///
    /// Set SATA SIR Index C8h[0] = 1b
    ///
    Data32And = 0xFFFFFFFE;
    Data32Or  = 0x00000001;
    SataSirAndThenOr32 (SataController->PciCfgBaseAddr, R_SATA_CFG_SIR_C8, Data32And, Data32Or);
  ///
  /// Set SATA SIR Index C8h[1] = 1b
  ///
  SataSirAndThenOr32 (SataController->PciCfgBaseAddr, R_SATA_CFG_SIR_C8, 0xFFFFFFFD, 0x00000002);
  }

  if (Sata->PrivateConfig->SataPowerGatingSupported) {
    //
    // Enable SATA PGD dynamic power gating and allow SATA PGD power down on
    // Hardware autonomous and on D3Hot entry.
    //
    SataSirOr32 (SataController->PciCfgBaseAddr, R_SATA_CFG_SIR_A8, (BIT18 | BIT17 | BIT16));
  }

  SataConfigureCominitComwake (Sata);

  ConfigureSataControllerMode (Sata);

  ConfigureSataInterrupts (Sata);

  //
  // Sata throttling actions
  //
  SataThrottlingConfiguration (SataController, SataConfig);

  DEBUG ((DEBUG_INFO, "ConfigureSata() Ended (SATA controler index: %d)\n", SataController->SataCtrlIndex ));
}

