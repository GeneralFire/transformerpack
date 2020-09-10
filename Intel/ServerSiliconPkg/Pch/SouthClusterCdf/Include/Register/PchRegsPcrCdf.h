/** @file
  Register names for CDF SC private chipset register
  Conventions:
  Prefixes:
  Definitions beginning with "R_" are registers
  Definitions beginning with "B_" are bits within registers
  Definitions beginning with "V_" are meaningful values within the bits
  Definitions beginning with "S_" are register sizes
  Definitions beginning with "N_" are the bit position
  In general, PCH registers are denoted by "_PCH_" in register names
  Registers / bits that are different between PCH generations are denoted by
  _PCH_[generation_name]_" in register/bit names.
  Registers / bits that are different between SKUs are denoted by "_[SKU_name]"
  at the end of the register/bit names
  Registers / bits of new devices introduced in a PCH generation will be just named
  as "_PCH_" without [generation_name] inserted.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#ifndef _PCH_REGS_PCR_CDF_H_
#define _PCH_REGS_PCR_CDF_H_

/**
  Definition for SBI PID
  The PCH_SBI_PID defines the PID for PCR MMIO programming and PCH SBI programming as well.
**/
#define PID_ESPISPI                            0x93
#define PID_ICLK                               0x99
#define PID_PMC                                0x82
#define PID_XHCI                               0xA2
#define PID_PCIE0_P0                           0xB4 //PCIE0.P0 in CDF
#define PID_PCIE0_P1                           0x8D //PCIE0.P1 in CDF
#define PID_PCIE0_P2                           0x8E //PCIE0.P2 in CDF
#define PID_PCIE0_P3                           0x8F //PCIE0.P3 in CDF
#define PID_PCIE1_P0                           0xB3 //PCIE1.P0 in CDF
#define PID_PCIE1_P1                           0x8A //PCIE1.P1 in CDF
#define PID_PCIE1_P2                           0x8B //PCIE1.P2 in CDF
#define PID_PCIE1_P3                           0x8C //PCIE1.P3 in CDF
#define PID_PCIE2_P0                           0x71 //PCIE2.P0 in CDF
#define PID_PCIE2_P1                           0x72 //PCIE2.P1 in CDF
#define PID_PCIE2_P2                           0x73 //PCIE2.P2 in CDF
#define PID_PCIE2_P3                           0x74 //PCIE2.P3 in CDF
#define PID_SATA0                              0xB5
#define PID_SATA1                              0xB6
#define PID_SATA2                              0xB1
#define PID_FIA                                0xB0
#define PID_USB2                               0xA7
#define PID_LPC                                0xD2
#define PID_SMB                                0xCF
#define PID_P2S                                0xD4
#define PID_ITSS                               0xD0
#define PID_PSF0                               0x24
#define PID_PSF1                               0xA9
#define PID_PSF2                               0xA0
#define PID_PSF3                               0xC6
#define PID_PSF_CSME                           0xE3
#define PID_PSF_IE                             0xF3
#define PID_PSF_SATA0                          0xB7
#define PID_PSF_SATA1                          0xB8
#define PID_PSF_SATA2                          0xB2
#define PID_PSFS                               0xAA
#define PID_MEPTIO                             0xE4
#define PID_DMI                                0x4D //RLINK in CDF
#define PID_IEPTIO                             0xF4
#define PID_DCI                                0xA8
#define PID_NPK                                0x88
#define PID_GPIOCOM0                           0xC5 //GPIOE
#define PID_GPIOCOM1                           0xC2 //GPIOW
#define PID_MODPHY0                            0xB9 //White Mountain shared by SATA and PCIE
#define PID_MODPHY1                            0xBA //White Mountain shared by SATA and USB
#define PID_MODPHY2                            0xBB //White Mountain dedicated to PCIe
#define PID_PSTH                               0x89
#define PID_EMMC                               0xD6
#define PID_RTC_HOST                           0xD1
#define PID_TAP2IOSFSB                         0x83
#define PID_PMCBR                              0x85
#define PID_HOTHAM                             0xF1
#define PID_CSMERTC                            0xED
#define PID_DFXAGG                             0x84
#define PID_CTB2                               0xDA
#define PID_CTB1                               0xD8
#define PID_CTB0                               0xD7
#define PID_MCSMB                              0xCC
//
// SC_CONV
// remove flags after sync to client
//
#define  PID_CSME_PSF                          PID_PSF_CSME
#define  PID_CSME12                            PID_MEPTIO

typedef  UINT8                                 PCH_SBI_PID;

#endif
