/** @file
  Header file for PeiEnterpriseRpInitLib which provides functions
  for initializing enterprise PCIe root ports

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

#ifndef _PEI_ENTERPRISE_PCIE_RP_INIT_LIB_H_
#define _PEI_ENTERPRISE_PCIE_RP_INIT_LIB_H_

#include <EnterprisePcieConfig.h>
#include <Library/PcieRpHandleLib.h>

/**
  Initializaes root ports in pre-mem. At this point we are responsible for transitioning
  all active root ports with devices connected to them into link active.

  @param[in] SiPreMemPolicyPpi  Pointer to pre mem policy PPI
**/
VOID
PcieRpInitPreMem (
  IN SI_PREMEM_POLICY_PPI  *SiPreMemPolicyPpi
  );

/**
  Perform Initialization of the Downstream Root Ports.

  @param[in] SiPolicy             The SI Policy PPI

**/
VOID
PcieRpInit (
  IN CONST SI_POLICY_PPI     *SiPolicy
  );

/**
  Perform Initialization of the Downstream Root Ports.

  @param[in     ] SiPolicy            The SI Policy PPI
  @param[in     ] IoeId               IOE index - starting from 1
**/
VOID
IoePcieRpInit (
  IN  SI_POLICY_PPI  *SiPolicy,
  IN  UINT32          IoeId
  );

/**
  Configure PCIe Grant Counts
**/
VOID
PcieRpConfigureGrantCounts (
  IN CONST SI_POLICY_PPI     *SiPolicy
  );

/*
  Disable Root Ports at PSF level
*/
VOID
PcieDisableUnusedPort (
  IN  PCH_RP_HANDLE          *RpHandle
  );

/**
  Check for device presence with timeout.

  @param[in]     RpBase      Root Port base address
  @param[in]     TimeoutUs   Timeout in microseconds
  @param[in,out] Timer       Timer value, must be initialized to zero
                             before the first call of this function.
**/
BOOLEAN
IsPcieDevicePresent (
  IN     UINT64  RpBase,
  IN     UINT32  TimeoutUs,
  IN OUT UINT32  *Timer
  );

/**
  Enable PCI Express Clock Gating for Root Ports and Virtual Root Ports.
  This settings are relevant to all ports including disabled ports.
  Use sideband access since primary may not be available.

  @param[in] RpHandle           Pointer to PCIE RP handle
**/
VOID
PchPcieEnableRootPortClockGating (
  IN  PCH_RP_HANDLE *RpHandle
  );

/*
  Checks if given rootport has an endpoint connected

  @param[in] DeviceBase       PCI segment base address of root port

  @retval                     TRUE if endpoint is connected
  @retval                     FALSE if no endpoint was detected
*/
BOOLEAN
IsEndpointConnected (
  UINT64 DeviceBase
  );

/**
  Program Root Port PTM

  @param[in] RpHandle             Pointer to PCIE RP handle
**/
VOID
PcieRpProgramPtm (
  IN  PCH_RP_HANDLE           *RpHandle
  );

/*
  Some PCIe devices may take long time between they become detected and form a link.
  This function waits until all enabled, non-empty rootports form a link or until a timeout

  @param[in] MaxRootPorts      number of rootports
  @param[in] DisabledPortMask  mask of rootprots that don't need to be considered
*/
VOID
WaitForLinkActive (
  UINT32 MaxRootPorts,
  UINT32 DisabledPortMask
  );

/**
  The function to change the root port speed

  @param[in] PcieConfig           Root port configuration

  @retval EFI_SUCCESS             Succeeds.
**/
EFI_STATUS
PcieRpSpeedChange (
  IN CONST PCH_PCIE_CONFIG *PcieConfig
  );

/**
  Sets Target Link Speed of root port device.

  @param[in] RpBase     Root port base address
  @param[in] PciSpeed   Target link speed: 1 - gen1, 2 - gen2, 3 - gen3
**/
VOID
PchPcieSetTargetLinkSpeed (
  IN UINT64  RpBase,
  IN UINT32  PciSpeed
  );

/**
  Sets Retrain Link register of root port device.

  @param[in] RpBase     Root port base address
  @param[in] Set        Sets or clears bit
**/
VOID
PchPcieSetRetrainLink (
  IN UINT64  RpBase,
  IN BOOLEAN Set
  );

/**
  Disables the root port. Use sideband access unless the port is still available.

  @param[in] RpHandle           Pointer to PCIE RP handle
**/
VOID
DisablePcieRootPort (
  IN PCH_RP_HANDLE *RpHandle
  );

/**
  Perform Initialization of the Downstream Root Ports.

  @param[in] RpHandle             Pointer to PCIE RP handle
**/
VOID
PcieRpDownstreamRootPortInit (
  IN  PCH_RP_HANDLE           *RpHandle
  );

/**
  Determines whether PCIe link is active

  @param[in] RpBase    Root Port base address
  @retval BOOLEAN      Link Active state
**/
BOOLEAN
PchPcieIsLinkActive (
  IN UINT64 RpBase
  );

/**
  Get PSF config table

  @param[in     ] RpDevice                 Pointer to PCIE RP device
  @param[in, out] PsfPcieCtrlConfig        Pointer to PSF config table
**/
VOID
PcieRpGetPsfConfig (
  IN     PCH_RP_HANDLE            *RpHandle,
  IN OUT PSF_PCIE_CTRL_CONFIG     *PsfPcieCtrlConfig
  );

/**
  Detect whether CLKREQ# is supported by the platform and device.

  Assumes device presence. Device will pull CLKREQ# low until CPM is enabled.
  Test pad state to verify that the signal is correctly connected.
  This function will change pad mode to GPIO!

  @param[in] RootPortConfig      Root port configuration
  @param[in] DevicePresent       Determines whether there is a device on the port

  @retval TRUE if supported, FALSE otherwise.
**/
BOOLEAN
PcieDetectClkreq (
  IN       UINT32          RpIndex,
  IN CONST PCH_PCIE_CONFIG *PcieConfig
  );

/**
  Verify whether the PCIe port does own all lanes according to the port width.

  @param[in] RpHandle           Pointer to PCIE RP handle
  @param[in] RpCtrlHandle       Pointer to handle for first PCIE RP in controller

  @retval TRUE   All PCIe lanes are connected to PHY
  @retval FALSE  At least one lane is not connected to PHY
**/
BOOLEAN
IsPcieRootPortWired (
  IN  PCH_RP_HANDLE *RpHandle,
  IN  PCH_RP_HANDLE *RpCtrlHandle
  );

/*
  Setup Root Port Link

  @param[in] RpHandle                   Pointer to PCIE RP handle
  @param[in] PcieRpLinkEqPreMemConfig   Pointer to PCIE RP pre mem config
  @param[in] RpEnabledMask              Root ports enabled mask
*/
VOID
PcieRpSetupLink (
  IN PCH_RP_HANDLE                       *RpHandle,
  IN ENTERPRISE_PCIE_LINK_PREMEM_CONFIG  *PcieRpLinkEqPreMemConfig,
  IN UINT32                              RpEnabledMask
  );

/**
  Program bifurcation configuration for each PCIe RP controller based on policy.
  This also locks the bifurcation control register.

  @param[in] RpHandle                  Pointer to PCIE RP handle
**/
VOID
PcieBifurcationInit (
  IN PCH_RP_HANDLE             *RpHandle
  );

/**
  Translate PCIe Port/Lane pair to 0-based PCIe lane number.

  @param[in] RpHandle     Pointer to PCIE RP handle
  @param[in] RpCtrlHandle Pointer to handle for first PCIE RP in controller
  @param[in] RpLane       Root Port Lane

  @retval PCIe lane number (0-based)
**/
UINT32
PchPciePhysicalLane (
  IN  PCH_RP_HANDLE *RpHandle,
  IN  PCH_RP_HANDLE *RpCtrlHandle,
  IN  UINT32        RpLane
  );

#endif

