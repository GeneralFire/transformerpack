/** @file
  The IOE Root Port controller SoC specific implementation.

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
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PchPcieRpSocLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PchPolicyLib.h>
#include <Library/PchIoExpander.h>
#include <Library/PcieRpHandleLib.h>
#include <Library/PeiEnterprisePcieRpInitLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/TimerLib.h>
#include <Register/EnterprisePcieRpRegs.h>
#include <PcieRegs.h>
#include <PchLimits.h>
#include <PchPcieRpInfo.h>

#define LINK_ACTIVE_POLL_INTERVAL 100     // in microseconds
#define LINK_ACTIVE_POLL_TIMEOUT  1000000 // in microseconds

/**
  Get IOE PCIE RP handle

  @param[in     ] SiPreMemPolicyPpi   The SI PreMem Policy PPI
  @param[in     ] SiPolicy            The SI Policy PPI
  @param[in     ] IoeId               IOE index - starting from 1
  @param[in, out] RpHandle            Root Port device handle
  @param[in, out] RpPrivate           Pointer to PCIE RP private structure
  @param[in, out] RpDevice            Pointer to PCIE RP device structure
  @param[in, out] RpCallback          Pointer to PCIE RP callback structure

  @retval     EFI_SUCCESS           - Completted successfully
              EFI_INVALID_PARAMETER - PchId is out of range
**/
VOID
IoeRpHandleInit (
  IN      SI_PREMEM_POLICY_PPI  *SiPreMemPolicyPpi,
  IN      SI_POLICY_PPI         *SiPolicy,
  IN      UINT32                IoeId,
  IN      UINT32                RplIndex,
  IN OUT  PCH_RP_HANDLE         *RpHandle,
  IN OUT  PCH_RP_PRIVATE_CONFIG *RpPrivate,
  IN OUT  PCH_RP_DEVICE         *RpDevice,
  IN OUT  PCH_RP_CALLBACK       *RpCallback
  )
{
  EFI_STATUS            Status;

  if (IoeId >= IO_EXPANDER_MAX) {
    ASSERT (FALSE);
    return;
  }

  ZeroMem (RpHandle, sizeof (PCH_RP_HANDLE));
  ZeroMem (RpCallback, sizeof (PCH_RP_CALLBACK));
  ZeroMem (RpPrivate, sizeof (PCH_RP_PRIVATE_CONFIG));

  IoeGetRpDevice (IoeId, RplIndex, RpDevice);

  RpHandle->Device        = RpDevice;
  RpHandle->PrivateConfig = RpPrivate;
  RpHandle->Callback      = RpCallback;

  PchPcieRpAssignCallbacks (RpCallback);

  // Override I/O Expander callback for now
  RpCallback->PchRpIsPcieRootPortLaneConnected = NULL;
  RpCallback->PchRpIoPort80DecodeSet = NULL;
  RpCallback->PchRpPcrAndThenOr16 = NULL;
  RpCallback->PchRpPcrRead32 = NULL;
  RpCallback->PchRpPmcDisablePcieRootPort = NULL;
  RpCallback->PchRpPsfDisablePcieRootPort = NULL;
  RpCallback->PchRpSbiExecution = NULL;
  RpCallback->PchRpSbiExecutionEx = NULL;

  if (SiPolicy) {
    Status = GetConfigBlock ((VOID *) SiPolicy, &gPchPcieConfigGuid, (VOID *) &RpHandle->PciExpressConfig);
    ASSERT_EFI_ERROR (Status);
  }

  if (SiPreMemPolicyPpi) {
    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gPcieRpPreMemConfigGuid, (VOID *) &RpHandle->PcieRpPreMemConfig);
    ASSERT_EFI_ERROR (Status);

    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gPcieLinkPrememConfigGuid, (VOID *) &RpHandle->PcieRpLinkEqPreMemConfig);
    ASSERT_EFI_ERROR (Status);
  }
}


/**
  Set bit in provided disable mask based on root port enabling/disabling

  @param[in     ] RpHandle            Root Port device handle
  @param[in     ] RpCtrlHandle        Pointer to handle for first PCIE RP in controller
  @param[in, out] RpDisableMask       Root Ports disable mask
**/
STATIC
VOID
PcieRpGetDisableMaskBitForPort (
  IN     PCH_RP_HANDLE       *RpHandle,
  IN     PCH_RP_HANDLE       *RpCtrlHandle,
  IN OUT UINT32              *RpDisableMask
  )
{
  UINT8                      TempPciBusMin;
  UINT8                      TempPciBusMax;
  PCH_PCIE_CONFIG            *PciExpressConfig;
  UINT32                     Timer;
  PCH_RP_DEVICE              *RpDevice;
  UINT32                     RpIndex;
  UINT64                     RpBase;
  UINT32                     DetectTimeoutUs;

  RpDevice = RpHandle->Device;
  PciExpressConfig = RpHandle->PciExpressConfig;

  TempPciBusMin = RpDevice->TempPciBusMin;
  TempPciBusMax = RpDevice->TempPciBusMax;

  RpIndex = RpDevice->RootPortIndex;
  RpBase = RpDevice->PciCfgBaseAddr;

  Timer            = 0;

  //
  // Determine available ports based on lane ownership and port configuration (x1/x2/x4)
  // Root ports can be already disabled by PchEarlyDisabledDeviceHandling
  //
  if (PciSegmentRead16 (RpBase) == 0xFFFF) {
    DEBUG ((DEBUG_INFO, "PCIEe RP Index %d device not available.\n", RpIndex));
    *RpDisableMask |= (BIT0 << RpIndex);
    return;
  } else if (!IsPcieRootPortWired (RpHandle, RpCtrlHandle)) {
    DEBUG ((DEBUG_INFO, "PCIe RP Index %d NOT owns lanes.\n", RpIndex));
    *RpDisableMask |= (BIT0 << RpIndex);
    return;
  }

  ///
  /// Set the Slot Implemented Bit.
  /// PCH BIOS Spec section 8.2.3, The System BIOS must
  /// initialize the "Slot Implemented" bit of the PCI Express* Capabilities Register,
  /// XCAP Dxx:Fn:42h[8] of each available and enabled downstream root port.
  /// Ports with hotplug capability must have SI bit set
  /// The register is write-once so must be written even if we're not going to set SI, in order to lock it.
  ///
  /// This must happen before code reads PresenceDetectState, because PDS is invalid unless SI is set
  ///
  if (PciExpressConfig->RootPort[RpIndex].PcieRpCommonConfig.SlotImplemented) {
    PciSegmentOr16 (RpBase + R_PCH_PCIE_CFG_EXPCAP, B_PCIE_XCAP_SI);
  } else {
    PciSegmentAnd16 (RpBase + R_PCH_PCIE_CFG_EXPCAP, (UINT16)(~B_PCIE_XCAP_SI));
  }

  ///
  /// For non-hotplug ports disable the port if there is no device present.
  ///
  DetectTimeoutUs = PciExpressConfig->RootPort[RpIndex].PcieRpCommonConfig.DetectTimeoutMs * 1000;
  if (IsPcieDevicePresent (RpBase, DetectTimeoutUs, &Timer)) {
    DEBUG ((DEBUG_INFO, "Port %d has a device attached.\n", RpIndex));
  } else {
    if (PciExpressConfig->RootPort[RpIndex].PcieRpCommonConfig.HotPlug == FALSE) {
      *RpDisableMask |= (BIT0 << RpIndex);
    }
  }
}

/**
  Enable Power gating for root port

  @param[in     ] RpHandle            Root Port device handle
**/
STATIC
VOID
PcieRpPowerGatingForPort (
  IN      PCH_RP_HANDLE       *RpHandle
  )
{
  ///
  /// Configure power management applicable to all port including disabled ports.
  ///
    PchPcieEnableRootPortClockGating (
      RpHandle
      );
}


/**
  Configure PCIe Grant Counts

  @param[in     ] SiPolicy            The SI Policy PPI
  @param[in     ] IoeId               IOE index - starting from 1
**/
VOID
IoePcieRpConfigureGrantCounts (
  IN  SI_POLICY_PPI  *SiPolicy,
  IN  UINT32         IoeId
  )
{
  UINT32                   ControllerMax;
  UINT32                   Controller;
  PSF_PCIE_CTRL_CONFIG     PsfPcieCtrlConfigTable[PCH_MAX_PCIE_CONTROLLERS];
  PCH_RP_HANDLE            RpHandle;
  PCH_RP_PRIVATE_CONFIG    RpPrivate;
  PCH_RP_DEVICE            RpDevice;
  PCH_RP_CALLBACK          RpCallback;

  ControllerMax = GetPchMaxPcieControllerNum ();

  ZeroMem (&RpCallback, sizeof (PCH_RP_CALLBACK));
  for (Controller = 0; Controller < ControllerMax; Controller++) {
    IoeRpHandleInit (NULL, SiPolicy, IoeId, Controller * PCH_PCIE_CONTROLLER_PORTS, &RpHandle, &RpPrivate, &RpDevice, &RpCallback);
    PcieRpGetPsfConfig (&RpHandle, &PsfPcieCtrlConfigTable[Controller]);
  }

  //PsfConfigurePcieGrantCounts (PsfPcieCtrlConfigTable, ControllerMax);
}


/**
  Perform Initialization of the Downstream Root Ports.

  @param[in     ] SiPolicy            The SI Policy PPI
  @param[in     ] IoeId               IOE index - starting from 1
**/
VOID
IoePcieRpInit (
  IN  SI_POLICY_PPI  *SiPolicy,
  IN  UINT32          IoeId
  )
{
  EFI_STATUS                          Status;
  PCH_RP_HANDLE                       RpHandle[PCH_MAX_PCIE_ROOT_PORTS];
  PCH_RP_PRIVATE_CONFIG               RpPrivate[PCH_MAX_PCIE_ROOT_PORTS];
  PCH_RP_DEVICE                       RpDevice[PCH_MAX_PCIE_ROOT_PORTS];
  PCH_RP_CALLBACK                     RpCallback[PCH_MAX_PCIE_ROOT_PORTS];
  SI_PREMEM_POLICY_PPI                *SiPreMemPolicy;
  UINT32                              RpIndex;
  UINT32                              RpDisableMask;
  UINT32                              MaxRootPorts;
  UINT8                               CtrlIndex;
  UINT32                              PortMask;
  UINT32                              Time;

  //
  // Get Policy settings through the SiPreMemPolicy PPI
  //
  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicy
             );
  if (Status != EFI_SUCCESS) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  MaxRootPorts = GetPchMaxPciePortNum ();
  for (RpIndex = 0; RpIndex < MaxRootPorts; RpIndex++) {
    IoeRpHandleInit (SiPreMemPolicy, SiPolicy, IoeId, RpIndex, &RpHandle[RpIndex], &RpPrivate[RpIndex], &RpDevice[RpIndex], &RpCallback[RpIndex]);
  }

  for (CtrlIndex = 0; CtrlIndex < GetPchMaxPcieControllerNum (); CtrlIndex++) {
    RpIndex = CtrlIndex * PCH_PCIE_CONTROLLER_PORTS;
    //
    // Bifurcation has to be done as a first step in init flow
    // since all PCI config accesses are blocked before bifurcation
    // is done. This will also prevent root ports from training link in
    // wrong link configuration.
    //
    PcieBifurcationInit (&RpHandle[RpIndex]);

    //
    // Disable RPs of PCIe controller based on programmed bifurcation
    //
    DEBUG ((DEBUG_INFO, "Multilane check for PCIE Controller %d: ", CtrlIndex));
    switch (RpHandle[RpIndex].PcieRpPreMemConfig->Bifurcation[CtrlIndex]) {
      case PcieBifurcation1x4_2x2:
      case PcieBifurcation1x2_2x1:
        //
        // Disable port 2 of a controller when it's set to 1x4 - 2x2 or 1x2 - 2x1
        //
        DEBUG ((DEBUG_INFO, "Disabling RP %d\n", RpIndex + 1));
        //PsfDisablePcieRootPort (RpHandle[RpIndex].RootPortIndex + 1);
        break;
      case PcieBifurcation2x2_1x4:
        //
        // Disable port 4 of a controller when it's set to 2x2 - 1x4
        //
        DEBUG ((DEBUG_INFO, "Disabling RP %d\n", RpIndex + 3));
        //PsfDisablePcieRootPort (RpHandle[RpIndex].RootPortIndex + 3);
        break;
      case PcieBifurcation2x4:
      case PcieBifurcation2x2:
        //
        // Disable ports 2, 4 of a controller when it's set to 2x4 or 2x2
        //
        DEBUG ((DEBUG_INFO, "Disabling RP %d/%d\n", RpIndex + 1, RpIndex + 3));
        //PsfDisablePcieRootPort (RpHandle[RpIndex].RootPortIndex + 1);
        //PsfDisablePcieRootPort (RpHandle[RpIndex].RootPortIndex + 3);
        break;
      case PcieBifurcation1x8:
      case PcieBifurcation1x4:
        //
        // Disable ports 2, 3, 4 of a controller when it's set to 1x8 or 1x4
        //
        DEBUG ((DEBUG_INFO, "Disabling RP %d/%d/%d\n", RpIndex + 1, RpIndex + 2, RpIndex + 3));
        //PsfDisablePcieRootPort (RpHandle[RpIndex].RootPortIndex + 1);
        //PsfDisablePcieRootPort (RpHandle[RpIndex].RootPortIndex + 2);
        //PsfDisablePcieRootPort (RpHandle[RpIndex].RootPortIndex + 3);
        break;
      case PcieBifurcation4x2:
      case PcieBifurcation4x1:
        //
        // No ports of a controller to disable when it's set to 4x2 or 4x1
        //
      default:
        DEBUG ((DEBUG_INFO, "No ports disabled\n"));
        break;
    }
  }
  //
  // Delay for 1 us after bifurcation is done.
  //
  MicroSecondDelay (1);

  for (RpIndex = 0; RpIndex < MaxRootPorts; RpIndex++) {
    PcieRpSetupLink (&RpHandle[RpIndex], RpHandle[RpIndex].PcieRpLinkEqPreMemConfig, RpHandle[RpIndex].PcieRpPreMemConfig->RpEnabledMask);
  }


  //PchFiaSetClockOutputDelay ();

  RpDisableMask = 0;
  for (RpIndex = 0; RpIndex < MaxRootPorts; RpIndex++) {
    PcieRpGetDisableMaskBitForPort (&RpHandle[RpIndex], &RpHandle[RpDevice[RpIndex].CtrlIndex * PCH_PCIE_CONTROLLER_PORTS], &RpDisableMask);
  }

  for (RpIndex = 0; RpIndex < MaxRootPorts; RpIndex++) {
    if ((RpIndex % PCH_PCIE_CONTROLLER_PORTS) == 0) {
      PcieRpPowerGatingForPort (&RpHandle[RpIndex]);
    }
  }


  /// Some PCIe devices may take long time between they become detected and form a link.
  /// This function waits until all enabled, non-empty rootports form a link or until a timeout
  PortMask = (0x1 << MaxRootPorts) - 1;
  PortMask &= ~RpDisableMask;
  Time = 0;
  while (Time < LINK_ACTIVE_POLL_TIMEOUT) {
    for (RpIndex = 0; RpIndex < MaxRootPorts; RpIndex ++) {
      if (!(PortMask & (BIT0 << RpIndex))) {
        continue;
      }
      //
      // if PDS is not set or if LA is set then this rootport is done - clear it from mask
      //
      if (!IsEndpointConnected (RpDevice[RpIndex].PciCfgBaseAddr) ||
           PchPcieIsLinkActive (RpDevice[RpIndex].PciCfgBaseAddr)) {
        PortMask &= ~ (BIT0 << RpIndex);
      }
    }
    if (PortMask == 0x0) {
      DEBUG ((DEBUG_INFO, "WaitForLinkActive, all RPs done, lost %dms waiting\n", Time / 1000));
      break;
    }
    MicroSecondDelay (LINK_ACTIVE_POLL_INTERVAL);
    Time += LINK_ACTIVE_POLL_INTERVAL;
  }


  for (CtrlIndex = 0; CtrlIndex < GetPchMaxPcieControllerNum (); CtrlIndex++) {
    PcieRpProgramPtm (&RpHandle[CtrlIndex * PCH_PCIE_CONTROLLER_PORTS]);
  }

  for (RpIndex = 0; RpIndex < MaxRootPorts; ++RpIndex) {
    if (RpDisableMask & (BIT0 << RpIndex)) {
        DisablePcieRootPort (&RpHandle[RpIndex]);
    } else {
      PcieRpDownstreamRootPortInit (&RpHandle[RpIndex]);
    }
  }
}
