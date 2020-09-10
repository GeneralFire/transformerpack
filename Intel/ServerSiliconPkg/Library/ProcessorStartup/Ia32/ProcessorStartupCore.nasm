; @file
;
; @copyright
; INTEL CONFIDENTIAL
; Copyright 1999 - 2020 Intel Corporation. <BR>
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
%include "ProcessorUncoreCommon.inc"

extern ASM_PFX(OemFindMicrocode)
extern ASM_PFX(GetPlatformCpuSettings)
extern ASM_PFX(GetLocalSktId)
extern ASM_PFX(GetBusNum)
extern ASM_PFX(ReadCpuCsr)

extern ASM_PFX(PcdGet32 (PcdFlashSecCacheRegionBase))
extern ASM_PFX(PcdGet32 (PcdFlashSecCacheRegionSize))
extern ASM_PFX(PcdGet32 (PcdFlashIcxFlashRegionBase))
extern ASM_PFX(PcdGet32 (PcdFlashIcxFlashRegionSize))
extern ASM_PFX(PcdGet32 (PcdFlashCpxFlashRegionBase))
extern ASM_PFX(PcdGet32 (PcdFlashCpxFlashRegionSize))
extern ASM_PFX(PcdGet32 (PcdTemporaryRamBase))
extern ASM_PFX(PcdGet32 (PcdTemporaryRamSize))
extern ASM_PFX(PcdGet32 (PcdSecDataCacheRegionPattern))
extern ASM_PFX(PcdGet32 (PcdSecInFsp))
extern ASM_PFX(PcdGet32 (PcdFlashOemSecPeiRegionBase))
extern ASM_PFX(PcdGet32 (PcdFlashOemSecPeiRegionSize))

bits 32

global ASM_PFX(EarlyCoreInit)
ASM_PFX(EarlyCoreInit):
        IS_SKX_CPU
        je          short SkipMcaEn
        IS_SNR_CPU
        je          short SkipMcaEn
        IS_SPRSP_CPU
        je          short SkipMcaEn
        IS_GNRSP_CPU
        je          short SkipMcaEn

        ;---Enable MCA on Non Nem cached access, now tied to BIOS_DONE msr
        mov     eax, MCAONNONNEMCACHEABLEMMIO_BIT
        xor     edx, edx
        mov     ecx, VIRTUAL_MSR_MCA_ON_NON_NEW_CACHABLE_MMIO_EN_ADDR

        wrmsr

SkipMcaEn:
        ;---If Limit CPU ID enabled because of soft reset, disable.
        mov     ecx, 0x1a0
        rdmsr
        btr     eax, 22                                 ; Reset bit
        jnc     .0                                      ; If already reset, do not write to 1a0.

        wrmsr
.0:
        ;---Change APIC Address---
        mov   ecx, MSR_XAPIC_BASE                       ; Enable local APIC
        rdmsr
        and   edx, 0xfffffff0                           ; Mask local APIC address
        and   eax, 0x00000fff                           ; Mask local APIC address
        or    eax, LOCAL_APIC_BASE
        wrmsr

        RET_ESI_MMX3

global ASM_PFX(LoadProcessorMicrocode)
ASM_PFX(LoadProcessorMicrocode):
  ;
  ; Load microcode on all pacakge BSPs.
  ; Input: Stack NOT available
  ;        MMX3 = routine return address

        CALL_MMX4  ASM_PFX(OemFindMicrocode)

        or    eax,  eax                                 ; Microcode found?
        jz    MicrocodeLoadDone                         ; JIf not.

        mov   esi,  eax                                 ; ESI -> Microcode Update header.

        cmp   dword [esi + UpdateHeaderStruc.dUpdateRevision], 0  ; Microcode version is negative?
        jl    short load_ucode                            ; yes, always load

   ; Read MSR(8Bh) to verify microcode version
        mov   ecx, MSR_IA32_BIOS_SIGN_ID
        xor   eax, eax                                  ; Clear EAX
        xor   edx, edx                                  ; Clear EDX
        wrmsr                                           ; Write 0 to MSR 8Bh
        mov   eax, 1
        cpuid                                           ; this will update MSR 8Bh
        mov   ecx, MSR_IA32_BIOS_SIGN_ID
        rdmsr                                           ; edx = current microcode signature/revision

        cmp   edx, [esi + UpdateHeaderStruc.dUpdateRevision]
        jge   short MicrocodeLoadDone                   ; ucode patch already loaded. Skip.

load_ucode:
        mov   eax,  esi                                 ; eax => ucode buffer
        mov   ecx,  MSR_IA32_BIOS_UPDT_TRIG             ; MSR 79h
        xor   edx,  edx
        add   eax,  48                                  ; EAX -> Microcode Update data.
        wrmsr                                           ; load the microcode

   ; Read MSR(8Bh) to verify that the microcode loading was successful
        mov   ecx, MSR_IA32_BIOS_SIGN_ID
        xor   eax, eax                                  ; Clear EAX
        xor   edx, edx                                  ; Clear EDX
        wrmsr                                           ; Write 0 to MSR 8Bh
        mov   eax, 1
        cpuid                                           ; this will update MSR 8Bh
        mov   ecx, MSR_IA32_BIOS_SIGN_ID
        rdmsr                                           ; edx = current microcode signature/revision

        cmp   edx, [esi + UpdateHeaderStruc.dUpdateRevision]
        je    short MicrocodeLoadDone                   ; successful

MicrocodeLoadFailure:
        PORT80 (MCU_LOAD_ERROR_CODE)                    ; failure - microcode loading faiure
        jmp     load_ucode

MicrocodeLoadDone:

        RET_ESI_MMX3

global ASM_PFX(CheckGenuineIntelCpu)
ASM_PFX(CheckGenuineIntelCpu):
  ;
  ; Executes the CPUID instruction with EAX = 0, then reads
  ; the EBX, ECX, and EDX registers to determine if the CPU is "GenuineIntel"
  ;
        xor   eax, eax
        cpuid
        cmp   ebx, 0x756e6547    ; "Genu"
        jne   short cgic_error
        cmp   ecx, 0x6c65746e    ; "ntel"
        jne   short cgic_error
        cmp   edx, 0x49656e69    ; "ineI"
        jne   short cgic_error

        RET_ESI_MMX3

cgic_error:
        ; Error: non-Intel processor found
        PORT80  (NON_INTEL_CPU_ERROR_CODE)
        jmp  $                         ; dead loop
;;        hlt
;;        jmp short cgic_error

global ASM_PFX(ConfigDfx)
ASM_PFX(ConfigDfx):
  ;
  ; Config package BSP CPU  DEBUG_INTERFACE_MSR.Enable bit.
  ;        Note:  BIOS is only allowed to write 1 to this bit (never 0).
  ;
  ; Input: Stack NOT available
  ;        MMX3 = routine return address
  ;

        mov   eax, 1
        cpuid
        test  ecx, BIT11                                ; Is  DEBUG_INTERFACE_MSR supported?
        jz    ConfigDfxExit                             ; No

        CALL_MMX5  ASM_PFX(GetPlatformCpuSettings)      ; Get CPU specific settings from platform hook
        test  ecx, CPU_OPTIONS_BIT_DEBUG_INTERFACE_EN   ; 1 -> Enable DEBUG_INTERFACE
        jz    ConfigDfxExit                             ; 0 -> Leave this MSR alone (as is)

        mov  ecx, MSR_IA32_DEBUG_INTERFACE              ; Read the MSR
        rdmsr
        test eax, BIT30                                 ; Lock bit set?
        jnz  ConfigDfxExit                              ; yes
        test eax, BIT0                                  ; Enable bit set already?
        jnz  ConfigDfxExit                              ; yes

        or   eax, BIT0                                  ; set ENABLE bit = 1
        wrmsr                                           ; write back to MSR

ConfigDfxExit:

        RET_ESI_MMX3

global ASM_PFX(ConfigDcuMode)
ASM_PFX(ConfigDcuMode):
  ;
  ; Config package BSP CPU DCU_MODE (32KB 8-way v.s. 16KB 4-way ECC)
  ; This must be done before CR0.CD is changed from 1 to 0 for the first time after reset, i.e., before NEM init.
  ; Refer to DCU_MODE MSR in CPU BWG
  ;
  ; Input: Stack NOT available
  ;        MMX3 = routine return address
  ;
        IS_SNR_CPU
        je    short ConfigDcuModeExit        ; Skip if SNR
        mov   ecx, MSR_PLATFORM_INFO         ; Check if CPU supports DCU Mode select
        rdmsr
        test  eax, (1 << 26)                ; DCU Mode Select supported?
        jz    short ConfigDcuModeExit        ; Skip if not

        CALL_MMX5  ASM_PFX(GetPlatformCpuSettings)        ; Get CPU specific settings from platform hook
        test  ecx, CPU_OPTIONS_BIT_DCU_MODE_SEL  ; 0/1 -> 32KB 8-way(HW default) / 16KB 4-way ECC
        jz    ConfigDcuModeExit              ; JIf zero

        mov   ecx,  MSR_DCU_MODE
        rdmsr                                ; read MSR_DCU_MODE
        or    al,   BIT0                                        ; set DCU_MODE bit
        wrmsr

ConfigDcuModeExit:

        RET_ESI_MMX3

FindFreeMtrr:
;************************************************************
; Input: Stack NOT available
;        MMX4 = routine return address
; Output:
;        ECX = MSR address of the next free MTRR   PHYS_BASE
;
; Registers changed:
;        EAX, EBX, ECX, EDX, ESI
;************************************************************

;
;  Find total number of MTRRs supported
;
  mov    ecx, MTRR_CAP
  rdmsr
  movzx  esi, al                    ; ESI  = number of variable MTRR pairs
  shl    esi, 1                     ; ESI x 2
  add    esi, MTRR_PHYS_BASE_0      ; ESI  = (Addr of last MTRR) +1

  mov    ecx, MTRR_PHYS_MASK_0      ; Start with MTRR 0
.1:
  rdmsr
  test   eax, BIT11                 ; Valid = 1?
  jz     FoundFreeMtrr
  add    ecx, 2                     ; next MTRR
  cmp    ecx, esi                   ; all available MTRRs checked?
  jb     .1                         ; No, keep searching
  ;
  ; No available MTRR, halt system
  ;
  PORT80 (NEM_NO_FREE_MTRR_ERROR_CODE)   ; fatal error - No free MTRR found
  jmp    $

FoundFreeMtrr:
  dec    ecx                        ; return ECX = MTRR PHYS_BASE address

  RET_ESI_MMX4

global ASM_PFX(ConfigureMtrrs)
ASM_PFX(ConfigureMtrrs):
;************************************************************
; Input: Stack NOT available
;        MMX4 = routine return address
;        ECX  = MTRR Base MSR register address to start with
;        EBX  = Memory Type MTRR describes, only BL is used.
;        EAX  = Memory Size
;        EDI  = Memory Base Address
;
; Registers changed:
;        EAX, EBX, ECX, EDX, EDI, ESI
;************************************************************

  ;
  ; Define "local" vars for this routine
  ;
%define CODE_SIZE_TO_CACHE mm5
%define CODE_BASE_TO_CACHE mm6
%define NEXT_MTRR_SIZE     mm7
  ;
  ; Initialize "locals"
  ;
  shl     ebx, 16                       ; save Memory Type into upper 16 bits
  mov     bx, cx                        ; save MTRR MSR addr

  ;
  ; Save remaining size to cache
  ;
  movd    CODE_SIZE_TO_CACHE, eax       ; Size of code cache region that must be cached
  movd    CODE_BASE_TO_CACHE, edi       ; Base code cache address
  mov     edi, ebx                      ; temporarily save Memory type and next MTRR MSR

  ;
  ; Configure MTRR_PHYS_MASK_HIGH for proper addressing above 4GB
  ; based on the physical address size supported for this processor
  ; This is based on read from CPUID EAX = 080000008h, EAX bits [7:0]
  ;
  ; Examples:
  ;  MTRR_PHYS_MASK_HIGH = 00000000Fh  For 36 bit addressing
  ;  MTRR_PHYS_MASK_HIGH = 0000000FFh  For 40 bit addressing
  ;
  mov   eax, 0x80000008                  ; Address sizes leaf
  cpuid
  sub     al, 32
  movzx   eax, al
  xor     esi, esi
  bts     esi, eax
  dec     esi                           ; esi <- MTRR_PHYS_MASK_HIGH

  mov     ebx, edi                      ; restore memory type and next MTRR MSR to ebx
nextMtrr:
  ;
  ; Get remaining size to cache
  ;
  movd    eax, CODE_SIZE_TO_CACHE
  and     eax, eax
  jz      ConfigureMtrrsExit            ; If no left size - we are done
  ;
  ; Determine next size to cache.
  ; We start from bottom up. Use the following algorithm:
  ; 1. Get our own alignment. Max size we can cache equals to our alignment
  ; 2. Determine what is bigger - alignment or remaining size to cache.
  ;    If alignment is bigger - cache it.
  ;      Adjust remaing size to cache and base address
  ;      Loop to 1.
  ;    If remaining size to cache is bigger
  ;      Determine the biggest 2^N part of it and cache it.
  ;      Adjust remaing size to cache and base address
  ;      Loop to 1.
  ; 3. End when there is no left size to cache or no left MTRRs
  ;
  movd    edi, CODE_BASE_TO_CACHE
  bsf     ecx, edi                      ; Get index of lowest bit set in base address
  ;
  ; Convert index into size to be cached by next MTRR
  ;
  mov     edx, 0x1
  shl     edx, cl                       ; Alignment is in edx
  cmp     edx, eax                      ; What is bigger, alignment or remaining size?
  jbe     gotSize                       ; JIf aligment is less
  ;
  ; Remaining size is bigger. Get the biggest part of it, 2^N in size
  ;
  bsr     ecx, eax                      ; Get index of highest set bit
  ;
  ; Convert index into size to be cached by next MTRR
  ;
  mov     edx, 1
  shl     edx, cl                       ; Size to cache

gotSize:
  mov     eax, edx
  movd    NEXT_MTRR_SIZE, eax           ; Save

  ;
  ; Compute MTRR mask value:  Mask = NOT (Size - 1)
  ;
  dec     eax                           ; eax - size to cache less one byte
  not     eax                           ; eax contains low 32 bits of mask
  or      eax, MTRR_PHYS_MASK_VALID     ; Set valid bit

  ;
  ; Program mask register
  ;
  movzx   ecx, bx                       ; Move MTRR MSR into ecx
  inc     ecx                           ; increment MSR address from BASE to MASK
  mov     edx, esi                      ; edx <- MTRR_PHYS_MASK_HIGH
  wrmsr
  ;
  ; Program base register
  ;
  dec     ecx                            ; decrement MSR back to BASE
  xor     edx, edx                       ; MTRR_PHYS_MASK_HIGH = 0
  movd    eax, CODE_BASE_TO_CACHE
  ror     ebx, 16                        ; temporarily put MTRR type in bx
  or      al, bl                         ; set memory type
  rol     ebx, 16                        ; restore MTRR type back into upper 16 bits
  wrmsr
  ;
  ; Advance and loop
  ; Reduce remaining size to cache
  ;
  movd    edx, CODE_SIZE_TO_CACHE
  movd    eax, NEXT_MTRR_SIZE
  sub     edx, eax
  movd    CODE_SIZE_TO_CACHE, edx

  ;
  ; Increment MTRR MSR to next MTRR BASE register
  ;
  add     bx, 2

  ;
  ; Increment base address to cache
  ;
  movd    edx, CODE_BASE_TO_CACHE
  movd    eax, NEXT_MTRR_SIZE
  add     edx, eax
  movd    CODE_BASE_TO_CACHE, edx

  jmp     nextMtrr

ConfigureMtrrsExit:

  RET_ESI_MMX4

global ASM_PFX(NemInit)
ASM_PFX(NemInit):
;
;************************************************************
;
; Input: Stack NOT available
;        MMX3 = routine return address
;
; Description:
;
;   This function initializes the Cache for Data, Stack, and Code
;   as specified in the  BIOS Writer's Guide.
;************************************************************
;
; Check CPUID if support Boot Guard feature
;
  mov     eax, BIT0                   ; To get CPU signature.
  cpuid                               ; EAX = CPU signature.
  and     eax, 0xFFFF0
  shr     eax, 4
  cmp     eax, CPU_FAMILY_SPRSP       ; Check SPR cpu
  je      DetectBootGuard
  cmp     eax, CPU_FAMILY_ICX         ; Check ICX cpu
  je      DetectBootGuard
  cmp     eax, CPU_FAMILY_SNR         ; Check SNR cpu
  je      DetectBootGuard
  cmp     eax, CPU_FAMILY_ICXD        ; Check ICX cpu
  je      short DetectBootGuard
  cmp     eax, CPU_FAMILY_SKX         ; Check SKX cpu
  je      DetectBootGuard
  cmp     eax, CPU_FAMILY_SPRSP       ; Check SPR cpu
  je      DetectBootGuard
  cmp     eax, CPU_FAMILY_GNRSP       ; Check GNR cpu
  je      DetectBootGuard
  cmp     eax, CPU_FAMILY_SRFSP       ; Check SRF cpu
  je      DetectBootGuard
  jmp     SkipDetectBootGuard         ; Default skip

;
; Detect Boot Guard Boot
;
DetectBootGuard:
  mov     ecx, MSR_BOOT_GUARD_SACM_INFO;
  rdmsr
  test    eax, BIT0                     ; BootGuard status = 1?
  jnz     BootGuardNemSetup             ; Yes

#ifndef SPR_PO_FLAG
  ;
  ; BTGP3 fails to boot when ACM detected an error.
  ; check to see if BTGP3 reported an error and stop PBET timer if so.
  ;
  mov     esi, TXT_PUBLIC_BASE
  mov     eax, dword [esi+0xa4]
  test    eax, BIT16                    ; BIT48 of 0xFED300A0 = 1 --> BTG_FAILED?
  jz      SkipDetectBootGuard

  mov    ecx, MSR_BC_PBEC
  mov    eax, BIT0
  xor    edx, edx
  wrmsr
#endif


SkipDetectBootGuard:
;
;  Normal flow without BootGuard handling
;
  mov  ecx, MTRR_CAP
  rdmsr
  movzx  eax, al                        ; EAX = number of variable MTRR pairs
  lea  ebx, [4 * eax + FixedMtrrSize]   ; EBX = size of the MtrrInitTable table
                                        ; to program: 4 * VariableMtrrPairs + FixedMtrrSize
  xor  eax, eax                         ; EAX = 0
  xor  edx, edx                         ; EDX = 0

  GET_ABS_ADDR MtrrInitTable            ; ESI : location of MtrrInitTable

InitMtrrLoop:
  sub  ebx, 2
  movzx  ecx, word [cs:ebx + esi]       ; ECX <- address of MTRR to zero
  wrmsr
  jnz  InitMtrrLoop                     ; loop through the whole table

  ;
  ; Configure the default memory type to un-cacheable (UC) in the
  ; IA32_MTRR_DEF_TYPE MSR.
  ;
  mov     ecx, MTRR_DEF_TYPE            ; Load the MTRR default type index
  rdmsr
  and     eax, ~ (0xCFF)          ; Clear the enable bits and def type UC.
  wrmsr

  ;
  ; Find available MTRR, ECX returned with address of free MTRR_PHYS_BASE address
  ; ECX is an input parameter to ConfigureMtrrs
  ;
  CALL_MMX4    FindFreeMtrr

  ;
  ; Configure the DataStack region as write-back (WB) cacheable memory type
  ; using the variable range MTRRs. EAX and EDI are input parameters to ConfigureMtrrs
  ;
  mov     eax, [ASM_PFX(PcdGet32 (PcdTemporaryRamSize))]
  mov     edi, [ASM_PFX(PcdGet32 (PcdTemporaryRamBase))]

  ;
  ; Set Writeback MTRR memory type in EBX, input parameter to ConfigureMtrrs
  ;
  mov     ebx, MTRR_MEMORY_TYPE_WB

  ;
  ; Round up size in EAX to page size
  ;
  mov     edx, eax                      ; Save
  and     edx, 0xFFFF0000               ; Number of pages in 64K
  and     eax, 0xFFFF                   ; Number of "less-than-page" bytes
  jz      rounded1
  mov     eax, 0x10000                   ; Add the whole page size

rounded1:
  add     eax, edx                      ; eax - rounded up code cache size

  CALL_MMX4    ASM_PFX(ConfigureMtrrs)

  ;
  ; Find available MTRR, ECX returned with address of free MTRR_PHYS_BASE address
  ; ECX is an input parameter to ConfigureMtrrs
  ;
  CALL_MMX4    FindFreeMtrr

  ;
  ; Configure the DataStack region as write-back (WB) cacheable memory type
  ; using the variable range MTRRs. EAX and EDI are input parameters to ConfigureMtrrs
  ;
  mov     eax, [ASM_PFX(PcdGet32 (PcdFlashIcxFlashRegionSize))]
  mov     edi, [ASM_PFX(PcdGet32 (PcdFlashIcxFlashRegionBase))]

  cmp     edi, 0
  je      SkipUnifyCacheSetting         ; The PCD PcdFlashIcxFlashRegionBase will be ZERO in Non-Unified IFWI
  ;
  ; Set Writeback MTRR memory type in EBX, input parameter to ConfigureMtrrs
  ;
  mov     ebx, MTRR_MEMORY_TYPE_WB

  ;
  ; Round up size in EAX to page size
  ;
  mov     edx, eax                      ; Save
  and     edx, 0xFFFF0000               ; Number of pages in 64K
  and     eax, 0xFFFF                   ; Number of "less-than-page" bytes
  jz      rounded8
  mov     eax, 0x10000                   ; Add the whole page size

rounded8:
  add     eax, edx                      ; eax - rounded up code cache size

  CALL_MMX4    ASM_PFX(ConfigureMtrrs)
SkipUnifyCacheSetting:

  ;
  ; Find available MTRR, ECX returned with address of free MTRR_PHYS_BASE address
  ; ECX is an input parameter to ConfigureMtrrs
  ;
  CALL_MMX4    FindFreeMtrr

  ;
  ; Configure the BIOS code region as write-protected (WP) cacheable
  ; memory type using the variable range MTRRs. EAX and EDI are input parameters
  ; to ConfigureMtrrs
  ;
  mov     eax, [ASM_PFX(PcdGet32 (PcdSecInFsp))]
  cmp     eax, 0
  jz      NotInFsp_3
  mov     eax, dword [esp]
  movd    xmm2, eax                 ; Save pointer for later use
  mov     edi, dword [eax + 0x28]   ; Code region base
  mov     eax, dword [eax + 0x2C]   ; Code region size
NotInFsp_3:
  jnz     InFsp_3
  mov     eax, [ASM_PFX(PcdGet32 (PcdFlashSecCacheRegionSize))]
  mov     edi, [ASM_PFX(PcdGet32 (PcdFlashSecCacheRegionBase))]
InFsp_3:

  ;
  ; Set Write-protected MTRR memory type in EBX, input parameter to ConfigureMtrrs
  ;
  mov     ebx, MTRR_MEMORY_TYPE_WP

  ;
  ; Round up size in EAX to page size
  ;
  mov     edx, eax                      ; Save
  and     edx, 0xFFFF0000               ; Number of pages in 64K
  and     eax, 0xFFFF                   ; Number of "less-than-page" bytes
  jz      rounded2
  mov     eax, 0x10000                   ; Add the whole page size

rounded2:
  add     eax, edx                      ; eax - rounded up code cache size

  CALL_MMX4    ASM_PFX(ConfigureMtrrs)

  ;
  ; Configure the OemSecPei region as write-protected (WP) cacheable memory type
  ; using the variable range MTRRs. EAX and EDI are input parameters to ConfigureMtrrs
  ;
  mov     edi, [ASM_PFX(PcdGet32 (PcdFlashOemSecPeiRegionBase))]
  cmp     edi, 0
  je      SkipOemSecPeiCacheSetting         ; The PCD PcdFlashOemSecPeiRegionBase will not be ZERO in Unified PC IFWI

  ;
  ; Find available MTRR, ECX returned with address of free MTRR_PHYS_BASE address
  ; ECX is an input parameter to ConfigureMtrrs
  ;
  CALL_MMX4    FindFreeMtrr

  mov     eax, [ASM_PFX(PcdGet32 (PcdFlashOemSecPeiRegionSize))]
  ;
  ; Set Write-protected MTRR memory type in EBX, input parameter to ConfigureMtrrs
  ;
  mov     ebx, MTRR_MEMORY_TYPE_WP

  ;
  ; Round up size in EAX to page size
  ;
  mov     edx, eax                      ; Save
  and     edx, 0xFFFF0000               ; Number of pages in 64K
  and     eax, 0xFFFF                   ; Number of "less-than-page" bytes
  jz      rounded3
  mov     eax, 0x10000                   ; Add the whole page size

rounded3:
  add     eax, edx                      ; eax - rounded up code cache size

  CALL_MMX4    ASM_PFX(ConfigureMtrrs)

SkipOemSecPeiCacheSetting:
  ;
  ; Enable the MTRRs by setting the IA32_MTRR_DEF_TYPE MSR E flag.
  ;
  mov     ecx, MTRR_DEF_TYPE            ; Load the MTRR default type index
  rdmsr
  or      eax, MTRR_DEF_TYPE_E          ; Enable variable range MTRRs
  wrmsr

  ;
  ; Need to disable fast strings to prevent ItoM transition which leads to illegal WbtoMmio in NEM mode
  ;
  mov   ecx, IA32_MISC_ENABLE
  rdmsr
  and   eax, ~ (FAST_STRING_ENABLE_BIT)
  wrmsr

  ;
  ; Enable the logical processor's (BSP) cache: execute INVD and set
  ; CR0.CD = 0, CR0.NW = 0.
  ;
  mov     eax, cr0
  and     eax, ~ (CR0_CACHE_DISABLE + CR0_NO_WRITE)
  invd
  mov     cr0, eax
  ;
  ; Enable No-Eviction Mode Setup State by setting
  ; NO_EVICT_MODE  MSR 2E0h bit [0] = '1'.
  ;
  mov     ecx, NO_EVICT_MODE
  rdmsr
  or      eax, 1
  wrmsr

  ;
  ; One location in each 64-byte cache line of the DataStack region
  ; must be written to set all cache values to the modified state.
  ;
  mov     edi, [ASM_PFX(PcdGet32 (PcdTemporaryRamBase))]
  mov     ecx, [ASM_PFX(PcdGet32 (PcdTemporaryRamSize))]
  shr     ecx, 6            ; Divide by 64
  mov     eax, [ASM_PFX(PcdGet32 (PcdSecDataCacheRegionPattern))]

.2:
  mov     [edi], eax
  sfence
  add     edi, 64
  loop    .2

  ;
  ; Enable No-Eviction Mode Run State by setting
  ; NO_EVICT_MODE MSR 2E0h bit [1] = '1'.
  ;
  mov     ecx, NO_EVICT_MODE
  rdmsr
  or      eax, 2
  wrmsr

  ;
  ; Finished with cache configuration
  ;

  jmp     FinishedCacheConfig

BootGuardNemSetup:
  ;
  ; Control comes here when Boot Guard boot and NEM is initialized by Boot Guard ACM
  ;
  ; Configure MTRR_PHYS_MASK_HIGH for proper addressing above 4GB
  ; based on the physical address size supported for this processor
  ; This is based on read from CPUID EAX = 080000008h, EAX bits [7:0]
  ;
  ; Examples:
  ;  MTRR_PHYS_MASK_HIGH = 00000000Fh  For 36 bit addressing
  ;  MTRR_PHYS_MASK_HIGH = 0000000FFh  For 40 bit addressing
  ;
  mov   eax, 0x80000008                  ; Address sizes leaf
  cpuid
  sub   al, 32
  movzx eax, al
  xor   esi, esi
  bts   esi, eax
  dec   esi                             ; esi <- MTRR_PHYS_MASK_HIGH
  mov   edi, esi                        ; edi <- MTRR_PHYS_MASK_HIGH

  ;
  ; Configure the DataStack region as write-back (WB) cacheable memory type
  ; using the variable range MTRRs.
  ;
  ;
  ; Find available MTRR
  ;
  CALL_MMX4    FindFreeMtrr             ; ECX = addr of free MTRR_PHYS_BASE

  ;
  ; Set the base address of the DataStack cache range
  ;
  mov     eax, [ASM_PFX(PcdGet32 (PcdTemporaryRamBase))]
  or      eax, MTRR_MEMORY_TYPE_WB      ; Load the write-back cache value
  xor     edx, edx                      ; clear upper dword
  wrmsr                                 ; the value in MTRR_PHYS_BASE_0

  ;
  ; Set the mask for the DataStack cache range
  ; Compute MTRR mask value:  Mask = NOT (Size - 1)
  ;
  inc     ecx                           ; ecx = PHYS_MASK msr
  mov     eax, [ASM_PFX(PcdGet32 (PcdTemporaryRamSize))]
  dec     eax                           ; eax - size to cache less one byte
  not     eax                           ; eax contains low 32 bits of mask
  or      eax, MTRR_PHYS_MASK_VALID     ; turn on the Valid flag
  mov     edx, edi                      ; edx <- MTRR_PHYS_MASK_HIGH
  wrmsr

  ;
  ; Need to disable fast strings to prevent ItoM transition which leads to illegal WbtoMmio in NEM mode
  ;
  mov   ecx, IA32_MISC_ENABLE
  rdmsr
  and   eax, ~ (FAST_STRING_ENABLE_BIT)
  wrmsr

  ;
  ; One location in each 64-byte cache line of the DataStack region
  ; must be written to set all cache values to the modified state.
  ;
  mov     edi, [ASM_PFX(PcdGet32 (PcdTemporaryRamBase))]
  mov     ecx, [ASM_PFX(PcdGet32 (PcdTemporaryRamSize))]
  shr     ecx, 6        ; Divide by 64
  mov     eax, [ASM_PFX(PcdGet32 (PcdSecDataCacheRegionPattern))]
  not     eax

.3:
  mov     [edi], eax
  sfence
  add     edi, 64
  loop    .3

  ;
  ; Stop PBE timer
  ;
  mov     ecx, MSR_BC_PBEC
  mov     eax, BIT0                     ; Set B_STOP_PBET BIT0
  xor     edx, edx
  wrmsr

  ;
  ; Finished with cache configuration
  ;

FinishedCacheConfig:
  ;
  ; Optionally test the Data Region by writing and reading
  ;
  cld
  mov     edi, [ASM_PFX(PcdGet32 (PcdTemporaryRamBase))]
  mov     ecx, [ASM_PFX(PcdGet32 (PcdTemporaryRamSize))]
  shr     ecx, 2        ; Divide by 4
  mov     eax, [ASM_PFX(PcdGet32 (PcdSecDataCacheRegionPattern))]
  not     eax

TestDataStackArea:
  stosd
  cmp     eax, DWORD [edi-4]
  jnz     DataStackTestFail
  loop    TestDataStackArea
  jmp     DataStackTestPass

  ;
  ; Data Cache test failed
  ;
DataStackTestFail:
  PORT80 (NEM_DATA_RW_TEST_ERROR_CODE)
  jmp     $

DataStackTestPass:

  ;
  ; Get CPU Faimly/stepping info
  ;
  mov     eax, BIT0                   ; To get CPU signature.
  cpuid                               ; EAX = CPU signature.
  and     eax, 0xFFFF0
  shr     eax, 4
  cmp     eax, CPU_FAMILY_ICX         ; Check ICX cpu
  jne     CopyCpxIBB

  ;
  ; For ICX-SP/SNR/ICXDE 1706775263: Faf requires code region to be cached to avoid SPI fetch during mode activation
  ;
  mov     ecx, [ASM_PFX(PcdGet32 (PcdFlashIcxFlashRegionSize))]
  mov     edi, [ASM_PFX(PcdGet32 (PcdFlashIcxFlashRegionBase))]
  cmp     edi, 0
  je      CopyCpxIBBEnd               ; The PCD PcdFlashIcxFlashRegionBase will be ZERO in Non-Unified IFWI
  shr     ecx, 6                      ; Divide by 64 Bytes (cache line size)
FafWaAgain:
  mov     eax, DWORD [edi]
  add     edi, 64
  loop    FafWaAgain
  jmp     CopyCpxIBBEnd

CopyCpxIBB:
  ;
  ; Copy CPX Specific-IBB to the range of ICX Specific-IBB
  ;
  mov     esi, [ASM_PFX(PcdGet32 (PcdFlashCpxFlashRegionBase))]       ; Src address
  mov     edi, [ASM_PFX(PcdGet32 (PcdFlashIcxFlashRegionBase))]       ; Des address
  cmp     edi, 0
  je      CopyCpxIBBEnd               ; The PCD PcdFlashIcxFlashRegionBase will be ZERO in Non-Unified IFWI
  mov     ecx, [ASM_PFX(PcdGet32 (PcdFlashCpxFlashRegionSize))]
  shr     ecx, 2                                                      ; Number of DWORD
  rep     movsd
CopyCpxIBBEnd:

  ;
  ; At this point you may continue normal execution.  Typically this would include
  ; reserving stack, initializing the stack pointer, etc.
  ;

  ;
  ; After memory initialization is complete, please follow the algorithm in the BIOS
  ; Writer's Guide to properly transition to a normal system configuration.
  ; The algorithm covers the required sequence to properly exit this mode.
  ;

  RET_ESI_MMX3

global ASM_PFX(EstablishNemStack)
ASM_PFX(EstablishNemStack):
  ; Establish stack
  ;
  ; Fill the stack with a recognizable pattern.
  ;
  mov   esp, [ASM_PFX(PcdGet32 (PcdTemporaryRamBase))]
  add   esp, [ASM_PFX(PcdGet32 (PcdTemporaryRamSize))]
  sub   esp, 0x40
  mov   edi, [ASM_PFX(PcdGet32 (PcdTemporaryRamBase))]
  cld
  mov   ecx, [ASM_PFX(PcdGet32 (PcdTemporaryRamSize))]
  shr   ecx, 2
  mov   eax, [ASM_PFX(PcdGet32 (PcdSecDataCacheRegionPattern))]
  rep   stosd

  RET_ESI_MMX3

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

FixedMtrrSize EQU ($ - MtrrInitTable)

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

;-------------------------------------------------------------------------
; TxtInit
;   LTsx-related CPU early init that must be executed by all package BSPs
;
; Input:
;   AccessCSR proc has been executed
;   No Eviction Mode (NEM) is enabled, stack is available
; Output:
;-------------------------------------------------------------------------

global ASM_PFX(TxtInit)
ASM_PFX(TxtInit):

    ;
    ; Check if CPU is fused for LTSX support
    ;
    mov     eax, 1
    cpuid
    test    ecx, BIT6
    jz      TxtInitExit         ; no, skip

    ;
    ; Check local LT_CONTROL.lt_target_enable (bit 4/bit 5)
    ;
    shr     eax, 4
    cmp     eax, CPU_FAMILY_SKX
    je      short CPUID_Skx1
    cmp     eax, CPU_FAMILY_SPRSP
    je      short CPUID_Spr1
    cmp     eax, CPU_FAMILY_GNRSP
    je      short CPUID_Gnr1
    cmp     eax, CPU_FAMILY_SRFSP
    je      short CPUID_Gnr1
    mov     eax, CSR_LT_CONTROL_CHABC_ICX
    jmp     short CPUID_Icx1
CPUID_Skx1:
    mov     eax, CSR_LT_CONTROL_CHABC_SKX
CPUID_Icx1:
    call    ASM_PFX(ReadCpuCsr)                ; EAX = current CSR data
    bt      eax, 4
    jnc     TxtInitExit
    jmp     TxtContinue

    ;
    ; starting with SPR LT_Control uses BIT5 for lt_target_en.
    ;
CPUID_Gnr1:
    mov     eax, CSR_LT_CONTROL_CHABC_GNR
    jmp     BitTest_Spr
CPUID_Spr1:
    mov     eax, CSR_LT_CONTROL_CHABC_SPR
BitTest_Spr:
    call    ASM_PFX(ReadCpuCsr)                ; EAX = current CSR data
    bt      eax, 5
    jnc     TxtInitExit

TxtContinue:
    ;
    ; Check EXISTS registe TXT_PUBLIC_BASE + 10h  to verify
    ; if the thread is registered as LT capable (each socket has a corresponding bit)
    ;
    call   ASM_PFX(GetLocalSktId)              ; CL = own socket ID (0, 1, 2, 3)
    mov    esi, TXT_PUBLIC_BASE
    mov    eax, dword [esi+0x10]
    bt     eax, ecx             ; TXT.EXIST = 1?
    jc     short x1             ; yes
; TXT.EXIST = 0, Check POISON.STS
    mov    eax, dword [esi+0x08]
    test   al, 0x01             ; bit0 = 1 -> POISION.STS active?
    jz     TxtInitExit          ; no, skip

x1:

%ifdef PCH_LBG_ASM
    ; DWR Flow check
    mov     esi, 0xFED30360
    mov     eax, dword [ds:esi]
    test    eax, BIT1
    jnz     SkipTPMRead
%endif

    ;
    ;  Do a read to TPM Status (and drop the data).
    ;  If 1, then hardware will clear WAKE-ERROR.STS bits in TXT public reg space offset 08h[6]
    ;
    mov    esi, TPM_STATUS_REG_ADDRESS
    mov    eax, dword [esi]

SkipTPMRead:
    ;
    ; In 10nm programs we have to wait until MRC enables the iMC before we can unlock memory
    ;
    mov     eax, 1
    cpuid
    shr     eax, 4
    cmp     eax, CPU_FAMILY_SKX
    jne     short TxtInitExit

    ;
    ;  Unlock memory by writing 0 to MSR 2E6h
    ;

    mov     ecx, 0x2E6
    xor     eax, eax
    xor     edx, edx
    wrmsr

TxtInitExit:
    ret

;-------------------------------------------------------------------------
; Read a Dword from PCI register 00h thru FFh using IO port CF8h/CFCh
; Assumptions:
;   No Eviction Mode (NEM) is enabled, stack is available
; Input:
;   EAX = legacy PCI address format
;   EAX[31:24] = 80h
;   EAX[23:16] = Bus #
;   EAX[15:11] = Dev #
;   EAX[10:8]  = Func #
;   EAX[7:0]   = Reg offset (dword aligned)
;
; Output: EAX = regiser dword data
; Registers changed: None except EAX
;-------------------------------------------------------------------------

global ASM_PFX(ReadPciLegacy)
ASM_PFX(ReadPciLegacy):

   push  edx
   mov   dx, 0xcf8
   out   dx, eax
   mov   dx, 0xcfc
   in    eax, dx
   pop   edx
   ret

;-------------------------------------------------------------------------
; Write a Dword to a PCI register 00h thru FFh using IO port CF8h/CFCh
; Assumptions:
;   No Eviction Mode (NEM) is enabled, stack is available
; Input:
;   EAX = legacy PCI address format
;   EAX[31:24] = 80h
;   EAX[23:16] = Bus #
;   EAX[15:11] = Dev #
;   EAX[10:8]  = Func #
;   EAX[7:0]   = Reg offset (dword aligned)
;   EBX = dword data to be written
; Output: None
; Registers changed: None
;-------------------------------------------------------------------------

global ASM_PFX(WritePciLegacy)
ASM_PFX(WritePciLegacy):

   push  edx
   mov   dx, 0xcf8
   out   dx, eax
   mov   dx, 0xcfc
   xchg  eax, ebx
   out   dx, eax
   xchg  eax, ebx
   pop   edx

   ret
