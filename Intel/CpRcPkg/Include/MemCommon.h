/** @file
  Mem common Hearder File

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

#ifndef __MEM_COMMON_H__
#define __MEM_COMMON_H__
#include <Library/PcdLib.h>

#if !defined(SILENT_MODE)
#define DEBUG_CODE_BLOCK  1
#endif

#ifdef DEBUG_CODE_BLOCK
#define  MRC_TRACE  1
#endif //DEBUG_CODE_BLOCK

//
// Post Package Repair
//

#define MAX_PPR_ADDR_ENTRIES           20
#define MAX_PPR_ADDR_ENTRIES_SPPR      40

#define MAX_NUMBER_SSA_BIOS_RESULTS    2  //i.e. max. no. of GUIDS: 1.) For BIOS SSA tools  2.) Stitched in RMT results

#define NOT_SUP                    0xFF

#define CATCHALL_TIMEOUT  100000   // 100 ms

#define FOUR_GB_MEM         0x40                // 4GB in 64MB units
#define MAX_MEM_ADDR        0x40000             // 46-bit addressing (256MB units)
#define MAX_MEM_ADDR_ICX    0x1000000           // 52-bit addressing (256MB units)
#define MAX_MEM_ADDR_SPR    0x1000000           // 52-bit addressing (256MB units)
#define MAX_MEM_ADDR_SNR    0x4000              // 42-bit addressing (256MB units)
#define HIGH_GAP            1                   // High gap (256 MB units)

#define MMIOH_SIZE          0                   // MMIOH Size Granularity per stack (1GB, (1<<(2*size)))
#define DEFAULT_COM_PORT    0x80

#define R_PCH_SMBUS_BASE               0x20  // This is not defined for CDF in the PCH code

//
// Bit values used when reading the CSR that corresponds
// with $mrc in Simics scripts.
//

#define QUIET_MODE                        BIT16     // flag to enable minimal debug messages
#define RANDOM_TRAINING                   BIT17     // flag to enable random training responses from Simics
#define FORCE_SETUP                       BIT18     // flag to force BIOS setup
#define DDR_TRAINING_EN                   BIT19     // flag to tell the BIOS to execute DDR training
#define UBIOS_GENERATION_EN               BIT22     // flag to enable DfxUbiosGeneration from Simics
#define HYBRID_SYSTEM_LEVEL_EMULATION_EN  BIT23     // flag to enable DfxHybridSystemLevelEmulation from Simics

#define DDRT_DIMM_RANK      0 // This is the only rank on a DDRT DIMM
#define MAX_CH              ((MAX_IMC)*(MAX_MC_CH))         // Max channels per socket
#define QCLK_PER_DCLK       2
#define DCLK_PER_HCLK       2

//
// Valid logical strobes in a DDR5 full 80-bit channel are (0-19) for x4 devices or (0-4, 10-14) for x8 devices
//
#define VALID_LOGICAL_STROBE(_STROBE_CH, _MAX_STROBE_CH, _MAX_STROBE_SUBCH) \
        ((_STROBE_CH % (_MAX_STROBE_CH / 2)) < _MAX_STROBE_SUBCH)

#ifdef DDR5_SUPPORT
#define BURST_LENGTH    16
#else
#define BURST_LENGTH    8
#endif //


typedef enum {
  ReservedMemSs,
  Ddr4MemSs = 1,
  Ddr5MemSs = 2,
  LpDdr4MemSs = 3,
  LpDdr5MemSs = 4,
  Hbm2MemSs = 5,
  MrcMstMax,
  MrcMstDelim = MAX_INT32
} MRC_MST;

typedef enum {
  MemTechDdr = 0,
  MemTechHbm,
  MemTechFmc,
  MemTech_Max
} MEM_TECH_TYPE;

typedef enum {
  TYPE_SCF_BAR = 0,
  TYPE_PCU_BAR,
  TYPE_MEM_BAR0,
  TYPE_MEM_BAR1,
  TYPE_MEM_BAR2,
  TYPE_MEM_BAR3,
  TYPE_MEM_BAR4,
  TYPE_MEM_BAR5,
  TYPE_MEM_BAR6,
  TYPE_MEM_BAR7,
  TYPE_SBREG_BAR,
  TYPE_MAX_MMIO_BAR
} MMIO_BARS;

//
// Memory Timings
//
typedef enum {
  tREFI = 0,
  tRFC,
  tRAS,
  tCWL,
  tCL,
  tRP,
  tRCD,
  tRRD,
  tRRD_S,
  tRRD_L,
  tWTR,
  tWTR_S,
  tWTR_L,
  tCCD,
  tCCD_S,
  tCCD_L,
  tCCD_WR,
  tCCD_WR_L,
  tRTP,
  tCKE,
  tFAW,
  tWR,
  tPRPDEN,
  tXP,
  tZQCS,
  tSTAB,
  tMOD,
  tREFIx9,
  tXSDLL,
  tXSOFFSET,
  tRRDR,
  tRRDD,
  tRRSG,
  tRRSR,
  tRWDD,
  tRWDR,
  tRWSG,
  tRWSR,
  tWRDD,
  tWRDR,
  tWRSG,
  tWRSR,
  tWWDD,
  tWWDR,
  tWWSG,
  tWWSR,
  tRC,
  tRDA,
  tWRA,
  tWRPRE,
  tWRRDA,
  tRDPDEN,
  tWRPDEN,
  tRRDS,
  tRWDS,
  tWRDS,
  tWWDS,
  MemTimingTypeMax
} MEM_TIMING_TYPE;

//
// Provide an association between MRC_MST values and memory subsystem type strings
//
typedef struct {
  MRC_MST     MemSsType;
  CHAR8       *String;
} MRC_MST_STRING;

//
// Min/Max value pair
//
typedef struct {
  UINT32              MinValue;
  UINT32              MaxValue;
} MIN_MAX_PAIR;


//
// DIMM types
//
#define RDIMM             0
#define UDIMM             1
#define RDIMMandUDIMM     2
#define SODIMM            2
#define LRDIMM            9
#define QRDIMM            10
#define NVMDIMM           11
#define VLPRDIMM          12
#if FixedPcdGetBool(PcdMcrSupport)
#define MCRDIMM           13
#endif // FixedPcdGetBool(PcdMcrSupport)
//
// DDR4 frequencies 1333 - 4200
//
#define DDR_800  0   // tCK(ns)=2.5
#define DDR_1000 1   // tCK(ns)=2.0
#define DDR_1066 2   // tCK(ns)=1.875
#define DDR_1200 3   // tCK(ns)=1.667
#define DDR_1333 4   // tCK(ns)=1.5
#define DDR_1400 5   // tCK(ns)=1.429
#define DDR_1600 6   // tCK(ns)=1.25
#define DDR_1800 7   // tCK(ns)=1.11
#define DDR_1866 8   // tCK(ns)=1.07
#define DDR_2000 9   // tCK(ns)=1.0
#define DDR_2133 10  // tCK(ns)=0.9375
#define DDR_2200 11  // tCK(ns)=0.909
#define DDR_2400 12  // tCK(ns)=0.833
#define DDR_2600 13  // tCK(ns)=0.769
#define DDR_2666 14  // tCK(ns)=0.750
#define DDR_2800 15  // tCK(ns)=0.714
#define DDR_2933 16  // tCK(ns)=0.682
#define DDR_3000 17  // tCK(ns)=0.667
#define DDR_3200 18  // tCK(ns)=0.625
#define DDR_3400 19  // tCK(ns)=0.588
#define DDR_3466 20  // tCK(ns)=0.577
#define DDR_3600 21  // tCK(ns)=0.556
#define DDR_3733 22  // tCK(ns)=0.536
#define DDR_3800 23  // tCK(ns)=0.526
#define DDR_4000 24  // tCK(ns)=0.5
#define DDR_4200 25  // tCK(ns)=0.476
#define DDR_4266 26  // tCK(ns)=0.469
#define DDR_4400 27  // tCK(ns)=0.455
#define DDR_4800 28  // tCK(ns)=0.417
#define DDR_5200 29  // tCK(ns)=0.384
#define DDR_5600 30  // tCK(ns)=0.357
#define DDR_6000 31  // tCK(ns)=0.333
#define DDR_6400 32  // tCK(ns)=0.313
#define DDR_6800 33  // tCK(ns)=0.295
#define DDR_7200 34  // tCK(ns)=0.278
#define DDR_7600 35  // tCK(ns)=0.264
#define DDR_8000 36  // tCK(ns)=0.250
#define DDR_8400 37  // tCK(ns)=0.238
#if FixedPcdGetBool(PcdMcrSupport)
#define DDR_8800 38  // tCK(ns)=0.228
#endif // FixedPcdGetBool(PcdMcrSupport)
#define DDR_FREQ_ANY    0xFF
#define DDR_OVERCLOCK_FREQ DDR_FREQ_ANY // Value to be used during overclocking
//
// DDR_*_TCK_MIN use 1 ps(picosecond) precision
//
#define FREQUENCY_MTB_OFFSET            1000
#define FREQUENCY_FTB_OFFSET            1
#define DDR_800_TCK_MIN                 2500
#define DDR_1000_TCK_MIN                2000
#define DDR_1066_TCK_MIN                1875
#define DDR_1200_TCK_MIN                1667
#define DDR_1333_TCK_MIN                1500
#define DDR_1400_TCK_MIN                1429
#define DDR_1600_TCK_MIN                1250
#define DDR_1800_TCK_MIN                1110
#define DDR_1866_TCK_MIN                1071
#define DDR_2000_TCK_MIN                1000
#define DDR_2133_TCK_MIN                938
#define DDR_2200_TCK_MIN                909
#define DDR_2400_TCK_MIN                833
#define DDR_2600_TCK_MIN                769
#define DDR_2666_TCK_MIN                750
#define DDR_2800_TCK_MIN                714
#define DDR_2933_TCK_MIN                682
#define DDR_3000_TCK_MIN                667
#define DDR_3200_TCK_MIN                625
#define DDR_3400_TCK_MIN                589
#define DDR_3466_TCK_MIN                577
#define DDR_3600_TCK_MIN                556
#define DDR_3733_TCK_MIN                536
#define DDR_3800_TCK_MIN                527
#define DDR_4000_TCK_MIN                500
#define DDR_4200_TCK_MIN                477
#define DDR_4266_TCK_MIN                469
#define DDR_4400_TCK_MIN                455
#define DDR_4800_TCK_MIN                417
#define DDR_5200_TCK_MIN                385
#define DDR_5600_TCK_MIN                358
#define DDR_6000_TCK_MIN                334
#define DDR_6400_TCK_MIN                313
#define DDR_6800_TCK_MIN                295
#define DDR_7200_TCK_MIN                278
#define DDR_7600_TCK_MIN                264
#define DDR_8000_TCK_MIN                250
#define DDR_8400_TCK_MIN                239
#if FixedPcdGetBool(PcdMcrSupport)
#define DDR_8800_TCK_MIN                228
#endif // FixedPcdGetBool(PcdMcrSupport)

///
/// HBM2 frequencies GTPerSec
///
typedef enum {
  HBM2_1p6 = 0,
  HBM2_2p0,
  HBM2_2p4,
  HBM2E_2p8,
  HBM2E_3p2,
  HBM_FREQ_MAX
} HBM_FREQUENCY;

// 1 ns == 1000 ps(picosecond)
#define NS_TO_PS_OFFSET            1000

//
// NOT_SUP is used for PorFreq and StretchFreq in DIMM_POR_ENTRY when the configuration
// does not have a corresponding supported frequency.
//
#define NOT_SUP     0xFF

#if FixedPcdGetBool(PcdMcrSupport)
#define MAX_SUP_FREQ      39 // 39 frequencies are supported ( 800 1000 1066 1200 1333 1400 1600
                             //                               1800 1866 2000 2133 2200 2400 2600
                             //                               2666 2800 2933 3000 3200 3400 3467
                             //                               3600 3733 3800 4000 4200 4266 4400
                             //                               4800 5200 5600 6000 6400 6800 7200
                             //                               7600 8000 8400 8800)
#else
//
// DDR4 frequencies 800 - 4400
//
#define MAX_SUP_FREQ      38 // 38 frequencies are supported ( 800 1000 1066 1200 1333 1400 1600
                             //                               1800 1866 2000 2133 2200 2400 2600
                             //                               2666 2800 2933 3000 3200 3400 3467
                             //                               3600 3733 3800 4000 4200 4266 4400
                             //                               4800 5200 5600 6000 6400 6800 7200
                             //                               7600 8000 8400)
#endif // FixedPcdGetBool(PcdMcrSupport)

// Memory parameters and SPD JEDEC definitions
#define MAX_SPD_BYTE_DDR5 1024 // Number of bytes in Serial EEPROM on DDR5
#define MAX_SPD_BYTE_DDR4 512 // Number of bytes in Serial EEPROM on DDR4

#ifdef DDR5_SUPPORT
#define MAX_SPD_BYTE_DDR MAX_SPD_BYTE_DDR5
#else
#define MAX_SPD_BYTE_DDR MAX_SPD_BYTE_DDR4
#endif

#define DDP_DIE_COUNT  2  // Multi load stack DIMM with 2 die count indicates Dual Die Package
#define QDP_DIE_COUNT  4  // Multi load stack DIMM with 4 die count indicates Quad Die Package

///
/// MRC status
///
typedef enum {
  MRC_STATUS_SUCCESS,
  MRC_STATUS_FAILURE,
  MRC_STATUS_STROBE_NOT_PRESENT,
  MRC_STATUS_RANK_NOT_PRESENT,
  MRC_STATUS_CH_NOT_PRESENT,
  MRC_STATUS_LEVEL_NOT_SUPPORTED,
  MRC_STATUS_GROUP_NOT_SUPPORTED,
  MRC_STATUS_SIGNAL_NOT_SUPPORTED,
  MRC_STATUS_GSM_LIMIT,
  MRC_STATUS_NOT_FOUND,
  MRC_STATUS_INVALID_PARAMETER,
  MRC_FUNCTION_NOT_IMPLEMENTED,
  MRC_STATUS_UNSUPPORTED,
  MRC_STATUS_BUFFER_TOO_SMALL,
  MRC_STATUS_OUT_OF_RESOURCES
} MRC_STATUS;

//
// CpuPciAccess
//
#define READ_ACCESS  0
#define WRITE_ACCESS 1

//
// Platform Assigned CSRs for PIPE/SOCKET/MRC MILESTONES, Pipe.h
// These macros are used by code that does not have visiblity to the chip register definitions
//
#define SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ADDR(Host)        (MEM_CHIP_POLICY_VALUE(Host, SrMemoryDataStorageDispatchPipeCsr))
#define SR_MEMORY_DATA_STORAGE_COMMAND_PIPE_CSR_ADDR(Host)         (MEM_CHIP_POLICY_VALUE(Host, SrMemoryDataStorageCommandPipeCsr))
#define SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_ADDR(Host)            (MEM_CHIP_POLICY_VALUE(Host, SrMemoryDataStorageDataPipeCsr))
#define SR_ERROR_CODE_CSR_ADDR(Host)                               (MEM_CHIP_POLICY_VALUE(Host, SrErrorCodeCsr))
#define SR_BIOS_SERIAL_DEBUG_CSR_ADDR(Host)                        (MEM_CHIP_POLICY_VALUE(Host, SrBiosSerialDebugCsr))
#define SR_POST_CODE_CSR_ADDR(Host)                                (MEM_CHIP_POLICY_VALUE(Host, SrPostCodeCsr))

// Scratchpad register indirect references
#define SR_BIOS_SERIAL_DEBUG  6
#define SR_POST_CODE          7

#endif //#ifndef __MEM_COMMON_H__
