## @file
#
# @copyright
# INTEL CONFIDENTIAL
# Copyright 2008 - 2009 Intel Corporation. <BR>
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

.text
.p2align 2

GCC_ASM_EXPORT(SetJump)
GCC_ASM_EXPORT(InternalLongJump)

#/**
#  Saves the current CPU context that can be restored with a call to LongJump() and returns 0.#
#
#  Saves the current CPU context in the buffer specified by JumpBuffer and returns 0.  The initial
#  call to SetJump() must always return 0.  Subsequent calls to LongJump() cause a non-zero
#  value to be returned by SetJump().
#
#  If JumpBuffer is NULL, then ASSERT().
#  For IPF CPUs, if JumpBuffer is not aligned on a 16-byte boundary, then ASSERT().
#
#  @param  JumpBuffer    A pointer to CPU context buffer.
#
#**/
#
#UINTN
#EFIAPI
#SetJump (
#  IN      BASE_LIBRARY_JUMP_BUFFER  *JumpBuffer  // R0
#  );
#
ASM_PFX(SetJump):
  mov   r3, r13
  stmia r0, {r3-r12,r14}
  eor   r0, r0, r0
  bx    lr

#/**
#  Restores the CPU context that was saved with SetJump().#
#
#  Restores the CPU context from the buffer specified by JumpBuffer.
#  This function never returns to the caller.
#  Instead is resumes execution based on the state of JumpBuffer.
#
#  @param  JumpBuffer    A pointer to CPU context buffer.
#  @param  Value         The value to return when the SetJump() context is restored.
#
#**/
#VOID
#EFIAPI
#InternalLongJump (
#  IN      BASE_LIBRARY_JUMP_BUFFER  *JumpBuffer,  // R0
#  IN      UINTN                     Value         // R1
#  );
#
ASM_PFX(InternalLongJump):
  ldmia  r0, {r3-r12,r14}
  mov    r13, r3
  mov    r0, r1
  bx     lr

ASM_FUNCTION_REMOVE_IF_UNREFERENCED
