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


//
// Include files
//
#include <Library/MemoryCoreLib.h>
#include <Library/MemCpgcIpLib.h>
#include "Include/MemCpgcRegs.h"
#include <UncoreCommonIncludes.h>
#include <Library/SysHostPointerLib.h>
#include "Include/CpgcChip.h"
#include "Include/MemCpgcRegCache.h"
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
#include <Library/SpdDecodeLib.h>
#include <Library/PerformanceTrackerLib.h>
#include <Memory/Ddr5RcwRegs.h>
#include <Memory/Ddr5DbRegs.h>
#include <Library/SiliconWorkaroundPMemLib.h>

#define PATTERN_REPEATS_DCADFE  255

/**

  Fix up the address bit field when programming address patterns

  @param[in]     Sig            - Signal to test
  @param[in]     Ca13Enable     - Ca13 is enabled or not
  @param[in out] Prog0          - Data to program
  @param[in out] Prog1          - Data to program

  @retval N/A

**/
STATIC VOID
FixupAddrBitsDdr5 (
  IN GSM_CSN            Sig,
  IN BOOLEAN            Ca13Enable,
  IN OUT PatCadb2Prog0  Prog0[CADB2_LINES],
  IN OUT PatCadb2Prog1  Prog1[CADB2_LINES]
  );

//
//NOP command
//
// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 NopPattern0[CADB2_LINES] = {
{ 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FFF}, // Row 0
{ 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FFF}, // Row 1
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FFF}, // Row 2
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FFF}, // Row 3
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FFF}, // Row 4
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FFF}, // Row 5
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FFF}, // Row 6
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FFF}, // Row 7
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FFF}, // Row 8
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FFF}, // Row 9
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FFF}, // Row 10
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FFF}, // Row 11
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FFF}, // Row 12
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FFF}, // Row 13
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FFF}, // Row 14
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FFF}  // Row 15
};

//
// Basic cadb pattern for Jedec commands
// Specific functions need to override CA fields for each commands
// Currently it's used for MRW, MRR and PRE
// Will align more commands to this pattern for space saving
//
// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 BasicCmdPattern0[CADB2_LINES] = {
{ 0xFE, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}, // Row 0
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}, // Row 1
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}, // Row 2
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}, // Row 3
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}, // Row 4
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}, // Row 5
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}, // Row 6
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}, // Row 7
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}, // Row 8
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}, // Row 9
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}, // Row 10
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}, // Row 11
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}, // Row 12
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}, // Row 13
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}, // Row 14
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}  // Row 15
};

// CWV A0 Workaround
// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 BasicCmdPatternCwvA00[CADB2_LINES] = {
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF }, // Row 0
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF }, // Row 1
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF }, // Row 2
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF }, // Row 3
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF }, // Row 4
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF }, // Row 5
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF }, // Row 6
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF }, // Row 7
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF }, // Row 8
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF }, // Row 9
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF }, // Row 10
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF }, // Row 11
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF }, // Row 12
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF }, // Row 13
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF }, // Row 14
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF }  // Row 15
};

//
// This can be shared by command and training patterns if needed
// Ca,  ctl,  bank
//
STATIC PatCadb2Prog1 BasicPattern1[CADB2_LINES] = {
{ 0x3FFF, 0, 0 }, // Row 0
{ 0x3FFF, 0, 0 }, // Row 1
{ 0x3FFF, 0, 0 }, // Row 2
{ 0x3FFF, 0, 0 }, // Row 3
{ 0x3FFF, 0, 0 }, // Row 4
{ 0x3FFF, 0, 0 }, // Row 5
{ 0x3FFF, 0, 0 }, // Row 6
{ 0x3FFF, 0, 0 }, // Row 7
{ 0x3FFF, 0, 0 }, // Row 8
{ 0x3FFF, 0, 0 }, // Row 9
{ 0x3FFF, 0, 0 }, // Row 10
{ 0x3FFF, 0, 0 }, // Row 11
{ 0x3FFF, 0, 0 }, // Row 12
{ 0x3FFF, 0, 0 }, // Row 13
{ 0x3FFF, 0, 0 }, // Row 14
{ 0x3FFF, 0, 0 }  // Row 15
};

STATIC PatCadb2Prog1 BasicDcaDfePattern1[CADB2_LINES] = {
{ 0x3FFE, 0, 0 }, // Row 0
{ 0x3FFE, 0, 0 }, // Row 1
{ 0x3FFF, 0, 0 }, // Row 2
{ 0x3FFF, 0, 0 }, // Row 3
{ 0x3FFE, 0, 0 }, // Row 4
{ 0x3FFE, 0, 0 }, // Row 5
{ 0x3FFF, 0, 0 }, // Row 6
{ 0x3FFF, 0, 0 }, // Row 7
{ 0x3FFE, 0, 0 }, // Row 8
{ 0x3FFE, 0, 0 }, // Row 9
{ 0x3FFF, 0, 0 }, // Row 10
{ 0x3FFF, 0, 0 }, // Row 11
{ 0x3FFE, 0, 0 }, // Row 12
{ 0x3FFE, 0, 0 }, // Row 13
{ 0x3FFF, 0, 0 }, // Row 14
{ 0x3FFF, 0, 0 }  // Row 15
};


STATIC PatCadb2Prog1 BasicDcaDfeDparPattern1[CADB2_LINES] = {
{ 0x3FFE, 0, 0 }, // Row 0
{ 0x3FFF, 0, 0 }, // Row 1
{ 0x3FFE, 0, 0 }, // Row 2
{ 0x3FFF, 0, 0 }, // Row 3
{ 0x3FFE, 0, 0 }, // Row 4
{ 0x3FFF, 0, 0 }, // Row 5
{ 0x3FFE, 0, 0 }, // Row 6
{ 0x3FFF, 0, 0 }, // Row 7
{ 0x3FFE, 0, 0 }, // Row 8
{ 0x3FFF, 0, 0 }, // Row 9
{ 0x3FFE, 0, 0 }, // Row 10
{ 0x3FFF, 0, 0 }, // Row 11
{ 0x3FFE, 0, 0 }, // Row 12
{ 0x3FFF, 0, 0 }, // Row 13
{ 0x3FFE, 0, 0 }, // Row 14
{ 0x3FFF, 0, 0 }  // Row 15
};


//
//MPC command pattern definition
//

// MPC common pattern for other commands
// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 MpcCADBPattern0[CADB2_LINES] = {
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FFF}, // Row 0
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FFF}, // Row 1
{ 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FFF}, // Row 2
{ 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FFF}, // Row 3
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}, // Row 4
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}, // Row 5
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}, // Row 6
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}, // Row 7
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}, // Row 8
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}, // Row 9
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}, // Row 10
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}, // Row 11
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}, // Row 12
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}, // Row 13
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}, // Row 14
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x0, 0x0, 0x3FFF}  // Row 15
};

//
// MPC Pattern for PDA Enumerate ID.
//
// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 MpcPdaEnumerateIdCADBPattern0[CADB2_LINES_PDA_ENUMERATE_ID] = {
{ 0xFF, 0x7, 0xff, 0xff, 0x1,    0x1, 0x0, 0x0C0F}, // Row 0
{ 0xFF, 0x7, 0xff, 0xff, 0x1,    0x1, 0x0, 0x0018}, // Row 1
{ 0xFE, 0x7, 0xff, 0xff, 0x1,    0x1, 0x0, 0x000F}, // Row 2
{ 0xFE, 0x7, 0xff, 0xff, 0x1,    0x1, 0x0, 0x0018}, // Row 3
{ 0xFF, 0x7, 0xff, 0xff, 0x1,    0x0, 0x0, 0x001F}, // Row 4
{ 0xFF, 0x7, 0xff, 0xff, 0x1,    0x0, 0x0, 0x001F}, // Row 5
};

//Ca,  ctl,  bank
STATIC PatCadb2Prog1 MpcPdaEnumerateIdCADBPattern1[CADB2_LINES_PDA_ENUMERATE_ID] = {
{ 0x0C0F, 0, 0 }, // Row 0
{ 0, 0, 0 }, // Row 1
{ 0, 0, 0 }, // Row 2
{ 0, 0, 0 }, // Row 3
{ 0, 0, 0 }, // Row 4
{ 0, 0, 0 }, // Row 5
};

//
// Pattern for PBA Enumerate
//
// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 MpcPbaEnumerateIdCADBPattern0[CADB2_LINES_PDA_ENUMERATE_ID] = {
{ 0xFE, 0x7, 0xff, 0xff, 0x0,    0x1, 0x0, 0x3245}, // Row 0
};

//Ca,  ctl,  bank
STATIC PatCadb2Prog1 MpcPbaEnumerateIdCADBPattern1[CADB2_LINES_PDA_ENUMERATE_ID] = {
{ 0x3F00, 0, 0 }, // Row 0
};

//Ca,  ctl,  bank
STATIC PatCadb2Prog1 MpcCADBPattern12N[CADB2_LINES] = {
{ 0x3FFE, 0, 0 }, // Row 0
{ 0x3FFF, 0, 0 }, // Row 1
{ 0x3FFE, 0, 0 }, // Row 2
{ 0x3FFF, 0, 0 }, // Row 3
{ 0x3FFE, 0, 0 }, // Row 4
{ 0x3FFF, 0, 0 }, // Row 5
{ 0x3FFE, 0, 0 }, // Row 6
{ 0x3FFF, 0, 0 }, // Row 7
{ 0x3FFE, 0, 0 }, // Row 8
{ 0x3FFF, 0, 0 }, // Row 9
{ 0x3FFE, 0, 0 }, // Row 10
{ 0x3FFF, 0, 0 }, // Row 11
{ 0x3FFE, 0, 0 }, // Row 12
{ 0x3FFF, 0, 0 }, // Row 13
{ 0x3FFE, 0, 0 }, // Row 14
{ 0x3FFF, 0, 0 }  // Row 15
};

// VrefCa pattern
// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 VrefCaCADBPattern0[CADB2_LINES] = {
{ 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0xFFFF}, // Row 0
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0xFFFF}, // Row 1
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0xFFFF}, // Row 2
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0xFFFF}, // Row 3
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0xFFFF}, // Row 4
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0xFFFF}, // Row 5
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0xFFFF}, // Row 6
{ 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0xFFFF}  // Row 7
};

//Ca,  ctl,  bank
STATIC PatCadb2Prog1 VrefCaCADBPattern1[CADB2_LINES] = {
{ 0x3FFF, 0x0, 0x0}, // Row 0
{ 0x3FFF, 0x0, 0x0}, // Row 1
{ 0x3FFF, 0x0, 0x0}, // Row 2
{ 0x3FFF, 0x0, 0x0}, // Row 3
{ 0x3FFF, 0x0, 0x0}, // Row 4
{ 0x3FFF, 0x0, 0x0}, // Row 5
{ 0x3FFF, 0x0, 0x0}, // Row 6
{ 0x3FFF, 0x0, 0x0}  // Row 7
};

//
// BCOMTM Pattern
//
// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 BcomPattern0[CADB2_LINES] = {
{ 0xFE, 0x0, 0x0, 0x0, 0x1, 0x1, 0x0, 0x3FE5 }, // Row 0
{ 0xFF, 0x0, 0x0, 0x0, 0x1, 0x1, 0x0, 0x3FED }, // Row 1
{ 0xFE, 0x0, 0x0, 0x0, 0x1, 0x1, 0x0, 0x3FF7 }, // Row 2
{ 0xFF, 0x0, 0x0, 0x0, 0x1, 0x1, 0x0, 0x3FFF }, // Row 3
{ 0xFE, 0x0, 0x0, 0x0, 0x1, 0x1, 0x0, 0x3FF5 }, // Row 4
{ 0xFF, 0x0, 0x0, 0x0, 0x1, 0x1, 0x0, 0x3FFD }, // Row 5
{ 0xFE, 0x0, 0x0, 0x0, 0x1, 0x1, 0x0, 0x3FE7 }, // Row 6
{ 0xFF, 0x0, 0x0, 0x0, 0x1, 0x1, 0x0, 0x3FEF }  // Row 7
};


// Ca,    ctl, bank
STATIC PatCadb2Prog1 BcomPattern1[CADB2_LINES] = {
{ 0x3FFF, 0x0, 0x0}, // Row 0
{ 0x3FFF, 0x0, 0x0}, // Row 1
{ 0x3FFF, 0x0, 0x0}, // Row 2
{ 0x3FFF, 0x0, 0x0}, // Row 3
{ 0x3FFF, 0x0, 0x0}, // Row 4
{ 0x3FFF, 0x0, 0x0}, // Row 5
{ 0x3FFF, 0x0, 0x0}, // Row 6
{ 0x3FFF, 0x0, 0x0}  // Row 7
};

//
// CSTM Pattern
//
// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 CstmPattern0[CADB2_LINES] = {
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 0
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 1
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 2
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 3
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 4
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 5
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 6
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 7
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 8
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 9
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 10
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 11
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 12
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 13
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 14
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF } // Row 15
};

//
// CATM Pattern
//
// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 CatmPatternRcd1NSimple0[CADB2_LINES] = {
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 0
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 1
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 2
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 3
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 4
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 5
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFE }, // Row 6
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }  // Row 7
};

STATIC PatCadb2Prog0 CatmPatternRcd1NSimple0_invert[CADB2_LINES] = {
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFE }, // Row 0
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFE }, // Row 1
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFE }, // Row 2
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFE }, // Row 3
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFE }, // Row 4
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFE }, // Row 5
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 6
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFE }  // Row 7
};

// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 CatmPatternRcd2NSimple0[CADB2_LINES] = {
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFE }, // Row 0
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFE }, // Row 1
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFE }, // Row 2
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFE }, // Row 3
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFE }, // Row 4
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFE }, // Row 5
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 6
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 7
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFE }, // Row 8
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFE }, // Row 9
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFE }, // Row 10
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFE }, // Row 11
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFE }, // Row 12
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFE }, // Row 13
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 14
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }  // Row 15
};

// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 CatmPatternRcd2NSimple0Par[CADB2_LINES] = {
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 0
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 1
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 2
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 3
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 4
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 5
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 6
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF },  // Row 7
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 0
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 1
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 2
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 3
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 4
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 5
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 6
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }  // Row 7
};

// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 CatmPatternNonRcdSimple0[CADB2_LINES] = {
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 0
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 1
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 2
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFE }, // Row 3
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 4
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 5
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 6
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFE }  // Row 7
};

// Catm pattern for CA0-CA13 of Rcd Dimm
// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 CatmPatternRcdXtalk0[CADB2_LINES] = {
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0      }, // Row 0
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1555 }, // Row 1
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0,   0x2AAA }, // Row 2
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0,   0x3FFF }, // Row 3
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0      }, // Row 4
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1555 }, // Row 5
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0,   0x2AAA }, // Row 6
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0,   0x3FFF }, // Row 7
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0      }, // Row 8
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1555 }, // Row 9
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0,   0x2AAA }, // Row 10
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0,   0x3FFF }, // Row 11
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0      }, // Row 12
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1555 }, // Row 13
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0,   0x2AAA }, // Row 14
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0,   0x3FFF }  // Row 15
};

// Catm pattern for CA0-CA13 of Non-Rcd Dimm
// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 CatmPatternNonRcdCa13Xtalk0[CADB2_LINES] = {
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x2000 }, // Row 0
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x2AAA }, // Row 1
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1555 }, // Row 2
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1FFF }, // Row 3
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x2000 }, // Row 4
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x2AAA }, // Row 5
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1555 }, // Row 6
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1FFF }, // Row 7
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x2000 }, // Row 8
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x2AAA }, // Row 9
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1555 }, // Row 10
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1FFF }, // Row 11
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x2000 }, // Row 12
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x2AAA }, // Row 13
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1555 }, // Row 14
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1FFF }  // Row 15
};

// Catm pattern for CA0-CA6, CA13 = 0. CA12 = 1 of Non-Rcd Dimm
// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 CatmPatternNonRcdLowBitsXtalk0[CADB2_LINES] = {
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1000 }, // Row 0
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1AAA }, // Row 1
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1555 }, // Row 2
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1FFF }, // Row 3
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1000 }, // Row 4
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1AAA }, // Row 5
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1555 }, // Row 6
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1FFF }, // Row 7
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1000 }, // Row 8
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1AAA }, // Row 9
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1555 }, // Row 10
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1FFF }, // Row 11
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1000 }, // Row 12
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1AAA }, // Row 13
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1555 }, // Row 14
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1FFF }  // Row 15
};

// Catm pattern for CA7-CA12, CA13 = 0. CA0 = 1 of Non-Rcd Dimm
// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 CatmPatternNonRcdHighBitsXtalk0[CADB2_LINES] = {
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0      }, // Row 0
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0xAAB  }, // Row 1
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1555 }, // Row 2
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1FFF }, // Row 3
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0      }, // Row 4
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0xAAB  }, // Row 5
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1555 }, // Row 6
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1FFF }, // Row 7
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0      }, // Row 8
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0xAAB }, // Row 9
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1555 }, // Row 10
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1FFF }, // Row 11
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0      }, // Row 12
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0xAAB }, // Row 13
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1555 }, // Row 14
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x1FFF }  // Row 15
};

// RCD DCA Duty Cycle pattern for CA0-CA13 of Rcd Dimm
// CS          - uniseq2, should be always asserted
// CA0 and CA7 - uniseq0, victim
// Rest of CAs - uniseq1, aggressor
// Parity      - /uniseq0, used to XOR with CA[0:6] for feedback
// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 RcdDcaDutyCyclePattern0[CADB2_LINES] = {
  { 0xFE, 0x7, 0xF, 0xF, 0x1,    0x1, 0x1, 0      }, // Row 0
  { 0xFE, 0x7, 0xF, 0xF, 0x1,    0x1, 0,   0x0081 }, // Row 1
  { 0xFE, 0x7, 0xF, 0xF, 0x1,    0x1, 0x1, 0x3F7E }, // Row 2
  { 0xFE, 0x7, 0xF, 0xF, 0x1,    0x1, 0,   0x3FFF }, // Row 3
  { 0xFF, 0x7, 0xF, 0xF, 0x1,    0x1, 0x1, 0      }, // Row 4
  { 0xFF, 0x7, 0xF, 0xF, 0x1,    0x1, 0,   0x0081 }, // Row 5
  { 0xFF, 0x7, 0xF, 0xF, 0x1,    0x1, 0x1, 0x3F7E }, // Row 6
  { 0xFF, 0x7, 0xF, 0xF, 0x1,    0x1, 0,   0x3FFF }, // Row 7
  { 0xFE, 0x7, 0xF, 0xF, 0x1,    0x1, 0x1, 0      }, // Row 8
  { 0xFE, 0x7, 0xF, 0xF, 0x1,    0x1, 0,   0x0081 }, // Row 9
  { 0xFE, 0x7, 0xF, 0xF, 0x1,    0x1, 0x1, 0x3F7E }, // Row 10
  { 0xFE, 0x7, 0xF, 0xF, 0x1,    0x1, 0,   0x3FFF }, // Row 11
  { 0xFF, 0x7, 0xF, 0xF, 0x1,    0x1, 0x1, 0      }, // Row 12
  { 0xFF, 0x7, 0xF, 0xF, 0x1,    0x1, 0,   0x0081 }, // Row 13
  { 0xFF, 0x7, 0xF, 0xF, 0x1,    0x1, 0x1, 0x3F7E }, // Row 14
  { 0xFF, 0x7, 0xF, 0xF, 0x1,    0x1, 0,   0x3FFF }  // Row 15
};

// RCD DCA DFE pattern for CA0-CA13 of Rcd Dimm
// CS          - uniseq2, should be asserted at every 4 tck
// CA0 and CA7 - uniseq0, victim
// Rest of CAs - uniseq1, aggressor
// Parity      - uniseq1, aggressor
// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 RcdDcaDfePattern0[CADB2_LINES] = {
  { 0xFE, 0x7, 0xF, 0xF, 0x1,    0x1, 0,   0      }, // Row 0
  { 0xFE, 0x7, 0xF, 0xF, 0x1,    0x1, 0,   0x0081 }, // Row 1
  { 0xFE, 0x7, 0xF, 0xF, 0x1,    0x1, 0x1, 0x3F7E }, // Row 2
  { 0xFE, 0x7, 0xF, 0xF, 0x1,    0x1, 0x1, 0x3FFF }, // Row 3
  { 0xFF, 0x7, 0xF, 0xF, 0x1,    0x1, 0,   0      }, // Row 4
  { 0xFF, 0x7, 0xF, 0xF, 0x1,    0x1, 0,   0x0081 }, // Row 5
  { 0xFF, 0x7, 0xF, 0xF, 0x1,    0x1, 0x1, 0x3F7E }, // Row 6
  { 0xFF, 0x7, 0xF, 0xF, 0x1,    0x1, 0x1, 0x3FFF }, // Row 7
  { 0xFE, 0x7, 0xF, 0xF, 0x1,    0x1, 0,   0      }, // Row 8
  { 0xFE, 0x7, 0xF, 0xF, 0x1,    0x1, 0,   0x0081 }, // Row 9
  { 0xFE, 0x7, 0xF, 0xF, 0x1,    0x1, 0x1, 0x3F7E }, // Row 10
  { 0xFE, 0x7, 0xF, 0xF, 0x1,    0x1, 0x1, 0x3FFF }, // Row 11
  { 0xFF, 0x7, 0xF, 0xF, 0x1,    0x1, 0,   0      }, // Row 12
  { 0xFF, 0x7, 0xF, 0xF, 0x1,    0x1, 0,   0x0081 }, // Row 13
  { 0xFF, 0x7, 0xF, 0xF, 0x1,    0x1, 0x1, 0x3F7E }, // Row 14
  { 0xFF, 0x7, 0xF, 0xF, 0x1,    0x1, 0x1, 0x3FFF }  // Row 15
};

// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 RcdDcaDfeDparPattern0[CADB2_LINES] = {
  { 0xFE, 0x7, 0xF, 0xF, 0x1,    0x1, 0,   0      }, // Row 0
  { 0xFE, 0x7, 0xF, 0xF, 0x1,    0x1, 0,   0x0080 }, // Row 1
  { 0xFE, 0x7, 0xF, 0xF, 0x1,    0x1, 0x1, 0x3F7F }, // Row 2
  { 0xFE, 0x7, 0xF, 0xF, 0x1,    0x1, 0x1, 0x3FFF }, // Row 3
  { 0xFF, 0x7, 0xF, 0xF, 0x1,    0x1, 0,   0      }, // Row 4
  { 0xFF, 0x7, 0xF, 0xF, 0x1,    0x1, 0,   0x0080 }, // Row 5
  { 0xFF, 0x7, 0xF, 0xF, 0x1,    0x1, 0x1, 0x3F7F }, // Row 6
  { 0xFF, 0x7, 0xF, 0xF, 0x1,    0x1, 0x1, 0x3FFF }, // Row 7
  { 0xFE, 0x7, 0xF, 0xF, 0x1,    0x1, 0,   0      }, // Row 8
  { 0xFE, 0x7, 0xF, 0xF, 0x1,    0x1, 0,   0x0080 }, // Row 9
  { 0xFE, 0x7, 0xF, 0xF, 0x1,    0x1, 0x1, 0x3F7F }, // Row 10
  { 0xFE, 0x7, 0xF, 0xF, 0x1,    0x1, 0x1, 0x3FFF }, // Row 11
  { 0xFF, 0x7, 0xF, 0xF, 0x1,    0x1, 0,   0      }, // Row 12
  { 0xFF, 0x7, 0xF, 0xF, 0x1,    0x1, 0,   0x0080 }, // Row 13
  { 0xFF, 0x7, 0xF, 0xF, 0x1,    0x1, 0x1, 0x3F7F }, // Row 14
  { 0xFF, 0x7, 0xF, 0xF, 0x1,    0x1, 0x1, 0x3FFF }  // Row 15
};


// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 CatmPatternRcd1NSimple0Par[CADB2_LINES] = {
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 0
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 1
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 2
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 3
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 4
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 5
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 6
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }  // Row 7
};

// Ca,      resverd
STATIC PatCadb2Prog1 CatmPattern1Par[CADB2_LINES] = {
  { 0x3FFF,     0x0, 0x0  }, // Row 0
  { 0x3FFF,     0x0, 0x0  }, // Row 1
  { 0x3FFF,     0x0, 0x0  }, // Row 2
  { 0x3FFF,     0x0, 0x0  }, // Row 3
  { 0x3FFF,     0x0, 0x0  }, // Row 4
  { 0x3FFF,     0x0, 0x0  }, // Row 5
  { 0x3FFE,     0x0, 0x0  }, // Row 6
  { 0x3FFF,     0x0, 0x0  },  // Row 7
  { 0x3FFF,     0x0, 0x0  }, // Row 8
  { 0x3FFF,     0x0, 0x0  }, // Row 9
  { 0x3FFF,     0x0, 0x0  }, // Row 10
  { 0x3FFF,     0x0, 0x0  }, // Row 11
  { 0x3FFF,     0x0, 0x0  }, // Row 12
  { 0x3FFF,     0x0, 0x0  }, // Row 13
  { 0x3FFF,     0x0, 0x0  }, // Row 14
  { 0x3FFF,     0x0, 0x0  }  // Row 15
};


// Ca,      resverd
STATIC PatCadb2Prog1 CatmPattern1Par_invert[CADB2_LINES] = {
  { 0x3FFE,     0x0, 0x0  }, // Row 0
  { 0x3FFE,     0x0, 0x0  }, // Row 1
  { 0x3FFE,     0x0, 0x0  }, // Row 2
  { 0x3FFE,     0x0, 0x0  }, // Row 3
  { 0x3FFE,     0x0, 0x0  }, // Row 4
  { 0x3FFE,     0x0, 0x0  }, // Row 5
  { 0x3FFF,     0x0, 0x0  }, // Row 6
  { 0x3FFE,     0x0, 0x0  },  // Row 7
  { 0x3FFE,     0x0, 0x0  }, // Row 8
  { 0x3FFE,     0x0, 0x0  }, // Row 9
  { 0x3FFE,     0x0, 0x0  }, // Row 10
  { 0x3FFE,     0x0, 0x0  }, // Row 11
  { 0x3FFE,     0x0, 0x0  }, // Row 12
  { 0x3FFE,     0x0, 0x0  }, // Row 13
  { 0x3FFE,     0x0, 0x0  }, // Row 14
  { 0x3FFE,     0x0, 0x0  }  // Row 15
};

// Ca,      resverd
STATIC PatCadb2Prog1 CatmPattern1Par2N[CADB2_LINES] = {
  { 0x3FFF,     0x0, 0x0  }, // Row 0
  { 0x3FFF,     0x0, 0x0  }, // Row 1
  { 0x3FFF,     0x0, 0x0  }, // Row 2
  { 0x3FFF,     0x0, 0x0  }, // Row 3
  { 0x3FFF,     0x0, 0x0  }, // Row 4
  { 0x3FFF,     0x0, 0x0  }, // Row 5
  { 0x3FFE,     0x0, 0x0  }, // Row 6
  { 0x3FFE,     0x0, 0x0  },  // Row 7
  { 0x3FFF,     0x0, 0x0  }, // Row 8
  { 0x3FFF,     0x0, 0x0  }, // Row 9
  { 0x3FFF,     0x0, 0x0  }, // Row 10
  { 0x3FFF,     0x0, 0x0  }, // Row 11
  { 0x3FFF,     0x0, 0x0  }, // Row 12
  { 0x3FFF,     0x0, 0x0  }, // Row 13
  { 0x3FFE,     0x0, 0x0  }, // Row 14
  { 0x3FFE,     0x0, 0x0  }  // Row 15
};

//
// QCA Simple Pattern, applicable for 1N and 2N, with and without CA13 supported
//
// Ca0 = uniseq 0
// Par = uniseq 1
// Cs  = uniseq 2
// cs,   cid, odt, cke, halfca, val, par, ca

STATIC PatCadb2Prog0 QcaPatternSimple0[CADB2_QCA_SIMPLE_PATTERN_LINES] = {
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FFE }, // Row 0
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FFF }, // Row 1
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFE }, // Row 2
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 3
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FFE }, // Row 4
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FFF }, // Row 5
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFE }, // Row 6
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }  // Row 7
};

//
// QCA Xtalk Pattern, applicable for 1N, with CA13 supported, and without CA13 supported when sweeping QCA0 to QCA9
// Ca0-Ca2 = uniseq 0
// Ca3     = uniseq 1
// Ca4     = uniseq 0 AND uniseq 2
// Ca5     = uniseq 1 AND uniseq 2
// Ca6     = uniseq 0 AND uniseq 1
// Par     = All 1s
// Cs      = uniseq 3
//
// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 QcaPattern1NXtalk0[CADB2_QCA_XTALK_PATTERN_LINES] = {
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3F80 }, // Row 0
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FC5 }, // Row 1
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FCA }, // Row 2
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3F8F }, // Row 3
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3F80 }, // Row 4
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FD5 }, // Row 5
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FEA }, // Row 6
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FBF }, // Row 7
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3F80 }, // Row 8
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FC5 }, // Row 9
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FCA }, // Row 10
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3F8F }, // Row 11
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3F80 }, // Row 12
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FD5 }, // Row 13
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FEA }, // Row 14
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FBF }  // Row 15
};
//
// QCA Xtalk Pattern, applicable for 1N, without CA13 supported when sweeping QCA10 to QCA12
// Ca0      = uniseq 0 AND uniseq 2
// Ca1      = uniseq 1 AND uniseq 2
// Ca2, Ca4 = uniseq 0
// Ca3, Ca5 = uniseq 1
// Ca6      = uniseq 0 XOR uniseq 1
// Par      = All 1s
// Cs       = uniseq 3
//
// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 QcaPattern1NQca10ToQca12WithoutCa13Xtalk0[CADB2_QCA_XTALK_PATTERN_LINES] = {
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3F80 }, // Row 0
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FD4 }, // Row 1
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FE8 }, // Row 2
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FBC }, // Row 3
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3F80 }, // Row 4
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FD5 }, // Row 5
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FEA }, // Row 6
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FBF }, // Row 7
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3F80 }, // Row 8
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FD4 }, // Row 9
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FE8 }, // Row 10
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FBC }, // Row 11
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3F80 }, // Row 12
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FD5 }, // Row 13
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FEA }, // Row 14
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FBF }  // Row 15
};

//
// QCA Xtalk Pattern, applicable for 2N, with CA13 supported
// Ca0/2/4/6 = uniseq 0
// Ca1/3/5   = uniseq 1
// Par       = uniseq 2
// Cs        = uniseq 3
//
// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 QcaPattern2NXtalk0[CADB2_QCA_XTALK_PATTERN_LINES] = {
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3F80 }, // Row 0
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FD5 }, // Row 1
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FAA }, // Row 2
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FFF }, // Row 3
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3F80 }, // Row 4
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FD5 }, // Row 5
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FAA }, // Row 6
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }, // Row 7
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3F80 }, // Row 8
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FD5 }, // Row 9
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FAA }, // Row 10
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FFF }, // Row 11
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3F80 }, // Row 12
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FD5 }, // Row 13
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FAA }, // Row 14
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FFF }  // Row 15
};

//
// QCA Xtalk Pattern, applicable for 2N, without CA13 supported when sweeping QCAs except QCA6
// Ca0/2/4   = uniseq 0
// Ca1/3/5   = uniseq 1
// Ca6       = all 0s
// Par       = uniseq 2
// Cs        = uniseq 3
//
// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 QcaPattern2NWithoutCa13Xtalk0[CADB2_QCA_XTALK_PATTERN_LINES] = {
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3F80 }, // Row 0
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3F95 }, // Row 1
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FAA }, // Row 2
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FBF }, // Row 3
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3F80 }, // Row 4
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3F95 }, // Row 5
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FAA }, // Row 6
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FBF }, // Row 7
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3F80 }, // Row 8
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3F95 }, // Row 9
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FAA }, // Row 10
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FBF }, // Row 11
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3F80 }, // Row 12
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3F95 }, // Row 13
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FAA }, // Row 14
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FBF }  // Row 15
};

//
// QCA Xtalk Pattern, applicable for 2N, without CA13 supported when sweeping QCA6
// Ca0       = uniseq 0 AND uniseq 1
// Ca1/3/5   = uniseq 0
// Ca2/4/6   = NOT uniseq 0
// Par       = uniseq 2
// Cs        = uniseq 3
//
// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 QcaPattern2NQca6WithoutCa13Xtalk0[CADB2_QCA_XTALK_PATTERN_LINES] = {
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FD4 }, // Row 0
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FAA }, // Row 1
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FD4 }, // Row 2
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FAB }, // Row 3
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FD4 }, // Row 4
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FAA }, // Row 5
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FD4 }, // Row 6
  { 0xFE, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FAB }, // Row 7
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FD4 }, // Row 8
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FAA }, // Row 9
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FD4 }, // Row 10
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x0, 0x3FAB }, // Row 11
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FD4 }, // Row 12
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FAA }, // Row 13
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FD4 }, // Row 14
  { 0xFF, 0x7, 0xf, 0xf, 0x1,    0x1, 0x1, 0x3FAB }  // Row 15
};

//
// Read Training Pattern by reading MR31 via MRR
//
// Rcd-Dimm and Non-Rcd-Dimm 1N MRR

// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 ReadTrainingPattern0[CADB2_READ_TRAINING_PATTERN_LINES] = {
{ 0xFE, 0x7, 0xf, 0xf, 0x0,  0x1, 0x1, 0x23F5 } // Row 0
};

// Ca, ctl, bank
STATIC PatCadb2Prog1 ReadTrainingPattern1[CADB2_READ_TRAINING_PATTERN_LINES] = {
{ 0x3BFC, 0x0, 0x0 } // Row 0
};

// DDRT GNT (0x1D) to MRA = 0x1F (MRR31)
// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 GntTrainingPattern0[] = {
  { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3FD }, // Row 0
};

// CW asserted high (BIT10)
// Ca, ctl, bank
STATIC PatCadb2Prog1 GntTrainingPattern1[] = {
  { 0x400, 0x0, 0x0 } // Row 0
};



// Rcd-Dimm @ 2N

// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 ReadTraining2NRcdPattern0[CADB2_READ_TRAINING_PATTERN_LINES_2N_RCD] = {
  { 0xFE, 0x7, 0xf, 0xf, 0x0,  0x1, 0x1, 0x3AF5 }, // Row 0
  { 0xFF, 0x7, 0xf, 0xf, 0x0,  0x1, 0x1, 0x3E7C }  // Row 1
};

// Ca, ctl, bank
STATIC PatCadb2Prog1 ReadTraining2NRcdPattern1[CADB2_READ_TRAINING_PATTERN_LINES_2N_RCD] = {
  { 0x23C7, 0x0, 0x0 }, // Row 0
  { 0x3BF7, 0x0, 0x0 }  // Row 1
};

// Non-Rcd-Dimm in 2N MRR

// cs,   cid, odt, cke, halfca, val, par, ca
STATIC PatCadb2Prog0 ReadTraining2NNonRcdPattern0[CADB2_READ_TRAINING_PATTERN_LINES_2N_NON_RCD] = {
  { 0xFE, 0x7, 0xf, 0xf, 0x0,  0x1, 0x1, 0x23F5 }, // Row 0
  { 0xFF, 0x7, 0xf, 0xf, 0x0,  0x1, 0x1, 0x3BFC }  // Row 1
};

// Ca, ctl, bank
STATIC PatCadb2Prog1 ReadTraining2NNonRcdPattern1[CADB2_READ_TRAINING_PATTERN_LINES_2N_NON_RCD] = {
  { 0x23F5, 0x0, 0x0 }, // Row 0
  { 0x3BFC, 0x0, 0x0 }  // Row 1
};

/**

Set subchannelmask to indicate which subchannels will be trained based on signal number and current training mode

@param[in]  Trainingstep         Current train step
@param[in]  Sig                - Signal to be trained
@param[out] SubChannelMask       Pointer to sub-channel mask

@retval n/a

**/
VOID
SubChannelSelect (
  IN     MEMFLOWS  Trainingstep,
  IN     GSM_CSN   Sig,
     OUT UINT8     *SubChannelMask
  )
{
  MRC_STATUS  Status;
  UINT8       TrainingMode;
  UINT8       SubCh;

  Status = TrainingModeSelect (Trainingstep, &TrainingMode);
  if (Status != MRC_STATUS_SUCCESS) {
    //
    // By default, use serial mode for this training step
    //
    RcDebugPrint (SDBG_MINMAX, "Warning: Training step %d doesn't support training mode selection. Use serial mode by default.\n", Trainingstep);
    TrainingMode = RUN_IN_SERIAL;
  }

  if (TrainingMode == RUN_IN_PARALLEL) {
    *SubChannelMask = TWO_SUBCH_MASK;
    return;
  }

  if (SIG_COMPARE (Sig, A0A, PARA) ||         // SubCh-A - DCA
      SIG_COMPARE (Sig, Q0ACA0, Q0BCA13) ||   // SubCh-A - QCA
      SIG_COMPARE (Sig, CS0_A_N, CS3_A_N)) {  // SubCh-A - DCS

    SubCh = SUB_CH_A;

  } else if (SIG_COMPARE (Sig, A0B, PARB) ||  // SubCh-B - DCA
      SIG_COMPARE (Sig, Q1ACA0, Q1BCA13) ||   // SubCh-B - QCA
      SIG_COMPARE (Sig, CS0_B_N, CS3_B_N)) {  // SubCh-B - DCS

    SubCh = SUB_CH_B;

  } else {
    RcDebugPrint  (SDBG_MINMAX, "Error: Invalid signal number %d\n", Sig);
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_198);
    return;
  }

  *SubChannelMask = 1 << SubCh;

  return;
}

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
  IN OUT CPGC_ERROR_STATUS CpgcErrorStatusLegacy[MAX_CH][SUB_CH]
  )
{
  CPGC_ERROR_STATUS CpgcErrorStatus[MAX_CH];
  UINT8             Ch;

  UINT8                                        MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  CpgcAdvTrainingErrorStatus (Host, Socket, DdrChEnMap, CpgcErrorStatus);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    //
    // skip inactive channels
    //
    if (!(DdrChEnMap & (1 << Ch))) {
      continue;
    }
    CpgcErrorStatusLegacy[Ch][SUB_CH_A].cpgcErrDat0S = CpgcErrorStatus[Ch].cpgcErrDat0S; // Data lane [31:0] even UI error status
    CpgcErrorStatusLegacy[Ch][SUB_CH_A].cpgcErrDat1S = 0; // Data lane [63:32] even UI error status not available
    CpgcErrorStatusLegacy[Ch][SUB_CH_A].cpgcErrDat2S = CpgcErrorStatus[Ch].cpgcErrDat2S; // Data lane [31:0] odd UI error status
    CpgcErrorStatusLegacy[Ch][SUB_CH_A].cpgcErrDat3S = 0; // Data lane [63:32] odd UI error status not available
    CpgcErrorStatusLegacy[Ch][SUB_CH_A].cpgcErrEccS  = CpgcErrorStatus[Ch].cpgcErrEccS & WORD_MASK;

    CpgcErrorStatusLegacy[Ch][SUB_CH_B].cpgcErrDat0S = CpgcErrorStatus[Ch].cpgcErrDat1S; // Data lane [31:0] even UI error status
    CpgcErrorStatusLegacy[Ch][SUB_CH_B].cpgcErrDat1S = 0; // Data lane [63:32] even UI error status not available
    CpgcErrorStatusLegacy[Ch][SUB_CH_B].cpgcErrDat2S = CpgcErrorStatus[Ch].cpgcErrDat3S; // Data lane [31:0] odd UI error status
    CpgcErrorStatusLegacy[Ch][SUB_CH_B].cpgcErrDat3S = 0; // Data lane [63:32] odd UI error status not available
    CpgcErrorStatusLegacy[Ch][SUB_CH_B].cpgcErrEccS  = (CpgcErrorStatus[Ch].cpgcErrEccS >> WORD_WIDTH) & WORD_MASK;
  }

  return;
} // CpgcAdvTrainingErrorStatusDdr5

/**

Collect DDR advanced command CPGC error status

@param[in]  Host                - Pointer to sysHost
@param[in]  Socket              - Socket to train
@param[in]  DdrChEnMap          - 4-bit bit mask of ddr channels to test - (1 = active channel)
@param[out] CpgcErrorStatus     - error status for each channel

@retval n/a

**/
VOID
CpgcAdvTrainingErrorStatusDdr51 (
  IN     PSYSHOST              Host,
  IN     UINT8                 Socket,
  IN     UINT32                DdrChEnMap,
  IN OUT CPGC_ERROR_STATUS_OLD CpgcErrorStatus[MAX_CH][SUB_CH]
  )
{

CpgcAdvTrainingErrorStatusWorker (Host, Socket, DdrChEnMap, CpgcErrorStatus);

  return ;
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

  Programs the CPGC for PDA/PBA enumeration in accordance with the CPGC dashboard.

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number
  @param[in] Channel         - Channel number
  @param[in] Dimm            - DIMM number
  @param[in] Subchannelmask  - Sub channel mask

  @retval N/A

**/
VOID
ProgramCpgcForPdaPba (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     SubchannelMask
  )
{
  UINT8                                           SubCh;
  UINT32                                          Data32;
  UINT32                                          CpgcLfsrSize[CPGC_MAX_NUMBER_DPAT_UNISEQ] = {0, 0, 0};
  UINT32                                          CpgcMuxCtl[CPGC_MAX_NUMBER_DPAT_UNISEQ] = {BTBUFFER, BTBUFFER, BTBUFFER};
  CPGC2_ADDRESS_SIZE_N0_MCCPGC_MAIN_STRUCT        AddressSize0;
  CPGC2_ADDRESS_SIZE_N1_MCCPGC_MAIN_STRUCT        AddressSize1;
  CPGC2_REGION_LOW_N0_MCCPGC_MAIN_STRUCT          RegionLow0;
  CPGC2_REGION_LOW_N1_MCCPGC_MAIN_STRUCT          RegionLow1;
  CPGC2_BASE_ADDRESS_CONTROL_MCCPGC_MAIN_STRUCT   BaseAddressControl;
  CPGC2_DATA_CONTROL_MCCPGC_MAIN_STRUCT           DataControl;
  CPGC2_BLOCK_REPEATS_MCCPGC_MAIN_STRUCT          BlockRep;
  CPGC2_BASE_REPEATS_MCCPGC_MAIN_STRUCT           BaseRepeats;
  TAddressCarry                                   AddressCarry;
  CPGC2_ADDRESS_INSTRUCTION_0_MCCPGC_MAIN_STRUCT  AddressIns;
  CPGC_SEQ_CFG_A_MCCPGC_MAIN_STRUCT               SeqCfgA;
  CPGC_SEQ_CFG_B_MCCPGC_MAIN_STRUCT               SeqCfgB;
  CPGC_DPAT_ALT_BUF_CFG_MCCPGC_MAIN_STRUCT        CpgcDpatAltBufCfg;
  CPGC_DPAT_INV_DC_MASK_LO_MCCPGC_MAIN_STRUCT     DpatInvDcMaskLo;
  CPGC_DPAT_INV_DC_MASK_HI_MCCPGC_MAIN_STRUCT     DpatInvDcMaskHi;
  CPGC_DPAT_INVDC_CFG_MCCPGC_MAIN_STRUCT          DpatInvDcCfg;

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & (SubchannelMask)) == 0) {
      continue;
    }

    //
    // Program cpgc2_address_size
    //
    GetSetCpgcRegs (Host, Socket, Channel, SubCh, CPGC_REG_READ, CPGC2_ADDRESS_SIZE_N0_MCCPGC_MAIN_REG, &AddressSize0.Data);
    AddressSize0.Bits.block_size_max_rank = 0;
    AddressSize0.Bits.block_size_max_bank = 0;
    AddressSize0.Bits.block_size_bits_row = 0;
    AddressSize0.Bits.block_size_bits_col = 0;
    GetSetCpgcRegs (Host, Socket, Channel, SubCh, CPGC_REG_WRITE, CPGC2_ADDRESS_SIZE_N0_MCCPGC_MAIN_REG, &AddressSize0.Data);
    GetSetCpgcRegs (Host, Socket, Channel, SubCh, CPGC_REG_READ, CPGC2_ADDRESS_SIZE_N1_MCCPGC_MAIN_REG, &AddressSize1.Data);
    AddressSize1.Bits.bl_chop_en = 0;
    AddressSize1.Bits.request_data_size = 0;
    AddressSize1.Bits.region_size_max_rank = 0;
    AddressSize1.Bits.region_size_max_bank = 0;
    AddressSize1.Bits.region_size_bits_row = 0;
    GetSetCpgcRegs (Host, Socket, Channel, SubCh, CPGC_REG_WRITE, CPGC2_ADDRESS_SIZE_N1_MCCPGC_MAIN_REG, &AddressSize1.Data);

    //
    // Program cpgc2_region_low
    //
    GetSetCpgcRegs (Host, Socket, Channel, SubCh, CPGC_REG_READ, CPGC2_REGION_LOW_N0_MCCPGC_MAIN_REG, &RegionLow0.Data);
    RegionLow0.Bits.low_rank = 0;
    RegionLow0.Bits.low_col = 0;
    GetSetCpgcRegs (Host, Socket, Channel, SubCh, CPGC_REG_WRITE, CPGC2_REGION_LOW_N0_MCCPGC_MAIN_REG, &RegionLow0.Data);
    GetSetCpgcRegs (Host, Socket, Channel, SubCh, CPGC_REG_READ, CPGC2_REGION_LOW_N1_MCCPGC_MAIN_REG, &RegionLow1.Data);
    RegionLow1.Bits.low_bank = 0;
    RegionLow1.Bits.low_row = 0;
    GetSetCpgcRegs (Host, Socket, Channel, SubCh, CPGC_REG_WRITE, CPGC2_REGION_LOW_N1_MCCPGC_MAIN_REG, &RegionLow1.Data);

    //
    // Program cpgc2_base_address_control
    //
    GetSetCpgcRegs (Host, Socket, Channel, SubCh, CPGC_REG_READ, CPGC2_BASE_ADDRESS_CONTROL_MCCPGC_MAIN_REG, &BaseAddressControl.Data);
    BaseAddressControl.Bits.addr_prbs_rnk_en = 0;
    BaseAddressControl.Bits.addr_prbs_bnk_en = 0;
    BaseAddressControl.Bits.addr_prbs_row_en = 0;
    BaseAddressControl.Bits.addr_prbs_col_en = 0;
    BaseAddressControl.Bits.addr_prbs_carry_mode = 0;
    BaseAddressControl.Bits.address_inversion_rate = 0;
    BaseAddressControl.Bits.address_inversion_enable = 0;
    BaseAddressControl.Bits.address_order0_inc_rate = 0;
    BaseAddressControl.Bits.reserved_rank_inc = 0;
    BaseAddressControl.Bits.bank_inc = 2;
    BaseAddressControl.Bits.col_inc = 0;
    BaseAddressControl.Bits.block_rank_move_one = 0;
    BaseAddressControl.Bits.row_inc = 0;
    BaseAddressControl.Bits.block_row_move_half_block = 0;
    BaseAddressControl.Bits.block_row_move_one_row = 0;
    BaseAddressControl.Bits.block_bank_move_one = 0;
    BaseAddressControl.Bits.hammer_inc_rate = 0;
    BaseAddressControl.Bits.block_col_move_half_block = 0;
    BaseAddressControl.Bits.block_col_move_one_col = 0;
    GetSetCpgcRegs (Host, Socket, Channel, SubCh, CPGC_REG_WRITE, CPGC2_BASE_ADDRESS_CONTROL_MCCPGC_MAIN_REG, &BaseAddressControl.Data);

    //
    // Program cpgc2_data_control
    //
    GetSetCpgcRegs (Host, Socket, Channel, SubCh, CPGC_REG_READ, CPGC2_DATA_CONTROL_MCCPGC_MAIN_REG, &DataControl.Data);
    DataControl.Bits.data_select_rotation_repeats = 0;
    GetSetCpgcRegs (Host, Socket, Channel, SubCh, CPGC_REG_WRITE, CPGC2_DATA_CONTROL_MCCPGC_MAIN_REG, &DataControl.Data);

    //
    // Program cpgc2_block_repeats
    //
    GetSetCpgcRegs (Host, Socket, Channel, SubCh, CPGC_REG_READ, CPGC2_BLOCK_REPEATS_MCCPGC_MAIN_REG, &BlockRep.Data);
    BlockRep.Bits.block_repeats = 0;
    GetSetCpgcRegs (Host, Socket, Channel, SubCh, CPGC_REG_WRITE, CPGC2_BLOCK_REPEATS_MCCPGC_MAIN_REG, &BlockRep.Data);

    //
    // Program one time write
    //
    BaseRepeats.Data = 0;
    BaseRepeats.Bits.base_repeats = 0;
    GetSetCpgcRegs (Host, Socket, Channel, SubCh, CPGC_REG_WRITE, CPGC2_BASE_REPEATS_MCCPGC_MAIN_REG, &BaseRepeats.Data);

    //
    // Program following register via instruction caching mechanism
    // - cpgc2_command_instruction
    // - cpgc2_algorithm_instruction
    // - cpgc2_algorithm_control
    // - cpgc2_data_instruction
    // - cpgc2_address_control
    //
    AddressCarry.AddressDirection = AddressDirNorth;
    AddressCarry.AddressOrder = AddressOrderColRowBankRank;
    // Same setup for PDA/PBA
    SetCpgcCmdPat (Host, Socket, Channel, SubCh, PatPdaEnum, &AddressCarry, 0);

    //
    // Program cpgc2_address_instruction
    //
    GetSetCpgcRegs (Host, Socket, Channel, SubCh, CPGC_REG_READ, CPGC2_ADDRESS_INSTRUCTION_0_MCCPGC_MAIN_REG, &Data32);
    AddressIns.Data = Data32 & 0xFF;
    AddressIns.Bits.last = 1;
    AddressIns.Bits.address_decode_enable = 0;
    AddressIns.Bits.address_direction = 0;
    AddressIns.Bits.address_order = 4;
    Data32 = AddressIns.Data;
    GetSetCpgcRegs (Host, Socket, Channel, SubCh, CPGC_REG_WRITE, CPGC2_ADDRESS_INSTRUCTION_0_MCCPGC_MAIN_REG, &Data32);

    //
    // Req gen 1 is programmed as CPGC_Active_Mode.
    //
    GetSetCpgcRegs (Host, Socket, Channel, SubCh, CPGC_REG_READ, CPGC_SEQ_CFG_A_MCCPGC_MAIN_REG, &SeqCfgA.Data);
    SeqCfgA.Bits.strict_wr_to_rd_order = 0;
    SeqCfgA.Bits.channel_assign = (SubCh == 1) ? 3 : 0;
    SeqCfgA.Bits.initialization_mode = (SubCh == 1) ? CPGC_Active_Mode : Idle_Mode;
    SeqCfgA.Bits.global_stop_bind = 0;
    SeqCfgA.Bits.global_start_bind = 0;
    SeqCfgA.Bits.global_stop_on_err_bind = 0;
    SeqCfgA.Bits.global_clear_err_bind = 0;
    GetSetCpgcRegs (Host, Socket, Channel, SubCh, CPGC_REG_WRITE, CPGC_SEQ_CFG_A_MCCPGC_MAIN_REG, &SeqCfgA.Data);

    //
    // Program cpgc_seq_cfg_b
    //
    GetSetCpgcRegs (Host, Socket, Channel, SubCh, CPGC_REG_READ, CPGC_SEQ_CFG_B_MCCPGC_MAIN_REG, &SeqCfgB.Data);
    SeqCfgB.Bits.start_delay = 0;
    GetSetCpgcRegs (Host, Socket, Channel, SubCh, CPGC_REG_WRITE, CPGC_SEQ_CFG_B_MCCPGC_MAIN_REG, &SeqCfgB.Data);

    //
    // Program cpgc_dpat_usq_cfg
    //
    SetCpgcDpatUniseqCfg (Host, Socket, Channel, SubCh, CpgcMuxCtl, CpgcLfsrSize);

    //
    // Program cpgc_dpat_alt_buf_cfg
    //
    GetSetCpgcRegs (Host, Socket, Channel, SubCh, CPGC_REG_READ, CPGC_DPAT_ALT_BUF_CFG_MCCPGC_MAIN_REG, &CpgcDpatAltBufCfg.Data);
    CpgcDpatAltBufCfg.Bits.zero_data_en = 0;
    CpgcDpatAltBufCfg.Bits.buf_strt_pntr = 0;
    CpgcDpatAltBufCfg.Bits.buf_end_pntr = 0x1F;
    CpgcDpatAltBufCfg.Bits.buf_pntr_inc_mode = 0;
    GetSetCpgcRegs (Host, Socket, Channel, SubCh, CPGC_REG_WRITE, CPGC_DPAT_ALT_BUF_CFG_MCCPGC_MAIN_REG, &CpgcDpatAltBufCfg.Data);

    //
    // Program cpgc_dpat_inv_dc_mask_hi
    //
    DpatInvDcMaskHi.Data = 0;
    DpatInvDcMaskHi.Bits.data_inv_dc_mask_hi = 0xFFFFFFFF;
    MemWritePciCfgEp (Socket, Channel, SUBCH_OFFSET_CPGC (SubCh, CPGC_DPAT_INV_DC_MASK_HI_MCCPGC_MAIN_REG), DpatInvDcMaskHi.Data);

    //
    // Program cpgc_dpat_inv_dc_mask_lo
    //
    DpatInvDcMaskLo.Data = 0;
    DpatInvDcMaskLo.Bits.data_inv_dc_mask_lo = 0xFFFFF0FF;
    MemWritePciCfgEp (Socket, Channel, SUBCH_OFFSET_CPGC (SubCh, CPGC_DPAT_INV_DC_MASK_LO_MCCPGC_MAIN_REG), DpatInvDcMaskLo.Data);

    //
    // Program cpgc_dpat_invdc_cfg
    //
    DpatInvDcCfg.Data = MemReadPciCfgEp (Socket, Channel, SUBCH_OFFSET_CPGC (SubCh, CPGC_DPAT_INVDC_CFG_MCCPGC_MAIN_REG));
    DpatInvDcCfg.Bits.ecc_inv_dc_mask = 0xFF;
    MemWritePciCfgEp (Socket, Channel, SUBCH_OFFSET_CPGC (SubCh, CPGC_DPAT_INVDC_CFG_MCCPGC_MAIN_REG), DpatInvDcCfg.Data);
  }
}

/**

This function writes MPC Command by CADB for PDA enumerate ID.

@param[in] Host            - include all the MRC data
@param[in] Socket          - Memory Controller
@param[in] Ch              - Ch to send command to
@param[in] Dimm            - Dimm to send command to
@param[in] Subchannelmask  - sub channel mask
@param[in] Rank            - CS to send the command to
@param[in] Opcode          - Opcode to use
@param[in] Timingmode      - Timingmode to use. Support MPC_1N, MPC_2N, MPC_4N.
@param[in] PassThroughMode - pass through mode

@retval N/A

**/
VOID
WriteCadbMpcCmdPdaEnumerateId (
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
  UINT8                                                  SubCh = 0;
  UINT8                                                  i;
  UINT8                                                  LogicalRank;
  UINT32                                                 CadbPatbuf[CADB_MUX_MAX] = {CADB_DSEL_UNISEQ_PBUF_0_PDA, CADB_DSEL_UNISEQ_PBUF_1_PDA, CADB_DSEL_UNISEQ_PBUF_2_PDA, CADB_DSEL_UNISEQ_PBUF_3_PDA};
  UINT32                                                 CadbMuxCtl[CADB_MUX_MAX] = {GALOIS_LFSR_MODE, GALOIS_LFSR_MODE, GALOIS_LFSR_MODE, BTBUFFER};
  UINT32                                                 CadbLfsrSize[CADB_MUX_MAX] = {0, 0, 0, 0};
  UINT32                                                 CadbPoly[CADB_MUX_MAX] = {0x8000000, 0x8000000, 0x8000000, 0};
  struct channelNvram                                    (*ChannelNvList) [MAX_CH];
  PatCadb2Prog0                                          MpcPattern0Local[CADB2_LINES];
  PatCadb2Prog1                                          MpcPattern1Local[CADB2_LINES];
  CADB_MRSCFG_N0_MCCADB_MAIN_STRUCT                      CadbMrscfgN0;
  CADB_CFG_MCCADB_MAIN_STRUCT                            CadbCfg;
  CADB_CTL_MCCADB_MAIN_STRUCT                            CadbCtl;
  CADB_DLY_MCCADB_MAIN_STRUCT                            CadbDly;
  CADB_AO_MRSCFG_MCCADB_MAIN_STRUCT                      CadbAoMrscfg;
  CADB_SELCFG_MCCADB_MAIN_STRUCT                         CadbSelCfg;

  SetCadbModeEn (Host, Socket, 1 << Ch, TRUE);

  if (Host->DdrioUltSupport) {
    return;
  }

  ChannelNvList = GetChannelNvList (Host, Socket);
  if ((*ChannelNvList) [Ch].enabled == 0) {
    return;
  }

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "WriteCadbMpcCmdPdaEnumerateId starts with Opcode%x\n", Opcode);

  if ((Opcode & 0xF0) != PDA_ENUMERATE_ID) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "Warning: the wrong MPC Pda Opcode%x\n", Opcode);
    return;
  }

  LogicalRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);
  //
  // Configure CADB
  //
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & (Subchannelmask)) == 0) {
      continue;
    }

    //
    // Program cadb_Cfg
    //
    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);
    CadbCfg.Data = 0;
    CadbCfg.Bits.initial_dsel_sseq_en = 0;
    CadbCfg.Bits.initial_dsel_en = 0;
    CadbCfg.Bits.cadb_mode = CADB_MODE_ACTIVE_SELECT_DESLECT;
    CadbCfg.Bits.cadb_to_cpgc_bind = 1;
    CadbCfg.Bits.lane_deselect_en = DRIVE_CADB_PATTERN_ON_ALL_PINS;
    CadbCfg.Bits.cadb_sel_throttle_mode = 1;
    CadbCfg.Bits.cadb_dsel_throttle_mode = 1;
    CadbCfg.Bits.cmd_deselect_start = 1;
    CadbCfg.Bits.cmd_deselect_stop = 0;
    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);

    //
    // Program cadb_ao_mrscfg
    //
    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_AO_MRSCFG_MCCADB_MAIN_REG, &CadbAoMrscfg.Data);
    CadbAoMrscfg.Bits.mrs_ao_repeats = 0;
    CadbAoMrscfg.Bits.mrs_gap = 0;
    CadbAoMrscfg.Bits.mrs_gap_scale = 0;
    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_AO_MRSCFG_MCCADB_MAIN_REG, &CadbAoMrscfg.Data);

    //
    // Program cadb_mrscfg
    //
    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_MRSCFG_N0_MCCADB_MAIN_REG, &CadbMrscfgN0.Data);
    CadbMrscfgN0.Data = 0;
    CadbMrscfgN0.Bits.mrs_start_ptr = 0;
    CadbMrscfgN0.Bits.mrs_end_ptr = 0;
    CadbMrscfgN0.Bits.mrs_goto_ptr = 0;
    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_MRSCFG_N0_MCCADB_MAIN_REG, &CadbMrscfgN0.Data);

    //
    // Program cadb_selcfg
    //
    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_SELCFG_MCCADB_MAIN_REG, &CadbSelCfg.Data);
    CadbSelCfg.Bits.initial_sel_sseq_en = 1;
    CadbSelCfg.Bits.pre_select_enable = 0;
    CadbSelCfg.Bits.act_select_enable = 0;
    CadbSelCfg.Bits.rd_select_enable = 0;
    CadbSelCfg.Bits.wr_select_enable = 7;
    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_SELCFG_MCCADB_MAIN_REG, &CadbSelCfg.Data);

    //
    // Program cadb_dly
    //
    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_DLY_MCCADB_MAIN_REG, &CadbDly.Data);
    CadbDly.Bits.stop_delay = 0xc8;
    CadbDly.Bits.start_delay = 4;
    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_DLY_MCCADB_MAIN_REG, &CadbDly.Data);

    //
    // Program cadb_dsel_uniseq_cfg
    //
    SetCadbDselUniseqCfg (Host, Socket, Ch, SubCh, CadbMuxCtl, CadbLfsrSize);

    //
    // Program cadb_dsel_uniseq_pbuf.
    //
    SetCadbDselUniseqPat (Host, Socket, Ch, SubCh, CadbPatbuf);

    //
    // Program cadb_dsel_uniseq_poly
    //
    SetCadbDselUniseqPoly (Host, Socket, Ch, SubCh, CadbPoly);
  } // SubCh loop

  for (i = 0; i < CADB2_LINES_PDA_ENUMERATE_ID; i++) {
    CopyMem (&MpcPattern0Local[i], &MpcPdaEnumerateIdCADBPattern0[i], sizeof (PatCadb2Prog0));
    CopyMem (&MpcPattern1Local[i], &MpcPdaEnumerateIdCADBPattern1[i], sizeof (PatCadb2Prog1));
  }

  FixupCS (LogicalRank, 0, MpcPattern0Local, (*ChannelNvList) [Ch].encodedCSMode);

  //
  // Update the CA Value according to CPGC2_Dashboard_v4 updating file and Program cadb_buf[0:5]
  // ca1 = ca0[13:7] + (ca0[15] << 14 )
  // ca2 = ca0[6:0]  + (ca0[14] << 14 )
  // ca3 = ca0[13:7] + (ca0[15] << 14 )
  //
  MpcPattern0Local[0].ca = MpcPattern0Local[0].ca | (Opcode << 5);
  MpcPattern1Local[0].ca = MpcPattern1Local[0].ca | (Opcode << 5);
  MpcPattern0Local[1].ca = MpcPattern0Local[1].ca | ((Opcode & 0x0c) >> 2);
  MpcPattern0Local[2].ca = MpcPattern0Local[2].ca | ((Opcode & 0x3) << 5);
  MpcPattern0Local[3].ca = MpcPattern0Local[3].ca | ((Opcode & 0x0c) >> 2);
  ProgramCADB2 (Host, Socket, Ch, Subchannelmask, 0, 6, CADB_BUF_MODE_DDR5_SELECT, FALSE, MpcPattern0Local, MpcPattern1Local);

  //
  // Configure CPGC
  //
  ProgramCpgcForPdaPba (Host, Socket, Ch, Subchannelmask);

  //
  // Delay if this is a CTE build, otherwise do nothing.
  //
  CteDelayDclk (50);

  //
  // Start Test and Poll on completion
  //
  IO_Reset (Host, Socket);

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (SubCh == 0) {
      continue;
    }
    CpgcGlobalTestStart (Host, Socket, 1 << Ch, SubCh);
  }

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (SubCh == 0) {
      continue;
    }
    CpgcPollGlobalTestDone (Host, Socket, 1 << Ch, SubCh);
  }

  //
  // Poll test completion. Also need to poll the CADB done as the test is CADB binding CPGC.
  //
  CadbPollTestDone (Host, Socket, 1 << Ch, TWO_SUBCH_MASK);


  CteDelayQclk (60);
  RunIOTest10nmDelayTarget (Host, Socket, Ch);


  //
  // Stop the test
  //
  CadbCtl.Data = 0;
  CadbCtl.Bits.start_test = 0;
  CadbCtl.Bits.stop_test = 1;
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & (Subchannelmask)) == 0) {
      continue;
    }
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CADB (SubCh, CADB_CTL_MCCADB_MAIN_REG), CadbCtl.Data);
  }

  //
  // Delay if this is a CTE build, otherwise do nothing.
  //

  CteDelayDclk (10);

  return;

} // WriteCadbMpcCmdPdaEnumerateId

/**

This function writes MPC Command by CADB in BRS

@param[in] Host            - include all the MRC data
@param[in] Socket          - Memory Controller
@param[in] Ch              - Ch to send command to
@param[in] Dimm            - Dimm to send command to
@param[in] Subchannelmask  - sub channel mask
@param[in] Rank            - CS to send the command to
@param[in] Opcode          - Opcode to use
@param[in] Timingmode      - Timingmode to use. Support MPC_1N, MPC_2N, MPC_4N.
@param[in] PassThroughMode - pass through mode

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
  UINT32                                         i;
  UINT8                                          SubCh = 0;
  struct channelNvram                            (*ChannelNvList) [MAX_CH];
  PatCadb2Prog0                                  MpcPattern0Local[CADB2_LINES];
  PatCadb2Prog1                                  MpcPattern1Local[CADB2_LINES];
  CADB_CFG_MCCADB_MAIN_STRUCT                    CadbCfg;
  CADB_AO_MRSCFG_MCCADB_MAIN_STRUCT              CadbAoMrscfg;
  UINT32                                         Data32;
  CADB_CTL_MCCADB_MAIN_STRUCT                    CadbCtl;
  UINT32                                         Patbuf[CADB_MUX_MAX];
  UINT32                                         MuxCtl[CADB_MUX_MAX] = {BTBUFFER, BTBUFFER, BTBUFFER, BTBUFFER};
  UINT32                                         LfsrSize[CADB_MUX_MAX] = {0, 0, 0, 0};

  if (((Opcode & 0xF0) == PDA_ENUMERATE_ID)) {

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "PDA Enumerate ID with opcode%x\n", Opcode);

    //
    // Clear the CS override to avoid CS got latched when PDA command sent out
    //
    OverrideCke (Socket, Ch, 0, 0);

    SetMprModeFilterActPre (Host, Socket, Ch, 1);
    WriteCadbMpcCmdPdaEnumerateId (Host, Socket, Ch, Dimm, Subchannelmask, Rank, Opcode, Timingmode, PassThroughMode);
    SetMprModeFilterActPre (Host, Socket, Ch, 0);
    return;
  }

  SetCadbModeEn (Host, Socket, 1 << Ch, TRUE);

  if (Host->DdrioUltSupport) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "ULT MPC Command  Opcode 0x%04X, timing mode %04d  Rank %d \n", Opcode, Timingmode, GetPhyRank (Dimm, Rank));
    CteDdrCmd ("mpc", 1 << Ch, Subchannelmask, GetPhyRank (Dimm, Rank), Opcode, Timingmode);
    return;
  }

  ChannelNvList = GetChannelNvList (Host, Socket);
  if ((*ChannelNvList) [Ch].enabled == 0) {
    return;
  }

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "WriteCadbMpcCmd starts\n");

  //
  // Configure CADB
  //


  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & (Subchannelmask)) == 0) {
      continue;
    }
    CadbAoMrscfg.Data = 0;
    CadbAoMrscfg.Bits.mrs_ao_repeats = 1;
    CadbAoMrscfg.Bits.mrs_gap = 15;
    CadbAoMrscfg.Bits.mrs_gap_scale = 0;

    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);
    CadbCfg.Bits.cadb_mode = CADB_MODE_ALWAYS_ON;
    CadbCfg.Bits.initial_dsel_sseq_en = 0;
    CadbCfg.Bits.initial_dsel_en = 1;
    CadbCfg.Bits.cadb_to_cpgc_bind = 1;
    //
    // Drive CADB pattern on all pins including (CS, CA and PAR)
    //
    CadbCfg.Bits.lane_deselect_en = DRIVE_CADB_PATTERN_ON_ALL_PINS;
    CadbCfg.Bits.cmd_deselect_start = 7;
    CadbCfg.Bits.cmd_deselect_stop = 0;
    if ((*ChannelNvList) [Ch].timingMode == TIMING_1N) {
      CadbCfg.Bits.cadb_dsel_throttle_mode = 1;
      CadbCfg.Bits.cadb_sel_throttle_mode = 1;
    } else {
      CadbCfg.Bits.cadb_dsel_throttle_mode = 2;
      CadbCfg.Bits.cadb_sel_throttle_mode = 2;
    }
    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);

    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_AO_MRSCFG_MCCADB_MAIN_REG, &CadbAoMrscfg.Data);
    // No delay required
    Data32 = 0;
    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_DLY_MCCADB_MAIN_REG, &Data32);

    SetCadbDselUniseqCfg (Host, Socket, Ch, SubCh, MuxCtl, LfsrSize);

    //
    // Program the CADB buffer pattern.
    //
    if ((*ChannelNvList) [Ch].timingMode == TIMING_1N) {
      Patbuf[0] = CADB_DSEL_UNISEQ_PBUF_0_MPC_1N;
      Patbuf[1] = CADB_DSEL_UNISEQ_PBUF_1_MPC_1N;
    } else {
      Patbuf[0] = CADB_DSEL_UNISEQ_PBUF_0_MPC_2N;
      Patbuf[1] = CADB_DSEL_UNISEQ_PBUF_1_MPC_2N;
    }
    Patbuf[2] = 0;
    Patbuf[3] = 0;
    SetCadbDselUniseqPat (Host, Socket, Ch, SubCh, Patbuf);
  } //subch loop

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "MPC with Opcode%x\n", Opcode);

  for (i = 0; i < CADB2_LINES; i++) {
    CopyMem (&MpcPattern0Local[i], &MpcCADBPattern0[i], sizeof (PatCadb2Prog0));
    if ((*ChannelNvList) [Ch].timingMode == TIMING_1N) {
      CopyMem (&MpcPattern1Local[i], &BasicPattern1[i], sizeof (PatCadb2Prog1));
    } else {
      CopyMem (&MpcPattern1Local[i], &MpcCADBPattern12N[i], sizeof (PatCadb2Prog1));
    }
    if ((i % 2) == 0) {
      MpcPattern0Local[i].ca = MPC1 (Opcode);
    } else {
      MpcPattern0Local[i].ca = MPC2 (Opcode);
    }
  }

  FixupCS (GetRankIndex (Socket, Ch, Dimm, Rank), 0, MpcPattern0Local, (*ChannelNvList) [Ch].encodedCSMode);
  ProgramCADB2 (Host, Socket, Ch, Subchannelmask, 0, CADB2_LINES, CADB_BUF_MODE_DDR5_DESELECT, FALSE, MpcPattern0Local, MpcPattern1Local);

  //
  // Delay if this is a CTE build, otherwise do nothing.
  //
  CteDelayDclk (50);

  //
  // Start test.
  //

  CadbTestStart (Host, Socket, (1 << Ch), Subchannelmask);

  //
  // Poll test completion
  //
  CadbPollTestDone (Host, Socket, 1 << Ch, Subchannelmask);
  //
  // Stop the test
  //
  CadbCtl.Data = 0;
  CadbCtl.Bits.start_test = 0;
  CadbCtl.Bits.stop_test = 1;
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & (Subchannelmask)) == 0) {
      continue;
    }
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CADB (SubCh, CADB_CTL_MCCADB_MAIN_REG), CadbCtl.Data);
  }
  //
  // In EarlyCaBacksideLoopbackTest CTE non-I3C test, need to use the inband MRW, need to keep the CADB mode enable during whole backside
  // QCA training, no need to do the enable it, disbale it, enable it sequence.
  //
  if (GetCurrentTestType (Socket) != EarlyCaBacksideLoopbackTest) {
    //
    // Disable CADB Mode enable bit.
    //
    SetCadbModeEn (Host, Socket, 1 << Ch, FALSE);
    //
    // Add the delay to wait for the bit change to take effective.
    //
    TrainingDelay (Host, 5 * 16, 1);
  }
  //
  // Delay if this is a CTE build, otherwise do nothing.
  //

  CteDelayDclk (10);

  return;

} // WriteCadbMpcCmd

/**

  This function sends the NOP command by CADB for DDR5.

  @param[in] Host            - include all the MRC data
  @param[in] Socket          - socket number
  @param[in] Channel         - ch to send command to
  @param[in] Dimm            - Dimm to send command to
  @param[in] Subchannelmask  - sub channel mask
  @param[in] Rank            - CS to send the command to
  @param[in] Cw              - CW to use
  @param[in] Cmd             - which command: NOP_CMD
  @param[in] PassThroughMode - Pass Through Mode
  @param[in] ExitCATM        - exit catm

  @retval N/A

**/
VOID
WriteCadbNopCmdExitCatm (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Subchannelmask,
  IN UINT8     Rank,
  IN UINT8     Cw,
  IN UINT8     Cmd,
  IN BOOLEAN   PassThroughMode,
  IN BOOLEAN   ExitCATM
  )
{
  UINT32                                         i;
  UINT8                                          SubCh = 0;
  struct channelNvram                            (*ChannelNvList) [MAX_CH];
  PatCadb2Prog0                                  MpcPattern0Local[CADB2_LINES];
  PatCadb2Prog1                                  MpcPattern1Local[CADB2_LINES];
  CADB_CFG_MCCADB_MAIN_STRUCT                    CadbCfg;
  CADB_AO_MRSCFG_MCCADB_MAIN_STRUCT              CadbAoMrscfg;
  UINT32                                         Data32;
  CADB_CTL_MCCADB_MAIN_STRUCT                    CadbCtl;
  UINT32                                         Patbuf[CADB_MUX_MAX];
  UINT32                                         MuxCtl[CADB_MUX_MAX] = {BTBUFFER, BTBUFFER, BTBUFFER, BTBUFFER};
  UINT32                                         LfsrSize[CADB_MUX_MAX] = {0, 0, 0, 0};
  UINT8                                          LogicalRank;

  LogicalRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);

  SetCadbModeEn (Host, Socket, 1 << Ch, TRUE);

  ChannelNvList = GetChannelNvList (Host, Socket);
  if ((*ChannelNvList) [Ch].enabled == 0) {
    return;
  }

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "WriteCadbNopCmd2 starts\n");

  //
  // Configure CADB
  //

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & (Subchannelmask)) == 0) {
      continue;
    }
    CadbAoMrscfg.Data = 0;
    CadbAoMrscfg.Bits.mrs_ao_repeats = 1;
    CadbAoMrscfg.Bits.mrs_gap = 15;
    CadbAoMrscfg.Bits.mrs_gap_scale = 0;

    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);
    CadbCfg.Bits.cadb_mode = CADB_MODE_ALWAYS_ON;
    CadbCfg.Bits.initial_dsel_sseq_en = 0;
    CadbCfg.Bits.initial_dsel_en = 1;
    CadbCfg.Bits.cadb_to_cpgc_bind = 1;
    //
    // Drive CADB pattern on all pins including (CS, CA and PAR)
    //
    CadbCfg.Bits.lane_deselect_en = DRIVE_CADB_PATTERN_ON_ALL_PINS;
    CadbCfg.Bits.cmd_deselect_start = 7;
    CadbCfg.Bits.cmd_deselect_stop = 0;
    if ((*ChannelNvList) [Ch].timingMode == TIMING_1N) {
      CadbCfg.Bits.cadb_dsel_throttle_mode = 1;
      CadbCfg.Bits.cadb_sel_throttle_mode = 1;
    } else {
      CadbCfg.Bits.cadb_dsel_throttle_mode = 2;
      CadbCfg.Bits.cadb_sel_throttle_mode = 2;
    }
    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);

    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_AO_MRSCFG_MCCADB_MAIN_REG, &CadbAoMrscfg.Data);
    // No delay required
    Data32 = 0;
    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_DLY_MCCADB_MAIN_REG, &Data32);

    SetCadbDselUniseqCfg (Host, Socket, Ch, SubCh, MuxCtl, LfsrSize);

    //
    // Program the CADB buffer pattern.
    //
    if ((*ChannelNvList) [Ch].timingMode == TIMING_1N) {
      Patbuf[0] = CADB_DSEL_UNISEQ_PBUF_0_MPC_1N;
      Patbuf[1] = CADB_DSEL_UNISEQ_PBUF_1_MPC_1N;
    } else {
      Patbuf[0] = CADB_DSEL_UNISEQ_PBUF_0_MPC_2N;
      Patbuf[1] = CADB_DSEL_UNISEQ_PBUF_1_MPC_2N;
    }
    Patbuf[2] = 0;
    Patbuf[3] = 0;
    SetCadbDselUniseqPat (Host, Socket, Ch, SubCh, Patbuf);
  } //subch loop


  for (i = 0; i < CADB2_LINES; i++) {
    CopyMem (&MpcPattern0Local[i], &MpcCADBPattern0[i], sizeof (PatCadb2Prog0));
    if ((*ChannelNvList) [Ch].timingMode == TIMING_1N) {
      CopyMem (&MpcPattern1Local[i], &BasicPattern1[i], sizeof (PatCadb2Prog1));
    } else {
      CopyMem (&MpcPattern1Local[i], &MpcCADBPattern12N[i], sizeof (PatCadb2Prog1));
    }
  }
  FixupCS (LogicalRank, 0, MpcPattern0Local, (*ChannelNvList) [Ch].encodedCSMode);
  ProgramCADB2 (Host, Socket, Ch, Subchannelmask, 0, CADB2_LINES, CADB_BUF_MODE_DDR5_DESELECT, FALSE, MpcPattern0Local, MpcPattern1Local);

  //
  // Delay if this is a CTE build, otherwise do nothing.
  //
  CteDelayDclk (50);

  //
  // Start test.
  //
  CadbTestStart (Host, Socket, (1 << Ch), Subchannelmask);

  //
  // Poll test completion
  //
  CadbPollTestDone (Host, Socket, 1 << Ch, Subchannelmask);
  //
  // Stop the test
  //
  CadbCtl.Data = 0;
  CadbCtl.Bits.start_test = 0;
  CadbCtl.Bits.stop_test = 1;
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & (Subchannelmask)) == 0) {
      continue;
    }
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CADB (SubCh, CADB_CTL_MCCADB_MAIN_REG), CadbCtl.Data);
  }
  //
  // In EarlyCaBacksideLoopbackTest CTE non-I3C test, need to use the inband MRW, need to keep the CADB mode enable during whole backside
  // QCA training, no need to do the enable it, disbale it, enable it sequence.
  //
  if (GetCurrentTestType (Socket) != EarlyCaBacksideLoopbackTest) {
    //
    // Disable CADB Mode enable bit.
    //
    SetCadbModeEn (Host, Socket, 1 << Ch, FALSE);
    //
    // Add the delay to wait for the bit change to take effective.
    //
    TrainingDelay (Host, 5 * 16, 1);
  }
  //
  // Delay if this is a CTE build, otherwise do nothing.
  //

  CteDelayDclk (10);

  return;

} // WriteCadbNopCmdExitCatm

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
  UINT32                                         Line;
  UINT8                                          SubCh = 0;
  struct channelNvram                            (*ChannelNvList) [MAX_CH];
  PatCadb2Prog0                                  MpcPattern0Local[CADB2_LINES];
  PatCadb2Prog1                                  MpcPattern1Local[CADB2_LINES];
  CADB_CFG_MCCADB_MAIN_STRUCT                    CadbCfg;
  CADB_AO_MRSCFG_MCCADB_MAIN_STRUCT              CadbAoMrscfg;
  UINT32                                         Data32;
  CADB_CTL_MCCADB_MAIN_STRUCT                    CadbCtl;
  UINT32                                         Patbuf[CADB_MUX_MAX];
  UINT32                                         MuxCtl[CADB_MUX_MAX] = {BTBUFFER, BTBUFFER, BTBUFFER, BTBUFFER};
  UINT32                                         LfsrSize[CADB_MUX_MAX] = {0, 0, 0, 0};
  UINT8                                          LogicalRank;

  LogicalRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);
  SetCadbModeEn (Host, Socket, 1 << Ch, TRUE);

  if (Host->DdrioUltSupport) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "ULT VrefCa Command  Opcode 0x%04X, timing mode %04d \n", Opcode, Timingmode);
    if (CsCaMode == CA_MODE) {
      if (Timingmode == TIMING_SINGLE_CS) {
        CteDdrCmd ("vrefca", 1 << Ch, Subchannelmask, GetPhyRank (Dimm, Rank), Opcode, TIMING_1N);
      } else {
        CteDdrCmd ("vrefca", 1 << Ch, Subchannelmask, GetPhyRank (Dimm, Rank), Opcode, TIMING_4N);
      }
    } else {
      if (Timingmode == TIMING_SINGLE_CS) {
        CteDdrCmd ("vrefcs", 1 << Ch, Subchannelmask, GetPhyRank (Dimm, Rank), Opcode, TIMING_1N);
      } else {
        CteDdrCmd ("vrefcs", 1 << Ch, Subchannelmask, GetPhyRank (Dimm, Rank), Opcode, TIMING_4N);
        }
    }
    return;
  }

  ChannelNvList = GetChannelNvList (Host, Socket);
  if ((*ChannelNvList) [Ch].enabled == 0) {
    return;
  }

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "WriteCadbVrefCaCmd starts\n");

  //
  // Configure CADB
  //
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & (Subchannelmask)) == 0) {
      continue;
    }
    CadbAoMrscfg.Data = 0;
    CadbAoMrscfg.Bits.mrs_ao_repeats = 1;
    CadbAoMrscfg.Bits.mrs_gap = 15;
    CadbAoMrscfg.Bits.mrs_gap_scale = 0;

    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);
    CadbCfg.Bits.cadb_mode = CADB_MODE_ALWAYS_ON;
    CadbCfg.Bits.initial_dsel_sseq_en = 0;
    CadbCfg.Bits.initial_dsel_en = 1;
    CadbCfg.Bits.cadb_to_cpgc_bind = 1;
    //
    // Drive CADB pattern on all pins including (CS, CA and PAR)
    //
    CadbCfg.Bits.lane_deselect_en = DRIVE_CADB_PATTERN_ON_ALL_PINS;
    CadbCfg.Bits.cmd_deselect_start = 7;
    CadbCfg.Bits.cmd_deselect_stop = 0;
    if ((*ChannelNvList) [Ch].timingMode == TIMING_1N) {
      CadbCfg.Bits.cadb_dsel_throttle_mode = 1;
      CadbCfg.Bits.cadb_sel_throttle_mode = 1;
    } else {
      CadbCfg.Bits.cadb_dsel_throttle_mode = 2;
      CadbCfg.Bits.cadb_sel_throttle_mode = 2;
    }
    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);

    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_AO_MRSCFG_MCCADB_MAIN_REG, &CadbAoMrscfg.Data);
    // No delay required
    Data32 = 0;
    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_DLY_MCCADB_MAIN_REG, &Data32);

    SetCadbDselUniseqCfg (Host, Socket, Ch, SubCh, MuxCtl, LfsrSize);

    //
    // Program the CADB buffer pattern.
    //
    if ((*ChannelNvList) [Ch].timingMode == TIMING_1N) {
      Patbuf[0] = CADB_DSEL_UNISEQ_PBUF_0_MPC_1N;
      Patbuf[1] = CADB_DSEL_UNISEQ_PBUF_1_MPC_1N;
    } else {
      Patbuf[0] = CADB_DSEL_UNISEQ_PBUF_0_MPC_2N;
      Patbuf[1] = CADB_DSEL_UNISEQ_PBUF_1_MPC_2N;
    }
    Patbuf[2] = 0;
    Patbuf[3] = 0;
    SetCadbDselUniseqPat (Host, Socket, Ch, SubCh, Patbuf);
  } //subch loop

  if (CsCaMode == CA_MODE) {
    RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "VREFA with Opcode%x\n", Opcode);
  } else {
      RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "VREFS with Opcode%x\n", Opcode);
  }

  for (Line = 0; Line < CADB2_LINES; Line++) {
    CopyMem (&MpcPattern0Local[Line], &MpcCADBPattern0[Line], sizeof (PatCadb2Prog0));
    if ((*ChannelNvList) [Ch].timingMode == TIMING_1N) {
      CopyMem (&MpcPattern1Local[Line], &BasicPattern1[Line], sizeof (PatCadb2Prog1));
    } else {
      CopyMem (&MpcPattern1Local[Line], &MpcCADBPattern12N[Line], sizeof (PatCadb2Prog1));
    }

    if (CsCaMode == CA_MODE) {
      if ((Line % 2) == 0) {
        MpcPattern0Local[Line].ca = VREFCA1 (Opcode);
      } else {
        MpcPattern0Local[Line].ca = VREFCA2 (Opcode);
      }
    } else {
      if ((Line % 2) == 0) {
        MpcPattern0Local[Line].ca = VREFCS1 (Opcode);
      } else {
        MpcPattern0Local[Line].ca = VREFCS2 (Opcode);
      }
    }
  }
  FixupCS (LogicalRank, 0, MpcPattern0Local, (*ChannelNvList) [Ch].encodedCSMode);
  ProgramCADB2 (Host, Socket, Ch, Subchannelmask, 0, CADB2_LINES, CADB_BUF_MODE_DDR5_DESELECT, FALSE, MpcPattern0Local, MpcPattern1Local);

  //
  // Delay if this is a CTE build, otherwise do nothing.
  //
  CteDelayDclk (50);

  //
  // Start test.
  //
  CadbCtl.Data = 0;
  CadbCtl.Bits.start_test = 1;
  CadbCtl.Bits.stop_test = 0;
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & (Subchannelmask)) == 0) {
      continue;
    }
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CADB (SubCh, CADB_CTL_MCCADB_MAIN_REG), CadbCtl.Data);
  }

  //
  // Poll test completion
  //
  CadbPollTestDone (Host, Socket, 1 << Ch, Subchannelmask);
  //
  // Stop the test
  //
  CadbCtl.Data = 0;
  CadbCtl.Bits.start_test = 0;
  CadbCtl.Bits.stop_test = 1;
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & (Subchannelmask)) == 0) {
      continue;
    }
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CADB (SubCh, CADB_CTL_MCCADB_MAIN_REG), CadbCtl.Data);
  }
  //
  // In EarlyCaBacksideLoopbackTest CTE non-I3C test, need to use the inband MRW, need to keep the CADB mode enable during whole backside
  // QCA training, no need to do the enable it, disbale it, enable it sequence.
  //
  if (GetCurrentTestType (Socket) != EarlyCaBacksideLoopbackTest) {
    //
    // Disable CADB Mode enable bit.
    //
    SetCadbModeEn (Host, Socket, 1 << Ch, FALSE);
    //
    // Add the delay to wait for the bit change to take effective.
    //
    TrainingDelay (Host, 5 * 16, 1);
  }
  //
  // Delay if this is a CTE build, otherwise do nothing.
  //
  CteDelayDclk (10);

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

  WriteCadbVrefCsCaCmd (Host, Socket, Ch, Dimm, Subchannelmask, Rank, Opcode, Timingmode, PassThroughMode, CS_MODE);
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

This function is to format Mrr and Mrw pattern in CADB buffer only when in always-on mode according to timing mode and dimm type
Mrw and Mrr are 2 UI commands (x means don't care)
          non-RCD DIMM (UDIMM, SODIMM)                 RCD DIMM (RDIMM, LRDIMM)
   1N     2 CADB rows using CA[13:0]                   4 CADB rows using only CA[6:0]
          cs_row0: 0, CA[13:0]: CmdUi0                 cs_row0: 0, CA[6:0]: CmdUi0[6:0]    cs_row2: 1, CA[6:0]: CmdUi1[6:0]
          cs_row1: 1, CA[13:0]: CmdUi1                 cs_row1: x, CA[6:0]: CmdUi0[13:7]   cs_row3: x, CA[6:0]: CmdUi1[13:7]
   2N     4 CADB rows using CA[13:0]                   8 CADB rows using only CA[6:0]
          cs_row0: 0, CA[13:0]: CmdUi0                 cs_row0: 0, CA[6:0]: CmdUi0[6:0]    cs_row4: 1, CA[6:0]: CmdUi1[6:0]
          cs_row1: 1, CA[13:0]: CmdUi0                 cs_row1: x, CA[6:0]: CmdUi0[6:0]    cs_row5: x, CA[6:0]: CmdUi1[6:0]
          cs_row2: 1, CA[13:0]: CmdUi1                 cs_row2: 1, CA[6:0]: CmdUi0[13:7]   cs_row6: 1, CA[6:0]: CmdUi1[13:7]
          cs_row3: 1, CA[13:0]: CmdUi1                 cs_row3: x, CA[6:0]: CmdUi0[13:7]   cs_row7: x, CA[6:0]: CmdUi1[13:7]
@param[in]  Host               - Pointer to sysHost
@param[in]  Socket             - Socket number
@param[in]  Ch                 - Ch to send command to
@param[in]  Dimm               - Dimm to send command to
@param[in]  TimingMode         - 1N or 2N timing mode
@param[in]  DimmType           - UDIMM, SODIMM, RDIMM or LRDIMM
@param[in]  Opcode             - Opcode to use
@param[in]  Cw                 - CW to use
@param[in]  Cmd                - which command: MRR_CMD or MRW_CMD
@param[out] MrPattern0Local    - CADB low 32-bit buffer
@param[out] MrPattern1Local    - CADB high 32-bit buffer
@param[out] NumLine            - Number of CADB buffer rows for formatted patterns

@retval N/A

**/
VOID
FormatAlwaysOnMrCmdPattern (
  IN   PSYSHOST       Host,
  IN   UINT8          Socket,
  IN   UINT8          Ch,
  IN   UINT8          Dimm,
  IN   UINT8          TimingMode,
  IN   UINT8          DimmType,
  IN   UINT16         Address,
  IN   UINT16         Opcode,
  IN   UINT8          Cw,
  IN   UINT8          Cmd,
  OUT  PatCadb2Prog0  *MrPattern0Local,
  OUT  PatCadb2Prog1  *MrPattern1Local,
  OUT  UINT8          *NumLine
  )
{
  UINT8 Line;

  for (Line = 0; Line < CADB2_LINES; Line++) {
    if (IsPmemSiliconWorkaroundEnabled(Host, Socket, Ch, Dimm, "S14011274250") && (Host->nvram.mem.socket[Socket].ddrFreq > DDR_3600)) {
      CopyMem(&MrPattern0Local[Line], &BasicCmdPatternCwvA00[Line], sizeof(PatCadb2Prog0));
    } else {
      CopyMem(&MrPattern0Local[Line], &BasicCmdPattern0[Line], sizeof(PatCadb2Prog0));
    }
    CopyMem (&MrPattern1Local[Line], &BasicPattern1[Line], sizeof (PatCadb2Prog1));
  }

  if (Cmd == MRR_CMD) {

    MrPattern0Local[0].ca = MRRL (MrPattern0Local[0].ca, Address);
    MrPattern1Local[0].ca = MRRH (MrPattern1Local[0].ca, Cw);
    MrPattern0Local[1].ca = 0xFFFF; // Dsel
    MrPattern1Local[1].ca = 0xFFFF; // Dsel
    MrPattern0Local[2].ca = 0xFFFF; // Dsel
    MrPattern1Local[2].ca = 0xFFFF; // Dsel
    MrPattern0Local[3].ca = 0xFFFF; // Dsel
    MrPattern1Local[3].ca = 0xFFFF; // Dsel
    MrPattern0Local[0].val = 1;
    MrPattern0Local[1].val = 1;
    MrPattern0Local[2].val = 1;
    MrPattern0Local[3].val = 1;
    *NumLine = 4;

  } else {
    //
    // MRW patterns
    //
    if (TimingMode == TIMING_1N) {
      //
      // 1N MRW
      //
      if ((DimmType == UDIMM) || (DimmType == SODIMM)) {
        MrPattern0Local[0].ca = MRWL (MrPattern0Local[0].ca, Address);
        MrPattern0Local[1].ca = MRWH (MrPattern0Local[1].ca, Opcode, Cw);
        MrPattern0Local[0].val = 1;
        MrPattern0Local[1].val = 1;
        *NumLine = 2;
      } else if ((DimmType == RDIMM) || (DimmType == LRDIMM)) {
        MrPattern0Local[0].ca = MRWL1N0 (Address);
        MrPattern0Local[1].ca = MRWL1N1 (Address);
        MrPattern0Local[2].ca = MRWH1N0 (Opcode, Cw);
        MrPattern0Local[3].ca = MRWH1N1 (Opcode, Cw);
        MrPattern0Local[0].val = 1;
        MrPattern0Local[1].val = 1;
        MrPattern0Local[2].val = 1;
        MrPattern0Local[3].val = 1;
        *NumLine = 4;
      }
    } else {
      //
      // 2N MRW
      //
      if ((DimmType == UDIMM) || (DimmType == SODIMM)) {
        MrPattern0Local[0].ca = MRWL (MrPattern0Local[0].ca, Address);
        MrPattern0Local[1].ca = MrPattern0Local[0].ca;
        MrPattern0Local[2].ca = MRWH (MrPattern0Local[1].ca, Opcode, Cw);
        MrPattern0Local[3].ca = MrPattern0Local[2].ca;
        MrPattern0Local[0].val = 1;
        MrPattern0Local[1].val = 1;
        MrPattern0Local[2].val = 1;
        MrPattern0Local[3].val = 1;
        *NumLine = 4;
      } else if ((DimmType == RDIMM) || (DimmType == LRDIMM)) {
        MrPattern0Local[0].ca = MRWLL (MrPattern0Local[0].ca, Address);
        MrPattern0Local[1].ca = MrPattern0Local[0].ca;
        MrPattern0Local[2].ca = MRWLH (MrPattern0Local[0].ca, Address);
        MrPattern0Local[3].ca = MrPattern0Local[2].ca;
        MrPattern0Local[4].ca = MRWHL (MrPattern0Local[1].ca, Opcode, Cw);
        MrPattern0Local[5].ca = MrPattern0Local[4].ca;
        MrPattern0Local[6].ca = MRWHH (MrPattern0Local[1].ca, Opcode, Cw);
        MrPattern0Local[7].ca = MrPattern0Local[6].ca;
        MrPattern0Local[0].val = 1;
        MrPattern0Local[1].val = 1;
        MrPattern0Local[2].val = 1;
        MrPattern0Local[3].val = 1;
        MrPattern0Local[4].val = 1;
        MrPattern0Local[5].val = 1;
        MrPattern0Local[6].val = 1;
        MrPattern0Local[7].val = 1;
        *NumLine = 8;
      }
    }
  }
}

/**

  Programs the CADB for PBA enumeration in accordance with the CPGC dashboard.

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number
  @param[in] Channel         - Channel to send command to
  @param[in] Dimm            - DIMM to send command to
  @param[in] Subchannelmask  - Sub channel mask
  @param[in] Rank            - CS to send the command to
  @param[in] Opcode          - PBA enumerate ID

  @retval N/A

**/
VOID
ProgramCadbForPba (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT8     SubchannelMask,
  IN UINT8     Rank,
  IN UINT16    Opcode
  )
{
  UINT8                              SubCh;
  UINT8                              StartLine = 0;
  UINT8                              EndLine = 1;  // Only programming 1 line for PBA
  UINT8                              LogicalRank;
  struct channelNvram                (*ChannelNvList) [MAX_CH];
  PatCadb2Prog0                      MrPattern0Local[CADB2_LINES];
  PatCadb2Prog1                      MrPattern1Local[CADB2_LINES];
  CPGC_MISCCKECTL_MCDDC_CTL_STRUCT   CpgcMiscCkeCtlReg;
  CADB_CTL_MCCADB_MAIN_STRUCT        CadbCtl;
  CADB_CFG_MCCADB_MAIN_STRUCT        CadbCfg;
  CADB_DLY_MCCADB_MAIN_STRUCT        CadbDly;
  CADB_MRSCFG_N0_MCCADB_MAIN_STRUCT  CadbMrscfgN0;
  CADB_MRSCFG_N1_MCCADB_MAIN_STRUCT  CadbMrscfgN1;
  CADB_AO_MRSCFG_MCCADB_MAIN_STRUCT  CadbAoMrscfg;
  CADB_SELCFG_MCCADB_MAIN_STRUCT     CadbSelCfg;

  ChannelNvList = GetChannelNvList (Host, Socket);
  LogicalRank = GetLogicalRank (Host, Socket, Channel, Dimm, Rank);

  // Filter out activate and precharge commands
  SetMprModeFilterActPre (Host, Socket, Channel, 1);

  SetCadbModeEn (Host, Socket, 1 << Channel, TRUE);
  //
  // Configure CADB
  //
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & (SubchannelMask)) == 0) {
      continue;
    }

    // Setup CADB CFG
    GetSetCadbRegs (Host, Socket, Channel, SubCh, CPGC_REG_READ, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);

    CadbCfg.Bits.cadb_mode = CADB_MODE_ACTIVE_SELECT_DESLECT; // Select and trigger driven Deselect (0x7) for PBA
    CadbCfg.Bits.cadb_to_cpgc_bind = 1;
    if ((*ChannelNvList) [Channel].timingMode == TIMING_1N) {
      CadbCfg.Bits.cadb_dsel_throttle_mode = 1;
      CadbCfg.Bits.cadb_sel_throttle_mode = 1;
    } else {
      CadbCfg.Bits.cadb_dsel_throttle_mode = 2;
      CadbCfg.Bits.cadb_sel_throttle_mode = 2;
    }

    CadbCfg.Bits.lane_deselect_en = 0;
    CadbCfg.Bits.initial_dsel_en = 0;
    GetSetCadbRegs (Host, Socket, Channel, SubCh, CPGC_REG_WRITE, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);

    // Setup CADB MRSCFG
    GetSetCadbRegs (Host, Socket, Channel, SubCh, CPGC_REG_READ, CADB_MRSCFG_N1_MCCADB_MAIN_REG, &CadbMrscfgN1.Data);
    GetSetCadbRegs (Host, Socket, Channel, SubCh, CPGC_REG_READ, CADB_MRSCFG_N0_MCCADB_MAIN_REG, &CadbMrscfgN0.Data);
    CadbMrscfgN1.Bits.mrs_delay_cnt = 0;
    if ((*ChannelNvList) [Channel].timingMode == TIMING_1N) {
      CadbMrscfgN0.Bits.mrs_cs_mode = ONE_CYCLE_CS;
    } else {
      CadbMrscfgN0.Bits.mrs_cs_mode = TWO_CYCLE_CS;
    }
    CadbMrscfgN0.Bits.mrs_goto_ptr = 0;
    CadbMrscfgN0.Bits.mrs_end_ptr = 0;
    CadbMrscfgN0.Bits.mrs_start_ptr = 0;
    CadbMrscfgN0.Bits.cs_active_polarity = 0;
    CadbMrscfgN0.Bits.setup_clocks = 0;
    CadbMrscfgN0.Bits.dsel_clocks = 0;
    CadbMrscfgN0.Bits.cs_clocks = 0;
    GetSetCadbRegs (Host, Socket, Channel, SubCh, CPGC_REG_WRITE, CADB_MRSCFG_N1_MCCADB_MAIN_REG, &CadbMrscfgN1.Data);
    GetSetCadbRegs (Host, Socket, Channel, SubCh, CPGC_REG_WRITE, CADB_MRSCFG_N0_MCCADB_MAIN_REG, &CadbMrscfgN0.Data);

    // Setup CADB SELCFG
    GetSetCadbRegs (Host, Socket, Channel, SubCh, CPGC_REG_READ, CADB_SELCFG_MCCADB_MAIN_REG, &CadbSelCfg.Data);
    CadbSelCfg.Bits.initial_sel_sseq_en = 1;
    CadbSelCfg.Bits.pre_select_enable = 0;
    CadbSelCfg.Bits.act_select_enable = 0;
    CadbSelCfg.Bits.rd_select_enable = 0;
    CadbSelCfg.Bits.wr_select_enable = 4;
    GetSetCadbRegs (Host, Socket, Channel, SubCh, CPGC_REG_WRITE, CADB_SELCFG_MCCADB_MAIN_REG, &CadbSelCfg.Data);

    // Setup CADB DLY
    GetSetCadbRegs (Host, Socket, Channel, SubCh, CPGC_REG_READ, CADB_DLY_MCCADB_MAIN_REG, &CadbDly.Data);
    CadbDly.Bits.start_delay = 4;    // Set delays for PBA
    CadbDly.Bits.stop_delay  = 0xC8;
    GetSetCadbRegs (Host, Socket, Channel, SubCh, CPGC_REG_WRITE, CADB_DLY_MCCADB_MAIN_REG, &CadbDly.Data);

    // Setup CADB A0 MRSCFG
    GetSetCadbRegs (Host, Socket, Channel, SubCh, CPGC_REG_READ, CADB_AO_MRSCFG_MCCADB_MAIN_REG, &CadbAoMrscfg.Data);
    CadbAoMrscfg.Bits.mrs_ao_repeats = 0;
    CadbAoMrscfg.Bits.mrs_gap = 0;
    CadbAoMrscfg.Bits.mrs_gap_scale = 0;
    GetSetCadbRegs (Host, Socket, Channel, SubCh, CPGC_REG_WRITE, CADB_AO_MRSCFG_MCCADB_MAIN_REG, &CadbAoMrscfg.Data);

  } //subch loop

  // PBA only programs one line
  CopyMem (&MrPattern0Local[0], &MpcPbaEnumerateIdCADBPattern0[0], sizeof (PatCadb2Prog0));
  CopyMem (&MrPattern1Local[0], &MpcPbaEnumerateIdCADBPattern1[0], sizeof (PatCadb2Prog1));

  // Program PBA Enumerate ID
  MrPattern1Local[0].ca = MrPattern1Local[0].ca | (Opcode & 0xF);

  FixupCS (LogicalRank, 0, MrPattern0Local, (*ChannelNvList) [Channel].encodedCSMode);
  ProgramCADB2 (Host, Socket, Channel, SubchannelMask, StartLine, EndLine, CADB_BUF_MODE_DDR5_SELECT, FALSE, MrPattern0Local, MrPattern1Local);

  //
  // Configure CPGC
  //
  ProgramCpgcForPdaPba (Host, Socket, Channel, SubchannelMask);
  
  CpgcMiscCkeCtlReg.Data = MemReadPciCfgEp (Socket, Channel, CPGC_MISCCKECTL_MCDDC_CTL_REG);
  CpgcMiscCkeCtlReg.Bits.cke_override     = 0;
  CpgcMiscCkeCtlReg.Bits.cke_on           = 0;
  MemWritePciCfgEp (Socket, Channel, CPGC_MISCCKECTL_MCDDC_CTL_REG, CpgcMiscCkeCtlReg.Data);
  

  //
  // Delay if this is a CTE build, otherwise do nothing.
  //
  CteDelayDclk (50);

  //
  // Start Test and Poll on completion
  //
  IO_Reset (Host, Socket);

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (SubCh == 0) {
      continue;
    }
    CpgcGlobalTestStart (Host, Socket, 1 << Channel, SubCh);
  }

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (SubCh == 0) {
      continue;
    }
    CpgcPollGlobalTestDone (Host, Socket, 1 << Channel, SubCh);
  }

  //
  // Poll test completion. Also need to poll the CADB done as the test is CADB binding CPGC.
  //
  CadbPollTestDone (Host, Socket, 1 << Channel, TWO_SUBCH_MASK);


  CteDelayQclk (60);
  RunIOTest10nmDelayTarget (Host, Socket, Channel);

  //
  // Stop the test
  //
  CadbCtl.Data = 0;
  CadbCtl.Bits.start_test = 0;
  CadbCtl.Bits.stop_test = 1;
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & (SubchannelMask)) == 0) {
      continue;
    }
    MemWritePciCfgEp (Socket, Channel, SUBCH_OFFSET_CADB (SubCh, CADB_CTL_MCCADB_MAIN_REG), CadbCtl.Data);
  }

  //
  // Delay if this is a CTE build, otherwise do nothing.
  //
  CteDelayDclk (10);

  // Restore activate/precharge setting
  SetMprModeFilterActPre (Host, Socket, Channel, 0);

  return;
}

/**

  This function sends mode register read or write command by CADB for DDR5.

  @param[in] Host            - Include all the MRC data
  @param[in] Socket          - Socket number
  @param[in] Ch              - Ch to send command to
  @param[in] Dimm            - Dimm to send command to
  @param[in] SubChMask       - Sub channel mask
  @param[in] Rank            - CS to send the command to
  @param[in] Address         - Mode register Address
  @param[in] Opcode          - Opcode to use
  @param[in] Cw              - CW to use
  @param[in] Cmd             - Which command: MRR_CMD or MRW_CMD

  @retval N/A

**/
VOID
WriteCadbMrCmd (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     SubChMask,
  IN UINT8     Rank,
  IN UINT16    Address,
  IN UINT16    Opcode,
  IN UINT8     Cw,
  IN UINT8     Cmd
  )
{
  UINT8                                         SubCh = 0;
  UINT8                                         LogicalRank;
  struct channelNvram                           (*ChannelNvList) [MAX_CH];
  PatCadb2Prog0                                 MrPattern0Local[CADB2_LINES];
  PatCadb2Prog1                                 MrPattern1Local[CADB2_LINES];
  CADB_MRSCFG_N0_MCCADB_MAIN_STRUCT             CadbMrscfgN0;
  CADB_CFG_MCCADB_MAIN_STRUCT                   CadbCfg;
  CADB_AO_MRSCFG_MCCADB_MAIN_STRUCT             CadbAoMrscfg;
  CADB_CTL_MCCADB_MAIN_STRUCT                   CadbCtl;
  CADB_SELCFG_MCCADB_MAIN_STRUCT                CadbSelCfg;
  CADB_DLY_MCCADB_MAIN_STRUCT                   CadbDly;
  CADB_MRSCFG_N1_MCCADB_MAIN_STRUCT             CadbMrscfgN1;
  CPGC2_RASTER_REPO_CONFIG_MCCPGC_MAIN_STRUCT   RasterRepoConfig;
  CPGC_ERR_CTL_MCCPGC_MAIN_STRUCT               ErrCtl;
  CPGC_SEQ_CFG_A_MCCPGC_MAIN_STRUCT             SeqCfgA;
  UINT32                                        Patbuf[CADB_MUX_MAX];
  UINT32                                        MuxCtl[CADB_MUX_MAX] = {BTBUFFER, BTBUFFER, BTBUFFER, BTBUFFER};
  UINT32                                        LfsrSize[CADB_MUX_MAX] = {0, 0, 0, 0};
  UINT8                                         StartLine = 0;
  UINT8                                         EndLine = 0;
  UINT16                                        NumOfCacheLine;
  UINT8                                         LoopCount;
  TCPGCAddress                                  CpgcAddress;
  TWDBPattern                                   WdbPattern;
  UINT8                                         OrigMprMode;
  BOOLEAN                                       OrigDdrt2Mode;

  ChannelNvList = GetChannelNvList (Host, Socket);

  if ((*ChannelNvList) [Ch].enabled == 0) {
    return;
  }

  LogicalRank = GetLogicalRank(Host, Socket, Ch, Dimm, Rank);

  if (IsPmemSiliconWorkaroundEnabled(Host, Socket, Ch, Dimm, "S14011274250") && (Host->nvram.mem.socket[Socket].ddrFreq > DDR_3600)) {
    BgfReset(Host, Socket, Ch, Dimm, 1);
  }

  OrigDdrt2Mode = GetDdrt2Mode (Host, Socket, Ch);
  //
  // DDRT2 Scheduler can't send MRW in 1N cmd timing
  //
  SetCpgcTrainTarget (Socket, Ch, (BOOLEAN) 0);
  SetDdrtForceMode (Socket, Ch, (BOOLEAN) 0);

  if (Cmd == MRW_CMD) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "MRW command with Address 0x%04X,  Data0x%04X,  CW 0x%04X, SubChMask %d Cmd %d by WriteCadbMrCmd\n", Address, Opcode, Cw, SubChMask, Cmd);
  } else {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "MRR command with Address 0x%04X, CW 0x%04X, SubChMask %d Cmd %d by WriteCadbMrCmd\n", Address, Cw, SubChMask, Cmd);
  }
  if (FeaturePcdGet (PcdCteBuild) == TRUE) {

    if (Host->DdrioUltSupport) {
      if (Cmd == MRR_CMD) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
          "MRR Command  Address 0x%04X\n", Address);
        CteDdrCmd("mrr", 1 << Ch, SubChMask, GetPhyRank(Dimm, Rank), Address, Opcode | (Cw << 8));
        return;
      } else {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
          "MRW Command  Address 0x%04X, data 0x%04X\n", Address, Opcode);
        CteDdrCmd("mrw", 1 << Ch, SubChMask, GetPhyRank(Dimm, Rank), Address, Opcode | (Cw << 8));
        return;
      }
    }
  }

  // If programming for PBA, do special CADB setup
  if (Cmd == MRW_CMD && (Address == DDR5_DATA_BUFFER_RW92_REG) && (Cw == CW_RCD_DB)) {
    ProgramCadbForPba (Host, Socket, Ch, Dimm, SubChMask, Rank, Opcode);
    return;
  }

  FormatAlwaysOnMrCmdPattern (Host, Socket, Ch, Dimm, (*ChannelNvList) [Ch].timingMode, Host->nvram.mem.dimmTypePresent, Address, Opcode, Cw, Cmd, MrPattern0Local, MrPattern1Local, &EndLine);

  if (EndLine == 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "Error: Format command pattern. Timing Mode: %d, Dimm Type: %d, Cmd: %d\n", (*ChannelNvList) [Ch].timingMode, Host->nvram.mem.dimmTypePresent, Cmd);
    return;
  }

  SetCadbModeEn (Host, Socket, 1 << Ch, TRUE);

  //
  // Configure CADB
  //
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & SubChMask) == 0) {
      continue;
    }
    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_MRSCFG_N1_MCCADB_MAIN_REG, &CadbMrscfgN1.Data);
    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_MRSCFG_N0_MCCADB_MAIN_REG, &CadbMrscfgN0.Data);
    CadbMrscfgN1.Bits.mrs_delay_cnt = 0;
    CadbMrscfgN0.Bits.mrs_cs_mode = ONE_CYCLE_CS;
    CadbMrscfgN0.Bits.mrs_goto_ptr = 0;
    CadbMrscfgN0.Bits.mrs_end_ptr = 0;
    CadbMrscfgN0.Bits.mrs_start_ptr = 0;
    CadbMrscfgN0.Bits.cs_active_polarity = 0;
    CadbMrscfgN0.Bits.setup_clocks = 0;
    CadbMrscfgN0.Bits.dsel_clocks = 0;
    CadbMrscfgN0.Bits.cs_clocks = 0;
    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_MRSCFG_N1_MCCADB_MAIN_REG, &CadbMrscfgN1.Data);
    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_MRSCFG_N0_MCCADB_MAIN_REG, &CadbMrscfgN0.Data);

    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);
    if (Cmd == MRW_CMD) {
      CadbCfg.Bits.cadb_mode = CADB_MODE_ALWAYS_ON; // CADB MRS mode doesn't work for MRW, need to use always on mode
      CadbCfg.Bits.cadb_to_cpgc_bind = 0;
      CadbCfg.Bits.cadb_dsel_throttle_mode = 1;
      CadbCfg.Bits.cadb_sel_throttle_mode = 0;
      CadbCfg.Bits.lane_deselect_en = DRIVE_CADB_PATTERN_ON_ALL_PINS; // Drive CADB pattern on all pins including (CS, CA and PAR)
      CadbCfg.Bits.initial_dsel_en = 1;
    } else {
      CadbCfg.Bits.cadb_mode = CADB_MODE_ACTIVE_SELECT_DESLECT; // Work with CPGC select cycle for MRR
      CadbCfg.Bits.cadb_to_cpgc_bind = 1;
      if ((*ChannelNvList) [Ch].timingMode == TIMING_1N) {
        CadbCfg.Bits.cadb_dsel_throttle_mode = 1;
        CadbCfg.Bits.cadb_sel_throttle_mode = 1;
      } else {
        CadbCfg.Bits.cadb_dsel_throttle_mode = 2;
        CadbCfg.Bits.cadb_sel_throttle_mode = 2;
      }
      CadbCfg.Bits.lane_deselect_en = 0; // Don't drive CADB during deselect cycle
      CadbCfg.Bits.initial_dsel_en = 0;
    }
    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);

    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_AO_MRSCFG_MCCADB_MAIN_REG, &CadbAoMrscfg.Data);
    CadbAoMrscfg.Bits.mrs_ao_repeats = 1;
    if ((Host->nvram.mem.dimmTypePresent == RDIMM) || (Host->nvram.mem.dimmTypePresent == LRDIMM)) {
      CadbAoMrscfg.Bits.mrs_gap = (EndLine - StartLine) /2 -1;
    } else {
      CadbAoMrscfg.Bits.mrs_gap = (EndLine - StartLine) -1;
    }
    if ((*ChannelNvList) [Ch].timingMode == TIMING_1N) {
      CadbAoMrscfg.Bits.mrs_gap = 1;
    }
    CadbAoMrscfg.Bits.mrs_gap_scale = 0;
    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_AO_MRSCFG_MCCADB_MAIN_REG, &CadbAoMrscfg.Data);

    SetCadbDselUniseqCfg (Host, Socket, Ch, SubCh, MuxCtl, LfsrSize);

    Patbuf[0] = SEQUENTIAL_BUFFER_PATTERN_0;
    Patbuf[1] = SEQUENTIAL_BUFFER_PATTERN_1;
    Patbuf[2] = SEQUENTIAL_BUFFER_PATTERN_2;
    Patbuf[3] = SEQUENTIAL_BUFFER_PATTERN_3;

    if ((Cmd == MRW_CMD) && ((*ChannelNvList) [Ch].timingMode == TIMING_1N)) {
      Patbuf[0] = CADB_BUFFER_PATTERN_10;
      Patbuf[1] = CADB_BUFFER_PATTERN_11;
      Patbuf[2] = CADB_BUFFER_PATTERN_12;
      Patbuf[3] = CADB_BUFFER_PATTERN_13;
    }

    SetCadbDselUniseqPat (Host, Socket, Ch, SubCh, Patbuf);

    if (Cmd == MRR_CMD) {
      //
      // CADB driver CA pins and CPGC drive others for MRR
      //
      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_SELCFG_MCCADB_MAIN_REG, &CadbSelCfg.Data);
      CadbSelCfg.Bits.rd_select_enable = 4;    // Drive pattern only on CA pins. Don't override CS or parity
      CadbSelCfg.Bits.initial_sel_sseq_en = 1; // Use read select at the beginning of subsequence
      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_SELCFG_MCCADB_MAIN_REG, &CadbSelCfg.Data);

      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_DLY_MCCADB_MAIN_REG, &CadbDly.Data);
      CadbDly.Bits.stop_delay = 0xC8;
      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_DLY_MCCADB_MAIN_REG, &CadbDly.Data);
    }
  } //subch loop

  //
  // Load command pattern and trigger the command
  //
  if (Cmd == MRW_CMD) {
    //
    // MRW
    //
    FixupCS (LogicalRank, 0, MrPattern0Local, (*ChannelNvList) [Ch].encodedCSMode);
    ProgramCADB2 (Host, Socket, Ch, SubChMask, StartLine, EndLine, CADB_BUF_MODE_DDR5_DESELECT, FALSE, MrPattern0Local, MrPattern1Local);
    //
    // Delay if this is a CTE build, otherwise do nothing
    //
    CteDelayDclk (50);

    //
    // Start test. SPR_TODO. HSD 1507158123: Update the ExecuteCmdSignalTest to support SubChMask parameter, then replace it by ExecuteCmdSignalTest.
    //
    CadbTestStart(Host, Socket, (1 << Ch), SubChMask);

    //
    // Poll test completion
    //
    CadbPollTestDone(Host, Socket, 1 << Ch, SubChMask);
    //
    // Stop the test
    //
    CadbCtl.Data = 0;
    CadbCtl.Bits.start_test = 0;
    CadbCtl.Bits.stop_test = 1;
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & (SubChMask)) == 0) {
        continue;
      }
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CADB (SubCh, CADB_CTL_MCCADB_MAIN_REG), CadbCtl.Data);
    }

  } else {
    //
    // MRR
    //
    FixupCS (LogicalRank, 0, MrPattern0Local, (*ChannelNvList) [Ch].encodedCSMode);
    ProgramCADB2 (Host, Socket, Ch, SubChMask, StartLine, EndLine, CADB_BUF_MODE_DDR5_SELECT, FALSE, MrPattern0Local, MrPattern1Local);

    SelectCPGCRanks(Host, Socket, Ch, 1 << LogicalRank, 0, 0);
    //
    // 1 loop count and 1 cache line read to send MRR
    //
    NumOfCacheLine = 1;
    LoopCount = 1;
    //
    // One location at address 0 for MRR
    //
    SetMem ((UINT8 *) &CpgcAddress, sizeof (CpgcAddress), 0);
    SetMem ((UINT8 *) &WdbPattern, sizeof (WdbPattern), 0);
    //
    // Setup CPGC
    //
    SetupIOTest (Host, Socket, 1 << Ch, PatRdMrr, NumOfCacheLine, LoopCount, &CpgcAddress, NSOE, &WdbPattern, 0, 0, 0);

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      if (((1 << SubCh) & (SubChMask)) == 0) {
        continue;
      }
      //
      // Save original Mpr mode and turn it on for MRR
      //
      OrigMprMode = GetMprModeFilterActPre (Socket, Ch, SubCh);
      SetMprModeFilterActPre (Host, Socket, Ch, FILTER_ACT_PRE_ENABLE);

      //
      // Assign channels on cpgc reqgen1 for testing
      //
      SeqCfgA.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_B, CPGC_SEQ_CFG_A_MCCPGC_MAIN_REG));
      SeqCfgA.Bits.channel_assign = SubChMask;
      SeqCfgA.Bits.initialization_mode = CPGC_Active_Mode;
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_B, CPGC_SEQ_CFG_A_MCCPGC_MAIN_REG), SeqCfgA.Data);

      RasterRepoConfig.Data = 0;
      RasterRepoConfig.Bits.rasterrepoclear = 1; // Reset the raster repo entries.
      RasterRepoConfig.Bits.stoponraster = 1; // The test will stop after status condition is met in the raster repo
      RasterRepoConfig.Bits.rasterrepomode = CPGC_RASTER_REPO_BITMAP_MODE; // All dq bit and chunk error can be retrived in repo content register
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_RASTER_REPO_CONFIG_MCCPGC_MAIN_REG), RasterRepoConfig.Data);

      //
      // Provide the Initial value (all 0s) for the Alternate Data pattern generator.
      //
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_DPAT_INV_DC_MASK_LO_MCCPGC_MAIN_REG), 0);
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_DPAT_INV_DC_MASK_HI_MCCPGC_MAIN_REG), 0);
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_DPAT_INVDC_CFG_MCCPGC_MAIN_REG), 0);

      ErrCtl.Data = 0;
      ErrCtl.Bits.errchk_mask_cacheline = CPGC_ERROR_CHECK_MASK_CACHELINE;
      ErrCtl.Bits.be_train_err_en = 1; // Use default
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_CTL_MCCPGC_MAIN_REG), ErrCtl.Data);

      //
      // Enable error check on all 16 chunks in a burst
      //
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_LNEN_HI_MCCPGC_MAIN_REG), CPGC_ERROR_CHECK_MASK_CHUNK);
    }

    //
    // Trigger the test on CPGC reqgen1 (which is on sub-channel B)
    //
    CpgcGlobalTestStart (Host, Socket, 1 << Ch, SUB_CH_B);
    CpgcPollGlobalTestDone (Host, Socket, 1 << Ch, SUB_CH_B);

    //
    // Alsp poll cadb test done on the given sub-channel
    //
    CadbPollTestDone (Host, Socket, 1 << Ch, SubChMask);

    //
    // Restore Mpr mode
    //
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      if (((1 << SubCh) & (SubChMask)) == 0) {
        continue;
      }
      SetMprModeFilterActPre (Host, Socket, Ch, OrigMprMode);
    }
  } // MRR

  //
  // In EarlyCaBacksideLoopbackTest CTE non-I3C test, need to use the inband MRW, need to keep the CADB mode enable during whole backside
  // QCA training, no need to do the enable it, disbale it, enable it sequence.
  //
  if (GetCurrentTestType (Socket) != EarlyCaBacksideLoopbackTest) {
    //
    // Disable CADB Mode
    //
    SetCadbModeEn (Host, Socket, 1 << Ch, FALSE);
    //
    // Add the delay to wait for the bit change to take effective.
    //
    TrainingDelay (Host, 5 * 16, 1);
  }

  //
  // Restore Mode
  //

  SetCpgcTrainTarget (Socket, Ch, OrigDdrt2Mode);
  SetDdrtForceMode (Socket, Ch, OrigDdrt2Mode);

  if (IsPmemSiliconWorkaroundEnabled(Host, Socket, Ch, Dimm, "S14011274250") && (Host->nvram.mem.socket[Socket].ddrFreq > DDR_3600)) {
    BgfReset(Host, Socket, Ch, Dimm, 0);
  }
  //
  // Delay if this is a CTE build, otherwise do nothing.
  //
  CteDelayDclk (10);

  return;
} // WriteCadbMrCmd

/**

  Read CPGC repo contents for particular mode register which is specified by Address parameter.
  For two sub-channels, the first half of data array for sub-channel A and the second half for
  sub-channel B.

  X8 dimm   SubChMSVx8 = 5
  LogicalStrobe     CPGC lanes
  0                 [7:0]
  1                 [15:8]
  2                 [23:16]
  3                 [31:24]
  4                 [39:32] ECC

  X4 dimm   SubChMSVx4 = 10
  LogicalStrobe     CPGC lanes
  0                 [3:0]
  5                 [7:4]
  1                 [11:8]
  6                 [15:12]
  2                 [19:16]
  7                 [23:20]
  3                 [27:24]
  8                 [31:28]
  4                 [35:32] ECC
  9                 [39:36] ECC

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

  PSYSHOST  Host;
  UINT32    RegDataDq[CPGC_MAX_NUMBER_REPO_CONTENT];
  UINT32    RegDataEcc[CPGC_MAX_NUMBER_REPO_CONTENT_ECC];
  UINT16    ChunkDataDq[DDR5_SUBCH_MAX_BITS];
  UINT16    ChunkDataEcc[DDR5_SUBCH_MAX_ECC_BITS];
  UINT8     IndexReg;
  UINT8     IndexDq;
  UINT8     IndexStrobe;
  UINT8     OffsetChunk;
  UINT8     OffsetDq;
  UINT8     SubCh;
  UINT8     Byte0;
  UINT8     Byte1;
  UINT8     Byte2;
  UINT8     Byte3;
  UINT8     SubChMSV;

  UINT32    RepoRegsDq[CPGC_MAX_NUMBER_REPO_CONTENT] = {
                         CPGC2_RASTER_REPO_CONTENT_0_N0_MCCPGC_MAIN_REG, CPGC2_RASTER_REPO_CONTENT_0_N1_MCCPGC_MAIN_REG,
                         CPGC2_RASTER_REPO_CONTENT_1_N0_MCCPGC_MAIN_REG, CPGC2_RASTER_REPO_CONTENT_1_N1_MCCPGC_MAIN_REG,
                         CPGC2_RASTER_REPO_CONTENT_2_N0_MCCPGC_MAIN_REG, CPGC2_RASTER_REPO_CONTENT_2_N1_MCCPGC_MAIN_REG,
                         CPGC2_RASTER_REPO_CONTENT_3_N0_MCCPGC_MAIN_REG, CPGC2_RASTER_REPO_CONTENT_3_N1_MCCPGC_MAIN_REG,
                         CPGC2_RASTER_REPO_CONTENT_4_N0_MCCPGC_MAIN_REG, CPGC2_RASTER_REPO_CONTENT_4_N1_MCCPGC_MAIN_REG,
                         CPGC2_RASTER_REPO_CONTENT_5_N0_MCCPGC_MAIN_REG, CPGC2_RASTER_REPO_CONTENT_5_N1_MCCPGC_MAIN_REG,
                         CPGC2_RASTER_REPO_CONTENT_6_N0_MCCPGC_MAIN_REG, CPGC2_RASTER_REPO_CONTENT_6_N1_MCCPGC_MAIN_REG,
                         CPGC2_RASTER_REPO_CONTENT_7_N0_MCCPGC_MAIN_REG, CPGC2_RASTER_REPO_CONTENT_7_N1_MCCPGC_MAIN_REG
                         };

  UINT32     RepoRegsEcc[CPGC_MAX_NUMBER_REPO_CONTENT_ECC] = {
                         CPGC2_RASTER_REPO_CONTENT_ECC_0_N0_MCCPGC_MAIN_REG, CPGC2_RASTER_REPO_CONTENT_ECC_0_N1_MCCPGC_MAIN_REG,
                         CPGC2_RASTER_REPO_CONTENT_ECC_1_N0_MCCPGC_MAIN_REG, CPGC2_RASTER_REPO_CONTENT_ECC_1_N1_MCCPGC_MAIN_REG
                         };

  //
  // Use the first DQ's content within a strobe for MRR returned data
  //
  UINT8      StrobeToCpgcLaneMapping[MAX_STROBE_DDR5 / 2] = {0, 8, 16, 24, 32, 4, 12, 20, 28, 36};

  if ((Data == NULL)) {
    return MRC_STATUS_FAILURE;
  }

  Host = GetSysHostPointer();

  if (IsX4Dimm (Socket, Ch, Dimm)) {
    SubChMSV = GetSubChMaxStrobeValid (Host);
  } else {
    SubChMSV = GetSubChMaxStrobeValid (Host) / 2;
  }

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & SubChMask) == 0) {
      continue;
    }

    RcDebugPrint (SDBG_CPGC, "\nMRR for SubCh = %d",SubCh);

    //
    // 1.a - Read mode register content from DQ lanes
    //
    for (IndexReg = 0; IndexReg < CPGC_MAX_NUMBER_REPO_CONTENT; IndexReg++) {
      RegDataDq[IndexReg] = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, RepoRegsDq[IndexReg]));
    }

    //
    // 1.b - Convert to register data to chunk data for each ECC lanes
    //
    for (IndexDq = 0; IndexDq < DDR5_SUBCH_MAX_DQ_BITS; IndexDq++) {

      ChunkDataDq[IndexDq] = 0;

      for (IndexReg = 0; IndexReg < CPGC_MAX_NUMBER_REPO_CONTENT; IndexReg++) {
        ChunkDataDq[IndexDq] |= ((RegDataDq[IndexReg] >> IndexDq) & BIT0) << IndexReg;
      }
      if (IndexDq % BYTE_WIDTH == 0) {
        RcDebugPrint (SDBG_CPGC,"\n");
      }

      //
      // Per Jedec Spec,
      // if chunk[7:0] is 0, then MRR data in chunk[15-8] is not inverted.
      // If chunk[7:0] is 0xFF, then MRR data in chunk[15-8] is inverted.
      //
      if ((ChunkDataDq[IndexDq] & BYTE_MASK) == MRR_INVERSION_FLAG) {
        RcDebugPrint (SDBG_CPGC, "DQ%02d:0x%02x ", IndexDq, (~ChunkDataDq[IndexDq] >> BYTE_WIDTH) & BYTE_MASK);
      } else if ((ChunkDataDq[IndexDq] & BYTE_MASK) == MRR_NON_INVERSION_FLAG) {
        RcDebugPrint (SDBG_CPGC, "DQ%02d:0x%02x ", IndexDq, (ChunkDataDq[IndexDq] >> BYTE_WIDTH) & BYTE_MASK);
      } else {
        RcDebugPrint (SDBG_ERROR, "Error: The format of MRR returned data is incorrect, DQ[%d]=0x%x.\n", IndexDq, ChunkDataDq[IndexDq]);
        return MRC_STATUS_FAILURE;
      }
    }

    RcDebugPrint (SDBG_CPGC,"\n");

    //
    // 2.a - Read mode register content from ECC lanes
    // ECC_0_N0/N1 for even chunks
    // ECC_1_N0/N1 for odd chunks
    //
    for (IndexReg = 0; IndexReg < CPGC_MAX_NUMBER_REPO_CONTENT_ECC; IndexReg++) {
      RegDataEcc[IndexReg] = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, RepoRegsEcc[IndexReg]));
    }

    //
    // 2.b - Convert to register data to chunk data for each ECC lanes
    // ECC_0_N0/N1 for even chunks, ECC_1_N0/N1 for odd chunks
    //
    // ECC_0_N0[31:24] = Chunk6    ECC_0_N1[31:24] = Chunk14
    // ECC_0_N0[23:16] = Chunk4    ECC_0_N1[23:16] = Chunk12
    // ECC_0_N0[15:8]  = Chunk2    ECC_0_N1[15:8]  = Chunk10
    // ECC_0_N0[7:0]   = Chunk0    ECC_0_N1[7:0]   = Chunk8
    // ECC_1_N0[31:24] = Chunk7    ECC_1_N1[31:24] = Chunk15
    // ECC_1_N0[23:16] = Chunk5    ECC_1_N1[23:16] = Chunk13
    // ECC_1_N0[15:8]  = Chunk3    ECC_1_N1[15:8]  = Chunk11
    // ECC_1_N0[7:0]   = Chunk1    ECC_1_N1[7:0]   = Chunk9
    //
    for (IndexDq = 0; IndexDq < DDR5_SUBCH_MAX_ECC_BITS; IndexDq++) {

      ChunkDataEcc[IndexDq] = 0;

      for (IndexReg = 0; IndexReg < CPGC_MAX_NUMBER_REPO_CONTENT_ECC; IndexReg++) {
        Byte0 = RegDataEcc[IndexReg] & BYTE_MASK;
        Byte1 = (RegDataEcc[IndexReg] >> BYTE_WIDTH) & BYTE_MASK;
        Byte2 = (RegDataEcc[IndexReg] >> WORD_WIDTH) & BYTE_MASK;
        Byte3 = (RegDataEcc[IndexReg] >> (WORD_WIDTH + BYTE_WIDTH)) & BYTE_MASK;
        if (IndexReg < CPGC_REG_OFFSET_REPO_CONTENT_ECC_ODD) {
          //
          // ECC_0_N0/N1 for even chunks
          //
          OffsetChunk = IndexReg * BYTE_WIDTH;
        } else {
          //
          // ECC_1_N0/N1 for odd chunks
          //
          OffsetChunk = (IndexReg - CPGC_REG_OFFSET_REPO_CONTENT_ECC_ODD) * BYTE_WIDTH + CPGC_ODD_CHUNK_INC;
        }
        ChunkDataEcc[IndexDq] |= ((Byte0 >> IndexDq) & BIT0) << (OffsetChunk + CPGC_EVEN_CHUNK_INC * INDEX_BYTE_0);
        ChunkDataEcc[IndexDq] |= ((Byte1 >> IndexDq) & BIT0) << (OffsetChunk + CPGC_EVEN_CHUNK_INC * INDEX_BYTE_1);
        ChunkDataEcc[IndexDq] |= ((Byte2 >> IndexDq) & BIT0) << (OffsetChunk + CPGC_EVEN_CHUNK_INC * INDEX_BYTE_2);
        ChunkDataEcc[IndexDq] |= ((Byte3 >> IndexDq) & BIT0) << (OffsetChunk + CPGC_EVEN_CHUNK_INC * INDEX_BYTE_3);
      }

      //
      // Per Jedec Spec,
      // if chunk[7:0] is 0, then MRR data in chunk[15-8] is not inverted.
      // If chunk[7:0] is 0xFF, then MRR data in chunk[15-8] is inverted.
      //
      if ((ChunkDataEcc[IndexDq] & BYTE_MASK) == MRR_INVERSION_FLAG) {
        RcDebugPrint (SDBG_CPGC, "DQ%02d:0x%02x ", IndexDq + DDR5_SUBCH_MAX_DQ_BITS, (~ChunkDataEcc[IndexDq] >> BYTE_WIDTH) & BYTE_MASK);
      } else if ((ChunkDataEcc[IndexDq] & BYTE_MASK) == MRR_NON_INVERSION_FLAG) {
        RcDebugPrint (SDBG_CPGC, "DQ%02d:0x%02x ", IndexDq + DDR5_SUBCH_MAX_DQ_BITS, (ChunkDataEcc[IndexDq] >> BYTE_WIDTH) & BYTE_MASK);
      } else {
        RcDebugPrint (SDBG_ERROR,"Error: The format of MRR returned data is incorrect, DQ[%d]=0x%x.\n", IndexDq + DDR5_SUBCH_MAX_DQ_BITS, ChunkDataEcc[IndexDq]);
        return MRC_STATUS_FAILURE;
      }

      ChunkDataDq[IndexDq + DDR5_SUBCH_MAX_DQ_BITS] = ChunkDataEcc[IndexDq];
    }

    RcDebugPrint (SDBG_CPGC,"\n");

    //
    // 3 - Pick up corresponding DQ's returned data for MRR
    //
    for (IndexStrobe = 0; IndexStrobe < SubChMSV; IndexStrobe++) {
      OffsetDq = StrobeToCpgcLaneMapping[IndexStrobe];
      if ((ChunkDataDq[OffsetDq] & BYTE_MASK) == MRR_INVERSION_FLAG) {
        (*Data)[SubCh][IndexStrobe] = (~ChunkDataDq[OffsetDq] >> BYTE_WIDTH) & BYTE_MASK;
      } else {
        (*Data)[SubCh][IndexStrobe] = (ChunkDataDq[OffsetDq] >> BYTE_WIDTH) & BYTE_MASK;
      }
    }
  }

  return MRC_STATUS_SUCCESS;
}

/**

  This function writes Vref CA Command by CADB in PDA mode

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
  // Set dqovren bit as 1 to enable DQ override and set the dqvalueovr to override the DQ value.
  //
  //
  // DDR5_TODO. to check whether needs to set RTT_CA.
  //
  //
  // Enter PDA mode.
  //
  //WriteCadbMpcCmd (Host, socket, ch, dimm, subchannelmask, rank, ENTER_PDA_ENUMERATE_PROGRAM_MODE, timingmode);
  //
  // Set the VrefCA.
  //
  WriteCadbVrefCaCmd (Host, Socket, Ch, Dimm, Subchannelmask, Rank, Opcode, Timingmode, PassThroughMode);
  //
  // Exit PDA mode.
  //
  //WriteCadbMpcCmd (Host, socket, ch, dimm, subchannelmask, rank, EXIT_PDA_ENUMERATE_PROGRAM_MODE, timingmode);
  return;

} // WriteCadbVrefCaPdaCmd

/**

  Early CS CLK training pattern sets up. DDR5 only.
  It is used to setup CS training patterns for all channels defined by input parameter "ChannelMask"

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket to train
  @param[in] ChannelMask      - Ch Bit mask for which test should be setup for
  @param[in] Signal           - Signal

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcCsClkPattern (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChannelMask,
  IN GSM_CSN   Signal
  )
{
  UINT32                             i;
  UINT8                              SubCh;
  UINT32                             CadbMuxPattern[CADB_MUX_MAX];
  UINT32                             CadbMuxCtl[CADB_MUX_MAX];
  UINT32                             LfsrSize[CADB_MUX_MAX] = {CPGC_DONT_CARE, CPGC_DONT_CARE, CPGC_DONT_CARE, CPGC_DONT_CARE};
  PatCadb2Prog0                      CsClkPattern0Buf[CADB2_LINES];
  PatCadb2Prog1                      CsClkPattern1Buf[CADB2_LINES];
  CADB_CFG_MCCADB_MAIN_STRUCT        CadbCfg;
  CADB_AO_MRSCFG_MCCADB_MAIN_STRUCT  CadbAoMrsCfg;
  CADB_DLY_MCCADB_MAIN_STRUCT        CadbDly;
  UINT8                              SubChannelMask = 0;
  UINT8                              Ch;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "CpgcCsClkPattern entry\n");

  SubChannelSelect (EarlyCsClkTraining, Signal, &SubChannelMask);

  //
  // Program CADB entries
  //
  if ((Host->nvram.mem.dimmTypePresent == RDIMM) || (Host->nvram.mem.dimmTypePresent == LRDIMM)) {
    // RCD DIMM type needs doubld the pump
    for (i = 0; i < (CADB2_LINES / 2); i++) {
      //
      // 0:0,
      // 1:0,
      // 2:1,
      // 3:1,
      // 4:2,
      // 5:2,
      // 6:3
      // 7:3
      // 8:4
      // 9:4
      //10:5
      //11:5
      //12:6
      //13:6
      //14:7
      //15:7
      //
      CopyMem (&CsClkPattern0Buf[i * 2],     &CstmPattern0[i], sizeof (PatCadb2Prog0));
      CopyMem (&CsClkPattern0Buf[i * 2 + 1], &CstmPattern0[i], sizeof (PatCadb2Prog0));
      CopyMem (&CsClkPattern1Buf[i * 2],     &BasicPattern1[i], sizeof (PatCadb2Prog1));
      CopyMem (&CsClkPattern1Buf[i * 2 + 1], &BasicPattern1[i], sizeof (PatCadb2Prog1));
    }
  } else {
    for (i = 0; i < CADB2_LINES; i++) {
      CopyMem (&CsClkPattern0Buf[i], &CstmPattern0[i], sizeof (PatCadb2Prog0));
      CopyMem (&CsClkPattern1Buf[i], &BasicPattern1[i], sizeof (PatCadb2Prog1));
    }
  }

  for (i = 0; i < CADB2_LINES; i++) {
    if (CsClkPattern0Buf[i].cs != 0xFF) {
      if (Signal < CS0_B_N) {
        CsClkPattern0Buf[i].cs = (~(1 << (Signal - CS0_A_N))) & 0xFF;
      } else {
        CsClkPattern0Buf[i].cs = (~(1 << (Signal - CS0_B_N))) & 0xFF;
      }
    }
  }

  CadbMuxPattern[0] = CADB_MUX_PATTERN0_CSCLK;
  CadbMuxPattern[1] = CADB_MUX_PATTERN1_CSCLK;
  CadbMuxPattern[2] = CADB_MUX_PATTERN2_CSCLK;
  CadbMuxPattern[3] = CADB_MUX_PATTERN3_CSCLK;

  CadbMuxCtl[0] = CADB_MUX_CTRL_BUFFER;
  CadbMuxCtl[1] = CADB_MUX_CTRL_BUFFER;
  CadbMuxCtl[2] = CADB_MUX_CTRL_BUFFER;
  CadbMuxCtl[3] = CADB_MUX_CTRL_BUFFER;

  for (Ch = 0; Ch < GetMaxChDdr (); Ch++) {

    if ((ChannelMask & (1 << Ch)) == 0) {
      continue;
    }
    ProgramCADB2 (Host, Socket, Ch, SubChannelMask, 0, CADB2_LINES, CADB_BUF_MODE_DDR5_DESELECT, FALSE, CsClkPattern0Buf, CsClkPattern1Buf);

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      if (!((1 << SubCh) & SubChannelMask)) {
        continue;
      }
      //
      // Enable CADB
      //

      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_DLY_MCCADB_MAIN_REG, &CadbDly.Data);
      CadbDly.Bits.start_delay = 4; // Delay at least 4 clocks
      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_DLY_MCCADB_MAIN_REG, &CadbDly.Data);

      SetCadbDselUniseqPat (Host, Socket, Ch, SubCh, CadbMuxPattern);
      SetCadbDselUniseqCfg (Host, Socket, Ch, SubCh, CadbMuxCtl, LfsrSize);

      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);
      CadbCfg.Bits.cadb_mode = CADB_MODE_ALWAYS_ON;
      CadbCfg.Bits.cadb_dsel_throttle_mode = 1;
      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);

      CadbAoMrsCfg.Data = 0;
      CadbAoMrsCfg.Bits.mrs_ao_repeats = 0; //Infinite Pattern
      CadbAoMrsCfg.Bits.mrs_gap_scale = 0;  //Linear
      CadbAoMrsCfg.Bits.mrs_gap = 1;
      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_AO_MRSCFG_MCCADB_MAIN_REG, &CadbAoMrsCfg.Data);
    }
  }
  return SUCCESS;
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
  UINT8                                                   Ch;
  struct    dimmNvram                                     (*DimmNvList) [MAX_DIMM];
  DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_STRUCT DbInterfaceTrainingModes;

  for (Ch = 0; Ch < GetMaxChDdr (); Ch++) {
    if ((ChannelMask & (1 << Ch)) == 0) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    //
    // Enable QCS Training Mode.
    //
    DbInterfaceTrainingModes.Data = (*DimmNvList)[Dimm].rcCache[DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG];
    DbInterfaceTrainingModes.Bits.qcs_training_mode_enable      = 1;
    if (Rank == 0) {
      DbInterfaceTrainingModes.Bits.qcs_training_rank_selection = 0;
    } else {
      DbInterfaceTrainingModes.Bits.qcs_training_rank_selection = 1;
    }
    RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG, DbInterfaceTrainingModes.Data);
    (*DimmNvList)[Dimm].rcCache[DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG] = DbInterfaceTrainingModes.Data;
  }
  return SUCCESS;
}

/**

  Check whether the QCA signal is located within QCA0~QCA6 or not

  @param[in] Sig     - QCA signals

  @retval TRUE       - The QCA signal is located within QCA0~QCA6
          FALSE      - The QCA signal is not located within QCA0~QCA6

**/
BOOLEAN
IsLowHalfQca (
  IN GSM_CSN   Sig
  )
{
  if (((Sig >= Q0ACA0) && (Sig <= Q0ACA6)) ||
      ((Sig >= Q0BCA0) && (Sig <= Q0BCA6)) ||
      ((Sig >= Q1ACA0) && (Sig <= Q1ACA6)) ||
      ((Sig >= Q1BCA0) && (Sig <= Q1BCA6))) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**

  Check whether the QCA signal is QCA6 or not.

  @param[in] Sig     - QCA signals

  @retval TRUE       - The QCA signal is QCA6
          FALSE      - The QCA signal is not QCA6

**/
BOOLEAN
IsQca6 (
  IN GSM_CSN   Sig
  )
{
  if ((Sig == Q0ACA6) || (Sig == Q0BCA6) || (Sig == Q1ACA6) || (Sig == Q1BCA6)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**

  Check whether the QCA signal is one of QCA10 to QCA12 or not.

  @param[in] Sig     - QCA signals

  @retval TRUE       - The QCA signal is one of QCA10 to QCA12
          FALSE      - The QCA signal is not one of QCA10 to QCA12

**/
BOOLEAN
IsQca10ToQca12 (
  IN GSM_CSN   Sig
  )
{
  if (SIG_COMPARE (Sig, Q0ACA10, Q0ACA12) ||
      SIG_COMPARE (Sig, Q0BCA10, Q0BCA12) ||
      SIG_COMPARE (Sig, Q1ACA10, Q1ACA12) ||
      SIG_COMPARE (Sig, Q1BCA10, Q1BCA12)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**

  Early CA CLK training pattern sets up.
  It is used to setup CA training patterns for all channels defined by input parameter "ChannelMask"

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket to train
  @param[in] ChannelMask      - Ch Bit mask for which test should be setup for
  @param[in] Rank             - Rank number
  @param[in] Subrank          - Subrank number
  @param[in] Sig              - Signal
  @param[in] Mode             - Programming mode
  @param[in] Patternloop      - Pattern loop

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcCaClkPattern (
  IN PSYSHOST                  Host,
  IN UINT8                     Socket,
  IN UINT32                    ChannelMask,
  IN UINT8                     Rank,
  IN UINT8                     Subrank,
  IN GSM_CSN                   Sig,
  IN CPGC_ADV_CMD_PARITY_MODE  Mode,
  IN UINT8                     Patternloop
  )
{
  UINT32                                        i;
  MRC_MST                                       MemSsType;
  UINT32                                        Status = SUCCESS;
  UINT8                                         Ch;
  UINT8                                         SubCh;
  struct channelNvram                           (*ChannelNvList) [MAX_CH];
  UINT32                                        CadbMuxPattern[CADB_MUX_MAX];
  UINT32                                        CadbMuxCtl[CADB_MUX_MAX];
  UINT32                                        LfsrSize[CADB_MUX_MAX] = {CPGC_DONT_CARE,
                                                                          CPGC_DONT_CARE,
                                                                          CPGC_DONT_CARE,
                                                                          CPGC_DONT_CARE};
  PatCadb2Prog0                                 AddressPattern0Local[CADB2_LINES];
  PatCadb2Prog1                                 AddressPattern1Local[CADB2_LINES];
  CADB_CFG_MCCADB_MAIN_STRUCT                   CadbCfg;
  CADB_AO_MRSCFG_MCCADB_MAIN_STRUCT             CadbAoMrsCfg;
  CADB_DLY_MCCADB_MAIN_STRUCT                   CadbDly;
  UINT8                                         SubChannelMask = 0;
  UINT8                                         Dimm;


  ChannelNvList = GetChannelNvList (Host, Socket);
  MemSsType     = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  Dimm          = Rank/MAX_RANK_DIMM;  //Get the dimm number the rank is on

  for (Ch = 0; Ch < GetMaxChDdr (); Ch++) {
    if ((ChannelMask & (1 << Ch)) == 0) {
      continue;
    }
    RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "CpgcCaClkPattern %d\n", Sig);
    SetCadbModeEn (Host, Socket, 1 << Ch, TRUE);

    SubChannelSelect (CaClkTraining, Sig, &SubChannelMask);

    if (Host->var.mem.socket[Socket].TrainingStatus.Bits.TrainingPatternMode == CA_TRAINING_PATTERN_XTALK) {
      //
      // Program CADB pattern buffer
      //
      CadbMuxPattern[0] = CADB_MUX_PATTERN0_CA_XTALK;
      CadbMuxPattern[1] = CADB_MUX_PATTERN1_CA_XTALK;
      CadbMuxPattern[2] = CADB_MUX_PATTERN2_CA_XTALK;
      CadbMuxPattern[3] = 0;

      CadbMuxCtl[0] = CADB_MUX_CTRL_LFSR;
      CadbMuxCtl[1] = CADB_MUX_CTRL_LFSR;
      CadbMuxCtl[2] = CADB_MUX_CTRL_BUFFER;
      CadbMuxCtl[3] = CADB_MUX_CTRL_BUFFER;
      LfsrSize[0]   = CADB_LFSR_BIT_32;
      LfsrSize[1]   = CADB_LFSR_BIT_32;

      if ((Host->nvram.mem.dimmTypePresent == RDIMM) || (Host->nvram.mem.dimmTypePresent == LRDIMM)) {
        // Rcd-Dimm 1N & 2N
        for (i = 0; i < CADB2_LINES; i++) {
          CopyMem (&AddressPattern0Local[i], &CatmPatternRcdXtalk0[i], sizeof (PatCadb2Prog0));
          CopyMem (&AddressPattern1Local[i], &BasicPattern1[i], sizeof (PatCadb2Prog1));
        }
        CadbMuxPattern[2] = CADB_MUX_PATTERN2_CA_XTALK_RCD;
      } else { // Non-Rcd-Dimm 1N & 2N
        if (Ca13Enabled (Host, Socket, Ch, Dimm, Rank)) { // Refer to Jedec Spec table "CA training mode output"
          for (i = 0; i < CADB2_LINES; i++) {
            CopyMem (&AddressPattern0Local[i], &CatmPatternNonRcdCa13Xtalk0[i], sizeof (PatCadb2Prog0));
            CopyMem (&AddressPattern1Local[i], &BasicPattern1[i], sizeof (PatCadb2Prog1));
          }
        } else { // Ca13 is not supported
          if (((Sig < A7A) && (Sig >= A0A)) || ((Sig < A7B) && (Sig >= A0B))) { // CA0-CA6
            for (i = 0; i < CADB2_LINES; i++) {
              CopyMem (&AddressPattern0Local[i], &CatmPatternNonRcdLowBitsXtalk0[i], sizeof (PatCadb2Prog0));
              CopyMem (&AddressPattern1Local[i], &BasicPattern1[i], sizeof (PatCadb2Prog1));
            }
          } else if (((Sig < A13A) && (Sig >= A7A)) || ((Sig < A13B) && (Sig >= A7B))){ // CA7-CA12
            for (i = 0; i < CADB2_LINES; i++) {
              CopyMem (&AddressPattern0Local[i], &CatmPatternNonRcdHighBitsXtalk0[i], sizeof (PatCadb2Prog0));
              CopyMem (&AddressPattern1Local[i], &BasicPattern1[i], sizeof (PatCadb2Prog1));
            }
          } else {
            RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
              "Warning: Unexpected CA signal to be trained: %d \n", Sig);
            continue;
          }
        }
      }
    } else {
      //
      // Program CADB pattern buffer
      //
      CadbMuxPattern[0] = CADB_MUX_PATTERN0_CA;
      CadbMuxPattern[1] = CADB_MUX_PATTERN1_CA;
      CadbMuxPattern[2] = CADB_MUX_PATTERN2_CA;
      CadbMuxPattern[3] = 0;

      CadbMuxCtl[0] = CADB_MUX_CTRL_BUFFER;
      CadbMuxCtl[1] = CADB_MUX_CTRL_BUFFER;
      CadbMuxCtl[2] = CADB_MUX_CTRL_BUFFER;
      CadbMuxCtl[3] = CADB_MUX_CTRL_BUFFER;

      if ((Host->nvram.mem.dimmTypePresent == RDIMM) || (Host->nvram.mem.dimmTypePresent == LRDIMM)) {
        if ((*ChannelNvList) [Ch].timingMode == TIMING_1N) { // Rcd-Dimm 1N
          if ((Sig == PARA) || (Sig == PARB)) {
            for (i = 0; i < CADB2_LINES; i++) {
              if (Patternloop == 0) {
                CopyMem (&AddressPattern0Local[i], &CatmPatternRcd1NSimple0Par[i], sizeof (PatCadb2Prog0));
                CopyMem (&AddressPattern1Local[i], &CatmPattern1Par[i], sizeof (PatCadb2Prog1));
              } else {
                CopyMem (&AddressPattern0Local[i], &CatmPatternRcd1NSimple0Par[i], sizeof (PatCadb2Prog0));
                CopyMem (&AddressPattern1Local[i], &CatmPattern1Par_invert[i], sizeof (PatCadb2Prog1));
              }
           }
          } else {
            for (i = 0; i < CADB2_LINES; i++) {
              if (Patternloop == 0) {
                CopyMem (&AddressPattern0Local[i], &CatmPatternRcd1NSimple0[i], sizeof (PatCadb2Prog0));
                CopyMem (&AddressPattern1Local[i], &BasicPattern1[i], sizeof (PatCadb2Prog1));
              } else {
                CopyMem (&AddressPattern0Local[i], &CatmPatternRcd1NSimple0_invert[i], sizeof (PatCadb2Prog0));
                CopyMem (&AddressPattern1Local[i], &BasicPattern1[i], sizeof (PatCadb2Prog1));
              }
            }
          }
        } else { // Rcd-Dimm 2N
        if ((Sig == PARA) || (Sig == PARB)) {
          for (i = 0; i < CADB2_LINES; i++) {
            CopyMem (&AddressPattern0Local[i], &CatmPatternRcd2NSimple0Par[i], sizeof (PatCadb2Prog0));
            CopyMem (&AddressPattern1Local[i], &CatmPattern1Par2N[i], sizeof (PatCadb2Prog1));
          }
        } else {
          for (i = 0; i < CADB2_LINES; i++) {
            CopyMem (&AddressPattern0Local[i], &CatmPatternRcd2NSimple0[i], sizeof (PatCadb2Prog0));
            CopyMem (&AddressPattern1Local[i], &BasicPattern1[i], sizeof (PatCadb2Prog1));
          }
        }
        }
      } else { // Non-Rcd-Dimm 1N & 2N
        for (i = 0; i < CADB2_LINES; i++) {
          CopyMem (&AddressPattern0Local[i], &CatmPatternNonRcdSimple0[i], sizeof (PatCadb2Prog0));
          CopyMem (&AddressPattern1Local[i], &BasicPattern1[i], sizeof (PatCadb2Prog1));
          if (Ca13Enabled (Host, Socket, Ch, Dimm, Rank)) {
            AddressPattern0Local[i].ca = AddressPattern0Local[i].ca | 0x2000; // Set CA13 as simple pattern
          } else {
            AddressPattern0Local[i].ca = AddressPattern0Local[i].ca & 0x1FFF; // Clear CA13 per Jedec Spec
          }
        }
      }
    }

    //
    // CADB entries for signals.
    //
    if ((Sig >= A0A) && (Sig <= PARB)) {
      FixupCS (Rank, Subrank, AddressPattern0Local, (*ChannelNvList) [Ch].encodedCSMode);
      if (Host->var.mem.socket[Socket].TrainingStatus.Bits.TrainingPatternMode == CA_TRAINING_PATTERN_SIMPLE) {
        FixupAddrBitsDdr5 (Sig, Ca13Enabled (Host, Socket, Ch, Dimm, Rank), AddressPattern0Local, AddressPattern1Local);
      }
      ProgramCADB2 (Host, Socket, Ch, SubChannelMask, 0, CADB2_LINES, CADB_BUF_MODE_DDR5_DESELECT, TRUE, AddressPattern0Local, AddressPattern1Local);
    } else {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "Warning: Unexpected CA signal to be trained: %d \n", Sig);
      continue;
    }

    if (Mode != RestoreCadbPattern) {
      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        if (!((1 << SubCh) & SubChannelMask)) {
          continue;
        }
        //
        // Enable CADB
        //
        SetCadbDselUniseqPat (Host, Socket, Ch, SubCh, CadbMuxPattern);
        SetCadbDselUniseqCfg (Host, Socket, Ch, SubCh, CadbMuxCtl, LfsrSize);

        GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_DLY_MCCADB_MAIN_REG, &CadbDly.Data);
        CadbDly.Bits.start_delay = 4; // Delay at least 4 clocks
        GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_DLY_MCCADB_MAIN_REG, &CadbDly.Data);

        GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);
        CadbCfg.Bits.cadb_mode = CADB_MODE_ALWAYS_ON;
        CadbCfg.Bits.cadb_dsel_throttle_mode = 1;
        GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);

        CadbAoMrsCfg.Data = 0;
        CadbAoMrsCfg.Bits.mrs_ao_repeats = 0; //Infinite Pattern
        CadbAoMrsCfg.Bits.mrs_gap_scale = 0;  //Linear
        CadbAoMrsCfg.Bits.mrs_gap = 1;
        GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_AO_MRSCFG_MCCADB_MAIN_REG, &CadbAoMrsCfg.Data);
      }
    }
  }
  return Status;
} // CpgcCaClkPattern

/**

  RCD DCA Duty Cycle Training Pattern Setup
  It is used to setup RCD DCA Duty Cycle training patterns for all channels defined by input parameter "ChannelMask"

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket to train
  @param[in] ChannelMask      - Ch Bit mask for which test should be setup fo
  @param[in] Rank             - Rank number
  @param[in] Subrank          - Subrank number
  @param[in] Sig              - Signal
  @param[in] Mode             - Programming mode.

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcRcdDcaDutyCyclePattern (
  IN PSYSHOST                  Host,
  IN UINT8                     Socket,
  IN UINT32                    ChannelMask,
  IN UINT8                     Rank,
  IN UINT8                     Subrank,
  IN GSM_CSN                   Sig,
  IN CPGC_ADV_CMD_PARITY_MODE  Mode
  )
{
  UINT32                                        Index;
  MRC_MST                                       MemSsType;
  UINT32                                        Status = SUCCESS;
  UINT8                                         Ch;
  UINT8                                         SubCh;
  struct channelNvram                           (*ChannelNvList) [MAX_CH];
  UINT32                                        CadbMuxPattern[CADB_MUX_MAX];
  UINT32                                        CadbMuxCtl[CADB_MUX_MAX];
  UINT32                                        LfsrSize[CADB_MUX_MAX] = {CPGC_DONT_CARE,
                                                                          CPGC_DONT_CARE,
                                                                          CPGC_DONT_CARE,
                                                                          CPGC_DONT_CARE};
  PatCadb2Prog0                                 AddressPattern0Local[CADB2_LINES];
  PatCadb2Prog1                                 AddressPattern1Local[CADB2_LINES];
  CADB_CFG_MCCADB_MAIN_STRUCT                   CadbCfg;
  CADB_AO_MRSCFG_MCCADB_MAIN_STRUCT             CadbAoMrsCfg;
  CADB_DLY_MCCADB_MAIN_STRUCT                   CadbDly;
  UINT8                                         SubChannelMask = 0;


  ChannelNvList = GetChannelNvList (Host, Socket);
  MemSsType     = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

  SubChannelSelect (DcaDckDutyCycleTraining, Sig, &SubChannelMask);

  for (Index = 0; Index < CADB2_LINES; Index++) {
    CopyMem (&AddressPattern0Local[Index], &RcdDcaDutyCyclePattern0[Index], sizeof (PatCadb2Prog0));
    CopyMem (&AddressPattern1Local[Index], &BasicPattern1[Index], sizeof (PatCadb2Prog1));
  }

  if (Host->var.mem.socket[Socket].TrainingStatus.Bits.TrainingPatternMode == DCA_DCK_DUTYE_CYCLE_TRAINING_PATTERN_COMPLEX ||
      Host->var.mem.socket[Socket].TrainingStatus.Bits.TrainingPatternMode == CA_TRAINING_PATTERN_XTALK ||
      Host->var.mem.socket[Socket].TrainingStatus.Bits.TrainingPatternMode == DCA_DCK_DUTYE_CYCLE_TRAINING_PATTERN_COMPLEX_CS_ALWAYS_LOW
      ) {
    //
    // DCA Duty Cycle - Complex pattern (default)
    //
    CadbMuxPattern[0] = CADB_MUX_PATTERN0_DCA_DFE_LFSR; // seed 0 for target CA
    CadbMuxPattern[1] = CADB_MUX_PATTERN1_DCA_DFE_LFSR; // seed 1 for non-target CAs
    CadbMuxCtl[0] = CADB_MUX_CTRL_LFSR; // 32 bit LFSR, target CA uniseq mode
    CadbMuxCtl[1] = CADB_MUX_CTRL_LFSR; // 32 bit LFSR, non-target CAs uniseq mode
    LfsrSize[0] = CADB_LFSR_BIT_32;
    LfsrSize[1] = CADB_LFSR_BIT_32;
  } else {
    //
    // DCA Duty Cycle - Relaxed pattern if complex pattern can't be passed
    //
    CadbMuxPattern[0] = CADB_MUX_PATTERN0_DCA_DFE_LFSR; // seed 0 for target CA
    CadbMuxPattern[1] = CADB_MUX_PATTERN1_DCA_DFE_QUIET; // quiet pattern for non-target CAs
    CadbMuxCtl[0] = CADB_MUX_CTRL_LFSR; // 32 bit LFSR, target CA uniseq mode
    CadbMuxCtl[1] = CADB_MUX_CTRL_BUFFER; // non-target CAs uniseq mode
    LfsrSize[0] = CADB_LFSR_BIT_32;
  }

  //
  // DCA Duty Cycle Relaxed pattern and Complex pattern have same CS toggling
  //
  CadbMuxPattern[2] = CADB_MUX_PATTERN2_DCA_DUTY_CYCLE; // cs pattern, asserted at every 4 tCK
  if (Host->var.mem.socket[Socket].TrainingStatus.Bits.TrainingPatternMode == DCA_DCK_DUTYE_CYCLE_TRAINING_PATTERN_COMPLEX_CS_ALWAYS_LOW) {
    CadbMuxPattern[2] = 0; // CS always low and being asserted.
  }
  CadbMuxPattern[3] = 0; // not used
  CadbMuxCtl[2] = CADB_MUX_CTRL_BUFFER; // cs uniseq mode
  CadbMuxCtl[3] = CADB_MUX_CTRL_BUFFER; // not used

  for (Ch = 0; Ch < GetMaxChDdr (); Ch++) {
    if ((ChannelMask & (1 << Ch)) == 0) {
      continue;
    }
    RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "CpgcRcdDcaDutyCyclePattern %d\n", Sig);

    SetCadbModeEn (Host, Socket, 1 << Ch, TRUE);

    //
    // CADB entries for signals.
    //
    if ((Sig >= A0A) && (Sig <= PARB)) {
      FixupCS (Rank, Subrank, AddressPattern0Local, (*ChannelNvList) [Ch].encodedCSMode);
      //
      // This step is for Host -> RCD side only
      // need to set CA13Enabled is always to TRUE
      // since CA13 and CA7 share same location on RCD side
      //
      FixupAddrBitsDdr5 (Sig, TRUE, AddressPattern0Local, AddressPattern1Local);
      ProgramCADB2 (Host, Socket, Ch, SubChannelMask, 0, CADB2_LINES, CADB_BUF_MODE_DDR5_DESELECT, TRUE, AddressPattern0Local, AddressPattern1Local);
      OverrideRcdDcaDutyCycle (Host, Socket, Ch, AddressPattern0Local);
    } else {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "Warning: Unexpected CA signal to be trained: %d \n", Sig);
      continue;
    }

    if (Mode != RestoreCadbPattern) {
      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        if (!((1 << SubCh) & SubChannelMask)) {
          continue;
        }
        //
        // Enable CADB
        //
        SetCadbDselUniseqPat (Host, Socket, Ch, SubCh, CadbMuxPattern);
        SetCadbDselUniseqCfg (Host, Socket, Ch, SubCh, CadbMuxCtl, LfsrSize);

        GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_DLY_MCCADB_MAIN_REG, &CadbDly.Data);
        CadbDly.Bits.start_delay = 4; // Delay at least 4 clocks
        GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_DLY_MCCADB_MAIN_REG, &CadbDly.Data);

        GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);
        CadbCfg.Bits.cadb_mode = CADB_MODE_ALWAYS_ON;
        CadbCfg.Bits.cadb_dsel_throttle_mode = 1;
        GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);

        CadbAoMrsCfg.Data = 0;
        CadbAoMrsCfg.Bits.mrs_ao_repeats = 0; //Infinite Pattern
        CadbAoMrsCfg.Bits.mrs_gap_scale = 0;  //Linear
        CadbAoMrsCfg.Bits.mrs_gap = 1;
        GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_AO_MRSCFG_MCCADB_MAIN_REG, &CadbAoMrsCfg.Data);
      }
    }
  }
  return Status;
} // CpgcRcdDcaDutyCyclePattern

/**

  RCD DCA DFE Training Pattern Setup
  It is used to setup RCD DCA DFE training patterns for all channels defined by input parameter "ChannelMask"

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket to train
  @param[in] ChannelMask      - Ch Bit mask for which test should be setup fo
  @param[in] Rank             - Rank number
  @param[in] Subrank          - Subrank number
  @param[in] Sig              - Signal
  @param[in] Mode             - Programming mode.

  @retval SUCCESS / FAILURE

  **/
UINT32
CpgcRcdDcaDfePattern (
  IN PSYSHOST                  Host,
  IN UINT8                     Socket,
  IN UINT32                    ChannelMask,
  IN UINT8                     Rank,
  IN UINT8                     Subrank,
  IN GSM_CSN                   Sig,
  IN CPGC_ADV_CMD_PARITY_MODE  Mode
  )
{
  UINT32                                        Index;
  MRC_MST                                       MemSsType;
  UINT32                                        Status = SUCCESS;
  UINT8                                         Ch;
  UINT8                                         SubCh;
  struct channelNvram                           (*ChannelNvList) [MAX_CH];
  UINT32                                        CadbMuxPattern[CADB_MUX_MAX];
  UINT32                                        CadbMuxCtl[CADB_MUX_MAX];
  PatCadb2Prog0                                 AddressPattern0Local[CADB2_LINES];
  PatCadb2Prog1                                 AddressPattern1Local[CADB2_LINES];
  CADB_CFG_MCCADB_MAIN_STRUCT                   CadbCfg;
  CADB_AO_MRSCFG_MCCADB_MAIN_STRUCT             CadbAoMrsCfg;
  CADB_DLY_MCCADB_MAIN_STRUCT                   CadbDly;
  UINT8                                         SubChannelMask = 0;
  UINT32                                        CadbPoly[CADB_MUX_MAX] = {GALOIS_POLY_8,
                                                                          GALOIS_POLY_8,
                                                                          CPGC_DONT_CARE,
                                                                          CPGC_DONT_CARE};
  UINT32                                        LfsrSize[CADB_MUX_MAX] = {CADB_8_BIT_LFSR,
                                                                          CADB_8_BIT_LFSR,
                                                                          CADB_8_BIT_LFSR,
                                                                          CADB_8_BIT_LFSR};
  ChannelNvList = GetChannelNvList (Host, Socket);
  MemSsType     = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

  SubChannelSelect (RcdDcaDfeDdr5, Sig, &SubChannelMask);

  for (Index = 0; Index < CADB2_LINES; Index++) {

    if ((Sig == PARA) || (Sig == PARB)) {
      CopyMem (&AddressPattern0Local[Index], &RcdDcaDfeDparPattern0[Index], sizeof (PatCadb2Prog0));
      CopyMem (&AddressPattern1Local[Index], &BasicDcaDfeDparPattern1[Index], sizeof (PatCadb2Prog1));
    } else {
      CopyMem (&AddressPattern0Local[Index], &RcdDcaDfePattern0[Index], sizeof (PatCadb2Prog0));
      CopyMem (&AddressPattern1Local[Index], &BasicDcaDfePattern1[Index], sizeof (PatCadb2Prog1));
    }
  }

  if (Host->var.mem.socket[Socket].TrainingStatus.Bits.TrainingPatternMode == CA_TRAINING_PATTERN_XTALK) {
    //
    // DCA DFE - Complex pattern for the rest of iterations
    //
    CadbMuxPattern[0] = CADB_MUX_PATTERN0_DCA_DFE_LFSR; // seed 0 for target CA
    CadbMuxPattern[1] = CADB_MUX_PATTERN1_DCA_DFE_LFSR; // seed 1 for non-target CAs
    CadbMuxCtl[0] = CADB_MUX_CTRL_LFSR_GALOIS; // target CA uniseq mode
    CadbMuxCtl[1] = CADB_MUX_CTRL_LFSR_GALOIS; // non-target CAs uniseq mode
  } else {
    //
    // DCA DFE - Simple pattern for the first iteration
    //
    CadbMuxPattern[0] = CADB_MUX_PATTERN0_DCA_DFE_LFSR; // seed 0 for target CA
    CadbMuxPattern[1] = CADB_MUX_PATTERN1_DCA_DFE_QUIET; // quiet pattern for non-target CAs
    CadbMuxCtl[0] = CADB_MUX_CTRL_LFSR_GALOIS; // target CA uniseq mode
    CadbMuxCtl[1] = CADB_MUX_CTRL_BUFFER; // non-target CAs uniseq mode
  }

  //
  // DCA DFE Simple pattern and Complex pattern have same CS toggling
  //
  CadbMuxPattern[2] = 0; // cs pattern, need always assert when sending patterns
  CadbMuxPattern[3] = 0; // not used
  CadbMuxCtl[2] = CADB_MUX_CTRL_BUFFER; // cs uniseq mode
  CadbMuxCtl[3] = CADB_MUX_CTRL_BUFFER; // not used

  for (Ch = 0; Ch < GetMaxChDdr (); Ch++) {
    if ((ChannelMask & (1 << Ch)) == 0) {
      continue;
    }
    RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "CpgcRcdDcaDfePattern %d\n", Sig);
    SetCadbModeEn (Host, Socket, 1 << Ch, TRUE);

    //
    // CADB entries for signals.
    //
    if ((Sig >= A0A) && (Sig <= PARB)) {
      FixupCS (Rank, Subrank, AddressPattern0Local, (*ChannelNvList) [Ch].encodedCSMode);
      //
      // This step is for Host -> RCD side only
      // need to set CA13Enabled is always to TRUE
      // since CA13 and CA7 share same location on RCD side
      //
      FixupAddrBitsDdr5 (Sig, TRUE, AddressPattern0Local, AddressPattern1Local);
      ProgramCADB2 (Host, Socket, Ch, SubChannelMask, 0, CADB2_LINES, CADB_BUF_MODE_DDR5_DESELECT, TRUE, AddressPattern0Local, AddressPattern1Local);
    } else {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "Warning: Unexpected CA signal to be trained: %d \n", Sig);
      continue;
    }

    if (Mode != RestoreCadbPattern) {
      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        if (!((1 << SubCh) & SubChannelMask)) {
          continue;
        }
        //
        // Enable CADB
        //
        SetCadbDselUniseqPat (Host, Socket, Ch, SubCh, CadbMuxPattern);
        SetCadbDselUniseqCfg (Host, Socket, Ch, SubCh, CadbMuxCtl, LfsrSize);

        GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_DLY_MCCADB_MAIN_REG, &CadbDly.Data);
        CadbDly.Bits.start_delay = 4; // Delay at least 4 clocks
        GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_DLY_MCCADB_MAIN_REG, &CadbDly.Data);

        GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);
        CadbCfg.Bits.cadb_mode = CADB_MODE_ALWAYS_ON;
        CadbCfg.Bits.cadb_dsel_throttle_mode = 1;
        GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);

        CadbAoMrsCfg.Data = 0;
        CadbAoMrsCfg.Bits.mrs_ao_repeats = PATTERN_REPEATS_DCADFE; //finite Pattern 256
        CadbAoMrsCfg.Bits.mrs_gap_scale = 0;  //Linear
        CadbAoMrsCfg.Bits.mrs_gap = 1;
        GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_AO_MRSCFG_MCCADB_MAIN_REG, &CadbAoMrsCfg.Data);

        SetCadbDselUniseqPoly (Host, Socket, Ch, SubCh, CadbPoly);
      }
    }
  }
  return Status;
} // CpgcRcdDcaDfePattern

/**

  Fix up the address bit field when programming address patterns

  @param[in]     Sig            - Signal to test
  @param[in]     Ca13Enable     - Ca13 is enabled or not
  @param[in out] Prog0          - Data to program
  @param[in out] Prog1          - Data to program

  @retval N/A

**/
STATIC VOID
FixupAddrBitsDdr5 (
  IN GSM_CSN            Sig,
  IN BOOLEAN            Ca13Enable,
  IN OUT PatCadb2Prog0  Prog0[CADB2_LINES],
  IN OUT PatCadb2Prog1  Prog1[CADB2_LINES]
  )
{
  UINT8           CadbLine;
  PSYSHOST        Host;
  UINT8           Socket;
  GSM_CSN         BaseSignal;

  Host = GetSysHostPointer ();
  Socket = GetCurrentSocketId ();
  if (GetCurrentTestType (Socket) == DcaDfeDdr5Test) {
    for (CadbLine = 0; CadbLine < CADB2_LINES; CadbLine++) {
      if (SIG_COMPARE (Sig, A0A, A13A)) {
        Prog0[CadbLine].ca = DCA_PATTERN_SHIFT_WITHCA7 (Sig, A0A, Prog0[CadbLine].ca);
      } else if (SIG_COMPARE (Sig, A0B, A13B)) {
        Prog0[CadbLine].ca = DCA_PATTERN_SHIFT_WITHCA7 (Sig, A0B, Prog0[CadbLine].ca);
      }
    }
    return;
  } //if

  if (SIG_COMPARE (Sig, A0A, PARB)) {
    //
    // All DCA
    //
    if (Ca13Enable) {
      for (CadbLine = 0; CadbLine < CADB2_LINES; CadbLine++) {
        if ((GetCurrentTestType (Socket) == EarlyRcdCaLoopbackTest) ||
            (GetCurrentTestType (Socket) == DcaDckDutyCycleTest)) {
          if (SIG_COMPARE (Sig, A0A, A13A)) {
            Prog0[CadbLine].ca = DCA_PATTERN_SHIFT_WITHCA13 (Sig, A0A, Prog0[CadbLine].ca);
          } else if (SIG_COMPARE (Sig, A0B, A13B)) {
            Prog0[CadbLine].ca = DCA_PATTERN_SHIFT_WITHCA13 (Sig, A0B, Prog0[CadbLine].ca);
          }
        }
      } // CadbLine loop
    } else { // Ca13 is not enabled
      for (CadbLine = 0; CadbLine < CADB2_LINES; CadbLine++) {
        if (SIG_COMPARE (Sig, A0A, A13A)) {
          Prog0[CadbLine].ca = DCA_PATTERN_SHIFT_WITHOUTCA13 (Sig, A0A, Prog0[CadbLine].ca);
        } else if (SIG_COMPARE (Sig, A0B, A13B)) {
          Prog0[CadbLine].ca = DCA_PATTERN_SHIFT_WITHOUTCA13 (Sig, A0B, Prog0[CadbLine].ca);
        }
      } // CadbLine loop
    }
  } else if (SIG_COMPARE (Sig, Q0ACA0, Q1BCA13)) {
    //
    // All QCA
    //
    if (SIG_COMPARE (Sig, Q0ACA0, Q0ACA6)) {
      BaseSignal = Q0ACA0;
    } else if (SIG_COMPARE (Sig, Q0ACA7, Q0ACA13)) {
      BaseSignal = Q0ACA7;
    } else if (SIG_COMPARE (Sig, Q1ACA0, Q1ACA6)) {
      BaseSignal = Q1ACA0;
    } else if (SIG_COMPARE (Sig, Q1ACA7, Q1ACA13)) {
      BaseSignal = Q1ACA7;
    } else if (SIG_COMPARE (Sig, Q0BCA0, Q0BCA6)) {
      BaseSignal = Q0BCA0;
    } else if (SIG_COMPARE (Sig, Q0BCA7, Q0BCA13)) {
      BaseSignal = Q0BCA7;
    } else if (SIG_COMPARE (Sig, Q1BCA0, Q1BCA6)) {
      BaseSignal = Q1BCA0;
    } else if (SIG_COMPARE (Sig, Q1BCA7, Q1BCA13)) {
      BaseSignal = Q1BCA7;
    }

    for (CadbLine = 0; CadbLine < CADB2_LINES; CadbLine++) {
      Prog0[CadbLine].ca = QCA_PATTERN_SHIFT (Sig, BaseSignal, Prog0[CadbLine].ca);
    } // CadbLine loop
  }
} // FixupAddrBitsdDR5

/**

  Early BCOM CLK training pattern sets up.
  It is used to setup BCOM CLK training patterns for all channels defined by input parameter "ChannelMask"

  @param[in] Host                - Pointer to sysHost
  @param[in] Socket              - Socket to train
  @param[in] ChannelMask         - Ch Bit mask for which test should be setup for
  @param[in] RankInCh            - Rank number in channel
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
  UINT8                              Ch;
  UINT32                             i;
  UINT8                              SubCh;
  UINT32                             CadbMuxPattern[CADB_MUX_MAX];
  UINT32                             CadbMuxCtl[CADB_MUX_MAX];
  PatCadb2Prog0                      BcomClkPattern0Buf[CADB2_LINES];
  PatCadb2Prog1                      BcomClkPattern1Buf[CADB2_LINES];
  CADB_CFG_MCCADB_MAIN_STRUCT        CadbCfg;
  CADB_AO_MRSCFG_MCCADB_MAIN_STRUCT  CadbAoMrsCfg;
  CADB_DLY_MCCADB_MAIN_STRUCT        CadbDly;
  struct channelNvram                (*ChannelNvList)[MAX_CH];
  UINT32                             LfsrSize[CADB_MUX_MAX] = {CPGC_DONT_CARE,
                                                               CPGC_DONT_CARE,
                                                               CPGC_DONT_CARE,
                                                               CPGC_DONT_CARE};
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "CpgcBcomClkPattern entry\n");
  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // Program CADB entries
  //
  for (i = 0; i < CADB2_LINES; i++) {
    CopyMem (&BcomClkPattern0Buf[i], &BcomPattern0[i], sizeof (PatCadb2Prog0));
    CopyMem (&BcomClkPattern1Buf[i], &BcomPattern1[i], sizeof (PatCadb2Prog1));
  }

  //
  // Program CADB pattern buffer
  //
  CadbMuxPattern[0] = CADB_MUX_PATTERN0_BCOM;
  CadbMuxPattern[1] = CADB_MUX_PATTERN1_BCOM;
  CadbMuxPattern[2] = CADB_MUX_PATTERN2_BCOM;
  CadbMuxPattern[3] = 0;

  CadbMuxCtl[0] = CADB_MUX_CTRL_LFSR;
  CadbMuxCtl[1] = CADB_MUX_CTRL_LFSR;
  CadbMuxCtl[2] = CADB_MUX_CTRL_BUFFER;
  CadbMuxCtl[3] = CADB_MUX_CTRL_BUFFER;
  LfsrSize[0] = CADB_LFSR_BIT_32;
  LfsrSize[1] = CADB_LFSR_BIT_32;


  CadbCfg.Data = 0;
  CadbCfg.Bits.cadb_mode = CADB_MODE_ALWAYS_ON; // AO mode
  // DDR5 RDIMM, need 2N mode (we need the unisequencers toggling at DCLK, not 2X DCLK) as BCOM only samples rising edge of CLK
  CadbCfg.Bits.cadb_dsel_throttle_mode = 2;
  // Sel not really applicable in Always On mode, but keeping it consistent with the Dsel setting
  CadbCfg.Bits.cadb_sel_throttle_mode = 2;
  // Make it consistent, although not totally required in Always On mode
  CadbCfg.Bits.initial_dsel_en = 1;
  // Make it consistent, although not totally required in Always On mode
  CadbCfg.Bits.lane_deselect_en = DRIVE_CADB_PATTERN_ON_ALL_PINS;

  CadbAoMrsCfg.Data = 0;
  CadbAoMrsCfg.Bits.mrs_ao_repeats = 0; // Infinite Pattern
  CadbAoMrsCfg.Bits.mrs_gap_scale = 0;  // Linear
  CadbAoMrsCfg.Bits.mrs_gap = 0;        // No Gap

  CadbDly.Data = 0;
  CadbDly.Bits.start_delay = 4; // Set start delay to 4 for aligning to even cycle

  for (Ch = 0; Ch < GetMaxChDdr (); Ch += GetChInc (Host)) {
    if ((ChannelMask & (1 << Ch)) == 0) {
      continue;
    }
    FixupCS (RankInCh, 0, BcomClkPattern0Buf, (*ChannelNvList) [Ch].encodedCSMode);
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      ProgramCADB2 (Host, Socket, Ch, 1 << SubCh, 0, CADB2_LINES, CADB_BUF_MODE_DDR5_DESELECT, FALSE, BcomClkPattern0Buf, BcomClkPattern1Buf);
      SetCadbDselUniseqPat (Host, Socket, Ch, SubCh, CadbMuxPattern);
      SetCadbDselUniseqCfg (Host, Socket, Ch, SubCh, CadbMuxCtl, LfsrSize);
      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);
      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_AO_MRSCFG_MCCADB_MAIN_REG, &CadbAoMrsCfg.Data);
      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_DLY_MCCADB_MAIN_REG, &CadbDly.Data);
    }
  }  // Ch loop
  return SUCCESS;
}

/**

  This function sends the NOP command by CADB for DDR5.

  @param[in] Host            - include all the MRC data
  @param[in] Socket          - socket number
  @param[in] Channel         - ch to send command to
  @param[in] Dimm            - Dimm to send command to
  @param[in] Subchannelmask  - sub channel mask
  @param[in] Rank            - CS to send the command to
  @param[in] Cw              - CW to use
  @param[in] Cmd             - which command: NOP_CMD
  @param[in] PassThroughMode - Pass Through Mode
  @param[in] ExitCATM        - exit catm

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
  UINT8                                         SubCh = 0;
  UINT32                                        i;
  UINT32                                        LfsrSize[CADB_MUX_MAX] = {0, 0, 0, 0};
  UINT32                                        MuxCtl[CADB_MUX_MAX] = {BTBUFFER, BTBUFFER, BTBUFFER, BTBUFFER};
  UINT32                                        Patbuf[CADB_MUX_MAX] = {SEQUENTIAL_BUFFER_PATTERN_0, SEQUENTIAL_BUFFER_PATTERN_1, SEQUENTIAL_BUFFER_PATTERN_2, SEQUENTIAL_BUFFER_PATTERN_3};
  struct channelNvram                           (*ChannelNvList) [MAX_CH];
  PatCadb2Prog0                                 NopPattern0Local[CADB2_LINES];
  PatCadb2Prog1                                 NopCADBPattern1Local[CADB2_LINES];
  CADB_MRSCFG_N0_MCCADB_MAIN_STRUCT             CadbMrscfgN0;
  CADB_MRSCFG_N1_MCCADB_MAIN_STRUCT             CadbMrscfgN1;
  CADB_CFG_MCCADB_MAIN_STRUCT                   CadbCfg;
  CADB_AO_MRSCFG_MCCADB_MAIN_STRUCT             CadbAoMrscfg;
  CADB_DLY_MCCADB_MAIN_STRUCT                   CadbDly;
  CADB_SELCFG_MCCADB_MAIN_STRUCT                CadbSelCfg;
  UINT8                                         LogicalRank;

  LogicalRank = GetLogicalRank (Host, Socket, Channel, Dimm, Rank);

  if (ExitCATM) {
     WriteCadbNopCmdExitCatm (Host, Socket, Channel, Dimm, Subchannelmask, Rank, Cw, Cmd, PassThroughMode, ExitCATM);
     return;
  }

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "WriteCadbNopCmd starts Cmd %d\n", Cmd);

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {

    if (Host->DdrioUltSupport) {
      if (Cmd == NOP_CMD) {
        if (ExitCATM) {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
            "NOP Command to exit CATM\n");
          CteDdrCmd ("nop", 1 << Channel, Subchannelmask, GetPhyRank (Dimm, Rank), 0, 0);
          CteDdrCmd ("nop", 1 << Channel, Subchannelmask, GetPhyRank (Dimm, Rank), 0, 0);
        } else {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
            "NOP Command\n");
          CteDdrCmd ("nop", 1 << Channel, Subchannelmask, GetPhyRank (Dimm, Rank), 0, 0);
        }
        return;
      }
    }
  }

  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // Configure CADB
  //
  SetCadbModeEn (Host, Socket, 1 << Channel, TRUE);
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & (Subchannelmask)) == 0) {
      continue;
    }

    //
    // Config Cadb_cfg
    //
    GetSetCadbRegs (Host, Socket, Channel, SubCh, CPGC_REG_READ, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);
    CadbCfg.Data = 0;
    CadbCfg.Bits.cadb_mode = CADB_MODE_ALWAYS_ON;
    CadbCfg.Bits.cadb_dsel_throttle_mode = 1;
    GetSetCadbRegs (Host, Socket, Channel, SubCh, CPGC_REG_WRITE, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);

    //
    // Config cadb_ao_mrscfg
    //
    GetSetCadbRegs (Host, Socket, Channel, SubCh, CPGC_REG_READ, CADB_AO_MRSCFG_MCCADB_MAIN_REG, &CadbAoMrscfg.Data);
    CadbAoMrscfg.Bits.mrs_ao_repeats = 1;
    CadbAoMrscfg.Bits.mrs_gap = 1;
    CadbAoMrscfg.Bits.mrs_gap_scale = 0;
    GetSetCadbRegs (Host, Socket, Channel, SubCh, CPGC_REG_WRITE, CADB_AO_MRSCFG_MCCADB_MAIN_REG, &CadbAoMrscfg.Data);

    //
    // Config cadb_mrscfg
    //
    GetSetCadbRegs (Host, Socket, Channel, SubCh, CPGC_REG_READ, CADB_MRSCFG_N1_MCCADB_MAIN_REG, &CadbMrscfgN1.Data);
    GetSetCadbRegs (Host, Socket, Channel, SubCh, CPGC_REG_READ, CADB_MRSCFG_N0_MCCADB_MAIN_REG, &CadbMrscfgN0.Data);
    CadbMrscfgN1.Data = 0;
    CadbMrscfgN0.Data = 0;
    GetSetCadbRegs (Host, Socket, Channel, SubCh, CPGC_REG_WRITE, CADB_MRSCFG_N1_MCCADB_MAIN_REG, &CadbMrscfgN1.Data);
    GetSetCadbRegs (Host, Socket, Channel, SubCh, CPGC_REG_WRITE, CADB_MRSCFG_N0_MCCADB_MAIN_REG, &CadbMrscfgN0.Data);

    //
    // Config Cadb_Dly
    //
    GetSetCadbRegs (Host, Socket, Channel, SubCh, CPGC_REG_READ, CADB_DLY_MCCADB_MAIN_REG, &CadbDly.Data);
    CadbDly.Data = 0;
    GetSetCadbRegs (Host, Socket, Channel, SubCh, CPGC_REG_WRITE, CADB_DLY_MCCADB_MAIN_REG, &CadbDly.Data);

    //
    // Config Cadb_Selcfg
    //
    GetSetCadbRegs (Host, Socket, Channel, SubCh, CPGC_REG_READ, CADB_SELCFG_MCCADB_MAIN_REG, &CadbSelCfg.Data);
    CadbSelCfg.Data = 0;
    GetSetCadbRegs (Host, Socket, Channel, SubCh, CPGC_REG_WRITE, CADB_SELCFG_MCCADB_MAIN_REG, &CadbSelCfg.Data);

    //
    // Configure uniseq_lfsr_polynomial_size(0) and uniseq_mode(1)
    //
    SetCadbDselUniseqCfg (Host, Socket, Channel, SubCh, MuxCtl, LfsrSize);

    //
    // Program the CADB buffer pattern
    //
    SetCadbDselUniseqPat (Host, Socket, Channel, SubCh, Patbuf);
  } // Subch loop

  if ((*ChannelNvList) [Channel].enabled == 0) {
    return;
  }

  for (i = 0; i < CADB2_LINES; i++) {
    CopyMem (&NopPattern0Local[i], &NopPattern0[i], sizeof (PatCadb2Prog0));
    CopyMem (&NopCADBPattern1Local[i], &BasicPattern1[i], sizeof (PatCadb2Prog1));
  }
  FixupCS (LogicalRank, 0, NopPattern0Local, (*ChannelNvList) [Channel].encodedCSMode);
  ProgramCADB2 (Host, Socket, Channel, Subchannelmask, 0, CADB2_LINES, CADB_BUF_MODE_DDR5_DESELECT, FALSE, NopPattern0Local, NopCADBPattern1Local);

  //
  // Start test
  //
  CadbTestStart (Host, Socket, (1 << Channel), Subchannelmask);

  //
  // Poll test completion
  //
  CadbPollTestDone (Host, Socket, 1 << Channel, Subchannelmask);

  SetCadbModeEn (Host, Socket, 1 << Channel, FALSE);

  return;
} // WriteCadbNopCmd

/**

  Configure uniseq mode and pbuf for QCA training

  @param[in]  TrainingPatternMode     - QCA training pattern mode: QCA_TRAINING_PATTERN_XTALK or QCA_TRAINING_PATTERN_SIMPLE
  @param[in]  TimingMode              - QCA training timing mode: TIMING_1N or TIMING_2N
  @param[in]  IsCa13En                - Check whether or not CA13 is enabled
  @param[in]  Sig                     - QCA Signal
  @param[out] CadbMuxPattern          - Pattern buffer array
  @param[out] CadbMuxCtl              - Uniseq mux array
  @param[out] LfsrSize                - LFSR size array

  @retval NA

**/
VOID
ConfigQcaCadbUniseq (
  IN     UINT8   TrainingPatternMode,
  IN     UINT8   TimingMode,
  IN     BOOLEAN IsCa13En,
  IN     GSM_CSN Sig,
     OUT UINT32  CadbMuxPattern[],
     OUT UINT32  CadbMuxCtl[],
     OUT UINT32  LfsrSize[]
  )
{
  if (TrainingPatternMode == QCA_TRAINING_PATTERN_XTALK) {
    //
    // Common settings for Xtalk pattern
    //
    CadbMuxCtl[0] = CADB_MUX_CTRL_LFSR;
    CadbMuxCtl[1] = CADB_MUX_CTRL_LFSR;
    CadbMuxCtl[2] = CADB_MUX_CTRL_BUFFER;
    CadbMuxCtl[3] = CADB_MUX_CTRL_BUFFER;
    LfsrSize[0] = CADB_LFSR_BIT_32;
    LfsrSize[1] = CADB_LFSR_BIT_32;

    if (TimingMode == TIMING_1N) {
      //
      // Xtalk pattern, 1N
      //
      CadbMuxPattern[0] = CADB_MUX_PATTERN0_QCA_XTALK_LFSR0;
      CadbMuxPattern[1] = CADB_MUX_PATTERN1_QCA_XTALK_LFSR1;

      if (IsCa13En) {
        //
        // Xtalk pattern, 1N, with CA13 supported
        //
        CadbMuxPattern[2] = CADB_MUX_PATTERN2_QCA_XTALK_1N_WITH_CA13;
      } else {
        //
        // Xtalk pattern, 1N, without CA13 supported
        //
        CadbMuxPattern[2] = CADB_MUX_PATTERN2_QCA_XTALK_1N_WITHOUT_CA13;
      }

      CadbMuxPattern[3] = CADB_MUX_PATTERN3_QCA_XTALK_1N_CS;

    } else {
      //
      // Xtalk pattern, 2N
      //
      if ((!IsCa13En) && IsQca6 (Sig)) {
        //
        // Xtalk pattern, 2N, without CA13 supported, QCA6 only
        //
        CadbMuxPattern[0] = CADB_MUX_PATTERN0_QCA_XTALK_2N_LFSR0_QCA6_WITHOUT_CA13;
        CadbMuxPattern[1] = CADB_MUX_PATTERN1_QCA_XTALK_2N_LFSR1_QCA6_WITHOUT_CA13;
        CadbMuxPattern[2] = CADB_MUX_PATTERN2_QCA_XTALK_2N_PAR_QCA6_WITHOUT_CA13;
        CadbMuxPattern[3] = CADB_MUX_PATTERN3_QCA_XTALK_2N_CS_QCA6_WITHOUT_CA13;
      } else {
        //
        // Xtalk pattern, 2N, with QCA13 supported, or without QCA13 supported when sweeping QCA0-QCA5 or QCA7-QCA12
        //
        CadbMuxPattern[0] = CADB_MUX_PATTERN0_QCA_XTALK_LFSR0;
        CadbMuxPattern[1] = CADB_MUX_PATTERN1_QCA_XTALK_LFSR1;
        CadbMuxPattern[2] = CADB_MUX_PATTERN2_QCA_XTALK_2N_PAR;
        CadbMuxPattern[3] = CADB_MUX_PATTERN3_QCA_XTALK_2N_CS;
      }
    }
  } else {
    //
    // Common settings for Simple pattern
    //
    CadbMuxCtl[0] = CADB_MUX_CTRL_BUFFER;
    CadbMuxCtl[1] = CADB_MUX_CTRL_BUFFER;
    CadbMuxCtl[2] = CADB_MUX_CTRL_BUFFER;
    CadbMuxCtl[3] = CADB_MUX_CTRL_BUFFER;

    //
    // Uniseq 3 is not used for simple pattern
    //
    CadbMuxPattern[3] = 0;

    if (TimingMode == TIMING_1N) {
      //
      // Simple pattern, 1N
      //
      if (IsLowHalfQca (Sig)) {
        //
        // QCA0~QCA6
        //
        CadbMuxPattern[0] = CADB_MUX_PATTERN0_QCA_SIMPLE_1N_QCA0_TO_QCA6;
      } else {
        //
        // QCA7~QCA13
        //
        CadbMuxPattern[0] = CADB_MUX_PATTERN0_QCA_SIMPLE_1N_QCA7_TO_QCA13;
      }

      CadbMuxPattern[1] = CADB_MUX_PATTERN1_QCA_SIMPLE_1N_PAR;
      CadbMuxPattern[2] = CADB_MUX_PATTERN2_QCA_SIMPLE_1N_CS;

    } else {
      //
      // Simple pattern, 2N
      //
      if (IsLowHalfQca (Sig)) {
        //
        // QCA0~QCA6
        //
        CadbMuxPattern[0] = CADB_MUX_PATTERN0_QCA_SIMPLE_2N_QCA0_TO_QCA6;
      } else {
        //
        // QCA7~QCA13
        //
        CadbMuxPattern[0] = CADB_MUX_PATTERN0_QCA_SIMPLE_2N_QCA7_TO_QCA13;
      }
    }
  }
}

/**

  Configure CADB buffer for QCA training

  @param[in]  Rank                     - Rank number
  @param[in]  Subrank                  - Subrank number
  @param[in]  EncodedCSMode            - Encoded CS mode
  @param[in]  TrainingPatternMode      - QCA training pattern mode: QCA_TRAINING_PATTERN_XTALK or QCA_TRAINING_PATTERN_SIMPLE
  @param[in]  TimingMode               - QCA training timing mode: TIMING_1N or TIMING_2N
  @param[in]  IsCa13En                 - Check whether or not CA13 is enabled
  @param[in]  Sig                      - QCA Signal
  @param[out] AddressPattern0          - CADB low 32-bit buffer
  @param[out] AddressPattern1          - CADB high 32-bit buffer
  @param[out] NumLines                 - Pointer of the number of cadb buffer lines

  @retval NA

**/
VOID
ConfigQcaCadbBuf (
  IN     UINT8         Rank,
  IN     UINT8         Subrank,
  IN     UINT8         EncodedCSMode,
  IN     UINT8         TrainingPatternMode,
  IN     UINT8         TimingMode,
  IN     BOOLEAN       IsCa13En,
  IN     GSM_CSN       Sig,
     OUT PatCadb2Prog0 AddressPattern0[],
     OUT PatCadb2Prog1 AddressPattern1[],
     OUT UINT8         *NumLines
  )
{
  PatCadb2Prog0  *AddressPattern0Ptr;
  UINT8          Index;

  if (TrainingPatternMode == QCA_TRAINING_PATTERN_XTALK) {
    //
    // Xtalk pattern
    //
    if (TimingMode == TIMING_1N) {

      if ((!IsCa13En) && IsQca10ToQca12 (Sig)) {
        //
        // Xtalk pattern, 1N, without CA13 supported when sweeping QCA10 to QCA12
        //
        AddressPattern0Ptr = &QcaPattern1NQca10ToQca12WithoutCa13Xtalk0[0];
      } else {
        //
        // Xtalk pattern, 1N, with CA13 supported, or without CA13 supported when sweeping QCA0 to QCA9
        //
        AddressPattern0Ptr = &QcaPattern1NXtalk0[0];
      }
    } else {
      //
      // Xtalk pattern, 2N
      //
      if (IsCa13En) {
        //
        // Xtalk pattern, 2N, with CA13 supported
        //
        AddressPattern0Ptr = &QcaPattern2NXtalk0[0];
      } else if (IsQca6 (Sig)) {
        //
        // Xtalk pattern, 2N, without CA13 supported when sweeping QCA6 only
        //
        AddressPattern0Ptr = &QcaPattern2NQca6WithoutCa13Xtalk0[0];
      } else {
        //
        // Xtalk pattern, 2N, without CA13 supported when sweeping QCA0-QCA5 or QCA7-QCA12
        //
        AddressPattern0Ptr = &QcaPattern2NWithoutCa13Xtalk0[0];
      }

      *NumLines = CADB2_QCA_XTALK_PATTERN_LINES;
    }
  } else {
    //
    // Simple pattern
    //
    AddressPattern0Ptr = &QcaPatternSimple0[0];
    *NumLines = CADB2_QCA_SIMPLE_PATTERN_LINES;
  }

  for (Index = 0; Index < *NumLines; Index++) {
    CopyMem (&AddressPattern0[Index], &AddressPattern0Ptr[Index], sizeof (PatCadb2Prog0));
    CopyMem (&AddressPattern1[Index], &BasicPattern1[Index], sizeof (PatCadb2Prog1));
  }
  //
  // Update target QCA signal pattern and CS
  //
  FixupCS (Rank, Subrank, AddressPattern0, EncodedCSMode);
  if (TrainingPatternMode == QCA_TRAINING_PATTERN_SIMPLE) {
    //
    // Doesn't matter if Ca13Enabled is TRUE or FALSE since both cases share same pattern but with different XOR selection.
    // When Ca13Enabled is TRUE, XOR counts 0. Otherwise, counts 1.
    //
    FixupAddrBitsDdr5 (Sig, TRUE, AddressPattern0, AddressPattern1);
  }
}

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
  MRC_MST                                       MemSsType;
  UINT8                                         Ch;
  UINT8                                         SubCh;
  struct channelNvram                           (*ChannelNvList) [MAX_CH];
  UINT32                                        CadbMuxPattern[CADB_MUX_MAX];
  UINT32                                        CadbMuxCtl[CADB_MUX_MAX];
  PatCadb2Prog0                                 AddressPattern0Local[CADB2_LINES];
  PatCadb2Prog1                                 AddressPattern1Local[CADB2_LINES];
  CADB_CFG_MCCADB_MAIN_STRUCT                   CadbCfg;
  CADB_DLY_MCCADB_MAIN_STRUCT                   CadbDly;
  UINT8                                         SubChannelMask = 0;
  UINT8                                         NumLines = 0;
  UINT8                                         TrainingPatternMode;
  UINT8                                         TimingMode;
  UINT8                                         Dimm;
  CADB_MRSCFG_N1_MCCADB_MAIN_STRUCT             CadbMrscfgN1;
  CADB_MRSCFG_N0_MCCADB_MAIN_STRUCT             CadbMrscfgN0;
  CADB_AO_MRSCFG_MCCADB_MAIN_STRUCT             CadbAoMrscfg;
  UINT32                                        LfsrSize[CADB_MUX_MAX] = {CPGC_DONT_CARE,
                                                                          CPGC_DONT_CARE,
                                                                          CPGC_DONT_CARE,
                                                                          CPGC_DONT_CARE};

  ChannelNvList       = GetChannelNvList (Host, Socket);
  MemSsType           = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  Dimm                = Rank/MAX_RANK_DIMM;  //Get the dimm number the rank is on
  TrainingPatternMode = Host->var.mem.socket[Socket].TrainingStatus.Bits.TrainingPatternMode;

  if (SIG_COMPARE (Sig, Q0ACA0, Q1BCA13) == FALSE) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
      "Error: Unexpected QCA signal to be trained: %d \n", Sig);
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_198);
      return FAILURE;
  }


  SubChannelSelect (CaClkTrainingBackSide, Sig, &SubChannelMask);

  ZeroMem (&AddressPattern0Local, sizeof (PatCadb2Prog0));
  ZeroMem (&AddressPattern1Local, sizeof (PatCadb2Prog1));

  for (Ch = 0; Ch < GetMaxChDdr (); Ch++) {
    if ((ChannelMask & (1 << Ch)) == 0) {
      continue;
    }

    RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT, "CpgcQcaClkPattern\n");

    SetCadbModeEn (Host, Socket, 1 << Ch, TRUE);
    TimingMode = (*ChannelNvList) [Ch].timingMode;

    ConfigQcaCadbUniseq (TrainingPatternMode, TimingMode, Ca13Enabled (Host, Socket, Ch, Dimm, Rank), Sig, CadbMuxPattern, CadbMuxCtl, LfsrSize);
    ConfigQcaCadbBuf (Rank, Subrank, (*ChannelNvList) [Ch].encodedCSMode, TrainingPatternMode, TimingMode,
                      Ca13Enabled (Host, Socket, Ch, Dimm, Rank), Sig, AddressPattern0Local, AddressPattern1Local, &NumLines);

    ProgramCADB2 (Host, Socket, Ch, SubChannelMask, 0, NumLines, CADB_BUF_MODE_DDR5_DESELECT, TRUE, AddressPattern0Local, AddressPattern1Local);

    if (Mode == RestoreCadbPattern) {
      continue;
    }

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      if (!((1 << SubCh) & SubChannelMask)) {
        continue;
      }
      //
      // Enable CADB
      //
      SetCadbDselUniseqPat (Host, Socket, Ch, SubCh, CadbMuxPattern);
      SetCadbDselUniseqCfg (Host, Socket, Ch, SubCh, CadbMuxCtl, LfsrSize);
      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_MRSCFG_N1_MCCADB_MAIN_REG, &CadbMrscfgN1.Data);
      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_MRSCFG_N0_MCCADB_MAIN_REG, &CadbMrscfgN0.Data);
      CadbMrscfgN1.Bits.mrs_delay_cnt = 0;
      CadbMrscfgN0.Bits.mrs_cs_mode = ONE_CYCLE_CS;
      CadbMrscfgN0.Bits.mrs_goto_ptr = 0;
      CadbMrscfgN0.Bits.mrs_end_ptr = 0;
      CadbMrscfgN0.Bits.mrs_start_ptr = 0;
      CadbMrscfgN0.Bits.cs_active_polarity = 0;
      CadbMrscfgN0.Bits.setup_clocks = 0;
      CadbMrscfgN0.Bits.dsel_clocks = 0;
      CadbMrscfgN0.Bits.cs_clocks = 0;
      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_MRSCFG_N1_MCCADB_MAIN_REG, &CadbMrscfgN1.Data);
      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_MRSCFG_N0_MCCADB_MAIN_REG, &CadbMrscfgN0.Data);

      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);
      CadbCfg.Bits.cadb_mode = CADB_MODE_ALWAYS_ON;
      //
      // Drive CADB pattern on all pins including (CS, CA and PAR)
      //
      CadbCfg.Bits.lane_deselect_en = DRIVE_CADB_PATTERN_ON_ALL_PINS;
      CadbCfg.Bits.initial_dsel_en = 1;
      CadbCfg.Bits.cadb_dsel_throttle_mode = 1;
      CadbCfg.Bits.cadb_sel_throttle_mode = 1;
      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);

      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_AO_MRSCFG_MCCADB_MAIN_REG, &CadbAoMrscfg.Data);
      CadbAoMrscfg.Bits.mrs_ao_repeats = 0;
      CadbAoMrscfg.Bits.mrs_gap = 1;
      CadbAoMrscfg.Bits.mrs_gap_scale = 0;
      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_AO_MRSCFG_MCCADB_MAIN_REG, &CadbAoMrscfg.Data);

      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_DLY_MCCADB_MAIN_REG, &CadbDly.Data);
      CadbDly.Bits.start_delay = 4; // Delay at least 4 clocks
      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_DLY_MCCADB_MAIN_REG, &CadbDly.Data);
    }
  }

  return SUCCESS;
} // CpgcQcaClkPattern

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

  for (SubCh = 0 ; SubCh < SUB_CH; SubCh++) {
    GetChannelMapping (Ch, SubCh, &ChannelLocal);
    cpgcMiscODTCtl.Data = MemReadPciCfgEp (Socket, ChannelLocal, CPGC_MISCODTCTL_MCDDC_CTL_REG);
    //
    // For spr, the ddr5_convert_rd_to_mrr bit needs to be 0.
    //
    cpgcMiscODTCtl.Bits.ddr5_convert_rd_to_mrr = 0;
    cpgcMiscODTCtl.Bits.odt_override = 0;
    cpgcMiscODTCtl.Bits.odt_on = 0;
    MemWritePciCfgEp (Socket, ChannelLocal, CPGC_MISCODTCTL_MCDDC_CTL_REG, cpgcMiscODTCtl.Data);
  }
}

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
  UINT32                                        i;
  UINT8                                         SubCh = 0;
  struct channelNvram                           (*ChannelNvList) [MAX_CH];
  PatCadb2Prog0                                 CmdPattern0Local[CADB2_LINES];
  PatCadb2Prog1                                 CmdPattern1Local[CADB2_LINES];
  CADB_CFG_MCCADB_MAIN_STRUCT                   CadbCfg;
  CADB_AO_MRSCFG_MCCADB_MAIN_STRUCT             CadbAoMrscfg;
  UINT32                                        Data32;
  CADB_CTL_MCCADB_MAIN_STRUCT                   CadbCtl;
  UINT32                                        Patbuf[CADB_MUX_MAX] = {SEQUENTIAL_BUFFER_PATTERN_0, 0, 0, 0};
  UINT32                                        MuxCtl[CADB_MUX_MAX] = {BTBUFFER, BTBUFFER, BTBUFFER, BTBUFFER};
  UINT32                                        LfsrSize[CADB_MUX_MAX] = {CPGC_DONT_CARE, CPGC_DONT_CARE, CPGC_DONT_CARE, CPGC_DONT_CARE};
  UINT8                                         LogicalRank;

  LogicalRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);

  ChannelNvList = GetChannelNvList (Host, Socket);
  if ((*ChannelNvList) [Ch].enabled == 0) {
    return;
  }

  SetCadbModeEn (Host, Socket, 1 << Ch, TRUE);

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "WriteCadbPrechargeCmdDdr5 starts\n");

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

    if (((1 << SubCh) & (Subchannelmask)) == 0) {
      continue;
    }

    CadbAoMrscfg.Data = 0;
    CadbAoMrscfg.Bits.mrs_ao_repeats = 1;
    CadbAoMrscfg.Bits.mrs_gap = 15;
    CadbAoMrscfg.Bits.mrs_gap_scale = 0;

    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);

    CadbCfg.Bits.cadb_mode = CADB_MODE_ALWAYS_ON;
    CadbCfg.Bits.initial_dsel_sseq_en = 0;
    CadbCfg.Bits.initial_dsel_en = 1;
    //
    // Drive CADB pattern on all pins including (CS, CA and PAR)
    //
    CadbCfg.Bits.lane_deselect_en = DRIVE_CADB_PATTERN_ON_ALL_PINS;
    if ((*ChannelNvList) [Ch].timingMode == TIMING_1N) {
      CadbCfg.Bits.cadb_dsel_throttle_mode = 1;
      CadbCfg.Bits.cadb_sel_throttle_mode = 1;
    } else {
      CadbCfg.Bits.cadb_dsel_throttle_mode = 2;
      CadbCfg.Bits.cadb_sel_throttle_mode = 2;
    }

    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);
    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_AO_MRSCFG_MCCADB_MAIN_REG, &CadbAoMrscfg.Data);

    // No delay required
    Data32 = 0;
    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_DLY_MCCADB_MAIN_REG, &Data32);

    SetCadbDselUniseqCfg (Host, Socket, Ch, SubCh, MuxCtl, LfsrSize);

    //
    // Program the CADB buffer pattern.
    //
    SetCadbDselUniseqPat (Host, Socket, Ch, SubCh, Patbuf);
  } //subch loop

  for (i = 0; i < CADB_ROW_NUMBER_1UI_CMD; i++) {
    CopyMem (&CmdPattern0Local[i], &BasicCmdPattern0[i], sizeof (PatCadb2Prog0));
    CopyMem (&CmdPattern1Local[i], &BasicPattern1[i], sizeof (PatCadb2Prog0));

    if ((i % 2) == 0) {
      if (Cmd == PREab_CMD) {
        CmdPattern0Local[i].ca = PREabL;
      } else if (Cmd == PREsb_CMD) {
        CmdPattern0Local[i].ca = PREsbL (Bank);
      } else if (Cmd == PREpb_CMD) {
        CmdPattern0Local[i].ca = PREpbL (Bank, BankGroup);
      }
    } else {
      if (Cmd == PREab_CMD) {
        CmdPattern0Local[i].ca = PREabH;
      } else if (Cmd == PREsb_CMD) {
        CmdPattern0Local[i].ca = PREsbH (Bank);
      } else if (Cmd == PREpb_CMD) {
        CmdPattern0Local[i].ca = PREpbH (Bank, BankGroup);
      }
    }

    CmdPattern0Local[i].val = 1;
  }
  FixupCS (LogicalRank, 0, CmdPattern0Local, (*ChannelNvList) [Ch].encodedCSMode);
  ProgramCADB2 (Host, Socket, Ch, Subchannelmask, 0, CADB_ROW_NUMBER_1UI_CMD, CADB_BUF_MODE_DDR5_DESELECT, FALSE, CmdPattern0Local, CmdPattern1Local);

  //
  // Delay if this is a CTE build, otherwise do nothing.
  //
  CteDelayDclk (50);

  //
  // Start test.
  //
  CadbTestStart (Host, Socket, (1 << Ch), Subchannelmask);

  //
  // Poll test completion
  //
  CadbPollTestDone (Host, Socket, 1 << Ch, Subchannelmask);
  //
  // Stop the test
  //
  CadbCtl.Data = 0;
  CadbCtl.Bits.start_test = 0;
  CadbCtl.Bits.stop_test = 1;
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & (Subchannelmask)) == 0) {
      continue;
    }
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CADB (SubCh, CADB_CTL_MCCADB_MAIN_REG), CadbCtl.Data);
  }

  SetCadbModeEn (Host, Socket, 1 << Ch, FALSE);
  //
  // Add the delay to wait for the bit change to take effective.
  //
  TrainingDelay (Host, 5 * 16, 1);

  return;
} // WriteCadbPrechargeCmdDdr5

/**

  Setup CADB to generate Mrr pattern

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket to train
  @param[in] ChannelMask      - Ch Bit mask for which test should be setup for.
  @param[in] LogicalRank      - Logical rank number

  @retval SUCCESS / FAILURE

  **/
UINT32
SetupCadbMrr (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChannelMask,
  IN UINT8     LogicalRank
  )
{
  UINT32                                        Index;
  UINT8                                         Ch;
  UINT8                                         SubCh;
  struct channelNvram                           (*ChannelNvList) [MAX_CH];
  PatCadb2Prog0                                 AddressPattern0Local[CADB2_LINES];
  PatCadb2Prog1                                 AddressPattern1Local[CADB2_LINES];
  CADB_CFG_MCCADB_MAIN_STRUCT                   CadbCfg;
  CADB_MRSCFG_N0_MCCADB_MAIN_STRUCT             CadbMrscfgN0;
  CADB_MRSCFG_N1_MCCADB_MAIN_STRUCT             CadbMrscfgN1;
  CADB_SELCFG_MCCADB_MAIN_STRUCT                CadbSelCfg;
  CADB_DLY_MCCADB_MAIN_STRUCT                   CadbDly;
  MRC_MST                                       MemSsType;
  UINT32                                        MaxCh;
  UINT8                                         NumberLines;
  UINT32                                        Patbuf[CADB_MUX_MAX] = { CADB_BUFFER_PATTERN_10,
                                                                         CADB_BUFFER_PATTERN_11,
                                                                         CADB_BUFFER_PATTERN_12,
                                                                         CADB_BUFFER_PATTERN_13 };

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "SetupCadbMrr\n");

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  MaxCh = GetMaxCh (Host, Socket, MemSsType);

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxCh; Ch++) {
    if ((ChannelMask & (1 << Ch)) == 0) {
      continue;
    }

    for (Index = 0; Index < CADB2_READ_TRAINING_PATTERN_LINES; Index++) {
      if (IsDdrtDimm (Host, Socket, Ch)) {
        CopyMem (&AddressPattern0Local[Index], &GntTrainingPattern0[Index], sizeof (PatCadb2Prog0));
        CopyMem (&AddressPattern1Local[Index], &GntTrainingPattern1[Index], sizeof (PatCadb2Prog1));
      } else {
        CopyMem (&AddressPattern0Local[Index], &ReadTrainingPattern0[Index], sizeof (PatCadb2Prog0));
        CopyMem (&AddressPattern1Local[Index], &ReadTrainingPattern1[Index], sizeof (PatCadb2Prog1));
      }
    }
    NumberLines = CADB2_READ_TRAINING_PATTERN_LINES;

    //
    // Setup CADB buffer
    //
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      //
      // Setup CADB
      //
      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);
      CadbCfg.Bits.cadb_mode = CADB_MODE_ACTIVE_SELECT_DESLECT;
      CadbCfg.Bits.cadb_to_cpgc_bind = 1;
      if ((*ChannelNvList) [Ch].timingMode == TIMING_1N) {
        CadbCfg.Bits.cadb_dsel_throttle_mode = 1;
        CadbCfg.Bits.cadb_sel_throttle_mode = 1;
      } else {
        CadbCfg.Bits.cadb_dsel_throttle_mode = 2;
        CadbCfg.Bits.cadb_sel_throttle_mode = 2;
      }
      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);

      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_DLY_MCCADB_MAIN_REG, &CadbDly.Data);
      CadbDly.Bits.stop_delay = 0xC8;
      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_DLY_MCCADB_MAIN_REG, &CadbDly.Data);

      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_MRSCFG_N1_MCCADB_MAIN_REG, &CadbMrscfgN1.Data);
      CadbMrscfgN1.Bits.mrs_delay_cnt = 0;
      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_MRSCFG_N1_MCCADB_MAIN_REG, &CadbMrscfgN1.Data);

      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_MRSCFG_N0_MCCADB_MAIN_REG, &CadbMrscfgN0.Data);
      CadbMrscfgN0.Bits.mrs_cs_mode = ONE_CYCLE_CS;
      CadbMrscfgN0.Bits.mrs_goto_ptr = 0;
      CadbMrscfgN0.Bits.mrs_end_ptr = 0; //NumberLines;
      CadbMrscfgN0.Bits.mrs_start_ptr = 0;
      CadbMrscfgN0.Bits.cs_active_polarity = 0;
      CadbMrscfgN0.Bits.setup_clocks = 0;
      CadbMrscfgN0.Bits.dsel_clocks = 0;
      CadbMrscfgN0.Bits.cs_clocks = 0;
      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_MRSCFG_N0_MCCADB_MAIN_REG, &CadbMrscfgN0.Data);

      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_SELCFG_MCCADB_MAIN_REG, &CadbSelCfg.Data);
      CadbSelCfg.Bits.rd_select_enable = 4;    // Drive pattern only on CA pins. Don't override CS or parity
      CadbSelCfg.Bits.initial_sel_sseq_en = 1; // Use read select at the beginning of subsequence
      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_SELCFG_MCCADB_MAIN_REG, &CadbSelCfg.Data);

      SetCadbDselUniseqPat (Host, Socket, Ch, SubCh, Patbuf);
    }

    FixupCS (LogicalRank, 0, AddressPattern0Local, (*ChannelNvList) [Ch].encodedCSMode);
    ProgramCADB2 (Host, Socket, Ch, TWO_SUBCH_MASK, 0, NumberLines, CADB_BUF_MODE_DDR5_SELECT, FALSE, AddressPattern0Local, AddressPattern1Local);
  }

  return SUCCESS;
} // SetupCadbMrr

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
