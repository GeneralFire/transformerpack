/** @file
  MemCmdCtlClkCommon.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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

#ifndef _MEM_CMD_CTL_CLK_COMMON_H_
#define _MEM_CMD_CTL_CLK_COMMON_H_

//
// Define maximums for CMD, CTL and CK for all processors
//

#define MAX_CMD_DELAY     (3 * 64 + 63)   // Logic value of 3 and Pi value of 63
#define MAX_CTL_DELAY     (3 * 64 + 63)   // Logic value of 3 and Pi value of 63
#define MAX_CLK_DELAY     127
#define MAX_CK_DELAY      128
#define MAX_CMD_DELAY_FNV (1 * 64 + 63)   // Logic value of 1 and Pi value of 63

// Ctl FUBs
#define CC_SOUTH          12

#define SIDE_A            0
#define SIDE_B            1
#define MAX_CMD           3

#ifdef DDR5_SUPPORT

#define MAX_CLK           8

#else // #ifdef DDR5_SUPPORT

#define MAX_CLK           4

#endif // #ifdef DDR5_SUPPORT

#endif   // #ifndef _MEM_CMD_CTL_CLK_COMMON_H_
