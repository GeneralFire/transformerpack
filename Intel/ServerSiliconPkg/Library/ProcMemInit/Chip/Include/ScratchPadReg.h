/** @file
  Definitions of scratch pad registers used by BIOS SSA EV agent.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2017 Intel Corporation. <BR>

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

#ifndef __SCRATCHPAD_REG_H__
#define __SCRATCHPAD_REG_H__

#include <Library/MemoryCoreLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Ppi/SsaBiosServicesPpi.h>
#include <Library/DebugLib.h>

extern UINT32 BiosNonStickyScratchPadCsr[];
#define SCRATCH_PAD_NUM 13

//
// Address for the scratchpad registers
//

#define EV_AGENT_SCRATCHPAD_CHECKPOINT_REG_ADDRESS          (BIOSSCRATCHPAD6_UBOX_MISC_REG) // upper 16 bits
#define EV_AGENT_SCRATCHPAD_CURRENT_CHECKPOINT_REG_ADDRESS  (BIOSNONSTICKYSCRATCHPAD7_UBOX_MISC_REG)
#define EV_AGENT_SCRATCHPAD_CMD_REG_ADDRESS                 (BIOSSCRATCHPAD6_UBOX_MISC_REG) // lower 16 bits
#define EV_AGENT_SCRATCHPAD_DATA0_REG_ADDRESS               (BIOSSTICKYLOCKBYPASSSCRATCHPAD0_UBOX_CFG_REG)
#define EV_AGENT_SCRATCHPAD_DATA1_REG_ADDRESS               (BIOSSTICKYLOCKBYPASSSCRATCHPAD1_UBOX_CFG_REG)

#endif //__SCRATCHPAD_REG_H__
