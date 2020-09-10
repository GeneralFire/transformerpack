/** @file
  Interface header file for the HBM library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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

#ifndef _HBM_CORE_COMMON_H_
#define _HBM_CORE_COMMON_H_

//
// Definitions used in HbmCoreDisplay.c
//
#define HBM_DISPLAY_CHANNEL                       0
#define HBM_DISPLAY_STROBE                        1

//
// MRS Access Methods
//
#define IEEE_1500                                 0
#define HBM_CPGC                                  1



//
// Timer flags
//
#define TIMER_IGNORE_CHECK_SIMICS                 BIT0
#define TIMER_IGNORE_CHECK_CTE                    BIT1
//
// SimDelay Units
//
#define TIMER_DCLK_CTE                            0         ///< By default always pick D_CLK.
                                                            ///< Uses "SimDelay"
#define TIMER_QCLK_CTE                            BIT16     ///< Uses "SimDelay"
#define TIMER_US_CTE                              BIT17     ///< Uses "UsDelay", instead of "SimDelay"

#define TIMER_NONE                                0



//
// Mode Parameters
// To use HBM_GSM_FAST_* we need to make sure the CACHE is in sync correctly with the CSR Value
//
#define HBM_GSM_READ_CSR                          BIT0      // Set to force the read from hardware. Clear to read form cache.
#define HBM_GSM_READ_ONLY                         BIT1      // Set to skip the write flow.
#define HBM_GSM_WRITE_OFFSET                      BIT2      // Set to use the "value" input as an offset. Set to use the "value" input as an absolute.
#define HBM_GSM_FORCE_WRITE                       BIT3      // Set to force a write even if the data to be written matches the cached value.
#define HBM_GSM_UPDATE_CACHE                      BIT4      // Set to update cached value with the read or write value
#define HBM_GSM_READ_UC                           (HBM_GSM_READ_CSR | HBM_GSM_READ_ONLY | HBM_GSM_UPDATE_CACHE)
#define HBM_GSM_WRITE_UC_FW                       (HBM_GSM_READ_CSR | HBM_GSM_UPDATE_CACHE | HBM_GSM_FORCE_WRITE)
#define HBM_GSM_FAST_WRITE_UC_FW                  (HBM_GSM_UPDATE_CACHE | HBM_GSM_FORCE_WRITE)
#define HBM_GSM_WRITE_OFFSET_UC_FW                (HBM_GSM_READ_CSR | HBM_GSM_WRITE_OFFSET | HBM_GSM_UPDATE_CACHE | HBM_GSM_FORCE_WRITE)
#define HBM_GSM_FAST_WRITE_OFFSET_UC_FW           (HBM_GSM_WRITE_OFFSET | HBM_GSM_UPDATE_CACHE | HBM_GSM_FORCE_WRITE)
//
// Broadcast
//
#define HBM_ALL_CH                                0xFF
#define HBM_ALL_STROBES                           0xFF

//
// Test Modes
//
#define HBM_MODE_XTALK                            BIT0
#define HBM_MODE_VIC_AGG                          BIT1
#define HBM_MODE_START_ZERO                       BIT2
#define HBM_MODE_POS_ONLY                         BIT3
#define HBM_MODE_NEG_ONLY                         BIT4
#define HBM_MODE_DATA_MPR                         BIT5
#define HBM_MODE_DATA_FIXED                       BIT6
#define HBM_MODE_DATA_LFSR                        BIT7
#define HBM_MODE_ADDR_LFSR                        BIT8
#define HBM_MODE_ADDR_CMD0_SETUP                  BIT9
#define HBM_MODE_CHECK_PARITY                     BIT10
#define HBM_MODE_DESELECT                         BIT11
#define HBM_MODE_VA_DESELECT                      BIT12
#define HBM_MODE_TETHER_PN                        BIT13

//
// Scope of the margin
//
#define HBM_SCOPE_SOCKET                          0
#define HBM_SCOPE_CH                              1
#define HBM_SCOPE_DIMM                            2
#define HBM_SCOPE_RANK                            3
#define HBM_SCOPE_STROBE                          4
#define HBM_SCOPE_BIT                             5

//
// Csr init
//
#define HBM_CSR_NONE_POINT                        0
#define HBM_CSR_MIDDLE_POINT                      1
#define HBM_CSR_MIN_POINT                         2
#define HBM_CSR_MAX_POINT                         3

#endif // _HBM_CORE_COMMON_H_

