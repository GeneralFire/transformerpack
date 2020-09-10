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
#include <Library/BaseMemoryLib.h>
#include <Library/MemPopulationPorLib.h>

MPOR_TBL MemPorTable = {
  MPOR_SIGNATURE, // "MPOR"
  MPOR_REVISION,
  1,
  MAX_CH,
  MAX_DIMM,
  0,
  {
#if MPOR_BITMAPS_8 > 1
//  Flags           DdrBitmap8[0] DdrBitmap8[1] DcpmmBitmap8[0] DcpmmBitmap8[1]
    {NOT_VALIDATED, {NO_DIMMS,    NO_DIMMS},    {NO_DIMMS,      NO_DIMMS}}
#else
//  Flags           DdrBitmap8[0] DcpmmBitmap8[0]
    {NOT_VALIDATED, {NO_DIMMS},   {NO_DIMMS}}
#endif // MPOR_BITMAPS_8 > 1
  }
};

/**

Get the pointer to the POR table.

  @param Host  - Pointer to sysHost

  @retval      - Pointer to the POR table

**/
MPOR_TBL *
GetPorTablePtr (
  PSYSHOST Host
  )
{
  RcDebugPrint (SDBG_MAX, "GetPorTablePtr - Using Default Matrix\n");
  return &MemPorTable;
}
