/** @file
  Header file for PCIe SIP init library.

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

#ifndef _PCIE_SIP_INIT_LIB_H_
#define _PCIE_SIP_INIT_LIB_H_

#include <PchPcieRpConfig.h>

typedef enum {
  PcieSip14 = 14,
  PcieSip15,
  PcieSip16,
  PcieSip17,
  PcieSip18
} PCIE_SIP_VERSION;

typedef enum {
  PcieUndefinedIntegration,
  PchPcie,
  CpuPcie,
  TbtPcie,
} PCIE_INTEGRATION;

typedef struct {
  UINT16  Segment;
  UINT16  Bus;
  UINT16  Device;
  UINT16  Function;
} PCIE_SBDF;

typedef struct _PCIE_ROOT_PORT_DEV  PCIE_ROOT_PORT_DEV;

/**
  Reads a 8-bit PCI configuration register

  @param  RpDev       Pointer to the PCIe root port device
  @param  Offset      Offset of the register within the PCI config

  @return The 8-bit PCI configuration register specified by Address
**/
typedef
UINT8
(*PCIE_SIP_READ8) (
  IN PCIE_ROOT_PORT_DEV        *RpDev,
  IN UINT64                    Offset
  );

/**
  Writes a 8-bit PCI configuration register

  @param  RpDev       Pointer to the PCIe root port device
  @param  Offset      Offset of the register within the PCI config
  @param  Value       The value to write

  @return The parameter of Value
**/
typedef
UINT8
(*PCIE_SIP_WRITE8) (
  IN PCIE_ROOT_PORT_DEV        *RpDev,
  IN UINT64                    Offset,
  IN UINT8                     Value
  );

/**
  Performs a bitwise OR of a 8-bit PCI configuration register with a 8-bit value

  @param  RpDev     Pointer to PCIe root port device
  @param  Offset    Offset of the register within the PCI config
  @param  OrData    The value to OR with the PCI configuration register

  @return The value written to the PCI configuration register
**/
typedef
UINT8
(*PCIE_SIP_OR8) (
  IN PCIE_ROOT_PORT_DEV        *RpDev,
  IN UINT64                    Offset,
  IN UINT8                     OrData
  );

/**
  Performs a bitwise AND of a 8-bit PCI configuration register with a 8-bit value

  @param  RpDev     Pointer to PCIe root port device
  @param  Offset    Offset of the register within the PCI config
  @param  Address   Address that encodes the PCI Segment, Bus, Device, Function, and Register
  @param  AndData   The value to AND with the PCI configuration register

  @return The value written to the PCI configuration register
**/
typedef
UINT8
(*PCIE_SIP_AND8) (
  IN PCIE_ROOT_PORT_DEV        *RpDev,
  IN UINT64                    Offset,
  IN UINT8                     AndData
  );

/**
  Performs a bitwise AND of a 8-bit PCI configuration register with a 8-bit value,
  followed a  bitwise OR with another 8-bit value

  @param  RpDev     Pointer to PCIe root port device
  @param  Offset    Offset of the register within the PCI config
  @param  AndData   The value to AND with the PCI configuration register
  @param  OrData    The value to OR with the PCI configuration register

  @return The value written to the PCI configuration register
**/
typedef
UINT8
(*PCIE_SIP_AND_THEN_OR8) (
  IN PCIE_ROOT_PORT_DEV        *RpDev,
  IN UINT64                    Offset,
  IN UINT8                     AndData,
  IN UINT8                     OrData
  );

/**
  Reads a 16-bit PCI configuration register

  @param  RpDev       Pointer to the PCIe root port device
  @param  Offset      Offset of the register within the PCI config

  @return The 16-bit PCI configuration register specified by Address
**/
typedef
UINT16
(*PCIE_SIP_READ16) (
  IN PCIE_ROOT_PORT_DEV        *RpDev,
  IN UINT64                    Offset
  );

/**
  Writes a 16-bit PCI configuration register

  @param  RpDev       Pointer to the PCIe root port device
  @param  Offset      Offset of the register within the PCI config
  @param  Value       The value to write

  @return The parameter of Value
**/
typedef
UINT16
(*PCIE_SIP_WRITE16) (
  IN PCIE_ROOT_PORT_DEV        *RpDev,
  IN UINT64                    Offset,
  IN UINT16                    Value
  );

/**
  Performs a bitwise OR of a 16-bit PCI configuration register with a 16-bit value

  @param  RpDev     Pointer to PCIe root port device
  @param  Offset    Offset of the register within the PCI config
  @param  OrData    The value to OR with the PCI configuration register

  @return The value written to the PCI configuration register
**/
typedef
UINT16
(*PCIE_SIP_OR16) (
  IN PCIE_ROOT_PORT_DEV        *RpDev,
  IN UINT64                    Offset,
  IN UINT16                    OrData
  );

/**
  Performs a bitwise AND of a 16-bit PCI configuration register with a 16-bit value

  @param  RpDev     Pointer to PCIe root port device
  @param  Offset    Offset of the register within the PCI config
  @param  Address   Address that encodes the PCI Segment, Bus, Device, Function, and Register
  @param  AndData   The value to AND with the PCI configuration register

  @return The value written to the PCI configuration register
**/
typedef
UINT16
(*PCIE_SIP_AND16) (
  IN PCIE_ROOT_PORT_DEV        *RpDev,
  IN UINT64                    Offset,
  IN UINT16                    AndData
  );

/**
  Performs a bitwise AND of a 16-bit PCI configuration register with a 16-bit value,
  followed a  bitwise OR with another 16-bit value

  @param  RpDev     Pointer to PCIe root port device
  @param  Offset    Offset of the register within the PCI config
  @param  AndData   The value to AND with the PCI configuration register
  @param  OrData    The value to OR with the PCI configuration register

  @return The value written to the PCI configuration register
**/
typedef
UINT16
(*PCIE_SIP_AND_THEN_OR16) (
  IN PCIE_ROOT_PORT_DEV        *RpDev,
  IN UINT64                    Offset,
  IN UINT16                    AndData,
  IN UINT16                    OrData
  );

/**
  Reads a 32-bit PCI configuration register

  @param  RpDev       Pointer to the PCIe root port device
  @param  Offset      Offset of the register within the PCI config

  @return The 32-bit PCI configuration register specified by Address
**/
typedef
UINT32
(*PCIE_SIP_READ32) (
  IN PCIE_ROOT_PORT_DEV        *RpDev,
  IN UINT64                    Offset
  );

/**
  Writes a 32-bit PCI configuration register

  @param  RpDev       Pointer to the PCIe root port device
  @param  Offset      Offset of the register within the PCI config
  @param  Value       The value to write

  @return The parameter of Value
**/
typedef
UINT32
(*PCIE_SIP_WRITE32) (
  IN PCIE_ROOT_PORT_DEV        *RpDev,
  IN UINT64                    Offset,
  IN UINT32                    Value
  );

/**
  Performs a bitwise OR of a 32-bit PCI configuration register with a 32-bit value

  @param  RpDev     Pointer to PCIe root port device
  @param  Offset    Offset of the register within the PCI config
  @param  OrData    The value to OR with the PCI configuration register

  @return The value written to the PCI configuration register
**/
typedef
UINT32
(*PCIE_SIP_OR32) (
  IN PCIE_ROOT_PORT_DEV        *RpDev,
  IN UINT64                    Offset,
  IN UINT32                    OrData
  );

/**
  Performs a bitwise AND of a 32-bit PCI configuration register with a 32-bit value

  @param  RpDev     Pointer to PCIe root port device
  @param  Offset    Offset of the register within the PCI config
  @param  Address   Address that encodes the PCI Segment, Bus, Device, Function, and Register
  @param  AndData   The value to AND with the PCI configuration register

  @return The value written to the PCI configuration register
**/
typedef
UINT32
(*PCIE_SIP_AND32) (
  IN PCIE_ROOT_PORT_DEV        *RpDev,
  IN UINT64                    Offset,
  IN UINT32                    AndData
  );

/**
  Performs a bitwise AND of a 32-bit PCI configuration register with a 32-bit value,
  followed a  bitwise OR with another 32-bit value

  @param  RpDev     Pointer to PCIe root port device
  @param  Offset    Offset of the register within the PCI config
  @param  AndData   The value to AND with the PCI configuration register
  @param  OrData    The value to OR with the PCI configuration register

  @return The value written to the PCI configuration register
**/
typedef
UINT32
(*PCIE_SIP_AND_THEN_OR32) (
  IN PCIE_ROOT_PORT_DEV        *RpDev,
  IN UINT64                    Offset,
  IN UINT32                    AndData,
  IN UINT32                    OrData
  );

typedef struct {
  PCIE_SIP_READ8          Read8;
  PCIE_SIP_WRITE8         Write8;
  PCIE_SIP_OR8            Or8;
  PCIE_SIP_AND8           And8;
  PCIE_SIP_AND_THEN_OR8   AndThenOr8;

  PCIE_SIP_READ16         Read16;
  PCIE_SIP_WRITE16        Write16;
  PCIE_SIP_OR16           Or16;
  PCIE_SIP_AND16          And16;
  PCIE_SIP_AND_THEN_OR16  AndThenOr16;

  PCIE_SIP_READ32         Read32;
  PCIE_SIP_WRITE32        Write32;
  PCIE_SIP_OR32           Or32;
  PCIE_SIP_AND32          And32;
  PCIE_SIP_AND_THEN_OR32  AndThenOr32;
} PCI_CONFIG_ACCESS;

typedef struct {
  PCIE_SIP_READ32         Read32;
  PCIE_SIP_WRITE32        Write32;
  PCIE_SIP_OR32           Or32;
  PCIE_SIP_AND32          And32;
  PCIE_SIP_AND_THEN_OR32  AndThenOr32;
} PCI_SBI_MSG_CONFIG_ACCESS;

typedef struct {
  PCIE_SIP_READ32         Read32;
  PCIE_SIP_WRITE32        Write32;
  PCIE_SIP_OR32           Or32;
  PCIE_SIP_AND32          And32;
  PCIE_SIP_AND_THEN_OR32  AndThenOr32;
} PCI_SBI_MSG_MEM_ACCESS;

struct _PCIE_ROOT_PORT_DEV {
  PCIE_SIP_VERSION  SipVersion;
  PCIE_INTEGRATION  Integration;

  //
  // This member describes the root port location on PCI.
  // Note that depending on the integration root port might
  // not be accessible via the PCI config so library will not
  // attempt to construct memory addresses from this Sbdf.
  //
  PCIE_SBDF         Sbdf;

  //
  // This member allows the SIP library to access
  // the IP PCI config register bank. Please note that
  // depending on the integration the PCI config bank might
  // not be accessible via the standard PCI access mechanism
  // such as PCI Express BAR.
  //
  PCI_CONFIG_ACCESS  PciCfgAccess;

  //
  // This member allows the SIP library to access the
  // IP PCI config registers by sending the SBI message
  // on the sideband.
  //
  PCI_SBI_MSG_CONFIG_ACCESS  PciSbiMsgCfgAccess;

  //
  // This member allows the SIP library to access the
  // IP memory registers via the SBI message on the
  // sideband.
  //
  PCI_SBI_MSG_MEM_ACCESS  PciSbiMsgMemAccess;
};

typedef struct {
  BOOLEAN  NoSnoopEnable;
  UINT32   NoSnoopScale;
  UINT32   NoSnoopValue;
  BOOLEAN  SnoopEnable;
  UINT32   SnoopScale;
  UINT32   SnoopValue;
} LTR_SUBTRACTION_CONFIG;

/**
  Configure all LTR subtraction registers according to configuration

  @param[in] RpDev             Pointer to the root port device
  @param[in] L1StandardConfig  Pointer to L1Standard LTR subtraction config
  @param[in] L1p1Config        Pointer to L1.1 LTR subtraction config
  @param[in] L1p2Config        Pointer to L1.2 LTR subtraction config
  @param[in] LtrSubL11Npg      Pointer to L1.1 No Power Gate LTR subtraction config
**/
VOID
PcieSipConfigureLtrSubstraction (
  IN PCIE_ROOT_PORT_DEV      *RpDev,
  IN LTR_SUBTRACTION_CONFIG  *L1StandardConfig,
  IN LTR_SUBTRACTION_CONFIG  *L1p1Config,
  IN LTR_SUBTRACTION_CONFIG  *L1p2Config,
  IN LTR_SUBTRACTION_CONFIG  *LtrSubL11Npg
  );

#define MAX_PTM_STAGE_DELAY_CONFIG_REGS_SIP17  14
#define MAX_PTM_STAGE_DELAY_CONFIG_REGS_SIP16  11
#define MAX_PTM_STAGE_DELAY_CONFIG_REGS_SIP14  5
#define MAX_PTM_STAGE_DELAY_CONFIG_REGS        MAX_PTM_STAGE_DELAY_CONFIG_REGS_SIP17

typedef struct {
  UINT32  PtmPipeStageDelay[MAX_PTM_STAGE_DELAY_CONFIG_REGS];
  UINT32  PtmConfig;
} PTM_CONFIGURATION;

/**
  Configures PTM settings on the PCIe controller.

  @param[in] RpDev      Pointer to the root port device
  @param[in] PtmConfig  Pointer to the PTM configuration structure
**/
VOID
PcieSipConfigurePtm (
  IN PCIE_ROOT_PORT_DEV  *RpDev,
  IN PTM_CONFIGURATION   *PtmConfig
  );

/**
  Enables retimer presence and two retimers presence detect on supported
  SIP versions.

  @param[in] RpDev  Pointer to the root port device
**/
VOID
PcieSipConfigureRetimerSupport (
  IN PCIE_ROOT_PORT_DEV  *RpDev
  );

/**
  Configure 10-Bit Tag

  @param[in] RpDev               Pointer to the root port device
  @param[in] CompleterSupported  If TRUE it indicates 10-Bit Tag Completer is supported.
  @param[in] RequesterSupported  If TRUE it indicates 10-Bit Tag Requester is supported.

**/
VOID
PcieSipConfigure10BitTag (
  IN PCIE_ROOT_PORT_DEV      *RpDev,
  IN BOOLEAN                 CompleterSupported,
  IN BOOLEAN                 RequesterSupported
  );

/**
  Configure Peer Disable

  @param[in] RpDev               Pointer to the root port device
  @param[in] CfgReads            TRUE/FALSE, enable/disable forwarding of Upstream Posted Memory Reads
  @param[in] CfgWrites           TRUE/FALSE, enable/disable forwarding of Upstream Posted Memory Writes
**/
VOID
PcieSipConfigurePeerDisable (
  IN PCIE_ROOT_PORT_DEV    *RpDev,
  IN BOOLEAN               CfgReads,
  IN BOOLEAN               CfgWrites
  );

/**
  Configure PCIe Completion Coalescing

  @param[in] RpDev               Pointer to the root port device
**/
VOID
PcieConfigureCoalescing (
  IN  PCIE_ROOT_PORT_DEV    *RpDev
  );

/**
  Configure Pcie Squelch Power Management.

  @param[in] RpDev  Pointer to the root port device.

**/
VOID
ConfigurePcieSquelchPowerManagement (
  IN PCIE_ROOT_PORT_DEV      *RpDev
  );

/**
  Configures the dynamic clock gating

  @param[in] RpDev               Pointer to the root port device
**/
VOID
ConfigureDynamicClockGating (
  IN  PCIE_ROOT_PORT_DEV    *RpDev
  );

typedef struct {
  UINT32  PostCursorCoefficient;
  UINT32  CursorCoefficient;
  UINT32  PreCursorCoefficient;
} PCIE_PRESET_TO_COEFFICIENT_MAPPING;

typedef struct {
  //
  // This flag indicates if project's PCIe PIPE implementation supports
  // get preset to coefficient mapping query in the phy. If this query is supported project
  // doesn't have to specify preset to coefficient mapping in the BIOS as root port
  // can get it from phy.
  //
  BOOLEAN                             PipePresetToCoefficientQuerySupported;
  UINT32                              LowFrequencyValue; // Only valid if query to PIPE is unsupported
  UINT32                              FullSwingValue; // Only valid if query to PIPE is unsupported
  PCIE_PRESET_TO_COEFFICIENT_MAPPING  PresetToCoefficient[PCIE_LINK_EQ_PRESETS_MAX]; // Only valid if query to PIPE is unsupported
  UINT32                              PresetCoefficientEvaluationTimeMs;
} PCIE_LINK_EQ_PROJECT_SETTINGS;

typedef struct {
  PCIE_LINK_EQ_PLATFORM_SETTINGS  PlatformSettings;
  PCIE_LINK_EQ_PROJECT_SETTINGS   ProjectSettings;
} PCIE_LINK_EQ_SETTINGS;

/**
  Performs the initialization for hardware equalization to Gen3.
  Please make sure this function is called before programming any
  power management register in PCIe Root Port

  @param[in] RpDev                   Pointer to the root port device
  @param[in] PcieGen3LinkEqSettings  PCIe Gen3 link equalization settings to be used during EQ
  @param[in] PcieGen4LinkEqSettings  PCIe Gen4 link equalization settings to be used during EQ
**/
VOID
PcieSipLinkEqualizeInit (
  IN PCIE_ROOT_PORT_DEV       *RpDev,
  IN PCIE_LINK_EQ_SETTINGS    *PcieGen3LinkEqSettings,
  IN PCIE_LINK_EQ_SETTINGS    *PcieGen4LinkEqSettings
  );

/**
  Performs PCIe link equalization according to platform settings.
  Please make sure that link is in gen1 or gen2 before calling this
  procedure.

  @param[in] RpDev                   Pointer to the root port device
  @param[in] EndPointAttached        Is End Point device connected to the Root Port
  @param[in] PcieGen3LinkEqSettings  PCIe Gen3 link equalization settings to be used during EQ
  @param[in] PcieGen4LinkEqSettings  PCIe Gen4 link equalization settings to be used during EQ
  @param[in] RootPortPcieSpeed       PCIe speed of Root Port
  @param[in] EndpointPcieSpeed       PCIe speed of Endpoint connected to corresponding Root Port
**/
VOID
PcieSipLinkEqualize (
  IN PCIE_ROOT_PORT_DEV       *RpDev,
  IN BOOLEAN                  EndPointAttached,
  IN PCIE_LINK_EQ_SETTINGS    *PcieGen3LinkEqSettings,
  IN PCIE_LINK_EQ_SETTINGS    *PcieGen4LinkEqSettings,
  IN PCIE_SPEED               RootPortPcieSpeed,
  IN PCIE_SPEED               EndpointPcieSpeed
  );

/**
  Get max link width.

  @param[in] RpDev  Pointer to the root port device
  @retval           Max link width
**/
UINT8
PcieSipGetMaxLinkWidth (
  IN PCIE_ROOT_PORT_DEV    *RpDev
  );

/**
  Get PCIe port number for enabled port.
  @param[in] RpDev     Pointer to the root port device

  @retval Root Port number (1 based)
**/
UINT32
PcieSipGetPortNum (
  IN PCIE_ROOT_PORT_DEV    *RpDev
  );

/**
  Get the negotiated link width

  @param[in] RpDev     Pointer to the root port device

  @return negotiated link width
**/
UINT8
PcieSipGetNegotiatedLinkWidth (
  PCIE_ROOT_PORT_DEV  *RpDev
  );

  /**
  Configure Pcie Phy Configuration.
  This function has to be executed
  before performing equalization.

  @param[in] RpDev  Pointer to the root port device.

**/
VOID
ConfigurePcieSipPhyInit (
  IN PCIE_ROOT_PORT_DEV      *RpDev
  );
#endif
