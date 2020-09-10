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

ASM_GLOBAL ASM_PFX(InternalX86DisablePaging32)

#------------------------------------------------------------------------------
# VOID
# EFIAPI
# InternalX86DisablePaging32 (
#   IN      SWITCH_STACK_ENTRY_POINT  EntryPoint,
#   IN      VOID                      *Context1,    OPTIONAL
#   IN      VOID                      *Context2,    OPTIONAL
#   IN      VOID                      *NewStack
#   );
#------------------------------------------------------------------------------
ASM_PFX(InternalX86DisablePaging32):
    movl    4(%esp), %ebx
    movl    8(%esp), %ecx
    movl    12(%esp), %edx
    pushfl
    pop     %edi                        # save EFLAGS to edi
    cli
    movl    %cr0, %eax
    btrl    $31, %eax
    movl    16(%esp), %esp
    movl    %eax, %cr0
    push    %edi
    popfl                               # restore EFLAGS from edi
    push    %edx
    push    %ecx
    call    *%ebx
    jmp     .                           # EntryPoint() should not return
