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
// AsmReadDbr
//
// This routine is used to Reads the current value of Data Breakpoint Register (DBR).
//
// Arguments :
//
// On Entry : The 8-bit DBR index to read.
//
// Return Value: The current value of DBR by Index.
//
//--
//----------------------------------------------------------------------------------
.text
.type   AsmReadDbr, @function
.proc   AsmReadDbr
.regstk 1, 0, 0, 0

AsmReadDbr::
        mov             r8 = dbr[in0];;
        br.ret.dpnt     b0;;
.endp   AsmReadDbr

//---------------------------------------------------------------------------------
//++
// AsmWriteDbr
//
// This routine is used to write the current value to Data Breakpoint Register (DBR).
//
// Arguments :
//
// On Entry : The 8-bit DBR index to read.
//            The value should be written to DBR
//
// Return Value: The value written to DBR.
//
//--
//----------------------------------------------------------------------------------
.text
.type   AsmWriteDbr, @function
.proc   AsmWriteDbr
.regstk 2, 0, 0, 0

AsmWriteDbr::
        mov             dbr[in0] = in1
        mov             r8 = in1;;
        srlz.d;;
        br.ret.dpnt     b0;;
.endp   AsmWriteDbr


//---------------------------------------------------------------------------------
//++
// AsmReadIbr
//
// This routine is used to Reads the current value of Instruction Breakpoint Register (IBR).
//
// Arguments :
//
// On Entry : The 8-bit IBR index.
//
// Return Value: The current value of IBR by Index.
//
//--
//----------------------------------------------------------------------------------
.text
.type   AsmReadIbr, @function
.proc   AsmReadIbr
.regstk 1, 0, 0, 0

AsmReadIbr::
        mov             r8 = ibr[in0];;
        br.ret.dpnt     b0;;
.endp   AsmReadIbr

//---------------------------------------------------------------------------------
//++
// AsmWriteIbr
//
// This routine is used to write the current value to Instruction Breakpoint Register (IBR).
//
// Arguments :
//
// On Entry : The 8-bit IBR index.
//            The value should be written to IBR
//
// Return Value: The value written to IBR.
//
//--
//----------------------------------------------------------------------------------
.text
.type   AsmWriteIbr, @function
.proc   AsmWriteIbr
.regstk 2, 0, 0, 0

AsmWriteIbr::
        mov             ibr[in0] = in1
        mov             r8 = in1;;
        srlz.i;;
        br.ret.dpnt     b0;;
.endp   AsmWriteIbr
