/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef _SGX_DEBUG_LIB_H_
#define _SGX_DEBUG_LIB_H_

#include <Uefi/UefiBaseType.h>
#include <Library/SynchronizationLib.h>

#define SGX_DEFAULT_QUEUE_BUFFER_SIZE         2000
#define SGX_DBG_MESSAGE_MAX_LEN               256
#define SGX_DBG_MESSAGE_FORMAT                "%a\n"
#define SGX_DBG_MESSAGE_CODE_FORMAT           "[SGX] %03x"
#define SGX_DBG_MESSAGE_CODE_FORMAT_VERBOSE   "[SGX] %03x %a"
//
// Sizeof minimal queued debug msg  ("XXX" + "\0" + msg_size )
//
#define SGX_MIN_QUEUE_ELEMENT_LEN 6

#define _SGX_VERBOSE_DEBUG_

/**
  Generic error codes

  PEI_PREMEM              0x100
  PEI_POSTMEM             0x200
  DXE_SMM                 0x300
  DXE                     0x400
  BIOS_DONE_CALLBACK      0x500
  BIOS_DONE_CALBACK_SMM   0x600
**/
typedef enum {
  //
  // base codes
  //
  PEI_100_DBG           = 0x100,
  PEI_200_DBG           = 0x200,
  DXE_SMM_300_DBG       = 0x300,
  DXE_400_DBG           = 0x400,
  BIOS_DONE_500_DBG     = 0x500,
  BIOS_DONE_SMM_600_DBG = 0x600,

  //
  // PEI MpServices
  //
  PEI_210_ERROR_DETECTING_THREAD     = 0x210,

  PEI_217_CONFIGURE_PRMRRS_BEGIN     = 0x217,
  PEI_218_CONFIGURE_PRMRRS_END       = 0x218,
  PEI_219_PRMRR_BASE                 = 0x219,
  PEI_21A_PRMRR_MASK                 = 0x21A,

  PEI_21B_ENABLING_SGX_DEBUG_MODE    = 0x21B,
  PEI_21C_DISABLING_SGX_DEBUG_MODE   = 0x21C,

  PEI_21D_MSR_IA32_FEATURE_CONTROL   = 0x21D,
  PEI_21E_FLEXIBLE_LAUNCH_CONTROL    = 0x21E,
  PEI_21F_SGX_QOS_ENABLED_AND_LOCKED = 0x21F,
} SGX_DBG_MESSAGE_ID;

#ifdef _SGX_VERBOSE_DEBUG_
//
// Map used to translate SGX_DBG_MESSAGE_ID into CONST CHAR* arrays
//
static CONST CHAR8* MessageMap[0x6FF] = {
  //
  // PEI MpServices
  //
  [PEI_210_ERROR_DETECTING_THREAD]      = "Error detecting thread",

  [PEI_217_CONFIGURE_PRMRRS_BEGIN]      = "ConfigureCorePrmrrs BEGIN",
  [PEI_218_CONFIGURE_PRMRRS_END]        = "ConfigureCorePrmrrs END",
  [PEI_219_PRMRR_BASE]                  = "MSR_PRMRR_BASE",
  [PEI_21A_PRMRR_MASK]                  = "MSR_PRMRR_MASK",

  [PEI_21B_ENABLING_SGX_DEBUG_MODE]     = "Enabling SGX Debug Mode",
  [PEI_21C_DISABLING_SGX_DEBUG_MODE]    = "Disabling SGX Debug Mode",
  [PEI_21D_MSR_IA32_FEATURE_CONTROL]    = "MSR_IA32_FEATURE_CONTROL",
  [PEI_21E_FLEXIBLE_LAUNCH_CONTROL]     = "Flexible Launch Control",
  [PEI_21F_SGX_QOS_ENABLED_AND_LOCKED]  = "SGX QoS Enabled & Locked"
};
#endif

/**
  Debug queue data
  Queue consists of a linear buffer where each null-terminated string is preceded by one-byte containing string's size
  For example execution of these two instructions (assuming that _SGX_VERBOSE_DEBUG_ is not defined, and default format is "[SGX] %x\n"):
    SgxQueueDebug (PEI_219_PRMRR_BASE, NULL)
    SgxQueueDebug (PEI_21A_PRMRR_MASK, "%d", 5)
  Will result in following buffer layout:
    +-----+-----+-----+-----+------+-----+-----+-----+-----+-----+-----+------+
    |  4  | '2' | '1' | '9' | '\0' |  6  | '2' | '1' | 'A' | ' ' | '5' | '\0' |
    +-----+-----+-----+-----+------+-----+-----+-----+-----+-----+-----+------+
       ^                              ^
       |                              |
    msg_len                        msg_len

  Queue's organization is FIFO so SgxDebugQueueConsume () will produce following oputput:
    [SGX] 219
    [SGX] 21A 5
**/
typedef struct {
  CONST CHAR8   *Buffer;
  CHAR8         *Tail;
  UINTN         Length;
  UINTN         BufferSize;
  BOOLEAN       HeapAlloc;
} SGX_DBG_QUEUE;

/**
  Returns a pointer to SgxDebugQueue

  @retval SGX_DBG_QUEUE * A pointer to SgxDebugQueue used by all functions
**/
SGX_DBG_QUEUE *
GetSgxDebugQueuePtr (
  VOID
  );

/**
  Returns a pointer to QueueDebugPrintLock

  @retval SPIN_LOCK * A pointer to QueueDebugPrintLock used by all functions
**/
SPIN_LOCK *
GetQueueDebugPrintLockPtr (
  VOID
  );

/**
  Prints a DbgMessageID with given error level. The message is preceded
  by prefix defined in SGX_DBG_MESSAGE_CODE_FORMAT and followed by newline
  symbol.

  If _SGX_VERBOSE_DEBUG_ is defined, then debug message is followed by
  verbal message assigned to specific SGX_DBG_MESSAGE_ID value.
  Otherwise print just the SGX_DBG_MESSAGE_ID.

  If Format is not NULL, then the generic message is followed by Format
  and the associated variable argument list

  If any of the bits in ErrorLevel isn't also set in DebugPrintErrorLevelLib function
  GetDebugPrintErrorLevel (), then don't print the message and return

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
  );

/**
  Prints a debug message with given error level. The message is preceded
  by prefix defined in SGX_DBG_MESSAGE_CODE_FORMAT and followed by newline
  symbol. The only difference between SgxDebugPrintS and SgxDebugPrint
  is that SgxDebugPrintS isn't affected by _SGX_VERBOSE_DEBUG_ macro,
  so the debug will always be sparse.

  If Format is not NULL, then the generic message is followed by Format
  and the associated variable argument list

  If any of the bits in ErrorLevel isn't also set in DebugPrintErrorLevelLib function
  GetDebugPrintErrorLevel (), then don't print the message and return

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
  );

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

  If any of the bits in ErrorLevel isn't also set in DebugPrintErrorLevelLib function
  GetDebugPrintErrorLevel (), then don't queue the message and return

  @param  ErrorLevel    The error level of the debug message
  @param  DbgMessageID  Generic sgx debug message ID.
  @param  Format        Format string for the debug message to print.
  @param  ...           Variable argument list whose contents are accessed
                        based on the format string specified by Format.

  @retval EFI_ABORTED           Specified error level is suppressed
  @retval EFI_BUFFER_TOO_SMALL  SgxDebugQueue is full
  @retval EFI_SUCCESS           Debug Message queued successfully
**/
EFI_STATUS
SgxQueueDebugM (
  IN  UINTN               ErrorLevel,
  IN  SGX_DBG_MESSAGE_ID  DbgMessageID,
  IN  CONST CHAR8         *Format OPTIONAL,
  ...
  );

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

  If any of the bits in ErrorLevel isn't also set in DebugPrintErrorLevelLib function
  GetDebugPrintErrorLevel (), then don't queue the message and return

  @param  ErrorLevel    The error level of the debug message
  @param  DbgMessageID  Generic sgx debug message ID.
  @param  Format        Format string for the debug message to print.
  @param  ...           Variable argument list whose contents are accessed
                        based on the format string specified by Format.

  @retval EFI_ABORTED           Specified error level is suppressed
  @retval EFI_BUFFER_TOO_SMALL  SgxDebugQueue is full
  @retval EFI_SUCCESS           Debug Message queued successfully
**/
EFI_STATUS
SgxQueueDebugMS (
  IN  UINTN               ErrorLevel,
  IN  SGX_DBG_MESSAGE_ID  DbgMessageID,
  IN  CONST CHAR8         *Format OPTIONAL,
  ...
  );

/**
  Queues a debug message with given error level. The message is followed
  by the newline symbol. This function is Mp safe, which makes it convenient
  for MpServices debugging. This function works more like convetnional printing
  methods with just the format and VA_List in contrast to SgxQueueDebugM,
  which uses MessageMap and predefined prefix

  If Format is NULL, then ASSERT

  If SgxDebugQueue has not been initialized (buffer == NULL) then ASSERT

  If any of the bits in ErrorLevel isn't also set in DebugPrintErrorLevelLib function
  GetDebugPrintErrorLevel (), then don't queue the message and return

  @param  ErrorLevel    The error level of the debug message
  @param  Format        Format string for the debug message to print.
  @param  ...           Variable argument list whose contents are accessed
                        based on the format string specified by Format.

  @retval EFI_ABORTED           Specified error level is suppressed
  @retval EFI_BUFFER_TOO_SMALL  SgxDebugQueue is full
  @retval EFI_SUCCESS           Debug Message queued successfully
**/
EFI_STATUS
SgxQueueDebug (
  IN  UINTN               ErrorLevel,
  IN  CONST CHAR8         *Format,
  ...
  );

/**
  Initializes SgxDebugQueue by allocating memory for SgxDebugQueue
  buffer and initializing dedicated control variables

  @retval EFI_NOT_FOUND   MemoryPages allocation failed
  @retval EFI_SUCCESS     Queue initialized succesfully
**/
EFI_STATUS
SgxDebugQueueAllocInit (
  VOID
  );

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
  );

/**
  Initializes SgxDebugQueue using provided buffer
  for SgxDebugQueue and initializing dedicated control variables

  @param Buffer       Memory that will be used as queue buffer
  @param SufferSize   Size of passed buffer

  @retval EFI_NOT_FOUND   MemoryPool allocation failed
  @retval EFI_SUCCESS     Queue initialized succesfully
**/
EFI_STATUS
SgxDebugQueueInit (
  IN OUT CONST CHAR8  *Buffer,
  IN UINTN            BufferSize
  );


/**
  Prints all messages queued in SgxDebugQueue and
  frees memory allocated for the queue

  If SgxDebugQueue has not been initialized (buffer == NULL) then ASSERT

  @retval EFI_VOLUME_CORRUPTED  Queue buffer has been corrupted
  @retval EFI_SUCCESS           Queue initialized succesfully
**/
EFI_STATUS
SgxDebugQueueConsume (
  VOID
  );

/**
  Helper function used to manage Tail pointer in the process
  of appending an element to the queue. It's responsible for
  placing Tail at the beginning of buffer's free space, so that
  the pointer can be used as a base for string printing.

  If SgxDebugQueue has not been initialized (buffer == NULL) then ASSERT

  @param BufferSize   Number of free bytes in buffer for the next
                      queue element

  @retval EFI_BUFFER_TOO_SMALL  Space remaining in queue is smaller that
                                required minimum (SGX_MIN_QUEUE_ELEMENT_LEN)
  @retval EFI_SUCCESS           Queue initialized succesfully
**/
EFI_STATUS
SgxDebugQueueExtend (
  OUT UINTN *BufferSize
  );

#endif
