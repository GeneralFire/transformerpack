; @file ProcessorStartup.inc
;  MACROs used by Processor Startup Driver
;
; @copyright
; INTEL CONFIDENTIAL
; Copyright 1999 - 2020 Intel Corporation. <BR>
;
; The source code contained or described herein and all documents related to the
; source code ("Material") are owned by Intel Corporation or its suppliers or
; licensors. Title to the Material remains with Intel Corporation or its suppliers
; and licensors. The Material may contain trade secrets and proprietary and
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

%macro GET_ABS_ADDR 1
; Calculate address of input parameter
  movd esi, mm2
  sub  esi, %1
  neg  esi
%endmacro

%macro CALL_MMX3 1
  GET_ABS_ADDR %%ReturnAddress
  movd    mm3, esi                      ; save ReturnAddress into MM3
  jmp     %1                            ; Relative jump
  %%ReturnAddress:
%endmacro

%macro RET_ESI_MMX3 0
  movd    esi, mm3                      ; restore return address from MM3
  jmp     esi                           ; Absolute jump
%endmacro

%macro CALL_MMX4 1
  GET_ABS_ADDR %%ReturnAddress
  movd    mm4, esi                      ; save ReturnAddress into MM4
  jmp     %1                            ; Relative jump
  %%ReturnAddress:
%endmacro

%macro RET_ESI_MMX4 0
  movd    esi, mm4                      ; restore return address from MM4
  jmp     esi                           ; Absolute jump
%endmacro

%macro CALL_MMX5 1
  GET_ABS_ADDR %%ReturnAddress
  movd    mm5, esi                      ; save ReturnAddress into MM5
  jmp     %1                            ; Relative jump
  %%ReturnAddress:
%endmacro

%macro RET_ESI_MMX5 0
  movd    esi, mm5                      ; restore return address from MM5
  jmp     esi                           ; Absolute jump
%endmacro

%macro CALL_MMX6 1
  GET_ABS_ADDR %%ReturnAddress
  movd    mm6, esi                      ; save ReturnAddress into MM6
  jmp     %1                            ; Relative jump
  %%ReturnAddress:
%endmacro

%macro RET_ESI_MMX6 0
  movd    esi, mm6                      ; restore return address from MM6
  jmp     esi                           ; Absolute jump
%endmacro

;
; POST code macro
;
%macro PORT80 1
  mov al, %1
  out 0x80, al
%endmacro

;; EQUATEs used by Processor Startup Driver
MSR_XAPIC_BASE      EQU 01Bh


; Abstract:
;
;   IA32 architecture MSRs
;
;------------------------------------------------------------------------------

MTRR_CAP                      EQU 00FEh
MTRR_PHYS_BASE_0              EQU 0200h
MTRR_PHYS_MASK_0              EQU 0201h
MTRR_PHYS_BASE_1              EQU 0202h
MTRR_PHYS_MASK_1              EQU 0203h
MTRR_PHYS_BASE_2              EQU 0204h
MTRR_PHYS_MASK_2              EQU 0205h
MTRR_PHYS_BASE_3              EQU 0206h
MTRR_PHYS_MASK_3              EQU 0207h
MTRR_PHYS_BASE_4              EQU 0208h
MTRR_PHYS_MASK_4              EQU 0209h
MTRR_PHYS_BASE_5              EQU 020Ah
MTRR_PHYS_MASK_5              EQU 020Bh
MTRR_PHYS_BASE_6              EQU 020Ch
MTRR_PHYS_MASK_6              EQU 020Dh
MTRR_PHYS_BASE_7              EQU 020Eh
MTRR_PHYS_MASK_7              EQU 020Fh
MTRR_PHYS_BASE_8              EQU 0210h
MTRR_PHYS_MASK_8              EQU 0211h
MTRR_PHYS_BASE_9              EQU 0212h
MTRR_PHYS_MASK_9              EQU 0213h
MTRR_FIX_64K_00000            EQU 0250h
MTRR_FIX_16K_80000            EQU 0258h
MTRR_FIX_16K_A0000            EQU 0259h
MTRR_FIX_4K_C0000             EQU 0268h
MTRR_FIX_4K_C8000             EQU 0269h
MTRR_FIX_4K_D0000             EQU 026Ah
MTRR_FIX_4K_D8000             EQU 026Bh
MTRR_FIX_4K_E0000             EQU 026Ch
MTRR_FIX_4K_E8000             EQU 026Dh
MTRR_FIX_4K_F0000             EQU 026Eh
MTRR_FIX_4K_F8000             EQU 026Fh
MTRR_DEF_TYPE                 EQU 02FFh

MTRR_MEMORY_TYPE_UC           EQU 00h
MTRR_MEMORY_TYPE_WC           EQU 01h
MTRR_MEMORY_TYPE_WT           EQU 04h
MTRR_MEMORY_TYPE_WP           EQU 05h
MTRR_MEMORY_TYPE_WB           EQU 06h

MTRR_DEF_TYPE_E               EQU 0800h
MTRR_DEF_TYPE_FE              EQU 0400h
MTRR_PHYSMASK_VALID           EQU 0800h

;
; Define the high 32 bits of MTRR masking
; This should be read from CPUID EAX = 080000008h, EAX bits [7:0]
; But for most platforms this will be a fixed supported size so it is
; fixed to save space.
;
MTRR_PHYS_MASK_VALID          EQU 0800h
MTRR_PHYS_MASK_HIGH           EQU 00000000Fh      ; For 36 bit addressing
;MTRR_PHYS_MASK_HIGH           EQU 0000000FFh      ; For 40 bit addressing ICX_TODO, ICX support 52-bit addressing

IA32_MISC_ENABLE              EQU 1A0h
FAST_STRING_ENABLE_BIT        EQU 01h


VIRTUAL_MSR_MCA_ON_NON_NEW_CACHABLE_MMIO_EN_ADDR EQU 061h
MCAONNONNEMCACHEABLEMMIO_BIT  EQU 1h

CR0_CACHE_DISABLE             EQU 040000000h
CR0_NO_WRITE                  EQU 020000000h

IA32_PLATFORM_ID              EQU 017h
IA32_BIOS_UPDT_TRIG           EQU 079h
IA32_BIOS_SIGN_ID             EQU 08Bh
PLATFORM_INFO                 EQU 0CEh
NO_EVICT_MODE                 EQU 2E0h
NO_EVICTION_ENABLE_BIT        EQU 01h

;
; MSR definitions
;
MSR_IA32_PLATFORM_ID          EQU 0017h
MSR_IA32_APIC_BASE            EQU 001Bh
MSR_DCU_MODE                  EQU 0031h
MSR_SOCKET_ID                 EQU 0039h
MSR_IA32_FEATURE_CONTROL      EQU 003Ah
MSR_BDF_PLA_VMSR_62H          EQU 0062h
MSR_IA32_BIOS_UPDT_TRIG       EQU 0079h
MSR_IA32_BIOS_SIGN_ID         EQU 008Bh
MSR_PLATFORM_INFO             EQU 00CEh
MSR_CLOCK_CST_CONFIG_CONTROL  EQU 00E2h
MSR_IA32_PERF_STS             EQU 0198h
MSR_IA32_PERF_CTL             EQU 0199h
MSR_IA32_MISC_ENABLES         EQU 01A0h
MSR_IA32_DCA_CAP              EQU 01F9h
MSR_IA32_MC8_MISC2            EQU 0288h
MSR_IA32_MC7_CTL              EQU 041Ch
MSR_IA32_MC7_STATUS           EQU 0419h
MSR_IA32_MC8_STATUS           EQU 041Dh
MSR_PRIMARy_PLANE_CFG_CTRL    EQU 0601h
MSR_CPU_BUS_NO                EQU 0300h
MSR_CPU_BUS_NO_10NM           EQU 0128h
MSR_IA32_DEBUG_INTERFACE      EQU 0C80h  ; IA32_DEBUG_INTERFACE_MSR
MSR_BOOT_GUARD_SACM_INFO      EQU 013Ah  ; BootGuard Info MSR
MSR_BC_PBEC                   EQU 0139h  ; BootGuard PBE TIMER MSR
;

;
; Cache control macro
;
%macro DISABLE_CACHE 0
    mov   eax, cr0
    or    eax, CR0_CACHE_DISABLE + CR0_NO_WRITE
    wbinvd
    mov   cr0, eax
%endmacro

%macro ENABLE_CACHE 0
    mov   eax, cr0
    and   eax, NOT (CR0_CACHE_DISABLE + CR0_NO_WRITE)
    wbinvd
    mov   cr0, eax
%endmacro

;------------------------------------------------------------------------------
; macro to read a PCI config dword via lagacy CF8/CFC method
;------------------------------------------------------------------------------
%macro READ_PCI_DWORD_LEGACY 0
; input: EAX = legacy PCI address format with bit31 = 1
; output: EBX = dword register data
;         DX is destroyed
   mov   dx, 0cf8h
   out   dx, eax
   xchg  eax, ebx
   add   dx, 4
   in    eax, dx
   xchg  eax, ebx
%endmacro

; macro to write a PCI config dword via lagacy CF8/CFC method
; input:  EAX = legacy PCI address format with bit31 = 1
;         EBX = dword data to write
; output: DX is destroyed
%macro WRITE_PCI_DWORD_LEGACY 0
   mov   dx, 0cf8h
   out   dx, eax
   xchg  eax, ebx
   add   dx, 4
   out   dx, eax
   xchg  eax, ebx
%endmacro

STRUC UpdateHeaderStruc
  .dHeaderVersion        RESD 1          ; Header version#
  .dUpdateRevision       RESD 1          ; Update revision#
  .dDate                 RESD 1          ; Date in binary (08/13/07 as 0x08132007)
  .dProcessorSignature   RESD 1          ; CPU type, family, model, stepping
  .dChecksum             RESD 1          ; Checksum
  .dLoaderRevision       RESD 1          ; Update loader version#
  .dProcessorFlags       RESD 1          ; Processor Flags
  .dDataSize             RESD 1          ; Size of encrypted data
  .dTotalSize            RESD 1          ; Total size of update in bytes
  .bReserved             RESB 12         ; 12 bytes reserved
  .size:
ENDSTRUC

STRUC ext_sig_hdr
  .count        RESD 1
  .checksum     RESD 1
  .rsvd         RESD 3
  .size:
ENDSTRUC

STRUC ext_sig
  .processor    RESD 1
  .flags        RESD 1
  .checksum     RESD 1
  .size:
ENDSTRUC

; APTIOV_SERVER_OVERRIDE_RC_START: Use the Type 1 FIT Entry to find the Microcode
FIT_SIGNATURE_LOW       EQU 05449465Fh       ;"_FIT"
FIT_SIGNATURE_HIGH      EQU 02020205Fh       ;"_   "
FIT_EMPTY_ADDRESS       EQU 0FFFFFFFFh

STRUC FitEntry
  .address_low   RESD 1  ; Lower 4 bytes of address
  .address_high  RESD 1  ; Higher 4 bytes of address
  .size_low      RESW 1  ; Lower 2 bytes of size
  .size_high     RESB 1  ; Higher 1 byte of size
  .reserved      RESB 1  ; Reserved 1 byte
  .version       RESW 1  ; 2 bytes of version
  .type_cv       RESB 1  ; Type(lower 7 bits) and C_V(higher 1 bit) - 1 byte
  .chksum        RESB 1  ; Checksum - 1 byte
  .size:
ENDSTRUC
; APTIOV_SERVER_OVERRIDE_RC_END: Use the Type 1 FIT Entry to find the Microcode
