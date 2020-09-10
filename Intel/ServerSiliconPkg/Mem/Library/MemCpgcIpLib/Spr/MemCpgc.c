/** @file
  Memory Initialization Module.

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
#include "Include/MemCpgcTestTable.h"
#include "Include/MemCpgcRegCache.h"
#include <Library/MemMcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
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
#include <Library/AdvMemTestLib.h>
#include <Library/RcMemAllocLib.h>
#include <Memory/JedecDefinitions.h>
#include <Library/MemoryEvContentLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/SpdDecodeLib.h>
#include <Library/PerformanceTrackerLib.h>

extern UINT8  tWL_DDR4[MAX_SUP_FREQ];
extern UINT8 columnBitsDDR4[MAX_TECH];     // Number of column address bits

#define START_INDEX_DPAT_EXT_BUFFER      0
#define STOP_INDEX_DPAT_EXT_BUFFER       15

#define DDRT_CPGC_MAX_CREDIT             56
#define DB_DFE_BLOCK_SIZE_MAX_BANK       3
#define DB_DFE_BASE_COL_REPEATS          256


//
// Array to indicate how to map the LFSR # with the Lane and UI
// The different LFSR selection on each Lane for a given UI
// There are 4 patterns used for advanced backside ddr training
//
#define DDR_CPGC_NUM_PATTERNS       4
STATIC UINT32 CpgcAdvCmdPatternSetup (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 Dimm,UINT8 rank, UINT8 dqLoopCount);
STATIC UINT32 CpgcAdvDqSetup (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 rank, UINT8 subRank, UINT32 dqLoopCount);
STATIC INT16  GetMinDwl (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 dimm);
STATIC UINT32 CpgcFillWdbDqVa (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 mode);

STATIC VOID
FixupAddrBits (
  PSYSHOST      Host,
  GSM_CSN       sig,
  PatCadbProg0  prog0[CADB_LINES],
  PatCadbProg1  prog1[CADB_LINES]
  );

/**

  Fix up the address bit field for a backside address for programming the CADB

  @param[in]      sig     Signal to test
  @param[in, out] prog0   Data to program
  @param[in, out] prog1   Data to program

**/
STATIC VOID
FixupBacksideAddrBits (
  IN      GSM_CSN       Sig,
  IN OUT  PatCadbProg0  Prog0[CADB_LINES],
  IN OUT  PatCadbProg1  Prog1[CADB_LINES]
  );

/**

  Fix up the Chip Select field for backside CS for programming the CADB

  @param[in]        Cs     Rank chip select
  @param[in, out]   Prog1  Data to program

**/
STATIC VOID
FixupBacksideCS (
  IN      UINT8         Cs,
  IN OUT  PatCadbProg1  Prog1[CADB_LINES]
  );

STATIC UINT32
GetPmonCounter (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch
  );

STATIC UINT8
IsAepRank (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    logRank
  );

STATIC VOID
ProgCADBRow (
  PSYSHOST      Host,
  UINT8         socket,
  UINT8         ch,
  UINT8         dimm,
  UINT8         rank,
  UINT8         bank,
  UINT32        data,
  UINT8         side,
  UINT8         pdaTag,
  UINT8         control,
  PatCadbProg0  *patCADBProg0,
  PatCadbProg1  *patCADBProg1
  );

STATIC VOID
SetupPmon (
  PSYSHOST        Host,
  UINT8           socket,
  UINT8           ch,
  UINT8           eventSelect,
  UINT8           unitMask
  );

typedef union {
  struct {
    UINT32  Cs          : 4;  ///< Bits  0:3
    UINT32  Rsvd_4      : 3;  ///< Bits  4:6
    UINT32  Cs_nt       : 4;  ///< Bits  7:10
    UINT32  Rsvd_11     : 5;  ///< Bits  11:15
    UINT32  Val         : 1;  ///< Bit   16
    UINT32  Rsvd_17     : 1;  ///< Bits  17
    UINT32  Ca_1st      : 14; ///< Bits  18:31
  } Ddr5Sel;
  struct {
    UINT32  Cs          : 4;   ///< Bits  0:3
    UINT32  Rsvd_4      : 12;  ///< Bits  4:15
    UINT32  Val         : 1;   ///< Bits  16
    UINT32  Rsvd_17     : 1;   ///< Bits  17
    UINT32  Ca          : 14;  ///< Bits  18:31
  } Ddr5Desel;
  struct {
    UINT32  Cs          : 4;   ///< Bits  0:3
    UINT32  Cid         : 3;   ///< Bits  4:6
    UINT32  Odt         : 4;   ///< Bits  7:10
    UINT32  Cke         : 4;   ///< Bits  11:14
    UINT32  Rsvd_15     : 1;   ///< Bits  15
    UINT32  Val         : 1;   ///< Bits  16
    UINT32  Par         : 1;   ///< Bits  17
    UINT32  Ma          : 14;  ///< Bits  18:31
  } Ddr4;
  UINT32  Data;
} CADB_BUF_0_N0_MCCADB_MAIN_STRUCT_LOCAL;

typedef union {
  struct {
    UINT32  Par_r_1st   : 1;  ///< Bit  0:0
    UINT32  Par_f_1st   : 1;  ///< Bit  1:1
    UINT32  Ca_2nd      : 14; ///< Bits 2:15
    UINT32  Par_r_2nd   : 1;  ///< Bit  16:16
    UINT32  Par_f_2nd   : 1;  ///< Bit  17:17
    UINT32  Rsvd_18     : 14; ///< Bits 18:31
  } Ddr5Sel;
  struct {
    UINT32  Par_r       : 1;  ///< Bit  0:0
    UINT32  Par_f       : 1;  ///< Bit  1:1
    UINT32  Rsvd_2      : 30; ///< Bits 2:31
  } Ddr5Desel;
  struct {
    UINT32  Control     : 3;  ///< Bit  0:2 (Bit[2]: MA[16]/RAS_n, Bit[1]: MA[15]/CAS_n, Bit[0]: MA[14]/WE_n)
    UINT32  Rsvd_3      : 1;  ///< Bits 3
    UINT32  Bank        : 4;  ///< Bit  4:7
    UINT32  Act_n       : 1;  ///< Bits 8
    UINT32  Rsvd_9      : 23; ///< Bits 9:31
  } Ddr4;
  UINT32  Data;
} CADB_BUF_0_N1_MCCADB_MAIN_STRUCT_LOCAL;


STATIC PatCadbProg0 CtlClkPattern0[CADB_LINES] = {
// addr, cid, bank, pdatag
  {0x0,   0x0, 0x0, 0x0}, // Row 0
  {0x0,   0x0, 0x0, 0x0}, // Row 1
  {0x0,   0x0, 0x0, 0x0}, // Row 2
  {0x0,   0x0, 0x0, 0x0}, // Row 3
  {0x0,   0x0, 0x0, 0x0}, // Row 4
  {0x0,   0x0, 0x0, 0x0}, // Row 5
  {0x0,   0x0, 0x0, 0x0}, // Row 6
  {0x0,   0x0, 0x0, 0x0}, // Row 7
  {0x0,   0x0, 0x0, 0x0}, // Row 8
  {0x0,   0x0, 0x0, 0x0}, // Row 9
  {0x0,   0x0, 0x0, 0x0}, // Row 10
  {0x0,   0x0, 0x0, 0x0}, // Row 11
  {0x0,   0x0, 0x0, 0x0}, // Row 12
  {0x0,   0x0, 0x0, 0x0}, // Row 13
  {0x0,   0x0, 0x0, 0x0}, // Row 14
  {0x0,   0x0, 0x0, 0x0}, // Row 15
};

STATIC PatCadbProg1 CtlClkPattern1[CADB_LINES] = {
// CS, Control, ODT,  CKE, Par
  {0x3FF, 0, 0x3F, 0x3F, 0}, // Row 0
  {0x0,   0,  0x0,  0x0, 0}, // Row 1
  {0x3FF, 0, 0x3F, 0x3F, 0}, // Row 2
  {0x0,   0,  0x0,  0x0, 0}, // Row 3
  {0x3FF, 0, 0x3F, 0x3F, 0}, // Row 4
  {0x0,   0,  0x0,  0x0, 0}, // Row 5
  {0x3FF, 0, 0x3F, 0x3F, 0}, // Row 6
  {0x0,   0,  0x0,  0x0, 0}, // Row 7
  {0x3FF, 0, 0x3F, 0x3F, 0}, // Row 8
  {0x0,   0,  0x0,  0x0, 0}, // Row 9
  {0x3FF, 0, 0x3F, 0x3F, 0}, // Row 10
  {0x0,   0,  0x0,  0x0, 0}, // Row 11
  {0x3FF, 0, 0x3F, 0x3F, 0}, // Row 12
  {0x0,   0,  0x0,  0x0, 0}, // Row 13
  {0x3FF, 0, 0x3F, 0x3F, 0}, // Row 14
  {0x0,   0,  0x0,  0x0, 0}, // Row 15
};

UINT32 OverrideCadbDcaDutyCycle[CADB2_LINES] = {
  0x0001FFFE,
  0x5555FFFE,
  0xAAA9FFFE,
  0xFFFDFFFE,
  0x0001FFFF,
  0x5555FFFF,
  0xAAA9FFFF,
  0xFFFDFFFF,
  0x0001FFFE,
  0x5555FFFE,
  0xAAA9FFFE,
  0xFFFDFFFE,
  0x0001FFFF,
  0x5555FFFF,
  0xAAA9FFFF,
  0xFFFDFFFF
};

UINT32 OverrideCadbDcaDutyCycle32_64[CADB2_LINES] = {
  0xFFFFFFFF,
  0xFFFFFFFD,
  0xFFFFFFFE,
  0xFFFFFFFC,
  0xFFFFFFFF,
  0xFFFFFFFD,
  0xFFFFFFFE,
  0xFFFFFFFC,
  0xFFFFFFFF,
  0xFFFFFFFD,
  0xFFFFFFFE,
  0xFFFFFFFC,
  0xFFFFFFFF,
  0xFFFFFFFD,
  0xFFFFFFFE,
  0xFFFFFFFC
};
                                                // addr, cid, bank, pdatag
STATIC PatCadbProg0 ParityPattern0[CADB_LINES] = {{0x0,   0x0, 0x1, 0x0}, // Row 0
                                                  {0x0,   0x0, 0x1, 0x0}, // Row 1
                                                  {0x0,   0x0, 0x1, 0x0}, // Row 2
                                                  {0x0,   0x0, 0x1, 0x0}, // Row 3
                                                  {0x0,   0x0, 0x1, 0x0}, // Row 4
                                                  {0x0,   0x0, 0x1, 0x0}, // Row 5
                                                  {0x0,   0x0, 0x1, 0x0}, // Row 6
                                                  {0x0,   0x0, 0x1, 0x0}, // Row 7
                                                  {0x0,   0x0, 0x0, 0x0}, // Row 8
                                                  {0x0,   0x0, 0x0, 0x0}, // Row 9
                                                  {0x0,   0x0, 0x0, 0x0}, // Row 10
                                                  {0x0,   0x0, 0x0, 0x0}, // Row 11
                                                  {0x0,   0x0, 0x0, 0x0}, // Row 12
                                                  {0x0,   0x0, 0x0, 0x0}, // Row 13
                                                  {0x0,   0x0, 0x0, 0x0}, // Row 14
                                                  {0x0,   0x0, 0x0, 0x0}, // Row 15
                                                 };

                                                  // CS  , Control, ODT,  CKE, Par
STATIC PatCadbProg1 ParityPattern1[CADB_LINES] = {{0x3FF,     0xF, 0x0, 0x3F, 0}, // Row 0
                                                  {0x3FF,     0xF, 0x0, 0x3F, 0}, // Row 1
                                                  {0x3FF,     0xF, 0x0, 0x3F, 0}, // Row 2
                                                  {0x3FF,     0xF, 0x0, 0x3F, 0}, // Row 3
                                                  {0x3FE,     0xF, 0x0, 0x3F, 1}, // Row 4*
                                                  {0x3FF,     0xF, 0x0, 0x3F, 0}, // Row 5
                                                  {0x3FF,     0xF, 0x0, 0x3F, 0}, // Row 6
                                                  {0x3FF,     0xF, 0x0, 0x3F, 0}, // Row 7
                                                  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 8
                                                  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 9
                                                  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 10
                                                  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 11
                                                  {0x3FE,     0xF, 0x0, 0x3F, 0}, // Row 12*
                                                  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 13
                                                  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 14
                                                  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 15
                                                 };

STATIC PatCadbProg0 BacksideParityPattern0[CADB_LINES] = {
  // addr,  cid, bank, pdatag
  {0x0, 0x0, 0x8, 0x0}, // Row 0
  {0x0, 0x0, 0x8, 0x0}, // Row 1
  {0x0, 0x0, 0x8, 0x0}, // Row 2
  {0x0, 0x0, 0x8, 0x0}, // Row 3
  {0x0, 0x0, 0x8, 0x0}, // Row 4
  {0x0, 0x0, 0x8, 0x0}, // Row 5
  {0x0, 0x0, 0x8, 0x0}, // Row 6
  {0x0, 0x0, 0x8, 0x0}, // Row 7
  {0x0, 0x0, 0x0, 0x0}, // Row 8
  {0x0, 0x0, 0x0, 0x0}, // Row 9
  {0x0, 0x0, 0x0, 0x0}, // Row 10
  {0x0, 0x0, 0x0, 0x0}, // Row 11
  {0x0, 0x0, 0x0, 0x0}, // Row 12
  {0x0, 0x0, 0x0, 0x0}, // Row 13
  {0x0, 0x0, 0x0, 0x0}, // Row 14
  {0x0, 0x0, 0x0, 0x0}, // Row 15
};

STATIC PatCadbProg1 BacksideParityPattern1[CADB_LINES] = {
  // CS, Control, ODT,  CKE, Par
  {0x3FF, 0xF, 0x0, 0x3F, 0}, // Row 0
  {0x3FD, 0xF, 0x0, 0x3F, 0}, // Row 1
  {0x3FE, 0xF, 0x0, 0x3F, 1}, // Row 2*
  {0x3FD, 0xF, 0x0, 0x3F, 0}, // Row 3
  {0x3FF, 0xF, 0x0, 0x3F, 0}, // Row 4
  {0x3FF, 0xF, 0x0, 0x3F, 0}, // Row 5
  {0x3FF, 0xF, 0x0, 0x3F, 0}, // Row 6
  {0x3FF, 0xF, 0x0, 0x3F, 0}, // Row 7
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 8
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 9
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 10
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 11
  {0x3FD, 0xF, 0x0, 0x3F, 1}, // Row 12
  {0x3FE, 0xF, 0x0, 0x3F, 0}, // Row 13*
  {0x3FD, 0xF, 0x0, 0x3F, 1}, // Row 14
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 15
};

STATIC PatCadbProg0 BacksideActivatePattern0[CADB_LINES] = {
  // addr,  cid, bank, pdatag
  {0x0, 0x0, 0x0, 0x0}, // Row 0
  {0x0, 0x0, 0x0, 0x0}, // Row 1
  {BIT14 | BIT15 | BIT16, 0x0, 0x0, 0x0}, // Row 2*
  {0x0, 0x0, 0x0, 0x0}, // Row 3
  {0x0, 0x0, 0x0, 0x0}, // Row 4
  {0x0, 0x0, 0x0, 0x0}, // Row 5
  {0x0, 0x0, 0x0, 0x0}, // Row 6
  {0x0, 0x0, 0x0, 0x0}, // Row 7
  {0x0, 0x0, 0x0, 0x0}, // Row 8
  {0x0, 0x0, 0x0, 0x0}, // Row 9
  {0x0, 0x0, 0x0, 0x0}, // Row 10
  {0x0, 0x0, 0x0, 0x0}, // Row 11
  {0x0, 0x0, 0x0, 0x0}, // Row 12
  {0x0, 0x0, 0x0, 0x0}, // Row 13*
  {0x0, 0x0, 0x0, 0x0}, // Row 14
  {0x0, 0x0, 0x0, 0x0}, // Row 15
};

STATIC PatCadbProg1 BacksideActivatePattern1[CADB_LINES] = {
  // CS  , Control, ODT,  CKE, Par
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 0
  {0x3FD, 0xF, 0x0, 0x3F, 1}, // Row 1
  {0x3FE, 0x3, 0x0, 0x3F, 1}, // Row 2*
  {0x3FD, 0xF, 0x0, 0x3F, 1}, // Row 3
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 4
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 5
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 6
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 7
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 8
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 9
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 10
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 11
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 12
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 13*
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 14
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 15
};

STATIC PatCadbProg0 BacksideCommandPattern0[CADB_LINES] = {
  // addr,  cid, bank, pdatag
  {0x0, 0x0, 0x8, 0x0}, // Row 0
  {0x0, 0x0, 0x8, 0x0}, // Row 1
  {BIT14, 0x0, 0x8, 0x0}, // Row 2*
  {0x0, 0x0, 0x8, 0x0}, // Row 3
  {0x0, 0x0, 0x8, 0x0}, // Row 4
  {0x0, 0x0, 0x8, 0x0}, // Row 5
  {0x0, 0x0, 0x8, 0x0}, // Row 6
  {0x0, 0x0, 0x8, 0x0}, // Row 7
  {0x0, 0x0, 0x0, 0x0}, // Row 8
  {0x0, 0x0, 0x0, 0x0}, // Row 9
  {0x0, 0x0, 0x0, 0x0}, // Row 10
  {0x0, 0x0, 0x0, 0x0}, // Row 11
  {0x0, 0x0, 0x0, 0x0}, // Row 12
  {BIT15 | BIT16, 0x0, 0x0, 0x0}, // Row 13*
  {0x0, 0x0, 0x0, 0x0}, // Row 14
  {0x0, 0x0, 0x0, 0x0}, // Row 15
};

STATIC PatCadbProg1 BacksideCommandPattern1[CADB_LINES] = {
  // CS  , Control, ODT,  CKE, Par
  {0x3FF, 0x0, 0x0, 0x3F, 1}, // Row 0
  {0x3FD, 0x0, 0x0, 0x3F, 1}, // Row 1
  {0x3FE, 0x3, 0x0, 0x3F, 0}, // Row 2*
  {0x3FD, 0x0, 0x0, 0x3F, 1}, // Row 3
  {0x3FF, 0x0, 0x0, 0x3F, 1}, // Row 4
  {0x3FF, 0x0, 0x0, 0x3F, 1}, // Row 5
  {0x3FF, 0x0, 0x0, 0x3F, 1}, // Row 6
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 7
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 8
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 9
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 10
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 11
  {0x3FD, 0xF, 0x0, 0x3F, 1}, // Row 12
  {0x3FE, 0x3, 0x0, 0x3F, 0}, // Row 13*
  {0x3FD, 0xF, 0x0, 0x3F, 1}, // Row 14
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 15
};

STATIC PatCadbProg0 BacksideAddressPattern0[CADB_LINES] = {
  // addr,  cid, bank, pdatag
  {0x0, 0x0, 0x0, 0x0}, // Row 0
  {0x0, 0x0, 0x0, 0x0}, // Row 1
  {0x1, 0x0, 0x0, 0x0}, // Row 2*
  {0x0, 0x0, 0x0, 0x0}, // Row 3
  {0x0, 0x0, 0x0, 0x0}, // Row 4
  {0x0, 0x0, 0x0, 0x0}, // Row 5
  {0x0, 0x0, 0x0, 0x0}, // Row 6
  {0x0, 0x0, 0x0, 0x0}, // Row 7
  {0x1, 0x0, 0x0, 0x0}, // Row 8
  {0x1, 0x0, 0x0, 0x0}, // Row 9
  {0x1, 0x0, 0x0, 0x0}, // Row 10
  {0x1, 0x0, 0x0, 0x0}, // Row 11
  {0x1, 0x0, 0x0, 0x0}, // Row 12
  {0x0, 0x0, 0x0, 0x0}, // Row 13*
  {0x1, 0x0, 0x0, 0x0}, // Row 14
  {0x1, 0x0, 0x0, 0x0}, // Row 15
};

STATIC PatCadbProg1 BacksideAddressPattern1[CADB_LINES] = {
  // CS, Control, ODT,  CKE, Par
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 0
  {0x3FD, 0xF, 0x0, 0x3F, 1}, // Row 1
  {0x3FE, 0xF, 0x0, 0x3F, 1}, // Row 2*
  {0x3FD, 0xF, 0x0, 0x3F, 1}, // Row 3
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 4
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 5
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 6
  {0x3FF, 0xF, 0x0, 0x3F, 1}, // Row 7
  {0x3FF, 0xF, 0x0, 0x3F, 0}, // Row 8
  {0x3FF, 0xF, 0x0, 0x3F, 0}, // Row 9
  {0x3FF, 0xF, 0x0, 0x3F, 0}, // Row 10
  {0x3FF, 0xF, 0x0, 0x3F, 0}, // Row 11
  {0x3FD, 0xF, 0x0, 0x3F, 0}, // Row 12
  {0x3FE, 0xF, 0x0, 0x3F, 0}, // Row 13*
  {0x3FD, 0xF, 0x0, 0x3F, 0}, // Row 14
  {0x3FF, 0xF, 0x0, 0x3F, 0}, // Row 15
};

                                            // addr, cid, bank, pdatag
STATIC PatCadbProg0 ActivatePattern0[CADB_LINES] = {{BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 0
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 1
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 2
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 3
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 4*
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 5
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 6
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 7
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 8
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 9
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 10
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 11
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 12
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 13
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 14
                                             {BIT14 | BIT15,   0x0, 0x0, 0x0}, // Row 15
                                            };

                                            // CS  , Control, ODT,  CKE, Par
STATIC PatCadbProg1 ActivatePattern1[CADB_LINES] = {{0x3FF,     0xB, 0x0, 0x3F, 0}, // Row 0
                                             {0x3FF,     0xB, 0x0, 0x3F, 0}, // Row 1
                                             {0x3FF,     0xB, 0x0, 0x3F, 0}, // Row 2
                                             {0x3FF,     0xB, 0x0, 0x3F, 0}, // Row 3
                                             {0x3FE,     0x3, 0x0, 0x3F, 0}, // Row 4*
                                             {0x3FF,     0xB, 0x0, 0x3F, 0}, // Row 5
                                             {0x3FF,     0xB, 0x0, 0x3F, 0}, // Row 6
                                             {0x3FF,     0xB, 0x0, 0x3F, 0}, // Row 7
                                             {0x3FF,     0xB, 0x0, 0x3F, 0}, // Row 8
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 9
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 10
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 11
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 12
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 13
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 14
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 15
                                            };

                                            // addr, cid, bank, pdatag
STATIC PatCadbProg0 PrechargePattern0[CADB_LINES] = {{0x400,   0x0, 0x0, 0x0}, // Row 0
                                             {0x400,   0x0, 0x0, 0x0}, // Row 1
                                             {0x400,   0x0, 0x0, 0x0}, // Row 2
                                             {0x400,   0x0, 0x0, 0x0}, // Row 3
                                             {0x400,   0x0, 0x0, 0x0}, // Row 4
                                             {0x400,   0x0, 0x0, 0x0}, // Row 5
                                             {0x400,   0x0, 0x0, 0x0}, // Row 6
                                             {0x400,   0x0, 0x0, 0x0}, // Row 7
                                             {0x400,   0x0, 0x0, 0x0}, // Row 8
                                             {0x0,   0x0, 0x0, 0x0}, // Row 9
                                             {0x0,   0x0, 0x0, 0x0}, // Row 10
                                             {0x0,   0x0, 0x0, 0x0}, // Row 11
                                             {0x0,   0x0, 0x0, 0x0}, // Row 12
                                             {0x0,   0x0, 0x0, 0x0}, // Row 13
                                             {0x0,   0x0, 0x0, 0x0}, // Row 14
                                             {0x0,   0x0, 0x0, 0x0}, // Row 15
                                            };

                                            // CS  , Control, ODT,  CKE, Par
STATIC PatCadbProg1 PrechargePattern1[CADB_LINES] = {{0x3FF,     0xA, 0x0, 0x3F, 1}, // Row 0
                                             {0x3FF,     0xA, 0x0, 0x3F, 1}, // Row 1
                                             {0x3FF,     0xA, 0x0, 0x3F, 1}, // Row 2
                                             {0x3FF,     0xA, 0x0, 0x3F, 1}, // Row 3
                                             {0x3FE,     0xA, 0x0, 0x3F, 1}, // Row 4*
                                             {0x3FF,     0xA, 0x0, 0x3F, 1}, // Row 5
                                             {0x3FF,     0xA, 0x0, 0x3F, 1}, // Row 6
                                             {0x3FF,     0xA, 0x0, 0x3F, 1}, // Row 7
                                             {0x3FF,     0xA, 0x0, 0x3F, 1}, // Row 8
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 9
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 10
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 11
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 12
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 13
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 14
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 15
                                            };

                                        // addr, cid, bank, pdatag
STATIC PatCadbProg0 FnvGoodParity0[CADB_LINES] = {{0x0,   0x0, 0x0, 0x0}, // Row 0
                                          {0x0,   0x0, 0x0, 0x0}, // Row 1
                                          {0x0,   0x0, 0x0, 0x0}, // Row 2
                                          {0x0,   0x0, 0x7, 0x0}, // Row 3*
                                          {0x0,   0x0, 0x7, 0x0}, // Row 4*
                                          {0x0,   0x0, 0x7, 0x0}, // Row 5*
                                          {0x0,   0x0, 0x0, 0x0}, // Row 6
                                          {0x0,   0x0, 0x0, 0x0}, // Row 7
                                          {0x0,   0x0, 0x0, 0x0}, // Row 8
                                          {0x0,   0x0, 0x0, 0x0}, // Row 9
                                          {0x0,   0x0, 0x0, 0x0}, // Row 10
                                          {0x0,   0x0, 0x0, 0x0}, // Row 11
                                          {0x0,   0x0, 0x0, 0x0}, // Row 12
                                          {0x0,   0x0, 0x0, 0x0}, // Row 13
                                          {0x0,   0x0, 0x0, 0x0}, // Row 14
                                          {0x0,   0x0, 0x0, 0x0}, // Row 15
                                        };

                                          // CS  , Control, ODT,  CKE, Par
STATIC PatCadbProg1 FnvGoodParity1[CADB_LINES] = {{0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 0
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 1
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 2
                                          {0x3FF,     0x8, 0x0, 0x3F, 0}, // Row 3*
                                          {0x3FE,     0x8, 0x0, 0x3F, 0}, // Row 4*
                                          {0x3FF,     0x8, 0x0, 0x3F, 0}, // Row 5*
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 6
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 7
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 8
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 9
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 10
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 11
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 12
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 13
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 14
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 15
                                        };

                                            // CS  , Control, ODT,  CKE, Par
STATIC PatCadbProg1 DeselectPattern1[CADB_LINES] = {{0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 0
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 1
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 2
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 3
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 4
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 5
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 6
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 7
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 8
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 9
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 10
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 11
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 12
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 13
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 14
                                             {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 15
                                            };


  // CS  , Control, ODT,  CKE, Par
STATIC PatCadbProg1 GntPattern1[CADB_LINES] = {
  {0x3FE,     0xF, 0x0, 0x3F, 1}, // Row 0  //GNT
  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 1
  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 2
  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 3
  {0x3FE,     0xF, 0x0, 0x3F, 1}, // Row 4  // GNT
  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 5
  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 6
  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 7
  {0x3FE,     0xF, 0x0, 0x3F, 1}, // Row 8  // GNT
  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 9
  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 10
  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 11
  {0x3FE,     0xF, 0x0, 0x3F, 1}, // Row 12  //GNT
  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 13
  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 14
  {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 15
};
                                         // addr, cid, bank, pdatag
STATIC PatCadbProg0 AddressPattern0[CADB_LINES] = {{0x0,   0x0, 0x0, 0x0}, // Row 0
                                          {0x0,   0x0, 0x0, 0x0}, // Row 1
                                          {0x0,   0x0, 0x0, 0x0}, // Row 2
                                          {0x0,   0x0, 0x0, 0x0}, // Row 3
                                          {0x0,   0x0, 0x8, 0x0}, // Row 4
                                          {0x0,   0x0, 0x0, 0x0}, // Row 5
                                          {0x0,   0x0, 0x0, 0x0}, // Row 6
                                          {0x0,   0x0, 0x0, 0x0}, // Row 7
                                          {0x0,   0x0, 0x0, 0x0}, // Row 8
                                          {0x0,   0x0, 0x8, 0x0}, // Row 9
                                          {0x0,   0x0, 0x8, 0x0}, // Row 10
                                          {0x0,   0x0, 0x8, 0x0}, // Row 11
                                          {0x0,   0x0, 0x0, 0x0}, // Row 12
                                          {0x0,   0x0, 0x8, 0x0}, // Row 13
                                          {0x0,   0x0, 0x8, 0x0}, // Row 14
                                          {0x0,   0x0, 0x8, 0x0}, // Row 15
                                         };

                                         // CS  , Control, ODT,  CKE, Par
STATIC PatCadbProg1 AddressPattern1[CADB_LINES] = {{0x3FF,     0x3, 0x0, 0x3F, 1}, // Row 0
                                          {0x3FF,     0x3, 0x0, 0x3F, 1}, // Row 1
                                          {0x3FF,     0x3, 0x0, 0x3F, 1}, // Row 2
                                          {0x3FF,     0x3, 0x0, 0x3F, 1}, // Row 3
                                          {0x3FE,     0x3, 0x0, 0x3F, 1}, // Row 4
                                          {0x3FF,     0x3, 0x0, 0x3F, 1}, // Row 5
                                          {0x3FF,     0x3, 0x0, 0x3F, 1}, // Row 6
                                          {0x3FF,     0x3, 0x0, 0x3F, 1}, // Row 7
                                          {0x3FF,     0x3, 0x0, 0x3F, 1}, // Row 8
                                          {0x3FF,     0x3, 0x0, 0x3F, 0}, // Row 9
                                          {0x3FF,     0x3, 0x0, 0x3F, 0}, // Row 10
                                          {0x3FF,     0x3, 0x0, 0x3F, 0}, // Row 11
                                          {0x3FE,     0x3, 0x0, 0x3F, 0}, // Row 12
                                          {0x3FF,     0x3, 0x0, 0x3F, 0}, // Row 13
                                          {0x3FF,     0x3, 0x0, 0x3F, 0}, // Row 14
                                          {0x3FF,     0x3, 0x0, 0x3F, 0}, // Row 15
                                         };

                                         // addr, cid, bank, pdatag
STATIC PatCadbProg0 AdvCmdPattern0[CADB_LINES] = {{0x00000,   0x0, 0x0, 0x0}, // Row 0
                                          {0x22222,   0x0, 0x08, 0x0}, // Row 1
                                          {0x11111,   0x2, 0x04, 0x0}, // Row 2
                                          {0x33333,   0x2, 0x0C, 0x0}, // Row 3
                                          {0x08888,   0x1, 0x02, 0x0}, // Row 4
                                          {0x2AAAA,   0x1, 0x0A, 0x0}, // Row 5
                                          {0x19999,   0x3, 0x06, 0x0}, // Row 6
                                          {0x3BBBB,   0x3, 0x0E, 0x0}, // Row 7
                                          {0x04444,   0x0, 0x01, 0x0}, // Row 8
                                          {0x26666,   0x0, 0x09, 0x0}, // Row 9
                                          {0x15555,   0x2, 0x05, 0x0}, // Row 10
                                          {0x37777,   0x2, 0x0D, 0x0}, // Row 11
                                          {0x0CCCC,   0x1, 0x03, 0x0}, // Row 12
                                          {0x2EEEE,   0x1, 0x0B, 0x0}, // Row 13
                                          {0x1DDDD,   0x3, 0x07, 0x0}, // Row 14
                                          {0x3FFFF,   0x3, 0x0F, 0x0}, // Row 15
                                         };

                                         // CS  , Control, ODT,  CKE, Par
STATIC PatCadbProg1 AdvCmdPattern1[CADB_LINES] = {{0x0,     0x00, 0x0, 0x0, 0x0}, // Row 0
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 1
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 2
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 3
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 4
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 5
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 6
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 7
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 8
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 9
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 10
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 11
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 12
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 13
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 14
                                          {0x0,     0x03, 0x0, 0x0, 0x0}, // Row 15
                                         };

                                         // addr, cid, bank, pdatag
STATIC PatCadbProg0 AggAdvCmdPattern0[CADB_LINES] = {{0x00000,   0x0, 0x0, 0x0}, // Row 0
                                          {0x02040,   0x0, 0x00, 0x0}, // Row 1
                                          {0x3DFBF,   0x7, 0x0F, 0x0}, // Row 2
                                          {0x3FFFF,   0x7, 0x0F, 0x0}, // Row 3
                                          {0x00000,   0x0, 0x00, 0x0}, // Row 4
                                          {0x02040,   0x0, 0x00, 0x0}, // Row 5
                                          {0x3DFBF,   0x7, 0x0F, 0x0}, // Row 6
                                          {0x3FFFF,   0x7, 0x0F, 0x0}, // Row 7
                                          {0x00000,   0x0, 0x00, 0x0}, // Row 8
                                          {0x02040,   0x0, 0x00, 0x0}, // Row 9
                                          {0x3DFBF,   0x7, 0x0F, 0x0}, // Row 10
                                          {0x3FFFF,   0x7, 0x0F, 0x0}, // Row 11
                                          {0x00000,   0x0, 0x00, 0x0}, // Row 12
                                          {0x02040,   0x0, 0x00, 0x0}, // Row 13
                                          {0x3DFBF,   0x7, 0x0F, 0x0}, // Row 14
                                          {0x3FFFF,   0x7, 0x0F, 0x0}, // Row 15
                                         };

                                         // CS  , Control, ODT,  CKE, Par
STATIC PatCadbProg1 AggAdvCmdPattern1[CADB_LINES] = {{0x0,     0x00, 0x0, 0x0, 0x0}, // Row 0
                                          {0x0,     0x08, 0x0, 0x0, 0x0}, // Row 1
                                          {0x0,     0x07, 0x0, 0x0, 0x0}, // Row 2
                                          {0x0,     0x0F, 0x0, 0x0, 0x0}, // Row 3
                                          {0x0,     0x00, 0x0, 0x0, 0x0}, // Row 4
                                          {0x0,     0x08, 0x0, 0x0, 0x0}, // Row 5
                                          {0x0,     0x07, 0x0, 0x0, 0x0}, // Row 6
                                          {0x0,     0x0F, 0x0, 0x0, 0x0}, // Row 7
                                          {0x0,     0x00, 0x0, 0x0, 0x0}, // Row 8
                                          {0x0,     0x08, 0x0, 0x0, 0x0}, // Row 9
                                          {0x0,     0x07, 0x0, 0x0, 0x0}, // Row 10
                                          {0x0,     0x0F, 0x0, 0x0, 0x0}, // Row 11
                                          {0x0,     0x00, 0x0, 0x0, 0x0}, // Row 12
                                          {0x0,     0x08, 0x0, 0x0, 0x0}, // Row 13
                                          {0x0,     0x07, 0x0, 0x0, 0x0}, // Row 14
                                          {0x0,     0x0F, 0x0, 0x0, 0x0}, // Row 15
                                         };

STATIC PatCadbProg0 ReadCommandsMRSMode0[CADB_LINES] = {{0x4000,   0x0, 0x0, 0x0}, // Row 0
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 1
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 2
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 3
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 4
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 5
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 6
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 7
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 8
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 9
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 10
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 11
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 12
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 13
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 14
                                                {0x4000,   0x0, 0x0, 0x0}, // Row 15
                                               };

                                         // CS  , Control, ODT,  CKE, Par
STATIC PatCadbProg1 ReadCommandsMRSMode1[CADB_LINES] = {{0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 0
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 1
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 2
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 3
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 4*
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 5
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 6
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 7
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 8
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 9
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 10
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 11
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 12*
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 13
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 14
                                                {0x3FE,     0x1, 0x0, 0x3F, 1}, // Row 15
                                               };

// wdb chunks 0-3, 4-7
/*
const UINT32 CpgcWdbDqVaPat [8] =     { 0xfffe0100,           // wdb 0 for dq/va
                                        0xfffd0200,           // wdb 1 for dq/va
                                        0xfffb0400,           // wdb 2 for dq/va
                                        0xfff70800,           // wdb 3 for dq/va
                                        0xffef1000,           // wdb 4 for dq/va
                                        0xffdf2000,           // wdb 5 for dq/va
                                        0xffbf4000,           // wdb 6 for dq/va
                                        0xff7f8000,           // wdb 7 for dq/va
                                      };
 */
STATIC const UINT8 CpgcWdbDqVaPat [8][8] =  {
                                        {0, 0x01, 0xFE, 0xFF, 0, 0x01, 0xFE, 0xFF}, //0xfffe0100,           // wdb 0 for dq/va
                                        {0, 0x02, 0xFD, 0xFF, 0, 0x02, 0xFD, 0xFF}, //0xfffd0200,           // wdb 1 for dq/va
                                        {0, 0x04, 0xFB, 0xFF, 0, 0x04, 0xFB, 0xFF}, //0xfffb0400,           // wdb 2 for dq/va
                                        {0, 0x08, 0xF7, 0xFF, 0, 0x08, 0xF7, 0xFF}, //0xfff70800,           // wdb 3 for dq/va
                                        {0, 0x10, 0xEF, 0xFF, 0, 0x10, 0xEF, 0xFF}, //0xffef1000,           // wdb 4 for dq/va
                                        {0, 0x20, 0xDF, 0xFF, 0, 0x20, 0xDF, 0xFF}, //0xffdf2000,           // wdb 5 for dq/va
                                        {0, 0x40, 0xBF, 0xFF, 0, 0x40, 0xBF, 0xFF}, //0xffbf4000,           // wdb 6 for dq/va
                                        {0, 0x80, 0x7F, 0xFF, 0, 0x80, 0x7F, 0xFF}  //0xff7f8000,           // wdb 7 for dq/va
                                      };

STATIC const UINT8 CpgcWdbDqVaPat2 [8][8] =  {
                                        {0, 0xEE, 0x11, 0xFF, 0, 0xEE, 0x11, 0xFF}, //          // wdb 0 for dq/va
                                        {0, 0xEE, 0x11, 0xFF, 0, 0xEE, 0x11, 0xFF}, //          // wdb 1 for dq/va
                                        {0, 0xEE, 0x11, 0xFF, 0, 0xEE, 0x11, 0xFF}, //          // wdb 2 for dq/va
                                        {0, 0xEE, 0x11, 0xFF, 0, 0xEE, 0x11, 0xFF}, //          // wdb 3 for dq/va
                                        {0, 0xEE, 0x11, 0xFF, 0, 0xEE, 0x11, 0xFF}, //          // wdb 4 for dq/va
                                        {0, 0xEE, 0x11, 0xFF, 0, 0xEE, 0x11, 0xFF}, //          // wdb 5 for dq/va
                                        {0, 0xEE, 0x11, 0xFF, 0, 0xEE, 0x11, 0xFF}, //          // wdb 6 for dq/va
                                        {0, 0xEE, 0x11, 0xFF, 0, 0xEE, 0x11, 0xFF} //           // wdb 7 for dq/va

};

TAddressCarry AddressOrderMapping[MAX_CPGC_ADDRESS_ORDER_MAPPING] = {
  // Use to convert address order to CPGC 2 format
  // Field0: Encoded address order -- bit[15:12]=Col, bit[11:8]=Row, bit[7:4]=Bank,bit[3,0]= Rank
  // Field1: CPGC2_ADDRESS_DIRECTION_TYPE
  // Field2: CPGC2_ADDRESS_ORDER_TYPE
  { 0x0123, AddressDirNorth,   AddressOrderColRowBankRank },
  { 0x1023, AddressDirEast,    AddressOrderColRowBankRank },
  { 0x1203, AddressDirNorth,   AddressOrderBankColRowRank },
  { 0x2103, AddressDirEast,    AddressOrderBankColRowRank },
  { 0x0132, AddressDirNorth,   AddressOrderColRowRankBank },
  { 0x1032, AddressDirEast,    AddressOrderColRowRankBank },
  { 0x1230, AddressDirNorth,   AddressOrderRankColRowBank },
  { 0x2130, AddressDirEast,    AddressOrderRankColRowBank },
  { 0x2301, AddressDirNorth,   AddressOrderBankRankColRow },
  { 0x3201, AddressDirEast,    AddressOrderBankRankColRow },
  { 0x2310, AddressDirNorth,   AddressOrderRankBankColRow },
  { 0x3210, AddressDirEast,    AddressOrderRankBankColRow }
};


/*
*/
STATIC VOID
CADBPointerWorkaround (
  PatCadbProg0 prog0[],
  PatCadbProg1 prog1[]
  )
{
}

/**
  Get max sub ch per ch for CPGC2 IP Lib

  @param[in]  None

  @retval     0x1 or 0x2     This is the valid sub ch number for this IP

**/

UINT8
CpgcGetValidSubCh (
  VOID
  )
{
  UINT8  CpgcSubChPerCh;
  UINT16 DramType;

  CpgcSubChPerCh = SUB_CH;

  DramType = GetDramType ();

  if (DramType == SPD_TYPE_DDR5) {
    CpgcSubChPerCh = SUB_CH;
  } else if (DramType == SPD_TYPE_DDR4) {
    //
    // For DDR4 case, the SUB_CH is defined as 2 for DDRIO registers are in 2 half registers to control all signals of DDR4.
    // but for CPGC, SubCh number is seen as 1 for registers for DDR4.
    //
    CpgcSubChPerCh = 1;
  }

  return CpgcSubChPerCh;
}

/**
  Calculate even parity

  @param[in]  Field   Bit field to calculate the parity over

  @retval     0x0     There is an even numer of ones in the input field
  @retval     0x1     There is an odd number of ones in the input field
**/
UINT32
CalculateEvenParity (
  IN UINT32   Field
  )
{
  const UINT32 Length = 32;
  UINT32 Shift;

  for (Shift = Length / 2; Shift > 0; Shift >>= 1) {
    Field ^= Field >> Shift;
  }

  return Field & BIT0;
}

/**
  Calculate CADB-related parity (without accounting for the slot number)

  @param[in]  channelNvList   Pointer to the Channel NV list
  @param[in]  Prog0           Pointer to PatCADBProg0 structure
  @param[in]  Prog1           Pointer to PatCADBProg1 structure
  @param[in]  Ch              Channel number
**/
UINT8
CalculateCadbParity (
  IN struct channelNvram  (*ChannelNvList)[MAX_CH],
  IN PatCadbProg0         *Prog0,
  IN PatCadbProg1         *Prog1,
  IN UINT8                Ch
  )
{
  UINT32 Parity = 0;

  //
  // Calculate parity of CID bits if they are to be included
  //
  if ((*ChannelNvList)[Ch].encodedCSMode == 2) {
    Parity = ((*ChannelNvList)[Ch].cidBitMap & BIT2) && ((*Prog0).cid & BIT2);
    Parity ^= ((*ChannelNvList)[Ch].cidBitMap & BIT1) && ((*Prog0).cid & BIT1);
    Parity ^= ((*ChannelNvList)[Ch].cidBitMap & BIT0) && ((*Prog0).cid & BIT0);
  }

  //
  // Address bit parity
  //
  Parity ^= CalculateEvenParity ((*Prog0).address);

  //
  // Bank bit parity
  //
  Parity ^= CalculateEvenParity ((*Prog0).bank & 0xF);

  //
  // Control bit parity
  //
  Parity ^= CalculateEvenParity ((*Prog1).control & 0xF);

  return Parity & BIT0;
}

/**
  Calculate address bit mask based on whether or not A17 is enabled

  @param[in]  Host        - Pointer to SysHost
  @param[in]  Socket      - Socket number
  @param[in]  Ch          - Channel number
  @param[in]  Dimm        - Dimm number
  @param[out] AddressMask - Mask to use to zero out unused address bits
**/
VOID
CalculateAddressMask (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  OUT UINT32    *AddressMask
  )
{

  if (A17EnabledBackside (Host, Socket, Ch, Dimm)) {
    *AddressMask = 0x3FFFF;
  } else {
    *AddressMask = 0x1FFFF;
  }

  return;
}


/**
  Begin or end the process of loading the CADB buffer write pointer
  The function must be called with CADB_MUX_LOAD_START before the CADB Mux Pattern registers (CPGC_PATCADBMUX[0-3]PB) are programmed.
  This function must be called with CADB_MUX_LOAD_END after the CADB Mux Pattern registers (CPGC_PATCADBMUX[0-3]PB) are programmed.

  @param[in]  Socket      - Socket number
  @param[in]  Ch          - Channel number
  @param[in]  StartStop   - CADB_MUX_LOAD_START or CADB_MUX_LOAD_END

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
CadbMuxPatternLoad (
  IN    UINT8     Socket,
  IN    UINT8     Ch,
  IN    UINT8     StartStop
  )
{
  return MRC_STATUS_SUCCESS;
}

/**

  Detect if there cached and desired(new) values for CADB Mux Pattern are out of synced

  @param[in]                Socket              Current socket under test (0-Based)
  @param[in]                Channel             Current channel under test (0-Based)
  @param[in]                CadbMuxPattern      Pointer to array that contain the desired values for CadbMuxPattern

  @retval                   TRUE                Cache and desired values are out of sync
  @retval                   FALSE               Cache and desired values are in sync
  @retval                   TRUE                MemSsType is HBM, so there are no cache values (Out of sync)

**/
BOOLEAN
EFIAPI
CheckCadbMuxPatternCache (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT32          *CadbMuxPattern
  )
{
  PSYSHOST              Host;
  MRC_MST               MemSsType;
  struct channelNvram   (*ChannelNvList) [MAX_CH];

  Host = GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  MemSsType = GetMemSsType (Socket);

  if (MemSsType == Hbm2MemSs) {
    return TRUE;
  }

  if (((*ChannelNvList)[Channel].cadbMuxPattern[0] != CadbMuxPattern[0]) ||
      ((*ChannelNvList)[Channel].cadbMuxPattern[1] != CadbMuxPattern[1]) ||
      ((*ChannelNvList)[Channel].cadbMuxPattern[2] != CadbMuxPattern[2]) ||
      ((*ChannelNvList)[Channel].cadbMuxPattern[3] != CadbMuxPattern[3])) {
    return TRUE;
  }
  return FALSE;
}

/**

  Update cached with desired(new) values for CADB Mux Pattern

  @param[in]                Socket              Current socket under test (0-Based)
  @param[in]                Channel             Current channel under test (0-Based)
  @param[in]                CadbMuxPattern      Pointer to array that contain the desired values for CadbMuxPattern


**/
VOID
EFIAPI
UpdateCadbMuxPatternCache (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT32          *CadbMuxPattern
  )
{
  PSYSHOST              Host;
  MRC_MST               MemSsType;
  struct channelNvram   (*ChannelNvList) [MAX_CH];

  Host = GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  MemSsType = GetMemSsType (Socket);

  if (MemSsType == Hbm2MemSs) {
    return;
  }

  (*ChannelNvList)[Channel].cadbMuxPattern[0] = CadbMuxPattern[0];
  (*ChannelNvList)[Channel].cadbMuxPattern[1] = CadbMuxPattern[1];
  (*ChannelNvList)[Channel].cadbMuxPattern[2] = CadbMuxPattern[2];
  (*ChannelNvList)[Channel].cadbMuxPattern[3] = CadbMuxPattern[3];

}

/*
      Write VA pattern in CADB
      Use basic VA pattern for CADB with 2 LFSRs. Rotation is manual
      Bit Order is  [CKE[3:0], ODT[3:0], CMD[2:0], CS[3:0], BA[2:0], MA[15:0]]
                      [59:56]  [51:48]  [42:40] [35:32]  [26:24]  [15:0]

      NOTE: CKE, ODT and CS are not used in functional mode and are ignored

       @param Host         - Include all MRC global data.
       @param socket         - Memory Controller
       @param VicSpread      -
       @param VicBit            -
       @param LMNEn           - To enable LMN counter

     @retval N/A
*/
STATIC VOID
SetupCADB (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     VicSpread,
  UINT8     VicBit,
  UINT8     LMNEn
  )
{

} // SetupCADB

/**
  This function specifies the number of cache lines that dq lanes in WDB will be rotated

  @param[in] Host              - Pointer to sysHost
  @param[in] Socket            - Socket Id
  @param[in] ChbitMask         - Ch Bit mask for which test should be setup for.
  @param[in] RotateRate        - Specify the number of cache lines that dq lanes in WDB will be rotated

  @retval n/a

**/
VOID
SetCpgcDqRotateRate (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChbitMask,
  IN UINT8     RotateRate
  )
{
  CPGC_DPAT_CFG_MCCPGC_MAIN_STRUCT   DpatCfg;
  UINT8                              Ch;
  UINT8                              SubCh;

  for (Ch = 0; Ch < GetMaxChDdr (); Ch++) {
    if ((IsChannelEnabled (Socket, Ch) == FALSE) ||
        ((ChbitMask & (1 << Ch)) == 0)) {
      continue;
    }

    for (SubCh = 0; SubCh < CpgcGetValidSubCh (); SubCh++) {
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CPGC_DPAT_CFG_MCCPGC_MAIN_REG, &DpatCfg.Data);
      if (RotateRate == 0) {
        DpatCfg.Bits.lane_rotate_rate = 0;
      } else {
        DpatCfg.Bits.lane_rotate_rate = Log2x32 (RotateRate) + 1; // RotateRate = 2^(lane_rotate_rate-1)
      }
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC_DPAT_CFG_MCCPGC_MAIN_REG, &DpatCfg.Data);

    }
  }
}

/**
  This function specifies DQ pattern loaded on WDB

  @param[in] Host              - Pointer to sysHost
  @param[in] Socket            - Socket Id
  @param[in] ChannelMask       - Ch Bit mask for which test should be setup for.
  @param[in] DQPat             - Specify DQ pattern

  @retval NA

**/
VOID
SetWdbDqPattern (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChannelMask,
  IN UINT8     DQPat
  )
{
  if (DQPat == DqAAAAAAAV) { // 1 victim and 7 aggressor
    WriteWDBDqVaPattern (Host, Socket, ChannelMask, 8);
  } else if (DQPat == DqAAAVAAAV) { // 1 victim and 3 aggressor
    WriteWDBDqVaPattern (Host, Socket, ChannelMask, 4);
  } else if (DQPat == DqAVAVAVAV) { // 1 victim and 1 aggressor
    WriteWDBDqVaPattern (Host, Socket, ChannelMask, 2);
  } else if (DQPat == DqAllSamePat) { // Select uniseq 0 by default
    WriteWDBDqLanePattern (Host, Socket, SEQUENTIAL_BUFFER_PATTERN_0, MAX_WDB2_CACHELINE, FALSE);
  }
}

/**
  Convert address order to CPGC 2 format

  @param[in]  Order[4]         Address order array, [0]=Rank, [1]=Bank, [2]=Row, [3]=Col
  @param[out] *AddressCarry    Structure that contains address order info  CPGC 2 can recognize
**/
VOID
ConvertAddressCarry (
  IN  UINT8           Order[4],
  OUT TAddressCarry  *AddressCarry
  )
{
  UINT8  PrioityList[MAX_CPGC_ADDRESS_TYPE]; // Hold the carry priority for byte0 = Col, byte1 = Row, byte2 = Bank, byte3 = Rank
  UINT8  Num;
  UINT8  Priority;
  UINT8  Index;
  UINT16 EncodedOrder;

  ZeroMem ((UINT8 *) PrioityList, sizeof (PrioityList));
  Priority = 0;

  for (Num = 0; Num < MAX_CPGC_ADDRESS_ORDER_NUMBER; Num++) { // Scan address order from 0->3
    for (Index = 0; Index < MAX_CPGC_ADDRESS_TYPE; Index++) { // Scan address type from col->row->bank->rank
      if (Order[MAX_CPGC_ADDRESS_TYPE-1-Index] == Num) {
        PrioityList[Index] = Priority++; // Set the priority for specific address type: col->row->bank->rank
        break;
      }
    }// Col/Row/Bank/Rank loop
  }// Order priority loop

  // Form the encoded order in UINT16 which can be directly mapped to CPGC 2 format based on the mapping table
  // Encoded order: bit[15:12]=Col, bit[11:8]=Row, bit[7:4]=Bank,bit[3,0]= Rank
  EncodedOrder = (PrioityList[0] << 12) + (PrioityList[1] << 8) + (PrioityList[2] << 4) + PrioityList[3];

  for (Index = 0; Index < MAX_CPGC_ADDRESS_ORDER_MAPPING; Index++) {
    if (EncodedOrder == AddressOrderMapping[Index].EncodedOrder) {
      AddressCarry->AddressOrder = AddressOrderMapping[Index].AddressOrder;
      AddressCarry->AddressDirection = AddressOrderMapping[Index].AddressDirection;
      break;
    }
  }

  // If the address order is not compatible with CPGC 2 definition, then make it as CPGC 2 supported default order
  // That is Col->Row->Bank->Rank
  if (Index == MAX_CPGC_ADDRESS_ORDER_MAPPING) {
    AddressCarry->AddressOrder = AddressOrderMapping[0].AddressOrder;
    AddressCarry->AddressDirection = AddressOrderMapping[0].AddressDirection;
  }
}

/**

  Configure Uniseq Mode Before Training.

  @param[in]  Socket              Socket number
  @param[in]  Ch                  Channel number

  @retval N/A
**/
VOID
EFIAPI
ConfigureUniseqModeBeforeTraining (
  IN UINT8         Socket,
  IN UINT8         Ch
  )
{
  UINT8                                   SubCh;
  PSYSHOST                                Host;
  UINT32                                  LfsrSize[CPGC_MAX_NUMBER_DPAT_UNISEQ] = {0, 0, 0};
  UINT32                                  MuxCtl[CPGC_MAX_NUMBER_DPAT_UNISEQ] = {BTBUFFER, BTBUFFER, BTBUFFER};

  Host = GetSysHostPointer ();

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    SetCpgcDpatUniseqCfg (Host, Socket, Ch, SubCh, MuxCtl, LfsrSize);
  }
  return;
} //ConfigureUniseqModeBeforeTraining

/**

  Programs the CPGC base col repeat register

  @param[in] Host                 - Include all MRC global data.
  @param[in] socket               - Memory Controller
  @param[in] ChbitMask            - ch Bit mak for which test should be setup for.
  @param[in] BaseColRepeatsValue  - Base Col Repeats Value (Alternate Base_Address_Loopcount)

  @retval N/A
**/
VOID
ProgramBaseColRepeats (
  PSYSHOST      Host,
  UINT8         Socket,
  UINT32        ChbitMask,
  UINT32        BaseColRepeatsValue
  )
{
  UINT8                                                  Ch;
  UINT8                                                  SubCh;
  CPGC2_BASE_COL_REPEATS_MCCPGC_MAIN_STRUCT              BaseColRepeats;
  UINT8                                                  MaxSubCh;
  MRC_MST                                                MemSsType;
  CPGC2_ADDRESS_SIZE_N0_MCCPGC_MAIN_STRUCT               AddressSize0;
  MaxSubCh = CpgcGetValidSubCh ();
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  for (Ch = 0; Ch < GetMaxCh (Host, Socket, MemSsType); Ch++) {
    if (!(ChbitMask & (1 << Ch))) {
      continue;
    }
    for (SubCh = 0; SubCh < MaxSubCh; SubCh++) {
      BaseColRepeats.Data = 0;
      BaseColRepeats.Bits.base_col_repeats = BaseColRepeatsValue - 1;
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_BASE_COL_REPEATS_MCCPGC_MAIN_REG, &BaseColRepeats.Data);
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CPGC2_ADDRESS_SIZE_N0_MCCPGC_MAIN_REG, &AddressSize0.Data);
      AddressSize0.Bits.block_size_max_bank = DB_DFE_BLOCK_SIZE_MAX_BANK;
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_ADDRESS_SIZE_N0_MCCPGC_MAIN_REG, &AddressSize0.Data);

    } // SubCh loop
  } // Ch loop

} // ProgramBaseColRepeats
/**

     Programs all the key registers to define a CPCG test

      @param Host           - Include all MRC global data.
      @param socket         - Memory Controller
      @param ChbitMask      - ch Bit mak for which test should be setup for.
      @param CmdPat         = [0: PatWrRd (Standard Write/Read Loopback),
                                     1: PatWr (Write Only),
                                     2: PatRd (Read Only),
                                     3: PatRdWrTA (ReadWrite Turnarounds),
                                     4: PatWrRdTA (WriteRead Turnarounds),
                                     5: PatODTTA (ODT Turnaround]
      @param NumCL          = Number of Cache lines
      @param LC             = Loop Count
      @param CPGCAddress    = Structure that stores start, stop and increment details for address
      @param SOE            = [0: Never Stop, 1: Stop on Any Lane, 2: Stop on All Byte, 3: Stop on All Lane]
      @param WDBPattern     - Structure that stores start, Stop, IncRate and Dqpat for pattern.
      @param EnCADB         - Enable test to write random deselect packages on bus to create xtalk/isi
      @param EnCKE          - Enable CKE power down by adding 64
      @param SubSeqWait     - # of Dclks to stall at the end of a sub-sequence

    @retval N/A
**/
VOID
SetupIOTest (
  PSYSHOST      Host,
  UINT8         Socket,
  UINT32        ChbitMask,
  UINT8         CmdPat,
  UINT16        NumCL,
  UINT32        LC,
  TCPGCAddress  *CPGCAddress,
  UINT8         SOE,
  TWDBPattern   *WDBPattern,
  UINT8         EnCADB,
  UINT8         EnCKE,
  UINT16        SubSeqWait
  )
{
  UINT8                                                  Ch;
  UINT8                                                  SubCh;
  MRC_MST                                                MemSsType;
  TAddressCarry                                          AddressCarry;
  CPGC2_BASE_ADDRESS_CONTROL_MCCPGC_MAIN_STRUCT          BaseAddressControl;
  CPGC2_BASE_COL_REPEATS_MCCPGC_MAIN_STRUCT              BaseColRepeats;
  CPGC2_BASE_REPEATS_MCCPGC_MAIN_STRUCT                  BaseRepeats;
  CPGC2_BLOCK_REPEATS_MCCPGC_MAIN_STRUCT                 BlockRepeats;
  CPGC2_REGION_LOW_N0_MCCPGC_MAIN_STRUCT                 RegionLow0;
  CPGC2_REGION_LOW_N1_MCCPGC_MAIN_STRUCT                 RegionLow1;
  CPGC2_DATA_CONTROL_MCCPGC_MAIN_STRUCT                  DataControl;
  CPGC2_ADDRESS_SIZE_N0_MCCPGC_MAIN_STRUCT               AddressSize0;
  CPGC2_ADDRESS_SIZE_N1_MCCPGC_MAIN_STRUCT               AddressSize1;
  CPGC2_ADDRESS_CONTROL_MCCPGC_MAIN_STRUCT               AddressControl;
  CPGC_SEQ_CTL_MCCPGC_MAIN_STRUCT                        SeqCtl;
  CPGC_SEQ_CFG_A_MCCPGC_MAIN_STRUCT                      SeqCfgA;
  CPGC_DPAT_CFG_MCCPGC_MAIN_STRUCT                       DpatCfg;
  CPGC_DPAT_BUF_CFG_MCCPGC_MAIN_STRUCT                   DpatBufCfg;
  CPGC_ERR_CTL_MCCPGC_MAIN_STRUCT                        ErrCtl;
  CPGC2_BASE_CLOCK_CONFIG_MCCPGC_MAIN_STRUCT             BaseClockConfig;
  CPGC_MISCCKECTL_MCDDC_CTL_STRUCT                       CpgcMiscCkeCtlReg;
  CPGC_ERR_CNTRCTL_0_MCCPGC_MAIN_STRUCT                  ErrCntrctl0;
  CPGC_ERR_CNTRCTL_1_MCCPGC_MAIN_STRUCT                  ErrCntrctl1;
  CPGC_ERR_CNTRCTL_2_MCCPGC_MAIN_STRUCT                  ErrCntrctl2;
  CPGC2_RASTER_REPO_CONFIG_MCCPGC_MAIN_STRUCT            RasterRepoConfig;
  UINT8                                                  MaxSubCh;
  UINT32                                                 Patbuf[CPGC_MAX_NUMBER_DPAT_UNISEQ] = {RD_DQ_DQS_PATTERN_DDR5,
                                                                                                CPGC_DONT_CARE,
                                                                                                CPGC_DONT_CARE};
  UINT32                                                 LfsrSize[CPGC_MAX_NUMBER_DPAT_UNISEQ] = {0, CPGC_DONT_CARE, CPGC_DONT_CARE};
  UINT32                                                 MuxCtl[CPGC_MAX_NUMBER_DPAT_UNISEQ] = {BTBUFFER, CPGC_DONT_CARE, CPGC_DONT_CARE};

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "SetupIOTest Starts\n");

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  MaxSubCh = CpgcGetValidSubCh ();

  if (SubSeqWait > 0xFF) {
    SubSeqWait = 0xFF;
  }
  ConvertAddressCarry (CPGCAddress->Order, &AddressCarry);

  for (Ch = 0; Ch < GetMaxCh (Host, Socket, MemSsType); Ch++) {
    if (!(ChbitMask & (1 << Ch))) {
      continue;
    }
    //
    // Disable the CKE override. (No CKE in DDR5)
    //
    CpgcMiscCkeCtlReg.Data = MemReadPciCfgEp (Socket, Ch, CPGC_MISCCKECTL_MCDDC_CTL_REG);
    CpgcMiscCkeCtlReg.Bits.cke_override = 0;
    CpgcMiscCkeCtlReg.Bits.refresh_enable = 0;
    CpgcMiscCkeCtlReg.Bits.cke_on = 0;
    MemWritePciCfgEp (Socket, Ch, CPGC_MISCCKECTL_MCDDC_CTL_REG, CpgcMiscCkeCtlReg.Data);
    //
    // CPGC in order mode enable.
    //
    CpgcInorderModeEnable (Socket, Ch, TRUE);

    for (SubCh = 0; SubCh < MaxSubCh; SubCh++) {

      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_CTL_MCCPGC_MAIN_REG), 4);   // Clear errors, start and stop

      SeqCtl.Data = 0;
      SeqCtl.Bits.single_march_element_mode = 0; // Don't pause after algorithm instruction
      SeqCtl.Bits.continue_single_mode = 0;      // No need fpr continous tests
      SeqCtl.Bits.single_cmd_mode = 0;           // Don't pause command issuing
      SeqCtl.Bits.clear_errors = 0;              // No need to clear errors for the test
      SeqCtl.Bits.stop_test = 0;                 // Stop previous test if any
      //
      //the req CPGC_SEQ is assigned to sub channels
      //
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_CTL_MCCPGC_MAIN_REG), SeqCtl.Data);

      SeqCfgA.Data = 0;
      SeqCfgA.Bits.channel_assign = (SubCh == 1) ? 3 : 0; // Assign sub channels for this generator (2 sub-ch runs in parallel.)
      SeqCfgA.Bits.initialization_mode = (SubCh == 1) ? CPGC_Active_Mode : Idle_Mode; // Test Mode
      //
      // Bind this request engine to all other globally enabled request engines
      //
      SeqCfgA.Bits.global_start_bind = 0;
      SeqCfgA.Bits.global_stop_bind = 0;
      SeqCfgA.Bits.global_clear_err_bind = 0;
      SeqCfgA.Bits.global_stop_on_err_bind = 0;
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_CFG_A_MCCPGC_MAIN_REG), SeqCfgA.Data);

      BaseClockConfig.Data = 0;
      BaseClockConfig.Bits.increment_value = 1; // Wait timer advance by 1
      BaseClockConfig.Bits.clock_freq = 400;// 400/4 = 100 MHZ.
      BaseClockConfig.Bits.prescaler_for_clock_freq = 1; // No divide for MC provided clock (divided by 1)
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_BASE_CLOCK_CONFIG_MCCPGC_MAIN_REG), BaseClockConfig.Data);

      AddressSize0.Data = 0;
      AddressSize0.Bits.block_size_bits_col = Log2x32(CPGCAddress->Stop[3] - CPGCAddress->Start[3] + 1); // 2**N
      AddressSize0.Bits.block_size_bits_row = Log2x32(CPGCAddress->Stop[2] - CPGCAddress->Start[2] + 1); // 2**N
      AddressSize0.Bits.block_size_max_bank = CPGCAddress->Stop[1] - CPGCAddress->Start[1]; // N-1
      AddressSize0.Bits.block_size_max_rank = CPGCAddress->Stop[0] - CPGCAddress->Start[0]; // N-1
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_ADDRESS_SIZE_N0_MCCPGC_MAIN_REG, &AddressSize0.Data);

      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CPGC2_ADDRESS_SIZE_N1_MCCPGC_MAIN_REG, &AddressSize1.Data);
      AddressSize1.Bits.bl_chop_en = 0; // No half-line requests
      AddressSize1.Bits.region_size_bits_col = Log2x32(CPGCAddress->Stop[3] - CPGCAddress->Start[3] + 1); // 2**N
      AddressSize1.Bits.region_size_bits_row = Log2x32(CPGCAddress->Stop[2] - CPGCAddress->Start[2] + 1); // 2**N
      AddressSize1.Bits.region_size_max_bank = CPGCAddress->Stop[1] - CPGCAddress->Start[1]; // N-1
      AddressSize1.Bits.region_size_max_rank = CPGCAddress->Stop[0] - CPGCAddress->Start[0]; // N-1
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_ADDRESS_SIZE_N1_MCCPGC_MAIN_REG, &AddressSize1.Data);

      RegionLow0.Data = 0;
      RegionLow1.Data = 0;
      RegionLow0.Bits.low_col = CPGCAddress->Start[3];
      RegionLow1.Bits.low_row = CPGCAddress->Start[2];
      RegionLow1.Bits.low_bank = CPGCAddress->Start[1];
      RegionLow0.Bits.low_rank = CPGCAddress->Start[0];
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_REGION_LOW_N0_MCCPGC_MAIN_REG, &RegionLow0.Data);
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_REGION_LOW_N1_MCCPGC_MAIN_REG, &RegionLow1.Data);

      BaseRepeats.Data = 0;
      BaseRepeats.Bits.base_repeats = NumCL - 1;// NumCL - 1 means NumCL cache line numbers.
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_BASE_REPEATS_MCCPGC_MAIN_REG, &BaseRepeats.Data);

      BlockRepeats.Data = 0;
      BlockRepeats.Bits.block_repeats = LC - 1;// LC -1 means LC loop counts.
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_BLOCK_REPEATS_MCCPGC_MAIN_REG, &BlockRepeats.Data);

      AddressControl.Data = 0;
      AddressControl.Bits.address_decode_rotate_repeats = 0; // 1 repeat at address level
      AddressControl.Bits.fasty_address_direction = AddressCarry.AddressDirection;
      AddressControl.Bits.fasty_address_order = AddressCarry.AddressOrder;
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_ADDRESS_CONTROL_MCCPGC_MAIN_REG, &AddressControl.Data);

      DataControl.Data = 0;
      DataControl.Bits.data_select_rotation_repeats = 0;
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_DATA_CONTROL_MCCPGC_MAIN_REG, &DataControl.Data);

      BaseColRepeats.Data = 0;
      BaseColRepeats.Bits.base_col_repeats = 0;
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_BASE_COL_REPEATS_MCCPGC_MAIN_REG, &BaseColRepeats.Data);

      BaseAddressControl.Data = 0;
      BaseAddressControl.Bits.col_inc = Log2x32 (CPGCAddress->IncVal[3]);
      BaseAddressControl.Bits.row_inc = Log2x32 (CPGCAddress->IncVal[2]);
      BaseAddressControl.Bits.bank_inc = Log2x32 (CPGCAddress->IncVal[1]);
      BaseAddressControl.Bits.reserved_rank_inc = Log2x32 (CPGCAddress->IncVal[0]); //value is 0.
      //Increment Address after executing the Command_Instruction with the Last bit set
      BaseAddressControl.Bits.address_order0_inc_rate = 0;
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_BASE_ADDRESS_CONTROL_MCCPGC_MAIN_REG, &BaseAddressControl.Data);

      //
      // DDRT2 need Speial wr-delay-rd
      //
      if (IsDdrtDimm (Host, Socket, Ch) && (CmdPat == PatWrRd)) {
        CmdPat = PatWrRdDdrt;
        SubSeqWait = CPGC_SUBSEQ_WAIT_DDRT2; // DDRT2 value is 512
      }

      SetCpgcCmdPat (Host, Socket, Ch, SubCh, CmdPat, &AddressCarry, SubSeqWait);

      SetCpgcDpatUniseqCfg (Host, Socket, Ch, SubCh, MuxCtl, LfsrSize);
      // Low freq clock pattern
      SetCpgcDpatUniseqPat (Host, Socket, Ch, SubCh, Patbuf);

      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CPGC_DPAT_CFG_MCCPGC_MAIN_REG, &DpatCfg.Data);;
      DpatCfg.Bits.ecc_replace_byte_control = 1;
      DpatCfg.Bits.ecc_data_source_sel = 0; // Byte 0 used to replace ECC byte
      if (Host->nvram.mem.eccEn) {
        DpatCfg.Bits.ecc_disable = 0;
      } else {
        DpatCfg.Bits.ecc_disable = 1;
      }
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC_DPAT_CFG_MCCPGC_MAIN_REG, &DpatCfg.Data);

      // Disable any data/ecc inversion so that we see data from extbuf directly on DQ lanes
      SetCpgcDcMaskLanes(Host, Socket, Ch, SubCh, 0, 0, 0);

      //
      // Program Write Data Buffer Related Entries
      //
      DpatBufCfg.Data = 0;
      DpatBufCfg.Bits.buf_pntr_inc_scale = 0; // Exponential number of Cache Lines
      DpatBufCfg.Bits.buf_pntr_inc_rate = Log2x32 (WDBPattern->IncRate);
      DpatBufCfg.Bits.buf_strt_pntr = WDBPattern->Start;
      DpatBufCfg.Bits.buf_end_pntr = WDBPattern->Stop;
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_DPAT_BUF_CFG_MCCPGC_MAIN_REG), DpatBufCfg.Data);

      RasterRepoConfig.Data = 0;
      RasterRepoConfig.Bits.rasterrepoclear = 1; //Do it in the test start // Reset the raster repo entries. This bit auto-clears
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_RASTER_REPO_CONFIG_MCCPGC_MAIN_REG), RasterRepoConfig.Data);

      // No masking of reads for error checking
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_MASK_ERRS_FIRST_N_READS_MCCPGC_MAIN_REG), 0);

      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_DPAT_ALT_BUF_CFG_MCCPGC_MAIN_REG), 0);

      ErrCntrctl0.Data = 0;
      ErrCntrctl0.Bits.counter_pointer = 0; // don't care for counter_control_scope = 0
      ErrCntrctl0.Bits.counter_control_sel = 0; // don't care for counter_control_scope = 0
      ErrCntrctl0.Bits.counter_control_ui = 0; // count errors on all UI's
      ErrCntrctl0.Bits.counter_control_scope = 0; // non-specific selection (count everything)
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_CNTRCTL_0_MCCPGC_MAIN_REG), ErrCntrctl0.Data);
      ErrCntrctl1.Data = 0;
      ErrCntrctl1.Bits.counter_pointer = 0; // only matters for particular UI (counter_control_ui = 3)
      ErrCntrctl1.Bits.counter_control_sel = 3; // set to count errors on UI's
      ErrCntrctl1.Bits.counter_control_ui = 1; // count errors on even UI's
      ErrCntrctl1.Bits.counter_control_scope = 1; // specific selection
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_CNTRCTL_1_MCCPGC_MAIN_REG), ErrCntrctl1.Data);
      ErrCntrctl2.Data = 0;
      ErrCntrctl2.Bits.counter_pointer = 0; // only matters for particular UI (counter_control_ui = 3)
      ErrCntrctl2.Bits.counter_control_sel = 3;  // set to count errors on UI's
      ErrCntrctl2.Bits.counter_control_ui = 2; // count errors on odd UI's
      ErrCntrctl2.Bits.counter_control_scope = 1; // specific selection
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_CNTRCTL_2_MCCPGC_MAIN_REG), ErrCntrctl2.Data);

      ErrCtl.Data = 0;
      ErrCtl.Bits.stop_on_n = 0;
      ErrCtl.Bits.stop_on_error_ctl = SOE;
      ErrCtl.Bits.errchk_mask_chunk = 0;
      ErrCtl.Bits.errchk_mask_cacheline = 0xFF;
      ErrCtl.Bits.be_train_err_en = 1; // Use default (unclear what this is for)
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_CTL_MCCPGC_MAIN_REG), ErrCtl.Data);

      // Lanes [31:0] error checking enabled
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_LNEN_LO_MCCPGC_MAIN_REG), 0);
      // Check Enable mask for UI[15:0] -for x32 bus this register is repurposed as chunk/UI enable
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_LNEN_HI_MCCPGC_MAIN_REG), 0xFFFF);

      //
      // Do not check ECC bits if ECC is disabled
      //
      if (Host->nvram.mem.eccEn) {
        MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_XLNEN_MCCPGC_MAIN_REG), 0);
      } else {
        MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_XLNEN_MCCPGC_MAIN_REG), 0xFF);
      }

    } // SubCh loop
  } // Ch loop

} // SetupIOTest

/**

  Setup IO Test Basic VA

  @param Host      - Pointer to sysHost
  @param socket    - Socket from which we are going to read the registers
  @param ChbitMask - ch Bit mask for which test should be setup for.
  @param LC        - Loop Count
  @param SOE       - [0: Never Stop, 1: Stop on Any Lane, 2:Stop on All Byte, 3: Stop on All Lane]
  @param EnCADB    - Enable test to write random deselect packages on bus to create xtalk/isi
  @param EnCKE     - Enable CKE power down by adding 64
  @param Spread    - Pattern Spread

  @retval N/A

**/
VOID
SetupIOTestBasicVA (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    ChbitMask,
  UINT8     LC,
  UINT8     SOE,
  UINT8     EnCADB,
  UINT8     EnCKE,
  UINT32    Spread
  )
{

} // SetupIOTestBasicVA10nm

/**

  Setup IO Test MPR

  @param Host      - Pointer to sysHost
  @param socket    - Socket from which we are going to read the registers
  @param ChbitMask - ch Bit mask for which test should be setup for.
  @param LC        - Loop Count
  @param SOE       - [0: Never Stop, 1: Stop on Any Lane, 2:Stop on All Byte, 3: Stop on All Lane]
  @param EnCADB    - Enable test to write random deselect packages on bus to create xtalk/isi
  @param EnCKE     - Enable CKE power down by adding 64

  @retval N/A

**/
VOID
SetupIOTestMPR (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    ChbitMask,
  UINT8     LC,
  UINT8     SOE,
  UINT8     EnCADB,
  UINT8     EnCKE
  )
{

} // SetupIOTestMPR10nm

/**

  Runs one or more CPGC tests (based on TestType) and returns ch errors

  @param Host        - Pointer to sysHost
  @param socket      - Socket from which we are going to read the registers
  @param ChBitMask   - ch Bit mask for which test should be setup for.
  @param DQPat       - DQPat = [0: BasicVA, 1: SegmentWDB, 2: CADB, 3: TurnAround, 4: LMNVa]
  @param SeqLCs      - SeqLCs is an array of one or more loopcounts.
  @param ClearErrors - BOOLEAN to clear or not clear errors
  @param Mode        - mode allows for different types of modes for margining:

  @retval SUCCESS
  @retval FAILURE

**/
UINT32
RunIOTest (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT32    ChBitMask,
  UINT8     DQPat,
  UINT8     *SeqLCs,
  UINT8     ClearErrors,
  UINT16    Mode
  )
{
  UINT8   Ch = 0;
  UINT8   SubCh = 0;
  UINT8   Reload;
  UINT8   NumTests, TestIndex;
  UINT32  TestRand;
  UINT32  TRand;
  CPGC_DPAT_BUF_CFG_MCCPGC_MAIN_STRUCT   DpatBufCfg;
  CPGC2_BLOCK_REPEATS_MCCPGC_MAIN_STRUCT BlockRepeats;
  // Runs one or more CPGC tests (based on TestType) and returns ch errors
  // WDBIncRates is an array of one or more increments rates for the write data buffer
  // SeqLCs is an array of one or more loopcounts.
  // DQPat = [0: BasicVA, 1: SegmentWDB, 2: CADB, 3: TurnAround, 4: LMNVa]
  // mode allows for different types of modes for margining:
  //       mode is {Bit0: PhLock (keep all bytes within in ch in phase),
  //               Bit1: Ch2Ch Data out of phase (LFSR seed), Bit 15:2: Reserved}

  UINT8   WDBIncRates[8] = {1, 1, 1, 1, 1, 1, 1, 1};
  UINT8   WDBStart[8]    = {0, 0, 0, 0, 0, 0, 0, 0};
  UINT8   WDBStop[8]     = {9, 9, 9, 9, 9, 9, 9, 9};
  UINT8   PollOnTestComplete = 1;
  UINT8   MaxChDdr;
  MRC_TT  CurrentTestType;
  UINT32  Patbuf[CPGC_MAX_NUMBER_DPAT_UNISEQ];

  CurrentTestType = GetCurrentTestType (Socket);
  MaxChDdr = GetMaxChDdr ();
  RcDebugPrintWithDevice (SDBG_CPGC, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "RunIOTest Starts\n");

  TestRand = 0xBAD00451;
  NumTests = 1;

  if (DQPat == SegmentWDB) {
    NumTests = 4;
    // WDBIncRates[3:0]  = [25, 25, 19, 10]
    // WDBStart[3:2]     = 10
    // WDBStop[3:2]      = 63

    WDBIncRates[3] = 10;
    WDBIncRates[2] = 19;
    WDBIncRates[1] = 25;
    WDBIncRates[0] = 25;

    WDBStart[3]    = 10;
    WDBStart[2]    = 10;
    WDBStop[3]     = 63;
    WDBStop[2]     = 63;
  } else if (DQPat == CADB) {
    NumTests = 7;
  } else if (DQPat == TurnAroundWR) {
    NumTests = 8;
  } else if (DQPat == TurnAroundODT) {
    NumTests = 4;
  }

  for (TestIndex = 0; TestIndex < NumTests; TestIndex++) {
    Reload = Log2x32 (WDBIncRates[TestIndex]-1);
    if (WDBIncRates[TestIndex] > 31) {
      WDBIncRates[TestIndex] = Reload;
    } else {
      WDBIncRates[TestIndex] += 32 - 1;
    }

    for (Ch = 0; Ch < MaxChDdr; Ch++) {

      if (!((1 << Ch) & ChBitMask)) {
        continue;
      }
      for (SubCh = 0; SubCh < CpgcGetValidSubCh (); SubCh++) {
        //
        // Check for SOE
        //
        MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_CTL_MCCPGC_MAIN_REG));

        if (Mode & BIT1) {
          // Circular shift by 9*ch
          TRand = (TestRand >> (9 * Ch)) | (TestRand << (32 - (9 * Ch)));
          Patbuf[0] = 0x0EA10CA1 ^ TRand;
          Patbuf[1] = 0xDEAD0A1E ^ TRand;
          Patbuf[2] = 0xBEEF0D08 ^ TRand;
          SetCpgcDpatUniseqPat (Host, Socket, Ch, SubCh, Patbuf);
        }

        if (DQPat == SegmentWDB) {
          DpatBufCfg.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_DPAT_BUF_CFG_MCCPGC_MAIN_REG));
          DpatBufCfg.Bits.buf_pntr_inc_scale = 0;  //Exponential number of Cache Lines
          DpatBufCfg.Bits.buf_pntr_inc_rate = WDBIncRates[TestIndex];
          DpatBufCfg.Bits.buf_strt_pntr = WDBStart[TestIndex];
          DpatBufCfg.Bits.buf_end_pntr = WDBStop[TestIndex];
          MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_DPAT_BUF_CFG_MCCPGC_MAIN_REG), DpatBufCfg.Data);

          // Program desired loopcount
          GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CPGC2_BLOCK_REPEATS_MCCPGC_MAIN_REG, &BlockRepeats.Data);
          BlockRepeats.Bits.block_repeats = ((SeqLCs[TestIndex] + 1) & 0x1F);
          GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_BLOCK_REPEATS_MCCPGC_MAIN_REG, &BlockRepeats.Data);
        } else if (DQPat == CADB) {
          SetupCADB(Host, Socket, Ch, NumTests, TestIndex, 0);   // LMNEn = 0
        }
        if (IsInfiniteCpgcTest (Host, Socket, Ch, SubCh)) {
          PollOnTestComplete = 0;
        }
      } // SubCh loop
    } // Ch loop

    IO_Reset (Host, Socket);

    //
    // Start Test and Poll on completion on cpgc reqgen1
    //
    CpgcGlobalTestStart (Host, Socket, ChBitMask, SUB_CH_B);

    if (PollOnTestComplete) {
        CpgcPollGlobalTestDone (Host, Socket, ChBitMask, SUB_CH_B);
    }

  } // TestIndex loop

  CteDelayQclk (60);
  RunIOTest10nmDelayTarget (Host, Socket, Ch);

  return SUCCESS;
} // RunIOTest

/**
  Map physical ranks to CPGC logical ranks.

  @param[in]  Host                      - Pointer to sysHost
  @param[in]  Socket                    - Socket from which we are going to read the registers
  @param[in]  Ch                        - Channel
  @param[in]  Phy2LogicalRankMapping    - Physical to logical rank mapping
  @param[in]  LogicalRankCount          - Logical rank number
  @param[in]  RankFeatureEnable         - RankFeatureEnable is a bit mask that can enable Refresh or ZQ
  @param[in]  Phy2LogicalSubRankMapping - Sub-Rank number when 3DS dimm. It applies to all the ranks specified by RankBitMask

  @retval VOID

**/
VOID
MapCPGCRanks (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     (*Phy2LogicalRankMapping)[CPGC_RANK_MAPPING_MAX_NUMBER],
  IN UINT8     LogicalRankCount,
  IN UINT8     RankFeatureEnable,
  IN UINT8     (*Phy2LogicalSubRankMapping)[2]
  )
{
  UINT32                                          CRValueA; // rank0 - rank5 mapping
  UINT32                                          CRValueB; // rank6 - rank11 mapping
  UINT8                                           LogicalRank;
  UINT8                                           SubCh;
  UINT8                                           MaxSubCh;
  CPGC2_ADDRESS_SIZE_N0_MCCPGC_MAIN_STRUCT        AddressSize0;
  CPGC2_ADDRESS_SIZE_N1_MCCPGC_MAIN_STRUCT        AddressSize1;
  UINT8                                           RankBitMask;

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "MapCPGCRanks Starts\n");

  MaxSubCh      = CpgcGetValidSubCh ();

  for (SubCh = 0; SubCh < MaxSubCh; SubCh++) {
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_RANK_L2P_MAPPING_A_MCCPGC_MAIN_REG), 0);
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_RANK_L2P_MAPPING_B_MCCPGC_MAIN_REG), 0);
  }


  CRValueA  = 0;
  CRValueB  = 0;
  RankBitMask = 0;


  for (LogicalRank = 0; LogicalRank < LogicalRankCount; LogicalRank++) {

    RankBitMask |= 1 << (*Phy2LogicalRankMapping)[LogicalRankCount];

    if (LogicalRank < MAX_LG2PHRANKMAPPING_PER_REGISTER) {
      CRValueA |= ((*Phy2LogicalRankMapping)[LogicalRank] << (5 * LogicalRank));
    } else {
      CRValueB |= ((*Phy2LogicalRankMapping)[LogicalRank] << (5 * (LogicalRank - MAX_LG2PHRANKMAPPING_PER_REGISTER)));
    }

  } // rank loop

  SetCpgcTrainTarget (Socket, Ch, FALSE);
  SetDdrtForceMode (Socket, Ch, FALSE);

  //
  // Write New Rank Mapping and Max Rank
  //
  for (SubCh = 0; SubCh < MaxSubCh; SubCh++) {
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_RANK_L2P_MAPPING_A_MCCPGC_MAIN_REG), CRValueA);
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_RANK_L2P_MAPPING_B_MCCPGC_MAIN_REG), CRValueB);
  }

  for (SubCh = 0; SubCh < MaxSubCh; SubCh++) {
    GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CPGC2_ADDRESS_SIZE_N0_MCCPGC_MAIN_REG, &AddressSize0.Data);
    AddressSize0.Bits.block_size_max_rank = LogicalRankCount - 1;
    GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_ADDRESS_SIZE_N0_MCCPGC_MAIN_REG, &AddressSize0.Data);

    GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CPGC2_ADDRESS_SIZE_N1_MCCPGC_MAIN_REG, &AddressSize1.Data);
    AddressSize1.Bits.region_size_max_rank = LogicalRankCount - 1;
    GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_ADDRESS_SIZE_N1_MCCPGC_MAIN_REG, &AddressSize1.Data);
  }

  //
  // SPR_TODO: HSD 1507709040
  // [CPGC] SPR: CPGC rank select functionality shall support 3DS DIMM's subrank selection for signal training
  //

  return;
}


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
  UINT8                           SubCh;
  UINT8                           MaxSubCh;
  UINT8                           Ch = 0;
  UINT8                           MaxChDdr;
  struct channelNvram             (*ChannelNvList) [MAX_CH];
  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "SaveCpgcRankL2pMapping Starts\n");

  MaxSubCh = CpgcGetValidSubCh ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList) [Ch].enabled == 0) {
      continue;
    }
    for (SubCh = 0; SubCh < MaxSubCh; SubCh++) {
      CpgcSeqRankL2PMapping[Ch][SubCh][0] = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_RANK_L2P_MAPPING_A_MCCPGC_MAIN_REG));
      CpgcSeqRankL2PMapping[Ch][SubCh][1] = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_RANK_L2P_MAPPING_B_MCCPGC_MAIN_REG));
    }
  }
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
  UINT8                           SubCh;
  UINT8                           MaxSubCh;
  UINT8                           Ch = 0;
  UINT8                           MaxChDdr;
  struct channelNvram             (*ChannelNvList) [MAX_CH];
  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "RestoreCpgcRankL2pMapping Starts\n");

  MaxSubCh = CpgcGetValidSubCh ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList) [Ch].enabled == 0) {
      continue;
    }
    for (SubCh = 0; SubCh < MaxSubCh; SubCh++) {
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_RANK_L2P_MAPPING_A_MCCPGC_MAIN_REG), CpgcSeqRankL2PMapping[Ch][SubCh][0]);
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_RANK_L2P_MAPPING_B_MCCPGC_MAIN_REG), CpgcSeqRankL2PMapping[Ch][SubCh][1]);
    }
  }
  return;
}

/**
  Programs CPGC to run on the selected logical ranks

  @param[in]  Host              - Pointer to sysHost
  @param[in]  Socket            - Socket from which we are going to read the registers
  @param[in]  Ch                - Channel
  @param[in]  RankBitMask       - Bit mask of logical ranks to work on
  @param[in]  RankFeatureEnable - RankFeatureEnable is a bit mask that can enable Refresh or ZQ
  @param[in]  SubRank           - Sub-Rank number when 3DS dimm. It applies to all the ranks specified by RankBitMask

  @retval 0                     - Nothing is selected
  @retval (UINT8) (1 << Ch)     - Ranks are selected for test

**/
UINT8
SelectCPGCRanks (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     RankBitMask,
  IN UINT8     RankFeatureEnable,
  IN UINT8     SubRank
  )
{
  UINT32                                          CRValueA; // rank0 - rank5 mapping
  UINT32                                          CRValueB; // rank6 - rank11 mapping
  UINT8                                           Dimm;
  UINT8                                           Rank;
  UINT8                                           RankCount;
  UINT8                                           McId;
  UINT8                                           SubCh;
  UINT8                                           MaxSubCh;
  UINT8                                           TrngTargetSelected;
  struct dimmNvram                                (*DimmNvList)[MAX_DIMM];
  struct channelNvram                             (*ChannelNvList)[MAX_CH];
  UINT8                                           DdrtTarget = 0;
  CPGC2_ADDRESS_SIZE_N0_MCCPGC_MAIN_STRUCT        AddressSize0;
  CPGC2_ADDRESS_SIZE_N1_MCCPGC_MAIN_STRUCT        AddressSize1;

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "SelectCPGCRanks Starts\n");

  ChannelNvList = GetChannelNvList (Host, Socket);
  DimmNvList    = GetDimmNvList(Host, Socket, Ch);
  McId          = GetMCID(Host, Socket, Ch);
  MaxSubCh      = CpgcGetValidSubCh ();

  // Programs CPGC to run on the selected logical ranks


  // Check if nothing is selected
  if ((RankBitMask & 0xFF) == 0) {
    for (SubCh = 0; SubCh < MaxSubCh; SubCh++) {
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_RANK_L2P_MAPPING_A_MCCPGC_MAIN_REG), 0);
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_RANK_L2P_MAPPING_B_MCCPGC_MAIN_REG), 0);
    }

    return 0;

  } else {
    // Normal case
    // Setup GPGC Test to iteration through appropriate ranks during test
    CRValueA  = 0;
    CRValueB  = 0;
    RankCount = 0;

    // Prepare Rank Mapping and Max Rank
    TrngTargetSelected = 0;
    for (Rank = 0; Rank < MAX_RANK_CH; Rank++) { //rank in range(4):
      if (!((1 << Rank) & RankBitMask)) {
        continue;
      }

      //Set TRNG_TARGET on selected NVMDIMM ranks; else clear TRNG_TARGET on selected DDR4 ranks
      //Note: SKX design assumes NVMDIMM and DDR4 ranks cannot be selected in the same CPGC sequence
      if ((*DimmNvList)[Rank / MAX_RANK_DIMM].DcpmmPresent) {
        if (TrngTargetSelected && (DdrtTarget == 0)) {
          RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_45);
        } else {
          DdrtTarget = 1;
        }
      } else {
        if (TrngTargetSelected && (DdrtTarget == 1)) {
          RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_46);
        } else {
          DdrtTarget = 0;
        }
      }
      TrngTargetSelected = 1;
      if (RankCount < MAX_LG2PHRANKMAPPING_PER_REGISTER) {
        CRValueA |= (Rank << (5 * RankCount));
      } else {
        CRValueB |= (Rank << (5 * (RankCount - MAX_LG2PHRANKMAPPING_PER_REGISTER)));
      }

      RankCount += 1;
    } // rank loop

    if ((GetCurrentTestType (Socket) == MdqsReceiveEnablePhaseTraining) ||
        (GetCurrentTestType (Socket) == RcvenFineTest) ||
        (GetCurrentTestType (Socket) == RdDqTest) ||
        (GetCurrentTestType (Socket) == DqSwzDiscTestDdr5) ||
        (GetCurrentTestType (Socket) == ReadPreDfe2DCenteringTest) ||
        (GetCurrentTestType (Socket) == ReadDfeTest) ||
        (GetCurrentTestType (Socket) == ReadPostDfe2DCenteringTest) ||
        (GetCurrentTestType (Socket) == EarlyRxVrefTest) ||
        (GetCurrentTestType (Socket) == MdqsCoarseReadDelayTraining)) {
      DdrtTarget = 0;
    }

    SetCpgcTrainTarget (Socket, Ch, (BOOLEAN) DdrtTarget);

    SetDdrtForceMode (Socket, Ch, (BOOLEAN) DdrtTarget);

    // Write New Rank Mapping and Max Rank
    for (SubCh = 0; SubCh < MaxSubCh; SubCh++) {
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_RANK_L2P_MAPPING_A_MCCPGC_MAIN_REG), CRValueA);
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_RANK_L2P_MAPPING_B_MCCPGC_MAIN_REG), CRValueB);
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Rank = 0; Rank < MAX_RANK_CH; Rank++) { //rank in range(4):
      if (!((1 << Rank) & RankBitMask)) {
        continue;
      }
      //
      // skip if not in encoded mode
      //
      if ((*ChannelNvList)[Ch].encodedCSMode == 0) {
        continue;
      }

      // convert logical rank mask to dimm number 0/1 -> 0, 4/5 ->1, 6/7 ->2
      Dimm = Rank / 4;
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

    } //rank

    for (SubCh = 0; SubCh < MaxSubCh; SubCh++) {
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CPGC2_ADDRESS_SIZE_N0_MCCPGC_MAIN_REG, &AddressSize0.Data);
      AddressSize0.Bits.block_size_max_rank = RankCount - 1;
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_ADDRESS_SIZE_N0_MCCPGC_MAIN_REG, &AddressSize0.Data);

      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CPGC2_ADDRESS_SIZE_N1_MCCPGC_MAIN_REG, &AddressSize1.Data);
      AddressSize1.Bits.region_size_max_rank = RankCount - 1;
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_ADDRESS_SIZE_N1_MCCPGC_MAIN_REG, &AddressSize1.Data);
    }
  }

  //
  // SPR_TODO: HSD 1507709040
  // [CPGC] SPR: CPGC rank select functionality shall support 3DS DIMM's subrank selection for signal training
  //

  //
  // RankFeatureEnable is not supported on SPR
  //

  return (UINT8) (1 << Ch);
} // SelectGPGCRanks

/**

  Perform RPQ drain operation

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number
  @param[in] Ch              - Channel number
  @param[in] Dimm            - DIMM number
  @param[in] Rank            - Rank number

  @retval N/A

**/
VOID
RPQDrainCadbMRSMode (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Dimm,
  IN      UINT8     Rank
  )
{

}

/**
  This function evaluates if MRS Stacking is enabled for DDR,
  DDRT or NVDIMMs

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number
  @param[in] Channel - Channel number
  @param[in] Dimm    - DIMM number

  @retval TRUE or FALSE
*/
BOOLEAN
IsMrsStackingEnabled (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm
  )
{
  struct dimmNvram (*DimmNvList) [MAX_DIMM];

  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  if (((FeaturePcdGet (MrsStackingDdr) == FALSE) && !(*DimmNvList) [Dimm].DcpmmPresent && !(*DimmNvList) [Dimm].NvDimmNPresent) ||
      ((FeaturePcdGet (MrsStackingDdrt) == FALSE) && (*DimmNvList) [Dimm].DcpmmPresent) ||
      ((FeaturePcdGet (MrsStackingNvdimm) == FALSE) && (*DimmNvList) [Dimm].NvDimmNPresent)) {
    return FALSE;
  }

  return TRUE;
}

/**

  Returns Sub-channel bitmask for given SubCh number

  @param SubCh        - Sub channel number.
  @retval SubChMask

**/
UINT8
GetSubChMask (
  UINT8     SubCh
  )
{
  UINT8 SubChMask;

  if (SubCh == NO_SUBCH) {
    SubChMask = (1 << CpgcGetValidSubCh ()) - 1;
  } else {
    SubChMask = 1 << SubCh;
  }
  return SubChMask;
}

/**

  This function writes DDR4 command to CADB

  @param[in] Host            - Include all the MRC data
  @param[in] Socket          - Socket number
  @param[in] Ch              - Ch to send command to
  @param[in] Dimm            - DIMM to send command to
  @param[in] Rank            - CS to send the command to
  @param[in] Command         - 0: MRS, 1: REF, 2: PRE, 3: ACT, 4: WR, 5: RD, 6: ZQ, 7: NOP
  @param[in] Bank            - MRS command to be sent
  @param[in] Data            - Value to be sent
  @param[in] Delay           - Delay in Dclocks

@retval UINT32 -  SUCCESS if the reset is succeded

**/
UINT32
WriteCADBCmd (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     Command,
  IN UINT8     Bank,
  IN UINT32    Data,
  IN UINT8     Delay
  )
{
  UINT8                                         CurrentRow;
  UINT32                                        Status = SUCCESS;
  EFI_STATUS                                    CteStatus;
  PatCadb2Prog0                                 PatCADBProg0;
  PatCadb2Prog1                                 PatCADBProg1;
  PatCadb2Prog0                                 DselOfNop0;
  PatCadb2Prog1                                 DselOfNop1;
  struct ddrRank                                (*RankList)[MAX_RANK_DIMM];
  struct channelNvram                           (*ChannelNvList)[MAX_CH];
  UINT32                                        AddressMask;
  INT32                                         FakeMrs = 0;
  CADB_MRSCFG_N0_MCCADB_MAIN_STRUCT             CadbMrscfgN0;
  CADB_CFG_MCCADB_MAIN_STRUCT                   CadbCfg;
  CADB_AO_MRSCFG_MCCADB_MAIN_STRUCT             CadbAoMrscfg;
  CADB_CTL_MCCADB_MAIN_STRUCT                   CadbCtl;
  CADB_MRSCFG_N1_MCCADB_MAIN_STRUCT             CadbMrscfgN1;
  CADB_DSEL_UNISEQ_LMN_MCCADB_MAIN_STRUCT       CadbDselUniseqLmn;
  UINT32                                        Patbuf[CADB_MUX_MAX];
  UINT32                                        MuxCtl[CADB_MUX_MAX];
  UINT32                                        LfsrSize[CADB_MUX_MAX] = {0, 0, 0, 0};

  CalculateAddressMask (Host, Socket, Ch, Dimm, &AddressMask);

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {

    if (MRS_CMD == Command) {

      if (Host->DdrioUltSupport) {

        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
          "ULT MRS%d data 0x%04X\n", Bank, Data);
        CteDdrCmd ("mrs", 1 << Ch, 1, (UINT32) GetPhyRank (Dimm, Rank), (INT32)Bank, (UINT32)Data); //set the sub channel mask as 1 for ddr4.
        return SUCCESS;

      } else {
        Status = SUCCESS;
        CteStatus = CteGetOptions ("fakemrs", &FakeMrs);
        if (EFI_ERROR (CteStatus)) {
          if (EFI_UNSUPPORTED != CteStatus) {
            Status = FAILURE;
          }
          FakeMrs = 0;
        }
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
          "MRS%d data 0x%04X, fake=%d\n", Bank, Data, FakeMrs);

        if (FakeMrs) {
          CteStatus = CteMrsWrite (Ch, Rank, Bank, Data);
          if (CteStatus != EFI_SUCCESS) {
            if (EFI_UNSUPPORTED != CteStatus) {
              Status = FAILURE;
              RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                "WriteCADBCmd: Failure writing CteMrsWrite with status %d\n", CteStatus);
            }
          }
          return Status;
        }
      }
    }
  }

#ifdef MRC_TRACE
  RcDebugPrintWithDevice (SDBG_TRACE, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "WriteCADBCmd Starts: Command 0x%02x, Bank 0x%02x, Data 0x%04x\n", Command, Bank, Data);
#endif // #ifdef MRC_TRACE

  RankList = GetRankNvList (Host, Socket, Ch, Dimm);
  ChannelNvList = GetChannelNvList (Host, Socket);
  CurrentRow = 0;

  ZeroMem (&PatCADBProg0, sizeof (PatCADBProg0));
  ZeroMem (&PatCADBProg1, sizeof (PatCADBProg1));
  ZeroMem (&DselOfNop0, sizeof (PatCADBProg0));
  ZeroMem (&DselOfNop1, sizeof (PatCADBProg1));

  PatCADBProg0.ca = (Data & 0x3FFF); // ca0~ca13
  PatCADBProg1.ca17 = ((Data >> 17) & 0x1); // ca17
  PatCADBProg1.bank    = Bank;
  PatCADBProg0.cid     = 0;

  if (Command == RCW_CMD) {
    PatCADBProg0.cs      = ~((1 << (*RankList)[Rank].CSIndex) + (1 << ((*RankList)[Rank].CSIndex + 1)));
  } else {
    PatCADBProg0.cs      = ~(1 << (*RankList)[Rank].CSIndex);
  }
  if (Command == RCW_CMD) {
    PatCADBProg1.control = 0;
  } else {
    PatCADBProg1.control = Command;
  }

  // zero out CID0/1 as CS's bits
  if ((*ChannelNvList)[Ch].encodedCSMode == 2) {
    PatCADBProg0.cs = PatCADBProg0.cs  & ~(0xc << (Dimm*4));
  }

  PatCADBProg0.odt = 0;
  PatCADBProg0.cke = 0;

  if (ShouldDriveCkeOn (Host, Socket, Ch)) {
    PatCADBProg0.cke = 0xF;
  }

  // Assert ACT bit
  PatCADBProg1.control |= BIT3;

  // Calculate and set parity
  PatCADBProg0.par     = (UINT8) CalculateEvenParity (PatCADBProg0.ca);
  PatCADBProg0.par    ^= (UINT8) CalculateEvenParity (PatCADBProg1.bank & 0xF);
  PatCADBProg0.par    ^= (UINT8) CalculateEvenParity (PatCADBProg1.control & 0xF);
  PatCADBProg0.par    ^= (UINT8) CalculateEvenParity (PatCADBProg1.ca17 & 0x1);

  PatCADBProg0.val     = 1;

  if ((*ChannelNvList) [Ch].timingMode == TIMING_1N) {
    DselOfNop0.ca      = 0;
    DselOfNop0.cid     = 0;
    DselOfNop0.cs      = 0xF;
    DselOfNop0.odt     = 0xF;
    DselOfNop0.cke     = 0xF;
    DselOfNop0.val     = 0;
    DselOfNop1.bank    = Bank;
    DselOfNop1.control = 0xF; // Act, Ras, Cas, and WE are all high
    DselOfNop0.par     = (UINT8) CalculateEvenParity (DselOfNop0.ca);
    DselOfNop0.par    ^= (UINT8) CalculateEvenParity (DselOfNop1.bank & 0xF);
    DselOfNop0.par    ^= (UINT8) CalculateEvenParity (DselOfNop1.control & 0xF);
    DselOfNop0.par    ^= (UINT8) CalculateEvenParity (DselOfNop1.ca17 & 0x1);
    ProgramCADB2 (Host, Socket, Ch, SUB_CH_A_MASK, CurrentRow, ++CurrentRow, CADB_BUF_MODE_DDR4, FALSE, &PatCADBProg0, &PatCADBProg1);
    ProgramCADB2 (Host, Socket, Ch, SUB_CH_A_MASK, CurrentRow, ++CurrentRow, CADB_BUF_MODE_DDR4, FALSE, &DselOfNop0, &DselOfNop1);
  } else {
    DselOfNop0.ca      = PatCADBProg0.ca;
    DselOfNop0.cid     = 0;
    DselOfNop0.cs      = 0xF;
    DselOfNop0.odt     = 0;
    DselOfNop0.cke     = 0xF;
    DselOfNop0.val     = 1;
    DselOfNop1.bank    = Bank;
    DselOfNop1.control = PatCADBProg1.control; // Act, Ras, Cas, and WE are all high
    DselOfNop0.par     = (UINT8) CalculateEvenParity (DselOfNop0.ca);
    DselOfNop0.par    ^= (UINT8) CalculateEvenParity (DselOfNop1.bank & 0xF);
    DselOfNop0.par    ^= (UINT8) CalculateEvenParity (DselOfNop1.control & 0xF);
    DselOfNop0.par    ^= (UINT8) CalculateEvenParity (DselOfNop1.ca17 & 0x1);
    // Swap 0 and 1 contents if you want to have more SETUP time, instead of more HOLD time.
    ProgramCADB2 (Host, Socket, Ch, SUB_CH_A_MASK, CurrentRow, ++CurrentRow, CADB_BUF_MODE_DDR4, FALSE, &DselOfNop0, &DselOfNop1);
    ProgramCADB2 (Host, Socket, Ch, SUB_CH_A_MASK, CurrentRow, ++CurrentRow, CADB_BUF_MODE_DDR4, FALSE, &PatCADBProg0, &PatCADBProg1);
  }

  SetCadbModeEn (Host, Socket, 1 << Ch, TRUE);

  //
  // Configure CADB
  //
  GetSetCadbRegs (Host, Socket, Ch, SUB_CH_A, CPGC_REG_READ, CADB_MRSCFG_N1_MCCADB_MAIN_REG, &CadbMrscfgN1.Data);
  GetSetCadbRegs (Host, Socket, Ch, SUB_CH_A, CPGC_REG_READ, CADB_MRSCFG_N0_MCCADB_MAIN_REG, &CadbMrscfgN0.Data);
  CadbMrscfgN1.Bits.mrs_delay_cnt = 0;
  CadbMrscfgN0.Bits.mrs_cs_mode = ONE_CYCLE_CS;
  CadbMrscfgN0.Bits.mrs_goto_ptr = 0;
  CadbMrscfgN0.Bits.mrs_end_ptr = 0;
  CadbMrscfgN0.Bits.mrs_start_ptr = 0;
  CadbMrscfgN0.Bits.cs_active_polarity = 0;
  CadbMrscfgN0.Bits.setup_clocks = 0;
  CadbMrscfgN0.Bits.dsel_clocks = 0;
  CadbMrscfgN0.Bits.cs_clocks = 0;
  GetSetCadbRegs (Host, Socket, Ch, SUB_CH_A, CPGC_REG_WRITE, CADB_MRSCFG_N1_MCCADB_MAIN_REG, &CadbMrscfgN1.Data);
  GetSetCadbRegs (Host, Socket, Ch, SUB_CH_A, CPGC_REG_WRITE, CADB_MRSCFG_N0_MCCADB_MAIN_REG, &CadbMrscfgN0.Data);

  GetSetCadbRegs (Host, Socket, Ch, SUB_CH_A, CPGC_REG_READ, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);
  CadbCfg.Bits.cadb_mode = CADB_MODE_ALWAYS_ON;
  CadbCfg.Bits.lane_deselect_en = DRIVE_CADB_PATTERN_ON_ALL_PINS;
  CadbCfg.Bits.initial_dsel_en = 1;
  if ((*ChannelNvList) [Ch].timingMode == TIMING_1N) {
    CadbCfg.Bits.cadb_dsel_throttle_mode = 1;
    CadbCfg.Bits.cadb_sel_throttle_mode = 1;
  } else {
    CadbCfg.Bits.cadb_dsel_throttle_mode = 2;
    CadbCfg.Bits.cadb_sel_throttle_mode = 2;
  }
  GetSetCadbRegs (Host, Socket, Ch, SUB_CH_A, CPGC_REG_WRITE, CADB_CFG_MCCADB_MAIN_REG, &CadbCfg.Data);

  GetSetCadbRegs (Host, Socket, Ch, SUB_CH_A, CPGC_REG_READ, CADB_AO_MRSCFG_MCCADB_MAIN_REG, &CadbAoMrscfg.Data);
  CadbAoMrscfg.Bits.mrs_ao_repeats = 1;
  if ((*ChannelNvList) [Ch].timingMode == TIMING_1N) {
    CadbAoMrscfg.Bits.mrs_gap = 0;
  } else {
    CadbAoMrscfg.Bits.mrs_gap = 1;
  }
  CadbAoMrscfg.Bits.mrs_gap_scale = 0;
  GetSetCadbRegs (Host, Socket, Ch, SUB_CH_A, CPGC_REG_WRITE, CADB_AO_MRSCFG_MCCADB_MAIN_REG, &CadbAoMrscfg.Data);

  MuxCtl[0] = BTBUFFER;
  MuxCtl[1] = LMNMode;
  MuxCtl[2] = BTBUFFER;
  MuxCtl[3] = BTBUFFER;
  SetCadbDselUniseqCfg (Host, Socket, Ch, SUB_CH_A, MuxCtl, LfsrSize);

  if ((*ChannelNvList) [Ch].timingMode == TIMING_1N) {
    Patbuf[0] = CADB_BUFFER_PATTERN_10;
    Patbuf[1] = CADB_BUFFER_PATTERN_11;
    Patbuf[2] = CADB_BUFFER_PATTERN_12;
    Patbuf[3] = CPGC_DONT_CARE;
  } else {
    // LMN mode for CS selection - always runs at 1N program to toggle each clock/UI
    GetSetCadbRegs (Host, Socket, Ch, SUB_CH_A, CPGC_REG_READ, CADB_DSEL_UNISEQ_LMN_MCCADB_MAIN_REG, &CadbDselUniseqLmn.Data);
    CadbDselUniseqLmn.Bits.l_sel = 0;
    CadbDselUniseqLmn.Bits.l_cnt = 1;
    CadbDselUniseqLmn.Bits.m_cnt = 1;
    CadbDselUniseqLmn.Bits.n_cnt = 1;
    GetSetCadbRegs (Host, Socket, Ch, SUB_CH_A, CPGC_REG_WRITE, CADB_DSEL_UNISEQ_LMN_MCCADB_MAIN_REG, &CadbDselUniseqLmn.Data);
    Patbuf[0] = CPGC_DONT_CARE;
    Patbuf[1] = CADB_BUFFER_PATTERN_10;
    Patbuf[2] = 0;
    Patbuf[3] = CPGC_DONT_CARE;
  }
  SetCadbDselUniseqPat (Host, Socket, Ch, SUB_CH_A, Patbuf);

  //
  // Delay if this is a CTE build, otherwise do nothing.
  //
  CteDelayDclk (50);

  //
  // Start test. SPR_TODO. HSD 1507158123: Update the ExecuteCmdSignalTest to support SubChMask parameter, then replace it by ExecuteCmdSignalTest.
  //
  Status = CadbTestStart (Host, Socket, (1 << Ch), SUB_CH_A_MASK);

  //
  // Poll test completion
  //
  CadbPollTestDone (Host, Socket, 1 << Ch, SUB_CH_A_MASK);
  //
  // Stop the test
  //
  CadbCtl.Data = 0;
  CadbCtl.Bits.start_test = 0;
  CadbCtl.Bits.stop_test = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CADB (SUB_CH_A, CADB_CTL_MCCADB_MAIN_REG), CadbCtl.Data);

  //
  // Disable CADB Mode
  //
  SetCadbModeEn (Host, Socket, 1 << Ch, FALSE);
  //
  // Delay if this is a CTE build, otherwise do nothing.
  //

  CteDelayDclk (10);

  return Status;
} // WriteCADBCmd


/**

Routine Description:

  This function allows stacking of MRS commands in cadb buffer in WriteCADBCmd  function. Stacking commands in CADB
  is more efficient as the MRS are only ~32 clks apart instead of spread out by a bunch of CSR accesses

Arguments:
  Host            - include all the MRC data
  socket          - Memory Controller
  ch              - ch to send command to
  dimm            - dimm to send command to
  Command         - NORMAL_MRS  - How WriteCADBCmd  usually works
                  - LOAD_MRS - WriteCADBCmd  will load commands into cadb buffer but not execute
                  - EXECUTE_MRS - WriteCADBCmd  will execute the stack of commands


Returns:
 UINT32 -  SUCCESS if the reset is succeded

--*/
VOID
MRSCmdStacking (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     Command
  )
{
  if (IsMrsStackingEnabled (Host, socket, ch, dimm)) {
    Host->var.mem.socket[socket].cadbMRSMode = Command;

    if (Command == LOAD_MRS) {
      Host->var.mem.socket[socket].cadbMRSIndex[ch]=0;
    }
    if (Command == EXECUTE_MRS) {
      if(Host->var.mem.socket[socket].cadbMRSIndex[ch] != 0) {  //Check for exactly 16 CADB commands
        //WriteCADBCmd (Host, socket, ch, dimm, rank, Command, Bank, Data, Delay);
        WriteCADBCmd (Host, socket, ch, dimm, 0, 0, 0, 0, 6); //Delay =2**6 Need to support NVMCTLR
      }
      Host->var.mem.socket[socket].cadbMRSMode = NORMAL_MRS;                 // return to normal MRS mode
      Host->var.mem.socket[socket].cadbMRSIndex[ch]=0;                           // reset index after execute
    }
  }
} //MRSCmdStacking10nm

/**

  this function writes to CADB

  @param Host            - include all the MRC data
  @param socket          - Memory Controller
  @param ch              - ch to send command to
  @param dimm            - dimm to send command to
  @param rank            - CS to send the command to
  @param Command         - 0: MRS, 1: REF, 2: PRE, 3: ACT, 4: WR, 5: RD, 6: ZQ, 7: NOP
  @param Bank            - MRS command to be sent
  @param Data            - Value to be sent

**/
VOID
WriteMRSPDA (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     dram,
  UINT16    data,
  UINT8     bank
  )
{

} // WriteMRSPDA10nm

VOID
WriteMR6 (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT16    data,
  UINT8     bank
  )
/*++

this function writes to CADB

@param Host            - include all the MRC data
@param socket          - Memory Controller
@param ch              - ch to send command to
@param dimm            - dimm to send command to
@param rank            - CS to send the command to
@param Bank            - MRS command to be sent
@param Data            - Value to be sent

--*/
{

} // WriteMR610nm


#ifdef LRDIMM_SUPPORT

STATIC INT16
GetMinDwl (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm
  )
{
  INT16                                   cwlAdj;

  cwlAdj = -3; // write absolute minimum CWL_ADJ of -3

  return cwlAdj;
}

VOID
WritePBA (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     Dimm,
  UINT8     rank,
  UINT8     *controlWordDataPtr,
  UINT8     controlWordFunc,
  UINT8     controlWordAddr,
  UINT8     bufferAddr
  )
/*++

Routine Description:

  this function writes to CADB

Arguments:
  Host               - include all the MRC data
  socket             - Memory Controller
  ch                 - ch to send command to
  dimm               - dimm to send command to
  rank               - CS to send the command to
  controlWordDataPtr - Pointer to a 9 byte array of control word data
  controlWordFunc    - Function index in the LR buffer
  controlWordAddr    - Control Word Address
  bufferAddr         - Address of the individual buffer to target...or 'FF' for all buffers

Returns:
  N/A

--*/
{

} // WritePBA10nm
#endif //LRDIMM_SUPPORT

/**

  this function write the MR2 register for all the ranks

  @param Host            - sysHost data structure
  @param socket          - Processor socket
  @param ch              - Channel to issue the ZQ to
  @param zqType          - Type of ZQ Calibration: Long or Short

@retval UINT32 - SUCCESS if the reset is succeded

**/
UINT32
CpgcIssueZQ (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     zqType
  )
{
  return EFI_SUCCESS;
} // CpgcIssueZQ10nm

/**

  DDR advanced training CPGC setup

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket to train
  @param[in] DdrChEnMap  - 4-bit bit mask of ddr channels to test - (1 = channel is selected to run tests)
  @param[in] Dimm        - Target dimm
  @param[in] Rank        - logical rank number (0-7)
  @param[in] SubRank     - subrank number
  @param[in] Pattern     - ddr pattern (0-3) ( 0 = dq, 1 = cmd group 1, 2 = cmd group 2, 3 = cmd group 3)
  @param[in] DqLoopCount - dq sequence loop count - number of times dq sequence is executed (2^(dqLoopCount - 1))

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcAdvTrainingSetup (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    DdrChEnMap,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     SubRank,
  IN UINT32    Pattern,
  IN UINT8     DqLoopCount
  )
{
  UINT8               Ch;
  UINT32              Status;
  UINT8               MaxChDdr;

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, NO_CH, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                 "CpgcAdvTrainingSetup: ChEnMap = 0x%x, pattern = %d\n", DdrChEnMap, Pattern);

  Status = SUCCESS;
  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {

    // Skip ddr channels not selected or disabled
    if ((!(DdrChEnMap & (1 << Ch))) ||
       (!IsChannelEnabled (Socket, Ch))) {
      continue;
    }

    switch (Pattern & DDR_CPGC_PATTERN_MASK) {
      case DDR_CPGC_DQ_PATTERN:
        CpgcAdvDqSetup (Host, Socket, Ch, Rank, SubRank, DqLoopCount);
        break;
      case DDR_CPGC_ADV_CMD_PATTERN:
        CpgcAdvCmdPatternSetup (Host, Socket, Ch, Dimm, Rank, DqLoopCount);
        break;
      default:
        RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_47);
        Status = FAILURE;
    }
  }

  return Status;
} // CpgcAdvTrainingSetup

/**

  Select another rank for testing with the same CPGC setup

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket to train
  @param[in] DdrChEnMap  - 4-bit bit mask of ddr channels to test - (1 = channel is selected to run tests)
  @param[in] Dimm        - Target dimm
  @param[in] Rank        - logical rank number (0-7)
  @param[in] SubRank     - sub-rank number
  @param[in] Pattern     - ddr pattern (0-3) ( 0 = dq, 1 = cmd group 1, 2 = cmd group 2, 3 = cmd group 3)

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcAdvTrainingModifyRank (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    DdrChEnMap,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     SubRank,
  IN UINT32    Pattern
  )
{
  UINT8               Ch;
  MRC_MST             MemSsType;
  UINT32              Status;
  struct channelNvram (*ChannelNvList) [MAX_CH];

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                  "CpgcAdvTrainingModifyRank: ChEnMap = 0x%x, pattern = %d\n", DdrChEnMap, Pattern);

  Status = SUCCESS;
  ChannelNvList = GetChannelNvList (Host, Socket);
  MemSsType = GetSysCpuCsrAccessVar()->MemSsType[Socket];

  for (Ch = 0; Ch < GetMaxCh (Host, Socket, MemSsType); Ch++) {
    if ((!(DdrChEnMap & (1 << Ch))) ||
      (((*ChannelNvList) [Ch].enabled == 0)))  {
      continue;
    }

    switch (Pattern & DDR_CPGC_PATTERN_MASK) {
      case DDR_CPGC_DQ_PATTERN:
        SelectCPGCRanks (Host, Socket, Ch, 1 << Rank, 0, SubRank);
        break;
      case DDR_CPGC_ADV_CMD_PATTERN:
        SelectCPGCRanks (Host, Socket, Ch, 1 << Rank, 0, 0);
        break;
      default:
        RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_47);
    }
  }

  return Status;
} // CpgcAdvTrainingModifyRank

/**

  DDR DQ victim aggressor CPGC setup.

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket to train
  @param[in] Ch          - Ddr channel to setup
  @param[in] LogRank     - Logical rank number
  @param[in] SubRank     - Subrank number
  @param[in] DqLoopCount - Dq sequence loop count - number of times dq sequence is executed (2^(dqLoopCount - 1))

  @retval SUCCESS / FAILURE

**/
STATIC UINT32
CpgcAdvDqSetup (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     LogRank,
  IN UINT8     SubRank,
  IN UINT32    DqLoopCount
  )
{
  MRC_MST                           MemSsType;
  MRC_RT                            MrcRankList;
  MRC_TT                            CurrentTestType;

#ifdef SPR_PO_FLAG
  BOOLEAN                           Change = FALSE;
#endif //SPR_PO_FLAG

  MemSsType = GetSysCpuCsrAccessVar()->MemSsType[Socket];
  CurrentTestType = GetCurrentTestType (Socket);

  MrcRankList.Dimm = LogRank / MAX_RANK_DIMM;  // Dimm number
  MrcRankList.Rank = LogRank % MAX_RANK_DIMM;  // Physical rank number
  //
  // Program logical to physical rank mapping
  //
  SelectCPGCRanks (Host, Socket, Ch, 1 << LogRank, 0, SubRank);
  if ((CurrentTestType == WritePreDfe2DCenteringTest) ||
      (CurrentTestType == WritePostDfe2DCenteringTest) ||
      (CurrentTestType == ReadPreDfe2DCenteringTest) ||
      (CurrentTestType == ReadPostDfe2DCenteringTest) ||
      (CurrentTestType == DramDutyCycleAdjusterTest) ||
      (CurrentTestType == DqSlewRateTest) ||
      (CurrentTestType == WriteDfeTest) ||
      (CurrentTestType == ReadDfeTest)) {
#ifdef SPR_PO_FLAG
        if ((GetCurrentTestType (Socket) == WriteDfeTest)) {
          Change = TRUE;
          SetCurrentTestType (Host->var.mem.currentSocket, RmtDqTest);
          //
          // Once this line PatternLengthToCpgcLoopCnt (64) is fixed in HSD 1507953729, the below line can be removed and get the value from the parameter
          //
          DqLoopCount = CPGC_LOOP_COUNT_65536;
        }
#endif //SPR_PO_FLAG
        SetupTest (Host, Socket, MemSsType, 1 << Ch, 1, &MrcRankList, DdrLevel, GetCurrentTestType (Socket), gsmCsnDelim, DqLoopCount, FIRST_ITERATION_LOOP);
#ifdef SPR_PO_FLAG
        if (Change == TRUE) {
          SetCurrentTestType (Host->var.mem.currentSocket, WriteDfeTest);
        }
#endif //SPR_PO_FLAG
      } else {
        SetupTest (Host, Socket, MemSsType, 1 << Ch, 1, &MrcRankList, DdrLevel, RmtDqTest, gsmCsnDelim, DqLoopCount, FIRST_ITERATION_LOOP);
      }

  return SUCCESS;
} // CpgcAdvDqSetup

/**

  DDR Advanced CMD Pattern CPGC setup.

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket to train
  @param[in] Ch          - Ddr channel to setup
  @param[in] Dimm        - Target dimm
  @param[in] Rank        - Logical rank number
  @param[in] DqLoopCount - Dq sequence loop count - number of times dq sequence is executed (2^(dqLoopCount - 1))

  @retval SUCCESS / FAILURE

**/
STATIC UINT32
CpgcAdvCmdPatternSetup (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT8     Rank,
  UINT8     DqLoopCount
  )
{
  MRC_MST                           MemSsType;
  MRC_RT                            MrcRankList;

  MemSsType = GetSysCpuCsrAccessVar()->MemSsType[Socket];

  MrcRankList.Dimm = Rank / MAX_RANK_DIMM;  // Dimm number
  MrcRankList.Rank = Rank % MAX_RANK_DIMM;  // Physical rank number
  //
  // Program logical to physical rank mapping
  //
  SelectCPGCRanks (Host, Socket, Ch, 1 << Rank, 0, 0);

  SetupTest (Host, Socket, MemSsType, 1 << Ch, 1, &MrcRankList, DdrLevel, RmtCmdTest, gsmCsnDelim, DqLoopCount, FIRST_ITERATION_LOOP);

  return SUCCESS;
} // CpgcAdvCmdPatternSetup

/**

  Enable deselect patterns

  @param Host        - Pointer to sysHost
  @param socket      - Socket to train
  @param ddrChEnMap  - DDR channels to setup
  @param Dimm        - Dimm to train
  @param mode        - MODE_DESELECT
                     - MODE_VA_DESELECT

  @retval SUCCESS / FAILURE

**/
VOID
EnableCADBDeselect (
                   PSYSHOST  Host,
                   UINT8     socket,
                   UINT32    ddrChEnMap,
                   UINT8     Dimm,
                   UINT16    mode
                   )
{

} // EnableCADBDeselect

/**

  DDR CPGC fill wdb for dq/va pattern

  @param Host    - Pointer to sysHost
  @param socket  - Socket to train
  @param ch      - ddr channel to setup

  @retval SUCCESS / FAILURE

**/
STATIC UINT32
CpgcFillWdbDqVa (
                PSYSHOST  Host,
                UINT8     socket,
                UINT8     ch,
                UINT8     mode
                )
{

  return EFI_SUCCESS;
} // CpgcFillWdbDqVa


/**

  CPGC setup per rank for Advanced Memory test

  @param[in] Host              - Pointer to sysHost
  @param[in] Socket            - Memory controller to train
  @param[in] DdrChEnMap        - ddr channels to setup
  @param[in] LogRank           - logical rank to setup
  @param[in] LogSubRank        - logical subrank number to test
  @param[in] ColAddressBits    - number of DRAM column address bits to test
  @param[in] RowAddressBits    - number of DRAM row address bits to test
  @param[in] BankAddressBits   - number of DRAM bank address bits to test
  @param[in] Mode              - Type of sequence MT_CPGC_WRITE, MT_CPGC_READ, or MT_CPGC_READ_WRITE
  @param[in] AddressMode       - FAST_Y for column first, FAST_X for row first
  @param[in] Direction         - Sequential address direction MT_ADDR_DIR_UP, MT_ADDR_DIR_DN
  @param[in] Bank              - Base bank address number to test
  @param[in] BaseBankBits      - Number of bank address bits to skip on increment (for BG interleave)
  @param[in] RowAddr[MAX_CH]   - Base row address to start testing
  @param[in] RowSize[MAX_CH]   - Number of rows to test
  @param[in] DeviceMask[MAX_CH][3] - Bitmask of DQ lanes to not test
  @param[in] LineMask[MAX_CH]      - Each bit corresponds to a cache line; 1 means test, 0 do not test
  @param[in] StopOnErr[MAX_CH]     - 1 = stop on 1st error; 0 = stop on all DQ lanes error
  @param[in] SeqDataInv[MT_MAX_SUBSEQ] - Enables pattern inversion per subsequence

  @retval None

**/
VOID
CpgcMemTestRankSetupMATSRowRange (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    DdrChEnMap,
  IN UINT8     LogRank,
  IN UINT8     LogSubRank,
  IN UINT8     ColAddressBits[MAX_CH],
  IN UINT8     RowAddressBits[MAX_CH],
  IN UINT8     BankAddressBits[MAX_CH],
  IN UINT8     Mode,
  IN UINT8     AddressMode,
  IN UINT8     Direction,
  IN UINT8     Bank,
  IN UINT8     BaseBankBits,
  IN UINT32    RowAddr[MAX_CH],
  IN UINT32    RowSize[MAX_CH],
  IN UINT32    DeviceMask[MAX_CH][3],
  IN UINT8     LineMask[MAX_CH],
  IN UINT8     StopOnErr[MAX_CH],
  IN UINT8     SeqDataInv[MT_MAX_SUBSEQ]
  )
{
  UINT8                                           Index;
  UINT8                                           SizeFound;
  UINT8                                           Ch;
  UINT8                                           SubCh;
  UINT8                                           CpgcRowAddressBits;
  UINT8                                           CpgcColAddressBits;
  UINT8                                           CpgcBankAddressBits;
  UINT32                                          LoopCount = 0;
  INT32                                           MatsBurstBits = 0;
  struct channelNvram                             (*ChannelNvList)[MAX_CH];
  UINT8                                           MaxChDdr;
  CPGC_ERR_CTL_MCCPGC_MAIN_STRUCT                 ErrCtl;
  CPGC2_ADDRESS_SIZE_N0_MCCPGC_MAIN_STRUCT        AddressSize0;
  CPGC2_ADDRESS_SIZE_N1_MCCPGC_MAIN_STRUCT        AddressSize1;
  CPGC2_REGION_LOW_N0_MCCPGC_MAIN_STRUCT          RegionLow0;
  CPGC2_REGION_LOW_N1_MCCPGC_MAIN_STRUCT          RegionLow1;
  CPGC2_BASE_REPEATS_MCCPGC_MAIN_STRUCT           BaseRepeats;
  CPGC2_BLOCK_REPEATS_MCCPGC_MAIN_STRUCT          BlockRepeats;
  CPGC2_DATA_CONTROL_MCCPGC_MAIN_STRUCT           DataControl;
  CPGC2_BASE_COL_REPEATS_MCCPGC_MAIN_STRUCT       BaseColRepeats;
  CPGC2_BASE_ADDRESS_CONTROL_MCCPGC_MAIN_STRUCT   BaseAddressControl;
  CPGC_SEQ_BANK_L2P_MAPPING_A_MCCPGC_MAIN_STRUCT  SeqBankL2pMappingA;
  CPGC_SEQ_BANK_L2P_MAPPING_B_MCCPGC_MAIN_STRUCT  SeqBankL2pMappingB;
  CPGC_SEQ_BANK_L2P_MAPPING_C_MCCPGC_MAIN_STRUCT  SeqBankL2pMappingC;
  CPGC_SEQ_BANK_L2P_MAPPING_D_MCCPGC_MAIN_STRUCT  SeqBankL2pMappingD;
  CPGC_SEQ_BANK_L2P_MAPPING_E_MCCPGC_MAIN_STRUCT  SeqBankL2pMappingE;
  CPGC_SEQ_BANK_L2P_MAPPING_F_MCCPGC_MAIN_STRUCT  SeqBankL2pMappingF;

  MaxChDdr      = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList(Host, Socket);

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, NO_CH, NO_DIMM, NO_SUBCH, LogRank, NO_STROBE, NO_BIT,
                 "CpgcMemTestRankSetupMATSRowRange: ddrChEnMap = 0x%x\n", DdrChEnMap);

  //
  // CPGC setup
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    //
    // skip ddr channels not selected or disabled ddr channels
    // Must disable global start so start test does not activate cpgc on disabled channel for this rank
    //
    if (!(DdrChEnMap & (1 << Ch)) ||
        (RowSize[Ch] == 0) ||
        (Host->nvram.mem.socket[Socket].channelList[Ch].enabled == 0) ||
        (((*ChannelNvList)[Ch].encodedCSMode == 0) && (LogSubRank != 0))) {
        CpgcDisableGlobalStart (Host, Socket, Ch);
        continue;
    }

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, LogRank, NO_STROBE, NO_BIT,
                   "colBits = %d, rowBits = %d, bankBits = %d, bank = %2d, row = 0x%08x, size = 0x%08x, dir = %d, devMask = 0x%02x%08x%08x, lineMask = 0x%02x\n",
                   ColAddressBits[Ch], RowAddressBits[Ch], BankAddressBits[Ch], Bank, RowAddr[Ch], RowSize[Ch], Direction,
                   DeviceMask[Ch][2], DeviceMask[Ch][1], DeviceMask[Ch][0], LineMask[Ch]);
    //
    // Programs GPGC to run on the selected physical ranks
    //
    SelectCPGCRanks (Host, Socket, Ch, 1 << LogRank, BIT3 | BIT1, LogSubRank);
    ClearDdrtForceMode (Socket, Ch);

    //
    //Assert that rowSize[ch] is 2^n
    //
    SizeFound = 0;
    CpgcRowAddressBits = 0;
    for (Index = 0; Index< 32; Index++) {
      if (RowSize[Ch] & (1 << Index)) {
        SizeFound++;
        CpgcRowAddressBits = Index;
      }
    }
    RC_FATAL_ERROR (SizeFound < 2, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_153);

    //
    // DDR5 Burst Length is 16
    //
    CpgcColAddressBits = ColAddressBits[Ch] - Log2x32 (BURST_LENGTH);

    CpgcBankAddressBits = BankAddressBits[Ch];

    MatsBurstBits = CpgcColAddressBits + (CpgcBankAddressBits - BaseBankBits);
    LoopCount = RowSize[Ch];

    for (SubCh = 0; SubCh < CpgcGetValidSubCh (); SubCh++) {

      //
      // Program error mask registers
      // 0 = no mask
      // 0xF = mask error checking on the DQ nibble
      //
      // Lanes [31:0] error checking enabled
      //
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_LNEN_LO_MCCPGC_MAIN_REG), DeviceMask[Ch][0]);
      //
      // Check Enable mask for UI[15:0] -for x32 bus this register is repurposed as chunk/UI enable
      // 16 chunks per CL
      //
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_LNEN_HI_MCCPGC_MAIN_REG), 0xFFFF);

      //
      // Do not check ECC bits if ECC is disabled
      //
      if (Host->nvram.mem.eccEn) {
        MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_XLNEN_MCCPGC_MAIN_REG), DeviceMask[Ch][2]);
      } else {
        MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_XLNEN_MCCPGC_MAIN_REG), 0xFF);
      }

      //
      // Set cache line mask and stop on error control
      //
      ErrCtl.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_CTL_MCCPGC_MAIN_REG));
      if (StopOnErr[Ch]) {
        ErrCtl.Bits.stop_on_error_ctl = 1; // Stop on first error
      } else {
        ErrCtl.Bits.stop_on_error_ctl = CPGC_ERR_STOP_ON_ALL_ERR; // Stop on all lane error
      }
      ErrCtl.Bits.errchk_mask_cacheline = LineMask[Ch];
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_CTL_MCCPGC_MAIN_REG), ErrCtl.Data);

      AddressSize0.Data = 0;
      AddressSize0.Bits.block_size_max_rank = 0; // 1 rank per test
      AddressSize0.Bits.block_size_bits_col = CpgcColAddressBits;// 2**N
      AddressSize0.Bits.block_size_bits_row = CpgcRowAddressBits;// 2**N
      AddressSize0.Bits.block_size_max_bank = (1 << (CpgcBankAddressBits - BaseBankBits)) - 1; // total logic banks
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_ADDRESS_SIZE_N0_MCCPGC_MAIN_REG, &AddressSize0.Data);

      AddressSize1.Data = AddressSize0.Data;
      AddressSize1.Bits.bl_chop_en = 0; // No half-line requests
      AddressSize1.Bits.request_data_size = 1;  // 64B transactions
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_ADDRESS_SIZE_N1_MCCPGC_MAIN_REG, &AddressSize1.Data);

      RegionLow0.Data = 0;
      RegionLow1.Data = 0;
      RegionLow0.Bits.low_rank = 0;
      RegionLow0.Bits.low_col  = 0;
      RegionLow1.Bits.low_row  = RowAddr[Ch];
      RegionLow1.Bits.low_bank = 0; // logic bank0 maps to target bank

      if (GetInjectErrorMATS(Host, Ch)) {
        RegionLow0.Bits.low_col  = 0x20;
        RegionLow1.Bits.low_bank = 0;
      }
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_REGION_LOW_N0_MCCPGC_MAIN_REG, &RegionLow0.Data);
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_REGION_LOW_N1_MCCPGC_MAIN_REG, &RegionLow1.Data);

      BaseRepeats.Data = 0;
      if (GetInjectErrorMATS (Host, Ch)) {
        BaseRepeats.Bits.base_repeats = 0; // 1 time
      } else {
        BaseRepeats.Bits.base_repeats = (1 << (MatsBurstBits + CpgcRowAddressBits)) - 1; // N-1, (colomn bits + highest bank bits + row bits - 1)
      }
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_BASE_REPEATS_MCCPGC_MAIN_REG, &BaseRepeats.Data);

      BlockRepeats.Data = 0;
      BlockRepeats.Bits.block_repeats = 0; // 1 repeat
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_BLOCK_REPEATS_MCCPGC_MAIN_REG, &BlockRepeats.Data);

      DataControl.Data = 0;
      DataControl.Bits.data_select_rotation_repeats =0;
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_DATA_CONTROL_MCCPGC_MAIN_REG, &DataControl.Data);

      BaseColRepeats.Data = 0;
      BaseColRepeats.Bits.base_col_repeats =0;
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_BASE_COL_REPEATS_MCCPGC_MAIN_REG, &BaseColRepeats.Data);

      //
      // Used for bank interleave
      //
      SeqBankL2pMappingA.Data = 0;
      SeqBankL2pMappingB.Data = 0;
      SeqBankL2pMappingC.Data = 0;
      SeqBankL2pMappingD.Data = 0;
      SeqBankL2pMappingE.Data = 0;
      SeqBankL2pMappingF.Data = 0;
      SeqBankL2pMappingA.Bits.l2p_bank0_mapping = Bank;
      switch (CpgcBankAddressBits - BaseBankBits) {
        case 0:
          //
          // no bank interleave
          //
        break;
        case 1:
          //
          // use highest one bank group bit for interleave
          //
          SeqBankL2pMappingA.Bits.l2p_bank1_mapping = (1 << BaseBankBits) + Bank;
        break;
        case 2:
          //
          // use highest two bank group bits for interleave
          //
          SeqBankL2pMappingA.Bits.l2p_bank1_mapping = Bank + (1 << BaseBankBits);
          SeqBankL2pMappingA.Bits.l2p_bank2_mapping = Bank + (1 << BaseBankBits) * 2;
          SeqBankL2pMappingA.Bits.l2p_bank3_mapping = Bank + (1 << BaseBankBits) * 3;
        break;
        case 3:
          //
          // use highest three bank group bits for interleave
          //
          SeqBankL2pMappingA.Bits.l2p_bank1_mapping = Bank + (1 << BaseBankBits);
          SeqBankL2pMappingA.Bits.l2p_bank2_mapping = Bank + (1 << BaseBankBits) * 2;
          SeqBankL2pMappingA.Bits.l2p_bank3_mapping = Bank + (1 << BaseBankBits) * 3;
          SeqBankL2pMappingA.Bits.l2p_bank4_mapping = Bank + (1 << BaseBankBits) * 4;
          SeqBankL2pMappingA.Bits.l2p_bank5_mapping = Bank + (1 << BaseBankBits) * 5;
          SeqBankL2pMappingB.Bits.l2p_bank6_mapping = Bank + (1 << BaseBankBits) * 6;
          SeqBankL2pMappingB.Bits.l2p_bank7_mapping = Bank + (1 << BaseBankBits) * 7;
          break;
        case 5:
          //
          // use highest three bank group bits for interleave
          //
          SeqBankL2pMappingA.Bits.l2p_bank1_mapping = Bank + (1 << 2) * 1;
          SeqBankL2pMappingA.Bits.l2p_bank2_mapping = Bank + (1 << 2) * 2;
          SeqBankL2pMappingA.Bits.l2p_bank3_mapping = Bank + (1 << 2) * 3;
          SeqBankL2pMappingA.Bits.l2p_bank4_mapping = Bank + (1 << 2) * 4;
          SeqBankL2pMappingA.Bits.l2p_bank5_mapping = Bank + (1 << 2) * 5;

          SeqBankL2pMappingB.Bits.l2p_bank6_mapping = Bank + (1 << 2) * 6;
          SeqBankL2pMappingB.Bits.l2p_bank7_mapping = Bank + (1 << 2) * 7;
          SeqBankL2pMappingB.Bits.l2p_bank8_mapping = Bank + 1 + (1 << 2) * 0;
          SeqBankL2pMappingB.Bits.l2p_bank9_mapping = Bank + 1 + (1 << 2) * 1;
          SeqBankL2pMappingB.Bits.l2p_bank10_mapping = Bank + 1 + (1 << 2) * 2;
          SeqBankL2pMappingB.Bits.l2p_bank11_mapping = Bank + 1 + (1 << 2) * 3;

          SeqBankL2pMappingC.Bits.l2p_bank12_mapping = Bank + 1 + (1 << 2) * 4;
          SeqBankL2pMappingC.Bits.l2p_bank13_mapping = Bank + 1 + (1 << 2) * 5;
          SeqBankL2pMappingC.Bits.l2p_bank14_mapping = Bank + 1 + (1 << 2) * 6;
          SeqBankL2pMappingC.Bits.l2p_bank15_mapping = Bank + 1 + (1 << 2) * 7;
          SeqBankL2pMappingC.Bits.l2p_bank16_mapping = Bank + 2 + (1 << 2) * 0;
          SeqBankL2pMappingC.Bits.l2p_bank17_mapping = Bank + 2 + (1 << 2) * 1;

          SeqBankL2pMappingD.Bits.l2p_bank18_mapping = Bank + 2 + (1 << 2) * 2;
          SeqBankL2pMappingD.Bits.l2p_bank19_mapping = Bank + 2 + (1 << 2) * 3;
          SeqBankL2pMappingD.Bits.l2p_bank20_mapping = Bank + 2 + (1 << 2) * 4;
          SeqBankL2pMappingD.Bits.l2p_bank21_mapping = Bank + 2 + (1 << 2) * 5;
          SeqBankL2pMappingD.Bits.l2p_bank22_mapping = Bank + 2 + (1 << 2) * 6;
          SeqBankL2pMappingD.Bits.l2p_bank23_mapping = Bank + 2 + (1 << 2) * 7;

          SeqBankL2pMappingE.Bits.l2p_bank24_mapping = Bank + 3 + (1 << 2) * 0;
          SeqBankL2pMappingE.Bits.l2p_bank25_mapping = Bank + 3 + (1 << 2) * 1;
          SeqBankL2pMappingE.Bits.l2p_bank26_mapping = Bank + 3 + (1 << 2) * 2;
          SeqBankL2pMappingE.Bits.l2p_bank27_mapping = Bank + 3 + (1 << 2) * 3;
          SeqBankL2pMappingE.Bits.l2p_bank28_mapping = Bank + 3 + (1 << 2) * 4;
          SeqBankL2pMappingE.Bits.l2p_bank29_mapping = Bank + 3 + (1 << 2) * 5;

          SeqBankL2pMappingF.Bits.l2p_bank30_mapping = Bank + 3 + (1 << 2) * 6;
          SeqBankL2pMappingF.Bits.l2p_bank31_mapping = Bank + 3 + (1 << 2) * 7;

          break;
        default:
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, LogRank, NO_STROBE, NO_BIT,
                 "Warning: Incorect bank interleave for memory test! Total Bank Bits = %d, Base Bank Bits = %d. No bank interleave used for test.\n",
                 CpgcBankAddressBits, BaseBankBits);
        break;
      }
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_BANK_L2P_MAPPING_A_MCCPGC_MAIN_REG), SeqBankL2pMappingA.Data);
      if ((CpgcBankAddressBits - BaseBankBits) > 2) {
        MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_BANK_L2P_MAPPING_B_MCCPGC_MAIN_REG), SeqBankL2pMappingB.Data);
      }
      if ((CpgcBankAddressBits - BaseBankBits) > 4) {
        MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_BANK_L2P_MAPPING_C_MCCPGC_MAIN_REG), SeqBankL2pMappingC.Data);
        MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_BANK_L2P_MAPPING_D_MCCPGC_MAIN_REG), SeqBankL2pMappingD.Data);
        MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_BANK_L2P_MAPPING_E_MCCPGC_MAIN_REG), SeqBankL2pMappingE.Data);
        MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_BANK_L2P_MAPPING_F_MCCPGC_MAIN_REG), SeqBankL2pMappingF.Data);
      }

      BaseAddressControl.Data = 0;
      BaseAddressControl.Bits.col_inc = 0;
      BaseAddressControl.Bits.row_inc = 0;
      BaseAddressControl.Bits.bank_inc = 0; // increase by 1 logic bank under l2p_bank0/1_mapping setting
      BaseAddressControl.Bits.reserved_rank_inc = 0;
      BaseAddressControl.Bits.address_order0_inc_rate =0;
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_BASE_ADDRESS_CONTROL_MCCPGC_MAIN_REG, &BaseAddressControl.Data);
    } // SubCh loop
  } // Ch

  return;
} // CpgcMemTestRankSetupMATSRowRange

/**

  CPGC WDB setup for Advanced memory test

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Memory controller to train
  @param[in] Ch               - ddr channel to setup
  @param[in] WdbIncrRate      - increment rate
  @param[in] WdbIncrScale     - increment scale
  @param[in] WdbStartPntr     - start pointer
  @param[in] WdbEndPntr       - end pointer

  @retval None

**/
STATIC VOID
CpgcSetupWdbMATS (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     WdbIncrRate,
  IN UINT8     WdbIncrScale,
  IN UINT8     WdbStartPntr,
  IN UINT8     WdbEndPntr
  )
{
  CPGC_DPAT_CFG_MCCPGC_MAIN_STRUCT                       DpatCfg;
  CPGC_DPAT_BUF_CFG_MCCPGC_MAIN_STRUCT                   DpatBufCfg;
  UINT8                                                  SubCh;
  UINT32                                                 Patbuf[CPGC_MAX_NUMBER_DPAT_UNISEQ] = {SEQUENTIAL_BUFFER_PATTERN_0,
                                                                                                SEQUENTIAL_BUFFER_PATTERN_1,
                                                                                                SEQUENTIAL_BUFFER_PATTERN_2};
  UINT32                                                 LfsrSize[CPGC_MAX_NUMBER_DPAT_UNISEQ] = {0, 0, 0};
  UINT32                                                 MuxCtl[CPGC_MAX_NUMBER_DPAT_UNISEQ] = {BTBUFFER, BTBUFFER, BTBUFFER};

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "CpgcSetupWdbMATS\n");


  DpatCfg.Data = 0;
  if (Host->nvram.mem.eccEn) {
    DpatCfg.Bits.ecc_disable = 0;
    DpatCfg.Bits.ecc_replace_byte_control = 1;
    DpatCfg.Bits.ecc_data_source_sel = 0; // Use byte 0
  } else {
    DpatCfg.Bits.ecc_disable = 1;
    DpatCfg.Bits.ecc_replace_byte_control = 0;
    DpatCfg.Bits.ecc_data_source_sel = 0; // Don't care
  }
  DpatCfg.Bits.reload_lfsr_seed_rate = 0; // No reload
  DpatCfg.Bits.save_lfsr_seed_rate = 0; // No save
  DpatCfg.Bits.lane_rotate_rate = 0; // No rotation
  DpatCfg.Bits.read_address_as_data = 0;
  DpatCfg.Bits.write_address_as_data = 0;

  DpatBufCfg.Data = 0;
  DpatBufCfg.Bits.buf_end_pntr = WdbEndPntr;
  DpatBufCfg.Bits.buf_strt_pntr = WdbStartPntr;
  DpatBufCfg.Bits.buf_pntr_inc_scale = WdbIncrScale;
  DpatBufCfg.Bits.buf_pntr_inc_rate = WdbIncrRate;

  for (SubCh = 0; SubCh < CpgcGetValidSubCh (); SubCh++) {
    SetCpgcDpatUniseqCfg (Host, Socket, Ch, SubCh, MuxCtl, LfsrSize);
    GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC_DPAT_CFG_MCCPGC_MAIN_REG, &DpatCfg.Data);
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_DPAT_BUF_CFG_MCCPGC_MAIN_REG), DpatBufCfg.Data);
    SetCpgcDpatUniseqPat (Host, Socket, Ch, SubCh, Patbuf);
  }

  return;
} // CpgcSetupWdbMATS

/**

  CPGC global setup for Advanced memory test

  @param[in] Host                         - Pointer to sysHost
  @param[in] Socket                       - Memory controller to train
  @param[in] Ch                           - ddr channel to setup
  @param[in] Rank                         - Logical rank number
  @param[in] DqLoopCount                  - CPGC sequence loop count - number of times sequence is executed (2^(dqLoopCount - 1))
  @param[in] TestType                     - Advanced MemTest type
  @param[in] Mode                         - Type of sequence MT_CPGC_WRITE, MT_CPGC_READ, or MT_CPGC_READ_WRITE
  @param[in] SeqDataInv[MT_MAX_SUBSEQ]    - Enables pattern inversion per subsequence
  @param[in] PatternQW[MT_PATTERN_DEPTH]  - 64-bit wide data pattern to use per UI
  @param[in] IsUseInvtPat                 - Info to indicate whether or not patternQW is inverted by comparing original pattern
  @param[in] UiShl[MT_PATTERN_DEPTH]      - Enables pattern rotation per UI
  @param[in] NumCacheLines                - Number of cachelines to use in WDB
  @param[in] PatternDepth                 - Number of UI to use in patternQW
  @param[in] Direction                    - Sequential address direction MT_ADDR_DIR_UP, MT_ADDR_DIR_DN

  @retval SUCCESS

**/
UINT32
CpgcMemTestGlobalSetupMATS (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Rank,
  IN UINT8     DqLoopCount,
  IN UINT8     TestType,
  IN UINT8     Mode,
  IN UINT8     SeqDataInv[MT_MAX_SUBSEQ],
  IN UINT64_STRUCT PatternQW[MT_PATTERN_DEPTH],
  IN BOOLEAN   IsUseInvtPat,
  IN UINT8     UiShl[MT_PATTERN_DEPTH],
  IN UINT8     NumCacheLines,
  IN UINT8     PatternDepth,
  IN UINT8     Direction
  )
{
  UINT32                                      Status;
  UINT8                                       SubCh;
  CPGC_GLOBAL_TRAINING_SETUP                  CpgcGblTrainingSetup;
  UINT32                                      Data32;

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                 "CpgcMemTestSetupMATS:\n  pattern[0] = 0x%08x%08x\n  pattern[1] = 0x%08x%08x\n",
                 PatternQW[0].hi, PatternQW[0].lo, PatternQW[1].hi, PatternQW[1].lo);

  Status = SUCCESS;

  CpgcGblTrainingSetup.seqLoopCnt        = DqLoopCount;                      // test will end after (2^(n-1)) loops through the sequence
  CpgcGblTrainingSetup.startDelay        = 0;                                // Delay start
  CpgcGblTrainingSetup.addrUpdRateMode   = CPGC_UPDATE_BY_CL;                // Update based on the # of read and write cacheline

  // Default to WRITE
  CpgcGblTrainingSetup.rwMode            = CPGC_BASE_WRITE_SUBSEQ;           // Base Read or write - subseq0

  // If MEM_TEST is read, set to READ
  if (Mode == MT_CPGC_READ_WRITE || Mode == MT_CPGC_READ) {
    CpgcGblTrainingSetup.rwMode          = CPGC_BASE_READ_SUBSEQ;            // Base Read or write - subseq0
  }

  CpgcGblTrainingSetup.numCacheLines     = 0;                                // Number of Cachelines (2^N)
  CpgcGblTrainingSetup.burstSeparation   = 0;                                // (sub_seqwait) - need to add delay to drain credits if doing write / read subseq
  CpgcGblTrainingSetup.errEnChunk        = CPGC_ERR_MONITOR_ALL_CHUNK;       // monitor errors on any chunk
  CpgcGblTrainingSetup.errEnCl           = CPGC_ERR_MONITOR_ALL_CL;          // monitor errors on any cacheline
  if (TestType == ADV_MT_TYPE_LTEST_SCRAM) {
    CpgcGblTrainingSetup.stopOnErrCtl    = CPGC_ERR_STOP_ON_ALL_ERR;         // Stop on all lane error
  } else {
    CpgcGblTrainingSetup.stopOnErrCtl    = 1;                                // Stop on first error
  }

  if (Mode == MT_CPGC_READ_WRITE) {
    CpgcGblTrainingSetup.subseqEndPtr    = 1;                                // subseq end pointer
    CpgcGblTrainingSetup.useSubSeq1      = CPGC_BASE_WRITE_SUBSEQ;
  } else {
    CpgcGblTrainingSetup.subseqEndPtr    = 0;                                // subseq end pointer
    CpgcGblTrainingSetup.useSubSeq1      = CPGC_NO_SUBSEQ1;                  // do not use sub seq1 (base read)
  }
  CpgcGblTrainingSetup.cpgcLinkTrain     = 0;                                // do not use cpgc link train
  CpgcGblTrainingSetup.back2backWrEn     = 1;                                // enable b2b writes
  CpgcGblTrainingSetup.mprControl        = 0;                                // MPR disable
  CpgcGblTrainingSetup.x8Mode            = 0;
  CpgcGblTrainingSetup.direction         = Direction;

  CpgcGblTrainingSetup.InOrderExecutionEnable = TRUE;

  //
  // IsUseInvtPat contains the info whether ot not pattern in PatternQW is inverted or not
  // by comparing the original memory test patterns defined in Spec.
  //
  CpgcGblTrainingSetup.IsUseInvtPat      = IsUseInvtPat;

  //
  // Recode whether or not subseq0/1's data need to inverted
  //
  CpgcGblTrainingSetup.SeqDataInv[0]     = SeqDataInv[0];
  CpgcGblTrainingSetup.SeqDataInv[1]     = SeqDataInv[1];

  //
  // Don't enable CPGC in-order execution mode during MemInit
  // if "S14010044298" is in effect.
  //
  if ((TestType == ADV_MT_TYPE_LINIT_SCRAM) &&
      (IsSiliconWorkaroundEnabled ("S14010044298"))) {
    CpgcGblTrainingSetup.InOrderExecutionEnable = FALSE;
  }

  CpgcGlobalTrainingSetup (Host, Socket, Ch, &CpgcGblTrainingSetup);

  //
  // Setup WDB
  //
  CpgcSetupWdbMATS (Host, Socket, Ch,
                    0,                           // wdbIncrRate
                    0,                           // wdbIncrScale: exponential increment
                    0,                           // wdbStartPntr
                    NumCacheLines - 1            // wdbEndPntr
                   );

  //
  // Disable CADB
  //
  Data32 = 0;
  for (SubCh = 0; SubCh < CpgcGetValidSubCh (); SubCh++) {
    GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_CFG_MCCADB_MAIN_REG, &Data32);
  }

  return Status;
} // CpgcMemTestGlobalSetupMATS


/**

  Collect DDR advanced command CPGC error status

  cpgcErrDat0S                       - Repo Inactive - SubCh-A data lane [31:0] even UI error status.
                                     - Repo Active   - SubCh-A data lane [31:0] UI error status.
  cpgcErrDat1S                       - Repo Inactive - SubCh-B data lane [31:0] even UI error status.
                                     - Repo Active   - SubCh-B data lane [31:0] UI error status.
  cpgcErrDat2S                       - Repo Inactive - SubCh-A data lane [31:0] odd UI error status.
                                     - Repo Active   - Same to cpgcErrDat0S
  cpgcErrDat3S                       - Repo Inactive - SubCh-B data lane [31:0] odd UI error status.
                                     - Repo Active   - Same to cpgcErrDat1S
  cpgcErrEccS                        - Repo Inactive
                                     - [7:0]    - SubCh-A ecc lane [7:0] even UI error status.
                                     - [15:8]   - SubCh-A ecc lane [7:0] odd UI error status.
                                     - [23:16]  - SubCh-B ecc lane [7:0] even UI error status.
                                     - [31:24]  - SubCh-B ecc lane [7:0] odd UI error status.
                                     - Repo Active
                                     - [7:0]    - SubCh-A ecc lane [7:0] UI error status.
                                     - [15:8]   - Same to [7:0]
                                     - [23:16]  - SubCh-B ecc lane [7:0] UI error status.
                                     - [31:24]  - Same to [23:16]
  cpgcErrChunk                       - [15:0]   - SubCh-A chunk error status
                                     - [31:16]  - SubCh-B chunk error status
  cpgcErrColumn                      - SubCh-A Column address that error happens on
  cpgcErrColumnExt                   - SubCh-B Column address that error happens on
  cpgcErrRow                         - SubCh-A Row address that error happens on
  cpgcErrRowExt                      - SubCh-B Row address that error happens on
  cpgcErrBank                        - [4:0]   SubCh-A Bank address that error happens on
                                     - [20:16] SubCh-B Bank address that error happens on
  cpgcErrRank                        - [2:0]   SubCh-A Rank address that error happens on
                                     - [18:16] SubCh-B Rank address that error happens on
  cpgcSubrank                        - [2:0]   SubCh-A Subrank address that error happens on
                                     - [18:16] SubCh-B Subrank address that error happens on

  @param[in]  Host                    - Pointer to sysHost
  @param[in]  Socket                  - Memory controller number
  @param[in]  DdrChEnMap              - Channel mask
  @param[out] CpgcErrorStatus[MAX_CH] - Array of error status

  @retval   SUCCESS/FAILURE

**/
UINT32
CpgcAdvTrainingErrorStatusMATS (
  IN     PSYSHOST                Host,
  IN     UINT8                   Socket,
  IN     UINT32                  DdrChEnMap,
     OUT CPGC_ERROR_STATUS_MATS  CpgcErrorStatusMats[MAX_CH]
  )
{
  UINT32                                          Status = SUCCESS;
  UINT8                                           Ch;
  UINT8                                           MaxChDdr;
  CPGC2_ERR_ADDR_INFO_LO                          ErrInfoLo;
  CPGC2_ERR_ADDR_INFO_HI                          ErrInfoHi;
  MC_CPGC_SUBRANK_MAP_MC_MAIN_STRUCT              McCpgcSubrankMap;
  CPGC_SEQ_RANK_L2P_MAPPING_A_MCCPGC_MAIN_STRUCT  SeqRankL2pMapping;
  CPGC_ERROR_STATUS                               CpgcErrorStatus[MAX_CH];

  MaxChDdr = GetMaxChDdr ();

  SetMem (CpgcErrorStatusMats, MaxChDdr * sizeof (CPGC_ERROR_STATUS_MATS), 0);
  SetMem (CpgcErrorStatus, MaxChDdr * sizeof (CPGC_ERROR_STATUS), 0);

  CpgcAdvTrainingErrorStatus (Host, Socket, DdrChEnMap, CpgcErrorStatus);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {

    //
    // Skip inactive channels
    //
    if (!(DdrChEnMap & (1 << Ch))) {
      continue;
    }

    CpgcErrorStatusMats[Ch].cpgcErrDat0S = CpgcErrorStatus[Ch].cpgcErrDat0S;
    CpgcErrorStatusMats[Ch].cpgcErrDat1S = CpgcErrorStatus[Ch].cpgcErrDat1S;
    CpgcErrorStatusMats[Ch].cpgcErrDat2S = CpgcErrorStatus[Ch].cpgcErrDat2S;
    CpgcErrorStatusMats[Ch].cpgcErrDat3S = CpgcErrorStatus[Ch].cpgcErrDat3S;
    CpgcErrorStatusMats[Ch].cpgcErrEccS  = CpgcErrorStatus[Ch].cpgcErrEccS;

    //
    // Rank and subrank info
    //
    SeqRankL2pMapping.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_A, CPGC_SEQ_RANK_L2P_MAPPING_A_MCCPGC_MAIN_REG));
    CpgcErrorStatusMats[Ch].cpgcErrRank = SeqRankL2pMapping.Bits.l2p_rank0_mapping;
    SeqRankL2pMapping.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_B, CPGC_SEQ_RANK_L2P_MAPPING_A_MCCPGC_MAIN_REG));
    CpgcErrorStatusMats[Ch].cpgcErrRank |= SeqRankL2pMapping.Bits.l2p_rank0_mapping << WORD_WIDTH;

    McCpgcSubrankMap.Data = MemReadPciCfgEp (Socket, Ch, MC_CPGC_SUBRANK_MAP_MC_MAIN_REG);
    CpgcErrorStatusMats[Ch].cpgcSubrank = McCpgcSubrankMap.Bits.rank0_mapping;
    CpgcErrorStatusMats[Ch].cpgcSubrank |= McCpgcSubrankMap.Bits.rank0_mapping << WORD_WIDTH;

    //
    // 16 chucks per cacheline for DDR5
    //
    CpgcErrorStatusMats[Ch].cpgcErrChunk  = (WORD_MASK & MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_A, CPGC_ERR_STAT47_MCCPGC_MAIN_REG)));
    CpgcErrorStatusMats[Ch].cpgcErrChunk |= (WORD_MASK & MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_B, CPGC_ERR_STAT47_MCCPGC_MAIN_REG))) << WORD_WIDTH;

    //
    // Physical address info
    // SubCh-A
    //
    ErrInfoLo.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_A, CPGC2_ERR_SUMMARY_B_MCCPGC_MAIN_REG));
    ErrInfoHi.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_A, CPGC2_ERR_SUMMARY_C_MCCPGC_MAIN_REG));

    CpgcErrorStatusMats[Ch].cpgcErrRow = ErrInfoLo.Bits.Row | (ErrInfoHi.Bits.Row << CPGC2_ERR_ADDR_INFO_HI_OFFSET);
    //
    // Lower bits are cacheline boundary
    //
    CpgcErrorStatusMats[Ch].cpgcErrColumn = ErrInfoLo.Bits.Column << Log2x32 (BURST_LENGTH);

    //
    // Based on AMT Spec, swap CPGC bank[3:2] with bank[1:0] based on empirical results of testing
    //
    CpgcErrorStatusMats[Ch].cpgcErrBank = ((ErrInfoHi.Bits.Bank & 0x3) << 2) | ((ErrInfoHi.Bits.Bank >> 2) & 3);
    //
    // SubCh-B
    //
    ErrInfoLo.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_B, CPGC2_ERR_SUMMARY_B_MCCPGC_MAIN_REG));
    ErrInfoHi.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_B, CPGC2_ERR_SUMMARY_C_MCCPGC_MAIN_REG));

    CpgcErrorStatusMats[Ch].cpgcErrRowExt = ErrInfoLo.Bits.Row | (ErrInfoHi.Bits.Row << CPGC2_ERR_ADDR_INFO_HI_OFFSET);
    //
    // Lower bits are cacheline boundary
    //
    CpgcErrorStatusMats[Ch].cpgcErrColumnExt = ErrInfoLo.Bits.Column << Log2x32 (BURST_LENGTH);
    //
    // Based on AMT Spec, swap CPGC bank[3:2] with bank[1:0] based on empirical results of testing
    //
    CpgcErrorStatusMats[Ch].cpgcErrBank = (((ErrInfoHi.Bits.Bank & 0x3) << 2) | ((ErrInfoHi.Bits.Bank >> 2) & 3)) << WORD_WIDTH;

    RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, SUB_CH_A, NO_RANK, NO_STROBE, NO_BIT,
                   "CpgcAdvTrainingErrorStatusMATS: Row = 0x%06x, Column = 0x%04x, Bank = 0x%02x, Chunk = 0x%02x, cpgcErrEven = 0x%08x, cpgcErrOdd = 0x%08x, cpgcErrEccS = 0x%04x\n",
                    CpgcErrorStatusMats[Ch].cpgcErrRow,
                    CpgcErrorStatusMats[Ch].cpgcErrColumn,
                    CpgcErrorStatusMats[Ch].cpgcErrBank & WORD_MASK,
                    CpgcErrorStatusMats[Ch].cpgcErrChunk & WORD_MASK,
                    CpgcErrorStatusMats[Ch].cpgcErrDat0S,
                    CpgcErrorStatusMats[Ch].cpgcErrDat2S,
                    CpgcErrorStatusMats[Ch].cpgcErrEccS
      );

    RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, SUB_CH_B, NO_RANK, NO_STROBE, NO_BIT,
                   "CpgcAdvTrainingErrorStatusMATS: Row = 0x%06x, Column = 0x%04x, Bank = 0x%02x, Chunk = 0x%02x, cpgcErrEven = 0x%08x, cpgcErrOdd = 0x%08x, cpgcErrEccS = 0x%04x\n",
                    CpgcErrorStatusMats[Ch].cpgcErrRowExt,
                    CpgcErrorStatusMats[Ch].cpgcErrColumnExt,
                    CpgcErrorStatusMats[Ch].cpgcErrBank >> WORD_WIDTH,
                    CpgcErrorStatusMats[Ch].cpgcErrChunk >> WORD_WIDTH,
                    CpgcErrorStatusMats[Ch].cpgcErrDat1S,
                    CpgcErrorStatusMats[Ch].cpgcErrDat3S,
                    CpgcErrorStatusMats[Ch].cpgcErrEccS >> WORD_WIDTH
      );
  } // Ch

  return Status;
} // CpgcAdvTrainingErrorStatusMATS

STATIC UINT32
CpgcInitializeTransientErrChecking (
  PSYSHOST          Host,
  UINT8             socket,
  UINT8             ch
  )
/*++

Routine Description:

  Initialize DDR advanced command CPGC error status based err counters to eliminate transient errors.

Arguments:

  Host                - Pointer to sysHost
  socket              - Socket to train
  ch                  - Channel ID

Returns:

  SUCCESS / FAILURE

Notes:

    0 = no error
    1 = error


--*/
{

  return EFI_SUCCESS;

} // CpgcInitializeTransientErrChecking

/**

  Cpgc command pattern setup up for memory test

  @param[in] Host                  - Pointer to sysHost
  @param[in] Socket                - Socket to train
  @param[in] Ch                    - Ddr channel to setup
  @param[in] SubCh                 - Ddr sub-channel to setup
  @param[in] AddressCarry          - Pointer to structure for Cpgc address carry
  @param[in] RwMode                - Cpgc read and write mode
  @param[in] UseSubSeq1            - Cpgc subseq1 usage
  @param[in] Direction             - Address direction during memory test
  @param[in] SeqDataInv            - Enables pattern inversion per subsequence
  @param[in] IsUseInvtPat          - Info to indicate whether or not patternQW is inverted by comparing original pattern

  @retval n/a

**/
VOID
CpgcMemTestCmdPatSetup (
  IN PSYSHOST       Host,
  IN UINT8          Socket,
  IN UINT8          Ch,
  IN UINT8          SubCh,
  IN TAddressCarry  *AddressCarry,
  IN UINT8          RwMode,
  IN UINT8          UseSubSeq1,
  IN UINT8          Direction,
  IN UINT8          SeqDataInv[MT_MAX_SUBSEQ],
  IN BOOLEAN        IsUseInvtPat
  )
{
  UINT16  BurstWait;

  if (UseSubSeq1 == CPGC_NO_SUBSEQ1) {
    //
    // only read or write
    //
    if (RwMode == CPGC_BASE_READ_SUBSEQ) {
      //
      // read only
      //
      GetRdDqBurstWait (&BurstWait);
      if (IsUseInvtPat) {
        if (Direction == MT_ADDR_DIR_UP) { // ^(rI)
          SetCpgcCmdPat (Host, Socket, Ch, SubCh, PatRdUpInvt, AddressCarry, BurstWait);
        } else { // v(rI)
          SetCpgcCmdPat (Host, Socket, Ch, SubCh, PatRdDownInvt, AddressCarry, BurstWait);
        }
      } else {
        if (Direction == MT_ADDR_DIR_UP) { // ^(rD)
          SetCpgcCmdPat (Host, Socket, Ch, SubCh, PatRdUp, AddressCarry, BurstWait);
        } else { // v(rD)
          SetCpgcCmdPat (Host, Socket, Ch, SubCh, PatRdDown, AddressCarry, BurstWait);
        }
      }
    } else {
      //
      // write only
      //
      GetWrDqBurstWait (&BurstWait);
      if (IsUseInvtPat) {
        if (Direction == MT_ADDR_DIR_UP) { // ^(wI)
          SetCpgcCmdPat (Host, Socket, Ch, SubCh, PatWrUpInvt, AddressCarry, BurstWait);
        } else { // v(wI)
          SetCpgcCmdPat (Host, Socket, Ch, SubCh, PatWrDownInvt, AddressCarry, BurstWait);
        }
      } else {
        if (Direction == MT_ADDR_DIR_UP) { // ^(wD)
          SetCpgcCmdPat (Host, Socket, Ch, SubCh, PatWrUp, AddressCarry, BurstWait);
        } else { // v(wD)
          SetCpgcCmdPat (Host, Socket, Ch, SubCh, PatWrDown, AddressCarry, BurstWait);
        }
      }
    }
  } else {
    //
    // read and then write
    //
    GetWrDqBurstWait (&BurstWait);
    if (IsUseInvtPat) {
      if (Direction == MT_ADDR_DIR_UP) {
        if (SeqDataInv[1] == 1) { // ^(rI, wD)
          SetCpgcCmdPat (Host, Socket, Ch, SubCh, PatRdWrUpInvtAlt, AddressCarry, BurstWait);
        } else { // ^(rI, wI)
          SetCpgcCmdPat (Host, Socket, Ch, SubCh, PatRdWrUpInvt, AddressCarry, BurstWait);
        }
      } else {
        if (SeqDataInv[1] == 1) { // v(rI, wD)
          SetCpgcCmdPat (Host, Socket, Ch, SubCh, PatRdWrDownInvtAlt, AddressCarry, BurstWait);
        } else { // v(rI, wI)
          SetCpgcCmdPat (Host, Socket, Ch, SubCh, PatRdWrDownInvt, AddressCarry, BurstWait);
        }
      }
    } else {
      if (Direction == MT_ADDR_DIR_UP) {
        if (SeqDataInv[1] == 1) { // ^(rD, wI)
          SetCpgcCmdPat (Host, Socket, Ch, SubCh, PatRdWrUpAlt, AddressCarry, BurstWait);
        } else { // ^(rD, wD)
          SetCpgcCmdPat (Host, Socket, Ch, SubCh, PatRdWrUp, AddressCarry, BurstWait);
        }
      } else {
        if (SeqDataInv[1] == 1) { // v(rD, wI)
          SetCpgcCmdPat (Host, Socket, Ch, SubCh, PatRdWrDownAlt, AddressCarry, BurstWait);
        } else { // v(rD, wD)
          SetCpgcCmdPat (Host, Socket, Ch, SubCh, PatRdWrDown, AddressCarry, BurstWait);
        }
      }
    }
  }
}

/**

  Global training CPGC setup

  @param[in] Host                  - Pointer to sysHost
  @param[in] Socket                - Socket to train
  @param[in] Ch                    - Ddr channel to setup
  @param[in] CpgcGblTrainingSetup  - Point to Structure for CPGC global training setup

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcGlobalTrainingSetup (
  IN PSYSHOST                     Host,
  IN UINT8                        Socket,
  IN UINT8                        Ch,
  IN CPGC_GLOBAL_TRAINING_SETUP  *CpgcGblTrainingSetup
  )
{
  UINT32                                                 Status;
  UINT8                                                  StartDelay;
  UINT8                                                  SeqLoopCnt;
  UINT8                                                  NumCacheLines;
  UINT8                                                  ErrEnCl;
  UINT8                                                  StopOnErrCtl;
  UINT8                                                  Direction;
  UINT8                                                  SubCh;
  CPGC_MISCREFCTL_MCDDC_CTL_STRUCT                       CpgcMiscRefCtlReg;
  CPGC_MISCZQCTL_MCDDC_CTL_STRUCT                        CpgcMiscZQCtlReg;
  CPGC_MISCODTCTL_MCDDC_CTL_STRUCT                       CpgcMiscOdtCtlReg;
  CPGC_MISCCKECTL_MCDDC_CTL_STRUCT                       CpgcMiscCkeCtlReg;
  CPGC_SEQ_CTL_MCCPGC_MAIN_STRUCT                        SeqCtl;
  CPGC_SEQ_CFG_B_MCCPGC_MAIN_STRUCT                      SeqCfgB;
  CPGC_SEQ_CFG_A_MCCPGC_MAIN_STRUCT                      SeqCfgA;
  CPGC2_BASE_CLOCK_CONFIG_MCCPGC_MAIN_STRUCT             BaseClockConfig;
  CPGC2_BASE_REPEATS_MCCPGC_MAIN_STRUCT                  BaseRepeats;
  CPGC2_BLOCK_REPEATS_MCCPGC_MAIN_STRUCT                 BlockRepeats;
  CPGC2_RASTER_REPO_CONFIG_MCCPGC_MAIN_STRUCT            RasterRepoConfig;
  CPGC_ERR_CNTRCTL_0_MCCPGC_MAIN_STRUCT                  ErrCntrctl0;
  CPGC_ERR_CNTRCTL_1_MCCPGC_MAIN_STRUCT                  ErrCntrctl1;
  CPGC_ERR_CNTRCTL_2_MCCPGC_MAIN_STRUCT                  ErrCntrctl2;
  CPGC_ERR_CTL_MCCPGC_MAIN_STRUCT                        ErrCtl;
  TAddressCarry                                          AddressCarry;
  UINT8                                                  RwMode;
  UINT8                                                  UseSubSeq1;
  UINT8                                                  SeqDataInv[MT_MAX_SUBSEQ];
  BOOLEAN                                                IsUseInvtPat;

  Status = SUCCESS;

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "CpgcGlobalTrainingSetup\n");

  StartDelay          = CpgcGblTrainingSetup->startDelay;
  SeqLoopCnt          = CpgcGblTrainingSetup->seqLoopCnt;
  NumCacheLines       = CpgcGblTrainingSetup->numCacheLines;
  ErrEnCl             = CpgcGblTrainingSetup->errEnCl;
  StopOnErrCtl        = CpgcGblTrainingSetup->stopOnErrCtl;
  Direction           = CpgcGblTrainingSetup->direction;
  RwMode              = CpgcGblTrainingSetup->rwMode;
  UseSubSeq1          = CpgcGblTrainingSetup->useSubSeq1;
  SeqDataInv[0]       = CpgcGblTrainingSetup->SeqDataInv[0];
  SeqDataInv[1]       = CpgcGblTrainingSetup->SeqDataInv[1];
  IsUseInvtPat        = CpgcGblTrainingSetup->IsUseInvtPat;

  //
  // program scheduler (based on ch)
  //

  //
  // MC Controller Init - disable miscellaneous events
  // functional CKE logic, do not disable CKE powerdown,
  //
  CpgcMiscCkeCtlReg.Data = MemReadPciCfgEp (Socket, Ch, CPGC_MISCCKECTL_MCDDC_CTL_REG);
  CpgcMiscRefCtlReg.Data = MemReadPciCfgEp (Socket, Ch, CPGC_MISCREFCTL_MCDDC_CTL_REG);
  CpgcMiscZQCtlReg.Data  = MemReadPciCfgEp (Socket, Ch, CPGC_MISCZQCTL_MCDDC_CTL_REG);
  CpgcMiscOdtCtlReg.Data  = MemReadPciCfgEp (Socket, Ch, CPGC_MISCODTCTL_MCDDC_CTL_REG);

  CpgcMiscCkeCtlReg.Bits.cke_override     = 0;
  CpgcMiscCkeCtlReg.Bits.cke_on           = 0;
  CpgcMiscOdtCtlReg.Bits.odt_override     = 0;
  CpgcMiscOdtCtlReg.Bits.mpr_train_ddr_on = 0;

  MemWritePciCfgEp (Socket, Ch, CPGC_MISCCKECTL_MCDDC_CTL_REG, CpgcMiscCkeCtlReg.Data);
  MemWritePciCfgEp (Socket, Ch, CPGC_MISCREFCTL_MCDDC_CTL_REG, CpgcMiscRefCtlReg.Data);
  MemWritePciCfgEp (Socket, Ch, CPGC_MISCZQCTL_MCDDC_CTL_REG, CpgcMiscZQCtlReg.Data);
  MemWritePciCfgEp (Socket, Ch, CPGC_MISCODTCTL_MCDDC_CTL_REG, CpgcMiscOdtCtlReg.Data);

  if (CpgcGblTrainingSetup->InOrderExecutionEnable) {
    CpgcInorderModeEnable (Socket, Ch, TRUE);
  }

  for (SubCh = 0; SubCh < CpgcGetValidSubCh (); SubCh++) {
    SeqCtl.Data = 0;
    SeqCtl.Bits.single_march_element_mode = 0; // Don't pause after algorithm instruction
    SeqCtl.Bits.continue_single_mode = 0;      // No need fpr continous tests
    SeqCtl.Bits.single_cmd_mode = 0;           // Don't pause command issuing
    SeqCtl.Bits.clear_errors = 1;              // Clear errors for the test
    SeqCtl.Bits.stop_test = 0;                 // Stop previous test if any
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_CTL_MCCPGC_MAIN_REG), SeqCtl.Data);

    SeqCfgA.Data = 0;
    SeqCfgA.Bits.channel_assign = (SubCh == 1) ? 3 : 0; //(1 << SubCh); // Assign channels for this generator (2 sub-ch runs independently)
    SeqCfgA.Bits.initialization_mode = (SubCh == 1) ? CPGC_Active_Mode : Idle_Mode; // Test Mode

    if (IsSiliconWorkaroundEnabled ("S1507902816")) {
      //
      // Restrict write/read order which could slow down the transaction in in-order mode
      //
      SeqCfgA.Bits.posted_wr_to_rd_order = 1;
    }

    //
    // Bind this request engine to all other globally enabled request engines
    //
    SeqCfgA.Bits.global_start_bind = 0;
    SeqCfgA.Bits.global_stop_bind = 0;
    SeqCfgA.Bits.global_clear_err_bind = 0;
    SeqCfgA.Bits.global_stop_on_err_bind = 0;
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_CFG_A_MCCPGC_MAIN_REG), SeqCfgA.Data);

    SeqCfgB.Data = 0;
    //
    // StartDelay for subchA, StartDelay + 1 for subchB, need to offset the commands between subchannels due to shared data path
    //
    SeqCfgB.Bits.start_delay = (SubCh == 1) ? (StartDelay + 1) : StartDelay;
    GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC_SEQ_CFG_B_MCCPGC_MAIN_REG, &SeqCfgB.Data);

    BaseClockConfig.Data = 0;
    BaseClockConfig.Bits.increment_value = 1; // Wait timer advance by 1
    BaseClockConfig.Bits.clock_freq = 400;// 400/4 = 100 MHZ. update per frequency. 25; // Use native MC clock
    BaseClockConfig.Bits.prescaler_for_clock_freq = 1; // No divide for MC provided clock (divided by 1)
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_BASE_CLOCK_CONFIG_MCCPGC_MAIN_REG), BaseClockConfig.Data);

    BaseRepeats.Data = 0;
    BaseRepeats.Bits.base_repeats = NumCacheLines;  // N-1
    GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_BASE_REPEATS_MCCPGC_MAIN_REG, &BaseRepeats.Data);
    BlockRepeats.Data = 0;
    BlockRepeats.Bits.block_repeats = (1 << SeqLoopCnt) - 1; // N-1
    GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_BLOCK_REPEATS_MCCPGC_MAIN_REG, &BlockRepeats.Data);

    AddressCarry.AddressDirection  = AddressDirNorth; // Default to set Fast-Y
    AddressCarry.AddressOrder = AddressOrderBankColRowRank; // Default to set bank interleave for good performance
    CpgcMemTestCmdPatSetup (Host, Socket, Ch, SubCh, &AddressCarry, RwMode, UseSubSeq1, Direction, SeqDataInv, IsUseInvtPat);

    RasterRepoConfig.Data = 0;
    RasterRepoConfig.Bits.rasterrepoclear = 1; //Do it in the test start. Reset the raster repo entries. This bit auto-clears
    RasterRepoConfig.Bits.rasterrepomode = CPGC_RASTER_REPO_BITMAP_MODE; // Error address info (Rnk/Bnk/Row/Col) could be available in this mode
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_RASTER_REPO_CONFIG_MCCPGC_MAIN_REG), RasterRepoConfig.Data);

    //
    // Make sure the first read being checked
    //
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_MASK_ERRS_FIRST_N_READS_MCCPGC_MAIN_REG), 0);

    //
    //  Disable any data/ecc inversion so that we see data from extbuf directly on DQ lanes
    //
    SetCpgcDcMaskLanes (Host, Socket, Ch, SubCh, 0, 0, 0);

    //
    // No masking of reads for error checking
    //
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_MASK_ERRS_FIRST_N_READS_MCCPGC_MAIN_REG), 0);

    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_DPAT_ALT_BUF_CFG_MCCPGC_MAIN_REG), 0);

    //
    // Error check settings
    //
    ErrCntrctl0.Data = 0;
    ErrCntrctl0.Bits.counter_pointer = 0; // don't care for counter_control_scope=0
    ErrCntrctl0.Bits.counter_control_sel = 0; // don't care for counter_control_scope=0
    ErrCntrctl0.Bits.counter_control_ui = 0; // count errors on all UI's
    ErrCntrctl0.Bits.counter_control_scope = 0; // non-specific selection (count everything)
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_CNTRCTL_0_MCCPGC_MAIN_REG), ErrCntrctl0.Data);
    ErrCntrctl1.Data = 0;
    ErrCntrctl1.Bits.counter_pointer = 0; // only matters for particular UI (counter_control_ui = 3)
    ErrCntrctl1.Bits.counter_control_sel = 3; // set to count errors on UI's
    ErrCntrctl1.Bits.counter_control_ui = 1; // count errors on even UI's
    ErrCntrctl1.Bits.counter_control_scope = 1; // specific selection
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_CNTRCTL_1_MCCPGC_MAIN_REG), ErrCntrctl1.Data);
    ErrCntrctl2.Data = 0;
    ErrCntrctl2.Bits.counter_pointer = 0; // only matters for particular UI (counter_control_ui = 3)
    ErrCntrctl2.Bits.counter_control_sel = 3;  // set to count errors on UI's
    ErrCntrctl2.Bits.counter_control_ui = 2; // count errors on odd UI's
    ErrCntrctl2.Bits.counter_control_scope = 1; // specific selection
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_CNTRCTL_2_MCCPGC_MAIN_REG), ErrCntrctl2.Data);

    //
    // Error Control Registers
    //  stop on nth error (must be 0)
    //  do not stop on error
    //  configure monitor errors on all cachelines
    //  do not monitor errors on all chunks
    //
    ErrCtl.Data = 0;
    ErrCtl.Bits.stop_on_n = 0; // Stop on first error
    ErrCtl.Bits.stop_on_error_ctl = StopOnErrCtl;
    ErrCtl.Bits.errchk_mask_chunk = 0;
    ErrCtl.Bits.errchk_mask_cacheline = ErrEnCl;
    ErrCtl.Bits.be_train_err_en = 1; // Use default (unclear what this is for)
    ErrCtl.Bits.errchk_mask_chunk = 0; // only used in 64 bit bus, uses err_lnen_hi instead for DDR5
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_CTL_MCCPGC_MAIN_REG), ErrCtl.Data);
    //
    // Lanes [31:0] error checking enabled
    //
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_LNEN_LO_MCCPGC_MAIN_REG), 0);
    //
    // Check Enable mask for UI[15:0] -for x32 bus this register is repurposed as chunk/UI enable
    // 16 chunks per CL
    //
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_LNEN_HI_MCCPGC_MAIN_REG), 0xFFFF);

    //
    // Do not check ECC bits if ECC is disabled
    //
    if (Host->nvram.mem.eccEn) {
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_XLNEN_MCCPGC_MAIN_REG), 0);
    } else {
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_XLNEN_MCCPGC_MAIN_REG), 0xFF);
    }
  } // SubCh loop

   return Status;
} // CpgcGlobalTrainingSetup

/**

  Loop based on PatternLength

  @param[in] PatternLength - Number of loops

  @retval dqLoopCount

**/
UINT8
PatternLengthToCpgcLoopCnt (
  IN UINT32 PatternLength
  )
{
  // SPR_TODO: 1507507284: [CPGC2 ] The input parameter "loopcount"
  // in the function SetupTest() and SetupIOTest() should be used in consistent way

  return (UINT8) PatternLength;
}

/**

  DDR advanced command CPGC execution

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Memory controller to train
  @param[in] DdrChEnMap  - 4-bit bit mask of ddr channels to test - (1 = channel is selected to run tests)
  @param[in] Pattern     - ddr pattern (0-3) ( 0 = dq, 1 = cmd group 1, 2 = cmd group 2, 3 = cmd group 3)

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcAdvTrainingExecTest (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    DdrChEnMap,
  IN UINT32    Pattern
  )
{
  UINT8   SubCh = 0;

  IO_Reset (Host, Socket);

  for (SubCh = 0; SubCh < CpgcGetValidSubCh (); SubCh++) {
    if (SubCh == SUB_CH_A) {
      continue;
    }
    CpgcGlobalTestStart (Host, Socket, DdrChEnMap, SubCh);
  }

  for (SubCh = 0; SubCh < CpgcGetValidSubCh (); SubCh++) {
    if (SubCh == SUB_CH_A) {
      continue;
    }
    CpgcPollGlobalTestDone (Host, Socket, DdrChEnMap, SubCh);
  }

  CteDelayQclk (60);

  return EFI_SUCCESS;

} // CpgcAdvTrainingExecTest

UINT32
CpgcAdvTrainingStartTest (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    ddrChEnMap,
  UINT16    pattern,
  UINT8     SubCh
  )
{
  return EFI_SUCCESS;
} // CpgcAdvTrainingExecTest10nm

/**

  DDR advanced command CPGC execution

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Memory controller to train
  @param[in] DdrChEnMap  - 4-bit bit mask of ddr channels to test - (1 = channel is selected to run tests)
  @param[in] Pattern     - ddr pattern (0-3) ( 0 = dq, 1 = cmd group 1, 2 = cmd group 2, 3 = cmd group 3)

  @retval SUCCESS
  @retval FAILURE

**/
UINT32
CpgcAdvTrainingExecTestGroup (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT32    DdrChEnMap,
  IN    UINT32    Pattern
  )
{
  return CpgcAdvTrainingExecTest (Host, Socket, DdrChEnMap, Pattern);
} // CpgcAdvTrainingExecTestGroup

/** Returns whether or not a channel is active.

  @param[in]  ChannelMask   Bit mask indicating channel activity (active = 1, inactive = 0)
  @param[in]  Channel       The channel to check against

  @retval FALSE The channel is not active
  @retval TRUE  The channel is active

**/
BOOLEAN
ChannelActive (
  UINT32  ChannelMask,
  UINT8   Channel
  )
{
  return (ChannelMask & (1 << Channel)) != 0;
}

/** Returns whether or not a memory controller is active.

  @param[in]  ChannelMask       Bit mask indicating channel activity (active = 1, inactive = 0)
  @param[in]  MemoryController  The memory controller to check against

  @retval FALSE The memory controller is not active
  @retval TRUE  The memory controller is active

**/
BOOLEAN
MemoryControllerActive (
  UINT32  ChannelMask,
  UINT8   MemoryController
  )
{
  UINT8 Channel;
  UINT8 NumChPerMc;

  NumChPerMc = GetNumChannelPerMc ();

  for (Channel = MemoryController * NumChPerMc;
       Channel < (MemoryController + 1) * NumChPerMc;
       Channel++) {
    if (ChannelActive (ChannelMask, Channel)) {
      // Return TRUE if any channel in the memory controller is active.
      return TRUE;
    }
  }

  // Return FALSE if no channels in the memory controller are active.
  return FALSE;
}

/** Set whether a given channel has hit a DDRT Read Pending Queue timeout.

  @param[in] Host     Pointer to sysHost
  @param[in] Socket   Processor socket within the system (0-based)
  @param[in] Channel  Number of the channel to check
  @param[in] TimedOut Whether or not the channel has hit a Read Pending Queue timeout.

**/
VOID
SetDdrtReadPendingQueueTimedOut (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  BOOLEAN   TimedOut
  )
{
  Host->var.mem.DdrtReadPendingQueueTimedOut[Socket][Channel] = TimedOut;
}

/** Perform setup required before polling on CPGC test done registers.

  The following are initialized for each channel on the indicated memory controller:
    - DDRT Read Pending Queue timeout (occurred) indicators are cleared.
    - DDRT Read Pending Queue timeout enabled indicators are set.

  @param[in]  Host                                Pointer to SysHost
  @param[in]  Socket                              Socket being trained
  @param[in]  MemoryController                    Memory controller to be polled on
  @param[in]  ChannelMask                         Bit mask indicating channel activity (active = 1, inactive = 0)
  @param[out] DdrtReadPendingQueueTimeoutEnabled  Array indicating whether the Read Pending Queue timeout is enabled
                                                  for a given channel

  @retval EFI_SUCCESS   No errors were encountered
  @retval !EFI_SUCCESS  An error was encountered

**/
EFI_STATUS
SetUpTestDonePolling (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     MemoryController,
  IN  UINT32    ChannelMask,
  OUT BOOLEAN   DdrtReadPendingQueueTimeoutEnabled[MAX_CH]
  )
{
  UINT8                                Channel;
  CPGC_DDRT_ERR_CTL_STS_MC_2LM_STRUCT  CpgcDdrtErrCtlSts;
  UINT8                                NumChPerMc;

  NumChPerMc = GetNumChannelPerMc ();

  // Initialize timeout indicators.
  for (Channel = MemoryController * NumChPerMc;
       Channel < (MemoryController + 1) * NumChPerMc;
       Channel++) {

    // Clear the previous timeout indicator for the channel.
    SetDdrtReadPendingQueueTimedOut (Host, Socket, Channel, FALSE);

    // Determine whether timeouts are enabled (or even relevant) for the channel.
    if (ChannelActive (ChannelMask, Channel) &&
        IsDdrtDimm (Host, Socket, Channel)) {
      CpgcDdrtErrCtlSts.Data = MemReadPciCfgEp (Socket, Channel, CPGC_DDRT_ERR_CTL_STS_MC_2LM_REG);
      DdrtReadPendingQueueTimeoutEnabled[Channel] = CpgcDdrtErrCtlSts.Bits.timeout_base_clk != 0 &&
        CpgcDdrtErrCtlSts.Bits.timeout_threshold != 0;
    } else {
      DdrtReadPendingQueueTimeoutEnabled[Channel] = FALSE;
    }

  } // for (Channel)

  return EFI_SUCCESS;
} // SetUpTestDonePolling

/** Check DDRT-specific conditions to determine whether a channel is done being tested.

  This checks for Read Pending Queue timeouts on DDRT ranks.

  @param[in]  Host                                Pointer to SysHost
  @param[in]  Socket                              Socket being trained
  @param[in]  Channel                             Channel being trained
  @param[in]  DdrtReadPendingQueueTimeoutEnabled  Array indicating whether the Read Pending Queue timeout is enabled
                                                  for a given channel
  @param[out] ChannelDone                         Whether or not the channel is done testing.

  @retval EFI_SUCCESS   No errors were encountered.
  @retval !EFI_SUCCESS  An error was encountered.

**/
EFI_STATUS
CheckDdrtChannelDone (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  BOOLEAN   DdrtReadPendingQueueTimeoutEnabled[MAX_CH],
  OUT BOOLEAN   *ChannelDone
  )
{
  CPGC_DDRT_ERR_CTL_STS_MC_2LM_STRUCT  CpgcDdrtErrCtlSts;

  // Note if a DDRT rank with the Read Pending Queue timeout enabled is hung, marking test done for the channel.
  if (DdrtReadPendingQueueTimeoutEnabled[Channel] && IsDdrtDimm (Host, Socket, Channel)) {

    // Mark the channel as hung if a) it hasn't already timed out, b) it isn't already marked as done, and c) the
    // status register indicates a timeout has occurred.
    if ((!DdrtReadPendingQueueTimedOut (Host, Socket, Channel)) && (!*ChannelDone)) {
      CpgcDdrtErrCtlSts.Data = MemReadPciCfgEp (Socket, Channel, CPGC_DDRT_ERR_CTL_STS_MC_2LM_REG);
      if (CpgcDdrtErrCtlSts.Bits.timeout_error == 1) {
        SetDdrtReadPendingQueueTimedOut (Host, Socket, Channel, TRUE);
      }
    }

    // If a timeout has occurred (either in this do/while iteration or a previous one), mark testing as being done
    // for the channel.
    if (DdrtReadPendingQueueTimedOut (Host, Socket, Channel)) {
      *ChannelDone = TRUE;
    }

  } // if (DdrtReadPendingQueueTimeoutEnabled && IsDdrtDimm)

  return EFI_SUCCESS;
}

/** Poll on channel-specific indicators of test completion.

  Polling will end when all active channels on the indicated memory controller have either finished testing.

  @param[in]  Host                                Pointer to SysHost
  @param[in]  Socket                              Socket being trained
  @param[in]  MemoryController                    Memory controller to be polled on
  @param[in]  ChannelMask                         Bit mask indicating channel activity (active = 1, inactive = 0)
  @param[in]  SubChMask                           Bit mask indicating sub-channel activity (active = 1, inactive = 0)
  @param[in]  DdrtReadPendingQueueTimeoutEnabled  Array indicating whether the Read Pending Queue timeout is enabled
                                                  for a given channel

  @retval EFI_SUCCESS   No errors were encountered.
  @retval !EFI_SUCCESS  An error was encountered.

**/
EFI_STATUS
PollUntilChannelTestingDone (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     MemoryController,
  IN  UINT32    ChannelMask,
  IN  UINT8     SubChMask,
  IN  BOOLEAN   DdrtReadPendingQueueTimeoutEnabled[MAX_CH]
  )
{
  BOOLEAN                              AllChannelsDone;
  UINT8                                Channel;
  UINT8                                SubCh;
  BOOLEAN                              ChannelDone;
  CPGC_SEQ_STATUS_MCCPGC_MAIN_STRUCT   CpgcSeqStatus;
  EFI_STATUS                           EfiStatus = EFI_SUCCESS;
  SYS_SETUP                            *Setup;
  BOOLEAN                              ShouldBreak;
  EFI_STATUS                           TemporaryEfiStatus;
  UINT8                                NumChPerMc;
  UINT64                               StartCount;
  UINT32                               TimeOut = 5000000; // 5s

  NumChPerMc = GetNumChannelPerMc ();
  Setup = GetSysSetupPointer ();
  StartCount = GetCountTsc ();

  // Poll on all channels until the test is done and errors are cleared.
  do { // while (!AllChannelsDone)

    AllChannelsDone = TRUE;

    // For each active channel in the MC, check whether the test is done or the DDRT RPQ is hung.
    for (Channel = MemoryController * NumChPerMc;
         Channel < (MemoryController + 1) * NumChPerMc;
         Channel++) {

      if (!ChannelActive (ChannelMask, Channel)) {
        continue;
      }

      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        if (!(SubChMask & (1 << SubCh))) {
          continue;
        }
        CpgcSeqStatus.Data = MemReadPciCfgEp (Socket, Channel, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_STATUS_MCCPGC_MAIN_REG));
        ChannelDone = (BOOLEAN) (CpgcSeqStatus.Bits.test_done == 1);

        // Accumulate done statuses for channels in the memory controller.
        AllChannelsDone = AllChannelsDone && ChannelDone;
      } // for (SubCh)

      // Mark the channel as done if DDRT-specific conditions are met.
      TemporaryEfiStatus = CheckDdrtChannelDone (Host, Socket, Channel, DdrtReadPendingQueueTimeoutEnabled, &ChannelDone);
      if (EfiStatus == EFI_SUCCESS) {
        EfiStatus = TemporaryEfiStatus;
      }
    } // for (Channel)

    BreakChannelTestTarget (&ShouldBreak);
    if (GetDelayTsc (StartCount) > TimeOut) {

      //
      // ShouldBreak is not necessarily due to error, for example
      // RcSim will have ShouldBreak set to TRUE. But if the timeout
      // is exceeded that is a fatal error
      //

      if (FeaturePcdGet (PcdCteBuild) != TRUE) {
        RcDebugPrint (SDBG_ERROR, "PollUntilChannelTestingDone timeout error. Status = 0x%08x\n", CpgcSeqStatus.Data);
        RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_219);
      }

      ShouldBreak = TRUE;
    }
    if (ShouldBreak) {
      break;
    }

    // If this is a CTE build, delay between reads so we don't poll so often.
    CteDelayDclk (10);

    CountTrackingData (PTC_POLLING_COUNT, 1);

  } while (!AllChannelsDone);

  // Generate uBIOS assembly.
  if (Setup->common.ddrtXactor == 0 && GetUbiosFnvAccessValue ()) {

    for (Channel = MemoryController * NumChPerMc;
         Channel < (MemoryController + 1) * NumChPerMc;
         Channel++) {

      if (!ChannelActive (ChannelMask, Channel)) {
        continue;
      }
      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        if (!(SubChMask & (1 << SubCh))) {
          continue;
        }
        // Poll on per-channel test done status bits.
        EmulationPoll (Host, Socket, Channel, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_STATUS_MCCPGC_MAIN_REG), BIT31, 1);
      }
    } // for (Channel)

  } // if (Setup->common.ddrtXactor == 0 && GetUbiosFnvAccessValue ())

  return EfiStatus;

} // PollUntilChannelTestingDone

/** Poll on the global (memory controller-level) clear errors bit until it deasserts.

  @param[in]  Host              Pointer to SysHost
  @param[in]  Socket            Socket being trained
  @param[in]  MemoryController  Memory controller to be polled on
  @param[in]  ChannelMask       Bit mask indicating channel activity (active = 1, inactive = 0)
  @param[in]  SubChMask         Bit mask indicating sub-channel activity (active = 1, inactive = 0)

  @retval EFI_SUCCESS   No errors were encountered
  @retval !EFI_SUCCESS  An error was encountered

**/
EFI_STATUS
PollOnGlobalClearErrors (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     MemoryController,
  IN  UINT32    ChannelMask,
  IN  UINT8     SubChMask
  )
{
  CPGC_SEQ_CTL_MCCPGC_MAIN_STRUCT      SeqCtl;
  SYS_SETUP                            *Setup;
  BOOLEAN                              ShouldBreak;
  BOOLEAN                              AllChannelsDone;
  UINT8                                Channel;
  UINT8                                SubCh;
  BOOLEAN                              ChannelDone;
  UINT8                                NumChPerMc;
  UINT64                               StartCount;
  UINT32                               TimeOut = 5000000; // 5s

  NumChPerMc = GetNumChannelPerMc ();
  Setup = GetSysSetupPointer ();
  StartCount = GetCountTsc ();

  do {

    AllChannelsDone = TRUE;

    // For each active channel in the MC, check whether the test is done or the DDRT RPQ is hung.
    for (Channel = MemoryController * NumChPerMc;
         Channel < (MemoryController + 1) * NumChPerMc;
         Channel++) {

      if (!ChannelActive (ChannelMask, Channel)) {
        continue;
      }

      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        if (!(SubChMask & (1 << SubCh))) {
          continue;
        }
        SeqCtl.Data = MemReadPciCfgEp (Socket, Channel, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_CTL_MCCPGC_MAIN_REG));
        ChannelDone = (BOOLEAN) (SeqCtl.Bits.clear_errors == 0);

        // Accumulate done statuses for channels in the memory controller.
        AllChannelsDone = AllChannelsDone && ChannelDone;
      }
    }
    BreakChannelTestTarget (&ShouldBreak);
    if (GetDelayTsc (StartCount) > TimeOut) {
      ShouldBreak = TRUE;
    }
    if (ShouldBreak) {
      break;
    }

    // If this is a CTE build, delay between reads so we don't poll so often.
    CteDelayDclk (10);

    CountTrackingData (PTC_POLLING_COUNT, 1);

  } while (!AllChannelsDone);

  // Generate uBIOS assembly.
  if (Setup->common.ddrtXactor == 0 && GetUbiosFnvAccessValue ()) {
    for (Channel = MemoryController * NumChPerMc;
         Channel < (MemoryController + 1) * NumChPerMc;
         Channel++) {

      if (!ChannelActive (ChannelMask, Channel)) {
        continue;
      }
      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        if (!(SubChMask & (1 << SubCh))) {
          continue;
        }
        // CPGC_SEQ_CTL_MCCPGC_MAIN_REG[2] is clear_errs.
        EmulationPoll (Host, Socket, Channel, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_CTL_MCCPGC_MAIN_REG), BIT2, 0);
      }
    }
  }

  return EFI_SUCCESS;

} // PollOnGlobalClearErrors

/** Conclude testing/polling at the memory controller-level.

  If the indicated memory controller hasn't timed out, polling will occur until the memory controller indicates that
  testing. However, if the memory controller *has* timed out, CPGC will be forced to stop.

  @param[in]  Host              Pointer to SysHost
  @param[in]  Socket            Socket being trained
  @param[in]  MemoryController  Memory controller to be polled on
  @param[in]  ChannelMask       Bit mask indicating channel activity (active = 1, inactive = 0)
  @param[in]  SubChMask         Bit mask indicating sub-channel activity (active = 1, inactive = 0)

  @retval EFI_SUCCESS   No errors were encountered
  @retval !EFI_SUCCESS  An error was encountered

**/
EFI_STATUS
ConcludeMemoryControllerTesting (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     MemoryController,
  IN  UINT32    ChannelMask,
  IN  UINT8     SubChMask
  )
{
  CPGC_SEQ_CTL_MCCPGC_MAIN_STRUCT      SeqCtl;
  SYS_SETUP                            *Setup;
  BOOLEAN                              ShouldBreak;
  BOOLEAN                              AllChannelsDone;
  UINT8                                Channel;
  UINT8                                SubCh;
  BOOLEAN                              ChannelDone;
  UINT8                                NumChPerMc;
  UINT64                               StartCount;
  UINT32                               TimeOut = 5000000; // 5s
  BOOLEAN                              MemoryControllerTimedOut = FALSE;

  NumChPerMc = GetNumChannelPerMc ();
  Setup = GetSysSetupPointer ();
  StartCount = GetCountTsc ();

  // Determine whether any channel in the memory controller timed out.
  for (Channel = MemoryController * NumChPerMc;
       Channel < (MemoryController + 1) * NumChPerMc;
       Channel++) {
    if (DdrtReadPendingQueueTimedOut (Host, Socket, Channel)) {
      MemoryControllerTimedOut = TRUE;
      break;
    }
  }

  // Perform final recovery or polling.
  if (MemoryControllerTimedOut) {

    for (Channel = MemoryController * NumChPerMc;
         Channel < (MemoryController + 1) * NumChPerMc;
         Channel++) {

      if (!ChannelActive (ChannelMask, Channel)) {
        continue;
      }
      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        if (!(SubChMask & (1 << SubCh))) {
          continue;
        }
        // Completely stop CPGC if timed out.
        SeqCtl.Data = MemReadPciCfgEp (Socket, Channel, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_CTL_MCCPGC_MAIN_REG));
        SeqCtl.Bits.stop_test = 1;
        SeqCtl.Bits.start_test = 0;
        SeqCtl.Bits.clear_errors = 1;
        MemWritePciCfgEp (Socket, Channel, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_CTL_MCCPGC_MAIN_REG), SeqCtl.Data);
      }
    }
  } else {

    // If the memory controller didn't time out, wait for the memory controller to indicate that the test is done. This
    // prevents a race condition/timing issue based on using only the channel-level bits. (Note:
    // CPGC_GLOBALCTRL.start_test won't go low if a timeout occurred.)
    do {

      AllChannelsDone = TRUE;

      // For each active channel in the MC, check whether the test is done or the DDRT RPQ is hung.
      for (Channel = MemoryController * NumChPerMc;
           Channel < (MemoryController + 1) * NumChPerMc;
           Channel++) {

        if (!ChannelActive (ChannelMask, Channel)) {
          continue;
        }

        for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
          if (!(SubChMask & (1 << SubCh))) {
            continue;
          }
          SeqCtl.Data = MemReadPciCfgEp (Socket, Channel, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_CTL_MCCPGC_MAIN_REG));
          ChannelDone = (BOOLEAN) (SeqCtl.Bits.start_test == 0);

          // Accumulate done statuses for channels in the memory controller.
          AllChannelsDone = AllChannelsDone && ChannelDone;
        }
      }
      BreakChannelTestTarget (&ShouldBreak);
      if (GetDelayTsc (StartCount) > TimeOut) {
        ShouldBreak = TRUE;
      }
      if (ShouldBreak) {
        break;
      }

      // If this is a CTE build, delay between reads so we don't poll so often.
      CteDelayDclk (10);

      CountTrackingData (PTC_POLLING_COUNT, 1);
    } while (!AllChannelsDone);
  }

  // Generate uBIOS assembly.
  if (Setup->common.ddrtXactor == 0 && GetUbiosFnvAccessValue ()) {
    for (Channel = MemoryController * NumChPerMc;
         Channel < (MemoryController + 1) * NumChPerMc;
         Channel++) {

      if (!ChannelActive (ChannelMask, Channel)) {
        continue;
      }
      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        if (!(SubChMask & (1 << SubCh))) {
          continue;
        }
        // CPGC_SEQ_CTL_MCCPGC_MAIN_REG[0] is start_test.
        EmulationPoll (Host, Socket, Channel, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_CTL_MCCPGC_MAIN_REG), BIT0, 0);
      }
    }
  }

  return EFI_SUCCESS;

} // ConcludeMemoryControllerTesting

/** Clean up the DDRT Read Pending Queue state following a test.

  This clears bits related to timeout errors and drains the DDRT Read Pending Queue if a timeout occurred.

  @param[in]  Host                                Pointer to SysHost
  @param[in]  Socket                              Socket being trained
  @param[in]  MemoryController                    Memory controller to be polled on
  @param[in]  ChannelMask                         Bit mask indicating channel activity (active = 1, inactive = 0)
  @param[in]  DdrtReadPendingQueueTimeoutEnabled  Array indicating whether the Read Pending Queue timeout is enabled
                                                  for a given channel

  @retval EFI_SUCCESS   No errors were encountered.
  @retval !EFI_SUCCESS  An error was encountered.

**/
EFI_STATUS
CleanUpDdrtReadPendingQueue (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     MemoryController,
  IN  UINT32    ChannelMask,
  IN  BOOLEAN   DdrtReadPendingQueueTimeoutEnabled[MAX_CH]
  )
{
  UINT8                                Channel;
  CPGC_DDRT_ERR_CTL_STS_MC_2LM_STRUCT  CpgcDdrtErrCtlSts;
  UINT8                                Dimm;
  UINT8                                NumChPerMc;

  NumChPerMc = GetNumChannelPerMc ();

  // Perform cleanup on active channels with the credit limit set if the rank under test has DDRT.
  for (Channel = MemoryController * NumChPerMc;
       Channel < (MemoryController + 1) * NumChPerMc;
       Channel++) {

    if (ChannelActive (ChannelMask, Channel) &&
        DdrtReadPendingQueueTimeoutEnabled[Channel] &&
        IsDdrtDimm (Host, Socket, Channel)) {

      // If the channel is done being tested, clear the DDRT scheduler's error bits.
      CpgcDdrtErrCtlSts.Data = MemReadPciCfgEp (Socket, Channel, CPGC_DDRT_ERR_CTL_STS_MC_2LM_REG);
      CpgcDdrtErrCtlSts.Bits.crdt_limit_hit = 0;
      CpgcDdrtErrCtlSts.Bits.timeout_error = 0;
      MemWritePciCfgEp (Socket, Channel, CPGC_DDRT_ERR_CTL_STS_MC_2LM_REG, CpgcDdrtErrCtlSts.Data);

      // Drain the Read Pending Queue if a timeout has occurred.
      if (DdrtReadPendingQueueTimedOut (Host, Socket, Channel)) {
        for (Dimm = 0; Dimm < MAX_DIMM; ++Dimm) {
          RPQDrain (Host, Socket, Channel, Dimm, 0); // AEP only has rank 0.
        }
      } // if (DdrtReadPendingQueueTimedOut (Host, Channel))

    } // if (ChannelActive && DdrtReadPendingQueueTimeoutEnabled && IsDdrtDimm)

  } // Channel loop

  return EFI_SUCCESS;
} // CleanUpDdrtReadPendingQueue

/** CPGC Global Poll function

  @param[in]  Host      Pointer to sysHost
  @param[in]  Socket    Socket number
  @param[in]  ChMask    Bit mask of channels to use
  @param[in]  SubCh     Sub-channel number to use

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcPollGlobalTestDone (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT32    ChMask,
  IN  UINT8     SubCh
  )
{
  EFI_STATUS  EfiStatus = EFI_SUCCESS;
  UINT8       MemoryController;
  BOOLEAN     DdrtReadPendingQueueTimeoutEnabled[MAX_CH];
  UINT8       Status = SUCCESS;

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CpgcPollGlobalTestDone Starts\n");

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {
    if (Host->DdrioUltSupport) {
      CteWaitPatternDone ();
      return Status;
    }
  }

  if (SubCh == NO_SUBCH) {
    SubCh = SUB_CH_A; // DDR4 Core code uses NO_SUBCH. As to CPGC2, only SUB_CH_A applies for DDR4.
  }

  // Poll each memory controller until errors are cleared and tests (loopback) are started.
  for (MemoryController = 0; MemoryController < MAX_IMC; MemoryController++) {

    if (!MemoryControllerActive (ChMask, MemoryController)) {
      continue;
    }

    // Setup
    EfiStatus = SetUpTestDonePolling (Host, Socket, MemoryController, ChMask, DdrtReadPendingQueueTimeoutEnabled);
    if (EfiStatus != EFI_SUCCESS) {
      Status = FAILURE;
    }

    // Execution
    EfiStatus = PollUntilChannelTestingDone (Host, Socket, MemoryController, ChMask, GetSubChMask (SubCh), DdrtReadPendingQueueTimeoutEnabled);
    if (EfiStatus != EFI_SUCCESS) {
      Status = FAILURE;
    }

    EfiStatus = PollOnGlobalClearErrors (Host, Socket, MemoryController, ChMask, GetSubChMask (SubCh));
    if (EfiStatus != EFI_SUCCESS) {
      Status = FAILURE;
    }

    EfiStatus = ConcludeMemoryControllerTesting (Host, Socket, MemoryController, ChMask, GetSubChMask (SubCh));
    if (EfiStatus != EFI_SUCCESS) {
      Status = FAILURE;
    }

    // Cleanup
    EfiStatus = CleanUpDdrtReadPendingQueue (Host, Socket, MemoryController, ChMask, DdrtReadPendingQueueTimeoutEnabled);
    if (EfiStatus != EFI_SUCCESS) {
      Status = FAILURE;
    }

  } // for (MemoryController)

  // Delay only if this is a CTE build, otherwise do nothing.
  CteDelayQclk (30);

  if (Status == FAILURE) {
    RcDebugPrintWithDevice (SDBG_MAX | SDBG_CPGC, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "CpgcPollGlobalTestDone: Exit -- Return FAILURE\n");
  } else {
    RcDebugPrint (SDBG_CPGC,
      "CpgcPollGlobalTestDone: Exit -- Return SUCCESS\n");
  }

  return Status;
} // CpgcPollGlobalTestDone

/**

  CADB Poll function

  @param[in]  Host          Pointer to sysHost
  @param[in]  Socket        Socket number
  @param[in]  DdrChEnMap    Bit mask of channels to use
  @param[in]  SubChEnMap    Bit mask of Sub channels to use

  @retval SUCCESS / FAILURE

**/
UINT32
CadbPollTestDone (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT32    DdrChEnMap,
  IN  UINT8     SubChEnMap
  )
{
  UINT8       Status = SUCCESS;
  UINT8       Ch = 0;
  UINT32      TimeOut;
  BOOLEAN     ShouldBreak;
  UINT64      StartCount;
  UINT8       SubCh = 0;
  BOOLEAN     SubChTestDone = 1;
  BOOLEAN     ChTestDone;
  BOOLEAN     AllChDone;
  BOOLEAN     TestBusyDone = FALSE;
  CADB_STATUS_MCCADB_MAIN_STRUCT  CadbStatusReg;
  UINT8                           MaxChDdr;


  // Workaround due to CADB is not integrated yet in current Simics model
  // Will remove it after Simics integrates it
  if (IsSimics ()) {
    return SUCCESS;
  }

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "CadbPollTestDone Starts\n");

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {
    if (Host->DdrioUltSupport) {
      CteWaitPatternDone ();
      return Status;
    }
  }

  // Delay if this is a CTE build, otherwise do nothing.
  CteDelayDclk (10);

  TimeOut = 15000000;
  StartCount = GetCount ();
  MaxChDdr = GetMaxChDdr ();

  do {
    AllChDone = TRUE;
    BreakChannelTestTarget (&ShouldBreak);
    if (ShouldBreak == TRUE) {
      break;
    }

    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if (!(DdrChEnMap & (1 << Ch))) {
        continue;
      }

      CadbStatusReg.Data = 0;
      ChTestDone = TRUE;
      for (SubCh = 0; SubCh < CpgcGetValidSubCh (); SubCh ++) {
        if (!((1 << SubCh) & (SubChEnMap))) {
          continue;
        }
        //
        // check the test_done bit
        //
        CadbStatusReg.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CADB (SubCh, CADB_STATUS_MCCADB_MAIN_REG));
        //
        // Need to poll both the test_done and test_busy.
        //
        if (CadbStatusReg.Bits.test_busy == 0) {
          TestBusyDone = TRUE;
        } else {
          TestBusyDone = FALSE;
        }
        SubChTestDone = ((BOOLEAN) CadbStatusReg.Bits.test_done) && TestBusyDone;
        ChTestDone = ChTestDone && SubChTestDone;
      }
      AllChDone = AllChDone && ChTestDone;
    } // ch loop

    //
    // Delay if this is a CTE build, otherwise do nothing.
    //
    CteDelayDclk (10); // delay between reads so we don't poll so often.

    //
    // Timeout after a certain amount of time, and return an error.
    //
    if (GetDelay (StartCount) > TimeOut) {
      Status = FAILURE;
      break;
    }
  } while (!AllChDone);


  // Delay only if this is a CTE build, otherwise do nothing.
  CteDelayQclk (30);
  //
  //Clear test done bit by writing test_done bit with 1
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (!(DdrChEnMap & (1 << Ch))) {
      continue;
    }
    for (SubCh = 0; SubCh < CpgcGetValidSubCh (); SubCh++) {
      if (!((1 << SubCh) & (SubChEnMap))) {
        continue;
      }
      CadbStatusReg.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CADB (SubCh, CADB_STATUS_MCCADB_MAIN_REG));
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CADB (SubCh, CADB_STATUS_MCCADB_MAIN_REG), CadbStatusReg.Data);
    }
  }

  if (Status == FAILURE) {
    RcDebugPrint (SDBG_MAX | SDBG_CPGC,
      "CadbPollTestDone: Exit -- Return FAILURE\n");
  } else {
    RcDebugPrint (SDBG_CPGC,
      "CadbPollTestDone: Exit -- Return SUCCESS\n");
  }

  return Status;
} // CadbPollTestDone


/**

  Get CADB Test Done Status

  @param[in]  Host          Pointer to sysHost
  @param[in]  Socket        Socket number
  @param[in]  Ch            Channel number
  @param[in]  SubCh         Sub Channel number

  @retval TRUE:  Done
          FALSE: Not Done

**/
BOOLEAN
EFIAPI
GetCadbTestDone (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     SubCh
  )
{
  BOOLEAN     SubChTestDone = FALSE;
  BOOLEAN     TestBusyDone = FALSE;

  CADB_STATUS_MCCADB_MAIN_STRUCT  CadbStatusReg;

  //
  // Workaround due to CADB is not integrated in current Simics model
  // Will remove it after Simics enables it.
  //
  if (IsSimics ()) {
    return TRUE;
  }

  CadbStatusReg.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CADB (SubCh, CADB_STATUS_MCCADB_MAIN_REG));
  if (CadbStatusReg.Bits.test_busy == 0) {
    TestBusyDone = TRUE;
  } else {
    TestBusyDone = FALSE;
  }
  SubChTestDone = ((BOOLEAN) CadbStatusReg.Bits.test_done) && TestBusyDone;

  return SubChTestDone;
} // CadbPollTestDone

/**

  Disable global start control

  @param[in] Host                - Pointer to sysHost
  @param[in] Socket              - Socket number
  @param[in] Ch                  - Ddr channel number

  @retval N/A

**/
VOID
CpgcDisableGlobalStart (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  UINT8                              SubCh;
  CPGC_SEQ_CTL_MCCPGC_MAIN_STRUCT    SeqCtl;
  CPGC_SEQ_CFG_A_MCCPGC_MAIN_STRUCT  SeqCfgA;

  for (SubCh = 0; SubCh < CpgcGetValidSubCh (); SubCh++) {
    SeqCtl.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_CTL_MCCPGC_MAIN_REG));
    SeqCtl.Bits.single_march_element_mode = 0; // Don't pause after algorithm instruction
    SeqCtl.Bits.continue_single_mode = 0;      // No need fpr continous tests
    SeqCtl.Bits.single_cmd_mode = 0;           // Don't pause command issuing
    SeqCtl.Bits.clear_errors = 1;              // Clear previous errors
    SeqCtl.Bits.stop_test = 0;                 // Stop previous test if any
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_CTL_MCCPGC_MAIN_REG), SeqCtl.Data);

    //
    // Not bind this request engine to all other globally enabled request engines
    //
    SeqCfgA.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_CFG_A_MCCPGC_MAIN_REG));
    SeqCfgA.Bits.global_start_bind = 0;
    SeqCfgA.Bits.global_stop_bind = 0;
    SeqCfgA.Bits.global_clear_err_bind = 0;
    SeqCfgA.Bits.global_stop_on_err_bind = 0;
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_CFG_A_MCCPGC_MAIN_REG), SeqCfgA.Data);
  }
} // CpgcDisableGlobalStart

/**

  CADB Test Start

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket number
  @param[in] DdrChEnMap - Bit mask of channels to use
  @param[in] SubChEnMap - Bit mask of sub-channels to use

  @retval SUCCESS

**/
UINT32
CadbTestStart (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT32    DdrChEnMap,
  IN  UINT8     SubChEnMap
  )
{
  UINT8                             Ch;
  UINT8                             SubCh;
  UINT8                             Imc;
  UINT8                             ImcDone[MAX_IMC];
  CADB_CTL_MCCADB_MAIN_STRUCT       CadbCtlReg;
  UINT8                             NumChPerMc;
  UINT8                             MaxChDdr;

  NumChPerMc = GetNumChannelPerMc ();
  MaxChDdr = GetMaxChDdr ();
  SetMem ((UINT8 *)&ImcDone, sizeof (ImcDone), 0);

  CountTrackingData (PTC_CPGC_COUNT, 1);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (DdrChEnMap & (1 << Ch)) {

      //
      // Fence for posted CSR writes
      // If posted CSR writes enabled, then read from each endpoint behind MEM SS bridge
      // The CADB_CTL_MCCADB_MAIN_REG read acts as feance for MC posted writes
      //
      Imc = Ch / NumChPerMc;
      if (ImcDone[Imc] == 0) {
        FencePostedCsrWritesDdrio (Host, Socket, Imc);
        MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CADB (0, CADB_CTL_MCCADB_MAIN_REG));
        ImcDone[Imc] = 1;
      }
      CadbCtlReg.Data = 0;
      CadbCtlReg.Bits.start_test = 1;
      CadbCtlReg.Bits.stop_test = 0;
      for (SubCh = 0; SubCh < CpgcGetValidSubCh (); SubCh ++) {
        if ((1 << SubCh) & SubChEnMap) {
          MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CADB (SubCh, CADB_CTL_MCCADB_MAIN_REG), CadbCtlReg.Data);
        }
      }
    }
  } // ch loop

  return SUCCESS;
} // CadbTestStart

/**

  CADB Test Stop

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket number
  @param[in] DdrChEnMap - Bit mask of channels to use
  @param[in] SubChEnMap - Bit mask of sub-channels to use

  @retval SUCCESS

**/
UINT32
CadbTestStop (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT32    DdrChEnMap,
  IN  UINT8     SubChEnMap
  )
{
  UINT8                             Ch;
  UINT8                             SubCh;
  CADB_CTL_MCCADB_MAIN_STRUCT       CadbCtlReg;
  UINT8                             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (DdrChEnMap & (1 << Ch)) {
      CadbCtlReg.Data = 0;
      CadbCtlReg.Bits.start_test = 0;
      CadbCtlReg.Bits.stop_test = 1;
      for (SubCh = 0; SubCh < CpgcGetValidSubCh (); SubCh++) {
        if ((1 << SubCh) & SubChEnMap) {
          MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CADB (SubCh, CADB_CTL_MCCADB_MAIN_REG), CadbCtlReg.Data);
        }
      }
    }
  } // ch loop

  //
  // Poll test completion
  //
  CadbPollTestDone (Host, Socket, DdrChEnMap, SubChEnMap);

  return SUCCESS;
} // CadbTestStop

/**

  CPGC Global Start Start

  @param[in]  Host      Pointer to sysHost
  @param[in]  Socket    Socket number
  @param[in]  ChMask    Bit mask of channels to use
  @param[in]  SubCh     Sub-channel number

  @retval     SUCCESS

**/
UINT32
CpgcGlobalTestStart (
  IN        PSYSHOST        Host,
  IN        UINT8           Socket,
  IN        UINT32          ChMask,
  IN        UINT8           SubCh
  )
{
  UINT8                             Ch;
  CPGC_SEQ_CTL_MCCPGC_MAIN_STRUCT   SeqCtl;
  UINT8                             McId = 0;
  CPGC2_RASTER_REPO_CONFIG_MCCPGC_MAIN_STRUCT            RasterRepoConfig;
  UINT8                             MaxChDdr;
  MRC_TT                            CurrentTestType;

  CurrentTestType = GetCurrentTestType (Socket);
  MaxChDdr = GetMaxChDdr ();
  RcDebugPrintWithDevice (SDBG_CPGC, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CpgcGlobalTestStart Starts\n");

  if (SubCh == NO_SUBCH) {
    SubCh = SUB_CH_A; // DDR4 Core code uses NO_SUBCH. As to CPGC2, only SUB_CH_A applies for DDR4.: the comments are not correct.
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (!(ChMask & (1 << Ch))) {
      continue;
    }
    //
    // Fence for posted CSR writes
    // If posted CSR writes enabled, then read from each endpoint behind MEM SS bridge
    // The CPGC_SEQ_CTL_MCCPGC_MAIN_REG read acts as feance for MC posted writes
    //
    McId = GetMCID (Host, Socket, Ch);
    FencePostedCsrWritesDdrio (Host, Socket, McId);
    SeqCtl.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_CTL_MCCPGC_MAIN_REG));

    //
    // Delay only if this is a CTE build, otherwise do nothing.
    //
    CteDelayQclk (10); // slight delay to allow everything to settle before starting.
    //
    // Put the rasterrepoclear here before set the clear_errors
    //
    RasterRepoConfig.Data = (UINT16) MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_A, CPGC2_RASTER_REPO_CONFIG_MCCPGC_MAIN_REG));
    RasterRepoConfig.Bits.rasterrepoclear = 1; //Do it in the test start before setting clear_errors. Reset the raster repo entries. This bit auto-clears
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_A, CPGC2_RASTER_REPO_CONFIG_MCCPGC_MAIN_REG), RasterRepoConfig.Data);
    RasterRepoConfig.Data = (UINT16) MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_B, CPGC2_RASTER_REPO_CONFIG_MCCPGC_MAIN_REG));
    RasterRepoConfig.Bits.rasterrepoclear = 1; //Do it in the test start before setting clear_errors. Reset the raster repo entries. This bit auto-clears
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_B, CPGC2_RASTER_REPO_CONFIG_MCCPGC_MAIN_REG), RasterRepoConfig.Data);
    SeqCtl.Bits.start_test = 1;
    SeqCtl.Bits.stop_test = 0;
    SeqCtl.Bits.clear_errors = 1;
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_CTL_MCCPGC_MAIN_REG), SeqCtl.Data);

    CountTrackingData (PTC_CPGC_COUNT, 1);
  } // ch loop

  //
  // Delay if this is a CTE build, otherwise do nothing.
  //

  CteDelayDclk (10); // slight delay to allow everything to settle before starting.

  return SUCCESS;
}

/**

    CPGC Global Test Stop

    @param Host       - Pointer to sysHost
    @param socket     - socket number
    @param ddrChEnMap - bit mask of channels to use

    @retval SUCCESS

**/
UINT32
CpgcGlobalTestStop (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT32    ddrChEnMap,
  UINT8     SubCh
  )
{
  UINT8                             Ch;
  CPGC_SEQ_CTL_MCCPGC_MAIN_STRUCT   SeqCtl;
  UINT8                             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  RcDebugPrintWithDevice (SDBG_CPGC, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CpgcGlobalTestStop Starts\n");

  if (SubCh == NO_SUBCH) {
    SubCh = SUB_CH_A; // DDR4 Core code uses NO_SUBCH. As to CPGC2, only SUB_CH_A applies for DDR4.
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (!(ddrChEnMap & (1 << Ch))) {
      continue;
    }

    SeqCtl.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_CTL_MCCPGC_MAIN_REG));
    SeqCtl.Bits.stop_test = 1;
    SeqCtl.Bits.start_test = 0;
    SeqCtl.Bits.clear_errors = 1;
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_CTL_MCCPGC_MAIN_REG), SeqCtl.Data);
  } // ch loop

  CpgcPollGlobalTestDone (Host, Socket, ddrChEnMap, SubCh); // wait for test done.
  return SUCCESS;
}


/**

  DDR loopback control training

  @param Host    - Pointer to sysHost
  @param socket  - Socket to train
  @param ch      - DDR channel to setup
  @param rank    - Logical rank number

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcCtlClkPattern (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  GSM_CSN   signal
  )

{
  return EFI_SUCCESS;
}



/**

  DDR parity based command training

  @param[in] Host     Pointer to sysHost
  @param[in] Socket   Socket to train
  @param[in] Ch       DDR channel to setup
  @param[in] Rank     Logical rank number
  @param[in] SubRank  Sub-rank number
  @param[in] Signal   Signal to test
  @param[in] Mode     CADB Mode (NormalCadbSetup, BacksideCadbSetup, or RestoreCadbPattern)

  @retval SUCCESS   All operations successful
  @retval FAILURE   Failure(s) occurred

**/
UINT32
CpgcAdvCmdParity (
  IN  PSYSHOST                  Host,
  IN  UINT8                     Socket,
  IN  UINT8                     Ch,
  IN  UINT8                     Rank,
  IN  UINT8                     SubRank,
  IN  GSM_CSN                   Signal,
  IN  CPGC_ADV_CMD_PARITY_MODE  Mode
  )
{
  return EFI_SUCCESS;
} // CpgcAdvCmdParity

/**

Fix up the CS field for programming the CADB

@param[in] Cs             - Chip select
@param[in] Subrank        - Sub Rank
@param[in] Pattern        - Data to program
@param[in] EncodedCSMode  - Encoded CS mode

**/
VOID
FixupCS (
  IN UINT8         Cs,
  IN UINT8         Subrank,
  IN PatCadb2Prog0 Prog0[CADB2_LINES],
  IN UINT8         EncodedCSMode
  )
{
  UINT8 CadbLine;
  UINT8 Dimm;

  for (CadbLine = 0; CadbLine < CADB2_LINES; CadbLine++) {
    if (Prog0[CadbLine].cs != 0xFF) {
      Prog0[CadbLine].cs = (~(1 << Cs)) & 0xFF;
    }
    if (EncodedCSMode == 2) {
      Dimm = (Cs / 4) * 4;
      Prog0[CadbLine].cs = Prog0[CadbLine].cs & ~(0xc<< (Dimm));
      Prog0[CadbLine].cs |= (Subrank << (Dimm + 2)); // program subranks CID0/CID1
      Prog0[CadbLine].cid = Subrank;  // program CID2 if its needed
    }
  }
} // FixupCS

/**

  Fix up the Chip Select field for backside CS for programming the CADB

  @param[in]        Cs     Rank chip select
  @param[in, out]   Prog1  Data to program

**/
STATIC VOID
FixupBacksideCS (
  IN      UINT8         Cs,
  IN OUT  PatCadbProg1  Prog1[CADB_LINES]
  )
{
  UINT8 CadbLine;

  for (CadbLine = 0; CadbLine < CADB_LINES; CadbLine++) {
    if (Prog1[CadbLine].cs == 0x3FE) {
      // target CS
      Prog1[CadbLine].cs = (~(1 << Cs)) & 0x3FF;
    } else if (Prog1[CadbLine].cs == 0x3FD) {
      // non-target CS
      Prog1[CadbLine].cs = (~(1 << (Cs ^ 0x1))) & 0x3FF;
    }
  } // CadbLine
} // FixupBacksideCS


/**

  Fix up the address bit field for programming the CADB

  @param sig     - Signal to test
  @param pattern - Data to program

**/
STATIC VOID
FixupAddrBits (
  PSYSHOST      Host,
  GSM_CSN       sig,
  PatCadbProg0  prog0[CADB_LINES],
  PatCadbProg1  prog1[CADB_LINES]
  )
{

} // FixupAddrBits

/**

  Fix up the address bit field for a backside address for programming the CADB

  @param[in]      sig     Signal to test
  @param[in, out] prog0   Data to program
  @param[in, out] prog1   Data to program

**/
STATIC VOID
FixupBacksideAddrBits (
  IN      GSM_CSN       Sig,
  IN OUT  PatCadbProg0  Prog0[CADB_LINES],
  IN OUT  PatCadbProg1  Prog1[CADB_LINES]
  )
{

} // FixupBacksideAddrBits

/**

  Program CADB buffer for Ddr4 functions by converting
  PatCadbProg0/1 to PatCadb2Prog0/1 data structure
  and calling ProgramCADB2 (PatCadb2Prog0/1)

  @param Host         - Pointer to sysHost
  @param Socket       - Socket to train
  @param Ch           - DDR channel to setup
  @param Subch        - DDR5 sub-channel to setup
  @param StartLine    - Start line of buffer
  @param EndLine      - End line of buffer
  @param Prog0        - CADB low 32-bit buffer
  @param Prog1        - CADB high 32-bit buffer

  @retval N/A

**/
VOID
ProgramCADB (
  IN PSYSHOST       Host,
  IN UINT8          Socket,
  IN UINT8          Ch,
  IN UINT8          Subch,
  IN UINT8          StartLine,
  IN UINT8          EndLine,
  IN PatCadbProg0   *Prog0,
  IN PatCadbProg1   *Prog1
  )
{
  UINT8            CadbLine;
  PatCadb2Prog0    CadbBuf0Local[CADB2_LINES];
  PatCadb2Prog1    CadbBuf1Local[CADB2_LINES];

  ZeroMem ((UINT8 *) CadbBuf0Local, sizeof (CadbBuf0Local));
  ZeroMem ((UINT8 *) CadbBuf1Local, sizeof (CadbBuf1Local));

  for (CadbLine = StartLine; CadbLine < EndLine; CadbLine++) {
    CadbBuf1Local[CadbLine - StartLine].ca      = (UINT16) Prog0[CadbLine - StartLine].address;
    CadbBuf0Local[CadbLine - StartLine].cid     = Prog0[CadbLine - StartLine].cid;
    CadbBuf1Local[CadbLine - StartLine].bank    = Prog0[CadbLine - StartLine].bank;
    CadbBuf0Local[CadbLine - StartLine].cs      = (UINT8) Prog1[CadbLine - StartLine].cs;
    CadbBuf1Local[CadbLine - StartLine].control = Prog1[CadbLine - StartLine].control;
    CadbBuf0Local[CadbLine - StartLine].odt     = Prog1[CadbLine - StartLine].odt;
    CadbBuf0Local[CadbLine - StartLine].cke     = Prog1[CadbLine - StartLine].cke;
    CadbBuf0Local[CadbLine - StartLine].par     = Prog1[CadbLine - StartLine].par;
  } // CadbLine loop

  ProgramCADB2 (Host, Socket, Ch, GetSubChMask (Subch), StartLine, EndLine, CADB_BUF_MODE_DDR4, FALSE, CadbBuf0Local, CadbBuf1Local);

  return;

} // ProgramCADB

/**

  Populates the provided CADB row

  @param Host    - Pointer to sysHost
  @param socket  - Socket to train
  @param ch      - DDR channel to setup
  @param rank    - Logical rank number
  @param bank    - MRS to write to
  @param data    - Data to write
  @param side    - Side of the register (A or B)
  @param pdaTag  - PDA

**/
STATIC VOID
ProgCADBRow (
            PSYSHOST      Host,
            UINT8         socket,
            UINT8         ch,
            UINT8         dimm,
            UINT8         rank,
            UINT8         bank,
            UINT32        data,
            UINT8         side,
            UINT8         pdaTag,
            UINT8         control,
            PatCadbProg0  *patCADBProg0,
            PatCadbProg1  *patCADBProg1
            )
{

} // ProgCADBRow


//-----------------------------------------------------------------------------------------------
/**

  This function loads the WDB using PCI accesses

  @param Host              - Pointer to sysHost
  @param socket            - Socket number
  @param ch                - Channel number
  @param NumberOfWdbLines  - Number of cachelines to load
  @param WdbLines          - Pointer to data to write to the WDB

  @retval N/A

**/
STATIC VOID
WdbPreLoadChunk (
                PSYSHOST  Host,
                UINT8     socket,
                UINT8     ch,
                UINT32    *wdbLines
                )
{
} // WdbPreLoadChunk

//-----------------------------------------------------------------------------------------------
/**

Arguments:

  Host        - Pointer to the system Host (root) structure
  ch          - DDR Channel ID

  @retval 0 - success
  @retval 1 - failure

**/
VOID
CpgcSetup64 (
            PSYSHOST Host,
            UINT8  Socket,
            UINT8  Ch,
            UINT8  Dimm,
            UINT32 dpa,
            UINT8  mode,
            UINT8  uiBitMask,
            UINT64_STRUCT *data
            )

{
  UINT8                                                  c;
  MRC_MST                                                MemSsType;
  UINT8                                                  Subchannel;
  TAddressCarry                                          AddressCarry;
  CPGC_SEQ_CFG_A_MCCPGC_MAIN_STRUCT                      SeqCfgA;
  CPGC_SEQ_CTL_MCCPGC_MAIN_STRUCT                        SeqCtl;
  CPGC2_REGION_LOW_N0_MCCPGC_MAIN_STRUCT                 RegionLow0;
  CPGC2_REGION_LOW_N1_MCCPGC_MAIN_STRUCT                 RegionLow1;
  CPGC2_ADDRESS_SIZE_N0_MCCPGC_MAIN_STRUCT               AddressSize0;
  CPGC2_ADDRESS_SIZE_N1_MCCPGC_MAIN_STRUCT               AddressSize1;
  CPGC_ERR_CTL_MCCPGC_MAIN_STRUCT                        ErrCtl;
  CPGC2_BASE_CLOCK_CONFIG_MCCPGC_MAIN_STRUCT             BaseClockConfig;
  UINT32                                                 Patbuf[CPGC_MAX_NUMBER_DPAT_UNISEQ] = {0, 0, 0};
  UINT32                                                 Data32;

  //
  // A7 determines subchannel. Address is interleaved every 64B
  //
  if (dpa & BIT1) {
    Subchannel = 1;
  } else {
    Subchannel = 0;
  }

  WdbPreLoadChip (Host, Socket, Ch, 1, (VOID *) data, 0);

  AddressCarry.AddressDirection = 0;
  AddressCarry.AddressOrder = 0;

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  // Disable CPGC on other channels
  for (c = 0; c < GetMaxCh (Host, Socket, MemSsType); c++) {
    MemWritePciCfgEp (Socket, c, SUBCH_OFFSET_CPGC (0, CPGC_SEQ_CFG_A_MCCPGC_MAIN_REG), 0);
    MemWritePciCfgEp (Socket, c, SUBCH_OFFSET_CPGC (1, CPGC_SEQ_CFG_A_MCCPGC_MAIN_REG), 0);
  }

  SeqCtl.Data = 0;
  SeqCtl.Bits.single_march_element_mode = 0; // Don't pause after algorithm instruction
  SeqCtl.Bits.continue_single_mode = 0;      // No need fpr continous tests
  SeqCtl.Bits.single_cmd_mode = 0;           // Don't pause command issuing
  SeqCtl.Bits.clear_errors = 0;              // No need to clear errors for the test
  SeqCtl.Bits.stop_test = 0;                 // Stop previous test if any
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (Subchannel, CPGC_SEQ_CTL_MCCPGC_MAIN_REG), SeqCtl.Data);

  SeqCfgA.Data = 0;
  SeqCfgA.Bits.channel_assign = 1;
  SeqCfgA.Bits.initialization_mode = CPGC_Active_Mode; // Test Mode
                                                       // Bind this request engine to all other globally enabled request engines
  SeqCfgA.Bits.global_start_bind = 1;
  SeqCfgA.Bits.global_stop_bind = 1;
  SeqCfgA.Bits.global_clear_err_bind = 1;
  SeqCfgA.Bits.global_stop_on_err_bind = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (Subchannel, CPGC_SEQ_CFG_A_MCCPGC_MAIN_REG), SeqCfgA.Data);

  BaseClockConfig.Data = 0;
  BaseClockConfig.Bits.increment_value = 1; // Wait timer advance by 1
  BaseClockConfig.Bits.clock_freq = 1; // Use native MC clock
  BaseClockConfig.Bits.prescaler_for_clock_freq = 1; // No divide for MC provided clock (divided by 1)
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (Subchannel, CPGC2_BASE_CLOCK_CONFIG_MCCPGC_MAIN_REG), BaseClockConfig.Data);

  AddressSize0.Data = 0;
  AddressSize0.Bits.block_size_bits_col = 1;
  AddressSize0.Bits.block_size_bits_row = 1;
  AddressSize0.Bits.block_size_max_bank = 0;
  AddressSize0.Bits.block_size_max_rank = 0;
  GetSetCpgcRegs (Host, Socket, Ch, Subchannel, CPGC_REG_WRITE, CPGC2_ADDRESS_SIZE_N0_MCCPGC_MAIN_REG, &AddressSize0.Data);

  // Make region same as block, doesn't matter
  AddressSize1.Data = 0;
  AddressSize1.Bits.bl_chop_en = 0; // No half-line requests
  AddressSize1.Bits.request_data_size = 1;  // 64B transactions
  AddressSize1.Bits.region_size_bits_col = 1;
  AddressSize1.Bits.region_size_bits_row = 1;
  AddressSize1.Bits.region_size_max_bank = 0;
  AddressSize1.Bits.region_size_max_rank = 0;
  GetSetCpgcRegs (Host, Socket, Ch, Subchannel, CPGC_REG_WRITE, CPGC2_ADDRESS_SIZE_N1_MCCPGC_MAIN_REG, &AddressSize1.Data);

  RegionLow0.Data = 0;
  RegionLow1.Data = 0;
  RegionLow0.Bits.low_col = dpa;
  RegionLow1.Bits.low_row = 0;
  RegionLow1.Bits.low_bank = 0;
  RegionLow0.Bits.low_rank = 0;
  GetSetCpgcRegs (Host, Socket, Ch, Subchannel, CPGC_REG_WRITE, CPGC2_REGION_LOW_N0_MCCPGC_MAIN_REG, &RegionLow0.Data);
  GetSetCpgcRegs (Host, Socket, Ch, Subchannel, CPGC_REG_WRITE, CPGC2_REGION_LOW_N1_MCCPGC_MAIN_REG, &RegionLow1.Data);

  Data32 = 0;
  GetSetCpgcRegs (Host, Socket, Ch, Subchannel, CPGC_REG_WRITE, CPGC2_BASE_REPEATS_MCCPGC_MAIN_REG, &Data32); // 1 cacheline
  GetSetCpgcRegs (Host, Socket, Ch, Subchannel, CPGC_REG_WRITE, CPGC2_BLOCK_REPEATS_MCCPGC_MAIN_REG, &Data32); // 1 loopcount
  GetSetCpgcRegs (Host, Socket, Ch, Subchannel, CPGC_REG_WRITE, CPGC2_DATA_CONTROL_MCCPGC_MAIN_REG, &Data32);
  GetSetCpgcRegs (Host, Socket, Ch, Subchannel, CPGC_REG_WRITE, CPGC2_BASE_COL_REPEATS_MCCPGC_MAIN_REG, &Data32);
  GetSetCpgcRegs (Host, Socket, Ch, Subchannel, CPGC_REG_WRITE, CPGC2_BASE_ADDRESS_CONTROL_MCCPGC_MAIN_REG, &Data32);
  GetSetCpgcRegs (Host, Socket, Ch, Subchannel, CPGC_REG_WRITE, CPGC_DPAT_CFG_MCCPGC_MAIN_REG, &Data32);
  GetSetCpgcRegs (Host, Socket, Ch, Subchannel, CPGC_REG_WRITE, CPGC_SEQ_CFG_B_MCCPGC_MAIN_REG, &Data32);

  SetCpgcCmdPat (Host, Socket, Ch, Subchannel, mode, &AddressCarry, 0);

  SetCpgcDpatUniseqPat (Host, Socket, Ch, Subchannel, Patbuf);

  // Disable any data/ecc inversion so that we see data from extbuf directly on DQ lanes
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (Subchannel, CPGC_DPAT_INV_DC_MASK_LO_MCCPGC_MAIN_REG), 0);
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (Subchannel, CPGC_DPAT_INV_DC_MASK_HI_MCCPGC_MAIN_REG), 0);
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (Subchannel, CPGC_DPAT_INVDC_CFG_MCCPGC_MAIN_REG), 0);

  //
  // Program Write Data Buffer Related Entries
  //
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (Subchannel, CPGC_DPAT_BUF_CFG_MCCPGC_MAIN_REG), 0);

  //
  // Enable selective_error_enable_chunk and selective_error_enable_cacheline, mask later
  // the bits we don't want to check.
  //
  ErrCtl.Data = 0;
  ErrCtl.Bits.stop_on_n = 1;
  ErrCtl.Bits.stop_on_error_ctl = 1;
  ErrCtl.Bits.errchk_mask_cacheline = uiBitMask;
  ErrCtl.Bits.errchk_mask_chunk = 0xFF;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (Subchannel, CPGC_ERR_CTL_MCCPGC_MAIN_REG), ErrCtl.Data);

  //
  // Do not mask off errors
  //
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (Subchannel, CPGC_ERR_LNEN_LO_MCCPGC_MAIN_REG), 0);
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (Subchannel, CPGC_ERR_LNEN_HI_MCCPGC_MAIN_REG), 0);

}

/**
  This function sets CPGC logic bank to physical bank mappings

  @param[in] Host              - Pointer to sysHost
  @param[in] Socket            - Socket Id
  @param[in] ChbitMask         - Ch Bit mask for which test should be setup for.
  @param[in] Dimm              - Dimm Id

  @retval NA

**/
VOID
SetCpgcBankL2pMapping (
  IN PSYSHOST      Host,
  IN UINT8         Socket,
  IN UINT32        ChbitMask,
  IN UINT8         Dimm
  )
{
  UINT8                                           Ch;
  UINT8                                           SubCh;
  CPGC_SEQ_BANK_L2P_MAPPING_A_MCCPGC_MAIN_STRUCT  SeqBankL2pMappingA;
  struct dimmNvram                                (*DimmNvList)[MAX_DIMM];

  for (Ch = 0; Ch < GetMaxChDdr (); Ch++) {
    if ((ChbitMask & (1 << Ch)) == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    for (SubCh = 0; SubCh < CpgcGetValidSubCh (); SubCh++) {

      SeqBankL2pMappingA.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_BANK_L2P_MAPPING_A_MCCPGC_MAIN_REG));

      SeqBankL2pMappingA.Bits.l2p_bank0_mapping = 0;
      if (((*DimmNvList)[Dimm].sdramCapacity == SPD_8Gb) && IsX4Dimm (Socket, Ch, Dimm)) {
        //
        // x4 8Gb DRAMs: 2 banks per bankgroup
        //
        SeqBankL2pMappingA.Bits.l2p_bank1_mapping = 2;
        SeqBankL2pMappingA.Bits.l2p_bank2_mapping = 4;
        SeqBankL2pMappingA.Bits.l2p_bank3_mapping = 6;
      } else {
        //
        // others: 4 banks per bankgroup
        //
        SeqBankL2pMappingA.Bits.l2p_bank1_mapping = 4;
        SeqBankL2pMappingA.Bits.l2p_bank2_mapping = 8;
        SeqBankL2pMappingA.Bits.l2p_bank3_mapping = 12;
      }

      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_SEQ_BANK_L2P_MAPPING_A_MCCPGC_MAIN_REG), SeqBankL2pMappingA.Data);
    }
  }
}

/*++

  Setup Data Pattern Configuration and Data Pattern Sequencer registers

  @param[in] Host                - Pointer to sysHost
  @param[in] Socket              - Socket number
  @param[in] Ch                  - Channel number
  @param[in] SequencerMode       - SequencerMode struct
  @param[in] *SequencerPattern   - Pointer to array of pattern sequences that are to be programmed

Returns: NA

--*/
VOID
SetupPatternSequencer (
  IN PSYSHOST      Host,
  IN UINT8         Socket,
  IN UINT8         Ch,
  IN SequencerMode SequencerMode,
  IN UINT32        *SequencerPattern
  )
{
  UINT8                                      MaxSubCh;
  UINT8                                      SubCh;
  UINT32                                     MuxCtl[CPGC_MAX_NUMBER_DPAT_UNISEQ];
  UINT32                                     LfsrSize[CPGC_MAX_NUMBER_DPAT_UNISEQ] = {0, 0, 0};

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "SetupPatternSequencer Starts\n");

  MaxSubCh = CpgcGetValidSubCh ();

  MuxCtl[0] = (UINT8) SequencerMode.Bits.seqMode0;
  MuxCtl[1] = (UINT8) SequencerMode.Bits.seqMode1;
  MuxCtl[2] = (UINT8) SequencerMode.Bits.seqMode2;

  for (SubCh = 0; SubCh < MaxSubCh; SubCh++) {
    SetCpgcDpatUniseqCfg (Host, Socket, Ch, SubCh, MuxCtl, LfsrSize);
    SetCpgcDpatUniseqPat (Host, Socket, Ch, SubCh, SequencerPattern);
  }

} //SetupPatternSequencer

/*++

  Set Lfsr Polynomial for Data Pattern

  @param[in] Host                - Pointer to sysHost
  @param[in] Socket              - Socket number
  @param[in] ChbitMask           - Ch Bit mask for which test should be setup for.
  @param[in] Poly0               - LFSR polynomial 0
  @param[in] Poly1               - LFSR polynomial 1
  @param[in] Poly2               - LFSR polynomial 2

Returns: NA

--*/
VOID
SetLfsrPolynomial (
  IN PSYSHOST      Host,
  IN UINT8         Socket,
  IN UINT32        ChbitMask,
  IN UINT32        Poly0,
  IN UINT32        Poly1,
  IN UINT32        Poly2
  )
{
  UINT8  Ch, SubCh;
  UINT32 Poly[CPGC_MAX_NUMBER_DPAT_UNISEQ];

  Poly[0] = Poly0;
  Poly[1] = Poly1;
  Poly[2] = Poly2;

  for (Ch = 0; Ch < GetMaxChDdr (); Ch++) {
    if ((ChbitMask & (1 << Ch)) == 0) {
      continue;
    }
    for (SubCh = 0; SubCh < CpgcGetValidSubCh(); SubCh++) {
      SetCpgcDpatUniseqPoly(Host, Socket, Ch, SubCh, Poly);
    }
  }
} //SetLfsrPolynomial

/**
  Clears the bitwise error mask registers

  @param Host:         Pointer to sysHost
  @param socket:       Processor socket to clear
  @param ch:           Channel to clear

**/
VOID
ClearBwErrorMask (
                 PSYSHOST        Host,
                 UINT8           socket
                 )
{

} // ClearBwErrorMask10nm

/**
  Sets the IOSAV_CH_X_BW_SERR registers to mask off data bits from reporting failures

  @param Host:         Pointer to sysHost
  @param socket:         Processor socket to mask
  @param dimm:         DIMM to mask
  @param rank:         Rank to mask
  @param bitMask:      Bits to be masked

**/
VOID
SetBwErrorMask (
              PSYSHOST        Host,
              UINT8           socket,
              UINT8           dimm,
              UINT8           rank,
              struct bitMask  *mask
              )
{

} // SetBwErrorMask10nm

STATIC VOID
FillWdbPprPattern (
    PSYSHOST  Host,
    UINT8     socket,
    UINT8     ch,
    UINT8     dimm,
    UINT32    dramMask
 )
/*++

  Setup WDB pattern for PPR

  @param Host        - Pointer to sysHost
  @param socket      - Memory controller to train
  @param ch          - ddr channel to setup
  @param dimm        - DIMM id
  @param dram        - dram that is target of PPR sequence

  @retval None

--*/
{

}


UINT32
CpgcSetupPprPattern (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     subRank,
  UINT32    dramMask,
  UINT8     bank,
  UINT32    row
  )
/*++

  CPGC setup for PPR Sequence

  @param Host        - Pointer to sysHost
  @param socket      - Memory controller to train
  @param ch          - ddr channel to setup
  @param dimm        - DIMM id
  @param rank        - rank id
  @param subRank     - sub rank ID
  @param dramMask    - bitmap of dram that is target of PPR sequence
  @param bank        - bank id
  @param row         - row id

  @retval SUCCESS
  @retval FAILURE

--*/
{
  return EFI_SUCCESS;
}


UINT32
CpgcPprTestGlobalSetup (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     subRank,
  UINT32    dramMask,
  UINT8     bank,
  UINT32    row,
  UINT32    pattern
  )
/*++

  DDR Mem test / Mem Init CPGC setup.

  @param Host        - Pointer to sysHost
  @param socket      - Memory controller to train
  @param ch          - ddr channel to setup
  @param dimm        - DIMM id
  @param rank        - rank id
  @param subRank     - sub rank ID
  @param dram        - dram that is target of PPR sequence
  @param bank        - bank id
  @param row         - row id
  @param pattern     - flag to indicate mem init vs. mem test and read vs. write and data pattern

  @retval SUCCESS
  @retval FAILURE

--*/
{
  return EFI_SUCCESS;
} // CpgcPprTestGlobalSetup


UINT32
CpgcPprCleanup (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    chBitMask
  )
/*++

  Clean-up after PPR sequence is done

  @param Host        - Pointer to sysHost
  @param socket      - Memory controller to train
  @param chBitMask   - Bit map of channels to mask off
--*/
{
  return SUCCESS;
}

/**
  This function determines if cpgc is targeting an NVMDIMM Rank

  @param Host:         Pointer to sysHost
  @param socket:       Socket Id
  @param ch:           DDR channel number within the processor socket

  @retval 1:           Rank belongs to AEP Dimm.
  @retval 0:           Rank belongs to DDR4 Dimm
**/

STATIC UINT8
IsAepRank (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    logRank
  )
{
  UINT8            dimm;
  struct dimmNvram (*dimmNvList) [MAX_DIMM];

  dimmNvList = GetDimmNvList (Host, socket, ch);
  dimm = logRank / MAX_RANK_DIMM;
  return ((*dimmNvList) [dimm].DcpmmPresent);
}

STATIC VOID
SetupPmon (
  PSYSHOST        Host,
  UINT8           socket,
  UINT8           ch,
  UINT8           eventSelect,
  UINT8           unitMask
  )
{

}

STATIC UINT32
GetPmonCounter (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch
  )
{
  return EFI_SUCCESS;
}


UINT8
isHynix2013 (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  )
{
  return EFI_SUCCESS;
}
UINT8
IsDdrtDimm (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Ch
  )
{
  CPGC_SEQ_RANK_L2P_MAPPING_A_MCCPGC_MAIN_STRUCT cpgcSeqRanklg2phymap;

  cpgcSeqRanklg2phymap.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (0, CPGC_SEQ_RANK_L2P_MAPPING_A_MCCPGC_MAIN_REG));
  return (IsAepRank (Host, Socket, Ch, (UINT8) cpgcSeqRanklg2phymap.Bits.l2p_rank0_mapping));
}

/**
  Disable CADB AlwaysOn Mode

  @param Socket - Socket number
  @param Ch     - Channel number

  @retval VOID
**/
VOID
DisableCADBAlwaysOnMode (
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{

}

/**
  Program CPGC to enable or disable IN-ORDER execution


  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Ch                  Current Channel (0-Based)
  @param[in]                Enable              TRUE: Enable IN-ORDER, FALSE: Disable IN-ORDER

  @retval                   EFI_SUCCESS         This function completed execution with no warnings/errors

**/
EFI_STATUS
EFIAPI
CpgcInorderModeEnable (
  IN        UINT8           Socket,
  IN        UINT8           Ch,
  IN        BOOLEAN         Enable
  )
{
  SCHEDULER_ENABLES_MCDDC_CTL_STRUCT    SchedulerEnables;
  SYSHOST                               *Host;
  SCHEDULER_ENABLES_BS_MCDDC_CTL_STRUCT SchedulerEnablesBs;

  Host = GetSysHostPointer ();

  SchedulerEnables.Data = MemReadPciCfgEp (Socket, Ch, SCHEDULER_ENABLES_MCDDC_CTL_REG);
  if (Enable) {
    SchedulerEnables.Bits.cpgc_inorder_mode_en = 1;
  } else {
    SchedulerEnables.Bits.cpgc_inorder_mode_en = 0;
  }
  MemWritePciCfgEp (Socket, Ch, SCHEDULER_ENABLES_MCDDC_CTL_REG, SchedulerEnables.Data);

  SchedulerEnablesBs.Data = MemReadPciCfgEp (Socket, Ch, SCHEDULER_ENABLES_BS_MCDDC_CTL_REG);
  if (Enable) {
    SchedulerEnablesBs.Bits.cpgc_inorder_mode_en = 1;
  } else {
    SchedulerEnablesBs.Bits.cpgc_inorder_mode_en = 0;
  }
  MemWritePciCfgEp (Socket, Ch, SCHEDULER_ENABLES_BS_MCDDC_CTL_REG, SchedulerEnablesBs.Data);

  return EFI_SUCCESS;
}

/**

  This function loads the WDB using PCI accesses

  @param[in] Host                   - Pointer to sysHost
  @param[in] Socket                 - Socket number
  @param[in] Ch                     - Channel number
  @param[in] NumberOfWdbLines       - Number of WDB data line to load
  @param[in] Wdb2Lines              - Pointer to data to write to the WDB
  @param[in] StartWdbDataLineIndex  - Start offset data line number on the WDB.

  @retval N/A

**/
VOID
WdbPreLoadChip (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     NumberOfWdbLines,
  IN UINT8     *Wdb2Lines,
  IN UINT8     StartWdbDataLineIndex
  )
{
  UINT8     (*PLine) [MRC_WDB2_LANE_NUM];
  UINT8     SubCh;
  UINT8     Lane, Line;
  UINT32    Data32New, Data32Old;
  UINT32    Mask;

  // Make sure we do not try to write more than the WDB can hold
  if (NumberOfWdbLines + StartWdbDataLineIndex > MAX_WDB2_DATA_LINES) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Warning: WDB cacheline is overflow %d in WDBFill. Limit to %d.\n", NumberOfWdbLines, MAX_WDB2_DATA_LINES - StartWdbDataLineIndex);
    NumberOfWdbLines = MAX_WDB2_DATA_LINES - StartWdbDataLineIndex;
  }

  PLine = (UINT8 (*) [MRC_WDB2_LANE_NUM]) Wdb2Lines;

  for (Lane = 0; Lane < MRC_WDB2_LANE_NUM; Lane++) {

    Data32New = PLine[0][Lane] | (PLine[1][Lane] << 8) | (PLine[2][Lane] << 16) | (PLine[3][Lane] << 24);

    for (SubCh = 0; SubCh < CpgcGetValidSubCh (); SubCh++) {

      Data32Old = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_DPAT_EXTBUF_0_MCCPGC_MAIN_REG + 4 * Lane));

      Mask = 0;

      for (Line = StartWdbDataLineIndex; Line < (StartWdbDataLineIndex + NumberOfWdbLines); Line++) {
         Mask |= (0xFF << (8 * Line));
      }

      Data32New = (Data32Old & ~Mask) | (Data32New & Mask);

      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_DPAT_EXTBUF_0_MCCPGC_MAIN_REG + 4 * Lane), Data32New);
    }
  }

} // WdbPreLoadChip10nm

/**

  This function sets byte group mapping for virtual DQ lanes selection when using WDB

  @param[in] Host                 - Pointer to sysHost
  @param[in] Socket               - Socket number
  @param[in] Ch                   - Channel number
  @param[in] ByteGroup            - Byte group mapping to be set for virtual DQ lanes selection

  @retval N/A

**/
VOID
WdbSetByteGroupMappingChip (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT16    ByteGroup
  )
{
  UINT8                                   SubCh;
  CPGC_DPAT_INVDC_CFG_MCCPGC_MAIN_STRUCT  DpatInvdcCfg;

  for (SubCh = 0; SubCh < CpgcGetValidSubCh (); SubCh++) {
    DpatInvdcCfg.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_DPAT_INVDC_CFG_MCCPGC_MAIN_REG));
    DpatInvdcCfg.Bits.bytegroup_mapping = ByteGroup;
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_DPAT_INVDC_CFG_MCCPGC_MAIN_REG), DpatInvdcCfg.Data);
  }
}


/**
  Initialize CADB pattern buffer

  @param[in]  Host      - Pointer to SysHost
  @param[in]  Socket    - Socket instance

  @retval N/A
**/
VOID
CADBChickenBit (
  IN    PSYSHOST Host,
  IN    UINT8    Socket
  )
{

}


/**

  this function writes to CADB

  @param Host            - include all the MRC data
  @param socket             - Memory Controller
  @param ch              - ch to send command to
  @param rank            - CS to send the command to

**/
VOID
ReadMprCADBSeq (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     side,
  UINT8     mprPage,
  UINT8     mprLoc
  )
{

} // ReadMprCADBSeq

/**

  Reads MPR page/location from given rank and updates associated MprData per DRAM device

  @param Host    - Pointer to sysHost
  @param socket  - Socket Id
  @param MprData - each byte is updated with UI[0-7] data per DRAM device

**/
VOID
ReadMprRankDdr4 (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     mprPage,
  UINT8     mprLoc,
  UINT8     MprData[MAX_STROBE]
  )
{

} // ReadMprRankDdr410nm

#ifdef LRDIMM_SUPPORT
/**

  Reads MPR page/location from given rank and updates associated MprData per DRAM device

  @param Host    - Pointer to sysHost
  @param socket  - Socket Id
  @param MprData - each byte is updated with UI[0-7] data per DRAM device

**/
VOID
ReadBcwDimmDdr4 (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     bcwFunc,
  UINT8     bcwByte,
  UINT8     bcwNibble,
  UINT8     MprData[MAX_STROBE]
  )
{

} // ReadBcwRankDdr4
#endif //LRDIMM_SUPPORT

VOID
InitRtlOffsetChip (
  UINT8     rtlOffset[2]
  )
{

}

/**

  This routine to set CpgcDdrtMiscCtl register
  @param  Host                  - Host pointer
  @param  Socket                - Socket index
  @param  Ch                    - Channel index
  @param  CpgcMaxCredit
  @param  DisableMaxCreditCheck - boolean
  @param  EnableEridReturn      - boolean
  @param  MultiCreditOn         - boolean

  @retval N/A

**/
VOID
SetCpgcDdrtMiscCtl (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Ch,
  BOOLEAN  ChangeMaxCredit,
  UINT8    CpgcMaxCredit,
  BOOLEAN  DisableMaxCreditCheck,
  BOOLEAN  EnableEridReturn,
  BOOLEAN  MultiCreditOn
  )
{

}

/**

  This routine to set  multi_credit_on field in register CpgcDdrtMiscCtl
  @param[in]  Host                  - Host pointer
  @param[in]  Socket                - Socket index
  @param[in]  Ch                    - Channel index
  @param[in]  MultiCreditOn         - boolean

  @retval N/A

**/
VOID
SetCpgcMultiCreditOn (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN BOOLEAN  MultiCreditOn
  )
{
  CPGC_DDRT_MISC_CTL_MC_2LM_STRUCT CpgcDdrtMiscCtl;

  CpgcDdrtMiscCtl.Data = MemReadPciCfgEp (Socket, Ch, CPGC_DDRT_MISC_CTL_MC_2LM_REG);
  CpgcDdrtMiscCtl.Bits.multi_credit_on = MultiCreditOn;
  CpgcDdrtMiscCtl.Bits.cpgc_max_credit = DDRT_CPGC_MAX_CREDIT;
  CpgcDdrtMiscCtl.Bits.cpgc_wait_for_rpq_empty = 1;
  MemWritePciCfgEp (Socket, Ch, CPGC_DDRT_MISC_CTL_MC_2LM_REG, CpgcDdrtMiscCtl.Data);
}

/**

  This routine to set CpgcRpqForceDrain in CpgcDdrtMiscCtl register
  @param  Host                  - Host pointer
  @param  Socket                - Socket index
  @param  Ch                    - Channel index
  @param  CpgcRpqForceDrain     - boolean

  @retval N/A

**/

VOID
SetCpgcRpqForceDrain (
  PSYSHOST Host,
  UINT8 Socket,
  UINT8 Ch,
  BOOLEAN  CpgcRpqForceDrain
  )
{

}

/**

  This routine gets/sets CSRs required by BiosSetZQCalConfig

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param Controller                 - Controller number (0-based)
  @param Enable                     - Specifies whether ZQCal is enabled.  TRUE enables ZQCal; FALSE disables it
  @param PrevEnable                 - Pointer to where the previous ZQCal enable setting will be stored. This value may be NULL

  @retval N/A

**/
VOID
BiosSetZQCalConfigChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Controller,
  BOOLEAN               Enable,
  BOOLEAN               *PrevEnable
  )
{

}


/**

This function applies the given ZQ mask

@param[in] Host      - Pointer to sysHost
@param[in] Socket    - Socket
@param[in] Ch        - Channel
@param[in] ZQMask    - ZQ mask to apply

@retval none

**/
VOID
SetZQMask (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     ZQMask
  )
{
  CPGC_MISCZQCTL_MCDDC_CTL_STRUCT  MiscZqCtl;

  MiscZqCtl.Data = MemReadPciCfgEp (Socket, Ch, CPGC_MISCZQCTL_MCDDC_CTL_REG);
  //
  // ZQ
  //
  MiscZqCtl.Bits.always_do_zq = 0; // ZQ operation will take place at the normal programmed period after a refresh occurs as long as the DIMM/rank does not appear in the DimmRankPairs array
  MiscZqCtl.Bits.zq_rankmask = ZQMask;
  MemWritePciCfgEp (Socket, Ch, CPGC_MISCZQCTL_MCDDC_CTL_REG, MiscZqCtl.Data);

  return;
} // SetZQMask
/**

This function applies the given Refresh mask

@param[in] Host      - Pointer to sysHost
@param[in] Socket    - Socket
@param[in] Ch        - Channel
@param[in] RefMask   - Refresh mask to apply

@retval none

**/
VOID
SetRefreshMask (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     RefMask
  )
{
  CPGC_MISCREFCTL_MCDDC_CTL_STRUCT  MiscRefCtl;

  MiscRefCtl.Data = MemReadPciCfgEp (Socket, Ch, CPGC_MISCREFCTL_MCDDC_CTL_REG);
  //
  // Enable Refresh - 0's to the the desired ranks
  //
  MiscRefCtl.Bits.ref_rankmask = RefMask; // Enable all non-selected ranks
  MemWritePciCfgEp (Socket, Ch, CPGC_MISCREFCTL_MCDDC_CTL_REG, MiscRefCtl.Data);

  return;
} // SetRefreshMask


/**

  This routine to sets or clears refresh enable based on RefreshEnable
  In 10nm version, it introduced stagger host refresh enable feature

  @param[in]  Host                    - Host pointer
  @param[in]  Socket                  - Socket index
  @param[in]  TrefiPerAviableChannel  - Enbable Stragger Trefi Per Available Channel
  @param[in]  NumberOfRank            - Select number of rank to keep stagger host refresh apart
  @param[in]  TrefiDelay              - Apply addition Trefi Delay when enable Host Refresh across different iMC Group
  @retval N/A

**/
VOID
SetRefreshEnable10nm (
  IN   PSYSHOST  Host,
  IN   UINT8     Socket,
  IN   UINT8     TrefiPerAviableChannel,
  IN   UINT8     NumberOfRank,
  IN   UINT16    TrefiDelay
  )
{
  UINT8                             Ch;
  CPGC_MISCCKECTL_MCDDC_CTL_STRUCT  MiscCKECtl[MAX_CH];
  struct channelNvram               (*ChannelNvList)[MAX_CH];
  UINT8                             MaxChDdr;
  BOOLEAN                           FirstActiveCh;
  UINT8                             PrevActiveCh = 0xFF;
  UINT64                            StartTime = 0;
  UINT64                            PrevStartTime = 0;
  UINT32                            TimeDifference;
  UINT32                            StartWriteTime[MAX_CH];
  UINT32                            HostRefreshStartTime[MAX_CH];
  SYS_SETUP                         *Setup;
  UINT8                             NumOfAvailableCh = 0;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  Setup = GetSysSetupPointer ();

  ZeroMem ((UINT32 *) StartWriteTime, sizeof (StartWriteTime));
  ZeroMem ((UINT32 *) HostRefreshStartTime, sizeof (HostRefreshStartTime));

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    NumOfAvailableCh ++;

    //
    // Cached refresh_enable setting
    //
    MiscCKECtl[Ch].Data = MemReadPciCfgEp (Socket, Ch, CPGC_MISCCKECTL_MCDDC_CTL_REG);
    MemWritePciCfgEp (Socket, Ch, CPGC_MISCCKECTL_MCDDC_CTL_REG, MiscCKECtl[Ch].Data);
    MiscCKECtl[Ch].Bits.refresh_enable = 1;
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    if (TrefiPerAviableChannel == 1) {
      HostRefreshStartTime[Ch] = REFRESH_SYNC_TIME / (NumberOfRank * NumOfAvailableCh) + REFRESH_SYNC_TIME;
    } else {
      HostRefreshStartTime[Ch] = REFRESH_SYNC_TIME;
    }
    RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "REFRESH_SYNC_TIME_PerCh= %d\n", HostRefreshStartTime[Ch]);
  } // Ch loop

  //
  // Apply additional Trefi delay when enable Host Refresh across different iMC 01/23 Group
  //
  if ((TrefiDelay > 0) && (MaxChDdr > FirstChanneliMc23)) {
    for (Ch = FirstChanneliMc23; Ch < MaxChDdr; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }
      HostRefreshStartTime[Ch] -= TrefiDelay;
      RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "FirstAvailableChBehindImc23 REFRESH_SYNC_TIME_PerCh= %d\n", HostRefreshStartTime[Ch]);
      break;
    }
  } else {
    //
    // To maintain TREFI apart, compensate the write latency in between previous active channel and current active channel
    // Formula will be REFRESH_SYNC_TIME + (write_delay[prev_active_channel] - write_delay[curr_active_channel]) / 2
    //
    FirstActiveCh = TRUE;
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }

      if (!FirstActiveCh) {
        //
        // While Multi-Socket MRC is enabled or SBPS, CSR write delay of each socket will be consumed directly
        //
        if ((Socket == 0) || ((Setup->mem.options & MULTI_THREAD_MRC_EN) == MULTI_THREAD_MRC_EN)) {
          if (Host->nvram.mem.socket[Socket].CsrWriteLatency[PrevActiveCh] >= Host->nvram.mem.socket[Socket].CsrWriteLatency[Ch]) {
            HostRefreshStartTime[Ch] = HostRefreshStartTime[Ch] + ((Host->nvram.mem.socket[Socket].CsrWriteLatency[PrevActiveCh] - Host->nvram.mem.socket[Socket].CsrWriteLatency[Ch]) / 2);
          } else {
            HostRefreshStartTime[Ch] = HostRefreshStartTime[Ch] - ((Host->nvram.mem.socket[Socket].CsrWriteLatency[Ch] - Host->nvram.mem.socket[Socket].CsrWriteLatency[PrevActiveCh]) / 2);
          }
        } else {
          //
          // For Single-Socket MRC Mode, consume CSR read delay instead of CSR write for remote sockets since CSR write will be Posted Write when executed code in SBSP
          //
          if (Host->nvram.mem.socket[Socket].CsrReadLatency[PrevActiveCh] >= Host->nvram.mem.socket[Socket].CsrReadLatency[Ch]) {
            HostRefreshStartTime[Ch] = HostRefreshStartTime[Ch] + ((Host->nvram.mem.socket[Socket].CsrReadLatency[PrevActiveCh] - Host->nvram.mem.socket[Socket].CsrReadLatency[Ch]) / 2);
          } else {
            HostRefreshStartTime[Ch] = HostRefreshStartTime[Ch] - ((Host->nvram.mem.socket[Socket].CsrReadLatency[Ch] - Host->nvram.mem.socket[Socket].CsrReadLatency[PrevActiveCh]) / 2);
          }
        }
      } else {
        FirstActiveCh = FALSE;
      }
      RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "HostRefreshStartTime= %d\n", HostRefreshStartTime[Ch]);
      PrevActiveCh = Ch;
    }
  }

  //
  // Enable periodic refresh engine
  //
  FirstActiveCh = TRUE;
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (((*ChannelNvList)[Ch].enabled == 1) || (((*ChannelNvList)[Ch].ddrtEnabled == 1) && ((*ChannelNvList)[Ch].mb3trainingfailure == 0))) {
      //
      // Inject delay between refresh enables
      //
      if(!FirstActiveCh) {
        do {
          StartTime = GetPerformanceCounter ();
          TimeDifference = TimeDiff (PrevStartTime, StartTime, TDIFF_UNIT_NS);
          if (!(GetEmulation () & SIMICS_FLAG)) {
            TimeDifference = HostRefreshStartTime[Ch];
          }
        } while (TimeDifference < HostRefreshStartTime[Ch]);
      } else {
        StartTime = GetPerformanceCounter ();
      }

      //StartTime = GetPerformanceCounter ();
      MemWritePciCfgEp (Socket, Ch, CPGC_MISCCKECTL_MCDDC_CTL_REG, MiscCKECtl[Ch].Data);
      //EndTime = GetPerformanceCounter ();

      //
      // Log times since previous refresh enable
      //
      if(!FirstActiveCh) {
        StartWriteTime[Ch] = TimeDiff (PrevStartTime, StartTime, TDIFF_UNIT_NS);
      } else {
        FirstActiveCh = FALSE;
      }
      //
      // Save the current start and end times
      //
      PrevStartTime = StartTime;
    }
  } // Ch loop

  //
  // Print the timing details between mc host refresh enables
  //
  FirstActiveCh = TRUE;
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (((*ChannelNvList)[Ch].enabled == 1) || (((*ChannelNvList)[Ch].ddrtEnabled == 1) && ((*ChannelNvList)[Ch].mb3trainingfailure == 0))) {
      if (FirstActiveCh) {
        FirstActiveCh = FALSE;
      } else {
        RcDebugPrint (SDBG_MINMAX, "EnableHostRefresh Start Write Time diff[%d]=%d ns\n", Ch, StartWriteTime[Ch]);
      }
    }
  } // Priting host refresh enable timing details
}

/**

  This function needs to immediately follow the
  SetRefreshEnable10nm() funtion.  This provides an end label that C
  can use to read the entire function (ExitSelfRefresh10nm) into
  cache before executing it.

**/
UINT8
SetRefreshEnable10nmEndMarker (
  VOID
  )
{
  UINT8 Status;

  Status = 0;
  return Status;
}

/**

This function asserts CKE on each channel and unmasks periodic refresh

@param Host    - Pointer to sysHost
@param socket  - Socket

@retval none

**/
VOID
ExitSelfRefreshCriticalSection (
  PSYSHOST  Host,
  UINT8     socket
  )
{

}

/**

  This function needs to immediately follow the
  ExitSelfRefresh10nm() funtion.  This provides an end label that C
  can use to read the entire function (ExitSelfRefresh10nm) into
  cache before executing it.

**/
UINT8
ExitSelfRefreshCriticalSectionEndMarker (
  VOID
  )
{
  return EFI_SUCCESS;
}


/**

  Return address of CPGC_ERR_DATA0_S_MCDDC_CTL_REG

  @param Host           - Point to sysHost
  @param Socket         - Socket number
  @param Ch             - Channel number

  @retval  Physical address of CPGC_ERR_DATA0_S_MCDDC_CTL_REG

**/
UINT8 *
GetCpgcErrData0sAddr (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  return EFI_SUCCESS;
}

/** Return whether a given channel has hit a Read Pending Queue timeout.

  @param[in] Host     Pointer to sysHost
  @param[in] Channel  Number of the channel to check

  @retval FALSE The timeout has not been hit.
  @retval TRUE  The timeout has been hit.

**/
BOOLEAN
EFIAPI
DdrtReadPendingQueueTimedOut (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{
  return Host->var.mem.DdrtReadPendingQueueTimedOut[Socket][Channel];
}

/** Set the DDRT Read Pending Queue (RPQ) credit limit with the default value.

  If implemented with a non-null library, the credit limit is set for all
  channels with a DDRT DIMM.  However, this should only be implemented with a
  non-null library for projects that need to alternately set and clear the
  credit limit for specific training steps.

  @param[in] Host         Pointer to Host structure
  @param[in] Socket       Processor socket within the system (0-based)

  @retval N/A

**/
VOID
EFIAPI
SetDefaultCpgcRpqCreditLimitIfNeeded (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
  // Do nothing. (No functionality needed.)
}

/** Clear the DDRT Read Pending Queue (RPQ) credit limit.

  If implemented with a non-null library, the credit limit is cleared for all
  channels with a DDRT DIMM.  However, this should only be implemented with a
  non-null library for projects that need to alternately set and clear the
  credit limit for specific training steps.

  @param[in] Host     Pointer to Host structure
  @param[in] Socket   Processor socket within the system (0-based)

  @retval N/A

**/
VOID
EFIAPI
ClearCpgcRpqCreditLimitIfNeeded (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
  // Do nothing. (No functionality needed.)
}

/**

  Finds command margins for the given command group

  @param Host         - Pointer to sysHost
  @param Socket       - Socket number
  @param ChBitmask    - Channel bit mask
  @param SubCh        - Sub channel number.
  @retval N/A

**/
VOID
ExecuteCmdSignalTest (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT32    ChBitmask,
  UINT8     SubCh
  )
{

  //
  // Start test
  //
  CadbTestStart (Host, Socket, ChBitmask, GetSubChMask (SubCh));

  //
  // Poll test completion
  //
  CadbPollTestDone (Host, Socket, ChBitmask, GetSubChMask (SubCh));

  SetCadbModeEn (Host, Socket, ChBitmask, FALSE);

  //
  // Delay if this is a CTE build, otherwise do nothing.
  //
  CteDelayDclk (10);

} // ExecuteCmdSignalTest

/**
This function is used to start a test, stop a test, and clear errors.
This function does not assume that all tests are CPGC tests (i.e. WL
fine test, which is executed in the IO only). This function will use
the last Level and TestType parameters given by SetupTest for the channels.
  @param[in] Host:           Pointer to sysHost
  @param[in] Socket:         Processor socket within the system (0-based)
  @param[in] MemSs           MemSS number within the socket (0-based)
  @param[in] ChannelMask:    Channel mask within the MemSS
  @param[in] SubCh:          Sub channel number
  @param[in] TestType:       Indicates the test type to be configured
  @param[in] ClearErrors:    Clear the error status
  @param[in] StartTest:      Start the test: 1: start, 0: not start.
  @param[in] StopTest:       Stop the test: 1: stop, 0: not stop.

  @retval MRC_STATUS

**/
MRC_STATUS
TestControl (
  IN PSYSHOST      Host,
  IN UINT8         Socket,
  IN UINT32        MemSs,
  IN UINT32        ChannelMask,
  IN UINT8         SubCh,
  IN MRC_TT        TestType,
  IN BOOLEAN       ClearErrors,
  IN BOOLEAN       StartTest,
  IN BOOLEAN       StopTest
  )
{
  UINT32 Status = SUCCESS;
  UINT8  DumArr[7] = { 1, 1, 1, 1, 1, 1, 1 };
  UINT8  MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  if (StartTest) {
    switch (TestType) {
#ifdef DDR5_SUPPORT
      case EarlyCsLoopbackTest:
      case EarlyCaLoopbackTest:
      case EarlyCaBacksideLoopbackTest:
      case BcomTrainingTest:
      case DcaDckDutyCycleTest:
      case DcaDfeDdr5Test:
        SetCadbModeEn (Host, Socket, ChannelMask, TRUE);
        Status = CadbTestStart (Host, Socket, ChannelMask, GetSubChMask (SubCh));
        break;
      case  WLDdr5:
      case  WrDqTest:
      case  MdqsWriteDelay:
      case  DqSlewRateTest:
      case  WritePreDfe2DCenteringTest:
      case  WriteDfeTest:
      case  WriteDbDfeTest:
      case  WritePostDfe2DCenteringTest:
      case  RmtDqTest:
      case  RmtCmdTest:
      case  TurnAroundSgTest:
      case  TurnAroundSrTest:
      case  TurnAroundDrDdRwTest:
      case  TurnAroundDrDdWwRrWrTest:
        Status = RunIOTest (Host, Socket, ChannelMask, BasicVA, DumArr, ClearErrors, 0);
        break;
      case  MdqsReceiveEnablePhaseTraining:
      case  MdqsCoarseReadDelayTraining:
      case  BacksideDqSwizzleDiscovery:
      case  FrontsideDqSwizzleDiscovery:
      case  MdqsReadDelayDeskew:
      case  DqSwzDiscTestDdr5:
      case  RdDqTest:
      case  ReadPreDfe2DCenteringTest:
      case  DramDutyCycleAdjusterTest:
      case  ReadDfeTest:
      case  ReadPostDfe2DCenteringTest:
#endif
      case  RcvenFineTest:
        SetCadbModeEn (Host, Socket, ChannelMask, TRUE);
        Status = RunIOTest (Host, Socket, ChannelMask, BasicVA, DumArr, ClearErrors, 0);
        CadbPollTestDone (Host, Socket, ChannelMask, TWO_SUBCH_MASK);
        SetCadbModeEn (Host, Socket, ChannelMask, FALSE);
        break;
      default:
        Status = CpgcGlobalTestStart (Host, Socket, ChannelMask, SubCh);
    }
  }
  if (StopTest) {
    switch (TestType) {
#ifdef DDR5_SUPPORT
      case EarlyCsLoopbackTest:
      case EarlyCaLoopbackTest:
      case BcomTrainingTest:
      case DcaDckDutyCycleTest:
      case DcaDfeDdr5Test:
        Status = CadbTestStop (Host, Socket, ChannelMask, GetSubChMask (SubCh));
        SetCadbModeEn (Host, Socket, ChannelMask, FALSE);
        break;
      case EarlyCaBacksideLoopbackTest:
        Status = CadbTestStop (Host, Socket, ChannelMask, GetSubChMask (SubCh));
        break;
#endif
      default:
        Status = CpgcGlobalTestStop (Host, Socket, ChannelMask, SubCh);
    }
  }
  return Status;
}

/**
  Calculate CADB Address Parity

  @param[in] Address   CADB Address

 @retval Parity
**/
UINT8
CalculateCadbAddressParity (
  IN UINT32  Address
  )
{
  UINT32 Parity = 0;

  //
  // Address bit parity
  //
  Parity ^= CalculateEvenParity (Address);

  return Parity & BIT0;
}

/**

  Program CADB2 buffer

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Socket to train
  @param[in] Ch           - DDR channel to setup
  @param[in] Subch        - DDR5 sub-channel to setup
  @param[in] StartLine    - Start line of buffer
  @param[in] NumLines     - The number of CADB lines
  @param[in] Mode         - CADB work mode
  @param[in] ParOverWrite - Overwrite parity pattern
  @param[in] Prog0        - CADB low 32-bit buffer
  @param[in] Prog1        - CADB high 32-bit buffer

  @retval N/A

**/
VOID
ProgramCADB2 (
  IN PSYSHOST       Host,
  IN UINT8          Socket,
  IN UINT8          Ch,
  IN UINT8          SubchMask,
  IN UINT8          StartLine,
  IN UINT8          NumLines,
  IN UINT8          Mode,
  IN BOOLEAN        ParOverWrite,
  IN PatCadb2Prog0  *Prog0,
  IN PatCadb2Prog1  *Prog1
  )
{
  EFI_STATUS                                Status;
  UINT8                                     CadbLine;
  UINT32                                    FastCadb = FALSE;
  CADB_BUF_0_N0_MCCADB_MAIN_STRUCT_LOCAL    CadbBuf0Reg;
  CADB_BUF_0_N1_MCCADB_MAIN_STRUCT_LOCAL    CadbBuf1Reg;
  UINT8                                     SubCh = 0;
  UINT8                                     MaxSubCh = 0;
  MRC_TT                                    CurrentTestType;

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "ProgramCADB2 entry\n");
  Status = CteGetOptions ("fastcadb", &FastCadb);
  if (EFI_ERROR (Status)) {
    FastCadb = FALSE;
  }

  CurrentTestType = GetCurrentTestType (Socket);
  MaxSubCh = CpgcGetValidSubCh ();

  //
  // Program 8 lines in the CADB
  //
  if (Mode == CADB_BUF_MODE_DDR5_DESELECT) {
    RcDebugPrint (SDBG_CPGC, "    CS  VALID  CA  PAR_R\n");

    for (CadbLine = StartLine; CadbLine < NumLines + StartLine; CadbLine++) {

      if (Prog0[CadbLine].val == 0) {
        continue;
      }

      CadbBuf0Reg.Data = 0;
      CadbBuf1Reg.Data = 0;
      CadbBuf0Reg.Ddr5Desel.Cs = Prog0[CadbLine].cs;
      CadbBuf0Reg.Ddr5Desel.Val = Prog0[CadbLine].val;
      CadbBuf0Reg.Ddr5Desel.Ca = Prog0[CadbLine].ca;
      if (ParOverWrite) {
        CadbBuf1Reg.Ddr5Desel.Par_r = Prog0[CadbLine].par;
      } else {
        CadbBuf1Reg.Ddr5Desel.Par_r = CalculateCadbAddressParity (((CadbBuf0Reg.Ddr5Desel.Ca) & 0x7f));
      }
      CadbBuf1Reg.Ddr5Desel.Par_f = CalculateCadbAddressParity ((((CadbBuf0Reg.Ddr5Desel.Ca) & 0x3F80) >> 7));//no need thsis for ddr5 rdimm.
      if (CurrentTestType == EarlyRcdCaLoopbackTest || CurrentTestType == DcaDckDutyCycleTest || CurrentTestType == DcaDfeDdr5Test) {
        CadbBuf1Reg.Ddr5Desel.Par_r = (Prog1[CadbLine].ca & 0x00000001);
        CadbBuf1Reg.Ddr5Desel.Par_f = 0;
      }
      RcDebugPrint (SDBG_CPGC, "%02d 0x%02x  %1x  0x%04x  0x%1x\n",
         CadbLine, CadbBuf0Reg.Ddr5Desel.Cs, CadbBuf0Reg.Ddr5Desel.Val, CadbBuf0Reg.Ddr5Desel.Ca, CadbBuf1Reg.Ddr5Desel.Par_r);
      if (FastCadb == FALSE) {
        RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "FastCadb is false and start programing the CADB_BUF_0_N0_MCCADB_MAIN_REG value is %x and CADB_BUF_0_N1_MCCADB_MAIN_REG value is %x \n",
          CadbBuf0Reg.Data, CadbBuf1Reg.Data);
        for (SubCh = 0; SubCh < MaxSubCh; SubCh++) {
          if (!((1 << SubCh) & (SubchMask))) {
            continue;
          }
          SetCadbBuf (Host, Socket, Ch, SubCh, CadbLine, CadbBuf0Reg.Data, CadbBuf1Reg.Data);
        }
      } else {
        RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "FastCadb is true , start CteProgramCadb and program the CADB_BUF_0_N0_MCCADB_MAIN_REG \n");
        CteProgramCadb (SubchMask, CadbLine, CadbBuf0Reg.Data, 0);
      }
    } // cadbLine loop
  } else if (Mode == CADB_BUF_MODE_DDR5_SELECT) {
    RcDebugPrint (SDBG_CPGC, "    CS CS_NT VALID CA0 PAR_R0 PAR_F0 CA1 PAR_R1 PAR_F1\n");

    for (CadbLine = StartLine; CadbLine < NumLines; CadbLine++) {
      CadbBuf0Reg.Data = 0;
      CadbBuf1Reg.Data = 0;
      CadbBuf0Reg.Ddr5Sel.Cs = Prog0[CadbLine].cs;
      CadbBuf0Reg.Ddr5Sel.Cs_nt = 0x0F;
      CadbBuf0Reg.Ddr5Sel.Val = Prog0[CadbLine].val;
      CadbBuf0Reg.Ddr5Sel.Ca_1st = Prog0[CadbLine].ca;

      CadbBuf1Reg.Ddr5Sel.Par_r_1st = CalculateCadbAddressParity (((CadbBuf0Reg.Ddr5Sel.Ca_1st) & 0x7f));
      CadbBuf1Reg.Ddr5Sel.Par_f_1st = CalculateCadbAddressParity ((((CadbBuf0Reg.Ddr5Sel.Ca_1st) & 0x3F80) >> 7));
      CadbBuf1Reg.Ddr5Sel.Ca_2nd = Prog1[CadbLine].ca;
      CadbBuf1Reg.Ddr5Sel.Par_r_2nd = CalculateCadbAddressParity (((CadbBuf1Reg.Ddr5Sel.Ca_2nd) & 0x7f));
      CadbBuf1Reg.Ddr5Sel.Par_f_2nd = CalculateCadbAddressParity ((((CadbBuf1Reg.Ddr5Sel.Ca_2nd) & 0x3F80) >> 7));

      RcDebugPrint (SDBG_CPGC, "%02d 0x%02x  0x%02x  %1x  0x%04x  0x%1x 0x%1x 0x%04x  0x%1x 0x%1x\n",
          CadbLine, CadbBuf0Reg.Ddr5Sel.Cs, CadbBuf0Reg.Ddr5Sel.Cs_nt, CadbBuf0Reg.Ddr5Sel.Val,
      CadbBuf0Reg.Ddr5Sel.Ca_1st, CadbBuf1Reg.Ddr5Sel.Par_r_1st, CadbBuf1Reg.Ddr5Sel.Par_f_1st,
      CadbBuf1Reg.Ddr5Sel.Ca_2nd, CadbBuf1Reg.Ddr5Sel.Par_r_2nd, CadbBuf1Reg.Ddr5Sel.Par_f_2nd);

      if (FastCadb == FALSE) {
        RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "FastCadb is false and start programing the CADB_BUF_0_N0_MCCADB_MAIN_REG value is %x and CADB_BUF_0_N1_MCCADB_MAIN_REG value is %x \n",
          CadbBuf0Reg.Data, CadbBuf1Reg.Data);
        for (SubCh = 0; SubCh < MaxSubCh; SubCh++) {
          if (!((1 << SubCh) & (SubchMask))) {
            continue;
          }
          SetCadbBuf (Host, Socket, Ch, SubCh, CadbLine, CadbBuf0Reg.Data, CadbBuf1Reg.Data);
        }

      } else {
        RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "FastCadb is true , start CteProgramCadb and program the CADB_BUF_0_N0_MCCADB_MAIN_REG \n");
        CteProgramCadb (SubchMask, CadbLine, CadbBuf0Reg.Data, 0);
      }
    } // cadbLine loop
  } else if (Mode == CADB_BUF_MODE_DDR4) {
    for (CadbLine = StartLine; CadbLine < NumLines; CadbLine++) {

      CadbBuf0Reg.Data         = 0;
      CadbBuf1Reg.Data         = 0;
      CadbBuf0Reg.Ddr4.Cs      = Prog0[CadbLine].cs;
      CadbBuf0Reg.Ddr4.Cid     = Prog0[CadbLine].cid;
      CadbBuf0Reg.Ddr4.Odt     = Prog0[CadbLine].odt;
      CadbBuf0Reg.Ddr4.Cke     = Prog0[CadbLine].cke;
      CadbBuf0Reg.Ddr4.Val     = Prog0[CadbLine].val;
      CadbBuf0Reg.Ddr4.Par     = Prog0[CadbLine].par;
      CadbBuf0Reg.Ddr4.Ma      = Prog0[CadbLine].ca;
      CadbBuf1Reg.Ddr4.Control = (Prog1[CadbLine].control & 0x7);  // Bit[2]: MA[16]/RAS_n, Bit[1]: MA[15]/CAS_n, Bit[0]: MA[14]/WE_n
      CadbBuf1Reg.Ddr4.Bank    = Prog1[CadbLine].bank;
      CadbBuf1Reg.Ddr4.Act_n   = (Prog1[CadbLine].control >> 3) & BIT0; // Bit[3]: ACT_n

      if (FastCadb == FALSE) {
        RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "FastCadb is false and start programing the CADB_BUF_0_N0_MCCADB_MAIN_REG value is %x and CADB_BUF_0_N1_MCCADB_MAIN_REG value is %x \n",
          CadbBuf0Reg.Data, CadbBuf1Reg.Data);
        for (SubCh = 0; SubCh < MaxSubCh; SubCh++) {
          if (!((1 << SubCh) & (SubchMask))) {
            continue;
          }
          SetCadbBuf (Host, Socket, Ch, SubCh, CadbLine, CadbBuf0Reg.Data, CadbBuf1Reg.Data);
        }
      } else {
        RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "FastCadb is true , start CteProgramCadb and program the CADB_BUF_0_N0_MCCADB_MAIN_REG \n");
        CteProgramCadb (SubchMask, CadbLine, CadbBuf0Reg.Data, 0);
      }
    } // cadbLine loop
  }
  //
  // Delay if this is a CTE build, otherwise do nothing.
  //
  CteDelayDclk (50);
  return;
} // ProgramCADB2

/**
  Override CPGC setting to match aggressive pattern during DCA DUTY CYCLE training

  @param[in]  Host    Pointer to syshost structure
  @param[in]  Socket  Current socket under test (0-based)
  @param[in]  Ch      Current channel under test (0-based)
  @param[in]  Prog0   CADB array with setting to program the buffers

**/
VOID
OverrideRcdDcaDutyCycle (
  IN PSYSHOST       Host,
  IN UINT8          Socket,
  IN UINT8          Ch,
  IN PatCadb2Prog0  *Prog0
  )
{
  UINT8        CadbLine;
  UINT8        SubCh = 0;
  UINT8        MaxSubCh = 0;
  CADB_AO_MRSCFG_MCCADB_MAIN_STRUCT         CadbAoMrsCfg;
  CADB_BUF_0_N0_MCCADB_MAIN_STRUCT_LOCAL    CadbBuf0Reg;
  CADB_BUF_0_N1_MCCADB_MAIN_STRUCT_LOCAL    CadbBuf1Reg;

  MaxSubCh = CpgcGetValidSubCh ();
  //
  // Program 16 lines in the CADB
  //
  for (CadbLine = 0; CadbLine < CADB2_LINES; CadbLine++) {

    CadbBuf1Reg.Data = OverrideCadbDcaDutyCycle32_64[CadbLine];
    CadbBuf0Reg.Data = OverrideCadbDcaDutyCycle[CadbLine];
    CadbBuf0Reg.Ddr5Desel.Cs = Prog0[CadbLine].cs;

    for (SubCh = 0; SubCh < MaxSubCh; SubCh++) {

      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CADB_AO_MRSCFG_MCCADB_MAIN_REG, &CadbAoMrsCfg.Data);
      CadbAoMrsCfg.Bits.mrs_ao_repeats = 0; //Infinite Pattern
      GetSetCadbRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CADB_AO_MRSCFG_MCCADB_MAIN_REG, &CadbAoMrsCfg.Data);

      SetCadbBuf (Host, Socket, Ch, SubCh, CadbLine, CadbBuf0Reg.Data, CadbBuf1Reg.Data);
    }
  } // cadbLine loop
  //
  // Delay if this is a CTE build, otherwise do nothing.
  //
  CteDelayDclk (50);
  return;
} // OverrideRcdDcaDutyCycle



/**

  Set minimum good samples for receive enable feedback

  @param Host         - Pointer to sysHost, the system Host (root) structure

  @retval UINT8       - The number of minimum good samples

**/
UINT8
SetMinGoodInit (
  PSYSHOST Host
  )
{
  UINT32 LoopCount;

  GetRcvEnLoopCount (&LoopCount);

  if (LoopCount == 1) {
    return 1;
  } else {
    return (UINT8) (LoopCount/2);
  }
} // SetMinGoodInit

/**
  Executes an based memory test for advance trainings

  @param Host:         Pointer to sysHost
  @param Socket:       Processor socket to check
  @param DdrChEnMap    Bit-mask of enabled channels to test. 1 = Enabled, 0 = Skipped
  @param Dimm:         Target DIMM to test
  @param Rank:         Target Rank to test
  @param Group:        Parameter to be margined
                         RxDqsDelay    : Margin Rx DqDqs
                         TxDqDelay     : Margin Tx DqDqs
                         RxVref         : Margin Rx Vref
                         TxVref         : Margin Tx Vref
                         RecEnDelay    : Margin Receive Enable
                         WrLvlDelay    : Margin Write Level
                         CmdGrp0        : Margin CMD group 0
                         CmdGrp1        : Margin CMD group 1
                         CmdGrp2        : Margin CMD group 2
                         CmdAll         : Margin all CMD groups
  @param Mode:         Margin mode to use
                         MODE_XTALK            BIT0: Enable crosstalk (placeholder)
                         MODE_VIC_AGG          BIT1: Enable victim/aggressor
                         MODE_START_ZERO       BIT2: Start at margin 0
                         MODE_POS_ONLY         BIT3: Margin only the positive side
                         MODE_NEG_ONLY         BIT4: Margin only the negative side
                         MODE_DATA_MPR         BIT5: Enable MPR Data pattern
                         MODE_DATA_FIXED       BIT6: Enable Fixed Data pattern
                         MODE_DATA_LFSR        BIT7: Enable Data LFSR
                         MODE_ADDR_LFSR        BIT8: Enable Address LFSR
                         MODE_ADDR_CMD0_SETUP  BIT9
                         MODE_CHECK_PARITY     BIT10: Enable parity checking
                         MODE_DESELECT         BIT11: Enable deselect patterns
                         MODE_VA_DESELECT      BIT12: Enable Victim - Aggressor deselect patterns

  @param PatternLength:  Number of sequences to test (units of burstLength cache lines)
  @param BurstLength:    Number of cachelines to test for each write/read operation
                         It is the repeat count for the WR/RD subsequence

  @retval FAILURE: CPGC testing was unsuccessful.
  @retval SUCCESS: CPGC testing was successful or channel was not enabled.

**/
UINT32
ExecuteTest (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT32    DdrChEnMap,
  UINT8     Dimm,
  UINT8     Rank,
  MRC_GT    Group,              // Don't care since CPGC runs only dq pattern
  UINT16    Mode,
  UINT32    PatternLength,
  UINT8     BurstLength
  )
{
  UINT8 LogRank = 0;
  UINT8 LogSubRank = 0;
  struct channelNvram (*ChannelNvList) [MAX_CH];
  UINT8  Ch;
  UINT32 Status = SUCCESS;
  UINT8  MaxChDdr;
  UINT32 Pattern;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  if (DdrChEnMap != 0) {
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((DdrChEnMap & (1 << Ch)) == 0) {
        continue;
      }
      // Convert physical DIMM/physical rank into VMSE logical rank
      LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);
      if ((*ChannelNvList) [Ch].encodedCSMode) {
        LogSubRank = (Rank >> 1);
      }
      if (CheckCMDGroup (Group) || CheckCTLGroup (Group)) {
        Pattern = DDR_CPGC_ADV_CMD_PATTERN;
        CpgcAdvTrainingSetup (Host, Socket, (1 << Ch), Dimm, LogRank, 0, Pattern, PatternLengthToCpgcLoopCnt (PatternLength));
        RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
          "CMD/CTL patternlength = %d\n",PatternLengthToCpgcLoopCnt (PatternLength));
      } else {
        Pattern = DDR_CPGC_DQ_PATTERN;
        RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
          "RMT patternlength = %d\n",PatternLengthToCpgcLoopCnt (PatternLength));
        CpgcAdvTrainingSetup (Host, Socket, (1 << Ch), Dimm, LogRank, LogSubRank, Pattern, PatternLengthToCpgcLoopCnt (PatternLength));
      } // if check CMD CTL grp
    } //ch
    //
    // Set up the CADB if necessary
    //
    if (Mode & (MODE_DESELECT | MODE_VA_DESELECT)) {
      EnableCADBDeselect (Host, Socket, DdrChEnMap, Dimm, Mode);
    }
    // Run CPGC training test for the selected channel(s)
    Status = CpgcAdvTrainingExecTestGroup (Host, Socket, DdrChEnMap, Pattern);
  }

  return Status;
} // ExecuteTest

/**
  Set up the memory training or memory test.

  @param[in] Host:           Pointer to sysHost
  @param[in] Socket:         Processor socket within the system (0-based)
  @param[in] MemSs:          MemSS number within the socket (0-based)
  @param[in] ChannelMask:    Channel mask within the MemSS
  @param[in] RankListSize:   Number of items in RankList array
  @param[in] *RankList:      List of ranks, subranks to operate on
  @param[in] Level:          Provides context for LRDIMM buffer back-side vs. Host-side test
  @param[in] TestType:       Indicates the test type to be configured
  @param[in] TargetSignal:   Required for some early command training pattern test types
  @param[in] SeqCnt:         Optional to override default loopcount for given test type
                        This setting impacts the number of times the sequence repeats.
                        This is a linear value. This does not impact number of cachelines
                        per subsequence.  Value of 0 means "infinite" setting.
  @param[in] Patternloop:    Pattern loop

  @retval MRC_STATUS

**/
MRC_STATUS
SetupTest (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN MRC_MST     MemSs,
  IN UINT32      ChannelMask,
  IN UINT32      RankListSize,
  IN MRC_RT      *RankList,
  IN MRC_LT      Level,
  IN MRC_TT      TestType,
  IN MRC_CSN     TargetSignal,
  IN UINT32      SeqCnt,
  IN UINT8       Patternloop
  )
{
  UINT8                    Dimm = 0;
  UINT8                    Rank = 0;
  UINT32                   Status = SUCCESS;
#ifdef DDR5_SUPPORT
  CPGC_TEST_PARAM          *CpgcTestParamPtr;
  CPGC_TEST_ENV_PARAM      *CpgcTestEnvPtr;
  UINT32                   BaseColRepeatsValue = 0;
#endif

  if (ChannelMask == 0) {
    return MRC_STATUS_CH_NOT_PRESENT;
  }

  if (RankList != NULL) {
    RC_FATAL_ERROR ((RankListSize == 1), ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_209);
    Dimm = (RankList + RankListSize - 1)->Dimm;
    Rank = (RankList + RankListSize - 1)->Rank;
  }

  switch (TestType) {
#ifdef DDR5_SUPPORT
    case EarlyCsLoopbackTest:
      SetCadbModeEn (Host, Socket, ChannelMask, TRUE);
      Status = CpgcCsClkPattern (Host, Socket, ChannelMask, TargetSignal);
      return Status;
    case EarlyCsBacksideLoopbackTest:
      Status = CpgcQcsClkPattern (Host, Socket, ChannelMask, Dimm, Rank);
      return Status;
    case EarlyCaLoopbackTest:
      if (Host->var.mem.socket[Socket].TrainingStatus.Bits.TrainingPatternMode == CA_TRAINING_PATTERN_XTALK) {
        SetCadbModeEn (Host, Socket, ChannelMask, TRUE);
        Status = CpgcRcdDcaDutyCyclePattern (Host, Socket, ChannelMask, GetPhyRank (Dimm, Rank), 0, TargetSignal, NormalCadbSetup);
      } else {
        SetCadbModeEn (Host, Socket, ChannelMask, TRUE);
        Status = CpgcCaClkPattern (Host, Socket, ChannelMask, GetPhyRank (Dimm, Rank), 0, TargetSignal, NormalCadbSetup, Patternloop);
      }
      return Status;
    case DcaDckDutyCycleTest:
      SetCadbModeEn (Host, Socket, ChannelMask, TRUE);
      Status = CpgcRcdDcaDutyCyclePattern (Host, Socket, ChannelMask, GetPhyRank (Dimm, Rank), 0, TargetSignal, NormalCadbSetup);
      return Status;
    case DcaDfeDdr5Test:
      SetCadbModeEn (Host, Socket, ChannelMask, TRUE);
      Status = CpgcRcdDcaDfePattern (Host, Socket, ChannelMask, GetPhyRank (Dimm, Rank), 0, TargetSignal, NormalCadbSetup);
      return Status;
    case EarlyCaBacksideLoopbackTest:
      SetCadbModeEn (Host, Socket, ChannelMask, TRUE);
      Status = CpgcQcaClkPattern (Host, Socket, ChannelMask, GetPhyRank (Dimm, Rank), 0, TargetSignal, NormalCadbSetup);
      return Status;
    case BcomTrainingTest:
      Status = CpgcBcomClkPattern (Host, Socket, ChannelMask, GetPhyRank (Dimm, Rank), TargetSignal);
      return Status;
    case WLDdr5:
    case RcvenFineTest:
    case MdqsReceiveEnablePhaseTraining:
    case MdqsCoarseReadDelayTraining:
    case BacksideDqSwizzleDiscovery:
    case FrontsideDqSwizzleDiscovery:
    case MdqsReadDelayDeskew:
    case MdqsWriteDelay:
    case DqSwzDiscTestDdr5:
    case RdDqTest:
    case ReadPreDfe2DCenteringTest:
    case DramDutyCycleAdjusterTest:
    case ReadDfeTest:
    case ReadPostDfe2DCenteringTest:
    case WrDqTest:
    case DqSlewRateTest:
    case WritePreDfe2DCenteringTest:
    case WriteDfeTest:
    case WritePostDfe2DCenteringTest:
    case WriteDbDfeTest:
    case RmtDqTest:
    case RmtCmdTest:
    case TurnAroundSgTest:
    case TurnAroundSrTest:
    case TurnAroundDrDdRwTest:
    case TurnAroundDrDdWwRrWrTest:

      if ((GetCpgcTestParamPtrs (TestType, &CpgcTestParamPtr) == MRC_STATUS_FAILURE) ||
          (GetCpgcTestEnvPtrs (TestType, &CpgcTestEnvPtr) == MRC_STATUS_FAILURE)) {
        return MRC_STATUS_FAILURE;
      }
      if (TestType == WriteDbDfeTest) {
        BaseColRepeatsValue = DB_DFE_BASE_COL_REPEATS;
      } else {
        BaseColRepeatsValue = DEFAULT_BASE_COL_REPEATS;
      }
      Status = CpgcTestWorker (Socket, ChannelMask, GetLogicalRank (Host, Socket, 0, Dimm, Rank), SeqCnt, CpgcTestParamPtr, CpgcTestEnvPtr, BaseColRepeatsValue);
#endif // #ifdef DDR5_SUPPORT
    default:
      break;
  }

  return Status;
} // setuptest

/**
Modify memory training or memory test set up by the setuptest function.
  @param Host:           Pointer to sysHost
  @param Socket:         Processor socket within the system (0-based)
  @param MemSs           MemSS number within the socket (0-based)
  @param ChannelMask:    Channel mask within the MemSS
  @param RankListSize:   Number of items in RankList array
  @param *RankList:      List of ranks, subranks to operate on
  @param TestType:       Indicates the test type to be configured

@retval MRC_STATUS

**/
MRC_STATUS
ModifyTest (
  IN PSYSHOST      Host,
  IN UINT8         Socket,
  IN UINT32        MemSs,
  IN UINT32        ChannelMask,
  IN UINT32        RankListSize,
  IN MRC_RT        *RankList,
  IN MRC_TT        TestType
  )
{
  UINT8    Dimm;
  UINT8    Rank;
  UINT8    LogRank = 0;
  UINT8    LogSubRank = 0;
  UINT16   Mode = 0;
  struct   channelNvram (*ChannelNvList) [MAX_CH];
  UINT8    Ch;
  UINT32   Status = SUCCESS;
  UINT8    MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Dimm = (RankList + RankListSize - 1)->Dimm;
  Rank = (RankList + RankListSize - 1)->Rank;
  ChannelNvList = GetChannelNvList (Host, Socket);
  if (TestType == LateCmdTest) {
    Mode = MODE_VA_DESELECT;
  }
  if (ChannelMask != 0) {
    // Setup CPGC training pattern for specific group for all channels (parallel)
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((ChannelMask & (1 << Ch)) == 0) {
        continue;
      }
      // Convert physical DIMM/physical rank into logical rank
      LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);
      if ((*ChannelNvList) [Ch].encodedCSMode) {
        LogSubRank = (Rank >> 1);
      }
      if (TestType == LateCmdTest) {
        CpgcAdvTrainingModifyRank (Host, Socket, (1 << Ch), Dimm, LogRank, 0, DDR_CPGC_ADV_CMD_PATTERN);
      } else {
        CpgcAdvTrainingModifyRank (Host, Socket, (1 << Ch), Dimm, LogRank, LogSubRank, DDR_CPGC_DQ_PATTERN);
      }
    } //Ch
  }
  return Status;
}

/**

  Disable the DDRT Read Pending Queue (RPQ) credit limit for all channels with a DDRT DIMM.

  @param[in] Host             - Pointer to Host structure
  @param[in] Socket           - Processor socket within the system (0-based)

  @retval N/A

**/
VOID
DisableCpgcRpqCreditLimit (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  // Not implemented
}

/**

  This function sets CPGC register used to enable/disable refresh per rank

  @param Host:           Pointer to sysHost
  @param Socket:         Processor socket within the system (0-based)
  @param RefMaskValue:   Bit-field per physical chip-select rank. 0 = refresh commands masked; 0xFF = refresh commands not masked

  @retval n/a

**/
VOID
SetRefresh (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     refMaskValue
  )
{
  UINT8                               ch;
  CPGC_MISCREFCTL_MCDDC_CTL_STRUCT    cpgcMiscRefCtlReg;
  CPGC_MISCCKECTL_MCDDC_CTL_STRUCT       miscCKECtl;
  UINT8                               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  for (ch = 0; ch < MaxChDdr; ch++) {
    cpgcMiscRefCtlReg.Data = MemReadPciCfgEp (socket, ch, CPGC_MISCREFCTL_MCDDC_CTL_REG);
    cpgcMiscRefCtlReg.Bits.ref_rankmask = refMaskValue;
    MemWritePciCfgEp (socket, ch, CPGC_MISCREFCTL_MCDDC_CTL_REG, cpgcMiscRefCtlReg.Data);

    miscCKECtl.Data = MemReadPciCfgEp (socket, ch, CPGC_MISCCKECTL_MCDDC_CTL_REG);
    miscCKECtl.Bits.refresh_enable = !refMaskValue;
    MemWritePciCfgEp (socket, ch, CPGC_MISCCKECTL_MCDDC_CTL_REG, miscCKECtl.Data);
  } // ch loop

  if (refMaskValue == 0) {
    host->var.mem.socket[socket].hostRefreshStatus = 1;
  } else {
    host->var.mem.socket[socket].hostRefreshStatus = 0;
  }

} // SetRefresh

/**

  Collect DDR5 DQ lanes CPGC error status when CPGC raster repo inactive

  cpgcErrDat0S                   - SubCh-A data lane [31:0] even UI error status.
  cpgcErrDat1S                   - SubCh-B data lane [31:0] even UI error status.
  cpgcErrDat2S                   - SubCh-A data lane [31:0] odd UI error status.
  cpgcErrDat3S                   - SubCh-B data lane [31:0] odd UI error status.
  cpgcErrEccS                    - [7:0]    - SubCh-A ecc lane [7:0] even UI error status.
                                 - [15:8]   - SubCh-A ecc lane [7:0] odd UI error status.
                                 - [23:16]  - SubCh-B ecc lane [7:0] even UI error status.
                                 - [31:24]  - SubCh-B ecc lane [7:0] odd UI error status.

  @param[in]  Host                - Pointer to sysHost
  @param[in]  Socket              - Socket to train
  @param[in]  Ch                  - Channel Id
  @param[in]  SubCh               - Sub-channel Id
  @param[out] CpgcErrorStatus     - Error status for each channel

  @retval n/a

**/
VOID
CpgcLanesErrorStatusRepoInactive (
  IN     PSYSHOST          Host,
  IN     UINT8             Socket,
  IN     UINT8             Ch,
  IN     UINT8             SubCh,
     OUT CPGC_ERROR_STATUS CpgcErrorStatus[MAX_CH]
  )
{
   if (SubCh == SUB_CH_A) {
    //
    // SubCh-A data lane [31:0] even UI error status.
    //
    CpgcErrorStatus[Ch].cpgcErrDat0S = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_A, CPGC2_RASTER_REPO_CONTENT_ECC_0_N0_MCCPGC_MAIN_REG));
    //
    // SubCh-A data lane [31:0] odd UI error status.
    //
    CpgcErrorStatus[Ch].cpgcErrDat2S = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_A, CPGC2_RASTER_REPO_CONTENT_ECC_1_N0_MCCPGC_MAIN_REG));
    //
    // Clear SubCh-A Ecc lane [7:0] even and odd error status
    //
    CpgcErrorStatus[Ch].cpgcErrEccS &= (WORD_MASK << WORD_WIDTH);

    if (Host->nvram.mem.eccEn) {
      //
      // SubCh-A Ecc lane [7:0] even UI error status
      //
      CpgcErrorStatus[Ch].cpgcErrEccS |= (BYTE_MASK & MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_A, CPGC2_ERR_SUMMARY_A_MCCPGC_MAIN_REG)));
      //
      // SubCh-A Ecc lane [7:0] odd UI error status
      //
      CpgcErrorStatus[Ch].cpgcErrEccS |= (BYTE_MASK & MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_A, CPGC2_ERR_SUMMARY_B_MCCPGC_MAIN_REG))) << BYTE_WIDTH;
    }
  }

  if (SubCh == SUB_CH_B) {
    //
    // SubCh-B data lane [31:0] even UI error status.
    //
    CpgcErrorStatus[Ch].cpgcErrDat1S = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_B, CPGC2_RASTER_REPO_CONTENT_ECC_0_N0_MCCPGC_MAIN_REG));
    //
    // SubCh-B data lane [31:0] odd UI error status.
    //
    CpgcErrorStatus[Ch].cpgcErrDat3S = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_B, CPGC2_RASTER_REPO_CONTENT_ECC_1_N0_MCCPGC_MAIN_REG));

    //
    // Clear SubCh-B Ecc lane [7:0] even and odd error status
    //
    CpgcErrorStatus[Ch].cpgcErrEccS &= WORD_MASK;

    if (Host->nvram.mem.eccEn) {
      //
      // SubCh-B Ecc lane [7:0] even UI error status
      //
      CpgcErrorStatus[Ch].cpgcErrEccS |= (BYTE_MASK & MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_B, CPGC2_ERR_SUMMARY_A_MCCPGC_MAIN_REG))) << WORD_WIDTH;
      //
      // SubCh-B Ecc lane [7:0] odd UI error status
      //
      CpgcErrorStatus[Ch].cpgcErrEccS |= (BYTE_MASK & MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_B, CPGC2_ERR_SUMMARY_B_MCCPGC_MAIN_REG))) << (WORD_WIDTH + BYTE_WIDTH);

    }
  }

  return;

} // CpgcLanesErrorStatusRepoInactive

/**

  Collect DDR5 DQ lanes CPGC error status when CPGC raster repo active

  cpgcErrDat0S                   - SubCh-A data lane [31:0] UI error status.
  cpgcErrDat1S                   - SubCh-B data lane [31:0] UI error status.
  cpgcErrDat2S                   - Same to cpgcErrDat0S
  cpgcErrDat3S                   - Same to cpgcErrDat1S
  cpgcErrEccS                    - [7:0]    - SubCh-A ecc lane [7:0] UI error status.
                                 - [15:8]   - Same to [7:0]
                                 - [23:16]  - SubCh-B ecc lane [7:0] UI error status.
                                 - [31:24]  - Same to [23:16]

  @param[in]  Host                - Pointer to sysHost
  @param[in]  Socket              - Socket to train
  @param[in]  Ch                  - Channel Id
  @param[in]  SubCh               - Sub-channel Id
  @param[out] CpgcErrorStatus     - Error status for each channel

  @retval n/a

**/
VOID
CpgcLanesErrorStatusRepoActive (
  IN     PSYSHOST          Host,
  IN     UINT8             Socket,
  IN     UINT8             Ch,
  IN     UINT8             SubCh,
     OUT CPGC_ERROR_STATUS CpgcErrorStatus[MAX_CH]
  )
{
  CPGC_ERR_ECC_CHNK_RANK_STAT_MCCPGC_MAIN_STRUCT  ErrEccChnkRankStat;

  if (SubCh == SUB_CH_A) {
    //
    // SubCh-A data lane [31:0] UI error status.
    //
    CpgcErrorStatus[Ch].cpgcErrDat0S = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_A, CPGC_ERR_STAT03_MCCPGC_MAIN_REG));
    CpgcErrorStatus[Ch].cpgcErrDat2S = CpgcErrorStatus[Ch].cpgcErrDat0S;

    //
    // Clear SubCh-A Ecc lane [7:0] even and odd error status
    //
    CpgcErrorStatus[Ch].cpgcErrEccS &= (WORD_MASK << WORD_WIDTH);

    if (Host->nvram.mem.eccEn) {
      //
      // SubCh-A ecc lane [7:0] UI error status.
      //
      ErrEccChnkRankStat.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_A, CPGC_ERR_ECC_CHNK_RANK_STAT_MCCPGC_MAIN_REG));
      CpgcErrorStatus[Ch].cpgcErrEccS |= ErrEccChnkRankStat.Bits.ecc_lane_err_stat;
      CpgcErrorStatus[Ch].cpgcErrEccS |= ErrEccChnkRankStat.Bits.ecc_lane_err_stat << BYTE_WIDTH;
    }
  }

  if (SubCh == SUB_CH_B) {
    //
    // SubCh-B data lane [31:0] UI error status.
    //
    CpgcErrorStatus[Ch].cpgcErrDat1S = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_B, CPGC_ERR_STAT03_MCCPGC_MAIN_REG));
    CpgcErrorStatus[Ch].cpgcErrDat3S = CpgcErrorStatus[Ch].cpgcErrDat1S;
    //
    // Clear SubCh-B Ecc lane [7:0] even and odd error status
    //
    CpgcErrorStatus[Ch].cpgcErrEccS &= WORD_MASK;

    if (Host->nvram.mem.eccEn) {
      //
      // SubCh-B ecc lane [7:0] UI error status.
      //
      ErrEccChnkRankStat.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_B, CPGC_ERR_ECC_CHNK_RANK_STAT_MCCPGC_MAIN_REG));
      CpgcErrorStatus[Ch].cpgcErrEccS |= ErrEccChnkRankStat.Bits.ecc_lane_err_stat << WORD_WIDTH;
      CpgcErrorStatus[Ch].cpgcErrEccS |= ErrEccChnkRankStat.Bits.ecc_lane_err_stat << (WORD_WIDTH + BYTE_WIDTH);

    }
  }

  return;

} // CpgcLanesErrorStatusRepoActive

/**

  Collect DDR advanced command CPGC error status

  cpgcErrDat0S                   - Repo Inactive - SubCh-A data lane [31:0] even UI error status.
                                 - Repo Active   - SubCh-A data lane [31:0] UI error status.
  cpgcErrDat1S                   - Repo Inactive - SubCh-B data lane [31:0] even UI error status.
                                 - Repo Active   - SubCh-B data lane [31:0] UI error status.
  cpgcErrDat2S                   - Repo Inactive - SubCh-A data lane [31:0] odd UI error status.
                                 - Repo Active   - Same to cpgcErrDat0S
  cpgcErrDat3S                   - Repo Inactive - SubCh-B data lane [31:0] odd UI error status.
                                 - Repo Active   - Same to cpgcErrDat1S
  cpgcErrEccS                    - Repo Inactive
                                 - [7:0]    - SubCh-A ecc lane [7:0] even UI error status.
                                 - [15:8]   - SubCh-A ecc lane [7:0] odd UI error status.
                                 - [23:16]  - SubCh-B ecc lane [7:0] even UI error status.
                                 - [31:24]  - SubCh-B ecc lane [7:0] odd UI error status.
                                 - Repo Active
                                 - [7:0]    - SubCh-A ecc lane [7:0] UI error status.
                                 - [15:8]   - Same to [7:0]
                                 - [23:16]  - SubCh-B ecc lane [7:0] UI error status.
                                 - [31:24]  - Same to [23:16]
cpgcErrNibS                      - [7:0]    - SubCh-A data lane [31:0] even UI nibble error status.
                                 - [15:8]   - SubCh-A data lane [31:0] odd UI nibble error status.
                                 - [23:16]  - SubCh-B data lane [31:0] even UI nibble error status.
                                 - [31:24]  - SubCh-B data lane [31:0] odd UI nibble error status.
cpgcErrMiscS                     - [3:0]    - SubCh-A data lane [31:0] byte group error status.
                                 - [4]      - SubCh-A ecc lane [7:0] byte group error status.
                                 - [6:5]    - SubCh-A ecc lane [7:0] even UI nibble error status.
                                 - [19:16]  - SubCh-B data lane [31:0] byte group error status.
                                 - [20]     - SubCh-B ecc lane [7:0] byte group error status.
                                 - [22:21]  - SubCh-B ecc lane [7:0] odd UI nibble error status.

  @param[in]  Host                - Pointer to sysHost
  @param[in]  Socket              - Socket to train
  @param[in]  DdrChEnMap          - 8-bit bit mask of ddr channels to test - (1 = active channel)
  @param[out] CpgcErrorStatus     - Error status for each channel

  @retval SUCCESS / FAILURE

**/
UINT32
CpgcAdvTrainingErrorStatus (
  IN     PSYSHOST          Host,
  IN     UINT8             Socket,
  IN     UINT32            DdrChEnMap,
     OUT CPGC_ERROR_STATUS CpgcErrorStatus[MAX_CH]
  )
{
  UINT8                                        Ch;
  UINT8                                        MaxChDdr;
  UINT32                                       Data32;
  CPGC2_RASTER_REPO_CONFIG_MCCPGC_MAIN_STRUCT  RasterRepoConfig;
  UINT32                                       Status = SUCCESS;

  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    //
    // skip inactive channels
    //
    if (!(DdrChEnMap & (1 << Ch))) {
      continue;
    }

    //
    // Data and ecc lanes error status information depends on remo mode
    //
    RasterRepoConfig.Data = (UINT16) (WORD_MASK & MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_A, CPGC2_RASTER_REPO_CONFIG_MCCPGC_MAIN_REG)));

    if (RasterRepoConfig.Bits.rasterrepomode == CPGC_RASTER_REPO_INACTIVE_MODE) {
      CpgcLanesErrorStatusRepoInactive (Host, Socket, Ch, SUB_CH_A, CpgcErrorStatus);
    } else {
      CpgcLanesErrorStatusRepoActive (Host, Socket, Ch, SUB_CH_A, CpgcErrorStatus);
    }

    //
    // SubCh-A Even UI nibble error status.
    //
    Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_A, CPGC2_ERR_SUMMARY_A_MCCPGC_MAIN_REG));
    CpgcErrorStatus[Ch].cpgcErrNibS.Bits.SubChADataEven    = CPGC_ERR_STAT_NIBBLE_DATA (Data32);
    CpgcErrorStatus[Ch].cpgcErrMiscS.Bits.SubChANibEccEven = CPGC_ERR_STAT_NIBBLE_ECC (Data32);
    //
    // SubCh-A Odd UI nibble error status.
    //
    Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_A, CPGC2_ERR_SUMMARY_B_MCCPGC_MAIN_REG));
    CpgcErrorStatus[Ch].cpgcErrNibS.Bits.SubChADataOdd    = CPGC_ERR_STAT_NIBBLE_DATA (Data32);
    CpgcErrorStatus[Ch].cpgcErrMiscS.Bits.SubChANibEccOdd = CPGC_ERR_STAT_NIBBLE_ECC (Data32);
    //
    // SubCh-A byte group error status.
    //
    Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_A, CPGC_ERR_BYTE_NTH_PAR_STAT_MCCPGC_MAIN_REG));
    CpgcErrorStatus[Ch].cpgcErrMiscS.Bits.SubChABtyGrpDatS  =  CPGC_ERR_STAT_BYTE_GROUP_DATA (Data32);
    CpgcErrorStatus[Ch].cpgcErrMiscS.Bits.SubChABtyGrpEccS  =  CPGC_ERR_STAT_BYTE_GROUP_ECC (Data32);

    //
    // Data and ecc lanes error status information depends on remo mode
    //
    RasterRepoConfig.Data = (UINT16) (WORD_MASK & MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_B, CPGC2_RASTER_REPO_CONFIG_MCCPGC_MAIN_REG)));

    if (RasterRepoConfig.Bits.rasterrepomode == CPGC_RASTER_REPO_INACTIVE_MODE) {
      CpgcLanesErrorStatusRepoInactive (Host, Socket, Ch, SUB_CH_B, CpgcErrorStatus);
    } else {
      CpgcLanesErrorStatusRepoActive (Host, Socket, Ch, SUB_CH_B, CpgcErrorStatus);
    }

    //
    // SubCh-B Even UI nibble error status.
    //
    Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_B, CPGC2_ERR_SUMMARY_A_MCCPGC_MAIN_REG));
    CpgcErrorStatus[Ch].cpgcErrNibS.Bits.SubChBDataEven    = CPGC_ERR_STAT_NIBBLE_DATA (Data32);
    CpgcErrorStatus[Ch].cpgcErrMiscS.Bits.SubChBNibEccEven = CPGC_ERR_STAT_NIBBLE_ECC (Data32);
    //
    // SubCh-B Odd UI nibble error status.
    //
    Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_B, CPGC2_ERR_SUMMARY_B_MCCPGC_MAIN_REG));
    CpgcErrorStatus[Ch].cpgcErrNibS.Bits.SubChBDataOdd    = CPGC_ERR_STAT_NIBBLE_DATA (Data32);
    CpgcErrorStatus[Ch].cpgcErrMiscS.Bits.SubChBNibEccOdd = CPGC_ERR_STAT_NIBBLE_ECC (Data32);
    //
    // SubCh-B byte group error status.
    //
    Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SUB_CH_B, CPGC_ERR_BYTE_NTH_PAR_STAT_MCCPGC_MAIN_REG));
    CpgcErrorStatus[Ch].cpgcErrMiscS.Bits.SubChBBtyGrpDatS  =  CPGC_ERR_STAT_BYTE_GROUP_DATA (Data32);
    CpgcErrorStatus[Ch].cpgcErrMiscS.Bits.SubChBBtyGrpEccS  =  CPGC_ERR_STAT_BYTE_GROUP_ECC (Data32);

    if (Host->nvram.mem.eccEn == 0) {
      //
      // Clean ECC error status fields
      //
      CpgcErrorStatus[Ch].cpgcErrMiscS.Bits.SubChANibEccEven = 0;
      CpgcErrorStatus[Ch].cpgcErrMiscS.Bits.SubChANibEccOdd  = 0;
      CpgcErrorStatus[Ch].cpgcErrMiscS.Bits.SubChABtyGrpEccS = 0;
      CpgcErrorStatus[Ch].cpgcErrMiscS.Bits.SubChBNibEccEven = 0;
      CpgcErrorStatus[Ch].cpgcErrMiscS.Bits.SubChBNibEccOdd  = 0;
      CpgcErrorStatus[Ch].cpgcErrMiscS.Bits.SubChBBtyGrpEccS = 0;
    }
  } // Ch

  return Status;

} // CpgcAdvTrainingErrorStatus

/**

  Specify CPGC error counter and error type for error counting

  @param[in] Host                - Pointer to sysHost
  @param[in] Socket              - Socket number
  @param[in] Ch                  - Channel number
  @param[in] SubCh               - Subchannel number
  @param[in] CounterIndex        - Error counter index: 0~8
  @param[in] ErrorType           - Error type
  @param[in] CounterPointer      - Pointer used to indicate which lane, bytegroup, nibble or UI the counter will count errors on

  @retval SUCCESS                - Setup error counter successfully
          FAILURE                - Failed to Setup error counter

**/
UINT32
CpgcErrorCounterSelect (
  IN PSYSHOST               Host,
  IN UINT8                  Socket,
  IN UINT8                  Ch,
  IN UINT8                  SubCh,
  IN UINT8                  CounterIndex,
  IN CPGC_ERROR_COUNT_TYPE  ErrorType,
  IN UINT8                  CounterPointer
)
{
  UINT8 CounterControlScope = 0;
  UINT8 CounterControlUi    = 0;
  UINT8 CounterControlSel   = 0;
  CPGC_ERR_CNTRCTL_0_MCCPGC_MAIN_STRUCT  CpgcErrCntrctl;

  if (CounterIndex >= CPGC_ERROR_COUNTER_MAX_NUMBER) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
                "CpgcErrorCounterSelect: Incorrect CPGC Error Counter Index %d\n", CounterIndex);
    return FAILURE;
  }

  switch (ErrorType) {
    case CpgcErrCntAllLanesOnAllUi:
      CounterControlScope = CPGC_ERROR_COUNTER_POINTER_UNUSED;
      CounterControlUi    = CPGC_ERROR_COUNT_UI_ON_ALL_UI;
      break;
    case CpgcErrCntAllLanesOnEvenUi:
      CounterControlScope = CPGC_ERROR_COUNTER_POINTER_UNUSED;
      CounterControlUi    = CPGC_ERROR_COUNT_UI_ON_EVEN_UI;
      break;
    case CpgcErrCntAllLanesOnOddUi:
      CounterControlScope = CPGC_ERROR_COUNTER_POINTER_UNUSED;
      CounterControlUi    = CPGC_ERROR_COUNT_UI_ON_ODD_UI;
      break;
    case CpgcErrCntUiOnAllLanes:
      CounterControlScope = CPGC_ERROR_COUNTER_POINTER_USED;
      CounterControlUi    = CPGC_ERROR_COUNT_UI_ON_PARTICULAR_UI;
      CounterControlSel   = CPGC_ERROR_COUNT_SEL_ON_PARTICULAR_UI;
      break;
    case CpgcErrCntLaneOnAllUi:
      CounterControlScope = CPGC_ERROR_COUNTER_POINTER_USED;
      CounterControlUi    = CPGC_ERROR_COUNT_UI_ON_ALL_UI;
      CounterControlSel   = CPGC_ERROR_COUNT_SEL_ON_LANE;
      break;
    case CpgcErrCntLaneOnEvenUi:
      CounterControlScope = CPGC_ERROR_COUNTER_POINTER_USED;
      CounterControlUi    = CPGC_ERROR_COUNT_UI_ON_EVEN_UI;
      CounterControlSel   = CPGC_ERROR_COUNT_SEL_ON_LANE;
      break;
    case CpgcErrCntLaneOnOddUi:
      CounterControlScope = CPGC_ERROR_COUNTER_POINTER_USED;
      CounterControlUi    = CPGC_ERROR_COUNT_UI_ON_ODD_UI;
      CounterControlSel   = CPGC_ERROR_COUNT_SEL_ON_LANE;
      break;
    case CpgcErrCntByteGroupOnAllUi:
      CounterControlScope = CPGC_ERROR_COUNTER_POINTER_USED;
      CounterControlUi    = CPGC_ERROR_COUNT_UI_ON_ALL_UI;
      CounterControlSel   = CPGC_ERROR_COUNT_SEL_ON_BYTEGROUP;
      break;
    case CpgcErrCntByteGroupOnEvenUi:
      CounterControlScope = CPGC_ERROR_COUNTER_POINTER_USED;
      CounterControlUi    = CPGC_ERROR_COUNT_UI_ON_EVEN_UI;
      CounterControlSel   = CPGC_ERROR_COUNT_SEL_ON_BYTEGROUP;
      break;
    case CpgcErrCntByteGroupOnOddUi:
      CounterControlScope = CPGC_ERROR_COUNTER_POINTER_USED;
      CounterControlUi    = CPGC_ERROR_COUNT_UI_ON_ODD_UI;
      CounterControlSel   = CPGC_ERROR_COUNT_SEL_ON_BYTEGROUP;
      break;
    case CpgcErrCntNibbleGroupOnAllUi:
      CounterControlScope = CPGC_ERROR_COUNTER_POINTER_USED;
      CounterControlUi    = CPGC_ERROR_COUNT_UI_ON_ALL_UI;
      CounterControlSel   = CPGC_ERROR_COUNT_SEL_ON_NIBBLEGROUP;
      break;
    case CpgcErrCntNibbleGroupOnEvenUi:
      CounterControlScope = CPGC_ERROR_COUNTER_POINTER_USED;
      CounterControlUi    = CPGC_ERROR_COUNT_UI_ON_EVEN_UI;
      CounterControlSel   = CPGC_ERROR_COUNT_SEL_ON_NIBBLEGROUP;
      break;
    case CpgcErrCntNibbleGroupOnOddUi:
      CounterControlScope = CPGC_ERROR_COUNTER_POINTER_USED;
      CounterControlUi    = CPGC_ERROR_COUNT_UI_ON_ODD_UI;
      CounterControlSel   = CPGC_ERROR_COUNT_SEL_ON_NIBBLEGROUP;
      break;
    default:
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
                "CpgcErrorCounterSelect: Incorrect CPGC Error Type %d\n", ErrorType);
      return FAILURE;
  }

  CpgcErrCntrctl.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_CNTRCTL_0_MCCPGC_MAIN_REG) + CounterIndex * 4 );
  CpgcErrCntrctl.Bits.counter_control_scope = CounterControlScope;
  CpgcErrCntrctl.Bits.counter_control_ui    = CounterControlUi;
  CpgcErrCntrctl.Bits.counter_control_sel   = CounterControlSel;
  CpgcErrCntrctl.Bits.counter_pointer       = CounterPointer;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC(SubCh, CPGC_ERR_CNTRCTL_0_MCCPGC_MAIN_REG) + CounterIndex * 4, CpgcErrCntrctl.Data);

  return SUCCESS;
}

/**

  Read specific CPGC error counter

  @param[in]  Host                - Pointer to sysHost
  @param[in]  Socket              - Socket number
  @param[in]  Ch                  - Channel number
  @param[in]  SubCh               - Subchannel number
  @param[in]  CounterIndex        - Error counter index: 0~8
  @param[out] *CounterNumber      - Pointer to counter value
  @param[out] *CounterOverFlow    - Pointer to indicate whether or not the counter is overflow

  @retval SUCCESS                 - Read error counter successfully
          FAILURE                 - Failed to Read error counter

**/
UINT32
CpgcReadErrorCounter (
  IN  PSYSHOST               Host,
  IN  UINT8                  Socket,
  IN  UINT8                  Ch,
  IN  UINT8                  SubCh,
  IN  UINT8                  CounterIndex,
  OUT UINT32                *CounterNumber,
  OUT BOOLEAN               *CounterOverFlow
)
{
  CPGC_ERR_CNTR_0_MCCPGC_MAIN_STRUCT  CpgcErrCntr;
  CPGC_ERR_CNTR_OV_MCCPGC_MAIN_STRUCT CpgcErrCntrOv;

  if (CounterIndex >= CPGC_ERROR_COUNTER_MAX_NUMBER) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
                "CpgcReadErrorCounter: Incorrect CPGC Error Counter Index %d, return.\n", CounterIndex);
    return FAILURE;
  }

  if ((CounterNumber == NULL) || (CounterOverFlow == NULL)) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
                "CpgcReadErrorCounter: NULL pointer detected, return.\n");
    return FAILURE;
  }

  CpgcErrCntr.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_CNTR_0_MCCPGC_MAIN_REG) + CounterIndex * 4 );
  *CounterNumber = CpgcErrCntr.Bits.error_counter;

  CpgcErrCntrOv.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_CNTR_OV_MCCPGC_MAIN_REG));
  *CounterOverFlow = ((CpgcErrCntrOv.Bits.error_counter_overflow >> CounterIndex) & BIT0) ? TRUE : FALSE;

  return SUCCESS;
}

/**
  Set the CPGC lock state.

  Refer to the enum CPGC_LOCK_STATE for available options. Note that this is not available on all steppings of all
  processors.

  @param[in]  Socket            The target socket
  @param[in]  CpgcLockState     The desired lock state

  @retval   EFI_STATUS  Whether or not setting the lock state was successful.
**/
EFI_STATUS
EFIAPI
SetCpgcLock (
  IN  UINT8           Socket,
  IN  CPGC_LOCK_STATE CpgcLockState
  )
{
  UINT8                           Channel;
  CHANNEL_NVRAM_STRUCT            (*ChannelNvList) [MAX_CH];
  PSYSHOST                        Host;
  LT_CPGC_CONTROL_MC_MAIN_STRUCT  LtCpgcControl;
  UINT8                           MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  // Loop for each channel
  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if ((*ChannelNvList) [Channel].enabled == 0) {
      continue;
    }

    LtCpgcControl.Data = MemReadPciCfgEp (Socket, Channel, LT_CPGC_CONTROL_MC_MAIN_REG);
    if (LtCpgcControl.Bits.res_lock != (UINT32) CpgcLockState) {
      LtCpgcControl.Bits.res_lock = (UINT32) CpgcLockState;
      MemWritePciCfgEp (Socket, Channel, LT_CPGC_CONTROL_MC_MAIN_REG, LtCpgcControl.Data);
    }
  } // Channel loop

  return EFI_SUCCESS;
}

/**
  Early config on Cpgc CMI Credit to enable CPGC for memory training

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number

  @retval EFI_UNSUPPORTED        If CPGC doesn't require early config of CMI credit
          EFI_SUCCESS            If CPGC requires early config of CMI credit
**/
EFI_STATUS
EFIAPI
EarlyConfigCpgcCmi (
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{
  CPGC2_CREDIT_MCCPGC_MAIN_STRUCT          Cpgc2Credit;
  CPGC2_V_CHICKEN_MCCPGC_MAIN_STRUCT       CpgcVChicken;
  CPGC2_STG_CHICKEN_N0_MCCPGC_MAIN_STRUCT  Cpgc2StgChickenN0;
  CPGC2_STG_CHICKEN_N1_MCCPGC_MAIN_STRUCT  Cpgc2StgChickenN1;
  PSYSHOST                                 Host;

  Host = GetSysHostPointer ();

  Cpgc2Credit.Data = MemReadPciCfgEp (Socket, Ch, CPGC2_CREDIT_MCCPGC_MAIN_REG);
  Cpgc2Credit.Bits.rd_cpl_credits_init = 20;
  Cpgc2Credit.Bits.rsp_credits_init = 0x4;
  MemWritePciCfgEp (Socket, Ch, CPGC2_CREDIT_MCCPGC_MAIN_REG, Cpgc2Credit.Data);

  CpgcVChicken.Data = MemReadPciCfgEp (Socket, Ch, CPGC2_V_CHICKEN_MCCPGC_MAIN_REG);
  CpgcVChicken.Bits.rtn_bypass_disable = 0x1;
  CpgcVChicken.Bits.xpq_ch_credit = 0x16;
  CpgcVChicken.Bits.xpq_ch_override = 0x0;
  CpgcVChicken.Bits.stg_ch_credit = 0xA;
  CpgcVChicken.Bits.stg_ch_override = 0x0;
  CpgcVChicken.Bits.axi_order_mode = 0x0;
  CpgcVChicken.Bits.mpr_test_req_dblr = 0x0;
  CpgcVChicken.Bits.internal_err = 0x0;
  MemWritePciCfgEp (Socket, Ch, CPGC2_V_CHICKEN_MCCPGC_MAIN_REG, CpgcVChicken.Data);

  Cpgc2StgChickenN0.Data = MemReadPciCfgEp (Socket, Ch, CPGC2_STG_CHICKEN_N0_MCCPGC_MAIN_REG);
  Cpgc2StgChickenN0.Bits.rtn_wtrmrk = 0x0;
  Cpgc2StgChickenN0.Bits.stg_wtrmrk = 0x0;
  MemWritePciCfgEp (Socket, Ch, CPGC2_STG_CHICKEN_N0_MCCPGC_MAIN_REG, Cpgc2StgChickenN0.Data);

  Cpgc2StgChickenN1.Data = MemReadPciCfgEp (Socket, Ch, CPGC2_STG_CHICKEN_N1_MCCPGC_MAIN_REG);
  Cpgc2StgChickenN1.Bits.rtn_preload = 0x0;
  Cpgc2StgChickenN1.Bits.stg_preload = 0x0;
  MemWritePciCfgEp (Socket, Ch, CPGC2_STG_CHICKEN_N1_MCCPGC_MAIN_REG, Cpgc2StgChickenN1.Data);

  return EFI_SUCCESS;
}

/**
  Polling whether or not CPGC credit ISM is in the ISM_IDLE state

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number

  @retval EFI_UNSUPPORTED        If CPGC doesn't support this
          EFI_TIMEOUT            Polling timeout
          EFI_SUCCESS            Otherwise
**/
EFI_STATUS
EFIAPI
PollCpgcIsmIdle (
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{
  EFI_STATUS                               Status = EFI_SUCCESS;
  PSYSHOST                                 Host;
  CPGC2_CREDIT_MCCPGC_MAIN_STRUCT          Cpgc2Credit;
  UINT32                                   InMcUlt = 0;
  BOOLEAN                                  BreakEarly = FALSE;
  UINT64                                   StartCount;

  Host = GetSysHostPointer ();

  Status = CteGetOptions ("mc_ult", &(InMcUlt));
  if ((Status != EFI_SUCCESS) && (Status != EFI_UNSUPPORTED)) {
    RcDebugPrint (SDBG_DEFAULT, "PollCpgcIsmIdle: Failed CteGetOptions for mc_ult\n");
  } else if ((Status == EFI_SUCCESS) && (InMcUlt == 1)) {
    BreakEarly = TRUE;
  }


  StartCount = GetCountTsc ();

  do {
    if (BreakEarly) {
      break;
    }

    Cpgc2Credit.Data = MemReadPciCfgEp (Socket, Ch, CPGC2_CREDIT_MCCPGC_MAIN_REG);

    if (GetDelayTsc (StartCount) > CATCHALL_TIMEOUT) {
      Status = EFI_TIMEOUT;
      RcDebugPrint (SDBG_DEFAULT, "PollCpgcIsmIdle: Timeout\n");
      break;
    }
    //
    // If this is a CTE build, delay between reads so we don't poll so often.
    //
    CteDelayQclk (50);
  } while (Cpgc2Credit.Bits.ism_idle != 1);

  return Status;
}

/**
  Polling whether or not CPGC credit negotiation process is complete.

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number

  @retval EFI_UNSUPPORTED        If CPGC doesn't support this
          EFI_TIMEOUT            Polling timeout
          EFI_SUCCESS            Otherwise
**/
EFI_STATUS
EFIAPI
PollCpgcCreditsInitialized (
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{
  EFI_STATUS                               Status = EFI_SUCCESS;
  PSYSHOST                                 Host;
  CPGC2_CREDIT_MCCPGC_MAIN_STRUCT          Cpgc2Credit;
  UINT32                                   InMcUlt = 0;
  BOOLEAN                                  BreakEarly = FALSE;
  UINT64                                   StartCount;

  Host = GetSysHostPointer ();

  Status = CteGetOptions ("mc_ult", &(InMcUlt));
  if ((Status != EFI_SUCCESS) && (Status != EFI_UNSUPPORTED)) {
    RcDebugPrint (SDBG_DEFAULT, "PollCpgcCreditsInitialized: Failed CteGetOptions for mc_ult\n");
  } else if ((Status == EFI_SUCCESS) && (InMcUlt == 1)) {
    BreakEarly = TRUE;
  }


  StartCount = GetCountTsc ();

  do {
    if (BreakEarly) {
      break;
    }

    Cpgc2Credit.Data = MemReadPciCfgEp (Socket, Ch, CPGC2_CREDIT_MCCPGC_MAIN_REG);

    if (GetDelayTsc (StartCount) > CATCHALL_TIMEOUT) {
      Status = EFI_TIMEOUT;
      RcDebugPrint (SDBG_DEFAULT, "PollCpgcCreditsInitialized: Timeout\n");
      break;
    }
    //
    // If this is a CTE build, delay between reads so we don't poll so often.
    //
    CteDelayQclk (50);
  } while (Cpgc2Credit.Bits.credits_initialized != 1);

  return Status;

}

/**
  Initialize CPGC Credit Config Done bit to force re-negotiation for credits

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number

  @retval EFI_UNSUPPORTED        If CPGC doesn't support this
          EFI_SUCCESS            Otherwise
**/
EFI_STATUS
EFIAPI
InitCpgcCreditConfigDone (
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{
  CPGC2_CREDIT_MCCPGC_MAIN_STRUCT          Cpgc2Credit;
  PSYSHOST                                 Host;

  Host = GetSysHostPointer ();

  Cpgc2Credit.Data = MemReadPciCfgEp (Socket, Ch, CPGC2_CREDIT_MCCPGC_MAIN_REG);
  Cpgc2Credit.Bits.credits_config_done = 0x0;
  MemWritePciCfgEp (Socket, Ch, CPGC2_CREDIT_MCCPGC_MAIN_REG, Cpgc2Credit.Data);

  return EFI_SUCCESS;
}

/**
  Set Credit Config Done bit once the desired number of credits have been placed in the CREDIT_CFG register

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number

  @retval EFI_UNSUPPORTED        If CPGC doesn't support this
          EFI_SUCCESS            Otherwise
**/
EFI_STATUS
EFIAPI
SetCpgcCreditConfigDone (
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{
  CPGC2_CREDIT_MCCPGC_MAIN_STRUCT          Cpgc2Credit;
  PSYSHOST                                 Host;

  Host = GetSysHostPointer ();

  Cpgc2Credit.Data = MemReadPciCfgEp (Socket, Ch, CPGC2_CREDIT_MCCPGC_MAIN_REG);
  Cpgc2Credit.Bits.credits_config_done = 0x1;
  MemWritePciCfgEp (Socket, Ch, CPGC2_CREDIT_MCCPGC_MAIN_REG, Cpgc2Credit.Data);

  return EFI_SUCCESS;
}

/**
  Check if CPGC is running under infinite test

  @param[in]   Host               - Pointer to SysHost
  @param[in]   Socket             - Socket number
  @param[in]   Ch                 - Channel number
  @param[in]   Subch              - Sub channel number

  @retval TRUE                    - CPGC is running under infinite test
          FALSE                   - CPGC is not running under infinite test

**/
BOOLEAN
IsInfiniteCpgcTest (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     SubCh
  )
{
  UINT8                                            Index;
  UINT32                                           Data32;
  CPGC2_ADDRESS_INSTRUCTION_0_MCCPGC_MAIN_STRUCT   AddressInstruction;
  UINT32                                           CpgcAddrInstRegs[CPGC_MAX_NUMBER_ADDRESS_INSTRUCTION] = {
                                                                   CPGC2_ADDRESS_INSTRUCTION_0_MCCPGC_MAIN_REG,
                                                                   CPGC2_ADDRESS_INSTRUCTION_1_MCCPGC_MAIN_REG,
                                                                   CPGC2_ADDRESS_INSTRUCTION_2_MCCPGC_MAIN_REG,
                                                                   CPGC2_ADDRESS_INSTRUCTION_3_MCCPGC_MAIN_REG};

  for (Index = 0; Index < CPGC_MAX_NUMBER_ADDRESS_INSTRUCTION; Index++) {
    GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CpgcAddrInstRegs[Index], &Data32);
    AddressInstruction.Data = (UINT8) Data32;
    if (AddressInstruction.Bits.last == 1) {
      return FALSE;
    }
  }
  return TRUE;
}

/**
  Initialize CpgcHost data

  @param   None

  @retval  N/A

**/
VOID
InitCpgcHostData (
  VOID
  )
{
  SYS_SETUP        *Setup;
  CPGC2_HOST_DATA  *CpgcHost;
  CPGC2_HOST_CACHE *CpgcCachePtr;

  //
  // return if CpgcHost data is initialized
  //
  if (GetCpgcHostPointer ()) {
    return;
  }

  //
  // Allocate resource for CpgcHost data
  //
  CpgcHost = (CPGC2_HOST_DATA *) RcAllocatePool (sizeof (CPGC2_HOST_DATA));

  if (CpgcHost == NULL) {
    RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Error: Cpgc host data allocation failure.\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_201);
    return;
  }

  ZeroMem (CpgcHost, sizeof (CPGC2_HOST_DATA));

  SetCpgcHostPointer (CpgcHost);

  //
  // Query system setup data
  //
  Setup = GetSysSetupPointer ();
  if (Setup == NULL) {
    RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Error: MRC setup data is not initialized yet so that Cpgc data initialization is too early.\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_201);
    return;
  }

  CpgcHost->CpgcIn.IsCacheEnable = Setup->mem.dfxMemSetup.DfxCpgcRegCacheAccessAllowed;

  //
  // Allocate register cache resource if cache is enabled
  //
  if (CpgcHost->CpgcIn.IsCacheEnable) {
    CpgcCachePtr = (CPGC2_HOST_CACHE *) RcAllocatePool (sizeof (CPGC2_HOST_CACHE));

    if (CpgcCachePtr == NULL) {
      RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Error: Cpgc cache data allocation failure.\n");
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_201);
      return;
    }

    ZeroMem (CpgcCachePtr, sizeof (CPGC2_HOST_CACHE));

    CpgcHost->CpgcCachePtr = (UINT32) CpgcCachePtr;
  }
}

/**
  Set up the CPGC user defined DQ pattern for BSSA test

  @param Host:           Pointer to sysHost
  @param Socket:         Processor socket within the system (0-based)
  @param Channel:        Channel within socket
  @param CpgcConfig:     Pointer to a CPGC configuration structure.

**/
STATIC
VOID
SetupBssaUserDefineDqPattern (
  IN PSYSHOST     Host,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN CPGC2_CONFIG *CpgcConfig
  )
{
  UINT8  McCh;
  UINT8  Line;
  UINT8  Lane;
  UINT8  Chunk;
  UINT8  NumCacheLine;
  UINT8  Wdb2Lines[MAX_WDB2_DATA_LINES][MRC_WDB2_LANE_NUM];

  McCh = Channel % GetNumChannelPerMc ();

  ZeroMem (Wdb2Lines, sizeof (Wdb2Lines));

  //
  // Convert BSSA user defined pattern into CPCG2 WDB2 format
  //
  NumCacheLine = CpgcConfig->UserDefinedDqdbPatternLengths[McCh] - 1;

  RC_FATAL_ERROR ((NumCacheLine <= MAX_WDB2_DATA_LINES), ERR_MRC_STRUCT, ERR_ARRAY_OUT_OF_BOUNDS);

  for (Line = 0; Line < NumCacheLine; Line++) {
    for (Lane = 0; Lane < MRC_WDB2_LANE_NUM; Lane++) {
      for (Chunk = 0; Chunk < NUMBER_CHUNKS_PER_DQDB_CACHELINE; Chunk++) {
        Wdb2Lines[Line][Lane] |= ((CpgcConfig->UserDefinedDqdbPatterns[McCh][Line][Chunk][Lane / 8] >> (Lane % 8)) & BIT0) << Chunk;
      }
    }
  }

  WdbSetByteGroupMappingChip (Host, Socket, Channel, CPGC2_BYTEGROUP_DEFAULT);
  WDBFill (Host, Socket, Channel, (TWdbLine *)Wdb2Lines, NumCacheLine, 0);

}
/**
  Set up the CPGC LMN for BSSA test

  @param Host:           Pointer to sysHost
  @param Socket:         Processor socket within the system (0-based)
  @param Channel:        Channel within socket
  @param CpgcConfig:     Pointer to a CPGC configuration structure.

**/
STATIC
VOID
SetupBssaLmn (
  IN PSYSHOST     Host,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN CPGC2_CONFIG *CpgcConfig
  )
{
  UINT8 SubCh;
  UINT8 MaxChPerMc;
  UINT8 i;
  CPGC2_LMN_CONFIG LmnConfig;

  MaxChPerMc = GetNumChannelPerMc ();

  for (SubCh = 0; SubCh < CpgcGetValidSubCh (); SubCh++) {
    for (i = 0; i < NUMBER_UNISEQ_SUPPORT_LMN; i++) {
      if (CpgcConfig->DqdbUniseqModes[Channel / MaxChPerMc][Channel % MaxChPerMc][i] == LmnDqdbUniseqMode) {
        LmnConfig.LSel = 0;
        LmnConfig.LCnt = (UINT8)CpgcConfig->DqdbLmnValues[Channel / MaxChPerMc][Channel % MaxChPerMc][i][0];
        LmnConfig.MCnt = (UINT8)CpgcConfig->DqdbLmnValues[Channel / MaxChPerMc][Channel % MaxChPerMc][i][1];
        LmnConfig.NCnt = (UINT8)CpgcConfig->DqdbLmnValues[Channel / MaxChPerMc][Channel % MaxChPerMc][i][2];
        SetCpgcLmnCnt (Host, Socket, Channel, SubCh, i, LmnConfig);
      }
    } // i
  } //SubCh
}

/**
  Set up the CPGC start delay for BSSA test

  @param Host:           Pointer to sysHost
  @param Socket:         Processor socket within the system (0-based)
  @param Channel:        Channel within socket
  @param CpgcConfig:     Pointer to a CPGC configuration structure.

**/
STATIC
VOID
SetupBssaStartDelay (
  IN PSYSHOST     Host,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN CPGC2_CONFIG *CpgcConfig
  )
{
  UINT8  MaxChPerMc;
  UINT16 Dedays[SUB_CH];

  MaxChPerMc = GetNumChannelPerMc ();

  Dedays[SUB_CH_A] = CpgcConfig->StartDelays[Channel / MaxChPerMc][Channel % MaxChPerMc];
  Dedays[SUB_CH_B] = Dedays[SUB_CH_A];

  SetCpgcStartDelay (Host, Socket, 1 << Channel, Dedays);
}

/**
  Set up the CPGC ECC data source for BSSA test

  @param Host:           Pointer to sysHost
  @param Socket:         Processor socket within the system (0-based)
  @param Channel:        Channel within socket
  @param CpgcConfig:     Pointer to a CPGC configuration structure.


**/
STATIC
VOID
SetupBssaEccSource (
  IN PSYSHOST     Host,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN CPGC2_CONFIG *CpgcConfig
  )
{
  UINT8 SubCh;
  UINT8 MaxChPerMc;
  MaxChPerMc = GetNumChannelPerMc ();

  for (SubCh = 0; SubCh < CpgcGetValidSubCh (); SubCh++) {
    SetCpgcEccSource (Host, Socket, Channel, SubCh, CpgcConfig->EccDataSource[Channel % MaxChPerMc]);
  }
}

/**
  Set up the CPGC stop conidtion for BSSA test

  @param Host:           Pointer to sysHost
  @param Socket:         Processor socket within the system (0-based)
  @param Channel:        Channel within socket
  @param CpgcConfig:     Pointer to a CPGC configuration structure.

**/
STATIC
VOID
SetupBssaStopOnErr (
  IN PSYSHOST     Host,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN CPGC2_CONFIG *CpgcConfig
  )
{
  UINT8 SubCh;

  for (SubCh = 0; SubCh < CpgcGetValidSubCh (); SubCh++) {
    SetCpgcStopOnErr (Host, Socket, Channel, SubCh, CpgcConfig->CpgcStopMode);
  }
}

/**
  Set up the CPGC DPAT control for BSSA test

  @param Host:           Pointer to sysHost
  @param Socket:         Processor socket within the system (0-based)
  @param Channel:        Channel within socket
  @param CpgcConfig:     Pointer to a CPGC configuration structure.


**/
STATIC
VOID
SetupBssaDpatCfg (
  IN PSYSHOST     Host,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN CPGC2_CONFIG *CpgcConfig
  )
{
  UINT8 SubCh;
  UINT8 McCh;
  UINT8 Start;
  UINT8 End;

  McCh = Channel % GetNumChannelPerMc ();

  //
  // Default pattern uses only 1 line
  //
  Start = 0;
  End = 0;

  if (CpgcConfig->EnableUserDefinedDqdbPatterns) {
    End = CpgcConfig->UserDefinedDqdbPatternLengths[McCh] - 1;
  }

  for (SubCh = 0; SubCh < CpgcGetValidSubCh (); SubCh++) {
    SetCpgcDpatCfg (Host, Socket, Channel, SubCh, Start, End, CpgcConfig->DqdbIncRateModes[McCh], CpgcConfig->DqdbIncRates[McCh]);
  }
}

/**
  Set up the CPGC DC mask and polarity for BSSA test

  @param Host:           Pointer to sysHost
  @param Socket:         Processor socket within the system (0-based)
  @param Channel:        Channel within socket
  @param CpgcConfig:     Pointer to a CPGC configuration structure.

**/
STATIC
VOID
SetupBssaDcMaskAndPolarity (
  IN PSYSHOST     Host,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN CPGC2_CONFIG *CpgcConfig
  )
{
  UINT8  SubCh;
  UINT8  McCh;
  UINT32 MaskLo;
  UINT32 MaskHi;
  UINT8  MaskEcc;
  UINT8  i;
  UINT8  BytesInSubCh;

  McCh = Channel % GetNumChannelPerMc ();
  BytesInSubCh = DDR5_SUBCH_MAX_BITS / 8;

  for (SubCh = 0; SubCh < CpgcGetValidSubCh (); SubCh++) {
      SetCpgcDcPolarity (Host, Socket, Channel, SubCh, MaskForDcPol, CpgcConfig->DcPolarity[McCh]);

      MaskLo = 0;
      MaskHi = 0;
      MaskEcc = 0;
      for (i = 0; i < BytesInSubCh; i++) {
        if (i < 4) {
          MaskLo |= CpgcConfig->DcBitmasks[McCh][i + SubCh * BytesInSubCh] << (i * 8);
        } else {
          MaskEcc = CpgcConfig->DcBitmasks[McCh][i + SubCh * BytesInSubCh];
        }
      }
      SetCpgcDcMaskLanes (Host, Socket, Channel, SubCh, MaskLo, MaskHi, MaskEcc);
  }
}

/**
  Set up the CPGC error checker for BSSA test

  @param Host:           Pointer to sysHost
  @param Socket:         Processor socket within the system (0-based)
  @param Channel:        Channel within socket
  @param CpgcConfig:     Pointer to a CPGC configuration structure.

**/
STATIC
VOID
SetupBssaErrorChecker (
  IN PSYSHOST     Host,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN CPGC2_CONFIG *CpgcConfig
  )
{
  UINT8  SubCh;
  UINT8  McCh;
  UINT8  Mc;
  UINT8  MaxChPerMc;
  UINT32 MaskLo;
  UINT32 MaskHi;
  UINT8  MaskEcc;
  UINT8  i;
  UINT8  BytesInSubCh;

  MaxChPerMc = GetNumChannelPerMc ();
  Mc = Channel / MaxChPerMc;
  McCh = Channel % MaxChPerMc;
  BytesInSubCh = DDR5_SUBCH_MAX_BITS / 8;

  for (SubCh = 0; SubCh < CpgcGetValidSubCh (); SubCh++) {
    MaskLo = 0;
    MaskHi = 0;
    MaskEcc = 0;

    //
    // The lane/bit mask format of the BSSA CPGC test configure structre assumes:
    // Bits [31:0] or [3:0] bytes are the subchannel 0 data and Bits[39:32] or byte 4 are the ECC of subchannel 0.
    // Bits [71:40] or [8:5] bytes are the subchannel 1 data and Bits[79:72] or byte 9 are the ECC of subchannel 1.
    //
    for (i = 0; i < BytesInSubCh; i++) {
      if (i < 4) {
        MaskLo |= CpgcConfig->LaneValBitmasks[Mc][McCh][i + SubCh * BytesInSubCh] << (i * 8);
      } else {
        MaskEcc = CpgcConfig->LaneValBitmasks[Mc][McCh][i + SubCh * BytesInSubCh];
      }
    }
    SetCpgcErrChkMaskLanes (Host, Socket, Channel, SubCh, ~MaskLo, ~MaskHi, ~MaskEcc);

    SetCpgcErrChkMaskChunks (Host, Socket, Channel, SubCh, CpgcConfig->ChunkValBitmasks[Mc][McCh]);
  }
}

/**
  Get the traffic type for BSSA test.
  Returned traffic type (aka. CmdPat) is used by the CpgcTestWorker to set up the
  algoirthm.

  @param Host:           Pointer to sysHost
  @param Socket:         Processor socket within the system (0-based)
  @param Channel:        Channel within socket
  @param CpgcConfig:     Pointer to a CPGC configuration structure
  @param TrafficType:    Pointer to a storage for return data

  @retval MRC_STATUS_SUCCESS, MRC_FUNCTION_NOT_IMPLEMENTED
**/
STATIC
MRC_STATUS
GetBssaTrafficType (
  IN  PSYSHOST     Host,
  IN  UINT8        Socket,
  IN  UINT8        Channel,
  IN  CPGC2_CONFIG *CpgcConfig,
  OUT UINT8        *TrafficType
  )
{
  UINT8      McCh;
  UINT8      Mc;
  MRC_STATUS RetStatus;

  Mc = Channel / GetNumChannelPerMc ();
  McCh = Channel % GetNumChannelPerMc ();

  RetStatus = MRC_STATUS_SUCCESS;

  switch (CpgcConfig->TrafficModes[Mc][McCh]) {
  case LoopbackTrafficMode:
    *TrafficType = PatBack2BackWrRd;
    break;
  case WriteOnlyTrafficMode:
    *TrafficType = PatWr;
    break;
  case ReadOnlyTrafficMode:
    *TrafficType = PatRd;
    break;
  default:
    *TrafficType = PatBack2BackWrRd;
    RetStatus = MRC_FUNCTION_NOT_IMPLEMENTED;
  }

  return RetStatus;
}

/**
  Both pattern and checker setting are "static" which not change during the test even switch to different tested DIMM / Rank.

  @param Host           Pointer to sysHost
  @param socket         Processor socket within the system (0 - based)
  @param MemSs          MemSS number within the socket (0-based)
  @param ChannelMask    Channel mask within the MemSS
  @param Dimm           Dimm index inside the channel
  @param Rank           Phyiscal rank index inisde the dimm
  @param CpgcConfig     Pointer to a CPGC configuration structure.

  @retval MRC_STATUS

**/
MRC_STATUS
SetupPatternAndCheckerForBssaCpgcTest (
  IN  PSYSHOST        Host,
  IN  UINT8           Socket,
  IN  MRC_MST         MemSs,
  IN  UINT32          ChannelMask,
  IN  UINT8           Dimm,
  IN  UINT8           Rank,
  IN  CPGC2_CONFIG    *CpgcConfig
  )
{
  UINT8               Ch;
  UINT8               MaxChPerMc;
  UINT8               McCh;
  UINT8               Mc;
  UINT8               MaxChDdr;
  UINT8               i;
  UINT8               LogRank;
  CPGC_TEST_PARAM     CpgcTestParam;
  CPGC_TEST_ENV_PARAM CpgcTestEnvParam;
  CPGC_TEST_PARAM     *CpgcTestParamFromTable;
  CPGC_TEST_ENV_PARAM *CpgcTestEnvParamFromTable;
  MRC_TT              TestType;
  MRC_MST             MemSsType;

  MaxChDdr = GetMaxChDdr ();
  MaxChPerMc = GetNumChannelPerMc ();
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (!IsChannelEnabled (Socket, Ch)) {
      continue;
    }

    if ((ChannelMask & (1 << Ch)) == 0) {
      continue;
    }

    Mc = Ch / MaxChPerMc;
    McCh = Ch % MaxChPerMc;

    //
    // Binding is not enabled SPR, idle means no start test.
    //
    if (CpgcConfig->TrafficModes[Mc][McCh] == IdleTrafficMode) {
      continue;
    }

    //
    // Initialize the CPGC_TEST_PARAM and ENV_PARAM by use the training BKM setting
    //
    TestType = RmtDqTest;
    if (CpgcConfig->AddrModes[Mc][McCh] == ActiveAddrMode) {
      TestType = RmtCmdTest;
    }

    SetCurrentTestType (Socket, TestType);

    if ((GetCpgcTestParamPtrs (TestType, &CpgcTestParamFromTable) == MRC_STATUS_FAILURE) ||
      (GetCpgcTestEnvPtrs (TestType, &CpgcTestEnvParamFromTable) == MRC_STATUS_FAILURE)) {
      return MRC_STATUS_FAILURE;
    }

    CopyMem (&CpgcTestParam, CpgcTestParamFromTable, sizeof (CPGC_TEST_PARAM));
    CopyMem (&CpgcTestEnvParam, CpgcTestEnvParamFromTable, sizeof (CPGC_TEST_ENV_PARAM));

    //
    // Update the unisequence mode and seed
    //
    for (i = 0; i < CPGC_MAX_NUMBER_DPAT_UNISEQ; i++) {
      CpgcTestParam.Mux[i] = CpgcConfig->DqdbUniseqModes[Mc][McCh][i];
      CpgcTestParam.Patbuf[i] = CpgcConfig->DqdbUniseqSeeds[Mc][McCh][i];
    }

    //
    // Update CmdPat field based on the TrafficModes setting
    //
    if (GetBssaTrafficType (Host, Socket, Ch, CpgcConfig, &CpgcTestParam.CmdPat) == MRC_FUNCTION_NOT_IMPLEMENTED) {
      return MRC_STATUS_FAILURE;
    }

    //
    // Update burst length, and subseq wait
    //
    CpgcTestEnvParam.NumCacheLine = CpgcConfig->BurstLengths[Mc][McCh];
    CpgcTestEnvParam.SubSeqWait = CpgcConfig->InterSubseqWaits[Mc][McCh];

    //
    // Call worker function which provides the basic CPGC setup that commonly used in training steps.
    //
    LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);
    CpgcTestWorker (Socket, 1 << Ch, LogRank, 0, &CpgcTestParam, &CpgcTestEnvParam, DEFAULT_BASE_COL_REPEATS);

    //
    // Continue to customize the CPGC setting
    //

    SetupTestLoopCount (Host, Socket, MemSsType, 1 << Ch, (1 << (UINT8)CpgcConfig->LoopCounts[Mc][McCh]) - 1);
    SetupBssaStartDelay (Host, Socket, Ch, CpgcConfig);

    //
    // DQ pattern
    //
    if (CpgcConfig->EnableUserDefinedDqdbPatterns) {
      SetupBssaUserDefineDqPattern (Host, Socket, Ch, CpgcConfig);
    }

    SetupBssaDpatCfg (Host, Socket, Ch, CpgcConfig);
    SetCpgcDqRotateRate (Host, Socket, 1 << Ch, CpgcConfig->DqdbLaneRotationRates[McCh]);
    SetupBssaLmn (Host, Socket, Ch, CpgcConfig);
    SetupBssaEccSource (Host, Socket, Ch, CpgcConfig);
    SetupBssaDcMaskAndPolarity (Host, Socket, Ch, CpgcConfig);

    //
    // Error checker
    //
    SetupBssaStopOnErr (Host, Socket, Ch, CpgcConfig);
    SetupBssaErrorChecker (Host, Socket, Ch, CpgcConfig);

    SetCurrentTestType (Socket, 0);
  } // Ch

  return MRC_STATUS_SUCCESS;
}

/**
  Set up the CPGC Sequence and address scheme for BSSA.

  Both pattern and checker setting are "static" which not change during the test even target different Dimm/Rank.

  @param Host           Pointer to sysHost
  @param socket         Processor socket within the system (0 - based)
  @param MemSs          MemSS number within the socket (0-based)
  @param ChannelMask    Channel mask within the MemSS
  @param Dimm           Dimm index inside the channel
  @param Rank           Phyiscal rank index inisde the dimm
  @param CpgcConfig:    Pointer to a CPGC configuration structure.

  @retval MRC_STATUS

**/
MRC_STATUS
SetupTargetRankForBssaCpgcTest (
  IN  PSYSHOST        Host,
  IN  UINT8           Socket,
  IN  MRC_MST         MemSs,
  IN  UINT32          ChannelMask,
  IN  UINT8           Dimm,
  IN  UINT8           Rank,
  IN  CPGC2_CONFIG    *CpgcConfig
)
{
  UINT8 LogRank;
  UINT8 LogSubRank;
  UINT8 MaxChDdr;
  UINT8 Ch;
  struct   channelNvram (*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);
  MaxChDdr = GetMaxChDdr ();


  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (!IsChannelEnabled (Socket, Ch)) {
      continue;
    }

    if ((ChannelMask & (1 << Ch)) == 0) {
      continue;
    }

    //
    // Convert physical Dimm/physical rank into logical rank
    //
    LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);
    if ((*ChannelNvList)[Ch].encodedCSMode) {
      LogSubRank = (Rank >> 1);
    }

    //
    // Program logical to physical rank mapping
    //
    SelectCPGCRanks (Host, Socket, Ch, 1 << LogRank, 0, LogSubRank);
  } // Ch

  return MRC_STATUS_SUCCESS;
}

/**
  Set up the CPGC engine for BSSA test.

  @param Host:                     Pointer to sysHost
  @param socket:                   Processor socket within the system (0-based)
  @param MemSs                     MemSS type within the socket
  @param ChannelMask:              Channel mask within the MemSS
  @param RankListSize:             Number of items in RankList array
  @param *RankList:                List of ranks, subranks to operate on
  @param CpgcConfig:               Pointer to a CPGC configuration structure.
  @param UpdatePatternAndChecker:  Flag to update the CPGC pattern and error checker settings.
  @param UpdateSequenceAndAddress: Flag to update the CPGC sequencers and address generation settings.

  @retval MRC_STATUS_SUCCESS, MRC_STATUS_FAILURE, MRC_FUNCTION_NOT_IMPLEMENTED

**/
MRC_STATUS
EFIAPI
SetupBssaCpgcTest (
  IN PSYSHOST               Host,
  IN UINT8                  Socket,
  IN MRC_MST                MemSs,
  IN UINT32                 ChannelMask,
  IN UINT32                 RankListSize,
  IN MRC_RT                *RankList,
  IN CPGC_CONFIGURE_HEADER *CpgcConfig,
  IN BOOLEAN                UpdatePatternAndChecker,
  IN BOOLEAN                UpdateSequenceAndAddress
  )
{
  CPGC2_CONFIG *Cpgc2Config;
  UINT8         Dimm;
  UINT8         Rank;
  MRC_STATUS    RetStatus = MRC_STATUS_SUCCESS;

  //
  // Check the version of the config to ensure it is CPGC2
  //
  if (CpgcConfig->MajorVersion != 2) {
    return MRC_STATUS_FAILURE;
  }

  Cpgc2Config = (CPGC2_CONFIG*)CpgcConfig;

  //
  // Currently only support single rank test!
  //
  RC_FATAL_ERROR ((RankListSize == 1), ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_209);
  Dimm = RankList[0].Dimm;
  Rank = RankList[0].Rank;

  if (UpdatePatternAndChecker) {
    if (SetupPatternAndCheckerForBssaCpgcTest (Host, Socket, MemSs, ChannelMask, Dimm, Rank, Cpgc2Config) != MRC_STATUS_SUCCESS) {
      RetStatus = MRC_STATUS_FAILURE;
    }
  }

  if (UpdateSequenceAndAddress) {
    if (SetupTargetRankForBssaCpgcTest (Host, Socket, MemSs, ChannelMask, Dimm, Rank, Cpgc2Config) != MRC_STATUS_SUCCESS) {
      RetStatus = MRC_STATUS_FAILURE;
    }
  }

  return RetStatus;
}

/**
  Progrem CPGC loopcount.

  @param[in] Host:           Pointer to sysHost
  @param[in] Socket:         Processor socket within the system (0-based)
  @param[in] MemSs           MemSS type within the socket
  @param[in] ChannelMask:    Channel mask within the MemSS
  @param[in] Loopcount:      CPGC loopcount.

  @retval MRC_STATUS_SUCCESS, MRC_STATUS_FAILURE, MRC_FUNCTION_NOT_IMPLEMENTED

**/
MRC_STATUS
EFIAPI
SetupTestLoopCount (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN MRC_MST   MemSs,
  IN UINT32    ChannelMask,
  IN UINT32    LoopCount
  )
{
  UINT8                                                  Ch;
  UINT8                                                  SubCh;
  UINT8                                                  MaxSubCh;
  CPGC2_BLOCK_REPEATS_MCCPGC_MAIN_STRUCT                 BlockRepeats;

  MaxSubCh = CpgcGetValidSubCh ();

  for (Ch = 0; Ch < GetMaxCh (Host, Socket, MemSs); Ch++) {
    if (!(ChannelMask & (1 << Ch))) {
      continue;
    }
    for (SubCh = 0; SubCh < MaxSubCh; SubCh++) {
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CPGC2_BLOCK_REPEATS_MCCPGC_MAIN_REG, &BlockRepeats.Data);
      BlockRepeats.Bits.block_repeats = LoopCount;
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_BLOCK_REPEATS_MCCPGC_MAIN_REG, &BlockRepeats.Data);
    }
  }

  return MRC_STATUS_SUCCESS;
}

/**

  This function is used to get the lane/bit test status for BSSA test.

  If there are multiple SubChannels inside a Channel, then the LaneErroStatus
  of SubChannels are concatenated.

  For DDR5:
  SUB_CH0 32bits lane error status are stored to the LaneErrorStatus[3:0]
  SUB_CH0 8bits ECC lane error status are stored to the LaneErorStatus[4]
  SUB_CH1 32bits lane error status are stored to the LaneErrorStatus[8:5]
  SUB_CH1 8bits ECC lane error status are storeed to the LaneErrorStatus[9]

  @param[in] Host:            Pointer to sysHost
  @param[in] Socket:          Processor socket within the system (0-based)
  @param[in] MemSs            MemSS type within the socket
  @param[in] Channel          Channel number within socket (0-based)
  @param[in] TestType         Indicates the test type
  @param[in] ChunkErrSel      Select the chunck/UIs of the cache line to be checked
  @param[in] Size             The size of the Status array
  @param[out] LaneErrorStatus Pointer to array where the lane error status values will be stored.
                              The number of array elements is (MAX_BITS / 8).
                              Bit value 1 = at least one error occurred; bit value 0 = no errors occurred.

  @retval MRC_STATUS_SUCCESS, MRC_STATUS_FAILURE, MRC_FUNCTION_NOT_IMPLEMENTED

**/
MRC_STATUS
EFIAPI
GetCpgcLaneErrorStatus (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      MRC_MST   MemSs,
  IN      UINT8     Channel,
  IN      MRC_TT    TestType,
  IN      MRC_CT    ChunkErrSel,
  IN      UINT8     Size,
  OUT     UINT8     *LaneErrorStatus
  )
{
#ifdef DDR5_SUPPORT
  CPGC_ERROR_STATUS     CpgcErrorStatus[MAX_CH][SUB_CH];
  UINT32                BwSerr[SUB_CH][3]; // 0: lower DQ 32bits 1: upper DQ 32 bits. Not used in DDR5 2: ECC bits.
  UINT8                 SubCh;
  UINT8                 i;

  ZeroMem (CpgcErrorStatus, sizeof (CpgcErrorStatus));
  ZeroMem (LaneErrorStatus, Size);
  ZeroMem (BwSerr, sizeof (BwSerr));

  if (!IsChannelEnabled (Socket, Channel)) {
    return MRC_STATUS_FAILURE;
  }

  CpgcAdvTrainingErrorStatusDdr5 (Host, Socket, 1 << Channel, CpgcErrorStatus);

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

    if ((ChunkErrSel == Even) || (ChunkErrSel == All)) {
      BwSerr[SubCh][0] |= CpgcErrorStatus[Channel][SubCh].cpgcErrDat0S;
    }

    if ((ChunkErrSel == Even) || (ChunkErrSel == All)) {
      BwSerr[SubCh][0] |= CpgcErrorStatus[Channel][SubCh].cpgcErrDat2S;
    }

    if (Host->nvram.mem.eccEn) {
      if ((ChunkErrSel == Even) || (ChunkErrSel == All)) {
        BwSerr[SubCh][2] |= CpgcErrorStatus[Channel][SubCh].cpgcErrEccS & 0xFF;
      }
      if ((ChunkErrSel == Odd) || (ChunkErrSel == All)) {
        BwSerr[SubCh][2] |= (CpgcErrorStatus[Channel][SubCh].cpgcErrEccS >> 8) & 0xFF;
      }
    }
  } //Subch

  //
  // Map the SUB_CH0 32bits lane error status to LaneErrorStatus[3:0]
  // Map the SUB_CH0 8bits ECC lane error status to LaneErrorStatus[4]
  // Map the SUB_CH1 32bits lane error status to LaneErrorStatus[8:5]
  // Map the SUB_CH1 8bits ECC lane error status to LaneErrorStatus[9]
  //
  for (i = 0; i < Size; i++) {
    if (i < 4) {
      *(LaneErrorStatus + i) = (UINT8)((BwSerr[0][0] >> i * 8) & 0xFF);
    }  else if (i < 5){
      *(LaneErrorStatus + i) = (UINT8)(BwSerr[0][2] & 0xFF); // SUB_CH0 ECC
    } else if (i < 9) {
      *(LaneErrorStatus + i) = (UINT8)((BwSerr[1][0] >> (i - 5) * 8) & 0xFF);
    } else {
      *(LaneErrorStatus + i) = (UINT8)(BwSerr[1][2] & 0xFF); // SUB_CH1 ECC
    }
  } // i

  return  MRC_STATUS_SUCCESS;
#else
  //
  // DDR4 is not supportted
  //
  return MRC_FUNCTION_NOT_IMPLEMENTED;

#endif
}

/**

  This function is used to get the CPGC error counter values for BSSA test.

  If there are multiple SubChannels inside a Channel, then the error count
  of SubChannels are added together.

  @param[in] Host:            Pointer to sysHost
  @param[in] Socket:          Processor socket within the system (0-based)
  @param[in] MemSs            MemSS type within the socket
  @param[in] Channel          Channel number within socket (0-based)
  @param[in] TestType         Indicates the test type
  @param[in] Counter,         Counter ID number
  @param[in,out] CountStatus  Pointer to the erorr count status

  @retval MRC_STATUS_SUCCESS, MRC_STATUS_FAILURE, MRC_FUNCTION_NOT_IMPLEMENTED

**/
MRC_STATUS
EFIAPI
GetErrorCount (
  IN      PSYSHOST                 Host,
  IN      UINT8                    Socket,
  IN      MRC_MST                  MemSs,
  IN      UINT8                    Channel,
  IN      MRC_TT                   TestType,
  IN      UINT8                    Counter,
  OUT     CPGC_ERROR_COUNT_STATUS *CountStatus
  )
{
  UINT8   SubCh;
  UINT32  ErrorCount;
  BOOLEAN ErrorCountOverflow;

  CountStatus->Count = 0;
  CountStatus->Overflow = FALSE;

  if (!IsChannelEnabled (Socket, Channel)) {
    return MRC_STATUS_FAILURE;
  }

  if (Counter > CPGC_ERROR_COUNTER_MAX_NUMBER - 1) {
    return MRC_STATUS_FAILURE;
  }

  //
  // Sum the error count and overflow of all SubChannels
  //
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

    CpgcReadErrorCounter (Host, Socket, Channel, SubCh, Counter, &ErrorCount, &ErrorCountOverflow);

    CountStatus->Count += ErrorCount;

    if (ErrorCountOverflow) {
      CountStatus->Overflow = TRUE;
    }
  } // SubCh

  return  MRC_STATUS_SUCCESS;
}

/**

  This function is used to set the delay after CPGC start_test is asserted.

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Processor socket within the system (0-based)
  @param[in] ChbitMask        - Ch Bit mask for which channels should be setup for.
  @param[in] StartDelay       - Delays array in CPGC clock domain for SubChA and SubChB

  @retval N/A

**/
VOID
SetCpgcStartDelay (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT32   ChbitMask,
  IN UINT16   StartDelay[SUB_CH]
  )
{
  UINT8                              Ch;
  UINT8                              SubCh;
  CPGC_SEQ_CFG_B_MCCPGC_MAIN_STRUCT  SeqCfgB;

  for (Ch = 0; Ch < GetMaxChDdr (); Ch++) {
    if ((ChbitMask & (1 << Ch)) == 0) {
      continue;
    }

    for (SubCh = 0; SubCh < CpgcGetValidSubCh (); SubCh++) {
      SeqCfgB.Data = 0;
      SeqCfgB.Bits.start_delay = StartDelay[SubCh];
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC_SEQ_CFG_B_MCCPGC_MAIN_REG, &SeqCfgB.Data);
    }
  }
}

/**

  This function is used to set CPGC request data size for transactions

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Processor socket within the system (0-based)
  @param[in] ChbitMask        - Ch Bit mask for which channels should be setup for.
  @param[in] RequestDataSize  - Size of requests CPGC is issuing, 64B and 32B transactions.
                              - RequestDataSize32B (0) - 32 Byte Transactions
                              - RequestDataSize64B (1) - 64 Byte Transactions

  @retval N/A

**/
VOID
SetCpgcRequestDataSize (
  IN PSYSHOST                   Host,
  IN UINT8                      Socket,
  IN UINT32                     ChbitMask,
  IN CPGC2_REQUEST_DATA_SIZE    RequestDataSize
  )
{
  UINT8                                    Ch;
  UINT8                                    SubCh;
  CPGC2_ADDRESS_SIZE_N1_MCCPGC_MAIN_STRUCT AddressSize1;

  for (Ch = 0; Ch < GetMaxChDdr (); Ch++) {
    if ((ChbitMask & (1 << Ch)) == 0) {
      continue;
    }

    for (SubCh = 0; SubCh < CpgcGetValidSubCh (); SubCh++) {
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CPGC2_ADDRESS_SIZE_N1_MCCPGC_MAIN_REG, &AddressSize1.Data);
      AddressSize1.Bits.request_data_size = RequestDataSize;
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_ADDRESS_SIZE_N1_MCCPGC_MAIN_REG, &AddressSize1.Data);
    }
  }
}

/**

  This function is used to set ECC data source during CPGC test

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Processor socket within the system (0-based)
  @param[in] Ch               - Channel number
  @param[in] SubCh            - Sub-channel number
  @param[in] EccSource        - Ecc data source
                              - EccDataSourceByte0, EccDataSourceByte7

  @retval N/A

**/
VOID
SetCpgcEccSource (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT8                 Ch,
  IN UINT8                 SubCh,
  IN CPGC2_ECC_DATA_SOURCE EccSource
  )
{
  CPGC_DPAT_CFG_MCCPGC_MAIN_STRUCT DpatCfg;

  GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CPGC_DPAT_CFG_MCCPGC_MAIN_REG, &DpatCfg.Data);
  DpatCfg.Bits.ecc_replace_byte_control = 1;
  DpatCfg.Bits.ecc_data_source_sel = EccSource;
  GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC_DPAT_CFG_MCCPGC_MAIN_REG, &DpatCfg.Data);
}

/**

  Programs the CPGC reload seed rate register

  @param[in] Host             - Include all MRC global data.
  @param[in] socket           - Memory Controller
  @param[in] ChbitMask        - ch Bit mak for which test should be setup for.
  @param[in] ReloadSeedValue  - Reload Seed Value.

  @retval N/A
**/
VOID
SetCpgcReloadSeedRate (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT32                ChannelMask,
  IN UINT8                 ReloadSeedValue
  )
{
  CPGC_DPAT_CFG_MCCPGC_MAIN_STRUCT DpatCfg;
  UINT8                                                  Ch;
  UINT8                                                  SubCh;
  UINT8                                                  MaxSubCh;
  MRC_MST                                                MemSsType;
  MaxSubCh = CpgcGetValidSubCh ();
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
  for (Ch = 0; Ch < GetMaxCh (Host, Socket, MemSsType); Ch++) {
    if (!(ChannelMask & (1 << Ch))) {
      continue;
    }
    for (SubCh = 0; SubCh < MaxSubCh; SubCh++) {
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CPGC_DPAT_CFG_MCCPGC_MAIN_REG, &DpatCfg.Data);
      DpatCfg.Bits.reload_lfsr_seed_rate = ReloadSeedValue;
      GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC_DPAT_CFG_MCCPGC_MAIN_REG, &DpatCfg.Data);
    }
  }
}
/**

  This function is used to selects the signal masked to be driven through the inversion/DC mask with specified polarity

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Processor socket within the system (0-based)
  @param[in] Ch               - Channel number
  @param[in] SubCh            - Sub-channel number
  @param[in] DcOrInv          - Select the signal masked to be driven through the inversion or DC mask
                              - MaskForInversion, MaskForDcPol
  @param[in] Polarity         - If DC mask is selected, specify signal's polarity; Otherwise, no impact with this setting
                              - DcPolarityLogicLow, DcPolarityLogicHigh
  @retval N/A

**/
VOID
SetCpgcDcPolarity (
  IN PSYSHOST          Host,
  IN UINT8             Socket,
  IN UINT8             Ch,
  IN UINT8             SubCh,
  IN CPGC2_DC_OR_INV   DcOrInv,
  IN CPGC2_DC_POLARITY Polarity
  )
{
  CPGC_DPAT_INVDC_CFG_MCCPGC_MAIN_STRUCT DpatInvDcCfg;

  DpatInvDcCfg.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_DPAT_INVDC_CFG_MCCPGC_MAIN_REG));
  DpatInvDcCfg.Bits.dc_or_inv = DcOrInv;
  DpatInvDcCfg.Bits.dc_pol = Polarity;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_DPAT_INVDC_CFG_MCCPGC_MAIN_REG), DpatInvDcCfg.Data);
}

/**

  This function is used to specify which lanes are inverted or a DC value driven on it.

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Processor socket within the system (0-based)
  @param[in] Ch               - Channel number
  @param[in] SubCh            - Sub-channel number
  @param[in] MaskLo           - Bit mask for low 32 lanes
  @param[in] MaskHi           - Bit mask for high 32 lanes
  @param[in] MaskEcc          - Bit mask for 8 ECC lanes

  @retval N/A

**/
VOID
SetCpgcDcMaskLanes (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubCh,
  IN UINT32   MaskLo,
  IN UINT32   MaskHi,
  IN UINT8    MaskEcc
  )
{
  CPGC_DPAT_INVDC_CFG_MCCPGC_MAIN_STRUCT  DpatInvDcCfg;

  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_DPAT_INV_DC_MASK_LO_MCCPGC_MAIN_REG), MaskLo);
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_DPAT_INV_DC_MASK_HI_MCCPGC_MAIN_REG), MaskHi);

  DpatInvDcCfg.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_DPAT_INVDC_CFG_MCCPGC_MAIN_REG));
  DpatInvDcCfg.Bits.ecc_inv_dc_mask = MaskEcc;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_DPAT_INVDC_CFG_MCCPGC_MAIN_REG), DpatInvDcCfg.Data);
}

/**

  This function is used to configure LMN unisequencer

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Processor socket within the system (0-based)
  @param[in] Ch               - Channel number
  @param[in] SubCh            - Sub-channel number
  @param[in] Index            - LMN unisequencer Index
  @param[in] LmnConfig        - Structure to hold LMN unisequencer configuration data

  @retval N/A

**/
VOID
SetCpgcLmnCnt (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Ch,
  IN UINT8            SubCh,
  IN UINT8            Index,
  IN CPGC2_LMN_CONFIG LmnConfig
  )
{
  CPGC_DPAT_UNISEQ_LMN_0_MCCPGC_MAIN_STRUCT  DpatUniseqLmn;

  DpatUniseqLmn.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_DPAT_UNISEQ_LMN_0_MCCPGC_MAIN_REG + 4 * Index));
  DpatUniseqLmn.Bits.l_sel = LmnConfig.LSel;
  DpatUniseqLmn.Bits.l_cnt = LmnConfig.LCnt;
  DpatUniseqLmn.Bits.m_cnt = LmnConfig.MCnt;
  DpatUniseqLmn.Bits.n_cnt = LmnConfig.NCnt;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_DPAT_UNISEQ_LMN_0_MCCPGC_MAIN_REG + 4 * Index), DpatUniseqLmn.Data);
}

/**

  This function is used to configure error check mask for lanes

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Processor socket within the system (0-based)
  @param[in] Ch               - Channel number
  @param[in] SubCh            - Sub-channel number
  @param[in] MaskLo           - Bit mask for low 32 lanes
  @param[in] MaskHi           - Bit mask for high 32 lanes
  @param[in] MaskEcc          - Bit mask for 8 ECC lanes

  @retval N/A

**/
VOID
SetCpgcErrChkMaskLanes (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubCh,
  IN UINT32   MaskLo,
  IN UINT32   MaskHi,
  IN UINT8    MaskEcc
  )
{
  CPGC_ERR_XLNEN_MCCPGC_MAIN_STRUCT  CpgcErrXlnen;

  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_LNEN_LO_MCCPGC_MAIN_REG), MaskLo);

  if (IsDdr5Present (Host, Socket) == FALSE) {
    //
    // Only applicable to DDR4 because it was repurposed as chunk mask for DDR5
    //
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_LNEN_HI_MCCPGC_MAIN_REG), MaskHi);
  }

  CpgcErrXlnen.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_XLNEN_MCCPGC_MAIN_REG));
  CpgcErrXlnen.Bits.ecc_errchk_mask = MaskEcc;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_XLNEN_MCCPGC_MAIN_REG), CpgcErrXlnen.Data);
}

/**

  This function is used to configure error check mask for cache line

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Processor socket within the system (0-based)
  @param[in] Ch               - Channel number
  @param[in] SubCh            - Sub-channel number
  @param[in] MaskRank         - Bit mask for cache line

  @retval N/A

**/
VOID
SetCpgcErrChkMaskCacheline (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubCh,
  IN UINT8    MaskCacheLine
  )
{
  CPGC_ERR_CTL_MCCPGC_MAIN_STRUCT  ErrCtl;

  ErrCtl.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_CTL_MCCPGC_MAIN_REG));
  ErrCtl.Bits.errchk_mask_cacheline  = MaskCacheLine;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_CTL_MCCPGC_MAIN_REG), ErrCtl.Data);
}

/**

  This function is used to configure error check mask for chunks

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Processor socket within the system (0-based)
  @param[in] Ch               - Channel number
  @param[in] SubCh            - Sub-channel number
  @param[in] MaskChunk        - Bit mask for chunks

  @retval N/A

**/
VOID
SetCpgcErrChkMaskChunks (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubCh,
  IN UINT32   MaskChunk
)
{
  CPGC_ERR_CTL_MCCPGC_MAIN_STRUCT  ErrCtl;

  if (IsDdr5Present (Host, Socket)) {
    //
    // Repurpose as chunk mask for DDR5
    //
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_LNEN_HI_MCCPGC_MAIN_REG), MaskChunk);
  } else {
    ErrCtl.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_CTL_MCCPGC_MAIN_REG));
    ErrCtl.Bits.errchk_mask_chunk = (UINT8) MaskChunk & 0xFF;
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_CTL_MCCPGC_MAIN_REG), ErrCtl.Data);
  }
}

/**

  This function is used to configure DPAT control

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Processor socket within the system (0-based)
  @param[in] Ch               - Channel number
  @param[in] SubCh            - Sub-channel number
  @param[in] Start            - Starting line number in the CPGC_DPAT_EXTBUF
  @param[in] End              - Ending line number in the CPGC_DPAT_EXTBUF
  @param[in] IncRateMode      - Increment rate mode/scale
  @param[in] IncRate          - Increment rate value.

  @retval N/A

**/
VOID
SetCpgcDpatCfg (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT8                 Ch,
  IN UINT8                 SubCh,
  IN UINT8                 Start,
  IN UINT8                 End,
  IN DQDB_INC_RATE_MODE    IncRateMode,
  IN UINT8                 IncRate
  )
{
  CPGC_DPAT_BUF_CFG_MCCPGC_MAIN_STRUCT DpatBufCfg;

  DpatBufCfg.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_DPAT_BUF_CFG_MCCPGC_MAIN_REG));
  DpatBufCfg.Bits.buf_pntr_inc_scale = (IncRateMode == LinearDqdbIncRateMode) ? 1 : 0;
  DpatBufCfg.Bits.buf_pntr_inc_rate = IncRate;
  DpatBufCfg.Bits.buf_strt_pntr = Start;
  DpatBufCfg.Bits.buf_end_pntr = End;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_DPAT_BUF_CFG_MCCPGC_MAIN_REG), DpatBufCfg.Data);

}

/**

  This function is used to configure test stop condition

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Processor socket within the system (0-based)
  @param[in] Ch               - Channel number
  @param[in] SubCh            - Sub-channel number
  @param[in] Mode             - Type of StopOnError

  @retval N/A

**/
VOID
SetCpgcStopOnErr (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT8                 Ch,
  IN UINT8                 SubCh,
  IN TStopOnError          Mode
  )
{
  CPGC_ERR_CTL_MCCPGC_MAIN_STRUCT ErrCtl;

  ErrCtl.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_CTL_MCCPGC_MAIN_REG));
  ErrCtl.Bits.stop_on_n = 1;
  ErrCtl.Bits.stop_on_error_ctl = Mode;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_CTL_MCCPGC_MAIN_REG), ErrCtl.Data);
}

/**
  This function configures whether or not to generate PRBS address

  @param[in] Host              - Pointer to sysHost
  @param[in] Socket            - Socket Id
  @param[in] ChbitMask         - Ch Bit mask for which test should be setup for.
  @param[in] IsGenPrbsAddr     - Check whether or not to generate PRBS address
                               - PRBS_ADDR_EN (TRUE):  Generate PRBS address
                               - PRBS_ADDR_DIS (FALSE): Don't generate PRBS address

  @retval N/A

**/
VOID
GenPrbsAddress (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT32   ChbitMask,
  IN BOOLEAN  IsGenPrbsAddr
)
{
  CPGC2_ADDRESS_PRBS_POLY_N0_MCCPGC_MAIN_STRUCT  AddrPrbsPolyN0;
  CPGC2_ADDRESS_PRBS_POLY_N1_MCCPGC_MAIN_STRUCT  AddrPrbsPolyN1;
  CPGC2_BASE_ADDRESS_CONTROL_MCCPGC_MAIN_STRUCT  BaseAddressControl;
  UINT8                                          Ch, SubCh;

  for (Ch = 0; Ch < GetMaxChDdr(); Ch++) {
    if ((ChbitMask & (1 << Ch)) == 0) {
      continue;
    }

    for (SubCh = 0; SubCh < CpgcGetValidSubCh(); SubCh++) {

      if (IsGenPrbsAddr) {

        GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CPGC2_BASE_ADDRESS_CONTROL_MCCPGC_MAIN_REG, &BaseAddressControl.Data);
        BaseAddressControl.Bits.addr_prbs_carry_mode = 1; // Enable Address Direction/Order to dictate theorder of PRBS Address Generator fields advancing.
        BaseAddressControl.Bits.addr_prbs_col_en = 1;
        BaseAddressControl.Bits.addr_prbs_row_en = 1;
        BaseAddressControl.Bits.addr_prbs_bnk_en = 0; // b2b doesn't require bank prbs address
        BaseAddressControl.Bits.addr_prbs_rnk_en = 0; // No need to generate rank prbs address
        GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_BASE_ADDRESS_CONTROL_MCCPGC_MAIN_REG, &BaseAddressControl.Data);

        AddrPrbsPolyN0.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ADDRESS_PRBS_POLY_N0_MCCPGC_MAIN_REG));
        AddrPrbsPolyN0.Bits.address_prbs_column_poly = CPGC_PRBS3;
        MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ADDRESS_PRBS_POLY_N0_MCCPGC_MAIN_REG), AddrPrbsPolyN0.Data);

        AddrPrbsPolyN1.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ADDRESS_PRBS_POLY_N1_MCCPGC_MAIN_REG));
        AddrPrbsPolyN1.Bits.address_prbs_row_poly = CPGC_PRBS16;
        MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ADDRESS_PRBS_POLY_N1_MCCPGC_MAIN_REG), AddrPrbsPolyN1.Data);

      } else {

        GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_READ, CPGC2_BASE_ADDRESS_CONTROL_MCCPGC_MAIN_REG, &BaseAddressControl.Data);
        BaseAddressControl.Bits.addr_prbs_carry_mode = 0;
        BaseAddressControl.Bits.addr_prbs_col_en = 0;
        BaseAddressControl.Bits.addr_prbs_row_en = 0;
        BaseAddressControl.Bits.addr_prbs_bnk_en = 0;
        BaseAddressControl.Bits.addr_prbs_rnk_en = 0;
        GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_BASE_ADDRESS_CONTROL_MCCPGC_MAIN_REG, &BaseAddressControl.Data);
      }
    }
  }
}

/**

  Initialize CPGC controller rank mask registers

  @param[in] Socket  - Socket number

  @retval n/a

**/
VOID
ConfigureCpgcRankMask (
  IN UINT8 Socket
  )
{
  SYSHOST                             *Host;
  UINT8                               Ch;
  UINT8                               MaxChDdr;
  UINT8                               Dimm;
  UINT8                               Rank;
  struct dimmNvram                    (*DimmNvList) [MAX_DIMM];
  CPGC_MISCREFCTL_MCDDC_CTL_STRUCT    CpgcMiscRefCtlReg;
  CPGC_MISCZQCTL_MCDDC_CTL_STRUCT     CpgcMiscZQCtlReg;
  UINT32                              RefMaskValue;

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (!IsChannelEnabled (Socket, Ch)) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    RefMaskValue = 0x0;
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        RefMaskValue |= ((1 << Rank) << (Dimm * 2));
      } //Rank
    } //Dimm
    RefMaskValue = ~RefMaskValue | 0xF0;
    CpgcMiscRefCtlReg.Data = MemReadPciCfgEp (Socket, Ch, CPGC_MISCREFCTL_MCDDC_CTL_REG);
    CpgcMiscRefCtlReg.Bits.ref_rankmask = RefMaskValue;
    MemWritePciCfgEp (Socket, Ch, CPGC_MISCREFCTL_MCDDC_CTL_REG, CpgcMiscRefCtlReg.Data);

    CpgcMiscZQCtlReg.Data  = MemReadPciCfgEp (Socket, Ch, CPGC_MISCZQCTL_MCDDC_CTL_REG);
    CpgcMiscZQCtlReg.Bits.zq_rankmask = RefMaskValue;
    MemWritePciCfgEp (Socket, Ch, CPGC_MISCZQCTL_MCDDC_CTL_REG, CpgcMiscZQCtlReg.Data);
  }
}

/**

  Collect DDR advanced command CPGC error status

  @param[in] Host                - Pointer to sysHost
  @param[in] Socket              - Socket to train
  @param[in] DdrChEnMap          - 8-bit bit mask of ddr channels to test - (1 = active channel)
  @param[in] CpgcErrorStatus     - Error status for each channel

  @retval SUCCESS / FAILURE

  Notes:

  0 = no error
  1 = error

**/
VOID
CpgcAdvTrainingErrorStatusWorker (
  IN PSYSHOST          Host,
  IN UINT8             Socket,
  IN UINT32            DdrChEnMap,
  IN CPGC_ERROR_STATUS_OLD CpgcErrorStatus[MAX_CH][SUB_CH]
  )
{

  UINT32 Status;
  UINT8  Ch, SubCh;
  UINT8  MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Status = SUCCESS;

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    //
    // skip inactive channels
    //
    if (!(DdrChEnMap & (1 << Ch))) {
      continue;
    }
    for (SubCh = 0; SubCh < CpgcGetValidSubCh (); SubCh++) {
      //
      // Data lane [31:0] even UI error status
      //
      CpgcErrorStatus[Ch][SubCh].cpgcErrDat0S = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_RASTER_REPO_CONTENT_ECC_0_N0_MCCPGC_MAIN_REG));
      //
      // Data lane [63:32] even UI error status
      //
      CpgcErrorStatus[Ch][SubCh].cpgcErrDat1S = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_RASTER_REPO_CONTENT_ECC_0_N1_MCCPGC_MAIN_REG));
      //
      // Data lane [31:0] odd UI error status
      //
      CpgcErrorStatus[Ch][SubCh].cpgcErrDat2S = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_RASTER_REPO_CONTENT_ECC_1_N0_MCCPGC_MAIN_REG));
      //
      // Data lane [63:32] odd UI error status
      //
      CpgcErrorStatus[Ch][SubCh].cpgcErrDat3S = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_RASTER_REPO_CONTENT_ECC_1_N1_MCCPGC_MAIN_REG));
      //
      // 18 bits of even UI nibble error status. Ecc nibbles are always at nibble [9:8].
      //
      CpgcErrorStatus[Ch][SubCh].cpgcErrNibSEven = (0x3FFFF & (MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ERR_SUMMARY_A_MCCPGC_MAIN_REG)) >> 8));
      //
      // 18 bits of odd UI nibble error status. Ecc nibbles are always at nibble [9:8].
      //
      CpgcErrorStatus[Ch][SubCh].cpgcErrNibSOdd = (0x3FFFF & (MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ERR_SUMMARY_B_MCCPGC_MAIN_REG)) >> 8));
      //
      // 9 bits of byte group error status. Ecc byte group is always at byte group [8].
      //
      CpgcErrorStatus[Ch][SubCh].cpgcErrBytGrp = (0x1FF & MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC_ERR_BYTE_NTH_PAR_STAT_MCCPGC_MAIN_REG)));
      if (Host->nvram.mem.eccEn) {
        //
        // Ecc lane [7:0] even UI error status
        //
        CpgcErrorStatus[Ch][SubCh].cpgcErrEccSEven = (0xFF & MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ERR_SUMMARY_A_MCCPGC_MAIN_REG)));
        //
        // Ecc lane [7:0] odd UI error status
        //
        CpgcErrorStatus[Ch][SubCh].cpgcErrEccSOdd = (0xFF & MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ERR_SUMMARY_B_MCCPGC_MAIN_REG)));
        //
        // Ecc lane [7:0] error status
        //
        CpgcErrorStatus[Ch][SubCh].cpgcErrEccS = CpgcErrorStatus[Ch][SubCh].cpgcErrEccSEven | (CpgcErrorStatus[Ch][SubCh].cpgcErrEccSOdd << 8);
      } else {
        //
        // Clean ECC error status fields
        //
        CpgcErrorStatus[Ch][SubCh].cpgcErrEccS = 0;
        CpgcErrorStatus[Ch][SubCh].cpgcErrNibSEven &= (~(BIT8 | BIT9));
        CpgcErrorStatus[Ch][SubCh].cpgcErrNibSOdd &= (~(BIT8 | BIT9));
        CpgcErrorStatus[Ch][SubCh].cpgcErrBytGrp &= (~BIT8);
      }
      //
      // For DDR4, it's different.
      //
    }
  } // Ch

  return;

} // CpgcAdvTrainingErrorStatusWorker