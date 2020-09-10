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

%define MSR_IA32_MISC_ENABLE 0x1A0
%define MSR_EFER 0xc0000080
%define MSR_EFER_XD 0x800

%define MSR_SMM_SUPOVR_STATE_LOCK          0x141
%define   MSR_SMM_SUPOVR_STATE_LOCK_PAGING_STATE_ENABLE_BIT  0x1
%define   MSR_SMM_SUPOVR_STATE_LOCK_SMBASE_ENABLE_BIT        0x2

%define DSC_OFFSET 0xfb00
%define DSC_GDTPTR 0x30
%define DSC_GDTSIZ 0x38
%define DSC_CS 14
%define DSC_DS 16
%define DSC_SS 18
%define DSC_OTHERSEG 20

%define PROTECT_MODE_CS 0x8
%define PROTECT_MODE_DS 0x20
%define TSS_SEGMENT 0x40

SECTION .data

extern ASM_PFX(SmiRendezvous)
extern ASM_PFX(SmmFeatureCpuSmmDebugEntry)
extern ASM_PFX(SmmFeatureCpuSmmDebugExit)
extern ASM_PFX(FeaturePcdGet (PcdCpuSmmStackGuard))

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

align 16
ASM_PFX(gSmmFeatureSmiHandlerIdtr):
    DW 0
    DD 0

gPMStackDescOffset equ ASM_PFX(gPMStackDesc) - _SmiPMEntryPoint
gSmmFeatureSmbase EQU SmmFeatureSmbasePatch - 4
gSmmFeatureSmiStack EQU SmmFeatureSmiStackPatch - 4
gProtModeSmbase EQU ProtModeSmbasePatch - 4
gSmmFeatureSmiCr3 EQU SmmFeatureSmiCr3Patch - 4
gSmmFeatureXdSupported EQU SmmFeatureXdSupportedPatch - 1

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
    mov     ebp, eax                      ; ebp = GDT base
o32 lgdt    [cs:bx]                       ; lgdt fword ptr cs:[bx]
    mov     ax, PROTECT_MODE_CS
    mov     [cs:bx-0x2],ax
o32 mov     edi, strict dword 0
SmmFeatureSmbasePatch:
    lea     eax, [edi + (@32bit - _SmiEntryPoint) + 0x8000]
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
@32bit:
    mov     ax, PROTECT_MODE_DS
o16 mov     ds, ax
o16 mov     es, ax
o16 mov     fs, ax
o16 mov     gs, ax
o16 mov     ss, ax
    mov esp, strict dword 0
SmmFeatureSmiStackPatch:
    mov     eax, ASM_PFX(gSmmFeatureSmiHandlerIdtr)
    lidt    [eax]
    jmp     ProtFlatMode

ASM_PFX(gcSmiPMHandlerTemplate):
_SmiPMEntryPoint:
; if SMM PROT MODE feature is ok, processor will break here with 32bit protected mode
    mov edi, strict dword 0
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
    mov     eax, strict dword 0
SmmFeatureSmiCr3Patch:
    mov     cr3, eax
;
; Need to test for CR4 specific bit support
;
    mov     eax, 1
    cpuid                               ; use CPUID to determine if specific CR4 bits are supported
    xor     eax, eax                    ; Clear EAX
    test    edx, BIT2                   ; Check for DE capabilities
    jz      .0
    or      eax, BIT3
.0:
    test    edx, BIT6                   ; Check for PAE capabilities
    jz      .1
    or      eax, BIT5
.1:
    test    edx, BIT7                   ; Check for MCE capabilities
    jz      .2
    or      eax, BIT6
.2:
    test    edx, BIT24                  ; Check for FXSR capabilities
    jz      .3
    or      eax, BIT9
.3:
    test    edx, BIT25                  ; Check for SSE capabilities
    jz      .4
    or      eax, BIT10
.4:
    mov     cr4, eax                    ; in PreModifyMtrrs() to flush TLB.

    cmp     byte [dword ASM_PFX(FeaturePcdGet (PcdCpuSmmStackGuard))], 0
    jz      .6

; Load TSS
    mov     byte [ebp + TSS_SEGMENT + 5], 0x89 ; clear busy flag
    mov     eax, TSS_SEGMENT
    ltr     ax
.6:
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
    push    edx                        ; save MSR_IA32_MISC_ENABLE[63-32]
    test    edx, BIT2                  ; MSR_IA32_MISC_ENABLE[34]
    jz      .5
    and     dx, 0xFFFB                 ; clear XD Disable bit if it is set
    wrmsr
.5:
    mov     ecx, MSR_EFER
    rdmsr
    or      ax, MSR_EFER_XD             ; enable NXE
    wrmsr
    jmp     @XdDone
@SkipXd:
    sub     esp, 4
@XdDone:

    mov     ebx, cr0
    or      ebx, 0x80010023             ; enable paging + WP + NE + MP + PE
    mov     cr0, ebx
    lea     ebx, [edi + DSC_OFFSET]
    mov     ax, [ebx + DSC_DS]
    mov     ds, eax
    mov     ax, [ebx + DSC_OTHERSEG]
    mov     es, eax
    mov     fs, eax
    mov     gs, eax
    mov     ax, [ebx + DSC_SS]
    mov     ss, eax

;   jmp     _SmmFeatureSmiHandler                 ; instruction is not needed

global ASM_PFX(_SmmFeatureSmiHandler)
ASM_PFX(_SmmFeatureSmiHandler):
    mov     ebx, [esp + 4]                  ; CPU Index
    push    ebx
    mov     eax, ASM_PFX(SmmFeatureCpuSmmDebugEntry)
    call    eax
    add     esp, 4

    mov     eax, ASM_PFX(SmmSupovrStateLock)
    call    eax

    push    ebx
    mov     eax, ASM_PFX(SmiRendezvous)
    call    eax
    add     esp, 4

    push    ebx
    mov     eax, ASM_PFX(SmmFeatureCpuSmmDebugExit)
    call    eax
    add     esp, 4

    mov     eax, ASM_PFX(gSmmFeatureXdSupported)
    mov     al, [eax]
    cmp     al, 0
    jz      .7
    pop     edx                       ; get saved MSR_IA32_MISC_ENABLE[63-32]
    test    edx, BIT2
    jz      .7
    mov     ecx, MSR_IA32_MISC_ENABLE
    rdmsr
    or      dx, BIT2                  ; set XD Disable bit if it was set before entering into SMM
    wrmsr

.7:
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
