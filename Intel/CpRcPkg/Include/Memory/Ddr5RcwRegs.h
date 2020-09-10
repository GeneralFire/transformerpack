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

#if FixedPcdGetBool(PcdMcrSupport)
#include "Ddr5McrRcwRegs0p92.h"
#else
#ifdef USE_LATEST_RCD_SPEC
#include "Ddr5RcwRegs0p85.h"
#else // #ifdef USE_LATEST_RCD_SPEC
#include "Ddr5RcwRegs0p71.h"
#endif // #ifdef USE_LATEST_RCD_SPEC
#endif // FixedPcdGetBool(PcdMcrSupport)

//
// Values for fields in RW02:
//
// - DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_STRUCT.Bits.host_interface_training_mode_selection_ch_a
// - DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_STRUCT.Bits.host_interface_training_mode_selection_ch_b
//
#define TRAINING_MODE_NORMAL_OPERATION    0x0   // 00=Normal operating mode
#define TRAINING_MODE_CLOCK_TO_DCAY_AB    0x1   // 01=Clock-to-DCAy_A/B training mode
#define TRAINING_MODE_DCS0_AB_N           0x2   // 10=DCS0_A/B_n training mode
#define TRAINING_MODE_DCS1_AB_N           0x3   // 11=DCS1_A/B_n training mode
//
// - DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_STRUCT.Bits.dca_training_xor_sampling_edge
//
#define XOR_SAMPLING_EDGE_BOTH            0x0   // 00=Both Rising and Falling Edges
#define XOR_SAMPLING_EDGE_RISING          0x1   // 01=Rising Edge
#define XOR_SAMPLING_EDGE_FALLING         0x2   // 10=Falling Edge

//
// Values for field in RW04:
//
// - DDR5_RCD_COMMAND_SPACE_STRUCT.Bits.command
//
#define RCD_COMMAND_NOP                           0   // CMD 0:  NOP
#define RCD_COMMAND_CH_A_DB_RESET                 1   // CMD 1:  Switches BRST_A_n to active which is LOW. Does NOT reset RCD.
#define RCD_COMMAND_CLEAR_CH_A_DB_RESET           2   // CMD 2:  Switches BRST_A_n to inactive which is HIGH.
#define RCD_COMMAND_CH_B_DB_RESET                 3   // CMD 3:  Switches BRST_B_n to active which is LOW. Does NOT reset RCD.
#define RCD_COMMAND_CLEAR_CH_B_DB_RESET           4   // CMD 4:  Switches BRST_B_n to inactive which is HIGH
#define RCD_COMMAND_CH_A_DRAM_RESET               5   // CMD 5:  Switches QRST_A_n & QCSx_A_n to active which is LOW
#define RCD_COMMAND_CLEAR_CH_A_DRAM_RESET         6   // CMD 6:  Switches QRST_A_n to inactive which is HIGH
#define RCD_COMMAND_CH_B_DRAM_RESET               7   // CMD 7:  Switches QRST_B_n & QCSx_B_n to active which is LOW
#define RCD_COMMAND_CLEAR_CH_B_DRAM_RESET         8   // CMD 8:  Switches QRST_B_n to inactive which is HIGH
#define RCD_COMMAND_CH_A_CLEAR_PARITY_ERROR       9   // CMD 9:  Clear CA Parity Error Status bit and > 1 Error bit and re-enable parity checking (if not already enabled)
#define RCD_COMMAND_CH_B_CLEAR_PARITY_ERROR       10  // CMD 10: Clear CA Parity Error Status bit and > 1 Error bit and re-enable parity checking (if not already enabled)
#define RCD_COMMAND_CH_A_DFE_ERROR_COUNTER_RESET  11  // CMD 11: Resets All of CH_A DFE ERROR Counters to zero
#define RCD_COMMAND_CH_B_DFE_ERROR_COUNTER_RESET  12  // CMD 12: Resets All of CH_B DFE ERROR Counters to zero
#define RCD_COMMAND_ALERT_TOGGLE                  13  // CMD 13: Sends one 6 to 10-cycle LOW pulse on ALERT_n
#define RCD_COMMAND_CH_A_QCS_HIGH                 14  // CMD 14: QCSx_A_n Released
#define RCD_COMMAND_CH_B_QCS_HIGH                 15  // CMD 15: QCSx_B_n Released

#define MAX_RCD_DIRECT_CONTROL_WORDS      0x60
#define MAX_RCD_PAGES                     4
#define MAX_RCD_CONTROL_WORDS_PER_PAGE    0x20
#define PAGED_CONTROL_WORD_INITIAL_OFFSET 0x60

#define NO_PBA             0
//
// CW related macro in DDRDB01 Spec Reev 0.95
//
#define MAX_DB_DIRECT_CONTROL_WORDS       0x60
#define MAX_DB_CONTROL_WORDS_PER_PAGE      0x20
#define DB_PAGED_CONTROL_WORD_INITIAL_OFFSET 0xE0
#define DB_DIRECT_CONTROL_WORD_INITIAL_OFFSET 0x80

#define RCW_PAGE_ID(Address) (((Address) >> 8) & 0xFF)
#define RCW_REG_NUM(Address) ((Address) & 0xFF)

#if FixedPcdGetBool(PcdMcrSupport)
//
// DB Pass through Mode Direction
//
#define DDR5_DB_DQ_PASS_THRU_DIR_WR       0
#define DDR5_DB_DQ_PASS_THRU_DIR_RD       1

//
// MCR specific registers are in page 0x70
//
#define MAX_MCR_PAGES                     1
#define MCR_START_PAGE                    0x70

//
// MCR Operating modes
//
#define MCR_DDR5_STD_MODE                 0x00
#define MCR_DDR5_MODE                     0x02
#define MCR_DDR4_MODE                     0x03

//
// MCR DRAM width
//
#define MCR_x4_DRAM                      0x00
#define MCR_x8_DRAM                      0x01

//
// Pass through mode DQ Mask
//
#define DDR5_MCR_PASS_THRU_DQ_MASK_PS0      0x00
#define DDR5_MCR_PASS_THRU_DQ_MASK_PS1      0xFF
#define DDR5_MCR_PASS_THRU_DQ_MASK_PS0_PS1  0xCC
#endif // FixedPcdGetBool(PcdMcrSupport)

//
// I3C related macro in RCD Spec Rev 0.95
//
// SidebandBus Command Format in I3C Basic Mode
#define RCD_SB_CMD_READ_DWORD  0xC0  // Read DWord
#define RCD_SB_CMD_WRITE_BYTE  0xC4  // Write Byte
#define RCD_SB_CMD_WRITE_DWORD 0xCC  // Write DWord

// Command/Data length to send Read Dword commad to RCD
// Command length 6       // 1 byte sb cmd + 1 byte for cmd length + 4 bytes for cmd
// Data length    6       // 1 byte byte count + 1 byte for Status + 4 bytes for read data
#define RCD_READ_DWORD      6

// Command length to send Write Dword/Byte commad to RCD
#define RCD_WRITE_DWORD_CMD  10     // 1 byte sb cmd + 1 byte for cmd length + 4 bytes for cmd + 4 bytes for write data
#define RCD_WRITE_BYTE_CMD   7      // 1 byte sb cmd + 1 byte for cmd length + 4 bytes for cmd + 1 byte for write data
#define RCD_CMD_DATA_LEN_OFFSET  2  // 1 byte sb cmd + 1 byte for cmd length / 1 byte byte count + 1 byte for Status
