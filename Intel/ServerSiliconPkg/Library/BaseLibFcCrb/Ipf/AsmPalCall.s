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

//
// PAL function calls
//
#define PAL_MC_CLEAR_LOG               0x0015
#define PAL_MC_DYNAMIC_STATE           0x0018
#define PAL_MC_ERROR_INFO              0x0019
#define PAL_MC_RESUME                  0x001a


.text
.proc AsmPalCall
.type AsmPalCall, @function

AsmPalCall::
         alloc          loc1 = ar.pfs,5,8,4,0
         mov            loc0 = b0
         mov            loc3 = b5
         mov            loc4 = r2
         mov            loc7 = r1
         mov            r2 = psr;;
         mov            r28 = in1
         mov            loc5 = r2;;

         movl           loc6 = 0x100;;
         cmp.ge         p6,p7 = r28,loc6;;

(p6)     movl           loc6 = 0x1FF;;
(p7)     br.dpnt.few PalCallStatic;;                  // 0 ~ 255 make a static Pal Call
(p6)     cmp.le         p6,p7 = r28,loc6;;
(p6)     br.dpnt.few PalCallStacked;;                 // 256 ~ 511 make a stacked Pal Call
(p7)     movl           loc6 = 0x300;;
(p7)     cmp.ge         p6,p7 = r28,loc6;;
(p7)     br.dpnt.few PalCallStatic;;                  // 512 ~ 767 make a static Pal Call
(p6)     movl           loc6 = 0x3FF;;
(p6)     cmp.le         p6,p7 = r28,loc6;;
(p6)     br.dpnt.few PalCallStacked;;                 // 768 ~ 1023 make a stacked Pal Call

(p7)     mov            r8 = 0xFFFFFFFFFFFFFFFF;;     // > 1024 return invalid
(p7)     br.dpnt.few    ComeBackFromPALCall;;

PalCallStatic:
         movl           loc6 = PAL_MC_CLEAR_LOG;;
         cmp.eq         p6,p7 = r28,loc6;;

(p7)     movl           loc6 = PAL_MC_DYNAMIC_STATE;;
(p7)     cmp.eq         p6,p7 = r28,loc6;;

(p7)     movl           loc6 = PAL_MC_ERROR_INFO;;
(p7)     cmp.eq         p6,p7 = r28,loc6;;

(p7)     movl           loc6 = PAL_MC_RESUME;;
(p7)     cmp.eq         p6,p7 = r28,loc6 ;;

         mov            loc6 = 0x1;;
(p7)     dep            r2 = loc6,r2,13,1;;           // psr.ic = 1

// p6 will be true, if it is one of the MCHK calls. There has been lots of debate
// on psr.ic for these values. For now, do not do any thing to psr.ic

         dep            r2 = r0,r2,14,1;;             // psr.i = 0

         mov            psr.l = r2
         srlz.d                                       // Needs data serailization.
         srlz.i                                       // Needs instruction serailization.

StaticGetPALLocalIP:
         mov            loc2 = ip;;
         add            loc2 = ComeBackFromPALCall - StaticGetPALLocalIP,loc2;;
         mov            b0 = loc2                     // return address after Pal call

         mov            r29 = in2
         mov            r30 = in3
         mov            r31 = in4
         mov            b5 = in0;;                    // get the PalProcEntrypt from input
         br.sptk        b5;;                          // Take the plunge.

PalCallStacked:
         dep            r2 = r0,r2,14,1;;             // psr.i = 0
         mov            psr.l = r2;;
         srlz.d                                       // Needs data serailization.
         srlz.i                                       // Needs instruction serailization.

StackedGetPALLocalIP:
         mov            out0 = in1
         mov            out1 = in2
         mov            out2 = in3
         mov            out3 = in4
         mov            b5 =  in0 ;;                  // get the PalProcEntrypt from input
         br.call.dpnt   b0 = b5 ;;                    // Take the plunge.

ComeBackFromPALCall:
         mov            psr.l = loc5 ;;
         srlz.d                                       // Needs data serailization.
         srlz.i                                       // Needs instruction serailization.

         mov            b5 = loc3
         mov            r2 = loc4
         mov            r1 = loc7

         mov            b0 = loc0
         mov            ar.pfs = loc1;;
         br.ret.dpnt    b0;;

.endp AsmPalCall

