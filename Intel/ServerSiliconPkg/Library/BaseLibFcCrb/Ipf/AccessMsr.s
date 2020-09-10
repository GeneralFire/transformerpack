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

//---------------------------------------------------------------------------------
//++
// AsmReadMsr
//
// Reads the current value of a Machine Specific Register (MSR).
//
// Reads and returns the current value of the Machine Specific Register specified by Index.  No
// parameter checking is performed on Index, and if the Index value is beyond the implemented MSR
// register range, a Reserved Register/Field fault may occur.  The caller must either guarantee that
// Index is valid, or the caller must set up fault handlers to catch the faults.  This function is
// only available on IPF.
//
// Arguments :
//
// On Entry : The 8-bit Machine Specific Register index to read.
//
// Return Value: The current value of the Machine Specific Register specified by Index.
//
//--
//----------------------------------------------------------------------------------
.text
.type   AsmReadMsr, @function
.proc   AsmReadMsr
.regstk 1, 0, 0, 0

AsmReadMsr::
  mov    r8=msr[in0];;
  br.ret.sptk  b0;;
.endp   AsmReadMsr

//---------------------------------------------------------------------------------
//++
// AsmWriteMsr
//
// Writes the current value of a Machine Specific Register (MSR).
//
// Writes Value to the Machine Specific Register specified by Index.  Value is returned.  No
// parameter checking is performed on Index, and if the Index value is beyond the implemented MSR
// register range, a Reserved Register/Field fault may occur.  The caller must either guarantee that
// Index is valid, or the caller must set up fault handlers to catch the faults.  This function is
// only available on IPF.
//
// Arguments :
//
// On Entry : The 8-bit Machine Specific Register index to write.
//            The 64-bit value to write to the Machine Specific Register.
//
// Return Value: The 64-bit value to write to the Machine Specific Register.
//
//--
//----------------------------------------------------------------------------------
.text
.type   AsmWriteMsr, @function
.proc   AsmWriteMsr
.regstk 2, 0, 0, 0

AsmWriteMsr::
  mov             msr[in0] = in1
  mov             r8 = in1;;
  srlz.d;;
  br.ret.sptk     b0;;
.endp   AsmWriteMsr

