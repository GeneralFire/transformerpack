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

ASM_GLOBAL ASM_PFX(InternalMathDivRemU64x32)

#------------------------------------------------------------------------------
# UINT64
# EFIAPI
# InternalMathDivRemU64x32 (
#   IN      UINT64                    Dividend,
#   IN      UINT32                    Divisor,
#   OUT     UINT32                    *Remainder
#   );
#------------------------------------------------------------------------------
ASM_PFX(InternalMathDivRemU64x32):
    movl    12(%esp), %ecx          # ecx <- divisor
    movl    8(%esp), %eax           # eax <- dividend[32..63]
    xorl    %edx, %edx
    divl    %ecx                    # eax <- quotient[32..63], edx <- remainder
    push    %eax
    movl    8(%esp), %eax           # eax <- dividend[0..31]
    divl    %ecx                    # eax <- quotient[0..31]
    movl    20(%esp), %ecx          # ecx <- Remainder
    jecxz   L1                      # abandon remainder if Remainder == NULL
    movl    %edx, (%ecx)
L1:
    pop     %edx                    # edx <- quotient[32..63]
    ret
