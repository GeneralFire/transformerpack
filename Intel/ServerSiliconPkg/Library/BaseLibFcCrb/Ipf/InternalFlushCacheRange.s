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

.file  "IpfCpuCache.s"

#include <IpfMacro.i>

//
//  Internal worker function to invalidate a range of instruction cache lines
//  in the cache coherency domain of the calling CPU.
//
//  Internal worker function to invalidate the instruction cache lines specified
//  by Address and Length. If Address is not aligned on a cache line boundary,
//  then entire instruction cache line containing Address is invalidated. If
//  Address + Length is not aligned on a cache line boundary, then the entire
//  instruction cache line containing Address + Length -1 is invalidated. This
//  function may choose to invalidate the entire instruction cache if that is more
//  efficient than invalidating the specified range. If Length is 0, the no instruction
//  cache lines are invalidated. Address is returned.
//  This function is only available on IPF.
//
//  @param  Address The base address of the instruction cache lines to
//                  invalidate. If the CPU is in a physical addressing mode, then
//                  Address is a physical address. If the CPU is in a virtual
//                  addressing mode, then Address is a virtual address.
//
//  @param  Length  The number of bytes to invalidate from the instruction cache.
//
//  @return Address
//  
//  VOID *
//  EFIAPI
//  InternalFlushCacheRange (
//    IN      VOID                      *Address,
//    IN      UINTN                     Length
//    );
//
PROCEDURE_ENTRY (InternalFlushCacheRange)

      NESTED_SETUP (5,8,0,0)
            
      mov         loc2 = ar.lc
      
      mov         loc3 = in0                  // Start address.
      mov         loc4 = in1;;                // Length in bytes.
      
      cmp.eq  p6,p7 = loc4, r0;;               // If Length is zero then don't flush any cache
      (p6)  br.spnt.many DoneFlushingC;;         
      
      add         loc4 = loc4,loc3 
      mov         loc5 = 1;;
      sub         loc4 = loc4, loc5 ;; // the End address to flush
                                         
      dep         loc3 = r0,loc3,0,5          
      dep         loc4 = r0,loc4,0,5;;         
      shr         loc3 = loc3,5             
      shr         loc4 = loc4,5;;    // 32 byte cache line
      
      sub         loc4 = loc4,loc3;; // total flush count, It should be add 1 but 
                                     // the br.cloop will first execute one time 
      mov         loc3 = in0                  
      mov         loc5 = 32      
      mov         ar.lc = loc4;;

StillFlushingC:
      fc          loc3;; 
      sync.i;;
      srlz.i;;
      add         loc3 = loc5,loc3;;
      br.cloop.sptk.few StillFlushingC;;

DoneFlushingC:      
      mov         ar.lc = loc2     
      mov          r8   = in0       // return *Address
      NESTED_RETURN

PROCEDURE_EXIT (InternalFlushCacheRange)

