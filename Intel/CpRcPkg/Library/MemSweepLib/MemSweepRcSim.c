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


#include "MemSweep.h"

struct baseMargin GenericDelayRcSim[20] = {
    {14, 44},   // S0
    {16, 44},   // S1
    {14, 46},   // S2
    {12, 44},   // S3
    {14, 42},   // S4
    {12, 42},   // S5
    {16, 46},   // S6
    {16, 42},   // S7
    {12, 46},   // S8
    {12, 46},   // S9
    {16, 42},   // S10
    {16, 46},   // S11
    {12, 42},   // S12
    {14, 42},   // S13
    {12, 44},   // S14
    {14, 46},   // S15
    {16, 44},   // S16
    {14, 44},   // S17
    {10, 40},   // S18
    {20, 50},   // S19
};

struct baseMargin TxDqDelayRcSim[20] = {
    {-24, 54},   // S0
    {-26, 54},   // S1
    {-24, 56},   // S2
    {-22, 54},   // S3
    {-24, 52},   // S4
    {-22, 52},   // S5
    {-26, 56},   // S6
    {-26, 52},   // S7
    {-22, 56},   // S8
    {-22, 56},   // S9
    {-26, 52},   // S10
    {-26, 56},   // S11
    {-22, 52},   // S12
    {-24, 52},   // S13
    {-22, 54},   // S14
    {-24, 56},   // S15
    {-26, 54},   // S16
    {-24, 54},   // S17
    {-20, 50},   // S18
    {-30, 50},   // S19
};

struct baseMargin TxRxVrefRcSim[20] = {
    {58, 95},   // S0
    {66, 94},   // S1
    {64, 96},   // S2
    {62, 94},   // S3
    {64, 92},   // S4
    {62, 92},   // S5
    {66, 96},   // S6
    {66, 92},   // S7
    {62, 96},   // S8
    {62, 96},   // S9
    {66, 92},   // S10
    {66, 96},   // S11
    {62, 92},   // S12
    {64, 92},   // S13
    {62, 94},   // S14
    {64, 96},   // S15
    {66, 94},   // S16
    {64, 94},   // S17
    {60, 90},   // S18
    {65, 99},   // S19
};

//
// This is populated on relative offsets,
// not absolute values
//
struct baseMargin CmdCtlRcSim[20] = {
    {-15, 80},   // S0
    {-14, 79},   // S1
    {-13, 78},   // S2
    {-12, 77},   // S3
    {-11, 76},   // S4
    {-16, 81},   // S5
    {-17, 82},   // S6
    {-18, 83},   // S7
    {-19, 84},   // S8
    {-17, 82},   // S9
    {-16, 83},   // S10
    {-15, 84},   // S11
    {-14, 85},   // S12
    {-13, 86},   // S13
    {-17, 82},   // S14
    {-18, 81},   // S15
    {-19, 82},   // S16
    {-20, 83},   // S17
    {-21, 84},   // S18
    {-22, 85},   // S19
};

struct baseMargin RxRelativeRcSim[20] = {
    {-15, 24},   // S0
    {-14, 26},   // S1
    {-13, 24},   // S2
    {-12, 22},   // S3
    {-11, 24},   // S4
    {-16, 22},   // S5
    {-17, 26},   // S6
    {-18, 26},   // S7
    {-19, 22},   // S8
    {-17, 22},   // S9
    {-16, 26},   // S10
    {-15, 26},   // S11
    {-14, 22},   // S12
    {-13, 24},   // S13
    {-17, 22},   // S14
    {-18, 24},   // S15
    {-19, 26},   // S16
    {-20, 24},   // S17
    {-14, 21},   // S18
    {-16, 23},   // S19
};

/*

  Injects passing/failing conditions for different strobes
  The passing/failing is also modulated based on channel/knob/rank

  @param[in]  CommonParametersH   Pointer to the context information for horizontal sweep
  @param[in]  CommonParameters2D  Pointer to the context information for horizontal and vertical sweeps

  @retval   MRC_STATUS_SUCCESS    Results were overriden correctly for RcSim environment

*/
MRC_STATUS
ResultProcessingAdvanceTargetRcSim (
  IN        COMMON_1D_PARAMETERS    *CommonParametersH,
  IN        COMMON_2D_PARAMETERS    *CommonParameters2D    OPTIONAL
)
{

  return MRC_STATUS_SUCCESS;
}

