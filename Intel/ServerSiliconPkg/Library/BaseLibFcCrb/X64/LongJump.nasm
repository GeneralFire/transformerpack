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

    DEFAULT REL
    SECTION .text

;------------------------------------------------------------------------------
; VOID
; EFIAPI
; InternalLongJump (
;   IN      BASE_LIBRARY_JUMP_BUFFER  *JumpBuffer,
;   IN      UINTN                     Value
;   );
;------------------------------------------------------------------------------
global ASM_PFX(InternalLongJump)
ASM_PFX(InternalLongJump):
    mov     rbx, [rcx]
    mov     rsp, [rcx + 8]
    mov     rbp, [rcx + 0x10]
    mov     rdi, [rcx + 0x18]
    mov     rsi, [rcx + 0x20]
    mov     r12, [rcx + 0x28]
    mov     r13, [rcx + 0x30]
    mov     r14, [rcx + 0x38]
    mov     r15, [rcx + 0x40]
    ; load non-volatile fp registers
    ldmxcsr [rcx + 0x50]
    movdqu  xmm6,  [rcx + 0x58]
    movdqu  xmm7,  [rcx + 0x68]
    movdqu  xmm8,  [rcx + 0x78]
    movdqu  xmm9,  [rcx + 0x88]
    movdqu  xmm10, [rcx + 0x98]
    movdqu  xmm11, [rcx + 0xA8]
    movdqu  xmm12, [rcx + 0xB8]
    movdqu  xmm13, [rcx + 0xC8]
    movdqu  xmm14, [rcx + 0xD8]
    movdqu  xmm15, [rcx + 0xE8]
    mov     rax, rdx               ; set return value
    jmp     qword [rcx + 0x48]

