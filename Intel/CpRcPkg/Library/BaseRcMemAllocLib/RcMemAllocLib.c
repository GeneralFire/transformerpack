/** @file
  Implementation for RcMemAllocLib that allocate pool from stolen CAR

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#include <Base.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/RcHeapAddrLib.h>
#include <Library/RcMemAllocLib.h>

//
// Set this to 1 to turn on memory allocation debug info
// in the serial log. Dumps a lot of info to the log, so
// do not turn on by default.
//

#define DEBUG_MEMORY_ALLOCATION   0

#define OCCUPIED           0x00000001
#define HEAP_END           0x80000000
#define RC_HEAP_SIGNATURE  SIGNATURE_32 ('R', 'C', 'H', 'P') 

typedef struct _HEAP_BUFFER_HEADER HEAP_BUFFER_HEADER;

struct _HEAP_BUFFER_HEADER {
  UINT8   *BufferBase;
  UINT32  BufferLimit;
  UINT32  BufferFlags;
};

typedef struct _HEAP_HEADER    HEAP_HEADER;

struct _HEAP_HEADER {
  UINT32  Signature;
  UINT32  MaxLimit;
};

/**

  Initialize pool area at the end of Host data structure

**/
VOID
RcInitHeap (
  VOID
  )
{
  UINT8                 *HeapLimitPtr;
  HEAP_BUFFER_HEADER    *HeapBase;
  HEAP_HEADER           *HeapHeader;
  UINT32                HeapSize;
   
  HeapHeader = GetHeapBaseAddr ();
  HeapSize = PcdGet32 (PcdPeiTemporaryRamRcHeapSize);
  //
  // This library instance requires preallocate heap size in SEC phase.
  //
  ASSERT (HeapSize > 0);
  HeapLimitPtr = (UINT8 *) HeapHeader + HeapSize;

  //
  // Initialize the start header
  //
  ZeroMem (HeapHeader, HeapSize);
  HeapHeader->Signature = RC_HEAP_SIGNATURE;
  HeapHeader->MaxLimit  = (2 * sizeof (HEAP_BUFFER_HEADER)) + sizeof (HEAP_HEADER);
  HeapBase              = (HEAP_BUFFER_HEADER *) (HeapHeader + 1);
  HeapBase->BufferBase  = (UINT8 *) (HeapBase + 1);
  HeapBase->BufferLimit = HeapSize - (2 * sizeof (HEAP_BUFFER_HEADER)) - sizeof (HEAP_HEADER);
  HeapBase->BufferFlags = 0;

  //
  // Initialize the end header
  //
  HeapBase              = (HEAP_BUFFER_HEADER *) (HeapLimitPtr - sizeof (HEAP_BUFFER_HEADER));
  HeapBase->BufferBase  = HeapLimitPtr;
  HeapBase->BufferLimit = 0;
  HeapBase->BufferFlags = HEAP_END;
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
#if defined (DEBUG_MEMORY_ALLOCATION) && (DEBUG_MEMORY_ALLOCATION == 1)

  HEAP_BUFFER_HEADER    *HeaderPtr;
  HEAP_HEADER           *HeapHeader;
  UINT32                AllocatedSize;
  UINT32                TotalHeapSize;
  UINT32                LargestEverAllocatedSize;

  HeapHeader = GetHeapBaseAddr ();

  LargestEverAllocatedSize = HeapHeader->MaxLimit;

  //
  // Set HeaderPtr to the beginning
  //

  HeaderPtr = (HEAP_BUFFER_HEADER *) (HeapHeader + 1);

  //
  // Get the total heap size
  //

  TotalHeapSize = PcdGet32 (PcdPeiTemporaryRamRcHeapSize);

  //
  // Loop through the allocated buffers and increment the allocated size
  //

  AllocatedSize = 0;
  while ((HeaderPtr->BufferFlags & HEAP_END) == 0) {
    if ((HeaderPtr->BufferFlags & OCCUPIED) != 0) {
      AllocatedSize += HeaderPtr->BufferLimit;
    }

    //
    // Increment to the next buffer
    //
    HeaderPtr = (HEAP_BUFFER_HEADER *) (HeaderPtr->BufferBase + HeaderPtr->BufferLimit);

  }

  DEBUG ((EFI_D_INFO, "RcMemAllocLib: Current total allocated bytes:            %d\n", AllocatedSize));
  DEBUG ((EFI_D_INFO, "RcMemAllocLib: Remaining bytes available for allocation: %d\n", TotalHeapSize - AllocatedSize));
  DEBUG ((EFI_D_INFO, "RcMemAllocLib: Most heap ever allocated:                 %d\n", LargestEverAllocatedSize));

#endif // #if defined (DEBUG_MEMORY_ALLOCATION) && (DEBUG_MEMORY_ALLOCATION == 1)

  return;

} // RcDumpHeapUsage

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
  HEAP_BUFFER_HEADER    *HeaderPtr;
  HEAP_BUFFER_HEADER    *NextHeaderPtr;
  HEAP_HEADER           *HeapHeader;
  UINT32                MaxLimit;

  //
  // Round size up to a QWORD integral
  //
  AllocationSize = ALIGN_VALUE (AllocationSize, 8);

  //
  // Initialize the root header
  //
  HeapHeader = GetHeapBaseAddr ();
  if (HeapHeader->Signature != RC_HEAP_SIGNATURE) {
    RcInitHeap ();
  }

  HeaderPtr = (HEAP_BUFFER_HEADER *) (HeapHeader + 1);

  //
  // Walk the heap looking for an available Bufferfer
  //
  while (((HeaderPtr->BufferFlags & OCCUPIED) || (HeaderPtr->BufferLimit < AllocationSize)) && !(HeaderPtr->BufferFlags & HEAP_END)) {
    HeaderPtr = (HEAP_BUFFER_HEADER *) (HeaderPtr->BufferBase + HeaderPtr->BufferLimit);
  }
  //
  // Check for the end of heap space
  //
  if (HeaderPtr->BufferFlags & HEAP_END) {
    DEBUG ((EFI_D_ERROR, "RcAllocatePool: Not enough available heap for requested allocation\n"));
    ASSERT (FALSE);
    return NULL;
  }
  //
  // Lock memory for the Buffer
  //
  HeaderPtr->BufferFlags |= OCCUPIED;

  //
  // Initialize the current size and next header if required
  //
  if ((HeaderPtr->BufferLimit - AllocationSize) > sizeof (HEAP_BUFFER_HEADER)) {
    NextHeaderPtr              = (HEAP_BUFFER_HEADER *) (HeaderPtr->BufferBase + AllocationSize);
    NextHeaderPtr->BufferBase  = (UINT8 *) NextHeaderPtr + sizeof (HEAP_BUFFER_HEADER);
    NextHeaderPtr->BufferLimit = (UINT32)(HeaderPtr->BufferLimit - AllocationSize - sizeof (HEAP_BUFFER_HEADER));
    NextHeaderPtr->BufferFlags = 0;
    HeaderPtr->BufferLimit     = (UINT32) AllocationSize;
  }
  MaxLimit = (UINT32)((UINT8 *) HeaderPtr - (UINT8 *) HeapHeader + HeaderPtr->BufferLimit) + sizeof (HEAP_BUFFER_HEADER);
  if (MaxLimit > HeapHeader->MaxLimit) {
    HeapHeader->MaxLimit = MaxLimit;
  }

#if defined (DEBUG_MEMORY_ALLOCATION) && (DEBUG_MEMORY_ALLOCATION == 1)

  DEBUG ((EFI_D_INFO, "RcMemAllocLib: Allocating Memory at Address = 0x%x, Bytes Allocated = %d\n", HeaderPtr->BufferBase, AllocationSize));

  RcDumpHeapUsage ();

#endif // #if defined (DEBUG_MEMORY_ALLOCATION) && (DEBUG_MEMORY_ALLOCATION == 1)

  //
  // Return the current base
  //
  return HeaderPtr->BufferBase;
}

/**
  Frees the buffer specified by Buffer.  Buffer must have been allocated on a previous call to the
  pool allocation services of the Memory Allocation Library.  
  
  @param  Buffer                Pointer to the buffer to free.

  @retval RETURN_SUCCESS             Buffer is successfully freed.
  @retval RETURN_INVALID_PARAMETER   Buffer is Buffer was not allocated with RcAllocotePool.

**/
RETURN_STATUS
EFIAPI
RcFreePool (
  IN VOID    *Buffer
  )
{
  HEAP_BUFFER_HEADER  *HeaderPtr;
  HEAP_BUFFER_HEADER  *TempPtr;
  HEAP_HEADER         *HeapHeader;

  DEBUG((DEBUG_POOL, "RcFreePool Start: Buffer pointer:            %08x\n", Buffer));

  if (Buffer == NULL) {
    DEBUG ((EFI_D_ERROR, "RcFreePool: Cannot free NULL pointer\n"));
    ASSERT (FALSE);
    return RETURN_INVALID_PARAMETER;
  }
  //
  // Initialize a pointer to the given Bufferfer header
  //
  HeaderPtr = (HEAP_BUFFER_HEADER *) ((UINT8 *) Buffer - sizeof (HEAP_BUFFER_HEADER));

  //
  // Validate the given pointer before proceeding
  //
  if (HeaderPtr->BufferBase != Buffer) {
    DEBUG ((EFI_D_ERROR, "RcFreePool: Address is not memory allocated via RcAllocatePool\n"));
    ASSERT (FALSE);
    return RETURN_INVALID_PARAMETER;
  }

#if defined (DEBUG_MEMORY_ALLOCATION) && (DEBUG_MEMORY_ALLOCATION == 1)

  DEBUG ((EFI_D_INFO, "RcMemAllocLib: Freeing Memory at Address = 0x%x, Bytes Allocated = %d\n", HeaderPtr->BufferBase, HeaderPtr->BufferLimit));

#endif // #if defined (DEBUG_MEMORY_ALLOCATION) && (DEBUG_MEMORY_ALLOCATION == 1)

  //
  // Free the given Bufferfer
  //
  HeaderPtr->BufferFlags &= ~OCCUPIED;

  //
  // Initialize the root header
  //
  HeapHeader = GetHeapBaseAddr ();
  HeaderPtr = (HEAP_BUFFER_HEADER *) (HeapHeader + 1);

  //
  // Walk the heap looking for holes to merge
  //
  do {
    //
    // Find the next hole
    //
    while (HeaderPtr->BufferFlags & OCCUPIED) {
      HeaderPtr = (HEAP_BUFFER_HEADER *) (HeaderPtr->BufferBase + HeaderPtr->BufferLimit);

    }
    //
    // Check for the end of heap space
    //
    if (HeaderPtr->BufferFlags & HEAP_END) {
      break;

    }
    //
    // Look for adjacent holes to merge
    //
    TempPtr = (HEAP_BUFFER_HEADER *) (HeaderPtr->BufferBase + HeaderPtr->BufferLimit);
    while (!(TempPtr->BufferFlags & (OCCUPIED | HEAP_END))) {
      //
      // Add this Bufferfer to the current limit and move to the next Bufferfer
      //
      HeaderPtr->BufferLimit += TempPtr->BufferLimit + sizeof (HEAP_BUFFER_HEADER);
      TempPtr = (HEAP_BUFFER_HEADER *) (TempPtr->BufferBase + TempPtr->BufferLimit);
    }
    //
    // Move to the next Bufferfer
    //
    HeaderPtr = (HEAP_BUFFER_HEADER *) (HeaderPtr->BufferBase + HeaderPtr->BufferLimit);

  } while (!(HeaderPtr->BufferFlags & HEAP_END));

  DEBUG((DEBUG_POOL, "RcFreePool End\n"));

  return RETURN_SUCCESS;
}

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

#if defined (DEBUG_MEMORY_ALLOCATION) && (DEBUG_MEMORY_ALLOCATION == 1)

  HEAP_BUFFER_HEADER    *HeaderPtr;
  HEAP_HEADER           *HeapHeader;

  HeapHeader = GetHeapBaseAddr ();

  DEBUG ((EFI_D_INFO, "Checking for memory allocated with RcAllocPool that has not been freed\n"));

  //
  // Set HeaderPtr to the beginning
  //

  HeaderPtr = (HEAP_BUFFER_HEADER *) (HeapHeader + 1);

  while ((HeaderPtr->BufferFlags & HEAP_END) == 0) {

    //
    // Check for memory that is still occupied
    //

    if ((HeaderPtr->BufferFlags & OCCUPIED) != 0) {
      DEBUG ((EFI_D_INFO, "RcMemAllocLib: Memory at Address = 0x%x was never freed, Bytes Allocated = %d\n", HeaderPtr->BufferBase, HeaderPtr->BufferLimit));

    }

    //
    // Increment to the next buffer
    //
    HeaderPtr = (HEAP_BUFFER_HEADER *) (HeaderPtr->BufferBase + HeaderPtr->BufferLimit);

  }

  RcDumpHeapUsage ();

#endif // #if defined (DEBUG_MEMORY_ALLOCATION) && (DEBUG_MEMORY_ALLOCATION == 1)

  return;

} // RcCheckForMemoryLeaks

