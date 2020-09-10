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


#include <MemDecode.h>
#include <MemDecodeCommonIncludes.h>


/**
  Initializes security memory map private data.

  @param[in, out] MemMapData  Point to memory map private data structure.

**/
VOID
InitializeSecurityData (
  IN OUT MEMORY_MAP_DATA *MemMapData
  )
{
  return;
}

/**
  Handles security request during SAD interleave algorithm.

  This function calculates valid PRMRR bitmap size for the given SAD interleave.
  If PrmrrSize in private DS is 0, it just calculates all possible PRMRR size with 64MB as minimal size.
  If PrmrrSize in private DS is not 0, it allocates the PRMRR region with that size.
  If this function does actual allocation of PRMRR region for the SAD interleave, InterleaveBase
  field will be adjusted to skip PRMRR region.

  @param[in, out] MemMapData           Point to memory map private data structure.
  @param[in]      SocketId             Socket id.
  @param[in]      MemType              Type of memory that the interleave request corresponds to.
  @param[in]      ImcInterBitmap       Bit map of IMCs participating in the interleave.
  @param[in]      InterleaveSize       The interleave size in 64MB unit.
  @param[in]      InterleaveBase       The base address of interleave start.

**/
VOID
HandleSecurityRequest (
  IN OUT MEMORY_MAP_DATA *MemMapData,
  IN     UINT8           SocketId,
  IN     UINT16          MemType,
  IN     UINT8           ImcInterBitmap,
  IN     UINT32          InterleaveSize,
  IN     UINT32          InterleaveBase
  )
{
  return;
}
