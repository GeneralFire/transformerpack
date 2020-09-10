/** @file
  Register names for enterprise root port devices

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
  Copyright 2019 - 2020 Intel Corporation.

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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/
#ifndef _ENTERPRISE_PCIE_RP_REGS_H_
#define _ENTERPRISE_PCIE_RP_REGS_H_

#include "Generated/EnterprisePcieCfgRegs.h"
#include "Generated/EnterprisePcieMmioExpptmbarRegs.h"
#include "Generated/EnterprisePcieMmioExpptmbarSbRegs.h"
#include "Generated/EnterprisePcieMsgRegs.h"

#define V_PCH_H_PCIE_CFG_DID_PORT0              0x1BB8  ///< PCI Express Root Port #0,  EBG
#define V_PCH_H_PCIE_CFG_DID_PORT1              0x1BB9  ///< PCI Express Root Port #1,  EBG
#define V_PCH_H_PCIE_CFG_DID_PORT2              0x1BBA  ///< PCI Express Root Port #2,  EBG
#define V_PCH_H_PCIE_CFG_DID_PORT3              0x1BBB  ///< PCI Express Root Port #3,  EBG
#define V_PCH_H_PCIE_CFG_DID_PORT4              0x1BBC  ///< PCI Express Root Port #4,  EBG
#define V_PCH_H_PCIE_CFG_DID_PORT5              0x1BBD  ///< PCI Express Root Port #5,  EBG
#define V_PCH_H_PCIE_CFG_DID_PORT6              0x1BBE  ///< PCI Express Root Port #6,  EBG
#define V_PCH_H_PCIE_CFG_DID_PORT7              0x1BBF  ///< PCI Express Root Port #7,  EBG
#define V_PCH_H_PCIE_CFG_DID_PORT8              0x1BB0  ///< PCI Express Root Port #8,  EBG
#define V_PCH_H_PCIE_CFG_DID_PORT9              0x1BB1  ///< PCI Express Root Port #9,  EBG
#define V_PCH_H_PCIE_CFG_DID_PORT10             0x1BB2  ///< PCI Express Root Port #10, EBG
#define V_PCH_H_PCIE_CFG_DID_PORT11             0x1BB3  ///< PCI Express Root Port #11, EBG
#define V_PCH_H_PCIE_CFG_DID_PORT12             0x1BB4  ///< PCI Express Root Port #12, EBG
#define V_PCH_H_PCIE_CFG_DID_PORT13             0x1BB5  ///< PCI Express Root Port #13, EBG
#define V_PCH_H_PCIE_CFG_DID_PORT14             0x1BB6  ///< PCI Express Root Port #14, EBG
#define V_PCH_H_PCIE_CFG_DID_PORT15             0x1BB7  ///< PCI Express Root Port #15, EBG

#define V_PCH_PCIE_CFG_DEVCTL2_CTV_DEFAULT              0x0
#define V_PCH_PCIE_CFG_DEVCTL2_CTV_50US_100US           0x1
#define V_PCH_PCIE_CFG_DEVCTL2_CTV_1MS_10MS             0x2
#define V_PCH_PCIE_CFG_DEVCTL2_CTV_16MS_55MS            0x5
#define V_PCH_PCIE_CFG_DEVCTL2_CTV_65MS_210MS           0x6
#define V_PCH_PCIE_CFG_DEVCTL2_CTV_260MS_900MS          0x9
#define V_PCH_PCIE_CFG_DEVCTL2_CTV_1S_3P5S              0xA
#define V_PCH_PCIE_CFG_DEVCTL2_CTV_4S_13S               0xD
#define V_PCH_PCIE_CFG_DEVCTL2_CTV_17S_64S              0xE
#define V_PCH_PCIE_CFG_LINKCTL2_TLS_GEN1                 0x1
#define V_PCH_PCIE_CFG_LINKCTL2_TLS_GEN2                 0x2
#define V_PCH_PCIE_CFG_LINKCTL2_TLS_GEN3                 0x3
#define V_PCH_PCIE_CFG_PPBIFCTL_PRIV_BIFCTL0_2_2_2_2      0
#define V_PCH_PCIE_CFG_PPBIFCTL_PRIV_BIFCTL0_2_2_4        1
#define V_PCH_PCIE_CFG_PPBIFCTL_PRIV_BIFCTL0_4_2_2        2
#define V_PCH_PCIE_CFG_PPBIFCTL_PRIV_BIFCTL0_4_4          3
#define V_PCH_PCIE_CFG_PPBIFCTL_PRIV_BIFCTL0_8            4
#define V_PCH_PCIE_CFG_PPBIFCTL_PRIV_BIFCTL0_UNSUPPORTED  0xFFFFFFFF

#define V_PCH_PCIE_CFG_PIPECTL2_L1P0_PSMAP                0x2

#define V_PCH_PCIE_CFG_AERCAPHDR_CV                       0x1
#define V_PCH_PCIE_CFG_ACSCAPHDR_CV                       0x1
#define V_PCH_PCIE_CFG_VC0RCTL_TCVCM                      0x7F
#define V_PCH_PCIE_CFG_VC0RCTL_MVC                        0x1

#define V_PCH_PCIE_CFG_VC1RCTL_MVC                        0x8000000E

#define V_PCH_PCIE_CFG_PTMECH_CV                          0x1
#define V_PCH_PCIE_CFG_L1PMSECH_CV                        0x1
#define V_PCH_PCIE_CFG_SECEXPCAP_CV                       0x1
#define V_PCH_PCIE_CFG_DPCECH_CV                          0x1

#define V_PCH_PCIE_CFG_LTSSMSMSTS_LTSSMSTATESUB_DETRDY             0x01
#define V_PCH_PCIE_CFG_LTSSMSMSTS_LTSSMSTATESUB_DETRDYECINP1CG     0x0E
#define V_PCH_PCIE_CFG_LTSSMSMSTS_LTSSMSTATESUB_L0                 0x33
#define V_PCH_PCIE_CFG_LTSSMSMSTS_LTSSMSTATESUB_DISWAIT            0x5E
#define V_PCH_PCIE_CFG_LTSSMSMSTS_LTSSMSTATESUB_DISWAITPG          0x60
#define V_PCH_PCIE_CFG_LTSSMSMSTS_LTSSMSTATESUB_RECOVERYSPEEDREADY 0x6C
#define V_PCH_PCIE_CFG_LTSSMSMSTS_LTSSMSTATESUB_RECOVERYLNK2DETECT 0x6F
#define V_PCH_PCIE_CFG_PTMGTECSR_IOSFMADP                   0x2

#define R_PCH_PCIE_STRAPCSR1                          0x7F8
#define B_PCH_PCIE_STRAPCSR1_PORTEN                   (BIT3 | BIT2 | BIT1 | BIT0)  //> Strap CSR 1 register- Strap Info- Port Enable
#define R_PCH_PCIE_FUSECSR1                           0x7F0
#define R_PCH_PCIE_FUSECSR1_PORTEN                    (BIT3 | BIT2 | BIT1 | BIT0)  //> Fuse CSR 1 register- Fuse Info- Port Enable

#endif
