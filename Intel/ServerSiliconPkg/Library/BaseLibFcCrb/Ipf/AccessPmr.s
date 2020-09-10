## @file
#
# @copyright
# INTEL CONFIDENTIAL
# Copyright 2017 - 2006 Intel Corporation. <BR>
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

//---------------------------------------------------------------------------------
//++
// AsmReadPmc
//
// This routine is used to Reads the current value of Performance Monitor Configuration Register (PMC).
//
// Arguments :
//
// On Entry : The 8-bit PMC index.
//
// Return Value: The current value of PMC by Index.
//
//--
//----------------------------------------------------------------------------------
.text
.type   AsmReadPmc, @function
.proc   AsmReadPmc
.regstk 1, 0, 0, 0

AsmReadPmc::
        srlz.i;;
        srlz.d;;
        mov             r8 = pmc[in0];;
        br.ret.dpnt     b0;;
.endp   AsmReadPmc

//---------------------------------------------------------------------------------
//++
// AsmWritePmc
//
// This routine is used to write the current value to a Performance Monitor Configuration Register (PMC).
//
// Arguments :
//
// On Entry : The 8-bit PMC index.
//            The value should be written to PMC
//
// Return Value: The value written to PMC.
//
//--
//----------------------------------------------------------------------------------
.text
.type   AsmWritePmc, @function
.proc   AsmWritePmc
.regstk 2, 0, 0, 0

AsmWritePmc::
        mov             pmc[in0] = in1
        mov             r8 = in1;;
        srlz.i;;
        srlz.d;;
        br.ret.dpnt     b0;;
.endp   AsmWritePmc


//---------------------------------------------------------------------------------
//++
// AsmReadPmd
//
// This routine is used to Reads the current value of Performance Monitor Data Register (PMD).
//
// Arguments :
//
// On Entry : The 8-bit PMD index.
//
// Return Value: The current value of PMD by Index.
//
//--
//----------------------------------------------------------------------------------
.text
.type   AsmReadPmd, @function
.proc   AsmReadPmd
.regstk 1, 0, 0, 0

AsmReadPmd::
        srlz.i;;
        srlz.d;;
        mov             r8 = pmd[in0];;
        br.ret.dpnt     b0;;
.endp   AsmReadPmd

//---------------------------------------------------------------------------------
//++
// AsmWritePmd
//
// This routine is used to write the current value to Performance Monitor Data Register (PMD).
//
// Arguments :
//
// On Entry : The 8-bit PMD index.
//            The value should be written to PMD
//
// Return Value: The value written to PMD.
//
//--
//----------------------------------------------------------------------------------
.text
.type   AsmWritePmd, @function
.proc   AsmWritePmd
.regstk 2, 0, 0, 0

AsmWritePmd::
        mov             pmd[in0] = in1
        mov             r8 = in1;;
        srlz.i;;
        srlz.d;;
        br.ret.dpnt     b0;;
.endp   AsmWritePmd
