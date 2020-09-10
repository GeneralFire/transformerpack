/**
  @copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2006 - 2018, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Base.h>

/**
  Flushes a cache line from all the instruction and data caches within the
  coherency domain of the CPU in a manner optimized for throughput.

  Flushes the cache line specified by LinearAddress.

  @param  LinearAddress The address of the cache line to flush. If the CPU is
                        in a physical addressing mode, then LinearAddress is a
                        physical address. If the CPU is in a virtual
                        addressing mode, then LinearAddress is a virtual
                        address.
**/
VOID
EFIAPI
AsmFlushCacheLineOpt (
  IN  VOID  *LinearAddress
  )
{
  __asm__ __volatile__ (
    ".byte 0x66;clflush (%0)"
    :
    : "r" (LinearAddress)
    : "memory"
    );
}
