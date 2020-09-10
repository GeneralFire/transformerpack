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

    SECTION . text
%include "ProcessorStartupPlatform.inc"
%include "ProcessorStartupUncore.inc"
%include "ProcessorStartup.inc"

extern ASM_PFX(PcdGet32 (PcdFlashNvStorageMicrocodeBase))
extern ASM_PFX(PcdGet32 (PcdFlashNvStorageMicrocodeSize))
extern ASM_PFX(PcdGet32 (PcdFlashNvStorageMicrocode2Base))
extern ASM_PFX(PcdGet32 (PcdFlashNvStorageMicrocode2Size))
extern ASM_PFX(AmILegacySocketNoStack)
extern ASM_PFX(GetMmcfgNoStack)

bits 32

;-----------------------------------------------------------------
; This routine returns CPU-specific platform setup options/settings
; to the caller via ECX register
;-----------------------------------------------------------------
;   Executed by SBSP and NBSP
; Input:
;   Stack not available
;   mmx5 = routine return address
;   MMCFG/CPUBUSNO has been configured so node BSPs can access CSRs on socket 0, and vice-versa
;   xmm registers initialized by EnableAccessCSR routine
;      xmm1 register [31:24] contains local Socket ID
;      xmm3 register contains MMCFG_RULE
;   xmm4,xmm6 registers contain the IIO bus number array initialized by EnableAccessCSR routine
;       xmm4: socket 0 - 3, xmm6: 4 - 7
;   xmm5,xmm7 registers contain the Uncore bus number array initialized by EnableAccessCSR routine
;       xmm5: socket 0 - 3, xmm7: 4 - 7
;  Output:
;  Output:
;    ECX[0]  - DCU_MODE select   0/1:   32KB 8-way non-ECC (hardware default) / 16KB 4-way ECC
;    ECX[1]  - debugInterfaceEn   1:   Enable Debug Interface
;    ECX[31:2] reserved
;   All other general purpose registers destroyed
;-----------------------------------------------------------------

global ASM_PFX(GetPlatformCpuSettings)
ASM_PFX(GetPlatformCpuSettings):

;
; DCU_MODE Select setup data is stored(copied) into sticky scratch pad register SSR07[24] on the warm boot path.
; Read the scratch pad register and returns the DCU_MODE setting to caller
;
   IS_SKX_CPU
   je    short Cpuid_Skx1

   CALL_MMX6  ASM_PFX(GetMmcfgNoStack)                      ;[edi] = MMCFG base
   add   edi, CSR_BIOSSCRATCHPAD7_ICX

   jmp   short Cpuid_Icx1
Cpuid_Skx1:
   CALL_MMX6  ASM_PFX(GetMmcfgNoStack)                      ;[edi] = MMCFG base
   add   edi, CSR_BIOSSCRATCHPAD7_SKX
Cpuid_Icx1:

   xor   edx, edx
   mov   ebx, dword [edi]                          ; ebx = CSR data

   test  ebx, BIT5                                 ; "Comlepted Cold Reset Flow"?
   jz    short gdms_50                             ; no
   test  ebx,  BIT27                               ; ebx[27] = debugInterfaceEn info saved from cold boot path
   jz    .0
   or    edx, CPU_OPTIONS_BIT_DEBUG_INTERFACE_EN   ; return debugInterfaceEn = 1
.0:
   test  ebx,  BIT24                               ; ebx[24] = DCU Mode Select info saved from cold boot path
   jz    short gdms_50
   or    edx, CPU_OPTIONS_BIT_DCU_MODE_SEL         ; return DCU_MODE select = 1

gdms_50:
   mov   ecx, edx                                  ; return data in ecx
   RET_ESI_MMX5

; Stub dummy hook for MiniBIOS implementation
global ASM_PFX(_OEMCpuSecInit)
ASM_PFX(_OEMCpuSecInit):
       ret

;-----------------------------------------------------------------
; This routine returns the address of CPU microcode update in the 
; specified firmware volume.
;-----------------------------------------------------------------
;   Executed by SBSP and NBSP
; Input:
;   Stack not available
;   mmx5 = routine return address
;   ESP = Start address of microcode FV
;   EBP = Size of microcode FV
; Output:
;   EAX = Zero       No matching microcode found
;   EAX = Non-Zero   Linear address of CPU microcode update header that matches the current CPU
;   All other registers destroyed
;-----------------------------------------------------------------
global ASM_PFX(OemFindMicrocodeInFv)
ASM_PFX(OemFindMicrocodeInFv):

; APTIOV_SERVER_OVERRIDE_RC_START: Use the Type 1 FIT Entry to find the Microcode
   CALL_MMX6   ASM_PFX(OemFindMicrocodeFromFit)           ;Get Microcode from FIT Table
   cmp   eax, FIT_EMPTY_ADDRESS
   jne   exit
; APTIOV_SERVER_OVERRIDE_RC_END: Use the Type 1 FIT Entry to find the Microcode
   mov   esi, esp  ; esi = Start address of ucode region
   add   esi, MICROCODEFV_PATCHDATA_OFFSET   ; get patch raw data
   ; Get processor signature and platform ID from the installed processor
   ; and save into registers for later use
   ; ebx = processor signature
   ; edx = platform ID
   mov   eax, 1
   cpuid
   mov   ebx, eax
   mov   ecx, MSR_IA32_PLATFORM_ID
   rdmsr
   mov   ecx, edx
   shr   ecx, 50-32
   and   ecx, 0x7
   mov   edx, 1
   shl   edx, cl

   ; Current register usage
   ; esi -> microcode update to check
   ; ebx = processor signature
   ; edx = platform ID

check_main_header:
   ; Check for valid microcode header
   ; Minimal test checking for header version and loader version as 1
   mov   eax, dword 1
   cmp   [esi + UpdateHeaderStruc.dHeaderVersion], eax
   jne   advance_fixed_size
   cmp   [esi + UpdateHeaderStruc.dLoaderRevision], eax
   jne   advance_fixed_size

   ; Check if signature and plaform ID match
   cmp   ebx, [esi + UpdateHeaderStruc.dProcessorSignature]
   jne   .1
   test  edx, [esi + UpdateHeaderStruc.dProcessorFlags]
   jnz   load_check  ; Jif signature and platform ID match

.1:
   ; Check if extended header exists
   ; First check if total_size and data_size are valid
   xor   eax, eax
   cmp   [esi + UpdateHeaderStruc.dTotalSize], eax
   je    next_microcode
   cmp   [esi + UpdateHeaderStruc.dDataSize], eax
   je    next_microcode

   ; Then verify total size - sizeof header > data size
   mov   ecx, [esi + UpdateHeaderStruc.dTotalSize]
   sub   ecx, UpdateHeaderStruc.size
   cmp   ecx, [esi + UpdateHeaderStruc.dDataSize]
   jbe   next_microcode    ; Jif extended header does not exist

   ; Check if total size fits in microcode region
   mov   edi, esi
   add   edi, [esi + UpdateHeaderStruc.dTotalSize]
   mov   ecx, esp
   add   ecx, ebp
   cmp   edi, ecx
   ja    exit_not_found              ; Jif address is outside of ucode region

   ; Set edi -> extended header
   mov   edi, esi
   add   edi, UpdateHeaderStruc.size
   add   edi, [esi + UpdateHeaderStruc.dDataSize]

   ; Get count of extended structures
   mov   ecx, [edi + ext_sig_hdr.count]

   ; Move pointer to first signature structure
   add   edi, ext_sig_hdr.size

check_ext_sig:
   ; Check if extended signature and platform ID match
   cmp   [edi + ext_sig.processor], ebx
   jne   .2
   test  [edi + ext_sig.flags], edx
   jnz   load_check     ; Jif signature and platform ID match
.2:
   ; Check if any more extended signatures exist
   add   edi, ext_sig.size
   loop  check_ext_sig

next_microcode:
   ; Advance just after end of this microcode
   xor   eax, eax
   cmp   [esi + UpdateHeaderStruc.dTotalSize], eax
   je    .3
   add   esi, [esi + UpdateHeaderStruc.dTotalSize]
   jmp   check_address
.3:
   add   esi, dword 2048
   jmp   check_address

advance_fixed_size:
   ; Advance by 4X dwords
   add   esi, dword 1024

check_address:
   ; Address >= microcode region address + microcode region size?
   mov   eax, esp
   add   eax, ebp
   cmp   esi, eax
   jae   exit_not_found        ;Jif address is outside of ucode region
   jmp   check_main_header

load_check:
   ; Get the revision of the current microcode update loaded
   mov ecx, MSR_IA32_BIOS_SIGN_ID
   xor   eax, eax                ; Clear EAX
   xor   edx, edx                ; Clear EDX
   wrmsr                         ; Load 0 to MSR at 8Bh
   mov   eax, 1
   cpuid
   mov   ecx, MSR_IA32_BIOS_SIGN_ID
   rdmsr                         ; Get current microcode signature

   ; Verify this microcode update is not already loaded
   cmp   [esi + UpdateHeaderStruc.dUpdateRevision], edx
   je    exit_not_found

found_microcode:
   mov   eax, esi                ; EAX contains the linear address of the start of the Microcode Update Header
   jmp   short exit

exit_not_found:
   xor   eax, eax                ; ucode not found

exit:
   RET_ESI_MMX5                  ; return to caller

;-----------------------------------------------------------------
; This routine returns the address of CPU microcode update
;-----------------------------------------------------------------
;   Executed by SBSP and NBSP
; Input:
;   Stack not available
;   mmx4 = routine return address
; Output:
;   EAX = Zero       No matching microcode found
;   EAX = Non-Zero   Linear address of CPU microcode update header that matches the current CPU
;   All other registers destroyed
;-----------------------------------------------------------------
global ASM_PFX(OemFindMicrocode)
ASM_PFX(OemFindMicrocode):
   mov   esp, [ASM_PFX(PcdGet32 (PcdFlashNvStorageMicrocodeBase))]
   mov   ebp, [ASM_PFX(PcdGet32 (PcdFlashNvStorageMicrocodeSize))]
   CALL_MMX5  ASM_PFX(OemFindMicrocodeInFv)
   movd  mm7, eax                      ; MMX7 <- Zero or Address of microcode update header in FV1

   mov   esp, [ASM_PFX(PcdGet32 (PcdFlashNvStorageMicrocode2Base))]
   mov   ebp, [ASM_PFX(PcdGet32 (PcdFlashNvStorageMicrocode2Size))]
   cmp   esp, 0                   ; Microcode FV2 exist?
   je    oem_find_microcode_exit  ; JIf no FV2.
   CALL_MMX5  ASM_PFX(OemFindMicrocodeInFv)
   movd  ebx, mm7  

   ; Current register usage
   ; EAX: Zero, or address of microcode update header in FV2
   ; EBX: Zero, or address of microcode update header in FV1

   or   ebx, ebx                  ; Microcode found in FV1?
   jz   oem_find_microcode_exit   ; JIf not.
   or   eax, eax                  ; Microcode found in FV2?
   jz   return_ebx_microcode      ; JIf not.

   mov  edx, [ebx + UpdateHeaderStruc.dUpdateRevision]
   cmp  edx, [eax + UpdateHeaderStruc.dUpdateRevision]
   ja   return_ebx_microcode    ; Use unsigned compare since negative version (debug microcode) takes priority.
   
   jmp  oem_find_microcode_exit
     
return_ebx_microcode:
   mov  eax, ebx

oem_find_microcode_exit:
   RET_ESI_MMX4                  ; return to caller
; APTIOV_SERVER_OVERRIDE_RC_START: Use the Type 1 FIT Entry to find the Microcode
;-----------------------------------------------------------------
; This routine returns the CPU microcode update address from FIT
;-----------------------------------------------------------------
;   Executed by SBSP and NBSP
; Input:
;   Stack not available
;   mmx6 = routine return address
; Output:
;   EAX = 0x00000000       No matching microcode found in FIT or 
;         0xFFFFFFFF       Either FIT not available or No microcode entry in FIT
;         Non-Zero         32 bit Linear address of CPU microcode update header that matches the current CPU
;   All other registers destroyed
;-----------------------------------------------------------------

global ASM_PFX(OemFindMicrocodeFromFit)
ASM_PFX(OemFindMicrocodeFromFit):
   
   mov   edi, 0x0FFFFFFC0                                          ; FIT table pointer updated during build by FIT tool
   mov   esi, [edi]                                                ; FIT starting address.
   mov   eax, FIT_EMPTY_ADDRESS                                    ; Copying EMPTY FIT address will be used in OemFindMicrocode
   cmp   dword [esi + FitEntry.address_low], FIT_SIGNATURE_LOW     ; Type 0 entry has FIT signature
   jnz   exit_find_ucode         
   cmp   dword [esi + FitEntry.address_high], FIT_SIGNATURE_HIGH
   jnz   exit_find_ucode                                           ; If "_FIT_   " is not matched jmp to exit and proceed loading from FV
   mov   edi, esi                                                  ; Saving the FIT pointer to EDI
   mov   ebp, 0                                                    ; Initializing the EBP used for preserving latest revision ucode pointer
   mov   ecx, 0                                                    ; ECX - Flag, Type exhausted or its not there
fit_next_microcode:
   add   edi, FitEntry.size                                        ; Skipping to next entry
   mov   eax, FIT_EMPTY_ADDRESS                                    ; Copying EMPTY FIT address will be used in OemFindMicrocode
   mov   bl, [edi + FitEntry.type_cv]                              ; Skip to TYPE in FIT Entry
   and   bl, 0x7f                                                  ; Getting the type field bit0-bit6
   cmp   bl, 0x01                                                  ; Compare if TYPE is 1 
   jnz    decide_exit                                              ; If no type 1 present then exit proceed loading from FV.
   mov   esi, [edi + FitEntry.address_low]                         ; Getting the base address of uCode header in ESI.
   cmp   eax, [esi + UpdateHeaderStruc.dHeaderVersion]             ; check if first dword content is 0xFFFF_FFFF - Reserved uCode slot.
   je    fit_next_microcode                                        ; If it is reserved uCode slot jump to next microcode 
    
   ; ESI <- Pointing to 1st byte of uCode update header
   ; Get processor signature and platform ID from the installed processor
   ; and save into registers for later use
   ; ebx = processor signature
   ; edx = platform ID
   
   mov   eax, 1
   cpuid
   mov   ebx, eax                                                  ; Processor Signature in EBX register
   mov   ecx, MSR_IA32_PLATFORM_ID                                 ; Reading MSR 0x017
   rdmsr
   mov   ecx, edx                                               
   shr   ecx, 18                                                   ; Geting the Platform ID Bit 50-52 of EDX:EAX of RDMSR
   and   ecx, 7h                                                   ; Making other bits 0 except platform ID
   mov   edx, 1             
   shl   edx, cl

   ; Current register usage
   ; esi -> microcode update to check
   ; ebx = processor signature
   ; edx = platform ID
check_ucode_hdr:
   ; Check for valid microcode header
   ; Minimal test checking for header version and loader version as 1
   mov   eax, dword 0x1
   mov   ecx, 1                                                    ; ECX=1 to handle in decide_exit block to indicate type 1 is present
   cmp   eax, [esi + UpdateHeaderStruc.dHeaderVersion]
   jne   fit_next_microcode
   cmp   eax, [esi + UpdateHeaderStruc.dLoaderRevision]
   jne   fit_next_microcode
   
   ; Check if signature and plaform ID match
   cmp   ebx, [esi + UpdateHeaderStruc.dProcessorSignature]
   jne   .0
   test  edx, [esi + UpdateHeaderStruc.dProcessorFlags]
   jnz   check_latest  ; Jif signature and platform ID match
.0:
   ; Check if extended header exists
   ; First check if total_size and data_size are valid
   xor   eax, eax
   cmp   [esi + UpdateHeaderStruc.dTotalSize], eax
   je    fit_next_microcode
   cmp   [esi + UpdateHeaderStruc.dDataSize], eax
   je    fit_next_microcode

   ; Then verify total size - sizeof header > data size
   mov   ecx, [esi + UpdateHeaderStruc.dTotalSize]
   sub   ecx, UpdateHeaderStruc.size
   cmp   ecx, [esi + UpdateHeaderStruc.dDataSize]
   mov   ecx, 1                                                    ; ECX=1 to handle in decide_exit block to indicate type 1 is present
   jbe   fit_next_microcode                                            ; Jif extended header does not exist

   ; Set eax -> extended header
   mov   eax, esi
   add   eax, UpdateHeaderStruc.size
   add   eax, [esi + UpdateHeaderStruc.dDataSize]

   ; Get count of extended structures
   mov   ecx, [eax + ext_sig_hdr.count]

   ; Move pointer to first signature structure
   add   eax, ext_sig_hdr.size

check_extd_sig:
   ; Check if extended signature and platform ID match
   cmp   ebx, [eax + ext_sig.processor]
   jne   .1
   test  edx, [eax + ext_sig.flags]
   jnz   check_latest                                              ; Jif signature and platform ID match
.1:
   ; Check if any more extended signatures exist
   add   eax, ext_sig.size
   loop  check_extd_sig
   mov   ecx, 1                                                    ; ECX=1 to handle in decide_exit block to indicate type 1 is present
   jmp   fit_next_microcode
check_latest:
   cmp   ebp, 0                                                    ; If EBP is 0 save current ucode ptr to EBP
   je    store_ptr
   mov   eax, [ebp + UpdateHeaderStruc.dUpdateRevision]
   cmp   eax, [esi + UpdateHeaderStruc.dUpdateRevision]              ; Compare previous ucode update revision with current
   jg    fit_next_microcode                                            ; If previous ucode patch is greater, Scan for next microcode
                                                                   ; If current ucode patch has greater revision save ptr to EBP
store_ptr:
   mov   ebp, esi                                                  ; Saving current pointer to EBP
   mov   ecx, 1                                                    ; ECX=1 to handle in decide_exit block to indicate type 1 is present
   jmp   fit_next_microcode

check_loaded:
   mov   ecx, MSR_IA32_BIOS_SIGN_ID
   xor   eax, eax                                                  ; Clear EAX
   xor   edx, edx                                                  ; Clear EDX
   wrmsr                                                           ; Load 0 to MSR at 8Bh
   mov   eax, 1
   cpuid
   mov   ecx, MSR_IA32_BIOS_SIGN_ID
   rdmsr                                                           ; Get current microcode signature

   ; Verify this microcode update is not already loaded
   cmp   edx, [esi + UpdateHeaderStruc.dUpdateRevision]
   je    ucode_not_found
ucode_found:
   mov   eax, esi
   jmp   exit_find_ucode
decide_exit:
   cmp   ecx, 0                                                    ; If ECX = 0 then it FIT entry has no type 1
   je    exit_find_ucode                                           ; Exit with returning EAX=0xFFFF_FFFF
   mov   esi, ebp                                                  ; Saving the latest microcode header pointer to ESI
   cmp   ebp, 0                                                    ; If EBP is not 0, uCode found and check for already loaded 
   jne   check_loaded                                              ; Check if the latest ucode patch(in EBP) is already loaded or not
ucode_not_found:
   xor   eax, eax                                                  ; Retuning EAX<-0
exit_find_ucode:
   RET_ESI_MMX6
; APTIOV_SERVER_OVERRIDE_RC_END: Use the Type 1 FIT Entry to find the Microcode
