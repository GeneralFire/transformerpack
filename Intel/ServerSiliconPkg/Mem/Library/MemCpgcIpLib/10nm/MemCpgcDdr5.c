/** @file
CPGC code specific to DDR5

@copyright
INTEL CONFIDENTIAL
Copyright 2009 - 2020 Intel Corporation. <BR>

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

#ifdef DDR5_SUPPORT
//
// Include files
//
#include <Library/MemoryCoreLib.h>
#include <Library/MemCpgcIpLib.h>
#include "MemCpgcRegs.h"
#include <UncoreCommonIncludes.h>
#include <Library/SysHostPointerLib.h>
#include "CpgcChip.h"
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemRcLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Chip/Include/CpuPciAccess.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/CteNetLib.h>
#include <Library/MemCpgcIpTargetLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/TimerLib.h>
#include <Library/CsrAccessLib.h>
#include <Memory/Ddr5Cmds.h>
#include <Library/MemRoutingLib.h>
#include <Library/Ddr5CoreLib.h>
#include <Library/PerformanceTrackerLib.h>
#include <Memory/Ddr5DbRegs.h>
#include <Memory/Ddr5MrRegs.h>
#include <Memory/Ddr5RcwRegs.h>
#include <Memory/JedecDefinitions.h>

STATIC VOID
FixupAddrBitsDdr5 (
  IN PSYSHOST         Host,
  IN GSM_CSN          sig,
  IN OUT PatCadbProg0 prog0[CADB_LINES],
  IN OUT PatCadbProg1 prog1[CADB_LINES]
  );


//
// NOP Command Defintion
//
// addr,                      cid, bank, pdatag
static PatCadbProg0 NopPattern0[CADB_LINES] =
{ { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 0
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 1
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 2
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 3
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 4
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 5
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 6
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 7
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 8
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 9
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 10
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 11
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 12
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 13
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 14
{ 0xFFFF,   0x0, 0x0, 0x0 } // Row 15
};

//
// Precharge Command Defintion
//
// addr,                      cid, bank, pdatag
static PatCadbProg0 PrechargePattern0[CADB_LINES] =
{ { 0x000B,   0x0, 0x0, 0x0 }, // Row 0
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 1
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 2
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 3
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 4
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 5
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 6
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 7
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 8
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 9
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 10
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 11
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 12
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 13
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 14
{ 0xFFFF,   0x0, 0x0, 0x0 } // Row 15
};

//
// Write Command Defintion
//
// addr,                      cid, bank, pdatag
static PatCadbProg0 WritePattern0[CADB_LINES] =
{ { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 0
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 1
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 2
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 3
{ 0x002D,   0x0, 0x0, 0x0 }, // Row 4
{ 0x1E01,   0x0, 0x0, 0x0 }, // Row 5
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 6
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 7
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 8
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 9
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 10
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 11
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 12
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 13
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 14
{ 0xFFFF,   0x0, 0x0, 0x0 } // Row 15
};
//
// MRR Command Defintion
//
// addr,                      cid, bank, pdatag
static PatCadbProg0 MrrPattern0[CADB_LINES] =
{ { 0xE035,   0x0, 0x0, 0x0 }, // Row 0
{ 0xFB00,   0x0, 0x0, 0x0 }, // Row 1
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 2
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 3
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 4
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 5
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 6
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 7
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 8
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 9
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 10
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 11
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 12
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 13
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 14
{ 0xFFFF,   0x0, 0x0, 0x0 } // Row 15
};
//
// MRW Command Definition
//
// addr,                      cid, bank, pdatag
static PatCadbProg0 MrwPattern0[CADB_LINES] =
{ { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 0
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 1
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 2
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 3
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 4
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 5
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 6
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 7
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 8
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 9
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 10
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 11
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 12
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 13
{ 0xFFFF,   0x0, 0x0, 0x0 }, // Row 14
{ 0xFFFF,   0x0, 0x0, 0x0 } // Row 15
};
//
//MPC command pattern definition
//

// addr,                      cid, bank, pdatag
STATIC PatCadbProg0 MpcCATrainingModeEntryPattern0[CADB_LINES] =
{ { MPC1(Enter_CA_Training_Mode),   0x0, 0x0, 0x0 }, // Row 0
{ MPC2(Enter_CA_Training_Mode),   0x0, 0x0, 0x0 }, // Row 1
{ MPC1(Enter_CA_Training_Mode),   0x0, 0x0, 0x0 }, // Row 2
{ MPC2(Enter_CA_Training_Mode),   0x0, 0x0, 0x0 }, // Row 3
{ MPC1(Enter_CA_Training_Mode),   0x0, 0x0, 0x0 }, // Row 4
{ MPC2(Enter_CA_Training_Mode),   0x0, 0x0, 0x0 }, // Row 5
{ MPC1(Enter_CA_Training_Mode),   0x0, 0x0, 0x0 }, // Row 6
{ MPC2(Enter_CA_Training_Mode),   0x0, 0x0, 0x0 }, // Row 7
{ MPC1(Enter_CA_Training_Mode),   0x0, 0x0, 0x0 }, // Row 8
{ MPC2(Enter_CA_Training_Mode),   0x0, 0x0, 0x0 }, // Row 9
{ MPC1(Enter_CA_Training_Mode),   0x0, 0x0, 0x0 }, // Row 10
{ MPC2(Enter_CA_Training_Mode),   0x0, 0x0, 0x0 }, // Row 11
{ MPC1(Enter_CA_Training_Mode),   0x0, 0x0, 0x0 }, // Row 12
{ MPC2(Enter_CA_Training_Mode),   0x0, 0x0, 0x0 }, // Row 13
{ MPC1(Enter_CA_Training_Mode),   0x0, 0x0, 0x0 }, // Row 14
{ MPC2(Enter_CA_Training_Mode),   0x0, 0x0, 0x0 } // Row 15
};
// CS,      Control, ODT,  CKE, Par
STATIC PatCadbProg1 MpcCATrainingModeEntryPattern1_4N[CADB_LINES] =
{ { 0x3FF,  0X3, 0x0, 0, 0 }, // Row 0
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 1
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 2
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 3
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 4
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 5
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 6
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 7
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 8
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 9
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 10
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 11
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 12
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 13
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 14
{ 0x3FF,     0X3, 0x0, 0, 0 }  // Row 15
};
STATIC PatCadbProg0 MpcCATrainingModeExitPattern0[CADB_LINES] =
{ { NOP_Command,   0x0, 0x0, 0x0 }, // Row 0
{ NOP_Command,   0x0, 0x0, 0x0 }, // Row 1
{ NOP_Command,   0x0, 0x0, 0x0 }, // Row 2
{ NOP_Command,   0x0, 0x0, 0x0 }, // Row 3
{ NOP_Command,   0x0, 0x0, 0x0 }, // Row 4
{ NOP_Command,   0x0, 0x0, 0x0 }, // Row 5
{ NOP_Command,   0x0, 0x0, 0x0 }, // Row 6
{ NOP_Command,   0x0, 0x0, 0x0 }, // Row 7
{ NOP_Command,   0x0, 0x0, 0x0 }, // Row 8
{ NOP_Command,   0x0, 0x0, 0x0 }, // Row 9
{ NOP_Command,   0x0, 0x0, 0x0 }, // Row 10
{ NOP_Command,   0x0, 0x0, 0x0 }, // Row 11
{ NOP_Command,   0x0, 0x0, 0x0 }, // Row 12
{ NOP_Command,   0x0, 0x0, 0x0 }, // Row 13
{ NOP_Command,   0x0, 0x0, 0x0 }, // Row 14
{ NOP_Command,   0x0, 0x0, 0x0 } // Row 15
};

// CS,      Control, ODT,  CKE, Par
STATIC PatCadbProg1 MpcCATrainingModeExitPattern1_4N[CADB_LINES] =
{ { 0x3FF,     0X3, 0x0, 0, 0 }, // Row 0
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 1
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 2
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 3
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 4
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 5
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 6
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 7
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 8
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 9
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 10
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 11
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 12
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 13
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 14
{ 0x3FF,     0X3, 0x0, 0, 0 } // Row 15
};
STATIC PatCadbProg0 MpcCSTrainingModeEntryPattern0[CADB_LINES] =
{ { MPC1(Enter_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 0
{ MPC2(Enter_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 1
{ MPC1(Enter_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 2
{ MPC2(Enter_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 3
{ MPC1(Enter_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 4
{ MPC2(Enter_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 5
{ MPC1(Enter_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 6
{ MPC2(Enter_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 7
{ MPC1(Enter_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 8
{ MPC2(Enter_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 9
{ MPC1(Enter_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 10
{ MPC2(Enter_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 11
{ MPC1(Enter_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 12
{ MPC2(Enter_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 13
{ MPC1(Enter_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 14
{ MPC2(Enter_CS_Training_Mode),   0x0, 0x0, 0x0 } // Row 15
};

// CS,      Control, ODT,  CKE, Par
STATIC PatCadbProg1 MpcCSTrainingModeEntryPattern1_4N[CADB_LINES] =
{ { 0x3FF,     0X3, 0x0, 0, 0 }, // Row 0
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 1
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 2
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 3
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 4
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 5
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 6
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 7
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 8
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 9
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 10
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 11
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 12
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 13
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 14
{ 0x3FF,     0X3, 0x0, 0, 0 } // Row 15
};
STATIC PatCadbProg0 MpcCSTrainingModeExitPattern0[CADB_LINES] =
{ { MPC1(Exit_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 0
{ MPC2(Exit_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 1
{ MPC1(Exit_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 2
{ MPC2(Exit_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 3
{ MPC1(Exit_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 4
{ MPC2(Exit_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 5
{ MPC1(Exit_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 6
{ MPC2(Exit_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 7
{ MPC1(Exit_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 8
{ MPC2(Exit_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 9
{ MPC1(Exit_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 10
{ MPC2(Exit_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 11
{ MPC1(Exit_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 12
{ MPC2(Exit_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 13
{ MPC1(Exit_CS_Training_Mode),   0x0, 0x0, 0x0 }, // Row 14
{ MPC2(Exit_CS_Training_Mode),   0x0, 0x0, 0x0 } // Row 15
};

// CS,      Control, ODT,  CKE, Par
STATIC PatCadbProg1 MpcCsTrainingModeExitPattern1_4N[CADB_LINES] =
{ { 0x3FF,     0X3, 0x0, 0, 0 }, // Row 0
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 1
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 2
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 3
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 4
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 5
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 6
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 7
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 8
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 9
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 10
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 11
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 12
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 13
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 14
{ 0x3FF,     0X3, 0x0, 0, 0 } // Row 15
};

// CS,      Control, ODT,  CKE, Par
STATIC PatCadbProg1 MpcDllResetPattern1_4N[CADB_LINES] = {
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 0
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 1
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 2
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 3
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 4
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 5
  { 0x3FE,     0x3, 0x0, 0, 0 },  // Row 6
  { 0x3FE,     0x3, 0x0, 0, 0 },  // Row 7
  { 0x3FE,     0x3, 0x0, 0, 0 },  // Row 8
  { 0x3FE,     0x3, 0x0, 0, 0 },  // Row 9
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 10
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 11
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 12
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 13
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 14
  { 0x3FF,     0x3, 0x0, 0, 0 }   // Row 15
};
STATIC PatCadbProg0 MpcZQCalLatchPattern0[CADB_LINES] =
{ { MPC1(ZQCAL_LATCH),   0x0, 0x0, 0x0 }, // Row 0
{ MPC2(ZQCAL_LATCH),   0x0, 0x0, 0x0 }, // Row 1
{ MPC1(ZQCAL_LATCH),   0x0, 0x0, 0x0 }, // Row 2
{ MPC2(ZQCAL_LATCH),   0x0, 0x0, 0x0 }, // Row 3
{ MPC1(ZQCAL_LATCH),   0x0, 0x0, 0x0 }, // Row 4
{ MPC2(ZQCAL_LATCH),   0x0, 0x0, 0x0 }, // Row 5
{ MPC1(ZQCAL_LATCH),   0x0, 0x0, 0x0 }, // Row 6
{ MPC2(ZQCAL_LATCH),   0x0, 0x0, 0x0 }, // Row 7
{ MPC1(ZQCAL_LATCH),   0x0, 0x0, 0x0 }, // Row 8
{ MPC2(ZQCAL_LATCH),   0x0, 0x0, 0x0 }, // Row 9
{ MPC1(ZQCAL_LATCH),   0x0, 0x0, 0x0 }, // Row 10
{ MPC2(ZQCAL_LATCH),   0x0, 0x0, 0x0 }, // Row 11
{ MPC1(ZQCAL_LATCH),   0x0, 0x0, 0x0 }, // Row 12
{ MPC2(ZQCAL_LATCH),   0x0, 0x0, 0x0 }, // Row 13
{ MPC1(ZQCAL_LATCH),   0x0, 0x0, 0x0 }, // Row 14
{ MPC2(ZQCAL_LATCH),   0x0, 0x0, 0x0 } // Row 15
};

// CS,      Control, ODT,  CKE, Par
STATIC PatCadbProg1 MpcZQCalLatchPattern1_4N[CADB_LINES] = {
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 0
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 1
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 2
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 3
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 4
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 5
  { 0x3FE,     0x3, 0x0, 0, 0 },  // Row 6
  { 0x3FE,     0x3, 0x0, 0, 0 },  // Row 7
  { 0x3FE,     0x3, 0x0, 0, 0 },  // Row 8
  { 0x3FE,     0x3, 0x0, 0, 0 },  // Row 9
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 10
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 11
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 12
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 13
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 14
  { 0x3FF,     0x3, 0x0, 0, 0 }   // Row 15
};
STATIC PatCadbProg0 MpcZQCalStartPattern0[CADB_LINES] =
{ { MPC1(ZQCAL_START),   0x0, 0x0, 0x0 }, // Row 0
{ MPC2(ZQCAL_START),   0x0, 0x0, 0x0 }, // Row 1
{ MPC1(ZQCAL_START),   0x0, 0x0, 0x0 }, // Row 2
{ MPC2(ZQCAL_START),   0x0, 0x0, 0x0 }, // Row 3
{ MPC1(ZQCAL_START),   0x0, 0x0, 0x0 }, // Row 4
{ MPC2(ZQCAL_START),   0x0, 0x0, 0x0 }, // Row 5
{ MPC1(ZQCAL_START),   0x0, 0x0, 0x0 }, // Row 6
{ MPC2(ZQCAL_START),   0x0, 0x0, 0x0 }, // Row 7
{ MPC1(ZQCAL_START),   0x0, 0x0, 0x0 }, // Row 8
{ MPC2(ZQCAL_START),   0x0, 0x0, 0x0 }, // Row 9
{ MPC1(ZQCAL_START),   0x0, 0x0, 0x0 }, // Row 10
{ MPC2(ZQCAL_START),   0x0, 0x0, 0x0 }, // Row 11
{ MPC1(ZQCAL_START),   0x0, 0x0, 0x0 }, // Row 12
{ MPC2(ZQCAL_START),   0x0, 0x0, 0x0 }, // Row 13
{ MPC1(ZQCAL_START),   0x0, 0x0, 0x0 }, // Row 14
{ MPC2(ZQCAL_START),   0x0, 0x0, 0x0 } // Row 15
};

// CS,      Control, ODT,  CKE, Par
STATIC PatCadbProg1 MpcZQCalStartPattern1_4N[CADB_LINES] = {
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 0
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 1
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 2
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 3
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 4
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 5
  { 0x3FE,     0x3, 0x0, 0, 0 },  // Row 6
  { 0x3FE,     0x3, 0x0, 0, 0 },  // Row 7
  { 0x3FE,     0x3, 0x0, 0, 0 },  // Row 8
  { 0x3FE,     0x3, 0x0, 0, 0 },  // Row 9
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 10
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 11
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 12
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 13
  { 0x3FF,     0x3, 0x0, 0, 0 },  // Row 14
  { 0x3FF,     0x3, 0x0, 0, 0 }   // Row 15
};

STATIC PatCadbProg0 MpcStopDQSOscPattern0[CADB_LINES] =
{ { MPC1(Stop_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 0
{ MPC2(Stop_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 1
{ MPC1(Stop_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 2
{ MPC2(Stop_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 3
{ MPC1(Stop_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 4
{ MPC2(Stop_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 5
{ MPC1(Stop_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 6
{ MPC2(Stop_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 7
{ MPC1(Stop_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 8
{ MPC2(Stop_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 9
{ MPC1(Stop_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 10
{ MPC2(Stop_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 11
{ MPC1(Stop_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 12
{ MPC2(Stop_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 13
{ MPC1(Stop_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 14
{ MPC2(Stop_DQS_Osc),   0x0, 0x0, 0x0 } // Row 15
};

// CS,      Control, ODT,  CKE, Par
STATIC PatCadbProg1 MpcStopDQSOscPattern1_4N[CADB_LINES] =
{ { 0x3FF,     0X3, 0x0, 0, 0 }, // Row 0
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 1
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 2
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 3
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 4
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 5
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 6
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 7
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 8
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 9
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 10
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 11
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 12
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 13
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 14
{ 0x3FF,     0X3, 0x0, 0, 0 } // Row 15
};
STATIC PatCadbProg0 MpcStartDQSOscPattern0[CADB_LINES] =
{ { MPC1(Start_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 0
{ MPC2(Start_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 1
{ MPC1(Start_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 2
{ MPC2(Start_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 3
{ MPC1(Start_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 4
{ MPC2(Start_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 5
{ MPC1(Start_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 6
{ MPC2(Start_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 7
{ MPC1(Start_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 8
{ MPC2(Start_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 9
{ MPC1(Start_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 10
{ MPC2(Start_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 11
{ MPC1(Start_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 12
{ MPC2(Start_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 13
{ MPC1(Start_DQS_Osc),   0x0, 0x0, 0x0 }, // Row 14
{ MPC2(Start_DQS_Osc),   0x0, 0x0, 0x0 } // Row 15
};

// CS,      Control, ODT,  CKE, Par
STATIC PatCadbProg1 MpcStartDQSOscPattern1_4N[CADB_LINES] =
{ { 0x3FF,     0X3, 0x0, 0, 0 }, // Row 0
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 1
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 2
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 3
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 4
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 5
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 6
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 7
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 8
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 9
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 10
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 11
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 12
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 13
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 14
{ 0x3FF,     0X3, 0x0, 0, 0 } // Row 15
};

STATIC PatCadbProg0 MpcEnterPdaModePattern0[CADB_LINES] =
{ { MPC1(ENTER_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 0
{ MPC2(ENTER_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 1
{ MPC1(ENTER_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 2
{ MPC2(ENTER_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 3
{ MPC1(ENTER_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 4
{ MPC2(ENTER_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 5
{ MPC1(ENTER_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 6
{ MPC2(ENTER_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 7
{ MPC1(ENTER_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 8
{ MPC2(ENTER_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 9
{ MPC1(ENTER_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 10
{ MPC2(ENTER_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 11
{ MPC1(ENTER_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 12
{ MPC2(ENTER_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 13
{ MPC1(ENTER_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 14
{ MPC2(ENTER_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }  // Row 15
};

// CS,      Control, ODT,  CKE, Par
STATIC PatCadbProg1 MpcEnterPdaModePattern1_4N[CADB_LINES] =
{ { 0x3FF,     0X3, 0x0, 0, 0 }, // Row 0
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 1
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 2
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 3
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 4
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 5
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 6
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 7
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 8
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 9
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 10
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 11
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 12
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 13
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 14
{ 0x3FF,     0X3, 0x0, 0, 0 }  // Row 15
};
STATIC PatCadbProg0 MpcExitPdaModePattern0[CADB_LINES] =
{ { MPC1(EXIT_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 0
{ MPC2(EXIT_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 1
{ MPC1(EXIT_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 2
{ MPC2(EXIT_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 3
{ MPC1(EXIT_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 4
{ MPC2(EXIT_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 5
{ MPC1(EXIT_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 6
{ MPC2(EXIT_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 7
{ MPC1(EXIT_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 8
{ MPC2(EXIT_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 9
{ MPC1(EXIT_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 10
{ MPC2(EXIT_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 11
{ MPC1(EXIT_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 12
{ MPC2(EXIT_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 13
{ MPC1(EXIT_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }, // Row 14
{ MPC2(EXIT_PDA_ENUMERATE_PROGRAM_MODE),   0x0, 0x0, 0x0 }  // Row 15
};

STATIC PatCadbProg0 MpcGroupARttCkPattern0[CADB_LINES] =
{ { MPC1(GROUP_A_RTT_CK),   0x0, 0x0, 0x0 }, // Row 0
{ MPC2(GROUP_A_RTT_CK),   0x0, 0x0, 0x0 }, // Row 1
{ MPC1(GROUP_A_RTT_CK),   0x0, 0x0, 0x0 }, // Row 2
{ MPC2(GROUP_A_RTT_CK),   0x0, 0x0, 0x0 }, // Row 3
{ MPC1(GROUP_A_RTT_CK),   0x0, 0x0, 0x0 }, // Row 4
{ MPC2(GROUP_A_RTT_CK),   0x0, 0x0, 0x0 }, // Row 5
{ MPC1(GROUP_A_RTT_CK),   0x0, 0x0, 0x0 }, // Row 6
{ MPC2(GROUP_A_RTT_CK),   0x0, 0x0, 0x0 }, // Row 7
{ MPC1(GROUP_A_RTT_CK),   0x0, 0x0, 0x0 }, // Row 8
{ MPC2(GROUP_A_RTT_CK),   0x0, 0x0, 0x0 }, // Row 9
{ MPC1(GROUP_A_RTT_CK),   0x0, 0x0, 0x0 }, // Row 10
{ MPC2(GROUP_A_RTT_CK),   0x0, 0x0, 0x0 }, // Row 11
{ MPC1(GROUP_A_RTT_CK),   0x0, 0x0, 0x0 }, // Row 12
{ MPC2(GROUP_A_RTT_CK),   0x0, 0x0, 0x0 }, // Row 13
{ MPC1(GROUP_A_RTT_CK),   0x0, 0x0, 0x0 }, // Row 14
{ MPC2(GROUP_A_RTT_CK),   0x0, 0x0, 0x0 }  // Row 15
};


STATIC PatCadbProg0 MpcGroupBRttCkPattern0[CADB_LINES] =
{ { MPC1(GROUP_B_RTT_CK),   0x0, 0x0, 0x0 }, // Row 0
{ MPC2(GROUP_B_RTT_CK),   0x0, 0x0, 0x0 }, // Row 1
{ MPC1(GROUP_B_RTT_CK),   0x0, 0x0, 0x0 }, // Row 2
{ MPC2(GROUP_B_RTT_CK),   0x0, 0x0, 0x0 }, // Row 3
{ MPC1(GROUP_B_RTT_CK),   0x0, 0x0, 0x0 }, // Row 4
{ MPC2(GROUP_B_RTT_CK),   0x0, 0x0, 0x0 }, // Row 5
{ MPC1(GROUP_B_RTT_CK),   0x0, 0x0, 0x0 }, // Row 6
{ MPC2(GROUP_B_RTT_CK),   0x0, 0x0, 0x0 }, // Row 7
{ MPC1(GROUP_B_RTT_CK),   0x0, 0x0, 0x0 }, // Row 8
{ MPC2(GROUP_B_RTT_CK),   0x0, 0x0, 0x0 }, // Row 9
{ MPC1(GROUP_B_RTT_CK),   0x0, 0x0, 0x0 }, // Row 10
{ MPC2(GROUP_B_RTT_CK),   0x0, 0x0, 0x0 }, // Row 11
{ MPC1(GROUP_B_RTT_CK),   0x0, 0x0, 0x0 }, // Row 12
{ MPC2(GROUP_B_RTT_CK),   0x0, 0x0, 0x0 }, // Row 13
{ MPC1(GROUP_B_RTT_CK),   0x0, 0x0, 0x0 }, // Row 14
{ MPC2(GROUP_B_RTT_CK),   0x0, 0x0, 0x0 }  // Row 15
};

STATIC PatCadbProg0 MpcGroupARttCsPattern0[CADB_LINES] =
{ { MPC1(GROUP_A_RTT_CS),   0x0, 0x0, 0x0 }, // Row 0
{ MPC2(GROUP_A_RTT_CS),   0x0, 0x0, 0x0 }, // Row 1
{ MPC1(GROUP_A_RTT_CS),   0x0, 0x0, 0x0 }, // Row 2
{ MPC2(GROUP_A_RTT_CS),   0x0, 0x0, 0x0 }, // Row 3
{ MPC1(GROUP_A_RTT_CS),   0x0, 0x0, 0x0 }, // Row 4
{ MPC2(GROUP_A_RTT_CS),   0x0, 0x0, 0x0 }, // Row 5
{ MPC1(GROUP_A_RTT_CS),   0x0, 0x0, 0x0 }, // Row 6
{ MPC2(GROUP_A_RTT_CS),   0x0, 0x0, 0x0 }, // Row 7
{ MPC1(GROUP_A_RTT_CS),   0x0, 0x0, 0x0 }, // Row 8
{ MPC2(GROUP_A_RTT_CS),   0x0, 0x0, 0x0 }, // Row 9
{ MPC1(GROUP_A_RTT_CS),   0x0, 0x0, 0x0 }, // Row 10
{ MPC2(GROUP_A_RTT_CS),   0x0, 0x0, 0x0 }, // Row 11
{ MPC1(GROUP_A_RTT_CS),   0x0, 0x0, 0x0 }, // Row 12
{ MPC2(GROUP_A_RTT_CS),   0x0, 0x0, 0x0 }, // Row 13
{ MPC1(GROUP_A_RTT_CS),   0x0, 0x0, 0x0 }, // Row 14
{ MPC2(GROUP_A_RTT_CS),   0x0, 0x0, 0x0 }  // Row 15
};

STATIC PatCadbProg0 MpcGroupBRttCsPattern0[CADB_LINES] =
{ { MPC1(GROUP_B_RTT_CS ),   0x0, 0x0, 0x0 }, // Row 0
{ MPC2(GROUP_B_RTT_CS ),   0x0, 0x0, 0x0 }, // Row 1
{ MPC1(GROUP_B_RTT_CS ),   0x0, 0x0, 0x0 }, // Row 2
{ MPC2(GROUP_B_RTT_CS ),   0x0, 0x0, 0x0 }, // Row 3
{ MPC1(GROUP_B_RTT_CS ),   0x0, 0x0, 0x0 }, // Row 4
{ MPC2(GROUP_B_RTT_CS ),   0x0, 0x0, 0x0 }, // Row 5
{ MPC1(GROUP_B_RTT_CS ),   0x0, 0x0, 0x0 }, // Row 6
{ MPC2(GROUP_B_RTT_CS ),   0x0, 0x0, 0x0 }, // Row 7
{ MPC1(GROUP_B_RTT_CS ),   0x0, 0x0, 0x0 }, // Row 8
{ MPC2(GROUP_B_RTT_CS ),   0x0, 0x0, 0x0 }, // Row 9
{ MPC1(GROUP_B_RTT_CS ),   0x0, 0x0, 0x0 }, // Row 10
{ MPC2(GROUP_B_RTT_CS ),   0x0, 0x0, 0x0 }, // Row 11
{ MPC1(GROUP_B_RTT_CS ),   0x0, 0x0, 0x0 }, // Row 12
{ MPC2(GROUP_B_RTT_CS ),   0x0, 0x0, 0x0 }, // Row 13
{ MPC1(GROUP_B_RTT_CS ),   0x0, 0x0, 0x0 }, // Row 14
{ MPC2(GROUP_B_RTT_CS ),   0x0, 0x0, 0x0 }  // Row 15
};

STATIC PatCadbProg0 MpcGroupARttCaPattern0[CADB_LINES] =
{ { MPC1(GROUP_A_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 0
{ MPC2(GROUP_A_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 1
{ MPC1(GROUP_A_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 2
{ MPC2(GROUP_A_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 3
{ MPC1(GROUP_A_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 4
{ MPC2(GROUP_A_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 5
{ MPC1(GROUP_A_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 6
{ MPC2(GROUP_A_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 7
{ MPC1(GROUP_A_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 8
{ MPC2(GROUP_A_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 9
{ MPC1(GROUP_A_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 10
{ MPC2(GROUP_A_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 11
{ MPC1(GROUP_A_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 12
{ MPC2(GROUP_A_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 13
{ MPC1(GROUP_A_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 14
{ MPC2(GROUP_A_RTT_CA ),   0x0, 0x0, 0x0 }  // Row 15
};

STATIC PatCadbProg0 MpcGroupBRttCaPattern0[CADB_LINES] =
{ { MPC1(GROUP_B_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 0
{ MPC2(GROUP_B_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 1
{ MPC1(GROUP_B_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 2
{ MPC2(GROUP_B_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 3
{ MPC1(GROUP_B_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 4
{ MPC2(GROUP_B_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 5
{ MPC1(GROUP_B_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 6
{ MPC2(GROUP_B_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 7
{ MPC1(GROUP_B_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 8
{ MPC2(GROUP_B_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 9
{ MPC1(GROUP_B_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 10
{ MPC2(GROUP_B_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 11
{ MPC1(GROUP_B_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 12
{ MPC2(GROUP_B_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 13
{ MPC1(GROUP_B_RTT_CA ),   0x0, 0x0, 0x0 }, // Row 14
{ MPC2(GROUP_B_RTT_CA ),   0x0, 0x0, 0x0 }  // Row 15
};

STATIC PatCadbProg0 MpcDqsRttParkPattern0[CADB_LINES] =
{ { MPC1(DQS_RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 0
{ MPC2(DQS_RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 1
{ MPC1(DQS_RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 2
{ MPC2(DQS_RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 3
{ MPC1(DQS_RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 4
{ MPC2(DQS_RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 5
{ MPC1(DQS_RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 6
{ MPC2(DQS_RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 7
{ MPC1(DQS_RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 8
{ MPC2(DQS_RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 9
{ MPC1(DQS_RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 10
{ MPC2(DQS_RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 11
{ MPC1(DQS_RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 12
{ MPC2(DQS_RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 13
{ MPC1(DQS_RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 14
{ MPC2(DQS_RTT_PARK ),   0x0, 0x0, 0x0 }  // Row 15
};

STATIC PatCadbProg0 MpcRttParkPattern0[CADB_LINES] =
{ { MPC1(RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 0
{ MPC2(RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 1
{ MPC1(RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 2
{ MPC2(RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 3
{ MPC1(RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 4
{ MPC2(RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 5
{ MPC1(RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 6
{ MPC2(RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 7
{ MPC1(RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 8
{ MPC2(RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 9
{ MPC1(RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 10
{ MPC2(RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 11
{ MPC1(RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 12
{ MPC2(RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 13
{ MPC1(RTT_PARK ),   0x0, 0x0, 0x0 }, // Row 14
{ MPC2(RTT_PARK ),   0x0, 0x0, 0x0 }  // Row 15
};

STATIC PatCadbProg0 MpcApplyVrefcaRttCaCsCkPattern0[CADB_LINES] =
{ { MPC1(APPLY_VREFCA_RTT_CA_CS_CK ),   0x0, 0x0, 0x0 }, // Row 0
{ MPC2(APPLY_VREFCA_RTT_CA_CS_CK ),   0x0, 0x0, 0x0 }, // Row 1
{ MPC1(APPLY_VREFCA_RTT_CA_CS_CK ),   0x0, 0x0, 0x0 }, // Row 2
{ MPC2(APPLY_VREFCA_RTT_CA_CS_CK ),   0x0, 0x0, 0x0 }, // Row 3
{ MPC1(APPLY_VREFCA_RTT_CA_CS_CK ),   0x0, 0x0, 0x0 }, // Row 4
{ MPC2(APPLY_VREFCA_RTT_CA_CS_CK ),   0x0, 0x0, 0x0 }, // Row 5
{ MPC1(APPLY_VREFCA_RTT_CA_CS_CK ),   0x0, 0x0, 0x0 }, // Row 6
{ MPC2(APPLY_VREFCA_RTT_CA_CS_CK ),   0x0, 0x0, 0x0 }, // Row 7
{ MPC1(APPLY_VREFCA_RTT_CA_CS_CK ),   0x0, 0x0, 0x0 }, // Row 8
{ MPC2(APPLY_VREFCA_RTT_CA_CS_CK ),   0x0, 0x0, 0x0 }, // Row 9
{ MPC1(APPLY_VREFCA_RTT_CA_CS_CK ),   0x0, 0x0, 0x0 }, // Row 10
{ MPC2(APPLY_VREFCA_RTT_CA_CS_CK ),   0x0, 0x0, 0x0 }, // Row 11
{ MPC1(APPLY_VREFCA_RTT_CA_CS_CK ),   0x0, 0x0, 0x0 }, // Row 12
{ MPC2(APPLY_VREFCA_RTT_CA_CS_CK ),   0x0, 0x0, 0x0 }, // Row 13
{ MPC1(APPLY_VREFCA_RTT_CA_CS_CK ),   0x0, 0x0, 0x0 }, // Row 14
{ MPC2(APPLY_VREFCA_RTT_CA_CS_CK ),   0x0, 0x0, 0x0 }  // Row 15
};

STATIC PatCadbProg0 MpcDllResetPattern0[CADB_LINES] =
{ { MPC1(DLL_RESET ),   0x0, 0x0, 0x0 }, // Row 0
{ MPC2(DLL_RESET ),   0x0, 0x0, 0x0 }, // Row 1
{ MPC1(DLL_RESET ),   0x0, 0x0, 0x0 }, // Row 2
{ MPC2(DLL_RESET ),   0x0, 0x0, 0x0 }, // Row 3
{ MPC1(DLL_RESET ),   0x0, 0x0, 0x0 }, // Row 4
{ MPC2(DLL_RESET ),   0x0, 0x0, 0x0 }, // Row 5
{ MPC1(DLL_RESET ),   0x0, 0x0, 0x0 }, // Row 6
{ MPC2(DLL_RESET ),   0x0, 0x0, 0x0 }, // Row 7
{ MPC1(DLL_RESET ),   0x0, 0x0, 0x0 }, // Row 8
{ MPC2(DLL_RESET ),   0x0, 0x0, 0x0 }, // Row 9
{ MPC1(DLL_RESET ),   0x0, 0x0, 0x0 }, // Row 10
{ MPC2(DLL_RESET ),   0x0, 0x0, 0x0 }, // Row 11
{ MPC1(DLL_RESET ),   0x0, 0x0, 0x0 }, // Row 12
{ MPC2(DLL_RESET ),   0x0, 0x0, 0x0 }, // Row 13
{ MPC1(DLL_RESET ),   0x0, 0x0, 0x0 }, // Row 14
{ MPC2(DLL_RESET ),   0x0, 0x0, 0x0 }  // Row 15
};

// CS,      Control, ODT,  CKE, Par
STATIC PatCadbProg1 MpcSetRTTCAPattern1_4N[CADB_LINES] =
{ { 0x3FF,     0X3, 0x0, 0, 0 }, // Row 0
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 1
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 2
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 3
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 4
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 5
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 6
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 7
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 8
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 9
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 10
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 11
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 12
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 13
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 14
{ 0x3FF,     0X3, 0x0, 0, 0 } // Row 15
};

// CS,      Control, ODT,  CKE, Par
STATIC PatCadbProg1 MpcCADBPattern1_4N[CADB_LINES] =
{ { 0x3FF,    0X3, 0x0, 0, 0 }, // Row 0
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 1
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 2
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 3
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 4
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 5
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 6
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 7
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 8
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 9
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 10
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 11
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 12
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 13
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 14
{ 0x3FF,     0X3, 0x0, 0, 0 } // Row 15
};

// CS,      Control, ODT,  CKE, Par
STATIC PatCadbProg1 MpcCADBPattern1_4N_CAPT[CADB_LINES] =
{ { 0x3FF,    0X3, 0x0, 0, 0 }, // Row 0
{ 0x3FF,     0X3, 0x0, 0, 1 }, // Row 1
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 2
{ 0x3FF,     0X3, 0x0, 0, 1 }, // Row 3
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 4
{ 0x3FF,     0X3, 0x0, 0, 1 }, // Row 5
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 6
{ 0x3FE,     0X3, 0x0, 0, 1 }, // Row 7
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 8
{ 0x3FE,     0X3, 0x0, 0, 1 }, // Row 9
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 10
{ 0x3FF,     0X3, 0x0, 0, 1 }, // Row 11
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 12
{ 0x3FF,     0X3, 0x0, 0, 1 }, // Row 13
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 14
{ 0x3FF,     0X3, 0x0, 0, 1 } // Row 15
};

// CS,      Control, ODT,  CKE, Par
STATIC PatCadbProg1 MpcCADBPattern1_2N[CADB_LINES] =
{ { 0x3FF,    0X3, 0x0, 0, 0 }, // Row 0
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 1
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 2
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 3
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 4
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 5
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 6
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 7
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 8
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 9
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 10
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 11
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 12
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 13
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 14
{ 0x3FF,     0X3, 0x0, 0, 0 } // Row 15
};

// CS,      Control, ODT,  CKE, Par
STATIC PatCadbProg1 MpcCADBPattern1_1N[CADB_LINES] =
{ { 0x3FF,    0X3, 0x0, 0, 0 }, // Row 0
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 1
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 2
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 3
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 4
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 5
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 6
{ 0x3FE,     0X3, 0x0, 0, 0 }, // Row 7
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 8
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 9
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 10
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 11
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 12
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 13
{ 0x3FF,     0X3, 0x0, 0, 0 }, // Row 14
{ 0x3FF,     0X3, 0x0, 0, 0 } // Row 15
};
//
// CSTM Pattern
//
// addr,     cid, bank, pdatag
STATIC PatCadbProg0 CstmPattern0[CADB_LINES] =
{ { 0x3FFF, 0x0, 0x0, 0x0 }, // Row 0
{ 0x3FFF, 0x0, 0x0, 0x0 }, // Row 1
{ 0x3FFF, 0x0, 0x0, 0x0 }, // Row 2
{ 0x3FFF, 0x0, 0x0, 0x0 }, // Row 3
{ 0x3FFF, 0x0, 0x0, 0x0 }, // Row 4
{ 0x3FFF, 0x0, 0x0, 0x0 }, // Row 5
{ 0x3FFF, 0x0, 0x0, 0x0 }, // Row 6
{ 0x3FFF, 0x0, 0x0, 0x0 }, // Row 7
{ 0x3FFF, 0x0, 0x0, 0x0 }, // Row 8
{ 0x3FFF, 0x0, 0x0, 0x0 }, // Row 9
{ 0x3FFF, 0x0, 0x0, 0x0 }, // Row 10
{ 0x3FFF, 0x0, 0x0, 0x0 }, // Row 11
{ 0x3FFF, 0x0, 0x0, 0x0 }, // Row 12
{ 0x3FFF, 0x0, 0x0, 0x0 }, // Row 13
{ 0x3FFF, 0x0, 0x0, 0x0 }, // Row 14
{ 0x3FFF, 0x0, 0x0, 0x0 } // Row 15
};

// CS,      Control, ODT,  CKE, Par //DDR5_TODO. To check whether 3 is needed.
STATIC PatCadbProg1 CstmPattern1[CADB_LINES] =
{ { 0x3FE, 0X3, 0x0, 0, 0 }, // Row 0  3 is a legacy setting which was from HSX. Just leave it here. For non-activate command.
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 1
{ 0x3FE, 0X3, 0x0, 0, 0 }, // Row 2
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 3
{ 0x3FE, 0X3, 0x0, 0, 0 }, // Row 4
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 5
{ 0x3FE, 0X3, 0x0, 0, 0 }, // Row 6
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 7
{ 0x3FE, 0X3, 0x0, 0, 0 }, // Row 8
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 9
{ 0x3FE, 0X3, 0x0, 0, 0 }, // Row 10
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 11
{ 0x3FE, 0X3, 0x0, 0, 0 }, // Row 12
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 13
{ 0x3FE, 0X3, 0x0, 0, 0 }, // Row 14
{ 0x3FF, 0X3, 0x0, 0, 0 } // Row 15
};

//
// BCOMTM Pattern
//
// addr,     cid, bank, pdatag
STATIC PatCadbProg0 BcomPattern0[CADB_LINES] =
{ { 0xFFE5, 0x0, 0x0, 0x0 }, // Row 0
{ 0xFFED, 0x0, 0x0, 0x0 }, // Row 1
{ 0xFFF7, 0x0, 0x0, 0x0 }, // Row 2
{ 0xFFFF, 0x0, 0x0, 0x0 }, // Row 3
{ 0xFFF5, 0x0, 0x0, 0x0 }, // Row 4
{ 0xFFFD, 0x0, 0x0, 0x0 }, // Row 5
{ 0xFFE7, 0x0, 0x0, 0x0 }, // Row 6
{ 0xFFEF, 0x0, 0x0, 0x0 }, // Row 7
{ 0xFFFF, 0x0, 0x0, 0x0 }, // Row 8
{ 0xFFFF, 0x0, 0x0, 0x0 }, // Row 9
{ 0xFFFF, 0x0, 0x0, 0x0 }, // Row 10
{ 0xFFFF, 0x0, 0x0, 0x0 }, // Row 11
{ 0xFFFF, 0x0, 0x0, 0x0 }, // Row 12
{ 0xFFFF, 0x0, 0x0, 0x0 }, // Row 13
{ 0xFFFF, 0x0, 0x0, 0x0 }, // Row 14
{ 0xFFFF, 0x0, 0x0, 0x0 }  // Row 15
};

// CS,      Control, ODT,  CKE, Par
STATIC PatCadbProg1 BcomPattern1[CADB_LINES] =
{ { 0x3FE, 0X3, 0x0, 0, 0 }, // Row 0  3 is a legacy setting which was from HSX. Just leave it here. For non-activate command.
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 1
{ 0x3FE, 0X3, 0x0, 0, 0 }, // Row 2
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 3
{ 0x3FE, 0X3, 0x0, 0, 0 }, // Row 4
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 5
{ 0x3FE, 0X3, 0x0, 0, 0 }, // Row 6
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 7
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 8
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 9
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 10
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 11
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 12
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 13
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 14
{ 0x3FF, 0X3, 0x0, 0, 0 }  // Row 15
};
//
// CATM Pattern
//
// addr, cid, bank, pdatag
STATIC PatCadbProg0 CatmPattern0[CADB_LINES] = {
  { 0x1F80,   0x0, 0x0, 0x0 }, // Row 0
  { 0x3F01,   0x0, 0x0, 0x0 }, // Row 1
  { 0x20FE,   0x0, 0x0, 0x0 }, // Row 2
  { 0x007F,   0x0, 0x0, 0x0 }, // Row 3
  { 0x3F80,   0x0, 0x0, 0x0 }, // Row 4
  { 0x3F01,   0x0, 0x0, 0x0 }, // Row 5
  { 0x00FE,   0x0, 0x0, 0x0 }, // Row 6
  { 0x007F,   0x0, 0x0, 0x0 }, // Row 7
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 8
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 9
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 10
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 11
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 12
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 13
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 14
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 15
};

STATIC PatCadbProg0 CatmPatternSimple0[CADB_LINES] = {
  { 0x3FFF,   0x0, 0x0, 0x0 }, // Row 0
  { 0x3FFE,   0x0, 0x0, 0x0 }, // Row 1
  { 0x3FFF,   0x0, 0x0, 0x0 }, // Row 2
  { 0x3FFE,   0x0, 0x0, 0x0 }, // Row 3
  { 0x3FFE,   0x0, 0x0, 0x0 }, // Row 4
  { 0x3FFE,   0x0, 0x0, 0x0 }, // Row 5
  { 0x3FFE,   0x0, 0x0, 0x0 }, // Row 6
  { 0x3FFE,   0x0, 0x0, 0x0 }, // Row 7
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 8
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 9
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 10
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 11
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 12
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 13
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 14
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 15
};

STATIC PatCadbProg0 CatmParPatternSimple0[CADB_LINES] = {
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 0
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 1
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 2
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 3
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 4
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 5
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 6
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 7
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 8
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 9
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 10
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 11
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 12
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 13
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 14
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 15
};

// CS,      Control, ODT,  CKE, Par
STATIC PatCadbProg1 CatmParPattern1[CADB_LINES] =
{ { 0x3FE, 0X3, 0x0, 0, 3 }, // Row 0  3 is a legacy setting which was from HSX. Just leave it here. For non-activate command.
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 1
{ 0x3FE, 0X3, 0x0, 0, 3 }, // Row 2
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 3
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 4
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 5
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 6
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 7
{ 0x3FF, 0X3, 0x0, 0, 3 }, // Row 8
{ 0x3FF, 0X3, 0x0, 0, 3 }, // Row 9
{ 0x3FF, 0X3, 0x0, 0, 3 }, // Row 10
{ 0x3FF, 0X3, 0x0, 0, 3 }, // Row 11
{ 0x3FF, 0X3, 0x0, 0, 3 }, // Row 12
{ 0x3FF, 0X3, 0x0, 0, 3 }, // Row 13
{ 0x3FF, 0X3, 0x0, 0, 3 }, // Row 14
{ 0x3FF, 0X3, 0x0, 0, 3 } // Row 15
};

STATIC PatCadbProg0 DeselectPattern0Simple0[CADB_LINES] = {
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 0
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 1
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 2
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 3
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 4
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 5
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 6
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 7
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 8
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 9
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 10
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 11
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 12
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 13
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 14
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 15
};

// CS,      Control, ODT,  CKE, Par
STATIC PatCadbProg1 DeselctPattern1Simple1[CADB_LINES] =
{ { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 0  3 is a legacy setting which was from HSX. Just leave it here. For non-activate command.
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 1
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 2
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 3
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 4
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 5
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 6
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 7
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 8
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 9
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 10
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 11
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 12
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 13
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 14
{ 0x3FF, 0X3, 0x0, 0, 0 } // Row 15
};


// addr, cid, bank, pdatag
STATIC PatCadbProg0 QcatmPatternSimple0_1st[CADB_LINES] = {
  { 0xFFFE,   0x0, 0x0, 0x0 }, // Row 0
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 1
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 2
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 3
  { 0xFFFE,   0x0, 0x0, 0x0 }, // Row 4
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 5
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 6
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 7
  { 0xFFFE,   0x0, 0x0, 0x0 }, // Row 8
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 9
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 10
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 11
  { 0xFFFE,   0x0, 0x0, 0x0 }, // Row 12
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 13
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 14
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 15
};

// addr, cid, bank, pdatag
STATIC PatCadbProg0 QcatmPatternSimple0_2nd[CADB_LINES] = {
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 0
  { 0xFFFE,   0x0, 0x0, 0x0 }, // Row 1
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 2
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 3
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 4
  { 0xFFFE,   0x0, 0x0, 0x0 }, // Row 5
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 6
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 7
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 8
  { 0xFFFE,   0x0, 0x0, 0x0 }, // Row 9
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 10
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 11
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 12
  { 0xFFFE,   0x0, 0x0, 0x0 }, // Row 13
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 14
  { 0xFFFF,   0x0, 0x0, 0x0 }, // Row 15
};

// CS,      Control, ODT,  CKE, Par
STATIC PatCadbProg1 CatmPattern1[CADB_LINES] =
{ { 0x3FE, 0X3, 0x0, 0, 0 }, // Row 0  3 is a legacy setting which was from HSX. Just leave it here. For non-activate command.
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 1
{ 0x3FE, 0X3, 0x0, 0, 0 }, // Row 2
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 3
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 4
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 5
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 6
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 7
{ 0x3FF, 0X3, 0x0, 0, 3 }, // Row 8
{ 0x3FF, 0X3, 0x0, 0, 3 }, // Row 9
{ 0x3FF, 0X3, 0x0, 0, 3 }, // Row 10
{ 0x3FF, 0X3, 0x0, 0, 3 }, // Row 11
{ 0x3FF, 0X3, 0x0, 0, 3 }, // Row 12
{ 0x3FF, 0X3, 0x0, 0, 3 }, // Row 13
{ 0x3FF, 0X3, 0x0, 0, 3 }, // Row 14
{ 0x3FF, 0X3, 0x0, 0, 3 } // Row 15
};

// CS,      Control, ODT,  CKE, Par
STATIC PatCadbProg1 CatmPattern1_CAPT[CADB_LINES] =
{ { 0x3FE, 0X3, 0x0, 0, 0 }, // Row 0  3 is a legacy setting which was from HSX. Just leave it here. For non-activate command.
{ 0x3FF, 0X3, 0x0, 0, 1 }, // Row 1
{ 0x3FE, 0X3, 0x0, 0, 0 }, // Row 2
{ 0x3FF, 0X3, 0x0, 0, 1 }, // Row 3
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 4
{ 0x3FF, 0X3, 0x0, 0, 1 }, // Row 5
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 6
{ 0x3FF, 0X3, 0x0, 0, 1 }, // Row 7
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 8
{ 0x3FF, 0X3, 0x0, 0, 1 }, // Row 9
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 10
{ 0x3FF, 0X3, 0x0, 0, 1 }, // Row 11
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 12
{ 0x3FF, 0X3, 0x0, 0, 1 }, // Row 13
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 14
{ 0x3FF, 0X3, 0x0, 0, 1 } // Row 15
};



// CS,      Control, ODT,  CKE, Par
STATIC PatCadbProg1 QcatmPattern1_CAPT[CADB_LINES] =
{ { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 0  3 is a legacy setting which was from HSX. Just leave it here. For non-activate command.
{ 0x3FF, 0X3, 0x0, 0, 1 }, // Row 1
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 2
{ 0x3FF, 0X3, 0x0, 0, 1 }, // Row 3
{ 0x3FE, 0X3, 0x0, 0, 0 }, // Row 4
{ 0x3FF, 0X3, 0x0, 0, 1 }, // Row 5
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 6
{ 0x3FF, 0X3, 0x0, 0, 1 }, // Row 7
{ 0x3FE, 0X3, 0x0, 0, 0 }, // Row 8
{ 0x3FF, 0X3, 0x0, 0, 1 }, // Row 9
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 10
{ 0x3FF, 0X3, 0x0, 0, 1 }, // Row 11
{ 0x3FE, 0X3, 0x0, 0, 0 }, // Row 12
{ 0x3FF, 0X3, 0x0, 0, 1 }, // Row 13
{ 0x3FF, 0X3, 0x0, 0, 0 }, // Row 14
{ 0x3FF, 0X3, 0x0, 0, 1 } // Row 15
};

typedef union {
  struct {
    UINT8 Bit0 : 1;
    UINT8 Bit1 : 1;
    UINT8 Bit2 : 1;
    UINT8 Bit3 : 1;
    UINT8 Bit4 : 1;
    UINT8 Bit5 : 1;
    UINT8 Bit6 : 1;
    UINT8 Bit7 : 1;
  } Bits;
  UINT8 Data;
} BITS_IN_BYTE;

/**

  This function deswizzles CPGC error status relative to DQ bit lanes per byte.

  @param[in] CpgcError      - Dword of CPGC error status
  @param[in] SwapNibbles    - Flag to indicate if nibbles need to be swapped


**/
VOID
BrsCpgcErrorStatusDeswizzle (
  IN OUT  UINT32  *CpgcError,
  IN      UINT8   SwapNibbles
)
{
  UINT8   ByteIndex;
  UINT32  DqError;
  BITS_IN_BYTE DqByte;
  BITS_IN_BYTE CpgcByte;

  //
  //  CPGC bit    DRAM on 1Rx8   DQ at BRS pin (same as DIMM connector)
  //  0           5              4
  //  1           7              6
  //  2           6              5
  //  3           4              7
  //  4           1              0
  //  5           3              2
  //  6           2              1
  //  7           0              3
  //
  for (ByteIndex = 0; ByteIndex < sizeof (UINT32); ByteIndex++) {
    DqByte.Data = 0;
    CpgcByte.Data = *((UINT8 *)CpgcError + ByteIndex);

    if (SwapNibbles) {

      DqByte.Bits.Bit0 = CpgcByte.Bits.Bit0;
      DqByte.Bits.Bit2 = CpgcByte.Bits.Bit1;
      DqByte.Bits.Bit1 = CpgcByte.Bits.Bit2;
      DqByte.Bits.Bit3 = CpgcByte.Bits.Bit3;
      DqByte.Bits.Bit4 = CpgcByte.Bits.Bit4;
      DqByte.Bits.Bit6 = CpgcByte.Bits.Bit5;
      DqByte.Bits.Bit5 = CpgcByte.Bits.Bit6;
      DqByte.Bits.Bit7 = CpgcByte.Bits.Bit7;

    } else {

      DqByte.Bits.Bit4 = CpgcByte.Bits.Bit0;
      DqByte.Bits.Bit6 = CpgcByte.Bits.Bit1;
      DqByte.Bits.Bit5 = CpgcByte.Bits.Bit2;
      DqByte.Bits.Bit7 = CpgcByte.Bits.Bit3;
      DqByte.Bits.Bit0 = CpgcByte.Bits.Bit4;
      DqByte.Bits.Bit2 = CpgcByte.Bits.Bit5;
      DqByte.Bits.Bit1 = CpgcByte.Bits.Bit6;
      DqByte.Bits.Bit3 = CpgcByte.Bits.Bit7;
    }

    *((UINT8 *)&DqError + ByteIndex) = DqByte.Data;

  }

  *CpgcError = DqError;

  return;

} //BrsCpgcErrorStatusDeswizzle


/**

Collect DDR advanced command CPGC error status

@param[in]  Host                - Pointer to sysHost
@param[in]  Socket              - Socket to train
@param[in]  DdrChEnMap          - 4-bit bit mask of ddr channels to test - (1 = active channel)
@param[out] CpgcErrorStatus     - error status for each channel

@retval n/a


**/
VOID
CpgcAdvTrainingErrorStatusDdr5 (
  IN     PSYSHOST              Host,
  IN     UINT8                 Socket,
  IN     UINT32                DdrChEnMap,
  IN OUT CPGC_ERROR_STATUS CpgcErrorStatus[MAX_CH][SUB_CH]
  )
{
  UINT8  Ch;
  UINT8  SubCh = 0;
  UINT8  ChannelLocal = 0;
  UINT8  MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    //
    // skip inactive channels
    //
    if (!(DdrChEnMap & (1 << Ch))) {
      continue;
    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      ChannelLocal = Ch;
      GetChannelMapping (Ch, SubCh, &ChannelLocal);

      CpgcErrorStatus[Ch][SubCh].cpgcErrDat0S = MemReadPciCfgEp (Socket, ChannelLocal, CPGC_ERR_DATA0_S_MCDDC_CTL_REG);
      CpgcErrorStatus[Ch][SubCh].cpgcErrDat2S = MemReadPciCfgEp (Socket, ChannelLocal, CPGC_ERR_DATA2_S_MCDDC_CTL_REG);
      if (Host->nvram.mem.eccEn) {
        CpgcErrorStatus[Ch][SubCh].cpgcErrEccS = MemReadPciCfgEp (Socket, ChannelLocal, CPGC_ERR_ECC_S_MCDDC_CTL_REG);
      } else {
        CpgcErrorStatus[Ch][SubCh].cpgcErrEccS = 0;
      }

      if (IsBrsPresent (Host, Socket)) {
        //
        // HSD-ES: 1706873035
        //
        BrsCpgcErrorStatusDeswizzle (&CpgcErrorStatus[Ch][SubCh].cpgcErrDat0S, 0);
        BrsCpgcErrorStatusDeswizzle (&CpgcErrorStatus[Ch][SubCh].cpgcErrDat2S, 0);
        if (SubCh == 1) {
          BrsCpgcErrorStatusDeswizzle (&CpgcErrorStatus[Ch][SubCh].cpgcErrEccS, 1);
        } else {
          BrsCpgcErrorStatusDeswizzle (&CpgcErrorStatus[Ch][SubCh].cpgcErrEccS, 0);
        }
        //
        // Account for CPGC mapping on BRS
        // Move byte 8 into byte 4 position
        //
        CpgcErrorStatus[Ch][SubCh].cpgcErrDat1S = CpgcErrorStatus[Ch][SubCh].cpgcErrEccS & 0xFF;
        CpgcErrorStatus[Ch][SubCh].cpgcErrDat3S = (CpgcErrorStatus[Ch][SubCh].cpgcErrEccS >> 8) & 0xFF;
      }

    }
  }
  return;

} // CpgcAdvTrainingErrorStatusDdr5


/**

 Get the CPGC errors in [SubCh][Ch] format

 @param[in]   Host        Pointer to syshost
 @param[in]   Socket      Current socket under test (0-based)
 @param[in]   DdrChEnMap  One hot variable to indicate if an specific channel has to be executed
 @param[out]  CpgcErrorStatus Pointer array to return the CPGC results

  @retval     MRC_STATUS_SUCCESS      When function completes execution without issues

**/
MRC_STATUS
CpgcAdvTrainingErrorStatusAdapter (
  IN        PSYSHOST          Host,
  IN        UINT8             Socket,
  IN        UINT32            DdrChEnMap,
      OUT   CPGC_ERROR_STATUS CpgcErrorStatus[SUB_CH][MAX_CH]
  )
{
  UINT8  Ch;
  UINT8  SubCh;
  UINT8  MaxChDdr;
  CPGC_ERROR_STATUS CpgcErrorStatusLocal[MAX_CH][SUB_CH];

  MaxChDdr = GetMaxChDdr ();

  CpgcAdvTrainingErrorStatusDdr5 (Host, Socket, DdrChEnMap, CpgcErrorStatusLocal);

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    //
    // Skip Inactive Channels
    //
    if (!(DdrChEnMap & (1 << Ch))) {
      continue;
    }

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

      CpgcErrorStatus[SubCh][Ch] = CpgcErrorStatusLocal[Ch][SubCh];

    } // SubCh loop ...

  } // Ch loop ...

  return MRC_STATUS_SUCCESS;

}


/**

This function writes MPC Command by CADB in BRS

@param Host            - include all the MRC data
@param Socket          - Memory Controller
@param Ch              - Ch to send command to
@param Dimm            - Dimm to send command to
@param Subchannelmask  - sub channel mask
@param Rank            - CS to send the command to
@param Opcode           - Opcode to use
@param Timingmode       - Timingmode to use. Support MPC_1N, MPC_2N, MPC_4N.

@retval N/A

**/
VOID
WriteCadbMpcCmd (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Subchannelmask,
  IN UINT8     Rank,
  IN UINT16    Opcode,
  IN UINT8     Timingmode,
  IN BOOLEAN   PassThroughMode
  )
{
  UINT32                                        i;
  UINT8                                         ChBitmask = 0;
  UINT8                                         ChannelLocal = 0;
  UINT8                                         ChannelLocal2 = 0;
  UINT8                                         SubChLocal = 0;
  UINT8                                         SubCh = 0;
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT              CpgcPatCadbCtlReg;
  struct                                        channelNvram (*ChannelNvList) [MAX_CH];
  PatCadbProg0                                  MpcCATrainingModeEntryPattern0Local[CADB_LINES];
  PatCadbProg0                                  MpcCATrainingModeExitPattern0Local[CADB_LINES];
  PatCadbProg0                                  MpcCSTrainingModeEntryPattern0Local[CADB_LINES];
  PatCadbProg0                                  MpcCSTrainingModeExitPattern0Local[CADB_LINES];
  PatCadbProg0                                  MpcDllResetPattern0Local[CADB_LINES];
  PatCadbProg0                                  MpcZQCalLatchPattern0Local[CADB_LINES];
  PatCadbProg0                                  MpcZQCalStartPattern0Local[CADB_LINES];
  PatCadbProg0                                  MpcStopDQSOscPattern0Local[CADB_LINES];
  PatCadbProg0                                  MpcStartDQSOscPattern0Local[CADB_LINES];
  PatCadbProg0                                  MpcSetRTTCAPattern0Local[CADB_LINES];
  PatCadbProg0                                  MpcEnterPdaModePattern0Local[CADB_LINES];
  PatCadbProg0                                  MpcExitPdaModePattern0Local[CADB_LINES];
  PatCadbProg0                                  MpcPattern0Local[CADB_LINES];
  PatCadbProg1                                  MpcCADBPattern1Local[CADB_LINES];
  UINT32                                        CadbMuxPattern[CADB_MUX_MAX];
  UINT8                                         CadbMuxCtl[CADB_MUX_MAX];
  CPGC_GLOBAL_TRAINING_SETUP                    CpgcGblTrainingSetup;
  CPGC_DDRT_MIX_CFG_MC_MAIN_STRUCT              CpgcMixCfg;
  CPGC_MISCODTCTL_MCDDC_CTL_STRUCT              CpgcMiscOdtCtlReg;
  UINT8                                         ChBitmaskLocal;
  UINT8                                         ChannelModeStore;
  UINT8                                         LogicalRank = GetPhyRank (Dimm, Rank);

  ChannelModeStore = Host->ChannelModeOperation;

  OverrideChannelModeTo80bMode ();
  RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "WriteCadbMpcCmd10nmBrsDdr5 starts\n");

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {

    if (Host->DdrioUltSupport) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "ULT MPC Command  Opcode 0x%04X, timing Mode %04d \n", Opcode, Timingmode);
      CteDdrCmd ("mpc", 1 << Ch, Subchannelmask, Rank, Opcode, Timingmode);
      return;
    }

  }

  ChannelNvList = GetChannelNvList (Host, Socket);
  ChannelLocal = Ch;
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & (Subchannelmask)) == 0) {
      continue;
    }
    GetChannelMapping (Ch, SubCh, &ChannelLocal);

    // Temporarily disable 3DS Mode for MPC commands
    if (((*ChannelNvList) [ChannelLocal].encodedCSMode == 2) && (Host->nvram.mem.threeDsModeDisabled == 0)) {
      Toggle3DSMode (Host, Socket, ChannelLocal, 0);
    }

    CadbMuxPattern[0] = CADB_MUX_PATTERN0;
    CadbMuxPattern[1] = CADB_MUX_PATTERN1;
    CadbMuxPattern[2] = CADB_MUX_PATTERN2;
    CadbMuxPattern[3] = CADB_MUX_PATTERN3;

    CadbMuxCtl[0] = CADB_MUX_CTRL_BUFFER;
    CadbMuxCtl[1] = CADB_MUX_CTRL_BUFFER;
    CadbMuxCtl[2] = CADB_MUX_CTRL_BUFFER;
    CadbMuxCtl[3] = CADB_MUX_CTRL_BUFFER;

    if ((*ChannelNvList) [Ch].enabled == 0) {
      return;
    }
    //
    // for brs, the two sub channel in one mc will have the same cpgc setting.
    //
    CpgcMiscOdtCtlReg.Data = MemReadPciCfgEp (Socket, ChannelLocal, CPGC_MISCODTCTL_MCDDC_CTL_REG);

    ProgramCadbMux (Socket, ChannelLocal, NO_SUBCH, CadbMuxPattern, CadbMuxCtl);

    for (i = 0; i < CADB_LINES; i++) {
      CopyMem (&MpcCATrainingModeEntryPattern0Local[i], &MpcCATrainingModeEntryPattern0[i], sizeof (PatCadbProg0));
      CopyMem (&MpcCATrainingModeExitPattern0Local[i], &MpcCATrainingModeExitPattern0[i], sizeof (PatCadbProg0));
      CopyMem (&MpcCSTrainingModeEntryPattern0Local[i],&MpcCSTrainingModeEntryPattern0[i], sizeof (PatCadbProg0));
      CopyMem (&MpcCSTrainingModeExitPattern0Local[i], &MpcCSTrainingModeExitPattern0[i], sizeof (PatCadbProg0));
      CopyMem (&MpcDllResetPattern0Local[i], &MpcDllResetPattern0[i], sizeof (PatCadbProg0));
      CopyMem (&MpcZQCalLatchPattern0Local[i], &MpcZQCalLatchPattern0[i], sizeof (PatCadbProg0));
      CopyMem (&MpcZQCalStartPattern0Local[i], &MpcZQCalStartPattern0[i], sizeof (PatCadbProg0));
      CopyMem (&MpcStopDQSOscPattern0Local[i], &MpcStopDQSOscPattern0[i], sizeof (PatCadbProg0));
      CopyMem (&MpcStartDQSOscPattern0Local[i], &MpcStartDQSOscPattern0[i], sizeof (PatCadbProg0));
      CopyMem (&MpcSetRTTCAPattern0Local[i], &MpcGroupARttCaPattern0[i], sizeof (PatCadbProg0));
      CopyMem (&MpcEnterPdaModePattern0Local[i], &MpcEnterPdaModePattern0[i], sizeof (PatCadbProg0));
      CopyMem (&MpcExitPdaModePattern0Local[i], &MpcExitPdaModePattern0[i], sizeof (PatCadbProg0));
      if (Timingmode == TIMING_1N) {
        CopyMem (&MpcCADBPattern1Local[i], &MpcCADBPattern1_1N[i], sizeof (PatCadbProg1));
      } else if (Timingmode == TIMING_2N) {
        CopyMem (&MpcCADBPattern1Local[i], &MpcCADBPattern1_2N[i], sizeof (PatCadbProg1));
      } else if ((Timingmode == TIMING_4N) && (PassThroughMode)) {
        CopyMem (&MpcCADBPattern1Local[i], &MpcCADBPattern1_4N_CAPT[i], sizeof (PatCadbProg1));
      } else if ((Timingmode == TIMING_4N) && (!PassThroughMode)) {
        CopyMem (&MpcCADBPattern1Local[i], &MpcCADBPattern1_4N[i], sizeof (PatCadbProg1));
      } else {
        RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
          "MPC command timing Mode  %d is not supported.\n", Timingmode);
      }
    }
    if (((Opcode&0xF0) == 0x60)) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "PDA Enumerate ID or PDA Select ID with Opcode%x\n", Opcode);
      for (i = 4; i < CADB_LINES; i++) {
        if ((i%2) == 0) {
          MpcPattern0Local[i].address = MPC1 (Opcode);
        } else {
          MpcPattern0Local[i].address = MPC2 (Opcode);
        }
        MpcPattern0Local[i].cid = 0;
        MpcPattern0Local[i].bank = 0;
        MpcPattern0Local[i].pdatag = 0;
      }
      MpcPattern0Local[0].address = 0x002D;
      MpcPattern0Local[0].cid = 0;
      MpcPattern0Local[0].bank = 0;
      MpcPattern0Local[0].pdatag = 0;

      MpcPattern0Local[1].address = 0x0000;
      MpcPattern0Local[1].cid = 0;
      MpcPattern0Local[1].bank = 0;
      MpcPattern0Local[1].pdatag = 0;

      MpcPattern0Local[2].address = 01;
      MpcPattern0Local[2].cid = 0;
      MpcPattern0Local[2].bank = 0;
      MpcPattern0Local[2].pdatag = 0;

      MpcPattern0Local[3].address = 0x1E;
      MpcPattern0Local[3].cid = 0;
      MpcPattern0Local[3].bank = 0;
      MpcPattern0Local[3].pdatag = 0;

      FixupCS (LogicalRank, 0, MpcPattern0Local, MpcCADBPattern1Local, (*ChannelNvList) [ChannelLocal].encodedCSMode);
      ProgramCADBBrs (Host, Socket, ChannelLocal, 0, CADB_LINES, MpcPattern0Local, MpcCADBPattern1Local);
    } else if (((Opcode&0xF0) == 0x70)) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "PDA Select ID with Opcode%x\n", Opcode);
      for (i = 0; i < CADB_LINES; i++) {
        if ((i%2) == 0) {
          MpcPattern0Local[i].address = MPC1 (Opcode);
        } else {
          MpcPattern0Local[i].address = MPC2 (Opcode);
        }
        MpcPattern0Local[i].cid = 0;
        MpcPattern0Local[i].bank = 0;
        MpcPattern0Local[i].pdatag = 0;
      }
      FixupCS (LogicalRank, 0, MpcPattern0Local, MpcCADBPattern1Local, (*ChannelNvList) [ChannelLocal].encodedCSMode);
      ProgramCADBBrs (Host, Socket, ChannelLocal, 0, CADB_LINES, MpcPattern0Local, MpcCADBPattern1Local);
    } else {

      for (i = 0; i < CADB_LINES; i++) {
        if ((i%2) == 0) {
          MpcPattern0Local[i].address = MPC1 (Opcode);
        } else {
          MpcPattern0Local[i].address = MPC2 (Opcode);
        }
        MpcPattern0Local[i].cid = 0;
        MpcPattern0Local[i].bank = 0;
        MpcPattern0Local[i].pdatag = 0;
      }
      FixupCS (LogicalRank, 0, MpcPattern0Local, MpcCADBPattern1Local, (*ChannelNvList) [ChannelLocal].encodedCSMode);
      ProgramCADBBrs (Host, Socket, ChannelLocal, 0, CADB_LINES, MpcPattern0Local, MpcCADBPattern1Local);
    }
    //
    // Delay if this is a CTE build, otherwise do nothing.
    //
    TrainingDelay (Host, 1600, 32);
    CpgcGblTrainingSetup.seqLoopCnt = 1;                                // test will end after one loop through the sub-sequence
    CpgcGblTrainingSetup.startDelay = 0;                                // Delay start
    CpgcGblTrainingSetup.addrUpdRateMode = CPGC_UPDATE_BY_CL;                // Update based on the # of read and write cacheline
    CpgcGblTrainingSetup.rwMode = CPGC_BASE_WRITE_SUBSEQ;           // Base Read or write - subseq0
    CpgcGblTrainingSetup.numCacheLines = 2;                                // Number of Cachelines (2^N)
    CpgcGblTrainingSetup.burstSeparation = 0;                                // Burst separation based on training step
    CpgcGblTrainingSetup.errEnChunk = CPGC_ERR_MONITOR_NO_CHUNK;        // Do not monitor errors on chunks
    CpgcGblTrainingSetup.errEnCl = CPGC_ERR_MONITOR_NO_CL;           // Do not monitor errors on cacheline
    CpgcGblTrainingSetup.stopOnErrCtl = CPGC_ERR_NEVER_STOP_ON_ERR;       // Do not Stop on all lane error
    CpgcGblTrainingSetup.subseqEndPtr = 1;                                // subseq end pointer
    CpgcGblTrainingSetup.useSubSeq1 = CPGC_BASE_READ_SUBSEQ;            // use sub seq1 (base read)
    CpgcGblTrainingSetup.cpgcLinkTrain = 0;                                // do not use cpgc link train
    CpgcGblTrainingSetup.back2backWrEn = 0;                                // Enable b2b writes
    CpgcGblTrainingSetup.mprControl = 0;                                // MPR disable
    CpgcGblTrainingSetup.refMask = 0xFF;                             // disable cpgc self refresh
    CpgcGblTrainingSetup.zqMask = 0xFF;                             // disable cpgc zq
    CpgcGblTrainingSetup.x8Mode = 0;
    CpgcGlobalTrainingSetup (Host, Socket, ChannelLocal, &CpgcGblTrainingSetup);
    //
    // Enable CADB
    //
    for (SubChLocal = 0; SubChLocal < SUB_CH; SubChLocal++) {
      GetChannelMapping (Ch, SubChLocal, &ChannelLocal2);
      CpgcPatCadbCtlReg.Data = MemReadPciCfgEp (Socket, ChannelLocal2, CPGC_PATCADBCTL_MCDDC_CTL_REG);
      CpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 0;
      CpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 1;
      CpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 1;
      CpgcPatCadbCtlReg.Bits.enable_twopasscadb_always_on = 0;
      CpgcPatCadbCtlReg.Bits.enable_cadb_parityntiming = 0;
      MemWritePciCfgEp (Socket, ChannelLocal2, CPGC_PATCADBCTL_MCDDC_CTL_REG, CpgcPatCadbCtlReg.Data);

      CpgcMixCfg.Data = MemReadPciCfgEp (Socket, ChannelLocal2, CPGC_DDRT_MIX_CFG_MC_MAIN_REG);
      CpgcMixCfg.Bits.enable_twopasscadb_always_on_shadow = 0;
      MemWritePciCfgEp (Socket, ChannelLocal2, CPGC_DDRT_MIX_CFG_MC_MAIN_REG, CpgcMixCfg.Data);
    }
    //
    // Delay if this is a CTE build, otherwise do nothing.
    //
    TrainingDelay (Host, 1600, 32);

    // Start the test and poll the test done.
    //
    ChBitmask |= 1 << Ch;
    if (Subchannelmask == TWO_SUBCH_MASK) {
      ChBitmaskLocal = (Subchannelmask << Ch);
      ExecuteCmdSignalTest (Host, Socket, ChBitmaskLocal, NO_SUBCH);
    } else {
      if (Subchannelmask == SUB_CH_A_MASK) {
        SubChLocal = SUB_CH_A;
      } else if (Subchannelmask == SUB_CH_B_MASK) {
        SubChLocal = SUB_CH_B;
      } else {
        SubChLocal = 0;
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
          "Fail: Subchannelmask value 0x%04X is not valid\n", Subchannelmask);
      }
      ExecuteCmdSignalTest (Host, Socket, ChBitmask, SubChLocal);
    }
    //
    // Re-Enable 3DS Mode
    //
    if (((*ChannelNvList) [ChannelLocal].encodedCSMode == 2) && (Host->nvram.mem.threeDsModeDisabled == 0)) {
      Toggle3DSMode (Host, Socket, ChannelLocal, 1);
    }
    //
    // Disable CADB one pass and two pass always on.
    //
    for (SubChLocal = 0; SubChLocal < SUB_CH; SubChLocal++) {
      GetChannelMapping (Ch, SubChLocal, &ChannelLocal2);
      CpgcPatCadbCtlReg.Data = MemReadPciCfgEp (Socket, ChannelLocal2, CPGC_PATCADBCTL_MCDDC_CTL_REG);
      CpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 0;
      CpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 0;
      CpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 0;
      CpgcPatCadbCtlReg.Bits.enable_twopasscadb_always_on = 0;
      CpgcPatCadbCtlReg.Bits.enable_cadb_parityntiming = 0;
      MemWritePciCfgEp (Socket, ChannelLocal2, CPGC_PATCADBCTL_MCDDC_CTL_REG, CpgcPatCadbCtlReg.Data);

      CpgcMixCfg.Data = MemReadPciCfgEp (Socket, ChannelLocal2, CPGC_DDRT_MIX_CFG_MC_MAIN_REG);
      CpgcMixCfg.Bits.enable_twopasscadb_always_on_shadow = 0;
      MemWritePciCfgEp (Socket, ChannelLocal2, CPGC_DDRT_MIX_CFG_MC_MAIN_REG, CpgcMixCfg.Data);
    }
    //
    // Program it as deselect pattern.
    //
    CpgcAdvCmdParity (Host, Socket, ChannelLocal, GetPhyRank (Dimm, Rank), 0, DESELECT, NormalCadbSetup);
    MemWritePciCfgEp (Socket, ChannelLocal, CPGC_MISCODTCTL_MCDDC_CTL_REG, CpgcMiscOdtCtlReg.Data);
  }
  Host->ChannelModeOperation = ChannelModeStore;
  return;

} // WriteCadbMpcCmd

/**

This function writes Vref CS/CA Command by CADB

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number
  @param[in] Ch              - Ch to send command to
  @param[in] Dimm            - Dimm to send command to
  @param[in] Subchannelmask  - Sub channel mask
  @param[in] Rank            - CS to send the command to
  @param[in] Opcode          - Opcode to use
  @param[in] Timingmode      - Supports TIMING_SINGLE_CS, TIMING_MULTI_CS
  @param[in] PassThroughMode - PassThrough mode or not
  @param[in] CS/CA Mode      - CA or CS command

@retval N/A

**/
VOID
WriteCadbVrefCsCaCmd (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Subchannelmask,
  IN UINT8     Rank,
  IN UINT16    Opcode,
  IN UINT8     Timingmode,
  IN BOOLEAN   PassThroughMode,
  IN UINT8     CsCaMode
  )
{
  UINT32                                        Line;
  UINT8                                         ChBitmask = 0;
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT              CpgcPatCadbCtlReg;
  struct                                        channelNvram (*ChannelNvList) [MAX_CH];
  PatCadbProg0                                  VrefCaCADBPattern0[CADB_LINES];
  UINT32                                        CadbMuxPattern[CADB_MUX_MAX];
  UINT8                                         CadbMuxCtl[CADB_MUX_MAX];
  CPGC_GLOBAL_TRAINING_SETUP                    CpgcGblTrainingSetup;
  CPGC_DDRT_MIX_CFG_MC_MAIN_STRUCT              CpgcMixCfg;
  UINT8                                         ChannelLocal = 0;
  UINT8                                         ChannelLocal2 = 0;
  UINT8                                         SubChLocal = 0;
  CPGC_MISCODTCTL_MCDDC_CTL_STRUCT              CpgcMiscOdtCtlReg;
  UINT8                                         SubCh = 0;
  // CS,      Control, ODT,  CKE, Par
  PatCadbProg1 VrefCaCADBPattern1_4N[CADB_LINES] =
  { { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 0
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 1
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 2
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 3
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 4
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 5
  { 0x3FE, 0X3, 0x0, 0, 0 }, // Row 6
  { 0x3FE, 0X3, 0x0, 0, 0 }, // Row 7
  { 0x3FE, 0X3, 0x0, 0, 0 }, // Row 8
  { 0x3FE, 0X3, 0x0, 0, 0 }, // Row 9
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 10
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 11
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 12
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 13
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 14
  { 0x3FF, 0X3, 0x0, 0, 0 } // Row 15
  };

  // CS,      Control, ODT,  CKE, Par
  PatCadbProg1 VrefCaCADBPattern1_4N_CAPT[CADB_LINES] =
  { { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 0
  { 0x3FF, 0X3, 0x0, 0, 1 }, // Row 1
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 2
  { 0x3FF, 0X3, 0x0, 0, 1 }, // Row 3
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 4
  { 0x3FF, 0X3, 0x0, 0, 1 }, // Row 5
  { 0x3FE, 0X3, 0x0, 0, 0 }, // Row 6
  { 0x3FE, 0X3, 0x0, 0, 1 }, // Row 7
  { 0x3FE, 0X3, 0x0, 0, 0 }, // Row 8
  { 0x3FE, 0X3, 0x0, 0, 1 }, // Row 9
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 10
  { 0x3FF, 0X3, 0x0, 0, 1 }, // Row 11
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 12
  { 0x3FF, 0X3, 0x0, 0, 1 }, // Row 13
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 14
  { 0x3FF, 0X3, 0x0, 0, 1 } // Row 15
  };

  // CS,      Control, ODT,  CKE, Par
  PatCadbProg1 VrefCaCADBPattern1_1N[CADB_LINES] =
  { { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 0
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 1
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 2
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 3
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 4
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 5
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 6
  { 0x3FE, 0X3, 0x0, 0, 0 }, // Row 7
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 8
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 9
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 10
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 11
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 12
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 13
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 14
  { 0x3FF, 0X3, 0x0, 0, 0 } // Row 15
  };
  UINT8 ChannelModeStore;
  UINT8 LogicalRank =  GetPhyRank (Dimm, Rank);
  ChannelModeStore = Host->ChannelModeOperation;

  OverrideChannelModeTo80bMode ();

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "WriteCadbVrefCsCaCmd starts\n");
  ChannelNvList = GetChannelNvList (Host, Socket);

  ChannelLocal = Ch;
  if (Subchannelmask == SUB_CH_A_MASK) {
    SubCh = SUB_CH_A;
  } else if (Subchannelmask == SUB_CH_B_MASK) {
    SubCh = SUB_CH_B;
  } else if (Subchannelmask != TWO_SUBCH_MASK) {
    SubCh = SUB_CH_A;
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "Fail: Subchannelmask value 0x%04X is not valid\n", Subchannelmask);
  }
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & (Subchannelmask)) == 0) {
      continue;
    }
    GetChannelMapping (Ch, SubCh, &ChannelLocal);
    if (FeaturePcdGet (PcdCteBuild) == TRUE) {

      if (Host->DdrioUltSupport) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "ULT VrefCA Command  data 0x%04X, timing mode %04d \n", Opcode, Timingmode);
        if (CsCaMode == CA_MODE) {
          if (Timingmode == TIMING_SINGLE_CS) {
            CteDdrCmd ("vrefca", 1 << Ch, Subchannelmask, Rank, Opcode, TIMING_1N);
          } else {
            CteDdrCmd ("vrefca", 1 << Ch, Subchannelmask, Rank, Opcode, TIMING_4N);
          }
        } else {
          if (Timingmode == TIMING_SINGLE_CS) {
            CteDdrCmd ("vrefcs", 1 << Ch, Subchannelmask, Rank, Opcode, TIMING_1N);
          } else {
            CteDdrCmd ("vrefcs", 1 << Ch, Subchannelmask, Rank, Opcode, TIMING_4N);
          }
        }
        return;
      }

    }

    if ((*ChannelNvList) [Ch].enabled == 0) {
      return;
    }

    CadbMuxPattern[0] = CADB_MUX_PATTERN0;
    CadbMuxPattern[1] = CADB_MUX_PATTERN1;
    CadbMuxPattern[2] = CADB_MUX_PATTERN2;
    CadbMuxPattern[3] = CADB_MUX_PATTERN3;

    CadbMuxCtl[0] = CADB_MUX_CTRL_BUFFER;
    CadbMuxCtl[1] = CADB_MUX_CTRL_BUFFER;
    CadbMuxCtl[2] = CADB_MUX_CTRL_BUFFER;
    CadbMuxCtl[3] = CADB_MUX_CTRL_BUFFER;

    //
    // for brs, the two sub channel in one mc will have the same cpgc setting.
    //
    CpgcMiscOdtCtlReg.Data = MemReadPciCfgEp (Socket, ChannelLocal, CPGC_MISCODTCTL_MCDDC_CTL_REG);

    ProgramCadbMux (Socket, ChannelLocal, NO_SUBCH, CadbMuxPattern, CadbMuxCtl);

  for (Line = 0; Line < CADB_LINES; Line++) {
    if (CsCaMode == CA_MODE) {
      if ((Line % 2) == 0) {
        VrefCaCADBPattern0[Line].address = VrefCA1 (Opcode);
      } else {
        VrefCaCADBPattern0[Line].address = VrefCA2 (Opcode);
      }
    } else {
      if ((Line % 2) == 0) {
        VrefCaCADBPattern0[Line].address = VrefCS1 (Opcode);
      } else {
        VrefCaCADBPattern0[Line].address = VrefCS2 (Opcode);
      }
    }

    VrefCaCADBPattern0[Line].cid = 0;
    VrefCaCADBPattern0[Line].bank = 0;
    VrefCaCADBPattern0[Line].pdatag = 0;
    }

  if (Timingmode == TIMING_SINGLE_CS) {
      FixupCS (LogicalRank, 0, VrefCaCADBPattern0, VrefCaCADBPattern1_1N, (*ChannelNvList) [Ch].encodedCSMode);
      ProgramCADBBrs (Host, Socket, ChannelLocal, 0, CADB_LINES, VrefCaCADBPattern0, VrefCaCADBPattern1_1N);
  } else if ((Timingmode == TIMING_MULTI_CS) && (PassThroughMode)) {
      FixupCS (LogicalRank, 0, VrefCaCADBPattern0, VrefCaCADBPattern1_4N_CAPT, (*ChannelNvList) [Ch].encodedCSMode);
      ProgramCADBBrs (Host, Socket, ChannelLocal, 0, CADB_LINES, VrefCaCADBPattern0, VrefCaCADBPattern1_4N_CAPT);
  } else if ((Timingmode == TIMING_MULTI_CS)&& (!PassThroughMode)) {
      FixupCS (LogicalRank, 0, VrefCaCADBPattern0, VrefCaCADBPattern1_4N, (*ChannelNvList) [Ch].encodedCSMode);
      ProgramCADBBrs (Host, Socket, ChannelLocal, 0, CADB_LINES, VrefCaCADBPattern0, VrefCaCADBPattern1_4N);
    } else {
      if (CsCaMode == CA_MODE) {
        RcDebugPrintWithDevice(SDBG_CPGC, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
          "Vref CA command timing mode  %d is not supported.\n", Timingmode);
      } else {
        RcDebugPrintWithDevice(SDBG_CPGC, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
          "Vref CS command timing mode  %d is not supported.\n", Timingmode);
      }
    }

    //
    // Delay if this is a CTE build, otherwise do nothing.
    //
    TrainingDelay (Host, 1600, 0);

    CpgcGblTrainingSetup.seqLoopCnt = 1;                                // test will end after one loop through the sub-sequence
    CpgcGblTrainingSetup.startDelay = 0;                                // Delay start
    CpgcGblTrainingSetup.addrUpdRateMode = CPGC_UPDATE_BY_CL;                // Update based on the # of read and write cacheline
    CpgcGblTrainingSetup.rwMode = CPGC_BASE_WRITE_SUBSEQ;           // Base Read or write - subseq0
    CpgcGblTrainingSetup.numCacheLines = 2;                                // Number of Cachelines (2^N)
    CpgcGblTrainingSetup.burstSeparation = 0;                                // Burst separation based on training step
    CpgcGblTrainingSetup.errEnChunk = CPGC_ERR_MONITOR_NO_CHUNK;        // Do not monitor errors on chunks
    CpgcGblTrainingSetup.errEnCl = CPGC_ERR_MONITOR_NO_CL;           // Do not monitor errors on cacheline
    CpgcGblTrainingSetup.stopOnErrCtl = CPGC_ERR_NEVER_STOP_ON_ERR;       // Do not Stop on all lane error
    CpgcGblTrainingSetup.subseqEndPtr = 1;                                // subseq end pointer
    CpgcGblTrainingSetup.useSubSeq1 = CPGC_BASE_READ_SUBSEQ;            // use sub seq1 (base read)
    CpgcGblTrainingSetup.cpgcLinkTrain = 0;                                // do not use cpgc link train
    CpgcGblTrainingSetup.back2backWrEn = 0;                                // Enable b2b writes
    CpgcGblTrainingSetup.mprControl = 0;                                // MPR disable
    CpgcGblTrainingSetup.refMask = 0xFF;                             // disable cpgc self refresh
    CpgcGblTrainingSetup.zqMask = 0xFF;                             // disable cpgc zq
    CpgcGblTrainingSetup.x8Mode = 0;
    CpgcGlobalTrainingSetup (Host, Socket, ChannelLocal, &CpgcGblTrainingSetup);
    //
    // Enable CADB
    //
    for (SubChLocal = 0; SubChLocal < SUB_CH; SubChLocal++) {
      GetChannelMapping (Ch, SubChLocal, &ChannelLocal2);
      CpgcPatCadbCtlReg.Data = MemReadPciCfgEp (Socket, ChannelLocal2, CPGC_PATCADBCTL_MCDDC_CTL_REG);
      CpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 0;
      CpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 1;
      CpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 1;
      CpgcPatCadbCtlReg.Bits.enable_twopasscadb_always_on = 0;
      CpgcPatCadbCtlReg.Bits.enable_cadb_parityntiming = 0;
      MemWritePciCfgEp (Socket, ChannelLocal2, CPGC_PATCADBCTL_MCDDC_CTL_REG, CpgcPatCadbCtlReg.Data);

      CpgcMixCfg.Data = MemReadPciCfgEp (Socket, ChannelLocal2, CPGC_DDRT_MIX_CFG_MC_MAIN_REG);
      CpgcMixCfg.Bits.enable_twopasscadb_always_on_shadow = 0;
      MemWritePciCfgEp (Socket, ChannelLocal2, CPGC_DDRT_MIX_CFG_MC_MAIN_REG, CpgcMixCfg.Data);
    }
    //
    // Delay if this is a CTE build, otherwise do nothing.
    //
     TrainingDelay (Host, 1600, 0);
    // Start the test and poll the test done.
    //
    ChBitmask |= 1 << Ch;
    ExecuteCmdSignalTest (Host, Socket, ChBitmask, SubCh);
    //
    // Disable CADB one pass and two pass always on.
    //
    for (SubChLocal = 0; SubChLocal < SUB_CH; SubChLocal++) {
      GetChannelMapping (Ch, SubChLocal, &ChannelLocal2);
      CpgcPatCadbCtlReg.Data = MemReadPciCfgEp (Socket, ChannelLocal2, CPGC_PATCADBCTL_MCDDC_CTL_REG);
      CpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 0;
      CpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 0;
      CpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 0;
      CpgcPatCadbCtlReg.Bits.enable_twopasscadb_always_on = 0;
      CpgcPatCadbCtlReg.Bits.enable_cadb_parityntiming = 0;
      MemWritePciCfgEp (Socket, ChannelLocal2, CPGC_PATCADBCTL_MCDDC_CTL_REG, CpgcPatCadbCtlReg.Data);

      CpgcMixCfg.Data = MemReadPciCfgEp (Socket, ChannelLocal2, CPGC_DDRT_MIX_CFG_MC_MAIN_REG);
      CpgcMixCfg.Bits.enable_twopasscadb_always_on_shadow = 0;
      MemWritePciCfgEp (Socket, ChannelLocal2, CPGC_DDRT_MIX_CFG_MC_MAIN_REG, CpgcMixCfg.Data);
    }
    //
    // Program it as deselect pattern.
    //
    CpgcAdvCmdParity (Host, Socket, ChannelLocal, GetPhyRank (Dimm, Rank), 0, DESELECT, NormalCadbSetup);
    MemWritePciCfgEp (Socket, ChannelLocal, CPGC_MISCODTCTL_MCDDC_CTL_REG, CpgcMiscOdtCtlReg.Data);
  }


  Host->ChannelModeOperation = ChannelModeStore;
  return;

} // WriteCadbVrefCsCaCmd

/**

This function writes Vref CS Command by CADB

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number
  @param[in] Ch              - Ch to send command to
  @param[in] Dimm            - Dimm to send command to
  @param[in] Subchannelmask  - Sub channel mask
  @param[in] Rank            - CS to send the command to
  @param[in] Opcode          - Opcode to use
  @param[in] Timingmode      - Supports TIMING_SINGLE_CS, TIMING_MULTI_CS
  @param[in] PassThroughMode - PassThrough mode or not

  @retval N/A

**/
VOID
WriteCadbVrefCsCmd (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Subchannelmask,
  IN UINT8     Rank,
  IN UINT16    Opcode,
  IN UINT8     Timingmode,
  IN BOOLEAN   PassThroughMode
)
{

  WriteCadbVrefCsCaCmd(Host, Socket, Ch, Dimm, Subchannelmask, Rank, Opcode, Timingmode, PassThroughMode, CS_MODE);
  return;

} // WriteCadbVrefCsCmd

/**

This function writes Vref CA Command by CADB

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number
  @param[in] Ch              - Ch to send command to
  @param[in] Dimm            - Dimm to send command to
  @param[in] Subchannelmask  - Sub channel mask
  @param[in] Rank            - CS to send the command to
  @param[in] Opcode          - Opcode to use
  @param[in] Timingmode      - Supports TIMING_SINGLE_CS, TIMING_MULTI_CS
  @param[in] PassThroughMode - PassThrough mode or not

  @retval N/A

**/
VOID
WriteCadbVrefCaCmd (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Subchannelmask,
  IN UINT8     Rank,
  IN UINT16    Opcode,
  IN UINT8     Timingmode,
  IN BOOLEAN   PassThroughMode
)
{

  WriteCadbVrefCsCaCmd (Host, Socket, Ch, Dimm, Subchannelmask, Rank, Opcode, Timingmode, PassThroughMode, CA_MODE);
  return;

} // WriteCadbVrefCaCmd

/**
  Temporary workaround
  WriteCadbMrCmd modifies CPGC_SEQ_BASEADDRSTART_HI0.rank_addr. This can break training
  steps that assume rank_addr stays 0 after the CPGC has been configured once.

  @param[in]  Host            - Pointer to SysHost
  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel to configure - DDR5 80-bit channel
  @param[in]  SubChannelMask  - Mask of subchannels to configure

  @retval  None
**/
VOID
FixCpgcSeqBaseAddrStartWrapBrs (
  IN PSYSHOST      Host,
  IN UINT8         Socket,
  IN UINT8         Ch,
  IN UINT8         Subchannelmask
  )
{
  UINT8                                     SubCh;
  UINT8                                     ChannelLocal;
  CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_STRUCT SeqBaseAddrStartHi0;

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

    if (!((1 << SubCh) & (Subchannelmask))) {
      continue;
    }
    GetChannelMapping (Ch, SubCh, &ChannelLocal);

    SeqBaseAddrStartHi0.Data = MemReadPciCfgEp (Socket, ChannelLocal, CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_REG);
    SeqBaseAddrStartHi0.Bits.rank_addr  = 0;
    MemWritePciCfgEp (Socket, ChannelLocal, CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_REG, SeqBaseAddrStartHi0.Data);
  }
}

/**

This function sends Mode register read or write command by CADB for DDR5.

@param Host            - include all the MRC data
@param Socket          - Socket number
@param Ch              - Ch to send command to
@param Dimm            - Dimm to send command to
@param Subchannelmask  - sub channel mask
@param Rank            - CS to send the command to
@param Address         - Mode register Address
@param Opcode          - Opcode to use
@param Cw              - CW to use
@param Cmd             - which command: MRR_CMD or MRW_CMD
@retval N/A

**/
VOID
WriteCadbMrCmd (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Subchannelmask,
  IN UINT8     Rank,
  IN UINT16    Address,
  IN UINT16    Opcode,
  IN UINT8     Cw,
  IN UINT8     Cmd
  )
{
  UINT32                                        i;
  UINT8                                         ChBitmask = 0;
  UINT8                                         ChannelLocal = 0;
  UINT8                                         ChannelLocal2 = 0;
  UINT8                                         SubChLocal = 0;
  UINT8                                         SubCh = 0;
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT              CpgcPatCadbCtlReg;
  struct                                        channelNvram (*ChannelNvList) [MAX_CH];
  UINT32                                        CadbMuxPattern[CADB_MUX_MAX];
  UINT8                                         CadbMuxCtl[CADB_MUX_MAX];
  PatCadbProg0                                  MrPattern0Local[CADB_LINES];
  PatCadbProg1                                  MrPattern1Local[CADB_LINES];
  CPGC_DDRT_MIX_CFG_MC_MAIN_STRUCT              CpgcMixCfg;
  CPGC_GLOBAL_TRAINING_SETUP                    CpgcGblTrainingSetup;
  CPGC_MISCODTCTL_MCDDC_CTL_STRUCT    CpgcMiscOdtCtlReg;
  UINT32                               MprTrainDdrOn[2];
  UINT8 ChannelModeStore;
  ChannelModeStore = Host->ChannelModeOperation;

  OverrideChannelModeTo80bMode ();

  // CS,      Control, ODT,  CKE, Par
  PatCadbProg1 MrCADBPattern1[CADB_LINES] =
  { { 0x3FE, 0X3, 0x0, 0, 3}, // Row 0
  { 0x3FF, 0X3, 0x0, 0, 3 }, // Row 1
  { 0x3FF, 0X3, 0x0, 0, 3 }, // Row 2
  { 0x3FF, 0X3, 0x0, 0, 3 }, // Row 3
  { 0x3FF, 0X3, 0x0, 0, 3 }, // Row 4
  { 0x3FF, 0X3, 0x0, 0, 3 }, // Row 5
  { 0x3FF, 0X3, 0x0, 0, 3 }, // Row 6
  { 0x3FF, 0X3, 0x0, 0, 3 }, // Row 7
  { 0x3FF, 0X3, 0x0, 0, 3 }, // Row 8
  { 0x3FF, 0X3, 0x0, 0, 3 }, // Row 9
  { 0x3FF, 0X3, 0x0, 0, 3 }, // Row 10
  { 0x3FF, 0X3, 0x0, 0, 3 }, // Row 11
  { 0x3FF, 0X3, 0x0, 0, 3 }, // Row 12
  { 0x3FF, 0X3, 0x0, 0, 3 }, // Row 13
  { 0x3FF, 0X3, 0x0, 0, 3 }, // Row 14
  { 0x3FF, 0X3, 0x0, 0, 3 } // Row 15
  };

  // CS,      Control, ODT,  CKE, Par
  PatCadbProg1 MrCADBPbaPattern1[CADB_LINES] =
  { { 0x3FF, 0X3, 0x0, 0, 3}, // Row 0
  { 0x3FF, 0X3, 0x0, 0, 3 }, // Row 1
  { 0x3FF, 0X3, 0x0, 0, 3 }, // Row 2
  { 0x3FF, 0X3, 0x0, 0, 3 }, // Row 3
  { 0x3FE, 0X3, 0x0, 0, 3 }, // Row 4
  { 0x3FF, 0X3, 0x0, 0, 3 }, // Row 5
  { 0x3FF, 0X3, 0x0, 0, 3 }, // Row 6
  { 0x3FF, 0X3, 0x0, 0, 3 }, // Row 7
  { 0x3FF, 0X3, 0x0, 0, 3 }, // Row 8
  { 0x3FF, 0X3, 0x0, 0, 3 }, // Row 9
  { 0x3FF, 0X3, 0x0, 0, 3 }, // Row 10
  { 0x3FF, 0X3, 0x0, 0, 3 }, // Row 11
  { 0x3FF, 0X3, 0x0, 0, 3 }, // Row 12
  { 0x3FF, 0X3, 0x0, 0, 3 }, // Row 13
  { 0x3FF, 0X3, 0x0, 0, 3 }, // Row 14
  { 0x3FF, 0X3, 0x0, 0, 3 } // Row 15
  };

  ZeroMem ((UINT8 *) MprTrainDdrOn, sizeof (MprTrainDdrOn));

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "WriteCadbMrCmd starts\n");
  ChannelNvList = GetChannelNvList (Host, Socket);

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {

    if (Host->DdrioUltSupport) {
      if (Cmd == MRR_CMD) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
          "MRR Command  Address 0x%04X\n", Address);
        CteDdrCmd("mrr", 1 << Ch, Subchannelmask, Rank, Opcode, Address); //DDR5_TODO. To check with CTE
        return;
      } else {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
          "MRW Command  Address 0x%04X, data 0x%04X\n", Address, Opcode);
        CteDdrCmd("mrw", 1 << Ch, Subchannelmask, Rank, Opcode, Address); //DDR5_TODO. To check with CTE
        return;
      }
    }
  }
  ChannelLocal = Ch;
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

    if (!((1 << SubCh) & (Subchannelmask))) {
      continue;
    }
    GetChannelMapping (Ch, SubCh, &ChannelLocal);
    // Temporarily disable 3DS Mode.
    if (((*ChannelNvList) [ChannelLocal].encodedCSMode == 2) && (Host->nvram.mem.threeDsModeDisabled == 0)) {
      Toggle3DSMode (Host, Socket, ChannelLocal, 0);
    }
    if ((*ChannelNvList) [Ch].enabled == 0) {
      return;
    }
    CpgcMiscOdtCtlReg.Data = MemReadPciCfgEp (Socket, ChannelLocal, CPGC_MISCODTCTL_MCDDC_CTL_REG);
    MprTrainDdrOn[SubCh] = CpgcMiscOdtCtlReg.Bits.mpr_train_ddr_on;
    CadbMuxPattern[0] = CADB_MUX_PATTERN0;
    CadbMuxPattern[1] = CADB_MUX_PATTERN1;
    CadbMuxPattern[2] = CADB_MUX_PATTERN2;
    CadbMuxPattern[3] = CADB_MUX_PATTERN3;

    CadbMuxCtl[0] = CADB_MUX_CTRL_BUFFER;
    CadbMuxCtl[1] = CADB_MUX_CTRL_BUFFER;
    CadbMuxCtl[2] = CADB_MUX_CTRL_BUFFER;
    CadbMuxCtl[3] = CADB_MUX_CTRL_BUFFER;

    ProgramCadbMux (Socket, ChannelLocal, NO_SUBCH, CadbMuxPattern, CadbMuxCtl);
    if (Cmd == MRR_CMD) {
      for (i = 0; i < CADB_LINES; i++) {
        CopyMem (&MrPattern0Local[i], &MrrPattern0[i], sizeof (PatCadbProg0));
        CopyMem (&MrPattern1Local[i], &MrCADBPattern1[i], sizeof(PatCadbProg1));
      }
      MrPattern0Local[0].address = MRRL (MrPattern0Local[0].address, Address);
      MrPattern0Local[1].address = MRRH (MrPattern0Local[1].address, Cw);
    } else {
      if ((Address == DDR5_DATA_BUFFER_RW92_REG) && (Cw == 1)) {
        for (i = 0; i < CADB_LINES; i++) {
          CopyMem (&MrPattern0Local[i], &MrwPattern0[i], sizeof (PatCadbProg0));
          CopyMem (&MrPattern1Local[i], &MrCADBPbaPattern1[i], sizeof (PatCadbProg1));
        }
        MrPattern0Local[4].address = MRW1 (Address);
        MrPattern0Local[5].address = MRW2 (Address);
        MrPattern0Local[6].address = MRW3 (Opcode);
        MrPattern0Local[7].address = MRW4 (Opcode, Cw);

        // Send the write command before sending the MRW because of the delay in when DQS toggles
        //  after the write is sent.  We need DQS to toggle right after the MRW to sent (to set PBA
        //  ID) and this pulls the toggle closer.
        MrPattern0Local[0].address = 0x002D;
        MrPattern0Local[0].cid = 0;
        MrPattern0Local[0].bank = 0;
        MrPattern0Local[0].pdatag = 0;

        MrPattern0Local[1].address = 0x0000;
        MrPattern0Local[1].cid = 0;
        MrPattern0Local[1].bank = 0;
        MrPattern0Local[1].pdatag = 0;

        MrPattern0Local[2].address = 01;
        MrPattern0Local[2].cid = 0;
        MrPattern0Local[2].bank = 0;
        MrPattern0Local[2].pdatag = 0;

        MrPattern0Local[3].address = 0x1E;
        MrPattern0Local[3].cid = 0;
        MrPattern0Local[3].bank = 0;
        MrPattern0Local[3].pdatag = 0;
      } else {
        for (i = 0; i < CADB_LINES; i++) {
          CopyMem (&MrPattern0Local[i], &MrwPattern0[i], sizeof (PatCadbProg0));
          CopyMem (&MrPattern1Local[i], &MrCADBPattern1[i], sizeof (PatCadbProg1));
        }
        MrPattern0Local[0].address = MRW1 (Address);
        MrPattern0Local[1].address = MRW2 (Address);
        MrPattern0Local[2].address = MRW3 (Opcode);
        MrPattern0Local[3].address = MRW4 (Opcode, Cw);
      }
    }
    FixupCS (GetPhyRank (Dimm, Rank), 0, MrPattern0Local, MrPattern1Local, (*ChannelNvList) [ChannelLocal].encodedCSMode);
    ProgramCADBBrs (Host, Socket, ChannelLocal, 0, CADB_LINES, MrPattern0Local, MrPattern1Local);
    if(Subchannelmask != TWO_SUBCH_MASK) {
      GetChannelMapping (Ch, SubCh ^ 1, &ChannelLocal2);
      ProgramCADBBrs (Host, Socket, ChannelLocal2, 0, CADB_LINES, DeselectPattern0Simple0, DeselctPattern1Simple1);
    }
    //
    // Delay if this is a CTE build, otherwise do nothing.
    //
    TrainingDelay (Host, 1600, 32);
    //

    CpgcGblTrainingSetup.seqLoopCnt = 1;                                // test will end after one loop through the sub-sequence
    CpgcGblTrainingSetup.startDelay = 0;                                // Delay start
    CpgcGblTrainingSetup.addrUpdRateMode = CPGC_UPDATE_BY_CL;                // Update based on the # of read and write cacheline
    CpgcGblTrainingSetup.rwMode = CPGC_BASE_WRITE_SUBSEQ;           // Base Read or write - subseq0
    CpgcGblTrainingSetup.numCacheLines = 2;                                // Number of Cachelines (2^N)
    CpgcGblTrainingSetup.burstSeparation = 0;                                // Burst separation based on training step
    CpgcGblTrainingSetup.errEnChunk = CPGC_ERR_MONITOR_NO_CHUNK;        // Do not monitor errors on chunks
    CpgcGblTrainingSetup.errEnCl = CPGC_ERR_MONITOR_NO_CL;           // Do not monitor errors on cacheline
    CpgcGblTrainingSetup.stopOnErrCtl = CPGC_ERR_NEVER_STOP_ON_ERR;       // Do not Stop on all lane error
    CpgcGblTrainingSetup.subseqEndPtr = 1;                                // subseq end pointer
    CpgcGblTrainingSetup.useSubSeq1 = CPGC_BASE_READ_SUBSEQ;            // use sub seq1 (base read)
    CpgcGblTrainingSetup.cpgcLinkTrain = 0;                                // do not use cpgc link train
    CpgcGblTrainingSetup.back2backWrEn = 0;                                // Enable b2b writes
    CpgcGblTrainingSetup.mprControl = 0;                                // MPR disable
    CpgcGblTrainingSetup.refMask = 0xFF;                             // disable cpgc self refresh
    CpgcGblTrainingSetup.zqMask = 0xFF;                             // disable cpgc zq
    CpgcGblTrainingSetup.x8Mode = 0;
    CpgcGlobalTrainingSetup (Host, Socket, ChannelLocal, &CpgcGblTrainingSetup);

    //
    // Enable CADB
    //
    for (SubChLocal = 0; SubChLocal < SUB_CH; SubChLocal++) {
      GetChannelMapping(Ch, SubChLocal, &ChannelLocal2);
      CpgcPatCadbCtlReg.Data = MemReadPciCfgEp (Socket, ChannelLocal2, CPGC_PATCADBCTL_MCDDC_CTL_REG);
      CpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 0;
      CpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 1;
      CpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 1;
      CpgcPatCadbCtlReg.Bits.enable_twopasscadb_always_on = 0;
      CpgcPatCadbCtlReg.Bits.enable_cadb_parityntiming = 0;
      MemWritePciCfgEp (Socket, ChannelLocal2, CPGC_PATCADBCTL_MCDDC_CTL_REG, CpgcPatCadbCtlReg.Data);

      CpgcMixCfg.Data = MemReadPciCfgEp (Socket, ChannelLocal2, CPGC_DDRT_MIX_CFG_MC_MAIN_REG);
      CpgcMixCfg.Bits.enable_twopasscadb_always_on_shadow = 0;
      MemWritePciCfgEp (Socket, ChannelLocal2, CPGC_DDRT_MIX_CFG_MC_MAIN_REG, CpgcMixCfg.Data);
      TrainingDelay (Host, 1600, 32);
    }
  } //sub channel loop
  //
  // Start the test and poll the test done.
  //
  //
  // Delay if this is a CTE build, otherwise do nothing.
  //
  TrainingDelay (Host, 1600, 32);
  ChBitmask |= 1 << Ch;
  if (Subchannelmask == TWO_SUBCH_MASK) {
    if ((Ch % 2) == 1) {
      ChBitmask = ChBitmask | (1 << (Ch - 1));
    } else {
      ChBitmask = ChBitmask | (1 << (Ch + 1));
    }
    SubCh = NO_SUBCH;
  } else {
    if (Subchannelmask == SUB_CH_A_MASK) {
      SubCh = SUB_CH_A;
    } else if (Subchannelmask == SUB_CH_B_MASK) {
      SubCh = SUB_CH_B;
    } else {
      SubCh = 0;
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "Fail: Subchannelmask value 0x%04X is not valid\n", Subchannelmask);
    }
  }
  ExecuteCmdSignalTest (Host, Socket, ChBitmask, SubCh);
  //
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

    if (!((1 << SubCh) & (Subchannelmask))) {
      continue;
    }
    GetChannelMapping (Ch, SubCh, &ChannelLocal);

    if ((*ChannelNvList) [Ch].enabled == 0) {
      return;
    }
    if (((*ChannelNvList) [ChannelLocal].encodedCSMode == 2) && (Host->nvram.mem.threeDsModeDisabled == 0)) {
      Toggle3DSMode (Host, Socket, ChannelLocal, 1);
    }
    // Disable CADB one pass and two pass always on.
    //
    for (SubChLocal = 0; SubChLocal < SUB_CH; SubChLocal++) {
      GetChannelMapping(Ch, SubChLocal, &ChannelLocal2);
      CpgcPatCadbCtlReg.Data = MemReadPciCfgEp (Socket, ChannelLocal2, CPGC_PATCADBCTL_MCDDC_CTL_REG);
      CpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 0;
      CpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 0;
      CpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 0;
      CpgcPatCadbCtlReg.Bits.enable_twopasscadb_always_on = 0;
      CpgcPatCadbCtlReg.Bits.enable_cadb_parityntiming = 0;
      MemWritePciCfgEp (Socket, ChannelLocal2, CPGC_PATCADBCTL_MCDDC_CTL_REG, CpgcPatCadbCtlReg.Data);

      CpgcMixCfg.Data = MemReadPciCfgEp (Socket, ChannelLocal2, CPGC_DDRT_MIX_CFG_MC_MAIN_REG);
      CpgcMixCfg.Bits.enable_twopasscadb_always_on_shadow = 0;
      MemWritePciCfgEp (Socket, ChannelLocal2, CPGC_DDRT_MIX_CFG_MC_MAIN_REG, CpgcMixCfg.Data);
    }
    //
    // Program it as deselect pattern.
    //
    CpgcAdvCmdParity (Host, Socket, ChannelLocal, GetPhyRank (Dimm, Rank), 0, DESELECT, NormalCadbSetup);
    CpgcMiscOdtCtlReg.Data = MemReadPciCfgEp (Socket, ChannelLocal, CPGC_MISCODTCTL_MCDDC_CTL_REG);
    CpgcMiscOdtCtlReg.Bits.mpr_train_ddr_on = MprTrainDdrOn[SubCh];
    MemWritePciCfgEp (Socket, ChannelLocal, CPGC_MISCODTCTL_MCDDC_CTL_REG, CpgcMiscOdtCtlReg.Data);
  } //subch loop

  FixCpgcSeqBaseAddrStartWrapBrs (Host, Socket, Ch, Subchannelmask);
  Host->ChannelModeOperation = ChannelModeStore;
  return;
} // WriteCadbMrCmd

/**

This function writes Vref CA Command by CADB in PDA Mode

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number
  @param[in] Ch              - Ch to send command to
  @param[in] Dimm            - Dimm to send command to
  @param[in] Subchannelmask  - Sub channel mask
  @param[in] Rank            - CS to send the command to
  @param[in] Strobe          - Current Strobe (0-Based)
  @param[in] Opcode          - Opcode to use
  @param[in] Timingmode      - Supports TIMING_SINGLE_CS, TIMING_MULTI_CS
  @param[in] PassThroughMode - PassThrough mode or not

@retval N/A

**/
VOID
WriteCadbVrefCaPdaCmd (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Subchannelmask,
  IN UINT8     Rank,
  IN UINT8     Strobe,
  IN UINT16    Opcode,
  IN UINT8     Timingmode,
  IN BOOLEAN   PassThroughMode
  )
{

  //
  // Set dqovren bit as 1 to Enable DQ override and set the dqvalueovr to override the DQ value.
  //
  //
  // DDR5_TODO. to check whether needs to set RTT_CA.
  //
  //
  // Enter PDA Mode.
  //
  //WriteCadbMpcCmd (Host, Socket, Ch, Dimm, Subchannelmask, Rank, ENTER_PDA_ENUMERATE_PROGRAM_MODE, Timingmode);
  //
  // Set the VrefCA.
  //
  WriteCadbVrefCaCmd (Host, Socket, Ch, Dimm, Subchannelmask, Rank, Opcode, Timingmode, PassThroughMode);
  //
  // Exit PDA Mode.
  //
  //WriteCadbMpcCmd (Host, Socket, Ch, Dimm, Subchannelmask, Rank, EXIT_PDA_ENUMERATE_PROGRAM_MODE, Timingmode);
  return;

} // WriteCadbVrefCaPdaCmd

/**

  Early CS CLK training pattern sets up. DDR5 only.
  It is used to setup CS training patterns for all channels defined by input parameter "ChannelMask"

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket to train
  @param[in] ChannelMask      - Ch Bit mask for which test should be setup for
  @param[in] Signal           - Signal to test

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcCsClkPattern (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT32   ChannelMask,
  IN GSM_CSN  Signal
  )
{
  UINT32                                        Status;
  UINT8                                         Channel;
  UINT8                                         SubCh;
  UINT32                                        i;
  UINT32                                        CadbMuxPattern[CADB_MUX_MAX];
  UINT8                                         CadbMuxCtl[CADB_MUX_MAX];
  CPGC_GLOBAL_TRAINING_SETUP                    CpgcGblTrainingSetup;
  CPGC_SEQ_BASEADDRSTART_LO0_MC_MAIN_STRUCT     SeqBaseAddrStartLo0;
  CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_STRUCT     SeqBaseAddrStartHi0;
  CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAIN_STRUCT      SeqBaseAddrWrapLo0;
  CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_STRUCT      SeqBaseAddrWrapHi0;
  CPGC_SEQ_BAINCCTL00_MC_MAIN_STRUCT            SeqBaincCtl00;
  CPGC_SEQ_BAOCIC0_MC_MAIN_STRUCT               CpgcSeqBaoCic0;
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT              CpgcPatCadbCtlReg;
  CPGC_SEQ_BAINCCTL10_MC_MAIN_STRUCT            SeqBaincCtl10;
  struct channelNvram                           (*ChannelNvList) [MAX_CH];
  PatCadbProg0                                  CsClkPattern0Buf[CADB_LINES];
  PatCadbProg1                                  CsClkPattern1Buf[CADB_LINES];
  PatCadbProg1                                  CsmPattern[CADB_LINES];
  UINT8                                         Ch;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "CpgcCsClkPattern entry\n");
  ChannelNvList = GetChannelNvList (Host, Socket);

  if (Signal < CS0_B_N) {
    SubCh = 0;
  } else {
    SubCh = 1;
  }

  if (IS_CS_SIGNAL(Signal)) {
    //
    // Global training settings
    //
    CpgcGblTrainingSetup.seqLoopCnt = 1;                                // test will end after one loop through the sub-sequence
    CpgcGblTrainingSetup.startDelay = 0;                                // Delay start
    CpgcGblTrainingSetup.addrUpdRateMode = CPGC_UPDATE_BY_CL;                // Update based on the # of read and write cacheline
    CpgcGblTrainingSetup.rwMode = CPGC_BASE_WRITE_SUBSEQ;           // Base Read or write - subseq0
    CpgcGblTrainingSetup.numCacheLines = 2;                                // Number of Cachelines (2^N)
    CpgcGblTrainingSetup.burstSeparation = 0;                                // Burst separation based on training step
    CpgcGblTrainingSetup.errEnChunk = CPGC_ERR_MONITOR_NO_CHUNK;        // Do not monitor errors on chunks
    CpgcGblTrainingSetup.errEnCl = CPGC_ERR_MONITOR_NO_CL;           // Do not monitor errors on cacheline
    CpgcGblTrainingSetup.stopOnErrCtl = CPGC_ERR_NEVER_STOP_ON_ERR;       // Do not Stop on all lane error
    CpgcGblTrainingSetup.subseqEndPtr = 1;                                // subseq end pointer
    CpgcGblTrainingSetup.useSubSeq1 = CPGC_BASE_READ_SUBSEQ;            // use sub seq1 (base read)
    CpgcGblTrainingSetup.cpgcLinkTrain = 0;                                // do not use cpgc link train
    CpgcGblTrainingSetup.back2backWrEn = 0;                                // Enable b2b writes
    CpgcGblTrainingSetup.mprControl = 0;                                // MPR disable
    CpgcGblTrainingSetup.refMask = 0xFF;                             // disable cpgc self refresh
    CpgcGblTrainingSetup.zqMask = 0xFF;                             // disable cpgc zq
    CpgcGblTrainingSetup.x8Mode = 0;

    //
    // WDB programming settings
    //
    SeqBaseAddrStartLo0.Data = 0;
    SeqBaseAddrStartHi0.Data = 0;
    SeqBaseAddrStartHi0.Bits.rank_addr = 0;

    SeqBaseAddrWrapLo0.Data = 0;
    SeqBaseAddrWrapLo0.Bits.column_addr = 0x1FF;
    SeqBaseAddrWrapLo0.Bits.bank_addr = 0x7;

    SeqBaseAddrWrapHi0.Data = 0;
    SeqBaseAddrWrapHi0.Bits.row_addr = 0x1000;
    SeqBaseAddrWrapHi0.Bits.rank_addr = 0;

    //
    // Set Sequence Bank Address Increment control
    //
    SeqBaincCtl00.Data = 0;
    SeqBaincCtl00.Bits.column_ba_inc = 0;
    SeqBaincCtl00.Bits.column_ba_updrate = 0;
    SeqBaincCtl00.Bits.column_ba_updscale = 0;
    SeqBaincCtl00.Bits.row_ba_inc = 12;
    SeqBaincCtl00.Bits.row_ba_updrate = 0;
    SeqBaincCtl00.Bits.row_ba_updscale = 1;

    SeqBaincCtl10.Data = 0;
    SeqBaincCtl10.Bits.bank_ba_inc = 4;
    SeqBaincCtl10.Bits.bank_ba_updrate = 0;
    SeqBaincCtl10.Bits.bank_ba_updscale = 1;
    SeqBaincCtl10.Bits.rank_ba_inc = 0;
    SeqBaincCtl10.Bits.rank_ba_updrate = 0;
    SeqBaincCtl10.Bits.rank_ba_updscale = 0;
    SeqBaincCtl10.Bits.column_ba_inc_msb = 0;

    //
    // Set wrap order
    //
    CpgcSeqBaoCic0.Data = 0;

    //
    // Program CADB pattern buffer
    //
    CadbMuxPattern[0] = CS_CLK_CADB_MUX_PATTERN0;
    CadbMuxPattern[1] = 0;
    CadbMuxPattern[2] = 0;
    CadbMuxPattern[3] = 0;

    CadbMuxCtl[0] = CADB_MUX_CTRL_BUFFER;
    CadbMuxCtl[1] = CADB_MUX_CTRL_BUFFER;
    CadbMuxCtl[2] = CADB_MUX_CTRL_BUFFER;
    CadbMuxCtl[3] = CADB_MUX_CTRL_BUFFER;

  } //if

  //
  // Program CADB entries
  //
  for (i = 0; i < CADB_LINES; i++) {

    CopyMem(&CsmPattern[i], &CstmPattern1[i], sizeof(PatCadbProg1));

    if (CstmPattern1[i].cs != 0x3FF) {
      if (Signal < CS0_B_N) {
        CsmPattern[i].cs = (~(1 << (Signal - CS0_A_N))) & 0x3FF;
        if (Signal == CS2_A_N) {
          CsmPattern[i].cs = (~(1 << (4))) & 0x3FF;
        }
        if (Signal == CS3_A_N) {
          CsmPattern[i].cs = (~(1 << (5))) & 0x3FF;
        }
      } else {
        CsmPattern[i].cs = (~(1 << (Signal - CS0_B_N))) & 0x3FF;
        if (Signal == CS2_B_N) {
          CsmPattern[i].cs = (~(1 << (4))) & 0x3FF;
        }
        if (Signal == CS3_B_N) {
          CsmPattern[i].cs = (~(1 << (5))) & 0x3FF;
        }
      }
    }

  }

  for (i = 0; i < CADB_LINES; i++) {
    CopyMem (&CsClkPattern0Buf[i], &CstmPattern0[i], sizeof (PatCadbProg0));
    CopyMem (&CsClkPattern1Buf[i], &CsmPattern[i], sizeof (PatCadbProg1));
  }

  //
  // Setup training patterns for Channels
  //
  for (Ch = 0; Ch < GetMaxChDdr(); Ch += GetChInc (Host)) {
    if ((ChannelMask & (1 << Ch)) == 0) {
      continue;
    }
    GetChannelMapping (Ch, SubCh, &Channel);
    Status  = SUCCESS;

    if (IS_CS_SIGNAL(Signal)) {

      CpgcGlobalTrainingSetup (Host, Socket, Channel, &CpgcGblTrainingSetup);
      //
      // WDB programming
      //
      CpgcSetupWdb (Host, Socket, Channel,
      0,                             // wdbIncrRate
      0,                             // wdbIncrScale
      0,                             // wdbStartPntr
      3,                             // wdbEndPntr
      1,                             // setupPbwr
      CPGC_WDB_MUX_CTL_LFSR24,       // mux = pattern buffer
      SeqBaincCtl00,
      SeqBaincCtl10,
      SeqBaseAddrStartLo0,
      SeqBaseAddrStartHi0,
      SeqBaseAddrWrapLo0,
      SeqBaseAddrWrapHi0,
      CpgcSeqBaoCic0
      );
    }

    //
    // Program CADB entries
    //
    ProgramCADBBrs (Host, Socket, Channel, 0, CADB_LINES, CsClkPattern0Buf, CsClkPattern1Buf);//DDR5_TODO. confirm the lines need to change from 2 to CADB_LINES or not.

    //
    // Enable CADB
    //
    if (IS_CS_SIGNAL(Signal)) {
      //
      // Delay if this is a CTE build, otherwise do nothing.
      //
      TrainingDelay (Host, 1600, 32);
      ProgramCadbMux (Socket, Channel, NO_SUBCH, CadbMuxPattern, CadbMuxCtl);
      TrainingDelay (Host, 1600, 32);

      //CADB CTL has to be programmed for both Subchannels to avoid CPGC hang.
      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        GetChannelMapping(Ch, SubCh, &Channel);
        CpgcPatCadbCtlReg.Data = MemReadPciCfgEp (Socket, Channel, CPGC_PATCADBCTL_MCDDC_CTL_REG);
        CpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 0;
        CpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 1;
        CpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 0;
        MemWritePciCfgEp (Socket, Channel, CPGC_PATCADBCTL_MCDDC_CTL_REG, CpgcPatCadbCtlReg.Data);
      }
    }
  }
  return Status;
} //CpgcCsClkPattern

/**

  Early QCS CLK training pattern sets up. DDR5 only.
  It is used to setup QCS training patterns for all channels defined by input parameter "ChannelMask"

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket to train
  @param[in] ChannelMask      - Ch Bit mask for which test should be setup for
  @param[in] Dimm             - Dimm Id
  @param[in] Rank             - Rank Number

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcQcsClkPattern (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChannelMask,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
  UINT8                                         Ch;
  UINT32                                        SmbData = 0;

  for (Ch = 0; Ch < GetMaxChDdr (); Ch += GetChInc (Host)) {
    if ((ChannelMask & (1 << Ch)) == 0) {
      continue;
    }
    //
    // BRS_TODO. To confirm BRS will use which channel's SMBUS to connect to DIMM.
    // Enable QCS Training Mode.
    //
    ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, (UINT8 *)&SmbData);
    SmbData = SmbData | BIT0 | (Rank << 1);
    WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, SUB_CH_A_MASK, BYTE_WRITE, RDIMM_CW_PAGE0, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, &SmbData);
  }
  return SUCCESS;
}

/**

  Early BCOM CLK training pattern sets up.
  It is used to setup BCOM CLK training patterns for all channels defined by input parameter "ChannelMask"

  @param[in] Host                - Pointer to sysHost
  @param[in] Socket              - Socket to train
  @param[in] ChannelMask         - Ch Bit mask for which test should be setup for
  @param[in] Rank                - Rank number in Channel
  @param[in] Signal              - Signal to test

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcBcomClkPattern (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT32   ChannelMask,
  IN UINT8    RankInCh,
  IN GSM_CSN  Signal
  )
{
  UINT32                                        status;
  UINT8                                         Ch;
  UINT8                                         Channel;
  UINT8                                         SubChannel;
  UINT32                                        i;
  UINT32                                        CadbMuxPattern[CADB_MUX_MAX];
  UINT8                                         CadbMuxCtl[CADB_MUX_MAX];
  CPGC_GLOBAL_TRAINING_SETUP                    cpgcGblTrainingSetup;
  CPGC_SEQ_BASEADDRSTART_LO0_MC_MAIN_STRUCT     seqBaseAddrStartLo0;
  CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_STRUCT     seqBaseAddrStartHi0;
  CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAIN_STRUCT      seqBaseAddrWrapLo0;
  CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_STRUCT      seqBaseAddrWrapHi0;
  CPGC_SEQ_BAINCCTL00_MC_MAIN_STRUCT            seqBaincCtl00;
  CPGC_SEQ_BAOCIC0_MC_MAIN_STRUCT               cpgcSeqBaoCic0;
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT              cpgcPatCadbCtlReg;
  CPGC_SEQ_BAINCCTL10_MC_MAIN_STRUCT            seqBaincCtl10;
  struct channelNvram                           (*channelNvList)[MAX_CH];
  PatCadbProg0                                  BcomClkPattern0Buf[CADB_LINES];
  PatCadbProg1                                  BcomClkPattern1Buf[CADB_LINES];

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "CpgcBcomPattern entry\n");
  channelNvList = GetChannelNvList(Host, Socket);

  if (Signal < BCOM_B) {
    SubChannel = 0;
  } else {
    SubChannel = 1;
  }

  status = SUCCESS;

  cpgcGblTrainingSetup.seqLoopCnt = 0;                                // test will end after one loop through the sub-sequence
  cpgcGblTrainingSetup.startDelay = 2;                                // Delay start
  cpgcGblTrainingSetup.addrUpdRateMode = CPGC_UPDATE_BY_CL;                // Update based on the # of read and write cacheline
  cpgcGblTrainingSetup.rwMode = CPGC_BASE_WRITE_SUBSEQ;           // Base Read or write - subseq0
  cpgcGblTrainingSetup.numCacheLines = 2;                                // Number of Cachelines (2^N)
  cpgcGblTrainingSetup.burstSeparation = 0;                                // Burst separation based on training step
  cpgcGblTrainingSetup.errEnChunk = CPGC_ERR_MONITOR_NO_CHUNK;        // Do not monitor errors on chunks
  cpgcGblTrainingSetup.errEnCl = CPGC_ERR_MONITOR_NO_CL;           // Do not monitor errors on cacheline
  cpgcGblTrainingSetup.stopOnErrCtl = CPGC_ERR_NEVER_STOP_ON_ERR;       // Do not Stop on all lane error
  cpgcGblTrainingSetup.subseqEndPtr = 1;                                // subseq end pointer
  cpgcGblTrainingSetup.useSubSeq1 = CPGC_BASE_READ_SUBSEQ;            // use sub seq1 (base read)
  cpgcGblTrainingSetup.cpgcLinkTrain = 0;                                // do not use cpgc link train
  cpgcGblTrainingSetup.back2backWrEn = 0;                                // enable b2b writes
  cpgcGblTrainingSetup.mprControl = 0;                                // MPR disable
  cpgcGblTrainingSetup.refMask = 0xFF;                             // disable cpgc self refresh
  cpgcGblTrainingSetup.zqMask = 0xFF;                             // disable cpgc zq
  cpgcGblTrainingSetup.x8Mode = 0;

  //
  // WDB programming settings
  //
  seqBaseAddrStartLo0.Data = 0;
  seqBaseAddrStartHi0.Data = 0;
  seqBaseAddrStartHi0.Bits.rank_addr = 0;

  seqBaseAddrWrapLo0.Data = 0;
  seqBaseAddrWrapLo0.Bits.column_addr = 0x1FF;
  seqBaseAddrWrapLo0.Bits.bank_addr = 0x7;

  seqBaseAddrWrapHi0.Data = 0;
  seqBaseAddrWrapHi0.Bits.row_addr = 0x1000;
  seqBaseAddrWrapHi0.Bits.rank_addr = 0;

  //
  // Sequence Bank Address Increment control
  //
  seqBaincCtl00.Data = 0;
  seqBaincCtl00.Bits.column_ba_inc = 0;
  seqBaincCtl00.Bits.column_ba_updrate = 0;
  seqBaincCtl00.Bits.column_ba_updscale = 0;
  seqBaincCtl00.Bits.row_ba_inc = 12;
  seqBaincCtl00.Bits.row_ba_updrate = 0;
  seqBaincCtl00.Bits.row_ba_updscale = 1;

  seqBaincCtl10.Data = 0;
  seqBaincCtl10.Bits.bank_ba_inc = 4;
  seqBaincCtl10.Bits.bank_ba_updrate = 0;
  seqBaincCtl10.Bits.bank_ba_updscale = 1;
  seqBaincCtl10.Bits.rank_ba_inc = 0;
  seqBaincCtl10.Bits.rank_ba_updrate = 0;
  seqBaincCtl10.Bits.rank_ba_updscale = 0;
  seqBaincCtl10.Bits.column_ba_inc_msb = 0;

  //
  // Set wrap order
  //
  cpgcSeqBaoCic0.Data = 0;

  //
  // Program CADB pattern buffer
  //
  CadbMuxPattern[0] = CADB_MUX_PATTERN0;
  CadbMuxPattern[1] = CADB_MUX_PATTERN1;
  CadbMuxPattern[2] = 0xaaaa;
  CadbMuxPattern[3] = 0;

  CadbMuxCtl[0]     = CADB_MUX_CTRL_LFSR16;
  CadbMuxCtl[1]     = CADB_MUX_CTRL_LFSR16;
  CadbMuxCtl[2]     = CADB_MUX_CTRL_BUFFER;
  CadbMuxCtl[3]     = CADB_MUX_CTRL_BUFFER;


  for (i = 0; i < CADB_LINES; i++) {
    CopyMem (&BcomClkPattern0Buf[i], &BcomPattern0[i], sizeof (PatCadbProg0));
    CopyMem (&BcomClkPattern1Buf[i], &BcomPattern1[i], sizeof (PatCadbProg1));
  }

  for (Ch = 0; Ch < GetMaxChDdr (); Ch += GetChInc (Host)) {
    if ((ChannelMask & (1 << Ch)) == 0) {
      continue;
    }

    GetChannelMapping (Ch, SubChannel, &Channel);
    CpgcGlobalTrainingSetup (Host, Socket, Channel, &cpgcGblTrainingSetup);
    CpgcSetupWdb (Host, Socket, Channel,
    0,                             // wdbIncrRate
    0,                             // wdbIncrScale
    0,                             // wdbStartPntr
    3,                             // wdbEndPntr
    1,                             // setupPbwr
    CPGC_WDB_MUX_CTL_LFSR24,       // mux = pattern buffer
    seqBaincCtl00,
    seqBaincCtl10,
    seqBaseAddrStartLo0,
    seqBaseAddrStartHi0,
    seqBaseAddrWrapLo0,
    seqBaseAddrWrapHi0,
    cpgcSeqBaoCic0
    );

    FixupCS (RankInCh, 0, BcomClkPattern0Buf, BcomClkPattern1Buf, (*channelNvList) [Ch].encodedCSMode);
    ProgramCADBBrs (Host, Socket, Channel, 0, CADB_LINES, BcomClkPattern0Buf, BcomClkPattern1Buf);//DDR5_TODO. confirm the lines need to change from 2 to CADB_LINES or not.

    //
    // Enable CADB
    //
    cpgcPatCadbCtlReg.Data = MemReadPciCfgEp (Socket, Channel, CPGC_PATCADBCTL_MCDDC_CTL_REG);
    cpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 0;
    cpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 1;
    cpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 0;
    MemWritePciCfgEp (Socket, Channel, CPGC_PATCADBCTL_MCDDC_CTL_REG, cpgcPatCadbCtlReg.Data);


    ProgramCadbMux (Socket, Channel, NO_SUBCH, CadbMuxPattern, CadbMuxCtl);
  } // Ch loop
  return status;
} //CpgcBcomPattern


/**

  Early CA CLK training pattern sets up.
  It is used to setup CA training patterns for all channels defined by input parameter "ChannelMask"

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket to train
  @param[in] ChannelMask      - Ch Bit mask for which test should be setup for
  @param[in] Rank             - Rank number
  @param[in] Subrank          - Subrank number
  @param[in] Sig              - Signal
  @param[in] Mode             - Programming mode.
  @param[in]  PatternLoop      - Pattern Loop

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcCaClkPattern (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChannelMask,
  IN UINT8     Rank,
  IN UINT8     Subrank,
  IN GSM_CSN   Sig,
  IN CPGC_ADV_CMD_PARITY_MODE  Mode,
  IN UINT8     Patternloop
  )
{
  UINT32                                        i;
  MRC_MST                                       MemSsType;
  UINT32                                        Status;
  UINT8                                         Ch;
  UINT8                                         Channel;
  UINT8                                         SubCh;
  CPGC_GLOBAL_TRAINING_SETUP                    CpgcGblTrainingSetup;
  CPGC_SEQ_BASEADDRSTART_LO0_MC_MAIN_STRUCT     SeqBaseAddrStartLo0;
  CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_STRUCT     SeqBaseAddrStartHi0;
  CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAIN_STRUCT      SeqBaseAddrWrapLo0;
  CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_STRUCT      SeqBaseAddrWrapHi0;
  CPGC_SEQ_BAINCCTL00_MC_MAIN_STRUCT            SeqBaincCtl00;
  CPGC_SEQ_BAOCIC0_MC_MAIN_STRUCT               CpgcSeqBaoCic0;
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT              CpgcPatCadbCtlReg;
  CPGC_SEQ_BAINCCTL10_MC_MAIN_STRUCT            SeqBaincCtl10;
  struct channelNvram                           (*ChannelNvList) [MAX_CH];
  UINT32                                        CadbMuxPattern[CADB_MUX_MAX];
  UINT8                                         CadbMuxCtl[CADB_MUX_MAX];
  PatCadbProg0                                  AddressPattern0Local[CADB_LINES];
  PatCadbProg1                                  AddressPattern1Local[CADB_LINES];
  PatCadbProg1                                  ParPattern1Local[CADB_LINES];
  UINT8                                         ChannelLocal2 = 0;
  UINT8                                         SubChLocal = 0;

  ChannelNvList = GetChannelNvList (Host, Socket);
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

  if (Sig < A0B) {
    SubCh = 0;
  } else {
    SubCh = 1;
  }
  //
  // Program CADB pattern buffer
  //
  CadbMuxPattern[0] = CADB_MUX_PATTERN0;//DDR5_TODO. seed0--it can be any value now.
  CadbMuxPattern[1] = CADB_MUX_PATTERN1; //DDR5_TODO. seed1--it can be any value now.
  CadbMuxPattern[2] = CADB_MUX_PATTERN2_CACLK;
  CadbMuxPattern[3] = CADB_MUX_PATTERN3_CACLK;

  CadbMuxCtl[0] = 2;
  CadbMuxCtl[1] = 2;
  CadbMuxCtl[2] = 1;
  CadbMuxCtl[3] = 1;

  for (Ch = 0; Ch < GetMaxChDdr (); Ch++) {
    if ((ChannelMask & (1 << Ch)) == 0) {
      continue;
    }

    RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "CpgcCaClkPattern\n");
    GetChannelMapping (Ch, SubCh, &Channel); //BRS_TODO. To confirm.
    ProgramCadbMux (Socket, Ch, SubCh, CadbMuxPattern, CadbMuxCtl);

    //
    // Delay if this is a CTE build, otherwise do nothing.
    //
    CteDelayDclk (10);


    if (Host->var.mem.socket[Socket].TrainingStatus.Bits.TrainingPatternMode == QCA_TRAINING_PATTERN_XTALK) {
      for (i = 0; i < CADB_LINES; i++) {
        CopyMem (&AddressPattern0Local[i], &CatmPattern0[i], sizeof (PatCadbProg0));
        CopyMem (&AddressPattern1Local[i], &CatmPattern1[i], sizeof (PatCadbProg1));
        CopyMem (&ParPattern1Local[i], &CatmParPattern1[i], sizeof (PatCadbProg1));
      }
    } else {
      for (i = 0; i < CADB_LINES; i++) {
        CopyMem (&AddressPattern0Local[i], &CatmPatternSimple0[i], sizeof (PatCadbProg0));
        CopyMem (&AddressPattern1Local[i], &CatmPattern1[i], sizeof (PatCadbProg1));
        CopyMem (&ParPattern1Local[i], &CatmParPattern1[i], sizeof (PatCadbProg1));
      }
    }
    GetChannelMapping (Ch, SubCh, &Channel); //BRS_TODO. To confirm.

    Status = SUCCESS;

    if (Mode != RestoreCadbPattern) {
      if (Sig == GNT) {
        CpgcGblTrainingSetup.seqLoopCnt = 0;                                // DDRT2 ? // test will end after one loop through the sub-sequence //?? Incorrect comment
      } else {
        CpgcGblTrainingSetup.seqLoopCnt = 1;                                // test will end after one loop through the sub-sequence
      }
      CpgcGblTrainingSetup.startDelay = 0;                                // Delay start
      CpgcGblTrainingSetup.addrUpdRateMode = CPGC_UPDATE_BY_CL;                // Update based on the # of read and write cacheline
      CpgcGblTrainingSetup.rwMode = CPGC_BASE_WRITE_SUBSEQ;           // Base Read or write - subseq0
      CpgcGblTrainingSetup.numCacheLines = 2;                                // Number of Cachelines (2^N)
      CpgcGblTrainingSetup.burstSeparation = 0;                                // Burst separation based on training step
      CpgcGblTrainingSetup.errEnChunk = CPGC_ERR_MONITOR_ALL_CHUNK;       // monitor errors on any chunk
      CpgcGblTrainingSetup.errEnCl = CPGC_ERR_MONITOR_ALL_CL;          // monitor errors on any cacheline
      CpgcGblTrainingSetup.stopOnErrCtl = CPGC_ERR_STOP_ON_ALL_ERR;         // Stop on all lane error
      CpgcGblTrainingSetup.subseqEndPtr = 1;                                // subseq end pointer
      CpgcGblTrainingSetup.useSubSeq1 = CPGC_BASE_READ_SUBSEQ;            // use sub seq1 (base read)
      CpgcGblTrainingSetup.cpgcLinkTrain = 0;                                // do not use cpgc link train
      CpgcGblTrainingSetup.back2backWrEn = 0;                                // Enable b2b writes
      CpgcGblTrainingSetup.mprControl = 0;                                // MPR disable
      CpgcGblTrainingSetup.refMask = 0xFF;                             // disable cpgc self refresh
      CpgcGblTrainingSetup.zqMask = 0xFF;                             // disable cpgc zq
      //if(x8) {
      CpgcGblTrainingSetup.x8Mode = 0;

      CpgcGlobalTrainingSetup (Host, Socket, Channel, &CpgcGblTrainingSetup);

      //
      // WDB programming
      //
      //DDR5_TODO. To check the below code can be removed or not for always on mode?

      SeqBaseAddrStartLo0.Data = 0;
      SeqBaseAddrStartHi0.Data = 0;
      SeqBaseAddrStartHi0.Bits.rank_addr = 0;

      SeqBaseAddrWrapLo0.Data = 0;
      SeqBaseAddrWrapLo0.Bits.column_addr = 0x1FF;
      SeqBaseAddrWrapLo0.Bits.bank_addr = 0x7;

      SeqBaseAddrWrapHi0.Data = 0;
      SeqBaseAddrWrapHi0.Bits.row_addr = 0x1000;
      SeqBaseAddrWrapHi0.Bits.rank_addr = Rank;

      //
      // Sequence Bank Address Increment control
      //
      SeqBaincCtl00.Data = 0;
      SeqBaincCtl00.Bits.column_ba_inc = 0;
      SeqBaincCtl00.Bits.column_ba_updrate = 0;
      SeqBaincCtl00.Bits.column_ba_updscale = 0;
      SeqBaincCtl00.Bits.row_ba_inc = 12;
      SeqBaincCtl00.Bits.row_ba_updrate = 0;
      SeqBaincCtl00.Bits.row_ba_updscale = 1;

      SeqBaincCtl10.Data = 0;
      SeqBaincCtl10.Bits.bank_ba_inc = 4;
      SeqBaincCtl10.Bits.bank_ba_updrate = 0;
      SeqBaincCtl10.Bits.bank_ba_updscale = 1;
      SeqBaincCtl10.Bits.rank_ba_inc = 0;
      SeqBaincCtl10.Bits.rank_ba_updrate = 0;
      SeqBaincCtl10.Bits.rank_ba_updscale = 0;
      SeqBaincCtl10.Bits.column_ba_inc_msb = 0;

      //
      // Set wrap order
      //
      CpgcSeqBaoCic0.Data = 0;

      CpgcSetupWdb (Host, Socket, Channel,
        0,                             // wdbIncrRate
        0,                             // wdbIncrScale
        0,                             // wdbStartPntr
        3,                             // wdbEndPntr
        1,                             // setupPbwr
        CPGC_WDB_MUX_CTL_LFSR24,       // mux = pattern buffer
        SeqBaincCtl00,
        SeqBaincCtl10,
        SeqBaseAddrStartLo0,
        SeqBaseAddrStartHi0,
        SeqBaseAddrWrapLo0,
        SeqBaseAddrWrapHi0,
        CpgcSeqBaoCic0
        );


    } // if !RestoreCadbPattern
    //
    // CADB entries for signals.
    //
    switch (Sig) {
      case A0A:
      case A1A:
      case A2A:
      case A3A:
      case A4A:
      case A5A:
      case A6A:
      case A7A:
      case A8A:
      case A9A:
      case A10A:
      case A11A:
      case A12A:
      case A13A:
      case A0B:
      case A1B:
      case A2B:
      case A3B:
      case A4B:
      case A5B:
      case A6B:
      case A7B:
      case A8B:
      case A9B:
      case A10B:
      case A11B:
      case A12B:
      case A13B:
        FixupCS(Rank, Subrank, AddressPattern0Local, AddressPattern1Local, (*ChannelNvList)[Ch].encodedCSMode);//DDR5_TODO. to update.
        FixupAddrBitsDdr5 (Host, Sig, AddressPattern0Local, AddressPattern1Local);
        ProgramCADBBrs (Host, Socket, Channel, 0, CADB_LINES, AddressPattern0Local, AddressPattern1Local);
        GetChannelMapping (Ch, !SubCh, &ChannelLocal2);
        ProgramCADBBrs (Host, Socket, ChannelLocal2, 0, CADB_LINES, DeselectPattern0Simple0, DeselctPattern1Simple1);
        break;
      case PARA:
      case PARB:
        FixupCS (Rank, Subrank, CatmParPatternSimple0, ParPattern1Local, (*ChannelNvList)[Ch].encodedCSMode);//DDR5_todo. to update.
        ProgramCADBBrs (Host, Socket, Channel, 0, CADB_LINES, CatmParPatternSimple0, ParPattern1Local);
        GetChannelMapping (Ch, !SubCh, &ChannelLocal2);
        ProgramCADBBrs (Host, Socket, ChannelLocal2, 0, CADB_LINES, DeselectPattern0Simple0, DeselctPattern1Simple1);
        break;
    /*case GNT: //DDRT2?
      FixupCS(Rank + 1, 0, ActivatePattern0Local, GntPattern1Local, (*ChannelNvList)[Ch].encodedCSMode); // GNT is on the 2nd CS per Dimm
      ProgramCADB(Host, Socket, Ch, NO_SUBCH, 0, CADB_LINES, ActivatePattern0Local, GntPattern1Local);
      break;*/
      default:
        //ProgramCADB(Host, Socket, Ch, NO_SUBCH, 0, CADB_LINES, PrechargePattern0Local, DeselectPattern1Local);
        break;
    }

    if (Mode != RestoreCadbPattern) {
      //
      // Enable CADB
      //
      for (SubChLocal = 0; SubChLocal < SUB_CH; SubChLocal++) {
        GetChannelMapping(Ch, SubChLocal, &ChannelLocal2);
        CpgcPatCadbCtlReg.Data = MemReadPciCfgEp (Socket, ChannelLocal2, CPGC_PATCADBCTL_MCDDC_CTL_REG);
        CpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 0;
        CpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 1;
        CpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 0;

        MemWritePciCfgEp (Socket, ChannelLocal2, CPGC_PATCADBCTL_MCDDC_CTL_REG, CpgcPatCadbCtlReg.Data);
      }
    }
  }
  return Status;

} // CpgcCaClkPattern


/**

  Early QCA CLK training pattern sets up.
  It is used to setup QCA training patterns for all channels defined by input parameter "ChannelMask"

  @param[in] Host                - Pointer to sysHost
  @param[in] Socket              - Socket to train
  @param[in] ChannelMask         - Ch Bit mask for which test should be setup for
  @param[in] Rank                - Rank number
  @param[in] Subrank             - Subrank number
  @param[in] Sig                 - Signal
  @param[in] Mode                - Programming mode.

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcQcaClkPattern (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChannelMask,
  IN UINT8     Rank,
  IN UINT8     Subrank,
  IN GSM_CSN   Sig,
  IN CPGC_ADV_CMD_PARITY_MODE  Mode
  )
{
  UINT32                                        Index;
  UINT8                                         c;
  UINT8                                         ChBitmask = 0;
  UINT32                                        Status;
  UINT8                                         Channel;
  UINT8                                         SubCh;
  CPGC_GLOBAL_TRAINING_SETUP                    CpgcGblTrainingSetup;
  CPGC_SEQ_BASEADDRSTART_LO0_MC_MAIN_STRUCT     SeqBaseAddrStartLo0;
  CPGC_SEQ_BASEADDRSTART_HI0_MC_MAIN_STRUCT     SeqBaseAddrStartHi0;
  CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAIN_STRUCT      SeqBaseAddrWrapLo0;
  CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAIN_STRUCT      SeqBaseAddrWrapHi0;
  CPGC_SEQ_BAINCCTL00_MC_MAIN_STRUCT            SeqBaincCtl00;
  CPGC_SEQ_BAOCIC0_MC_MAIN_STRUCT               CpgcSeqBaoCic0;
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT              CpgcPatCadbCtlReg;
  CPGC_SEQ_BAINCCTL10_MC_MAIN_STRUCT            SeqBaincCtl10;
  struct channelNvram                           (*ChannelNvList) [MAX_CH];
  UINT32                                        CadbMuxPattern[CADB_MUX_MAX];
  UINT8                                         CadbMuxCtl[CADB_MUX_MAX];
  PatCadbProg0                                  AddressPattern0Local[CADB_LINES];
  PatCadbProg1                                  AddressPattern1Local[CADB_LINES];
  UINT8                                         MaxChDdr;
  UINT8                                         Ch;


  ChannelNvList = GetChannelNvList (Host, Socket);
  MaxChDdr      = GetMaxChDdr ();

  if (Sig < Q1ACA0) {
    SubCh = 0;
  } else {
    SubCh = 1;
  }

  //
  // Program CADB pattern buffer
  //
  CadbMuxPattern[0] = CADB_MUX_PATTERN0;
  CadbMuxPattern[1] = CADB_MUX_PATTERN1;
  CadbMuxPattern[2] = CADB_MUX_PATTERN2;
  CadbMuxPattern[3] = CADB_MUX_PATTERN3;

  CadbMuxCtl[0]     = CADB_MUX_CTRL_BUFFER;
  CadbMuxCtl[1]     = CADB_MUX_CTRL_BUFFER;
  CadbMuxCtl[2]     = CADB_MUX_CTRL_BUFFER;
  CadbMuxCtl[3]     = CADB_MUX_CTRL_BUFFER;

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if ((*ChannelNvList) [Ch].enabled == 0) {
      continue;
    }

    // Temporarily disable 3DS Mode for MRS commands
    if (((*ChannelNvList) [Ch].encodedCSMode == 2) && (Host->nvram.mem.threeDsModeDisabled == 0)) {
      Toggle3DSMode (Host, Socket, Ch, 0);
    }

    ProgramCadbMux (Socket, Ch, SubCh, CadbMuxPattern, CadbMuxCtl);
    //
    // Delay if this is a CTE build, otherwise do nothing.
    //
    CteDelayDclk (100);

  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((ChannelMask & (1 << Ch)) == 0) {
      continue;
    }

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "CpgcQcaClkPattern\n");

    GetChannelMapping (Ch, SubCh, &Channel);
    if ((*ChannelNvList) [Channel].cadbMuxPattern[0] != CadbMuxPattern[0] || (*ChannelNvList) [Channel].cadbMuxPattern[1] != CadbMuxPattern[1] ||
        (*ChannelNvList) [Channel].cadbMuxPattern[2] != CadbMuxPattern[2] || (*ChannelNvList) [Channel].cadbMuxPattern[3] != CadbMuxPattern[3]) {

      CadbMuxCtl[0] = CADB_MUX_CTRL_BUFFER;
      CadbMuxCtl[1] = CADB_MUX_CTRL_BUFFER;
      CadbMuxCtl[2] = CADB_MUX_CTRL_BUFFER;
      CadbMuxCtl[3] = CADB_MUX_CTRL_BUFFER;
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "CadbMuxPattern doesn't match, need to reprogram the mux pattern\n");
      for (c = 0; c < MaxChDdr; c++) {
        if ((*ChannelNvList) [c].enabled == 0) {
          continue;
        }
        ChBitmask |= 1 << c;

        TrainingDelay (Host, 1600, 32);
        ProgramCadbMux (Socket, c, SubCh, CadbMuxPattern, CadbMuxCtl);
        //
        // Delay if this is a CTE build, otherwise do nothing.
        //
        TrainingDelay (Host, 1600, 32);
      }
      ExecuteCmdSignalTest (Host, Socket, ChBitmask, SubCh);
      Status = CpgcQcaClkPattern (Host, Socket, 1 << Ch, Rank, 0, Sig, Mode);
      continue;
    }
    if (Host->var.mem.socket[Socket].TrainingStatus.Bits.TrainingPatternMode == QCA_TRAINING_PATTERN_XTALK) {
      for (Index = 0; Index < CADB_LINES; Index++) {
        CopyMem (&AddressPattern0Local[Index], &CatmPattern0[Index], sizeof (PatCadbProg0));
        CopyMem (&AddressPattern1Local[Index], &QcatmPattern1_CAPT[Index], sizeof (PatCadbProg1));
        //GntPattern1Local[i] = GntPattern1[i]; //DDRT2_TODO.
      }
    } else {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "Program CPGC QCA Simple pattern \n");
      if (((Sig >= Q0ACA0) && (Sig <= Q0ACA6)) || ((Sig >= Q1ACA0) && (Sig <= Q1ACA6))) {
        for (Index = 0; Index < CADB_LINES; Index++) {
          CopyMem (&AddressPattern0Local[Index], &QcatmPatternSimple0_1st[Index], sizeof (PatCadbProg0));
          CopyMem (&AddressPattern1Local[Index], &QcatmPattern1_CAPT[Index], sizeof (PatCadbProg1));
        }
      } else {
        for (Index = 0; Index < CADB_LINES; Index++) {
          CopyMem (&AddressPattern0Local[Index], &QcatmPatternSimple0_2nd[Index], sizeof (PatCadbProg0));
          CopyMem (&AddressPattern1Local[Index], &QcatmPattern1_CAPT[Index], sizeof (PatCadbProg1));
        }
      }
    }
    GetChannelMapping (Ch, SubCh, &Channel);

    Status = SUCCESS;

    if (Mode != RestoreCadbPattern) {
      if (Sig == GNT) {
        CpgcGblTrainingSetup.seqLoopCnt = 0;                                // DDRT2 ? // test will end after one loop through the sub-sequence //?? Incorrect comment
      } else {
        CpgcGblTrainingSetup.seqLoopCnt = 1;                                // test will end after one loop through the sub-sequence
      }
      CpgcGblTrainingSetup.startDelay = 0;                                // Delay start
      CpgcGblTrainingSetup.addrUpdRateMode = CPGC_UPDATE_BY_CL;                // Update based on the # of read and write cacheline
      CpgcGblTrainingSetup.rwMode = CPGC_BASE_WRITE_SUBSEQ;           // Base Read or write - subseq0
      CpgcGblTrainingSetup.numCacheLines = 2;                                // Number of Cachelines (2^N)
      CpgcGblTrainingSetup.burstSeparation = 0;                                // Burst separation based on training step
      CpgcGblTrainingSetup.errEnChunk = CPGC_ERR_MONITOR_ALL_CHUNK;       // monitor errors on any chunk
      CpgcGblTrainingSetup.errEnCl = CPGC_ERR_MONITOR_ALL_CL;          // monitor errors on any cacheline
      CpgcGblTrainingSetup.stopOnErrCtl = CPGC_ERR_STOP_ON_ALL_ERR;         // Stop on all lane error
      CpgcGblTrainingSetup.subseqEndPtr = 1;                                // subseq end pointer
      CpgcGblTrainingSetup.useSubSeq1 = CPGC_BASE_READ_SUBSEQ;            // use sub seq1 (base read)
      CpgcGblTrainingSetup.cpgcLinkTrain = 0;                                // do not use cpgc link train
      CpgcGblTrainingSetup.back2backWrEn = 0;                                // enable b2b writes
      CpgcGblTrainingSetup.mprControl = 0;                                // MPR disable
      CpgcGblTrainingSetup.refMask = 0xFF;                            // disable cpgc self refresh
      CpgcGblTrainingSetup.zqMask = 0xFF;                             // disable cpgc zq
      CpgcGblTrainingSetup.x8Mode = 0;

      CpgcGlobalTrainingSetup (Host, Socket, Channel, &CpgcGblTrainingSetup);

      //
      // WDB programming
      //

      SeqBaseAddrStartLo0.Data = 0;
      SeqBaseAddrStartHi0.Data = 0;
      SeqBaseAddrStartHi0.Bits.rank_addr = 0;

      SeqBaseAddrWrapLo0.Data = 0;
      SeqBaseAddrWrapLo0.Bits.column_addr = 0x1FF;
      SeqBaseAddrWrapLo0.Bits.bank_addr = 0x7;

      SeqBaseAddrWrapHi0.Data = 0;
      SeqBaseAddrWrapHi0.Bits.row_addr = 0x1000;
      SeqBaseAddrWrapHi0.Bits.rank_addr = 0;

      //
      // Sequence Bank Address Increment control
      //
      SeqBaincCtl00.Data = 0;
      SeqBaincCtl00.Bits.column_ba_inc = 0;
      SeqBaincCtl00.Bits.column_ba_updrate = 0;
      SeqBaincCtl00.Bits.column_ba_updscale = 0;
      SeqBaincCtl00.Bits.row_ba_inc = 12;
      SeqBaincCtl00.Bits.row_ba_updrate = 0;
      SeqBaincCtl00.Bits.row_ba_updscale = 1;

      SeqBaincCtl10.Data = 0;
      SeqBaincCtl10.Bits.bank_ba_inc = 4;
      SeqBaincCtl10.Bits.bank_ba_updrate = 0;
      SeqBaincCtl10.Bits.bank_ba_updscale = 1;
      SeqBaincCtl10.Bits.rank_ba_inc = 0;
      SeqBaincCtl10.Bits.rank_ba_updrate = 0;
      SeqBaincCtl10.Bits.rank_ba_updscale = 0;
      SeqBaincCtl10.Bits.column_ba_inc_msb = 0;

      //
      // Set wrap order
      //
      CpgcSeqBaoCic0.Data = 0;

      CpgcSetupWdb (Host, Socket, Channel,
        0,                             // wdbIncrRate
        0,                             // wdbIncrScale
        0,                             // wdbStartPntr
        3,                             // wdbEndPntr
        1,                             // setupPbwr
        CPGC_WDB_MUX_CTL_LFSR24,       // mux = pattern buffer
        SeqBaincCtl00,
        SeqBaincCtl10,
        SeqBaseAddrStartLo0,
        SeqBaseAddrStartHi0,
        SeqBaseAddrWrapLo0,
        SeqBaseAddrWrapHi0,
        CpgcSeqBaoCic0
      );
    } // if !RestoreCadbPattern
    //
    // CADB entries for signals.
    //

    FixupCS (Rank, Subrank, AddressPattern0Local, AddressPattern1Local, (*ChannelNvList) [Ch].encodedCSMode);
    FixupAddrBitsDdr5 (Host, Sig, AddressPattern0Local, AddressPattern1Local);
    ProgramCADBBrs (Host, Socket, Channel, 0, CADB_LINES, AddressPattern0Local, AddressPattern1Local);


    //
    // Enable CADB
    //
    CpgcPatCadbCtlReg.Data = MemReadPciCfgEp (Socket, Channel, CPGC_PATCADBCTL_MCDDC_CTL_REG);
    CpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 0;
    CpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 1;
    CpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 0;
    CpgcPatCadbCtlReg.Bits.enable_twopasscadb_always_on = 0;
    MemWritePciCfgEp (Socket, Channel, CPGC_PATCADBCTL_MCDDC_CTL_REG, CpgcPatCadbCtlReg.Data);
    //
    // To give enough delay for CTE and silicon.
    //
    TrainingDelay (Host, 1600, 32);
  }
  return Status;

} // CpgcQcaClkPattern

  /**
  Fix up the address bit field for programming the CADB

  @param sig     - Signal to test
  @param pattern - Data to program

  **/
STATIC VOID
FixupAddrBitsDdr5 (
  IN PSYSHOST         Host,
  IN GSM_CSN          sig,
  IN OUT PatCadbProg0 prog0[CADB_LINES],
  IN OUT PatCadbProg1 prog1[CADB_LINES]
  )
{
  UINT8   Socket;
  UINT8   cadbLine;

  Socket = GetCurrentSocketId ();

  for (cadbLine = 0; cadbLine < CADB_LINES; cadbLine++) {
    if (GetCurrentTestType (Socket) == EarlyRcdCaLoopbackTest) {
      if ((sig >= A0A) && (sig <= A13A)) {
        prog0[cadbLine].address = (prog0[cadbLine].address << (sig - A0A)) | (prog0[cadbLine].address >> (14 - (sig - A0A)));//@ to confirm. to check the rotate around 14 or 7. For now, use 14 to rotate around whole queen is ok.
      } else if ((sig >= A0B) && (sig <= A13B)) {
        prog0[cadbLine].address = (prog0[cadbLine].address << (sig - A0B)) | (prog0[cadbLine].address >> (14 - (sig - A0B)));//@ to confirm. to check the rotate around 14 or 7. For now, use 14 to rotate around whole queen is ok.
      } else {
        prog0[cadbLine].address = 0;
      }
    } else {
      if ((sig >= Q0ACA0) && (sig <= Q0ACA6)) {
        prog0[cadbLine].address = (prog0[cadbLine].address << (sig - Q0ACA0)) | (((prog0[cadbLine].address) & 0x7F) >> (7 - (sig - Q0ACA0)));//@ to confirm. to check the rotate around 14 or 7. For now, use 14 to rotate around whole queen is ok.
      } else if ((sig >= Q0ACA7) && (sig <= Q0ACA13)) {
        prog0[cadbLine].address = (prog0[cadbLine].address << (sig - Q0ACA7)) | (((prog0[cadbLine].address) & 0x7F) >> (7 - (sig - Q0ACA7)));//@ to confirm. to check the rotate around 14 or 7. For now, use 14 to rotate around whole queen is ok.
      } else if ((sig >= Q1ACA0) && (sig <= Q1ACA6)) {
        prog0[cadbLine].address = (prog0[cadbLine].address << (sig - Q1ACA0)) | (((prog0[cadbLine].address) & 0x7F) >> (7 - (sig - Q1ACA0)));//@ to confirm. to check the rotate around 14 or 7. For now, use 14 to rotate around whole queen is ok.
      } else if ((sig >= Q1ACA7) && (sig <= Q1ACA13)) {
        prog0[cadbLine].address = (prog0[cadbLine].address << (sig - Q1ACA7)) | (((prog0[cadbLine].address) & 0x7F) >> (7 - (sig - Q1ACA7)));//@ to confirm. to check the rotate around 14 or 7. For now, use 14 to rotate around whole queen is ok.
      } else {
        prog0[cadbLine].address = 0;
      }
    }
  } // cadbLine loop
} // FixupAddrBitsdDR5

/**

Loads the CADB

@param Host       - Pointer to sysHost
@param Socket     - Socket to train
@param Ch         - DDR channel to setup
@param StartLine  - start line
@param NumLines   - number of lines
@param Prog0       - Data to program for PatCadbProg0
@param Prog1       - Data to program for PatCadbProg1

@retval NA

**/
VOID
ProgramCADBBrs (
  IN PSYSHOST      Host,
  IN UINT8         Socket,
  IN UINT8         Ch,
  IN UINT8         StartLine,
  IN UINT8         NumLines,
  IN PatCadbProg0  Prog0[CADB_LINES],
  IN PatCadbProg1  Prog1[CADB_LINES]
  )
{
  EFI_STATUS                          Status;
  UINT8                               CadbLine;
  CPGC_PATCADBPROG0_MCDDC_CTL_STRUCT  PatCADBProg0;

  UINT32                              FastCadb = FALSE;
  RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "ProgramCADBBrs entry\n");
  Status = CteGetOptions ("fastcadb", &FastCadb);
  if (EFI_ERROR (Status)) {
    FastCadb = FALSE;
  }

  if (FastCadb == FALSE) {
    RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "ProgramCADB: using normal cadb programing\n");
    //
    // Set the write pointer to 0
    //
    MemWritePciCfgEp (Socket, Ch, CPGC_PATCADBWRPNTR_MCDDC_CTL_REG, StartLine);
  } else {
    RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "ProgramCADB10nm: using fast cadb programing\n");
  }

  //
  // Program 16 lines in the CADB
  //
  for (CadbLine = StartLine; CadbLine < NumLines; CadbLine++) {
    PatCADBProg0.Data = 0;
    PatCADBProg0.Bits.address = Prog0[CadbLine - StartLine].address;
    PatCADBProg0.Bits.pdatag = Prog0[CadbLine - StartLine].pdatag;
    PatCADBProg0.Bits.cs = Prog1[CadbLine - StartLine].cs;
    PatCADBProg0.Bits.par = Prog1[CadbLine - StartLine].par;
    if (FastCadb == FALSE) {
      RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "FastCadb is false and start programing the CPGC_PATCADBPROG0_MCDDC_CTL_REG value is %x \n", PatCADBProg0.Data);
      MemWritePciCfgEp (Socket, Ch, CPGC_PATCADBPROG0_MCDDC_CTL_REG, PatCADBProg0.Data);
    } else {
      RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "FastCadb is true , start CteProgramCadb and program the CPGC_PATCADBPROG0_MCDDC_CTL_REG \n");
      CteProgramCadb (Ch, CadbLine, PatCADBProg0.Data, 0);
    }
  } // CadbLine loop

  //
  // Delay if this is a CTE build, otherwise do nothing.
  //
  TrainingDelay (Host, 1600, 32);
  return;

} // ProgramCADBBrs


  /**

  This function sends the NOP command by CADB for DDR5.

  @param Host            - include all the MRC data
  @param Socket          - Socket number
  @param Channel         - Ch to send command to
  @param Dimm            - Dimm to send command to
  @param Subchannelmask  - sub channel mask
  @param Rank            - CS to send the command to
  @param Address         - Mode register Address
  @param Opcode          - Opcode to use
  @param Cw              - CW to use
  @param Cmd             - which command: NOP_CMD
  @retval N/A

  **/
VOID
WriteCadbNopCmd (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT8     Subchannelmask,
  IN UINT8     Rank,
  IN UINT8     Cw,
  IN UINT8     Cmd,
  IN BOOLEAN   PassThroughMode,
  IN BOOLEAN   ExitCATM
  )
{
  UINT32                                        i;
  UINT32                                        ChBitmask = 0;
  UINT32                                        ChBitmaskLocal = 0;
  UINT8                                         ChannelLocal = 0;
  UINT8                                         ChannelLocal2 = 0;
  UINT8                                         SubChLocal = 0;
  UINT8                                         SubCh = 0;
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT              CpgcPatCadbCtlReg;
  struct                                        channelNvram (*ChannelNvList) [MAX_CH];
  UINT32                                        CadbMuxPattern[CADB_MUX_MAX];
  UINT8                                         CadbMuxCtl[CADB_MUX_MAX];
  PatCadbProg0                                  NopPattern0Local[CADB_LINES];
  CPGC_DDRT_MIX_CFG_MC_MAIN_STRUCT              CpgcMixCfg;
  CPGC_GLOBAL_TRAINING_SETUP                    CpgcGblTrainingSetup;

  // CS,      Control, ODT,  CKE, Par
  PatCadbProg1 NopCADBPattern1[CADB_LINES] =
  { { 0x3FE, 0X3, 0x0, 0, 0 }, // Row 0
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 1
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 2
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 3
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 4
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 5
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 6
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 7
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 8
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 9
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 10
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 11
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 12
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 13
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 14
  { 0x3FF, 0X3, 0x0, 0, 0 } // Row 15
  };

  // CS,      Control, ODT,  CKE, Par
  PatCadbProg1 NopCADBPattern1_CAPT[CADB_LINES] =
  { { 0x3FE, 0X3, 0x0, 0, 2 }, // Row 0
  { 0x3FF, 0X3, 0x0, 0, 2 }, // Row 1
  { 0x3FF, 0X3, 0x0, 0, 2 }, // Row 2
  { 0x3FF, 0X3, 0x0, 0, 2 }, // Row 3
  { 0x3FF, 0X3, 0x0, 0, 2 }, // Row 4
  { 0x3FF, 0X3, 0x0, 0, 2 }, // Row 5
  { 0x3FF, 0X3, 0x0, 0, 2 }, // Row 6
  { 0x3FF, 0X3, 0x0, 0, 2 }, // Row 7
  { 0x3FF, 0X3, 0x0, 0, 2 }, // Row 8
  { 0x3FF, 0X3, 0x0, 0, 2 }, // Row 9
  { 0x3FF, 0X3, 0x0, 0, 2 }, // Row 10
  { 0x3FF, 0X3, 0x0, 0, 2 }, // Row 11
  { 0x3FF, 0X3, 0x0, 0, 2 }, // Row 12
  { 0x3FF, 0X3, 0x0, 0, 2 }, // Row 13
  { 0x3FF, 0X3, 0x0, 0, 2 }, // Row 14
  { 0x3FF, 0X3, 0x0, 0, 2 } // Row 15
  };

  // CS,      Control, ODT,  CKE, Par
  PatCadbProg1 NopCADBPattern1_ExitCatm[CADB_LINES] =
  { { 0x3FE, 0X3, 0x0, 0, 0 }, // Row 0
  { 0x3FE, 0X3, 0x0, 0, 0 }, // Row 1
  { 0x3FE, 0X3, 0x0, 0, 0 }, // Row 2
  { 0x3FE, 0X3, 0x0, 0, 0 }, // Row 3
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 4
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 5
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 6
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 7
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 8
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 9
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 10
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 11
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 12
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 13
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 14
  { 0x3FF, 0X3, 0x0, 0, 0 } // Row 15
  };
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "WriteCadbNopCmd starts Cmd %d\n", Cmd);
  // CS,      Control, ODT,  CKE, Par
  PatCadbProg1 NopCADBPattern1_ExitCatm_CAPT[CADB_LINES] =
  { { 0x3FE, 0X3, 0x0, 0, 2 }, // Row 0
  { 0x3FE, 0X3, 0x0, 0, 2 }, // Row 1
  { 0x3FE, 0X3, 0x0, 0, 2 }, // Row 2
  { 0x3FE, 0X3, 0x0, 0, 2 }, // Row 3
  { 0x3FF, 0X3, 0x0, 0, 2 }, // Row 4
  { 0x3FF, 0X3, 0x0, 0, 2 }, // Row 5
  { 0x3FF, 0X3, 0x0, 0, 2 }, // Row 6
  { 0x3FF, 0X3, 0x0, 0, 2 }, // Row 7
  { 0x3FF, 0X3, 0x0, 0, 2 }, // Row 8
  { 0x3FF, 0X3, 0x0, 0, 2 }, // Row 9
  { 0x3FF, 0X3, 0x0, 0, 2 }, // Row 10
  { 0x3FF, 0X3, 0x0, 0, 2 }, // Row 11
  { 0x3FF, 0X3, 0x0, 0, 2 }, // Row 12
  { 0x3FF, 0X3, 0x0, 0, 2 }, // Row 13
  { 0x3FF, 0X3, 0x0, 0, 2 }, // Row 14
  { 0x3FF, 0X3, 0x0, 0, 2 } // Row 15
  };
  UINT8 LogicalRank = GetPhyRank (Dimm, Rank);
  ChannelNvList = GetChannelNvList (Host, Socket);

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {

    if (Host->DdrioUltSupport) {
      if (Cmd == NOP_CMD) {
        if (ExitCATM) {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
            "NOP Command to exit CATM\n");
          CteDdrCmd ("nop", 1 << Channel, Subchannelmask, Rank, 0, 0);
          CteDdrCmd ("nop", 1 << Channel, Subchannelmask, Rank, 0, 0);
        } else {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
            "NOP Command\n");
          CteDdrCmd("nop", 1 << Channel, Subchannelmask, Rank, 0, 0); //DDR5_TODO. To check with CTE
        }
        return;
      }
    }
  }
  ChannelLocal = Channel;
#if 0
  if (Subchannelmask == 1) {
    SubCh = 0;
  } else if (Subchannelmask == 2) {
    SubCh = 1;
  } else {
    SubCh = 0;
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "Fail: Subchannelmask value 0x%04X is not valid\n", Subchannelmask);
  }
#endif //if 0
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (!((1 << SubCh) & (Subchannelmask))) {
      continue;
    }
    GetChannelMapping (Channel, SubCh, &ChannelLocal);
    // Temporarily disable 3DS Mode for Vref CA command.
    if (((*ChannelNvList) [ChannelLocal].encodedCSMode == 2) && (Host->nvram.mem.threeDsModeDisabled == 0)) {
      Toggle3DSMode (Host, Socket, ChannelLocal, 0);
    }

    if ((*ChannelNvList) [Channel].enabled == 0) {
      return;
    }

    CadbMuxPattern[0] = CADB_MUX_PATTERN0;
    CadbMuxPattern[1] = CADB_MUX_PATTERN1;
    CadbMuxPattern[2] = CADB_MUX_PATTERN2;
    CadbMuxPattern[3] = CADB_MUX_PATTERN3;

    CadbMuxCtl[0] = CADB_MUX_CTRL_BUFFER;
    CadbMuxCtl[1] = CADB_MUX_CTRL_BUFFER;
    CadbMuxCtl[2] = CADB_MUX_CTRL_BUFFER;
    CadbMuxCtl[3] = CADB_MUX_CTRL_BUFFER;

    ProgramCadbMux (Socket, ChannelLocal, NO_SUBCH, CadbMuxPattern, CadbMuxCtl);

    if (Cmd == NOP_CMD) {
      for (i = 0; i < CADB_LINES; i++) {
        CopyMem (&NopPattern0Local[i], &NopPattern0[i], sizeof (PatCadbProg0));
      }
    }

    if (ExitCATM && PassThroughMode) {
      FixupCSBrs (LogicalRank, 0, NopPattern0Local, NopCADBPattern1_ExitCatm_CAPT, (*ChannelNvList) [ChannelLocal].encodedCSMode);
      ProgramCADBBrs (Host, Socket, ChannelLocal, 0, CADB_LINES, NopPattern0Local, NopCADBPattern1_ExitCatm_CAPT);
    } else if (ExitCATM) {
      FixupCSBrs (LogicalRank, 0, NopPattern0Local, NopCADBPattern1_ExitCatm, (*ChannelNvList) [ChannelLocal].encodedCSMode);
      ProgramCADBBrs (Host, Socket, ChannelLocal, 0, CADB_LINES, NopPattern0Local, NopCADBPattern1_ExitCatm);
    } else if (PassThroughMode) {
      FixupCSBrs (LogicalRank, 0, NopPattern0Local, NopCADBPattern1_CAPT, (*ChannelNvList) [ChannelLocal].encodedCSMode);
      ProgramCADBBrs (Host, Socket, ChannelLocal, 0, CADB_LINES, NopPattern0Local, NopCADBPattern1_CAPT);
    } else {
      FixupCS (LogicalRank, 0, NopPattern0Local, NopCADBPattern1, (*ChannelNvList) [ChannelLocal].encodedCSMode);
      ProgramCADBBrs (Host, Socket, ChannelLocal, 0, CADB_LINES, NopPattern0Local, NopCADBPattern1);
    }
    if(Subchannelmask != TWO_SUBCH_MASK) {
      GetChannelMapping (Channel, SubCh ^ 1, &ChannelLocal2);
      ProgramCADBBrs (Host, Socket, ChannelLocal2, 0, CADB_LINES, DeselectPattern0Simple0, DeselctPattern1Simple1);
    }
    //
    // Delay if this is a CTE build, otherwise do nothing.
    //
    TrainingDelay (Host, 1600, 32);
    CpgcGblTrainingSetup.seqLoopCnt = 1;                                // test will end after one loop through the sub-sequence
    CpgcGblTrainingSetup.startDelay = 0;                                // Delay start
    CpgcGblTrainingSetup.addrUpdRateMode = CPGC_UPDATE_BY_CL;           // Update based on the # of read and write cacheline
    CpgcGblTrainingSetup.rwMode = CPGC_BASE_WRITE_SUBSEQ;               // Base Read or write - subseq0
    CpgcGblTrainingSetup.numCacheLines = 2;                             // Number of Cachelines (2^N)
    CpgcGblTrainingSetup.burstSeparation = 0;                           // Burst separation based on training step
    CpgcGblTrainingSetup.errEnChunk = CPGC_ERR_MONITOR_NO_CHUNK;        // Do not monitor errors on chunks
    CpgcGblTrainingSetup.errEnCl = CPGC_ERR_MONITOR_NO_CL;              // Do not monitor errors on cacheline
    CpgcGblTrainingSetup.stopOnErrCtl = CPGC_ERR_NEVER_STOP_ON_ERR;     // Do not Stop on all lane error
    CpgcGblTrainingSetup.subseqEndPtr = 1;                              // subseq end pointer
    CpgcGblTrainingSetup.useSubSeq1 = CPGC_BASE_READ_SUBSEQ;            // use sub seq1 (base read)
    CpgcGblTrainingSetup.cpgcLinkTrain = 0;                             // do not use cpgc link train
    CpgcGblTrainingSetup.back2backWrEn = 0;                             // Enable b2b writes
    CpgcGblTrainingSetup.mprControl = 0;                                // MPR disable
    CpgcGblTrainingSetup.refMask = 0xFF;                                // disable cpgc self refresh
    CpgcGblTrainingSetup.zqMask = 0xFF;                                 // disable cpgc zq
    CpgcGblTrainingSetup.x8Mode = 0;
    CpgcGlobalTrainingSetup (Host, Socket, ChannelLocal, &CpgcGblTrainingSetup);
    //
    // Enable CADB
    //
    for (SubChLocal = 0; SubChLocal < SUB_CH; SubChLocal++) {
      GetChannelMapping (Channel, SubCh, &ChannelLocal2);
      CpgcPatCadbCtlReg.Data = MemReadPciCfgEp (Socket, ChannelLocal2, CPGC_PATCADBCTL_MCDDC_CTL_REG);
      CpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 0;
      CpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 1;
      CpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 1;
      CpgcPatCadbCtlReg.Bits.enable_twopasscadb_always_on = 1;
      CpgcPatCadbCtlReg.Bits.enable_cadb_parityntiming = 0;
      MemWritePciCfgEp (Socket, ChannelLocal2, CPGC_PATCADBCTL_MCDDC_CTL_REG, CpgcPatCadbCtlReg.Data);

      CpgcMixCfg.Data = MemReadPciCfgEp (Socket, ChannelLocal2, CPGC_DDRT_MIX_CFG_MC_MAIN_REG);
      CpgcMixCfg.Bits.enable_twopasscadb_always_on_shadow = 1;
      MemWritePciCfgEp (Socket, ChannelLocal2, CPGC_DDRT_MIX_CFG_MC_MAIN_REG, CpgcMixCfg.Data);
    }
    TrainingDelay (Host, 1600, 32);
  } //subch loop
  //
  // Start the test and poll the test done.
  //
  TrainingDelay (Host, 1600, 32);
  ChBitmask |= 1 << Channel;
  if (Subchannelmask == 3) {
    ChBitmaskLocal = Subchannelmask << Channel;
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "ChBitmaskLocal is 0x%x\n", ChBitmaskLocal);
    ExecuteCmdSignalTest (Host, Socket, ChBitmaskLocal, NO_SUBCH);
  } else {
    if (Subchannelmask == 1) {
      SubCh = 0;
    } else if (Subchannelmask == 2) {
      SubCh = 1;
    } else {
      SubCh = 0;
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "Fail: Subchannelmask value 0x%04X is not valid\n", Subchannelmask);
    }
    ExecuteCmdSignalTest (Host, Socket, ChBitmask, SubCh);
  }
  //
  // Re-Enable 3DS Mode
  //
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

    if (!((1 << SubCh) & (Subchannelmask))) {
      continue;
    }
    GetChannelMapping (Channel, SubCh, &ChannelLocal);

    if ((*ChannelNvList) [Channel].enabled == 0) {
      return;
    }
    if (((*ChannelNvList) [ChannelLocal].encodedCSMode == 2) && (Host->nvram.mem.threeDsModeDisabled == 0)) {
      Toggle3DSMode (Host, Socket, ChannelLocal, 1);
    }
    for (SubChLocal = 0; SubChLocal < SUB_CH; SubChLocal++) {
      GetChannelMapping (Channel, SubCh, &ChannelLocal2);
      //
      // Disable CADB one pass and two pass always on.
      //
      CpgcPatCadbCtlReg.Data = MemReadPciCfgEp (Socket, ChannelLocal2, CPGC_PATCADBCTL_MCDDC_CTL_REG);
      CpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 0;
      CpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 0;
      CpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 0;
      CpgcPatCadbCtlReg.Bits.enable_twopasscadb_always_on = 0;
      CpgcPatCadbCtlReg.Bits.enable_cadb_parityntiming = 0;
      MemWritePciCfgEp (Socket, ChannelLocal2, CPGC_PATCADBCTL_MCDDC_CTL_REG, CpgcPatCadbCtlReg.Data);

      CpgcMixCfg.Data = MemReadPciCfgEp (Socket, ChannelLocal2, CPGC_DDRT_MIX_CFG_MC_MAIN_REG);
      CpgcMixCfg.Bits.enable_twopasscadb_always_on_shadow = 0;
      MemWritePciCfgEp (Socket, ChannelLocal2, CPGC_DDRT_MIX_CFG_MC_MAIN_REG, CpgcMixCfg.Data);
    }
    //
    // Program it as deselect pattern.
    //
    CpgcAdvCmdParity (Host, Socket, ChannelLocal, GetPhyRank (Dimm, Rank), 0, DESELECT, NormalCadbSetup);
  } // Subch loop
  return;

} // WriteCadbNopCmd

/**

This function sends mode register read or write command by CADB for DDR5.

@param Host            - include all the MRC data
@param socket          - socket number
@param ch              - ch to send command to
@param dimm            - dimm to send command to
@param subchannelmask  - sub channel mask
@param rank            - CS to send the command to
@param address         - mode register address
@param data          - Data to write
@param cw              - CW to use
@param cmd             - which command: NOP_CMD
@retval N/A

**/
VOID
WriteCadbWriteCmd (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Subchannelmask,
  IN UINT8     Rank,
  IN UINT16    Address,
  IN UINT16    Opcode,
  IN UINT8     Cmd
  )
{
  UINT32                                        i;
  UINT32                                         ChBitmask = 0;
  UINT32                                        ChBitmaskLocal = 0;
  UINT8                                         ChannelLocal = 0;
  UINT8                                         ChannelLocal2 = 0;
  UINT8                                         SubChLocal = 0;
  UINT8                                         SubCh = 0;
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT              CpgcPatCadbCtlReg;
  struct                                        channelNvram (*ChannelNvList) [MAX_CH];
  UINT32                                        CadbMuxPattern[CADB_MUX_MAX];
  UINT8                                         CadbMuxCtl[CADB_MUX_MAX];
  PatCadbProg0                                  WritePattern0Local[CADB_LINES];
  CPGC_DDRT_MIX_CFG_MC_MAIN_STRUCT              CpgcMixCfg;
  CPGC_GLOBAL_TRAINING_SETUP                    CpgcGblTrainingSetup;

  // CS,      Control, ODT,  CKE, Par
  PatCadbProg1 WriteCADBPattern1[CADB_LINES] =
  { { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 0
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 1
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 2
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 3
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 4
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 5
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 6
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 7
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 8
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 9
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 10
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 11
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 12
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 13
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 14
  { 0x3FF, 0X3, 0x0, 0, 0 } // Row 15
  };


  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "WriteCadbWriteCmd starts cmd %d\n", Cmd);
  ChannelNvList = GetChannelNvList (Host, Socket);

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {

    if (Host->DdrioUltSupport) {
      if (Cmd == WR_CMD) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
          "Write Command  Address 0x%04X\n", Address);
        CteDdrCmd("Write", 1 << Ch, Subchannelmask, Rank, Opcode, Address); //DDR5_TODO. To check with CTE
        return;
      }
    }
  }
  ChannelLocal = Ch;
#if 0
  if (subchannelmask == 1) {
    SubCh = 0;
  } else if (subchannelmask == 2) {
    SubCh = 1;
  } else {
    SubCh = 0;
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "Fail: subchannelmask value 0x%04X is not valid\n", subchannelmask);
  }
#endif //if 0
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (!((1 << SubCh) & (Subchannelmask))) {
      continue;
    }
    GetChannelMapping (Ch, SubCh, &ChannelLocal);
    // Temporarily disable 3DS mode for Vref CA command.
    if (((*ChannelNvList) [Ch].encodedCSMode == 2) && (Host->nvram.mem.threeDsModeDisabled == 0)) {
      Toggle3DSMode (Host, Socket, ChannelLocal, 0);
    }

    if ((*ChannelNvList) [Ch].enabled == 0) {
      return;
    }

    CadbMuxPattern[0] = CADB_MUX_PATTERN0;
    CadbMuxPattern[1] = CADB_MUX_PATTERN1;
    CadbMuxPattern[2] = CADB_MUX_PATTERN2;
    CadbMuxPattern[3] = CADB_MUX_PATTERN3;

    CadbMuxCtl[0] = CADB_MUX_CTRL_BUFFER;
    CadbMuxCtl[1] = CADB_MUX_CTRL_BUFFER;
    CadbMuxCtl[2] = CADB_MUX_CTRL_BUFFER;
    CadbMuxCtl[3] = CADB_MUX_CTRL_BUFFER;

    ProgramCadbMux (Socket, ChannelLocal, NO_SUBCH, CadbMuxPattern, CadbMuxCtl);

    if (Cmd == WR_CMD) {
      for (i = 0; i < CADB_LINES; i++) {
        CopyMem (&WritePattern0Local[i], &WritePattern0[i], sizeof (PatCadbProg0));
      }
    }

    FixupCS (Rank, 0, WritePattern0Local,  WriteCADBPattern1, (*ChannelNvList) [Ch].encodedCSMode);
    ProgramCADBBrs (Host, Socket, ChannelLocal, 0, CADB_LINES,  WritePattern0Local,  WriteCADBPattern1);

    //
    // Delay if this is a CTE build, otherwise do nothing.
    //
    TrainingDelay (Host, 1600, 32);

    CpgcGblTrainingSetup.seqLoopCnt        = 1;                                // test will end after one loop through the sub-sequence
    CpgcGblTrainingSetup.startDelay        = 0;                                // Delay start
    CpgcGblTrainingSetup.addrUpdRateMode   = CPGC_UPDATE_BY_CL;                // Update based on the # of read and write cacheline
    CpgcGblTrainingSetup.rwMode            = CPGC_BASE_WRITE_SUBSEQ;           // Base Read or write - subseq0
    CpgcGblTrainingSetup.numCacheLines     = 2;                                // Number of Cachelines (2^N)
    CpgcGblTrainingSetup.burstSeparation   = 0;                                // Burst separation based on training step
    CpgcGblTrainingSetup.errEnChunk        = CPGC_ERR_MONITOR_NO_CHUNK;        // Do not monitor errors on chunks
    CpgcGblTrainingSetup.errEnCl           = CPGC_ERR_MONITOR_NO_CL;           // Do not monitor errors on cacheline
    CpgcGblTrainingSetup.stopOnErrCtl      = CPGC_ERR_NEVER_STOP_ON_ERR;       // Do not Stop on all lane error
    CpgcGblTrainingSetup.subseqEndPtr      = 1;                                // subseq end pointer
    CpgcGblTrainingSetup.useSubSeq1        = CPGC_BASE_READ_SUBSEQ;            // use sub seq1 (base read)
    CpgcGblTrainingSetup.cpgcLinkTrain     = 0;                                // do not use cpgc link train
    CpgcGblTrainingSetup.back2backWrEn     = 0;                                // enable b2b writes
    CpgcGblTrainingSetup.mprControl        = 0;                                // MPR disable
    CpgcGblTrainingSetup.refMask           = 0xFF;                             // disable cpgc self refresh
    CpgcGblTrainingSetup.zqMask            = 0xFF;                             // disable cpgc zq
    CpgcGblTrainingSetup.x8Mode            = 0;
    CpgcGlobalTrainingSetup (Host, Socket, ChannelLocal, &CpgcGblTrainingSetup);

    //
    // Enable CADB
    //
    for (SubChLocal = 0; SubChLocal < SUB_CH; SubChLocal++) {
      GetChannelMapping(Ch, SubChLocal, &ChannelLocal2);
      CpgcPatCadbCtlReg.Data = MemReadPciCfgEp (Socket, ChannelLocal2, CPGC_PATCADBCTL_MCDDC_CTL_REG);
      CpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 0;
      CpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 1;
      CpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 1;
      CpgcPatCadbCtlReg.Bits.enable_twopasscadb_always_on = 1;
      CpgcPatCadbCtlReg.Bits.enable_cadb_parityntiming = 0;
      MemWritePciCfgEp (Socket, ChannelLocal2, CPGC_PATCADBCTL_MCDDC_CTL_REG, CpgcPatCadbCtlReg.Data);

      CpgcMixCfg.Data = MemReadPciCfgEp (Socket, ChannelLocal2, CPGC_DDRT_MIX_CFG_MC_MAIN_REG);
      CpgcMixCfg.Bits.enable_twopasscadb_always_on_shadow = 1;
      MemWritePciCfgEp (Socket, ChannelLocal2, CPGC_DDRT_MIX_CFG_MC_MAIN_REG, CpgcMixCfg.Data);
    }
    TrainingDelay (Host, 1600, 32);
  } //subch loop
  //
  // Start the test and poll the test done.
  //
  TrainingDelay (Host, 1600, 32);

  ChBitmask |= 1 << Ch;
  if (Subchannelmask == 3) {
    ChBitmaskLocal = Subchannelmask;
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "ChBitmaskLocal is 0x%x\n", ChBitmaskLocal);
    ExecuteCmdSignalTest (Host, Socket, ChBitmaskLocal, NO_SUBCH);
  } else {
    if (Subchannelmask == 1) {
      SubCh = 0;
    } else if (Subchannelmask == 2) {
      SubCh = 1;
    } else {
      SubCh = 0;
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "Fail: subchannelmask value 0x%04X is not valid\n", Subchannelmask);
    }
    ExecuteCmdSignalTest (Host, Socket, ChBitmask, SubCh);
  }
  //
  // Re-enable 3DS mode
  //
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

    if (!((1 << SubCh) & (Subchannelmask))) {
      continue;
    }
    GetChannelMapping (Ch, SubCh, &ChannelLocal);

    if ((*ChannelNvList) [Ch].enabled == 0) {
      return;
    }
    if (((*ChannelNvList) [Ch].encodedCSMode == 2) && (Host->nvram.mem.threeDsModeDisabled == 0)) {
      Toggle3DSMode (Host, Socket, ChannelLocal, 1);
    }
    //
    // Disable CADB one pass and two pass always on.
    //
    for (SubChLocal = 0; SubChLocal < SUB_CH; SubChLocal++) {
      GetChannelMapping (Ch, SubChLocal, &ChannelLocal2);
      CpgcPatCadbCtlReg.Data = MemReadPciCfgEp (Socket, ChannelLocal2, CPGC_PATCADBCTL_MCDDC_CTL_REG);
      CpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 0;
      CpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 0;
      CpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 0;
      CpgcPatCadbCtlReg.Bits.enable_twopasscadb_always_on = 0;
      CpgcPatCadbCtlReg.Bits.enable_cadb_parityntiming = 0;
      MemWritePciCfgEp (Socket, ChannelLocal2, CPGC_PATCADBCTL_MCDDC_CTL_REG, CpgcPatCadbCtlReg.Data);

      CpgcMixCfg.Data = MemReadPciCfgEp (Socket, ChannelLocal2, CPGC_DDRT_MIX_CFG_MC_MAIN_REG);
      CpgcMixCfg.Bits.enable_twopasscadb_always_on_shadow = 0;
      MemWritePciCfgEp (Socket, ChannelLocal2, CPGC_DDRT_MIX_CFG_MC_MAIN_REG, CpgcMixCfg.Data);
    }
    //
    // Program it as deselect pattern.
    //
    CpgcAdvCmdParity (Host, Socket, ChannelLocal, GetPhyRank (Dimm, Rank), 0, DESELECT, NormalCadbSetup);
  } //subch loop
  return;

} // WriteCadbWriteCmd

/**

  This routine sets the CSRs to convert read to MRR command.

  @param[in] Host                              - Pointer to sysHost
  @param[in] Socket                            - Socket Id
  @param[in] Ch                                - Channel number (0-based)
  @param[in] Enable                            - 1/0 for Enable or not

  @retval N/A

**/
VOID
ConvertRdToMrr (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT8       Ch,
  IN UINT8       Enable
  )
{
  UINT8 SubCh = 0;
  UINT8 ChannelLocal = 0;
  CPGC_MISCODTCTL_MCDDC_CTL_STRUCT   cpgcMiscODTCtl;

  ChannelLocal = Ch;
  for (SubCh = 0 ; SubCh < SUB_CH; SubCh++) {
    if (IsBrsPresent (Host, Socket)) {
      GetChannelMapping (Ch, SubCh, &ChannelLocal);
    }
    cpgcMiscODTCtl.Data = MemReadPciCfgEp (Socket, ChannelLocal, CPGC_MISCODTCTL_MCDDC_CTL_REG);
    cpgcMiscODTCtl.Bits.ddr5_convert_rd_to_mrr = Enable;
    MemWritePciCfgEp (Socket, ChannelLocal, CPGC_MISCODTCTL_MCDDC_CTL_REG, cpgcMiscODTCtl.Data);
  }
}

/**

  Setup write levelling parameter

  @param[in]  Host         - Pointer to sysHost, the system Host (root) structure
  @param[out] CPGCAddress  - CPGC address array
  @param[out] WDBPattern   - WDB Pattern
  @param[out] LoopCount    - Number of cycles
  @param[out] NumCl        - Number of cache lines
  @param[out] BurstWait    - Number of DCLK spacing between bursts

  @retval None

**/
VOID
SetupWriteLevellingParamDdr5 (
  IN  PSYSHOST     Host,
  OUT TCPGCAddress *CPGCAddress,
  OUT TWDBPattern  *WDBPattern,
  OUT UINT8        *LoopCount,
  OUT UINT8        *NumCl,
  OUT UINT8        *BurstWait
  )
{                                                    // Rank,  Bank,  Row,    Col
  STATIC TCPGCAddress WriteLevellingCpgcAddress = {{ 0,     0,     0xFFFF, 0    },   // Start
                                                   { 0,     4,     0,      0x1F },   // Stop
                                                   { 0,     0,     0,      0    },   // Order
                                                   { 0,     0,     0,      0    },   // IncRate
                                                   { 0,     4,     0,      1    }};  // IncValue, change bank group with each command for B2B transactions
  //                                     IncRate, Start, Stop, DQPat
  TWDBPattern  WriteLevellingWdbPattern = { 0, 0, 3, BasicVA }; // Should not matter what this is. We do not compare data.

  CopyMem (CPGCAddress, &WriteLevellingCpgcAddress, sizeof (TCPGCAddress));
  CopyMem (WDBPattern, &WriteLevellingWdbPattern, sizeof (TWDBPattern));

  //
  // LoopCount shall set to 32 Write which is 2^(N-1), N=6
  //
  *LoopCount = 6;
  *NumCl = WL_NUM_CL_DDR5;

  //
  // Subsequential wait set to 40 is to ensure the spacing of each WR commands
  //
  *BurstWait = CPGC_SUBSEQ_WAIT_BRS_WL;

} // SetupWriteLevellingParamDdr5

/**

  Setup Dq Swizzle Discovery parameter

  @param[in]  Host         - Pointer to sysHost, the system Host (root) structure
  @param[in]  Socket       - Socket number
  @param[out] CPGCAddress  - CPGC address array
  @param[out] WDBPattern   - WDB Pattern
  @param[out] LoopCount    - Number of cycles
  @param[out] NumCl        - Number of cache lines
  @param[out] BurstWait    - Number of DCLK spacing between bursts

  @retval None

**/
VOID
SetupDqSwizzleDiscoveryParamDdr5 (
  IN  PSYSHOST     Host,
  IN  UINT8        Socket,
  OUT TCPGCAddress *CPGCAddress,
  OUT TWDBPattern  *WDBPattern,
  OUT UINT8        *LoopCount,
  OUT UINT8        *NumCl,
  OUT UINT8        *BurstWait
  )
{
                              // IncRate,  Start, Stop,  DQPat
  TCPGCAddress CPGCAddressBrs = { {0, 0, 0, 31},    // Start
                                  {0, 0, 0, 31},    // Stop
                                  {0, 0, 0, 0},     // Order
                                  {0, 0, 0, 0},     // IncRate
                                  {0, 0, 0, 0}};    // IncValue DDR5_TODO. To confirm

  TWDBPattern  DqSwizzleDiscoveryWDBPattern = { 32, 0, 0, BasicVA};

  CopyMem (CPGCAddress, &CPGCAddressBrs, sizeof (TCPGCAddress));
  CopyMem (WDBPattern, &DqSwizzleDiscoveryWDBPattern, sizeof (TWDBPattern));

  *LoopCount = 1;
  *NumCl = 32;
  *BurstWait = 0;

} // SetupDqSwizzleDiscoveryParamDdr5

/**

  This function sends Precharge command by CADB for DDR5.

  @param[in] Host            - Include all the MRC data
  @param[in] Socket          - Socket number
  @param[in] Ch              - Ch to send command to
  @param[in] Dimm            - Dimm to send command to
  @param[in] Subchannelmask  - Sub channel mask
  @param[in] Rank            - CS to send the command to
  @param[in] Bank            - Bank number
  @param[in] BankGroup       - Bank group number
  @param[in] Cmd             - Jedec command
                             - PREab_CMD: Precharge All, CA[3-0]=1011b, CA[4]=0b, CA[9:6]=vvvvb
                             - PREsb_CMD: Precharge Same Bank, CA[3-0]=1011b, CA[4]=0b, CA[9:6]=vvxxb, CA[10]=1b
                             - PREpb_CMD: Precharge, CA[3-0]=1011b, CA[4]=1b, CA[10:6]=xxxxxb

  @retval N/A

**/
VOID
WriteCadbPrechargeCmdDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Subchannelmask,
  IN UINT8     Rank,
  IN UINT8     Bank,
  IN UINT8     BankGroup,
  IN UINT8     Cmd
  )
{
  UINT32                            i;
  UINT32                            ChBitmask = 0;
  UINT32                            ChBitmaskLocal = 0;
  UINT8                             ChannelLocal = 0;
  UINT8                             ChannelLocal2 = 0;
  UINT8                             SubChLocal = 0;
  UINT8                             SubCh = 0;
  CPGC_PATCADBCTL_MCDDC_CTL_STRUCT  CpgcPatCadbCtlReg;
  struct                            channelNvram (*ChannelNvList) [MAX_CH];
  UINT32                            CadbMuxPattern[CADB_MUX_MAX];
  UINT8                             CadbMuxCtl[CADB_MUX_MAX];
  PatCadbProg0                      PrechargePattern0Local[CADB_LINES];
  CPGC_DDRT_MIX_CFG_MC_MAIN_STRUCT  CpgcMixCfg;
  CPGC_GLOBAL_TRAINING_SETUP        CpgcGblTrainingSetup;
  // CS,      Control, ODT,  CKE, Par
  PatCadbProg1 PrechargeCADBPattern1[CADB_LINES] =
  { { 0x3FE, 0X3, 0x0, 0, 0}, // Row 0
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 1
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 2
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 3
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 4
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 5
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 6
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 7
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 8
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 9
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 10
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 11
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 12
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 13
  { 0x3FF, 0X3, 0x0, 0, 0 }, // Row 14
  { 0x3FF, 0X3, 0x0, 0, 0 }  // Row 15
  };

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "WriteCadbPrechargeCmd starts cmd %d\n", Cmd);

  ChannelNvList = GetChannelNvList (Host, Socket);
  ChannelLocal = Ch;
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (!((1 << SubCh) & (Subchannelmask))) {
      continue;
    }

    GetChannelMapping (Ch, SubCh, &ChannelLocal);
    // Temporarily disable 3DS mode for Vref CA command.
    if (((*ChannelNvList) [Ch].encodedCSMode == 2) && (Host->nvram.mem.threeDsModeDisabled == 0)) {
      Toggle3DSMode (Host, Socket, ChannelLocal, 0);
    }

    if ((*ChannelNvList) [Ch].enabled == 0) {
      return;
    }

    CadbMuxPattern[0] = CADB_MUX_PATTERN0;
    CadbMuxPattern[1] = CADB_MUX_PATTERN1;
    CadbMuxPattern[2] = CADB_MUX_PATTERN2;
    CadbMuxPattern[3] = CADB_MUX_PATTERN3;

    CadbMuxCtl[0] = CADB_MUX_CTRL_BUFFER;
    CadbMuxCtl[1] = CADB_MUX_CTRL_BUFFER;
    CadbMuxCtl[2] = CADB_MUX_CTRL_BUFFER;
    CadbMuxCtl[3] = CADB_MUX_CTRL_BUFFER;

    ProgramCadbMux (Socket, ChannelLocal, NO_SUBCH, CadbMuxPattern, CadbMuxCtl);

    for (i = 0; i < CADB_LINES; i++) {
      CopyMem (&PrechargePattern0Local[i], &PrechargePattern0[i], sizeof (PatCadbProg0));
    }

    // PREab_CMD: Precharge All,       CA[3-0]=1011b, CA[4]=0b, CA[9:6]=vvvvb
    // PREsb_CMD: Precharge Same Bank, CA[3-0]=1011b, CA[4]=0b, CA[9:6]=vvxxb, CA[10]=1b
    // PREpb_CMD: Precharge Per Bank,  CA[3-0]=1011b, CA[4]=1b, CA[10:6]=xxxxxb
    if (Cmd == PREab_CMD) {
      PrechargePattern0Local[0].address |= BIT6 | BIT7 | BIT8 | BIT9;
    } else if (Cmd == PREsb_CMD) {
      PrechargePattern0Local[0].address |= ((Bank & (BIT1 | BIT0)) << 6) | BIT10;
    } else if (Cmd == PREpb_CMD) {
      PrechargePattern0Local[0].address |= BIT4 | ((Bank & (BIT1 | BIT0)) << 6) | ((BankGroup & (BIT2 | BIT1 | BIT0)) << 8);
    }

    FixupCS (GetPhyRank (Dimm, Rank), 0, PrechargePattern0Local,  PrechargeCADBPattern1, (*ChannelNvList) [Ch].encodedCSMode);
    ProgramCADBBrs (Host, Socket, ChannelLocal, 0, CADB_LINES,  PrechargePattern0Local,  PrechargeCADBPattern1);
    //
    // Delay if this is a CTE build, otherwise do nothing.
    //
    TrainingDelay (Host, 1600, 32);

    CpgcGblTrainingSetup.seqLoopCnt        = 1;                                // test will end after one loop through the sub-sequence
    CpgcGblTrainingSetup.startDelay        = 0;                                // Delay start
    CpgcGblTrainingSetup.addrUpdRateMode   = CPGC_UPDATE_BY_CL;                // Update based on the # of read and write cacheline
    CpgcGblTrainingSetup.rwMode            = CPGC_BASE_WRITE_SUBSEQ;           // Base Read or write - subseq0
    CpgcGblTrainingSetup.numCacheLines     = 2;                                // Number of Cachelines (2^N)
    CpgcGblTrainingSetup.burstSeparation   = 0;                                // Burst separation based on training step
    CpgcGblTrainingSetup.errEnChunk        = CPGC_ERR_MONITOR_NO_CHUNK;        // Do not monitor errors on chunks
    CpgcGblTrainingSetup.errEnCl           = CPGC_ERR_MONITOR_NO_CL;           // Do not monitor errors on cacheline
    CpgcGblTrainingSetup.stopOnErrCtl      = CPGC_ERR_NEVER_STOP_ON_ERR;       // Do not Stop on all lane error
    CpgcGblTrainingSetup.subseqEndPtr      = 1;                                // subseq end pointer
    CpgcGblTrainingSetup.useSubSeq1        = CPGC_BASE_READ_SUBSEQ;            // use sub seq1 (base read)
    CpgcGblTrainingSetup.cpgcLinkTrain     = 0;                                // do not use cpgc link train
    CpgcGblTrainingSetup.back2backWrEn     = 0;                                // enable b2b writes
    CpgcGblTrainingSetup.mprControl        = 0;                                // MPR disable
    CpgcGblTrainingSetup.refMask           = 0xFF;                             // disable cpgc self refresh
    CpgcGblTrainingSetup.zqMask            = 0xFF;                             // disable cpgc zq
    CpgcGblTrainingSetup.x8Mode            = 0;
    CpgcGlobalTrainingSetup (Host, Socket, ChannelLocal, &CpgcGblTrainingSetup);

    //
    // Enable CADB
    //
    for (SubChLocal = 0; SubChLocal < SUB_CH; SubChLocal++) {
      GetChannelMapping(Ch, SubChLocal, &ChannelLocal2);
      CpgcPatCadbCtlReg.Data = MemReadPciCfgEp (Socket, ChannelLocal2, CPGC_PATCADBCTL_MCDDC_CTL_REG);
      CpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 0;
      CpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 1;
      CpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 1;
      CpgcPatCadbCtlReg.Bits.enable_twopasscadb_always_on = 1;
      CpgcPatCadbCtlReg.Bits.enable_cadb_parityntiming = 0;
      MemWritePciCfgEp (Socket, ChannelLocal2, CPGC_PATCADBCTL_MCDDC_CTL_REG, CpgcPatCadbCtlReg.Data);

      CpgcMixCfg.Data = MemReadPciCfgEp (Socket, ChannelLocal2, CPGC_DDRT_MIX_CFG_MC_MAIN_REG);
      CpgcMixCfg.Bits.enable_twopasscadb_always_on_shadow = 1;
      MemWritePciCfgEp (Socket, ChannelLocal2, CPGC_DDRT_MIX_CFG_MC_MAIN_REG, CpgcMixCfg.Data);
    }
    TrainingDelay (Host, 1600, 32);
  } //subch loop
  //
  // Start the test and poll the test done.
  //
  TrainingDelay (Host, 1600, 32);

  ChBitmask |= 1 << Ch;
  if (Subchannelmask == TWO_SUBCH_MASK) {
    ChBitmaskLocal = (Subchannelmask << Ch);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "ChBitmaskLocal is 0x%x\n", ChBitmaskLocal);
    ExecuteCmdSignalTest (Host, Socket, ChBitmaskLocal, NO_SUBCH);
  } else {
    if (Subchannelmask == SUB_CH_A_MASK) {
      SubCh = SUB_CH_A;
    } else if (Subchannelmask == SUB_CH_A_MASK) {
      SubCh = SUB_CH_B;
    } else {
      SubCh = 0;
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "Fail: subchannelmask value 0x%04X is not valid\n", Subchannelmask);
    }
    ExecuteCmdSignalTest (Host, Socket, ChBitmask, SubCh);
  }
  //
  // Re-enable 3DS mode
  //
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (!((1 << SubCh) & (Subchannelmask))) {
      continue;
    }

    GetChannelMapping (Ch, SubCh, &ChannelLocal);

    if ((*ChannelNvList) [Ch].enabled == 0) {
      return;
    }

    if (((*ChannelNvList) [Ch].encodedCSMode == 2) && (Host->nvram.mem.threeDsModeDisabled == 0)) {
      Toggle3DSMode (Host, Socket, ChannelLocal, 1);
    }
    //
    // Disable CADB one pass and two pass always on.
    //
    for (SubChLocal = 0; SubChLocal < SUB_CH; SubChLocal++) {
      GetChannelMapping (Ch, SubChLocal, &ChannelLocal2);
      CpgcPatCadbCtlReg.Data = MemReadPciCfgEp (Socket, ChannelLocal2, CPGC_PATCADBCTL_MCDDC_CTL_REG);
      CpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 0;
      CpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 0;
      CpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 0;
      CpgcPatCadbCtlReg.Bits.enable_twopasscadb_always_on = 0;
      CpgcPatCadbCtlReg.Bits.enable_cadb_parityntiming = 0;
      MemWritePciCfgEp (Socket, ChannelLocal2, CPGC_PATCADBCTL_MCDDC_CTL_REG, CpgcPatCadbCtlReg.Data);

      CpgcMixCfg.Data = MemReadPciCfgEp (Socket, ChannelLocal2, CPGC_DDRT_MIX_CFG_MC_MAIN_REG);
      CpgcMixCfg.Bits.enable_twopasscadb_always_on_shadow = 0;
      MemWritePciCfgEp (Socket, ChannelLocal2, CPGC_DDRT_MIX_CFG_MC_MAIN_REG, CpgcMixCfg.Data);
    }
  } //subch loop

  return;
} // WriteCadbPrechargeCmdDdr5

/**

  Read CPGC repo contents for particular mode register which is specified by Address parameter.
  For two sub-channels, the first half of data array for sub-channel A and the second half for
  sub-channel B.

  @param[in]  Socket          - Socket Id
  @param[in]  Ch              - Channel number (0-based)
  @param[in]  SubChMask       - Sub-channel bit mask
  @param[in]  Dimm            - DIMM number (0-based)
  @param[out] Data            - Data array to store the returned result for all dram devices

  @retval MRC_STATUS_SUCCESS if read is successful
  @retval MRC_STATUS_FAILURE otherwise

  **/
MRC_STATUS
EFIAPI
ReadCpgcMrrData (
  IN     UINT8  Socket,
  IN     UINT8  Ch,
  IN     UINT8  SubChMask,
  IN     UINT8  Dimm,
     OUT UINT8  (*Data)[SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  //
  // return MRC_STATUS_SUCCESS to unblock high level flow
  //
  return MRC_STATUS_SUCCESS;
}

#ifdef LRDIMM_SUPPORT
/**

  Returns the number of writes the CPGC will do during the Data Buffer Write
  DFE test.

  @param N/A

  @retval NumWrites - The number of writes the CPGC will issue every test.

**/
UINT32
EFIAPI
GetDbDfeNumCpgcWrites (
  VOID
  )
{
  // DB DFE CPGC Command Pattern is PatBack2BackWrRd. To find the number of writes, multiply
  // the cacheline number (how many writes per loop) and the total number of loops.
  return DB_DFE_CAHCE_LINES * DB_DFE_LOOP_COUNT;
}
#endif // #ifdef LRDIMM_SUPPORT

/**
 Save CPGC Rank Logical to physical mapping.

  @param[in]  Host                      - Pointer to sysHost
  @param[in]  Socket                    - Socket from which we are going to read the registers
  @param[in]  CpgcSeqRankL2PMapping     - Array to store the CPGC rank L2P mapping

  @retval VOID

**/
VOID
EFIAPI
SaveCpgcRankL2pMapping (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    CpgcSeqRankL2PMapping[MAX_CH][SUB_CH][2]
  )
{
  return;
}

/**
  Restore the CPGC rank logical to physical mapping.

  @param[in]  Host                      - Pointer to sysHost
  @param[in]  Socket                    - Socket from which we are going to read the registers
  @param[in]  CpgcSeqRankL2PMapping     - CPGC rank L2P mapping to be restored

  @retval VOID

**/
VOID
EFIAPI
RestoreCpgcRankL2pMapping (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    CpgcSeqRankL2PMapping[MAX_CH][SUB_CH][2]
  )
{
  return;
}
#endif // DDR5_SUPPORT
