/** @file
  MemHost.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#ifndef _memhost_h
#define _memhost_h

#include <Ppi/MemoryPolicyPpi.h>
#include <ReferenceCodeDataTypes.h>
#include "SysRegs.h"
#include "MemRegs.h"
#include <Memory/MemDefaults.h>
#include "NGNDimmPlatformCfgData.h"
#include "MrcCommonTypes.h"
#include "MemHostChipCommon.h"
#include <MemCommon.h>
#include <Memory/CpgcDefinitions.h>
#include <PiPei.h>
#include <Ppi/SsaBiosServicesPpi.h>
#include <Library/CheckpointLib.h>
#include <Library/SpdAccessLib.h>
#ifdef DDR5_SUPPORT
#include <Memory/Ddr5RcwRegs.h>
#include <Memory/Ddr5MrRegs.h>
#include <Memory/Ddr5DbRegs.h>
#endif // DDR5_SUPPORT

#define MMS_TABLE_TYPE_CR       0
#define MMS_TABLE_TYPE_ID       1
#define MMS_TABLE_TYPE_CD       2
#define MMS_TABLE_TYPE_DP       3
#define MMS_TABLE_TYPE_BW       4
#define MMS_TABLE_TYPE_INVALID  5
#define MMS_TABLE_TYPES         MMS_TABLE_TYPE_INVALID

typedef struct PMFTableInfo {
  UINT32  headerRevision;
  UINT32  headerOffset;
  UINT32  headerLength;
  UINT8   bodyRevision;
  UINT32  bodyOffset;
  UINT32  bodyLength;
} PMFTableInfo;

#define DB_DRAM_VREF_RANGE2_OFFSET  23

// Timing
#define SR_MODE                 0   // Same rank
#define DR_MODE                 1   // Different rank/same DIMM
#define DD_MODE                 2   // Different DIMM

typedef enum {
  t_cke,
  t_xp,
  t_odt_oe,
  t_xsdll
}WORK_AROUND_TYPE;

//
// MemEarlyRid
//
#define GNT2ERID_START  24
#define GNT2ERID_START_EMULATION  16
#define GNT2ERID_START_COEMULATION  (GNT2ERID_START_EMULATION + 2)

//
// WL Results Display Mode
//
#define DISPLAY_FROM_REGISTER      0
#define DISPLAY_FROM_CACHE         1

//
// Get Set Mode (GSM) defines
//
//
// Mode Parameters
//
#define GSM_READ_CSR      BIT0      // Set to force the read from hardware. Clear to read from cache.
#define GSM_READ_ONLY     BIT1      // Set to skip the write flow.
#define GSM_WRITE_OFFSET  BIT2      // Set to use the "value" input as an offset. Clear to use the "value" input as an absolute.
#define GSM_FORCE_WRITE   BIT3      // Set to force a write even if the data to be written matches the cached value.
#define GSM_UPDATE_CACHE  BIT4      // Set to update cached value with the read or write value.
#define GSM_COMBINE_EDGES BIT5      // Set to use value1 as left/low side edge and value2 as right/high side edge
#define GSM_SKIP_CMD_FUB  BIT6      //
                                    // Clear to use value1 as center and don't use value2
#define GSM_FLUSH_OFFSET  BIT7      // This affects only delays that are able to operate in multicast mode

typedef struct TrainingStepDoneStruct {
  UINT8 DramRxEq  : 1;
  UINT8 HostFlyBy : 1;
  UINT8 TxVref    : 1;
  UINT8 DqSwzDone : 1;
  UINT8 Reserved  : 4;
} TRAINING_STEP_DONE_STRUCT;

typedef struct {
  MRC_GT  group;    ///< Command group number
  INT16   value;    ///< Offset selected by mode GSM_WRITE_OFFSET
} GSM_CGVAL;

typedef struct {
  MRC_GT  group;    ///< Command group number
  INT16   le;       ///< Left edge
  INT16   re;       ///< Right edge
} GSM_CGEDGE;

typedef struct {
  GSM_CSN signal;   ///< Command signal name
  INT16   value;    ///< Offset selected by mode GSM_WRITE_OFFSET
} GSM_CSVAL;

typedef struct {
  GSM_CSN signal;   ///< Command signal name
  INT16   le;       ///< Left edge
  INT16   re;       ///< Right edge
} GSM_CSEDGE;



typedef struct {
  GSM_CSN signal;   ///< Command signal name
  INT16   le;       ///< Left edge
  INT16   re;       ///< Right edge
#ifdef DDR5_SUPPORT
  UINT16  EyeWidth;
  INT16   CenterPoint;
  UINT16  EyeOffset;
#endif
  UINT8   toSample;
} GSM_CSEDGE_CTL;

typedef struct {
  GSM_CSN signal;   ///< Command signal name
  INT16   le;       ///< Left edge
  INT16   re;       ///< Right edge
  UINT16  EyeWidth;
  UINT8   toSample;
  UINT8   Retrain;
} GSM_CSEDGE_CA;

typedef struct {
   INT16 left;
   INT16 right;
} SIGNAL_EYE;

typedef struct {
  UINT8    reqData;
  UINT16   freqValue;
} REQ_DATA_FREQ;

typedef struct {
  UINT16   freqValue;
  INT32    minTck;
} MIN_TCK;

typedef struct {
  MRC_GT  group;
  UINT16  minVal;
  UINT16  maxVal;
  UINT16  usDelay;
  CHAR8   *groupStr;
} GSM_LIMIT;

#define UNMARGINED_CMD_EDGE   511

#define CLEAR_MODE            0xFF
#define WR_LEVELING_CLEAR_MODE            0x53

//
// DRAM timing equates
//
#define tDLLK   512
#define ZQ_SHORT  0
#define ZQ_LONG   1
#define tZQOPER 300
#define tZQinit 512 // define the tZQinit as define in jedec spec
#define tMRD    8   // HSD 5370282

#define TRRD_S_DEFINE_DDR5 8

#ifdef DDR5_SUPPORT
#define tCCD_S_DEFINE 8
#else
#define tCCD_S_DEFINE 4
#endif

#define CADB_MUX_MAX  4

//
// MemTest definitions
//
#define MEMTEST 0
#define MEMINIT 1
#define WCRDT   32
#define RCRDT   22

//
// Advanced MemTest definitions
//
#define ADV_MT_TYPE_XMATS8      0
#define ADV_MT_TYPE_XMATS16     1
#define ADV_MT_TYPE_XMATS32     2
#define ADV_MT_TYPE_XMATS64     3
#define ADV_MT_TYPE_WCMATS8     4
#define ADV_MT_TYPE_WCMCH8      5
#define ADV_MT_TYPE_GNDB64      6
#define ADV_MT_TYPE_MARCHCM64   7
#define ADV_MT_TYPE_LTEST_SCRAM 8
#define ADV_MT_TYPE_LINIT_SCRAM 9
#define ADV_MT_TYPE_RANGE_TEST_SCRAM  10
#define ADV_MT_TYPE_TWR         11
#define ADV_MT_TYPE_DATA_RET    12
#define ADV_MT_TYPE_MATS8_TC1   13
#define ADV_MT_TYPE_MATS8_TC2   14
#define ADV_MT_TYPE_MATS8_TC3   15
#define ADV_MT_TYPE_PPR         0xFF

//
// advMemTestOptions
//
#define ADV_MT_XMATS8      (1 << ADV_MT_TYPE_XMATS8)
#define ADV_MT_XMATS16     (1 << ADV_MT_TYPE_XMATS16)
#define ADV_MT_XMATS32     (1 << ADV_MT_TYPE_XMATS32)
#define ADV_MT_XMATS64     (1 << ADV_MT_TYPE_XMATS64)
#define ADV_MT_WCMATS8     (1 << ADV_MT_TYPE_WCMATS8)
#define ADV_MT_WCMCH8      (1 << ADV_MT_TYPE_WCMCH8)
#define ADV_MT_GNDB64      (1 << ADV_MT_TYPE_GNDB64)
#define ADV_MT_MARCHCM64   (1 << ADV_MT_TYPE_MARCHCM64)
#define ADV_MT_LTEST_SCRAM (1 << ADV_MT_TYPE_LTEST_SCRAM)
#define ADV_MT_LINIT_SCRAM (1 << ADV_MT_TYPE_LINIT_SCRAM)
#define ADV_MT_RANGE_TEST_SCRAM     (1 << ADV_MT_TYPE_RANGE_TEST_SCRAM)

#define ADV_MT_TWR         (1 << ADV_MT_TYPE_TWR)
#define ADV_MT_DATA_RET    (1 << ADV_MT_TYPE_DATA_RET)
#define ADV_MT_MATS8_TC1  (1 << ADV_MT_TYPE_MATS8_TC1)
#define ADV_MT_MATS8_TC2  (1 << ADV_MT_TYPE_MATS8_TC2)
#define ADV_MT_MATS8_TC3  (1 << ADV_MT_TYPE_MATS8_TC3)
// Define the maximum number of day's between cold-boots
#define MEM_MAX_COLD_BOOT_DAYS      (90)  // Approximately 3 months

//Defined to enable error injection for developer unit testing of error handling features
//Example:FAULTY_PARTS_TRACKING_INJECT_ERROR if 1 == Error injection else 0 == No Error injection
//To enable error injection the below macro's should be defined to 1
#define FAULTY_PARTS_TRACKING_INJECT_ERROR    (0)
#define FAULTY_PARTS_TRACKING_INJECT_2ND_ERROR    (0)

#define FPT_OK                      (0x00)
#define FPT_REC_ENABLE_FAILED       (BIT0)
#define FPT_RD_DQ_DQS_FAILED        (BIT1)
#define FPT_WR_LEVELING_FAILED      (BIT2)
#define FPT_WR_FLY_BY_FAILED        (BIT3)
#define FPT_WR_DQ_DQS_FAILED        (BIT4)
#define FPT_MEM_BIST_FAILED         (BIT5)
#define FPT_LRDIMM_TRAINING_FAILED  (BIT6)
#define FPT_TRAINING_FAILED         (BIT7)

#define FPT_PI_READ_TYPE            (BIT0)
#define FPT_PI_WRITE_TYPE           (BIT1)
#define FPT_PI_LRDIMM_READ_TYPE     (BIT2)
#define FPT_PI_LRDIMM_WRITE_TYPE    (BIT3)
#define FPT_PI_LRDIMM_RD_MRD_TYPE   (BIT4)
#define FPT_PI_LRDIMM_WR_MRD_TYPE   (BIT5)
#define PI_CS_CLK_TYPE              (BIT6)
#define PI_WL_FINE_DDR5_TYPE        (BIT7)
#define PI_BCOM_CLK_TYPE            (BIT0|BIT1)
#define PRINT_ARRAY_TYPE_LRDIMM_WL_PHASE     2
#define PRINT_ARRAY_TYPE_LRDIMM_RCV_EN_PHASE 3
#define PRINT_ARRAY_TYPE_LRDIMM_MDQ_RD_DELAY 4
#define PRINT_ARRAY_TYPE_LRDIMM_MDQ_WR_DELAY 5
#define PRINT_ARRAY_TYPE_LRDIMM_RCV_EN_PHASE_DDR5 6

#define FPT_NO_ERROR                (0x00)
#define FPT_CORRECTABLE_ERROR       (BIT0)
#define FPT_NO_CORRECTABLE_ERROR    (BIT1)

#define MAX_BITS_IN_BYTE      8                     // BITS per byte
#define MAX_BITS_IN_DWORD     32                    // BITS per dword
#define BITS_PER_NIBBLE       4
#define MAX_NIBBLES_PER_BYTE  2
#define INDEX_NIBBLE0         0
#define INDEX_NIBBLE1         1

//
// DDR4 DB has 4 DFE coeff taps
//
#define DB_DFE_TAP                   4
#define NumofDBDfeTapEnable          2 // Enable Tap1 and Tap2 for DDR4 LRDIMM

//
// Command Clock
//

#define COMPOSITE_GROUPS      1

//
// Cross over Mode
//
#define XOVER_MODE_AUTO   0
#define XOVER_MODE_1TO1   2

#define CATCHALL_TIMEOUT    100000               // 100 ms

//
// Refresh Sync time (in ns)
//
#define REFRESH_SYNC_TIME  7800

//
// First Channel in imc group 2/3 per package level
//
#define FirstChanneliMc23  4

#pragma pack(push, 1)

extern const UINT16 rankSize[MAX_TECH];          ///< Rank size in 64 MB units
extern const UINT16 rankSizeDDR4[MAX_TECH];      ///< Rank size in 64 MB units
extern const UINT16 rankSizeDdrt[MAX_TECH_DDRT];  ///< Rank size in 64 MB units
//
// PDA execution during x16 devices operation
//
#define AUTO_PDA_MODE_X16     0     ///< Auto: will not modify PDA Mode
#define SAFE_PDA_MODE_X16     1     ///< Safe: will disable PDA operation when a x16 device is detected
#define DEFAULT_PDA_MODE_X16  AUTO_PDA_MODE_X16

//
// Features Equates
//
#define DOUBLE_REFI       BIT1
#define EXTENDED_TEMP     BIT2
#define X4_PRESENT        BIT3
#define X8_PRESENT        BIT4
#define X4_AND_X8_PRESENT BIT5
#define ASR_SUPPORT       BIT6
#define X16_PRESENT       BIT7

//
// Mode register definitions
//
#define MR0_B4       BIT1  // Burst 4
#define MR0_ONTHEFLY BIT0  // On-the-fly Burst
#define MR0_RBT      BIT3  // Read Burst Type
#define MR0_TM       BIT7  // Mode (normal, test)
#define MR0_DLL      BIT8  // DLL reset
#define MR0_PPD      BIT12 // Precharge Power Down
#define MR1_DLL      BIT0  // DLL Enable
#define MR1_RTT_NOM  (BIT1 | BIT2)  //Output Driver Impedance.
#define MR1_RX_EQ    (BIT13 | BIT6 | BIT5)  // DQ RX EQ
#define MR1_WLE      BIT7  // Write Leveling Enable
#define MR1_TDQS     BIT11 // TDQS Enable
#define MR1_QOFF     BIT12 // Output Buffer Enable
#define MR2_ASR      BIT6  // Auto Self Refresh
#define MR2_SRT      BIT7  // Self Refresh Temperature Range
#define MR2_ASR_DDR4 BIT6 | BIT7 //Auto Self Refresh
#define MR3_MPR      BIT2  // Multi Purpose Register
#define MR3_GD       BIT3  // Geardown mode
#define MR3_PDA      BIT4  // Per DRAM Addressability
#define MR3_TSR      BIT5  // Temperature Sensor Readout
#define MR3_FGRM     BIT6 | BIT7 | BIT8 // Fine Granularityy Refresh Mode
#define MR3_WCL      BIT9 | BIT10 // Write CMD Latency
#define MR4_RE       BIT10 // Read Preamble Training Mode
#define MR4_RP       BIT11 // Read Preamble
#define MR4_WP       BIT12 // Write Preamble

#define NO_PDA    0
#define ENTER_PDA 1
#define EXIT_PDA  2

#define ALL_DRAMS 0xFF  // Indicates to write to all DRAMs when in PDA mode
#define ALL_DATABUFFERS 0xF // Indicates to write to all Buffers in PBA Mode

///
/// Rand per channel information
//
typedef struct RankCh {
  UINT8             dimm;                             ///< DIMM this rank belongs to
  UINT8             rank;                             ///< Rank on the DIMM (0-3)
  UINT8             Roundtrip;
#ifdef DDR5_SUPPORT
  UINT8             IOLatency[SUB_CH];                ///< IOLatency[0] for SUB_CH_A; IOLatency[1] for SUB_CH_B
#else
  UINT8             IOLatency;
#endif // #ifdef DDR5_SUPPORT
} CHANNEL_RANKS;

//
// Workarounds Equates
//
#define WA_SCRAMBLER_DIS    BIT0        // Disable scrambler for this iMC
#define WA_NO_MORE_RCW      BIT1        // Disable any more WriteRC and/or WriteRCLrBuf


typedef enum {
INVALID_BUS,
SMBUS,
EMRS,
CPGC,
SAD,
} BUS_TYPE;

#define BITMAP_CH0_CH1_CH2  ( ( BIT0 ) | (BIT1 ) | (BIT2) )
#define BITMAP_CH0_CH1      ( ( BIT0 ) | (BIT1 ) )
#define BITMAP_CH1_CH2      ( ( BIT1 ) | (BIT2 ) )
#define BITMAP_CH0_CH2      ( ( BIT0 ) | (BIT2 ) )
#define BITMAP_CH0       BIT0
#define BITMAP_CH1       BIT1
#define BITMAP_CH2       BIT2

#define DDRT_CTRL_SIZE 2  //128MB of NVMCTLR CSR/Mailbox/BLK CTRL region per NVMDIMM_IF dimm (in 64MB granularity)
#define DDRT_CTRL_DPA_START 0
#define DDRT_VOL_DPA_START 4  //Volatile region starts above the control regions and BLK RW windows(which is 256MB in size including reserved fields)

#define MEM_MAP_LVL_NORMAL              0
#define MEM_MAP_LVL_IGNORE_CFGIN        1
#define MEM_MAP_LVL_IGNORE_CFGIN_IGNORE_NEW_DIMMS   2

#define MEM_MAP_VOL_MEM_MODE_UNDEFINED                 2

#define MEM_MAP_STATE_RESOURCE_CALCULATION             0
#define MEM_MAP_STATE_RESOURCE_CALCULATION_FAILURE     1
#define MEM_MAP_STATE_RESOURCE_CALCULATION_SUCCESS     2
#define MEM_MAP_STATE_PARTITION_FAILURE                3

//NVMDIMM training mode
#define DISABLE_TRAINING_MODE 0
#define ENABLE_TRAINING_MODE  1
#define DISABLE_TRAINING_STEP 2

//CL supported range
#define HIGH_CL_RANGE 1

#define WDB_END               7

#define NOT_NVDIMM                 0
#define JEDEC_NVDIMM_N             1

#define NVDIMM_SUCCESS 0
#define NVDIMM_ERROR 1
#define NVDIMM_NODATA 2
#define NVDIMM_RESTORE 3

#define STATUS_RESTORE_NEEDED      (1 << 0)
#define STATUS_RESTORE_INPROGRESS  (1 << 1)
#define STATUS_RESTORE_SUCCESSFUL  (1 << 2)
#define STATUS_ARMED               (1 << 3)

#define ERROR_DETECT               (1 << 8)
#define ERROR_RESTORE              (1 << 9)
#define ERROR_ARM                  (1 << 10)
#define ERROR_SAVE                 (1 << 11)

typedef struct DdrtPorDdrTable {
  UINT8   DimmType;
  UINT8   NumRanks;
  UINT8   DataWidth;
  UINT8   Density;
  UINT8   DieCount;
  UINT16  DimmSize;
  UINT8   Validated;
} DDRT_POR_DDR_TABLE;

typedef enum {
  NoDimm    = 0,
  Rdimm     = 1,
  Rdimm3ds  = 2,
  Lrdimm    = 3,
  Lrdimm3ds = 4,
  Ddrt      = 5
} DIMM_TYPE;

typedef struct DdrTypeMatrix {
  DIMM_TYPE Type;
  UINT16    Capacity;
  UINT16    NonVolCap;
} DDR_TYPE_MATRIX;

typedef struct DdrtImcTopology {
  UINT8           Modes;    // BIT0 - AD  BIT1 - MM
  UINT8           NumDdrt;
  UINT16          Dpc;
  DDR_TYPE_MATRIX DimmSlot[MAX_MC_CH][MAX_DIMM];
} DDRT_IMC_TOPOLOGY;

#define MEM_MODE_AD     BIT0
#define MEM_MODE_MM     BIT1

#define DRV_GAP            (1 * MAX_PHASE_IN_FINE_ADJUSTMENT)  // define DRV_GAP default value.
#define IO_DIFF            (1 * MAX_PHASE_IN_FINE_ADJUSTMENT)  // define the IO diff value.
#define DRV_GAP_RD_WR      (1 * MAX_PHASE_IN_FINE_ADJUSTMENT)  // define the DRV GAP read write value.
#define TA_PAD             0
#define LR_RANK_MULT_TA    2

#define PERCENTAGE_TOLERANCE 95
#define POWERTRAINING_DEFAULT_SETTING -1

#define MAX_SETTINGS              32
#define MAX_SETTINGS_PER_STROBE   MAX_SETTINGS
#ifdef LRDIMM_SUPPORT
#define MAX_SETTINGS_PER_BIT      MAX_SETTINGS / 2
#else
#define MAX_SETTINGS_PER_BIT      4
#endif
#define MAX_GROUPS                2
#define MAX_PARAMS                4
#define AVERAGE_FILTER            0
#define POWER_TRENDLINE           1
#define MAX_MC_ODT                2
#define MAX_RTT_PARK              8
#define MAX_RTT_NOM               8
#define PXC_DISABLE               0
#define PXC_ENABLE                1

#define DONT_CHECK_MAPOUT 0
#define CHECK_MAPOUT      BIT0

typedef union {
struct  {
   UINT32 channelindex:3;
   UINT32 rsvd_14_3:12;
   UINT32 selector:1;
   UINT32 cmdval:16;
}Bits;
UINT32 Data;
}CMD_WRITE_PWR_BW_SCALE_DATA;

typedef union {
struct  {
   UINT32 type   : 1;
   UINT32 scale  : 3;
   UINT32 rsvd_27:28;
}Bits;
UINT32 Data;
}MEMORY_PM_SCALE_DATA;

typedef union {
struct  {
   UINT32 rankspresent:16;
   UINT32 rsvd_8:8;
   UINT32 channelindex:8;
}Bits;
UINT32 Data;
}CMD_DDR_RANKS_PRESENT;

typedef union {
struct  {
   UINT32 dimmpresent:16;
   UINT32 rsvd_8:8;
   UINT32 channelindex:8;
}Bits;
UINT32 Data;
}CMD_DDR_DIMM_PRESENT;

typedef union {
struct  {
   UINT32 Countertype:1;
   UINT32 counterbitvector:16;
   UINT32 rsvd_15:15;
}Bits;
UINT32 Data;
}CMD_DRAM_PM_RESET_CTR;

typedef union {
struct  {
   UINT32 Countertype:1;
   UINT32 counterbitvector:16;
   UINT32 rsvd_15:15;
}Bits;
UINT32 Data;
}CMD_DRAM_PM_START_CTR;

typedef union {
struct  {
   UINT32 Countertype:1;
   UINT32 counterbitvector:16;
   UINT32 rsvd_15:15;
}Bits;
UINT32 Data;
}CMD_DRAM_PM_STOP_CTR;

typedef union {
struct  {
   UINT32 Countertype:1;
   UINT32 counternumber2read:4;
   UINT32 rsvd_27:27;
}Bits;
UINT32 Data;
}CMD_DRAM_PM_READ_CTR;

typedef union {
struct  {
   UINT32 raplmode:2;
   UINT32 rsvd_30:30;
}Bits;
UINT32 Data;
}CMD_DRAM_PM_RAPL_MODE;

typedef union {
struct  {
   UINT32 vr0chmask:4;
   UINT32 vr1chmask:4;
   UINT32 rsvd_24:24;
}Bits;
UINT32 Data;
}CMD_DRAM_PM_CH_VR_MAP;

typedef struct  {
  UINT8  RAPL3SigmaPolicy;      ///< BIT1 = Page Policy (1=close Page / 0=open page), BIT0 = CKE (1=slow / 0=fast)
  UINT16 RAPLRanksPresentData[MAX_CH];    ///<  15:0= ranks present
  UINT16 RAPLDRAMPmWrPwrCOEFFValConst[0xF];
  UINT16 RAPLDRAMPmWrPwrCOEFFValPower[0xF];
  UINT8  RAPLMode;        ///< 0 = Estimated, 1=VR Measured
  UINT8  RAPLDramAdjVal;
  UINT16 RAPLDramPwrInfoMaxVal;
  UINT16 RAPLDramPwrInfoTdpVal;
  UINT16 RAPLDramPwrInfoMinVal;
  UINT16 RAPLWritePwrScaleSval[0xF];
  UINT16 RAPLWritePwrScaleMval;
  UINT8  RAPLDdrEnergyScaleFactor;
}DramRaplDataStruct;

typedef struct {
  UINT16 DramRaplDataDramPmWritePowerCoeffCval;
  UINT16 DramRaplDataDramMaxPwr;
  UINT16 DramRaplDataDramTdp;
  UINT16 DramRaplDataDramMinPwrClosedPage;
  UINT16 DramRaplDataDramWritePwrScaleClosedPage;
  UINT16 DramRaplDataDramMaxPwrOpenPage;
  UINT16 DramRaplDataDramTdpOpenPage;
  UINT16 DramRaplDataDramMinPwrOpenPage;
  UINT16 DramRaplDataDramWritePwrScaleOpenPage;
  UINT32 DramRaplDataDramRefreshRateSlope;
  UINT16 DramRaplDataDramSelfRefreshPower;
}DramRaplLUTDataStruct;

#pragma pack(pop)

#define RESTORE_SCALING 0
#define SAVE_SCALING    1

///
/// (MPT_MT - MemeoryPowerTraining_MarginType)param type for power training steps
///
typedef enum {
  GetMargin,
  TerMargin,
  BerMargin,
  MarginMax
} MPT_MT;

///
/// (MPT_PT - MemoryPowerTraining_ParamType)param type for power training steps
///
typedef enum {
  PerMc,
  PerCh,
  PerDimmPerCh,
  PerBytePerCh,
  PerNibblePerCh,
  PerRankPerCh,
  PerBitPerRankPerCh,
  PerNibblePerRankPerCh,
  PerBytePerRankPerCh,
  PtMax
} MPT_PT;

typedef union {
  INT16               PerStrobeGroupSetting[MAX_GROUPS][MAX_SETTINGS_PER_STROBE];
  INT16               PerBitGroupSetting[MAX_GROUPS][MAX_SETTINGS_PER_BIT];
} PER_GROUP_SETTINGS;

typedef union {
  INT16               PerStrobe[MAX_CH][MAX_RANK_CH][MAX_STROBE][MAX_GROUPS][MAX_SETTINGS_PER_STROBE];
  INT16               PerBit[MAX_CH][MAX_RANK_CH][MAX_BITS][MAX_GROUPS][MAX_SETTINGS_PER_BIT];
} AVERAGE_MARGINS;

typedef union {
  struct bitMargin    BitMargins;
  struct strobeMargin StrobeMargins;
} MARGIN_RESULTS;

typedef union {
  INT16               PerStrobe[MAX_CH][MAX_RANK_CH][MAX_STROBE][MAX_SETTINGS_PER_STROBE];
  INT16               PerBit[MAX_CH][MAX_RANK_CH][MAX_BITS][MAX_SETTINGS_PER_BIT];
} POWER_TREND_LINE;

typedef struct {
  MRC_GT            ParamGsm;                   // Parameter enumerated name - e.g. TxTco, TxVref, etc.
  CHAR8             *Name;                      // Parameter string used for debug prints
  MPT_PT            ParamType;                  // Parameter type
  UINT8             NumSettings;                // Number of settings for this parameter
  INT16             SettingList[MAX_SETTINGS];  // List of parameter settings
  UINT16            Mode;                       // Margining mode - e.g. MODE_VIC_AGG
  UINT32            PatternLength;              // Margining pattern length
  MPT_MT            MarginType;                 // Type of margin
} POWER_TRAIN_PARAM;

typedef struct {
  MRC_GT            ParamGsm;                                                               // Parameter name
  MRC_STATUS        (*GetSettings) (PSYSHOST Host, UINT8 Socket, POWER_TRAIN_PARAM *Param); // The GetSettings function pointer
} POWER_PARAM_SETTING_GETTER;

#define IMC0 0
#define IMC1 1

//
// PPR Types
//
typedef enum {
  PprTypeSoft    = 0,
  PprTypeHard    = 1
} PPR_TYPE;

//
// PPR Status
//
#define PPR_STS_SUCCESS         0x00
#define PPR_STS_ADDR_VALID      0x01
#define PPR_STS_FAILED          0x02

//
// PPR Guard Keys
//
#define PPR_GUARD_KEY0  0xCFF
#define PPR_GUARD_KEY1  0x7FF
#define PPR_GUARD_KEY2  0xBFF
#define PPR_GUARD_KEY3  0x3FF

#define DRAM_UNKNOWN    0xFF

#define READ_PREAMBLE             BIT0
#define WRITE_PREAMBLE            BIT1
#define READ_OR_WRITE_PREAMBLE    (READ_PREAMBLE | WRITE_PREAMBLE)

#define READ_POSTAMBLE            BIT0
#define WRITE_POSTAMBLE           BIT1
#define READ_OR_WRITE_POSTAMBLE   (READ_POSTAMBLE | WRITE_POSTAMBLE)

#pragma pack(push, 1)
//
// -----------------------------------------------------------------------------
///
/// Common DRAM timings
///
struct comTime {
  UINT8   nCL;
  UINT8   nWR;
  UINT8   nRCD;
  UINT8   nAL;
  UINT8   nRP;
  UINT8   nRC;
  UINT8   nWTR;
  UINT8   nWTR_L;
  UINT8   nRAS;
  UINT8   nRTP;
  UINT8   nFAW;
  UINT8   nRRD;
  UINT8   nRRD_L;
  UINT8   nWL;
  UINT16  nRFC;
  UINT32  tCL;
  UINT16  tRCD;
  UINT16  tRP;
  UINT16  tRC;
  UINT16  tRFC;
  UINT16  tRRD;
  UINT16  tRRD_L;
  UINT16  tRAS;
  UINT16  tCCD;
  UINT16  tCCD_WR;
  UINT16  tCCD_L;
  UINT16  tCCD_WR_L;
  UINT64  casSup;
  UINT8   casSupRange;
}; //struct comTime

//
// Only applicable for x16 devices where 2 strobes are within a single DRAM device
//
typedef struct _X16_MAPPING {
  UINT8   PairLogical[MAX_STROBE/2];    ///< Identifies logical strobe within the same DRAM device
  BOOLEAN IsLow[MAX_STROBE/2];          ///< TRUE:  represents the current logical strobe is connected to DQSL
                                        ///< FALSE: represents the current logical strobe is connected to DQSU
} X16_MAPPING;

//
// Packed bit field structure for storing MR1 RX EQ bits
//
typedef struct {
  UINT32  Strobe0 : 3; //bits 0...2
  UINT32  Strobe1 : 3; //bits 3...5
  UINT32  Strobe2 : 3; //bits 6...8
  UINT32  Strobe3 : 3; //bits 9...11
  UINT32  Strobe4 : 3; //bits 12..14
  UINT32  Strobe5 : 3; //bits 15..17
  UINT32  Strobe6 : 3; //bits 18..20
  UINT32  Strobe7 : 3; //bits 21..23
  UINT32  Strobe8 : 3; //bits 24..26
  UINT32  Rsvd    : 5; //bits 27..31
} Mr1RxEqStruct;

//
// NVRAM structures for S3 state
//

///
/// DIMM rank info
/// List indexed by rank number
///
typedef struct ddrRank {
  UINT8   enabled;                    ///< 0 = disabled,  1 = enabled
  UINT8   rankIndex;
  UINT8   phyRank;                    ///< Physical Rank #(3:0)
  UINT8   logicalRank;                ///< Logical Rank number (0 - 7)
  UINT8   ckIndex;                    ///< Index to the clock for this rank
  UINT8   ctlIndex;                   ///< Index to the control group for this rank
  UINT8   CKEIndex;
  UINT8   ODTIndex;
  UINT8   CSIndex;
  UINT8   devTagInfo;
  UINT16  RttWr;
  UINT16  RttPrk;
#ifdef DDR5_SUPPORT
  UINT16  RttNomRd;
  UINT16  RttNomWr;
  UINT16  DqsRttPrk;
#else // DDR4 RttNom
  UINT16  RttNom;
#endif // DDR RttNom
  UINT16  rankSize;                   ///< Units of 64 MB
  UINT16  remSize;                    ///< Units of 64 MB
  UINT16  NVrankSize;                   ///< Units of 64 MB

  UINT8   cmdLeft;
  UINT8   cmdRight;
  UINT8   cmdLow;
  UINT8   cmdHigh;

  UINT8   rxDQLeftSt;
  UINT8   rxDQRightSt;
  UINT8   txDQLeftSt;
  UINT8   txDQRightSt;
  UINT16  OutDrvImpCtrl;
  UINT8   DqSwzDdr5[SUB_CH][20];
#ifdef DDR5_SUPPORT
  UINT8   DeDqSwzDdr5[SUB_CH][MAX_BITS_DDR5/SUB_CH]; //store the dq deswizzle result
#endif // DDR5_SUPPORT
  UINT8   faultyParts[MAX_STROBE];
#ifdef LRDIMM_SUPPORT
  UINT8   lrbufRxVref[MAX_STROBE];      ///< Actual value of backside RxVref
  UINT8   lrBuf_FxBC4x5x[MAX_STROBE];
  UINT8   lrBuf_FxBC8x9x[MAX_STROBE];
#if defined (SPR_HOST) || defined (DDR5_SUPPORT) // Define SPR_HOST is to avoid Ktisim had dependency of MRC code when enable DDR5_SUPPORT flag
  UINT8   lrBuf_RWE2E3[MAX_STROBE];
  UINT8   LrBuf_PG01RWE4E5[MAX_STROBE]; // DDR5 MRD Read
  UINT8   LrBuf_PG01RWE6E7[MAX_STROBE]; // DDR5 MDQ Write
#endif
#endif
  UINT8   lrBuf_FxBC2x3x[MAX_STROBE];
  UINT8   lrBuf_FxBCAxBx[MAX_STROBE];
  UINT8   lrBuf_FxBCCxEx[MAX_STROBE/2];
  UINT8   lrBuf_FxBCDxFx[MAX_STROBE/2];
  UINT32  lrbufTxVref[MAX_STROBE];      ///< Actual value of backside TxVref
  //
  // MR1 per strobe Rx Eq value structure with 9 elements per structure
  // therefore you need MAX_STROBE/9 structures to store all elements
  //
  Mr1RxEqStruct Mr1RxEqSettings[MAX_STROBE/9];

} DDR_RANK_STRUCT;  //struct ddrRank

typedef enum {
  DIMM_RANK_MAP_OUT_UNKNOWN = 0,
  DIMM_RANK_MAP_OUT_MEM_DECODE,
  DIMM_RANK_MAP_OUT_POP_POR_VIOLATION,
  DIMM_RANK_MAP_OUT_RANK_DISABLED,
  DIMM_RANK_MAP_OUT_MAX
} DIMM_RANK_MAP_OUT_REASON;

///
/// DIMM information stored in NVRAM
//
typedef struct dimmNvram {
  UINT8                         dimmPresent;        ///< 1 = DIMM present in this slot
  UINT8                         mailboxReady;
  UINT16                        lrbufswizzle;
  INT32                         minTCK;             ///< minimum tCK for this DIMM (SPD_MIN_TCK)
  UINT8                         ftbTCK;             ///< fine offset for tCK
  UINT32                        tCL;
  UINT16                        tRCD;
  UINT16                        tRP;
  UINT8                         ftbTRP;             ///< DDR4 fine offset for tRP
  UINT8                         ftbTRCD;            ///< DDR4 fine offset for tRCD
  UINT8                         ftbTAA;             ///< DDR4 fine offset for tAA
  UINT8                         mapOut[MAX_RANK_DIMM]; ///< 1 = User requested rank made non-Present
  UINT8                         numRanks;           ///< Number of logical ranks on this DIMM
  UINT8                         numDramRanks;       ///< Number of physical DRAM ranks on this DIMM
  UINT8                         techIndex;          ///< Index into DIMM technology table
  UINT8                         aepTechIndex;       ///< Index into NVM DIMM technology table
  UINT8                         fmcType;            ///< Far Memory Controller Type
  UINT8                         fmcRev;             ///< Far Memory Controller Rev Type
  SPD_AEP_MOD_REVISION_STRUCT   FmcModRev; ///< Far Memory Controller Module Rev and Type
  UINT8                         NvmStepping;        ///< NVM Device Stepping
  UINT8                         NvmDevDensity;      ///< NVM DEV DENSITY
  UINT8                         NvmDevType;         ///< NVM DEV TYPE
  UINT8                         SPDRawCard;         ///< Raw Card Number
  UINT8                         DimmRevType;        ///< RCD Revision
  UINT8                         SPDOtherOptFeatures;///< SDRAM Other Optional features
  UINT8                         SPDAddrMapp;        ///< Address Mapping from Edge connector to DRAM
  UINT8                         SPDRegRev;          ///< Register Revision
  UINT8                         SPDODCtl;           ///< Register Output Drive Strength for Control
  UINT8                         SPDODCk;            ///< Register Output Drive Strength for Clock
#ifdef LRDIMM_SUPPORT
  UINT16                        SPDLrbufVen;          ///< LRDIMM Data Buffer Vendor ID
  UINT8                         SPDLrbufDramVrefdqR0; ///< DRAM VrefDQ for Package Rank 0
  UINT8                         SPDLrbufDramVrefdqR1; ///< DRAM VrefDQ for Package Rank 1
  UINT8                         SPDLrbufDramVrefdqR2; ///< DRAM VrefDQ for Package Rank 2
  UINT8                         SPDLrbufDramVrefdqR3; ///< DRAM VrefDQ for Package Rank 3
  UINT8                         SPDLrbufDbVrefdq; ///< LR Data Buffer VrefDQ for DRAM Interface
  UINT8                         SPDLrbufDbDsRttLe1866; ///< LR Data Buffer MDQ Drive Strength and RTT for data rate <= 1866
  UINT8                         SPDLrbufDbDsRttGt1866Le2400; ///< LR Data Buffer MDQ Drive Strength and RTT for data rate > 1866 and <= 2400
  UINT8                         SPDLrbufDbDsRttGt2400Le3200; ///< LR Data Buffer MDQ Drive Strength and RTT for data rate > 2400 and <= 3200
  UINT8                         SPDLrbufDramDs; ///< LR Buffer DRAM Drive Strength (for data rates <1866, 1866 < data rate < 2400, and 2400 < data rate < 3200)
  UINT8                         SPDLrbufDramOdtWrNomLe1866; ///< LR Buffer DRAM ODT (RTT_WR and RTT_NOM) for data rate <= 1866
  UINT8                         SPDLrbufDramOdtWrNomGt1866Le2400; ///< LR Buffer DRAM ODT (RTT_WR and RTT_NOM) for data rate > 1866 and <= 2400
  UINT8                         SPDLrbufDramOdtWrNomGt2400Le3200; ///< LR Buffer DRAM ODT (RTT_WR and RTT_NOM) for data rate > 2400 and <= 3200
  UINT8                         SPDLrbufDramOdtParkLe1866; ///< LR Buffer DRAM ODT (RTT_WR and RTT_NOM) for data rate <= 1866
  UINT8                         SPDLrbufDramOdtParkGt1866Le2400; ///< LR Buffer DRAM ODT (RTT_WR and RTT_NOM) for data rate > 1866 and <= 2400
  UINT8                         SPDLrbufDramOdtParkGt2400Le3200; ///< LR Buffer DRAM ODT (RTT_WR and RTT_NOM) for data rate > 2400 and <= 3200
  UINT8                         rcLrFunc;
#if defined (DDR5_SUPPORT)
  DDR5_DATA_BUFFER_RW86_STRUCT  DbRw86;
  DDR5_DATA_BUFFER_RW87_STRUCT  DbRw87;
  DDR5_DATA_BUFFER_RW88_STRUCT  DbRw88;
#else // DDR5_SUPPORT
  UINT8                         lrBuf_BC00;
  UINT8                         lrBuf_BC01;
  UINT8                         lrBuf_BC02;
  UINT8                         lrBuf_BC03;
  UINT8                         lrBuf_BC04;
  UINT8                         lrBuf_BC05;
  UINT8                         lrBuf_BC1x;
  UINT8                         LrBuf_DFECoef[MAX_BITS_IN_BYTE][DB_DFE_TAP][MAX_STROBE/2];  // JEDEC F3BCCx-Fx coeffcient. 8 DQ x 4 taps x 9 DB
  UINT8                         Dfe_F2BCEX; // JEDEC DB02DFE DQ selection and global enable
#endif    // DDR5_SUPPORT
#endif    // LRDIMM_SUPPORT

  UINT8                         XMPOrg;             ///< XMP organization and configuration
  UINT8                         XMPRev;
  UINT32                        XMPChecksum;

  UINT8                         x4Present;          ///< Set if this is a x4 rank
  UINT8                         keyByte;
  UINT8                         keyByte2;           ///< Logical module type (for MRC compat)
  UINT8                         DcpmmPresent;       ///< Is an NVM DIMM present?
  BOOLEAN                       NvmDimmDisable;     ///< 1 = NVMDimm present but got disabled in this slot
  UINT8                         actKeyByte2;        ///< Actual module type reported by SPD
  UINT8                         SPDModuleOrg;       ///< Logical number of DRAM ranks and device width
  UINT8                         actSPDModuleOrg;    ///< Actual number of DRAM ranks and device width
  UINT8                         dramIOWidth;        ///< The encoded value of DRAM IO width
  UINT8                         dramIOWidthSecondary;    ///< The encoded value of IO width for secondary SDRAM
  BOOLEAN                       rankMix;            ///< Used to indicate if the target DIMM is asymmetric
  UINT8                         SPDDeviceType;      ///< Primary SDRAM Device Type
  UINT8                         SPDSecondaryDeviceType; ///< Secondary SDRAM Device Type
  UINT8                         numBankAddressBits; ///< Number of bank address bits per bank group
  UINT8                         numBankAddressBitsSecondary; ///< Number of bank address bits per bank group of Secondary SDRAM
  UINT8                         numBankGroupBits;   ///< Number of bank group bits
  UINT8                         numBankGroupBitsSecondary; ///< Number of bank group bits of Secondary SDRAM
  UINT8                         sdramCapacity;      ///< Encoded SDRAM capacity based on DDR4 format
  UINT8                         sdramCapacitySecondary;      ///< Encoded SDRAM capacity based on DDR4 format for Secondary SDRAM
  UINT8                         numRowBits;         ///< Number of row address bits
  UINT8                         numRowBitsSecondary;    ///< Number of row address bits of Secondary SDRAM
  UINT8                         numColBits;         ///< Number of column address bits
  UINT8                         numColBitsSecondary;    ///< Number of column address bits of Secondary SDRAM
  UINT8                         dieCount;           ///< Number of SDRAM dies per package for symmetric DIMMs or Primary SDRAM dies per package for asymmetric DIMMs
  UINT8                         dieCountSecondary;  ///< Number of Secondary DRAM dies per package for asymmetric DIMMs
  UINT8                         cidBitMap;          ///< SubRanks per chip select per dimm of DDR4 3DS and non3ds_lrdimm
  UINT8                         lrRankMult;         ///< Rank multiplication factor
  UINT8                         SPDMemBusWidth;     ///< Width of the Primary bus and extension
  UINT8                         dimmTs;             ///< Module Thermal Sensor
  UINT8                         dimmAttrib;         ///< Module attributes
  UINT8                         mtbDiv;             ///< medium time base divisor (SPD_MTB_DIV)
  UINT8                         SPDftb;             ///< fine time base (SPD_FTB)
  UINT32                        dimmMemTech;        ///< Used to program DIMMMTR
  UINT16                        SPDRegVen;          ///< Register Vendor ID
  UINT16                        SPDMMfgId;          ///< Module Mfg Id from SPD
  UINT8                         SPDMMfLoc;          ///< Module Mfg Location from SPD
  UINT16                        SPDModDate;         ///< Module Manufacturing Date
  UINT16                        SPDDramMfgId;       ///< DRAM Mfg Id
  UINT8                         SPDDramRev;         ///< DRAM Rev Id
  UINT8                         SPDModSN[SPD_MODULE_SERIAL];               ///< Module Serial Number
#ifdef DDR5_SUPPORT
  UINT8                         SPDModPartDDR5[SPD_MODULE_PART_DDR5];      ///< Module Part Number DDR5
#endif // DDR5_SUPPORT
  UINT8                         SPDModPartDDR4[SPD_MODULE_PART_DDR4];      ///< Module Part Number DDR4
  UINT8                         SPDOptionalFeature; ///< DDR4 optional feature
  UINT8                         AitDramPoll;
  BOOLEAN                       NvDimmNPresent;     ///< JEDEC NVDIMM-N Type Memory Present
  UINT16                        NvDimmStatus;
  // Bit            Description
  //  0             NVDIMM controller failure
  //  1             NVDIMM restore failed
  //  2             NVDIMM restore retry
  //  3             NVDIMM backup failed
  //  4             NVDIMM erase failed
  //  5             NVDIMM erase retry
  //  6             NVDIMM arm failed
  //  7             No backup energy source detected
  //  8             Backup energy source charge failed
  //  9             NVDIMM uncorrectable memory error
  // 10             NVDIMM correctable memory memory error threshold
  // 11-15          Reserved

  UINT8                         lrbufRid;           ///< LR Buffer Revision ID (from buffer not SPD)
  UINT8                         lrbufGen;           ///< LR Buffer Gen
  UINT8                         SPDIntelSN[5];      ///< Intel DIMM serial number
  struct ddrRank                rankList[MAX_RANK_DIMM];
  UINT8                         SPDmtb;             ///< DDR4 medium time base (SPD_TB_DDR4)
  UINT8                         SPDSpecRev;         ///< Revision of the SPD spec for this data
  UINT8                         SpdModuleType;      ///< SPD Byte 2 (0x002): Key Byte / Host Bus Command Protocol Type
  UINT8                         rcCache[16];        /// DDR4 RCD cache
  UINT8                         rcxCache[16];
  UINT8                         lrDimmPresent;
  UINT8                         rcClk;
  UINT8                         rcCmd;
  UINT8                         rcCtl;
  UINT16                        rcVref;
  INT16                         QxCAClkOffset;      ///< RCD QxCA Training Offset
  UINT8                         dqSwz[36];
  UINT16                        rawCap;             ///< Raw Capacity
  UINT16                        VendorID;           ///< Vendor ID for NVMDIMM Mgmt driver thru Nfit
  UINT16                        DeviceID;           ///< Device ID for NVMDIMM Mgmt driver thru Nfit
  UINT16                        RevisionID;
  UINT16                        SubsystemVendorID;
  UINT16                        SubsystemDeviceID;
  UINT16                        SubsystemRevisionID;
  UINT8                         DimmGnt2Erid;       ///< DIMM-side Grant-to-ERID (produced by SPD and consumed by FMC; distinct from MC-programmed value)
  X16_MAPPING                   LogicalX16Mapping;
  UINT8                         manufacturer[NGN_MAX_MANUFACTURER_STRLEN];  /* Manufacturer */
  UINT8                         serialNumber[NGN_MAX_SERIALNUMBER_STRLEN];  /* Serial Number */
  UINT8                         PartNumber[NGN_MAX_PARTNUMBER_STRLEN]; /* Part Number */
  UINT16                        volCap;             /* Volatile Capacity (2LM region) */
  UINT16                        nonVolCap;          /* NonVolatile Capacity (PMEM/PMEM$ region + Blk region) */
  UINT16                        perCap;             /* Persistent Capcity (PMEM/PMEM$). This size is not obtained from FNV. This is derived data. */
  UINT16                        WbCachePerCap;       /* Persistent WB cache Capcity (AD-WB) This size is not obtained from FNV. This is derived data. */
  UINT32                        volRegionDPA;       /* DPA start address of 2LM Region */
  UINT32                        perRegionDPA;       /* DPA start address of PMEM Region */
  struct firmwareRev            firmwareRevision; /* Firmware Revision */
  UINT8                         dimmSku;            /* Dimm SKU info Bit0:MemorymodeEnabled 1: StoragemodeEnabled 2:AppDirectmodeEnabled 3:DieSparingCapable 4:SoftProgrammableSKU 5:MemorymodeEncryptionEnabled 6:AppDirectmodeEncryptionEnabled 7:StoragemodeEncrytionEnabled */
  UINT16                        InterfaceFormatCode;
  UINT16                        VendorIDIdentifyDIMM;
  UINT16                        DeviceIDIdentifyDIMM;
  UINT32                        DdrtDimmBasicTiming;
  UINT8                         Uniqueid[NGN_MAX_UID_STRLEN];  /* Unique ID */
  DIMM_RANK_MAP_OUT_REASON      MapOutReason[MAX_RANK_DIMM];
  UINT8                         VlpRdimmPresent;
#ifdef DDR5_SUPPORT
  UINT8                         McrDimmPresent;
#endif // DDR5_SUPPORT
} DIMM_NVRAM_STRUCT; //struct dimmNvram

#ifdef DDR5_SUPPORT
#define MAX_CMD_CSR 46
#else
#define MAX_CMD_CSR 16
#endif
#define MAX_SIDE    2

///
/// Channel information stored in NVRAM
///
typedef struct channelNvram {
  UINT8             enabled;            ///< 0 = channel disabled, 1 = channel enabled
  UINT8             mb3trainingfailure;
  UINT8             features;           ///< Bit mask of features to enable or disable
  UINT8             maxDimm;            ///< Number of DIMM
  UINT8             numRanks;           ///< Number of ranks on this channel
  UINT8             numQuadRanks;       ///< Number of QR DIMMs on this channel
  UINT8             timingMode;         ///< Command timing mode(1N, 2N, or 3N) for normal operation
  UINT8             trainTimingMode;    ///< Command timing mode(1N, 2N, or 3N) for training
  UINT8             ckeMask;            ///< CKE signals to assert during IOSAV mode
  UINT8             chFailed;           ///< ddr4 memory in this channel had failed (MFO)
  UINT8             ngnChFailed;        ///< NGN memory in this channel had failed (MFO)
  struct comTime    common;             ///< Common timings for this channel
  struct dimmNvram  dimmList[MAX_DIMM];
  struct RankCh     rankPerCh[MAX_RANK_CH];
  UINT32            dimmVrefControlFnv1;
  UINT32            dimmVrefControlFnv1Sa; // SA fub
  UINT32            ddrCrCmdPiCodingFnv;
  UINT32            ddrCrCmdPiCodingFnv2;
  UINT8             encodedCSMode;
  UINT8             cidBitMap;
  UINT8             txVrefSafe[MAX_RANK_CH];
#ifdef DDR5_SUPPORT
  INT8              DcaVrefRegValue[MAX_RANK_CH];
  INT8              DcsVrefRegValue[MAX_RANK_CH];
  UINT8             IbtClkRegValue[MAX_RANK_CH];
  UINT8             IbtCsRegValue[MAX_RANK_CH];
  UINT8             IbtCaRegValue[MAX_RANK_CH];
#endif //DDR5_SUPPORT
  UINT8             dimmRevType;
  UINT8             lrDimmPresent;
#ifdef LRDIMM_SUPPORT
  UINT8             chOneRankTimingModeEnable;
#endif
  UINT8             lrRankMultEnabled;
  UINT8             v110NotSupported;
  UINT8             v120NotSupported;
  UINT32            rankErrCountStatus;
  UINT32            rankErrCountInfo[(MAX_RANK_CH * SUB_CH)/2];
  UINT32            rankErrThresholdInfo[(MAX_RANK_CH * SUB_CH)/2];
  UINT8             ddrtEnabled;
  UINT32            SmiSpareCtlMcMainExt;
  UINT8             spareInUse;
  UINT8             spareDimm[MAX_RANK_CH/2];
  UINT8             spareRank[MAX_RANK_CH/2];
  UINT16            spareRankSize[MAX_RANK_CH/2];
  UINT8             spareLogicalRank[MAX_SPARE_RANK];
  UINT8             sparePhysicalRank[MAX_SPARE_RANK];
#ifdef DDR5_SUPPORT
  UINT32            ioLatency1[SUB_CH];  ///< ioLatency1[0] for SUB_CH_A; ioLatency1[1] for SUB_CH_B
#else
  UINT32            ioLatency1;
#endif // #ifdef DDR5_SUPPORT
  UINT32            tcrwp;              ///< TCRWP_MCDDC_CTL_STRUCT
  UINT32            tcothp;             ///< TCOTHP_MCDDC_CTL_STRUCT
  UINT32            tcothp2;
  UINT32            TCMr0Shadow;
  UINT32            TCMr2Shadow;
  UINT32            TCMr4Shadow;
  UINT32            TCMr5Shadow;
  INT16             normalizationFactor;
  UINT8             FmcWrCreditLimit;
  UINT8             FmcRdCreditLimit;
  UINT8             twoXRefreshSetPerCh;  /* Saves the refreshRate value for each channel */
  UINT8             tCCDAdder;
  BUS_TYPE          fnvAccessMode;
  UINT32            ddrtDimm0BasicTiming;
  UINT32            ddrtDimm1BasicTiming;
  UINT32            cadbMuxPattern[CADB_MUX_MAX];
  UINT8             EccModeMcMain[(MAX_RANK_CH * SUB_CH)];
  UINT32            Plus1Failover[(MAX_RANK_CH * SUB_CH)];
  UINT32            LinkRetryErrLimits;
  UINT32            LinkLinkFail;
  UINT32            mtCas2CasDr;                         /* CNX Change */
  UINT32            mtCas2CasDd;                         /* CNX Change */
  UINT32            mtCas2CasSr;                         /* CNX Change */
  UINT32            mtCas2CasSg;                         /* CNX Change */
  UINT32            mtCas2CasDs;                         /* CNX Change */
  UINT32            tCke;                                /* CNX Change */
  UINT32            MemoryTimingsAdj;
#ifndef DDRIO_DATA_1S
  UINT32            dataControl0;
  UINT32            dataControl1[MAX_STROBE];       ///< need to save for each strobe
  UINT32            dataControl2[MAX_STROBE];
  UINT32            dataControl4[MAX_STROBE];
  UINT32            dataControl3[MAX_STROBE];
  UINT32            dataOffsetComp[MAX_STROBE];
  UINT8             DrvStaticLegCfg[MAX_STROBE];
  UINT8             OdtSegmentEnable[MAX_STROBE];
  UINT8             DataSegmentEnable[MAX_STROBE];
  UINT8             RcompCode[MAX_STROBE];
  UINT32            LegacyRxDfeTxEcho[MAX_STROBE];   //Used to store Legacy RxDfe or TxEcho register values
  UINT32            ddrCRClkControls;
  UINT32            DdrCrCmdNControls;
  UINT32            DdrCrCmdSControls;
  UINT32            DdrCrCkeControls;
  UINT32            DdrCrCtlControls;
  UINT32            clkCsr;
  INT16             TxDqsDelay[MAX_RANK_CH][MAX_STROBE];
  UINT32            txGroup0[MAX_RANK_CH][MAX_STROBE];
  UINT32            txGroup1[MAX_RANK_CH][MAX_STROBE];
  UINT32            TxEqCoefTap2[MAX_RANK_CH][MAX_STROBE];
  UINT16            XtalkDeltaN0[MAX_RANK_CH][MAX_STROBE];
  UINT16            XtalkDeltaN1[MAX_RANK_CH][MAX_STROBE];
  UINT32            txTco[MAX_RANK_CH][MAX_STROBE];
  UINT32            rxGroup0[MAX_RANK_CH][MAX_STROBE];
  UINT32            rxGroup1[MAX_RANK_CH][MAX_STROBE];
  UINT32            RxDfeCoeff[MAX_STROBE];
  UINT32            RxDfeControl[MAX_STROBE];
  UINT32            rxOffset[MAX_RANK_CH][MAX_STROBE];
  UINT32            rxVrefCtrl[MAX_STROBE];
  UINT8             txVrefCache[MAX_RANK_CH][MAX_STROBE]; ///< Cached value of txVref (this might not be the programmed value)
  UINT8             txVref[MAX_RANK_CH][MAX_STROBE];      ///< Actual current value of txVref
  UINT32            ddrCRCmdTrainingCmdN;
  UINT32            ddrCRCmdTrainingCmdS;
  UINT32            ddrCRCtlTraining;
  UINT32            ddrCRCkeTraining;
  UINT32            ddrCRClkTraining;
  UINT32            ddrCRClkRanksUsed;
  UINT32            dataOffsetTrain[MAX_STROBE];
  UINT32            DataTrainFeedbackMultiCast;
  UINT32            ddrCRCmdControls3CmdN;
  UINT32            ddrCRCmdControls3CmdS;
  UINT32            ddrCRCmdControls3Ctl;
  UINT32            ddrCRCmdControls3Cke;
  UINT32            cmdCsr[MAX_CMD_CSR];
  UINT32            rxGroup1n[MAX_RANK_CH][MAX_STROBE];  /* CNX Change */
  UINT32            rxGroup1p[MAX_RANK_CH][MAX_STROBE];  /* CNX Change */
  UINT32            DdrCrintfDataTiming0;
  UINT32            TxDqBitClockDelta[MAX_RANK_CH][MAX_STROBE];  ///< Each unit represent 64 ticks
                                                                 ///< [7:0]   -> UIs for Bit 0
                                                                 ///< [15:8]  -> UIs for Bit 1
                                                                 ///< [23:16] -> UIs for Bit 2
                                                                 ///< [31:24] -> UIs for Bit 3
  UINT8             T_rrdr_org;   ///< Nvram Variable to store original tRRDR turnaround timing parameter
  UINT8             T_rrdd_org;   ///< Nvram Variable to store original tRRDD turnaround timing parameter
  UINT8             T_rrdr;   ///< Nvram Variable to store current tRRDR turnaround timing parameter
  UINT8             T_rrdd;   ///< Nvram Variable to store current tRRDD turnaround timing parameter
  UINT32            EnableRidUnderfillOrg;   ///< Nvram Variable to store original EnableRidUnderfill RDB entry
  UINT32            EnableRidVc2Org;         ///< Nvram Variable to store original EnableRidVc2 RDB entry
  UINT32            EnableRidVc3Org;         ///< Nvram Variable to store original EnableRidVC3 RDB entry
#endif // !DDRIO_DATA_1S
#ifdef DDR5_SUPPORT
  UINT8             McrDimmPresent;
#endif // DDR5_SUPPORT
} CHANNEL_NVRAM_STRUCT, *PCHANNEL_NVRAM_STRUCT;

///
/// IMC information stored in NVRAM
///
typedef struct imcNvram {
  UINT8               enabled;          ///< 0 = imc disabled, 1 = imc enabled
  UINT32              scrubMask;        ///< Scrub mask
  UINT32              scrubMask2;       ///< Scrub mask2
  UINT8               EmcaLtCtlMcMainExt;
  UINT32              ExRasConfigHaCfg;
  UINT32              SmiSpareCtlMcMainExt;
  UINT8               AppDirectHoleSize;
  UINT8               imcNodeId;               /*  System wide socket id for imc */
  BOOLEAN             LaneReversalEn;
#ifndef DDRIO_DATA_1S
  UINT32              ddrCRCompCtl0;
  UINT32              ddrCRCompCtl3; // This is need for silicon workaround 'S1409370801'
  UINT32              dimmVrefControl1;
#endif // !DDRIO_DATA_1S
} IMC_NVRAM_STRUCT; //struct imcNvram

///
/// Socket information stored in NVRAM
///
struct socketNvram {
  UINT8                     enabled;
  INT32                     minTCK;           ///< minimum tCK for this DIMM
  UINT8                     ddrFreq;          ///< DDR Frequency of this socket
  UINT16                    ddrFreqMHz;       ///< DDR Frequency of this socket in MHz
  UINT16                    QCLKps;           ///< Qclk period in pS
  UINT8                     cmdClkTrainingDone;
  UINT8                     ddrVoltage;       ///< Voltage of this socket
  UINT8                     lrDimmPresent;
  UINT8                     DcpmmPresent;
  BOOLEAN                   x16DimmPresent;
  UINT8                     maxDimmPop;       ///< Maximum number of DIMM populated on a channel for a socket
  UINT8                     wa;               ///< Bit field for workarounds
  UINT8                     ddr4SpdPageEn;
  struct channelNvram       channelList[MAX_CH];
  struct imcNvram           imc[MAX_IMC];
  UINT64_STRUCT             procPpin;         ///< Processor PPIN number
  UINT32                    smiCtrlUboxMisc;
  UINT8                     refreshRate;
  INT8                      normalizationFactorEn;
  UINT16                    WdbCacheValidPerChannel;  ///< Channel bitmask indicating whether the WDB cache is valid
  TRAINING_STEP_DONE_STRUCT TrainingStepDone;     ///< Indicators of whether a given training step is done
  UINT8                     ddrtFreq;
  UINT8                     cmdVrefTrainingDone;
  UINT8                     mcpPresent;
  UINT32                    FaultyPartsFlag[MAX_CH]; ///< Store faulty strobe info in a channel to NVRAM
  UINT32                    CsrWriteLatency[MAX_CH]; ///< CSR Write Latency from Core-Ubox-iMC[ch] roundtrip
  UINT32                    CsrReadLatency[MAX_CH];  ///< CSR Read Latency from Core-Ubox-iMC[ch] roundtrip
#ifdef DDR5_SUPPORT
  UINT8                     McrDimmPresent;
#endif // DDRT_SUPPORT
}; //struct socketNvram
typedef struct socketNvram  SOCKET_NVRAM;

#define HIGH_ADDR_EN        BIT0  // HIGH_ADDR_EN enables extention of the MMIO hole to force memory to high address region
#define DFX_PCOMMIT_DDR4    BIT1  // This is used to enable(1) and disable(0) PCOMMIT for DDR4 dimm config only
#define CR_MIXED_SKU        BIT2  //used to enable(1)- halt on mixed sku discovery and disable(0) - warn on mixed sku discovery
#define LOCK_NGN_CSR_DIS    BIT3  // Used to disable locking NVDIMM CSRs
#define SHORT_STROKE_INT    BIT4  //used to enable(1) - perform 2GB short stroke interleaved; disable(0) - do not perform 2GB short stroke
#define SHORT_STROKE_NO_INT BIT5  //used to enable(1) - perform 2GB short stroke non-interleaved; disable(0) - do not perform 2GB short stroke
#define FACTORY_RESET_CLEAR BIT7  // Used to disable Factory reset/clear MB command
#define CR_SKIP_ARS_ON_BOOT BIT8
#define CR_NO_BACKGROUND_ARS BIT9
#define CR_SEAMLESS_OPT_IN  BIT14
#define CR_CMD_FUB_MARGIN   BIT15
#ifdef CWV_A0_REMOVE_POST_PO
#define CR_SKIP_MB2_COMMAND BIT16 // CPS PON Use only
#endif

typedef struct memNvram {
  UINT8           DataGood;                   ///< Set to one if valid data is present in this structure
  UINT8           RASmode;                    ///< RAS mode (lockstep, mirror, sparing)
  UINT16          RASmodeEx;                  ///< Extended RAS mode (patrol scrub)
  UINT8           ratioIndex;                 ///< Index into the DDR3 ratio table
  UINT8           eccEn;                      ///< Set if ECC will be enabled
  UINT8           dimmTypePresent;            ///< Type of DIMMs populated (RDIMM,UDIMM,SODIMM)
  UINT8           DcpmmPresent;               ///< Is an DCPMM present in the system?
  UINT16          dramType;                   ///< DDR3 or DDR4 (from keybyte in SPD)
  UINT32          scrambleSeed;               ///< Data scrambling seed
  UINT32          socketBitMap;               ///< CPU present mask
  UINT8           ExtendedADDDCEn;
  struct memSetup savedSetupData;
  struct socketNvram socket[MAX_SOCKET];

  UINT8           XMPProfilesSup;
  UINT8           XMPProfilesRevision;
  struct          memTiming  profileMemTime[2];

  UINT16          Crc16;
  BOOLEAN         FmcCacheDone;               ///< Variable to indicate FMC Register caching is finished
  UINT8           threeDsModeDisabled;        ///< Variable to track if 3DS mode is enabled/disabled for CPGC
  UINT8           volMemMode;
  UINT8           CacheMemType;               ///< Only valid if volMemMode is 2LM
#ifdef DDRIO_DATA_1S
  UINT8           DdrioNvdata[DDRIO_DATA_1S * MAX_SOCKET];
#endif // DDRIO_DATA_1S
#ifdef DRAM_DATA_1S
  UINT8           DramNvdata[DRAM_DATA_1S * MAX_SOCKET];
#endif // DRAM_DATA_1S
#ifdef RCD_DATA_1S
  UINT8           RcdNvdata[RCD_DATA_1S * MAX_SOCKET];
#endif // RCD_DATA_1S
#ifdef LRDIMM_DB_DATA_1S
#if MAX_SOCKET <= 4
  UINT8           DbNvdata[LRDIMM_DB_DATA_1S * MAX_SOCKET];
#endif // MAX_SOCKET <= 4
#endif // LRDIMM_DB_DATA_1S

} MEM_NVRAM_STRUCT;

///
/// Error result struct used in read/write dq/dqs algo.
///
typedef struct {
  UINT8 DimmErrorResult[MAX_STROBE][MAX((MAX_PHASE_IN_FINE_ADJUSTMENT * 2), MAX_PHASE_IN_READ_ADJ_DQ_RX_DFE)];  ///< need to be good for read and write.
} TErrorResult, *PTErrorResult;
///
/// Error result struct used in read/write dq/dqs algo.
///
typedef struct {
  UINT8 BitErrorResult[MAX_BITS][MAX((MAX_PHASE_IN_FINE_ADJUSTMENT * 2), MAX_PHASE_IN_READ_ADJ_DQ_RX_DFE)];  ///< needs to be good for read and write.
} BErrorResult;

///
/// Per bit PI structure
///
typedef struct {
  UINT16 PiPosition[MAX_BITS];  ///< need to be good for read and write.
} PerBitPi;

//
// Max number for FMC cache register and structure
//
#define  MAX_FMC_CACHE    2

struct FmcCacheSt {
  UINT8   Status;  // Cache status
  UINT32  Reg;     // Register
  UINT32  Data;    // Data
}; // struct FmcCacheSt


///
/// Rank info
///
struct rankDevice {
#ifndef DDR5_SUPPORT
  UINT16  MR0;  ///< MR0 value for this rank
  UINT16  MR1;  ///< MR1 value for this rank
  UINT16  MR2;  ///< MR2 value for this rank
  UINT16  MR3;  ///< MR3 value for this rank
  UINT16  MR4;  ///< MR4 value for this rank
  UINT16  MR5;  ///< MR5 value for this rank
#endif // !DDR5_SUPPORT
  UINT16  MR6[MAX_STROBE];  ///< MR6 value for this rank/dram
#ifdef LRDIMM_SUPPORT
  UINT8   CurrentLrdimmTrainingMode;
  UINT8   CurrentDramMode;
  UINT16  CurrentMpr0Pattern;
  UINT8   lrbufRxVrefCache[MAX_STROBE];
  UINT8   cachedLrBuf_FxBC2x3x[MAX_STROBE];
  UINT8   cachedLrBuf_FxBC4x5x[MAX_STROBE];
  UINT8   cachedLrBuf_FxBC8x9x[MAX_STROBE];
  UINT8   cachedLrBuf_FxBCAxBx[MAX_STROBE];
  UINT8   cachedLrBuf_FxBCCxEx[MAX_STROBE/2];
  UINT8   cachedLrBuf_FxBCDxFx[MAX_STROBE/2];
#if defined (SPR_HOST) || defined (DDR5_SUPPORT)
  UINT8   cachedLrBuf_RWE2E3[MAX_STROBE];
  UINT8   cachedLrBuf_PG01RWE4E5[MAX_STROBE]; //DDR5 MRD Read
  UINT8   cachedLrBuf_PG01RWE6E7[MAX_STROBE]; //DDR5 MDQ Write
  INT8    cachedLrBuf_PG01RWE1[MAX_STROBE/2]; //DDR5 DWL External Coarse
  INT8    cachedLrBuf_PG01RWE8[MAX_STROBE/2]; //DDR5 DWL External Fine
  INT8    cachedLrBuf_PG01RWE9[MAX_STROBE/2]; //DDR5 DWL External Fine
  INT8    cachedLrBuf_PG01RWE1_IntFine[MAX_STROBE/2]; //DDR5 DWL Internal Fine
#endif
#endif
  UINT32  lrbufTxVrefCache[MAX_STROBE];
}; //struct rankDevice

///
/// DIMM info
/// List ordered by proximity to Host (far to near)
///
typedef struct dimmDevice {
  INT32               minTCK;     ///< minimum tCK for this DIMM (SPD_MIN_TCK)
#ifdef DEBUG_CODE_BLOCK
  UINT32              tCL;
  UINT16              tRCD;
  UINT16              tRP;
#endif  // DEBUG_CODE_BLOCK
  UINT16              NVmemSize;
  UINT16              memSize;    ///< Memory size for this DIMM (64MB granularity)
  UINT16              UnmappedMemSize;
  struct rankDevice   rankStruct[MAX_RANK_DIMM];
  struct FmcCacheSt   FmcCache[MAX_FMC_CACHE]; ///< FMC cache info/status
  UINT8               SPDPartitionRatio[MAX_SOCKET * MAX_IMC]; ///< NVM DIMM partitionRatios
  UINT8               CachedLrBuf_DFECoef[MAX_BITS_IN_BYTE][DB_DFE_TAP][MAX_STROBE/2];  // JEDEC F3BCCx-Fx coeffcient. 8 DQ x 4 taps x 9 DB
} DIMM_DEVICE_INFO_STRUCT; //struct dimmDevice

///
/// DDRT DIMM Mapping out reason
///
typedef enum _NVDIMM_UNMAPPED_REASON {
  NVDIMM_UNMAPPED_POPULATION_NOT_POR,
  NVDIMM_UNMAPPED_SKU_LIMIT_VIOLATION,
  NVDIMM_UNMAPPED_REASON_MAX
} NVDIMM_UNMAPPED_REASON;

///
/// DDRT DIMM info
///
typedef struct {
  UINT16                NgnLogSeqNum[NGN_LOG_TYPE_NUM][NGN_LOG_LEVEL_NUM];
  UINT16                NgnMaxLogEntries[NGN_LOG_TYPE_NUM][NGN_LOG_LEVEL_NUM];
  UINT8                 NvdimmLinkFailOnPrevBoot  : 1, /* Link failure was detected in this boot */
                        NvdimmMediaErrLogged      : 1, /* Media error log was detected in this boot */
                        NvdimmTempErrLogged       : 1, /* Fatal temperature error log was detected in this boot */
                        NvdimmUnmapped            : 1, /* NVDIMM is not to be mapped per memory population POR enforcement or SKU Limit violation. */
                        NvdimmUnmappedReason      : 1, /* Reason of NVDIMM is not to be mapped 0 - population POR enforcement, 1 - SKU Limit Violation */
                        NvdimmRemapped            : 1, /* NVDIMM is not to be unmapped per memory population POR enforcement. */
                        Reserved                  : 2;
  UINT64_STRUCT         NgnBsr;                        /* NGN NVDIMM Boot Status Register */
} HOST_DDRT_DIMM_DEVICE_INFO_STRUCT;

typedef struct {
  UINT8 WdbLine[MRC_WDB_LINE_SIZE];
} TWdbLine;

typedef struct {
  UINT32  address;
  UINT8   cid;
  UINT8   bank;
  UINT8   pdatag;
} PatCadbProg0;

typedef struct {
  UINT16  cs;
  UINT8   control;
  UINT8   odt;
  UINT8   cke;
  UINT8   par;
} PatCadbProg1;

typedef struct {
  UINT8   cs;
  UINT8   cid;
  UINT8   odt;
  UINT8   cke;
  UINT8   use_half_ca;
  UINT8   val;
  UINT8   par;
  UINT16  ca;
} PatCadb2Prog0;

typedef struct {
  UINT16  ca;
  UINT8   control; //(Bit[3]: ACT_n, Bit[2]: MA[16]/RAS_n, Bit[1]: MA[15]/CAS_n, Bit[0]: MA[14]/WE_n)
  UINT8   bank;
  UINT8   ca17; // for DDR4
} PatCadb2Prog1;
///
/// TT channel info
///
typedef struct ddrChannel {
  UINT8                             mcId;              ///<Memory controller number
  UINT8                             numDimmSlots;      ///<Number of DIMM slots per channel
  UINT32                            memSize;           ///<Memory size for this channel (64MB granularity)
  UINT32                            ddr4RemSize;       ///<Remaining DDR4 Memory size for this channel
  UINT32                            volSize;           ///<Volatile size of the NVM dimms in this channel
  UINT32                            NonVolSize;        ///<Non Volatile size of the NVM dimms in this channel
  UINT32                            volRemSize;        ///<Remaining Volatile size of the NVM dimms in this channel
  UINT32                            perSize;           ///<Pmem size of the NVM dimms in this channel
  UINT32                            perRemSize;        ///<Remaining Pmem size of the NVM dimms in this channel
  UINT32                            WbCachePerSize;    ///<Persistent WB cache (AD-WB) size of the NVM dimms in this channel
  UINT32                            WbCachePerRemSize; ///<Remaining Persistent WB cache (AD-WB) size of the NVM dimms in this channel
  UINT32                            ctrlSize;          ///<Ctrl region size of the NVM dimms in this channel
  UINT32                            ctrlRemSize;       ///<Remaining Ctrl region size of the NVM dimms in this channel
  UINT32                            NVmemSize;         ///< NVDIMM channel memory size
  UINT32                            NVmemRemSize;
  UINT32                            remSize;           ///< Size not yet mapped in units of 64 MB
  UINT32                            DdrCacheSize;      ///< Size of DDR memory reserved for 2LM cache (64MB granularity)
  DIMM_DEVICE_INFO_STRUCT           dimmList[MAX_DIMM];
  HOST_DDRT_DIMM_DEVICE_INFO_STRUCT DdrtDimmList[MAX_DDRT_DIMM_PER_CH];
  UINT8                             CsrUnlock;              ///< Whether FMC CSRs are unlocked (accessible/writable)
  UINT16                            FisVersion[MAX_DIMM];   ///< Firmwre Interface Specification version
} DDR_CHANNEL_STRUCT; //struct ddrChannel


///
/// List ordered by rank size
///
typedef struct rankSort {
  UINT8   socketNum;///< Node this rank is on
  UINT8   chNum;    ///< Channel this rank is on
  UINT8   dimmNum;  ///< DIMM this rank is on
  UINT8   rankNum;  ///< Index into rank list
  UINT8   rankID;   ///< Rank ID for this rank
  UINT16  rankSize; ///< Units of 64 MB
} RANK_SORT_STRUCT;

///
/// List ordered by socket size
///
struct socketSort {
  UINT8   socketNum;    ///< Socket number
  UINT8   haNum;        ///< HA number
  UINT8   haNodeId;     ///< HA socket id
  UINT16  socketSize;   ///< Units of 64 MB
};

typedef union {
  struct {
    UINT8 TrainingPatternMode : 3; // Bits[2:0] - Training Pattern Mode
    UINT8 QcaUcaTrainingDone : 1; // Bits[3] - QCA training or UDIMM CA training are done or not
    UINT8 BitIndex : 2; // Bits[5:4] - Used to indicate which bit to test.
    UINT8 CoarseRdDqDqsTrainingDone : 1; // Bits[6] - Coarse Read Dq/Dqs training are done or not
    UINT8 Reserved : 1; // Bits[7] - Reserved for future use.
  } Bits;
  UINT8 Data;
} TRAINING_STATUS_STRUCT;

///
/// List ordered by channel size
///
typedef struct channelSort {
  UINT8   chNum;          ///< Index into channel list
  UINT8   socketNum;      ///< Socket number
  UINT32  chSize;         ///< Units of 64 MB
} CHANNEL_SORT_STRUCT; // struct channelSort


#define MAX_FAIL_RANGE 128

typedef union {
  struct {
    UINT32 row : 18;        // This field is used for < > comparisons; more significant bits are used for match comparison only
    UINT32 bankPair : 4;    // This field covers worst case of DDR5 BG[1:0],BA[1:0], with BG[2] being the upper most BG bit for interleave
    UINT32 logicalSubRank : 3;
    UINT32 logicalRank : 3;
    UINT32 upperBgMask : 2; // bit-0 = 1 means failure with upper BG bit = 0; bit-1 = 1 means failure with upper BG = 1
    UINT32 rfu: 1;
    UINT32 valid : 1;
  } Bits;
  UINT32 Data;
} ROW_ADDR;

typedef struct {
  ROW_ADDR  addr;
  UINT32 size;
  UINT32 mask[3];
} ROW_FAIL_RANGE;

///
/// socket info
///
typedef struct Socket {
  UINT8                   SocketDieCount;           ///< Number of cpu dies present in the socket
  UINT32                  NumMemSs;
  MRC_MSM                 MemSsList[MAX_MEM_SS];
  BOOLEAN                 ImcPresent[MAX_IMC];      ///< on multi-die, some iMCs might not exist
  UINT8                   imcEnabled[MAX_IMC];
  UINT8                   fatalError;
  UINT8                   majorCode;
  UINT8                   minorCode;
  UINT8                   maxRankDimm;              ///< Maximum number or ranks supported per DIMM
  UINT32                  memSize;                  ///< DDR4 memory size for this socket (64MB granularity)
  TRAINING_STATUS_STRUCT  TrainingStatus;
  struct ddrChannel       channelList[MAX_CH];
  UINT8                   socketSubBootMode;
  UINT8                   hostRefreshStatus;
  UINT8                   firstJEDECDone;           ///< Flag to indicate the first JEDEC Init has executed
  UINT64_STRUCT           procPpin;
  UINT8                   cadbMRSMode;              ///< Mode of operation (LOAD / EXECUTE / NORMAL)
  UINT8                   cadbMRSIndex[MAX_CH];
  UINT32                  NVmemSize;                ///< Memory size for this node
  UINT32                  TotalInterleavedMemSize;  ///< DDR4 memory size for this socket (64MB granularity)
  UINT32                  TotalInterleavedNVMemSize;///< Actual NVMEM interleaved.
  UINT32                  volSize;                  ///< Volatile size of the NVM dimms for this socket (64MB granularity)
  UINT32                  perSize;                  ///< Persistent size of the NVM dimms for this socket (64MB granularity)
  UINT32                  WbCachePerSize;           ///< Persistent WB cache (AD-WB) size of the NVM dimms for this socket (64MB granularity)
  BOOLEAN                 TurnaroundInitDone;       ///< Turnaround Initialization Done
  MRC_TT                  CurrentTestType;          ///< Training step currently being executed by this socket
  //  SOCKET_HBM
  SOCKET_CHIP                                 ///< Chip hook to enable Socket fields
} SOCKET_INFO_STRUCT; // struct Socket

///
/// Sub-boot state internal to MRC (8-15 are definable).  The 2 main boot types and paths through KTI RC/MRC - NormalBoot and S3Resume.
/// Within NormalBoot and S3Resume, the sub-boot type can be cold, warm, fast warm, fast cold, and ADR resume.  These are populated
/// at the beginning of MRC so they are not applicable for KTI RC.
///
typedef enum SubBootMode
{
  ColdBoot     = 8,   // Normal path through MRC with full mem detection, init, training, etc.
  WarmBoot     = 9,   // Warm boot path through MRC. Some functionality can be skipped for speed.
  WarmBootFast = 10,  // Fast warm boot path uses the NVRAM structure to skip as much MRC
                            // code as possible to try to get through MRC fast. Should be as close
                            //    as possible to the S3 flow.
  ColdBootFast = 11,  // Fast cold boot path uses the NVRAM structure to skip as much MRC
                            //    code as possible on a cold boot.
  AdrResume    = 12,  // ADR flow can skip most of MRC (i.e. take the S3 path) for DIMMs that
                      //    are in self-refresh.  But the DIMMs that are not in self-refresh
                      //    must go through more of MRC.
  NvDimmResume = 13   // NvDimm flow is similar to Adr Batterybackup, but the DIMMs need
                      //    Rx & Mx registers initialized.
} SUB_BOOT_MODE;

//
// XMP Identification String
//
#define XMP_ID  0x0C4A

//
// Write Leveling Clean Up Ddr Technology
//
#define CWLC_DDR4   0
#define CWLC_DDRT   1

struct Interleaves {
  UINT8   ways;
  UINT16  channel[MAX_SOCKET * MAX_CH];
};

///
/// define the Training_Result_UP/DOWN CRs struct.
///
struct TrainingResults {
  UINT32  results[4];
};

#ifdef LRDIMM_SUPPORT
struct lrMrecTrainingResults {
  UINT8  results;
};
#endif // LRDIMM_SUPPORT

typedef struct {
  UINT32 TRrdSOrg;
  UINT32 TRrdLOrg;
  UINT32 TRcdRdOrg;
  UINT32 TRrsrOrg;
  UINT32 TRrSgOrg;
} ACT_TO_ACT_SAVE_STRUCT;

#define MEM_CHIP_POLICY_DEF(x)          Host->var.mem.memChipPolicy.x
#define MEM_CHIP_POLICY_VALUE(Host, x)  Host->var.mem.memChipPolicy.x

typedef struct {
  UINT32  TxPiSampleSimDelay;                                       // CHIP_TX_PI_SAMPLE_YAMDELAY
} MEM_CHIP_POLICY_SIM;

typedef struct {
  UINT8                     maxVrefSettings;                        // MAX_VREF_SETTINGS
  UINT8                     earlyVrefStepSize;                      // EARLY_VREF_STEP_SIZE
  INT16                     ctlPiGrp;                               // CTL_PI_GRP
  UINT8                     minIoLatency;                           // MIN_IO_LATENCY
  UINT16                    cas2DrvenMaxGap;                        // CAS2DRVEN_MAXGAP
  UINT8                     mrcRoundTripIoComp;                     // MRC_ROUND_TRIP_IO_COMPENSATION;
  UINT8                     mrcRoundTripIoCompStart;                // MRC_ROUND_TRIP_IO_COMP_START;
  UINT8                     mrcRoundTripMax;                        // MRC_ROUND_TRIP_MAX_VALUE;
  UINT32                    SrPbspCheckinCsr;                       // SR_PBSP_CHECKIN_CSR                       (BIOSNONSTICKYSCRATCHPAD2_UBOX_MISC_REG)           // UBOX scratchpad CSR02
  UINT32                    SrBiosSerialDebugCsr;                   // SR_BIOS_SERIAL_DEBUG_CSR(BIOSSCRATCHPAD6_UBOX_MISC_REG)                                      // UBOX scratchpad CSR6
  UINT32                    SrPostCodeCsr;                          // SR_POST_CODE_CSR         (BIOSNONSTICKYSCRATCHPAD7_UBOX_MISC_REG)                            // UBOX scratchpad CSR7
  UINT32                    SrErrorCodeCsr;                         // SR_ERROR_CODE_CSR        (BIOSNONSTICKYSCRATCHPAD8_UBOX_MISC_REG)                            // UBOX scratchpad CSR8
  UINT32                    SrMemoryDataStorageDispatchPipeCsr;     // #define SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR  (BIOSNONSTICKYSCRATCHPAD13_UBOX_MISC_REG)  // UBOX scratchpad CSR13
  UINT32                    SrMemoryDataStorageCommandPipeCsr;      // #define SR_MEMORY_DATA_STORAGE_COMMAND_PIPE_CSR   (BIOSNONSTICKYSCRATCHPAD14_UBOX_MISC_REG)  // UBOX scratchpad CSR14
  UINT32                    SrMemoryDataStorageDataPipeCsr;         // #define SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR      (BIOSNONSTICKYSCRATCHPAD15_UBOX_MISC_REG)  // UBOX scratchpad CSR15
  UINT32                    SrBdatStructPtrCsr;                     // SR_BDAT_STRUCT_PTR_CSR
  UINT32                    BiosStickyScratchPad0;                  // BIOSSCRATCHPAD0_UBOX_MISC_REG
  UINT8                     PerBitMarginDefault;                    // PER_BIT_MARGIN_DEFAULT
  UINT8                     RxOdtDefault;                           // RX_ODT_DEFAULT
  UINT8                     CmdTxEqDefault;                         // CMD_TX_EQ_DEFAULT
  UINT8                     RxDfeDefault;                           // RX_DFE_DEFAULT
  UINT8                     TxRiseFallSlewRateDefault;              // TX_RF_SLEW_RATE_DEFAULT
  UINT8                     RmtColdFastBootDefault;                 // RMT_COLD_FAST_BOOT_DEFAULT
  UINT8                     RxVrefTrainingMode;                     // RX_VREF_TRAINING_MODE
  UINT8                     TxVrefTrainingMode;                     // TX_VREF_TRAINING_MODE
  UINT16                    MaxPhaseInReadAdjustmentDq;             // MAX_PHASE_IN_READ_ADJUSTMENT_DQ
} MEM_CHIP_POLICY;

struct DimmDeviceTraining {
  UINT8   oneRankTimingModeLrbuf_FxBC2x3x[MAX_STROBE];
  UINT8   oneRankTimingModeLrbuf_FxBC4x5x[MAX_STROBE];
  UINT8   oneRankTimingModeLrbuf_FxBC8x9x[MAX_STROBE];
  UINT8   oneRankTimingModeLrbuf_FxBCAxBx[MAX_STROBE];
  UINT8   oneRankTimingModeLrbuf_FxBCCxEx[MAX_STROBE/2];
  UINT8   oneRankTimingModeLrbuf_FxBCDxFx[MAX_STROBE/2];
  UINT8   originalRank0Lrbuf_FxBC2x3x[MAX_RANK_DIMM][MAX_STROBE];
  UINT8   originalRank0Lrbuf_FxBC4x5x[MAX_RANK_DIMM][MAX_STROBE];
  UINT8   originalRank0Lrbuf_FxBC8x9x[MAX_RANK_DIMM][MAX_STROBE];
  UINT8   originalRank0Lrbuf_FxBCAxBx[MAX_RANK_DIMM][MAX_STROBE];
  UINT8   originalRank0Lrbuf_FxBCCxEx[MAX_RANK_DIMM][MAX_STROBE/2];
  UINT8   originalRank0Lrbuf_FxBCDxFx[MAX_RANK_DIMM][MAX_STROBE/2];
}; //struct DimmDeviceTraining

/// TT channel info
///
struct DdrChannelTraining {
  TWdbLine                  WdbLines[MRC_WDB_LINES];
  struct DimmDeviceTraining dimmList[MAX_DIMM];
}; //struct DdrChannelTraining

struct TrainingVariable {
  UINT8   rdVrefLo;
  UINT8   rdVrefHi;
  UINT8   wrVrefLo;
  UINT8   wrVrefHi;
  UINT8   cmdVrefLo;
  UINT8   cmdVrefHi;
  UINT8   DQPat;       ///< Global Variables storing the current DQPat REUT Test
  UINT8   DQPatLC;     ///< Global Variables storing the current DQPat Loopcount
  BOOLEAN EnDumRd;     ///< Enable/Disable Logic Analizer

  INT16   TxDqLeft[MAX_CH][MAX_RANK_CH][MAX_STROBE];
  INT16   TxDqRight[MAX_CH][MAX_RANK_CH][MAX_STROBE];
  INT16   TxVrefLow[MAX_CH][MAX_RANK_CH][MAX_STROBE];
  INT16   TxVrefHigh[MAX_CH][MAX_RANK_CH][MAX_STROBE];
  INT16   RxDqsLeft[MAX_CH][MAX_RANK_CH][MAX_STROBE];
  INT16   RxDqsRight[MAX_CH][MAX_RANK_CH][MAX_STROBE];
  INT16   RxVrefLow[MAX_CH][MAX_RANK_CH][MAX_STROBE];
  INT16   RxVrefHigh[MAX_CH][MAX_RANK_CH][MAX_STROBE];

  struct  DdrChannelTraining  channelList[MAX_CH];
#ifdef SPR_HOST
  UINT8   RxSamplerLo;
  UINT8   RxSamplerHi;
#endif //SPR_HOST
}; //struct TrainingVariable

///
/// Indicates how SPD data should be retrieved:
///   SpdSmbus         Data should be retrieved via SMBUS
///   SpdInternal      Data should be retrieved via internal SPD array
///   SpdInternalTrace Data should be retrieved via internal SPD array, but the SMBUS transactions should still be
///                      carried out to generate register traces for debugging
///
typedef enum {
  SpdSmbus,
  SpdInternal,
  SpdInternalTrace,
  SpdMax
} SPD_SOURCE;

//
// CMI Read and Write credit configuration register defaults
//
typedef struct {
  UINT32      CmiRdCreditConfigN0;
  UINT32      CmiRdCreditConfigN1;
  UINT32      CmiWrCreditConfigN0;
  UINT32      CmiWrCreditConfigN1;
  BOOLEAN     Valid;                /// Set to TRUE when structure is updated
} CMI_CREDIT_CONFIG_DEFAULT;

//
// Scheduler enables register defaults
//
typedef struct {
  UINT32      SchedulerEnables;
  UINT32      SchedulerEnablesBs;
  BOOLEAN     Valid;                /// Set to TRUE when structure is updated
} SCHEDULER_ENABLES_CONFIG_DEFAULT;

typedef struct {
  SCHEDULER_ENABLES_CONFIG_DEFAULT  SchedulerEnablesDefault[MAX_CH];
  CMI_CREDIT_CONFIG_DEFAULT         CmiCreditConfigDefault;
  UINT8                             RefreshModeDefault;
} MEM_TEST_CONFIG;

///
/// TT Host info
///
typedef struct memVar {
  UINT8                   currentSocket;    ///< Current socket being initialized
  UINT8                   PostCodeMinor;
  SUB_BOOT_MODE           subBootMode;      ///< WarmBoot, WarmBootFast, etc.
  UINT8                   wipeMemory;       ///< Write 0 to all memory to clean secrets
  UINT8                   skipMemoryInit;   ///< Skip memory init based on certain conditions.
  UINT8                   ddrFreqLimit;     ///< Set to limit frequency by the user
  UINT8                   chInter;          ///< Number of ways to interleave channels (1,2,3, or 4)
  UINT8                   callingTrngOffstCfgOnce; ///<to prevent looping inside RMT
  UINT8                   earlyCmdClkExecuted;
#ifdef DDR5_SUPPORT
  UINT8                   earlyCaClkExecuted;
#endif
  UINT8                   checkMappedOutRanks;
#ifdef DEBUG_CODE_BLOCK
  UINT8                   earlyCtlClkSerialDebugFlag;
#endif // DEBUG_CODE_BLOCK
  UINT32                  memSize;          ///< Total physical memory size
  UINT32                  NVmemSize;          ///< Total physical memory size
  UINT32                  TotalInterleavedMemSize;      ///< DDR4 memory size for this socket (64MB granularity)
  UINT32                  TotalInterleavedNVMemSize; /// < Actual NVMEM interleaved.
  UINT32                  QCLKPeriod;       ///< QCLK Period in pico seconds
  UINT32                  lowMemBase;       ///< Mem base in 64MB units for below 4GB mem.
  UINT32                  lowMemSize;       ///< Mem size in 64MB units for below 4GB mem.
  UINT32                  highMemBase;      ///< Mem base in 64MB units for above 4GB mem.
  UINT32                  highMemSize;      ///< Mem size in 64MB units for above 4GB mem.
  UINT32                  initialLFSRSeed;
  UINT32                  piSettingStopFlag[MAX_CH];
  UINT8                   pxcEnabled;
  struct Socket           socket[MAX_SOCKET];                        ///< Per socket structure
  struct TrainingVariable TrainingVar;       ///< Local variable for DDR training
  UINT32                  NumValidPprEntries;
  PPR_ADDR_MRC_SETUP      pprAddrSetup[MAX_PPR_ADDR_ENTRIES_SPPR];
  UINT8                   pprStatus[MAX_PPR_ADDR_ENTRIES_SPPR];
  UINT8                   softPprDone[MAX_SOCKET];
  ///
  /// Flag to skip RowTestPPR execution, so it wont return false error
  ///
  BOOLEAN                 SkipPprRowTest[MAX_SOCKET];
  //
  // Avanced Memtest Failure Range tracking
  //
  UINT8                   FailRangeInitDone[MAX_SOCKET];
  ROW_FAIL_RANGE          FailRange[MAX_CH][MAX_FAIL_RANGE];
  UINT32                  FailMax[MAX_CH];  // This is initialized to 0 prior to any memtest, indicating no failures
  INT32                   FailIndex[MAX_CH];
  UINT8                   RetryState;
  ROW_FAIL_RANGE          LastFail[MAX_CH];
  UINT8                   PprResourceAvailable[MAX_CH][MAX_DIMM * MAX_RANK_DIMM_3DS * MAX_SUBRANK_3DS][MAX_STROBE];  // Max logical ranks = 16 (2DPC, 3DS 2R-4H), each BG is bit-mapped into UINT8
  UINT64_STRUCT           AdvMemtestErrInjMask;
  UINT8                   InjectErrorMATS[MAX_CH];
  UINT8                   AmtErrInjDone;
  BOOLEAN                 IsDdrMemInitDone;    ///< Flag indicates if DDR memory init is done.

#ifdef LRDIMM_SUPPORT
  struct TrainingResults  lrTrainRes[MAX_CH][MAX_STROBE];
  struct lrMrecTrainingResults lrMrecTrainRes[MAX_CH][MAX_STROBE];
  struct TrainingResults  lrMrdTrainRes[MAX_CH][MAX_STROBE];
  struct TrainingResults  lrDwlTrainRes[MAX_CH][MAX_STROBE];
  struct lrMrecTrainingResults lrCwlTrainRes[MAX_CH][MAX_STROBE];
  struct TrainingResults  lrMwdTrainRes[MAX_CH][MAX_STROBE];
  UINT8                   InPbaWaMode;
  UINT8                   InOvrRttPrkMode;
#endif // LRDIMM_SUPPORT

  UINT8                   maxSubRank;
  UINT8                   currentSubRank;
  struct TrainingResults  trainRes[MAX_CH][MAX_STROBE];
  BOOLEAN                 PerformanceTrackerInitialized;
  UINT8                   firstPass;
  UINT8                   previousBootError;
  UINT8                   xoverModeVar;     ///< xover mode (1 = xover2:2, 0 = native 1:1)
  UINT32                  bssaBdatSize;                                 ///<total size for all the saveToBdat() calls (made for each different HOB)
  UINT8                   bssaNumberHobs[MAX_NUMBER_SSA_BIOS_RESULTS];  ///<same GUID multiple HOBs (<60kB)
  EFI_GUID                bssaBdatGuid[MAX_NUMBER_SSA_BIOS_RESULTS];    ///<track all GUIDs used with saveToBdat()
  UINT8                   bssaNumberDistinctGuids;                      ///<size of array

  SPD_SOURCE              SpdSource;        ///< Determines SPD data source
  UINT8                   SmbMode[MAX_SOCKET][MAX_SMB_INSTANCE]; ///< Stores type of smbus mode: 0 - I2C mode, 1 - I3C mode
  UINT8                   rankInter;        ///< 1, 2, 4, or 8 way interleave
  UINT32                  mccpubusno;
  UINT32                  rtDefaultValue;
  UINT8                   channelEn;
  UINT8                   runningRmt;   ///< Flag to check that the RankMarginTool is currently being running
  UINT32                  lastCheckpoint[MAX_SOCKET];
  UINT8                   notRunningFromCT; ///< PostPackageRepairMain called from the CallTable (CT)
  UINT32                  chBitMask;
  UINT8                   FmcMaxCached;     ///< Record how many FMC register been cached
  UINT32                  FmcCachedReads;   ///< Record the amount of total FMC cache read times
  MEM_CHIP_POLICY         memChipPolicy;
  BOOLEAN                 secondPass[MAX_CH][MAX_STROBE];
  UINT8                   volMemMode;       ///< 0: 1LM  1:2LM 2:Undefined
  UINT8                   CacheMemType;     ///< 0: DDR$DDRT  1:HBM$DDR  Only valid if volMemMode is 2LM

  UINT8                   PmemCaching;        // caching contorl for AppDirect
  UINT8                   EadrSupport;        // eADR support
  UINT8                   EadrCacheFlushMode;

  UINT8                   imcInter;           // Number of ways to interleave imc (1 or 2)
  UINT32                  mmiohBase;          // MMIOH base in 64MB granularity
  UINT8                   read2tckCL[MAX_CH];     // increases to CL or CWL based on TCLK preamble
  UINT8                   write2tckCWL[MAX_CH];
  UINT8                   memInterleaveGran1LM;
  struct dfxMemVars       dfxMemVars;
  UINT32                  optionsNgn;
  UINT8                   setSecureEraseAllDIMMs;
  UINT8                   setSecureEraseSktCh[MAX_SOCKET][MAX_CH];
  UINT8                   SetSecureEraseSktChHob[MAX_SOCKET][MAX_CH];
  BOOLEAN                 AdrStateForPmemModule[MAX_SOCKET][MAX_CH];   // ADR state for Intel PMEM Modules
  UINT8                   AppDirectMemoryHole;
  UINT8                   LsxImplementation;
  UINT32                  NvdimmSmbusMaxAccessTime;
  UINT32                  NvdimmSmbusReleaseDelay;
  MEMORY_MAP_BLOCK_DECODER_DATA BlockDecoderData;   // Block decoder data for memory map
  UINT8                   ExtendedType17;

  BOOLEAN                 DdrtReadPendingQueueTimedOut[MAX_SOCKET][MAX_CH];  ///< Each element represents whether a given channel has had a DDRT Read Pending Queue (RPQ) timeout
  BOOLEAN                 DdrtReadPendingQueueCreditLimitSet[MAX_SOCKET][MAX_CH];  ///< Each element represents whether a given channel has the DDRT Read Pending (RPQ) credit limit enabled
  BOOLEAN                 DdrtSkipRpqDrainInCpgcPolling[MAX_SOCKET];  ///< Indicates whether to drain the DDRT RPQ when polling for CPGC done
  BOOLEAN                 WrLvlDeNormalizeStatus[MAX_SOCKET][MAX_CH][MAX_DIMM];
  BOOLEAN                 EkvPresent;   ///< Set if EKV controller on system
  BOOLEAN                 BwvPresent;   ///< Set if BWV controller on system
  BOOLEAN                 DcpmmWaitMediaReady; ///< Set on Slow Warm boots to indicate that BIOS needs to poll for Media Ready on DCPMM DIMMs after CKE is asserted
  BOOLEAN                 WarmBootRequested; ///< Set on Warm boots when is not possible to detect warm boot it via subBootMode (when BIOS internally promoted warm reset to cold reset/fast cold reset)
  CMI_CREDIT_CONFIG_DEFAULT   CmiCreditConfigDefault;
  UINT16                  BiosFisRevision;
  UINT16                  MaxAveragePowerLimit;  ///< Max Power limit in mW used for averaged power ( Valid range starts from 10000mW).
  UINT16                  MaxMbbPowerLimit;    ///< Max MBB power limit ( Valid range starts from 15000mW).
  UINT32                  MaxMbbAveragePowerTimeConstant;  ///<Max MBB Average Power Time Constant.
} MEM_VAR_STRUCT;  // struct memVar

#pragma pack(pop)

#endif // _memhost_h

