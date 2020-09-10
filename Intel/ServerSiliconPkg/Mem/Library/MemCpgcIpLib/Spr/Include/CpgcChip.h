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

#ifndef _CPGCCHIP_H
#define _CPGCCHIP_H

#include <ReferenceCodeDataTypes.h>
#include "MemCpgcRegs.h"

//
// Used to skip writes for registers which don't care
//
#define CPGC_DONT_CARE  0xDEADBEEF

#define CADB_8_BIT_LFSR       0

//
// In CADB always-on mode, when RCD-DIMMs (RDIMM and LRDIMM),
// only CA[6:0] of each CADB buffer rows are used.
// So 1 UI commands take 2 CADB buffer rows and 2 UI commands take 4 rows.
//
#define CADB_ROW_NUMBER_1UI_CMD          2
#define CADB_ROW_NUMBER_2UI_CMD          4
//
// CPGC Init modes
//
#define Idle_Mode                           0
#define CPGC_Active_Mode                    1
#define CPGC_Direct_Mode                    2

#define CPGC_ERR_MONITOR_ALL_CL        0xFF // monitor errors on all cachelines
#define CPGC_ERR_MONITOR_NO_CL         0x0  // do not monitor errors on any cacheline
#define CPGC_ERR_MONITOR_ALL_CHUNK     0xFF // monitor errors on all chunks
#define CPGC_ERR_MONITOR_NO_CHUNK      0x0  // do not monitor errors on any chunk
#define CPGC_ERR_NEVER_STOP_ON_ERR     0x0  // Prevent any err from causing a test to stop
#define CPGC_ERR_STOP_ON_ALL_ERR       0x3  // If all data lanes see an err (Data_ Err_Status and ECC_Err_Status) then a test is stopped.

//
// CADB Buffer Mode
//
#define CADB_BUF_MODE_DDR5_DESELECT 0
#define CADB_BUF_MODE_DDR5_SELECT   1
#define CADB_BUF_MODE_DDR4          2
//
// Write DQ-DQS pattern
// Only used for CPGC_DPAT_UNISEQ_[0:2] programming
//
#define DPAT_UNISEQ_1_1_DCLK_FREQ         0xAAAAAAAA      // 1 DCLK Frequency
#define DPAT_UNISEQ_1_2_DCLK_FREQ         0xCCCCCCCC      // 1/2 DCLK Frequency
#define DPAT_UNISEQ_1_3_DCLK_FREQ         0x8E388E38      // 1/3 DCLK Frequency
#define DPAT_UNISEQ_1_4_DCLK_FREQ         0xF0F0F0F0      // 1/4 DCLK Frequency

//
// MR26 and MR27 Default Value for LFSR Seed0 and Seed1 According to Jedec Spec
//
#define MR26_LFSR_0_DEFAULT_VALUE         0x5A
#define MR27_LFSR_1_DEFAULT_VALUE         0x3C

//
// CPGC WDB all 1's pattern which is currently used by DQ swizzling discovery
//
#define DPAT_UNISEQ_ALL_1S                0xFFFFFFFF

//
// CADB Mux# Pattern/LFSR Buffer initial values
// These patterns are chosen to cause the CADB buffer patterns to appear on the CMD pins in this order: 0 -> 15.
// NOTE: For the values below, must also program MCSCHED_CHKN_BIT.def_1 to 1. Otherwise the patterns will appear
//       on the CMD pins in this order: 1 -> 15, 0.
//
#define CADB_MUX_PATTERN0_CA              0xAAAAAAAA
#define CADB_MUX_PATTERN1_CA              0xCCCCCCCC
#define CADB_MUX_PATTERN2_CA              0xF0F0F0F0

#define CADB_MUX_PATTERN0_CA_XTALK          0x5A5A5A5A
#define CADB_MUX_PATTERN1_CA_XTALK          0x3C3C3C3C
#define CADB_MUX_PATTERN2_CA_XTALK          0xEEEEEEEE
#define CADB_MUX_PATTERN2_CA_XTALK_RCD      0xFCFCFCFC // CS pattern. Dsel_uniseq is x2 rate for RCD, need slow down with this MUX

//
// CADB Buffer Row Number for QCA Patterns
//
#define CADB2_QCA_SIMPLE_PATTERN_LINES                   8
#define CADB2_QCA_XTALK_PATTERN_LINES                    16

//
// QCA Simple pattern, 1N
//
#define CADB_MUX_PATTERN0_QCA_SIMPLE_1N_QCA0_TO_QCA6       0xEAEAEAEA // 1N mode, CA0~CA6 pattern
#define CADB_MUX_PATTERN0_QCA_SIMPLE_1N_QCA7_TO_QCA13      0xD5D5D5D5 // 1N mode, CA7~CA13 pattern
#define CADB_MUX_PATTERN1_QCA_SIMPLE_1N_PAR                0xFFFFFFFF // 1N mode, PAR not used in 1N
#define CADB_MUX_PATTERN2_QCA_SIMPLE_1N_CS                 0x3F3F3F3F // 1N mode, 1 CS_n assertion at most every 4 clks

//
// QCA Simple pattern, 2N
//
#define CADB_MUX_PATTERN0_QCA_SIMPLE_2N_QCA0_TO_QCA6       0xBBBBBBBB // 2N mode, CA0~CA6 pattern
#define CADB_MUX_PATTERN0_QCA_SIMPLE_2N_QCA7_TO_QCA13      0x77777777 // 2N mode, CA7~CA13 pattern
#define CADB_MUX_PATTERN1_QCA_SIMPLE_2N_PAR                0xAAAAAAAA // 2N mode, PAR used in 2N (RCD CA Pass through SDR mode)
#define CADB_MUX_PATTERN2_QCA_SIMPLE_2N_CS                 0xDDDDDDDD // 2N mode, 1 CS_n assertion at most every 4 clks

//
// Common LFSR seeds for QCA Xtalk patterns
//
#define CADB_MUX_PATTERN0_QCA_XTALK_LFSR0                  0x5A5A5A5A // LFSR0 seed
#define CADB_MUX_PATTERN1_QCA_XTALK_LFSR1                  0x3C3C3C3C // LFSR1 seed

//
// QCA Xtalk pattern, 1N
//
#define CADB_MUX_PATTERN2_QCA_XTALK_1N_WITH_CA13           0xFFFFFFFF // 1N mode, Used for Masking Odd UI (No Masking for CA13 support)
#define CADB_MUX_PATTERN2_QCA_XTALK_1N_WITHOUT_CA13        0x55555555 // 1N mode, Used for Masking Odd UI (Masking for NO-CA13 support)
#define CADB_MUX_PATTERN3_QCA_XTALK_1N_CS                  0x3F3F3F3F // 1N mode, 1 CS_n assertion at most every 4 clks

//
// QCA Xtalk pattern, 2N, with CA13 supported, or without CA13 supported when sweeping QCA0-QCA5 or QCA7 to QCA12
//
#define CADB_MUX_PATTERN2_QCA_XTALK_2N_PAR                 0xAAAAAAAA // 2N mode, DPAR signal toggling
#define CADB_MUX_PATTERN3_QCA_XTALK_2N_CS                  0xEEEEEEEE // 2N mode, 1 CS_n assertion at most every 4 clks

//
// QCA Xtalk pattern, 2N, without CA13 supported, QCA6 only
//
#define CADB_MUX_PATTERN0_QCA_XTALK_2N_LFSR0_QCA6_WITHOUT_CA13    0x33030FCF // 2N mode, Pattern (fixed pattern 2N with some ISI ) -->0101000100111011
#define CADB_MUX_PATTERN1_QCA_XTALK_2N_LFSR1_QCA6_WITHOUT_CA13    0xAAAAAAAA // 2N mode, Mask for even vs. odd DDR CA
#define CADB_MUX_PATTERN2_QCA_XTALK_2N_PAR_QCA6_WITHOUT_CA13      0xCCCCCCCC // 2N mode, clock pattern (2N)
#define CADB_MUX_PATTERN3_QCA_XTALK_2N_CS_QCA6_WITHOUT_CA13       0xFCFCFCFC // 2N mode, 1 CS assertion every 4 clocks (2N)

#define CADB_MUX_PATTERN0_DCA_DFE_LFSR      0x5A5A5A5A // Seed 0 for target CA
#define CADB_MUX_PATTERN1_DCA_DFE_LFSR      0x3C3C3C3C // Seed 1 for non-target CA
#define CADB_MUX_PATTERN1_DCA_DFE_QUIET     0xFFFFFFFF // Quiet pattern for non-target CA
#define CADB_MUX_PATTERN2_DCA_DUTY_CYCLE    0xFCFCFCFC // CS pattern. Dsel_uniseq is x2 rate for RCD, need slow down with this MUX

#define CADB_MUX_PATTERN0_CSCLK           0xAAAAAAAA
#define CADB_MUX_PATTERN1_CSCLK           0xCCCCCCCC
#define CADB_MUX_PATTERN2_CSCLK           0xF0F0F0F0
#define CADB_MUX_PATTERN3_CSCLK           0xFF00FF00

#define CADB_MUX_PATTERN0_BCOM            0x5A5A5A5A
#define CADB_MUX_PATTERN1_BCOM            0x3C3C3C3C
#define CADB_MUX_PATTERN2_BCOM            0xAAAAAAAA

#define CADB_MUX_CTRL_LMN          0
#define CADB_MUX_CTRL_BUFFER       1
#define CADB_MUX_CTRL_LFSR         2
#define CADB_MUX_CTRL_LFSR_GALOIS  3

#define CADB_LFSR_BIT_8            0
#define CADB_LFSR_BIT_15           1
#define CADB_LFSR_BIT_31           2
#define CADB_LFSR_BIT_23           3
#define CADB_LFSR_BIT_7            4
#define CADB_LFSR_BIT_16           5
#define CADB_LFSR_BIT_23_ALT       6
#define CADB_LFSR_BIT_32           7

#define CADB_MUX_LOAD_START   0
#define CADB_MUX_LOAD_END     1

#define CADB_BUFFER_PATTERN_10 0x0000000a
#define CADB_BUFFER_PATTERN_11 0x0000000c
#define CADB_BUFFER_PATTERN_12 0x00000000
#define CADB_BUFFER_PATTERN_13 0x00000000

//
// Common SEQUENTIAL_BUFFER_PATTERN for both CADB and CPGC
//
#define SEQUENTIAL_BUFFER_PATTERN_0 0xAAAAAAAA
#define SEQUENTIAL_BUFFER_PATTERN_1 0xCCCCCCCC
#define SEQUENTIAL_BUFFER_PATTERN_2 0xF0F0F0F0
#define SEQUENTIAL_BUFFER_PATTERN_3 0xFF00FF00

#define RCVEN_PATTERN_DDR5_0 0x0EA10CA1
#define RCVEN_PATTERN_DDR5_1 0xDEAD0A1E
#define RCVEN_PATTERN_DDR5_2 0xBEEF0D08

//
// CADB Mode Defintion
//
#define CADB_MODE_OFF                             0
#define CADB_MODE_ALWAYS_ON                       1
#define CADB_MODE_NORMAL_GLOBAL_DESELECT          2
#define CADB_MODE_NORMAL_SELECT_DESELECT          3
#define CADB_MODE_ACTIVE_SELECT_DESELECT_ONE_SHOT 4
#define CADB_MODE_MRS                             5
#define CADB_MODE_ACTIVE_GLOBAL_DESELECT          6
#define CADB_MODE_ACTIVE_SELECT_DESLECT           7

#define NO_CS_OVERRIDE  0
#define PROGRAMMABLE_CS 1
#define ONE_CYCLE_CS    2
#define TWO_CYCLE_CS    3

//
// RTL Offset Delay Values
//
#define RTL_OFFSET_0   0
#define RTL_OFFSET_1   2

#define CPGC_BASE_RW_SUBSEQ_CHIP   2 // 0010: Base Read Write
#define CPGC_BASE_WR_SUBSEQ_CHIP   3 // 0011: Base Write Read

//
// MISC Definitions
//
#define ENABLE_WAIT_FOR_DDRT_RPQ_EMPTY    1
#define ENABLE_USE_WRITE_TRACKER          1

#define SUBCH_OFFSET_CADB(SUBCH, OFFSET) (OFFSET + ((SUBCH) << 12))
#define SUBCH_OFFSET_CPGC(SUBCH, OFFSET) (OFFSET + ((SUBCH*4) << 8))

#define MAX_LG2PHRANKMAPPING_PER_REGISTER  6
#define MAX_CPGC_ADDRESS_ORDER_MAPPING     12 // Max valid address order combination
#define MAX_CPGC_ADDRESS_ORDER_NUMBER      4  // order number: 0,1,2,3
#define MAX_CPGC_ADDRESS_TYPE              4  // Col, Row, Bank, Rank

#define CADB_DSEL_UNISEQ_32                              0x8000000
#define CADB_DSEL_UNISEQ_PBUF_0_PDA                      0x0aaaaaaa
#define CADB_DSEL_UNISEQ_PBUF_1_PDA                      0x0000ffc0
#define CADB_DSEL_UNISEQ_PBUF_2_PDA                      0x0c000000
#define CADB_DSEL_UNISEQ_PBUF_3_PDA                      0x00000000

#define CADB_DSEL_UNISEQ_PBUF_0_MPC_1N                   0x2AAAAAAA
#define CADB_DSEL_UNISEQ_PBUF_1_MPC_1N                   0x000FFC00
#define CADB_DSEL_UNISEQ_PBUF_0_MPC_2N                   0x0000AAAA
#define CADB_DSEL_UNISEQ_PBUF_1_MPC_2N                   0x00000FF0

#define DRIVE_CADB_PATTERN_ON_ALL_PINS                   7
#define MPC_PDA_ENUMERATE_ID_OPCODE                      0x0c0f
#define CADB_UPPER_MASK_DDR5RSEL                         0x3F80

#define QCA_PATTERN_SHIFT(SIG, BASESIG, PATTERN) ((PATTERN << (SIG - BASESIG)) | ((PATTERN & 0x7F) >> (7 - (SIG - BASESIG))))
#define DCA_PATTERN_SHIFT_WITHOUTCA13(SIG, BASESIG, PATTERN) ((PATTERN << (SIG - BASESIG)) | ((PATTERN & 0x7F) >> (13 - (SIG - BASESIG))))
#define DCA_PATTERN_SHIFT_WITHCA13(SIG, BASESIG, PATTERN) ((PATTERN << (SIG - BASESIG)) | ((PATTERN & 0x7F) >> (14 - (SIG - BASESIG))))
#define SIG_COMPARE(SIG, SIGMIN, SIGMAX) (SIG >= SIGMIN) && (Sig <= SIGMAX)
#define DCA_PATTERN_SHIFT_WITHCA7(SIG, TARGETSIG, PATTERN) ((PATTERN << (SIG - TARGETSIG)) | ((PATTERN & 0x7F) >> (7 - (SIG - TARGETSIG))))

#define CADB2_READ_TRAINING_PATTERN_LINES                1
#define CADB2_READ_TRAINING_PATTERN_LINES_1N             1
#define CADB2_READ_TRAINING_PATTERN_LINES_2N_RCD         2
#define CADB2_READ_TRAINING_PATTERN_LINES_2N_NON_RCD     2

#define GALOIS_POLY_PRBS_15                              0x6000      //x15 + x14 + 1
#define GALOIS_POLY_16                                   0x8000
#define GALOIS_POLY_8                                    0x000000B8  // x8+x6+x5+x4+1
#define NATIVE_CPGC_CLOCK                                3

// CPGC Error Counter Control Definition
#define CPGC_ERROR_COUNTER_MAX_NUMBER           9
#define CPGC_ERROR_COUNTER_POINTER_UNUSED       0
#define CPGC_ERROR_COUNTER_POINTER_USED         1
#define CPGC_ERROR_COUNT_SEL_ON_LANE            0
#define CPGC_ERROR_COUNT_SEL_ON_BYTEGROUP       1
#define CPGC_ERROR_COUNT_SEL_ON_NIBBLEGROUP     2
#define CPGC_ERROR_COUNT_SEL_ON_PARTICULAR_UI   3
#define CPGC_ERROR_COUNT_UI_ON_ALL_UI           0
#define CPGC_ERROR_COUNT_UI_ON_EVEN_UI          1
#define CPGC_ERROR_COUNT_UI_ON_ODD_UI           2
#define CPGC_ERROR_COUNT_UI_ON_PARTICULAR_UI    3

//
// CPCG2.x byte DPAT group mapping definition
//
#define CPGC2_BYTEGROUP_XXXX  0x0
#define CPGC2_BYTEGROUP_YXXX  0x88
#define CPGC2_BYTEGROUP_XYXX  0x44
#define CPGC2_BYTEGROUP_YYXX  0xCC
#define CPGC2_BYTEGROUP_XXYX  0x22
#define CPGC2_BYTEGROUP_YXYX  0xAA
#define CPGC2_BYTEGROUP_XYYX  0x66
#define CPGC2_BYTEGROUP_YYYX  0xEE

#define CPGC2_BYTEGROUP_DEFAULT  CPGC2_BYTEGROUP_YXYX

//
// Maximum number of dpat unisequencers that CPGC 2.x can support.
// The configuration registers include dpat_uniseq[3], dpat_uniseq_cfg[3],
// dpat_uniseq_poly[3], dpat_uniseq_stagr[3] and dpat_uniseq_lmn[3]
//
#define CPGC_MAX_NUMBER_DPAT_UNISEQ             3

//
// Maximum number of address instructions, data instrutions,
// command instructions and algorithm instructions that
// CPGC 2.x can support.
//
#define CPGC_MAX_NUMBER_ADDRESS_INSTRUCTION     4
#define CPGC_MAX_NUMBER_DATA_INSTRUCTION        4
#define CPGC_MAX_NUMBER_COMMAND_INSTRUCTION     24
#define CPGC_MAX_NUMBER_ALGORITHM_INSTRUCTION   8

#define CPGC_MAX_NUMBER_REPO_CONTENT            16
#define CPGC_MAX_NUMBER_REPO_CONTENT_ECC        4
#define CPGC_REG_OFFSET_REPO_CONTENT_ECC_EVEN   0
#define CPGC_REG_OFFSET_REPO_CONTENT_ECC_ODD    2
#define CPGC_ODD_CHUNK_INC                      1
#define CPGC_EVEN_CHUNK_INC                     2
#define INDEX_BYTE_0                            0
#define INDEX_BYTE_1                            1
#define INDEX_BYTE_2                            2
#define INDEX_BYTE_3                            3

#define MRR_INVERSION_FLAG                      0xFF
#define MRR_NON_INVERSION_FLAG                  0
#define CPGC_ERROR_CHECK_MASK_CHUNK             0xFFFF
#define CPGC_ERROR_CHECK_MASK_CACHELINE         0xFF

//
// Cpgc 2.x built-in capability for WDB and CADB line number
//
#define MAX_WDB2_DATA_LINES             4   // 4 data lines in WDB per lane
#define MRC_WDB2_LANE_NUM               16  // 16 unique lanes
#define MRC_WDB2_DATA_LINE_WIDTH        8   // 8 bit width
#define CADB2_LINES                     16  // 16 pattern buffer lines in CADB2
#define CADB2_LINES_PDA_ENUMERATE_ID    6   // 6 pattern buffer lines for PDA enumerate ID

//
// DDR5 and DDR4 have different burst length so as cache line depth in WDB
//
#define WDB2_LINES_PER_CACHELINE_DDR5   2
#define WDB2_LINES_PER_CACHELINE        WDB2_LINES_PER_CACHELINE_DDR5
#define MAX_WDB2_CACHELINE              (MAX_WDB2_DATA_LINES / WDB2_LINES_PER_CACHELINE_DDR5)

//
// CPGC Raster Repo Mode
//
#define CPGC_RASTER_REPO_INACTIVE_MODE             0
#define CPGC_RASTER_REPO_BITMAP_MODE               1
#define CPGC_RASTER_REPO_SUMMARY_MODE              2
#define CPGC_RASTER_REPO_FILTERED_FAILED_ROW_MODE  3
#define CPGC_RASTER_REPO_ACCUMULATED_BITMAP_MODE   5

#define CPGC_MAX_ADV_DQ_WR_ALGO                 2
#define CPGC_MAX_ADV_DQ_RD_ALGO                 2
#define CPGC_MAX_ADV_DQ_WAIT_ALGO               4
#define CPGC_GALOIS_LFSR_SEED_0                 0x13A5FC3A // Used for Aggressor lanes
#define CPGC_GALOIS_LFSR_SEED_1                 0x24B6ED5C // Used for Victim lanes

#define PRBS_ADDR_EN                           TRUE
#define PRBS_ADDR_DIS                          FALSE

#define CPGC_PRBS3                             0x6
#define CPGC_PRBS16                            0x09924

//
// Get CPGC nibble error status from CPGC2_ERR_SUMMARY_A/B registers
//
#define  CPGC_ERR_STAT_NIBBLE_DATA(Data)       ((Data >> 8) & 0xFF)
#define  CPGC_ERR_STAT_NIBBLE_ECC(Data)        ((Data >> 16) & 0x3)
#define  CPGC_ERR_STAT_BYTE_GROUP_DATA(Data)   (Data & 0xF)
#define  CPGC_ERR_STAT_BYTE_GROUP_ECC(Data)    ((Data >> 8) & 1)
#define  CPGC2_ERR_ADDR_INFO_HI_OFFSET         14

#define CPGC_EVENT_WAIT_TIMER_DISABLE          0xF
#define CPGC_LOOP_COUNT_65536                  8192
#define DB_DFE_LOOP_COUNT                      256
#define DB_DFE_CAHCE_LINES                     256
#define DEFAULT_BASE_COL_REPEATS               0

typedef enum {
  AddressOrderColRowBankRank = 0,
  AddressOrderBankColRowRank = 1,
  AddressOrderColRowRankBank = 2,
  AddressOrderRankColRowBank = 3,
  AddressOrderBankRankColRow = 4,
  AddressOrderRankBankColRow = 5,
  AddressOrderAllUnchanged   = 6,
  AddressOrderColRowNoCarry  = 7
} CPGC2_ADDRESS_ORDER_TYPE;

typedef enum {
  AddressDirNorth             = 0,    // Col -> Row
  AddressDirDiagNorthEast     = 1,
  AddressDirDiag2SouthEash    = 2,
  AddressDirEast              = 3,    // Row -> Col
  AddressDirSouth             = 4,
  AddressDirInvDiagSouthEast  = 5,
  AddressDirInvDiag2NorthWest = 6,
  AddressDirWest              = 7
} CPGC2_ADDRESS_DIRECTION_TYPE;

typedef struct _TAddressCarry {
  UINT16                         EncodedOrder; // bit[15:12]=Col, bit[11:8]=Row, bit[7:4]=Bank,bit[3,0]= Rank
  CPGC2_ADDRESS_DIRECTION_TYPE   AddressDirection;
  CPGC2_ADDRESS_ORDER_TYPE       AddressOrder;
} TAddressCarry, *PTAddressCarry;

typedef struct {
  UINT8 LSel;
  UINT8 LCnt;
  UINT8 MCnt;
  UINT8 NCnt;
} CPGC2_LMN_CONFIG;

typedef enum {
  DcPolarityLogicLow = 0,
  DcPolarityLogicHigh = 1,
} CPGC2_DC_POLARITY;

typedef enum {
  MaskForInversion = 0,
  MaskForDcPol = 1,
} CPGC2_DC_OR_INV;

typedef enum {
  EccDataSourceByte0  = 0,
  EccDataSourceByte7  = 1,
} CPGC2_ECC_DATA_SOURCE;

typedef enum {
  RequestDataSize32B = 0,
  RequestDataSize64B = 1,
} CPGC2_REQUEST_DATA_SIZE;

typedef struct CpgcGlobalTrainingSetup {
  UINT8     startDelay;
  UINT8     addrUpdRateMode;
  UINT8     seqLoopCnt;
  UINT8     rwMode;
  UINT8     numCacheLines;
  UINT8     burstSeparation;
  UINT8     errEnChunk;
  UINT8     errEnCl;
  UINT8     stopOnErrCtl;
  UINT8     subseqEndPtr;
  UINT8     useSubSeq1;
  UINT8     cpgcLinkTrain;
  UINT8     back2backWrEn;
  UINT8     mprControl;
  UINT8     refMask;
  UINT8     zqMask;
  UINT8     x8Mode;
  BOOLEAN   InOrderExecutionEnable;
  UINT8     direction;
  UINT8     SeqDataInv[MT_MAX_SUBSEQ];
  BOOLEAN   IsUseInvtPat;
} CPGC_GLOBAL_TRAINING_SETUP;

//
// Refer to CPGC 2.x HAS
//
typedef union {
  struct {
    UINT32 Rsvd   : 2;  // [1:0]
    UINT32 Column : 16; // [17:2]
    UINT32 Row    : 14; // [31:18]
  } Bits;
  UINT32 Data;
} CPGC2_ERR_ADDR_INFO_LO;

typedef union {
  struct {
    UINT32 Row    : 4;  // [3:0]
    UINT32 Bank   : 5;  // [8:4]
    UINT32 Rank   : 3;  // [11:9]
    UINT32 Rsvd   : 20; // [31:12]
  } Bits;
  UINT32 Data;
} CPGC2_ERR_ADDR_INFO_HI;

#pragma pack(1)

//
// Cpgc private data which are only accessed inside of Cpgc library.
// It's currently reserved. Could add more by analyzing Cpgc library.
//
typedef struct {
  UINT8  Reserved;
} CPGC2_HOST_PRIV;

//
// Cpgc input Data which info are passed from outside of Cpgc library.
//
typedef struct {
  BOOLEAN IsCacheEnable;
} CPGC2_HOST_IN;

//
// Data belonging to CPGC library. It's used to seperate Cpgc data from SysHost from data perspective.
// CPGC2_HOST_IN holds input data which are passed from outside of Cpgc library.
// CPGC2_HOST_PRIV holds Cpgc private data which are only accessed inside of Cpgc library.
//
typedef struct {
  CPGC2_HOST_IN     CpgcIn;
  CPGC2_HOST_PRIV   CpgcPriv;
  UINT32            CpgcCachePtr;
} CPGC2_HOST_DATA;

#pragma pack()

/**
  Initialize CpgcHost data

  @retval N/A

**/
VOID
InitCpgcHostData (
  VOID
  );

//
// Function Decls
//

VOID
CpgcDisableGlobalStart (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch
  );

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
  );

/**

  Polls for cpgc test done on ddr channels indicated with mask

  @param Host                        - Pointer to sysHost
  @param socket                        - socket to train
  @param ddrChMask                   - ddr channel mask (1 = active channel)
  @param SubCh                       - Sub channel number

  @retval SUCCESS / FAILURE


**/
UINT32
CpgcPollGlobalTestDoneDdr (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    ddrChMask,
  UINT8     SubCh
  );

/**

    CPGC Global Test Start for DDR

    @param Host       - Pointer to sysHost
    @param socket     - socket number
    @param ddrChEnMap - bit mask of channels to use
    @param SubCh      - Sub channel number

    @retval SUCCESS

**/
UINT32
CpgcGlobalTestStartDdr (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    ddrChEnMap,
  UINT8     SubCh
  );

/**

Fix up the CS field for programming the CADB

@param cs             - Chip select
@param subrank        - Sub Rank
@param pattern        - Data to program
@param encodedCSMode  - Encoded CS mode

**/
VOID
FixupCS(
  UINT8         cs,
  UINT8         subrank,
  PatCadb2Prog0  prog0[CADB2_LINES],
  UINT8         encodedCSMode
);

/**

Global training CPGC setup

@param Host                - Pointer to sysHost
@param socket              - socket to train
@param ch                  - ddr channel to setup
@param seqLoopCnt          - Iterations of the complete Sequence loop
@param startDelay          - Delay period (in DClks) for start sequence
@param AddrUpdRateMode     - address update rate mode
@param rwMode              - subsequence type for subseq0:  0000 - base read, 0001 base write
@param numCacheLines       - number of cachelines to be transmitted
@param burstSeparation     - # of DCLK cycles between completion of the current sub-sequence and beginning
the next sub-sequence
@param errEnChunk          - Bit mask to enable any checking on any specific chunk within the Cache Line.
@param errEnCl             - Bit mask to enable what Cachelines are checked within each group of 8 received.
@param stopOnErrCtl        - Defines what type of err is will Stop a test:
00: Never Stop On Any Err
01: Stop on the Nth Any Lane Err
10: Stop on All Byte Groups Err
11: Stop on All Lanes Err
@param subseqEndPtr        - specifies end of subsequence
@param useSubSeq1          - flag to indicate init of subseq1 needed (0 - not used, 1 - base read, 2 - base writes)
@param cpgcLinkTrain       - set cpgc_link_train bit
@param back2backWrEn       - enable back2back writes
@param mprControl          - enable / disable MPR
@param refMask             - self refresh mask
@param zqMask              - zq mask

@retval SUCCESS / FAILURE

**/
UINT32
CpgcGlobalTrainingSetup(
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  CPGC_GLOBAL_TRAINING_SETUP *cpgcGblTrainingSetup
);

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
  );

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
  );

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
  );

/**

  This function fills the WDB buffer

  @param[in] Host                  - Pointer to sysHost
  @param[in] Socket                - Socket number
  @param[in] Ch                    - Channel number
  @param[in] Wdb2Lines             - Array that include data to write to the WDB
  @param[in] NumberOfWdbLines      - The number of cachelines to write
  @param[in] StartWdbDataLineIndex - Start offset data line number on the WDB.

  @retval N/A

**/
VOID
WDBFill (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN TWdbLine *Wdb2Lines,
  IN UINT8    NumberOfWdbLines,
  IN UINT8    StartWdbDataLineIndex
  );

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
  );

/**

  This function loads the WDB using PCI accesses

  @param[in] Host                   - Pointer to sysHost
  @param[in] Socket                 - Socket number
  @param[in] Ch                     - Channel number
  @param[in] NumberOfWdbLines       - Number of cachelines to load
  @param[in] Wdb2Lines              - Pointer to data to write to the WDB
  @param[in] StartWdbDataLineIndex  - Start offset/cacheline number on the WDB.

  @retval N/A

**/
VOID
WdbPreLoadChip (
  IN   PSYSHOST  Host,
  IN   UINT8     Socket,
  IN   UINT8     Ch,
  IN   UINT8     NumberOfWdbLines,
  IN   UINT8     *Wdb2Lines,
  IN   UINT8     StartWdbDataLineIndex
  );

/**

  Returns Sub-channel bitmask for given SubCh number

  @param SubCh        - Sub channel number.
  @retval SubChMask

**/
UINT8
GetSubChMask (
  UINT8     SubCh
);

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
);

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
);

/**

  Perform setup required before polling on CPGC test done registers.
  The following are initialized for each channel on the indicated memory controller:
    - DDRT Read Pending Queue timeout (occurred) indicators are cleared.
    - DDRT Read Pending Queue timeout enabled indicators are set.

  @param[in]  Host                                - Pointer to SysHost
  @param[in]  Socket                              - Socket being trained
  @param[in]  MemoryController                    - Memory controller to be polled on
  @param[in]  ChannelMask                         - Bit mask indicating channel activity (active = 1, inactive = 0)
  @param[out] DdrtReadPendingQueueTimeoutEnabled  - Array indicating whether the Read Pending Queue timeout is enabled
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
  );

/**

  Poll on channel-specific indicators of test completion.
  Polling will end when all active channels on the indicated memory controller have either finished testing.

  @param[in]  Host                                - Pointer to SysHost
  @param[in]  Socket                              - Socket being trained
  @param[in]  MemoryController                    - Memory controller to be polled on
  @param[in]  ChannelMask                         - Bit mask indicating channel activity (active = 1, inactive = 0)
  @param[in]  SubChMask                           - Bit mask indicating sub-channel activity (active = 1, inactive = 0)
  @param[in]  DdrtReadPendingQueueTimeoutEnabled  - Array indicating whether the Read Pending Queue timeout is enabled
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
  );

/**

  Poll on the global (memory controller-level) clear errors bit until it deasserts.

  @param[in]  Host              - Pointer to SysHost
  @param[in]  Socket            - Socket being trained
  @param[in]  MemoryController  - Memory controller to be polled on
  @param[in]  ChannelMask       - Bit mask indicating channel activity (active = 1, inactive = 0)
  @param[in]  SubChMask         - Bit mask indicating sub-channel activity (active = 1, inactive = 0)

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
  );

/**

  Conclude testing/polling at the memory controller-level.

  If the indicated memory controller hasn't timed out, polling will occur until the memory controller indicates that
  testing. However, if the memory controller *has* timed out, CPGC will be forced to stop.

  @param[in]  Host              - Pointer to SysHost
  @param[in]  Socket            - Socket being trained
  @param[in]  MemoryController  - Memory controller to be polled on
  @param[in]  ChannelMask       - Bit mask indicating channel activity (active = 1, inactive = 0)
  @param[in]  SubChMask         - Bit mask indicating sub-channel activity (active = 1, inactive = 0)

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
  );

/**

  Clean up the DDRT Read Pending Queue state following a test.

  This clears bits related to timeout errors and drains the DDRT Read Pending Queue if a timeout occurred.

  @param[in]  Host                                - Pointer to SysHost
  @param[in]  Socket                              - Socket being trained
  @param[in]  MemoryController                    - Memory controller to be polled on
  @param[in]  ChannelMask                         - Bit mask indicating channel activity (active = 1, inactive = 0)
  @param[in]  DdrtReadPendingQueueTimeoutEnabled  - Array indicating whether the Read Pending Queue timeout is enabled
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
  );

/**
  Get max sub ch per ch for CPGC2 IP Lib

  @param[in]  None

  @retval     0x1 or 0x2     This is the valid sub ch number for this IP

**/

UINT8
CpgcGetValidSubCh (
  VOID
  );

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
);

/**
  This function assigns CPGC uniseq1's pattern for victim DQ lanes and uniseq0's pattern for aggressor DQ lanes
  in WDB according to the given pattern length

  @param[in] Host              - Pointer to sysHost
  @param[in] Socket            - Socket Id
  @param[in] ChbitMask         - Ch Bit mask for which test should be setup for.
  @param[in] PatternWidth      - Specify how many bits/lanes of a victim-aggressor pair contains, support 2, 4, 8 and 16

  @retval SUCCESS              - Assgin Victim-Aggressor patterns successfully
          FAILURE              - Failed to assgin Victim-Aggressor patterns

**/
UINT32
WriteWDBDqVaPattern (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChbitMask,
  IN UINT32    PatternWidth
  );

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
  );

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
  );

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
  );

/**

  This function is used to set CPGC request data size for transaction

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

/**

  Set Lfsr Polynomial for Data Pattern

  @param[in] Host                - Pointer to sysHost
  @param[in] Socket              - Socket number
  @param[in] ChbitMask           - Ch Bit mask for which test should be setup for.
  @param[in] Poly0               - LFSR polynomial 0
  @param[in] Poly1               - LFSR polynomial 1
  @param[in] Poly2               - LFSR polynomial 2

  @retval N/A

**/
VOID
SetLfsrPolynomial (
  IN PSYSHOST      Host,
  IN UINT8         Socket,
  IN UINT32        ChbitMask,
  IN UINT32        Poly0,
  IN UINT32        Poly1,
  IN UINT32        Poly2
  );

/**
  This function specifies the number of cache lines that dq lanes in WDB will be rotated

  @param[in] Host              - Pointer to sysHost
  @param[in] Socket            - Socket Id
  @param[in] ChbitMask         - Ch Bit mask for which test should be setup for.
  @param[in] RotateRate        - Specify the number of cache lines that dq lanes in WDB will be rotated

  @retval NA

**/
VOID
SetCpgcDqRotateRate (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChbitMask,
  IN UINT8     RotateRate
  );

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
  );

/**
  Get current Mpr training mode.

  @param[in] Socket                            - Socket Id
  @param[in] Ch                                - Channel number (0-based)
  @param[in] SubCh                             - Sub-channel number (0-based)

  @retval UINT8                                - Mpr Mode. 0 - off, 1 - on.

**/
UINT8
GetMprModeFilterActPre (
  IN UINT8       Socket,
  IN UINT8       Ch,
  IN UINT8       SubCh
  );

/**
  This function specifies DQ pattern loaded on WDB

  @param[in] Host              - Pointer to sysHost
  @param[in] Socket            - Socket Id
  @param[in] ChbitMask         - Ch Bit mask for which test should be setup for.
  @param[in] DQPat             - Specify DQ pattern

  @retval NA

**/
VOID
SetWdbDqPattern (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChbitMask,
  IN UINT8     DQPat
  );

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
  );

/**

  Programs the CPGC base col repeat register

  @param[in] Host                 - Include all MRC global data.
  @param[in] socket               - Memory Controller
  @param[in] ChbitMask            - ch Bit mak for which test should be setup for.
  @param[in] BaseColRepeatsValue  - Base Col Repeats Value

  @retval N/A
**/
VOID
ProgramBaseColRepeats (
  IN PSYSHOST      Host,
  IN UINT8         Socket,
  IN UINT32        ChbitMask,
  IN UINT32        BaseColRepeatsValue
  );

#endif
