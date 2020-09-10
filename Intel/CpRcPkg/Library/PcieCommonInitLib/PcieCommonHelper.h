/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2020 Intel Corporation. <BR>

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

#ifndef _PCI_COMMON_LIB_HELPER_H_
#define _PCI_COMMON_LIB_HELPER_H_


/******************************************************************************
 * Definitions.
 ******************************************************************************/
typedef enum {
  PciCapPhase0 = 0,
  PciCapPhase1 = 1,
  PciCapPhaseMax
} PCI_CAP_PHASE;


/******************************************************************************
 * Function prototypes.
 ******************************************************************************/
/**
  Initialize rootport in a PCI host (stack) and its downstream devices.

  @param[in]     Phase        - Phase of PCI scan.
  @param[in,out] HostInfoPtr  - Pointer to PCI host (aka stack) info structure.
  @param[in,out] RootInfoPtr  - Pointer to rootport info in PCI host.
  @param[in]     ParAddrPtr   - Pointer to parent address when function called recursively, NULL for first call.
  @param[in]     BrAddrPtr    - Pointer to bridge address when function called recursively, NULL for first call.
  @param[in]     CleanTmpFlag - True to clean bus assigned and restore original values,
                                False to keep assigned bus numbers.
**/
VOID
PciInitBridgeDownstreamDevices (
  IN     PCI_CAP_PHASE             Phase,
  IN OUT PCIE_LIB_HOST_BRIDGE_INF *HostInfoPtr,
  IN OUT PCIE_LIB_ROOT_PORT_INF   *RootInfoPtr,
  IN     USRA_ADDRESS             *ParAddrPtr,
  IN     USRA_ADDRESS             *BriAddrPtr,
  IN     BOOLEAN                   CleanTmpBus
  );

/**
  Find the offset in PCI device config space of PCIe capabilities structure of given ID.
  Example CAPID list:
    0x01 = PCI Power Management Interface
    0x04 = Slot Identification
    0x05 = MSI Capability
    0x10 = PCI Express Capability
  See "PCI Code and ID Assignment Specification" Table 2-1: Capability IDs available at
  pcisig.com for full list of PCIe capabilities.

  @param[in] PciAddr  PCI device address.
  @param[in] CapId    CAPID to search for.

  @return Capability structure offset is returned, or zero if CAPID not found.
**/
UINT8
PciFindCapId (
  IN USRA_ADDRESS *PciAddr,
  IN UINT8         CapId
  );

/**
  Find the offset in PCI device config space of capabilities structure of given ID.
  Example Extended CAPID list:
    0x0001 = Advanced Error Reporting capability
    0x0002 = Virtual Channel capability
    0x0003 = Device Serial Number capability
    0x0004 = Power Budgeting capability
    0x001F = Precision Time Measurement capability
  See "PCI Code and ID Assignment Specification" Table 3-1: Extended Capability IDs
  available at pcisig.com for full list of extended PCIe capabilities.

  @param[in] PciAddr  PCI device address.
  @param[in] CapId    CAPID to search for.

  @return Capability structure offset is returned, or zero if CAPID not found.
**/
UINT16
PciFindExtendedCapId (
  IN USRA_ADDRESS *PciAddr,
  IN UINT16        CapId
  );

/**
  Check PCI config space, whether PCI device is present.

  @param[in]  PciAddrPtr Pointer to USRA address of PCI device config space.
  @param[out] PciHdrPtr   Optional pointer to a buffer for PCI device header.

  @return If PCI device at PcieAddrPtr exists true is returned, otherwise false.
**/
BOOLEAN
PciIsDevicePresent (
  IN     USRA_ADDRESS     *PciAddrPtr,
     OUT PCI_TYPE_GENERIC *PciHdrPtr
  );

#endif  // _PCI_COMMON_LIB_HELPER_H_

