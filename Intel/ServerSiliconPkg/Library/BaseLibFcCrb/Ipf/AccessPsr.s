## @file
#
# @copyright
# INTEL CONFIDENTIAL
# Copyright 2006 - 2010 Intel Corporation. <BR>
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

#define CpuModeMask           0x0000001008020000

#define CpuInVirtualMode             0x1
#define CpuInPhysicalMode            0x0
#define CpuInMixMode                 (0x0 - 0x1)

//---------------------------------------------------------------------------------
//++
// AsmReadPsr
//
// This routine is used to read the current value of Processor Status Register (PSR).
//
// Arguments :
//
// On Entry :
//
// Return Value: The current PSR value.
//
//--
//----------------------------------------------------------------------------------
.text
.type   AsmReadPsr, @function
.proc   AsmReadPsr

AsmReadPsr::
        mov             r8 = psr;;
        br.ret.dpnt     b0;;
.endp   AsmReadPsr

//---------------------------------------------------------------------------------
//++
// AsmWritePsr
//
// This routine is used to write the value of Processor Status Register (PSR).
//
// Arguments :
//
// On Entry : The value need to be written.
//
// Return Value: The value have been written.
//
//--
//----------------------------------------------------------------------------------
.text
.type   AsmWritePsr, @function
.proc   AsmWritePsr
.regstk 1, 0, 0, 0

AsmWritePsr::
        mov             psr.l = in0
        mov             r8 = in0;;
        srlz.d;;
        srlz.i;;
        br.ret.dpnt     b0;;
.endp   AsmWritePsr

//---------------------------------------------------------------------------------
//++
// AsmCpuVirtual
//
// This routine is used to determines if the CPU is currently executing
// in virtual, physical, or mixed mode.
//
// If the CPU is in virtual mode(PSR.RT=1, PSR.DT=1, PSR.IT=1), then 1 is returned.
// If the CPU is in physical mode(PSR.RT=0, PSR.DT=0, PSR.IT=0), then 0 is returned.
// If the CPU is not in physical mode or virtual mode, then it is in mixed mode,
// and -1 is returned.
//
// Arguments:
//
// On Entry: None
//
// Return Value: The CPU mode flag
//               return  1  The CPU is in virtual mode.
//               return  0  The CPU is in physical mode.
//               return -1  The CPU is in mixed mode.
//
//--
//----------------------------------------------------------------------------------
.text
.type   AsmCpuVirtual, @function
.proc   AsmCpuVirtual

AsmCpuVirtual::
        mov            r29 = psr
        movl           r30 = CpuModeMask;;
        and            r28 = r30, r29;;
        cmp.eq         p6, p7 = r30, r28;;
(p6)    mov            r8 = CpuInVirtualMode;;
(p6)    br.ret.dpnt    b0;;
(p7)    cmp.eq         p6, p7 = 0x0, r28;;
(p6)    mov            r8 = CpuInPhysicalMode;;
(p7)    mov            r8 = CpuInMixMode;;
        br.ret.dpnt    b0;;
.endp   AsmCpuVirtual
