/** @file
  Implementation for RcMemAllocLib that allocate pool from stolen CAR

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation.

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

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/RcMemAllocLib.h>

#define PAGE_TAG_SIGNATURE SIGNATURE_32 ('P', 'G', 'T', 'G')

typedef struct _PAGE_TAG PAGE_TAG;

struct _PAGE_TAG {
  UINT32  Signature;
  UINT32  Pages;
};

/**

  Allocates the number bytes specified by AllocationSize and returns a pointer to the allocated Bufferfer.
  If there is not enough memory remaining to satisfy the request, then NULL is returned.

  @param  AllocationSize        The number of bytes to allocate.

  @return Pointer to allocated memory

**/
VOID *
EFIAPI
RcAllocatePool (
  IN  UINTN   AllocationSize
  )
{
  PAGE_TAG    *PageTag;
  UINTN       Pages;

  if (AllocationSize > MAX_UINTN - sizeof (sizeof (PAGE_TAG))) {
    return NULL;
  }

  Pages = EFI_SIZE_TO_PAGES (AllocationSize + sizeof (PAGE_TAG));
  PageTag = AllocatePages (Pages);
  if (PageTag == NULL) {
    return NULL;
  }

  PageTag->Signature = PAGE_TAG_SIGNATURE;
  PageTag->Pages = (UINT32) Pages;

  return (PageTag + 1);
}

/**
  Frees the buffer specified by Buffer.  Buffer must have been allocated on a previous call to the
  pool allocation services of the Memory Allocation Library.  
  
  @param  Buffer                Pointer to the buffer to free.

  @retval RETURN_SUCCESS             Buffer is successfully freed.
  @retval RETURN_INVALID_PARAMETER   Buffer is not allocated with RcAllocotePool.

**/
RETURN_STATUS
EFIAPI
RcFreePool (
  IN VOID    *Buffer
  )
{
  PAGE_TAG   *PageTag;

  if (Buffer == NULL) {
    return RETURN_INVALID_PARAMETER;
  }

  PageTag = (PAGE_TAG *) Buffer - 1;
  if (PageTag->Signature != PAGE_TAG_SIGNATURE) {
    return RETURN_INVALID_PARAMETER;
  }

  FreePages (PageTag, PageTag->Pages);
  return RETURN_SUCCESS;
}

/**

  Dump the current heap usage and available space to the serial log.

  @param  None

  @return None

**/

VOID
EFIAPI
RcDumpHeapUsage (
  VOID
  )
{
  //
  // Not implemented in this instance
  //

  return;

} // RcDumpHeapUsage

/**

  Dump memory allocations to the serial log that have not been freed.

  DEBUG_MEMORY_ALLOCATION needs to be set to 1 in RcMemAllocLib
  in order for this to be useful. That will turn on output to the serial
  log with the address of each memory allocation. Then call this function
  to see which allocations have not been freed at the time this function
  is called. This function should only be called when debugging, and is not
  called normally.

  @param  None

  @return None

**/

VOID
EFIAPI
RcCheckForMemoryLeaks (
  VOID
  )
{
  //
  // Not implemented in this instance
  //

  return;

} // RcCheckForMemoryLeaks
