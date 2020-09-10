/** @file
  UEFI variables used by DXE to pass PCI resource reconfiguration request to PEI.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#ifndef _SOCKET_PCI_RESOURCE_CONFIG_DATA_H_
#define _SOCKET_PCI_RESOURCE_CONFIG_DATA_H_

extern EFI_GUID gEfiSocketPciResourceDataGuid;

/**
 * PCI bus resource reconfiguration request structure.
 **/
#define SYSTEM_PCI_BUS_CONFIGURATION_DATA_NAME      L"SystemPciBusConfigData"

//
// This variable is used as a communication vehicle by DXE driver to an OEM hook which updates the KTI resource map.
// It provides size of PCI bus number pools for particular IIO stacks required for the system. KTI creates default
// initial resources allocation which may appear not suficient to enumerate all PCI devices in some stack, while
// other may be empty. This request does rebalancing of the resources.
//
typedef struct {
  struct {
    UINT8  Segment;                                  ///< Remember segment, if it changes reset everything
    UINT8  StackPciBusPoolSize[MAX_LOGIC_IIO_STACK]; ///< Number of bus numbers needed for IIO stack
  } Socket[MAX_SOCKET];
} SYSTEM_PCI_BUS_CONFIGURATION;


/**
 * PCI MMIO and IO resource reconfiguration request structure.
 **/
#define SYSTEM_PCI_RESOURCE_CONFIGURATION_DATA_NAME L"SystemPciResourceConfigData"

typedef struct {
  UINT64 Base;    ///< Base (starting) address of a range (I/O, 32 and 64-bit mmio regions)
  UINT64 Limit;   ///< Limit (last valid) address of a range
} PCIE_BASE_LIMIT;

typedef struct {
  PCIE_BASE_LIMIT Io;        ///< Base and limit of I/O range assigned to entity
  PCIE_BASE_LIMIT LowMmio;   ///< Base and limit of low MMIO region for entity
  PCIE_BASE_LIMIT HighMmio;  ///< Base and limit of high (64-bit) MMIO region for entity
} PCI_BASE_LIMITS;

typedef struct {
  PCI_BASE_LIMITS SocketLimits;                     ///< Base and Limit of all PCIe resources for the socket
  PCI_BASE_LIMITS StackLimits[MAX_LOGIC_IIO_STACK]; ///< Base and Limit of all PCIe resources for each stack of the socket
} SOCKET_PCI_BASE_LIMITS;

typedef struct {
  //
  // Save basic system configuration parameters along with the resource map to detect a change.
  // Remember low and high I/O memory range when saving recource configuration. It is used to verify
  // whether system memory map changed. Remember also stacks configured when creating the map.
  // If anything changed reset the system PCI resource configuration.
  //
  UINT64                 MmioHBase;
  UINT64                 MmioHLimit;
  UINT32                 MmioLBase;
  UINT32                 MmioLLimit;
  UINT16                 IoBase;
  UINT16                 IoLimit;
  UINT16                 StackPresentBitmap[MAX_SOCKET];
  //
  // Used by the PciHostBridge DXE driver, these variables don't need to be exposed through setup options
  // The variables are used as a communication vehicle from the PciHostBridge DXE driver to an OEM hook
  // which updates the KTI resource map.
  //
  SOCKET_PCI_BASE_LIMITS Socket[MAX_SOCKET];        ///< Base and limit of all PCIe resources for each socket
} SYSTEM_PCI_BASE_LIMITS;


#endif // _SOCKET_PCI_RESOURCE_CONFIG_DATA_H_

