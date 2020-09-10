/** @file
  Register names for EBG GPIO
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
  Copyright 2018 - 2019 Intel Corporation. <BR>

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

#ifndef _PCH_REGS_GPIO_EBG_H_
#define _PCH_REGS_GPIO_EBG_H_


//Restricted begin:
//Based on ebg_rtl_05e
//Restricted end:

//Restricted begin:
//TODO Ebg Gpio: Check registers for Ebg (Additional pins for SPI, JTAG)
//Restricted end:
// Group: A
#define R_EBG_PCH_GPIO_PCR_GPP_A_PAD_OWN                  0x000000A0
#define R_EBG_PCH_GPIO_PCR_GPP_A_PADCFGLOCK               0x00000100
#define R_EBG_PCH_GPIO_PCR_GPP_A_PADCFGLOCKTX             0x00000104
#define R_EBG_PCH_GPIO_PCR_GPP_A_HOSTSW_OWN               0x00000130
#define R_EBG_PCH_GPIO_PCR_GPP_A_GPI_IS                   0x00000200
#define R_EBG_PCH_GPIO_PCR_GPP_A_GPI_IE                   0x00000210
#define R_EBG_PCH_GPIO_PCR_GPP_A_GPI_GPE_STS              0x00000230
#define R_EBG_PCH_GPIO_PCR_GPP_A_GPI_GPE_EN               0x00000250
#define R_EBG_PCH_GPIO_PCR_GPP_A_SMI_STS                  0x00000270
#define R_EBG_PCH_GPIO_PCR_GPP_A_SMI_EN                   0x00000290
#define R_EBG_PCH_GPIO_PCR_GPP_A_NMI_STS                  0x000002B0
#define R_EBG_PCH_GPIO_PCR_GPP_A_NMI_EN                   0x000002D0
//Restricted begin:
//TODO: check for Ebg
//Restricted end:
#define V_PCH_GPIO_GPP_A_PAD_MAX 20
// Group: B
#define R_EBG_PCH_GPIO_PCR_GPP_B_PAD_OWN                  0x000000AC
#define R_EBG_PCH_GPIO_PCR_GPP_B_PADCFGLOCK               0x00000108
#define R_EBG_PCH_GPIO_PCR_GPP_B_PADCFGLOCKTX             0x0000010C
#define R_EBG_PCH_GPIO_PCR_GPP_B_HOSTSW_OWN               0x00000134
#define R_EBG_PCH_GPIO_PCR_GPP_B_GPI_IS                   0x00000204
#define R_EBG_PCH_GPIO_PCR_GPP_B_GPI_IE                   0x00000214
#define R_EBG_PCH_GPIO_PCR_GPP_B_GPI_GPE_STS              0x00000234
#define R_EBG_PCH_GPIO_PCR_GPP_B_GPI_GPE_EN               0x00000254
#define R_EBG_PCH_GPIO_PCR_GPP_B_SMI_STS                  0x00000274
#define R_EBG_PCH_GPIO_PCR_GPP_B_SMI_EN                   0x00000294
#define R_EBG_PCH_GPIO_PCR_GPP_B_NMI_STS                  0x000002B4
#define R_EBG_PCH_GPIO_PCR_GPP_B_NMI_EN                   0x000002D4
//Restricted begin:
//TODO: check for Ebg
//Restricted end:
#define V_PCH_GPIO_GPP_B_PAD_MAX 24
// Group: C
#define R_EBG_PCH_GPIO_PCR_GPP_C_PAD_OWN                  0x000000A0
#define R_EBG_PCH_GPIO_PCR_GPP_C_PADCFGLOCK               0x00000100
#define R_EBG_PCH_GPIO_PCR_GPP_C_PADCFGLOCKTX             0x00000104
#define R_EBG_PCH_GPIO_PCR_GPP_C_HOSTSW_OWN               0x00000130
#define R_EBG_PCH_GPIO_PCR_GPP_C_GPI_IS                   0x00000200
#define R_EBG_PCH_GPIO_PCR_GPP_C_GPI_IE                   0x00000210
#define R_EBG_PCH_GPIO_PCR_GPP_C_GPI_GPE_STS              0x00000230
#define R_EBG_PCH_GPIO_PCR_GPP_C_GPI_GPE_EN               0x00000250
#define R_EBG_PCH_GPIO_PCR_GPP_C_SMI_STS                  0x00000270
#define R_EBG_PCH_GPIO_PCR_GPP_C_SMI_EN                   0x00000290
#define R_EBG_PCH_GPIO_PCR_GPP_C_NMI_STS                  0x000002B0
#define R_EBG_PCH_GPIO_PCR_GPP_C_NMI_EN                   0x000002D0
//Restricted begin:
//TODO: check for Ebg
//Restricted end:
#define V_PCH_GPIO_GPP_C_PAD_MAX 22
// Group: D
#define R_EBG_PCH_GPIO_PCR_GPP_D_PAD_OWN                  0x000000AC
#define R_EBG_PCH_GPIO_PCR_GPP_D_PADCFGLOCK               0x00000108
#define R_EBG_PCH_GPIO_PCR_GPP_D_PADCFGLOCKTX             0x0000010C
#define R_EBG_PCH_GPIO_PCR_GPP_D_HOSTSW_OWN               0x00000134
#define R_EBG_PCH_GPIO_PCR_GPP_D_GPI_IS                   0x00000204
#define R_EBG_PCH_GPIO_PCR_GPP_D_GPI_IE                   0x00000214
#define R_EBG_PCH_GPIO_PCR_GPP_D_GPI_GPE_STS              0x00000234
#define R_EBG_PCH_GPIO_PCR_GPP_D_GPI_GPE_EN               0x00000254
#define R_EBG_PCH_GPIO_PCR_GPP_D_SMI_STS                  0x00000274
#define R_EBG_PCH_GPIO_PCR_GPP_D_SMI_EN                   0x00000294
#define R_EBG_PCH_GPIO_PCR_GPP_D_NMI_STS                  0x000002B4
#define R_EBG_PCH_GPIO_PCR_GPP_D_NMI_EN                   0x000002D4
//Restricted begin:
//TODO: check for Ebg
//Restricted end:
#define V_PCH_GPIO_GPP_D_PAD_MAX 24
// Group: E
#define R_EBG_PCH_GPIO_PCR_GPP_E_PAD_OWN                  0x000000A0
#define R_EBG_PCH_GPIO_PCR_GPP_E_PADCFGLOCK               0x00000100
#define R_EBG_PCH_GPIO_PCR_GPP_E_PADCFGLOCKTX             0x00000104
#define R_EBG_PCH_GPIO_PCR_GPP_E_HOSTSW_OWN               0x00000130
#define R_EBG_PCH_GPIO_PCR_GPP_E_GPI_IS                   0x00000200
#define R_EBG_PCH_GPIO_PCR_GPP_E_GPI_IE                   0x00000210
#define R_EBG_PCH_GPIO_PCR_GPP_E_GPI_GPE_STS              0x00000230
#define R_EBG_PCH_GPIO_PCR_GPP_E_GPI_GPE_EN               0x00000250
#define R_EBG_PCH_GPIO_PCR_GPP_E_SMI_STS                  0x00000270
#define R_EBG_PCH_GPIO_PCR_GPP_E_SMI_EN                   0x00000290
#define R_EBG_PCH_GPIO_PCR_GPP_E_NMI_STS                  0x000002B0
#define R_EBG_PCH_GPIO_PCR_GPP_E_NMI_EN                   0x000002D0
//Restricted begin:
//TODO: check for Ebg
//Restricted end:
#define V_PCH_GPIO_GPP_E_PAD_MAX 24
// Group: H
#define R_EBG_PCH_GPIO_PCR_GPP_H_PAD_OWN                  0x000000A0
#define R_EBG_PCH_GPIO_PCR_GPP_H_PADCFGLOCK               0x00000100
#define R_EBG_PCH_GPIO_PCR_GPP_H_PADCFGLOCKTX             0x00000104
#define R_EBG_PCH_GPIO_PCR_GPP_H_HOSTSW_OWN               0x00000130
#define R_EBG_PCH_GPIO_PCR_GPP_H_GPI_IS                   0x00000200
#define R_EBG_PCH_GPIO_PCR_GPP_H_GPI_IE                   0x00000210
#define R_EBG_PCH_GPIO_PCR_GPP_H_GPI_GPE_STS              0x00000230
#define R_EBG_PCH_GPIO_PCR_GPP_H_GPI_GPE_EN               0x00000250
#define R_EBG_PCH_GPIO_PCR_GPP_H_SMI_STS                  0x00000270
#define R_EBG_PCH_GPIO_PCR_GPP_H_SMI_EN                   0x00000290
#define R_EBG_PCH_GPIO_PCR_GPP_H_NMI_STS                  0x000002B0
#define R_EBG_PCH_GPIO_PCR_GPP_H_NMI_EN                   0x000002D0
//Restricted begin:
//TODO: check for Ebg
//Restricted end:
#define V_PCH_GPIO_GPP_H_PAD_MAX 20
// Group: I
#define R_EBG_PCH_GPIO_PCR_GPP_I_PAD_OWN                  0x000000A0
#define R_EBG_PCH_GPIO_PCR_GPP_I_PADCFGLOCK               0x00000100
#define R_EBG_PCH_GPIO_PCR_GPP_I_PADCFGLOCKTX             0x00000104
#define R_EBG_PCH_GPIO_PCR_GPP_I_HOSTSW_OWN               0x00000130
#define R_EBG_PCH_GPIO_PCR_GPP_I_GPI_IS                   0x00000200
#define R_EBG_PCH_GPIO_PCR_GPP_I_GPI_IE                   0x00000210
#define R_EBG_PCH_GPIO_PCR_GPP_I_GPI_GPE_STS              0x00000230
#define R_EBG_PCH_GPIO_PCR_GPP_I_GPI_GPE_EN               0x00000250
#define R_EBG_PCH_GPIO_PCR_GPP_I_SMI_STS                  0x00000270
#define R_EBG_PCH_GPIO_PCR_GPP_I_SMI_EN                   0x00000290
#define R_EBG_PCH_GPIO_PCR_GPP_I_NMI_STS                  0x000002B0
#define R_EBG_PCH_GPIO_PCR_GPP_I_NMI_EN                   0x000002D0
//Restricted begin:
//TODO: check for Ebg
//Restricted end:
#define V_PCH_GPIO_GPP_I_PAD_MAX 24
// Group: J
#define R_EBG_PCH_GPIO_PCR_GPP_J_PAD_OWN                  0x000000AC
#define R_EBG_PCH_GPIO_PCR_GPP_J_PADCFGLOCK               0x00000108
#define R_EBG_PCH_GPIO_PCR_GPP_J_PADCFGLOCKTX             0x0000010C
#define R_EBG_PCH_GPIO_PCR_GPP_J_HOSTSW_OWN               0x00000134
#define R_EBG_PCH_GPIO_PCR_GPP_J_GPI_IS                   0x00000204
#define R_EBG_PCH_GPIO_PCR_GPP_J_GPI_IE                   0x00000214
#define R_EBG_PCH_GPIO_PCR_GPP_J_GPI_GPE_STS              0x00000234
#define R_EBG_PCH_GPIO_PCR_GPP_J_GPI_GPE_EN               0x00000254
#define R_EBG_PCH_GPIO_PCR_GPP_J_SMI_STS                  0x00000274
#define R_EBG_PCH_GPIO_PCR_GPP_J_SMI_EN                   0x00000294
#define R_EBG_PCH_GPIO_PCR_GPP_J_NMI_STS                  0x000002B4
#define R_EBG_PCH_GPIO_PCR_GPP_J_NMI_EN                   0x000002D4
//Restricted begin:
//TODO: check for Ebg
//Restricted end:
#define V_PCH_GPIO_GPP_J_PAD_MAX 18
// Group: L
#define R_EBG_PCH_GPIO_PCR_GPP_L_PAD_OWN                  0x000000AC
#define R_EBG_PCH_GPIO_PCR_GPP_L_PADCFGLOCK               0x00000108
#define R_EBG_PCH_GPIO_PCR_GPP_L_PADCFGLOCKTX             0x0000010C
#define R_EBG_PCH_GPIO_PCR_GPP_L_HOSTSW_OWN               0x00000134
#define R_EBG_PCH_GPIO_PCR_GPP_L_GPI_IS                   0x00000204
#define R_EBG_PCH_GPIO_PCR_GPP_L_GPI_IE                   0x00000214
#define R_EBG_PCH_GPIO_PCR_GPP_L_GPI_GPE_STS              0x00000234
#define R_EBG_PCH_GPIO_PCR_GPP_L_GPI_GPE_EN               0x00000254
#define R_EBG_PCH_GPIO_PCR_GPP_L_SMI_STS                  0x00000274
#define R_EBG_PCH_GPIO_PCR_GPP_L_SMI_EN                   0x00000294
#define R_EBG_PCH_GPIO_PCR_GPP_L_NMI_STS                  0x000002B4
#define R_EBG_PCH_GPIO_PCR_GPP_L_NMI_EN                   0x000002D4
//Restricted begin:
//TODO: check for Ebg
//Restricted end:
#define V_PCH_GPIO_GPP_L_PAD_MAX 18
// Group: M
#define R_EBG_PCH_GPIO_PCR_GPP_M_PAD_OWN                  0x000000B8
#define R_EBG_PCH_GPIO_PCR_GPP_M_PADCFGLOCK               0x00000110
#define R_EBG_PCH_GPIO_PCR_GPP_M_PADCFGLOCKTX             0x00000114
#define R_EBG_PCH_GPIO_PCR_GPP_M_HOSTSW_OWN               0x00000138
#define R_EBG_PCH_GPIO_PCR_GPP_M_GPI_IS                   0x00000208
#define R_EBG_PCH_GPIO_PCR_GPP_M_GPI_IE                   0x00000218
#define R_EBG_PCH_GPIO_PCR_GPP_M_GPI_GPE_STS              0x00000238
#define R_EBG_PCH_GPIO_PCR_GPP_M_GPI_GPE_EN               0x00000258
#define R_EBG_PCH_GPIO_PCR_GPP_M_SMI_STS                  0x00000278
#define R_EBG_PCH_GPIO_PCR_GPP_M_SMI_EN                   0x00000298
#define R_EBG_PCH_GPIO_PCR_GPP_M_NMI_STS                  0x000002B8
#define R_EBG_PCH_GPIO_PCR_GPP_M_NMI_EN                   0x000002D8
//Restricted begin:
//TODO: check for Ebg
//Restricted end:
#define V_PCH_GPIO_GPP_M_PAD_MAX 18
// Group: N
#define R_EBG_PCH_GPIO_PCR_GPP_N_PAD_OWN                  0x000000C4
#define R_EBG_PCH_GPIO_PCR_GPP_N_PADCFGLOCK               0x00000118
#define R_EBG_PCH_GPIO_PCR_GPP_N_PADCFGLOCKTX             0x0000011C
#define R_EBG_PCH_GPIO_PCR_GPP_N_HOSTSW_OWN               0x0000013C
#define R_EBG_PCH_GPIO_PCR_GPP_N_GPI_IS                   0x0000020C
#define R_EBG_PCH_GPIO_PCR_GPP_N_GPI_IE                   0x0000021C
#define R_EBG_PCH_GPIO_PCR_GPP_N_GPI_GPE_STS              0x0000023C
#define R_EBG_PCH_GPIO_PCR_GPP_N_GPI_GPE_EN               0x0000025C
#define R_EBG_PCH_GPIO_PCR_GPP_N_SMI_STS                  0x0000027C
#define R_EBG_PCH_GPIO_PCR_GPP_N_SMI_EN                   0x0000029C
#define R_EBG_PCH_GPIO_PCR_GPP_N_NMI_STS                  0x000002BC
#define R_EBG_PCH_GPIO_PCR_GPP_N_NMI_EN                   0x000002DC
//Restricted begin:
//TODO: check for Ebg
//Restricted end:
#define V_PCH_GPIO_GPP_N_PAD_MAX 18
// Group: O
#define R_EBG_PCH_GPIO_PCR_GPP_O_PAD_OWN                  0x000000A0
#define R_EBG_PCH_GPIO_PCR_GPP_O_PADCFGLOCK               0x00000100
#define R_EBG_PCH_GPIO_PCR_GPP_O_PADCFGLOCKTX             0x00000104
#define R_EBG_PCH_GPIO_PCR_GPP_O_HOSTSW_OWN               0x00000130
#define R_EBG_PCH_GPIO_PCR_GPP_O_GPI_IS                   0x00000200
#define R_EBG_PCH_GPIO_PCR_GPP_O_GPI_IE                   0x00000210
#define R_EBG_PCH_GPIO_PCR_GPP_O_GPI_GPE_STS              0x00000230
#define R_EBG_PCH_GPIO_PCR_GPP_O_GPI_GPE_EN               0x00000250
#define R_EBG_PCH_GPIO_PCR_GPP_O_SMI_STS                  0x00000270
#define R_EBG_PCH_GPIO_PCR_GPP_O_SMI_EN                   0x00000290
#define R_EBG_PCH_GPIO_PCR_GPP_O_NMI_STS                  0x000002B0
#define R_EBG_PCH_GPIO_PCR_GPP_O_NMI_EN                   0x000002D0
#define V_PCH_GPIO_GPP_O_PAD_MAX 17


//Restricted begin:
//TODO: check for Ebg
//Restricted end:
#define S_EBG_PCH_GPIO_PAD_CFG_ADDR_OFFSET             0x10 //Size of the 4 32b DW configuration registers
//Restricted begin:
//TODO: check for Ebg
//Restricted end:
#define V_EBG_PCH_GPIO_PCR_GPP_A_PADCFG_OFFSET         0x700  //First DW configuration register
#define V_EBG_PCH_GPIO_PCR_GPP_B_PADCFG_OFFSET         (V_EBG_PCH_GPIO_PCR_GPP_A_PADCFG_OFFSET + (S_EBG_PCH_GPIO_PAD_CFG_ADDR_OFFSET * V_PCH_GPIO_GPP_A_PAD_MAX))

#define V_EBG_PCH_GPIO_PCR_GPP_C_PADCFG_OFFSET         0x700
#define V_EBG_PCH_GPIO_PCR_GPP_D_PADCFG_OFFSET         (V_EBG_PCH_GPIO_PCR_GPP_C_PADCFG_OFFSET + (S_EBG_PCH_GPIO_PAD_CFG_ADDR_OFFSET * V_PCH_GPIO_GPP_C_PAD_MAX))

#define V_EBG_PCH_GPIO_PCR_GPP_E_PADCFG_OFFSET         0x700
#define V_EBG_PCH_GPIO_PCR_GPP_H_PADCFG_OFFSET         0x700
#define V_EBG_PCH_GPIO_PCR_GPP_J_PADCFG_OFFSET         (V_EBG_PCH_GPIO_PCR_GPP_H_PADCFG_OFFSET + (S_EBG_PCH_GPIO_PAD_CFG_ADDR_OFFSET * V_PCH_GPIO_GPP_H_PAD_MAX))

#define V_EBG_PCH_GPIO_PCR_GPP_I_PADCFG_OFFSET         0x700

#define V_EBG_PCH_GPIO_PCR_GPP_L_PADCFG_OFFSET          (V_EBG_PCH_GPIO_PCR_GPP_I_PADCFG_OFFSET + (S_EBG_PCH_GPIO_PAD_CFG_ADDR_OFFSET * V_PCH_GPIO_GPP_I_PAD_MAX))
#define V_EBG_PCH_GPIO_PCR_GPP_M_PADCFG_OFFSET         0x9A0
#define V_EBG_PCH_GPIO_PCR_GPP_N_PADCFG_OFFSET         0xAC0
#define V_EBG_PCH_GPIO_PCR_GPP_O_PADCFG_OFFSET         0x700
#define V_EBG_PCH_H_GPIO_GROUP_MAX                     15



//
//      GSX
//
// TODO: Update values for Ebg
#define R_EBG_PCH_PCR_GPIO_GSX_CAP                     0x021C
#define R_EBG_PCH_PCR_GPIO_GSX_C0CAP_DW0               0x0220
#define N_PCH_PCR_GPIO_GSX_C0CAP_DW0_NOUT              0x5
#define R_EBG_PCH_PCR_GPIO_GSX_C0CAP_DW1               0x0224
#define R_EBG_PCH_PCR_GPIO_GSX_C0GPILVL_DW0            0x0228
#define R_EBG_PCH_PCR_GPIO_GSX_C0GPILVL_DW1            0x022C
#define R_EBG_PCH_PCR_GPIO_GSX_C0GPOLVL_DW0            0x0230
#define R_EBG_PCH_PCR_GPIO_GSX_C0GPOLVL_DW1            0x0234
#define R_EBG_PCH_PCR_GPIO_GSX_C0CMD                   0x0238
#define B_PCH_GPIO_GSX_C0CMD_BUSY                      BIT1
#define B_PCH_GPIO_GSX_C0CMD_START                     BIT0
#define R_EBG_PCH_PCR_GPIO_GSX_C0TM                    0x023C

#define V_EBG_PCH_H_GSX_GROUP_MAX                      10

//
// PCH-H GPIO registers
//
//
// GPIO Community Common Private Configuration Registers
//
// TODO: Update values for Ebg
#define V_CNL_PCH_H_GPIO_PCR_MISCCFG_GPE0_GPP_A      0x0

#endif
