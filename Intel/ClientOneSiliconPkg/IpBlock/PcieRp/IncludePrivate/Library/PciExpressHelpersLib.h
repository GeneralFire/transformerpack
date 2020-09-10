/** @file
  Header file for PCI Express helpers library

@copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation.

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
#ifndef _PCI_EXPRESS_HELPERS_LIB_H_
#define _PCI_EXPRESS_HELPERS_LIB_H_

#include <PchPolicyCommon.h>
#include <Library/PcieRpLib.h>

#define VC_CAP_ID     0x0002  // Virtual Channel capability ID
#define MFVC_CAP_ID   0x0009  // Optional Multi Functional Virtual Channel capability ID
#define VC_CAP_REG    0x04    // Virtual Channel capability register
#define VC0_CTRL_REG  0x14    // VC0 control register
#define VC1_CTRL_REG  0x20    // VC1 control register - 14h + n*(0Ch) Here n is channel 1

typedef enum {
  TpoScale2us,
  TpoScale10us,
  TpoScale100us,
  TpoScaleMax
} T_PO_SCALE;

typedef struct {
  UINT32     Value;
  T_PO_SCALE Scale;
} T_POWER_ON;

//
// This structure keeps segment:bus:device:function coordinates of a PCIe device
// in a single variable. PcieCap is offset to PCI Express capabilities.
//
typedef struct {
  UINT32 Seg : 8;
  UINT32 Bus : 8;
  UINT32 Dev : 5;
  UINT32 Func : 3;
  UINT32 PcieCap : 8;
} SBDF;

/**
  Get the negotiated link width

  @param[in] RpBase    Root Port base address

  @return negotiated link width
**/
UINT8
PcieGetNegotiatedLinkWidth (
  UINT64  RpBase
  );

/*
  Returns Tpower_on capability of device

  @param[in] DeviceBase       device's PCI segment base address
  @param[in] L1ssCapOffset    offset to L1substates capability in device's extended config space

  @retval                     structure containing Tpoweron scale and value
*/
T_POWER_ON
GetTpoCapability (
  UINT64 DeviceBase,
  UINT32 L1ssCapOffset
  );

/*
  Converts Tpower_on from value:scale notation to microseconds

  @param[in] TpoScale   T power on scale
  @param[in] TpoValue   T power on value

  @retval    number of microseconds
*/
UINT32
TpoToUs (
  UINT32 TpoScale,
  UINT32 TpoValue
  );

/**
PCIe controller Sku.
**/

typedef enum {
  EnumPchPcie = 0,
  EnumiTbtPcie = 1,
  EnumCpuPcie = 2,
  EnumPciSkuMax = 3
} PCI_SKU;

/*
  Initializes the following features in rootport and devices behind it:
  Maximum Payload Size (generic)
  Rootport packet split (proprietary)
  EonOfInterrupt forwarding (proprietary)
  Common Clock Configuration (generic)

  Generic: any code written according to PCIE Express base specification can do that.
  Proprietary: code uses registers and features that are specific to Intel silicon
  and probably only this Reference Code knows how to handle that.

  If OEM implemented generic feature enabling in his platform code or trusts Operating System
  to do it, then those features can be deleted from here.

  CCC requires link retrain, which takes a while. CCC must happen before L0s/L1 programming.
  If there was guarantee no code would access PCI while links retrain, it would be possible to skip this waiting

  @param[in] RpSegment  address of rootport on PCIe
  @param[in] RpBus      address of rootport on PCIe
  @param[in] RpDevice   address of rootport on PCIe
  @param[in] RpFunction address of rootport on PCIe
  @param[in] BusMin     minimum Bus number that can be assigned below this rootport
  @param[in] BusMax     maximum Bus number that can be assigned below this rootport
*/
VOID
RootportDownstreamConfiguration (
  UINT8                     RpSegment,
  UINT8                     RpBus,
  UINT8                     RpDevice,
  UINT8                     RpFunction,
  UINT8                     BusMin,
  UINT8                     BusMax,
  PCI_SKU                   PciSku
  );

/*
  Configures the following power-management related features in rootport and devices behind it:
  LTR limit (generic)
  LTR override (proprietary)
  Clock Power Management (generic)
  L1 substates (generic except for the override table)
  L1.LOW substate (proprietary)
  L0s and L1 (generic)

  Generic: any code written according to PCIE Express base specification can do that.
  Proprietary: code uses registers and features that are specific to Intel silicon
  and probably only this Reference Code knows how to handle that.

  If OEM implemented generic feature enabling in his platform code or trusts Operating System
  to do it, then those features can be deleted from here.

  @param[in] RpSegment                address of rootport on PCIe
  @param[in] RpBus                    address of rootport on PCIe
  @param[in] RpDevice                 address of rootport on PCIe
  @param[in] RpFunction               address of rootport on PCIe
  @param[in] BusMin                   minimal Bus number that can be assigned below this rootport
  @param[in] BusMax                   maximum Bus number that can be assigned below this rootport
  @param[in] PcieRpCommonConfig       a pointer to Pcie Root Port Common Config
  @param[in] AspmOverrideTableSize    size of override array
  @param[in] AspmOverrideTable        array of device that need exceptions in configuration
*/
VOID
RootportDownstreamPmConfiguration (
  UINT8                               RpSegment,
  UINT8                               RpBus,
  UINT8                               RpDevice,
  UINT8                               RpFunction,
  UINT8                               BusMin,
  UINT8                               BusMax,
  PCIE_ROOT_PORT_COMMON_CONFIG        *PcieRpCommonConfig,
  UINT32                              AspmOverrideTableSize,
  PCH_PCIE_DEVICE_OVERRIDE            *AspmOverrideTable
  );

typedef struct {
  UINT32 MaxSnoopLatencyValue         : 10;
  UINT32 MaxSnoopLatencyScale         : 3;
  UINT32 MaxNoSnoopLatencyValue       : 10;
  UINT32 MaxNoSnoopLatencyScale       : 3;
  UINT32 Reserved                     : 6;
} LTR_LIMIT;

/**
Configures the following power-management related features in rootport and devices behind it:
LTR limit (generic)
L0s and L1 (generic)

Generic: any code written according to PCIE Express base specification can do that.

@param[in] RpSegment                address of rootport on PCIe
@param[in] RpBus                    address of rootport on PCIe
@param[in] RpDevice                 address of rootport on PCIe
@param[in] RpFunction               address of rootport on PCIe
@param[in] PolicyLtr                table with MaxSnoop and MaxNoSnoop Latency values to be programmed
**/
VOID
TcssRootportDownstreamPmConfiguration(
  UINT8                     RpSegment,
  UINT8                     RpBus,
  UINT8                     RpDevice,
  UINT8                     RpFunction,
  LTR_LIMIT                 PolicyLtr
);

/**
  Get current PCIe link speed.

  @param[in] RpBase    Root Port base address
  @return Link speed
**/
UINT32
GetLinkSpeed (
  UINT64  RpBase
  );

/**
  Get max PCIe link speed supported by the root port.

  @param[in] RpBase    Root Port pci segment base address
  @return Max link speed
**/
UINT32
GetMaxLinkSpeed (
  UINT64 RpBase
  );

/**
  Checks if given PCI device is capable of Latency Tolerance Reporting

  @param[in] Sbdf            device's segment:bus:device:function coordinates

  @retval TRUE if yes
**/
BOOLEAN
IsLtrCapable (
  SBDF Sbdf
  );

/**
  Returns combination of two LTR override values
  The resulting LTR override separately chooses stricter limits for snoop and nosnoop

  @param[in] LtrA      LTR override values to be combined
  @param[in] LtrB      LTR override values to be combined

  @retval LTR override value
**/
LTR_OVERRIDE
CombineLtr (
  LTR_OVERRIDE LtrA,
  LTR_OVERRIDE LtrB
  );

/**
  Extended Virtual Channel Configuration
**/
typedef struct {
  UINT16 CapOffset;
  UINT8  ExtVcCount;
} MULTI_VC_SUPPORT;

/**
  This function checks if both root port and end point supports MultiVC and enables it only if both supports MultiVc

  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device

  @return    Multi Virtual channel capability offset and count supported by the Root Port
**/
MULTI_VC_SUPPORT
RecursiveMultiVcConfiguration (
  SBDF       Sbdf
  );

/**
  This function enables MultiVc support

  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device
  @param[in] MULTI_VC_SUPPORT               Multi Virtual channel capability offset and count
**/
VOID
EnableMultiVc (
  SBDF              Sbfd,
  MULTI_VC_SUPPORT  ExtVc
  );

#endif // _PCI_EXPRESS_HELPERS_LIB_H_
