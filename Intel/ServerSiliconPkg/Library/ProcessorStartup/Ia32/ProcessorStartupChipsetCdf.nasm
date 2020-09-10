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

    SECTION .text

%include "ProcessorStartupPlatform.inc"
%include "ProcessorStartupChipset.inc"
%include "ProcessorStartup.inc"

extern ASM_PFX(PcdGet32 (PcdSecInFsp))
; APTIOV_SERVER_OVERRIDE_RC_START: Added support to Load Debug Microcode Patch on Production CPUs
extern ASM_PFX(FeaturePcdGet (PcdLoadDebugMicrocodePatch))
; APTIOV_SERVER_OVERRIDE_RC_END: Added support to Load Debug Microcode Patch on Production CPUs

;
; EarlyCycleDecoding is a helper function configure cycle decoding
;
extern ASM_PFX(EarlyCycleDecoding)
extern ASM_PFX(PcdGet16 (PcdTcoBaseAddress))

bits 32

global ASM_PFX(LateChipsetInit)
ASM_PFX(LateChipsetInit):

        ;
        ; program resource decoding
        ;

        ;
        ; Enable HPET decode in PCH.
        ;

        xor     eax, eax
        mov     esi, HPET_COMP_1
        mov     Dword [esi], eax
        mov     esi, HPET_COMP_2
        mov     Dword [esi], eax
        mov     esi, PCH_HPET_REG
        mov     al,  PCH_HPET_AE
        mov     Byte [esi], al
        mov     al,  Byte [esi]                     ; read back
        xor     eax, eax
        mov     esi, HPET_COMP_1
        mov     Dword [esi], eax
        mov     esi, HPET_COMP_2
        mov     Dword [esi], eax

        pushad
        call    ASM_PFX(EarlyCycleDecoding)
        popad
; APTIOV_SERVER_OVERRIDE_RC_START : Added to halt TCO timer
        ;
        ; Halt TCO timer.
        ;  
        mov    dx, [ASM_PFX(PcdGet16 (PcdTcoBaseAddress))] ; #TcoBase - Tco Base address register is changed in LBG
        or     dx, ICH_IOREG_TCO1_CNT
        in     ax, dx
        or     ax, BIT_TCO_TMR_HLT
        out    dx, ax

        ;
        ; Reload timer to prevent timeout
        ;
        mov   dx, [ASM_PFX(PcdGet16 (PcdTcoBaseAddress))] ; #TcoBase - Tco Base address register is changed in LBG
        mov   ax, 4
        out   dx, ax
        
        ;
        ; Clear TCO_STS
        ;
        mov   dx, [ASM_PFX(PcdGet16 (PcdTcoBaseAddress))] ; #TcoBase - Tco Base address register is changed in LBG
        or    dx, ICH_IOREG_TCO1_STS
        mov   ax, 0x008
        out   dx, ax
        
        ;
        ; Clear SMI_STS.TCO_STS
        ;
        mov   dx, PCH_ACPI_BASE_ADDRESS
        or    dx, 0x034                     ; SMI_STS offset 34h
        mov   ax, 0x02000
        out   dx, ax
; APTIOV_SERVER_OVERRIDE_RC_END : Added to halt TCO timer

        ret
; APTIOV_SERVER_OVERRIDE_RC_START: Added support to Load Debug Microcode Patch on Production CPUs
global ASM_PFX(EarlyChpsetInit_CrbHook)
ASM_PFX(EarlyChpsetInit_CrbHook):

    cmp byte [dword ASM_PFX(FeaturePcdGet (PcdLoadDebugMicrocodePatch))], 0
    jz  ExitChipsetInitCrbHook

LtUnlockMemory:
    mov esi, 0xFED40000 ; read TPM Status - drop the data
    mov eax, dword [ds:esi] ; This will clear the Wake Status bit
    mov ecx, 0x2e6 ; use this MSR to unlock memory
    xor eax, eax
    xor edx, edx
    wrmsr ; write a value of 0x0 to MSR (0x2E6)

FeatureControlMsr:
    mov ecx, 0x3A
    rdmsr
    test eax, BIT0 ; check if LOCK bit set (warm reset)
    jnz DebugInterfaceMsr ; do not write MSR if so
    xor eax, eax
    xor edx, edx
    wrmsr ; else write a value of 0x0 to MSR (0x3a)

DebugInterfaceMsr:
    mov ecx, 0x0C80
    rdmsr
    test eax, BIT30 ; check the LOCK bit
    jnz ExitChipsetInitCrbHook ; skip if set
    test eax, BIT0 ; check if DEBUG bit set (ITP installed)
    jnz ExitChipsetInitCrbHook ; do not write MSR if so
    or eax, 1 ; "or" in the DEBUG bit
    xor edx, edx
    wrmsr ; else write the value to MSR (0xC80)

ExitChipsetInitCrbHook:
    RET_ESI_MMX3

; APTIOV_SERVER_OVERRIDE_RC_END: Added support to Load Debug Microcode Patch on Production CPUs

