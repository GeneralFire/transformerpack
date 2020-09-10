/** @file
  MemoryCheckpointCodes.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#ifndef _MEMORY_CHECKPOINT_CODES_H_
#define _MEMORY_CHECKPOINT_CODES_H_

//
// Major Checkpoint codes
//

#define CHECKPOINT_MAJOR_HBM                              0x70
#define CHECKPOINT_MAJOR_HBM_DEBUG                        0x71
#define CHECKPOINT_MAJOR_HBM_INTERNAL                     0x72
#define CHECKPOINT_MAJOR_PIPE_SYNC                        0x7E

#define CHECKPOINT_MAJOR_DIMM_DETECT                      0xB0
#define CHECKPOINT_MAJOR_CLOCK_INIT                       0xB1
#define CHECKPOINT_MAJOR_SPD_DATA                         0xB2
#define CHECKPOINT_MAJOR_GLOBAL_EARLY                     0xB3
#define CHECKPOINT_MAJOR_RANK_DETECT                      0xB4
#define CHECKPOINT_MAJOR_PARALLEL_DISPATCH                0xB5
#define CHECKPOINT_MAJOR_DDRIO_INIT                       0xB6
#define CHECKPOINT_MAJOR_CHANNEL_TRAINING                 0xB7
#define CHECKPOINT_MAJOR_INIT_THROTTLING                  0xB8
#define CHECKPOINT_MAJOR_MEMBIST                          0xB9
#define CHECKPOINT_MAJOR_MEMINIT                          0xBA
#define CHECKPOINT_MAJOR_DDR_MEMMAP                       0xBB
#define CHECKPOINT_MAJOR_RAS_CONFIG                       0xBC
#define CHECKPOINT_MAJOR_GET_MARGINS                      0xBD
#define CHECKPOINT_MAJOR_SSA_API_INIT                     0xBE
#define CHECKPOINT_MAJOR_MRC_DONE                         0xBF

#define CHECKPOINT_MAJOR_CHECK_POR                        0xC1
#define CHECKPOINT_MAJOR_UNLOCK_MEM_REGS                  0xC2
#define CHECKPOINT_MAJOR_CHECK_STATUS                     0xC3
#define CHECKPOINT_MAJOR_CONFIG_XMP                       0xC4
#define CHECKPOINT_MAJOR_EARLY_INIT_MEM                   0xC5
#define CHECKPOINT_MAJOR_DIMM_INFO                        0xC6
#define CHECKPOINT_MAJOR_NVDIMM                           0xC7
#define CHECKPOINT_MAJOR_SVL_SCRAMBLE                     0xC9
#define CHECKPOINT_MAJOR_CMI_CREDIT                       0xCA
#define CHECKPOINT_MAJOR_CHECK_RAS                        0xCB
#define CHECKPOINT_MAJOR_INIT_ADR                         0xCC
#define CHECKPOINT_MAJOR_INIT_STRUCT_LATE                 0xCD
#define CHECKPOINT_MAJOR_MEM_LATE                         0xCE
#define CHECKPOINT_MAJOR_SELECT_BOOT_MODE                 0xCF

#define CHECKPOINT_MAJOR_MKTME_EARLY_FLOW                 0xD0
#define CHECKPOINT_MAJOR_SGX_PREMEM                       0xD1
#define CHECKPOINT_MAJOR_MEM_HEALTH_TEST                  0xD2
#define CHECKPOINT_MAJOR_ENABLE_2N_MODE                   0xD3
#define CHECKPOINT_MAJOR_CPL2                             0xD5
#define CHECKPOINT_MAJOR_OFFSET_TRAIN_RESULT              0xD6

#define CHECKPOINT_MAJOR_NO_CODE                          0xFF

//
// Minor Checkpoint Codes associated with CHECKPOINT_MAJOR_PIPE_SYNC.
//
// CHECKPOINT_MINOR_PIPE_AP_XXX: Data source is AP. SBSP queries data from APs.
// CHECKPOINT_MINOR_PIPE_SBSP_XXX: Data source is SBSP. SBSP sends data to APs.
//

#define CHECKPOINT_MINOR_PIPE_AP_BOOT_MODE                0x00
#define CHECKPOINT_MINOR_PIPE_SBSP_BOOT_MODE              0x01
#define CHECKPOINT_MINOR_PIPE_AP_DATA                     0x02
#define CHECKPOINT_MINOR_PIPE_SBSP_STATUS                 0x03
#define CHECKPOINT_MINOR_PIPE_SBSP_DATA                   0x04
#define CHECKPOINT_MINOR_PIPE_AP_SSA_DATA                 0x05
#define CHECKPOINT_MINOR_PIPE_SBSP_SSA_DATA               0x06
#define CHECKPOINT_MINOR_PIPE_AP_NVRAM_DATA               0x07
#define CHECKPOINT_MINOR_PIPE_SBSP_VAR_DATA               0x08
#define CHECKPOINT_MINOR_PIPE_SBSP_MEM_MODE               0x09
#define CHECKPOINT_MINOR_PIPE_AP_FINAL_ERROR_SYNC         0x0A
#define CHECKPOINT_MINOR_PIPE_THERMAL_SYNC                0x0B
#define CHECKPOINT_MINOR_PIPE_CPL2                        0x0C

//
// Minor Checkpoint Codes associated with CHECKPOINT_MAJOR_DIMM_INFO.
//

#define CHECKPOINT_MINOR_DIMM_INFO_EARLY                  0x01
#define CHECKPOINT_MINOR_DIMM_INFO_LATE                   0x02

//
// Minor Checkpoint Codes associated with CHECKPOINT_MAJOR_NVDIMM.
//

#define CHECKPOINT_MINOR_NVDIMM_STS                       0x01
#define CHECKPOINT_MINOR_NVDIMM_RESTORE                   0x02
#define CHECKPOINT_MINOR_NVDIMM_ARM                       0x03

//
// Minor Checkpoint Codes associated with CHECKPOINT_MAJOR_CHANNEL_TRAINING.
//

#define CHECKPOINT_MINOR_RECEIVE_ENABLE_BASIC             0x00
#define CHECKPOINT_MINOR_WRITE_LEVELING_BASIC             0x01
#define CHECKPOINT_MINOR_WR_FLY_BY                        0x02
#define CHECKPOINT_MINOR_RX_DQ_DQS_BASIC                  0x03
#define CHECKPOINT_MINOR_TX_DQ_BASIC                      0x04
#define CHECKPOINT_MINOR_LATE_CMD_CLK                     0x05
#define CHECKPOINT_MINOR_TRAIN_RD_DQS                     0x06
#define CHECKPOINT_MINOR_TRAIN_WR_DQS                     0x07
#define CHECKPOINT_MINOR_TRAIN_RD_VREF                    0x08
#define CHECKPOINT_MINOR_TRAIN_WR_VREF                    0x09
#define CHECKPOINT_MINOR_CROSSOVER_CALIBRATION            0x0A
#define CHECKPOINT_MINOR_SENSE_AMP                        0X0B
#define CHECKPOINT_MINOR_EARLY_CMD_CLK                    0x0C
#define CHECKPOINT_MINOR_PER_BIT_DESKEW_RX                0x0D
#define CHECKPOINT_MINOR_PER_BIT_DESKEW_TX                0x0E
#define CHECKPOINT_MINOR_RX_VREF_TRAINING                 0x0F
#define CHECKPOINT_MINOR_RMT                              0x10
#define CHECKPOINT_MINOR_END_TRAINING                     0x11
#define CHECKPOINT_MINOR_NORMAL_MODE                      0x12
#define CHECKPOINT_MINOR_ROUND_TRIP_OPTIMIZE              0x13
#define CHECKPOINT_MINOR_RX_BACKSIDE_PHASE_TRAINING       0x14
#define CHECKPOINT_MINOR_RX_BACKSIDE_CYCLE_TRAINING       0x15
#define CHECKPOINT_MINOR_RX_BACKSIDE_DELAY_TRAINING       0x16
#define CHECKPOINT_MINOR_TX_BACKSIDE_FINE_WL_TRAINING     0x17
#define CHECKPOINT_MINOR_TX_BACKSIDE_COARSE_WL_TRAINING   0x18
#define CHECKPOINT_MINOR_DISABLE_SENSE_AMP                0x19
#define CHECKPOINT_MINOR_EARLY_CTL_CLK                    0x1A
#define CHECKPOINT_MINOR_TX_EQ_TRAINING                   0x1B
#define CHECKPOINT_MINOR_TX_BACKSIDE_DELAY_TRAINING       0x1C
#define CHECKPOINT_MINOR_IMODE_TRAINING                   0x1D
#define CHECKPOINT_MINOR_TRAIN_WR_VREF_LRDIMM             0x1E
#define CHECKPOINT_MINOR_CMD_VREF_CENTERING               0x24
#define CHECKPOINT_MINOR_DQ_SWIZZLE_DISCOVERY             0x25
#define CHECKPOINT_MINOR_EARLY_RID_FINE                   0x26
#define CHECKPOINT_MINOR_EARLY_RID_COARSE                 0x27
#define CHECKPOINT_MINOR_EARLY_RID_FINAL                  0x28
#define CHECKPOINT_MINOR_MEM_TEST                         0x2B
#define CHECKPOINT_MINOR_PACKAGE_DELAY_CCC                0x2C
#define CHECKPOINT_MINOR_POST_PACKAGE_DELAY_CCC           0x2D
#define CHECKPOINT_MINOR_E_TRAIN_RD_VREF                  0x31
#define CHECKPOINT_MINOR_E_TRAIN_WR_VREF                  0x32
#define CHECKPOINT_MINOR_TRAIN_RD_VREF_LRDIMM             0x33
#define CHECKPOINT_MINOR_TX_LRDIMM_DQ_CENTERING           0x34
#define CHECKPOINT_MINOR_RX_LRDIMM_DQ_CENTERING           0x35
#define CHECKPOINT_MINOR_DFE_PATH_FINDING                 0x36
#define CHECKPOINT_MINOR_WRITE_DFE                        0x37
#define CHECKPOINT_MINOR_READ_DFE                         0x38
#define CHECKPOINT_MINOR_BACKSIDE_SWIZZLE                 0x39

#define CHECKPOINT_MINOR_CLK_TRAINING                     0x3A
#define CHECKPOINT_MINOR_QX_CA_CLK                        0x3B
#define CHECKPOINT_MINOR_PXC_TRAINING                     0x3C
#define CHECKPOINT_MINOR_RCSIM_CACHE_DDRIO                0x3D
#define CHECKPOINT_MINOR_DQS_SWIZZLE_DISCOVERY            0x3E
#define CHECKPOINT_MINOR_PERIODIC_TX_RETRAIN        0x3F

#define CHECKPOINT_MINOR_PPR_FLOW                         0x40
#define CHECKPOINT_MINOR_PPR_PRE_TEST                     0x41
#define CHECKPOINT_MINOR_PPR_SEQ_PREA_BEFORE              0x42
#define CHECKPOINT_MINOR_PPR_SEQ_DIS_DB_SNOOP             0x43
#define CHECKPOINT_MINOR_PPR_SEQ_EN_PPR_MODE              0x44
#define CHECKPOINT_MINOR_PPR_SEQ_EN_GUARD_KEY0            0x45
#define CHECKPOINT_MINOR_PPR_SEQ_EN_GUARD_KEY1            0x46
#define CHECKPOINT_MINOR_PPR_SEQ_EN_GUARD_KEY2            0x47
#define CHECKPOINT_MINOR_PPR_SEQ_EN_GUARD_KEY3            0x48
#define CHECKPOINT_MINOR_PPR_SEQ_WR_PATTERN               0x49
#define CHECKPOINT_MINOR_PPR_SEQ_PREA_AFTER               0x4A
#define CHECKPOINT_MINOR_PPR_SEQ_DIS_PPR_MODE             0x4B
#define CHECKPOINT_MINOR_PPR_SEQ_EN_DB_SNOOP              0x4C
#define CHECKPOINT_MINOR_PPR_SEQ_RESTORE_MR0              0x4D
#define CHECKPOINT_MINOR_PPR_POST_TEST                    0x4E

#define CHECKPOINT_MINOR_LRDIMM_EXTERNAL_WRITE_LEVEL_COARSE   0x60
#define CHECKPOINT_MINOR_LRDIMM_EXTERNAL_WRITE_LEVEL_FINE     0x61
#define CHECKPOINT_MINOR_LRDIMM_INTERNAL_WRITE_LEVEL_COARSE   0x62
#define CHECKPOINT_MINOR_LRDIMM_INTERNAL_WRITE_LEVEL_FINE     0x63
#define CHECKPOINT_MINOR_T_DRAM_RX_EQ                     0x64
#define CHECKPOINT_MINOR_T_TX_EQ                          0x65
#define CHECKPOINT_MINOR_T_IMODE                          0x66
#define CHECKPOINT_MINOR_T_CTLE                           0x67
#define CHECKPOINT_MINOR_T_MC_RON                         0x68
#define CHECKPOINT_MINOR_T_RTT_WR                         0x69
#define CHECKPOINT_MINOR_T_MC_ODT                         0x6A
#define CHECKPOINT_MINOR_T_NT_ODT                         0x6B
#define CHECKPOINT_MINOR_T_DRAM_RON                       0x6C
#define CHECKPOINT_MINOR_T_RX_ODT                         0x6D
#define CHECKPOINT_MINOR_T_TCO_COMP                       0x6E
#define CHECKPOINT_MINOR_T_TX_RISE_FALL_SLEW_RATE         0x6F
#define CHECKPOINT_MINOR_T_CMD_TX_EQ                      0x70
#define CHECKPOINT_MINOR_DDJC_TRAINING                    0x71
#define CHECKPOINT_MINOR_T_RX_DFE                         0x72
#define CHECKPOINT_MINOR_EARLY_CS_CLK                     0x73
#define CHECKPOINT_MINOR_EARLY_CS_CLK_BACKSIDE            0x74
#define CHECKPOINT_MINOR_EARLY_CA_CLK                     0x75
#define CHECKPOINT_MINOR_EARLY_CA_CLK_BACKSIDE            0x76
#define CHECKPOINT_MINOR_WRITE_LEVELING_BASIC_DDR5        0x77
#define CHECKPOINT_MINOR_PRE_TRAINING_INIT                0x78
#define CHECKPOINT_MINOR_RCOMP_STAT_LEG                   0x79
#define CHECKPOINT_MINOR_DISPLAY_RESULTS                  0x7A
#define CHECKPOINT_MINOR_POST_TRAINING_INIT               0x7B
#define CHECKPOINT_MINOR_BIT_SWIZZLE_INSPECTION           0x7C
#define CHECKPOINT_MINOR_EARLY_BCOM_CLK                   0x7D
#define CHECKPOINT_MINOR_PBA_ENUMERATE_ID                 0x7E
#define CHECKPOINT_MINOR_T_TX_ECHO                        0x7F
#define CHECKPOINT_MINOR_LEGACY_RXDFE                     0x80
#define CHECKPOINT_MINOR_REQ_CLK                          0x81
#define CHECKPOINT_MINOR_T_LRDIMM_DB_DFE                  0x82
#define CHECKPOINT_MINOR_RCD_DCA_DCK_DUTY_CYCLE_TRAINING  0x83
#define CHECKPOINT_MINOR_WRITE_DQDQS_PRE_DFE_2D_CENTERING 0x84
#define CHECKPOINT_MINOR_WRITE_DQDQS_POST_DFE_2D_CENTERING 0x85
#define CHECKPOINT_MINOR_CA_SLEW_RATE                      0x86
#define CHECKPOINT_MINOR_DCA_DFE                           0x87
#define CHECKPOINT_MINOR_DDRT2_MODE                        0x8A
#define CHECKPOINT_MINOR_READ_DQDQS_PRE_DFE_2D_CENTERING   0x8B
#define CHECKPOINT_MINOR_READ_DQDQS_POST_DFE_2D_CENTERING  0x8C
#define CHECKPOINT_MINOR_FRONTSIDE_SWIZZLE                 0x8D
#define CHECKPOINT_MINOR_BACKSIDE_RX_PER_BIT_DESKEW        0x8E
#define CHECKPOINT_MINOR_DCA_TCO                           0x8F
#define CHECKPOINT_MINOR_TURNAROUND                        0x90
#define CHECKPOINT_MINOR_LRDIMM_EXTERNAL_WRITE_DELAY       0x91
#define CHECKPOINT_MINOR_DQ_SLEW_RATE                      0x92
#define CHECKPOINT_MINOR_NORMALIZE_CMD                     0x93
#define CHECKPOINT_MINOR_TCO_DQ                            0x94
#define CHECKPOINT_MINOR_DRAM_DUTY_CYCLE_ADJUSTER          0x95
#define CHECKPOINT_MINOR_WRITE_DB_DFE                      0x96
#define CHECKPOINT_MINOR_WRITE_ODT_LATENCY                 0x97

//
// Checkpoint Data codes associated with CHECKPOINT_MINOR_WR_FLY_BY.
// Note that this is not a major or minor checkpoint code.
//

#define CHECKPOINT_DATA_WR_FLY_BY_TRAINING                0x01
#define CHECKPOINT_DATA_WR_FLY_BY_EVALUATE                0x02

//
// Checkpoint Data code associated with CHECKPOINT_MINOR_EARLY_CMD_CLK.
// Note that this is not a major or minor checkpoint code.
//

#define CHECKPOINT_DATA_ENABLE_PARITY                     0x03

//
// Checkpoint Data code associated with CHECKPOINT_MINOR_RMT.
// Note that this is not a major or minor checkpoint code.
//

#define CHECKPOINT_DATA_LATE_CONFIG                       0x01
#define CHECKPOINT_DATA_INIT_THROTTLING                   0x02

//
// Checkpoint Data code associated with CHECKPOINT_MINOR_NORMAL_MODE.
// Note that this is not a major or minor checkpoint code.
//

#define CHECKPOINT_DATA_SYNC                              0x01

//
// Minor Checkpoint Codes associated with CHECKPOINT_MAJOR_DDRIO_INIT.
//

#define CHECKPOINT_MINOR_EARLY_INIT                       0x01
#define CHECKPOINT_MINOR_LATE_INIT                        0x02
#define CHECKPOINT_MINOR_JEDEC_RESET_INIT                 0x04
#define CHECKPOINT_MINOR_DDRT_IO_INIT                     0x05
#define CHECKPOINT_MINOR_TXVREF_CACHE                     0x06
#define CHECKPOINT_MINOR_CLOCK_EN_S3                      0x07
#define CHECKPOINT_MINOR_SENSE_AMP_CAL                    0x08
#define CHECKPOINT_MINOR_CLOCK_EN                         0x09
#define CHECKPOINT_MINOR_BEFORE_DDRT_IO_INIT              0x0A

//
// Minor Checkpoint Codes associated with CHECKPOINT_MAJOR_INIT_THROTTLING.
//

#define CHECKPOINT_MINOR_EARLY_DDR_THERM                  0x01
#define CHECKPOINT_MINOR_MID_DDR_THERM                    0x02
#define CHECKPOINT_MINOR_LATE_DDR_THERM                   0x03

//
// Minor Checkpoint Codes associated with CHECKPOINT_MAJOR_SSA_API_INIT.
//

#define CHECKPOINT_MINOR_SSA_API_INIT_START               0x01
#define CHECKPOINT_MINOR_SSA_API_INIT_EV_LOADER           0x02

//
// Minor Checkpoint Codes associated with CHECKPOINT_MAJOR_DIMM_DETECT.
//

#define CHECKPOINT_MINOR_DIMM_DETECT                      0x00
#define CHECKPOINT_MINOR_DISPATCH_SLAVES                  0x01
#define CHECKPOINT_MINOR_INIT_SMB                         0x02
#define CHECKPOINT_MINOR_GET_SLAVE_DATA                   0x03
#define CHECKPOINT_MINOR_MAILBOX_READY                    0x04

//
// Minor Checkpoint Codes associated with CHECKPOINT_MAJOR_CLOCK_INIT.
//

#define CHECKPOINT_MINOR_SEND_FREQ                        0x01
#define CHECKPOINT_MINOR_SEND_STATUS                      0x02
#define CHECKPOINT_MINOR_SET_FREQ                         0x03
#define CHECKPOINT_MINOR_SET_CLK_VDD                      0x04
#define CHECKPOINT_MINOR_XMP_INIT                         0x05
#define CHECKPOINT_MINOR_DCPMM_ENABLE_RESYNC_FSM          0x06

//
// Minor Checkpoint Codes associated with CHECKPOINT_MAJOR_RANK_DETECT.
//

#define CHECKPOINT_MINOR_SEND_DATA                        0x01

//
// Minor Checkpoint Codes associated with CHECKPOINT_MAJOR_GLOBAL_EARLY.
//

#define CHECKPOINT_MINOR_PROG_TIMINGS                     0x01
#define CHECKPOINT_MINOR_CLOCK_DISABLE                    0x02

//
// Minor Checkpoint Codes associated with CHECKPOINT_MAJOR_MEMBIST.
//

#define CHECKPOINT_MINOR_MT_STEP1                         0x01
#define CHECKPOINT_MINOR_MT_STEP2                         0x02
#define CHECKPOINT_MINOR_MT_STEP3                         0x03
#define CHECKPOINT_MINOR_MT_STEP4                         0x04
#define CHECKPOINT_MINOR_MT_STEP5                         0x05
#define CHECKPOINT_MINOR_MT_STEP6                         0x06
#define CHECKPOINT_MINOR_MT_STEP7                         0x07
#define CHECKPOINT_MINOR_MT_STEP8                         0x08
#define CHECKPOINT_MINOR_MT_STEP9                         0x09
#define CHECKPOINT_MINOR_MT_STEP10                        0x0A
#define CHECKPOINT_MINOR_MT_STEP11                        0x0B
#define CHECKPOINT_MINOR_MT_STEP12                        0x0C
#define CHECKPOINT_MINOR_MT_STEP13                        0x0D
#define CHECKPOINT_MINOR_MT_STEP14                        0x0E
#define CHECKPOINT_MINOR_MT_STEP15                        0x0F
#define CHECKPOINT_MINOR_MT_STEP16                        0x10
#define CHECKPOINT_MINOR_MT_STEP17                        0x11
#define CHECKPOINT_MINOR_MT_STEP18                        0x12
#define CHECKPOINT_MINOR_MT_STEP19                        0x13
#define CHECKPOINT_MINOR_MT_STEP20                        0x14
#define CHECKPOINT_MINOR_MT_STEP21                        0x15

//
// Minor Checkpoint Codes associated with CHECKPOINT_MAJOR_DDR_MEMMAP.
//

#define CHECKPOINT_MINOR_SAD_INTERLEAVE                   0x01
#define CHECKPOINT_MINOR_TAD_INTERLEAVE                   0x02
#define CHECKPOINT_MINOR_SAD_NONINTER                     0x03
#define CHECKPOINT_MINOR_TAD_NONINTER                     0x04
#define CHECKPOINT_MINOR_RANK_INTER                       0x05
#define CHECKPOINT_MINOR_WRITE_SAD                        0x06
#define CHECKPOINT_MINOR_WRITE_TAD                        0x07
#define CHECKPOINT_MINOR_WRITE_RIR                        0x08
#define CHECKPOINT_MINOR_WRITE_SPARE_RANK                 0x0A

//
// Minor Checkpoint Codes associated with CHECKPOINT_MAJOR_RAS_CONFIG.
//

#define CHECKPOINT_MINOR_MIRROR_MODE                      0x01
#define CHECKPOINT_MINOR_SPARE_MODE                       0x02
#define CHECKPOINT_MINOR_DEVICE_TAG                       0x03
#define CHECKPOINT_MINOR_ERR_THRESH                       0x04
#define CHECKPOINT_MINOR_LOCKSTEP_MODE                    0x05
#define CHECKPOINT_MINOR_ADDDC_MIRROR                     0x06

//
// Minor Checkpoint Codes associated with CHECKPOINT_MAJOR_MKTME_EARLY_FLOW.
//
typedef enum {
  CHECKPOINT_MINOR_TME_INIT_FLOW = 0,
  CHECKPOINT_MINOR_TME_CHECK_CAP_ST,
  CHECKPOINT_MINOR_TME_PROGRAM_MSRS_ST,
  CHECKPOINT_MINOR_TME_PROGRAM_MSRS_MT
} CHECKPOINT_MINOR_TME_TYPE;

typedef enum {
  CHECKPOINT_MINOR_MKTME_ENABLE_FLOW = 0,
  CHECKPOINT_MINOR_MKTME_CHECK_CAP_ST,
  CHECKPOINT_MINOR_MKTME_CHECK_CAP_MT,
  CHECKPOINT_MINOR_MKTME_EARLY_FLOW
} CHECKPOINT_MINOR_MKTME_TYPE;
//
// Minor Checkpoint Codes associated with CHECKPOINT_MAJOR_SGX_PREMEM.
//
typedef enum {
  CHECKPOINT_MINOR_SGX_CHECK_CAP_ST = 0,
  CHECKPOINT_MINOR_SGX_CHECK_CAP_MT,
  CHECKPOINT_MINOR_SGX_PREMEM
} CHECKPOINT_MINOR_SGX_TYPE;

//
// Minor Checkpoint Codes associated with CHECKPOINT_MAJOR_HBM.
//
#define CHECKPOINT_MINOR_HBM_PRE_TRAINING                 0x00
#define CHECKPOINT_MINOR_HBM_INIT                         0x01
#define CHECKPOINT_MINOR_HBM_DISCOVERY                    0x02
#define CHECKPOINT_MINOR_HBM_CHECK_POR                    0x03
#define CHECKPOINT_MINOR_HBM_DISPLAY                      0x04
#define CHECKPOINT_MINOR_HBM_MMRC                         0x05
#define CHECKPOINT_MINOR_HBM_EARLY_CONFIG                 0x06
#define CHECKPOINT_MINOR_HBM_TRAINING                     0x07
#define CHECKPOINT_MINOR_HBM_BASIC_FSM                    0x08
#define CHECKPOINT_MINOR_HBM_RX_DQ_DELAY                  0x09
#define CHECKPOINT_MINOR_HBM_ADV_AWORD                    0x0A
#define CHECKPOINT_MINOR_HBM_TX_DS_TCO                    0x0B
#define CHECKPOINT_MINOR_HBM_RX_VREF                      0x0C
#define CHECKPOINT_MINOR_HBM_ADV_RX_DWORD                 0x0D
#define CHECKPOINT_MINOR_HBM_ADV_TX_DWORD                 0x0E
#define CHECKPOINT_MINOR_HBM_RNDTRIP_LAT                  0x0F
#define CHECKPOINT_MINOR_HBM_RX_DELAY_OPT                 0x10
#define CHECKPOINT_MINOR_HBM_THROTTLING                   0x11
#define CHECKPOINT_MINOR_HBM_MEM_TEST                     0x12
#define CHECKPOINT_MINOR_HBM_MEM_INIT                     0x13
#define CHECKPOINT_MINOR_HBM_RAS                          0x14
#define CHECKPOINT_MINOR_HBM_TX_NORM                      0x15
#define CHECKPOINT_MINOR_HBM_BUS_TURNAROUND               0x16
#define CHECKPOINT_MINOR_HBM_POST_TRAINING                0x17
#define CHECKPOINT_MINOR_HBM_LATE_CONFIG                  0x18
#define CHECKPOINT_MINOR_HBM_RMT                          0x19
#define CHECKPOINT_MINOR_HBM_NORMAL_MODE                  0x1A

//
// Minor Checkpoint Codes associated with CHECKPOINT_MAJOR_CMI_CREDIT.
//

#define CHECKPOINT_MINOR_CMI_CREDIT_INIT                  0x00
#define CHECKPOINT_MINOR_CMI_SET_DONE_BITS                0x01

//
// The following does not appear as a check point but is used to
// set currentMemPhase for performance tracking.
//

#define CHECKPOINT_MAJOR_JEDEC_INIT                       0xC0

#endif // #ifndef _MEMORY_CHECKPOINT_CODES_H_
