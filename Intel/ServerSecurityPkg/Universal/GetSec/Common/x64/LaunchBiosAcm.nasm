; @file
;
; @copyright
; INTEL CONFIDENTIAL
; Copyright 2007 - 2017 Intel Corporation. <BR>
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

section .text

%include "txt.inc"
%include "mmx64.inc"

;-----------------------------------------------------------------------------
;  Procedure:   LaunchBiosAcm
;
;  Input:       AcmBase  - Base address of LT BIOS ACM in flash
;               EsiValue -
;
;  Output:
;
;  Stack:       Available at beginning of routine, but routine goes 'stackless'
;
;  Registers:
;
;  Description: Setup GETSEC environment (protected mode, mtrrs, etc) and
;                 invoke GETSEC:ENTERACCS with requested BIOS ACM entry point.
;
;-----------------------------------------------------------------------------

%define ACM_SIZE_TO_CACHE xmm0
%define ACM_BASE_TO_CACHE xmm1
%define NEXT_MTRR_INDEX   xmm2
%define NEXT_MTRR_SIZE    xmm3

global ASM_PFX(LaunchBiosAcm)
ASM_PFX(LaunchBiosAcm):

    enter 0x30, 0
    %define BIOS_GDT [rbp-0x10]
    %define BIOS_IDT [rbp-0x20]

    ;
    ; Save the general purpose register state
    ;
    pushfq
    PUSHA_64

    ;
    ; Save the parameters passed to us
    ;
    mov     rax, rcx        ; save address of BIOS ACM in MMX0
    MOVQIN0
    mov     rax, cr3        ; save page table in MMX1
    MOVQIN1
    mov     rax, rdx        ; save value of ESI for GETSEC[ENTERACCS] in MMX2
    MOVQIN2
    or      rdx, rdx        ; have we been called to run SCLEAN?
    jz      BeginLaunch     ; if so, we don't need to save state.
    cmp     rbx, 5          ; have we been called to run ClearSecretsFlag?
    je      BeginLaunch     ; if so, we don't need to save state.

    sgdt    BIOS_GDT        ; save gdtr
    sidt    BIOS_IDT        ; save idtr

    lea     rax, BIOS_GDT   ; rax = address of saved gdtr
    MOVQIN3                 ; mm3 = address of saved gdtr

    mov     rax, rbp
    MOVQIN4                 ; mm4 = rbp
    xor     rax, rax
    mov     ax, ss
    MOVQIN5                 ; mm5 = ss
    mov     ax, cs
    MOVQIN6                 ; mm6 = cs

    ;
    ; Save control registers
    ;
    mov     rax, cr4
    push    rax
    mov     rax, cr0
    push    rax

    ;
    ; Save MISC ENABLE MSR
    ;
    mov     rcx, IA32_MISC_ENABLE
    rdmsr
    push    rax
    push    rdx

    ;
    ; Save IA32_EFER MSR
    ;
    mov     rcx, IA32_EFER_MSR
    rdmsr
    push    rax
    push    rdx

    ;
    ; Save MTRR
    ;
    mov     rcx, IA32_MTRR_CAP
    rdmsr
    and     rax, 0xFF
    shl     rax, 1
    mov     rcx, rax

SaveNextMtrr:
    add     rcx, IA32_MTRR_PHYSBASE0 - 1
    rdmsr
    push    rdx
    push    rax
    sub     rcx, IA32_MTRR_PHYSBASE0 - 1
    loop    SaveNextMtrr

    ; Save def MTRR type
    mov     rcx, IA32_MTRR_DEF_TYPE
    rdmsr
    push    rax
    push    rdx

    ; Save the rest of the segment registers
    xor     rax, rax
    mov     ax, gs
    push    rax
    mov     ax, fs
    push    rax
    mov     ax, es
    push    rax
    mov     ax, ds
    push    rax

    ;
    ; For reloading CS to Long Mode
    ;
    mov     rcx, cs
    shl     rcx, 32
    mov     rdx, ReloadCS   ; BUGBUG: Assume it is below 4G
    or      rcx, rdx
    push    rcx

    mov     rax, cr3        ; save page table in stack
    push    rax

BeginLaunch:
    ;
    ; Now that all of our state is on the stack, save esp
    ;
    mov     rax, rsp
    MOVQIN7                 ; mm7 = rsp

    ;--------------------------------
    ; Begin to launch ACM
    ;--------------------------------

    ;
    ; Enable SMXE, SSE and debug extensions
    ;
    mov         rax, cr4
    or          rax, CR4_OSFXSR_MASK + CR4_DE_MASK + CR4_SMXE
    mov         cr4, rax

    ;
    ; Disable cache
    ;
    mov         rax, cr0    ; set CR0:CD and CR0:NE, clear CR0:NW
    or          rax, CR0_CD_MASK | CR0_NE_MASK
    and         rax, ~ CR0_NW_MASK
    mov         cr0, rax

    ;
    ; Check to see how we should invalidate the cache
    ;
    MOVQOUT2
    or      rax, rax        ; have we been called to run SCLEAN?
    jz      ScleanInvd
    wbinvd                  ; Nope, SCHECK.  Writeback is necessary
    jmp     OverScleanInvd

ScleanInvd:
    invd                    ; Yep, SCLEAN. Invalidate the cache

OverScleanInvd:
    ;
    ; Disable MTRRs, set Default Type to UC
    ;
    mov     rcx, IA32_MTRR_DEF_TYPE
    xor     rax, rax
    xor     rdx, rdx
    wrmsr

    ;
    ; Clear all of the Variable MTRRs
    ;
    mov     rcx, IA32_MTRR_CAP
    rdmsr
    and     rax, 0xFF
    shl     rax, 1
    mov     rcx, rax
    xor     rax, rax
    xor     rdx, rdx
ClearMtrrContinue:
    add     rcx, IA32_MTRR_PHYSBASE0 - 1
    wrmsr
    sub     rcx, IA32_MTRR_PHYSBASE0 - 1
    loop    ClearMtrrContinue

    ;
    ; Determine size of AC module
    ;
    MOVQOUT0                            ; rax = AcmBase
    mov         rsi, rax                ; rsi = AcmBase
    mov         rax, [rsi+ACMOD_SIZE]   ; rax = size of ACM
    shl         rax, 2                  ;  ...in bytes (ACM header has size in dwords)

    ;
    ; Round up to page size
    ;
    mov         rcx, rax                        ; Save
    and         rcx, 0xFFFFF000                 ; Number of pages in AC module
    and         rax, 0xFFF                      ; Number of "less-than-page" bytes
    jz          rounded
    mov         rax, 0x1000                      ; Add the whole page size

rounded:
    add         rax, rcx                        ; rax = rounded up AC module size
    mov         rbx, rax                        ; rbx = rounded up AC module size

    ;
    ; Initialize "locals"
    ;
    xor     rcx, rcx
    movd    NEXT_MTRR_INDEX, ecx    ; Start from MTRR0

    ;
    ; Save remaining size to cache
    ;
    movd    ACM_SIZE_TO_CACHE, ebx  ; Size of ACM that must be cached
    movd    ACM_BASE_TO_CACHE, esi  ; Base ACM address

    ; Configure MTRR_PHYS_MASK_HIGH for proper addressing above 4GB
    ; based on the physical address size supported for this processor
    ; This is based on read from CPUID EAX = 080000008h, EAX bits [7:0]
    ;
    ; Examples:
    ;  MTRR_PHYS_MASK_HIGH = 00000000Fh  For 36 bit addressing
    ;  MTRR_PHYS_MASK_HIGH = 0000000FFh  For 40 bit addressing
    ; 
    mov     eax, 80000008h                ; Address sizes leaf
    cpuid
    sub     al, 32
    movzx   eax, al
    xor     esi, esi
    bts     esi, eax
    dec     esi                           ; esi <- MTRR_PHYS_MASK_HIGH
    mov     ebp, esi                      ; Save

nextMtrr:
    ;
    ; Get remaining size to cache
    ;
    xor     rax, rax
    movd    eax, ACM_SIZE_TO_CACHE
    and     rax, rax
    jz      done                    ; If no left size - we are done
    ;
    ; Determine next size to cache.
    ; We start from bottom up. Use the following algorythm:
    ; 1. Get our own alignment. Max size we can cache equals to our alignment
    ; 2. Determine what is bigger - alignment or remaining size to cache.
    ;    If aligment is bigger - cache it.
    ;      Adjust remaing size to cache and base address
    ;      Loop to 1.
    ;    If remaining size to cache is bigger
    ;      Determine the biggest 2^N part of it and cache it.
    ;      Adjust remaing size to cache and base address
    ;      Loop to 1.
    ; 3. End when there is no left size to cache or no left MTRRs
    ;
    xor     rsi, rsi
    movd    esi, ACM_BASE_TO_CACHE
    bsf     rcx, rsi                ; Get index of lowest bit set in base address
    ;
    ; Convert index into size to be cached by next MTRR
    ;
    mov     rdx, 0x1
    shl     rdx, cl                 ; Alignment is in rdx
    cmp     rdx, rax                ; What is bigger, alignment or remaining size?
    jbe     gotSize                 ; JIf aligment is less
    ;
    ; Remaining size is bigger. Get the biggest part of it, 2^N in size
    ;
    bsr     rcx, rax                ; Get index of highest set bit
    ;
    ; Convert index into size to be cached by next MTRR
    ;
    mov     rdx, 1
    shl     rdx, cl                 ; Size to cache

gotSize:
    mov     rax, rdx
    movd    NEXT_MTRR_SIZE, eax     ; Save

    ;
    ; Compute MTRR mask value:  Mask = NOT (Size - 1)
    ;
    dec     rax                     ; rax - size to cache less one byte
    not     eax                     ; eax contains low 32 bits of mask
    or      rax, MTRR_VALID         ; Set valid bit
    ;
    ; Program mask register
    ;
    mov     rcx, IA32_MTRR_PHYSMASK0 ; setup variable mtrr
    xor     rbx, rbx
    movd    ebx, NEXT_MTRR_INDEX
    add     rcx, rbx
 ;   mov     rdx, 0Fh       ; 8K range (FFFFFFE800)
    mov     edx, ebp        ; ebp <- MTRR_PHYS_MASK_HIGH
    wrmsr
    ;
    ; Program base register
    ;
    xor     rdx, rdx
    mov     rcx, IA32_MTRR_PHYSBASE0 ; setup variable mtrr
    add     ecx, ebx                 ; ebx is still NEXT_MTRR_INDEX

    xor     rax, rax
    movd    eax, ACM_BASE_TO_CACHE
    or      eax, WB                 ; set type to write back
    wrmsr
    ;
    ; Advance and loop
    ; Reduce remaining size to cache
    ;
    movd    ebx, ACM_SIZE_TO_CACHE
    movd    eax, NEXT_MTRR_SIZE
    sub     ebx, eax
    movd    ACM_SIZE_TO_CACHE, ebx

    ;
    ; Increment MTRR index
    ;
    movd    ebx, NEXT_MTRR_INDEX
    add     ebx, 2
    movd    NEXT_MTRR_INDEX, ebx
    ;
    ; Increment base address to cache
    ;
    movd    ebx, ACM_BASE_TO_CACHE
    movd    eax, NEXT_MTRR_SIZE
    add     ebx, eax
    movd    ACM_BASE_TO_CACHE, ebx

    jmp     nextMtrr
done:

    ;
    ; Re-enable Variable MTRRs
    ;
    mov     rcx, IA32_MTRR_DEF_TYPE
    xor     rdx, rdx
    mov     rax, MTRR_ENABLE
    wrmsr

    ;
    ; Re-enable cache
    ;
    mov     rax, cr0
    and     rax, ~ CR0_CD_MASK
    mov     cr0, rax

    ;
    ; Clean all MCi_STATUS MSR registers
    ; SCLEAN will generate GPF otherwise
    ;
    mov     rcx, MCG_CAP
    rdmsr
    movzx   rbx, al                 ; rbx = MCR bank count
    xor     rax, rax                ; Write 0 into all MCi_STATUS registers
    xor     rdx, rdx
    mov     rcx, MC0_STATUS

McaErrorCleanLoopStart:
    wrmsr
    dec     rbx
    jz      CallGetsec
    add     rcx, 4                  ; rcx = number of MSRs per bank
    jmp     McaErrorCleanLoopStart

CallGetsec:
    ;
    ; Change to Compatible Segment
    ;
    mov     rcx, 0x20               ; BUGBUG: Hardcode Compatible mode segment
    shl     rcx, 32
    mov     rdx, Compatible         ; BUGBUG: assume address < 4G
    or      rcx, rdx
    push    rcx
    retf

Compatible:
    ;
    ; disable paging
    ;
    mov     rax, cr0
    btr     eax, 31 ; set PG=0
    mov     cr0, rax
    ;
    ; set EFER.LME = 0 to leave long mode
    ;
    mov     ecx, 0xc0000080 ; EFER MSR number.
    rdmsr                   ; Read EFER.
    btr     eax, 8          ; Set LME=0.
    wrmsr                   ; Write EFER.

    ;
    ; Call GETSEC[ENTERACCS]
    ;
    MOVDOUT2                        ; eax = ACM function
    mov     esi, eax                ; esi = ACM function
    MOVDOUT0                        ; eax = AcmBase
    mov     ebx, eax                ; ebx = AcmBase
    mov     ecx, DWORD [rbx+ACMOD_SIZE]   ; ecx = size of ACM in dwords
    shl     ecx, 2                            ; ecx = size of ACM in bytes
    xor     edx, edx
    xor     edi, edi
    mov     eax, ENTERACCS          ; eax = ENTERACCS
    getsec

    ;
    ; Check if we need to restore BIOS state and configuration
    ;
    MOVDOUT2
    or      eax, eax        ; Have we been called to run SCLEAN?
    jz      ResetSystem     ; Jump to ResetSystem, no-need to restore state,
                            ; because system will reset soon.
    cmp     eax, 5          ; Have we been called to run ClearSecretsFlag?
    je      ResetSystem     ; Jump to ResetSystem, no-need to restore state,
                            ; because system will reset soon.

    ;
    ; Reload the GDTR
    ;
    MOVDOUT3                ; ebx = the address of our local variable
    lgdt    [rax]

    ;
    ; Restore the stack
    ;
    MOVDOUT4
    mov     ebp, eax        ; restore ebp
    MOVDOUT7
    mov     esp, eax        ; restore esp
    MOVDOUT5
    mov     ss, ax          ; restore ss

    ;
    ; Enable protected mode
    ;
    mov     ecx, 0x33
    mov     cr0, rcx

    ;
    ; Enable PAE in CR4
    ;
    mov     ecx, 0x668 | CR4_SMXE
    mov     cr4, rcx

    ;
    ; Reload CR3
    ;
    pop     rax
    mov     cr3, rax
    pop     rax       ; pop the redandant

    ;
    ; Setup re-enable of paging
    ;
    mov     rbx, cr0
    bts     ebx, 31

    ;
    ; Set EFER.LME to re-enable ia32-e
    ;
    mov     ecx, 0xc0000080
    xor     edx, edx
    mov     eax, 0x500
    wrmsr

    ;
    ; Re-enable paging
    ;
    mov     cr0, rbx

    ;
    ; Now we're in Compatibility mode
    ;
    MOVDOUT3                ; rax = the address of our local variable
    lgdt    [rax]
    lidt    [rax-0x10]

    ;
    ; Reload cs register
    ;
    retf
ReloadCS:
    ;
    ; Now we're in Long Mode
    ;
    ;
    ; Restore the rest of the segment registers
    ;
    pop     rax
    mov     ds, ax
    pop     rax
    mov     es, ax
    pop     rax
    mov     fs, ax
    pop     rax
    mov     gs, ax

    ;
    ; Clear out the cache
    ;
    mov         rax, cr0            ; Set CR0:CD and CR0:NE, clear CR0:NW
    or          rax, CR0_CD_MASK | CR0_NE_MASK
    and         rax, ~ CR0_NW_MASK
    mov         cr0, rax
    wbinvd                          ; Flush and invalidate the cache

    ;
    ; Restore def MTRR type
    ;
    mov     rcx, IA32_MTRR_DEF_TYPE
    pop     rdx
    pop     rax
    wrmsr

    ;
    ; Reset Variable MTRRs to Pre-GETSEC state
    ;
    mov     rcx, IA32_MTRR_CAP
    rdmsr
    and     rax, 0xFF
    shl     rax, 1
    mov     rcx, rax
    mov     rbx, rax
    shl     rbx, 4     ; rbx = the total length for MTRR register
    add     rsp, rbx   ; adjust rsp to skip MTRR register
    mov     rbx, rsp
    sub     rbx, 8     ; let rbx point to head of MTRR register now

RestoreNextMtrr:
    add     rcx, IA32_MTRR_PHYSBASE0 - 1
    mov     rdx, [rbx]
    mov     rax, [rbx - 8]
    wrmsr
    sub     rcx, IA32_MTRR_PHYSBASE0 - 1
    sub     rbx, 16
    loop    RestoreNextMtrr

    ;
    ; Restore IA32_EFER MSR
    ;
    pop     rdx
    pop     rax
    mov     rcx, IA32_EFER_MSR
    wrmsr

    ;
    ; Restore MISC ENABLE MSR
    ;
    pop     rdx
    pop     rax
    mov     rcx, IA32_MISC_ENABLE
    wrmsr

    ;
    ; Restore control registers
    ;
    pop     rax
    mov     cr0, rax
    pop     rax
    mov     cr4, rax

    wbinvd                          ; Flush and invalidate the cache

    POPA_64
    popfq

    emms
    leave
    ret 0

ResetSystem:
    mov     dx, 0xCF9
    mov     al, 0xA
    out     dx, al
    jmp     .0
.0:
    jmp     .1
.1:
    mov     al, 0xE
    out     dx, al
DeadLoop:
    jmp     $           ; should never get here, but just in case
