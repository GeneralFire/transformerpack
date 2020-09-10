/** @file
  SysRegs.h

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

#ifndef _sysregs_h
#define _sysregs_h

#define PCH_DEV_MIN   16
#define LPC_BUS       0
#define LPC_DEVICE    31
#define P2P_DEVICE    30
#define EHCI1_DEVICE  29
#define HDA_DEVICE    27
#define EHCI2_DEVICE  26
#define EHCI1_FUNC    7
#define EHCI2_FUNC    7
#define LPC_FUNC      0
#define PMC_BUS       0
#define PMC_DEVICE    31
#define PMC_FUNC      2

// ADR related PCH defines, still used by RAS
#define PM_SYNC_MODE             0xD4
#define ADR_EN                   0xF0

//----------------------------------------------------------------------
//Define Dallas DS12C887 Real Time Clock INDEX and DATA registers
//----------------------------------------------------------------------
#define RTC_INDEX_REG       0x70
#define RTC_DATA_REG        0x71

//----------------------------------------------------------------------
//Define Internal Registers for Dallas DS12C887 Real Time Clock
//----------------------------------------------------------------------
#define RTC_SECONDS_REG         0x00    // R/W  Range 0..59
#define RTC_SECONDS_ALARM_REG   0x01    // R/W  Range 0..59
#define RTC_MINUTES_REG         0x02    // R/W  Range 0..59
#define RTC_MINUTES_ALARM_REG   0x03    // R/W  Range 0..59
#define RTC_HOURS_REG           0x04    // R/W  Range 1..12 or 0..23 Bit 7 is AM/PM
#define RTC_HOURS_ALARM_REG     0x05    // R/W  Range 1..12 or 0..23 Bit 7 is AM/PM
#define RTC_DAY_OF_WEEK_REG     0x06    // R/W  Range 1..7
#define RTC_DAY_OF_MONTH_REG    0x07    // R/W  Range 1..31
#define RTC_MONTH_REG           0x08    // R/W  Range 1..12
#define RTC_YEAR_REG            0x09    // R/W  Range 0..99
#define RTC_REG_A_INDEX         0x0a    // R/W[0..6]  R0[7]
#define RTC_REG_B_INDEX         0x0b    // R/W
#define RTC_REG_C_INDEX         0x0c    // RO
#define RTC_REG_D_INDEX         0x0d    // RO

#define RTC_NMI_MASK            0x80    // NMI mask bit.



#endif // _sysregs_h
