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

#include <ReferenceCodeDataTypes.h>
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
#include <Memory/Ddr5Cmds.h>
#include <Memory/Ddr5MrRegs.h>
#include <Memory/Ddr5DbRegs.h>
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
#include <Library/IoAccessLib.h>
#include <Library/MemRoutingLib.h>
#include <Library/MemSweepLib.h>
#include <Library/MemRcLib.h>

#include <ReferenceCodeFatalErrors.h>
#include <Memory/MemoryCheckpointCodes.h>


#define MAX_RCD_REGISTER_TO_SWEEP   2
#define MAX_RAW_CARDS               4
//
// Signal numbers between backside A copy and B copy
//
#define QCA_DISTANCE_A_B_COPY    14
#define QCS_DISTANCE_A_B_COPY     8

//
// MAX strobe number for a QCA/QCS A or B copy.
//
#define MAX_A_B_STROBE            5

#define FILTERING                         3  //Use 3 consecutive settings to average the score, used to filter out noise
#define NO_CHANGE_ITERATIONS              3
#define FIGURE_OF_MERIT_MASK              30 //SPR_todo. Need EV to provide the value. HSD 16010478187
#define TAP_CHANGE_THRESHOLD              2

  #define MAX_RX_DFE_GAIN_BIAS                1
  #define MAX_TX_DFE_GAIN_BIAS                0
  #define MAX_DCA_DFE_GAIN_BIAS               6
  #define MAX_ITERATION_LOOP                  1

#define MAX_ITERATION_LOOP_DCADFE           1

#define RX_DFE_GAIN_BIAS_BEGIN              1
#define TX_DFE_GAIN_BIAS_BEGIN              0
#define DCA_DFE_GAIN_BIAS_BEGIN             0
#define DB_DFE_GAIN_BIAS_BEGIN              0
#define MAX_DB_DFE_GAIN_BIAS                0
#define SECOND_ITERATION_LOOP               1
#define TAP_ARRAY_INDEX(a, b ,c)            ((a-b)/c)
#define MAX_TAP_NUM                         4
#define MAX_TAP_SWEEP_RANGE                 50
#define MAX_TAP_SWEEP_RANGE_DCADFE          66
#define DELTA_BETWEEN_DQ_DFE_TAP_MR_ADDRESS 8
#define TAP_NUM_INDEX(DfeTap, TapStart)     (DfeTap - TapStart)
#define TAP1_INDEX                          0
#define TAP2_INDEX                          1
#define TAP3_INDEX                          2
#define TAP4_INDEX                          3
#define MAX_TAP_BIAS                        40
#define MIN_TAP_BIAS                        -40

#define TXDFE_TAP1_INIT_VALUE               -10
#define TXDFE_TAP2_3_4_INIT_VALUE            0

//
// DCA DFE parameter
//
#define CWV_DCA_DFE_OUTER_LOOP_START              0
#define CWV_DCA_DFE_OUTER_LOOP_STEP_SIZE          0
#define CWV_DCA_DFE_OUTER_LOOP_SWEEP_RANGE_TAP1  32
#define CWV_DCA_DFE_OUTER_LOOP_SWEEP_RANGE_TAP2  16
#define CWV_DCA_DFE_OUTER_LOOP_SWEEP_RANGE_TAP3  16
#define CWV_DCA_DFE_OUTER_LOOP_SWEEP_RANGE_TAP4  16
#define DDR5_RCD_DFETM_OUTER_LOOP_START_VALUE_MASK 0x7F

//Write ODT Latency training parameters common to both DRAM and backside LRDIMM algorithms
#define MWD_SCALE               128
#define ODTON_TDQSOFFSET_MAX    -5   // Spec defined value of -0.5 multiplied by a factor of 10 to account for rounding
#define TADC_MAX                -8   // Spec defined value of -0.8 multiplied by a factor of 10 to account for rounding
#define ODTON_DRIFT_GUARD       -100 // Spec defined value of -10  multiplied by a factor of 10 to account for rounding
#define ODTOFF_TDQSOFFSET_MAX   5    // Spec defined value of 0.5  multiplied by a factor of 10 to account for rounding
#define ODTOFF_DRIFT_GUARD      200  // Spec defined value of 20   multiplied by a factor of 10 to account for rounding

//
// Algorithm parameters
//
#define ITERATION_CHANGE_RANGE     1 //change the sweep range on the 2nd iteration. //SPR_todo confirm? HSD 16010478187
#define CHANGE_RANGE_AMOUNT        2 //change the sweep range to half the range
#define DFE_FILTERING              3 // Use 3 consecutive settings to average the score, used to filter out noise.
#define SKIP_TESTED_COEFFICIENTS   1
#define MINIMUM_RANGE              10
#define FIGURE_OF_MERIT            EYEHEIGHT

#define RECEIVE_ENABLE_OFFSET_32   32
#define RECEIVE_ENABLE_OFFSET_128  128
#pragma pack (push,1)

struct cmdEye {
  INT16 left;
  INT16 right;
};
//
// DDR5 Core Defines
//
typedef struct _CHIP_ELY_CA_CK_STRUCT {
  INT16                               le[MAX_CH][SUB_CH];
  INT16                               re[MAX_CH][SUB_CH];
  DUMMY_REG                           tcdbp;
  DUMMY_REG                           trainingEn;
} CHIP_ERLY_CA_CK_STRUCT, *PCHIP_ERLY_CA_CK_STRUCT;

//
// DDR5RCD01 Spec
// Sideband Bus Protocol Addressing Fields
//
typedef union {
  struct {
    UINT32  Reserved : 8;
    UINT32  Dev : 4;
    UINT32  Channel : 4;
    UINT32  PageNum : 8;
    UINT32  RegNum : 8;
  } Bits;
  UINT32  Data;
} RCD_SMBUS_ADDRESS_DDR5;


typedef struct _QCX_SIGNAL_RAW_CARD_MAP {
  UINT8     RcdRegisters[MAX_RCD_REGISTER_TO_SWEEP];
  UINT8     MaxRcdRegisters;
} QCX_SIGNAL_RAW_CARD_MAP;

typedef union _RAW_CARD_SELECTION {
  struct {
    UINT8       X4X8      :1;                 ///< 0: X8, 1: X4
    UINT8       DRSR      :1;                 ///< 0: SR, 1: DR
    UINT8       Reserved  :6;
  } Bits;
  UINT8     Data;
} RAW_CARD_SELECTION;

typedef enum {
  Both_Rising_Falling_Edge,
  Rising_Edge,
  Falling_Edge
} DCA_SAMPLING_EDGE;

//
// Dfe Tap Bias Program Mode
//
typedef enum {
  SWEEP_PROGRAM_MODE = 0,
  NORMAL_PROGRAM_MODE = 1,
} DfeTapBiasProgramMode;

typedef struct {
  INT8          TapBiasValue[MAX_TAP_NUM];
  UINT8         FigureofMerit;
  UINT16        Score;
} TapBias;

typedef struct {
  INT8          TapBiasValue[MAX_TAP_NUM];
  UINT16         FigureofMerit;
  UINT16        Score;
} DcaDfe_TapBias;

typedef struct {
  UINT8         FigureofMerit;
  INT16         Offset;
} EYEPOINT;

typedef struct {
  UINT16        FigureofMerit;
  INT16         Offset;
} DCADFE_EYEPOINT;

typedef struct {
  INT8          TapBiasValue[MAX_TAP_NUM];
} TapBiasValue;

typedef struct {
  INT8          Begin;
  INT8          End;
} DfeTapBiasSweepRange;

#pragma pack (pop)

//
// Define BCOM modes
//
typedef enum {
  BcomResetMode  = 0,  // The mode corresponding to BRST_n assert
  BcomNormalMode = 1,
  BcomModeMax
  } BCOM_MODE;

VOID
DisplayEdgesDDR5 (
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
DisplayCompositeEyeDDR5 (
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
SetNormalCMDTimingDdr5 (
  PSYSHOST  Host,
  UINT8     socket
  );

/**

  Determine if the given DQ bit number is valid for ECC mode on current socket for DDR5

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Bit           - DQ bit number to check

  @retVal   1     DQ bit number is not valid
  @retVal   0     DQ bit number is valid

**/
BOOLEAN
IsBitNotValidSubChDdr5 (
  IN      PSYSHOST        Host,
  IN      UINT8           Bit
  );

  /**

Program Backside ODT

@param Host        - Pointer to sysHost
@param Socket      - Socket number

@retval SUCCESS

**/
UINT32
ProgramBacksideOdt(
  PSYSHOST  Host,
  UINT8     Socket
);

/**

Program QCK Delay.

@param rank        - rank number
@param signal      - Signal
@param Delay       - Delay

@retval N/A

**/
UINT32
ProgramQckDelay (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  GSM_CSN   signal,
  UINT8     mode,
  INT16     Delay
  );

/**

  Get/Set DDR5 QCSx Delay
  QCSx is the CSx Signal from the RCD to the DRAM
  Programs QCS delay depending on the Raw Card detected

  @param[in]      Host      - Pointer to SysHost
  @param[in]      Socket    - Socket number
  @param[in]      Ch        - Channel number
  @param[in]      Dimm      - DIMM number within the channel
  @param[in]      Rank      - DIMM Rank number
  @param[in]      Signal    - Signal name, valid signals are: Q0CS0_N, Q0CS1_N, Q0CS2_N, Q0CS3_N, Q1CS0_N, Q1CS1_N, Q1CS2_N, Q1CS3_N
  @param[in]      BacksideCopy - QCS A or B copy
  @param[in]      Mode      - Access Mode
  @param[in,out]  Delay     - Pointer to Delay value
  @param[in,out]  MinVal    - Pointer to a memory location that keep track of the minimum value
  @param[in,out]  MaxVal    - Pointer to a memory location that keep track of the maximum value

  @retval   - MRC_STATUS_SUCCESS if command completed successfully
  @retval   - !MRC_STATUS_SUCCESS otherwise

**/
MRC_STATUS
GetSetQcsDelay (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      GSM_CSN   Signal,
  IN      UINT8     BacksideCopy,
  IN      UINT8     Mode,
  IN OUT  INT16     *Delay,
  IN OUT  INT16     *MinVal,    OPTIONAL
  IN OUT  INT16     *MaxVal     OPTIONAL
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

  Display the bitwise error status for DDR5.

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure struct
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number (0-based)
  @param[in] SubCh   - Current sub channel
  @param[in] Dimm    - Current dimm
  @param[in] Rank    - Rank number (0-based)
  @param[in] BwSerr  - BitWise error status
  @param[in] Margin  - Margin

  @retval None

**/
VOID
DisplayBwSerrDdr5Ch (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubCh,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT32   BwSerr[3],
  IN INT16    Margin
  );

/**
  Displays the bitwise error status

  @param[in] Host:                   Pointer to sysHost
  @param[in] Socket:                 Processor Socket
  @param[in] Ch:                     Memory Channel
  @param[in] SubCh:                  Memory Sub Channel
  @param[in] Dimm:                   Dimm Number
  @param[in] Rank:                   Rank Number
  @param[in] BwSerr[2][3]            BitWise error status
  @param[in] Margin:                 Margin
  @param[in] AllStrobePassDqsOffset  Subchannel pass info

  @retval N/A

**/
VOID
DisplayBwSerrEvenOddDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     SubCh,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT32    BwSerr[2][3],
  IN INT16     Margin,
  IN UINT8     AllStrobePassDqsOffset [MAX_CH][SUB_CH]
  );


/**

Displays the DDR5 CWL_ADJ results

@param Host    - Pointer to sysHost, the system Host (root) structure struct
@param socket  - Socket Id
@param mode    - Program mode.
@param group   - MRC_GT group

@retval None

**/
VOID
DisplayCwlAdjDdr5 (
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
WriteMpcDdr5 (
  IN PSYSHOST Host,           // Pointer to the system Host (root) structure
  IN UINT8    socket,         // Socket Id
  IN UINT8    ch,             // Channel number (0-based)
  IN UINT8    dimm,           // DIMM number (0-based)
  IN UINT8    SubChannelMask, //sub channel mask
  IN UINT8    rank,           // Rank number (0-based)
  IN UINT16   opcode,         // MPC command opcode
  IN UINT8    timingmode,      //It supports 1N, 2N, 4N.
  IN BOOLEAN  PassThroughMode  //PassThrough mode or not
  );

/**

Writes Vref CS Command

@param[in] Host             - Pointer to sysHost
@param[in] Socket           - Socket Id
@param[in] Ch               - Channel number (0-based)
@param[in] Dimm             - DIMM number (0-based)
@param[in] Subchannelmask   - Sub channel mask
@param[in] Rank             - Rank number (0-based)
@param[in] Dram             - DRAM number in the subchannel
@param[in] Opcode           - Vref CA command opcode
@param[in] Timingmode       - Supports TIMING_SINGLE_CS, TIMING_MULTI_CS
@param[in] PassThroughMode  - PassThrough mode or not

@retval None

**/
VOID
WriteVrefCs (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    Subchannelmask,
  IN UINT8    Rank,
  IN UINT8    Dram,
  IN UINT16   Opcode,
  IN UINT8    Timingmode,
  IN BOOLEAN  PassThroughMode
  );

/**

Writes Vref CA Command

@param[in] Host             - Pointer to sysHost
@param[in] Socket           - Socket Id
@param[in] Ch               - Channel number (0-based)
@param[in] Dimm             - DIMM number (0-based)
@param[in] Subchannelmask   - Sub channel mask
@param[in] Rank             - Rank number (0-based)
@param[in] Dram             - DRAM number in the subchannel
@param[in] Opcode           - Vref CA command opcode
@param[in] Timingmode       - Supports TIMING_SINGLE_CS, TIMING_MULTI_CS
@param[in] PassThroughMode  - PassThrough mode or not

@retval None

**/
VOID
WriteVrefCa (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    Subchannelmask,
  IN UINT8    Rank,
  IN UINT8    Dram,
  IN UINT16   Opcode,
  IN UINT8    Timingmode,
  IN BOOLEAN  PassThroughMode
  );

/**
InitDramInterface

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket Id
  @param[in] Ch         - Channel number (0-based)
  @param[in] Dimm       - DIMM number (0-based)
  @param[in] Rank       - Rank number (0-based)
  @param[in] Opcode     - opcode for MPC command
  @param[in] Timingmode - support MPC_1N, MPC_2N, MPC_4N.

@retval N/A

**/
VOID
InitDramInterface (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    SubChannelMask,
  IN UINT8    Rank
  );

/**
Init Db and Dram Interface

@param[in] Socket          - Socket Id
@param[in] Ch              - Channel number (0-based)
@param[in] Dimm            - DIMM number (0-based)
@param[in] SubChannelMas   - opcode for MPC command
@param[in] Rank            - support MPC_1N, MPC_2N, MPC_4N.

@retval N/A

**/
VOID
EFIAPI
InitDbDramInterface (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    SubChannelMask,
  IN UINT8    Rank
  );

  /**
InitDbInterface

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket Id
  @param[in] Ch              - Channel number (0-based)
  @param[in] Dimm            - DIMM number (0-based)
  @param[in] SubChannelMask  - Sub channel mask
  @param[in] Rank            - Rank number (0-based)

@retval N/A

**/
VOID
InitDbInterface (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    SubChannelMask,
  IN UINT8    Rank
  );

/**
  Configure the data buffer DQ pass-through mode for read.

  @param[in] Socket     - Socket Id
  @param[in] Channel    - Channel number (0-based)
  @param[in] Dimm       - DIMM number (0-based)
  @param[in] Rank       - Rank number (0-based)
  @param[in] Enable     - TRUE: Enter DB DQ pass-through mode for read.
                          FALSE: Exit DB DQ pass-through mode.

  @retval N/A

**/
VOID
SetDbDqPassThroughModeForRead (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN BOOLEAN  Enable
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

Writes Vref CS Command for DDR5

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
WriteVrefCsDdr5 (
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
  Program signal CA Vref.

  This function calls the correct function based on whether a DCPMM DIMM is
  present or not. For RcSim, it also calls a function to prime the feedback
  logic.

  @param[in]  Host        - Pointer to sysHost
  @param[in]  Socket      - Socket number
  @param[in]  Channel     - channel number
  @param[in]  Dimm        - DIMM number
  @param[in]  SubChannel  - sub-channel number
  @param[in]  Signal      - Signal
  @param[in]  CaVrefValue - CA Vref Value
**/
VOID
ProgramSignalCaVref (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT8     SubChannel,
  IN GSM_CSN   Signal,
  IN UINT8     CaVrefValue
  );

/**

Set PDA Enumerate ID.

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket to initialize
  @param[in] ChannelMask      - Bitmask of channels to init (1 in a bit position = init that channel number)

  @retval status

**/
UINT32
SetPdaEnumerateIdDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChannelMask
  );

/**
Determines a bit mask of valid strobe numbers for ECC mode on current socket for DDR5.

@param[in]      Host          - Pointer to sysHost

@retVal   UINT32    Bit mask of valid Strobe numbers
**/
UINT32
GetSubChStrobeMaskDdr5 (
  IN      PSYSHOST        Host
  );

/**

Finds cs backside margins for the given Signal

@param Host        - Pointer to sysHost
@param Socket      - Socket number
@param Dimm        - DIMM number
@param Rank        - Rank number
@param Signal      - Signal
@param *Le         - Left edge
@param *Re         - Right edge

@retval N/A

**/
VOID
GetCsBacksideMarginsSweep (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Dimm,
  UINT8     Rank,
  GSM_CSN   SignalSubCh[MAX_CH][SUB_CH],
  INT16     CsLeft[MAX_CH][SUB_CH][COPY_NUM],
  INT16     CsRight[MAX_CH][SUB_CH][COPY_NUM],
  INT16     EyeWidth[MAX_CH][SUB_CH][COPY_NUM],
  INT16     CsValue[MAX_CH][SUB_CH][COPY_NUM],
  INT8      CsVrefValue,
  struct    ClkTrainingResults (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

/*

 Returns an index to access a backside CS signal in the CsToSweepBackSide[] array

 @param[in]   Channel     Current Channel under test (0-based)
 @param[in]   SubChannel  Current SubChannel under test (0-based)
 @param[in]   Dimm        Current Dimm under test (0-based)
 @param[in]   Rank        Current Rank under test (0-based)

 @return      An index value to be used to access an element in the CsToSweepBackSide[] array

*/
UINT8
GetBacksideCsSignalIndex (
  IN        UINT8     Channel,
  IN        UINT8     SubChannel,
  IN        UINT8     Dimm,
  IN        UINT8     Rank
  );

/**

  Get the QCS signal based on the Dimm, Rank and sub-channel
  @param[in]       Dimm                   Dimm slot index (0-based)
  @param[in]       Rank                   Rank index (0-based)
  @param[in]       SubChannel             Current sub-channel under test (0-based)

  @return   QCS0_A_n, QCS1_A_n, QCS0_B_n, QCS1_B_n, QCS2_A_n, QCS3_A_n, QCS2_B_n, QCS3_B_n

**/
GSM_CSN
GetBacksideCsSignal (
  IN UINT8 Dimm,
  IN UINT8 Rank,
  IN UINT8 SubChannel
  );
/*

  Return the cache value for the current QCA signal

  @param[in]      Host    Pointer to SysHost
  @param[in]      Socket  Current socket under test (0-based)
  @param[in]      Channel Current channel under test (0-based)
  @param[in]      Dimm    Current dimm under test (0-based)
  @param[in]      Rank    Current rank under test (0-based)
  @param[in]      Signal  Current signal under test (expected to be between Q0ACA0-->Q1ACA13)
  @param[in]      UpdateCache   TRUE: Updates cache in global Structure, FALSE: Skips update
  @param[in,out]  Value   Pointer to memory location that contains the value to be written

  @return   The cache value for the current QCA signal

*/
UINT8
GetUpdateCachedQcaDelay (
  IN  OUT   PSYSHOST    Host,
  IN        UINT8       Socket,
  IN        UINT8       Channel,
  IN        UINT8       Dimm,
  IN        UINT8       Rank,
  IN        GSM_CSN     Signal,
  IN        BOOLEAN     UpdateCache,
  IN        UINT8       *Value
  );

/*

 Returns an index to access a backside CA signal in the SignalsToSweepBacksideDDR5[] array

 @param[in]   Channel     Current Channel under test (0-based)
 @param[in]   SubChannel  Current SubChannel under test (0-based)
 @param[in]   Dimm        Current Dimm under test (0-based)
 @param[in]   Rank        Current Rank under test (0-based)
 @param[in]   SignalIndex This is the logical index and MUST be between 0 to (BACKSIDE_CA_SIGNALS_TO_SWEEP - 1)

 @return      An index value to be used to access an element in the SignalsToSweepBacksideDDR5[] array

*/
UINT8
GetBacksideCaSignalIndex (
  IN        UINT8     Channel,
  IN        UINT8     SubChannel,
  IN        UINT8     Dimm,
  IN        UINT8     Rank,
  IN        UINT8     SignalIndex
  );

/**
  Execute 1N workaround required for emulation only

  @param[in]  Host    - Pointer to SysHost
  @param[in]  Socket  - Socket number

  @retval N/A

**/
VOID
Emulation1nCmdTimingWorkaround (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  );

/**
  Determine if MRW should be sent over SMBUS

  @param[in]  Host    - Pointer to SysHost
  @param[in]  Socket  - Socket number

  @retval TRUE  - Use SMBUS to send MRW
  @retval FALSE - Do not use SMBUS for MRW

**/
BOOLEAN
UseSmbusMrw (
  IN  PSYSHOST    Host,
  IN  UINT8       Socket
  );

/**
  Check whether or not Coarse Rd Dq/Dqs training is completed

  @param[in] Socket         - Socket number

  @retval TRUE              - Coarse Rd Dq/Dqs training is completed
          FALSE             - Coarse Rd Dq/Dqs training is not completed

**/
BOOLEAN
IsCoarseRdDqDqsTrainingDone (
  IN  UINT8       Socket
  );

/**
  Enter RCD CA pass through mode.

  @param[in]  Host      Pointer to sysHost
  @param[in]  Socket    Socket ID
  @param[in]  Channel   Channel number
  @param[in]  Dimm      DIMM number
  @param[in]  Rank      Rank number

  @retval N/A
**/
VOID
EnterRcdCaPassThroughMode (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  );

/**
  Exit RCD CA pass through mode.

  @param[in]  Host      Pointer to sysHost
  @param[in]  Socket    Socket ID
  @param[in]  Channel   Channel number
  @param[in]  Dimm      DIMM number
  @param[in]  Rank      Rank number

  @retval N/A
**/
VOID
ExitRcdCaPassThroughMode (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  );

/**

  Prints the edges found for DDR5 receive enable training.

  @param[in] Host          - Pointer to sysHost
  @param[in] Socket        - Current Socket
  @param[in] Ch            - Channel number
  @param[in] PulseWidth    - Pointer to save the pulse width
  @param[in] FallingEdge   - Pointer to save the falling edge
  @param[in] RisingEdge    - Pointer to save the rising edge
  @param[in] CenterPoint   - Pointer to save the center point
  @param[in] MaxStrobe     - Maximum number of strobes

  @retval N/A

**/
VOID
DisplayEdgesReceiveEnableDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT16    PulseWidth[MAX_STROBE],
  IN UINT16    FallingEdge[MAX_STROBE],
  IN UINT16    RisingEdge[MAX_STROBE],
  IN UINT16    CenterPoint[MAX_STROBE],
  IN UINT8     MaxStrobe
  );

/*++

  Program Dfe Gain Bias.

  @param[in] Host                - Pointer to sysHost
  @param[in] Socket              - Socket nubmer
  @param[in] DfeGainBiasValue    - Dfe Gain Bias Value
  @param[in] GroupInput          - Group Input

  @retval SUCCESS

--*/
VOID
ProgramDfeGainBias (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     DfeGainBiasValue,
  IN MRC_GT    GroupInput
  );

/*++

  Program Dfe Tap Bias

  @param[in] Host                      - Pointer to sysHost
  @param[in] Socket                    - Socket nubmer
  @param[in] DfeTap                    - Dfe Tap
  @param[in] TapBiasOffsetLoopCount    - Tap Bias Offset Loop Count
  @param[in] DfeTapSweepStepSize       - Dfe Tap Sweep Step Size
  @param[in] DfeTapSweepRange          - Dfe Tap Sweep Range
  @param[in] IterationLoop             - Iteration Loop
  @param[in] TapBiasSweepBeginValue    - Tap Bias Sweep Begin Value
  @param[in] Mode                      - Mode
  @param[in] BestTapBias               - Best Tap Bias

  @retval SUCCESS

--*/
VOID
ProgramDfeTapBias (
  IN PSYSHOST              Host,
  IN UINT8                 Socket,
  IN UINT8                 DfeTap,
  IN INT8                  TapBiasOffsetLoopCount,
  IN UINT8                 DfeTapSweepStepSize,
  IN UINT8                 DfeTapSweepRange,
  IN UINT8                 IterationLoop,
  IN INT8                  TapBiasSweepBeginValue,
  IN DfeTapBiasProgramMode Mode,
  IN TapBias               (*BestTapBias) [MAX_CH][MAX_RANK_CH][MAX_BITS][MAX_TAP_NUM][MAX_ITERATION_LOOP]
  );

/**

  Dfe Tap Bias Sweep Range Init.

  @param[in] DfeTapBiasSweepRangeInitValue    - Dfe Tap Bias Sweep Range Init Value
  @param[in] GroupInput                       - Group Input

  @retval None

**/
VOID
DfeTapBiasSweepRangeInit (
  IN DfeTapBiasSweepRange (*DfeTapBiasSweepRangeInitValue) [MAX_TAP_NUM],
  IN MRC_GT    GroupInput
  );

/*++

  Get DFE Gain Bias settings

  @param[in]  GroupInput         - Group Input
  @param[out] MaxGainBiasValue   - Max Gain Bias Value
  @param[out] GainBiasValueBegin - Gain Bias Value Begin

  @retval None

--*/
VOID
GetDfeGainBias (
  IN  MRC_GT    GroupInput,
  OUT UINT8     *MaxGainBiasValue,
  OUT UINT8     *GainBiasValueBegin
  );

/**
  Initialize eyeSizeScaling for DFE (RxDFE, TxDFE) for RCSIM.

  @param[in]      Host                - Pointer to sysHost
  @param[in]      Socket              - Socket number
  @param[in]      TapIndex            - Tap Inex value
  @param[in]      SettingIndex        - Index into the Tap bias setting array
  @param[in]      IterationLoopIndex  - Iteration Loop Index
  @param[in]      GroupInput          - Group Input Value

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetEyeSizeScalingDfeDdr5Sim (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      UINT8             TapIndex,
  IN      UINT8             SettingIndex,
  IN      UINT8             IterationLoopIndex,
  IN      MRC_GT            GroupInput
  );

/**

Send MRW Command by PDA.

@param Host      - Pointer to sysHost
@param Socket    - Socket Id
@param Ch        - Channel number (0-based)
@param SubChMask - Sub Channel Mask
@param Dimm      - DIMM number (0-based)
@param Rank      - Rank number (0-based)
@param Dram      - Dram device number (0-based)
@param Address   - Address for the MRW write
@param Data      - Data to write

@retval NA

**/
VOID
EFIAPI
SendMrwPda (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Dram,
  IN UINT8    Address,
  IN UINT8    Data
  );

/**
  Display DRAM mode register contents from the cached values

  @param[in]  Socket      - Socket number

  @retval   N/A
**/
VOID
EFIAPI
DumpDramMrCache (
  IN  UINT8     Socket
  );

/**
  Program the same training parameter setting for all subchannels.
  This is used for parameters that have one setting per-subchannel.

  @param[in]      Param         - Pointer a specific parameter structures
  @param[in]      SettingIndex  - Index into the parameter's setting array

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetParamPerSubCh (
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             SettingIndex
  );

/**
  Program the same training parameter setting for all strobes/bytes in all channels
  This is used for parameters that have one setting per-strobe for each channel.

  @param[in]      Param         - Pointer a specific parameter structures
  @param[in]      SettingIndex  - Index into the parameter's setting array

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetParamPerChAllStrobes (
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             SettingIndex
  );

/**
  Sets all Dca Tco values of an output structure of just setting values to an input value

  @param[in]  Value            - Value to set
  @param[out] Settings         - Pointer the current Setting Values in the structure

  @retval None
**/
VOID
SetAllTcoSettingValues (
  IN      INT16             Value,
     OUT  INT16             (*Settings)[MAX_CH][SUB_CH]
  );

/**
  Sets all Tco DqDqs values of an output structure of just setting values to an input value

  @param[in]  Value            - Value to set
  @param[out] Settings         - Pointer the current Setting Values in the structure

  @retval None
**/
VOID
SetAllTcoDqDqsSettingValues (
  IN      INT16             Value,
     OUT  INT16             (*Settings)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 4]
  );

/**
  Sweep Tx and discover margins per subchannel per strobe

  @param[in, out] TxDqDqsResults       - Pointer to a Tco training results data structure

  @retval MRC_STATUS_SUCCESS
**/
MRC_STATUS
SweepTxPerStrobe (
  IN OUT  INT16             (*TxDqDqsResults)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 4]
  );

/**
  Copy Tco DqDqs setting values from the input structure to an output structure of just setting values

  @param[in]  BestTcoResults   - Pointer to a Tco training results data structure
  @param[out] Settings         - Pointer the current Setting Values in the structure

  @retval None
**/
VOID
GetTcoDqDqsSettingValues (
  IN      SETTINGVALUE_EYEWIDTH_PAIR  (*BestTcoResults)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 4],
     OUT  INT16                       (*Settings)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 4]
);

/*
  Update the TcoResults structure with input margin data if the input margin data represents smaller non-zero margins

  @param[in]      CommonParamaters  Pointer to the context information
  @param[in]      EyewidthPerSubCh  Array of per channel/subchannel results to check
  @param[in, out] TcoResults        Array that contains minimum per sub-channel minimum margin widths

  @retval None

*/
VOID
UpdateMinimumEyewidthInStructure (
  IN      COMMON_1D_PARAMETERS  *CommonParameters,
  IN      INT16                 (*EyewidthPerSubCh)[MAX_CH][SUB_CH],
  IN OUT  INT16                 (*TcoResults)[MAX_CH][SUB_CH]
  );

/**
  Compare a given margin results structure with the current best results and update if better

  @param[in]      Param            - Pointer a specific parameter structures
  @param[in]      SettingValue     - Pointer with values of the settings used for the input results structure
  @param[in]      Results          - Pointer that contains the margin results for the SettingValue
  @param[in, out] BestTcoResults   - Pointer to a Tco training results data structure

  @retval None
**/
VOID
UpdateBestTcoResults (
  IN     POWER_TRAIN_PARAM           *Param,
  IN     INT16                       (*SettingValue)[MAX_CH][SUB_CH],
  IN     INT16                       (*PerParamTcoResults)[MAX_CH][SUB_CH],
  IN OUT SETTINGVALUE_EYEWIDTH_PAIR  (*BestTcoResults)[MAX_CH][SUB_CH]
  );

/**
  Program the training parameter setting for all subchannels based
  on input data in a TcoResults structure.

  @param[in]      Param            - Pointer a specific parameter structures
  @param[in]      BestTcoResults   - Pointer to a Tco training results data structure

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
SetTcoPerSubChToBestResults (
  IN     POWER_TRAIN_PARAM           *Param,
  IN     SETTINGVALUE_EYEWIDTH_PAIR  (*BestTcoResults)[MAX_CH][SUB_CH]
  );

/**
  Apply the same offset setting for all subchannels.
  This is used for parameters that have one setting per-subchannel.

  @param[in]      Param         - Pointer a specific parameter structures
  @param[in]      Offset        - Offset to apply

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
ApplyOffsetToParamPerSubCh (
  IN      POWER_TRAIN_PARAM *Param,
  IN      INT16             Offset
  );

/**
  Apply the same offset setting for all strobes.
  This is used for parameters that have one setting per-strobe.

  @param[in]      Param         - Pointer a specific parameter structures
  @param[in]      Offset        - Offset to apply

  @retVal MRC_STATUS_SUCCESS
**/
MRC_STATUS
ApplyOffsetToParamPerStrobe (
  IN      POWER_TRAIN_PARAM *Param,
  IN      INT16             Offset
  );

/**
  Adds signed input value to all setting values in the output structure

  @param[in]  Group            - Training group related to the value
  @param[in]  Value            - Value to assign
  @param[out] Settings         - Pointer the current Setting Values in the structure

  @retval None
**/
VOID
OffsetAllTcoSettingValues (
  IN      MRC_GT               Group,
  IN      INT16                Value,
     OUT  INT16                (*Settings)[MAX_CH][SUB_CH]
  );

/**
  Display RCD control word contents from the cached values

  @param[in]  Socket      - Socket number

  @retval   N/A
**/
VOID
EFIAPI
DumpRcdCwCache (
  IN  UINT8     Socket
  );

/**
  Restore cached RCD control words to the RCD

  @param[in]  Socket     - Socket number
  @param[in]  Ch         - Channel number within the socket
  @param[in]  SubCh      - Subchannel within the channel
  @param[in]  Dimm       - DIMM number within the channel

  @retval   MRC_STATUS_SUCCESS if registers are restored
  @retval   MRC_STATUS_FAILURE otherwise
  @retval   MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
RestoreRcdControlWordsFromCache (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     SubCh,
  IN  UINT8     Dimm
  );

//
// DCPMM training mode context
//
typedef struct {
  UINT32  DaTrainingEn;
  UINT32  DTrainingEn;
  UINT32  HsioTrainingEn;
} DCPMM_TRAIN_CONTEXT;

/**
  Save and disable DCPMM training mode for FMC
  This function saves the DCPMM training context for restore by RestoreDcpmmTrainingContext

  @param[in]  Host                  - Pointer to SysHost
  @param[in]  Socket                - Socket number
  @param[in]  ChBitmask             - Bitmask of channels to initialize
  @param[out] TrainingContext       - DCPMM training context array pointer

  @retval N/A
**/
VOID
SaveDcpmmTrainingContext (
  IN    PSYSHOST             Host,
  IN    UINT8                Socket,
  IN    UINT32               ChBitMask,
    OUT DCPMM_TRAIN_CONTEXT  TrainingContext[MAX_CH]
  );

/**
  Resetore DCPMM training mode for FMC from context buffer

  @param[in]  Host                  - Pointer to SysHost
  @param[in]  Socket                - Socket number
  @param[in]  ChBitmask             - Bitmask of channels to initialize
  @param[in]  TrainingContext       - DCPMM training context array pointer

  @retval N/A
**/
VOID
RestoreDcpmmTrainingContext (
  IN  PSYSHOST                Host,
  IN  UINT8                   Socket,
  IN  UINT32                  ChBitMask,
  IN  DCPMM_TRAIN_CONTEXT     TrainingContext[MAX_CH]
  );

/**
  Enable DFE settings

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Socket number
  @param[in] GroupInput   - Group Input

  @retval MRC_STATUS_SUCCESS  Successfully enabled
  @retval other               An error occurred
**/
MRC_STATUS
EnableDfe (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN MRC_GT    GroupInput
  );

typedef enum {
  DcaDfeDisable = 0,
  DcaDfeEnableCircuit,
  DcaDfeEnableTraining,
  DcaDfeModeMax
} DCA_DFE_MODE;

/**
  This sets the CA training mode in the register.

  Supported modes are:

    DcaDfeEnableCircuit     - enable DFE ciruit for early training steps
    DcaDfeEnableTraining    - enable DFE training
    DcaDfeDisable           - disable DFE

  @param[in] Host           - Pointer to sysHost
  @param[in] Socket         - Socket number
  @param[in] Mode           - Enable/disable mode
  @param[in] Signal         - Signal input.

  @retval MRC_STATUS_SUCCESS  Successfully enabled
  @retval other               An error occurred
**/
MRC_STATUS
DcaDfeInitDdr5 (
  IN PSYSHOST          Host,
  IN UINT8             Socket,
  IN DCA_DFE_MODE      Mode,
  IN GSM_CSN           Signal
  );

/**
  Display Backside Edges for DDR5

  @param[in] Host              - Pointer to sysHost
  @param[in] Socket            - Current Socket
  @param[in] Ch                - Channel number
  @param[in] PulseWidth        - data of the pulse width
  @param[in] LeftEdge          - data of the left edge
  @param[in] RightEdge         - data of the right edge
  @param[in] SubChannel        - sub channel number
  @param[in] RawCardStrobe     - Raw Card Strobe

  @retval N/A
**/
VOID
DisplayBacksideEdgesDDR5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT16    PulseWidth[SUB_CH][MAX_STROBE_DDR5 / 2],
  IN INT16     LeftEdge[SUB_CH][MAX_STROBE_DDR5 / 2],
  IN INT16     RightEdge[SUB_CH][MAX_STROBE_DDR5 / 2],
  IN UINT8     SubChannel,
  IN UINT8     RawCardStrobe[MAX_A_B_STROBE]
  );

/**
  Get raw card mapping

  @param[in]  Host              - Pointer to sysHost
  @param[in]  Socket            - Current Socket
  @param[in]  Ch                - Channel number
  @param[in]  SubCh             - Sub Channel number
  @param[in]  Dimm              - Dimm number on Ch
  @param[in]  BacksideCopyNum   - Backside Copy Number
  @param[out] RawCardStrobe     - Raw Card Strober

  @retval N/A
**/
VOID
GetRawCardMapping (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     SubCh,
  IN  UINT8     Dimm,
  IN  UINT8     BacksideCopyNum,
  OUT UINT8     RawCardStrobe[MAX_A_B_STROBE]
  );
/*++

/**

  DDR5 DRAM read pattern initialization

  @param[in] Host           - Pointer to sysHost
  @param[in] Socket         - Socket to initialize
  @param[in] PatternMode    - Read test pattern mode
  @param[in] UiPattern      - UI pattern
  @param[in] PatternInvert  - Indicates DQs to invert in the pattern

  @retval N/A

**/
VOID
DramReadPatternInit (
  IN  PSYSHOST            Host,
  IN  UINT8               Socket,
  IN  READ_PATTERN_MODE   PatternMode,
  IN  UINT16              UiPattern,
  IN  UINT16              PatternInvert
  );

/**

  Read dram mode register

  @param[in]  Socket          - Socket Id
  @param[in]  Ch              - Channel number (0-based)
  @param[in]  SubChMask       - Sub-channel bit mask
  @param[in]  Dimm            - DIMM number (0-based)
  @param[in]  Rank            - Rank number (0-based)
  @param[in]  Address         - Mode register Address
  @param[out] Data            - Data array to store the returned result for all dram devices

  @retval MRC_STATUS_SUCCESS if read is successful
  @retval MRC_STATUS_FAILURE otherwise

  **/
MRC_STATUS
ReadDramMr (
  IN     UINT8        Socket,
  IN     UINT8        Ch,
  IN     UINT8        SubChMask,
  IN     UINT8        Dimm,
  IN     UINT8        Rank,
  IN     UINT8        Address,
    OUT  UINT8        (*Data)[SUB_CH][MAX_STROBE_DDR5 / 2]
  );

/**

  Read data buffer mode register

  @param[in]  Socket          - Socket Id
  @param[in]  Ch              - Channel number (0-based)
  @param[in]  SubChMask       - Sub-channel bit mask
  @param[in]  Dimm            - DIMM number (0-based)
  @param[in]  Rank            - Rank number (0-based)
  @param[in]  Address         - Mode register Address
  @param[out] Data            - Data array to store the returned result

  @retval MRC_STATUS_SUCCESS if read is successful
  @retval MRC_STATUS_FAILURE otherwise

**/
MRC_STATUS
ReadDbMr (
  IN     UINT8        Socket,
  IN     UINT8        Ch,
  IN     UINT8        SubChMask,
  IN     UINT8        Dimm,
  IN     UINT8        Rank,
  IN     UINT8        Address,
    OUT  UINT8        (*Data)[SUB_CH][MAX_STROBE_DDR5 / 2]
  );

/**
  Get Dfe Tap Sweep Parameter.
  @param[in]      Host                             - Pointer to sysHost
  @param[in]      Socket                           - Socket Number
  @param[in]      DfeTap                           - Dfe Tap
  @param[in]      IterationLoop                    - Iteration Loop
  @param[in][out] DfeTapSweepRange                 - Sweep Range
  @param[in][out] DfeTapSweepStepSize              - Dfe Tap Sweep step size
  @param[in][out] TapBiasCenter                    - Tap Bias Center
  @param[in]      DfeTapBiasSweepRangeInitValue    - Dfe Tap Bias Sweep Range Init Value
  @param[in][out] TapBiasSweepBegin                - Tap Bias Sweep Begin

  @retval SUCCESS

**/
VOID
GetDcaDfeTapSweepParameter (
  IN     PSYSHOST                Host,
  IN     UINT8                   Socket,
  IN     UINT8                   DfeTapIndex,
  IN     UINT8                   IterationLoop,
  IN OUT UINT8                   *DfeTapSweepRange,
  IN OUT UINT8                   *DfeTapSweepStepSize,
  IN OUT INT8                    (*TapBiasCenter) [MAX_CH][MAX_TAP_NUM][MAX_ITERATION_LOOP_DCADFE],
  IN     DfeTapBiasSweepRange    (*DfeTapBiasSweepRangeInitValue) [MAX_TAP_NUM],
  IN OUT INT8                    *TapBiasSweepBegin
  );

/**
  Display RCD control words by reading directly using sideband access

  @param[in]  Socket      - Socket number

  @retval   N/A
**/
VOID
EFIAPI
DumpRcdControlWords (
  IN  UINT8     Socket
  );

/**
  Determine if MRR should be sent over SMBUS

  @param[in]  Socket  - Socket number

  @retval TRUE  - Use SMBUS to send MRR
  @retval FALSE - Do not use SMBUS for MRR

**/
BOOLEAN
UseSmbusMrr (
  IN  UINT8       Socket
  );

/**

  DDR5 read DQ DQS Dfe training Pattern setup.

  @param[in] Host  - Pointer to sysHost

  @retval Status

**/
VOID
ReadDfeDramPatternInit (
  IN PSYSHOST Host
  );

/**

  Sets the lower bits [2:0] of OdtlWriteRegValue to the register encoding described in the DDR5
  Spec based on the OdtlOnWrOffset value.

  @param[in]  Socket             - Socket number
  @param[in]  Channel            - Channel number
  @param[in]  Strobe             - Strobe number
  @param[in]  OdtlOnWrOffset     - tODTLon_WR value to be programmed
  @param[out] OdtlWriteRegValue  - Pointer which stores register value to be programmed

  @retval N/A

**/
VOID
GetOdtlOnRegValue (
  IN  UINT8                         Socket,
  IN  UINT8                         Channel,
  IN  UINT8                         Strobe,
  IN  INT16                         OdtlOnWrOffset,
  OUT DDR5_MODE_REGISTER_37_STRUCT  *OdtlMr37
  );

/**

  Sets the upper bits [5:3] of OdtlWriteRegValue to the register encoding described in the DDR5
  Spec based on the OdtlOffWrOffset value.

  @param[in]  Socket             - Socket number
  @param[in]  Channel            - Channel number
  @param[in]  Strobe             - Strobe number
  @param[in]  OdtlOnWrOffset     - tODTLoff_WR value to be programmed
  @param[out] OdtlWriteRegValue  - Pointer which stores register value to be programmed

  @retval N/A

**/
VOID
GetOdtlOffRegValue (
  IN  UINT8                         Socket,
  IN  UINT8                         Channel,
  IN  UINT8                         Strobe,
  IN  INT16                         OdtlOffWrOffset,
  OUT DDR5_MODE_REGISTER_37_STRUCT  *OdtlMr37
  );
/**

  Calculates and programs ODTLon_WR_Offset and ODTLoff_WR_Offset values.

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number
  @param[in] SubChannel       - SubChannel number
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number on DIMM

  @retval N/A

**/
VOID
CalculateAndProgramDramOdtlValues (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     SubChannel,
  IN UINT8     Dimm,
  IN UINT8     Rank
  );

/**

Program BCOM Delay.

@param[in] Host            - Pointer to sysHost
@param[in] SocketSocket    - Socket number
@param[in] Ch              - Channel number
@param[in] Dimm            - Dimm number
@param[in] SubchMask       - Sub channel mask
@param[in] Rank            - Rank number
@param[in] Mode            - Get set mode
@param[in] Delay           - Delay
@param[in] PassThroughMode - Pass through mode

@retval Status

**/
UINT32
ProgramBcomDelay (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Subch,
  IN UINT8     Rank,
  IN UINT8     Mode,
  IN INT16     Delay,
  IN BOOLEAN   PassThroughMode
  );

/**

  Toggles BRST_A/BRST_B pins to de-assert BRST_n

  @param[in] Host           - Pointer to SysHost
  @param[in] Socket         - Socket number
  @param[in] Channel        - Channel number
  @param[in] SubChannelMask - Sub Channel bit-mask
  @param[in] Dimm           - Dimm number
  @param[in] Rank           - Rank number

  @retval  N/A

**/
VOID
ToggleBReset (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Channel,
  UINT8     SubChannelMask,
  UINT8     Dimm,
  UINT8     Rank
  );

/**

  This function is to init the Tx dfe tap value before Tx DFE training starts.

  @param[IN] Host                - Pointer to sysHost
  @param[IN] Socket              - Socket number

  @retval N/A

**/
VOID
TxDfeTapInitDdr5 (
  IN PSYSHOST           Host,
  IN UINT8              Socket
  );


/**

  Set BCOM[2:0] according to mode.

  @param[in] Socket         - Socket number
  @param[in] ChannelBitMask - Bitmask of channels to reset
  @param[in] Dimm           - Dimm number
  @param[in] Rank           - Rank number
  @param[in] BcomMode       - Bcom mode
                              - BcomResetMode
                              - Other modes

  @retval  MRC_STATUS_SUCCESS   Set BCOM mode successfully
  @retval  MRC_STATUS_FAILURE   Invalid input parameter

**/
MRC_STATUS
EFIAPI
SetBcomMode (
  IN UINT8     Socket,
  IN UINT32    ChannelBitMask,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN BCOM_MODE BcomMode
  );

#if FixedPcdGetBool(PcdMcrSupport)
/**
  Enter DB DQ pass through mode.

  @param[in]  Socket    Socket ID
  @param[in]  Channel   Channel number
  @param[in]  Dimm      DIMM number
  @param[in]  Rank      Rank number
  @param[in]  DqMask    DQ -> PS Mapping
  @param[in]  Rank      Pass Thru direction - Read / Write

  @retval N/A
**/
VOID
EnterDbDqPassThroughMode (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    DqMask,
  IN UINT8    PassThruDirection
  );


/**
  Exit DB DQ pass through mode.

  @param[in]  Socket    Socket ID
  @param[in]  Channel   Channel number
  @param[in]  Dimm      DIMM number
  @param[in]  Rank      Rank number

  @retval N/A
**/
VOID
ExitDbDqPassThroughMode (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  );
#endif // FixedPcdGetBool(PcdMcrSupport)
#endif // _DDR5_CORE_H_
