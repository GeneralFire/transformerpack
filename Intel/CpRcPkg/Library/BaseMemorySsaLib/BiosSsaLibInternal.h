/** @file
  BiosSsaLibInternal.h contains macro definitions and functions for BSSA library internal use.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#ifndef  _BIOS_SSA_LIG_INTERNAL_H_
#define  _BIOS_SSA_LIG_INTERNAL_H_

#define MAX_DDRT_BACKSIDE_CCC_RANGE   (3 * 64 + 63)   // Logic value of 3 and Pi value of 63

#ifdef DDR5_SUPPORT

//
// DDR5
// Map the SUB_CH0 32bits lane error status to the LaneMasks[3:0]
// Map the SUB_CH0 8bits ECC lane error status to the LaneMasks[4]
// Map the SUB_CH1 32bits lane error status to the LaneMasks[8:5]
// Map the SUB_CH1 8bits ECC lane error status to the laneMasks[9]
//
#define OFFSET_LOWER_LANES_START    0
#define OFFSET_LOWER_LANES_END      3
#define OFFSET_UPPER_LANES_START    5
#define OFFSET_UPPER_LANES_END      8
#define OFFSET_ECC_LANES            4
#define OFFSET_ECC_SUBCH_B_LANES    9
#define NUM_ECC_BYTE                2

#else

//
// DDR4
// Map lower 32bits lane error status to the LaneMasks[3:0]
// Map upper 32bits lane error status to the LaneMasks[7:4]
// Map 8 bits ECC lane error status to the LaneMasks[8]
//
#define OFFSET_LOWER_LANES_START    0
#define OFFSET_LOWER_LANES_END      3
#define OFFSET_UPPER_LANES_START    4
#define OFFSET_UPPER_LANES_END      7
#define OFFSET_ECC_LANES            8
#define NUM_ECC_BYTE                1

#endif

/**
  Helper function to get DQ and ECC lanes bits mask according to the info from LaneMasks array

  @param[in]      Socket      - Zero based CPU socket number.
  @param[in]      LaneMasks   - Pointer to array of masks of the lanes/strobes that participate to the margin parameter offset limit calculation.
  @param[out]     DqMaskLow   - Pointer to lower 32 bits DQ bit mask for Ddr4, or 32 bits DQ bit mask for Ddr5 sub-ch A.
  @param[out]     DqMaskHigh  - Pointer to higher 32 bits DQ bit mask for Ddr4, or 32 bits DQ bit mask for Ddr5 sub-ch B.
  @param[out]     EccMask     - Pointer to 16 bit Ecc bit mask. Ddr4 only uses lower 8 bits. For Ddr5, lower 8 bits for sub-ch A and higher 8 bits for sub-ch B.

  @retval n/a
**/
VOID
GetDqAndEccMask (
  IN UINT8         Socket,
  IN CONST UINT8   *LaneMasks,
  IN UINT32        *DqMaskLow,
  IN UINT32        *DqMaskHigh,
  IN UINT16        *EccMask
  );

#endif // _BIOS_SSA_LIG_INTERNAL_H_
