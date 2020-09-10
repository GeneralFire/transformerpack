## @file
#
# @copyright
# INTEL CONFIDENTIAL
# Copyright 2006 - 2008 Intel Corporation. <BR>
#
# The source code contained or described herein and all documents related to the
# source code ("Material") are owned by Intel Corporation or its suppliers or
# licensors. Title to the Material remains with Intel Corporation or its suppliers
# and licensors. The Material may contain trade secrets and proprietary    and
# confidential information of Intel Corporation and its suppliers and licensors,
# and is protected by worldwide copyright and trade secret laws and treaty
# provisions. No part of the Material may be used, copied, reproduced, modified,
# published, uploaded, posted, transmitted, distributed, or disclosed in any way
# without Intel's prior express written permission.
#
# No license under any patent, copyright, trade secret or other intellectual
# property right is granted to or conferred upon you by disclosure or delivery
# of the Materials, either expressly, by implication, inducement, estoppel or
# otherwise. Any license under such intellectual property rights must be
# express and approved by Intel in writing.
#
# Unless otherwise agreed by Intel in writing, you may not remove or alter
# this notice or any other notice embedded in Materials by Intel or
# Intel's suppliers or licensors in any way.
##

#------------------------------------------------------------------------------
#  VOID
#  EFIAPI
#  AsmCpuid (
#    IN   UINT32  RegisterInEax,
#    OUT  UINT32  *RegisterOutEax  OPTIONAL,
#    OUT  UINT32  *RegisterOutEbx  OPTIONAL,
#    OUT  UINT32  *RegisterOutEcx  OPTIONAL,
#    OUT  UINT32  *RegisterOutEdx  OPTIONAL
#    )
#------------------------------------------------------------------------------
ASM_GLOBAL ASM_PFX(AsmCpuid)
ASM_PFX(AsmCpuid):
    push    %rbx
    mov     %ecx, %eax
    push    %rax                         # save Index on stack
    push    %rdx
    cpuid
    test    %r9, %r9
    jz      L1
    mov     %ecx, (%r9)
L1:
    pop     %rcx
    jrcxz   L2
    mov     %eax, (%rcx)
L2:
    mov     %r8, %rcx
    jrcxz   L3
    mov     %ebx, (%rcx)
L3:
    mov     0x38(%rsp), %rcx
    jrcxz   L4
    mov     %edx, (%rcx)
L4:
    pop     %rax                         # restore Index to rax as return value
    pop     %rbx
    ret
