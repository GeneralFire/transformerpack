;
; This file contains an 'Intel Pre-EFI Module' and is licensed
; for Intel CPUs and Chipsets under the terms of your license
; agreement with Intel or your vendor.  This file may be
; modified by the user, subject to additional terms of the
; license agreement
;
;------------------------------------------------------------------------------
;
; Copyright (c) 1999 - 2020, Intel Corporation. All rights reserved.<BR>
; This software and associated documentation (if any) is furnished
; under a license and may only be used or copied in accordance
; with the terms of the license. Except as permitted by such
; license, no part of this software or documentation may be
; reproduced, stored in a retrieval system, or transmitted in any
; form or by any means without the express written consent of
; Intel Corporation.
;
; Module Name:
;
;  ProcessorStartupUncoreSpr.inc
;
; Abstract:
;
;
;------------------------------------------------------------------------------
;
; This is an assembly include file and is
; licensed for Intel CPUs and chipsets under the terms of your
; license agreement with Intel or your vendor.  This file may
; be modified by the user, subject to additional terms of the
; license agreement
;
;------------------------------------------------------------------------------

%include "ProcessorUncoreCommon.inc"

;-------------------------------------------------------------------------------------------------------------
; CSR register address in legacy PCI format Bus:Dev:Func:Offset (used via IO port CF8/CFC access method)
;-------------------------------------------------------------------------------------------------------------
;Reg Header Date Stamp: 15ww22a
;----------------------------------------------------------------------------------------------------------------------------
;Bios Name                                        Address    CSR Name                                Bus    Device  Fun     Offset
;----------------------------------------------------------------------------------------------------------------------------


;----------------------------------------------------------------------------------------------------------------------------
;;; SPR/GNR-CSR-definition start
;----------------------------------------------------------------------------------------------------------------------------
; CSR definition started
;
; CPUBUSNO CSR
;
%define CSR_LEGACY_CPUBUSNO_VALID_UBOX_SPR BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0xd4 ;NCDECS_CR_CPUBUSNO_VALID_CFG                     30 0   2  0x00d4
%define CSR_LEGACY_CPUBUSNO_UBOX_SPR BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0xd0 ;NCDECS_CR_CPUBUSNO_CFG                                 30 0   2  0x00d0

%define CSR_LEGACY_CPUBUSNO_VALID_UBOX_GNR BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0xd4 ;NCDECS_CR_CPUBUSNO_VALID_CFG                     30 0   2  0x00d4
%define CSR_LEGACY_CPUBUSNO_UBOX_GNR BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0xd0 ;NCDECS_CR_CPUBUSNO_CFG                                 30 0   2  0x00d0

;
; CHA related CSRs
;
%define CSR_LEGACY_SAD_CONTROL_CBO_SPR       BIT31 | (BUS_U1 << 16) | (29 << 11) | (1 << 8) | 0xf4 ;CBO_CR_SAD_CONTROL_CFG                         31  29  1  0x00f4
%define CSR_LEGACY_SAD_TARGET_CBO_SPR        BIT31 | (BUS_U1 << 16) | (29 << 11) | (1 << 8) | 0xf8 ;CBO_CR_SAD_TARGET_CFG                          31  29  1  0x00f8
%define CSR_LEGACY_MMCFG_TARGET_LIST_CBO_SPR BIT31 | (BUS_U1 << 16) | (29 << 11) | (1 << 8) | 0xec ;CBO_CR_MMCFG_TARGET_LIST_CFG                   31  29  1  0x00ec
%define CSR_LEGACY_MMCFG_LOCAL_RULE_CFG_CBO_SPR BIT31 | (BUS_U1 << 16) | (29 << 11) | (1 << 8) | 0xc8 ;CBO_CR_MMCFG_LOCAL_RULE_CFG                 31  29  1  0x00c8
%define CSR_LEGACY_MMCFG_LOCAL_RULE_CFG2_CBO_SPR BIT31 | (BUS_U1 << 16) | (29 << 11) | (1 << 8) | 0xcc ;CBO_CR_MMCFG_LOCAL_RULE_CFG2               31  29  1  0x00cc
%define CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS3_CBO_SPR BIT31 | (BUS_U1 << 16) | (29 << 11) | (1 << 8) | 0xbc ;CBO_CR_MMCFG_LOCAL_RULE_ADDRESS_CFG_3  31  29  1  0x00bc
%define CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS2_CBO_SPR BIT31 | (BUS_U1 << 16) | (29 << 11) | (1 << 8) | 0xb8 ;CBO_CR_MMCFG_LOCAL_RULE_ADDRESS_CFG_2  31  29  1  0x00b8
%define CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS1_CBO_SPR BIT31 | (BUS_U1 << 16) | (29 << 11) | (1 << 8) | 0xb4 ;CBO_CR_MMCFG_LOCAL_RULE_ADDRESS_CFG_1  31  29  1  0x00b4
%define CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS0_CBO_SPR BIT31 | (BUS_U1 << 16) | (29 << 11) | (1 << 8) | 0xb0 ;CBO_CR_MMCFG_LOCAL_RULE_ADDRESS_CFG_0  31  29  1  0x00b0
%define CSR_LEGACY_MMCFG_RULE_HI_CBO_SPR BIT31 | (BUS_U1 << 16) | (29 << 11) | (1 << 8) | 0xc4 ;CBO_CR_MMCFG_RULE_CFG_N1                           31  29  1  0x00c4
%define CSR_LEGACY_MMCFG_RULE_CBO_SPR BIT31 | (BUS_U1 << 16) | (29 << 11) | (1 << 8) | 0xc0 ;CBO_CR_MMCFG_RULE_CFG_N0                              31  29  1  0x00c0
%define CSR_LEGACY_MMIO_RULE19_CBO_SPR BIT31 | (BUS_U1 << 16) | (29 << 11) | (1 << 8) | 0x154 ;MMIO_RULE_CFG_19_CHABC_SAD1_REG                     31  29  1  0x0154

%define CSR_LEGACY_SAD_CONTROL_CBO_GNR       BIT31 | (BUS_U0 << 16) | (13 << 11) | (1 << 8) | 0xf4 ;CBO_CR_SAD_CONTROL_CFG                         30  13  1  0x00f4
%define CSR_LEGACY_SAD_TARGET_CBO_GNR        BIT31 | (BUS_U0 << 16) | (13 << 11) | (1 << 8) | 0xf8 ;CBO_CR_SAD_TARGET_CFG                          30  13  1  0x00f8
%define CSR_LEGACY_MMCFG_TARGET_LIST_CBO_GNR BIT31 | (BUS_U0 << 16) | (13 << 11) | (1 << 8) | 0xec ;CBO_CR_MMCFG_TARGET_LIST_CFG                   30  13  1  0x00ec
%define CSR_LEGACY_MMCFG_LOCAL_RULE_CFG_CBO_GNR BIT31 | (BUS_U0 << 16) | (13 << 11) | (1 << 8) | 0xc8 ;CBO_CR_MMCFG_LOCAL_RULE_CFG                 30  13  1  0x00c8
%define CSR_LEGACY_MMCFG_LOCAL_RULE_CFG2_CBO_GNR BIT31 | (BUS_U0 << 16) | (13 << 11) | (1 << 8) | 0xcc ;CBO_CR_MMCFG_LOCAL_RULE_CFG2               30  13  1  0x00cc
%define CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS3_CBO_GNR BIT31 | (BUS_U0 << 16) | (13 << 11) | (1 << 8) | 0xbc ;CBO_CR_MMCFG_LOCAL_RULE_ADDRESS_CFG_3  30  13  1  0x00bc
%define CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS2_CBO_GNR BIT31 | (BUS_U0 << 16) | (13 << 11) | (1 << 8) | 0xb8 ;CBO_CR_MMCFG_LOCAL_RULE_ADDRESS_CFG_2  30  13  1  0x00b8
%define CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS1_CBO_GNR BIT31 | (BUS_U0 << 16) | (13 << 11) | (1 << 8) | 0xb4 ;CBO_CR_MMCFG_LOCAL_RULE_ADDRESS_CFG_1  30  13  1  0x00b4
%define CSR_LEGACY_MMCFG_LOCAL_RULE_ADDRESS0_CBO_GNR BIT31 | (BUS_U0 << 16) | (13 << 11) | (1 << 8) | 0xb0 ;CBO_CR_MMCFG_LOCAL_RULE_ADDRESS_CFG_0  30  13  1  0x00b0
%define CSR_LEGACY_MMCFG_RULE_HI_CBO_GNR BIT31 | (BUS_U0 << 16) | (13 << 11) | (1 << 8) | 0xc4 ;CBO_CR_MMCFG_RULE_CFG_N1                           30  13  1  0x00c4
%define CSR_LEGACY_MMCFG_RULE_CBO_GNR BIT31 | (BUS_U0 << 16) | (13 << 11) | (1 << 8) | 0xc0 ;CBO_CR_MMCFG_RULE_CFG_N0                              30  13  1  0x00c0
%define CSR_LEGACY_MMIO_RULE19_CBO_GNR BIT31 | (BUS_U0 << 16) | (13 << 11) | (1 << 8) | 0x154 ;MMIO_RULE_CFG_19_CHABC_SAD1_REG                     30  13  1  0x0154

;
; SCRATCHPAD related CSRs
;
%define LEGACY_BIOSSNONSTICKYSCRATCHPAD0_SPR BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0xc0 ;NCDECS_CR_BIOSNONSTICKYSCRATCHPAD0_CFG         30  0  2  0x00c0
%define LEGACY_BIOSSNONSTICKYSCRATCHPAD13_SPR BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0x94 ;NCDECS_CR_BIOSNONSTICKYSCRATCHPAD13_CFG       30  0  2  0x0094
%define LEGACY_BIOSSNONSTICKYSCRATCHPAD15_SPR BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0x9c ;NCDECS_CR_BIOSNONSTICKYSCRATCHPAD15_CFG       30  0  2  0x009c
%define LEGACY_BIOSSCRATCHPAD0_SPR BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0xa0 ;NCDECS_CR_BIOSSCRATCHPAD0_CFG                            30  0  2  0x00a0
%define LEGACY_BIOSSCRATCHPAD2_SPR BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0xa8 ;NCDECS_CR_BIOSSCRATCHPAD2_CFG                            30  0  2  0x00a8
%define LEGACY_BIOSSCRATCHPAD3_SPR BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0xac ;NCDECS_CR_BIOSSCRATCHPAD3_CFG                            30  0  2  0x00ac
%define LEGACY_BIOSSCRATCHPAD7_SPR BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0xbc ;NCDECS_CR_BIOSSCRATCHPAD7_CFG                            30  0  2  0x00bc
%define CSR_LEGACY_LOCKCONTROL_SPR BIT31 | (BUS_U0 << 16) | (0  << 11) | (0 << 8) | 0xd0 ;NCEVENTS_CR_LOCKCONTROL_CFG                              30  0  0  0x00d0

%define LEGACY_BIOSSNONSTICKYSCRATCHPAD0_GNR BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0xc0 ;NCDECS_CR_BIOSNONSTICKYSCRATCHPAD0_CFG         30  0  2  0x00c0
%define LEGACY_BIOSSNONSTICKYSCRATCHPAD13_GNR BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0x94 ;NCDECS_CR_BIOSNONSTICKYSCRATCHPAD13_CFG       30  0  2  0x0094
%define LEGACY_BIOSSNONSTICKYSCRATCHPAD15_GNR BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0x9c ;NCDECS_CR_BIOSNONSTICKYSCRATCHPAD15_CFG       30  0  2  0x009c
%define LEGACY_BIOSSCRATCHPAD0_GNR BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0xa0 ;NCDECS_CR_BIOSSCRATCHPAD0_CFG                            30  0  2  0x00a0
%define LEGACY_BIOSSCRATCHPAD2_GNR BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0xa8 ;NCDECS_CR_BIOSSCRATCHPAD2_CFG                            30  0  2  0x00a8
%define LEGACY_BIOSSCRATCHPAD3_GNR BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0xac ;NCDECS_CR_BIOSSCRATCHPAD3_CFG                            30  0  2  0x00ac
%define LEGACY_BIOSSCRATCHPAD7_GNR BIT31 | (BUS_U0 << 16) | (0  << 11) | (2 << 8) | 0xbc ;NCDECS_CR_BIOSSCRATCHPAD7_CFG                            30  0  2  0x00bc
%define CSR_LEGACY_LOCKCONTROL_GNR BIT31 | (BUS_U0 << 16) | (0  << 11) | (0 << 8) | 0xd0 ;NCEVENTS_CR_LOCKCONTROL_CFG                              30  0  0  0x00d0

;
; MMCFG CSRs
;
%define CSR_LEGACY_MMCFG_IIO_SPR BIT31 | (0 << 16) | (0  << 11) | (0 << 8) | 0x90 ;IIO_CSTACK_CR_MMCFG_BASE_0_0_0_CFG_N0                           0  0   0  0x0090
%define CSR_LEGACY_MMCFG_IIO_GNR BIT31 | (0 << 16) | (0  << 11) | (0 << 8) | 0x90 ;IIO_CSTACK_CR_MMCFG_BASE_0_0_0_CFG_N0                           0  0   0  0x0090

;----------------------------------------------------------------------------------------------------------------------------
;----------------------------------------------------------------------------------------------------------------------------

%define SSR_LEGACY_BUS_CONFIG_INFO_CSR_SPR LEGACY_BIOSSCRATCHPAD3_SPR
%define SSR_LEGACY_BOOT_CONFIG_INFO_CSR_SPR LEGACY_BIOSSCRATCHPAD7_SPR

%define SSR_LEGACY_BUS_CONFIG_INFO_CSR_GNR LEGACY_BIOSSCRATCHPAD3_GNR
%define SSR_LEGACY_BOOT_CONFIG_INFO_CSR_GNR LEGACY_BIOSSCRATCHPAD7_GNR

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

;
; UBOX related CSRs
;
%define CSR_CPUBUSNO_VALID_UBOX_SPR (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0xd4 ;01e020d4h  NCDECS_CR_CPUBUSNO_VALID_CFG                        30 0   2  0x00d4
%define CSR_CPUBUSNO_UBOX_SPR (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0xd0       ;01e020d0h  NCDECS_CR_CPUBUSNO_CFG                              30 0   2  0x00d0
%define CSR_CPUNODEID_UBOX_SPR (BUS_U0 << 20) | (0  << 15) | (0 << 12) | 0xc0      ;01e000c0h  NCEVENTS_CR_CPUNODEID_CFG                           30 0   0  0x00c0

%define CSR_CPUBUSNO_VALID_UBOX_GNR (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0xd4 ;01e020d4h  NCDECS_CR_CPUBUSNO_VALID_CFG                        30 0   2  0x00d4
%define CSR_CPUBUSNO_UBOX_GNR (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0xd0       ;01e020d0h  NCDECS_CR_CPUBUSNO_CFG                              30 0   2  0x00d0
%define CSR_CPUNODEID_UBOX_GNR (BUS_U0 << 20) | (0  << 15) | (0 << 12) | 0xc0      ;01e000c0h  NCEVENTS_CR_CPUNODEID_CFG                           30 0   0  0x00c0

;
; PCU related CSRs
;
%define CSR_GLOBAL_PKG_C_S_CONTROL_PCU_SPR (BUS_U1 << 20) | (30 << 15) | (2 << 12) | 0xa0  ;001f20a0h  PCU_CR_GLOBAL_PKG_C_S_CONTROL_REGISTER      31 30  2  0x00a0

%define CSR_GLOBAL_PKG_C_S_CONTROL_PCU_GNR (BUS_U0 << 20) | (5 << 15) | (0 << 12) | 0x540  ;001e5540h  PCU_CR_GLOBAL_PKG_C_S_CONTROL_REGISTER      30 5   0  0x0540

;
; CPL related CSRs
;
%define CSR_BIOS_RESET_CPL_SPR (BUS_U1 << 20) | (30 << 15) | (1 << 12) | 0x94  ;001f1094h  PCU_CR_BIOS_RESET_CPL_CFG                               31 30  1  0x0094

%define CSR_BIOS_RESET_CPL_GNR (BUS_U0 << 20) |  (5 << 15) | (0 << 12) | 0x198 ;001e5198h  PCU_CR_BIOS_RESET_CPL_CFG                               30 5   0  0x0198

;
; CHA related CSRs
;
%define CSR_SAD_CONTROL_CBO_SPR (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xf4               ;001e90f4h  CBO_CR_SAD_CONTROL_CFG                    31 29  1  0x00f4
%define CSR_SAD_TARGET_CBO_SPR (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xf8                ;001e90f8h  CBO_CR_SAD_TARGET_CFG                     31 29  1  0x00f8
%define CSR_MMCFG_TARGET_LIST_CBO_SPR (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xec         ;001e90ech  CBO_CR_MMCFG_TARGET_LIST_CFG              31 29  1  0x00ec
%define CSR_MMCFG_LOCAL_RULE_CFG_CBO_SPR (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xc8      ;001e90c8h  CBO_CR_MMCFG_LOCAL_RULE_CFG               31 29  1  0x00c8
%define CSR_MMCFG_LOCAL_RULE_CFG2_CBO_SPR (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xcc     ;001e90cch  CBO_CR_MMCFG_LOCAL_RULE_CFG2              31 29  1  0x00cc
%define CSR_IOAPIC_TARGET_LIST_7_SPR (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xac          ;001e90ach  CBO_CR_IOAPIC_TARGET_LIST_CFG_7           31 29  1  0x00ac
%define CSR_IOAPIC_TARGET_LIST_6_SPR (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xa8          ;001e90a8h  CBO_CR_IOAPIC_TARGET_LIST_CFG_6           31 29  1  0x00a8
%define CSR_IOAPIC_TARGET_LIST_5_SPR (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xa4          ;001e90a4h  CBO_CR_IOAPIC_TARGET_LIST_CFG_5           31 29  1  0x00a4
%define CSR_IOAPIC_TARGET_LIST_4_SPR (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xa0          ;001e90a0h  CBO_CR_IOAPIC_TARGET_LIST_CFG_4           31 29  1  0x00a0
%define CSR_IOAPIC_TARGET_LIST_3_SPR (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0x9c          ;001e909ch  CBO_CR_IOAPIC_TARGET_LIST_CFG_3           31 29  1  0x009c
%define CSR_IOAPIC_TARGET_LIST_2_SPR (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0x98          ;001e9098h  CBO_CR_IOAPIC_TARGET_LIST_CFG_2           31 29  1  0x0098
%define CSR_IOAPIC_TARGET_LIST_1_SPR (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0x94          ;001e9094h  CBO_CR_IOAPIC_TARGET_LIST_CFG_1           31 29  1  0x0094
%define CSR_IOAPIC_TARGET_LIST_0_SPR (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0x90          ;001e9090h  CBO_CR_IOAPIC_TARGET_LIST_CFG_0           31 29  1  0x0090
%define CSR_MMCFG_LOCAL_RULE_ADDRESS3_CBO_SPR (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xbc ;001e90bch  CBO_CR_MMCFG_LOCAL_RULE_ADDRESS_CFG_1     31 29  1  0x00bc
%define CSR_MMCFG_LOCAL_RULE_ADDRESS2_CBO_SPR (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xb8 ;001e90b8h  CBO_CR_MMCFG_LOCAL_RULE_ADDRESS_CFG_0     31 29  1  0x00b8
%define CSR_MMCFG_LOCAL_RULE_ADDRESS1_CBO_SPR (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xb4 ;001e90b4h  CBO_CR_MMCFG_LOCAL_RULE_ADDRESS_CFG_1     31 29  1  0x00b4
%define CSR_MMCFG_LOCAL_RULE_ADDRESS0_CBO_SPR (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xb0 ;001e90b0h  CBO_CR_MMCFG_LOCAL_RULE_ADDRESS_CFG_0     31 29  1  0x00b0
%define CSR_MMCFG_RULE_HI_CBO_SPR (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xc4             ;001e90c4h  CBO_CR_MMCFG_RULE_CFG_N1                  31 29  1  0x00c4
%define CSR_MMCFG_RULE_CBO_SPR (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xc0                ;001e90c0h  CBO_CR_MMCFG_RULE_CFG_N0                  31 29  1  0x00c0
%define CSR_MMIO_RULE0_CBO_SPR (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0x108               ;001e9108h  MMIO_RULE_CFG_0_CHABC_SAD1_REG            31 29  1  0x0108
%define CSR_MMIO_RULE19_CBO_SPR (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0x154              ;001e9154h  MMIO_RULE_CFG_19_CHABC_SAD1_REG           31 29  1  0x0154
%define CSR_IOPORT_TARGET_LIST_7_CBO_SPR (BUS_U1 << 20) | (29 << 15) | (0 << 12) | 0x2fc     ;001e82e0h  CBO_CR_IOPORT_TARGET_LIST_CFG_7           31 29  0  0x02fc
%define CSR_IOPORT_TARGET_LIST_6_CBO_SPR (BUS_U1 << 20) | (29 << 15) | (0 << 12) | 0x2f8     ;001e82e0h  CBO_CR_IOPORT_TARGET_LIST_CFG_6           31 29  0  0x02f8
%define CSR_IOPORT_TARGET_LIST_5_CBO_SPR (BUS_U1 << 20) | (29 << 15) | (0 << 12) | 0x2f4     ;001e82e0h  CBO_CR_IOPORT_TARGET_LIST_CFG_5           31 29  0  0x02f4
%define CSR_IOPORT_TARGET_LIST_4_CBO_SPR (BUS_U1 << 20) | (29 << 15) | (0 << 12) | 0x2f0     ;001e82e0h  CBO_CR_IOPORT_TARGET_LIST_CFG_4           31 29  0  0x02f0
%define CSR_IOPORT_TARGET_LIST_3_CBO_SPR (BUS_U1 << 20) | (29 << 15) | (0 << 12) | 0x2ec     ;001e82e0h  CBO_CR_IOPORT_TARGET_LIST_CFG_3           31 29  0  0x02ec
%define CSR_IOPORT_TARGET_LIST_2_CBO_SPR (BUS_U1 << 20) | (29 << 15) | (0 << 12) | 0x2e8     ;001e82e0h  CBO_CR_IOPORT_TARGET_LIST_CFG_2           31 29  0  0x02e8
%define CSR_IOPORT_TARGET_LIST_1_CBO_SPR (BUS_U1 << 20) | (29 << 15) | (0 << 12) | 0x2e4     ;001e82e0h  CBO_CR_IOPORT_TARGET_LIST_CFG_1           31 29  0  0x02e4
%define CSR_IOPORT_TARGET_LIST_0_CBO_SPR (BUS_U1 << 20) | (29 << 15) | (0 << 12) | 0x2e0     ;001e82e0h  CBO_CR_IOPORT_TARGET_LIST_CFG_0           31 29  0  0x02e0

%define CSR_SAD_CONTROL_CBO_GNR (BUS_U0 << 20) | (13 << 15) | (1 << 12) | 0xf4               ;001e690f4h  CBO_CR_SAD_CONTROL_CFG                   30 13  1  0x00f4
%define CSR_SAD_TARGET_CBO_GNR (BUS_U0 << 20) | (13 << 15) | (1 << 12) | 0xf8                ;001e690f8h  CBO_CR_SAD_TARGET_CFG                    30 13  1  0x00f8
%define CSR_MMCFG_TARGET_LIST_CBO_GNR (BUS_U0 << 20) | (13 << 15) | (1 << 12) | 0xec         ;001e690ech  CBO_CR_MMCFG_TARGET_LIST_CFG             30 13  1  0x00ec
%define CSR_MMCFG_LOCAL_RULE_CFG_CBO_GNR (BUS_U0 << 20) | (13 << 15) | (1 << 12) | 0xc8      ;001e690c8h  CBO_CR_MMCFG_LOCAL_RULE_CFG              30 13  1  0x00c8
%define CSR_MMCFG_LOCAL_RULE_CFG2_CBO_GNR (BUS_U0 << 20) | (13 << 15) | (1 << 12) | 0xcc     ;001e690cch  CBO_CR_MMCFG_LOCAL_RULE_CFG2             30 13  1  0x00cc
%define CSR_IOAPIC_TARGET_LIST_7_GNR (BUS_U0 << 20) | (13 << 15) | (1 << 12) | 0xac          ;001e690ach  CBO_CR_IOAPIC_TARGET_LIST_CFG_7          30 13  1  0x00ac
%define CSR_IOAPIC_TARGET_LIST_6_GNR (BUS_U0 << 20) | (13 << 15) | (1 << 12) | 0xa8          ;001e690a8h  CBO_CR_IOAPIC_TARGET_LIST_CFG_6          30 13  1  0x00a8
%define CSR_IOAPIC_TARGET_LIST_5_GNR (BUS_U0 << 20) | (13 << 15) | (1 << 12) | 0xa4          ;001e690a4h  CBO_CR_IOAPIC_TARGET_LIST_CFG_5          30 13  1  0x00a4
%define CSR_IOAPIC_TARGET_LIST_4_GNR (BUS_U0 << 20) | (13 << 15) | (1 << 12) | 0xa0          ;001e690a0h  CBO_CR_IOAPIC_TARGET_LIST_CFG_4          30 13  1  0x00a0
%define CSR_IOAPIC_TARGET_LIST_3_GNR (BUS_U0 << 20) | (13 << 15) | (1 << 12) | 0x9c          ;001e6909ch  CBO_CR_IOAPIC_TARGET_LIST_CFG_3          30 13  1  0x009c
%define CSR_IOAPIC_TARGET_LIST_2_GNR (BUS_U0 << 20) | (13 << 15) | (1 << 12) | 0x98          ;001e69098h  CBO_CR_IOAPIC_TARGET_LIST_CFG_2          30 13  1  0x0098
%define CSR_IOAPIC_TARGET_LIST_1_GNR (BUS_U0 << 20) | (13 << 15) | (1 << 12) | 0x94          ;001e69094h  CBO_CR_IOAPIC_TARGET_LIST_CFG_1          30 13  1  0x0094
%define CSR_IOAPIC_TARGET_LIST_0_GNR (BUS_U0 << 20) | (13 << 15) | (1 << 12) | 0x90          ;001e69090h  CBO_CR_IOAPIC_TARGET_LIST_CFG_0          30 13  1  0x0090
%define CSR_MMCFG_LOCAL_RULE_ADDRESS3_CBO_GNR (BUS_U0 << 20) | (13 << 15) | (1 << 12) | 0xbc ;001e690bch  CBO_CR_MMCFG_LOCAL_RULE_ADDRESS_CFG_3    30 13  1  0x00bc
%define CSR_MMCFG_LOCAL_RULE_ADDRESS2_CBO_GNR (BUS_U0 << 20) | (13 << 15) | (1 << 12) | 0xb8 ;001e690b8h  CBO_CR_MMCFG_LOCAL_RULE_ADDRESS_CFG_2    30 13  1  0x00b8
%define CSR_MMCFG_LOCAL_RULE_ADDRESS1_CBO_GNR (BUS_U0 << 20) | (13 << 15) | (1 << 12) | 0xb4 ;001e690b4h  CBO_CR_MMCFG_LOCAL_RULE_ADDRESS_CFG_1    30 13  1  0x00b4
%define CSR_MMCFG_LOCAL_RULE_ADDRESS0_CBO_GNR (BUS_U0 << 20) | (13 << 15) | (1 << 12) | 0xb0 ;001e690b0h  CBO_CR_MMCFG_LOCAL_RULE_ADDRESS_CFG_0    30 13  1  0x00b0
%define CSR_MMCFG_RULE_HI_CBO_GNR (BUS_U0 << 20) | (13 << 15) | (1 << 12) | 0xc4             ;001e690c4h  CBO_CR_MMCFG_RULE_CFG_N1                 30 13  1  0x00c4
%define CSR_MMCFG_RULE_CBO_GNR (BUS_U0 << 20) | (13 << 15) | (1 << 12) | 0xc0                ;001e690c0h  CBO_CR_MMCFG_RULE_CFG_N0                 30 13  1  0x00c0
%define CSR_MMIO_RULE0_CBO_GNR (BUS_U0 << 20) | (13 << 15) | (1 << 12) | 0x108               ;001e69108h  MMIO_RULE_CFG_0_CHABC_SAD1_REG           30 13  1  0x0108
%define CSR_MMIO_RULE19_CBO_GNR (BUS_U0 << 20) | (13 << 15) | (1 << 12) | 0x154              ;001e69154h  MMIO_RULE_CFG_19_CHABC_SAD1_REG          30 13  1  0x0154
%define CSR_IOPORT_TARGET_LIST_7_CBO_GNR (BUS_U0 << 20) | (13 << 15) | (0 << 12) | 0x2fc     ;001e682e0h  CBO_CR_IOPORT_TARGET_LIST_CFG_7          30 13  0  0x02fc
%define CSR_IOPORT_TARGET_LIST_6_CBO_GNR (BUS_U0 << 20) | (13 << 15) | (0 << 12) | 0x2f8     ;001e682e0h  CBO_CR_IOPORT_TARGET_LIST_CFG_6          30 13  0  0x02f8
%define CSR_IOPORT_TARGET_LIST_5_CBO_GNR (BUS_U0 << 20) | (13 << 15) | (0 << 12) | 0x2f4     ;001e682e0h  CBO_CR_IOPORT_TARGET_LIST_CFG_5          30 13  0  0x02f4
%define CSR_IOPORT_TARGET_LIST_4_CBO_GNR (BUS_U0 << 20) | (13 << 15) | (0 << 12) | 0x2f0     ;001e682e0h  CBO_CR_IOPORT_TARGET_LIST_CFG_4          30 13  0  0x02f0
%define CSR_IOPORT_TARGET_LIST_3_CBO_GNR (BUS_U0 << 20) | (13 << 15) | (0 << 12) | 0x2ec     ;001e682e0h  CBO_CR_IOPORT_TARGET_LIST_CFG_3          30 13  0  0x02ec
%define CSR_IOPORT_TARGET_LIST_2_CBO_GNR (BUS_U0 << 20) | (13 << 15) | (0 << 12) | 0x2e8     ;001e682e0h  CBO_CR_IOPORT_TARGET_LIST_CFG_2          30 13  0  0x02e8
%define CSR_IOPORT_TARGET_LIST_1_CBO_GNR (BUS_U0 << 20) | (13 << 15) | (0 << 12) | 0x2e4     ;001e682e0h  CBO_CR_IOPORT_TARGET_LIST_CFG_1          30 13  0  0x02e4
%define CSR_IOPORT_TARGET_LIST_0_CBO_GNR (BUS_U0 << 20) | (13 << 15) | (0 << 12) | 0x2e0     ;001e682e0h  CBO_CR_IOPORT_TARGET_LIST_CFG_0          30 13  0  0x02e0

;
; SCRATCHPAD related CSRs
;
%define SR_PBSP_CHECKIN_CSR_SPR (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0xfc           ;001e020fch  NCDECS_CR_BIOSNONSTICKYSCRATCHPAD2_CFG       30 0   2  0x00fc
%define CSR_BIOSNONSTICKYSCRATCHPAD0_SPR (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0xc0  ;001e020c0h  NCDECS_CR_BIOSNONSTICKYSCRATCHPAD0_CFG       30 0   2  0x00c0
%define CSR_BIOSNONSTICKYSCRATCHPAD2_SPR (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0xfc  ;001e020fch  NCDECS_CR_BIOSNONSTICKYSCRATCHPAD2_CFG       30 0   2  0x00fc
%define CSR_BIOSNONSTICKYSCRATCHPAD11_SPR (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0x8c ;001e0208ch  NCDECS_CR_BIOSNONSTICKYSCRATCHPAD11_CFG      30 0   2  0x008c
%define CSR_BIOSNONSTICKYSCRATCHPAD13_SPR (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0x94 ;001e02094h  NCDECS_CR_BIOSNONSTICKYSCRATCHPAD13_CFG      30 0   2  0x0094
%define CSR_BIOSNONSTICKYSCRATCHPAD14_SPR (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0x98 ;001e02098h  NCDECS_CR_BIOSNONSTICKYSCRATCHPAD14_CFG      30 0   2  0x0098
%define CSR_BIOSNONSTICKYSCRATCHPAD15_SPR (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0x9c ;001e0209ch  NCDECS_CR_BIOSNONSTICKYSCRATCHPAD15_CFG      30 0   2  0x009c
%define CSR_BIOSSCRATCHPAD7_SPR (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0xbc           ;001e020bch  NCDECS_CR_BIOSSCRATCHPAD7_CFG                30 0   2  0x00bc
%define CSR_BIOSSCRATCHPAD0_SPR (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0xa0           ;001e020a0h  NCDECS_CR_BIOSSCRATCHPAD0_CFG                30 0   2  0x00a0

%define SR_PBSP_CHECKIN_CSR_GNR (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0xfc           ;001e020fch  NCDECS_CR_BIOSNONSTICKYSCRATCHPAD2_CFG       30 0   2  0x00fc
%define CSR_BIOSNONSTICKYSCRATCHPAD0_GNR (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0xc0  ;001e020c0h  NCDECS_CR_BIOSNONSTICKYSCRATCHPAD0_CFG       30 0   2  0x00c0
%define CSR_BIOSNONSTICKYSCRATCHPAD2_GNR (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0xfc  ;001e020fch  NCDECS_CR_BIOSNONSTICKYSCRATCHPAD2_CFG       30 0   2  0x00fc
%define CSR_BIOSNONSTICKYSCRATCHPAD11_GNR (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0x8c ;001e0208ch  NCDECS_CR_BIOSNONSTICKYSCRATCHPAD11_CFG      30 0   2  0x008c
%define CSR_BIOSNONSTICKYSCRATCHPAD13_GNR (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0x94 ;001e02064h  NCDECS_CR_BIOSNONSTICKYSCRATCHPAD13_CFG      30 0   2  0x0094
%define CSR_BIOSNONSTICKYSCRATCHPAD14_GNR (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0x98 ;001e02098h  NCDECS_CR_BIOSNONSTICKYSCRATCHPAD14_CFG      30 0   2  0x0098
%define CSR_BIOSNONSTICKYSCRATCHPAD15_GNR (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0x9c ;001e0209ch  NCDECS_CR_BIOSNONSTICKYSCRATCHPAD15_CFG      30 0   2  0x009c
%define CSR_BIOSSCRATCHPAD7_GNR (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0xbc           ;001e020bch  NCDECS_CR_BIOSSCRATCHPAD7_CFG                30 0   2  0x00bc
%define CSR_BIOSSCRATCHPAD0_GNR (BUS_U0 << 20) | (0  << 15) | (2 << 12) | 0xa0           ;001e020a0h  NCDECS_CR_BIOSSCRATCHPAD0_CFG                30 0   2  0x00a0

;
; LOCKCONTROL CSR
;
%define CSR_LOCKCONTROL_SPR (BUS_U0 << 20) | (0  << 15) | (0 << 12) | 0xd0 ;000400d0h  NCEVENTS_CR_LOCKCONTROL_CFG                                 30 0   0  0x00d0
%define CSR_LOCKCONTROL_GNR (BUS_U0 << 20) | (0  << 15) | (0 << 12) | 0xd0 ;000400d0h  NCEVENTS_CR_LOCKCONTROL_CFG                                 30 0   0  0x00d0

;
; M3KTI related CSRs
;
%define CSR_VID_M3KTI_MAIN_SPR (BUS_U0 << 20) | (5  << 15) | (0 << 12) | 0x00 ;01e28000h  VID_M3KTI_MAIN_REG                                       30 5   0  0x0000
%define CSR_VID_M3KTI_MAIN_GNR (BUS_U0 << 20) | (5  << 15) | (0 << 12) | 0x00 ;01e28000h  VID_M3KTI_MAIN_REG                                       30 5   0  0x0000

;
; MMCFG related CSRs
;
%define CSR_MMCFG_IIO_SPR (0 << 20) | (0  << 15) | (0 << 12) | 0x90  ;00000090h  IIO_CSTACK_CR_MMCFG_BASE_0_0_0_CFG_N0                             0  0   0  0x0090
%define CSR_MMCFG_IIO_GNR (0 << 20) | (0  << 15) | (0 << 12) | 0x90  ;00000090h  IIO_CSTACK_CR_MMCFG_BASE_0_0_0_CFG_N0                             0  0   0  0x0090

;
; KTI related CSRs
;
%define CSR_KTIREUT_PH_CTR1_SPR (BUS_U0 << 20) | (1  << 15) | (2 << 12) | 0x12c      ;01e0a12ch  KTIREUT_PH_CTR1_KTI_REUT_REG                      30  1  2  0x012c
%define CSR_KTIREUT_PH_CSS_SPR (BUS_U0 << 20) | (1  << 15) | (2 << 12) | 0x150       ;01e0a150h  KTIREUT_PH_CSS_KTI_REUT_REG                       30  1  2  0x0150
%define CSR_KTIREUT_PH_CIS_SPR (BUS_U0 << 20) | (1  << 15) | (2 << 12) | 0x148       ;01e0a148h  KTIREUT_PH_CIS_KTI_REUT_REG                       30  1  2  0x0148
%define CSR_PIPE_CLK_RATE_CTRL_SPR (BUS_U0 << 20) | (1  << 15) | (2 << 12) | 0x2e0   ;01e0a2e0h  PIPE_CLK_RATE_CTRL_KTI_REUT_REG                   30  1  2  0x02e0
%define CSR_KTILP0_KTI_LLPMON_REG_SPR (BUS_U0 << 20) | (1  << 15) | (1 << 12) | 0x94 ;01e09094h  KTILP0_KTI_LLPMON_REG                             30  1  1  0x0094
%define CSR_KTILCL_KTI_LLPMON_REG_SPR (BUS_U0 << 20) | (1  << 15) | (1 << 12) | 0x84 ;01e09084h  KTILCL_KTI_LLPMON_REG                             30  1  1  0x0084

%define CSR_KTIREUT_PH_CTR1_GNR (BUS_U1 << 20) | (16  << 15) | (2 << 12) | 0x12c      ;01f8212ch  KTIREUT_PH_CTR1_KTI_REUT_REG                     31  16 2  0x012c
%define CSR_KTIREUT_PH_CSS_GNR (BUS_U1 << 20) | (16  << 15) | (2 << 12) | 0x150       ;01f82150h  KTIREUT_PH_CSS_KTI_REUT_REG                      31  16 2  0x0150
%define CSR_KTIREUT_PH_CIS_GNR (BUS_U1 << 20) | (16  << 15) | (2 << 12) | 0x148       ;01f82148h  KTIREUT_PH_CIS_KTI_REUT_REG                      31  16 2  0x0148
%define CSR_PIPE_CLK_RATE_CTRL_GNR (BUS_U1 << 20) | (16  << 15) | (2 << 12) | 0x2e0   ;01f822e0h  PIPE_CLK_RATE_CTRL_KTI_REUT_REG                  31  16 2  0x02e0
%define CSR_KTILP0_KTI_LLPMON_REG_GNR (BUS_U1 << 20) | (16  << 15) | (1 << 12) | 0x94 ;01f81094h  KTILP0_KTI_LLPMON_REG                            31  16 1  0x0094
%define CSR_KTILCL_KTI_LLPMON_REG_GNR (BUS_U1 << 20) | (16  << 15) | (1 << 12) | 0x84 ;01f81084h  KTILCL_KTI_LLPMON_REG                            31  16 1  0x0084

;
; PCU related CSRs
;
%define CSR_BIOS_MAILBOX_DATA_PCU_FUN1_REG_SPR (BUS_U1 << 20) | (30 << 15) | (1 << 12) | 0x8C      ;01ff108ch BIOS_MAILBOX_DATA_PCU_FUN1_REG       31  30 1  0x8C
%define CSR_BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG_SPR (BUS_U1 << 20) | (30 << 15) | (1 << 12) | 0x90 ;01ff1090h BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG  31  30 1  0x90

%define CSR_BIOS_MAILBOX_DATA_PCU_FUN0_REG_GNR (BUS_U0 << 20) | (5 << 15) | (0 << 12) | 0x190      ;01e28190h BIOS_MAILBOX_DATA_PCU_FUN0_REG       30  5  0  0x190
%define CSR_BIOS_MAILBOX_INTERFACE_PCU_FUN0_REG_GNR (BUS_U0 << 20) | (5 << 15) | (0 << 12) | 0x194 ;01e28194h BIOS_MAILBOX_INTERFACE_PCU_FUN0_REG  30  5  0  0x194

;
; ROOTBUS related CSRs
;
%define CSR_ROOTBUS_IIO_VTD_REG_SPR (0 << 20) | (0  << 15) | (0 << 12) | 0x108 ;00000108 ROOTBUS_IIO_VTD_REG                                        0  0  0  0x108
%define CSR_ROOTBUS_IIO_VTD_REG_GNR (0 << 20) | (0  << 15) | (0 << 12) | 0x108 ;00000108 ROOTBUS_IIO_VTD_REG                                        0  0  0  0x108

%define CSR_EMULATION_FLAG_UBOX_CFG_REG (BUS_U0 << 20) | (0  << 15) | (1 << 12) | 0xf00  ;000420bch ;                                               30 0  1  0xf00
;----------------------------------------------------------------------------------------------------------------------------

%define CSR_MMCFG_IIO_BASE_LIMIT_GAP_SPR 8 ;MMCFG_IIO register is QWORD size
%define MMIO_RULE19_BASEADDRESS 0xFC000000
%define MMIO_RULE19_LIMITADDRESS 0xFFFFFFFF

%define SR_LEGACY_BUSCFG_SYNC_CSR_SPR LEGACY_BIOSSNONSTICKYSCRATCHPAD13_SPR
%define SR_LEGACY_BUSCFG_DATA_CSR_SPR LEGACY_BIOSSNONSTICKYSCRATCHPAD15_SPR
%define SR_BUSCFG_SYNC_CSR_SPR CSR_BIOSNONSTICKYSCRATCHPAD13_SPR
%define SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_SPR CSR_BIOSNONSTICKYSCRATCHPAD13_SPR
%define SR_MEMORY_DATA_STORAGE_COMMAND_PIPE_CSR_SPR CSR_BIOSNONSTICKYSCRATCHPAD14_SPR
%define SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_SPR CSR_BIOSNONSTICKYSCRATCHPAD15_SPR

%define SR_LEGACY_BUSCFG_SYNC_CSR_GNR LEGACY_BIOSSNONSTICKYSCRATCHPAD13_GNR
%define SR_LEGACY_BUSCFG_DATA_CSR_GNR LEGACY_BIOSSNONSTICKYSCRATCHPAD15_GNR
%define SR_BUSCFG_SYNC_CSR_GNR CSR_BIOSNONSTICKYSCRATCHPAD13_GNR
%define SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_GNR CSR_BIOSNONSTICKYSCRATCHPAD13_GNR
%define SR_MEMORY_DATA_STORAGE_COMMAND_PIPE_CSR_GNR CSR_BIOSNONSTICKYSCRATCHPAD14_GNR
%define SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_GNR CSR_BIOSNONSTICKYSCRATCHPAD15_GNR

;----------------------------------------------------------------------------------------------------------------------------
; SPR/GNR-CSR-definition ended
;----------------------------------------------------------------------------------------------------------------------------

;------------------------------------------------------------------------------
; Pre-defined constants
;------------------------------------------------------------------------------
; APTIOV_SERVER_OVERRIDE_RC_START : To resolve Build error
%define DEFAULT_MMCFG_RULE (PCIEX_BASE_ADDRESS + 8)  ; mmcfg.base, with max 256 buses
; APTIOV_SERVER_OVERRIDE_RC_END : To resolve Build error
%define DEFAULT_MMCFG_LOCAL_RULE_SKX 0x3f543210                 ; Local Targets = [5,4,3,2,1,0], Mode[5:0] = 111111b for SKX
%define DEFAULT_MMCFG_LOCAL_RULE_ICX 0x543210                   ; Local Targets = [5,4,3,2,1,0], Mode[5:0] = 000000b for ICX
%define DEFAULT_MMCFG_LOCAL_RULE_CFG_SPR 0x543210                   ; Local Targets = [5,4,3,2,1,0], Mode[5:0] = 000000b for SPR
%define DEFAULT_MMCFG_LOCAL_RULE_CFG2_SPR 0xBA9876                  ; Local Targets = [11,10,9,8,7,6], Mode[5:0] = 000000b for SPR
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
%define DEFAULT_IOAPIC_TARGET_LIST_SPR 0x10101010             ; all cfg requests are confined to the local socket for SPR

%define MMCFG_RULE_ADDR_MASK 0xffffc000                ; mask off bit[13:0]

%define MMCFG_RULE_CLR_ENABLE 0xfffffffe                ; mask bit[0]

%define LEGACY_BUS_NO0 0x0                        ; Legacy socket stack0 bus number
%define LEGACY_BUS_NO1 0x11                       ; Legacy socket stack1 bus number
%define LEGACY_BUS_NO2 0x12                       ; Legacy socket stack2 bus number
%define LEGACY_BUS_NO3 0x13                       ; Legacy socket stack3 bus number
%define LEGACY_BUS_NO4 0x14                       ; Legacy socket stack4 bus number
%define LEGACY_BUS_NO5 0x15                       ; Legacy socket stack5 bus number
%define LEGACY_BUS_NO6 0x16                       ; Legacy socket stack6 bus number
%define LEGACY_BUS_NO7 0x17                       ; Legacy socket stack7 bus number
%define LEGACY_BUS_NO8 0x18                       ; Legacy socket stack8 bus number
%define LEGACY_BUS_NO9 0x19                       ; Legacy socket stack9 bus number
%define LEGACY_BUS_NO10 0x1a                      ; Legacy socket stack10 bus number
%define LEGACY_BUS_NO11 0x1b                      ; Legacy socket stack11 bus number
%define LEGACY_BUS_NO12 0x1c                      ; Legacy socket stack12 bus number
%define LEGACY_BUS_NO13 0x1d                      ; Legacy socket stack13 bus number
%define LEGACY_BUS_NO14 0x1e                      ; Legacy socket stack14 bus number
%define LEGACY_BUS_NO15 0x1f                      ; Legacy socket stack15 bus number

%define DEFAULT_BUS_NO0 0x0
%define DEFAULT_BUS_NO1 0x1
%define DEFAULT_BUS_NO2 0x2
%define DEFAULT_BUS_NO3 0x3
%define DEFAULT_BUS_NO4 0x4
%define DEFAULT_BUS_NO5 0x5
%define DEFAULT_BUS_NO6 0x6
%define DEFAULT_BUS_NO7 0x7
%define DEFAULT_BUS_NO8 0x8
%define DEFAULT_BUS_NO9 0x9
%define DEFAULT_BUS_NO10 0xa
%define DEFAULT_BUS_NO11 0xb
%define DEFAULT_BUS_NO12 0xc
%define DEFAULT_BUS_NO13 0xd
%define DEFAULT_BUS_NO14 0xe
%define DEFAULT_BUS_NO15 0xf

; UBOX CPUBUSNO register only have ubus0 and ubus1 fields
%define DEFAULT_COLDBOOT_CPUBUSNO (BUS_U1 << 8) + BUS_U0     ; bus#[0x1F,0x1E]
%define LEGACY_COLDBOOT_CPUBUSNO (BUS_U1 << 8) + BUS_U0

%define DEFAULT_MMCFG_LOCAL_RULE_ADDRESS0_SPR (DEFAULT_BUS_NO2 << 24) + (DEFAULT_BUS_NO1 << 16) + ((DEFAULT_BUS_NO1-1) << 8)+ DEFAULT_BUS_NO0
%define DEFAULT_MMCFG_LOCAL_RULE_ADDRESS1_SPR (DEFAULT_BUS_NO6 << 24) + (DEFAULT_BUS_NO5 << 16) + (DEFAULT_BUS_NO4 << 8) + DEFAULT_BUS_NO3
%define DEFAULT_MMCFG_LOCAL_RULE_ADDRESS2_SPR (DEFAULT_BUS_NO10 << 24) + (DEFAULT_BUS_NO9 << 16) + (DEFAULT_BUS_NO8 << 8) + DEFAULT_BUS_NO7
%define DEFAULT_MMCFG_LOCAL_RULE_ADDRESS3_SPR DEFAULT_BUS_NO11
%define LEGACY_MMCFG_LOCAL_RULE_ADDRESS0_SPR (LEGACY_BUS_NO2 << 24) + (LEGACY_BUS_NO1 << 16) + ((LEGACY_BUS_NO1-1) << 8)+ LEGACY_BUS_NO0
%define LEGACY_MMCFG_LOCAL_RULE_ADDRESS1_SPR (LEGACY_BUS_NO6 << 24) + (LEGACY_BUS_NO5 << 16) + (LEGACY_BUS_NO4 << 8) + LEGACY_BUS_NO3
%define LEGACY_MMCFG_LOCAL_RULE_ADDRESS2_SPR (LEGACY_BUS_NO10 << 24) + (LEGACY_BUS_NO9 << 16) + (LEGACY_BUS_NO8 << 8) + LEGACY_BUS_NO7
%define LEGACY_MMCFG_LOCAL_RULE_ADDRESS3_SPR LEGACY_BUS_NO11

;
; GNR supports 16 stacks, when model fully suported these rules must be modified
;
%define DEFAULT_MMCFG_LOCAL_RULE_ADDRESS0_GNR (DEFAULT_BUS_NO2  << 24) + (DEFAULT_BUS_NO1 << 16) + (DEFAULT_BUS_NO0 << 8) + DEFAULT_BUS_NO0
%define DEFAULT_MMCFG_LOCAL_RULE_ADDRESS1_GNR (DEFAULT_BUS_NO6  << 24) + (DEFAULT_BUS_NO5 << 16) + (DEFAULT_BUS_NO4 << 8) + DEFAULT_BUS_NO3
%define DEFAULT_MMCFG_LOCAL_RULE_ADDRESS2_GNR (DEFAULT_BUS_NO10 << 24) + (DEFAULT_BUS_NO9 << 16) + (DEFAULT_BUS_NO8 << 8) + DEFAULT_BUS_NO7
%define DEFAULT_MMCFG_LOCAL_RULE_ADDRESS3_GNR DEFAULT_BUS_NO11
%define LEGACY_MMCFG_LOCAL_RULE_ADDRESS0_GNR (LEGACY_BUS_NO2  << 24) + (LEGACY_BUS_NO1 << 16) + (LEGACY_BUS_NO0 << 8) + LEGACY_BUS_NO0
%define LEGACY_MMCFG_LOCAL_RULE_ADDRESS1_GNR (LEGACY_BUS_NO6  << 24) + (LEGACY_BUS_NO5 << 16) + (LEGACY_BUS_NO4 << 8) + LEGACY_BUS_NO3
%define LEGACY_MMCFG_LOCAL_RULE_ADDRESS2_GNR (LEGACY_BUS_NO10 << 24) + (LEGACY_BUS_NO9 << 16) + (LEGACY_BUS_NO8 << 8) + LEGACY_BUS_NO7
%define LEGACY_MMCFG_LOCAL_RULE_ADDRESS3_GNR LEGACY_BUS_NO11

%define BusCfgSync_PBSP_Ready 0xABCDEF01
%define BusCfgSync_PBSP_Go 0xABCDEF02
%define BusCfgSync_PBSP_Done 0xABCDEF03

%define BIOS_MAILBOX_COMMAND_SWITCHTO_P1 0x800000af    ;ICX_TODO check later if icx mailbox command value is changed from skx
%define BIOS_MAILBOX_DATA_SWITCHTO_P1 0x3f3f

%define PIPE_DISPATCH_SYNCH_PSYSHOST 4
%define PIPE_DISPATCH_COMMAND_KTI_ROUTINE   5
%define PIPE_DISPATCH_COMMAND_MULTI_SKT_UPD_GO_HLT 6
%define PIPE_DISPATCH_COMMAND_KTI_ADAPTATION   11

; VCU Mailbox API related equates
%define VCODE_API_OPCODE_SET_PARAMETER 0x1
%define VCODE_API_SEQ_ID_IOT_LLC_SETUP 0x1000E
%define VCODE_API_OPCODE_OPEN_SEQUENCE 0x3
%define VCODE_API_OPCODE_CLOSE_SEQUENCE 0x4
%define VCODE_API_OPCODE_IOT_LLC_SETUP 0x1007
%define VCODE_API_DELAY_COUNT 0x1000000

%define MAX_KTI_PORTS_PER_SOCKET      4
%define C_INIT_BEGIN_BIT              BIT5
%define CP_RESET_BIT                  BIT0
%define C_INIT_BITS                   BIT6 | BIT7
%define CRI_PCLKCHANGOK_FLAG_BIT      BIT3
%define BASE_NODEID_KTILP0_OFFSET     16
%define LINK_LAYER_RESET_BIT          BIT0
%define SP_USE_BACK_CHANNEL_INUSE_BIT BIT6

%define UPI_SLOW_MODE_MASK 0xFFFFFF00
%define UPI_LINK_DISABLE_MASK 0xFF
%define UPI_TXEQ_ADAPTATION_LOOPBACK_PHASE 0
%define UPI_TXEQ_ADAPTATION_SLOWMODE_PHASE 1

%define MAILBOX_BIOS_CMD_SET_UPI_FREQ      0x88
%define MAILBOX_BIOS_CMD_SET_UPI_FREQ_INTERFACE_LINK_INSTANCE_ID_LSB      0x8
%define MAILBOX_BIOS_CMD_SET_UPI_FREQ_INTERFACE_FAST_MODE_BIT         BIT12
%define MAILBOX_BIOS_CMD_SET_UPI_FREQ_INTERFACE_TRIGGER_BIT           BIT11
%define RUN_BUSY_BIT      BIT31
%define DELAY_COUNT 0x1000000
%define ROOT_BUSS_VALID BIT15

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

%macro BUSNO1_EBX 0
  and   ebx, 0xf00fffff
  or    ebx, LEGACY_BUS_NO1 << 20
%endmacro
