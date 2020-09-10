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

.auto
.text

ASM_GLOBAL InternalAssertJumpBuffer
.type   InternalAssertJumpBuffer, @function

.proc   SetJump
.type   SetJump, @function
SetJump::
        alloc               loc0 = ar.pfs, 1, 2, 1, 0
        mov                 loc1 = b0
        mov                 out0 = in0

        brl.call.sptk.many  b0  = InternalAssertJumpBuffer

        mov                 r14 = ar.unat
        mov                 r15 = ar.bsp
        add                 r10 = 0x10*20, in0

        stf.spill.nta       [in0] = f2, 0x10
        st8.spill.nta       [r10] = r4, 8
        mov                 r21 = b1

        stf.spill.nta       [in0] = f3, 0x10
        st8.spill.nta       [r10] = r5, 8
        mov                 r22 = b2

        stf.spill.nta       [in0] = f4, 0x10
        st8.spill.nta       [r10] = r6, 8
        mov                 r23 = b3

        stf.spill.nta       [in0] = f5, 0x10
        st8.spill.nta       [r10] = r7, 8
        mov                 r24 = b4

        stf.spill.nta       [in0] = f16, 0x10
        st8.spill.nta       [r10] = sp, 8
        mov                 r25 = b5

        stf.spill.nta       [in0] = f17, 0x10
        st8.nta             [r10] = loc1, 8
        mov                 r16 = pr

        stf.spill.nta       [in0] = f18, 0x10
        st8.nta             [r10] = r21, 8
        mov                 r17 = ar.lc

        stf.spill.nta       [in0] = f19, 0x10
        st8.nta             [r10] = r22, 8

        stf.spill.nta       [in0] = f20, 0x10
        st8.nta             [r10] = r23, 8

        stf.spill.nta       [in0] = f21, 0x10
        st8.nta             [r10] = r24, 8

        stf.spill.nta       [in0] = f22, 0x10
        st8.nta             [r10] = r25, 8

        stf.spill.nta       [in0] = f23, 0x10
        mov                 r18 = ar.unat

        stf.spill.nta       [in0] = f24, 0x10
        st8.nta             [r10] = r14, 8          // UNAT

        stf.spill.nta       [in0] = f25, 0x10
        st8.nta             [r10] = r18, 8          // UNAT after spill

        stf.spill.nta       [in0] = f26, 0x10
        st8.nta             [r10] = loc0, 8         // PFS

        stf.spill.nta       [in0] = f27, 0x10
        st8.nta             [r10] = r15, 8          // BSP
        mov                 r8  = 0

        stf.spill.nta       [in0] = f28, 0x10
        mov                 r19 = ar.fpsr

        stf.spill.nta       [in0] = f29, 0x10
        st8.nta             [r10] = r16, 8          // PR
        mov                 ar.pfs = loc0

        stf.spill.nta       [in0] = f30, 0x10
        st8.nta             [r10] = r17, 8          // LC
        mov                 b0  = loc1

        stf.spill.nta       [in0] = f31, 0x10
        st8.nta             [r10] = r19             // FPSR

        mov                 ar.unat = r14
        br.ret.sptk         b0
.endp   SetJump
