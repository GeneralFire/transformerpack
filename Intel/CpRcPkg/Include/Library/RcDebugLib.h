/** @file
  Interface header file for the RC debug library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation. <BR>

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

#ifndef _RC_DEBUG_LIB_H_
#define _RC_DEBUG_LIB_H_

#include <Uefi.h>
#include <Guid/StatusCodeDataTypeExDebug.h>

//
// Wait period in NS before forced buffer flush
//

#define BUFFER_FLUSH_TIME 75000

//
// Debug levels
// NOTE: Each level(bit) is meant to be independent, and should not have any hierarchical relationship.
//
#define SDBG_NONE           0
#define SDBG_ERROR          BIT0
#define SDBG_WARN           BIT1
#define SDBG_TRACE          BIT2
#define SDBG_MEM_TRAIN      BIT3
#define SDBG_TST            BIT4
#define SDBG_CPGC           BIT5
#define SDBG_REG_ACCESS     BIT6  // Displays all register accesses.
#define SDBG_SCRATCHPAD     BIT7
#define SDBG_DIMM_INFO      BIT8
#define SDBG_MARGINS        BIT9
#define SDBG_RCWRITETAG     BIT10
#define SDBG_INFO           BIT11
#define SDBG_CRITICAL_INFO  BIT12
#define SDBG_HBM            BIT13

#define SDBG_SWEEP_ERROR    BIT20
#define SDBG_SWEEP_WARN     BIT21
#define SDBG_SWEEP_RESULTS  BIT22   ///< Force to print the results (overwrites debug level to 'SDBG_SWEEP_L0')
#define SDBG_SWEEP_L0       BIT23   ///< Level 0 debug messages (most important)
                                    ///< Strobe header
                                    ///< Result condition at a bit level
                                    ///< Accessed Nx.Cxx.Dx.Rx.
#define SDBG_SWEEP_L1       BIT24   ///< Level 1 debug messages
                                    ///< Current delay values
#define SDBG_SWEEP_L2       BIT25   ///< Level 2 debug messages
                                    ///< Current strobe limits
                                    ///< Current state for each strobe
#define SDBG_SWEEP_L3       BIT26   ///< Level 3 debug messages (least important, only for debug purposes)
                                    ///< Current dither value at knob/bit per each strobe
#define SDBG_SWEEP_MASK     (((SDBG_SWEEP_L3 << 1) - 1) & ~(SDBG_SWEEP_ERROR - 1))

#define SDBG_FORCE_ALL      BIT29 // Don't suppress messages when set
#define SDBG_BSSA           BIT30
#define SDBG_LEGACY         BIT31 // For messages previously without a level

//
// Combo Debug Levels
// NOTE: These are just for backward compatibility and may be removed in future.
//       Use specific level macro(s) from above when creating debug messages.
//
// NOTE: Creating or using Combo Levels is not a good Debug practice as it makes
//       it very difficult to disable those prints. To show a print only one of
//       the combo bits needs to be set, but to suppress the print all the combo
//       bits must be unset.
//
#define SDBG_MIN            SDBG_CRITICAL_INFO
#define SDBG_MAX            SDBG_INFO
#define SDBG_DEFAULT        SDBG_LEGACY
#define SDBG_MINMAX         (SDBG_INFO | SDBG_ERROR | SDBG_WARN | SDBG_CRITICAL_INFO)

//
// Definitions for use with PrintLine
//
#define SINGLE_LINE     0
#define DOUBLE_LINE     1
#define NOCRLF_FLAG     0
#define CRLF_FLAG       1

#define DEBUG_BUF_LIMIT (4096 - 160)
#define DEBUG_BUF_SIZE  (DEBUG_BUF_LIMIT + 160)

#pragma pack(1)
typedef struct {
  UINT64                    LastTimestamp;
  UINT8                     SocketId;
  UINT16                    CurrentPosition;
  UINT8                     Buffer[DEBUG_BUF_SIZE];
} DEBUG_BUFFER;
#pragma pack()

typedef struct {
  DEBUG_BUFFER              BufferData;
  UINT8                     EnableBuffering;
  UINT8                     EnableSocketSplit;
  UINT8                     EnableCompression;
  UINT32                    MessageLevel;
  PRINT_SYNC_ACQUIRE        AcquirePrintFunction; // Currently must be dynamic link
  PRINT_SYNC_RELEASE        ReleasePrintFunction; // and no independent PPI can exist
} DEBUG_DATA;

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
  );

/**
  Send request to call print acquisition function.
  This is used to keep cohesive prints in a multi-socket environment.

**/
VOID
AcquirePrintControl (
  VOID
  );

/**
  Send request to call print release function.
  This is used to keep cohesive prints in a multi-socket environment.

**/
VOID
ReleasePrintControl (
  VOID
  );

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
  );

/**
  Prints a string of spaces followed by a |

  @param[in]  DbgLevel   Debug level
  @param[in]  NumSpaces  Number of spaces for the empty block
**/
VOID
RcDebugPrintEmptyBlock (
  IN UINT32 DbgLevel,
  IN UINT8  NumSpaces
  );

//
// Definitions for use with RcDebugPrintWithDevice
// to indication no socket, channel, etc.
//

#define NO_SOCKET       0xFF
#define NO_CH           0xFF
#define NO_DIMM         0xFF
#define NO_SUBCH        0xFF
#define NO_RANK         0xFF
#define NO_STROBE       0xFF
#define NO_BIT          0xFF

//
// Definition for use with RcDebugPrintWithDevice
// Channel parameter.
//

#define HBM_IO_REPLACE  (BIT7)

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
  );

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
  );

/**
  Flush buffered debug output.

**/
VOID
FlushDebugBuffer (
  VOID
  );

/**
  Initializes debug message infrastructure.

  @param VOID

  @retval VOID

**/
VOID
InitializeDebugMessages (
  VOID
  );

/**
  Update Semaphore sync data based on the present sockets.

  @param[in]  Acquire  Pointer to semaphore acquire function.
  @param[in]  Release  Pointer to semaphore release function.

**/
VOID
UpdateSemaphoreSyncData (
  IN PRINT_SYNC_ACQUIRE Acquire,
  IN PRINT_SYNC_RELEASE Release
  );

/**
  Cleans up debug message infrastructure.

**/
VOID
ResetDebugMessages (
  VOID
  );

/**

  Get the debug level.

  @param  None

  @retval Debug level.

**/

UINT32
EFIAPI
GetDebugLevel (
  VOID
  );

/**

  Set the debug level.

  @param[in] DebugLevel - Debug level to set.

  @retval None.

**/

VOID
EFIAPI
SetDebugLevel (
  IN        UINT32          DebugLevel
  );

//---------------------------------------------------------------
// Debug Preproccesor Optimizations
//---------------------------------------------------------------

#define EXPAND(x) x

/**
  Internal worker macro that calls RcDebugPrintInternal().

  This macro calls RcDebugPrintInternal() passing in the debug error level, a format
  string, and a variable argument list.

**/

#define RcDebugPrint(DbgLevel, ...)                     \
  do {                                                  \
    if (CheckDebugPrintLevel (DbgLevel)) {              \
      RcDebugPrintInternal (DbgLevel, __VA_ARGS__);     \
    }                                                   \
  } while (FALSE)

/**
  Internal worker macro that calls RcDebugPrintWithDeviceInternal().

  This macro calls RcDebugPrintWithDeviceInternal() passing in the debug error level, a format
  string, and a variable argument list.

**/

#define RcDebugPrintWithDevice(DbgLevel, ...)                     \
  do {                                                  \
    if (CheckDebugPrintLevel (DbgLevel)) {              \
      RcDebugPrintWithDeviceInternal (DbgLevel, __VA_ARGS__);     \
    }                                                   \
  } while (FALSE)

//---------------------------------------------------------------
// While old interfaces still in use, smooth over with macro glue
//---------------------------------------------------------------

//
// BSSA variant macro glue
//

#define NORMAL_LOG(SsaBiosServices, PrintLevel, ...) GlueRcDebugPrint (SDBG_DEFAULT | SDBG_BSSA, __VA_ARGS__)

//
// MSP variant macro glue
//

#define MspAssert(MspData, ErrStr, Function, Line) \
  { \
    GlueRcDebugPrint (SDBG_DEFAULT, ErrStr); \
    CpuDeadLoop (); \
  }

//
// Mimic existing print shutoffs via from SILENT
//

#if !defined(SILENT_MODE)
#define GlueRcDebugPrint(...) EXPAND(RcDebugPrint (__VA_ARGS__))
#define GlueRcDebugPrintWithDevice(...) EXPAND(RcDebugPrintWithDevice (__VA_ARGS__))
#else
#define GlueRcDebugPrint(...)
#define GlueRcDebugPrintWithDevice(...)
#endif // !defined(SILENT_MODE)

#endif  // _RC_DEBUG_LIB_H_
