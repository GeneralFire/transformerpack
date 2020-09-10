/** @file

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

#include <Library/RcDebugLib.h>

/**
  Send request to call print acquisition function.
  This is used to keep cohesive prints in a multi-socket environment.

**/
VOID
AcquirePrintControl (
  VOID
  )
{
} // AcquirePrintControl

/**
  Send request to call print release function.
  This is used to keep cohesive prints in a multi-socket environment.

**/
VOID
ReleasePrintControl (
  VOID
  )
{
} // ReleasePrintControl

/**
  Prints a line or a double line

  @param[in]  Count       Number of units in the line
  @param[in]  DoubleLine  0 prints '-'   1 prints '='
  @param[in]  CrLfBefore  1 = return before printing; 0 = ignore
  @param[in]  CrLfAfter   1 = return after printing; 0 = ignore

**/
VOID
RcDebugPrintLine (
  IN UINT32 DbgLevel,
  IN UINT16 Count,
  IN UINT8  DoubleLine,
  IN UINT8  CrLfBefore,
  IN UINT8  CrLfAfter
  )
{
} // PrintLine

/**
  Prints a string of spaces followed by a |

  @param[in]  DbgLevel   Debug level
  @param[in]  NumSpaces  Number of spaces for the empty block
**/
VOID
RcDebugPrintEmptyBlock (
  IN UINT32 DbgLevel,
  IN UINT8  NumSpaces
  )
{
} // EmptyBlock

/**
  MRC specific print to serial output with device information

  @param[in]  Host      Pointer to the system Host (root) structure
  @param[in]  DbgLevel  Debug level
  @param[in]  Socket    Socket
  @param[in]  Channel   Channel
  @param[in]  Dimm      Dimm
  @param[in]  Subch     SubCh
  @param[in]  Rank      Rank
  @param[in]  Strobe    Strobe
  @param[in]  Bit       Bit
  @param[in]  Format    string format
  @param[in]  ...       Varargs

**/

VOID
RcDebugPrintWithDeviceInternal (
  IN UINT32 DbgLevel,
  IN UINT8  Socket,
  IN UINT8  Channel,
  IN UINT8  Dimm,
  IN UINT8  SubCh,
  IN UINT8  Rank,
  IN UINT8  Strobe,
  IN UINT8  Bit,
  IN CHAR8 *Format,
  ...
  )
{
} // RcDebugPrintWithDeviceInternal

/**
  MRC specific print to serial output.

  @param[in]  DbgLevel  Level of message to be printed.
  @param[in]  Format    A "printf" format string.
  @param[in]  ...       Varargs matching specified format.

**/
VOID
RcDebugPrintInternal (
  IN UINT32 DbgLevel,
  IN CHAR8* Format,
  IN ...
  )
{
} // RcDebugPrintInternal

/**
  Flush buffered debug output.

**/
VOID
FlushDebugBuffer (
  VOID
  )
{
} // FlushDebugBuffer

/**
  Initializes debug message infrastructure.

  @param VOID

  @retval VOID

**/
VOID
InitializeDebugMessages (
  VOID
  )
{
} // InitializeDebugMessages

/**
  Update Semaphore sync data based on the present sockets.

  @param[in]  Acquire  Pointer to semaphore acquire function.
  @param[in]  Release  Pointer to semaphore release function.

**/
VOID
UpdateSemaphoreSyncData (
  IN PRINT_SYNC_ACQUIRE Acquire,
  IN PRINT_SYNC_RELEASE Release
  )
{
} // UpdateSemaphoreSyncData

/**
  Cleans up debug message infrastructure.

**/
VOID
ResetDebugMessages (
  VOID
  )
{
} // ResetDebugMessages

/**
  Returns TRUE if any one of the bit is set both in ErrorLevel and PcdRcDebugAllowedLevelsMask.

  This function compares the bit mask of ErrorLevel and PcdRcDebugAllowedLevelsMask.

  @retval  TRUE    Current ErrorLevel is supported.
  @retval  FALSE   Current ErrorLevel is not supported.

**/
BOOLEAN
EFIAPI
CheckDebugPrintLevel (
  IN  CONST UINTN        ErrorLevel
  )
{
  return FALSE;
} // CheckDebugPrintLevel

/**

  Get the debug level.

  @param  None

  @retval Debug level.

**/

UINT32
EFIAPI
GetDebugLevel (
  VOID
  )
{
  return 0;
}

/**

  Set the debug level.

  @param[in] DebugLevel - Debug level to set.

  @retval None.

**/

VOID
EFIAPI
SetDebugLevel (
  IN UINT32  DebugLevel
  )
{
  return;
}
