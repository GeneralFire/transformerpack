/** @file
  PCH PCIe Root port device

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

@par Specification
**/
#ifndef _PCH_PCIE_ROOT_PORT_DEV_H_
#define _PCH_PCIE_ROOT_PORT_DEV_H_

/**
  PCIe Root port structure
  Stores information required to access to registers
  like base address, S:B:D:F etc.
  and definitions sepecific to root port.
**/
typedef struct {
  /**
    PCIe Root port PCI config space address
    in PCI Segment Library representation.
  **/
  UINT64                      PciCfgBaseAddr;
  /**
    PCIe Root port PCI Segment
  **/
  UINT8                       Segment;
  /**
    PCIe Root port PCI Bus
  **/
  UINT8                       Bus;
  /**
    PCIe Root port PCI Device
  **/
  UINT8                       Device;
  /**
    PCIe Root port PCI Function
  **/
  UINT8                       Function;
  /**
    PCIe Root port controller index
  **/
  UINT32                      RootPortIndex;
  /**
    PCIe Root port temporary minimum bus number
  **/
  UINT8                       TempPciBusMin;
  /**
    PCIe Root port temporary maximum bus number
  **/
  UINT8                       TempPciBusMax;
  /**
    PCIe Root port controller index
  **/
  UINT32                      CtrlIndex;
  /**
    PCIe Root private data used by FRU
  **/
  UINT32                      Private;

} PCH_RP_DEVICE;

#endif // _PCH_PCIE_ROOT_PORT_DEV_H_
