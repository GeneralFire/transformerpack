; @file ProcessorStartupChipset.inc
;  Chipset constants and macros
;
; @copyright
; INTEL CONFIDENTIAL
; Copyright 1999 - 2017 Intel Corporation. <BR>
;
; The source code contained or described herein and all documents related to the
; source code ("Material") are owned by Intel Corporation or its suppliers or
; licensors. Title to the Material remains with Intel Corporation or its suppliers
; and licensors. The Material may contain trade secrets and proprietary and
; confidential information of Intel Corporation and its suppliers and licensors,
; and is protected by worldwide copyright and trade secret laws and treaty
; provisions. No part of the Material may be used, copied, reproduced, modified,
; published, uploaded, posted, transmitted, distributed, or disclosed in any way
; without Intel's prior express written permission.
;
; No license under any patent, copyright, trade secret or other intellectual
; property right is granted to or conferred upon you by disclosure or delivery
; of the Materials, either expressly, by implication, inducement, estoppel or
; otherwise. Any license under such intellectual property rights must be
; express and approved by Intel in writing.
;
; Unless otherwise agreed by Intel in writing, you may not remove or alter
; this notice or any other notice embedded in Materials by Intel or
; Intel's suppliers or licensors in any way.
;

;
; APIC register
;
%define APICID 0xFEE00020

%define PCH_SPI_BAR 0xFE010000
%define PCH_PWRM_BAR 0xFE000000
%define PCH_TCO_BASE_ADDRESS 0x400

; APTIOV_SERVER_OVERRIDE_RC_START : Added to halt TCO timer
%define ICH_IOREG_TCO1_STS    0x04
%define ICH_IOREG_TCO1_CNT    0x08
%define BIT_TCO_TMR_HLT       0x0800
; APTIOV_SERVER_OVERRIDE_RC_END : Added to halt TCO timer
;
; PCH PCR and ACPI base addresses
;
%define PCH_PCR_BASE 0xFD000000
%define PCH_PCR_BASE_REG (0xF9010 + PCIEXPRESS_BASE_ADDRESS)     ; PCH Register B0:D1F:F1:R10
%define PCH_P2SB_CMD_REG (0xF9004 + PCIEXPRESS_BASE_ADDRESS)     ; PCH Register B0:D1F:F1:R04
%define PCH_ACPI_BASE_ADDRESS 0x500
%define SB_REG_BAR 0xFD000000

;
; PCH RTC configuration registers
;
%define PCH_PCR_RTC_CONF (0xC30000 + 0x3400)
%define PCH_PCR_RTC_CONF_UCMOS_EN 0x4
%define PCH_RTC_CONF_UE_PCI_ADDR 0x4

;
; PCH sideband PCR DMI GCS configuration
;
%define PCH_PCR_DMI_GCS_CONF (0xEF0000 + 0x274C)

%define ACPI_PM1_STS 0x0
%define ACPI_PM1_CNT 0x4

;
; HPET general registers
;
%define PCH_HPET_REG (0xF9060 + PCIEXPRESS_BASE_ADDRESS)     ; PCH Register B0:D1F:F1:R60
%define PCH_HPET_AE 0x80             ; [7] Address Enable
%define HPET_COMP_1 0xFED00108
%define HPET_COMP_2 0xFED0010C
%define HPET_COMP_3 0xFED00128
%define HPET_COMP_4 0xFED00148

;
; PCI registers
;
%define PCIEXPRESS_BASE_ADDRESS 0x80000000
%define SYRE_CPURST 14

;
; PCIEXBAR constants for enable in bit [0]
;
%define ENABLE 1

;
; PCIEXBAR constants for size in bit [2:1]
;
%define PCIEXBAR_64MB 010b
%define PCIEXBAR_128MB 001b
%define PCIEXBAR_256MB 000b

%define PCHBAR_REG (0x48 + PCIEXPRESS_BASE_ADDRESS)
%define PCH_BASE_ADDRESS 0xFED10000

%define PCI_LPC_BASE 0x8000F800
%define PCI_PMC_BASE 0x8000FA00     ; PCH Register B0:D31:RF2
%define PCI_SPI_BASE 0x8000FD00     ; PCH Register B0:D31:RF5

;
; PCH SMBUS base address
;
%define PCH_SMBUS_BASE_ADDRESS 0xEFA0
%define PCH_SMBUS_CMD_REG (0xFC004 + PCIEXPRESS_BASE_ADDRESS)     ; PCH Register B0:D1F:F4:R04
%define PCH_SMBUS_BASE_REG (0xFC020 + PCIEXPRESS_BASE_ADDRESS)     ; PCH Register B0:D1F:F4:R20
%define PCH_SMBUS_HCFG_REG (0xFC040 + PCIEXPRESS_BASE_ADDRESS)     ; PCH Register B0:D1F:F4:R40
%define PCH_SMBUS_TCOBASE_REG (0xFC050 + PCIEXPRESS_BASE_ADDRESS)     ; PCH Register B0:D1F:F4:R50
%define PCH_SMBUS_TCOCTL_REG (0xFC054 + PCIEXPRESS_BASE_ADDRESS)     ; PCH Register B0:D1F:F4:R54

;
; PCI registers
;
%define PCH_HSX_LPC_PMBASE_PCI_ADDR ((0x1F * 8 + 0x0) * 0x1000 + 0x40)
%define PCH_HSX_LPC_ACPICNTL_PCI_ADDR ((0x1F * 8 + 0x0) * 0x1000 + 0x44)
%define PCH_LPC_PMBASE_PCI_ADDR ((0x1F * 8 + 0x0) * 0x1000 + 0x40 + PCIEXPRESS_BASE_ADDRESS)
%define PCH_LPC_ACPICNTL_PCI_ADDR ((0x1F * 8 + 0x0) * 0x1000 + 0x44 + PCIEXPRESS_BASE_ADDRESS)
%define PCH_PMC_PMBASE_PCI_ADDR ((0x1F * 8 + 0x2) * 0x1000 + 0x40 + PCIEXPRESS_BASE_ADDRESS)
%define PCH_PMC_ACPICNTL_PCI_ADDR ((0x1F * 8 + 0x2) * 0x1000 + 0x44 + PCIEXPRESS_BASE_ADDRESS)
%define PCH_LPC_RCRB_PCI_ADDR ((0x1F * 8 + 0x0) * 0x1000 + 0xF0 + PCIEXPRESS_BASE_ADDRESS)

;
; Use PCI write via CF8/CFC access mechanism to write MMCFG_BASE
; as (4GB -512MB) using GQ1_CR_PCIEXBAR
;
%define GQ1_CR_PCIEXBAR 0x80000150       ; D0:F1:R50h

%define BIT1 0x2
%define BIT2 0x4
%define BIT3 0x8
%define BIT4 0x10
%define BIT7 0x80
%define BIT9 0x200
;
;Defines for PCH DEVICE 31
;
%define SB_BUS 0x0
%define SB_PCI2ISA_DEVICE 31
%define SB_PCI2ISA_FUNC 0
%define SB_SMBUS_FUNC 3
;Word Equate for Bus, Device, Function number for I/O Controller Hub
%define SB_PCI2ISA_BUS_DEV_FUNC (SB_BUS SHL 8) + ((SB_PCI2ISA_DEVICE SHL 3)+ SB_PCI2ISA_FUNC)
%define SB_SMBUS_BUS_DEV_FUNC (SB_BUS SHL 8) + ((SB_PCI2ISA_DEVICE SHL 3) + SB_SMBUS_FUNC)

; Define the equates here
%define PCI_LPC_BASE BIT31 + (SB_PCI2ISA_BUS_DEV_FUNC SHL 8)
%define P2I_GEN_PMCON_2_OFFSET 0xA2            ; General PM Configuration 2
  %define CPUPWR_FLR BIT1
%define P2I_GEN_PMCON_3_OFFSET 0xA4            ; General PM Configuration 3
  %define PWR_FLR BIT1
  %define RTC_PWR_STS BIT2
  %define GEN_RST_STS BIT9
%define P2I_PMIR_OFFSET 0xAC            ; Power Management Intitialization Register.

%define PCI_SMBUS_BASE BIT31 + (SB_SMBUS_BUS_DEV_FUNC SHL 8)
%define LPC_IO_KBC_ENB 0x83

;
; IIO registers
;
%define IIO_MISCSS (20 SHL 15) + (0 SHL 12) + 0x9C ; B0:D20:F0:R9Ch
%define IIO_D20F0_DID (20 SHL 15) + (0 SHL 12) + 0x2  ; B0:D20:F0:R02h
;
; Clarksfield IIO Scratch pad register equates
;
%define IIO_SPAD_SR00 (20 SHL 15) + (1 SHL 12) + 0x7C ; B0:D20:F1:R7Ch
%define IIO_SPAD_SR01 (20 SHL 15) + (1 SHL 12) + 0x80 ; B0:D20:F1:R80h
%define IIO_SPAD_SR02 (20 SHL 15) + (1 SHL 12) + 0x84 ; B0:D20:F1:R84h
%define IIO_SPAD_SR03 (20 SHL 15) + (1 SHL 12) + 0x88 ; B0:D20:F1:R88h
%define IIO_SPAD_SR04 (20 SHL 15) + (1 SHL 12) + 0x8C ; B0:D20:F1:R8Ch
%define IIO_SPAD_SR05 (20 SHL 15) + (1 SHL 12) + 0x90 ; B0:D20:F1:R90h
%define IIO_SPAD_SR06 (20 SHL 15) + (1 SHL 12) + 0x94 ; B0:D20:F1:R94h
