; @file CpuQuiesceHandler.nasm
;  This file contains supporting procs for Quiesce
;
; @copyright
; INTEL CONFIDENTIAL
; Copyright 1999 - 2018 Intel Corporation. <BR>
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

SECTION .text
;
;BKL_PORTING:
;   The Quiesce code needs to be updated for Platform
;

;extern ASM_PFX(mMirrorEnabled)

%define QUIESCE_CACHE_CODE_SIZE 0x10000 ; 64K

%define MKF_I_AM_MONARCH 0xff
%define MKF_I_AM_MONARCH_AP 0xfe

%define MONARCH_STACK_OFFSET 0x30000
%define MONARCH_AP_STACK_OFFSET (MONARCH_STACK_OFFSET - 0x400) ; 1K apart
%define QUIESCE_CACHE_DATA_SIZE 0x10000 ; 64K

%define QUIESCE_CTL1_MSR 0x50
%define B_QUIESCE_CTL1_QUIESCE 0x1   ; bit0
%define B_QUIESCE_CTL1_UNQUIESCE 0x2   ; bit1
%define B_QUIESCE_CTL1_UNCORE_FENCE 04    ; bit2

%define MSR_MISC_FEATURE_CONTROL 0x1A4

;------------------------------------------------------------------------------
; VOID
; SaveMisc_Feature_Msr (
;   rcx //pointer to memory where Msr value being saved
;   );
;------------------------------------------------------------------------------
global ASM_PFX(Save_Misc_Feature_Msr)
ASM_PFX(Save_Misc_Feature_Msr):

    push rbx

    mov rbx, rcx       ;svae pointer
    mov ecx, MSR_MISC_FEATURE_CONTROL
    rdmsr

    and rax, 0xFFFFFFFF
    shl rdx, 32
    or  rax, rdx

    mov qword [ebx], rax
    mfence

    pop rbx
    ret

;return UINT64
global ASM_PFX(GetTimerTick)
ASM_PFX(GetTimerTick):
    rdtsc
    shl     rdx, 0x20
    or      rax, rdx
    ret

;return EFI_STATUS
global ASM_PFX(QuiesceAcquireLock)
ASM_PFX(QuiesceAcquireLock):

    mov         al, 0xFF

    xchg        al, byte [rcx]
    cmp         al, 0
    jz          GetLock

GetLock:
    ret

global ASM_PFX(QuiesceReleaseLock)
ASM_PFX(QuiesceReleaseLock):

    mov         al, 0
    xchg        al, byte [rcx]

    ret

;------------------------------------------------------------------------------
;EFI_STATUS
;DoQuiesce (
;  IN UINT64  CacheBase,                //rcx
;  IN UINT64 MMCfgBase,             //rdx
;  IN UINT64 MiscFeatureControlValue, //r8
;  IN BOOLEAN  SMTFlag              //r9
;  );
; r13b: Mirror Enable flag used in Quiesce cache code
;------------------------------------------------------------------------------
global ASM_PFX(DoQuiesce)
ASM_PFX(DoQuiesce):

  push rbp
  push rbx
  push rsp
  push r13

  ;mov r13b, [ASM_PFX(mMirrorEnabled)] ; get mirror enable flag
  mov rbp, rcx          ; save quiesce base

  ; save old cr3
  mov rax, cr3
  push rax                ; save old cr3 in old stack
  ; now setup Stack from Quiesce
  mov rax, rsp
  add rcx, MONARCH_STACK_OFFSET
  mov rsp, rcx      ; set stack on quiesce
  push rax          ; save old stack pointer

  mov  r10, MKF_I_AM_MONARCH
  call rbp

  mov rbx, rax      ; save return Status in rbx

  ; restore old statck
  pop rax           ;
  mov rsp, rax      ; restore old rsp back

  pop rax           ; get old cr3 back
  mov cr3, rax

  mov rax, rbx      ; return Status in rax

  pop r13
  pop rsp
  pop rbx
  pop rbp

  ret

;;------------------------------------------------------------------------------
;EFI_STATUS
;Monarch_Ap_DoQuiesce(
;  IN UINT64 CacheBase,               //rcx
;  IN EFI_PHYSICAL_ADDRESS MMCFGBase, //rdx
;  IN UINT64 MiscFeatureControlValue, //r8
;  BOOLEAN   SMTFlag                  //r9 always 1
;  );
;------------------------------------------------------------------------------
global ASM_PFX(Monarch_Ap_DoQuiesce)
ASM_PFX(Monarch_Ap_DoQuiesce):

  push rbp
  push rbx
  push rsp

  mov rbp, rcx          ; save quiesce base

  ; save old cr3
  mov rax, cr3
  push rax                ; save old cr3 in old stack

  ; now save old rsp and setp up Monarch AP stack for Quiesce
  add rcx, MONARCH_AP_STACK_OFFSET  ;point to (last address - 1K) of cache
  mov rbx, rsp
  mov rsp, rcx      ; update rsp to pointer new stack
  push rbx          ; save old stack pointer in cache data stack

  mov  r10, MKF_I_AM_MONARCH_AP
  call rbp          ; run out of Cache

  mov rbx, rax      ; save return Status
  ; restore old statck
  pop rax           ;
  mov rsp, rax      ; restore old rsp back

  pop rax           ; get old cr3
  mov cr3, rax

  mov rax, rbx      ; restore return Status

  pop rsp
  pop rbx
  pop rbp

  ret

