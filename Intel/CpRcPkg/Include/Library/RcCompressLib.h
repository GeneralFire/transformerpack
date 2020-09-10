/** @file
  Interface header file for the Compress library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 Intel Corporation. <BR>

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

#ifndef _COMPRESS_LIB_H_
#define _COMPRESS_LIB_H_

#include <SysHost.h>

#define BITBUFSIZ 32
#define MAXMATCH  256
#define THRESHOLD 3
#define CODE_BIT  16
#define BAD_TABLE - 1

//
// C: Char&Len Set; P: Position Set; T: exTra Set
//
#define NC      (0xff + MAXMATCH + 2 - THRESHOLD)
#define CBIT    9
#define MAXPBIT 5
#define TBIT    5
#define MAXNP   ((1U << MAXPBIT) - 1)
#define _NT      (CODE_BIT + 3)
#if _NT > MAXNP
#define NPT _NT
#else
#define NPT MAXNP
#endif
//
// C: the Char&Len Set; P: the Position Set; T: the exTra Set
//
#define CBIT              9
#define NP                (WNDBIT + 1)
// PBIT is Version dependent. Version 1 - PBIT = 4, Version 2 - PBIT = 5
//#define PBIT              4
#define NT                (CODE_BIT + 3)
#define TBIT              5

//
// Macro Definitions
//
#define SHELL_FREE_NON_NULL(x)
typedef INT16             NODE;
#define UINT8_BIT         8
#define THRESHOLD         3
#define INIT_CRC          0
#define WNDBIT            10
#define WNDSIZ            (1U << WNDBIT)
#define MAXMATCH          256
#define BLKSIZ            (1U << 11)  // 11 * 1024U
#define PERC_FLAG         0x8000U
#define CODE_BIT          16
#define NIL               0
#define MAX_HASH_VAL      (3 * WNDSIZ + (WNDSIZ / 512 + 1) * MAX_UINT8)
#define HASH(LoopVar7, LoopVar5)        ((LoopVar7) + ((LoopVar5) << (WNDBIT - 9)) + WNDSIZ * 2)
#define CRCPOLY           0xA001
#define UPDATE_CRC(LoopVar5)     Globals->mCrc = Globals->mCrcTable[(Globals->mCrc ^ (LoopVar5)) & 0xFF] ^ (Globals->mCrc >> UINT8_BIT)

#define  INSUFFICIENT_SCRATCH_SIZE     1
#define  SRC_SIZE                      2
#define  SRC_LESS_COMP                 3
#define  BUFFER_SIZE_MISMATCH          4
#define  UNKNOWN_VERSION               5
#define  BAD_TABLE_FLAG                6

typedef struct Scratch_Data {
  UINT8  *mSrcBase;  ///< Starting address of compressed data
  UINT8  *mDstBase;  ///< Starting address of decompressed data
  UINT32 mOutBuf;
  UINT32 mInBuf;
  UINT8  *mSrcUpperLimit;
  UINT8  *mDstUpperLimit;

  UINT8  mLevel[WNDSIZ + MAX_UINT8 + 1];
  UINT8  mText[WNDSIZ * 2 + MAXMATCH];
  UINT8  mChildCount[WNDSIZ + MAX_UINT8 + 1];
  UINT8  mBuf[BLKSIZ];
  UINT8  mCLen[NC];
  UINT8  mPTLen[NPT];
  UINT8  *mLen;
  INT16  mHeap[NC + 1];
  INT32  mRemainder;
  INT32  mMatchLen;
  INT32  mBitCount;
  INT32  mHeapSize;
  INT32  mTempInt32;
  UINT32 mBufSiz;
  UINT32 mOutputPos;
  UINT32 mOutputMask;
  UINT32 mBitBuf;
  UINT32 mSubBitBuf;
  UINT32 mCrc;
  UINT32 mCompSize;
  UINT32 mOrigSize;
  UINT32 CPos;

  UINT16 mBlockSize;
  UINT16 mBadTableFlag;
  UINT16 *mFreq;
  UINT16 *mSortPtr;
  UINT16 mLenCnt[17];
  UINT16 mLeft[2 * NC - 1];
  UINT16 mRight[2 * NC - 1];
  UINT16 mCrcTable[MAX_UINT8 + 1];
  UINT16 mCFreq[2 * NC - 1];
  UINT16 mCCode[NC];
  UINT16 mPFreq[2 * NP - 1];
  UINT16 mPTCode[NPT];
  UINT16 mTFreq[2 * NT - 1];

  NODE   mPos;
  NODE   mMatchPos;
  NODE   mAvail;
  NODE   mPosition[WNDSIZ + MAX_UINT8 + 1];
  NODE   mParent[WNDSIZ * 2];
  NODE   mPrev[WNDSIZ * 2];
  NODE   mNext[MAX_HASH_VAL + 1];
  INT32  mHuffmanDepth;
  UINT16 mCTable[4096];
  UINT16 mPTTable[256];
  UINT8  mPBit;          ///< The length of the field 'Position Set Code Length Array Size' in Block Header.\n
                         ///< For EFI 1.1 de/compression algorithm, mPBit = 4 \n
                         ///< For Tiano de/compression algorithm, mPBit = 5

} SCRATCH_DATA, *PSCRATCH_DATA;

#include <Uefi.h>

/**
  The compression routine RC wrapper.

  @param[in]       SrcBuffer     The buffer containing the source data.
  @param[in]       SrcSize       The number of bytes in SrcBuffer.
  @param[in]       DstBuffer     The buffer to put the compressed image in.
  @param[in, out]  DstSize       On input the size (in bytes) of DstBuffer, on
                                return the number of bytes placed in DstBuffer.

  @retval EFI_SUCCESS           The compression was sucessful.
  @retval EFI_BUFFER_TOO_SMALL  The buffer was too small.  DstSize is required.
**/

UINT32
CompressRc (
  IN       VOID     *SrcBuffer,
  IN       UINT32   SrcSize,
  IN       VOID     *DstBuffer,
  IN OUT   UINT32   *DstSize
  );

/*++

  The internal implementation of *_DECOMPRESS_PROTOCOL.Decompress().

  @param[in] Source      - The source buffer containing the compressed data.
  @param[in] SrcSize     - The size of source buffer
  @param[out] Destination - The destination buffer to store the decompressed data
  @param[in] DstSize     - The size of destination buffer.
  @param[in] Scratch     - The buffer used internally by the decompress routine. This  buffer is needed to store intermediate data.
  @param[in] ScratchSize - The size of scratch buffer.
  @param[in] Version     - The version of de/compression algorithm.
                           Version 1 for EFI 1.1 de/compression algorithm.
                           Version 2 for Tiano de/compression algorithm.

  @retval 0 - Success
  @retval Other - Failure error code

--*/

UINT32
Decompress (
  IN VOID    *Source,
  IN UINT32  SrcSize,
  OUT VOID   *Destination,
  IN UINT32  DstSize,
  IN VOID    *Scratch,
  IN UINT32  ScratchSize,
  IN UINT8   Version
  );

/*++

  Decompress code at src to dest

  @param[in]  Src   - Pointer to the compressed code
  @param[out] Dest  - Pointer to place the uncompressed code

  @retval 0 - Success
  @retval Other - Failure error code

--*/

UINT32
DecompressRc (
  IN UINT8    *Src,
  OUT UINT8    *Dest
  );


#endif  // _COMPRESS_LIB_H_
