/** @file
  HECI Transport driver core operations for use in PEI, DXE, and SMM.

 @copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2019 - 2020 Intel Corporation.

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

**/
#ifndef _HECI_TRANSPORT_CORE_H_
#define _HECI_TRANSPORT_CORE_H_

/**
  HECI MBAR registers definition

  This structure defines registers mapped at HECI MBAR.
**/
#define R_H_CB_WW                       0x00    ///< Circular Buffer Write Window
#define R_H_CSR                         0x04    ///< Host Control and Status Register
#define R_ME_CB_RW                      0x08    ///< Circular Buffer Read Window
#define R_ME_CSR_HA                     0x0C    ///< ME Control and Status Register (read only)

/**
  HECI_MBAR_REGS::R_H_CSR / R_ME_CSR_HA - Host / ME Control and Status Register
**/
#define B_CSR_INT_ENABLE_MASK           BIT0
#define N_CSR_INT_ENABLE_SHIFT          0
#define B_CSR_INT_STATUS_MASK           BIT1
#define N_CSR_INT_STATUS_SHIFT          1
#define B_CSR_INT_GENERATE_MASK         BIT2
#define N_CSR_INT_GENERATE_SHIFT        2
#define B_CSR_READY_MASK                BIT3
#define N_CSR_READY_SHIFT               3
#define B_CSR_RESET_MASK                BIT4
#define N_CSR_RESET_SHIFT               4
#define B_CSR_READ_POINTER_MASK         0x0000FF00
#define N_CSR_READ_POINTER_SHIFT        8
#define B_CSR_WRITE_POINTER_MASK        0x00FF0000
#define N_CSR_WRITE_POINTER_SHIFT       16
#define B_CSR_DEPTH_MASK                0xFF000000
#define N_CSR_DEPTH_SHIFT               24
#define GET_INT_ENABLE(Csr32)          ((Csr32&B_CSR_INT_ENABLE_MASK)>>N_CSR_INT_ENABLE_SHIFT)
#define SET_INT_ENABLE(Csr32, Value)   Csr32=(((Value<<N_CSR_INT_ENABLE_SHIFT)&B_CSR_INT_ENABLE_MASK)|\
                                       (Csr32&~B_CSR_INT_ENABLE_MASK))
#define GET_INT_STATUS(Csr32)          (Csr32&B_CSR_INT_STATUS_MASK)>>N_CSR_INT_STATUS_SHIFT)
#define SET_INT_STATUS(Csr32, Value)   Csr32=(((Value<<N_CSR_INT_STATUS_SHIFT)&B_CSR_INT_STATUS_MASK)|\
                                       (Csr32&~B_CSR_INT_STATUS_MASK))
#define GET_INT_GENERATE(Csr32)        ((Csr32&B_CSR_INT_GENERATE_MASK)>>N_CSR_INT_GENERATE_SHIFT)
#define SET_INT_GENERATE(Csr32, Value) Csr32=(((Value<<N_CSR_INT_GENERATE_SHIFT)&B_CSR_INT_GENERATE_MASK)|\
                                       (Csr32&~B_CSR_INT_GENERATE_MASK))
#define GET_READY(Csr32)               ((Csr32&B_CSR_READY_MASK)>>N_CSR_READY_SHIFT)
#define SET_READY(Csr32, Value)        Csr32=(((Value<<N_CSR_READY_SHIFT)&B_CSR_READY_MASK)|\
                                       (Csr32&~B_CSR_READY_MASK))
#define GET_RESET(Csr32)               ((Csr32&B_CSR_RESET_MASK)>>N_CSR_RESET_SHIFT)
#define SET_RESET(Csr32, Value)        Csr32=(((Value<<N_CSR_RESET_SHIFT)&B_CSR_RESET_MASK)|\
                                       (Csr32&~B_CSR_RESET_MASK))
#define GET_CB_READ_POINTER(Csr32)     ((Csr32&B_CSR_READ_POINTER_MASK)>>N_CSR_READ_POINTER_SHIFT)
#define GET_CB_WRITE_POINTER(Csr32)    ((Csr32&B_CSR_WRITE_POINTER_MASK)>>N_CSR_WRITE_POINTER_SHIFT)
#define GET_CB_DEPTH(Csr32)            ((Csr32&B_CSR_DEPTH_MASK)>>N_CSR_DEPTH_SHIFT)

#define MAX_CB_DEPTH_SIMICS            32

#endif // _HECI_TRANSPORT_CORE_H_
