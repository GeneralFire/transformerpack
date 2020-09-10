/** @file
  MRC Performance data structures. Shared between the MRC and the
  Shell performance tracking tool.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef _PERFORMANCE_TRACKER_H_
#define _PERFORMANCE_TRACKER_H_

#include <Uefi.h>

#define MAX_NOZONE 10

//
//  Performance tracker zones, used with GetMemPhase
//

typedef enum {
  PTZ_NOZONE,
  PTZ_DIMM_DETECT,
  PTZ_CHECK_DIMM_RANKS,
  PTZ_GATHER_SPD,
  PTZ_EARLY_CONFIG,
  PTZ_JEDEC_INIT,
  PTZ_CMD_CLK,
  PTZ_REC_EN,
  PTZ_RDDQDQS,
  PTZ_WR_LVL,
  PTZ_WRDQDQS,
  PTZ_WR_FLYBY,
  PTZ_CLOCK_INIT,
  PTZ_RDDQDQS_ADV,
  PTZ_WRDQDQS_ADV,
  PTZ_RANK_MARGIN_TOOL,
  PTZ_MEM_MAPPING,
  PTZ_RAS_CONFIG,
  PTZ_EYE_DIAGRAM,
  PTZ_XOVER_CALIB,
  PTZ_BIT_DESKEW_RX,
  PTZ_RD_VREF,
  PTZ_NORMAL_MODE,
  PTZ_CMD_CLK_EARLY,
  PTZ_BIT_DESKEW_TX,
  PTZ_WR_VREF,
  PTZ_RT_OPT,
  PTZ_SENSE_AMPLIFIER,
  PTZ_DDRIO_INIT,
  PTZ_CTL_CLK_EARLY,
  PTZ_LRDIMM_RX,
  PTZ_LRDIMM_TX,
  PTZ_WR_VREF_LRDIMM,
  PTZ_CMD_VREF_CEN,
  PTZ_TRAIN_TX_EQ,
  PTZ_TRAIN_IMODE,
  PTZ_TRAIN_CTLE,
  PTZ_TRAIN_MC_RON,
  PTZ_TRAIN_DRAM_RX_EQ,
  PTZ_TRAIN_RTT_WR,
  PTZ_TRAIN_RX_ODT,
  PTZ_TRAIN_DRAM_RON,
  PTZ_SWIZZLE_DISCOVERY,
  PTZ_RDDQDQS_DFE,
  PTZ_TRAIN_RX_POST_DFE,
  PTZ_SWITCH_DDRT2_MODE,
  PTZ_BS_WR_LVL_LRDIMM,
  PTZ_E_WR_VREF,
  PTZ_E_RD_VREF,
  PTZ_RD_VREF_LRDIMM,
  PTZ_TX_DQ_CENTERING_LRDIMM,
  PTZ_RX_DQ_CENTERING_LRDIMM,
  PTZ_E_RID,
  PTZ_LATE_CONFIG,
  PTZ_MEM_BIST,
  PTZ_MEM_INIT,
  PTZ_POST_PKG_RPR,
  PTZ_TRAIN_CMD_TX_EQ,
  PTZ_HBM_ADV_AWORD,
  PTZ_HBM_ADV_TX_DWORD,
  PTZ_HBM_ADV_RX_DWORD,
  PTZ_HBM_ADV_RX_VREF,
  PTZ_HBM_RX_DQS_HOLD,
  PTZ_HBM_RMT,
  PTZ_HBM_RX_ROUNDTRIP,
  PTZ_HBM_TCO,
  PTZ_HBM_MEM_TEST,
  PTZ_HBM_MEM_INIT,
  PTZ_TRAIN_TX_RFSR,
  PTZ_TRAIN_TCO_COMP,
  PTZ_TRAIN_DCA_TCO,
  PTZ_TRAIN_TCO_DQDQS,
  PTZ_TRAIN_TX_ODT,
  PTZ_TRAIN_PXC,
  PTZ_TRAIN_DDJC,
  PTZ_TRAIN_RX_DFE,
  PTZ_TRAIN_READ_DFE,
  PTZ_TRAIN_WRITE_DFE,
  PTZ_TRAIN_TX_POST_DFE,
  PTZ_TRAIN_TX_RETRAIN,
  PTZ_CS_CLK_EARLY,
  PTZ_CA_CLK_EARLY,
  PTZ_CA_SLEW_RATE,
  PTZ_DCA_DCLK_DUTY_CYCLE,
  PTZ_DCA_DFE,
  PTZ_LRDIMM_BCOM,
  PTZ_EARLY_CS_CLK_BACKSIDE,
  PTZ_EARLY_CA_CLK_BACKSIDE,
  PTZ_LRDIMM_PBA_ENUMERATE_ID,
  PTZ_EARLY_REQ_CLK,
  PTZ_SSA_API_INIT_ST,
  PTZ_DDRIO_RCSIM_CACHE,
  PTZ_HBM_BASIC_FSM,
  PTZ_HBM_BUS_TND,
  PTZ_HBM_DISCOVERY,
  PTZ_HBM_DISPLAY,
  PTZ_HBM_EARLY_CONFIG,
  PTZ_HBM_INIT,
  PTZ_HBM_LATE_CONFIG,
  PTZ_HBM_MMRC,
  PTZ_HBM_NORMAL_MODE,
  PTZ_HBM_POR,
  PTZ_HBM_RX_DQ_DELAY,
  PTZ_HBM_THROTTLING,
  PTZ_HBM_TX_DS_TCO,
  PTZ_HBM_TX_NORM,
  PTZ_HBM_PRE_TRAINING,
  PTZ_HBM_TRAINING,
  PTZ_HBM_POST_TRAINING,
  PTZ_PIPE_POLL,
  PTZ_PIPE_SYNC,
  PTZ_TRAIN_DDR,
  PTZ_PRE_MRC,
  PTZ_DIMM_INFO_DISPLAY,
  PTZ_NVDIMM_STATUS,
  PTZ_RCOMP_STAT_LEG,
  PTZ_DISPLAY_RESULTS,
  PTZ_CHECK_POR,
  PTZ_UNLOCK_MEM_REGS,
  PTZ_CHECK_STATUS,
  PTZ_CONFIG_XMP,
  PTZ_EARLY_DDR_THERM,
  PTZ_MID_DDR_THERM,
  PTZ_LATE_DDR_THERM,
  PTZ_EARLY_INIT_MEM,
  PTZ_POST_TRAINING_INIT,
  PTZ_NVDIMM_RESTORE,
  PTZ_NVDIMM_ARM,
  PTZ_SVL_SCRAMBLE,
  PTZ_CMI_CREDIT,
  PTZ_CHECK_RAS,
  PTZ_INIT_ADR,
  PTZ_CALL_TABLE,
  PTZ_INIT_STRUCT_LATE,
  PTZ_SELECT_BOOT_MODE,
  PTZ_PRE_TRAINING_INIT,
  PTZ_SET_CLK_VDD,
  PTZ_MEM_LATE,
  PTZ_PACKAGE_DELAY,
  PTZ_QxCA_CLK,
  PTZ_MEM_HEALTH_TEST,
  PTZ_TX_ECHO,
  PTZ_LEGACY_RXDFE,
  PTZ_SET_CPL2,
  PTZ_LDRIMM_DB_DFE,
  PTZ_WRDQDQS_DFE,
  PTZ_DRAM_DUTY_CYCLE_ADJ,
  PTZ_TURNAROUND,
  PTZ_DQ_SLEW_RATE,
  PTZ_NORMALIZE_CMD,
  PTZ_MAX_ZONE
} PT_ZONE;

//
// The performance data counters structure (one instance per performance tracker zone/phase)
//
typedef struct _PERF_DATA {
  UINT32  PciCount;
  UINT32  TestCount;
  UINT32  JedecCount;
  UINT32  FixedDelay;
  UINT32  PollingCount;
  UINT32  VrefCount;
  UINT32  CpgcCount;
  UINT32  SmbusRead;
  UINT32  SmbusWrite;
  UINT32  MrsWrite;
  UINT32  DurationTime;
  UINT32  GetSysHostPointerCalls;
} PERF_DATA;

//
// The performance tracker description
//
typedef struct _PERFORMANCE_TRACKER_DESC {
  PT_ZONE       MemPhase;
  CHAR8         *String;
} PERFORMANCE_TRACKER_DESC;

//
// The performance data structure (one instance per performance tracker zone/phase)
//
typedef struct _PERFORMANCE_TRACKER_DATA {
  PT_ZONE       MemPhase;
  CHAR8         *String;
  PERF_DATA     PerfData;
} PERFORMANCE_TRACKER_DATA;

//
// The Hob performance data structure (one instance per performance tracker zone/phase)
//
typedef struct _PERFORMANCE_TRACKER_HOB_DATA {
  PT_ZONE    MemPhase;
  PERF_DATA  PerfData;
} PERFORMANCE_TRACKER_HOB_DATA;

//
// Hob structure used to pass MRC performance data to DXE
//
typedef struct _PERF_TRACKER_HOB_DATA {
  UINT16                        ZoneCount;
  PERF_DATA                     Totals;
  PERF_DATA                     NoZone[MAX_NOZONE];
  PERFORMANCE_TRACKER_HOB_DATA  PerformanceData[PTZ_MAX_ZONE];
} PERF_TRACKER_HOB_DATA;

//
//  Performance counters - Data to be tracked. Used with CountTrackingData ()
//

typedef enum {
  PTC_PCI_ACCESS,
  PTC_SMBUS_READ,
  PTC_JEDEC_COUNT,
  PTC_FIXED_DELAY,
  PTC_SMBUS_WRITE,
  PTC_MRS_WRITE,
  PTC_POLLING_COUNT,
  PTC_VREF_MOVE,
  PTC_CPGC_COUNT,
  PTC_DURATION_TIME,
  PTC_GETSYSHOSTPTR_CALLS,
  PTC_MAX_COUNTER,
  PTC_RX_SAMPLER_OFFSET_MOVE
} PERFORMANCE_COUNTER;

#endif // #ifndef _PERFORMANCE_TRACKER_H_
