/** @file
  Header file for Emmitsburg PCH devices PCI Bus Device Function map.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation.

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
#ifndef _PCH_BDF_ASSIGNMENT_H_
#define _PCH_BDF_ASSIGNMENT_H_

#define NOT_PRESENT                     0xFF

#define MAX_SATA_CONTROLLER             3

//
// PCH PCIe COntrollers (D8-18,26-29:F0)
//
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_1          8
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_2          9
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_3          10
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_4          11
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_5          12
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_6          13
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_7          14
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_8          15
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_9          16
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_10         17
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_11         18
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_12         19
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_13         26
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_14         27
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_15         28
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_16         29
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_17         NOT_PRESENT
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_18         NOT_PRESENT
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_19         NOT_PRESENT
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_20         NOT_PRESENT
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_21         NOT_PRESENT
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_22         NOT_PRESENT
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_23         NOT_PRESENT
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_24         NOT_PRESENT
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_25         NOT_PRESENT
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_26         NOT_PRESENT
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_27         NOT_PRESENT
#define PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_28         NOT_PRESENT

#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_1        0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_2        0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_3        0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_4        0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_5        0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_6        0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_7        0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_8        0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_9        0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_10       0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_11       0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_12       0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_13       0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_14       0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_15       0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_16       0
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_17       NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_18       NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_19       NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_20       NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_21       NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_22       NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_23       NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_24       NOT_PRESENT

//
// USB3 (XHCI) Controller PCI config
//
#define PCI_DEVICE_NUMBER_PCH_XHCI                    20
#define PCI_FUNCTION_NUMBER_PCH_XHCI                  0

//
// xDCI (OTG) USB Device Controller (not present in EBG)
//
#define PCI_DEVICE_NUMBER_PCH_XDCI                    NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_XDCI                  NOT_PRESENT

//
//  Thermal Device
//
#define PCI_DEVICE_NUMBER_PCH_THERMAL                 NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_THERMAL               NOT_PRESENT

//
// CNVi WiFi (not present in EBG)
//
#define PCI_DEVICE_NUMBER_PCH_CNVI_WIFI               NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_CNVI_WIFI             NOT_PRESENT

//
// South IEH Controller
//
#define PCI_DEVICE_NUMBER_PCH_IEH                     20
#define PCI_FUNCTION_NUMBER_PCH_IEH                   4

//
// CSME HECI #1
//
#define PCI_DEVICE_NUMBER_PCH_HECI1                   22
#define PCI_FUNCTION_NUMBER_PCH_HECI1                 0

//
// CSME HECI #2
//
#define PCI_DEVICE_NUMBER_PCH_HECI2                   22
#define PCI_FUNCTION_NUMBER_PCH_HECI2                 1

//
// CSME IDE-Redirection (IDE-R)
//
#define PCI_DEVICE_NUMBER_PCH_IDER                    22
#define PCI_FUNCTION_NUMBER_PCH_IDER                  2

//
// CSME Keyboard and Text (KT) Redirection
//
#define PCI_DEVICE_NUMBER_PCH_KTR                     22
#define PCI_FUNCTION_NUMBER_PCH_KTR                   3

//
// CSME HECI #3
//
#define PCI_DEVICE_NUMBER_PCH_HECI3                   22
#define PCI_FUNCTION_NUMBER_PCH_HECI3                 4

//
// CSME HECI #4
//
#define PCI_DEVICE_NUMBER_PCH_HECI4                   22
#define PCI_FUNCTION_NUMBER_PCH_HECI4                 5

//
// CSME MROM
//
#define PCI_DEVICE_NUMBER_PCH_MROM                    22
#define PCI_FUNCTION_NUMBER_PCH_MROM                  6

//
// SATA Controllers Registers (D23,24,25:F0)
//
#define PCI_DEVICE_NUMBER_PCH_SATA_1                  23
#define PCI_FUNCTION_NUMBER_PCH_SATA_1                0
#define PCI_DEVICE_NUMBER_PCH_SATA_2                  24
#define PCI_FUNCTION_NUMBER_PCH_SATA_2                0
#define PCI_DEVICE_NUMBER_PCH_SATA_3                  25
#define PCI_FUNCTION_NUMBER_PCH_SATA_3                0

//
//  PCH LP & H Serial IO I2C #0 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C0          NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C0        NOT_PRESENT

//
//  PCH LP & H Serial IO I2C #1 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C1          NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C1        NOT_PRESENT

//
//  PCH LP & H Serial IO I2C #2 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C2          NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C2        NOT_PRESENT

//
//  PCH LP & H Serial IO I2C #3 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C3          NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C3        NOT_PRESENT

//
//  PCH LP & H Serial IO I2C #4 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C4          NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C4        NOT_PRESENT

//
//  PCH LP & H Serial IO I2C #5 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C5          NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C5        NOT_PRESENT

//
//  PCH LP & H Serial IO I2C #6 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C6          NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C6        NOT_PRESENT

//
//  PCH LP & H Serial IO I2C #7 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C7          NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C7        NOT_PRESENT

//
//  PCH LP & H Serial IO SPI #0 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI0          NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI0        NOT_PRESENT

//
//  PCH LP & H Serial IO SPI #1 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI1          NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI1        NOT_PRESENT

//
//  PCH LP & H Serial IO SPI #2 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI2          NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI2        NOT_PRESENT

//
//  PCH LP & H Serial IO SPI #3 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI3          NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI3        NOT_PRESENT

//
//  PCH LP & H Serial IO SPI #4 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI4          NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI4        NOT_PRESENT

//
//  PCH LP & H Serial IO SPI #5 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI5          NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI5        NOT_PRESENT
#define PCI_DEVICE_NUMBER_PCH_H_SERIAL_IO_SPI5        NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_H_SERIAL_IO_SPI5      NOT_PRESENT

//
//  PCH LP & H Serial IO SPI #6 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI6          NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI6        NOT_PRESENT
#define PCI_DEVICE_NUMBER_PCH_H_SERIAL_IO_SPI6        NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_H_SERIAL_IO_SPI6      NOT_PRESENT

//
//  PCH N Serial IO SPI #0 Controller
//
#define PCI_DEVICE_NUMBER_PCH_N_SERIAL_IO_SPI0        NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_N_SERIAL_IO_SPI0      NOT_PRESENT

//
//  PCH N Serial IO SPI #1 Controller
//
#define PCI_DEVICE_NUMBER_PCH_N_SERIAL_IO_SPI1        NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_N_SERIAL_IO_SPI1      NOT_PRESENT

//
//  PCH N Serial IO SPI #2 Controller
//
#define PCI_DEVICE_NUMBER_PCH_N_SERIAL_IO_SPI2        NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_N_SERIAL_IO_SPI2      NOT_PRESENT

//
//  PCH N Serial IO SPI #3 Controller
//
#define PCI_DEVICE_NUMBER_PCH_N_SERIAL_IO_SPI3        NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_N_SERIAL_IO_SPI3      NOT_PRESENT

//
//  PCH N Serial IO SPI #4 Controller
//
#define PCI_DEVICE_NUMBER_PCH_N_SERIAL_IO_SPI4        NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_N_SERIAL_IO_SPI4      NOT_PRESENT

//
//  PCH N Serial IO SPI #5 Controller
//
#define PCI_DEVICE_NUMBER_PCH_N_SERIAL_IO_SPI5        NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_N_SERIAL_IO_SPI5      NOT_PRESENT

//
//  PCH N Serial IO SPI #6 Controller
//
#define PCI_DEVICE_NUMBER_PCH_N_SERIAL_IO_SPI6        NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_N_SERIAL_IO_SPI6      NOT_PRESENT

//
//  PCH LP & H Serial IO UART #0 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART0         30
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART0       0

//
//  PCH LP & H Serial IO UART #1 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART1         30
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART1       1

//
//  PCH LP & H Serial IO UART #2 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART2         NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART2       NOT_PRESENT

//
//  PCH LP & H Serial IO UART #3 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART3         NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART3       NOT_PRESENT

//
//  PCH LP & H Serial IO UART #4 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART4         NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART4       NOT_PRESENT

//
//  PCH LP & H Serial IO UART #5 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART5         NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART5       NOT_PRESENT

//
//  PCH LP & H Serial IO UART #6 Controller
//
#define PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART6         NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART6       NOT_PRESENT

//
// DMA-SMBus Controller
//
#define PCI_DEVICE_NUMBER_PCH_DMA_SMBUS               21
#define PCI_FUNCTION_NUMBER_PCH_DMA_SMBUS             0

//
// TSN GbE Controller
//
#define PCI_DEVICE_NUMBER_PCH_TSN                     NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_TSN                   NOT_PRESENT

//
// LPC Controller
//
#define PCI_DEVICE_NUMBER_PCH_LPC                     31
#define PCI_FUNCTION_NUMBER_PCH_LPC                   0

//
// eSPI Controller
//
#define PCI_DEVICE_NUMBER_PCH_ESPI                    31
#define PCI_FUNCTION_NUMBER_PCH_ESPI                  0

//
// Primary to Sideband (P2SB) Bridge
//
#define PCI_DEVICE_NUMBER_PCH_P2SB                    31
#define PCI_FUNCTION_NUMBER_PCH_P2SB                  1

//
// PMC (D31:F2)
//
#define PCI_DEVICE_NUMBER_PCH_PMC                     31
#define PCI_FUNCTION_NUMBER_PCH_PMC                   2

//
// PMC SSRAM Registers
//
#define PCI_DEVICE_NUMBER_PCH_PMC_SSRAM               NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_PMC_SSRAM             NOT_PRESENT

//
// HD-A Controller
//
#define PCI_DEVICE_NUMBER_PCH_HDA                     31
#define PCI_FUNCTION_NUMBER_PCH_HDA                   3

//
// SMBus Controller
//
#define PCI_DEVICE_NUMBER_PCH_SMBUS                   31
#define PCI_FUNCTION_NUMBER_PCH_SMBUS                 4

//
// SPI Controller (D31:F5)
//
#define PCI_DEVICE_NUMBER_PCH_SPI                     31
#define PCI_FUNCTION_NUMBER_PCH_SPI                   5

//
// Gigabit Ethernet LAN Controller (D31:F6)
//
#define PCI_DEVICE_NUMBER_GBE                         31
#define PCI_FUNCTION_NUMBER_GBE                       6

//
// PCH TraceHub (D31:F7)
//
#define PCI_DEVICE_NUMBER_PCH_TRACE_HUB               31
#define PCI_FUNCTION_NUMBER_PCH_TRACE_HUB             7

//
// ISH Controller (not present in EBG)
//
#define PCI_DEVICE_NUMBER_PCH_ISH                     NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_ISH                   NOT_PRESENT

//
//  SCS SDCARD Controller PCI config (not present in EBG)
//
#define PCI_DEVICE_NUMBER_PCH_SCS_SDCARD              NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_SCS_SDCARD            NOT_PRESENT

//
//  SCS eMMC Controller PCI config (not present in EBG)
//
#define PCI_DEVICE_NUMBER_PCH_SCS_EMMC                NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_SCS_EMMC              NOT_PRESENT

//
//  SCS UFS Controller PCI config (not present in EBG)
//
#define PCI_DEVICE_NUMBER_PCH_SCS_UFS0                NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_SCS_UFS0              NOT_PRESENT
#define PCI_DEVICE_NUMBER_PCH_SCS_UFS1                NOT_PRESENT
#define PCI_FUNCTION_NUMBER_PCH_SCS_UFS1              NOT_PRESENT

#endif // _PCH_BDF_ASSIGNMENT_H_
