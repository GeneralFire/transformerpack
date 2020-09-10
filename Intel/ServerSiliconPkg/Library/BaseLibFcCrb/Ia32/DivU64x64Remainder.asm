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

    .386
    .model  flat,C
    .code

EXTERN  InternalMathDivRemU64x32:PROC

;------------------------------------------------------------------------------
; UINT64
; EFIAPI
; InternalMathDivRemU64x64 (
;   IN      UINT64                    Dividend,
;   IN      UINT64                    Divisor,
;   OUT     UINT64                    *Remainder    OPTIONAL
;   );
;------------------------------------------------------------------------------
InternalMathDivRemU64x64    PROC
    mov     ecx, [esp + 16]             ; ecx <- divisor[32..63]
    test    ecx, ecx
    jnz     _@DivRemU64x64              ; call _@DivRemU64x64 if Divisor > 2^32
    mov     ecx, [esp + 20]
    jecxz   @F
    and     dword ptr [ecx + 4], 0      ; zero high dword of remainder
    mov     [esp + 16], ecx             ; set up stack frame to match DivRemU64x32
@@:
    jmp     InternalMathDivRemU64x32
InternalMathDivRemU64x64    ENDP

_@DivRemU64x64  PROC PRIVATE    USES    ebx esi edi
    mov     edx, dword ptr [esp + 20]
    mov     eax, dword ptr [esp + 16]   ; edx:eax <- dividend
    mov     edi, edx
    mov     esi, eax                    ; edi:esi <- dividend
    mov     ebx, dword ptr [esp + 24]   ; ecx:ebx <- divisor
@@:
    shr     edx, 1
    rcr     eax, 1
    shrd    ebx, ecx, 1
    shr     ecx, 1
    jnz     @B
    div     ebx
    mov     ebx, eax                    ; ebx <- quotient
    mov     ecx, [esp + 28]             ; ecx <- high dword of divisor
    mul     dword ptr [esp + 24]        ; edx:eax <- quotient * divisor[0..31]
    imul    ecx, ebx                    ; ecx <- quotient * divisor[32..63]
    add     edx, ecx                    ; edx <- (quotient * divisor)[32..63]
    mov     ecx, dword ptr [esp + 32]   ; ecx <- addr for Remainder
    jc      @TooLarge                   ; product > 2^64
    cmp     edi, edx                    ; compare high 32 bits
    ja      @Correct
    jb      @TooLarge                   ; product > dividend
    cmp     esi, eax
    jae     @Correct                    ; product <= dividend
@TooLarge:
    dec     ebx                         ; adjust quotient by -1
    jecxz   @Return                     ; return if Remainder == NULL
    sub     eax, dword ptr [esp + 24]
    sbb     edx, dword ptr [esp + 28]   ; edx:eax <- (quotient - 1) * divisor
@Correct:
    jecxz   @Return
    sub     esi, eax
    sbb     edi, edx                    ; edi:esi <- remainder
    mov     [ecx], esi
    mov     [ecx + 4], edi
@Return:
    mov     eax, ebx                    ; eax <- quotient
    xor     edx, edx                    ; quotient is 32 bits long
    ret
_@DivRemU64x64  ENDP

    END
