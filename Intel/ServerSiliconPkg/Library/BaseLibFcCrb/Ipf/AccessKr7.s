## @file
#
# @copyright
# INTEL CONFIDENTIAL
# Copyright 2006 - 2009 Intel Corporation. <BR>
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
// AsmReadKr7
//
// This routine is used to get KR7.
//
// Arguments :
//
// On Entry :  None.
//
// Return Value: The value store in KR7.
//
//--
//----------------------------------------------------------------------------------
.text
.type   AsmReadKr7, @function
.proc   AsmReadKr7

AsmReadKr7::
        mov             r8 = ar.k7;;
        br.ret.dpnt     b0;;
.endp   AsmReadKr7

//---------------------------------------------------------------------------------
//++
// AsmWriteKr7
//
// This routine is used to write KR7.
//
// Arguments :
//
// On Entry :  None.
//
// Return Value: The value written to the KR7.
//
//--
//----------------------------------------------------------------------------------
.text
.type   AsmWriteKr7, @function
.proc   AsmWriteKr7
.regstk 1, 3, 0, 0

AsmWriteKr7::
        mov             ar.k7 = in0
        mov             r8 = in0;;
        br.ret.dpnt     b0;;
.endp   AsmWriteKr7
