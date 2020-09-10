/** @file
  Register names for DMI and OP-DMI

Conventions:

  - Register definition format:
    Prefix_[GenerationName]_[ComponentName]_SubsystemName_RegisterSpace_RegisterName
  - Prefix:
    Definitions beginning with "R_" are registers
    Definitions beginning with "B_" are bits within registers
    Definitions beginning with "V_" are meaningful values within the bits
    Definitions beginning with "S_" are register size
    Definitions beginning with "N_" are the bit position
  - [GenerationName]:
    Three letter acronym of the generation is used (e.g. SKL,KBL,CNL etc.).
    Register name without GenerationName applies to all generations.
  - [ComponentName]:
    This field indicates the component name that the register belongs to (e.g. PCH, SA etc.)
    Register name without ComponentName applies to all components.
    Register that is specific to -H denoted by "_PCH_H_" in component name.
    Register that is specific to -LP denoted by "_PCH_LP_" in component name.
  - SubsystemName:
    This field indicates the subsystem name of the component that the register belongs to
    (e.g. PCIE, USB, SATA, GPIO, PMC etc.).
  - RegisterSpace:
    MEM - MMIO space register of subsystem.
    IO  - IO space register of subsystem.
    PCR - Private configuration register of subsystem.
    CFG - PCI configuration space register of subsystem.
  - RegisterName:
    Full register name.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2017 Intel Corporation.

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
#ifndef _PCH_REGS_DMI_H_
#define _PCH_REGS_DMI_H_

//
// DMI Chipset Configuration Registers (PID:DMI)
//

//
// VC Configuration (Common)
//
#define B_PCH_DMI_PCR_V0CTL_EN               BIT31
#define B_PCH_DMI_PCR_V0CTL_ID               (7 << 24)                   ///< Bit[26:24]
#define N_PCH_DMI_PCR_V0CTL_ID               24
#define V_PCH_DMI_PCR_V0CTL_ETVM_MASK        0xFC00
#define V_PCH_DMI_PCR_V0CTL_TVM_MASK         0x7E
#define B_PCH_DMI_PCR_V0STS_NP               BIT1
#define B_PCH_DMI_PCR_V1CTL_EN               BIT31
#define B_PCH_DMI_PCR_V1CTL_ID               (0x0F << 24)                ///< Bit[27:24]
#define N_PCH_DMI_PCR_V1CTL_ID               24
#define V_PCH_DMI_PCR_V1CTL_ETVM_MASK        0xFC00
#define V_PCH_DMI_PCR_V1CTL_TVM_MASK         0xFE
#define B_PCH_DMI_PCR_V1STS_NP               BIT1

//
// Internal Link Configuration (DMI Only)
//
#define R_PCH_DMI_PCR_LCAP                   0x204C                      ///< Link Capabilities
#define B_PCH_DMI_PCR_LCAP_EL1               (BIT17 | BIT16 | BIT15)     ///< L1 Exit Latency
#define N_PCH_DMI_PCR_LCAP_EL1               15                          ///< L1 Exit Latency
#define V_PCH_DMI_PCR_LCAP_EL1_8US_16US      0x4                         ///< L1 Exit Latency = 8us to less than 16us
#define B_PCH_DMI_PCR_LCAP_EL0               (BIT14 | BIT13 | BIT12)     ///< L0 Exit Latency
#define N_PCH_DMI_PCR_LCAP_EL0               12                          ///< L0 Exit Latency
#define V_PCH_DMI_PCR_LCAP_EL0_256NS_512NS   0x3                         ///< L0 Exit Latency = 256ns to less than 512ns
#define B_PCH_DMI_PCR_LCAP_APMS              (BIT11 | BIT10)             ///< L0 is supported on DMI
#define B_PCH_DMI_PCR_LCAP_MLW               0x000003F0
#define B_PCH_DMI_PCR_LCAP_MLS               0x0000000F

#define R_PCH_DMI_PCR_LCAP2                  0x216C                      ///< Link Capabilities 2
typedef union {
  UINT32   Dword;
  struct {
    UINT32 Rsrvd0   : 1,  ///< 0
           SLSV     : 7,  ///< 7:1 Supported Link Speed Vector
           Rsrvd1   :24;  ///< 31:8
  } Bits;
} PCH_DMI_PCR_LCAP2;


#define R_PCH_DMI_PCR_LCTL2                  0x2170                      ///< Link Control 2
typedef union {
  UINT32   Dword;
  struct {
    UINT32 TLS    : 4,    ///< 0:3 Target Link Speed
           Rsrvd0 :28;    ///< 31:4
  } Bits;
} PCH_DMI_PCR_LCTL2;

//
// DMI Source Decode PCRs (Common)
//
#define R_PCH_DMI_PCR_IOT1              0x0E50                ///< I/O Trap Register 1
#define R_PCH_DMI_PCR_LPCIOD            0x0E70                ///< LPC I/O Decode Ranges
#define R_PCH_DMI_PCR_LPCIOE            0x0E74                ///< LPC I/O Enables

//
// Opi PHY registers
//
#define R_PCH_OPIPHY_PCR_0000           0x0000
#define R_PCH_OPIPHY_PCR_0110           0x0110
#define R_PCH_OPIPHY_PCR_0118           0x0118
#define R_PCH_OPIPHY_PCR_011C           0x011C
#define R_PCH_OPIPHY_PCR_0354           0x0354
#define R_PCH_OPIPHY_PCR_B104           0xB104
#define R_PCH_OPIPHY_PCR_B10C           0xB10C

#endif
