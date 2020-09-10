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

ASM_GLOBAL ASM_PFX(InternalX86EnablePaging64)

#------------------------------------------------------------------------------
# VOID
# EFIAPI
# InternalX86EnablePaging64 (
#   IN      UINT16                    CodeSelector,
#   IN      UINT64                    EntryPoint,
#   IN      UINT64                    Context1,    OPTIONAL
#   IN      UINT64                    Context2,    OPTIONAL
#   IN      UINT64                    NewStack
#   );
#------------------------------------------------------------------------------
ASM_PFX(InternalX86EnablePaging64):
    cli
    movl    $LongStart, (%esp)          # offset for far retf, seg is the 1st arg
    movl    %cr4, %eax
    orb     $0x20, %al
    movl    %eax, %cr4                  # enable PAE
    movl    $0xc0000080, %ecx
    rdmsr
    orb     $1, %ah                     # set LME
    wrmsr
    movl    %cr0, %eax
    btsl    $31, %eax                   # set PG
    movl    %eax, %cr0                  # enable paging
    lret                                # topmost 2 dwords hold the address
LongStart:                              # long mode starts here
    .byte   0x67, 0x48                  # 32-bit address size, 64-bit operand size
    movl    (%esp), %ebx                # mov rbx, [esp]
    .byte   0x67, 0x48
    movl    8(%esp), %ecx               # mov rcx, [esp + 8]
    .byte   0x67, 0x48
    movl    0x10(%esp), %edx            # mov rdx, [esp + 10h]
    .byte   0x67, 0x48
    movl    0x18(%esp), %esp            # mov rsp, [esp + 18h]
    .byte   0x48
    addl    $-0x20, %esp                # add rsp, -20h
    call    *%ebx                       # call rbx
    jmp     .                           # no one should get here
