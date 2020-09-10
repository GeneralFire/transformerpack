/** @file
  Header file containing Voltage Regulator initialization common definitions.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 -2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/
#ifndef _PEI_VR_COMMON_LIB_H_
#define _PEI_VR_COMMON_LIB_H_

#define MIN_VR_INDEX  0x0
#define MAX_VR_INDEX  0x5

//
//  VR mailbox commands
//
#define READ_VR_STRAP_CONFIG_CMD          0x80000018
#define READ_ACDC_LOADLINE_CMD            0x80000118
#define WRITE_ACDC_LOADLINE_CMD           0x80000218
#define READ_PSI_CUTOFF_CMD               0x80001318
#define WRITE_PSI_CUTOFF_CMD              0x80000318
#define READ_IMON_CONFIG_CMD              0x80001418
#define WRITE_IMON_CONFIG_CMD             0x80000418
#define READ_VR_ICC_MAX_CMD               0x80000518
#define WRITE_VR_ICC_MAX_CMD              0x80000618
#define READ_VR_VOLTAGE_LIMIT_CMD         0x80000718
#define WRITE_VR_VOLTAGE_LIMIT_CMD        0x80000818
#define READ_PSYS_PMAX_CMD                0x80000B17
#define WRITE_PSYS_CONFIG_CMD             0x80000918
#define WRITE_PSYS_PMAX_CMD               0x80000B18
#define READ_VR_TDC_CONFIG_CMD            0x80000019
#define WRITE_VR_TDC_CONFIG_CMD           0x8000001A
#define WRITE_VOLTAGE_OPTIMIZATION_CMD    0x8000001F
#define READ_PLL_VOLTAGE_OFFSET_CMD       0x8000002C
#define WRITE_PLL_VOLTAGE_OFFSET_CMD      0x8000002D
#define WRITE_MPS_VR_IMPV8_CMD            0x80000E18
#define WRITE_PS4_EXIT_VR_CMD             0x80000034
#define MPS_VR_CMD_REQUESTED              0x01
#define PS4_EXIT_VR_CMD_REQUESTED         0x02
#define WRITE_VCCIN_MIN_VOLTAGE           0x80000059
#define READ_VR_VENDOR_ID_CMD             0x80000004
#define GET_SVID_REG0                     0x70
#define GET_SVID_REG1                     0x10070
#define GET_SVID_REG2                     0x20070
#define SVID_SETTLED_TIME_9US             0x2328
#define VR_SET_SVID_SETTLED_TIME_CMD      0x80001F18

//
//  VR mailbox commands for Acoustic Noise Mitigation
//
#define WRITE_SVID_SET_VR_SLEW_RATE_CMD           0x80000C18
#define WRITE_SVID_SET_DISABLE_FAST_PKGC_RAMP_CMD 0x80000D18
#define READ_ACOUSTIC_MIGITATION_RANGE            0x80000007
#define WRITE_ACOUSTIC_MIGITATION_RANGE           0x80000008
#define NOISE_MIGITATION_RANGE_MASK               0xFF
#define RAMP_UP_OFFSET                            8
#define RAMP_DOWN_OFFSET                          16

#define VR_ADDRESS_MASK                   0xF
#define VR_ADDRESS_OFFSET                 16
#define AC_LOADLINE_MASK                  0xFFFF
#define DC_LOADLINE_MASK                  0xFFFF0000
#define VR_TDC_ADDRESS_OFFSET             8
#define DC_LOADLINE_OFFSET                16
#define AC_DC_LOADLINE_MAX                6249 ///< 62.49 mOhm max
#define PSI_THRESHOLD_MASK                0x3FF
#define PSI2_THRESHOLD_OFFSET_MASK        0xFFC00
#define PSI3_THRESHOLD_OFFSET_MASK        0x3FF00000
#define PSI2_THRESHOLD_OFFSET             10
#define PSI3_THRESHOLD_OFFSET             20
#define PSI3_ENABLE_OFFSET                30
#define PSI4_ENABLE_OFFSET                31
#define VR_IMON_SLOPE_OFFSET              16
#define VR_IMON_SLOPE_MASK                0xFFFF0000
#define VR_IMON_OFFSET_MASK               0xFFFF
#define VR_VOLTAGE_LIMIT_MASK             0xFFFF
#define VR_PSYS_SLOPE_OFFSET              16
#define VR_PSYS_OFFSET_MASK               0xFFFF
#define VR_TDC_CURRENT_LIMIT_MASK         0x7FFF
#define VR_TDC_TIME_WINDOW_MASK           0x7F
#define VR_TDC_TIME_WINDOW_LOCATION_MASK  0xFE0000
#define VR_TDC_ENABLE_OFFSET              15
#define VR_TDC_TIME_WINDOW_OFFSET         17
#define VR_TDC_IRMS_OFFSET                30
#define VR_TDC_LOCK_OFFSET                31
#define VR_TDC_TIME_WINDOW_MAX            10
#define VR_ICCMAX_MAX                     1023
#define B_VR_UNLIMITED_ICCMAX             BIT31
///
///  VR default override values
///

#endif // _PEI_VR_COMMON_LIB_H_
