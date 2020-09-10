/** @file
  Register names for CDF PCH PMC device

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
  Copyright 2018 Intel Corporation.

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

@par Specification
**/
#ifndef _PCH_REGS_PMC_CDF_H_
#define _PCH_REGS_PMC_CDF_H_

//
// PWRM Registers
//

#define R_PCH_PMC_MSG_PCH_PM_STS                                          0x181C  // PCH Power Management Status
#define B_PCH_PMC_MSG_PCH_PM_STS_ADR_RST_STS                             0x10000  //> ADR Reset Status
#define N_PCH_PMC_MSG_PCH_PM_STS_ADR_RST_STS                                  16

#define R_PCH_PMC_MSG_ADR_EN                                              0x18F0  // ADR Enable
#define S_PCH_PMC_MSG_ADR_EN                                                   4
#define B_PCH_PMC_MSG_ADR_EN_ADR_GPIO_SEL                             0xC0000000  //> ADR GPIO Selection
#define N_PCH_PMC_MSG_ADR_EN_ADR_GPIO_SEL                                     30
#define V_PCH_PMC_MSG_ADR_EN_ADR_GPIO_SEL_GPIO_B                               1  //> ADR GPIO Selection GPIO B
#define V_PCH_PMC_MSG_ADR_EN_ADR_GPIO_SEL_GPIO_C                               2  //> ADR GPIO Selection GPIO C
#define B_PCH_PMC_MSG_ADR_EN_ADR_GPIO_RST_EN                          0x20000000  //> ADR GPIO Reset Enable
#define N_PCH_PMC_MSG_ADR_EN_ADR_GPIO_RST_EN                                  29
#define B_PCH_PMC_MSG_ADR_EN_HPR_ADR_EN                               0x10000000  //> Host Partition Reset ADR Enable
#define N_PCH_PMC_MSG_ADR_EN_HPR_ADR_EN                                       28
#define B_PCH_PMC_MSG_ADR_EN_ADR_GPIO_OUT_EN                           0x8000000  //> ADR GPIO Output Enable
#define N_PCH_PMC_MSG_ADR_EN_ADR_GPIO_OUT_EN                                  27
#define B_PCH_PMC_MSG_ADR_EN_RSVD27_20                                 0x7800000  //> Reserved
#define N_PCH_PMC_MSG_ADR_EN_RSVD27_20                                        23
#define B_PCH_PMC_MSG_ADR_EN_SPARE2_GBL_RST_ADR_EN                      0x400000  //> Spare2 Global Reset ADR Enable
#define N_PCH_PMC_MSG_ADR_EN_SPARE2_GBL_RST_ADR_EN                            22
#define B_PCH_PMC_MSG_ADR_EN_SPARE1_GBL_RST_ADR_EN                      0x200000  //> Spare1 Global Reset ADR Enable
#define N_PCH_PMC_MSG_ADR_EN_SPARE1_GBL_RST_ADR_EN                            21
#define B_PCH_PMC_MSG_ADR_EN_SPARE0_GBL_RST_ADR_EN                      0x100000  //> Spare0 Global Reset ADR Enable
#define N_PCH_PMC_MSG_ADR_EN_SPARE0_GBL_RST_ADR_EN                            20
#define B_PCH_PMC_MSG_ADR_EN_IE_UERR_ADR_EN                              0x80000  //> IE Uncorrectable Error ADR Enable
#define N_PCH_PMC_MSG_ADR_EN_IE_UERR_ADR_EN                                   19
#define B_PCH_PMC_MSG_ADR_EN_IEGBL_ADR_EN                                0x40000  //> IE-Initiated Global Reset ADR Enable
#define N_PCH_PMC_MSG_ADR_EN_IEGBL_ADR_EN                                     18
#define B_PCH_PMC_MSG_ADR_EN_IEWDT_ADR_EN                                0x20000  //> IE FW Watchdog Timer ADR Enable
#define N_PCH_PMC_MSG_ADR_EN_IEWDT_ADR_EN                                     17
#define B_PCH_PMC_MSG_ADR_EN_IEPBO_ADR_EN                                0x10000  //> IE-Initiated Power Button Override ADR Enable
#define N_PCH_PMC_MSG_ADR_EN_IEPBO_ADR_EN                                     16
#define B_PCH_PMC_MSG_ADR_EN_ISH_ADR_EN                                   0x8000  //> ISH ADR Enable
#define N_PCH_PMC_MSG_ADR_EN_ISH_ADR_EN                                       15
#define B_PCH_PMC_MSG_ADR_EN_PMC_PARERR_ADR_EN                            0x4000  //> PMC Parity Error ADR Enable
#define N_PCH_PMC_MSG_ADR_EN_PMC_PARERR_ADR_EN                                14
#define B_PCH_PMC_MSG_ADR_EN_OCWDT_ADR_EN                                 0x2000  //> Over-Clocking WDT ADR Enable
#define N_PCH_PMC_MSG_ADR_EN_OCWDT_ADR_EN                                     13
#define B_PCH_PMC_MSG_ADR_EN_SYSPWR_ADR_EN                                0x1000  //> SYS_PWROK Failure ADR Enable
#define N_PCH_PMC_MSG_ADR_EN_SYSPWR_ADR_EN                                    12
#define B_PCH_PMC_MSG_ADR_EN_ME_UERR_ADR_EN                                0x800  //> ME Uncorrectable Error ADR Enable
#define N_PCH_PMC_MSG_ADR_EN_ME_UERR_ADR_EN                                   11
#define B_PCH_PMC_MSG_ADR_EN_PMCWDT_ADR_EN                                 0x400  //> PMC FW Watchdog Timer ADR Enable
#define N_PCH_PMC_MSG_ADR_EN_PMCWDT_ADR_EN                                    10
#define B_PCH_PMC_MSG_ADR_EN_CTWDT_ADR_EN                                  0x200  //> CPU Thermal Watchdog Timer ADR Enable
#define N_PCH_PMC_MSG_ADR_EN_CTWDT_ADR_EN                                      9
#define B_PCH_PMC_MSG_ADR_EN_MEGBL_ADR_EN                                  0x100  //> ME-Initiated Global Reset ADR Enable
#define N_PCH_PMC_MSG_ADR_EN_MEGBL_ADR_EN                                      8
#define B_PCH_PMC_MSG_ADR_EN_MEWDT_ADR_EN                                   0x80  //> ME FW Watchdog Timer ADR Enable
#define N_PCH_PMC_MSG_ADR_EN_MEWDT_ADR_EN                                      7
#define B_PCH_PMC_MSG_ADR_EN_MEPBO_ADR_EN                                   0x40  //> ME-Initiated Power Button Override ADR Enable
#define N_PCH_PMC_MSG_ADR_EN_MEPBO_ADR_EN                                      6
#define B_PCH_PMC_MSG_ADR_EN_PBO_ADR_EN                                     0x20  //> Power Button Override ADR Enable
#define N_PCH_PMC_MSG_ADR_EN_PBO_ADR_EN                                        5
#define B_PCH_PMC_MSG_ADR_EN_PCHTHRM_ADR_EN                                 0x10  //> PCH Internal Thermal Trip ADR Enable
#define N_PCH_PMC_MSG_ADR_EN_PCHTHRM_ADR_EN                                    4
#define B_PCH_PMC_MSG_ADR_EN_CPUTHRM_ADR_EN                                  0x8  //> CPU Thermal Trip ADR Enable
#define N_PCH_PMC_MSG_ADR_EN_CPUTHRM_ADR_EN                                    3
#define B_PCH_PMC_MSG_ADR_EN_PMCGBL_ADR_EN                                   0x4  //> PMC FW-Initiated Global Reset ADR Enable
#define N_PCH_PMC_MSG_ADR_EN_PMCGBL_ADR_EN                                     2
#define B_PCH_PMC_MSG_ADR_EN_LTRESET_ADR_EN                                  0x2  //> LT RESET ADR Enable
#define N_PCH_PMC_MSG_ADR_EN_LTRESET_ADR_EN                                    1
#define B_PCH_PMC_MSG_ADR_EN_ADR_FEAT_EN                                     0x1  //> ADR Feature Enable
#define N_PCH_PMC_MSG_ADR_EN_ADR_FEAT_EN                                       0

#define R_PCH_PMC_MSG_PM_SYNC_MODE_C0                                     0x18F4  // PM_SYNC Pin Mode in C0
#define R_PCH_PMC_MSG_PM_SYNC_MODE                                        0x18D4  // PM_SYNC Pin Mode
#define V_PCH_PMC_MSG_PM_SYNC_MODE_MESSAGE_NUMBER_GPIO_A                      12
#define V_PCH_PMC_MSG_PM_SYNC_MODE_MESSAGE_NUMBER_GPIO_B                      13
#define V_PCH_PMC_MSG_PM_SYNC_MODE_MESSAGE_NUMBER_GPIO_C                      14
#define V_PCH_PMC_MSG_PM_SYNC_MODE_MESSAGE_NUMBER_GPIO_D                      15

#define R_PCH_PMC_MSG_ADR_GEN_CFG                                         0x1908  // ADR General Configuration Register
#define B_PCH_PMC_MSG_ADR_GEN_CFG_ADR_TIMER_SCALE                          0x700  //> ADR Timer Scale (Tick Frequency)
#define V_PCH_PMC_MSG_ADR_GEN_CFG_ADR_TIMER_SCALE_1US                          0
#define V_PCH_PMC_MSG_ADR_GEN_CFG_ADR_TIMER_SCALE_10US                         1
#define V_PCH_PMC_MSG_ADR_GEN_CFG_ADR_TIMER_SCALE_100US                        2
#define V_PCH_PMC_MSG_ADR_GEN_CFG_ADR_TIMER_SCALE_1MS                          3
#define V_PCH_PMC_MSG_ADR_GEN_CFG_ADR_TIMER_SCALE_10MS                         4
#define V_PCH_PMC_MSG_ADR_GEN_CFG_ADR_TIMER_SCALE_100MS                        5
#define V_PCH_PMC_MSG_ADR_GEN_CFG_ADR_TIMER_SCALE_1S                           6
#define V_PCH_PMC_MSG_ADR_GEN_CFG_ADR_TIMER_SCALE_10S                          7
#define N_PCH_PMC_MSG_ADR_GEN_CFG_ADR_TIMER_SCALE                              8
#define B_PCH_PMC_MSG_ADR_GEN_CFG_ADR_TIMER_TOV                             0xFF  //> ADR Timer Base
#define N_PCH_PMC_MSG_ADR_GEN_CFG_ADR_TIMER_TOV                                0

#define R_PCH_PMC_MSG_BM_CX_CNF                                           0x1BDC  // BM_BREAK_EN and Cx State Configuration Register
#define B_PCH_PMC_MSG_BM_CX_CNF_PM_SYNC_MSG_MODE                           0x200  //> PM_SYNC Serial Message Mode Enable

// TODO: GPIO values should be verified for CDF and Ebg

#define V_CNL_PCH_LP_PMC_PWRM_GPIO_CFG_GPP_A                        0x0
#define V_CNL_PCH_LP_PMC_PWRM_GPIO_CFG_GPP_B                        0x1
#define V_CNL_PCH_LP_PMC_PWRM_GPIO_CFG_GPP_C                        0xD
#define V_CNL_PCH_LP_PMC_PWRM_GPIO_CFG_GPP_D                        0x4
#define V_CNL_PCH_LP_PMC_PWRM_GPIO_CFG_GPP_E                        0xE
#define V_CNL_PCH_LP_PMC_PWRM_GPIO_CFG_GPP_F                        0x5
#define V_CNL_PCH_LP_PMC_PWRM_GPIO_CFG_GPP_G                        0x2
#define V_CNL_PCH_LP_PMC_PWRM_GPIO_CFG_GPP_H                        0x6
#define V_CNL_PCH_LP_PMC_PWRM_GPIO_CFG_GPD                          0xA
#define V_CNL_PCH_LP_PMC_PWRM_GPIO_CFG_VGPIO                        0x7
#define V_CNL_PCH_LP_PMC_PWRM_GPIO_CFG_SPI                          0x3
#define V_CNL_PCH_LP_PMC_PWRM_GPIO_CFG_AZA                          0xB
#define V_CNL_PCH_LP_PMC_PWRM_GPIO_CFG_JTAG                         0xF

#define V_CNL_PCH_H_PMC_PWRM_GPIO_CFG_GPP_A                         0x0
#define V_CNL_PCH_H_PMC_PWRM_GPIO_CFG_GPP_B                         0x1
#define V_CNL_PCH_H_PMC_PWRM_GPIO_CFG_GPP_C                         0x2
#define V_CNL_PCH_H_PMC_PWRM_GPIO_CFG_GPP_D                         0x3
#define V_CNL_PCH_H_PMC_PWRM_GPIO_CFG_GPP_E                         0xA
#define V_CNL_PCH_H_PMC_PWRM_GPIO_CFG_GPP_F                         0xB
#define V_CNL_PCH_H_PMC_PWRM_GPIO_CFG_GPP_G                         0x4
#define V_CNL_PCH_H_PMC_PWRM_GPIO_CFG_GPP_H                         0x9
#define V_CNL_PCH_H_PMC_PWRM_GPIO_CFG_GPP_I                         0xC
#define V_CNL_PCH_H_PMC_PWRM_GPIO_CFG_GPP_J                         0xD
#define V_CNL_PCH_H_PMC_PWRM_GPIO_CFG_GPP_K                         0x8
#define V_CNL_PCH_H_PMC_PWRM_GPIO_CFG_GPD                           0x7
#define V_CNL_PCH_H_PMC_PWRM_GPIO_CFG_VGPIO                         0x5

#define V_PMC_PWRM_EN_SN_SLOW_RING                                  0x0A2D8501
#define V_PMC_PWRM_EN_SN_SA                                         0x0A2D8501
#define V_PMC_PWRM_EN_NS_SA                                         0x00043400
#define V_PMC_PWRM_EN_PA_SLOW_RING                                  0x00180100
#define V_PMC_PWRM_EN_PA_SA                                         0x00180100

#endif // _PCH_REGS_PMC_CDF_H_
