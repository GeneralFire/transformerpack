/** @file
  CreateMaxResultBuffer.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2018 Intel Corporation. <BR>

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

#include "ChkRetStat.h"
#include <Library/RcMemAllocLib.h>
#include "CreateMaxResultBuffer.h"

// flag to enable BIOS-SSA debug messages
#define ENBL_BIOS_SSA_API_DEBUG_MSGS (0)

// flag to enable function trace debug messages
#define ENBL_FUNCTION_TRACE_DEBUG_MSGS (0)

// flag to enable memory pool debug log messages
#define ENBL_MEM_POOL_DEBUG_LOG_MSGS (0)

/**
  Function used to find the maximum result element count by adding increasing
  2n values to the given base result element count.

  @param[in, out]  SsaServicesHandle   Pointer to SSA services.
  @param[in]       MetadataSize        Size of metadata structure.
  @param[in]       ResultElementSize   Size of columnar result element structure.
  @param[in]       BaseResultEleCount  Base result element count.

  @retval  Maximum result element count.
**/
static UINT32 FindMaxResultEleCount(
   IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
   IN UINT32 MetadataSize,
   IN UINT32 ResultElementSize,
   IN UINT32 BaseResultEleCount)
{
  SSA_STATUS RetStat;
  UINT32 ResultEleCount = 1;
  UINT32 MaxResultEleCount = BaseResultEleCount;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "FindMaxResultEleCount(MetadataSize=%u ResultElementSize=%u BaseResultEleCount=%u)\n",
    MetadataSize, ResultElementSize, BaseResultEleCount);
#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Create result buffers of increasing size until a failure is detected.
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  do {
#if ENBL_MEM_POOL_DEBUG_LOG_MSGS
    RcDumpHeapUsage ();
#endif
#if ENBL_BIOS_SSA_API_DEBUG_MSGS || ENBL_MEM_POOL_DEBUG_LOG_MSGS
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "  CreateResultBuffer(MetadataSize=%u ResultElementSize=%u ResultsCapacity=%u)\n",
      MetadataSize, ResultElementSize, (BaseResultEleCount + ResultEleCount));
#endif
    RetStat = SsaServicesHandle->SsaResultsConfig->CreateResultBuffer(
      SsaServicesHandle, MetadataSize, ResultElementSize,
      (BaseResultEleCount + ResultEleCount));
#if ENBL_MEM_POOL_DEBUG_LOG_MSGS
    RcDumpHeapUsage ();
#endif

    if (RetStat == Success) {
      MaxResultEleCount = BaseResultEleCount + ResultEleCount;

      ResultEleCount *= 2;
    }
  } while (RetStat == Success);

  return MaxResultEleCount;
} // end function FindMaxResultEleCount

/**
  This function creates the largest results buffer possible.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       MetadataSize       Size of metadata structure.
  @param[in]       ResultElementSize  Size of columnar result element structure.
  @param[in]       ReservedSize       Number of bytes to keep in reserve.
  @param[out]      pResultEleCnt      Pointer to where count of allocated result elements will be stored.  Can be NULL.

  @retval  0  success
  @retval  1  error occurred
**/
UINT32 CreateMaxResultBuffer(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN UINT32 MetadataSize,
  IN UINT32 ResultElementSize,
  IN UINT32 ReservedSize,
  OUT UINT32 *pResultEleCnt)
{
  SSA_STATUS RetStat;
  UINT32 CrntMaxResultEleCnt = 0;
  UINT32 NewMaxResultEleCnt = 0;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "CreateMaxResultBuffer(MetadataSize=%u ResultElementSize=%u ReservedSize=%u)\n",
    MetadataSize, ResultElementSize, ReservedSize);
#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Create result buffers of increasing size and increasing refinement until
  // there's no more room.
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  do {
    CrntMaxResultEleCnt = NewMaxResultEleCnt;
    NewMaxResultEleCnt = FindMaxResultEleCount(SsaServicesHandle, MetadataSize,
      ResultElementSize, CrntMaxResultEleCnt);
  } while (NewMaxResultEleCnt != CrntMaxResultEleCnt);

  // IF reserved bytes are requested
  if (ReservedSize) {
    // IF the size of the reserved bytes exceeds the available space
    if (CrntMaxResultEleCnt < (ReservedSize / ResultElementSize)) {
      ProcError(SsaServicesHandle, Failure, __FILE__, __LINE__, NULL);
      return 1;
    }

    // subtract elements to cover the number of bytes to keep in reserve
    CrntMaxResultEleCnt -= ReservedSize / ResultElementSize;

    // IF the number of bytes to keep in reserve is not an integral number of
    // elements THEN subtract an additional element
    if (ReservedSize % ResultElementSize) {
      // IF there is insufficient available space
      if (CrntMaxResultEleCnt < 2) {
        ProcError(SsaServicesHandle, Failure, __FILE__, __LINE__, NULL);
        return 1;
      }

      CrntMaxResultEleCnt--;
    }

    if (pResultEleCnt != NULL) {
      *pResultEleCnt = CrntMaxResultEleCnt;
    }
  }

#if ENBL_MEM_POOL_DEBUG_LOG_MSGS
  RcDumpHeapUsage ();
#endif
#if ENBL_BIOS_SSA_API_DEBUG_MSGS || ENBL_MEM_POOL_DEBUG_LOG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "  CreateResultBuffer(MetadataSize=%u ResultElementSize=%u ResultsCapacity=%u)\n",
    MetadataSize, ResultElementSize, CrntMaxResultEleCnt);
#endif
  RetStat = SsaServicesHandle->SsaResultsConfig->CreateResultBuffer(
    SsaServicesHandle, MetadataSize, ResultElementSize,
    CrntMaxResultEleCnt);
#if ENBL_MEM_POOL_DEBUG_LOG_MSGS
  RcDumpHeapUsage ();
#endif
  if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__, NULL)) {
    return 1;
  }

  return 0;
} // end function CreateMaxResultBuffer

// end file CreateMaxResultBuffer.c
