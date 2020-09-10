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

    SECTION .text

;------------------------------------------------------------------------------
; VOID
; EFIAPI
; InternalX86EnablePaging64 (
;   IN      UINT16                    Cs,
;   IN      UINT64                    EntryPoint,
;   IN      UINT64                    Context1,  OPTIONAL
;   IN      UINT64                    Context2,  OPTIONAL
;   IN      UINT64                    NewStack
;   );
;------------------------------------------------------------------------------
global ASM_PFX(InternalX86EnablePaging64)
ASM_PFX(InternalX86EnablePaging64):
    cli
    mov     DWORD [esp], .0         ; offset for far retf, seg is the 1st arg
    mov     eax, cr4
    or      al, (1 << 5)
    mov     cr4, eax                    ; enable PAE
    mov     ecx, 0xc0000080
    rdmsr
    or      ah, 1                       ; set LME
    wrmsr
    mov     eax, cr0
    bts     eax, 31                     ; set PG
    mov     cr0, eax                    ; enable paging
    retf                                ; topmost 2 dwords hold the address
.0:
    DB      0x67, 0x48                    ; 32-bit address size, 64-bit operand size
    mov     ebx, [esp]                  ; mov rbx, [esp]
    DB      0x67, 0x48
    mov     ecx, [esp + 8]              ; mov rcx, [esp + 8]
    DB      0x67, 0x48
    mov     edx, [esp + 0x10]            ; mov rdx, [esp + 10h]
    DB      0x67, 0x48
    mov     esp, [esp + 0x18]            ; mov rsp, [esp + 18h]
    DB      0x48
    add     esp, -0x20                   ; add rsp, -20h
    call    ebx                         ; call rbx
    hlt                                 ; no one should get here

