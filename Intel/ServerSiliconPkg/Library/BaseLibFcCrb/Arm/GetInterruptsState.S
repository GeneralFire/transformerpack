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
GCC_ASM_EXPORT (GetInterruptState)

#/**
#  Retrieves the current CPU interrupt state.
#
#  Returns TRUE is interrupts are currently enabled. Otherwise
#  returns FALSE.
#
#  @retval TRUE  CPU interrupts are enabled.
#  @retval FALSE CPU interrupts are disabled.
#
#**/
#
#BOOLEAN
#EFIAPI
#GetInterruptState (
#  VOID
# );
#
ASM_PFX(GetInterruptState):
    mrs    R0, CPSR
    tst    R0, #0x80  @Check if IRQ is enabled.
    moveq  R0, #1
    movne  R0, #0
    bx     LR
