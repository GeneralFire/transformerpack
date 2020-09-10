/** @file
  Ddr5Core.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#ifndef  _DDR5_CORE_H_
#define  _DDR5_CORE_H_

#include <Library/Ddr5CoreLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/SysHostPointerLib.h>
#include <MaxSocket.h>

#include <MrcCommonTypes.h>
#include <MemHostChipCommon.h>
#include <Memory/MemDefaults.h>

#include "SysHost.h"
#include <Library/UsraAccessApi.h>
#include <Library/ProcMemInitChipLib.h>

#include <Library/MemoryCoreLib.h>
#include <Memory/CpgcDefinitions.h>
#include <Memory/MemoryCheckpointCodes.h>
#include "MemHostChipCommonDdr5.h"
#include <Library/BaseMemoryLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/RcMemAllocLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/CteNetLib.h>
#include <Library/PrintLib.h>

//
// DDR5 Core Defines
//
typedef struct _CHIP_ELY_CA_CK_STRUCT
{
    INT16                               le[MAX_CH][SUB_CH];
    INT16                               re[MAX_CH][SUB_CH];
    DUMMY_REG                           tcdbp;
    DUMMY_REG                           trainingEn;
} CHIP_ERLY_CA_CK_STRUCT, *PCHIP_ERLY_CA_CK_STRUCT;



VOID
DisplayEdgesDDR5(
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT16    pulseWidth[SUB_CH][MAX_STROBE_DDR5 / 2],
  INT16     LeftEdge[SUB_CH][MAX_STROBE_DDR5 / 2],
  INT16     RightEdge[SUB_CH][MAX_STROBE_DDR5 / 2],
  UINT8     SubChannel,
  UINT8     maxStrobe
);

/**

Display Composite Eye for DDR5

@param Host             - Pointer to sysHost
@param socket           - Current Socket
@param ch               - Channel number
@param LeftEdge         - Pointer to save the LeftEdge
@param RightEdge        - Pointer to save the RightEdge
@param EyeWidth         - Pointer to save the EyeWidth
@param EyeCenterPoint   - Pointer to save EyeCenterPoint
@param SubChannel       - SubChannel number
@param maxStrobe        - Maximum number of strobes

@retval N/A

**/
VOID
DisplayCompositeEyeDDR5(
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  INT16     LeftEdge[SUB_CH],
  INT16     RightEdge[SUB_CH],
  UINT16    EyeWidth[SUB_CH],
  INT16     EyeCenterPoint[SUB_CH],
  UINT8     SubChannel,
  UINT8     maxStrobe
);

VOID
SetNormalCMDTimingDdr5(
  PSYSHOST  Host,
  UINT8     socket
);

/**

Program QCK Delay.

@param rank        - rank number
@param signal      - Signal
@param Delay       - Delay

@retval N/A

**/
UINT32
ProgramQckDelay(
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  GSM_CSN   signal,
  UINT8     mode,
  INT16     Delay
);

//
// Used in MemDisplay.c
//
/**

  This function print the sample array

  @param Host      - Pointer to sysHost, the system Host (root) structure struct
  @param socket    - Socket number
  @param ch        - Channel number
  @param subch     - Sub Channel number
  @param dimm      - DIMM number
  @param ReadMode  - 5 = LRDIMM Write MWD Delay
                     4 = LRDIMM Read DQ DQS
                     3 = LRDIMM Read Recieve Enable Phase
                     2 = LRDIMM Write DWL
                     1 = Read DqDqs
                     0 = Write DqDqs

  @retval None

**/
VOID   PrintSampleArrayDdr5 (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 subch, UINT8 dimm, UINT8 ReadMode,
                             struct TrainingResultsDdr5 (*trainResInput) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]);
/**

  Displays the DDR5 training results

  @param Host    - Pointer to sysHost, the system Host (root) structure struct
  @param socket  - Socket Id
  @param group   - MRC_GT group

  @retval None

**/
VOID   DisplayResultsDdr5 (PSYSHOST Host, UINT8 socket, MRC_GT group);

/**

  Display the bitwise error status for DDR5.

  @param Host    - Pointer to sysHost, the system Host (root) structure struct
  @param socket  - Socket Id
  @param ch      - Channel number (0-based)
  @param dimm    - Current dimm
  @param rank    - Rank number (0-based)
  @param bwSerr  - BitWise error status
  @param margin  - Margin

  @retval None

**/
VOID   DisplayBwSerrDdr5 (PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 SubCh, UINT8 dimm,  UINT8 rank, UINT32 bwSerr[3], INT16 margin);  // CORE


/**

Displays the DDR5 CWL_ADJ results

@param Host    - Pointer to sysHost, the system Host (root) structure struct
@param socket  - Socket Id
@param mode    - Program mode.
@param group   - MRC_GT group

@retval None

**/
VOID   DisplayCwlAdjDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     socket,
  IN UINT8     mode,
  IN INT16     (*CwlAdj) [MAX_CH][SUB_CH][MAX_RANK_CH]
  );

/**

  Displays the DDR5 training results for specific Rank.

  @param[in] Host           - Pointer to sysHost, the system Host (root) structure struct
  @param[in] Socket         - Socket Id
  @param[in] Ch             - Ch number
  @param[in] Dimm           - Dimm number
  @param[in] Rank           - Rank number
  @param[in] Mode           - program Mode
  @param[in] DisplayINTCAL  - display internal cycle alignment or not
  @param[in] CwlAdjInput    - cwl_adj value input
  @param[in] Group          - MRC_GT Group

  @retval None

**/
VOID
DisplaySpecificRankResultsDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     socket,
  IN UINT8     ch,
  IN UINT8     dimm,
  IN UINT8     rank,
  IN UINT8     mode,
  IN BOOLEAN   DisplayINTCAL,
  IN INT16     (*CwlAdj) [MAX_CH][SUB_CH][MAX_RANK_CH],
  IN MRC_GT    group
);

/**

Displays the DDR5 training results for specific rank.

@param Host    - Pointer to sysHost, the system Host (root) structure struct
@param socket  - Socket Id
@param mode    - program mode
@param group   - MRC_GT group

@retval None

**/
VOID
DisplayTxDqsDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     socket,
  IN UINT8     mode,
  IN MRC_GT    group
);
/**

Displays the DDR5 Internal Cycle Alignment training results .

@param Host                         - Pointer to sysHost, the system Host (root) structure struct
@param socket                       - Socket Id
@param ch                           - ch number
@param dimm                         - dimm number
@param rank                         - rank number
@param mode                         - program mode
@param InternalCycleAlignmentInput  - Internal cycle alingment value input
@param group                        - MRC_GT group

@retval None

**/
VOID
DisplayInternalCycleAlingmentDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     Mode,
  IN INT16     InternalCycleAlignment [MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE_DDR5 / 2],
  IN MRC_GT    Group
);

/**

Writes Multi-Purpose Command

@param Host    - Pointer to sysHost, the system Host (root) structure
@param socket  - Socket Id
@param ch      - Channel number (0-based)
@param dimm    - DIMM number (0-based)
@param rank    - Rank number (0-based)
@param opcode    - MPC command opcode
@param timingmode    - MPC command timingmode. It supports 1N, 2N, 4N.

@retval None

**/
VOID
WriteMPC(
    IN PSYSHOST Host,
    IN UINT8    socket,
    IN UINT8    ch,
    IN UINT8    dimm,
    IN UINT8    SubChannelMask,
    IN UINT8    rank,
    IN UINT16   opcode,
    IN UINT8    timingmode,
    IN BOOLEAN  PassThroughMode
);

/**

Writes Multi-Purpose Command

@param Host    - Pointer to sysHost, the system Host (root) structure
@param socket  - Socket Id
@param ch      - Channel number (0-based)
@param dimm    - DIMM number (0-based)
@param rank    - Rank number (0-based)
@param opcode    - MPC command opcode
@param timingmode    - MPC command timingmode. It supports 1N, 2N, 4N.

@retval None

**/
VOID
WriteMpcDdr5(
    IN PSYSHOST Host,           // Pointer to the system Host (root) structure
    IN UINT8    socket,         // Socket Id
    IN UINT8    ch,             // Channel number (0-based)
    IN UINT8    dimm,           // DIMM number (0-based)
    IN UINT8    SubChannelMask, //sub channel mask
    IN UINT8    rank,           // Rank number (0-based)
    IN UINT16   opcode,         // MPC command opcode
    IN UINT8    timingmode,     //It supports 1N, 2N, 4N.
    IN BOOLEAN  PassThroughMode  //PassThrough mode or not
);

/**

Writes Vref CA Command for DDR5

@param[in] Host             - Pointer to sysHost
@param[in] Socket           - Socket Id
@param[in] Ch               - Channel number (0-based)
@param[in] Dimm             - DIMM number (0-based)
@param[in] Subchannelmask   - Sub channel mask
@param[in] Rank             - Rank number (0-based)
@param[in] Opcode           - Vref CA command opcode
@param[in] Timingmode       - Supports TIMING_SINGLE_CS, TIMING_MULTI_CS
@param[in] PassThroughMode  - PassThrough mode or not

@retval None

**/
VOID
WriteVrefCaDdr5 (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    Subchannelmask,
  IN UINT8    Rank,
  IN UINT16   Opcode,
  IN UINT8    Timingmode,
  IN BOOLEAN  PassThroughMode
);

/**

Program signal CA vref.

@param Host        - Pointer to sysHost
@param socket      - Socket number
@param ch          - channel number
@param dimm        - DIMM number
@param subch       - sub channel number
@param signal      - Signal
@param CaVref      - CA Vref Value
@retval N/A

**/
UINT32
ProgramSignalCaVref(
    IN PSYSHOST  Host,
    IN UINT8     socket,
    IN UINT8     ch,
    IN UINT8     dimm,
    IN UINT8     subch,
    IN GSM_CSN   signal,
    IN UINT8     CaVref
);

#endif // _DDR5_CORE_H_
