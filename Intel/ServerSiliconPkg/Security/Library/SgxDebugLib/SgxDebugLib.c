/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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
#include <Library/SgxDebugLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/MemoryAllocationLib.h>

static SGX_DBG_QUEUE   SgxDebugQueue;
static SPIN_LOCK       QueueDebugPrintLock;

/**
  Returns a pointer to SgxDebugQueue

  @retval SGX_DBG_QUEUE * A pointer to SgxDebugQueue used by all functions
**/
SGX_DBG_QUEUE *
GetSgxDebugQueuePtr (
  VOID
  )
{
  return &SgxDebugQueue;
}

/**
  Returns a pointer to QueueDebugPrintLock

  @retval SPIN_LOCK * A pointer to QueueDebugPrintLock used by all functions
**/
SPIN_LOCK *
GetQueueDebugPrintLockPtr (
  VOID
  )
{
  return &QueueDebugPrintLock;
}

/**
  Prints a DbgMessageID with given error level. The message is preceded
  by prefix defined in SGX_DBG_MESSAGE_CODE_FORMAT and followed by newline
  symbol.

  If _SGX_VERBOSE_DEBUG_ is defined, then debug message is followed by
  verbal message assigned to specific SGX_DBG_MESSAGE_ID value.
  Otherwise print just the SGX_DBG_MESSAGE_ID.

  If Format is not NULL, then the generic message is followed by Format
  and the associated variable argument list

  If any of the bits in ErrorLevel isn't also set in PcdDebugPrintErrorLevel token,
  then don't print the message and return

  @param  ErrorLevel    The error level of the debug message
  @param  DbgMessageID  Generic sgx debug message ID.
  @param  Format        Format string for the debug message to print.
  @param  ...           Variable argument list whose contents are accessed
                        based on the format string specified by Format.
**/
VOID
SgxDebugPrint (
  IN  UINTN               ErrorLevel,
  IN  SGX_DBG_MESSAGE_ID  DbgMessageID,
  IN  CONST CHAR8         *Format OPTIONAL,
  ...
  )
{
  VA_LIST   Marker;
  CHAR8     Buffer[SGX_DBG_MESSAGE_MAX_LEN];
  UINTN     CharsProduced;

  //
  // Check driver debug mask value and global mask
  //
  if ((ErrorLevel & PcdGet32 (PcdDebugPrintErrorLevel)) == 0) {
    return;
  }

#ifdef _SGX_VERBOSE_DEBUG_
  if (MessageMap[DbgMessageID] != NULL) {
    //
    // print message mapped to certain SGX_DBG_MESSAGE_ID (_SGX_VERBOSE_DEBUG_ enabled)
    //
    CharsProduced = AsciiSPrint (
      Buffer,
      sizeof (Buffer),
      SGX_DBG_MESSAGE_CODE_FORMAT_VERBOSE,
      DbgMessageID,
      MessageMap[DbgMessageID]
      );
  } else {
    //
    // print raw SGX_DBG_MESSAGE_ID
    //
    CharsProduced = AsciiSPrint (
      Buffer,
      sizeof (Buffer),
      SGX_DBG_MESSAGE_CODE_FORMAT,
      DbgMessageID
      );
  }

#else
  //
  // print raw SGX_DBG_MESSAGE_ID (_SGX_VERBOSE_DEBUG_ disabled)
  //
  CharsProduced = AsciiSPrint (
    Buffer,
    sizeof (Buffer),
    SGX_DBG_MESSAGE_CODE_FORMAT,
    DbgMessageID
    );
#endif

  if (Format != NULL) {
    //
    // add space after generic message
    //
    CharsProduced += AsciiSPrint (Buffer + CharsProduced, sizeof (Buffer) - CharsProduced, " ");

    VA_START (Marker, Format);
    CharsProduced += AsciiVSPrint (Buffer + CharsProduced, sizeof (Buffer) - CharsProduced, Format, Marker);
    VA_END (Marker);
  }

  DebugPrint (ErrorLevel, SGX_DBG_MESSAGE_FORMAT, Buffer);
}

/**
  Prints a debug message with given error level. The message is preceded
  by prefix defined in SGX_DBG_MESSAGE_CODE_FORMAT and followed by newline
  symbol. The only difference between SgxDebugPrintS and SgxDebugPrint
  is that SgxDebugPrintS isn't affected by _SGX_VERBOSE_DEBUG_ macro,
  so the debug will always be sparse.

  If Format is not NULL, then the generic message is followed by Format
  and the associated variable argument list

  If any of the bits in ErrorLevel isn't also set in PcdDebugPrintErrorLevel token,
  then don't print the message and return

  @param  ErrorLevel    The error level of the debug message
  @param  DbgMessageID  Generic sgx debug message ID.
  @param  Format        Format string for the debug message to print.
  @param  ...           Variable argument list whose contents are accessed
                        based on the format string specified by Format.
**/
VOID
SgxDebugPrintS (
  IN  UINTN               ErrorLevel,
  IN  SGX_DBG_MESSAGE_ID  DbgMessageID,
  IN  CONST CHAR8         *Format OPTIONAL,
  ...
  )
{
  VA_LIST   Marker;
  CHAR8     Buffer[SGX_DBG_MESSAGE_MAX_LEN];
  UINTN     CharsProduced;

  //
  // Check driver debug mask value and global mask
  //
  if ((ErrorLevel & PcdGet32 (PcdDebugPrintErrorLevel)) == 0) {
    return;
  }

  //
  // print raw SGX_DBG_MESSAGE_ID
  //
  CharsProduced = AsciiSPrint (
    Buffer,
    sizeof (Buffer),
    SGX_DBG_MESSAGE_CODE_FORMAT,
    DbgMessageID
    );

  if (Format != NULL) {
    //
    // add space after generic message
    //
    CharsProduced += AsciiSPrint (Buffer + CharsProduced, sizeof (Buffer) - CharsProduced, " ");

    VA_START (Marker, Format);
    CharsProduced += AsciiVSPrint (Buffer + CharsProduced, sizeof (Buffer) - CharsProduced, Format, Marker);
    VA_END (Marker);
  }

  DebugPrint (ErrorLevel, SGX_DBG_MESSAGE_FORMAT, Buffer);
}

/**
  Queues a debug message with given error level. The message is preceded
  by prefix defined in SGX_DBG_MESSAGE_CODE_FORMAT and followed by newline
  symbol. This function is Mp safe, which makes it convenient for MpServices
  debugging.

  If _SGX_VERBOSE_DEBUG_ is defined, then debug message is followed by
  verbal message assigned to specific SGX_DBG_MESSAGE_ID value.
  Otherwise print just the SGX_DBG_MESSAGE_ID.

  If Format is not NULL, then the generic message is followed by Format
  and the associated variable argument list

  If SgxDebugQueue has not been initialized (buffer == NULL) then ASSERT

  If any of the bits in ErrorLevel isn't also set in PcdDebugPrintErrorLevel token,
  then don't queue the message and return

  @param  ErrorLevel    The error level of the debug message
  @param  DbgMessageID  Generic sgx debug message ID.
  @param  Format        Format string for the debug message to print.
  @param  ...           Variable argument list whose contents are accessed
                        based on the format string specified by Format.

  @retval EFI_ABORTED           Specified error level is suppressed
  @retval EFI_NOT_READY         SgxDebugQueue has not been initialized and ASSERT macro is disabled
  @retval EFI_BUFFER_TOO_SMALL  SgxDebugQueue is full
  @retval EFI_SUCCESS           Debug Message queued successfully
**/
EFI_STATUS
SgxQueueDebugM (
  IN  UINTN               ErrorLevel,
  IN  SGX_DBG_MESSAGE_ID  DbgMessageID,
  IN  CONST CHAR8         *Format OPTIONAL,
  ...
  )
{
  VA_LIST     Marker;
  UINTN       CharsProduced;
  EFI_STATUS  Status;
  UINTN       BufferSize;

  //
  // Check driver debug mask value and global mask
  //
  if ((ErrorLevel & PcdGet32 (PcdDebugPrintErrorLevel)) == 0) {
    return EFI_ABORTED;
  }

  ASSERT (SgxDebugQueue.Buffer != NULL);
  if (SgxDebugQueue.Buffer == NULL) {
    return EFI_NOT_READY;
  }

  while (!AcquireSpinLockOrFail (&QueueDebugPrintLock)) {
      CpuPause ();
  }

  //
  // Attempt to allocate space for next message in queue buffer
  //
  Status = SgxDebugQueueExtend (&BufferSize);
  if (EFI_ERROR (Status)) {
    ReleaseSpinLock (&QueueDebugPrintLock);
    return Status;
  }

  //
  // Allocate one byte for msg_len
  //
  SgxDebugQueue.Tail++;
  BufferSize -= 1;
#ifdef _SGX_VERBOSE_DEBUG_
  if (MessageMap[DbgMessageID] != NULL) {
    //
    // print message mapped to certain SGX_DBG_MESSAGE_ID (_SGX_VERBOSE_DEBUG_ enabled)
    //
    CharsProduced = AsciiSPrint (
      SgxDebugQueue.Tail,
      BufferSize,
      SGX_DBG_MESSAGE_CODE_FORMAT_VERBOSE,
      DbgMessageID,
      MessageMap[DbgMessageID]
      );
  } else {
    //
    // print raw SGX_DBG_MESSAGE_ID
    //
    CharsProduced = AsciiSPrint (
      SgxDebugQueue.Tail,
      BufferSize,
      SGX_DBG_MESSAGE_CODE_FORMAT,
      DbgMessageID
      );
  }

#else
  //
  // print raw SGX_DBG_MESSAGE_ID (_SGX_VERBOSE_DEBUG_ disabled)
  //
  CharsProduced = AsciiSPrint (
    SgxDebugQueue.Tail,
    BufferSize,
    SGX_DBG_MESSAGE_CODE_FORMAT,
    DbgMessageID);
#endif

  if (Format != NULL) {
    //
    // add space after generic message
    //
    CharsProduced += AsciiSPrint (SgxDebugQueue.Tail + CharsProduced, BufferSize - CharsProduced, " ");
    VA_START (Marker, Format);
    CharsProduced += AsciiVSPrint (SgxDebugQueue.Tail + CharsProduced, BufferSize - CharsProduced, Format, Marker);
    VA_END (Marker);
  }

  // Value returned by AsciiSPrint doesn't include null-terminator, however it has to be included in "CharsProduced"
  CharsProduced++;
  // Store msg_len in buffer
  SgxDebugQueue.Tail[-1] = (UINT8)CharsProduced;

  ReleaseSpinLock (&QueueDebugPrintLock);
  return EFI_SUCCESS;
}

/**
  Queues a debug message with given error level. The message is preceded
  by prefix defined in SGX_DBG_MESSAGE_CODE_FORMAT and followed by newline
  symbol. This function is Mp safe, which makes it convenient for MpServices
  debugging. The only difference between SgxQueueDebugMS and SgxQueueDebugM
  is that SgxQueueDebugMS isn't affected by _SGX_VERBOSE_DEBUG_ macro,
  so the debug will always be sparse.

  If Format is not NULL, then the generic message is followed by Format
  and the associated variable argument list

  If SgxDebugQueue has not been initialized (buffer == NULL) then ASSERT

  If any of the bits in ErrorLevel isn't also set in PcdDebugPrintErrorLevel token,
  then don't queue the message and return

  @param  ErrorLevel    The error level of the debug message
  @param  DbgMessageID  Generic sgx debug message ID.
  @param  Format        Format string for the debug message to print.
  @param  ...           Variable argument list whose contents are accessed
                        based on the format string specified by Format.

  @retval EFI_ABORTED           Specified error level is suppressed
  @retval EFI_NOT_READY         SgxDebugQueue has not been initialized and ASSERT macro is disabled
  @retval EFI_BUFFER_TOO_SMALL  SgxDebugQueue is full
  @retval EFI_SUCCESS           Debug Message queued successfully
**/
EFI_STATUS
SgxQueueDebugMS (
  IN  UINTN               ErrorLevel,
  IN  SGX_DBG_MESSAGE_ID  DbgMessageID,
  IN  CONST CHAR8         *Format OPTIONAL,
  ...
  )
{
  VA_LIST     Marker;
  UINTN       CharsProduced;
  EFI_STATUS  Status;
  UINTN       BufferSize;

  ASSERT (SgxDebugQueue.Buffer != NULL);
  if (SgxDebugQueue.Buffer == NULL) {
    return EFI_NOT_READY;
  }

  //
  // Check driver debug mask value and global mask
  //
  if ((ErrorLevel & PcdGet32 (PcdDebugPrintErrorLevel)) == 0) {
    return EFI_ABORTED;
  }

  while (!AcquireSpinLockOrFail (&QueueDebugPrintLock)) {
    CpuPause ();
  }

  //
  // Attempt to allocate space for next message in queue buffer
  //
  Status = SgxDebugQueueExtend (&BufferSize);
  if (EFI_ERROR (Status)) {
    ReleaseSpinLock (&QueueDebugPrintLock);
    return Status;
  }

  //
  // Allocate one byte for msg_len
  //
  SgxDebugQueue.Tail++;
  BufferSize -= 1;

  //
  // print raw SGX_DBG_MESSAGE_ID
  //
  CharsProduced = AsciiSPrint (
    SgxDebugQueue.Tail,
    BufferSize,
    SGX_DBG_MESSAGE_CODE_FORMAT,
    DbgMessageID
    );

  if (Format != NULL) {
    //
    // add space after generic message
    //
    CharsProduced += AsciiSPrint (SgxDebugQueue.Tail + CharsProduced, BufferSize - CharsProduced, " ");
    VA_START (Marker, Format);
    CharsProduced += AsciiVSPrint (SgxDebugQueue.Tail + CharsProduced, BufferSize - CharsProduced, Format, Marker);
    VA_END (Marker);
  }

  // Value returned by AsciiSPrint doesn't include null-terminator, however it has to be included in "CharsProduced"
  CharsProduced++;
  // Store msg_len in buffer
  SgxDebugQueue.Tail[-1] = (UINT8)CharsProduced;

  ReleaseSpinLock (&QueueDebugPrintLock);
  return EFI_SUCCESS;
}

/**
  Queues a debug message with given error level. The message is followed
  by the newline symbol. This function is Mp safe, which makes it convenient
  for MpServices debugging. This function works more like convetnional printing
  methods with just the format and VA_List in contrast to SgxQueueDebugM,
  which uses MessageMap and predefined prefix

  If Format is NULL, then ASSERT

  If SgxDebugQueue has not been initialized (buffer == NULL) then ASSERT

  If any of the bits in ErrorLevel isn't also set in PcdDebugPrintErrorLevel token,
  then don't queue the message and return

  @param  ErrorLevel    The error level of the debug message
  @param  Format        Format string for the debug message to print.
  @param  ...           Variable argument list whose contents are accessed
                        based on the format string specified by Format.

  @retval EFI_ABORTED           Specified error level is suppressed
  @retval EFI_NOT_READY         SgxDebugQueue has not been initialized and ASSERT macro is disabled
  @retval EFI_INVALID_PARAMETER Format is NULL and ASSERT macro is disabled
  @retval EFI_BUFFER_TOO_SMALL  SgxDebugQueue is full
  @retval EFI_SUCCESS           Debug Message queued successfully
**/
EFI_STATUS
SgxQueueDebug (
  IN  UINTN               ErrorLevel,
  IN  CONST CHAR8         *Format,
  ...
  )
{
  VA_LIST     Marker;
  UINTN       CharsProduced;
  EFI_STATUS  Status;
  UINTN       BufferSize;

  //
  // Check driver debug mask value and global mask
  //
  if ((ErrorLevel & PcdGet32 (PcdDebugPrintErrorLevel)) == 0) {
    return EFI_ABORTED;
  }

  ASSERT (SgxDebugQueue.Buffer != NULL);
  if (SgxDebugQueue.Buffer == NULL) {
    return EFI_NOT_READY;
  }
  ASSERT (Format != NULL);
  if (Format == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  while (!AcquireSpinLockOrFail (&QueueDebugPrintLock)) {
      CpuPause ();
  }

  //
  // Attempt to allocate space for next message in queue buffer
  //
  Status = SgxDebugQueueExtend (&BufferSize);
  if (EFI_ERROR (Status)) {
    ReleaseSpinLock (&QueueDebugPrintLock);
    return Status;
  }

  //
  // Allocate one byte for msg_len
  //
  SgxDebugQueue.Tail++;
  BufferSize -= 1;

  //
  // use format string to print debug message into the buffer
  //
  VA_START (Marker, Format);
  CharsProduced = AsciiVSPrint (SgxDebugQueue.Tail, BufferSize, Format, Marker);
  VA_END (Marker);


  // Value returned by AsciiSPrint doesn't include null-terminator, however it has to be included in "CharsProduced"
  CharsProduced++;
  // Store msg_len in buffer
  SgxDebugQueue.Tail[-1] = (UINT8)CharsProduced;

  ReleaseSpinLock (&QueueDebugPrintLock);
  return EFI_SUCCESS;
}

/**
  Initializes SgxDebugQueue by allocating memory for SgxDebugQueue
  buffer and initializing dedicated control variables

  @retval EFI_NOT_FOUND   MemoryPages allocation failed
  @retval EFI_SUCCESS     Queue initialized succesfully
**/
EFI_STATUS
SgxDebugQueueAllocInit (
  VOID
  )
{
  SgxDebugQueue.Buffer      = (CHAR8 *)AllocatePages (EFI_SIZE_TO_PAGES (SGX_DEFAULT_QUEUE_BUFFER_SIZE));
  SgxDebugQueue.BufferSize  = EFI_PAGES_TO_SIZE (EFI_SIZE_TO_PAGES (SGX_DEFAULT_QUEUE_BUFFER_SIZE) );
  SgxDebugQueue.HeapAlloc   = TRUE;
  if (!SgxDebugQueue.Buffer) {
    return EFI_NOT_FOUND;
  }

  SgxDebugQueue.Tail = (CHAR8 *)(SgxDebugQueue.Buffer);
  SgxDebugQueue.Length = 0;

  InitializeSpinLock (&QueueDebugPrintLock);
  return EFI_SUCCESS;
}

/**
  Initializes SgxDebugQueue by allocating memory for SgxDebugQueue
  buffer and initializing dedicated control variables. The memory
  is allocated in pages.

  @param BufferSize       Desired size of the queue buffer

  @retval EFI_NOT_FOUND   MemoryPages allocation failed
  @retval EFI_SUCCESS     Queue initialized succesfully
**/
EFI_STATUS
SgxDebugQueueCustomAllocInit (
  IN UINTN  BufferSize
  )
{
  SgxDebugQueue.Buffer      = (CHAR8 *) AllocatePages (EFI_SIZE_TO_PAGES (BufferSize));
  SgxDebugQueue.BufferSize  = EFI_PAGES_TO_SIZE (EFI_SIZE_TO_PAGES (BufferSize) );;
  SgxDebugQueue.HeapAlloc   = TRUE;
  if (!SgxDebugQueue.Buffer) {
    return EFI_NOT_FOUND;
  }

  SgxDebugQueue.Tail = (CHAR8 *)(SgxDebugQueue.Buffer);
  SgxDebugQueue.Length = 0;

  InitializeSpinLock (&QueueDebugPrintLock);
  return EFI_SUCCESS;
}

/**
  Initializes SgxDebugQueue using provided buffer
  for SgxDebugQueue and initializing dedicated control variables

  @param Buffer       Memory that will be used as queue buffer
  @param SufferSize   Size of passed buffer

  @retval EFI_NOT_FOUND   MemoryPool allocation failed
  @retval EFI_SUCCESS     Queue initialized succesfully
**/
EFI_STATUS
EFIAPI
SgxDebugQueueInit (
  IN OUT CONST CHAR8  *Buffer,
  IN UINTN            BufferSize
  )
{
  SgxDebugQueue.Buffer      = Buffer;
  SgxDebugQueue.BufferSize  = BufferSize;
  SgxDebugQueue.HeapAlloc   = FALSE;
  if (!SgxDebugQueue.Buffer) {
    return EFI_NOT_FOUND;
  }

  SgxDebugQueue.Tail = (CHAR8 *)(SgxDebugQueue.Buffer);
  SgxDebugQueue.Length = 0;

  InitializeSpinLock (&QueueDebugPrintLock);
  return EFI_SUCCESS;
}

/**
  Prints all messages queued in SgxDebugQueue and
  frees memory allocated for the queue

  If SgxDebugQueue has not been initialized (buffer == NULL) then ASSERT

  @retval EFI_NOT_READY         SgxDebugQueue has not been initialized and ASSERT macro is disabled
  @retval EFI_VOLUME_CORRUPTED  Queue buffer has been corrupted
  @retval EFI_SUCCESS           Queue initialized succesfully
**/
EFI_STATUS
SgxDebugQueueConsume (
  VOID
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  CHAR8       *CurrentMsg;
  UINTN       CurrentMsgLen;

  ASSERT (SgxDebugQueue.Buffer != NULL);
  if (SgxDebugQueue.Buffer == NULL) {
    return EFI_NOT_READY;
  }

  CurrentMsgLen = *((UINT8 *)(SgxDebugQueue.Buffer));
  CurrentMsg = (CHAR8 *)(SgxDebugQueue.Buffer) + 1;
  while ( SgxDebugQueue.Length > 0 ) {
    if ((CurrentMsg + CurrentMsgLen > SgxDebugQueue.Buffer + SgxDebugQueue.BufferSize) ||
        (*(CurrentMsg + CurrentMsgLen - 1) != '\0')) {
      //
      //  CurrentMsgLen exceeds buffer or
      //  the null terminator is missing
      //
      Status = EFI_VOLUME_CORRUPTED;
      break;
    }

    //
    // Print queued message
    //
    DebugPrint (EFI_D_ERROR, SGX_DBG_MESSAGE_FORMAT, CurrentMsg);

    //
    // Move to the next element in queue
    //
    SgxDebugQueue.Length--;
    if (SgxDebugQueue.Length <= 0) {
      break;
    }

    CurrentMsg += CurrentMsgLen + 1;
    CurrentMsgLen = ((UINT8 *)CurrentMsg)[-1];
  }
  if (SgxDebugQueue.HeapAlloc) {
    FreePages ((VOID *)SgxDebugQueue.Buffer, EFI_SIZE_TO_PAGES (SgxDebugQueue.BufferSize));
  }

  SgxDebugQueue.Buffer  = NULL;
  SgxDebugQueue.Tail    = NULL;
  SgxDebugQueue.Length  = 0;
  SgxDebugQueue.BufferSize = 0;

  return Status;
}

/**
  Helper function used to manage Tail pointer in the process
  of appending an element to the queue. It's responsible for
  placing Tail at the beginning of buffer's free space, so that
  the pointer can be used as a base for string printing.

  If SgxDebugQueue has not been initialized (buffer == NULL) then ASSERT

  @param BufferSize   Number of free bytes in buffer for the next
                      queue element

  @retval EFI_NOT_READY         SgxDebugQueue has not been initialized and ASSERT macro is disabled
  @retval EFI_BUFFER_TOO_SMALL  Space remaining in queue is smaller that
                                required minimum (SGX_MIN_QUEUE_ELEMENT_LEN)
  @retval EFI_SUCCESS           Queue initialized succesfully
**/
EFI_STATUS
SgxDebugQueueExtend (
  OUT UINTN *BufferSize
  )
{
  UINTN CurrentQueueSize;
  UINTN LastMsgLen;

  ASSERT (SgxDebugQueue.Buffer != NULL);
  if (SgxDebugQueue.Buffer == NULL) {
    return EFI_NOT_READY;
  }

  if (SgxDebugQueue.Tail == SgxDebugQueue.Buffer) {
    //
    // Queue is empty so Tail pointer doesnt have to be moved
    //
    *BufferSize = SgxDebugQueue.BufferSize;
  } else {
    //
    // Update tail to point at the beginning of buffer's free space
    //
    LastMsgLen = ((UINT8 *)(SgxDebugQueue.Tail))[-1];
    SgxDebugQueue.Tail += LastMsgLen;
    //
    // Calculate remaining size in queue buffer
    //
    CurrentQueueSize = (UINTN)( SgxDebugQueue.Tail - SgxDebugQueue.Buffer);
    *BufferSize = SgxDebugQueue.BufferSize - CurrentQueueSize;

    if (*BufferSize < SGX_MIN_QUEUE_ELEMENT_LEN) {
      //
      // Not enough space in buffer
      //
      return EFI_BUFFER_TOO_SMALL;
    }
  }
  if (*BufferSize > SGX_DBG_MESSAGE_MAX_LEN) {
    //
    // Max possible length of single message is defined by SGX_DBG_MESSAGE_MAX_LEN
    //
    *BufferSize = SGX_DBG_MESSAGE_MAX_LEN;
  }
  SgxDebugQueue.Length++;
  return EFI_SUCCESS;
}
