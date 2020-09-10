; @file ProcessorUncoreCommon.inc
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

%define CPU_FAMILY_HSX     0x306F         ; bit[19:4] of CPUID_EAX
%define CPU_FAMILY_SKX     0x5065         ; bit[19:4] of CPUID_EAX
%define CPU_FAMILY_ICX     0x606A         ; bit[19:4] of CPUID_EAX
%define CPU_FAMILY_ICXD    0x606C         ; bit[19:4] of CPUID_EAX
%define CPU_FAMILY_SNR     0x8066         ; bit[19:4] of CPUID_EAX
%define CPU_FAMILY_SPRSP   0x806F         ; bit[19:4] of CPUID_EAX
%define CPU_FAMILY_GNRSP   0xA06D         ; bit[19:4] of CPUID_EAX
%define CPU_FAMILY_SRFSP   0xA06F         ; bit[19:4] of CPUID_EAX

%define A0_REV_HSX         0              ; bit[3:0]  of CPUID_EAX
%define R0_REV_ICX         0              ; bit[3:0]  of CPUID_EAX
%define L0_REV_ICX         4              ; bit[3:0]  of CPUID_EAX
%define ICX_D_LCC_NEW_REV  28             ; bit[28]   of XMM0

%define A0_REV_SNR 0                      ; bit[3:0]  of CPUID_EAX
%define A1_REV_SNR 1                      ; bit[3:0]  of CPUID_EAX
%define A2_REV_SNR 2                      ; bit[3:0]  of CPUID_EAX
%define A3_REV_SNR 3                      ; bit[3:0]  of CPUID_EAX
%define B0_REV_SNR 4                      ; bit[3:0]  of CPUID_EAX

%define A0_REV_CLX 0x05                   ; bit[3:0]  of CPUID_EAX
%define A0_REV_CPX 0x0A                   ; bit[3:0]  of CPUID_EAX

%define CPU_OPTIONS_BIT_DCU_MODE_SEL BIT0
%define CPU_OPTIONS_BIT_DEBUG_INTERFACE_EN BIT1

%define TXT_PUBLIC_BASE        0x0FED30000
%define TPM_STATUS_REG_ADDRESS 0xFED40000

; ICX/ICXD/SNR/SPR/GNR uses below buses as the default uncore buses.
%define BUS_U0 30    ; after CSR_LEGACY_CPUBUSNO_UBOX set, switch to this bus no
%define BUS_U1 31

;-------------------------------------------------------------------------------------------------------------
; CSR register address in legacy PCI format Bus:Dev:Func:Offset (used via IO port CF8/CFC access method)
;-------------------------------------------------------------------------------------------------------------
;Reg Header Date Stamp: 15ww22a
;----------------------------------------------------------------------------------------------------------------------------
;Bios Name                                        Address    CSR Name                                Bus    Device  Fun     Offset
;----------------------------------------------------------------------------------------------------------------------------
%define CSR_LT_CONTROL_CHABC_SKX      (1 << 20) | (29 << 15) | (1 << 12) | 0xD0 ;001E90D0h  LT_CONTROL_CHABC_SAD1_REG     1 29  1  0x00d0
%define CSR_LT_CONTROL_CHABC_ICX (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xF0 ;01FE90F0h  LT_CONTROL_CHABC_SAD1_REG    31 29  1  0x00f0
%define CSR_LT_CONTROL_CHABC_SPR (BUS_U1 << 20) | (29 << 15) | (1 << 12) | 0xF0 ;00FE90F0h  LT_CONTROL_CHABC_SAD1_REG    31 29  1  0x00f0
%define CSR_LT_CONTROL_CHABC_GNR (BUS_U0 << 20) | (13 << 15) | (1 << 12) | 0xF0 ;01E690F0h  LT_CONTROL_CHABC_SAD1_REG    30 13  1  0x00f0

;------------------------------------------------------------------------------
; macro to detect if CPU type == SKX
;------------------------------------------------------------------------------
; input :  None
; output:  E flag set if SKX CPU
;          E flag cleared if not SKX CPU
; Note: rax/rbx/rcx/rdx register will be destroyed !!!
%macro IS_SKX_CPU 0
    mov     eax, 1
    cpuid
    and     eax, 0xfffff               ; keep Family/Stepping info
    shr     eax, 4
    cmp     eax, CPU_FAMILY_SKX
%endmacro

;------------------------------------------------------------------------------
; macro to detect if CPU type == SKX
;------------------------------------------------------------------------------
; input :  None
; output:  ZF flag set if SKX CPU
;          ZF flag cleared if not SKX CPU
; Note: It's assumed that the CPUID information is stored in the XMM0 register
; it's done like that to avoid corrupting other registers.

%macro IS_SKX_CPU_FROM_XMM0 0
    movd  ecx, xmm0
    shr   ecx, 8
    and   ecx, 0xfffff               ; keep Family/Stepping info
    shr   ecx, 4
    cmp   ecx, CPU_FAMILY_SKX
%endmacro

;------------------------------------------------------------------------------
; macro to detect if CPU type == ICX
;------------------------------------------------------------------------------
; input :  None
; output:  ZF flag set if ICX CPU
;          ZF flag cleared if not ICX CPU
; Note: rax/rbx/rcx/rdx register will be destroyed !!!
%macro IS_ICX_CPU 0
    mov     eax, 1
    cpuid
    and     eax, 0xfffff               ; keep Family/Stepping info
    shr     eax, 4
    cmp     eax, CPU_FAMILY_ICX
%endmacro

;------------------------------------------------------------------------------
; macro to detect if CPU type == ICX
;------------------------------------------------------------------------------
; input :  None
; output:  ZF flag set if ICX CPU
;          ZF flag cleared if not ICX CPU
; Note: It's assumed that the CPUID information is stored in the XMM0 register
; it's done like that to avoid corrupting other registers.

%macro IS_ICX_CPU_FROM_XMM0 0
    movd  ecx, xmm0
    shr   ecx, 8
    and   ecx, 0xfffff               ; keep Family/Stepping info
    shr   ecx, 4
    cmp   ecx, CPU_FAMILY_ICX
%endmacro

;------------------------------------------------------------------------------
; macro to detect if CPU type == ICXD
;------------------------------------------------------------------------------
; input :  None
; output:  E flag set if ICXD CPU
;          E flag cleared if not ICXD CPU
; Note: rax/rbx/rcx/rdx register will be destroyed !!!
%macro IS_ICXD_CPU 0
  mov   eax,1
  cpuid
  and   eax, 0xfffff              ; keep Family/Stepping info
  shr   eax, 4
  cmp   eax, CPU_FAMILY_ICXD
%endmacro

;------------------------------------------------------------------------------
; macro to detect if CPU type == ICXD
;------------------------------------------------------------------------------
; input :  None
; output:  ZF flag set if ICXD CPU
;          ZF flag cleared if not ICXD CPU
; Note: It's assumed that the CPUID information is stored in the XMM0 register
; it's done like that to avoid corrupting other registers.

%macro IS_ICXD_CPU_FROM_XMM0 0
  movd  ecx, xmm0
  shr   ecx, 8
  and   ecx, 0xfffff              ; keep Family/Stepping info
  shr   ecx, 4
  cmp   ecx, CPU_FAMILY_ICXD
%endmacro

;------------------------------------------------------------------------------
; macro to detect if CPU type == SNR
;------------------------------------------------------------------------------
; input :  None
; output:  ZF flag set if SNR CPU
;          ZF flag cleared if not SNR CPU
; Note: rax/rbx/rcx/rdx register will be destroyed !!!
%macro IS_SNR_CPU 0
    mov     eax, 1
    cpuid
    and     eax, 0xfffff               ; keep Family/Stepping info
    shr     eax, 4
    cmp     eax, CPU_FAMILY_SNR
%endmacro

;------------------------------------------------------------------------------
; macro to detect if CPU type == SNR
;------------------------------------------------------------------------------
; input :  None
; output:  ZF flag set if SNR CPU
;          ZF flag cleared if not SNR CPU
; Note: It's assumed that the CPUID information is stored in the XMM0 register
; it's done like that to avoid corrupting other registers.
%macro IS_SNR_CPU_FROM_XMM0 0
    movd  ecx, xmm0
    shr   ecx, 8
    and   ecx, 0xfffff               ; keep Family/Stepping info
    shr   ecx, 4
    cmp   ecx, CPU_FAMILY_SNR
%endmacro

;------------------------------------------------------------------------------
; macro to detect if CPU type == SPRSP
;------------------------------------------------------------------------------
; input :  None
; output:  E flag set if SPR CPU
;          E flag cleared if not SPR CPU
; Note: rax/rbx/rcx/rdx register will be destroyed !!!
%macro IS_SPRSP_CPU 0
    mov     eax, 1
    cpuid
    and     eax, 0xfffff               ; keep Family/Stepping info
    shr     eax, 4
    cmp     eax, CPU_FAMILY_SPRSP
%endmacro

;------------------------------------------------------------------------------
; macro to detect if CPU type == SPRSP
;------------------------------------------------------------------------------
; input :  None
; output:  ZF flag set if SPR CPU
;          ZF flag cleared if not SPR CPU
; Note: It's assumed that the CPUID information is stored in the XMM0 register
; it's done like that to avoid corrupting other registers.

%macro IS_SPR_CPU_FROM_XMM0 0
    movd  ecx, xmm0
    shr   ecx, 8
    and   ecx, 0xfffff               ; keep Family/Stepping info
    shr   ecx, 4
    cmp   ecx, CPU_FAMILY_SPRSP
%endmacro

;------------------------------------------------------------------------------
; macro to detect if CPU type == GNRSP
;------------------------------------------------------------------------------
; input :  None
; output:  E flag set if GNR CPU
;          E flag cleared if not GNR CPU
; Note: rax/rbx/rcx/rdx register will be destroyed !!!
%macro IS_GNRSP_CPU 0
    mov     eax, 1
    cpuid
    and     eax, 0xfffff               ; keep Family/Stepping info
    shr     eax, 4
    cmp     eax, CPU_FAMILY_GNRSP
%endmacro

;------------------------------------------------------------------------------
; macro to detect if CPU type == GNRSP
;------------------------------------------------------------------------------
; input :  None
; output:  ZF flag set if GNR CPU
;          ZF flag cleared if not GNR CPU
; Note: It's assumed that the CPUID information is stored in the XMM0 register
; it's done like that to avoid corrupting other registers.

%macro IS_GNR_CPU_FROM_XMM0 0
    movd  ecx, xmm0
    shr   ecx, 8
    and   ecx, 0xfffff               ; keep Family/Stepping info
    shr   ecx, 4
    cmp   ecx, CPU_FAMILY_GNRSP
%endmacro

%macro BUSNO1_EBX 0
  and   ebx, 0xf00fffff
  or    ebx, LEGACY_BUS_NO1 << 20
%endmacro

;------------------------------------------------------------------------------
; macro to obtain if CPU steping from XMM0
;------------------------------------------------------------------------------
; input :  None
; output:  ZF flag set if SNR CPU
;          ZF flag cleared if not SNR CPU
; Note: It's assumed that the CPUID information is stored in the XMM0 register
; it's done like that to avoid corrupting other registers.
%macro GET_CPU_STEPPING_FROM_XMM0 0
  movd  ecx, xmm0
  shr   ecx, 8
  and   ecx, (BIT3 + BIT2 + BIT1 + BIT0)
%endmacro
