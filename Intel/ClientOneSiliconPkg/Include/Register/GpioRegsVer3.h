/** @file
  Register names for VER3 GPIO
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
  Registers / bits that are specific to PCH-H denoted by "_H_" in register/bit names.
  Registers / bits that are specific to PCH-LP denoted by "_LP_" in register/bit names.
  e.g., "_PCH_", "_PCH_"
  Registers / bits names without _H_ or _LP_ apply for both H and LP.
  Registers / bits that are different between SKUs are denoted by "_[SKU_name]"
  at the end of the register/bit names
  Registers / bits of new devices introduced in a PCH generation will be just named
  as "_PCH_" without [generation_name] inserted.

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

@par Specification Reference:
**/

#ifndef _PCH_REGS_GPIO_VER3_H_
#define _PCH_REGS_GPIO_VER3_H_


//Restricted begin:
//Based on ebg_rtl_1_0
//Restricted end:

#define GPIO_VER3_GPIO_GROUP_MAX  14

//
// GPIO Community 0 Private Configuration Registers
// Group: A
//
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_PAD_OWN                  0x000000A0
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_PADCFGLOCK               0x00000100
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_PADCFGLOCKTX             0x00000104
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_HOSTSW_OWN               0x00000130
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_GPI_IS                   0x00000200
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_GPI_IE                   0x00000210
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_GPI_GPE_STS              0x00000230
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_GPI_GPE_EN               0x00000250
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_SMI_STS                  0x00000270
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_SMI_EN                   0x00000290
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_NMI_STS                  0x000002B0
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_NMI_EN                   0x000002D0
#define GPIO_VER3_GPIO_GPP_A_PAD_MAX                            20

//
// Group: B
//
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_PAD_OWN                  0x000000AC
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_PADCFGLOCK               0x00000108
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_PADCFGLOCKTX             0x0000010C
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_HOSTSW_OWN               0x00000134
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_GPI_IS                   0x00000204
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_GPI_IE                   0x00000214
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_GPI_GPE_STS              0x00000234
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_GPI_GPE_EN               0x00000254
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_SMI_STS                  0x00000274
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_SMI_EN                   0x00000294
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_NMI_STS                  0x000002B4
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_NMI_EN                   0x000002D4
#define GPIO_VER3_GPIO_GPP_B_PAD_MAX                            24

//
// Group: Spi
//
#define R_GPIO_VER3_PCH_GPIO_PCR_SPI_PAD_OWN                    0x000000B8
#define R_GPIO_VER3_PCH_GPIO_PCR_SPI_PADCFGLOCK                 0x00000110
#define R_GPIO_VER3_PCH_GPIO_PCR_SPI_PADCFGLOCKTX               0x00000114
#define R_GPIO_VER3_PCH_GPIO_PCR_SPI_HOSTSW_OWN                 0x00000138
#define R_GPIO_VER3_PCH_GPIO_PCR_SPI_GPI_IS                     0x00000208
#define R_GPIO_VER3_PCH_GPIO_PCR_SPI_GPI_IE                     0x00000218
#define R_GPIO_VER3_PCH_GPIO_PCR_SPI_GPI_GPE_STS                0x00000238
#define R_GPIO_VER3_PCH_GPIO_PCR_SPI_GPI_GPE_EN                 0x00000258
#define R_GPIO_VER3_PCH_GPIO_PCR_SPI_SMI_STS                    0x00000278
#define R_GPIO_VER3_PCH_GPIO_PCR_SPI_SMI_EN                     0x00000298
#define R_GPIO_VER3_PCH_GPIO_PCR_SPI_NMI_STS                    0x000002B8
#define R_GPIO_VER3_PCH_GPIO_PCR_SPI_NMI_EN                     0x000002D8
#define GPIO_VER3_GPIO_SPI_PAD_MAX                              21

//
// GPIO Community 1 Private Configuration Registers
// Group: C
//
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_PAD_OWN                  0x000000A0
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_PADCFGLOCK               0x00000100
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_PADCFGLOCKTX             0x00000104
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_HOSTSW_OWN               0x00000130
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_GPI_IS                   0x00000200
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_GPI_IE                   0x00000210
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_GPI_GPE_STS              0x00000230
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_GPI_GPE_EN               0x00000250
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_SMI_STS                  0x00000270
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_SMI_EN                   0x00000290
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_NMI_STS                  0x000002B0
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_NMI_EN                   0x000002D0
#define GPIO_VER3_GPIO_GPP_C_PAD_MAX                            22

//
// Group: D
//
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_PAD_OWN                  0x000000AC
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_PADCFGLOCK               0x00000108
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_PADCFGLOCKTX             0x0000010C
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_HOSTSW_OWN               0x00000134
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_GPI_IS                   0x00000204
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_GPI_IE                   0x00000214
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_GPI_GPE_STS              0x00000234
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_GPI_GPE_EN               0x00000254
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_SMI_STS                  0x00000274
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_SMI_EN                   0x00000294
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_NMI_STS                  0x000002B4
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_NMI_EN                   0x000002D4
#define GPIO_VER3_GPIO_GPP_D_PAD_MAX                            24

//
// GPIO Community 3 Private Configuration Registers
// Group: E
//
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_PAD_OWN                  0x000000A0
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_PADCFGLOCK               0x00000100
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_PADCFGLOCKTX             0x00000104
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_HOSTSW_OWN               0x00000130
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_GPI_IS                   0x00000200
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_GPI_IE                   0x00000210
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_GPI_GPE_STS              0x00000230
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_GPI_GPE_EN               0x00000250
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_SMI_STS                  0x00000270
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_SMI_EN                   0x00000290
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_NMI_STS                  0x000002B0
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_NMI_EN                   0x000002D0
#define GPIO_VER3_GPIO_GPP_E_PAD_MAX                            24

//
// Group: Jtag
//
#define R_GPIO_VER3_PCH_GPIO_PCR_JTAG_PAD_OWN                   0x000000AC
#define R_GPIO_VER3_PCH_GPIO_PCR_JTAG_PADCFGLOCK                0x00000108
#define R_GPIO_VER3_PCH_GPIO_PCR_JTAG_PADCFGLOCKTX              0x0000010C
#define R_GPIO_VER3_PCH_GPIO_PCR_JTAG_HOSTSW_OWN                0x00000134
#define R_GPIO_VER3_PCH_GPIO_PCR_JTAG_GPI_IS                    0x00000204
#define R_GPIO_VER3_PCH_GPIO_PCR_JTAG_GPI_IE                    0x00000214
#define R_GPIO_VER3_PCH_GPIO_PCR_JTAG_GPI_GPE_STS               0x00000234
#define R_GPIO_VER3_PCH_GPIO_PCR_JTAG_GPI_GPE_EN                0x00000254
#define R_GPIO_VER3_PCH_GPIO_PCR_JTAG_SMI_STS                   0x00000274
#define R_GPIO_VER3_PCH_GPIO_PCR_JTAG_SMI_EN                    0x00000294
#define R_GPIO_VER3_PCH_GPIO_PCR_JTAG_NMI_STS                   0x000002B4
#define R_GPIO_VER3_PCH_GPIO_PCR_JTAG_NMI_EN                    0x000002D4
#define GPIO_VER3_GPIO_JTAG_PAD_MAX                             10

//
// GPIO Community 4 Private Configuration Registers
// Group: H
//
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_PAD_OWN                  0x000000A0
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_PADCFGLOCK               0x00000100
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_PADCFGLOCKTX             0x00000104
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_HOSTSW_OWN               0x00000130
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_GPI_IS                   0x00000200
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_GPI_IE                   0x00000210
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_GPI_GPE_STS              0x00000230
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_GPI_GPE_EN               0x00000250
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_SMI_STS                  0x00000270
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_SMI_EN                   0x00000290
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_NMI_STS                  0x000002B0
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_NMI_EN                   0x000002D0
#define GPIO_VER3_GPIO_GPP_H_PAD_MAX                            20

//
// Group: J
//
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_PAD_OWN                  0x000000AC
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_PADCFGLOCK               0x00000108
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_PADCFGLOCKTX             0x0000010C
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_HOSTSW_OWN               0x00000134
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_GPI_IS                   0x00000204
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_GPI_IE                   0x00000214
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_GPI_GPE_STS              0x00000234
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_GPI_GPE_EN               0x00000254
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_SMI_STS                  0x00000274
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_SMI_EN                   0x00000294
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_NMI_STS                  0x000002B4
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_NMI_EN                   0x000002D4
#define GPIO_VER3_GPIO_GPP_J_PAD_MAX                            18

//
// GPIO Community 5 Private Configuration Registers
// Group: I
//
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_PAD_OWN                  0x000000A0
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_PADCFGLOCK               0x00000100
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_PADCFGLOCKTX             0x00000104
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_HOSTSW_OWN               0x00000130
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_GPI_IS                   0x00000200
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_GPI_IE                   0x00000210
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_GPI_GPE_STS              0x00000230
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_GPI_GPE_EN               0x00000250
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_SMI_STS                  0x00000270
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_SMI_EN                   0x00000290
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_NMI_STS                  0x000002B0
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_NMI_EN                   0x000002D0
#define GPIO_VER3_GPIO_GPP_I_PAD_MAX                            24

//
// Group: L
//
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_PAD_OWN                  0x000000AC
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_PADCFGLOCK               0x00000108
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_PADCFGLOCKTX             0x0000010C
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_HOSTSW_OWN               0x00000134
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_GPI_IS                   0x00000204
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_GPI_IE                   0x00000214
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_GPI_GPE_STS              0x00000234
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_GPI_GPE_EN               0x00000254
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_SMI_STS                  0x00000274
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_SMI_EN                   0x00000294
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_NMI_STS                  0x000002B4
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_NMI_EN                   0x000002D4
#define GPIO_VER3_GPIO_GPP_L_PAD_MAX                            18

//
// Group: M
//
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_PAD_OWN                  0x000000B8
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_PADCFGLOCK               0x00000110
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_PADCFGLOCKTX             0x00000114
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_HOSTSW_OWN               0x00000138
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_GPI_IS                   0x00000208
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_GPI_IE                   0x00000218
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_GPI_GPE_STS              0x00000238
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_GPI_GPE_EN               0x00000258
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_SMI_STS                  0x00000278
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_SMI_EN                   0x00000298
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_NMI_STS                  0x000002B8
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_NMI_EN                   0x000002D8
#define GPIO_VER3_GPIO_GPP_M_PAD_MAX                            18

//
// Group: N
//
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_PAD_OWN                  0x000000C4
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_PADCFGLOCK               0x00000118
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_PADCFGLOCKTX             0x0000011C
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_HOSTSW_OWN               0x0000013C
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_GPI_IS                   0x0000020C
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_GPI_IE                   0x0000021C
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_GPI_GPE_STS              0x0000023C
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_GPI_GPE_EN               0x0000025C
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_SMI_STS                  0x0000027C
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_SMI_EN                   0x0000029C
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_NMI_STS                  0x000002BC
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_NMI_EN                   0x000002DC
#define GPIO_VER3_GPIO_GPP_N_PAD_MAX                           18

//
// Group: O
// GPIO Community 2 Private Configuration Registers
//
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_PAD_OWN                  0x000000A0
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_PADCFGLOCK               0x00000100
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_PADCFGLOCKTX             0x00000104
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_HOSTSW_OWN               0x00000130
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_GPI_IS                   0x00000200
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_GPI_IE                   0x00000210
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_GPI_GPE_STS              0x00000230
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_GPI_GPE_EN               0x00000250
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_SMI_STS                  0x00000270
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_SMI_EN                   0x00000290
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_NMI_STS                  0x000002B0
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_NMI_EN                   0x000002D0
#define GPIO_VER3_GPIO_GPP_O_PAD_MAX                            17

//
// community 0
//
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_A_PADCFG_OFFSET         0x700 // First DW configuration register
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_B_PADCFG_OFFSET         0x850
#define R_GPIO_VER3_PCH_GPIO_PCR_SPI_PADCFG_OFFSET           0x9D0
//
// community 1
//
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_C_PADCFG_OFFSET         0x700
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_D_PADCFG_OFFSET         0x860
//
// community 3
//
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_E_PADCFG_OFFSET         0x700
#define R_GPIO_VER3_PCH_GPIO_PCR_JTAG_PADCFG_OFFSET          0x880
//
// community 4
//
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_H_PADCFG_OFFSET         0x700
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_J_PADCFG_OFFSET         0x840
//
// community 5
//
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_I_PADCFG_OFFSET         0x700
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_L_PADCFG_OFFSET         0x880
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_M_PADCFG_OFFSET         0x9A0
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_N_PADCFG_OFFSET         0xAC0
//
// community 2
//
#define R_GPIO_VER3_PCH_GPIO_PCR_GPP_O_PADCFG_OFFSET         0x700

//
// GPIO Community Common Private Configuration Registers
//
#define V_GPIO_VER3_PCH_GPIO_PCR_MISCCFG_GPE0_GPP_A     0x0
#define V_GPIO_VER3_PCH_GPIO_PCR_MISCCFG_GPE0_GPP_B     0x1
#define V_GPIO_VER3_PCH_GPIO_PCR_MISCCFG_GPE0_GPP_SPI   0xB
#define V_GPIO_VER3_PCH_GPIO_PCR_MISCCFG_GPE0_GPP_C     0x3
#define V_GPIO_VER3_PCH_GPIO_PCR_MISCCFG_GPE0_GPP_D     0x2
#define V_GPIO_VER3_PCH_GPIO_PCR_MISCCFG_GPE0_GPP_E     0x6
#define V_GPIO_VER3_PCH_GPIO_PCR_MISCCFG_GPE0_JTAG      0xA
#define V_GPIO_VER3_PCH_GPIO_PCR_MISCCFG_GPE0_GPP_H     0x5
#define V_GPIO_VER3_PCH_GPIO_PCR_MISCCFG_GPE0_GPP_J     0xF
#define V_GPIO_VER3_PCH_GPIO_PCR_MISCCFG_GPE0_GPP_I     0x7
#define V_GPIO_VER3_PCH_GPIO_PCR_MISCCFG_GPE0_GPP_L     0xE
#define V_GPIO_VER3_PCH_GPIO_PCR_MISCCFG_GPE0_GPP_M     0xC
#define V_GPIO_VER3_PCH_GPIO_PCR_MISCCFG_GPE0_GPP_N     0xD
#define V_GPIO_VER3_PCH_GPIO_PCR_MISCCFG_GPE0_GPP_O     0x9

//
// GPIO Community 0 Capabilities List Private Configuration Registers
//

//
// GSX
//
#define R_GPIO_VER3_PCH_GPIO_GSX_PCR_CAP              0x031C
#define R_GPIO_VER3_PCH_GPIO_GSX_PCR_C0CAP_DW0        0x0320
#define R_GPIO_VER3_PCH_GPIO_GSX_PCR_C0CAP_DW1        0x0324
#define R_GPIO_VER3_PCH_GPIO_GSX_PCR_C0GPILVL_DW0     0x0328
#define R_GPIO_VER3_PCH_GPIO_GSX_PCR_C0GPILVL_DW1     0x032C
#define R_GPIO_VER3_PCH_GPIO_GSX_PCR_C0GPOLVL_DW0     0x0330
#define R_GPIO_VER3_PCH_GPIO_GSX_PCR_C0GPOLVL_DW1     0x0334
#define R_GPIO_VER3_PCH_GPIO_GSX_PCR_C0CMD            0x0338
#define R_GPIO_VER3_PCH_GPIO_GSX_PCR_C0TM             0x033C

#endif
