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

ASM_GLOBAL  ASM_PFX(AsmCpuid)

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
ASM_PFX(AsmCpuid):
    push    %ebx
    push    %ebp
    movl    %esp, %ebp
    movl    12(%ebp), %eax
    cpuid
    push    %ecx
    movl    16(%ebp), %ecx
    jecxz   L1
    movl    %eax, (%ecx)
L1:
    movl    20(%ebp), %ecx
    jecxz   L2
    movl    %ebx, (%ecx)
L2:
    movl    24(%ebp), %ecx
    jecxz   L3
    popl    (%ecx)
L3:
    movl    28(%ebp), %ecx
    jecxz   L4
    movl    %edx, (%ecx)
L4:
    movl    12(%ebp), %eax
    leave
    pop     %ebx
    ret
