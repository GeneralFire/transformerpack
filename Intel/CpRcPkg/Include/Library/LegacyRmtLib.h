/** @file
  LegacyRmtLib.h

  Library header with API to support Legacy RMT function

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2018 Intel Corporation. <BR>

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


#ifndef _LEGACY_RMT_LIB_H_
#define _LEGACY_RMT_LIB_H_

//Defines for Legacy RMT

//
// Legacy RMT Disable
//
#define RMT_DIS       0
//
// Legacy RMT Enable
//
#define RMT_EN        1
//
// Disable Backside RMT
//
#define BACKSIDE_RMT_DISABLE     0
//
// Enable Backside RMT
//
#define BACKSIDE_RMT_ENABLE      1

//
// Disable Backside CMD RMT
//
#define BACKSIDE_CMD_RMT_DISABLE 0
//
// Enable Backside CMD RMT
//
#define BACKSIDE_CMD_RMT_ENABLE  1

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
  );

/*

 DFE path finding algorithm to analyze the behavior of each TAP setting

 @param[in]   Host    SysHost structure pointer

 @retval      SUCCESS Indicate proper execution

*/
UINT32
EFIAPI
DfePathFinding (
  IN PSYSHOST Host
  );
#endif //#ifndef _LEGACY_RMT_LIB_H_
