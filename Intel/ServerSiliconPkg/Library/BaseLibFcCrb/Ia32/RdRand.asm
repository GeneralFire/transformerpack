; @file
;
; @copyright
; INTEL CONFIDENTIAL
; Copyright 2015 - 2016 Intel Corporation. <BR>
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

.686P
.model flat, C

.code

;------------------------------------------------------------------------------
;  Generates a 16 bit random number through RDRAND instruction.
;  Return TRUE if Rand generated successfully, or FALSE if not.
;
;  BOOLEAN EFIAPI InternalX86RdRand16 (UINT16 *Rand);
;------------------------------------------------------------------------------
InternalX86RdRand16  PROC
    ; rdrand   ax                  ; generate a 16 bit RN into ax
                                   ; CF=1 if RN generated ok, otherwise CF=0
    db     0fh, 0c7h, 0f0h         ; rdrand r16: "0f c7 /6  ModRM:r/m(w)"
    jc     rn16_ok                 ; jmp if CF=1
    xor    eax, eax                ; reg=0 if CF=0
    ret                            ; return with failure status
rn16_ok:
    mov    edx, dword ptr [esp + 4]
    mov    [edx], ax
    mov    eax,  1
    ret
InternalX86RdRand16 ENDP

;------------------------------------------------------------------------------
;  Generates a 32 bit random number through RDRAND instruction.
;  Return TRUE if Rand generated successfully, or FALSE if not.
;
;  BOOLEAN EFIAPI InternalX86RdRand32 (UINT32 *Rand);
;------------------------------------------------------------------------------
InternalX86RdRand32  PROC
    ; rdrand   eax                 ; generate a 32 bit RN into eax
                                   ; CF=1 if RN generated ok, otherwise CF=0
    db     0fh, 0c7h, 0f0h         ; rdrand r32: "0f c7 /6  ModRM:r/m(w)"
    jc     rn32_ok                 ; jmp if CF=1
    xor    eax, eax                ; reg=0 if CF=0
    ret                            ; return with failure status
rn32_ok:
    mov    edx, dword ptr [esp + 4]
    mov    [edx], eax
    mov    eax,  1
    ret
InternalX86RdRand32 ENDP

;------------------------------------------------------------------------------
;  Generates a 64 bit random number through RDRAND instruction.
;  Return TRUE if Rand generated successfully, or FALSE if not.
;
;  BOOLEAN EFIAPI InternalX86RdRand64 (UINT64 *Rand);
;------------------------------------------------------------------------------
InternalX86RdRand64  PROC
    ; rdrand   eax                 ; generate a 32 bit RN into eax
                                   ; CF=1 if RN generated ok, otherwise CF=0
    db     0fh, 0c7h, 0f0h         ; rdrand r32: "0f c7 /6  ModRM:r/m(w)"
    jnc    rn64_ret                ; jmp if CF=0
    mov    edx, dword ptr [esp + 4]
    mov    [edx], eax

    db     0fh, 0c7h, 0f0h         ; generate another 32 bit RN
    jnc    rn64_ret                ; jmp if CF=0
    mov    [edx + 4], eax

    mov    eax,  1
    ret
rn64_ret:
    xor    eax, eax
    ret                            ; return with failure status
InternalX86RdRand64 ENDP

    END
