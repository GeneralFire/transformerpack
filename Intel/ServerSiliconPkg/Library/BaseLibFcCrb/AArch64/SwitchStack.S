## @file
#
# @copyright
# INTEL CONFIDENTIAL
# Copyright 2011 - 2013 Intel Corporation. <BR>
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
.align 5

GCC_ASM_EXPORT(InternalSwitchStackAsm)
GCC_ASM_EXPORT(CpuPause)

/**
//
//  This allows the caller to switch the stack and goes to the new entry point
//
// @param      EntryPoint   The pointer to the location to enter
// @param      Context      Parameter to pass in
// @param      Context2     Parameter2 to pass in
// @param      NewStack     New Location of the stack
//
// @return     Nothing. Goes to the Entry Point passing in the new parameters
//
VOID
EFIAPI
InternalSwitchStackAsm (
  SWITCH_STACK_ENTRY_POINT EntryPoint,
  VOID  *Context,
  VOID  *Context2,
  VOID  *NewStack
  );
**/
ASM_PFX(InternalSwitchStackAsm):
    mov   x29, #0
    mov   x30, x0
    mov   sp, x3
    mov   x0, x1
    mov   x1, x2
    ret

/**
//
//  Requests CPU to pause for a short period of time.
//
//  Requests CPU to pause for a short period of time. Typically used in MP
//  systems to prevent memory starvation while waiting for a spin lock.
//
VOID
EFIAPI
CpuPause (
  VOID
  )
**/
ASM_PFX(CpuPause):
    nop
    nop
    nop
    nop
    nop
    ret
