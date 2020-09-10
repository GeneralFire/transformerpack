; @file
;
; @copyright
; INTEL CONFIDENTIAL
; Copyright 2006 Intel Corporation. <BR>
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

    .code

EXTERN   InternalAssertJumpBuffer:PROC

;------------------------------------------------------------------------------
; UINTN
; EFIAPI
; SetJump (
;   OUT     BASE_LIBRARY_JUMP_BUFFER  *JumpBuffer
;   );
;------------------------------------------------------------------------------
SetJump     PROC
    push    rcx
    add     rsp, -20h
    call    InternalAssertJumpBuffer
    add     rsp, 20h
    pop     rcx
    pop     rdx
    mov     [rcx], rbx
    mov     [rcx + 8], rsp
    mov     [rcx + 10h], rbp
    mov     [rcx + 18h], rdi
    mov     [rcx + 20h], rsi
    mov     [rcx + 28h], r12
    mov     [rcx + 30h], r13
    mov     [rcx + 38h], r14
    mov     [rcx + 40h], r15
    mov     [rcx + 48h], rdx
    ; save non-volatile fp registers
    stmxcsr [rcx + 50h]
    movdqu  [rcx + 58h], xmm6
    movdqu  [rcx + 68h], xmm7
    movdqu  [rcx + 78h], xmm8
    movdqu  [rcx + 88h], xmm9
    movdqu  [rcx + 98h], xmm10
    movdqu  [rcx + 0A8h], xmm11
    movdqu  [rcx + 0B8h], xmm12
    movdqu  [rcx + 0C8h], xmm13
    movdqu  [rcx + 0D8h], xmm14
    movdqu  [rcx + 0E8h], xmm15
    xor     rax, rax
    jmp     rdx
SetJump     ENDP

    END
