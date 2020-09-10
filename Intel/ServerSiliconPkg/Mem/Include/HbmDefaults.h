/** @file

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

#ifndef _HBM_DEFAULTS_H_
#define _HBM_DEFAULTS_H_

//
// HBM Thermal polling interval
//
#define HBM_THERMAL_POLL_INTERVAL       0x187

//
// Page Policy
//
#define HBM_CLOSED_PAGE_DEFAULT         CLOSED_PAGE_DIS

#define HBM_IDLE_PAGE_RST_VAL     0x200
#define HBM_WIN_SIZE              0x20
#define HBM_PPC_TH                6
#define HBM_OPC_TH                6
#define HBM_LINK_LIST_THRESHOLD   0x3F

//
// Scheduler Enables definition
//
#define HBM_SCHEDULER_ENABLES_8_BANKS          0
#define HBM_SCHEDULER_ENABLES_16_BANKS         1
#define HBM_SCHEDULER_ENABLES_32_BANKS         2
#define HBM_SCHEDULER_ENABLES_RESERVED_BANKS   3

//
// HBM Scheduler RMM
//
#define HBM_RMM_OPP_TIMER               0xF
#define HBM_RMM_OPP_WRITE_LIMIT         0xF
#define HBM_RMM_OPP_TIMER_GRAN          2
#define HBM_RMM_OPP_WRITE_LIMIT_GRAN    0

//
// HBM Scheduler WMM
//
#define HBM_WMM_EXIT_THRESHOLD          0xE
#define HBM_WMM_ENTER_THRESHOLD         0x1B
#define HBM_WMM_OPP_TIMER               0xF
#define HBM_WMM_OPP_READ_LIMIT          0xF
#define HBM_WMM_OPP_TIMER_GRAN          2
#define HBM_WMM_OPP_READ_LIMIT_GRAN     0

//
// HBM Read Starve
//
#define HBM_READ_WAIT_COUNT             0x40
#define HBM_READ_STARVE_COUNT           0x80

//
// HBM Write Starve
//
#define HBM_WRITE_WAIT_COUNT            0x80
#define HBM_WRITE_STARVE_COUNT          0x300

//
// HBM Command Starvation
//
#define HBM_CMD_STARVED_COUNT           0x8
#define HBM_CMD_CRITICAL_COUNT          0x8

//
// HBM Rid
//
#define HBM_ENABLE_RID                  0xFFFFFF
#define HBM_RID_VC2_LOW_THRESHOLD       0

//
// HBM RPQ/WPG PCH0/PCH1
//
#define HBM_RPQ_PCH0_ENTRY_ENABLES      0xFFFFFFFF
#define HBM_RPQ_PCH1_ENTRY_ENABLES      0xFFFFFFFF
#define HBM_WPQ_PCH0_ENTRY_ENABLES      0xFFFFFFFF
#define HBM_WPQ_PCH1_ENTRY_ENABLES      0xFFFFFFFF

//
// HBMIO Read Latency (nCK)
//
#define HBM_MIN_READ_LATENCY_NCK        5
#define HBM_READ_LATENCY_NCK_DEFAULT    8

//
// HBMIO Write Latency (nCK)
//
#define HBM_MIN_WRITE_LATENCY_NCK       4
#define HBM_WRITE_LATENCY_NCK_DEFAULT   4

//
// HBMIO Parity Latency (nCK)
//
#define HBM_PARITY_LATENCY_NCK_DEFAULT  0

//
// HBM refresh-to-refresh commands separation for per-bank refresh
//
#define HBM_T_BANK_STAGGER_DEFAULT      10

//
// HBM minimum time between refresh or refresh per-bank commands on the channel
//
#define HBM_T_STAGGER_REFRESH_DEFAULT   2

//
// DBI Mode
//
#define HBM_DBI_DIS                     0       // Disable DBI Mode
#define HBM_DBI_EN                      1       // Enable DBI Mode
#define HBM_DBI_DEFAULT                 HBM_DBI_EN

//
// Relaxed turnaround
//
#define HBM_RELAXED_TA_DIS              0       // Disable relaxed timing settings for turnaround
#define HBM_RELAXED_TA_EN               1       // Enable relaxed timing settings for turnaround
#define HBM_RELAXED_TA_DEFAULT          HBM_RELAXED_TA_DIS

//
// Per bank refresh mode
//
#define HBM_ALL_BANK_REFRESH            0
#define HBM_PER_BANK_REFRESH            1
#define HBM_REFRESH_MODE_DEFAULT        HBM_ALL_BANK_REFRESH

//
// Self Refresh Programming
//
#define HBM_SREF_PROG_DEFAULT           SREF_PROG_AUTO

//
// Self refresh idle timer
//
#define HBM_SREF_IDLE_TIMER_DEFAULT     20

//
// Opportunistic self refresh
//
#define HBM_OPP_SREF_DEFAULT            OPP_SREF_DISABLE

//
// PkgC Self refresh
//
#define HBM_PKGC_SREF_DEFAULT           PKGC_SREF_DISABLE

#endif // _HBM_DEFAULTS_H_
