/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2019 Intel Corporation. <BR>

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
#include "CpgcChip.h"
#include <Library/MemMcIpLib.h>
#include <Library/MemRoutingLib.h>
#include <Library/MemRcLib.h>
#include <Library/MemRoutingLib.h>

VOID WdbPreLoadChip (IN PSYSHOST Host, IN UINT8 Socket, IN UINT8 Ch, IN UINT8 NumberOfWdbLines, IN UINT8 *WdbLines, IN UINT8 StartCachelineIndex);

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
  UINT8               Ch;
  UINT8               Bit;
  UINT8               Line;
  UINT8               Chunk;
  UINT8               MaxChunk;
  UINT8               MaxBits;
  UINT32              Pattern = 0;
  UINT32              *WdbLinePtr;
  TWdbLine            WdbLines[32];
  struct channelNvram(*channelNvList)[MAX_CH];
  UINT8               TotalCacheLines;
  //  UINT8               SubCh = 0;
  //UINT8               ChannelLocal = 0;

  MaxBits = NumCachelines * 8;

  if (IncludeDeselects == TRUE) {
    TotalCacheLines = NumCachelines + 4;
  } else {
    TotalCacheLines = NumCachelines;
  }

  //
  //Initialize the WDB buffer to all 0's
  //
  for (Line = 0; Line < 32; Line++) {
    ZeroMem(WdbLines[Line].WdbLine, MRC_WDB_LINE_SIZE);
  }

  //
  // Fill the WDB buffer with the write content.
  //
  for (Bit = 0; Bit < MaxBits; Bit++) {

    if (TemporalPattern & (1 << Bit)) {
      Pattern = 0xFFFFFFFF;
    } else {
      Pattern = 0;
    }

    //
    // Get the cacheline this bit belongs to
    //
    Line = Bit / 8;
    if (IncludeDeselects == TRUE) {
      Line = Line + 4;
    }

    //
    // Get the chunk in the current cacheline this bit belongs to
    //
    Chunk = (Bit * 2) % 16;
    MaxChunk = Chunk + 2;

    WdbLinePtr = (UINT32 *)&WdbLines[Line].WdbLine[0];

    for (; Chunk < MaxChunk; Chunk++) {
      if (Chunk >= MRC_WDB_LINE_SIZE / 4) continue;
      WdbLinePtr[Chunk] = Pattern;
    } // chunk loop
  } // bit loop

  channelNvList = GetChannelNvList(Host, Socket);

  for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
    if ((*channelNvList)[Ch].enabled == 0) {
      continue;
    }

    WDBFill (Host, Socket, Ch, WdbLines, TotalCacheLines, 0);

  } // Ch loop

} // WriteTemporalPattern

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

  @param[in] Host                - Pointer to sysHost
  @param[in] Socket              - Socket number
  @param[in] Ch                  - Channel number
  @param[in] WdbLines            - Array that include data to write to the WDB
  @param[in] NumberOfWdbLines    - The number of cachelines to write
  @param[in] StartCachelineIndex - Start offset/cacheline number on the WDB.

  @retval N/A

**/
VOID
WDBFillBrs (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN TWdbLine *WdbLines,
  IN UINT8    NumberOfWdbLines,
  IN UINT8    StartCachelineIndex
  )
{
  UINT8                                 ChIndex = 0;
  struct DdrChannelTraining            (*ChannelListTraining)[MAX_CH];

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "WDBFill Starts\n");

  ChIndex = Ch;
  ChannelListTraining = GetChannelListTraining ();

  // Make sure we do not try to write more than the WDB can hold
  if (NumberOfWdbLines > MRC_WDB_LINES) {
    NumberOfWdbLines = MRC_WDB_LINES;
  }
    //
    // Fill the WDB
    //
    WdbPreLoadChip (Host, Socket, Ch, NumberOfWdbLines, (VOID *) WdbLines, StartCachelineIndex);
    //
    // Update cached values
    //
    CopyMem ((UINT8 *)&(*ChannelListTraining)[ChIndex].WdbLines[StartCachelineIndex], (UINT8 *) WdbLines, sizeof (TWdbLine) * NumberOfWdbLines);
    SetWdbCacheValid (Socket, ChIndex);
}
/**

  This function fills the WDB buffer

  @param[in] Host                - Pointer to sysHost
  @param[in] Socket              - Socket number
  @param[in] Ch                  - Channel number
  @param[in] WdbLines            - Array that include data to write to the WDB
  @param[in] NumberOfWdbLines    - The number of cachelines to write
  @param[in] StartCachelineIndex - Start offset/cacheline number on the WDB.

  @retval N/A

**/
VOID
WDBFill (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN TWdbLine *WdbLines,
  IN UINT8    NumberOfWdbLines,
  IN UINT8    StartCachelineIndex
  )
{
  UINT8                                 ChIndex = 0;
  struct DdrChannelTraining            (*ChannelListTraining)[MAX_CH];

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "WDBFill Starts\n");

  ChIndex = Ch;
#ifdef DDR5_SUPPORT
  if (IsBrsPresent (Host, Socket)) {
    WDBFillBrs (Host, Socket, Ch, WdbLines, NumberOfWdbLines, StartCachelineIndex);
    return;
  }
#endif //DDR5_SUPPORT
  ChannelListTraining = GetChannelListTraining ();

  // Make sure we do not try to write more than the WDB can hold
  if (NumberOfWdbLines > MRC_WDB_LINES) {
    NumberOfWdbLines = MRC_WDB_LINES;
  }

  // Compare cached values
  if ((CompareMem ((UINT8 *)&(*ChannelListTraining)[ChIndex].WdbLines[StartCachelineIndex], (UINT8 *) WdbLines, sizeof (TWdbLine) * NumberOfWdbLines))
      || !IsWdbCacheValid (Socket, ChIndex)) {
    //
    // Fill the WDB
    //
    WdbPreLoadChip (Host, Socket, Ch, NumberOfWdbLines, (VOID *) WdbLines, StartCachelineIndex);
    //
    // Update cached values
    //
    CopyMem ((UINT8 *)&(*ChannelListTraining)[ChIndex].WdbLines[StartCachelineIndex], (UINT8 *) WdbLines, sizeof (TWdbLine) * NumberOfWdbLines);

    SetWdbCacheValid (Socket, ChIndex);
  }
}

/**
  This function Writes DQ lane-based pattern to the WDB with/without deselect patterns

  @param[in] Host              - Pointer to sysHost
  @param[in] Socket            - Socket Id
  @param[in] PatternDQ         - Pattern for each DQ lane
  @param[in] NumCachelines     - Number of cacheline to be written
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
  UINT8               Bit;
  UINT8               Line;
  UINT8               Chunk;
  UINT8               MaxChunk;
  UINT8               MaxBits;
  UINT32              Pattern = 0;
  UINT32              *WdbLinePtr;
  TWdbLine            WdbLines[MRC_WDB_LINES];
  struct channelNvram (*ChannelNvList)[MAX_CH];
  UINT8               TotalCacheLines;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  MaxBits = NumCachelines * 8;

  if (IncludeDeselects == TRUE) {
    TotalCacheLines = NumCachelines + 4;
  } else {
    TotalCacheLines = NumCachelines;
  }

  //
  //Initialize the WDB buffer to all 0's
  //
  for (Line = 0; Line < 32; Line++) {
    ZeroMem (WdbLines[Line].WdbLine, MRC_WDB_LINE_SIZE);
  }

  //
  // Fill the WDB buffer with the write content.
  //
  for (Bit = 0; Bit < MaxBits; Bit++) {

    if (PatternDQ & (1 << Bit)) {
      Pattern = 0xFFFFFFFF;
    } else {
      Pattern = 0;
    }

    //
    // Get the cacheline this bit belongs to
    //
    Line = Bit / 8;
    if (IncludeDeselects == TRUE) {
      Line = Line + 4;
    }

    //
    // Get the chunk in the current cacheline this bit belongs to
    //
    Chunk = (Bit * 2) % 16;
    MaxChunk = Chunk + 2;

    WdbLinePtr = (UINT32 *) &WdbLines[Line].WdbLine[0];
    //RcDebugPrint (SDBG_MAX,
    //              "bit %d, line %d, chunk %d = 0x%x\n", bit, line, chunk, pattern);
    for (; Chunk < MaxChunk; Chunk++) {
      if (Chunk >= MRC_WDB_LINE_SIZE / 4) {
        continue;
      }
      WdbLinePtr[Chunk] = Pattern;
    } // chunk loop
  } // bit loop

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    WDBFill (Host, Socket, Ch, WdbLines, TotalCacheLines, 0);
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
  UINT8               Bit;
  UINT8               Line;
  UINT8               Chunk;
  UINT8               MaxChunk;
  UINT8               MaxBits;
  UINT32              Pattern = 0;
  UINT32              *WdbLinePtr;
  TWdbLine            WdbLines[MRC_WDB_LINES];
  struct channelNvram (*ChannelNvList) [MAX_CH];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  MaxBits = NumCachelines * 8;

  //
  // Fill the WDB buffer with the write content.
  //
  for (Bit = 0; Bit < MaxBits; Bit++) {
    //
    // DDR5_TODO. Confirm the below pattern below.
    //
    if (Bit & 0x1) {
      Pattern = (~PatternChunk);
    } else {
      Pattern = PatternChunk;
    }

    //
    // Get the cacheline this bit belongs to
    //
    Line = Bit / 8;

    //
    // Get the byte pair (they call it chunk, but it's not). There are 32 byte pairs
    //
    Chunk = (Bit * 2) % 16;
    MaxChunk = Chunk + 2;

    WdbLinePtr = (UINT32 *) &WdbLines[Line].WdbLine[0];
    for (; Chunk < MaxChunk; Chunk++) {
      WdbLinePtr[Chunk] = Pattern; // We need to iterate through all 64 of these
    } // chunk loop
  } // Bit loop

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    WDBFill (Host, Socket, Ch, WdbLines, NumCachelines, 0);
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
WriteWDBMultiChunkPattern (
  IN PSYSHOST     Host,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN CONST UINT64 *Pattern,
  IN UINT8        CachelineCount,
  IN UINT8        StartCachelineIndex
  )
{
  UINT8          ChunkIdx;  // Representing the 8chunks (64bits) for each cacheline entered via "*Pattern"
  UINT8          CacheLine;
  UINT32         *WdbLinePtr;
  TWdbLine       WdbLines[MRC_WDB_LINES];
  UINT64_STRUCT  PatternToWrite;
  UINT8          ChunkCntPerCacheline = MRC_WDB_LINE_SIZE / (sizeof (UINT32) * 2); //8

  if ((CachelineCount + StartCachelineIndex) > MRC_WDB_LINES) {
    return;
  }

  ClearWdbCacheValidAllChannels (Socket);

  PatternToWrite.lo = 0;
  PatternToWrite.hi = 0;

  // Fill the WDB buffer with the write content.
  for (CacheLine = 0; CacheLine < CachelineCount; CacheLine++) {
    WdbLinePtr = (UINT32 *) &WdbLines[CacheLine].WdbLine[0];  //Address of base of the array WdbLines saved to WdbLinePrt

    for(ChunkIdx = 0; ChunkIdx < ChunkCntPerCacheline; ChunkIdx++) { // will make 8 iterations(512bits) as in each iteration we copy 2 x 4bytes(64bits) of the *pattern into the array WdbLine
      PatternToWrite.lo = (UINT32)(Pattern[(CacheLine*ChunkCntPerCacheline) + ChunkIdx] & 0xFFFFFFFF);  //64bit I/P "Pattern" --> x8 chunks
      PatternToWrite.hi = (UINT32)RShiftU64(Pattern[(CacheLine*ChunkCntPerCacheline) + ChunkIdx], 32);  //64bit I/P "Pattern" --> x8 chunk
      WdbLinePtr[ChunkIdx * 2] = PatternToWrite.lo;  //representing the array location (16, 32bit chunks) where each "*Pattern" (.lo/.hi) gets saved.. so *2
      WdbLinePtr[(ChunkIdx * 2) + 1] = PatternToWrite.hi;
    }
  } // CacheLine loop

  //64bytes - We have an array(WdbLine) of size 8bits*64 ; We need an array of 32bits*16, Input(for each Pattern) 8arrays of 64bits
  //Call WDBFill to save the pattern in WdbLines into the WDB
  WDBFill (Host, Socket, Channel, WdbLines, CachelineCount, StartCachelineIndex);
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
  UINT8                     Ch;
  UINT8                     RotateBit;
  UINT16                    Index;
  UINT16                    LfsrNum;
  UINT32                    LfsrSeed;
  UINT32                    TempSeed;
  UINT32                    Pattern;
  UINT32                    *WdbLinePrt;
  TWdbLine                  WdbLines[MRC_WDB_LINES];
  struct channelNvram       (*ChannelNvList)[MAX_CH];
  UINT8                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  LfsrSeed = 0x73DA65;

  if (Mode == MEMTEST) {
    //
    // Determine LFSR seed
    //
    LfsrNum = LoopNum >> 1;
    TempSeed = LfsrSeed;
    if (LfsrNum) {
      //
      // Rotate the LFSR seed for short term effect
      //
      for (Index = 0; Index < (LfsrNum % 23); Index++) {
        RotateBit = (UINT8) (TempSeed & BIT0);
        TempSeed >>= 1;
        if (RotateBit) {
          TempSeed |= BIT22;
        }
      }
      //
      // Add unique number for long term effect
      //
      TempSeed += LfsrNum / 23;
    }

    // Pattern to use
    Pattern = TempSeed;

  } else {
    //
    // MEMINIT
    //
    Pattern = 0;
  }

  //
  // Fill the WDB buffer with the write content.
  //
  WdbLinePrt = (UINT32 *) &WdbLines[0].WdbLine[0];
  for (Index = 0; Index < (MRC_WDB_LINE_SIZE / sizeof (UINT32)); Index++) {
    WdbLinePrt[Index] = Pattern;
  }

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    WDBFill (Host, Socket, Ch, WdbLines, 1, 0);
  } // ch loop
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
  UINT16                    Index;
  UINT32                    Pattern = 0;
  UINT32                    *WdbLinePrt;
  TWdbLine                  WdbLines[MRC_WDB_LINES];
  struct channelNvram       (*ChannelNvList)[MAX_CH];
  UINT8                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  if (FlipPattern) {
    Pattern = 0xffffffff;
  }

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    //
    // Fill the WDB buffer with the write content.
    //
    WdbLinePrt = (UINT32 *) &WdbLines[0].WdbLine[0];
    for (Index = 0; Index < (MRC_WDB_LINE_SIZE / sizeof (UINT32)); Index++) {
      WdbLinePrt[Index] = Pattern;
    }

    WDBFill (Host, Socket, Ch, WdbLines, 1, 0);

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
  UINT8     Line;
  UINT8     Index;
  UINT32    *WdbLinePtr;
  TWdbLine  WdbLines[MRC_WDB_LINES];

  //
  // Fill the WDB buffer with the write content.
  //
  for (Line = 0; Line < NumCachelines + NumSpread; Line++) {
    WdbLinePtr = (UINT32 *) &WdbLines[Line].WdbLine[0];
    for (Index = 0; Index < (MRC_WDB_LINE_SIZE / sizeof (UINT32)); Index++) {
      if (Line < NumCachelines) {
        WdbLinePtr[Index] = 0;
      } else {
        WdbLinePtr[Index] = PatternChunk;
      }
    } // i loop
  } // Line loop

  WDBFill (Host, Socket, Ch, WdbLines, NumCachelines + NumSpread, 0);

} // WriteWDBFixedPattern

/**
  WDB setup for Advanced Memory test

  @param[in] Host                         - Pointer to sysHost
  @param[in] Socket                       - Memory controller to train
  @param[in] Ch                           - Ddr channel to setup
  @param[in] Mode                         - Type of sequence MT_CPGC_WRITE, MT_CPGC_READ, or MT_CPGC_READ_WRITE
  @param[in] SeqDataInv[MT_MAX_SUBSEQ]    - Enables pattern inversion per subsequence
  @param[in] PatternQW[MT_PATTERN_DEPTH]  - 64-bit wide data pattern to use per UI
  @param[in] IsUseInvtPat                 - Info to indicate whether or not patternQW is inverted by comparing original pattern. Not used.
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
  UINT8             i;
  UINT8             Index;
  UINT8             Temp;
  UINT32            *WdbLinePtr;
  TWdbLine          WdbLines[MRC_WDB_LINES];
  UINT64_STRUCT     TempQW[MT_PATTERN_DEPTH];
  UINT32            Templo, Temphi;

  // Check for error
  if ((PatternDepth > MT_PATTERN_DEPTH) || (NumCacheLines > MRC_WDB_LINES)) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_48);
  }
  // Use local variable
  for (Index = 0; Index < MT_PATTERN_DEPTH; Index++) {
    TempQW[Index].lo = PatternQW[Index].lo;
    TempQW[Index].hi = PatternQW[Index].hi;

    if (Direction == MT_ADDR_DIR_DN) {
      // Handle down direction
      TempQW[Index] = RotateLeftUINT64 (TempQW[Index], UiShl[Index] * 8 * (NumCacheLines - 1));

      // Handle address offset
      TempQW[Index] = RotateRightUINT64 (TempQW[Index], UiShl[Index] * 8 * AddrOffset);

    } else {
      // Handle address offset
      TempQW[Index] = RotateLeftUINT64 (TempQW[Index], UiShl[Index] * 8 * AddrOffset);
    }
  }


  // For each cacheline (burst of 8 UI)
  for (Line = 0; Line < NumCacheLines; Line++) {

    WdbLinePtr = (UINT32 *) &WdbLines[Line].WdbLine[0];
    for (i = 0; i < (MRC_WDB_LINE_SIZE / sizeof (UINT32)); i++) {

      // Chunk 0 gets pattern[0]
      // Chunk 1 gets pattern[1]
      // ...
      // Chunk N gets pattern[N MOD MT_PATTERN_DEPTH]

      // program 64-bit UI of line (8UI per cache line)
      Index = (i / 2) % PatternDepth;

      // Handle down direction
      if (Line && (Direction == MT_ADDR_DIR_DN)) {
        for (Temp = 0; Temp < PatternDepth; Temp++) {
          if (i == Temp * 2) {
            TempQW[Index] = RotateRightUINT64 (TempQW[Index], UiShl[Index] * 16);
            break;
          }
        }
      }
      if (i % 2 == 1) {
        // Most significant 32 bits of UI
        WdbLinePtr[i] = ((TempQW[Index].hi & ~ErrInjMask.hi) | (~TempQW[Index].hi & ErrInjMask.hi));
      } else {
        // Least significant 32 bits of UI
        WdbLinePtr[i] = ((TempQW[Index].lo & ~ErrInjMask.lo) | (~TempQW[Index].lo & ErrInjMask.lo));
      }
      // Rotate pattern left for each UI
      if (i % 2 == 1) {
        TempQW[Index] = RotateLeftUINT64 (TempQW[Index], UiShl[Index]);
      }
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
  // For each cacheline (burst of 8 UI)
  for (Line = 0; Line < NumCacheLines; Line++) {
    WdbLinePtr = (UINT32 *) &WdbLines[Line].WdbLine[0];
    for (i = 0; i < (MRC_WDB_LINE_SIZE / sizeof (UINT32)); i++) {
      if ((i & 1) && (i > 0)) {
        if (SeqDataInv[0]) {
          Temphi = ~WdbLinePtr[i];
          Templo = ~WdbLinePtr[i-1];
        } else {
          Temphi = WdbLinePtr[i];
          Templo = WdbLinePtr[i-1];
        }
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                        "CL%2d UI%d 0x%08x%08x\n", Line, i/2, Temphi, Templo);
      }
    } // i loop
  } // Line loop

  // subsequence 1
  if (Mode == MT_CPGC_READ_WRITE) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,"Subseq1 Write pattern:\n");

    // For each cacheline (burst of 8 UI)
    for (Line = 0; Line < NumCacheLines; Line++) {
      WdbLinePtr = (UINT32 *) &WdbLines[Line].WdbLine[0];
      for (i = 0; i < (MRC_WDB_LINE_SIZE / sizeof (UINT32)); i++) {
        if ((i & 1) && (i > 0)) {
          if (SeqDataInv[1]) {
            Temphi = ~WdbLinePtr[i];
            Templo = ~WdbLinePtr[i-1];
          } else {
            Temphi = WdbLinePtr[i];
            Templo = WdbLinePtr[i-1];
          }
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "CL%2d UI%d 0x%08x%08x\n", Line, i/2, Temphi, Templo);
        }
      } // i loop
    } // Line loop
  }

  //
  // Fill the WDB buffer with the write content.
  //
  WDBFill (Host, Socket, Ch, WdbLines, NumCacheLines, 0);

} // WriteWDBFixedPatternMATS
