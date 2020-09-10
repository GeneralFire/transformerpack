/** @file
  Pcie Root Port EBG info header

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

#ifndef _PCH_PCIERP_EBG_INFO_H_
#define _PCH_PCIERP_EBG_INFO_H_

//
// Number of PCIe ports per PCIe controller
//
#define PCH_PCIE_CONTROLLER_PORTS                     4u

//
// Number of PCIe lanes per PCIe port
//
#define PCH_PCIE_LANES_PER_PORT                       2

//
// Number of PCIe lanes per PCIe controller
//
#define PCH_PCIE_LANES_PER_CONTROLLER                 (PCH_PCIE_CONTROLLER_PORTS * PCH_PCIE_LANES_PER_PORT)
//
// Number of PCIe lanes registers per Root Port in PCIe controller
//
#define PCH_PCIE_LANES_RP0                           8
#define PCH_PCIE_LANES_RP1                           2
#define PCH_PCIE_LANES_RP2                           4
#define PCH_PCIE_LANES_RP3                           2

typedef enum RpIdxPerController {
  Rp0PerController = 0,
  Rp1PerController,
  Rp2PerController,
  Rp3PerController
} PCIE_RP_PER_CONTROLLER;
//
// PCH PCI Express Root Ports for single function devices in EBG
//

#define PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_1           8
#define PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_2           9
#define PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_3           10
#define PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_4           11
#define PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_5           12
#define PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_6           13
#define PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_7           14
#define PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_8           15
#define PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_9           16
#define PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_10          17
#define PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_11          18
#define PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_12          19
#define PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_13          26
#define PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_14          27
#define PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_15          28
#define PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_16          29
#define PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_17          14
#define PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_18          14
#define PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_19          14
#define PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_20          14

#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS         16

#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_1      0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_2      0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_3      0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_4      0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_5      0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_6      0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_7      0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_8      0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_9      0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_10     0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_11     0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_12     0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_13     0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_14     0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_15     0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_16     0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_17     0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_18     1
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_19     2
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_20     3


#endif
