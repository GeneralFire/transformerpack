/** @file
  Support PCI chipset initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2016 Intel Corporation. <BR>

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

#ifndef  _EFI_PCI_SUPPORT_H_
#define  _EFI_PCI_SUPPORT_H_

#include <Protocol/PciRootBridgeIo.h>

#include <Guid/SetupVariable.h>

typedef struct {
  UINT8  PcieCapPtr;
  UINT8  Function;
  UINT8  Device;
  UINT8  Bus;
  UINT16 PcieLnkCap;
  UINT16 PcieDevCap;
  //Added to Support AtomicOp Request-->Start
  UINT16 PcieDevCap2;
  //Added to Support AtomicOp Request-->End
} PCIE_CAP_INFO;

typedef struct {
  INTN            Top;
  PCIE_CAP_INFO   PcieCapInfo[FixedPcdGet32(PcdMaxNestedLevel)];
} PCIE_STACK;

extern  PCIE_STACK  mPcieStack;

BOOLEAN
IsPciDevicePresent (
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *PciRootBridgeIo,
  OUT PCI_TYPE00                      *Pci,
  UINT8                               Bus,
  UINT8                               Device,
  UINT8                               Func
  );


#endif
