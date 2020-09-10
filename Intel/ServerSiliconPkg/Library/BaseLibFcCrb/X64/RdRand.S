## @file
#
# @copyright
# INTEL CONFIDENTIAL
# Copyright 2015 - 2016 Intel Corporation. <BR>
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

//------------------------------------------------------------------------------
//  Generates a 16 bit random number through RDRAND instruction.
//  Return TRUE if Rand generated successfully, or FALSE if not.
//
//  BOOLEAN EFIAPI InternalX86RdRand16 (UINT16 *Rand);
//------------------------------------------------------------------------------
ASM_GLOBAL ASM_PFX(InternalX86RdRand16)
ASM_PFX(InternalX86RdRand16):
    .byte  0x0f, 0xc7, 0xf0        // rdrand r16: "0f c7 /6  ModRM:r/m(w)"
    jc     rn16_ok                 // jmp if CF=1
    xor    %rax, %rax              // reg=0 if CF=0
    ret                            // return with failure status
rn16_ok:
    mov    %ax, (%rcx)
    mov    $0x1, %rax
    ret

//------------------------------------------------------------------------------
//  Generates a 32 bit random number through RDRAND instruction.
//  Return TRUE if Rand generated successfully, or FALSE if not.
//
//  BOOLEAN EFIAPI InternalX86RdRand32 (UINT32 *Rand);
//------------------------------------------------------------------------------
ASM_GLOBAL ASM_PFX(InternalX86RdRand32)
ASM_PFX(InternalX86RdRand32):
    .byte  0x0f, 0xc7, 0xf0        // rdrand r32: "0f c7 /6  ModRM:r/m(w)"
    jc     rn32_ok                 // jmp if CF=1
    xor    %rax, %rax              // reg=0 if CF=0
    ret                            // return with failure status
rn32_ok:
    mov    %eax, (%rcx)
    mov    $0x1, %rax
    ret

//------------------------------------------------------------------------------
//  Generates a 64 bit random number through RDRAND instruction.
//  Return TRUE if Rand generated successfully, or FALSE if not.
//
//  BOOLEAN EFIAPI InternalX86RdRand64 (UINT64 *Rand);
//------------------------------------------------------------------------------
ASM_GLOBAL ASM_PFX(InternalX86RdRand64)
ASM_PFX(InternalX86RdRand64):
    .byte  0x48, 0x0f, 0xc7, 0xf0  // rdrand r64: "REX.W + 0f c7 /6 ModRM:r/m(w)"
    jc     rn64_ok                 // jmp if CF=1
    xor    %rax, %rax              // reg=0 if CF=0
    ret                            // return with failure status
rn64_ok:
    mov    %rax, (%rcx)
    mov    $0x1, %rax
    ret
