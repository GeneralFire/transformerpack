/** @file
  Header file for private PeiPcieRpInitLib.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation.

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

#ifndef _PEI_PCIE_RP_INIT_LIB_H_
#define _PEI_PCIE_RP_INIT_LIB_H_

#include <Library/PeiPcieSipInitLib.h>
#include <Ppi/SiPolicy.h>
#include <PchPcieRpConfig.h>

/**
  Device information structure
**/
typedef struct {
  UINT16  Vid;
  UINT16  Did;
  UINT8   MaxLinkSpeed;
} PCIE_DEVICE_INFO;

/**
  Hide rootports disabled by policy. This needs to be done in premem,
  because graphics init from SystemAgent code depends on those ports
  being already hidden

  @param[in] PcieRpPreMemConfig   Platform policy
**/
VOID
PcieRpEarlyDisabling (
  IN PCH_PCIE_RP_PREMEM_CONFIG *PcieRpPreMemConfig
  );

/**
  Initializes ports with NonCommonClock configuration. Such ports can't detect endpoints
  before NCC init ends. To prevent boot delays, it happens in pre-mem
**/
VOID
PcieRpEarlyNccHandling (
  VOID
  );

/**
  Configure root port function number mapping
**/
VOID
PcieRpConfigureRpfnMapping (
  VOID
  );

/*
  Get PCIE Port configuration and disable value for each RP
  and perform function disable.
*/
VOID
GetConfigAndDisablePcieRootPort (
  VOID
  );

/*
  Perform some early Pcie Root port configuration to enable RCRB and program decoder control registers
*/
VOID
EarlyPcieRootPortConfig (
  VOID
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
  Configure PCIe Grant Counts
**/
VOID
PcieRpConfigureGrantCounts (
  VOID
  );

/**
  Get information about the endpoint

  @param[in]  RpBase      Root port pci segment base address
  @param[in]  TempPciBus  Temporary bus number
  @param[out] DeviceInfo  Device information structure

  @raturn TRUE if device was found, FALSE otherwise
**/
BOOLEAN
GetDeviceInfo (
  IN  UINT64            RpBase,
  IN  UINT8             TempPciBus,
  OUT PCIE_DEVICE_INFO  *DeviceInfo
  );

/**
  Returns the SIP version of the PCIe root port.

  @param[in] RpIndex  Index of the root port in PCH

  @return Version of the SIP
**/
PCIE_SIP_VERSION
PcieRpGetSipVer (
  IN UINT32  RpIndex
  );

/**
  PCIe controller configuration.
**/
typedef enum {
  Pcie4x1      = 0,
  Pcie1x2_2x1  = 1,
  Pcie2x2      = 2,
  Pcie1x4      = 3
} PCIE_CONTROLLER_CONFIG;

/**
  Returns the PCIe controller configuration (4x1, 1x2-2x1, 2x2, 1x4)

  @param[in] ControllerIndex        Number of PCIe controller (0 based)

  @retval PCIe controller configuration
**/
PCIE_CONTROLLER_CONFIG
GetPcieControllerConfig (
  IN UINT32  ControllerIndex
  );

/**
  Enables PCIe controller trunk clock gating.

  @param[in] RpIndex  Index of the controller on which to enable trunk clock gating.
**/
VOID
PcieEnableTrunkClockGate (
  IN UINT32 ControllerIndex
  );

/**
  Translate PCIe Port/Lane pair to 0-based PCIe lane number.

  @param[in] RpIndex    Root Port index
  @param[in] RpLane     Root Port Lane

  @retval PCIe lane number (0-based)
**/
UINT32
PchPciePhysicalLane (
  UINT32  RpIndex,
  UINT32  RpLane
  );

/**
  Initializes ports with NonCommonClock and SSC configuration.
**/
VOID
PcieRpNccWithSscHandling (
  VOID
  );
#endif
