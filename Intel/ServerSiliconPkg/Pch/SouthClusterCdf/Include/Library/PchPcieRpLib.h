/** @file
  Header file for PchPcieRpLib.

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
#ifndef _PCH_PCIERP_LIB_H_
#define _PCH_PCIERP_LIB_H_

//
// SC_CONV
// remove flags after sync to client
//
#include <Private/Library/PchPciExpressHelpersLib.h>
#include <Register/PchRegsPcr.h>
#include <Private/Library/PchSbiAccessLib.h>

#define PCH_MAX_PCIE_PTM_DELAY_REGISTER             6

typedef struct {
  UINT32 AndValue;
  UINT32 OrValue;
} PTM_PIPE_STAGE_DELAY_VALUE;

typedef struct {
  PTM_PIPE_STAGE_DELAY_VALUE Register[PCH_MAX_PCIE_PTM_DELAY_REGISTER];
} PTM_PIPE_STAGE_DELAY_CONTROLLER;


#define RST_PCIE_STORAGE_CR_1                       0
#define RST_PCIE_STORAGE_CR_2                       1
#define RST_PCIE_STORAGE_CR_3                       2
#define RST_PCIE_STORAGE_CR_INVALID                 99

typedef struct {
  UINT8 DevNum;
  UINT8 FunNum;
  UINT8 Pid;
  UINT8 RpNumBase;
} PCH_PCIE_CONTROLLER_INFO;


//
// SC_CONV
// remove flags after sync to client
//
/**
  Returns total number of PIDs for PCIE root ports.

  @retval UINT32    Total number of PIDs for PCIE root ports
**/
UINT32
PchGetPcieRpPidCount (
  VOID
);

/**
  PCIe controller bifurcation configuration.
**/
typedef enum {
  PcieBifurcationDefault = 0,
  PcieBifurcation4x1,
  PcieBifurcation1x2_2x1,
  PcieBifurcation2x2,
  PcieBifurcation1x4,
  PcieBifurcation4x2,
  PcieBifurcation1x4_2x2,
  PcieBifurcation2x2_1x4,
  PcieBifurcation2x4,
  PcieBifurcation1x8,
  PcieBifurcationUnknown,
  PcieBifurcationMax
} PCIE_BIFURCATION_CONFIG;

/**
  This function returns PID according to PCIe controller index

  @param[in] ControllerIndex     PCIe controller index

  @retval PCH_SBI_PID    Returns PID for SBI Access
**/
PCH_SBI_PID
PchGetPcieControllerSbiPid (
  IN  UINT32  ControllerIndex
  );

/**
  This function returns PID according to Root Port Number

  @param[in] RpPort             Root Port Number

  @retval PCH_SBI_PID    Returns PID for SBI Access
**/
PCH_SBI_PID
PchGetPcieRpSbiPid (
  IN  UINTN  RpPort
  );

/**
  Calculate root port device number based on physical port index.

  @param[in]  RpIndex              Root port index (0-based).

  @retval     Root port device number.
**/
UINT32
PchGetPcieRpDevice (
  IN  UINT32   RpIndex
  );

/**
  Gets Vid of root port device.

  @param[in] RpBase     Root port base address
  @retval UINT32        Vid value
**/
UINT16
PchPcieGetVid (
  IN UINT64 RpBase
  );

/**
  Determines whether PCIe link is active

  @param[in] RpBase    Root Port base address
  @retval BOOLEAN      Link Active state
**/
BOOLEAN
PchPcieIsLinkActive (
  UINT64  RpBase
  );

/**
  Get max PCIe link speed supported by the root port.

  @param[in] RpBase    Root Port base address
  @retval Max link speed
**/
UINT8
PchPcieGetMaxLinkSpeed (
  IN UINT64 RpBase
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
  Sets Target Link Speed of root port device.

  @param[in] RpBase     Root port base address
  @param[in] PciSpeed   Target link speed: 1 - gen1, 2 - gen2, 3 - gen3
**/
VOID
PchPcieSetTargetLinkSpeed (
  IN UINT64 RpBase,
  IN UINT8  PciSpeed
  );

/**
  Sets Max Link Speed of root port device.

  @param[in] RpBase     Root port base address
  @param[in] PciSpeed   Max link speed: 1 - gen1, 2 - gen2, 3 - gen3
**/
VOID
PchPcieSetMaxLinkSpeed (
  IN UINT64 RpBase,
  IN UINT8  PciSpeed
  );

/**
  Sets ASPM capability of root port device.

  @param[in] PortIndex      Root port index
  @param[in] Aspm           Aspm modes supported
  @param[in] L1ExitLatency  Exit latency of L1
**/
VOID
PchPcieSetAspmCap (
  IN UINT8                    PortIndex,
  IN PCH_PCIE_ASPM_CONTROL    Aspm,
  IN PCH_PCIE_L1_EXIT_LATENCY L1ExitLatency
  );

/**
  Sets ASPM control of root port device.

  @param[in] PortIndex      Root port index
  @param[in] Aspm           Aspm modes enabled
**/
VOID
PchPcieSetAspmCtrl (
  IN UINT8                  PortIndex,
  IN PCH_PCIE_ASPM_CONTROL  Aspm
  );

  /**
  Sets ASPM Optionality Compliance bit of root port device.

  @param[in] PortIndex      Root port index
  @param[in] AspmOpComp     Set ASPM Optionality Compliance bit
**/
VOID
PchPcieSetAspmOpCmp (
  IN UINT8    PortIndex,
  IN BOOLEAN  AspmOpComp
  );

/**
  Returns the PCIe controller configuration

  @param[in] ControllerIndex        Number of PCIe controller (0 based)

  @retval PCIe controller configuration
**/
PCIE_BIFURCATION_CONFIG
PchPcieGetBifurcationConfig (
  IN     UINT32        ControllerIndex
  );

/**
  Sets the PCIe controller bifurcation configuration.
  Locks the register after write.

  @param[in] ControllerIndex        Number of PCIe controller (0 based)
  @param[in] Config                 PCIe controller bifurcation configuration

  @retval EFI_STATUS                Status
**/
EFI_STATUS
PchPcieSetBifurcationConfig (
  IN  UINT32                   ControllerIndex,
  IN  PCIE_BIFURCATION_CONFIG  Config
  );

/**
  Get Pch Pcie Root Port Device and Function Number by Root Port physical Number

  @param[in]  RpNumber            Root port physical number. (0-based)
  @param[out] RpDev               Return corresponding root port device number.
  @param[out] RpFun               Return corresponding root port function number.

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
GetPchPcieRpDevFun (
  IN  UINTN   RpNumber,
  OUT UINTN   *RpDev,
  OUT UINTN   *RpFun
  );

/**
  Get Root Port physical Number by Pch Pcie Root Port Device and Function Number

  @param[in]  RpDev                 Root port device number.
  @param[in]  RpFun                 Root port function number.
  @param[out] RpNumber              Return corresponding physical Root Port index (0-based)

  @retval     EFI_SUCCESS           Physical root port is retrieved
  @retval     EFI_INVALID_PARAMETER RpDev and/or RpFun are invalid
  @retval     EFI_UNSUPPORTED       Root port device and function is not assigned to any physical root port
**/
EFI_STATUS
EFIAPI
GetPchPcieRpNumber (
  IN  UINTN   RpDev,
  IN  UINTN   RpFun,
  OUT UINTN   *RpNumber
  );

/**
  Gets pci segment base address of PCIe root port.

  @param RpIndex    Root Port Index (0 based)
  @return PCIe port base address.
**/
UINT64
PchPcieBase (
  IN  UINT32   RpIndex
  );

/**
  Checks which CLK_REQ signal is assigned to given CLK_SRC

  @param ClkSrc    number of CLK_SRC, 0-based
  @retval          number of CLK_REQ, 0-based
**/
UINT8
GetClkReqForClkSrc (
  IN UINT8 ClkSrc
  );

/**
  Determines whether L0s is supported on current stepping.

  @return TRUE if L0s is supported, FALSE otherwise
**/
BOOLEAN
PchIsPcieL0sSupported (
  VOID
  );

/**
  Some early PCH steppings require Native ASPM to be disabled due to hardware issues:
   - RxL0s exit causes recovery
   - Disabling PCIe L0s capability disables L1
  Use this function to determine affected steppings.

  @return TRUE if Native ASPM is supported, FALSE otherwise
**/
BOOLEAN
PchIsPcieNativeAspmSupported (
  VOID
  );

/**
  Check the RST PCIe Storage Cycle Router number according to the root port number and PCH type

  @param[in] RootPortNum  Root Port Number

  @return  The RST PCIe Storage Cycle Router Number
**/
UINT8
RstGetCycleRouterNumber (
  IN  UINT32                   RootPortNum
  );

/**
 If HotPlug is enabled by policy, HotPlug should be enabled by setting enable bit
 in VPPCSR register and defining number of PCA9555 device and slot number of PCA9555
 Not applicable in CNL

  @param[in] RpIndex      Root Port Index (0 based)
  @param[in] RpBase       Root Port base address
**/
VOID
VppHotPlugInit (
  IN  UINT8                                  VppPort,
  IN  UINT8                                  VppAddress,
  IN  UINT64                                 RpBase
  );

/*
  Reads PCIe RP device hide/disable register and returns mask of enabled
  devices.

  @retval UINT32    Bit mask of enabled device. BIT0 - RP0, BIT1 = RP1, ...
*/
UINT32
PchGetPcieRpEnabledMask (
  VOID
  );


/* Capabilities init functions */

/*
  Entry point for setting capabilities list.

  @param[in] RpBase  Root Port pci segment base address
*/
VOID
PchPcieCapabilitiesInitEntry (
  IN UINT64 RpBase
  );

/*
  Sets Downstream Port Containment capability

  @param[in] NextCap            Next capability on list offset
  @param[in] RpBase             Root Port pci segment base address
  @param[in] Supported          Capability supported
  @param[in] DpcWithExtensions  Are DPC Extensions to be enabled
  @retval UINT16                If capability supported returns this capability offset
*/
UINT16
PchPcieCapabilitiesInitDpcExt (
  IN UINT16  NextCap,
  IN UINT64  RpBase,
  IN BOOLEAN Supported,
  IN BOOLEAN DpcWithExtensions
  );

/*
  Sets Secondary PCI Express Extended Capability

  @param[in] NextCap    Next capability on list offset
  @param[in] RpBase     Root Port pci segment base address
  @param[in] Supported  Capability supported
  @retval UINT16        If capability supported returns this capability offset
*/
UINT16
PchPcieCapabilitiesInitSecExp (
  IN UINT16  NextCap,
  IN UINT64  RpBase,
  IN BOOLEAN Supported
  );

/*
  Set L1PM Substates capability

  @param[in] NextCap          Next capability on list offset
  @param[in] RpBase           Root Port pci segment base address
  @param[in] Supported        Capability supported
  @param[in] L12PMSSupported  L1.2 version supported
  @retval UINT16              If capability supported returns this capability offset
*/
UINT16
PchPcieCapabilitiesInitL1PMS (
  IN UINT16  NextCap,
  IN UINT64  RpBase,
  IN BOOLEAN Supported,
  IN BOOLEAN L12PMSSupported
  );

/*
  Set PTM capability

  @param[in] NextCap          Next capability on list offset
  @param[in] RpBase           Root Port pci segment base address
  @param[in] Supported        Capability supported
  @retval UINT16              If capability supported returns this capability offset
*/
UINT16
PchPcieCapabilitiesInitPtm (
  IN UINT16  NextCap,
  IN UINT64  RpBase,
  IN BOOLEAN Supported
  );

/*
  Set ACS Capability.

  @param[in] NextCap          Next capability on list offset
  @param[in] RpBase           Root Port pci segment base address
  @param[in] Supported        Capability supported
  @retval UINT16              If capability supported returns this capability offset
*/
UINT16
PchPcieCapabilitiesInitAcs (
  IN UINT16  NextCap,
  IN UINT64  RpBase,
  IN BOOLEAN Supported
  );

/*
  Set Advanced Error Reporting capability

  @param[in] NextCap          Next capability on list offset
  @param[in] RpBase           Root Port pci segment base address
  @param[in] Supported        Capability supported
  @retval UINT16              If capability supported returns this capability offset
*/
UINT16
PchPcieCapabilitiesInitAer (
  IN UINT16  NextCap,
  IN UINT64  RpBase,
  IN BOOLEAN Supported
  );

/*
  Masks the elastic buffer error for given root port base.

  @param[in] RpBase       Root Port base address
*/
VOID
PchPcieMaskElasticBufferError (
  IN  UINT64 RpBase
  );

/*
  Disable leaky bucket Gen3 degrade..

  @param[in] RpBase       Root Port base address
*/
VOID
PchPcieDisableLeakyBucketGen3Degrade (
  IN  UINT64 RpBase
  );

/*
  Sets the HW equalization for given port

  @param[in] PortIndex    Root port index
*/
VOID
PchPcieSetEqualization (
  IN UINT8 PortIndex
);

/*
  Sets the lane EQ presets for upstream and downstream ports.

  @param[in] PortIndex    Root port index
  @param[in] Uptp         Upstream port transmitter preset
  @param[in] Dptp         Downstream port transmitter preset
*/
VOID
PchPcieSetLaneEqualizationPresets (
  IN UINT8    PortIndex,
  IN UINT32   Uptp,
  IN UINT32   Dptp
  );

/**
  Returns the maximum value of the "Payload" parameter of all available PchPcie Ports.

  @param VOID
  @retval INT16     The value to assign in field max payload size field
**/
UINT16
PchPcieGetMaxPayloadSizeForAllPorts (
  VOID
  );

/*
  Get the number of LANEEQCTL registers per root port number in controller

  @param[in] PortIndex       Root Port index
  @retval    UINT8           Number of LANEEQCTL registers per root port number in controller
*/
UINT8
PchPcieGetLanesRegPerRp (
  IN UINT8 PortIndex
  );


#endif // _PCH_PCIERP_LIB_H_

