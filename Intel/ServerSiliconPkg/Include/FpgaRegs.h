/** @file
  Register names for FPGA Device

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2017 Intel Corporation. <BR>

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

#ifndef _FPGA_REGS_H_
#define _FPGA_REGS_H_

//
// FPGA Device 0xBCBD PCI registes
//
#define R_FPGA_FME_BAR                         0x10
#define R_FPGA_VTD_BAR                         0x180

//
//  FPGA FME BAR MMIO registers
//
#define R_FME_MMIO_TMP_THRESHOLD               0x1008
#define B_FME_MMIO_TMP_THRESHOLD_1             (BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0)
#define B_FME_MMIO_TMP_THRESHOLD_1_EN          BIT7
#define B_FME_MMIO_TMP_THRESHOLD_2             (BIT14 | BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8)
#define N_FME_MMIO_TMP_THRESHOLD_2             8
#define B_FME_MMIO_TMP_THRESHOLD_2_EN          BIT15
#define B_FME_MMIO_TMP_THRESHOLD_1_POLICY      BIT44
#define N_FME_MMIO_TMP_THRESHOLD_1_POLICY      44
#define V_THRESHOLD_1_POLICY_90_P              0
#define V_THRESHOLD_1_POLICY_50_P              1

#define FME_ERROR_MASK   0x4008
#define FME_ERROR        0x4010
#define FME_FIRST_ERROR  0x4018
#define KTI_ERROR_STAT0  0x0390
#define KTI_ERROR_EN0    0x0394
#define KTI_ERROR_STAT1  0x0398
#define KTI_ERROR_EN1    0x039c
#define KTI_ERROR_STAT2  0x03a0
#define KTI_ERROR_EN2    0x03a4
#define KTI_ERROR_STAT3  0x03a8
#define KTI_ERROR_EN3    0x03ac
#define KTI_ERROR_STAT4  0x03b0
#define KTI_ERROR_EN4    0x03b4
#define KTI_ERROR_STAT5  0x03b8
#define KTI_ERROR_EN5    0x03bc
#define KTI_FERR         0x03c8
#define KTI_NERR         0x03cc
#endif
