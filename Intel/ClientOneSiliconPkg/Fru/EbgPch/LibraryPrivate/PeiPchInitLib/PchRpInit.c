/** @file
  The PCH Root Port controller SoC specific implementation.

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
#include <Library/PchInfoLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PchPcieRpSocLib.h>
#include <Library/PcieRpLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PchPolicyLib.h>
#include <Library/PchFiaLib.h>
#include <Library/PchDmiLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/PmcSocLib.h>
#include <Library/PeiPchPcieClocksLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PeiEnterprisePcieRpInitLib.h>
#include <Library/TimerLib.h>
#include <Library/IoLib.h>
#include <Library/PciExpressHelpersLib.h>
#include <Register/EnterprisePcieRpRegs.h>
#include <PcieRegs.h>
#include <PchLimits.h>
#include <PchPcieRpInfo.h>

/**
  Execute PCH SBI message
  Take care of that there is no lock protection when using SBI programming in both POST time and SMI.
  It will clash with POST time SBI programming when SMI happen.
  Programmer MUST do the save and restore opration while using the PchSbiExecution inside SMI
  to prevent from racing condition.
  This function will reveal P2SB and hide P2SB if it's originally hidden. If more than one SBI access
  needed, it's better to unhide the P2SB before calling and hide it back after done.

  When the return value is "EFI_SUCCESS", the "Response" do not need to be checked as it would have been
  SBI_SUCCESS. If the return value is "EFI_DEVICE_ERROR", then this would provide additional information
  when needed.

  @param[in] RpDevice                   Pointer to PCIE RP device
  @param[in] Pid                        Port ID of the SBI message
  @param[in] Offset                     Offset of the SBI message
  @param[in] Opcode                     Opcode
  @param[in] Posted                     Posted message
  @param[in, out] Data32                Read/Write data
  @param[out] Response                  Response

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_DEVICE_ERROR              Transaction fail
  @retval EFI_INVALID_PARAMETER         Invalid parameter
  @retval EFI_TIMEOUT                   Timeout while waiting for response
**/
STATIC
EFI_STATUS
PchRpSbiExecution (
  IN     PCH_RP_DEVICE      *RpDevice,
  IN     PCH_SBI_PID        Pid,
  IN     UINT64             Offset,
  IN     PCH_SBI_OPCODE     Opcode,
  IN     BOOLEAN            Posted,
  IN OUT UINT32             *Data32,
  OUT    UINT8              *Response
  )
{
  EFI_STATUS Status;

  if (RpDevice == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = PchSbiExecution (Pid, Offset, Opcode, Posted, Data32, Response);

  return Status;
}

/**
  Disable PCIe Root Port at PSF level

  @param[in] RpDevice            Pointer to PCIE RP device
  @retval     EFI_SUCCESS           - Completted successfully
              EFI_INVALID_PARAMETER - RpDevice is NULL
**/
STATIC
EFI_STATUS
PchRpPsfDisablePcieRootPort (
  IN  PCH_RP_DEVICE         *RpDevice
  )
{
  if (RpDevice == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  PsfDisablePcieRootPort (RpDevice->RootPortIndex);

  return EFI_SUCCESS;
}

/**
  Full function for executing PCH SBI message
  Take care of that there is no lock protection when using SBI programming in both POST time and SMI.
  It will clash with POST time SBI programming when SMI happen.
  Programmer MUST do the save and restore opration while using the PchSbiExecution inside SMI
  to prevent from racing condition.
  This function will reveal P2SB and hide P2SB if it's originally hidden. If more than one SBI access
  needed, it's better to unhide the P2SB before calling and hide it back after done.

  When the return value is "EFI_SUCCESS", the "Response" do not need to be checked as it would have been
  SBI_SUCCESS. If the return value is "EFI_DEVICE_ERROR", then this would provide additional information
  when needed.

  @param[in] RpDevice                   Pointer to PCIE RP device
  @param[in] Pid                        Port ID of the SBI message
  @param[in] Offset                     Offset of the SBI message
  @param[in] Opcode                     Opcode
  @param[in] Posted                     Posted message
  @param[in] Fbe                        First byte enable
  @param[in] Bar                        Bar
  @param[in] Fid                        Function ID
  @param[in, out] Data32                Read/Write data
  @param[out] Response                  Response

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_DEVICE_ERROR              Transaction fail
  @retval EFI_INVALID_PARAMETER         Invalid parameter
  @retval EFI_TIMEOUT                   Timeout while waiting for response
**/
STATIC
EFI_STATUS
PchRpSbiExecutionEx (
  IN     PCH_RP_DEVICE      *RpDevice,
  IN     PCH_SBI_PID        Pid,
  IN     UINT64             Offset,
  IN     PCH_SBI_OPCODE     Opcode,
  IN     BOOLEAN            Posted,
  IN     UINT16             Fbe,
  IN     UINT16             Bar,
  IN     UINT16             Fid,
  IN OUT UINT32             *Data32,
  OUT    UINT8              *Response
  )
{
  EFI_STATUS Status;

  if (RpDevice == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = PchSbiExecutionEx (Pid, Offset, Opcode, Posted, Fbe, Bar, Fid, Data32, Response);

  return Status;
}

/**
  Checks if a given PCIe lane is assigned any FIA lane

  @param[in]  RpDevice          Pointer to PCIE RP device
  @param[in]  PciePhysicalLane  Physical lane number

  @return  TRUE if given PCIe lane is assigned a FIA lane, FALSE otherwise
**/
STATIC
BOOLEAN
PchRpIsPcieRootPortLaneConnected (
  IN  PCH_RP_DEVICE         *RpDevice,
  IN  UINT32                PciePhysicalLane
  )
{
  BOOLEAN Status;
  UINT8  LaneNum;

  if (RpDevice == NULL) {
    return FALSE;
  }

  Status = PchFiaGetPcieLaneNum (PciePhysicalLane, &LaneNum);

  return Status;
}

/**
  Read PCR register.
  It returns PCR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  RpDevice Pointer to PCIE RP device
  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of this Port ID
**/
STATIC
UINT32
PchRpPcrRead32 (
  IN  PCH_RP_DEVICE         *RpDevice,
  IN  PCH_SBI_PID           Pid,
  IN  UINT32                Offset
  )
{
  UINT32 Value;

  if (RpDevice == NULL) {
    return 0;
  }

  Value = PchPcrRead32 (Pid, Offset);

  return Value;
}

/**
  This function disables (non-static power gating) PCIe Root Port and enables ModPHY SPD gating (PCH-LP only).

  @param[in] RpDevice            Pointer to PCIE RP device
  @retval     EFI_SUCCESS           - Completted successfully
              EFI_INVALID_PARAMETER - RpDevice is NULL
**/
STATIC
EFI_STATUS
PchRpPmcDisablePcieRootPort (
  IN  PCH_RP_DEVICE         *RpDevice
  )
{
  if (RpDevice == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  PmcDisablePcieRootPort (RpDevice->RootPortIndex);

  return EFI_SUCCESS;
}

/**
  Write PCR register.
  It programs PCR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  RpDevice Pointer to PCIE RP device
  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of Port ID.
  @param[in]  AndData  AND Data. Must be the same size as Size parameter.
  @param[in]  OrData   OR Data. Must be the same size as Size parameter.

  @retval UINT16       Value written to register
**/
STATIC
UINT16
PchRpPcrAndThenOr16 (
  IN  PCH_RP_DEVICE         *RpDevice,
  IN  PCH_SBI_PID           Pid,
  IN  UINT32                Offset,
  IN  UINT16                AndData,
  IN  UINT16                OrData
  )
{
  UINT16 Value;

  if (RpDevice == NULL) {
    return 0;
  }

  Value = PchPcrAndThenOr16 (Pid, Offset, AndData, OrData);

  return Value;
}

/**
  Set PCH IO port 80h cycle decoding to PCIE root port.
  System BIOS is likely to do this very soon after reset before PCI bus enumeration.
  This cycle decoding is allowed to set when DMI is unlocked

  @param[in] RpDevice               Pointer to PCIE RP device
  @retval     EFI_SUCCESS           - Completted successfully
              EFI_INVALID_PARAMETER - RpDevice is NULL
**/
STATIC
EFI_STATUS
PchRpIoPort80DecodeSet (
  IN  PCH_RP_DEVICE         *RpDevice
  )
{
  EFI_STATUS Status;

  if (RpDevice == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = PchIoPort80DecodeSet (RpDevice->RootPortIndex);

  return Status;
}


/**
  Assign callback functions

  @param[in] RpCallback          Pointer to PCIE RP callback structure
 **/
VOID
PchPcieRpAssignCallbacks (
  IN  PCH_RP_CALLBACK       *RpCallback
  )
{
  RpCallback->PchRpIsPcieRootPortLaneConnected = PchRpIsPcieRootPortLaneConnected;
  RpCallback->PchRpIoPort80DecodeSet = PchRpIoPort80DecodeSet;
  RpCallback->PchRpPcrAndThenOr16 = PchRpPcrAndThenOr16;
  RpCallback->PchRpPcrRead32 = PchRpPcrRead32;
  RpCallback->PchRpPmcDisablePcieRootPort = PchRpPmcDisablePcieRootPort;
  RpCallback->PchRpPsfDisablePcieRootPort = PchRpPsfDisablePcieRootPort;
  RpCallback->PchRpSbiExecution = PchRpSbiExecution;
  RpCallback->PchRpSbiExecutionEx = PchRpSbiExecutionEx;
}

/**
  Get PCIE RP handle

  @param[in     ] SiPreMemPolicyPpi   The SI PreMem Policy PPI
  @param[in     ] SiPolicy            The SI Policy PPI
  @param[in     ] RpIndex             PCIE RP index
  @param[in, out] RpHandle            Root Port device handle
  @param[in, out] RpPrivate           Pointer to PCIE RP private structure
  @param[in, out] RpDevice            Pointer to PCIE RP device structure
  @param[in, out] RpCallback          Pointer to PCIE RP callback structure
 **/
VOID
PchRpHandleInit (
  IN       SI_PREMEM_POLICY_PPI  *SiPreMemPolicyPpi,
  IN CONST SI_POLICY_PPI         *SiPolicy,
  IN       UINT32                RpIndex,
  IN OUT   PCH_RP_HANDLE         *RpHandle,
  IN OUT   PCH_RP_PRIVATE_CONFIG *RpPrivate,
  IN OUT   PCH_RP_DEVICE         *RpDevice,
  IN OUT   PCH_RP_CALLBACK       *RpCallback
  )
{
  EFI_STATUS            Status;

  ZeroMem (RpHandle, sizeof (PCH_RP_HANDLE));
  ZeroMem (RpCallback, sizeof (PCH_RP_CALLBACK));
  ZeroMem (RpPrivate, sizeof (PCH_RP_PRIVATE_CONFIG));

  PchGetRpDevice (RpIndex, RpDevice);
  RpHandle->Device        = RpDevice;
  RpHandle->PrivateConfig = RpPrivate;
  RpHandle->Callback      = RpCallback;

  PchPcieRpAssignCallbacks (RpCallback);

  if (SiPolicy) {
    Status = GetConfigBlock ((VOID *) SiPolicy, &gPchPcieConfigGuid, (VOID **) &RpHandle->PciExpressConfig);
    ASSERT_EFI_ERROR (Status);
  }

  if (SiPreMemPolicyPpi) {
    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gPcieRpPreMemConfigGuid, (VOID **) &RpHandle->PcieRpPreMemConfig);
    ASSERT_EFI_ERROR (Status);

    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gPcieLinkPrememConfigGuid, (VOID **) &RpHandle->PcieRpLinkEqPreMemConfig);
    ASSERT_EFI_ERROR (Status);
  }
}


/**
  Perform Initialization of the Downstream Root Ports.

  @param[in] SiPolicy             The SI Policy PPI
**/

VOID
PcieRpInit (
  IN CONST SI_POLICY_PPI     *SiPolicy
  )
{
  PCH_PCIE_CONFIG            *PciExpressConfig;
  EFI_STATUS                 Status;
  UINT8                      RpIndex;
  UINT64                     RpBase;
  UINT8                      MaxPciePortNum;
  UINT32                     RpDisableMask;
  UINT32                     RpClkreqMask;
  UINT32                     Timer;
  UINT32                     DetectTimeoutUs;
  UINT8                      CtrlIndex;
  PCH_RP_HANDLE              RpHandle[PCH_MAX_PCIE_ROOT_PORTS];
  PCH_RP_PRIVATE_CONFIG      RpPrivate[PCH_MAX_PCIE_ROOT_PORTS];
  PCH_RP_DEVICE              RpDevice[PCH_MAX_PCIE_ROOT_PORTS];
  PCH_RP_CALLBACK            RpCallback[PCH_MAX_PCIE_ROOT_PORTS];

  DEBUG ((DEBUG_INFO, "PcieRpInit() Start\n"));

  Timer            = 0;
  MaxPciePortNum   = GetPchMaxPciePortNum ();
  RpDisableMask    = 0;
  RpClkreqMask     = 0;

  for (RpIndex = 0; RpIndex < MaxPciePortNum; RpIndex++) {
    PchRpHandleInit (NULL, SiPolicy, RpIndex, &RpHandle[RpIndex], &RpPrivate[RpIndex], &RpDevice[RpIndex], &RpCallback[RpIndex]);
  }

  PciExpressConfig = RpHandle[0].PciExpressConfig;

  PchFiaSetClockOutputDelay ();

  for (RpIndex = 0; RpIndex < MaxPciePortNum; RpIndex++) {
    RpBase = RpDevice[RpIndex].PciCfgBaseAddr;

    //
    // Enable CLKREQ# regardless of port being available/enabled to allow clock gating.
    //
    if (IsClkReqAssigned (PchClockUsagePchPcie0 + RpIndex)) {
      RpClkreqMask |= (BIT0 << RpIndex);
    }

    //
    // Determine available ports based on lane ownership and port configuration (x1/x2/x4)
    // Root ports can be already disabled by PchEarlyDisabledDeviceHandling
    //
    if (PciSegmentRead16 (RpBase) == 0xFFFF) {
      DEBUG ((DEBUG_INFO, "PCIEe RP Index %d device not available.\n", RpIndex));
      RpDisableMask |= (BIT0 << RpIndex);
      continue;
    } else if (!IsPcieRootPortWired (&RpHandle[RpIndex], &RpHandle[RpDevice[RpIndex].CtrlIndex * PCH_PCIE_CONTROLLER_PORTS])) {
      DEBUG ((DEBUG_INFO, "PCIe RP Index %d NOT owns lanes.\n", RpIndex));
      RpDisableMask |= (BIT0 << RpIndex);
      continue;
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
      //
      // At this point in boot, CLKREQ pad is still configured as GP input and doesnt' block clock generation
      // regardless of input state. Before switching it to native mode when it will start gating clock, we
      // verify if CLKREQ is really connected. If not, pad will not switch and power management
      // will be disabled in rootport.
      // By the time this code runs device can't have CPM or L1 substates enabled, so it is guaranteed to pull ClkReq down.
      // If ClkReq is detected to be high anyway, it means ClkReq is not connected correctly.
      // Checking pad's input value is primarily a measure to prevent problems with long cards inserted into short
      // open-ended PCIe slots on motherboards which route PRSNT signal to CLKREQ. Such config causes CLKREQ signal to float.
      //
      if (!PcieDetectClkreq (RpIndex, PciExpressConfig)) {
        RpClkreqMask &= ~(BIT0 << RpIndex);
      }
    } else {
      if (PciExpressConfig->RootPort[RpIndex].PcieRpCommonConfig.HotPlug == FALSE) {
        RpDisableMask |= (BIT0 << RpIndex);
      }
    }
  }


  for (RpIndex = 0; RpIndex < MaxPciePortNum; RpIndex++) {
    if (RpClkreqMask & (BIT0 << RpIndex)) {
      //
      // Enabled CLKREQ# pad if supported to allow clock gating regardless of port being enabled.
      //
      EnableClkReq (PchClockUsagePchPcie0 + RpIndex);
    }

    ///
    /// Configure power management applicable to all port including disabled ports.
    ///
      if ((RpIndex % PCH_PCIE_CONTROLLER_PORTS) == 0) {
        PchPcieEnableRootPortClockGating (
          &RpHandle[RpIndex]
          );
      }
  }


  //
  // Wait for all ports with PresenceDetect=1 to form a link
  // Having an active link is necessary to access and configure the endpoint
  // We cannot use results of IsPcieDevicePresent() because it checks PDS only and may include
  // PCIe cards that never form a link, such as compliance load boards.
  //
  WaitForLinkActive (MaxPciePortNum, RpDisableMask);

  for (CtrlIndex = 0; CtrlIndex < GetPchMaxPcieControllerNum (); CtrlIndex++) {
    PcieRpProgramPtm (&RpHandle[CtrlIndex * PCH_PCIE_CONTROLLER_PORTS]);
  }

  for (RpIndex = 0; RpIndex < MaxPciePortNum; ++RpIndex) {
    if (RpDisableMask & (BIT0 << RpIndex)) {
        DisablePcieRootPort (&RpHandle[RpIndex]);
    } else {
      PcieRpDownstreamRootPortInit (&RpHandle[RpIndex]);
    }
  }

  ///
  /// Clear GPE0 Register PCI_EXP_STS and HOT_PLUG_STS by writing 1
  ///
  IoWrite32 (
    PmcGetAcpiBase () + R_ACPI_IO_GPE0_STS_127_96,
    B_ACPI_IO_GPE0_STS_127_96_PCI_EXP | B_ACPI_IO_GPE0_STS_127_96_HOT_PLUG
    );

  ///
  /// If SCI is enabled in any port, Set BIOS_PCI_EXP_EN bit, PMC PCI offset A0h[10],
  /// to globally enable the setting of the PCI_EXP_STS bit by a PCI Express* PME event.
  ///
  for (RpIndex = 0; RpIndex < MaxPciePortNum; RpIndex++) {
    if (PciExpressConfig->RootPort[RpIndex].PcieRpCommonConfig.PmSci) {
      PmcEnablePciExpressPmeEvents ();
      break;
    }
  }

  ///
  /// PCH BIOS Spec Section 8.2.9
  /// Enable PCIe Relaxed Order to always allow downstream completions to pass posted writes.
  /// To enable this feature configure DMI and PSF:
  ///
  PchDmiEnablePcieRelaxedOrder ();

  //
  // Program the root port target link speed based on policy.
  //
  Status = PcieRpSpeedChange (PciExpressConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "PcieRpInit() End\n"));
}


/**
  Configure PCIe Grant Counts

  @param[in] SiPolicy             The SI Policy PPI
**/
VOID
PcieRpConfigureGrantCounts (
  IN CONST SI_POLICY_PPI     *SiPolicy
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

  for (Controller = 0; Controller < ControllerMax; Controller++) {
    PchRpHandleInit (NULL, SiPolicy, Controller * PCH_PCIE_CONTROLLER_PORTS, &RpHandle, &RpPrivate, &RpDevice, &RpCallback);
    PcieRpGetPsfConfig (&RpHandle, &PsfPcieCtrlConfigTable[Controller]);
  }

  PsfConfigurePcieGrantCounts (PsfPcieCtrlConfigTable, ControllerMax);
}

/**
  Disable unused PCIe Ports

**/
VOID
PchPcieRpDisableUnusedPorts (
  VOID
  )
{
  UINT32                    RpIndex;
  PCH_RP_HANDLE             RpHandle;
  PCH_RP_PRIVATE_CONFIG     RpPrivate;
  PCH_RP_DEVICE             RpDevice;
  PCH_RP_CALLBACK           RpCallback;

  //
  // Get PCIE Port configuration and disable value for each RP
  // and perform PSF function disable
  //
  for (RpIndex = 0; RpIndex < GetPchMaxPciePortNum (); RpIndex++) {
    PchRpHandleInit (NULL, NULL, RpIndex, &RpHandle, &RpPrivate, &RpDevice, &RpCallback);
    PcieDisableUnusedPort (&RpHandle);
  }
}

/**
  Initializaes root ports in pre-mem. At this point we are responsible for transitioning
  all active root ports with devices connected to them into link active.

  @param[in] SiPreMemPolicyPpi  Pointer to pre mem policy PPI
**/
VOID
PcieRpInitPreMem (
  IN SI_PREMEM_POLICY_PPI  *SiPreMemPolicyPpi
  )
{
  PCH_RP_HANDLE                       RpHandle[PCH_MAX_PCIE_ROOT_PORTS];
  PCH_RP_PRIVATE_CONFIG               RpPrivate[PCH_MAX_PCIE_ROOT_PORTS];
  PCH_RP_DEVICE                       RpDevice[PCH_MAX_PCIE_ROOT_PORTS];
  PCH_RP_CALLBACK                     RpCallback[PCH_MAX_PCIE_ROOT_PORTS];
  UINT32                              MaxRpIndex;
  UINT8                               CtrlIndex;
  UINT32                              RpIndex;

  MaxRpIndex = GetPchMaxPciePortNum ();
  for (RpIndex = 0; RpIndex < MaxRpIndex; RpIndex++) {
    PchRpHandleInit (SiPreMemPolicyPpi, NULL, RpIndex, &RpHandle[RpIndex], &RpPrivate[RpIndex], &RpDevice[RpIndex], &RpCallback[RpIndex]);
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
        PsfDisablePcieRootPort (RpDevice[RpIndex].RootPortIndex + 1);
        break;
      case PcieBifurcation2x2_1x4:
        //
        // Disable port 4 of a controller when it's set to 2x2 - 1x4
        //
        DEBUG ((DEBUG_INFO, "Disabling RP %d\n", RpIndex + 3));
        PsfDisablePcieRootPort (RpDevice[RpIndex].RootPortIndex + 3);
        break;
      case PcieBifurcation2x4:
      case PcieBifurcation2x2:
        //
        // Disable ports 2, 4 of a controller when it's set to 2x4 or 2x2
        //
        DEBUG ((DEBUG_INFO, "Disabling RP %d/%d\n", RpIndex + 1, RpIndex + 3));
        PsfDisablePcieRootPort (RpDevice[RpIndex].RootPortIndex + 1);
        PsfDisablePcieRootPort (RpDevice[RpIndex].RootPortIndex + 3);
        break;
      case PcieBifurcation1x8:
      case PcieBifurcation1x4:
        //
        // Disable ports 2, 3, 4 of a controller when it's set to 1x8 or 1x4
        //
        DEBUG ((DEBUG_INFO, "Disabling RP %d/%d/%d\n", RpIndex + 1, RpIndex + 2, RpIndex + 3));
        PsfDisablePcieRootPort (RpDevice[RpIndex].RootPortIndex + 1);
        PsfDisablePcieRootPort (RpDevice[RpIndex].RootPortIndex + 2);
        PsfDisablePcieRootPort (RpDevice[RpIndex].RootPortIndex + 3);
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

  for (RpIndex = 0; RpIndex < MaxRpIndex; RpIndex++) {
    PcieRpSetupLink (&RpHandle[RpIndex], RpHandle[RpIndex].PcieRpLinkEqPreMemConfig, RpHandle[RpIndex].PcieRpPreMemConfig->RpEnabledMask);
  }
}
