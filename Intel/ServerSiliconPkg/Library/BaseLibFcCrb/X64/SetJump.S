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

ASM_GLOBAL ASM_PFX(SetJump)
ASM_PFX(SetJump):
    push   %rcx
    add    $0xffffffffffffffe0,%rsp
    call   ASM_PFX(InternalAssertJumpBuffer)
    add    $0x20,%rsp
    pop    %rcx
    pop    %rdx
    mov    %rbx,(%rcx)
    mov    %rsp,0x8(%rcx)
    mov    %rbp,0x10(%rcx)
    mov    %rdi,0x18(%rcx)
    mov    %rsi,0x20(%rcx)
    mov    %r12,0x28(%rcx)
    mov    %r13,0x30(%rcx)
    mov    %r14,0x38(%rcx)
    mov    %r15,0x40(%rcx)
    mov    %rdx,0x48(%rcx)
    # save non-volatile fp registers
    stmxcsr 0x50(%rcx)
    movdqu  %xmm6, 0x58(%rcx) 
    movdqu  %xmm7, 0x68(%rcx)
    movdqu  %xmm8, 0x78(%rcx)
    movdqu  %xmm9, 0x88(%rcx)
    movdqu  %xmm10, 0x98(%rcx)
    movdqu  %xmm11, 0xA8(%rcx)
    movdqu  %xmm12, 0xB8(%rcx)
    movdqu  %xmm13, 0xC8(%rcx)
    movdqu  %xmm14, 0xD8(%rcx)
    movdqu  %xmm15, 0xE8(%rcx)     
    xor    %rax,%rax
    jmpq   *%rdx
