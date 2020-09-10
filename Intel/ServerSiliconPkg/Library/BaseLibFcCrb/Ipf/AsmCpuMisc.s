## @file
#
# @copyright
# INTEL CONFIDENTIAL
# Copyright 2017 - 2008 Intel Corporation. <BR>
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
.proc CpuBreakpoint
.type CpuBreakpoint, @function

CpuBreakpoint::
        break.i 0;;
        br.ret.dpnt    b0;;

.endp CpuBreakpoint

.proc MemoryFence
.type MemoryFence, @function

MemoryFence::
        mf;;    // memory access ordering

        // do we need the mf.a also here?
        mf.a    // wait for any IO to complete?
        
        // not sure if we need serialization here, just put it, in case...
        
        srlz.d;;
        srlz.i;;
        
        br.ret.dpnt    b0;;
.endp MemoryFence

.proc DisableInterrupts
.type DisableInterrupts, @function

DisableInterrupts::
         rsm      0x4000
         srlz.d;;
         br.ret.dpnt    b0;;

.endp DisableInterrupts

.proc EnableInterrupts
.type EnableInterrupts, @function

EnableInterrupts::
      ssm     0x4000
      srlz.d;;
      br.ret.dpnt    b0;;

.endp EnableInterrupts

.proc EnableDisableInterrupts
.type EnableDisableInterrupts, @function

EnableDisableInterrupts::
         ssm      0x4000
         srlz.d;;
         srlz.i;;
         rsm      0x4000
         srlz.d;;

         br.ret.dpnt    b0;;

.endp EnableDisableInterrupts

