; @file ProcessorStartupUncore.inc
;
; @copyright
; INTEL CONFIDENTIAL
; Copyright 1999 - 2020 Intel Corporation. <BR>
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

%include "ProcessorUncoreCommon.inc"

%define MSR_SIMICS_CSR_VERSION 0xdeadbeef            ; Simics fake MSR for CSR register header verion
%define BIOS_CSR_HEADER_VERSION 0x1814b              ; Reg Header Date Stamp: 18ww14b

%define BIOS_CSR_HEADER_SKX_VERSION 0x1624b        ; Reg Header Date Stamp: 16ww24b
%define BIOS_CSR_HEADER_CLX_VERSION 0x1815b        ; Reg Header Date Stamp: 18ww15b
%define BIOS_CSR_HEADER_CPX_VERSION 0x1901b        ; Reg Header Date Stamp: 19ww01b

%define BIOS_CSR_HEADER_ICXD_VERSION 0x1828a      ; Reg Header Date Stamp: 18ww28a
%define BIOS_CSR_HEADER_ICX_R0_VERSION 0x1832b     ; Reg Header Date Stamp: 18ww32b
%define BIOS_CSR_HEADER_ICX_L0_VERSION 0x1910b     ; Reg Header Date Stamp: 18ww52e

%define BIOS_CSR_HEADER_SNR_A0_VERSION 0x1821b     ; Reg Header Date Stamp: 18ww21b
%define BIOS_CSR_HEADER_SNR_B0_VERSION 0x1901a     ; Reg Header Date Stamp: 18ww44a
;APTIOV_SERVER_OVERRIDE_RC_START
%define BIOS_CSR_HEADER_VERSION 0x1745e              ; Reg Header Date Stamp: 18ww04b
;APTIOV_SERVER_OVERRIDE_RC_END
;-------------------------------------------------------------------------------------------------------------
; CSR register address in legacy PCI format Bus:Dev:Func:Offset (used via IO port CF8/CFC access method)
;-------------------------------------------------------------------------------------------------------------
;Reg Header Date Stamp: 15ww22a
;----------------------------------------------------------------------------------------------------------------------------
;Bios Name                                        Address    SKX Name                                Bus    Device  Fun     Offset
;----------------------------------------------------------------------------------------------------------------------------
;----------------------------------------------------------------------------------------------------------------------------
;;; SKX-CSR-definition start
;----------------------------------------------------------------------------------------------------------------------------
; //SKX registers
%define CSR_LEGACY_MMIO_TARGET_LIST_1_CBO_SKX 0x8001e9f8 ;CBO_CR_MMIO_TARGET_LIST_CFG_1           1      29      1       0x00f8
%define CSR_LEGACY_SAD_CONTROL_CBO_SKX 0x8001e9f4 ;CBO_CR_SAD_CONTROL_CFG                  1      29      1       0x00f4
%define CSR_LEGACY_SAD_TARGET_CBO_SKX 0x8001e9f0 ;CBO_CR_SAD_TARGET_CFG                   1      29      1       0x00f0
%define CSR_LEGACY_MMCFG_TARGET_LIST_CBO_SKX 0x8001e9ec ;CBO_CR_MMCFG_TARGET_LIST_CFG            1      29      1       0x00ec
%define CSR_LEGACY_MMIO_TARGET_LIST_0_CBO_SKX 0x8001e9e8 ;CBO_CR_MMIO_TARGET_LIST_CFG_0           1      29      1       0x00e8
%define CSR_LEGACY_MMCFG_LOCAL_RULE_CBO_SKX 0x8001e9e4 ;CBO_CR_MMCFG_LOCAL_RULE_CFG             1      29      1       0x00e4
%define CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS1_CBO_SKX 0x8001e9cc ;CBO_CR_MMCFG_LOCAL_RULE_ADDRESS_CFG_1   1      29      1       0x00cc
%define CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS0_CBO_SKX 0x8001e9c8 ;CBO_CR_MMCFG_LOCAL_RULE_ADDRESS_CFG_0   1      29      1       0x00c8
%define CSR_LEGACY_MMCFG_RULE_HI_CBO_SKX 0x8001e9c4 ;CBO_CR_MMCFG_RULE_CFG_N1                1      29      1       0x00c4
%define CSR_LEGACY_MMCFG_RULE_CBO_SKX 0x8001e9c0 ;CBO_CR_MMCFG_RULE_CFG_N0                1      29      1       0x00c0
%define CSR_LEGACY_MMIO_RULE11_CBO_SKX 0x8001e998 ;CBO_CR_MMIO_RULE_CFG_11_N0              1      29      1       0x0098
%define CSR_LEGACY_MMCFG_RULE_UBOX_SKX 0x800042dc ;NCDECS_CR_MMCFG_RULE_CFG                0      8       2       0x00dc
%define CSR_LEGACY_CPUBUSNO_VALID_UBOX_SKX 0x800042d4 ;NCDECS_CR_CPUBUSNO_VALID_CFG            0      8       2       0x00d4
%define CSR_LEGACY_CPUBUSNO1_UBOX_SKX 0x800042d0 ;NCDECS_CR_CPUBUSNO1_CFG                 0      8       2       0x00d0
%define CSR_LEGACY_CPUBUSNO_UBOX_SKX 0x800042cc ;NCDECS_CR_CPUBUSNO_CFG                  0      8       2       0x00cc
%define LEGACY_BIOSSNONSTICKYSCRATCHPAD0_SKX 0x800042c0 ;NCDECS_CR_BIOSNONSTICKYSCRATCHPAD0_CFG  0      8       2       0x00c0
%define LEGACY_BIOSSNONSTICKYSCRATCHPAD13_SKX 0x80004294 ;NCDECS_CR_BIOSNONSTICKYSCRATCHPAD13_CFG 0      8       2       0x0094
%define LEGACY_BIOSSNONSTICKYSCRATCHPAD15_SKX 0x8000429c ;NCDECS_CR_BIOSNONSTICKYSCRATCHPAD15_CFG 0      8       2       0x009c
%define LEGACY_BIOSSCRATCHPAD0_SKX 0x800042a0 ;NCDECS_CR_BIOSSCRATCHPAD0_CFG           0      8       2       0x00a0
%define LEGACY_BIOSSCRATCHPAD2_SKX 0x800042a8 ;NCDECS_CR_BIOSSCRATCHPAD2_CFG           0      8       2       0x00a8
%define LEGACY_BIOSSCRATCHPAD3_SKX 0x800042ac ;NCDECS_CR_BIOSSCRATCHPAD3_CFG           0      8       2       0x00ac
%define LEGACY_BIOSSCRATCHPAD7_SKX 0x800042bc ;NCDECS_CR_BIOSSCRATCHPAD7_CFG           0      8       2       0x00bc
%define CSR_LEGACY_LOCKCONTRO_SKXL 0x800040d0 ;NCEVENTS_CR_LOCKCONTROL_CFG             0      8       0       0x00d0
%define CSR_LEGACY_MMCFG_IIO_SKX 0x80002890 ;IIO_CSTACK_CR_MMCFG_BASE_0_5_0_CFG_N0   0      5       0       0x0090
%define CSR_LEGACY_EMULATION_FLAG_DMI_SKX 0x800000fc ;Virtual CSR on DMI for SW Emulation     0      0       0       0x00fc
;----------------------------------------------------------------------------------------------------------------------------

%define SSR_LEGACY_BUS_CONFIG_INFO_CSR_SKX LEGACY_BIOSSCRATCHPAD3_SKX
%define SSR_LEGACY_BOOT_CONFIG_INFO_CSR_SKX LEGACY_BIOSSCRATCHPAD7_SKX

;-------------------------------------------------------------------------------------------------------------
; CSR register address used by ReadCpuCsr() and WriteCpuCsr()
;  Address format defined as follows
;     [31:28] = 0h
;     [27:20] = Bus # of local target (Bus0,Bus1,...Bus5)
;     [19:15] = Dev #
;     [14:12] = Func #
;     [11:0]  = Reg offset (dword aligned)
;-------------------------------------------------------------------------------------------------------------
; CSR register address in legacy PCIe format Bus:Dev:Func:Offset (used via MMCFG access method)
;----------------------------------------------------------------------------------------------------------------------------
;Bios Name                                        Address    SKX Name                                Bus    Device  Fun     Offset
;----------------------------------------------------------------------------------------------------------------------------
%define CSR_VCU_MAILBOX_DATA_SKX 0x1f8084 ;VCU_CR_CSR_MAILBOX_DATA_CFG             1      31      0       0x0084
%define CSR_VCU_MAILBOX_INTERFACE_SKX 0x1f8080 ;VCU_CR_CSR_MAILBOX_INTERFACE_CFG        1      31      0       0x0080
%define CSR_CPUBUSNO_45_PCU_SKX 0x1f4098 ;PCU_CR_CPUBUSNO1_CFG                    1      30      4       0x0098
%define CSR_CPUBUSNO_03_CPU_SKX 0x1f4094 ;PCU_CR_CPUBUSNO_CFG                     1      30      4       0x0094
%define CSR_GLOBAL_PKG_C_S_CONTROL_PCU_SKX 0x1f20a0 ;PCU_CR_GLOBAL_PKG_C_S_CONTROL_REGISTER  1      30      2       0x00a0
%define CSR_BIOS_RESET_CPL_SKX 0x1f1094 ;PCU_CR_BIOS_RESET_CPL_CFG               1      30      1       0x0094
%define CSR_MMIO_TARGET_LIST_1_CBO_SKX 0x1e90f8 ;CBO_CR_MMIO_TARGET_LIST_CFG_1           1      29      1       0x00f8
%define CSR_SAD_CONTROL_CBO_SKX 0x1e90f4 ;CBO_CR_SAD_CONTROL_CFG                  1      29      1       0x00f4
%define CSR_SAD_TARGET_CBO_SKX 0x1e90f0 ;CBO_CR_SAD_TARGET_CFG                   1      29      1       0x00f0
%define CSR_MMCFG_TARGET_LIST_CBO_SKX 0x1e90ec ;CBO_CR_MMCFG_TARGET_LIST_CFG            1      29      1       0x00ec
%define CSR_MMIO_TARGET_LIST_0_CBO_SKX 0x1e90e8 ;CBO_CR_MMIO_TARGET_LIST_CFG_0           1      29      1       0x00e8
%define CSR_MMCFG_LOCAL_RULE_CBO_SKX 0x1e90e4 ;CBO_CR_MMCFG_LOCAL_RULE_CFG             1      29      1       0x00e4
%define CSR_IOAPIC_TARGET_LIST_3_SKX 0x1e90e0 ;CBO_CR_IOAPIC_TARGET_LIST_CFG_3         1      29      1       0x00e0
%define CSR_IOAPIC_TARGET_LIST_2_SKX 0x1e90dc ;CBO_CR_IOAPIC_TARGET_LIST_CFG_2         1      29      1       0x00dc
%define CSR_IOAPIC_TARGET_LIST_1_SKX 0x1e90d8 ;CBO_CR_IOAPIC_TARGET_LIST_CFG_1         1      29      1       0x00d8
%define CSR_IOAPIC_TARGET_LIST_0_SKX 0x1e90d4 ;CBO_CR_IOAPIC_TARGET_LIST_CFG_0         1      29      1       0x00d4
%define CSR_MMCFG_LOCAL_RULE_ADDRESS1_CBO_SKX 0x1e90cc ;CBO_CR_MMCFG_LOCAL_RULE_ADDRESS_CFG_1   1      29      1       0x00cc
%define CSR_MMCFG_LOCAL_RULE_ADDRESS0_CBO_SKX 0x1e90c8 ;CBO_CR_MMCFG_LOCAL_RULE_ADDRESS_CFG_0   1      29      1       0x00c8
%define CSR_MMCFG_RULE_HI_CBO_SKX 0x1e90c4 ;CBO_CR_MMCFG_RULE_CFG_N1                1      29      1       0x00c4
%define CSR_MMCFG_RULE_CBO_SKX 0x1e90c0 ;CBO_CR_MMCFG_RULE_CFG_N0                1      29      1       0x00c0
%define CSR_MMIO_RULE11_CBO_SKX 0x1e9098 ;CBO_CR_MMIO_RULE_CFG_11_N0              1      29      1       0x0098
%define CSR_IOPORT_TARGET_LIST_0_CBO_SKX 0x1e82b0 ;CBO_CR_IOPORT_TARGET_LIST_CFG_0         1      29      0       0x02b0
%define CSR_MMCFG_RULE_UBOX_SKX 0x420dc ;NCDECS_CR_MMCFG_RULE_CFG                0      8       2       0x00dc
%define CSR_CPUBUSNO_VALID_UBOX_SKX 0x420d4 ;NCDECS_CR_CPUBUSNO_VALID_CFG            0      8       2       0x00d4
%define CSR_CPUBUSNO1_UBOX_SKX 0x420d0 ;NCDECS_CR_CPUBUSNO1_CFG                 0      8       2       0x00d0
%define CSR_CPUBUSNO_UBOX_SKX 0x420cc ;NCDECS_CR_CPUBUSNO_CFG                  0      8       2       0x00cc
%define CSR_SYSTEMSEMAPHORE0_SKX 0x42210 ;NCDECS_CR_SYSTEMSEMAPHORE0_CFG          0      8       2       0x0210
%define SR_PBSP_CHECKIN_CSR_SKX 0x420c8 ;NCDECS_CR_BIOSNONSTICKYSCRATCHPAD2_CFG  0      8       2       0x00c8
%define CSR_BIOSNONSTICKYSCRATCHPAD0_SKX 0x420c0 ;NCDECS_CR_BIOSNONSTICKYSCRATCHPAD0_CFG  0      8       2       0x00c0
%define CSR_BIOSNONSTICKYSCRATCHPAD11_SKX 0x4208c ;NCDECS_CR_BIOSNONSTICKYSCRATCHPAD11_CFG 0      8       2       0x008c
%define CSR_BIOSNONSTICKYSCRATCHPAD13_SKX 0x42094 ;NCDECS_CR_BIOSNONSTICKYSCRATCHPAD13_CFG 0      8       2       0x0094
%define CSR_BIOSNONSTICKYSCRATCHPAD14_SKX 0x42098 ;NCDECS_CR_BIOSNONSTICKYSCRATCHPAD14_CFG 0      8       2       0x0098
%define CSR_BIOSNONSTICKYSCRATCHPAD15_SKX 0x4209c ;NCDECS_CR_BIOSNONSTICKYSCRATCHPAD15_CFG 0      8       2       0x009c
%define CSR_BIOSSCRATCHPAD7_SKX 0x420bc ;NCDECS_CR_BIOSSCRATCHPAD7_CFG           0      8       2       0x00bc
%define CSR_BIOSSCRATCHPAD0_SKX 0x420a0 ;NCDECS_CR_BIOSSCRATCHPAD0_CFG           0      8       2       0x00a0
%define CSR_LOCKCONTROL_SKX 0x400d0 ;NCEVENTS_CR_LOCKCONTROL_CFG             0      8       0       0x00d0
%define CSR_CPUNODEID_UBOX_SKX 0x400c0 ;NCEVENTS_CR_CPUNODEID_CFG               0      8       0       0x00c0
%define CSR_CPUBUSNO_IIO_SKX 0x28108 ;IIO_CSTACK_CR_CPUBUSNO_0_5_0_CFG        0      5       0       0x0108
%define CSR_MMCFG_IIO_SKX 0x28090 ;IIO_CSTACK_CR_MMCFG_BASE_0_5_0_CFG_N0   0      5       0       0x0090
%define CSR_EMULATION_FLAG_DMI_SKX 0xfc ;Virtual CSR on DMI for SW Emulation     0      0       0       0x00fc
;----------------------------------------------------------------------------------------------------------------------------

%define CSR_MMCFG_IIO_BASE_LIMIT_GAP_SKX 8 ;MMCFG_IIO register is QWORD size

%define SR_LEGACY_BUSCFG_SYNC_CSR_SKX LEGACY_BIOSSNONSTICKYSCRATCHPAD13_SKX
%define SR_LEGACY_BUSCFG_DATA_CSR_SKX LEGACY_BIOSSNONSTICKYSCRATCHPAD15_SKX
%define SR_BUSCFG_SYNC_CSR_SKX CSR_BIOSNONSTICKYSCRATCHPAD13_SKX
%define SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_SKX CSR_BIOSNONSTICKYSCRATCHPAD13_SKX
%define SR_MEMORY_DATA_STORAGE_COMMAND_PIPE_CSR_SKX CSR_BIOSNONSTICKYSCRATCHPAD14_SKX
%define SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_SKX CSR_BIOSNONSTICKYSCRATCHPAD15_SKX
;----------------------------------------------------------------------------------------------------------------------------
;;; SKX-CSR-definition ended
;----------------------------------------------------------------------------------------------------------------------------

;----------------------------------------------------------------------------------------------------------------------------
;;; ICX-CSR-definition start
;----------------------------------------------------------------------------------------------------------------------------
; //ICX_TODO TBD
; ICX CSR definition started

%define CSR_LEGACY_CPUBUSNO_VALID_UBOX_ICX BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0xd4 ;NCDECS_CR_CPUBUSNO_VALID_CFG            0  0   2  0x00d4
%define CSR_LEGACY_CPUBUSNO_UBOX_10NMVERSION0 BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0xcc ;NCDECS_CR_CPUBUSNO_CFG                  0  0   2  0x00cc
%define CSR_LEGACY_CPUBUSNO_UBOX_ICX BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0xd0 ;NCDECS_CR_CPUBUSNO_CFG                  0  0   2  0x00d0

%define CSR_LEGACY_MMIO_TARGET_LIST_1_CBO_ICX BIT31 | (BUS_U1 << 16) | (29 << 11) | (1 << 8) | 0x1a4 ;CBO_CR_MMIO_TARGET_LIST_CFG_1          31  29  1  0x01a4
%define CSR_LEGACY_SAD_CONTROL_CBO_ICX       BIT31 | (BUS_U1 << 16) | (29 << 11) | (1 << 8) | 0xf4 ;CBO_CR_SAD_CONTROL_CFG                 31  29  1  0x00f4
%define CSR_LEGACY_SAD_TARGET_CBO_ICX        BIT31 | (BUS_U1 << 16) | (29 << 11) | (1 << 8) | 0xf8 ;CBO_CR_SAD_TARGET_CFG                  31  29  1  0x00f8
%define CSR_LEGACY_MMCFG_TARGET_LIST_CBO_ICX BIT31 | (BUS_U1 << 16) | (29 << 11) | (1 << 8) | 0xec ;CBO_CR_MMCFG_TARGET_LIST_CFG           31  29  1  0x00ec
%define CSR_LEGACY_MMIO_TARGET_LIST_0_CBO_ICX BIT31 | (BUS_U1 << 16) | (29 << 11) | (1 << 8) | 0x1a0 ;CBO_CR_MMIO_TARGET_LIST_CFG_0          31  29  1  0x01a0
%define CSR_LEGACY_MMCFG_LOCAL_RULE_CBO_ICX BIT31 | (BUS_U1 << 16) | (29 << 11) | (1 << 8) | 0xe4 ;CBO_CR_MMCFG_LOCAL_RULE_CFG            31  29  1  0x00e4
%define CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS1_CBO_ICX BIT31 | (BUS_U1 << 16) | (29 << 11) | (1 << 8) | 0xcc ;CBO_CR_MMCFG_LOCAL_RULE_ADDRESS_CFG_1  31  29  1  0x00cc
%define CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS0_CBO_ICX BIT31 | (BUS_U1 << 16) | (29 << 11) | (1 << 8) | 0xc8 ;CBO_CR_MMCFG_LOCAL_RULE_ADDRESS_CFG_0  31  29  1  0x00c8
%define CSR_LEGACY_MMCFG_RULE_HI_CBO_ICX BIT31 | (BUS_U1 << 16) | (29 << 11) | (1 << 8) | 0xc4 ;CBO_CR_MMCFG_RULE_CFG_N1               31  29  1  0x00c4
%define CSR_LEGACY_MMCFG_RULE_CBO_ICX BIT31 | (BUS_U1 << 16) | (29 << 11) | (1 << 8) | 0xc0 ;CBO_CR_MMCFG_RULE_CFG_N0               31  29  1  0x00c0
%define CSR_LEGACY_MMIO_RULE11_CBO_ICX BIT31 | (BUS_U1 << 16) | (29 << 11) | (1 << 8) | 0x160 ;CBO_CR_MMIO_RULE_CFG_11_N0             31  29  1  0x0160
%define CSR_LEGACY_MMIO_RULE13_CBO_ICX BIT31 | (BUS_U1 << 16) | (29 << 11) | (1 << 8) | 0x170 ;CBO_CR_MMIO_RULE_CFG_13_N0             31  29  1  0x0170

%define LEGACY_BIOSSNONSTICKYSCRATCHPAD0_ICX BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0xc0 ;NCDECS_CR_BIOSNONSTICKYSCRATCHPAD0_CFG  30  0  2  0x00c0
%define LEGACY_BIOSSNONSTICKYSCRATCHPAD13_ICX BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0x94 ;NCDECS_CR_BIOSNONSTICKYSCRATCHPAD13_CFG 30  0  2  0x0094
%define LEGACY_BIOSSNONSTICKYSCRATCHPAD15_ICX BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0x9c ;NCDECS_CR_BIOSNONSTICKYSCRATCHPAD15_CFG 30  0  2  0x009c
%define LEGACY_BIOSSCRATCHPAD0_ICX BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0xa0 ;NCDECS_CR_BIOSSCRATCHPAD0_CFG           30  0  2  0x00a0
%define LEGACY_BIOSSCRATCHPAD2_ICX BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0xa8 ;NCDECS_CR_BIOSSCRATCHPAD2_CFG           30  0  2  0x00a8
%define LEGACY_BIOSSCRATCHPAD3_ICX BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0xac ;NCDECS_CR_BIOSSCRATCHPAD3_CFG           30  0  2  0x00ac
%define LEGACY_BIOSSCRATCHPAD7_ICX BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0xbc ;NCDECS_CR_BIOSSCRATCHPAD7_CFG           30  8   2  0x00bc
%define CSR_LEGACY_LOCKCONTROL_ICX BIT31 | (BUS_U0 << 16) | (0  << 11) | (0 << 8) | 0xd0 ;NCEVENTS_CR_LOCKCONTROL_CFG             30  8   0  0x00d0

%define CSR_LEGACY_MMCFG_IIO_ICX BIT31 | (0 << 16) | (0  << 11) | (0 << 8) | 0x90 ;IIO_CSTACK_CR_MMCFG_BASE_0_0_0_CFG_N0   0  0   0  0x0090
%define CSR_LEGACY_EMULATION_FLAG_DMI_ICX BIT31 | (0 << 16) | (3  << 11) | (0 << 8) | 0xfc ;Virtual CSR on DMI for SW Emulation     0  3   0  0x00fc

%ifdef ICX_HOST
%define CSR_BIOSSTICKYLOCKBYPASSSCRATCHPAD4 BIT31 | (BUS_U0 << 16) | (0  << 11) | (0 << 8) | 0x84 ;BIOSSTICKYLOCKBYPASSSCRATCHPAD4_UBOX_CFG_REG 30 0 0 0x84
%elif SKX_HOST
%define CSR_BIOSSTICKYLOCKBYPASSSCRATCHPAD4 BIT31 | (00 << 16) | (8  << 11) | (0 << 8) | 0x84 ;BIOSSTICKYLOCKBYPASSSCRATCHPAD4_UBOX_CFG_REG 30 0 0 0x84
%endif

;----------------------------------------------------------------------------------------------------------------------------
;----------------------------------------------------------------------------------------------------------------------------

%define SSR_LEGACY_BUS_CONFIG_INFO_CSR_ICX LEGACY_BIOSSCRATCHPAD3_ICX
%define SSR_LEGACY_BOOT_CONFIG_INFO_CSR_ICX LEGACY_BIOSSCRATCHPAD7_ICX

;-------------------------------------------------------------------------------------------------------------
; CSR register address used by ReadCpuCsr() and WriteCpuCsr()
;  Address format defined as follows
;     [31:28] = 0h
;     [27:20] = Bus # of local target (Bus0,Bus1,...Bus5)
;     [19:15] = Dev #
;     [14:12] = Func #
;     [11:0]  = Reg offset (dword aligned)
;-------------------------------------------------------------------------------------------------------------
; CSR register address in legacy PCIe format Bus:Dev:Func:Offset (used via MMCFG access method)
;----------------------------------------------------------------------------------------------------------------------------
%define CSR_CPUBUSNO_45_PCU_ICX (BUS_U1 << 20) | (30 << 15) | (4 << 12) | 0x98  ;001f4098h ;PCU_CR_CPUBUSNO1_CFG                    1  30  4  0x0098
%define CSR_CPUBUSNO_03_CPU_ICX (BUS_U1 << 20) | (30 << 15) | (4 << 12) | 0x94  ;001f4094h ;PCU_CR_CPUBUSNO_CFG                     1  30  4  0x0094
%define CSR_GLOBAL_PKG_C_S_CONTROL_PCU_ICX (BUS_U1 << 20) | (30 << 15) | (2 << 12) | 0xa0  ;001f20a0h ;PCU_CR_GLOBAL_PKG_C_S_CONTROL_REGISTER  1  30  2  0x00a0
%define CSR_BIOS_RESET_CPL_ICX (BUS_U1 << 20) | (30 << 15) | (1 << 12) | 0x94  ;001f1094h ;PCU_CR_BIOS_RESET_CPL_CFG               1  30  1  0x0094
%define CSR_MMIO_TARGET_LIST_1_CBO_ICX (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0x1a4  ;001e91a4h ;CBO_CR_MMIO_TARGET_LIST_CFG_1           1  29  1  0x01a4
%define CSR_SAD_CONTROL_CBO_ICX (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xf4  ;001e90f4h ;CBO_CR_SAD_CONTROL_CFG                  1  29  1  0x00f4
%define CSR_SAD_TARGET_CBO_ICX (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xf8  ;001e90f8h ;CBO_CR_SAD_TARGET_CFG                   1  29  1  0x00f8
%define CSR_MMCFG_TARGET_LIST_CBO_ICX (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xec  ;001e90ech ;CBO_CR_MMCFG_TARGET_LIST_CFG            1  29  1  0x00ec
%define CSR_MMIO_TARGET_LIST_0_CBO_ICX (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0x1a0  ;001e91a0h ;CBO_CR_MMIO_TARGET_LIST_CFG_0           1  29  1  0x01a0
%define CSR_MMCFG_LOCAL_RULE_CBO_ICX (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xe4  ;001e90e4h ;CBO_CR_MMCFG_LOCAL_RULE_CFG             1  29  1  0x00e4
%define CSR_IOAPIC_TARGET_LIST_3_ICX (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xdc  ;001e90dch ;CBO_CR_IOAPIC_TARGET_LIST_CFG_3         1  29  1  0x00dc
%define CSR_IOAPIC_TARGET_LIST_2_ICX (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xd8  ;001e90d8h ;CBO_CR_IOAPIC_TARGET_LIST_CFG_2         1  29  1  0x00d8
%define CSR_IOAPIC_TARGET_LIST_1_ICX (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xd4  ;001e90d4h ;CBO_CR_IOAPIC_TARGET_LIST_CFG_1         1  29  1  0x00d4
%define CSR_IOAPIC_TARGET_LIST_0_ICX (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xd0  ;001e90d0h ;CBO_CR_IOAPIC_TARGET_LIST_CFG_0         1  29  1  0x00d0
%define CSR_MMCFG_LOCAL_RULE_ADDRESS1_CBO_ICX (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xcc  ;001e90cch ;CBO_CR_MMCFG_LOCAL_RULE_ADDRESS_CFG_1   1  29  1  0x00cc
%define CSR_MMCFG_LOCAL_RULE_ADDRESS0_CBO_ICX (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xc8  ;001e90c8h ;CBO_CR_MMCFG_LOCAL_RULE_ADDRESS_CFG_0   1  29  1  0x00c8
%define CSR_MMCFG_RULE_HI_CBO_ICX (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xc4  ;001e90c4h ;CBO_CR_MMCFG_RULE_CFG_N1                1  29  1  0x00c4
%define CSR_MMCFG_RULE_CBO_ICX (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xc0  ;001e90c0h ;CBO_CR_MMCFG_RULE_CFG_N0                1  29  1  0x00c0
%define CSR_MMIO_RULE11_CBO_ICX (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0x160  ;001e9160h ;CBO_CR_MMIO_RULE_CFG_11_N0              1  29  1  0x0160
%define CSR_MMIO_RULE13_CBO_ICX (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0x170  ;001e9170h ;CBO_CR_MMIO_RULE_CFG_13_N0              1  29  1  0x0170
%define CSR_IOPORT_TARGET_LIST_0_CBO_ICX (BUS_U1 << 20) | (29 << 15) | (0 << 12) | 0x2C0  ;001e82c0h ;CBO_CR_IOPORT_TARGET_LIST_CFG_0         1  29  0  0x02c0

%define CSR_CPUBUSNO_VALID_UBOX_ICX (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0xd4  ;000420d4h ;NCDECS_CR_CPUBUSNO_VALID_CFG            0  8   2  0x00d4
%define CSR_CPUBUSNO_UBOX_10NMVERSION0 (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0xcc  ;000420cch ;NCDECS_CR_CPUBUSNO_CFG                  0  8   2  0x00cc
%define CSR_CPUBUSNO_UBOX_ICX (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0xd0  ;000420d0h ;NCDECS_CR_CPUBUSNO_CFG                  0  8   2  0x00d0
%define CSR_SYSTEMSEMAPHORE0_ICX (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0x210  ;00042210h ;NCDECS_CR_SYSTEMSEMAPHORE0_CFG          0  8   2  0x0210
%define SR_PBSP_CHECKIN_CSR_10NMVERSION0 (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0x78  ;00042078h ;NCDECS_CR_BIOSNONSTICKYSCRATCHPAD2_CFG  0  8   2  0x0078
%define SR_PBSP_CHECKIN_CSR_ICX (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0xfc  ;000420fch ;NCDECS_CR_BIOSNONSTICKYSCRATCHPAD2_CFG  0  8   2  0x00fc
%define CSR_BIOSNONSTICKYSCRATCHPAD0_ICX (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0xc0  ;000420c0h ;NCDECS_CR_BIOSNONSTICKYSCRATCHPAD0_CFG  0  8   2  0x00c0
%define CSR_BIOSNONSTICKYSCRATCHPAD11_ICX (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0x8c  ;0004208ch ;NCDECS_CR_BIOSNONSTICKYSCRATCHPAD11_CFG 0  8   2  0x008c
%define CSR_BIOSNONSTICKYSCRATCHPAD13_ICX (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0x94  ;00042094h ;NCDECS_CR_BIOSNONSTICKYSCRATCHPAD13_CFG 0  8   2  0x0094
%define CSR_BIOSNONSTICKYSCRATCHPAD14_ICX (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0x98  ;00042098h ;NCDECS_CR_BIOSNONSTICKYSCRATCHPAD14_CFG 0  8   2  0x0098
%define CSR_BIOSNONSTICKYSCRATCHPAD15_ICX (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0x9c  ;0004209ch ;NCDECS_CR_BIOSNONSTICKYSCRATCHPAD15_CFG 0  8   2  0x009c
%define CSR_BIOSSCRATCHPAD7_ICX (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0xbc  ;000420bch ;NCDECS_CR_BIOSSCRATCHPAD7_CFG           0  8   2  0x00bc
%define CSR_BIOSSCRATCHPAD0_ICX (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0xa0  ;000420a0h ;NCDECS_CR_BIOSSCRATCHPAD0_CFG           0  8   2  0x00a0
%define CSR_LOCKCONTROL_ICX (BUS_U0 << 20) | (0  << 15) | (0 << 12) | 0xd0  ;000400d0h ;NCEVENTS_CR_LOCKCONTROL_CFG             0  8   0  0x00d0
%define CSR_CPUNODEID_UBOX_ICX (BUS_U0 << 20) | (0  << 15) | (0 << 12) | 0xc0  ;000400c0h ;NCEVENTS_CR_CPUNODEID_CFG               0  8   0  0x00c0
%define CSR_VID_0_M3KTI_ICX    (BUS_U0 << 20) | (5  << 15) | (0 << 12) | 0x00  ;01e28000h ;VID_0_M3KTI_MAIN_REG                   30  5   0  0x0000

%define CSR_CPUBUSNO_IIO_ICX (0 << 20) | (0  << 15) | (0 << 12) | 0x108  ;00000108h ;IIO_CSTACK_CR_CPUBUSNO_0_0_0_CFG        0  0   0  0x0108
%define CSR_MMCFG_IIO_ICX (0 << 20) | (0  << 15) | (0 << 12) | 0x90  ;00000090h ;IIO_CSTACK_CR_MMCFG_BASE_0_0_0_CFG_N0   0  0   0  0x0090

%define CSR_EMULATION_FLAG_DMI_ICX (0 << 20) | (3  << 15) | (0 << 12) | 0xfc  ;000000fch ;Virtual CSR on DMI for SW Emulation     0  3   0  0x00fc
;----------------------------------------------------------------------------------------------------------------------------

%define CSR_MMCFG_IIO_BASE_LIMIT_GAP_ICX 8 ;MMCFG_IIO register is QWORD size

%define MMIO_RULE11_BASEADDRESS 0xFC000000
%define MMIO_RULE11_LIMITADDRESS 0xFFFFFFFF
%define MMIO_RULE13_BASEADDRESS 0xFC000000
%define MMIO_RULE13_LIMITADDRESS 0xFFFFFFFF
%define SR_LEGACY_BUSCFG_SYNC_CSR_ICX LEGACY_BIOSSNONSTICKYSCRATCHPAD13_ICX
%define SR_LEGACY_BUSCFG_DATA_CSR_ICX LEGACY_BIOSSNONSTICKYSCRATCHPAD15_ICX
%define SR_BUSCFG_SYNC_CSR_ICX CSR_BIOSNONSTICKYSCRATCHPAD13_ICX
%define SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ICX CSR_BIOSNONSTICKYSCRATCHPAD13_ICX
%define SR_MEMORY_DATA_STORAGE_COMMAND_PIPE_CSR_ICX CSR_BIOSNONSTICKYSCRATCHPAD14_ICX
%define SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_ICX CSR_BIOSNONSTICKYSCRATCHPAD15_ICX
;----------------------------------------------------------------------------------------------------------------------------
; ICX-CSR-definition ended
;----------------------------------------------------------------------------------------------------------------------------

;------------------------------------------------------------------------------
; Pre-defined constants
;------------------------------------------------------------------------------
; APTIOV_SERVER_OVERRIDE_RC_START : To resolve Build error
%define DEFAULT_MMCFG_RULE (PCIEX_BASE_ADDRESS + 8)  ; mmcfg.base, with max 256 buses
; APTIOV_SERVER_OVERRIDE_RC_END : To resolve Build error
%define DEFAULT_MMCFG_LOCAL_RULE_SKX 0x3f543210                 ; Local Targets = [5,4,3,2,1,0], Mode[5:0] = 111111b for SKX
%define DEFAULT_MMCFG_LOCAL_RULE_ICX 0x543210                 ; Local Targets = [5,4,3,2,1,0], Mode[5:0] = 000000b for ICX
%define DEFAULT_MMCFG_TARGET_LIST_LOCAL 0x88888888                 ; all cfg requests are confined to the local socket
%define S0_MMCFG_TARGET_LIST_GLOBAL 0x76543210                 ; when legacy socketId = 0    [7,6,5,4,3,2,1,0], 8-socket config
%define S1_MMCFG_TARGET_LIST_GLOBAL 0x76543201                 ; when legacy socketId = 1
%define S2_MMCFG_TARGET_LIST_GLOBAL 0x76543102                 ; when legacy socketId = 2
%define S3_MMCFG_TARGET_LIST_GLOBAL 0x76542103               ; ......
%define S4_MMCFG_TARGET_LIST_GLOBAL 0x76532104
%define S5_MMCFG_TARGET_LIST_GLOBAL 0x76432105
%define S6_MMCFG_TARGET_LIST_GLOBAL 0x75432106
%define S7_MMCFG_TARGET_LIST_GLOBAL 0x65432107

%define DEFAULT_IOAPIC_TARGET_LIST 0x88888888                 ; all cfg requests are confined to the local socket

%define MMCFG_RULE_ADDR_MASK 0xffffc000                ; mask off bit[13:0]

%define MMCFG_RULE_CLR_ENABLE 0xfffffffe                ; mask bit[0]

%define LEGACY_BUS_NO0 0x0                        ; Legacy socket stack0 bus number
%define LEGACY_BUS_NO1 0x11                       ; Legacy socket stack1 bus number
%define LEGACY_BUS_NO2 0x12                       ; Legacy socket stack2 bus number
%define LEGACY_BUS_NO3 0x13                       ; Legacy socket stack3 bus number
%define LEGACY_BUS_NO4 0x14                       ; Legacy socket stack4 bus number
%define LEGACY_BUS_NO5 0x15                       ; Legacy socket stack5 bus number

%define DEFAULT_BUS_NO0 0x0
%define DEFAULT_BUS_NO1 0x1
%define DEFAULT_BUS_NO2 0x2
%define DEFAULT_BUS_NO3 0x3
%define DEFAULT_BUS_NO4 0x4
%define DEFAULT_BUS_NO5 0x5

%define DEFAULT_COLDBOOT_CPUBUSNO_SKX (DEFAULT_BUS_NO3 << 24) + (DEFAULT_BUS_NO2 << 16) + (DEFAULT_BUS_NO1 << 8) + DEFAULT_BUS_NO0
%define DEFAULT_COLDBOOT_CPUBUSNO1_SKX (DEFAULT_BUS_NO5 << 8) + DEFAULT_BUS_NO4
%define DEFAULT_MMCFG_LOCAL_RULE_ADDRESS0 (DEFAULT_BUS_NO2 << 24) + (DEFAULT_BUS_NO1 << 16) + ((DEFAULT_BUS_NO1-1) << 8)+ DEFAULT_BUS_NO0
%define DEFAULT_MMCFG_LOCAL_RULE_ADDRESS1 (DEFAULT_BUS_NO5 << 16) + (DEFAULT_BUS_NO4 << 8) + DEFAULT_BUS_NO3
%define DEFAULT_MMCFG_LOCAL_RULE_ADDRESS1_SNR_ICXD (DEFAULT_BUS_NO4 << 8) + DEFAULT_BUS_NO3

%define LEGACY_COLDBOOT_CPUBUSNO_SKX (LEGACY_BUS_NO3 << 24) + (LEGACY_BUS_NO2 << 16) + (LEGACY_BUS_NO1 << 8) + LEGACY_BUS_NO0
%define LEGACY_COLDBOOT_CPUBUSNO1_SKX (LEGACY_BUS_NO5 << 8) + LEGACY_BUS_NO4
%define LEGACY_MMCFG_LOCAL_RULE_ADDR0 (LEGACY_BUS_NO2 << 24) + (LEGACY_BUS_NO1 << 16) + ((LEGACY_BUS_NO1-1) << 8)+ LEGACY_BUS_NO0
%define LEGACY_MMCFG_LOCAL_RULE_ADDR1 (LEGACY_BUS_NO5 << 16) + (LEGACY_BUS_NO4 << 8) + LEGACY_BUS_NO3
%define LEGACY_MMCFG_LOCAL_RULE_ADDR1_SNR_ICXD (LEGACY_BUS_NO4 << 8) + LEGACY_BUS_NO3

%define DEFAULT_COLDBOOT_CPUBUSNO_ICX (BUS_U1 << 8) + BUS_U0     ; bus#[0x1F,0x1E]

%define BusCfgSync_PBSP_Ready 0xABCDEF01
%define BusCfgSync_PBSP_Go 0xABCDEF02
%define BusCfgSync_PBSP_Done 0xABCDEF03

%define CSR_BIOS_MAILBOX_DATA_PCU_FUN1_REG_ICX (BUS_U1 << 20) | (30 << 15) | (1 << 12) | 0x8C ; 1:30:1:0x8C  BIOS_MAILBOX_DATA_PCU_FUN1_REG
%define CSR_BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG_ICX (BUS_U1 << 20) | (30 << 15) | (1 << 12) | 0x90 ; 1:30:1:0x90  BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG

%define CSR_BIOS_MAILBOX_DATA_PCU_FUN1_REG_SKX (1 << 20) | (30 << 15) | (1 << 12) | 0x8C ; 1:30:1:0x8C  BIOS_MAILBOX_DATA_PCU_FUN1_REG
%define CSR_BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG_SKX (1 << 20) | (30 << 15) | (1 << 12) | 0x90 ; 1:30:1:0x90  BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG
%define BIOS_MAILBOX_COMMAND_SWITCHTO_P1 0x800000af    ;ICX_TODO check later if icx mailbox command value is changed from skx
%define BIOS_MAILBOX_DATA_SWITCHTO_P1 0x3f3f

%define PIPE_DISPATCH_SYNCH_PSYSHOST 4

; VCU Mailbox API related equates
%define VCODE_API_OPCODE_SET_PARAMETER 0x1
%define VCODE_API_SEQ_ID_IOT_LLC_SETUP 0x1000E
%define VCODE_API_OPCODE_OPEN_SEQUENCE 0x3
%define VCODE_API_OPCODE_CLOSE_SEQUENCE 0x4
%define VCODE_API_OPCODE_IOT_LLC_SETUP 0x1007
%define VCODE_API_DELAY_COUNT 0x1000

;------------------------------------------------------------------------------
;        BIOS Usage of Scratchpad LEGACY_BIOSSCRATCHPAD2
;------------------------------------------------------------------------------
;LEGACY_BIOSSCRATCHPAD2             EQU  LEGACY_BIOSSCRATCHPAD0 + 4*2    ; SSR2,  0:8:2:0xA8
    ; [7:0]       - CPU package present bitmap (KTIRC updates, in all sockets)
    ; Other bits  - UPI Link Valid flags (Used by KTIRC)

;------------------------------------------------------------------------------
;        BIOS Usage of Scratchpad LEGACY_BIOSSCRATCHPAD3 (SSR_LEGACY_BUS_CONFIG_INFO_CSR)
;------------------------------------------------------------------------------
;SSR_LEGACY_BUS_CONFIG_INFO_CSR     EQU  LEGACY_BIOSSCRATCHPAD0 + 4*3    ;SSR03,  0:8:2:0xAC

;------------------------------------------------------------------------------0.

;        BIOS Usage of Scratchpad LEGACY_BIOSSCRATCHPAD7 (SSR_LEGACY_BOOT_CONFIG_INFO_CSR)
;------------------------------------------------------------------------------
;SSR_LEGACY_BOOT_CONFIG_INFO_CSR    EQU  LEGACY_BIOSSCRATCHPAD0 + 4*7    ;SSR07, 0:8:2:0xBC
;         Scratch Pad Register 7 Format
;         [31] - AepDimmPresent
;         [30] - Snc recovery
;         [29] - Remote socket released in LT enabled system
;         [28] - FRB error
;         [27] - Fail mem check in fast cold boot
;         [26] - In DWR
;         [25] - Bios stall if enter dwr
;         [24] - DCU mode select
;         [23:6] - unused
;         [5] - Completed Cold Reset Flow
;         [4] - Entered Cold Reset Flow
;         [3:0]  - Unused

;------------------------------------------------------------------------------
;        BIOS Usage of Scratchpad SR_PBSP_CHECKIN_CSR
;------------------------------------------------------------------------------

;SR_PBSP_CHECKIN_CSR  BIOS usage definition
            ; [30:27] = Stepping of Package BSP
            ; [23:8] = APIC ID of Package BSP
            ; [0]    = Socket BSP check-in Flag
            ; All other bits reserved

%ifdef PCH_LBG_ASM
%define PCI_PMC_BASE 0x8000FA00 ; PCH Register B0:D31:RF2
%define PCH_PWRM_BAR 0xFE000000
%define PCIEXPRESS_BASE_ADDRESS 0x80000000
%define PCH_PMC_CMD_PCI_ADDR ((0x1F * 8 + 0x02) * 0x1000 + 0x0004 + PCIEXPRESS_BASE_ADDRESS)
%define PCH_PMC_ACPICNTL_PCI_ADDR ((0x1F * 8 + 0x02) * 0x1000 + 0x0044 + PCIEXPRESS_BASE_ADDRESS)
%endif

%define CAPID1_PCU_FUN3_REG_SKX  0x8001f388 ; CAPID1_PCU_FUN3_REG           1      30      3       0x0088
%define CAPID1_PCU_FUN3_REG_10NM 0x801ff388 ; CAPID1_PCU_FUN3_REG           31     30      3       0x0088
%define CAPID2_PCU_FUN3_REG_10NM 0x801ff38C ; CAPID2_PCU_FUN3_REG           31     30      3       0x008C
%define CAPID4_PCU_FUN3_REG_10NM 0x801ff394 ; CAPID4_PCU_FUN3_REG           31     30      3       0x0094

%define CSR_ROOTBUS_IIO_VTD_REG_ICX (0 << 20) | (0  << 15) | (0 << 12) | 0x108 ; ROOTBUS_IIO_VTD_REG 0 0 0 0x108
%define ROOT_BUSS_VALID BIT15
