/** @file
  This file provides register definitions for SIO device.

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

#ifndef __SIO_REGS_H__
#define __SIO_REGS_H__

#define R_SIO_CLOCK_SELECT               0x2D
#define R_SIO_CLOCK_CONFIG               0x29

#define R_SIO_LOGICAL_DEVICE             0x07
#define   V_SIO_FLOPPY                   0x00
#define   V_SIO_PARA                     0x01
#define   V_SIO_COM1                     0x02
#define   V_SIO_COM2                     0x03
#define   V_SIO_MOUSE                    0x05
#define   V_SIO_KYBD                     0x06
#define   V_SIO_GPIO                     0x07
#define   V_SIO_MAILBOX                  0x0E

#define R_SIO_ACTIVATE                   0x30
#define R_SIO_BASE_ADDRESS_HIGH0         0x60
#define R_SIO_BASE_ADDRESS_LOW0          0x61
#define R_SIO_BASE_ADDRESS_HIGH1         0x62
#define R_SIO_BASE_ADDRESS_LOW1          0x63
#define R_SIO_PRIMARY_INTERRUPT_SELECT   0x70
#define R_SIO_DMA_CHANNEL_SELECT         0x74

#define V_UNLOCK      0xA5
#define V_LOCK        0xAA

#define AST2500_LOGICAL_DEVICE            R_SIO_LOGICAL_DEVICE
#define AST2500_ACTIVATE                  R_SIO_ACTIVATE
#define AST2500_BASE_ADDRESS_HIGH0        R_SIO_BASE_ADDRESS_HIGH0
#define AST2500_BASE_ADDRESS_LOW0         R_SIO_BASE_ADDRESS_LOW0
#define AST2500_BASE_ADDRESS_HIGH1        R_SIO_BASE_ADDRESS_HIGH1
#define AST2500_BASE_ADDRESS_LOW1         R_SIO_BASE_ADDRESS_LOW1
#define AST2500_PRIMARY_INTERRUPT_SELECT  R_SIO_PRIMARY_INTERRUPT_SELECT

#define SIORE_30      0x30
#define SIORE_60      0x60
#define SIORE_61      0x61

#endif
