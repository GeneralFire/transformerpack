; @file
;
; @copyright
; INTEL CONFIDENTIAL
; Copyright 1999 - 2017 Intel Corporation. <BR>
;
; The source code contained or described herein and all documents related to the
; source code ("Material") are owned by Intel Corporation or its suppliers or
; licensors. Title to the Material remains with Intel Corporation or its suppliers
; and licensors. The Material may contain trade secrets and proprietary    and
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
;

%define MOVQIN0                        movq mm0, rax
%define MOVQOUT0                       movq rax, mm0
%define MOVDOUT0                       movd eax, mm0

%define MOVQIN1                        movq mm1, rax
%define MOVQOUT1                       movq rax, mm1
%define MOVDOUT1                       movd eax, mm1

%define MOVQIN2                        movq mm2, rax
%define MOVQOUT2                       movq rax, mm2
%define MOVDOUT2                       movd eax, mm2

%define MOVQIN3                        movq mm3, rax
%define MOVQOUT3                       movq rax, mm3
%define MOVDOUT3                       movd eax, mm3

%define MOVQIN4                        movq mm4, rax
%define MOVQOUT4                       movq rax, mm4
%define MOVDOUT4                       movd eax, mm4

%define MOVQIN5                        movq mm5, rax
%define MOVQOUT5                       movq rax, mm5
%define MOVDOUT5                       movd eax, mm5

%define MOVQIN6                        movq mm6, rax
%define MOVQOUT6                       movq rax, mm6
%define MOVDOUT6                       movd eax, mm6

%define MOVQIN7                        movq mm7, rax
%define MOVQOUT7                       movq rax, mm7
%define MOVDOUT7                       movd eax, mm7

;-----------------------------------------------------------------------------
;  Macro: PUSHA_64
;
;  Description: Saves all registers on stack
;
;  Input:   None
;
;  Output:  None
;-----------------------------------------------------------------------------
%macro PUSHA_64 0
    push    rsp
    push    rbp
    push    rax
    push    rbx
    push    rcx
    push    rdx
    push    rsi
    push    rdi
    push    r8
    push    r9
    push    r10
    push    r11
    push    r12
    push    r13
    push    r14
    push    r15
%endmacro

;-----------------------------------------------------------------------------
;  Macro: POPA_64
;
;  Description: Restores all registers from stack
;
;  Input:   None
;
;  Output:  None
;-----------------------------------------------------------------------------
%macro POPA_64 0
    pop    r15
    pop    r14
    pop    r13
    pop    r12
    pop    r11
    pop    r10
    pop    r9
    pop    r8
    pop    rdi
    pop    rsi
    pop    rdx
    pop    rcx
    pop    rbx
    pop    rax
    pop    rbp
    pop    rsp
%endmacro
