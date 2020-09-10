; @file
;
; @copyright
; INTEL CONFIDENTIAL
; Copyright 2016 - 2011 Intel Corporation. <BR>
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

;------------------------------------------------------------------------------
; VOID
; EFIAPI
; InternalSwitchStack (
;   IN      SWITCH_STACK_ENTRY_POINT  EntryPoint,
;   IN      VOID                      *Context1,   OPTIONAL
;   IN      VOID                      *Context2,   OPTIONAL
;   IN      VOID                      *NewStack
;   );
;------------------------------------------------------------------------------
global ASM_PFX(InternalSwitchStack)
ASM_PFX(InternalSwitchStack):
  push  ebp
  mov   ebp, esp

  mov   esp, [ebp + 20]    ; switch stack
  sub   esp, 8
  mov   eax, [ebp + 16]
  mov   [esp + 4], eax
  mov   eax, [ebp + 12]
  mov   [esp], eax
  push  0                  ; keeps gdb from unwinding stack
  jmp   dword [ebp + 8]    ; call and never return
