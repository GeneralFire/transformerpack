; @file
;
; @copyright
; INTEL CONFIDENTIAL
; Copyright 2006 - 2008 Intel Corporation. <BR>
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
; InternalX86DisablePaging64 (
;   IN      UINT16                    Cs,
;   IN      UINT32                    EntryPoint,
;   IN      UINT32                    Context1,  OPTIONAL
;   IN      UINT32                    Context2,  OPTIONAL
;   IN      UINT32                    NewStack
;   );
;------------------------------------------------------------------------------
global ASM_PFX(InternalX86DisablePaging64)
ASM_PFX(InternalX86DisablePaging64):
    cli
    lea     rsi, [.0]                     ; rsi <- The start address of transition code
    mov     edi, [rsp + 0x28]            ; rdi <- New stack
    lea     rax, [mTransitionEnd]         ; rax <- end of transition code
    sub     rax, rsi                    ; rax <- The size of transition piece code
    add     rax, 4                      ; Round RAX up to the next 4 byte boundary
    and     al, 0xfc
    sub     rdi, rax                    ; rdi <- Use stack to hold transition code
    mov     r10d, edi                   ; r10 <- The start address of transicition code below 4G
    push    rcx                         ; save rcx to stack
    mov     rcx, rax                    ; rcx <- The size of transition piece code
    rep     movsb                       ; copy transition code to top of new stack which must be below 4GB
    pop     rcx                         ; restore rcx

    mov     esi, r8d
    mov     edi, r9d
    mov     eax, r10d                   ; eax <- start of the transition code on the stack
    sub     eax, 4                      ; eax <- One slot below transition code on the stack
    push    rcx                         ; push Cs to stack
    push    r10                         ; push address of tansition code on stack
    DB      0x48                         ; prefix to composite "retq" with next "retf"
    retf                                ; Use far return to load CS register from stack

; Start of transition code
.0:
    mov     esp, eax                    ; set up new stack
    mov     rax, cr0
    btr     eax, 31                     ; Clear CR0.PG
    mov     cr0, rax                    ; disable paging and caches

    mov     ebx, edx                    ; save EntryPoint to rbx, for rdmsr will overwrite rdx
    mov     ecx, 0xc0000080
    rdmsr
    and     ah, ~ 1                   ; clear LME
    wrmsr
    mov     rax, cr4
    and     al, ~ (1 << 5)           ; clear PAE
    mov     cr4, rax
    push    rdi                         ; push Context2
    push    rsi                         ; push Context1
    call    rbx                         ; transfer control to EntryPoint
    hlt                                 ; no one should get here

mTransitionEnd:

