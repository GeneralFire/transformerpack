/** @file

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

#ifndef  _mem_defaults_h
#define  _mem_defaults_h

#define AUTO                  0

//
// Enforce POR
//
#define ENFORCE_POR_EN       0
#define ENFORCE_STRETCH_EN   1
#define ENFORCE_POR_DIS      2
// Default is set by Fixed PCD PcdEnforcePorDefault

//
// Enforce Population POR
//
#define ENFORCE_POPULATION_POR_DIS                0
#define ENFORCE_POPULATION_POR_ENFORCE_SUPPORTED  1
#define ENFORCE_POPULATION_POR_ENFORCE_VALIDATED  2
// Default is set by Fixed PCD PcdEnforcePopulationPorDefault

//
// PPR definitions
//
#define PPR_TYPE_HARD     1
#define PPR_TYPE_SOFT     2
#define PPR_DISABLED      0
#define PPR_TYPE_DEFAULT  PPR_TYPE_SOFT

//
// PPR Error injection
//
#define PPR_ERR_INJ_DISABLE   0
#define PPR_ERR_INJ_ENABLE    1
#define PPR_ERR_INJ_DEFAULT   PPR_ERR_INJ_DISABLE

//
// DDR Frequency
//
#define DDR_FREQ_AUTO       AUTO

//
// Promote MRC warnings
//
#define PROMOTE_MRC_WARN_DISABLE   0      // Promote MRC warnings disabled
#define PROMOTE_MRC_WARN_ENABLE    1      // Promote MRC warnings enabled
#define PROMOTE_MRC_WARN_DEFAULT   PROMOTE_MRC_WARN_ENABLE

//
// Promote warnings
//
#define PROMOTE_WARN_DISABLE      0      // Promote warnings disabled
#define PROMOTE_WARN_ENABLE       1      // Promote warnings enabled
#define PROMOTE_WARN_DEFAULT      PROMOTE_WARN_ENABLE

//
// Halt on error
//
#define HALT_ON_ERROR_DISABLE     0      // Halt on error disabled
#define HALT_ON_ERROR_ENABLE      1      // Halt on error enabled
#ifdef DEBUG_CODE_BLOCK
#define HALT_ON_ERROR_AUTO_RESET  2      // Reset and set Max Serial debug level Halt on error
#endif //ifdef DEBUG_CODE_BLOCK
#define HALT_ON_ERROR_DEFAULT     HALT_ON_ERROR_ENABLE

//
// Multi-Threaded MRC default set by PcdMrcMultiThreadedDefault
//

#define MULTI_THREADED_DISABLE 0
#define MULTI_THREADED_ENABLE  1

//
// ECC Support
//
#define ECC_DISABLE   0           // Disable ECC support
#define ECC_ENABLE    1           // Enable ECC support
#define ECC_DEFAULT   ECC_ENABLE  // Changing the default here requires a change in the help string of the respective BIOS knob.

//
// Log Parsing - option of enabling additional information in the log to make parsing it more generic and easier in MT
//
#define LOG_PARSING_DIS     0     // Disabled
#define LOG_PARSING_EN      1     // Enabled
#define LOG_PARSING_DEFAULT LOG_PARSING_DIS

//
// LRDIMM Module Delay
//
#define MODULE_DELAY_AUTO       0       // Boundary check SPD MD and use if good, else defaults
#define MODULE_DELAY_DISABLE    1       // Do NOT use SPD MD. Use Defaults
#define MODULE_DELAY_DEFAULT    MODULE_DELAY_AUTO

//
// Memory test enable
//
#define MEM_TEST_DISABLE   0          // Disable memory test
#define MEM_TEST_ENABLE    1          // Enable memory test
#define MEM_TEST_DEFAULT   MEM_TEST_ENABLE

//
// Memory Training Comp Options Values
//
#define TRAINING_COMP_MMRC_INIT         0
#define TRAINING_COMP_EVERY_JEDEC_INIT  1
#define TRAINING_COMP_EVERY_TRAINING    2
#define TRAINING_COMP_DEFAULT TRAINING_COMP_MMRC_INIT

//
// Memory Periodic Rcomp
//
#define PERIODIC_RCOMP_DISABLE 0
#define PERIODIC_RCOMP_ENABLE  1
#define PERIODIC_RCOMP_AUTO    2
#define PERIODIC_RCOMP_DEFAULT PERIODIC_RCOMP_AUTO

//
// Memory Periodic Rcomp Interval.
//
#define MIN_PERIODIC_RCOMP_INTERVAL     0x0
#define MAX_PERIODIC_RCOMP_INTERVAL     0xF

//
// Number of MemTests loops to execute for legacy MemTest (type 8 and 10), that
// provides the ability of inverting the data pattern in every odd pass for detecting
// opposite polarity faults
//
#define MEM_TEST_LOOPS_DEFAULT      1          // Number of MemTests to execute

//
// DRAM Maintenance Test-> DMT_EN DMT_DIS
//
#define DMT_DIS               0
#define DMT_EN                1
#define DMT_AUTO              DMT_DIS
#define DMT_DIRECTION_UP      0
#define DMT_DIRECTION_DOWN    1
#define DMT_INVERT_DIS        0
#define DMT_INVERT_EN         1
#define DMT_REPETITIONS       100000
#define DMT_ITERATIONS        1
#define DMT_SWIZZLE_DIS       0
#define DMT_SWIZZLE_EN        1
#define DMT_SWIZZLE_AUTO      2
#define DMT_REFRESH_DIS       0
#define DMT_REFRESH_EN        1

//
// DDR Memory Type
//
#define RDIMM_TYPE       0
#define UDIMM_TYPE       1
#define UDIMM_RDIMM_TYPE 2
#define DDR_TYPE_DEFAULT UDIMM_RDIMM_TYPE

//
// Do we attempt a fast boot path through MRC? Default set by PcdMrcFastBootDefault
//
#define FAST_BOOT_DISABLE     0       // Disable fast boot (i.e. take the normal cold boot path through MRC)
#define FAST_BOOT_ENABLE      1       // Enable fast boot (i.e. skip parts of MRC when possible)
#define FAST_BOOT_AUTO        2

//
// Fast Cold Boot default set by PcdMrcFastColdBootDefault
//
#define FAST_BOOT_COLD_DISABLE     0       // Disable fast boot (i.e. take the normal cold boot path through MRC)
#define FAST_BOOT_COLD_ENABLE      1       // Enable fast boot (i.e. skip parts of MRC when possible)


// Memory test enable for cold fast boot scenario
#define MEM_TEST_COLD_FAST_BOOT_DISABLE   0      // Disable memory test during cold fast boot
#define MEM_TEST_COLD_FAST_BOOT_ENABLE    1      // Enable memory test during cold fast boot
#define MEM_TEST_COLD_FAST_BOOT_DEFAULT   MEM_TEST_COLD_FAST_BOOT_DISABLE

//
// RMT on Cold Fast Boot
//
#define RMT_COLD_FAST_BOOT_DISABLE   0
#define RMT_COLD_FAST_BOOT_ENABLE    1
#define RMT_COLD_FAST_BOOT_DEFAULT_SKX   RMT_COLD_FAST_BOOT_DISABLE
#define RMT_COLD_FAST_BOOT_DEFAULT_10NM  RMT_COLD_FAST_BOOT_DISABLE

//
// Legacy RMT default
//
#define LEGACY_RMT_DISABLE            0
#define LEGACY_RMT_ENABLE             1

//
// Bdat enable set by PcdMrcBdatDefault
//

#define BDAT_DISABLE    0
#define BDAT_ENABLE     1

//
// DDRT Scrambling support
//
#define SCRAMBLE_DDRT_DISABLE    0       // Disable Data Scrambling
#define SCRAMBLE_DDRT_ENABLE     1       // Enable Data Scrambling
#define SCRAMBLE_DDRT_DEFAULT    SCRAMBLE_DDRT_ENABLE

//
// Scrambling support
//
#define SCRAMBLE_DISABLE    0       // Disable Data Scrambling
#define SCRAMBLE_ENABLE     1       // Enable Data Scrambling
#define SCRAMBLE_DEFAULT    SCRAMBLE_ENABLE

//
// ALLOW_CORRECTABLE_ERRORS
//
#define ALLOW_CORRECTABLE_ERROR_DISABLE     0
#define ALLOW_CORRECTABLE_ERROR_ENABLE      1
#define ALLOW_CORRECTABLE_ERROR_AUTO        2
#define ALLOW_CORRECTABLE_ERROR_DEFAULT     ALLOW_CORRECTABLE_ERROR_DISABLE  // Changing the default here requires a change in the help string of the respective BIOS knob.

//
// ALLOW_CORRECTABLE_MEM_TEST_ERRORS
//
#define ALLOW_CORRECTABLE_MEM_TEST_ERROR_DISABLE   0
#define ALLOW_CORRECTABLE_MEM_TEST_ERROR_ENABLE    1
#define ALLOW_CORRECTABLE_MEM_TEST_ERROR_DEFAULT   ALLOW_CORRECTABLE_MEM_TEST_ERROR_ENABLE

//
// Force ODT ON control
//
#define FORCE_ODT_ON_DISABLE 0
#define FORCE_ODT_ON_ENABLE  1
#define FORCE_ODT_ON_DEFAULT FORCE_ODT_ON_ENABLE

//
// Early Write CRC in the DDRIO and late Write CRC in the DDRIO, MC and DRAM control
//
#define WR_CRC_DISABLE             0
#define WR_CRC_ENABLE              1
#define EARLY_WR_CRC_DDRIO_DEFAULT WR_CRC_DISABLE
#define WR_CRC_DEFAULT             WR_CRC_DISABLE

//
// DIMM Isolation
//
#define DIMM_ISOLATION_DISABLE    0
#define DIMM_ISOLATION_ENABLE     1
// Default is set by Fixed PCD PcdDimmIsolationDefault

//
// Read/WritePreamble TCLK set by PcdMrcWritePreambleTclkDefault
// and PcdMrcReadPreambleTclkDefault
//

#define PREAMBLE_1TCLK          0
#define PREAMBLE_2TCLK          1
#define PREAMBLE_3TCLK          2
#define PREAMBLE_4TCLK          3
#define PREAMBLE_AUTO           0xFF

//
// Read/WtitePostamble TCLK
//

#define POSTAMBLE_0P5TCLK      0
#define POSTAMBLE_1P5TCLK      1

//
// Scramble seed
//
#define MIN_SCRAMBLE_SEED_LOW  0
#define MAX_SCRAMBLE_SEED_LOW  65535
#define SCRAMBLE_SEED_LOW      41003

#define MIN_SCRAMBLE_SEED_HIGH 0
#define MAX_SCRAMBLE_SEED_HIGH 65535
#define SCRAMBLE_SEED_HIGH     54165

//
// Rank Switch Fix
//
#define RANK_SWITCH_FIX_DIS               0
#define RANK_SWITCH_FIX_TA_FLOOR          1
#define RANK_SWITCH_FIX_RCVEN_AVE         2
#define RANK_SWITCH_FIX_RCVEN_AVE_PT1     3


//
// ADREn
//
#define ADR_DISABLE   0
#define ADR_ENABLE    1
#define ADR_DEFAULT   ADR_ENABLE

//
// Legacy ADR
//
#define LEGACY_ADR_MODE_DISABLE   0
#define LEGACY_ADR_MODE_ENABLE    1

//
// Check PCH_PM_STS
//
#define CHECK_PM_STS_DISABLE   0
#define CHECK_PM_STS_ENABLE    1
#define CHECK_PM_STS_DEFAULT   CHECK_PM_STS_DISABLE

//
// Check PlatformDetectADR
//
#define CHECK_PLATFORM_DETECT_DISABLE  0
#define CHECK_PLATFORM_DETECT_ENABLE   1
#define CHECK_PLATFORM_DETECT_DEFAULT  CHECK_PLATFORM_DETECT_DISABLE

//
// CustomRefreshRate
//
#define CUSTOM_REFRESH_RATE_DISABLE 0
#define CUSTOM_REFRESH_RATE_ENABLE  1
#define CUSTOM_REFRESH_RATE_DEFAULT CUSTOM_REFRESH_RATE_DISABLE

//
// CustomRefreshRate
//
#define CUST_REFRESH_RATE_MIN      20
#define CUST_REFRESH_RATE_MAX_SKX  40
#define CUST_REFRESH_RATE_MAX_10NM 80
#define CUST_REFRESH_RATE_DEFAULT  CUST_REFRESH_RATE_MIN

//
// DLL Reset Test
//
#define MIN_DLL_RESET_TEST_LOOPS        0
#define MAX_DLL_RESET_TEST_LOOPS     0xFF
#define DLL_RESET_TEST_LOOPS_DEFAULT MIN_DLL_RESET_TEST_LOOPS

//
// Opportunistic Reads in WMM
//
#define OPP_RD_WMM_DISABLE   0
#define OPP_RD_WMM_ENABLE    1
#define OPP_RD_WMM_DEFAULT   OPP_RD_WMM_ENABLE

//
// Normal Operation Interval
//
#define MIN_NORMAL_OPPERATION_INTERVAL     0
#define MAX_NORMAL_OPPERATION_INTERVAL     0xFFFF
#define NORMAL_OPPERATION_INTERVAL_DEFAULT 1024

//
// C/A Parity
//
#define CA_PARITY_DISABLE     0
#define CA_PARITY_ENABLE      1
#define CA_PARITY_DEFAULT     CA_PARITY_ENABLE  // Changing the default here requires a change in the help string of the respective BIOS knob.

//
// SMBUS Clk Period default set by PcdMrcSmbusSpeedDefault
//
#define SMB_CLK_100K       0
#define SMB_CLK_400K       1
#define SMB_CLK_700K       2
#define SMB_CLK_1M         3

//
// SMBUS SPD Lock access
//
#define SMB_SPD_LOCK           0
#define SMB_SPD_UNLOCK         1
#ifdef CWV_A0_REMOVE_POST_PO
#define SMB_SPD_ACCESS_DEFAULT SMB_SPD_UNLOCK
#else
#define SMB_SPD_ACCESS_DEFAULT SMB_SPD_LOCK
#endif
//
// SPD Print default set by PcdMrcSpdPrintDefault
//
#define SPD_PRINT_DIS     0
#define SPD_PRINT_EN      1

// DDR Cycling Support
#define DDR_CYCLING_DISABLE 0       // Disable DDR Cycling
#define DDR_CYCLING_ENABLE  1       // Enable DDR Cycling
#define DDR_CYCLING_DEFAULT DDR_CYCLING_DISABLE

//
// MEM_FLOWS
//
#define MIN_MEM_FLOWS      0
#define MAX_MEM_FLOWS      0xFFFFFFFF
#ifdef SPR_PO_FLAG
// HSD 22010487103
// Disable following
// Round Trip Optimization - RoundTripOptimizeTraining - bit13
#define MEM_FLOWS_DEFAULT  0xFFFFDFFF
#else
#define MEM_FLOWS_DEFAULT  MAX_MEM_FLOWS
#endif

//
// MEM_FLOWS_EXT
//
#define MIN_MEM_FLOWS_EXT      0
#define MAX_MEM_FLOWS_EXT      0xFFFFFFFF
#define MEM_FLOWS_EXT_DEFAULT  MAX_MEM_FLOWS_EXT

//
// MEM_FLOWS_EXT2
//
#define MIN_MEM_FLOWS_EXT2      0
#define MAX_MEM_FLOWS_EXT2      0xFFFFFFFF
#ifdef SPR_PO_FLAG
//
// HSD 14011201082: [SPR A0 PO] Workaround --
// Disable advanced training steps by default to unblock other teams.
//
// HSD 22010487103
// Disable following:
// CA Slew Rate Training - CaSlewRateTraining - 71

//
// HSD 1507913272
// Disable following:
// Turnaround Training - TurnaroundTraining - 74
//
// HSD 1508007548
// Disable following:
// Tx Dq Slew Rate - DqSlewRateTraining - 77
//

//
// HSD 1508062307
// Disable following since the CSR trace log is too large with it (increase 2.4GB)
// Rcd Dca Dfe - RcdDcaDfeDdr5 - 67
//

//
// HSD 22011065215 to disable Periodic Retrain.
//
#define MEM_FLOWS_EXT2_DEFAULT  0xFFFFDB3F

#else // #ifdef SPR_PO_FLAG
#define MEM_FLOWS_EXT2_DEFAULT  MAX_MEM_FLOWS_EXT2
#endif // #ifdef SPR_PO_FLAG

//
// MEM_FLOWS_EXT3
//
#define MIN_MEM_FLOWS_EXT3      0
#define MAX_MEM_FLOWS_EXT3      0xFFFFFFFF
#define MEM_FLOWS_EXT3_DEFAULT  MAX_MEM_FLOWS_EXT3

#define PO_SAFE_VALUE             0
#define POR_VALUE                 1

#define NumofRank_1          1
#define NumofRank_2          2
#define NumofRank_4          4

#define MIN_TREFI_DELAY     0
#define MAX_TREFI_DELAY     400
#define TREFI_DELAY_DEFAULT 0

#define MIN_CMD_SETUP_PERCENT     0
#define MAX_CMD_SETUP_PERCENT     100
#define CMD_SETUP_PERCENT_DEFAULT 50
//
// BLOCK GNT2CMD1CYC
//
#define BLOCK_GNT2CMD1CYC_DEFAULT POR_VALUE

//
// Disable NVMDIMM OPPRD
//
#ifdef CWV_A0_REMOVE_POST_PO
#define DIS_NVMDIMM_OPPRDC_DEFAULT PO_SAFE_VALUE
#else
#define DIS_NVMDIMM_OPPRDC_DEFAULT POR_VALUE
#endif
//
// Rank Mask
//
#define RANK_MASK_DISABLE 0
#define RANK_MASK_ENABLE  1
#define RANK_MASK_DEFAULT RANK_MASK_DISABLE

//
// Channel disabling for socket memory configuration
//
#define MIN_RANK_ENABLE     0
#define MAX_RANK_ENABLE     0xFF
#define RANK_ENABLE_DEFAULT MAX_RANK_ENABLE

//
// Page Policy
//
#define CLOSED_PAGE_DIS     0       // Closed Page mode disabled
#define CLOSED_PAGE_EN      1       // Closed Page mode enabled
#define OPEN_PAGE_DIS       0       // Open Page mode disabled
#define OPEN_PAGE_EN        1       // Open Page mode enabled
#define OPEN_PAGE_ADAPTIVE  2       // Adaptive Open page mode enabled
#define CLOSED_PAGE_DEFAULT OPEN_PAGE_ADAPTIVE

//
// Tx Equalization calibration
//
#define TX_EQ_DISABLE 0
#define TX_EQ_ENABLE  1
#define TX_EQ_DEFAULT TX_EQ_ENABLE

//
// CMD Tx Equalization calibration
//
#define CMD_TX_EQ_DISABLE 0
#define CMD_TX_EQ_ENABLE  1
#define CMD_TX_EQ_DEFAULT_10NM  CMD_TX_EQ_DISABLE
#define CMD_TX_EQ_DEFAULT_SKX   CMD_TX_EQ_DISABLE

//
// RX DFE default set by PcdMrcRxDfeDefault
//
#define RX_DFE_DISABLE          0
#define RX_DFE_ENABLE           1
#define RX_DFE_AUTO             2

//
// RX DFE DDR5 default set by PcdMrcRxDfeDdr5TrainingDefault
//
#define RX_DFE_DDR5_TRAINING_DISABLE  0
#define RX_DFE_DDR5_TRAINING_ENABLE   1

//
// TX DFE DDR5 default set by PcdMrcTxDfeDdr5TrainingDefault
//
#define TX_DFE_DDR5_TRAINING_DISABLE  0
#define TX_DFE_DDR5_TRAINING_ENABLE   1

//
// TX Rise Fall Slew Rate set by PcdMrcTxRfSlewRateDefault
//
#define TX_RF_SLEW_RATE_DISABLE  0
#define TX_RF_SLEW_RATE_ENABLE   1
#define TX_RF_SLEW_RATE_AUTO     2

//
// iMode training
//
#define IMODE_DISABLE 0
#define IMODE_ENABLE  1

//
// Tco Comp training
//
#define TCO_COMP_DISABLE 0
#define TCO_COMP_ENABLE  1

//
// Round Trip Latency
// Default is set by PcdMrcRoundTripOptimizationTrainingEnable
//
#define ROUND_TRIP_LATENCY_DISABLE 0
#define ROUND_TRIP_LATENCY_ENABLE  1

//
// Duty Cycle Training
//
#define DUTY_CYCLE_DISABLE 0
#define DUTY_CYCLE_ENABLE  1
#define DUTY_CYCLE_AUTO    2
#define DUTY_CYCLE_DEFAULT DUTY_CYCLE_ENABLE

//
// Rx Vref Training
//
#define RX_VREF_DISABLE   0
#define RX_VREF_ENABLE    1
#define RX_VREF_DEFAULT   RX_VREF_ENABLE

//
// Rx Vref Training
//
#define EYE_DIAGRAM_DISABLE   0
#define EYE_DIAGRAM_ENABLE    1
#define EYE_DIAGRAM_AUTO      2
#define EYE_DIAGRAM_DEFAULT   EYE_DIAGRAM_DISABLE

//
// PXC Training set by PcdMrcPxcDefault
//
#define PXC_DISABLE 0
#define PXC_ENABLE  1

//
// DDJC Training set by PcdMrcDdjcDefault
//
#define DDJC_DISABLE 0
#define DDJC_ENABLE  1

//
// LRDIMM DB DFE training set by PcdMrcLrdimmDbDfeDefault
//
#define DBDFE_DISABLE     0
#define DBDFE_PMEM_ONLY   1
#define DBDFE_ALL_LRDIMM  2

//
// Turnaround Time Optimization set by PcdMrcTurnaroundOptimizations
//
#define TURNAROUND_DISABLE   0
#define TURNAROUND_ENABLE    1

//
// DDRT Turnaround Time Optimization set by PcdMrcTurnaroundOptimizationsDdrt
//
#define TURNAROUND_DDRT_DISABLE    0
#define TURNAROUND_DDRT_ENABLE     1

//
// One Rank Timing Mode Option
//
#define ONE_RANK_TIMING_MODE_DISABLED 0
#define ONE_RANK_TIMING_MODE_ENABLED  1
#define ONE_RANK_TIMING_MODE_DEFAULT ONE_RANK_TIMING_MODE_ENABLED

//
// PDA
//
#define PDA_DISABLE 0
#define PDA_ENABLE  1
#if defined (DDR5_SUPPORT) && !defined (SPR_HOST)
  #define PDA_DEFAULT PDA_DISABLE
#else
  #define PDA_DEFAULT PDA_ENABLE
#endif

//
// Tx Vref Training
//
#define TX_VREF_DISABLE       0
#define TX_VREF_ENABLE        1
#define TX_VREF_DEFAULT       TX_VREF_ENABLE

//
// Thermal throttling
//
#define THERMAL_THROT_DISABLE 0
#define OLTT_ENABLE           1
#define CLTT_ENABLE           2
#define CLTT_PECI_ENABLE      3

//
// Disable CLTT for Wilson City PPO interposer
// This is here because the current build configuration for SKX uses this file
// instead of the Purley version of MemDefaults.h
//
#if defined(SKX_HOST) && defined(PPO_FLAG)
#define THERMAL_THROT_DEFAULT THERMAL_THROT_DISABLE
#else
#define THERMAL_THROT_DEFAULT CLTT_ENABLE
#endif

//
// MemTripReporting
//
#define MEMTRIP_REPORTING_DISABLE 0
#define MEMTRIP_REPORTING_ENABLE  1
#define MEMTRIP_REPORTING_DEFAULT MEMTRIP_REPORTING_DISABLE

//
// Thermal trip
//
#define OFFPKG_MEM_TO_THERMTRIP_DISABLE 0
#define OFFPKG_MEM_TO_THERMTRIP_ENABLE  1
#define OFFPKG_MEM_TO_THERMTRIP_DEFAULT OFFPKG_MEM_TO_THERMTRIP_DISABLE

//
// OffPkg MemTrip
//
#define OFFPKG_MEM_TO_MEMTRIP_DISABLE 0
#define OFFPKG_MEM_TO_MEMTRIP_ENABLE  1
#define OFFPKG_MEM_TO_MEMTRIP_DEFAULT OFFPKG_MEM_TO_MEMTRIP_DISABLE

//
// Thermal trip
//
#define INPKG_MEM_TO_THERMTRIP_DISABLE 0
#define INPKG_MEM_TO_THERMTRIP_ENABLE  1
#define INPKG_MEM_TO_THERMTRIP_DEFAULT INPKG_MEM_TO_THERMTRIP_DISABLE

//
// OffPkg MemTrip
//
#define INPKG_MEM_TO_MEMTRIP_DISABLE 0
#define INPKG_MEM_TO_MEMTRIP_ENABLE  1
#define INPKG_MEM_TO_MEMTRIP_DEFAULT INPKG_MEM_TO_MEMTRIP_DISABLE
#define TEMP_HI_AEP 85

//
// MEMHOT Throttling Mode
//
#define MEMHOT_DISABLE      0
#define MEMHOT_OUTPUT_ONLY  1
#define MEMHOT_INPUT_ONLY   2
#define MEMHOT_INPUT_OUTPUT 3
#define MEMHOT_ENABLE       4
#define MEMHOT_DEFAULT      MEMHOT_DISABLE

//
// MEMHOT IN MODE
//
#define MEMHOT_IN_DISABLE 0
#define MEMHOT_IN_ENABLE 1
#define MEMHOT_IN_DEFAULT MEMHOT_IN_DISABLE

//
// MEMHOT IN MODE
//
#define MEMHOT_OUT_DISABLE 0
#define MEMHOT_OUT_ENABLE 1
#define MEMHOT_OUT_DEFAULT MEMHOT_OUT_DISABLE

//
// MEMHOT Output Only Options
//
#define MEMHOT_OUTPUT_ONLY_DIS          0
#define MEMHOT_OUTPUT_ONLY_HIGH         1
#define MEMHOT_OUTPUT_ONLY_HIGH_MED     2
#define MEMHOT_OUTPUT_ONLY_HIGH_MED_LOW 3
#define MEMHOT_OUTPUT_ONLY_DEFAULT      MEMHOT_OUTPUT_ONLY_HIGH

//
// Throttling mid on temp lo set by PcdMrcThrottlingMidOnTempLo
//
#define THRTMID_ON_TEMPLO_DISABLED 0
#define THRTMID_ON_TEMPLO_ENABLED  1

//
// CKE
//
#define CKE_AUTO    0
#define CKE_MANUAL  1
#define CKE_DEFAULT CKE_AUTO

//
// Self Refresh Programming
//
#define SREF_PROG_AUTO    0
#define SREF_PROG_MANUAL  1
#define SREF_PROG_DEFAULT SREF_PROG_AUTO

//
// PKGC SREF
//
#define PKGC_SREF_DISABLE 0
#define PKGC_SREF_ENABLE  1
#define PKGC_SREF_DEFAULT PKGC_SREF_ENABLE

//
// Data DLL Off
//
#define DATA_DLL_OFF_DISABLE 0
#define DATA_DLL_OFF_ENABLE  1
#define DATA_DLL_OFF_DEFAULT DATA_DLL_OFF_ENABLE

#define MIN_CKE_IDLE_TIMER     0
#define MAX_CKE_IDLE_TIMER     255
#define CKE_IDLE_TIMER_DEFAULT 20

//
// APD
//
#define APD_DISABLE 0
#define APD_ENABLE  1
#define APD_DEFAULT APD_DISABLE

//
// PPD
//
#define PPD_DISABLE 0
#define PPD_ENABLE  1
#define PPD_DEFAULT PPD_ENABLE

//
// DDRT CKE
//
#define DDRT_CKE_DISABLE 0
#define DDRT_CKE_ENABLE  1
#define DDRT_CKE_DEFAULT DDRT_CKE_ENABLE

#define MIN_SREF_IDLE_TIMER     0
#define MAX_SREF_IDLE_TIMER     255

//
// Opportunistic Self-Refresh
// Default controlled by PcdOpportunisticSelfRefreshDefault
//
#define OPP_SREF_DISABLE 0
#define OPP_SREF_ENABLE  1

//
// DDRT SREF
// Default controlled by PcdDdrtSrefDefault
//
#define DDRT_SREF_DISABLE 0
#define DDRT_SREF_ENABLE  1

//
// MDLL
//
#define MDLL_OFF_DISABLE 0
#define MDLL_OFF_ENABLE  1
#define MDLL_OFF_DEFAULT MDLL_OFF_ENABLE

//
// CK/CK# OFF for Self Refresh
//
#define CK_SELF_REF_DRIVEN  0    // CK driven during self refresh
#define CK_SELF_REF_LOW     2    // CK pulled low during self refresh
#define CK_SELF_REF_DEFAULT CK_SELF_REF_LOW

//
//  XMP Support
//
#define XMP_AUTO      0
#define XMP_MANUAL    1
#define XMP_PROFILE_1 2
#define XMP_PROFILE_2 3
#define XMP_DEFAULT   XMP_AUTO

//
// VDD
//
#define MIN_VDD     900
#define MAX_VDD     1850

#if defined(DDR5_SUPPORT)
#ifdef SPR_PO_FLAG
#define VDD_DEFAULT 1140 // This is for PO Only
#else
#define VDD_DEFAULT 1125 // This is for POR
#endif

#else //! DDR5_SUPPORT
#define VDD_DEFAULT 1200
#endif
//
// Command timming
//
#define CMD_TIMING_AUTO    0
#define CMD_TIMING_1N      1
#define CMD_TIMING_2N      2
#define CMD_TIMING_3N      3
#define CMD_TIMING_DEFAULT CMD_TIMING_AUTO

#define MIN_REFRESH_RATE     0
#define MAX_REFRESH_RATE     32767
#define REFRESH_RATE_DEFAULT MIN_REFRESH_RATE

#define MIN_CAS_LATENCY      0
#define MAX_CAS_LATENCY      32
#define CAS_LATENCY_DEFAULT  MIN_CAS_LATENCY

#define MIN_TRP_LATENCY      0
#define MAX_TRP_LATENCY      32
#define TRP_LATENCY_DEFAULT  MIN_TRP_LATENCY

#define MIN_TRCD_LATENCY     0
#define MAX_TRCD_LATENCY     32
#define TRCD_LATENCY_DEFAULT MIN_TRCD_LATENCY

#define MIN_TRAS_LATENCY     0
#define MAX_TRAS_LATENCY     63
#define TRAS_LATENCY_DEFAULT MIN_TRAS_LATENCY

#define MIN_TWR_LATENCY      0
#define MAX_TWR_LATENCY      50
#define TWR_LATENCY_DEFAULT  MIN_TWR_LATENCY

#define MIN_TRFC_LATENCY     0
#define MAX_TRFC_LATENCY     1023
#define TRFC_LATENCY_DEFAULT MIN_TRFC_LATENCY

#define MIN_TRRD_LATENCY     0
#define MAX_TRRD_LATENCY     255
#define TRRD_LATENCY_DEFAULT MIN_TRRD_LATENCY

#define MIN_TRRDL_LATENCY     0
#define MAX_TRRDL_LATENCY     255
#define TRRDL_LATENCY_DEFAULT MIN_TRRDL_LATENCY

#define MIN_TRTP_LATENCY     0
#define MAX_TRTP_LATENCY     255
#define TRTP_LATENCY_DEFAULT MIN_TRTP_LATENCY

#define MIN_TWTR_LATENCY     0
#define MAX_TWTR_LATENCY     255
#define TWTR_LATENCY_DEFAULT MIN_TWTR_LATENCY

#define MIN_TFAW_LATENCY     0
#define MAX_TFAW_LATENCY     63
#define TFAW_LATENCY_DEFAULT MIN_TFAW_LATENCY

#define MIN_TRC_LATENCY      0
#define MAX_TRC_LATENCY      255
#define TRC_LATENCY_DEFAULT  MIN_TRC_LATENCY

#define MIN_TCWL_LATENCY     0
#define MAX_TCWL_LATENCY     31
#define TCWL_LATENCY_DEFAULT MIN_TCWL_LATENCY

//
// Volatile Memory Mode
//
#define VOL_MEM_MODE_1LM            0
#define VOL_MEM_MODE_2LM            1
#define VOL_MEM_MODE_MIX_1LM2LM     2
#if defined (SPR_HOST) || defined(CPX_HOST)
#define VOL_MEM_MODE_DEFAULT        VOL_MEM_MODE_1LM
#else
#define VOL_MEM_MODE_DEFAULT        VOL_MEM_MODE_2LM
#endif

//
// DDR Cache Size
//
#define DDR_ENCODED_CACHE_SIZE_8GB          0
#define DDR_ENCODED_CACHE_SIZE_16GB         1
#define DDR_ENCODED_CACHE_SIZE_32GB         2
#define DDR_ENCODED_CACHE_SIZE_64GB         3
#define DDR_ENCODED_CACHE_SIZE_128GB        4
#define DDR_ENCODED_CACHE_SIZE_256GB        5
#define DDR_ENCODED_CACHE_SIZE_ALL          0xFF
#define DDR_ENCODED_CACHE_SIZE_DEFAULT      DDR_ENCODED_CACHE_SIZE_ALL

#define CACHE_TYPE_DDR_CACHE_PMEM 0
#define CACHE_TYPE_HBM_CACHE_DDR  1
#define CACHE_TYPE_DEFAULT  CACHE_TYPE_DDR_CACHE_PMEM

//
// Interleave Granularity 1LM
//
//
// 256B Target Granularity, 256B Channel Granularity
//
#define MEM_INT_GRAN_1LM_256B_256B            1
//
// 64B Target Granularity, 64B Channel Granularity
//
#define MEM_INT_GRAN_1LM_64B_64B              2
//
// 4KB Target Granularity, 256B Channel Granularity
// 4KB granularity across CPU sockets, 256B granularity across MCs within sockets,
// 256B granularity across channels within MCs
//
#define MEM_INT_GRAN_1LM_4KB_256B_256B        3
//
// Default 1LM Granularity
//
#if (MAX_IMC > 2)
//
// ICX_HOST or SPR_HOST
//
#define MEM_INT_GRAN_1LM_DEFAULT              MEM_INT_GRAN_1LM_4KB_256B_256B
#else
#define MEM_INT_GRAN_1LM_DEFAULT              MEM_INT_GRAN_1LM_256B_256B
#endif

//
// Channel interleave granularity for DCPMM
//
#define MEM_INT_GRAN_CH_256B_TGT_4KB          1
#define MEM_INT_GRAN_CH_4KB_TGT_4KB           2
#define MEM_INT_GRAN_DDRT_DEFAULT             MEM_INT_GRAN_CH_4KB_TGT_4KB

// HBM Cache Enable
//
#define HBM_CACHE_ENABLE                      1
#define HBM_CACHE_DISABLE                     0
#define HBM_CACHE_ENABLE_DEFAULT              HBM_CACHE_DISABLE

//
// HBM Cache Ratio
//
#define HBM_CACHE_RATIO_MIN                   0   // minimum ratio is 1/mx where mx = (total HBM size)/1GB
#define HBM_CACHE_RATIO_3_125_PERCENT         1   // 3.125% ( 1/32) is cache (invalid input; use HBM_CACHE_RATIO_MIN)
#define HBM_CACHE_RATIO_6_25_PERCENT          2   // 6.25%  ( 2/32) is cache (invalid input; use HBM_CACHE_RATIO_MIN)
#define HBM_CACHE_RATIO_12_5_PERCENT          4   // 12.5%  ( 4/32) is cache (invalid input; use HBM_CACHE_RATIO_MIN)
#define HBM_CACHE_RATIO_25_PERCENT            8   // 25%    ( 8/32) is cache
#define HBM_CACHE_RATIO_50_PERCENT            16  // 50%    (16/32) is cache
#define HBM_CACHE_RATIO_93_75_PERCENT         30  // 93.75% (30/32) is cache (invalid input; only for bank sparing)
#define HBM_CACHE_RATIO_100_PERCENT           32  // 100%   (32/32) is cache
#define HBM_CACHE_RATIO_DIVISOR               32  // multiply by one of the above, divide by this (not used in setup)
#define HBM_CACHE_RATIO_DEFAULT               HBM_CACHE_RATIO_100_PERCENT

//
// HBM Cache Far Memory Type
//
#define HBM_CACHE_FM_TYPE_DDR4                0
#define HBM_CACHE_FM_TYPE_DDRT                1
#define HBM_CACHE_FM_TYPE_AUTO                2
#define HBM_CACHE_FM_TYPE_DEFAULT             HBM_CACHE_FM_TYPE_AUTO

//
// HBM Far Memory Channel Granularity for DDRT
//
#define HBM_CACHE_DDRT_GRAN_256B              0
#define HBM_CACHE_DDRT_GRAN_4KB               1
#define HBM_CACHE_DDRT_GRAN_DEFAULT           HBM_CACHE_DDRT_GRAN_256B

//
// Route table mode definition
//
#define ROUTE_TABLE_MODE_SINGLE_TARGET        0
#define ROUTE_TABLE_MODE_2WAY                 1
#define ROUTE_TABLE_MODE_MODULO_3             2
#define ROUTE_TABLE_MODE_4WAY                 3
#define ROUTE_TABLE_MODE_8WAY                 6

//
// IMC Interleave
//
#define IMC_1WAY                1
#define IMC_2WAY                2
#define IMC_3WAY                3
#define IMC_4WAY                4
#define IMC_6WAY                6
#define IMC_8WAY                8
#if (MAX_IMC == 8)
#define IMC_INTER_DEFAULT       IMC_8WAY
#elif (MAX_IMC == 6)
#define IMC_INTER_DEFAULT       IMC_6WAY
#elif (MAX_IMC == 4)
#define IMC_INTER_DEFAULT       IMC_4WAY
#elif (MAX_IMC == 2)
#define IMC_INTER_DEFAULT       IMC_2WAY
#else
#define IMC_INTER_DEFAULT       IMC_1WAY
#endif

//
// Channel Interleave
//
#define CH_1WAY               1
#define CH_2WAY               2
#define CH_3WAY               3
#define CH_4WAY               4
#define CH_INTER_DEFAULT      CH_3WAY

//
// Rank Interleave
//
#define RANK_1WAY             1
#define RANK_2WAY             2
#define RANK_4WAY             4
#define RANK_8WAY             8

//
//
// Dimm Interleaving
#define DIMM_INTER_ENABLE       1
#define DIMM_INTER_DISABLE      0
#define DIMM_INTER_DEFAULT      DIMM_INTER_ENABLE

//
// Mod3 Support for 3-way Channel Interleave
//
#define MOD3_SUPPORT_ENABLE    1
#define MOD3_SUPPORT_DISABLE   0
#define MOD3_SUPPORT_DEFAULT   MOD3_SUPPORT_ENABLE

//
// TDP POWER_10W
//
#define TDP_POWER_10W     10
#define TDP_POWER_12W     12
#define TDP_POWER_15W     15
#define TDP_POWER_18W     18
#define TDP_POWER_DEFAULT TDP_POWER_12W

//
// SMBIOS Extended Type 17 Structure Support
//
#define EXTENDED_TYPE_17_DISABLE    0
#define EXTENDED_TYPE_17_ENABLE     1
#define EXTENDED_TYPE_17_DEFAULT    EXTENDED_TYPE_17_DISABLE

//
// NVDIMM SMBus interface for UEFI CR MGMT Driver settings (time unit is ms)
//
#define NVDIMM_SMBUS_ACCESS_RELEASE_DELAY_MS_DEFAULT 150
#define NVDIMM_SMBUS_MAX_ACCESS_TIME_MS_DEFAULT      350

//
// LOCK NGN
//
#define LOCK_NGN_CSR_ENABLE       0
#define LOCK_NGN_CSR_DISABLE      1
#define LOCK_NGN_CSR_DEFAULT      LOCK_NGN_CSR_ENABLE   // Setupdefault for lockNgnCsr

#define NGN_FACTORY_RESET_CLEAR_DISABLE      0
#define NGN_FACTORY_RESET_CLEAR_ENABLE       1
#define NGN_FACTORY_RESET_CLEAR_DEFAULT      NGN_FACTORY_RESET_CLEAR_DISABLE   // Setupdefault for reset/clear

//
// NGN DEBUG LOCK
//
#define NGN_DEBUG_LOCK_DISABLE   0
#define NGN_DEBUG_LOCK_ENABLE    1
#define NGN_DEBUG_LOCK_AUTO      2
#ifdef CWV_A0_REMOVE_POST_PO
#define NGN_DEBUG_LOCK_DEFAULT   NGN_DEBUG_LOCK_ENABLE // Setupdefault for ngnDebugLock
#else
#define NGN_DEBUG_LOCK_DEFAULT   NGN_DEBUG_LOCK_DISABLE // Setupdefault for ngnDebugLock
#endif
//
// NGN CMD
//
#define NGN_CMD_1N                0
#define NGN_CMD_2N                1
#define NGN_CMD_TIME_AUTO         2
#define NGN_CMD_TIME_DEFAULT      NGN_CMD_TIME_AUTO             // Setupdefault for ngnCmdTime

//
// NGN ECC CORR
//
#define NGN_ECC_CORR_DISABLE      0
#define NGN_ECC_CORR_ENABLE       1
#define NGN_ECC_CORR_AUTO         2
#define NGN_ECC_CORR_DEFAULT      NGN_ECC_CORR_DISABLE   // Setupdefault for ngnEccCorr

//
// NGN ECC WR CHK
//
#define NGN_ECC_WR_CHK_DISABLE    0
#define NGN_ECC_WR_CHK_ENABLE     1
#define NGN_ECC_WR_CHK_AUTO       2
#define NGN_ECC_WR_CHK_DEFAULT    NGN_ECC_WR_CHK_ENABLE // Setupdefault for ngnEccWrChk

//
// NGN ECC EXIT CORR
//
#define NGN_ECC_EXIT_CORR_DISABLE  0
#define NGN_ECC_EXIT_CORR_ENABLE   1
#define NGN_ECC_EXIT_CORR_AUTO     2
#define NGN_ECC_EXIT_CORR_DEFAULT  NGN_ECC_EXIT_CORR_DISABLE // Setupdefault for ngnEccExitCorr

//
// NGN ECC RD CHK
//
#define NGN_ECC_RD_CHK_DISABLE    0
#define NGN_ECC_RD_CHK_ENABLE     1
#define NGN_ECC_RD_CHK_AUTO       2
#define NGN_ECC_RD_CHK_DEFAULT    NGN_ECC_RD_CHK_ENABLE // Setupdefault for ngnEccRdChk

//
// Secure Erase Unit
//
#define DISABLE_SECURE_ERASE 0
#define ENABLE_SECURE_ERASE  1

#define SECURE_ERASE_ALL_DEFAULT DISABLE_SECURE_ERASE

#define SECURE_ERASE_SKT0_CH0_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT0_CH1_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT0_CH2_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT0_CH3_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT0_CH4_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT0_CH5_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT0_CH6_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT0_CH7_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT1_CH0_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT1_CH1_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT1_CH2_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT1_CH3_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT1_CH4_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT1_CH5_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT1_CH6_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT1_CH7_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT2_CH0_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT2_CH1_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT2_CH2_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT2_CH3_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT2_CH4_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT2_CH5_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT2_CH6_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT2_CH7_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT3_CH0_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT3_CH1_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT3_CH2_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT3_CH3_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT3_CH4_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT3_CH5_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT3_CH6_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT3_CH7_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT4_CH0_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT4_CH1_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT4_CH2_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT4_CH3_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT4_CH4_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT4_CH5_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT4_CH6_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT4_CH7_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT5_CH0_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT5_CH1_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT5_CH2_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT5_CH3_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT5_CH4_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT5_CH5_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT5_CH6_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT5_CH7_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT6_CH0_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT6_CH1_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT6_CH2_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT6_CH3_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT6_CH4_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT6_CH5_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT6_CH6_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT6_CH7_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT7_CH0_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT7_CH1_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT7_CH2_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT7_CH3_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT7_CH4_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT7_CH5_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT7_CH6_DEFAULT DISABLE_SECURE_ERASE
#define SECURE_ERASE_SKT7_CH7_DEFAULT DISABLE_SECURE_ERASE

//
// Biased 2-way near memory cache support
//
#define TWO_WAY_NM_CACHE_DISABLED       0
#define TWO_WAY_NM_CACHE_ENABLED        1
#define TWO_WAY_NM_CACHE_DEFAULT        TWO_WAY_NM_CACHE_DISABLED

#define NON_PREFERRED_WAY_MASK_MIN      0x0
#define NON_PREFERRED_WAY_MASK_MAX      0x3FF
#define NON_PREFERRED_WAY_MASK_DEFAULT  0x01

#define NON_PREFERRED_WAY_READ_FIRST    0
#define PREFERRED_WAY_READ_FIRST        1
#define PREFERRED_READ_FIRST_DEFAULT    PREFERRED_WAY_READ_FIRST

//
// Boot-time fast zero memory
//
#define FAST_ZERO_MEM_DISABLED          0
#define FAST_ZERO_MEM_ENABLED           1
#if defined(ICX_HOST) || defined(SKX_HOST) || defined(CWV_A0_REMOVE_POST_PO)
#define FAST_ZERO_MEM_DEFAULT           FAST_ZERO_MEM_DISABLED
#else
#define FAST_ZERO_MEM_DEFAULT           FAST_ZERO_MEM_ENABLED
#endif

//
// App Direct Memory Hole default set by PcdMrcPmemMemHoleDefault
//
#define PMEM_MEM_HOLE_DISABLE   0
#define PMEM_MEM_HOLE_ENABLE    1

// MemorySetup.hfr finishes

#define TEMPHIGH_EN           BIT0
#define PDWN_MODE_APD         BIT1
#define PDWN_MODE_PPD         BIT2
#define PDWN_MODE_SLOW_EXIT   BIT3
#define PDWN_MODE_IBT_OFF_EN  BIT4
#define PDWN_IDLE_CNTR        0x40

//
// RAS Mode
//
#define MIRROR_EN             BIT0    // Mirror Mode
#define LOCKSTEP_EN           BIT1    // Lockstep Mode

#define CH_ALL_MIRROR         (FULL_MIRROR_1LM | FULL_MIRROR_2LM | PARTIAL_MIRROR_1LM | PARTIAL_MIRROR_2LM)

//
// MC_ODT
//
#define MC_ODT_50_OHMS        0
#define MC_ODT_100_OHMS       1
#define MC_ODT_AUTO           2
#define MC_ODT_DEFAULT        MC_ODT_50_OHMS

//
// Memory Power Savings Mode
//
#define MEM_PWR_SAV_DISABLE   0       // Disable the PPM control and force this to be off all the time
#define MEM_PWR_SAV_SLOW      1       // PPDS with processor control of parameters
#define MEM_PWR_SAV_FAST      2       // PPDF with processor control of parameters
#define MEM_PWR_SAV_USER      3       // Disable processor control of parameters, set values manually
#define MEM_PWR_SAV_AUTO      4
#define MEM_PWR_SAV_DEFAULT_DDR4   MEM_PWR_SAV_USER
#define MEM_PWR_SAV_DEEP      1
#define MEM_PWR_SAV_SHALLOW   2
#define MEM_PWR_SAV_DEFAULT_DDRT   MEM_PWR_SAV_DEEP

//
// CKE Throttling
//
#define CKE_MODE_OFF       0       // CKE Throttling Disabled
#define CKE_APD            1       // Advanced Power Down Enabled, Pre-charge Power Down Disabled
#define CKE_PPDF           2       // Advanced Power Down Disabled, Pre-charge Power Down Fast Enabled
#define CKE_PPDS           3       // Advanced Power Down Disabled, Pre-charge Power Slow Enabled
#define CKE_APD_PPDF       4       // Advanced Power Down Enabled, Pre-charge Power Down Enabled
#define CKE_APD_PPDS       5       // Advanced Power Down Enabled, Pre-charge Power Slow Enabled
#define CKE_MODE_AUTO      6
#define CKE_MODE_DEFAULT   CKE_APD

//
// MDLL Shut Down Enable
//
#define MDLL_SDEN_DIS         0
#define MDLL_SDEN_EN          1
#define MDLL_SDEN_AUTO        2
#define MDLL_SDEN_DEFAULT     MDLL_SDEN_DIS


#define CLTT_THRT_MID_DEFAULT 255
#define CLTT_THRT_HI_DEFAULT 26   // = 255 * 10% * 1 rounded up
#define CLTT_THRT_HI_DDRT_DEFAULT 1
#define CLTT_THRT_CRIT_DEFAULT 3  // 0.1GB/s/DIMM

#define BW_LIMIT_TF_DDRT_DEFAULT  600  //HSD s5371498


#define DIMMTEMP_THRT_CRIT_DEFAULT 1  // 0.1GB/s/DIMM //Same value for CLTT\OLTT
#define TEMP_HI_AEP 85              //Same value for CLTT\OLTT

//3DS Workaround value
#define TEMP_3DS_DIMM_4H_16GB_LOW 82
#define TEMP_3DS_DIMM_4H_16GB_MID 83

#define TT_MAX_WORST_CASE_DIMM_TEMP_OFFSET 15

// Use 3 celsius TS offset values for DDR5 module
#define TT_DIMM_TEMP_OFFSET_DDR5 3

//Based on Whitley CLTT 612245 0.7 whitepaper
#define OLTT_STATIC_DIMM_TEMP_VALUE  84 //This will force Throttle High always
#define OLTT_TEMP_LO_DEFAULT 82
#define OLTT_TEMP_MID_DEFAULT 83
#define OLTT_TEMP_HI_DEFAULT 95
#define OLTT_THRT_MID_DEFAULT 255
#define OLTT_THRT_HI_DEFAULT 204   // = 255 * 80% * 1 rounded up


//
// Dram Power Table Default Values
//
#define UDIMM_MAX_POWER_DEFAULT 93
#define UDIMM_DRAM_TDP_DEFAULT 47
#define UDIMM_MIN_PWR_CLOSED_PAGE_DEFAULT 15
#define UDIMM_WRITE_PWR_SCALE_CLOSED_PAGE_DEFAULT 943
#define UDIMM_MAX_PWR_OPEN_PAGE_DEFAULT 88
#define UDIMM_TDP_OPEN_PAGE_DEFAULT 49
#define UDIMM_MIN_PWR_OPEN_PAGE_DEFAULT 18
#define UDIMM_WRITE_PWR_SCALE_OPEN_PAGE_DEFAULT 1066
#define UDIMM_REFRESH_RATE_SLOPE_DEFAULT 1663
#define UDIMM_SELF_REFRESH_POWER_DEFAULT 584

#define LRDIMM_MAX_POWER_DEFAULT 212
#define LRDIMM_DRAM_TDP_DEFAULT 141
#define LRDIMM_MIN_PWR_CLOSED_PAGE_DEFAULT 52
#define LRDIMM_WRITE_PWR_SCALE_CLOSED_PAGE_DEFAULT 466
#define LRDIMM_MAX_PWR_OPEN_PAGE_DEFAULT 203
#define LRDIMM_TDP_OPEN_PAGE_DEFAULT 157
#define LRDIMM_MIN_PWR_OPEN_PAGE_DEFAULT 75
#define LRDIMM_WRITE_PWR_SCALE_OPEN_PAGE_DEFAULT 506
#define LRDIMM_REFRESH_RATE_SLOPE_DEFAULT 6650
#define LRDIMM_SELF_REFRESH_POWER_DEFAULT 2332

#define RDIMM_MAX_POWER_DEFAULT 191
#define RDIMM_DRAM_TDP_DEFAULT 116
#define RDIMM_MIN_PWR_CLOSED_PAGE_DEFAULT 49
#define RDIMM_WRITE_PWR_SCALE_CLOSED_PAGE_DEFAULT 710
#define RDIMM_MAX_PWR_OPEN_PAGE_DEFAULT 182
#define RDIMM_TDP_OPEN_PAGE_DEFAULT 137
#define RDIMM_MIN_PWR_OPEN_PAGE_DEFAULT 72
#define RDIMM_WRITE_PWR_SCALE_OPEN_PAGE_DEFAULT 776
#define RDIMM_REFRESH_RATE_SLOPE_DEFAULT 6650
#define RDIMM_SELF_REFRESH_POWER_DEFAULT 2369

#define DRAM_RAPL_REFRESH_BASE_DEFAULT 10

//
// DRAM RAPL Extended Range Options
//
#define DRAM_RAPL_EXTENDED_RANGE_DISABLE 0
#define DRAM_RAPL_EXTENDED_RANGE_ENABLE 1
#define DRAM_RAPL_EXTENDED_RANGE_DEFAULT DRAM_RAPL_EXTENDED_RANGE_ENABLE

//
// Set DramRaplEnable Default
//
#define DRAM_RAPL_DISABLE    0
#define DRAM_RAPL_ENABLE     1
#if defined (DDR5_SUPPORT) && !defined (SPR_HOST)
#define DRAM_RAPL_DEFAULT    DRAM_RAPL_DISABLE
#else
#define DRAM_RAPL_DEFAULT    DRAM_RAPL_ENABLE
#endif

//
// Default active idle dimm value
//
#define DEFAULT_ACTIVE_IDLE_DIMM_VALUE 5

//
// UDIMM default active idle dimm value
//
#define DEFAULT_UDIMM_ACTIVE_IDLE_DIMM_VALUE 8

//
// RDIMM default active idle dimm value
//
#define DEFAULT_RDIMM_ACTIVE_IDLE_DIMM_VALUE 8

//
// LRDIMM default active idle dimm value
//
#define DEFAULT_LRDIMM_ACTIVE_IDLE_DIMM_VALUE 8
//
// Default power ODT Count dimm value
//
#define DEFAULT_PWR_ODT_CNT_DIMM_VALUE 16
//
// Default TSOD Poll rate
//
#define DEFAULT_TSOD_POLL_RATE        0x3E800

// Disable TSOD polling
//
#define SMBUS_TSOD_DISABLE            0

//
// Default Temperature thershold low
//
#define DEFAULT_TEMP_THERSHOLD_LOW      0x55

//
// Default Temperature thershold mid
//
#define DEFAULT_TEMP_THERSHOLD_MID      0x5A

//
// Default Temperature thershold high
//
#define DEFAULT_TEMP_THERSHOLD_HIGH     0x5F

//
// Default Temperature throttle mid
//
#define DEFAULT_TEMP_THROTTLE_MID       0XFF

//
// Default Temperature throttle high
//
#define DEFAULT_TEMP_THROTTLE_HIGH      0XF

//
// Default Temperature throttle critical
//
#define DEFAULT_TEMP_THROTTLE_CRITICAL   0X0

//
// 1.2V Voltage for Memweight Table
//
#define DEFAULT_MEMWEIGHT_VOLTAGE        0x40

//
// Enable TSOD polling
//
#define SMBUS_TSOD_ENABLE             1

//
// Channel Max Mempower for UDIMM
//
#define CHANNEL_MAX_MEM_PWR_UDIMM         126

//
// Channel Max Mempower for UDIMM ECC
//
#define CHANNEL_MAX_MEM_PWR_UDIMM_ECC     114

//
// Channel Max Mempower for LRDIMM
//
#define CHANNEL_MAX_MEM_PWR_LRDIMM        120

//
// Channel Max Mempower for RDIMM
//
#define CHANNEL_MAX_MEM_PWR_RDIMM         113

//
// Channel SelfRefresh power for UDIMM
//
#define CHANNEL_SEFL_REFRESH_PWR_UDIMM         2679

//
// Channel SelfRefresh power for UDIMM ECC
//
#define CHANNEL_SEFL_REFRESH_PWR_UDIMM_ECC     2679

//
// Channel SelfRefresh power for LRDIMM
//
#define CHANNEL_SEFL_REFRESH_PWR_LRDIMM        2679

//
// Channel SelfRefresh power for RDIMM
//
#define CHANNEL_SEFL_REFRESH_PWR_RDIMM         1782

//
// UDIMM power Coeff value
//
#define UDIMM_POWER_COEFF_DEFAULT 113

//
// LRDIMM power Coeff value
//
#define LRDIMM_POWER_COEFF_DEFAULT 336

//
// UDIMM power Coeff value
//
#define RDIMM_POWER_COEFF_DEFAULT 251

#define CR_ACPI_DEBUG_INTERFACE_ACPI    1
#define CR_ACPI_DEBUG_INTERFACE_COM1    2
#define CR_ACPI_DEBUG_INTERFACE_DEFAULT CR_ACPI_DEBUG_INTERFACE_ACPI

#define NFIT_PUBLISH_MAILBOX_STRUCTS_DISABLE 0
#define NFIT_PUBLISH_MAILBOX_STRUCTS_ENABLE  1
#define NFIT_PUBLISH_MAILBOX_STRUCTS_DEFAULT NFIT_PUBLISH_MAILBOX_STRUCTS_DISABLE

//
// DDRT Scheduler logic debug - default set by PcdDdrtSchedulerDebugDefault
//
#define DDRT_SCHEDULER_DBG_DISABLE  0
#define DDRT_SCHEDULER_DBG_ENABLE   1

//
// Platform specific value to be programmed into the TSOD CONFIG register
//
//#define OEM_MTS_CONFIG_VALUE    MTS_CFG_TCRIT_ONLY + MTS_CFG_EVENT_CTRL
//#define OEM_MTS_CONFIG_VALUE    0   //replaced by PCD

//
// Electrical Throttling Options
//
#define ET_DISABLE  0
#define ET_ENABLE   1
#define ET_AUTO     2
// TODO: Make sure this is disabled for PO

//
// High Temperature Enable
//
#define HTE_DISABLE  0
#define HTE_ENABLE   1
#define HTE_AUTO     2
#define HTE_DEFAULT  HTE_DISABLE

//
// Allow 2X Refresh enable
//
#define A2R_DISABLE  0
#define A2R_ENABLE   1
#define A2R_AUTO     2
#define A2R_DEFAULT  A2R_ENABLE

//
// DDR5 refresh operation
//
#define DDR5_REFRESH_MODE_AB_NORMAL 0
#define DDR5_REFRESH_MODE_AB_FINE   1
#define DDR5_REFRESH_MODE_SB_FINE   2
#define DDR5_REFRESH_MODE_AUTO      3

//
// DDR5 SSA RxSampler Margin Parameter Selection
//
#define DDR5_SSA_RXSAMPLER_EVEN_ODD    0
#define DDR5_SSA_RXSAMPLER_EVEN_ONLY   1
#define DDR5_SSA_RXSAMPLER_ODD_ONLY    2
#define DDR5_SSA_RXVREF                3
#define DDR5_SSA_RXSAMPLER_AUTO        4

//
// Altitude
//
#define UNKNOWN   0
#define ALT_300M  1
#define ALT_900M  2
#define ALT_1500M 3
#define ALT_3000M 4

// Memory Type
#define RDIMM_ONLY            0       // RDIMMs only
#define UDIMM_ONLY            1       // UDIMMs (and SODIMMs) only
#define RDIMMandUDIMM         2       // Both RDIMMs and UDIMMs supported
#define MEM_TYPE_AUTO         3
#define MEM_TYPE_DEFAULT      RDIMMandUDIMM

// DDR3 Voltage
#define DDR3_VDD_AUTO         0
#define DDR3_VDD_150          1
#define DDR3_VDD_135          2
#define DDR3_VDD_DEFAULT      DDR3_VDD_AUTO

//Page Table Aliasing -  option of choosing which bit to use for page table aliasing - either bit 1 or 2.
#define PAGE_TBL_AL_SET0               0       // if set to 0, alias will occur on bit 1.
#define PAGE_TBL_AL_SET1               1       // If set to 1, alias will occur with bit 2
#define PAGE_TBL_AL_AUTO               2
#define PAGE_TBL_AL_DEFAULT            PAGE_TBL_AL_SET0

// Software Memory test enable
#define SW_MEM_TEST_DIS    0          // Disable memory test
#define SW_MEM_TEST_EN     1          // Enable memory test
#define SW_MEM_TEST_AUTO   SW_MEM_TEST_DIS

//
// Advanced MemTest definitions
//
// Setting:                          Value:
// ADV_MT_DIS                         0x0000
// ADV_MT_XMATS8                      0x0001
// ADV_MT_XMATS16                     0x0002
// ADV_MT_XMATS32                     0x0004
// ADV_MT_XMATS64                     0x0008
// ADV_MT_WCMATS8                     0x0010
// ADV_MT_WCMCH8                      0x0020
// ADV_MT_GNDB64                      0x0040
// ADV_MT_MARCHCM64                   0x0080
// ADV_MT_LTEST_SCRAM                 0x0100
// ADV_MT_RANGE_TEST_SCRAM            0x0400
// ADV_MT_TWR                         0x0800
// ADV_MT_DATA_RET                    0x1000
// ADV_MT_MATS8_TC1                   0x2000
// ADV_MT_MATS8_TC2                   0x4000
// ADV_MT_MATS8_TC3                   0x8000
//
#define ADV_MEM_TEST_MAX             0xFFFF
#define ADV_MEM_TEST_OPTIONS_DEFAULT 0x0000

#define ADV_MEM_TEST_RESET_LIST_DEFAULT      0

#define ADV_MEM_TEST_COND_DISABLE       0
#define ADV_MEM_TEST_COND_AUTO          1
#define ADV_MEM_TEST_COND_MANUAL        2
#define ADV_MEM_TEST_COND_DEFAULT      ADV_MEM_TEST_COND_AUTO

#define ADV_MEM_TEST_COND_VDD_MIN           1080        // units = mV
#define ADV_MEM_TEST_COND_VDD_MAX           1320        // allow max/min margining range of +/-10% of VDD = 1.2V
#define ADV_MEM_TEST_COND_VDD_DEFAULT       1260

#define ADV_MEM_TEST_COND_TWR_MIN           10          // units = tCK
#define ADV_MEM_TEST_COND_TWR_MAX           26
#define ADV_MEM_TEST_COND_TWR_DEFAULT       10

#define ADV_MEM_TEST_COND_TREFI_MIN         3900        // units = nsec
#define ADV_MEM_TEST_COND_TREFI_MAX         32767
#define ADV_MEM_TEST_COND_TREFI_DEFAULT     15600

#define ADV_MEM_TEST_COND_PAUSE_MIN         0           // units = usec
#define ADV_MEM_TEST_COND_PAUSE_MAX         512000
#define ADV_MEM_TEST_COND_PAUSE_DEFAULT     100000

//
// Refresh parameter timing constraints
//
#define OREFNI                63
#define REF_HI_WM             8
#define REF_PANIC_WM          9
#define REF_HI_WM_LOW         1
#define REF_PANIC_WM_LOW      2

// Default value is controlled by Fixed PCD PcdMrcPanicWatermarkDefault
#define REFRESH_WM_AUTO         0
#define REFRESH_WM_HIGH         1
#define REFRESH_WM_LOW          2

//
// Refresh parameter timing constraints specific for DDR5
//
#define REF_HI_WM_DDR5          4
#define REF_PANIC_WM_DDR5       5

//
// For DDR5, if total rank number per channel <= 8, set per bank mode
//
#define PERBANK_MODE_DDR5_MAX_RANK_CH  8

//
// ZQ Calibration Timing Parameter
//
#define ZQCS_PERIOD           128

//
// Self-refresh idle counter
//
#define SREF_IDLE_CNTR        0x3e800

//
// NUMA Mode
//
#define NUMA_DIS              0
#define NUMA_EN               1
#define NUMA_AUTO             2
#define NUMA_DEFAULT          NUMA_EN

//
// Bank Group Interleaving Mode
//
#define BANK_GROUP_INTERLEAVING_COARSE             0  // BANK_INT_6_10_11_13 (HBM) CGBG (DDR4) (Coarse grained bank group interleave)
#define BANK_GROUP_INTERLEAVING_FINE               1  // 1=BANK_INT_7_10_11_13 (HBM) FGBG_INT_2 (DDR4/5) (Fine grained bank group interleave of one bank bit)
#define BANK_GROUP_INTERLEAVING_FINE_TWO_BANK_BITS       2  // BANK_INT_8_10_11_13 (HBM) or FGBG_INT_4 (DDR4) or FGBG_INT_8 (DDR5) (Fine grained bank group interleave on two bank bits)
#define BANK_GROUP_INTERLEAVING_HBM_BANK_INT_8_9_11_13   3
#define BANK_GROUP_INTERLEAVING_HBM_BANK_INT_6_11_12_13  4
#define BANK_GROUP_INTERLEAVING_HBM_BANK_INT_7_11_12_13  5
#define BANK_GROUP_INTERLEAVING_HBM_BANK_INT_7_8_9_10    6
#define BANK_GROUP_INTERLEAVING_HBM_BANK_INT_10_11_12_13 7
#define BANK_GROUP_INTERLEAVING_DEFAULT BANK_GROUP_INTERLEAVING_FINE

#define DIMM_MGMT_BIOS_SETUP      0
#define DIMM_MGMT_CR_MGMT_DRIVER  1
#define DIMM_MGMT_DEFAULT         DIMM_MGMT_CR_MGMT_DRIVER // if this value changes, suppress statements in MemoryDfxSetup.hfr must be updated

#define CMD_FUB_DISABLE 0
#define CMD_FUB_ENABLE  1

//
// Option to disable\enable silicon workaround S1409718810
// Reduce TME read completion fabric credits when TME is enabled
//
#define TME_CMI_CPL_FAB_CREDIT_WORKAROUND_DISABLE      0
#define TME_CMI_CPL_FAB_CREDIT_WORKAROUND_ENABLE       1
#define TME_CMI_CPL_FAB_CREDIT_WORKAROUND_DEFAULT      TME_CMI_CPL_FAB_CREDIT_WORKAROUND_ENABLE


//DDRT partition ratios
#define RATIO_50_50_0   0
#define RATIO_50_0_50   1
#define RATIO_0_50_50   2
#define RATIO_100_0_0   3
#define RATIO_0_100_0   4
#define RATIO_0_0_100   5
#define RATIO_DEFAULT   RATIO_50_50_0

//HighAddressBitStartPosition
#define HIGH_ADDR_START_BIT_POSITION_33 33
#define HIGH_ADDR_START_BIT_POSITION_34 34
#define HIGH_ADDR_START_BIT_POSITION_35 35
#define HIGH_ADDR_START_BIT_POSITION_36 36
#define HIGH_ADDR_START_BIT_POSITION_37 37
#define HIGH_ADDR_START_BIT_POSITION_38 38
#define HIGH_ADDR_START_BIT_POSITION_39 39
#define HIGH_ADDR_START_BIT_POSITION_40 40
#define HIGH_ADDR_START_BIT_POSITION_41 41
#define HIGH_ADDR_START_BIT_POSITION_42 42
#define HIGH_ADDR_START_BIT_POSITION_43 43
#define HIGH_ADDR_START_BIT_POSITION_44 44
#define HIGH_ADDR_START_BIT_POSITION_45 45
#define HIGH_ADDR_START_BIT_POSITION_46 46
#define HIGH_ADDR_START_BIT_POSITION_47 47
#define HIGH_ADDR_START_BIT_POSITION_48 48
#define HIGH_ADDR_START_BIT_POSITION_49 49
#define HIGH_ADDR_START_BIT_POSITION_50 50
#define HIGH_ADDR_START_BIT_POSITION_51 51

//
// partition DDRT DIMM
//
#define PARTITION_DDRT_DIMM_DIS               0
#define PARTITION_DDRT_DIMM_EN                1

#define SCK_GRAN_256B     1
#define CH_GRAN_256B      1

#define MAX_AEP_DIMM_SETUP 48 //(MAX_CH * MAX_SOCKET * MAX_DCPMM_CH)

//
// Persistent Memory Mode
//
#define NON_PER_MEM_MODE    0
#define PER_MEM_MODE        1
#define MEM_MODE_DEFAULT    PER_MEM_MODE // if this value changes, suppress statements in MemorySetup.hfr must be updated

//
// Pmem/AppDirect WB caching
//
#define PMEM_WBCACHE_DISABLED 0
#define PMEM_WBCACHE_ENABLED  1
#define PMEM_WBCACHE_ENABLE_DEFAULT PMEM_WBCACHE_DISABLED

//
// eADR support Status
//
#define EADR_DISABLED 0
#define EADR_ENABLED  1
#define EADR_AUTO     2
#define EADR_DEFAULT  EADR_DISABLED

//
// eADR Flush Mode
//
#define EADR_FLUSH_SERIAL   0
#define EADR_FLUSH_PARALLEL 1

//
// eADR Debug info
//
#define EADR_DEBUG_LOGS_AUTO    0
#define EADR_DEBUG_LOGS_DRIVER  1
#define EADR_DEBUG_LOGS_FLOW    2
#define EADR_DEBUG_LOGS_ALL     3
#define EADR_DEBUG_LOGS_DEFAULT EADR_DEBUG_LOGS_AUTO

//
// fADR support
//
#define FADR_DISABLED 0
#define FADR_ENABLED  1
#define FADR_DEFAULT  FADR_DISABLED

//
// fADR Debug info
//
#define FADR_DEBUG_LOGS_AUTO    0
#define FADR_DEBUG_LOGS_DRIVER  1
#define FADR_DEBUG_LOGS_FLOW    2
#define FADR_DEBUG_LOGS_ALL     3
#define FADR_DEBUG_LOGS_DEFAULT FADR_DEBUG_LOGS_AUTO

//
// Force Persistent Memory Mode Region
//
#define NON_PER_MEM_REGION   0
#define PER_MEM_REGION       1

#define MEM_INT_GRAN_ERROR                    0xFF

//
// Config Mask for 2LM
//
#define CFG_MASK_2LM_NORMAL                   0
#define CFG_MASK_2LM_AGGRESSIVE               1

//
// FIS 1.x Power Management Policy
//
#define NGN_PEAK_POWER_MIN          10000
#define NGN_PEAK_POWER_MAX          20000
#define NGN_PEAK_POWER_DEFAULT      20000
#define NGN_PEAK_POWER_INCREMENT    250
#define NGN_AVERAGE_POWER_MIN       10000
#define NGN_AVERAGE_POWER_MAX       18000
#define NGN_AVERAGE_POWER_DEFAULT   15000
#define NGN_AVERAGE_POWER_INCREMENT 250
//
// FIS Common Power Management Policy for 2.0 and later
//
#define AVERAGE_POWER_LIMIT_MIN                   10000
#define AVERAGE_POWER_LIMIT_MAX                   15000 // Setting Max to highest possible value
#define AVERAGE_POWER_LIMIT_DEFAULT               15000
#define AVERAGE_POWER_LIMIT_INCREMENT             250
#define TURBO_POWER_LIMIT_MIN                     15000
#define TURBO_POWER_LIMIT_MAX                     18000 // Setting Max to highest possible value
#define TURBO_POWER_LIMIT_DEFAULT                 TURBO_POWER_LIMIT_MAX
#define TURBO_MODE_STATE_ENABLED                  1
#define TURBO_MODE_STATE_DISABLED                 0
#define TURBO_MODE_STATE_DEFAULT                  TURBO_MODE_STATE_ENABLED

//
// FIS 2.0 Power Management Policy
//
#define AVERAGE_POWER_TIME_CONSTANT_DEFAULT_2_0   0x06 // Value corresponds to 96 msec

//
// FIS 2.1 or later Power Management Policy
//
#define AVERAGE_POWER_TIME_CONSTANT_MIN_2_1_OR_LATER            1000    // Value corresponds to msec
#define AVERAGE_POWER_TIME_CONSTANT_MAX_2_1_OR_LATER            120000  // Value corresponds to msec
#define AVERAGE_POWER_TIME_CONSTANT_DEFAULT_2_1_OR_LATER        15000   // Value corresponds to 15000 msec
#define AVERAGE_POWER_TIME_CONSTANT_INCREMENT_2_1_OR_LATER      1000

// EDS table(ver 1.7) for Average Power
// DIMM Capacity | Valid Average Power Limit
//    128 GiB    |     10W to 15W
//    256 GiB    |     12W to 18W
//    512 GiB    |     12W to 18W
#define NGN_128GB_AVERAGE_POWER_MIN          10000
#define NGN_128GB_AVERAGE_POWER_MAX          15000
#define NGN_256GB_AVERAGE_POWER_MIN          12000
#define NGN_256GB_AVERAGE_POWER_MAX          18000
#define NGN_512GB_AVERAGE_POWER_MIN          12000
#define NGN_512GB_AVERAGE_POWER_MAX          18000
#define NGN_128GB_AVERAGE_POWER_DEFAULT      15000
#define NGN_256_512GB_AVERAGE_POWER_DEFAULT  15000

//capacity by SPD byte 176 and 178
// 176[3:0] + 178[6:2]
// - 0x100: 64GB
// - 0x120: 128GB
// - 0x124: 256GB
// - 0x12C: 512GB
// - 0x180: 1TB
//
#define CAPACITYSPD64GB  0x100
#define CAPACITYSPD128GB 0x120
#define CAPACITYSPD256GB 0x124
#define CAPACITYSPD512GB 0x12C
#define CAPACITYSPD1TB   0x180

//
// Set System Time Opcode
//
#define SET_SYSTEM_TIME_OPCODE  0x7

//
// Set System Time sub-opcode
//
#define SET_SYSTEM_TIME_SUB_OPCODE 0x0

//
// Identify DIMM opcode and sub-opcode
//
#define IDENTIFY_DIMM_OPCODE_SUB_OPCODE 0x1

//
// get power management policy opcode
//
#define GET_POWER_MANAGEMENT_POLICY_OPCODE 0x4

//
// get/set power management policy sub-opcode
//
#define GET_SET_POWER_MANAGEMENT_POLICY_SUB_OPCODE 0x2

//
// Set power management policy opcode
//
#define SET_POWER_MANAGEMENT_POLICY_OPCODE 0x5

//
// Get Partition Info opcode
//
#define GET_PARTITION_INFO_OPCODE 0x6

//
// Set,Get Partition Info sub-opcode
//
#define GET_PARTITION_INFO_SUB_OPCODE 0x2
//
// Get Platform Config data opcode
//
#define GET_PLATFORM_CONFIG_DATA_OPCODE 0x6
//
// Get Platform Config data sub-opcode
//
#define GET_PLATFORM_CONFIG_DATA_SUB_OPCODE 0x1
//
// Set Die Sparing Policy opcode
//
#define SET_DIE_SPARING_POLICY_OPCODE 0x5
//
// Set Die Sparing Policy sub-opcode
//
#define SET_DIE_SPARING_POLICY_SUB_OPCODE 0x3

//
// Get security state opcode
//
#define GET_SECURITY_STATE_OPCODE 0x2

//
// Get security state sub-opcode
//
#define GET_SECURITY_STATE_SUB_OPCODE 0x0

//
// Secure Erase Unit opcode
//
#define SECURE_ERASE_UNIT_OPCODE 0x3

//
// Secure Erase Unit sub-opcode
//
#define SECURE_ERASE_UNIT_SUB_OPCODE 0xF5

//
// Set Config Lock down opcode
//
#define SET_CONFIG_LOCK_DOWN_OPCODE  0x07
//
// Set Config Lock down sub-opcode
//
#define SET_CONFIG_LOCK_DOWN_SUB_OPCODE  0x05

//
// RAS Mode
//
#define RAS_MODE_DISABLE        0

#define MIRROR_1LM_ENABLED      1
#define MIRROR_2LM_ENABLED      2
#define PMIRROR_1LM_ENABLED     3
#define PMIRROR_2LM_ENABLED     4

#define LOCKSTEP_ENABLED        3

#define SPARING_DISABLED        0
#define SPARING_ENABLED         1
#define ONE_RANK_SPARE          1
#define TWO_RANK_SPARE          2

/*#define FAILOVER_DIR_SCRUB    0
#define FAILOVER_BADCHNL_FIRST  1
#define FAILOVER_SWAP_MAPOUT    2*/

#define FAILOVER_DISABLED       0
#define FAILOVER_DIR_SCRUB      1
#define FAILOVER_BADCHNL_FIRST  2
#define FAILOVER_HIGHER_REL     3
#define FAILOVER_SWAP_MAPOUT    4 // De-PORed

// Index of the internal ThresholdLog2 type to read/write for FAILOVER_SAWP_MAPOUT error.
#define  LINK_TRANSIENT_ERR_TYPE  0
#define  ECC_CORRECTED_ERR_TYPE   1
#define  UNCORRECTABLE_ERR_TYPE   2
#define  MIRRSCRUBRD_NOERR_TYPE   3
#define  MIRRSCRUBRD_ERR_TYPE     4

#define PMIRROR_DISABLED   0
#define PMIRROR_ENABLED    1
#define MAX_PMIRR_SIZE          0xFFFF //Max multiplier for 64MB partial mirrors
#define MAX_PMIRR_PERCENT_SIZE  5000  //Maximum percentage that can be mirrored is 50%
#define MAX_PARTIAL_MIRROR      4  //Maximum number of partial mirror regions that can be created

#define NGN_DIE_SPARING_EN                               1
#define NGN_DIE_SPARING_DIS                              0

#define NGN_ADDRESS_RANGE_SCRUB_EN                       1
#define NGN_ADDRESS_RANGE_SCRUB_DIS                      0

#define NGN_HOST_ALERT_POISON_EN                     0
#define NGN_HOST_ALERT_VIRAL_EN                      1

#define NGN_HOST_ALERT_UNC_PATROL_SCRUB_EN            1
#define NGN_HOST_ALERT_UNC_PATROL_SCRUB_DIS           0

//
//  BGF Threshold
//
#define BGF_THRESHOLD         4

//
// Scheduler Pending Queue
//
#define LINK_LIST_THRESHOLD    0x3F
#define LINK_LIST_THRESHOLD_B0 0x10

//
// RDIMM Timing Control 2
//
#define RDIMM_TIMING_CNTL2_TCKOFF 0x7F
#define RDIMM_TIMING_CNTL2_TCKEV_DDR_2133 4
#define RDIMM_TIMING_CNTL2_TCKEV_DDR_2666 6
#define RDIMM_TIMING_CNTL2_TCKEV 8
#define NVMDIMM_TIMING_CNTL2_TCKOFF 0xFF

//
// Partial Write Starvation Counter
//
#define WPQ_IS                37    // HSD 4928321
#define WPQ_IS_ISOC           37    // HSD 4928321

//
// Channel PPDS idle counter
//
#define PPDS_IDLE_TIMER       0x200

//
// Pattern length for advanced training steps
//
#define TRAIN_RD_DQS_PL       64
#define TRAIN_WR_DQS_PL       64
#define TRAIN_RD_DQS_DL       256

//
// Minimum timing check
//
#define MIN_TIMING_LIMIT      5

//
// DRAM Ron default set by PcdMrcDramRonDefault
//
#define DRAM_RON_DISABLE      0
#define DRAM_RON_ENABLE       1

//
// DRAM Rx Eq
//
#define DRAM_RX_EQ_DISABLE      0
#define DRAM_RX_EQ_ENABLE       1
#define DRAM_RX_EQ_DEFAULT      DRAM_RX_EQ_ENABLE

//
// DRAM Tx Echo Cancellation
//
#define DRAM_TX_ECHO_CANCEL_DISABLE      0
#define DRAM_TX_ECHO_CANCEL_ENABLE       1
#define DRAM_TX_ECHO_CANCEL_DEFAULT      DRAM_TX_ECHO_CANCEL_DISABLE

//
// DRAM Legacy RxDfe Training
//
#define DRAM_LEGACY_RXDFE_DISABLE      0
#define DRAM_LEGACY_RXDFE_ENABLE       1
#define DRAM_LEGACY_RXDFE_DEFAULT      DRAM_LEGACY_RXDFE_DISABLE

//
// Rx Ctle Extended Range
//
#define RX_CTLE_EXTENDED_RANGE_DISABLE      0
#define RX_CTLE_EXTENDED_RANGE_ENABLE       1
#define RX_CTLE_EXTENDED_RANGE_DEFAULT      RX_CTLE_EXTENDED_RANGE_ENABLE

//
// RX ODT default set by PcdMrcMcOdtDefault
//
#define RX_ODT_DISABLE        0
#define RX_ODT_ENABLE         1

//
// MC RON default set by PcdMrcMcRonDefault
//
#define MC_RON_DISABLE        0
#define MC_RON_ENABLE         1

//
// RXCTLE training
//
#define RX_CTLE_DISABLE      0
#define RX_CTLE_ENABLE       1

//
// RPQAGE defaults
//
#define IO_COUNT              0x40
//#define CPU_GT_COUNT          0x40
// HSD4929074
#define CPU_GT_COUNT          0x100

//
// CMD Normalization
// Default is set by PcdMrcCmdNormalizationTrainingEnable
//
#define CMD_NORMAL_DISABLE          0
#define CMD_NORMAL_ENABLE           1

//
// CMD Vref Training default set by PcdMrcCmdVrefCenteringTrainingEnable
//
#define CMD_VREF_DISABLE       0
#define CMD_VREF_ENABLE        1

//
// LRDIMM Backside Vref Training
//
#define LRDIMM_BACKSIDE_VREF_DISABLE         0
#define LRDIMM_BACKSIDE_VREF_ENABLE          1
#define LRDIMM_BACKSIDE_VREF_DEFAULT         LRDIMM_BACKSIDE_VREF_ENABLE

#define LRDIMM_WR_VREF_DISABLE               0
#define LRDIMM_WR_VREF_ENABLE                1
#define LRDIMM_WR_VREF_DEFAULT               LRDIMM_WR_VREF_ENABLE  //Permanently disabled for HSX, applies to BDX only.

#define LRDIMM_RD_VREF_DISABLE               0
#define LRDIMM_RD_VREF_ENABLE                1
#define LRDIMM_RD_VREF_DEFAULT               LRDIMM_RD_VREF_ENABLE  //Permanently disabled for HSX, applies to BDX only.

//
// LRDIMM Baside RX DQ Centering
//
#define LRDIMM_RX_DQ_CENTERING_DISABLE       0
#define LRDIMM_RX_DQ_CENTERING_ENABLE        1
#define LRDIMM_RX_DQ_CENTERING_DEFAULT       LRDIMM_RX_DQ_CENTERING_ENABLE

//
// LRDIMM Baside TX DQ Centering
//
#define LRDIMM_TX_DQ_CENTERING_DISABLE       0
#define LRDIMM_TX_DQ_CENTERING_ENABLE        1
#define LRDIMM_TX_DQ_CENTERING_DEFAULT       LRDIMM_TX_DQ_CENTERING_ENABLE


// IOT MEMORY BUFFER RESERVATION
//
#define IOT_BUFFER_DEFAULT    0  // 1 = 64MB, 2= 128MB, so on till 256 = 16384MB (16GB)

//
// Per Bit DeSkew Training
//
#define BIT_DESKEW_DISABLE   0
#define BIT_DESKEW_ENABLE    1
#define BIT_DESKEW_AUTO      2
#define BIT_DESKEW_DEFAULT   BIT_DESKEW_ENABLE

//
// DRAM Dutye Cycle Adjuster Training
//
#define DRAM_DUTY_CYCLE_ADJUSTER_DISABLE        0
#define DRAM_DUTY_CYCLE_ADJUSTER_ENABLE         1

//
// Option to offset the final memory training results
//
#define TRNG_RESULT_OFFSET_DISABLE        0
#define TRNG_RESULT_OFFSET_ENABLE         1
#define TRNG_RESULT_OFFSET_DEFAULT        TRNG_RESULT_OFFSET_DISABLE
#define TRNG_RESULT_OFFSET_DEFAULT_VALUE  100 // equates offset to 0 as, +ve starts at 0, -ve starts at 100
#define TRNG_RESULT_OFFSET_MIN_VALUE      0
#define TRNG_RESULT_OFFSET_MAX_VALUE      32767

//
// LSX implementation
//
#define LSX_IMPLEMENTATION_SWSMI   0
#define LSX_IMPLEMENTATION_ASL     1
#define LSX_IMPLEMENTATION_DEFAULT LSX_IMPLEMENTATION_SWSMI

//
// Latch System Shutdown State
//
#define CR_LATCH_SYSTEM_SHUTDOWN_DISABLE            0
#define CR_LATCH_SYSTEM_SHUTDOWN_ENABLE             1
#define CR_LATCH_SYSTEM_SHUTDOWN_DEFAULT            CR_LATCH_SYSTEM_SHUTDOWN_ENABLE

//
// Enabling the BIOS SSA loader
//
#define BIOS_SSA_LOADER_DISABLE           0
#define BIOS_SSA_LOADER_ENABLE            1
#define BIOS_SSA_LOADER_AUTO              2
//Default is set by PCD PcdSsaLoaderEnable

//
// Enabling the BIOS SSA Stitched Mode (RMT)
//
#define BIOS_SSA_RMT_DISABLE                 0
#define BIOS_SSA_RMT_ENABLE                  1
#define BIOS_SSA_RMT_AUTO                    2
//Default is set by PCD PcdSsaBuiltInRmtEnable

//
// Enable RMT on Advacned Training
//
#define RMT_ON_ADVANCED_TRAINING_DISABLE     0
#define RMT_ON_ADVANCED_TRAINING_ENABLE      1
// Default is set by PCD PcdRmtAdvancedTraining

//
// NGN BCOM Margining
//
#define NGN_BCOM_MARGINING_DISABLE           0
#define NGN_BCOM_MARGINING_ENABLE            1
// Default is set by PcdEnableNgnBcomMargining


//
// Enabling the BIOS SSA Stitched Mode (RMT) on FAST COLD BOOT(FCB)
//
#define BIOS_SSA_RMT_FCB_DISABLE                 0
#define BIOS_SSA_RMT_FCB_ENABLE                  1
#define BIOS_SSA_RMT_FCB_AUTO                    2
#define BIOS_SSA_RMT_FCB_DEFAULT                 BIOS_SSA_RMT_FCB_DISABLE

// *Sub-menu with Configuration options for the BIOS SSA Rank Margin Tool*

//Generic defaults
#define BIOS_SSA_RMT_DEFAULT_VALUE                                1
#define BIOS_SSA_LOOP_CNT_DEFAULT                                16
//
// SIMICS Training step for RMT
//
#define BIOS_SSA_RMT_SIMICS_VALUE                                32

//
// BSSA RMT Per-Bit Margining
//
#define BIOS_SSA_PER_BIT_MARGINING_DISABLE              0
#define BIOS_SSA_PER_BIT_MARGINING_ENABLE               1
#define BIOS_SSA_PER_BIT_MARGINING_AUTO                 2
#define BIOS_SSA_PER_BIT_MARGINING_DEFAULT              BIOS_SSA_PER_BIT_MARGINING_ENABLE

//
// BSSA RMT Display Tables
//
#define BIOS_SSA_DISPLAY_TABLE_DISABLE                  0
#define BIOS_SSA_DISPLAY_TABLE_ENABLE                   1
#define BIOS_SSA_DISPLAY_TABLE_AUTO                     2
#define BIOS_SSA_DISPLAY_TABLE_DEFAULT                  BIOS_SSA_DISPLAY_TABLE_ENABLE

//
// BSSA RMT Display Plots
//
#define BIOS_SSA_DISPLAY_PLOTS_DISABLE                  0
#define BIOS_SSA_DISPLAY_PLOTS_ENABLE                   1
#define BIOS_SSA_DISPLAY_PLOTS_AUTO                     2
#define BIOS_SSA_DISPLAY_PLOTS_DEFAULT                  BIOS_SSA_DISPLAY_PLOTS_ENABLE

//
// BSSA RMT Backside Margining
//
#define BIOS_SSA_BACKSIDE_MARGINING_DISABLE             0
#define BIOS_SSA_BACKSIDE_MARGINING_ENABLE              1
#define BIOS_SSA_BACKSIDE_MARGINING_AUTO                2
#define BIOS_SSA_BACKSIDE_MARGINING_DEFAULT             BIOS_SSA_BACKSIDE_MARGINING_DISABLE

//
// BSSA RMT Early Read ID Margining
//
#define BIOS_SSA_EARLY_READ_ID_DISABLE                  0
#define BIOS_SSA_EARLY_READ_ID_ENABLE                   1
#define BIOS_SSA_EARLY_READ_ID_AUTO                     2
#define BIOS_SSA_EARLY_READ_ID_DEFAULT                  BIOS_SSA_EARLY_READ_ID_DISABLE

//
// BSSA RMT Step Size Override
//
#define BIOS_SSA_STEP_SIZE_OVERRIDE_DISABLE             0
#define BIOS_SSA_STEP_SIZE_OVERRIDE_ENABLE              1
#define BIOS_SSA_STEP_SIZE_OVERRIDE_AUTO                2
#define BIOS_SSA_STEP_SIZE_OVERRIDE_DEFAULT             BIOS_SSA_STEP_SIZE_OVERRIDE_DISABLE

//
// BSSA RMT Debug Messages
//
#define BIOS_SSA_RMT_DEBUG_MSGS_DISABLE                  2
#define BIOS_SSA_RMT_DEBUG_MSGS_ENABLE                   5 //values are 2 for disabled, and 5 for enabled.
#define BIOS_SSA_RMT_DEBUG_MSGS_AUTO                     9
#define BIOS_SSA_RMT_DEBUG_MSGS_DEFAULT                  BIOS_SSA_RMT_DEBUG_MSGS_DISABLE

// *Sub-menu with Configuration options for the BIOS SSA Rank Margin Tool*

//Generic defaults
#define MIN_BIOS_SSA_LOOP_CNT               0
#define MAX_BIOS_SSA_LOOP_CNT               31
#define BIOS_SSA_LOOP_CNT_DEFAULT           16

#define BSSA_VALUE_1                        1
#define BSSA_VALUE_2                        2
#define BSSA_VALUE_4                        4
#define BSSA_VALUE_8                        8

//
// Patrol scrubbing
//
#define PATROL_SCRUB_DURATION_DEFAULT 24
#define PATROL_SCRUB_DURATION_MAX 24
#define PATROL_SCRUB_DIS      0
#define PATROL_SCRUB_EN       1
#define PATROL_SCRUB_REVERSE_ADDR   0
#define PATROL_SCRUB_SPA            1
#define PATROL_SCRUB_EOP            2
//
// Demand Scrub
//
#define DEMAND_SCRUB_DIS      0
#define DEMAND_SCRUB_EN       1

//
// Leaky Bucket Default Values
//
#define LEAKY_BUCKET_TIME_WINDOW_DISABLED       0
#define LEAKY_BUCKET_TIME_WINDOW_ENABLED        1
#define LEAKY_BUCKET_LO       0x28
#define LEAKY_BUCKET_HI       0x29
#define LEAKY_BUCKET_HOUR        24
#define LEAKY_BUCKET_MINUTE      0

//
// MC Data Parity Check
//
#define MC_DATA_PARITY_CHECK_DISABLE       0
#define MC_DATA_PARITY_CHECK_ENABLE        1
#define MC_DATA_PARITY_CHECK_AUTO          2
#define MC_DATA_PARITY_CHECK_DEFAULT       MC_DATA_PARITY_CHECK_AUTO

//
// Multi-Vref algorithm defaults
//
#define RX_MULTI_VREF_POINTS  7
#define TX_MULTI_VREF_POINTS  7
#define MAX_VREF_POINTS       10    // Maximum number of points possible
#define RX_MULTI_VREF_STEP    4
#define TX_MULTI_VREF_STEP    6
#define WEIGHT_COEF_A         0
#define WEIGHT_COEF_B         8
#define WEIGHT_COEF_C         100

//
// Alternate Address Map
//
#define ALTERNATE_ADDRESS_MAP_DIS     0
#define ALTERNATE_ADDRESS_MAP_EN      1
#define ALTERNATE_ADDRESS_MAP_AUTO    2
#define ALTERNATE_ADDRESS_MAP_DEFAULT ALTERNATE_ADDRESS_MAP_EN

//
// DIMMTEMP / DimmTemp default
//
#define DIMMTEMP_DEFAULT    0x5C

//
//
//
#define RX_VREF_PER_BIT        0xF
#define RX_VREF_PER_NIBBLE     0x8
#define RX_VREF_SETTING        RX_VREF_PER_BIT
#define RX_VREF_PER_BIT_BUS    0x8
#define RX_VREF_PER_NIBBLE_BUS 0xF
#define RX_VREF_TO_NIBBLE_BUS  RX_VREF_PER_BIT_BUS

//
// DDRT Transcation
//
#define DDRT_XACTOR_DEFAULT 1

//
// ADR related options
//
#define ADR_BBU     1
#define ADR_NVDIMM  2

#define SKX_A0_WORKAROUND 1

//
// NGN Publishing ARS capability
//
#define NGN_ARS_PUBLISH_DIS       0
#define NGN_ARS_PUBLISH_EN        1
#define NGN_ARS_PUBLISH_AUTO      2
#define NGN_ARS_PUBLISH_DEFAULT   NGN_ARS_PUBLISH_EN

//
// CR Media Status exception
//
#define NVMCTRL_MEDIA_STATUS_EXCEPTION_DISABLE        0
#define NVMCTRL_MEDIA_STATUS_EXCEPTION_ENABLE         1
#define NVMCTRL_MEDIA_STATUS_EXCEPTION_DEFAULT        NVMCTRL_MEDIA_STATUS_EXCEPTION_ENABLE

//
// CR Not Supported AEP DIMM exception
//
#define AEP_DIMM_NOT_SUPPORTED_EXCEPTION_DISABLE        0
#define AEP_DIMM_NOT_SUPPORTED_EXCEPTION_ENABLE         1
#define AEP_DIMM_NOT_SUPPORTED_EXCEPTION_DEFAULT        AEP_DIMM_NOT_SUPPORTED_EXCEPTION_DISABLE

//
// CR QoS Configuration default set by PcdMrcCrQosConfigDefault
//
#define CR_FASTGO_DEFAULT                       0
#define CR_FASTGO_DISABLE                       1
#define CR_FASTGO_DISABLE_MLC_SQ_THRESHOLD_5    2
#define CR_FASTGO_DISABLE_MLC_SQ_THRESHOLD_6    3
#define CR_FASTGO_DISABLE_MLC_SQ_THRESHOLD_8    4
#define CR_FASTGO_DISABLE_MLC_SQ_THRESHOLD_10   5
#define CR_FASTGO_AUTOMATIC                     6
#define CR_FASTGO_LAST_OPTION                   CR_FASTGO_AUTOMATIC
#define CR_FASTGO_KNOB_DEFAULT                  CR_FASTGO_AUTOMATIC

//
// CR Mixed SKU
//
#define CR_MIXED_SKU_DIS        0
#define CR_MIXED_SKU_EN         1
#define CR_MIXED_SKU_AUTO       2
#define CR_MIXED_SKU_DEFAULT    CR_MIXED_SKU_EN

//
// Load NGN DIMM Management Drivers
//
#define LOAD_NGN_DIMM_MGMT_DRIVER_DIS     0
#define LOAD_NGN_DIMM_MGMT_DRIVER_EN      1
#define LOAD_NGN_DIMM_MGMT_DRIVER_DEFAULT LOAD_NGN_DIMM_MGMT_DRIVER_EN

//
// IMC BCLK
//
#define IMC_BCLK_100          100
#define IMC_BCLK_133          133
#define IMC_BCLK_AUTO         0

//
// BCLK Freq Request Type:
//      0h MC frequency request for 133 Mhz
//      1h MC frequency request for 100 Mhz
//      All other values are reserved.
//
#define BCLK_TYPE_133    0
#define BCLK_TYPE_100    1

//
// SPD CRC Check
//
#define SPD_CRC_CHECK_DIS     0
#define SPD_CRC_CHECK_EN      1
#define SPD_CRC_CHECK_AUTO    2
#define SPD_CRC_CHECK_DEFAULT SPD_CRC_CHECK_DIS

//
//Crystal Ridge 2LM: Eliminate directory in Far Memory (FM)
//
#define ELIMINATE_DIRECTORY_IN_FM_ENABLE              0
#define ELIMINATE_DIRECTORY_IN_FM_DISABLE             1

//
// NVMDIMM Performance Setting
//
#define NVMDIMM_BW_OPTIMIZED                          0
#define NVMDIMM_LATENCY_OPTIMIZED                     1
#define NVMDIMM_BALANCED                              2

//
// Early completion setup options
//
#define DDR_EARLY_CMP_ENABLED          1
#define DDR_EARLY_CMP_DISABLED         0
#define DDR_EARLY_CMP_DEFAULT          DDR_EARLY_CMP_DISABLED

#define HBM_EARLY_CMP_ENABLED          1
#define HBM_EARLY_CMP_DISABLED         0
#define HBM_EARLY_CMP_DEFAULT          HBM_EARLY_CMP_ENABLED

//
// XOR decoder
//
#define CLUSTER_XOR_ENABLED            1
#define CLUSTER_XOR_DISABLED           0
#define CLUSTER_XOR_DEFAULT            CLUSTER_XOR_ENABLED

#define CHANNEL_XOR_ENABLED            1
#define CHANNEL_XOR_DISABLED           0
#define CHANNEL_XOR_DEFAULT            CHANNEL_XOR_ENABLED

#define PSEUDO_CHANNEL_XOR_ENABLED     1
#define PSEUDO_CHANNEL_XOR_DISABLED    0
#define PSEUDO_CHANNEL_XOR_DEFAULT     PSEUDO_CHANNEL_XOR_DISABLED

#define DIMM_XOR_ENABLED               1
#define DIMM_XOR_DISABLED              0
#define DIMM_XOR_DEFAULT               DIMM_XOR_DISABLED

#define RANK_XOR_ENABLED               1
#define RANK_XOR_DISABLED              0
#define RANK_XOR_DEFAULT               RANK_XOR_DISABLED

#define SID_XOR_ENABLED                1
#define SID_XOR_DISABLED               0
#define SID_XOR_DEFAULT                SID_XOR_DISABLED

#define BANK_XOR_ENABLED               1
#define BANK_XOR_DISABLED              0
#ifdef ICX_HOST
#define BANK_XOR_DEFAULT               BANK_XOR_ENABLED
#else
#define BANK_XOR_DEFAULT               BANK_XOR_DISABLED
#endif
//
// Below DFx Setup knobs have been created for FC Emulation usage only
//
#ifdef SKX_HOST
#define INPUT_RT_DEFAULT          64
#define INPUT_DDRT_RT_DEFAULT     64
#else
#define INPUT_RT_DEFAULT          255
#define INPUT_DDRT_RT_DEFAULT     255
#endif
#define INPUT_IOLAT_DEFAULT       31
#define INPUT_IOLATIOCOMP_DEFAULT 127
#define INPUT_TXDQ_DEFAULT         2047
#define INPUT_TXDQS_DEFAULT        2047
#define INPUT_TXDQBIT_DEFAULT      2047
#define INPUT_DDRT_TXDQS_DEFAULT   2047
#define INPUT_DDRT_TXDQBIT_DEFAULT 2047
#define INPUT_RXDQ_DEFAULT        255
#define INPUT_RXDQS_DEFAULT       255
#define INPUT_RXDQBIT_DEFAULT     63
#define INPUT_GNT2ERID_DEFAULT    511
#define INPUT_CWLP_DEFAULT        15
#define INPUT_CWLN_DEFAULT        7
#define RCV_EN_DEL_DEFAULT        2047
#define DDRT_RCV_EN_DEL_DEFAULT   2047
#define INPUT_CMD_DELAY_DEFAULT   0

#define UBIOS_SMBUS_TRACE_DISABLE   0
#define UBIOS_SMBUS_TRACE_ENABLE    1
#define UBIOS_SMBUS_TRACE_AUTO      2
#define UBIOS_SMBUS_TRACE_DEFAULT   UBIOS_SMBUS_TRACE_DISABLE

#define SMBUS_I3C_MODE_DISABLE   0
#define SMBUS_I3C_MODE_ENABLE    1
#define SMBUS_I3C_MODE_AUTO      2
#define SMBUS_I3C_MODE_DEFAULT   SMBUS_I3C_MODE_ENABLE

//Energy Policy Management
#define ENERGY_POLICY_DEVICE_MANAGED  0
#define ENERGY_POLICY_HOST_MANAGED    1
#define ENERGY_POLICY_DEFAULT         ENERGY_POLICY_DEVICE_MANAGED

//
//CMI Initialize default set by PcdMrcCmiInitUseResetDefault
//
#define CMI_INIT_AUTO 0
#define CMI_INIT_RESET 1

//
// 2GB Short Stroke Configuration
//
#define SHORT_STROKE_2GB_DIS        0
#define SHORT_STROKE_2GB_INT        1
#define SHORT_STROKE_2GB_NON_INT    2
#define SHORT_STROKE_2GB_DEFAULT    SHORT_STROKE_2GB_DIS

//
// Crystal Ridge: skip ARS on boot
//
#define CR_SKIP_ARS_ON_BOOT_DIS       0
#define CR_SKIP_ARS_ON_BOOT_EN        1
#define CR_SKIP_ARS_ON_BOOT_DEFAULT   CR_SKIP_ARS_ON_BOOT_DIS

#define CR_SEAMLESS_OPT_IN_DIS 0
#define CR_SEAMLESS_OPT_IN_EN 1
#define CR_SEAMLESS_OPT_IN_DEFAULT CR_SEAMLESS_OPT_IN_DIS

//
// Crystal Ridge: Background ARS
//
#define CR_BACKGROUND_ARS_AUTO    2
#define CR_BACKGROUND_ARS_DIS     0
#define CR_BACKGROUND_ARS_DEFAULT CR_BACKGROUND_ARS_AUTO

//
//Crystal Ridge AD: Disable Directory writes For AD
//
#define DIRECTORY_FOR_APP_DIRECT_DISABLE              0
#define DIRECTORY_FOR_APP_DIRECT_ENABLE               1

//
//14nm CR QoS Configuration Recipes
//
#define NVM_QOS_DISABLE                              0
#define NVM_QOS_RECIPE1                              1
#define NVM_QOS_RECIPE2                              2
#define NVM_QOS_RECIPE3                              3
#define NVM_QOS_DEFAULT                              NVM_QOS_DISABLE

//
//10nm CR QoS Configuration Recipes
//
#define DDRT_QOS_MODE_0  0       // KTI_DISABLE
#define DDRT_QOS_MODE_1  1       // M2M QoS Enable, CHA QoS Disable
#define DDRT_QOS_MODE_2  2       // M2M QoS Enable, CHA QoS Enable


//
// Fast Cold Boot
//
#define FAST_BOOT_COLD_DIS                           0       // Disable fast boot (i.e. take the normal cold boot path through MRC)
#define FAST_BOOT_COLD_EN                            1       // Enable fast boot (i.e. skip parts of MRC when possible)
#define FAST_BOOT_COLD_AUTO                          2
#define FAST_BOOT_COLD_DEFAULT                       FAST_BOOT_COLD_EN   // Changing the default here requires a change in the help string of the respective BIOS knob.

// Mem boot health check feature support default set by PcdMemBootHealthFeatureSupported
#define MEM_BOOT_HEALTH_ENABLE  1
#define MEM_BOOT_HEALTH_DISABLE 0

// Mem Boot Health check default option set by PcdMemBootHealthCheck
// Option to choose Mem Boot Health configuration type.
// 00=>Auto (Use defaults)
// 01=>Manual (Override defaults with setup option)
// 02=>Disable (Disable feature)
#define MEM_BOOT_HEALTH_CONFIG_AUTO    0
#define MEM_BOOT_HEALTH_CONFIG_MANUAL  1
#define MEM_BOOT_HEALTH_CONFIG_DISABLE 2

//
// System reboot on critical margin failure default set by PcdResetOnCriticalError
//
#define ENABLE_REBOOT_ON_CRITICAL_FAILURE  1
#define DISABLE_REBOOT_ON_CRITICAL_FAILURE 0

//
// Posted CSR access settings
//
#define POSTED_CSR_ACCESS_DIS   0
#define POSTED_CSR_ACCESS_EN    1
#define POSTED_CSR_ACCESS_AUTO  2

//
// Cpgc register cache access settings
//
#define CPGC_REG_CACHE_ACCESS_DIS   0
#define CPGC_REG_CACHE_ACCESS_EN    1
#define CPGC_REG_CACHE_ACCESS_AUTO  2

//
// DFX Option for SSA RMT Receive Enable Denormalization
// When Enabled, applies denormalization before SSA RMT Execution
// After SSA RMT Execution, Renormalization is applied.
//
#define DFX_SSA_RMT_RECV_ENABLE_DENORMALIZE_ENABLE  1
#define DFX_SSA_RMT_RECV_ENABLE_DENORMALIZE_DISABLE 0

//
// Synchronous CPGC start
//
#define CPGC_SYNC_START_DIS   0
#define CPGC_SYNC_START_EN    1
#define CPGC_SYNC_START_AUTO  2

//
// DFX Periodic ZQCal enable/disable settings
//
#define DFX_PERIODIC_ZQCAL_DISABLE  0
#define DFX_PERIODIC_ZQCAL_ENABLE   1
#define DFX_PERIODIC_ZQCAL_AUTO     2

//
// DFX Periodic ZQCal enable/disable settings
//
#define DFX_SENSE_AMP_CALIB_HW_FSM  0
#define DFX_SENSE_AMP_CALIB_SW      1
#define DFX_SENSE_AMP_CALIB_AUTO    2

//
// DFE SW Xover mode selection
//
#define DFX_XOVER_SW_SERIAL   0
#define DFX_XOVER_SW_PARALLEL 1

#ifdef CWV_A0_REMOVE_POST_PO
//
//Skip ddrt MB2 command
//
#define DFX_DDRT_SKIP_MB2_DISABLE  0
#define DFX_DDRT_SKIP_MB2_ENABLE   1
#endif
#endif   // _mem_defaults_h
