/** @file
  Register names for Cedarfork SouthCluster PCI-E root port devices

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
  Copyright 2017-2020 Intel Corporation.

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
#ifndef _PCH_REGS_PCIE_CDF_H_
#define _PCH_REGS_PCIE_CDF_H_

#define V_PCH_H_PCIE_CFG_DID_PORT1              0x18A4  ///< PCI Express Root Port #1,  CDF
#define V_PCH_H_PCIE_CFG_DID_PORT2              0x18A5  ///< PCI Express Root Port #2,  CDF
#define V_PCH_H_PCIE_CFG_DID_PORT3              0x18A6  ///< PCI Express Root Port #3,  CDF
#define V_PCH_H_PCIE_CFG_DID_PORT4              0x18A7  ///< PCI Express Root Port #4,  CDF
#define V_PCH_H_PCIE_CFG_DID_PORT5              0x18A8  ///< PCI Express Root Port #5,  CDF
#define V_PCH_H_PCIE_CFG_DID_PORT6              0x18A9  ///< PCI Express Root Port #6,  CDF
#define V_PCH_H_PCIE_CFG_DID_PORT7              0x18AA  ///< PCI Express Root Port #7,  CDF
#define V_PCH_H_PCIE_CFG_DID_PORT8              0x18AB  ///< PCI Express Root Port #8,  CDF
#define V_PCH_H_PCIE_CFG_DID_PORT9              0x18AD  ///< PCI Express Root Port #9,  CDF
#define V_PCH_H_PCIE_CFG_DID_PORT10             0x18AE  ///< PCI Express Root Port #10, CDF
#define V_PCH_H_PCIE_CFG_DID_PORT11             0x18AF  ///< PCI Express Root Port #11, CDF
#define V_PCH_H_PCIE_CFG_DID_PORT12             0x18A2  ///< PCI Express Root Port #12, CDF

#define R_PCH_PCIE_CFG_EXPCAPLST                        0x40
#define R_PCH_PCIE_CFG_EXPCAP                           0x42
#define R_PCH_PCIE_CFG_DEVCAP                           0x44
#define R_PCH_PCIE_CFG_DEVCTL                           0x48
#define B_PCH_PCIE_CFG_DEVCTL_MPS                       (BIT7 | BIT6 | BIT5) ///< Max Payload Size
#define N_PCH_PCIE_CFG_DEVCTL_MPS                       5
#define B_PCH_PCIE_CFG_DEVCTL_URRE                      BIT3 ///< Unsupported Request Reporting Enable
#define B_PCH_PCIE_CFG_DEVCTL_FERE                      BIT2 ///< Fatal Error Reporting Enable
#define B_PCH_PCIE_CFG_DEVCTL_NFERE                     BIT1 ///< Non-Fatal Error Reporting Enable
#define B_PCH_PCIE_CFG_DEVCTL_CERE                      BIT0 ///< Correctable Error Reporting Enable
#define R_PCH_PCIE_CFG_LINKCAP                          0x4C
#define B_PCH_PCIE_CFG_LINKCAP_PN                       0xFF000000
#define N_PCH_PCIE_CFG_LINKCAP_PN                       24
#define N_PCH_PCIE_CFG_LINKCAP_MLS                      0
#define B_PCH_PCIE_CFG_LINKCAP_MLS                      (BIT3 | BIT2 | BIT1 | BIT0)
#define V_PCH_PCIE_CFG_LINKCAP_MLS_GEN1                 0x1
#define V_PCH_PCIE_CFG_LINKCAP_MLS_GEN2                 0x2
#define V_PCH_PCIE_CFG_LINKCAP_MLS_GEN3                 0x3
#define R_PCH_PCIE_CFG_LINKCTL                          0x50
#define R_PCH_PCIE_CFG_LINKSTS                          0x52
#define R_PCH_PCIE_CFG_SLOTCAP                          0x54
#define R_PCH_PCIE_CFG_SLOTCTL                          0x58
#define R_PCH_PCIE_CFG_SLOTSTS                          0x5A
#define R_PCH_PCIE_CFG_ROOTCTL                          0x5C
#define B_PCH_PCIE_CFG_ROOTCTL_SEFEE                    BIT2 ///< System Error on Fatal Error Enable
#define B_PCH_PCIE_CFG_ROOTCTL_SENFEE                   BIT1 ///< System Error on Non-Fatal Error Enable
#define B_PCH_PCIE_CFG_ROOTCTL_SECEE                    BIT0 ///< System Error on Correctable Error Enable
#define R_PCH_PCIE_CFG_ROOTSTS                          0x60
#define R_PCH_PCIE_CFG_DEVCAP2                          0x64
#define R_PCH_PCIE_CFG_DEVCTL2                          0x68
#define V_PCH_PCIE_CFG_DEVCTL2_CTV_DEFAULT              0x0
#define V_PCH_PCIE_CFG_DEVCTL2_CTV_50US_100US           0x1
#define V_PCH_PCIE_CFG_DEVCTL2_CTV_1MS_10MS             0x2
#define V_PCH_PCIE_CFG_DEVCTL2_CTV_16MS_55MS            0x5
#define V_PCH_PCIE_CFG_DEVCTL2_CTV_65MS_210MS           0x6
#define V_PCH_PCIE_CFG_DEVCTL2_CTV_260MS_900MS          0x9
#define V_PCH_PCIE_CFG_DEVCTL2_CTV_1S_3P5S              0xA
#define V_PCH_PCIE_CFG_DEVCTL2_CTV_4S_13S               0xD
#define V_PCH_PCIE_CFG_DEVCTL2_CTV_17S_64S              0xE
#define R_PCH_PCIE_CFG_LINKCTL2                         0x70
#define B_PCH_PCIE_CFG_LINKCTL2_TLS                     0xF // Target Link Speed
#define V_PCH_PCIE_CFG_LINKCTL2_TLS_GEN1                0x1
#define V_PCH_PCIE_CFG_LINKCTL2_TLS_GEN2                0x2
#define V_PCH_PCIE_CFG_LINKCTL2_TLS_GEN3                0x3
#define R_PCH_PCIE_CFG_LINKSTS2                         0x72

#define R_PCH_PCIE_CFG_MSICAPLST                        0x90
#define S_PCH_PCIE_CFG_MSICAPLST                        2

#define R_PCH_PCIE_CFG_SSCAPLST                         0x88
#define S_PCH_PCIE_CFG_SSCAPLST                         2
#define R_PCH_PCIE_CFG_SSVID                            0x8c

#define R_PCH_PCIE_CFG_PMCSR                            0x84

#define R_PCH_PCIE_CFG_RPPMCSR                          0xA8
#define B_PCH_PCIE_CFG_RPPMCSR_PME_TO_ACK_TOP           BIT4

#define R_PCH_PCIE_MSG_PPBIFCTL_PRIV                      0xC0
#define B_PCH_PCIE_MSG_PPBIFCTL_PRIV_CL                   (BIT31)
#define N_PCH_PCIE_MSG_PPBIFCTL_PRIV_CL                   31
#define B_PCH_PCIE_MSG_PPBIFCTL_PRIV_BIFCTL0              (BIT2 | BIT1 | BIT0)
#define N_PCH_PCIE_MSG_PPBIFCTL_PRIV_BIFCTL0              0
#define V_PCH_PCIE_CFG_PPBIFCTL_PRIV_BIFCTL0_2_2_2_2      0
#define V_PCH_PCIE_CFG_PPBIFCTL_PRIV_BIFCTL0_4_2_2        1
#define V_PCH_PCIE_CFG_PPBIFCTL_PRIV_BIFCTL0_2_2_4        2
#define V_PCH_PCIE_CFG_PPBIFCTL_PRIV_BIFCTL0_4_4          3
#define V_PCH_PCIE_CFG_PPBIFCTL_PRIV_BIFCTL0_8            4
#define V_PCH_PCIE_CFG_PPBIFCTL_PRIV_BIFCTL0_UNSUPPORTED  0xFFFFFFFF

#define R_PCH_PCIE_CFG_SMICSR                         0xC8
#define S_PCH_PCIE_CFG_SMICSR                         4
#define N_PCH_PCIE_CFG_SMICSR_HPLSCSMISTS             18      // Hot Plug DLLSC SMI Status
#define B_PCH_PCIE_CFG_SMICSR_HPSMISTS                BIT17
#define N_PCH_PCIE_CFG_SMICSR_HPSMISTS                17
#define B_PCH_PCIE_CFG_SMICSR_PMSMISTS                BIT16
#define B_PCH_PCIE_CFG_SMICSR_HPSMIE                  BIT1
#define N_PCH_PCIE_CFG_SMICSR_HPSMIE                  1
#define B_PCH_PCIE_CFG_SMICSR_PMSMIE                  BIT0

#define R_PCH_PCIE_CFG_SCICSR                         0xCC
#define S_PCH_PCIE_CFG_SCICSR                         4
#define B_PCH_PCIE_CFG_SCICSR_PMSCISTS                BIT16
#define B_PCH_PCIE_CFG_SCICSR_HPSCIE                  BIT1
#define B_PCH_PCIE_CFG_SCICSR_PMSCIE                  BIT0

#define R_PCH_PCIE_CFG_PPD0                           0xD4  // PCIe Port Definition Control Register 0
#define B_PCH_PCIE_CFG_PPD0_ILINKTRN0                 BIT3  // Initiate Link Training 0

#define R_PCH_PCIE_CFG_PBTXNCTL                       0xD8  // Private Block Transaction Layer Control
#define B_PCH_PCIE_CFG_PBTXNCTL_DEOIM                 BIT30 // Disable APIC EOI Message
#define B_PCH_PCIE_CFG_PBTXNCTL_P8XDE                 BIT0  // Port 8xh Decode Enable

#define R_PCH_PCIE_CFG_VPPCSR                         0xDC
#define B_PCH_PCIE_CFG_VPPCSR_VPPE                    BIT25
#define B_PCH_PCIE_CFG_VPPCSR_VPPN                    (BIT29 | BIT28 | BIT27 | BIT26)
#define N_PCH_PCIE_CFG_VPPCSR_VPPN                    26

#define R_PCH_PCIE_CFG_LTRCSR                        0xEC
#define B_PCH_PCIE_CFG_LTRCSR_LTRAMD                 BIT3 ///< LTR Force Override Enable
#define B_PCH_PCIE_CFG_LTRCSR_LTROVRPLCY             BIT2 ///< LTR Override Lock
#define B_PCH_PCIE_CFG_LTRCSR_LTRNSOVREN             BIT1 ///< LTR Non-Snoop Override Enable
#define B_PCH_PCIE_CFG_LTRCSR_LTRSOVREN              BIT0 ///< LTR Snoop Override Enable

#define R_PCH_PCIE_CFG_LTROVR                         0xF4
#define B_PCH_PCIE_CFG_LTROVR_LTRNSROVR               BIT31 ///< LTR Non-Snoop Requirement Bit Override
#define B_PCH_PCIE_CFG_LTROVR_LTRSROVR                BIT15 ///< LTR Snoop Requirement Bit Override

//
//PCI Express Extended Capability Registers
//

#define R_PCH_PCIE_CFG_EXCAP_OFFSET                              0x100

#define R_PCH_PCIE_CFG_ROOTERRCMD                                0x12C
#define B_PCH_PCIE_CFG_ROOTERRCMD_FERE                           BIT2
#define B_PCH_PCIE_CFG_ROOTERRCMD_NFERE                          BIT1
#define B_PCH_PCIE_CFG_ROOTERRCMD_CERE                           BIT0

#define R_PCH_PCIE_CFG_ROOTERRSTS                                0x130
#define S_PCH_PCIE_CFG_ROOTERRSTS                                4
#define B_PCH_PCIE_CFG_ROOTERRSTS_AEMN                           0xF8000000
#define B_PCH_PCIE_CFG_ROOTERRSTS_FEMR                           BIT6
#define B_PCH_PCIE_CFG_ROOTERRSTS_NFEMR                          BIT5
#define B_PCH_PCIE_CFG_ROOTERRSTS_FUF                            BIT4
#define B_PCH_PCIE_CFG_ROOTERRSTS_MEFR                           BIT3
#define B_PCH_PCIE_CFG_ROOTERRSTS_EFR                            BIT2
#define B_PCH_PCIE_CFG_ROOTERRSTS_MCER                           BIT1
#define B_PCH_PCIE_CFG_ROOTERRSTS_CER                            BIT0

#define R_PCH_PCIE_CFG_AERCAPHDR                         0x100 ///< Advanced Error Reporting Capability Header
#define V_PCH_PCIE_CFG_AERCAPHDR_CV                      0x1
#define R_PCH_PCIE_CFG_ERRUNCMSK                        (R_PCH_PCIE_CFG_AERCAPHDR + R_PCIE_EX_UEM_OFFSET) // Uncorrectable Error Mask

#define R_PCH_PCIE_CFG_ACSCAPHDR                         0x148 ///< ACS Extended Capability Header
#define V_PCH_PCIE_CFG_ACSCAPHDR_CV                      0x1
#define R_PCH_PCIE_CFG_ACSCAP                           (R_PCH_PCIE_CFG_ACSCAPHDR + R_PCIE_EX_ACSCAPR_OFFSET)
#define R_PCH_PCIE_CFG_ACSCTL                            0x14E
#define B_PCH_PCIE_CFG_ACSCTL_VACSSVE                    BIT0

#define R_PCH_PCIE_CFG_PTMECH                            0x1E0 ///< PTM Extended Capability Header
#define V_PCH_PCIE_CFG_PTMECH_CV                         0x1
#define R_PCH_PCIE_CFG_PTMCAP                           (R_PCH_PCIE_CFG_PTMECH + R_PCIE_EX_PTMCAP_OFFSET)
#define R_PCH_PCIE_CFG_PTMCTL                            0x1E8
#define B_PCH_PCIE_CFG_PTMCTL_PTME                       BIT0

#define R_PCH_PCIE_CFG_L1PMSECH                          0x1D0 ///< L1 Sub-States Extended Capability Header
#define V_PCH_PCIE_CFG_L1PMSECH_CV                       0x1
#define R_PCH_PCIE_CFG_L1PMSCAP                         (R_PCH_PCIE_CFG_L1PMSECH + R_PCIE_EX_L1SCAP_OFFSET)

#define R_PCH_PCIE_CFG_SECEXPCAP                         0x200 ///< Secondary PCI Express Extended Capability Header
#define V_PCH_PCIE_CFG_SECEXPCAP_CV                      0x1
#define R_PCH_PCIE_CFG_LINKCTL3                         (R_PCH_PCIE_CFG_SECEXPCAP + R_PCIE_EX_LCTL3_OFFSET)
#define R_PCH_PCIE_CFG_LANEEQCTL_0                       0x20C
#define S_PCH_PCIE_CFG_LANEEQCTL_0                       2
#define B_PCH_PCIE_CFG_LANEEQCTL_0_UPTP                 (BIT11 | BIT10 | BIT9 | BIT8)
#define B_PCH_PCIE_CFG_LANEEQCTL_0_DPTP                 (BIT3 | BIT2 | BIT1 | BIT0)
#define N_PCH_PCIE_CFG_LANEEQCTL_0_UPTP                  8
#define N_PCH_PCIE_CFG_LANEEQCTL_0_DPTP                  0
#define R_PCH_PCIE_CFG_DPCECH                            0x190 ///< Downstream Port Containment
#define V_PCH_PCIE_CFG_DPCECH_CV                         0x1

#define R_PCH_PCIE_CFG_DPCCTL                            0x196 ///< DPC Control
#define B_PCH_PCIE_CFG_DPCCTL_DPCTE                      (BIT1 | BIT0) ///< DPC Control
#define V_PCH_PCIE_CFG_DPCCTL_DPCTE_DISABLE               0 ///DPC is disabled
#define V_PCH_PCIE_CFG_DPCCTL_DPCTE_FATAL_ENABLE          1 ///DPC is enabled triggered when the Downstream Port detects an unmasked uncorrectable error or when the Downstream Port receives an ERR_FATAL Message.
#define V_PCH_PCIE_CFG_DPCCTL_DPCTE_FATAL_NONFATAL_ENABLE 2 ///DPC is enabled triggered when the Downstream Port detects an unmasked uncorrectable error or when the Downstream Port receives an ERR_NONFATAL or ERR_FATAL Message.

#define R_PCH_PCIE_CFG_VC0RCTL                        0x294  // VC0 Resource Control
#define N_PCH_PCIE_CFG_VC0RCTL_TCVCM                  1
#define V_PCH_PCIE_CFG_VC0RCTL_TCVCM                  0x3F
#define V_PCH_PCIE_CFG_VC0RCTL_MVC                    0x1

#define R_PCH_PCIE_CFG_VC1RCTL                        0x2A0  // VC1 Resource Control
#define N_PCH_PCIE_CFG_VC1RCTL_TCVCM                  1
#define V_PCH_PCIE_CFG_VC1RCTL_MVC                    0x8100000E

#define R_PCH_PCIE_CFG_LTSSMTRNCTL2                                0x00000A38
#define B_PCH_PCIE_CFG_LTSSMTRNCTL2_TXRXDETPOL                     BIT16

#define R_PCH_PCIE_CFG_PIPECTL                                     0xA58
#define B_PCH_PCIE_CFG_PIPECTL_PWRDNPERPORT                        BIT0

#define R_PCH_PCIE_CFG_PIPECTL2                                    0xA5C
#define B_PCH_PCIE_CFG_PIPECTL2_L1P0_PSMAP                         (BIT6 | BIT5 | BIT4)
#define N_PCH_PCIE_CFG_PIPECTL2_L1P0_PSMAP                         4
#define V_PCH_PCIE_CFG_PIPECTL2_L1P0_PSMAP                         0x2

#define R_PCH_PCIE_CFG_LTSSMSMSTS                                  0xA68              //register name in CDF is LTSSMSMSTS, fields LTSSMSTATEMAIN and LTSSMSTATESUB
#define B_PCH_PCIE_CFG_LTSSMSMSTS_LTSSMSTATESUB                    0xFF000000
#define N_PCH_PCIE_CFG_LTSSMSMSTS_LTSSMSTATESUB                    24
#define V_PCH_PCIE_CFG_LTSSMSMSTS_LTSSMSTATESUB_DETRDY             0x01
#define V_PCH_PCIE_CFG_LTSSMSMSTS_LTSSMSTATESUB_DETRDYECINP1CG     0x0E
#define V_PCH_PCIE_CFG_LTSSMSMSTS_LTSSMSTATESUB_L0                 0x33
#define V_PCH_PCIE_CFG_LTSSMSMSTS_LTSSMSTATESUB_DISWAIT            0x5E
#define V_PCH_PCIE_CFG_LTSSMSMSTS_LTSSMSTATESUB_DISWAITPG          0x60
#define V_PCH_PCIE_CFG_LTSSMSMSTS_LTSSMSTATESUB_RECOVERYSPEEDREADY 0x6C
#define V_PCH_PCIE_CFG_LTSSMSMSTS_LTSSMSTATESUB_RECOVERYLNK2DETECT 0x6F
#define B_PCH_PCIE_CFG_LTSSMSMSTS_LNKREVERSED                      BIT2  // Link Reversed

#define R_PCH_PCIE_CFG_PTMGTECSR                        0x794 ///< PTM Extended Configuration
#define N_PCH_PCIE_CFG_PTMGTECSR_GTFFC                  14
#define V_PCH_PCIE_CFG_PTMGTECSR_GTFFC                  1
#define B_PCH_PCIE_CFG_PTMGTECSR_PGTSCFF                (BIT12 | BIT11 | BIT10)
#define N_PCH_PCIE_CFG_PTMGTECSR_PGTSCFF                10
#define B_PCH_PCIE_CFG_PTMGTECSR_PGTSCFE                BIT9
#define B_PCH_PCIE_CFG_PTMGTECSR_TGTSCF                 BIT8
#define B_PCH_PCIE_CFG_PTMGTECSR_IOSFMADP               (BIT3 | BIT2 | BIT1 | BIT0)
#define V_PCH_PCIE_CFG_PTMGTECSR_IOSFMADP                1

#define R_PCH_PCIE_CFG_PTMG1TXPSD                       0x798 ///< PTM Pipe Stage Delay Configuration 1
#define R_PCH_PCIE_CFG_PTMG1RXPSD                       0x79c ///< PTM Pipe Stage Delay Configuration 2
#define R_PCH_PCIE_CFG_PTMG2TXPSD                       0x7a0 ///< PTM Pipe Stage Delay Configuration 3
#define R_PCH_PCIE_CFG_PTMG2RXPSD                       0x7a4 ///< PTM Pipe Stage Delay Configuration 4
#define R_PCH_PCIE_CFG_PTMG3TXPSD                       0x7a8 ///< PTM Pipe Stage Delay Configuration 5
#define R_PCH_PCIE_CFG_PTMG3RXPSD                       0x7ac ///< PTM Pipe Stage Delay Configuration 6

#define R_PCH_PCIE_CFG_IOSFDEVCLKGCTL                 0x2E4  // IOSF Device Clock Gate CTRL Register
#define B_PCH_PCIE_CFG_IOSFDEVCLKGCTL_ICGE            BIT15  // Idle Clock Gating Enable
#define R_PCH_PCIE_CFG_SBDEVCLKGCTL                   0x2E6  // Sideband Device Clock Gate CTRL Register
#define B_PCH_PCIE_CFG_SBDEVCLKGCTL_ICGE              BIT15
#define R_PCH_PCIE_CFG_PT0TXNCLKGCTL                  0x8E4  // Port 0 Transaction Clock Gate CTRL Register
#define B_PCH_PCIE_CFG_PT0TXNCLKGCTL_ICGE             BIT15
#define R_PCH_PCIE_CFG_PT1TXNCLKGCTL                  0x8E6  // Port 1 Transaction Clock Gate CTRL Register
#define B_PCH_PCIE_CFG_PT1TXNCLKGCTL_ICGE             BIT15
#define R_PCH_PCIE_CFG_PT2TXNCLKGCTL                  0x8E8  // Port 2 Transaction Clock Gate CTRL Register
#define B_PCH_PCIE_CFG_PT2TXNCLKGCTL_ICGE             BIT15
#define R_PCH_PCIE_CFG_PT3TXNCLKGCTL                  0x8EA  // Port 3 Transaction Clock Gate CTRL Register
#define B_PCH_PCIE_CFG_PT3TXNCLKGCTL_ICGE             BIT15
#define R_PCH_PCIE_CFG_XPTDEF2                        0x8F8  // PCI-Express Transaction Layer Defeature Register 2 (lock is B_PCH_PCIE_CFG_PLKCTL_TLCL)
#define B_PCH_PCIE_CFG_XPTDEF2_TCPAR_CHK_DIS          BIT11  // IOSF tcparity check disable
#define B_PCH_PCIE_CFG_XPTDEF2_TDPAR_CHK_DIS          BIT12  // IOSF tdparity check disable
#define R_PCH_PCIE_CFG_PT0LNKCLKGCTL                  0x9E4  // Port 0 Link Clock Gate CTRL Register
#define B_PCH_PCIE_CFG_PT0LNKCLKGCTL_ICGE             BIT15
#define R_PCH_PCIE_CFG_PT1LNKCLKGCTL                  0x9E6  // Port 1 Link Clock Gate CTRL Register
#define B_PCH_PCIE_CFG_PT1LNKCLKGCTL_ICGE             BIT15
#define R_PCH_PCIE_CFG_PT2LNKCLKGCTL                  0x9E8  // Port 2 Link Clock Gate CTRL Register
#define B_PCH_PCIE_CFG_PT2LNKCLKGCTL_ICGE             BIT15
#define R_PCH_PCIE_CFG_PT3LNKCLKGCTL                  0x9EA  // Port 3 Link Clock Gate CTRL Register
#define B_PCH_PCIE_CFG_PT3LNKCLKGCTL_ICGE             BIT15
#define R_PCH_PCIE_CFG_CFGPCIECLKGCTL                 0x9EC  // Configuration PCIe Clock Gate CTRL Register
#define B_PCH_PCIE_CFG_CFGPCIECLKGCTL_ICGE            BIT15
#define R_PCH_PCIE_CFG_PT0PHYCLKGCTL                  0xAE4  // Port 0 Physical Clock Gate CTRL Register
#define B_PCH_PCIE_CFG_PT0PHYCLKGCTL_ICGE             BIT15
#define R_PCH_PCIE_CFG_PT1PHYCLKGCTL                  0xAE6  // Port 1 Physical Clock Gate CTRL Register
#define B_PCH_PCIE_CFG_PT1PHYCLKGCTL_ICGE             BIT15
#define R_PCH_PCIE_CFG_PT2PHYCLKGCTL                  0xAE8  // Port 2 Physical Clock Gate CTRL Register
#define B_PCH_PCIE_CFG_PT2PHYCLKGCTL_ICGE             BIT15
#define R_PCH_PCIE_CFG_PT3PHYCLKGCTL                  0xAEA  // Port 3 Physical Clock Gate CTRL Register
#define B_PCH_PCIE_CFG_PT3PHYCLKGCTL_ICGE             BIT15
#define R_PCH_PCIE_CFG_PLLSTBYCTL                     0xBAC  // PLL Standby Control
#define B_PCH_PCIE_CFG_PLLSTBYCTL_PCLK_TRUNK_CGE      BIT4   // PCLK Trunk Clock Gate Enable
#define B_PCH_PCIE_CFG_PLLSTBYCTL_PSE                 (BIT3 | BIT2 | BIT1 | BIT0)

#define R_PCH_PCIE_CFG_SPAD                            0x8C4  // Scratchpad register

#define R_PCH_PCIE_CFG_SPEEDCTL                       0xB68
#define B_PCH_PCIE_CFG_SPEEDCTL_LBEG3DEGRADEEN        BIT25

#define R_PCH_PCIE_CFG_EBERRMSK                       0xB78  // Elastic Buffer Error Mask
#define B_PCH_PCIE_CFG_EBERRMSK_MASK                  0xFF

//
// PCIE PCRs (PID: PID_PCIE0_P0 PID_PCIE1_P0 PID_PCIE2_P0)
//
#define R_PCH_PCIE_MSG_DEVHIDE                        0x7D8
#define B_PCH_PCIE_MSG_DEVHIDE_P0GBE                  BIT5  //> Port 0 Global Block Enable
#define B_PCH_PCIE_MSG_DEVHIDE_PCFGHP                 BIT4  //> Port CFG Hide Policy
#define B_PCH_PCIE_MSG_DEVHIDE_P0CFGH                 BIT0  //> Port 0 CFG Hide

#define R_PCH_PCIE_MSG_EQEVALCTL                      0x31D0
#define V_PCH_PCIE_MSG_EQEVALCTL_HWEQ                 0x00000000
#define R_PCH_PCIE_MSG_EQEVALCTL2                     0x31D4
#define V_PCH_PCIE_MSG_EQEVALCTL2_HWEQ                0x00000ba0
#define R_PCH_PCIE_MSG_EQEVALCTL3                     0x31D8
#define V_PCH_PCIE_MSG_EQEVALCTL3_HWEQ                0x00000802
#define R_PCH_PCIE_MSG_EQEVALCTL4                     0x31B8
#define V_PCH_PCIE_MSG_EQEVALCTL4_HWEQ                0x4820004
#define R_PCH_PCIE_MSG_EQEVALCTL5                     0x31BC
#define V_PCH_PCIE_MSG_EQEVALCTL5_HWEQ                0x3a801013
#define R_PCH_PCIE_MSG_EQEVALCTL6                     0x318C
#define V_PCH_PCIE_MSG_EQEVALCTL6_HWEQ                0x04904623
#define R_PCH_PCIE_MSG_EQEVALCTL7                     0x3190
#define V_PCH_PCIE_MSG_EQEVALCTL7_HWEQ                0x00000020

#define R_PCH_PCIE_MSG_GEN3FARTXCOEFF0                0x35C0
#define V_PCH_PCIE_MSG_GEN3FARTXCOEFF0_HWEQ           0x00000000
#define R_PCH_PCIE_MSG_GEN3FARTXCOEFF1                0x35C4
#define V_PCH_PCIE_MSG_GEN3FARTXCOEFF1_HWEQ           0x00000000
#define R_PCH_PCIE_MSG_GEN3FARTXCOEFF2                0x35C8
#define V_PCH_PCIE_MSG_GEN3FARTXCOEFF2_HWEQ           0x00000000
#define R_PCH_PCIE_MSG_GEN3FARTXCOEFF3                0x35CC
#define V_PCH_PCIE_MSG_GEN3FARTXCOEFF3_HWEQ           0x00000000
#define R_PCH_PCIE_MSG_GEN3FARTXCOEFF4                0x35D0
#define V_PCH_PCIE_MSG_GEN3FARTXCOEFF4_HWEQ           0x00000000
#define R_PCH_PCIE_MSG_GEN3FARTXCOEFF5                0x35D4
#define V_PCH_PCIE_MSG_GEN3FARTXCOEFF5_HWEQ           0x00000000
#define R_PCH_PCIE_MSG_GEN3FARTXCOEFF6                0x35D8
#define V_PCH_PCIE_MSG_GEN3FARTXCOEFF6_HWEQ           0x00000000
#define R_PCH_PCIE_MSG_GEN3FARTXCOEFF7                0x35DC
#define V_PCH_PCIE_MSG_GEN3FARTXCOEFF7_HWEQ           0x00000000

#define R_PCH_PCIE_MSG_FOMHINTTBL0                    0x3A00
#define V_PCH_PCIE_MSG_FOMHINTTBL0_HWEQ               0x0000b506
#define R_PCH_PCIE_MSG_FOMHINTTBL1                    0x3A04
#define V_PCH_PCIE_MSG_FOMHINTTBL1_HWEQ               0x0000b448
#define R_PCH_PCIE_MSG_FOMHINTTBL2                    0x3A08
#define V_PCH_PCIE_MSG_FOMHINTTBL2_HWEQ               0x0000b4ca
#define R_PCH_PCIE_MSG_FOMHINTTBL3                    0x3A0C
#define V_PCH_PCIE_MSG_FOMHINTTBL3_HWEQ               0x0000b50c
#define R_PCH_PCIE_MSG_FOMHINTTBL4                    0x3A10
#define V_PCH_PCIE_MSG_FOMHINTTBL4_HWEQ               0x0000b44e
#define R_PCH_PCIE_MSG_FOMHINTTBL5                    0x3A14
#define V_PCH_PCIE_MSG_FOMHINTTBL5_HWEQ               0x000034c6
#define R_PCH_PCIE_MSG_FOMHINTTBL6                    0x3A18
#define V_PCH_PCIE_MSG_FOMHINTTBL6_HWEQ               0x000034c6
#define R_PCH_PCIE_MSG_FOMHINTTBL7                    0x3A1C
#define V_PCH_PCIE_MSG_FOMHINTTBL7_HWEQ               0x000034c6
#define R_PCH_PCIE_MSG_FOMHINTTBL8                    0x3A20
#define V_PCH_PCIE_MSG_FOMHINTTBL8_HWEQ               0x000034c6
#define R_PCH_PCIE_MSG_FOMHINTTBL9                    0x3A24
#define V_PCH_PCIE_MSG_FOMHINTTBL9_HWEQ               0x000034c6
#define R_PCH_PCIE_MSG_FOMHINTTBL10                   0x3A28
#define V_PCH_PCIE_MSG_FOMHINTTBL10_HWEQ              0x000034c6
#define R_PCH_PCIE_MSG_FOMHINTTBL11                   0x3A2C
#define V_PCH_PCIE_MSG_FOMHINTTBL11_HWEQ              0x000034c6

#define R_PCH_PCIE_MSG_FOMINCCTL                      0x3A80
#define V_PCH_PCIE_MSG_FOMINCCTL_HWEQ                 0x0004ffff

#define R_PCH_PCIE_CFG_PLKCTL                         0xEA  // Personality Lock Key Control
#define B_PCH_PCIE_CFG_PLKCTL_PLCL                    0x10  //> Phy Layer Capability Lock
#define B_PCH_PCIE_CFG_PLKCTL_LLCL                    0x4   //> Lnk Layer Capability Lock
#define B_PCH_PCIE_CFG_PLKCTL_TLCL                    0x2   //> Txn Layer Capability Lock
#define B_PCH_PCIE_CFG_PLKCTL_CL                      0x1   //> Capability Lock

#endif
