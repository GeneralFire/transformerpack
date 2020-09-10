/** @file
  Register names for PCIe SIP specific registers

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
  Copyright 2020 Intel Corporation.

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

#ifndef _PCIE_SIP_RP_REGS_H_
#define _PCIE_SIP_RP_REGS_H_

#include <PcieRegs.h>


#define R_PCIE_CFG_CLIST                              0x40
#define R_PCIE_CFG_XCAP                               (R_PCIE_CFG_CLIST + R_PCIE_XCAP_OFFSET)
#define R_PCIE_CFG_DCAP                               (R_PCIE_CFG_CLIST + R_PCIE_DCAP_OFFSET)
#define R_PCIE_CFG_DCTL                               (R_PCIE_CFG_CLIST + R_PCIE_DCTL_OFFSET)
#define R_PCIE_CFG_DSTS                               (R_PCIE_CFG_CLIST + R_PCIE_DSTS_OFFSET)
#define R_PCIE_CFG_LCAP                               (R_PCIE_CFG_CLIST + R_PCIE_LCAP_OFFSET)
#define B_PCIE_CFG_LCAP_PN                            0xFF000000
#define N_PCIE_CFG_LCAP_PN                            24
#define R_PCIE_CFG_LCTL                               (R_PCIE_CFG_CLIST + R_PCIE_LCTL_OFFSET)
#define R_PCIE_CFG_LSTS                               (R_PCIE_CFG_CLIST + R_PCIE_LSTS_OFFSET)
#define R_PCIE_CFG_SLCAP                              (R_PCIE_CFG_CLIST + R_PCIE_SLCAP_OFFSET)
#define R_PCIE_CFG_SLCTL                              (R_PCIE_CFG_CLIST + R_PCIE_SLCTL_OFFSET)
#define R_PCIE_CFG_SLSTS                              (R_PCIE_CFG_CLIST + R_PCIE_SLSTS_OFFSET)
#define R_PCIE_CFG_RCTL                               (R_PCIE_CFG_CLIST + R_PCIE_RCTL_OFFSET)
#define R_PCIE_CFG_RSTS                               (R_PCIE_CFG_CLIST + R_PCIE_RSTS_OFFSET)
#define R_PCIE_CFG_DCAP2                              (R_PCIE_CFG_CLIST + R_PCIE_DCAP2_OFFSET)
#define R_PCIE_CFG_DCTL2                              (R_PCIE_CFG_CLIST + R_PCIE_DCTL2_OFFSET)
#define R_PCIE_CFG_LCTL2                              (R_PCIE_CFG_CLIST + R_PCIE_LCTL2_OFFSET)
#define R_PCIE_CFG_LSTS2                              (R_PCIE_CFG_CLIST + R_PCIE_LSTS2_OFFSET)
#define R_PCIE_CFG_LCAP2                              (R_PCIE_CFG_CLIST + R_PCIE_LCAP2_OFFSET)

#define R_PCIE_CFG_CCFG                               0xD0
#define B_PCIE_CFG_CCFG_UMRPD                         BIT26
#define B_PCIE_CFG_CCFG_UPMWPD                        BIT25
#define B_PCIE_CFG_CCFG_UPSD                          BIT24
#define B_PCIE_CFG_CCFG_UNSD                          BIT23
#define B_PCIE_CFG_CCFG_DCGEISMA                      BIT15
#define B_PCIE_CFG_CCFG_UNRD                          (BIT13 | BIT12)
#define N_PCIE_CFG_CCFG_UNRD                          12
#define B_PCIE_CFG_CCFG_UNRS                          (BIT6 | BIT5 | BIT4)
#define N_PCIE_CFG_CCFG_UNRS                          4
#define V_PCIE_CFG_CCFG_UNRS_128B                     0
#define V_PCIE_CFG_CCFG_UNRS_256B                     1
#define V_PCIE_CFG_CCFG_UNRS_64B                      7
#define B_PCIE_CFG_CCFG_UPRS                          (BIT2 | BIT1 | BIT0)
#define N_PCIE_CFG_CCFG_UPRS                          0
#define V_PCIE_CFG_CCFG_UPRS_128B                     0
#define V_PCIE_CFG_CCFG_UPRS_256B                     1
#define V_PCIE_CFG_CCFG_UPRS_64B                      7

#define R_PCIE_CFG_MPC                                0xD8
#define B_PCIE_CFG_MPC_FCDL1E                         BIT21

#define R_PCIE_PWRCTL                                 0xE8           ///< Power Control Register
#define B_PCIE_PWRCTL_RSP                             BIT25          ///< RxElecIdle Sampling Policy
#define B_PCIE_PWRCTL_LIFECF                          BIT23
#define B_PCIE_CFG_PWRCTL_WPDMPGEP                    BIT17
#define B_PCIE_CFG_PWRCTL_WPDMPGEP_OFFSET             17
#define B_PCIE_PWRCTL_RPL1SQPOL                       BIT1           ///< Root Port L1 Squelch Polling
#define B_PCIE_PWRCTL_RPDTSQPOL                       BIT0           ///< Root Port Detect Squelch Polling

#define R_PCIE_PHYCTL3                                0xF6           ///< Physical Layer And AFE Control 3 Register
#define B_PCIE_PHYCTL3_SQDIROVREN                     BIT2           ///< Squelch Direction Override Enable
#define B_PCIE_PHYCTL3_SQDIRCTRL                      BIT1           ///< Squelch Direction

#define R_PCIE_CFG_PVCCR1                             0x284
#define B_PCIE_CFG_PVCCR1_EVCC_MASK                   0x7
#define B_PCIE_CFG_PVCCR1_EVCC_OFFSET                 0
#define V_PCIE_CFG_PVCCR1_EVCC_2_VC                   0x1
#define V_PCIE_CFG_PVCCR1_EVCC_3_VC                   0x2

#define R_PCIE_CFG_VC0CTL                             0x294
#define B_PCIE_CFG_VC0CTL_TVM_MASK                    0xFE
#define B_PCIE_CFG_VC0CTL_TVM_OFFSET                  1
#define V_PCIE_CFG_VC0CTL_TVM_NO_VC                   0x7F
#define V_PCIE_CFG_VC0CTL_TVM_EN_VC                   0x1f

#define R_PCIE_CFG_VC1CTL                             0x2A0
#define B_PCIE_CFG_VC1CTL_TVM_MASK                    0xFE
#define B_PCIE_CFG_VC1CTL_TVM                         BIT1
#define B_PCIE_CFG_VC1CTL_TVM_OFFSET                  1
#define V_PCIE_CFG_VC1CTL_TVM_EN_VC                   0x60
#define B_PCIE_CFG_VC1CTL_ID_MASK                     0xF000000
#define B_PCIE_CFG_VC1CTL_ID_OFFSET                   24
#define V_PCIE_CFG_VC1CTL_ID_ONE                      1
#define B_PCIE_CFG_VC1CTL_EN_OFFSET                   0x80000000
#define B_PCIE_CFG_VC1CTL_EN                          BIT31

#define R_PCIE_PCIECFG2                               0x320          ///< PCI Express Configuration 2 Register
#define B_PCIE_PCIECFG2_LBWSSTE                       BIT30          ///< Low Bandwidth Squelch Settling Timer Enable
#define B_PCIE_PCIECFG2_RLLG3R                        BIT27          ///< Reset Link Layer In GEN3 Recovery
#define B_PCIE_PCIECFG2_CROAOV                        BIT24          ///< Completion Relaxed Ordering Attribute Override Value
#define B_PCIE_PCIECFG2_CROAOE                        BIT23          ///< Completion Relaxed Ordering Attribute Override Enable
#define B_PCIE_PCIECFG2_CRSREN                        BIT22          ///< Completion Retry Status Replay Enable
#define R_PCIE_PCIEDBG                                0x324          ///< PCI Express Debug And Configuration Resgister
#define B_PCIE_PCIEDBG_USSP                           BIT26 | BIT27  ///< Un-Squelch Sampling Period
#define N_PCIE_PCIEDBG_USSP_OFFSET                    26             ///< Un-Squelch Sampling Period offset
#define V_PCIE_PCIEDBG_USSP_32NS                      2              ///< 32ns
#define V_PCIE_PCIEDBG_USSP_16NS                      1              ///< 16ns
#define B_PCIE_PCIEDBG_LGCLKSQEXITDBTIMERS            BIT24          ///< Link Clock Domain Squelch Exit Debounce Timers
#define V_PCIE_PCIEDBG_LGCLKSQEXITDBTIMERS_0NS        0
#define N_PCIE_PCIEDBG_LGCLKSQEXITDBTIMERS_OFFSET     24
#define B_PCIE_PCIEDBG_SQOL0                          BIT7           ///< Squelch OFF in L0
#define B_PCIE_PCIEDBG_SPCE                           BIT5           ///< Squelch Propagation Control Enable

#define R_PCIE_PCIEPMECTL                             0x420          ///< PCIe PM Extension Control
#define B_PCIE_PCIEPMECTL_FDPPGE                      BIT31
#define B_PCIE_PCIEPMECTL_FDPPGE_OFFSET               31
#define B_PCIE_PCIEPMECTL_DLSULDLSD                   BIT29          ///< Disabled, Detect, L23_Rdy State,Un-Configured Lane and
                                                                    ///< Down-Configured Lane Squelch Disable
#define B_PCIE_PCIEPMECTL_POFFWT                      BIT19|BIT18
#define B_PCIE_PCIEPMECTL_L1FSOE                      BIT0           ///< L1 Full Squelch OFF Enable

#define R_PCIE_CFG_PCIEPMECTL2                        0x424
#define B_PCIE_CFG_PCIEPMECTL2_CPMCSRE                BIT27
#define B_PCIE_CFG_PCIEPMECTL2_CPMCSRE_OFFSET         27
#define B_PCIE_CFG_PCIEPMECTL2_CPGEXH                 BIT14
#define B_PCIE_CFG_PCIEPMECTL2_CPGEXH_OFFSET          14
#define B_PCIE_CFG_PCIEPMECTL2_CPGEXH_MASK            0xC000
#define V_PCIE_CFG_PCIEPMECTL2_CPGEXH                 0x1
#define B_PCIE_CFG_PCIEPMECTL2_CPGENH                 BIT12
#define B_PCIE_CFG_PCIEPMECTL2_CPGENH_OFFSET          12
#define B_PCIE_CFG_PCIEPMECTL2_CPGENH_MASK            0x3000
#define B_PCIE_CFG_PCIEPMECTL2_PHYCLPGE               BIT11
#define B_PCIE_CFG_PCIEPMECTL2_PHYCLPGE_OFFSET        11
#define B_PCIE_CFG_PCIEPMECTL2_L1SPHYDLPGE            BIT9
#define B_PCIE_CFG_PCIEPMECTL2_L1SPHYDLPGE_OFFSET     9
#define B_PCIE_CFG_PCIEPMECTL2_FDCPGE                 BIT8
#define B_PCIE_CFG_PCIEPMECTL2_FDCPGE_OFFSET          8
#define B_PCIE_CFG_PCIEPMECTL2_DETSCPGE               BIT7
#define B_PCIE_CFG_PCIEPMECTL2_DETSCPGE_OFFSET        7
#define B_PCIE_CFG_PCIEPMECTL2_L23RDYSCPGE            BIT6
#define B_PCIE_CFG_PCIEPMECTL2_L23RDYSCPGE_OFFSET     6
#define B_PCIE_CFG_PCIEPMECTL2_DISSCPGE               BIT5
#define B_PCIE_CFG_PCIEPMECTL2_DISSCPGE_OFFSET        5
#define B_PCIE_CFG_PCIEPMECTL2_L1SCPGE                BIT4
#define B_PCIE_CFG_PCIEPMECTL2_L1SCPGE_OFFSET         4

#define R_PCIE_CFG_PCE                                0x428
#define B_PCIE_CFG_PCE_HAE                            BIT5
#define B_PCIE_CFG_PCE_HAE_OFFSET                     5
#define B_PCIE_CFG_PCE_SE                             BIT3
#define B_PCIE_CFG_PCE_SE_OFFSET                      3
#define B_PCIE_CFG_PCE_PMCRE                          BIT0
#define B_PCIE_CFG_PCE_PMCRE_OFFSET                   0

#define R_PCIE_CFG_PGCBCTL1                           0x42C
#define B_PCIE_CFG_PGCBCTL1_TINACCRSTUP_MASK          0x3000
#define B_PCIE_CFG_PGCBCTL1_TACCRSTUP                 BIT10
#define B_PCIE_CFG_PGCBCTL1_TACCRSTUP_OFFSET          10
#define B_PCIE_CFG_PGCBCTL1_TACCRSTUP_MASK            0xC00

#define R_PCIE_CFG_PCIEPMECTL3                        0x434
#define B_PCIE_CFG_PCIEPMECTL3_L1PGAUTOPGEN           BIT4
#define B_PCIE_CFG_PCIEPMECTL3_PMREQCPGEXH            BIT0
#define B_PCIE_CFG_PCIEPMECTL3_PMREQCPGEXH_MASK       0x3
#define B_PCIE_CFG_PCIEPMECTL3_PMREQCPGEXH_OFFSET     0
#define V_PCIE_CFG_PCIEPMECTL3_PMREQCPGEXH            0x0

#define R_PCIE_CFG_EX_SPEECH                          0xA30 ///< Secondary PCI Express Extended Capability Header
#define V_PCIE_CFG_EX_SPEECH_CV                       0x1
#define R_PCIE_CFG_EX_LCTL3                           (R_PCIE_CFG_EX_SPEECH + R_PCIE_EX_LCTL3_OFFSET)
#define R_PCIE_CFG_EX_LES                             (R_PCIE_CFG_EX_SPEECH + R_PCIE_EX_LES_OFFSET)
#define R_PCIE_CFG_EX_LECTL                           (R_PCIE_CFG_EX_SPEECH + R_PCIE_EX_L01EC_OFFSET)
#define B_PCIE_CFG_EX_LECTL_UPTPH                     (BIT14 | BIT13 | BIT12)
#define N_PCIE_CFG_EX_LECTL_UPTPH                     12
#define B_PCIE_CFG_EX_LECTL_UPTP                      0x0F00
#define N_PCIE_CFG_EX_LECTL_UPTP                      8
#define B_PCIE_CFG_EX_LECTL_DPTPH                     (BIT6 | BIT5 | BIT4)
#define N_PCIE_CFG_EX_LECTL_DPTPH                     4
#define B_PCIE_CFG_EX_LECTL_DPTP                      0x000F
#define N_PCIE_CFG_EX_LECTL_DPTP                      0

#define R_PCIE_CFG_EQCFG1                             0x450
#define S_PCIE_CFG_EQCFG1                             4
#define B_PCIE_CFG_EQCFG1_REC                         0xFF000000
#define N_PCIE_CFG_EQCFG1_REC                         24
#define B_PCIE_CFG_EQCFG1_REIFECE                     BIT23
#define N_PCIE_CFG_EQCFG1_LERSMIE                     21
#define B_PCIE_CFG_EQCFG1_LEP23B                      BIT18
#define B_PCIE_CFG_EQCFG1_LEP3B                       BIT17
#define B_PCIE_CFG_EQCFG1_RTLEPCEB                    BIT16
#define B_PCIE_CFG_EQCFG1_RTPCOE                      BIT15
#define B_PCIE_CFG_EQCFG1_HPCMQE                      BIT13
#define B_PCIE_CFG_EQCFG1_HAED                        BIT12
#define V_PCIE_CFG_EQCFG1_RWTNEVE                     0xF
#define N_PCIE_CFG_EQCFG1_RWTNEVE                     8
#define B_PCIE_CFG_EQCFG1_EQTS2IRRC                   BIT7
#define B_PCIE_CFG_EQCFG1_TUPP                        BIT1

#define R_PCIE_CFG_RTPCL1                             0x454
#define B_PCIE_CFG_RTPCL1_PCM                         BIT31
#define B_PCIE_CFG_RTPCL1_RTPRECL2PL4                 0x3F000000
#define B_PCIE_CFG_RTPCL1_RTPOSTCL1PL3                0xFC0000
#define B_PCIE_CFG_RTPCL1_RTPRECL1PL2                 0x3F000
#define B_PCIE_CFG_RTPCL1_RTPOSTCL0PL1                0xFC0
#define B_PCIE_CFG_RTPCL1_RTPRECL0PL0                 0x3F

#define R_PCIE_CFG_EX_L01EC                           (R_PCIE_CFG_EX_SPEECH + R_PCIE_EX_L01EC_OFFSET)
#define B_PCIE_CFG_EX_L01EC_TP                        0xF ///< Bits used for each transmitter preset value
#define S_PCIE_CFG_EX_L01EC_TP                        8 ///< Each of the transmitter presets configuration fields
#define S_PCIE_CFG_EX_PL16L01EC_TP                    4 ///< Each of the transmitter presets configuration fields

#define R_PCIE_CFG_FOMS                               0x464
#define B_PCIE_CFG_FOMS_I                             (BIT30 | BIT29)
#define N_PCIE_CFG_FOMS_I                             29
#define B_PCIE_CFG_FOMS_LN                            0x1F000000
#define N_PCIE_CFG_FOMS_LN                            24
#define B_PCIE_CFG_FOMS_FOMSV                         0x00FFFFFF
#define B_PCIE_CFG_FOMS_FOMSV0                        0x000000FF
#define N_PCIE_CFG_FOMS_FOMSV0                        0
#define B_PCIE_CFG_FOMS_FOMSV1                        0x0000FF00
#define N_PCIE_CFG_FOMS_FOMSV1                        8
#define B_PCIE_CFG_FOMS_FOMSV2                        0x00FF0000
#define N_PCIE_CFG_FOMS_FOMSV2                        16

#define R_PCIE_CFG_HAEQ                               0x468
#define N_PCIE_CFG_HAEQ_DL                            8
#define B_PCIE_CFG_HAEQ_HAPCCPI                       (BIT31 | BIT30 | BIT29 | BIT28)
#define N_PCIE_CFG_HAEQ_HAPCCPI                       28
#define B_PCIE_CFG_HAEQ_MACFOMC                       BIT19

#define R_PCIE_CFG_LTCO1                              0x470
#define B_PCIE_CFG_LTCO1_L1TCOE                       BIT25
#define B_PCIE_CFG_LTCO1_L0TCOE                       BIT24
#define B_PCIE_CFG_LTCO1_L1TPOSTCO                    0xFC0000
#define N_PCIE_CFG_LTCO1_L1TPOSTCO                    18
#define B_PCIE_CFG_LTCO1_L1TPRECO                     0x3F000
#define N_PCIE_CFG_LTCO1_L1TPRECO                     12
#define B_PCIE_CFG_LTCO1_L0TPOSTCO                    0xFC0
#define N_PCIE_CFG_LTCO1_L0TPOSTCO                    6
#define B_PCIE_CFG_LTCO1_L0TPRECO                     0x3F
#define N_PCIE_CFG_LTCO1_L0TPRECO                     0

#define R_PCIE_CFG_EQCFG2                             0x47C
#define B_PCIE_CFG_EQCFG2_EMD                         BIT23
#define B_PCIE_CFG_EQCFG2_NTSS                        (BIT22 | BIT21 | BIT20)
#define B_PCIE_CFG_EQCFG2_PCET                        (BIT19 | BIT18 | BIT17 | BIT16)
#define N_PCIE_CFG_EQCFG2_PCET                        16
#define B_PCIE_CFG_EQCFG2_HAPCSB                      (BIT15 | BIT14 | BIT13 | BIT12)
#define N_PCIE_CFG_EQCFG2_HAPCSB                      12
#define B_PCIE_CFG_EQCFG2_NTEME                       BIT11
#define B_PCIE_CFG_EQCFG2_MPEME                       BIT10
#define B_PCIE_CFG_EQCFG2_REWMETM                     (BIT9 | BIT8)
#define B_PCIE_CFG_EQCFG2_REWMET                      0xFF

#define R_PCIE_CFG_MM                                 0x480
#define B_PCIE_CFG_MM_MSST                            0xFFFFFF00
#define N_PCIE_CFG_MM_MSST                            8
#define B_PCIE_CFG_MM_MSS                             0xFF

#define R_PCIE_CFG_EQCFG4                             0x48C
#define S_PCIE_CFG_EQCFG4                             4
#define B_PCIE_CFG_EQCFG4_PX8GTSWLPCE                 (BIT26 | BIT25 | BIT24)
#define B_PCIE_CFG_EQCFG4_PX8GTSWLPCE_OFFSET          24
#define V_PCIE_CFG_EQCFG4_PX8GTSWLPCE                 0x0
#define B_PCIE_CFG_EQCFG4_PX16GLEP23B                 BIT18
#define B_PCIE_CFG_EQCFG4_PX16GLEP3B                  BIT17
#define B_PCIE_CFG_EQCFG4_PX16GRTLEPCEB               BIT16
#define B_PCIE_CFG_EQCFG4_PX16GRTPCOE                 BIT15
#define B_PCIE_CFG_EQCFG4_HPCMQE                      BIT13
#define B_PCIE_CFG_EQCFG4_PX16GHAED                   BIT12
#define V_PCIE_CFG_EQCFG4_PX16GRWTNEVE                0xF
#define N_PCIE_CFG_EQCFG4_PX16GRWTNEVE                8
#define V_PCIE_CFG_EQCFG4_PX16GRWTNEVE_3US            0x3
#define B_PCIE_CFG_EQCFG4_PX16GEQTS2IRRC              BIT7
#define B_PCIE_CFG_EQCFG4_PX16GHAPCCPI                (BIT6 | BIT5 | BIT4 | BIT3)
#define B_PCIE_CFG_EQCFG4_PX16GHAPCCPI_OFFSET         3
#define V_PCIE_CFG_EQCFG4_PX16GHAPCCPI                0x2
#define B_PCIE_CFG_EQCFG4_PX16GHAPCCPIE               BIT2
#define B_PCIE_CFG_EQCFG4_PX16GMFLNTL                 BIT0

#define R_PCIE_CFG_CONTROL2                           0x4A4
#define B_PCIE_CFG_CONTROL2_PMETOFD                   BIT6

#define R_PCIE_CFG_PX16GRTPCL1                        0x4DC
#define B_PCIE_CFG_PX16GRTPCL1_PCM                    BIT31
#define B_PCIE_CFG_PX16GRTPCL1_RTPRECL2PL4            0x3F000000
#define B_PCIE_CFG_PX16GRTPCL1_RTPOSTCL1PL3           0xFC0000
#define B_PCIE_CFG_PX16GRTPCL1_RTPRECL1PL2            0x3F000
#define B_PCIE_CFG_PX16GRTPCL1_RTPOSTCL0PL1           0xFC0
#define B_PCIE_CFG_PX16GRTPCL1_RTPRECL0PL0            0x3F

#define R_PCIE_CFG_EQCFG5                             0x4F8
#define B_PCIE_CFG_EQCFG5_PCET                        (BIT19 | BIT18 | BIT17 | BIT16)
#define N_PCIE_CFG_EQCFG5_PCET                        16
#define B_PCIE_CFG_EQCFG5_HAPCSB                      (BIT15 | BIT14 | BIT13 | BIT12)
#define N_PCIE_CFG_EQCFG5_HAPCSB                      12
#define B_PCIE_CFG_EQCFG5_NTEME                       BIT11
#define B_PCIE_CFG_EQCFG5_MPEME                       BIT10
#define B_PCIE_CFG_EQCFG5_REWMETM                     (BIT9 | BIT8)
#define B_PCIE_CFG_EQCFG5_REWMET                      0xFF

#define R_PCIE_CFG_LANE0_PRESETS                      0x500
#define S_PCIE_CFG_SINGLE_LANE_PRESETS                0x20
#define R_PCIE_CFG_LFFS_OFFSET_FROM_LANE_BASE         0x1C   ///< This register offset is taken from base of the lane presets configuration
                                                             ///< starting with R_PCIE_CFG_LANE0_PRESETS register.
#define N_PCIE_CFG_LFFS_FS                            8      ///< Offset of LFFS being 0x51C

#define R_PCIE_CFG_PX16GP0P1PCM                       0x520

#define R_PCIE_CFG_PX16GLTCO1                         0x600
#define B_PCIE_CFG_PX16GLTCO1_L1TCOE                  BIT25
#define B_PCIE_CFG_PX16GLTCO1_L0TCOE                  BIT24
#define B_PCIE_CFG_PX16GLTCO1_L1TPOSTCO               0xFC0000
#define N_PCIE_CFG_PX16GLTCO1_L1TPOSTCO               18
#define B_PCIE_CFG_PX16GLTCO1_L1TPRECO                0x3F000
#define N_PCIE_CFG_PX16GLTCO1_L1TPRECO                12
#define B_PCIE_CFG_PX16GLTCO1_L0TPOSTCO               0xFC0
#define N_PCIE_CFG_PX16GLTCO1_L0TPOSTCO               6
#define B_PCIE_CFG_PX16GLTCO1_L0TPRECO                0x3F
#define N_PCIE_CFG_PX16GLTCO1_L0TPRECO                0

#define R_PCIE_CFG_EX_PL16L01EC                       0xABC
#define R_PCIE_CFG_EX_PL16L23EC                       0xABE

//
// PTM pipe stage delay registers. Valid from PCIe SIP14
//
#define R_PCIE_CFG_PTMPSDC1                           0x39C ///< PTM Pipe Stage Delay Configuration 1
#define R_PCIE_CFG_PTMPSDC2                           0x3A0 ///< PTM Pipe Stage Delay Configuration 2
#define R_PCIE_CFG_PTMPSDC3                           0x3A4 ///< PTM Pipe Stage Delay Configuration 3
#define R_PCIE_CFG_PTMPSDC4                           0x3A8 ///< PTM Pipe Stage Delay Configuration 4
#define R_PCIE_CFG_PTMPSDC5                           0x3AC ///< PTM Pipe Stage Delay Configuration 5
#define R_PCIE_CFG_PTMECFG                            0x3B0 ///< PTM Extended Configuration

//
// Coalescing registers. From PCIe SIP 16
//
#define R_PCIE_CFG_COCTL                              0x594
#define B_PCIE_CFG_COCTL_NPCLM                        BIT15
#define N_PCIE_CFG_COCTL_NPCLM_OFFSET                 15
#define B_PCIE_CFG_COCTL_NPCLM_MASK                   (BIT15 | BIT16)
#define B_PCIE_CFG_COCTL_PCLM                         BIT13
#define N_PCIE_CFG_COCTL_PCLM_OFFSET                  13
#define B_PCIE_CFG_COCTL_PCLM_MASK                    (BIT13 | BIT14)
#define B_PCIE_CFG_COCTL_MAGPARCD                     BIT12
#define N_PCIE_CFG_COCTL_MAGPARCD_OFFSET              12
#define B_PCIE_CFG_COCTL_ROAOP                        BIT11
#define N_PCIE_CFG_COCTL_ROAOP_OFFSET                 11
#define B_PCIE_CFG_COCTL_CTE                          BIT10
#define N_PCIE_CFG_COCTL_CTE_OFFSET                   10
#define B_PCIE_CFG_COCTL_CT                           BIT2
#define N_PCIE_CFG_COCTL_CT_OFFSET                    2
#define B_PCIE_CFG_COCTL_CT_MASK                      0x3FC
#define B_PCIE_CFG_COCTL_DDCE                         BIT1
#define N_PCIE_CFG_COCTL_DDCE_OFFSET                  1
#define B_PCIE_CFG_COCTL_PWCE                         BIT0
#define N_PCIE_CFG_COCTL_PWCE_OFFSET                  0

//
// PTM pipe stage delay register to cover x16 and x8 links. Added in PCIe SIP16
//
#define R_PCIE_CFG_PTMPSDC6                           0x5B0 ///< SIP16 PTM Pipe Stage Delay Configuration 6
#define R_PCIE_CFG_PTMPSDC7                           0x5B4 ///< SIP16 PTM Pipe Stage Delay Configuration 7
#define R_PCIE_CFG_PTMPSDC8                           0x5B8 ///< SIP16 PTM Pipe Stage Delay Configuration 8


//
// Advance Mode Control Register
//
#define R_PCIE_ADVMCTRL                               0x5BC
#define B_PCIE_ADVMCTRL_F10BTSE                       BIT24
#define B_PCIE_ADVMCTRL_INRXL0CTRL                    BIT22
#define B_PCIE_ADVMCTRL_ACCRM                         BIT21
#define B_PCIE_ADVMCTRL_EIOSDISDS                     BIT20
#define B_PCIE_ADVMCTRL_EIOSMASKRX                    BIT19
#define B_PCIE_ADVMCTRL_PMREQCWC                      BIT16
#define B_PCIE_ADVMCTRL_PMREQCWC_MASK                 (BIT18 | BIT17 | BIT16)
#define N_PCIE_ADVMCTRL_PMREQCWC_OFFSET               16
#define V_PCIE_ADVMCTRL_PMREQCWC_LNK_PRIM             6
#define B_PCIE_ADVMCTRL_RXL0DC                        BIT15
#define B_PCIE_ADVMCTRL_G3STFER                       BIT13
#define B_PCIE_ADVMCTRL_RRLLCL                        BIT11
#define B_PCIE_ADVMCTRL_RLLG12R                       BIT10
#define B_PCIE_ADVMCTRL_PMREQBLKPGRSPT                BIT5
#define B_PCIE_ADVMCTRL_PMREQBLKPGRSPT_MASK           (BIT7 | BIT6 | BIT5)
#define N_PCIE_ADVMCTRL_PMREQBLKPGRSPT_OFFSET         5
#define V_PCIE_ADVMCTRL_PMREQBLKPGRSPT_10US           2

#define R_PCIE_CFG_HWSNR                              0x5F0
#define B_PCIE_HWSNR_READY4PG                         BIT10
#define B_PCIE_HWSNR_READY4PG_OFFSET                  10
#define B_PCIE_CFG_HWSNR_EEH                          BIT8
#define B_PCIE_CFG_HWSNR_EEH_OFFSET                   8
#define B_PCIE_CFG_HWSNR_REPW                         BIT4
#define B_PCIE_CFG_HWSNR_REPW_OFFSET                  4
#define B_PCIE_CFG_HWSNR_BEPW                         BIT0
#define B_PCIE_CFG_HWSNR_BEPW_OFFSET                  0
#define B_PCIE_CFG_HWSNR_BEPW_8_CLKS                  0x5
#define R_PCIE_CFG_PGCTRL                             0x5F4
#define B_PCIE_CFG_PGCTRL_PMREQBLKRSPT                BIT0
#define B_PCIE_CFG_PGCTRL_PMREQBLKRSPT_OFFSET         0
#define V_PCIE_CFG_PGCTRL_PMREQBLKRSPT_10MICRO_SEC    0x2
#define V_PCIE_CFG_PGCTRL_PMREQBLKRSPT_25MICRO_SEC    0x5

#define R_PCIE_CFG_PGTHRES                            0x5C0
#define B_PCIE_CFG_PGTHRES_L1PGLTRTLSV                BIT29
#define N_PCIE_CFG_PGTHRES_L1PGLTRTLSV_OFFSET         29
#define B_PCIE_CFG_PGTHRES_L1PGLTRTLSV_MASK           0xE0000000
#define B_PCIE_CFG_PGTHRES_L1PGLTRTLV                 BIT16
#define N_PCIE_CFG_PGTHRES_L1PGLTRTLV_OFFSET          16
#define B_PCIE_CFG_PGTHRES_L1PGLTRTLV_MASK            0x3FF0000
#define B_PCIE_CFG_PGTHRES_L1PGLTREN                  BIT0
#define N_PCIE_CFG_PGTHRES_L1PGLTREN_OFFSET           0

#define  R_PCH_PCIE_DLFECH                            0xA90
#define  B_PCH_PCIE_DLFECH_NCO                        BIT20
#define  B_PCH_PCIE_DLFECH_NCO_OFFSET                 20
#define  B_PCH_PCIE_DLFECH_NCO_MASK                   0xFFF00000
#define  B_PCH_PCIE_DLFECH_CV                         BIT16
#define  B_PCH_PCIE_DLFECH_CV_OFFSET                  16
#define  B_PCH_PCIE_DLFECH_CV_MASK                    0xF0000
#define  V_PCH_PCIE_DLFECH_CV                         0x1
#define  B_PCH_PCIE_DLFECH_PCIEECID                   BIT0
#define  B_PCH_PCIE_DLFECH_PCIEECID_OFFSET            0
#define  B_PCH_PCIE_DLFECH_PCIEECID_MASK              0xFFFF

#define R_PCIE_CFG_HWSNR                              0x5F0
#define B_PCIE_HWSNR_READY4PG                         BIT10
#define B_PCIE_HWSNR_READY4PG_OFFSET                  10
#define B_PCIE_CFG_HWSNR_EEH                          BIT8
#define B_PCIE_CFG_HWSNR_EEH_MASK                     (BIT9 | BIT8)
#define V_PCIE_CFG_HWSNR_EEH_16CLK                    2
#define N_PCIE_CFG_HWSNR_EEH_OFFSET                   8
#define B_PCIE_CFG_HWSNR_REPW                         BIT4
#define B_PCIE_CFG_HWSNR_REPW_MASK                    (BIT7 | BIT6 | BIT5 | BIT4)
#define V_PCIE_CFG_HWSNR_REPW_2CLK                    1
#define N_PCIE_CFG_HWSNR_REPW_OFFSET                  4
#define B_PCIE_CFG_HWSNR_BEPW                         BIT0
#define B_PCIE_CFG_HWSNR_BEPW_MASK                    (BIT0 | BIT1 | BIT2 | BIT3)
#define V_PCIE_CFG_HWSNR_BEPW_8CLK                    5

#define R_PCIE_CFG_PGCTRL                             0x5F4
#define B_PCIE_CFG_PGCTRL_PMREQBLKRSPT                BIT0
#define B_PCIE_CFG_PGCTRL_PMREQBLKRSPT_MASK           (BIT0 | BIT1 | BIT2)
#define V_PCIE_CFG_PGCTRL_PMREQBLKRSPT_10US           2

#define R_PCIE_CFG_ACRG3                              0x6CC
#define B_PCIE_CFG_ACRG3_CPGWAKECTRL                  BIT22
#define N_PCIE_CFG_ACRG3_CPGWAKECTRL_OFFSET           22
#define B_PCIE_CFG_ACRG3_CPGWAKECTRL_MASK             0xC00000
#define V_PCIE_CFG_ACRG3_CPGWAKECTRL                  0x2

#define R_PCIE_CFG_DLFECH                             0xA90
#define B_PCIE_CFG_DLFECH_NCO                         BIT20
#define B_PCIE_CFG_DLFECH_NCO_OFFSET                  20
#define B_PCIE_CFG_DLFECH_NCO_MASK                    0xFFF00000
#define B_PCIE_CFG_DLFECH_CV                          BIT16
#define B_PCIE_CFG_DLFECH_CV_OFFSET                   16
#define B_PCIE_CFG_DLFECH_CV_MASK                     0xF0000
#define V_PCIE_CFG_DLFECH_CV                          0x1
#define B_PCIE_CFG_DLFECH_PCIEECID                    BIT0
#define B_PCIE_CFG_DLFECH_PCIEECID_OFFSET             0
#define B_PCIE_CFG_DLFECH_PCIEECID_MASK               0xFFFF

#define R_PCIE_CFG_PL16GECH                           0xA9C
#define B_PCIE_CFG_PL16GECH_NCO                       BIT20
#define B_PCIE_CFG_PL16GECH_NCO_OFFSET                20
#define B_PCIE_CFG_PL16GECH_NCO_MASK                  0xFFF00000
#define B_PCIE_CFG_PL16GECH_CV                        BIT16
#define B_PCIE_CFG_PL16GECH_CV_OFFSET                 16
#define B_PCIE_CFG_PL16GECH_CV_MASK                   0xF0000
#define V_PCIE_CFG_PL16GECH_CV                        1
#define B_PCIE_CFG_PL16GECH_CID                       BIT0
#define B_PCIE_CFG_PL16GECH_CID_OFFSET                0
#define B_PCIE_CFG_PL16GECH_CID_MASK                  0xFFFF

#define R_PCIE_CFG_APEC                               0xB0C
#define B_PCIE_CFG_APEC_NCO                           BIT20
#define B_PCIE_CFG_APEC_NCO_OFFSET                    20
#define B_PCIE_CFG_APEC_NCO_MASK                      0xFFF00000
#define B_PCIE_CFG_APEC_CV                            BIT16
#define B_PCIE_CFG_APEC_CV_OFFSET                     16
#define B_PCIE_CFG_APEC_CV_MASK                       0xF0000
#define V_PCIE_CFG_APEC_CV                            1
#define B_PCIE_CFG_APEC_CID                           BIT0
#define B_PCIE_CFG_APEC_CID_OFFSET                    0
#define B_PCIE_CFG_APEC_CID_MASK                      0xFFFF

//
// PTM pipe stage delay register to cover GEN4 stage delays. Added in PCIe SIP16
//
#define R_PCIE_CFG_PTMPSDC9                           0xCB0 ///< SIP16 PTM Pipe Stage Delay Configuration 9
#define R_PCIE_CFG_PTMPSDC10                          0xCB4 ///< SIP16 PTM Pipe Stage Delay Configuration 10
#define R_PCIE_CFG_PTMPSDC11                          0xCB8 ///< SIP16 PTM Pipe Stage Delay Configuration 11

#define R_PCIE_CFG_PL16MECH                           0xEDC
#define B_PCIE_CFG_PL16MECH_NCO                       BIT20
#define B_PCIE_CFG_PL16MECH_NCO_OFFSET                20
#define B_PCIE_CFG_PL16MECH_NCO_MASK                  0xFFF00000
#define B_PCIE_CFG_PL16MECH_CV                        BIT16
#define B_PCIE_CFG_PL16MECH_CV_OFFSET                 16
#define B_PCIE_CFG_PL16MECH_CV_MASK                   0xF0000
#define V_PCIE_CFG_PL16MECH_CV                        1
#define B_PCIE_CFG_PL16MECH_CID                       BIT0
#define B_PCIE_CFG_PL16MECH_CID_OFFSET                0
#define B_PCIE_CFG_PL16MECH_CID_MASK                  0xFFFF

//
// LTR subtraction registers. Added in PCIe SIP 16
//
#define R_PCIE_CFG_LTRSUBL1STD                        0xC5C
#define R_PCIE_CFG_LTRSUBL11                          0xC60
#define R_PCIE_CFG_LTRSUBL12                          0xC64
#define B_PCIE_CFG_LTRSUB_LTRNSLSUBEN                 BIT31
#define B_PCIE_CFG_LTRSUB_LTRNSLSSUBV                 BIT26
#define N_PCIE_CFG_LTRSUB_LTRNSLSSUBV                 26
#define B_PCIE_CFG_LTRSUB_LTRNSLSSUBV_MASK            (BIT28 | BIT27 | BIT26)
#define B_PCIE_CFG_LTRSUB_LTRNSLSUBV                  BIT16
#define N_PCIE_CFG_LTRSUB_LTRNSLSUBV                  16
#define B_PCIE_CFG_LTRSUB_LTRNSLSUBV_MASK             0x03FF0000
#define B_PCIE_CFG_LTRSUB_LTRSLSUBEN                  BIT15
#define B_PCIE_CFG_LTRSUB_LTRSLSSUBV                  BIT10
#define N_PCIE_CFG_LTRSUB_LTRSLSSUBV                  10
#define B_PCIE_CFG_LTRSUB_LTRSLSSUBV_MASK             (BIT12 | BIT11 | BIT10)
#define B_PCIE_CFG_LTRSUB_LTRSLSUBV                   BIT0
#define N_PCIE_CFG_LTRSUB_LTRSLSUBV                   0
#define B_PCIE_CFG_LTRSUB_LTRSLSUBV_MASK              0x000003FF

#define R_PCIE_LTRSUBL11NPG                           0xC68

#define R_PCIE_CFG_LPCR                               0xC8C
#define B_PCIE_CFG_LPCR_DIDOVR_LOCK                   BIT24
#define B_PCIE_CFG_LPCR_LTRCFGLOCK                    BIT16
#define B_PCIE_CFG_LPCR_SERL                          BIT8
#define B_PCIE_CFG_LPCR_SRL                           BIT0

//
// PCIe RCRB space available from SIP17
//
#define R_PCIE_RCRB_FCTL                              0x1300 ///< SIP17 Feature Control
#define R_PCIE_RCRB_FCTL2                             0x1330               ///< SIP17 Feature Control 2
#define B_PCIE_RCRB_FCTL2_RPTOT                       BIT6 | BIT5 | BIT4
#define B_PCIE_RCRB_FCTL2_HRTCTL                      BIT9 | BIT8 | BIT7

#define R_PCIE_RCRB_RPR                               0x1334               ///< SIP17 Reset Policy
#define B_PCIE_RCRB_RPR_WRM                           BIT3 | BIT2
#define B_PCIE_RCRB_RPR_CRM                           BIT5 | BIT4
#define B_PCIE_RCRB_RPR_S3SM                          BIT7 | BIT6
#define B_PCIE_RCRB_RPR_S4SM                          BIT9 | BIT8
#define B_PCIE_RCRB_RPR_S5SM                          BIT11 | BIT10
#define B_PCIE_RCRB_RPR_DTMO                          BIT13 | BIT12
#define B_PCIE_RCRB_RPR_WRMTO                         BIT15 | BIT14
#define B_PCIE_RCRB_RPR_CRMTO                         BIT17 | BIT16
#define B_PCIE_RCRB_RPR_S3SMTO                        BIT19 | BIT18
#define B_PCIE_RCRB_RPR_S4SMTO                        BIT21 | BIT20
#define B_PCIE_RCRB_RPR_S5SMTO                        BIT23 | BIT22

#define R_PCIE_RCRB_DCGEN1                            0x1350
#define B_PCIE_RCRB_DCGEN1_PXCP4ULDCGEN               BIT27
#define B_PCIE_RCRB_DCGEN1_PXTTSSULDCGEN              BIT26
#define B_PCIE_RCRB_DCGEN1_PXTTSULDCGEN               BIT25
#define B_PCIE_RCRB_DCGEN1_PXTTULDCGEN                BIT24
#define B_PCIE_RCRB_DCGEN1_PXCP3ULDCGEN               BIT19
#define B_PCIE_RCRB_DCGEN1_PXTRSSULDCGEN              BIT18
#define B_PCIE_RCRB_DCGEN1_PXTRSULDCGEN               BIT17
#define B_PCIE_RCRB_DCGEN1_PXTRULDCGEN                BIT16
#define B_PCIE_RCRB_DCGEN1_PXCP2ULDCGEN               BIT12
#define B_PCIE_RCRB_DCGEN1_PXLSULDCGEN                BIT11
#define B_PCIE_RCRB_DCGEN1_PXLIULDCGEN                BIT10
#define B_PCIE_RCRB_DCGEN1_PXLTULDCGEN                BIT9
#define B_PCIE_RCRB_DCGEN1_PXLRULDCGEN                BIT8
#define B_PCIE_RCRB_DCGEN1_PXCULDCGEN                 BIT1
#define B_PCIE_RCRB_DCGEN1_PXKGULDCGEN                BIT0

#define  R_PCIE_RCRB_DCGEN2                           0x1354
#define  B_PCIE_RCRB_DCGEN2_PXCP5ULDCGEN              BIT6
#define  B_PCIE_RCRB_DCGEN2_PXFRULDCGEN               BIT5
#define  B_PCIE_RCRB_DCGEN2_PXFTULDCGEN               BIT4
#define  B_PCIE_RCRB_DCGEN2_PXFIULDCGEN               BIT3
#define  B_PCIE_RCRB_DCGEN2_PXPBULDCGEN               BIT2
#define  B_PCIE_RCRB_DCGEN2_PXPSULDCGEN               BIT1
#define  B_PCIE_RCRB_DCGEN2_PXPIULDCGEN               BIT0

#define  R_PCIE_RCRB_DCGM1                            0x1358
#define  B_PCIE_RCRB_DCGM1_PXCP4ULDCGM                BIT27
#define  B_PCIE_RCRB_DCGM1_PXTTSSULDCGM               BIT26
#define  B_PCIE_RCRB_DCGM1_PXTTSULDCGM                BIT25
#define  B_PCIE_RCRB_DCGM1_PXTTULDCGM                 BIT24
#define  B_PCIE_RCRB_DCGM1_PXCP3ULDCGM                BIT19
#define  B_PCIE_RCRB_DCGM1_PXTRSSULDCGM               BIT18
#define  B_PCIE_RCRB_DCGM1_PXTRSULDCGM                BIT17
#define  B_PCIE_RCRB_DCGM1_PXTRULDCGM                 BIT16
#define  B_PCIE_RCRB_DCGM1_PXCP2ULDCGM                BIT12
#define  B_PCIE_RCRB_DCGM1_PXLSULDCGM                 BIT11
#define  B_PCIE_RCRB_DCGM1_PXLIULDCGM                 BIT10
#define  B_PCIE_RCRB_DCGM1_PXLTULDCGM                 BIT9
#define  B_PCIE_RCRB_DCGM1_PXLRULDCGM                 BIT8
#define  B_PCIE_RCRB_DCGM1_PXCULDCGM                  BIT1
#define  B_PCIE_RCRB_DCGM1_PXKGULDCGM                 BIT0

#define  R_PCIE_RCRB_DCGM2                            0x135C
#define  B_PCIE_RCRB_DCGM2_PXCP5ULDCGM                BIT6
#define  B_PCIE_RCRB_DCGM2_PXFRULDCGM                 BIT5
#define  B_PCIE_RCRB_DCGM2_PXFTULDCGM                 BIT4
#define  B_PCIE_RCRB_DCGM2_PXFIULDCGM                 BIT3
#define  B_PCIE_RCRB_DCGM2_PXPBULDCGM                 BIT2
#define  B_PCIE_RCRB_DCGM2_PXPSULDCGM                 BIT1
#define  B_PCIE_RCRB_DCGM2_PXPIULDCGM                 BIT0

#define  R_PCIE_RCRB_DCGEN3                           0x1360
#define  B_PCIE_RCRB_DCGEN3_PXCP4UPDCGEN              BIT27
#define  B_PCIE_RCRB_DCGEN3_PXTTSSUPDCGEN             BIT26
#define  B_PCIE_RCRB_DCGEN3_PXTTSUPDCGEN              BIT25
#define  B_PCIE_RCRB_DCGEN3_PXTTUPDCGEN               BIT24
#define  B_PCIE_RCRB_DCGEN3_PXCP3UPDCGEN              BIT20
#define  B_PCIE_RCRB_DCGEN3_PXTOUPDCGEN               BIT19
#define  B_PCIE_RCRB_DCGEN3_PXTRSSUPDCGEN             BIT18
#define  B_PCIE_RCRB_DCGEN3_PXTRSUPDCGEN              BIT17
#define  B_PCIE_RCRB_DCGEN3_PXTRUPDCGEN               BIT16
#define  B_PCIE_RCRB_DCGEN3_PXCP2UPDCGEN              BIT12
#define  B_PCIE_RCRB_DCGEN3_PXLIUPDCGEN               BIT10
#define  B_PCIE_RCRB_DCGEN3_PXLTUPDCGEN               BIT9
#define  B_PCIE_RCRB_DCGEN3_PXLRUPDCGEN               BIT8
#define  B_PCIE_RCRB_DCGEN3_PXCUPSRCDCGEN             BIT6
#define  B_PCIE_RCRB_DCGEN3_PXCUPSNRDCGEN             BIT5
#define  B_PCIE_RCRB_DCGEN3_PXSRUSSNRDCGEN            BIT4
#define  B_PCIE_RCRB_DCGEN3_PXCUPDCGEN                BIT2
#define  B_PCIE_RCRB_DCGEN3_PXBUPDCGEN                BIT1
#define  B_PCIE_RCRB_DCGEN3_PXEUPDCGEN                BIT0

#define  R_PCIE_RCRB_DCGM3                            0x1368
#define  B_PCIE_RCRB_DCGM3_PXCP4UPDCGM                BIT27
#define  B_PCIE_RCRB_DCGM3_PXTTSSUPDCGM               BIT26
#define  B_PCIE_RCRB_DCGM3_PXTTSUPDCGM                BIT25
#define  B_PCIE_RCRB_DCGM3_PXTTUPDCGM                 BIT24
#define  B_PCIE_RCRB_DCGM3_PXCP3UPDCGM                BIT20
#define  B_PCIE_RCRB_DCGM3_PXTOUPDCGM                 BIT19
#define  B_PCIE_RCRB_DCGM3_PXTRSSUPDCGM               BIT18
#define  B_PCIE_RCRB_DCGM3_PXTRSUPDCGM                BIT17
#define  B_PCIE_RCRB_DCGM3_PXTRUPDCGM                 BIT16
#define  B_PCIE_RCRB_DCGM3_PXCP2UPDCGM                BIT12
#define  B_PCIE_RCRB_DCGM3_PXLIUPDCGM                 BIT10
#define  B_PCIE_RCRB_DCGM3_PXLTUPDCGM                 BIT9
#define  B_PCIE_RCRB_DCGM3_PXLRUPDCGM                 BIT8
#define  B_PCIE_RCRB_DCGM3_PXSRUSSNRDCGM              BIT6
#define  B_PCIE_RCRB_DCGM3_PXCUPSNRDCGM               BIT5
#define  B_PCIE_RCRB_DCGM3_PXCUPSRCDCGM               BIT4
#define  B_PCIE_RCRB_DCGM3_PXCUPDCGM                  BIT2
#define  B_PCIE_RCRB_DCGM3_PXBUPDCGM                  BIT1
#define  B_PCIE_RCRB_DCGM3_PXEUPDCGM                  BIT0

#define  R_PCIE_RCRB_DCGM4                            0x136C
#define  B_PCIE_RCRB_DCGM4_PXCP6UPDCGM                BIT8
#define  B_PCIE_RCRB_DCGM4_PXCP5UPDCGM                BIT0

#define  R_PCIE_RCRB_IPCLKCTR                         0x1370
#define  B_PCIE_RCRB_IPCLKCTR_PXDCGE                  BIT2

#define  R_PCIE_RCRB_LPHYCP1                          0x1410
#define  B_PCIE_RCRB_LPHYCP1_BPRXSTNDBYHSRECAL        BIT12
#define  B_PCIE_RCRB_LPHYCP1_RXADPRECE                BIT9
#define  B_PCIE_RCRB_LPHYCP1_PIPEMBIP                 BIT6
#define  B_PCIE_RCRB_LPHYCP1_RXEQFNEVC                BIT4
#define  B_PCIE_RCRB_LPHYCP1_RXADPHM                  BIT3
#define  B_PCIE_RCRB_LPHYCP1_RXADPSVH                 BIT2

#define  R_PCIE_RCRB_LPHYCP4                             0x1434
#define  B_PCIE_RCRB_LPHYCP4_REEFRXL0SED                 BIT11
#define  B_PCIE_RCRB_LPHYCP4_REEFRXL0SED_OFFSET          11
#define  B_PCIE_RCRB_LPHYCP4_PLLBUSDRC                   BIT10
#define  B_PCIE_RCRB_LPHYCP4_OSUSSP                      BIT8
#define  B_PCIE_RCRB_LPHYCP4_OSUSSP_OFFSET               8
#define  B_PCIE_RCRB_LPHYCP4_OSUSSP_MASK                 0x300
#define  V_PCIE_RCRB_LPHYCP4_OSUSSP                      0x1
#define  B_PCIE_RCRB_LPHYCP4_LGUSSP                      BIT6
#define  B_PCIE_RCRB_LPHYCP4_LGUSSP_OFFSET               6
#define  B_PCIE_RCRB_LPHYCP4_LGUSSP_MASK                 0xC0
#define  V_PCIE_RCRB_LPHYCP4_LGUSSP                      0x1
#define  B_PCIE_RCRB_LPHYCP4_OSCCLKSQEXITDBTIMERS        BIT3
#define  B_PCIE_RCRB_LPHYCP4_OSCCLKSQEXITDBTIMERS_OFFSET 3
#define  B_PCIE_RCRB_LPHYCP4_OSCCLKSQEXITDBTIMERS_MASK   0x38
#define  V_PCIE_RCRB_LPHYCP4_OSCCLKSQEXITDBTIMERS        0x2
#define  B_PCIE_RCRB_LPHYCP4_LGCLKSQEXITDBTIMERS         BIT0
#define  B_PCIE_RCRB_LPHYCP4_LGCLKSQEXITDBTIMERS_OFFSET  0
#define  B_PCIE_RCRB_LPHYCP4_LGCLKSQEXITDBTIMERS_MASK    0x7
#define  V_PCIE_RCRB_LPHYCP4_LGCLKSQEXITDBTIMERS         0x2

#define R_PCIE_RCRB_PIPEPDCTL                         0x1594               ///< SIP17 PIPE Power Down Control
#define B_PCIE_RCRB_PIPEPDCTL_L1PGUPGPDCTL            0xF0000000
#define B_PCIE_RCRB_PIPEPDCTL_L1PGNOPGPDCTL           0x0F000000
#define B_PCIE_RCRB_PIPEPDCTL_DISPGPDCTL              0x00F00000
#define B_PCIE_RCRB_PIPEPDCTL_DISNOPGPDCTL            0x000F0000
#define B_PCIE_RCRB_PIPEPDCTL_L23PGPDCTL              0x0000F000
#define B_PCIE_RCRB_PIPEPDCTL_L23NOPGPDCTL            0x00000F00
#define B_PCIE_RCRB_PIPEPDCTL_DETPGPDCTL              0x000000F0
#define B_PCIE_RCRB_PIPEPDCTL_DETNOPGPDCTL            0x0000000F

#define R_PCIE_RCRB_PIPEPDCTL2                        0x1598               ///< SIP17 PIPE Power Down Control 2
#define B_PCIE_RCRB_PIPEPDSTL2_DUCFGPGPDCTL           0xF0000000
#define B_PCIE_RCRB_PIPEPDSTL2_DUCFGNOPGPDCTL         0x0F000000
#define B_PCIE_RCRB_PIPEPDSTL2_L1D2PGPDCTL            0x00F00000
#define B_PCIE_RCRB_PIPEPDSTL2_L1D2NOPGPDCTL          0x000F0000
#define B_PCIE_RCRB_PIPEPDSTL2_L1D1PGPGPDCTL          0x0000F000
#define B_PCIE_RCRB_PIPEPDSTL2_L1D1PGNOPGPDCTL        0x00000F00
#define B_PCIE_RCRB_PIPEPDSTL2_L1D1UPUPGPDCTL         0x000000F0
#define B_PCIE_RCRB_PIPEPDSTL2_L1UPNOPGPDCTL          0x0000000F

#define R_PCIE_RCRB_PIPEPDCTL3                        0x159C               ///< SIP17 PIPE Power Down Control 3
#define B_PCIE_RCRB_PIPEPDCTL3_L1DLOWPGPDCTL          0x000000F0
#define B_PCIE_RCRB_PIPEPDCTL3_L1DLOWNOPGPDCTL        0x0000000F

#define R_PCIE_RCRB_PIPEPDCTLEXT                      0x15A0               ///< SIP17 PIPE Power Down Control Extension
#define B_PCIE_RCRB_PIPEPDCTLEXT_P2TP2TP              BIT2
#define B_PCIE_RCRB_PIPEPDCTLEXT_P2TP2TCD             BIT1

#define R_PCIE_RCRB_RPDEC1                            0x1780               ///< SIP17 Reset Prep Decode
#define B_PCIE_RCRB_RPDEC1_RPWRERT                    0xFF
#define B_PCIE_RCRB_RPDEC1_RPWREPT                    0xFF00
#define B_PCIE_RCRB_RPDEC1_RPCRERT                    0xFF0000
#define B_PCIE_RCRB_RPDEC1_RPCREPT                    0xFF000000

#define R_PCIE_RCRB_RPDEC2                            0x1784               ///< SIP17 Reset Prep Decode 2
#define B_PCIE_RCRB_RPDEC2_RPS3ERT                    0xFF
#define B_PCIE_RCRB_RPDEC2_RPS3EPT                    0xFF00
#define B_PCIE_RCRB_RPDEC2_RPS4ERT                    0xFF0000
#define B_PCIE_RCRB_RPDEC2_RPS4EPT                    0xFF000000

#define R_PCIE_RCRB_RPDEC3                            0x1788               ///< SIP17 Reset Prep Decode 3
#define B_PCIE_RCRB_RPDEC3_RPS5ERT                    0xFF
#define B_PCIE_RCRB_RPDEC3_RPS5EPT                    0xFF00
#define B_PCIE_RCRB_RPDEC3_RPDH                       0x70000

#define R_PCIE_RCRB_CFG_DECCTL                        0x1904
#define B_PCIE_RCRB_CFG_DECCTL_RXIMDECEN              BIT31
#define B_PCIE_RCRB_CFG_DECCTL_URRXMCTPNTCO           BIT30
#define B_PCIE_RCRB_CFG_DECCTL_RXMCTPBRCDECEN         BIT29
#define B_PCIE_RCRB_CFG_DECCTL_URRXUVDMRGRTRC         BIT28
#define B_PCIE_RCRB_CFG_DECCTL_URRXUVDMRBFRC          BIT27
#define B_PCIE_RCRB_CFG_DECCTL_LCRXPTMREQ             BIT26
#define B_PCIE_RCRB_CFG_DECCTL_RSVD_RW                BIT25
#define B_PCIE_RCRB_CFG_DECCTL_LCRXERRMSG             BIT24
#define B_PCIE_RCRB_CFG_DECCTL_RXLTRMDECH             BIT23
#define B_PCIE_RCRB_CFG_DECCTL_RXADLEDDECEN           BIT22
#define B_PCIE_RCRB_CFG_DECCTL_RXSBEMCAPDECEN         BIT21
#define B_PCIE_RCRB_CFG_DECCTL_RXGPEDECEN             BIT20
#define B_PCIE_RCRB_CFG_DECCTL_RXVDMDECE              BIT19
#define B_PCIE_RCRB_CFG_DECCTL_RXMCTPDECEN            BIT18
#define B_PCIE_RCRB_CFG_DECCTL_ICHKINVREQRMSGRBID     BIT17
#define B_PCIE_RCRB_CFG_DECCTL_URRXVMCTPBFRC          BIT16
#define B_PCIE_RCRB_CFG_DECCTL_URRXUORVDM             BIT15
#define B_PCIE_RCRB_CFG_DECCTL_URRXURIDVDM            BIT14
#define B_PCIE_RCRB_CFG_DECCTL_URRXURAVDM             BIT13
#define B_PCIE_RCRB_CFG_DECCTL_URRXULTVDM             BIT12
#define B_PCIE_RCRB_CFG_DECCTL_URRXURTRCVDM           BIT11
#define B_PCIE_RCRB_CFG_DECCTL_URRXUVDMUVID           BIT10
#define B_PCIE_RCRB_CFG_DECCTL_URRXUVDMINTELVID       BIT9
#define B_PCIE_RCRB_CFG_DECCTL_URVDME16DW             BIT8
#define B_PCIE_RCRB_CFG_DECCTL_VDMATAC                BIT7
#define B_PCIE_RCRB_CFG_DECCTL_LCRXINT                BIT6
#define B_PCIE_RCRB_CFG_DECCTL_DROPCPLATNZCE          BIT4
#define B_PCIE_RCRB_CFG_DECCTL_OUTBEXPCPLCHKEN        BIT3
#define B_PCIE_RCRB_CFG_DECCTL_MTRXLTC                BIT2
#define B_PCIE_RCRB_CFG_DECCTL_BUSNUMZCHK             BIT1
#define B_PCIE_RCRB_CFG_DECCTL_ATSCE                  BIT0

#define R_PCIE_RCRB_CFG_PIDECCTL                      0x190C
#define B_PCIE_RCRB_CFG_PIDECCTL_CPLBNCHK             BIT0

#define R_PCIE_RCRB_CFG_PTMPSDC1                      0x1920 ///< SIP17 PTM Pipe Stage Delay Configuration 1
#define R_PCIE_RCRB_CFG_PTMPSDC2                      0x1924 ///< SIP17 PTM Pipe Stage Delay Configuration 2
#define R_PCIE_RCRB_CFG_PTMPSDC3                      0x1928 ///< SIP17 PTM Pipe Stage Delay Configuration 3
#define R_PCIE_RCRB_CFG_PTMPSDC4                      0x192C ///< SIP17 PTM Pipe Stage Delay Configuration 4
#define R_PCIE_RCRB_CFG_PTMPSDC5                      0x1930 ///< SIP17 PTM Pipe Stage Delay Configuration 5
#define R_PCIE_RCRB_CFG_PTMPSDC6                      0x1934 ///< SIP17 PTM Pipe Stage Delay Configuration 6
#define R_PCIE_RCRB_CFG_PTMPSDC7                      0x1938 ///< SIP17 PTM Pipe Stage Delay Configuration 7
#define R_PCIE_RCRB_CFG_PTMPSDC8                      0x193C ///< SIP17 PTM Pipe Stage Delay Configuration 8
#define R_PCIE_RCRB_CFG_PTMPSDC9                      0x1940 ///< SIP17 PTM Pipe Stage Delay Configuration 9
#define R_PCIE_RCRB_CFG_PTMPSDC10                     0x1944 ///< SIP17 PTM Pipe Stage Delay Configuration 10
#define R_PCIE_RCRB_CFG_PTMPSDC11                     0x1948 ///< SIP17 PTM Pipe Stage Delay Configuration 11
#define R_PCIE_RCRB_CFG_PTMPSDC12                     0x194C ///< SIP17 PTM Pipe Stage Delay Configuration 12
#define R_PCIE_RCRB_CFG_PTMPSDC13                     0x1950 ///< SIP17 PTM Pipe Stage Delay Configuration 13
#define R_PCIE_RCRB_CFG_PTMPSDC14                     0x1954 ///< SIP17 PTM Pipe Stage Delay Configuration 14

//
// PCIe Gen5 Equalization, accessible via RCRB
//
#define R_PCIE_CFG_PX32GLTCO1                         0x1E48
#define B_PCIE_CFG_PX32GLTCO1_L1TCOE                  BIT25
#define B_PCIE_CFG_PX32GLTCO1_L0TCOE                  BIT24
#define B_PCIE_CFG_PX32GLTCO1_L1TPOSTCO               0xFC0000
#define N_PCIE_CFG_PX32GLTCO1_L1TPOSTCO               18
#define B_PCIE_CFG_PX32GLTCO1_L1TPRECO                0x3F000
#define N_PCIE_CFG_PX32GLTCO1_L1TPRECO                12
#define B_PCIE_CFG_PX32GLTCO1_L0TPOSTCO               0xFC0
#define N_PCIE_CFG_PX32GLTCO1_L0TPOSTCO               6
#define B_PCIE_CFG_PX32GLTCO1_L0TPRECO                0x3F
#define N_PCIE_CFG_PX32GLTCO1_L0TPRECO                0

#define R_PCIE_CFG_PCIEPMECTL                         0x420
#define B_PCIE_CFG_PCIEPMECTL_FDPPGE                  BIT31
#define B_PCIE_CFG_PCIEPMECTL_FDPPGE_OFFSET           31

#define R_PCIE_CFG_PCIEPMECTL2                        0x424
#define B_PCIE_CFG_PCIEPMECTL2_CPMCSRE                BIT27
#define B_PCIE_CFG_PCIEPMECTL2_CPMCSRE_OFFSET         27
#define B_PCIE_CFG_PCIEPMECTL2_CPGEXH                 BIT14
#define B_PCIE_CFG_PCIEPMECTL2_CPGEXH_OFFSET          14
#define B_PCIE_CFG_PCIEPMECTL2_CPGEXH_MASK            0xC000
#define V_PCIE_CFG_PCIEPMECTL2_CPGEXH                 0x1
#define B_PCIE_CFG_PCIEPMECTL2_CPGENH                 BIT12
#define B_PCIE_CFG_PCIEPMECTL2_CPGENH_OFFSET          12
#define B_PCIE_CFG_PCIEPMECTL2_CPGENH_MASK            0x3000
#define B_PCIE_CFG_PCIEPMECTL2_PHYCLPGE               BIT11
#define B_PCIE_CFG_PCIEPMECTL2_PHYCLPGE_OFFSET        11
#define B_PCIE_CFG_PCIEPMECTL2_L1SPHYDLPGE            BIT9
#define B_PCIE_CFG_PCIEPMECTL2_L1SPHYDLPGE_OFFSET     9
#define B_PCIE_CFG_PCIEPMECTL2_FDCPGE                 BIT8
#define B_PCIE_CFG_PCIEPMECTL2_FDCPGE_OFFSET          8
#define B_PCIE_CFG_PCIEPMECTL2_DETSCPGE               BIT7
#define B_PCIE_CFG_PCIEPMECTL2_DETSCPGE_OFFSET        7
#define B_PCIE_CFG_PCIEPMECTL2_L23RDYSCPGE            BIT6
#define B_PCIE_CFG_PCIEPMECTL2_L23RDYSCPGE_OFFSET     6
#define B_PCIE_CFG_PCIEPMECTL2_DISSCPGE               BIT5
#define B_PCIE_CFG_PCIEPMECTL2_DISSCPGE_OFFSET        5
#define B_PCIE_CFG_PCIEPMECTL2_L1SCPGE                BIT4
#define B_PCIE_CFG_PCIEPMECTL2_L1SCPGE_OFFSET         4

#define R_PCIE_CFG_PCE                                0x428
#define B_PCIE_CFG_PCE_HAE                            BIT5
#define B_PCIE_CFG_PCE_HAE_OFFSET                     5
#define B_PCIE_CFG_PCE_SE                             BIT3
#define B_PCIE_CFG_PCE_SE_OFFSET                      3
#define B_PCIE_CFG_PCE_PMCRE                          BIT0
#define B_PCIE_CFG_PCE_PMCRE_OFFSET                   0

#define R_PCIE_CFG_PGCBCTL1                           0x42C
#define B_PCIE_CFG_PGCBCTL1_TINACCRSTUP_MASK          0x3000
#define B_PCIE_CFG_PGCBCTL1_TACCRSTUP                 BIT10
#define B_PCIE_CFG_PGCBCTL1_TACCRSTUP_OFFSET          10
#define B_PCIE_CFG_PGCBCTL1_TACCRSTUP_MASK            0xC00

#define R_PCIE_CFG_PCIEPMECTL3                        0x434
#define B_PCIE_CFG_PCIEPMECTL3_L1PGAUTOPGEN           BIT4
#define B_PCIE_CFG_PCIEPMECTL3_PMREQCPGEXH            BIT0
#define B_PCIE_CFG_PCIEPMECTL3_PMREQCPGEXH_MASK       0x3
#define B_PCIE_CFG_PCIEPMECTL3_PMREQCPGEXH_OFFSET     0
#define V_PCIE_CFG_PCIEPMECTL3_PMREQCPGEXH            0x0


#define R_PCIE_CFG_PGTHRES                            0x5C0
#define B_PCIE_CFG_PGTHRES_L1PGLTRTLSV                BIT29
#define B_PCIE_CFG_PGTHRES_L1PGLTRTLSV_OFFSET         29
#define B_PCIE_CFG_PGTHRES_L1PGLTRTLSV_MASK           0xE0000000
#define B_PCIE_CFG_PGTHRES_L1PGLTRTLV                 BIT16
#define B_PCIE_CFG_PGTHRES_L1PGLTRTLV_OFFSET          16
#define B_PCIE_CFG_PGTHRES_L1PGLTRTLV_MASK            0x3FF0000
#define B_PCIE_CFG_PGTHRES_L1PGLTREN                  BIT0
#define B_PCIE_CFG_PGTHRES_L1PGLTREN_OFFSET           0

#define R_PCIE_CFG_ACRG3                              0x6CC
#define B_PCIE_CFG_ACRG3_CPGWAKECTRL                  BIT22
#define B_PCIE_CFG_ACRG3_CPGWAKECTRL_OFFSET           22
#define B_PCIE_CFG_ACRG3_CPGWAKECTRL_MASK             0xC00000
#define V_PCIE_CFG_ACRG3_CPGWAKECTRL                  0x2

#define R_PCIE_CFG_RCRB_CFG_PHYPG                     0x1590
#define B_PCIE_CFG_RCRB_CFG_PHYPG_RSVD_MASK           0xFFFFFFF0
#define B_PCIE_CFG_RCRB_CFG_PHYPG_RSVD_OFFSET         4
#define B_PCIE_CFG_RCRB_CFG_PHYPG_RSVD                BIT4
#define V_PCIE_CFG_RCRB_CFG_PHYPG_DUCFGPHYPGE         0x1
#define B_PCIE_CFG_RCRB_CFG_PHYPG_DUCFGPHYPGE         BIT3
#define V_PCIE_CFG_RCRB_CFG_PHYPG_L23PHYPGE           0x1
#define B_PCIE_CFG_RCRB_CFG_PHYPG_L23PHYPGE           BIT2
#define V_PCIE_CFG_RCRB_CFG_PHYPG_DISPHYPGE           0x1
#define B_PCIE_CFG_RCRB_CFG_PHYPG_DISPHYPGE           BIT1
#define V_PCIE_CFG_RCRB_CFG_PHYPG_DETPHYPGE           0x1
#define B_PCIE_CFG_RCRB_CFG_PHYPG_DETPHYPGE           BIT0

#endif
