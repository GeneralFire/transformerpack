/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2017 Intel Corporation. <BR>

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

#ifndef _PCIE_MMCFG_TABLE_H_
#define _PCIE_MMCFG_TABLE_H_

#define PCIE_MMCFG_TABLE_SIGNATURE   0x4746434d

//
// PCIE MMCFG Table definition
// This table was based on PCI Firmware Spec Rev 3.1
//

typedef struct _PCIE_MMCFG_HEADER_TYPE {
  UINT8                         Signature[4];       // "MCFG" Signature For this Table
  UINT32                        Length;             // Length, in bytes, include base address allocation structures.
  UINT8                         Revision;           // "1"
  UINT8                         SegMax;             // The Maximum number of Segments
  UINT16                        ValidSegMap;        // Valid Segment Bit Map, LSB Bit0 for Seg0, bit1 for seg1 ...
  UINT8                         Reserved[4];        // Reserved
} PCIE_MMCFG_HEADER_TYPE;

typedef struct _PCIE_MMCFG_BASE_ADDRESS_TYPE {
  UINT32                        BaseAddressL;       // Processor-relative Base Address (Lower 32-bit) for the Enhanced Configuration Access Mechanism
  UINT32                        BaseAddressH;       // Processor-relative Base Address (Upper 32-bit) for the Enhanced Configuration Access Mechanism
  UINT16                        Segment;            // PCI Segment Group Number. Default is 0.
  UINT8                         StartBus;           // Start PCI Bus number decoded by the Host bridge
  UINT8                         EndBus;             // End PCI Bus number decoded by the Host bridge
  UINT8                         Reserved[4];        // Reserved
} PCIE_MMCFG_BASE_ADDRESS_TYPE;

typedef struct _PCIE_MMCFG_TABLE_TYPE {
  PCIE_MMCFG_HEADER_TYPE        Header;             // The header of MMCFG Table
  PCIE_MMCFG_BASE_ADDRESS_TYPE  MmcfgBase[1];       // First Array of base address allocation structures.
} PCIE_MMCFG_TABLE_TYPE;

#endif // _PCIE_MMCFG_TABLE_H_
