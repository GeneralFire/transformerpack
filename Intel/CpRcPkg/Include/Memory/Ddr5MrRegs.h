/** @file

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

#ifndef _DDR5_MODE_REGISTERS_H_
#define _DDR5_MODE_REGISTERS_H_

#ifdef USE_LATEST_DRAM_SPEC
#include "Ddr5MrRegs0p87.h"
#else
#include "Ddr5MrRegs0p72.h"
//
// Define registers to unblock builds - these will not be used at run time
//
#define DDR5_MODE_REGISTER_43_REG 0x2b
#define DDR5_MODE_REGISTER_44_REG 0x2c
#endif

//
// DDR5/DDRT2 Read Preamble Settings
//
#define READ_PREAMBLE_1TCK       0
#define READ_PREAMBLE_2TCK       1
#define READ_PREAMBLE_2TCK_DDR4  2
#define READ_PREAMBLE_3TCK       3
#define READ_PREAMBLE_4TCK       4

#define DDR5_MR6_WRITE_RECOVERY_TIME_66NCK        0x3 // 0011B: 66nCK
#define DDR5_MR6_WRITE_RECOVERY_TIME_96NCK        0x8 // 1000B: 96nCK
#define DDR5_MR6_TRTP_12NCK                       0x0 // 0000B: 12nCK
#define DDR5_MR6_TRTP_24NCK                       0x8 // 1000B: 24nCK

#define DDR5_MR0_CAS_LATENCY_RL_22                0x00  // 00000B: 22
#define DDR5_MR0_CAS_LATENCY_RL_24                0x01  // 00001B: 24

#define DDR5_MR0_CAS_LATENCY_RL_MIN               22
#define DDR5_MR0_CAS_LATENCY_RL_MAX               66
#define DDR5_MR0_CAS_LATENCY_RL_STEP              2
#define _CAS_LATENCY_RL_LIMIT(v)                  \
  (((v) > DDR5_MR0_CAS_LATENCY_RL_MAX) ? DDR5_MR0_CAS_LATENCY_RL_MAX : \
  (((v) < DDR5_MR0_CAS_LATENCY_RL_MIN) ? DDR5_MR0_CAS_LATENCY_RL_MIN : (v)))
#define DDR5_MR0_CAS_LATENCY_RL(v)                \
  ((_CAS_LATENCY_RL_LIMIT (v) - DDR5_MR0_CAS_LATENCY_RL_MIN) / DDR5_MR0_CAS_LATENCY_RL_STEP)

//
// Mode Register 42 dca_types_supported value meanings
//
typedef enum {
  DcaNotSupported     = 0,
  DcaSingleOrTwoPhase = 1,
  DcaFourPhase        = 2,
  MaxDcaType
} DCA_TYPE;

//
// DRAM MR25 Read Training Mode Settings
//
#define READ_TRAINING_PATTERN_SERIAL          0
#define READ_TRAINING_PATTERN_LFSR            1
#define READ_PATTERN_OPTION_LFSR              0
#define READ_PATTERN_OPTION_CLOCK             1
#define READ_PATTERN_BURST_MODE_MRR           0
#define READ_PATTERN_BURST_MODE_CONTINUOUS    1

typedef union {
  struct {
    UINT8  PatternFormat: 1;
    UINT8  Lfsr0PatternOption : 1;
    UINT8  Lfsr1PatternOption : 1;
    UINT8  ContinuesBurstMode : 1;
    UINT8  Reserved : 4;
  } Bits;
  UINT8 Data;
} READ_PATTERN_MODE;

#endif // _DDR5_MODE_REGISTERS_H_
