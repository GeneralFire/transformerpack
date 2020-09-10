/** @file
  Register definition for CNL PSF component
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
  e.g., "_PCH_H_", "_PCH_LP_"
  Registers / bits names without _H_ or _LP_ apply for both H and LP.
  Registers / bits that are different between SKUs are denoted by "_[SKU_name]"
  at the end of the register/bit names
  Registers / bits of new devices introduced in a PCH generation will be just named
  as "_PCH_" without [generation_name] inserted.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2017 Intel Corporation. <BR>

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

#ifndef _PCH_REGS_PSF_CDF_H_
#define _PCH_REGS_PSF_CDF_H_

//
// Private chipset regsiter (Memory space) offset definition
// The PCR register defines is used for PCR MMIO programming and PCH SBI programming as well.
//

//
// PSFx segment registers
//
#define R_CDF_PCH_PSF_PCR_PORT_CONFIG_PG0_PORT0                         0x4028                  ///< PSF Segment Port Configuration Register
#define R_CDF_PCH_PSF_PCR_PORT_CONFIG_PG1_PORT0                         0x402C
#define R_CDF_PCH_PSF_PCR_PORT_CONFIG_PG1_PORT1                         0x4030
#define R_CDF_PCH_PSF_PCR_PORT_CONFIG_PG1_PORT2                         0x4034

#define V_CDF_PCH_PSFX_PCR_PSF_MC_AGENT_MCAST_TGT_P2SB                  0x03B8

// PSF0 Base Address
#define R_CDF_PCH_PSF0_PCR_T1_SHDW_PCIE05_RS0_REG_BASE                  0x2000                  ///< PCH PSF base address (PCIE RP5)
#define R_CDF_PCH_PSF0_PCR_T1_SHDW_PCIE06_RS0_REG_BASE                  0x2100                  ///< PCH PSF base address (PCIE RP6)
#define R_CDF_PCH_PSF0_PCR_T1_SHDW_PCIE07_RS0_REG_BASE                  0x2200                  ///< PCH PSF base address (PCIE RP7)
#define R_CDF_PCH_PSF0_PCR_T1_SHDW_PCIE08_RS0_REG_BASE                  0x2300                  ///< PCH PSF base address (PCIE RP8)
#define R_CDF_PCH_PSF0_PCR_T1_SHDW_PCIE09_RS0_REG_BASE                  0x2400                  ///< PCH PSF base address (PCIE RP9)
#define R_CDF_PCH_PSF0_PCR_T1_SHDW_PCIE10_RS0_REG_BASE                  0x2500                  ///< PCH PSF base address (PCIE RP10)
#define R_CDF_PCH_PSF0_PCR_T1_SHDW_PCIE11_RS0_REG_BASE                  0x2600                  ///< PCH PSF base address (PCIE RP11)
#define R_CDF_PCH_PSF0_PCR_T1_SHDW_PCIE12_RS0_REG_BASE                  0x2700                  ///< PCH PSF base address (PCIE RP12)

#define R_CDF_PCH_PSF0_PCR_T1_SHDW_BUS_NCPM_RS0_REG_BASE                0x2008
#define R_CDF_PCH_PSF0_PCR_T1_SHDW_BUS_NCPM_RS3_REG_BASE                0x2108
#define R_CDF_PCH_PSF0_PCR_T1_SHDW_PCIEN_NCPM_RS0_REG_BASE              0x203C
#define R_CDF_PCH_PSF0_PCR_T1_SHDW_PCIEN_NCPM_RS3_REG_BASE              0x213C
#define R_CDF_PCH_PSF0_PCR_PSF_MC_CONTROL_MCAST0_EOI                    0x4038
#define R_CDF_PCH_PSF0_PCR_PSF_MC_CONTROL_MCAST1_RS0_MCTP1              0x4048
#define R_CDF_PCH_PSF0_PCR_PSF_MC_AGENT_MCAST0_TGT0_EOI                 0x4058
#define R_CDF_PCH_PSF0_PCR_PSF_MC_AGENT_MCAST1_RS0_TGT0_MCTP1           0x4088
#define R_CDF_PCH_PSF0_PCR_DEV_GNTCNT_RELOAD_DGCR0                      0x41D4
#define R_CDF_PCH_PSF0_PCR_TARGET_GNTCNT_RELOAD_PG1_TGT0                0x429C

//PSF 0 Multicast Message Configuration
#define R_PCH_PSF_0_PSF_MC_CONTROL_MCAST1_RS0_MCTP1                     0x4048      ///< Multicast Control Register
#define R_PCH_PSF_0_PSF_MC_AGENT_MCAST1_RS0_TGT0_MCTP1                  0x4088      ///< Destination ID
#define R_PCH_PSF_0_PSF_MC_AGENT_MCAST1_RS0_TGT1_MCTP1                  0x408C      ///< Destination ID
#define R_PCH_PSF_0_PSF_MC_AGENT_MCAST1_RS0_TGT2_MCTP1                  0x4090      ///< Destination ID
#define R_PCH_PSF_0_PSF_MC_AGENT_MCAST1_RS0_TGT3_MCTP1                  0x4094      ///< Destination ID
#define R_PCH_PSF_0_PSF_MC_AGENT_MCAST1_RS0_TGT4_MCTP1                  0x4098      ///< Destination ID
#define R_PCH_PSF_0_PSF_MC_AGENT_MCAST1_RS0_TGT5_MCTP1                  0x409C      ///< Destination ID

// PSF1 Base Address
#define R_CDF_PCH_PSF1_PCR_T0_SHDW_SATA_1_REG_BASE                      0x0300                  ///< PCH PSF base address (SATA 1)
#define R_CDF_PCH_PSF1_PCR_T0_SHDW_SATA_2_REG_BASE                      0x0400                  ///< PCH PSF base address (SATA 2)
#define R_CDF_PCH_PSF1_PCR_T0_SHDW_SATA_3_REG_BASE                      0x0500                  ///< PCH PSF base address (SATA 3)
#define R_CDF_PCH_PSF1_PCR_T1_SHDW_PCIE01_RS0_REG_BASE                  0x2200                  ///< PCH PSF base address (PCIE RP1)
#define R_CDF_PCH_PSF1_PCR_T1_SHDW_PCIE02_RS0_REG_BASE                  0x2300                  ///< PCH PSF base address (PCIE RP2)
#define R_CDF_PCH_PSF1_PCR_T1_SHDW_PCIE03_RS0_REG_BASE                  0x2400                  ///< PCH PSF base address (PCIE RP3)
#define R_CDF_PCH_PSF1_PCR_T1_SHDW_PCIE04_RS0_REG_BASE                  0x2500                  ///< PCH PSF base address (PCIE RP4)
#define R_CDF_PCH_PSF1_PCR_PSF_MC_CONTROL_MCAST0_MCTP0                  0x4040
#define R_CDF_PCH_PSF1_PCR_PSF_MC_AGENT_MCAST0_TGT0_MCTP0               0x4050
#define R_CDF_PCH_PSF1_PCR_DEV_GNTCNT_RELOAD_DGCR0                      0x40F0
#define R_CDF_PCH_PSF1_PCR_TARGET_GNTCNT_RELOAD_PG1_TGT0                0x41A4

//PSF 1 Multicast Message Configuration
#define R_PCH_PSF_1_PSF_MC_CONTROL_MCAST0_RS0_MCTP0                     0x4040      ///< Multicast Control Register
#define R_PCH_PSF_1_PSF_MC_AGENT_MCAST0_RS0_TGT0_MCTP0                  0x4050      ///< Destination ID
#define R_PCH_PSF_1_PSF_MC_AGENT_MCAST0_RS0_TGT1_MCTP0                  0x4054      ///< Destination ID
#define R_PCH_PSF_1_PSF_MC_AGENT_MCAST0_RS0_TGT2_MCTP0                  0x4058      ///< Destination ID
#define R_PCH_PSF_1_PSF_MC_AGENT_MCAST0_RS0_TGT3_MCTP0                  0x405C      ///< Destination ID

// PSF2 Base Address
#define R_CDF_PCH_PSF2_PCR_T0_SHDW_NPK_REG_BASE                         0x0200                  ///< D31F6 PSF base address (NPK)
#define R_CDF_PCH_PSF2_PCR_T0_SHDW_NPK_ACPI_REG_BASE                    0x0100                  ///< D31F7 PSF base address (NPK ACPI)
#define R_CDF_PCH_PSF2_PCR_T0_SHDW_XHCI_REG_BASE                        0x0400                  ///< D30F0 PSF base address (XHCI)
#define R_CDF_PCH_PSF2_PCR_T0_SHDW_ME_HECI1_REG_BASE        0x0500                  ///< D24F0 PSF base address (ME: HECI1)
#define R_CDF_PCH_PSF2_PCR_T0_SHDW_ME_HECI2_REG_BASE        0x0600                  ///< D24F1 PSF base address (ME: HECI2)
#define R_CDF_PCH_PSF2_PCR_T0_SHDW_ME_IDER_REG_BASE         0x0D00                  ///< D24F2 PSF base address (ME: IDER)
#define R_CDF_PCH_PSF2_PCR_T0_SHDW_ME_KT_REG_BASE           0x0E00                  ///< D24F3 PSF base address (ME: KT)
#define R_CDF_PCH_PSF2_PCR_T0_SHDW_ME_HECI3_REG_BASE        0x0700                  ///< D24F4 PSF base address (ME: HECI3)
#define R_CDF_PCH_PSF2_PCR_T0_SHDW_ME_HECI4_REG_BASE        0x0800                  ///< D24F6 PSF base address (ME: HECI4)

#define R_CDF_PCH_PSF2_PCR_T0_SHDW_IE_HECI1_REG_BASE        0x0F00                  ///< D27F0 PSF base address (IE: HECI1)
#define R_CDF_PCH_PSF2_PCR_T0_SHDW_IE_HECI2_REG_BASE        0x1000                  ///< D27F1 PSF base address (IE: HECI2)
#define R_CDF_PCH_PSF2_PCR_T0_SHDW_IE_IDER_REG_BASE         0x1600                  ///< D27F2 PSF base address (IE: IDER)
#define R_CDF_PCH_PSF2_PCR_T0_SHDW_IE_KT_REG_BASE           0x1700                  ///< D27F3 PSF base address (IE: KT)
#define R_CDF_PCH_PSF2_PCR_T0_SHDW_IE_HECI3_REG_BASE        0x1100                  ///< D27F4 PSF base address (IE: HECI3)
#define R_CDF_PCH_PSF2_PCR_T0_SHDW_IE_HECI4_REG_BASE        0x1200                  ///< D27F6 PSF base address (IE: HECI4)

//PSF 2 Multicast Message Configuration
#define R_PCH_PSF_2_PSF_MC_CONTROL_MCAST0_RS0_MCTP0                     0x4044      ///< Multicast Control Register
#define R_PCH_PSF_2_PSF_MC_AGENT_MCAST0_RS0_TGT0_MCTP0                  0x4054      ///< Destination ID
#define R_PCH_PSF_2_PSF_MC_AGENT_MCAST0_RS0_TGT1_MCTP0                  0x4058      ///< Destination ID
#define R_PCH_PSF_2_PSF_MC_AGENT_MCAST0_RS0_TGT2_MCTP0                  0x405C      ///< Destination ID

// PSF3 Base Address
#define R_CDF_PCH_PSF3_PCR_T0_SHDW_EMMC_REG_BASE            0x0100                  ///< D28F0 PSF base address (SCC: eMMC)
#define R_CDF_PCH_PSF3_PCR_T0_SHDW_PMC_REG_BASE             0x0700                  ///< D31F2 PSF base address (PMC)
#define R_CDF_PCH_PSF3_PCR_T0_SHDW_DMA_SMBUS_REG_BASE                   0x0B00                  ///< D15F0 PSF base address (DMA SMBUS)
#define R_CDF_PCH_PSF3_PCR_T0_SHDW_SMBUS_REG_BASE                       0x0C00                  ///< D31F4 PSF base address (Legacy SMBUS)

//PSF 4 Multicast Message Configuration
#define R_PCH_PSF_4_PSF_MC_CONTROL_MCAST0_RS0_MCTP0                     0x4040      ///< Multicast Control Register
#define R_PCH_PSF_4_PSF_MC_AGENT_MCAST0_RS0_TGT0_MCTP0                  0x4050      ///< Destination ID

//PSF 5 Multicast Message Configuration
#define R_PCH_PSF_5_PSF_MC_CONTROL_MCAST0_RS0_MCTP0                     0x4048      ///< Multicast Control Register
#define R_PCH_PSF_5_PSF_MC_AGENT_MCAST0_RS0_TGT0_MCTP0                  0x4058      ///< Destination ID

// PSF9 Base Address
#define R_CDF_PCH_PSF9_PCR_DEV_GNTCNT_RELOAD_DGCR0                      0x4038

// temporary register defines
#define R_CDF_PSF_0_RC_OWNER_RS0                        0x4008
#define R_CDF_PSF_1_RC_OWNER_RS0                        0x4008
#define R_CDF_PSF_5_RC_OWNER_RS0                        0x4008
#define R_CDF_PSF_2_PSF_MC_CONTROL_MCAST0_RS0_MCTP0     0x4044
#define R_CDF_PSF_2_PSF_MC_AGENT_MCAST0_RS0_TGT0_MCTP0  0x4054
#define R_CDF_PSF_2_PSF_MC_AGENT_MCAST0_RS0_TGT1_MCTP0  0x4058
#define R_CDF_PSF_0_PSF_MC_CONTROL_MCAST1_RS0_MCTP1     0x4048
#define R_CDF_PSF_1_PSF_MC_CONTROL_MCAST0_RS0_MCTP0     0x4040
#define R_CDF_PSF_2_PSF_MC_AGENT_MCAST0_RS0_TGT2_MCTP0  0x405C
#define R_CDF_PSF_4_PSF_MC_CONTROL_MCAST0_RS0_MCTP0     0x4040
#define R_CDF_PSF_4_PSF_MC_AGENT_MCAST0_RS0_TGT0_MCTP0  0x4050
#define R_CDF_PSF_5_PSF_MC_CONTROL_MCAST0_RS0_MCTP0     0x4048
#define R_CDF_PSF_5_PSF_MC_AGENT_MCAST0_RS0_TGT0_MCTP0  0x4058

#endif

