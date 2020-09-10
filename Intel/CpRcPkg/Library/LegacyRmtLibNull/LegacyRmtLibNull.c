/** @file
  LegacyRmtLibNull.c

  Library implementation of API to support Legacy RMT function.
  This is a NULL implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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
#include <Uefi.h>
#include "SysHost.h"

/**

  Finds margins for various parameters per Rank

  @param[in] Host    - Pointer to sysHost, 
                       the system Host (root) structure struct

  @retval SUCCESS       Function executed successfully
  @retVal FAILURE       Function has errors

**/
UINT32
EFIAPI
RankMarginTool (
  IN PSYSHOST Host
  )
{
  return SUCCESS;
}

/*

 DFE path finding algorithm to analyze the behavior of each TAP setting

 @param[in]   Host    SysHost structure pointer

 @retval      SUCCESS Indicate proper execution

*/
UINT32
EFIAPI
DfePathFinding (
  IN PSYSHOST Host
  )
{
  return SUCCESS;
}
