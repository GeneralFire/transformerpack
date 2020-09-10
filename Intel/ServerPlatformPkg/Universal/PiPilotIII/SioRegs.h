/** @file
  This file provides register definitions for SIO device.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2018 Intel Corporation. <BR>

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

#ifndef __SIO_REGS_H__
#define __SIO_REGS_H__

#define R_SIO_CLOCK_SELECT               0x2D
#define R_SIO_CLOCK_CONFIG               0x29

#define R_SIO_LOGICAL_DEVICE             0x07
#define   V_SIO_PSR                      0x00
#define   V_SIO_COM2                     0x01
#define   V_SIO_COM1                     0x02
#define   V_SIO_SWCP                     0x03
#define   V_SIO_GPIO                     0x04
#define   V_SIO_WDT                      0x05
#define   V_SIO_KCS3                     0x08
#define   V_SIO_KCS4                     0x09
#define   V_SIO_KCS5                     0x0A
#define   V_SIO_BT                       0x0B
#define   V_SIO_SMIC                     0x0C
#define   V_SIO_MAILBOX                  0x0D
#define   V_SIO_RTC                      0x0E
#define   V_SIO_SPI                      0x0F
#define   V_SIO_TAP                      0x10

#define R_SIO_CHIP_ID                    0x20
#define   V_SIO_CHIP_ID                  0x03

#define R_SIO_ACTIVATE                   0x30

#define R_SIO_BASE_ADDRESS_HIGH0         0x60
#define R_SIO_BASE_ADDRESS_LOW0          0x61
#define R_SIO_BASE_ADDRESS_HIGH1         0x62
#define R_SIO_BASE_ADDRESS_LOW1          0x63
#define R_SIO_BASE_ADDRESS_HIGH2         0x64
#define R_SIO_BASE_ADDRESS_LOW2          0x65
#define R_SIO_BASE_ADDRESS_HIGH3         0x66
#define R_SIO_BASE_ADDRESS_LOW3          0x67

#define R_SIO_PRIMARY_INTERRUPT_SELECT   0x70

#define R_SIO_DMA_CHANNEL_SELECT0        0x74
#define R_SIO_DMA_CHANNEL_SELECT1        0x75

#define   V_UNLOCK                       0x5A
#define   V_LOCK                         0xA5


#define PILOTIII_SWC_BASE_ADDRESS       0xA00
#define PILOTIII_PM1b_EVT_BLK_BASE_ADDRESS 0x0A80
#define PILOTIII_PM1b_CNT_BLK_BASE_ADDRESS 0x0A84
#define PILOTIII_GPE1_BLK_BASE_ADDRESS     0x0A86
#define PILOTIII_KCS3_DATA_BASE_ADDRESS    0x0CA4
#define PILOTIII_KCS3_CMD_BASE_ADDRESS     0x0CA5
#define PILOTIII_KCS4_DATA_BASE_ADDRESS    0x0CA2
#define PILOTIII_KCS4_CMD_BASE_ADDRESS     0x0CA3
#define PILOTIII_MAILBOX_BASE_ADDRESS      0x0600
#define PILOTIII_MAILBOX_MASK              0xFFE0

#define  SIO_DELAY    500     // 0.5 milli seconds

#endif
