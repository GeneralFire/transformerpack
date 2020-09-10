## @file
#
# @copyright
# INTEL CONFIDENTIAL
# Copyright 2006 - 2012 Intel Corporation. <BR>
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

.proc   AsmSwitchStackAndBackingStore
.type   AsmSwitchStackAndBackingStore, @function
.regstk 5, 0, 0, 0
AsmSwitchStackAndBackingStore::
        mov                 r14 = ar.rsc
        movl                r2  = ~((((1 << 14) - 1) << 16) | 3)

        mov                 r17 = in1
        mov                 r18 = in2
        and                 r2  = r14, r2
        
        flushrs
        
        mov                 ar.rsc = r2
        mov                 sp  = in3
        mov                 r19 = in4

        ld8.nt1             r16 = [in0], 8
        ld8.nta             gp  = [in0]
        mov                 r3  = -1

        loadrs
        mov                 ar.bspstore = r19
        mov                 b7  = r16

        alloc               r2  = ar.pfs, 0, 0, 2, 0
        mov                 out0 = r17
        mov                 out1 = r18

        mov                 ar.rnat = r3
        mov                 ar.rsc = r14
        br.call.sptk.many   b0  = b7
.endp   AsmSwitchStackAndBackingStore
