; @file
;
; @copyright
; INTEL CONFIDENTIAL
; Copyright 2012 - 2017 Intel Corporation. <BR>
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

;----------------------------------------------------------------------------
section .text code align=16
BITS 32
ALIGN 16
;----------------------------------------------------------------------------

; Externs
extern ASM_PFX(PcdGet8 (PcdSerialDbgLvlAtBadCmos))
extern ASM_PFX(PcdGet8 (PcdCmosDebugPrintLevelReg))
extern ASM_PFX(PcdGet32 (PcdSecInFsp))

;Constant equates
%define CMOS_INDEX 0x70
%define CMOS_DATA 0x71
%define CMOS_STATUS 0xE
%define CMOS_RTCSTS 0x80
%define CMOS_CHECKSUMSTS 0x40

;----------------------------------------------------------------------------
;
; Procedure:  EarlyChpsetInit_OemHook
;
; Description:  This routine provide oem hook after EarlyChipsetInit
;
; Input:  Stack NOT available
;        MMX3 = routine return address
;
; Output:  None
;
; Modified:  All,
;
;----------------------------------------------------------------------------
global ASM_PFX(EarlyChpsetInit_OemHook)
ASM_PFX(EarlyChpsetInit_OemHook):
 ;platform OEM code

  mov eax, [ASM_PFX(PcdGet32 (PcdSecInFsp))]
  cmp eax, 0
  jnz skipInFsp

  mov al, CMOS_STATUS
  out CMOS_INDEX, al
  in al, CMOS_DATA

  and al, CMOS_CHECKSUMSTS | CMOS_RTCSTS
  jz CmosGood
  mov bl, [ASM_PFX(PcdGet8 (PcdSerialDbgLvlAtBadCmos))]
  xor edx, edx                          ; Serial debug message level data default = 00
  cmp bl, 01
  jnz .0
  mov edx, 0x80000000                    ; Minimum level
  jmp UpdateData
.0:
  cmp bl, 02
  jnz .1
  mov edx, 0x80000042                    ; Normal level
  jmp UpdateData
.1:
  cmp bl, 03
  jnz UpdateData
  mov edx, 0xFFFFFFFF                   ; Maximum level

UpdateData:
  mov ecx, 4
  mov al, [ASM_PFX(PcdGet8 (PcdCmosDebugPrintLevelReg))]
.2:
  out CMOS_INDEX, al
  ror eax, 16
  mov al, dl
  out CMOS_DATA, al
  shr edx, 8
  ror eax, 16
  inc al
  loop .2

CmosGood:
skipInFsp:

 ;the following code is RET_ESI_MMX3
  movd    esi, mm3                      ; restore return address from MM3
  jmp     esi                           ; Absolute jump

;----------------------------------------------------------------------------
;
; Procedure:  LateChipsetInit_OemHook
;
; Description:  This routine provide oem hook after LateChipsetInit
;
; Input:  none
;
; Output:  None
;
; Modified:  All, except EBP and ESP
;
;----------------------------------------------------------------------------
global ASM_PFX(LateChipsetInit_OemHook)
ASM_PFX(LateChipsetInit_OemHook):
 ;platform OEM code
  ret
