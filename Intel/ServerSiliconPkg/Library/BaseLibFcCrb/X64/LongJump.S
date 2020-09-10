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
# VOID
# EFIAPI
# InternalLongJump (
#   IN      BASE_LIBRARY_JUMP_BUFFER  *JumpBuffer,
#   IN      UINTN                     Value
#   );
#------------------------------------------------------------------------------
ASM_GLOBAL ASM_PFX(InternalLongJump)
ASM_PFX(InternalLongJump):
    mov     (%rcx), %rbx
    mov     0x8(%rcx), %rsp
    mov     0x10(%rcx), %rbp
    mov     0x18(%rcx), %rdi
    mov     0x20(%rcx), %rsi
    mov     0x28(%rcx), %r12
    mov     0x30(%rcx), %r13
    mov     0x38(%rcx), %r14
    mov     0x40(%rcx), %r15
    # load non-volatile fp registers
    ldmxcsr 0x50(%rcx)
    movdqu  0x58(%rcx), %xmm6
    movdqu  0x68(%rcx), %xmm7
    movdqu  0x78(%rcx), %xmm8
    movdqu  0x88(%rcx), %xmm9
    movdqu  0x98(%rcx), %xmm10
    movdqu  0xA8(%rcx), %xmm11
    movdqu  0xB8(%rcx), %xmm12
    movdqu  0xC8(%rcx), %xmm13
    movdqu  0xD8(%rcx), %xmm14
    movdqu  0xE8(%rcx), %xmm15  
    mov     %rdx, %rax          # set return value
    jmp     *0x48(%rcx)
