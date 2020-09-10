/** @file

  Functions providing lists of CMD and CTL SPR DDR5 PHY signals and register indexes

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

#include <SysHostChip.h>
#include <MemCommon.h>
#include <MrcCommonTypes.h>
#include <Library/MemoryCoreLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/Ddr5CoreLib.h>
#include <Library/RcDebugLib.h>
#include "MemCmdControl.h"

//
// SPR Signal definitions
//

SIGNAL_DESCRIPTOR PlatformCmdCtlGroupAllChip[] = {
  {17,  SUB_CH_A}, // PARA
  {16,  SUB_CH_A}, // RSPA_SA
  {15,  SUB_CH_A}, // RSPB_SA
  {14,  SUB_CH_A}, // A13A
  {13,  SUB_CH_A}, // A12A
  {12,  SUB_CH_A}, // A11A
  {11,  SUB_CH_A}, // A10A
  {10,  SUB_CH_A}, // A9A
  {9,   SUB_CH_A}, // A8A
  {8,   SUB_CH_A}, // A7A
  {7,   SUB_CH_A}, // A6A
  {6,   SUB_CH_A}, // A5A
  {5,   SUB_CH_A}, // A4A
  {4,   SUB_CH_A}, // A3A
  {3,   SUB_CH_A}, // A2A
  {2,   SUB_CH_A}, // A1A
  {1,   SUB_CH_A}, // A0A
  {0,   SUB_CH_A}, // ALERT_N_DDR5
  {0,   SUB_CH_B}, // SPARE
  {1,   SUB_CH_B}, // A0B
  {2,   SUB_CH_B}, // A1B
  {3,   SUB_CH_B}, // A2B
  {4,   SUB_CH_B}, // A3B
  {5,   SUB_CH_B}, // A4B
  {6,   SUB_CH_B}, // A5B
  {7,   SUB_CH_B}, // A6B
  {8,   SUB_CH_B}, // A7B
  {9,   SUB_CH_B}, // A8B
  {10,  SUB_CH_B}, // A9B
  {11,  SUB_CH_B}, // A10B
  {12,  SUB_CH_B}, // A11B
  {13,  SUB_CH_B}, // A12B
  {14,  SUB_CH_B}, // A13B
  {15,  SUB_CH_B}, // RSPB_SB
  {16,  SUB_CH_B}, // RSPA_SB
  {17,  SUB_CH_B}, // PARB
  {21,  SUB_CH_A}, // CS2_A_N
  {20,  SUB_CH_A}, // CS3_A_N
  {19,  SUB_CH_A}, // CS0_A_N
  {18,  SUB_CH_A}, // CS1_A_N
  {18,  SUB_CH_B}, // CS1_B_N
  {19,  SUB_CH_B}, // CS0_B_N
  {20,  SUB_CH_B}, // CS3_B_N
  {21,  SUB_CH_B}, // CS2_B_N
};

SIGNAL_DESCRIPTOR PlatformCtlGroupAllChip[] = {
  {21,  SUB_CH_A}, // CS2_A_n
  {20,  SUB_CH_A}, // CS3_A_n
  {19,  SUB_CH_A}, // CS0_A_n
  {18,  SUB_CH_A}, // CS1_A_n
  {18,  SUB_CH_B}, // CS1_B_n
  {19,  SUB_CH_B}, // CS0_B_n
  {20,  SUB_CH_B}, // CS3_B_n
  {21,  SUB_CH_B}, // CS2_B_n
};

SIGNAL_DESCRIPTOR PlatformCtlGroup0Chip[] = {
  {19,  SUB_CH_A}, // CS0_A_n
  {19,  SUB_CH_B}, // CS0_B_n
};

SIGNAL_DESCRIPTOR PlatformCtlGroup1Chip[] = {
  {18,  SUB_CH_A}, // CS1_A_n
  {18,  SUB_CH_B}, // CS1_B_n
};

SIGNAL_DESCRIPTOR PlatformCtlGroup2Chip[] = {
  {21,  SUB_CH_A}, // CS2_A_n
  {21,  SUB_CH_B}, // CS2_B_n
};

SIGNAL_DESCRIPTOR PlatformCtlGroup3Chip[] = {
  {20,  SUB_CH_A}, // CS3_A_n
  {20,  SUB_CH_B}, // CS3_B_n
};

// Map Platform CMD Groups to IO CMD Groups
SIGNAL_DESCRIPTOR PlatformCmdGroupAllChip[] = {
  {17,  SUB_CH_A}, // PARA
  // {16,  SUB_CH_A}, // RSPA_SA
  // {15,  SUB_CH_A}, // RSPB_SA
  {14,  SUB_CH_A}, // A13A
  {13,  SUB_CH_A}, // A12A
  {12,  SUB_CH_A}, // A11A
  {11,  SUB_CH_A}, // A10A
  {10,  SUB_CH_A}, // A9A
  {9,   SUB_CH_A}, // A8A
  {8,   SUB_CH_A}, // A7A
  {7,   SUB_CH_A}, // A6A
  {6,   SUB_CH_A}, // A5A
  {5,   SUB_CH_A}, // A4A
  {4,   SUB_CH_A}, // A3A
  {3,   SUB_CH_A}, // A2A
  {2,   SUB_CH_A}, // A1A
  {1,   SUB_CH_A}, // A0A
  //{0,   SUB_CH_A}, // ALERT_N_DDR5
  //{0,   SUB_CH_B}, // SPARE
  {1,   SUB_CH_B}, // A0B
  {2,   SUB_CH_B}, // A1B
  {3,   SUB_CH_B}, // A2B
  {4,   SUB_CH_B}, // A3B
  {5,   SUB_CH_B}, // A4B
  {6,   SUB_CH_B}, // A5B
  {7,   SUB_CH_B}, // A6B
  {8,   SUB_CH_B}, // A7B
  {9,   SUB_CH_B}, // A8B
  {10,  SUB_CH_B}, // A9B
  {11,  SUB_CH_B}, // A10B
  {12,  SUB_CH_B}, // A11B
  {13,  SUB_CH_B}, // A12B
  {14,  SUB_CH_B}, // A13B
  //{15,  SUB_CH_B}, // RSPB_SB
  //{16,  SUB_CH_B}, // RSPA_SB
  {17,  SUB_CH_B}, // PARB
};

// Map Platform CMD Groups to IO CMD Group with RCD
SIGNAL_DESCRIPTOR PlatformRcdCmdGroupAllChip[] = {
  {17,  SUB_CH_A}, // PARA
  {7,   SUB_CH_A}, // A6A
  {6,   SUB_CH_A}, // A5A
  {5,   SUB_CH_A}, // A4A
  {4,   SUB_CH_A}, // A3A
  {3,   SUB_CH_A}, // A2A
  {2,   SUB_CH_A}, // A1A
  {1,   SUB_CH_A}, // A0A
  {1,   SUB_CH_B}, // A0B
  {2,   SUB_CH_B}, // A1B
  {3,   SUB_CH_B}, // A2B
  {4,   SUB_CH_B}, // A3B
  {5,   SUB_CH_B}, // A4B
  {6,   SUB_CH_B}, // A5B
  {7,   SUB_CH_B}, // A6B
  {17,  SUB_CH_B}, // PARB
};

/**
  Gets pointer to CMD/CTL signal group and number of elements in the group.

  @param Host:             Pointer to SYSHOST.
  @param Group:            CMD/CTL signal group to get.
  @param CmdCtlGroupSize:  Ponter to number of elements in the CMD/CTL signal group.
  @param CmdCtlGroupPtr:   Pointer to CMD/CTL signal group.

  @retval MRC_STATUS_SUCCESS or MRC_STATUS_GROUP_NOT_SUPPORTED

**/
MRC_STATUS
GetCmdCtlGroupList (
  PSYSHOST          Host,
  MRC_GT            Group,
  UINT8             *CmdCtlGroupSize,
  SIGNAL_DESCRIPTOR **CmdCtlGroupPtr
  )
{
  MRC_STATUS Status = MRC_STATUS_SUCCESS;
  UINT8 SocketId;

  SocketId = GetSysSocketId ();

  switch (Group) {
    case CmdAll:
      *CmdCtlGroupSize = ARRAY_SIZE (PlatformCmdGroupAllChip);
      *CmdCtlGroupPtr = PlatformCmdGroupAllChip;
      break;

    case CmdCtlAll:
      *CmdCtlGroupSize = ARRAY_SIZE (PlatformCmdCtlGroupAllChip);
      *CmdCtlGroupPtr = PlatformCmdCtlGroupAllChip;
      break;
    case CtlGrp0:
      *CmdCtlGroupSize = ARRAY_SIZE (PlatformCtlGroup0Chip);
      *CmdCtlGroupPtr = PlatformCtlGroup0Chip;
      break;
    case CtlGrp1:
      *CmdCtlGroupSize = ARRAY_SIZE (PlatformCtlGroup1Chip);
      *CmdCtlGroupPtr = PlatformCtlGroup1Chip;
      break;
    case CtlGrp2:
      *CmdCtlGroupSize = ARRAY_SIZE (PlatformCtlGroup2Chip);
      *CmdCtlGroupPtr = PlatformCtlGroup2Chip;
      break;
    case CtlGrp3:
      *CmdCtlGroupSize = ARRAY_SIZE (PlatformCtlGroup3Chip);
      *CmdCtlGroupPtr = PlatformCtlGroup3Chip;
      break;
    case RcdCmdAll:
      *CmdCtlGroupSize = ARRAY_SIZE(PlatformRcdCmdGroupAllChip);
      *CmdCtlGroupPtr = PlatformRcdCmdGroupAllChip;
      break;
    default:
      Status = MRC_STATUS_GROUP_NOT_SUPPORTED;
      break;
  }

  return Status;
}
