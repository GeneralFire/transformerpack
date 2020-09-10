/** @file
  MemDramDutyCycleAdjuster.h
  Definitions and prototypes required to execute the DRAM Duty Cycle Adjuster training

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

#ifndef _MEM_DRAM_DUTY_CYCLE_ADJUSTER_H_
#define _MEM_DRAM_DUTY_CYCLE_ADJUSTER_H_

#include "Ddr5Core.h"
#include <Library/MemoryCoreLib.h>
#include <Memory/Ddr5MrRegs.h>
#include <Library/MemoryServicesLib.h>

//
// DRAM Duty Cycle Adjuster Settings limits for QCLK, IBCLK and QBCLK
//
#define  MIN_DRAM_CLOCK_DCA  (-7)
#define  MAX_DRAM_CLOCK_DCA  (7)

//
// MR42.dca_training_assist_mode definitions
//
#define DCA_TRAINING_ASSIST_DISABLE 0
#define DCA_TRAINING_ASSIST_IBCLK   1
#define DCA_TRAINING_ASSIST_ICLK    2

//
// DCA sweep values for per-device registers MR43 and MR44
//
#define DRAM_DCA_SWEEP_START      MIN_DRAM_CLOCK_DCA
#define DRAM_DCA_SWEEP_MAX        MAX_DRAM_CLOCK_DCA
#define DRAM_DCA_SWEEP_STEP_SIZE  1
#define DRAM_DCA_SWEEP_SIZE       ((DRAM_DCA_SWEEP_MAX - DRAM_DCA_SWEEP_START + 1 + (DRAM_DCA_SWEEP_STEP_SIZE - 1)) / DRAM_DCA_SWEEP_STEP_SIZE)
//
// Convert sweep index to DCA value to be programmed
//
#define SWEEP_INDEX_TO_DCA_VALUE(_Index) ((INT16)_Index + DRAM_DCA_SWEEP_START)

//
// The DRAM is aligned to the phase of ICLK or IBCLK
//
#define ICLK_PHASE_ALIGNED    0
#define IBCLK_PHASE_ALIGNED   1

#define MAX_EYE_WIDTH   MAX_INT16
#define MAX_UI          BURST_LENGTH

typedef struct {
  INT16       DcaSettingPerStrobe[MAX_CH][MAX_RANK_CH][MAX_STROBE];
} DCA_SETTING_ALL_STROBE;

typedef struct {
  INT16      EyeWidth[MAX_CH][MAX_RANK_CH][MAX_STROBE];
} EYE_WIDTH_ALL_STROBE;

typedef struct {
  //
  // Track eye width for each Channel, Rank, Device, UI(Chunk)
  // Burst length is 16 for DDR5 but upper 8 and lower 8 chunks are measured together.
  // i.e. EyeWidthAllStrobe[0] is the eye width for chunks 0 and 8
  //
  EYE_WIDTH_ALL_STROBE      EyeWidthAllStrobe[MAX_UI / 2];
} EYE_WIDTH_ALL_UI;

//
// Structure to track CLK phase alignment for each DRAM
// ICLK_PHASE_ALIGNED or IBCLK_PHASE_ALIGNED
//
typedef struct {
  UINT8   AlignedClk[MAX_CH][MAX_RANK_CH];
} CLK_PHASE_ALIGNMENT;

//
// Private structure to pass private data around this module using the
// public CommonParameters pointer
//
typedef struct _DRAM_DCA_TRAINING_CONTEXT {
  COMMON_1D_PARAMETERS        CommonParameters;

  //
  // Type of training to be performed - depends on what the DIMMs support
  //
  DCA_TYPE                    DcaType;
  //
  // Group under test (the DCA control) - DramDcaClk, DramDcaQClk, DramDcaIbClk or DramDcaQbClk
  //
  MRC_GT                      Group;
  //
  // Current socket number
  //
  UINT8                       Socket;
  //
  // Mask of channels that support the training (0 = training supported)
  //
  UINT32                      ChannelMask;
  //
  // Mask of ranks that support the training (0 = training supported)
  //
  UINT32                      RankMask[MAX_CH];
  //
  // Each DRAM is phase aligned to either ICLK or IBCLK
  //
  CLK_PHASE_ALIGNMENT         ClkPhaseAlignment[MAX_STROBE];
  //
  // The eyewidth is measured for only a single UI(Chunk) per test
  //
  UINT32                      UiIndex;
} DRAM_DCA_TRAINING_CONTEXT;



#endif // _MEM_DRAM_DUTY_CYCLE_ADJUSTER_H_
