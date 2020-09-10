## @file
#
# @copyright
# INTEL CONFIDENTIAL
# Copyright 2009 - 2010 Intel Corporation. <BR>
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
	.align 2
	GCC_ASM_EXPORT(InternalMathLShiftU64)

ASM_PFX(InternalMathLShiftU64):
	stmfd	sp!, {r4, r5, r6}
	mov	r6, r1
	rsb	ip, r2, #32
	mov	r4, r6, asl r2
	subs	r1, r2, #32
	orr	r4, r4, r0, lsr ip
	mov	r3, r0, asl r2
	movpl	r4, r0, asl r1
	mov	r5, r0
	mov	r0, r3
	mov	r1, r4
	ldmfd	sp!, {r4, r5, r6}
	bx	lr

	.align 2
	GCC_ASM_EXPORT(InternalMathRShiftU64)

ASM_PFX(InternalMathRShiftU64):
	stmfd	sp!, {r4, r5, r6}
	mov	r5, r0
	rsb	ip, r2, #32
	mov	r3, r5, lsr r2
	subs	r0, r2, #32
	orr	r3, r3, r1, asl ip
	mov	r4, r1, lsr r2
	movpl	r3, r1, lsr r0
	mov	r6, r1
	mov	r0, r3
	mov	r1, r4
	ldmfd	sp!, {r4, r5, r6}
	bx	lr

	.align 2
	GCC_ASM_EXPORT(InternalMathARShiftU64)

ASM_PFX(InternalMathARShiftU64):
	stmfd	sp!, {r4, r5, r6}
	mov	r5, r0
	rsb	ip, r2, #32
	mov	r3, r5, lsr r2
	subs	r0, r2, #32
	orr	r3, r3, r1, asl ip
	mov	r4, r1, asr r2
	movpl	r3, r1, asr r0
	mov	r6, r1
	mov	r0, r3
	mov	r1, r4
	ldmfd	sp!, {r4, r5, r6}
	bx	lr

	.align 2
	GCC_ASM_EXPORT(InternalMathLRotU64)

ASM_PFX(InternalMathLRotU64):
	stmfd	sp!, {r4, r5, r6, r7, lr}
	add	r7, sp, #12
	mov	r6, r1
	rsb	ip, r2, #32
	mov	r4, r6, asl r2
	rsb	lr, r2, #64
	subs	r1, r2, #32
	orr	r4, r4, r0, lsr ip
	mov	r3, r0, asl r2
	movpl	r4, r0, asl r1
	sub	ip, r2, #32
	mov	r5, r0
	mov	r0, r0, lsr lr
	rsbs	r2, r2, #32
	orr	r0, r0, r6, asl ip
	mov	r1, r6, lsr lr
	movpl	r0, r6, lsr r2
	orr	r1, r1, r4
	orr	r0, r0, r3
	ldmfd	sp!, {r4, r5, r6, r7, pc}


	.align 2
	GCC_ASM_EXPORT(InternalMathRRotU64)

ASM_PFX(InternalMathRRotU64):
	stmfd	sp!, {r4, r5, r6, r7, lr}
	add	r7, sp, #12
	mov	r5, r0
	rsb	ip, r2, #32
	mov	r3, r5, lsr r2
	rsb	lr, r2, #64
	subs	r0, r2, #32
	orr	r3, r3, r1, asl ip
	mov	r4, r1, lsr r2
	movpl	r3, r1, lsr r0
	sub	ip, r2, #32
	mov	r6, r1
	mov	r1, r1, asl lr
	rsbs	r2, r2, #32
	orr	r1, r1, r5, lsr ip
	mov	r0, r5, asl lr
	movpl	r1, r5, asl r2
	orr	r0, r0, r3
	orr	r1, r1, r4
	ldmfd	sp!, {r4, r5, r6, r7, pc}

	.align 2
	GCC_ASM_EXPORT(InternalMathMultU64x32)

ASM_PFX(InternalMathMultU64x32):
	stmfd	sp!, {r7, lr}
	add	r7, sp, #0
	mov	r3, #0
	mov	ip, r0
	mov	lr, r1
	umull	r0, r1, ip, r2
	mla	r1, lr, r2, r1
	mla	r1, ip, r3, r1
	ldmfd	sp!, {r7, pc}

	.align 2
	GCC_ASM_EXPORT(InternalMathMultU64x64)

ASM_PFX(InternalMathMultU64x64):
	stmfd	sp!, {r7, lr}
	add	r7, sp, #0
	mov	ip, r0
	mov	lr, r1
	umull	r0, r1, ip, r2
	mla	r1, lr, r2, r1
	mla	r1, ip, r3, r1
	ldmfd	sp!, {r7, pc}

	.align 2
	GCC_ASM_EXPORT(InternalMathDivU64x32)

ASM_PFX(InternalMathDivU64x32):
	stmfd	sp!, {r7, lr}
	add	r7, sp, #0
	mov	r3, #0
	bl 	ASM_PFX(__udivdi3)
	ldmfd	sp!, {r7, pc}
	
	
	.align 2
	GCC_ASM_EXPORT(InternalMathModU64x32)

ASM_PFX(InternalMathModU64x32):
	stmfd	sp!, {r7, lr}
	add	r7, sp, #0
	mov	r3, #0
	bl 	ASM_PFX(__umoddi3)
	ldmfd	sp!, {r7, pc}
	
	
	.align 2
	GCC_ASM_EXPORT(InternalMathDivRemU64x32)

ASM_PFX(InternalMathDivRemU64x32):
	stmfd	sp!, {r4, r5, r6, r7, lr}
	add	r7, sp, #12
	stmfd	sp!, {r10, r11}
	subs	r6, r3, #0
	mov	r10, r0
	mov	r11, r1
	moveq	r4, r2
	moveq	r5, #0
	beq	L22
	mov	r4, r2
	mov	r5, #0
	mov	r3, #0
	bl 	ASM_PFX(__umoddi3)
	str	r0, [r6, #0]
L22:
	mov	r0, r10
	mov	r1, r11
	mov	r2, r4
	mov	r3, r5
	bl 	ASM_PFX(__udivdi3)
	ldmfd	sp!, {r10, r11}
	ldmfd	sp!, {r4, r5, r6, r7, pc}
	
	
	.align 2
	GCC_ASM_EXPORT(InternalMathDivRemU64x64)

ASM_PFX(InternalMathDivRemU64x64):
	stmfd	sp!, {r4, r5, r6, r7, lr}
	add	r7, sp, #12
	stmfd	sp!, {r10, r11}
	ldr	r6, [sp, #28]
	mov	r4, r0
	cmp	r6, #0
	mov	r5, r1
	mov	r10, r2
	mov	r11, r3
	beq	L26
	bl 	ASM_PFX(__umoddi3)
	stmia	r6, {r0-r1}
L26:
	mov	r0, r4
	mov	r1, r5
	mov	r2, r10
	mov	r3, r11
	bl 	ASM_PFX(__udivdi3)
	ldmfd	sp!, {r10, r11}
	ldmfd	sp!, {r4, r5, r6, r7, pc}
	
	
	.align 2
	GCC_ASM_EXPORT(InternalMathDivRemS64x64)

ASM_PFX(InternalMathDivRemS64x64):
	stmfd	sp!, {r4, r5, r6, r7, lr}
	add	r7, sp, #12
	stmfd	sp!, {r10, r11}
	ldr	r6, [sp, #28]
	mov	r4, r0
	cmp	r6, #0
	mov	r5, r1
	mov	r10, r2
	mov	r11, r3
	beq	L30
	bl 	ASM_PFX(__moddi3)
	stmia	r6, {r0-r1}
L30:
	mov	r0, r4
	mov	r1, r5
	mov	r2, r10
	mov	r3, r11
	bl 	ASM_PFX(__divdi3)
	ldmfd	sp!, {r10, r11}
	ldmfd	sp!, {r4, r5, r6, r7, pc}
	
	
	.align 2
	GCC_ASM_EXPORT(InternalMathSwapBytes64)

ASM_PFX(InternalMathSwapBytes64):
	stmfd	sp!, {r4, r5, r7, lr}
	mov	r5, r1
	bl	ASM_PFX(SwapBytes32)
	mov	r4, r0
	mov	r0, r5
	bl	ASM_PFX(SwapBytes32)
	mov	r1, r4
	ldmfd	sp!, {r4, r5, r7, pc}


ASM_FUNCTION_REMOVE_IF_UNREFERENCED
