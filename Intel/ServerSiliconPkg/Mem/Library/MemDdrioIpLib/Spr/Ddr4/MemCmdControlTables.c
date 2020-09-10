/** @file

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
#include <Library/MemoryCoreLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/BaseLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/BaseMemoryLib.h>
#include "MemCmdControl.h"
#include <Memory/MemCmdCtlClkCommon.h>
#include "Include/MemDdrioRegs.h"
#include <UncoreCommonIncludes.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/CheckpointLib.h>
#include <Library/MemRcLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/PkgDelayLib.h>
#include <Include/MemIoControl.h>
#include <Chip/Include/MemHostChip.h>
#include <Library/MemRoutingLib.h>
#include <Library/Ddr5CoreLib.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Include/MemDdrioSpecific.h>

//
// SPR DDR4 CMD Signal List.
// Map Platform CMD Groups to IO CMD Groups.
//
SIGNAL_DESCRIPTOR PlatformCmdGroupAllChip[] = {
  {5,   SUB_CH_B}, // BA0,   CmdGrp0
  {6,   SUB_CH_B}, // BA1,   CmdGrp0
  {1,   SUB_CH_A}, // A0,    CmdGrp0
  {2,   SUB_CH_A}, // A1,    CmdGrp0
  {3,   SUB_CH_A}, // A2,    CmdGrp1
  {4,   SUB_CH_A}, // A3,    CmdGrp0
  {5,   SUB_CH_A}, // A4,    CmdGrp1
  {6,   SUB_CH_A}, // A5,    CmdGrp1
  {7,   SUB_CH_A}, // A6,    CmdGrp1
  {8,   SUB_CH_A}, // A7,    CmdGrp1
  {9,   SUB_CH_A}, // A8,    CmdGrp1
  {10,  SUB_CH_A}, // A9,    CmdGrp1
  {11,  SUB_CH_A}, // A10,   CmdGrp0
  {12,  SUB_CH_A}, // A11,   CmdGrp2
  {13,  SUB_CH_A}, // A12,   CmdGrp1
  {14,  SUB_CH_A}, // A13,   CmdGrp0
  {1,   SUB_CH_B}, // A14,   CmdGrp0; WE
  {2,   SUB_CH_B}, // A15,   CmdGrp0; CAS
  {3,   SUB_CH_B}, // A16,   CmdGrp0; RAS
  {4,   SUB_CH_B}, // A17,   CmdGrp2
  {17,  SUB_CH_B}, // PAR,   CmdGrp0
  {19,  SUB_CH_A}, // BG0,   CmdGrp2
  {18,  SUB_CH_A}, // BG1,   CmdGrp2
  {17,  SUB_CH_A}, // ACT_N, CmdGrp2
  {13,  SUB_CH_B}  // C2,    CmdGrp2
};

//
// SPR DDR4 CTL Signal List.
// Map Platform CTL Groups to IO CTL Groups.
//

//
// ODT0, CKE0, CS0, CS2, CS3
//
SIGNAL_DESCRIPTOR PlatformCtlGroup0Chip[] = {
  {10,  SUB_CH_B}, // ODT0,  CtlGrp0
  {21,  SUB_CH_A}, // CKE0,  CtlGrp0
  {12,  SUB_CH_B}, // CS0_N, CtlGrp0
  {8,   SUB_CH_B}, // CS2_N, CtlGrp0
  {11,  SUB_CH_B}  // CS3_N, CtlGrp0
};

//
// ODT1, CKE1, CS1
//
SIGNAL_DESCRIPTOR PlatformCtlGroup1Chip[] = {
  {16,  SUB_CH_B}, // ODT1,  CtlGrp1
  {16,  SUB_CH_A}, // CKE1,  CtlGrp1
  {9,   SUB_CH_B}  // CS1_N, CtlGrp1
};

//
// ODT2, CKE2, CS4, CS6, CS7
//
SIGNAL_DESCRIPTOR PlatformCtlGroup2Chip[] = {
  {0,   SUB_CH_B}, // ODT2,  CtlGrp2
  {20,  SUB_CH_A}, // CKE2,  CtlGrp2
  {19,  SUB_CH_B}, // CS4_N, CtlGrp2
  {21,  SUB_CH_B}, // CS6_N, CtlGrp2
  {20,  SUB_CH_B}  // CS7_N, CtlGrp2,
};

//
// ODT3, CKE3, CS5
//
SIGNAL_DESCRIPTOR PlatformCtlGroup3Chip[] = {
  {15,  SUB_CH_B}, // ODT3,  CtlGrp3
  {15,  SUB_CH_A}, // CKE3,  CtlGrp,
  {18,  SUB_CH_B}  // CS5_N, CtlGrp3
};

//
// All CTL signals.
//
SIGNAL_DESCRIPTOR PlatformCtlGroupAllChip[] = {
  {10,  SUB_CH_B}, // ODT0,  CtlGrp0
  {21,  SUB_CH_A}, // CKE0,  CtlGrp0
  {12,  SUB_CH_B}, // CS0_N, CtlGrp0
  {8,   SUB_CH_B}, // CS2_N, CtlGrp0
  {11,  SUB_CH_B}, // CS3_N, CtlGrp0
  {16,  SUB_CH_B}, // ODT1,  CtlGrp1
  {16,  SUB_CH_A}, // CKE1,  CtlGrp1
  {9,   SUB_CH_B}, // CS1_N, CtlGrp1
  {0,   SUB_CH_B}, // ODT2,  CtlGrp2
  {20,  SUB_CH_A}, // CKE2,  CtlGrp2
  {19,  SUB_CH_B}, // CS4_N, CtlGrp2
  {21,  SUB_CH_B}, // CS6_N, CtlGrp2
  {20,  SUB_CH_B}, // CS7_N, CtlGrp2
  {15,  SUB_CH_B}, // ODT3,  CtlGrp3
  {15,  SUB_CH_A}, // CKE3,  CtlGrp,
  {18,  SUB_CH_B}  // CS5_N, CtlGrp3

};

//
// SPR DDR4 Full CTL and CMD Signal List.
//
SIGNAL_DESCRIPTOR PlatformCmdCtlGroupAllChip[] = {
  {5,   SUB_CH_B}, // BA0,   CmdGrp0
  {6,   SUB_CH_B}, // BA1,   CmdGrp0
  {1,   SUB_CH_A}, // A0,    CmdGrp0
  {2,   SUB_CH_A}, // A1,    CmdGrp0
  {3,   SUB_CH_A}, // A2,    CmdGrp1
  {4,   SUB_CH_A}, // A3,    CmdGrp0
  {5,   SUB_CH_A}, // A4,    CmdGrp1
  {6,   SUB_CH_A}, // A5,    CmdGrp1
  {7,   SUB_CH_A}, // A6,    CmdGrp1
  {8,   SUB_CH_A}, // A7,    CmdGrp1
  {9,   SUB_CH_A}, // A8,    CmdGrp1
  {10,  SUB_CH_A}, // A9,    CmdGrp1
  {11,  SUB_CH_A}, // A10,   CmdGrp0
  {12,  SUB_CH_A}, // A11,   CmdGrp2
  {13,  SUB_CH_A}, // A12,   CmdGrp1
  {14,  SUB_CH_A}, // A13,   CmdGrp0
  {1,   SUB_CH_B}, // A14,   CmdGrp0; WE
  {2,   SUB_CH_B}, // A15,   CmdGrp0; CAS
  {3,   SUB_CH_B}, // A16,   CmdGrp0; RAS
  {4,   SUB_CH_B}, // A17,   CmdGrp2
  {17,  SUB_CH_B}, // PAR,   CmdGrp0
  {19,  SUB_CH_A}, // BG0,   CmdGrp2
  {18,  SUB_CH_A}, // BG1,   CmdGrp2
  {17,  SUB_CH_A}, // ACT_N, CmdGrp2
  {13,  SUB_CH_B}, // C2,    CmdGrp2
  {10,  SUB_CH_B}, // ODT0,  CtlGrp0
  {21,  SUB_CH_A}, // CKE0,  CtlGrp0
  {12,  SUB_CH_B}, // CS0_N, CtlGrp0
  {8,   SUB_CH_B}, // CS2_N, CtlGrp0
  {11,  SUB_CH_B}, // CS3_N, CtlGrp0
  {16,  SUB_CH_B}, // ODT1,  CtlGrp1
  {16,  SUB_CH_A}, // CKE1,  CtlGrp1
  {9,   SUB_CH_B}, // CS1_N, CtlGrp1
  {0,   SUB_CH_B}, // ODT2,  CtlGrp2
  {20,  SUB_CH_A}, // CKE2,  CtlGrp2
  {19,  SUB_CH_B}, // CS4_N, CtlGrp2
  {21,  SUB_CH_B}, // CS6_N, CtlGrp2
  {20,  SUB_CH_B}, // CS7_N, CtlGrp2
  {15,  SUB_CH_B}, // ODT3,  CtlGrp3
  {15,  SUB_CH_A}, // CKE3,  CtlGrp,
  {18,  SUB_CH_B}  // CS5_N, CtlGrp3
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

  switch (Group) {
    case CmdAll:
      *CmdCtlGroupSize = ARRAY_SIZE (PlatformCmdGroupAllChip);
      *CmdCtlGroupPtr = PlatformCmdGroupAllChip;
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
    case CtlAll:
      *CmdCtlGroupSize = ARRAY_SIZE (PlatformCtlGroupAllChip);
      *CmdCtlGroupPtr = PlatformCtlGroupAllChip;
      break;
    case CmdCtlAll:
      *CmdCtlGroupSize = ARRAY_SIZE (PlatformCmdCtlGroupAllChip);
      *CmdCtlGroupPtr = PlatformCmdCtlGroupAllChip;
      break;
    default:
      Status = MRC_STATUS_GROUP_NOT_SUPPORTED;
      break;
  }

  return Status;
}
