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

#include <Library/MemoryCoreLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include "Include/CpgcChip.h"

#define CPGC2_MUX_UNISEQ_0       0xAA
#define CPGC2_MUX_UNISEQ_1       0xCC
#define CLOCK_PATTERN            0x55555555

VOID WdbSetByteGroupMappingChip (IN PSYSHOST Host, IN UINT8 Socket, IN UINT8 Ch, IN UINT16 ByteGroup);
VOID WdbPreLoadChip (IN PSYSHOST Host, IN UINT8 Socket, IN UINT8 Ch, IN UINT8 NumberOfWdbLines, IN UINT8 *Wdb2Lines, IN UINT8 StartWdbDataLineIndex);

/**

  This function checks and converts a pattern to which WDB2 could support.
  WDB2 has only 16 unique virtual lanes which are broken into 2 sets of 8 lanes.

  @param[in]  Pattern         - 32 bit general pattern which needs to be converted in WDB2 form
  @param[out] WordPattern     - Pointer to 16 lanes WDB2 pattern
  @param[out] ByteGroup       - Pointer to byte group which uses for virtual to physical DQ lanes mapping

  @retval SUCCESS             - Pattern is valid for WDB2
  @retval FAILURE             - Pattern is not valid for WDB2

**/
UINT32
ConvertWDB2Pattern (
  IN  UINT32  Pattern,
  OUT UINT16  *WordPattern,
  OUT UINT16  *ByteGroup
  )
{
  UINT8 Byte0, Byte1, Byte2, Byte3;
  UINT8 BytePattern0, BytePattern1;

  Byte0 = (UINT8) (Pattern & 0xFF);
  Byte1 = (UINT8) ((Pattern >> 8) & 0xFF);
  Byte2 = (UINT8) ((Pattern >> 16) & 0xFF);
  Byte3 = (UINT8) ((Pattern >> 24) & 0xFF);

  if (Byte0 == Byte1) {
    BytePattern0 = Byte0;
    if (Byte2 == BytePattern0) {
      BytePattern1 = Byte3;
      *ByteGroup = CPGC2_BYTEGROUP_YXXX;
    } else {
      BytePattern1 = Byte2;
      if (Byte3 == BytePattern0) {
        *ByteGroup = CPGC2_BYTEGROUP_XYXX;
      } else if (Byte3 == BytePattern1) {
        *ByteGroup = CPGC2_BYTEGROUP_YYXX;
      } else {
        RC_FATAL_ERROR (FALSE, ERR_MRC_STRUCT, ERR_ARRAY_OUT_OF_BOUNDS);
        return FAILURE;
      }
    }
  } else {
    BytePattern0 = Byte0;
    BytePattern1 = Byte1;
    if (Byte2 == BytePattern0) {
      if (Byte3 == BytePattern0) {
        *ByteGroup = CPGC2_BYTEGROUP_XXYX;
      }
      else if (Byte3 == BytePattern1) {
        *ByteGroup = CPGC2_BYTEGROUP_YXYX;
      } else {
        RC_FATAL_ERROR (FALSE, ERR_MRC_STRUCT, ERR_ARRAY_OUT_OF_BOUNDS);
        return FAILURE;
      }
    } else if (Byte2 == BytePattern1) {
      if (Byte3 == BytePattern0) {
        *ByteGroup = CPGC2_BYTEGROUP_XYYX;
      }
      else if (Byte3 == BytePattern1) {
        *ByteGroup = CPGC2_BYTEGROUP_YYYX;
      } else {
        RC_FATAL_ERROR (FALSE, ERR_MRC_STRUCT, ERR_ARRAY_OUT_OF_BOUNDS);
        return FAILURE;
      }
    } else {
      RC_FATAL_ERROR (FALSE, ERR_MRC_STRUCT, ERR_ARRAY_OUT_OF_BOUNDS);
      return FAILURE;
    }
  }

  *WordPattern = (BytePattern1 << 8) | BytePattern0;

  return SUCCESS;
}

/**
  Determine whether the cached value in the WDB matches the programmed value.

  @param[in] Socket   Target socket
  @param[in] Channel  Target channel

  @retval TRUE    The cached value in WDB matches the programmed value
  @retval FALSE   The cached value in WDB doesn't match the programmed value
**/
BOOLEAN
IsWdbCacheValid (
  IN  UINT8 Socket,
  IN  UINT8 Channel
  )
{
  return (((PSYSHOST)GetSysHostPointer ())->nvram.mem.socket[Socket].WdbCacheValidPerChannel & (1 << Channel)) != 0;
}

/*

  Function to write a temporal basic pattern for low stress test

  @param[in]  Host        Pointer to SysHost structure
  @param[in]  Socket      Current socket under test (0-based)
  @param[in]  TemporalPattern   Pattern to be written
  @param[in]  NumCachelines     Number of cachelines
  @param[in]  IncludeDeselects  TRUE: Include deselects, FALSE: does not include deselects

*/
VOID
WriteTemporalPattern (
  IN        PSYSHOST  Host,
  IN        UINT8     Socket,
  IN        UINT32    TemporalPattern,
  IN        UINT8     NumCachelines,
  IN        BOOLEAN   IncludeDeselects
  )
{
}

/**
  Set an indicator that the cached value in the WDB matches the programmed value for a target channel.

  @param[in] Socket   Target socket
  @param[in] Channel  Target channel

  @retval N/A
**/
VOID
SetWdbCacheValid (
  IN  UINT8 Socket,
  IN  UINT8 Channel
  )
{
  ((PSYSHOST)GetSysHostPointer ())->nvram.mem.socket[Socket].WdbCacheValidPerChannel |= (1 << Channel);
}

/**
  Clear indicators for all channels signifying that cached values in the WDB match the programmed values.

  @param[in] Socket   Target socket

  @retval N/A
**/
VOID
EFIAPI
ClearWdbCacheValidAllChannels (
  IN  UINT8 Socket
  )
{
  ((PSYSHOST)GetSysHostPointer ())->nvram.mem.socket[Socket].WdbCacheValidPerChannel = 0;
}

/**

  This function fills the WDB buffer

  @param[in] Host                  - Pointer to sysHost
  @param[in] Socket                - Socket number
  @param[in] Ch                    - Channel number
  @param[in] Wdb2Lines             - Array that include data to write to the WDB
  @param[in] NumberOfWdbLines      - The number of WDB data line to write
  @param[in] StartWdbDataLineIndex - Start offset data line number on the WDB.

  @retval N/A

**/
VOID
WDBFill (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN TWdbLine *Wdb2Lines,
  IN UINT8    NumberOfWdbLines,
  IN UINT8    StartWdbDataLineIndex
  )
{
  struct DdrChannelTraining      (*ChannelListTraining)[MAX_CH];
  UINT8                          (*PLine)[MRC_WDB2_LANE_NUM];
  UINT8                          (*PWdb2Line)[MRC_WDB2_LANE_NUM];
  UINT8                          Lane;
  UINT32                         Data32;

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "WDBFill Starts\n");

  ChannelListTraining = GetChannelListTraining ();

  // Make sure we do not try to write more than the WDB can hold
  if (NumberOfWdbLines + StartWdbDataLineIndex > MAX_WDB2_DATA_LINES) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Warning: WDB cacheline is overflow %d in WDBFill. Limit to %d.\n", NumberOfWdbLines, MAX_WDB2_DATA_LINES - StartWdbDataLineIndex);
    NumberOfWdbLines = MAX_WDB2_DATA_LINES - StartWdbDataLineIndex;
  }

  PLine = (UINT8 (*)[MRC_WDB2_LANE_NUM])&(*ChannelListTraining)[Ch].WdbLines[0];
  PWdb2Line = (UINT8 (*)[MRC_WDB2_LANE_NUM])Wdb2Lines;

  // Compare cached values
  if ((CompareMem ((UINT8 *)PLine[StartWdbDataLineIndex], (UINT8 *) PWdb2Line[StartWdbDataLineIndex], MRC_WDB2_LANE_NUM * NumberOfWdbLines))
      || !IsWdbCacheValid (Socket, Ch)) {
    //
    // Fill the WDB
    //
    WdbPreLoadChip (Host, Socket, Ch, NumberOfWdbLines, (VOID *) PWdb2Line, StartWdbDataLineIndex);
    //
    // Update cached values
    //
    CopyMem ((UINT8 *)PLine[StartWdbDataLineIndex], (UINT8 *) PWdb2Line[StartWdbDataLineIndex], MRC_WDB2_LANE_NUM * NumberOfWdbLines);

    SetWdbCacheValid (Socket, Ch);
  } else {
    for (Lane = 0; Lane < MRC_WDB2_LANE_NUM; Lane++) {
      Data32= PWdb2Line[0][Lane] | (PWdb2Line[1][Lane] << 8) | (PWdb2Line[2][Lane] << 16) | (PWdb2Line[3][Lane] << 24);
      RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Lane%02d [0x%08x]\n", Lane, Data32);
    }
  }
}

/**
  This function Writes DQ lane-based pattern to the WDB with/without deselect patterns

  @param[in] Host              - Pointer to sysHost
  @param[in] Socket            - Socket Id
  @param[in] PatternDQ         - Pattern for each DQ lane.
                               - CL0 = PatternDQ[7:0]
                               - CL1 = PatternDQ[15:8]
                               - CL2 = PatternDQ[23:16]
                               - CL3 = PatternDQ[31:24]
  @param[in] NumCachelines     - Number of cacheline to be written. Max number is 4.
  @param[in] IncludeDeselects  - Whether includes deselect pattern or not.

  @retval None

**/
VOID
WriteWDBDqLanePattern (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    PatternDQ,
  IN UINT8     NumCachelines,
  IN UINT8     IncludeDeselects
  )
{
  UINT8               Ch;
  UINT8               Lane, Line;
  UINT8               Wdb2Lines[MAX_WDB2_DATA_LINES][MRC_WDB2_LANE_NUM];
  struct channelNvram (*ChannelNvList)[MAX_CH];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  for (Line = 0; Line < MAX_WDB2_DATA_LINES; Line++) {
    for (Lane = 0; Lane < MRC_WDB2_LANE_NUM; Lane++) {
      Wdb2Lines[Line][Lane] = (PatternDQ >> (Line*8)) & 0xFF;
    }
  }

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    WdbSetByteGroupMappingChip (Host, Socket, Ch, CPGC2_BYTEGROUP_DEFAULT);
    WDBFill (Host, Socket, Ch, (VOID *)Wdb2Lines, NumCachelines * WDB2_LINES_PER_CACHELINE, 0);
  } // Ch loop
} // WriteWDBDqLanePattern

/**
  This function Writes chunk-based pattern (clock pattern) to the WDB

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket Id
  @param[in] PatternChunk    - Initial pattern for the first chunk
  @param[in] NumCachelines   - Number of cacheline to be written

  @retval None

**/
VOID
WriteWDBChunkPattern (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    PatternChunk,
  IN UINT8     NumCachelines
  )
{
  UINT8               Ch;
  UINT8               Lane, Line;
  UINT16              ByteGroup;
  UINT16              WordPattern;
  UINT8               Wdb2Lines[MAX_WDB2_DATA_LINES][MRC_WDB2_LANE_NUM];
  struct channelNvram (*ChannelNvList) [MAX_CH];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ConvertWDB2Pattern (PatternChunk, &WordPattern, &ByteGroup);

  for (Line = 0; Line < MAX_WDB2_DATA_LINES; Line++) {
    for (Lane = 0; Lane < MRC_WDB2_LANE_NUM; Lane++) {
      if ((WordPattern >> Lane) & BIT0) {
        Wdb2Lines[Line][Lane] = CLOCK_PATTERN & 0xFF;
      } else {
        Wdb2Lines[Line][Lane] = (~CLOCK_PATTERN) & 0xFF;
      }
    }
  }

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    WdbSetByteGroupMappingChip (Host, Socket, Ch, ByteGroup);
    WDBFill (Host, Socket, Ch, (VOID *)Wdb2Lines, NumCachelines * WDB2_LINES_PER_CACHELINE, 0);
  } // Ch loop
}

/**

  This function Writes multiple chunk-based patterns to the specific channel's WDB

  @param[in] Host                - Pointer to sysHost
  @param[in] Socket              - Socket Id
  @param[in] Channel             - Channel Id
  @param[in] Pattern             - Pointer to buffer containing chunk-based patterns.
  @param[in] CachelineCount      - Size of the buffer pattern in number of cachelines.  Each cacheline has 8 UINT64 elements.  For example, if CachelineCount is 1 then the Pattern buffer size is 64 bytes.
  @param[in] StartCachelineIndex - Start offset on the cacheline.  The sum of the CachelineCount and StartCachelineIndex values must be less than or equal to MRC_WDB_LINES.

  @retval None

**/
VOID
WriteWDBMultiChunkPattern(
  IN PSYSHOST     Host,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN CONST UINT64 *Pattern,
  IN UINT8        CachelineCount,
  IN UINT8        StartCachelineIndex
  )
{
  // SPR_TODO: Used by BSSA 1.0
  return;
}

/*++

  Sets the patter to be used during test

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Mode    - 0 = MemTest; 1 = MemInit
  @param[in] LoopNum - Loop number for MemTest

  @retval N/A

--*/
VOID
SetPatternBuffers (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT16    Mode,
  IN UINT16    LoopNum
  )
{
  if (Mode == MEMTEST) {
    RC_FATAL_ERROR (FALSE, ERR_MEM_TEST, ERR_MEM_TEST_MINOR_SOFTWARE);
    return;
  } else {
    //
    // MEMINIT
    //
    WriteWDBDqLanePattern (Host, Socket, 0, MAX_WDB2_CACHELINE, FALSE);
  }
} // SetPatternBuffers

/*++

  Sets the pattern to be used during test

  @param[in] Host           - Pointer to sysHost
  @param[in] Socket         - Socket Id
  @param[in] FlipPattern    - 0 = Normal Pattern; 1 = Flip Pattern

  @retval N/A

--*/
VOID
SetPatternBuffersX16Dqs (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  BOOLEAN   FlipPattern
  )
{
  UINT8                     Ch;
  UINT8                     Lane, Line;
  UINT8                     Wdb2Lines[MAX_WDB2_DATA_LINES][MRC_WDB2_LANE_NUM];
  struct channelNvram       (*ChannelNvList)[MAX_CH];
  UINT8                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  for (Lane = 0; Lane < MRC_WDB2_LANE_NUM; Lane++) {
    for (Line = 0; Line < MAX_WDB2_DATA_LINES; Line++) {
      if (FlipPattern) {
        Wdb2Lines[Line][Lane] = 0xFF;
      } else {
        Wdb2Lines[Line][Lane] = 0;
      }
    }
  }

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    WdbSetByteGroupMappingChip (Host, Socket, Ch, CPGC2_BYTEGROUP_DEFAULT);
    WDBFill (Host, Socket, Ch, (VOID *)Wdb2Lines, 1 * WDB2_LINES_PER_CACHELINE, 0);
  } // Ch loop

} // SetPatternBuffersX16Dqs

/**
  This function Writes same fixed chunk-based patterns to the specific channel's WDB
  Before "NumCachelines" of cachelines, zero patterns are loaded
  After "NumCachelines" of cachelines, "NumSpread" of cachelines of PatternChunk patterns are loaded

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket Id
  @param[in] Ch              - Channel Id
  @param[in] PatternChunk    - Pattern for chunks
  @param[in] NumSpread       - Number of cacheline for PatternChunk pattern
  @param[in] NumCachelines   - Number of cacheline for zero pattern

  @retval None

**/
VOID
WriteWDBFixedPattern (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT32    PatternChunk,
  IN UINT8     NumSpread,
  IN UINT8     NumCachelines
  )
{
  //SPR_TODO: Used by PPR previously
  return;
} // WriteWDBFixedPattern

/**
  WDB setup for Advanced Memory test

  @param[in] Host                         - Pointer to sysHost
  @param[in] Socket                       - Memory controller to train
  @param[in] Ch                           - Ddr channel to setup
  @param[in] Mode                         - Type of sequence MT_CPGC_WRITE, MT_CPGC_READ, or MT_CPGC_READ_WRITE
  @param[in] SeqDataInv[MT_MAX_SUBSEQ]    - Enables pattern inversion per subsequence
  @param[in] PatternQW[MT_PATTERN_DEPTH]  - 64-bit wide data pattern to use per UI
  @param[in] IsUseInvtPat                 - Info to indicate whether or not patternQW is inverted by comparing original pattern
  @param[in] UiShl[MT_PATTERN_DEPTH]      - Enables pattern rotation per UI
  @param[in] NumCacheLines                - Number of cachelines to use in WDB
  @param[in] PatternDepth                 - Number of UI to use in patternQW
  @param[in] AddrOffset                   - Number of cachelines to advance pattern in given direction
  @param[in] Direction                    - Sequential address direction MT_ADDR_DIR_UP, MT_ADDR_DIR_DN
  @param[in] ErrInjMask                   - Bitmask of DQ lanes to inject error

  @retval None

**/
VOID
WriteWDBFixedPatternMATS (
  IN PSYSHOST          Host,
  IN UINT8             Socket,
  IN UINT8             Ch,
  IN UINT8             Mode,
  IN UINT8             SeqDataInv[MT_MAX_SUBSEQ],
  IN UINT64_STRUCT     PatternQW[MT_PATTERN_DEPTH],
  IN BOOLEAN           IsUseInvtPat,
  IN UINT8             UiShl[MT_PATTERN_DEPTH],
  IN UINT8             NumCacheLines,
  IN UINT8             PatternDepth,
  IN UINT8             AddrOffset,
  IN UINT8             Direction,
  IN UINT64_STRUCT     ErrInjMask
  )
{
  UINT8             Line;
  UINT8             Lane;
  UINT8             Chunk;
  UINT8             Index;
  UINT8             NumDataLines;
  UINT16            WdbLinesPerChunk[MAX_WDB2_CACHELINE][BURST_LENGTH];
  UINT8             Wdb2Lines[MAX_WDB2_DATA_LINES][MRC_WDB2_LANE_NUM];
  UINT16            Temp16[MT_PATTERN_DEPTH];
  UINT16            ChunkData;
  UINT16            ErrInjMask16;
  UINT32            Temp32;
  UINT32            AddrOffsetShiftRate;

  NumDataLines = NumCacheLines * WDB2_LINES_PER_CACHELINE;

  //
  // Check for error
  //
  if ((PatternDepth > MT_PATTERN_DEPTH) || (NumDataLines > MAX_WDB2_DATA_LINES)) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_48);
  }

  //
  // Cpgc 2.x only supports 16 unique lanes (extbuf[0]~extbuf[15])
  // Only care about lower 16 bits error injection
  //
  ErrInjMask16 = (UINT16) (ErrInjMask.lo & 0xFFFF);

  if ((IsUseInvtPat == TRUE) && (ErrInjMask16 == 0)) {
    //
    // Cpgc 2.x can handle inverted pattern by setupping corresponding algo and cmd instructions instead of updating WDB here
    // If no error injection but just setup inverted pattern, then return.
    //
    return;
  }

  //
  // Use local variable
  //
  for (Index = 0; Index < PatternDepth; Index++) {
    //
    // Cpgc 2.x only supports 16 unique lanes (extbuf[0]~extbuf[15])
    // All other 16-bits-based byte groups have the same lanes setting with it
    // So MATS would only supports 16 unique lanes pattern
    // Take lower 16 bits of PatternQW.lo as MATS pattern
    // Pattern in PatternQW.hi would be ignored
    //
    // For example:
    //             dq31            dq16 dq15              dq0
    // 0x12341234 -  0001 0010 0011 0100 0001 0010 0011 0100
    //
    // dq0~dq15 can be unique, though dq16~dq23 and dq24~dq31 needs to be a subset of dq0~dq7 and dq8~dq15 per CPGC 2.x design
    //
    // For MAST, we always set dq16~dq31 (or dq32~dq63 for longer bus width) same to dq0~dq15
    // This is expected per AMT Spec
    //
    //
    if (IsUseInvtPat == TRUE) {
      //
      // By using Cpgc 2.x, we always setup WDB with original pattern and use algo and cmd instructions to handle pattern inversion
      // and address traverse direction. Here is requesting to inject errors based on inverted pattern on corresponding bits indicated by
      // ErrInjMask16. Since currently what WDB contains are original patterns, we need INVERTED this input "inverted" pattern again which
      // could match with what WDB currently has, then inverted corresponding bits for error injection.
      //
      // For example, PatternQW[Index].lo = FFFE, ErrInjMask16 = 0x000F, wihch means we want to invert bit[3:0] as errors, i.e. 0xFFF1.
      // Now we inverted this "inverted" pattern again to align to original pattern. So Temp16 = 0x0001. Then the final pattern written into WDB
      // would be 000E. when setting correct pattern inversion on algo and cmd instructions, Cpgc would finally generate ~000E, i.e. 0xFFF1.
      //
      Temp16[Index] = (UINT16) (~PatternQW[Index].lo & 0xFFFF);
    } else {
      Temp16[Index] = (UINT16) (PatternQW[Index].lo & 0xFFFF);
    }

    //
    // Handle address offset
    // Cpgc 2.x only allows lanes shift within 16 unique
    //
    AddrOffsetShiftRate = (UiShl[Index] * 8 * AddrOffset);
    if ( AddrOffsetShiftRate > MRC_WDB2_LANE_NUM) {
      //
      // We don't want to break the whole error injection flow but just provide proper "AddrOffset" value to fix it.
      // This "AddrOffset" is expected to be hardcoded with proper value by callers for testing purpose.
      //
      RcDebugPrintWithDevice(SDBG_MINMAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Warning: WriteWDBFixedPatternMATS has AddrOffsetShiftRate %d is greater than %d.\n", AddrOffsetShiftRate, MRC_WDB2_LANE_NUM);
    } else {
      Temp16[Index] = ROTATE_LEFT_UINT16 (Temp16[Index], AddrOffsetShiftRate);
    }
  }

  //
  // For each cacheline (burst of 16 UI for DDR5)
  //
  for (Line = 0; Line < NumCacheLines; Line++) {
    for (Chunk = 0; Chunk < BURST_LENGTH; Chunk++) {

      // Chunk 0 gets pattern[0]
      // Chunk 1 gets pattern[1]
      // ...
      // Chunk N gets pattern[N MOD MT_PATTERN_DEPTH]

      // program 16-bit UI of line
      Index = Chunk % PatternDepth;

      WdbLinesPerChunk[Line][Chunk] = ((Temp16[Index] & ~ErrInjMask16) | (~Temp16[Index] & ErrInjMask16));

      // Rotate pattern left for each UI
      Temp16[Index] = ROTATE_LEFT_UINT16 (Temp16[Index], UiShl[Index]);

    } // i loop
  } // Line loop

  //
  // Print debug info... patterns expected on the bus
  //
  // subsequence 0
  if (Mode == MT_CPGC_WRITE) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,"Subseq0 Write pattern:\n");
  } else {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,"Subseq0 Read pattern:\n");
  }
  // For each cacheline (burst of 16 UI)
  for (Line = 0; Line < NumCacheLines; Line++) {
    for (Chunk = 0; Chunk < BURST_LENGTH; Chunk++) {

      ChunkData = WdbLinesPerChunk[Line][Chunk];

      if (SeqDataInv[0]) {
        Temp32 = (~ChunkData & 0xFFFF) | (~ChunkData & 0xFFFF) << 16;
      } else {
        Temp32 = ChunkData | ChunkData << 16;
      }

      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                        "CL%2d UI%2d 0x%08x\n", Line, Chunk, Temp32);
    } // Chunk loop
  } // Line loop

  // subsequence 1
  if (Mode == MT_CPGC_READ_WRITE) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,"Subseq1 Write pattern:\n");

    // For each cacheline (burst of 16 UI)
    for (Line = 0; Line < NumCacheLines; Line++) {
      for (Chunk = 0; Chunk < BURST_LENGTH; Chunk++) {
        ChunkData = WdbLinesPerChunk[Line][Chunk];

        if (SeqDataInv[1]) {
          Temp32 = (~ChunkData & 0xFFFF) | (~ChunkData & 0xFFFF) << 16;
        } else {
          Temp32 = ChunkData | ChunkData << 16;
        }

        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                        "CL%2d UI%2d 0x%08x\n", Line, Chunk, Temp32);
      } // Chunk loop
    } // Line loop
  }

  //
  // Convert WdbLinesPerChunk to Wdb2Lines which are per lane based strucuture for CPGC 2.x programming
  //
  SetMem (Wdb2Lines, sizeof (Wdb2Lines), 0);

  for (Line = 0; Line < NumCacheLines; Line++) {
    for (Chunk = 0; Chunk < BURST_LENGTH; Chunk++) {
      //
      // 16 bit chunk data for 16 lanes
      //
      ChunkData = WdbLinesPerChunk[Line][Chunk];

      if (Chunk < MRC_WDB2_DATA_LINE_WIDTH) {
        for (Lane = 0; Lane < MRC_WDB2_LANE_NUM; Lane++) {
          Wdb2Lines[Line * WDB2_LINES_PER_CACHELINE][Lane] |= ((ChunkData >> Lane) & BIT0) << Chunk;
        }
      } else {
        for (Lane = 0; Lane < MRC_WDB2_LANE_NUM; Lane++) {
          Wdb2Lines[Line * WDB2_LINES_PER_CACHELINE + 1][Lane] |= ((ChunkData >> Lane) & BIT0) << (Chunk - MRC_WDB2_DATA_LINE_WIDTH);
        }
      }
    }
  }

  WdbSetByteGroupMappingChip (Host, Socket, Ch, CPGC2_BYTEGROUP_DEFAULT);
  WDBFill (Host, Socket, Ch, (VOID *) Wdb2Lines, NumDataLines, 0);

  return;
} // WriteWDBFixedPatternMATS

/**
  This function assigns CPGC uniseq1's pattern for victim DQ lanes and uniseq0's pattern for aggressor DQ lanes
  in WDB according to the given pattern length

  @param[in] Host              - Pointer to sysHost
  @param[in] Socket            - Socket Id
  @param[in] ChbitMask         - Ch Bit mask for which test should be setup for.
  @param[in] PatternWidth      - Specify how many bits/lanes of a victim-aggressor pair contains, support 2, 4, 8 and 16

  @retval SUCCESS              - Assgin Victim-Aggressor patterns successfully
          FAILURE              - Failed to assgin Victim-Aggressor patterns

**/
UINT32
WriteWDBDqVaPattern (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChbitMask,
  IN UINT32    PatternWidth
  )
{
  UINT8               Ch;
  UINT8               Lane, Line;
  UINT8               Wdb2Lines[MAX_WDB2_DATA_LINES][MRC_WDB2_LANE_NUM];
  struct channelNvram (*ChannelNvList)[MAX_CH];
  UINT8               MaxChDdr;

  if ((PatternWidth < 2) || ((MRC_WDB2_LANE_NUM % PatternWidth) != 0)) {
    return FAILURE;
  }
  for (Line = 0; Line < MAX_WDB2_DATA_LINES; Line++) {
    for (Lane = 0; Lane < MRC_WDB2_LANE_NUM; Lane++) {
      if ((GetCurrentTestType (Socket) == ReadDfeTest)) {
        if ((Lane % BITS_PER_NIBBLE) == Host->var.mem.socket[Socket].TrainingStatus.Bits.BitIndex) {
          Wdb2Lines[Line][Lane] = CPGC2_MUX_UNISEQ_0;//seed0
        } else {
          Wdb2Lines[Line][Lane] = CPGC2_MUX_UNISEQ_1;//seed1
        }
      } else {
        if (Lane % PatternWidth) {
          Wdb2Lines[Line][Lane] = CPGC2_MUX_UNISEQ_0; // assign uniseq 0 for aggressor lanes
        } else {
          Wdb2Lines[Line][Lane] = CPGC2_MUX_UNISEQ_1; // assign uniseq 1 for victim lanes
        }
      }
    }
  }

  ChannelNvList = GetChannelNvList (Host, Socket);
  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((IsChannelEnabled (Socket, Ch) == FALSE) ||
        ((ChbitMask & (1 << Ch)) == 0)) {
      continue;
    }
    WdbSetByteGroupMappingChip (Host, Socket, Ch, CPGC2_BYTEGROUP_DEFAULT);
    WDBFill (Host, Socket, Ch, (VOID *)Wdb2Lines, MAX_WDB2_DATA_LINES, 0);
  } // Ch loop

  return SUCCESS;
}