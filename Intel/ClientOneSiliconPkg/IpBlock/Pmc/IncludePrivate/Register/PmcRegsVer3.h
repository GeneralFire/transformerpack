/** @file
  Register names for Ver3 PCH PMC device

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
    Register that is specific to -LP denoted by "_PCH_" in component name.
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
  Copyright 2019 Intel Corporation.

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
#ifndef _PMC_REGS_VER3_H_
#define _PMC_REGS_VER3_H_

//
// PWRM Registers
//
#define V_VER3_PCH_PMC_PWRM_GPIO_CFG_GPP_A     0x0
#define V_VER3_PCH_PMC_PWRM_GPIO_CFG_GPP_B     0x1
#define V_VER3_PCH_PMC_PWRM_GPIO_CFG_GPP_SPI   0x2
#define V_VER3_PCH_PMC_PWRM_GPIO_CFG_GPP_C     0x3
#define V_VER3_PCH_PMC_PWRM_GPIO_CFG_GPP_D     0x4
#define V_VER3_PCH_PMC_PWRM_GPIO_CFG_GPP_E     0x5
#define V_VER3_PCH_PMC_PWRM_GPIO_CFG_JTAG      0xF
#define V_VER3_PCH_PMC_PWRM_GPIO_CFG_GPP_H     0x6
#define V_VER3_PCH_PMC_PWRM_GPIO_CFG_GPP_J     0x7
#define V_VER3_PCH_PMC_PWRM_GPIO_CFG_GPP_I     0x8
#define V_VER3_PCH_PMC_PWRM_GPIO_CFG_GPP_L     0x9
#define V_VER3_PCH_PMC_PWRM_GPIO_CFG_GPP_M     0xA
#define V_VER3_PCH_PMC_PWRM_GPIO_CFG_GPP_N     0xB
#define V_VER3_PCH_PMC_PWRM_GPIO_CFG_GPP_O     0xC

#define R_PCH_PMC_MSG_ADR_GEN_CFG                                         0x1908  // ADR General Configuration Register
#define B_PCH_PMC_MSG_ADR_GEN_CFG_ADR_P2_TIMER_SCALE                  0x1C000000  //> ADR Timer Scale
#define N_PCH_PMC_MSG_ADR_GEN_CFG_ADR_P2_TIMER_SCALE                          26
#define B_PCH_PMC_MSG_ADR_GEN_CFG_ADR_P2_EN                              0x80000  //> ADR Phase 2 Enable
#define N_PCH_PMC_MSG_ADR_GEN_CFG_ADR_P2_EN                                   19
#define B_PCH_PMC_MSG_ADR_GEN_CFG_ADR_P2_TIMER_TOV                       0x7F800  //> ADR Timer Base
#define N_PCH_PMC_MSG_ADR_GEN_CFG_ADR_P2_TIMER_TOV                            11
#define B_PCH_PMC_MSG_ADR_GEN_CFG_ADR_P1_TIMER_SCALE                       0x700  //> ADR Timer Scale (Tick Frequency)
#define N_PCH_PMC_MSG_ADR_GEN_CFG_ADR_P1_TIMER_SCALE                           8
#define B_PCH_PMC_MSG_ADR_GEN_CFG_ADR_P1_TIMER_TOV                          0xFF  //> ADR Timer Base
#define N_PCH_PMC_MSG_ADR_GEN_CFG_ADR_P1_TIMER_TOV                             0

#define R_PCH_PMC_MSG_ADR_CTRL_STS                                        0x1960  // PCH Power Management Status
#define B_PCH_PMC_MSG_ADR_CTRL_STS_ADR_RST_STS                           0x10000  //> ADR Reset Status
#define N_PCH_PMC_MSG_ADR_CTRL_STS_ADR_RST_STS                                16

#define R_PCH_PMC_MSG_ADR_GBLRST_EN_0                                     0x191C  // ADR Global Reset Enable 0
#define B_PCH_PMC_MSG_ADR_GBLRST_EN_0_PPBR_PARITY_ERR                 0x20000000  //> ppbr_parity_err Global Reset ADR Enable
#define B_PCH_PMC_MSG_ADR_GBLRST_EN_0_PMC_RF_FUSA_ERR                  0x1000000  //> pmc_rf_fusa_err Global Reset ADR Enable
#define B_PCH_PMC_MSG_ADR_GBLRST_EN_0_PMC_IROM_PARITY                   0x800000  //> pmc_irom_parity Global Reset ADR Enable
#define B_PCH_PMC_MSG_ADR_GBLRST_EN_0_PMC_SRAM_UNC_ERR                  0x400000  //> pmc_sram_unc_err Global Reset ADR Enable
#define B_PCH_PMC_MSG_ADR_GBLRST_EN_0_CSE_HEC_UNC_ERR                   0x200000  //> cse_hec_unc_err Global Reset ADR Enable
#define B_PCH_PMC_MSG_ADR_GBLRST_EN_0_OCWDT_ICC                         0x100000  //> ocwdt_icc Global Reset ADR Enable
#define B_PCH_PMC_MSG_ADR_GBLRST_EN_0_OCWDT_NOICC                        0x80000  //> ocwdt_noicc Global Reset ADR Enable
#define B_PCH_PMC_MSG_ADR_GBLRST_EN_0_ADR_GPIO                           0x40000  //> adr_gpio Global Reset ADR Enable
#define B_PCH_PMC_MSG_ADR_GBLRST_EN_0_ME_UNC_ERR                         0x20000  //> me_unc_err Global Reset ADR Enable
#define B_PCH_PMC_MSG_ADR_GBLRST_EN_0_CPU_THRM_WDT                       0x10000  //> cpu_thrm_wdt Global Reset ADR Enable
#define B_PCH_PMC_MSG_ADR_GBLRST_EN_0_MIA_UX_ERR                          0x8000  //> mia_ux_err Global Reset ADR Enable
#define B_PCH_PMC_MSG_ADR_GBLRST_EN_0_MIA_UXS_ERR                         0x4000  //> mia_uxs_err Global Reset ADR Enable
#define B_PCH_PMC_MSG_ADR_GBLRST_EN_0_SYSPWR_FLR                          0x1000  //> syspwr_flr Global Reset ADR Enable
#define B_PCH_PMC_MSG_ADR_GBLRST_EN_0_PCHPWR_FLR                           0x800  //> pchpwr_flr Global Reset ADR Enable
#define B_PCH_PMC_MSG_ADR_GBLRST_EN_0_PMC_FW                               0x400  //> pmc_fw Global Reset ADR Enable
#define B_PCH_PMC_MSG_ADR_GBLRST_EN_0_ME_WDT                               0x200  //> me_wdt Global Reset ADR Enable
#define B_PCH_PMC_MSG_ADR_GBLRST_EN_0_PMC_WDT                              0x100  //> pmc_wdt Global Reset ADR Enable
#define B_PCH_PMC_MSG_ADR_GBLRST_EN_0_LT_RESET                              0x80  //> lt_reset Global Reset ADR Enable
#define B_PCH_PMC_MSG_ADR_GBLRST_EN_0_MEGBL                                 0x40  //> megbl Global Reset ADR Enable
#define B_PCH_PMC_MSG_ADR_GBLRST_EN_0_CPU_THRM                              0x20  //> cpu_thrm Global Reset ADR Enable
#define B_PCH_PMC_MSG_ADR_GBLRST_EN_0_ME_PBO                                0x10  //> me_pbo Global Reset ADR Enable
#define B_PCH_PMC_MSG_ADR_GBLRST_EN_0_PCH_THRM                               0x8  //> pch_thrm Global Reset ADR Enable
#define B_PCH_PMC_MSG_ADR_GBLRST_EN_0_PMC_UNC_ERR                            0x4  //> pmc_unc_err Global Reset ADR Enable
#define B_PCH_PMC_MSG_ADR_GBLRST_EN_0_PBO                                    0x2  //> pbo Global Reset ADR Enable

#endif // _PMC_REGS_VER3_H_
