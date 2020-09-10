/** @file
  AsmFlushCacheRange() function for IPF.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary    and
  confidential information of Intel Corporation and its suppliers and licensors,
  and is protected by worldwide copyright and trade secret laws and treaty
  provisions. No part of the Material may be used, copied, reproduced, modified,
  published, uploaded, posted, transmitted, distributed, or disclosed in any way
  without Intel's prior express written permission.

  No license under any patent, copyright, trade secret or other intellectual
  property right is granted to or conferred upon you by disclosure or delivery
  of the Materials, either expressly, by implication, inducement, estoppel or
  otherwise. Any license under such intellectual property rights must be
  express and approved by Intel in writing.

  Unless otherwise agreed by Intel in writing, you may not remove or alter
  this notice or any other notice embedded in Materials by Intel or
  Intel's suppliers or licensors in any way.
**/

#include "BaseLibInternals.h"

/**
  Flush a range of  cache lines in the cache coherency domain of the calling
  CPU.

  Flushes the cache lines specified by Address and Length.  If Address is not aligned 
  on a cache line boundary, then entire cache line containing Address is flushed.  
  If Address + Length is not aligned on a cache line boundary, then the entire cache 
  line containing Address + Length - 1 is flushed.  This function may choose to flush 
  the entire cache if that is more efficient than flushing the specified range.  If 
  Length is 0, the no cache lines are flushed.  Address is returned.   
  This function is only available on IPF.

  If Length is greater than (MAX_ADDRESS - Address + 1), then ASSERT().

  @param  Address The base address of the instruction lines to invalidate. If
                  the CPU is in a physical addressing mode, then Address is a
                  physical address. If the CPU is in a virtual addressing mode,
                  then Address is a virtual address.

  @param  Length  The number of bytes to invalidate from the instruction cache.

  @return Address.

**/
VOID *
EFIAPI
AsmFlushCacheRange (
  IN      VOID                      *Address,
  IN      UINTN                     Length
  )
{
  ASSERT (Length <= MAX_ADDRESS - (UINTN)Address + 1);
  return InternalFlushCacheRange (Address, Length);
}
