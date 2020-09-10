/** @file
  Register definition for VER3 PSF component
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
  Copyright 2019 - 2020 Intel Corporation.

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

#ifndef _PSF_REGS_EBG_PCH_H_
#define _PSF_REGS_EBG_PCH_H_

//
// Private chipset regsiter (Memory space) offset definition
// The PCR register defines is used for PCR MMIO programming and PCH SBI programming as well.
//

#define V_VER3_PCH_PSFX_PCR_PSF_MC_AGENT_MCAST_TGT_P2SB    0x38A00
#define V_VER3_PCH_PSFX_PSF_PCR_MC_AGENT_MCAST_TGT_DMI     0x10000

//
// PSFx segment registers
//
#define R_VER3_PCH_PSF_PCR_PORT_CONFIG_PG0_PORT0         0x4020                  ///< PSF Segment Port Configuration Register
#define R_VER3_PCH_PSF_PCR_PORT_CONFIG_PG0_PORT1         0x4024                  ///< PSF Segment Port Configuration Register
#define R_VER3_PCH_PSF_PCR_PORT_CONFIG_PG1_PORT0         0x4028
#define R_VER3_PCH_PSF_PCR_PORT_CONFIG_PG1_PORT1         0x402C

//
// PSF1 PCRs
//
#define R_VER3_PCH_PSF1_PCR_T0_SHDW_IDER_REG_BASE           0x0180          ///< D22F2 PSF base address (CSME: IDER)
#define R_VER3_PCH_PSF1_PCR_T0_SHDW_KT_BASE                 0x0200          ///< D22F3 PSF base address (CSME: KT)
#define R_VER3_PCH_PSF1_PCR_T0_SHDW_HECI1_REG_BASE          0x0280          ///< D22F0 PSF base address (CSME: HECI1)
#define R_VER3_PCH_PSF1_PCR_T0_SHDW_HECI2_REG_BASE          0x0300          ///< D22F1 PSF base address (CSME: HECI2)
#define R_VER3_PCH_PSF1_PCR_T0_SHDW_HECI3_REG_BASE          0x0380          ///< D22F4 PSF base address (CSME: HECI3)
#define R_VER3_PCH_PSF1_PCR_T0_SHDW_HECI4_REG_BASE          0x0400          ///< D22F5 PSF base address (CSME: HECI4)
#define R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE00_RS0_REG_BASE     0x2000          ///< D8F0  PSF base address (PCIE PORT 00)
#define R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE01_RS0_REG_BASE     0x2080          ///< D9F0  PSF base address (PCIE PORT 01)
#define R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE02_RS0_REG_BASE     0x2100          ///< D10F0 PSF base address (PCIE PORT 02)
#define R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE03_RS0_REG_BASE     0x2180          ///< D11F0 PSF base address (PCIE PORT 03)
#define R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE12_RS0_REG_BASE     0x2200          ///< D26F0 PSF base address (PCIE PORT 12)
#define R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE13_RS0_REG_BASE     0x2280          ///< D27F0 PSF base address (PCIE PORT 13)
#define R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE14_RS0_REG_BASE     0x2300          ///< D28F0 PSF base address (PCIE PORT 14)
#define R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE15_RS0_REG_BASE     0x2380          ///< D29F0 PSF base address (PCIE PORT 15)
#define R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE04_RS0_REG_BASE     0x2400          ///< D12F0 PSF base address (PCIE PORT 04)
#define R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE05_RS0_REG_BASE     0x2480          ///< D13F0 PSF base address (PCIE PORT 05)
#define R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE06_RS0_REG_BASE     0x2500          ///< D14F0 PSF base address (PCIE PORT 06)
#define R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE07_RS0_REG_BASE     0x2580          ///< D15F0 PSF base address (PCIE PORT 07)
#define R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE08_RS0_REG_BASE     0x2600          ///< D16F0 PSF base address (PCIE PORT 08)
#define R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE09_RS0_REG_BASE     0x2680          ///< D17F0 PSF base address (PCIE PORT 09)
#define R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE10_RS0_REG_BASE     0x2700          ///< D18F0 PSF base address (PCIE PORT 10)
#define R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE11_RS0_REG_BASE     0x2780          ///< D19F0 PSF base address (PCIE PORT 11)

//
// PSF 1 Multicast Message Configuration
//
#define R_VER3_PCH_PSF1_PCR_MC_CONTROL_MCAST0_EOI           0x403C          ///< Multicast Control Register
#define R_VER3_PCH_PSF1_PCR_MC_AGENT_MCAST0_TGT0_EOI        0x4054          ///< Destination ID
#define R_VER3_PCH_PSF1_PCR_MC_CONTROL_MCAST1_MCTP1         0x4048          ///< Multicast Control Register
#define R_VER3_PCH_PSF1_PCR_MC_AGENT_MCAST1_TGT0_MCTP1      0x40B0          ///< Destination ID

//
// controls the PCI configuration header of a PCI function
//
#define R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPC_RS0_D19_F0     0x4314    ///< SPC
#define R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPC_RS0_D18_F0     0x4318    ///< SPC
#define R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPC_RS0_D17_F0     0x431C    ///< SPC
#define R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPC_RS0_D16_F0     0x4320    ///< SPC
#define R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPB_RS0_D15_F0     0x4324    ///< SPB
#define R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPB_RS0_D14_F0     0x4328    ///< SPB
#define R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPB_RS0_D13_F0     0x432C    ///< SPB
#define R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPB_RS0_D12_F0     0x4330    ///< SPB
#define R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPD_RS0_D29_F0     0x4334    ///< SPD
#define R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPD_RS0_D28_F0     0x4338    ///< SPD
#define R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPD_RS0_D27_F0     0x433C    ///< SPD
#define R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPD_RS0_D26_F0     0x4340    ///< SPD
#define R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPA_RS0_D11_F0     0x4344    ///< SPA
#define R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPA_RS0_D10_F0     0x4348    ///< SPA
#define R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPA_RS0_D9_F0      0x434C    ///< SPA
#define R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPA_RS0_D8_F0      0x4350    ///< SPA

//
// PSF 1 grant count registers
//
#define R_VER3_PCH_PSF1_PCR_DEV_GNTCNT_RELOAD_DGCR0           0x4358
#define R_VER3_PCH_PSF1_PCR_TARGET_GNTCNT_RELOAD_PG0_TGT0     0x44BC

//
// PSF2 PCRs
//
#define R_VER3_PCH_PSF2_PCR_T0_SHDW_NPK_ACPI_REG_BASE       0x0100           ///< D20F7 PSF base address (NPK ACPI)
#define R_VER3_PCH_PSF2_PCR_T0_SHDW_NPK_RS0_REG_BASE        0x0180           ///< D31F7 PSF base address (NPK)
#define R_VER3_PCH_PSF2_PCR_T0_SHDW_NPK_RS1_REG_BASE        0x0200           ///< D31F7 PSF base address (NPK)
#define R_VER3_PCH_PSF2_PCR_T0_SHDW_XHCI_REG_BASE           0x0280           ///< D20F0 PSF base address (XHCI)
#define R_VER3_PCH_PSF2_PCR_T0_SHDW_SATA1_REG_BASE          0x0300           ///< D23F0 PSF base address (SATA 1)
#define R_VER3_PCH_PSF2_PCR_T0_SHDW_SATA2_REG_BASE          0x0380           ///< D24F0 PSF base address (SATA 2)
#define R_VER3_PCH_PSF2_PCR_T0_SHDW_SATA3_REG_BASE          0x0400           ///< D25F0 PSF base address (SATA 3)

//
// PSF 2 grant count registers
//
#define R_VER3_PCH_PSF2_PCR_DEV_GNTCNT_RELOAD_DGCR0         0x4120
#define R_VER3_PCH_PSF2_PCR_TARGET_GNTCNT_RELOAD_PG0_TGT0   0x41F8

//
// PSF3 PCRs
//
#define R_VER3_PCH_PSF3_PCR_T0_SHDW_DMA_SMBUS_REG_BASE     0x0100           ///< D21F0 PSF base address (DMA-SMBus)
#define R_VER3_PCH_PSF3_PCR_T0_SHDW_GBE_RS0_REG_BASE       0x0180           ///< D31F6 PSF base address (GBE)
#define R_VER3_PCH_PSF3_PCR_T0_SHDW_GBE_RS1_REG_BASE       0x0200           ///< D27F0 PSF base address (GBE)
#define R_VER3_PCH_PSF3_PCR_T0_SHDW_HDA_REG_BASE           0x0280           ///< D31F3 PSF base address (HDA)
#define R_VER3_PCH_PSF3_PCR_T0_SHDW_SMBUS_REG_BASE         0x0300           ///< D31F4 PSF base address (SMBUS)
#define R_VER3_PCH_PSF3_PCR_T0_SHDW_UART0_REG_BASE         0x0400           ///< D30F0 PSF base address (LPSS: UART0)
#define R_VER3_PCH_PSF3_PCR_T0_SHDW_UART1_REG_BASE         0x0480           ///< D30F1 PSF base address (LPSS: UART1)
#define R_VER3_PCH_PSF3_PCR_T0_SHDW_P2SB_REG_BASE          0x0580           ///< D31F1 PSF base address (P2SB)
#define R_VER3_PCH_PSF3_PCR_T0_SHDW_PMC_REG_BASE           0x0600           ///< D31F2 PSF base address (PMC)
#define R_VER3_PCH_PSF3_PCR_T0_SHDW_SSRAM_REG_BASE         0x0680           ///< D20F2 PSF base address (SSRAM)
#define R_VER3_PCH_PSF3_PCR_T0_SHDW_SPI_RS0_REG_BASE       0x0780           ///< D31F5 PSF base address (SPI)
#define R_VER3_PCH_PSF3_PCR_T0_SHDW_SPI_RS1_REG_BASE       0x0880           ///< D25F0 PSF base address (SPI)
#define R_VER3_PCH_PSF3_PCR_T0_SHDW_ESPI_RS1_REG_BASE      0x0900           ///< D25F1 PSF base address (eSPI)

// PSF 3 Multicast Message Configuration
//
#define R_VER3_PCH_PSF3_PCR_MC_CONTROL_MCAST0_EOI          0x4050
#define R_VER3_PCH_PSF3_PCR_MC_AGENT_MCAST0_TGT0_EOI       0x405C

//
// PSF 3 grant count registers
//
#define R_VER3_PCH_PSF3_PCR_DEV_GNTCNT_RELOAD_DGCR0         0x41D0
#define R_VER3_PCH_PSF3_PCR_TARGET_GNTCNT_RELOAD_PG0_TGT0   0x42E8

//
// PSF4 PCRs
//
#define R_VER3_PCH_PSF4_PCR_T0_SHDW_PECI_REG_BASE           0x0100           ///< D12F0 PSF base address (PECI)
#define R_VER3_PCH_PSF4_PCR_T0_SHDW_PMT_REG_BASE            0x0180           ///< D14F0 PSF base address (PMT)
#define R_VER3_PCH_PSF4_PCR_T0_SHDW_PTIO_IDER_REG_BASE      0x0100           ///< D13F0 PSF base address (PTIO-IDER)
#define R_VER3_PCH_PSF4_PCR_T0_SHDW_PTIO_KT_REG_BASE        0x0280           ///< D13F1 PSF base address (PTIO-KT)
#define R_VER3_PCH_PSF4_PCR_T0_SHDW_SMT0_REG_BASE           0x0300           ///< D10F0 PSF base address (SMT0)
#define R_VER3_PCH_PSF4_PCR_T0_SHDW_SMT1_REG_BASE           0x0380           ///< D10F1 PSF base address (SMT1)
#define R_VER3_PCH_PSF4_PCR_T0_SHDW_SMT2_REG_BASE           0x0400           ///< D10F2 PSF base address (SMT2)
#define R_VER3_PCH_PSF4_PCR_T0_SHDW_SMT3_REG_BASE           0x0480           ///< D10F3 PSF base address (SMT3)
#define R_VER3_PCH_PSF4_PCR_T0_SHDW_SMT4_REG_BASE           0x0500           ///< D10F4 PSF base address (SMT4)
#define R_VER3_PCH_PSF4_PCR_T0_SHDW_SMT5_REG_BASE           0x0580           ///< D10F5 PSF base address (SMT5)
#define R_VER3_PCH_PSF4_PCR_T0_SHDW_USBR1_REG_BASE          0x0600           ///< D9F0 PSF base address (USBr1)
#define R_VER3_PCH_PSF4_PCR_T0_SHDW_USBR2_REG_BASE          0x0680           ///< D9F1 PSF base address (USBr2)

//
// PSF 4 Multicast Message Configuration
//
#define R_VER3_PCH_PSF4_PCR_MC_CONTROL_MCAST0_MCTP0         0x4040
#define R_VER3_PCH_PSF4_PCR_MC_AGENT_MCAST0_TGT0_MCTP0      0x404C

//
// PSF 4 grant count registers
//
#define R_VER3_PCH_PSF4_PCR_DEV_GNTCNT_RELOAD_DGCR0         0x4138
#define R_VER3_PCH_PSF4_PCR_TARGET_GNTCNT_RELOAD_PG0_TGT0   0x420C
//
// PSF5 PCRs
//

//
// PSF 5 grant count registers
//
#define R_VER3_PCH_PSF5_PCR_DEV_GNTCNT_RELOAD_DGCR0         0x4048
#define R_VER3_PCH_PSF5_PCR_TARGET_GNTCNT_RELOAD_PG0_TGT0   0x40F4

//
// PSF6 PCRs
//

//
// PSF 6 grant count registers
//
#define R_VER3_PCH_PSF6_PCR_DEV_GNTCNT_RELOAD_DGCR0         0x4048
#define R_VER3_PCH_PSF6_PCR_TARGET_GNTCNT_RELOAD_PG0_TGT0   0x40F4

//
// PSF7 PCRs
//

//
// PSF 7 grant count registers
//
#define R_VER3_PCH_PSF7_PCR_DEV_GNTCNT_RELOAD_DGCR0         0x4048
#define R_VER3_PCH_PSF7_PCR_TARGET_GNTCNT_RELOAD_PG0_TGT0   0x40F4

//
// PSF8 PCRs
//
#define R_VER3_PCH_PSF8_PCR_T1_SHDW_PCIE04_RS0_REG_BASE      0x2000           ///< D12F0 PSF base address (PCIE PORT 04)
#define R_VER3_PCH_PSF8_PCR_T1_SHDW_PCIE05_RS0_REG_BASE      0x2080           ///< D13F0 PSF base address (PCIE PORT 05)
#define R_VER3_PCH_PSF8_PCR_T1_SHDW_PCIE06_RS0_REG_BASE      0x2100           ///< D14F0 PSF base address (PCIE PORT 06)
#define R_VER3_PCH_PSF8_PCR_T1_SHDW_PCIE07_RS0_REG_BASE      0x2180           ///< D15F0 PSF base address (PCIE PORT 07)
#define R_VER3_PCH_PSF8_PCR_T1_SHDW_PCIE08_RS0_REG_BASE      0x2200           ///< D16F0 PSF base address (PCIE PORT 08)
#define R_VER3_PCH_PSF8_PCR_T1_SHDW_PCIE09_RS0_REG_BASE      0x2280           ///< D17F0 PSF base address (PCIE PORT 09)
#define R_VER3_PCH_PSF8_PCR_T1_SHDW_PCIE10_RS0_REG_BASE      0x2300           ///< D18F0 PSF base address (PCIE PORT 10)
#define R_VER3_PCH_PSF8_PCR_T1_SHDW_PCIE11_RS0_REG_BASE      0x2380           ///< D19F0 PSF base address (PCIE PORT 11)

//
// PSF 8 Multicast Message Configuration
//
#define R_VER3_PCH_PSF8_PCR_MC_CONTROL_MCAST0_EOI            0x402C
#define R_VER3_PCH_PSF8_PCR_MC_AGENT_MCAST0_TGT0_EOI         0x4044
#define R_VER3_PCH_PSF8_PCR_MC_CONTROL_MCAST1_MCTP1          0x4038
#define R_VER3_PCH_PSF8_PCR_MC_AGENT_MCAST1_TGT0_MCTP1       0x406C
//
// controls the PCI configuration header of a PCI function
//

#define R_VER3_PCH_PSF8_PCR_T1_AGENT_FUNCTION_CONFIG_SPC_RS0_D19_F0     0x4094    ///< SPC
#define R_VER3_PCH_PSF8_PCR_T1_AGENT_FUNCTION_CONFIG_SPC_RS0_D18_F0     0x4098    ///< SPC
#define R_VER3_PCH_PSF8_PCR_T1_AGENT_FUNCTION_CONFIG_SPC_RS0_D17_F0     0x409C    ///< SPC
#define R_VER3_PCH_PSF8_PCR_T1_AGENT_FUNCTION_CONFIG_SPC_RS0_D16_F0     0x40A0    ///< SPC
#define R_VER3_PCH_PSF8_PCR_T1_AGENT_FUNCTION_CONFIG_SPB_RS0_D15_F0     0x40A4    ///< SPB
#define R_VER3_PCH_PSF8_PCR_T1_AGENT_FUNCTION_CONFIG_SPB_RS0_D14_F0     0x40A8    ///< SPB
#define R_VER3_PCH_PSF8_PCR_T1_AGENT_FUNCTION_CONFIG_SPB_RS0_D13_F0     0x40AC    ///< SPB
#define R_VER3_PCH_PSF8_PCR_T1_AGENT_FUNCTION_CONFIG_SPB_RS0_D12_F0     0x40B0    ///< SPB
//
// PSF 8 grant count registers
//
#define R_VER3_PCH_PSF8_PCR_DEV_GNTCNT_RELOAD_DGCR0         0x40B8
#define R_VER3_PCH_PSF8_PCR_TARGET_GNTCNT_RELOAD_PG0_TGT0   0x4154
#define R_VER3_PCH_PSF8_PCR_TARGET_GNTCNT_RELOAD_PG1_TGT0   0x4158
#define V_VER3_PCH_PSFX_PCR_RC_OWNER_PMT                    0x40101

#endif //_PCH_REGS_PSF_VER3_H_
