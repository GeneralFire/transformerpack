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

%define MSR_TKRMODE_PATCH 0x52
%define MSR_BBOX_FEATURE_PATCH 0x55
%define MSR_BBOX_FEATURE_PATCH1 0x56

%define FW_SCRATCH_22 0xFEB204D8
%define PLAT_MISC_CTRL 0xFEB20294

%define BMP_C0_STEPPING 0x5

%define BMP_MODEL 0xE
%define WSM_EX_MODEL 0xF

;------------------------------------------------------------------------------
;UINT8
;GetCpuSteppingx64 (
;  VOID
;  );
;------------------------------------------------------------------------------
global ASM_PFX(GetCpuSteppingx64)
ASM_PFX(GetCpuSteppingx64):

  Push     Rbx
  Push     Rcx
  Push     Rdx

  Mov      Rax, 1
  Cpuid
  And      Rax, 0xF ; Extract Stepping ID

  Pop      Rdx
  Pop      Rcx
  Pop      Rbx
  ret

;------------------------------------------------------------------------------
;UINT8
;GetCpuModelx64 (
;  VOID
;  );
;------------------------------------------------------------------------------
global ASM_PFX(GetCpuModelx64)
ASM_PFX(GetCpuModelx64):

  Push     Rbx
  Push     Rcx
  Push     Rdx

  Mov      Rax, 1
  Cpuid
  And      Rax, 0xF0 ; Extract Stepping ID
  shr      Rax, 4;

  Pop      Rdx
  Pop      Rcx
  Pop      Rbx
  ret

global ASM_PFX(EfiDisableInterrupts_1)
ASM_PFX(EfiDisableInterrupts_1):
    cli
    ret

global ASM_PFX(EfiHalt_1)
ASM_PFX(EfiHalt_1):
    hlt
    ret

;------------------------------------------------------------------------------
;This function is called in RasMpLinkMileStone12() after RasMpLinkMileStone10().
;RasMpLinkMileStone10() handles coherence and updates UBx_CR_U_PCSR_FW_SCRATCH_22.
;Therefore, entering here, UBx_CR_U_PCSR_FW_SCRATCH_22 ready has the bits value for APs to write to MSR's
;In addition to MSR_TKRMODE_PATCH 0x52, MSR_BBOX_FEATURE_PATCH 0x55 is added to update BB0_CR_B_CCSR_DEBUG_REG1
;
;Apply the tracker mode patch for BMP sighting S3472299 (ECO_ID: 774905).
;Apply to only CPUs with NHM-EX stepping C0 and above
;
;RAS_MPLINK_STATUS
;TkrModePatchRas(
;  VOID
;  );
;------------------------------------------------------------------------------
global ASM_PFX(TkrModePatchRas)
ASM_PFX(TkrModePatchRas):

  Push     Rbx
  Push     Rcx
  Push     Rdx
  Push     Rsi

  Mov      Ecx, MSR_TKRMODE_PATCH
  Rdmsr
  And      Eax, 0xFFFFFFFD  ; Clear the tracker mode patch bit
  Mov      Esi, FW_SCRATCH_22 ;
  Mov      Ebx, Dword [esi]
  Test     Ebx, 0x100 ; BIT 8 is updated by SBSP with the setup value
  Jz       .0
  Or       Eax, 2  ; Set the tracker mode patch bit

.0:
  Or       Eax, 0x80  ; Set the lock bit
  Wrmsr

  call     ASM_PFX(GetCpuModelx64)
  cmp      al, WSM_EX_MODEL
  jne      TkrModePatchRasExit

  Mov      Esi, PLAT_MISC_CTRL
  Mov      Dword [Esi], 1           ; set Locked Tap Override for the pbox margining registers
  Mov      Ecx, MSR_BBOX_FEATURE_PATCH
  Rdmsr
  And      Eax, 0xFC7FFFFF              ; Clear bit[25:23]
  And      Edx, 0xFC7FFFFF              ; Clear bit[25:23]
  Mov      Esi, FW_SCRATCH_22           ;
  Mov      Esi, Dword [Esi]         ; Bit[30:28] has the 3 bit values
  Shr      Esi, 5                       ; bit 28-23
  And      Esi, 0x3800000              ; Extract bit[25:23]
  or       Eax, Esi
  or       Edx, Esi
  Wrmsr

  Mov      Ecx, MSR_BBOX_FEATURE_PATCH1
  Rdmsr
  And      Eax, ~ (0x30000000)         ; clear dispclscnflt(BIT29) and frcfwdinvitoe(BIT28)
  And      Edx, ~ (0x30000000)         ;
  Test     Esi, 0x800000              ; apply DAS w/a if DAS is enabled, i.e. MSR_BBOX_FEATURE_PATCH1 bit23
  jz       .1                           ;
  Or       Eax, 0x30000000               ; set dispclscnflt(BIT29) and frcfwdinvitoe(BIT28)
  Or       Edx, 0x30000000
.1:
  Wrmsr

  Mov      Esi, PLAT_MISC_CTRL
  Mov      Dword [Esi], 0           ; clear Locked Tap Override for the pbox margining registers

TkrModePatchRasExit:
  Xor      Rax, Rax	                ; Indicate Success
  Pop      Rsi
  Pop      Rdx
  Pop      Rcx
  Pop      Rbx
  ret
