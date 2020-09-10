/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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
#include <Library/SysHostPointerLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/DebugLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Library/RcCompressLib.h>
#include <Library/HobLib.h>
#include <Library/TimerLib.h>
#include <Library/KtiApi.h>

#include <Ppi/MemoryPolicyPpi.h>

/**
  Creates and initializes data for RC debug functionality and sets pointer.

  @retval RETURN_SUCCESS       - Debug data initialized
  @retval EFI_OUT_OF_RESOURCES - Error creating HOB for debug data
**/
EFI_STATUS
InitializeDebugData (
  VOID
  )
{
  DEBUG_DATA *Debug;

  Debug = BuildGuidHob (&gDebugDataGuid, sizeof (DEBUG_DATA));
  if (Debug == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem (Debug, sizeof (DEBUG_DATA));
  Debug->MessageLevel = FixedPcdGet32(PcdRcDebugAllowedLevelsMask);
  SetDebugDataPointer (Debug);

  return EFI_SUCCESS;
}

/**
  Attempts to fetch debug data from SysHostPointerLib, and initializes if not found

  @param[in,out]  DebugDataPtr          Pointer which stores location of the debug data

  @retval RETURN_SUCCESS        Debug data ready
  @retval EFI_NOT_READY         Cannot retrieve debug data
  @retval EFI_OUT_OF_RESOURCES  Cannot retrieve debug data

**/
EFI_STATUS
GetDebugData (
  IN OUT DEBUG_DATA **DebugDataPtr
  )
{
  EFI_STATUS Status;
  VOID      *GuidHob;
  UINT8     *DataPtr;
  UINT8      ByteNum;

  *DebugDataPtr = (DEBUG_DATA *) GetDebugDataPointer ();

  if (*DebugDataPtr == NULL) {
    Status = InitializeDebugData ();
    if (EFI_ERROR (Status)) {
      return Status;
    }
    *DebugDataPtr = (DEBUG_DATA *) GetDebugDataPointer ();
    if (*DebugDataPtr == NULL) {
      return EFI_NOT_READY;
    }
  }

  DataPtr = (UINT8 *)(*DebugDataPtr);
  for (ByteNum = 0; ByteNum < 4; ByteNum++) {
    if (*(DataPtr + ByteNum) != 0xFF) {
      return EFI_SUCCESS; // Valid value means valid pointer
    }
  }

  GuidHob = GetFirstGuidHob (&gDebugDataGuid);
  if (GuidHob != NULL) {
    *DebugDataPtr = GET_GUID_HOB_DATA (GuidHob);
    SetDebugDataPointer (*DebugDataPtr); // Set pointer to relocated HOB
  } else {
    return EFI_NOT_READY;
  }

  return EFI_SUCCESS;
} // GetDebugData

/**
  Compress the contents of the debug buffer

  @param[in,out]  Buffer  Pointer to the buffer to be compressed.
  @param[in,out]  Size    Size of the buffer.

  @retval         Size of the compressed buffer.
**/
UINT32
CompressDebugBuffer (
  IN OUT UINT8  *Buffer,
  IN OUT UINT32  Size
  )
{
  UINT8  TempBuffer[DEBUG_BUF_SIZE];
  UINT32 TempSize;
  UINT32 Status = SUCCESS;

  TempSize = sizeof(TempBuffer);
  Status = CompressRc (Buffer, Size, TempBuffer, &TempSize);

  if (Status != SUCCESS) {
    return 0;
  }

  CopyMem (Buffer, TempBuffer, TempSize);
  return TempSize;
} // CompressDebugBuffer

/**
  Build packet for sending. Does compression, socket split, and checksum.

  @param[in,out]  Debug  Pointer to the debug data structure.

  @retval        Pointer to final printable buffer.
**/
CHAR8 *
EncodeAndCompress (
  IN OUT DEBUG_DATA *Debug
  )
{
  UINT16 i;
  UINT8  Checksum;

  if (Debug == NULL) {
    return NULL; // Nothing we can do.
  }

  Debug->BufferData.SocketId = GetSysSocketId ();
  if (Debug->EnableCompression) {
    Debug->BufferData.CurrentPosition = (UINT16) CompressDebugBuffer (
                                                   Debug->BufferData.Buffer,
                                                   Debug->BufferData.CurrentPosition
                                                   );

    Debug->BufferData.SocketId |= 0x80; // MSB indicates compression
  }

  Checksum = 0;
  Debug->BufferData.CurrentPosition += 4;
  for (i = 0; i < Debug->BufferData.CurrentPosition - 1; i++) {
    Checksum += *((UINT8 *)&Debug->BufferData.SocketId + i);
  }
  *((UINT8 *)&Debug->BufferData.SocketId + i) = 0 - Checksum; // Append checksum

  return (CHAR8 *)&Debug->BufferData.SocketId;
} // EncodeAndCompress

/**
  Processes debug buffer and relays final output

  @param[in,out]  Debug       Pointer to the debug data structure.
  @param[in]      Buffer      Pointer to the buffer to be processed.
  @param[in,out]  BufferSize  Pointer to the size of buffer.

  @retval             Pointer to the final buffer, NULL if nothing to print.

**/
CHAR8 *
ProcessBuffer (
  IN OUT DEBUG_DATA *Debug,
  IN     CHAR8      *Buffer,
  IN OUT UINTN      *BufferSize
  )
{
  UINT64      TimeDiff = 0;
  UINT32      FinalSize;
  UINT8       *FinalBuffer = NULL;

  if (Debug == NULL) {
    return NULL; // Nothing we can do.
  }

  //
  // Copy to internal buffer.  It should always fit, but
  // check size anyway for safety and discard if not.
  //
  if ((Debug->BufferData.CurrentPosition + *BufferSize) < DEBUG_BUF_SIZE) {
    CopyMem (
      (Debug->BufferData.Buffer + Debug->BufferData.CurrentPosition),
      Buffer,
      *BufferSize
      );
    Debug->BufferData.CurrentPosition += (UINT16)(*BufferSize);
  }

  if (Debug->BufferData.LastTimestamp != 0) {
    TimeDiff = GetPerformanceCounter ();
    TimeDiff = GetTimeInNanoSecond (TimeDiff - Debug->BufferData.LastTimestamp);
  }

  //
  // Send if timer was zero, buffer was full, or time ran out
  //
  if ((Debug->BufferData.LastTimestamp == 0) ||
      (Debug->BufferData.CurrentPosition > DEBUG_BUF_LIMIT) ||
      (TimeDiff > BUFFER_FLUSH_TIME)) {
    if (Debug->EnableSocketSplit) {
      FinalBuffer = (UINT8 *)EncodeAndCompress (Debug);
    } else {
      FinalBuffer = Debug->BufferData.Buffer;
    }
    FinalSize = Debug->BufferData.CurrentPosition;

    //
    // Reset counters
    //
    Debug->BufferData.CurrentPosition = 0;
    Debug->BufferData.LastTimestamp = GetPerformanceCounter ();
  }

  return (CHAR8 *)FinalBuffer;
} // ProcessBuffer

/**
  rcPrintf with stdargs varargs stack frame.

  @param[in]  Debug     Debug data to populate status code
  @param[in]  Format    String containing characters to print and formating data.
  @param[in]  Marker    Data to be parsed and placed into format string.

**/
VOID
RcDebugPrintCore (
  IN DEBUG_DATA  *Debug,
  IN CONST CHAR8 *Format,
  IN VA_LIST      Marker
  )
{
  UINT8           Buffer[MAX_EX_DEBUG_SIZE];
  EX_DEBUG_INFO   *ExDebugInfo;

  ExDebugInfo = (EX_DEBUG_INFO*)Buffer;
  ExDebugInfo->DebugString = (CHAR8*)(ExDebugInfo + 1);

  //
  // Fill debug policy info with host data
  //

  if (Debug->EnableBuffering) {
    ExDebugInfo->ProcessBuffer = (PROCESS_BUFFER)ProcessBuffer;
  } else {
    ExDebugInfo->ProcessBuffer = NULL;
  }
  ExDebugInfo->ProcessDataPtr = Debug;
  ExDebugInfo->PrintSyncAcquire = Debug->AcquirePrintFunction;
  ExDebugInfo->PrintSyncRelease = Debug->ReleasePrintFunction;

  //
  // Parse format string - truncate rather than drop if too large
  //

  ExDebugInfo->DebugStringLen = (UINT32) AsciiVSPrint (
                                           ExDebugInfo->DebugString,
                                           MAX_EX_DEBUG_STR_LEN,
                                           Format,
                                           Marker
                                           );

  REPORT_STATUS_CODE_EX (
    EFI_DEBUG_CODE,
    (EFI_SOFTWARE_UNSPECIFIED | EFI_DC_UNSPECIFIED),
    0,
    NULL,
    &gStatusCodeDataTypeExDebugGuid,
    ExDebugInfo,
    sizeof (EX_DEBUG_INFO) + ExDebugInfo->DebugStringLen
    );
} // RcDebugPrintCore

/**
  Send request to call print acquisition function.
  This is used to keep cohesive prints in a multi-socket environment.

**/
VOID
AcquirePrintControl (
  VOID
  )
{
  EX_DEBUG_INFO   ExDebugInfo;
  DEBUG_DATA      *Debug;

  Debug = GetDebugDataPointer ();
  if (Debug == NULL) {
    return; // Nothing we can do
  }

  if (Debug->MessageLevel) {
    ExDebugInfo.DebugString = NULL;
    ExDebugInfo.DebugStringLen = 0;
    ExDebugInfo.ProcessBuffer = NULL;
    ExDebugInfo.ProcessDataPtr = NULL;
    ExDebugInfo.PrintSyncAcquire = Debug->AcquirePrintFunction;
    ExDebugInfo.PrintSyncRelease = NULL;

    REPORT_STATUS_CODE_EX (
      EFI_DEBUG_CODE,
      (EFI_SOFTWARE_UNSPECIFIED | EFI_DC_UNSPECIFIED),
      0,
      NULL,
      &gStatusCodeDataTypeExDebugGuid,
      &ExDebugInfo,
      sizeof (EX_DEBUG_INFO)
      );
  }
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
  EX_DEBUG_INFO   ExDebugInfo;
  DEBUG_DATA      *Debug;

  Debug = GetDebugDataPointer ();
  if (Debug == NULL) {
    return; // Nothing we can do
  }

  if (Debug->MessageLevel) {
    ExDebugInfo.DebugString = NULL;
    ExDebugInfo.DebugStringLen = 0;
    ExDebugInfo.ProcessBuffer = NULL;
    ExDebugInfo.ProcessDataPtr = NULL;
    ExDebugInfo.PrintSyncAcquire = NULL;
    ExDebugInfo.PrintSyncRelease = Debug->ReleasePrintFunction;

    REPORT_STATUS_CODE_EX (
      EFI_DEBUG_CODE,
      (EFI_SOFTWARE_UNSPECIFIED | EFI_DC_UNSPECIFIED),
      0,
      NULL,
      &gStatusCodeDataTypeExDebugGuid,
      &ExDebugInfo,
      sizeof (EX_DEBUG_INFO)
      );
  }
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
  CHAR8 Buffer[MAX_LINE];

  if (Count > MAX_LINE - 3) { // Leave room for 2x \n and null terminator
    Count = MAX_LINE - 3; // Truncate
  }

  if (DoubleLine) { // Select line character
    SetMem (Buffer, MAX_LINE, '=');
  } else {
    SetMem (Buffer, MAX_LINE, '-');
  }

  if (CrLfBefore) {
    Buffer[0] = '\n'; // Add \n before
    Count++;
  }

  if (CrLfAfter) {
    Buffer[Count++] = '\n'; // Add \n after
  }

  Buffer[Count] = 0; // Add null-terminator

  RcDebugPrint (DbgLevel, Buffer);
} // RcDebugPrintLine

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
  CHAR8 Buffer[MAX_LINE];

  SetMem (Buffer, MAX_LINE, ' ');

  if (NumSpaces > MAX_LINE - 2) { // Leave room for | and null terminator
    NumSpaces = MAX_LINE - 2;  // Truncate to max
  }

  Buffer[NumSpaces] = '|'; // Add |
  Buffer[NumSpaces + 1] = 0; // Add null-terminator

  RcDebugPrint (DbgLevel, Buffer);
} // RcDebugPrintEmptyBlock

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
  IN UINT32    DbgLevel,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT8     SubCh,
  IN UINT8     Rank,
  IN UINT8     Strobe,
  IN UINT8     Bit,
  IN CHAR8     *Format,
  ...
  )
{
  VA_LIST Marker;
  CHAR8 DevString[MAX_LINE];
  CHAR8 *Temp = DevString;
  UINTN CharCount;
  EFI_STATUS Status;
  DEBUG_DATA *Debug = NULL;

  Status = GetDebugData (&Debug);
  if (EFI_ERROR (Status) || Debug == NULL) {
    return; // Nothing we can do
  }

  if ((Debug->MessageLevel & DbgLevel) == 0) {
    return; // Message is masked by debug level
  }

  AcquirePrintControl ();
  if (Socket != NO_SOCKET) {
    CharCount = AsciiSPrint (Temp, MAX_LINE, "N%d.", Socket);
    Temp += CharCount;
  }

  if ((Channel & HBM_IO_REPLACE) != 0 && Channel != NO_CH) {
    CharCount = AsciiSPrint (Temp, MAX_LINE, "IO%d.", Channel&~HBM_IO_REPLACE);
    Temp += CharCount;
  } else if (Channel != NO_CH) {
    CharCount = AsciiSPrint (Temp, MAX_LINE, "C%02d.", Channel);
    Temp += CharCount;
  }

  if (Dimm != NO_DIMM) {
    CharCount = AsciiSPrint (Temp, MAX_LINE, "D%d.", Dimm);
    Temp += CharCount;
  }

  if (SubCh != NO_SUBCH) {
    CharCount = AsciiSPrint (Temp, MAX_LINE, "SC%d.", SubCh);
    Temp += CharCount;
  }

  if (Rank != NO_RANK) {
    CharCount = AsciiSPrint (Temp, MAX_LINE, "R%d.", Rank);
    Temp += CharCount;
  }

  if (Strobe != NO_STROBE) {
    CharCount = AsciiSPrint (Temp, MAX_LINE, "S%02d.", Strobe);
    Temp += CharCount;
  }

  if (Bit != NO_BIT) {
    CharCount = AsciiSPrint (Temp, MAX_LINE, "B%02d.", Bit);
    Temp += CharCount;
  }

  if (Temp != DevString) {
    AsciiSPrint ((Temp - 1), MAX_LINE, ": ");
    RcDebugPrint (DbgLevel, DevString);
  }

  VA_START (Marker, Format);
  RcDebugPrintCore (Debug, Format, Marker);
  VA_END (Marker);

  ReleasePrintControl ();
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
  VA_LIST     Marker;
  DEBUG_DATA *Debug = NULL;
  EFI_STATUS  Status;

  Status = GetDebugData (&Debug);
  if (EFI_ERROR (Status) || Debug == NULL) {
    return; // Nothing we can do
  }

  if ((Debug->MessageLevel & DbgLevel) == 0) {
    return; // Message is masked by debug level
  }

  VA_START (Marker, Format);
  RcDebugPrintCore (Debug, Format, Marker);
  VA_END (Marker);
} // RcDebugPrintInternal

/**
  Flush buffered debug output.

**/
VOID
FlushDebugBuffer (
  VOID
  )
{
  DEBUG_DATA *Debug = GetDebugDataPointer ();

  if (Debug == NULL) {
    return; // Nothing we can do
  }

  Debug->BufferData.LastTimestamp = 0; // Set timer to 0 for instant flush
  RcDebugPrint (SDBG_DEFAULT, "");
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
  SYS_SETUP   *Setup;
  DEBUG_DATA  *Debug;
  UINT32      RcDebugLevelDelta;
  UINT32      RcDebugBuildTimeDefault;

  GetDebugData (&Debug);

  Setup                   = GetSysSetupPointer ();
  RcDebugBuildTimeDefault = FixedPcdGet32 (PcdRcDebugAllowedLevelsMask);

  if (Debug == NULL || Setup == NULL) {
    return; // Nothing we can do.
  }

  //
  // Enable debug features based on setup options
  //
  Debug->MessageLevel = Setup->common.serialDebugMsgLvl;

  //
  // Only enable ERROR messages if all debug isnt being masked.
  //
  if (Debug->MessageLevel > 0) {
    Debug->MessageLevel |= SDBG_ERROR;
  }

  //
  // Debug message are stripped out by the compiler based on the build time level.
  // The max effective RunTime level is equal to BuildTime level. So if the requested
  // RunTime level is greater than the BuildTime level, then print a warning as such,
  // and output the requested RunTime bits that are unavailable in this build.
  //
  RcDebugLevelDelta = ((RcDebugBuildTimeDefault ^ Debug->MessageLevel) & ~RcDebugBuildTimeDefault);
  if (RcDebugLevelDelta != 0) {
    DEBUG((EFI_D_ERROR, "RunTime Debug Msg level is > BuildTime level, Debug Msg limited to BuildTime level: 0x%x\n", RcDebugLevelDelta));
  }

  Debug->EnableBuffering = Setup->common.serialBufEnable;
  Debug->EnableSocketSplit = Setup->common.serialPipeEnable;
  Debug->EnableCompression = Setup->common.serialPipeCompress;
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
  DEBUG_DATA *Debug;

  GetDebugData (&Debug);

  if (Debug == NULL) {
    return; // Nothing we can do.
  }

  Debug->AcquirePrintFunction = Acquire;
  Debug->ReleasePrintFunction = Release;

  RcDebugPrint (SDBG_DEFAULT, "RC debug buffering, compression, and sync ready\n");
} // UpdateSemaphoreSyncData

/**
  Cleans up debug message infrastructure.

**/
VOID
ResetDebugMessages (
  VOID
  )
{
  DEBUG_DATA *Debug = GetDebugDataPointer ();

  if (Debug == NULL) {
    return; // Nothing we can do
  }

  FlushDebugBuffer ();
  Debug->EnableBuffering = SDBG_BUF_DISABLE;
  Debug->EnableSocketSplit = SDBG_PIPE_DISABLE;
  Debug->EnableCompression = SDBG_PIPE_COMPRESS_DISABLE;
  Debug->AcquirePrintFunction = NULL;
  Debug->ReleasePrintFunction = NULL;
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
  return (BOOLEAN) ((ErrorLevel & FixedPcdGet32 (PcdRcDebugAllowedLevelsMask)) != 0);
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
  UINT32   DebugLevel = SDBG_NONE;
#ifdef DEBUG_CODE_BLOCK
  DEBUG_DATA  *Debug;

  Debug = GetDebugDataPointer ();

  if (Debug != NULL) {
    DebugLevel = Debug->MessageLevel;
  }
#endif // DEBUG_CODE_BLOCK
  return DebugLevel;
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
  DEBUG_DATA  *Debug;

  Debug = GetDebugDataPointer ();

  if (Debug != NULL) {
    Debug->MessageLevel = DebugLevel;
  }
}