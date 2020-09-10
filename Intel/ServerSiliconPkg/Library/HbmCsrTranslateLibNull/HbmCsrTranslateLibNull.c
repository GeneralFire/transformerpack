/** @file
  HbmCsrTranslateLibNull.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <MemCommon.h>
#include <UsraAccessType.h>
#include <Chip/Include/CpuPciAccess.h>

/**
  Used to do a translation between DDR MC to HBM MC

  @param[in] MemTechType  - Memory Technology Type
  @param[in] RegOffset    - Data of CSR register offset

  @retval RegOffset       - CSR register offset

**/
UINT32
DdrToHbmCompatibility (
  IN  MEM_TECH_TYPE  MemTechType,
  IN  UINT32         RegOffset
  )
{
  if (MemTechType == MemTechHbm) {
    //
    // Shall not reach here as these code is for DDR, access HBM should ASSERT
    //
    ASSERT(FALSE);
  }

  return RegOffset;
}

/**
  Used to calculate offset from DDR MC to HBM MC

  @param[in, out] CsrOffset    - pointer to data of CSR register offset

  @retval NULL

**/
VOID
DdrToHbmOffset (
  IN  OUT  CSR_OFFSET  *CsrOffset
  )
{
  return;
}
