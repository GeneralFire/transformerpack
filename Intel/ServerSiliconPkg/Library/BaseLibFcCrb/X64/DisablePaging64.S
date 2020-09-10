## @file
#
# @copyright
# INTEL CONFIDENTIAL
# Copyright 2006 - 2009 Intel Corporation. <BR>
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
# InternalX86DisablePaging64 (
#   IN      UINT16                    Cs,
#   IN      UINT32                    EntryPoint,
#   IN      UINT32                    Context1,  OPTIONAL
#   IN      UINT32                    Context2,  OPTIONAL
#   IN      UINT32                    NewStack
#   );
#------------------------------------------------------------------------------

ASM_GLOBAL ASM_PFX(InternalX86DisablePaging64)
ASM_PFX(InternalX86DisablePaging64):
    cli    
    lea    L1(%rip), %rsi                 # rsi <- The start address of transition code
    mov    0x28(%rsp), %edi               # rdi <- New stack
    lea    _mTransitionEnd(%rip), %rax    # rax <- end of transition code
    sub    %rsi, %rax                     # rax <- The size of transition piece code
    add    $4, %rax                       # round rax up to the next 4 byte boundary
    and    $0xfc, %al
    sub    %rax, %rdi                     # rdi <- use stack to hold transition code 
    mov    %edi, %r10d                    # r10 <- The start address of transicition code below 4G
    push   %rcx                           # save rcx to stack
    mov    %rax, %rcx                     # rcx <- The size of transition piece code
    rep
    movsb                                 # copy transition code to (new stack - 64byte) below 4G
    pop    %rcx                           # restore rcx
   
    mov    %r8d, %esi 
    mov    %r9d, %edi 
    mov    %r10d, %eax
    sub    $4, %eax
    push   %rcx                           # push Cs to stack
    push   %r10                           # push address of transition code on stack  
    .byte  0x48, 0xcb                     # retq: Use far return to load CS register from stack
                                          # (Use raw byte code since some GNU assemblers generates incorrect code for "retq")  
L1:
    mov    %eax,%esp                      # set up new stack
    mov    %cr0,%rax
    btr    $0x1f,%eax                     # clear CR0.PG
    mov    %rax,%cr0                      # disable paging

    mov    %edx,%ebx                      # save EntryPoint to ebx, for rdmsr will overwrite edx
    mov    $0xc0000080,%ecx
    rdmsr  
    and    $0xfe,%ah                      # clear LME
    wrmsr  
    mov    %cr4,%rax
    and    $0xdf,%al                      # clear PAE
    mov    %rax,%cr4
    push   %rdi                           # push Context2
    push   %rsi                           # push Context1
    callq  *%rbx                          # transfer control to EntryPoint
    jmp    .                              # no one should get here

_mTransitionEnd :
