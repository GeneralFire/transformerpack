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

//---------------------------------------------------------------------------------
//++
// AsmFc
//
// This routine is used to execute a FC instruction on the specific address.
//
// Arguments :
//
// On Entry :  The specific address need to execute FC instruction.
//
// Return Value: The specific address have been execute FC instruction.
//
//--
//----------------------------------------------------------------------------------
.text
.type   AsmFc, @function
.proc   AsmFc
.regstk 1, 0, 0, 0

AsmFc::
        fc              in0
        mov             r8 = in0;;
        br.ret.dpnt     b0;;
.endp   AsmFc


//---------------------------------------------------------------------------------
//++
// AsmFci
//
// This routine is used to execute a FC.i instruction on the specific address.
//
// Arguments :
//
// On Entry :  The specific address need to execute FC.i instruction.
//
// Return Value: The specific address have been execute FC.i instruction.
//
//--
//----------------------------------------------------------------------------------
.text
.type   AsmFci, @function
.proc   AsmFci
.regstk 1, 0, 0, 0

AsmFci::
        fc.i            in0
        mov             r8 = in0;;
        br.ret.dpnt     b0;;
.endp   AsmFci
