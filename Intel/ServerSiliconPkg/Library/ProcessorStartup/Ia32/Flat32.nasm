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

;-----------------------------------------------------------------------------------------------------------------------
    section .text
;-----------------------------------------------------------------------------------------------------------------------

%include "Flat32.inc"
%include "ProcessorStartupPlatform.inc"

extern ASM_PFX(PcdGet32(PcdTemporaryRamBase))
extern ASM_PFX(PcdGet32(PcdTemporaryRamSize))
extern ASM_PFX(ProcessorStartup)
extern ASM_PFX(SecStartup)

;-----------------------------------------------------------------------------------------------------------------------
    section .textr code align=16
;----------------------------------------------------------------------------------------------------------------------
BITS 16
align 16

global ASM_PFX(_ModuleEntryPoint)
ASM_PFX(_ModuleEntryPoint):
  ;
  ; Save BIST state in MM0
  ;
  fninit
  movd    mm0, eax

  mov     ax, 0xF000
  mov     ds, ax
  mov     si, 0xFFF0
  cmp     byte [si], 0xEA    ; Is it warm reset ?
  jne     NotWarmReset       ; JIf not.
  jmp     0xf000:0xe05b      ; Far jump to F000:E05B (legacy BIOS warm reset entry)

NotWarmReset:

  xor     eax, eax
  mov     es, ax
  mov     ax, cs
  mov     ds, ax


  ; Enter Protected mode.
  ;
  mov     esi,  GdtDesc
  o32 lgdt [cs:si]
  mov     eax, cr0                      ; Get control register 0
  or      eax, 0x3                      ; Set PE bit (bit #0) & MP bit (bit #1)
  mov     cr0, eax                      ; Activate protected mode
  mov     eax, cr4                      ; Get control register 4
  or      eax, 0x600                    ; Set OSFXSR bit (bit #9) & OSXMMEXCPT bit (bit #10)
  mov     cr4, eax

  ;
  ; Now we're in Protected16
  ; Set up the selectors for protected mode entry
  ;
  mov     ax, SYS_DATA_SEL
  mov     ds, ax
  mov     es, ax
  mov     fs, ax
  mov     gs, ax
  mov     ss, ax

  ;
  ; Go to Protected32
  ;
  mov     esi, NemInitLinearAddress
  o32 jmp far [cs:si]

;-----------------------------------------------------------------------------------------------------------------------
    section .text code align=16
;-----------------------------------------------------------------------------------------------------------------------
BITS 32
align 16

;-----------------------------------------------------------------------------------------------;
;	ProtectedModeSECStart
;-----------------------------------------------------------------------------------------------;
global ASM_PFX(ProtectedModeSECStart)
ASM_PFX(ProtectedModeSECStart):
  mov     esi, ProcessorStartupReturnAddress
  mov     edi, 0
  jmp     ASM_PFX(ProcessorStartup)

ProcessorStartupReturnAddress:
  jmp ExitSecPhase

ExitSecPhase:
  jmp  ASM_PFX(HandOffToNextModule)

global ASM_PFX(HandOffToNextModule)
ASM_PFX(HandOffToNextModule):
  ; Push CPU count to stack first, then AP's (if there is one)
  ; BIST status, and then BSP's
  ;

  ;
  ; Here work around for BIST
  ; Can only read BSP BIST, so only pass 1 CPU value
  ;
  movd    eax, mm0
  mov     ecx, 1
  push    ecx

PushBist:
  push    eax
  loop    PushBist

TransferToSecStartup:

  ;
  ; Add for PI
  ;

  mov     eax, esp
  mov     ebx, 0x87655678
  push    ebx
  push    eax
  mov     bx,  0
  push    bx
  lidt    [esp]        ;Load IDT register
  pop     bx
  pop     eax

  ;
  ; Pass BFV into the PEI Core
  ;
  mov     edi, FV_MAIN_BASE             ; 0FFFFFFFCh
  push    dword [edi]

  push    dword [ASM_PFX(PcdGet32 (PcdTemporaryRamBase))]  ; Pass TempRamBase into the PEI Core for PI
  push    dword [ASM_PFX(PcdGet32 (PcdTemporaryRamSize))]  ; Pass stack size into the PEI Core

  ;
  ; Pass Control into the PEI Core
  ;
  call ASM_PFX(SecStartup)

MtrrInitTable:
    DW  MTRR_DEF_TYPE
    DW  MTRR_FIX_64K_00000
    DW  MTRR_FIX_16K_80000
    DW  MTRR_FIX_16K_A0000
    DW  MTRR_FIX_4K_C0000
    DW  MTRR_FIX_4K_C8000
    DW  MTRR_FIX_4K_D0000
    DW  MTRR_FIX_4K_D8000
    DW  MTRR_FIX_4K_E0000
    DW  MTRR_FIX_4K_E8000
    DW  MTRR_FIX_4K_F0000
    DW  MTRR_FIX_4K_F8000
    DW  MTRR_PHYS_BASE_0
    DW  MTRR_PHYS_MASK_0
    DW  MTRR_PHYS_BASE_1
    DW  MTRR_PHYS_MASK_1
    DW  MTRR_PHYS_BASE_2
    DW  MTRR_PHYS_MASK_2
    DW  MTRR_PHYS_BASE_3
    DW  MTRR_PHYS_MASK_3
    DW  MTRR_PHYS_BASE_4
    DW  MTRR_PHYS_MASK_4
    DW  MTRR_PHYS_BASE_5
    DW  MTRR_PHYS_MASK_5
    DW  MTRR_PHYS_BASE_6
    DW  MTRR_PHYS_MASK_6
    DW  MTRR_PHYS_BASE_7
    DW  MTRR_PHYS_MASK_7
    DW  MTRR_PHYS_BASE_8
    DW  MTRR_PHYS_MASK_8
    DW  MTRR_PHYS_BASE_9
    DW  MTRR_PHYS_MASK_9
MtrrCount EQU (($ - MtrrInitTable) / 2)

align 16
global ASM_PFX(BootGDTtable)

;
; GDT[0]: 0x00: Null entry, never used.
;
NULL_SEL EQU $ - GDT_BASE               ; Selector [0]
GDT_BASE:
BootGDTtable DD 0
             DD 0
;
; Linear data segment descriptor
;
LINEAR_SEL EQU $ - GDT_BASE             ; Selector [0x8]
    DW  0xFFFF                          ; limit 0xFFFFF
    DW  0                               ; base 0
    DB  0
    DB  0x92                            ; present, ring 0, data, expand-up, writable
    DB  0xCF                            ; page-granular, 32-bit
    DB  0
;
; Linear code segment descriptor
;
LINEAR_CODE_SEL EQU $ - GDT_BASE        ; Selector [0x10]
    DW  0xFFFF                          ; limit 0xFFFFF
    DW  0                               ; base 0
    DB  0
    DB  0x9B                            ; present, ring 0, data, expand-up, not-writable
    DB  0xCF                            ; page-granular, 32-bit
    DB  0
;
; System data segment descriptor
;
SYS_DATA_SEL EQU $ - GDT_BASE           ; Selector [0x18]
    DW  0xFFFF                          ; limit 0xFFFFF
    DW  0                               ; base 0
    DB  0
    DB  0x93                            ; present, ring 0, data, expand-up, not-writable
    DB  0xCF                            ; page-granular, 32-bit
    DB  0

;
; System code segment descriptor
;
SYS_CODE_SEL EQU $ - GDT_BASE           ; Selector [0x20]
    DW  0xFFFF                          ; limit 0xFFFFF
    DW  0                               ; base 0
    DB  0
    DB  0x9A                            ; present, ring 0, data, expand-up, writable
    DB  0xCF                            ; page-granular, 32-bit
    DB  0
;
; Spare segment descriptor
;
SYS16_CODE_SEL EQU $ - GDT_BASE         ; Selector [0x28]
    DW  0xFFFF                          ; limit 0xFFFFF
    DW  0                               ; base 0
    DB  0xE                             ; Changed from F000 to E000.
    DB  0x9B                            ; present, ring 0, code, expand-up, writable
    DB  0x0                             ; byte-granular, 16-bit
    DB  0
;
; Spare segment descriptor
;
SYS16_DATA_SEL EQU $ - GDT_BASE         ; Selector [0x30]
    DW  0xFFFF                          ; limit 0xFFFF
    DW  0                               ; base 0
    DB  0
    DB  0x93                            ; present, ring 0, data, expand-up, not-writable
    DB  0x0                             ; byte-granular, 16-bit
    DB  0

;
; Spare segment descriptor
;
SPARE5_SEL EQU $ - GDT_BASE             ; Selector [0x38]
    DW  0                               ; limit 0
    DW  0                               ; base 0
    DB  0
    DB  0                               ; present, ring 0, data, expand-up, writable
    DB  0                               ; page-granular, 32-bit
    DB  0
GDT_SIZE EQU $ - BootGDTtable           ; Size, in bytes

GdtDesc:                                ; GDT descriptor
    DW  GDT_SIZE - 1                    ; GDT limit
    DD  BootGDTtable                    ; GDT base address

NemInitLinearAddress:
NemInitLinearOffset:
    DD  ASM_PFX(ProtectedModeSECStart)    ; Offset of our 32 bit code
    DW  LINEAR_CODE_SEL
