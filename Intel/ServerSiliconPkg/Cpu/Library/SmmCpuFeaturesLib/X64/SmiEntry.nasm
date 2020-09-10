; @file
;
; @copyright
; INTEL CONFIDENTIAL
; Copyright 2017 Intel Corporation. <BR>
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
; Variables referrenced by C code
;

%define MSR_IA32_MISC_ENABLE 0x1A0
%define MSR_EFER 0xc0000080
%define MSR_EFER_XD 0x800

%define MSR_SMM_SUPOVR_STATE_LOCK          0x141
%define   MSR_SMM_SUPOVR_STATE_LOCK_PAGING_STATE_ENABLE_BIT  0x1
%define   MSR_SMM_SUPOVR_STATE_LOCK_SMBASE_ENABLE_BIT        0x2

;
; Constants relating to PROCESSOR_SMM_DESCRIPTOR
;
%define DSC_OFFSET 0xfb00
%define DSC_GDTPTR 0x30
%define DSC_GDTSIZ 0x38
%define DSC_CS 14
%define DSC_DS 16
%define DSC_SS 18
%define DSC_OTHERSEG 20

;
; Constants relating to CPU State Save Area
;
%define SSM_DR6 0xffd0
%define SSM_DR7 0xffc8

%define PROTECT_MODE_CS 0x8
%define PROTECT_MODE_DS 0x20
%define LONG_MODE_CS 0x38
%define TSS_SEGMENT 0x40
%define GDT_SIZE 0x50

SECTION .data

extern ASM_PFX(SmiRendezvous)
extern ASM_PFX(SmmFeatureCpuSmmDebugEntry)
extern ASM_PFX(SmmFeatureCpuSmmDebugExit)

global ASM_PFX(gSmmFeatureSmiHandlerIdtr)
global ASM_PFX(gcSmmFeatureSmiHandlerTemplate)
global ASM_PFX(gPMStackDesc)
global ASM_PFX(gcSmmFeatureSmiHandlerSize)
global ASM_PFX(gcSmiPMHandlerSize)
global ASM_PFX(gSmmFeatureSmbase)
global ASM_PFX(gSmmFeatureSmiStack)
global ASM_PFX(gcSmiPMHandlerTemplate)
global ASM_PFX(gProtModeSmbase)
global ASM_PFX(gSmmFeatureSmiCr3)
global ASM_PFX(gSmmFeatureXdSupported)
global ASM_PFX(gSmmFeatureEnable5LevelPaging)

align 16
ASM_PFX(gSmmFeatureSmiHandlerIdtr):
    DW 0
    DQ 0

gPMStackDescOffset equ ASM_PFX(gPMStackDesc) - _SmiPMEntryPoint
gSmmFeatureSmbase EQU SmmFeatureSmbasePatch - 4
gSmmFeatureSmiStack EQU SmmFeatureSmiStackPatch - 4
gProtModeSmbase EQU ProtModeSmbasePatch - 4
gSmmFeatureSmiCr3 EQU SmmFeatureSmiCr3Patch - 4
gSmmFeatureXdSupported EQU SmmFeatureXdSupportedPatch - 1
gSmmFeatureEnable5LevelPaging EQU SmmFeatureEnable5LevelPagingPatch - 1

    DEFAULT REL
    SECTION .text

BITS 16
ASM_PFX(gcSmmFeatureSmiHandlerTemplate):
_SmiEntryPoint:
    mov     bx, _GdtDesc - _SmiEntryPoint + 0x8000
    mov     ax,[cs:DSC_OFFSET + DSC_GDTSIZ]
    dec     ax
    mov     [cs:bx], ax
    mov     eax, [cs:DSC_OFFSET + DSC_GDTPTR]
    mov     [cs:bx + 2], eax
o32 lgdt    [cs:bx]                       ; lgdt fword ptr cs:[bx]
    mov     ax, PROTECT_MODE_CS
    mov     [cs:bx-0x2],ax
    mov     edi, strict dword 0
SmmFeatureSmbasePatch:
    lea     eax, [edi + (@ProtectedMode - _SmiEntryPoint) + 0x8000]
    mov     [cs:bx-0x6],eax
    mov     ebx, cr0
    and     ebx, 0x9ffafff3
    or      ebx, 0x23
    mov     cr0, ebx
    jmp     dword 0x0:0x0
_GdtDesc:
    DW 0
    DD 0

BITS 32
@ProtectedMode:
    mov     ax, PROTECT_MODE_DS
o16 mov     ds, ax
o16 mov     es, ax
o16 mov     fs, ax
o16 mov     gs, ax
o16 mov     ss, ax
    mov     esp, strict dword 0
SmmFeatureSmiStackPatch:
    jmp     ProtFlatMode

ASM_PFX(gcSmiPMHandlerTemplate):
_SmiPMEntryPoint:
; if SMM PROT MODE feature is ok, processor will break here with 32bit protected mode
    mov     edi, strict dword 0
ProtModeSmbasePatch:
    ; reload SS:ESP, it should be done in one instruction
    mov     eax, gPMStackDescOffset
    add     eax, edi
    add     eax, 0x8000
    lss     esp, [eax]

    ; reload CS:EIP
    mov     eax, edi
    add     eax, ProtFlatMode - _SmiPMEntryPoint + 0x8000
    mov     dx, PROTECT_MODE_CS
    mov     [eax - 2], dx
    mov     [eax - 6], eax
    
    sgdt    [esp - 6]
    mov     ebp, [esp - 4]              ; save GDT base into ebp

    ; set cr0 value that is the same with real mode
    mov     ebx, cr0
    and     ebx, 0x9ffafff3
    or      ebx, 0x00000023
    mov     cr0, ebx
    jmp     strict dword 0x0:0x0        ; jmp ProtFlatMode

ProtFlatMode:
    mov eax, strict dword 0
SmmFeatureSmiCr3Patch:
    mov     cr3, eax
    mov     eax, 0x668                  ; as cr4.PGE is not set here, refresh cr3

    mov     cl, strict byte 0
SmmFeatureEnable5LevelPagingPatch:
    cmp     cl, 0
    je      SkipEnable5LevelPaging
    ;
    ; Enable 5-Level Paging bit
    ;
    bts     eax, 12

SkipEnable5LevelPaging:
    mov     cr4, eax                    ; in PreModifyMtrrs() to flush TLB.
; Load TSS
    sub     esp, 8                      ; reserve room in stack
    sgdt    [esp]
    mov     eax, [esp + 2]              ; eax = GDT base
    add     esp, 8
    mov     dl, 0x89
    mov     [eax + TSS_SEGMENT + 5], dl ; clear busy flag
    mov     eax, TSS_SEGMENT
    ltr     ax

; enable NXE if supported
    mov     al, strict byte 1
SmmFeatureXdSupportedPatch:
    cmp     al, 0
    jz      @SkipXd
;
; Check XD disable bit
;
    mov     ecx, MSR_IA32_MISC_ENABLE
    rdmsr
    sub     esp, 4
    push    edx                        ; save MSR_IA32_MISC_ENABLE[63-32]
    test    edx, BIT2                  ; MSR_IA32_MISC_ENABLE[34]
    jz      .0
    and     dx, 0xFFFB                 ; clear XD Disable bit if it is set
    wrmsr
.0:
    mov     ecx, MSR_EFER
    rdmsr
    or      ax, MSR_EFER_XD            ; enable NXE
    wrmsr
    jmp     @XdDone
@SkipXd:
    sub     esp, 8
@XdDone:

; Switch into @LongMode
    push    LONG_MODE_CS               ; push cs hardcore here
    call    Base                       ; push return address for retf later
Base:
    add     dword [esp], @LongMode - Base; offset for far retf, seg is the 1st arg
    mov     ecx, MSR_EFER
    rdmsr
    or      ah, 1                      ; enable LME
    wrmsr
    mov     ebx, cr0
    or      ebx, 0x80010023            ; enable paging + WP + NE + MP + PE
    mov     cr0, ebx
    retf

BITS 64
@LongMode:                             ; long mode (64-bit code) starts here
    mov     rax, ASM_PFX(gSmmFeatureSmiHandlerIdtr)
    lidt    [rax]
    lea     ebx, [rdi + DSC_OFFSET]
    mov     ax, [rbx + DSC_DS]
    mov     ds, eax
    mov     ax, [rbx + DSC_OTHERSEG]
    mov     es, eax
    mov     fs, eax
    mov     gs, eax
    mov     ax, [rbx + DSC_SS]
    mov     ss, eax
;   jmp     _SmmFeatureSmiHandler                 ; instruction is not needed

_SmmFeatureSmiHandler:
    mov     rbx, [rsp + 0x8]             ; rcx <- CpuIndex

    ;
    ; Save FP registers
    ;
    sub     rsp, 0x200
    fxsave64 [rsp]

    add     rsp, -0x20

    mov     rcx, rbx
    mov     rax, SmmFeatureCpuSmmDebugEntry
    call    rax

    mov     rax, SmmSupovrStateLock
    call    rax

    mov     rcx, rbx
    mov     rax, SmiRendezvous          ; rax <- absolute addr of SmiRedezvous
    call    rax

    mov     rcx, rbx
    mov     rax, SmmFeatureCpuSmmDebugExit
    call    rax

    add     rsp, 0x20

    ;
    ; Restore FP registers
    ;
    fxrstor64 [rsp]

    add     rsp, 0x200

    mov     rax, ASM_PFX(gSmmFeatureXdSupported)
    mov     al, [rax]
    cmp     al, 0
    jz      .1
    pop     rdx                       ; get saved MSR_IA32_MISC_ENABLE[63-32]
    test    edx, BIT2
    jz      .1
    mov     ecx, MSR_IA32_MISC_ENABLE
    rdmsr
    or      dx, BIT2                  ; set XD Disable bit if it was set before entering into SMM
    wrmsr

.1:
    rsm

global ASM_PFX(SmmSupovrStateLock)
ASM_PFX(SmmSupovrStateLock):
    ret     ; it will be patched to NOP in SmmCpuFeaturesCompleteSmmReadyToLock()
    mov     ecx, MSR_SMM_SUPOVR_STATE_LOCK
    mov     edx, 0
    mov     eax, MSR_SMM_SUPOVR_STATE_LOCK_PAGING_STATE_ENABLE_BIT + MSR_SMM_SUPOVR_STATE_LOCK_SMBASE_ENABLE_BIT
    wrmsr
    ret

align 16
; used by LSS
ASM_PFX(gPMStackDesc):
    DD 0 ; to be fixed
    DD PROTECT_MODE_DS

ASM_PFX(gcSmmFeatureSmiHandlerSize): DW $ - _SmiEntryPoint
ASM_PFX(gcSmiPMHandlerSize): DW $ - _SmiPMEntryPoint
