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

%define MOVDIN0                 movd mm0, ebx
%define MOVDOUT0                movd ebx, mm0

%define MOVDIN1                 movd mm1, ebx
%define MOVDOUT1                movd ebx, mm1

%define MOVDIN2                 movd mm2, ebx
%define MOVDOUT2                movd ebx, mm2

%define MOVDIN3                 movd mm3, ebx
%define MOVDOUT3                movd ebx, mm3

%define MOVDIN4                 movd mm4, ebx
%define MOVDOUT4                movd ebx, mm4

%define MOVDIN5                 movd mm5, ebx
%define MOVDOUT5                movd ebx, mm5

%define MOVDIN6                 movd mm6, ebx
%define MOVDOUT6                movd ebx, mm6

%define MOVDIN7                 movd mm7, ebx
%define MOVDOUT7                movd ebx, mm7

;-----  CALL_NS  ------------------------------------------------------------;
;                                                                            ;
;       Purpose:        Stackless "call" to destination procedure            ;
;                                                                            ;
;       Input:          routine to jump to                                   ;
;                                                                            ;
;       Output:         none                                                 ;
;                                                                            ;
;       Setup:          CR4 bit 9 must be set                                ;
;                                                                            ;
;       Uses:           esp, XMM4                                            ;
;                                                                            ;
;       Notes:          This macro allows the nesting of CALL_NS routines to ;
;                       be 3-deep.  DO NOT ATTEMPT TO GO DEEPER THAN THAT,   ;
;                       SINCE THE FIRST ADDRESS WILL BE LOST!!!              ;
;                                                                            ;
;----------------------------------------------------------------------------;
%macro CALL_NS 1
        local   retaddr                         ; address to jump back to

        mov     esp, offset retaddr             ; get the return address

        ; Shift XMM register left to make room for the WORD.
        pslldq  xmm4, 0x02

        ; Insert the word
        pinsrw  xmm4, esp, 0x00

        ; Do the above again, but for the high word of esp
        shr     esp, 16
        pslldq  xmm4, 0x02
        pinsrw  xmm4, esp, 0x00
        jmp     %1
retaddr:
%endmacro

;-----  RET_NS  -------------------------------------------------------------;
;                                                                            ;
;       Purpose:        Return to a stackless "call"                         ;
;                                                                            ;
;       Input:          none                                                 ;
;                                                                            ;
;       Output:         none                                                 ;
;                                                                            ;
;       Setup:          CR4 bit 9 must be set                                ;
;                                                                            ;
;       Uses:           esp, XMM4                                            ;
;                                                                            ;
;       Notes:          See the description for CALL_NS for details          ;
;                                                                            ;
;----------------------------------------------------------------------------;
%macro RET_NS 0
        ; Extract the most significant word of return address
        pextrw  xmm4, esp, 0x00
        ; Shift XMM register right to setup the next WORD.
        psrldq  xmm4, 0x02

        ; Do the above again, except for the low word of esp
        shl     esp, 16
        pextrw  xmm4, esp, 0x00
        psrldq  xmm4, 0x02
        jmp     esp
%endmacro
