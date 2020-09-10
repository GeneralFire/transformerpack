; @file Flat32.inc
;  Platform constants and macros
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

%define BIT0 0x1
%define BIT1 0x2
%define BIT2 0x4
%define BIT3 0x8
%define BIT4 0x10
%define BIT5 0x20
%define BIT6 0x40
%define BIT7 0x80
%define BIT8 0x100
%define BIT9 0x200
%define BIT10 0x400
%define BIT11 0x800
%define BIT12 0x1000
%define BIT13 0x2000
%define BIT14 0x4000
%define BIT15 0x8000
%define BIT16 0x10000
%define BIT17 0x20000
%define BIT18 0x40000
%define BIT19 0x80000
%define BIT20 0x100000
%define BIT21 0x200000
%define BIT22 0x400000
%define BIT23 0x800000
%define BIT24 0x1000000
%define BIT25 0x2000000
%define BIT26 0x4000000
%define BIT27 0x8000000
%define BIT28 0x10000000
%define BIT29 0x20000000
%define BIT30 0x40000000
%define BIT31 0x80000000

;IA32 Arch specific equates
;------------------------------------------------------------------------------

%define MTRR_PHYS_BASE_0 0x200
%define MTRR_PHYS_MASK_0 0x201
%define MTRR_PHYS_BASE_1 0x202
%define MTRR_PHYS_MASK_1 0x203
%define MTRR_PHYS_BASE_2 0x204
%define MTRR_PHYS_MASK_2 0x205
%define MTRR_PHYS_BASE_3 0x206
%define MTRR_PHYS_MASK_3 0x207
%define MTRR_PHYS_BASE_4 0x208
%define MTRR_PHYS_MASK_4 0x209
%define MTRR_PHYS_BASE_5 0x20A
%define MTRR_PHYS_MASK_5 0x20B
%define MTRR_PHYS_BASE_6 0x20C
%define MTRR_PHYS_MASK_6 0x20D
%define MTRR_PHYS_BASE_7 0x20E
%define MTRR_PHYS_MASK_7 0x20F
%define MTRR_PHYS_BASE_8 0x210
%define MTRR_PHYS_MASK_8 0x211
%define MTRR_PHYS_BASE_9 0x212
%define MTRR_PHYS_MASK_9 0x213
%define MTRR_FIX_64K_00000 0x250
%define MTRR_FIX_16K_80000 0x258
%define MTRR_FIX_16K_A0000 0x259
%define MTRR_FIX_4K_C0000 0x268
%define MTRR_FIX_4K_C8000 0x269
%define MTRR_FIX_4K_D0000 0x26A
%define MTRR_FIX_4K_D8000 0x26B
%define MTRR_FIX_4K_E0000 0x26C
%define MTRR_FIX_4K_E8000 0x26D
%define MTRR_FIX_4K_F0000 0x26E
%define MTRR_FIX_4K_F8000 0x26F
%define MTRR_DEF_TYPE 0x2FF

%define MTRR_MEMORY_TYPE_UC 0x0
%define MTRR_MEMORY_TYPE_WC 0x1
%define MTRR_MEMORY_TYPE_WT 0x4
%define MTRR_MEMORY_TYPE_WP 0x5
%define MTRR_MEMORY_TYPE_WB 0x6

%define MTRR_DEF_TYPE_E 0x800
%define MTRR_DEF_TYPE_FE 0x400
%define MTRR_PHYSMASK_VALID 0x800

;
; Define the high 32 bits of MTRR masking
; This should be read from CPUID EAX = 080000008h, EAX bits [7:0]
; But for most platforms this will be a fixed supported size so it is
; fixed to save space.
;
%define MTRR_PHYS_MASK_VALID 0x800
%define MTRR_PHYS_MASK_HIGH 0xF      ; For 36 bit addressing
;MTRR_PHYS_MASK_HIGH           EQU 0000000FFh      ; For 40 bit addressing

%define FV_MAIN_BASE 0xFFFFFFFC
